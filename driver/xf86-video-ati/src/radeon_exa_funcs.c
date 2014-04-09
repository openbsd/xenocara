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
 *    Michel Dänzer <michel@daenzer.net>
 *
 */

#include <errno.h>
#include <string.h>

#include "radeon.h"

#include "exa.h"

static int
RADEONMarkSync(ScreenPtr pScreen)
{
    RINFO_FROM_SCREEN(pScreen);

    TRACE;

    return ++info->accel_state->exaSyncMarker;
}

static void
RADEONSync(ScreenPtr pScreen, int marker)
{

}

static void Emit2DState(ScrnInfoPtr pScrn, int op)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int has_src;

    /* don't emit if no operation in progress */
    if (info->state_2d.op == 0 && op == 0)
	return;

    has_src = info->state_2d.src_pitch_offset || info->state_2d.src_bo;

    if (has_src) {
      BEGIN_ACCEL_RELOC(10, 2);
    } else {
      BEGIN_ACCEL_RELOC(9, 1);
    }
    OUT_RING_REG(RADEON_DEFAULT_SC_BOTTOM_RIGHT, info->state_2d.default_sc_bottom_right);
    OUT_RING_REG(RADEON_DP_GUI_MASTER_CNTL, info->state_2d.dp_gui_master_cntl);
    OUT_RING_REG(RADEON_DP_BRUSH_FRGD_CLR, info->state_2d.dp_brush_frgd_clr);
    OUT_RING_REG(RADEON_DP_BRUSH_BKGD_CLR, info->state_2d.dp_brush_bkgd_clr);
    OUT_RING_REG(RADEON_DP_SRC_FRGD_CLR,   info->state_2d.dp_src_frgd_clr);
    OUT_RING_REG(RADEON_DP_SRC_BKGD_CLR,   info->state_2d.dp_src_bkgd_clr);
    OUT_RING_REG(RADEON_DP_WRITE_MASK, info->state_2d.dp_write_mask);
    OUT_RING_REG(RADEON_DP_CNTL, info->state_2d.dp_cntl);

    OUT_RING_REG(RADEON_DST_PITCH_OFFSET, info->state_2d.dst_pitch_offset);
    OUT_RING_RELOC(info->state_2d.dst_bo, 0, info->state_2d.dst_domain);

    if (has_src) {
	OUT_RING_REG(RADEON_SRC_PITCH_OFFSET, info->state_2d.src_pitch_offset);
	OUT_RING_RELOC(info->state_2d.src_bo, RADEON_GEM_DOMAIN_GTT|RADEON_GEM_DOMAIN_VRAM, 0);
    }
    ADVANCE_RING();

    if (op)
	info->state_2d.op = op;
    info->reemit_current2d = Emit2DState;
}

static void
RADEONFlush2D(PixmapPtr pPix)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);

    TRACE;

    BEGIN_RING(2*2);
    OUT_RING_REG(RADEON_DSTCACHE_CTLSTAT, RADEON_RB2D_DC_FLUSH_ALL);
    OUT_RING_REG(RADEON_WAIT_UNTIL,
                  RADEON_WAIT_2D_IDLECLEAN | RADEON_WAIT_DMA_GUI_IDLE);
    ADVANCE_RING();
}

static void
RADEONDone2D(PixmapPtr pPix)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);
    info->state_2d.op = 0;

    RADEONFlush2D(pPix);
}

