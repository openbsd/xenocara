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

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "xorg-server.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86cmap.h"
#include "xf86drm.h"
#include "compiler.h"
#include "mipointer.h"
#include "micmap.h"
#include <X11/extensions/randr.h>
#include "fb.h"
#include "miscstruct.h"
#include "dixstruct.h"
#include "xf86xv.h"
#include "shadow.h"
#include "intel.h"
#include "intel_video.h"
#include "uxa_module.h"

#ifdef INTEL_XVMC
#define _INTEL_XVMC_SERVER_
#include "intel_xvmc.h"
#endif

#include "uxa.h"

#include "i915_drm.h"

#include "intel_glamor.h"
#include "intel_options.h"

static void i830AdjustFrame(ADJUST_FRAME_ARGS_DECL);
static Bool I830CloseScreen(CLOSE_SCREEN_ARGS_DECL);
static Bool I830EnterVT(VT_FUNC_ARGS_DECL);

/* temporary */
extern void xf86SetCursor(ScreenPtr screen, CursorPtr pCurs, int x, int y);

/* Export I830 options to i830 driver where necessary */
static void
I830LoadPalette(ScrnInfoPtr scrn, int numColors, int *indices,
		LOCO * colors, VisualPtr pVisual)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	int i, j, index;
	int p;
	uint16_t lut_r[256], lut_g[256], lut_b[256];

	for (p = 0; p < xf86_config->num_crtc; p++) {
		xf86CrtcPtr crtc = xf86_config->crtc[p];

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
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	screen->CreateScreenResources = intel->CreateScreenResources;
	if (!(*screen->CreateScreenResources) (screen))
		return FALSE;

	if (!intel_uxa_create_screen_resources(screen))
		return FALSE;

	intel_copy_fb(scrn);
	return TRUE;
}

static void PreInitCleanup(ScrnInfoPtr scrn)
{
	if (!scrn || !scrn->driverPrivate)
		return;

	free(scrn->driverPrivate);
	scrn->driverPrivate = NULL;
}

static void intel_check_chipset_option(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	intel_detect_chipset(scrn, intel->pEnt);
}

static Bool I830GetEarlyOptions(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Process the options */
	intel->Options = intel_options_get(scrn);
	if (!intel->Options)
		return FALSE;

	intel->fallback_debug = xf86ReturnOptValBool(intel->Options,
						     OPTION_FALLBACKDEBUG,
						     FALSE);

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

static Bool intel_option_cast_string_to_bool(intel_screen_private *intel,
					     int id, Bool val)
{
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,7,99,901,0)
	xf86getBoolValue(&val, xf86GetOptValString(intel->Options, id));
	return val;
#else
	return val;
#endif
}

static void intel_check_dri_option(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	intel->dri2 = intel->dri3 = DRI_NONE;
	if (!intel_option_cast_string_to_bool(intel, OPTION_DRI, TRUE))
		intel->dri2 = intel->dri3 = DRI_DISABLED;

	if (scrn->depth != 16 && scrn->depth != 24 && scrn->depth != 30) {
		xf86DrvMsg(scrn->scrnIndex, X_CONFIG,
			   "DRI is disabled because it "
			   "runs only at depths 16, 24, and 30.\n");
		intel->dri2 = intel->dri3 = DRI_DISABLED;
	}
}

static Bool intel_open_drm_master(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	intel->drmSubFD = intel_get_device(scrn);
	return intel->drmSubFD != -1;
}

static int intel_init_bufmgr(intel_screen_private *intel)
{
	int batch_size;

	batch_size = 4096 * 4;
	if (IS_I865G(intel))
		/* The 865 has issues with larger-than-page-sized batch buffers. */
		batch_size = 4096;

	intel->bufmgr = drm_intel_bufmgr_gem_init(intel->drmSubFD, batch_size);
	if (!intel->bufmgr)
		return FALSE;

	if (xf86ReturnOptValBool(intel->Options, OPTION_BUFFER_CACHE, TRUE))
		drm_intel_bufmgr_gem_enable_reuse(intel->bufmgr);
	drm_intel_bufmgr_gem_set_vma_cache_size(intel->bufmgr, 512);
	drm_intel_bufmgr_gem_enable_fenced_relocs(intel->bufmgr);

	list_init(&intel->batch_pixmaps);

	if ((INTEL_INFO(intel)->gen == 060)) {
		intel->wa_scratch_bo =
			drm_intel_bo_alloc(intel->bufmgr, "wa scratch",
					   4096, 4096);
	}

	return TRUE;
}

