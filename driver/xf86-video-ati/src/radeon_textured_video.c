/*
 * Copyright 2008 Alex Deucher
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
 *
 * Based on radeon_exa_render.c and kdrive ati_video.c by Eric Anholt, et al.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_probe.h"
#include "radeon_video.h"

#include <X11/extensions/Xv.h>
#include "fourcc.h"

#define IMAGE_MAX_WIDTH		2048
#define IMAGE_MAX_HEIGHT	2048

#define IMAGE_MAX_WIDTH_R500	4096
#define IMAGE_MAX_HEIGHT_R500	4096

static Bool
RADEONTilingEnabled(ScrnInfoPtr pScrn, PixmapPtr pPix)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

#ifdef USE_EXA
    if (info->useEXA) {
	if (info->tilingEnabled && exaGetPixmapOffset(pPix) == 0)
	    return TRUE;
	else
	    return FALSE;
    } else
#endif
	{
	    if (info->tilingEnabled && ((pPix->devPrivate.ptr - info->FB) == 0))
		return TRUE;
	    else
		return FALSE;
	}
}

static __inline__ uint32_t F_TO_DW(float val)
{
    union {
	float f;
	uint32_t l;
    } tmp;
    tmp.f = val;
    return tmp.l;
}

#define ACCEL_MMIO
#define VIDEO_PREAMBLE()	unsigned char *RADEONMMIO = info->MMIO
#define BEGIN_VIDEO(n)		RADEONWaitForFifo(pScrn, (n))
#define OUT_VIDEO_REG(reg, val)	OUTREG(reg, val)
#define OUT_VIDEO_REG_F(reg, val) OUTREG(reg, F_TO_DW(val))
#define FINISH_VIDEO()

#include "radeon_textured_videofuncs.c"

#undef ACCEL_MMIO
#undef VIDEO_PREAMBLE
#undef BEGIN_VIDEO
#undef OUT_VIDEO_REG
#undef FINISH_VIDEO

#ifdef XF86DRI

#define ACCEL_CP
#define VIDEO_PREAMBLE()						\
    RING_LOCALS;							\
    RADEONCP_REFRESH(pScrn, info)
#define BEGIN_VIDEO(n)		BEGIN_RING(2*(n))
#define OUT_VIDEO_REG(reg, val)	OUT_RING_REG(reg, val)
#define FINISH_VIDEO()		ADVANCE_RING()
#define OUT_VIDEO_RING_F(x) OUT_RING(F_TO_DW(x))

#include "radeon_textured_videofuncs.c"

#endif /* XF86DRI */

static int
RADEONPutImageTextured(ScrnInfoPtr pScrn,
		       short src_x, short src_y,
		       short drw_x, short drw_y,
		       short src_w, short src_h,
		       short drw_w, short drw_h,
		       int id,
		       unsigned char *buf,
		       short width,
		       short height,
		       Bool sync,
		       RegionPtr clipBoxes,
		       pointer data,
		       DrawablePtr pDraw)
{
    ScreenPtr pScreen = pScrn->pScreen;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONPortPrivPtr pPriv = (RADEONPortPrivPtr)data;
    INT32 x1, x2, y1, y2;
    int srcPitch, srcPitch2, dstPitch;
    int s2offset, s3offset, tmp;
    int top, left, npixels, nlines, size;
    BoxRec dstBox;
    int dst_width = width, dst_height = height;

    /* make the compiler happy */
    s2offset = s3offset = srcPitch2 = 0;

    /* Clip */
    x1 = src_x;
    x2 = src_x + src_w;
    y1 = src_y;
    y2 = src_y + src_h;

    dstBox.x1 = drw_x;
    dstBox.x2 = drw_x + drw_w;
    dstBox.y1 = drw_y;
    dstBox.y2 = drw_y + drw_h;

    if (!xf86XVClipVideoHelper(&dstBox, &x1, &x2, &y1, &y2, clipBoxes, width, height))
	return Success;

    src_w = (x2 - x1) >> 16;
    src_h = (y2 - y1) >> 16;
    drw_w = dstBox.x2 - dstBox.x1;
    drw_h = dstBox.y2 - dstBox.y1;

    if ((x1 >= x2) || (y1 >= y2))
	return Success;

    switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	dstPitch = ((dst_width << 1) + 15) & ~15;
	srcPitch = (width + 3) & ~3;
	srcPitch2 = ((width >> 1) + 3) & ~3;
	size = dstPitch * dst_height;
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	dstPitch = ((dst_width << 1) + 15) & ~15;
	srcPitch = (width << 1);
	srcPitch2 = 0;
	size = dstPitch * dst_height;
	break;
    }

