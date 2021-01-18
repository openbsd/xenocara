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
 *     Dave Airlie <airlied@redhat.com>
 *
 */
#ifndef DRMMODE_DISPLAY_H
#define DRMMODE_DISPLAY_H

#include "xf86drmMode.h"
#ifdef HAVE_LIBUDEV
#include "libudev.h"
#endif

#include "radeon_drm_queue.h"
#include "radeon_probe.h"

#ifndef DRM_CAP_TIMESTAMP_MONOTONIC
#define DRM_CAP_TIMESTAMP_MONOTONIC 0x6
#endif

typedef struct {
  struct radeon_bo_manager *bufmgr;
  ScrnInfoPtr scrn;
#ifdef HAVE_LIBUDEV
  struct udev_monitor *uevent_monitor;
  InputHandlerProc uevent_handler;
#endif
  drmEventContext event_context;
  int count_crtcs;

  Bool delete_dp_12_displays;

  Bool dri2_flipping;
  Bool present_flipping;
} drmmode_rec, *drmmode_ptr;

typedef struct {
  void *event_data;
  int flip_count;
  unsigned int fe_frame;
  uint64_t fe_usec;
  xf86CrtcPtr fe_crtc;
  radeon_drm_handler_proc handler;
  radeon_drm_abort_proc abort;
  struct drmmode_fb *fb[0];
} drmmode_flipdata_rec, *drmmode_flipdata_ptr;

struct drmmode_fb {
	int refcnt;
	uint32_t handle;
};

enum drmmode_scanout_status {
    DRMMODE_SCANOUT_OK,
    DRMMODE_SCANOUT_FLIP_FAILED = 1u << 0,
    DRMMODE_SCANOUT_VBLANK_FAILED = 1u << 1,
};

struct drmmode_scanout {
    struct radeon_buffer *bo;
    PixmapPtr pixmap;
    int width, height;
};

typedef struct {
    drmmode_ptr drmmode;
    drmModeCrtcPtr mode_crtc;
    int hw_id;

    CursorPtr cursor;
    int cursor_x;
    int cursor_y;
    int cursor_xhot;
    int cursor_yhot;
    unsigned cursor_id;
    struct radeon_bo *cursor_bo[2];

    struct drmmode_scanout rotate;
    struct drmmode_scanout scanout[2];
    DamagePtr scanout_damage;
    Bool ignore_damage;
    RegionRec scanout_last_region;
    unsigned scanout_id;
    uintptr_t scanout_update_pending;
    Bool tear_free;
    enum drmmode_scanout_status scanout_status;

    PixmapPtr prime_scanout_pixmap;

    int dpms_mode;
    CARD64 dpms_last_ust;
    uint32_t dpms_last_seq;
    int dpms_last_fps;
    uint32_t interpolated_vblanks;

    /* Modeset needed (for DPMS on or after a page flip crossing with a
     * modeset)
     */
    Bool need_modeset;
    /* For keeping track of nested calls to drm_wait_pending_flip /
     * drm_queue_handle_deferred
     */
    int wait_flip_nesting_level;
    /* A flip to this FB is pending for this CRTC */
    struct drmmode_fb *flip_pending;
    /* The FB currently being scanned out by this CRTC, if any */
    struct drmmode_fb *fb;
} drmmode_crtc_private_rec, *drmmode_crtc_private_ptr;

typedef struct {
    drmModePropertyPtr mode_prop;
    uint64_t value;
    int num_atoms; /* if range prop, num_atoms == 1; if enum prop, num_atoms == num_enums + 1 */
    Atom *atoms;
} drmmode_prop_rec, *drmmode_prop_ptr;


typedef struct {
    drmmode_ptr drmmode;
    int output_id;
    drmModeConnectorPtr mode_output;
    drmModeEncoderPtr *mode_encoders;
    drmModePropertyBlobPtr edid_blob;
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1, 17, 99, 901, 0)
    drmModePropertyBlobPtr tile_blob;
#endif
    int dpms_enum_id;
    int num_props;
    drmmode_prop_ptr props;
    int enc_mask;
    int enc_clone_mask;
    int tear_free;
} drmmode_output_private_rec, *drmmode_output_private_ptr;

typedef struct {
    uint32_t lessee_id;
} drmmode_lease_private_rec, *drmmode_lease_private_ptr;


enum drmmode_flip_sync {
    FLIP_VSYNC,
    FLIP_ASYNC,
};


