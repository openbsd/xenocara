/*
 * Copyright © 2009 Red Hat, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Dave Airlie <airlied@redhat.com>
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <sys/ioctl.h>
/* Driver data structures */
#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_probe.h"
#include "micmap.h"

#include "radeon_version.h"
#include "shadow.h"

#include "atipciids.h"

/* DPMS */
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include "radeon_chipinfo_gen.h"

#include "radeon_bo_gem.h"
#include "radeon_cs_gem.h"
#include "radeon_vbo.h"

extern SymTabRec RADEONChipsets[];
static Bool radeon_setup_kernel_mem(ScreenPtr pScreen);

const OptionInfoRec RADEONOptions_KMS[] = {
    { OPTION_ACCEL,          "Accel",            OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_SW_CURSOR,      "SWcursor",         OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_PAGE_FLIP,      "EnablePageFlip",   OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_COLOR_TILING,   "ColorTiling",      OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_COLOR_TILING_2D,"ColorTiling2D",    OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_RENDER_ACCEL,   "RenderAccel",      OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_SUBPIXEL_ORDER, "SubPixelOrder",    OPTV_ANYSTR,  {0}, FALSE },
#ifdef USE_GLAMOR
    { OPTION_ACCELMETHOD,    "AccelMethod",      OPTV_STRING,  {0}, FALSE },
#endif
    { OPTION_EXA_VSYNC,      "EXAVSync",         OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_EXA_PIXMAPS,    "EXAPixmaps",	 OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_ZAPHOD_HEADS,   "ZaphodHeads",      OPTV_STRING,  {0}, FALSE },
    { OPTION_PAGE_FLIP,      "EnablePageFlip",   OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_SWAPBUFFERS_WAIT,"SwapbuffersWait", OPTV_BOOLEAN, {0}, FALSE },
    { -1,                    NULL,               OPTV_NONE,    {0}, FALSE }
};

const OptionInfoRec *RADEONOptionsWeak(void) { return RADEONOptions_KMS; }

void radeon_cs_flush_indirect(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    int ret;

    if (!info->cs->cdw)
	return;

    /* release the current VBO so we don't block on mapping it later */
    if (info->accel_state->vbo.vb_offset && info->accel_state->vbo.vb_bo) {
        radeon_vbo_put(pScrn, &info->accel_state->vbo);
        info->accel_state->vbo.vb_start_op = -1;
    }

    /* release the current VBO so we don't block on mapping it later */
    if (info->accel_state->cbuf.vb_bo) {
        radeon_vbo_put(pScrn, &info->accel_state->cbuf);
        info->accel_state->cbuf.vb_start_op = -1;
    }

    radeon_cs_emit(info->cs);
    radeon_cs_erase(info->cs);

    if (accel_state->use_vbos)
        radeon_vbo_flush_bos(pScrn);

    ret = radeon_cs_space_check_with_bo(info->cs,
					accel_state->vbo.vb_bo,
					RADEON_GEM_DOMAIN_GTT, 0);
    if (ret)
      ErrorF("space check failed in flush\n");

    if (info->reemit_current2d && info->state_2d.op)
        info->reemit_current2d(pScrn, info->state_2d.op);

    if (info->dri2.enabled) {
        info->accel_state->XInited3D = FALSE;
        info->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;
    }

}

void radeon_ddx_cs_start(ScrnInfoPtr pScrn,
			 int n, const char *file,
			 const char *func, int line)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (info->cs->cdw + n > info->cs->ndw) {
	radeon_cs_flush_indirect(pScrn);

    }
    radeon_cs_begin(info->cs, n, file, func, line);
}


extern _X_EXPORT int gRADEONEntityIndex;

static int getRADEONEntityIndex(void)
{
    return gRADEONEntityIndex;
}


RADEONEntPtr RADEONEntPriv(ScrnInfoPtr pScrn)
{
    DevUnion     *pPriv;
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    pPriv = xf86GetEntityPrivate(info->pEnt->index,
                                 getRADEONEntityIndex());
    return pPriv->ptr;
}

/* Allocate our private RADEONInfoRec */
static Bool RADEONGetRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate) return TRUE;

    pScrn->driverPrivate = xnfcalloc(sizeof(RADEONInfoRec), 1);
    return TRUE;
}

/* Free our private RADEONInfoRec */
static void RADEONFreeRec(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info;

    if (!pScrn || !pScrn->driverPrivate) return;

    info = RADEONPTR(pScrn);

    if (info->dri2.drm_fd > 0) {
        DevUnion *pPriv;
        RADEONEntPtr pRADEONEnt;
        pPriv = xf86GetEntityPrivate(pScrn->entityList[0],
				     getRADEONEntityIndex());

        pRADEONEnt = pPriv->ptr;
        pRADEONEnt->fd_ref--;
        if (!pRADEONEnt->fd_ref) {
#ifdef XF86_PDEV_SERVER_FD
            if (!(pRADEONEnt->platform_dev &&
                    pRADEONEnt->platform_dev->flags & XF86_PDEV_SERVER_FD))
#endif
                drmClose(pRADEONEnt->fd);
            pRADEONEnt->fd = 0;
        }
    }

    if (info->accel_state) {
	free(info->accel_state);
	info->accel_state = NULL;
    }

    free(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
}

static void *
radeonShadowWindow(ScreenPtr screen, CARD32 row, CARD32 offset, int mode,
		   CARD32 *size, void *closure)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(screen);
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    int stride;

    stride = (pScrn->displayWidth * pScrn->bitsPerPixel) / 8;
    *size = stride;

    return ((uint8_t *)info->front_bo->ptr + row * stride + offset);
}

static void
radeonUpdatePacked(ScreenPtr pScreen, shadowBufPtr pBuf)
{
    shadowUpdatePacked(pScreen, pBuf);
}

static Bool RADEONCreateScreenResources_KMS(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    PixmapPtr pixmap;
    struct radeon_surface *surface;

    pScreen->CreateScreenResources = info->CreateScreenResources;
    if (!(*pScreen->CreateScreenResources)(pScreen))
	return FALSE;
    pScreen->CreateScreenResources = RADEONCreateScreenResources_KMS;

    if (!drmmode_set_desired_modes(pScrn, &info->drmmode))
	return FALSE;

    drmmode_uevent_init(pScrn, &info->drmmode);

    if (info->r600_shadow_fb) {
	pixmap = pScreen->GetScreenPixmap(pScreen);

	if (!shadowAdd(pScreen, pixmap, radeonUpdatePacked,
		       radeonShadowWindow, 0, NULL))
	    return FALSE;
    }

    if (info->dri2.enabled || info->use_glamor) {
	if (info->front_bo) {
	    PixmapPtr pPix = pScreen->GetScreenPixmap(pScreen);
	    radeon_set_pixmap_bo(pPix, info->front_bo);
	    surface = radeon_get_pixmap_surface(pPix);
	    if (surface) {
		*surface = info->front_surface;
	    }
	}
    }

    if (info->use_glamor)
	radeon_glamor_create_screen_resources(pScreen);

    return TRUE;
}

