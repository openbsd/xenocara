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
#include <poll.h>
#include <sys/ioctl.h>

#include "xorgVersion.h"

#include "intel.h"
#include "intel_bufmgr.h"
#include "intel_options.h"
#include "xf86drm.h"
#include "xf86drmMode.h"
#include "X11/Xatom.h"
#include "X11/extensions/dpmsconst.h"
#include "xf86DDC.h"
#include "fb.h"
#include "uxa.h"

#include "intel_glamor.h"

#define KNOWN_MODE_FLAGS ((1<<14)-1)

struct intel_mode {
	int fd;
	uint32_t fb_id;
	drmModeResPtr mode_res;
	int cpp;

	drmEventContext event_context;
	DRI2FrameEventPtr flip_info;
	int old_fb_id;
	int flip_count;
	unsigned int fe_frame;
	unsigned int fe_tv_sec;
	unsigned int fe_tv_usec;

	struct list outputs;
	struct list crtcs;
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
	drmModeEncoderPtr mode_encoder;
	int num_props;
	struct intel_property *props;
	void *private_data;

	Bool has_panel_limits;
	int panel_hdisplay;
	int panel_vdisplay;

	int dpms_mode;
	const char *backlight_iface;
	int backlight_active_level;
	int backlight_max;
	xf86OutputPtr output;
	struct list link;
};

static void
intel_output_dpms(xf86OutputPtr output, int mode);

static void
intel_output_dpms_backlight(xf86OutputPtr output, int oldmode, int mode);

#define BACKLIGHT_CLASS "/sys/class/backlight"

/*
 * List of available kernel interfaces in priority order
 */
static const char *backlight_interfaces[] = {
	"gmux_backlight",
	"asus-laptop",
	"asus-nb-wmi",
	"eeepc",
	"thinkpad_screen",
	"mbp_backlight",
	"fujitsu-laptop",
	"sony",
	"samsung",
	"acpi_video1", /* finally fallback to the generic acpi drivers */
	"acpi_video0",
	"intel_backlight",
	NULL,
};
/*
 * Must be long enough for BACKLIGHT_CLASS + '/' + longest in above table +
 * '/' + "max_backlight"
 */
#define BACKLIGHT_PATH_LEN 80
/* Enough for 10 digits of backlight + '\n' + '\0' */
#define BACKLIGHT_VALUE_LEN 12

static inline int
crtc_id(struct intel_crtc *crtc)
{
	return crtc->mode_crtc->crtc_id;
}

static void
intel_output_backlight_set(xf86OutputPtr output, int level)
{
	struct intel_output *intel_output = output->driver_private;
	char path[BACKLIGHT_PATH_LEN], val[BACKLIGHT_VALUE_LEN];
	int fd, len, ret;

	if (level > intel_output->backlight_max)
		level = intel_output->backlight_max;
	if (! intel_output->backlight_iface || level < 0)
		return;

	len = snprintf(val, BACKLIGHT_VALUE_LEN, "%d\n", level);
	sprintf(path, "%s/%s/brightness",
		BACKLIGHT_CLASS, intel_output->backlight_iface);
	fd = open(path, O_RDWR);
	if (fd == -1) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR, "failed to open %s for backlight "
			   "control: %s\n", path, strerror(errno));
		return;
	}

	ret = write(fd, val, len);
	if (ret == -1) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR, "write to %s for backlight "
			   "control failed: %s\n", path, strerror(errno));
	}

	close(fd);
}

static int
intel_output_backlight_get(xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private;
	char path[BACKLIGHT_PATH_LEN], val[BACKLIGHT_VALUE_LEN];
	int fd, level;

	sprintf(path, "%s/%s/actual_brightness",
		BACKLIGHT_CLASS, intel_output->backlight_iface);
	fd = open(path, O_RDONLY);
	if (fd == -1) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR, "failed to open %s "
			   "for backlight control: %s\n", path, strerror(errno));
		return -1;
	}

	memset(val, 0, sizeof(val));
	if (read(fd, val, BACKLIGHT_VALUE_LEN) == -1) {
		close(fd);
		return -1;
	}

	close(fd);

	level = atoi(val);
	if (level > intel_output->backlight_max)
		level = intel_output->backlight_max;
	if (level < 0)
		level = -1;
	return level;
}

