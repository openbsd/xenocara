/* Copyrightg (c) 2007 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Neither the name of the Advanced Micro Devices, Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "shadow.h"
#include "geode.h"

static void
LXShadowSave(ScreenPtr pScreen, ExaOffscreenArea * area)
{
    ScrnInfoPtr pScrni = xf86Screens[pScreen->myNum];
    GeodeRec *pGeode = GEODEPTR(pScrni);

    if (area == pGeode->shadowArea)
	pGeode->shadowArea = NULL;
}

static void *
LXWindowLinear(ScreenPtr pScreen, CARD32 row, CARD32 offset, int mode,
    CARD32 * size, void *closure)
{
    ScrnInfoPtr pScrni = xf86Screens[pScreen->myNum];
    GeodeRec *pGeode = GEODEPTR(pScrni);

    *size = pGeode->displayPitch;

    return (pGeode->FBBase + pGeode->displayOffset) +
	row * pGeode->displayPitch + offset;
}

static void
LXUpdateFunc(ScreenPtr pScreen, shadowBufPtr pBuf)
{
    RegionPtr damage = shadowDamage(pBuf);

    int nbox = REGION_NUM_RECTS(damage);
    BoxPtr pbox = REGION_RECTS(damage);
    int x, y, w, h, degrees = 0;
    FbStride shaStride;
    FbBits *shaBits;
    int shaBpp, dx = 0, dy = 0, dw, dh;
    int shaXoff, shaYoff;
    unsigned int srcOffset, dstOffset;
    PixmapPtr pShadow = pBuf->pPixmap;
    ScrnInfoPtr pScrni = xf86Screens[pScreen->myNum];
    GeodeRec *pGeode = GEODEPTR(pScrni);

    fbGetDrawable(&pShadow->drawable, shaBits, shaStride, shaBpp, shaXoff,
	shaYoff);

    /* Set up the blt */

    gp_wait_until_idle();
    gp_declare_blt(0);

    gp_set_bpp(pScrni->bitsPerPixel);

    switch (shaBpp) {
    case 8:
	gp_set_source_format(CIMGP_SOURCE_FMT_3_3_2);
	break;

    case 16:
	gp_set_source_format(CIMGP_SOURCE_FMT_0_5_6_5);
	break;

    case 24:
    case 32:
	gp_set_source_format(CIMGP_SOURCE_FMT_8_8_8_8);
	break;
    }

    gp_set_raster_operation(0xCC);
    gp_write_parameters();

    while (nbox--) {
	x = pbox->x1;
	y = pbox->y1;
	w = (pbox->x2 - pbox->x1);
	h = pbox->y2 - pbox->y1;

	srcOffset =
	    ((unsigned long)shaBits) - ((unsigned long)pGeode->FBBase);
	srcOffset += (y * pGeode->Pitch) + (x * (shaBpp >> 3));

	switch (pGeode->rotation) {
	case RR_Rotate_0:
	    dx = x;
	    dy = y;
	    dw = w;
	    dh = h;
	    degrees = 0;
	    break;

	case RR_Rotate_90:
	    dx = (pScrni->pScreen->height - 1) - (y + (h - 1));
	    dy = x;
	    dw = h;
	    dh = w;
	    degrees = 90;
	    break;

	case RR_Rotate_180:
	    dx = (pScrni->pScreen->width - 1) - (x + (w - 1));
	    dy = (pScrni->pScreen->height - 1) - (y + (h - 1));
	    dw = w;
	    dh = h;

	    degrees = 180;
	    break;

	case RR_Rotate_270:
	    dy = (pScrni->pScreen->width - 1) - (x + (w - 1));
	    dx = y;
	    dw = h;
	    dh = w;

	    degrees = 270;
	    break;
	}

	dstOffset = pGeode->displayOffset +
	    (dy * pGeode->displayPitch) + (dx * (pScrni->bitsPerPixel >> 3));

	gp_declare_blt(CIMGP_BLTFLAGS_HAZARD);
	gp_set_strides(pGeode->displayPitch, pGeode->Pitch);
	gp_rotate_blt(dstOffset, srcOffset, w, h, degrees);
	pbox++;
    }
}