#ifdef RADEON_PIXMAP_SHARING
static void
redisplay_dirty(ScreenPtr screen, PixmapDirtyUpdatePtr dirty)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(screen);
	RegionRec pixregion;

	PixmapRegionInit(&pixregion, dirty->slave_dst);
	DamageRegionAppend(&dirty->slave_dst->drawable, &pixregion);
	PixmapSyncDirtyHelper(dirty, &pixregion);

	radeon_cs_flush_indirect(pScrn);
	DamageRegionProcessPending(&dirty->slave_dst->drawable);
	RegionUninit(&pixregion);
}

static void
radeon_dirty_update(ScreenPtr screen)
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

static void RADEONBlockHandler_KMS(BLOCKHANDLER_ARGS_DECL)
{
    SCREEN_PTR(arg);
    ScrnInfoPtr    pScrn   = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info    = RADEONPTR(pScrn);

    pScreen->BlockHandler = info->BlockHandler;
    (*pScreen->BlockHandler) (BLOCKHANDLER_ARGS);
    pScreen->BlockHandler = RADEONBlockHandler_KMS;

    if (info->use_glamor)
	radeon_glamor_flush(pScrn);

    radeon_cs_flush_indirect(pScrn);
#ifdef RADEON_PIXMAP_SHARING
    radeon_dirty_update(pScreen);
#endif
}

static void
radeon_flush_callback(CallbackListPtr *list,
		      pointer user_data, pointer call_data)
{
    ScrnInfoPtr pScrn = user_data;

    if (pScrn->vtSema) {
        radeon_cs_flush_indirect(pScrn);
	radeon_glamor_flush(pScrn);
    }
}

static Bool RADEONIsFastFBWorking(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct drm_radeon_info ginfo;
    int r;
    uint32_t tmp = 0;

    memset(&ginfo, 0, sizeof(ginfo));
    ginfo.request = RADEON_INFO_FASTFB_WORKING;
    ginfo.value = (uintptr_t)&tmp;
    r = drmCommandWriteRead(info->dri2.drm_fd, DRM_RADEON_INFO, &ginfo, sizeof(ginfo));
    if (r) {
	return FALSE;
    }
    if (tmp == 1)
	return TRUE;
    return FALSE;
}

static Bool RADEONIsFusionGARTWorking(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct drm_radeon_info ginfo;
    int r;
    uint32_t tmp;

    memset(&ginfo, 0, sizeof(ginfo));
    ginfo.request = RADEON_INFO_FUSION_GART_WORKING;
    ginfo.value = (uintptr_t)&tmp;
    r = drmCommandWriteRead(info->dri2.drm_fd, DRM_RADEON_INFO, &ginfo, sizeof(ginfo));
    if (r) {
	return FALSE;
    }
    if (tmp == 1)
	return TRUE;
    return FALSE;
}

static Bool RADEONIsAccelWorking(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct drm_radeon_info ginfo;
    int r;
    uint32_t tmp;

    memset(&ginfo, 0, sizeof(ginfo));
    if (info->dri2.pKernelDRMVersion->version_minor >= 5)
	ginfo.request = RADEON_INFO_ACCEL_WORKING2;
    else
	ginfo.request = RADEON_INFO_ACCEL_WORKING;
    ginfo.value = (uintptr_t)&tmp;
    r = drmCommandWriteRead(info->dri2.drm_fd, DRM_RADEON_INFO, &ginfo, sizeof(ginfo));
    if (r) {
        /* If kernel is too old before 2.6.32 than assume accel is working */
        if (r == -EINVAL) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Kernel too old missing accel "
                       "information, assuming accel is working\n");
            return TRUE;
        }
        return FALSE;
    }
    if (info->ChipFamily == CHIP_FAMILY_HAWAII) {
        if (tmp == 2 || tmp == 3)
            return TRUE;
    } else if (tmp) {
        return TRUE;
    }
    return FALSE;
}

/* This is called by RADEONPreInit to set up the default visual */
static Bool RADEONPreInitVisual(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (!xf86SetDepthBpp(pScrn, 0, 0, 0, Support32bppFb))
	return FALSE;

    switch (pScrn->depth) {
    case 8:
    case 15:
    case 16:
    case 24:
	break;

    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Given depth (%d) is not supported by %s driver\n",
		   pScrn->depth, RADEON_DRIVER_NAME);
	return FALSE;
    }

    xf86PrintDepthBpp(pScrn);

    info->pix24bpp                   = xf86GetBppFromDepth(pScrn,
							   pScrn->depth);
    info->pixel_bytes  = pScrn->bitsPerPixel / 8;

    if (info->pix24bpp == 24) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Radeon does NOT support 24bpp\n");
	return FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Pixel depth = %d bits stored in %d byte%s (%d bpp pixmaps)\n",
	       pScrn->depth,
	       info->pixel_bytes,
	       info->pixel_bytes > 1 ? "s" : "",
	       info->pix24bpp);

    if (!xf86SetDefaultVisual(pScrn, -1)) return FALSE;

    if (pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Default visual (%s) is not supported at depth %d\n",
		   xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
	return FALSE;
    }
    return TRUE;
}

/* This is called by RADEONPreInit to handle all color weight issues */
static Bool RADEONPreInitWeight(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

				/* Save flag for 6 bit DAC to use for
				   setting CRTC registers.  Otherwise use
				   an 8 bit DAC, even if xf86SetWeight sets
				   pScrn->rgbBits to some value other than
				   8. */
    info->dac6bits = FALSE;

    if (pScrn->depth > 8) {
	rgb  defaultWeight = { 0, 0, 0 };

	if (!xf86SetWeight(pScrn, defaultWeight, defaultWeight)) return FALSE;
    } else {
	pScrn->rgbBits = 8;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d bits per RGB (%d bit DAC)\n",
	       pScrn->rgbBits, info->dac6bits ? 6 : 8);

    return TRUE;
}