static void intel_bufmgr_fini(intel_screen_private *intel)
{
	if (intel->bufmgr == NULL)
		return;

	drm_intel_bo_unreference(intel->wa_scratch_bo);
	drm_intel_bufmgr_destroy(intel->bufmgr);
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

static Bool drm_has_boolean_param(struct intel_screen_private *intel,
				  int param)
{
	drm_i915_getparam_t gp;
	int value;

	gp.value = &value;
	gp.param = param;
	if (drmIoctl(intel->drmSubFD, DRM_IOCTL_I915_GETPARAM, &gp))
		return FALSE;

	return value;
}

static Bool has_kernel_flush(struct intel_screen_private *intel)
{
	/* The BLT ring was introduced at the same time as the
	 * automatic flush for the busy-ioctl.
	 */
	return drm_has_boolean_param(intel, I915_PARAM_HAS_BLT);
}

static Bool has_relaxed_fencing(struct intel_screen_private *intel)
{
	return drm_has_boolean_param(intel, I915_PARAM_HAS_RELAXED_FENCING);
}

static Bool has_prime_vmap_flush(struct intel_screen_private *intel)
{
	return drm_has_boolean_param(intel, I915_PARAM_HAS_PRIME_VMAP_FLUSH);
}

static Bool can_accelerate_blt(struct intel_screen_private *intel)
{
	if (INTEL_INFO(intel)->gen == -1)
		return FALSE;

	if (xf86ReturnOptValBool(intel->Options, OPTION_ACCEL_DISABLE, FALSE) ||
	    !intel_option_cast_string_to_bool(intel, OPTION_ACCEL_METHOD, TRUE)) {
		xf86DrvMsg(intel->scrn->scrnIndex, X_CONFIG,
			   "Disabling hardware acceleration.\n");
		return FALSE;
	}

	if (INTEL_INFO(intel)->gen == 060) {
		struct pci_device *const device = intel->PciInfo;

		/* Sandybridge rev07 locks up easily, even with the
		 * BLT ring workaround in place.
		 * Thus use shadowfb by default.
		 */
		if (device->revision < 8) {
			xf86DrvMsg(intel->scrn->scrnIndex, X_WARNING,
				   "Disabling hardware acceleration on this pre-production hardware.\n");

			return FALSE;
		}
	}

	if (INTEL_INFO(intel)->gen >= 060) {
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

	return TRUE;
}

static void intel_setup_capabilities(ScrnInfoPtr scrn)
{
#ifdef INTEL_PIXMAP_SHARING
	intel_screen_private *intel = intel_get_screen_private(scrn);
	uint64_t value;
	int ret;

	scrn->capabilities = 0;

	ret = drmGetCap(intel->drmSubFD, DRM_CAP_PRIME, &value);
	if (ret == 0) {
		if (value & DRM_PRIME_CAP_EXPORT)
			scrn->capabilities |= RR_Capability_SourceOutput | RR_Capability_SinkOffload;
		if (value & DRM_PRIME_CAP_IMPORT)
			scrn->capabilities |= RR_Capability_SinkOutput;
	}
#endif
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
	intel_screen_private *intel;
	rgb defaultWeight = { 0, 0, 0 };
	EntityInfoPtr pEnt;
	int flags24;
	Gamma zeros = { 0.0, 0.0, 0.0 };

	if (scrn->numEntities != 1)
		return FALSE;

	pEnt = xf86GetEntityInfo(scrn->entityList[0]);
	if (pEnt == NULL)
		return FALSE;

	if (pEnt->location.type != BUS_PCI
#ifdef XSERVER_PLATFORM_BUS
	    && pEnt->location.type != BUS_PLATFORM
#endif
		)
		return FALSE;

	if (flags & PROBE_DETECT)
		return TRUE;

	if (((uintptr_t)scrn->driverPrivate) & 3) {
		intel = xnfcalloc(sizeof(*intel), 1);
		if (intel == NULL)
			return FALSE;

		intel->info = (void *)((uintptr_t)scrn->driverPrivate & ~3);
		scrn->driverPrivate = intel;
	}
	intel = intel_get_screen_private(scrn);
	intel->scrn = scrn;
	intel->pEnt = pEnt;

	scrn->displayWidth = 640;	/* default it */

	intel->PciInfo = xf86GetPciInfoForEntity(intel->pEnt->index);

	if (!intel_open_drm_master(scrn)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Failed to become DRM master.\n");
		return FALSE;
	}

	scrn->monitor = scrn->confScreen->monitor;
	scrn->progClock = TRUE;
	scrn->rgbBits = 8;

	flags24 = Support32bppFb | PreferConvert24to32 | SupportConvert24to32;

	if (!xf86SetDepthBpp(scrn, 0, 0, 0, flags24))
		return FALSE;

	switch (scrn->depth) {
	case 15:
	case 16:
	case 24:
	case 30:
		break;
	case 8:
	default:
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Given depth (%d) is not supported by intel driver\n",
			   scrn->depth);
		return FALSE;
	}
	xf86PrintDepthBpp(scrn);

	if (!xf86SetWeight(scrn, defaultWeight, defaultWeight))
		return FALSE;
	if (!xf86SetDefaultVisual(scrn, -1))
		return FALSE;

	intel->cpp = scrn->bitsPerPixel / 8;

	if (!I830GetEarlyOptions(scrn))
		return FALSE;

	intel_setup_capabilities(scrn);
	intel_check_chipset_option(scrn);
	intel_check_dri_option(scrn);

	if (!intel_init_bufmgr(intel)) {
		PreInitCleanup(scrn);
		return FALSE;
	}

	intel->force_fallback =
		drmCommandNone(intel->drmSubFD, DRM_I915_GEM_THROTTLE) != 0;

	/* Enable tiling by default */
	intel->tiling = INTEL_TILING_ALL;

	/* Allow user override if they set a value */
	if (!xf86ReturnOptValBool(intel->Options, OPTION_TILING_2D, TRUE))
		intel->tiling &= ~INTEL_TILING_2D;
	if (xf86ReturnOptValBool(intel->Options, OPTION_TILING_FB, FALSE))
		intel->tiling &= ~INTEL_TILING_FB;
	if (!can_accelerate_blt(intel)) {
		intel->force_fallback = TRUE;
		intel->tiling &= ~INTEL_TILING_FB;
	}

	intel->has_kernel_flush = has_kernel_flush(intel);

	intel->has_prime_vmap_flush = has_prime_vmap_flush(intel);

	intel->has_relaxed_fencing = INTEL_INFO(intel)->gen >= 033;
	/* And override the user if there is no kernel support */
	if (intel->has_relaxed_fencing)
		intel->has_relaxed_fencing = has_relaxed_fencing(intel);

	xf86DrvMsg(scrn->scrnIndex, X_CONFIG,
		   "Relaxed fencing %s\n",
		   intel->has_relaxed_fencing ? "enabled" : "disabled");

	/* SwapBuffers delays to avoid tearing */
	intel->swapbuffers_wait = xf86ReturnOptValBool(intel->Options,
						       OPTION_SWAPBUFFERS_WAIT,
						       TRUE);
	xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "Wait on SwapBuffers? %s\n",
		   intel->swapbuffers_wait ? "enabled" : "disabled");

	intel->use_triple_buffer =
		xf86ReturnOptValBool(intel->Options,
				     OPTION_TRIPLE_BUFFER,
				     TRUE);
	xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "Triple buffering? %s\n",
		   intel->use_triple_buffer ? "enabled" : "disabled");

	xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "Framebuffer %s\n",
		   intel->tiling & INTEL_TILING_FB ? "tiled" : "linear");
	xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "Pixmaps %s\n",
		   intel->tiling & INTEL_TILING_2D ? "tiled" : "linear");
	xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "3D buffers %s\n",
		   intel->tiling & INTEL_TILING_3D ? "tiled" : "linear");
	xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "SwapBuffers wait %sabled\n",
		   intel->swapbuffers_wait ? "en" : "dis");

	I830XvInit(scrn);

	if (!intel_mode_pre_init(scrn, intel->drmSubFD, intel->cpp)) {
		PreInitCleanup(scrn);
		return FALSE;
	}

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

	if (!intel_glamor_pre_init(scrn)) {
		PreInitCleanup(scrn);
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			"Failed to pre init glamor display.\n");
		return FALSE;
	}

	/* Load the dri modules if requested. */
