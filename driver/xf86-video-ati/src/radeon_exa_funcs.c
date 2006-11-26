/*
 * Copyright 2005 Eric Anholt
 * Copyright 2005 Benjamin Herrenschmidt
 * Copyright 2006 Tungsten Graphics, Inc.
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
 *
 */

#if defined(ACCEL_MMIO) && defined(ACCEL_CP)
#error Cannot define both MMIO and CP acceleration!
#endif

#if !defined(UNIXCPP) || defined(ANSICPP)
#define FUNC_NAME_CAT(prefix,suffix) prefix##suffix
#else
#define FUNC_NAME_CAT(prefix,suffix) prefix/**/suffix
#endif

#ifdef ACCEL_MMIO
#define FUNC_NAME(prefix) FUNC_NAME_CAT(prefix,MMIO)
#else
#ifdef ACCEL_CP
#define FUNC_NAME(prefix) FUNC_NAME_CAT(prefix,CP)
#else
#error No accel type defined!
#endif
#endif

#include <errno.h>
#include <string.h>

#include "radeon.h"
#include "atidri.h"

#include "exa.h"

static void
FUNC_NAME(RADEONSync)(ScreenPtr pScreen, int marker)
{
    TRACE;

    FUNC_NAME(RADEONWaitForIdle)(xf86Screens[pScreen->myNum]);

    RADEONPTR(xf86Screens[pScreen->myNum])->engineMode = EXA_ENGINEMODE_UNKNOWN;
}

static Bool
FUNC_NAME(RADEONPrepareSolid)(PixmapPtr pPix, int alu, Pixel pm, Pixel fg)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);
    CARD32 datatype, dst_pitch_offset;
    ACCEL_PREAMBLE();

    TRACE;

    if (pPix->drawable.bitsPerPixel == 24)
	RADEON_FALLBACK(("24bpp unsupported\n"));
    if (!RADEONGetDatatypeBpp(pPix->drawable.bitsPerPixel, &datatype))
	RADEON_FALLBACK(("RADEONGetDatatypeBpp failed\n"));
    if (!RADEONGetPixmapOffsetPitch(pPix, &dst_pitch_offset))
	RADEON_FALLBACK(("RADEONGetPixmapOffsetPitch failed\n"));

    RADEON_SWITCH_TO_2D();

    BEGIN_ACCEL(5);
    OUT_ACCEL_REG(RADEON_DP_GUI_MASTER_CNTL,
	    RADEON_GMC_DST_PITCH_OFFSET_CNTL |
	    RADEON_GMC_BRUSH_SOLID_COLOR |
	    (datatype << 8) |
	    RADEON_GMC_SRC_DATATYPE_COLOR |
	    RADEON_ROP[alu].pattern |
	    RADEON_GMC_CLR_CMP_CNTL_DIS);
    OUT_ACCEL_REG(RADEON_DP_BRUSH_FRGD_CLR, fg);
    OUT_ACCEL_REG(RADEON_DP_WRITE_MASK, pm);
    OUT_ACCEL_REG(RADEON_DP_CNTL,
	(RADEON_DST_X_LEFT_TO_RIGHT | RADEON_DST_Y_TOP_TO_BOTTOM));
    OUT_ACCEL_REG(RADEON_DST_PITCH_OFFSET, dst_pitch_offset);
    FINISH_ACCEL();

    return TRUE;
}


static void
FUNC_NAME(RADEONSolid)(PixmapPtr pPix, int x1, int y1, int x2, int y2)
{

    RINFO_FROM_SCREEN(pPix->drawable.pScreen);
    ACCEL_PREAMBLE();

    TRACE;

    BEGIN_ACCEL(2);
    OUT_ACCEL_REG(RADEON_DST_Y_X, (y1 << 16) | x1);
    OUT_ACCEL_REG(RADEON_DST_HEIGHT_WIDTH, ((y2 - y1) << 16) | (x2 - x1));
    FINISH_ACCEL();
}

static void
FUNC_NAME(RADEONDoneSolid)(PixmapPtr pPix)
{
    TRACE;
}

