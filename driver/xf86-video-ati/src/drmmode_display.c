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

#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include "cursorstr.h"
#include "damagestr.h"
#include "inputstr.h"
#include "list.h"
#include "micmap.h"
#include "mipointrst.h"
#include "xf86cmap.h"
#include "xf86Priv.h"
#include "radeon.h"
#include "radeon_bo_helper.h"
#include "radeon_glamor.h"
#include "radeon_reg.h"

#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,19,99,1,0)
#include <dri.h>
#endif

#include "drmmode_display.h"

/* DPMS */
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#define DEFAULT_NOMINAL_FRAME_RATE 60

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 22
#define HAVE_NOTIFY_FD	1
#endif

static Bool
drmmode_xf86crtc_resize (ScrnInfoPtr scrn, int width, int height);

static Bool
RADEONZaphodStringMatches(ScrnInfoPtr pScrn, const char *s, char *output_name)
{
    int i = 0;
    char s1[20];

    do {
	switch(*s) {
	case ',':
  	    s1[i] = '\0';
	    i = 0;
	    if (strcmp(s1, output_name) == 0)
		return TRUE;
	    break;
	case ' ':
	case '\t':
	case '\n':
	case '\r':
	    break;
	default:
	    s1[i] = *s;
	    i++;
	    break;
	}
    } while(*s++);

    s1[i] = '\0';
    if (strcmp(s1, output_name) == 0)
	return TRUE;

    return FALSE;
}


static PixmapPtr drmmode_create_bo_pixmap(ScrnInfoPtr pScrn,
					  int width, int height,
					  int depth, int bpp,
					  int pitch,
					  struct radeon_buffer *bo)
{
	RADEONInfoPtr info = RADEONPTR(pScrn);
	ScreenPtr pScreen = pScrn->pScreen;
	PixmapPtr pixmap;

	pixmap = (*pScreen->CreatePixmap)(pScreen, 0, 0, depth,
					  RADEON_CREATE_PIXMAP_SCANOUT);
	if (!pixmap)
		return NULL;

	if (!(*pScreen->ModifyPixmapHeader)(pixmap, width, height,
					    depth, bpp, pitch, NULL)) {
		goto fail;
	}

	if (!info->use_glamor)
		exaMoveInPixmap(pixmap);

	if (!radeon_set_pixmap_bo(pixmap, bo))
		goto fail;

	if (info->surf_man && !info->use_glamor) {
		struct radeon_surface *surface = radeon_get_pixmap_surface(pixmap);

		if (!radeon_surface_initialize(info, surface, width, height, bpp / 8,
					       radeon_get_pixmap_tiling_flags(pixmap), 0))
			goto fail;
	}

	if (!info->use_glamor ||
	    radeon_glamor_create_textured_pixmap(pixmap, bo))
		return pixmap;

fail:
	pScreen->DestroyPixmap(pixmap);
	return NULL;
}

static void drmmode_destroy_bo_pixmap(PixmapPtr pixmap)
{
	ScreenPtr pScreen = pixmap->drawable.pScreen;

	(*pScreen->DestroyPixmap)(pixmap);
}

static void
drmmode_ConvertFromKMode(ScrnInfoPtr	scrn,
		     drmModeModeInfo *kmode,
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

	mode->Flags = kmode->flags; //& FLAG_BITS;
	mode->name = strdup(kmode->name);

	if (kmode->type & DRM_MODE_TYPE_DRIVER)
		mode->type = M_T_DRIVER;
	if (kmode->type & DRM_MODE_TYPE_PREFERRED)
		mode->type |= M_T_PREFERRED;
	xf86SetModeCrtc (mode, scrn->adjustFlags);
}

static void
drmmode_ConvertToKMode(ScrnInfoPtr	scrn,
		     drmModeModeInfo *kmode,
		     DisplayModePtr	mode)
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

	kmode->flags = mode->Flags; //& FLAG_BITS;
	if (mode->name)
		strncpy(kmode->name, mode->name, DRM_DISPLAY_MODE_LEN);
	kmode->name[DRM_DISPLAY_MODE_LEN-1] = 0;

}

/*
 * Utility helper for drmWaitVBlank
 */
Bool
drmmode_wait_vblank(xf86CrtcPtr crtc, drmVBlankSeqType type,
		    uint32_t target_seq, unsigned long signal, uint64_t *ust,
		    uint32_t *result_seq)
{
	int crtc_id = drmmode_get_crtc_id(crtc);
	ScrnInfoPtr scrn = crtc->scrn;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
	drmVBlank vbl;

	if (crtc_id == 1)
		type |= DRM_VBLANK_SECONDARY;
	else if (crtc_id > 1)
		type |= (crtc_id << DRM_VBLANK_HIGH_CRTC_SHIFT) &
			DRM_VBLANK_HIGH_CRTC_MASK;

	vbl.request.type = type;
	vbl.request.sequence = target_seq;
	vbl.request.signal = signal;

	if (drmWaitVBlank(pRADEONEnt->fd, &vbl) != 0)
		return FALSE;

	if (ust)
		*ust = (uint64_t)vbl.reply.tval_sec * 1000000 +
			vbl.reply.tval_usec;
	if (result_seq)
		*result_seq = vbl.reply.sequence;

	return TRUE;
}

/*
 * Retrieves present time in microseconds that is compatible
 * with units used by vblank timestamps. Depending on the kernel
 * version and DRM kernel module configuration, the vblank
 * timestamp can either be in real time or monotonic time
 */
int drmmode_get_current_ust(int drm_fd, CARD64 *ust)
{
	uint64_t cap_value;
	int ret;
	struct timespec now;

	ret = drmGetCap(drm_fd, DRM_CAP_TIMESTAMP_MONOTONIC, &cap_value);
	if (ret || !cap_value)
		/* old kernel or drm_timestamp_monotonic turned off */
		ret = clock_gettime(CLOCK_REALTIME, &now);
	else
		ret = clock_gettime(CLOCK_MONOTONIC, &now);
	if (ret)
		return ret;
	*ust = ((CARD64)now.tv_sec * 1000000) + ((CARD64)now.tv_nsec / 1000);
	return 0;
}

/*
 * Get current frame count and frame count timestamp of the crtc.
 */
int drmmode_crtc_get_ust_msc(xf86CrtcPtr crtc, CARD64 *ust, CARD64 *msc)
{
    ScrnInfoPtr scrn = crtc->scrn;
    uint32_t seq;

    if (!drmmode_wait_vblank(crtc, DRM_VBLANK_RELATIVE, 0, 0, ust, &seq)) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		   "get vblank counter failed: %s\n", strerror(errno));
	return -1;
    }

    *msc = seq;

    return Success;
}

static void
drmmode_do_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	ScrnInfoPtr scrn = crtc->scrn;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
	CARD64 ust;
	int ret;

	if (drmmode_crtc->dpms_mode == DPMSModeOn && mode != DPMSModeOn) {
		uint32_t seq;

		radeon_drm_wait_pending_flip(crtc);

		/*
		 * On->Off transition: record the last vblank time,
		 * sequence number and frame period.
		 */
		if (!drmmode_wait_vblank(crtc, DRM_VBLANK_RELATIVE, 0, 0, &ust,
					 &seq))
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
				   "%s cannot get last vblank counter\n",
				   __func__);
		else {
			CARD64 nominal_frame_rate, pix_in_frame;

			drmmode_crtc->dpms_last_ust = ust;
			drmmode_crtc->dpms_last_seq = seq;
			nominal_frame_rate = crtc->mode.Clock;
			nominal_frame_rate *= 1000;
			pix_in_frame = crtc->mode.HTotal * crtc->mode.VTotal;
			if (nominal_frame_rate == 0 || pix_in_frame == 0)
				nominal_frame_rate = DEFAULT_NOMINAL_FRAME_RATE;
			else
				nominal_frame_rate /= pix_in_frame;
			drmmode_crtc->dpms_last_fps = nominal_frame_rate;
		}

		drmmode_crtc->dpms_mode = mode;
		radeon_drm_queue_handle_deferred(crtc);
	} else if (drmmode_crtc->dpms_mode != DPMSModeOn && mode == DPMSModeOn) {
		/*
		 * Off->On transition: calculate and accumulate the
		 * number of interpolated vblanks while we were in Off state
		 */
		ret = drmmode_get_current_ust(pRADEONEnt->fd, &ust);
		if (ret)
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
				   "%s cannot get current time\n", __func__);
		else if (drmmode_crtc->dpms_last_ust) {
			CARD64 time_elapsed, delta_seq;
			time_elapsed = ust - drmmode_crtc->dpms_last_ust;
			delta_seq = time_elapsed * drmmode_crtc->dpms_last_fps;
			delta_seq /= 1000000;
			drmmode_crtc->interpolated_vblanks += delta_seq;

		}

		drmmode_crtc->dpms_mode = DPMSModeOn;
	}
}

static void
drmmode_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(crtc->scrn);

	/* Disable unused CRTCs */
	if (!crtc->enabled || mode != DPMSModeOn) {
		drmmode_do_crtc_dpms(crtc, DPMSModeOff);
		drmModeSetCrtc(pRADEONEnt->fd, drmmode_crtc->mode_crtc->crtc_id,
			       0, 0, 0, NULL, 0, NULL);
		drmmode_fb_reference(pRADEONEnt->fd, &drmmode_crtc->fb, NULL);
	} else if (drmmode_crtc->dpms_mode != DPMSModeOn)
		crtc->funcs->set_mode_major(crtc, &crtc->mode, crtc->rotation,
					    crtc->x, crtc->y);
}

static PixmapPtr
create_pixmap_for_fbcon(drmmode_ptr drmmode,
			ScrnInfoPtr pScrn, int fbcon_id)
{
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	RADEONInfoPtr info = RADEONPTR(pScrn);
	PixmapPtr pixmap = info->fbcon_pixmap;
	struct radeon_buffer *bo;
	drmModeFBPtr fbcon;
	struct drm_gem_flink flink;

	if (pixmap)
	    return pixmap;

	fbcon = drmModeGetFB(pRADEONEnt->fd, fbcon_id);
	if (!fbcon)
		return NULL;

	if (fbcon->depth != pScrn->depth ||
	    fbcon->width != pScrn->virtualX ||
	    fbcon->height != pScrn->virtualY)
		goto out_free_fb;

	flink.handle = fbcon->handle;
	if (ioctl(pRADEONEnt->fd, DRM_IOCTL_GEM_FLINK, &flink) < 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Couldn't flink fbcon handle\n");
		goto out_free_fb;
	}

	bo = calloc(1, sizeof(struct radeon_buffer));
	if (!bo) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Couldn't allocate BO for fbcon handle\n");
		goto out_free_fb;
	}
	bo->ref_count = 1;

	bo->bo.radeon = radeon_bo_open(drmmode->bufmgr, flink.name, 0, 0, 0, 0);
	if (!bo) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Couldn't open BO for fbcon handle\n");
		goto out_free_fb;
	}

	pixmap = drmmode_create_bo_pixmap(pScrn, fbcon->width, fbcon->height,
					  fbcon->depth, fbcon->bpp, fbcon->pitch,
					  bo);
	info->fbcon_pixmap = pixmap;
	radeon_buffer_unref(&bo);
out_free_fb:
	drmModeFreeFB(fbcon);
	return pixmap;
}

static void
destroy_pixmap_for_fbcon(ScrnInfoPtr pScrn)
{
	RADEONInfoPtr info = RADEONPTR(pScrn);

	/* XXX: The current GPUVM support in the kernel doesn't allow removing
	 * the virtual address range for this BO, so we need to keep around
	 * the pixmap to avoid breaking glamor with GPUVM
	 *
	 * Similarly, need to keep around the pixmap with current glamor, to
	 * avoid issues due to a GEM handle lifetime conflict between us and
	 * Mesa
	 */
	if (info->use_glamor &&
	    (info->ChipFamily >= CHIP_FAMILY_CAYMAN ||
	     xorgGetVersion() >= XORG_VERSION_NUMERIC(1,19,99,1,0)))
		return;

	if (info->fbcon_pixmap)
		pScrn->pScreen->DestroyPixmap(info->fbcon_pixmap);
	info->fbcon_pixmap = NULL;
}

void drmmode_copy_fb(ScrnInfoPtr pScrn, drmmode_ptr drmmode)
{
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	RADEONInfoPtr info = RADEONPTR(pScrn);
	ScreenPtr pScreen = pScrn->pScreen;
	PixmapPtr src, dst = pScreen->GetScreenPixmap(pScreen);
	struct drmmode_fb *fb = radeon_pixmap_get_fb(dst);
	int fbcon_id = 0;
	Bool force;
	GCPtr gc;
	int i;

	for (i = 0; i < xf86_config->num_crtc; i++) {
		drmmode_crtc_private_ptr drmmode_crtc = xf86_config->crtc[i]->driver_private;

		if (drmmode_crtc->mode_crtc->buffer_id)
			fbcon_id = drmmode_crtc->mode_crtc->buffer_id;
	}

	if (!fbcon_id)
		return;

	if (fbcon_id == fb->handle) {
		/* in some rare case there might be no fbcon and we might already
		 * be the one with the current fb to avoid a false deadlck in
		 * kernel ttm code just do nothing as anyway there is nothing
		 * to do
		 */
		return;
	}

	src = create_pixmap_for_fbcon(drmmode, pScrn, fbcon_id);
	if (!src)
		return;

	gc = GetScratchGC(pScrn->depth, pScreen);
	ValidateGC(&dst->drawable, gc);

	force = info->accel_state->force;
	info->accel_state->force = TRUE;
	(*gc->ops->CopyArea)(&src->drawable, &dst->drawable, gc, 0, 0,
			     pScrn->virtualX, pScrn->virtualY, 0, 0);
	info->accel_state->force = force;

	FreeScratchGC(gc);

	pScreen->canDoBGNoneRoot = TRUE;
	destroy_pixmap_for_fbcon(pScrn);
	return;
}

