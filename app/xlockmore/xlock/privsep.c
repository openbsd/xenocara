/* $OpenBSD: privsep.c,v 1.4 2024/01/22 10:13:34 claudio Exp $ */
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
	imsg_close(ibuf, wbuf);

	if ((n = msgbuf_write(&ibuf->w)) == -1 && errno != EAGAIN) {
		warn("imsg_write");
		return -1;
	}
	if (n == 0)
		return -1;
	return 0;
}

static char *
ibuf_get_string(struct ibuf *buf, size_t len)
{
	char *str;

	if (ibuf_size(buf) < len) {
		errno = EBADMSG;
		return (NULL);
	}
	str = strndup(ibuf_data(buf), len);
	if (str == NULL)
		return (NULL);
	buf->rpos += len;
	return (str);
}

static int
receive_cmd(struct imsgbuf *ibuf, char **name, char **pass, char **style)
{
	struct imsg imsg;
	struct ibuf buf;
	struct priv_cmd_hdr hdr;
	ssize_t n, nread;

	do {
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
	} while (n == 0);
	if (imsg_get_type(&imsg) != XLOCK_CHECKPW_CMD) {
		warnx("invalid command");
		goto fail;
	}
	if (imsg_get_ibuf(&imsg, &buf) == -1 ||
	    ibuf_get(&buf, &hdr, sizeof(hdr)) == -1 ||
	    (*name = ibuf_get_string(&buf, hdr.namelen)) == NULL ||
	    (*pass = ibuf_get_string(&buf, hdr.passlen)) == NULL) {
		warn("truncated message");
		goto fail;
	}
	if (hdr.stylelen != 0) {
		if ((*style = ibuf_get_string(&buf, hdr.stylelen)) == NULL) {
			warn("truncated message");
			goto fail;
		}
	}
	imsg_free(&imsg);
	return 0;
fail:
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
	ssize_t n, nread;
	int retval = 0;
	struct imsg imsg;

	do {
		if ((nread = imsg_read(ibuf)) == -1 && errno != EAGAIN)
			return -1;
		if (nread == 0)
			return -1;
		if ((n = imsg_get(ibuf, &imsg)) == -1)
			return -1;
	} while (n == 0);
	if (imsg_get_type(&imsg) != XLOCK_CHECKPW_RESULT ||
	    imsg_get_data(&imsg, presult, sizeof(*presult)) == -1)
		retval = -1;
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
		user = pass = style = NULL;
		if (receive_cmd(&child_ibuf, &user, &pass, &style) == -1) {
			warn("receive_cmd");
			result = 0;
		} else
			result = pw_check(user, pass, style);
		if (user != NULL)
			freezero(user, strlen(user) + 1);
		if (pass != NULL)
			freezero(pass, strlen(pass) + 1);
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