#ifdef XF86DRI
   if (info->directRenderingEnabled && info->DMAForXv)
       /* The upload blit only supports multiples of 64 bytes */
       dstPitch = (dstPitch + 63) & ~63;
   else
#endif
       dstPitch = (dstPitch + 15) & ~15;

    if (pPriv->video_memory != NULL && size != pPriv->size) {
	RADEONFreeMemory(pScrn, pPriv->video_memory);
	pPriv->video_memory = NULL;
    }

    if (pPriv->video_memory == NULL) {
	pPriv->video_offset = RADEONAllocateMemory(pScrn,
						       &pPriv->video_memory,
						       size * 2);
	if (pPriv->video_offset == 0)
	    return BadAlloc;
    }

    if (pDraw->type == DRAWABLE_WINDOW)
	pPriv->pPixmap = (*pScreen->GetWindowPixmap)((WindowPtr)pDraw);
    else
	pPriv->pPixmap = (PixmapPtr)pDraw;

#ifdef USE_EXA
    if (info->useEXA) {
	/* Force the pixmap into framebuffer so we can draw to it. */
	exaMoveInPixmap(pPriv->pPixmap);
    }
#endif

    if (!info->useEXA &&
	(((char *)pPriv->pPixmap->devPrivate.ptr < (char *)info->FB) ||
	 ((char *)pPriv->pPixmap->devPrivate.ptr >= (char *)info->FB +
	  info->FbMapSize))) {
	/* If the pixmap wasn't in framebuffer, then we have no way in XAA to
	 * force it there. So, we simply refuse to draw and fail.
	 */
	return BadAlloc;
    }

    /* copy data */
    top = y1 >> 16;
    left = (x1 >> 16) & ~1;
    npixels = ((((x2 + 0xffff) >> 16) + 1) & ~1) - left;

    pPriv->src_offset = pPriv->video_offset + info->fbLocation + pScrn->fbOffset;
    pPriv->src_addr = (uint8_t *)(info->FB + pPriv->video_offset + (top * dstPitch));
    pPriv->src_pitch = dstPitch;
    pPriv->size = size;
    pPriv->pDraw = pDraw;

#if 0
    ErrorF("src_offset: 0x%x\n", pPriv->src_offset);
    ErrorF("src_addr: 0x%x\n", pPriv->src_addr);
    ErrorF("src_pitch: 0x%x\n", pPriv->src_pitch);
#endif

    switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	top &= ~1;
	nlines = ((((y2 + 0xffff) >> 16) + 1) & ~1) - top;
	s2offset = srcPitch * height;
	s3offset = (srcPitch2 * (height >> 1)) + s2offset;
	top &= ~1;
	pPriv->src_addr += left << 1;
	tmp = ((top >> 1) * srcPitch2) + (left >> 1);
	s2offset += tmp;
	s3offset += tmp;
	if (id == FOURCC_I420) {
	    tmp = s2offset;
	    s2offset = s3offset;
	    s3offset = tmp;
	}
	RADEONCopyMungedData(pScrn, buf + (top * srcPitch) + left,
			     buf + s2offset, buf + s3offset, pPriv->src_addr,
			     srcPitch, srcPitch2, dstPitch, nlines, npixels);
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	nlines = ((y2 + 0xffff) >> 16) - top;
	RADEONCopyData(pScrn, buf, pPriv->src_addr, srcPitch, dstPitch, nlines, npixels, 2);
	break;
    }

    /* update cliplist */
    if (!REGION_EQUAL(pScrn->pScreen, &pPriv->clip, clipBoxes)) {
	REGION_COPY(pScrn->pScreen, &pPriv->clip, clipBoxes);
    }

    pPriv->id = id;
    pPriv->src_w = src_w;
    pPriv->src_h = src_h;
    pPriv->drw_x = drw_x;
    pPriv->drw_y = drw_y;
    pPriv->dst_w = drw_w;
    pPriv->dst_h = drw_h;
    pPriv->w = width;
    pPriv->h = height;

