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
 * Author:   Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifndef _SISUSB_H
#define _SISUSB_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>

#define SISUSBDRIVERVERSIONYEAR    5
#define SISUSBDRIVERVERSIONMONTH   9
#define SISUSBDRIVERVERSIONDAY     28
#define SISUSBDRIVERREVISION       1

#define SISUSBDRIVERIVERSION ((SISUSBDRIVERVERSIONYEAR << 16) |  \
			     (SISUSBDRIVERVERSIONMONTH << 8)  |  \
                             SISUSBDRIVERVERSIONDAY 	      |  \
			     (SISUSBDRIVERREVISION << 24))

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

#ifndef SIS_LINUX		/* USB dongle driver only for Linux */
#error sisusb driver can only be compiled for Linux
#endif

#define SISUSB_NAME                "SISUSB"
#define SISUSB_DRIVER_NAME         "sisusb"

#define SISUSB_MAJOR_VERSION       PACKAGE_VERSION_MAJOR
#define SISUSB_MINOR_VERSION       PACKAGE_VERSION_MINOR
#define SISUSB_PATCHLEVEL          PACKAGE_VERSION_PATCHLEVEL
#define SISUSB_CURRENT_VERSION     ((SISUSB_MAJOR_VERSION << 16) | 	\
                                   (SISUSB_MINOR_VERSION << 8)   |	\
				   SISUSB_PATCHLEVEL)

#if 0
#define TWDEBUG    /* for debugging */
#endif

#include "compiler.h"
#include "xf86Priv.h"
#include "xf86_OSproc.h"
#include "xf86.h"
#include "xf86Cursor.h"
#include "xf86cmap.h"
#include "xaa.h"

#define SISUSB_HaveDriverFuncs 0

#ifdef XORG_VERSION_CURRENT
#include "xorgVersion.h"
#define SISUSBMYSERVERNAME "X.org"
#ifndef XF86_VERSION_NUMERIC
#define XF86_VERSION_NUMERIC(major,minor,patch,snap) \
	(((major) * 10000000) + ((minor) * 100000) + ((patch) * 1000) + snap)
#define XF86_VERSION_CURRENT XF86_VERSION_NUMERIC(4,3,99,902)
#endif
#ifdef HaveDriverFuncs
#define SISUSB_HAVE_DRIVER_FUNC
#undef  SISUSB_HaveDriverFuncs
#define SISUSB_HaveDriverFuncs HaveDriverFuncs
#endif
#else
#include "xf86Version.h"
#define SISUSBMYSERVERNAME "XFree86"
#endif

#if (XF86_VERSION_CURRENT >= XF86_VERSION_NUMERIC(4,3,99,0)) || (defined(XvExtension))
#include "xf86xv.h"
#include <X11/extensions/Xv.h>
#endif

/* Platform/architecture related definitions: */

#undef SIS_PC_PLATFORM
#undef SIS_USE_BIOS_SCRATCH
#undef SIS_NEED_MAP_IOP
#undef SISUSEDEVPORT

/* Our #includes: Require the arch/platform dependent #defines above */

#include "sisusb_osdef.h"
#include "sisusb_types.h"
#include "sisusb_struct.h"

/* Configurable stuff: ------------------------------------- */

#define SIS_ARGB_CURSOR		/* Include code for color hardware cursors */

#define SISVRAMQ		/* Use VRAM queue mode */

#undef SIS_ENABLEXV		/* Enable/Disable Xv overlay support */

#undef XV_SD_DEPRECATED		/* Include deprecated Xv interface for SiSCtrl */

/* End of configurable stuff --------------------------------- */

#define UNLOCK_ALWAYS		/* Always unlock the registers (should be set!) */

/* Need that for SiSCtrl */
#define NEED_REPLIES		/* ? */
#define EXTENSION_PROC_ARGS void *
#include "extnsionst.h" 			/* required */
#include <X11/extensions/panoramiXproto.h>	/* required */

#undef SISGAMMARAMP
#ifdef XORG_VERSION_CURRENT
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(6,8,99,13,0) || XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(4,0,0,0,0)
#define SISGAMMARAMP		/* Driver can set gamma ramp; requires additional symbols in xf86sym.h */
#endif
#endif

#undef SIS_GLOBAL_ENABLEXV
#if defined(XV_SD_DEPRECATED) || defined(SIS_ENABLEXV)
#define SIS_GLOBAL_ENABLEXV
#endif