#if HAVE_DRI2
	if (intel->dri2 != DRI_DISABLED && !xf86LoadSubModule(scrn, "dri2"))
		intel->dri2 = DRI_DISABLED;
#endif
#if HAVE_DRI3
	if (intel->dri3 != DRI_DISABLED && !xf86LoadSubModule(scrn, "dri3"))
		intel->dri3 = DRI_DISABLED;
#endif

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

#ifdef INTEL_PIXMAP_SHARING
static void
redisplay_dirty(ScreenPtr screen, PixmapDirtyUpdatePtr dirty)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	RegionRec pixregion;
	int was_blocked;

	PixmapRegionInit(&pixregion, dirty->slave_dst->master_pixmap);
	RegionTranslate(&pixregion, dirty->x, dirty->y);
	RegionIntersect(&pixregion, &pixregion, DamageRegion(dirty->damage));
	RegionTranslate(&pixregion, -dirty->x, -dirty->y);
	was_blocked = RegionNil(&pixregion);
	DamageRegionAppend(&dirty->slave_dst->drawable, &pixregion);
	RegionUninit(&pixregion);
	if (was_blocked)
		return;

	PixmapRegionInit(&pixregion, dirty->slave_dst->master_pixmap);
	PixmapSyncDirtyHelper(dirty, &pixregion);
	RegionUninit(&pixregion);

	intel_batch_submit(scrn);
	if (!intel->has_prime_vmap_flush) {
		drm_intel_bo *bo = intel_get_pixmap_bo(dirty->slave_dst->master_pixmap);
		was_blocked = xf86BlockSIGIO();
		drm_intel_bo_map(bo, FALSE);
		drm_intel_bo_unmap(bo);
		xf86UnblockSIGIO(was_blocked);
	}

	DamageRegionProcessPending(&dirty->slave_dst->drawable);
	return;
}

