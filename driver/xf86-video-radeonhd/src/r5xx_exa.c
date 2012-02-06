/*
 * Copyright 2005  Eric Anholt
 * Copyright 2005  Benjamin Herrenschmidt
 * Copyright 2006  Tungsten Graphics, Inc.
 * Copyright 2008  Luc Verhaegen <libv@exsuse.de>
 * Copyright 2008  Matthias Hopf <mhopf@novell.com>
 * Copyright 2008  Egbert Eich   <eich@novell.com>
 *
 * All Rights Reserved.
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
 *    Eric Anholt <anholt@FreeBSD.org>
 *    Zack Rusin <zrusin@trolltech.com>
 *    Benjamin Herrenschmidt <benh@kernel.crashing.org>
 *    Michel Dänzer <michel@tungstengraphics.com>
 *    Luc Verhaegen <libv@exsuse.de>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include "exa.h"

#if HAVE_XF86_ANSIC_H
# include "xf86_ansic.h"
#else
# include <string.h>
#endif

#include "rhd.h"
#include "rhd_cs.h"
#include "r5xx_accel.h"
#include "r5xx_regs.h"

extern struct R5xxRop R5xxRops[];

struct R5xxExaPrivate {
    int scrnIndex;

    int xdir;
    int ydir;

#ifdef USE_DRI
    /* For Download: claim an indirect buffer, as a scratch area for download. */
    CARD8 *Buffer;
    unsigned int BufferIntAddress;
    CARD32 BufferSize;
#endif

    int exaSyncMarker;
    int exaMarkerSynced;
#if X_BYTE_ORDER == X_BIG_ENDIAN
    unsigned long swapper_surfaces[6];
#endif /* X_BYTE_ORDER */
};

/*
 * Helpers.
 */
inline CARD8
R5xxEXADatatypeGet(int bitsPerPixel)
{
    switch (bitsPerPixel) {
    case 8:
	return R5XX_DATATYPE_CI8;
    case 16:
	return R5XX_DATATYPE_RGB565;
    case 32:
	return R5XX_DATATYPE_ARGB8888;
    default:
	return 0;
    }
}

#define R5XX_EXA_PITCH_CHECK(pitch) (((pitch) >= 0x4000) || ((pitch) & 0x003F))
#define R5XX_EXA_OFFSET_CHECK(offset) ((offset) & 0xFFF)
#define RHDPTRE(p) (RHDPTR(xf86Screens[(p)->myNum]))

/*
 *
 */
static int
R5xxEXAMarkSync(ScreenPtr pScreen)
{
    struct R5xxExaPrivate *ExaPrivate = RHDPTR(xf86Screens[pScreen->myNum])->TwoDPrivate;

    ExaPrivate->exaSyncMarker++;

    return ExaPrivate->exaSyncMarker;
}

/*
 *
 */
static void
R5xxEXASync(ScreenPtr pScreen, int marker)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    struct R5xxExaPrivate *ExaPrivate = RHDPTR(pScrn)->TwoDPrivate;

    if (ExaPrivate->exaMarkerSynced != marker) {
	struct RhdCS *CS = RHDPTR(pScrn)->CS;

	RHDCSFlush(CS);
	RHDCSIdle(CS);
	R5xx2DIdle(pScrn);

	ExaPrivate->exaMarkerSynced = marker;
    }
}

/*
 *
 */
