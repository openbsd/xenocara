/*
 * Copyright 2008  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2008  Matthias Hopf <mhopf@novell.com>
 * Copyright 2008  Egbert Eich   <eich@novell.com>
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
 */

#ifndef _RHD_VIDEO_H
#define _RHD_VIDEO_H

/* Xvideo port struct */
struct RHDPortPriv {
    DrawablePtr pDraw;
    PixmapPtr pPixmap;

    RegionRec clip;

    void *BufferHandle;
    CARD32 BufferOffset;
    CARD32 BufferPitch;

    int id;
    int src_w;
    int src_h;
    int dst_w;
    int dst_h;
    int w;
    int h;
    int drw_x;
    int drw_y;
};

extern void RHDRADEONDisplayTexturedVideo(ScrnInfoPtr pScrn, struct RHDPortPriv *pPriv);
extern void RHDInitVideo(ScreenPtr pScreen);

#endif /* _RHD_VIDEO_H */