static Bool RADEONPreInitAccel_KMS(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (!(info->accel_state = calloc(1, sizeof(struct radeon_accel_state)))) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to allocate accel_state rec!\n");
	return FALSE;
    }

    /* Check whether direct mapping is used for fast fb access*/
    if (RADEONIsFastFBWorking(pScrn)) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Direct mapping of fb aperture is enabled for fast fb access.\n");
	info->is_fast_fb = TRUE;
    }

    if (!xf86ReturnOptValBool(info->Options, OPTION_ACCEL, TRUE) ||
	(!RADEONIsAccelWorking(pScrn))) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "GPU accel disabled or not working, using shadowfb for KMS\n");
shadowfb:
	info->r600_shadow_fb = TRUE;
	if (!xf86LoadSubModule(pScrn, "shadow"))
	    info->r600_shadow_fb = FALSE;
	return TRUE;
    }

#ifdef DRI2
    info->dri2.available = !!xf86LoadSubModule(pScrn, "dri2");
#endif

    if (radeon_glamor_pre_init(pScrn))
	return TRUE;

    if (info->ChipFamily >= CHIP_FAMILY_TAHITI) {
	goto shadowfb;
    } else if (info->ChipFamily == CHIP_FAMILY_PALM) {
	info->accel_state->allowHWDFS = RADEONIsFusionGARTWorking(pScrn);
    } else
	info->accel_state->allowHWDFS = TRUE;

    if ((info->ChipFamily == CHIP_FAMILY_RS100) ||
	(info->ChipFamily == CHIP_FAMILY_RS200) ||
	(info->ChipFamily == CHIP_FAMILY_RS300) ||
	(info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480) ||
	(info->ChipFamily == CHIP_FAMILY_RS600) ||
	(info->ChipFamily == CHIP_FAMILY_RS690) ||
	(info->ChipFamily == CHIP_FAMILY_RS740))
	info->accel_state->has_tcl = FALSE;
    else {
	info->accel_state->has_tcl = TRUE;
    }

    {
	int errmaj = 0, errmin = 0;
	info->exaReq.majorversion = EXA_VERSION_MAJOR;
	info->exaReq.minorversion = EXA_VERSION_MINOR;
	if (!LoadSubModule(pScrn->module, "exa", NULL, NULL, NULL,
			   &info->exaReq, &errmaj, &errmin)) {
	    LoaderErrorMsg(NULL, "exa", errmaj, errmin);
	    return FALSE;
	}
    }

    return TRUE;
}

static Bool RADEONPreInitChipType_KMS(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    int i;

    info->Chipset = PCI_DEV_DEVICE_ID(info->PciInfo);
    pScrn->chipset = (char *)xf86TokenToString(RADEONChipsets, info->Chipset);
    if (!pScrn->chipset) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "ChipID 0x%04x is not recognized\n", info->Chipset);
	return FALSE;
    }

    if (info->Chipset < 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Chipset \"%s\" is not recognized\n", pScrn->chipset);
	return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	       "Chipset: \"%s\" (ChipID = 0x%04x)\n",
	       pScrn->chipset,
	       info->Chipset);

    for (i = 0; i < sizeof(RADEONCards) / sizeof(RADEONCardInfo); i++) {
	if (info->Chipset == RADEONCards[i].pci_device_id) {
	    RADEONCardInfo *card = &RADEONCards[i];
	    info->ChipFamily = card->chip_family;
	    break;
	}
    }

#ifdef RENDER
    info->RenderAccel = xf86ReturnOptValBool(info->Options, OPTION_RENDER_ACCEL,
					     info->Chipset != PCI_CHIP_RN50_515E &&
					     info->Chipset != PCI_CHIP_RN50_5969);
#endif
    return TRUE;
}

static int radeon_get_drm_master_fd(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
#ifdef XF86_PDEV_SERVER_FD
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
#endif
    struct pci_device *dev = info->PciInfo;
    char *busid;
    int fd;

#ifdef XF86_PDEV_SERVER_FD
    if (pRADEONEnt->platform_dev) {
        fd = xf86_get_platform_device_int_attrib(pRADEONEnt->platform_dev,
                                                 ODEV_ATTRIB_FD, -1);
        if (fd != -1)
            return fd;
    }
#endif

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,9,99,901,0)
    XNFasprintf(&busid, "pci:%04x:%02x:%02x.%d",
                dev->domain, dev->bus, dev->dev, dev->func);
#else
    busid = XNFprintf("pci:%04x:%02x:%02x.%d",
		      dev->domain, dev->bus, dev->dev, dev->func);
#endif

    fd = drmOpen(NULL, busid);
    if (fd == -1)
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[drm] Failed to open DRM device for %s: %s\n",
		   busid, strerror(errno));

    free(busid);
    return fd;
}

static Bool radeon_open_drm_master(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    drmSetVersion sv;
    int err;

    if (pRADEONEnt->fd) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   " reusing fd for second head\n");

	info->drmmode.fd = info->dri2.drm_fd = pRADEONEnt->fd;
	pRADEONEnt->fd_ref++;
        return TRUE;
    }

    info->dri2.drm_fd = radeon_get_drm_master_fd(pScrn);
    if (info->dri2.drm_fd == -1)
	return FALSE;

    /* Check that what we opened was a master or a master-capable FD,
     * by setting the version of the interface we'll use to talk to it.
     * (see DRIOpenDRMMaster() in DRI1)
     */
    sv.drm_di_major = 1;
    sv.drm_di_minor = 1;
    sv.drm_dd_major = -1;
    sv.drm_dd_minor = -1;
    err = drmSetInterfaceVersion(info->dri2.drm_fd, &sv);
    if (err != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[drm] failed to set drm interface version.\n");
	drmClose(info->dri2.drm_fd);
	info->dri2.drm_fd = -1;

	return FALSE;
    }

    pRADEONEnt->fd = info->dri2.drm_fd;
    pRADEONEnt->fd_ref = 1;
    info->drmmode.fd = info->dri2.drm_fd;
    return TRUE;
}

