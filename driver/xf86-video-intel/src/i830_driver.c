/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/i810/i830_driver.c,v 1.50 2004/02/20 00:06:00 alanh Exp $ */
/**************************************************************************

Copyright 2001 VA Linux Systems Inc., Fremont, California.
Copyright © 2002 by David Dawes

All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
on the rights to use, copy, modify, merge, publish, distribute, sub
license, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice (including the next
paragraph) shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
THE COPYRIGHT HOLDERS AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

/*
 * Reformatted with GNU indent (2.2.8), using the following options:
 *
 *    -bad -bap -c41 -cd0 -ncdb -ci6 -cli0 -cp0 -ncs -d0 -di3 -i3 -ip3 -l78
 *    -lp -npcs -psl -sob -ss -br -ce -sc -hnl
 *
 * This provides a good match with the original i810 code and preferred
 * XFree86 formatting conventions.
 *
 * When editing this driver, please follow the existing formatting, and edit
 * with <TAB> characters expanded at 8-column intervals.
 */

/*
 * Authors: Jeff Hartmann <jhartmann@valinux.com>
 *          Abraham van der Merwe <abraham@2d3d.co.za>
 *          David Dawes <dawes@xfree86.org>
 *          Alan Hourihane <alanh@tungstengraphics.com>
 */

/*
 * Mode handling is based on the VESA driver written by:
 * Paulo César Pereira de Andrade <pcpa@conectiva.com.br>
 */

/*
 * Changes:
 *
 *    23/08/2001 Abraham van der Merwe <abraham@2d3d.co.za>
 *        - Fixed display timing bug (mode information for some
 *          modes were not initialized correctly)
 *        - Added workarounds for GTT corruptions (I don't adjust
 *          the pitches for 1280x and 1600x modes so we don't
 *          need extra memory)
 *        - The code will now default to 60Hz if LFP is connected
 *        - Added different refresh rate setting code to work
 *          around 0x4f02 BIOS bug
 *        - BIOS workaround for some mode sets (I use legacy BIOS
 *          calls for setting those)
 *        - Removed 0x4f04, 0x01 (save state) BIOS call which causes
 *          LFP to malfunction (do some house keeping and restore
 *          modes ourselves instead - not perfect, but at least the
 *          LFP is working now)
 *        - Several other smaller bug fixes
 *
 *    06/09/2001 Abraham van der Merwe <abraham@2d3d.co.za>
 *        - Preliminary local memory support (works without agpgart)
 *        - DGA fixes (the code were still using i810 mode sets, etc.)
 *        - agpgart fixes
 *
 *    18/09/2001
 *        - Proper local memory support (should work correctly now
 *          with/without agpgart module)
 *        - more agpgart fixes
 *        - got rid of incorrect GTT adjustments
 *
 *    09/10/2001
 *        - Changed the DPRINTF() variadic macro to an ANSI C compatible
 *          version
 *
 *    10/10/2001
 *        - Fixed DPRINTF_stub(). I forgot the __...__ macros in there
 *          instead of the function arguments :P
 *        - Added a workaround for the 1600x1200 bug (Text mode corrupts
 *          when you exit from any 1600x1200 mode and 1280x1024@85Hz. I
 *          suspect this is a BIOS bug (hence the 1280x1024@85Hz case)).
 *          For now I'm switching to 800x600@60Hz then to 80x25 text mode
 *          and then restoring the registers - very ugly indeed.
 *
 *    15/10/2001
 *        - Improved 1600x1200 mode set workaround. The previous workaround
 *          was causing mode set problems later on.
 *
 *    18/10/2001
 *        - Fixed a bug in I830BIOSLeaveVT() which caused a bug when you
 *          switched VT's
 */
/*
 *    07/2002 David Dawes
 *        - Add Intel(R) 855GM/852GM support.
 */
/*
 *    07/2002 David Dawes
 *        - Cleanup code formatting.
 *        - Improve VESA mode selection, and fix refresh rate selection.
 *        - Don't duplicate functions provided in 4.2 vbe modules.
 *        - Don't duplicate functions provided in the vgahw module.
 *        - Rewrite memory allocation.
 *        - Rewrite initialisation and save/restore state handling.
 *        - Decouple the i810 support from i830 and later.
 *        - Remove various unnecessary hacks and workarounds.
 *        - Fix an 845G problem with the ring buffer not in pre-allocated
 *          memory.
 *        - Fix screen blanking.
 *        - Clear the screen at startup so you don't see the previous session.
 *        - Fix some HW cursor glitches, and turn HW cursor off at VT switch
 *          and exit.
 *
 *    08/2002 Keith Whitwell
 *        - Fix DRI initialisation.
 *
 *
 *    08/2002 Alan Hourihane and David Dawes
 *        - Add XVideo support.
 *
 *
 *    10/2002 David Dawes
 *        - Add Intel(R) 865G support.
 *
 *
 *    01/2004 Alan Hourihane
 *        - Add Intel(R) 915G support.
 *        - Add Dual Head and Clone capabilities.
 *        - Add lid status checking
 *        - Fix Xvideo with high-res LFP's
 *        - Add ARGB HW cursor support
 *
 *    05/2005 Alan Hourihane
 *        - Add Intel(R) 945G support.
 *
 *    09/2005 Alan Hourihane
 *        - Add Intel(R) 945GM support.
 *
 *    10/2005 Alan Hourihane, Keith Whitwell, Brian Paul
 *        - Added Rotation support
 *
 *    12/2005 Alan Hourihane, Keith Whitwell
 *        - Add Intel(R) 965G support.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef PRINT_MODE_INFO
#define PRINT_MODE_INFO 0
#endif

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Resources.h"
#include "xf86RAC.h"
#include "xf86cmap.h"
#include "compiler.h"
#include "mibstore.h"
#include "vgaHW.h"
#include "mipointer.h"
#include "micmap.h"
#include "shadowfb.h"
#include <X11/extensions/randr.h>
#include "fb.h"
#include "miscstruct.h"
#include "dixstruct.h"
#include "xf86xv.h"
#include <X11/extensions/Xv.h>
#include "vbe.h"
#include "shadow.h"
#include "i830.h"
#include "i830_display.h"
#include "i830_debug.h"
#include "i830_bios.h"
#include "i830_video.h"
#if HAVE_SYS_MMAN_H && HAVE_MPROTECT
#include <sys/mman.h>
#endif

#ifdef INTEL_XVMC
#define _INTEL_XVMC_SERVER_
#include "i830_hwmc.h"
#endif

#ifdef XF86DRI
#include "dri.h"
#include <sys/ioctl.h>
#ifdef XF86DRI_MM
#include "xf86mm.h"
#endif
#endif

#ifdef I830_USE_EXA
const char *I830exaSymbols[] = {
    "exaGetVersion",
    "exaDriverInit",
    "exaDriverFini",
    "exaOffscreenAlloc",
    "exaOffscreenFree",
    "exaWaitSync",
    NULL
};
#endif

#define BIT(x) (1 << (x))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define NB_OF(x) (sizeof (x) / sizeof (*x))

/* *INDENT-OFF* */
static SymTabRec I830Chipsets[] = {
   {PCI_CHIP_I830_M,		"i830"},
   {PCI_CHIP_845_G,		"845G"},
   {PCI_CHIP_I855_GM,		"852GM/855GM"},
   {PCI_CHIP_I865_G,		"865G"},
   {PCI_CHIP_I915_G,		"915G"},
   {PCI_CHIP_E7221_G,		"E7221 (i915)"},
   {PCI_CHIP_I915_GM,		"915GM"},
   {PCI_CHIP_I945_G,		"945G"},
   {PCI_CHIP_I945_GM,		"945GM"},
   {PCI_CHIP_I945_GME,		"945GME"},
   {PCI_CHIP_I965_G,		"965G"},
   {PCI_CHIP_G35_G,		"G35"},
   {PCI_CHIP_I965_Q,		"965Q"},
   {PCI_CHIP_I946_GZ,		"946GZ"},
   {PCI_CHIP_I965_GM,		"965GM"},
   {PCI_CHIP_I965_GME,		"965GME/GLE"},
   {PCI_CHIP_G33_G,		"G33"},
   {PCI_CHIP_Q35_G,		"Q35"},
   {PCI_CHIP_Q33_G,		"Q33"},
   {PCI_CHIP_GM45_GM,		"Mobile Intel® GM45 Express Chipset"},
   {PCI_CHIP_IGD_E_G,		"Intel Integrated Graphics Device"},
   {PCI_CHIP_G45_G,		"G45/G43"},
   {PCI_CHIP_Q45_G,		"Q45/Q43"},
   {-1,				NULL}
};

static PciChipsets I830PciChipsets[] = {
   {PCI_CHIP_I830_M,		PCI_CHIP_I830_M,	RES_SHARED_VGA},
   {PCI_CHIP_845_G,		PCI_CHIP_845_G,		RES_SHARED_VGA},
   {PCI_CHIP_I855_GM,		PCI_CHIP_I855_GM,	RES_SHARED_VGA},
   {PCI_CHIP_I865_G,		PCI_CHIP_I865_G,	RES_SHARED_VGA},
   {PCI_CHIP_I915_G,		PCI_CHIP_I915_G,	RES_SHARED_VGA},
   {PCI_CHIP_E7221_G,		PCI_CHIP_E7221_G,	RES_SHARED_VGA},
   {PCI_CHIP_I915_GM,		PCI_CHIP_I915_GM,	RES_SHARED_VGA},
   {PCI_CHIP_I945_G,		PCI_CHIP_I945_G,	RES_SHARED_VGA},
   {PCI_CHIP_I945_GM,		PCI_CHIP_I945_GM,	RES_SHARED_VGA},
   {PCI_CHIP_I945_GME,		PCI_CHIP_I945_GME,	RES_SHARED_VGA},
   {PCI_CHIP_I965_G,		PCI_CHIP_I965_G,	RES_SHARED_VGA},
   {PCI_CHIP_G35_G,		PCI_CHIP_G35_G,		RES_SHARED_VGA},
   {PCI_CHIP_I965_Q,		PCI_CHIP_I965_Q,	RES_SHARED_VGA},
   {PCI_CHIP_I946_GZ,		PCI_CHIP_I946_GZ,	RES_SHARED_VGA},
   {PCI_CHIP_I965_GM,		PCI_CHIP_I965_GM,	RES_SHARED_VGA},
   {PCI_CHIP_I965_GME,		PCI_CHIP_I965_GME,	RES_SHARED_VGA},
   {PCI_CHIP_G33_G,		PCI_CHIP_G33_G,		RES_SHARED_VGA},
   {PCI_CHIP_Q35_G,		PCI_CHIP_Q35_G,		RES_SHARED_VGA},
   {PCI_CHIP_Q33_G,		PCI_CHIP_Q33_G,		RES_SHARED_VGA},
   {PCI_CHIP_GM45_GM,		PCI_CHIP_GM45_GM,	RES_SHARED_VGA},
   {PCI_CHIP_IGD_E_G,		PCI_CHIP_IGD_E_G,	RES_SHARED_VGA},
   {PCI_CHIP_G45_G,		PCI_CHIP_G45_G,		RES_SHARED_VGA},
   {PCI_CHIP_Q45_G,		PCI_CHIP_Q45_G,		RES_SHARED_VGA},
   {-1,				-1,			RES_UNDEFINED}
};

/*
 * Note: "ColorKey" is provided for compatibility with the i810 driver.
 * However, the correct option name is "VideoKey".  "ColorKey" usually
 * refers to the tranparency key for 8+24 overlays, not for video overlays.
 */

typedef enum {
#if defined(I830_USE_XAA) && defined(I830_USE_EXA)
   OPTION_ACCELMETHOD,
#endif
   OPTION_NOACCEL,
   OPTION_SW_CURSOR,
   OPTION_CACHE_LINES,
   OPTION_DRI,
   OPTION_PAGEFLIP,
   OPTION_XVIDEO,
   OPTION_VIDEO_KEY,
   OPTION_COLOR_KEY,
   OPTION_CHECKDEVICES,
   OPTION_MODEDEBUG,
   OPTION_LVDS24BITMODE,
   OPTION_FBC,
   OPTION_TILING,
#ifdef XF86DRI_MM
   OPTION_INTELTEXPOOL,
#endif
   OPTION_LVDSFIXEDMODE,
   OPTION_TRIPLEBUFFER,
   OPTION_FORCEENABLEPIPEA,
#ifdef INTEL_XVMC
   OPTION_XVMC,
#endif
} I830Opts;

static OptionInfoRec I830Options[] = {
#if defined(I830_USE_XAA) && defined(I830_USE_EXA)
   {OPTION_ACCELMETHOD,	"AccelMethod",	OPTV_ANYSTR,	{0},	FALSE},
#endif
   {OPTION_NOACCEL,	"NoAccel",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_SW_CURSOR,	"SWcursor",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_CACHE_LINES,	"CacheLines",	OPTV_INTEGER,	{0},	FALSE},
   {OPTION_DRI,		"DRI",		OPTV_BOOLEAN,	{0},	TRUE},
   {OPTION_PAGEFLIP,	"PageFlip",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_XVIDEO,	"XVideo",	OPTV_BOOLEAN,	{0},	TRUE},
   {OPTION_COLOR_KEY,	"ColorKey",	OPTV_INTEGER,	{0},	FALSE},
   {OPTION_VIDEO_KEY,	"VideoKey",	OPTV_INTEGER,	{0},	FALSE},
   {OPTION_CHECKDEVICES, "CheckDevices",OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_MODEDEBUG,	"ModeDebug",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_LVDS24BITMODE, "LVDS24Bit",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_FBC,		"FramebufferCompression", OPTV_BOOLEAN, {0}, TRUE},
   {OPTION_TILING,	"Tiling",	OPTV_BOOLEAN,	{0},	TRUE},
#ifdef XF86DRI_MM
   {OPTION_INTELTEXPOOL,"Legacy3D",     OPTV_BOOLEAN,	{0},	FALSE},
#endif
   {OPTION_LVDSFIXEDMODE, "LVDSFixedMode", OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_TRIPLEBUFFER, "TripleBuffer", OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_FORCEENABLEPIPEA, "ForceEnablePipeA", OPTV_BOOLEAN,	{0},	FALSE},
#ifdef INTEL_XVMC
   {OPTION_XVMC,	"XvMC",		OPTV_BOOLEAN,	{0},	TRUE},
#endif
   {-1,			NULL,		OPTV_NONE,	{0},	FALSE}
};
/* *INDENT-ON* */

const char *i830_output_type_names[] = {
   "Unused",
   "Analog",
   "DVO",
   "SDVO",
   "LVDS",
   "TVOUT",
};

static void i830AdjustFrame(int scrnIndex, int x, int y, int flags);
static Bool I830CloseScreen(int scrnIndex, ScreenPtr pScreen);
static Bool I830EnterVT(int scrnIndex, int flags);
static CARD32 I830CheckDevicesTimer(OsTimerPtr timer, CARD32 now, pointer arg);
static Bool SaveHWState(ScrnInfoPtr pScrn);
static Bool RestoreHWState(ScrnInfoPtr pScrn);

/* temporary */
extern void xf86SetCursor(ScreenPtr pScreen, CursorPtr pCurs, int x, int y);

#ifdef I830DEBUG
void
I830DPRINTF_stub(const char *filename, int line, const char *function,
		 const char *fmt, ...)
{
   va_list ap;

   ErrorF("\n##############################################\n"
	  "*** In function %s, on line %d, in file %s ***\n",
	  function, line, filename);
   va_start(ap, fmt);
   VErrorF(fmt, ap);
   va_end(ap);
   ErrorF("##############################################\n\n");
}
#else /* #ifdef I830DEBUG */
void
I830DPRINTF_stub(const char *filename, int line, const char *function,
		 const char *fmt, ...)
{
   /* do nothing */
}
#endif /* #ifdef I830DEBUG */

/* Export I830 options to i830 driver where necessary */
const OptionInfoRec *
I830AvailableOptions(int chipid, int busid)
{
   int i;

   for (i = 0; I830PciChipsets[i].PCIid > 0; i++) {
      if (chipid == I830PciChipsets[i].PCIid)
	 return I830Options;
   }
   return NULL;
}

static Bool
I830GetRec(ScrnInfoPtr pScrn)
{
   I830Ptr pI830;

   if (pScrn->driverPrivate)
      return TRUE;
   pI830 = pScrn->driverPrivate = xnfcalloc(sizeof(I830Rec), 1);
   return TRUE;
}

static void
I830FreeRec(ScrnInfoPtr pScrn)
{
   I830Ptr pI830;

   if (!pScrn)
      return;
   if (!pScrn->driverPrivate)
      return;

   pI830 = I830PTR(pScrn);

   xfree(pScrn->driverPrivate);
   pScrn->driverPrivate = NULL;
}

