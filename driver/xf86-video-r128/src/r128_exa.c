/*
 * Copyright 2006 Joseph Garvin
 * Copyright 2012 Connor Behan
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
 *    Joseph Garvin <joseph.h.garvin@gmail.com>
 *    Connor Behan <connor.behan@gmail.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "r128.h"
#include "exa.h"

#include "r128_reg.h"
#include "r128_rop.h"

#include "xf86.h"

/* Assumes that depth 15 and 16 can be used as depth 16, which is okay since we
 * require src and dest datatypes to be equal.
 */
Bool R128GetDatatypeBpp(int bpp, uint32_t *type)
{
    switch (bpp) {
    case 8:
        *type = R128_DATATYPE_CI8;
        return TRUE;
    case 16:
        *type = R128_DATATYPE_RGB565;
        return TRUE;
    case 24:
        *type = R128_DATATYPE_RGB888;
        return TRUE;
    case 32:
        *type = R128_DATATYPE_ARGB8888;
        return TRUE;
    default:
        return FALSE;
    }
}

static Bool R128GetOffsetPitch(PixmapPtr pPix, int bpp, uint32_t *pitch_offset,
				 unsigned int offset, unsigned int pitch)
{
    ScreenPtr     pScreen   = pPix->drawable.pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);

    if (pitch > 16320 || pitch % info->ExaDriver->pixmapPitchAlign != 0) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Bad pitch 0x%08x\n", pitch));
	return FALSE;
    }

    if (offset % info->ExaDriver->pixmapOffsetAlign != 0) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Bad offset 0x%08x\n", offset));
	return FALSE;
    }

    *pitch_offset = ((pitch / bpp) << 21) | (offset >> 5);

    return TRUE;
}

Bool R128GetPixmapOffsetPitch(PixmapPtr pPix, uint32_t *pitch_offset)
{
    uint32_t pitch, offset;
    int bpp;

    bpp = pPix->drawable.bitsPerPixel;
    if (bpp == 24)
        bpp = 8;

    offset = exaGetPixmapOffset(pPix);
    pitch = exaGetPixmapPitch(pPix);

    return R128GetOffsetPitch(pPix, bpp, pitch_offset, offset, pitch);
}

static void Emit2DState(ScrnInfoPtr pScrn)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    int has_src		    = info->state_2d.src_pitch_offset;
    unsigned char *R128MMIO = info->MMIO;

    R128WaitForFifo(pScrn, (has_src ? 10 : 9));

    OUTREG(R128_DEFAULT_SC_BOTTOM_RIGHT, info->state_2d.default_sc_bottom_right);
    OUTREG(R128_DP_GUI_MASTER_CNTL, info->state_2d.dp_gui_master_cntl);
    OUTREG(R128_DP_BRUSH_FRGD_CLR, info->state_2d.dp_brush_frgd_clr);
    OUTREG(R128_DP_BRUSH_BKGD_CLR, info->state_2d.dp_brush_bkgd_clr);
    OUTREG(R128_DP_SRC_FRGD_CLR,   info->state_2d.dp_src_frgd_clr);
    OUTREG(R128_DP_SRC_BKGD_CLR,   info->state_2d.dp_src_bkgd_clr);
    OUTREG(R128_DP_WRITE_MASK, info->state_2d.dp_write_mask);
    OUTREG(R128_DP_CNTL, info->state_2d.dp_cntl);

    OUTREG(R128_DST_PITCH_OFFSET, info->state_2d.dst_pitch_offset);
    if (has_src) OUTREG(R128_SRC_PITCH_OFFSET, info->state_2d.src_pitch_offset);
}

#ifdef R128DRI
void EmitCCE2DState(ScrnInfoPtr pScrn)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    int has_src		    = info->state_2d.src_pitch_offset;
    RING_LOCALS;

    R128CCE_REFRESH( pScrn, info );

    BEGIN_RING( (has_src ? 20 : 18) );

    OUT_RING_REG( R128_DEFAULT_SC_BOTTOM_RIGHT, info->state_2d.default_sc_bottom_right );
    OUT_RING_REG( R128_DP_GUI_MASTER_CNTL, info->state_2d.dp_gui_master_cntl );
    OUT_RING_REG( R128_DP_BRUSH_FRGD_CLR, info->state_2d.dp_brush_frgd_clr );
    OUT_RING_REG( R128_DP_BRUSH_BKGD_CLR, info->state_2d.dp_brush_bkgd_clr );
    OUT_RING_REG( R128_DP_SRC_FRGD_CLR,   info->state_2d.dp_src_frgd_clr );
    OUT_RING_REG( R128_DP_SRC_BKGD_CLR,   info->state_2d.dp_src_bkgd_clr );
    OUT_RING_REG( R128_DP_WRITE_MASK, info->state_2d.dp_write_mask );
    OUT_RING_REG( R128_DP_CNTL, info->state_2d.dp_cntl );

    OUT_RING_REG( R128_DST_PITCH_OFFSET, info->state_2d.dst_pitch_offset );
    if (has_src) OUT_RING_REG( R128_SRC_PITCH_OFFSET, info->state_2d.src_pitch_offset );

    ADVANCE_RING();
}
#endif

