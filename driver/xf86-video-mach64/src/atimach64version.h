/*
 * Copyright 2000 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of Marc Aurele La France not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Marc Aurele La France makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as-is" without express or implied warranty.
 *
 * MARC AURELE LA FRANCE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO
 * EVENT SHALL MARC AURELE LA FRANCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _MACH64_VERSION_H_
#define _MACH64_VERSION_H_ 1

#undef  MACH64_NAME
#undef  MACH64_DRIVER_NAME
#undef  MACH64_VERSION_MAJOR
#undef  MACH64_VERSION_MINOR
#undef  MACH64_VERSION_PATCH
#undef  MACH64_VERSION_CURRENT
#undef  MACH64_VERSION_EVALUATE
#undef  MACH64_VERSION_STRINGIFY
#undef  MACH64_VERSION_NAME

#define MACH64_NAME          "MACH64"
#define MACH64_DRIVER_NAME   "mach64"

#define MACH64_VERSION_MAJOR 6
#define MACH64_VERSION_MINOR 7
#define MACH64_VERSION_PATCH 0

#ifndef MACH64_VERSION_EXTRA
#define MACH64_VERSION_EXTRA ""
#endif

#define MACH64_VERSION_CURRENT \
    ((MACH64_VERSION_MAJOR << 20) | \
     (MACH64_VERSION_MINOR << 10) | \
     (MACH64_VERSION_PATCH))

#define MACH64_VERSION_EVALUATE(__x) #__x
#define MACH64_VERSION_STRINGIFY(_x) MACH64_VERSION_EVALUATE(_x)
#define MACH64_VERSION_NAME                                             \
    MACH64_VERSION_STRINGIFY(MACH64_VERSION_MAJOR) "."                  \
    MACH64_VERSION_STRINGIFY(MACH64_VERSION_MINOR) "."                  \
    MACH64_VERSION_STRINGIFY(MACH64_VERSION_MINOR) MACH64_VERSION_EXTRA

#endif /* _MACH64_VERSION_H_ */