static void
I830ProbeDDC(ScrnInfoPtr pScrn, int index)
{
   vbeInfoPtr pVbe;

   /* The vbe module gets loaded in PreInit(), so no need to load it here. */

   pVbe = VBEInit(NULL, index);
   ConfiguredMonitor = vbeDoEDID(pVbe, NULL);
}

static int
I830DetectMemory(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
#if !XSERVER_LIBPCIACCESS
   PCITAG bridge;
#endif
   uint16_t gmch_ctrl;
   int memsize = 0, gtt_size;
   int range;
#if 0
   VbeInfoBlock *vbeInfo;
#endif

#if XSERVER_LIBPCIACCESS
   struct pci_device *bridge = intel_host_bridge ();
   pci_device_cfg_read_u16(bridge, & gmch_ctrl, I830_GMCH_CTRL);
#else
   bridge = pciTag(0, 0, 0);		/* This is always the host bridge */
   gmch_ctrl = pciReadWord(bridge, I830_GMCH_CTRL);
#endif

   if (IS_I965G(pI830)) {
      /* The 965 may have a GTT that is actually larger than is necessary
       * to cover the aperture, so check the hardware's reporting of the
       * GTT size.
       */
      switch (INREG(PGETBL_CTL) & PGETBL_SIZE_MASK) {
      case PGETBL_SIZE_512KB:
	 gtt_size = 512;
	 break;
      case PGETBL_SIZE_256KB:
	 gtt_size = 256;
	 break;
      case PGETBL_SIZE_128KB:
	 gtt_size = 128;
	 break;
      case PGETBL_SIZE_1MB:
	 gtt_size = 1024;
	 break;
      case PGETBL_SIZE_2MB:
	 gtt_size = 2048;
	 break;
      case PGETBL_SIZE_1_5MB:
	 gtt_size = 1024 + 512;
	 break;
      default:
	 FatalError("Unknown GTT size value: %08x\n", (int)INREG(PGETBL_CTL));
      }
   } else if (IS_G33CLASS(pI830)) {
      /* G33's GTT size is detect in GMCH_CTRL */
      switch (gmch_ctrl & G33_PGETBL_SIZE_MASK) {
      case G33_PGETBL_SIZE_1M:
	 gtt_size = 1024;
	 break;
      case G33_PGETBL_SIZE_2M:
	 gtt_size = 2048;
	 break;
      default:
	 FatalError("Unknown GTT size value: %08x\n",
		    (int)(gmch_ctrl & G33_PGETBL_SIZE_MASK));
      }
   } else {
      /* Older chipsets only had GTT appropriately sized for the aperture. */
      gtt_size = pI830->FbMapSize / (1024*1024);
   }

   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "detected %d kB GTT.\n", gtt_size);

   /* The stolen memory has the GTT at the top, and the 4KB popup below that.
    * Everything else can be freely used by the graphics driver.
    */
   range = gtt_size + 4;

   /* new 4 series hardware has seperate GTT stolen with GFX stolen */
   if (IS_G4X(pI830) || IS_GM45(pI830))
       range = 4;

   if (IS_I85X(pI830) || IS_I865G(pI830) || IS_I9XX(pI830)) {
      switch (gmch_ctrl & I855_GMCH_GMS_MASK) {
      case I855_GMCH_GMS_STOLEN_1M:
	 memsize = MB(1) - KB(range);
	 break;
      case I855_GMCH_GMS_STOLEN_4M:
	 memsize = MB(4) - KB(range);
	 break;
      case I855_GMCH_GMS_STOLEN_8M:
	 memsize = MB(8) - KB(range);
	 break;
      case I855_GMCH_GMS_STOLEN_16M:
	 memsize = MB(16) - KB(range);
	 break;
      case I855_GMCH_GMS_STOLEN_32M:
	 memsize = MB(32) - KB(range);
	 break;
      case I915G_GMCH_GMS_STOLEN_48M:
	 if (IS_I9XX(pI830))
	    memsize = MB(48) - KB(range);
	 break;
      case I915G_GMCH_GMS_STOLEN_64M:
	 if (IS_I9XX(pI830))
	    memsize = MB(64) - KB(range);
	 break;
      case G33_GMCH_GMS_STOLEN_128M:
	 if (IS_I9XX(pI830))
	     memsize = MB(128) - KB(range);
	 break;
      case G33_GMCH_GMS_STOLEN_256M:
	 if (IS_I9XX(pI830))
	     memsize = MB(256) - KB(range);
	 break;
      case INTEL_GMCH_GMS_STOLEN_96M:
	 if (IS_I9XX(pI830))
	     memsize = MB(96) - KB(range);
	 break;
      case INTEL_GMCH_GMS_STOLEN_160M:
	 if (IS_I9XX(pI830))
	     memsize = MB(160) - KB(range);
	 break;
      case INTEL_GMCH_GMS_STOLEN_224M:
	 if (IS_I9XX(pI830))
	     memsize = MB(224) - KB(range);
	 break;
      case INTEL_GMCH_GMS_STOLEN_352M:
	 if (IS_I9XX(pI830))
	     memsize = MB(352) - KB(range);
	 break;
      }
   } else {
      switch (gmch_ctrl & I830_GMCH_GMS_MASK) {
      case I830_GMCH_GMS_STOLEN_512:
	 memsize = KB(512) - KB(range);
	 break;
      case I830_GMCH_GMS_STOLEN_1024:
	 memsize = MB(1) - KB(range);
	 break;
      case I830_GMCH_GMS_STOLEN_8192:
	 memsize = MB(8) - KB(range);
	 break;
      case I830_GMCH_GMS_LOCAL:
	 memsize = 0;
	 xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		    "Local memory found, but won't be used.\n");
	 break;
      }
   }

#if 0
   /* And 64KB page aligned */
   memsize &= ~0xFFFF;
#endif

   if (memsize > 0) {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		 "detected %d kB stolen memory.\n", memsize / 1024);
   } else {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "no video memory detected.\n");
   }

   return memsize;
}

static Bool
I830MapMMIO(ScrnInfoPtr pScrn)
{
#if XSERVER_LIBPCIACCESS
   int err;
   struct pci_device *device;
#else
   int mmioFlags;
#endif
   I830Ptr pI830 = I830PTR(pScrn);

#if XSERVER_LIBPCIACCESS
   device = pI830->PciInfo;
   err = pci_device_map_range (device,
			       pI830->MMIOAddr,
			       I810_REG_SIZE,
			       PCI_DEV_MAP_FLAG_WRITABLE,
			       (void **) &pI830->MMIOBase);
   if (err) 
   {
      xf86DrvMsg (pScrn->scrnIndex, X_ERROR,
		  "Unable to map mmio range. %s (%d)\n",
		  strerror (err), err);
      return FALSE;
   }
#else

#if !defined(__alpha__)
   mmioFlags = VIDMEM_MMIO | VIDMEM_READSIDEEFFECT;
#else
   mmioFlags = VIDMEM_MMIO | VIDMEM_READSIDEEFFECT | VIDMEM_SPARSE;
#endif

   pI830->MMIOBase = xf86MapPciMem(pScrn->scrnIndex, mmioFlags,
				   pI830->PciTag,
				   pI830->MMIOAddr, I810_REG_SIZE);
   if (!pI830->MMIOBase)
      return FALSE;
#endif

   /* Set up the GTT mapping for the various places it has been moved over
    * time.
    */
   if (IS_I9XX(pI830)) {
      uint32_t gttaddr;

      if (IS_I965G(pI830)) 
      {
	 if (IS_GM45(pI830) || IS_G4X(pI830)) {
	     gttaddr = pI830->MMIOAddr + MB(2);
	     pI830->GTTMapSize = MB(2);
	 } else {
	     gttaddr = pI830->MMIOAddr + KB(512);
	     pI830->GTTMapSize = KB(512);
	 }
      }
      else
      {
	 gttaddr = I810_MEMBASE(pI830->PciInfo, 3) & 0xFFFFFF00;
	 pI830->GTTMapSize = pI830->FbMapSize / 1024;
      }
#if XSERVER_LIBPCIACCESS
      err = pci_device_map_range (device,
				  gttaddr, pI830->GTTMapSize,
				  PCI_DEV_MAP_FLAG_WRITABLE,
				  (void **) &pI830->GTTBase);
      if (err)
      {
	 xf86DrvMsg (pScrn->scrnIndex, X_ERROR,
		     "Unable to map GTT range. %s (%d)\n",
		     strerror (err), err);
	 return FALSE;
      }
#else
      pI830->GTTBase = xf86MapPciMem(pScrn->scrnIndex, mmioFlags,
				     pI830->PciTag,
				     gttaddr, pI830->GTTMapSize);
      if (pI830->GTTBase == NULL)
	 return FALSE;
#endif
   } else {
      /* The GTT aperture on i830 is write-only.  We could probably map the
       * actual physical pages that back it, but leave it alone for now.
       */
      pI830->GTTBase = NULL;
      pI830->GTTMapSize = 0;
   }

   return TRUE;
}

static Bool
I830MapMem(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   long i;
#if XSERVER_LIBPCIACCESS
   struct pci_device *const device = pI830->PciInfo;
   int err;
#endif

   for (i = 2; i < pI830->FbMapSize; i <<= 1) ;
   pI830->FbMapSize = i;

   if (!I830MapMMIO(pScrn))
      return FALSE;

#if XSERVER_LIBPCIACCESS
   err = pci_device_map_range (device, pI830->LinearAddr, pI830->FbMapSize,
			       PCI_DEV_MAP_FLAG_WRITABLE | PCI_DEV_MAP_FLAG_WRITE_COMBINE,
			       (void **) &pI830->FbBase);
    if (err)
	return FALSE;
#else
   pI830->FbBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_FRAMEBUFFER,
				 pI830->PciTag,
				 pI830->LinearAddr, pI830->FbMapSize);
   if (!pI830->FbBase)
      return FALSE;
#endif

   if (I830IsPrimary(pScrn) && pI830->LpRing->mem != NULL) {
      pI830->LpRing->virtual_start =
	 pI830->FbBase + pI830->LpRing->mem->offset;
   }

   return TRUE;
}

static void
I830UnmapMMIO(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

#if XSERVER_LIBPCIACCESS
   pci_device_unmap_range (pI830->PciInfo, pI830->MMIOBase, I810_REG_SIZE);
#else
   xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pI830->MMIOBase,
		   I810_REG_SIZE);
#endif
   pI830->MMIOBase = NULL;

   if (IS_I9XX(pI830)) {
#if XSERVER_LIBPCIACCESS
      pci_device_unmap_range (pI830->PciInfo, pI830->GTTBase, pI830->GTTMapSize);
#else
      xf86UnMapVidMem(pScrn->scrnIndex, pI830->GTTBase, pI830->GTTMapSize);
#endif
      pI830->GTTBase = NULL;
   }
}

static Bool
I830UnmapMem(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

#if XSERVER_LIBPCIACCESS
   pci_device_unmap_range (pI830->PciInfo, pI830->FbBase, pI830->FbMapSize);
#else
   xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pI830->FbBase,
		   pI830->FbMapSize);
#endif
   pI830->FbBase = NULL;
   I830UnmapMMIO(pScrn);
   return TRUE;
}

static void
I830LoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices,
		LOCO * colors, VisualPtr pVisual)
{
   xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
   int i,j, index;
   int p;
   uint16_t lut_r[256], lut_g[256], lut_b[256];

   DPRINTF(PFX, "I830LoadPalette: numColors: %d\n", numColors);

   for(p = 0; p < xf86_config->num_crtc; p++) {
      xf86CrtcPtr	   crtc = xf86_config->crtc[p];
      I830CrtcPrivatePtr   intel_crtc = crtc->driver_private;

      /* Initialize to the old lookup table values. */
      for (i = 0; i < 256; i++) {
	 lut_r[i] = intel_crtc->lut_r[i] << 8;
	 lut_g[i] = intel_crtc->lut_g[i] << 8;
	 lut_b[i] = intel_crtc->lut_b[i] << 8;
      }

      switch(pScrn->depth) {
      case 15:
	 for (i = 0; i < numColors; i++) {
	    index = indices[i];
	    for (j = 0; j < 8; j++) {
	       lut_r[index * 8 + j] = colors[index].red << 8;
	       lut_g[index * 8 + j] = colors[index].green << 8;
	       lut_b[index * 8 + j] = colors[index].blue << 8;
	    }
         }
	 break;
      case 16:
	 for (i = 0; i < numColors; i++) {
	    index = indices[i];

	    if (index <= 31) {
	       for (j = 0; j < 8; j++) {
		  lut_r[index * 8 + j] = colors[index].red << 8;
		  lut_b[index * 8 + j] = colors[index].blue << 8;
	       }
	    }

	    for (j = 0; j < 4; j++) {
	       lut_g[index * 4 + j] = colors[index].green << 8;
	    }
         }
        break;
      default:
	 for (i = 0; i < numColors; i++) {
	    index = indices[i];
	    lut_r[index] = colors[index].red << 8;
	    lut_g[index] = colors[index].green << 8;
	    lut_b[index] = colors[index].blue << 8;
	 }
	 break;
      }

      /* Make the change through RandR */
#ifdef RANDR_12_INTERFACE
      RRCrtcGammaSet(crtc->randr_crtc, lut_r, lut_g, lut_b);
#else
      crtc->funcs->gamma_set(crtc, lut_r, lut_g, lut_b, 256);
#endif
   }
}

static void
i830_update_front_offset(ScrnInfoPtr pScrn)
{
   ScreenPtr pScreen = pScrn->pScreen;
   I830Ptr pI830 = I830PTR(pScrn);

   /* Update buffer locations, which may have changed as a result of
    * i830_bind_all_memory().
    */
   pScrn->fbOffset = pI830->front_buffer->offset;

   /* If we are still in ScreenInit, there is no screen pixmap to be updated
    * yet.  We'll fix it up at CreateScreenResources.
    */
   if (!pI830->starting) {
      if (!pScreen->ModifyPixmapHeader(pScreen->GetScreenPixmap(pScreen),
				       -1, -1, -1, -1, -1,
				       (pointer)(pI830->FbBase +
						 pScrn->fbOffset)))
       FatalError("Couldn't adjust screen pixmap\n");
   }
}

/**
 * Adjust the screen pixmap for the current location of the front buffer.
 * This is done at EnterVT when buffers are bound as long as the resources
 * have already been created, but the first EnterVT happens before
 * CreateScreenResources.
 */
static Bool
i830CreateScreenResources(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);

   pScreen->CreateScreenResources = pI830->CreateScreenResources;
   if (!(*pScreen->CreateScreenResources)(pScreen))
      return FALSE;

   i830_update_front_offset(pScrn);

   return TRUE;
}

int
i830_output_clones (ScrnInfoPtr pScrn, int type_mask)
{
    xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR (pScrn);
    int			o;
    int			index_mask = 0;

    for (o = 0; o < config->num_output; o++)
    {
	xf86OutputPtr		output = config->output[o];
	I830OutputPrivatePtr	intel_output = output->driver_private;
	if (type_mask & (1 << intel_output->type))
	    index_mask |= (1 << o);
    }
    return index_mask;
}

/**
 * Set up the outputs according to what type of chip we are.
 *
 * Some outputs may not initialize, due to allocation failure or because a
 * controller chip isn't found.
 */
static void
I830SetupOutputs(ScrnInfoPtr pScrn)
{
   xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR (pScrn);
   I830Ptr  pI830 = I830PTR(pScrn);
   int	    o, c;
   Bool	    lvds_detected = FALSE;

   /* everyone has at least a single analog output */
   i830_crt_init(pScrn);

   /* Set up integrated LVDS */
   if (IS_MOBILE(pI830) && !IS_I830(pI830))
      i830_lvds_init(pScrn);

   if (IS_I9XX(pI830)) {
      if (INREG(SDVOB) & SDVO_DETECTED) {
	 Bool found = i830_sdvo_init(pScrn, SDVOB);

	 if (!found && SUPPORTS_INTEGRATED_HDMI(pI830))
	    i830_hdmi_init(pScrn, SDVOB);
      }

      if (INREG(SDVOC) & SDVO_DETECTED) {
	 Bool found = i830_sdvo_init(pScrn, SDVOC);

	 if (!found && SUPPORTS_INTEGRATED_HDMI(pI830))
	    i830_hdmi_init(pScrn, SDVOC);
      }
   } else {
      i830_dvo_init(pScrn);
   }
   if (IS_I9XX(pI830) && IS_MOBILE(pI830))
      i830_tv_init(pScrn);
   
   for (o = 0; o < config->num_output; o++)
   {
      xf86OutputPtr	   output = config->output[o];
      I830OutputPrivatePtr intel_output = output->driver_private;
      int		   crtc_mask;

      if (intel_output->type == I830_OUTPUT_LVDS)
	  lvds_detected = TRUE;
      
      crtc_mask = 0;
      for (c = 0; c < config->num_crtc; c++)
      {
	 xf86CrtcPtr	      crtc = config->crtc[c];
	 I830CrtcPrivatePtr   intel_crtc = crtc->driver_private;

	 if (intel_output->pipe_mask & (1 << intel_crtc->pipe))
	    crtc_mask |= (1 << c);
      }
      output->possible_crtcs = crtc_mask;
      output->possible_clones = i830_output_clones (pScrn, intel_output->clone_mask);
   }
}