/* EXA Callbacks */

static Bool
R128PrepareSolid(PixmapPtr pPixmap, int alu, Pixel planemask, Pixel fg)
{
    ScreenPtr     pScreen   = pPixmap->drawable.pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);

    int bpp = pPixmap->drawable.bitsPerPixel;
    uint32_t datatype, dst_pitch_offset;

    if (!R128GetDatatypeBpp(bpp, &datatype)) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "R128GetDatatypeBpp failed\n"));
	return FALSE;
    }
    if (!R128GetPixmapOffsetPitch(pPixmap, &dst_pitch_offset)) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "R128GetPixmapOffsetPitch failed\n"));
	return FALSE;
    }
    if (info->state_2d.in_use) return FALSE;

    info->state_2d.in_use = TRUE;
    info->state_2d.default_sc_bottom_right = (R128_DEFAULT_SC_RIGHT_MAX | R128_DEFAULT_SC_BOTTOM_MAX);
    info->state_2d.dp_brush_bkgd_clr = 0x00000000;
    info->state_2d.dp_src_frgd_clr = 0xffffffff;
    info->state_2d.dp_src_bkgd_clr = 0x00000000;
    info->state_2d.dp_gui_master_cntl = (R128_GMC_DST_PITCH_OFFSET_CNTL |
					  R128_GMC_BRUSH_SOLID_COLOR |
					  (datatype >> 8) |
					  R128_GMC_SRC_DATATYPE_COLOR |
					  R128_ROP[alu].pattern |
					  R128_GMC_CLR_CMP_CNTL_DIS);
    info->state_2d.dp_brush_frgd_clr = fg;
    info->state_2d.dp_cntl = (R128_DST_X_LEFT_TO_RIGHT | R128_DST_Y_TOP_TO_BOTTOM);
    info->state_2d.dp_write_mask = planemask;
    info->state_2d.dst_pitch_offset = dst_pitch_offset;
    info->state_2d.src_pitch_offset = 0;

#ifdef R128DRI
    if (info->directRenderingEnabled) {
        EmitCCE2DState(pScrn);
    } else
#endif
    {
        Emit2DState(pScrn);
    }
    return TRUE;
}

static void
R128Solid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
    ScreenPtr     pScreen   = pPixmap->drawable.pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    R128WaitForFifo(pScrn, 2);
    OUTREG(R128_DST_Y_X,          (y1 << 16) | x1);
    OUTREG(R128_DST_WIDTH_HEIGHT, ((x2-x1) << 16) | (y2-y1));
}

#define R128DoneSolid R128Done

void
R128DoPrepareCopy(ScrnInfoPtr pScrn, uint32_t src_pitch_offset,
			uint32_t dst_pitch_offset, uint32_t datatype, int alu, Pixel planemask)
{
    R128InfoPtr   info      = R128PTR(pScrn);

    info->state_2d.in_use = TRUE;
    info->state_2d.dp_gui_master_cntl = (R128_GMC_DST_PITCH_OFFSET_CNTL |
					  R128_GMC_SRC_PITCH_OFFSET_CNTL |
					  R128_GMC_BRUSH_NONE |
					  (datatype >> 8) |
					  R128_GMC_SRC_DATATYPE_COLOR |
					  R128_ROP[alu].rop |
					  R128_DP_SRC_SOURCE_MEMORY |
					  R128_GMC_CLR_CMP_CNTL_DIS);
    info->state_2d.dp_cntl = ((info->xdir >= 0 ? R128_DST_X_LEFT_TO_RIGHT : 0) |
			       (info->ydir >= 0 ? R128_DST_Y_TOP_TO_BOTTOM : 0));
    info->state_2d.dp_brush_frgd_clr = 0xffffffff;
    info->state_2d.dp_brush_bkgd_clr = 0x00000000;
    info->state_2d.dp_src_frgd_clr = 0xffffffff;
    info->state_2d.dp_src_bkgd_clr = 0x00000000;
    info->state_2d.dp_write_mask = planemask;
    info->state_2d.dst_pitch_offset = dst_pitch_offset;
    info->state_2d.src_pitch_offset = src_pitch_offset;
    info->state_2d.default_sc_bottom_right = (R128_DEFAULT_SC_RIGHT_MAX | R128_DEFAULT_SC_BOTTOM_MAX);

#ifdef R128DRI
    if (info->directRenderingEnabled) {
        EmitCCE2DState(pScrn);
    } else
#endif
    {
        Emit2DState(pScrn);
    }
}