static Bool r600_get_tile_config(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    struct drm_radeon_info ginfo;
    int r;
    uint32_t tmp;

    if (info->ChipFamily < CHIP_FAMILY_R600)
	return FALSE;

    memset(&ginfo, 0, sizeof(ginfo));
    ginfo.request = RADEON_INFO_TILING_CONFIG;
    ginfo.value = (uintptr_t)&tmp;
    r = drmCommandWriteRead(info->dri2.drm_fd, DRM_RADEON_INFO, &ginfo, sizeof(ginfo));
    if (r)
	return FALSE;

    info->tile_config = tmp;
    info->r7xx_bank_op = 0;
    if (info->ChipFamily >= CHIP_FAMILY_CEDAR) {
	if (info->dri2.pKernelDRMVersion->version_minor >= 7) {
	    switch (info->tile_config & 0xf) {
	    case 0:
                info->num_channels = 1;
                break;
	    case 1:
                info->num_channels = 2;
                break;
	    case 2:
                info->num_channels = 4;
                break;
	    case 3:
                info->num_channels = 8;
                break;
	    default:
                return FALSE;
	    }

	    switch((info->tile_config & 0xf0) >> 4) {
	    case 0:
		info->num_banks = 4;
		break;
	    case 1:
		info->num_banks = 8;
		break;
	    case 2:
		info->num_banks = 16;
		break;
	    default:
		return FALSE;
	    }

	    switch ((info->tile_config & 0xf00) >> 8) {
	    case 0:
                info->group_bytes = 256;
                break;
	    case 1:
                info->group_bytes = 512;
                break;
	    default:
                return FALSE;
	    }
	} else
	    return FALSE;
    } else {
	switch((info->tile_config & 0xe) >> 1) {
	case 0:
	    info->num_channels = 1;
	    break;
	case 1:
	    info->num_channels = 2;
	    break;
	case 2:
	    info->num_channels = 4;
	    break;
	case 3:
	    info->num_channels = 8;
	    break;
	default:
	    return FALSE;
	}
	switch((info->tile_config & 0x30) >> 4) {
	case 0:
	    info->num_banks = 4;
	    break;
	case 1:
	    info->num_banks = 8;
	    break;
	default:
	    return FALSE;
	}
	switch((info->tile_config & 0xc0) >> 6) {
	case 0:
	    info->group_bytes = 256;
	    break;
	case 1:
	    info->group_bytes = 512;
	    break;
	default:
	    return FALSE;
	}
    }

    info->have_tiling_info = TRUE;
    return TRUE;
}

static void RADEONSetupCapabilities(ScrnInfoPtr pScrn)
{
#ifdef RADEON_PIXMAP_SHARING
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    uint64_t value;
    int ret;

    pScrn->capabilities = 0;
    ret = drmGetCap(info->dri2.drm_fd, DRM_CAP_PRIME, &value);
    if (ret == 0) {
	if (value & DRM_PRIME_CAP_EXPORT)
	    pScrn->capabilities |= RR_Capability_SourceOutput | RR_Capability_SinkOffload;
	if (value & DRM_PRIME_CAP_IMPORT)
	    pScrn->capabilities |= RR_Capability_SourceOffload | RR_Capability_SinkOutput;
    }
#endif
}