static void
i830_init_clock_gating(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);

    /* Disable clock gating reported to work incorrectly according to the specs.
     */
    if (IS_GM45(pI830) || IS_G4X(pI830)) {
	OUTREG(RENCLK_GATE_D1, 0);
	OUTREG(RENCLK_GATE_D2, 0);
	OUTREG(RAMCLK_GATE_D, 0);
	OUTREG(DSPCLK_GATE_D, VRHUNIT_CLOCK_GATE_DISABLE |
	       OVRUNIT_CLOCK_GATE_DISABLE |
	       OVCUNIT_CLOCK_GATE_DISABLE);
    } else if (IS_I965GM(pI830)) {
	OUTREG(RENCLK_GATE_D1, I965_RCC_CLOCK_GATE_DISABLE);
	OUTREG(RENCLK_GATE_D2, 0);
	OUTREG(DSPCLK_GATE_D, 0);
	OUTREG(RAMCLK_GATE_D, 0);
	OUTREG16(DEUC, 0);
    } else if (IS_I965G(pI830)) {
	OUTREG(RENCLK_GATE_D1, I965_RCZ_CLOCK_GATE_DISABLE |
	       I965_RCC_CLOCK_GATE_DISABLE |
	       I965_RCPB_CLOCK_GATE_DISABLE |
	       I965_ISC_CLOCK_GATE_DISABLE |
	       I965_FBC_CLOCK_GATE_DISABLE);
	OUTREG(RENCLK_GATE_D2, 0);
    } else if (IS_I855(pI830) || IS_I865G(pI830)) {
	OUTREG(RENCLK_GATE_D1, SV_CLOCK_GATE_DISABLE);
    } else if (IS_I830(pI830)) {
	OUTREG(DSPCLK_GATE_D, OVRUNIT_CLOCK_GATE_DISABLE);
    }
}

static void
i830_init_bios_control(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

   /* Set "extended desktop" */
   OUTREG(SWF0, INREG(SWF0) | (1 << 21));

   /* Set "driver loaded",  "OS unknown", "APM 1.2" */
   OUTREG(SWF4, (INREG(SWF4) & ~((3 << 19) | (7 << 16))) |
		(1 << 23) | (2 << 16));
}

static int
I830LVDSPresent(ScrnInfoPtr pScrn)
{
   xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR (pScrn);
   int o, lvds_detected = FALSE;

   for (o = 0; o < config->num_output; o++) {
      xf86OutputPtr	   output = config->output[o];
      I830OutputPrivatePtr intel_output = output->driver_private;

      if (intel_output->type == I830_OUTPUT_LVDS)
	  lvds_detected = TRUE;
   }

   return lvds_detected;
}
/**
 * Setup the CRTCs
 */


static void 
I830PreInitDDC(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

   if (!xf86LoadSubModule(pScrn, "ddc")) {
      pI830->ddc2 = FALSE;
   } else {
      xf86LoaderReqSymLists(I810ddcSymbols, NULL);
      pI830->ddc2 = TRUE;
   }

   /* DDC can use I2C bus */
   /* Load I2C if we have the code to use it */
   if (pI830->ddc2) {
      if (xf86LoadSubModule(pScrn, "i2c")) {
	 xf86LoaderReqSymLists(I810i2cSymbols, NULL);

	 pI830->ddc2 = TRUE;
      } else {
	 pI830->ddc2 = FALSE;
      }
   }
}

static void
PreInitCleanup(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

   if (I830IsPrimary(pScrn)) {
      if (pI830->entityPrivate)
	 pI830->entityPrivate->pScrn_1 = NULL;
   } else {
      if (pI830->entityPrivate)
         pI830->entityPrivate->pScrn_2 = NULL;
   }
   if (pI830->MMIOBase)
      I830UnmapMMIO(pScrn);
   I830FreeRec(pScrn);
}

Bool
I830IsPrimary(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

   if (xf86IsEntityShared(pScrn->entityList[0])) {
	if (pI830->init == 0) return TRUE;
	else return FALSE;
   }

   return TRUE;
}

static Bool
i830_xf86crtc_resize (ScrnInfoPtr scrn, int width, int height)
{
    scrn->virtualX = width;
    scrn->virtualY = height;
    return TRUE;
}

static const xf86CrtcConfigFuncsRec i830_xf86crtc_config_funcs = {
    i830_xf86crtc_resize
};

#define HOTKEY_BIOS_SWITCH	0
#define HOTKEY_DRIVER_NOTIFY	1

/**
 * Controls the BIOS's behavior on hotkey switch.
 *
 * If the mode is HOTKEY_BIOS_SWITCH, the BIOS will be set to do a mode switch
 * on its own and update the state in the scratch register.
 * If the mode is HOTKEY_DRIVER_NOTIFY, the BIOS won't do a mode switch and
 * will just update the state to represent what it would have been switched to.
 */
static void
i830SetHotkeyControl(ScrnInfoPtr pScrn, int mode)
{
   I830Ptr pI830 = I830PTR(pScrn);
   uint8_t gr18;

   gr18 = pI830->readControl(pI830, GRX, 0x18);
   if (mode == HOTKEY_BIOS_SWITCH)
      gr18 &= ~HOTKEY_VBIOS_SWITCH_BLOCK;
   else
      gr18 |= HOTKEY_VBIOS_SWITCH_BLOCK;
   pI830->writeControl(pI830, GRX, 0x18, gr18);
}

static Bool
i830_detect_chipset(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    MessageType from = X_PROBED;
    const char *chipname;
    uint32_t capid;
    int fb_bar, mmio_bar;

    switch (DEVICE_ID(pI830->PciInfo)) {
    case PCI_CHIP_I830_M:
	chipname = "830M";
	break;
    case PCI_CHIP_845_G:
	chipname = "845G";
	break;
    case PCI_CHIP_I855_GM:
	/* Check capid register to find the chipset variant */
#if XSERVER_LIBPCIACCESS
	pci_device_cfg_read_u32 (pI830->PciInfo, &capid, I85X_CAPID);
#else
	capid = pciReadLong (pI830->PciTag, I85X_CAPID);
#endif
	pI830->variant = (capid >> I85X_VARIANT_SHIFT) & I85X_VARIANT_MASK;
	switch (pI830->variant) {
	case I855_GM:
	    chipname = "855GM";
	    break;
	case I855_GME:
	    chipname = "855GME";
	    break;
	case I852_GM:
	    chipname = "852GM";
	    break;
	case I852_GME:
	    chipname = "852GME";
	    break;
	default:
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Unknown 852GM/855GM variant: 0x%x)\n", pI830->variant);
	    chipname = "852GM/855GM (unknown variant)";
	    break;
	}
	break;
    case PCI_CHIP_I865_G:
	chipname = "865G";
	break;
    case PCI_CHIP_I915_G:
	chipname = "915G";
	break;
    case PCI_CHIP_E7221_G:
	chipname = "E7221 (i915)";
	break;
    case PCI_CHIP_I915_GM:
	chipname = "915GM";
	break;
    case PCI_CHIP_I945_G:
	chipname = "945G";
	break;
    case PCI_CHIP_I945_GM:
	chipname = "945GM";
	break;
    case PCI_CHIP_I945_GME:
	chipname = "945GME";
	break;
    case PCI_CHIP_I965_G:
	chipname = "965G";
	break;
    case PCI_CHIP_G35_G:
	chipname = "G35";
	break;
    case PCI_CHIP_I965_Q:
	chipname = "965Q";
	break;
    case PCI_CHIP_I946_GZ:
	chipname = "946GZ";
	break;
    case PCI_CHIP_I965_GM:
	chipname = "965GM";
	break;
    case PCI_CHIP_I965_GME:
	chipname = "965GME/GLE";
	break;
    case PCI_CHIP_G33_G:
	chipname = "G33";
	break;
    case PCI_CHIP_Q35_G:
	chipname = "Q35";
	break;
    case PCI_CHIP_Q33_G:
	chipname = "Q33";
	break;
    case PCI_CHIP_GM45_GM:
	chipname = "Mobile Intel® GM45 Express Chipset";
	break;
    case PCI_CHIP_IGD_E_G:
	chipname = "Intel Integrated Graphics Device";
	break;
    case PCI_CHIP_G45_G:
	chipname = "G45/G43";
	break;
    case PCI_CHIP_Q45_G:
	chipname = "Q45/Q43";
	break;
   default:
	chipname = "unknown chipset";
	break;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Integrated Graphics Chipset: Intel(R) %s\n", chipname);

    /* Set the Chipset and ChipRev, allowing config file entries to override. */
    if (pI830->pEnt->device->chipset && *pI830->pEnt->device->chipset) {
	pScrn->chipset = pI830->pEnt->device->chipset;
	from = X_CONFIG;
    } else if (pI830->pEnt->device->chipID >= 0) {
	pScrn->chipset = (char *)xf86TokenToString(I830Chipsets,
						   pI830->pEnt->device->chipID);
	from = X_CONFIG;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipID override: 0x%04X\n",
		   pI830->pEnt->device->chipID);
	DEVICE_ID(pI830->PciInfo) = pI830->pEnt->device->chipID;
    } else {
	from = X_PROBED;
	pScrn->chipset = (char *)xf86TokenToString(I830Chipsets,
						   DEVICE_ID(pI830->PciInfo));
    }

    if (pI830->pEnt->device->chipRev >= 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipRev override: %d\n",
		   pI830->pEnt->device->chipRev);
    }

    xf86DrvMsg(pScrn->scrnIndex, from, "Chipset: \"%s\"\n",
	       (pScrn->chipset != NULL) ? pScrn->chipset : "Unknown i8xx");

    /* Now that we know the chipset, figure out the resource base addrs */
    if (IS_I9XX(pI830)) {
	fb_bar = 2;
	mmio_bar = 0;
    } else {
	fb_bar = 0;
	mmio_bar = 1;
    }

    if (pI830->pEnt->device->MemBase != 0) {
	pI830->LinearAddr = pI830->pEnt->device->MemBase;
	from = X_CONFIG;
    } else {
	pI830->LinearAddr = I810_MEMBASE (pI830->PciInfo, fb_bar);
	if (pI830->LinearAddr == 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "No valid FB address in PCI config space\n");
	    PreInitCleanup(pScrn);
	    return FALSE;
	}
    }

    xf86DrvMsg(pScrn->scrnIndex, from, "Linear framebuffer at 0x%lX\n",
	       (unsigned long)pI830->LinearAddr);

    if (pI830->pEnt->device->IOBase != 0) {
	pI830->MMIOAddr = pI830->pEnt->device->IOBase;
	from = X_CONFIG;
    } else {
	pI830->MMIOAddr = I810_MEMBASE (pI830->PciInfo, mmio_bar);
	if (pI830->MMIOAddr == 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "No valid MMIO address in PCI config space\n");
	    PreInitCleanup(pScrn);
	    return FALSE;
	}
    }

    xf86DrvMsg(pScrn->scrnIndex, from, "IO registers at addr 0x%lX\n",
	       (unsigned long)pI830->MMIOAddr);

    /* Now figure out mapsize on 8xx chips */
    if (IS_I830(pI830) || IS_845G(pI830)) {
#if XSERVER_LIBPCIACCESS
	uint16_t		gmch_ctrl;
	struct pci_device *bridge;

	bridge = intel_host_bridge ();
	pci_device_cfg_read_u16 (bridge, &gmch_ctrl, I830_GMCH_CTRL);
#else
	PCITAG bridge;
	uint16_t gmch_ctrl;

	bridge = pciTag(0, 0, 0);		/* This is always the host bridge */
	gmch_ctrl = pciReadWord(bridge, I830_GMCH_CTRL);
#endif
	if ((gmch_ctrl & I830_GMCH_MEM_MASK) == I830_GMCH_MEM_128M) {
	    pI830->FbMapSize = 0x8000000;
	} else {
	    pI830->FbMapSize = 0x4000000; /* 64MB - has this been tested ?? */
	}
    } else {
	if (IS_I9XX(pI830)) {
#if XSERVER_LIBPCIACCESS
	    pI830->FbMapSize = pI830->PciInfo->regions[fb_bar].size;
#else
	    pI830->FbMapSize = 1UL << pciGetBaseSize(pI830->PciTag, 2, TRUE,
						     NULL);
#endif
	} else {
	    /* 128MB aperture for later i8xx series. */
	    pI830->FbMapSize = 0x8000000;
	}
    }

    return TRUE;
}

/**
 * This is called per zaphod head (so usually just once) to do initialization
 * before the Screen is created.
 *
 * This code generally covers probing, module loading, option handling
 * card mapping, and RandR setup.
 */