static int
intel_output_backlight_get_max(xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private;
	char path[BACKLIGHT_PATH_LEN], val[BACKLIGHT_VALUE_LEN];
	int fd, max = 0;

	sprintf(path, "%s/%s/max_brightness",
		BACKLIGHT_CLASS, intel_output->backlight_iface);
	fd = open(path, O_RDONLY);
	if (fd == -1) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR, "failed to open %s "
			   "for backlight control: %s\n", path, strerror(errno));
		return -1;
	}

	memset(val, 0, sizeof(val));
	if (read(fd, val, BACKLIGHT_VALUE_LEN) == -1) {
		close(fd);
		return -1;
	}

	close(fd);

	max = atoi(val);
	if (max <= 0)
		max = -1;
	return max;
}

static void
intel_output_backlight_init(xf86OutputPtr output)
{
	struct intel_output *intel_output = output->driver_private;
	intel_screen_private *intel = intel_get_screen_private(output->scrn);
	char path[BACKLIGHT_PATH_LEN];
	struct stat buf;
	char *str;
	int i;

	str = xf86GetOptValString(intel->Options, OPTION_BACKLIGHT);
	if (str != NULL) {
		sprintf(path, "%s/%s", BACKLIGHT_CLASS, str);
		if (!stat(path, &buf)) {
			intel_output->backlight_iface = str;
			intel_output->backlight_max = intel_output_backlight_get_max(output);
			if (intel_output->backlight_max > 0) {
				intel_output->backlight_active_level = intel_output_backlight_get(output);
				xf86DrvMsg(output->scrn->scrnIndex, X_CONFIG,
					   "found backlight control interface %s\n", path);
				return;
			}
		}
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			   "unrecognised backlight control interface %s\n", str);
	}

	for (i = 0; backlight_interfaces[i] != NULL; i++) {
		sprintf(path, "%s/%s", BACKLIGHT_CLASS, backlight_interfaces[i]);
		if (!stat(path, &buf)) {
			intel_output->backlight_iface = backlight_interfaces[i];
			intel_output->backlight_max = intel_output_backlight_get_max(output);
			if (intel_output->backlight_max > 0) {
				intel_output->backlight_active_level = intel_output_backlight_get(output);
				xf86DrvMsg(output->scrn->scrnIndex, X_PROBED,
					   "found backlight control interface %s\n", path);
				return;
			}
		}
	}
	intel_output->backlight_iface = NULL;
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

		if (output->crtc != crtc)
			continue;

		intel_output = output->driver_private;
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

static void
intel_crtc_load_cursor_argb(xf86CrtcPtr crtc, CARD32 *image)
{
	struct intel_crtc *intel_crtc = crtc->driver_private;
	int ret;

	ret = dri_bo_subdata(intel_crtc->cursor, 0, 64*64*4, image);
	if (ret)
		xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
			   "failed to set cursor: %s\n", strerror(-ret));
}

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
	unsigned long rotate_pitch;
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
	.load_cursor_argb = intel_crtc_load_cursor_argb,
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
intel_crtc_init(ScrnInfoPtr scrn, struct intel_mode *mode, int num)
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
					       mode->mode_res->crtcs[num]);
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
	drmModePropertyBlobPtr edid_blob = NULL;
	xf86MonPtr mon = NULL;
	int i;

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
			drmModeFreePropertyBlob(edid_blob);
			edid_blob =
				drmModeGetPropertyBlob(mode->fd,
						       koutput->prop_values[i]);
		}
		drmModeFreeProperty(props);
	}

	if (edid_blob) {
		mon = xf86InterpretEDID(output->scrn->scrnIndex,
					edid_blob->data);

		if (mon && edid_blob->length > 128)
			mon->flags |= MONITOR_EDID_COMPLETE_RAWDATA;
	}

	xf86OutputSetEDID(output, mon);

	if (edid_blob)
		drmModeFreePropertyBlob(edid_blob);
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

	for (i = 0; i < intel_output->num_props; i++) {
		drmModeFreeProperty(intel_output->props[i].mode_prop);
		free(intel_output->props[i].atoms);
	}
	free(intel_output->props);

	drmModeFreeConnector(intel_output->mode_output);
	intel_output->mode_output = NULL;

	list_del(&intel_output->link);
	free(intel_output);

	output->driver_private = NULL;
}