void
drmmode_crtc_scanout_destroy(drmmode_ptr drmmode,
			     struct drmmode_scanout *scanout)
{
	if (scanout->pixmap) {
		drmmode_destroy_bo_pixmap(scanout->pixmap);
		scanout->pixmap = NULL;
	}

	radeon_buffer_unref(&scanout->bo);
}

void
drmmode_crtc_scanout_free(xf86CrtcPtr crtc)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

	if (drmmode_crtc->scanout_update_pending) {
		radeon_drm_wait_pending_flip(crtc);
		radeon_drm_abort_entry(drmmode_crtc->scanout_update_pending);
		drmmode_crtc->scanout_update_pending = 0;
		radeon_drm_queue_handle_deferred(crtc);
	}

	drmmode_crtc_scanout_destroy(drmmode_crtc->drmmode,
				     &drmmode_crtc->scanout[0]);
	drmmode_crtc_scanout_destroy(drmmode_crtc->drmmode,
				     &drmmode_crtc->scanout[1]);

	if (drmmode_crtc->scanout_damage)
		DamageDestroy(drmmode_crtc->scanout_damage);
}

PixmapPtr
drmmode_crtc_scanout_create(xf86CrtcPtr crtc, struct drmmode_scanout *scanout,
			    int width, int height)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	RADEONInfoPtr info = RADEONPTR(pScrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;
	struct radeon_surface surface;
	uint32_t tiling = RADEON_CREATE_PIXMAP_TILING_MACRO;
	int pitch;

	if (scanout->pixmap) {
		if (scanout->width == width && scanout->height == height)
			return scanout->pixmap;

		drmmode_crtc_scanout_destroy(drmmode, scanout);
	}

	if (info->ChipFamily >= CHIP_FAMILY_R600)
		tiling |= RADEON_CREATE_PIXMAP_TILING_MICRO;
	scanout->bo = radeon_alloc_pixmap_bo(pScrn, width, height, pScrn->depth,
					     tiling, pScrn->bitsPerPixel,
					     &pitch, &surface, &tiling);
	if (!scanout->bo) {
		ErrorF("failed to create CRTC scanout BO\n");
		return NULL;
	}

	scanout->pixmap = drmmode_create_bo_pixmap(pScrn,
						 width, height,
						 pScrn->depth,
						 pScrn->bitsPerPixel,
						 pitch, scanout->bo);
	if (!scanout->pixmap) {
		ErrorF("failed to create CRTC scanout pixmap\n");
		goto error;
	}

	if (radeon_pixmap_get_fb(scanout->pixmap)) {
		scanout->width = width;
		scanout->height = height;
	} else {
		ErrorF("failed to create CRTC scanout FB\n");
error:		
		drmmode_crtc_scanout_destroy(drmmode, scanout);
	}

	return scanout->pixmap;
}

static void
radeon_screen_damage_report(DamagePtr damage, RegionPtr region, void *closure)
{
	drmmode_crtc_private_ptr drmmode_crtc = closure;

	if (drmmode_crtc->ignore_damage) {
		RegionEmpty(&damage->damage);
		drmmode_crtc->ignore_damage = FALSE;
		return;
	}

	/* Only keep track of the extents */
	RegionUninit(&damage->damage);
	damage->damage.data = NULL;
}

static void
drmmode_screen_damage_destroy(DamagePtr damage, void *closure)
{
	drmmode_crtc_private_ptr drmmode_crtc = closure;

	drmmode_crtc->scanout_damage = NULL;
	RegionUninit(&drmmode_crtc->scanout_last_region);
}

static Bool
drmmode_can_use_hw_cursor(xf86CrtcPtr crtc)
{
	RADEONInfoPtr info = RADEONPTR(crtc->scrn);

	/* Check for Option "SWcursor" */
	if (xf86ReturnOptValBool(info->Options, OPTION_SW_CURSOR, FALSE))
		return FALSE;

	/* Fall back to SW cursor if the CRTC is transformed */
	if (crtc->transformPresent)
		return FALSE;

#if XF86_CRTC_VERSION < 7
	/* Xorg doesn't correctly handle cursor position transform in the
	 * rotation case
	 */
	if (crtc->driverIsPerformingTransform &&
	    (crtc->rotation & 0xf) != RR_Rotate_0)
		return FALSE;
#endif

	/* HW cursor not supported with RandR 1.4 multihead up to 1.18.99.901 */
	if (xorgGetVersion() <= XORG_VERSION_NUMERIC(1,18,99,901,0) &&
	    !xorg_list_is_empty(&crtc->scrn->pScreen->pixmap_dirty_list))
		return FALSE;

	return TRUE;
}

static void
drmmode_crtc_update_tear_free(xf86CrtcPtr crtc)
{
	RADEONInfoPtr info = RADEONPTR(crtc->scrn);
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	int i;

	drmmode_crtc->tear_free = FALSE;

	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];
		drmmode_output_private_ptr drmmode_output = output->driver_private;

		if (output->crtc != crtc)
			continue;

		if (drmmode_output->tear_free == 1 ||
		    (drmmode_output->tear_free == 2 &&
		     (crtc->scrn->pScreen->isGPU ||
		      info->shadow_primary ||
		      crtc->transformPresent || crtc->rotation != RR_Rotate_0))) {
			drmmode_crtc->tear_free = TRUE;
			return;
		}
	}
}

#if XF86_CRTC_VERSION < 7
#define XF86DriverTransformOutput TRUE
#define XF86DriverTransformNone FALSE
#endif

static Bool
drmmode_handle_transform(xf86CrtcPtr crtc)
{
	Bool ret;

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,15,99,903,0)
	crtc->driverIsPerformingTransform = XF86DriverTransformOutput;
#else
	crtc->driverIsPerformingTransform = !crtc->transformPresent &&
		(crtc->rotation & 0xf) == RR_Rotate_0;
#endif

	ret = xf86CrtcRotate(crtc);

	crtc->driverIsPerformingTransform &= ret && crtc->transform_in_use;

	return ret;
}


static void
drmmode_crtc_prime_scanout_update(xf86CrtcPtr crtc, DisplayModePtr mode,
				  unsigned scanout_id, struct drmmode_fb **fb,
				  int *x, int *y)
{
	ScrnInfoPtr scrn = crtc->scrn;
	ScreenPtr screen = scrn->pScreen;
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

	if (drmmode_crtc->tear_free &&
	    !drmmode_crtc->scanout[1].pixmap) {
		RegionPtr region;
		BoxPtr box;

		drmmode_crtc_scanout_create(crtc, &drmmode_crtc->scanout[1],
					    mode->HDisplay,
					    mode->VDisplay);
		region = &drmmode_crtc->scanout_last_region;
		RegionUninit(region);
		region->data = NULL;
		box = RegionExtents(region);
		box->x1 = crtc->x;
		box->y1 = crtc->y;
		box->x2 = crtc->x + mode->HDisplay;
		box->y2 = crtc->y + mode->VDisplay;
	}

	if (scanout_id != drmmode_crtc->scanout_id) {
		PixmapDirtyUpdatePtr dirty = NULL;

		xorg_list_for_each_entry(dirty, &screen->pixmap_dirty_list,
					 ent) {
			if (radeon_dirty_src_equals(dirty, drmmode_crtc->prime_scanout_pixmap)) {
				dirty->secondary_dst =
					drmmode_crtc->scanout[scanout_id].pixmap;
				break;
			}
		}

		if (!drmmode_crtc->tear_free) {
			GCPtr gc = GetScratchGC(scrn->depth, screen);

			ValidateGC(&drmmode_crtc->scanout[0].pixmap->drawable, gc);
			gc->ops->CopyArea(&drmmode_crtc->scanout[1].pixmap->drawable,
					  &drmmode_crtc->scanout[0].pixmap->drawable,
					  gc, 0, 0, mode->HDisplay, mode->VDisplay,
					  0, 0);
			FreeScratchGC(gc);
			radeon_finish(scrn, drmmode_crtc->scanout[0].bo);
		}
	}

	*fb = radeon_pixmap_get_fb(drmmode_crtc->scanout[scanout_id].pixmap);
	*x = *y = 0;
	drmmode_crtc->scanout_id = scanout_id;
}
	

static void
drmmode_crtc_scanout_update(xf86CrtcPtr crtc, DisplayModePtr mode,
			    unsigned scanout_id, struct drmmode_fb **fb, int *x,
			    int *y)
{
	ScrnInfoPtr scrn = crtc->scrn;
	ScreenPtr screen = scrn->pScreen;
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

	drmmode_crtc_scanout_create(crtc, &drmmode_crtc->scanout[scanout_id],
				    mode->HDisplay, mode->VDisplay);
	if (drmmode_crtc->tear_free) {
		drmmode_crtc_scanout_create(crtc,
					    &drmmode_crtc->scanout[scanout_id ^ 1],
					    mode->HDisplay, mode->VDisplay);
	}

	if (drmmode_crtc->scanout[scanout_id].pixmap &&
	    (!drmmode_crtc->tear_free ||
	     drmmode_crtc->scanout[scanout_id ^ 1].pixmap)) {
		BoxRec extents = { .x1 = 0, .y1 = 0,
				   .x2 = scrn->virtualX, .y2 = scrn->virtualY };

		if (!drmmode_crtc->scanout_damage) {
			drmmode_crtc->scanout_damage =
				DamageCreate(radeon_screen_damage_report,
					     drmmode_screen_damage_destroy,
					     DamageReportRawRegion,
					     TRUE, screen, drmmode_crtc);
			DamageRegister(&screen->root->drawable,
				       drmmode_crtc->scanout_damage);
		}

		*fb = radeon_pixmap_get_fb(drmmode_crtc->scanout[scanout_id].pixmap);
		*x = *y = 0;

		if (radeon_scanout_do_update(crtc, scanout_id,
					     screen->GetWindowPixmap(screen->root),
					     extents)) {
			RegionEmpty(DamageRegion(drmmode_crtc->scanout_damage));
			radeon_finish(scrn, drmmode_crtc->scanout[scanout_id].bo);

			if (!drmmode_crtc->flip_pending) {
				radeon_drm_abort_entry(drmmode_crtc->
						       scanout_update_pending);
			}
		}
	}
}

static void
drmmode_crtc_gamma_do_set(xf86CrtcPtr crtc, uint16_t *red, uint16_t *green,
			  uint16_t *blue, int size)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(crtc->scrn);

	drmModeCrtcSetGamma(pRADEONEnt->fd,
			    drmmode_crtc->mode_crtc->crtc_id, size, red, green,
			    blue);
}

Bool
drmmode_set_mode(xf86CrtcPtr crtc, struct drmmode_fb *fb, DisplayModePtr mode,
		 int x, int y)
{
	ScrnInfoPtr scrn = crtc->scrn;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	uint32_t *output_ids = calloc(sizeof(uint32_t), xf86_config->num_output);
	int output_count = 0;
	drmModeModeInfo kmode;
	Bool ret;
	int i;

	if (!output_ids)
		return FALSE;

	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];
		drmmode_output_private_ptr drmmode_output = output->driver_private;

		if (output->crtc != crtc)
			continue;

		output_ids[output_count] = drmmode_output->mode_output->connector_id;
		output_count++;
	}

	drmmode_ConvertToKMode(scrn, &kmode, mode);

	ret = drmModeSetCrtc(pRADEONEnt->fd,
			     drmmode_crtc->mode_crtc->crtc_id,
			     fb->handle, x, y, output_ids,
			     output_count, &kmode) == 0;

	if (ret) {
		drmmode_fb_reference(pRADEONEnt->fd, &drmmode_crtc->fb, fb);
	} else {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "failed to set mode: %s\n", strerror(errno));
	}

	free(output_ids);
	return ret;
}

static Bool
drmmode_set_mode_major(xf86CrtcPtr crtc, DisplayModePtr mode,
		     Rotation rotation, int x, int y)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	ScreenPtr pScreen = pScrn->pScreen;
	RADEONInfoPtr info = RADEONPTR(pScrn);
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	Bool handle_deferred = FALSE;
	unsigned scanout_id = 0;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;
	int saved_x, saved_y;
	Rotation saved_rotation;
	DisplayModeRec saved_mode;
	Bool ret = FALSE;
	int i;
	struct drmmode_fb *fb = NULL;

	/* The root window contents may be undefined before the WindowExposures
	 * hook is called for it, so bail if we get here before that
	 */
	if (pScreen->WindowExposures == RADEONWindowExposures_oneshot)
		return FALSE;

	saved_mode = crtc->mode;
	saved_x = crtc->x;
	saved_y = crtc->y;
	saved_rotation = crtc->rotation;

	if (mode) {
		crtc->mode = *mode;
		crtc->x = x;
		crtc->y = y;
		crtc->rotation = rotation;

		if (!drmmode_handle_transform(crtc))
			goto done;

		drmmode_crtc_update_tear_free(crtc);
		if (drmmode_crtc->tear_free)
			scanout_id = drmmode_crtc->scanout_id;
		else
			drmmode_crtc->scanout_id = 0;

		if (drmmode_crtc->prime_scanout_pixmap) {
			drmmode_crtc_prime_scanout_update(crtc, mode, scanout_id,
							  &fb, &x, &y);
		} else if (drmmode_crtc->rotate.pixmap) {
			fb = radeon_pixmap_get_fb(drmmode_crtc->rotate.pixmap);
			x = y = 0;

		} else if (!pScreen->isGPU &&
			   (drmmode_crtc->tear_free ||
			    crtc->driverIsPerformingTransform ||
			    info->shadow_primary)) {
			drmmode_crtc_scanout_update(crtc, mode, scanout_id,
						    &fb, &x, &y);
		}

		if (!fb)
			fb = radeon_pixmap_get_fb(pScreen->GetWindowPixmap(pScreen->root));
		if (!fb) {
			fb = radeon_fb_create(pScrn, pRADEONEnt->fd,
					      pScrn->virtualX, pScrn->virtualY,
					      pScrn->displayWidth * info->pixel_bytes,
					      info->front_buffer->bo.radeon->handle);
			/* Prevent refcnt of ad-hoc FBs from reaching 2 */
			drmmode_fb_reference(pRADEONEnt->fd, &drmmode_crtc->fb, NULL);
			drmmode_crtc->fb = fb;
		}
		if (!fb) {
			ErrorF("failed to add FB for modeset\n");
			goto done;
		}

		radeon_drm_wait_pending_flip(crtc);
		handle_deferred = TRUE;

		if (!drmmode_set_mode(crtc, fb, mode, x, y))
			goto done;

		ret = TRUE;

		if (pScreen)
			xf86CrtcSetScreenSubpixelOrder(pScreen);

		drmmode_crtc->need_modeset = FALSE;

		/* go through all the outputs and force DPMS them back on? */
		for (i = 0; i < xf86_config->num_output; i++) {
			xf86OutputPtr output = xf86_config->output[i];

			if (output->crtc != crtc)
				continue;

			output->funcs->dpms(output, DPMSModeOn);
		}
	}

	/* Compute index of this CRTC into xf86_config->crtc */
	for (i = 0; i < xf86_config->num_crtc; i++) {
		if (xf86_config->crtc[i] != crtc)
			continue;

		if (!crtc->enabled || drmmode_can_use_hw_cursor(crtc))
			info->hwcursor_disabled &= ~(1 << i);
		else
			info->hwcursor_disabled |= 1 << i;

		break;
	}

