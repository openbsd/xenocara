/*
 * Copyright 2008  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2008  Matthias Hopf <mhopf@novell.com>
 * Copyright 2008  Egbert Eich   <eich@novell.com>
 * Copyright 2008  Alex Deucher
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

/* for memory management */
#include "xaa.h"
#ifdef USE_EXA
#include "exa.h"
#endif

#include "xf86xv.h"

#include "rhd.h"
#include "rhd_cs.h"

#include "r5xx_regs.h"

/* for R5xx3DInit */
#include "r5xx_accel.h"

#include "rhd_video.h"

#include "xf86.h"
#include "dixstruct.h"
#include "xf86fbman.h"

#include <X11/extensions/Xv.h>
#include "fourcc.h"

#ifdef USE_EXA
/*
 *
 */
static Bool
rhdXvAllocateEXA(ScrnInfoPtr pScrn, struct RHDPortPriv *pPriv, int size)
{
    ExaOffscreenArea *area = pPriv->BufferHandle;

    if (area && (area->size == size))
	return TRUE;

    if (area)
	exaOffscreenFree(pScrn->pScreen, area);

    area = exaOffscreenAlloc(pScrn->pScreen, size, RHD_FB_ALIGNMENT,
			     TRUE, NULL, NULL);
    if (!area) {
    	pPriv->BufferHandle = NULL;
	pPriv->BufferOffset = 0;
	return FALSE;
    } else {
	pPriv->BufferHandle = area;
	pPriv->BufferOffset = area->offset + RHDPTR(pScrn)->FbScanoutStart;
	return TRUE;
    }
}
#endif /* USE_EXA */

/*
 *
 */
static FBLinearPtr
rhdXvAllocateXAAHelper(ScreenPtr pScreen, FBLinearPtr linear, int size)
{
    if (linear) {
	if (linear->size == size)
	    return linear;

	if (xf86ResizeOffscreenLinear(linear, size))
	    return linear;

	xf86FreeOffscreenLinear(linear);
    }

    linear = xf86AllocateOffscreenLinear(pScreen, size, 1,
					 NULL, NULL, NULL);

    if (!linear) {
	int max_size;

	xf86QueryLargestOffscreenLinear(pScreen, &max_size, 1,
					PRIORITY_EXTREME);
	if (max_size < size)
	    return NULL;

	xf86PurgeUnlockedOffscreenAreas(pScreen);
	linear = xf86AllocateOffscreenLinear(pScreen, size, 1,
					     NULL, NULL, NULL);
    }

    return linear;
}

/*
 *
 */
static Bool
rhdXvAllocateXAA(ScrnInfoPtr pScrn, struct RHDPortPriv *pPriv, int size)
{
    int cpp = pScrn->bitsPerPixel >> 3;
    FBLinearPtr linear;

    /* We need to  do FB alignment manually */
    size += RHD_FB_ALIGNMENT - 1;

    /* XAA allocates in units of pixels */
    size = (size + cpp - 1) / cpp;

    linear = rhdXvAllocateXAAHelper(pScrn->pScreen, pPriv->BufferHandle, size);
    if (!linear) {
	pPriv->BufferHandle = NULL;
	pPriv->BufferOffset = 0;
	return FALSE;
    } else {
	pPriv->BufferHandle = linear;
	pPriv->BufferOffset = RHDPTR(pScrn)->FbScanoutStart +
	    RHD_FB_CHUNK(linear->offset * cpp + RHD_FB_ALIGNMENT - 1);
	return TRUE;
    }
}

/*
 *
 */
static void
rhdStopVideo(ScrnInfoPtr pScrn, pointer data, Bool cleanup)
{
    if (cleanup) {
	struct RHDPortPriv *pPriv = data;

	switch (RHDPTR(pScrn)->AccelMethod) {
#ifdef USE_EXA
	case RHD_ACCEL_EXA:
	    exaOffscreenFree(pScrn->pScreen,
			     (ExaOffscreenArea *) pPriv->BufferHandle);
	    break;
#endif /* USE_EXA */
	case RHD_ACCEL_XAA:
	    xf86FreeOffscreenLinear((FBLinearPtr) pPriv->BufferHandle);
	    break;
	default:
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "%s: cannot de-allocate memory!\n", __func__);
	    break;
	}

	pPriv->BufferHandle = NULL;
	pPriv->BufferOffset = 0;
    }
}

/*
 *
 */