static Bool
RADEONPrepareSolid(PixmapPtr pPix, int alu, Pixel pm, Pixel fg)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);
    uint32_t datatype, dst_pitch_offset;
    struct radeon_exa_pixmap_priv *driver_priv;
    int ret;

    TRACE;

    if (pPix->drawable.bitsPerPixel == 24)
	RADEON_FALLBACK(("24bpp unsupported\n"));
    if (!RADEONGetDatatypeBpp(pPix->drawable.bitsPerPixel, &datatype))
	RADEON_FALLBACK(("RADEONGetDatatypeBpp failed\n"));
    if (!RADEONGetPixmapOffsetPitch(pPix, &dst_pitch_offset))
	RADEON_FALLBACK(("RADEONGetPixmapOffsetPitch failed\n"));

    RADEON_SWITCH_TO_2D();

    radeon_cs_space_reset_bos(info->cs);

    driver_priv = exaGetPixmapDriverPrivate(pPix);
    radeon_cs_space_add_persistent_bo(info->cs, driver_priv->bo, 0, RADEON_GEM_DOMAIN_VRAM);

    ret = radeon_cs_space_check(info->cs);
    if (ret)
	RADEON_FALLBACK(("Not enough RAM to hw accel solid operation\n"));

    driver_priv = exaGetPixmapDriverPrivate(pPix);
    if (driver_priv) {
	info->state_2d.dst_bo = driver_priv->bo;
 	info->state_2d.dst_domain = driver_priv->shared ? RADEON_GEM_DOMAIN_GTT : RADEON_GEM_DOMAIN_VRAM;
    }

    info->state_2d.default_sc_bottom_right = (RADEON_DEFAULT_SC_RIGHT_MAX |
					       RADEON_DEFAULT_SC_BOTTOM_MAX);
    info->state_2d.dp_brush_bkgd_clr = 0x00000000;
    info->state_2d.dp_src_frgd_clr = 0xffffffff;
    info->state_2d.dp_src_bkgd_clr = 0x00000000;
    info->state_2d.dp_gui_master_cntl = (RADEON_GMC_DST_PITCH_OFFSET_CNTL |
					  RADEON_GMC_BRUSH_SOLID_COLOR |
					  (datatype << 8) |
					  RADEON_GMC_SRC_DATATYPE_COLOR |
					  RADEON_ROP[alu].pattern |
					  RADEON_GMC_CLR_CMP_CNTL_DIS);
    info->state_2d.dp_brush_frgd_clr = fg;
    info->state_2d.dp_cntl = (RADEON_DST_X_LEFT_TO_RIGHT | RADEON_DST_Y_TOP_TO_BOTTOM);
    info->state_2d.dp_write_mask = pm;
    info->state_2d.dst_pitch_offset = dst_pitch_offset;
    info->state_2d.src_pitch_offset = 0;
    info->state_2d.src_bo = NULL;

    info->accel_state->dst_pix = pPix;

    Emit2DState(pScrn, RADEON_2D_EXA_SOLID);

    return TRUE;
}


static void
RADEONSolid(PixmapPtr pPix, int x1, int y1, int x2, int y2)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);

    TRACE;

    if (CS_FULL(info->cs)) {
	RADEONFlush2D(info->accel_state->dst_pix);
	radeon_cs_flush_indirect(pScrn);
    }

    if (info->accel_state->vsync)
	RADEONWaitForVLine(pScrn, pPix,
			   radeon_pick_best_crtc(pScrn, FALSE, x1, x2, y1, y2),
			   y1, y2);

    BEGIN_RING(2*2);
    OUT_RING_REG(RADEON_DST_Y_X, (y1 << 16) | x1);
    OUT_RING_REG(RADEON_DST_HEIGHT_WIDTH, ((y2 - y1) << 16) | (x2 - x1));
    ADVANCE_RING();
}

