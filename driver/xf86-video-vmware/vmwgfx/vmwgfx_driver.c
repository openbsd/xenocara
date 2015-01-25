/*
 * Copyright 2008 Tungsten Graphics, Inc., Cedar Park, Texas.
 * Copyright 2011 VMWare, Inc.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * Author: Alan Hourihane <alanh@tungstengraphics.com>
 * Author: Jakob Bornecrantz <wallbraker@gmail.com>
 * Author: Thomas Hellstrom <thellstrom@vmware.com>
 */


#include <unistd.h>
#include "xorg-server.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"
#include "xf86Pci.h"
#include "mipointer.h"
#include "micmap.h"
#include <X11/extensions/randr.h>
#include "fb.h"
#include "edid.h"
#include "xf86i2c.h"
#include "xf86Crtc.h"
#include "miscstruct.h"
#include "dixstruct.h"
#include "xf86cmap.h"
#include "xf86xv.h"
#include "xorgVersion.h"
#ifndef XSERVER_LIBPCIACCESS
#error "libpciaccess needed"
#endif

#include <pciaccess.h>

#ifdef XSERVER_PLATFORM_BUS
#include "xf86platformBus.h"
#endif

#include "vmwgfx_driver.h"

#include <saa.h>
#include "vmwgfx_saa.h"
#include "../src/vmware_bootstrap.h"
#include "../src/vmware_common.h"
#include "vmwgfx_hosted.h"

/*
 * We can't incude svga_types.h due to conflicting types for Bool.
 */
typedef int64_t int64;
typedef uint64_t uint64;

typedef int32_t int32;
typedef uint32_t uint32;

typedef int16_t int16;
typedef uint16_t uint16;

typedef int8_t int8;
typedef uint8_t uint8;
#include "../src/svga_reg.h"

#define XA_VERSION_MINOR_REQUIRED 0
#define XA_VERSION_MAJOR_REQUIRED 1
#define XA_VERSION_MAJOR_COMPAT 2

#define DRM_VERSION_MAJOR_REQUIRED 2
#define DRM_VERSION_MINOR_REQUIRED 3

/*
 * Some macros to deal with function wrapping.
 */
#define vmwgfx_wrap(priv, real, mem, func) {\
	(priv)->saved_##mem = (real)->mem;	\
	(real)->mem = func;			\
}

#define vmwgfx_unwrap(priv, real, mem) {\
	(real)->mem = (priv)->saved_##mem;	\
}

#define vmwgfx_swap(priv, real, mem) {\
	void *tmp = (priv)->saved_##mem;		\
	(priv)->saved_##mem = (real)->mem;	\
	(real)->mem = tmp;			\
}

/*
 * Functions and symbols exported to Xorg via pointers.
 */

static Bool drv_pre_init(ScrnInfoPtr pScrn, int flags);
static Bool drv_screen_init(SCREEN_INIT_ARGS_DECL);
static Bool drv_switch_mode(SWITCH_MODE_ARGS_DECL);
static void drv_adjust_frame(ADJUST_FRAME_ARGS_DECL);
static Bool drv_enter_vt(VT_FUNC_ARGS_DECL);
static void drv_leave_vt(VT_FUNC_ARGS_DECL);
static void drv_free_screen(FREE_SCREEN_ARGS_DECL);
static ModeStatus drv_valid_mode(SCRN_ARG_TYPE arg, DisplayModePtr mode, Bool verbose,
			         int flags);

extern void xorg_tracker_set_functions(ScrnInfoPtr scrn);

void
vmwgfx_hookup(ScrnInfoPtr pScrn)
{
    pScrn->PreInit = drv_pre_init;
    pScrn->ScreenInit = drv_screen_init;
    pScrn->SwitchMode = drv_switch_mode;
    pScrn->FreeScreen = drv_free_screen;
    pScrn->ValidMode = drv_valid_mode;
}

void
vmwgfx_modify_flags(uint32_t *flags)
{
    *flags &= ~(HW_IO);
    vmwgfx_hosted_modify_flags(flags);
}
/*
 * Internal function definitions
 */

static Bool drv_close_screen(CLOSE_SCREEN_ARGS_DECL);

/*
 * Internal functions
 */

static Bool
drv_get_rec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate)
	return TRUE;

    pScrn->driverPrivate = xnfcalloc(1, sizeof(modesettingRec));

    return TRUE;
}

static void
drv_free_rec(ScrnInfoPtr pScrn)
{
    if (!pScrn)
	return;

    if (!pScrn->driverPrivate)
	return;

    free(pScrn->driverPrivate);

    pScrn->driverPrivate = NULL;
}

static void
drv_probe_ddc(ScrnInfoPtr pScrn, int index)
{
    ConfiguredMonitor = NULL;
}