static Bool
FUNC_NAME(RADEONPrepareCopy)(PixmapPtr pSrc,   PixmapPtr pDst,
			     int xdir, int ydir,
			     int rop,
			     Pixel planemask)
{
    RINFO_FROM_SCREEN(pDst->drawable.pScreen);
    CARD32 datatype, src_pitch_offset, dst_pitch_offset;
    ACCEL_PREAMBLE();

    TRACE;

    info->xdir = xdir;
    info->ydir = ydir;

    if (pDst->drawable.bitsPerPixel == 24)
	RADEON_FALLBACK(("24bpp unsupported"));
    if (!RADEONGetDatatypeBpp(pDst->drawable.bitsPerPixel, &datatype))
	RADEON_FALLBACK(("RADEONGetDatatypeBpp failed\n"));
    if (!RADEONGetPixmapOffsetPitch(pSrc, &src_pitch_offset))
	RADEON_FALLBACK(("RADEONGetPixmapOffsetPitch source failed\n"));
    if (!RADEONGetPixmapOffsetPitch(pDst, &dst_pitch_offset))
	RADEON_FALLBACK(("RADEONGetPixmapOffsetPitch dest failed\n"));

    RADEON_SWITCH_TO_2D();

    BEGIN_ACCEL(5);
    OUT_ACCEL_REG(RADEON_DP_GUI_MASTER_CNTL,
	RADEON_GMC_DST_PITCH_OFFSET_CNTL |
	RADEON_GMC_SRC_PITCH_OFFSET_CNTL |
	RADEON_GMC_BRUSH_NONE |
	(datatype << 8) |
	RADEON_GMC_SRC_DATATYPE_COLOR |
	RADEON_ROP[rop].rop |
	RADEON_DP_SRC_SOURCE_MEMORY |
	RADEON_GMC_CLR_CMP_CNTL_DIS);
    OUT_ACCEL_REG(RADEON_DP_WRITE_MASK, planemask);
    OUT_ACCEL_REG(RADEON_DP_CNTL,
	((xdir >= 0 ? RADEON_DST_X_LEFT_TO_RIGHT : 0) |
	 (ydir >= 0 ? RADEON_DST_Y_TOP_TO_BOTTOM : 0)));
    OUT_ACCEL_REG(RADEON_DST_PITCH_OFFSET, dst_pitch_offset);
    OUT_ACCEL_REG(RADEON_SRC_PITCH_OFFSET, src_pitch_offset);
    FINISH_ACCEL();

    return TRUE;
}

static void
FUNC_NAME(RADEONCopy)(PixmapPtr pDst,
		      int srcX, int srcY,
		      int dstX, int dstY,
		      int w, int h)
{

    RINFO_FROM_SCREEN(pDst->drawable.pScreen);
    ACCEL_PREAMBLE();

    TRACE;

    if (info->xdir < 0) {
	srcX += w - 1;
	dstX += w - 1;
    }
    if (info->ydir < 0) {
	srcY += h - 1;
	dstY += h - 1;
    }

    BEGIN_ACCEL(3);

    OUT_ACCEL_REG(RADEON_SRC_Y_X,	   (srcY << 16) | srcX);
    OUT_ACCEL_REG(RADEON_DST_Y_X,	   (dstY << 16) | dstX);
    OUT_ACCEL_REG(RADEON_DST_HEIGHT_WIDTH, (h  << 16) | w);

    FINISH_ACCEL();
}

static void
FUNC_NAME(RADEONDoneCopy)(PixmapPtr pDst)
{
    TRACE;
}

