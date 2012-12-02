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
 * Authors: Jeff Hartmann <jhartmann@valinux.com>
 *          Abraham van der Merwe <abraham@2d3d.co.za>
 *          David Dawes <dawes@xfree86.org>
 *          Alan Hourihane <alanh@tungstengraphics.com>
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
#include <poll.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Priv.h"
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
#include "shadow.h"
#include "intel.h"
#include "i830_reg.h"
#include "i830_display.h"
#include "i830_bios.h"
#include "intel_video.h"

#ifdef INTEL_XVMC
#define _INTEL_XVMC_SERVER_
#include "intel_hwmc.h"
#endif

#include "legacy/legacy.h"

#include <sys/ioctl.h>
#include "i915_drm.h"
#include <xf86drmMode.h>

#define BIT(x) (1 << (x))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define NB_OF(x) (sizeof (x) / sizeof (*x))

/* *INDENT-OFF* */
/*
 * Note: "ColorKey" is provided for compatibility with the i810 driver.
 * However, the correct option name is "VideoKey".  "ColorKey" usually
 * refers to the tranparency key for 8+24 overlays, not for video overlays.
 */

typedef enum {
   OPTION_DRI,
   OPTION_VIDEO_KEY,
   OPTION_COLOR_KEY,
   OPTION_MODEDEBUG,
   OPTION_FALLBACKDEBUG,
   OPTION_LVDS24BITMODE,
   OPTION_FBC,
   OPTION_TILING_FB,
   OPTION_TILING_2D,
   OPTION_SHADOW,
   OPTION_SWAPBUFFERS_WAIT,
   OPTION_LVDSFIXEDMODE,
   OPTION_FORCEENABLEPIPEA,
#ifdef INTEL_XVMC
   OPTION_XVMC,
#endif
   OPTION_PREFER_OVERLAY,
   OPTION_DEBUG_FLUSH_BATCHES,
   OPTION_DEBUG_FLUSH_CACHES,
   OPTION_DEBUG_WAIT,
} I830Opts;

static OptionInfoRec I830Options[] = {
   {OPTION_DRI,		"DRI",		OPTV_BOOLEAN,	{0},	TRUE},
   {OPTION_COLOR_KEY,	"ColorKey",	OPTV_INTEGER,	{0},	FALSE},
   {OPTION_VIDEO_KEY,	"VideoKey",	OPTV_INTEGER,	{0},	FALSE},
   {OPTION_MODEDEBUG,	"ModeDebug",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_FALLBACKDEBUG, "FallbackDebug", OPTV_BOOLEAN, {0},	FALSE},
   {OPTION_LVDS24BITMODE, "LVDS24Bit",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_FBC,		"FramebufferCompression", OPTV_BOOLEAN, {0}, TRUE},
   {OPTION_TILING_2D,	"Tiling",	OPTV_BOOLEAN,	{0},	TRUE},
   {OPTION_TILING_FB,	"LinearFramebuffer",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_SHADOW,	"Shadow",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_SWAPBUFFERS_WAIT, "SwapbuffersWait", OPTV_BOOLEAN,	{0},	TRUE},
   {OPTION_LVDSFIXEDMODE, "LVDSFixedMode", OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_FORCEENABLEPIPEA, "ForceEnablePipeA", OPTV_BOOLEAN,	{0},	FALSE},
#ifdef INTEL_XVMC
   {OPTION_XVMC,	"XvMC",		OPTV_BOOLEAN,	{0},	TRUE},
#endif
   {OPTION_PREFER_OVERLAY, "XvPreferOverlay", OPTV_BOOLEAN, {0}, FALSE},
   {OPTION_DEBUG_FLUSH_BATCHES, "DebugFlushBatches", OPTV_BOOLEAN, {0}, FALSE},
   {OPTION_DEBUG_FLUSH_CACHES, "DebugFlushCaches", OPTV_BOOLEAN, {0}, FALSE},
   {OPTION_DEBUG_WAIT, "DebugWait", OPTV_BOOLEAN, {0}, FALSE},
   {-1,			NULL,		OPTV_NONE,	{0},	FALSE}
};
/* *INDENT-ON* */

static void i830AdjustFrame(ADJUST_FRAME_ARGS_DECL);
static Bool I830CloseScreen(CLOSE_SCREEN_ARGS_DECL);
static Bool I830EnterVT(VT_FUNC_ARGS_DECL);
static Bool SaveHWState(ScrnInfoPtr scrn);
static Bool RestoreHWState(ScrnInfoPtr scrn);

/* temporary */
extern void xf86SetCursor(ScreenPtr screen, CursorPtr pCurs, int x, int y);

#ifdef I830DEBUG
void
I830DPRINTF(const char *filename, int line, const char *function,
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
#endif /* #ifdef I830DEBUG */

/* Export I830 options to i830 driver where necessary */
const OptionInfoRec *intel_uxa_available_options(int chipid, int busid)
{
	return I830Options;
}

static Bool I830GetRec(ScrnInfoPtr scrn)
{
	intel_screen_private *intel;

	if (scrn->driverPrivate)
		return TRUE;
	intel = scrn->driverPrivate = xnfcalloc(sizeof(intel_screen_private), 1);
	return TRUE;
}

static void I830FreeRec(ScrnInfoPtr scrn)
{
	if (!scrn)
		return;
	if (!scrn->driverPrivate)
		return;

	free(scrn->driverPrivate);
	scrn->driverPrivate = NULL;
}

static int
I830DetectMemory(ScrnInfoPtr scrn)
{
   intel_screen_private *intel = intel_get_screen_private(scrn);
   uint16_t gmch_ctrl;
   int memsize = 0, gtt_size;
   int range;
   struct pci_device *bridge = intel_host_bridge ();

   if (IS_GEN6(intel) || IS_GEN7(intel))
      pci_device_cfg_read_u16(bridge, &gmch_ctrl, SNB_GMCH_CTRL);
   else
      pci_device_cfg_read_u16(bridge, &gmch_ctrl, I830_GMCH_CTRL);

   if (IS_I965G(intel)) {
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
   } else if (IS_G33CLASS(intel)) {
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
      gtt_size = intel->FbMapSize / (1024*1024);
   }

   xf86DrvMsg(scrn->scrnIndex, X_INFO, "detected %d kB GTT.\n", gtt_size);

   /* The stolen memory has the GTT at the top, and the 4KB popup below that.
    * Everything else can be freely used by the graphics driver.
    */
   range = gtt_size + 4;

   /* new 4 series hardware has seperate GTT stolen with GFX stolen */
   if (IS_G4X(intel) || IS_IGD(intel) || IS_GEN5(intel) || IS_GEN6(intel) ||
     IS_GEN7(intel))
       range = 4;

   if (IS_GEN6(intel) || IS_GEN7(intel)) {
      switch (gmch_ctrl & SNB_GMCH_GMS_STOLEN_MASK) {
      case SNB_GMCH_GMS_STOLEN_32M:
	 memsize = MB(32) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_64M:
	 memsize = MB(64) - KB(range);
         break;
      case SNB_GMCH_GMS_STOLEN_96M:
	 memsize = MB(96) - KB(range);
         break;
      case SNB_GMCH_GMS_STOLEN_128M:
	 memsize = MB(128) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_160M:
	 memsize = MB(160) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_192M:
	 memsize = MB(192) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_224M:
	 memsize = MB(224) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_256M:
	 memsize = MB(256) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_288M:
	 memsize = MB(288) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_320M:
	 memsize = MB(320) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_352M:
	 memsize = MB(352) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_384M:
	 memsize = MB(384) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_416M:
	 memsize = MB(416) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_448M:
	 memsize = MB(448) - KB(range);
	 break;
      case SNB_GMCH_GMS_STOLEN_480M:
	 memsize = MB(480) - KB(range);
         break;
      case SNB_GMCH_GMS_STOLEN_512M:
	 memsize = MB(512) - KB(range);
         break;
      }
   } else if (IS_I85X(intel) || IS_I865G(intel) || IS_I9XX(intel)) {
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
	 if (IS_I9XX(intel))
	    memsize = MB(48) - KB(range);
	 break;
      case I915G_GMCH_GMS_STOLEN_64M:
	 if (IS_I9XX(intel))
	    memsize = MB(64) - KB(range);
	 break;
      case G33_GMCH_GMS_STOLEN_128M:
	 if (IS_I9XX(intel))
	     memsize = MB(128) - KB(range);
	 break;
      case G33_GMCH_GMS_STOLEN_256M:
	 if (IS_I9XX(intel))
	     memsize = MB(256) - KB(range);
	 break;
      case INTEL_GMCH_GMS_STOLEN_96M:
	 if (IS_I9XX(intel))
	     memsize = MB(96) - KB(range);
	 break;
      case INTEL_GMCH_GMS_STOLEN_160M:
	 if (IS_I9XX(intel))
	     memsize = MB(160) - KB(range);
	 break;
      case INTEL_GMCH_GMS_STOLEN_224M:
	 if (IS_I9XX(intel))
	     memsize = MB(224) - KB(range);
	 break;
      case INTEL_GMCH_GMS_STOLEN_352M:
	 if (IS_I9XX(intel))
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
	 xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		    "Local memory found, but won't be used.\n");
	 break;
      }
   }

#if 0
   /* And 64KB page aligned */
   memsize &= ~0xFFFF;
#endif

   if (memsize > 0) {
      xf86DrvMsg(scrn->scrnIndex, X_INFO,
		 "detected %d kB stolen memory.\n", memsize / 1024);
   } else {
      xf86DrvMsg(scrn->scrnIndex, X_INFO, "no video memory detected.\n");
   }

   return memsize;
}

static Bool
I830MapMMIO(ScrnInfoPtr scrn)
{
   int err;
   struct pci_device *device;
   intel_screen_private *intel = intel_get_screen_private(scrn);

   device = intel->PciInfo;
   err = pci_device_map_range (device,
			       intel->MMIOAddr,
			       intel->MMIOSize,
			       PCI_DEV_MAP_FLAG_WRITABLE,
			       (void **) &intel->MMIOBase);
   if (err) 
   {
      xf86DrvMsg (scrn->scrnIndex, X_ERROR,
		  "Unable to map mmio range. %s (%d)\n",
		  strerror (err), err);
      return FALSE;
   }

   /* Set up the GTT mapping for the various places it has been moved over
    * time.
    */
   if (IS_I9XX(intel)) {
      uint32_t gttaddr;

      if (IS_I965G(intel)) 
      {
	 if (IS_G4X(intel) || IS_GEN5(intel) || IS_GEN6(intel) ||
	    IS_GEN7(intel)) {
	     gttaddr = intel->MMIOAddr + MB(2);
	     intel->GTTMapSize = MB(2);
	 } else {
	     gttaddr = intel->MMIOAddr + KB(512);
	     intel->GTTMapSize = KB(512);
	 }
      }
      else
      {
	 gttaddr = I810_MEMBASE(intel->PciInfo, 3) & 0xFFFFFF00;
	 intel->GTTMapSize = intel->FbMapSize / 1024;
      }
      err = pci_device_map_range (device,
				  gttaddr, intel->GTTMapSize,
				  PCI_DEV_MAP_FLAG_WRITABLE,
				  (void **) &intel->GTTBase);
      if (err)
      {
	 xf86DrvMsg (scrn->scrnIndex, X_ERROR,
		     "Unable to map GTT range. %s (%d)\n",
		     strerror (err), err);
	 return FALSE;
      }
   } else {
      /* The GTT aperture on i830 is write-only.  We could probably map the
       * actual physical pages that back it, but leave it alone for now.
       */
      intel->GTTBase = NULL;
      intel->GTTMapSize = 0;
   }

   return TRUE;
}