static int
rhdSetPortAttribute(ScrnInfoPtr  pScrn,
		       Atom	    attribute,
		       INT32	    value,
		       pointer	    data)
{
    return BadMatch;
}

/*
 *
 */
static int
rhdGetPortAttribute(ScrnInfoPtr  pScrn,
		       Atom	    attribute,
		       INT32	    *value,
		       pointer	    data)
{
    return BadMatch;
}

/*
 *
 */
static void
rhdQueryBestSize(
  ScrnInfoPtr pScrn,
  Bool motion,
  short vid_w, short vid_h,
  short drw_w, short drw_h,
  unsigned int *p_w, unsigned int *p_h,
  pointer data
){

    *p_w = drw_w;
    *p_h = drw_h;
}

/*
 *
 */
static int
rhdQueryImageAttributes(ScrnInfoPtr pScrn, int id, CARD16 *w, CARD16 *h,
			int *pitches, int *offsets)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    int size;

    if (!pitches || !offsets)
	return 0;

    *w = ALIGN(*w, 2);

    if ((rhdPtr->ChipSet == RHD_RS690) || (rhdPtr->ChipSet == RHD_RS600) ||
	(rhdPtr->ChipSet == RHD_RS740)) {
	if (*w > 2048)
	    *w = 2048;
	if (*h > 2048)
	    *h = 2048;
    } else {
	if (*w > 4096)
	    *w = 4096;
	if (*h > 4096)
	    *h = 4096;
    }

    switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	*h = ALIGN(*h, 2);

	offsets[0] = 0;

	if (pitches)
	    pitches[0] = ALIGN(*w, 4);
	size = *h * pitches[0];

	offsets[1] = *h * pitches[0];
	pitches[1] = ALIGN(*w / 2, 4);

	offsets[2] = *h * (pitches[0] + pitches[1] / 2);
	pitches[2] = pitches[1];

	size = *h * (pitches[0] + pitches[1]);
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	offsets[0] = 0;
	pitches[0] = 2 * *w;
	size = pitches[0] * *h;
	break;
    }

    return size;
}

/*
 * Buffer swaps for big endian.
 */
#if X_BYTE_ORDER == X_BIG_ENDIAN
static inline void
MemCopySwap32(CARD8 *dst, CARD8 *src, unsigned int size)
{
    unsigned int *d = (unsigned int *)dst;
    unsigned int *s = (unsigned int *)src;
    unsigned int nwords = size >> 2;

    for (; nwords > 0; --nwords, ++d, ++s)
#ifdef __powerpc__
	asm volatile("stwbrx %0,0,%1" : : "r" (*s), "r" (d));
#else
        *d = ((*s >> 24) & 0xff) | ((*s >> 8) & 0xff00)
	    | ((*s & 0xff00) << 8) | ((*s & 0xff) << 24);
#endif
}
#else
#define MemCopySwap32 memcpy
#endif /* X_BYTE_ORDER */

/*
 *
 */
static void
R5xxXvCopyPackedDMA(RHDPtr rhdPtr, CARD8 *src, CARD8 *dst,
		    CARD16 srcPitch, CARD16 dstPitch, CARD16 h)
{
    struct RhdCS *CS = rhdPtr->CS;
    CARD32 Offset = dst - (CARD8 *)rhdPtr->FbBase + rhdPtr->FbIntAddress;
    CARD32 Control = R5XX_GMC_DST_PITCH_OFFSET_CNTL |
	R5XX_GMC_DST_CLIPPING | R5XX_GMC_BRUSH_NONE |
	R5XX_GMC_DST_8BPP_CI | R5XX_GMC_SRC_DATATYPE_COLOR |
	R5XX_ROP3_S | R5XX_DP_SRC_SOURCE_HOST_DATA |
	R5XX_GMC_CLR_CMP_CNTL_DIS | R5XX_GMC_WR_MSK_DIS;
    CARD16 y = 0, dwords;
    CARD16 hpass = ((CS->Size - 10) * 4) / srcPitch;

    while (h) {
	if (h < hpass)
	    hpass = h;

	dwords = hpass * srcPitch / 4;

	RHDCSGrab(CS, dwords + 10);
	RHDCSWrite(CS, CP_PACKET3(R5XX_CP_PACKET3_CNTL_HOSTDATA_BLT,
				  dwords + 10 - 2));
	RHDCSWrite(CS, Control);
	RHDCSWrite(CS, (dstPitch << 16) | (Offset >> 10));
	RHDCSWrite(CS, y << 16);
	RHDCSWrite(CS, ((y + hpass) << 16) | srcPitch);
	RHDCSWrite(CS, 0xFFFFFFFF);
	RHDCSWrite(CS, 0xFFFFFFFF);
	RHDCSWrite(CS, y << 16);
	RHDCSWrite(CS, (hpass << 16) | srcPitch);
	RHDCSWrite(CS, dwords);

	MemCopySwap32((CARD8 *) &CS->Buffer[CS->Wptr], src, hpass * srcPitch);
	CS->Wptr += dwords;

	src += hpass * srcPitch;

	y += hpass;
	h -= hpass;
    }

    RHDCSFlush(CS);

    return;
}