#ifdef TWDEBUG
#define SISUSBVERBLEVEL 3
#else
#define SISUSBVERBLEVEL 4
#endif

#define USB_CHIP_SIS315		0x8325  /* Fake */

#define SIS_VBFlagsVersion	1

/* pSiSUSB->VBFlags - if anything is changed here, increase VBFlagsVersion! */
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
#define VB_CONEXANT		0x00000800	/* Definition deprecated (now VBFlags2) */
#define VB_TRUMPION		VB_CONEXANT	/* Definition deprecated (now VBFlags2) */
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
#define OLDVB_301		0x00100000	/* Definition deprecated (now VBFlags2) */
#define OLDVB_301B		0x00200000	/* Definition deprecated (now VBFlags2) */
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
#define TV_STANDARD             (TV_NTSC | TV_PAL | TV_PALM | TV_PALN | TV_NTSCJ)
#define TV_INTERFACE            (TV_AVIDEO|TV_SVIDEO|TV_SCART|TV_HIVISION|TV_YPBPR)

/* Only if TV_YPBPR is set: */
#define TV_YPBPR525I		TV_NTSC
#define TV_YPBPR525P		TV_PAL
#define TV_YPBPR750P		TV_PALM
#define TV_YPBPR1080I	        TV_PALN
#define TV_YPBPRALL 		(TV_YPBPR525I | TV_YPBPR525P | TV_YPBPR750P | TV_YPBPR1080I)

#define TV_YPBPR43LB		TV_CHSCART
#define TV_YPBPR43		TV_CHYPBPR525I
#define TV_YPBPR169 		(TV_CHSCART | TV_CHYPBPR525I)
#define TV_YPBPRAR              (TV_CHSCART | TV_CHYPBPR525I)

#define DISPTYPE_DISP2		CRT2_ENABLE
#define DISPTYPE_DISP1		DISPTYPE_CRT1
#define VB_DISPMODE_SINGLE	SINGLE_MODE  	/* alias */
#define VB_DISPMODE_MIRROR	MIRROR_MODE  	/* alias */
#define VB_DISPMODE_DUAL	DUALVIEW_MODE 	/* alias */
#define DISPLAY_MODE            (SINGLE_MODE | MIRROR_MODE | DUALVIEW_MODE)

/* pSiSUSB->VBFlags2 (static stuff only!) */
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
#define VB2_LCDOVER1280BRIDGE	(VB2_301C  | VB2_307T   | VB2_302LV | VB2_302ELV | VB2_307LV)
#define VB2_LCDOVER1600BRIDGE	(VB2_307T  | VB2_307LV)
#define VB2_RAMDAC202MHZBRIDGE	(VB2_301C  | VB2_307T)

/* pSiSUSB->VBLCDFlags */
#define VB_LCD_320x480		0x00000001	/* DSTN/FSTN for 550 */
#define VB_LCD_640x480          0x00000002
#define VB_LCD_800x600          0x00000004
#define VB_LCD_1024x768         0x00000008
#define VB_LCD_1280x1024        0x00000010
#define VB_LCD_1280x960    	0x00000020
#define VB_LCD_1600x1200	0x00000040
#define VB_LCD_2048x1536	0x00000080
#define VB_LCD_1400x1050        0x00000100
#define VB_LCD_1152x864         0x00000200
#define VB_LCD_1152x768         0x00000400
#define VB_LCD_1280x768         0x00000800
#define VB_LCD_1024x600         0x00001000
#define VB_LCD_640x480_2	0x00002000  	/* DSTN/FSTN */
#define VB_LCD_640x480_3	0x00004000  	/* DSTN/FSTN */
#define VB_LCD_848x480		0x00008000	/* LVDS only, otherwise handled as custom */
#define VB_LCD_1280x800		0x00010000
#define VB_LCD_1680x1050	0x00020000
#define VB_LCD_1280x720         0x00040000
#define VB_LCD_320x240		0x00080000
#define VB_LCD_856x480		0x00100000
#define VB_LCD_1280x854		0x00200000
#define VB_LCD_1920x1200	0x00400000
#define VB_LCD_UNKNOWN		0x10000000
#define VB_LCD_BARCO1366        0x20000000
#define VB_LCD_CUSTOM  		0x40000000
#define VB_LCD_EXPANDING	0x80000000