static Bool
I830MapMem(ScrnInfoPtr scrn)
{
   intel_screen_private *intel = intel_get_screen_private(scrn);
   long i;
   struct pci_device *const device = intel->PciInfo;
   int err;

   for (i = 2; i < intel->FbMapSize; i <<= 1) ;
   intel->FbMapSize = i;

   err = pci_device_map_range (device, intel->LinearAddr, intel->FbMapSize,
			       PCI_DEV_MAP_FLAG_WRITABLE | PCI_DEV_MAP_FLAG_WRITE_COMBINE,
			       (void **) &intel->FbBase);
    if (err)
	return FALSE;

   return TRUE;
}

static void
I830UnmapMMIO(ScrnInfoPtr scrn)
{
   intel_screen_private *intel = intel_get_screen_private(scrn);

   pci_device_unmap_range (intel->PciInfo, intel->MMIOBase, intel->MMIOSize);
   intel->MMIOBase = NULL;

   if (IS_I9XX(intel)) {
      pci_device_unmap_range (intel->PciInfo, intel->GTTBase, intel->GTTMapSize);
      intel->GTTBase = NULL;
   }
}

static Bool
I830UnmapMem(ScrnInfoPtr scrn)
{
   intel_screen_private *intel = intel_get_screen_private(scrn);

   pci_device_unmap_range (intel->PciInfo, intel->FbBase, intel->FbMapSize);
   intel->FbBase = NULL;
   I830UnmapMMIO(scrn);
   return TRUE;
}

static void
I830LoadPalette(ScrnInfoPtr scrn, int numColors, int *indices,
		LOCO * colors, VisualPtr pVisual)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	int i, j, index;
	int p;
	uint16_t lut_r[256], lut_g[256], lut_b[256];

	DPRINTF(PFX, "I830LoadPalette: numColors: %d\n", numColors);

	for (p = 0; p < xf86_config->num_crtc; p++) {
		xf86CrtcPtr crtc = xf86_config->crtc[p];
		I830CrtcPrivatePtr intel_crtc = crtc->driver_private;

		/* Initialize to the old lookup table values. */
		for (i = 0; i < 256; i++) {
			lut_r[i] = intel_crtc->lut_r[i] << 8;
			lut_g[i] = intel_crtc->lut_g[i] << 8;
			lut_b[i] = intel_crtc->lut_b[i] << 8;
		}

		switch (scrn->depth) {
		case 15:
			for (i = 0; i < numColors; i++) {
				index = indices[i];
				for (j = 0; j < 8; j++) {
					lut_r[index * 8 + j] =
					    colors[index].red << 8;
					lut_g[index * 8 + j] =
					    colors[index].green << 8;
					lut_b[index * 8 + j] =
					    colors[index].blue << 8;
				}
			}
			break;
		case 16:
			for (i = 0; i < numColors; i++) {
				index = indices[i];

				if (index <= 31) {
					for (j = 0; j < 8; j++) {
						lut_r[index * 8 + j] =
						    colors[index].red << 8;
						lut_b[index * 8 + j] =
						    colors[index].blue << 8;
					}
				}

				for (j = 0; j < 4; j++) {
					lut_g[index * 4 + j] =
					    colors[index].green << 8;
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

/**
 * Adjust the screen pixmap for the current location of the front buffer.
 * This is done at EnterVT when buffers are bound as long as the resources
 * have already been created, but the first EnterVT happens before
 * CreateScreenResources.
 */
static Bool i830CreateScreenResources(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

	screen->CreateScreenResources = intel->CreateScreenResources;
	if (!(*screen->CreateScreenResources) (screen))
		return FALSE;

	return intel_uxa_create_screen_resources(screen);
}

static int i830_output_clones (ScrnInfoPtr scrn, int type_mask)
{
	xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR (scrn);
	int			o;
	int			index_mask = 0;

	for (o = 0; o < config->num_output; o++) {
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
static void I830SetupOutputs(ScrnInfoPtr scrn)
{
	xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR (scrn);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int	    o, c;
	Bool	    lvds_detected = FALSE;

	/* everyone has at least a single analog output */
	i830_crt_init(scrn);

	/* Set up integrated LVDS */
	if (IS_MOBILE(intel) && !IS_I830(intel))
		i830_lvds_init(scrn);

	if (HAS_PCH_SPLIT(intel)) {
		int found;

		if (INREG(HDMIB) & PORT_DETECTED) {
			/* check SDVOB */
			/* found = intel_sdvo_init(dev, HDMIB); */
			found = 0;
			if (!found)
				i830_hdmi_init(scrn, HDMIB);
		}

		if (INREG(HDMIC) & PORT_DETECTED)
			i830_hdmi_init(scrn, HDMIC);

		if (INREG(HDMID) & PORT_DETECTED)
			i830_hdmi_init(scrn, HDMID);

		/* Disable DP by force */
		OUTREG(PCH_DP_B, INREG(PCH_DP_B) & ~PORT_ENABLE);
		OUTREG(PCH_DP_C, INREG(PCH_DP_C) & ~PORT_ENABLE);
		OUTREG(PCH_DP_D, INREG(PCH_DP_D) & ~PORT_ENABLE);

	} else if (IS_I9XX(intel)) {
		Bool found = FALSE;
		if ((INREG(SDVOB) & SDVO_DETECTED)) {
			found = i830_sdvo_init(scrn, SDVOB);

			if (!found && SUPPORTS_INTEGRATED_HDMI(intel))
			i830_hdmi_init(scrn, SDVOB);
		}

		if ((INREG(SDVOB) & SDVO_DETECTED))
			found = i830_sdvo_init(scrn, SDVOC);

		if ((INREG(SDVOC) & SDVO_DETECTED) &&
		    !found && SUPPORTS_INTEGRATED_HDMI(intel))
			i830_hdmi_init(scrn, SDVOC);

	} else {
		i830_dvo_init(scrn);
	}

	if (SUPPORTS_TV(intel))
		i830_tv_init(scrn);
   
	for (o = 0; o < config->num_output; o++) {
		xf86OutputPtr	   output = config->output[o];
		I830OutputPrivatePtr intel_output = output->driver_private;
		int		   crtc_mask;

		if (intel_output->type == I830_OUTPUT_LVDS)
			lvds_detected = TRUE;
      
		crtc_mask = 0;
		for (c = 0; c < config->num_crtc; c++) {
			xf86CrtcPtr	      crtc = config->crtc[c];
			I830CrtcPrivatePtr   intel_crtc = crtc->driver_private;

			if (intel_output->pipe_mask & (1 << intel_crtc->pipe))
				crtc_mask |= (1 << c);
		}
		output->possible_crtcs = crtc_mask;
		output->possible_clones =
		    i830_output_clones(scrn, intel_output->clone_mask);
	}
}

static void i830_init_clock_gating(ScrnInfoPtr scrn)
{
    intel_screen_private *intel = intel_get_screen_private(scrn);

    /* Disable clock gating reported to work incorrectly according to the specs.
     */
	if (IS_GEN6(intel) || IS_GEN7(intel)) {
		uint32_t dspclk_gate = VRHUNIT_CLOCK_GATE_DISABLE;
		OUTREG(PCH_DSPCLK_GATE_D, dspclk_gate);
	} else if (IS_G4X(intel)) {
		uint32_t dspclk_gate;
		OUTREG(RENCLK_GATE_D1, 0);
		OUTREG(RENCLK_GATE_D2, VF_UNIT_CLOCK_GATE_DISABLE |
		    GS_UNIT_CLOCK_GATE_DISABLE | CL_UNIT_CLOCK_GATE_DISABLE);
		OUTREG(RAMCLK_GATE_D, 0);
		dspclk_gate = VRHUNIT_CLOCK_GATE_DISABLE |
		    OVRUNIT_CLOCK_GATE_DISABLE | OVCUNIT_CLOCK_GATE_DISABLE;
		if (IS_GM45(intel))
			dspclk_gate |= DSSUNIT_CLOCK_GATE_DISABLE;
		OUTREG(DSPCLK_GATE_D, dspclk_gate);
	} else if (IS_I965GM(intel)) {
		OUTREG(RENCLK_GATE_D1, I965_RCC_CLOCK_GATE_DISABLE);
		OUTREG(RENCLK_GATE_D2, 0);
		OUTREG(DSPCLK_GATE_D, 0);
		OUTREG(RAMCLK_GATE_D, 0);
		OUTREG16(DEUC, 0);
	} else if (IS_I965G(intel)) {
		OUTREG(RENCLK_GATE_D1, I965_RCZ_CLOCK_GATE_DISABLE |
		    I965_RCC_CLOCK_GATE_DISABLE | I965_RCPB_CLOCK_GATE_DISABLE |
		    I965_ISC_CLOCK_GATE_DISABLE | I965_FBC_CLOCK_GATE_DISABLE);
		OUTREG(RENCLK_GATE_D2, 0);
	} else if (IS_I855(intel) || IS_I865G(intel)) {
		OUTREG(RENCLK_GATE_D1, SV_CLOCK_GATE_DISABLE);
	} else if (IS_I830(intel)) {
		OUTREG(DSPCLK_GATE_D, OVRUNIT_CLOCK_GATE_DISABLE);
	}
}

static void i830_init_bios_control(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Set "extended desktop" */
	OUTREG(SWF0, INREG(SWF0) | (1 << 21));

	/* Set "driver loaded",  "OS unknown", "APM 1.2" */
	OUTREG(SWF4, (INREG(SWF4) & ~((3 << 19) | (7 << 16))) |
	    (1 << 23) | (2 << 16));
}

static int
I830LVDSPresent(ScrnInfoPtr scrn)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR (scrn);
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
I830PreInitDDC(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (!xf86LoadSubModule(scrn, "ddc")) {
		intel->ddc2 = FALSE;
	} else {
		intel->ddc2 = TRUE;
	}

	/* DDC can use I2C bus */
	/* Load I2C if we have the code to use it */
	if (intel->ddc2) {
		if (xf86LoadSubModule(scrn, "i2c")) {
			intel->ddc2 = TRUE;
		} else {
			intel->ddc2 = FALSE;
		}
	}
}

static void PreInitCleanup(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->MMIOBase)
		I830UnmapMMIO(scrn);
	I830FreeRec(scrn);
}

static Bool i830_xf86crtc_resize(ScrnInfoPtr scrn, int width, int height)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int	    old_width, old_height, old_pitch;
	drm_intel_bo *old_front;
	unsigned long pitch;
	uint32_t tiling;

	if (scrn->virtualX == width && scrn->virtualY == height)
		return TRUE;

	old_width = scrn->virtualX;
	old_height = scrn->virtualY;
	old_pitch = scrn->displayWidth;
	old_front = intel->front_buffer;

	intel->front_buffer = intel_allocate_framebuffer(scrn,
							width, height,
							intel->cpp,
							&pitch,
							&tiling);
	if (!intel->front_buffer)
		goto fail;

	intel->front_pitch = pitch;
	intel->front_tiling = tiling;

	scrn->virtualX = width;
	scrn->virtualY = height;

	intel_sync(scrn);
	i830WaitForVblank(scrn);

	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "New front buffer at 0x%lx\n",
		   intel->front_buffer->offset);
	i830_set_new_crtc_bo(scrn);
	intel_sync(scrn);
	i830WaitForVblank(scrn);

	intel_uxa_create_screen_resources(scrn->pScreen);

	if (old_front) {
		/* if we own the vt, don't forget to unpin */
		if (scrn->vtSema)
			(void)dri_bo_unpin(old_front);
		drm_intel_bo_unreference(old_front);
	}

	return TRUE;

fail:
	intel->front_buffer = old_front;
	scrn->virtualX = old_width;
	scrn->virtualY = old_height;
	scrn->displayWidth = old_pitch;

	return FALSE;
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
i830SetHotkeyControl(ScrnInfoPtr scrn, int mode)
{
   intel_screen_private *intel = intel_get_screen_private(scrn);
   uint8_t gr18;

   /* Don't mess with kernel settings... */
   if (intel->use_drm_mode)
       return;

   gr18 = intel->readControl(intel, GRX, 0x18);
   if (mode == HOTKEY_BIOS_SWITCH)
      gr18 &= ~HOTKEY_VBIOS_SWITCH_BLOCK;
   else
      gr18 |= HOTKEY_VBIOS_SWITCH_BLOCK;
   intel->writeControl(intel, GRX, 0x18, gr18);
}

/*
 * DRM mode setting Linux only at this point... later on we could
 * add a wrapper here.
 */
static Bool intel_kernel_mode_enabled(ScrnInfoPtr scrn)
{
	struct pci_device *dev;
	char id[20];
	int ret;

	dev = xf86GetPciInfoForEntity(xf86GetEntityInfo(scrn->entityList[0])->index);
	snprintf(id, sizeof(id),
		 "pci:%04x:%02x:%02x.%d",
		 dev->domain, dev->bus, dev->dev, dev->func);

	ret = drmCheckModesettingSupported(id);
	if (ret) {
		if (xf86LoadKernelModule("i915"))
			ret = drmCheckModesettingSupported(id);
	}
	/* Be nice to the user and load fbcon too */
	if (!ret)
		(void)xf86LoadKernelModule("fbcon");

	return ret == 0;
}

static Bool intel_check_chipset_option(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	MessageType from = X_PROBED;
	int fb_bar, mmio_bar;

	if (!intel->use_drm_mode)
		I830SetPIOAccess(intel);

	intel_detect_chipset(scrn,
			     intel->PciInfo,
			     &intel->chipset);

	/* Set the Chipset and ChipRev, allowing config file entries to override. */
	if (intel->pEnt->device->chipset && *intel->pEnt->device->chipset) {
		scrn->chipset = intel->pEnt->device->chipset;
		from = X_CONFIG;
	} else if (intel->pEnt->device->chipID >= 0) {
		scrn->chipset = (char *)xf86TokenToString(intel_chipsets,
							   intel->pEnt->device->chipID);
		from = X_CONFIG;
		xf86DrvMsg(scrn->scrnIndex, X_CONFIG,
			   "ChipID override: 0x%04X\n",
			   intel->pEnt->device->chipID);
		DEVICE_ID(intel->PciInfo) = intel->pEnt->device->chipID;
	} else {
		from = X_PROBED;
		scrn->chipset = (char *)xf86TokenToString(intel_chipsets,
							   DEVICE_ID(intel->PciInfo));
	}

	if (intel->pEnt->device->chipRev >= 0) {
		xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "ChipRev override: %d\n",
			   intel->pEnt->device->chipRev);
	}

	xf86DrvMsg(scrn->scrnIndex, from, "Chipset: \"%s\"\n",
		   (scrn->chipset != NULL) ? scrn->chipset : "Unknown i8xx");
	/* Check if the HW cursor needs physical address. */
	if (IS_MOBILE(intel) || IS_I9XX(intel))
		intel->CursorNeedsPhysical = TRUE;
	else
		intel->CursorNeedsPhysical = FALSE;

	if (IS_I965G(intel) || IS_G33CLASS(intel))
		intel->CursorNeedsPhysical = FALSE;

	/* Skip the rest if the kernel is taking care of things */
	if (intel->use_drm_mode)
		return TRUE;

	/* Now that we know the chipset, figure out the resource base addrs */
	if (IS_I9XX(intel)) {
		fb_bar = 2;
		mmio_bar = 0;
	} else {
		fb_bar = 0;
		mmio_bar = 1;
	}

	if (intel->pEnt->device->MemBase != 0) {
		intel->LinearAddr = intel->pEnt->device->MemBase;
		from = X_CONFIG;
	} else {
		intel->LinearAddr = I810_MEMBASE (intel->PciInfo, fb_bar);
		if (intel->LinearAddr == 0) {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			    "No valid FB address in PCI config space\n");
			PreInitCleanup(scrn);
			return FALSE;
		}
	}

	xf86DrvMsg(scrn->scrnIndex, from, "Linear framebuffer at 0x%lX\n",
	    (unsigned long)intel->LinearAddr);

	if (intel->pEnt->device->IOBase != 0) {
		intel->MMIOAddr = intel->pEnt->device->IOBase;
		from = X_CONFIG;
		intel->MMIOSize = I810_REG_SIZE;
	} else {
		intel->MMIOAddr = I810_MEMBASE (intel->PciInfo, mmio_bar);
		if (intel->MMIOAddr == 0) {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			    "No valid MMIO address in PCI config space\n");
			PreInitCleanup(scrn);
			return FALSE;
		}
		intel->MMIOSize = intel->PciInfo->regions[mmio_bar].size;
	}

	xf86DrvMsg(scrn->scrnIndex, from,
	    "IO registers at addr 0x%lX size %u\n",
	    (unsigned long)intel->MMIOAddr, intel->MMIOSize);

	/* Now figure out mapsize on 8xx chips */
	if (IS_I830(intel) || IS_845G(intel)) {
		uint16_t		gmch_ctrl;
		struct pci_device *bridge;

		bridge = intel_host_bridge ();
		pci_device_cfg_read_u16 (bridge, &gmch_ctrl, I830_GMCH_CTRL);
		if ((gmch_ctrl & I830_GMCH_MEM_MASK) == I830_GMCH_MEM_128M) {
			intel->FbMapSize = 0x8000000;
		} else {
			/* 64MB - has this been tested ?? */
			intel->FbMapSize = 0x4000000;
		}
	} else {
		if (IS_I9XX(intel)) {
			intel->FbMapSize = intel->PciInfo->regions[fb_bar].size;
		} else {
			/* 128MB aperture for later i8xx series. */
			intel->FbMapSize = 0x8000000;
		}
	}

    return TRUE;
}

static Bool I830LoadSyms(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->use_drm_mode)
		return TRUE;

	/* The vgahw module should be loaded here when needed */
	if (!xf86LoadSubModule(scrn, "vgahw"))
		return FALSE;

	if (!xf86LoadSubModule(scrn, "ramdac"))
		return FALSE;

	return TRUE;
}

