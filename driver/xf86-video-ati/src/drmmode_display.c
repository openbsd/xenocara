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
#include "micmap.h"
#include "xf86cmap.h"
#include "radeon.h"
#include "radeon_glamor.h"
#include "radeon_reg.h"

#include "drmmode_display.h"

/* DPMS */
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#define DEFAULT_NOMINAL_FRAME_RATE 60

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
					  int pitch, int tiling,
					  struct radeon_bo *bo, struct radeon_surface *psurf)
{
	RADEONInfoPtr info = RADEONPTR(pScrn);
	ScreenPtr pScreen = pScrn->pScreen;
	PixmapPtr pixmap;
	struct radeon_surface *surface;

	pixmap = (*pScreen->CreatePixmap)(pScreen, 0, 0, depth,
					  RADEON_CREATE_PIXMAP_SCANOUT);
	if (!pixmap)
		return NULL;

	if (!(*pScreen->ModifyPixmapHeader)(pixmap, width, height,
					    depth, bpp, pitch, NULL)) {
		return NULL;
	}

	if (!info->use_glamor)
		exaMoveInPixmap(pixmap);
	radeon_set_pixmap_bo(pixmap, bo);
	if (info->ChipFamily >= CHIP_FAMILY_R600) {
		surface = radeon_get_pixmap_surface(pixmap);
		if (surface && psurf) 
			*surface = *psurf;
		else if (surface) {
			memset(surface, 0, sizeof(struct radeon_surface));
			surface->npix_x = width;
			surface->npix_y = height;
			surface->npix_z = 1;
			surface->blk_w = 1;
			surface->blk_h = 1;
			surface->blk_d = 1;
			surface->array_size = 1;
			surface->last_level = 0;
			surface->bpe = bpp / 8;
			surface->nsamples = 1;
			surface->flags = RADEON_SURF_SCANOUT;
			/* we are requiring a recent enough libdrm version */
			surface->flags |= RADEON_SURF_HAS_TILE_MODE_INDEX;
			surface->flags |= RADEON_SURF_SET(RADEON_SURF_TYPE_2D, TYPE);
			surface->flags |= RADEON_SURF_SET(RADEON_SURF_MODE_LINEAR_ALIGNED, MODE);
			if (tiling & RADEON_TILING_MICRO) {
				surface->flags = RADEON_SURF_CLR(surface->flags, MODE);
				surface->flags |= RADEON_SURF_SET(RADEON_SURF_MODE_1D, MODE);
			}
			if (tiling & RADEON_TILING_MACRO) {
				surface->flags = RADEON_SURF_CLR(surface->flags, MODE);
				surface->flags |= RADEON_SURF_SET(RADEON_SURF_MODE_2D, MODE);
			}
			if (radeon_surface_best(info->surf_man, surface)) {
				return NULL;
			}
			if (radeon_surface_init(info->surf_man, surface)) {
				return NULL;
			}
		}
	}

	if (info->use_glamor &&
	    !radeon_glamor_create_textured_pixmap(pixmap,
						  radeon_get_pixmap_private(pixmap))) {
		pScreen->DestroyPixmap(pixmap);
	  	return NULL;
	}

	return pixmap;
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
    RADEONInfoPtr info = RADEONPTR(scrn);
    drmVBlank vbl;
    int ret;

    vbl.request.type = DRM_VBLANK_RELATIVE;
    vbl.request.type |= radeon_populate_vbl_request_type(crtc);
    vbl.request.sequence = 0;

    ret = drmWaitVBlank(info->dri2.drm_fd, &vbl);
    if (ret) {
	xf86DrvMsg(scrn->scrnIndex, X_WARNING,
		   "get vblank counter failed: %s\n", strerror(errno));
	return ret;
    }

    *ust = ((CARD64)vbl.reply.tval_sec * 1000000) + vbl.reply.tval_usec;
    *msc = vbl.reply.sequence;

    return Success;
}

static void
drmmode_do_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	ScrnInfoPtr scrn = crtc->scrn;
	RADEONInfoPtr info = RADEONPTR(scrn);
	CARD64 ust;
	int ret;

	if (drmmode_crtc->dpms_mode == DPMSModeOn && mode != DPMSModeOn) {
		drmVBlank vbl;

		/*
		 * On->Off transition: record the last vblank time,
		 * sequence number and frame period.
		 */
		vbl.request.type = DRM_VBLANK_RELATIVE;
		vbl.request.type |= radeon_populate_vbl_request_type(crtc);
		vbl.request.sequence = 0;
		ret = drmWaitVBlank(info->dri2.drm_fd, &vbl);
		if (ret)
			xf86DrvMsg(scrn->scrnIndex, X_ERROR,
				   "%s cannot get last vblank counter\n",
				   __func__);
		else {
			CARD64 seq = (CARD64)vbl.reply.sequence;
			CARD64 nominal_frame_rate, pix_in_frame;

			ust = ((CARD64)vbl.reply.tval_sec * 1000000) +
				vbl.reply.tval_usec;
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
	} else if (drmmode_crtc->dpms_mode != DPMSModeOn && mode == DPMSModeOn) {
		/*
		 * Off->On transition: calculate and accumulate the
		 * number of interpolated vblanks while we were in Off state
		 */
		ret = drmmode_get_current_ust(info->dri2.drm_fd, &ust);
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
	}
	drmmode_crtc->dpms_mode = mode;
}

static void
drmmode_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;

	/* Disable unused CRTCs */
	if (!crtc->enabled || mode != DPMSModeOn)
		drmModeSetCrtc(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id,
			       0, 0, 0, NULL, 0, NULL);
	else if (drmmode_crtc->dpms_mode != DPMSModeOn)
		crtc->funcs->set_mode_major(crtc, &crtc->mode, crtc->rotation,
					    crtc->x, crtc->y);
}

static PixmapPtr
create_pixmap_for_fbcon(drmmode_ptr drmmode,
			ScrnInfoPtr pScrn, int fbcon_id)
{
	RADEONInfoPtr info = RADEONPTR(pScrn);
	PixmapPtr pixmap = info->fbcon_pixmap;
	struct radeon_bo *bo;
	drmModeFBPtr fbcon;
	struct drm_gem_flink flink;

	if (pixmap)
	    return pixmap;

	fbcon = drmModeGetFB(drmmode->fd, fbcon_id);
	if (fbcon == NULL)
		return NULL;

	if (fbcon->depth != pScrn->depth ||
	    fbcon->width != pScrn->virtualX ||
	    fbcon->height != pScrn->virtualY)
		goto out_free_fb;

	flink.handle = fbcon->handle;
	if (ioctl(drmmode->fd, DRM_IOCTL_GEM_FLINK, &flink) < 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Couldn't flink fbcon handle\n");
		goto out_free_fb;
	}

	bo = radeon_bo_open(drmmode->bufmgr, flink.name, 0, 0, 0, 0);
	if (bo == NULL) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Couldn't allocate bo for fbcon handle\n");
		goto out_free_fb;
	}

	pixmap = drmmode_create_bo_pixmap(pScrn, fbcon->width, fbcon->height,
					  fbcon->depth, fbcon->bpp,
					  fbcon->pitch, 0, bo, NULL);
	info->fbcon_pixmap = pixmap;
	radeon_bo_unref(bo);
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
	 */
	if (info->use_glamor && info->ChipFamily >= CHIP_FAMILY_CAYMAN)
		return;

	if (info->fbcon_pixmap)
		pScrn->pScreen->DestroyPixmap(info->fbcon_pixmap);
	info->fbcon_pixmap = NULL;
}

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 10

