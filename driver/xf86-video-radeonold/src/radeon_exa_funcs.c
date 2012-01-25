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

#include "exa.h"

static int
FUNC_NAME(RADEONMarkSync)(ScreenPtr pScreen)
{
    RINFO_FROM_SCREEN(pScreen);

    TRACE;

    return ++info->accel_state->exaSyncMarker;
}

static void
FUNC_NAME(RADEONSync)(ScreenPtr pScreen, int marker)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);

    TRACE;

    if (info->accel_state->exaMarkerSynced != marker) {
	FUNC_NAME(RADEONWaitForIdle)(pScrn);
	info->accel_state->exaMarkerSynced = marker;
    }

    RADEONPTR(pScrn)->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;
}

static Bool
FUNC_NAME(RADEONPrepareSolid)(PixmapPtr pPix, int alu, Pixel pm, Pixel fg)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);
    uint32_t datatype, dst_pitch_offset;
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

    if (info->accel_state->vsync)
	FUNC_NAME(RADEONWaitForVLine)(pScrn, pPix, RADEONBiggerCrtcArea(pPix), y1, y2);

    BEGIN_ACCEL(2);
    OUT_ACCEL_REG(RADEON_DST_Y_X, (y1 << 16) | x1);
    OUT_ACCEL_REG(RADEON_DST_HEIGHT_WIDTH, ((y2 - y1) << 16) | (x2 - x1));
    FINISH_ACCEL();
}

static void
FUNC_NAME(RADEONDoneSolid)(PixmapPtr pPix)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);
    ACCEL_PREAMBLE();

    TRACE;

    BEGIN_ACCEL(2);
    OUT_ACCEL_REG(RADEON_DSTCACHE_CTLSTAT, RADEON_RB2D_DC_FLUSH_ALL);
    OUT_ACCEL_REG(RADEON_WAIT_UNTIL,
                  RADEON_WAIT_2D_IDLECLEAN | RADEON_WAIT_DMA_GUI_IDLE);
    FINISH_ACCEL();
}

void
FUNC_NAME(RADEONDoPrepareCopy)(ScrnInfoPtr pScrn, uint32_t src_pitch_offset,
			       uint32_t dst_pitch_offset, uint32_t datatype, int rop,
			       Pixel planemask)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    ACCEL_PREAMBLE();

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
	((info->accel_state->xdir >= 0 ? RADEON_DST_X_LEFT_TO_RIGHT : 0) |
	 (info->accel_state->ydir >= 0 ? RADEON_DST_Y_TOP_TO_BOTTOM : 0)));
    OUT_ACCEL_REG(RADEON_DST_PITCH_OFFSET, dst_pitch_offset);
    OUT_ACCEL_REG(RADEON_SRC_PITCH_OFFSET, src_pitch_offset);
    FINISH_ACCEL();
}

static Bool
FUNC_NAME(RADEONPrepareCopy)(PixmapPtr pSrc,   PixmapPtr pDst,
			     int xdir, int ydir,
			     int rop,
			     Pixel planemask)
{
    RINFO_FROM_SCREEN(pDst->drawable.pScreen);
    uint32_t datatype, src_pitch_offset, dst_pitch_offset;

    TRACE;

    info->accel_state->xdir = xdir;
    info->accel_state->ydir = ydir;

    if (pDst->drawable.bitsPerPixel == 24)
	RADEON_FALLBACK(("24bpp unsupported"));
    if (!RADEONGetDatatypeBpp(pDst->drawable.bitsPerPixel, &datatype))
	RADEON_FALLBACK(("RADEONGetDatatypeBpp failed\n"));
    if (!RADEONGetPixmapOffsetPitch(pSrc, &src_pitch_offset))
	RADEON_FALLBACK(("RADEONGetPixmapOffsetPitch source failed\n"));
    if (!RADEONGetPixmapOffsetPitch(pDst, &dst_pitch_offset))
	RADEON_FALLBACK(("RADEONGetPixmapOffsetPitch dest failed\n"));

    FUNC_NAME(RADEONDoPrepareCopy)(pScrn, src_pitch_offset, dst_pitch_offset,
				   datatype, rop, planemask);

    return TRUE;
}