Bool RADEONPreInit_KMS(ScrnInfoPtr pScrn, int flags)
{
    RADEONInfoPtr     info;
    RADEONEntPtr pRADEONEnt;
    DevUnion* pPriv;
    Gamma  zeros = { 0.0, 0.0, 0.0 };
    uint32_t tiling = 0;
    int cpp;

    if (flags & PROBE_DETECT)
        return TRUE;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONPreInit_KMS\n");
    if (pScrn->numEntities != 1) return FALSE;
    if (!RADEONGetRec(pScrn)) return FALSE;

    info               = RADEONPTR(pScrn);
    info->IsSecondary  = FALSE;
    info->IsPrimary = FALSE;
    info->pEnt         = xf86GetEntityInfo(pScrn->entityList[pScrn->numEntities - 1]);
    if (info->pEnt->location.type != BUS_PCI
#ifdef XSERVER_PLATFORM_BUS
        && info->pEnt->location.type != BUS_PLATFORM
#endif
        )
        goto fail;

    pPriv = xf86GetEntityPrivate(pScrn->entityList[0],
				 getRADEONEntityIndex());
    pRADEONEnt = pPriv->ptr;

    if(xf86IsEntityShared(pScrn->entityList[0]))
    {
        if(xf86IsPrimInitDone(pScrn->entityList[0]))
        {
            info->IsSecondary = TRUE;
            pRADEONEnt->pSecondaryScrn = pScrn;
        }
        else
        {
	    info->IsPrimary = TRUE;
            xf86SetPrimInitDone(pScrn->entityList[0]);
            pRADEONEnt->pPrimaryScrn = pScrn;
            pRADEONEnt->HasSecondary = FALSE;
        }
    }

    info->PciInfo = xf86GetPciInfoForEntity(info->pEnt->index);
    pScrn->monitor     = pScrn->confScreen->monitor;

    if (!RADEONPreInitVisual(pScrn))
	goto fail;

    xf86CollectOptions(pScrn, NULL);
    if (!(info->Options = malloc(sizeof(RADEONOptions_KMS))))
	goto fail;

    memcpy(info->Options, RADEONOptions_KMS, sizeof(RADEONOptions_KMS));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, info->Options);

    if (!RADEONPreInitWeight(pScrn))
	goto fail;

    if (!RADEONPreInitChipType_KMS(pScrn))
        goto fail;

    if (radeon_open_drm_master(pScrn) == FALSE) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Kernel modesetting setup failed\n");
	goto fail;
    }

    info->dri2.available = FALSE;
    info->dri2.enabled = FALSE;
    info->dri2.pKernelDRMVersion = drmGetVersion(info->dri2.drm_fd);
    if (info->dri2.pKernelDRMVersion == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "RADEONDRIGetVersion failed to get the DRM version\n");
	goto fail;
    }

    if (!RADEONPreInitAccel_KMS(pScrn))              goto fail;

    info->allowColorTiling2D = FALSE;

    RADEONSetupCapabilities(pScrn);

    /* don't enable tiling if accel is not enabled */
    if (!info->r600_shadow_fb) {
	Bool colorTilingDefault =
	    xorgGetVersion() >= XORG_VERSION_NUMERIC(1,9,4,901,0) &&
	    info->ChipFamily >= CHIP_FAMILY_R300 &&
	    /* this check could be removed sometime after a big mesa release
	     * with proper bit, in the meantime you need to set tiling option in
	     * xorg configuration files
	     */
	    info->ChipFamily <= CHIP_FAMILY_MULLINS &&
	    !info->is_fast_fb;

	/* 2D color tiling */
	if (info->ChipFamily >= CHIP_FAMILY_R600) {
		info->allowColorTiling2D = xf86ReturnOptValBool(info->Options, OPTION_COLOR_TILING_2D,
                                                                info->ChipFamily <= CHIP_FAMILY_MULLINS);
	}

	if (info->ChipFamily >= CHIP_FAMILY_R600) {
	    /* set default group bytes, overridden by kernel info below */
	    info->group_bytes = 256;
	    info->have_tiling_info = FALSE;
	    if (info->dri2.pKernelDRMVersion->version_minor >= 6) {
		if (r600_get_tile_config(pScrn)) {
		    info->allowColorTiling = xf86ReturnOptValBool(info->Options,
								  OPTION_COLOR_TILING, colorTilingDefault);
		    /* need working DFS for tiling */
		    if ((info->ChipFamily == CHIP_FAMILY_PALM) &&
			(!info->accel_state->allowHWDFS))
			info->allowColorTiling = FALSE;
		} else
		    info->allowColorTiling = FALSE;
	    } else
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "R6xx+ KMS Color Tiling requires radeon drm 2.6.0 or newer\n");
	} else
	    info->allowColorTiling = xf86ReturnOptValBool(info->Options,
							  OPTION_COLOR_TILING, colorTilingDefault);
    } else
	info->allowColorTiling = FALSE;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	 "KMS Color Tiling: %sabled\n", info->allowColorTiling ? "en" : "dis");
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	 "KMS Color Tiling 2D: %sabled\n", info->allowColorTiling2D ? "en" : "dis");

    if (info->dri2.pKernelDRMVersion->version_minor >= 8) {
	info->allowPageFlip = xf86ReturnOptValBool(info->Options,
						   OPTION_PAGE_FLIP, TRUE);
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "KMS Pageflipping: %sabled\n", info->allowPageFlip ? "en" : "dis");
    }

    info->swapBuffersWait = xf86ReturnOptValBool(info->Options,
						 OPTION_SWAPBUFFERS_WAIT, TRUE);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "SwapBuffers wait for vsync: %sabled\n", info->swapBuffersWait ? "en" : "dis");

    if (drmmode_pre_init(pScrn, &info->drmmode, pScrn->bitsPerPixel / 8) == FALSE) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Kernel modesetting setup failed\n");
	goto fail;
    }

    if (info->drmmode.mode_res->count_crtcs == 1)
        pRADEONEnt->HasCRTC2 = FALSE;
    else
        pRADEONEnt->HasCRTC2 = TRUE;


    /* fix up cloning on rn50 cards
     * since they only have one crtc sometimes the xserver doesn't assign
     * a crtc to one of the outputs even though both outputs have common modes
     * which results in only one monitor being enabled.  Assign a crtc here so
     * that both outputs light up.
     */
    if (info->ChipFamily == CHIP_FAMILY_RV100 && !pRADEONEnt->HasCRTC2) {
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	int i;

	for (i = 0; i < xf86_config->num_output; i++) {
	    xf86OutputPtr output = xf86_config->output[i];

	    /* XXX: double check crtc mode */
	    if ((output->probed_modes != NULL) && (output->crtc == NULL))
		output->crtc = xf86_config->crtc[0];
	}
    }

    /* set cursor size */
    if (info->ChipFamily >= CHIP_FAMILY_BONAIRE) {
	info->cursor_w = CURSOR_WIDTH_CIK;
	info->cursor_h = CURSOR_HEIGHT_CIK;
    } else {
	info->cursor_w = CURSOR_WIDTH;
	info->cursor_h = CURSOR_HEIGHT;
    }

    {
	struct drm_radeon_gem_info mminfo;

	if (!drmCommandWriteRead(info->dri2.drm_fd, DRM_RADEON_GEM_INFO, &mminfo, sizeof(mminfo)))
	{
	    info->vram_size = mminfo.vram_visible;
	    info->gart_size = mminfo.gart_size;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "mem size init: gart size :%llx vram size: s:%llx visible:%llx\n",
		       (unsigned long long)mminfo.gart_size,
		       (unsigned long long)mminfo.vram_size,
		       (unsigned long long)mminfo.vram_visible);
	}
    }

    if (!info->use_glamor) {
	info->exa_pixmaps = xf86ReturnOptValBool(info->Options,
						 OPTION_EXA_PIXMAPS,
						 (info->vram_size > (32 * 1024 * 1024) &&
						 info->RenderAccel &&
                                                 !info->is_fast_fb));
	if (info->exa_pixmaps)
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "EXA: Driver will allow EXA pixmaps in VRAM\n");
	else
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "EXA: Driver will not allow EXA pixmaps in VRAM\n");
    }

    /* no tiled scanout on r6xx+ yet */
    if (info->allowColorTiling) {
	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    tiling |= RADEON_TILING_MICRO;
	else
	    tiling |= RADEON_TILING_MACRO;
    }
    cpp = pScrn->bitsPerPixel / 8;
    pScrn->displayWidth =
	RADEON_ALIGN(pScrn->virtualX, drmmode_get_pitch_align(pScrn, cpp, tiling));

    /* Set display resolution */
    xf86SetDpi(pScrn, 0, 0);

	/* Get ScreenInit function */
    if (!xf86LoadSubModule(pScrn, "fb")) return FALSE;

    if (!xf86SetGamma(pScrn, zeros)) return FALSE;

    if (!xf86ReturnOptValBool(info->Options, OPTION_SW_CURSOR, FALSE)) {
	if (!xf86LoadSubModule(pScrn, "ramdac")) return FALSE;
    }

    if (pScrn->modes == NULL
#ifdef XSERVER_PLATFORM_BUS
        && !pScrn->is_gpu
#endif
        ) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No modes.\n");
      goto fail;
   }

    return TRUE;
 fail:
    RADEONFreeRec(pScrn);
    return FALSE;

}

static Bool RADEONCursorInit_KMS(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);

    return xf86_cursors_init (pScreen, info->cursor_w, info->cursor_h,
			      (HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
			       HARDWARE_CURSOR_AND_SOURCE_WITH_MASK |
			       HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_1 |
			       HARDWARE_CURSOR_UPDATE_UNHIDDEN |
			       HARDWARE_CURSOR_ARGB));
}

void
RADEONBlank(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86OutputPtr output;
    xf86CrtcPtr crtc;
    int o, c;

    for (c = 0; c < xf86_config->num_crtc; c++) {
       crtc = xf86_config->crtc[c];
       for (o = 0; o < xf86_config->num_output; o++) {
           output = xf86_config->output[o];
           if (output->crtc != crtc)
               continue;

           output->funcs->dpms(output, DPMSModeOff);
       }
      crtc->funcs->dpms(crtc, DPMSModeOff);
    }
}

void
RADEONUnblank(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86OutputPtr output;
    xf86CrtcPtr crtc;
    int o, c;
    for (c = 0; c < xf86_config->num_crtc; c++) {
       crtc = xf86_config->crtc[c];
       if(!crtc->enabled)
              continue;
       crtc->funcs->dpms(crtc, DPMSModeOn);
       for (o = 0; o < xf86_config->num_output; o++) {
           output = xf86_config->output[o];
           if (output->crtc != crtc)
               continue;
           output->funcs->dpms(output, DPMSModeOn);
       }
    }
}


