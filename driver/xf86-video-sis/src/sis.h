/*
 * Main global data and definitions
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1) Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2) Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3) The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors:   Thomas Winischhofer <thomas@winischhofer.net>
 *            others (old code base)
 *
 */

#ifndef _SIS_H_
#define _SIS_H_

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <setjmp.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <sispcirename.h>

#define SISDRIVERVERSIONYEAR    5
#define SISDRIVERVERSIONMONTH   9
#define SISDRIVERVERSIONDAY     20
#define SISDRIVERREVISION       1

#define SISDRIVERIVERSION ((SISDRIVERVERSIONYEAR << 16) |  \
			   (SISDRIVERVERSIONMONTH << 8) |  \
			   SISDRIVERVERSIONDAY 	       |  \
			   (SISDRIVERREVISION << 24))

#undef SIS_LINUX		/* Try to find out whether platform is Linux */
#if defined(__GNUC__) && (__GNUC__ >= 4)
#ifdef __linux__
#define SIS_LINUX
#endif
#else
#ifdef linux
#define SIS_LINUX
#endif
#endif

#if 0
#define TWDEBUG    /* for debugging */
#endif

#undef SIS_CP
#if 0
#include "siscp.H"
#endif

#include "compiler.h"
#include "xf86Pci.h"
#include "xf86_OSproc.h"
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#endif
#include "xf86.h"
#include "xf86Cursor.h"
#include "xf86cmap.h"
#include "vbe.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 12
#define _swapl(x, n) swapl(x,n)
#define _swaps(x, n) swaps(x,n)
#else
#define _swapl(x, n) swapl(x)
#define _swaps(x, n) swaps(x)
#endif

#define SIS_HaveDriverFuncs 0

#undef SISISXORG6899900
#ifdef XORG_VERSION_CURRENT
#include "xorgVersion.h"
#define SISMYSERVERNAME "X.org"
#ifndef XF86_VERSION_NUMERIC
#define XF86_VERSION_NUMERIC(major,minor,patch,snap,dummy) \
	(((major) * 10000000) + ((minor) * 100000) + ((patch) * 1000) + snap)
#define XF86_VERSION_CURRENT XF86_VERSION_NUMERIC(4,3,99,902,0)
#endif
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(6,8,99,900,0) || XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(4,0,0,0,0)
#define SISISXORG6899900
#endif
#if 0
#ifdef HaveDriverFuncs
#define SIS_HAVE_DRIVER_FUNC
#undef  SIS_HaveDriverFuncs
#define SIS_HaveDriverFuncs HaveDriverFuncs
#endif
#endif
#else
#include "xf86Version.h"
#define SISMYSERVERNAME "XFree86"
#endif

#define SIS_NAME                "SIS"
#define SIS_DRIVER_NAME         "sis"
#define SIS_MAJOR_VERSION	PACKAGE_VERSION_MAJOR
#ifdef SISISXORG6899900
#define SIS_MINOR_VERSION	PACKAGE_VERSION_MINOR	/* DRI changes */
#define SIS_PATCHLEVEL		PACKAGE_VERSION_PATCHLEVEL
#else
#define SIS_MINOR_VERSION       7
#define SIS_PATCHLEVEL          1
#endif
#define SIS_CURRENT_VERSION     ((SIS_MAJOR_VERSION << 16) | \
                                 (SIS_MINOR_VERSION << 8) | SIS_PATCHLEVEL )

#if (XF86_VERSION_CURRENT >= XF86_VERSION_NUMERIC(4,3,99,0,0)) || (defined(XvExtension))
#include "xf86xv.h"
#include <X11/extensions/Xv.h>
#endif

/* Platform/architecture related definitions: */

/* SIS_PC_PLATFORM: Map VGA memory at a0000 and save/restore fonts? */
/* List of architectures likely to be incomplete */
#define SIS_PC_PLATFORM
#if defined(__powerpc__) || defined(__mips__) || defined(__arm32__)
#undef SIS_PC_PLATFORM
#endif

/* SIS_USE_BIOS_SCRATCH: Save/restore mode number in BIOS scratch area? */
#undef SIS_USE_BIOS_SCRATCH
#if defined(i386) || defined(__i386) || defined(__i386__) || defined(__AMD64__) || defined(__amd64__) || defined(__x86_64__)
#define SIS_USE_BIOS_SCRATCH
#endif

/* SIS_NEED_MAP_IOP: Map i/o port area to virtual memory? */
/* List of architectures likely to be incomplete */
/* This is BROKEN, see comment in sis_driver.c */
#undef SIS_NEED_MAP_IOP
#if 0
#if defined(__arm32__) || defined(__mips__)
#define SIS_NEED_MAP_IOP
#endif
#endif

/* SISUSEDEVPORT: Used on architectures without direct inX/outX access. In this case,
 * we use read()/write() to /dev/port. LINUX ONLY! (How can this be done on *BSD?)
 */
#undef SISUSEDEVPORT
#if defined(SIS_LINUX) && (defined(__arm32__) || defined(__mips__))
#ifndef SIS_NEED_MAP_IOP
#define SISUSEDEVPORT
#endif
#endif

/* Our #includes: Require the arch/platform dependent #defines above */

#include "osdef.h"
#include "vgatypes.h"
#include "vstruct.h"

#undef SISHAVEDRMWRITE
#undef SISNEWDRI

/* if the server was built without DRI support, force-disable DRI */
#ifndef XF86DRI
#undef SISDRI
#endif

#ifdef SISDRI
#if XF86_VERSION_CURRENT >= XF86_VERSION_NUMERIC(4,2,99,3,0)
#define SISHAVEDRMWRITE
#endif
#if XF86_VERSION_CURRENT >= XF86_VERSION_NUMERIC(4,3,99,14,0)
#define SISNEWDRI
#endif
#undef SIS315DRI		/* define this if dri is adapted for 315/330 series */
#include "xf86drm.h"
#include "sarea.h"
#define _XF86DRI_SERVER_
#include "dri.h"
#include "GL/glxint.h"
#include "sis_dri.h"
#endif /* SISDRI */

/* Configurable stuff: ------------------------------------- */

#define SISDUALHEAD		/* Include Dual Head code  */

#define SISMERGED		/* Include Merged-FB code */

#undef SISXINERAMA
#ifdef SISMERGED
#define SISXINERAMA		/* Include SiS Pseudo-Xinerama for MergedFB mode */
#define SIS_XINERAMA_MAJOR_VERSION  1
#define SIS_XINERAMA_MINOR_VERSION  1
#endif

#define SIS_ARGB_CURSOR		/* Include code for color hardware cursors */

#define ENABLE_YPBPR		/* Include YPbPr support on SiS bridges (315 series and 661/741/760) */

#define SISVRAMQ		/* Use VRAM queue mode on 315/330/340/XGI series */

#undef INCL_YUV_BLIT_ADAPTOR
#ifdef SISVRAMQ
#define INCL_YUV_BLIT_ADAPTOR	/* Include support for YUV->RGB blit adaptors (VRAM queue mode only) */
#endif

#ifdef HAVE_XAA_H
#define SIS_USE_XAA		/* Include code for XAA */
#endif

#ifdef SISVRAMQ
#ifdef XORG_VERSION_CURRENT
#if defined(SIS_HAVE_EXA) || (defined(XF86EXA) && (XF86EXA != 0))
#if 1
#define SIS_USE_EXA		/* Include code for EXA */
#endif
#endif
#endif
#endif

#if 0
#define SISDEINT		/* Include Xv deinterlacer code (not functional yet!) */
#endif

#if 0
#define XV_SD_DEPRECATED	/* Include deprecated XV SD interface for SiSCtrl */
#endif

/* End of configurable stuff --------------------------------- */

#define UNLOCK_ALWAYS		/* Always unlock the registers (should be set!) */

#if !defined(SIS_USE_XAA) && !defined(SIS_USE_EXA)
#define SIS_USE_XAA
#endif

#ifdef SIS_USE_XAA
#include "xaa.h"
#endif
#ifdef SIS_USE_EXA
#include "exa.h"
#endif

/* Need that for SiSCtrl and Pseudo-Xinerama */
#define EXTENSION_PROC_ARGS void *
#include "extnsionst.h" 			/* required */
#include <X11/extensions/panoramiXproto.h> 	/* required */

#include "compat-api.h"

#undef SISCHECKOSSSE
#ifdef XORG_VERSION_CURRENT
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(6,8,99,13,0)
#define SISCHECKOSSSE		/* Automatic check OS for SSE; requires SigIll facility */
#endif
#endif

#undef SISGAMMARAMP
#ifdef XORG_VERSION_CURRENT
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(6,8,99,13,0)
#define SISGAMMARAMP		/* Driver can set gamma ramp; requires additional symbols in xf86sym.h */
#endif
#endif

#if 0	/* Perhaps for future use */
#if 1
#define SIS_PCI_BUS(a) (a)->bus
#define SIS_PCI_DEVICE(a) (a)->device
#define SIS_PCI_FUNC(a) (a)->func
#define SIS_PCI_TAG(a) pciTag((a)->bus, (a)->device, (a)->func);
#else
#define SIS_PCI_BUS(a) (a)->pciid.bus
#define SIS_PCI_DEVICE(a) (a)->pciid.device
#define SIS_PCI_FUNC(a) (a)->pciid.func
#define SIS_PCI_TAG(a) pciTag(&((a)->pciid));
#endif
#endif

#ifdef TWDEBUG
#define SISVERBLEVEL 3
#else
#define SISVERBLEVEL 4
#endif