static void
intel_dirty_update(ScreenPtr screen)
{
	RegionPtr region;
	PixmapDirtyUpdatePtr ent;

	if (xorg_list_is_empty(&screen->pixmap_dirty_list))
	    return;

	xorg_list_for_each_entry(ent, &screen->pixmap_dirty_list, ent) {
		region = DamageRegion(ent->damage);
		if (RegionNotEmpty(region)) {
			redisplay_dirty(screen, ent);
			DamageEmpty(ent->damage);
		}
	}
}
#endif

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
	intel_video_block_handler(intel);
#ifdef INTEL_PIXMAP_SHARING
	intel_dirty_update(screen);
#endif
}

static Bool
intel_init_initial_framebuffer(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int width = scrn->virtualX;
	int height = scrn->virtualY;
	int pitch;
	uint32_t tiling;

	intel->front_buffer = intel_allocate_framebuffer(scrn,
							 width, height,
							 intel->cpp,
							 &pitch, &tiling);

	if (!intel->front_buffer) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Couldn't allocate initial framebuffer.\n");
		return FALSE;
	}

	intel->front_pitch = pitch;
	intel->front_tiling = tiling;
	scrn->displayWidth = pitch / intel->cpp;

	return TRUE;
}

static void
intel_flush_callback(CallbackListPtr *list,
		     pointer user_data, pointer call_data)
{
	ScrnInfoPtr scrn = user_data;
	if (scrn->vtSema) {
		intel_batch_submit(scrn);
		intel_glamor_flush(intel_get_screen_private(scrn));
	}
}