static Bool
drv_crtc_resize(ScrnInfoPtr pScrn, int width, int height)
{
    modesettingPtr ms = modesettingPTR(pScrn);
    ScreenPtr pScreen = pScrn->pScreen;
    int old_width, old_height;
    PixmapPtr rootPixmap;

    if (width == pScrn->virtualX && height == pScrn->virtualY)
	return TRUE;

    if (ms->check_fb_size) {
	size_t size = width*(pScrn->bitsPerPixel / 8) * height + 1024;

	if (size > ms->max_fb_size) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Requested framebuffer size %dx%dx%d will not fit "
		       "in display memory.\n",
		       width, height, pScrn->bitsPerPixel);
	    return FALSE;
	}
    }

    old_width = pScrn->virtualX;
    old_height = pScrn->virtualY;
    pScrn->virtualX = width;
    pScrn->virtualY = height;

    /* ms->create_front_buffer will remove the old front buffer */

    rootPixmap = pScreen->GetScreenPixmap(pScreen);
    vmwgfx_disable_scanout(pScrn);
    if (!pScreen->ModifyPixmapHeader(rootPixmap, width, height, -1, -1, -1, NULL))
	goto error_modify;

    pScrn->displayWidth = rootPixmap->devKind / (rootPixmap->drawable.bitsPerPixel / 8);

    xf86SetDesiredModes(pScrn);
    return TRUE;

    /*
     * FIXME: Try out this error recovery path and fix problems.

     */
    //error_create:
    if (!pScreen->ModifyPixmapHeader(rootPixmap, old_width, old_height, -1, -1, -1, NULL))
	FatalError("failed to resize rootPixmap error path\n");

    pScrn->displayWidth = rootPixmap->devKind /
	(rootPixmap->drawable.bitsPerPixel / 8);


error_modify:
    pScrn->virtualX = old_width;
    pScrn->virtualY = old_height;

    if (xf86SetDesiredModes(pScrn))
	return FALSE;

    FatalError("failed to setup old framebuffer\n");
    return FALSE;
}

static const xf86CrtcConfigFuncsRec crtc_config_funcs = {
    .resize = drv_crtc_resize
};

static Bool vmwgfx_use_server_fd(modesettingPtr ms)
{
#ifdef XF86_PDEV_SERVER_FD
    return ms->platform_dev && (ms->platform_dev->flags & XF86_PDEV_SERVER_FD);
#else
    return FALSE;
#endif
}

static Bool
drv_init_drm(ScrnInfoPtr pScrn)
{
    modesettingPtr ms = modesettingPTR(pScrn);

    /* deal with server regeneration */
    if (ms->fd < 0) {

	ms->fd = vmwgfx_hosted_drm_fd(ms->hdriver, ms->hosted, ms->PciInfo);

#ifdef ODEV_ATTRIB_FD
	if (ms->fd < 0 && vmwgfx_use_server_fd(ms))
	    ms->fd = xf86_get_platform_device_int_attrib(ms->platform_dev,
	                                                 ODEV_ATTRIB_FD, -1);
#endif

	if (ms->fd < 0) {

	    char bus_id[64];

	    snprintf(bus_id, sizeof(bus_id), "PCI:%d:%d:%d",
		     ((ms->PciInfo->domain << 8) | ms->PciInfo->bus),
		     ms->PciInfo->dev, ms->PciInfo->func
		);

	    ms->fd = drmOpen("vmwgfx", bus_id);
	    ms->isMaster = TRUE;

	}

	if (ms->fd >= 0) {
	    drmVersionPtr ver = drmGetVersion(ms->fd);

	    if (ver == NULL) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Could not determine DRM version.\n");
		return FALSE;
	    }

	    ms->drm_major = ver->version_major;
	    ms->drm_minor = ver->version_minor;
	    ms->drm_patch = ver->version_patchlevel;

	    drmFreeVersion(ver);
	    return TRUE;
	}

	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Failed to open drm.\n");

	return FALSE;
    }

    return TRUE;
}

/**
 * vmwgfx_set_topology - Set the GUI topology according to an option string
 *
 * @pScrn: Pointer to a ScrnInfo struct.
 * @topology: String containing the topology description.
 * @info: Info describing the option used to invoke this function.
 *
 * This function reads a GUI topology according from @topology, and
 * calls into the kernel to set that topology.
 */
static Bool
vmwgfx_set_topology(ScrnInfoPtr pScrn, const char *topology, const char *info)
{
    modesettingPtr ms = modesettingPTR(pScrn);
    unsigned int num_outputs;
    xXineramaScreenInfo *screen_info;
    struct drm_vmw_rect *rects;
    int ret;
    unsigned int i;

    screen_info = VMWAREParseTopologyString(pScrn, topology, &num_outputs,
					    info);

    if (screen_info == NULL)
	return FALSE;

    rects = calloc(num_outputs, sizeof(*rects));
    if (rects == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Failed to allocate topology data.\n");
	goto out_no_rects;
    }

    for(i = 0; i < num_outputs; ++i) {
	rects[i].x = screen_info[i].x_org;
	rects[i].y = screen_info[i].y_org;
	rects[i].w = screen_info[i].width;
	rects[i].h = screen_info[i].height;
    }

    ret = vmwgfx_update_gui_layout(ms->fd, num_outputs, rects);
    free(rects);
    free(screen_info);

    return (ret == 0);

  out_no_rects:
    free(screen_info);
    return FALSE;
}