static Bool
R5xxEXAPrepareSolid(PixmapPtr pPix, int alu, Pixel pm, Pixel fg)
{
    RHDPtr rhdPtr = RHDPTRE(pPix->drawable.pScreen);
    struct RhdCS *CS = rhdPtr->CS;
    CARD32 datatype, pitch, offset;

    datatype = R5xxEXADatatypeGet(pPix->drawable.bitsPerPixel);
    if (!datatype) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Unsupported bitdepth %d\n",
		   __func__, pPix->drawable.bitsPerPixel);
	return FALSE;
    }

    pitch = exaGetPixmapPitch(pPix);
    if (R5XX_EXA_PITCH_CHECK(pitch)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Invalid pitch: %d\n",
		   __func__, (unsigned int) pitch);
	return FALSE;
    }

    offset = exaGetPixmapOffset(pPix);
    if (R5XX_EXA_OFFSET_CHECK(offset)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Invalid offset: %d\n",
		   __func__, (unsigned int) offset);
	return FALSE;
    }
    offset += rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart;

    R5xxEngineWaitIdle3D(CS);

    RHDCSGrab(CS, 2 * 5);

    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL, R5XX_GMC_DST_PITCH_OFFSET_CNTL |
		  R5XX_GMC_BRUSH_SOLID_COLOR | (datatype << 8) |
		  R5XX_GMC_SRC_DATATYPE_COLOR | R5xxRops[alu].pattern |
		  R5XX_GMC_CLR_CMP_CNTL_DIS);
    RHDCSRegWrite(CS, R5XX_DP_BRUSH_FRGD_CLR, fg);
    RHDCSRegWrite(CS, R5XX_DP_WRITE_MASK, pm);
    RHDCSRegWrite(CS, R5XX_DP_CNTL,
		  R5XX_DST_X_LEFT_TO_RIGHT | R5XX_DST_Y_TOP_TO_BOTTOM);
    RHDCSRegWrite(CS, R5XX_DST_PITCH_OFFSET, (pitch << 16) | (offset >> 10));

    RHDCSAdvance(CS);

    return TRUE;
}

/*
 *
 */
static void
R5xxEXASolid(PixmapPtr pPix, int x1, int y1, int x2, int y2)
{
    struct RhdCS *CS = RHDPTRE(pPix->drawable.pScreen)->CS;

    RHDCSGrab(CS, 2 * 2);

    RHDCSRegWrite(CS, R5XX_DST_Y_X, (y1 << 16) | x1);
    RHDCSRegWrite(CS, R5XX_DST_HEIGHT_WIDTH, ((y2 - y1) << 16) | (x2 - x1));

    RHDCSAdvance(CS);
}

/*
 *
 */
static void
R5xxEXADoneSolid(PixmapPtr pPix)
{
    ;
}

/*
 *
 */
static Bool
R5xxEXAPrepareCopy(PixmapPtr pSrc, PixmapPtr pDst, int xdir, int ydir, int rop,
		   Pixel planemask)
{
    RHDPtr rhdPtr = RHDPTRE(pDst->drawable.pScreen);
    struct RhdCS *CS = rhdPtr->CS;
    struct R5xxExaPrivate *ExaPrivate = rhdPtr->TwoDPrivate;
    CARD32 datatype, srcpitch, srcoffset, dstpitch, dstoffset;

    ExaPrivate->xdir = xdir;
    ExaPrivate->ydir = ydir;

    datatype = R5xxEXADatatypeGet(pDst->drawable.bitsPerPixel);
    if (!datatype) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Unsupported bitdepth %d\n",
		   __func__, pDst->drawable.bitsPerPixel);
	return FALSE;
    }

    srcpitch = exaGetPixmapPitch(pSrc);
    if (R5XX_EXA_PITCH_CHECK(srcpitch)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Invalid source pitch: %d\n",
		   __func__, (unsigned int) srcpitch);
	return FALSE;
    }

    srcoffset = exaGetPixmapOffset(pSrc);
    if (R5XX_EXA_OFFSET_CHECK(srcoffset)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Invalid source offset: %d\n",
		   __func__, (unsigned int) srcoffset);
	return FALSE;
    }
    srcoffset += rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart;

    dstpitch = exaGetPixmapPitch(pDst);
    if (R5XX_EXA_PITCH_CHECK(dstpitch)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Invalid destination pitch: %d\n",
		   __func__, (unsigned int) dstpitch);
	return FALSE;
    }

    dstoffset = exaGetPixmapOffset(pDst);
    if (R5XX_EXA_OFFSET_CHECK(dstoffset)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Invalid destination offset: %d\n",
		   __func__, (unsigned int) dstoffset);
	return FALSE;
    }
    dstoffset += rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart;

    R5xxEngineWaitIdle3D(CS);

    RHDCSGrab(CS, 2 * 5);

    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL,
		  R5XX_GMC_DST_PITCH_OFFSET_CNTL | R5XX_GMC_SRC_PITCH_OFFSET_CNTL |
		  R5XX_GMC_BRUSH_NONE | (datatype << 8) |
		  R5XX_GMC_SRC_DATATYPE_COLOR | R5xxRops[rop].rop |
		  R5XX_DP_SRC_SOURCE_MEMORY | R5XX_GMC_CLR_CMP_CNTL_DIS);
    RHDCSRegWrite(CS, R5XX_DP_WRITE_MASK, planemask);
    RHDCSRegWrite(CS, R5XX_DP_CNTL,
		  ((xdir >= 0 ? R5XX_DST_X_LEFT_TO_RIGHT : 0) |
		   (ydir >= 0 ? R5XX_DST_Y_TOP_TO_BOTTOM : 0)));
    RHDCSRegWrite(CS, R5XX_DST_PITCH_OFFSET, (dstpitch << 16) | (dstoffset >> 10));
    RHDCSRegWrite(CS, R5XX_SRC_PITCH_OFFSET, (srcpitch << 16) | (srcoffset >> 10));

    RHDCSAdvance(CS);

    return TRUE;
}

