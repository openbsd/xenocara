/*
 * Copyright © 2007 Red Hat, Inc.
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

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>

#include "xorg-server.h"
#include "xorgVersion.h"

#include "intel.h"
#include "intel_bufmgr.h"
#include "intel_options.h"
#include "backlight.h"
#include "xf86drm.h"
#include "xf86drmMode.h"
#include "X11/Xatom.h"
#if defined(HAVE_X11_EXTENSIONS_DPMSCONST_H)
#include <X11/extensions/dpmsconst.h>
#else
#define DPMSModeOn 0
#define DPMSModeOff 3
#endif
#include "xf86DDC.h"
#include "fb.h"
#include "uxa.h"

#include "intel_glamor.h"

#define KNOWN_MODE_FLAGS ((1<<14)-1)

struct intel_drm_queue {
        struct list list;
        xf86CrtcPtr crtc;
        uint32_t seq;
        void *data;
        ScrnInfoPtr scrn;
        intel_drm_handler_proc handler;
        intel_drm_abort_proc abort;
};

static void
intel_drm_abort_scrn(ScrnInfoPtr scrn);

static uint32_t intel_drm_seq;
static struct list intel_drm_queue;

struct intel_mode {
	int fd;
	uint32_t fb_id;
	int cpp;

	drmEventContext event_context;
	int old_fb_id;
	int flip_count;
	uint64_t fe_msc;
	uint64_t fe_usec;

	struct list outputs;
	struct list crtcs;

	void *pageflip_data;
	intel_pageflip_handler_proc pageflip_handler;
	intel_pageflip_abort_proc pageflip_abort;

	Bool delete_dp_12_displays;
};

struct intel_pageflip {
	struct intel_mode *mode;
	Bool dispatch_me;
};

struct intel_crtc {
	struct intel_mode *mode;
	drmModeModeInfo kmode;
	drmModeCrtcPtr mode_crtc;
	int pipe;
	dri_bo *cursor;
	dri_bo *rotate_bo;
	uint32_t rotate_pitch;
	uint32_t rotate_fb_id;
	xf86CrtcPtr crtc;
	struct list link;
	PixmapPtr scanout_pixmap;
	uint32_t scanout_fb_id;
	int32_t vblank_offset;
	uint32_t msc_prev;
	uint64_t msc_high;
};

struct intel_property {
	drmModePropertyPtr mode_prop;
	uint64_t value;
	int num_atoms; /* if range prop, num_atoms == 1; if enum prop, num_atoms == num_enums + 1 */
	Atom *atoms;
};

struct intel_output {
	struct intel_mode *mode;
	int output_id;
	drmModeConnectorPtr mode_output;
	drmModeEncoderPtr *mode_encoders;
	drmModePropertyBlobPtr edid_blob;
	int num_props;
	struct intel_property *props;
	void *private_data;

	Bool has_panel_limits;
	int panel_hdisplay;
	int panel_vdisplay;

	int dpms_mode;
	struct backlight backlight;
	int backlight_active_level;
	xf86OutputPtr output;
	struct list link;
	int enc_mask;
	int enc_clone_mask;
};

static void
intel_output_dpms(xf86OutputPtr output, int mode);

static void
intel_output_dpms_backlight(xf86OutputPtr output, int oldmode, int mode);

static inline int
crtc_id(struct intel_crtc *crtc)
{
	return crtc->mode_crtc->crtc_id;
}

static void
intel_output_backlight_set(xf86OutputPtr output, int level)
{
	struct intel_output *intel_output = output->driver_private;
	if (backlight_set(&intel_output->backlight, level) < 0) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "failed to set backlight %s to brightness level %d, disabling\n",
			   intel_output->backlight.iface, level);
		backlight_disable(&intel_output->backlight);
	}
}

static int
intel_output_backlight_get(xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private;
	return backlight_get(&intel_output->backlight);
}

static void
intel_output_backlight_init(xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private;
	intel_screen_private *intel = intel_get_screen_private(output->scrn);
	char *str;

#if !USE_BACKLIGHT
	return;
#endif

	str = xf86GetOptValString(intel->Options, OPTION_BACKLIGHT);
	if (str != NULL) {
		if (backlight_exists(str) != BL_NONE) {
			intel_output->backlight_active_level =
				backlight_open(&intel_output->backlight,
					       strdup(str));
			if (intel_output->backlight_active_level != -1) {
				xf86DrvMsg(output->scrn->scrnIndex, X_CONFIG,
					   "found backlight control interface %s\n", str);
				return;
			}
		}

		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "unrecognised backlight control interface %s\n", str);
	}

	intel_output->backlight_active_level =
		backlight_open(&intel_output->backlight, NULL);
	if (intel_output->backlight_active_level != -1) {
		xf86DrvMsg(output->scrn->scrnIndex, X_PROBED,
			   "found backlight control interface %s\n",
			   intel_output->backlight.iface);
		return;
	}
}

static void
mode_from_kmode(ScrnInfoPtr scrn,
		drmModeModeInfoPtr kmode,
		DisplayModePtr	mode)
{
	memset(mode, 0, sizeof(DisplayModeRec));
	mode->status = MODE_OK;

	mode->Clock = kmode->clock;

	mode->HDisplay = kmode->hdisplay;
	mode->HSyncStart = kmode->hsync_start;
	mode->HSyncEnd = kmode->hsync_end;
	mode->HTotal = kmode->htotal;
	mode->HSkew = kmode->hskew;

	mode->VDisplay = kmode->vdisplay;
	mode->VSyncStart = kmode->vsync_start;
	mode->VSyncEnd = kmode->vsync_end;
	mode->VTotal = kmode->vtotal;
	mode->VScan = kmode->vscan;

	mode->Flags = kmode->flags;
	mode->name = strdup(kmode->name);

	if (kmode->type & DRM_MODE_TYPE_DRIVER)
		mode->type = M_T_DRIVER;
	if (kmode->type & DRM_MODE_TYPE_PREFERRED)
		mode->type |= M_T_PREFERRED;

	if (mode->status == MODE_OK && kmode->flags & ~KNOWN_MODE_FLAGS)
		mode->status = MODE_BAD; /* unknown flags => unhandled */

	xf86SetModeCrtc (mode, scrn->adjustFlags);
}

static void
mode_to_kmode(ScrnInfoPtr scrn,
	      drmModeModeInfoPtr kmode,
	      DisplayModePtr mode)
{
	memset(kmode, 0, sizeof(*kmode));

	kmode->clock = mode->Clock;
	kmode->hdisplay = mode->HDisplay;
	kmode->hsync_start = mode->HSyncStart;
	kmode->hsync_end = mode->HSyncEnd;
	kmode->htotal = mode->HTotal;
	kmode->hskew = mode->HSkew;

	kmode->vdisplay = mode->VDisplay;
	kmode->vsync_start = mode->VSyncStart;
	kmode->vsync_end = mode->VSyncEnd;
	kmode->vtotal = mode->VTotal;
	kmode->vscan = mode->VScan;

	kmode->flags = mode->Flags;
	if (mode->name)
		strncpy(kmode->name, mode->name, DRM_DISPLAY_MODE_LEN);
	kmode->name[DRM_DISPLAY_MODE_LEN-1] = 0;
}

static void
intel_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
}

void
intel_mode_disable_unused_functions(ScrnInfoPtr scrn)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	struct intel_mode *mode = intel_get_screen_private(scrn)->modes;
	int i;

	/* Force off for consistency between kernel and ddx */
	for (i = 0; i < xf86_config->num_crtc; i++) {
		xf86CrtcPtr crtc = xf86_config->crtc[i];
		if (!crtc->enabled)
			drmModeSetCrtc(mode->fd, crtc_id(crtc->driver_private),
				       0, 0, 0, NULL, 0, NULL);
	}
}

static Bool
intel_crtc_apply(xf86CrtcPtr crtc)
{
	ScrnInfoPtr scrn = crtc->scrn;
	struct intel_crtc *intel_crtc = crtc->driver_private;
	struct intel_mode *mode = intel_crtc->mode;
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	uint32_t *output_ids;
	int output_count = 0;
	int fb_id, x, y;
	int i, ret = FALSE;

	output_ids = calloc(sizeof(uint32_t), xf86_config->num_output);
	if (!output_ids)
		return FALSE;

	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];
		struct intel_output *intel_output;

		/* Make sure we mark the output as off (and save the backlight)
		 * before the kernel turns it off due to changing the pipe.
		 * This is necessary as the kernel may turn off the backlight
		 * and we lose track of the user settings.
		 */
		if (output->crtc == NULL)
			output->funcs->dpms(output, DPMSModeOff);

		if (output->crtc != crtc)
			continue;

		intel_output = output->driver_private;
		if (!intel_output->mode_output)
			return FALSE;

		output_ids[output_count] =
			intel_output->mode_output->connector_id;
		output_count++;
	}

	if (!intel_crtc->scanout_fb_id) {
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,5,99,0,0)
		if (!xf86CrtcRotate(crtc, mode, rotation))
			goto done;
#else
		if (!xf86CrtcRotate(crtc))
			goto done;
#endif
	}

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,7,0,0,0)
	crtc->funcs->gamma_set(crtc, crtc->gamma_red, crtc->gamma_green,
			       crtc->gamma_blue, crtc->gamma_size);
