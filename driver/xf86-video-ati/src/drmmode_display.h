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
  int fd;
  unsigned fb_id;
  drmModeFBPtr mode_fb;
  int cpp;
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
  drmmode_ptr drmmode;
  unsigned old_fb_id;
  int flip_count;
  void *event_data;
  unsigned int fe_frame;
  uint64_t fe_usec;
  radeon_drm_handler_proc handler;
  radeon_drm_abort_proc abort;
} drmmode_flipdata_rec, *drmmode_flipdata_ptr;

typedef struct {
  drmmode_flipdata_ptr flipdata;
  Bool dispatch_me;
} drmmode_flipevtcarrier_rec, *drmmode_flipevtcarrier_ptr;

struct drmmode_scanout {
    struct radeon_bo *bo;
    PixmapPtr pixmap;
    DamagePtr damage;
    unsigned fb_id;
    int width, height;
};

typedef struct {
    drmmode_ptr drmmode;
    drmModeCrtcPtr mode_crtc;
    int hw_id;
    struct radeon_bo *cursor_bo;
    struct drmmode_scanout rotate;
    struct drmmode_scanout scanout[2];
    unsigned scanout_id;
    Bool scanout_update_pending;
    int dpms_mode;
    CARD64 dpms_last_ust;
    uint32_t dpms_last_seq;
    int dpms_last_fps;
    uint32_t interpolated_vblanks;
    uint16_t lut_r[256], lut_g[256], lut_b[256];
    int prime_pixmap_x;

    /* Modeset needed for DPMS on */
    Bool need_modeset;
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
    int dpms_enum_id;
    int num_props;
    drmmode_prop_ptr props;
    int enc_mask;
    int enc_clone_mask;
} drmmode_output_private_rec, *drmmode_output_private_ptr;


extern Bool drmmode_pre_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode, int cpp);
extern void drmmode_init(ScrnInfoPtr pScrn, drmmode_ptr drmmode);
extern void drmmode_fini(ScrnInfoPtr pScrn, drmmode_ptr drmmode);
extern Bool drmmode_set_bufmgr(ScrnInfoPtr pScrn, drmmode_ptr drmmode, struct radeon_bo_manager *bufmgr);
extern void drmmode_set_cursor(ScrnInfoPtr scrn, drmmode_ptr drmmode, int id, struct radeon_bo *bo);
void drmmode_adjust_frame(ScrnInfoPtr pScrn, drmmode_ptr drmmode, int x, int y);
extern Bool drmmode_set_desired_modes(ScrnInfoPtr pScrn, drmmode_ptr drmmode,
				      Bool set_hw);
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 10
extern void drmmode_copy_fb(ScrnInfoPtr pScrn, drmmode_ptr drmmode);
#endif
extern Bool drmmode_setup_colormap(ScreenPtr pScreen, ScrnInfoPtr pScrn);

extern void drmmode_scanout_free(ScrnInfoPtr scrn);

extern void drmmode_uevent_init(ScrnInfoPtr scrn, drmmode_ptr drmmode);
extern void drmmode_uevent_fini(ScrnInfoPtr scrn, drmmode_ptr drmmode);

extern int drmmode_get_crtc_id(xf86CrtcPtr crtc);
extern int drmmode_get_height_align(ScrnInfoPtr scrn, uint32_t tiling);
extern int drmmode_get_pitch_align(ScrnInfoPtr scrn, int bpe, uint32_t tiling);
extern int drmmode_get_base_align(ScrnInfoPtr scrn, int bpe, uint32_t tiling);

Bool radeon_do_pageflip(ScrnInfoPtr scrn, ClientPtr client,
			uint32_t new_front_handle, uint64_t id, void *data,
			int ref_crtc_hw_id, radeon_drm_handler_proc handler,
			radeon_drm_abort_proc abort);
int drmmode_crtc_get_ust_msc(xf86CrtcPtr crtc, CARD64 *ust, CARD64 *msc);
int drmmode_get_current_ust(int drm_fd, CARD64 *ust);

#endif