#ifndef _XF86_PCIINFO_H
#define PCI_VENDOR_SIS			0x1039
/* SiS */
#define PCI_CHIP_SG86C201		0x0001
#define PCI_CHIP_SG86C202		0x0002
#define PCI_CHIP_SG85C503		0x0008
#define PCI_CHIP_SIS5597		0x0200
/* Agregado por Carlos Duclos & Manuel Jander */
#define PCI_CHIP_SIS82C204		0x0204
#define PCI_CHIP_SG86C205		0x0205
#define PCI_CHIP_SG86C215		0x0215
#define PCI_CHIP_SG86C225		0x0225
#define PCI_CHIP_85C501			0x0406
#define PCI_CHIP_85C496			0x0496
#define PCI_CHIP_85C601			0x0601
#define PCI_CHIP_85C5107		0x5107
#define PCI_CHIP_85C5511		0x5511
#define PCI_CHIP_85C5513		0x5513
#define PCI_CHIP_SIS5571		0x5571
#define PCI_CHIP_SIS5597_2		0x5597
#define PCI_CHIP_SIS530			0x6306
#define PCI_CHIP_SIS6326		0x6326
#define PCI_CHIP_SIS7001		0x7001
#define PCI_CHIP_SIS300			0x0300
#define PCI_CHIP_SIS315H		0x0310
#define PCI_CHIP_SIS315PRO		0x0325
#define PCI_CHIP_SIS330			0x0330
#define PCI_CHIP_SIS630			0x6300
#define PCI_CHIP_SIS540			0x5300
#define PCI_CHIP_SIS550			0x5315
#define PCI_CHIP_SIS650			0x6325
#define PCI_CHIP_SIS730			0x7300

#endif

/* For SiS315/550/650/740/330/660 - these should be moved elsewhere! */
#ifndef PCI_CHIP_SIS315H
#define PCI_CHIP_SIS315H	0x0310
#endif
#ifndef PCI_CHIP_SIS315
#define PCI_CHIP_SIS315		0x0315
#endif
#ifndef PCI_CHIP_SIS315PRO
#define PCI_CHIP_SIS315PRO	0x0325
#endif
#ifndef PCI_CHIP_SIS550
#define PCI_CHIP_SIS550		0x5315	/* 550_VGA */
#endif
#ifndef PCI_CHIP_SIS650
#define PCI_CHIP_SIS650		0x6325  /* 650_VGA and 740_VGA */
#endif
#ifndef PCI_CHIP_SIS330
#define PCI_CHIP_SIS330		0x0330
#endif
#ifndef PCI_CHIP_SIS660
#define PCI_CHIP_SIS660		0x6330	/* 661_VGA, 741_VGA, 760_VGA, 761_VGA */
#endif
#ifndef PCI_CHIP_SIS340
#define PCI_CHIP_SIS340		0x0340
#endif

#ifndef PCI_VENDOR_XGI
#define PCI_VENDOR_XGI		0x18ca
#endif

#ifndef PCI_CHIP_XGIXG20
#define PCI_CHIP_XGIXG20	0x0020
#endif

#ifndef PCI_CHIP_XGIXG40
#define PCI_CHIP_XGIXG40	0x0040
#endif

/* pSiS->Flags (old series only) */
#define SYNCDRAM		0x00000001
#define RAMFLAG			0x00000002
#define ESS137xPRESENT		0x00000004
#define SECRETFLAG		0x00000008
#define A6326REVAB		0x00000010
#define MMIOMODE		0x00010000
#define LFBQMODE		0x00020000
#define AGPQMODE		0x00040000
#define UMA			0x80000000

#define BIOS_BASE		0xC0000
#define BIOS_SIZE		0x10000

#define SIS_VBFlagsVersion	1

/* pSiS->VBFlags - if anything is changed here, increase VBFlagsVersion! */
#define CRT2_DEFAULT		0x00000001
#define CRT2_LCD		0x00000002  /* Never change the order of the CRT2_XXX entries */
#define CRT2_TV			0x00000004
#define CRT2_VGA		0x00000008
#define TV_NTSC			0x00000010
#define TV_PAL			0x00000020
#define TV_HIVISION		0x00000040
#define TV_YPBPR		0x00000080
#define TV_AVIDEO		0x00000100
#define TV_SVIDEO		0x00000200
#define TV_SCART		0x00000400
#define OLDVB_CONEXANT		0x00000800	/* Definition deprecated (now VBFlags2) */
#define OLDVB_TRUMPION		OLDVB_CONEXANT	/* Definition deprecated (now VBFlags2) */
#define TV_PALM			0x00001000
#define TV_PALN			0x00002000
#define TV_NTSCJ		TV_PALM
#define OLDVB_302ELV		0x00004000	/* Definition deprecated (now VBFlags2) */
#define TV_CHSCART		0x00008000
#define TV_CHYPBPR525I		0x00010000
#define CRT1_VGA		0x00000000	/* ZERO - no mask! */
#define CRT1_LCDA		0x00020000
#define VGA2_CONNECTED		0x00040000
#define DISPTYPE_CRT1		0x00080000  	/* CRT1 connected and used */
#define TV_YPBPR625I		0x00100000
#define TV_YPBPR625P		0x00200000
#define OLDVB_302B		0x00400000	/* Definition deprecated (now VBFlags2) */
#define OLDVB_30xBDH		0x00800000      /* Definition deprecated (now VBFlags2) */
#define OLDVB_LVDS		0x01000000	/* Definition deprecated (now VBFlags2) */
#define OLDVB_CHRONTEL		0x02000000	/* Definition deprecated (now VBFlags2) */
#define OLDVB_301LV		0x04000000	/* Definition deprecated (now VBFlags2) */
#define OLDVB_302LV		0x08000000	/* Definition deprecated (now VBFlags2) */
#define OLDVB_301C		0x10000000	/* Definition deprecated (now VBFlags2) */
#define SINGLE_MODE		0x20000000   	/* CRT1 or CRT2; determined by DISPTYPE_CRTx */
#define MIRROR_MODE		0x40000000   	/* CRT1 + CRT2 identical (mirror mode) */
#define DUALVIEW_MODE		0x80000000   	/* CRT1 + CRT2 independent (dual head mode) */

/* Aliases: */
#define CRT2_ENABLE		(CRT2_LCD | CRT2_TV | CRT2_VGA)
#define TV_STANDARD		(TV_NTSC | TV_PAL | TV_PALM | TV_PALN | TV_NTSCJ)
#define TV_INTERFACE		(TV_AVIDEO|TV_SVIDEO|TV_SCART|TV_HIVISION|TV_YPBPR)

/* Only if TV_YPBPR is set: */
#define TV_YPBPR525I		TV_NTSC
#define TV_YPBPR525P		TV_PAL
#define TV_YPBPR750P		TV_PALM
#define TV_YPBPR1080I		TV_PALN
#define TV_YPBPRALL 		(TV_YPBPR525I | TV_YPBPR525P | \
				 TV_YPBPR625I | TV_YPBPR625P | \
				 TV_YPBPR750P | TV_YPBPR1080I)

#define TV_YPBPR43LB		TV_CHSCART
#define TV_YPBPR43		TV_CHYPBPR525I
#define TV_YPBPR169 		(TV_CHSCART | TV_CHYPBPR525I)
#define TV_YPBPRAR		(TV_CHSCART | TV_CHYPBPR525I)

#define DISPTYPE_DISP2		CRT2_ENABLE
#define DISPTYPE_DISP1		DISPTYPE_CRT1
#define VB_DISPMODE_SINGLE	SINGLE_MODE  	/* alias */
#define VB_DISPMODE_MIRROR	MIRROR_MODE  	/* alias */
#define VB_DISPMODE_DUAL	DUALVIEW_MODE 	/* alias */
#define DISPLAY_MODE		(SINGLE_MODE | MIRROR_MODE | DUALVIEW_MODE)

/* pSiS->VBFlags2 (static stuff only!) */
#define VB2_SISUMC		0x00000001
#define VB2_301			0x00000002	/* Video bridge type */
#define VB2_301B		0x00000004
#define VB2_301C		0x00000008
#define VB2_307T		0x00000010
#define VB2_302B		0x00000800
#define VB2_301LV		0x00001000
#define VB2_302LV		0x00002000
#define VB2_302ELV		0x00004000
#define VB2_307LV		0x00008000
#define VB2_30xBDH		0x08000000      /* 30xB DH version (w/o LCD support) */
#define VB2_CONEXANT		0x10000000	/* >=661 series only */
#define VB2_TRUMPION		0x20000000	/* 300 series only */
#define VB2_LVDS		0x40000000
#define VB2_CHRONTEL		0x80000000

#define VB2_SISLVDSBRIDGE	(VB2_301LV | VB2_302LV | VB2_302ELV | VB2_307LV)
#define VB2_SISTMDSBRIDGE	(VB2_301   | VB2_301B  | VB2_301C   | VB2_302B | VB2_307T)
#define VB2_SISBRIDGE		(VB2_SISLVDSBRIDGE | VB2_SISTMDSBRIDGE)

#define VB2_SISTMDSLCDABRIDGE	(VB2_301C | VB2_307T)
#define VB2_SISLCDABRIDGE	(VB2_SISTMDSLCDABRIDGE | VB2_301LV | VB2_302LV | VB2_302ELV | VB2_307LV)

#define VB2_SISHIVISIONBRIDGE	(VB2_301  | VB2_301B | VB2_302B)
#define VB2_SISYPBPRBRIDGE	(VB2_301C | VB2_307T | VB2_SISLVDSBRIDGE)
#define VB2_SISYPBPRARBRIDGE	(VB2_301C | VB2_307T | VB2_307LV)
#define VB2_SISTAP4SCALER	(VB2_301C | VB2_307T | VB2_302ELV | VB2_307LV)
#define VB2_SISTVBRIDGE		(VB2_SISHIVISIONBRIDGE | VB2_SISYPBPRBRIDGE)

#define VB2_SISVGA2BRIDGE	(VB2_301 | VB2_301B | VB2_301C | VB2_302B | VB2_307T)

#define VB2_VIDEOBRIDGE		(VB2_SISBRIDGE | VB2_LVDS | VB2_CHRONTEL | VB2_CONEXANT)

#define VB2_30xB		(VB2_301B  | VB2_301C   | VB2_302B  | VB2_307T)
#define VB2_30xBLV		(VB2_30xB  | VB2_SISLVDSBRIDGE)
#define VB2_30xC		(VB2_301C  | VB2_307T)
#define VB2_30xCLV		(VB2_301C  | VB2_307T   | VB2_302ELV| VB2_307LV)
#define VB2_SISEMIBRIDGE	(VB2_302LV | VB2_302ELV | VB2_307LV)
#define VB2_LCD162MHZBRIDGE	(VB2_301C  | VB2_307T)
/* CRT2/LCD over 1280 (overflow bits in Part4) */
#define VB2_LCDOVER1280BRIDGE	(VB2_301C  | VB2_307T   | VB2_302LV | VB2_302ELV | VB2_307LV)
/* CRT2/LCD over 1600? Is this really gonna happen, or will there be LCDA only for large panels? */
#define VB2_LCDOVER1600BRIDGE	(VB2_307T  | VB2_307LV)
/* VGA2 up to 202MHz (1600x1200@75) */
#define VB2_RAMDAC202MHZBRIDGE	(VB2_301C  | VB2_307T)