#endif

	x = crtc->x;
	y = crtc->y;
	fb_id = mode->fb_id;
	if (intel_crtc->rotate_fb_id) {
		fb_id = intel_crtc->rotate_fb_id;
		x = 0;
		y = 0;
	} else if (intel_crtc->scanout_fb_id && intel_crtc->scanout_pixmap->drawable.width >= crtc->mode.HDisplay && intel_crtc->scanout_pixmap->drawable.height >= crtc->mode.VDisplay) {
		fb_id = intel_crtc->scanout_fb_id;
		x = 0;
		y = 0;
	}
	ret = drmModeSetCrtc(mode->fd, crtc_id(intel_crtc),
			     fb_id, x, y, output_ids, output_count,
			     &intel_crtc->kmode);
	if (ret) {
		xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
			   "failed to set mode: %s\n", strerror(-ret));
		ret = FALSE;
	} else {
		ret = TRUE;

		/* Force DPMS to On for all outputs, which the kernel will have done
		 * with the mode set. Also, restore the backlight level
		 */
		for (i = 0; i < xf86_config->num_output; i++) {
		    xf86OutputPtr output = xf86_config->output[i];
		    struct intel_output *intel_output;

		    if (output->crtc != crtc)
			continue;

		    intel_output = output->driver_private;
		    intel_output_dpms_backlight(output, intel_output->dpms_mode, DPMSModeOn);
		    intel_output->dpms_mode = DPMSModeOn;
		}
	}

	if (scrn->pScreen)
		xf86_reload_cursors(scrn->pScreen);
        intel_drm_abort_scrn(scrn);

done:
	free(output_ids);
	return ret;
}

static Bool
intel_crtc_set_mode_major(xf86CrtcPtr crtc, DisplayModePtr mode,
			  Rotation rotation, int x, int y)
{
	ScrnInfoPtr scrn = crtc->scrn;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_crtc *intel_crtc = crtc->driver_private;
	struct intel_mode *intel_mode = intel_crtc->mode;
	int saved_x, saved_y;
	Rotation saved_rotation;
	DisplayModeRec saved_mode;
	int ret = TRUE;
	unsigned int pitch = scrn->displayWidth * intel->cpp;

	if (intel_mode->fb_id == 0) {
		ret = drmModeAddFB(intel_mode->fd,
				   scrn->virtualX, scrn->virtualY,
				   scrn->depth, scrn->bitsPerPixel,
				   pitch, intel->front_buffer->handle,
				   &intel_mode->fb_id);
		if (ret < 0) {
			ErrorF("failed to add fb\n");
			return FALSE;
		}

		drm_intel_bo_disable_reuse(intel->front_buffer);
	}

	saved_mode = crtc->mode;
	saved_x = crtc->x;
	saved_y = crtc->y;
	saved_rotation = crtc->rotation;

	crtc->mode = *mode;
	crtc->x = x;
	crtc->y = y;
	crtc->rotation = rotation;

	intel_glamor_flush(intel);
	intel_batch_submit(crtc->scrn);

	mode_to_kmode(crtc->scrn, &intel_crtc->kmode, mode);
	ret = intel_crtc_apply(crtc);
	if (!ret) {
		crtc->x = saved_x;
		crtc->y = saved_y;
		crtc->rotation = saved_rotation;
		crtc->mode = saved_mode;
	}
	return ret;
}

static void
intel_crtc_set_cursor_colors(xf86CrtcPtr crtc, int bg, int fg)
{

}

static void
intel_crtc_set_cursor_position (xf86CrtcPtr crtc, int x, int y)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;
	struct intel_mode *mode = intel_crtc->mode;

	drmModeMoveCursor(mode->fd, crtc_id(intel_crtc), x, y);
}

static int
__intel_crtc_load_cursor_argb(xf86CrtcPtr crtc, CARD32 *image)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;
	int ret;

	ret = dri_bo_subdata(intel_crtc->cursor, 0, 64*64*4, image);
	if (ret)
		xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
			   "failed to set cursor: %s\n", strerror(-ret));

	return ret;
}

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,15,99,902,2)
static Bool
intel_crtc_load_cursor_argb(xf86CrtcPtr crtc, CARD32 *image)
{
	return __intel_crtc_load_cursor_argb(crtc, image) == 0;
}
#else
static void
intel_crtc_load_cursor_argb(xf86CrtcPtr crtc, CARD32 *image)
{
	__intel_crtc_load_cursor_argb(crtc, image);
}
#endif

static void
intel_crtc_hide_cursor(xf86CrtcPtr crtc)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;
	struct intel_mode *mode = intel_crtc->mode;

	drmModeSetCursor(mode->fd, crtc_id(intel_crtc), 0, 64, 64);
}

static void
intel_crtc_show_cursor(xf86CrtcPtr crtc)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;
	struct intel_mode *mode = intel_crtc->mode;

	drmModeSetCursor(mode->fd, crtc_id(intel_crtc),
			 intel_crtc->cursor->handle, 64, 64);
}

static void *
intel_crtc_shadow_allocate(xf86CrtcPtr crtc, int width, int height)
{
	ScrnInfoPtr scrn = crtc->scrn;
	struct intel_crtc *intel_crtc = crtc->driver_private;
	struct intel_mode *mode = intel_crtc->mode;
	int rotate_pitch;
	uint32_t tiling;
	int ret;

	intel_crtc->rotate_bo = intel_allocate_framebuffer(scrn,
							     width, height,
							     mode->cpp,
							     &rotate_pitch,
							     &tiling);

	if (!intel_crtc->rotate_bo) {
		xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
			   "Couldn't allocate shadow memory for rotated CRTC\n");
		return NULL;
	}

	ret = drmModeAddFB(mode->fd, width, height, crtc->scrn->depth,
			   crtc->scrn->bitsPerPixel, rotate_pitch,
			   intel_crtc->rotate_bo->handle,
			   &intel_crtc->rotate_fb_id);
	if (ret) {
		ErrorF("failed to add rotate fb\n");
		drm_intel_bo_unreference(intel_crtc->rotate_bo);
		return NULL;
	}

	drm_intel_bo_disable_reuse(intel_crtc->rotate_bo);

	intel_crtc->rotate_pitch = rotate_pitch;
	return intel_crtc->rotate_bo;
}

static PixmapPtr
intel_crtc_shadow_create(xf86CrtcPtr crtc, void *data, int width, int height)
{
	ScrnInfoPtr scrn = crtc->scrn;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_crtc *intel_crtc = crtc->driver_private;
	PixmapPtr rotate_pixmap;

	if (!data) {
		data = intel_crtc_shadow_allocate (crtc, width, height);
		if (!data) {
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
				   "Couldn't allocate shadow pixmap for rotated CRTC\n");
			return NULL;
		}
	}
	if (intel_crtc->rotate_bo == NULL) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Couldn't allocate shadow pixmap for rotated CRTC\n");
		return NULL;
	}

	rotate_pixmap = GetScratchPixmapHeader(scrn->pScreen,
					       width, height,
					       scrn->depth,
					       scrn->bitsPerPixel,
					       intel_crtc->rotate_pitch,
					       NULL);

	if (rotate_pixmap == NULL) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Couldn't allocate shadow pixmap for rotated CRTC\n");
		return NULL;
	}

	intel_set_pixmap_bo(rotate_pixmap, intel_crtc->rotate_bo);

	intel->shadow_present = TRUE;

	return rotate_pixmap;
}

static void
intel_crtc_shadow_destroy(xf86CrtcPtr crtc, PixmapPtr rotate_pixmap, void *data)
{
	ScrnInfoPtr scrn = crtc->scrn;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_crtc *intel_crtc = crtc->driver_private;
	struct intel_mode *mode = intel_crtc->mode;

	if (rotate_pixmap) {
		intel_set_pixmap_bo(rotate_pixmap, NULL);
		FreeScratchPixmapHeader(rotate_pixmap);
	}

	if (data) {
		/* Be sure to sync acceleration before the memory gets
		 * unbound. */
		drmModeRmFB(mode->fd, intel_crtc->rotate_fb_id);
		intel_crtc->rotate_fb_id = 0;

		dri_bo_unreference(intel_crtc->rotate_bo);
		intel_crtc->rotate_bo = NULL;
	}

	intel->shadow_present = FALSE;
}

static void
intel_crtc_gamma_set(xf86CrtcPtr crtc,
		       CARD16 *red, CARD16 *green, CARD16 *blue, int size)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;
	struct intel_mode *mode = intel_crtc->mode;

	drmModeCrtcSetGamma(mode->fd, crtc_id(intel_crtc),
			    size, red, green, blue);
}

static void
intel_crtc_destroy(xf86CrtcPtr crtc)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;

	if (intel_crtc->cursor) {
		drmModeSetCursor(intel_crtc->mode->fd, crtc_id(intel_crtc), 0, 64, 64);
		drm_intel_bo_unreference(intel_crtc->cursor);
		intel_crtc->cursor = NULL;
	}

	list_del(&intel_crtc->link);
	free(intel_crtc);

	crtc->driver_private = NULL;
}