static Bool
R128PrepareCopy(PixmapPtr pSrcPixmap, PixmapPtr pDstPixmap, int xdir, int ydir, int alu, Pixel planemask)
{
    ScreenPtr     pScreen   = pSrcPixmap->drawable.pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);

    int bpp = pDstPixmap->drawable.bitsPerPixel;
    uint32_t datatype, src_pitch_offset, dst_pitch_offset;

    if (!R128GetDatatypeBpp(bpp, &datatype)) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "R128GetDatatypeBpp failed\n"));
	return FALSE;
    }
    if (!R128GetPixmapOffsetPitch(pSrcPixmap, &src_pitch_offset)) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "R128GetPixmapOffsetPitch source "
                            "failed\n"));
	return FALSE;
    }
    if (!R128GetPixmapOffsetPitch(pDstPixmap, &dst_pitch_offset)) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "R128GetPixmapOffsetPitch dest failed\n"));
	return FALSE;
    }
    if (info->state_2d.in_use) return FALSE;

    info->xdir = xdir;
    info->ydir = ydir;

    R128DoPrepareCopy(pScrn, src_pitch_offset, dst_pitch_offset, datatype, alu, planemask);

    return TRUE;
}

static void
R128Copy(PixmapPtr pDstPixmap, int srcX, int srcY, int dstX, int dstY, int width, int height)
{
    ScreenPtr     pScreen   = pDstPixmap->drawable.pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    if (info->xdir < 0) srcX += width - 1, dstX += width - 1;
    if (info->ydir < 0) srcY += height - 1, dstY += height - 1;

    R128WaitForFifo(pScrn, 3);
    OUTREG(R128_SRC_Y_X,          (srcY << 16) | srcX);
    OUTREG(R128_DST_Y_X,          (dstY << 16) | dstX);
    OUTREG(R128_DST_HEIGHT_WIDTH, (height << 16) | width);
}

#define R128DoneCopy R128Done

static void
R128Sync(ScreenPtr pScreen, int marker)
{
    R128WaitForIdle(xf86ScreenToScrn(pScreen));
}

void
R128Done(PixmapPtr pPixmap)
{
    ScreenPtr     pScreen   = pPixmap->drawable.pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);

    info->state_2d.in_use = FALSE;
#if defined(R128DRI) && defined(RENDER)
    if (info->state_2d.src_pix) {
        pScreen->DestroyPixmap(info->state_2d.src_pix);
	info->state_2d.src_pix = NULL;
    }
    if (info->state_2d.msk_pix) {
        pScreen->DestroyPixmap(info->state_2d.msk_pix);
	info->state_2d.msk_pix = NULL;
    }
#endif
}

#ifdef R128DRI

#define R128CCEPrepareSolid R128PrepareSolid

static void
R128CCESolid(PixmapPtr pPixmap, int x1, int y1, int x2, int y2)
{
    ScreenPtr     pScreen   = pPixmap->drawable.pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);
    RING_LOCALS;

    R128CCE_REFRESH( pScrn, info );

    BEGIN_RING( 4 );

    OUT_RING_REG( R128_DST_Y_X,          (y1 << 16) | x1 );
    OUT_RING_REG( R128_DST_WIDTH_HEIGHT, ((x2-x1) << 16) | (y2-y1) );

    ADVANCE_RING();
}

#define R128CCEDoneSolid R128Done

#define R128CCEPrepareCopy R128PrepareCopy