static Bool
vmwgfx_pre_init_mode(ScrnInfoPtr pScrn, int flags)
{
    modesettingPtr ms = modesettingPTR(pScrn);
    Bool ret = TRUE;

    ms->from_dp = (xf86GetOptValBool(ms->Options, OPTION_DIRECT_PRESENTS,
				     &ms->direct_presents)) ?
	X_CONFIG : X_DEFAULT;

    ms->from_hwp = (xf86GetOptValBool(ms->Options, OPTION_HW_PRESENTS,
				      &ms->only_hw_presents)) ?
	X_CONFIG : X_DEFAULT;

    /* Allocate an xf86CrtcConfig */
    xf86CrtcConfigInit(pScrn, &crtc_config_funcs);

    /* get max width and height */
    {
	drmModeResPtr res;
	int max_width, max_height;

	res = drmModeGetResources(ms->fd);
	max_width = res->max_width;
	max_height = res->max_height;

	xf86CrtcSetSizeRange(pScrn, res->min_width,
			     res->min_height, max_width, max_height);
	xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		   "Min width %d, Max Width %d.\n",
		   res->min_width, max_width);
	xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		   "Min height %d, Max Height %d.\n",
		   res->min_height, max_height);
	drmModeFreeResources(res);
    }

    ms->SWCursor = FALSE;
    if (!xf86ReturnOptValBool(ms->Options, OPTION_HW_CURSOR, TRUE)) {
	ms->SWCursor = TRUE;
    }

    if (xf86IsOptionSet(ms->Options, OPTION_GUI_LAYOUT)) {
	char *topology =
	    xf86GetOptValString(ms->Options, OPTION_GUI_LAYOUT);

	ret = FALSE;
	if (topology) {
	    ret = vmwgfx_set_topology(pScrn, topology, "gui");
	    free(topology);
	}

    } else if (xf86IsOptionSet(ms->Options, OPTION_STATIC_XINERAMA)) {
	char *topology =
	    xf86GetOptValString(ms->Options, OPTION_STATIC_XINERAMA);

	ret = FALSE;
	if (topology) {
	    ret = vmwgfx_set_topology(pScrn, topology, "static Xinerama");
	    free(topology);
	}
    }

    if (!ret)
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Falied parsing or setting "
		   "gui topology from config file.\n");

    xorg_crtc_init(pScrn);
    xorg_output_init(pScrn);

    if (!xf86InitialConfiguration(pScrn, TRUE)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes.\n");
	goto out_modes;
    }

    if (pScrn->modes == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No available modes.\n");
	goto out_modes;
    }

    pScrn->currentMode = pScrn->modes;

    return TRUE;

  out_modes:
    return FALSE;
}

static Bool
drv_pre_init(ScrnInfoPtr pScrn, int flags)
{
    modesettingPtr ms;
    rgb defaultWeight = { 0, 0, 0 };
    Gamma zeros = { 0.0, 0.0, 0.0 };
    EntityInfoPtr pEnt;
    uint64_t cap;

    if (pScrn->numEntities != 1)
	return FALSE;

    pEnt = xf86GetEntityInfo(pScrn->entityList[0]);

    if (flags & PROBE_DETECT) {
	drv_probe_ddc(pScrn, pEnt->index);
	return TRUE;
    }

    pScrn->driverPrivate = NULL;

    /* Allocate driverPrivate */
    if (!drv_get_rec(pScrn)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Failed to allocate driver private.\n");
    }

    ms = modesettingPTR(pScrn);
    ms->pEnt = pEnt;

    pScrn->displayWidth = 640;	       /* default it */

    ms->PciInfo = xf86GetPciInfoForEntity(ms->pEnt->index);
    if (!ms->PciInfo) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Incorrect bus for device.\n");
	goto out_err_bus;
    }

#ifdef XSERVER_PLATFORM_BUS
    if (pEnt->location.type == BUS_PLATFORM)
        ms->platform_dev = pEnt->location.id.plat;
