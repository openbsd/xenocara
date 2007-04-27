/*
 * calmwm - the calm window manager
 *
 * Copyright (c) 2004 Marius Aamodt Eriksen <marius@monkey.org>
 * All rights reserved.
 *
 * $Id: xmalloc.c,v 1.1.1.1 2007/04/27 17:58:48 bernd Exp $
 */

#include "headers.h"
#include "calmwm.h"

void *
xmalloc(size_t siz)
{
	void *p;

	if ((p = malloc(siz)) == NULL)
		err(1, "malloc");

	return (p);
}

void *
xcalloc(size_t siz)
{
	void *p;

	if ((p = calloc(1, siz)) == NULL)
		err(1, "calloc");

	return (p);
}

void
xfree(void *p)
{
	free(p);
}

char *
xstrdup(const char *str)
{
	char *p;

	if ((p = strdup(str)) == NULL)
		err(1, "strdup");

	return (p);
}
