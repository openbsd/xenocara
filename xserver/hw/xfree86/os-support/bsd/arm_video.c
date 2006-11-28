/* $XFree86: xc/programs/Xserver/hw/xfree86/os-support/bsd/ppc_video.c,v 1.6 2003/10/07 23:14:55 herrb Exp $ */
/* $OpenBSD: arm_video.c,v 1.2 2006/11/28 20:29:31 matthieu Exp $ */
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

/* $XConsortium: bsd_video.c /main/10 1996/10/25 11:37:57 kaleb $ */

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#include <X11/X.h>
#include "xf86.h"
#include "xf86Priv.h"

#include "xf86_OSlib.h"
#include "xf86OSpriv.h"

#include "bus/Pci.h"

#ifndef MAP_FAILED
#define MAP_FAILED ((caddr_t)-1)
#endif

#include <sys/param.h>
#include <sys/sysctl.h>

/***************************************************************************/
/* Video Memory Mapping section                                            */
/***************************************************************************/

#ifdef __OpenBSD__
#undef DEV_MEM
#define DEV_MEM "/dev/xf86"
#endif

static pointer ppcMapVidMem(int, unsigned long, unsigned long, int flags);
static void ppcUnmapVidMem(int, pointer, unsigned long);

void
xf86OSInitVidMem(VidMemInfoPtr pVidMem)
{
	pVidMem->linearSupported = TRUE;
	pVidMem->mapMem = ppcMapVidMem;
	pVidMem->unmapMem = ppcUnmapVidMem;
	pVidMem->initialised = TRUE;
}


volatile unsigned char *ioBase = MAP_FAILED;

static pointer
ppcMapVidMem(int ScreenNum, unsigned long Base, unsigned long Size, int flags)
{
	int fd = xf86Info.screenFd;
	pointer base;
#ifdef DEBUG
	xf86MsgVerb(X_INFO, 3, "mapVidMem %lx, %lx, fd = %d", 
		    Base, Size, fd);
#endif

	base = mmap(0, Size,
		    (flags & VIDMEM_READONLY) ?
		     PROT_READ : (PROT_READ | PROT_WRITE),
		    MAP_SHARED, fd, Base);
	if (base == MAP_FAILED)
		FatalError("%s: could not mmap screen [s=%lx,a=%lx] (%s)",
			   "xf86MapVidMem", Size, Base, strerror(errno));

	return base;
}

static void
ppcUnmapVidMem(int ScreenNum, pointer Base, unsigned long Size)
{
	munmap(Base, Size);
}

static int kmem = -1;

int
xf86ReadBIOS(unsigned long Base, unsigned long Offset, unsigned char *Buf,
	     int Len)
{
	int rv;

	if (Base < 0x80000000) {
		xf86Msg(X_WARNING, "No VGA Base=%#lx\n", Base);
		return 0;
	}

	if (kmem == -1) {
		kmem = open(DEV_MEM, 2);
		if (kmem == -1) {
			FatalError("xf86ReadBIOS: open %s", DEV_MEM);
		}
	}

#ifdef DEBUG
	xf86MsgVerb(X_INFO, 3, "xf86ReadBIOS() %lx %lx, %x\n", 
		    Base, Offset, Len);
#endif


	lseek(kmem, Base + Offset, 0);
	rv = read(kmem, Buf, Len);
	return rv;
}

/***************************************************************************/
/* Interrupt Handling section                                              */
/***************************************************************************/

Bool
xf86DisableInterrupts()
{

	return(TRUE);
}

void
xf86EnableInterrupts()
{

	return;
}

/*
 * Do all initialisation that need root privileges 
 */
void
xf86PrivilegedInit(void)
{
	int mib[2];
	char buf[128];
	size_t len;

	mib[0] = CTL_HW;
	mib[1] = HW_MACHINE;
	len = sizeof(buf);
	if (sysctl(mib, 2, buf, &len, NULL, 0) < 0) {
		FatalError("Cannot get hw.machine");
	}
	if (strcmp(buf, "zaurus") != 0) {
		/* Not Zaurus */
		kmem = open(DEV_MEM, 2);
		if (kmem == -1) {
			ErrorF("errno: %d\n", errno);
			FatalError("xf86PrivilegedInit: open %s", DEV_MEM);
		}
	}

	pciInit();
	xf86OpenConsole();
}

#ifdef __VFP_FP__
/*
 * force softfloat functions into binary,
 * yes the protos/ret are all bogus.
 */
arm_softfloat()
{
void __adddf3();
void __addsf3();
void __eqdf2();
void __eqsf2();
void __extendsfdf2();
void __fixdfsi();
void __fixsfsi();
void __fixunsdfsi();
void __fixunssfsi();
void __floatsidf();
void __floatsisf();
void __gedf2();
void __gesf2();
void __gtdf2();
void __gtsf2();
void __ledf2();
void __lesf2();
void __ltdf2();
void __ltsf2();
void __nedf2();
void __negdf2();
void __negsf2();
void __nesf2();
void __subdf3();
void __subsf3();
void __truncdfsf2();

__adddf3();
__addsf3();
__eqdf2();
__eqsf2();
__extendsfdf2();
__fixdfsi();
__fixsfsi();
__fixunsdfsi();
__fixunssfsi();
__floatsidf();
__floatsisf();
__gedf2();
__gesf2();
__gtdf2();
__gtsf2();
__ledf2();
__lesf2();
__ltdf2();
__ltsf2();
__nedf2();
__negdf2();
__negsf2();
__nesf2();
__subdf3();
__subsf3();
__truncdfsf2();
}
#endif /* __VFP_FP__ */