#ifdef XF86DRI
    if (info->directRenderingEnabled)
	RADEONDisplayTexturedVideoCP(pScrn, pPriv);
    else
#endif
	RADEONDisplayTexturedVideoMMIO(pScrn, pPriv);

    return Success;
}

/* client libraries expect an encoding */
static XF86VideoEncodingRec DummyEncoding[1] =
{
    {
	0,
	"XV_IMAGE",
	IMAGE_MAX_WIDTH, IMAGE_MAX_HEIGHT,
	{1, 1}
    }
};

static XF86VideoEncodingRec DummyEncodingR500[1] =
{
    {
	0,
	"XV_IMAGE",
	IMAGE_MAX_WIDTH_R500, IMAGE_MAX_HEIGHT_R500,
	{1, 1}
    }
};

#define NUM_FORMATS 3

static XF86VideoFormatRec Formats[NUM_FORMATS] =
{
    {15, TrueColor}, {16, TrueColor}, {24, TrueColor}
};

#define NUM_ATTRIBUTES 0

static XF86AttributeRec Attributes[NUM_ATTRIBUTES] =
{
};

#define NUM_IMAGES 4

static XF86ImageRec Images[NUM_IMAGES] =
{
    XVIMAGE_YUY2,
    XVIMAGE_YV12,
    XVIMAGE_I420,
    XVIMAGE_UYVY
};

XF86VideoAdaptorPtr
RADEONSetupImageTexturedVideo(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr    info = RADEONPTR(pScrn);
    RADEONPortPrivPtr pPortPriv;
    XF86VideoAdaptorPtr adapt;
    int i;
    int num_texture_ports = 16;

    adapt = xcalloc(1, sizeof(XF86VideoAdaptorRec) + num_texture_ports *
		    (sizeof(RADEONPortPrivRec) + sizeof(DevUnion)));
    if (adapt == NULL)
	return NULL;

    adapt->type = XvWindowMask | XvInputMask | XvImageMask;
    adapt->flags = 0;
    adapt->name = "Radeon Textured Video";
    adapt->nEncodings = 1;
    if (IS_R500_3D)
	adapt->pEncodings = DummyEncodingR500;
    else
	adapt->pEncodings = DummyEncoding;
    adapt->nFormats = NUM_FORMATS;
    adapt->pFormats = Formats;
    adapt->nPorts = num_texture_ports;
    adapt->pPortPrivates = (DevUnion*)(&adapt[1]);

    pPortPriv =
	(RADEONPortPrivPtr)(&adapt->pPortPrivates[num_texture_ports]);

    adapt->nAttributes = NUM_ATTRIBUTES;
    adapt->pAttributes = Attributes;
    adapt->pImages = Images;
    adapt->nImages = NUM_IMAGES;
    adapt->PutVideo = NULL;
    adapt->PutStill = NULL;
    adapt->GetVideo = NULL;
    adapt->GetStill = NULL;
    adapt->StopVideo = RADEONStopVideo;
    adapt->SetPortAttribute = RADEONSetPortAttribute;
    adapt->GetPortAttribute = RADEONGetPortAttribute;
    adapt->QueryBestSize = RADEONQueryBestSize;
    adapt->PutImage = RADEONPutImageTextured;
    adapt->ReputImage = NULL;
    adapt->QueryImageAttributes = RADEONQueryImageAttributes;

    for (i = 0; i < num_texture_ports; i++) {
	RADEONPortPrivPtr pPriv = &pPortPriv[i];

	pPriv->textured = TRUE;
	pPriv->videoStatus = 0;
	pPriv->currentBuffer = 0;
	pPriv->doubleBuffer = 0;

	/* gotta uninit this someplace, XXX: shouldn't be necessary for textured */
	REGION_NULL(pScreen, &pPriv->clip);
	adapt->pPortPrivates[i].ptr = (pointer) (pPriv);
    }

    return adapt;
}