/* pSiS->VBFlags3 (for future use) */
#define VB3_CRT1_TV		0x00000001
#define VB3_CRT1_LCD		0x00000002
#define VB3_CRT1_VGA		0x00000004
#define TV1_AVIDEO		0x00000100
#define TV1_SVIDEO		0x00000200
#define TV1_SCART		0x00000400
#define TV1_NTSC		0x00000800
#define TV1_PAL			0x00001000
#define TV1_YPBPR		0x00002000
#define TV1_PALM		0x00004000
#define TV1_PALN		0x00008000
#define TV1_NTSCJ		0x00010000
#define TV1_YPBPR525I		0x00020000
#define TV1_YPBPR525P		0x00040000
#define TV1_YPBPR625I		0x00080000
#define TV1_YPBPR625P		0x00100000
#define TV1_YPBPR750P		0x00200000
#define TV1_YPBPR1080I		0x00400000

/* pSiS->VBLCDFlags */
#define VB_LCD_320x480		0x00000001	/* DSTN/FSTN for 550 */
#define VB_LCD_640x480		0x00000002
#define VB_LCD_800x600		0x00000004
#define VB_LCD_1024x768		0x00000008
#define VB_LCD_1280x1024	0x00000010
#define VB_LCD_1280x960		0x00000020
#define VB_LCD_1600x1200	0x00000040
#define VB_LCD_2048x1536	0x00000080
#define VB_LCD_1400x1050	0x00000100
#define VB_LCD_1152x864		0x00000200
#define VB_LCD_1152x768		0x00000400
#define VB_LCD_1280x768		0x00000800
#define VB_LCD_1024x600		0x00001000
#define VB_LCD_640x480_2	0x00002000  	/* DSTN/FSTN */
#define VB_LCD_640x480_3	0x00004000  	/* DSTN/FSTN */
#define VB_LCD_848x480		0x00008000	/* LVDS only, otherwise handled as custom */
#define VB_LCD_1280x800		0x00010000
#define VB_LCD_1680x1050	0x00020000
#define VB_LCD_1280x720		0x00040000
#define VB_LCD_320x240		0x00080000
#define VB_LCD_856x480		0x00100000
#define VB_LCD_1280x854		0x00200000
#define VB_LCD_1920x1200	0x00400000
#define VB_LCD_UNKNOWN		0x10000000
#define VB_LCD_BARCO1366	0x20000000
#define VB_LCD_CUSTOM		0x40000000
#define VB_LCD_EXPANDING	0x80000000

#define VB_FORBID_CRT2LCD_OVER_1600		/* CRT2/LCD supports only up to 1600 pixels */

/* PresetMode argument */
#define SIS_MODE_SIMU		0
#define SIS_MODE_CRT1		1
#define SIS_MODE_CRT2		2

/* pSiS->MiscFlags */
#define MISC_CRT1OVERLAY	0x00000001  /* Current display mode supports overlay (CRT1) */
#define MISC_PANELLINKSCALER	0x00000002  /* Panel link is currently scaling */
#define MISC_CRT1OVERLAYGAMMA	0x00000004  /* Current display mode supports overlay gamma corr on CRT1 */
#define MISC_TVNTSC1024		0x00000008  /* Current display mode is TV NTSC/PALM/YPBPR525I 1024x768  */
#define MISC_CRT2OVERLAY	0x00000010  /* Current display mode supports overlay (CRT2) */
#define MISC_SIS760ONEOVERLAY	0x00000020  /* SiS760/761: Only one overlay available currently */
#define MISC_STNMODE		0x00000040  /* SiS550: xSTN active */

/* pSiS->SiS6326Flags */
#define SIS6326_HASTV		0x00000001
#define SIS6326_TVSVIDEO        0x00000002
#define SIS6326_TVCVBS		0x00000004
#define SIS6326_TVPAL		0x00000008
#define SIS6326_TVDETECTED	0x00000010
#define SIS6326_TVON		0x80000000

#ifdef  DEBUG
#define PDEBUG(p)       p
#else
#define PDEBUG(p)
#endif

#define BITMASK(h,l)		(((unsigned)(1U << ((h) - (l) + 1)) - 1) << (l))
#define GENMASK(mask)		BITMASK(1 ? mask, 0 ? mask)

#define GETBITS(var,mask)	(((var) & GENMASK(mask)) >> (0 ? mask))
#define SETBITS(val,mask)	((val) << (0 ? mask))
#define SETBIT(n)		(1 << (n))

#define GETBITSTR(val,from,to)       ((GETBITS(val, from)) << (0 ? to))
#define SETVARBITS(var,val,from,to)  (((var) & (~(GENMASK(to)))) | GETBITSTR(val,from,to))
#define GETVAR8(var)		((var) & 0xFF)
#define SETVAR8(var,val)	(var) =  GETVAR8(val)

typedef unsigned long  ULong;
typedef unsigned short UShort;
typedef unsigned char  UChar;

/* pSiS->VGAEngine - VGA engine types */
#define UNKNOWN_VGA 0
#define SIS_530_VGA 1
#define SIS_OLD_VGA 2
#define SIS_300_VGA 3
#define SIS_315_VGA 4   /* Includes 330/660/661/741/760/340/761 and M versions thereof, XGI */

/* pSiS->oldChipset */
#define OC_UNKNOWN   0
#define OC_SIS86201  1
#define OC_SIS86202  2
#define OC_SIS6205A  3
#define OC_SIS6205B  4
#define OC_SIS82204  5
#define OC_SIS6205C  6
#define OC_SIS6225   7
#define OC_SIS5597   8
#define OC_SIS6326   9
#define OC_SIS530A  11
#define OC_SIS530B  12 /* 620 in 620-WDR */
#define OC_SIS620   13

/* Chrontel type */
#define CHRONTEL_700x 0
#define CHRONTEL_701x 1

/* pSiS->ChipFlags */
/* Use only lower 16 bit for chip id! (sisctrl) */
#define SiSCF_LARGEOVERLAY  0x00000001
#define SiSCF_Is651         0x00000002
#define SiSCF_IsM650        0x00000004
#define SiSCF_IsM652        0x00000008
#define SiSCF_IsM653        0x00000010
#define SiSCF_Is652         0x00000020
#define SiSCF_Is65x         (SiSCF_Is651|SiSCF_IsM650|SiSCF_IsM652|SiSCF_IsM653|SiSCF_Is652)
#define SiSCF_IsM661        0x00000100  /* M661FX */
#define SiSCF_IsM741        0x00000200
#define SiSCF_IsM760        0x00000400
#define SiSCF_IsM661M       0x00000800  /* M661MX */
#define SiSCF_IsM66x        (SiSCF_IsM661 | SiSCF_IsM741 | SiSCF_IsM760 | SiSCF_IsM661M)
#define SiSCF_Is315USB      0x00001000  /* USB2VGA dongle */
#define SiSCF_Is315E	    0x00002000  /* 315E (lower clocks) */
#define SiSCF_IsXGIV3	    SiSCF_Is651 /* Volari V3(XT)  (If neither XGI... set, is V8) */
#define SiSCF_IsXGIV5	    SiSCF_IsM650/* Volari V5 */
#define SiSCF_IsXGIDUO	    SiSCF_IsM652/* Volari Duo */
/* ... */
#define SiSCF_315Core       0x00010000  /* 3D: Real 315 */
#define SiSCF_Real256ECore  0x00020000  /* 3D: Similar to 315 core, no T&L? (65x, 661, 740, 741) */
#define SiSCF_XabreCore     0x00040000  /* 3D: Real Xabre */
#define SiSCF_Ultra256Core  0x00080000  /* 3D: aka "Mirage 2"; similar to Xabre, no T&L?, no P:Shader? (760) */
#define SiSCF_MMIOPalette   0x00100000  /* HW supports MMIO palette writing/reading */
#define SiSCF_IsXGI	    0x00200000  /* Is XGI chip (Z7, V3, V5, V8) */
#define SiSCF_UseLCDA       0x01000000
#define SiSCF_760LFB        0x08000000  /* 76x: LFB active (if not set, UMA only) */
#define SiSCF_760UMA        0x10000000  /* 76x: UMA active (if not set, LFB only) */
#define SiSCF_CRT2HWCKaputt 0x20000000  /* CRT2 Mono HWCursor engine buggy (SiS 330) */
#define SiSCF_Glamour3      0x40000000
#define SiSCF_Integrated    0x80000000