void drmmode_copy_fb(ScrnInfoPtr pScrn, drmmode_ptr drmmode)
{
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	RADEONInfoPtr info = RADEONPTR(pScrn);
	PixmapPtr src, dst;
	ScreenPtr pScreen = pScrn->pScreen;
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

	if (fbcon_id == drmmode->fb_id) {
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

	dst = pScreen->GetScreenPixmap(pScreen);

	gc = GetScratchGC(pScrn->depth, pScreen);
	ValidateGC(&dst->drawable, gc);

	force = info->accel_state->force;
	info->accel_state->force = TRUE;
	(*gc->ops->CopyArea)(&src->drawable, &dst->drawable, gc, 0, 0,
			     pScrn->virtualX, pScrn->virtualY, 0, 0);
	info->accel_state->force = force;

	FreeScratchGC(gc);

	radeon_cs_flush_indirect(pScrn);
	radeon_bo_wait(info->front_bo);

	pScreen->canDoBGNoneRoot = TRUE;
	destroy_pixmap_for_fbcon(pScrn);
	return;
}

#endif /* GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 10 */

static void
drmmode_crtc_scanout_destroy(drmmode_ptr drmmode,
			     struct drmmode_scanout *scanout)
{
	if (scanout->pixmap) {
		drmmode_destroy_bo_pixmap(scanout->pixmap);
		scanout->pixmap = NULL;
	}

	if (scanout->bo) {
		drmModeRmFB(drmmode->fd, scanout->fb_id);
		scanout->fb_id = 0;
		radeon_bo_unmap(scanout->bo);
		radeon_bo_unref(scanout->bo);
		scanout->bo = NULL;
	}

	if (scanout->damage) {
		DamageDestroy(scanout->damage);
		scanout->damage = NULL;
	}
}

void
drmmode_scanout_free(ScrnInfoPtr scrn)
{
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	int c;

	for (c = 0; c < xf86_config->num_crtc; c++) {
		drmmode_crtc_private_ptr drmmode_crtc =
			xf86_config->crtc[c]->driver_private;

		drmmode_crtc_scanout_destroy(drmmode_crtc->drmmode,
					     &drmmode_crtc->scanout[0]);
		drmmode_crtc_scanout_destroy(drmmode_crtc->drmmode,
					     &drmmode_crtc->scanout[1]);
	}
}

static void *
drmmode_crtc_scanout_allocate(xf86CrtcPtr crtc,
			      struct drmmode_scanout *scanout,
			      int width, int height)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;
	int aligned_height;
	int size;
	int ret;
	unsigned long rotate_pitch;
	int base_align;

	if (scanout->bo) {
		if (scanout->width == width && scanout->height == height)
			return scanout->bo->ptr;

		drmmode_crtc_scanout_destroy(drmmode, scanout);
	}

	rotate_pitch =
		RADEON_ALIGN(width, drmmode_get_pitch_align(pScrn, drmmode->cpp, 0))
		* drmmode->cpp;
	aligned_height = RADEON_ALIGN(height, drmmode_get_height_align(pScrn, 0));
	base_align = drmmode_get_base_align(pScrn, drmmode->cpp, 0);
	size = RADEON_ALIGN(rotate_pitch * aligned_height, RADEON_GPU_PAGE_SIZE);

	scanout->bo = radeon_bo_open(drmmode->bufmgr, 0, size, base_align,
				     RADEON_GEM_DOMAIN_VRAM, 0);
	if (scanout->bo == NULL)
		return NULL;

	radeon_bo_map(scanout->bo, 1);

	ret = drmModeAddFB(drmmode->fd, width, height, pScrn->depth,
			   pScrn->bitsPerPixel, rotate_pitch,
			   scanout->bo->handle,
			   &scanout->fb_id);
	if (ret) {
		ErrorF("failed to add scanout fb\n");
		radeon_bo_unref(scanout->bo);
		scanout->bo = NULL;
		return NULL;
	}

	scanout->width = width;
	scanout->height = height;
	return scanout->bo->ptr;
}

static PixmapPtr
drmmode_crtc_scanout_create(xf86CrtcPtr crtc, struct drmmode_scanout *scanout,
			    int width, int height)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;
	unsigned long rotate_pitch;

	if (scanout->pixmap) {
		if (scanout->width == width && scanout->height == height)
			return scanout->pixmap;

		drmmode_crtc_scanout_destroy(drmmode, scanout);
	}

	if (!scanout->bo) {
		if (!drmmode_crtc_scanout_allocate(crtc, scanout, width, height))
			return NULL;
	}

	rotate_pitch = RADEON_ALIGN(width, drmmode_get_pitch_align(pScrn, drmmode->cpp, 0))
		* drmmode->cpp;

	scanout->pixmap = drmmode_create_bo_pixmap(pScrn,
						 width, height,
						 pScrn->depth,
						 pScrn->bitsPerPixel,
						 rotate_pitch,
						 0, scanout->bo, NULL);
	if (scanout->pixmap == NULL)
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Couldn't allocate scanout pixmap for CRTC\n");

	return scanout->pixmap;
}

static void
radeon_screen_damage_report(DamagePtr damage, RegionPtr region, void *closure)
{
	/* Only keep track of the extents */
	RegionUninit(&damage->damage);
	damage->damage.data = NULL;
}