#define VB_FORBID_CRT2LCD_OVER_1600		/* CRT2/LCD supports only up to 1600 pixels */

/* PresetMode argument */
#define SIS_MODE_SIMU 		0
#define SIS_MODE_CRT1 		1
#define SIS_MODE_CRT2 		2

/* pSiSUSB->MiscFlags */
#define MISC_CRT1OVERLAY	0x00000001  /* Current display mode supports overlay (CRT1) */
#define MISC_PANELLINKSCALER    0x00000002  /* Panel link is currently scaling */
#define MISC_CRT1OVERLAYGAMMA	0x00000004  /* Current display mode supports overlay gamma corr on CRT1 */
#define MISC_TVNTSC1024		0x00000008  /* Current display mode is TV NTSC/PALM/YPBPR525I 1024x768  */
#define MISC_CRT2OVERLAY	0x00000010  /* Current display mode supports overlay (CRT2) */
#define MISC_SIS760ONEOVERLAY	0x00000020  /* SiS760/761: Only one overlay available currently */
#define MISC_STNMODE		0x00000040  /* SiS550: xSTN active */

#ifdef  DEBUG
#define PDEBUG(p)       p
#else
#define PDEBUG(p)
#endif

#define BITMASK(h,l)    	(((unsigned)(1U << ((h) - (l) + 1)) - 1) << (l))
#define GENMASK(mask)   	BITMASK(1 ? mask, 0 ? mask)

#define GETBITS(var,mask)   	(((var) & GENMASK(mask)) >> (0 ? mask))
#define SETBITS(val,mask)   	((val) << (0 ? mask))
#define SETBIT(n)       	(1 << (n))

#define GETBITSTR(val,from,to)       ((GETBITS(val, from)) << (0 ? to))
#define SETVARBITS(var,val,from,to)  (((var) & (~(GENMASK(to)))) | GETBITSTR(val,from,to))
#define GETVAR8(var)            ((var) & 0xFF)
#define SETVAR8(var,val)        (var) =  GETVAR8(val)

typedef unsigned long  ULong;
typedef unsigned short UShort;
typedef unsigned char  UChar;

/* pSiSUSB->VGAEngine - VGA engine types */
#define UNKNOWN_VGA 0
#define SIS_530_VGA 1
#define SIS_OLD_VGA 2
#define SIS_300_VGA 3
#define SIS_315_VGA 4   /* Includes 330/660/661/741/760 and M versions thereof */

/* pSiSUSB->ChipFlags */
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
#define SiSCF_Is315E	    0x00002000  /* 315E */
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
#define SiSCF_760LFB        0x08000000  /* 760: LFB active (if not set, UMA only) */
#define SiSCF_760UMA        0x10000000  /* 760: UMA active (if not set, LFB only) */
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
#define SiS_SD2_SUPPORTXVDEINT 0x00080000   /* Xv deinterlacing supported (n/a) */
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
#define SiS_SD2_HAVESD34       0x40000000   /* Support SD3 and SD4 flags (for future use) */
#define SiS_SD2_NOOVERLAY      0x80000000   /* No video overlay */

#define SiS_SD3_OLDGAMMAINUSE  0x00000001   /* Old gamma brightness is currently in use */

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
#define SiS_CF2_TVYPBPR750P  SiS_CF2_TVPALM
#define SiS_CF2_TVYPBPR1080I (SiS_CF2_TVPALM | SiS_CF2_TVPAL)

/* Defines for our own vgaHW functions */
#define SISVGA_SR_MODE	 0x01
#define SISVGA_SR_FONTS	 0x02
#define SISVGA_SR_CMAP	 0x04
#define SISVGA_SR_ALL	 (SISVGA_SR_MODE | SISVGA_SR_FONTS | SISVGA_SR_CMAP)

/* For backup of register contents */
typedef struct {
    UChar  sisRegMiscOut;
    UChar  sisRegsATTR[22];
    UChar  sisRegsGR[10];
    UChar  sisDAC[768];
    UChar  sisRegs3C4[0x50];
    UChar  sisRegs3D4[0x90];
    UChar  sisRegs3C2;
    UChar  sisCapt[0x60];
    UChar  sisVid[0x50];
    UChar  VBPart1[0x50];
    ULong  sisMMIO85C0;
    UChar  BIOSModeSave;
} SISUSBRegRec, *SISUSBRegPtr;