#ifdef INTEL_PIXMAP_SHARING
static Bool
intel_set_scanout_pixmap(xf86CrtcPtr crtc, PixmapPtr ppix)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;
	ScrnInfoPtr scrn = crtc->scrn;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	dri_bo *bo;

	if (ppix == intel_crtc->scanout_pixmap)
		return TRUE;

	if (!ppix) {
		intel_crtc->scanout_pixmap = NULL;
		if (intel_crtc->scanout_fb_id) {
			drmModeRmFB(intel->drmSubFD, intel_crtc->scanout_fb_id);
			intel_crtc->scanout_fb_id = 0;
		}
		return TRUE;
	}

	bo = intel_get_pixmap_bo(ppix);
	if (intel->front_buffer) {
		ErrorF("have front buffer\n");
	}

	drm_intel_bo_disable_reuse(bo);

	intel_crtc->scanout_pixmap = ppix;
	return drmModeAddFB(intel->drmSubFD, ppix->drawable.width,
			   ppix->drawable.height, ppix->drawable.depth,
			   ppix->drawable.bitsPerPixel, ppix->devKind,
			   bo->handle, &intel_crtc->scanout_fb_id) == 0;
}
#endif

static const xf86CrtcFuncsRec intel_crtc_funcs = {
	.dpms = intel_crtc_dpms,
	.set_mode_major = intel_crtc_set_mode_major,
	.set_cursor_colors = intel_crtc_set_cursor_colors,
	.set_cursor_position = intel_crtc_set_cursor_position,
	.show_cursor = intel_crtc_show_cursor,
	.hide_cursor = intel_crtc_hide_cursor,
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,15,99,902,3)
	.load_cursor_argb_check = intel_crtc_load_cursor_argb,
#else
	.load_cursor_argb = intel_crtc_load_cursor_argb,
#endif
	.shadow_create = intel_crtc_shadow_create,
	.shadow_allocate = intel_crtc_shadow_allocate,
	.shadow_destroy = intel_crtc_shadow_destroy,
	.gamma_set = intel_crtc_gamma_set,
	.destroy = intel_crtc_destroy,
#ifdef INTEL_PIXMAP_SHARING
	.set_scanout_pixmap = intel_set_scanout_pixmap,
#endif
};

static void
intel_crtc_init(ScrnInfoPtr scrn, struct intel_mode *mode, drmModeResPtr mode_res, int num)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	xf86CrtcPtr crtc;
	struct intel_crtc *intel_crtc;

	intel_crtc = calloc(sizeof(struct intel_crtc), 1);
	if (intel_crtc == NULL)
		return;

	crtc = xf86CrtcCreate(scrn, &intel_crtc_funcs);
	if (crtc == NULL) {
		free(intel_crtc);
		return;
	}

	intel_crtc->mode_crtc = drmModeGetCrtc(mode->fd,
					       mode_res->crtcs[num]);
	if (intel_crtc->mode_crtc == NULL) {
		free(intel_crtc);
		return;
	}

	intel_crtc->mode = mode;
	crtc->driver_private = intel_crtc;

	intel_crtc->pipe = drm_intel_get_pipe_from_crtc_id(intel->bufmgr,
							   crtc_id(intel_crtc));

	intel_crtc->cursor = drm_intel_bo_alloc(intel->bufmgr, "ARGB cursor",
						4*64*64, 4096);

	intel_crtc->crtc = crtc;
	list_add(&intel_crtc->link, &mode->crtcs);
}

static Bool
is_panel(int type)
{
	return (type == DRM_MODE_CONNECTOR_LVDS ||
		type == DRM_MODE_CONNECTOR_eDP);
}

static xf86OutputStatus
intel_output_detect(xf86OutputPtr output)
{
	/* go to the hw and retrieve a new output struct */
	struct intel_output *intel_output = output->driver_private;
	struct intel_mode *mode = intel_output->mode;
	xf86OutputStatus status;

	drmModeFreeConnector(intel_output->mode_output);
	intel_output->mode_output =
		drmModeGetConnector(mode->fd, intel_output->output_id);
	if (intel_output->mode_output == NULL) {
		/* and hope we are safe everywhere else */
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "drmModeGetConnector failed, reporting output disconnected\n");
		return XF86OutputStatusDisconnected;
	}

	switch (intel_output->mode_output->connection) {
	case DRM_MODE_CONNECTED:
		status = XF86OutputStatusConnected;
		break;
	case DRM_MODE_DISCONNECTED:
		status = XF86OutputStatusDisconnected;
		break;
	default:
	case DRM_MODE_UNKNOWNCONNECTION:
		status = XF86OutputStatusUnknown;
		break;
	}
	return status;
}

static Bool
intel_output_mode_valid(xf86OutputPtr output, DisplayModePtr pModes)
{
	struct intel_output *intel_output = output->driver_private;

	/*
	 * If the connector type is a panel, we will use the panel limit to
	 * verfiy whether the mode is valid.
	 */
	if (intel_output->has_panel_limits) {
		if (pModes->HDisplay > intel_output->panel_hdisplay ||
		    pModes->VDisplay > intel_output->panel_vdisplay)
			return MODE_PANEL;
	}

	return MODE_OK;
}

static void
intel_output_attach_edid(xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private;
	drmModeConnectorPtr koutput = intel_output->mode_output;
	struct intel_mode *mode = intel_output->mode;
	xf86MonPtr mon = NULL;
	int i;

	if (!koutput) {
		xf86OutputSetEDID(output, mon);
		return;
	}

	/* look for an EDID property */
	for (i = 0; i < koutput->count_props; i++) {
		drmModePropertyPtr props;

		props = drmModeGetProperty(mode->fd, koutput->props[i]);
		if (!props)
			continue;

		if (!(props->flags & DRM_MODE_PROP_BLOB)) {
			drmModeFreeProperty(props);
			continue;
		}

		if (!strcmp(props->name, "EDID")) {
			drmModeFreePropertyBlob(intel_output->edid_blob);
			intel_output->edid_blob =
				drmModeGetPropertyBlob(mode->fd,
						       koutput->prop_values[i]);
		}
		drmModeFreeProperty(props);
	}

	if (intel_output->edid_blob) {
		mon = xf86InterpretEDID(output->scrn->scrnIndex,
					intel_output->edid_blob->data);

		if (mon && intel_output->edid_blob->length > 128)
			mon->flags |= MONITOR_EDID_COMPLETE_RAWDATA;
	}

	xf86OutputSetEDID(output, mon);
}

static DisplayModePtr
intel_output_panel_edid(xf86OutputPtr output, DisplayModePtr modes)
{
	xf86MonPtr mon = output->MonInfo;

	if (!mon || !GTF_SUPPORTED(mon->features.msc)) {
		DisplayModePtr i, m, p = NULL;
		int max_x = 0, max_y = 0;
		float max_vrefresh = 0.0;

		for (m = modes; m; m = m->next) {
			if (m->type & M_T_PREFERRED)
				p = m;
			max_x = max(max_x, m->HDisplay);
			max_y = max(max_y, m->VDisplay);
			max_vrefresh = max(max_vrefresh, xf86ModeVRefresh(m));
		}

		max_vrefresh = max(max_vrefresh, 60.0);
		max_vrefresh *= (1 + SYNC_TOLERANCE);

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,6,99,0,0)
		m = xf86GetDefaultModes();
#else
		m = xf86GetDefaultModes(0,0);
#endif

		xf86ValidateModesSize(output->scrn, m, max_x, max_y, 0);

		for (i = m; i; i = i->next) {
			if (xf86ModeVRefresh(i) > max_vrefresh)
				i->status = MODE_VSYNC;
			if (p && i->HDisplay >= p->HDisplay &&
			    i->VDisplay >= p->VDisplay &&
			    xf86ModeVRefresh(i) >= xf86ModeVRefresh(p))
				i->status = MODE_VSYNC;
		}

		xf86PruneInvalidModes(output->scrn, &m, FALSE);

		modes = xf86ModesAdd(modes, m);
	}

	return modes;
}

static DisplayModePtr
intel_output_get_modes(xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private;
	drmModeConnectorPtr koutput = intel_output->mode_output;
	DisplayModePtr Modes = NULL;
	int i;

	intel_output_attach_edid(output);

	if (!koutput)
		return Modes;

	/* modes should already be available */
	for (i = 0; i < koutput->count_modes; i++) {
		DisplayModePtr Mode;

		Mode = calloc(1, sizeof(DisplayModeRec));
		if (Mode) {
			mode_from_kmode(output->scrn, &koutput->modes[i], Mode);
			Modes = xf86ModesAdd(Modes, Mode);
		}
	}

	/*
	 * If the connector type is a panel, we will traverse the kernel mode to
	 * get the panel limit. And then add all the standard modes to fake
	 * the fullscreen experience.
	 * If it is incorrect, please fix me.
	 */
	intel_output->has_panel_limits = FALSE;
	if (is_panel(koutput->connector_type)) {
		for (i = 0; i < koutput->count_modes; i++) {
			drmModeModeInfo *mode_ptr;

			mode_ptr = &koutput->modes[i];
			if (mode_ptr->hdisplay > intel_output->panel_hdisplay)
				intel_output->panel_hdisplay = mode_ptr->hdisplay;
			if (mode_ptr->vdisplay > intel_output->panel_vdisplay)
				intel_output->panel_vdisplay = mode_ptr->vdisplay;
		}

		intel_output->has_panel_limits =
			intel_output->panel_hdisplay &&
			intel_output->panel_vdisplay;

		Modes = intel_output_panel_edid(output, Modes);
	}

	return Modes;
}