/*
 *
 */
void
R5xxEXACopy(PixmapPtr pDst, int srcX, int srcY, int dstX, int dstY, int w, int h)
{
    RHDPtr rhdPtr = RHDPTRE(pDst->drawable.pScreen);
    struct RhdCS *CS = rhdPtr->CS;
    struct R5xxExaPrivate *ExaPrivate = rhdPtr->TwoDPrivate;

    if (ExaPrivate->xdir < 0) {
	srcX += w - 1;
	dstX += w - 1;
    }
    if (ExaPrivate->ydir < 0) {
	srcY += h - 1;
	dstY += h - 1;
    }

    RHDCSGrab(CS, 2 * 3);

    RHDCSRegWrite(CS, R5XX_SRC_Y_X, (srcY << 16) | srcX);
    RHDCSRegWrite(CS, R5XX_DST_Y_X, (dstY << 16) | dstX);
    RHDCSRegWrite(CS, R5XX_DST_HEIGHT_WIDTH, (h << 16) | w);

    RHDCSAdvance(CS);
}

/*
 *
 */
static void
R5xxEXADoneCopy(PixmapPtr pDst)
{
    ;
}

/*
 * Buffer swaps for big endian.
 */
#if X_BYTE_ORDER == X_BIG_ENDIAN
static inline void
R5xxCopySwap32(CARD8 *dst, CARD8 *src, unsigned int size)
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

static inline void
R5xxCopySwapHDW(CARD8 *dst, CARD8 *src, unsigned int size)
{
    unsigned int *d = (unsigned int *)dst;
    unsigned int *s = (unsigned int *)src;
    unsigned int nwords = size >> 2;

    for (; nwords > 0; --nwords, ++d, ++s)
	*d = ((*s & 0xffff) << 16) | ((*s >> 16) & 0xffff);
}

#ifdef USE_DRI
static inline void
R5xxCopySwap16(CARD8 *dst, CARD8 *src, unsigned int size)
{
    unsigned short *d = (unsigned short *)dst;
    unsigned short *s = (unsigned short *)src;
    unsigned int nwords = size >> 1;

    for (; nwords > 0; --nwords, ++d, ++s)
#ifdef __powerpc__
	asm volatile("stwbrx %0,0,%1" : : "r" (*s), "r" (d));
#else
        *d = ((*s >> 24) & 0xff) | ((*s >> 8) & 0xff00)
	    | ((*s & 0xff00) << 8) | ((*s & 0xff) << 24);
#endif
}
#endif /* USE_DRI */

#endif /* X_BYTE_ORDER */

/* Copies a single pass worth of data for a hostdata blit set up by
 * R5XXHostDataBlit().
 */