static Bool I830GetEarlyOptions(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Process the options */
	xf86CollectOptions(scrn, NULL);
	if (!(intel->Options = malloc(sizeof(I830Options))))
		return FALSE;
	memcpy(intel->Options, I830Options, sizeof(I830Options));
	xf86ProcessOptions(scrn->scrnIndex, scrn->options, intel->Options);

	intel->fallback_debug = xf86ReturnOptValBool(intel->Options,
						     OPTION_FALLBACKDEBUG,
						     FALSE);

	if (xf86ReturnOptValBool(intel->Options, OPTION_MODEDEBUG, FALSE)) {
		intel->debug_modes = TRUE;
	} else {
		intel->debug_modes = FALSE;
	}

	if (xf86ReturnOptValBool(intel->Options, OPTION_LVDS24BITMODE, FALSE)) {
		intel->lvds_24_bit_mode = TRUE;
	} else {
		intel->lvds_24_bit_mode = FALSE;
	}

	if (xf86ReturnOptValBool(intel->Options, OPTION_LVDSFIXEDMODE, TRUE)) {
		intel->skip_panel_detect = FALSE;
	} else {
		intel->skip_panel_detect = TRUE;
	}

	if (xf86ReturnOptValBool(intel->Options,
	    OPTION_FORCEENABLEPIPEA, FALSE))
		intel->quirk_flag |= QUIRK_PIPEA_FORCE;
	intel->debug_flush = 0;

	if (xf86ReturnOptValBool(intel->Options,
				 OPTION_DEBUG_FLUSH_BATCHES,
				 FALSE))
		intel->debug_flush |= DEBUG_FLUSH_BATCHES;

	if (xf86ReturnOptValBool(intel->Options,
				 OPTION_DEBUG_FLUSH_CACHES,
				 FALSE))
		intel->debug_flush |= DEBUG_FLUSH_CACHES;

	if (xf86ReturnOptValBool(intel->Options,
				 OPTION_DEBUG_WAIT,
				 FALSE))
		intel->debug_flush |= DEBUG_FLUSH_WAIT;

	return TRUE;
}

static void
I830PreInitCrtcConfig(ScrnInfoPtr scrn)
{
	xf86CrtcConfigPtr   xf86_config;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int max_width, max_height;

	/* check quirks */
	i830_fixup_devices(scrn);

	/* Allocate an xf86CrtcConfig */
	xf86CrtcConfigInit (scrn, &i830_xf86crtc_config_funcs);
	xf86_config = XF86_CRTC_CONFIG_PTR(scrn);

	/* See i830_exa.c comments for why we limit the framebuffer size like
	 * this.
	 */
	if (IS_I965G(intel)) {
		max_height = max_width = min(16384 / intel->cpp, 8192);
	} else {
		max_width = 2048;
		max_height = 2048;
	}
	xf86CrtcSetSizeRange (scrn, 320, 200, max_width, max_height);
}

static void intel_check_dri_option(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	intel->directRenderingType = DRI_NONE;
	if (!xf86ReturnOptValBool(intel->Options, OPTION_DRI, TRUE))
		intel->directRenderingType = DRI_DISABLED;

	if (scrn->depth != 16 && scrn->depth != 24 && scrn->depth != 30) {
		xf86DrvMsg(scrn->scrnIndex, X_CONFIG,
			   "DRI is disabled because it "
			   "runs only at depths 16, 24, and 30.\n");
		intel->directRenderingType = DRI_DISABLED;
	}
}

#ifdef notyet
static void
drm_vblank_handler(int fd, unsigned int frame, unsigned int tv_sec,
    unsigned int tv_usec, void *event_data)
{
	I830DRI2FrameEventHandler(frame, tv_sec, tv_usec, event_data);
}

static void
drm_wakeup_handler(pointer data, int err, pointer p)
{
	intel_screen_private *intel = data;
	fd_set *read_mask = p;

	if (err >= 0 && FD_ISSET(intel->drmSubFD, read_mask))
	    drmHandleEvent(intel->drmSubFD, &intel->event_context);
}
#endif

static Bool i830_user_modesetting_init(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int i, num_pipe;

	if (!I830MapMMIO(scrn))
		return FALSE;

	if (DEVICE_ID(intel->PciInfo) == PCI_CHIP_E7221_G)
		num_pipe = 1;
	else
		if (IS_MOBILE(intel) || IS_I9XX(intel))
			num_pipe = 2;
		else
			num_pipe = 1;
	xf86DrvMsg(scrn->scrnIndex, X_INFO, "%d display pipe%s available.\n",
	    num_pipe, num_pipe > 1 ? "s" : "");

	I830PreInitDDC(scrn);
	for (i = 0; i < num_pipe; i++) {
		i830_crtc_init(scrn, i);
	}
	I830SetupOutputs(scrn);

	SaveHWState(scrn);

	if (!xf86InitialConfiguration (scrn, TRUE)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR, "No valid modes.\n");
		RestoreHWState(scrn);
		PreInitCleanup(scrn);
		return FALSE;
	}
	RestoreHWState(scrn);

	intel->stolen_size = I830DetectMemory(scrn);