static Bool
I830PreInit(ScrnInfoPtr pScrn, int flags)
{
   xf86CrtcConfigPtr   xf86_config;
   vgaHWPtr hwp;
   I830Ptr pI830;
   MessageType from = X_PROBED;
   rgb defaultWeight = { 0, 0, 0 };
   EntityInfoPtr pEnt;
   I830EntPtr pI830Ent = NULL;					
   int flags24;
   int i;
   char *s;
   pointer pVBEModule = NULL;
   int num_pipe;
   int max_width, max_height;

   if (pScrn->numEntities != 1)
      return FALSE;

   /* Load int10 module */
   if (!xf86LoadSubModule(pScrn, "int10"))
      return FALSE;
   xf86LoaderReqSymLists(I810int10Symbols, NULL);

   /* Load vbe module */
   if (!(pVBEModule = xf86LoadSubModule(pScrn, "vbe")))
      return FALSE;
   xf86LoaderReqSymLists(I810vbeSymbols, NULL);

   pEnt = xf86GetEntityInfo(pScrn->entityList[0]);

   if (flags & PROBE_DETECT) {
      I830ProbeDDC(pScrn, pEnt->index);
      return TRUE;
   }

   /* The vgahw module should be loaded here when needed */
   if (!xf86LoadSubModule(pScrn, "vgahw"))
      return FALSE;
   xf86LoaderReqSymLists(I810vgahwSymbols, NULL);

   /* Allocate a vgaHWRec */
   if (!vgaHWGetHWRec(pScrn))
      return FALSE;

   /* Allocate driverPrivate */
   if (!I830GetRec(pScrn))
      return FALSE;

   pI830 = I830PTR(pScrn);
   pI830->SaveGeneration = -1;
   pI830->pEnt = pEnt;

   pScrn->displayWidth = 640; /* default it */

   if (pI830->pEnt->location.type != BUS_PCI)
      return FALSE;

   pI830->PciInfo = xf86GetPciInfoForEntity(pI830->pEnt->index);
#if !XSERVER_LIBPCIACCESS
   pI830->PciTag = pciTag(pI830->PciInfo->bus, pI830->PciInfo->device,
			  pI830->PciInfo->func);
#endif

    /* Allocate an entity private if necessary */
    if (xf86IsEntityShared(pScrn->entityList[0])) {
	pI830Ent = xf86GetEntityPrivate(pScrn->entityList[0],
					I830EntityIndex)->ptr;
        pI830->entityPrivate = pI830Ent;
    } else 
        pI830->entityPrivate = NULL;

   if (xf86RegisterResources(pI830->pEnt->index, NULL, ResNone)) {
      PreInitCleanup(pScrn);
      return FALSE;
   }

   if (xf86IsEntityShared(pScrn->entityList[0])) {
      if (xf86IsPrimInitDone(pScrn->entityList[0])) {
	 pI830->init = 1;

         if (!pI830Ent->pScrn_1) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
 		 "Failed to setup second head due to primary head failure.\n");
	    return FALSE;
         }
      } else {
         xf86SetPrimInitDone(pScrn->entityList[0]);
	 pI830->init = 0;
      }
   }

   if (xf86IsEntityShared(pScrn->entityList[0])) {
      if (!I830IsPrimary(pScrn)) {
         pI830Ent->pScrn_2 = pScrn;
      } else {
         pI830Ent->pScrn_1 = pScrn;
         pI830Ent->pScrn_2 = NULL;
      }
   }

   pScrn->racMemFlags = RAC_FB | RAC_COLORMAP;
   pScrn->monitor = pScrn->confScreen->monitor;
   pScrn->progClock = TRUE;
   pScrn->rgbBits = 8;

   flags24 = Support32bppFb | PreferConvert24to32 | SupportConvert24to32;

   if (!xf86SetDepthBpp(pScrn, 0, 0, 0, flags24))
      return FALSE;

   switch (pScrn->depth) {
   case 8:
   case 15:
   case 16:
   case 24:
      break;
   default:
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "Given depth (%d) is not supported by I830 driver\n",
		 pScrn->depth);
      return FALSE;
   }
   xf86PrintDepthBpp(pScrn);

   if (!xf86SetWeight(pScrn, defaultWeight, defaultWeight))
      return FALSE;
   if (!xf86SetDefaultVisual(pScrn, -1))
      return FALSE;

   hwp = VGAHWPTR(pScrn);
   pI830->cpp = pScrn->bitsPerPixel / 8;

   pI830->preinit = TRUE;

   /* Process the options */
   xf86CollectOptions(pScrn, NULL);
   if (!(pI830->Options = xalloc(sizeof(I830Options))))
      return FALSE;
   memcpy(pI830->Options, I830Options, sizeof(I830Options));
   xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pI830->Options);

   if (xf86ReturnOptValBool(pI830->Options, OPTION_MODEDEBUG, FALSE)) {
      pI830->debug_modes = TRUE;
   } else {
      pI830->debug_modes = FALSE;
   }

   if (xf86ReturnOptValBool(pI830->Options, OPTION_LVDS24BITMODE, FALSE)) {
      pI830->lvds_24_bit_mode = TRUE;
   } else {
      pI830->lvds_24_bit_mode = FALSE;
   }

   if (xf86ReturnOptValBool(pI830->Options, OPTION_LVDSFIXEDMODE, TRUE)) {
      pI830->skip_panel_detect = FALSE;
   } else {
      pI830->skip_panel_detect = TRUE;
   }

   if (xf86ReturnOptValBool(pI830->Options, OPTION_FORCEENABLEPIPEA, FALSE))
       pI830->quirk_flag |= QUIRK_PIPEA_FORCE;

   /* We have to use PIO to probe, because we haven't mapped yet. */
   I830SetPIOAccess(pI830);

   if (!i830_detect_chipset(pScrn))
       return FALSE;

   /* check quirks */
   i830_fixup_devices(pScrn);

   /* Allocate an xf86CrtcConfig */
   xf86CrtcConfigInit (pScrn, &i830_xf86crtc_config_funcs);
   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);

   /* See i830_exa.c comments for why we limit the framebuffer size like this.
    */
   if (IS_I965G(pI830)) {
      max_width = 8192;
      max_height = 8192;
   } else {
      max_width = 2048;
      max_height = 2048;
   }
   xf86CrtcSetSizeRange (pScrn, 320, 200, max_width, max_height);

   /* Some of the probing needs MMIO access, so map it here. */
   I830MapMMIO(pScrn);

   if (pI830->debug_modes) {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Hardware state on X startup:\n");
      i830DumpRegs (pScrn);
   }

   i830TakeRegSnapshot(pScrn);

   if (DEVICE_ID(pI830->PciInfo) == PCI_CHIP_E7221_G)
      num_pipe = 1;
   else
   if (IS_MOBILE(pI830) || IS_I9XX(pI830))
      num_pipe = 2;
   else
      num_pipe = 1;
   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%d display pipe%s available.\n",
	      num_pipe, num_pipe > 1 ? "s" : "");

   if (xf86ReturnOptValBool(pI830->Options, OPTION_NOACCEL, FALSE)) {
      pI830->noAccel = TRUE;
   }

   /*
    * The ugliness below:
    * If either XAA or EXA (exclusive) is compiled in, default to it.
    * 
    * If both are compiled in, and the user didn't specify noAccel, use the
    * config option AccelMethod to determine which to use, defaulting to EXA
    * if none is specified, or if the string was unrecognized.
    *
    * All this *could* go away if we removed XAA support from this driver,
    * for example. :)
    */
   if (!pI830->noAccel) {
#ifdef I830_USE_EXA
       pI830->useEXA = TRUE;
#else
       pI830->useEXA = FALSE;
#endif
#if defined(I830_USE_XAA) && defined(I830_USE_EXA)
       from = X_DEFAULT;
       if ((s = (char *)xf86GetOptValString(pI830->Options,
					    OPTION_ACCELMETHOD))) {
	   if (!xf86NameCmp(s, "EXA")) {
	       from = X_CONFIG;
	       pI830->useEXA = TRUE;
	   }
	   else if (!xf86NameCmp(s, "XAA")) {
	       from = X_CONFIG;
	       pI830->useEXA = FALSE;
	   }
       }
#endif
       xf86DrvMsg(pScrn->scrnIndex, from, "Using %s for acceleration\n",
		  pI830->useEXA ? "EXA" : "XAA");
   }

   if (xf86ReturnOptValBool(pI830->Options, OPTION_SW_CURSOR, FALSE)) {
      pI830->SWCursor = TRUE;
   }

   pI830->directRenderingDisabled =
	!xf86ReturnOptValBool(pI830->Options, OPTION_DRI, TRUE);

#ifdef XF86DRI
   if (!pI830->directRenderingDisabled) {
      if (pI830->noAccel || pI830->SWCursor) {
	 xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "DRI is disabled because it "
		    "needs HW cursor and 2D acceleration.\n");
	 pI830->directRenderingDisabled = TRUE;
      } else if (pScrn->depth != 16 && pScrn->depth != 24) {
	 xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "DRI is disabled because it "
		    "runs only at depths 16 and 24.\n");
	 pI830->directRenderingDisabled = TRUE;
      }

      if (!pI830->directRenderingDisabled) {
	 pI830->allocate_classic_textures = TRUE;

	 from = X_PROBED;

#ifdef XF86DRI_MM
	 if (!IS_I965G(pI830)) {
	    Bool tmp;

	    if (xf86GetOptValBool(pI830->Options,
				  OPTION_INTELTEXPOOL, &tmp)) {
	       from = X_CONFIG;
	       if (!tmp)
		  pI830->allocate_classic_textures = FALSE;
	    }
	 }
#endif
      }
   } 
   
#endif

   if (i830_bios_init(pScrn))
      xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		 "VBIOS initialization failed.\n");

   I830PreInitDDC(pScrn);
   for (i = 0; i < num_pipe; i++) {
       i830_crtc_init(pScrn, i);
   }
   I830SetupOutputs(pScrn);

   SaveHWState(pScrn);
   if (!xf86InitialConfiguration (pScrn, FALSE))
   {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes.\n");
      RestoreHWState(pScrn);
      PreInitCleanup(pScrn);
      return FALSE;
   }
   RestoreHWState(pScrn);

   /* XXX This should go away, replaced by xf86Crtc.c support for it */
   pI830->rotation = RR_Rotate_0;

   /*
    * Let's setup the mobile systems to check the lid status
    */
   if (IS_MOBILE(pI830)) {
      pI830->checkDevices = TRUE;

      if (!xf86ReturnOptValBool(pI830->Options, OPTION_CHECKDEVICES, TRUE)) {
         pI830->checkDevices = FALSE;
         xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Monitoring connected displays disabled\n");
      } else
      if (pI830->entityPrivate && !I830IsPrimary(pScrn) &&
          !I830PTR(pI830->entityPrivate->pScrn_1)->checkDevices) {
         /* If checklid is off, on the primary head, then 
          * turn it off on the secondary*/
         xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Monitoring connected displays disabled\n");
         pI830->checkDevices = FALSE;
      } else
         xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Monitoring connected displays enabled\n");
   } else
      pI830->checkDevices = FALSE;

   pI830->stolen_size = I830DetectMemory(pScrn);

   pI830->XvDisabled =
	!xf86ReturnOptValBool(pI830->Options, OPTION_XVIDEO, TRUE);

#ifdef I830_XV
   if (xf86GetOptValInteger(pI830->Options, OPTION_VIDEO_KEY,
			    &(pI830->colorKey))) {
      from = X_CONFIG;
   } else if (xf86GetOptValInteger(pI830->Options, OPTION_COLOR_KEY,
			    &(pI830->colorKey))) {
      from = X_CONFIG;
   } else {
      pI830->colorKey = (1 << pScrn->offset.red) |
			(1 << pScrn->offset.green) |
			(((pScrn->mask.blue >> pScrn->offset.blue) - 1) <<
			 pScrn->offset.blue);
      from = X_DEFAULT;
   }
   xf86DrvMsg(pScrn->scrnIndex, from, "video overlay key set to 0x%x\n",
	      pI830->colorKey);
#endif

#ifdef XF86DRI
   pI830->allowPageFlip = FALSE;
   from = (!pI830->directRenderingDisabled &&
	   xf86GetOptValBool(pI830->Options, OPTION_PAGEFLIP,
			     &pI830->allowPageFlip)) ? X_CONFIG : X_DEFAULT;

   xf86DrvMsg(pScrn->scrnIndex, from, "Will%s try to enable page flipping\n",
	      pI830->allowPageFlip ? "" : " not");
#endif

#ifdef XF86DRI
   pI830->TripleBuffer = FALSE;
   from =  (!pI830->directRenderingDisabled &&
	    xf86GetOptValBool(pI830->Options, OPTION_TRIPLEBUFFER,
			      &pI830->TripleBuffer)) ? X_CONFIG : X_DEFAULT;

   xf86DrvMsg(pScrn->scrnIndex, from, "Triple buffering %sabled\n",
	      pI830->TripleBuffer ? "en" : "dis");
#endif

#ifdef INTEL_XVMC
   pI830->XvMCEnabled = FALSE;
   from =  (!pI830->directRenderingDisabled &&
	    xf86GetOptValBool(pI830->Options, OPTION_XVMC,
			      &pI830->XvMCEnabled)) ? X_CONFIG : X_DEFAULT;
   xf86DrvMsg(pScrn->scrnIndex, from, "Intel XvMC decoder %sabled\n",
	   pI830->XvMCEnabled ? "en" : "dis");
#endif

   /*
    * If the driver can do gamma correction, it should call xf86SetGamma() here.
    */

   {
      Gamma zeros = { 0.0, 0.0, 0.0 };

      if (!xf86SetGamma(pScrn, zeros)) {
         PreInitCleanup(pScrn);
	 return FALSE;
      }
   }

   /* Check if the HW cursor needs physical address. */
   if (IS_MOBILE(pI830) || IS_I9XX(pI830))
      pI830->CursorNeedsPhysical = TRUE;
   else
      pI830->CursorNeedsPhysical = FALSE;

   if (IS_I965G(pI830) || IS_G33CLASS(pI830))
      pI830->CursorNeedsPhysical = FALSE;

   /*
    * XXX If we knew the pre-initialised GTT format for certain, we could
    * probably figure out the physical address even in the StolenOnly case.
    */
   if (!I830IsPrimary(pScrn)) {
        I830Ptr pI8301 = I830PTR(pI830->entityPrivate->pScrn_1);
	if (!pI8301->SWCursor) {
          xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		 "Using HW Cursor because it's enabled on primary head.\n");
          pI830->SWCursor = FALSE;
        }
   } else 
   if (pI830->StolenOnly && pI830->CursorNeedsPhysical && !pI830->SWCursor) {
      xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		 "HW Cursor disabled because it needs agpgart memory.\n");
      pI830->SWCursor = TRUE;
   }

   if (pScrn->modes == NULL) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No modes.\n");
      PreInitCleanup(pScrn);
      return FALSE;
   }
   pScrn->currentMode = pScrn->modes;

   if (!IS_I965G(pI830) && pScrn->virtualY > 2048) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Cannot support > 2048 vertical lines. disabling acceleration.\n");
      pI830->noAccel = TRUE;
   }

   /* Set display resolution */
   xf86SetDpi(pScrn, 0, 0);

   /* Load the required sub modules */
   if (!xf86LoadSubModule(pScrn, "fb")) {
      PreInitCleanup(pScrn);
      return FALSE;
   }

   xf86LoaderReqSymLists(I810fbSymbols, NULL);

#ifdef I830_USE_XAA
   if (!pI830->noAccel && !pI830->useEXA) {
      if (!xf86LoadSubModule(pScrn, "xaa")) {
	 PreInitCleanup(pScrn);
	 return FALSE;
      }
      xf86LoaderReqSymLists(I810xaaSymbols, NULL);
   }
#endif

#ifdef I830_USE_EXA
   if (!pI830->noAccel && pI830->useEXA) {
      XF86ModReqInfo req;
      int errmaj, errmin;

      memset(&req, 0, sizeof(req));
      req.majorversion = 2;
#if EXA_VERSION_MINOR >= 2
      req.minorversion = 2;
#else
      req.minorversion = 1;
#endif
      if (!LoadSubModule(pScrn->module, "exa", NULL, NULL, NULL, &req,
		&errmaj, &errmin)) {
	 LoaderErrorMsg(NULL, "exa", errmaj, errmin);
	 PreInitCleanup(pScrn);
	 return FALSE;
      }
      xf86LoaderReqSymLists(I830exaSymbols, NULL);
   }
#endif
   if (!pI830->SWCursor) {
      if (!xf86LoadSubModule(pScrn, "ramdac")) {
	 PreInitCleanup(pScrn);
	 return FALSE;
      }
      xf86LoaderReqSymLists(I810ramdacSymbols, NULL);
   }

   i830CompareRegsToSnapshot(pScrn, "After PreInit");

   I830UnmapMMIO(pScrn);

   /*  We won't be using the VGA access after the probe. */
   I830SetMMIOAccess(pI830);
   xf86SetOperatingState(resVgaIo, pI830->pEnt->index, ResUnusedOpr);
   xf86SetOperatingState(resVgaMem, pI830->pEnt->index, ResDisableOpr);

#if 0
   if (I830IsPrimary(pScrn)) {
      vbeFree(pI830->pVbe);
   }
   pI830->pVbe = NULL;
#endif

#if defined(XF86DRI)
   /* Load the dri module if requested. */
   if (xf86ReturnOptValBool(pI830->Options, OPTION_DRI, FALSE) &&
       !pI830->directRenderingDisabled) {
      if (xf86LoadSubModule(pScrn, "dri")) {
	 xf86LoaderReqSymLists(I810driSymbols, I810drmSymbols, NULL);
      }
   }
#endif

   pI830->preinit = FALSE;

   return TRUE;
}

/*
 * Reset registers that it doesn't make sense to save/restore to a sane state.
 * This is basically the ring buffer and fence registers.  Restoring these
 * doesn't make sense without restoring GTT mappings.  This is something that
 * whoever gets control next should do.
 */
static void
i830_stop_ring(ScrnInfoPtr pScrn, Bool flush)
{
   I830Ptr pI830 = I830PTR(pScrn);
   unsigned long temp;

   DPRINTF(PFX, "ResetState: flush is %s\n", BOOLTOSTRING(flush));

   if (!I830IsPrimary(pScrn)) return;

   if (pI830->entityPrivate)
      pI830->entityPrivate->RingRunning = 0;

   /* Flush the ring buffer (if enabled), then disable it. */
   if (!pI830->noAccel) {
      temp = INREG(LP_RING + RING_LEN);
      if (temp & RING_VALID) {
	 i830_refresh_ring(pScrn);
	 I830Sync(pScrn);
      }

      OUTREG(LP_RING + RING_LEN, 0);
      OUTREG(LP_RING + RING_HEAD, 0);
      OUTREG(LP_RING + RING_TAIL, 0);
      OUTREG(LP_RING + RING_START, 0);
   }
}

static void
i830_start_ring(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   unsigned int itemp;

   DPRINTF(PFX, "SetRingRegs\n");

   if (pI830->noAccel)
      return;

   if (!I830IsPrimary(pScrn)) return;

   if (pI830->entityPrivate)
      pI830->entityPrivate->RingRunning = 1;

   OUTREG(LP_RING + RING_LEN, 0);
   OUTREG(LP_RING + RING_TAIL, 0);
   OUTREG(LP_RING + RING_HEAD, 0);

   assert((pI830->LpRing->mem->offset & I830_RING_START_MASK) ==
	   pI830->LpRing->mem->offset);

   /* Don't care about the old value.  Reserved bits must be zero anyway. */
   itemp = pI830->LpRing->mem->offset;
   OUTREG(LP_RING + RING_START, itemp);

   if (((pI830->LpRing->mem->size - 4096) & I830_RING_NR_PAGES) !=
       pI830->LpRing->mem->size - 4096) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "I830SetRingRegs: Ring buffer size - 4096 (%lx) violates its "
		 "mask (%x)\n", pI830->LpRing->mem->size - 4096,
		 I830_RING_NR_PAGES);
   }
   /* Don't care about the old value.  Reserved bits must be zero anyway. */
   itemp = (pI830->LpRing->mem->size - 4096) & I830_RING_NR_PAGES;
   itemp |= (RING_NO_REPORT | RING_VALID);
   OUTREG(LP_RING + RING_LEN, itemp);
   i830_refresh_ring(pScrn);
}

