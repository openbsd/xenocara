/*****************************************************************************
 * driDrawable.c: Lean Version of DRI utilities.
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

#include <X11/Xlibint.h>
#include <X11/Xutil.h>
#include "xf86drm.h"
#include "drm.h"
#include "xf86dri.h"
#include "drm_sarea.h"
#include "driDrawable.h"

static unsigned
drawStamp(volatile drm_sarea_t * pSarea, int index)
{
    return pSarea->drawableTable[index].stamp;
}

int
getDRIDrawableInfoLocked(void *drawHash, Display * display, int screen,
    Drawable draw, unsigned lockFlags, int drmFD, drm_context_t drmContext,
    drmAddress sarea, Bool updateInfo, drawableInfo ** info,
    unsigned long infoSize)
{
    drawableInfo *drawInfo;
    void *res;
    drm_drawable_t drmDraw = 0;
    volatile drm_sarea_t *pSarea = (drm_sarea_t *) sarea;
    drm_clip_rect_t *clipFront, *clipBack;

    int ret;

    if (drmHashLookup(drawHash, (unsigned long)draw, &res)) {

	/*
	 * The drawable is unknown to us. Create it and put it in the
	 * hash table.
	 */

	DRM_UNLOCK(drmFD, &pSarea->lock, drmContext);
	if (!uniDRICreateDrawable(display, screen, draw, &drmDraw)) {
	    DRM_LOCK(drmFD, &pSarea->lock, drmContext, lockFlags);
	    return 1;
	}
	DRM_LOCK(drmFD, &pSarea->lock, drmContext, lockFlags);

	drawInfo = (drawableInfo *) malloc(infoSize);
	if (!drawInfo)
	    return 1;

	drawInfo->drmDraw = drmDraw;
	drawInfo->stamp = 0;
	drawInfo->clipFront = 0;
	drawInfo->clipBack = 0;

	drmHashInsert(drawHash, (unsigned long)draw, drawInfo);

    } else {
	drawInfo = res;
    }

    drawInfo->touched = FALSE;
    while (!drawInfo->clipFront
	|| drawInfo->stamp != drawStamp(pSarea, drawInfo->index)) {

	/*
	 * The drawable has been touched since we last got info about it.
	 * obtain new info from the X server.
	 */

	drawInfo->touched = TRUE;

	if (updateInfo || !drawInfo->clipFront) {
	    DRM_UNLOCK(drmFD, &pSarea->lock, drmContext);

	    ret = uniDRIGetDrawableInfo(display, screen, draw,
		&drawInfo->index, &drawInfo->stamp, &drawInfo->x,
		&drawInfo->y, &drawInfo->w, &drawInfo->h,
		&drawInfo->numClipFront, &clipFront,
		&drawInfo->backX, &drawInfo->backY,
		&drawInfo->numClipBack, &clipBack);

	    DRM_LIGHT_LOCK(drmFD, &pSarea->lock, drmContext);

	    /*
	     * Error. Probably the drawable is destroyed. Return error and old values.
	     */

	    if (!ret) {
		free(drawInfo);
		drawInfo = NULL;
		drmHashDelete(drawHash, (unsigned long)draw);

		DRM_UNLOCK(drmFD, &pSarea->lock, drmContext);
		uniDRIDestroyDrawable(display, screen, draw);
		DRM_LOCK(drmFD, &pSarea->lock, drmContext, lockFlags);

		return 1;
	    }

	    if (drawInfo->stamp != drawStamp(pSarea, drawInfo->index)) {

		/*
		 * The info is already outdated. Sigh. Have another go.
		 */

		XFree(clipFront);
		XFree(clipBack);
		continue;
	    }

	    if (drawInfo->clipFront)
		XFree(drawInfo->clipFront);
	    drawInfo->clipFront = clipFront;
	    if (drawInfo->clipBack)
		XFree(drawInfo->clipBack);
	    drawInfo->clipBack = clipBack;
	} else {
	    if (!drawInfo->clipFront)
		drawInfo->clipFront = (drm_clip_rect_t *) ~ 0UL;
	    drawInfo->stamp = drawStamp(pSarea, drawInfo->index);
	}
    }
    *info = drawInfo;
    return 0;
}

void
driDestroyHashContents(void *drawHash)
{
    unsigned long key;
    void *content;
    drawableInfo *drawInfo;

    if (drmHashFirst(drawHash, &key, &content) < 1)
	return;
    drawInfo = (drawableInfo *) content;
    if (drawInfo->clipBack)
	XFree(drawInfo->clipBack);
    if (drawInfo->clipFront)
	XFree(drawInfo->clipFront);
    free(drawInfo);
    while (drmHashNext(drawHash, &key, &content) == 1) {
	drawInfo = (drawableInfo *) content;
	if (drawInfo->clipBack)
	    XFree(drawInfo->clipBack);
	if (drawInfo->clipFront)
	    XFree(drawInfo->clipFront);
	free(drawInfo);
    }

    return;
}