static void
intel_output_dpms_backlight(xf86OutputPtr output, int oldmode, int mode)
{
	struct intel_output *intel_output = output->driver_private;

	if (!intel_output->backlight_iface)
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

	for (i = 0; i < koutput->count_props; i++) {
		drmModePropertyPtr props;

		props = drmModeGetProperty(mode->fd, koutput->props[i]);
		if (!props)
			continue;

		if (!strcmp(props->name, "DPMS")) {
			/* Make sure to reverse the order between on and off. */
			if (dpms == DPMSModeOff)
				intel_output_dpms_backlight(output,
							    intel_output->dpms_mode,
							    dpms);
			drmModeConnectorSetProperty(mode->fd,
						    intel_output->output_id,
						    props->prop_id,
						    dpms);
			if (dpms != DPMSModeOff)
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
				     TRUE);
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
						     XA_ATOM, 32, PropModeReplace, 1, &p->atoms[j+1], FALSE, TRUE);
			if (err != 0) {
				xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
					   "RRChangeOutputProperty error, %d\n", err);
			}
		}
	}

	if (intel_output->backlight_iface) {
		/* Set up the backlight property, which takes effect
		 * immediately and accepts values only within the
		 * backlight_range.
		 */
		intel_output_create_ranged_atom(output, &backlight_atom,
					BACKLIGHT_NAME, 0,
					intel_output->backlight_max,
					intel_output->backlight_active_level,
					FALSE);
		intel_output_create_ranged_atom(output,
					&backlight_deprecated_atom,
					BACKLIGHT_DEPRECATED_NAME, 0,
					intel_output->backlight_max,
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
		if (val < 0 || val > intel_output->backlight_max)
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

		if (! intel_output->backlight_iface)
			return FALSE;

		val = intel_output_backlight_get(output);
		if (val < 0)
			return FALSE;

		err = RRChangeOutputProperty(output->randr_output, property,
					     XA_INTEGER, 32, PropModeReplace, 1, &val,
					     FALSE, TRUE);
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

static void
intel_output_init(ScrnInfoPtr scrn, struct intel_mode *mode, int num)
{
	xf86OutputPtr output;
	drmModeConnectorPtr koutput;
	drmModeEncoderPtr kencoder;
	struct intel_output *intel_output;
	const char *output_name;
	char name[32];

	koutput = drmModeGetConnector(mode->fd,
				      mode->mode_res->connectors[num]);
	if (!koutput)
		return;

	kencoder = drmModeGetEncoder(mode->fd, koutput->encoders[0]);
	if (!kencoder) {
		drmModeFreeConnector(koutput);
		return;
	}

	if (koutput->connector_type < ARRAY_SIZE(output_names))
		output_name = output_names[koutput->connector_type];
	else
		output_name = "UNKNOWN";
	snprintf(name, 32, "%s%d", output_name, koutput->connector_type_id);

	output = xf86OutputCreate (scrn, &intel_output_funcs, name);
	if (!output) {
		drmModeFreeEncoder(kencoder);
		drmModeFreeConnector(koutput);
		return;
	}

	intel_output = calloc(sizeof(struct intel_output), 1);
	if (!intel_output) {
		xf86OutputDestroy(output);
		drmModeFreeConnector(koutput);
		drmModeFreeEncoder(kencoder);
		return;
	}

	intel_output->output_id = mode->mode_res->connectors[num];
	intel_output->mode_output = koutput;
	intel_output->mode_encoder = kencoder;
	intel_output->mode = mode;

	output->mm_width = koutput->mmWidth;
	output->mm_height = koutput->mmHeight;

	output->subpixel_order = subpixel_conv_table[koutput->subpixel];
	output->driver_private = intel_output;

	if (is_panel(koutput->connector_type))
		intel_output_backlight_init(output);

	output->possible_crtcs = kencoder->possible_crtcs;
	output->interlaceAllowed = TRUE;

	intel_output->output = output;
	list_add(&intel_output->link, &mode->outputs);
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
	unsigned long pitch;
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
							 &pitch,
							 &tiling);
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

	for (i = 0; i < xf86_config->num_crtc; i++) {
		xf86CrtcPtr crtc = xf86_config->crtc[i];

		if (!crtc->enabled)
			continue;

		if (!intel_crtc_apply(crtc))
			goto fail;
	}

	intel_uxa_create_screen_resources(scrn->pScreen);

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

Bool
intel_do_pageflip(intel_screen_private *intel,
		  dri_bo *new_front,
		  DRI2FrameEventPtr flip_info, int ref_crtc_hw_id)
{
	ScrnInfoPtr scrn = intel->scrn;
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	struct intel_crtc *crtc = config->crtc[0]->driver_private;
	struct intel_mode *mode = crtc->mode;
	unsigned int pitch = scrn->displayWidth * intel->cpp;
	struct intel_pageflip *flip;
	uint32_t new_fb_id;
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

	/*
	 * Queue flips on all enabled CRTCs
	 * Note that if/when we get per-CRTC buffers, we'll have to update this.
	 * Right now it assumes a single shared fb across all CRTCs, with the
	 * kernel fixing up the offset of each CRTC as necessary.
	 *
	 * Also, flips queued on disabled or incorrectly configured displays
	 * may never complete; this is a configuration error.
	 */
	mode->fe_frame = 0;
	mode->fe_tv_sec = 0;
	mode->fe_tv_usec = 0;

	for (i = 0; i < config->num_crtc; i++) {
		if (!intel_crtc_on(config->crtc[i]))
			continue;

		mode->flip_info = flip_info;
		mode->flip_count++;

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

		if (drmModePageFlip(mode->fd,
				    crtc_id(crtc),
				    new_fb_id,
				    DRM_MODE_PAGE_FLIP_EVENT, flip)) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "flip queue failed: %s\n", strerror(errno));
			free(flip);
			goto error_undo;
		}
	}

	mode->old_fb_id = mode->fb_id;
	mode->fb_id = new_fb_id;
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
	return FALSE;
}