static Bool RADEONSaveScreen_KMS(ScreenPtr pScreen, int mode)
{
    ScrnInfoPtr  pScrn = xf86ScreenToScrn(pScreen);
    Bool         unblank;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONSaveScreen(%d)\n", mode);

    unblank = xf86IsUnblank(mode);
    if (unblank) SetTimeSinceLastInputEvent();

    if ((pScrn != NULL) && pScrn->vtSema) {
	if (unblank)
	    RADEONUnblank(pScrn);
	else
	    RADEONBlank(pScrn);
    }
    return TRUE;
}

static Bool radeon_set_drm_master(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
#ifdef XF86_PDEV_SERVER_FD
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
#endif
    int err;

#ifdef XF86_PDEV_SERVER_FD
    if (pRADEONEnt->platform_dev &&
            (pRADEONEnt->platform_dev->flags & XF86_PDEV_SERVER_FD))
        return TRUE;
#endif

    err = drmSetMaster(info->dri2.drm_fd);
    if (err)
        ErrorF("Unable to retrieve master\n");

    return err == 0;
}

static void radeon_drop_drm_master(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
#ifdef XF86_PDEV_SERVER_FD
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

    if (pRADEONEnt->platform_dev &&
            (pRADEONEnt->platform_dev->flags & XF86_PDEV_SERVER_FD))
        return;
#endif

    drmDropMaster(info->dri2.drm_fd);
}

/* Called at the end of each server generation.  Restore the original
 * text mode, unmap video memory, and unwrap and call the saved
 * CloseScreen function.
 */
static Bool RADEONCloseScreen_KMS(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONCloseScreen\n");

    drmmode_uevent_fini(pScrn, &info->drmmode);
    radeon_cs_flush_indirect(pScrn);

    DeleteCallback(&FlushCallback, radeon_flush_callback, pScrn);

    if (info->accel_state->exa) {
	exaDriverFini(pScreen);
	free(info->accel_state->exa);
	info->accel_state->exa = NULL;
    }

    if (info->accel_state->use_vbos)
        radeon_vbo_free_lists(pScrn);

    radeon_drop_drm_master(pScrn);

    drmmode_fini(pScrn, &info->drmmode);
    if (info->dri2.enabled)
	radeon_dri2_close_screen(pScreen);

    pScrn->vtSema = FALSE;
    xf86ClearPrimInitDone(info->pEnt->index);
    pScreen->BlockHandler = info->BlockHandler;
    pScreen->CloseScreen = info->CloseScreen;
    return (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);
}


void RADEONFreeScreen_KMS(FREE_SCREEN_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONFreeScreen\n");

    /* when server quits at PreInit, we don't need do this anymore*/
    if (!info) return;

    RADEONFreeRec(pScrn);
}

Bool RADEONScreenInit_KMS(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    int            subPixelOrder = SubPixelUnknown;
    const char *s;
    void *front_ptr;

    pScrn->fbOffset = 0;

    miClearVisualTypes();
    if (!miSetVisualTypes(pScrn->depth,
			  miGetDefaultVisualMask(pScrn->depth),
			  pScrn->rgbBits,
			  pScrn->defaultVisual)) return FALSE;
    miSetPixmapDepths ();

    if (!radeon_set_drm_master(pScrn))
        return FALSE;

    info->directRenderingEnabled = FALSE;
    if (info->r600_shadow_fb == FALSE)
        info->directRenderingEnabled = radeon_dri2_screen_init(pScreen);

    info->surf_man = radeon_surface_manager_new(info->dri2.drm_fd);
    if (!info->bufmgr)
        info->bufmgr = radeon_bo_manager_gem_ctor(info->dri2.drm_fd);
    if (!info->bufmgr) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "failed to initialise GEM buffer manager");
	return FALSE;
    }
    drmmode_set_bufmgr(pScrn, &info->drmmode, info->bufmgr);

    if (!info->csm)
        info->csm = radeon_cs_manager_gem_ctor(info->dri2.drm_fd);
    if (!info->csm) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "failed to initialise command submission manager");
	return FALSE;
    }

    if (!info->cs)
        info->cs = radeon_cs_create(info->csm, RADEON_BUFFER_SIZE/4);
    if (!info->cs) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "failed to initialise command submission buffer");
	return FALSE;
    }

    radeon_cs_set_limit(info->cs, RADEON_GEM_DOMAIN_GTT, info->gart_size);
    radeon_cs_space_set_flush(info->cs, (void(*)(void *))radeon_cs_flush_indirect, pScrn); 

    if (!radeon_setup_kernel_mem(pScreen)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "radeon_setup_kernel_mem failed\n");
	return FALSE;
    }
    front_ptr = info->front_bo->ptr;

    if (info->r600_shadow_fb) {
	info->fb_shadow = calloc(1,
				 pScrn->displayWidth * pScrn->virtualY *
				 ((pScrn->bitsPerPixel + 7) >> 3));
	if (info->fb_shadow == NULL) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Failed to allocate shadow framebuffer\n");
	    info->r600_shadow_fb = FALSE;
	} else {
	    if (!fbScreenInit(pScreen, info->fb_shadow,
			      pScrn->virtualX, pScrn->virtualY,
			      pScrn->xDpi, pScrn->yDpi, pScrn->displayWidth,
			      pScrn->bitsPerPixel))
		return FALSE;
	}
    }

    if (info->r600_shadow_fb == FALSE) {
	/* Init fb layer */
	if (!fbScreenInit(pScreen, front_ptr,
			  pScrn->virtualX, pScrn->virtualY,
			  pScrn->xDpi, pScrn->yDpi, pScrn->displayWidth,
			  pScrn->bitsPerPixel))
	    return FALSE;
    }

    xf86SetBlackWhitePixels(pScreen);

    if (pScrn->bitsPerPixel > 8) {
	VisualPtr  visual;

	visual = pScreen->visuals + pScreen->numVisuals;
	while (--visual >= pScreen->visuals) {
	    if ((visual->class | DynamicClass) == DirectColor) {
		visual->offsetRed   = pScrn->offset.red;
		visual->offsetGreen = pScrn->offset.green;
		visual->offsetBlue  = pScrn->offset.blue;
		visual->redMask     = pScrn->mask.red;
		visual->greenMask   = pScrn->mask.green;
		visual->blueMask    = pScrn->mask.blue;
	    }
	}
    }

    /* Must be after RGB order fixed */
    fbPictureInit (pScreen, 0, 0);

