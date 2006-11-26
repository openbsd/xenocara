/* $OpenBSD: policy.c,v 1.1.1.1 2006/11/26 10:58:43 matthieu Exp $ */
/*
 * Copyright (c) 2002 Matthieu Herrb and Niels Provos
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

#include <sys/param.h>

#include <ctype.h>
#include <err.h>
#include <libgen.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "policy.h"

#define MAX_SYSCALLARGS	10

static char home[MAXPATHLEN];		/* Home directory of user */
static char username[MAXLOGNAME];	/* Username: predicate match and expansion */

static struct policy_list *
alloc_policy(char *p)
{
	struct policy_list *np;

	np = (struct policy_list *)malloc(sizeof(struct policy_list));
	if (np == NULL)
		errx(1, "alloc_policy: cannot allocate memory");
	np->line = strdup(p);
	if (np->line == NULL)
		errx(1, "alloc_policy: cannot allocate memory");
	return (np);
}

void
free_policy(struct policy_list *p)
{
	free(p->line);
	free(p);
}

static char *
strrpl(char *str, size_t size, char *match, char *value)
{
	char *p, *e;
	int len, rlen;

	p = str;
	e = p + strlen(p);
	len = strlen(match);

	/* Try to match against the variable */
	while ((p = strchr(p, match[0])) != NULL) {
		if (!strncmp(p, match, len) && !isalnum(p[len]))
			break;
		p += len;

		if (p >= e)
			return (NULL);
	}

	if (p == NULL)
		return (NULL);

	rlen = strlen(value);

	if (strlen(str) - len + rlen > size)
		return (NULL);

	memmove(p + rlen, p + len, strlen(p + len) + 1);
	memcpy(p, value, rlen);

	return (p);
}

void
parameters(void)
{
	struct passwd *pw;
	uid_t uid = getuid();

	/* Find out current username. */
	if ((pw = getpwuid(uid)) == NULL)
		snprintf(username, sizeof(username), "uid %u", uid);
	else
		snprintf(username, sizeof(username), "%s", pw->pw_name);

	strlcpy(home, pw->pw_dir, sizeof(home));
}

static int
make_policy(char **type, char **data, int count, char **presult, int derive)
{
	static char result[4096];
	char one[2048];
	int i;
	int nfilename, isfilename;

	result[0] = '\0';
	nfilename = 0;
	for (i = 0; i < count; i++) {
		isfilename = 0;
		/* Special case for non existing filenames */
		if (strstr(data[i], "<non-existent filename>") != NULL) {
			snprintf(result, sizeof(result),
			    "filename%s sub \"<non-existent filename>\" then deny[enoent]", i ? "[1]" : "");
			break;
		}

		if (!strcmp(type[i], "uid") || !strcmp(type[i], "gid") ||
		    !strcmp(type[i], "argv"))
			continue;

		/* Special case for system calls with more than one filename */
		if (!strcmp(type[i], "filename")) {
			isfilename = 1;
			nfilename++;
		}

		if (strlen(result)) {
			if (strlcat(result, " and ",
				sizeof(result)) >= sizeof(result))
				return (-1);
		}

		/* Special treatment for filenames */
		if (isfilename) {
			char filename[2048];
			char *operator = "eq";

			if (derive) {
				operator = "match";

				snprintf(filename, sizeof(filename),
				    "%s/*", dirname(data[i]));
			} else
				strlcpy(filename, data[i], sizeof(filename));

			/* Make useful replacements */
			while (strrpl(filename, sizeof(filename),
				   home, "$HOME") != NULL)
				;
			while (strrpl(filename, sizeof(filename),
				   username, "$USER") != NULL)
				;

			snprintf(one, sizeof(one), "%s%s %s \"%s\"",
			    type[i], isfilename && nfilename == 2 ? "[1]" : "",
			    operator, filename);
		} else {
			snprintf(one, sizeof(one), "%s eq \"%s\"",
			    type[i], data[i]);
		}

		if (strlcat(result, one, sizeof(result)) >= sizeof(result))
			return (-1);;
	}

	if (!strlen(result))
		return (-1);

	/* Normal termination */
	if (i == count)
		strlcat(result, " then permit", sizeof(result));

	*presult = result;
	return (nfilename);
}

struct plist *
make_policy_suggestion(char *info)
{
	char line[4096], *next, *p, *syscall;
	char *type[MAX_SYSCALLARGS], *data[MAX_SYSCALLARGS];
	int count = 0, res;
	struct plist *items;
	struct policy_list *np;

	items = (struct plist *)malloc(sizeof(struct plist));
	if (items == NULL)
		errx(1, "make_policy_suggestion: cannot allocate memory");
	SIMPLEQ_INIT(items);

	/* Prepare parsing of info line */
	strlcpy(line, info, sizeof(line));

	next = line;
	syscall = strsep(&next, ",");

	/* See if we can make a suggestion for this system call */
	if (next == NULL)
		goto out;
	next++;
	if (!strncmp(next, "args: ", 6)) {
		count = -1;
		goto out;
	}

	while (next != NULL) {
		p = next;

		next = strstr(next, ", ");
		if (next != NULL) {
			*next = '\0';
			next += 2;
		}

		type[count] = strsep(&p, ":");
		data[count] = p + 1;

		count++;
	}

	res = make_policy(type, data, count, &p, 0);
	if (res != -1) {
		np = alloc_policy(p);
		SIMPLEQ_INSERT_TAIL(items, np, next);
	}

	if (res > 0) {
		res = make_policy(type, data, count, &p, 1);
		if (res != -1) {
			np = alloc_policy(p);
			SIMPLEQ_INSERT_TAIL(items, np, next);
		}
	}

 out:
	/* Simples policy */
	p = count == -1 ? "permit" : "true then permit";
	np = alloc_policy(p);
	SIMPLEQ_INSERT_TAIL(items, np, next);

	return (items);
}