#ifdef notyet
	intel->event_context.version = DRM_EVENT_CONTEXT_VERSION;
	intel->event_context.vblank_handler = drm_vblank_handler;
#endif


	return TRUE;
}

static Bool intel_open_drm_master(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct pci_device *dev = intel->PciInfo;
	drmSetVersion sv;
	struct drm_i915_getparam gp;
	int err, has_gem;
	char busid[20];

	snprintf(busid, sizeof(busid), "pci:%04x:%02x:%02x.%d",
		 dev->domain, dev->bus, dev->dev, dev->func);

	intel->drmSubFD = drmOpen("i915", busid);
	if (intel->drmSubFD == -1) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "[drm] Failed to open DRM device for %s: %s\n",
			   busid, strerror(errno));
		return FALSE;
	}

	/* Check that what we opened was a master or a master-capable FD,
	 * by setting the version of the interface we'll use to talk to it.
	 * (see DRIOpenDRMMaster() in DRI1)
	 */
	sv.drm_di_major = 1;
	sv.drm_di_minor = 1;
	sv.drm_dd_major = -1;
	sv.drm_dd_minor = -1;
	err = drmSetInterfaceVersion(intel->drmSubFD, &sv);
	if (err != 0) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "[drm] failed to set drm interface version.\n");
		drmClose(intel->drmSubFD);
		intel->drmSubFD = -1;
		return FALSE;
	}

	has_gem = FALSE;
	gp.param = I915_PARAM_HAS_GEM;
	gp.value = &has_gem;
	(void)drmCommandWriteRead(intel->drmSubFD, DRM_I915_GETPARAM,
				  &gp, sizeof(gp));
	if (!has_gem) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "[drm] Failed to detect GEM.  Kernel 2.6.28 required.\n");
		drmClose(intel->drmSubFD);
		intel->drmSubFD = -1;
		return FALSE;
	}

	return TRUE;
}

static void intel_close_drm_master(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	if (intel && intel->drmSubFD > 0) {
		drmClose(intel->drmSubFD);
		intel->drmSubFD = -1;
	}
}

void intel_init_bufmgr(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int batch_size;

	if (intel->bufmgr)
		return;

	batch_size = 4096 * 4;

	/* The 865 has issues with larger-than-page-sized batch buffers. */
	if (IS_I865G(intel))
		batch_size = 4096;

	intel->bufmgr = drm_intel_bufmgr_gem_init(intel->drmSubFD, batch_size);
	drm_intel_bufmgr_gem_enable_reuse(intel->bufmgr);
	drm_intel_bufmgr_gem_enable_fenced_relocs(intel->bufmgr);

	list_init(&intel->batch_pixmaps);
	list_init(&intel->flush_pixmaps);
	list_init(&intel->in_flight);
}

static Bool I830DrmModeInit(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (drmmode_pre_init(scrn, intel->drmSubFD, intel->cpp) == FALSE) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Kernel modesetting setup failed\n");
		PreInitCleanup(scrn);
		return FALSE;
	}

	intel_init_bufmgr(scrn);

	return TRUE;
}

static void I830XvInit(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	MessageType from = X_PROBED;

	intel->XvPreferOverlay =
	    xf86ReturnOptValBool(intel->Options, OPTION_PREFER_OVERLAY, FALSE);

	if (xf86GetOptValInteger(intel->Options, OPTION_VIDEO_KEY,
				 &(intel->colorKey))) {
		from = X_CONFIG;
	} else if (xf86GetOptValInteger(intel->Options, OPTION_COLOR_KEY,
					&(intel->colorKey))) {
		from = X_CONFIG;
	} else {
		intel->colorKey =
		    (1 << scrn->offset.red) | (1 << scrn->offset.green) |
		    (((scrn->mask.blue >> scrn->offset.blue) - 1) <<
		     scrn->offset.blue);
		from = X_DEFAULT;
	}
	xf86DrvMsg(scrn->scrnIndex, from, "video overlay key set to 0x%x\n",
		   intel->colorKey);
}

static Bool has_kernel_flush(struct intel_screen_private *intel)
{
	drm_i915_getparam_t gp;
	int value;

	/* The BLT ring was introduced at the same time as the
	 * automatic flush for the busy-ioctl.
	 */

	gp.value = &value;
	gp.param = I915_PARAM_HAS_BLT;
	if (drmIoctl(intel->drmSubFD, DRM_IOCTL_I915_GETPARAM, &gp))
		return FALSE;

	return value;
}

static Bool can_accelerate_blt(struct intel_screen_private *intel)
{
	if (0 && (IS_I830(intel) || IS_845G(intel))) {
		/* These pair of i8xx chipsets have a crippling erratum
		 * that prevents the use of a PTE entry by the BLT
		 * engine immediately following updating that
		 * entry in the GATT.
		 *
		 * As the BLT is fundamental to our 2D acceleration,
		 * and the workaround is lost in the midst of time,
		 * fallback.
		 *
		 * XXX disabled for release as causes regressions in GL.
		 */
		return FALSE;
	}

	if (INTEL_INFO(intel)->gen >= 60) {
		drm_i915_getparam_t gp;
		int value;

		/* On Sandybridge we need the BLT in order to do anything since
		 * it so frequently used in the acceleration code paths.
		 */
		gp.value = &value;
		gp.param = I915_PARAM_HAS_BLT;
		if (drmIoctl(intel->drmSubFD, DRM_IOCTL_I915_GETPARAM, &gp))
			return FALSE;
	}

	if (INTEL_INFO(intel)->gen == 60) {
		struct pci_device *const device = intel->PciInfo;

		/* Sandybridge rev07 locks up easily, even with the
		 * BLT ring workaround in place.
		 * Thus use shadowfb by default.
		 */
		if (device->revision < 8)
		    return FALSE;
	}

	return TRUE;
}

/**
 * This is called before ScreenInit to do any require probing of screen
 * configuration.
 *
 * This code generally covers probing, module loading, option handling
 * card mapping, and RandR setup.
 *
 * Since xf86InitialConfiguration ends up requiring that we set video modes
 * in order to detect configuration, we end up having to do a lot of driver
 * setup (talking to the DRM, mapping the device, etc.) in this function.
 * As a result, we want to set up that server initialization once rather
 * that doing it per generation.
 */
static Bool I830PreInit(ScrnInfoPtr scrn, int flags)
{
	vgaHWPtr hwp;
	intel_screen_private *intel;
	rgb defaultWeight = { 0, 0, 0 };
	EntityInfoPtr pEnt;
	int flags24;
	Gamma zeros = { 0.0, 0.0, 0.0 };
	int drm_mode_setting;

	if (scrn->numEntities != 1)
		return FALSE;

	drm_mode_setting = intel_kernel_mode_enabled(scrn);

	pEnt = xf86GetEntityInfo(scrn->entityList[0]);

	if (flags & PROBE_DETECT)
		return TRUE;

	/* Allocate driverPrivate */
	if (!I830GetRec(scrn))
		return FALSE;

	intel = intel_get_screen_private(scrn);
	intel->SaveGeneration = -1;
	intel->pEnt = pEnt;
	intel->scrn = scrn;
	intel->use_drm_mode = drm_mode_setting;

	if (!I830LoadSyms(scrn))
		return FALSE;

	if (!drm_mode_setting) {
		/* Allocate a vgaHWRec */
		if (!vgaHWGetHWRec(scrn))
			return FALSE;
		hwp = VGAHWPTR(scrn);
		vgaHWSetStdFuncs(hwp);
	}

	scrn->displayWidth = 640;	/* default it */

	if (intel->pEnt->location.type != BUS_PCI)
		return FALSE;

	intel->PciInfo = xf86GetPciInfoForEntity(intel->pEnt->index);

	if (!intel_open_drm_master(scrn))
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Failed to become DRM master.\n");

	scrn->monitor = scrn->confScreen->monitor;
	scrn->progClock = TRUE;
	scrn->rgbBits = 8;

	flags24 = Support32bppFb | PreferConvert24to32 | SupportConvert24to32;

	if (!xf86SetDepthBpp(scrn, 0, 0, 0, flags24))
		return FALSE;

	switch (scrn->depth) {
	case 8:
	case 15:
	case 16:
	case 24:
	case 30:
		break;
	default:
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Given depth (%d) is not supported by I830 driver\n",
			   scrn->depth);
		return FALSE;
	}
	xf86PrintDepthBpp(scrn);

	if (!xf86SetWeight(scrn, defaultWeight, defaultWeight))
		return FALSE;
	if (!xf86SetDefaultVisual(scrn, -1))
		return FALSE;

	if (!intel->use_drm_mode)
		hwp = VGAHWPTR(scrn);

	intel->cpp = scrn->bitsPerPixel / 8;

	if (!I830GetEarlyOptions(scrn))
		return FALSE;

	if (!intel_check_chipset_option(scrn))
		return FALSE;

	intel_check_dri_option(scrn);

	if (intel->use_drm_mode) {
		if (!I830DrmModeInit(scrn))
			return FALSE;
	} else {
		if (i830_bios_init(scrn))
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			    "VBIOS initialization failed.\n");
		I830PreInitCrtcConfig(scrn);
		if (!i830_user_modesetting_init(scrn))
			return FALSE;
	}

	I830XvInit(scrn);

	if (!xf86SetGamma(scrn, zeros)) {
		PreInitCleanup(scrn);
		return FALSE;
	}

	if (scrn->modes == NULL) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR, "No modes.\n");
		PreInitCleanup(scrn);
		return FALSE;
	}
	scrn->currentMode = scrn->modes;

	/* Set display resolution */
	xf86SetDpi(scrn, 0, 0);

	/* Load the required sub modules */
	if (!xf86LoadSubModule(scrn, "fb")) {
		PreInitCleanup(scrn);
		return FALSE;
	}

	if (!intel->use_drm_mode) {
		/* console restore hack */
		if (HAS_PCH_SPLIT(intel)) {
		    if (xf86LoadSubModule(scrn, "int10")) {
			intel->int10 = xf86InitInt10(pEnt->index);
			if (intel->int10) {
			    intel->int10->num = 0x10;
			    intel->int10->ax = 0x4f03;
			    intel->int10->bx =
			    intel->int10->cx =
			    intel->int10->dx = 0;
			    xf86ExecX86int10(intel->int10);
			    intel->int10Mode = intel->int10->bx & 0x3fff;
			    xf86DrvMsg(scrn->scrnIndex, X_PROBED,
				"Console VGA mode is 0x%x\n", intel->int10Mode);
			} else {
			    xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				"Failed int10 setup, VT switch won't work\n");
			}
		    } else {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			    "Failed to load int10module, ironlake vt switch broken");
		    }
		}

		I830UnmapMMIO(scrn);

		/*  We won't be using the VGA access after the probe. */
		I830SetMMIOAccess(intel);
	}

	/* Load the dri2 module if requested. */
	if (xf86ReturnOptValBool(intel->Options, OPTION_DRI, FALSE) &&
	    intel->directRenderingType != DRI_DISABLED) {
		xf86LoadSubModule(scrn, "dri2");
	}

	return TRUE;
}

enum pipe {
	PIPE_A = 0,
	PIPE_B,
};

static Bool i830_pipe_enabled(intel_screen_private *intel, enum pipe pipe)
{
	uint32_t dpll_reg;

	if (HAS_PCH_SPLIT(intel)) {
		dpll_reg = (pipe == PIPE_A) ? PCH_DPLL_A : PCH_DPLL_B;
	} else {
		dpll_reg = (pipe == PIPE_A) ? DPLL_A : DPLL_B;
	}

	return (INREG(dpll_reg) & DPLL_VCO_ENABLE);
}