#if HAVE_UDEV
static void
I830HandleUEvents(int fd, void *closure)
{
	ScrnInfoPtr scrn = closure;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct udev_device *dev;
	const char *hotplug;
	struct stat s;
	dev_t udev_devnum;

	dev = udev_monitor_receive_device(intel->uevent_monitor);
	if (!dev)
		return;

	udev_devnum = udev_device_get_devnum(dev);
	if (fstat(intel->drmSubFD, &s)) {
		udev_device_unref(dev);
		return;
	}
	/*
	 * Check to make sure this event is directed at our
	 * device (by comparing dev_t values), then make
	 * sure it's a hotplug event (HOTPLUG=1)
	 */

	hotplug = udev_device_get_property_value(dev, "HOTPLUG");

	if (memcmp(&s.st_rdev, &udev_devnum, sizeof (dev_t)) == 0 &&
			hotplug && atoi(hotplug) == 1)
	{
		intel_mode_hotplug(intel);
	}

	udev_device_unref(dev);
}

static void
I830UeventInit(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct udev *u;
	struct udev_monitor *mon;
	Bool hotplug;
	MessageType from = X_CONFIG;

	if (!xf86GetOptValBool(intel->Options, OPTION_HOTPLUG, &hotplug)) {
		from = X_DEFAULT;
		hotplug = TRUE;
	}

	xf86DrvMsg(scrn->scrnIndex, from, "hotplug detection: \"%s\"\n",
			hotplug ? "enabled" : "disabled");
	if (!hotplug)
		return;

	u = udev_new();
	if (!u)
		return;

	mon = udev_monitor_new_from_netlink(u, "udev");

	if (!mon) {
		udev_unref(u);
		return;
	}

	if (udev_monitor_filter_add_match_subsystem_devtype(mon,
				"drm",
				"drm_minor") < 0 ||
			udev_monitor_enable_receiving(mon) < 0)
	{
		udev_monitor_unref(mon);
		udev_unref(u);
		return;
	}

	intel->uevent_handler = xf86AddGeneralHandler(udev_monitor_get_fd(mon),
						      I830HandleUEvents, scrn);
	if (!intel->uevent_handler) {
		udev_monitor_unref(mon);
		udev_unref(u);
		return;
	}

	intel->uevent_monitor = mon;
}

static void
I830UeventFini(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->uevent_handler) {
		struct udev *u = udev_monitor_get_udev(intel->uevent_monitor);

		xf86RemoveGeneralHandler(intel->uevent_handler);

		udev_monitor_unref(intel->uevent_monitor);
		udev_unref(u);
		intel->uevent_handler = NULL;
		intel->uevent_monitor = NULL;
	}
}
#endif /* HAVE_UDEV */

static Bool
I830ScreenInit(SCREEN_INIT_ARGS_DECL)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	VisualPtr visual;
#ifdef INTEL_XVMC
	MessageType from;
#endif
	struct pci_device *const device = intel->PciInfo;
	int fb_bar = IS_GEN2(intel) ? 0 : 2;

	scrn->videoRam = device->regions[fb_bar].size / 1024;

	intel->last_3d = LAST_3D_OTHER;
	intel->overlayOn = FALSE;

	/*
	 * Set this so that the overlay allocation is factored in when
	 * appropriate.
	 */
	intel->XvEnabled = TRUE;

	if (!intel_init_initial_framebuffer(scrn))
		return FALSE;

	intel_batch_init(scrn);

	if (INTEL_INFO(intel)->gen >= 040 && INTEL_INFO(intel)->gen < 0100)
		gen4_render_state_init(scrn);

	miClearVisualTypes();
	if (!miSetVisualTypes(scrn->depth,
			      miGetDefaultVisualMask(scrn->depth),
			      scrn->rgbBits, scrn->defaultVisual))
		return FALSE;
	if (!miSetPixmapDepths())
		return FALSE;

	/* Must be first, before anything else installs screen callbacks. */
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

#if HAVE_DRI2
	if (intel->dri2 == DRI_NONE && I830DRI2ScreenInit(screen))
		intel->dri2 = DRI_ACTIVE;
#endif