static Bool
drmmode_set_mode_major(xf86CrtcPtr crtc, DisplayModePtr mode,
		     Rotation rotation, int x, int y)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	RADEONInfoPtr info = RADEONPTR(pScrn);
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;
	int saved_x, saved_y;
	Rotation saved_rotation;
	DisplayModeRec saved_mode;
	uint32_t *output_ids = NULL;
	int output_count = 0;
	Bool ret = TRUE;
	int i;
	int fb_id;
	drmModeModeInfo kmode;
	int pitch;
	uint32_t tiling_flags = 0;

	if (info->allowColorTiling) {
		if (info->ChipFamily >= CHIP_FAMILY_R600)
			tiling_flags |= RADEON_TILING_MICRO;
		else
			tiling_flags |= RADEON_TILING_MACRO;
	}

	pitch = RADEON_ALIGN(pScrn->displayWidth, drmmode_get_pitch_align(pScrn, info->pixel_bytes, tiling_flags)) *
		info->pixel_bytes;
	if (info->ChipFamily >= CHIP_FAMILY_R600) {
		pitch = info->front_surface.level[0].pitch_bytes;
	}

	if (drmmode->fb_id == 0) {
		ret = drmModeAddFB(drmmode->fd,
				   pScrn->virtualX, pScrn->virtualY,
                                   pScrn->depth, pScrn->bitsPerPixel,
				   pitch,
				   info->front_bo->handle,
                                   &drmmode->fb_id);
                if (ret < 0) {
                        ErrorF("failed to add fb\n");
                        return FALSE;
                }
        }

	saved_mode = crtc->mode;
	saved_x = crtc->x;
	saved_y = crtc->y;
	saved_rotation = crtc->rotation;

	if (mode) {
		crtc->mode = *mode;
		crtc->x = x;
		crtc->y = y;
		crtc->rotation = rotation;
		crtc->transformPresent = FALSE;

		output_ids = calloc(sizeof(uint32_t), xf86_config->num_output);
		if (!output_ids) {
			ret = FALSE;
			goto done;
		}

		ScreenPtr pScreen = pScrn->pScreen;

		for (i = 0; i < xf86_config->num_output; i++) {
			xf86OutputPtr output = xf86_config->output[i];
			drmmode_output_private_ptr drmmode_output;

			if (output->crtc != crtc)
				continue;

			drmmode_output = output->driver_private;
			output_ids[output_count] = drmmode_output->mode_output->connector_id;
			output_count++;
		}

		if (!xf86CrtcRotate(crtc)) {
			goto done;
		}
		crtc->funcs->gamma_set(crtc, crtc->gamma_red, crtc->gamma_green,
				       crtc->gamma_blue, crtc->gamma_size);

		drmmode_ConvertToKMode(crtc->scrn, &kmode, mode);

		fb_id = drmmode->fb_id;
#ifdef RADEON_PIXMAP_SHARING
		if (crtc->randr_crtc && crtc->randr_crtc->scanout_pixmap) {
			x = drmmode_crtc->prime_pixmap_x;
			y = 0;

			drmmode_crtc_scanout_destroy(drmmode, &drmmode_crtc->scanout[0]);
			drmmode_crtc_scanout_destroy(drmmode, &drmmode_crtc->scanout[1]);
		} else
#endif
		if (drmmode_crtc->rotate.fb_id) {
			fb_id = drmmode_crtc->rotate.fb_id;
			x = y = 0;

			drmmode_crtc_scanout_destroy(drmmode, &drmmode_crtc->scanout[0]);
			drmmode_crtc_scanout_destroy(drmmode, &drmmode_crtc->scanout[1]);
		} else if (info->tear_free || info->shadow_primary) {
			for (i = 0; i < (info->tear_free ? 2 : 1); i++) {
				drmmode_crtc_scanout_create(crtc,
							    &drmmode_crtc->scanout[i],
							    mode->HDisplay,
							    mode->VDisplay);

				if (drmmode_crtc->scanout[i].pixmap) {
					RegionPtr pRegion;
					BoxPtr pBox;

					if (!drmmode_crtc->scanout[i].damage) {
						drmmode_crtc->scanout[i].damage =
							DamageCreate(radeon_screen_damage_report,
								     NULL, DamageReportRawRegion,
								     TRUE, pScreen, NULL);
						DamageRegister(&pScreen->GetScreenPixmap(pScreen)->drawable,
							       drmmode_crtc->scanout[i].damage);
					}

					pRegion = DamageRegion(drmmode_crtc->scanout[i].damage);
					RegionUninit(pRegion);
					pRegion->data = NULL;
					pBox = RegionExtents(pRegion);
					pBox->x1 = min(pBox->x1, x);
					pBox->y1 = min(pBox->y1, y);
					pBox->x2 = max(pBox->x2, x + mode->HDisplay);
					pBox->y2 = max(pBox->y2, y + mode->VDisplay);
				}
			}

			if (drmmode_crtc->scanout[0].pixmap &&
			    (!info->tear_free || drmmode_crtc->scanout[1].pixmap)) {
				drmmode_crtc->scanout_id = 0;
				fb_id = drmmode_crtc->scanout[0].fb_id;
				x = y = 0;

				radeon_scanout_update_handler(pScrn, 0, 0, crtc);
				radeon_bo_wait(drmmode_crtc->scanout[0].bo);
			}
		}
		if (drmModeSetCrtc(drmmode->fd,
				   drmmode_crtc->mode_crtc->crtc_id,
				   fb_id, x, y, output_ids,
				   output_count, &kmode) != 0) {
			xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR,
				   "failed to set mode: %s", strerror(errno));
			ret = FALSE;
			goto done;
		} else
			ret = TRUE;

		if (crtc->scrn->pScreen)
			xf86CrtcSetScreenSubpixelOrder(crtc->scrn->pScreen);

		drmmode_crtc->need_modeset = FALSE;

		/* go through all the outputs and force DPMS them back on? */
		for (i = 0; i < xf86_config->num_output; i++) {
			xf86OutputPtr output = xf86_config->output[i];

			if (output->crtc != crtc)
				continue;

			output->funcs->dpms(output, DPMSModeOn);
		}
	}

	if (pScrn->pScreen &&
		!xf86ReturnOptValBool(info->Options, OPTION_SW_CURSOR, FALSE))
		xf86_reload_cursors(pScrn->pScreen);

done:
	if (!ret) {
		crtc->x = saved_x;
		crtc->y = saved_y;
		crtc->rotation = saved_rotation;
		crtc->mode = saved_mode;
	}
#if defined(XF86_CRTC_VERSION) && XF86_CRTC_VERSION >= 3
	else
		crtc->active = TRUE;
#endif
	free(output_ids);

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
	drmmode_ptr drmmode = drmmode_crtc->drmmode;

	drmModeMoveCursor(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id, x, y);
}

static void
drmmode_load_cursor_argb (xf86CrtcPtr crtc, CARD32 *image)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	RADEONInfoPtr info = RADEONPTR(pScrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	int i;
	uint32_t *ptr;
	uint32_t cursor_size = info->cursor_w * info->cursor_h;

	/* cursor should be mapped already */
	ptr = (uint32_t *)(drmmode_crtc->cursor_bo->ptr);

	for (i = 0; i < cursor_size; i++)
		ptr[i] = cpu_to_le32(image[i]);
}


static void
drmmode_hide_cursor (xf86CrtcPtr crtc)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	RADEONInfoPtr info = RADEONPTR(pScrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;

	drmModeSetCursor(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id, 0,
			 info->cursor_w, info->cursor_h);

}

static void
drmmode_show_cursor (xf86CrtcPtr crtc)
{
	ScrnInfoPtr pScrn = crtc->scrn;
	RADEONInfoPtr info = RADEONPTR(pScrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;
	uint32_t handle = drmmode_crtc->cursor_bo->handle;
	static Bool use_set_cursor2 = TRUE;

	if (use_set_cursor2) {
	    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	    CursorPtr cursor = xf86_config->cursor;
	    int ret;

	    ret =
		drmModeSetCursor2(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id,
				  handle, info->cursor_w, info->cursor_h,
				  cursor->bits->xhot, cursor->bits->yhot);
	    if (ret == -EINVAL)
		use_set_cursor2 = FALSE;
	    else
		return;
	}

	drmModeSetCursor(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id, handle,
			 info->cursor_w, info->cursor_h);
}

static void *
drmmode_crtc_shadow_allocate(xf86CrtcPtr crtc, int width, int height)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

	return drmmode_crtc_scanout_allocate(crtc, &drmmode_crtc->rotate,
					     width, height);
}