typedef struct _sisModeInfoPtr {
    int width;
    int height;
    int bpp;
    int n;
    struct _sisModeInfoPtr *next;
} sisModeInfoRec, *sisModeInfoPtr;

/* SISFBLayout is mainly there because of DGA. It holds the
 * current layout parameters needed for acceleration and other
 * stuff. When switching mode using DGA, these are set up
 * accordingly and not necessarily match pScrn's. Therefore,
 * driver modules should read these values instead of pScrn's.
 */
typedef struct {
    int             bitsPerPixel;    /* = pScrn->bitsPerPixel */
    int             depth;	     /* = pScrn->depth */
    int             displayWidth;    /* = pScrn->displayWidth */
    int		    displayHeight;   /* = imageHeight from DGA mode; ONLY WHEN DGA IS ACTIVE!!! */
    int		    DGAViewportX;
    int		    DGAViewportY;
    DisplayModePtr  mode;	     /* = pScrn->currentMode */
} SISFBLayout;

#define SISUSBPTR(p)    ((SISUSBPtr)((p)->driverPrivate))

typedef struct {
    ScrnInfoPtr		pScrn;
    EntityInfoPtr	pEnt;
    int			Chipset;	/* PCI ID (pseudo for USB) */
    int			ChipType;	/* From sisusb_types.h */
    int			ChipRev;

    int			VGAEngine;      /* see above */
    int	                hasTwoOverlays; /* Chipset supports two video overlays? */
    SiS_Private 	*SiS_Pr;        /* For new mode switching code */
    int			DSTN; 		/* For 550 FSTN/DSTN; set by option, no detection */
    ULong       	FbAddress;      /* VRAM physical address (in DHM: for each Fb!) */
    ULong       	realFbAddress;  /* For DHM/PCI mem mapping: store global FBAddress */
    UChar 		*FbBase;        /* VRAM virtual linear address */
    CARD32		IOAddress;      /* MMIO physical address */
    UChar 		*IOBase;        /* MMIO linear address */
    SISIOADDRESS	RelIO;          /* Relocated IO Ports baseaddress */
    int			MemClock;
    int			BusWidth;
    int			MinClock;
    int			MaxClock;
    int			Flags;          /* HW config flags */
    long		FbMapSize;	/* Used for Mem Mapping - DON'T CHANGE THIS */
    long		availMem;       /* Really available Fb mem (minus TQ, HWCursor) */
    ULong		maxxfbmem;      /* limit fb memory X is to use to this (KB) */
    ULong       	sisfbMem;       /* heapstart of sisfb (if running) */
    UChar       	OldMode;        /* Back old modeNo (if available) */
    Bool		NoAccel;
    Bool		NoXvideo;
    Bool		XvOnCRT2;       /* see sis_opt.c */
    Bool		HWCursor;
    int			VESA;
    int			forceCRT1;
    Bool		CRT1changed;
    UChar		oldCR17, oldCR63, oldSR1F;
    UChar		oldCR32, oldCR36, oldCR37;
    UChar		myCR32, myCR36, myCR37, myCR63;
    UChar		newCR32;
    unsigned int	VBFlags;		/* Video bridge configuration */
    unsigned int	VBFlags2;		/* Video bridge configuration 2 (static flags only) */
    unsigned int       	VBFlags_backup;         /* Backup for SlaveMode-modes */
    unsigned int	VBLCDFlags;             /* Moved LCD panel size bits here */
    short		scrnOffset;		/* Screen pitch (data) */
    short		scrnPitch;		/* Screen pitch (display; regarding interlace) */
    unsigned short	DstColor;
    unsigned long	SiS310_AccelDepth; 	/* used in accel for 315 series */
    int			CommandReg;
    int			MaxCMDQueueLen;
    int			CurCMDQueueLen;
    int			MinCMDQueueLen;
    CARD16		CursorSize;  		/* Size of HWCursor area (bytes) */
    CARD32		cursorOffset;		/* see sis_driver.c and sis_cursor.c */
    UChar		*USBCursorBuf;
    SISUSBRegRec	SavedReg;
    SISUSBRegRec	ModeReg;
    xf86CursorInfoPtr	CursorInfoPtr;
    CloseScreenProcPtr	CloseScreen;
    Bool        	(*ModeInit)(ScrnInfoPtr pScrn, DisplayModePtr mode);
    void        	(*SiSSave)(ScrnInfoPtr pScrn, SISUSBRegPtr sisreg);
    void        	(*SiSRestore)(ScrnInfoPtr pScrn, SISUSBRegPtr sisreg);
    int			cmdQueueLen;		/* Current cmdQueueLength (for 2D and 3D) */
    ULong		cmdQueueLenMax;
    ULong		cmdQueueLenMin;
    ULong		*cmdQueueBase;
    int			*cmdQueueLenPtr;	/* Ptr to variable holding the current queue length */
    int			*cmdQueueLenPtrBackup;	/* Backup for DRI init/restore */
    unsigned int	cmdQueueOffset;
    unsigned int	cmdQueueSize;
    ULong       	cmdQueueSizeMask;
    ULong		cmdQ_SharedWritePort_2D;
    ULong		*cmdQ_SharedWritePort;
    ULong		*cmdQ_SharedWritePortBackup;
    unsigned int	cmdQueueSize_div2;
    unsigned int	cmdQueueSize_div4;
    unsigned int	cmdQueueSize_4_3;

    Bool		IsAGPCard;
    ULong		DRIheapstart, DRIheapend;
    Bool		NeedFlush;  /* Need to flush cmd buf mem (760) */

    int			PerColorExpandBufferSize;
    int			ColorExpandBufferNumber;
    int			ColorExpandBufferCountMask;
    UChar		*ColorExpandBufferAddr[32];
    CARD32		ColorExpandBufferScreenOffset[32];
    CARD32		ColorExpandBase;

    /* ShadowFB support */
    Bool		ShadowFB;
    UChar		*ShadowPtr;
#if X_BYTE_ORDER == X_BIG_ENDIAN
    UChar		*ShadowPtrSwap;
#endif
    int			ShadowPitch;
    int			ShXmin, ShXmax, ShYmin, ShYmax, ShBoxcount, delaycount;

    /* sisusb and sisusbfb communication */
    CARD32		USBBus, USBDev;
    Bool		sisusbdevopen;
    int 		sisusbdev, sisusb_minor, sisusbfbactive, sisusbconactive;
    int			sisusberrorsleepcount;
    int 		sisusbfatalerror, timeout;
    Time		errorTime;
    ULong		sisusbmembase, sisusbmmiobase, sisusbioportbase, sisusbpcibase;
    ULong		sisusbvramsize;
    UChar		sisusbversion, sisusbrevision, sisusbpatchlevel, sisusbinit;
    Bool		sisfbfound;
    ULong		sisfbspecialtiming;
    Bool		sisfb_havelock;
    char		sisfbdevname[16];

    /* Xv */
    XF86VideoAdaptorPtr adaptor;
    ScreenBlockHandlerProcPtr BlockHandler;
    void		(*VideoTimerCallback)(ScrnInfoPtr, Time);
    void		(*ResetXv)(ScrnInfoPtr);
    void		(*ResetXvGamma)(ScrnInfoPtr);

    /* misc */
    OptionInfoPtr	Options;
    Bool		Blank;
    UChar		BIOSModeSave;
    int			CRT1off;		/* 1=CRT1 off, 0=CRT1 on */
    CARD16		LCDheight;		/* Vertical resolution of LCD panel */
    CARD16		LCDwidth;		/* Horizontal resolution of LCD panel */
    int			UseVESA;
    CARD16		maxBytesPerScanline;
    CARD32		*pal, *savedPal;
    int			mapPhys, mapOff, mapSize;
    int			statePage, stateSize, stateMode;
    CARD8		*fonts;
    CARD8		*state, *pstate;
    void		*base, *VGAbase;
    SISFBLayout		CurrentLayout;		/* Current framebuffer layout */
    UShort		SiS_DDC2_Index;
    UShort		SiS_DDC2_Data;
    UShort		SiS_DDC2_Clk;
    int			RealVideoRam;		/* 6326 can only address 4MB, but TQ can be above */
    CARD32		CmdQueLenMask;		/* Mask of queue length in MMIO register */
    CARD32		CmdQueLenFix;           /* Fix value to subtract from QueLen (530/620) */
    CARD32		CmdQueMaxLen;           /* (6326/5597/5598) Amount of cmds the queue can hold */
    CARD32		detectedCRT2Devices;	/* detected CRT2 devices before mask-out */
    Bool		noInternalModes;	/* Use our own default modes? */
    Bool		SCLogQuiet;

    int			EMI;
    UChar       	postVBCR32;
    ULong       	lockcalls;		/* Count unlock calls for debug */
    ULong       	sistvccbase;
    Bool		ForceCursorOff;
    Bool		HaveCustomModes;
    Bool		IsCustom;
    DisplayModePtr	backupmodelist;
#ifdef SIS_GLOBAL_ENABLEXV
    Atom		xvBrightness, xvContrast, xvColorKey, xvHue, xvSaturation;
    Atom		xvAutopaintColorKey, xvSetDefaults, xvSwitchCRT;
    Atom		xvDisableGfx, xvDisableGfxLR, xvTVXPosition, xvTVYPosition;
    Atom		xvDisableColorkey, xvUseChromakey, xvChromaMin, xvChromaMax;
    Atom		xvInsideChromakey, xvYUVChromakey, xvVSync;
    Atom		xvGammaRed, xvGammaGreen, xvGammaBlue;
    Atom		xv_QVF, xv_QVV, xv_USD, xv_SVF, xv_QDD, xv_TAF, xv_TSA, xv_TEE, xv_GSF;
    Atom		xv_TTE, xv_TCO, xv_TCC, xv_TCF, xv_TLF, xv_CMD, xv_CMDR, xv_CT1, xv_SGA;
    Atom		xv_GDV, xv_GHI, xv_OVR, xv_GBI, xv_TXS, xv_TYS, xv_CFI, xv_COC, xv_COF;
    Atom		xv_YFI, xv_GSS, xv_BRR, xv_BRG, xv_BRB, xv_PBR, xv_PBG, xv_PBB, xv_SHC;
    Atom		xv_BRR2, xv_BRG2, xv_BRB2, xv_PBR2, xv_PBG2, xv_PBB2, xv_PMD, xv_RDT;
    Atom 		xv_GARC2,xv_GAGC2,xv_GABC2, xv_GSF2;
    Atom		xv_BRRC2, xv_BRGC2, xv_BRBC2, xv_PBRC2, xv_PBGC2, xv_PBBC2;
#ifdef TWDEBUG
    Atom		xv_STR;
#endif
#endif
    int			xv_sisdirectunlocked;
    ULong		xv_sd_result;
    int			CRT1isoff;
    ULong       	ChipFlags;
    ULong       	SiS_SD_Flags, SiS_SD2_Flags, SiS_SD3_Flags, SiS_SD4_Flags;
    Bool		UseHWARGBCursor;
    int			OptUseColorCursor;
    UShort      	cursorBufferNum;
    int			vb;
    Bool		restorebyset;
    Bool		CRT1gamma, CRT1gammaGiven, XvGamma, XvGammaGiven;
    int			XvDefCon, XvDefBri, XvDefHue, XvDefSat;
    Bool		XvDefDisableGfx, XvDefDisableGfxLR, XvDefAdaptorBlit;
    Bool		XvUseChromaKey, XvDisableColorKey;
    Bool		XvInsideChromaKey, XvYUVChromaKey;
    int			XvChromaMin, XvChromaMax;
    int			XvGammaRed, XvGammaGreen, XvGammaBlue;
    int			XvGammaRedDef, XvGammaGreenDef, XvGammaBlueDef;
    CARD8		XvGammaRampRed[256], XvGammaRampGreen[256], XvGammaRampBlue[256];
    Bool		disablecolorkeycurrent;
    CARD32		colorKey;
    CARD32		MiscFlags;
    float		zClearVal;
    ULong		bClrColor, dwColor;
    Bool		enablesisctrl;
    short		Video_MaxWidth, Video_MaxHeight;
    int			FSTN;
    short               scrnPitch2;
    CARD32		CurFGCol, CurBGCol;
    UChar *		CurMonoSrc;
    CARD32 *		CurARGBDest;
    int			GammaBriR, GammaBriG, GammaBriB;
    float		NewGammaBriR, NewGammaBriG, NewGammaBriB;
    float		NewGammaConR, NewGammaConG, NewGammaConB;
    Bool		HideHWCursor;  /* Custom application */
    Bool		HWCursorIsVisible;
    ULong       	HWCursorBackup[16];
    int			HWCursorMBufNum, HWCursorCBufNum;
    ULong		mmioSize;
    Bool		skipswitchcheck;
    ULong		VBFlagsInit;
    DisplayModePtr	currentModeLast;
    IOADDRESS		MyPIOOffset;

    char		messagebuffer[64];
    unsigned int	VGAMapSize;		/* SiSVGA stuff */
    ULong		VGAMapPhys;
    void 		*VGAMemBase; /* mapped */
    Bool		VGAPaletteEnabled;
    Bool		VGACMapSaved;

    ExtensionEntry	*SiSCtrlExtEntry;
    char		devsectname[32];

    /* accel wrapper */
#if 0
    int 		SiSUSBGCIndex;	/* init -1 */
    CloseScreenProcPtr  AWCloseScreen;
    CreateGCProcPtr	AWCreateGC;
    Bool		AccelNeedSync;
    Bool 		IgnoreRefresh;
    int			PreAllocSize;
    void *		PreAllocMem;
#endif
} SISUSBRec, *SISUSBPtr;