static void
RADEONDoPrepareCopy(ScrnInfoPtr pScrn, uint32_t src_pitch_offset,
		    uint32_t dst_pitch_offset, uint32_t datatype, int rop,
		    Pixel planemask)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    /* setup 2D state */
    info->state_2d.dp_gui_master_cntl = (RADEON_GMC_DST_PITCH_OFFSET_CNTL |
					  RADEON_GMC_SRC_PITCH_OFFSET_CNTL |
					  RADEON_GMC_BRUSH_NONE |
					  (datatype << 8) |
					  RADEON_GMC_SRC_DATATYPE_COLOR |
					  RADEON_ROP[rop].rop |
					  RADEON_DP_SRC_SOURCE_MEMORY |
					  RADEON_GMC_CLR_CMP_CNTL_DIS);
    info->state_2d.dp_cntl = ((info->accel_state->xdir >= 0 ? RADEON_DST_X_LEFT_TO_RIGHT : 0) |
			       (info->accel_state->ydir >= 0 ? RADEON_DST_Y_TOP_TO_BOTTOM : 0));
    info->state_2d.dp_brush_frgd_clr = 0xffffffff;
    info->state_2d.dp_brush_bkgd_clr = 0x00000000;
    info->state_2d.dp_src_frgd_clr = 0xffffffff;
    info->state_2d.dp_src_bkgd_clr = 0x00000000;
    info->state_2d.dp_write_mask = planemask;
    info->state_2d.dst_pitch_offset = dst_pitch_offset;
    info->state_2d.src_pitch_offset = src_pitch_offset;
    info->state_2d.default_sc_bottom_right =  (RADEON_DEFAULT_SC_RIGHT_MAX
						| RADEON_DEFAULT_SC_BOTTOM_MAX);

    Emit2DState(pScrn, RADEON_2D_EXA_COPY);
}

static Bool
RADEONPrepareCopy(PixmapPtr pSrc,   PixmapPtr pDst,
		  int xdir, int ydir,
		  int rop,
		  Pixel planemask)
{
    RINFO_FROM_SCREEN(pDst->drawable.pScreen);
    uint32_t datatype, src_pitch_offset, dst_pitch_offset;
    struct radeon_exa_pixmap_priv *driver_priv;
    int ret;
    TRACE;

    if (pDst->drawable.bitsPerPixel == 24)
	RADEON_FALLBACK(("24bpp unsupported"));
    if (!RADEONGetDatatypeBpp(pDst->drawable.bitsPerPixel, &datatype))
	RADEON_FALLBACK(("RADEONGetDatatypeBpp failed\n"));
    if (!RADEONGetPixmapOffsetPitch(pSrc, &src_pitch_offset))
	RADEON_FALLBACK(("RADEONGetPixmapOffsetPitch source failed\n"));
    if (!RADEONGetPixmapOffsetPitch(pDst, &dst_pitch_offset))
	RADEON_FALLBACK(("RADEONGetPixmapOffsetPitch dest failed\n"));

    RADEON_SWITCH_TO_2D();

    radeon_cs_space_reset_bos(info->cs);

    driver_priv = exaGetPixmapDriverPrivate(pSrc);
    radeon_cs_space_add_persistent_bo(info->cs, driver_priv->bo, RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM, 0);
    info->state_2d.src_bo = driver_priv->bo;

    driver_priv = exaGetPixmapDriverPrivate(pDst);
    info->state_2d.dst_bo = driver_priv->bo;
    info->state_2d.dst_domain = driver_priv->shared ? RADEON_GEM_DOMAIN_GTT : RADEON_GEM_DOMAIN_VRAM;
    radeon_cs_space_add_persistent_bo(info->cs, driver_priv->bo, 0, info->state_2d.dst_domain);

    ret = radeon_cs_space_check(info->cs);
    if (ret)
	RADEON_FALLBACK(("Not enough RAM to hw accel copy operation\n"));

    info->accel_state->xdir = xdir;
    info->accel_state->ydir = ydir;
    info->accel_state->dst_pix = pDst;

    RADEONDoPrepareCopy(pScrn, src_pitch_offset, dst_pitch_offset,
			datatype, rop, planemask);

    return TRUE;
}