static void i830_save_palette(intel_screen_private *intel, enum pipe pipe)
{
	uint32_t reg = (pipe == PIPE_A ? PALETTE_A : PALETTE_B);
	uint32_t *array;
	int i;

	if (!i830_pipe_enabled(intel, pipe))
		return;

	if (HAS_PCH_SPLIT(intel))
		reg = (pipe == PIPE_A) ? LGC_PALETTE_A : LGC_PALETTE_B;

	if (pipe == PIPE_A)
		array = intel->savePaletteA;
	else
		array = intel->savePaletteB;

	for (i = 0; i < 256; i++)
		array[i] = INREG(reg + (i << 2));
}

static void i830_restore_palette(intel_screen_private *intel, enum pipe pipe)
{
	uint32_t reg = (pipe == PIPE_A ? PALETTE_A : PALETTE_B);
	uint32_t *array;
	int i;

	if (!i830_pipe_enabled(intel, pipe))
		return;

	if (HAS_PCH_SPLIT(intel))
		reg = (pipe == PIPE_A) ? LGC_PALETTE_A : LGC_PALETTE_B;

	if (pipe == PIPE_A)
		array = intel->savePaletteA;
	else
		array = intel->savePaletteB;

	for (i = 0; i < 256; i++)
		OUTREG(reg + (i << 2), array[i]);
}

static Bool SaveHWState(ScrnInfoPtr scrn)
{
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	vgaHWPtr hwp = VGAHWPTR(scrn);
	vgaRegPtr vgaReg = &hwp->SavedReg;
	int i;

	if (HAS_PCH_SPLIT(intel)) {
		for (i = 0; i < xf86_config->num_output; i++) {
			xf86OutputPtr   output = xf86_config->output[i];
			if (output->funcs->save)
				(*output->funcs->save) (output);
		}

		return TRUE;
	}

	/* Save video mode information for native mode-setting. */
	if (!DSPARB_HWCONTROL(intel))
		intel->saveDSPARB = INREG(DSPARB);

	intel->saveDSPACNTR = INREG(DSPACNTR);
	intel->savePIPEACONF = INREG(PIPEACONF);
	intel->savePIPEASRC = INREG(PIPEASRC);
	intel->saveFPA0 = INREG(FPA0);
	intel->saveFPA1 = INREG(FPA1);
	intel->saveDPLL_A = INREG(DPLL_A);
	if (IS_I965G(intel))
		intel->saveDPLL_A_MD = INREG(DPLL_A_MD);
	intel->saveHTOTAL_A = INREG(HTOTAL_A);
	intel->saveHBLANK_A = INREG(HBLANK_A);
	intel->saveHSYNC_A = INREG(HSYNC_A);
	intel->saveVTOTAL_A = INREG(VTOTAL_A);
	intel->saveVBLANK_A = INREG(VBLANK_A);
	intel->saveVSYNC_A = INREG(VSYNC_A);
	intel->saveBCLRPAT_A = INREG(BCLRPAT_A);
	intel->saveDSPASTRIDE = INREG(DSPASTRIDE);
	intel->saveDSPASIZE = INREG(DSPASIZE);
	intel->saveDSPAPOS = INREG(DSPAPOS);
	intel->saveDSPABASE = INREG(DSPABASE);

	i830_save_palette(intel, PIPE_A);

	if (xf86_config->num_crtc == 2) {
		intel->savePIPEBCONF = INREG(PIPEBCONF);
		intel->savePIPEBSRC = INREG(PIPEBSRC);
		intel->saveDSPBCNTR = INREG(DSPBCNTR);
		intel->saveFPB0 = INREG(FPB0);
		intel->saveFPB1 = INREG(FPB1);
		intel->saveDPLL_B = INREG(DPLL_B);
		if (IS_I965G(intel))
			intel->saveDPLL_B_MD = INREG(DPLL_B_MD);
		intel->saveHTOTAL_B = INREG(HTOTAL_B);
		intel->saveHBLANK_B = INREG(HBLANK_B);
		intel->saveHSYNC_B = INREG(HSYNC_B);
		intel->saveVTOTAL_B = INREG(VTOTAL_B);
		intel->saveVBLANK_B = INREG(VBLANK_B);
		intel->saveVSYNC_B = INREG(VSYNC_B);
		intel->saveBCLRPAT_B = INREG(BCLRPAT_B);
		intel->saveDSPBSTRIDE = INREG(DSPBSTRIDE);
		intel->saveDSPBSIZE = INREG(DSPBSIZE);
		intel->saveDSPBPOS = INREG(DSPBPOS);
		intel->saveDSPBBASE = INREG(DSPBBASE);

		i830_save_palette(intel, PIPE_B);
	}

	if (IS_I965G(intel)) {
		intel->saveDSPASURF = INREG(DSPASURF);
		intel->saveDSPBSURF = INREG(DSPBSURF);
		intel->saveDSPATILEOFF = INREG(DSPATILEOFF);
		intel->saveDSPBTILEOFF = INREG(DSPBTILEOFF);
	}

	intel->saveVCLK_DIVISOR_VGA0 = INREG(VCLK_DIVISOR_VGA0);
	intel->saveVCLK_DIVISOR_VGA1 = INREG(VCLK_DIVISOR_VGA1);
	intel->saveVCLK_POST_DIV = INREG(VCLK_POST_DIV);
	intel->saveVGACNTRL = INREG(VGACNTRL);

	intel->saveCURSOR_A_CONTROL = INREG(CURSOR_A_CONTROL);
	intel->saveCURSOR_A_POSITION = INREG(CURSOR_A_POSITION);
	intel->saveCURSOR_A_BASE = INREG(CURSOR_A_BASE);
	intel->saveCURSOR_B_CONTROL = INREG(CURSOR_B_CONTROL);
	intel->saveCURSOR_B_POSITION = INREG(CURSOR_B_POSITION);
	intel->saveCURSOR_B_BASE = INREG(CURSOR_B_BASE);

	for(i = 0; i < 7; i++) {
		intel->saveSWF[i] = INREG(SWF0 + (i << 2));
		intel->saveSWF[i+7] = INREG(SWF00 + (i << 2));
	}
	intel->saveSWF[14] = INREG(SWF30);
	intel->saveSWF[15] = INREG(SWF31);
	intel->saveSWF[16] = INREG(SWF32);

	intel->saveDSPCLK_GATE_D = INREG(DSPCLK_GATE_D);
	intel->saveRENCLK_GATE_D1 = INREG(RENCLK_GATE_D1);

	if (IS_I965G(intel)) {
		intel->saveRENCLK_GATE_D2 = INREG(RENCLK_GATE_D2);
		intel->saveRAMCLK_GATE_D = INREG(RAMCLK_GATE_D);
	}

	if (IS_I965GM(intel) || IS_GM45(intel))
		intel->savePWRCTXA = INREG(PWRCTXA);

	if (IS_MOBILE(intel) && !IS_I830(intel)) {
		if (HAS_PCH_SPLIT(intel))
			intel->saveLVDS = INREG(PCH_LVDS);
		else
			intel->saveLVDS = INREG(LVDS);
	}
	intel->savePFIT_CONTROL = INREG(PFIT_CONTROL);

	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr   output = xf86_config->output[i];
		if (output->funcs->save)
			(*output->funcs->save) (output);
	}

	vgaHWUnlock(hwp);
	vgaHWSave(scrn, vgaReg, VGA_SR_FONTS);

	return TRUE;
}

/* Wait for the PLL to settle down after programming */
static void i830_dpll_settle(void)
{
	usleep(10000); /* 10 ms *should* be plenty */
}