#endif

    xf86SetPrimInitDone(pScrn->entityList[0]);

    ms->hdriver = vmwgfx_hosted_detect();
    ms->hosted = vmwgfx_hosted_create(ms->hdriver, pScrn);
    if (ms->hdriver && !ms->hosted) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Failed to set up compositor hosted environment.\n");
	goto out_err_bus;
    }

    pScrn->monitor = pScrn->confScreen->monitor;
    pScrn->progClock = TRUE;
    pScrn->rgbBits = 8;

    if (!xf86SetDepthBpp
	(pScrn, 0, 0, 0,
	 PreferConvert24to32 | SupportConvert24to32 | Support32bppFb)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to set depth and bpp.\n");
	goto out_err_bus;
    }

    if (!vmwgfx_hosted_pre_init(ms->hdriver, ms->hosted, flags))
	goto out_err_bus;

    ms->fd = -1;
    if (!drv_init_drm(pScrn))
	goto out_no_drm;

    if (ms->drm_major != DRM_VERSION_MAJOR_REQUIRED ||
	ms->drm_minor < DRM_VERSION_MINOR_REQUIRED) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "DRM driver version is %d.%d.%d\n",
		   ms->drm_major, ms->drm_minor, ms->drm_patch);
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "But KMS- and 3D functionality needs at least "
		   "%d.%d.0 to work.\n",
		   DRM_VERSION_MAJOR_REQUIRED,
		   DRM_VERSION_MINOR_REQUIRED);
	goto out_drm_version;
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		   "DRM driver version is %d.%d.%d\n",
		   ms->drm_major, ms->drm_minor, ms->drm_patch);
    }

    ms->check_fb_size = (vmwgfx_max_fb_size(ms->fd, &ms->max_fb_size) == 0);

    if (vmwgfx_get_param(ms->fd, DRM_VMW_PARAM_HW_CAPS, &cap) != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to detect device "
		   "screen object capability.\n");
	goto out_depth;
    }

    if ((cap & SVGA_CAP_SCREEN_OBJECT_2) == 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Device is not screen object "
		   "capable.\n");
	goto out_depth;
    }

    switch (pScrn->depth) {
    case 15:
    case 16:
    case 24:
	break;
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Given depth (%d) is not supported with KMS enabled.\n",
		   pScrn->depth);
	goto out_depth;
    }
    xf86PrintDepthBpp(pScrn);

    if (!xf86SetWeight(pScrn, defaultWeight, defaultWeight))
	goto out_depth;
    if (!xf86SetDefaultVisual(pScrn, -1))
	goto out_depth;

    /* Process the options */
    xf86CollectOptions(pScrn, NULL);
    if (!(ms->Options = VMWARECopyOptions()))
	goto out_depth;
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, ms->Options);

    ms->accelerate_render = TRUE;
    ms->from_render = xf86GetOptValBool(ms->Options, OPTION_RENDER_ACCEL,
					&ms->accelerate_render) ?
	X_CONFIG : X_PROBED;

    ms->rendercheck = FALSE;
    ms->from_rendercheck = xf86GetOptValBool(ms->Options, OPTION_RENDERCHECK,
					     &ms->rendercheck) ?
	X_CONFIG : X_DEFAULT;

    ms->enable_dri = ms->accelerate_render;
    ms->from_dri = xf86GetOptValBool(ms->Options, OPTION_DRI,
				     &ms->enable_dri) ?
	X_CONFIG : X_PROBED;

    ms->direct_presents = FALSE;
    ms->only_hw_presents = FALSE;
    ms->SWCursor = TRUE;
    if (!vmwgfx_is_hosted(ms->hdriver)) {
	if (!vmwgfx_pre_init_mode(pScrn, flags))
	    goto out_modes;
    } else {
	ms->from_dp = X_CONFIG;
	ms->from_hwp = X_CONFIG;
    }

    xf86SetDpi(pScrn, 0, 0);

    if (!xf86SetGamma(pScrn, zeros)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to set gamma.\n");
	goto out_modes;
    }

    /* Load the required sub modules */
    if (!xf86LoadSubModule(pScrn, "fb")) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to load fb module.\n");
	goto out_modes;
    }

    if (!xf86LoadSubModule(pScrn, "dri2")) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to load dri2 module.\n");
	goto out_modes;
    }

    return TRUE;

  out_modes:
    free(ms->Options);
  out_depth:
  out_drm_version:
    if (!vmwgfx_is_hosted(ms->hdriver) && !vmwgfx_use_server_fd(ms))
	close(ms->fd);
  out_no_drm:
    vmwgfx_hosted_destroy(ms->hdriver, ms->hosted);
  out_err_bus:
    drv_free_rec(pScrn);
    return FALSE;

}

static Bool
vmwgfx_scanout_update(int drm_fd, int fb_id, RegionPtr dirty)
{
    unsigned num_cliprects = REGION_NUM_RECTS(dirty);
    drmModeClip *clip = alloca(num_cliprects * sizeof(drmModeClip));
    BoxPtr rect = REGION_RECTS(dirty);
    int i, ret;

    if (!num_cliprects)
	return TRUE;

    for (i = 0; i < num_cliprects; i++, rect++) {
	clip[i].x1 = rect->x1;
	clip[i].y1 = rect->y1;
	clip[i].x2 = rect->x2;
	clip[i].y2 = rect->y2;
    }

    ret = drmModeDirtyFB(drm_fd, fb_id, clip, num_cliprects);
    if (ret)
	LogMessage(X_ERROR, "%s: failed to send dirty (%i, %s)\n",
		   __func__, ret, strerror(-ret));
    return (ret == 0);
}