void
i830_refresh_ring(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

   /* If we're reaching RefreshRing as a result of grabbing the DRI lock
    * before we've set up the ringbuffer, don't bother.
    */
   if (pI830->LpRing->mem == NULL)
       return;

   pI830->LpRing->head = INREG(LP_RING + RING_HEAD) & I830_HEAD_MASK;
   pI830->LpRing->tail = INREG(LP_RING + RING_TAIL);
   pI830->LpRing->space = pI830->LpRing->head - (pI830->LpRing->tail + 8);
   if (pI830->LpRing->space < 0)
      pI830->LpRing->space += pI830->LpRing->mem->size;
   i830MarkSync(pScrn);
}

enum pipe {
    PIPE_A = 0,
    PIPE_B,
};

static Bool
i830_pipe_enabled(I830Ptr pI830, enum pipe pipe)
{
    if (pipe == PIPE_A)
	return (INREG(PIPEACONF) & PIPEACONF_ENABLE);
    else
	return (INREG(PIPEBCONF) & PIPEBCONF_ENABLE);
}

static void
i830_save_palette(I830Ptr pI830, enum pipe pipe)
{
    int i;

    if (!i830_pipe_enabled(pI830, pipe))
	return;

    for(i= 0; i < 256; i++) {
	if (pipe == PIPE_A)
	    pI830->savePaletteA[i] = INREG(PALETTE_A + (i << 2));
	else
	    pI830->savePaletteB[i] = INREG(PALETTE_B + (i << 2));
    }
}

static void
i830_restore_palette(I830Ptr pI830, enum pipe pipe)
{
    int i;

    if (!i830_pipe_enabled(pI830, pipe))
	return;

    for(i= 0; i < 256; i++) {
	if (pipe == PIPE_A)
	    OUTREG(PALETTE_A + (i << 2), pI830->savePaletteA[i]);
	else
	    OUTREG(PALETTE_B + (i << 2), pI830->savePaletteB[i]);
    }
}

static Bool
SaveHWState(ScrnInfoPtr pScrn)
{
   xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
   I830Ptr pI830 = I830PTR(pScrn);
   vgaHWPtr hwp = VGAHWPTR(pScrn);
   vgaRegPtr vgaReg = &hwp->SavedReg;
   int i;

   if (pI830->fb_compression) {
       pI830->saveFBC_CFB_BASE = INREG(FBC_CFB_BASE);
       pI830->saveFBC_LL_BASE = INREG(FBC_LL_BASE);
       pI830->saveFBC_CONTROL2 = INREG(FBC_CONTROL2);
       pI830->saveFBC_CONTROL = INREG(FBC_CONTROL);
       pI830->saveFBC_FENCE_OFF = INREG(FBC_FENCE_OFF);
   }

   /* Save video mode information for native mode-setting. */
   if (!DSPARB_HWCONTROL(pI830))
       pI830->saveDSPARB = INREG(DSPARB);

   pI830->saveDSPACNTR = INREG(DSPACNTR);
   pI830->savePIPEACONF = INREG(PIPEACONF);
   pI830->savePIPEASRC = INREG(PIPEASRC);
   pI830->saveFPA0 = INREG(FPA0);
   pI830->saveFPA1 = INREG(FPA1);
   pI830->saveDPLL_A = INREG(DPLL_A);
   if (IS_I965G(pI830))
      pI830->saveDPLL_A_MD = INREG(DPLL_A_MD);
   pI830->saveHTOTAL_A = INREG(HTOTAL_A);
   pI830->saveHBLANK_A = INREG(HBLANK_A);
   pI830->saveHSYNC_A = INREG(HSYNC_A);
   pI830->saveVTOTAL_A = INREG(VTOTAL_A);
   pI830->saveVBLANK_A = INREG(VBLANK_A);
   pI830->saveVSYNC_A = INREG(VSYNC_A);
   pI830->saveBCLRPAT_A = INREG(BCLRPAT_A);
   pI830->saveDSPASTRIDE = INREG(DSPASTRIDE);
   pI830->saveDSPASIZE = INREG(DSPASIZE);
   pI830->saveDSPAPOS = INREG(DSPAPOS);
   pI830->saveDSPABASE = INREG(DSPABASE);

   i830_save_palette(pI830, PIPE_A);

   if(xf86_config->num_crtc == 2) {
      pI830->savePIPEBCONF = INREG(PIPEBCONF);
      pI830->savePIPEBSRC = INREG(PIPEBSRC);
      pI830->saveDSPBCNTR = INREG(DSPBCNTR);
      pI830->saveFPB0 = INREG(FPB0);
      pI830->saveFPB1 = INREG(FPB1);
      pI830->saveDPLL_B = INREG(DPLL_B);
      if (IS_I965G(pI830))
	 pI830->saveDPLL_B_MD = INREG(DPLL_B_MD);
      pI830->saveHTOTAL_B = INREG(HTOTAL_B);
      pI830->saveHBLANK_B = INREG(HBLANK_B);
      pI830->saveHSYNC_B = INREG(HSYNC_B);
      pI830->saveVTOTAL_B = INREG(VTOTAL_B);
      pI830->saveVBLANK_B = INREG(VBLANK_B);
      pI830->saveVSYNC_B = INREG(VSYNC_B);
      pI830->saveBCLRPAT_B = INREG(BCLRPAT_B);
      pI830->saveDSPBSTRIDE = INREG(DSPBSTRIDE);
      pI830->saveDSPBSIZE = INREG(DSPBSIZE);
      pI830->saveDSPBPOS = INREG(DSPBPOS);
      pI830->saveDSPBBASE = INREG(DSPBBASE);

      i830_save_palette(pI830, PIPE_B);
   }

   if (IS_I965G(pI830)) {
      pI830->saveDSPASURF = INREG(DSPASURF);
      pI830->saveDSPBSURF = INREG(DSPBSURF);
      pI830->saveDSPATILEOFF = INREG(DSPATILEOFF);
      pI830->saveDSPBTILEOFF = INREG(DSPBTILEOFF);
   }

   pI830->saveVCLK_DIVISOR_VGA0 = INREG(VCLK_DIVISOR_VGA0);
   pI830->saveVCLK_DIVISOR_VGA1 = INREG(VCLK_DIVISOR_VGA1);
   pI830->saveVCLK_POST_DIV = INREG(VCLK_POST_DIV);
   pI830->saveVGACNTRL = INREG(VGACNTRL);

   pI830->saveCURSOR_A_CONTROL = INREG(CURSOR_A_CONTROL);
   pI830->saveCURSOR_A_POSITION = INREG(CURSOR_A_POSITION);
   pI830->saveCURSOR_A_BASE = INREG(CURSOR_A_BASE);
   pI830->saveCURSOR_B_CONTROL = INREG(CURSOR_B_CONTROL);
   pI830->saveCURSOR_B_POSITION = INREG(CURSOR_B_POSITION);
   pI830->saveCURSOR_B_BASE = INREG(CURSOR_B_BASE);

   for(i = 0; i < 7; i++) {
      pI830->saveSWF[i] = INREG(SWF0 + (i << 2));
      pI830->saveSWF[i+7] = INREG(SWF00 + (i << 2));
   }
   pI830->saveSWF[14] = INREG(SWF30);
   pI830->saveSWF[15] = INREG(SWF31);
   pI830->saveSWF[16] = INREG(SWF32);

   pI830->saveDSPCLK_GATE_D = INREG(DSPCLK_GATE_D);
   pI830->saveRENCLK_GATE_D1 = INREG(RENCLK_GATE_D1);

   if (IS_I965G(pI830)) {
      pI830->saveRENCLK_GATE_D2 = INREG(RENCLK_GATE_D2);
      pI830->saveRAMCLK_GATE_D = INREG(RAMCLK_GATE_D);
   }

   if (IS_I965GM(pI830) || IS_GM45(pI830))
      pI830->savePWRCTXA = INREG(PWRCTXA);

   if (IS_MOBILE(pI830) && !IS_I830(pI830))
      pI830->saveLVDS = INREG(LVDS);
   pI830->savePFIT_CONTROL = INREG(PFIT_CONTROL);

   for (i = 0; i < xf86_config->num_output; i++) {
      xf86OutputPtr   output = xf86_config->output[i];
      if (output->funcs->save)
	 (*output->funcs->save) (output);
   }

   vgaHWUnlock(hwp);
   vgaHWSave(pScrn, vgaReg, VGA_SR_FONTS);

   return TRUE;
}

/* Wait for the PLL to settle down after programming */
static void
i830_dpll_settle(void)
{
    usleep(10000); /* 10 ms *should* be plenty */
}

static Bool
RestoreHWState(ScrnInfoPtr pScrn)
{
   xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
   I830Ptr pI830 = I830PTR(pScrn);
   vgaHWPtr hwp = VGAHWPTR(pScrn);
   vgaRegPtr vgaReg = &hwp->SavedReg;
   int i;

   DPRINTF(PFX, "RestoreHWState\n");

#ifdef XF86DRI
   I830DRISetVBlankInterrupt (pScrn, FALSE);
#endif
   /* Disable outputs */
   for (i = 0; i < xf86_config->num_output; i++) {
      xf86OutputPtr   output = xf86_config->output[i];
      output->funcs->dpms(output, DPMSModeOff);
   }
   i830WaitForVblank(pScrn);
   
   /* Disable pipes */
   for (i = 0; i < xf86_config->num_crtc; i++) {
      xf86CrtcPtr crtc = xf86_config->crtc[i];
      crtc->funcs->dpms(crtc, DPMSModeOff);
   }
   i830WaitForVblank(pScrn);

   if (IS_MOBILE(pI830) && !IS_I830(pI830))
      OUTREG(LVDS, pI830->saveLVDS);

   if (!IS_I830(pI830) && !IS_845G(pI830))
     OUTREG(PFIT_CONTROL, pI830->savePFIT_CONTROL);

   if (!DSPARB_HWCONTROL(pI830))
       OUTREG(DSPARB, pI830->saveDSPARB);

   OUTREG(DSPCLK_GATE_D, pI830->saveDSPCLK_GATE_D);
   OUTREG(RENCLK_GATE_D1, pI830->saveRENCLK_GATE_D1);

   if (IS_I965G(pI830)) {
      OUTREG(RENCLK_GATE_D2, pI830->saveRENCLK_GATE_D2);
      OUTREG(RAMCLK_GATE_D, pI830->saveRAMCLK_GATE_D);
   }

   if (IS_I965GM(pI830) || IS_GM45(pI830))
      OUTREG(PWRCTXA, pI830->savePWRCTXA);

   /*
    * Pipe regs
    * To restore the saved state, we first need to program the PLL regs,
    * followed by the pipe configuration and finally the display plane
    * configuration.  The VGA registers can program one, both or neither
    * of the PLL regs, depending on their VGA_MOD_DIS bit value.
    */

   /*
    * Since either or both pipes may use the VGA clocks, make sure the
    * regs are valid.
    */
   OUTREG(VCLK_DIVISOR_VGA0, pI830->saveVCLK_DIVISOR_VGA0);
   OUTREG(VCLK_DIVISOR_VGA1, pI830->saveVCLK_DIVISOR_VGA1);
   OUTREG(VCLK_POST_DIV, pI830->saveVCLK_POST_DIV);

   /* If the pipe A PLL is active, we can restore the pipe & plane config */
   if (pI830->saveDPLL_A & DPLL_VCO_ENABLE)
   {
      OUTREG(DPLL_A, pI830->saveDPLL_A & ~DPLL_VCO_ENABLE);
      usleep(150);
   }
   OUTREG(FPA0, pI830->saveFPA0);
   OUTREG(FPA1, pI830->saveFPA1);
   OUTREG(DPLL_A, pI830->saveDPLL_A);
   i830_dpll_settle();
   if (IS_I965G(pI830))
      OUTREG(DPLL_A_MD, pI830->saveDPLL_A_MD);
   else
      OUTREG(DPLL_A, pI830->saveDPLL_A);
   i830_dpll_settle();

   /* Restore mode config */
   OUTREG(HTOTAL_A, pI830->saveHTOTAL_A);
   OUTREG(HBLANK_A, pI830->saveHBLANK_A);
   OUTREG(HSYNC_A, pI830->saveHSYNC_A);
   OUTREG(VTOTAL_A, pI830->saveVTOTAL_A);
   OUTREG(VBLANK_A, pI830->saveVBLANK_A);
   OUTREG(VSYNC_A, pI830->saveVSYNC_A);
   OUTREG(BCLRPAT_A, pI830->saveBCLRPAT_A);
   
   OUTREG(DSPASTRIDE, pI830->saveDSPASTRIDE);
   OUTREG(DSPASIZE, pI830->saveDSPASIZE);
   OUTREG(DSPAPOS, pI830->saveDSPAPOS);
   OUTREG(PIPEASRC, pI830->savePIPEASRC);
   OUTREG(DSPABASE, pI830->saveDSPABASE);
   if (IS_I965G(pI830))
   {
      OUTREG(DSPASURF, pI830->saveDSPASURF);
      OUTREG(DSPATILEOFF, pI830->saveDSPATILEOFF);
   }

   OUTREG(PIPEACONF, pI830->savePIPEACONF);
   i830WaitForVblank(pScrn);

   /*
    * Program Pipe A's plane
    * The corresponding display plane may be disabled, and should only be
    * enabled if pipe A is actually on (otherwise we have a bug in the initial
    * state).
    */
   if ((pI830->saveDSPACNTR & DISPPLANE_SEL_PIPE_MASK) ==
       DISPPLANE_SEL_PIPE_A) {
       OUTREG(DSPACNTR, pI830->saveDSPACNTR);
       OUTREG(DSPABASE, INREG(DSPABASE));
       i830WaitForVblank(pScrn);
   }
   if ((pI830->saveDSPBCNTR & DISPPLANE_SEL_PIPE_MASK) ==
       DISPPLANE_SEL_PIPE_A) {
       OUTREG(DSPBCNTR, pI830->saveDSPBCNTR);
       OUTREG(DSPBBASE, INREG(DSPBBASE));
       i830WaitForVblank(pScrn);
   }

   /* See note about pipe programming above */
   if(xf86_config->num_crtc == 2) 
   {
      /* If the pipe B PLL is active, we can restore the pipe & plane config */
      if (pI830->saveDPLL_B & DPLL_VCO_ENABLE)
      {
	 OUTREG(DPLL_B, pI830->saveDPLL_B & ~DPLL_VCO_ENABLE);
	 usleep(150);
      }
      OUTREG(FPB0, pI830->saveFPB0);
      OUTREG(FPB1, pI830->saveFPB1);
      OUTREG(DPLL_B, pI830->saveDPLL_B);
      i830_dpll_settle();
      if (IS_I965G(pI830))
	 OUTREG(DPLL_B_MD, pI830->saveDPLL_B_MD);
      else
	 OUTREG(DPLL_B, pI830->saveDPLL_B);
      i830_dpll_settle();
   
      /* Restore mode config */
      OUTREG(HTOTAL_B, pI830->saveHTOTAL_B);
      OUTREG(HBLANK_B, pI830->saveHBLANK_B);
      OUTREG(HSYNC_B, pI830->saveHSYNC_B);
      OUTREG(VTOTAL_B, pI830->saveVTOTAL_B);
      OUTREG(VBLANK_B, pI830->saveVBLANK_B);
      OUTREG(VSYNC_B, pI830->saveVSYNC_B);
      OUTREG(BCLRPAT_B, pI830->saveBCLRPAT_B);
      OUTREG(DSPBSTRIDE, pI830->saveDSPBSTRIDE);
      OUTREG(DSPBSIZE, pI830->saveDSPBSIZE);
      OUTREG(DSPBPOS, pI830->saveDSPBPOS);
      OUTREG(PIPEBSRC, pI830->savePIPEBSRC);
      OUTREG(DSPBBASE, pI830->saveDSPBBASE);
      if (IS_I965G(pI830))
      {
	 OUTREG(DSPBSURF, pI830->saveDSPBSURF);
	 OUTREG(DSPBTILEOFF, pI830->saveDSPBTILEOFF);
      }

      OUTREG(PIPEBCONF, pI830->savePIPEBCONF);
      i830WaitForVblank(pScrn);

      /*
       * Program Pipe B's plane
       * Note that pipe B may be disabled, and in that case, the plane
       * should also be disabled or we must have had a bad initial state.
       */
      if ((pI830->saveDSPACNTR & DISPPLANE_SEL_PIPE_MASK) ==
	  DISPPLANE_SEL_PIPE_B) {
	  OUTREG(DSPACNTR, pI830->saveDSPACNTR);
	  OUTREG(DSPABASE, INREG(DSPABASE));
	  i830WaitForVblank(pScrn);
      }
      if ((pI830->saveDSPBCNTR & DISPPLANE_SEL_PIPE_MASK) ==
	  DISPPLANE_SEL_PIPE_B) {
	  OUTREG(DSPBCNTR, pI830->saveDSPBCNTR);
	  OUTREG(DSPBBASE, INREG(DSPBBASE));
	  i830WaitForVblank(pScrn);
      }
   }

   OUTREG(VGACNTRL, pI830->saveVGACNTRL);

   /*
    * Restore cursors
    * Even though the X cursor is hidden before we restore the hw state,
    * we probably only disabled one cursor plane.  If we're going from
    * e.g. plane b to plane a here in RestoreHWState, we need to restore
    * both cursor plane settings.
    */
   OUTREG(CURSOR_A_POSITION, pI830->saveCURSOR_A_POSITION);
   OUTREG(CURSOR_A_BASE, pI830->saveCURSOR_A_BASE);
   OUTREG(CURSOR_A_CONTROL, pI830->saveCURSOR_A_CONTROL);
   OUTREG(CURSOR_B_POSITION, pI830->saveCURSOR_B_POSITION);
   OUTREG(CURSOR_B_BASE, pI830->saveCURSOR_B_BASE);
   OUTREG(CURSOR_B_CONTROL, pI830->saveCURSOR_B_CONTROL);

   /* Restore outputs */
   for (i = 0; i < xf86_config->num_output; i++) {
      xf86OutputPtr   output = xf86_config->output[i];
      if (output->funcs->restore)
	 output->funcs->restore(output);
   }
    
   i830_restore_palette(pI830, PIPE_A);
   i830_restore_palette(pI830, PIPE_B);

   for(i = 0; i < 7; i++) {
      OUTREG(SWF0 + (i << 2), pI830->saveSWF[i]);
      OUTREG(SWF00 + (i << 2), pI830->saveSWF[i+7]);
   }

   OUTREG(SWF30, pI830->saveSWF[14]);
   OUTREG(SWF31, pI830->saveSWF[15]);
   OUTREG(SWF32, pI830->saveSWF[16]);

   if (pI830->fb_compression) {
       OUTREG(FBC_CFB_BASE, pI830->saveFBC_CFB_BASE);
       OUTREG(FBC_LL_BASE, pI830->saveFBC_LL_BASE);
       OUTREG(FBC_FENCE_OFF, pI830->saveFBC_FENCE_OFF);
       OUTREG(FBC_CONTROL2, pI830->saveFBC_CONTROL2);
       OUTREG(FBC_CONTROL, pI830->saveFBC_CONTROL);
   }

   /* Clear any FIFO underrun status that may have occurred normally */
   OUTREG(PIPEASTAT, INREG(PIPEASTAT) | FIFO_UNDERRUN);
   OUTREG(PIPEBSTAT, INREG(PIPEBSTAT) | FIFO_UNDERRUN);

   vgaHWRestore(pScrn, vgaReg, VGA_SR_FONTS);
   vgaHWLock(hwp);

   return TRUE;
}