static Bool RestoreHWState(ScrnInfoPtr scrn)
{
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	vgaHWPtr hwp = VGAHWPTR(scrn);
	vgaRegPtr vgaReg = &hwp->SavedReg;
	int i;

	if (HAS_PCH_SPLIT(intel)) {
		/* Restore outputs */
		for (i = 0; i < xf86_config->num_output; i++) {
			xf86OutputPtr   output = xf86_config->output[i];
			if (output->funcs->restore)
				output->funcs->restore(output);
		}

		return TRUE;
	}

	DPRINTF(PFX, "RestoreHWState\n");

	/* Disable outputs */
	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr   output = xf86_config->output[i];
		output->funcs->dpms(output, DPMSModeOff);
	}
	i830WaitForVblank(scrn);
   
	/* Disable pipes */
	for (i = 0; i < xf86_config->num_crtc; i++) {
		xf86CrtcPtr crtc = xf86_config->crtc[i];
		i830_crtc_disable(crtc, TRUE);
	}
	i830WaitForVblank(scrn);

	if (IS_MOBILE(intel) && !IS_I830(intel))
		OUTREG(LVDS, intel->saveLVDS);

	if (!IS_I830(intel) && !IS_845G(intel))
		OUTREG(PFIT_CONTROL, intel->savePFIT_CONTROL);

	if (!DSPARB_HWCONTROL(intel))
		OUTREG(DSPARB, intel->saveDSPARB);

	OUTREG(DSPCLK_GATE_D, intel->saveDSPCLK_GATE_D);
	OUTREG(RENCLK_GATE_D1, intel->saveRENCLK_GATE_D1);

	if (IS_I965G(intel)) {
		OUTREG(RENCLK_GATE_D2, intel->saveRENCLK_GATE_D2);
		OUTREG(RAMCLK_GATE_D, intel->saveRAMCLK_GATE_D);
	}

	if (IS_I965GM(intel) || IS_GM45(intel))
		OUTREG(PWRCTXA, intel->savePWRCTXA);

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
	OUTREG(VCLK_DIVISOR_VGA0, intel->saveVCLK_DIVISOR_VGA0);
	OUTREG(VCLK_DIVISOR_VGA1, intel->saveVCLK_DIVISOR_VGA1);
	OUTREG(VCLK_POST_DIV, intel->saveVCLK_POST_DIV);

	/* If the pipe A PLL is active, we can restore the pipe & plane config */
	if (intel->saveDPLL_A & DPLL_VCO_ENABLE) {
		OUTREG(FPA0, intel->saveFPA0);
		OUTREG(DPLL_A, intel->saveDPLL_A & ~DPLL_VCO_ENABLE);
		POSTING_READ(DPLL_A);
		usleep(150);
	}
	OUTREG(FPA0, intel->saveFPA0);
	OUTREG(FPA1, intel->saveFPA1);
	OUTREG(DPLL_A, intel->saveDPLL_A);
	POSTING_READ(DPLL_A);
	i830_dpll_settle();
	if (IS_I965G(intel))
		OUTREG(DPLL_A_MD, intel->saveDPLL_A_MD);
	else
		OUTREG(DPLL_A, intel->saveDPLL_A);
	POSTING_READ(DPLL_A);
	i830_dpll_settle();

	/* Restore mode config */
	OUTREG(HTOTAL_A, intel->saveHTOTAL_A);
	OUTREG(HBLANK_A, intel->saveHBLANK_A);
	OUTREG(HSYNC_A, intel->saveHSYNC_A);
	OUTREG(VTOTAL_A, intel->saveVTOTAL_A);
	OUTREG(VBLANK_A, intel->saveVBLANK_A);
	OUTREG(VSYNC_A, intel->saveVSYNC_A);
	OUTREG(BCLRPAT_A, intel->saveBCLRPAT_A);

	OUTREG(DSPASTRIDE, intel->saveDSPASTRIDE);
	OUTREG(DSPASIZE, intel->saveDSPASIZE);
	OUTREG(DSPAPOS, intel->saveDSPAPOS);
	OUTREG(PIPEASRC, intel->savePIPEASRC);
	OUTREG(DSPABASE, intel->saveDSPABASE);
	if (IS_I965G(intel)) {
		OUTREG(DSPASURF, intel->saveDSPASURF);
		OUTREG(DSPATILEOFF, intel->saveDSPATILEOFF);
	}

	OUTREG(PIPEACONF, intel->savePIPEACONF);
	POSTING_READ(PIPEACONF);
	i830WaitForVblank(scrn);

	/*
	 * Program Pipe A's plane
	 * The corresponding display plane may be disabled, and should only be
	 * enabled if pipe A is actually on (otherwise we have a bug in the initial
	 * state).
	 */
	if ((intel->saveDSPACNTR & DISPPLANE_SEL_PIPE_MASK) ==
		DISPPLANE_SEL_PIPE_A) {
		OUTREG(DSPACNTR, intel->saveDSPACNTR);
		OUTREG(DSPABASE, INREG(DSPABASE));
		POSTING_READ(DSPABASE);
		i830WaitForVblank(scrn);
	}
	if ((intel->saveDSPBCNTR & DISPPLANE_SEL_PIPE_MASK) ==
	    DISPPLANE_SEL_PIPE_A) {
		OUTREG(DSPBCNTR, intel->saveDSPBCNTR);
		OUTREG(DSPBBASE, INREG(DSPBBASE));
		POSTING_READ(DSPBBASE);
		i830WaitForVblank(scrn);
	}

	/* See note about pipe programming above */
	if(xf86_config->num_crtc == 2) {
		/* If the pipe B PLL is active, we can restore the pipe
		 * & plane config
		 */
		if (intel->saveDPLL_B & DPLL_VCO_ENABLE) {
			OUTREG(FPB0, intel->saveFPB0);
			OUTREG(DPLL_B, intel->saveDPLL_B & ~DPLL_VCO_ENABLE);
			POSTING_READ(DPLL_B);
			usleep(150);
		}
		OUTREG(FPB0, intel->saveFPB0);
		OUTREG(FPB1, intel->saveFPB1);
		OUTREG(DPLL_B, intel->saveDPLL_B);
		POSTING_READ(DPLL_B);
		i830_dpll_settle();
		if (IS_I965G(intel))
			OUTREG(DPLL_B_MD, intel->saveDPLL_B_MD);
		else
			OUTREG(DPLL_B, intel->saveDPLL_B);
		POSTING_READ(DPLL_B);
		i830_dpll_settle();
   
		/* Restore mode config */
		OUTREG(HTOTAL_B, intel->saveHTOTAL_B);
		OUTREG(HBLANK_B, intel->saveHBLANK_B);
		OUTREG(HSYNC_B, intel->saveHSYNC_B);
		OUTREG(VTOTAL_B, intel->saveVTOTAL_B);
		OUTREG(VBLANK_B, intel->saveVBLANK_B);
		OUTREG(VSYNC_B, intel->saveVSYNC_B);
		OUTREG(BCLRPAT_B, intel->saveBCLRPAT_B);
		OUTREG(DSPBSTRIDE, intel->saveDSPBSTRIDE);
		OUTREG(DSPBSIZE, intel->saveDSPBSIZE);
		OUTREG(DSPBPOS, intel->saveDSPBPOS);
		OUTREG(PIPEBSRC, intel->savePIPEBSRC);
		OUTREG(DSPBBASE, intel->saveDSPBBASE);
		if (IS_I965G(intel)) {
			OUTREG(DSPBSURF, intel->saveDSPBSURF);
			OUTREG(DSPBTILEOFF, intel->saveDSPBTILEOFF);
		}

		OUTREG(PIPEBCONF, intel->savePIPEBCONF);
		POSTING_READ(PIPEBCONF);
		i830WaitForVblank(scrn);

		/*
		 * Program Pipe B's plane
		 * Note that pipe B may be disabled, and in that case, the plane
		 * should also be disabled or we must have had a bad initial state.
		 */
		if ((intel->saveDSPACNTR & DISPPLANE_SEL_PIPE_MASK) ==
		    DISPPLANE_SEL_PIPE_B) {
			OUTREG(DSPACNTR, intel->saveDSPACNTR);
			OUTREG(DSPABASE, INREG(DSPABASE));
			i830WaitForVblank(scrn);
		}
		if ((intel->saveDSPBCNTR & DISPPLANE_SEL_PIPE_MASK) ==
		    DISPPLANE_SEL_PIPE_B) {
			OUTREG(DSPBCNTR, intel->saveDSPBCNTR);
			OUTREG(DSPBBASE, INREG(DSPBBASE));
			i830WaitForVblank(scrn);
		}
	}

	OUTREG(VGACNTRL, intel->saveVGACNTRL);

	/*
	 * Restore cursors
	 * Even though the X cursor is hidden before we restore the hw state,
	 * we probably only disabled one cursor plane.  If we're going from
	 * e.g. plane b to plane a here in RestoreHWState, we need to restore
	 * both cursor plane settings.
	 */
	OUTREG(CURSOR_A_POSITION, intel->saveCURSOR_A_POSITION);
	OUTREG(CURSOR_A_BASE, intel->saveCURSOR_A_BASE);
	OUTREG(CURSOR_A_CONTROL, intel->saveCURSOR_A_CONTROL);
	OUTREG(CURSOR_B_POSITION, intel->saveCURSOR_B_POSITION);
	OUTREG(CURSOR_B_BASE, intel->saveCURSOR_B_BASE);
	OUTREG(CURSOR_B_CONTROL, intel->saveCURSOR_B_CONTROL);

	/* Restore outputs */
	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr   output = xf86_config->output[i];
		if (output->funcs->restore)
			output->funcs->restore(output);
	}

	i830_restore_palette(intel, PIPE_A);
	i830_restore_palette(intel, PIPE_B);

	for(i = 0; i < 7; i++) {
		OUTREG(SWF0 + (i << 2), intel->saveSWF[i]);
		OUTREG(SWF00 + (i << 2), intel->saveSWF[i+7]);
	}

	OUTREG(SWF30, intel->saveSWF[14]);
	OUTREG(SWF31, intel->saveSWF[15]);
	OUTREG(SWF32, intel->saveSWF[16]);

	vgaHWRestore(scrn, vgaReg, VGA_SR_FONTS);
	vgaHWLock(hwp);

	return TRUE;
}

/**
 * Intialiazes the hardware for the 3D pipeline use in the 2D driver.
 *
 * Some state caching is performed to avoid redundant state emits.  This
 * function is also responsible for marking the state as clobbered for DRI
 * clients.
 */
void IntelEmitInvarientState(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* If we've emitted our state since the last clobber by another client,
	 * skip it.
	 */
	if (intel->last_3d != LAST_3D_OTHER)
		return;

	if (IS_GEN2(intel))
		I830EmitInvarientState(scrn);
	else if IS_GEN3(intel)
		I915EmitInvarientState(scrn);
}

static void
I830BlockHandler(BLOCKHANDLER_ARGS_DECL)
{
	SCREEN_PTR(arg);
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	screen->BlockHandler = intel->BlockHandler;

	(*screen->BlockHandler) (BLOCKHANDLER_ARGS);

	intel->BlockHandler = screen->BlockHandler;
	screen->BlockHandler = I830BlockHandler;

	intel_uxa_block_handler(intel);
	intel_video_block_handler(scrn);
}

static void intel_fixup_mtrrs(ScrnInfoPtr scrn)
{
#ifdef HAS_MTRR_SUPPORT
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int fd;
	struct mtrr_gentry gentry;
	struct mtrr_sentry sentry;

	if ((fd = open("/proc/mtrr", O_RDONLY, 0)) != -1) {
		for (gentry.regnum = 0;
		     ioctl(fd, MTRRIOC_GET_ENTRY, &gentry) == 0;
		     ++gentry.regnum) {

			if (gentry.size < 1) {
				/* DISABLED */
				continue;
			}

			/* Check the MTRR range is one we like and if not - remove it.
			 * The Xserver common layer will then setup the right range
			 * for us.
			 */
			if (gentry.base == intel->LinearAddr &&
			    gentry.size < intel->FbMapSize) {

				xf86DrvMsg(scrn->scrnIndex, X_INFO,
					   "Removing bad MTRR range (base 0x%lx, size 0x%x)\n",
					   gentry.base, gentry.size);

				sentry.base = gentry.base;
				sentry.size = gentry.size;
				sentry.type = gentry.type;

				if (ioctl(fd, MTRRIOC_DEL_ENTRY, &sentry) == -1) {
					xf86DrvMsg(scrn->scrnIndex, X_ERROR,
						   "Failed to remove bad MTRR range\n");
				}
			}
		}
		close(fd);
	}
#endif
}

/*
 * Try to allocate memory for rendering
 */
static Bool i830_memory_init(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Set up our video memory allocator for the chosen videoRam */
	if (!i830_allocator_init(scrn, scrn->videoRam * KB(1))) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Couldn't initialize video memory allocator\n");
		PreInitCleanup(scrn);
		return FALSE;
	}

	xf86DrvMsg(scrn->scrnIndex,
		   intel->pEnt->device->videoRam ? X_CONFIG : X_DEFAULT,
		   "VideoRam: %d KB\n", scrn->videoRam);

	if (!i830_allocate_2d_memory(scrn))
		return FALSE;

	if (IS_I965GM(intel) || IS_GM45(intel))
		if (!i830_allocate_pwrctx(scrn))
			return FALSE;
	return TRUE;
}

Bool intel_crtc_on(xf86CrtcPtr crtc)
{
	ScrnInfoPtr scrn = crtc->scrn;
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->use_drm_mode) {
		int i, active_outputs = 0;

		if (!crtc->enabled)
			return FALSE;

		/* Kernel manages CRTC status based out output config */
		for (i = 0; i < xf86_config->num_output; i++) {
			xf86OutputPtr output = xf86_config->output[i];
			if (output->crtc == crtc &&
			    drmmode_output_dpms_status(output) == DPMSModeOn)
				active_outputs++;
		}

		if (active_outputs)
			return TRUE;
		return FALSE;
	} else {
		I830CrtcPrivatePtr intel_crtc = crtc->driver_private;

		if (!crtc->enabled)
			return FALSE;

		if (intel_crtc->dpms_mode == DPMSModeOn)
			return TRUE;
		return FALSE;
	}
}

int intel_crtc_to_pipe(xf86CrtcPtr crtc)
{
	ScrnInfoPtr scrn = crtc->scrn;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int pipe;

	if (intel->use_drm_mode) {
		pipe = drmmode_get_pipe_from_crtc_id(intel->bufmgr, crtc);
	} else {
		I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
		pipe = intel_crtc->pipe;
	}

	return pipe;
}

