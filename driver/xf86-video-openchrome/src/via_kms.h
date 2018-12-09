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

#ifdef HAVE_DRI
#include "xf86drmMode.h"
#endif
#ifdef HAVE_UDEV
#include "libudev.h"
#endif

typedef struct {
    int fd;
    uint32_t fb_id;
#ifdef HAVE_DRI
    drmModeResPtr mode_res;
    drmModeFBPtr mode_fb;
    drmEventContext event_context;
#endif
    ScrnInfoPtr scrn;
#ifdef HAVE_UDEV
    struct udev_monitor *uevent_monitor;
    InputHandlerProc uevent_handler;
#endif
    struct buffer_object *front_bo;

    Bool hwcursor;
} drmmode_rec, *drmmode_ptr;

typedef struct {
    drmmode_ptr drmmode;
#ifdef HAVE_DRI
    drmModeCrtcPtr mode_crtc;
#endif
    struct buffer_object *cursor_bo;
    unsigned rotate_fb_id;
    int index;
} drmmode_crtc_private_rec, *drmmode_crtc_private_ptr;

#ifdef HAVE_DRI
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
#endif

extern xf86CrtcPtr window_belongs_to_crtc(ScrnInfoPtr pScrn, int x, int y, int w, int h);
extern Bool KMSCrtcInit(ScrnInfoPtr pScrn, drmmode_ptr drmmode);
extern void drmmode_uevent_init(ScrnInfoPtr scrn, drmmode_ptr drmmode);
extern void drmmode_uevent_fini(ScrnInfoPtr scrn, drmmode_ptr drmmode);
#endif