static PixmapPtr
drmmode_crtc_shadow_create(xf86CrtcPtr crtc, void *data, int width, int height)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

	/* Xorg passes in the return value of drmmode_crtc_shadow_allocate
	 * for data, but that's redundant for drmmode_crtc_scanout_create.
	 */
	return drmmode_crtc_scanout_create(crtc, &drmmode_crtc->rotate, width,
					   height);
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
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;

	drmModeCrtcSetGamma(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id,
			    size, red, green, blue);
}

#ifdef RADEON_PIXMAP_SHARING
static Bool
drmmode_set_scanout_pixmap(xf86CrtcPtr crtc, PixmapPtr ppix)
{
	ScreenPtr screen = xf86ScrnToScreen(crtc->scrn);
	PixmapPtr screenpix = screen->GetScreenPixmap(screen);
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	int c, total_width = 0, max_height = 0, this_x = 0;

	if (!ppix) {
		if (crtc->randr_crtc->scanout_pixmap)
			PixmapStopDirtyTracking(crtc->randr_crtc->scanout_pixmap, screenpix);
		drmmode_crtc->prime_pixmap_x = 0;
		return TRUE;
	}

	/* iterate over all the attached crtcs -
	   work out bounding box */
	for (c = 0; c < xf86_config->num_crtc; c++) {
		xf86CrtcPtr iter = xf86_config->crtc[c];
		if (!iter->enabled && iter != crtc)
			continue;
		if (iter == crtc) {
			this_x = total_width;
			total_width += ppix->drawable.width;
			if (max_height < ppix->drawable.height)
				max_height = ppix->drawable.height;
		} else {
			total_width += iter->mode.HDisplay;
			if (max_height < iter->mode.VDisplay)
				max_height = iter->mode.VDisplay;
		}
#ifndef HAS_DIRTYTRACKING2
		if (iter != crtc) {
			ErrorF("Cannot do multiple crtcs without X server dirty tracking 2 interface\n");
			return FALSE;
		}
#endif
	}

	if (total_width != screenpix->drawable.width ||
	    max_height != screenpix->drawable.height) {
		Bool ret;
		ret = drmmode_xf86crtc_resize(crtc->scrn, total_width, max_height);
		if (ret == FALSE)
			return FALSE;

		screenpix = screen->GetScreenPixmap(screen);
		screen->width = screenpix->drawable.width = total_width;
		screen->height = screenpix->drawable.height = max_height;
	}
	drmmode_crtc->prime_pixmap_x = this_x;
#ifdef HAS_DIRTYTRACKING_ROTATION
	PixmapStartDirtyTracking(ppix, screenpix, 0, 0, this_x, 0, RR_Rotate_0);
#elif defined(HAS_DIRTYTRACKING2)
	PixmapStartDirtyTracking2(ppix, screenpix, 0, 0, this_x, 0);
#else
	PixmapStartDirtyTracking(ppix, screenpix, 0, 0);
#endif
	return TRUE;
}
#endif

static xf86CrtcFuncsRec drmmode_crtc_funcs = {
    .dpms = drmmode_crtc_dpms,
    .set_mode_major = drmmode_set_mode_major,
    .set_cursor_colors = drmmode_set_cursor_colors,
    .set_cursor_position = drmmode_set_cursor_position,
    .show_cursor = drmmode_show_cursor,
    .hide_cursor = drmmode_hide_cursor,
    .load_cursor_argb = drmmode_load_cursor_argb,

    .gamma_set = drmmode_crtc_gamma_set,
    .shadow_create = drmmode_crtc_shadow_create,
    .shadow_allocate = drmmode_crtc_shadow_allocate,
    .shadow_destroy = drmmode_crtc_shadow_destroy,
    .destroy = NULL, /* XXX */
#ifdef RADEON_PIXMAP_SHARING
    .set_scanout_pixmap = drmmode_set_scanout_pixmap,
#endif
};

int drmmode_get_crtc_id(xf86CrtcPtr crtc)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	return drmmode_crtc->hw_id;
}

void drmmode_crtc_hw_id(xf86CrtcPtr crtc)
{
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
	ScrnInfoPtr pScrn = crtc->scrn;
	RADEONInfoPtr info = RADEONPTR(pScrn);
	struct drm_radeon_info ginfo;
	int r;
	uint32_t tmp;

	memset(&ginfo, 0, sizeof(ginfo));
	ginfo.request = 0x4;
	tmp = drmmode_crtc->mode_crtc->crtc_id;
	ginfo.value = (uintptr_t)&tmp;
	r = drmCommandWriteRead(info->dri2.drm_fd, DRM_RADEON_INFO, &ginfo, sizeof(ginfo));
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

	crtc = xf86CrtcCreate(pScrn, &drmmode_crtc_funcs);
	if (crtc == NULL)
		return 0;

	drmmode_crtc = xnfcalloc(sizeof(drmmode_crtc_private_rec), 1);
	drmmode_crtc->mode_crtc = drmModeGetCrtc(drmmode->fd, mode_res->crtcs[num]);
	drmmode_crtc->drmmode = drmmode;
	crtc->driver_private = drmmode_crtc;
	drmmode_crtc_hw_id(crtc);

	/* Mark num'th crtc as in use on this device. */
	pRADEONEnt->assigned_crtcs |= (1 << num);
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Allocated crtc nr. %d to this screen.\n", num);

	return 1;
}