/* SiS Direct Xv-API */
#define SiS_SD_IS300SERIES     0x00000001
#define SiS_SD_IS315SERIES     0x00000002
#define SiS_SD_IS330SERIES     0x00000004
#define SiS_SD_SUPPORTPALMN    0x00000008   /* tv chip supports pal-m, pal-n */
#define SiS_SD_SUPPORT2OVL     0x00000010   /* set = 2 overlays, clear = support SWITCHCRT xv prop */
#define SiS_SD_SUPPORTTVPOS    0x00000020   /* supports changing tv position */
#define SiS_SD_ISDUALHEAD      0x00000040   /* Driver is in dual head mode */
#define SiS_SD_ISMERGEDFB      0x00000080   /* Driver is in merged fb mode */
#define SiS_SD_ISDHSECONDHEAD  0x00000100   /* Dual head: This is CRT1 (=second head) */
#define SiS_SD_ISDHXINERAMA    0x00000200   /* Dual head: We are running Xinerama */
#define SiS_SD_VBHASSCART      0x00000400   /* videobridge has SCART instead of VGA2 */
#define SiS_SD_ISDEPTH8        0x00000800   /* Depth is 8, no independent gamma correction */
#define SiS_SD_SUPPORTSOVER    0x00001000   /* Support for Chrontel Super Overscan */
#define SiS_SD_ENABLED         0x00002000   /* sisctrl is enabled (by option) */
#define SiS_SD_PSEUDOXINERAMA  0x00004000   /* pseudo xinerama is active */
#define SiS_SD_SUPPORTLCDA     0x00008000   /* Support LCD Channel A */
#define SiS_SD_SUPPORTNTSCJ    0x00010000   /* tv chip supports ntsc-j */
#define SiS_SD_ADDLSUPFLAG     0x00020000   /* 1 = the following flags are valid */
#define SiS_SD_SUPPORTVGA2     0x00040000   /* CRT2=VGA supported */
#define SiS_SD_SUPPORTSCART    0x00080000   /* CRT2=SCART supported */
#define SiS_SD_SUPPORTOVERSCAN 0x00100000   /* Overscan flag supported */
#define SiS_SD_SUPPORTXVGAMMA1 0x00200000   /* Xv Gamma correction for CRT1 supported */
#define SiS_SD_SUPPORTTV       0x00400000   /* CRT2=TV supported */
#define SiS_SD_SUPPORTYPBPR    0x00800000   /* CRT2=YPbPr (525i, 525p, 750p, 1080i) is supported */
#define SiS_SD_SUPPORTHIVISION 0x01000000   /* CRT2=HiVision is supported */
#define SiS_SD_SUPPORTYPBPRAR  0x02000000   /* YPbPr aspect ratio is supported */
#define SiS_SD_SUPPORTSCALE    0x04000000   /* Scaling of LCD panel supported */
#define SiS_SD_SUPPORTCENTER   0x08000000   /* If scaling supported: Centering of screen [NOT] supported (TMDS only) */
#define SiS_SD_SUPPORTREDETECT 0x10000000   /* Support re-detection of CRT2 devices */
#define SiS_SD_IS340SERIES     0x20000000
#define SiS_SD_SUPPORTSGRCRT2  0x40000000   /* Separate CRT2 gamma correction supported */
#define SiS_SD_CANSETGAMMA     0x80000000   /* Driver can set gamma ramp; otherwise: App needs to reset palette */
					    /* after disabling sep CRT2 gamma corr */

#define SiS_SD2_LCDTMDS	       0x00000001   /* SiS Bridge supports TMDS (DVI-D) */
#define SiS_SD2_LCDLVDS	       0x00000002   /* SiS Bridge supports LVDS */
#define SiS_SD2_SUPPORTLCD     0x00000004   /* Bridge supports LCD (LVDS or TMDS, SiS+3rd party) */
#define SiS_SD2_SUPPORTTVSIZE  0x00000008   /* TV resizing supported (SiS bridges) */
#define SiS_SD2_SUPPORTTVTYPE  0x00000010   /* TV type selection supported (SiS bridges) */
#define SiS_SD2_SUPPORTGAMMA2  0x00000020   /* Gamma corr for CRT2 supported (SiS bridges) */
#define SiS_SD2_SISBRIDGE      0x00000040   /* SiS bridge */
#define SiS_SD2_SUPPTVSAT      0x00000080   /* TV saturation supported */
#define SiS_SD2_SUPPTVEDGE     0x00000100   /* TV edge enhancement supported */
#define SiS_SD2_CHRONTEL       0x00000200   /* Chrontel TV encoder present */
#define SiS_SD2_VIDEOBRIDGE    0x00000400   /* Any type of video bridge present */
#define SiS_SD2_THIRDPARTYLVDS 0x00000800   /* Third party LVDS (non-SiS) */
#define SiS_SD2_ADDLFLAGS      0x00001000   /* Following flags valid */
#define SiS_SD2_SUPPORT760OO   0x00002000   /* Support dynamic one/two overlay configuration changes         */
					    /*    (If set, utility must re-read SD2 flags after mode change) */
#define SiS_SD2_SIS760ONEOVL   0x00004000   /* (76x:) Only one overlay currently */
#define SiS_SD2_MERGEDUCLOCK   0x00008000   /* Provide VRefresh in mode->Clock field in MergedFB mode */
#define SiS_SD2_SUPPORTXVHUESAT 0x00010000  /* Xv: Support hue & saturation */
#define SiS_SD2_NEEDUSESSE     0x00020000   /* Need "UseSSE" option to use SSE (otherwise auto) */
#define SiS_SD2_NODDCSUPPORT   0x00040000   /* No hardware DDC support (USB) */
#define SiS_SD2_SUPPORTXVDEINT 0x00080000   /* Xv deinterlacing supported (n/a, for future use) */
#define SiS_SD2_ISXGI	       0x00100000   /* Is XGI chip */
#define SiS_SD2_USEVBFLAGS2    0x00200000   /* Use VBFlags2 for bridge ID */
#define SiS_SD2_SUPPLTFLAG     0x00400000   /* Driver supports the following 3 flags */
#define SiS_SD2_ISLAPTOP       0x00800000   /* This machine is (very probably) a laptop */
#define SiS_SD2_MACHINETYPE2   0x01000000   /* Machine type 2 (for future use) */
#define SiS_SD2_MACHINETYPE3   0x02000000   /* Machine type 3 (for future use) */
#define SiS_SD2_SUPPORT625I    0x04000000   /* Support YPbPr 625i */
#define SiS_SD2_SUPPORT625P    0x08000000   /* Support YPbPr 625p */
#define SiS_SD2_VBINVB2ONLY    0x10000000   /* VB_* bits in vbflags no longer used for vb type */
#define SiS_SD2_NEWGAMMABRICON 0x20000000   /* Support new gamma brightness/contrast */
#define SiS_SD2_HAVESD34       0x40000000   /* Support SD3 and SD4 flags */
#define SiS_SD2_NOOVERLAY      0x80000000   /* No video overlay */

#define SiS_SD3_OLDGAMMAINUSE  0x00000001   /* Old gamma brightness is currently in use */
#define SiS_SD3_MFBALLOWOFFCL  0x00000002   /* Supports off'ing CRTx in MFB if a clone mode is active */
#define SiS_SD3_SUPPORTVBF34   0x00000004   /* Supports VBFlags3 and VBFlags4 */
#define SiS_SD3_SUPPORTDUALDVI 0x00000008   /* Supports dual dvi-d (for future use) */
#define SiS_SD3_SUPPORTDUALTV  0x00000010   /* Supports dual tv (for future use) */
#define SiS_SD3_NEWOUTPUTSW    0x00000020   /* Supports NEWSETVBFLAGS (for future use) */
#define SiS_SD3_CRT1SATGAIN    0x00000040   /* Supports CRT1 saturation gain */
#define SiS_SD3_CRT2SATGAIN    0x00000080   /* Supports CRT2 saturation gain (apart from TV, see SiS_SD2_SUPPTVSAT) */

#define SIS_DIRECTKEY          0x03145792

/* SiSCtrl: Check mode for CRT2 */
#define SiS_CF2_LCD          0x01
#define SiS_CF2_TV           0x02
#define SiS_CF2_VGA2         0x04
#define SiS_CF2_TVPAL        0x08
#define SiS_CF2_TVNTSC       0x10  /* + NTSC-J */
#define SiS_CF2_TVPALM       0x20
#define SiS_CF2_TVPALN       0x40
#define SiS_CF2_CRT1LCDA     0x80
#define SiS_CF2_TYPEMASK     (SiS_CF2_LCD | SiS_CF2_TV | SiS_CF2_VGA2 | SiS_CF2_CRT1LCDA)
#define SiS_CF2_TVSPECIAL    (SiS_CF2_LCD | SiS_CF2_TV)
#define SiS_CF2_TVSPECMASK   (SiS_CF2_TVPAL | SiS_CF2_TVNTSC | SiS_CF2_TVPALM | SiS_CF2_TVPALN)
#define SiS_CF2_TVHIVISION   SiS_CF2_TVPAL
#define SiS_CF2_TVYPBPR525I  SiS_CF2_TVNTSC
#define SiS_CF2_TVYPBPR525P  (SiS_CF2_TVPAL | SiS_CF2_TVNTSC)
#define SiS_CF2_TVYPBPR625I  SiS_CF2_TVPALN
#define SiS_CF2_TVYPBPR625P  (SiS_CF2_TVPALN | SiS_CF2_TVPAL)
#define SiS_CF2_TVYPBPR750P  SiS_CF2_TVPALM
#define SiS_CF2_TVYPBPR1080I (SiS_CF2_TVPALM | SiS_CF2_TVPAL)

/* AGP stuff for DRI */
#define AGP_PAGE_SIZE 4096
#define AGP_PAGES     2048	 /* Default: 2048 pages @ 4096 = 8MB */
/* 300 */
#define AGP_CMDBUF_PAGES 256
#define AGP_CMDBUF_SIZE (AGP_PAGE_SIZE * AGP_CMDBUF_PAGES)
/* 315/330 */
#define AGP_VTXBUF_PAGES 512
#define AGP_VTXBUF_SIZE (AGP_PAGE_SIZE * AGP_VTXBUF_PAGES)

/* Defines for our own vgaHW functions */
#define SISVGA_SR_MODE	 0x01
#define SISVGA_SR_FONTS	 0x02
#define SISVGA_SR_CMAP	 0x04
#define SISVGA_SR_ALL	 (SISVGA_SR_MODE | SISVGA_SR_FONTS | SISVGA_SR_CMAP)

#define SISKGA_FIX_OVERSCAN   1 /* overcan correction required */
#define SISKGA_ENABLE_ON_ZERO 2 /* if possible enable display at beginning of next scanline/frame */
#define SISKGA_BE_TOT_DEC     4 /* always fix problem by setting blank end */

/* CPU flags (for memcpy() etc.) */
#define SIS_CPUFL_LIBC  0x0001
#define SIS_CPUFL_BI    0x0002
#define SIS_CPUFL_SSE   0x0004
#define SIS_CPUFL_MMX   0x0008
#define SIS_CPUFL_3DNOW 0x0010
#define SIS_CPUFL_MMX2  0x0020
#define SIS_CPUFL_BI2   0x0040
#define SIS_CPUFL_SSE2  0x0080
#define SIS_CPUFL_FLAG  0x8000

/* Convenience macro for sisfb version checks */
#define SISFB_VERSION(a,b,c) ((a << 16) | (b << 8) | c)