static Bool
FUNC_NAME(RADEONUploadToScreen)(PixmapPtr pDst, int x, int y, int w, int h,
				char *src, int src_pitch)
{
#if X_BYTE_ORDER == X_BIG_ENDIAN || defined(ACCEL_CP)
    RINFO_FROM_SCREEN(pDst->drawable.pScreen);
#endif
    CARD8	   *dst	     = pDst->devPrivate.ptr;
    unsigned int   dst_pitch = exaGetPixmapPitch(pDst);
    unsigned int   bpp	     = pDst->drawable.bitsPerPixel;
#ifdef ACCEL_CP
    unsigned int   hpass;
    CARD32	   buf_pitch, dst_pitch_off;
#endif
#if X_BYTE_ORDER == X_BIG_ENDIAN 
    unsigned char *RADEONMMIO = info->MMIO;
    unsigned int swapper = info->ModeReg.surface_cntl &
	    ~(RADEON_NONSURF_AP0_SWP_32BPP | RADEON_NONSURF_AP1_SWP_32BPP |
	      RADEON_NONSURF_AP0_SWP_16BPP | RADEON_NONSURF_AP1_SWP_16BPP);
#endif

    TRACE;

    if (bpp < 8)
	return FALSE;

#ifdef ACCEL_CP
    if (info->directRenderingEnabled &&
	RADEONGetPixmapOffsetPitch(pDst, &dst_pitch_off)) {
	CARD8 *buf;
	int cpp = bpp / 8;
	ACCEL_PREAMBLE();

	RADEON_SWITCH_TO_2D();
	while ((buf = RADEONHostDataBlit(pScrn,
					 cpp, w, dst_pitch_off, &buf_pitch,
					 x, &y, (unsigned int*)&h, &hpass)) != 0) {
	    RADEONHostDataBlitCopyPass(pScrn, cpp, buf, (CARD8 *)src,
				       hpass, buf_pitch, src_pitch);
	    src += hpass * src_pitch;
	}

	exaMarkSync(pDst->drawable.pScreen);
	return TRUE;
  }
#endif

    /* Do we need that sync here ? probably not .... */
    exaWaitSync(pDst->drawable.pScreen);

#if X_BYTE_ORDER == X_BIG_ENDIAN
    switch(bpp) {
    case 15:
    case 16:
	swapper |= RADEON_NONSURF_AP0_SWP_16BPP
		|  RADEON_NONSURF_AP1_SWP_16BPP;
	break;
    case 24:
    case 32:
	swapper |= RADEON_NONSURF_AP0_SWP_32BPP
		|  RADEON_NONSURF_AP1_SWP_32BPP;
	break;
    }
    OUTREG(RADEON_SURFACE_CNTL, swapper);
#endif
    w *= bpp / 8;
    dst += (x * bpp / 8) + (y * dst_pitch);

    while (h--) {
	memcpy(dst, src, w);
	src += src_pitch;
	dst += dst_pitch;
    }

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* restore byte swapping */
    OUTREG(RADEON_SURFACE_CNTL, info->ModeReg.surface_cntl);
#endif

    return TRUE;
}

#ifdef ACCEL_CP
/* Emit blit with arbitrary source and destination offsets and pitches */
static void
RADEONBlitChunk(ScrnInfoPtr pScrn, CARD32 datatype, CARD32 src_pitch_offset,
		CARD32 dst_pitch_offset, int srcX, int srcY, int dstX, int dstY,
		int w, int h)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    ACCEL_PREAMBLE();

    BEGIN_ACCEL(6);
    OUT_ACCEL_REG(RADEON_DP_GUI_MASTER_CNTL,
		  RADEON_GMC_DST_PITCH_OFFSET_CNTL |
		  RADEON_GMC_SRC_PITCH_OFFSET_CNTL |
		  RADEON_GMC_BRUSH_NONE |
		  (datatype << 8) |
		  RADEON_GMC_SRC_DATATYPE_COLOR |
		  RADEON_ROP3_S |
		  RADEON_DP_SRC_SOURCE_MEMORY |
		  RADEON_GMC_CLR_CMP_CNTL_DIS |
		  RADEON_GMC_WR_MSK_DIS);
    OUT_ACCEL_REG(RADEON_SRC_PITCH_OFFSET, src_pitch_offset);
    OUT_ACCEL_REG(RADEON_DST_PITCH_OFFSET, dst_pitch_offset);
    OUT_ACCEL_REG(RADEON_SRC_Y_X, (srcY << 16) | srcX);
    OUT_ACCEL_REG(RADEON_DST_Y_X, (dstY << 16) | dstX);
    OUT_ACCEL_REG(RADEON_DST_HEIGHT_WIDTH, (h << 16) | w);
    FINISH_ACCEL();
}
#endif