static inline void
R5xxBufCopyUpload(CARD8 *dst, CARD8 *src, unsigned int size, CARD8 bpp)
{
#if X_BYTE_ORDER == X_BIG_ENDIAN
    switch (bpp) {
    case 8:
	R5xxCopySwap32(dst, src, size);
	return;
    case 16:
	R5xxCopySwapHDW(dst, src, size);
	return;
    default:
	memcpy(dst, src, size);
	return;
    }
#else
    memcpy(dst, src, size);
#endif
}

/*
 *
 */
static Bool
R5xxEXAUploadToScreenCP(PixmapPtr pDst, int x, int y, int w, int h,
			char *src, int srcpitch)
{
    RHDPtr rhdPtr = RHDPTRE(pDst->drawable.pScreen);
    struct RhdCS *CS = rhdPtr->CS;
    CARD32 hpass, datatype, dwords;
    CARD32 bufpitch, dstpitch, dstoffset;

    /* Why does EXA even bother to pass us this? */
    if (!w || !h || !srcpitch)
	return FALSE;

    datatype = R5xxEXADatatypeGet(pDst->drawable.bitsPerPixel);
    if (!datatype) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Unsupported bitdepth %d\n",
		   __func__, pDst->drawable.bitsPerPixel);
	return FALSE;
    }

    bufpitch = ((w * pDst->drawable.bitsPerPixel / 8) + 3) & ~3;
    hpass = ((CS->Size - 10) * 4) / bufpitch;

    dstpitch = exaGetPixmapPitch(pDst);
    if (R5XX_EXA_PITCH_CHECK(dstpitch)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Invalid destination pitch: %d\n",
		   __func__, (unsigned int) dstpitch);
	return FALSE;
    }

    dstoffset = exaGetPixmapOffset(pDst);
    if (R5XX_EXA_OFFSET_CHECK(dstoffset)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Invalid destination offset: %d\n",
		   __func__, (unsigned int) dstoffset);
	return FALSE;
    }
    dstoffset += rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart;

    R5xxEngineWaitIdle3D(CS);

    for (; h; ) {
	hpass = min((unsigned int) h, hpass);
	dwords = hpass * bufpitch / 4;

	RHDCSGrab(CS, dwords + 10);
	RHDCSWrite(CS, CP_PACKET3(R5XX_CP_PACKET3_CNTL_HOSTDATA_BLT, dwords + 10 - 2));
	RHDCSWrite(CS, R5XX_GMC_DST_PITCH_OFFSET_CNTL |
		   R5XX_GMC_DST_CLIPPING | R5XX_GMC_BRUSH_NONE |
		   (datatype << 8) | R5XX_GMC_SRC_DATATYPE_COLOR |
		   R5XX_ROP3_S | R5XX_DP_SRC_SOURCE_HOST_DATA |
		   R5XX_GMC_CLR_CMP_CNTL_DIS | R5XX_GMC_WR_MSK_DIS);
	RHDCSWrite(CS, (dstpitch << 16) | (dstoffset >> 10));
	RHDCSWrite(CS, (y << 16) | x );
	RHDCSWrite(CS, ((y + hpass) << 16) | (x + w) );
	RHDCSWrite(CS, 0xffffffff );
	RHDCSWrite(CS, 0xffffffff );
	RHDCSWrite(CS, (y << 16) | x );
	RHDCSWrite(CS, (hpass << 16) | ((bufpitch * 8) / pDst->drawable.bitsPerPixel) );
	RHDCSWrite(CS, dwords );

	/* now copy over the data, while doing pitch conversion */
	if (bufpitch == (CARD32) srcpitch)
	    R5xxBufCopyUpload((CARD8 *) (CS->Buffer + CS->Wptr), (CARD8 *)src,
			      hpass * srcpitch, pDst->drawable.bitsPerPixel);
	else {
	    CARD8 *SrcBuf = (CARD8 *) src;
	    CARD8 *DstBuf = (CARD8 *) (CS->Buffer + CS->Wptr);
	    unsigned int i;

	    for (i = 0; i < hpass; i++) {
		R5xxBufCopyUpload(DstBuf, SrcBuf, bufpitch,
			    pDst->drawable.bitsPerPixel);
		SrcBuf += srcpitch;
		DstBuf += bufpitch;
	    }
	}

	CS->Wptr += dwords;
	RHDCSFlush(CS);

	src += hpass * srcpitch;
	y += hpass;
	h -= hpass;
    }

    exaMarkSync(pDst->drawable.pScreen);
    return TRUE;
}