/* For backup of register contents */
typedef struct {
    UChar  sisRegMiscOut;
    UChar  sisRegsATTR[22];
    UChar  sisRegsGR[10];
    UChar  sisDAC[768];
    UChar  sisRegs3C4[0x80];
    UChar  sisRegs3D4[0x100];
    UChar  sisRegs3C2;
    UChar  sisCapt[0x60];
    UChar  sisVid[0x50];
    UChar  VBPart1[0x80];
    UChar  VBPart2[0x100];
    UChar  VBPart3[0x50];
    UChar  VBPart4[0x50];
    UShort ch70xx[64];
    unsigned int sisMMIO85C0;
    UChar  sis6326tv[0x46];
    unsigned int sisRegsPCI50, sisRegsPCIA0;
    UChar  BIOSModeSave;
} SISRegRec, *SISRegPtr;

typedef struct _sisModeInfoPtr {
    int width;
    int height;
    int bpp;
    int n;
    struct _sisModeInfoPtr *next;
} sisModeInfoRec, *sisModeInfoPtr;

/* SISFBLayout (which has nothing to do with sisfb, actually)
 * is mainly there because of DGA. It holds the current layout
 * parameters needed for acceleration and other stuff. When
 * switching mode using DGA, these are set up accordingly and
 * not necessarily match pScrn's. Therefore, driver modules
 * should read these values instead of pScrn's.
 */
typedef struct {
    int		    bitsPerPixel;    /* = pScrn->bitsPerPixel */
    int		    depth;	     /* = pScrn->depth */
    int		    displayWidth;    /* = pScrn->displayWidth */
    int		    displayHeight;   /* = imageHeight from DGA mode; ONLY WHEN DGA IS ACTIVE!!! */
    int		    DGAViewportX;
    int		    DGAViewportY;
    DisplayModePtr  mode;	     /* = pScrn->currentMode */
} SISFBLayout;

/* For extended memcpy() */
typedef void (*vidCopyFunc)(UChar *, const UChar *, int);

#ifdef SISISXORG6899900
#define SISAGPHTYPE drm_handle_t
#else
#define SISAGPHTYPE ULong
#endif

/* Dual head private entity structure */
#ifdef SISDUALHEAD
typedef struct {
    ScrnInfoPtr		pScrn_1;
    ScrnInfoPtr		pScrn_2;
    UChar		*BIOS;
    struct SiS_Private	*SiS_Pr;
#ifdef SISDRI
    SISAGPHTYPE		agpHandle;
    ULong		agpAddr;
    UChar		*agpBase;
    unsigned int	agpSize;
    unsigned int	agpWantedSize;
    unsigned int	agpWantedPages;
    ULong		agpCmdBufAddr;		/* 300 series */
    UChar		*agpCmdBufBase;
    unsigned int	agpCmdBufSize;
    unsigned int	agpCmdBufFree;
    ULong		agpVtxBufAddr;		/* 315/330 series */
    UChar       	*agpVtxBufBase;
    unsigned int	agpVtxBufSize;
    unsigned int	agpVtxBufFree;
    sisRegion		agp;
    int			drmSubFD;
#endif
    Bool		AGPInitOK;
    int			CRT1ModeNo;		/* Current display mode for CRT1 */
    DisplayModePtr	CRT1DMode;		/* Current display mode for CRT1 */
    int 		CRT2ModeNo;		/* Current display mode for CRT2 */
    DisplayModePtr	CRT2DMode;		/* Current display mode for CRT2 */
    Bool		CRT2ModeSet;		/* CRT2 mode has been set */
    Bool		CRT2IsCustom;
    UChar		CRT2CR30, CRT2CR31, CRT2CR35, CRT2CR38;
    int			refCount;
    int			lastInstance;		/* number of entities */
    Bool		DisableDual;		/* Emergency flag */
    Bool		ErrorAfterFirst;	/* Emergency flag: Error after first init -> Abort second */
    Bool		HWCursor;		/* Backup master settings for use on slave */
    Bool		TurboQueue;
    int			ForceCRT1Type;
    Bool		CRT1TypeForced;
    int			ForceCRT2Type;
    int			OptTVStand;
    int			OptTVOver;
    int			OptTVSOver;
    int			OptROMUsage;
    int			OptUseOEM;
    Bool		NoAccel;
    Bool		useEXA;
    int			forceCRT1;
    int			DSTN, FSTN;
    Bool		XvOnCRT2;
    int			maxUsedClock;		/* Max used pixelclock on master head */
    ULong		masterFbAddress;	/* Framebuffer addresses and sizes */
    ULong		masterFbSize;
    ULong		slaveFbAddress;
    ULong		slaveFbSize;
    void		*FbBase;         	/* VRAM linear address */
    UChar		*RealFbBase;         	/* Real VRAM linear address (for DHM, SiS76x UMA skipping) */
    void		*IOBase;         	/* MMIO linear address */
    UShort		MapCountIOBase;		/* map/unmap queue counter */
    UShort		MapCountFbBase;		/* map/unmap queue counter */
    Bool		forceUnmapIOBase;	/* ignore counter and unmap */
    Bool		forceUnmapFbBase;	/* ignore counter and unmap */
#ifdef __alpha__
    void		*IOBaseDense;    	/* MMIO for Alpha platform */
    UShort		MapCountIOBaseDense;
    Bool		forceUnmapIOBaseDense;  /* ignore counter and unmap */
#endif
    int			chtvlumabandwidthcvbs;  /* TV settings for Chrontel TV encoder */
    int			chtvlumabandwidthsvideo;
    int			chtvlumaflickerfilter;
    int			chtvchromabandwidth;
    int			chtvchromaflickerfilter;
    int			chtvcvbscolor;
    int			chtvtextenhance;
    int			chtvcontrast;
    int			sistvedgeenhance;	/* TV settings for SiS bridge */
    int			sistvantiflicker;
    int			sistvsaturation;
    int			sistvcolcalibc;
    int			sistvcolcalibf;
    int			sistvcfilter;
    int			sistvyfilter;
    int			tvxpos, tvypos;
    int			tvxscale, tvyscale;
    int			siscrt1satgain;
    Bool		crt1satgaingiven;
    int			ForceTVType, SenseYPbPr;
    unsigned int	ForceYPbPrType, ForceYPbPrAR;
    int			chtvtype;
    int			NonDefaultPAL, NonDefaultNTSC;
    UShort		tvx, tvy;
    UChar		p2_01, p2_02, p2_1f, p2_20, p2_43, p2_42, p2_2b;
    UChar		p2_44, p2_45, p2_46;
    unsigned int	sistvccbase;
    UChar		p2_35, p2_36, p2_37, p2_38, p2_48, p2_49, p2_4a;
    UChar		p2_0a, p2_2f, p2_30, p2_47;
    UChar		scalingp1[9], scalingp4[9], scalingp2[64];
    UShort		cursorBufferNum;
    Bool		restorebyset;
    Bool		CRT1gamma, CRT1gammaGiven, CRT2gamma, XvGamma, XvGammaGiven, XvDefAdaptorBlit;
    int			XvGammaRed, XvGammaGreen, XvGammaBlue;
    int			GammaBriR, GammaBriG, GammaBriB;		/* strictly for Xinerama */
    float		NewGammaBriR, NewGammaBriG, NewGammaBriB;	/* strictly for Xinerama */
    float		NewGammaConR, NewGammaConG, NewGammaConB;	/* strictly for Xinerama */
    unsigned int	CRT1MonGamma, CRT2MonGamma;
    unsigned int	CRT1VGAMonitorGamma, CRT2LCDMonitorGamma, CRT2VGAMonitorGamma;
    int			curxvcrtnum;
    int			UsePanelScaler, CenterLCD;
    int			AllowHotkey;
    Bool		enablesisctrl;
    unsigned int	cmdQ_SharedWritePort_2D;
    UChar		*RenderAccelArray;
    UChar		*FbBase1;
    ULong		OnScreenSize1;
    UChar		OldMode;
    int			HWCursorMBufNum, HWCursorCBufNum;
    Bool		ROM661New;
    Bool		HaveXGIBIOS;
    Bool		XvUseMemcpy;
    Bool		BenchMemCpy;
    Bool		HaveFastVidCpy;
    vidCopyFunc		SiSFastVidCopy, SiSFastMemCopy;
    vidCopyFunc		SiSFastVidCopyFrom, SiSFastMemCopyFrom;
    unsigned int	CPUFlags;
#ifdef SIS_NEED_MAP_IOP
    CARD32		IOPAddress;		/* I/O port physical address */
    void		*IOPBase;		/* I/O port linear address */
    UShort		MapCountIOPBase;	/* map/unmap queue counter */
    Bool		forceUnmapIOPBase;	/* ignore counter and unmap */
#endif
#ifdef SIS_CP
    SIS_CP_H_ENT
#endif
} SISEntRec, *SISEntPtr;
#endif

#define SISPTR(p)       ((SISPtr)((p)->driverPrivate))
#define XAAPTR(p)       ((XAAInfoRecPtr)(SISPTR(p)->AccelInfoPtr))

/* MergedFB: Relative position */
typedef enum {
   sisLeftOf,
   sisRightOf,
   sisAbove,
   sisBelow,
   sisClone
} SiSScrn2Rel;

typedef struct _region {
    int x0,x1,y0,y1;
} region;