static void
R128CCECopy(PixmapPtr pDstPixmap, int srcX, int srcY, int dstX, int dstY,
	 int width, int height)
{
    ScreenPtr     pScreen   = pDstPixmap->drawable.pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);
    RING_LOCALS;

    R128CCE_REFRESH( pScrn, info );

    if (info->xdir < 0) srcX += width - 1, dstX += width - 1;
    if (info->ydir < 0) srcY += height - 1, dstY += height - 1;

    BEGIN_RING( 6 );

    OUT_RING_REG( R128_SRC_Y_X,          (srcY << 16) | srcX );
    OUT_RING_REG( R128_DST_Y_X,          (dstY << 16) | dstX );
    OUT_RING_REG( R128_DST_HEIGHT_WIDTH, (height << 16) | width );

    ADVANCE_RING();
}

#define R128CCEDoneCopy R128Done

static void
R128CCESync(ScreenPtr pScreen, int marker)
{
    R128CCEWaitForIdle(xf86ScreenToScrn(pScreen));
}

#endif

Bool
R128EXAInit(ScreenPtr pScreen, int total)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    R128InfoPtr info  = R128PTR(pScrn);

    info->ExaDriver = exaDriverAlloc();
    if (!info->ExaDriver) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "Could not allocate EXA driver...\n");
        return FALSE;
    }

    info->ExaDriver->exa_major = EXA_VERSION_MAJOR;
    info->ExaDriver->exa_minor = EXA_VERSION_MINOR;

    info->ExaDriver->memoryBase = info->FB + pScrn->fbOffset;
    info->ExaDriver->offScreenBase = pScrn->virtualY *
                                        (pScrn->displayWidth *
                                        info->CurrentLayout.pixel_bytes);
    info->ExaDriver->memorySize = total;
    info->ExaDriver->flags = EXA_OFFSCREEN_PIXMAPS | EXA_OFFSCREEN_ALIGN_POT;

#if EXA_VERSION_MAJOR > 2 || (EXA_VERSION_MAJOR == 2 && EXA_VERSION_MINOR >= 3)
    info->ExaDriver->maxPitchBytes = 16320;
#endif
    /* Pitch alignment is in sets of 8 pixels, and we need to cover 32bpp, so it's 32 bytes */
    info->ExaDriver->pixmapPitchAlign = 32;
    info->ExaDriver->pixmapOffsetAlign = 32;
    info->ExaDriver->maxX = 2048;
    info->ExaDriver->maxY = 2048;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Setting up EXA callbacks\n");

#ifdef R128DRI
    if (info->directRenderingEnabled) {
	info->ExaDriver->PrepareSolid = R128CCEPrepareSolid;
	info->ExaDriver->Solid = R128CCESolid;
	info->ExaDriver->DoneSolid = R128CCEDoneSolid;

	info->ExaDriver->PrepareCopy = R128CCEPrepareCopy;
	info->ExaDriver->Copy = R128CCECopy;
	info->ExaDriver->DoneCopy = R128CCEDoneCopy;

#ifdef RENDER
	if (info->RenderAccel) {
	    info->ExaDriver->CheckComposite = R128CCECheckComposite;
	    info->ExaDriver->PrepareComposite = R128CCEPrepareComposite;
	    info->ExaDriver->Composite = R128CCEComposite;
	    info->ExaDriver->DoneComposite = R128CCEDoneComposite;
	}
#endif

	info->ExaDriver->WaitMarker = R128CCESync;
    } else
#endif
    {
	info->ExaDriver->PrepareSolid = R128PrepareSolid;
	info->ExaDriver->Solid = R128Solid;
	info->ExaDriver->DoneSolid = R128DoneSolid;

	info->ExaDriver->PrepareCopy = R128PrepareCopy;
	info->ExaDriver->Copy = R128Copy;
	info->ExaDriver->DoneCopy = R128DoneCopy;

	/* The registers used for r128 compositing are CCE specific, just like the
	 * registers used for radeon compositing are CP specific. The radeon driver
	 * falls back to different registers when there is no DRI. The equivalent
	 * registers on the r128 (if they even exist) are not listed in the register
	 * file so I can't implement compositing without DRI.
	 */

	info->ExaDriver->WaitMarker = R128Sync;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Initializing 2D acceleration engine...\n");

    R128EngineInit(pScrn);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Initializing EXA driver...\n");

    if (!exaDriverInit(pScreen, info->ExaDriver)) {
        free(info->ExaDriver);
	return FALSE;
    }

    info->state_2d.composite_setup = FALSE;
    return TRUE;
}