#ifndef HAVE_XF86_CURSOR_RESET_CURSOR
	if (!info->hwcursor_disabled)
		xf86_reload_cursors(pScreen);
#endif

done:
	if (!ret) {
		crtc->x = saved_x;
		crtc->y = saved_y;
		crtc->rotation = saved_rotation;
		crtc->mode = saved_mode;
	} else {
		crtc->active = TRUE;

		if (drmmode_crtc->scanout[scanout_id].pixmap &&
		    fb != radeon_pixmap_get_fb(drmmode_crtc->
					       scanout[scanout_id].pixmap)) {
			drmmode_crtc_scanout_free(crtc);
		} else if (!drmmode_crtc->tear_free) {
			drmmode_crtc_scanout_destroy(drmmode,
						     &drmmode_crtc->scanout[1]);
		}
	}

	if (handle_deferred)
		radeon_drm_queue_handle_deferred(crtc);

	return ret;
}

static void
drmmode_set_cursor_colors (xf86CrtcPtr crtc, int bg, int fg)
{

}

static void
drmmode_set_cursor_position (xf86CrtcPtr crtc, int x, int y)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(crtc->scrn);

#if XF86_CRTC_VERSION < 7
	if (crtc->driverIsPerformingTransform) {
		x += crtc->x;
		y += crtc->y;
		xf86CrtcTransformCursorPos(crtc, &x, &y);
	}
#endif

	drmmode_crtc->cursor_x = x;
	drmmode_crtc->cursor_y = y;

	drmModeMoveCursor(pRADEONEnt->fd, drmmode_crtc->mode_crtc->crtc_id, x, y);
}

#if XF86_CRTC_VERSION < 7

static int
drmmode_cursor_src_offset(Rotation rotation, int width, int height,
			  int x_dst, int y_dst)
{
	int t;

	switch (rotation & 0xf) {
	case RR_Rotate_90:
		t = x_dst;
		x_dst = height - y_dst - 1;
		y_dst = t;
		break;
	case RR_Rotate_180:
		x_dst = width - x_dst - 1;
		y_dst = height - y_dst - 1;
		break;
	case RR_Rotate_270:
		t = x_dst;
		x_dst = y_dst;
		y_dst = width - t - 1;
		break;
	}

	if (rotation & RR_Reflect_X)
		x_dst = width - x_dst - 1;
	if (rotation & RR_Reflect_Y)
		y_dst = height - y_dst - 1;

	return y_dst * height + x_dst;
}

#endif

static Bool
drmmode_cursor_pixel(xf86CrtcPtr crtc, uint32_t *argb, Bool *premultiplied,
		     Bool *apply_gamma)
{
	uint32_t alpha = *argb >> 24;
	uint32_t rgb[3];
	int i;

	if (premultiplied) {
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 18, 4, 0, 0)
		if (alpha == 0 && (*argb & 0xffffff) != 0) {
			/* Doesn't look like premultiplied alpha */
			*premultiplied = FALSE;
			return FALSE;
		}
#endif

		if (!(*apply_gamma))
			return TRUE;

		if (*argb > (alpha | alpha << 8 | alpha << 16 | alpha << 24)) {
			/* Un-premultiplied R/G/B would overflow gamma LUT,
			 * don't apply gamma correction
			 */
			*apply_gamma = FALSE;
			return FALSE;
		}
	}

	if (!alpha) {
		*argb = 0;
		return TRUE;
	}

	/* Extract RGB */
	for (i = 0; i < 3; i++)
		rgb[i] = (*argb >> (i * 8)) & 0xff;

	if (premultiplied) {
		/* Un-premultiply alpha */
		for (i = 0; i < 3; i++)
			rgb[i] = rgb[i] * 0xff / alpha;
	}

	if (*apply_gamma) {
		rgb[0] = crtc->gamma_blue[rgb[0]] >> 8;
		rgb[1] = crtc->gamma_green[rgb[1]] >> 8;
		rgb[2] = crtc->gamma_red[rgb[2]] >> 8;
	}

	/* Premultiply alpha */
	for (i = 0; i < 3; i++)
		rgb[i] = rgb[i] * alpha / 0xff;

	*argb = alpha << 24 | rgb[2] << 16 | rgb[1] << 8 | rgb[0];
	return TRUE;
}

static void
drmmode_load_cursor_argb (xf86CrtcPtr crtc, CARD32 *image)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	RADEONInfoPtr info = RADEONPTR(pScrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	unsigned id = drmmode_crtc->cursor_id;
	Bool premultiplied = TRUE;
	Bool apply_gamma = TRUE;
	uint32_t argb;
	uint32_t *ptr;

	if (drmmode_crtc->cursor &&
	    XF86_CRTC_CONFIG_PTR(pScrn)->cursor != drmmode_crtc->cursor)
		id ^= 1;

	/* cursor should be mapped already */
	ptr = (uint32_t *)(drmmode_crtc->cursor_bo[id]->ptr);

	if (crtc->scrn->depth != 24 && crtc->scrn->depth != 32)
		apply_gamma = FALSE;

#if XF86_CRTC_VERSION < 7
	if (crtc->driverIsPerformingTransform) {
		uint32_t cursor_w = info->cursor_w, cursor_h = info->cursor_h;
		int dstx, dsty;
		int srcoffset;

retry_transform:
		for (dsty = 0; dsty < cursor_h; dsty++) {
			for (dstx = 0; dstx < cursor_w; dstx++) {
				srcoffset = drmmode_cursor_src_offset(crtc->rotation,
								      cursor_w,
								      cursor_h,
								      dstx, dsty);
				argb = image[srcoffset];
				if (!drmmode_cursor_pixel(crtc, &argb, &premultiplied,
							  &apply_gamma))
					goto retry_transform;

				ptr[dsty * info->cursor_w + dstx] = cpu_to_le32(argb);
			}
		}
	} else
#endif
	{
		uint32_t cursor_size = info->cursor_w * info->cursor_h;
		int i;

retry:
		for (i = 0; i < cursor_size; i++) {
			argb = image[i];
			if (!drmmode_cursor_pixel(crtc, &argb, &premultiplied,
						  &apply_gamma))
				goto retry;

			ptr[i] = cpu_to_le32(argb);
		}
	}

	if (id != drmmode_crtc->cursor_id) {
		drmmode_crtc->cursor_id = id;
		crtc->funcs->show_cursor(crtc);
	}
}

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,15,99,903,0)

static Bool drmmode_load_cursor_argb_check(xf86CrtcPtr crtc, CARD32 * image)
{
	if (!drmmode_can_use_hw_cursor(crtc))
		return FALSE;

	drmmode_load_cursor_argb(crtc, image);
	return TRUE;
}

#endif

static void
drmmode_hide_cursor (xf86CrtcPtr crtc)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	RADEONInfoPtr info = RADEONPTR(pScrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

	drmModeSetCursor(pRADEONEnt->fd, drmmode_crtc->mode_crtc->crtc_id, 0,
			 info->cursor_w, info->cursor_h);
	drmmode_crtc->cursor = NULL;
}

static void
drmmode_show_cursor (xf86CrtcPtr crtc)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	RADEONInfoPtr info = RADEONPTR(pScrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	CursorPtr cursor = xf86_config->cursor;
	int xhot = cursor->bits->xhot;
	int yhot = cursor->bits->yhot;
	static Bool use_set_cursor2 = TRUE;
	struct drm_mode_cursor2 arg;

	drmmode_crtc->cursor = xf86_config->cursor;

	memset(&arg, 0, sizeof(arg));

	arg.handle = drmmode_crtc->cursor_bo[drmmode_crtc->cursor_id]->handle;
	arg.flags = DRM_MODE_CURSOR_BO;
	arg.crtc_id = drmmode_crtc->mode_crtc->crtc_id;
	arg.width = info->cursor_w;
	arg.height = info->cursor_h;

	if (crtc->rotation != RR_Rotate_0 &&
	    crtc->rotation != (RR_Rotate_180 | RR_Reflect_X |
			       RR_Reflect_Y)) {
	    int t;

	    /* Reflect & rotate hotspot position */
	    if (crtc->rotation & RR_Reflect_X)
		xhot = info->cursor_w - xhot - 1;
	    if (crtc->rotation & RR_Reflect_Y)
		yhot = info->cursor_h - yhot - 1;

	    switch (crtc->rotation & 0xf) {
	    case RR_Rotate_90:
		t = xhot;
		xhot = yhot;
		yhot = info->cursor_w - t - 1;
		break;
	    case RR_Rotate_180:
		xhot = info->cursor_w - xhot - 1;
		yhot = info->cursor_h - yhot - 1;
		break;
	    case RR_Rotate_270:
		t = xhot;
		xhot = info->cursor_h - yhot - 1;
		yhot = t;
	    }
	}

	if (xhot != drmmode_crtc->cursor_xhot || yhot != drmmode_crtc->cursor_yhot) {
	    arg.flags |= DRM_MODE_CURSOR_MOVE;
	    arg.x = drmmode_crtc->cursor_x += drmmode_crtc->cursor_xhot - xhot;
	    arg.y = drmmode_crtc->cursor_y += drmmode_crtc->cursor_yhot - yhot;
	    drmmode_crtc->cursor_xhot = xhot;
	    drmmode_crtc->cursor_yhot = yhot;
	}

	if (use_set_cursor2) {
	    int ret;

	    arg.hot_x = xhot;
	    arg.hot_y = yhot;

	    ret = drmIoctl(pRADEONEnt->fd, DRM_IOCTL_MODE_CURSOR2, &arg);
	    if (ret == -1 && errno == EINVAL)
		use_set_cursor2 = FALSE;
	    else
		return;
	}

	drmIoctl(pRADEONEnt->fd, DRM_IOCTL_MODE_CURSOR, &arg);
}

/* Xorg expects a non-NULL return value from drmmode_crtc_shadow_allocate, and
 * passes that back to drmmode_crtc_scanout_create; it doesn't use it for
 * anything else.
 */
static void *
drmmode_crtc_shadow_allocate(xf86CrtcPtr crtc, int width, int height)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

	if (!drmmode_crtc_scanout_create(crtc, &drmmode_crtc->rotate, width,
					 height))
		return NULL;

	return (void*)~0UL;
}

static PixmapPtr
drmmode_crtc_shadow_create(xf86CrtcPtr crtc, void *data, int width, int height)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

	if (!data) {
		drmmode_crtc_scanout_create(crtc, &drmmode_crtc->rotate, width,
					    height);
	}

	return drmmode_crtc->rotate.pixmap;
}

static void
drmmode_crtc_shadow_destroy(xf86CrtcPtr crtc, PixmapPtr rotate_pixmap, void *data)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;

	drmmode_crtc_scanout_destroy(drmmode, &drmmode_crtc->rotate);
}

static void
drmmode_crtc_gamma_set(xf86CrtcPtr crtc, uint16_t *red, uint16_t *green,
                      uint16_t *blue, int size)
{
	ScrnInfoPtr scrn = crtc->scrn;
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	RADEONInfoPtr info = RADEONPTR(scrn);
	int i;

	drmmode_crtc_gamma_do_set(crtc, red, green, blue, size);

	/* Compute index of this CRTC into xf86_config->crtc */
	for (i = 0; xf86_config->crtc[i] != crtc; i++) {}

	if (info->hwcursor_disabled & (1 << i))
		return;

#ifdef HAVE_XF86_CURSOR_RESET_CURSOR
	xf86CursorResetCursor(scrn->pScreen);
#else
	xf86_reload_cursors(scrn->pScreen);
#endif
}

static Bool
drmmode_set_scanout_pixmap(xf86CrtcPtr crtc, PixmapPtr ppix)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	unsigned scanout_id = drmmode_crtc->scanout_id;
	ScreenPtr screen = crtc->scrn->pScreen;
	PixmapDirtyUpdatePtr dirty;

	xorg_list_for_each_entry(dirty, &screen->pixmap_dirty_list, ent) {
		if (radeon_dirty_src_equals(dirty, drmmode_crtc->prime_scanout_pixmap)) {
			PixmapStopDirtyTracking(dirty->src, dirty->secondary_dst);
			break;
		}
	}

	drmmode_crtc_scanout_free(crtc);
	drmmode_crtc->prime_scanout_pixmap = NULL;

	if (!ppix)
		return TRUE;

	if (!drmmode_crtc_scanout_create(crtc, &drmmode_crtc->scanout[0],
					 ppix->drawable.width,
					 ppix->drawable.height))
		return FALSE;

	if (drmmode_crtc->tear_free &&
	    !drmmode_crtc_scanout_create(crtc, &drmmode_crtc->scanout[1],
					 ppix->drawable.width,
					 ppix->drawable.height)) {
		drmmode_crtc_scanout_free(crtc);
		return FALSE;
	}

	drmmode_crtc->prime_scanout_pixmap = ppix;