typedef struct {
    ScrnInfoPtr		pScrn;
    pciVideoPtr		PciInfo;
    int			PciBus, PciDevice, PciFunc;
#ifndef XSERVER_LIBPCIACCESS
    PCITAG		PciTag;
#endif
    EntityInfoPtr	pEnt;
    int			Chipset;
    unsigned char	ChipType;
    int			ChipRev;
    int			VGAEngine;	/* see above */
    int			hasTwoOverlays;	/* Chipset supports two video overlays? */
    struct SiS_Private	*SiS_Pr;	/* For mode switching code */
    int			DSTN;		/* For 550 FSTN/DSTN; set by option, no detection */
    ULong		FbAddress;	/* VRAM physical address (in DHM: for each Fb!) */
    ULong		realFbAddress;	/* For DHM/PCI mem mapping: store global FBAddress */
    void 		*FbBase;	/* VRAM virtual linear address */
    void 		*RealFbBase;	/* Real VRAM virtual linear address (for DHM and SiS76x UMA skipping) */
    CARD32		IOAddress;	/* MMIO physical address */
    void		*IOBase;	/* MMIO linear address */
    unsigned long	IODBase;	/* Base of PIO memory area */
#ifdef __alpha__
    void		*IOBaseDense;	/* MMIO for Alpha platform */
#endif
    SISIOADDRESS        RelIO;		/* Relocated IO Ports baseaddress */
    UChar		*BIOS;
    int			MemClock;
    int			BusWidth;
    int			MinClock;
    int			MaxClock;
    int			Flags;		/* HW config flags */
    long		FbMapSize;	/* Used for Mem Mapping - DON'T CHANGE THIS */
    long		availMem;	/* Really available Fb mem (minus TQ, HWCursor) */
    unsigned int	maxxfbmem;	/* limit fb memory X is to use to this (KB) */
    unsigned int	sisfbHeapStart;	/* heapstart of sisfb (if running) */
    unsigned int	dhmOffset;	/* Offset to memory for each head (0 or ..); also used on SiS76x/UMA+LFB */
    unsigned int	FbBaseOffset;
    DGAModePtr		DGAModes;
    int			numDGAModes;
    Bool		DGAactive;
    int			DGAViewportStatus;
    UChar       	OldMode;	/* Back old modeNo (if available) */
    Bool		NoAccel;
    Bool		NoXvideo;
    Bool		XvOnCRT2;	/* see sis_opt.c */
    Bool		HWCursor;
    Bool		UsePCIRetry;
    Bool		TurboQueue;
    int			VESA;
    int			ForceCRT1Type;
    Bool		CRT1Detected, CRT1TypeForced;
    int			ForceCRT2Type;
    int			OptTVStand;
    int			OptTVOver;
    int			OptROMUsage;
    int			UseCHOverScan;
    Bool		ValidWidth;
    Bool		FastVram;		/* now unused */
    int			forceCRT1;
    Bool		CRT1changed;
    UChar		oldCR17, oldCR63, oldSR1F;
    UChar		oldCR32, oldCR36, oldCR37;
    UChar		myCR32, myCR36, myCR37, myCR63;
    UChar		newCR32;
    unsigned int	VBFlags;		/* Video bridge configuration (dynamic) */
    unsigned int	VBFlags2;		/* Video bridge configuration 2 (static flags only) */
    unsigned int	VBFlags3, VBFlags4;	/* Video bridge configuration 3, 4 (dynamic) */
    unsigned int	VBFlags_backup;		/* Backup for SlaveMode-modes */
    unsigned int	VBFlags_backup3;	/* Backup for SlaveMode-modes */
    unsigned int	VBFlags_backup4;	/* Backup for SlaveMode-modes */
    unsigned int	VBLCDFlags, VBLCDFlags2;
    int			ChrontelType;		/* CHRONTEL_700x or CHRONTEL_701x */
    unsigned int	PDC, PDCA;		/* PanelDelayCompensation */
    short		scrnOffset;		/* Screen pitch (data) */
    short		scrnPitch;		/* Screen pitch (display; regarding interlace) */
    short		DstColor;
    unsigned int	SiS310_AccelDepth;	/* used in accel for 315 series */
    int			MaxCMDQueueLen;
    int			CurCMDQueueLen;
    int			MinCMDQueueLen;
    CARD16		CursorSize;		/* Size of HWCursor area (bytes) */
    CARD32		cursorOffset;		/* see sis_driver.c and sis_cursor.c */
    Bool		useEXA;
    void 		(*InitAccel)(ScrnInfoPtr pScrn);
    void 		(*SyncAccel)(ScrnInfoPtr pScrn);
    void		(*FillRect)(ScrnInfoPtr pScrn, int x, int y, int w, int h, int color);
    void		(*BlitRect)(ScrnInfoPtr pScrn, int srcx, int srcy, int dstx, int dsty,
					int w, int h, int color);
    int			CommandReg;
    Bool		ClipEnabled;
    int			Xdirection;		/* for temp use in accel */
    int			Ydirection;		/* for temp use in accel */
#ifdef SIS_USE_XAA
    XAAInfoRecPtr	AccelInfoPtr;
    UChar 		*XAAScanlineColorExpandBuffers[2];
    Bool		DoColorExpand;
    Bool		ColorExpandBusy;
    int			xcurrent;		/* for temp use in accel */
    int			ycurrent;		/* for temp use in accel */
    int			sisPatternReg[4];
    int			ROPReg;
#endif
#ifdef SIS_USE_EXA
    ExaDriverPtr	EXADriverPtr;
    int			fillPitch, fillBpp;
    CARD32		fillDstBase;
    int			copyBpp;
    int			copySPitch, copyDPitch;
    CARD32		copySrcBase, copyDstBase;
    int			copyXdir, copyYdir;
    ExaOffscreenArea *	exa_scratch;
    unsigned int 	exa_scratch_next;
#endif
    Bool		alphaBlitBusy;
    SISRegRec		SavedReg;
    SISRegRec		ModeReg;
    xf86CursorInfoPtr	CursorInfoPtr;
    CloseScreenProcPtr	CloseScreen;
    Bool		(*ModeInit)(ScrnInfoPtr pScrn, DisplayModePtr mode);
    void		(*SiSSave)(ScrnInfoPtr pScrn, SISRegPtr sisreg);
    void		(*SiSRestore)(ScrnInfoPtr pScrn, SISRegPtr sisreg);
    int			cmdQueueLen;		/* Current cmdQueueLength (for 2D and 3D) */
    unsigned int	*cmdQueueBase;
    int			*cmdQueueLenPtr;	/* Ptr to variable holding the current queue length */
    int			*cmdQueueLenPtrBackup;	/* Backup for DRI init/restore */
    unsigned int	cmdQueueOffset;
    unsigned int	cmdQueueSize;
    unsigned int	cmdQueueSizeMask;
    unsigned int	cmdQ_SharedWritePort_2D;
    unsigned int	*cmdQ_SharedWritePort;
    unsigned int	*cmdQ_SharedWritePortBackup;
    unsigned int	cmdQueueSize_div2;
    unsigned int	cmdQueueSize_div4;
    unsigned int	cmdQueueSize_4_3;
#ifdef SISDRI
    SISAGPHTYPE		agpHandle;
    ULong		agpAddr;
    UChar 		*agpBase;
    unsigned int	agpSize;
    unsigned int	agpWantedSize;
    unsigned int	agpWantedPages;
    ULong		agpCmdBufAddr;	/* 300 series */
    UChar		*agpCmdBufBase;
    unsigned int	agpCmdBufSize;
    unsigned int	agpCmdBufFree;
    ULong		agpVtxBufAddr;	/* 315 series */
    UChar		*agpVtxBufBase;
    unsigned int	agpVtxBufSize;
    unsigned int	agpVtxBufFree;
    sisRegion		agp;
#endif
    Bool		AGPInitOK;
    Bool		irqEnabled;
    int			irq;
    Bool		IsAGPCard, IsPCIExpress;
    unsigned int	DRIheapstart, DRIheapend;
    Bool		NeedFlush;	/* Need to flush cmd buf mem (760) */

#ifdef SIS_USE_XAA
    void		(*RenderCallback)(ScrnInfoPtr);
    Time		RenderTime;
    FBLinearPtr		AccelLinearScratch;
#endif
#ifdef SIS_USE_EXA
    void		(*ExaRenderCallback)(ScrnInfoPtr);
    Time		ExaRenderTime;
#endif
    UChar		*RenderAccelArray;
    Bool		doRender;

    int			ColorExpandRingHead;
    int			ColorExpandRingTail;
    int			PerColorExpandBufferSize;
    int			ColorExpandBufferNumber;
    int			ColorExpandBufferCountMask;
    UChar		*ColorExpandBufferAddr[32];
    CARD32		ColorExpandBufferScreenOffset[32];
    CARD32		ColorExpandBase;

    int			Rotate, Reflect;
    void		(*PointerMoved)(SCRN_ARG_TYPE arg, int x, int y);

    /* ShadowFB support */
    Bool		ShadowFB;
    UChar		*ShadowPtr;
    int			ShadowPitch;

#ifdef SISUSEDEVPORT
    Bool		sisdevportopen;
#endif

    /* DRI */
    Bool		loadDRI;
#ifdef SISDRI
    Bool		directRenderingEnabled;
    DRIInfoPtr 		pDRIInfo;
    int			drmSubFD;
    int			numVisualConfigs;
    __GLXvisualConfig*	pVisualConfigs;
    SISConfigPrivPtr	pVisualConfigsPriv;
    SISRegRec		DRContextRegs;
#endif

    /* Xv */
    XF86VideoAdaptorPtr adaptor;
    XF86VideoAdaptorPtr blitadaptor;
    void		*blitPriv;
    ScreenBlockHandlerProcPtr BlockHandler;
    void		(*VideoTimerCallback)(ScrnInfoPtr, Time);
    void		(*ResetXv)(ScrnInfoPtr);
    void		(*ResetXvDisplay)(ScrnInfoPtr);
    void		(*ResetXvGamma)(ScrnInfoPtr);

    /* misc */
    OptionInfoPtr	Options;
    UChar		BIOSModeSave;
    int			CRT1off;		/* 1=CRT1 off, 0=CRT1 on */
    CARD16		LCDheight;		/* Vertical resolution of LCD panel */
    CARD16		LCDwidth;		/* Horizontal resolution of LCD panel */
    vbeInfoPtr 		pVbe;			/* For VESA mode switching */
    CARD16		vesamajor;
    CARD16		vesaminor;
    int			UseVESA;
    sisModeInfoPtr      SISVESAModeList;
    xf86MonPtr		monitor;
    CARD16		maxBytesPerScanline;
    CARD32		*pal, *savedPal;
    int			mapPhys, mapOff, mapSize;
    int			statePage, stateSize, stateMode;
    CARD8		*fonts;
    CARD8		*state, *pstate;
    void		*base, *VGAbase;
#ifdef SISDUALHEAD
    Bool		DualHeadMode;		/* TRUE if we use dual head mode */
    Bool		SecondHead;		/* TRUE is this is the second head */
    SISEntPtr		entityPrivate;		/* Ptr to private entity (see above) */
    Bool		SiSXinerama;		/* Do we use Xinerama mode? */
#endif
    SISFBLayout		CurrentLayout;		/* Current framebuffer layout */
    UShort		SiS_DDC2_Index;
    UShort		SiS_DDC2_Data;
    UShort		SiS_DDC2_Clk;
    Bool		Primary;		/* Display adapter is primary */
    Bool		VGADecodingEnabled;	/* a0000 memory adress decoding is enabled */
    xf86Int10InfoPtr	pInt;			/* Our int10 */
    int			oldChipset;		/* Type of old chipset */
    int			RealVideoRam;		/* 6326 can only address 4MB, but TQ can be above */
    CARD32		CmdQueLenMask;		/* Mask of queue length in MMIO register */
    CARD32		CmdQueLenFix;           /* Fix value to subtract from QueLen (530/620) */
    CARD32		CmdQueMaxLen;           /* (6326/5597/5598) Amount of cmds the queue can hold */
    CARD32		TurboQueueLen;		/* For future use */
    CARD32		detectedCRT2Devices;	/* detected CRT2 devices before mask-out */
    Bool		HostBus;		/* Enable/disable 5597/5598 host bus */
    Bool		noInternalModes;	/* Use our own default modes? */
    int			OptUseOEM;		/* Use internal OEM data? */
    int			chtvlumabandwidthcvbs;  /* TV settings for Chrontel TV encoder */
    int			chtvlumabandwidthsvideo;
    int			chtvlumaflickerfilter;
    int			chtvchromabandwidth;
    int			chtvchromaflickerfilter;
    int			chtvcvbscolor;
    int			chtvtextenhance;
    int			chtvcontrast;
    int			sistvedgeenhance;	/* TV settings for SiS bridges */
    int			sistvantiflicker;
    int			sistvsaturation;
    int			sistvcolcalibc;
    int			sistvcolcalibf;
    int			sistvcfilter;
    int			sistvyfilter;
    int			OptTVSOver;		/* Chrontel 7005: Superoverscan */
    int			tvxpos, tvypos;
    int			tvxscale, tvyscale;
    int			SiS6326Flags;		/* SiS6326 TV settings */
    int			sis6326enableyfilter;
    int			sis6326yfilterstrong;
    int			sis6326tvplug;
    int			sis6326fscadjust;
    Bool		sisfbfound;
    Bool		donttrustpdc;		/* Don't trust the detected PDC */
    UChar		sisfbpdc, sisfbpdca;
    UChar       	sisfblcda;
    int			sisfbscalelcd;
    unsigned int	sisfbspecialtiming;
    Bool		sisfb_haveemi, sisfb_haveemilcd, sisfb_tvposvalid, sisfb_havelock;
    UChar		sisfb_emi30,sisfb_emi31,sisfb_emi32,sisfb_emi33;
    int			sisfb_tvxpos, sisfb_tvypos;
    int			siscrt1satgain;
    Bool		crt1satgaingiven;
    Bool		sisfbHaveNewHeapDef;
    unsigned int	sisfbHeapSize, sisfbVideoOffset;
    Bool		sisfbxSTN;
    unsigned int	sisfbDSTN, sisfbFSTN;
    Bool		sisfbcanpost, sisfbcardposted, sisfbprimary;
    char		sisfbdevname[16];
    int			EMI;
    int			PRGB;
    int			NoYV12;			/* Disable Xv YV12 support (old series) */
    UChar       	postVBCR32;
    int			newFastVram;		/* Replaces FastVram */
    int			ForceTVType, SenseYPbPr;
    int			NonDefaultPAL, NonDefaultNTSC;
    unsigned int	ForceYPbPrType, ForceYPbPrAR;
    ULong		lockcalls;		/* Count unlock calls for debug */
    UShort		tvx, tvy;		/* Backup TV position registers */
    UChar		p2_01, p2_02, p2_1f, p2_20, p2_43, p2_42, p2_2b; /* Backup TV position registers */
    UShort      	tvx1, tvx2, tvx3, tvy1;	/* Backup TV position registers */
    UChar		p2_44, p2_45, p2_46;
    unsigned int	sistvccbase;
    UChar		p2_35, p2_36, p2_37, p2_38, p2_48, p2_49, p2_4a;
    UChar		p2_0a, p2_2f, p2_30, p2_47;
    UChar		scalingp1[9], scalingp4[9], scalingp2[64];
    Bool		ForceCursorOff;
    Bool		HaveCustomModes;
    Bool		IsCustom;
    DisplayModePtr	backupmodelist;
    int			chtvtype;
    Atom		xvBrightness, xvContrast, xvColorKey, xvHue, xvSaturation;
    Atom		xvAutopaintColorKey, xvSetDefaults, xvSwitchCRT;
    Atom		xvDisableGfx, xvDisableGfxLR, xvTVXPosition, xvTVYPosition;
    Atom		xvDisableColorkey, xvUseChromakey, xvChromaMin, xvChromaMax;
    Atom		xvInsideChromakey, xvYUVChromakey, xvVSync;
#ifdef SISDEINT
    Atom		xvdeintmeth;
#endif
    Atom		xvGammaRed, xvGammaGreen, xvGammaBlue;
#ifdef XV_SD_DEPRECATED
    Atom		xv_QVF, xv_QVV, xv_USD, xv_SVF, xv_QDD, xv_TAF, xv_TSA, xv_TEE, xv_GSF;
    Atom		xv_TTE, xv_TCO, xv_TCC, xv_TCF, xv_TLF, xv_CMD, xv_CMDR, xv_CT1, xv_SGA;
    Atom		xv_GDV, xv_GHI, xv_OVR, xv_GBI, xv_TXS, xv_TYS, xv_CFI, xv_COC, xv_COF;
    Atom		xv_YFI, xv_GSS, xv_BRR, xv_BRG, xv_BRB, xv_PBR, xv_PBG, xv_PBB, xv_SHC;
    Atom		xv_BRR2, xv_BRG2, xv_BRB2, xv_PBR2, xv_PBG2, xv_PBB2, xv_PMD, xv_RDT;
    Atom		xv_GARC2,xv_GAGC2,xv_GABC2, xv_GSF2;
    Atom		xv_BRRC2, xv_BRGC2, xv_BRBC2, xv_PBRC2, xv_PBGC2, xv_PBBC2;
#ifdef TWDEBUG
    Atom		xv_STR;
#endif
    unsigned int	xv_sd_result;
#endif /* XV_SD_DEPRECATED */
    int			xv_sisdirectunlocked;
    int			SiS76xLFBSize;
    int			SiS76xUMASize;
    int			CRT1isoff;
    ULong		UMAsize, LFBsize;	/* For SiSCtrl extension info only */
#ifdef SIS_CP
    SIS_CP_H
#endif
    ULong		ChipFlags;
    ULong		SiS_SD_Flags, SiS_SD2_Flags, SiS_SD3_Flags, SiS_SD4_Flags;
    Bool		UseHWARGBCursor;
    int			OptUseColorCursor;
    int			OptUseColorCursorBlend;
    CARD32		OptColorCursorBlendThreshold;
    UShort		cursorBufferNum;
    int			vb;
    Bool		restorebyset;
    Bool		nocrt2ddcdetection;
    Bool		forcecrt2redetection;
    Bool		CRT1gamma, CRT1gammaGiven, CRT2gamma, XvGamma, XvGammaGiven;
    int			XvDefCon, XvDefBri, XvDefHue, XvDefSat;
    Bool		XvDefDisableGfx, XvDefDisableGfxLR, XvDefAdaptorBlit;
    Bool		XvUseMemcpy;
    Bool		XvUseChromaKey, XvDisableColorKey;
    Bool		XvInsideChromaKey, XvYUVChromaKey;
    int			XvChromaMin, XvChromaMax;
    int			XvGammaRed, XvGammaGreen, XvGammaBlue;
    int			XvGammaRedDef, XvGammaGreenDef, XvGammaBlueDef;
    CARD8		XvGammaRampRed[256], XvGammaRampGreen[256], XvGammaRampBlue[256];
    Bool		disablecolorkeycurrent;
    CARD32		colorKey;
    CARD32		MiscFlags;
    int			UsePanelScaler, CenterLCD;
    float		zClearVal;
    ULong		bClrColor, dwColor;
    int			AllowHotkey;
    Bool		enablesisctrl;
    short		Video_MaxWidth, Video_MaxHeight;
    int			FSTN;
    Bool		AddedPlasmaModes;
    short		scrnPitch2;
    CARD32		CurFGCol, CurBGCol;
    UChar		*CurMonoSrc;
    CARD32		*CurARGBDest;
    int			GammaBriR, GammaBriG, GammaBriB;
    unsigned int	CRT1MonGamma, CRT2MonGamma;
    unsigned int	CRT1VGAMonitorGamma, CRT2LCDMonitorGamma, CRT2VGAMonitorGamma;
    Bool		HideHWCursor;  /* Custom application */
    Bool		HWCursorIsVisible;
    unsigned int	HWCursorBackup[16];
    int			HWCursorMBufNum, HWCursorCBufNum;
    ULong		mmioSize;
    Bool		ROM661New;
    Bool		HaveXGIBIOS;
    Bool		NewCRLayout;
    Bool		skipswitchcheck;
    unsigned int	VBFlagsInit;
    DisplayModePtr	currentModeLast;
    unsigned long	MyPIOOffset;
    Bool		OverruleRanges;
    Bool		BenchMemCpy;
    Bool		NeedCopyFastVidCpy;
    Bool		SiSFastVidCopyDone;
    vidCopyFunc		SiSFastVidCopy, SiSFastMemCopy;
    vidCopyFunc		SiSFastVidCopyFrom, SiSFastMemCopyFrom;
    unsigned int	CPUFlags;
#ifndef SISCHECKOSSSE
    Bool		XvSSEMemcpy;
#endif
    char		messagebuffer[64];
    unsigned int	VGAMapSize;		/* SiSVGA stuff */
    ULong		VGAMapPhys;
    void 		*VGAMemBase; /* mapped */
    Bool		VGAPaletteEnabled;
    Bool		VGACMapSaved;
    Bool		CRT2SepGamma;		/* CRT2 separate gamma stuff */
    int			*crt2cindices;
    LOCO		*crt2gcolortable, *crt2colors;
    int			CRT2ColNum;
    float		GammaR2, GammaG2, GammaB2;
    int			GammaR2i, GammaG2i, GammaB2i;
    int			GammaBriR2, GammaBriG2, GammaBriB2;
    float		NewGammaBriR, NewGammaBriG, NewGammaBriB;
    float		NewGammaConR, NewGammaConG, NewGammaConB;
    float		NewGammaBriR2, NewGammaBriG2, NewGammaBriB2;
    float		NewGammaConR2, NewGammaConG2, NewGammaConB2;
    ExtensionEntry	*SiSCtrlExtEntry;
    char		devsectname[32];
    Bool		SCLogQuiet;
#ifdef SIS_NEED_MAP_IOP
    CARD32		IOPAddress;		/* I/O port physical address */
    void 		*IOPBase;		/* I/O port linear address */
#endif
#ifdef SISMERGED
    Bool		MergedFB, MergedFBAuto;
    SiSScrn2Rel		CRT2Position;
    char		*CRT2HSync;
    char		*CRT2VRefresh;
    char		*MetaModes;
    ScrnInfoPtr		CRT2pScrn;
    DisplayModePtr	CRT1Modes;
    DisplayModePtr	CRT1CurrentMode;
    int			CRT1frameX0;
    int			CRT1frameY0;
    int			CRT1frameX1;
    int			CRT1frameY1;
    Bool		CheckForCRT2;
    Bool		IsCustomCRT2;
    Bool		HaveCustomModes2;
    int			maxCRT1_X1, maxCRT1_X2, maxCRT1_Y1, maxCRT1_Y2;
    int			maxCRT2_X1, maxCRT2_X2, maxCRT2_Y1, maxCRT2_Y2;
    int			maxClone_X1, maxClone_X2, maxClone_Y1, maxClone_Y2;
    int			MergedFBXDPI, MergedFBYDPI;
    int			CRT1XOffs, CRT1YOffs, CRT2XOffs, CRT2YOffs;
    int			MBXNR1XMAX, MBXNR1YMAX, MBXNR2XMAX, MBXNR2YMAX;
    Bool		NonRect, HaveNonRect, HaveOffsRegions, MouseRestrictions;
    region		NonRectDead, OffDead1, OffDead2;
#ifdef SISXINERAMA
    Bool		UseSiSXinerama;
    Bool		CRT2IsScrn0;
    ExtensionEntry	*XineramaExtEntry;
    int			SiSXineramaVX, SiSXineramaVY;
    Bool		AtLeastOneNonClone;
#endif
#endif
} SISRec, *SISPtr;

