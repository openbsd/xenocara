#ifndef UTILS_H
#define	UTILS_H 1

  /*\
   *                          COPYRIGHT 1990
   *                    DIGITAL EQUIPMENT CORPORATION
   *                       MAYNARD, MASSACHUSETTS
   *                        ALL RIGHTS RESERVED.
   *
   * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
   * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
   * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE
   * FOR ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED
   * WARRANTY.
   *
   * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT
   * RIGHTS, APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN
   * ADDITION TO THAT SET FORTH ABOVE.
   *
   * Permission to use, copy, modify, and distribute this software and its
   * documentation for any purpose and without fee is hereby granted, provided
   * that the above copyright notice appear in all copies and that both that
   * copyright notice and this permission notice appear in supporting
   * documentation, and that the name of Digital Equipment Corporation not be
   * used in advertising or publicity pertaining to distribution of the
   * software without specific, written prior permission.
  \*/

/***====================================================================***/

#include 	<stdio.h>
#include	<X11/Xos.h>
#include	<X11/Xfuncproto.h>
#include	<X11/Xfuncs.h>
#include 	<stdarg.h>
#include	<stddef.h>

_XFUNCPROTOBEGIN

#ifndef NUL
#define	NUL	'\0'
#endif

/***====================================================================***/

#ifndef OPAQUE_DEFINED
typedef void *Opaque;
#endif
#ifndef NullOpaque
#define	NullOpaque	((Opaque)NULL)
#endif

#ifndef BOOLEAN_DEFINED
typedef char Boolean;
#endif

#ifndef True
#define	True	((Boolean)1)
#define	False	((Boolean)0)
#endif /* ndef True */
#define	booleanText(b)	((b)?"True":"False")

#ifndef COMPARISON_DEFINED
typedef int Comparison;

#define	Greater		((Comparison)1)
#define	Equal		((Comparison)0)
#define	Less		((Comparison)-1)
#define	CannotCompare	((Comparison)-37)
#define	comparisonText(c)	((c)?((c)<0?"Less":"Greater"):"Equal")
#endif


/***====================================================================***/

extern Boolean uSetErrorFile(const char *name);
extern void uInformation(const char *s, ...) _X_ATTRIBUTE_PRINTF(1, 2);
extern void uAction(const char *s, ...) _X_ATTRIBUTE_PRINTF(1, 2);
extern void uWarning(const char *s, ...) _X_ATTRIBUTE_PRINTF(1, 2);
extern void uError(const char *s, ...) _X_ATTRIBUTE_PRINTF(1, 2);
extern void uFatalError(const char *s, ...) _X_ATTRIBUTE_PRINTF(1, 2)
    _X_NORETURN;
extern void uInternalError(const char *s, ...) _X_ATTRIBUTE_PRINTF(1, 2);

/***====================================================================***/

#define	NullString	((char *)NULL)

#define	uStringText(s)		((s)==NullString?"<NullString>":(s))
#define	uStringEqual(s1,s2)	(uStringCompare(s1,s2)==Equal)
#define	uStringPrefix(p,s)	(strncmp(p,s,strlen(p))==0)
#define	uStringCompare(s1,s2)	(strcmp(s1,s2))
#define	uStrCaseEqual(s1,s2)	(strcasecmp(s1,s2)==0)

#ifndef HAVE_ASPRINTF
extern _X_HIDDEN int _X_ATTRIBUTE_PRINTF(2,3)
    uAsprintf(char ** ret, const char *format, ...);
#define asprintf                uAsprintf
#endif

/***====================================================================***/

_XFUNCPROTOEND

#endif                          /* UTILS_H */