static void
intel_output_destroy(xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private;
	int i;

	drmModeFreePropertyBlob(intel_output->edid_blob);

	for (i = 0; i < intel_output->num_props; i++) {
		drmModeFreeProperty(intel_output->props[i].mode_prop);
		free(intel_output->props[i].atoms);
	}
	free(intel_output->props);
	for (i = 0; i < intel_output->mode_output->count_encoders; i++) {
		drmModeFreeEncoder(intel_output->mode_encoders[i]);
	}
	free(intel_output->mode_encoders);
	drmModeFreeConnector(intel_output->mode_output);
	intel_output->mode_output = NULL;

	list_del(&intel_output->link);
	backlight_close(&intel_output->backlight);
	free(intel_output);

	output->driver_private = NULL;
}

static void
intel_output_dpms_backlight(xf86OutputPtr output, int oldmode, int mode)
{
	struct intel_output *intel_output = output->driver_private;

	if (!intel_output->backlight.iface)
		return;

	if (mode == DPMSModeOn) {
		/* If we're going from off->on we may need to turn on the backlight. */
		if (oldmode != DPMSModeOn)
			intel_output_backlight_set(output,
						   intel_output->backlight_active_level);
	} else {
		/* Only save the current backlight value if we're going from on to off. */
		if (oldmode == DPMSModeOn)
			intel_output->backlight_active_level = intel_output_backlight_get(output);
		intel_output_backlight_set(output, 0);
	}
}

static void
intel_output_dpms(xf86OutputPtr output, int dpms)
{
	struct intel_output *intel_output = output->driver_private;
	drmModeConnectorPtr koutput = intel_output->mode_output;
	struct intel_mode *mode = intel_output->mode;
	int i;

	if (!koutput)
		return;

	for (i = 0; i < koutput->count_props; i++) {
		drmModePropertyPtr props;

		props = drmModeGetProperty(mode->fd, koutput->props[i]);
		if (!props)
			continue;

		if (!strcmp(props->name, "DPMS")) {
			/* Make sure to reverse the order between on and off. */
			if (dpms != DPMSModeOn)
				intel_output_dpms_backlight(output,
							    intel_output->dpms_mode,
							    dpms);

			drmModeConnectorSetProperty(mode->fd,
						    intel_output->output_id,
						    props->prop_id,
						    dpms);

			if (dpms == DPMSModeOn)
				intel_output_dpms_backlight(output,
							    intel_output->dpms_mode,
							    dpms);
			intel_output->dpms_mode = dpms;
			drmModeFreeProperty(props);
			return;
		}

		drmModeFreeProperty(props);
	}
}

int
intel_output_dpms_status(xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private;
	return intel_output->dpms_mode;
}

static Bool
intel_property_ignore(drmModePropertyPtr prop)
{
	if (!prop)
		return TRUE;

	/* ignore blob prop */
	if (prop->flags & DRM_MODE_PROP_BLOB)
		return TRUE;

	/* ignore standard property */
	if (!strcmp(prop->name, "EDID") ||
	    !strcmp(prop->name, "DPMS"))
		return TRUE;

	return FALSE;
}

static void
intel_output_create_ranged_atom(xf86OutputPtr output, Atom *atom,
				const char *name, INT32 min, INT32 max,
				uint64_t value, Bool immutable)
{
	int err;
	INT32 atom_range[2];

	atom_range[0] = min;
	atom_range[1] = max;

	*atom = MakeAtom(name, strlen(name), TRUE);

	err = RRConfigureOutputProperty(output->randr_output, *atom, FALSE,
					TRUE, immutable, 2, atom_range);
	if (err != 0)
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "RRConfigureOutputProperty error, %d\n", err);

	err = RRChangeOutputProperty(output->randr_output, *atom, XA_INTEGER,
				     32, PropModeReplace, 1, &value, FALSE,
				     FALSE);
	if (err != 0)
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "RRChangeOutputProperty error, %d\n", err);
}

#define BACKLIGHT_NAME             "Backlight"
#define BACKLIGHT_DEPRECATED_NAME  "BACKLIGHT"
static Atom backlight_atom, backlight_deprecated_atom;

static void
intel_output_create_resources(xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private;
	drmModeConnectorPtr mode_output = intel_output->mode_output;
	struct intel_mode *mode = intel_output->mode;
	int i, j, err;

	intel_output->props = calloc(mode_output->count_props,
				     sizeof(struct intel_property));
	if (!intel_output->props)
		return;

	intel_output->num_props = 0;
	for (i = j = 0; i < mode_output->count_props; i++) {
		drmModePropertyPtr drmmode_prop;

		drmmode_prop = drmModeGetProperty(mode->fd,
						  mode_output->props[i]);
		if (intel_property_ignore(drmmode_prop)) {
			drmModeFreeProperty(drmmode_prop);
			continue;
		}

		intel_output->props[j].mode_prop = drmmode_prop;
		intel_output->props[j].value = mode_output->prop_values[i];
		j++;
	}
	intel_output->num_props = j;

	for (i = 0; i < intel_output->num_props; i++) {
		struct intel_property *p = &intel_output->props[i];
		drmModePropertyPtr drmmode_prop = p->mode_prop;

		if (drmmode_prop->flags & DRM_MODE_PROP_RANGE) {
			p->num_atoms = 1;
			p->atoms = calloc(p->num_atoms, sizeof(Atom));
			if (!p->atoms)
				continue;

			intel_output_create_ranged_atom(output, &p->atoms[0],
							drmmode_prop->name,
							drmmode_prop->values[0],
							drmmode_prop->values[1],
							p->value,
							drmmode_prop->flags & DRM_MODE_PROP_IMMUTABLE ? TRUE : FALSE);

		} else if (drmmode_prop->flags & DRM_MODE_PROP_ENUM) {
			p->num_atoms = drmmode_prop->count_enums + 1;
			p->atoms = calloc(p->num_atoms, sizeof(Atom));
			if (!p->atoms)
				continue;

			p->atoms[0] = MakeAtom(drmmode_prop->name, strlen(drmmode_prop->name), TRUE);
			for (j = 1; j <= drmmode_prop->count_enums; j++) {
				struct drm_mode_property_enum *e = &drmmode_prop->enums[j-1];
				p->atoms[j] = MakeAtom(e->name, strlen(e->name), TRUE);
			}

			err = RRConfigureOutputProperty(output->randr_output, p->atoms[0],
							FALSE, FALSE,
							drmmode_prop->flags & DRM_MODE_PROP_IMMUTABLE ? TRUE : FALSE,
							p->num_atoms - 1, (INT32 *)&p->atoms[1]);
			if (err != 0) {
				xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
					   "RRConfigureOutputProperty error, %d\n", err);
			}

			for (j = 0; j < drmmode_prop->count_enums; j++)
				if (drmmode_prop->enums[j].value == p->value)
					break;
			/* there's always a matching value */
			err = RRChangeOutputProperty(output->randr_output, p->atoms[0],
						     XA_ATOM, 32, PropModeReplace, 1, &p->atoms[j+1], FALSE, FALSE);
			if (err != 0) {
				xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
					   "RRChangeOutputProperty error, %d\n", err);
			}
		}
	}

	if (intel_output->backlight.iface) {
		/* Set up the backlight property, which takes effect
		 * immediately and accepts values only within the
		 * backlight_range.
		 */
		intel_output_create_ranged_atom(output, &backlight_atom,
					BACKLIGHT_NAME, 0,
					intel_output->backlight.max,
					intel_output->backlight_active_level,
					FALSE);
		intel_output_create_ranged_atom(output,
					&backlight_deprecated_atom,
					BACKLIGHT_DEPRECATED_NAME, 0,
					intel_output->backlight.max,
					intel_output->backlight_active_level,
					FALSE);
	}
}

static Bool
intel_output_set_property(xf86OutputPtr output, Atom property,
			    RRPropertyValuePtr value)
{
	struct intel_output *intel_output = output->driver_private;
	struct intel_mode *mode = intel_output->mode;
	int i;

	if (property == backlight_atom || property == backlight_deprecated_atom) {
		INT32 val;

		if (value->type != XA_INTEGER || value->format != 32 ||
		    value->size != 1)
		{
			return FALSE;
		}

		val = *(INT32 *)value->data;
		if (val < 0 || val > intel_output->backlight.max)
			return FALSE;

		if (intel_output->dpms_mode == DPMSModeOn)
			intel_output_backlight_set(output, val);
		intel_output->backlight_active_level = val;
		return TRUE;
	}

	for (i = 0; i < intel_output->num_props; i++) {
		struct intel_property *p = &intel_output->props[i];

		if (p->atoms[0] != property)
			continue;

		if (p->mode_prop->flags & DRM_MODE_PROP_RANGE) {
			uint32_t val;

			if (value->type != XA_INTEGER || value->format != 32 ||
			    value->size != 1)
				return FALSE;
			val = *(uint32_t *)value->data;

			drmModeConnectorSetProperty(mode->fd, intel_output->output_id,
						    p->mode_prop->prop_id, (uint64_t)val);
			return TRUE;
		} else if (p->mode_prop->flags & DRM_MODE_PROP_ENUM) {
			Atom	atom;
			const char	*name;
			int		j;

			if (value->type != XA_ATOM || value->format != 32 || value->size != 1)
				return FALSE;
			memcpy(&atom, value->data, 4);
			name = NameForAtom(atom);
			if (name == NULL)
				return FALSE;

			/* search for matching name string, then set its value down */
			for (j = 0; j < p->mode_prop->count_enums; j++) {
				if (!strcmp(p->mode_prop->enums[j].name, name)) {
					drmModeConnectorSetProperty(mode->fd, intel_output->output_id,
								    p->mode_prop->prop_id, p->mode_prop->enums[j].value);
					return TRUE;
				}
			}
			return FALSE;
		}
	}

	/* We didn't recognise this property, just report success in order
	 * to allow the set to continue, otherwise we break setting of
	 * common properties like EDID.
	 */
	return TRUE;
}