#if HAVE_DRI3
	if (!intel_sync_init(screen))
		intel->dri3 = DRI_DISABLED;
	if (intel->dri3 == DRI_NONE && intel_dri3_screen_init(screen))
		intel->dri3 = DRI_ACTIVE;
#endif

	if (xf86ReturnOptValBool(intel->Options, OPTION_PRESENT, TRUE))
		intel_present_screen_init(screen);

	xf86SetBackingStore(screen);
	xf86SetSilkenMouse(screen);
	miDCInitialize(screen, xf86GetPointerScreenFuncs());

	xf86DrvMsg(scrn->scrnIndex, X_INFO, "Initializing HW Cursor\n");
	if (!xf86_cursors_init(screen, 64, 64,
			       (HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
				HARDWARE_CURSOR_BIT_ORDER_MSBFIRST |
				HARDWARE_CURSOR_INVERT_MASK |
				HARDWARE_CURSOR_SWAP_SOURCE_AND_MASK |
				HARDWARE_CURSOR_AND_SOURCE_WITH_MASK |
				HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_64 |
				HARDWARE_CURSOR_UPDATE_UNHIDDEN |
				HARDWARE_CURSOR_ARGB))) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Hardware cursor initialization failed\n");
	}

	intel->BlockHandler = screen->BlockHandler;
	screen->BlockHandler = I830BlockHandler;

#ifdef INTEL_PIXMAP_SHARING
	screen->StartPixmapTracking = PixmapStartDirtyTracking;
	screen->StopPixmapTracking = PixmapStopDirtyTracking;
#endif

	if (!AddCallback(&FlushCallback, intel_flush_callback, scrn))
		return FALSE;

	screen->SaveScreen = xf86SaveScreen;
	intel->CloseScreen = screen->CloseScreen;
	screen->CloseScreen = I830CloseScreen;
	intel->CreateScreenResources = screen->CreateScreenResources;
	screen->CreateScreenResources = i830CreateScreenResources;

	intel_glamor_init(screen);
	if (!xf86CrtcScreenInit(screen))
		return FALSE;

	if (!miCreateDefColormap(screen))
		return FALSE;

	if (!xf86HandleColormaps(screen, 256, 8, I830LoadPalette, NULL,
				 CMAP_RELOAD_ON_MODE_SWITCH |
				 CMAP_PALETTED_TRUECOLOR)) {
		return FALSE;
	}

	xf86DPMSInit(screen, xf86DPMSSet, 0);

#ifdef INTEL_XVMC
	if (INTEL_INFO(intel)->gen >= 040)
		intel->XvMCEnabled = TRUE;
	from = (intel->dri2 == DRI_ACTIVE &&
		xf86GetOptValBool(intel->Options, OPTION_XVMC,
				  &intel->XvMCEnabled) ? X_CONFIG : X_DEFAULT);
	xf86DrvMsg(scrn->scrnIndex, from, "Intel XvMC decoder %sabled\n",
		   intel->XvMCEnabled ? "en" : "dis");
#endif
	/* Init video */
	if (intel->XvEnabled)
		I830InitVideo(screen);

#if HAVE_DRI2
	switch (intel->dri2) {
	case DRI_ACTIVE:
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "DRI2: Enabled\n");
		break;
	case DRI_DISABLED:
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "DRI2: Disabled\n");
		break;
	case DRI_NONE:
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "DRI2: Failed\n");
		break;
	}
#else
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "DRI2: Not available\n");
#endif

#if HAVE_DRI3
	switch (intel->dri3) {
	case DRI_ACTIVE:
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "DRI3: Enabled\n");
		break;
	case DRI_DISABLED:
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "DRI3: Disabled\n");
		break;
	case DRI_NONE:
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "DRI3: Failed\n");
		break;
	}
#else
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "DRI3: Not available\n");
#endif

	if (serverGeneration == 1)
		xf86ShowUnusedOptions(scrn->scrnIndex, scrn->options);

	intel_mode_init(intel);

	intel->suspended = FALSE;

#if HAVE_UDEV
	I830UeventInit(scrn);
#endif

	/* Must force it before EnterVT, so we are in control of VT and
	 * later memory should be bound when allocating, e.g rotate_mem */
	scrn->vtSema = TRUE;

	return I830EnterVT(VT_FUNC_ARGS(0));
}