static Bool
vmwgfx_scanout_present(ScreenPtr pScreen, int drm_fd,
		       struct vmwgfx_saa_pixmap *vpix,
		       RegionPtr dirty)
{
    uint32_t handle;
    unsigned int dummy;

    if (!REGION_NOTEMPTY(pScreen, dirty))
	return TRUE;

    if (!vpix->hw) {
	LogMessage(X_ERROR, "No surface to present from.\n");
	return FALSE;
    }

    if (_xa_surface_handle(vpix->hw, &handle, &dummy) != 0) {
	LogMessage(X_ERROR, "Could not get present surface handle.\n");
	return FALSE;
    }

    if (vmwgfx_present(drm_fd, vpix->fb_id, 0, 0, dirty, handle) != 0) {
	LogMessage(X_ERROR, "Failed present kernel call.\n");
	return FALSE;
    }

    return TRUE;
}

void xorg_flush(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    modesettingPtr ms = modesettingPTR(pScrn);
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
    PixmapPtr pixmap = NULL;
    struct vmwgfx_saa_pixmap *vpix;
    int i;
    xf86CrtcPtr crtc;
    PixmapPtr *pixmaps = calloc(config->num_crtc, sizeof(*pixmaps));
    unsigned int num_scanout = 0;
    unsigned int j;

    if (!pixmaps) {
	LogMessage(X_ERROR, "Failed memory allocation during screen "
		   "update.\n");
	return;
    }

    /*
     * Get an array of pixmaps from which we scan out.
     */
    for (i=0; i<config->num_crtc; ++i) {
	crtc = config->crtc[i];
	if (crtc->enabled) {
	    pixmap = crtc_get_scanout(crtc);
	    if (pixmap) {

		/*
		 * Remove duplicates.
		 */
		for (j=0; j<num_scanout; ++j) {
		    if (pixmap == pixmaps[j])
			break;
		}

		if (j == num_scanout)
		    pixmaps[num_scanout++] = pixmap;
	    }
	}
    }

    if (!num_scanout)
	return;

    for (j=0; j<num_scanout; ++j) {
	pixmap = pixmaps[j];
	vpix = vmwgfx_saa_pixmap(pixmap);

	if (vpix->fb_id != -1) {
	    if (vpix->pending_update) {
		if (ms->only_hw_presents &&
		    REGION_NOTEMPTY(pscreen, vpix->pending_update)) {
		    (void) vmwgfx_hw_accel_validate(pixmap, 0, XA_FLAG_SCANOUT,
						    0, NULL);
		    REGION_UNION(pScreen, vpix->pending_present,
				 vpix->pending_present, vpix->pending_update);
		} else
		    (void) vmwgfx_scanout_update(ms->fd, vpix->fb_id,
						 vpix->pending_update);
		REGION_EMPTY(pScreen, vpix->pending_update);
	    }
	    if (vpix->pending_present) {
		if (ms->only_hw_presents)
		    (void) vmwgfx_scanout_update(ms->fd, vpix->fb_id,
						 vpix->pending_present);
		else
		    (void) vmwgfx_scanout_present(pScreen, ms->fd, vpix,
						  vpix->pending_present);
		REGION_EMPTY(pScreen, vpix->pending_present);
	    }
	}
    }
    free(pixmaps);
}

static void drv_block_handler(BLOCKHANDLER_ARGS_DECL)
{
    SCREEN_PTR(arg);
    modesettingPtr ms = modesettingPTR(xf86ScreenToScrn(pScreen));

    vmwgfx_swap(ms, pScreen, BlockHandler);
    pScreen->BlockHandler(BLOCKHANDLER_ARGS);
    vmwgfx_swap(ms, pScreen, BlockHandler);

    if (vmwgfx_is_hosted(ms->hdriver))
	vmwgfx_hosted_post_damage(ms->hdriver, ms->hosted);
    else
	xorg_flush(pScreen);
}

static Bool
drv_create_screen_resources(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    modesettingPtr ms = modesettingPTR(pScrn);
    Bool ret;

    vmwgfx_swap(ms, pScreen, CreateScreenResources);
    ret = pScreen->CreateScreenResources(pScreen);
    vmwgfx_swap(ms, pScreen, CreateScreenResources);
    if (!ret)
	return ret;

    drv_adjust_frame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));

    return drv_enter_vt(VT_FUNC_ARGS);
}

static Bool
drv_set_master(ScrnInfoPtr pScrn)
{
    modesettingPtr ms = modesettingPTR(pScrn);

    if (!vmwgfx_is_hosted(ms->hdriver) && !vmwgfx_use_server_fd(ms) &&
            !ms->isMaster && drmSetMaster(ms->fd) != 0) {
	if (errno == EINVAL) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "drmSetMaster failed: 2.6.29 or newer kernel required for "
		       "multi-server DRI\n");
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "drmSetMaster failed: %s\n", strerror(errno));
	}
	return FALSE;
    }

    ms->isMaster = TRUE;
    return TRUE;
}

