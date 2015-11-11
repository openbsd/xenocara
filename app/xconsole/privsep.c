/* $OpenBSD: privsep.c,v 1.5 2015/11/11 21:18:13 matthieu Exp $ */
/*
 * Copyright 2001 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Copyright (c) 2002 Matthieu Herrb
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifdef HAVE_UTIL_H
#include <util.h>
#endif
#ifdef HAVE_PTY_H
#inlude <pty.h>
#endif

enum cmd_types {
	PRIV_OPEN_PTY,
	PRIV_REDIRECT_CONSOLE,
};


static int priv_fd = -1;

static void
send_fd(int socket, int fd)
{
	struct msghdr msg;
	union {
		struct cmsghdr hdr;
		char buf[CMSG_SPACE(sizeof(int))];
	} cmsgbuf;
	struct cmsghdr *cmsg;
	struct iovec vec;
	int result = 0;
	ssize_t n;

	memset(&msg, 0, sizeof(msg));

	if (fd >= 0) {
		msg.msg_control = &cmsgbuf.buf;
		msg.msg_controllen = sizeof(cmsgbuf.buf);
		cmsg = CMSG_FIRSTHDR(&msg);
		cmsg->cmsg_len = CMSG_LEN(sizeof(int));
		cmsg->cmsg_level = SOL_SOCKET;
		cmsg->cmsg_type = SCM_RIGHTS;
		*(int *)CMSG_DATA(cmsg) = fd;
	} else {
		result = errno;
	}

	vec.iov_base = &result;
	vec.iov_len = sizeof(int);
	msg.msg_iov = &vec;
	msg.msg_iovlen = 1;
	
	if ((n = sendmsg(socket, &msg, 0)) == -1)
		warn("%s: sendmsg(%d)", __func__, socket);
	if (n != sizeof(int))
		warnx("%s: sendmsg: expected sent 1 got %ld",
		    __func__, (long)n);
}

static int
receive_fd(int socket)
{
	struct msghdr msg;
	union {
		struct cmsghdr hdr;
		char buf[CMSG_SPACE(sizeof(int))];
	} cmsgbuf;
	struct cmsghdr *cmsg;
	struct iovec vec;
	ssize_t n;
	int result;
	int fd;

	memset(&msg, 0, sizeof(msg));
	vec.iov_base = &result;
	vec.iov_len = sizeof(int);
	msg.msg_iov = &vec;
	msg.msg_iovlen = 1;
	msg.msg_control = &cmsgbuf.buf;
	msg.msg_controllen = sizeof(cmsgbuf.buf);

	if ((n = recvmsg(socket, &msg, 0)) == -1)
		warn("%s: recvmsg", __func__);
	if (n != sizeof(int))
		warnx("%s: recvmsg: expected received 1 got %ld",
		    __func__, (long)n);
	if (result == 0) {
		cmsg = CMSG_FIRSTHDR(&msg);
		if (cmsg->cmsg_type != SCM_RIGHTS)
			warnx("%s: expected type %d got %d", __func__,
			    SCM_RIGHTS, cmsg->cmsg_type);
		fd = (*(int *)CMSG_DATA(cmsg));
		return fd;
	} else {
		errno = result;
		return -1;
	}
}

int
priv_init(uid_t uid, gid_t gid)
{
	int i, on, result, fd;
	int pty, tty;
	pid_t pid;
	int socks[2];
	int cmd;

	/* Create sockets */
	if (socketpair(AF_LOCAL, SOCK_STREAM, PF_UNSPEC, socks) == -1) {
		return -1;
	}
	pid = fork();
	if (pid < 0) {
		/* can't fork */
		return -1;
	}
	if (pid != 0) {
		/* Father - drop privileges and return */
		if (setgroups(1, &gid) == -1)
			return -1;
		
		if (setresgid(gid, gid, gid) == -1)
			return -1;
		if (setresuid(uid, uid, uid) == -1)
			return -1;
		close(socks[0]);
		priv_fd = socks[1];
		return 0;
	}
	/* son */
	for (i = 1; i <= _NSIG; i++) 
		signal(i, SIG_DFL);
#ifdef HAVE_SETPROCTILE
	setproctitle("[priv]");
#endif
	close(socks[1]);

	while (1) {
		if (read(socks[0], &cmd, sizeof(int)) == 0) {
			exit(0);
		}
		switch (cmd) {

		case PRIV_OPEN_PTY:
			if (openpty(&pty, &tty, NULL, NULL, NULL) == -1) {
				warn("%s: openpty", __func__);
				pty = -1;
				tty = -1;
			}
			send_fd(socks[0], pty);
			send_fd(socks[0], tty);
			if (pty != -1) 
				close(pty);
			if (tty != -1) 
				close(tty);
			break;
		case PRIV_REDIRECT_CONSOLE:
			on = 1;
			fd = receive_fd(socks[0]);
			result = ioctl(fd, TIOCCONS, (char *) &on);
			if (result < 0) {
				warn("%s: ioctl(TIOCCONS)", __func__);
			}
			write(socks[0], &result, sizeof(int));
			close(fd);
			break;
		default:
			errx(1, "%s: unknown command %d", __func__, cmd);
			break;
		}
	}
	_exit(1);
}

/* Open pseudo-tty */
int
priv_openpty(int *pty_ptr, int *tty_ptr)
{
	int cmd;
	int pty, tty;
	
	if (priv_fd != -1) {
		cmd = PRIV_OPEN_PTY;
		write(priv_fd, &cmd, sizeof(int));
		pty = receive_fd(priv_fd);
		tty = receive_fd(priv_fd);
		if (tty < 0 || pty < 0) {
			fprintf(stderr, "openpty: %d %d %d\n", 
			    pty, tty, errno);
			return -1;
		}
		if (pty_ptr != NULL) 
			*pty_ptr = pty;
		if (tty_ptr != NULL) 
			*tty_ptr = tty;
		return 0;
	} else 
		return openpty(pty_ptr, tty_ptr, NULL, NULL, NULL);
}

/* Redirect console output */
int
priv_set_console(int fd)
{
	int cmd;
	int on, result;

	if (priv_fd != -1) {
		cmd = PRIV_REDIRECT_CONSOLE;
		write(priv_fd, &cmd, sizeof(int));
		send_fd(priv_fd, fd);
		read(priv_fd, &result, sizeof(int));
		return result;
	} else {
		on = 1;
		return ioctl(fd, TIOCCONS, &on);
	}
}
