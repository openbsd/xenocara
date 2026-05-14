#ifndef _XKBFILEINT_H_
#define	_XKBFILEINT_H_ 1

/************************************************************
 Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.

 Permission to use, copy, modify, and distribute this
 software and its documentation for any purpose and without
 fee is hereby granted, provided that the above copyright
 notice appear in all copies and that both that copyright
 notice and this permission notice appear in supporting
 documentation, and that the name of Silicon Graphics not be
 used in advertising or publicity pertaining to distribution
 of the software without specific prior written permission.
 Silicon Graphics makes no representation about the suitability
 of this software for any purpose. It is provided "as is"
 without any express or implied warranty.

 SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
 GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
 THE USE OR PERFORMANCE OF THIS SOFTWARE.

 ********************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <X11/Xfuncproto.h>

#include "XKBfile.h"
#include <string.h>
#include <strings.h>

#ifdef DEBUG
#define	_XkbLibError(c,l,d) \
	{ fprintf(stderr,"xkbfile: %s in %s\n",_XkbErrMessages[c],(l)); \
	 _XkbErrCode= (c); _XkbErrLocation= (l); _XkbErrData= (d); }
#else
#define	_XkbLibError(c,l,d) \
	{ _XkbErrCode= (c); _XkbErrLocation= (l); _XkbErrData= (d); }
#endif


#define	_XkbAlloc(s)		malloc((s))
#define	_XkbCalloc(n,s)		calloc((n),(s))
#define	_XkbRealloc(o,s)	realloc((o),(s))
#define	_XkbReallocF(o,s)	reallocf((o),(s))
#define	_XkbTypedAlloc(t)	((t *)malloc(sizeof(t)))
#define	_XkbTypedCalloc(n,t)	((t *)calloc((n),sizeof(t)))
#define	_XkbTypedRealloc(o,n,t) \
	((o)?(t *)realloc((o),(n)*sizeof(t)):_XkbTypedCalloc(n,t))
#define	_XkbTypedReallocF(o,n,t) \
	((o)?(t *)reallocf((o),(n)*sizeof(t)):_XkbTypedCalloc(n,t))
#define	_XkbClearElems(a,f,l,t)	memset(&(a)[f], 0, ((l)-(f)+1)*sizeof(t))
#define	_XkbFree(p)		free(p)

#ifndef PATH_MAX
#ifdef MAXPATHLEN
#define PATH_MAX MAXPATHLEN
#else
#define PATH_MAX 1024
#endif
#endif


_XFUNCPROTOBEGIN

#ifndef HAVE_REALLOCF
/* realloc variant that frees old pointer on failure */
static inline void *
reallocf(void *old, size_t size)
{
    void *new = realloc(old, size);

    if (_X_UNLIKELY(new == NULL))
        free(old);

    return new;
}
#endif

static inline char *
_XkbDupString(const char *s)
{
    return s ? strdup(s) : NULL;
}

#define _XkbStrCaseEqual(s1,s2)	(_XkbStrCaseCmp(s1,s2)==0)
#define _XkbStrCaseCmp strcasecmp

_XFUNCPROTOEND
#endif                          /* _XKBFILEINT_H_ */
