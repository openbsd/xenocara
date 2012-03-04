/*
 * Copyright 2008 Jerome Glisse
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef RADEON_DRI2_H
#define RADEON_DRI2_H

struct radeon_dri2 {
    int         drm_fd;
    Bool        enabled;
    char	*device_name;
};

#ifdef RADEON_DRI2
#include "dri2.h"
Bool radeon_dri2_screen_init(ScreenPtr pScreen);
void radeon_dri2_close_screen(ScreenPtr pScreen);
#endif

int drmmode_get_crtc_id(xf86CrtcPtr crtc);
xf86CrtcPtr radeon_covering_crtc(ScrnInfoPtr pScrn, BoxPtr box,
                                 xf86CrtcPtr desired, BoxPtr crtc_box_ret);
void radeon_dri2_frame_event_handler(unsigned int frame, unsigned int tv_sec,
                                     unsigned int tv_usec, void *event_data);
void radeon_dri2_flip_event_handler(unsigned int frame, unsigned int tv_sec,
				    unsigned int tv_usec, void *event_data);

#endif
