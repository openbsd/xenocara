/* $OpenBSD: privsep.c,v 1.28 2015/11/11 21:07:49 matthieu Exp $ */
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
#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

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

#include "os.h"
#include "osdep.h"

enum cmd_types {
	PRIV_OPEN_DEVICE,
	PRIV_SIG_PARENT,
};

/* Command parameters */
typedef struct priv_cmd {
	int cmd;
	int _pad;
	union {
		struct _open {
			char path[MAXPATHLEN];
		} open;
	} arg;
} priv_cmd_t;


static int priv_fd = -1;
static pid_t parent_pid = -1;
struct okdev {
	const char *const name;
	int flags;
} allowed_devices[] = {
	/* Serial devices */
	{"/dev/wsmouse", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/wsmouse0", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/wsmouse1", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/wsmouse2", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/wsmouse3", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/uhid0", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/uhid1", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/uhid2", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/uhid3", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/tty00", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/tty01", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/tty02", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/tty03", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/tty04", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttya", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyb", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyc", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyd", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/wskbd", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/wskbd0", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/wskbd1", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/wskbd2", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/wskbd3", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyC0", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyC1", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyC2", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyC3", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyC4", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyC5", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyC6", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyC7", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyD0", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyE0", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyF0", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyG0", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyH0", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyI0", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/ttyJ0", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/pci", O_RDWR | O_NONBLOCK | O_EXCL},
	{"/dev/drm0", O_RDWR },
	{"/dev/drm1", O_RDWR },
	{"/dev/drm2", O_RDWR },
	{"/dev/drm3", O_RDWR },
	{"/dev/amdmsr", O_RDWR | O_EXCL},
	{NULL, 0}
};

/* return 1 if allowed to open said path */
static struct okdev *
open_ok(const char *path)
{
	struct okdev *p;
	struct stat sb;

	for (p = allowed_devices; p->name != NULL; p++) {
		if (strcmp(path, p->name) == 0) {
			if (stat(path, &sb) < 0) {
				/* path is valid, but doesn't exist */
				return NULL;
			}
			if (sb.st_mode & S_IFCHR) {
				/* File is a character device */
				return p;
			}
		}
	}
	/* path is not valid */
	return NULL;
}

static void
send_fd(int s, int fd)
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

	if ((n = sendmsg(s, &msg, 0)) == -1)
		warn("%s: sendmsg(%d)", __func__, s);
	if (n != sizeof(int))
		warnx("%s: sendmsg: expected sent 1 got %ld",
		    __func__, (long)n);
}

static int
receive_fd(int s)
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

	do
		n = recvmsg(s, &msg, 0);
	while (n == -1 && errno == EINTR);

	if (n != sizeof(int)) {
		warnx("%s: recvmsg: expected received 1 got %ld",
		    __func__, (long)n);
		return -1;
	}
	if (result == 0) {
		cmsg = CMSG_FIRSTHDR(&msg);
		if (cmsg == NULL) {
			warnx("%s: no message header", __func__);
			return -1;
		}
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
	int i, fd;
	pid_t pid;
	int socks[2];
	priv_cmd_t cmd;
	struct okdev *dev;

	parent_pid = getppid();

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
		if (setegid(gid) == -1)
			return -1;
		if (setgid(gid) == -1)
			return -1;
		if (seteuid(uid) == -1)
			return -1;
		if (setuid(uid) == -1)
			return -1;
		close(socks[0]);
		priv_fd = socks[1];
		return 0;
	}
	/* son */
	for (i = 1; i <= _NSIG; i++)
		signal(i, SIG_DFL);
	setproctitle("[priv]");
	close(socks[1]);

	if (pledge("stdio rpath wpath sendfd proc", NULL) == -1)
		err(1, "pledge");

	while (1) {
		if (read(socks[0], &cmd, sizeof(cmd)) == 0) {
			exit(0);
		}
		switch (cmd.cmd) {

		case PRIV_OPEN_DEVICE:
			if ((dev = open_ok(cmd.arg.open.path)) != NULL) {
				fd = open(cmd.arg.open.path, dev->flags);
			} else {
				fd = -1;
				errno = EPERM;
			}
			send_fd(socks[0], fd);
			if (fd >= 0)
				close(fd);
			break;
		case PRIV_SIG_PARENT:
			if (parent_pid > 1)
				kill(parent_pid, SIGUSR1);
			break;
		default:
			errx(1, "%s: unknown command %d", __func__, cmd.cmd);
			break;
		}
	}
	_exit(1);
}

/* Open file */
int
priv_open_device(const char *path)
{
	priv_cmd_t cmd;
	struct okdev *dev;

	if (priv_fd != -1) {
		cmd.cmd = PRIV_OPEN_DEVICE;
		strlcpy(cmd.arg.open.path, path, MAXPATHLEN);
		write(priv_fd, &cmd, sizeof(cmd));
		return receive_fd(priv_fd);
	} else {
		if ((dev = open_ok(path)) != NULL)
			return open(path, dev->flags);
		else {
			errno = EPERM;
			return -1;
		}
	}
}

void
priv_init_parent_process(pid_t ppid)
{
	parent_pid = ppid;
}

/* send signal to parent process */
void
priv_signal_parent(void)
{
	priv_cmd_t cmd;

	if (priv_fd != -1) {
		if (parent_pid == -1)
			warnx("parent_pid == -1\n");
		cmd.cmd = PRIV_SIG_PARENT;
		write(priv_fd, &cmd, sizeof(cmd));
	} else
		if (parent_pid > 1)
			kill(parent_pid, SIGUSR1);
}

#ifdef TEST
/* This is not a complete regression test */
int
main(int argc, char *argv[])
{
	int fd;

	if (priv_init(getuid(), getgid()) < 0) {
		err(1, "priv_init");
	}
	fd = priv_open_device("/dev/wsmouse");
	if (fd < 0) {
		err(1, "priv_open_device");
	}
	write(fd, "test\n", 5);
	close(fd);
	exit(0);
}
#endif