static Bool
intel_output_get_property(xf86OutputPtr output, Atom property)
{
	struct intel_output *intel_output = output->driver_private;
	int err;

	if (property == backlight_atom || property == backlight_deprecated_atom) {
		INT32 val;

		if (!intel_output->backlight.iface)
			return FALSE;

		if (intel_output->dpms_mode == DPMSModeOn) {
			val = intel_output_backlight_get(output);
			if (val < 0)
				return FALSE;
		} else {
			val = intel_output->backlight_active_level;
		}

		err = RRChangeOutputProperty(output->randr_output, property,
					     XA_INTEGER, 32, PropModeReplace, 1, &val,
					     FALSE, FALSE);
		if (err != 0) {
			xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
				   "RRChangeOutputProperty error, %d\n", err);
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

static const xf86OutputFuncsRec intel_output_funcs = {
	.create_resources = intel_output_create_resources,
#ifdef RANDR_12_INTERFACE
	.set_property = intel_output_set_property,
	.get_property = intel_output_get_property,
#endif
	.dpms = intel_output_dpms,
#if 0

	.save = drmmode_crt_save,
	.restore = drmmode_crt_restore,
	.mode_fixup = drmmode_crt_mode_fixup,
	.prepare = intel_output_prepare,
	.mode_set = drmmode_crt_mode_set,
	.commit = intel_output_commit,
#endif
	.detect = intel_output_detect,
	.mode_valid = intel_output_mode_valid,

	.get_modes = intel_output_get_modes,
	.destroy = intel_output_destroy
};

static const int subpixel_conv_table[7] = {
       	0,
       	SubPixelUnknown,
	SubPixelHorizontalRGB,
	SubPixelHorizontalBGR,
	SubPixelVerticalRGB,
	SubPixelVerticalBGR,
	SubPixelNone
};

static const char *output_names[] = {
       	"None",
	"VGA",
	"DVI",
	"DVI",
	"DVI",
	"Composite",
	"TV",
	"LVDS",
	"CTV",
	"DIN",
	"DP",
	"HDMI",
	"HDMI",
	"TV",
	"eDP",
};

static xf86OutputPtr find_output(ScrnInfoPtr pScrn, int id)
{
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	int i;
	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];
		struct intel_output *intel_output;

		intel_output = output->driver_private;
		if (intel_output->output_id == id)
			return output;
	}
	return NULL;
}

static int parse_path_blob(drmModePropertyBlobPtr path_blob, int *conn_base_id, char **path)
{
	char *conn;
	char conn_id[5];
	int id, len;
	char *blob_data;

	if (!path_blob)
		return -1;

	blob_data = path_blob->data;
	/* we only handle MST paths for now */
	if (strncmp(blob_data, "mst:", 4))
		return -1;

	conn = strchr(blob_data + 4, '-');
	if (!conn)
		return -1;
	len = conn - (blob_data + 4);
	if (len + 1 > 5)
		return -1;
	memcpy(conn_id, blob_data + 4, len);
	conn_id[len] = '\0';
	id = strtoul(conn_id, NULL, 10);

	*conn_base_id = id;

	*path = conn + 1;
	return 0;
}

static void
drmmode_create_name(ScrnInfoPtr pScrn, drmModeConnectorPtr koutput, char *name,
		    drmModePropertyBlobPtr path_blob)
{
	xf86OutputPtr output;
	int conn_id;
	char *extra_path;

	output = NULL;
	if (parse_path_blob(path_blob, &conn_id, &extra_path) == 0)
		output = find_output(pScrn, conn_id);
	if (output) {
		snprintf(name, 32, "%s-%s", output->name, extra_path);
	} else {
		const char *output_name;

		if (koutput->connector_type < ARRAY_SIZE(output_names))
			output_name = output_names[koutput->connector_type];
		else
			output_name = "UNKNOWN";

		snprintf(name, 32, "%s%d",
			 output_name, koutput->connector_type_id);
	}
}

static void
intel_output_init(ScrnInfoPtr scrn, struct intel_mode *mode, drmModeResPtr mode_res, int num, int dynamic)
{
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	xf86OutputPtr output;
	drmModeConnectorPtr koutput;
	drmModeEncoderPtr *kencoders = NULL;
	struct intel_output *intel_output;
	char name[32];
	drmModePropertyPtr props;
	drmModePropertyBlobPtr path_blob = NULL;
	int i;

	koutput = drmModeGetConnector(mode->fd,
				      mode_res->connectors[num]);
	if (!koutput)
		return;
	for (i = 0; i < koutput->count_props; i++) {
		props = drmModeGetProperty(mode->fd, koutput->props[i]);
		if (props && (props->flags & DRM_MODE_PROP_BLOB)) {
			if (!strcmp(props->name, "PATH")) {
				path_blob = drmModeGetPropertyBlob(mode->fd, koutput->prop_values[i]);

				drmModeFreeProperty(props);
				break;
			}
			drmModeFreeProperty(props);
		}
	}

	drmmode_create_name(scrn, koutput, name, path_blob);
	if (path_blob)
		drmModeFreePropertyBlob(path_blob);

	if (path_blob && dynamic) {
		/* See if we have an output with this name already
		 * and hook stuff up.
		 */
		for (i = 0; i < xf86_config->num_output; i++) {
			output = xf86_config->output[i];

			if (strncmp(output->name, name, 32))
				continue;

			intel_output = output->driver_private;
			intel_output->output_id = mode_res->connectors[num];
			intel_output->mode_output = koutput;
			return;
		}
	}
	kencoders = calloc(sizeof(drmModeEncoderPtr), koutput->count_encoders);
	if (!kencoders) {
		goto out_free_encoders;
	}

	for (i = 0; i < koutput->count_encoders; i++) {
		kencoders[i] = drmModeGetEncoder(mode->fd, koutput->encoders[i]);
		if (!kencoders[i])
			goto out_free_encoders;
	}

	output = xf86OutputCreate (scrn, &intel_output_funcs, name);
	if (!output) {
		goto out_free_encoders;
	}

	intel_output = calloc(sizeof(struct intel_output), 1);
	if (!intel_output) {
		xf86OutputDestroy(output);
		goto out_free_encoders;
	}

	intel_output->output_id = mode_res->connectors[num];
	intel_output->mode_output = koutput;
	intel_output->mode_encoders = kencoders;
	intel_output->mode = mode;

	output->mm_width = koutput->mmWidth;
	output->mm_height = koutput->mmHeight;

	output->subpixel_order = subpixel_conv_table[koutput->subpixel];
	output->driver_private = intel_output;

	if (is_panel(koutput->connector_type))
		intel_output_backlight_init(output);

	output->possible_crtcs = 0x7f;
	for (i = 0; i < koutput->count_encoders; i++) {
		output->possible_crtcs &= kencoders[i]->possible_crtcs;
	}
	output->interlaceAllowed = TRUE;

	intel_output->output = output;

	if (dynamic) {
		output->randr_output = RROutputCreate(xf86ScrnToScreen(scrn), output->name, strlen(output->name), output);
		intel_output_create_resources(output);
	}

	list_add(&intel_output->link, &mode->outputs);
	return;

out_free_encoders:
	if (kencoders) {
		for (i = 0; i < koutput->count_encoders; i++)
			drmModeFreeEncoder(kencoders[i]);
		free(kencoders);
	}
	drmModeFreeConnector(koutput);
}

static Bool
intel_xf86crtc_resize(ScrnInfoPtr scrn, int width, int height)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	struct intel_crtc *intel_crtc = xf86_config->crtc[0]->driver_private;
	struct intel_mode *mode = intel_crtc->mode;
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *old_front = NULL;
	Bool	    ret;
	uint32_t    old_fb_id;
	int	    i, old_width, old_height, old_pitch;
	int pitch;
	uint32_t tiling;
	ScreenPtr screen;

	if (scrn->virtualX == width && scrn->virtualY == height)
		return TRUE;

	intel_glamor_flush(intel);
	intel_batch_submit(scrn);

	old_width = scrn->virtualX;
	old_height = scrn->virtualY;
	old_pitch = scrn->displayWidth;
	old_fb_id = mode->fb_id;
	old_front = intel->front_buffer;

	if (intel->back_pixmap) {
		screen = intel->back_pixmap->drawable.pScreen;
		screen->DestroyPixmap(intel->back_pixmap);
		intel->back_pixmap = NULL;
	}

	if (intel->back_buffer) {
		drm_intel_bo_unreference(intel->back_buffer);
		intel->back_buffer = NULL;
	}

	intel->front_buffer = intel_allocate_framebuffer(scrn,
							 width, height,
							 intel->cpp,
							 &pitch, &tiling);
	if (!intel->front_buffer)
		goto fail;

	ret = drmModeAddFB(mode->fd, width, height, scrn->depth,
			   scrn->bitsPerPixel, pitch,
			   intel->front_buffer->handle,
			   &mode->fb_id);
	if (ret)
		goto fail;

	drm_intel_bo_disable_reuse(intel->front_buffer);
	intel->front_pitch = pitch;
	intel->front_tiling = tiling;

	scrn->virtualX = width;
	scrn->virtualY = height;

	if (!intel_uxa_create_screen_resources(scrn->pScreen))
		goto fail;

	for (i = 0; i < xf86_config->num_crtc; i++) {
		xf86CrtcPtr crtc = xf86_config->crtc[i];

		if (!crtc->enabled)
			continue;

		if (!intel_crtc_apply(crtc))
			goto fail;
	}

	if (old_fb_id)
		drmModeRmFB(mode->fd, old_fb_id);
	if (old_front)
		drm_intel_bo_unreference(old_front);

	return TRUE;

