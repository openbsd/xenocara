/*
 * Copyright 1990, 1991 by Thomas Roell, Dinkelscherben, Germany
 * Copyright 1992 by David Dawes <dawes@XFree86.org>
 * Copyright 1992 by Jim Tsillas <jtsilla@damon.ccs.northeastern.edu>
 * Copyright 1992 by Rich Murphey <Rich@Rice.edu>
 * Copyright 1992 by Robert Baron <Robert.Baron@ernst.mach.cs.cmu.edu>
 * Copyright 1992 by Orest Zborowski <obz@eskimo.com>
 * Copyright 1993 by Vrije Universiteit, The Netherlands
 * Copyright 1993 by David Wexelblat <dwex@XFree86.org>
 * Copyright 1994, 1996 by Holger Veit <Holger.Veit@gmd.de>
 * Copyright 1997 by Takis Psarogiannakopoulos <takis@dpmms.cam.ac.uk>
 * Copyright 1994-2003 by The XFree86 Project, Inc
 * Copyright 1999 by David Holland <davidh@iquest.net>
 * Copyright 2015 by VMware Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of the above listed copyright holders
 * not be used in advertising or publicity pertaining to distribution of
 * the software without specific, written prior permission.  The above listed
 * copyright holders make no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without express or
 * implied warranty.
 *
 * THE ABOVE LISTED COPYRIGHT HOLDERS DISCLAIM ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#include <stdbool.h>

#if defined(VMMOUSE_OS_BSD)
#include <sys/types.h>
#ifdef USE_I386_IOPL
#include <machine/sysarch.h>
/***************************************************************************/
/* I/O Permissions section                                                 */
/***************************************************************************/
static bool ExtendedEnabled = false;

bool
xf86EnableIO()
{
    if (ExtendedEnabled)
	return true;

    if (i386_iopl(1) < 0)
	return false;

    ExtendedEnabled = true;
    return true;
}

void
xf86DisableIO()
{
    if (!ExtendedEnabled)
	return;

    i386_iopl(0);

    ExtendedEnabled = false;
    return;
}

#endif /* USE_I386_IOPL */

#ifdef USE_AMD64_IOPL
#include <machine/sysarch.h>
/***************************************************************************/
/* I/O Permissions section                                                 */
/***************************************************************************/

static bool ExtendedEnabled = false;

bool
xf86EnableIO()
{
    if (ExtendedEnabled)
	return true;

    if (amd64_iopl(1) < 0)
	return false;

    ExtendedEnabled = true;
    return true;
}

void
xf86DisableIO()
{
    if (!ExtendedEnabled)
	return;

    if (amd64_iopl(0) == 0)
	ExtendedEnabled = false;

    return;
}

#endif /* USE_AMD64_IOPL */

#ifdef USE_DEV_IO
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
static int IoFd = -1;

bool
xf86EnableIO()
{
    if (IoFd >= 0)
	return true;

    if ((IoFd = open("/dev/io", O_RDWR)) == -1)
	return false;

    return true;
}

void
xf86DisableIO()
{
    if (IoFd < 0)
	return;

    close(IoFd);
    IoFd = -1;
    return;
}
#endif

#elif defined(VMMOUSE_OS_GENERIC)

static bool ExtendedEnabled = false;

extern int ioperm(unsigned long __from, unsigned long __num, int __turn_on);
extern int iopl(int __level);

bool xf86EnableIO(void)
{
    if (ExtendedEnabled)
	return true;

    if (ioperm(0, 1024, 1) || iopl(3))
	return false;

    ExtendedEnabled = true;
    return true;
}

void
xf86DisableIO(void)
{
    if (!ExtendedEnabled)
	return;

    iopl(0);
    ioperm(0, 1024, 0);
    ExtendedEnabled = false;

    return;
}

#elif defined(VMMOUSE_OS_SOLARIS)

#ifdef __GNUC__
#if defined(__sun) && !defined(sun)
#define sun 1
#endif
#if defined(__SVR4) && !defined(SVR4)
#define SVR4 1
#endif
#endif
/*
 * The below sequence of includes is stolen from Xserver. If it doesn't work
 * for your setup, please propose a patch to fix it.
 */
#include <sys/types.h>
#include <errno.h>
#if !(defined (sun) && defined (SVR4))
#include <sys/immu.h>
#include <sys/region.h>
#include <sys/proc.h>
#endif
#include <sys/tss.h>
#include <sys/sysi86.h>
#if defined(SVR4) && !defined(sun)
#include <sys/seg.h>
#endif                          /* SVR4 && !sun */
/* V86SC_IOPL was moved to <sys/sysi86.h> on Solaris 7 and later */
#if !defined(V86SC_IOPL)        /* Solaris 7 or later? */
#include <sys/v86.h>            /* Nope */
#endif
#if defined(sun) && (defined (__i386__) || defined(__i386) || defined(__x86))  && defined (SVR4)
#include <sys/psw.h>
#endif

static bool ExtendedEnabled = false;

bool
xf86EnableIO(void)
{
    if (ExtendedEnabled)
	return true;

    if (sysi86(SI86V86, V86SC_IOPL, PS_IOPL) < 0)
	return false;

    ExtendedEnabled = true;

    return true;
}

void
xf86DisableIO(void)
{
    if(!ExtendedEnabled)
	return;

    sysi86(SI86V86, V86SC_IOPL, 0);

    ExtendedEnabled = false;
}

#endif