/*
 *
 */
static void
R5xxXvCopyPacked(RHDPtr rhdPtr, CARD8 *src, CARD8 *dst,
		 CARD16 srcPitch, CARD16 dstPitch, CARD16 h)
{
#if X_BYTE_ORDER == X_BIG_ENDIAN
    CARD32 val, new;
    val = RHDRegRead(rhdPtr, R5XX_SURFACE_CNTL);
    new = val &
	~(R5XX_NONSURF_AP0_SWP_32BPP | R5XX_NONSURF_AP1_SWP_32BPP |
	  R5XX_NONSURF_AP0_SWP_16BPP | R5XX_NONSURF_AP1_SWP_16BPP);
    RHDRegWrite(rhdPtr, R5XX_SURFACE_CNTL, new);
#endif

    if (srcPitch == dstPitch)
	memcpy(dst, src, srcPitch * h);
    else {
	while (h--) {
	    memcpy(dst, src, srcPitch);
	    src += srcPitch;
	    dst += dstPitch;
	}
    }

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* restore byte swapping */
    RHDRegWrite(rhdPtr, R5XX_SURFACE_CNTL, val);
#endif
}

/*
 *
 */
static void
R5xxXvCopyPlanarToPacked(CARD8 *dst, CARD16 dstPitch,
			 CARD8 *src1, CARD16 src1Pitch,
			 CARD8 *src2, CARD16 src2Pitch,
			 CARD8 *src3, CARD16 width, CARD16 height)
{
    int i, j;

    for (i = 0; i < height; i++) {
	CARD32 *d = (CARD32 *) dst;
	CARD8 *s1 = src1;
	CARD8 *s2 = src2;
	CARD8 *s3 = src3;

	for (j = width / 2; j > 4; j -= 4) {
	    d[0] = s1[0] | (s1[1] << 16) | (s3[0] << 8) | (s2[0] << 24);
	    d[1] = s1[2] | (s1[3] << 16) | (s3[1] << 8) | (s2[1] << 24);
	    d[2] = s1[4] | (s1[5] << 16) | (s3[2] << 8) | (s2[2] << 24);
	    d[3] = s1[6] | (s1[7] << 16) | (s3[3] << 8) | (s2[3] << 24);
	    d += 4;
	    s2 += 4;
	    s3 += 4;
	    s1 += 8;
	}

	for (; j; j--) {
	    d[0] = s1[0] | (s1[1] << 16) | (s3[0] << 8) | (s2[0] << 24);
	    d++;
	    s2++;
	    s3++;
	    s1 += 2;
	}

	dst += dstPitch;
	src1 += src1Pitch;
	if (i & 1) {
	    src2 += src2Pitch;
	    src3 += src2Pitch;
	}
    }
}

/*
 * TODO: check big endian.
 */
