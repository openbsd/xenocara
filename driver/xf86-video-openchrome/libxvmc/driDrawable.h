/*****************************************************************************
 * driDrawable.h: Lean Version of DRI utilities.
 *
 * Copyright (c) 2005 Thomas Hellstrom. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR(S) OR COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _DRIDRAWABLE_H
#define _DRIDRAWABLE_H

typedef struct _drawableInfo
{
    drm_drawable_t drmDraw;
    unsigned stamp;
    unsigned index;
    drm_clip_rect_t *clipFront;
    drm_clip_rect_t *clipBack;
    int x;
    int y;
    int w;
    int h;
    int backX;
    int backY;
    int numClipFront;
    int numClipBack;
    Bool touched;
} drawableInfo;

/*
 * Get updated info about the drawable "draw". The drawableInfo record returned is malloced
 * and administrated internally. Never free it unless you know exactly what you are doing.
 * The drm hash table "drawHash" needs to be initialized externally.
 */

extern int
getDRIDrawableInfoLocked(void *drawHash, Display * display, int screen,
    Drawable draw, unsigned lockFlags, int drmFD, drm_context_t drmContext,
    drmAddress sarea, Bool updateInfo, drawableInfo ** info,
    unsigned long infoSize);

/*
 * Free all resources created by the above function. Typically done on exit.
 */

extern void driDestroyHashContents(void *drawHash);

#endif