#ifdef RENDER
    if ((s = xf86GetOptValString(info->Options, OPTION_SUBPIXEL_ORDER))) {
	if (strcmp(s, "RGB") == 0) subPixelOrder = SubPixelHorizontalRGB;
	else if (strcmp(s, "BGR") == 0) subPixelOrder = SubPixelHorizontalBGR;
	else if (strcmp(s, "NONE") == 0) subPixelOrder = SubPixelNone;
	PictureSetSubpixelOrder (pScreen, subPixelOrder);
    }
#endif

    pScrn->vtSema = TRUE;
    xf86SetBackingStore(pScreen);

    if (info->directRenderingEnabled) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Direct rendering enabled\n");
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Direct rendering disabled\n");
    }

    if (info->r600_shadow_fb) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Acceleration disabled\n");
	info->accelOn = FALSE;
    } else {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Initializing Acceleration\n");
	if (RADEONAccelInit(pScreen)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Acceleration enabled\n");
	    info->accelOn = TRUE;
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Acceleration initialization failed\n");
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Acceleration disabled\n");
	    info->accelOn = FALSE;
	}
    }

    /* Init DPMS */
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Initializing DPMS\n");
    xf86DPMSInit(pScreen, xf86DPMSSet, 0);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Initializing Cursor\n");

    /* Set Silken Mouse */
    xf86SetSilkenMouse(pScreen);

    /* Cursor setup */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    if (!xf86ReturnOptValBool(info->Options, OPTION_SW_CURSOR, FALSE)) {
	if (RADEONCursorInit_KMS(pScreen)) {
	}
    }

    /* DGA setup */
#ifdef XFreeXDGA
    /* DGA is dangerous on kms as the base and framebuffer location may change:
     * http://lists.freedesktop.org/archives/xorg-devel/2009-September/002113.html
     */
    /* xf86DiDGAInit(pScreen, info->LinearAddr + pScrn->fbOffset); */
#endif
    if (info->r600_shadow_fb == FALSE) {
        /* Init Xv */
        xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
                       "Initializing Xv\n");
        RADEONInitVideo(pScreen);
    }

    if (info->r600_shadow_fb == TRUE) {
        if (!shadowSetup(pScreen)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Shadowfb initialization failed\n");
            return FALSE;
        }
    }
    pScrn->pScreen = pScreen;

    /* Provide SaveScreen & wrap BlockHandler and CloseScreen */
    /* Wrap CloseScreen */
    info->CloseScreen    = pScreen->CloseScreen;
    pScreen->CloseScreen = RADEONCloseScreen_KMS;
    pScreen->SaveScreen  = RADEONSaveScreen_KMS;
    info->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = RADEONBlockHandler_KMS;

    if (!AddCallback(&FlushCallback, radeon_flush_callback, pScrn))
        return FALSE;

    info->CreateScreenResources = pScreen->CreateScreenResources;
    pScreen->CreateScreenResources = RADEONCreateScreenResources_KMS;

#ifdef RADEON_PIXMAP_SHARING
    pScreen->StartPixmapTracking = PixmapStartDirtyTracking;
    pScreen->StopPixmapTracking = PixmapStopDirtyTracking;
#endif

   if (!xf86CrtcScreenInit (pScreen))
       return FALSE;

   /* Wrap pointer motion to flip touch screen around */
//    info->PointerMoved = pScrn->PointerMoved;
//    pScrn->PointerMoved = RADEONPointerMoved;

    if (!drmmode_setup_colormap(pScreen, pScrn))
	return FALSE;

   /* Note unused options */
    if (serverGeneration == 1)
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);

    drmmode_init(pScrn, &info->drmmode);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONScreenInit finished\n");

    info->accel_state->XInited3D = FALSE;
    info->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;

    return TRUE;
}

Bool RADEONEnterVT_KMS(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONEnterVT_KMS\n");

    radeon_set_drm_master(pScrn);

    info->accel_state->XInited3D = FALSE;
    info->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;

    pScrn->vtSema = TRUE;

    if (!drmmode_set_desired_modes(pScrn, &info->drmmode))
	return FALSE;

    return TRUE;
}


void RADEONLeaveVT_KMS(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONLeaveVT_KMS\n");

    radeon_drop_drm_master(pScrn);

    xf86RotateFreeShadow(pScrn);

    xf86_hide_cursors (pScrn);
    info->accel_state->XInited3D = FALSE;
    info->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Ok, leaving now...\n");
}


Bool RADEONSwitchMode_KMS(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    Bool ret;
    ret = xf86SetSingleMode (pScrn, mode, RR_Rotate_0);
    return ret;

}

void RADEONAdjustFrame_KMS(ADJUST_FRAME_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    RADEONInfoPtr  info        = RADEONPTR(pScrn);
    drmmode_adjust_frame(pScrn, &info->drmmode, x, y);
    return;
}