static void
I830AdjustMemory(ScreenPtr screen)
{
	ScrnInfoPtr scrn;
	intel_screen_private *intel;
	unsigned long sys_mem;
	MessageType from;

	scrn = xf86Screens[screen->myNum];
	intel = intel_get_screen_private(scrn);

	/* Limit videoRam to how much we might be able to allocate from AGP */
	sys_mem = I830CheckAvailableMemory(scrn);
	if (sys_mem == -1) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		    "/dev/agpgart is either not available, or no memory "
		    "is available\nfor allocation.  Please enable agpgart\n.");
		scrn->videoRam = intel->stolen_size / KB(1);
	}
	if (sys_mem + (intel->stolen_size / 1024) < scrn->videoRam) {
		scrn->videoRam = sys_mem + (intel->stolen_size / 1024);
		from = X_PROBED;
		if (sys_mem + (intel->stolen_size / 1024) <
		    intel->pEnt->device->videoRam) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			    "VideoRAM reduced to %d kByte "
			    "(limited to available sysmem)\n", scrn->videoRam);
		}
	}

	/* Limit video RAM to the actual aperture size */
	if (scrn->videoRam > intel->FbMapSize / 1024) {
		scrn->videoRam = intel->FbMapSize / 1024;
		if (intel->FbMapSize / 1024 < intel->pEnt->device->videoRam) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			    "VideoRam reduced to %d kByte (limited to aperture "
			    "size)\n", scrn->videoRam);
		}
	}

	/* Make sure it's on a page boundary */
	if (scrn->videoRam & 3) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING, "VideoRam reduced to "
		    "%d KB (page aligned - was %d KB)\n", scrn->videoRam & ~3,
		    scrn->videoRam);
		scrn->videoRam &= ~3;
	}

	if (!IS_I965G(intel) && scrn->displayWidth > 2048) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		    "Cannot support DRI with frame buffer width > 2048.\n");
		intel->directRenderingType = DRI_DISABLED;
	}
}

static void
i830_disable_render_standby(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	uint32_t render_standby;

	/* Render Standby might cause hang issue, try always disable it.*/
	if (IS_I965GM(intel) || IS_GM45(intel)) {
		render_standby = INREG(MCHBAR_RENDER_STANDBY);
		if (render_standby & RENDER_STANDBY_ENABLE) {
			xf86DrvMsg(scrn->scrnIndex, X_INFO, "Disable render standby.\n");
			    OUTREG(MCHBAR_RENDER_STANDBY,
			    (render_standby & (~RENDER_STANDBY_ENABLE)));
	       }
	}
}

static void
intel_flush_callback(CallbackListPtr *list,
		     pointer user_data, pointer call_data)
{
	ScrnInfoPtr scrn = user_data;
	if (scrn->vtSema)
		intel_batch_submit(scrn);
}

static Bool
I830ScreenInit(SCREEN_INIT_ARGS_DECL)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	vgaHWPtr hwp = NULL;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	VisualPtr visual;
	MessageType from;

	if (!intel->use_drm_mode)
		hwp = VGAHWPTR(scrn);

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
	if (intel->pEnt->device->videoRam == 0) {
		from = X_DEFAULT;
		scrn->videoRam = intel->FbMapSize / KB(1);
	} else {
#if 0
		from = X_CONFIG;
		scrn->videoRam = intel->pEnt->device->videoRam;
#else
		/* Disable VideoRam configuration, at least for now.  Previously,
		 * VideoRam was necessary to avoid overly low limits on allocated
		 * memory, so users created larger, yet still small, fixed allocation
		 * limits in their config files.  Now, the driver wants to allocate more,
		 * and the old intention of the VideoRam lines that had been entered is
		 * obsolete.
		 */
		from = X_DEFAULT;
		scrn->videoRam = intel->FbMapSize / KB(1);

		if (scrn->videoRam != intel->pEnt->device->videoRam) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "VideoRam configuration found, which is no longer "
				   "recommended.\n");
			xf86DrvMsg(scrn->scrnIndex, X_INFO,
				   "Continuing with default %dkB VideoRam instead of %d "
				   "kB.\n",
				   scrn->videoRam,
				   intel->pEnt->device->videoRam);
		}
#endif
	}

	if (intel->use_drm_mode) {
		struct pci_device *const device = intel->PciInfo;
		int fb_bar = IS_GEN2(intel) ? 0 : 2;

		scrn->videoRam = device->regions[fb_bar].size / 1024;
	} else {
		I830AdjustMemory(screen);
	}

#ifdef DRI2
	if (intel->directRenderingType == DRI_NONE
	    && I830DRI2ScreenInit(screen))
		intel->directRenderingType = DRI_DRI2;
#endif

	intel->force_fallback = FALSE;
	intel->can_blt = can_accelerate_blt(intel);
#if 0 /* XXX oga */
	intel->has_kernel_flush = has_kernel_flush(intel);
#endif
	intel->has_kernel_flush = TRUE;
	intel->use_shadow = !intel->can_blt;

	/* Enable tiling by default */
	intel->tiling = INTEL_TILING_ALL;

	/* Allow user override if they set a value */
	if (!xf86ReturnOptValBool(intel->Options, OPTION_TILING_2D, TRUE))
		intel->tiling &= ~INTEL_TILING_2D;
	if (xf86ReturnOptValBool(intel->Options, OPTION_TILING_FB, FALSE))
		intel->tiling &= ~INTEL_TILING_FB;

	if (xf86IsOptionSet(intel->Options, OPTION_SHADOW)) {
		if (xf86ReturnOptValBool(intel->Options, OPTION_SHADOW, FALSE))
			intel->use_shadow = TRUE;
	}

	if (intel->use_shadow) {
		xf86DrvMsg(scrn->scrnIndex, X_CONFIG,
			   "Shadow buffer enabled,"
			   " 2D GPU acceleration disabled.\n");
	}

	/* SwapBuffers delays to avoid tearing */
	intel->swapbuffers_wait = TRUE;

	/* Allow user override if they set a value */
	if (xf86IsOptionSet(intel->Options, OPTION_SWAPBUFFERS_WAIT)) {
		if (xf86ReturnOptValBool
		    (intel->Options, OPTION_SWAPBUFFERS_WAIT, FALSE))
			intel->swapbuffers_wait = TRUE;
		else
			intel->swapbuffers_wait = FALSE;
	}

	xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "Framebuffer %s\n",
		   intel->tiling & INTEL_TILING_FB ? "tiled" : "linear");
	xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "Pixmaps %s\n",
		   intel->tiling & INTEL_TILING_2D ? "tiled" : "linear");
	xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "3D buffers %s\n",
		   intel->tiling & INTEL_TILING_3D ? "tiled" : "linear");
	xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "SwapBuffers wait %sabled\n",
		   intel->swapbuffers_wait ? "en" : "dis");

	intel->last_3d = LAST_3D_OTHER;
	intel->overlayOn = FALSE;

	/*
	 * Set this so that the overlay allocation is factored in when
	 * appropriate.
	 */
	intel->XvEnabled = TRUE;

	/* Need MMIO mapped to do GTT lookups during memory allocation. */
	if (!intel->use_drm_mode)
		I830MapMMIO(scrn);

	/* Need FB mapped to access non-GEM objects like
	 * a UMS frame buffer, or the fake bufmgr.
	 */
	if (!intel->use_drm_mode) {
		if (!I830MapMem(scrn))
			return FALSE;
		scrn->memPhysBase = (unsigned long)intel->FbBase;
	}

	if (!i830_memory_init(scrn)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Couldn't allocate video memory\n");
		return FALSE;
	}

	intel_fixup_mtrrs(scrn);

	intel_batch_init(scrn);

	if (INTEL_INFO(intel)->gen >= 40)
		gen4_render_state_init(scrn);

	miClearVisualTypes();
	if (!miSetVisualTypes(scrn->depth,
			      miGetDefaultVisualMask(scrn->depth),
			      scrn->rgbBits, scrn->defaultVisual))
		return FALSE;
	if (!miSetPixmapDepths())
		return FALSE;

	if (!intel->use_drm_mode) {
		vgaHWSetMmioFuncs(hwp, intel->MMIOBase, 0);
		vgaHWGetIOBase(hwp);
		DPRINTF(PFX, "assert( if(!vgaHWMapMem(scrn)) )\n");
		if (!vgaHWMapMem(scrn))
			return FALSE;
	}

	DPRINTF(PFX, "assert( if(!I830EnterVT(scrnIndex, 0)) )\n");

	DPRINTF(PFX, "assert( if(!fbScreenInit(screen, ...) )\n");
	if (!fbScreenInit(screen, NULL,
			  scrn->virtualX, scrn->virtualY,
			  scrn->xDpi, scrn->yDpi,
			  scrn->displayWidth, scrn->bitsPerPixel))
		return FALSE;

	if (scrn->bitsPerPixel > 8) {
		/* Fixup RGB ordering */
		visual = screen->visuals + screen->numVisuals;
		while (--visual >= screen->visuals) {
			if ((visual->class | DynamicClass) == DirectColor) {
				visual->offsetRed = scrn->offset.red;
				visual->offsetGreen = scrn->offset.green;
				visual->offsetBlue = scrn->offset.blue;
				visual->redMask = scrn->mask.red;
				visual->greenMask = scrn->mask.green;
				visual->blueMask = scrn->mask.blue;
			}
		}
	}

	fbPictureInit(screen, NULL, 0);

	xf86SetBlackWhitePixels(screen);

	if (!intel_uxa_init(screen)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Hardware acceleration initialization failed\n");
		return FALSE;
	}

	miInitializeBackingStore(screen);
	xf86SetBackingStore(screen);
	xf86SetSilkenMouse(screen);
	miDCInitialize(screen, xf86GetPointerScreenFuncs());

	xf86DrvMsg(scrn->scrnIndex, X_INFO, "Initializing HW Cursor\n");
	if (!I830CursorInit(screen))
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		    "Hardware cursor initialization failed\n");

	/* Must force it before EnterVT, so we are in control of VT and
	 * later memory should be bound when allocating, e.g rotate_mem */
	scrn->vtSema = TRUE;

	if (!I830EnterVT(VT_FUNC_ARGS))
		return FALSE;

	intel->BlockHandler = screen->BlockHandler;
	screen->BlockHandler = I830BlockHandler;

	if (!AddCallback(&FlushCallback, intel_flush_callback, scrn))
		return FALSE;

	screen->SaveScreen = xf86SaveScreen;
	intel->CloseScreen = screen->CloseScreen;
	screen->CloseScreen = I830CloseScreen;
	intel->CreateScreenResources = screen->CreateScreenResources;
	screen->CreateScreenResources = i830CreateScreenResources;

	if (!xf86CrtcScreenInit(screen))
		return FALSE;

	DPRINTF(PFX, "assert( if(!miCreateDefColormap(screen)) )\n");
	if (!miCreateDefColormap(screen))
		return FALSE;

	DPRINTF(PFX, "assert( if(!xf86HandleColormaps(screen, ...)) )\n");
	if (!xf86HandleColormaps(screen, 256, 8, I830LoadPalette, NULL,
				 CMAP_RELOAD_ON_MODE_SWITCH |
				 CMAP_PALETTED_TRUECOLOR)) {
		return FALSE;
	}

	xf86DPMSInit(screen, xf86DPMSSet, 0);

#ifdef INTEL_XVMC
	if (INTEL_INFO(intel)->gen >= 40)
		intel->XvMCEnabled = TRUE;
	from = ((intel->directRenderingType == DRI_DRI2) &&
		xf86GetOptValBool(intel->Options, OPTION_XVMC,
				  &intel->XvMCEnabled) ? X_CONFIG : X_DEFAULT);
	xf86DrvMsg(scrn->scrnIndex, from, "Intel XvMC decoder %sabled\n",
		   intel->XvMCEnabled ? "en" : "dis");
#endif
	/* Init video */
	if (intel->XvEnabled)
		I830InitVideo(screen);

#if defined(DRI2)
	switch (intel->directRenderingType) {
	case DRI_DRI2:
		intel->directRenderingOpen = TRUE;
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "direct rendering: DRI2 Enabled\n");
		break;
	case DRI_DISABLED:
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "direct rendering: Disabled\n");
		break;
	case DRI_NONE:
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "direct rendering: Failed\n");
		break;
	}