static void
I830PointerMoved(int index, int x, int y)
{
   ScrnInfoPtr pScrn = xf86Screens[index];
   I830Ptr pI830 = I830PTR(pScrn);
   int newX = x, newY = y;

   switch (pI830->rotation) {
      case RR_Rotate_0:
         break;
      case RR_Rotate_90:
         newX = y;
         newY = pScrn->pScreen->width - x - 1;
         break;
      case RR_Rotate_180:
         newX = pScrn->pScreen->width - x - 1;
         newY = pScrn->pScreen->height - y - 1;
         break;
      case RR_Rotate_270:
         newX = pScrn->pScreen->height - y - 1;
         newY = x;
         break;
   }

   (*pI830->PointerMoved)(index, newX, newY);
}

static Bool
I830InitFBManager(
    ScreenPtr pScreen,  
    BoxPtr FullBox
){
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   RegionRec ScreenRegion;
   RegionRec FullRegion;
   BoxRec ScreenBox;
   Bool ret;

   ScreenBox.x1 = 0;
   ScreenBox.y1 = 0;
   ScreenBox.x2 = pScrn->displayWidth;
   if (pScrn->virtualX > pScrn->virtualY)
      ScreenBox.y2 = pScrn->virtualX;
   else
      ScreenBox.y2 = pScrn->virtualY;

   if((FullBox->x1 >  ScreenBox.x1) || (FullBox->y1 >  ScreenBox.y1) ||
      (FullBox->x2 <  ScreenBox.x2) || (FullBox->y2 <  ScreenBox.y2)) {
	return FALSE;   
   }

   if (FullBox->y2 < FullBox->y1) return FALSE;
   if (FullBox->x2 < FullBox->x2) return FALSE;

   REGION_INIT(pScreen, &ScreenRegion, &ScreenBox, 1); 
   REGION_INIT(pScreen, &FullRegion, FullBox, 1); 

   REGION_SUBTRACT(pScreen, &FullRegion, &FullRegion, &ScreenRegion);

   ret = xf86InitFBManagerRegion(pScreen, &FullRegion);

   REGION_UNINIT(pScreen, &ScreenRegion);
   REGION_UNINIT(pScreen, &FullRegion);
    
   return ret;
}

/**
 * Intialiazes the hardware for the 3D pipeline use in the 2D driver.
 *
 * Some state caching is performed to avoid redundant state emits.  This
 * function is also responsible for marking the state as clobbered for DRI
 * clients.
 */
void
IntelEmitInvarientState(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   uint32_t ctx_addr;

   if (pI830->noAccel)
      return;

#ifdef XF86DRI
   if (pI830->directRenderingEnabled) {
      drmI830Sarea *sarea = DRIGetSAREAPrivate(pScrn->pScreen);

      /* Mark that the X Server was the last holder of the context */
      if (sarea)
	 sarea->ctxOwner = DRIGetContext(pScrn->pScreen);
   }
#endif

   /* If we've emitted our state since the last clobber by another client,
    * skip it.
    */
   if (*pI830->last_3d != LAST_3D_OTHER)
      return;

   ctx_addr = pI830->logical_context->offset;
   assert((pI830->logical_context->offset & 2047) == 0);
   {
      BEGIN_BATCH(2);
      OUT_BATCH(MI_SET_CONTEXT);
      OUT_BATCH(pI830->logical_context->offset |
		CTXT_NO_RESTORE |
		CTXT_PALETTE_SAVE_DISABLE | CTXT_PALETTE_RESTORE_DISABLE);
      ADVANCE_BATCH();
   }

   if (!IS_I965G(pI830))
   {
      if (IS_I9XX(pI830))
         I915EmitInvarientState(pScrn);
      else
         I830EmitInvarientState(pScrn);
   }
}

static void
I830BlockHandler(int i,
		 pointer blockData, pointer pTimeout, pointer pReadmask)
{
    ScreenPtr pScreen = screenInfo.screens[i];
    ScrnInfoPtr pScrn = xf86Screens[i];
    I830Ptr pI830 = I830PTR(pScrn);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);

    pScreen->BlockHandler = pI830->BlockHandler;

    (*pScreen->BlockHandler) (i, blockData, pTimeout, pReadmask);

    pI830->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = I830BlockHandler;

    /* Emit a flush of the rendering cache, or on the 965 and beyond
     * rendering results may not hit the framebuffer until significantly
     * later.  In the direct rendering case this is already done just
     * after the page flipping updates, so there's no need to duplicate
     * the effort here.
     */
    if (pScrn->vtSema && !pI830->noAccel && !pI830->directRenderingEnabled)
	I830EmitFlush(pScrn);

    /*
     * Check for FIFO underruns at block time (which amounts to just
     * periodically).  If this happens, it means our DSPARB or some other
     * memory arbitration setting is wrong for the current configuration
     * (except for mode setting, where it may occur naturally).
     * Check & ack the condition.
     */
    if (pScrn->vtSema && !DSPARB_HWCONTROL(pI830)) {
	if (xf86_config->crtc[0]->enabled &&
		(INREG(PIPEASTAT) & FIFO_UNDERRUN)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "underrun on pipe A!\n");
	    OUTREG(PIPEASTAT, INREG(PIPEASTAT) | FIFO_UNDERRUN);
	}
	if (xf86_config->num_crtc > 1 &&
		xf86_config->crtc[1]->enabled &&
		(INREG(PIPEBSTAT) & FIFO_UNDERRUN)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "underrun on pipe B!\n");
	    OUTREG(PIPEBSTAT, INREG(PIPEBSTAT) | FIFO_UNDERRUN);
	}
    }

    I830VideoBlockHandler(i, blockData, pTimeout, pReadmask);
}

static void
i830_fixup_mtrrs(ScrnInfoPtr pScrn)
{
#ifdef HAS_MTRR_SUPPORT
    I830Ptr pI830 = I830PTR(pScrn);
    int fd;
    struct mtrr_gentry gentry;
    struct mtrr_sentry sentry;

    if ( ( fd = open ("/proc/mtrr", O_RDONLY, 0) ) != -1 ) {
	for (gentry.regnum = 0; ioctl (fd, MTRRIOC_GET_ENTRY, &gentry) == 0;
		++gentry.regnum) {

	    if (gentry.size < 1) {
		/* DISABLED */
		continue;
	    }

	    /* Check the MTRR range is one we like and if not - remove it.
	     * The Xserver common layer will then setup the right range
	     * for us.
	     */
	    if (gentry.base == pI830->LinearAddr && 
		    gentry.size < pI830->FbMapSize) {

		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"Removing bad MTRR range (base 0x%lx, size 0x%x)\n",
			gentry.base, gentry.size);

		sentry.base = gentry.base;
		sentry.size = gentry.size;
		sentry.type = gentry.type;

		if (ioctl (fd, MTRRIOC_DEL_ENTRY, &sentry) == -1) {
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			    "Failed to remove bad MTRR range\n");
		}
	    }
	}
	close(fd);
    }
#endif
}

static Bool
i830_try_memory_allocation(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    Bool tiled = pI830->tiling;
    Bool dri = pI830->directRenderingEnabled;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	    "Attempting memory allocation with %stiled buffers.\n",
	    tiled ? "" : "un");

    if (!i830_allocate_2d_memory(pScrn))
	goto failed;

    if (IS_I965GM(pI830) || IS_GM45(pI830))
	if (!i830_allocate_pwrctx(pScrn))
	    goto failed;

    if (dri && !i830_allocate_3d_memory(pScrn))
	goto failed;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%siled allocation successful.\n",
	    tiled ? "T" : "Unt");
    return TRUE;

failed:
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%siled allocation failed.\n",
	    tiled ? "T" : "Unt");
    return FALSE;
}

/*
 * Try to allocate memory in several ways:
 *  1) If direct rendering is enabled, try to allocate enough memory for tiled
 *     surfaces by rounding up the display width to a tileable one.
 *  2) If that fails or the allocations themselves fail, try again with untiled
 *     allocations (if this works DRI will stay enabled).
 *  3) And if all else fails, disable DRI and try just 2D allocations.
 *  4) Give up and fail ScreenInit.
 */
static Bool
i830_memory_init(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    int savedDisplayWidth = pScrn->displayWidth;
    int i;
    Bool tiled = FALSE;

    /*
     * Adjust the display width to allow for front buffer tiling if possible
     */
    if (pI830->tiling) {
	if (IS_I965G(pI830)) {
	    int tile_pixels = 512 / pI830->cpp;
	    pScrn->displayWidth = (pScrn->displayWidth + tile_pixels - 1) &
		~(tile_pixels - 1);
	    tiled = TRUE;
	} else {
	    /* Good pitches to allow tiling.  Don't care about pitches < 1024
	     * pixels.
	     */
	    static const int pitches[] = {
		1024,
		2048,
		4096,
		8192,
		0
	    };

	    for (i = 0; pitches[i] != 0; i++) {
		if (pitches[i] >= pScrn->displayWidth) {
		    pScrn->displayWidth = pitches[i];
		    tiled = TRUE;
		    break;
		}
	    }
	}
    }
    /* Set up our video memory allocator for the chosen videoRam */
    if (!i830_allocator_init(pScrn, 0, pScrn->videoRam * KB(1))) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"Couldn't initialize video memory allocator\n");
	PreInitCleanup(pScrn);
	return FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex,
	    pI830->pEnt->device->videoRam ? X_CONFIG : X_DEFAULT,
	    "VideoRam: %d KB\n", pScrn->videoRam);

    if (xf86GetOptValInteger(pI830->Options, OPTION_CACHE_LINES,
		&(pI830->CacheLines))) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Requested %d cache lines\n",
		pI830->CacheLines);
    } else {
	pI830->CacheLines = -1;
    }

    /* Tiled first if we got a good displayWidth */
    if (tiled) {
	if (i830_try_memory_allocation(pScrn))
	    return TRUE;
	else {
	    i830_reset_allocations(pScrn);
	    pI830->tiling = FALSE;
	}
    }

    /* If tiling fails we have to disable page flipping & FBC */
    pScrn->displayWidth = savedDisplayWidth;
    if (pI830->allowPageFlip)
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"Couldn't allocate tiled memory, page flipping "
		"disabled\n");
    pI830->allowPageFlip = FALSE;
    if (pI830->fb_compression)
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"Couldn't allocate tiled memory, fb compression "
		"disabled\n");
    pI830->fb_compression = FALSE;

    /* Try again, but leave DRI enabled */
    if (pI830->directRenderingEnabled) {
	if (i830_try_memory_allocation(pScrn))
	    return TRUE;
	else {
	    i830_reset_allocations(pScrn);
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Couldn't allocate 3D memory, "
		    "disabling DRI.\n");
	    pI830->directRenderingEnabled = FALSE;
	}
    }

    if (i830_try_memory_allocation(pScrn))
	return TRUE;

    return FALSE;
}

static void
i830_disable_render_standby(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   uint32_t render_standby;

   /* Render Standby might cause hang issue, try always disable it.*/
   if (IS_I965GM(pI830) || IS_GM45(pI830)) {
       render_standby = INREG(MCHBAR_RENDER_STANDBY);
       if (render_standby & RENDER_STANDBY_ENABLE) {
	   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Disable render standby.\n");
	   OUTREG(MCHBAR_RENDER_STANDBY,
		   (render_standby & (~RENDER_STANDBY_ENABLE)));
       }
   }
}

static Bool
I830ScreenInit(int scrnIndex, ScreenPtr pScreen, int argc, char **argv)
{
   ScrnInfoPtr pScrn;
   vgaHWPtr hwp;
   I830Ptr pI830;
   VisualPtr visual;
   I830Ptr pI8301 = NULL;
   unsigned long sys_mem;
   int c;
   MessageType from;
#ifdef XF86DRI
   xf86CrtcConfigPtr config;
#endif

   pScrn = xf86Screens[pScreen->myNum];
   pI830 = I830PTR(pScrn);
   hwp = VGAHWPTR(pScrn);

   pScrn->displayWidth = (pScrn->virtualX + 63) & ~63;

   /*
    * The "VideoRam" config file parameter specifies the maximum amount of
    * memory that will be used/allocated.  When not present, we allow the
    * driver to allocate as much memory as it wishes to satisfy its
    * allocations, but if agpgart support isn't available, it gets limited
    * to the amount of pre-allocated ("stolen") memory.
    *
    * Note that in using this value for allocator initialization, we're
    * limiting aperture allocation to the VideoRam option, rather than limiting
    * actual memory allocation, so alignment and things will cause less than
    * VideoRam to be actually used.
    */
   if (pI830->pEnt->device->videoRam == 0) {
      from = X_DEFAULT;
      pScrn->videoRam = pI830->FbMapSize / KB(1);
   } else {
#if 0
      from = X_CONFIG;
      pScrn->videoRam = pI830->pEnt->device->videoRam;
#else
      /* Disable VideoRam configuration, at least for now.  Previously,
       * VideoRam was necessary to avoid overly low limits on allocated
       * memory, so users created larger, yet still small, fixed allocation
       * limits in their config files.  Now, the driver wants to allocate more,
       * and the old intention of the VideoRam lines that had been entered is
       * obsolete.
       */
      from = X_DEFAULT;
      pScrn->videoRam = pI830->FbMapSize / KB(1);

      if (pScrn->videoRam != pI830->pEnt->device->videoRam) {
	 xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		    "VideoRam configuration found, which is no longer "
		    "recommended.\n");
	 xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "Continuing with default %dkB VideoRam instead of %d "
		    "kB.\n",
		    pScrn->videoRam, pI830->pEnt->device->videoRam);
      }