/**
 * vmwgfx_use_hw_cursor_argb - wrapper around hw argb cursor check.
 *
 * screen: Pointer to the current screen metadata.
 * cursor: Pointer to the current cursor metadata.
 *
 * In addition to the default test, also check whether we might be
 * needing more than one hw cursor (which we don't support).
 */
static Bool
vmwgfx_use_hw_cursor_argb(ScreenPtr screen, CursorPtr cursor)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(screen);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86CursorInfoPtr cursor_info = xf86_config->cursor_info;
    modesettingPtr ms = modesettingPTR(pScrn);
    Bool ret;

    vmwgfx_swap(ms, cursor_info, UseHWCursorARGB);
    ret = cursor_info->UseHWCursorARGB(screen, cursor);
    vmwgfx_swap(ms, cursor_info, UseHWCursorARGB);
    if (!ret)
	return FALSE;

    /*
     * If there is a chance we might need two cursors,
     * revert to sw cursor.
     */
    return !vmwgfx_output_explicit_overlap(pScrn);
}

/**
 * vmwgfx_use_hw_cursor - wrapper around hw cursor check.
 *
 * screen: Pointer to the current screen metadata.
 * cursor: Pointer to the current cursor metadata.
 *
 * In addition to the default test, also check whether we might be
 * needing more than one hw cursor (which we don't support).
 */
static Bool
vmwgfx_use_hw_cursor(ScreenPtr screen, CursorPtr cursor)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(screen);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86CursorInfoPtr cursor_info = xf86_config->cursor_info;
    modesettingPtr ms = modesettingPTR(pScrn);
    Bool ret;

    vmwgfx_swap(ms, cursor_info, UseHWCursor);
    ret = cursor_info->UseHWCursor(screen, cursor);
    vmwgfx_swap(ms, cursor_info, UseHWCursor);
    if (!ret)
	return FALSE;

    /*
     * If there is a chance we might need two simultaneous cursors,
     * revert to sw cursor.
     */
    return !vmwgfx_output_explicit_overlap(pScrn);
}

/**
 * vmwgfx_wrap_use_hw_cursor - Wrap functions that check for hw cursor
 * support.
 *
 * pScrn: Pointer to current screen info.
 *
 * Enables the device-specific hw cursor support check functions.
 */
static void vmwgfx_wrap_use_hw_cursor(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86CursorInfoPtr	cursor_info = xf86_config->cursor_info;
    modesettingPtr ms = modesettingPTR(pScrn);

    vmwgfx_wrap(ms, cursor_info, UseHWCursor, vmwgfx_use_hw_cursor);
    vmwgfx_wrap(ms, cursor_info, UseHWCursorARGB, vmwgfx_use_hw_cursor_argb);
}


static void drv_load_palette(ScrnInfoPtr pScrn, int numColors,
			     int *indices, LOCO *colors, VisualPtr pVisual)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    modesettingPtr ms = modesettingPTR(pScrn);
    int index, j, i;
    int c;

    switch(pScrn->depth) {
    case 15:
	for (i = 0; i < numColors; i++) {
	    index = indices[i];
	    for (j = 0; j < 8; j++) {
		ms->lut_r[index * 8 + j] = colors[index].red << 8;
		ms->lut_g[index * 8 + j] = colors[index].green << 8;
		ms->lut_b[index * 8 + j] = colors[index].blue << 8;
	    }
	}
	break;
    case 16:
	for (i = 0; i < numColors; i++) {
	    index = indices[i];

	    if (index < 32) {
		for (j = 0; j < 8; j++) {
		    ms->lut_r[index * 8 + j] = colors[index].red << 8;
		    ms->lut_b[index * 8 + j] = colors[index].blue << 8;
		}
	    }

	    for (j = 0; j < 4; j++) {
		ms->lut_g[index * 4 + j] = colors[index].green << 8;
	    }
	}
	break;
    default:
	for (i = 0; i < numColors; i++) {
	    index = indices[i];
	    ms->lut_r[index] = colors[index].red << 8;
	    ms->lut_g[index] = colors[index].green << 8;
	    ms->lut_b[index] = colors[index].blue << 8;
	}
	break;
    }

    for (c = 0; c < xf86_config->num_crtc; c++) {
	xf86CrtcPtr crtc = xf86_config->crtc[c];

	/* Make the change through RandR */
#ifdef RANDR_12_INTERFACE
	if (crtc->randr_crtc)
	    RRCrtcGammaSet(crtc->randr_crtc, ms->lut_r, ms->lut_g, ms->lut_b);
	else
#endif
	    crtc->funcs->gamma_set(crtc, ms->lut_r, ms->lut_g, ms->lut_b, 256);
    }
}