static xf86OutputStatus
drmmode_output_detect(xf86OutputPtr output)
{
	/* go to the hw and retrieve a new output struct */
	drmmode_output_private_ptr drmmode_output = output->driver_private;
	drmmode_ptr drmmode = drmmode_output->drmmode;
	xf86OutputStatus status;
	drmModeFreeConnector(drmmode_output->mode_output);

	drmmode_output->mode_output = drmModeGetConnector(drmmode->fd, drmmode_output->output_id);
	if (!drmmode_output->mode_output)
		return XF86OutputStatusDisconnected;

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

static DisplayModePtr
drmmode_output_get_modes(xf86OutputPtr output)
{
	drmmode_output_private_ptr drmmode_output = output->driver_private;
	drmModeConnectorPtr koutput = drmmode_output->mode_output;
	drmmode_ptr drmmode = drmmode_output->drmmode;
	int i;
	DisplayModePtr Modes = NULL, Mode;
	drmModePropertyPtr props;
	xf86MonPtr mon = NULL;

	if (!koutput)
		return NULL;

	/* look for an EDID property */
	for (i = 0; i < koutput->count_props; i++) {
		props = drmModeGetProperty(drmmode->fd, koutput->props[i]);
		if (props && (props->flags & DRM_MODE_PROP_BLOB)) {
			if (!strcmp(props->name, "EDID")) {
				if (drmmode_output->edid_blob)
					drmModeFreePropertyBlob(drmmode_output->edid_blob);
				drmmode_output->edid_blob = drmModeGetPropertyBlob(drmmode->fd, koutput->prop_values[i]);
			}
		}
		if (props)
			drmModeFreeProperty(props);
	}

	if (drmmode_output->edid_blob) {
		mon = xf86InterpretEDID(output->scrn->scrnIndex,
					drmmode_output->edid_blob->data);
		if (mon && drmmode_output->edid_blob->length > 128)
			mon->flags |= MONITOR_EDID_COMPLETE_RAWDATA;
	}
	xf86OutputSetEDID(output, mon);

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

	if (drmmode_output->edid_blob)
		drmModeFreePropertyBlob(drmmode_output->edid_blob);
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
	drmmode_ptr drmmode = drmmode_output->drmmode;

	if (!koutput)
		return;

	if (mode != DPMSModeOn && crtc)
		drmmode_do_crtc_dpms(crtc, mode);

	drmModeConnectorSetProperty(drmmode->fd, koutput->connector_id,
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
    drmmode_output_private_ptr drmmode_output = output->driver_private;
    drmModeConnectorPtr mode_output = drmmode_output->mode_output;
    drmmode_ptr drmmode = drmmode_output->drmmode;
    drmModePropertyPtr drmmode_prop;
    int i, j, err;

    drmmode_output->props = calloc(mode_output->count_props, sizeof(drmmode_prop_rec));
    if (!drmmode_output->props)
	return;
    
    drmmode_output->num_props = 0;
    for (i = 0, j = 0; i < mode_output->count_props; i++) {
	drmmode_prop = drmModeGetProperty(drmmode->fd, mode_output->props[i]);
	if (drmmode_property_ignore(drmmode_prop)) {
	    drmModeFreeProperty(drmmode_prop);
	    continue;
	}
	drmmode_output->props[j].mode_prop = drmmode_prop;
	drmmode_output->props[j].value = mode_output->prop_values[i];
	drmmode_output->num_props++;
	j++;
    }

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

static Bool
drmmode_output_set_property(xf86OutputPtr output, Atom property,
		RRPropertyValuePtr value)
{
    drmmode_output_private_ptr drmmode_output = output->driver_private;
    drmmode_ptr drmmode = drmmode_output->drmmode;
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

	    drmModeConnectorSetProperty(drmmode->fd, drmmode_output->output_id,
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

	    /* search for matching name string, then set its value down */
	    for (j = 0; j < p->mode_prop->count_enums; j++) {
		if (!strcmp(p->mode_prop->enums[j].name, name)) {
		    drmModeConnectorSetProperty(drmmode->fd, drmmode_output->output_id,
			    p->mode_prop->prop_id, p->mode_prop->enums[j].value);
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
#if 0

    .save = drmmode_crt_save,
    .restore = drmmode_crt_restore,
    .mode_fixup = drmmode_crt_mode_fixup,
    .prepare = drmmode_output_prepare,
    .mode_set = drmmode_crt_mode_set,
    .commit = drmmode_output_commit,
#endif
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
		if (koutput->connector_type >= NUM_OUTPUT_NAMES)
			snprintf(name, 32, "Unknown%d-%d", koutput->connector_type,
				 koutput->connector_type_id - 1);
#ifdef RADEON_PIXMAP_SHARING
		else if (pScrn->is_gpu)
			snprintf(name, 32, "%s-%d-%d",
				 output_names[koutput->connector_type], pScrn->scrnIndex - GPU_SCREEN_OFFSET + 1,
				 koutput->connector_type_id - 1);
#endif
		else {
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
	RADEONInfoPtr info = RADEONPTR(pScrn);
	xf86OutputPtr output;
	drmModeConnectorPtr koutput;
	drmModeEncoderPtr *kencoders = NULL;
	drmmode_output_private_ptr drmmode_output;
	drmModePropertyPtr props;
	drmModePropertyBlobPtr path_blob = NULL;
	char name[32];
	int i;
	const char *s;

	koutput = drmModeGetConnector(drmmode->fd, mode_res->connectors[num]);
	if (!koutput)
		return 0;

	for (i = 0; i < koutput->count_props; i++) {
		props = drmModeGetProperty(drmmode->fd, koutput->props[i]);
		if (props && (props->flags & DRM_MODE_PROP_BLOB)) {
			if (!strcmp(props->name, "PATH")) {
				path_blob = drmModeGetPropertyBlob(drmmode->fd, koutput->prop_values[i]);
				drmModeFreeProperty(props);
				break;
			}
			drmModeFreeProperty(props);
		}
	}

	kencoders = calloc(sizeof(drmModeEncoderPtr), koutput->count_encoders);
	if (!kencoders) {
		goto out_free_encoders;
	}

	for (i = 0; i < koutput->count_encoders; i++) {
		kencoders[i] = drmModeGetEncoder(drmmode->fd, koutput->encoders[i]);
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
			if (!info->IsSecondary && (num != 0))
				goto out_free_encoders;
			else if (info->IsSecondary && (num != 1))
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
	
	output->possible_crtcs = 0xffffffff;
	for (i = 0; i < koutput->count_encoders; i++) {
		output->possible_crtcs &= kencoders[i]->possible_crtcs;
	}
	/* work out the possible clones later */
	output->possible_clones = 0;

	for (i = 0; i < koutput->count_props; i++) {
		props = drmModeGetProperty(drmmode->fd, koutput->props[i]);
		if (props && (props->flags & DRM_MODE_PROP_ENUM)) {
			if (!strcmp(props->name, "DPMS")) {
				drmmode_output->dpms_enum_id = koutput->props[i];
				drmModeFreeProperty(props);
				break;
			}
			drmModeFreeProperty(props);
		}
	}

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
	drmmode_crtc_private_ptr
		    drmmode_crtc = xf86_config->crtc[0]->driver_private;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;
	RADEONInfoPtr info = RADEONPTR(scrn);
	struct radeon_bo *old_front = NULL;
	Bool	    ret;
	ScreenPtr   screen = xf86ScrnToScreen(scrn);
	uint32_t    old_fb_id;
	int	    i, pitch, old_width, old_height, old_pitch;
	int aligned_height;
	uint32_t screen_size;
	int cpp = info->pixel_bytes;
	struct radeon_bo *front_bo;
	struct radeon_surface surface;
	struct radeon_surface *psurface;
	uint32_t tiling_flags = 0, base_align;
	PixmapPtr ppix = screen->GetScreenPixmap(screen);
	void *fb_shadow;
	xRectangle rect;
	Bool force;
	GCPtr gc;

	if (scrn->virtualX == width && scrn->virtualY == height)
		return TRUE;

	front_bo = info->front_bo;
	radeon_cs_flush_indirect(scrn);

	if (front_bo)
		radeon_bo_wait(front_bo);

	if (info->allowColorTiling && !info->shadow_primary) {
		if (info->ChipFamily >= CHIP_FAMILY_R600) {
			if (info->allowColorTiling2D) {
				tiling_flags |= RADEON_TILING_MACRO;
			} else {
				tiling_flags |= RADEON_TILING_MICRO;
			}
		} else
			tiling_flags |= RADEON_TILING_MACRO;
	}

	pitch = RADEON_ALIGN(width, drmmode_get_pitch_align(scrn, cpp, tiling_flags)) * cpp;
	aligned_height = RADEON_ALIGN(height, drmmode_get_height_align(scrn, tiling_flags));
	screen_size = RADEON_ALIGN(pitch * aligned_height, RADEON_GPU_PAGE_SIZE);
	base_align = 4096;
	if (info->ChipFamily >= CHIP_FAMILY_R600) {
		memset(&surface, 0, sizeof(struct radeon_surface));
		surface.npix_x = width;
		surface.npix_y = height;
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
			return FALSE;
		}
		if (radeon_surface_init(info->surf_man, &surface)) {
			return FALSE;
		}
		screen_size = surface.bo_size;
		base_align = surface.bo_alignment;
		pitch = surface.level[0].pitch_bytes;
		tiling_flags = 0;
		switch (surface.level[0].mode) {
		case RADEON_SURF_MODE_2D:
			tiling_flags |= RADEON_TILING_MACRO;
			tiling_flags |= surface.bankw << RADEON_TILING_EG_BANKW_SHIFT;
			tiling_flags |= surface.bankh << RADEON_TILING_EG_BANKH_SHIFT;
			tiling_flags |= surface.mtilea << RADEON_TILING_EG_MACRO_TILE_ASPECT_SHIFT;
			if (surface.tile_split)
				tiling_flags |= eg_tile_split(surface.tile_split)
						<< RADEON_TILING_EG_TILE_SPLIT_SHIFT;
			break;
		case RADEON_SURF_MODE_1D:
			tiling_flags |= RADEON_TILING_MICRO;
			break;
		default:
			break;
		}
		info->front_surface = surface;
	}

	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "Allocate new frame buffer %dx%d stride %d\n",
		   width, height, pitch / cpp);

	old_width = scrn->virtualX;
	old_height = scrn->virtualY;
	old_pitch = scrn->displayWidth;
	old_fb_id = drmmode->fb_id;
	old_front = info->front_bo;

	scrn->virtualX = width;
	scrn->virtualY = height;
	scrn->displayWidth = pitch / cpp;

	info->front_bo = radeon_bo_open(info->bufmgr, 0, screen_size, base_align,
					info->shadow_primary ?
					RADEON_GEM_DOMAIN_GTT :
					RADEON_GEM_DOMAIN_VRAM, 0);
	if (!info->front_bo)
		goto fail;

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

	ret = drmModeAddFB(drmmode->fd, width, height, scrn->depth,
			   scrn->bitsPerPixel, pitch,
			   info->front_bo->handle,
			   &drmmode->fb_id);
	if (ret)
		goto fail;

	if (!info->r600_shadow_fb) {
		radeon_set_pixmap_bo(ppix, info->front_bo);
		psurface = radeon_get_pixmap_surface(ppix);
		*psurface = info->front_surface;
		screen->ModifyPixmapHeader(ppix,
					   width, height, -1, -1, pitch, NULL);
	} else {
		if (radeon_bo_map(info->front_bo, 1))
			goto fail;
		fb_shadow = calloc(1, screen_size);
		if (fb_shadow == NULL)
			goto fail;
		free(info->fb_shadow);
		info->fb_shadow = fb_shadow;
		screen->ModifyPixmapHeader(ppix,
					   width, height, -1, -1, pitch,
					   info->fb_shadow);
	}
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,9,99,1,0)
	scrn->pixmapPrivate.ptr = ppix->devPrivate.ptr;
#endif

	if (info->use_glamor)
		radeon_glamor_create_screen_resources(scrn->pScreen);

	/* Clear new buffer */
	gc = GetScratchGC(ppix->drawable.depth, scrn->pScreen);
	force = info->accel_state->force;
	info->accel_state->force = TRUE;
	ValidateGC(&ppix->drawable, gc);
	rect.x = 0;
	rect.y = 0;
	rect.width = width;
	rect.height = height;
	(*gc->ops->PolyFillRect)(&ppix->drawable, gc, 1, &rect);
	FreeScratchGC(gc);
	info->accel_state->force = force;
	radeon_cs_flush_indirect(scrn);
	radeon_bo_wait(info->front_bo);

	for (i = 0; i < xf86_config->num_crtc; i++) {
		xf86CrtcPtr crtc = xf86_config->crtc[i];

		if (!crtc->enabled)
			continue;

		drmmode_set_mode_major(crtc, &crtc->mode,
				       crtc->rotation, crtc->x, crtc->y);
	}

	if (old_fb_id)
		drmModeRmFB(drmmode->fd, old_fb_id);
	if (old_front)
		radeon_bo_unref(old_front);

	radeon_kms_update_vram_limit(scrn, screen_size);
	return TRUE;

 fail:
	if (info->front_bo)
		radeon_bo_unref(info->front_bo);
	info->front_bo = old_front;
	scrn->virtualX = old_width;
	scrn->virtualY = old_height;
	scrn->displayWidth = old_pitch;
	drmmode->fb_id = old_fb_id;

	return FALSE;
}

static const xf86CrtcConfigFuncsRec drmmode_xf86crtc_config_funcs = {
	drmmode_xf86crtc_resize
};

static void
drmmode_flip_free(drmmode_flipevtcarrier_ptr flipcarrier)
{
	drmmode_flipdata_ptr flipdata = flipcarrier->flipdata;

	free(flipcarrier);

	if (--flipdata->flip_count > 0)
		return;

	free(flipdata);
}

static void
drmmode_flip_abort(ScrnInfoPtr scrn, void *event_data)
{
	drmmode_flipevtcarrier_ptr flipcarrier = event_data;
	drmmode_flipdata_ptr flipdata = flipcarrier->flipdata;

	if (flipdata->flip_count == 1)
		flipdata->abort(scrn, flipdata->event_data);

	drmmode_flip_free(flipcarrier);
}

static void
drmmode_flip_handler(ScrnInfoPtr scrn, uint32_t frame, uint64_t usec, void *event_data)
{
	drmmode_flipevtcarrier_ptr flipcarrier = event_data;
	drmmode_flipdata_ptr flipdata = flipcarrier->flipdata;

	/* Is this the event whose info shall be delivered to higher level? */
	if (flipcarrier->dispatch_me) {
		/* Yes: Cache msc, ust for later delivery. */
		flipdata->fe_frame = frame;
		flipdata->fe_usec = usec;
	}

	if (flipdata->flip_count == 1) {
		/* Deliver cached msc, ust from reference crtc to flip event handler */
		if (flipdata->event_data)
			flipdata->handler(scrn, flipdata->fe_frame,
					  flipdata->fe_usec,
					  flipdata->event_data);

		/* Release framebuffer */
		drmModeRmFB(flipdata->drmmode->fd, flipdata->old_fb_id);
	}

	drmmode_flip_free(flipcarrier);
}


static void
drm_wakeup_handler(pointer data, int err, pointer p)
{
	drmmode_ptr drmmode = data;
	fd_set *read_mask = p;

	if (err >= 0 && FD_ISSET(drmmode->fd, read_mask)) {
		drmHandleEvent(drmmode->fd, &drmmode->event_context);
	}
}

Bool drmmode_pre_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode, int cpp)
{
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	RADEONInfoPtr info = RADEONPTR(pScrn);
	int i, num_dvi = 0, num_hdmi = 0;
	drmModeResPtr mode_res;
	unsigned int crtcs_needed = 0;

	xf86CrtcConfigInit(pScrn, &drmmode_xf86crtc_config_funcs);

	drmmode->scrn = pScrn;
	drmmode->cpp = cpp;
	mode_res = drmModeGetResources(drmmode->fd);
	if (!mode_res)
		return FALSE;

	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Initializing outputs ...\n");
	for (i = 0; i < mode_res->count_connectors; i++)
		crtcs_needed += drmmode_output_init(pScrn, drmmode, mode_res,
						    i, &num_dvi, &num_hdmi, 0);
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "%d crtcs needed for screen.\n", crtcs_needed);

	if (info->r600_shadow_fb) {
		/* Rotation requires hardware acceleration */
		drmmode_crtc_funcs.shadow_allocate = NULL;
		drmmode_crtc_funcs.shadow_create = NULL;
		drmmode_crtc_funcs.shadow_destroy = NULL;
	}

	drmmode->count_crtcs = mode_res->count_crtcs;
	xf86CrtcSetSizeRange(pScrn, 320, 200, mode_res->max_width, mode_res->max_height);

	for (i = 0; i < mode_res->count_crtcs; i++)
		if (!xf86IsEntityShared(pScrn->entityList[0]) ||
		    (crtcs_needed && !(pRADEONEnt->assigned_crtcs & (1 << i))))
			crtcs_needed -= drmmode_crtc_init(pScrn, drmmode, mode_res, i);

	/* All ZaphodHeads outputs provided with matching crtcs? */
	if (xf86IsEntityShared(pScrn->entityList[0]) && (crtcs_needed > 0))
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "%d ZaphodHeads crtcs unavailable. Some outputs will stay off.\n",
			   crtcs_needed);

	/* workout clones */
	drmmode_clones_init(pScrn, drmmode, mode_res);

#ifdef RADEON_PIXMAP_SHARING
	xf86ProviderSetup(pScrn, NULL, "radeon");
#endif

	xf86InitialConfiguration(pScrn, TRUE);

	drmmode->event_context.version = DRM_EVENT_CONTEXT_VERSION;
	drmmode->event_context.vblank_handler = radeon_drm_queue_handler;
	drmmode->event_context.page_flip_handler = radeon_drm_queue_handler;

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
		AddGeneralSocket(drmmode->fd);
		RegisterBlockAndWakeupHandlers((BlockHandlerProcPtr)NoopDDA,
				drm_wakeup_handler, drmmode);
		pRADEONEnt->fd_wakeup_registered = serverGeneration;
		pRADEONEnt->fd_wakeup_ref = 1;
	} else
		pRADEONEnt->fd_wakeup_ref++;
}

void drmmode_fini(ScrnInfoPtr pScrn, drmmode_ptr drmmode)
{
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	RADEONInfoPtr info = RADEONPTR(pScrn);

	if (info->dri2.pKernelDRMVersion->version_minor < 4 || !info->drmmode_inited)
		return;

	if (pRADEONEnt->fd_wakeup_registered == serverGeneration &&
	    !--pRADEONEnt->fd_wakeup_ref) {
		RemoveGeneralSocket(drmmode->fd);
		RemoveBlockAndWakeupHandlers((BlockHandlerProcPtr)NoopDDA,
				drm_wakeup_handler, drmmode);
	}
}


Bool drmmode_set_bufmgr(ScrnInfoPtr pScrn, drmmode_ptr drmmode, struct radeon_bo_manager *bufmgr)
{
	drmmode->bufmgr = bufmgr;
	return TRUE;
}



void drmmode_set_cursor(ScrnInfoPtr scrn, drmmode_ptr drmmode, int id, struct radeon_bo *bo)
{
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	xf86CrtcPtr crtc = xf86_config->crtc[id];
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

	drmmode_crtc->cursor_bo = bo;
}

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
	int c;

	for (c = 0; c < config->num_crtc; c++) {
		xf86CrtcPtr	crtc = config->crtc[c];
		drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;
		xf86OutputPtr	output = NULL;
		int		o;

		/* Skip disabled CRTCs */
		if (!crtc->enabled) {
			if (set_hw) {
				drmmode_do_crtc_dpms(crtc, DPMSModeOff);
				drmModeSetCrtc(drmmode->fd,
					       drmmode_crtc->mode_crtc->crtc_id,
					       0, 0, 0, NULL, 0, NULL);
			}
			continue;
		}

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

		/* Mark that we'll need to re-set the mode for sure */
		memset(&crtc->mode, 0, sizeof(crtc->mode));
		if (!crtc->desiredMode.CrtcHDisplay)
		{
			DisplayModePtr  mode = xf86OutputFindClosestMode (output, pScrn->currentMode);

			if (!mode)
				return FALSE;
			crtc->desiredMode = *mode;
			crtc->desiredRotation = RR_Rotate_0;
			crtc->desiredX = 0;
			crtc->desiredY = 0;
		}

		if (set_hw) {
			if (!crtc->funcs->set_mode_major(crtc, &crtc->desiredMode, crtc->desiredRotation,
							 crtc->desiredX, crtc->desiredY))
				return FALSE;
		} else {
			crtc->mode = crtc->desiredMode;
			crtc->rotation = crtc->desiredRotation;
			crtc->x = crtc->desiredX;
			crtc->y = crtc->desiredY;
			if (!xf86CrtcRotate(crtc))
			    return FALSE;
		}
	}
	return TRUE;
}