static void
R5xxXvCopyPlanarDMA(RHDPtr rhdPtr, CARD8 *src1, CARD8 *src2, CARD8 *src3,
		    CARD8 *dst1, CARD16 srcPitch, CARD16 srcPitch2,
		    CARD16 dstPitch, CARD16 h, CARD16 w)
{
    struct RhdCS *CS = rhdPtr->CS;
    /* We need to make sure that our hpass is always even, so that we don't run
     * into trouble with R5xxXvCopyPlanarToPacked. */
    CARD16 hpass = (2 * (CS->Size - 10) / w) & ~0x01;
    CARD32 Offset = dst1 - (CARD8 *)rhdPtr->FbBase + rhdPtr->FbIntAddress;
    CARD32 Control = R5XX_GMC_DST_PITCH_OFFSET_CNTL |
	R5XX_GMC_DST_CLIPPING | R5XX_GMC_BRUSH_NONE |
	R5XX_GMC_DST_32BPP | R5XX_GMC_SRC_DATATYPE_COLOR |
	R5XX_ROP3_S | R5XX_DP_SRC_SOURCE_HOST_DATA |
	R5XX_GMC_CLR_CMP_CNTL_DIS | R5XX_GMC_WR_MSK_DIS;
    CARD16 y = 0, dwords;

    while (h) {
	if (h < hpass)
	    hpass = h;

	dwords = hpass * w / 2;

	RHDCSGrab(CS, dwords + 10);
	RHDCSWrite(CS, CP_PACKET3(R5XX_CP_PACKET3_CNTL_HOSTDATA_BLT, dwords + 10 - 2));
	RHDCSWrite(CS, Control);
	RHDCSWrite(CS, (dstPitch << 16) | (Offset >> 10));
	RHDCSWrite(CS, y << 16);
	RHDCSWrite(CS, ((y + hpass) << 16) | (w / 2));
	RHDCSWrite(CS, 0xFFFFFFFF);
	RHDCSWrite(CS, 0xFFFFFFFF);
	RHDCSWrite(CS, y << 16);
	RHDCSWrite(CS, (hpass << 16) | (w / 2));
	RHDCSWrite(CS, dwords);

	R5xxXvCopyPlanarToPacked((CARD8 *) &CS->Buffer[CS->Wptr], 2 * w, src1,
				 srcPitch, src2, srcPitch2, src3, w, hpass);
	CS->Wptr += dwords;

	src1 += hpass * srcPitch;
	src2 += hpass * srcPitch2 / 2;
	src3 += hpass * srcPitch2 / 2;

	y += hpass;
	h -= hpass;
    }

    RHDCSFlush(CS);
}

/*
 *
 */
static void
R5xxXvCopyPlanar(RHDPtr rhdPtr, CARD8 *src1, CARD8 *src2, CARD8 *src3,
		 CARD8 *dst1, CARD16 srcPitch, CARD16 srcPitch2,
		 CARD16 dstPitch, CARD16 h, CARD16 w)
{
#if X_BYTE_ORDER == X_BIG_ENDIAN
    CARD32 val = RHDRegRead(pScrn, R5XX_SURFACE_CNTL);
    RHDRegWrite(pScrn, R5XX_SURFACE_CNTL,
		(val | R5XX_NONSURF_AP0_SWP_32BPP) & ~R5XX_NONSURF_AP0_SWP_16BPP);
#endif

    R5xxXvCopyPlanarToPacked(dst1, dstPitch, src1, srcPitch,
			     src2, srcPitch2, src3, w, h);

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* restore byte swapping */
    RHDRegWrite(pScrn, R5XX_SURFACE_CNTL, val);
#endif
}

/*
 *
 */