/*
 *
 */
static Bool
R5xxEXAUploadToScreenManual(PixmapPtr pDst, int x, int y, int w, int h,
			    char *src, int src_pitch)
{
    RHDPtr rhdPtr = RHDPTRE(pDst->drawable.pScreen);
    CARD8 *dst = ((CARD8 *) rhdPtr->FbBase) +
	rhdPtr->FbScanoutStart + exaGetPixmapOffset(pDst);
    int dst_pitch = exaGetPixmapPitch(pDst);

    if (pDst->drawable.bitsPerPixel < 8)
	return FALSE;

    /* Do we need that sync here ? probably not .... */
    exaWaitSync(pDst->drawable.pScreen);

    w *= pDst->drawable.bitsPerPixel / 8;
    dst += (x * pDst->drawable.bitsPerPixel / 8) + (y * dst_pitch);

    while (h--) {
	memcpy(dst, src, w);
	src += src_pitch;
	dst += dst_pitch;
    }

    return TRUE;
}

/*
 *
 */
static Bool
R5xxEXADownloadFromScreenManual(PixmapPtr pSrc, int x, int y, int w, int h,
				char *dst, int dst_pitch)
{
    RHDPtr rhdPtr = RHDPTRE(pSrc->drawable.pScreen);
    CARD8 *src = ((CARD8 *) rhdPtr->FbBase) +
	rhdPtr->FbScanoutStart + exaGetPixmapOffset(pSrc);
    int	src_pitch = exaGetPixmapPitch(pSrc);

    /* Can't accelerate download */
    exaWaitSync(pSrc->drawable.pScreen);

    src += (x * pSrc->drawable.bitsPerPixel / 8) + (y * src_pitch);
    w *= pSrc->drawable.bitsPerPixel / 8;

    while (h--) {
	memcpy(dst, src, w);
	src += src_pitch;
	dst += dst_pitch;
    }

    return TRUE;
}

#ifdef USE_DRI
/*
 *
 */
static inline void
R5xxBufCopyDownload(CARD8 *dst, CARD8 *src, unsigned int size, CARD8 bpp)
{
#if X_BYTE_ORDER == X_BIG_ENDIAN
    switch (bpp) {
    case 16:
	R5xxCopySwap16(dst, src, size);
	return;
    case 32:
	R5xxCopySwap32(dst, src, size);
	return;
    default:
	memcpy(dst, src, size);
	return;
    }
#else
    memcpy(dst, src, size);
#endif
}

/*
 * Emit blit with arbitrary source and destination offsets and pitches
 */
static inline void
R5xxEXADownloadBlit(struct RhdCS *CS, CARD32 datatype,
		    CARD32 srcPitch, CARD32 srcOffset,
		    CARD32 dstPitch, CARD32 dstOffset,
		    int srcX, int srcY, int w, int h)
{
    RHDCSGrab(CS, 2 * 6);

    RHDCSRegWrite(CS, R5XX_DP_GUI_MASTER_CNTL,
		  R5XX_GMC_DST_PITCH_OFFSET_CNTL |
		  R5XX_GMC_SRC_PITCH_OFFSET_CNTL |
		  R5XX_GMC_BRUSH_NONE |
		  (datatype << 8) |
		  R5XX_GMC_SRC_DATATYPE_COLOR |
		  R5XX_ROP3_S |
		  R5XX_DP_SRC_SOURCE_MEMORY |
		  R5XX_GMC_CLR_CMP_CNTL_DIS |
		  R5XX_GMC_WR_MSK_DIS);
    RHDCSRegWrite(CS, R5XX_SRC_PITCH_OFFSET, (srcPitch << 16) | (srcOffset >> 10));
    RHDCSRegWrite(CS, R5XX_DST_PITCH_OFFSET, (dstPitch << 16) | (dstOffset >> 10));
    RHDCSRegWrite(CS, R5XX_SRC_Y_X, (srcY << 16) | srcX);
    RHDCSRegWrite(CS, R5XX_DST_Y_X, 0);
    RHDCSRegWrite(CS, R5XX_DST_HEIGHT_WIDTH, (h << 16) | w);

    RHDCSFlush(CS);
}