static void drmmode_load_palette(ScrnInfoPtr pScrn, int numColors,
                                 int *indices, LOCO *colors, VisualPtr pVisual)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    uint16_t       lut_r[256], lut_g[256], lut_b[256];
    int index, j, i;
    int c;

    for (c = 0; c < xf86_config->num_crtc; c++) {
        xf86CrtcPtr crtc = xf86_config->crtc[c];
	drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

        for (i = 0 ; i < 256; i++) {
            lut_r[i] = drmmode_crtc->lut_r[i] << 6;
            lut_g[i] = drmmode_crtc->lut_g[i] << 6;
            lut_b[i] = drmmode_crtc->lut_b[i] << 6;
        }

        switch(pScrn->depth) {
        case 15:
            for (i = 0; i < numColors; i++) {
                index = indices[i];
                for (j = 0; j < 8; j++) {
                    lut_r[index * 8 + j] = colors[index].red << 6;
                    lut_g[index * 8 + j] = colors[index].green << 6;
                    lut_b[index * 8 + j] = colors[index].blue << 6;
                }
            }
         break;
         case 16:
             for (i = 0; i < numColors; i++) {
                 index = indices[i];

                  if (i <= 31) {
                      for (j = 0; j < 8; j++) {
                          lut_r[index * 8 + j] = colors[index].red << 6;
                          lut_b[index * 8 + j] = colors[index].blue << 6;
                      }
                  }

                  for (j = 0; j < 4; j++) {
                      lut_g[index * 4 + j] = colors[index].green << 6;
                  }
              }
	  break;
          default:
              for (i = 0; i < numColors; i++) {
                  index = indices[i];
                  lut_r[index] = colors[index].red << 6;
                  lut_g[index] = colors[index].green << 6;
                  lut_b[index] = colors[index].blue << 6;
              }
              break;
          }

    /* Make the change through RandR */
        if (crtc->randr_crtc)
            RRCrtcGammaSet(crtc->randr_crtc, lut_r, lut_g, lut_b);
        else
            crtc->funcs->gamma_set(crtc, lut_r, lut_g, lut_b, 256);
     }
}