#ifdef HAS_DIRTYTRACKING_DRAWABLE_SRC
	PixmapStartDirtyTracking(&ppix->drawable,
				 drmmode_crtc->scanout[scanout_id].pixmap,
				 0, 0, 0, 0, RR_Rotate_0);
#elif defined(HAS_DIRTYTRACKING_ROTATION)
	PixmapStartDirtyTracking(ppix, drmmode_crtc->scanout[scanout_id].pixmap,
				 0, 0, 0, 0, RR_Rotate_0);
#elif defined(HAS_DIRTYTRACKING2)
	PixmapStartDirtyTracking2(ppix, drmmode_crtc->scanout[scanout_id].pixmap,
				  0, 0, 0, 0);
#else
	PixmapStartDirtyTracking(ppix, drmmode_crtc->scanout[scanout_id].pixmap, 0, 0);
#endif
	return TRUE;
}

static xf86CrtcFuncsRec drmmode_crtc_funcs = {
    .dpms = drmmode_crtc_dpms,
    .set_mode_major = drmmode_set_mode_major,
    .set_cursor_colors = drmmode_set_cursor_colors,
    .set_cursor_position = drmmode_set_cursor_position,
    .show_cursor = drmmode_show_cursor,
    .hide_cursor = drmmode_hide_cursor,
    .load_cursor_argb = drmmode_load_cursor_argb,
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,15,99,903,0)
    .load_cursor_argb_check = drmmode_load_cursor_argb_check,
#endif

    .gamma_set = drmmode_crtc_gamma_set,
    .shadow_create = drmmode_crtc_shadow_create,
    .shadow_allocate = drmmode_crtc_shadow_allocate,
    .shadow_destroy = drmmode_crtc_shadow_destroy,
    .destroy = NULL, /* XXX */
    .set_scanout_pixmap = drmmode_set_scanout_pixmap,
};

int drmmode_get_crtc_id(xf86CrtcPtr crtc)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	return drmmode_crtc->hw_id;
}

void drmmode_crtc_hw_id(xf86CrtcPtr crtc)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(crtc->scrn);
	struct drm_radeon_info ginfo;
	int r;
	uint32_t tmp;

	memset(&ginfo, 0, sizeof(ginfo));
	ginfo.request = 0x4;
	tmp = drmmode_crtc->mode_crtc->crtc_id;
	ginfo.value = (uintptr_t)&tmp;
	r = drmCommandWriteRead(pRADEONEnt->fd, DRM_RADEON_INFO, &ginfo, sizeof(ginfo));
	if (r) {
		drmmode_crtc->hw_id = -1;
		return;
	}
	drmmode_crtc->hw_id = tmp;
}

static unsigned int
drmmode_crtc_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode, drmModeResPtr mode_res, int num)
{
	xf86CrtcPtr crtc;
	drmmode_crtc_private_ptr drmmode_crtc;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	RADEONInfoPtr info = RADEONPTR(pScrn);

	crtc = xf86CrtcCreate(pScrn, &info->drmmode_crtc_funcs);
	if (!crtc)
		return 0;

	drmmode_crtc = xnfcalloc(sizeof(drmmode_crtc_private_rec), 1);
	drmmode_crtc->mode_crtc = drmModeGetCrtc(pRADEONEnt->fd, mode_res->crtcs[num]);
	drmmode_crtc->drmmode = drmmode;
	drmmode_crtc->dpms_mode = DPMSModeOff;
	crtc->driver_private = drmmode_crtc;
	drmmode_crtc_hw_id(crtc);

	/* Mark num'th crtc as in use on this device. */
	pRADEONEnt->assigned_crtcs |= (1 << num);
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Allocated crtc nr. %d to this screen.\n", num);

	return 1;
}

/*
 * Update all of the property values for an output
 */
static void
drmmode_output_update_properties(xf86OutputPtr output)
{
	drmmode_output_private_ptr drmmode_output = output->driver_private;
	int i, j, k;
	int err;
	drmModeConnectorPtr koutput;

	/* Use the most recently fetched values from the kernel */
	koutput = drmmode_output->mode_output;

	if (!koutput)
		return;

	for (i = 0; i < drmmode_output->num_props; i++) {
		drmmode_prop_ptr p = &drmmode_output->props[i];

		for (j = 0; j < koutput->count_props; j++) {
			if (koutput->props[j] != p->mode_prop->prop_id)
				continue;

			/* Check to see if the property value has changed */
			if (koutput->prop_values[j] == p->value)
				break;

			p->value = koutput->prop_values[j];

			if (p->mode_prop->flags & DRM_MODE_PROP_RANGE) {
				INT32 value = p->value;

				err = RRChangeOutputProperty(output->randr_output,
							     p->atoms[0], XA_INTEGER,
							     32, PropModeReplace, 1,
							     &value, FALSE, TRUE);
				if (err != 0) {
					xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
						   "RRChangeOutputProperty error, %d\n",
						   err);
				}
			} else if (p->mode_prop->flags & DRM_MODE_PROP_ENUM) {
				for (k = 0; k < p->mode_prop->count_enums; k++) {
					if (p->mode_prop->enums[k].value == p->value)
						break;
				}
				if (k < p->mode_prop->count_enums) {
					err = RRChangeOutputProperty(output->randr_output,
								     p->atoms[0], XA_ATOM,
								     32, PropModeReplace, 1,
								     &p->atoms[k + 1], FALSE,
								     TRUE);
					if (err != 0) {
						xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
							   "RRChangeOutputProperty error, %d\n",
							   err);
					}
				}
			}

			break;
		}
        }
}

static xf86OutputStatus
drmmode_output_detect(xf86OutputPtr output)
{
	/* go to the hw and retrieve a new output struct */
	drmmode_output_private_ptr drmmode_output = output->driver_private;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(output->scrn);
	xf86OutputStatus status;
	drmModeFreeConnector(drmmode_output->mode_output);

	drmmode_output->mode_output =
	    drmModeGetConnector(pRADEONEnt->fd, drmmode_output->output_id);
	if (!drmmode_output->mode_output) {
		drmmode_output->output_id = -1;
		return XF86OutputStatusDisconnected;
	}

	drmmode_output_update_properties(output);

	switch (drmmode_output->mode_output->connection) {
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
drmmode_output_mode_valid(xf86OutputPtr output, DisplayModePtr pModes)
{
	return MODE_OK;
}

static void
drmmode_output_attach_tile(xf86OutputPtr output)
{
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1, 17, 99, 901, 0)
	drmmode_output_private_ptr drmmode_output = output->driver_private;
	drmModeConnectorPtr koutput = drmmode_output->mode_output;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(output->scrn);
	struct xf86CrtcTileInfo tile_info, *set = NULL;
	int i;

	if (!koutput) {
		xf86OutputSetTile(output, NULL);
		return;
	}

	/* look for a TILE property */
	for (i = 0; i < koutput->count_props; i++) {
		drmModePropertyPtr props;
		props = drmModeGetProperty(pRADEONEnt->fd, koutput->props[i]);
		if (!props)
			continue;

		if (!(props->flags & DRM_MODE_PROP_BLOB)) {
			drmModeFreeProperty(props);
			continue;
		}

		if (!strcmp(props->name, "TILE")) {
			drmModeFreePropertyBlob(drmmode_output->tile_blob);
			drmmode_output->tile_blob =
				drmModeGetPropertyBlob(pRADEONEnt->fd,
						       koutput->prop_values[i]);
		}
		drmModeFreeProperty(props);
	}
	if (drmmode_output->tile_blob) {
		if (xf86OutputParseKMSTile(drmmode_output->tile_blob->data,
					   drmmode_output->tile_blob->length,
					   &tile_info) == TRUE)
			set = &tile_info;
	}
	xf86OutputSetTile(output, set);
#endif
}

static int
koutput_get_prop_idx(int fd, drmModeConnectorPtr koutput,
        int type, const char *name)
{
    int idx = -1;

    for (int i = 0; i < koutput->count_props; i++) {
        drmModePropertyPtr prop = drmModeGetProperty(fd, koutput->props[i]);

        if (!prop)
            continue;

        if (drm_property_type_is(prop, type) && !strcmp(prop->name, name))
            idx = i;

        drmModeFreeProperty(prop);

        if (idx > -1)
            break;
    }

    return idx;
}

static int
koutput_get_prop_id(int fd, drmModeConnectorPtr koutput,
        int type, const char *name)
{
    int idx = koutput_get_prop_idx(fd, koutput, type, name);

    return (idx > -1) ? koutput->props[idx] : -1;
}

static drmModePropertyBlobPtr
koutput_get_prop_blob(int fd, drmModeConnectorPtr koutput, const char *name)
{
    drmModePropertyBlobPtr blob = NULL;
    int idx = koutput_get_prop_idx(fd, koutput, DRM_MODE_PROP_BLOB, name);

    if (idx > -1)
        blob = drmModeGetPropertyBlob(fd, koutput->prop_values[idx]);

    return blob;
}

static DisplayModePtr
drmmode_output_get_modes(xf86OutputPtr output)
{
	drmmode_output_private_ptr drmmode_output = output->driver_private;
	drmModeConnectorPtr koutput = drmmode_output->mode_output;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(output->scrn);
	int i;
	DisplayModePtr Modes = NULL, Mode;
	xf86MonPtr mon = NULL;

	if (!koutput)
		return NULL;

	drmModeFreePropertyBlob(drmmode_output->edid_blob);

	/* look for an EDID property */
	drmmode_output->edid_blob =
		koutput_get_prop_blob(pRADEONEnt->fd, koutput, "EDID");

	if (drmmode_output->edid_blob) {
		mon = xf86InterpretEDID(output->scrn->scrnIndex,
					drmmode_output->edid_blob->data);
		if (mon && drmmode_output->edid_blob->length > 128)
			mon->flags |= MONITOR_EDID_COMPLETE_RAWDATA;
	}
	xf86OutputSetEDID(output, mon);

	drmmode_output_attach_tile(output);

	/* modes should already be available */
	for (i = 0; i < koutput->count_modes; i++) {
		Mode = xnfalloc(sizeof(DisplayModeRec));

		drmmode_ConvertFromKMode(output->scrn, &koutput->modes[i], Mode);
		Modes = xf86ModesAdd(Modes, Mode);

	}
	return Modes;
}

static void
drmmode_output_destroy(xf86OutputPtr output)
{
	drmmode_output_private_ptr drmmode_output = output->driver_private;
	int i;

	drmModeFreePropertyBlob(drmmode_output->edid_blob);
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1, 17, 99, 901, 0)
	drmModeFreePropertyBlob(drmmode_output->tile_blob);
#endif

	for (i = 0; i < drmmode_output->num_props; i++) {
		drmModeFreeProperty(drmmode_output->props[i].mode_prop);
		free(drmmode_output->props[i].atoms);
	}
	for (i = 0; i < drmmode_output->mode_output->count_encoders; i++) {
		drmModeFreeEncoder(drmmode_output->mode_encoders[i]);
	}
	free(drmmode_output->mode_encoders);
	free(drmmode_output->props);
	drmModeFreeConnector(drmmode_output->mode_output);
	free(drmmode_output);
	output->driver_private = NULL;
}

static void
drmmode_output_dpms(xf86OutputPtr output, int mode)
{
	drmmode_output_private_ptr drmmode_output = output->driver_private;
	xf86CrtcPtr crtc = output->crtc;
	drmModeConnectorPtr koutput = drmmode_output->mode_output;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(output->scrn);

	if (!koutput)
		return;

	if (mode != DPMSModeOn && crtc)
		drmmode_do_crtc_dpms(crtc, mode);

	drmModeConnectorSetProperty(pRADEONEnt->fd, koutput->connector_id,
				    drmmode_output->dpms_enum_id, mode);

	if (mode == DPMSModeOn && crtc) {
	    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

	    if (drmmode_crtc->need_modeset)
		drmmode_set_mode_major(crtc, &crtc->mode, crtc->rotation, crtc->x,
				       crtc->y);
	    else
		drmmode_do_crtc_dpms(crtc, mode);
	}
}