#endif
   }

   /* Limit videoRam to how much we might be able to allocate from AGP */
   sys_mem = I830CheckAvailableMemory(pScrn);
   if (sys_mem == -1) {
      if (pScrn->videoRam > pI830->stolen_size / KB(1)) {
	 xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		    "/dev/agpgart is either not available, or no memory "
		    "is available\nfor allocation.  "
		    "Using pre-allocated memory only.\n");
	 pScrn->videoRam = pI830->stolen_size / KB(1);
      }
      pI830->StolenOnly = TRUE;
   } else {
      if (sys_mem + (pI830->stolen_size / 1024) < pScrn->videoRam) {
	 pScrn->videoRam = sys_mem + (pI830->stolen_size / 1024);
	 from = X_PROBED;
	 if (sys_mem + (pI830->stolen_size / 1024) <
	     pI830->pEnt->device->videoRam)
	 {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "VideoRAM reduced to %d kByte "
		       "(limited to available sysmem)\n", pScrn->videoRam);
	 }
      }
   }

   /* Limit video RAM to the actual aperture size */
   if (pScrn->videoRam > pI830->FbMapSize / 1024) {
      pScrn->videoRam = pI830->FbMapSize / 1024;
      if (pI830->FbMapSize / 1024 < pI830->pEnt->device->videoRam) {
	 xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		    "VideoRam reduced to %d kByte (limited to aperture "
		    "size)\n",
		    pScrn->videoRam);
      }
   }

   /* Make sure it's on a page boundary */
   if (pScrn->videoRam & 3) {
      xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "VideoRam reduced to %d KB "
		 "(page aligned - was %d KB)\n",
		 pScrn->videoRam & ~3, pScrn->videoRam);
      pScrn->videoRam &= ~3;
   }

   if (!IS_I965G(pI830) && pScrn->displayWidth > 2048) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "Cannot support DRI with frame buffer width > 2048.\n");
      pI830->directRenderingDisabled = TRUE;
   }

#ifdef XF86DRI
   /* If DRI hasn't been explicitly disabled, try to initialize it.
    * It will be used by the memory allocator.
    */
   if (!pI830->directRenderingDisabled)
      pI830->directRenderingEnabled = I830DRIScreenInit(pScreen);
   else
      pI830->directRenderingEnabled = FALSE;
#else
   pI830->directRenderingEnabled = FALSE;
#endif

   /* Enable tiling by default */
   pI830->tiling = TRUE;

   /* Allow user override if they set a value */
   if (xf86IsOptionSet(pI830->Options, OPTION_TILING)) {
       if (xf86ReturnOptValBool(pI830->Options, OPTION_TILING, FALSE))
	   pI830->tiling = TRUE;
       else
	   pI830->tiling = FALSE;
   }

   /* Enable FB compression if possible */
   if (i830_fb_compression_supported(pI830))
       pI830->fb_compression = TRUE;
   else
       pI830->fb_compression = FALSE;

   /* Again, allow user override if set */
   if (xf86IsOptionSet(pI830->Options, OPTION_FBC)) {
       if (xf86ReturnOptValBool(pI830->Options, OPTION_FBC, FALSE))
	   pI830->fb_compression = TRUE;
       else
	   pI830->fb_compression = FALSE;
   }

   xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Framebuffer compression %sabled\n",
	      pI830->fb_compression ? "en" : "dis");
   xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Tiling %sabled\n", pI830->tiling ?
	      "en" : "dis");

   if (I830IsPrimary(pScrn)) {
      /* Alloc our pointers for the primary head */
      if (!pI830->LpRing)
         pI830->LpRing = xcalloc(1, sizeof(I830RingBuffer));
      if (!pI830->overlayOn)
         pI830->overlayOn = xalloc(sizeof(Bool));
      if (!pI830->last_3d)
         pI830->last_3d = xalloc(sizeof(enum last_3d));
      if (!pI830->LpRing || !pI830->overlayOn || !pI830->last_3d) {
         xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "Could not allocate primary data structures.\n");
         return FALSE;
      }
      *pI830->last_3d = LAST_3D_OTHER;
      *pI830->overlayOn = FALSE;
      if (pI830->entityPrivate)
         pI830->entityPrivate->XvInUse = -1;
   } else {
      /* Make our second head point to the first heads structures */
      pI8301 = I830PTR(pI830->entityPrivate->pScrn_1);
      pI830->LpRing = pI8301->LpRing;
      pI830->overlay_regs = pI8301->overlay_regs;
      pI830->overlayOn = pI8301->overlayOn;
      pI830->last_3d = pI8301->last_3d;
   }

#ifdef I830_XV
    /*
     * Set this so that the overlay allocation is factored in when
     * appropriate.
     */
    pI830->XvEnabled = !pI830->XvDisabled;
#endif


   /* Need MMIO mapped to do GTT lookups during memory allocation. */
   I830MapMMIO(pScrn);

   if (!i830_memory_init(pScrn)) {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	       "Couldn't allocate video memory\n");
       return FALSE;
   }

   I830UnmapMMIO(pScrn);

   i830_fixup_mtrrs(pScrn);

   pI830->starting = TRUE;

   miClearVisualTypes();
   if (!miSetVisualTypes(pScrn->depth,
			    miGetDefaultVisualMask(pScrn->depth),
			    pScrn->rgbBits, pScrn->defaultVisual))
	 return FALSE;
   if (!miSetPixmapDepths())
      return FALSE;

#ifdef I830_XV
   pI830->XvEnabled = !pI830->XvDisabled;
   if (pI830->XvEnabled) {
      if (!I830IsPrimary(pScrn)) {
         if (!pI8301->XvEnabled || pI830->noAccel) {
            pI830->XvEnabled = FALSE;
	    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Xv is disabled.\n");
         }
      } else
      if (pI830->noAccel || pI830->StolenOnly) {
	 xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Xv is disabled because it "
		    "needs 2D accel and AGPGART.\n");
	 pI830->XvEnabled = FALSE;
      }
   }
#else
   pI830->XvEnabled = FALSE;
#endif

   if (!pI830->noAccel) {
      if (pI830->LpRing->mem->size == 0) {
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		     "Disabling acceleration because the ring buffer "
		      "allocation failed.\n");
	   pI830->noAccel = TRUE;
      }
   }

#ifdef I830_XV
   if (pI830->XvEnabled) {
      if (pI830->noAccel) {
	 xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Disabling Xv because it "
		    "needs 2D acceleration.\n");
	 pI830->XvEnabled = FALSE;
      }
      if (!OVERLAY_NOEXIST(pI830) && pI830->overlay_regs == NULL) {
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		     "Disabling Xv because the overlay register buffer "
		      "allocation failed.\n");
	 pI830->XvEnabled = FALSE;
      }
   }
#endif

#ifdef XF86DRI
   /*
    * Setup DRI after visuals have been established, but before fbScreenInit
    * is called.   fbScreenInit will eventually call into the drivers
    * InitGLXVisuals call back.
    */

   if (pI830->directRenderingEnabled) {
      if (pI830->noAccel || pI830->SWCursor || (pI830->StolenOnly && I830IsPrimary(pScrn))) {
	 xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "DRI is disabled because it "
		    "needs HW cursor, 2D accel and AGPGART.\n");
	 pI830->directRenderingEnabled = FALSE;
      }
   }

   if (pI830->directRenderingEnabled)
      pI830->directRenderingEnabled = I830DRIDoMappings(pScreen);

   /* If we failed for any reason, free DRI memory. */
   if (!pI830->directRenderingEnabled)
      i830_free_3d_memory(pScrn);

   config = XF86_CRTC_CONFIG_PTR(pScrn);

   /*
    * If an LVDS display is present, swap the plane/pipe mappings so we can
    * use FBC on the builtin display.
    * Note: 965+ chips can compress either plane, so we leave the mapping
    *       alone in that case.
    * Also make sure the DRM can handle the swap.
    */
   if (I830LVDSPresent(pScrn) && !IS_I965GM(pI830) && !IS_GM45(pI830) &&
       (!pI830->directRenderingEnabled ||
	(pI830->directRenderingEnabled && pI830->drmMinor >= 10))) {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "adjusting plane->pipe mappings "
		  "to allow for framebuffer compression\n");
       for (c = 0; c < config->num_crtc; c++) {
	   xf86CrtcPtr	      crtc = config->crtc[c];
	   I830CrtcPrivatePtr   intel_crtc = crtc->driver_private;

	   if (intel_crtc->pipe == 0)
	       intel_crtc->plane = 1;
	   else if (intel_crtc->pipe == 1)
	       intel_crtc->plane = 0;
      }
   }

#else
   pI830->directRenderingEnabled = FALSE;
#endif

#ifdef XF86DRI

   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Page Flipping %sabled\n",
	      pI830->allowPageFlip ? "en" : "dis");
#endif

   DPRINTF(PFX, "assert( if(!I830MapMem(pScrn)) )\n");
   if (!I830MapMem(pScrn))
      return FALSE;

   pScrn->memPhysBase = (unsigned long)pI830->FbBase;

   if (I830IsPrimary(pScrn)) {
      pScrn->fbOffset = pI830->front_buffer->offset;
   } else {
      pScrn->fbOffset = pI8301->front_buffer_2->offset;
   }

   pI830->xoffset = (pScrn->fbOffset / pI830->cpp) % pScrn->displayWidth;
   pI830->yoffset = (pScrn->fbOffset / pI830->cpp) / pScrn->displayWidth;

   vgaHWSetMmioFuncs(hwp, pI830->MMIOBase, 0);
   vgaHWGetIOBase(hwp);
   DPRINTF(PFX, "assert( if(!vgaHWMapMem(pScrn)) )\n");
   if (!vgaHWMapMem(pScrn))
      return FALSE;

   i830_disable_render_standby(pScrn);

   DPRINTF(PFX, "assert( if(!I830EnterVT(scrnIndex, 0)) )\n");

   if (!pI830->useEXA) {
      if (I830IsPrimary(pScrn)) {
	 if (!I830InitFBManager(pScreen, &(pI830->FbMemBox))) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Failed to init memory manager\n");
	 }
      } else {
	 if (!I830InitFBManager(pScreen, &(pI8301->FbMemBox2))) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Failed to init memory manager\n");
	 }
      }
   }

    if (pScrn->virtualX > pScrn->displayWidth)
	pScrn->displayWidth = pScrn->virtualX;

   DPRINTF(PFX, "assert( if(!fbScreenInit(pScreen, ...) )\n");
   if (!fbScreenInit(pScreen, pI830->FbBase + pScrn->fbOffset, 
                     pScrn->virtualX, pScrn->virtualY,
		     pScrn->xDpi, pScrn->yDpi,
		     pScrn->displayWidth, pScrn->bitsPerPixel))
      return FALSE;

   if (pScrn->bitsPerPixel > 8) {
      /* Fixup RGB ordering */
      visual = pScreen->visuals + pScreen->numVisuals;
      while (--visual >= pScreen->visuals) {
	 if ((visual->class | DynamicClass) == DirectColor) {
	    visual->offsetRed = pScrn->offset.red;
	    visual->offsetGreen = pScrn->offset.green;
	    visual->offsetBlue = pScrn->offset.blue;
	    visual->redMask = pScrn->mask.red;
	    visual->greenMask = pScrn->mask.green;
	    visual->blueMask = pScrn->mask.blue;
	 }
      }
   }

   fbPictureInit(pScreen, NULL, 0);

   xf86SetBlackWhitePixels(pScreen);

   xf86DiDGAInit (pScreen, pI830->LinearAddr + pScrn->fbOffset);

   DPRINTF(PFX,
	   "assert( if(!I830InitFBManager(pScreen, &(pI830->FbMemBox))) )\n");

   if (!pI830->noAccel) {
      if (!I830AccelInit(pScreen)) {
	 xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "Hardware acceleration initialization failed\n");
      }
   }

   miInitializeBackingStore(pScreen);
   xf86SetBackingStore(pScreen);
   xf86SetSilkenMouse(pScreen);
   miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

   if (!pI830->SWCursor) {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Initializing HW Cursor\n");
      if (!I830CursorInit(pScreen))
	 xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "Hardware cursor initialization failed\n");
   } else
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Initializing SW Cursor!\n");

#ifdef XF86DRI
   /* Must be called before EnterVT, so we can acquire the DRI lock when
    * binding our memory.
    */
   if (pI830->directRenderingEnabled)
      pI830->directRenderingEnabled = I830DRIFinishScreenInit(pScreen);
#endif

   /* Must force it before EnterVT, so we are in control of VT and
    * later memory should be bound when allocating, e.g rotate_mem */
   pScrn->vtSema = TRUE;

   if (!I830EnterVT(scrnIndex, 0))
      return FALSE;

   pI830->BlockHandler = pScreen->BlockHandler;
   pScreen->BlockHandler = I830BlockHandler;

   pScreen->SaveScreen = xf86SaveScreen;
   pI830->CloseScreen = pScreen->CloseScreen;
   pScreen->CloseScreen = I830CloseScreen;
   pI830->CreateScreenResources = pScreen->CreateScreenResources;
   pScreen->CreateScreenResources = i830CreateScreenResources;

   if (!xf86CrtcScreenInit (pScreen))
       return FALSE;

   DPRINTF(PFX, "assert( if(!miCreateDefColormap(pScreen)) )\n");
   if (!miCreateDefColormap(pScreen))
      return FALSE;

   DPRINTF(PFX, "assert( if(!xf86HandleColormaps(pScreen, ...)) )\n");
   if (!xf86HandleColormaps(pScreen, 256, 8, I830LoadPalette, NULL,
			    CMAP_RELOAD_ON_MODE_SWITCH |
			    CMAP_PALETTED_TRUECOLOR)) {
      return FALSE;
   }

   xf86DPMSInit(pScreen, xf86DPMSSet, 0);

#ifdef I830_XV
   /* Init video */
   if (pI830->XvEnabled)
      I830InitVideo(pScreen);
#endif

   /* Setup 3D engine, needed for rotation too */
   IntelEmitInvarientState(pScrn);

#ifdef XF86DRI
   if (pI830->directRenderingEnabled) {
      pI830->directRenderingOpen = TRUE;
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "direct rendering: Enabled\n");
   } else {
      if (pI830->directRenderingDisabled)
	 xf86DrvMsg(pScrn->scrnIndex, X_INFO, "direct rendering: Disabled\n");
      else
	 xf86DrvMsg(pScrn->scrnIndex, X_INFO, "direct rendering: Failed\n");
   }
#else
   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "direct rendering: Not available\n");
#endif


   /* Wrap pointer motion to flip touch screen around */
   pI830->PointerMoved = pScrn->PointerMoved;
   pScrn->PointerMoved = I830PointerMoved;

   if (serverGeneration == 1)
      xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);

   pI830->starting = FALSE;
   pI830->closing = FALSE;
   pI830->suspended = FALSE;

   return TRUE;
}

static void
i830AdjustFrame(int scrnIndex, int x, int y, int flags)
{
   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
   xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR(pScrn);
   I830Ptr pI830 = I830PTR(pScrn);
   xf86OutputPtr  output = config->output[config->compat_output];
   xf86CrtcPtr	crtc = output->crtc;

   DPRINTF(PFX, "i830AdjustFrame: y = %d (+ %d), x = %d (+ %d)\n",
	   x, pI830->xoffset, y, pI830->yoffset);

   if (crtc && crtc->enabled)
   {
      /* Sync the engine before adjust frame */
      i830WaitSync(pScrn);
      i830PipeSetBase(crtc, crtc->desiredX + x, crtc->desiredY + y);
      crtc->x = output->initial_x + x;
      crtc->y = output->initial_y + y;
   }
}

static void
I830FreeScreen(int scrnIndex, int flags)
{
#ifdef INTEL_XVMC
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    I830Ptr pI830 = I830PTR(pScrn);
    if (pI830 && pI830->XvMCEnabled)
	intel_xvmc_finish(xf86Screens[scrnIndex]);
#endif
   I830FreeRec(xf86Screens[scrnIndex]);
   if (xf86LoaderCheckSymbol("vgaHWFreeHWRec"))
      vgaHWFreeHWRec(xf86Screens[scrnIndex]);
}