static void
RADEONCopy(PixmapPtr pDst,
	   int srcX, int srcY,
	   int dstX, int dstY,
	   int w, int h)
{
    RINFO_FROM_SCREEN(pDst->drawable.pScreen);

    TRACE;

    if (CS_FULL(info->cs)) {
        RADEONFlush2D(info->accel_state->dst_pix);
	radeon_cs_flush_indirect(pScrn);
    }

    if (info->accel_state->xdir < 0) {
	srcX += w - 1;
	dstX += w - 1;
    }
    if (info->accel_state->ydir < 0) {
	srcY += h - 1;
	dstY += h - 1;
    }

    if (info->accel_state->vsync)
	RADEONWaitForVLine(pScrn, pDst,
			   radeon_pick_best_crtc(pScrn, FALSE, dstX, dstX + w, dstY, dstY + h),
			   dstY, dstY + h);

    BEGIN_RING(2*3);

    OUT_RING_REG(RADEON_SRC_Y_X,	   (srcY << 16) | srcX);
    OUT_RING_REG(RADEON_DST_Y_X,	   (dstY << 16) | dstX);
    OUT_RING_REG(RADEON_DST_HEIGHT_WIDTH, (h  << 16) | w);

    ADVANCE_RING();
}

/* Emit blit with arbitrary source and destination offsets and pitches */
static void
RADEONBlitChunk(ScrnInfoPtr pScrn, struct radeon_bo *src_bo,
                struct radeon_bo *dst_bo, uint32_t datatype,
                uint32_t src_pitch_offset, uint32_t dst_pitch_offset,
                int srcX, int srcY, int dstX, int dstY, int w, int h,
                uint32_t src_domain, uint32_t dst_domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    if (src_bo && dst_bo) {
        BEGIN_ACCEL_RELOC(6, 2);
    } else if (src_bo && dst_bo == NULL) {
        BEGIN_ACCEL_RELOC(6, 1);
    } else {
        BEGIN_RING(2*6);
    }
    OUT_RING_REG(RADEON_DP_GUI_MASTER_CNTL,
		  RADEON_GMC_DST_PITCH_OFFSET_CNTL |
		  RADEON_GMC_SRC_PITCH_OFFSET_CNTL |
		  RADEON_GMC_BRUSH_NONE |
		  (datatype << 8) |
		  RADEON_GMC_SRC_DATATYPE_COLOR |
		  RADEON_ROP3_S |
		  RADEON_DP_SRC_SOURCE_MEMORY |
		  RADEON_GMC_CLR_CMP_CNTL_DIS |
		  RADEON_GMC_WR_MSK_DIS);
    OUT_RING_REG(RADEON_SRC_PITCH_OFFSET, src_pitch_offset);
    if (src_bo) {
	OUT_RING_RELOC(src_bo, src_domain, 0);
    }
    OUT_RING_REG(RADEON_DST_PITCH_OFFSET, dst_pitch_offset);
    if (dst_bo) {
	OUT_RING_RELOC(dst_bo, 0, dst_domain);
    }
    OUT_RING_REG(RADEON_SRC_Y_X, (srcY << 16) | srcX);
    OUT_RING_REG(RADEON_DST_Y_X, (dstY << 16) | dstX);
    OUT_RING_REG(RADEON_DST_HEIGHT_WIDTH, (h << 16) | w);
    ADVANCE_RING();
    BEGIN_RING(2*2);
    OUT_RING_REG(RADEON_DSTCACHE_CTLSTAT, RADEON_RB2D_DC_FLUSH_ALL);
    OUT_RING_REG(RADEON_WAIT_UNTIL,
                  RADEON_WAIT_2D_IDLECLEAN | RADEON_WAIT_DMA_GUI_IDLE);
    ADVANCE_RING();
}

