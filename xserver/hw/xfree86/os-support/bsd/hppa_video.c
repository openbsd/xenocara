/* $OpenBSD: hppa_video.c,v 1.2 2009/12/18 22:47:56 matthieu Exp $ */
/*
 * Copyright 1992 by Rich Murphey <Rich@Rice.edu>
 * Copyright 1993 by David Wexelblat <dwex@goblin.org>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Rich Murphey and David Wexelblat 
 * not be used in advertising or publicity pertaining to distribution of 
 * the software without specific, written prior permission.  Rich Murphey and
 * David Wexelblat make no representations about the suitability of this 
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 *
 * RICH MURPHEY AND DAVID WEXELBLAT DISCLAIM ALL WARRANTIES WITH REGARD TO 
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS, IN NO EVENT SHALL RICH MURPHEY OR DAVID WEXELBLAT BE LIABLE FOR 
 * ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER 
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF 
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#include <X11/X.h>
#include "xf86.h"
#include "xf86Priv.h"

#include "xf86_OSlib.h"
#include "xf86OSpriv.h"

#ifndef MAP_FAILED
#define MAP_FAILED ((caddr_t)-1)
#endif


/***************************************************************************/
/* Video Memory Mapping section                                            */
/***************************************************************************/

static pointer hppaMapVidMem(int, unsigned long, unsigned long, int);
static void hppaUnmapVidMem(int, pointer, unsigned long);


void
xf86OSInitVidMem(VidMemInfoPtr pVidMem)
{
	pVidMem->linearSupported = TRUE;
	pVidMem->mapMem = hppaMapVidMem;
	pVidMem->unmapMem = hppaUnmapVidMem;
#if HAVE_PCI_SYSTEM_INIT_DEV_MEM
       pci_system_init_dev_mem(xf86Info.screenFd);
#endif
	pVidMem->initialised = TRUE;
}


volatile unsigned char *ioBase = MAP_FAILED;

static pointer
hppaMapVidMem(int ScreenNum, unsigned long Base, unsigned long Size, int flags)
{
	int fd = xf86Info.screenFd;
	pointer base;

#ifdef DEBUG
	xf86MsgVerb(X_INFO, 3, "mapVidMem %lx, %lx, fd = %d\n", 
	    Base, Size, fd);
#endif

	base = mmap(0, Size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, Base);
	if (base == MAP_FAILED)
		FatalError("%s: could not mmap screen [s=%lx,a=%lx] (%s)",
			   "xf86MapVidMem", Size, Base, strerror(errno));

	return base;
}


static void
hppaUnmapVidMem(int ScreenNum, pointer Base, unsigned long Size)
{

	munmap(Base, Size);
}

int
xf86ReadBIOS(unsigned long Base, unsigned long Offset, unsigned char *Buf,
	     int Len)
{
	return 0;
}

/***************************************************************************/
/* Interrupt Handling section                                              */
/***************************************************************************/

#ifdef X_PRIVSEP
/*
 * Do all initialisation that need root privileges 
 */
void
xf86PrivilegedInit(void)
{
	/* pci_system_init(); */
	xf86OpenConsole();
}
#endif