static const xf86CrtcConfigFuncsRec intel_xf86crtc_config_funcs = {
	intel_xf86crtc_resize
};

static void
intel_vblank_handler(int fd, unsigned int frame, unsigned int tv_sec,
		       unsigned int tv_usec, void *event)
{
	I830DRI2FrameEventHandler(frame, tv_sec, tv_usec, event);
}

static void
intel_page_flip_handler(int fd, unsigned int frame, unsigned int tv_sec,
			  unsigned int tv_usec, void *event_data)
{
	struct intel_pageflip *flip = event_data;
	struct intel_mode *mode = flip->mode;

	/* Is this the event whose info shall be delivered to higher level? */
	if (flip->dispatch_me) {
		/* Yes: Cache msc, ust for later delivery. */
		mode->fe_frame = frame;
		mode->fe_tv_sec = tv_sec;
		mode->fe_tv_usec = tv_usec;
	}
	free(flip);

	/* Last crtc completed flip? */
	mode->flip_count--;
	if (mode->flip_count > 0)
		return;

	/* Release framebuffer */
	drmModeRmFB(mode->fd, mode->old_fb_id);

	if (mode->flip_info == NULL)
		return;

	/* Deliver cached msc, ust from reference crtc to flip event handler */
	I830DRI2FlipEventHandler(mode->fe_frame, mode->fe_tv_sec,
				 mode->fe_tv_usec, mode->flip_info);
}

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