static Bool
drmmode_property_ignore(drmModePropertyPtr prop)
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
drmmode_output_create_resources(xf86OutputPtr output)
{
    RADEONInfoPtr info = RADEONPTR(output->scrn);
    drmmode_output_private_ptr drmmode_output = output->driver_private;
    drmModeConnectorPtr mode_output = drmmode_output->mode_output;
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(output->scrn);
    drmModePropertyPtr drmmode_prop, tearfree_prop;
    int i, j, err;
    Atom name;

    /* Create CONNECTOR_ID property */
    name = MakeAtom("CONNECTOR_ID", 12, TRUE);
    if (name != BAD_RESOURCE) {
	INT32 value = mode_output->connector_id;

	err = RRConfigureOutputProperty(output->randr_output, name,
					FALSE, FALSE, TRUE, 1, &value);
	if (err != Success) {
	    xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", err);
	}

	err = RRChangeOutputProperty(output->randr_output, name,
				     XA_INTEGER, 32, PropModeReplace, 1,
				     &value, FALSE, FALSE);
	if (err != Success) {
	    xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
	}
    }

    drmmode_output->props = calloc(mode_output->count_props + 1, sizeof(drmmode_prop_rec));
    if (!drmmode_output->props)
	return;
    
    drmmode_output->num_props = 0;
    for (i = 0, j = 0; i < mode_output->count_props; i++) {
	drmmode_prop = drmModeGetProperty(pRADEONEnt->fd, mode_output->props[i]);
	if (drmmode_property_ignore(drmmode_prop)) {
	    drmModeFreeProperty(drmmode_prop);
	    continue;
	}
	drmmode_output->props[j].mode_prop = drmmode_prop;
	drmmode_output->props[j].value = mode_output->prop_values[i];
	drmmode_output->num_props++;
	j++;
    }

    /* Userspace-only property for TearFree */
    tearfree_prop = calloc(1, sizeof(*tearfree_prop));
    tearfree_prop->flags = DRM_MODE_PROP_ENUM;
    strcpy(tearfree_prop->name, "TearFree");
    tearfree_prop->count_enums = 3;
    tearfree_prop->enums = calloc(tearfree_prop->count_enums,
				  sizeof(*tearfree_prop->enums));
    strcpy(tearfree_prop->enums[0].name, "off");
    strcpy(tearfree_prop->enums[1].name, "on");
    tearfree_prop->enums[1].value = 1;
    strcpy(tearfree_prop->enums[2].name, "auto");
    tearfree_prop->enums[2].value = 2;
    drmmode_output->props[j].mode_prop = tearfree_prop;
    drmmode_output->props[j].value = info->tear_free;
    drmmode_output->tear_free = info->tear_free;
    drmmode_output->num_props++;

    for (i = 0; i < drmmode_output->num_props; i++) {
	drmmode_prop_ptr p = &drmmode_output->props[i];
	drmmode_prop = p->mode_prop;

	if (drmmode_prop->flags & DRM_MODE_PROP_RANGE) {
	    INT32 range[2];
	    INT32 value = p->value;

	    p->num_atoms = 1;
	    p->atoms = calloc(p->num_atoms, sizeof(Atom));
	    if (!p->atoms)
		continue;
	    p->atoms[0] = MakeAtom(drmmode_prop->name, strlen(drmmode_prop->name), TRUE);
	    range[0] = drmmode_prop->values[0];
	    range[1] = drmmode_prop->values[1];
	    err = RRConfigureOutputProperty(output->randr_output, p->atoms[0],
		    FALSE, TRUE,
		    drmmode_prop->flags & DRM_MODE_PROP_IMMUTABLE ? TRUE : FALSE,
		    2, range);
	    if (err != 0) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			"RRConfigureOutputProperty error, %d\n", err);
	    }
	    err = RRChangeOutputProperty(output->randr_output, p->atoms[0],
		    XA_INTEGER, 32, PropModeReplace, 1, &value, FALSE, TRUE);
	    if (err != 0) {
		xf86DrvMsg(output->scrn->scrnIndex, X_ERROR,
			"RRChangeOutputProperty error, %d\n", err);
	    }
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
}

static void
drmmode_output_set_tear_free(RADEONEntPtr pRADEONEnt,
			     drmmode_output_private_ptr drmmode_output,
			     xf86CrtcPtr crtc, int tear_free)
{
	if (drmmode_output->tear_free == tear_free)
		return;

	drmmode_output->tear_free = tear_free;

	if (crtc) {
		drmmode_set_mode_major(crtc, &crtc->mode, crtc->rotation,
				       crtc->x, crtc->y);
	}
}

static Bool
drmmode_output_set_property(xf86OutputPtr output, Atom property,
		RRPropertyValuePtr value)
{
    drmmode_output_private_ptr drmmode_output = output->driver_private;
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(output->scrn);
    int i;

    for (i = 0; i < drmmode_output->num_props; i++) {
	drmmode_prop_ptr p = &drmmode_output->props[i];

	if (p->atoms[0] != property)
	    continue;

	if (p->mode_prop->flags & DRM_MODE_PROP_RANGE) {
	    uint32_t val;

	    if (value->type != XA_INTEGER || value->format != 32 ||
		    value->size != 1)
		return FALSE;
	    val = *(uint32_t *)value->data;

	    drmModeConnectorSetProperty(pRADEONEnt->fd, drmmode_output->output_id,
		    p->mode_prop->prop_id, (uint64_t)val);
	    return TRUE;
	} else if (p->mode_prop->flags & DRM_MODE_PROP_ENUM) {
	    Atom	atom;
	    const char	*name;
	    int		j;

	    if (value->type != XA_ATOM || value->format != 32 || value->size != 1)
		return FALSE;
	    memcpy(&atom, value->data, 4);
	    if (!(name = NameForAtom(atom)))
                return FALSE;

	    /* search for matching name string, then set its value down */
	    for (j = 0; j < p->mode_prop->count_enums; j++) {
		if (!strcmp(p->mode_prop->enums[j].name, name)) {
		    if (i == (drmmode_output->num_props - 1)) {
			drmmode_output_set_tear_free(pRADEONEnt, drmmode_output,
						     output->crtc, j);
		    } else {
			drmModeConnectorSetProperty(pRADEONEnt->fd,
						    drmmode_output->output_id,
						    p->mode_prop->prop_id,
						    p->mode_prop->enums[j].value);
		    }

		    return TRUE;
		}
	    }
	}
    }

    return TRUE;
}

static Bool
drmmode_output_get_property(xf86OutputPtr output, Atom property)
{
    return TRUE;
}

static const xf86OutputFuncsRec drmmode_output_funcs = {
    .dpms = drmmode_output_dpms,
    .create_resources = drmmode_output_create_resources,
    .set_property = drmmode_output_set_property,
    .get_property = drmmode_output_get_property,
    .detect = drmmode_output_detect,
    .mode_valid = drmmode_output_mode_valid,

    .get_modes = drmmode_output_get_modes,
    .destroy = drmmode_output_destroy
};

static int subpixel_conv_table[7] = { 0, SubPixelUnknown,
				      SubPixelHorizontalRGB,
				      SubPixelHorizontalBGR,
				      SubPixelVerticalRGB,
				      SubPixelVerticalBGR,
				      SubPixelNone };

const char *output_names[] = { "None",
			       "VGA",
			       "DVI",
			       "DVI",
			       "DVI",
			       "Composite",
			       "S-video",
			       "LVDS",
			       "CTV",
			       "DIN",
			       "DisplayPort",
			       "HDMI",
			       "HDMI",
			       "TV",
			       "eDP"
};

#define NUM_OUTPUT_NAMES (sizeof(output_names) / sizeof(output_names[0]))

static xf86OutputPtr find_output(ScrnInfoPtr pScrn, int id)
{
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	int i;
	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];
		drmmode_output_private_ptr drmmode_output;

		drmmode_output = output->driver_private;
		if (drmmode_output->output_id == id)
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
		    drmModePropertyBlobPtr path_blob, int *num_dvi, int *num_hdmi)
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
		if (koutput->connector_type >= NUM_OUTPUT_NAMES) {
			snprintf(name, 32, "Unknown%d-%d", koutput->connector_type,
				 koutput->connector_type_id - 1);
		} else if (pScrn->is_gpu) {
			snprintf(name, 32, "%s-%d-%d",
				 output_names[koutput->connector_type], pScrn->scrnIndex - GPU_SCREEN_OFFSET + 1,
				 koutput->connector_type_id - 1);
		} else {
			/* need to do smart conversion here for compat with non-kms ATI driver */
			if (koutput->connector_type_id == 1) {
				switch(koutput->connector_type) {
				case DRM_MODE_CONNECTOR_DVII:
				case DRM_MODE_CONNECTOR_DVID:
				case DRM_MODE_CONNECTOR_DVIA:
					snprintf(name, 32, "%s-%d", output_names[koutput->connector_type], *num_dvi);
					(*num_dvi)++;
					break;
				case DRM_MODE_CONNECTOR_HDMIA:
				case DRM_MODE_CONNECTOR_HDMIB:
					snprintf(name, 32, "%s-%d", output_names[koutput->connector_type], *num_hdmi);
					(*num_hdmi)++;
					break;
				case DRM_MODE_CONNECTOR_VGA:
				case DRM_MODE_CONNECTOR_DisplayPort:
					snprintf(name, 32, "%s-%d", output_names[koutput->connector_type],
						 koutput->connector_type_id - 1);
					break;
				default:
					snprintf(name, 32, "%s", output_names[koutput->connector_type]);
					break;
				}
			} else {
				snprintf(name, 32, "%s-%d", output_names[koutput->connector_type],
					 koutput->connector_type_id - 1);
			}
		}
	}
}

static unsigned int
drmmode_output_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode, drmModeResPtr mode_res, int num, int *num_dvi, int *num_hdmi, int dynamic)
{
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	RADEONInfoPtr info = RADEONPTR(pScrn);
	xf86OutputPtr output;
	drmModeConnectorPtr koutput;
	drmModeEncoderPtr *kencoders = NULL;
	drmmode_output_private_ptr drmmode_output;
	drmModePropertyBlobPtr path_blob = NULL;
#if XF86_CRTC_VERSION >= 8
	Bool nonDesktop = FALSE;
#endif
	char name[32];
	int i;
	const char *s;

	koutput = drmModeGetConnector(pRADEONEnt->fd, mode_res->connectors[num]);
	if (!koutput)
		return 0;

	path_blob = koutput_get_prop_blob(pRADEONEnt->fd, koutput, "PATH");

#if XF86_CRTC_VERSION >= 8
	i = koutput_get_prop_idx(pRADEONEnt->fd, koutput, DRM_MODE_PROP_RANGE,
				 "non-desktop");
	if (i >= 0)
		nonDesktop = koutput->prop_values[i] != 0;
#endif

	kencoders = calloc(sizeof(drmModeEncoderPtr), koutput->count_encoders);
	if (!kencoders) {
		goto out_free_encoders;
	}

	for (i = 0; i < koutput->count_encoders; i++) {
		kencoders[i] = drmModeGetEncoder(pRADEONEnt->fd, koutput->encoders[i]);
		if (!kencoders[i]) {
			goto out_free_encoders;
		}
	}

	drmmode_create_name(pScrn, koutput, name, path_blob, num_dvi, num_hdmi);
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

			drmmode_output = output->driver_private;
			drmmode_output->output_id = mode_res->connectors[num];
			drmmode_output->mode_output = koutput;
#if XF86_CRTC_VERSION >= 8
			output->non_desktop = nonDesktop;
#endif
			for (i = 0; i < koutput->count_encoders; i++)
				drmModeFreeEncoder(kencoders[i]);
			free(kencoders);
			return 0;
		}
	}

	if (xf86IsEntityShared(pScrn->entityList[0])) {
		if ((s = xf86GetOptValString(info->Options, OPTION_ZAPHOD_HEADS))) {
			if (!RADEONZaphodStringMatches(pScrn, s, name))
				goto out_free_encoders;
		} else {
			if (info->instance_id != num)
				goto out_free_encoders;
		}
	}

	output = xf86OutputCreate (pScrn, &drmmode_output_funcs, name);
	if (!output) {
		goto out_free_encoders;
	}

	drmmode_output = calloc(sizeof(drmmode_output_private_rec), 1);
	if (!drmmode_output) {
		xf86OutputDestroy(output);
		goto out_free_encoders;
	}

	drmmode_output->output_id = mode_res->connectors[num];
	drmmode_output->mode_output = koutput;
	drmmode_output->mode_encoders = kencoders;
	drmmode_output->drmmode = drmmode;
	output->mm_width = koutput->mmWidth;
	output->mm_height = koutput->mmHeight;

	output->subpixel_order = subpixel_conv_table[koutput->subpixel];
	output->interlaceAllowed = TRUE;
	output->doubleScanAllowed = TRUE;
	output->driver_private = drmmode_output;
#if XF86_CRTC_VERSION >= 8
	output->non_desktop = nonDesktop;
#endif
	
	output->possible_crtcs = 0xffffffff;
	for (i = 0; i < koutput->count_encoders; i++) {
		output->possible_crtcs &= kencoders[i]->possible_crtcs;
	}
	/* work out the possible clones later */
	output->possible_clones = 0;

	drmmode_output->dpms_enum_id =
		koutput_get_prop_id(pRADEONEnt->fd, koutput, DRM_MODE_PROP_ENUM,
				    "DPMS");

	if (dynamic) {
		output->randr_output = RROutputCreate(xf86ScrnToScreen(pScrn), output->name, strlen(output->name), output);
		drmmode_output_create_resources(output);
	}

	return 1;
out_free_encoders:
	if (kencoders){
		for (i = 0; i < koutput->count_encoders; i++)
			drmModeFreeEncoder(kencoders[i]);
		free(kencoders);
	}
	drmModeFreeConnector(koutput);
	return 0;
}

uint32_t find_clones(ScrnInfoPtr scrn, xf86OutputPtr output)
{
	drmmode_output_private_ptr drmmode_output = output->driver_private, clone_drmout;
	int i;
	xf86OutputPtr clone_output;
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	int index_mask = 0;

	if (drmmode_output->enc_clone_mask == 0)
		return index_mask;

	for (i = 0; i < xf86_config->num_output; i++) {
		clone_output = xf86_config->output[i];
		clone_drmout = clone_output->driver_private;
		if (output == clone_output)
			continue;
		
		if (clone_drmout->enc_mask == 0)
			continue;
		if (drmmode_output->enc_clone_mask == clone_drmout->enc_mask)
			index_mask |= (1 << i);
	}
	return index_mask;
}


static void
drmmode_clones_init(ScrnInfoPtr scrn, drmmode_ptr drmmode, drmModeResPtr mode_res)
{
	int i, j;
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);

	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];
		drmmode_output_private_ptr drmmode_output;

		drmmode_output = output->driver_private;
		drmmode_output->enc_clone_mask = 0xff;
		/* and all the possible encoder clones for this output together */
		for (j = 0; j < drmmode_output->mode_output->count_encoders; j++)
		{
			int k;
			for (k = 0; k < mode_res->count_encoders; k++) {
				if (mode_res->encoders[k] == drmmode_output->mode_encoders[j]->encoder_id)
					drmmode_output->enc_mask |= (1 << k);
			}

			drmmode_output->enc_clone_mask &= drmmode_output->mode_encoders[j]->possible_clones;
		}
	}

	for (i = 0; i < xf86_config->num_output; i++) {
		xf86OutputPtr output = xf86_config->output[i];
		output->possible_clones = find_clones(scrn, output);
	}
}