fail:
	if (intel->front_buffer)
		drm_intel_bo_unreference(intel->front_buffer);
	intel->front_buffer = old_front;
	scrn->virtualX = old_width;
	scrn->virtualY = old_height;
	scrn->displayWidth = old_pitch;
	if (old_fb_id != mode->fb_id)
		drmModeRmFB(mode->fd, mode->fb_id);
	mode->fb_id = old_fb_id;

	return FALSE;
}

static void
intel_pageflip_handler(ScrnInfoPtr scrn, xf86CrtcPtr crtc,
                        uint64_t frame, uint64_t usec, void *data);

static void
intel_pageflip_abort(ScrnInfoPtr scrn, xf86CrtcPtr crtc, void *data);

static void
intel_pageflip_complete(struct intel_mode *mode);

static void
intel_drm_abort_seq (ScrnInfoPtr scrn, uint32_t seq);

Bool
intel_do_pageflip(intel_screen_private *intel,
		  dri_bo *new_front,
		  int ref_crtc_hw_id,
		  Bool async,
		  void *pageflip_data,
		  intel_pageflip_handler_proc pageflip_handler,
		  intel_pageflip_abort_proc pageflip_abort)
{
	ScrnInfoPtr scrn = intel->scrn;
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	struct intel_crtc *crtc = config->crtc[0]->driver_private;
	struct intel_mode *mode = crtc->mode;
	unsigned int pitch = scrn->displayWidth * intel->cpp;
	struct intel_pageflip *flip;
	uint32_t new_fb_id;
	uint32_t flags;
	uint32_t seq;
	int i;

	/*
	 * Create a new handle for the back buffer
	 */
	if (drmModeAddFB(mode->fd, scrn->virtualX, scrn->virtualY,
			 scrn->depth, scrn->bitsPerPixel, pitch,
			 new_front->handle, &new_fb_id))
		goto error_out;

	drm_intel_bo_disable_reuse(new_front);
	intel_glamor_flush(intel);
	intel_batch_submit(scrn);

	mode->pageflip_data = pageflip_data;
	mode->pageflip_handler = pageflip_handler;
	mode->pageflip_abort = pageflip_abort;

	/*
	 * Queue flips on all enabled CRTCs
	 * Note that if/when we get per-CRTC buffers, we'll have to update this.
	 * Right now it assumes a single shared fb across all CRTCs, with the
	 * kernel fixing up the offset of each CRTC as necessary.
	 *
	 * Also, flips queued on disabled or incorrectly configured displays
	 * may never complete; this is a configuration error.
	 */
	mode->fe_msc = 0;
	mode->fe_usec = 0;

	flags = DRM_MODE_PAGE_FLIP_EVENT;
	if (async)
		flags |= DRM_MODE_PAGE_FLIP_ASYNC;
	for (i = 0; i < config->num_crtc; i++) {
		if (!intel_crtc_on(config->crtc[i]))
			continue;

		crtc = config->crtc[i]->driver_private;

		flip = calloc(1, sizeof(struct intel_pageflip));
		if (flip == NULL) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "flip queue: carrier alloc failed.\n");
			goto error_undo;
		}

		/* Only the reference crtc will finally deliver its page flip
		 * completion event. All other crtc's events will be discarded.
		 */
		flip->dispatch_me = (intel_crtc_to_pipe(crtc->crtc) == ref_crtc_hw_id);
		flip->mode = mode;

		seq = intel_drm_queue_alloc(scrn, config->crtc[i], flip, intel_pageflip_handler, intel_pageflip_abort);
		if (!seq) {
			free(flip);
			goto error_undo;
		}

again:
		if (drmModePageFlip(mode->fd,
				    crtc_id(crtc),
				    new_fb_id,
				    flags, (void *)(uintptr_t)seq)) {
			if (intel_mode_read_drm_events(intel)) {
				xf86DrvMsg(scrn->scrnIndex, X_WARNING,
					   "flip queue retry\n");
				goto again;
			}
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "flip queue failed: %s\n", strerror(errno));
			if (seq)
				intel_drm_abort_seq(scrn, seq);
			free(flip);
			goto error_undo;
		}
		mode->flip_count++;
	}

	mode->old_fb_id = mode->fb_id;
	mode->fb_id = new_fb_id;

	if (!mode->flip_count)
		intel_pageflip_complete(mode);

	return TRUE;

error_undo:
	drmModeRmFB(mode->fd, new_fb_id);
	for (i = 0; i < config->num_crtc; i++) {
		if (config->crtc[i]->enabled)
			intel_crtc_apply(config->crtc[i]);
	}

error_out:
	xf86DrvMsg(scrn->scrnIndex, X_WARNING, "Page flip failed: %s\n",
		   strerror(errno));

	mode->flip_count = 0;
	return FALSE;
}

static const xf86CrtcConfigFuncsRec intel_xf86crtc_config_funcs = {
	intel_xf86crtc_resize
};

/*
 * Enqueue a potential drm response; when the associated response
 * appears, we've got data to pass to the handler from here
 */
uint32_t
intel_drm_queue_alloc(ScrnInfoPtr scrn,
		      xf86CrtcPtr crtc,
		      void *data,
		      intel_drm_handler_proc handler,
		      intel_drm_abort_proc abort)
{
	struct intel_drm_queue  *q;

	q = calloc(1, sizeof(struct intel_drm_queue));
	if (!q)
		return 0;

	if (!intel_drm_seq)
		++intel_drm_seq;
	q->seq = intel_drm_seq++;
	q->scrn = scrn;
	q->crtc = crtc;
	q->data = data;
	q->handler = handler;
	q->abort = abort;

	list_add(&q->list, &intel_drm_queue);

	return q->seq;
}

/*
 * Abort one queued DRM entry, removing it
 * from the list, calling the abort function and
 * freeing the memory
 */
static void
intel_drm_abort_one(struct intel_drm_queue *q)
{
	list_del(&q->list);
	q->abort(q->scrn, q->crtc, q->data);
	free(q);
}

/*
 * Externally usable abort function that uses a callback to match a single queued
 * entry to abort
 */
void
intel_drm_abort(ScrnInfoPtr scrn, Bool (*match)(void *data, void *match_data), void *match_data)
{
	struct intel_drm_queue *q;

	list_for_each_entry(q, &intel_drm_queue, list) {
		if (match(q->data, match_data)) {
			intel_drm_abort_one(q);
			break;
		}
	}
}

/*
 * Abort by drm queue sequence number
 */
static void
intel_drm_abort_seq(ScrnInfoPtr scrn, uint32_t seq)
{
	struct intel_drm_queue *q;

	list_for_each_entry(q, &intel_drm_queue, list) {
		if (q->seq == seq) {
			intel_drm_abort_one(q);
			break;
		}
	}
}

/*
 * Abort all queued entries on a specific scrn, used
 * when resetting the X server
 */
static void
intel_drm_abort_scrn(ScrnInfoPtr scrn)
{
	struct intel_drm_queue *q, *tmp;

	list_for_each_entry_safe(q, tmp, &intel_drm_queue, list) {
		if (q->scrn == scrn)
			intel_drm_abort_one(q);
	}
}

static uint32_t pipe_select(int pipe)
{
	if (pipe > 1)
		return pipe << DRM_VBLANK_HIGH_CRTC_SHIFT;
	else if (pipe > 0)
		return DRM_VBLANK_SECONDARY;
	else
		return 0;
}

/*
 * Get the current msc/ust value from the kernel
 */
static int
intel_get_msc_ust(ScrnInfoPtr scrn, xf86CrtcPtr crtc, uint32_t *msc, uint64_t *ust)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drmVBlank vbl;

	/* Get current count */
	vbl.request.type = DRM_VBLANK_RELATIVE | pipe_select(intel_crtc_to_pipe(crtc));
	vbl.request.sequence = 0;
	vbl.request.signal = 0;
	if (drmWaitVBlank(intel->drmSubFD, &vbl)) {
		*msc = 0;
		*ust = 0;
		return BadMatch;
	} else {
		*msc = vbl.reply.sequence;
		*ust = (CARD64) vbl.reply.tval_sec * 1000000 + vbl.reply.tval_usec;
		return Success;
	}
}

/*
 * Convert a 32-bit kernel MSC sequence number to a 64-bit local sequence
 * number, adding in the vblank_offset and high 32 bits, and dealing
 * with 64-bit wrapping
 */