void
FUNC_NAME(RADEONCopy)(PixmapPtr pDst,
		      int srcX, int srcY,
		      int dstX, int dstY,
		      int w, int h)
{
    RINFO_FROM_SCREEN(pDst->drawable.pScreen);
    ACCEL_PREAMBLE();

    TRACE;

    if (info->accel_state->xdir < 0) {
	srcX += w - 1;
	dstX += w - 1;
    }
    if (info->accel_state->ydir < 0) {
	srcY += h - 1;
	dstY += h - 1;
    }

    if (info->accel_state->vsync)    
	FUNC_NAME(RADEONWaitForVLine)(pScrn, pDst, RADEONBiggerCrtcArea(pDst), dstY, dstY + h);

    BEGIN_ACCEL(3);

    OUT_ACCEL_REG(RADEON_SRC_Y_X,	   (srcY << 16) | srcX);
    OUT_ACCEL_REG(RADEON_DST_Y_X,	   (dstY << 16) | dstX);
    OUT_ACCEL_REG(RADEON_DST_HEIGHT_WIDTH, (h  << 16) | w);

    FINISH_ACCEL();
}

static void
FUNC_NAME(RADEONDoneCopy)(PixmapPtr pDst)
{
    RINFO_FROM_SCREEN(pDst->drawable.pScreen);
    ACCEL_PREAMBLE();

    TRACE;

    BEGIN_ACCEL(2);
    OUT_ACCEL_REG(RADEON_DSTCACHE_CTLSTAT, RADEON_RB2D_DC_FLUSH_ALL);
    OUT_ACCEL_REG(RADEON_WAIT_UNTIL,
                  RADEON_WAIT_2D_IDLECLEAN | RADEON_WAIT_DMA_GUI_IDLE);
    FINISH_ACCEL();
}


#ifdef ACCEL_CP

static Bool
RADEONUploadToScreenCP(PixmapPtr pDst, int x, int y, int w, int h,
		       char *src, int src_pitch)
{
    RINFO_FROM_SCREEN(pDst->drawable.pScreen);
    unsigned int   bpp	     = pDst->drawable.bitsPerPixel;
    unsigned int   hpass;
    uint32_t	   buf_pitch, dst_pitch_off;

    TRACE;

    if (bpp < 8)
	return FALSE;

    if (info->directRenderingEnabled &&
	RADEONGetPixmapOffsetPitch(pDst, &dst_pitch_off)) {
	uint8_t *buf;
	int cpp = bpp / 8;
	ACCEL_PREAMBLE();

	RADEON_SWITCH_TO_2D();

	if (info->accel_state->vsync)
	    FUNC_NAME(RADEONWaitForVLine)(pScrn, pDst, RADEONBiggerCrtcArea(pDst), y, y + h);

	while ((buf = RADEONHostDataBlit(pScrn,
					 cpp, w, dst_pitch_off, &buf_pitch,
					 x, &y, (unsigned int*)&h, &hpass)) != 0) {
	    RADEONHostDataBlitCopyPass(pScrn, cpp, buf, (uint8_t *)src,
				       hpass, buf_pitch, src_pitch);
	    src += hpass * src_pitch;
	}

	exaMarkSync(pDst->drawable.pScreen);
	return TRUE;
    }

    return FALSE;
}

/* Emit blit with arbitrary source and destination offsets and pitches */
static void
RADEONBlitChunk(ScrnInfoPtr pScrn, uint32_t datatype, uint32_t src_pitch_offset,
		uint32_t dst_pitch_offset, int srcX, int srcY, int dstX, int dstY,
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
    BEGIN_ACCEL(2);
    OUT_ACCEL_REG(RADEON_DSTCACHE_CTLSTAT, RADEON_RB2D_DC_FLUSH_ALL);
    OUT_ACCEL_REG(RADEON_WAIT_UNTIL,
                  RADEON_WAIT_2D_IDLECLEAN | RADEON_WAIT_DMA_GUI_IDLE);
    FINISH_ACCEL();
}


