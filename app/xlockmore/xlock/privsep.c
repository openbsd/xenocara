/* $OpenBSD: privsep.c,v 1.3 2023/12/14 09:44:15 claudio Exp $ */
/*
 * Copyright 2001 Niels Provos <provos@citi.umich.edu>
 * All rights reserved.
 *
 * Copyright (c) 2021 Matthieu Herrb
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above
 *	copyright notice, this list of conditions and the following
 *	disclaimer in the documentation and/or other materials provided
 *	with the distribution.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef USE_PRIVSEP
#include <sys/queue.h>
#include <sys/socket.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <imsg.h>
#include <paths.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include <login_cap.h>
#include <bsd_auth.h>


/* Command */
enum xlock_cmd {
	XLOCK_CHECKPW_CMD,
	XLOCK_CHECKPW_RESULT
};

struct priv_cmd_hdr {
	size_t namelen;
	size_t passlen;
	size_t stylelen;
};

static struct imsgbuf parent_ibuf, child_ibuf;
static int priv_inited = 0;

static int
pw_check(char *name, char *pass, char *style)
{
	int authok;

	authok = auth_userokay(name, style, "auth-xlock", pass) ||
		auth_userokay("root", style, "auth-xlock", pass);
	if (authok)
		syslog(LOG_NOTICE, "%s: %s unlocked screen", "xlock", name);
	return authok;
}

static int
send_cmd(struct imsgbuf *ibuf, char *user, char *pass, char *style)
{
	size_t n, datalen = 0;
	struct ibuf *wbuf;
	struct priv_cmd_hdr hdr;

	memset(&hdr, 0, sizeof(struct priv_cmd_hdr));
	hdr.namelen = strlen(user) + 1;
	hdr.passlen = strlen(pass) + 1;
	if (style != NULL)
		hdr.stylelen = strlen(style) + 1;

	datalen = sizeof(struct priv_cmd_hdr) + hdr.namelen +
		hdr.passlen + hdr.stylelen;

	wbuf = imsg_create(ibuf, XLOCK_CHECKPW_CMD, 0, 0, datalen);
	if (wbuf == NULL) {
		warn("imsg_create");
		return -1;
	}
	if (imsg_add(wbuf, &hdr, sizeof(struct priv_cmd_hdr)) == -1) {
		warn("imsg_add");
		return -1;
	}
	if (imsg_add(wbuf, user, hdr.namelen) == -1) {
		warn("imsg_add");
		return -1;
	}
	if (imsg_add(wbuf, pass, hdr.passlen) == -1) {
		warn("imsg_add");
		return -1;
	}
	if (style != NULL)
		if (imsg_add(wbuf, style, hdr.stylelen) == -1) {
			warn("imsg_add");
			return -1;
		}
	wbuf->fd = -1;
	imsg_close(ibuf, wbuf);

	if ((n = msgbuf_write(&ibuf->w)) == -1 && errno != EAGAIN) {
		warn("imsg_write");
		return -1;
	}
	if (n == 0)
		return -1;
	return 0;
}

static int
receive_cmd(struct imsgbuf *ibuf, char **name, char **pass, char **style)
{
	struct imsg imsg;
	struct priv_cmd_hdr hdr;
	ssize_t n, nread, datalen;
	void *data;

	if ((nread = imsg_read(ibuf)) == -1 && errno != EAGAIN) {
		warn("imsg_read");
		return -1;
	}
	if (nread == 0) {
		/* parent exited */
		exit(0);
	}
	if ((n = imsg_get(ibuf, &imsg)) == -1) {
		warnx("imsg_get");
		return -1;
	}
	if (imsg.hdr.type != XLOCK_CHECKPW_CMD) {
		warnx("invalid command");
		imsg_free(&imsg);
		return -1;
	}
	datalen = imsg.hdr.len - IMSG_HEADER_SIZE;
	data = imsg.data;
	if (datalen < sizeof(struct priv_cmd_hdr)) {
		warnx("truncated header");
		imsg_free(&imsg);
		return -1;
	}
	memcpy(&hdr, data, sizeof(struct priv_cmd_hdr));
	if (datalen != sizeof(hdr) + hdr.namelen + hdr.passlen + hdr.stylelen) {
		warnx("truncated strings");
		imsg_free(&imsg);
		return -1;
	}
	data += sizeof(struct priv_cmd_hdr);
	*name = strndup(data, hdr.namelen);
	if (*name == NULL)
		goto nomem;
	data += hdr.namelen;
	*pass = strndup(data, hdr.passlen);
	if (*pass == NULL)
		goto  nomem;
	data += hdr.passlen;
	if (hdr.stylelen != 0) {
		*style = strndup(data, hdr.stylelen);
		if (*style == NULL)
			goto nomem;
	} else
		*style = NULL;
	imsg_free(&imsg);
	return 0;
nomem:
	warn("strndup");
	imsg_free(&imsg);
	return -1;
}