/* returns height alignment in pixels */
int drmmode_get_height_align(ScrnInfoPtr scrn, uint32_t tiling)
{
	RADEONInfoPtr info = RADEONPTR(scrn);
	int height_align = 1;

	if (info->ChipFamily >= CHIP_FAMILY_R600) {
		if (tiling & RADEON_TILING_MACRO)
			height_align =  info->num_channels * 8;
		else if (tiling & RADEON_TILING_MICRO)
			height_align = 8;
		else
			height_align = 8;
	} else {
		if (tiling & RADEON_TILING_MICRO_SQUARE)
			height_align =  32;
		else if (tiling)
			height_align = 16;
		else
			height_align = 1;
	}
	return height_align;
}

/* returns pitch alignment in pixels */
int drmmode_get_pitch_align(ScrnInfoPtr scrn, int bpe, uint32_t tiling)
{
	RADEONInfoPtr info = RADEONPTR(scrn);
	int pitch_align = 1;

	if (info->ChipFamily >= CHIP_FAMILY_R600) {
		if (tiling & RADEON_TILING_MACRO) {
			/* general surface requirements */
			pitch_align = MAX(info->num_banks,
					  (((info->group_bytes / 8) / bpe) * info->num_banks)) * 8;
			/* further restrictions for scanout */
			pitch_align = MAX(info->num_banks * 8, pitch_align);
		} else if (tiling & RADEON_TILING_MICRO) {
			/* general surface requirements */
			pitch_align = MAX(8, (info->group_bytes / (8 * bpe)));
			/* further restrictions for scanout */
			pitch_align = MAX(info->group_bytes / bpe, pitch_align);
		} else {
			if (info->have_tiling_info)
				/* linear aligned requirements */
				pitch_align = MAX(64, info->group_bytes / bpe);
			else
				/* default to 512 elements if we don't know the real
				 * group size otherwise the kernel may reject the CS
				 * if the group sizes don't match as the pitch won't
				 * be aligned properly.
				 */
				pitch_align = 512;
		}
	} else {
		/* general surface requirements */
		if (tiling)
			pitch_align = 256 / bpe;
		else
			pitch_align = 64;
	}
	return pitch_align;
}

/* returns base alignment in bytes */
int drmmode_get_base_align(ScrnInfoPtr scrn, int bpe, uint32_t tiling)
{
	RADEONInfoPtr info = RADEONPTR(scrn);
	int pixel_align = drmmode_get_pitch_align(scrn, bpe, tiling);
	int height_align = drmmode_get_height_align(scrn, tiling);
	int base_align = RADEON_GPU_PAGE_SIZE;

	if (info->ChipFamily >= CHIP_FAMILY_R600) {
		if (tiling & RADEON_TILING_MACRO)
			base_align = MAX(info->num_banks * info->num_channels * 8 * 8 * bpe,
					 pixel_align * bpe * height_align);
		else {
			if (info->have_tiling_info)
				base_align = info->group_bytes;
			else
				/* default to 512 if we don't know the real
				 * group size otherwise the kernel may reject the CS
				 * if the group sizes don't match as the base won't
				 * be aligned properly.
				 */
				base_align = 512;
		}
	}
	return base_align;
}

static Bool
drmmode_xf86crtc_resize (ScrnInfoPtr scrn, int width, int height)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	RADEONInfoPtr info = RADEONPTR(scrn);
	struct radeon_buffer *old_front = NULL;
	ScreenPtr   screen = xf86ScrnToScreen(scrn);
	int	    i, pitch, old_width, old_height, old_pitch;
	int usage = CREATE_PIXMAP_USAGE_BACKING_PIXMAP;
	int cpp = info->pixel_bytes;
	uint32_t tiling_flags;
	PixmapPtr ppix = screen->GetScreenPixmap(screen);
	void *fb_shadow;

	if (scrn->virtualX == width && scrn->virtualY == height)
		return TRUE;

	if (width > xf86_config->maxWidth || height > xf86_config->maxHeight) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "Xorg tried resizing screen to %dx%d, but maximum "
			   "supported is %dx%d\n", width, height,
			   xf86_config->maxWidth, xf86_config->maxHeight);
		return FALSE;
	}

	if (info->allowColorTiling && !info->shadow_primary) {
		if (info->ChipFamily < CHIP_FAMILY_R600 || info->allowColorTiling2D)
			usage |= RADEON_CREATE_PIXMAP_TILING_MACRO;
		else
			usage |= RADEON_CREATE_PIXMAP_TILING_MICRO;
	}

	xf86DrvMsg(scrn->scrnIndex, X_INFO, "Allocate new frame buffer %dx%d\n",
		   width, height);

	old_width = scrn->virtualX;
	old_height = scrn->virtualY;
	old_pitch = scrn->displayWidth;
	old_front = info->front_buffer;

	scrn->virtualX = width;
	scrn->virtualY = height;

	info->front_buffer = radeon_alloc_pixmap_bo(scrn, scrn->virtualX,
						    scrn->virtualY, scrn->depth,
						    usage, scrn->bitsPerPixel,
						    &pitch,
						    &info->front_surface,
						    &tiling_flags);
	if (!info->front_buffer)
		goto fail;

	scrn->displayWidth = pitch / cpp;

	if (!info->use_glamor) {
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
			radeon_bo_set_tiling(info->front_buffer->bo.radeon, tiling_flags, pitch);
	}

	if (!info->r600_shadow_fb) {
		if (info->surf_man && !info->use_glamor)
			*radeon_get_pixmap_surface(ppix) = info->front_surface;
		screen->ModifyPixmapHeader(ppix,
					   width, height, -1, -1, pitch, NULL);
	} else {
		if (radeon_bo_map(info->front_buffer->bo.radeon, 1))
			goto fail;
		fb_shadow = calloc(1, pitch * scrn->virtualY);
		if (!fb_shadow)
			goto fail;
		free(info->fb_shadow);
		info->fb_shadow = fb_shadow;
		screen->ModifyPixmapHeader(ppix,
					   width, height, -1, -1, pitch,
					   info->fb_shadow);
	}

	if (info->use_glamor)
		radeon_glamor_create_screen_resources(scrn->pScreen);

	if (!info->r600_shadow_fb) {
		if (!radeon_set_pixmap_bo(ppix, info->front_buffer))
			goto fail;
	}

	radeon_pixmap_clear(ppix);
	radeon_finish(scrn, info->front_buffer);

	for (i = 0; i < xf86_config->num_crtc; i++) {
		xf86CrtcPtr crtc = xf86_config->crtc[i];

		if (!crtc->enabled)
			continue;

		drmmode_set_mode_major(crtc, &crtc->mode,
				       crtc->rotation, crtc->x, crtc->y);
	}

	radeon_buffer_unref(&old_front);

	radeon_kms_update_vram_limit(scrn, pitch * scrn->virtualY);
	return TRUE;

 fail:
	radeon_buffer_unref(&info->front_buffer);
	info->front_buffer = old_front;
	scrn->virtualX = old_width;
	scrn->virtualY = old_height;
	scrn->displayWidth = old_pitch;

	return FALSE;
}

static void
drmmode_validate_leases(ScrnInfoPtr scrn)
{
#ifdef XF86_LEASE_VERSION
	ScreenPtr screen = scrn->pScreen;
	rrScrPrivPtr scr_priv = rrGetScrPriv(screen);
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
	drmModeLesseeListPtr lessees;
	RRLeasePtr lease, next;
	int l;

	/* We can't talk to the kernel about leases when VT switched */
	if (!scrn->vtSema)
		return;

	lessees = drmModeListLessees(pRADEONEnt->fd);
	if (!lessees)
		return;

	xorg_list_for_each_entry_safe(lease, next, &scr_priv->leases, list) {
		drmmode_lease_private_ptr lease_private = lease->devPrivate;

		for (l = 0; l < lessees->count; l++) {
			if (lessees->lessees[l] == lease_private->lessee_id)
				break;
		}

		/* check to see if the lease has gone away */
		if (l == lessees->count) {
			free(lease_private);
			lease->devPrivate = NULL;
			xf86CrtcLeaseTerminated(lease);
		}
	}

	free(lessees);
#endif
}

#ifdef XF86_LEASE_VERSION

static int
drmmode_create_lease(RRLeasePtr lease, int *fd)
{
	ScreenPtr screen = lease->screen;
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
	drmmode_lease_private_ptr lease_private;
	int noutput = lease->numOutputs;
	int ncrtc = lease->numCrtcs;
	uint32_t *objects;
	size_t nobjects;
	int lease_fd;
	int c, o;
	int i;

	nobjects = ncrtc + noutput;
	if (nobjects == 0 || nobjects > (SIZE_MAX / 4) ||
	    ncrtc > (SIZE_MAX - noutput))
		return BadValue;

	lease_private = calloc(1, sizeof (drmmode_lease_private_rec));
	if (!lease_private)
		return BadAlloc;

	objects = malloc(nobjects * 4);
	if (!objects) {
		free(lease_private);
		return BadAlloc;
	}

	i = 0;

	/* Add CRTC ids */
	for (c = 0; c < ncrtc; c++) {
		xf86CrtcPtr crtc = lease->crtcs[c]->devPrivate;
		drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

		objects[i++] = drmmode_crtc->mode_crtc->crtc_id;
	}

	/* Add connector ids */
	for (o = 0; o < noutput; o++) {
		xf86OutputPtr   output = lease->outputs[o]->devPrivate;
		drmmode_output_private_ptr drmmode_output = output->driver_private;

		objects[i++] = drmmode_output->mode_output->connector_id;
	}

	/* call kernel to create lease */
	assert (i == nobjects);

	lease_fd = drmModeCreateLease(pRADEONEnt->fd, objects, nobjects, 0,
				      &lease_private->lessee_id);

	free(objects);

	if (lease_fd < 0) {
		free(lease_private);
		return BadMatch;
	}

	lease->devPrivate = lease_private;

	xf86CrtcLeaseStarted(lease);

	*fd = lease_fd;
	return Success;
}

static void
drmmode_terminate_lease(RRLeasePtr lease)
{
	drmmode_lease_private_ptr lease_private = lease->devPrivate;
	ScreenPtr screen = lease->screen;
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);

	if (drmModeRevokeLease(pRADEONEnt->fd, lease_private->lessee_id) == 0) {
		free(lease_private);
		lease->devPrivate = NULL;
		xf86CrtcLeaseTerminated(lease);
	}
}

#endif // XF86_LEASE_VERSION

static const xf86CrtcConfigFuncsRec drmmode_xf86crtc_config_funcs = {
	.resize = drmmode_xf86crtc_resize,
#ifdef XF86_LEASE_VERSION
	.create_lease = drmmode_create_lease,
	.terminate_lease = drmmode_terminate_lease
#endif
};

static void
drmmode_flip_abort(xf86CrtcPtr crtc, void *event_data)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(crtc->scrn);
	drmmode_flipdata_ptr flipdata = event_data;
	int crtc_id = drmmode_get_crtc_id(crtc);
	struct drmmode_fb **fb = &flipdata->fb[crtc_id];

	if (drmmode_crtc->flip_pending == *fb) {
		drmmode_fb_reference(pRADEONEnt->fd, &drmmode_crtc->flip_pending,
				     NULL);
	}
	drmmode_fb_reference(pRADEONEnt->fd, fb, NULL);

	if (--flipdata->flip_count == 0) {
		if (!flipdata->fe_crtc)
			flipdata->fe_crtc = crtc;
		flipdata->abort(flipdata->fe_crtc, flipdata->event_data);
		free(flipdata);
	}
}

static void
drmmode_flip_handler(xf86CrtcPtr crtc, uint32_t frame, uint64_t usec, void *event_data)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(crtc->scrn);
	drmmode_flipdata_ptr flipdata = event_data;
	int crtc_id = drmmode_get_crtc_id(crtc);
	struct drmmode_fb **fb = &flipdata->fb[crtc_id];

	/* Is this the event whose info shall be delivered to higher level? */
	if (crtc == flipdata->fe_crtc) {
		/* Yes: Cache msc, ust for later delivery. */
		flipdata->fe_frame = frame;
		flipdata->fe_usec = usec;
	}

	if (*fb) {
		if (drmmode_crtc->flip_pending == *fb) {
			drmmode_fb_reference(pRADEONEnt->fd,
					     &drmmode_crtc->flip_pending, NULL);
		}
		drmmode_fb_reference(pRADEONEnt->fd, &drmmode_crtc->fb, *fb);
		drmmode_fb_reference(pRADEONEnt->fd, fb, NULL);
	}

	if (--flipdata->flip_count == 0) {
		/* Deliver MSC & UST from reference/current CRTC to flip event
		 * handler
		 */
		if (flipdata->fe_crtc)
			flipdata->handler(flipdata->fe_crtc, flipdata->fe_frame,
					  flipdata->fe_usec, flipdata->event_data);
		else
			flipdata->handler(crtc, frame, usec, flipdata->event_data);

		free(flipdata);
	}
}


#if HAVE_NOTIFY_FD
static void
drm_notify_fd(int fd, int ready, void *data)
#else
static void
drm_wakeup_handler(pointer data, int err, pointer p)
#endif
{
	drmmode_ptr drmmode = data;
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(drmmode->scrn);
	
#if !HAVE_NOTIFY_FD
	fd_set *read_mask = p;

	if (err >= 0 && FD_ISSET(pRADEONEnt->fd, read_mask))
#endif
	{
		radeon_drm_handle_event(pRADEONEnt->fd, &drmmode->event_context);
	}
}