static Bool
RADEONDownloadFromScreenCP(PixmapPtr pSrc, int x, int y, int w, int h,
				    char *dst, int dst_pitch)
{
    RINFO_FROM_SCREEN(pSrc->drawable.pScreen);
    uint8_t	  *src	     = info->FB + exaGetPixmapOffset(pSrc);
    int		   bpp	     = pSrc->drawable.bitsPerPixel;
    uint32_t datatype, src_pitch_offset, scratch_pitch = (w * bpp/8 + 63) & ~63, scratch_off = 0;
    drmBufPtr scratch;

    TRACE;

    /*
     * Try to accelerate download. Use an indirect buffer as scratch space,
     * blitting the bits to one half while copying them out of the other one and
     * then swapping the halves.
     */
    if (bpp != 24 && RADEONGetDatatypeBpp(bpp, &datatype) &&
	RADEONGetPixmapOffsetPitch(pSrc, &src_pitch_offset) &&
	(scratch = RADEONCPGetBuffer(pScrn)))
    {
	int swap = RADEON_HOST_DATA_SWAP_NONE, wpass = w * bpp / 8;
	int hpass = min(h, scratch->total/2 / scratch_pitch);
	uint32_t scratch_pitch_offset = scratch_pitch << 16
				    | (info->gartLocation + info->dri->bufStart
				       + scratch->idx * scratch->total) >> 10;
	drm_radeon_indirect_t indirect;
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

	    src = (uint8_t*)scratch->address + scratch_off;

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
	    while ((drmCommandNone(info->dri->drmFD, DRM_RADEON_CP_IDLE) == -EBUSY)
		   && (i++ < RADEON_TIMEOUT))
		;

	    /* Kick next blit */
	    if (hpass)
		FLUSH_RING();

	    /* Copy out data from previous blit */
	    if (wpass == scratch_pitch && wpass == dst_pitch) {
		RADEONCopySwap((uint8_t*)dst, src, wpass * oldhpass, swap);
		dst += dst_pitch * oldhpass;
	    } else while (oldhpass--) {
		RADEONCopySwap((uint8_t*)dst, src, wpass, swap);
		src += scratch_pitch;
		dst += dst_pitch;
	    }
	}

	indirect.idx = scratch->idx;
	indirect.start = indirect.end = 0;
	indirect.discard = 1;

	drmCommandWriteRead(info->dri->drmFD, DRM_RADEON_INDIRECT,
			    &indirect, sizeof(drm_radeon_indirect_t));

	info->accel_state->exaMarkerSynced = info->accel_state->exaSyncMarker;

	return TRUE;
    }

    return FALSE;
}

#endif	/* def ACCEL_CP */


Bool FUNC_NAME(RADEONDrawInit)(ScreenPtr pScreen)
{
    RINFO_FROM_SCREEN(pScreen);

    if (info->accel_state->exa == NULL) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "Memory map not set up\n");
	return FALSE;
    }

    info->accel_state->exa->exa_major = EXA_VERSION_MAJOR;
    info->accel_state->exa->exa_minor = EXA_VERSION_MINOR;

    info->accel_state->exa->PrepareSolid = FUNC_NAME(RADEONPrepareSolid);
    info->accel_state->exa->Solid = FUNC_NAME(RADEONSolid);
    info->accel_state->exa->DoneSolid = FUNC_NAME(RADEONDoneSolid);

    info->accel_state->exa->PrepareCopy = FUNC_NAME(RADEONPrepareCopy);
    info->accel_state->exa->Copy = FUNC_NAME(RADEONCopy);
    info->accel_state->exa->DoneCopy = FUNC_NAME(RADEONDoneCopy);

    info->accel_state->exa->MarkSync = FUNC_NAME(RADEONMarkSync);
    info->accel_state->exa->WaitMarker = FUNC_NAME(RADEONSync);