Bool
LXSetRotatePitch(ScrnInfoPtr pScrni)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);

    pScrni->displayWidth = pGeode->displayWidth;

    if (pGeode->Compression)
	pGeode->Pitch = GeodeCalculatePitchBytes(pScrni->displayWidth,
	    pScrni->bitsPerPixel);
    else
	pGeode->Pitch = (pScrni->displayWidth * (pScrni->bitsPerPixel >> 3));
    return TRUE;
}

Bool
LXAllocShadow(ScrnInfoPtr pScrni)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);
    PixmapPtr pPixmap;

    int size;

    if (pGeode->rotation == RR_Rotate_0) {

	if (pGeode->shadowArea) {
	    exaOffscreenFree(pScrni->pScreen, pGeode->shadowArea);
	    pGeode->shadowArea = NULL;
	}

	pScrni->fbOffset = pGeode->displayOffset;
    } else {
	if (pGeode->rotation == RR_Rotate_90
	    || pGeode->rotation == RR_Rotate_270)
	    size = pGeode->displayPitch * pScrni->virtualX;
	else
	    size = pGeode->displayPitch * pScrni->virtualY;

	if (pGeode->shadowArea) {

	    if (pGeode->shadowArea->size < size) {
		exaOffscreenFree(pScrni->pScreen, pGeode->shadowArea);
		pGeode->shadowArea = NULL;
	    }
	}

	if (pGeode->shadowArea == NULL) {
	    pGeode->shadowArea =
		exaOffscreenAlloc(pScrni->pScreen, size, 32, TRUE,
		LXShadowSave, pGeode);

	    if (pGeode->shadowArea == NULL)
		return FALSE;
	}

	pScrni->fbOffset = pGeode->shadowArea->offset;
    }

    pPixmap = pScrni->pScreen->GetScreenPixmap(pScrni->pScreen);

    pScrni->pScreen->ModifyPixmapHeader(pPixmap,
	pScrni->pScreen->width,
	pScrni->pScreen->height,
	pScrni->pScreen->rootDepth,
	pScrni->bitsPerPixel,
	PixmapBytePad(pScrni->displayWidth, pScrni->pScreen->rootDepth),
	(pointer) (pGeode->FBBase + pScrni->fbOffset));

    return TRUE;
}

Bool
LXRotate(ScrnInfoPtr pScrni, DisplayModePtr mode)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);
    Rotation curr = pGeode->rotation;
    unsigned int curdw = pScrni->displayWidth;
    PixmapPtr pPixmap;
    BOOL ret;

    pPixmap = pScrni->pScreen->GetScreenPixmap(pScrni->pScreen);
    pGeode->rotation = LXGetRotation(pScrni->pScreen);

    /* Leave if we have nothing to do */

    if (pGeode->rotation == curr && pGeode->curMode == mode) {
	return TRUE;
    }

    shadowRemove(pScrni->pScreen, NULL);

    LXSetRotatePitch(pScrni);

    if (pGeode->rotation != RR_Rotate_0) {

	ret = shadowAdd(pScrni->pScreen, pPixmap, LXUpdateFunc,
	    LXWindowLinear, pGeode->rotation, NULL);

	if (!ret)
	    goto error;
    }

    if (LXAllocShadow(pScrni))
	return TRUE;

  error:
    /* Restore the old rotation */
    pScrni->displayWidth = curdw;

    if (curr & (RR_Rotate_0 | RR_Rotate_180)) {
	pScrni->pScreen->width = pScrni->virtualX;
	pScrni->pScreen->height = pScrni->virtualY;
    } else {
	pScrni->pScreen->width = pScrni->virtualY;
	pScrni->pScreen->height = pScrni->virtualX;
    }

    pGeode->rotation = curr;
    return FALSE;
}