static Bool
FUNC_NAME(RADEONDownloadFromScreen)(PixmapPtr pSrc, int x, int y, int w, int h,
				    char *dst, int dst_pitch)
{
#if defined(ACCEL_CP) || X_BYTE_ORDER == X_BIG_ENDIAN
    RINFO_FROM_SCREEN(pSrc->drawable.pScreen);
#endif
#if X_BYTE_ORDER == X_BIG_ENDIAN
    unsigned char *RADEONMMIO = info->MMIO;
    unsigned int swapper = info->ModeReg.surface_cntl &
	    ~(RADEON_NONSURF_AP0_SWP_32BPP | RADEON_NONSURF_AP1_SWP_32BPP |
	      RADEON_NONSURF_AP0_SWP_16BPP | RADEON_NONSURF_AP1_SWP_16BPP);
#endif
    CARD8	  *src	     = pSrc->devPrivate.ptr;
    int		   src_pitch = exaGetPixmapPitch(pSrc);
    int		   bpp	     = pSrc->drawable.bitsPerPixel;
#ifdef ACCEL_CP
    CARD32 datatype, src_pitch_offset, scratch_pitch = (w * bpp/8 + 63) & ~63, scratch_off = 0;
    drmBufPtr scratch;
#endif

    TRACE;

#ifdef ACCEL_CP
    /*
     * Try to accelerate download. Use an indirect buffer as scratch space,
     * blitting the bits to one half while copying them out of the other one and
     * then swapping the halves.
     */
    if (info->accelDFS && bpp != 24 && RADEONGetDatatypeBpp(bpp, &datatype) &&
	RADEONGetPixmapOffsetPitch(pSrc, &src_pitch_offset) &&
	(scratch = RADEONCPGetBuffer(pScrn)))
    {
	int swap = RADEON_HOST_DATA_SWAP_NONE, wpass = w * bpp / 8;
	int hpass = min(h, scratch->total/2 / scratch_pitch);
	CARD32 scratch_pitch_offset = scratch_pitch << 16
				    | (info->gartLocation + info->bufStart
				       + scratch->idx * scratch->total) >> 10;
	drmRadeonIndirect indirect;
	ACCEL_PREAMBLE();

	RADEON_SWITCH_TO_2D();

	/* Kick the first blit as early as possible */
	RADEONBlitChunk(pScrn, datatype, src_pitch_offset, scratch_pitch_offset,
			x, y, 0, 0, w, hpass);
	FLUSH_RING();

#if X_BYTE_ORDER == X_BIG_ENDIAN
	switch (bpp) {
	case 16:
	  swap = RADEON_HOST_DATA_SWAP_16BIT;
	  break;
	case 32:
	  swap = RADEON_HOST_DATA_SWAP_32BIT;
	  break;
	}
#endif

	while (h) {
	    int oldhpass = hpass, i = 0;

	    src = (CARD8*)scratch->address + scratch_off;

	    y += oldhpass;
	    h -= oldhpass;
	    hpass = min(h, scratch->total/2 / scratch_pitch);

	    /* Prepare next blit if anything's left */
	    if (hpass) {
		scratch_off = scratch->total/2 - scratch_off;
		RADEONBlitChunk(pScrn, datatype, src_pitch_offset, scratch_pitch_offset + (scratch_off >> 10),
				x, y, 0, 0, w, hpass);
	    }

	    /*
	     * Wait for previous blit to complete.
	     *
	     * XXX: Doing here essentially the same things this ioctl does in
	     * the DRM results in corruption with 'small' transfers, apparently
	     * because the data doesn't actually land in system RAM before the
	     * memcpy. I suspect the ioctl helps mostly due to its latency; what
	     * we'd really need is a way to reliably wait for the host interface
	     * to be done with pushing the data to the host.
	     */
	    while ((drmCommandNone(info->drmFD, DRM_RADEON_CP_IDLE) == -EBUSY)
		   && (i++ < RADEON_TIMEOUT))
		;

	    /* Kick next blit */
	    if (hpass)
		FLUSH_RING();

	    /* Copy out data from previous blit */
	    if (wpass == scratch_pitch && wpass == dst_pitch) {
		RADEONCopySwap((CARD8*)dst, src, wpass * oldhpass, swap);
		dst += dst_pitch * oldhpass;
	    } else while (oldhpass--) {
		RADEONCopySwap((CARD8*)dst, src, wpass, swap);
		src += scratch_pitch;
		dst += dst_pitch;
	    }
	}

	indirect.idx = scratch->idx;
	indirect.start = indirect.end = 0;
	indirect.discard = 1;

	drmCommandWriteRead(info->drmFD, DRM_RADEON_INDIRECT,
			    &indirect, sizeof(drmRadeonIndirect));

	return TRUE;
    }
#endif

    /* Can't accelerate download */
    exaWaitSync(pSrc->drawable.pScreen);

#if X_BYTE_ORDER == X_BIG_ENDIAN
    switch(bpp) {
    case 15:
    case 16:
	swapper |= RADEON_NONSURF_AP0_SWP_16BPP
		|  RADEON_NONSURF_AP1_SWP_16BPP;
	break;
    case 24:
    case 32:
	swapper |= RADEON_NONSURF_AP0_SWP_32BPP
		|  RADEON_NONSURF_AP1_SWP_32BPP;
	break;
    }
    OUTREG(RADEON_SURFACE_CNTL, swapper);
#endif

    src += (x * bpp / 8) + (y * src_pitch);
    w *= bpp / 8;

    while (h--) {
	memcpy(dst, src, w);
	src += src_pitch;
	dst += dst_pitch;
    }

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* restore byte swapping */
    OUTREG(RADEON_SURFACE_CNTL, info->ModeReg.surface_cntl);
#endif

    return TRUE;
}