static int
send_result(struct imsgbuf *ibuf, int result)
{
	imsg_compose(ibuf, XLOCK_CHECKPW_RESULT, 0, 0, -1,
		     &result, sizeof(int));
	return msgbuf_write(&ibuf->w);
}

static int
receive_result(struct imsgbuf *ibuf, int *presult)
{
	ssize_t nread, n;
	int retval = 0;
	struct imsg imsg;

	if ((nread = imsg_read(ibuf)) == -1 && errno != EAGAIN)
		return -1;
	if (nread == 0)
		return -1;
	if ((n = imsg_get(ibuf, &imsg)) == -1)
		return -1;
	if (imsg.hdr.len - IMSG_HEADER_SIZE != sizeof(int))
		retval = -1;
	if (imsg.hdr.type != XLOCK_CHECKPW_RESULT)
		retval = -1;
	memcpy(presult, imsg.data, sizeof(int));
	imsg_free(&imsg);
	return retval;
}

int
priv_init(gid_t gid)
{
	pid_t pid;
	int socks[2], result;
	char *user, *pass, *style;

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
		/* parent - drop setgid privileges and return */
		if (gid != -1) {
			if (setresgid(gid, gid, gid) == -1)
				return -1;
		}
		close(socks[0]);
		imsg_init(&parent_ibuf, socks[1]);
		priv_inited = 1;
		return 0;
	}
	/* child */
	close(socks[1]);

	setproctitle("[priv]");

	imsg_init(&child_ibuf, socks[0]);

	if (unveil(_PATH_LOGIN_CONF, "r") == -1)
		err(1, "unveil %s", _PATH_LOGIN_CONF);
	if (unveil(_PATH_LOGIN_CONF ".db", "r") == -1)
		err(1, "unveil %s.db", _PATH_LOGIN_CONF);
	if (unveil(_PATH_LOGIN_CONF_D, "r") == -1)
		err(1, "unveil %s", _PATH_LOGIN_CONF_D);
	if (unveil(_PATH_AUTHPROGDIR, "rx") == -1)
		err(1, "unveil %s", _PATH_AUTHPROGDIR);
	if (pledge("stdio rpath getpw proc exec", NULL) == -1)
		err(1, "pledge");

	while (1) {
		if (receive_cmd(&child_ibuf, &user, &pass, &style) == -1) {
			warn("receive_cmd");
			result = 0;
		} else
			result = pw_check(user, pass, style);
		freezero(user, strlen(user));
		freezero(pass, strlen(pass));
		free(style);
		if (send_result(&child_ibuf, result) == -1)
			warn("send_result");
	}
}

int
priv_pw_check(char *user, char *pass, char *style)
{
	int result;

	if (priv_inited != 0) {
		if (send_cmd(&parent_ibuf, user, pass, style) == -1) {
			warn("send_cmd");
			return 0;
		}
		if (receive_result(&parent_ibuf, &result) == -1) {
			warn("receive_result");
			return 0;
		}
		return (result);
	} else
		return pw_check(user, pass, style);
}
#endif