/* Can the page flip ioctl be used for this CRTC? */
static inline Bool
drmmode_crtc_can_flip(xf86CrtcPtr crtc)
{
    drmmode_crtc_private_ptr drmmode_crtc = crtc->driver_private;

    return crtc->enabled &&
	drmmode_crtc->dpms_mode == DPMSModeOn &&
	!drmmode_crtc->rotate.bo &&
	(drmmode_crtc->tear_free ||
	 !drmmode_crtc->scanout[drmmode_crtc->scanout_id].bo);
}


static inline void
drmmode_fb_reference_loc(int drm_fd, struct drmmode_fb **old, struct drmmode_fb *new,
			 const char *caller, unsigned line)
{
    if (new) {
	if (new->refcnt <= 0) {
	    FatalError("New FB's refcnt was %d at %s:%u",
		       new->refcnt, caller, line);
	}

	new->refcnt++;
    }

    if (*old) {
	if ((*old)->refcnt <= 0) {
	    FatalError("Old FB's refcnt was %d at %s:%u",
		       (*old)->refcnt, caller, line);
	}

	if (--(*old)->refcnt == 0) {
	    drmModeRmFB(drm_fd, (*old)->handle);
	    free(*old);
	}
    }

    *old = new;
}

#define drmmode_fb_reference(fd, old, new) \
    drmmode_fb_reference_loc(fd, old, new, __func__, __LINE__)


extern int drmmode_page_flip_target_absolute(RADEONEntPtr pRADEONEnt,
					     drmmode_crtc_private_ptr drmmode_crtc,
					     int fb_id, uint32_t flags,
					     uintptr_t drm_queue_seq,
					     uint32_t target_msc);
extern int drmmode_page_flip_target_relative(RADEONEntPtr pRADEONEnt,
					     drmmode_crtc_private_ptr drmmode_crtc,
					     int fb_id, uint32_t flags,
					     uintptr_t drm_queue_seq,
					     uint32_t target_msc);
extern Bool drmmode_pre_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode, int cpp);
extern void drmmode_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode);
extern void drmmode_fini(ScrnInfoPtr pScrn, drmmode_ptr drmmode);
extern Bool drmmode_set_bufmgr(ScrnInfoPtr pScrn, drmmode_ptr drmmode, struct radeon_bo_manager *bufmgr);
void drmmode_adjust_frame(ScrnInfoPtr pScrn, drmmode_ptr drmmode, int x, int y);
extern Bool drmmode_set_desired_modes(ScrnInfoPtr pScrn, drmmode_ptr drmmode,
				      Bool set_hw);
extern void drmmode_copy_fb(ScrnInfoPtr pScrn, drmmode_ptr drmmode);
extern Bool drmmode_setup_colormap(ScreenPtr pScreen, ScrnInfoPtr pScrn);

extern void drmmode_crtc_scanout_destroy(drmmode_ptr drmmode,
					 struct drmmode_scanout *scanout);
void drmmode_crtc_scanout_free(xf86CrtcPtr crtc);
PixmapPtr drmmode_crtc_scanout_create(xf86CrtcPtr crtc,
				      struct drmmode_scanout *scanout,
				      int width, int height);

extern void drmmode_uevent_init(ScrnInfoPtr scrn, drmmode_ptr drmmode);
extern void drmmode_uevent_fini(ScrnInfoPtr scrn, drmmode_ptr drmmode);

Bool drmmode_set_mode(xf86CrtcPtr crtc, struct drmmode_fb *fb,
		      DisplayModePtr mode, int x, int y);

extern int drmmode_get_crtc_id(xf86CrtcPtr crtc);
extern int drmmode_get_height_align(ScrnInfoPtr scrn, uint32_t tiling);
extern int drmmode_get_pitch_align(ScrnInfoPtr scrn, int bpe, uint32_t tiling);
extern int drmmode_get_base_align(ScrnInfoPtr scrn, int bpe, uint32_t tiling);

Bool radeon_do_pageflip(ScrnInfoPtr scrn, ClientPtr client,
			PixmapPtr new_front, uint64_t id, void *data,
			xf86CrtcPtr ref_crtc, radeon_drm_handler_proc handler,
			radeon_drm_abort_proc abort,
			enum drmmode_flip_sync flip_sync,
			uint32_t target_msc);
int drmmode_crtc_get_ust_msc(xf86CrtcPtr crtc, CARD64 *ust, CARD64 *msc);
int drmmode_get_current_ust(int drm_fd, CARD64 *ust);

Bool drmmode_wait_vblank(xf86CrtcPtr crtc, drmVBlankSeqType type,
			 uint32_t target_seq, unsigned long signal,
			 uint64_t *ust, uint32_t *result_seq);


extern miPointerSpriteFuncRec drmmode_sprite_funcs;


#endif