/*
 * Blit from the framebuffer into a separate indirect buffer, and then copy
 * this data out of the gart space into the destination pixmap.
 */
static Bool
R5xxEXADownloadFromScreenCP(PixmapPtr pSrc, int x, int y, int w, int h,
			    char *dst, int dstpitch)
{
    ScrnInfoPtr pScrn = xf86Screens[pSrc->drawable.pScreen->myNum];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct RhdCS *CS = rhdPtr->CS;
    struct R5xxExaPrivate *ExaPrivate = rhdPtr->TwoDPrivate;
    CARD32 datatype, srcpitch, srcoffset;
    CARD32 BufferPitch;
    CARD32 wpass, hpass;

    /* maybe we shouldn't bother to idle the engine for tiny ones. */
    if ((w * h * (pSrc->drawable.bitsPerPixel / 8)) < 64)
	return R5xxEXADownloadFromScreenManual(pSrc, x, y, w, h, dst, dstpitch);

    datatype = R5xxEXADatatypeGet(pSrc->drawable.bitsPerPixel);
    if (!datatype) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Unsupported bitdepth %d\n",
		   __func__, pSrc->drawable.bitsPerPixel);
	return FALSE;
    }

    srcpitch = exaGetPixmapPitch(pSrc);
    if (R5XX_EXA_PITCH_CHECK(srcpitch)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Invalid source pitch: %d\n",
		   __func__, (unsigned int) srcpitch);
	return FALSE;
    }

    srcoffset = exaGetPixmapOffset(pSrc);
    if (R5XX_EXA_OFFSET_CHECK(srcoffset)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: Invalid source offset: %d\n",
		   __func__, (unsigned int) srcoffset);
	return FALSE;
    }
    srcoffset += rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart;

    wpass = w * pSrc->drawable.bitsPerPixel / 8;
    BufferPitch = (wpass + 63) & ~63;
    hpass = ExaPrivate->BufferSize / BufferPitch;

    R5xxEngineWaitIdle3D(CS);

    while (h) {
	hpass = min((unsigned int) h, hpass);
	R5xxEXADownloadBlit(CS, datatype, srcpitch, srcoffset, BufferPitch,
			    ExaPrivate->BufferIntAddress, x, y, w, hpass);
	y += hpass;
	h -= hpass;

	/* this is quite a big hammer, but we have no other option here */
	RHDCSFlush(CS);
	RHDCSIdle(CS);
	R5xx2DIdle(pScrn);

	/* Copy out data from previous blit */
	if ((wpass == BufferPitch) && (wpass == (unsigned int) dstpitch)) {
	    R5xxBufCopyDownload((CARD8*)dst, ExaPrivate->Buffer, wpass * hpass,
				pSrc->drawable.bitsPerPixel);
	    dst += dstpitch * hpass;
	} else {
	    CARD8 *buf = ExaPrivate->Buffer;
	    unsigned int i;

	    for (i = 0; i < hpass; i++) {
		R5xxBufCopyDownload((CARD8*)dst, buf, wpass,
				    pSrc->drawable.bitsPerPixel);
		buf += BufferPitch;
		dst += dstpitch;
	    }
	}
    }

    /* since we had a full idle every time, we make sure we don't do
       yet another system call here */
    ExaPrivate->exaMarkerSynced = ExaPrivate->exaSyncMarker;

    return TRUE;
}
#endif /* USE_DRI */

#if X_BYTE_ORDER == X_BIG_ENDIAN

/*
 * offset will undoubtedly have to be adapted to the moving scanout + offscreen.
 */