#ifdef ACCEL_CP
    info->accel_state->exa->UploadToScreen = RADEONUploadToScreenCP;
    if (info->accelDFS)
	info->accel_state->exa->DownloadFromScreen = RADEONDownloadFromScreenCP;
#endif

#if X_BYTE_ORDER == X_BIG_ENDIAN
    info->accel_state->exa->PrepareAccess = RADEONPrepareAccess;
    info->accel_state->exa->FinishAccess = RADEONFinishAccess;
#endif /* X_BYTE_ORDER == X_BIG_ENDIAN */

    info->accel_state->exa->flags = EXA_OFFSCREEN_PIXMAPS;
#ifdef EXA_SUPPORTS_PREPARE_AUX
    info->accel_state->exa->flags |= EXA_SUPPORTS_PREPARE_AUX;
#endif
    info->accel_state->exa->pixmapOffsetAlign = RADEON_BUFFER_ALIGN + 1;
    info->accel_state->exa->pixmapPitchAlign = 64;

#ifdef RENDER
    if (info->RenderAccel) {
	if (info->ChipFamily >= CHIP_FAMILY_R600)
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Render acceleration "
			       "unsupported on R600 and newer cards.\n");
	else if (IS_R300_3D || IS_R500_3D) {
	    if ((info->ChipFamily < CHIP_FAMILY_RS400)
#ifdef XF86DRI
		|| (info->directRenderingEnabled)
#endif
		) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Render acceleration "
			       "enabled for R300/R400/R500 type cards.\n");
		info->accel_state->exa->CheckComposite = R300CheckComposite;
		info->accel_state->exa->PrepareComposite =
		    FUNC_NAME(R300PrepareComposite);
		info->accel_state->exa->Composite = FUNC_NAME(RadeonComposite);
		info->accel_state->exa->DoneComposite = FUNC_NAME(RadeonDoneComposite);
	    } else
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "EXA Composite requires CP on R5xx/IGP\n");
	} else if ((info->ChipFamily == CHIP_FAMILY_RV250) ||
		   (info->ChipFamily == CHIP_FAMILY_RV280) ||
		   (info->ChipFamily == CHIP_FAMILY_RS300) ||
		   (info->ChipFamily == CHIP_FAMILY_R200)) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Render acceleration "
			       "enabled for R200 type cards.\n");
		info->accel_state->exa->CheckComposite = R200CheckComposite;
		info->accel_state->exa->PrepareComposite =
		    FUNC_NAME(R200PrepareComposite);
		info->accel_state->exa->Composite = FUNC_NAME(RadeonComposite);
		info->accel_state->exa->DoneComposite = FUNC_NAME(RadeonDoneComposite);
	} else {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Render acceleration "
			       "enabled for R100 type cards.\n");
		info->accel_state->exa->CheckComposite = R100CheckComposite;
		info->accel_state->exa->PrepareComposite =
		    FUNC_NAME(R100PrepareComposite);
		info->accel_state->exa->Composite = FUNC_NAME(RadeonComposite);
		info->accel_state->exa->DoneComposite = FUNC_NAME(RadeonDoneComposite);
	}
    }
#endif

#if EXA_VERSION_MAJOR > 2 || (EXA_VERSION_MAJOR == 2 && EXA_VERSION_MINOR >= 3)
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Setting EXA maxPitchBytes\n");

    info->accel_state->exa->maxPitchBytes = 16320;
    info->accel_state->exa->maxX = 8192;
#else
    info->accel_state->exa->maxX = 16320 / 4;
#endif
    info->accel_state->exa->maxY = 8192;

    if (xf86ReturnOptValBool(info->Options, OPTION_EXA_VSYNC, FALSE)) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "EXA VSync enabled\n");
	info->accel_state->vsync = TRUE;
    } else
	info->accel_state->vsync = FALSE;

    RADEONEngineInit(pScrn);

    if (!exaDriverInit(pScreen, info->accel_state->exa)) {
	xfree(info->accel_state->exa);
	return FALSE;
    }
    exaMarkSync(pScreen);

    return TRUE;
}

#undef FUNC_NAME