Bool drmmode_setup_colormap(ScreenPtr pScreen, ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);

    if (xf86_config->num_crtc) {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Initializing kms color map\n");
	if (!miCreateDefColormap(pScreen))
	    return FALSE;
	/* all radeons support 10 bit CLUTs */
	if (!xf86HandleColormaps(pScreen, 256, 10,
				 drmmode_load_palette, NULL,
				 CMAP_PALETTED_TRUECOLOR
#if 0 /* This option messes up text mode! (eich@suse.de) */
				 | CMAP_LOAD_EVEN_IF_OFFSCREEN
#endif
				 | CMAP_RELOAD_ON_MODE_SWITCH))
	    return FALSE;
    }
    return TRUE;
}

void
radeon_mode_hotplug(ScrnInfoPtr scrn, drmmode_ptr drmmode)
{
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	drmModeResPtr mode_res;
	int i, j;
	Bool found;
	Bool changed = FALSE;

	mode_res = drmModeGetResources(drmmode->fd);
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
		found = FALSE;

		for (j = 0; j < config->num_output; j++) {
			xf86OutputPtr output = config->output[j];
			drmmode_output_private_ptr drmmode_output;

			drmmode_output = output->driver_private;
			if (mode_res->connectors[i] == drmmode_output->output_id) {
				found = TRUE;
				break;
			}
		}
		if (found)
			continue;

		changed = TRUE;
		drmmode_output_init(scrn, drmmode, mode_res, i, NULL, NULL, 1);
	}

	if (changed) {
		RRSetChanged(xf86ScrnToScreen(scrn));
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
	dev = udev_monitor_receive_device(drmmode->uevent_monitor);
	if (!dev)
		return;

	radeon_mode_hotplug(scrn, drmmode);
	udev_device_unref(dev);
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
	if (info->ChipFamily < CHIP_FAMILY_R600 &&
	    info->r600_shadow_fb && tiling_flags)
	    tiling_flags |= RADEON_TILING_SURFACE;
#endif
}