static Bool drmmode_probe_page_flip_target(RADEONEntPtr pRADEONEnt)
{
#ifdef DRM_CAP_PAGE_FLIP_TARGET
	uint64_t cap_value;

	return drmGetCap(pRADEONEnt->fd, DRM_CAP_PAGE_FLIP_TARGET,
			 &cap_value) == 0 && cap_value != 0;
#else
	return FALSE;
#endif
}

static int
drmmode_page_flip(RADEONEntPtr pRADEONEnt,
		  drmmode_crtc_private_ptr drmmode_crtc, int fb_id,
		  uint32_t flags, uintptr_t drm_queue_seq)
{
	flags |= DRM_MODE_PAGE_FLIP_EVENT;
	return drmModePageFlip(pRADEONEnt->fd, drmmode_crtc->mode_crtc->crtc_id,
			       fb_id, flags, (void*)drm_queue_seq);
}

int
drmmode_page_flip_target_absolute(RADEONEntPtr pRADEONEnt,
				  drmmode_crtc_private_ptr drmmode_crtc,
				  int fb_id, uint32_t flags,
				  uintptr_t drm_queue_seq, uint32_t target_msc)
{
#ifdef DRM_MODE_PAGE_FLIP_TARGET
	if (pRADEONEnt->has_page_flip_target) {
		flags |= DRM_MODE_PAGE_FLIP_EVENT | DRM_MODE_PAGE_FLIP_TARGET_ABSOLUTE;
		return drmModePageFlipTarget(pRADEONEnt->fd,
					     drmmode_crtc->mode_crtc->crtc_id,
					     fb_id, flags, (void*)drm_queue_seq,
					     target_msc);
	}
#endif

	return drmmode_page_flip(pRADEONEnt, drmmode_crtc, fb_id, flags,
				 drm_queue_seq);
}

int
drmmode_page_flip_target_relative(RADEONEntPtr pRADEONEnt,
				  drmmode_crtc_private_ptr drmmode_crtc,
				  int fb_id, uint32_t flags,
				  uintptr_t drm_queue_seq, uint32_t target_msc)
{
#ifdef DRM_MODE_PAGE_FLIP_TARGET
	if (pRADEONEnt->has_page_flip_target) {
		flags |= DRM_MODE_PAGE_FLIP_EVENT | DRM_MODE_PAGE_FLIP_TARGET_RELATIVE;
		return drmModePageFlipTarget(pRADEONEnt->fd,
					     drmmode_crtc->mode_crtc->crtc_id,
					     fb_id, flags, (void*)drm_queue_seq,
					     target_msc);
	}
#endif

	return drmmode_page_flip(pRADEONEnt, drmmode_crtc, fb_id, flags,
				 drm_queue_seq);
}

Bool drmmode_pre_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode, int cpp)
{
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	RADEONInfoPtr info = RADEONPTR(pScrn);
	int i, num_dvi = 0, num_hdmi = 0;
	drmModeResPtr mode_res;
	unsigned int crtcs_needed = 0;
	unsigned int crtcs_got = 0;
	char *bus_id_string, *provider_name;

	xf86CrtcConfigInit(pScrn, &drmmode_xf86crtc_config_funcs);

	drmmode->scrn = pScrn;
	mode_res = drmModeGetResources(pRADEONEnt->fd);
	if (!mode_res)
		return FALSE;

	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Initializing outputs ...\n");
	for (i = 0; i < mode_res->count_connectors; i++)
		crtcs_needed += drmmode_output_init(pScrn, drmmode, mode_res,
						    i, &num_dvi, &num_hdmi, 0);
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "%d crtcs needed for screen.\n", crtcs_needed);

	/* Need per-screen drmmode_crtc_funcs, based on our global template,
	 * so we can disable some functions, depending on screen settings.
	 */
	info->drmmode_crtc_funcs = drmmode_crtc_funcs;

	if (info->r600_shadow_fb) {
		/* Rotation requires hardware acceleration */
		info->drmmode_crtc_funcs.shadow_allocate = NULL;
		info->drmmode_crtc_funcs.shadow_create = NULL;
		info->drmmode_crtc_funcs.shadow_destroy = NULL;
	}

	/* Hw gamma lut's are currently bypassed by the hw at color depth 30,
	 * so spare the server the effort to compute and update the cluts.
	 */
	if (pScrn->depth == 30)
		info->drmmode_crtc_funcs.gamma_set = NULL;

	drmmode->count_crtcs = mode_res->count_crtcs;
	xf86CrtcSetSizeRange(pScrn, 320, 200, mode_res->max_width, mode_res->max_height);

	for (i = 0; i < mode_res->count_crtcs; i++) {
		if (!xf86IsEntityShared(pScrn->entityList[0]) ||
		    (crtcs_got < crtcs_needed &&
		     !(pRADEONEnt->assigned_crtcs & (1 << i))))
			crtcs_got += drmmode_crtc_init(pScrn, drmmode, mode_res, i);
	}

	/* All ZaphodHeads outputs provided with matching crtcs? */
	if (crtcs_got < crtcs_needed) {
		if (crtcs_got == 0) {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
				   "No ZaphodHeads CRTC available, needed %u\n",
				   crtcs_needed);
			return FALSE;
		}

		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "%d ZaphodHeads crtcs unavailable. Some outputs will stay off.\n",
			   crtcs_needed);
	}

	/* workout clones */
	drmmode_clones_init(pScrn, drmmode, mode_res);

	bus_id_string = DRICreatePCIBusID(info->PciInfo);
	XNFasprintf(&provider_name, "%s @ %s", pScrn->chipset, bus_id_string);
	free(bus_id_string);
	xf86ProviderSetup(pScrn, NULL, provider_name);
	free(provider_name);

	xf86InitialConfiguration(pScrn, TRUE);

	pRADEONEnt->has_page_flip_target = drmmode_probe_page_flip_target(pRADEONEnt);

	drmModeFreeResources(mode_res);
	return TRUE;
}

void drmmode_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode)
{
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	RADEONInfoPtr info = RADEONPTR(pScrn);

	if (info->dri2.pKernelDRMVersion->version_minor < 4)
		return;

	info->drmmode_inited = TRUE;
	if (pRADEONEnt->fd_wakeup_registered != serverGeneration) {
#if HAVE_NOTIFY_FD
		SetNotifyFd(pRADEONEnt->fd, drm_notify_fd, X_NOTIFY_READ,
			    &info->drmmode);
#else
		AddGeneralSocket(pRADEONEnt->fd);
		RegisterBlockAndWakeupHandlers((BlockHandlerProcPtr)NoopDDA,
					       drm_wakeup_handler,
					       &info->drmmode);
#endif
		pRADEONEnt->fd_wakeup_registered = serverGeneration;
		pRADEONEnt->fd_wakeup_ref = 1;
	} else
		pRADEONEnt->fd_wakeup_ref++;
}

void drmmode_fini(ScrnInfoPtr pScrn, drmmode_ptr drmmode)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	RADEONInfoPtr info = RADEONPTR(pScrn);
	int c;

	if (info->dri2.pKernelDRMVersion->version_minor < 4 || !info->drmmode_inited)
		return;

	for (c = 0; c < config->num_crtc; c++)
		drmmode_crtc_scanout_free(config->crtc[c]);

	if (pRADEONEnt->fd_wakeup_registered == serverGeneration &&
	    !--pRADEONEnt->fd_wakeup_ref) {
#if HAVE_NOTIFY_FD
		RemoveNotifyFd(pRADEONEnt->fd);
#else
		RemoveGeneralSocket(pRADEONEnt->fd);
		RemoveBlockAndWakeupHandlers((BlockHandlerProcPtr)NoopDDA,
				drm_wakeup_handler, pScrn);
#endif
	}
}


Bool drmmode_set_bufmgr(ScrnInfoPtr pScrn, drmmode_ptr drmmode, struct radeon_bo_manager *bufmgr)
{
	drmmode->bufmgr = bufmgr;
	return TRUE;
}


static void drmmode_sprite_do_set_cursor(struct radeon_device_priv *device_priv,
					 ScrnInfoPtr scrn, int x, int y)
{
	RADEONInfoPtr info = RADEONPTR(scrn);
	CursorPtr cursor = device_priv->cursor;
	Bool sprite_visible = device_priv->sprite_visible;

	if (cursor) {
		x -= cursor->bits->xhot;
		y -= cursor->bits->yhot;

		device_priv->sprite_visible =
			x < scrn->virtualX && y < scrn->virtualY &&
			(x + cursor->bits->width > 0) &&
			(y + cursor->bits->height > 0);
	} else {
		device_priv->sprite_visible = FALSE;
	}

	info->sprites_visible += device_priv->sprite_visible - sprite_visible;
}

static void drmmode_sprite_set_cursor(DeviceIntPtr pDev, ScreenPtr pScreen,
				      CursorPtr pCursor, int x, int y)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pScreen);
	RADEONInfoPtr info = RADEONPTR(scrn);
	struct radeon_device_priv *device_priv =
		dixLookupScreenPrivate(&pDev->devPrivates,
				       &radeon_device_private_key, pScreen);

	device_priv->cursor = pCursor;
	drmmode_sprite_do_set_cursor(device_priv, scrn, x, y);

	info->SpriteFuncs->SetCursor(pDev, pScreen, pCursor, x, y);
}

static void drmmode_sprite_move_cursor(DeviceIntPtr pDev, ScreenPtr pScreen,
				       int x, int y)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pScreen);
	RADEONInfoPtr info = RADEONPTR(scrn);
	struct radeon_device_priv *device_priv =
		dixLookupScreenPrivate(&pDev->devPrivates,
				       &radeon_device_private_key, pScreen);

	drmmode_sprite_do_set_cursor(device_priv, scrn, x, y);

	info->SpriteFuncs->MoveCursor(pDev, pScreen, x, y);
}

static Bool drmmode_sprite_realize_realize_cursor(DeviceIntPtr pDev,
						  ScreenPtr pScreen,
						  CursorPtr pCursor)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pScreen);
	RADEONInfoPtr info = RADEONPTR(scrn);

	return info->SpriteFuncs->RealizeCursor(pDev, pScreen, pCursor);
}

static Bool drmmode_sprite_realize_unrealize_cursor(DeviceIntPtr pDev,
						    ScreenPtr pScreen,
						    CursorPtr pCursor)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pScreen);
	RADEONInfoPtr info = RADEONPTR(scrn);

	return info->SpriteFuncs->UnrealizeCursor(pDev, pScreen, pCursor);
}

static Bool drmmode_sprite_device_cursor_initialize(DeviceIntPtr pDev,
						    ScreenPtr pScreen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pScreen);
	RADEONInfoPtr info = RADEONPTR(scrn);

	return info->SpriteFuncs->DeviceCursorInitialize(pDev, pScreen);
}

static void drmmode_sprite_device_cursor_cleanup(DeviceIntPtr pDev,
						 ScreenPtr pScreen)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(pScreen);
	RADEONInfoPtr info = RADEONPTR(scrn);

	info->SpriteFuncs->DeviceCursorCleanup(pDev, pScreen);
}

miPointerSpriteFuncRec drmmode_sprite_funcs = {
	.RealizeCursor = drmmode_sprite_realize_realize_cursor,
	.UnrealizeCursor = drmmode_sprite_realize_unrealize_cursor,
	.SetCursor = drmmode_sprite_set_cursor,
	.MoveCursor = drmmode_sprite_move_cursor,
	.DeviceCursorInitialize = drmmode_sprite_device_cursor_initialize,
	.DeviceCursorCleanup = drmmode_sprite_device_cursor_cleanup,
};

	
void drmmode_adjust_frame(ScrnInfoPtr pScrn, drmmode_ptr drmmode, int x, int y)
{
	xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR(pScrn);
	xf86OutputPtr  output = config->output[config->compat_output];
	xf86CrtcPtr	crtc = output->crtc;

	if (crtc && crtc->enabled) {
		drmmode_set_mode_major(crtc, &crtc->mode, crtc->rotation,
				       x, y);
	}
}

Bool drmmode_set_desired_modes(ScrnInfoPtr pScrn, drmmode_ptr drmmode,
			       Bool set_hw)
{
	xf86CrtcConfigPtr   config = XF86_CRTC_CONFIG_PTR(pScrn);
	unsigned num_desired = 0, num_on = 0;
	int c;

	/* First, disable all unused CRTCs */
	if (set_hw) {
		for (c = 0; c < config->num_crtc; c++) {
			xf86CrtcPtr crtc = config->crtc[c];

			/* Skip disabled CRTCs */
			if (crtc->enabled)
				continue;

			drmmode_crtc_dpms(crtc, DPMSModeOff);
		}
	}

	/* Then, try setting the chosen mode on each CRTC */
	for (c = 0; c < config->num_crtc; c++) {
		xf86CrtcPtr	crtc = config->crtc[c];
		xf86OutputPtr	output = NULL;
		int		o;

		if (!crtc->enabled)
			continue;

		if (config->output[config->compat_output]->crtc == crtc)
			output = config->output[config->compat_output];
		else
		{
			for (o = 0; o < config->num_output; o++)
				if (config->output[o]->crtc == crtc)
				{
					output = config->output[o];
					break;
				}
		}
		/* paranoia */
		if (!output)
			continue;

		num_desired++;

		/* Mark that we'll need to re-set the mode for sure */
		memset(&crtc->mode, 0, sizeof(crtc->mode));
		if (!crtc->desiredMode.CrtcHDisplay)
		{
			DisplayModePtr  mode = xf86OutputFindClosestMode (output, pScrn->currentMode);

			if (!mode) {
				xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
					   "Failed to find mode for CRTC %d\n", c);
				continue;
			}
			crtc->desiredMode = *mode;
			crtc->desiredRotation = RR_Rotate_0;
			crtc->desiredX = 0;
			crtc->desiredY = 0;
		}

		if (set_hw) {
			if (crtc->funcs->set_mode_major(crtc, &crtc->desiredMode,
							crtc->desiredRotation,
							crtc->desiredX,
							crtc->desiredY)) {
				num_on++;
			} else {
				xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
					   "Failed to set mode on CRTC %d\n", c);
				RRCrtcSet(crtc->randr_crtc, NULL, crtc->x, crtc->y,
					  crtc->rotation, 0, NULL);
			}
		} else {
			crtc->mode = crtc->desiredMode;
			crtc->rotation = crtc->desiredRotation;
			crtc->x = crtc->desiredX;
			crtc->y = crtc->desiredY;
			if (drmmode_handle_transform(crtc))
				num_on++;
		}
	}

	if (num_on == 0 && num_desired > 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to enable any CRTC\n");
		return FALSE;
	}

	/* Validate leases on VT re-entry */
	if (dixPrivateKeyRegistered(rrPrivKey))
	    drmmode_validate_leases(pScrn);

	return TRUE;
}