static Bool
RADEONUploadToScreenCS(PixmapPtr pDst, int x, int y, int w, int h,
		       char *src, int src_pitch)
{
    ScreenPtr pScreen = pDst->drawable.pScreen;
    RINFO_FROM_SCREEN(pScreen);
    struct radeon_exa_pixmap_priv *driver_priv;
    struct radeon_bo *scratch = NULL;
    struct radeon_bo *copy_dst;
    unsigned char *dst;
    unsigned size;
    uint32_t datatype = 0;
    uint32_t dst_domain;
    uint32_t dst_pitch_offset;
    unsigned bpp = pDst->drawable.bitsPerPixel;
    uint32_t scratch_pitch = RADEON_ALIGN(w * bpp / 8, 64);
    uint32_t copy_pitch;
    uint32_t swap = RADEON_HOST_DATA_SWAP_NONE;
    int ret;
    Bool flush = TRUE;
    Bool r;
    int i;

    if (bpp < 8)
	return FALSE;

    driver_priv = exaGetPixmapDriverPrivate(pDst);
    if (!driver_priv || !driver_priv->bo)
	return FALSE;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    switch (bpp) {
    case 32:
	swap = RADEON_HOST_DATA_SWAP_32BIT;
	break;
    case 16:
	swap = RADEON_HOST_DATA_SWAP_16BIT;
	break;
    }
#endif

    /* If we know the BO won't be busy / in VRAM, don't bother with a scratch */
    copy_dst = driver_priv->bo;
    copy_pitch = pDst->devKind;
    if (!(driver_priv->tiling_flags & (RADEON_TILING_MACRO | RADEON_TILING_MICRO))) {
	if (!radeon_bo_is_referenced_by_cs(driver_priv->bo, info->cs)) {
	    flush = FALSE;
	    if (!radeon_bo_is_busy(driver_priv->bo, &dst_domain) &&
		!(dst_domain & RADEON_GEM_DOMAIN_VRAM))
		goto copy;
	}
	/* use cpu copy for fast fb access */
	if (info->is_fast_fb)
	    goto copy;
    }

    size = scratch_pitch * h;
    scratch = radeon_bo_open(info->bufmgr, 0, size, 0, RADEON_GEM_DOMAIN_GTT, 0);
    if (scratch == NULL) {
	goto copy;
    }
    radeon_cs_space_reset_bos(info->cs);
    radeon_add_pixmap(info->cs, pDst, 0, RADEON_GEM_DOMAIN_VRAM);
    radeon_cs_space_add_persistent_bo(info->cs, scratch, RADEON_GEM_DOMAIN_GTT, 0);
    ret = radeon_cs_space_check(info->cs);
    if (ret) {
	goto copy;
    }
    copy_dst = scratch;
    copy_pitch = scratch_pitch;
    flush = FALSE;

copy:
    if (flush)
	radeon_cs_flush_indirect(pScrn);

    ret = radeon_bo_map(copy_dst, 0);
    if (ret) {
        r = FALSE;
        goto out;
    }
    r = TRUE;
    size = w * bpp / 8;
    dst = copy_dst->ptr;
    if (copy_dst == driver_priv->bo)
	dst += y * copy_pitch + x * bpp / 8;
    for (i = 0; i < h; i++) {
        RADEONCopySwap(dst + i * copy_pitch, (uint8_t*)src, size, swap);
        src += src_pitch;
    }
    radeon_bo_unmap(copy_dst);

    if (copy_dst == scratch) {
	RADEONGetDatatypeBpp(pDst->drawable.bitsPerPixel, &datatype);
	RADEONGetPixmapOffsetPitch(pDst, &dst_pitch_offset);
	RADEON_SWITCH_TO_2D();
	RADEONBlitChunk(pScrn, scratch, driver_priv->bo, datatype, scratch_pitch << 16,
			dst_pitch_offset, 0, 0, x, y, w, h,
			RADEON_GEM_DOMAIN_GTT, RADEON_GEM_DOMAIN_VRAM);
    }

out:
    if (scratch)
	radeon_bo_unref(scratch);
    return r;
}