static int
rhdPutImageTextured(ScrnInfoPtr pScrn,
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
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct RHDPortPriv *pPriv = data;
    CARD8 *FBBuf;

    /*
     * First, make sure we can render to the drawable.
     */
    if (pDraw->type == DRAWABLE_WINDOW)
	pPriv->pPixmap = (*pScrn->pScreen->GetWindowPixmap)((WindowPtr)pDraw);
    else
	pPriv->pPixmap = (PixmapPtr)pDraw;


#if defined(USE_EXA) && ((EXA_VERSION_MAJOR > 2) || (EXA_VERSION_MAJOR == 2 && EXA_VERSION_MINOR >= 1))
    if (rhdPtr->AccelMethod == RHD_ACCEL_EXA) {
	/* Force the pixmap into framebuffer so we can draw to it. */
	exaMoveInPixmap(pPriv->pPixmap);
    } else
#endif
    /*
     * TODO: Copy the pixmap into the FB ourselves!!!
     */
    if (((rhdPtr->AccelMethod != RHD_ACCEL_NONE) || (rhdPtr->AccelMethod != RHD_ACCEL_SHADOWFB)) &&
	(((char *)pPriv->pPixmap->devPrivate.ptr < ((char *)rhdPtr->FbBase + rhdPtr->FbScanoutStart)) ||
	 ((char *)pPriv->pPixmap->devPrivate.ptr >= ((char *)rhdPtr->FbBase + rhdPtr->FbMapSize)))) {
	/* If the pixmap wasn't in framebuffer, then we have no way to
	 * force it there. So, we simply refuse to draw and fail.
	 */
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: pixmap is not in Framebuffer!\n", __func__);
	return BadAlloc;
    }

    pPriv->pDraw = pDraw;

    /* The upload blit only supports multiples of 64 bytes */
    if (rhdPtr->CS->Type == RHD_CS_CPDMA)
	pPriv->BufferPitch = ALIGN(2 * width, 64);
    else
	pPriv->BufferPitch = ALIGN(2 * width, 16);

    /*
     * Now, find out whether we have enough memory available.
     */
    switch (rhdPtr->AccelMethod) {
#ifdef USE_EXA
    case RHD_ACCEL_EXA:
	rhdXvAllocateEXA(pScrn, pPriv, 2 * pPriv->BufferPitch * height);
	break;
#endif /* USE_EXA */
    case RHD_ACCEL_XAA:
	rhdXvAllocateXAA(pScrn, pPriv, 2 * pPriv->BufferPitch * height);
	break;
    default:
	pPriv->BufferHandle = NULL;
	pPriv->BufferOffset = 0;
	break;
    }

    if (!pPriv->BufferHandle) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: Failed to allocate framebuffer memory.\n", __func__);
	return BadAlloc;
    }

    /*
     * Now copy the buffer to the framebuffer, and convert to planar when necessary.
     */
    FBBuf = (CARD8 *)rhdPtr->FbBase + pPriv->BufferOffset;

    switch(id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	{
	    int srcPitch = (width + 3) & ~3;
	    int srcPitch2 = ((width >> 1) + 3) & ~3;
	    int s2offset = srcPitch * height;
	    int s3offset = s2offset + srcPitch2 * (height >> 1);

	    if (id == FOURCC_YV12) {
		if (rhdPtr->CS->Type == RHD_CS_CPDMA)
		    R5xxXvCopyPlanarDMA(rhdPtr, buf, buf + s2offset,
					buf + s3offset, FBBuf, srcPitch,
					srcPitch2, pPriv->BufferPitch,
					height, width);
		else
		    R5xxXvCopyPlanar(rhdPtr, buf, buf + s2offset,
				     buf + s3offset, FBBuf, srcPitch,
				     srcPitch2, pPriv->BufferPitch,
				     height, width);
	    } else {
		if (rhdPtr->CS->Type == RHD_CS_CPDMA)
		    R5xxXvCopyPlanarDMA(rhdPtr, buf, buf + s3offset,
					buf + s2offset, FBBuf, srcPitch,
					srcPitch2, pPriv->BufferPitch,
					height, width);
		else
		    R5xxXvCopyPlanar(rhdPtr, buf, buf + s3offset,
				     buf + s2offset, FBBuf, srcPitch,
				     srcPitch2, pPriv->BufferPitch,
				     height, width);
	    }
	}
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	if (rhdPtr->CS->Type == RHD_CS_CPDMA)
	    R5xxXvCopyPackedDMA(rhdPtr, buf, FBBuf, 2 * width,
				pPriv->BufferPitch, height);
	else
	    R5xxXvCopyPacked(rhdPtr, buf, FBBuf, 2 * width,
			     pPriv->BufferPitch, height);
	break;
    }

    /*
     * Update cliplist
     */
    if (!REGION_EQUAL(pScrn->pScreen, &pPriv->clip, clipBoxes))
	REGION_COPY(pScrn->pScreen, &pPriv->clip, clipBoxes);

    /*
     * Now let the 3D engine work its magic.
     */
    pPriv->id = id;
    pPriv->src_w = src_w;
    pPriv->src_h = src_h;
    pPriv->drw_x = drw_x;
    pPriv->drw_y = drw_y;
    pPriv->dst_w = drw_w;
    pPriv->dst_h = drw_h;
    pPriv->w = width;
    pPriv->h = height;

    RHDRADEONDisplayTexturedVideo(pScrn, pPriv);

    return Success;
}

/*
 * RS690, RS600, RS740 all have a maximum texture size of 2048x2048.
 * R500s quadruple this to 4096x4096.
 */
static XF86VideoEncodingRec DummyEncodingRS600[1] =
{
    { 0, "XV_IMAGE", 2048, 2048, {1, 1}}
};

static XF86VideoEncodingRec DummyEncodingR500[1] =
{
    { 0, "XV_IMAGE", 4096, 4096, {1, 1}}
};

#define NUM_FORMATS 3

