/***********************************************************
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#ifndef XVLIBINT_H
#define XVLIBINT_H
/*
** File:
**
**   Xvlibint.h --- Xv library internal header file
**
** Author:
**
**   David Carver (Digital Workstation Engineering/Project Athena)
**
** Revisions:
**
**   01.24.91 Carver
**     - version 1.4 upgrade
**
*/

#include <X11/Xlibint.h>
#include <X11/extensions/Xvproto.h>
#include <X11/extensions/Xvlib.h>

/* names in Xvproto.h don't match the expectation of Xlib's GetReq* macros,
   so we have to provide our own implementation */

#define XvGetReq(name, req) \
    req = (xv##name##Req *) _XGetRequest(                               \
        dpy, (CARD8) info->codes->major_opcode, SIZEOF(xv##name##Req)); \
    req->xvReqType = xv_##name

#endif /* XVLIBINT_H */