#else
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "direct rendering: Not available\n");
#endif

	if (serverGeneration == 1)
		xf86ShowUnusedOptions(scrn->scrnIndex, scrn->options);

#ifdef notyet
	if (!intel->use_drm_mode {
		AddGeneralSocket(intel->drmSubFD);
		RegisterBlockAndWakeupHandlers((BlockHandlerProcPtr)NoopDDA,
		    drm_wakeup_handler, intel);
	}
#endif

	intel->starting = FALSE;
	intel->suspended = FALSE;

	return TRUE;
}

static void i830AdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
   SCRN_INFO_PTR(arg);
   xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR(scrn);
   intel_screen_private *intel = intel_get_screen_private(scrn);
   xf86OutputPtr  output = config->output[config->compat_output];
   xf86CrtcPtr	crtc = output->crtc;

   DPRINTF(PFX, "i830AdjustFrame: y = %d (+ %d), x = %d (+ %d)\n",
	   x, crtc->desiredX, y, crtc->desiredY);

   if (intel->use_drm_mode)
      return;

   if (crtc && crtc->enabled)
   {
      /* Sync the engine before adjust frame */
      intel_sync(scrn);
      i830PipeSetBase(crtc, crtc->desiredX + x, crtc->desiredY + y);
      crtc->x = output->initial_x + x;
      crtc->y = output->initial_y + y;
   }
}

static void I830FreeScreen(FREE_SCREEN_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);

	intel_close_drm_master(scrn);

	I830FreeRec(scrn);
	if (xf86LoaderCheckSymbol("vgaHWFreeHWRec"))
		vgaHWFreeHWRec(scrn);
}

static void I830LeaveVT(VT_FUNC_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int ret;

	DPRINTF(PFX, "Leave VT\n");

	i830SetHotkeyControl(scrn, HOTKEY_BIOS_SWITCH);

	xf86RotateFreeShadow(scrn);

	xf86_hide_cursors(scrn);

	if (!intel->use_drm_mode) {
		RestoreHWState(scrn);

		/* console restore hack */
		if (HAS_PCH_SPLIT(intel) && intel->int10 && intel->int10Mode) {
		    xf86Int10InfoPtr int10 = intel->int10;

		    /* Unlock the PP_CONTROL register, otherwise the
		     * int10 call fails to turn the panel back on.
		     */
		    OUTREG(PCH_PP_CONTROL,
		        INREG(PCH_PP_CONTROL) | (0xabcd << 16));

		    /* Use int10 to restore the console mode */
		    int10->num = 0x10;
		    int10->ax = 0x4f02;
		    int10->bx = intel->int10Mode | 0x8000;
		    int10->cx = int10->dx = 0;
		    xf86ExecX86int10(int10);
		}
	}

	i830_unbind_all_memory(scrn);

	if (!intel->use_drm_mode) {
		int ret;

		/* Tell the kernel to evict all buffer objects and block GTT
		 * usage while we're no longer in control of the chip.
		 */
		ret = drmCommandNone(intel->drmSubFD, DRM_I915_GEM_LEAVEVT);
		if (ret != 0)
			FatalError("DRM_I915_LEAVEVT failed: %s\n",
			    strerror(ret));
		}

	ret = drmDropMaster(intel->drmSubFD);
	if (ret)
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "drmDropMaster failed: %s\n", strerror(errno));
}

/*
 * This gets called when gaining control of the VT, and from ScreenInit().
 */
static Bool I830EnterVT(VT_FUNC_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int i, ret;

	DPRINTF(PFX, "Enter VT\n");

	ret = drmSetMaster(intel->drmSubFD);
	if (ret) {
		if (errno == EINVAL) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "drmSetMaster failed: 2.6.29 or newer kernel required for "
				   "multi-server DRI\n");
		} else {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "drmSetMaster failed: %s\n",
				   strerror(errno));
		}
	}

	/*
	 * Only save state once per server generation since that's what most
	 * drivers do.  Could change this to save state at each VT enter.
	 */
	if (intel->SaveGeneration != serverGeneration) {
		intel->SaveGeneration = serverGeneration;
		if (!intel->use_drm_mode)
			SaveHWState(scrn);
	}

	/* Get the hardware into a known state if needed */
	if (!intel->use_drm_mode) {
		/* Disable outputs */
		for (i = 0; i < xf86_config->num_output; i++) {
			xf86OutputPtr   output = xf86_config->output[i];
			output->funcs->dpms(output, DPMSModeOff);
		}
		i830WaitForVblank(scrn);

		/* Disable pipes */
		for (i = 0; i < xf86_config->num_crtc; i++) {
			xf86CrtcPtr crtc = xf86_config->crtc[i];
			if (HAS_PCH_SPLIT(intel))
			    ironlake_crtc_disable(crtc);
			else
			    i830_crtc_disable(crtc, TRUE);
		}
		i830WaitForVblank(scrn);
	}

	if (!intel->use_drm_mode) {
		int ret;

		i830_disable_render_standby(scrn);

		/* Tell the kernel that we're back in control and ready for GTT
		 * usage.
		 */
		ret = drmCommandNone(intel->drmSubFD, DRM_I915_GEM_ENTERVT);
		if (ret != 0)
			FatalError("DRM_I915_ENTERVT failed: %s\n",
			    strerror(ret));
	}

	if (!i830_bind_all_memory(scrn))
		return FALSE;

	i830_describe_allocations(scrn, 1, "");

	if (!intel->use_drm_mode) {
		I830InitHWCursor(scrn);

		/* Tell the BIOS that we're in control of mode setting now. */
		i830_init_bios_control(scrn);

		i830_init_clock_gating(scrn);

		if (intel->power_context)
			OUTREG(PWRCTXA, intel->power_context->offset |
			    PWRCTX_EN);
		/* Clear the framebuffer XXX drm_bo_map */
		memset(intel->FbBase + intel->front_buffer->offset, 0,
			scrn->virtualY * scrn->displayWidth * intel->cpp);
	}

	if (!xf86SetDesiredModes(scrn))
		return FALSE;

	if (!intel->use_drm_mode) {
		i830DescribeOutputConfiguration(scrn);
	}

	/* Set the hotkey to just notify us.  We could check its results
	 * periodically and attempt to do something, but it seems like we
	 * basically never get results when we should, and this should all
	 * be better handled through ACPI putting the key events out through
	 * evdev and your desktop environment picking it up.
	*/
	i830SetHotkeyControl(scrn, HOTKEY_DRIVER_NOTIFY);

	return TRUE;
}

static Bool I830SwitchMode(SWITCH_MODE_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);

	return xf86SetSingleMode(scrn, mode, RR_Rotate_0);
}

static Bool I830CloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (scrn->vtSema == TRUE) {
		I830LeaveVT(VT_FUNC_ARGS);
	}

	DeleteCallback(&FlushCallback, intel_flush_callback, scrn);

	if (!intel->use_drm_mode) {
		DPRINTF(PFX, "\nUnmapping memory\n");
		I830UnmapMem(scrn);
		vgaHWUnmapMem(scrn);
	}

	if (intel->uxa_driver) {
		uxa_driver_fini(screen);
		free(intel->uxa_driver);
		intel->uxa_driver = NULL;
	}
	if (intel->front_buffer) {
		if (!intel->use_shadow)
			intel_set_pixmap_bo(screen->GetScreenPixmap(screen),
					    NULL);
		if (intel->use_drm_mode)
			drmmode_closefb(scrn);
		/* already unpinned by leavevt */
		drm_intel_bo_unreference(intel->front_buffer);
		intel->front_buffer = NULL;
	}

	if (intel->shadow_buffer) {
		free(intel->shadow_buffer);
		intel->shadow_buffer = NULL;
	}

	if (intel->shadow_damage) {
		DamageUnregister(&screen->GetScreenPixmap(screen)->drawable,
				 intel->shadow_damage);
		DamageDestroy(intel->shadow_damage);
		intel->shadow_damage = NULL;
	}

	intel_batch_teardown(scrn);

	if (INTEL_INFO(intel)->gen >= 40)
		gen4_render_state_cleanup(scrn);

	xf86_cursors_fini(screen);

	i830_allocator_fini(scrn);

	i965_free_video(scrn);
	free(intel->offscreenImages);
	intel->offscreenImages = NULL;

	screen->CloseScreen = intel->CloseScreen;
	(*screen->CloseScreen) (CLOSE_SCREEN_ARGS);

	if (intel->directRenderingOpen
	    && intel->directRenderingType == DRI_DRI2) {
		intel->directRenderingOpen = FALSE;
		I830DRI2CloseScreen(screen);
	}

	xf86GARTCloseScreen(scrn->scrnIndex);
	scrn->vtSema = FALSE;
	return TRUE;
}

static ModeStatus
I830ValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode, Bool verbose, int flags)
{
	SCRN_INFO_PTR(arg);
	if (mode->Flags & V_INTERLACE) {
		if (verbose) {
			xf86DrvMsg(scrn->scrnIndex, X_PROBED,
				   "Removing interlaced mode \"%s\"\n",
				   mode->name);
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
static Bool I830PMEvent(SCRN_ARG_TYPE arg, pmEvent event, Bool undo)
{
	SCRN_INFO_PTR(arg);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	DPRINTF(PFX, "Enter VT, event %d, undo: %s\n", event,
		BOOLTOSTRING(undo));

	switch (event) {
	case XF86_APM_SYS_SUSPEND:
	case XF86_APM_CRITICAL_SUSPEND:	/*do we want to delay a critical suspend? */
	case XF86_APM_USER_SUSPEND:
	case XF86_APM_SYS_STANDBY:
	case XF86_APM_USER_STANDBY:
		if (!undo && !intel->suspended) {
			scrn->LeaveVT(VT_FUNC_ARGS);
			intel->suspended = TRUE;
			sleep(SUSPEND_SLEEP);
		} else if (undo && intel->suspended) {
			sleep(RESUME_SLEEP);
			scrn->EnterVT(VT_FUNC_ARGS);
			intel->suspended = FALSE;
		}
		break;
	case XF86_APM_STANDBY_RESUME:
	case XF86_APM_NORMAL_RESUME:
	case XF86_APM_CRITICAL_RESUME:
		if (intel->suspended) {
			sleep(RESUME_SLEEP);
			scrn->EnterVT(VT_FUNC_ARGS);
			intel->suspended = FALSE;
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
		ErrorF("I830PMEvent: Capability change\n");

		SaveScreens(SCREEN_SAVER_FORCER, ScreenSaverReset);
		if (intel->quirk_flag & QUIRK_RESET_MODES)
			xf86SetDesiredModes(scrn);

		break;
	default:
		ErrorF("I830PMEvent: received APM event %d\n", event);
	}
	return TRUE;
}

xf86CrtcPtr intel_pipe_to_crtc(ScrnInfoPtr scrn, int pipe)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	int c;

	for (c = 0; c < config->num_crtc; c++) {
		xf86CrtcPtr crtc = config->crtc[c];
		I830CrtcPrivatePtr intel_crtc = crtc->driver_private;

		if (intel_crtc->pipe == pipe)
			return crtc;
	}

	return NULL;
}

void intel_init_scrn(ScrnInfoPtr scrn)
{
	scrn->PreInit = I830PreInit;
	scrn->ScreenInit = I830ScreenInit;
	scrn->SwitchMode = I830SwitchMode;
	scrn->AdjustFrame = i830AdjustFrame;
	scrn->EnterVT = I830EnterVT;
	scrn->LeaveVT = I830LeaveVT;
	scrn->FreeScreen = I830FreeScreen;
	scrn->ValidMode = I830ValidMode;
	scrn->PMEvent = I830PMEvent;
}