static Bool
R5xxEXAPrepareAccess(PixmapPtr pPix, int index)
{
    ScrnInfoPtr pScrn = xf86Screens[pPix->drawable.pScreen->myNum];
    struct R5xxExaPrivate *ExaPrivate = RHDPTR(pScrn)->TwoDPrivate;
    CARD32 offset = exaGetPixmapOffset(pPix);
    int soff;
    CARD32 size, flags;

    /* Front buffer is always set with proper swappers */
    if (offset == 0)
        return TRUE;

    /* If same bpp as front buffer, just do nothing as the main
     * swappers will apply
     */
    if (pPix->drawable.bitsPerPixel == pScrn->bitsPerPixel)
        return TRUE;

    /* We need to setup a separate swapper, let's request a
     * surface. We need to align the size first
     */
    size = RHD_FB_CHUNK(exaGetPixmapSize(pPix));

    /* Set surface to tiling disabled with appropriate swapper */
    switch (pPix->drawable.bitsPerPixel) {
    case 16:
        flags = R5XX_SURF_AP0_SWP_16BPP | R5XX_SURF_AP1_SWP_16BPP;
	break;
    case 32:
        flags = R5XX_SURF_AP0_SWP_32BPP | R5XX_SURF_AP1_SWP_32BPP;
	break;
    default:
        flags = 0;
    }

    soff = (index + 1) * 0x10;
    RHDRegWrite(pScrn, R5XX_SURFACE0_INFO + soff, flags);
    RHDRegWrite(pScrn, R5XX_SURFACE0_LOWER_BOUND + soff, offset);
    RHDRegWrite(pScrn, R5XX_SURFACE0_UPPER_BOUND + soff, offset + size - 1);
    ExaPrivate->swapper_surfaces[index] = offset;
    return TRUE;
}

/*
 *
 */
static void
R5xxEXAFinishAccess(PixmapPtr pPix, int index)
{
    RHDPtr rhdPtr = RHDPTRE(pPix->drawable.pScreen);
    struct R5xxExaPrivate *ExaPrivate = rhdPtr->TwoDPrivate;
    CARD32 offset = exaGetPixmapOffset(pPix);
    int soff;

    /* Front buffer is always set with proper swappers */
    if (offset == 0)
        return;

    if (ExaPrivate->swapper_surfaces[index] == 0)
        return;

    soff = (index + 1) * 0x10;
    RHDRegWrite(rhdPtr, R5XX_SURFACE0_INFO + soff, 0);
    RHDRegWrite(rhdPtr, R5XX_SURFACE0_LOWER_BOUND + soff, 0);
    RHDRegWrite(rhdPtr, R5XX_SURFACE0_UPPER_BOUND + soff, 0);
    ExaPrivate->swapper_surfaces[index] = 0;
}

#endif /* X_BYTE_ORDER == X_BIG_ENDIAN */

/*
 *
 */
Bool
R5xxEXAInit(ScrnInfoPtr pScrn, ScreenPtr pScreen)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct RhdCS *CS = rhdPtr->CS;
    ExaDriverRec *EXAInfo;
    struct R5xxExaPrivate *ExaPrivate;

    RHDFUNC(pScrn);

    EXAInfo = exaDriverAlloc();
    if (EXAInfo == NULL)
	return FALSE;

    ExaPrivate = xnfcalloc(1, sizeof(struct R5xxExaPrivate));
    ExaPrivate->scrnIndex = pScrn->scrnIndex;

    EXAInfo->exa_major = EXA_VERSION_MAJOR;
    EXAInfo->exa_minor = EXA_VERSION_MINOR;

    EXAInfo->flags = EXA_OFFSCREEN_PIXMAPS
#ifdef EXA_SUPPORTS_PREPARE_AUX
	| EXA_SUPPORTS_PREPARE_AUX
#endif
	;
    EXAInfo->pixmapOffsetAlign = 0x1000;
    EXAInfo->pixmapPitchAlign = 64;

#if EXA_VERSION_MAJOR > 2 || (EXA_VERSION_MAJOR == 2 && EXA_VERSION_MINOR >= 3)
    EXAInfo->maxPitchBytes = 16320;
    EXAInfo->maxX = 8192;
#else
    EXAInfo->maxX = 16320 / 4;