Bool radeon_do_pageflip(ScrnInfoPtr scrn, ClientPtr client,
			uint32_t new_front_handle, uint64_t id, void *data,
			int ref_crtc_hw_id, radeon_drm_handler_proc handler,
			radeon_drm_abort_proc abort)
{
	RADEONInfoPtr info = RADEONPTR(scrn);
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(scrn);
	drmmode_crtc_private_ptr drmmode_crtc = config->crtc[0]->driver_private;
	drmmode_ptr drmmode = drmmode_crtc->drmmode;
	unsigned int pitch;
	int i;
	uint32_t tiling_flags = 0;
	drmmode_flipdata_ptr flipdata;
	drmmode_flipevtcarrier_ptr flipcarrier = NULL;
	struct radeon_drm_queue_entry *drm_queue = NULL;

	if (info->allowColorTiling) {
		if (info->ChipFamily >= CHIP_FAMILY_R600)
			tiling_flags |= RADEON_TILING_MICRO;
		else
			tiling_flags |= RADEON_TILING_MACRO;
	}

	pitch = RADEON_ALIGN(scrn->displayWidth, drmmode_get_pitch_align(scrn, info->pixel_bytes, tiling_flags)) *
		info->pixel_bytes;
	if (info->ChipFamily >= CHIP_FAMILY_R600 && info->surf_man) {
		pitch = info->front_surface.level[0].pitch_bytes;
	}

        flipdata = calloc(1, sizeof(drmmode_flipdata_rec));
        if (!flipdata) {
             xf86DrvMsg(scrn->scrnIndex, X_WARNING,
                        "flip queue: data alloc failed.\n");
             goto error;
        }

	/*
	 * Create a new handle for the back buffer
	 */
	flipdata->old_fb_id = drmmode->fb_id;
	if (drmModeAddFB(drmmode->fd, scrn->virtualX, scrn->virtualY,
			 scrn->depth, scrn->bitsPerPixel, pitch,
			 new_front_handle, &drmmode->fb_id))
		goto error;

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
        flipdata->drmmode = drmmode;
        flipdata->handler = handler;
        flipdata->abort = abort;

	for (i = 0; i < config->num_crtc; i++) {
		if (!config->crtc[i]->enabled)
			continue;

		flipdata->flip_count++;
		drmmode_crtc = config->crtc[i]->driver_private;

		flipcarrier = calloc(1, sizeof(drmmode_flipevtcarrier_rec));
		if (!flipcarrier) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "flip queue: carrier alloc failed.\n");
			goto error;
		}

		/* Only the reference crtc will finally deliver its page flip
		 * completion event. All other crtc's events will be discarded.
		 */
		flipcarrier->dispatch_me = (drmmode_crtc->hw_id == ref_crtc_hw_id);
		flipcarrier->flipdata = flipdata;

		drm_queue = radeon_drm_queue_alloc(scrn, client, id,
						   flipcarrier,
						   drmmode_flip_handler,
						   drmmode_flip_abort);
		if (!drm_queue) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "Allocating DRM queue event entry failed.\n");
			goto error;
		}

		if (drmModePageFlip(drmmode->fd, drmmode_crtc->mode_crtc->crtc_id,
				    drmmode->fb_id, DRM_MODE_PAGE_FLIP_EVENT,
				    drm_queue)) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "flip queue failed: %s\n", strerror(errno));
			goto error;
		}
		flipcarrier = NULL;
		drm_queue = NULL;
	}

	if (flipdata->flip_count > 0)
		return TRUE;

error:
	if (flipdata && flipdata->flip_count <= 1) {
		drmModeRmFB(drmmode->fd, drmmode->fb_id);
		drmmode->fb_id = flipdata->old_fb_id;
	}

	if (drm_queue)
		radeon_drm_abort_entry(drm_queue);
	else if (flipcarrier)
		drmmode_flip_abort(scrn, flipcarrier);
	else if (flipdata && flipdata->flip_count <= 1)
		free(flipdata);

	xf86DrvMsg(scrn->scrnIndex, X_WARNING, "Page flip failed: %s\n",
		   strerror(errno));
	return FALSE;
}