static Bool
drv_screen_init(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    modesettingPtr ms = modesettingPTR(pScrn);
    VisualPtr visual;

    if (!drv_set_master(pScrn))
	return FALSE;

    pScrn->pScreen = pScreen;

    /* HW dependent - FIXME */
    pScrn->displayWidth = pScrn->virtualX;

    miClearVisualTypes();

    if (!miSetVisualTypes(pScrn->depth,
			  miGetDefaultVisualMask(pScrn->depth),
			  pScrn->rgbBits, pScrn->defaultVisual))
	return FALSE;

    if (!miSetPixmapDepths())
	return FALSE;

    pScrn->memPhysBase = 0;
    pScrn->fbOffset = 0;

    if (!fbScreenInit(pScreen, NULL,
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

    vmwgfx_wrap(ms, pScreen, BlockHandler, drv_block_handler);
    vmwgfx_wrap(ms, pScreen, CreateScreenResources,
		drv_create_screen_resources);

    xf86SetBlackWhitePixels(pScreen);

    vmw_ctrl_ext_init(pScrn);

    if (ms->accelerate_render) {
	ms->xat = xa_tracker_create(ms->fd);
	if (!ms->xat) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Failed to initialize Gallium3D Xa. "
                       "No render acceleration available.\n");
	    ms->from_render = X_PROBED;
	} else {
	    int major, minor, patch;

	    xa_tracker_version(&major, &minor, &patch);
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Gallium3D XA version: %d.%d.%d.\n",
		       major, minor, patch);

	    if (major < XA_VERSION_MAJOR_REQUIRED ||
		major > XA_VERSION_MAJOR_COMPAT ||
		(major == XA_VERSION_MAJOR_REQUIRED &&
		 minor < XA_VERSION_MINOR_REQUIRED)) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "Expecting %d.%d.x >= XA version < %d.0.0.\n",
			   XA_VERSION_MAJOR_REQUIRED,
			   XA_VERSION_MINOR_REQUIRED,
			   XA_VERSION_MAJOR_COMPAT + 1);
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "No render acceleration available.\n");
		xa_tracker_destroy(ms->xat);
		ms->xat = NULL;
		ms->from_render = X_PROBED;
	    }
	}
	if (ms->xat == NULL && ms->rendercheck) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Turning off renercheck mode.\n");
	    ms->rendercheck = FALSE;
	    ms->from_rendercheck = X_PROBED;
	}
    }

    if (vmwgfx_is_hosted(ms->hdriver) && !ms->xat) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Can't run hosted without XA. Giving up.\n");
	return FALSE;
    }

    if (!vmwgfx_saa_init(pScreen, ms->fd, ms->xat, &xorg_flush,
			 ms->direct_presents,
			 ms->only_hw_presents,
			 ms->rendercheck)) {
	FatalError("Failed to initialize SAA.\n");
    }

    ms->dri2_available = FALSE;
    if (ms->enable_dri) {
	if (ms->xat) {
	    ms->dri2_available = xorg_dri2_init(pScreen);
	    if (!ms->dri2_available)
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Failed to initialize direct rendering.\n");
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Skipped initialization of direct rendering due "
		       "to lack of render acceleration.\n");
	    ms->from_dri = X_PROBED;
	}
    }

    xf86DrvMsg(pScrn->scrnIndex, ms->from_render, "Render acceleration is %s.\n",
	       (ms->xat != NULL) ? "enabled" : "disabled");

    xf86DrvMsg(pScrn->scrnIndex, ms->from_rendercheck,
	       "Rendercheck mode is %s.\n",
	       (ms->rendercheck) ? "enabled" : "disabled");

    xf86DrvMsg(pScrn->scrnIndex, ms->from_dri, "Direct rendering (3D) is %s.\n",
	       (ms->dri2_available) ? "enabled" : "disabled");
    if (ms->xat != NULL) {
	xf86DrvMsg(pScrn->scrnIndex, ms->from_dp, "Direct presents are %s.\n",
		   (ms->direct_presents) ? "enabled" : "disabled");
	xf86DrvMsg(pScrn->scrnIndex, ms->from_hwp, "Hardware only presents "
		   "are %s.\n",
		   (ms->only_hw_presents) ? "enabled" : "disabled");
    }

    xf86SetBackingStore(pScreen);
    xf86SetSilkenMouse(pScreen);
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    if (!vmwgfx_hosted_screen_init(ms->hdriver, ms->hosted, pScreen)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Failed hosted Screen init. Giving up.\n");
	return FALSE;
    }

    /* Need to extend HWcursor support to handle mask interleave */
    if (!ms->SWCursor) {
	xf86_cursors_init(pScreen, 64, 64,
			  HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_64 |
			  HARDWARE_CURSOR_ARGB |
			  HARDWARE_CURSOR_UPDATE_UNHIDDEN);
	vmwgfx_wrap_use_hw_cursor(pScrn);
    }

    /* Must force it before EnterVT, so we are in control of VT and
     * later memory should be bound when allocating, e.g rotate_mem */
    pScrn->vtSema = TRUE;

    pScreen->SaveScreen = xf86SaveScreen;
    vmwgfx_wrap(ms, pScreen, CloseScreen, drv_close_screen);

    if (!xf86CrtcScreenInit(pScreen))
	return FALSE;

    if (!miCreateDefColormap(pScreen))
	return FALSE;
    if (!xf86HandleColormaps(pScreen, 256, 8, drv_load_palette, NULL,
			     CMAP_PALETTED_TRUECOLOR |
			     CMAP_RELOAD_ON_MODE_SWITCH))
	return FALSE;

    xf86DPMSInit(pScreen, xf86DPMSSet, 0);

    if (serverGeneration == 1)
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);


    vmwgfx_wrap(ms, pScrn, EnterVT, drv_enter_vt);
    vmwgfx_wrap(ms, pScrn, LeaveVT, drv_leave_vt);
    vmwgfx_wrap(ms, pScrn, AdjustFrame, drv_adjust_frame);

    /*
     * Must be called _after_ function wrapping.
     */
    xorg_xv_init(pScreen);

    return TRUE;
}