#endif
    EXAInfo->maxY = 8192;

    EXAInfo->memoryBase = (CARD8 *) rhdPtr->FbBase + rhdPtr->FbScanoutStart;
    EXAInfo->offScreenBase = rhdPtr->FbOffscreenStart - rhdPtr->FbScanoutStart;
    EXAInfo->memorySize = rhdPtr->FbScanoutSize + rhdPtr->FbOffscreenSize;

    EXAInfo->PrepareSolid = R5xxEXAPrepareSolid;
    EXAInfo->Solid = R5xxEXASolid;
    EXAInfo->DoneSolid = R5xxEXADoneSolid;

    EXAInfo->PrepareCopy = R5xxEXAPrepareCopy;
    EXAInfo->Copy = R5xxEXACopy;
    EXAInfo->DoneCopy = R5xxEXADoneCopy;

    EXAInfo->MarkSync = R5xxEXAMarkSync;
    EXAInfo->WaitMarker = R5xxEXASync;

    if (CS->Type == RHD_CS_CPDMA)
	EXAInfo->UploadToScreen = R5xxEXAUploadToScreenCP;
    else
	EXAInfo->UploadToScreen = R5xxEXAUploadToScreenManual;

#ifdef USE_DRI
    if (CS->Type == RHD_CS_CPDMA) {
	ExaPrivate->Buffer =
	    RHDDRMIndirectBufferGet(CS->scrnIndex, &ExaPrivate->BufferIntAddress,
				    &ExaPrivate->BufferSize);
	if (ExaPrivate->Buffer)
	    EXAInfo->DownloadFromScreen = R5xxEXADownloadFromScreenCP;
	else {
	    xf86DrvMsg(CS->scrnIndex, X_INFO,
		       "Failed to get an indirect buffer for fast download.\n");
	    EXAInfo->DownloadFromScreen = R5xxEXADownloadFromScreenManual;
	}
    } else
#endif /* USE_DRI */
	EXAInfo->DownloadFromScreen = R5xxEXADownloadFromScreenManual;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    EXAInfo->PrepareAccess = R5xxEXAPrepareAccess;
    EXAInfo->FinishAccess = R5xxEXAFinishAccess;
#endif /* X_BYTE_ORDER == X_BIG_ENDIAN */

    /* if we have CP, we can use composite! */
    if (CS->Type != RHD_CS_MMIO)
	R5xxExaCompositeFuncs(pScrn->scrnIndex, EXAInfo);

    if (!exaDriverInit(pScreen, EXAInfo)) {
#ifdef USE_DRI
	if (ExaPrivate->Buffer)
	    RHDDRMIndirectBufferDiscard(CS->scrnIndex, ExaPrivate->Buffer);
#endif
	xfree(ExaPrivate);
	xfree(EXAInfo);
	return FALSE;
    }

    RHDPTR(pScrn)->EXAInfo = EXAInfo;

    rhdPtr->TwoDPrivate = ExaPrivate;

    /* if we have CP, we need set up the ThreeDPrivate */
    if ((CS->Type != RHD_CS_MMIO) && !rhdPtr->ThreeDPrivate)
	R5xx3DInit(pScrn);

    exaMarkSync(pScreen);

    return TRUE;
}

/*
 *
 */
void
R5xxEXACloseScreen(ScreenPtr pScreen)
{
    exaDriverFini(pScreen);
}

/*
 *
 */
void
R5xxEXADestroy(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);

    if (rhdPtr->EXAInfo) {
	xfree(rhdPtr->EXAInfo);
	rhdPtr->EXAInfo = NULL;
    }

    if (rhdPtr->TwoDPrivate) {
#ifdef USE_DRI
	struct R5xxExaPrivate *ExaPrivate = rhdPtr->TwoDPrivate;

	if (ExaPrivate->Buffer)
	    RHDDRMIndirectBufferDiscard(rhdPtr->scrnIndex, ExaPrivate->Buffer);
#endif

	xfree(rhdPtr->TwoDPrivate);
	rhdPtr->TwoDPrivate = NULL;
    }
}