static Bool
RADEONDownloadFromScreenCS(PixmapPtr pSrc, int x, int y, int w,
                           int h, char *dst, int dst_pitch)
{
    RINFO_FROM_SCREEN(pSrc->drawable.pScreen);
    struct radeon_exa_pixmap_priv *driver_priv;
    struct radeon_bo *scratch = NULL;
    struct radeon_bo *copy_src;
    unsigned size;
    uint32_t datatype = 0;
    uint32_t src_domain = 0;
    uint32_t src_pitch_offset;
    unsigned bpp = pSrc->drawable.bitsPerPixel;
    uint32_t scratch_pitch = RADEON_ALIGN(w * bpp / 8, 64);
    uint32_t copy_pitch;
    uint32_t swap = RADEON_HOST_DATA_SWAP_NONE;
    int ret;
    Bool flush = FALSE;
    Bool r;

    if (bpp < 8)
	return FALSE;

    driver_priv = exaGetPixmapDriverPrivate(pSrc);
    if (!driver_priv || !driver_priv->bo)
	return FALSE;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    switch (bpp) {
    case 32:
	swap = RADEON_HOST_DATA_SWAP_32BIT;
	break;
    case 16:
	swap = RADEON_HOST_DATA_SWAP_16BIT;
	break;
    }
#endif

    /* If we know the BO won't end up in VRAM anyway, don't bother with a scratch */
    copy_src = driver_priv->bo;
    copy_pitch = pSrc->devKind;
    if (!(driver_priv->tiling_flags & (RADEON_TILING_MACRO | RADEON_TILING_MICRO))) {
	if (radeon_bo_is_referenced_by_cs(driver_priv->bo, info->cs)) {
	    src_domain = radeon_bo_get_src_domain(driver_priv->bo);
	    if ((src_domain & (RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM)) ==
		(RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM))
		src_domain = 0;
	    else /* A write may be scheduled */
		flush = TRUE;
	}

	if (!src_domain)
	    radeon_bo_is_busy(driver_priv->bo, &src_domain);

	if (src_domain & ~(uint32_t)RADEON_GEM_DOMAIN_VRAM)
	    goto copy;
    }
    size = scratch_pitch * h;
    scratch = radeon_bo_open(info->bufmgr, 0, size, 0, RADEON_GEM_DOMAIN_GTT, 0);
    if (scratch == NULL) {
	goto copy;
    }
    radeon_cs_space_reset_bos(info->cs);
    radeon_add_pixmap(info->cs, pSrc, RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM, 0);
    radeon_cs_space_add_persistent_bo(info->cs, scratch, 0, RADEON_GEM_DOMAIN_GTT);
    ret = radeon_cs_space_check(info->cs);
    if (ret) {
	goto copy;
    }
    RADEONGetDatatypeBpp(pSrc->drawable.bitsPerPixel, &datatype);
    RADEONGetPixmapOffsetPitch(pSrc, &src_pitch_offset);
    RADEON_SWITCH_TO_2D();
    RADEONBlitChunk(pScrn, driver_priv->bo, scratch, datatype, src_pitch_offset,
                    scratch_pitch << 16, x, y, 0, 0, w, h,
                    RADEON_GEM_DOMAIN_VRAM | RADEON_GEM_DOMAIN_GTT,
                    RADEON_GEM_DOMAIN_GTT);
    copy_src = scratch;
    copy_pitch = scratch_pitch;
    flush = TRUE;

copy:
    if (flush)
	FLUSH_RING();

    ret = radeon_bo_map(copy_src, 0);
    if (ret) {
	ErrorF("failed to map pixmap: %d\n", ret);
        r = FALSE;
        goto out;
    }
    r = TRUE;
    w *= bpp / 8;
    if (copy_src == driver_priv->bo)
	size = y * copy_pitch + x * bpp / 8;
    else
	size = 0;
    while (h--) {
        RADEONCopySwap((uint8_t*)dst, copy_src->ptr + size, w, swap);
        size += copy_pitch;
        dst += dst_pitch;
    }
    radeon_bo_unmap(copy_src);
out:
    if (scratch)
	radeon_bo_unref(scratch);
    return r;
}