static Bool radeon_setup_kernel_mem(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int cpp = info->pixel_bytes;
    uint32_t screen_size;
    int pitch, base_align;
    uint32_t tiling_flags = 0;
    struct radeon_surface surface;

    if (info->accel_state->exa != NULL) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "Memory map already initialized\n");
	return FALSE;
    }
    if (!info->use_glamor && info->r600_shadow_fb == FALSE) {
        info->accel_state->exa = exaDriverAlloc();
        if (info->accel_state->exa == NULL) {
	    xf86DrvMsg(pScreen->myNum, X_ERROR, "exaDriverAlloc failed\n");
	    return FALSE;
	}
    }

    if (info->allowColorTiling) {
	if (info->ChipFamily >= CHIP_FAMILY_R600) {
		if (info->allowColorTiling2D) {
			tiling_flags |= RADEON_TILING_MACRO;
		} else {
			tiling_flags |= RADEON_TILING_MICRO;
		}
	} else
	    tiling_flags |= RADEON_TILING_MACRO;
    }
    pitch = RADEON_ALIGN(pScrn->virtualX, drmmode_get_pitch_align(pScrn, cpp, tiling_flags)) * cpp;
    screen_size = RADEON_ALIGN(pScrn->virtualY, drmmode_get_height_align(pScrn, tiling_flags)) * pitch;
    base_align = drmmode_get_base_align(pScrn, cpp, tiling_flags);
	if (info->ChipFamily >= CHIP_FAMILY_R600) {
		if(!info->surf_man) {
			xf86DrvMsg(pScreen->myNum, X_ERROR,
				   "failed to initialise surface manager\n");
			return FALSE;
		}
		memset(&surface, 0, sizeof(struct radeon_surface));
		surface.npix_x = pScrn->virtualX;
		surface.npix_y = pScrn->virtualY;
		surface.npix_z = 1;
		surface.blk_w = 1;
		surface.blk_h = 1;
		surface.blk_d = 1;
		surface.array_size = 1;
		surface.last_level = 0;
		surface.bpe = cpp;
		surface.nsamples = 1;
		surface.flags = RADEON_SURF_SCANOUT;
		/* we are requiring a recent enough libdrm version */
		surface.flags |= RADEON_SURF_HAS_TILE_MODE_INDEX;
		surface.flags |= RADEON_SURF_SET(RADEON_SURF_TYPE_2D, TYPE);
		surface.flags |= RADEON_SURF_SET(RADEON_SURF_MODE_LINEAR_ALIGNED, MODE);
		if (tiling_flags & RADEON_TILING_MICRO) {
			surface.flags = RADEON_SURF_CLR(surface.flags, MODE);
			surface.flags |= RADEON_SURF_SET(RADEON_SURF_MODE_1D, MODE);
		}
		if (tiling_flags & RADEON_TILING_MACRO) {
			surface.flags = RADEON_SURF_CLR(surface.flags, MODE);
			surface.flags |= RADEON_SURF_SET(RADEON_SURF_MODE_2D, MODE);
		}
		if (radeon_surface_best(info->surf_man, &surface)) {
			xf86DrvMsg(pScreen->myNum, X_ERROR,
				   "radeon_surface_best failed\n");
			return FALSE;
		}
		if (radeon_surface_init(info->surf_man, &surface)) {
			xf86DrvMsg(pScreen->myNum, X_ERROR,
				   "radeon_surface_init failed\n");
			return FALSE;
		}
		pitch = surface.level[0].pitch_bytes;
		screen_size = surface.bo_size;
		base_align = surface.bo_alignment;
		tiling_flags = 0;
		switch (surface.level[0].mode) {
		case RADEON_SURF_MODE_2D:
			tiling_flags |= RADEON_TILING_MACRO;
			tiling_flags |= surface.bankw << RADEON_TILING_EG_BANKW_SHIFT;
			tiling_flags |= surface.bankh << RADEON_TILING_EG_BANKH_SHIFT;
			tiling_flags |= surface.mtilea << RADEON_TILING_EG_MACRO_TILE_ASPECT_SHIFT;
			tiling_flags |= eg_tile_split(surface.tile_split) << RADEON_TILING_EG_TILE_SPLIT_SHIFT;
			break;
		case RADEON_SURF_MODE_1D:
			tiling_flags |= RADEON_TILING_MICRO;
			break;
		default:
			break;
		}
		info->front_surface = surface;
	}
    {
	int cursor_size;
	int c;

	cursor_size = info->cursor_w * info->cursor_h * 4;
	cursor_size = RADEON_ALIGN(cursor_size, RADEON_GPU_PAGE_SIZE);
	for (c = 0; c < xf86_config->num_crtc; c++) {
	    /* cursor objects */
            if (info->cursor_bo[c] == NULL) {
                info->cursor_bo[c] = radeon_bo_open(info->bufmgr, 0,
                                                    cursor_size, 0,
                                                    RADEON_GEM_DOMAIN_VRAM, 0);
                if (!info->cursor_bo[c]) {
                    ErrorF("Failed to allocate cursor buffer memory\n");
                    return FALSE;
                }

                if (radeon_bo_map(info->cursor_bo[c], 1)) {
                    ErrorF("Failed to map cursor buffer memory\n");
                }

                drmmode_set_cursor(pScrn, &info->drmmode, c, info->cursor_bo[c]);
            }
        }
    }

    screen_size = RADEON_ALIGN(screen_size, RADEON_GPU_PAGE_SIZE);

    if (info->front_bo == NULL) {
        info->front_bo = radeon_bo_open(info->bufmgr, 0, screen_size,
                                        base_align, RADEON_GEM_DOMAIN_VRAM, 0);
        if (info->r600_shadow_fb == TRUE) {
            if (radeon_bo_map(info->front_bo, 1)) {
                ErrorF("Failed to map cursor buffer memory\n");
            }
        }
#if X_BYTE_ORDER == X_BIG_ENDIAN
	switch (cpp) {
	case 4:
	    tiling_flags |= RADEON_TILING_SWAP_32BIT;
	    break;
	case 2:
	    tiling_flags |= RADEON_TILING_SWAP_16BIT;
	    break;
	}
	if (info->ChipFamily < CHIP_FAMILY_R600 &&
	    info->r600_shadow_fb && tiling_flags)
	    tiling_flags |= RADEON_TILING_SURFACE;
#endif
	if (tiling_flags)
            radeon_bo_set_tiling(info->front_bo, tiling_flags, pitch);
    }

    pScrn->displayWidth = pitch / cpp;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Front buffer size: %dK\n", info->front_bo->size/1024);
    radeon_kms_update_vram_limit(pScrn, screen_size);
    return TRUE;
}

void radeon_kms_update_vram_limit(ScrnInfoPtr pScrn, uint32_t new_fb_size)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint64_t remain_size_bytes;
    int c;

    for (c = 0; c < xf86_config->num_crtc; c++) {
	if (info->cursor_bo[c] != NULL) {
	    new_fb_size += (64 * 4 * 64);
	}
    }

    remain_size_bytes = info->vram_size - new_fb_size;
    remain_size_bytes = (remain_size_bytes / 10) * 9;
    if (remain_size_bytes > 0xffffffff)
	remain_size_bytes = 0xffffffff;
    radeon_cs_set_limit(info->cs, RADEON_GEM_DOMAIN_VRAM,
			(uint32_t)remain_size_bytes);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VRAM usage limit set to %uK\n",
	       (uint32_t)remain_size_bytes / 1024);
}

/* Used to disallow modes that are not supported by the hardware */
ModeStatus RADEONValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode,
                           Bool verbose, int flag)
{
    SCRN_INFO_PTR(arg);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

    /*
     * RN50 has effective maximum mode bandwidth of about 300MiB/s.
     * XXX should really do this for all chips by properly computing
     * memory bandwidth and an overhead factor.
    */
    if (info->ChipFamily == CHIP_FAMILY_RV100 && !pRADEONEnt->HasCRTC2) {
       if (xf86ModeBandwidth(mode, pScrn->bitsPerPixel) > 300)
          return MODE_BANDWIDTH;
    }
    /* There are problems with double scan mode at high clocks
     * They're likely related PLL and display buffer settings.
     * Disable these modes for now.
     */
    if (mode->Flags & V_DBLSCAN) {
       if ((mode->CrtcHDisplay >= 1024) || (mode->CrtcVDisplay >= 768))
           return MODE_CLOCK_RANGE;
   }
    return MODE_OK;
}