typedef struct _ModeInfoData {
    int mode;
    VbeModeInfoBlock *data;
    VbeCRTCInfoBlock *block;
} ModeInfoData;

#define SDMPTR(x) ((SiSMergedDisplayModePtr)(x->currentMode->Private))
#define CDMPTR    ((SiSMergedDisplayModePtr)(pSiS->CurrentLayout.mode->Private))

#define BOUND(test,low,hi) 			\
    {						\
	if((test) < (low)) (test) = (low);	\
	if((test) > (hi))  (test) = (hi);	\
    }

#define REBOUND(low,hi,test)		\
    {					\
	if((test) < (low)) {		\
		(hi) += (test)-(low);	\
		(low) = (test); 	\
	}				\
	if((test) > (hi)) {		\
		(low) += (test)-(hi);	\
		(hi) = (test); 		\
	}				\
    }

typedef struct _MergedDisplayModeRec {
    DisplayModePtr CRT1;
    DisplayModePtr CRT2;
    SiSScrn2Rel    CRT2Position;
} SiSMergedDisplayModeRec, *SiSMergedDisplayModePtr;

typedef struct _myhddctiming {
    int    whichone;
    UChar  mask;
    float  rate;
} myhddctiming;

typedef struct _myvddctiming {
    int    whichone;
    UChar  mask;
    int    rate;
} myvddctiming;