Bool RADEONDrawInit(ScreenPtr pScreen)
{
    RINFO_FROM_SCREEN(pScreen);

    if (info->accel_state->exa == NULL) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "Memory map not set up\n");
	return FALSE;
    }

    info->accel_state->exa->exa_major = EXA_VERSION_MAJOR;
    info->accel_state->exa->exa_minor = EXA_VERSION_MINOR;

    info->accel_state->exa->PrepareSolid = RADEONPrepareSolid;
    info->accel_state->exa->Solid = RADEONSolid;
    info->accel_state->exa->DoneSolid = RADEONDone2D;

    info->accel_state->exa->PrepareCopy = RADEONPrepareCopy;
    info->accel_state->exa->Copy = RADEONCopy;
    info->accel_state->exa->DoneCopy = RADEONDone2D;

    info->accel_state->exa->MarkSync = RADEONMarkSync;
    info->accel_state->exa->WaitMarker = RADEONSync;

    info->accel_state->exa->UploadToScreen = &RADEONUploadToScreenCS;
    info->accel_state->exa->DownloadFromScreen = &RADEONDownloadFromScreenCS;

    info->accel_state->exa->flags = EXA_OFFSCREEN_PIXMAPS | EXA_SUPPORTS_PREPARE_AUX |
	EXA_SUPPORTS_OFFSCREEN_OVERLAPS | EXA_HANDLES_PIXMAPS | EXA_MIXED_PIXMAPS;
    info->accel_state->exa->pixmapOffsetAlign = RADEON_GPU_PAGE_SIZE;
    info->accel_state->exa->pixmapPitchAlign = 64;

#ifdef RENDER
    if (info->RenderAccel) {
	if (IS_R300_3D || IS_R500_3D) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Render acceleration "
			       "enabled for R300/R400/R500 type cards.\n");
		info->accel_state->exa->CheckComposite = R300CheckComposite;
		info->accel_state->exa->PrepareComposite =
		    R300PrepareComposite;
		info->accel_state->exa->Composite = RadeonComposite;
		info->accel_state->exa->DoneComposite = RadeonDoneComposite;
	} else if (IS_R200_3D) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Render acceleration "
			       "enabled for R200 type cards.\n");
		info->accel_state->exa->CheckComposite = R200CheckComposite;
		info->accel_state->exa->PrepareComposite =
		    R200PrepareComposite;
		info->accel_state->exa->Composite = RadeonComposite;
		info->accel_state->exa->DoneComposite = RadeonDoneComposite;
	} else {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Render acceleration "
			       "enabled for R100 type cards.\n");
		info->accel_state->exa->CheckComposite = R100CheckComposite;
		info->accel_state->exa->PrepareComposite =
		    R100PrepareComposite;
		info->accel_state->exa->Composite = RadeonComposite;
		info->accel_state->exa->DoneComposite = RadeonDoneComposite;
	}
    }
#endif

    info->accel_state->exa->CreatePixmap = RADEONEXACreatePixmap;
    info->accel_state->exa->DestroyPixmap = RADEONEXADestroyPixmap;
    info->accel_state->exa->PixmapIsOffscreen = RADEONEXAPixmapIsOffscreen;
    info->accel_state->exa->PrepareAccess = RADEONPrepareAccess_CS;
    info->accel_state->exa->FinishAccess = RADEONFinishAccess_CS;
    info->accel_state->exa->CreatePixmap2 = RADEONEXACreatePixmap2;
#if (EXA_VERSION_MAJOR == 2 && EXA_VERSION_MINOR >= 6) 
    info->accel_state->exa->SharePixmapBacking = RADEONEXASharePixmapBacking; 
    info->accel_state->exa->SetSharedPixmapBacking = RADEONEXASetSharedPixmapBacking;
#endif


    info->accel_state->exa->maxPitchBytes = 16320;
    info->accel_state->exa->maxX = 8191;
    info->accel_state->exa->maxY = 8191;

    if (xf86ReturnOptValBool(info->Options, OPTION_EXA_VSYNC, FALSE)) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "EXA VSync enabled\n");
	info->accel_state->vsync = TRUE;
    } else
	info->accel_state->vsync = FALSE;

    RADEONEngineInit(pScrn);

    if (!exaDriverInit(pScreen, info->accel_state->exa)) {
	free(info->accel_state->exa);
	return FALSE;
    }
    exaMarkSync(pScreen);

    return TRUE;
}