static void i830AdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
}

static void I830FreeScreen(FREE_SCREEN_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel && !((uintptr_t)intel & 3)) {
		intel_mode_fini(intel);
		intel_bufmgr_fini(intel);
		intel_put_device(scrn);

		free(intel);
		scrn->driverPrivate = NULL;
	}
}

static void I830LeaveVT(VT_FUNC_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);

	xf86RotateFreeShadow(scrn);

	xf86_hide_cursors(scrn);

	if (intel_put_master(scrn))
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "drmDropMaster failed: %s\n", strerror(errno));
}

/*
 * This gets called when gaining control of the VT, and from ScreenInit().
 */
static Bool I830EnterVT(VT_FUNC_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);

	if (intel_get_master(scrn)) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "drmSetMaster failed: %s\n",
			   strerror(errno));
		return FALSE;
	}

	if (!xf86SetDesiredModes(scrn))
		return FALSE;

	intel_mode_disable_unused_functions(scrn);
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

#if HAVE_UDEV
	I830UeventFini(scrn);
#endif

	intel_mode_close(intel);

	DeleteCallback(&FlushCallback, intel_flush_callback, scrn);

	intel_glamor_close_screen(screen);

	TimerFree(intel->cache_expire);
	intel->cache_expire = NULL;

	if (intel->uxa_driver) {
		uxa_driver_fini(screen);
		free(intel->uxa_driver);
		intel->uxa_driver = NULL;
	}

	if (intel->back_pixmap) {
		screen->DestroyPixmap(intel->back_pixmap);
		intel->back_pixmap = NULL;
	}

	if (intel->back_buffer) {
		drm_intel_bo_unreference(intel->back_buffer);
		intel->back_buffer = NULL;
	}

	if (intel->front_buffer) {
		intel_mode_remove_fb(intel);
		drm_intel_bo_unreference(intel->front_buffer);
		intel->front_buffer = NULL;
	}

	if (scrn->vtSema == TRUE) {
		I830LeaveVT(VT_FUNC_ARGS(0));
	}

	intel_batch_teardown(scrn);

	if (INTEL_INFO(intel)->gen >= 040 && INTEL_INFO(intel)->gen < 0100)
		gen4_render_state_cleanup(scrn);

	xf86_cursors_fini(screen);

	i965_free_video(scrn);

	screen->CloseScreen = intel->CloseScreen;
	(*screen->CloseScreen) (CLOSE_SCREEN_ARGS);

	if (intel->dri2 == DRI_ACTIVE) {
		I830DRI2CloseScreen(screen);
		intel->dri2 = DRI_NONE;
	}

	if (intel->dri3 == DRI_ACTIVE) {
		/* nothing to do here? */
		intel->dri3 = DRI_NONE;
	}

#if HAVE_DRI3
	intel_sync_close(screen);
#endif

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

	switch (event) {
	case XF86_APM_SYS_SUSPEND:
	case XF86_APM_CRITICAL_SUSPEND:	/*do we want to delay a critical suspend? */
	case XF86_APM_USER_SUSPEND:
	case XF86_APM_SYS_STANDBY:
	case XF86_APM_USER_STANDBY:
		if (!undo && !intel->suspended) {
			scrn->LeaveVT(VT_FUNC_ARGS(0));
			intel->suspended = TRUE;
			sleep(SUSPEND_SLEEP);
		} else if (undo && intel->suspended) {
			sleep(RESUME_SLEEP);
			scrn->EnterVT(VT_FUNC_ARGS(0));
			intel->suspended = FALSE;
		}
		break;
	case XF86_APM_STANDBY_RESUME:
	case XF86_APM_NORMAL_RESUME:
	case XF86_APM_CRITICAL_RESUME:
		if (intel->suspended) {
			sleep(RESUME_SLEEP);
			scrn->EnterVT(VT_FUNC_ARGS(0));
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

		break;
	default:
		ErrorF("I830PMEvent: received APM event %d\n", event);
	}
	return TRUE;
}

Bool intel_init_scrn(ScrnInfoPtr scrn)
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
	return TRUE;
}