static XF86VideoFormatRec Formats[NUM_FORMATS] =
{
    {15, TrueColor}, {16, TrueColor}, {24, TrueColor}
};

#define NUM_IMAGES 4

static XF86ImageRec Images[NUM_IMAGES] =
{
    XVIMAGE_YUY2,
    XVIMAGE_YV12,
    XVIMAGE_I420,
    XVIMAGE_UYVY
};

/*
 *
 */
static XF86VideoAdaptorPtr
rhdSetupImageTexturedVideo(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RHDPtr    rhdPtr = RHDPTR(pScrn);
    struct RHDPortPriv *pPortPriv;
    XF86VideoAdaptorPtr adapt;
    int i;
    int num_texture_ports = 16;

    RHDFUNC(pScrn);

    adapt = xnfcalloc(1, sizeof(XF86VideoAdaptorRec) + num_texture_ports *
		      (sizeof(struct RHDPortPriv) + sizeof(DevUnion)));

    adapt->type = XvWindowMask | XvInputMask | XvImageMask;
    adapt->flags = 0;
    adapt->name = "RadeonHD Textured Video";
    adapt->nEncodings = 1;

    if ((rhdPtr->ChipSet == RHD_RS690) || (rhdPtr->ChipSet == RHD_RS600) ||
	(rhdPtr->ChipSet == RHD_RS740))
	adapt->pEncodings = DummyEncodingRS600;
    else
	adapt->pEncodings = DummyEncodingR500;

    adapt->nFormats = NUM_FORMATS;
    adapt->pFormats = Formats;
    adapt->nPorts = num_texture_ports;
    adapt->pPortPrivates = (DevUnion*)(&adapt[1]);

    pPortPriv =
	(struct RHDPortPriv *)(&adapt->pPortPrivates[num_texture_ports]);

    adapt->nAttributes = 0;
    adapt->pAttributes = NULL;
    adapt->pImages = Images;
    adapt->nImages = NUM_IMAGES;
    adapt->PutVideo = NULL;
    adapt->PutStill = NULL;
    adapt->GetVideo = NULL;
    adapt->GetStill = NULL;
    adapt->StopVideo = rhdStopVideo;
    adapt->SetPortAttribute = rhdSetPortAttribute;
    adapt->GetPortAttribute = rhdGetPortAttribute;
    adapt->QueryBestSize = rhdQueryBestSize;
    adapt->PutImage = rhdPutImageTextured;
    adapt->ReputImage = NULL;
    adapt->QueryImageAttributes = rhdQueryImageAttributes;

    for (i = 0; i < num_texture_ports; i++) {
	struct RHDPortPriv *pPriv = &pPortPriv[i];

	/* gotta uninit this someplace, XXX: shouldn't be necessary for textured */
	REGION_NULL(pScreen, &pPriv->clip);

	adapt->pPortPrivates[i].ptr = (pointer) (pPriv);
    }

    return adapt;
}

/*
 *
 */
void
RHDInitVideo(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    XF86VideoAdaptorPtr *adaptors, *newAdaptors = NULL;
    XF86VideoAdaptorPtr texturedAdaptor = NULL;
    int num_adaptors;

    RHDFUNC(pScrn);

    num_adaptors = xf86XVListGenericAdaptors(pScrn, &adaptors);
    newAdaptors = xalloc((num_adaptors + 2) * sizeof(XF86VideoAdaptorPtr *));
    if (newAdaptors == NULL)
	return;

    memcpy(newAdaptors, adaptors, num_adaptors * sizeof(XF86VideoAdaptorPtr));
    adaptors = newAdaptors;

    if ((rhdPtr->ChipSet < RHD_R600) && rhdPtr->TwoDPrivate &&
	((rhdPtr->CS->Type == RHD_CS_CP) || (rhdPtr->CS->Type == RHD_CS_CPDMA))) {

	texturedAdaptor = rhdSetupImageTexturedVideo(pScreen);

	adaptors[num_adaptors++] = texturedAdaptor;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Xv: Textured Video initialised.\n");

	/* EXA could've initialised this already */
	if (!rhdPtr->ThreeDPrivate)
	    R5xx3DInit(pScrn);

    } else
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Xv: No Textured Video possible for %s.\n", pScrn->chipset);

    if (num_adaptors)
	xf86XVScreenInit(pScreen, adaptors, num_adaptors);

    if (newAdaptors)
	xfree(newAdaptors);

}