static void
I830LeaveVT(int scrnIndex, int flags)
{
   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
   I830Ptr pI830 = I830PTR(pScrn);
#ifndef HAVE_FREE_SHADOW
   xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
   int o;
#endif

   DPRINTF(PFX, "Leave VT\n");

   pI830->leaving = TRUE;

   if (pI830->devicesTimer)
      TimerCancel(pI830->devicesTimer);
   pI830->devicesTimer = NULL;

   i830SetHotkeyControl(pScrn, HOTKEY_BIOS_SWITCH);

   if (!I830IsPrimary(pScrn)) {
   	I830Ptr pI8301 = I830PTR(pI830->entityPrivate->pScrn_1);
	if (!pI8301->gtt_acquired) {
		return;
	}
   }

#ifdef XF86DRI
   if (pI830->directRenderingOpen) {
      DRILock(screenInfo.screens[pScrn->scrnIndex], 0);

      I830DRISetVBlankInterrupt (pScrn, FALSE);
      drmCtlUninstHandler(pI830->drmSubFD);
   }
#endif

#ifndef HAVE_FREE_SHADOW
   for (o = 0; o < config->num_crtc; o++) {
       xf86CrtcPtr crtc = config->crtc[o];

       if (crtc->rotatedPixmap || crtc->rotatedData) {
	   crtc->funcs->shadow_destroy(crtc, crtc->rotatedPixmap,
				crtc->rotatedData);
	   crtc->rotatedPixmap = NULL;
	   crtc->rotatedData = NULL;
       }
   }
#else
   xf86RotateFreeShadow(pScrn);
#endif

   xf86_hide_cursors (pScrn);

   RestoreHWState(pScrn);

   i830_stop_ring(pScrn, TRUE);

   if (pI830->debug_modes) {
      i830CompareRegsToSnapshot(pScrn, "After LeaveVT");
      i830DumpRegs (pScrn);
   }

   if (I830IsPrimary(pScrn))
      i830_unbind_all_memory(pScrn);

   /* Tell the kernel to evict all buffer objects and block new buffer
    * allocations until we relese the lock.
    */
#ifdef XF86DRI_MM
   if (pI830->directRenderingOpen) {
      if (pI830->memory_manager != NULL && pScrn->vtSema) {
	 drmMMLock(pI830->drmSubFD, DRM_BO_MEM_TT, 1, 0);
      }
   }
#endif /* XF86DRI_MM */

   if (pI830->useEXA && IS_I965G(pI830))
      gen4_render_state_cleanup(pScrn);

   if (pI830->AccelInfoRec)
      pI830->AccelInfoRec->NeedToSync = FALSE;
}

/*
 * This gets called when gaining control of the VT, and from ScreenInit().
 */
static Bool
I830EnterVT(int scrnIndex, int flags)
{
   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
   I830Ptr  pI830 = I830PTR(pScrn);

   DPRINTF(PFX, "Enter VT\n");

   /*
    * Only save state once per server generation since that's what most
    * drivers do.  Could change this to save state at each VT enter.
    */
   if (pI830->SaveGeneration != serverGeneration) {
      pI830->SaveGeneration = serverGeneration;
      SaveHWState(pScrn);
   }

   pI830->leaving = FALSE;

#ifdef XF86DRI_MM
   if (pI830->directRenderingEnabled) {
      /* Unlock the memory manager first of all so that we can pin our
       * buffer objects
       */
      if (pI830->memory_manager != NULL && pScrn->vtSema) {
	 drmMMUnlock(pI830->drmSubFD, DRM_BO_MEM_TT, 1);
      }
   }
#endif /* XF86DRI_MM */

   if (I830IsPrimary(pScrn))
      if (!i830_bind_all_memory(pScrn))
         return FALSE;

   i830_describe_allocations(pScrn, 1, "");

   /* Update the screen pixmap in case the buffer moved */
   i830_update_front_offset(pScrn);

   if (pI830->useEXA && IS_I965G(pI830))
      gen4_render_state_init(pScrn);

   if (i830_check_error_state(pScrn)) {
      xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		 "Existing errors found in hardware state.\n");
   }

   i830_stop_ring(pScrn, FALSE);
   i830_start_ring(pScrn);
   if (!pI830->SWCursor)
      I830InitHWCursor(pScrn);

   /* Tell the BIOS that we're in control of mode setting now. */
   i830_init_bios_control(pScrn);

   i830_init_clock_gating(pScrn);

   if (pI830->power_context)
       OUTREG(PWRCTXA, pI830->power_context->offset | PWRCTX_EN);

   /* Clear the framebuffer */
   memset(pI830->FbBase + pScrn->fbOffset, 0,
	  pScrn->virtualY * pScrn->displayWidth * pI830->cpp);

   if (!xf86SetDesiredModes (pScrn))
      return FALSE;
   
   if (pI830->debug_modes) {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Hardware state at EnterVT:\n");
      i830DumpRegs (pScrn);
   }
   i830DescribeOutputConfiguration(pScrn);

#ifdef XF86DRI
   if (pI830->directRenderingEnabled) {
       /* HW status is fixed, we need to set it up before any drm
	* operation which accessing that page, like irq install, etc.
	*/
       if (pI830->starting) {
	   if (HWS_NEED_GFX(pI830) && !I830DRISetHWS(pScrn)) {
		   xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Fail to setup hardware status page.\n");
		   I830DRICloseScreen(pScrn->pScreen);
		   return FALSE;
	   }
	   if (!I830DRIInstIrqHandler(pScrn)) {
	       I830DRICloseScreen(pScrn->pScreen);
	       return FALSE;
	   }
       }

      /* Update buffer offsets in sarea and mappings, since buffer offsets
       * may have changed.
       */
      if (!i830_update_dri_buffers(pScrn))
	 FatalError("i830_update_dri_buffers() failed\n");

      I830DRISetVBlankInterrupt (pScrn, TRUE);

      if (!pI830->starting) {
         ScreenPtr pScreen = pScrn->pScreen;
         drmI830Sarea *sarea = (drmI830Sarea *) DRIGetSAREAPrivate(pScreen);
         int i;

	 I830DRIResume(screenInfo.screens[scrnIndex]);

	 i830_refresh_ring(pScrn);
	 I830Sync(pScrn);

	 sarea->texAge++;
	 for(i = 0; i < I830_NR_TEX_REGIONS+1 ; i++)
	    sarea->texList[i].age = sarea->texAge;

	 DPRINTF(PFX, "calling dri unlock\n");
	 DRIUnlock(screenInfo.screens[pScrn->scrnIndex]);
      }
      pI830->LockHeld = 0;
   }
#endif

   /* Set the hotkey to just notify us.  We can check its results periodically
    * in the CheckDevicesTimer.  Eventually we want the kernel to just hand us
    * an input event when someone presses the button, but for now we just have
    * to poll.
    */
   i830SetHotkeyControl(pScrn, HOTKEY_DRIVER_NOTIFY);

   if (pI830->checkDevices)
      pI830->devicesTimer = TimerSet(NULL, 0, 1000, I830CheckDevicesTimer, pScrn);

   /* Mark 3D state as being clobbered and setup the basics */
   *pI830->last_3d = LAST_3D_OTHER;
   IntelEmitInvarientState(pScrn);

   return TRUE;
}

static Bool
I830SwitchMode(int scrnIndex, DisplayModePtr mode, int flags)
{
   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
   I830Ptr pI830 = I830PTR(pScrn);

   return xf86SetSingleMode (pScrn, mode, pI830->rotation);
}

static Bool
I830CloseScreen(int scrnIndex, ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
   I830Ptr pI830 = I830PTR(pScrn);
#ifdef I830_USE_XAA
   XAAInfoRecPtr infoPtr = pI830->AccelInfoRec;
#endif

   pI830->closing = TRUE;

   if (pScrn->vtSema == TRUE) {
      I830LeaveVT(scrnIndex, 0);
#ifdef XF86DRI_MM
      if (pI830->directRenderingEnabled) {
 	 if (pI830->memory_manager != NULL) {
	    drmMMUnlock(pI830->drmSubFD, DRM_BO_MEM_TT, 1);
	 }
      }
#endif /* XF86DRI_MM */

   }

   if (pI830->devicesTimer)
      TimerCancel(pI830->devicesTimer);
   pI830->devicesTimer = NULL;

   DPRINTF(PFX, "\nUnmapping memory\n");
   I830UnmapMem(pScrn);
   vgaHWUnmapMem(pScrn);

   if (pI830->ScanlineColorExpandBuffers) {
      xfree(pI830->ScanlineColorExpandBuffers);
      pI830->ScanlineColorExpandBuffers = NULL;
   }
#ifdef I830_USE_XAA
   if (infoPtr) {
      if (infoPtr->ScanlineColorExpandBuffers)
	 xfree(infoPtr->ScanlineColorExpandBuffers);
      XAADestroyInfoRec(infoPtr);
      pI830->AccelInfoRec = NULL;
   }
#endif
#ifdef I830_USE_EXA
   if (pI830->useEXA && pI830->EXADriverPtr) {
       exaDriverFini(pScreen);
       xfree(pI830->EXADriverPtr);
       pI830->EXADriverPtr = NULL;
   }
#endif
   xf86_cursors_fini (pScreen);

   i830_allocator_fini(pScrn);
#ifdef XF86DRI
   if (pI830->directRenderingOpen) {
#ifdef DAMAGE
      if (pI830->pDamage) {
	 PixmapPtr pPix = pScreen->GetScreenPixmap(pScreen);

	 DamageUnregister(&pPix->drawable, pI830->pDamage);
	 DamageDestroy(pI830->pDamage);
	 pI830->pDamage = NULL;
      }
#endif
      pI830->directRenderingOpen = FALSE;
      I830DRICloseScreen(pScreen);
   }
#endif

   if (I830IsPrimary(pScrn)) {
      xf86GARTCloseScreen(scrnIndex);

      xfree(pI830->LpRing);
      pI830->LpRing = NULL;
      xfree(pI830->overlayOn);
      pI830->overlayOn = NULL;
      xfree(pI830->last_3d);
      pI830->last_3d = NULL;
   }

   pScrn->PointerMoved = pI830->PointerMoved;
   pScrn->vtSema = FALSE;
   pI830->closing = FALSE;
   pScreen->CloseScreen = pI830->CloseScreen;
   return (*pScreen->CloseScreen) (scrnIndex, pScreen);
}

static ModeStatus
I830ValidMode(int scrnIndex, DisplayModePtr mode, Bool verbose, int flags)
{
   if (mode->Flags & V_INTERLACE) {
      if (verbose) {
	 xf86DrvMsg(scrnIndex, X_PROBED,
		    "Removing interlaced mode \"%s\"\n", mode->name);
      }
      return MODE_BAD;
   }
   return MODE_OK;
}

#ifndef SUSPEND_SLEEP
#define SUSPEND_SLEEP 0
#endif
#ifndef RESUME_SLEEP
#define RESUME_SLEEP 0
#endif

/*
 * This function is only required if we need to do anything differently from
 * DoApmEvent() in common/xf86PM.c, including if we want to see events other
 * than suspend/resume.
 */
static Bool
I830PMEvent(int scrnIndex, pmEvent event, Bool undo)
{
   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
   I830Ptr pI830 = I830PTR(pScrn);

   DPRINTF(PFX, "Enter VT, event %d, undo: %s\n", event, BOOLTOSTRING(undo));
 
   switch(event) {
   case XF86_APM_SYS_SUSPEND:
   case XF86_APM_CRITICAL_SUSPEND: /*do we want to delay a critical suspend?*/
   case XF86_APM_USER_SUSPEND:
   case XF86_APM_SYS_STANDBY:
   case XF86_APM_USER_STANDBY:
      if (!undo && !pI830->suspended) {
	 pScrn->LeaveVT(scrnIndex, 0);
	 pI830->suspended = TRUE;
	 sleep(SUSPEND_SLEEP);
      } else if (undo && pI830->suspended) {
	 sleep(RESUME_SLEEP);
	 pScrn->EnterVT(scrnIndex, 0);
	 pI830->suspended = FALSE;
      }
      break;
   case XF86_APM_STANDBY_RESUME:
   case XF86_APM_NORMAL_RESUME:
   case XF86_APM_CRITICAL_RESUME:
      if (pI830->suspended) {
	 sleep(RESUME_SLEEP);
	 pScrn->EnterVT(scrnIndex, 0);
	 pI830->suspended = FALSE;
	 /*
	  * Turn the screen saver off when resuming.  This seems to be
	  * needed to stop xscreensaver kicking in (when used).
	  *
	  * XXX DoApmEvent() should probably call this just like
	  * xf86VTSwitch() does.  Maybe do it here only in 4.2
	  * compatibility mode.
	  */
	 SaveScreens(SCREEN_SAVER_FORCER, ScreenSaverReset);
      }
      break;
   /* This is currently used for ACPI */
   case XF86_APM_CAPABILITY_CHANGED:
#if 0
      /* If we had status checking turned on, turn it off now */
      if (pI830->checkDevices) {
         if (pI830->devicesTimer)
            TimerCancel(pI830->devicesTimer);
         pI830->devicesTimer = NULL;
         pI830->checkDevices = FALSE; 
      }
#endif
      if (!I830IsPrimary(pScrn))
         return TRUE;

      ErrorF("I830PMEvent: Capability change\n");

      I830CheckDevicesTimer(NULL, 0, pScrn);
      SaveScreens(SCREEN_SAVER_FORCER, ScreenSaverReset);
      if (pI830->quirk_flag & QUIRK_RESET_MODES)
	 xf86SetDesiredModes(pScrn);

      break;
   default:
      ErrorF("I830PMEvent: received APM event %d\n", event);
   }
   return TRUE;
}

xf86CrtcPtr
i830_pipe_to_crtc(ScrnInfoPtr pScrn, int pipe)
{
   xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR (pScrn);
   int c;
   
   for (c = 0; c < config->num_crtc; c++) {
      xf86CrtcPtr crtc = config->crtc[c];
      I830CrtcPrivatePtr intel_crtc = crtc->driver_private;

      if (intel_crtc->pipe == pipe)
	  return crtc;
   }

   return NULL;
} 

#if 0
/**
 * This function is used for testing of the screen detect functions from the
 * periodic timer.
 */
static void
i830MonitorDetectDebugger(ScrnInfoPtr pScrn)
{
   Bool found_crt;
   I830Ptr pI830 = I830PTR(pScrn);
   int start, finish, i;

   if (!pScrn->vtSema)
      return 1000;

   for (i = 0; i < xf86_config->num_output; i++) {
      enum output_status ret;
      char *result;

      start = GetTimeInMillis();
      ret = pI830->output[i].detect(pScrn, &pI830->output[i]);
      finish = GetTimeInMillis();

      if (ret == OUTPUT_STATUS_CONNECTED)
	 result = "connected";
      else if (ret == OUTPUT_STATUS_DISCONNECTED)
	 result = "disconnected";
      else
	 result = "unknown";

      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Detected SDVO as %s in %dms\n",
		 result, finish - start);
   }
}
#endif

static CARD32
I830CheckDevicesTimer(OsTimerPtr timer, CARD32 now, pointer arg)
{
   ScrnInfoPtr pScrn = (ScrnInfoPtr) arg;
   I830Ptr pI830 = I830PTR(pScrn);
   uint8_t gr18;

   if (!pScrn->vtSema)
      return 1000;

#if 0
   i830MonitorDetectDebugger(pScrn);
#endif

   /* Check for a hotkey press report from the BIOS. */
   gr18 = pI830->readControl(pI830, GRX, 0x18);
   if ((gr18 & (HOTKEY_TOGGLE | HOTKEY_SWITCH)) != 0) {
      /* The user has pressed the hotkey requesting a toggle or switch.
       * Re-probe our connected displays and turn on whatever we find.
       *
       * In the future, we want the hotkey to dump down to a user app which
       * implements a sensible policy using RandR-1.2.  For now, all we get
       * is this.
       */
      
      xf86ProbeOutputModes (pScrn, 0, 0);
      xf86SetScrnInfoModes (pScrn);
      xf86DiDGAReInit (pScrn->pScreen);
      xf86SwitchMode(pScrn->pScreen, pScrn->currentMode);

      /* Clear the BIOS's hotkey press flags */
      gr18 &= ~(HOTKEY_TOGGLE | HOTKEY_SWITCH);
      pI830->writeControl(pI830, GRX, 0x18, gr18);
   }

   return 1000;
}

void
i830WaitSync(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

#ifdef I830_USE_XAA
   if (!pI830->noAccel && !pI830->useEXA && pI830->AccelInfoRec 
	&& pI830->AccelInfoRec->NeedToSync) {
      (*pI830->AccelInfoRec->Sync)(pScrn);
      pI830->AccelInfoRec->NeedToSync = FALSE;
   }
#endif
#ifdef I830_USE_EXA
   if (!pI830->noAccel && pI830->useEXA && pI830->EXADriverPtr) {
	ScreenPtr pScreen = screenInfo.screens[pScrn->scrnIndex];
	exaWaitSync(pScreen);
   }
#endif
}

void
i830MarkSync(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);

#ifdef I830_USE_XAA
   if (!pI830->useEXA && pI830->AccelInfoRec)
      pI830->AccelInfoRec->NeedToSync = TRUE;
#endif
#ifdef I830_USE_EXA
   if (pI830->useEXA && pI830->EXADriverPtr) {
      ScreenPtr pScreen = screenInfo.screens[pScrn->scrnIndex];
      exaMarkSync(pScreen);
   }
#endif
}

void
I830InitpScrn(ScrnInfoPtr pScrn)
{
   pScrn->PreInit = I830PreInit;
   pScrn->ScreenInit = I830ScreenInit;
   pScrn->SwitchMode = I830SwitchMode;
   pScrn->AdjustFrame = i830AdjustFrame;
   pScrn->EnterVT = I830EnterVT;
   pScrn->LeaveVT = I830LeaveVT;
   pScrn->FreeScreen = I830FreeScreen;
   pScrn->ValidMode = I830ValidMode;
   pScrn->PMEvent = I830PMEvent;
}