static drmModeEncoderPtr
intel_get_kencoder(struct intel_mode *mode, int num)
{
	struct intel_output *iterator;
	int id = mode->mode_res->encoders[num];

	list_for_each_entry(iterator, &mode->outputs, link)
		if (iterator->mode_encoder->encoder_id == id)
			return iterator->mode_encoder;

	return NULL;
}

/*
 * Libdrm's possible_clones is a mask of encoders, Xorg's possible_clones is a
 * mask of outputs. This function sets Xorg's possible_clones based on the
 * values read from libdrm.
 */
static void
intel_compute_possible_clones(ScrnInfoPtr scrn, struct intel_mode *mode)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	struct intel_output *intel_output, *clone;
	drmModeEncoderPtr cloned_encoder;
	uint32_t mask;
	int i, j, k;
	CARD32 possible_clones;

	for (i = 0; i < config->num_output; i++) {
		possible_clones = 0;
		intel_output = config->output[i]->driver_private;

		mask = intel_output->mode_encoder->possible_clones;
		for (j = 0; mask != 0; j++, mask >>= 1) {

			if ((mask & 1) == 0)
				continue;

			cloned_encoder = intel_get_kencoder(mode, j);
			if (!cloned_encoder)
				continue;

			for (k = 0; k < config->num_output; k++) {
				clone = config->output[k]->driver_private;
				if (clone->mode_encoder->encoder_id ==
				    cloned_encoder->encoder_id)
					possible_clones |= (1 << k);
			}
		}

		config->output[i]->possible_clones = possible_clones;
	}
}

Bool intel_mode_pre_init(ScrnInfoPtr scrn, int fd, int cpp)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct drm_i915_getparam gp;
	struct intel_mode *mode;
	unsigned int i;
	int has_flipping;

	mode = calloc(1, sizeof *mode);
	if (!mode)
		return FALSE;

	mode->fd = fd;

	list_init(&mode->crtcs);
	list_init(&mode->outputs);

	xf86CrtcConfigInit(scrn, &intel_xf86crtc_config_funcs);

	mode->cpp = cpp;
	mode->mode_res = drmModeGetResources(mode->fd);
	if (!mode->mode_res) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "failed to get resources: %s\n", strerror(errno));
		free(mode);
		return FALSE;
	}

	xf86CrtcSetSizeRange(scrn, 320, 200, mode->mode_res->max_width,
			     mode->mode_res->max_height);
	for (i = 0; i < mode->mode_res->count_crtcs; i++)
		intel_crtc_init(scrn, mode, i);

	for (i = 0; i < mode->mode_res->count_connectors; i++)
		intel_output_init(scrn, mode, i);

	intel_compute_possible_clones(scrn, mode);

#ifdef INTEL_PIXMAP_SHARING
	xf86ProviderSetup(scrn, NULL, "Intel");
#endif

	xf86InitialConfiguration(scrn, TRUE);

	mode->event_context.version = DRM_EVENT_CONTEXT_VERSION;
	mode->event_context.vblank_handler = intel_vblank_handler;
	mode->event_context.page_flip_handler = intel_page_flip_handler;

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

	intel->modes = mode;
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

static Bool has_pending_events(int fd)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	return poll(&pfd, 1, 0) == 1;
}

void
intel_mode_close(intel_screen_private *intel)
{
	struct intel_mode *mode = intel->modes;

	if (mode == NULL)
		return;

	while (has_pending_events(mode->fd))
		drmHandleEvent(mode->fd, &mode->event_context);
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
	       intel_crtc->mode->fb_id == drm_crtc->buffer_id);
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
	pScreen->canDoBGNoneRoot = TRUE;

cleanup_dst:
	(*pScreen->DestroyPixmap)(dst);
cleanup_src:
	(*pScreen->DestroyPixmap)(src);
}