uint64_t
intel_sequence_to_crtc_msc(xf86CrtcPtr crtc, uint32_t sequence)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;

        sequence += intel_crtc->vblank_offset;
        if ((int32_t) (sequence - intel_crtc->msc_prev) < -0x40000000)
                intel_crtc->msc_high += 0x100000000L;
        intel_crtc->msc_prev = sequence;
        return intel_crtc->msc_high + sequence;
}

/*
 * Get the current 64-bit adjust MSC and UST value
 */
int
intel_get_crtc_msc_ust(ScrnInfoPtr scrn, xf86CrtcPtr crtc, uint64_t *msc, uint64_t *ust)
{
        uint32_t sequence;
        int ret;

        ret = intel_get_msc_ust(scrn, crtc, &sequence, ust);
	if (ret)
		return ret;

        *msc = intel_sequence_to_crtc_msc(crtc, sequence);
        return 0;
}

/*
 * Convert a 64-bit adjusted MSC value into a 32-bit kernel sequence number,
 * removing the high 32 bits and subtracting out the vblank_offset term.
 *
 * This also updates the vblank_offset when it notices that the value should
 * change.
 */

#define MAX_VBLANK_OFFSET       1000

uint32_t
intel_crtc_msc_to_sequence(ScrnInfoPtr scrn, xf86CrtcPtr crtc, uint64_t expect)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;
        uint64_t msc, ust;

	if (intel_get_crtc_msc_ust(scrn, crtc, &msc, &ust) == 0) {
		int64_t diff = expect - msc;

		/* We're way off here, assume that the kernel has lost its mind
		 * and smack the vblank back to something sensible
		 */
		if (diff < -MAX_VBLANK_OFFSET || diff > MAX_VBLANK_OFFSET) {
			intel_crtc->vblank_offset += (int32_t) diff;
			if (intel_crtc->vblank_offset > -MAX_VBLANK_OFFSET &&
			    intel_crtc->vblank_offset < MAX_VBLANK_OFFSET)
				intel_crtc->vblank_offset = 0;
		}
	}

        return (uint32_t) (expect - intel_crtc->vblank_offset);
}

/*
 * General DRM kernel handler. Looks for the matching sequence number in the
 * drm event queue and calls the handler for it.
 */
static void
intel_drm_handler(int fd, uint32_t frame, uint32_t sec, uint32_t usec, void *user_ptr)
{
	uint32_t user_data = (intptr_t)user_ptr;
	struct intel_drm_queue *q;

	list_for_each_entry(q, &intel_drm_queue, list) {
		if (q->seq == user_data) {
			list_del(&q->list);
			q->handler(q->scrn, q->crtc,
				   intel_sequence_to_crtc_msc(q->crtc, frame),
				   (uint64_t)sec * 1000000 + usec, q->data);
			free(q);
			break;
		}
	}
}


/*
 * Notify the page flip caller that the flip is
 * complete
 */
static void
intel_pageflip_complete(struct intel_mode *mode)
{
	/* Release framebuffer */
	drmModeRmFB(mode->fd, mode->old_fb_id);

	if (!mode->pageflip_handler)
		return;

	mode->pageflip_handler(mode->fe_msc, mode->fe_usec,
			       mode->pageflip_data);
}

/*
 * One pageflip event has completed. Update the saved msc/ust values
 * as needed, then check to see if the whole set of events are
 * complete and notify the application at that point
 */
static struct intel_mode *
intel_handle_pageflip(struct intel_pageflip *flip, uint64_t msc, uint64_t usec)
{
	struct intel_mode *mode = flip->mode;

	if (flip->dispatch_me) {
		/* Yes: Cache msc, ust for later delivery. */
		mode->fe_msc = msc;
		mode->fe_usec = usec;
	}
	free(flip);

	/* Last crtc completed flip? */
	mode->flip_count--;
	if (mode->flip_count > 0)
		return NULL;

	return mode;
}

/*
 * Called from the DRM event queue when a single flip has completed
 */
static void
intel_pageflip_handler(ScrnInfoPtr scrn, xf86CrtcPtr crtc,
		       uint64_t msc, uint64_t usec, void *data)
{
	struct intel_pageflip   *flip = data;
	struct intel_mode       *mode = intel_handle_pageflip(flip, msc, usec);

	if (!mode)
		return;
	intel_pageflip_complete(mode);
}

/*
 * Called from the DRM queue abort code when a flip has been aborted
 */
static void
intel_pageflip_abort(ScrnInfoPtr scrn, xf86CrtcPtr crtc, void *data)
{
	struct intel_pageflip   *flip = data;
	struct intel_mode       *mode = intel_handle_pageflip(flip, 0, 0);

	if (!mode)
		return;

	/* Release framebuffer */
	drmModeRmFB(mode->fd, mode->old_fb_id);

	if (!mode->pageflip_abort)
		return;

	mode->pageflip_abort(mode->pageflip_data);
}

/*
 * Check for pending DRM events and process them.
 */
static void
drm_wakeup_handler(pointer data, int err, pointer p)
{
	struct intel_mode *mode;
	fd_set *read_mask;

	if (data == NULL || err < 0)
		return;

	mode = data;
	read_mask = p;
	if (FD_ISSET(mode->fd, read_mask))
		drmHandleEvent(mode->fd, &mode->event_context);
}

/*
 * If there are any available, read drm_events
 */
int
intel_mode_read_drm_events(struct intel_screen_private *intel)
{
	struct intel_mode *mode = intel->modes;
	struct pollfd p = { .fd = mode->fd, .events = POLLIN };
	int r;

	do {
		r = poll(&p, 1, 0);
	} while (r == -1 && (errno == EINTR || errno == EAGAIN));

	if (r <= 0)
		return 0;

	return drmHandleEvent(mode->fd, &mode->event_context);
}

/*
 * Libdrm's possible_clones is a mask of encoders, Xorg's possible_clones is a
 * mask of outputs. This function sets Xorg's possible_clones based on the
 * values read from libdrm.
 */
static uint32_t find_clones(ScrnInfoPtr scrn, xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private, *clone_drmout;
	int i;
	xf86OutputPtr clone_output;
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	int index_mask = 0;

	if (intel_output->enc_clone_mask == 0)
		return index_mask;

	for (i = 0; i < xf86_config->num_output; i++) {
		clone_output = xf86_config->output[i];
		clone_drmout = clone_output->driver_private;
		if (output == clone_output)
			continue;

		if (clone_drmout->enc_mask == 0)
			continue;
		if (intel_output->enc_clone_mask == clone_drmout->enc_mask)
			index_mask |= (1 << i);
	}
	return index_mask;
}
static void
intel_compute_possible_clones(ScrnInfoPtr scrn, struct intel_mode *mode, drmModeResPtr mode_res)
{
	int i, j;
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);

	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];
		struct intel_output *intel_output;

		intel_output = output->driver_private;
		intel_output->enc_clone_mask = 0xff;
		/* and all the possible encoder clones for this output together */
		for (j = 0; j < intel_output->mode_output->count_encoders; j++)
		{
			int k;
			for (k = 0; k < mode_res->count_encoders; k++) {
				if (mode_res->encoders[k] == intel_output->mode_encoders[j]->encoder_id)
					intel_output->enc_mask |= (1 << k);
			}

			intel_output->enc_clone_mask &= intel_output->mode_encoders[j]->possible_clones;
		}
	}

	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];
		output->possible_clones = find_clones(scrn, output);
	}
}

Bool intel_mode_pre_init(ScrnInfoPtr scrn, int fd, int cpp)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct drm_i915_getparam gp;
	struct intel_mode *mode;
	unsigned int i;
	int has_flipping;
	drmModeResPtr mode_res;

	mode = calloc(1, sizeof *mode);
	if (!mode)
		return FALSE;

	mode->fd = fd;

	list_init(&mode->crtcs);
	list_init(&mode->outputs);

	xf86CrtcConfigInit(scrn, &intel_xf86crtc_config_funcs);

	mode->cpp = cpp;
	mode_res = drmModeGetResources(mode->fd);
	if (!mode_res) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "failed to get resources: %s\n", strerror(errno));
		free(mode);
		return FALSE;
	}

	xf86CrtcSetSizeRange(scrn, 320, 200, mode_res->max_width,
			     mode_res->max_height);
	for (i = 0; i < mode_res->count_crtcs; i++)
		intel_crtc_init(scrn, mode, mode_res, i);

	for (i = 0; i < mode_res->count_connectors; i++)
		intel_output_init(scrn, mode, mode_res, i, 0);

	intel_compute_possible_clones(scrn, mode, mode_res);

#ifdef INTEL_PIXMAP_SHARING
	xf86ProviderSetup(scrn, NULL, "Intel");
#endif

	xf86InitialConfiguration(scrn, TRUE);

	mode->event_context.version = DRM_EVENT_CONTEXT_VERSION;
	mode->event_context.vblank_handler = intel_drm_handler;
	mode->event_context.page_flip_handler = intel_drm_handler;

	/* XXX assumes only one intel screen */
	list_init(&intel_drm_queue);
	intel_drm_seq = 0;

	has_flipping = 0;
	gp.param = I915_PARAM_HAS_PAGEFLIPPING;
	gp.value = &has_flipping;
	(void)drmCommandWriteRead(intel->drmSubFD, DRM_I915_GETPARAM, &gp,
				  sizeof(gp));
	if (has_flipping && intel->swapbuffers_wait) {
		xf86DrvMsg(scrn->scrnIndex, X_INFO,
			   "Kernel page flipping support detected, enabling\n");
		intel->use_pageflipping = TRUE;
	}

	if (xf86ReturnOptValBool(intel->Options, OPTION_DELETE_DP12, FALSE)) {
		mode->delete_dp_12_displays = TRUE;
	}

	intel->modes = mode;
	drmModeFreeResources(mode_res);
	return TRUE;
}