Bool FUNC_NAME(RADEONDrawInit)(ScreenPtr pScreen)
{
    RINFO_FROM_SCREEN(pScreen);

    if (info->exa == NULL) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "Memory map not set up\n");
	return FALSE;
    }

    info->exa->exa_major = 2;
    info->exa->exa_minor = 0;

    info->exa->PrepareSolid = FUNC_NAME(RADEONPrepareSolid);
    info->exa->Solid = FUNC_NAME(RADEONSolid);
    info->exa->DoneSolid = FUNC_NAME(RADEONDoneSolid);

    info->exa->PrepareCopy = FUNC_NAME(RADEONPrepareCopy);
    info->exa->Copy = FUNC_NAME(RADEONCopy);
    info->exa->DoneCopy = FUNC_NAME(RADEONDoneCopy);

    info->exa->WaitMarker = FUNC_NAME(RADEONSync);
    info->exa->UploadToScreen = FUNC_NAME(RADEONUploadToScreen);
    info->exa->DownloadFromScreen = FUNC_NAME(RADEONDownloadFromScreen);

#if X_BYTE_ORDER == X_BIG_ENDIAN
    info->exa->PrepareAccess = RADEONPrepareAccess;
    info->exa->FinishAccess = RADEONFinishAccess;
#endif /* X_BYTE_ORDER == X_BIG_ENDIAN */

    info->exa->flags = EXA_OFFSCREEN_PIXMAPS;
    info->exa->pixmapOffsetAlign = RADEON_BUFFER_ALIGN + 1;
    info->exa->pixmapPitchAlign = 64;

    info->exa->maxX = 2047;
    info->exa->maxY = 2047;

#ifdef RENDER
    if (info->RenderAccel) {
	if (info->ChipFamily >= CHIP_FAMILY_R300) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Render acceleration "
			       "unsupported on R300 type cards and newer.\n");
	} else if ((info->ChipFamily == CHIP_FAMILY_RV250) || 
		   (info->ChipFamily == CHIP_FAMILY_RV280) || 
		   (info->ChipFamily == CHIP_FAMILY_RS300) || 
		   (info->ChipFamily == CHIP_FAMILY_R200)) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Render acceleration "
			       "enabled for R200 type cards.\n");
		info->exa->CheckComposite = R200CheckComposite;
		info->exa->PrepareComposite =
		    FUNC_NAME(R200PrepareComposite);
		info->exa->Composite = FUNC_NAME(RadeonComposite);
		info->exa->DoneComposite = RadeonDoneComposite;
	} else {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Render acceleration "
			       "enabled for R100 type cards.\n");
		info->exa->CheckComposite = R100CheckComposite;
		info->exa->PrepareComposite =
		    FUNC_NAME(R100PrepareComposite);
		info->exa->Composite = FUNC_NAME(RadeonComposite);
		info->exa->DoneComposite = RadeonDoneComposite;
	}
    }
#endif

    RADEONEngineInit(pScrn);

    if (!exaDriverInit(pScreen, info->exa)) {
	xfree(info->exa);
	return FALSE;
    }
    exaMarkSync(pScreen);

    return TRUE;
}

#undef FUNC_NAME
