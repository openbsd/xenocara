
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Rely on vasprintf (GNU extension) instead of vsnprintf if
   possible... */
#define _GNU_SOURCE
#include <stdio.h>

#include <xcb/xcb.h>
#include <stdlib.h>
#include <stdarg.h>
#include "xcb_atom.h"

#ifndef __has_attribute
# define __has_attribute(x) 0  /* Compatibility with older compilers. */
#endif

#if __has_attribute(__format__)                                         \
    || defined(__GNUC__) && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 203)
__attribute__((__format__(__printf__,1,2)))
#endif
static char *makename(const char *fmt, ...)
{
	char *ret;
	int n;
	va_list ap;

#ifndef HAVE_VASPRINTF
	char *np;
	int size = 64;

	/* First allocate 'size' bytes, should be enough usually */
	if((ret = malloc(size)) == NULL)
		return NULL;

	while(1)
	{
		va_start(ap, fmt);
		n = vsnprintf(ret, size, fmt, ap);
		va_end(ap);

		if(n < 0)
			return NULL;

		if(n < size)
			return ret;

		size = n + 1;
		if((np = realloc(ret, size)) == NULL)
		{
			free(ret);
			return NULL;
		}

		ret = np;
	}
#else
	va_start(ap, fmt);
	n = vasprintf(&ret, fmt, ap);
	va_end(ap);

	if(n < 0)
		return NULL;

	return ret;
#endif
}

char *xcb_atom_name_by_screen(const char *base, uint8_t screen)
{
	return makename("%s_S%u", base, screen);
}

char *xcb_atom_name_by_resource(const char *base, uint32_t resource)
{
	return makename("%s_R%08X", base, resource);
}

char *xcb_atom_name_unique(const char *base, uint32_t id)
{
	if(base)
		return makename("%s_U%u", base, id);
	else
		return makename("U%u", id);
}