void
intel_mode_init(struct intel_screen_private *intel)
{
	struct intel_mode *mode = intel->modes;

	/* We need to re-register the mode->fd for the synchronisation
	 * feedback on every server generation, so perform the
	 * registration within ScreenInit and not PreInit.
	 */
	mode->flip_count = 0;
	AddGeneralSocket(mode->fd);
	RegisterBlockAndWakeupHandlers((BlockHandlerProcPtr)NoopDDA,
				       drm_wakeup_handler, mode);
}

void
intel_mode_remove_fb(intel_screen_private *intel)
{
	struct intel_mode *mode = intel->modes;

	if (mode->fb_id) {
		drmModeRmFB(mode->fd, mode->fb_id);
		mode->fb_id = 0;
	}
}

void
intel_mode_close(intel_screen_private *intel)
{
	struct intel_mode *mode = intel->modes;

	if (mode == NULL)
		return;

        intel_drm_abort_scrn(intel->scrn);

	RemoveBlockAndWakeupHandlers((BlockHandlerProcPtr)NoopDDA,
				     drm_wakeup_handler, mode);
	RemoveGeneralSocket(mode->fd);
}

void
intel_mode_fini(intel_screen_private *intel)
{
	struct intel_mode *mode = intel->modes;

	if (mode == NULL)
		return;

	while(!list_is_empty(&mode->crtcs)) {
		xf86CrtcDestroy(list_first_entry(&mode->crtcs,
						 struct intel_crtc,
						 link)->crtc);
	}

	while(!list_is_empty(&mode->outputs)) {
		xf86OutputDestroy(list_first_entry(&mode->outputs,
						   struct intel_output,
						   link)->output);
	}

	if (mode->fb_id)
		drmModeRmFB(mode->fd, mode->fb_id);

	/* mode->rotate_fb_id should have been destroyed already */

	free(mode);
	intel->modes = NULL;
}

/* for the mode overlay */
int
intel_crtc_id(xf86CrtcPtr crtc)
{
	return crtc_id(crtc->driver_private);
}

int intel_crtc_to_pipe(xf86CrtcPtr crtc)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;
	return intel_crtc->pipe;
}

Bool intel_crtc_on(xf86CrtcPtr crtc)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	drmModeCrtcPtr drm_crtc;
	Bool ret;
	int i;

	if (!crtc->enabled)
		return FALSE;

	/* Kernel manages CRTC status based on output config */
	ret = FALSE;
	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];
		if (output->crtc == crtc &&
		    intel_output_dpms_status(output) == DPMSModeOn) {
			ret = TRUE;
			break;
		}
	}
	if (!ret)
		return FALSE;

	/* And finally check with the kernel that the fb is bound */
	drm_crtc = drmModeGetCrtc(intel_crtc->mode->fd, crtc_id(intel_crtc));
	if (drm_crtc == NULL)
		return FALSE;

	ret = (drm_crtc->mode_valid &&
	       (intel_crtc->mode->fb_id == drm_crtc->buffer_id ||
		intel_crtc->mode->old_fb_id == drm_crtc->buffer_id));
	free(drm_crtc);

	return ret;
}

static PixmapPtr
intel_create_pixmap_for_bo(ScreenPtr pScreen, dri_bo *bo,
			   int width, int height,
			   int depth, int bpp,
			   int pitch)
{
	PixmapPtr pixmap;

	pixmap = pScreen->CreatePixmap(pScreen, 0, 0, depth, 0);
	if (pixmap == NullPixmap)
		return pixmap;

	if (!pScreen->ModifyPixmapHeader(pixmap,
					 width, height,
					 depth, bpp,
					 pitch, NULL)) {
		pScreen->DestroyPixmap(pixmap);
		return NullPixmap;
	}

	intel_set_pixmap_bo(pixmap, bo);
	return pixmap;
}

static PixmapPtr
intel_create_pixmap_for_fbcon(ScrnInfoPtr scrn, int fbcon_id)
{
	ScreenPtr pScreen = xf86ScrnToScreen(scrn);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_mode *mode = intel->modes;
	int fd = mode->fd;
	drmModeFBPtr fbcon;
	struct drm_gem_flink flink;
	drm_intel_bo *bo;
	PixmapPtr pixmap = NullPixmap;

	fbcon = drmModeGetFB(fd, fbcon_id);
	if (fbcon == NULL)
		return NULL;

	if (fbcon->depth != scrn->depth ||
	    fbcon->width != scrn->virtualX ||
	    fbcon->height != scrn->virtualY)
		goto out_free_fb;

	flink.handle = fbcon->handle;
	if (ioctl(fd, DRM_IOCTL_GEM_FLINK, &flink) < 0) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Couldn't flink fbcon handle\n");
		goto out_free_fb;
	}

	bo = drm_intel_bo_gem_create_from_name(intel->bufmgr,
					       "fbcon", flink.name);
	if (bo == NULL) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Couldn't allocate bo for fbcon handle\n");
		goto out_free_fb;
	}

	pixmap = intel_create_pixmap_for_bo(pScreen, bo,
					    fbcon->width, fbcon->height,
					    fbcon->depth, fbcon->bpp,
					    fbcon->pitch);
	if (pixmap == NullPixmap)
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "Couldn't allocate pixmap fbcon contents\n");
	drm_intel_bo_unreference(bo);
out_free_fb:
	drmModeFreeFB(fbcon);

	return pixmap;
}

void intel_copy_fb(ScrnInfoPtr scrn)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	ScreenPtr pScreen = xf86ScrnToScreen(scrn);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	PixmapPtr src, dst;
	unsigned int pitch = scrn->displayWidth * intel->cpp;
	struct intel_crtc *intel_crtc;
	int i, fbcon_id;

	if (intel->force_fallback)
		return;

	fbcon_id = 0;
	for (i = 0; i < xf86_config->num_crtc; i++) {
		intel_crtc = xf86_config->crtc[i]->driver_private;
		if (intel_crtc->mode_crtc->buffer_id)
			fbcon_id = intel_crtc->mode_crtc->buffer_id;
	}
	if (!fbcon_id)
		return;

	src = intel_create_pixmap_for_fbcon(scrn, fbcon_id);
	if (src == NULL)
		return;

	/* We dont have a screen Pixmap yet */
	dst = intel_create_pixmap_for_bo(pScreen, intel->front_buffer,
					 scrn->virtualX, scrn->virtualY,
					 scrn->depth, scrn->bitsPerPixel,
					 pitch);
	if (dst == NullPixmap)
		goto cleanup_src;

	if (!intel->uxa_driver->prepare_copy(src, dst,
					     -1, -1,
					     GXcopy, FB_ALLONES))
		goto cleanup_dst;

	intel->uxa_driver->copy(dst,
				0, 0,
				0, 0,
				scrn->virtualX, scrn->virtualY);
	intel->uxa_driver->done_copy(dst);
#if ABI_VIDEODRV_VERSION >= SET_ABI_VERSION(10, 0)
	pScreen->canDoBGNoneRoot = TRUE;
#endif

cleanup_dst:
	(*pScreen->DestroyPixmap)(dst);
cleanup_src:
	(*pScreen->DestroyPixmap)(src);
}

void
intel_mode_hotplug(struct intel_screen_private *intel)
{
	ScrnInfoPtr scrn = intel->scrn;
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	drmModeResPtr mode_res;
	int i, j;
	Bool found;
	Bool changed = FALSE;
	struct intel_mode *mode = intel->modes;
	mode_res = drmModeGetResources(intel->drmSubFD);
	if (!mode_res)
		goto out;

restart_destroy:
	for (i = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];
		struct intel_output *intel_output;

		intel_output = output->driver_private;
		found = FALSE;
		for (j = 0; j < mode_res->count_connectors; j++) {
			if (mode_res->connectors[j] == intel_output->output_id) {
				found = TRUE;
				break;
			}
		}
		if (found)
			continue;

		drmModeFreeConnector(intel_output->mode_output);
		intel_output->mode_output = NULL;
		intel_output->output_id = -1;

		changed = TRUE;
		if (mode->delete_dp_12_displays) {
			RROutputDestroy(output->randr_output);
			xf86OutputDestroy(output);
			goto restart_destroy;
		}
	}

	/* find new output ids we don't have outputs for */
	for (i = 0; i < mode_res->count_connectors; i++) {
		found = FALSE;

		for (j = 0; j < config->num_output; j++) {
			xf86OutputPtr output = config->output[j];
			struct intel_output *intel_output;

			intel_output = output->driver_private;
			if (mode_res->connectors[i] == intel_output->output_id) {
				found = TRUE;
				break;
			}
		}
		if (found)
			continue;

		changed = TRUE;
		intel_output_init(scrn, intel->modes, mode_res, i, 1);
	}

	if (changed) {
		RRSetChanged(xf86ScrnToScreen(scrn));
		RRTellChanged(xf86ScrnToScreen(scrn));
	}

	drmModeFreeResources(mode_res);
out:
	RRGetInfo(xf86ScrnToScreen(scrn), TRUE);
}