typedef struct _pdctable {
    int  subsysVendor;
    int  subsysCard;
    int  pdc;
    char *vendorName;
    char *cardName;
} pdctable;

typedef struct _chswtable {
    int  subsysVendor;
    int  subsysCard;
    char *vendorName;
    char *cardName;
} chswtable;

typedef struct _customttable {
    UShort chipID;
    char   *biosversion;
    char   *biosdate;
    CARD32 bioschksum;
    UShort biosFootprintAddr[5];
    UChar  biosFootprintData[5];
    UShort pcisubsysvendor;
    UShort pcisubsyscard;
    char   *vendorName;
    char   *cardName;
    ULong  SpecialID;
    char   *optionName;
} customttable;

#ifdef SISMERGED
#ifdef SISXINERAMA
typedef struct _SiSXineramaData {
    int x;
    int y;
    int width;
    int height;
} SiSXineramaData;
#endif
#endif

extern const customttable SiS_customttable[];

/* prototypes */

extern void  sisSaveUnlockExtRegisterLock(SISPtr pSiS, UChar *reg1, UChar *reg2);
extern void  sisRestoreExtRegisterLock(SISPtr pSiS, UChar reg1, UChar reg2);
extern void  SiSOptions(ScrnInfoPtr pScrn);
extern const OptionInfoRec * SISAvailableOptions(int chipid, int busid);
extern void  SiSSetup(ScrnInfoPtr pScrn);
extern void  SISVGAPreInit(ScrnInfoPtr pScrn);
extern Bool  SiSHWCursorInit(ScreenPtr pScreen);
extern Bool  SiSAccelInit(ScreenPtr pScreen);
extern Bool  SiS300AccelInit(ScreenPtr pScreen);
extern Bool  SiS530AccelInit(ScreenPtr pScreen);
extern Bool  SiS315AccelInit(ScreenPtr pScreen);
extern void  SISInitVideo(ScreenPtr pScreen);
extern void  SIS6326InitVideo(ScreenPtr pScreen);
extern Bool  SISDGAInit(ScreenPtr pScreen);

/* For extended mempy() support */
extern unsigned int SiSGetCPUFlags(ScrnInfoPtr pScrn);
extern vidCopyFunc SiSVidCopyInit(ScreenPtr pScreen, vidCopyFunc *UMemCpy, Bool from);
extern vidCopyFunc SiSVidCopyGetDefault(void);

extern void  SiSMemCopyToVideoRam(SISPtr pSiS, UChar *to, UChar *from, int size);
extern void  SiSMemCopyFromVideoRam(SISPtr pSiS, UChar *to, UChar *from, int size);

extern void  SiS_SetCHTVlumabandwidthcvbs(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetCHTVlumabandwidthsvideo(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetCHTVlumaflickerfilter(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetCHTVchromabandwidth(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetCHTVchromaflickerfilter(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetCHTVcvbscolor(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetCHTVtextenhance(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetCHTVcontrast(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetSISTVedgeenhance(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetSISTVantiflicker(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetSISTVsaturation(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetSISTVcfilter(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetSISTVyfilter(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetSISTVcolcalib(ScrnInfoPtr pScrn, int val, Bool coarse);
extern void  SiS_SetSIS6326TVantiflicker(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetSIS6326TVenableyfilter(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetSIS6326TVyfilterstrong(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetTVxposoffset(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetTVyposoffset(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetTVxscale(ScrnInfoPtr pScrn, int val);
extern void  SiS_SetTVyscale(ScrnInfoPtr pScrn, int val);
extern int   SiS_GetCHTVlumabandwidthcvbs(ScrnInfoPtr pScrn);
extern int   SiS_GetCHTVlumabandwidthsvideo(ScrnInfoPtr pScrn);
extern int   SiS_GetCHTVlumaflickerfilter(ScrnInfoPtr pScrn);
extern int   SiS_GetCHTVchromabandwidth(ScrnInfoPtr pScrn);
extern int   SiS_GetCHTVchromaflickerfilter(ScrnInfoPtr pScrn);
extern int   SiS_GetCHTVcvbscolor(ScrnInfoPtr pScrn);
extern int   SiS_GetCHTVtextenhance(ScrnInfoPtr pScrn);
extern int   SiS_GetCHTVcontrast(ScrnInfoPtr pScrn);
extern int   SiS_GetSISTVedgeenhance(ScrnInfoPtr pScrn);
extern int   SiS_GetSISTVantiflicker(ScrnInfoPtr pScrn);
extern int   SiS_GetSISTVsaturation(ScrnInfoPtr pScrn);
extern int   SiS_GetSISTVcfilter(ScrnInfoPtr pScrn);
extern int   SiS_GetSISTVyfilter(ScrnInfoPtr pScrn);
extern int   SiS_GetSISTVcolcalib(ScrnInfoPtr pScrn, Bool coarse);
extern int   SiS_GetSIS6326TVantiflicker(ScrnInfoPtr pScrn);
extern int   SiS_GetSIS6326TVenableyfilter(ScrnInfoPtr pScrn);
extern int   SiS_GetSIS6326TVyfilterstrong(ScrnInfoPtr pScrn);
extern int   SiS_GetTVxposoffset(ScrnInfoPtr pScrn);
extern int   SiS_GetTVyposoffset(ScrnInfoPtr pScrn);
extern int   SiS_GetTVxscale(ScrnInfoPtr pScrn);
extern int   SiS_GetTVyscale(ScrnInfoPtr pScrn);
extern int   SiS_GetSISCRT1SaturationGain(ScrnInfoPtr pScrn);
extern void  SiS_SetSISCRT1SaturationGain(ScrnInfoPtr pScrn, int val);

extern unsigned int sis_pci_read_device_u32(int device, int offset);
extern unsigned char sis_pci_read_device_u8(int device, int offset);
extern unsigned int sis_pci_read_host_bridge_u32(int offset);
extern unsigned char sis_pci_read_host_bridge_u8(int offset);
extern void sis_pci_write_host_bridge_u8(int offset, unsigned char value);
extern void sis_pci_write_host_bridge_u32(int offset, unsigned int value);
#endif  /* _SIS_H_ */