static void
drv_adjust_frame(ADJUST_FRAME_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    modesettingPtr ms = modesettingPTR(pScrn);
    xf86CrtcConfigPtr config;
    xf86OutputPtr output;
    xf86CrtcPtr crtc;

    if (vmwgfx_is_hosted(ms->hdriver))
	return;

    config = XF86_CRTC_CONFIG_PTR(pScrn);
    output = config->output[config->compat_output];
    crtc = output->crtc;

    if (crtc && crtc->enabled) {
      //	crtc->funcs->set_mode_major(crtc, pScrn->currentMode,
      //				    RR_Rotate_0, x, y);
	crtc->x = output->initial_x + x;
	crtc->y = output->initial_y + y;
    }
}

static void
drv_free_screen(FREE_SCREEN_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    modesettingPtr ms = modesettingPTR(pScrn);

    vmwgfx_hosted_destroy(ms->hdriver, ms->hosted);
    drv_free_rec(pScrn);
}

static void
drv_leave_vt(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    modesettingPtr ms = modesettingPTR(pScrn);

    if (!vmwgfx_is_hosted(ms->hdriver)) {
	vmwgfx_cursor_bypass(ms->fd, 0, 0);
	vmwgfx_disable_scanout(pScrn);
    }

    vmwgfx_saa_drop_master(pScrn->pScreen);

    if (!vmwgfx_is_hosted(ms->hdriver) && !vmwgfx_use_server_fd(ms) &&
            drmDropMaster(ms->fd))
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "drmDropMaster failed: %s\n", strerror(errno));
    ms->isMaster = FALSE;
    pScrn->vtSema = FALSE;
}

/*
 * This gets called when gaining control of the VT, and from ScreenInit().
 */
static Bool
drv_enter_vt(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    modesettingPtr ms = modesettingPTR(pScrn);

    if (!drv_set_master(pScrn))
	return FALSE;

    vmwgfx_saa_set_master(pScrn->pScreen);

    if (!vmwgfx_is_hosted(ms->hdriver) && !xf86SetDesiredModes(pScrn))
	return FALSE;

    return TRUE;
}

static Bool
drv_switch_mode(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);

    return xf86SetSingleMode(pScrn, mode, RR_Rotate_0);
}

static Bool
drv_close_screen(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    modesettingPtr ms = modesettingPTR(pScrn);

    if (ms->cursor) {
       FreeCursor(ms->cursor, None);
       ms->cursor = NULL;
    }

    if (ms->dri2_available)
	xorg_dri2_close(pScreen);

    if (pScrn->vtSema)
        pScrn->LeaveVT(VT_FUNC_ARGS);

    pScrn->vtSema = FALSE;

    vmwgfx_unwrap(ms, pScrn, EnterVT);
    vmwgfx_unwrap(ms, pScrn, LeaveVT);
    vmwgfx_unwrap(ms, pScrn, AdjustFrame);
    vmwgfx_unwrap(ms, pScreen, CloseScreen);
    vmwgfx_hosted_screen_close(ms->hdriver, ms->hosted);
    vmwgfx_unwrap(ms, pScreen, BlockHandler);
    vmwgfx_unwrap(ms, pScreen, CreateScreenResources);

    if (ms->xat)
	xa_tracker_destroy(ms->xat);

    return (*pScreen->CloseScreen) (CLOSE_SCREEN_ARGS);
}

static ModeStatus
drv_valid_mode(SCRN_ARG_TYPE arg, DisplayModePtr mode, Bool verbose, int flags)
{
    return MODE_OK;
}

/* vim: set sw=4 ts=8 sts=4: */