extern void  sisusbSaveUnlockExtRegisterLock(SISUSBPtr pSiS, UChar *reg1, UChar *reg2);
extern void  sisusbRestoreExtRegisterLock(SISUSBPtr pSiS, UChar reg1, UChar reg2);
extern void  SiSUSBOptions(ScrnInfoPtr pScrn);
extern const OptionInfoRec * SISUSBAvailableOptions(int chipid, int busid);
extern void  SiSUSBSetup(ScrnInfoPtr pScrn);
extern void  SISUSBVGAPreInit(ScrnInfoPtr pScrn);
extern Bool  SiSUSBHWCursorInit(ScreenPtr pScreen);
extern Bool  SiSUSBAccelInit(ScreenPtr pScreen);
#if 0
extern void  SiSUSBSync(ScrnInfoPtr pScrn);
#endif
#ifdef SIS_GLOBAL_ENABLEXV
extern void  SISUSBInitVideo(ScreenPtr pScreen);
#endif
extern Bool  SiSUSBFBInit(ScreenPtr pScreen);

extern void   SiSUSBMemCopyToVideoRam(SISUSBPtr pSiSUSB, UChar *to, UChar *from, int size);
extern UChar  inSISREG(SISUSBPtr pSiSUSB, ULong base);
extern UShort inSISREGW(SISUSBPtr pSiSUSB, ULong base);
extern ULong  inSISREGL(SISUSBPtr pSiSUSB, ULong base);
extern void   outSISREG(SISUSBPtr pSiSUSB, ULong base, UChar val);
extern void   outSISREGW(SISUSBPtr pSiSUSB, ULong base, UShort val);
extern void   outSISREGL(SISUSBPtr pSiSUSB, ULong base, unsigned int val);
extern void   orSISREG(SISUSBPtr pSiSUSB, ULong base, UChar val);
extern void   andSISREG(SISUSBPtr pSiSUSB, ULong base, UChar val);
extern void   outSISIDXREG(SISUSBPtr pSiSUSB, ULong base, UChar idx, UChar val);
extern UChar  __inSISIDXREG(SISUSBPtr pSiSUSB, ULong base, UChar idx);
extern void   orSISIDXREG(SISUSBPtr pSiSUSB, ULong base, UChar idx, UChar val);
extern void   andSISIDXREG(SISUSBPtr pSiSUSB, ULong base, UChar idx, UChar val);
extern void   setSISIDXREG(SISUSBPtr pSiSUSB, ULong base, UChar idx, UChar myand, UChar myor);
extern void   setSISIDXREGmask(SISUSBPtr pSiSUSB, ULong base, UChar idx, UChar data, UChar mask);
extern void   sisclearvram(SISUSBPtr pSiSUSB, UChar *where, unsigned int howmuch);
extern void   sisrestoredestroyconsole(SISUSBPtr pSiSUSB, int what);
extern void   SIS_MMIO_OUT8(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset, CARD8 val);
extern void   SIS_MMIO_OUT16(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset, CARD16 val);
extern void   SIS_MMIO_OUT32(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset, CARD32 val);
extern CARD8  SIS_MMIO_IN8(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset);
extern CARD16 SIS_MMIO_IN16(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset);
extern CARD32 SIS_MMIO_IN32(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset);

#define inSISIDXREG(struct,base,idx,var)  var = __inSISIDXREG(struct, base, idx)

#define SIS_USEIOCTL

#endif  /* _SISUSB_H_ */