Bool drmmode_setup_colormap(ScreenPtr pScreen, ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    if (xf86_config->num_crtc) {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Initializing kms color map\n");
	if (!miCreateDefColormap(pScreen))
	    return FALSE;

	/* All radeons support 10 bit CLUTs. They get bypassed at depth 30. */
	if (pScrn->depth != 30) {
	    if (!xf86HandleColormaps(pScreen, 256, 10, NULL, NULL,
				     CMAP_PALETTED_TRUECOLOR
				     | CMAP_RELOAD_ON_MODE_SWITCH))
		return FALSE;

	    for (i = 0; i < xf86_config->num_crtc; i++) {
		xf86CrtcPtr crtc = xf86_config->crtc[i];

		drmmode_crtc_gamma_do_set(crtc, crtc->gamma_red,
					  crtc->gamma_green,
					  crtc->gamma_blue,
					  crtc->gamma_size);
	    }
	}
    }

    return TRUE;
}

static Bool
drmmode_find_output(ScrnInfoPtr scrn, int output_id, int *num_dvi,
		    int *num_hdmi)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	int i;

	for (i = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];
		drmmode_output_private_ptr drmmode_output = output->driver_private;

		if (drmmode_output->output_id == output_id) {
			switch(drmmode_output->mode_output->connector_type) {
			case DRM_MODE_CONNECTOR_DVII:
			case DRM_MODE_CONNECTOR_DVID:
			case DRM_MODE_CONNECTOR_DVIA:
				(*num_dvi)++;
				break;
			case DRM_MODE_CONNECTOR_HDMIA:
			case DRM_MODE_CONNECTOR_HDMIB:
				(*num_hdmi)++;
				break;
			}

			return TRUE;
		}
	}

	return FALSE;
}

void
radeon_mode_hotplug(ScrnInfoPtr scrn, drmmode_ptr drmmode)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
	drmModeResPtr mode_res;
	int i, j;
	Bool found;
	Bool changed = FALSE;
	int num_dvi = 0, num_hdmi = 0;

	/* Try to re-set the mode on all the connectors with a BAD link-state:
	 * This may happen if a link degrades and a new modeset is necessary, using
	 * different link-training parameters. If the kernel found that the current
	 * mode is not achievable anymore, it should have pruned the mode before
	 * sending the hotplug event. Try to re-set the currently-set mode to keep
	 * the display alive, this will fail if the mode has been pruned.
	 * In any case, we will send randr events for the Desktop Environment to
	 * deal with it, if it wants to.
	 */
	for (i = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];
		xf86CrtcPtr crtc = output->crtc;
		drmmode_output_private_ptr drmmode_output = output->driver_private;

		drmmode_output_detect(output);

		if (!crtc || !drmmode_output->mode_output)
			continue;

		/* Get an updated view of the properties for the current connector and
		 * look for the link-status property
		 */
		for (j = 0; j < drmmode_output->num_props; j++) {
			drmmode_prop_ptr p = &drmmode_output->props[j];

			if (!strcmp(p->mode_prop->name, "link-status")) {
				if (p->value != DRM_MODE_LINK_STATUS_BAD)
					break;

				/* the connector got a link failure, re-set the current mode */
				drmmode_set_mode_major(crtc, &crtc->mode, crtc->rotation,
						       crtc->x, crtc->y);

				xf86DrvMsg(scrn->scrnIndex, X_WARNING,
					   "hotplug event: connector %u's link-state is BAD, "
					   "tried resetting the current mode. You may be left"
					   "with a black screen if this fails...\n",
					   drmmode_output->mode_output->connector_id);

				break;
			}
		}
	}

	mode_res = drmModeGetResources(pRADEONEnt->fd);
	if (!mode_res)
		goto out;

restart_destroy:
	for (i = 0; i < config->num_output; i++) {
		xf86OutputPtr output = config->output[i];
		drmmode_output_private_ptr drmmode_output = output->driver_private;
		found = FALSE;
		for (j = 0; j < mode_res->count_connectors; j++) {
			if (mode_res->connectors[j] == drmmode_output->output_id) {
				found = TRUE;
				break;
			}
		}
		if (found)
			continue;

		drmModeFreeConnector(drmmode_output->mode_output);
		drmmode_output->mode_output = NULL;
		drmmode_output->output_id = -1;

		changed = TRUE;
		if (drmmode->delete_dp_12_displays) {
			RROutputDestroy(output->randr_output);
			xf86OutputDestroy(output);
			goto restart_destroy;
		}
	}

	/* find new output ids we don't have outputs for */
	for (i = 0; i < mode_res->count_connectors; i++) {
		for (j = 0; j < pRADEONEnt->num_scrns; j++) {
			if (drmmode_find_output(pRADEONEnt->scrn[j],
						mode_res->connectors[i],
						&num_dvi, &num_hdmi))
				break;
		}

		if (j < pRADEONEnt->num_scrns)
			continue;

		if (drmmode_output_init(scrn, drmmode, mode_res, i, &num_dvi,
					&num_hdmi, 1) != 0)
			changed = TRUE;
	}

	/* Check to see if a lessee has disappeared */
	drmmode_validate_leases(scrn);

	if (changed) {
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,14,99,2,0)
		RRSetChanged(xf86ScrnToScreen(scrn));
#else
		rrScrPrivPtr rrScrPriv = rrGetScrPriv(scrn->pScreen);
		rrScrPriv->changed = TRUE;
#endif
		RRTellChanged(xf86ScrnToScreen(scrn));
	}

	drmModeFreeResources(mode_res);
out:
	RRGetInfo(xf86ScrnToScreen(scrn), TRUE);
}
#ifdef HAVE_LIBUDEV
static void
drmmode_handle_uevents(int fd, void *closure)
{
	drmmode_ptr drmmode = closure;
	ScrnInfoPtr scrn = drmmode->scrn;
	struct udev_device *dev;
	Bool received = FALSE;
	struct timeval tv = { 0, 0 };
	fd_set readfd;

	FD_ZERO(&readfd);
	FD_SET(fd, &readfd);

	while (select(fd + 1, &readfd, NULL, NULL, &tv) > 0 &&
	       FD_ISSET(fd, &readfd)) {
		/* select() ensured that this will not block */
		dev = udev_monitor_receive_device(drmmode->uevent_monitor);
		if (dev) {
			udev_device_unref(dev);
			received = TRUE;
		}
	}

	if (received)
		radeon_mode_hotplug(scrn, drmmode);
}
#endif

void drmmode_uevent_init(ScrnInfoPtr scrn, drmmode_ptr drmmode)
{
#ifdef HAVE_LIBUDEV
	struct udev *u;
	struct udev_monitor *mon;

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
	    udev_monitor_enable_receiving(mon) < 0) {
		udev_monitor_unref(mon);
		udev_unref(u);
		return;
	}

	drmmode->uevent_handler =
		xf86AddGeneralHandler(udev_monitor_get_fd(mon),
				      drmmode_handle_uevents,
				      drmmode);

	drmmode->uevent_monitor = mon;
#endif
}

void drmmode_uevent_fini(ScrnInfoPtr scrn, drmmode_ptr drmmode)
{
#ifdef HAVE_LIBUDEV
	if (drmmode->uevent_handler) {
		struct udev *u = udev_monitor_get_udev(drmmode->uevent_monitor);
		xf86RemoveGeneralHandler(drmmode->uevent_handler);

		udev_monitor_unref(drmmode->uevent_monitor);
		udev_unref(u);
	}
#endif
}

Bool radeon_do_pageflip(ScrnInfoPtr scrn, ClientPtr client,
			PixmapPtr new_front, uint64_t id, void *data,
			xf86CrtcPtr ref_crtc, radeon_drm_handler_proc handler,
			radeon_drm_abort_proc abort,
			enum drmmode_flip_sync flip_sync,
			uint32_t target_msc)
{
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(scrn);
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	xf86CrtcPtr crtc = NULL;
	drmmode_crtc_private_ptr drmmode_crtc = config->crtc[0]->driver_private;
	int crtc_id;
	uint32_t flip_flags = flip_sync == FLIP_ASYNC ? DRM_MODE_PAGE_FLIP_ASYNC : 0;
	drmmode_flipdata_ptr flipdata;
	Bool handle_deferred = FALSE;
	uintptr_t drm_queue_seq = 0;
	struct drmmode_fb *fb;
	int i = 0;

	flipdata = calloc(1, sizeof(*flipdata) + drmmode_crtc->drmmode->count_crtcs *
			  sizeof(flipdata->fb[0]));
        if (!flipdata) {
             xf86DrvMsg(scrn->scrnIndex, X_WARNING,
                        "flip queue: data alloc failed.\n");
             goto error;
        }

	fb = radeon_pixmap_get_fb(new_front);
	if (!fb) {
		ErrorF("Failed to get FB for flip\n");
		goto error;
	}

	/*
	 * Queue flips on all enabled CRTCs
	 * Note that if/when we get per-CRTC buffers, we'll have to update this.
	 * Right now it assumes a single shared fb across all CRTCs, with the
	 * kernel fixing up the offset of each CRTC as necessary.
	 *
	 * Also, flips queued on disabled or incorrectly configured displays
	 * may never complete; this is a configuration error.
	 */

        flipdata->event_data = data;
        flipdata->handler = handler;
        flipdata->abort = abort;
        flipdata->fe_crtc = ref_crtc;

	for (i = 0; i < config->num_crtc; i++) {
		crtc = config->crtc[i];
		drmmode_crtc = crtc->driver_private;
		crtc_id = drmmode_get_crtc_id(crtc);

		if (!drmmode_crtc_can_flip(crtc) ||
		    (drmmode_crtc->tear_free && crtc != ref_crtc))
			continue;

		flipdata->flip_count++;

		drm_queue_seq = radeon_drm_queue_alloc(crtc, client, id,
						       flipdata,
						       drmmode_flip_handler,
						       drmmode_flip_abort,
						       TRUE);
		if (drm_queue_seq == RADEON_DRM_QUEUE_ERROR) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "Allocating DRM queue event entry failed.\n");
			goto error;
		}

		if (drmmode_crtc->tear_free) {
			BoxRec extents = { .x1 = 0, .y1 = 0,
					   .x2 = new_front->drawable.width,
					   .y2 = new_front->drawable.height };
			int scanout_id = drmmode_crtc->scanout_id ^ 1;

			if (flip_sync == FLIP_ASYNC) {
				if (!drmmode_wait_vblank(crtc,
							 DRM_VBLANK_RELATIVE |
							 DRM_VBLANK_EVENT,
							 0, drm_queue_seq,
							 NULL, NULL))
					goto flip_error;
				goto next;
			}

			drmmode_fb_reference(pRADEONEnt->fd, &flipdata->fb[crtc_id],
					     radeon_pixmap_get_fb(drmmode_crtc->scanout[scanout_id].pixmap));
			if (!flipdata->fb[crtc_id]) {
				ErrorF("Failed to get FB for TearFree flip\n");
				goto error;
			}

			radeon_scanout_do_update(crtc, scanout_id, new_front,
						 extents);
			radeon_cs_flush_indirect(crtc->scrn);

			if (drmmode_crtc->scanout_update_pending) {
				radeon_drm_wait_pending_flip(crtc);
				handle_deferred = TRUE;
				radeon_drm_abort_entry(drmmode_crtc->scanout_update_pending);
				drmmode_crtc->scanout_update_pending = 0;
			}
		} else {
			drmmode_fb_reference(pRADEONEnt->fd, &flipdata->fb[crtc_id], fb);
		}

		if (crtc == ref_crtc) {
			if (drmmode_page_flip_target_absolute(pRADEONEnt,
							      drmmode_crtc,
							      flipdata->fb[crtc_id]->handle,
							      flip_flags,
							      drm_queue_seq,
							      target_msc) != 0)
				goto flip_error;
		} else {
			if (drmmode_page_flip_target_relative(pRADEONEnt,
							      drmmode_crtc,
							      flipdata->fb[crtc_id]->handle,
							      flip_flags,
							      drm_queue_seq, 0) != 0)
				goto flip_error;
		}

		if (drmmode_crtc->tear_free) {
			drmmode_crtc->scanout_id ^= 1;
			drmmode_crtc->ignore_damage = TRUE;
		}

		drmmode_fb_reference(pRADEONEnt->fd, &drmmode_crtc->flip_pending,
				     flipdata->fb[crtc_id]);

	next:
		drm_queue_seq = 0;
	}

	if (handle_deferred)
		radeon_drm_queue_handle_deferred(ref_crtc);
	if (flipdata->flip_count > 0)
		return TRUE;

flip_error:
	xf86DrvMsg(scrn->scrnIndex, X_WARNING, "flip queue failed: %s\n",
		   strerror(errno));

error:
	if (drm_queue_seq)
		radeon_drm_abort_entry(drm_queue_seq);
	else if (crtc)
		drmmode_flip_abort(crtc, flipdata);
	else {
		abort(NULL, data);
		free(flipdata);
	}

	xf86DrvMsg(scrn->scrnIndex, X_WARNING, "Page flip failed: %s\n",
		   strerror(errno));
	if (handle_deferred)
		radeon_drm_queue_handle_deferred(ref_crtc);
	return FALSE;
}
