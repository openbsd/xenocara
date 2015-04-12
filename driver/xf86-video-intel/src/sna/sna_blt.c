/*
 * Based on code from intel_uxa.c and i830_xaa.c
 * Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
 * Copyright (c) 2005 Jesse Barnes <jbarnes@virtuousgeek.org>
 * Copyright (c) 2009-2011 Intel Corporation
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
 *    Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sna.h"
#include "sna_render.h"
#include "sna_render_inline.h"
#include "sna_reg.h"
#include "rop.h"

#define NO_BLT_COMPOSITE 0
#define NO_BLT_COPY 0
#define NO_BLT_COPY_BOXES 0
#define NO_BLT_FILL 0
#define NO_BLT_FILL_BOXES 0

#ifndef PICT_TYPE_BGRA
#define PICT_TYPE_BGRA 8
#endif

static const uint8_t copy_ROP[] = {
	ROP_0,                  /* GXclear */
	ROP_DSa,                /* GXand */
	ROP_SDna,               /* GXandReverse */
	ROP_S,                  /* GXcopy */
	ROP_DSna,               /* GXandInverted */
	ROP_D,                  /* GXnoop */
	ROP_DSx,                /* GXxor */
	ROP_DSo,                /* GXor */
	ROP_DSon,               /* GXnor */
	ROP_DSxn,               /* GXequiv */
	ROP_Dn,                 /* GXinvert */
	ROP_SDno,               /* GXorReverse */
	ROP_Sn,                 /* GXcopyInverted */
	ROP_DSno,               /* GXorInverted */
	ROP_DSan,               /* GXnand */
	ROP_1                   /* GXset */
};

static const uint8_t fill_ROP[] = {
	ROP_0,
	ROP_DPa,
	ROP_PDna,
	ROP_P,
	ROP_DPna,
	ROP_D,
	ROP_DPx,
	ROP_DPo,
	ROP_DPon,
	ROP_PDxn,
	ROP_Dn,
	ROP_PDno,
	ROP_Pn,
	ROP_DPno,
	ROP_DPan,
	ROP_1
};

static void nop_done(struct sna *sna, const struct sna_composite_op *op)
{
	assert(sna->kgem.nbatch <= KGEM_BATCH_SIZE(&sna->kgem));
	if (sna->kgem.nexec > 1 && __kgem_ring_empty(&sna->kgem)) {
		DBG(("%s: flushing BLT operation on empty ring\n", __FUNCTION__));
		_kgem_submit(&sna->kgem);
	}
	(void)op;
}

static void gen6_blt_copy_done(struct sna *sna, const struct sna_composite_op *op)
{
	struct kgem *kgem = &sna->kgem;

	assert(kgem->nbatch <= KGEM_BATCH_SIZE(kgem));
	if (kgem->nexec > 1 && __kgem_ring_empty(kgem)) {
		DBG(("%s: flushing BLT operation on empty ring\n", __FUNCTION__));
		_kgem_submit(kgem);
		return;
	}

	if (kgem_check_batch(kgem, 3)) {
		uint32_t *b = kgem->batch + kgem->nbatch;
		assert(sna->kgem.mode == KGEM_BLT);
		b[0] = XY_SETUP_CLIP;
		b[1] = b[2] = 0;
		kgem->nbatch += 3;
		assert(kgem->nbatch < kgem->surface);
	}
	assert(sna->kgem.nbatch <= KGEM_BATCH_SIZE(&sna->kgem));
	(void)op;
}

static bool sna_blt_fill_init(struct sna *sna,
			      struct sna_blt_state *blt,
			      struct kgem_bo *bo,
			      int bpp,
			      uint8_t alu,
			      uint32_t pixel)
{
	struct kgem *kgem = &sna->kgem;

	assert(kgem_bo_can_blt (kgem, bo));
	assert(bo->tiling != I915_TILING_Y);
	blt->bo[0] = bo;

	blt->br13 = bo->pitch;
	blt->cmd = XY_SCANLINE_BLT;
	if (kgem->gen >= 040 && bo->tiling) {
		blt->cmd |= BLT_DST_TILED;
		blt->br13 >>= 2;
	}
	assert(blt->br13 <= MAXSHORT);

	if (alu == GXclear)
		pixel = 0;
	else if (alu == GXcopy) {
		if (pixel == 0)
			alu = GXclear;
		else if (pixel == -1)
			alu = GXset;
	}

	blt->br13 |= 1<<31 | (fill_ROP[alu] << 16);
	switch (bpp) {
	default: assert(0);
	case 32: blt->br13 |= 1 << 25; /* RGB8888 */
	case 16: blt->br13 |= 1 << 24; /* RGB565 */
	case 8: break;
	}

	blt->pixel = pixel;
	blt->bpp = bpp;
	blt->alu = alu;

	kgem_set_mode(kgem, KGEM_BLT, bo);
	if (!kgem_check_batch(kgem, 14) ||
	    !kgem_check_bo_fenced(kgem, bo)) {
		kgem_submit(kgem);
		if (!kgem_check_bo_fenced(kgem, bo))
			return false;
		_kgem_set_mode(kgem, KGEM_BLT);
	}

	if (sna->blt_state.fill_bo != bo->unique_id ||
	    sna->blt_state.fill_pixel != pixel ||
	    sna->blt_state.fill_alu != alu)
	{
		uint32_t *b;

		if (!kgem_check_reloc(kgem, 1)) {
			_kgem_submit(kgem);
			if (!kgem_check_bo_fenced(kgem, bo))
				return false;
			_kgem_set_mode(kgem, KGEM_BLT);
		}

		assert(sna->kgem.mode == KGEM_BLT);
		b = kgem->batch + kgem->nbatch;
		if (sna->kgem.gen >= 0100) {
			b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 8;
			if (bpp == 32)
				b[0] |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
			if (bo->tiling)
				b[0] |= BLT_DST_TILED;
			b[1] = blt->br13;
			b[2] = 0;
			b[3] = 0;
			*(uint64_t *)(b+4) =
				kgem_add_reloc64(kgem, kgem->nbatch + 4, bo,
						 I915_GEM_DOMAIN_RENDER << 16 |
						 I915_GEM_DOMAIN_RENDER |
						 KGEM_RELOC_FENCED,
						 0);
			b[6] = pixel;
			b[7] = pixel;
			b[8] = 0;
			b[9] = 0;
			kgem->nbatch += 10;
		} else {
			b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 7;
			if (bpp == 32)
				b[0] |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
			if (bo->tiling && kgem->gen >= 040)
				b[0] |= BLT_DST_TILED;
			b[1] = blt->br13;
			b[2] = 0;
			b[3] = 0;
			b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, bo,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      I915_GEM_DOMAIN_RENDER |
					      KGEM_RELOC_FENCED,
					      0);
			b[5] = pixel;
			b[6] = pixel;
			b[7] = 0;
			b[8] = 0;
			kgem->nbatch += 9;
		}
		assert(kgem->nbatch < kgem->surface);

		sna->blt_state.fill_bo = bo->unique_id;
		sna->blt_state.fill_pixel = pixel;
		sna->blt_state.fill_alu = alu;
	}

	return true;
}

noinline static void sna_blt_fill_begin(struct sna *sna,
					const struct sna_blt_state *blt)
{
	struct kgem *kgem = &sna->kgem;
	uint32_t *b;

	if (kgem->nreloc) {
		_kgem_submit(kgem);
		_kgem_set_mode(kgem, KGEM_BLT);
		assert(kgem->nbatch == 0);
	}

	assert(kgem->mode == KGEM_BLT);
	b = kgem->batch + kgem->nbatch;
	if (sna->kgem.gen >= 0100) {
		b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 8;
		if (blt->bpp == 32)
			b[0] |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
		if (blt->bo[0]->tiling)
			b[0] |= BLT_DST_TILED;
		b[1] = blt->br13;
		b[2] = 0;
		b[3] = 0;
		*(uint32_t *)(b+4) =
			kgem_add_reloc64(kgem, kgem->nbatch + 4, blt->bo[0],
					 I915_GEM_DOMAIN_RENDER << 16 |
					 I915_GEM_DOMAIN_RENDER |
					 KGEM_RELOC_FENCED,
					 0);
		b[6] = blt->pixel;
		b[7] = blt->pixel;
		b[8] = 0;
		b[9] = 0;
		kgem->nbatch += 10;
	} else {
		b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 7;
		if (blt->bpp == 32)
			b[0] |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
		if (blt->bo[0]->tiling && kgem->gen >= 040)
			b[0] |= BLT_DST_TILED;
		b[1] = blt->br13;
		b[2] = 0;
		b[3] = 0;
		b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, blt->bo[0],
				      I915_GEM_DOMAIN_RENDER << 16 |
				      I915_GEM_DOMAIN_RENDER |
				      KGEM_RELOC_FENCED,
				      0);
		b[5] = blt->pixel;
		b[6] = blt->pixel;
		b[7] = 0;
		b[8] = 0;
		kgem->nbatch += 9;
	}
}

inline static void sna_blt_fill_one(struct sna *sna,
				    const struct sna_blt_state *blt,
				    int16_t x, int16_t y,
				    int16_t width, int16_t height)
{
	struct kgem *kgem = &sna->kgem;
	uint32_t *b;

	DBG(("%s: (%d, %d) x (%d, %d): %08x\n",
	     __FUNCTION__, x, y, width, height, blt->pixel));

	assert(x >= 0);
	assert(y >= 0);
	assert((y+height) * blt->bo[0]->pitch <= kgem_bo_size(blt->bo[0]));

	if (!kgem_check_batch(kgem, 3))
		sna_blt_fill_begin(sna, blt);

	assert(sna->kgem.mode == KGEM_BLT);
	b = kgem->batch + kgem->nbatch;
	kgem->nbatch += 3;
	assert(kgem->nbatch < kgem->surface);

	b[0] = blt->cmd;
	b[1] = y << 16 | x;
	b[2] = b[1] + (height << 16 | width);
}

static bool sna_blt_copy_init(struct sna *sna,
			      struct sna_blt_state *blt,
			      struct kgem_bo *src,
			      struct kgem_bo *dst,
			      int bpp,
			      uint8_t alu)
{
	struct kgem *kgem = &sna->kgem;

	assert(kgem_bo_can_blt (kgem, src));
	assert(kgem_bo_can_blt (kgem, dst));

	blt->bo[0] = src;
	blt->bo[1] = dst;

	blt->cmd = XY_SRC_COPY_BLT_CMD | (kgem->gen >= 0100 ? 8 : 6);
	if (bpp == 32)
		blt->cmd |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;

	blt->pitch[0] = src->pitch;
	if (kgem->gen >= 040 && src->tiling) {
		blt->cmd |= BLT_SRC_TILED;
		blt->pitch[0] >>= 2;
	}
	assert(blt->pitch[0] <= MAXSHORT);

	blt->pitch[1] = dst->pitch;
	if (kgem->gen >= 040 && dst->tiling) {
		blt->cmd |= BLT_DST_TILED;
		blt->pitch[1] >>= 2;
	}
	assert(blt->pitch[1] <= MAXSHORT);

	blt->overwrites = alu == GXcopy || alu == GXclear || alu == GXset;
	blt->br13 = (copy_ROP[alu] << 16) | blt->pitch[1];
	switch (bpp) {
	default: assert(0);
	case 32: blt->br13 |= 1 << 25; /* RGB8888 */
	case 16: blt->br13 |= 1 << 24; /* RGB565 */
	case 8: break;
	}

	kgem_set_mode(kgem, KGEM_BLT, dst);
	if (!kgem_check_many_bo_fenced(kgem, src, dst, NULL)) {
		kgem_submit(kgem);
		if (!kgem_check_many_bo_fenced(kgem, src, dst, NULL))
			return false;
		_kgem_set_mode(kgem, KGEM_BLT);
	}

	sna->blt_state.fill_bo = 0;
	return true;
}

static bool sna_blt_alpha_fixup_init(struct sna *sna,
				     struct sna_blt_state *blt,
				     struct kgem_bo *src,
				     struct kgem_bo *dst,
				     int bpp, uint32_t alpha)
{
	struct kgem *kgem = &sna->kgem;

	DBG(("%s: dst handle=%d, src handle=%d, bpp=%d, fixup=%08x\n",
	     __FUNCTION__, dst->handle, src->handle, bpp, alpha));
	assert(kgem_bo_can_blt(kgem, src));
	assert(kgem_bo_can_blt(kgem, dst));

	blt->bo[0] = src;
	blt->bo[1] = dst;

	blt->cmd = XY_FULL_MONO_PATTERN_BLT | (kgem->gen >= 0100 ? 12 : 10);
	blt->pitch[0] = src->pitch;
	if (kgem->gen >= 040 && src->tiling) {
		blt->cmd |= BLT_SRC_TILED;
		blt->pitch[0] >>= 2;
	}
	assert(blt->pitch[0] <= MAXSHORT);

	blt->pitch[1] = dst->pitch;
	if (kgem->gen >= 040 && dst->tiling) {
		blt->cmd |= BLT_DST_TILED;
		blt->pitch[1] >>= 2;
	}
	assert(blt->pitch[1] <= MAXSHORT);

	blt->overwrites = 1;
	blt->br13 = (0xfc << 16) | blt->pitch[1];
	switch (bpp) {
	default: assert(0);
	case 32: blt->cmd |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
		 blt->br13 |= 1 << 25; /* RGB8888 */
	case 16: blt->br13 |= 1 << 24; /* RGB565 */
	case 8: break;
	}
	blt->pixel = alpha;

	kgem_set_mode(kgem, KGEM_BLT, dst);
	if (!kgem_check_many_bo_fenced(kgem, src, dst, NULL)) {
		kgem_submit(kgem);
		if (!kgem_check_many_bo_fenced(kgem, src, dst, NULL))
			return false;
		_kgem_set_mode(kgem, KGEM_BLT);
	}

	sna->blt_state.fill_bo = 0;
	return true;
}

static void sna_blt_alpha_fixup_one(struct sna *sna,
				    const struct sna_blt_state *blt,
				    int src_x, int src_y,
				    int width, int height,
				    int dst_x, int dst_y)
{
	struct kgem *kgem = &sna->kgem;
	uint32_t *b;

	DBG(("%s: (%d, %d) -> (%d, %d) x (%d, %d)\n",
	     __FUNCTION__, src_x, src_y, dst_x, dst_y, width, height));

	assert(src_x >= 0);
	assert(src_y >= 0);
	assert((src_y + height) * blt->bo[0]->pitch <= kgem_bo_size(blt->bo[0]));
	assert(dst_x >= 0);
	assert(dst_y >= 0);
	assert((dst_y + height) * blt->bo[1]->pitch <= kgem_bo_size(blt->bo[1]));
	assert(width > 0);
	assert(height > 0);

	if (!kgem_check_batch(kgem, 14) ||
	    !kgem_check_reloc(kgem, 2)) {
		_kgem_submit(kgem);
		_kgem_set_mode(kgem, KGEM_BLT);
	}

	assert(sna->kgem.mode == KGEM_BLT);
	b = kgem->batch + kgem->nbatch;
	b[0] = blt->cmd;
	b[1] = blt->br13;
	b[2] = (dst_y << 16) | dst_x;
	b[3] = ((dst_y + height) << 16) | (dst_x + width);
	if (sna->kgem.gen >= 0100) {
		*(uint64_t *)(b+4) =
			kgem_add_reloc64(kgem, kgem->nbatch + 4, blt->bo[1],
					 I915_GEM_DOMAIN_RENDER << 16 |
					 I915_GEM_DOMAIN_RENDER |
					 KGEM_RELOC_FENCED,
					 0);
		b[6] = blt->pitch[0];
		b[7] = (src_y << 16) | src_x;
		*(uint64_t *)(b+8) =
			kgem_add_reloc64(kgem, kgem->nbatch + 8, blt->bo[0],
					 I915_GEM_DOMAIN_RENDER << 16 |
					 KGEM_RELOC_FENCED,
					 0);
		b[10] = blt->pixel;
		b[11] = blt->pixel;
		b[12] = 0;
		b[13] = 0;
		kgem->nbatch += 14;
	} else {
		b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, blt->bo[1],
				      I915_GEM_DOMAIN_RENDER << 16 |
				      I915_GEM_DOMAIN_RENDER |
				      KGEM_RELOC_FENCED,
				      0);
		b[5] = blt->pitch[0];
		b[6] = (src_y << 16) | src_x;
		b[7] = kgem_add_reloc(kgem, kgem->nbatch + 7, blt->bo[0],
				      I915_GEM_DOMAIN_RENDER << 16 |
				      KGEM_RELOC_FENCED,
				      0);
		b[8] = blt->pixel;
		b[9] = blt->pixel;
		b[10] = 0;
		b[11] = 0;
		kgem->nbatch += 12;
	}
	assert(kgem->nbatch < kgem->surface);
}

static void sna_blt_copy_one(struct sna *sna,
			     const struct sna_blt_state *blt,
			     int src_x, int src_y,
			     int width, int height,
			     int dst_x, int dst_y)
{
	struct kgem *kgem = &sna->kgem;
	uint32_t *b;

	DBG(("%s: (%d, %d) -> (%d, %d) x (%d, %d)\n",
	     __FUNCTION__, src_x, src_y, dst_x, dst_y, width, height));

	assert(src_x >= 0);
	assert(src_y >= 0);
	assert((src_y + height) * blt->bo[0]->pitch <= kgem_bo_size(blt->bo[0]));
	assert(dst_x >= 0);
	assert(dst_y >= 0);
	assert((dst_y + height) * blt->bo[1]->pitch <= kgem_bo_size(blt->bo[1]));
	assert(width > 0);
	assert(height > 0);

	/* Compare against a previous fill */
	if (blt->overwrites &&
	    kgem->reloc[kgem->nreloc-1].target_handle == blt->bo[1]->target_handle) {
		if (sna->kgem.gen >= 0100) {
			if (kgem->nbatch >= 7 &&
			    kgem->batch[kgem->nbatch-7] == (XY_COLOR_BLT | (blt->cmd & (BLT_DST_TILED | BLT_WRITE_ALPHA | BLT_WRITE_RGB)) | 5) &&
			    kgem->batch[kgem->nbatch-5] == ((uint32_t)dst_y << 16 | (uint16_t)dst_x) &&
			    kgem->batch[kgem->nbatch-4] == ((uint32_t)(dst_y+height) << 16 | (uint16_t)(dst_x+width))) {
				DBG(("%s: replacing last fill\n", __FUNCTION__));
				if (kgem_check_batch(kgem, 3)) {
					assert(kgem->mode == KGEM_BLT);
					b = kgem->batch + kgem->nbatch - 7;
					b[0] = blt->cmd;
					b[1] = blt->br13;
					b[6] = (src_y << 16) | src_x;
					b[7] = blt->pitch[0];
					*(uint64_t *)(b+8) =
						kgem_add_reloc64(kgem, kgem->nbatch + 8 - 7, blt->bo[0],
								 I915_GEM_DOMAIN_RENDER << 16 |
								 KGEM_RELOC_FENCED,
								 0);
					kgem->nbatch += 3;
					assert(kgem->nbatch < kgem->surface);
					return;
				}
				kgem->nbatch -= 7;
				kgem->nreloc--;
			}
		} else {
			if (kgem->nbatch >= 6 &&
			    kgem->batch[kgem->nbatch-6] == (XY_COLOR_BLT | (blt->cmd & (BLT_DST_TILED | BLT_WRITE_ALPHA | BLT_WRITE_RGB)) | 4) &&
			    kgem->batch[kgem->nbatch-4] == ((uint32_t)dst_y << 16 | (uint16_t)dst_x) &&
			    kgem->batch[kgem->nbatch-3] == ((uint32_t)(dst_y+height) << 16 | (uint16_t)(dst_x+width))) {
				DBG(("%s: replacing last fill\n", __FUNCTION__));
				if (kgem_check_batch(kgem, 8-6)) {
					assert(kgem->mode == KGEM_BLT);
					b = kgem->batch + kgem->nbatch - 6;
					b[0] = blt->cmd;
					b[1] = blt->br13;
					b[5] = (src_y << 16) | src_x;
					b[6] = blt->pitch[0];
					b[7] = kgem_add_reloc(kgem, kgem->nbatch + 7 - 6, blt->bo[0],
							      I915_GEM_DOMAIN_RENDER << 16 |
							      KGEM_RELOC_FENCED,
							      0);
					kgem->nbatch += 8 - 6;
					assert(kgem->nbatch < kgem->surface);
					return;
				}
				kgem->nbatch -= 6;
				kgem->nreloc--;
			}
		}
	}

	if (!kgem_check_batch(kgem, 10) ||
	    !kgem_check_reloc(kgem, 2)) {
		_kgem_submit(kgem);
		_kgem_set_mode(kgem, KGEM_BLT);
	}

	assert(sna->kgem.mode == KGEM_BLT);
	b = kgem->batch + kgem->nbatch;
	b[0] = blt->cmd;
	b[1] = blt->br13;
	b[2] = (dst_y << 16) | dst_x;
	b[3] = ((dst_y + height) << 16) | (dst_x + width);
	if (kgem->gen >= 0100) {
		*(uint64_t *)(b+4) =
			kgem_add_reloc64(kgem, kgem->nbatch + 4, blt->bo[1],
					 I915_GEM_DOMAIN_RENDER << 16 |
					 I915_GEM_DOMAIN_RENDER |
					 KGEM_RELOC_FENCED,
					 0);
		b[6] = (src_y << 16) | src_x;
		b[7] = blt->pitch[0];
		*(uint64_t *)(b+8) =
			kgem_add_reloc64(kgem, kgem->nbatch + 8, blt->bo[0],
					 I915_GEM_DOMAIN_RENDER << 16 |
					 KGEM_RELOC_FENCED,
					 0);
		kgem->nbatch += 10;
	} else {
		b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, blt->bo[1],
				      I915_GEM_DOMAIN_RENDER << 16 |
				      I915_GEM_DOMAIN_RENDER |
				      KGEM_RELOC_FENCED,
				      0);
		b[5] = (src_y << 16) | src_x;
		b[6] = blt->pitch[0];
		b[7] = kgem_add_reloc(kgem, kgem->nbatch + 7, blt->bo[0],
				      I915_GEM_DOMAIN_RENDER << 16 |
				      KGEM_RELOC_FENCED,
				      0);
		kgem->nbatch += 8;
	}
	assert(kgem->nbatch < kgem->surface);
}

bool
sna_get_rgba_from_pixel(uint32_t pixel,
			uint16_t *red,
			uint16_t *green,
			uint16_t *blue,
			uint16_t *alpha,
			uint32_t format)
{
	int rbits, bbits, gbits, abits;
	int rshift, bshift, gshift, ashift;

	rbits = PICT_FORMAT_R(format);
	gbits = PICT_FORMAT_G(format);
	bbits = PICT_FORMAT_B(format);
	abits = PICT_FORMAT_A(format);

	if (PICT_FORMAT_TYPE(format) == PICT_TYPE_A) {
		rshift = gshift = bshift = ashift = 0;
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ARGB) {
		bshift = 0;
		gshift = bbits;
		rshift = gshift + gbits;
		ashift = rshift + rbits;
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ABGR) {
		rshift = 0;
		gshift = rbits;
		bshift = gshift + gbits;
		ashift = bshift + bbits;
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_BGRA) {
		ashift = 0;
		rshift = abits;
		if (abits == 0)
			rshift = PICT_FORMAT_BPP(format) - (rbits+gbits+bbits);
		gshift = rshift + rbits;
		bshift = gshift + gbits;
	} else {
		return false;
	}

	if (rbits) {
		*red = ((pixel >> rshift) & ((1 << rbits) - 1)) << (16 - rbits);
		while (rbits < 16) {
			*red |= *red >> rbits;
			rbits <<= 1;
		}
	} else
		*red = 0;

	if (gbits) {
		*green = ((pixel >> gshift) & ((1 << gbits) - 1)) << (16 - gbits);
		while (gbits < 16) {
			*green |= *green >> gbits;
			gbits <<= 1;
		}
	} else
		*green = 0;

	if (bbits) {
		*blue = ((pixel >> bshift) & ((1 << bbits) - 1)) << (16 - bbits);
		while (bbits < 16) {
			*blue |= *blue >> bbits;
			bbits <<= 1;
		}
	} else
		*blue = 0;

	if (abits) {
		*alpha = ((pixel >> ashift) & ((1 << abits) - 1)) << (16 - abits);
		while (abits < 16) {
			*alpha |= *alpha >> abits;
			abits <<= 1;
		}
	} else
		*alpha = 0xffff;

	return true;
}

bool
_sna_get_pixel_from_rgba(uint32_t * pixel,
			uint16_t red,
			uint16_t green,
			uint16_t blue,
			uint16_t alpha,
			uint32_t format)
{
	int rbits, bbits, gbits, abits;
	int rshift, bshift, gshift, ashift;

	rbits = PICT_FORMAT_R(format);
	gbits = PICT_FORMAT_G(format);
	bbits = PICT_FORMAT_B(format);
	abits = PICT_FORMAT_A(format);
	if (abits == 0)
	    abits = PICT_FORMAT_BPP(format) - (rbits+gbits+bbits);

	if (PICT_FORMAT_TYPE(format) == PICT_TYPE_A) {
		*pixel = alpha >> (16 - abits);
		return true;
	}

	if (!PICT_FORMAT_COLOR(format))
		return false;

	if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ARGB) {
		bshift = 0;
		gshift = bbits;
		rshift = gshift + gbits;
		ashift = rshift + rbits;
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ABGR) {
		rshift = 0;
		gshift = rbits;
		bshift = gshift + gbits;
		ashift = bshift + bbits;
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_BGRA) {
		ashift = 0;
		rshift = abits;
		gshift = rshift + rbits;
		bshift = gshift + gbits;
	} else
		return false;

	*pixel = 0;
	*pixel |= (blue  >> (16 - bbits)) << bshift;
	*pixel |= (green >> (16 - gbits)) << gshift;
	*pixel |= (red   >> (16 - rbits)) << rshift;
	*pixel |= (alpha >> (16 - abits)) << ashift;

	return true;
}

uint32_t
sna_rgba_for_color(uint32_t color, int depth)
{
	return color_convert(color, sna_format_for_depth(depth), PICT_a8r8g8b8);
}

uint32_t
sna_rgba_to_color(uint32_t rgba, uint32_t format)
{
	return color_convert(rgba, PICT_a8r8g8b8, format);
}

static uint32_t
get_pixel(PicturePtr picture)
{
	PixmapPtr pixmap = get_drawable_pixmap(picture->pDrawable);

	DBG(("%s: %p\n", __FUNCTION__, pixmap));

	if (!sna_pixmap_move_to_cpu(pixmap, MOVE_READ))
		return 0;

	switch (pixmap->drawable.bitsPerPixel) {
	case 32: return *(uint32_t *)pixmap->devPrivate.ptr;
	case 16: return *(uint16_t *)pixmap->devPrivate.ptr;
	default: return *(uint8_t *)pixmap->devPrivate.ptr;
	}
}

static uint32_t
get_solid_color(PicturePtr picture, uint32_t format)
{
	if (picture->pSourcePict) {
		PictSolidFill *fill = (PictSolidFill *)picture->pSourcePict;
		return color_convert(fill->color, PICT_a8r8g8b8, format);
	} else
		return color_convert(get_pixel(picture), picture->format, format);
}

static bool
is_solid(PicturePtr picture)
{
	if (picture->pSourcePict) {
		if (picture->pSourcePict->type == SourcePictTypeSolidFill)
			return true;
	}

	if (picture->pDrawable) {
		if (picture->pDrawable->width  == 1 &&
		    picture->pDrawable->height == 1 &&
		    picture->repeat)
			return true;
	}

	return false;
}

bool
sna_picture_is_solid(PicturePtr picture, uint32_t *color)
{
	if (!is_solid(picture))
		return false;

	if (color)
		*color = get_solid_color(picture, PICT_a8r8g8b8);
	return true;
}

static bool
pixel_is_transparent(uint32_t pixel, uint32_t format)
{
	unsigned int abits;

	abits = PICT_FORMAT_A(format);
	if (!abits)
		return false;

	if (PICT_FORMAT_TYPE(format) == PICT_TYPE_A ||
	    PICT_FORMAT_TYPE(format) == PICT_TYPE_BGRA) {
		return (pixel & ((1 << abits) - 1)) == 0;
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ARGB ||
		   PICT_FORMAT_TYPE(format) == PICT_TYPE_ABGR) {
		unsigned int ashift = PICT_FORMAT_BPP(format) - abits;
		return (pixel >> ashift) == 0;
	} else
		return false;
}

static bool
pixel_is_opaque(uint32_t pixel, uint32_t format)
{
	unsigned int abits;

	abits = PICT_FORMAT_A(format);
	if (!abits)
		return true;

	if (PICT_FORMAT_TYPE(format) == PICT_TYPE_A ||
	    PICT_FORMAT_TYPE(format) == PICT_TYPE_BGRA) {
		return (pixel & ((1 << abits) - 1)) == (unsigned)((1 << abits) - 1);
	} else if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ARGB ||
		   PICT_FORMAT_TYPE(format) == PICT_TYPE_ABGR) {
		unsigned int ashift = PICT_FORMAT_BPP(format) - abits;
		return (pixel >> ashift) == (unsigned)((1 << abits) - 1);
	} else
		return false;
}

static bool
pixel_is_white(uint32_t pixel, uint32_t format)
{
	switch (PICT_FORMAT_TYPE(format)) {
	case PICT_TYPE_A:
	case PICT_TYPE_ARGB:
	case PICT_TYPE_ABGR:
	case PICT_TYPE_BGRA:
		return pixel == ((1U << PICT_FORMAT_BPP(format)) - 1);
	default:
		return false;
	}
}

static bool
is_opaque_solid(PicturePtr picture)
{
	if (picture->pSourcePict) {
		PictSolidFill *fill = (PictSolidFill *) picture->pSourcePict;
		return (fill->color >> 24) == 0xff;
	} else
		return pixel_is_opaque(get_pixel(picture), picture->format);
}

static bool
is_white(PicturePtr picture)
{
	if (picture->pSourcePict) {
		PictSolidFill *fill = (PictSolidFill *) picture->pSourcePict;
		return fill->color == 0xffffffff;
	} else
		return pixel_is_white(get_pixel(picture), picture->format);
}

static bool
is_transparent(PicturePtr picture)
{
	if (picture->pSourcePict) {
		PictSolidFill *fill = (PictSolidFill *) picture->pSourcePict;
		return fill->color == 0;
	} else
		return pixel_is_transparent(get_pixel(picture), picture->format);
}

bool
sna_composite_mask_is_opaque(PicturePtr mask)
{
	if (mask->componentAlpha && PICT_FORMAT_RGB(mask->format))
		return is_solid(mask) && is_white(mask);
	else if (!PICT_FORMAT_A(mask->format))
		return true;
	else
		return is_solid(mask) && is_opaque_solid(mask);
}

fastcall
static void blt_composite_fill(struct sna *sna,
			       const struct sna_composite_op *op,
			       const struct sna_composite_rectangles *r)
{
	int x1, x2, y1, y2;

	x1 = r->dst.x + op->dst.x;
	y1 = r->dst.y + op->dst.y;
	x2 = x1 + r->width;
	y2 = y1 + r->height;

	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;

	if (x2 > op->dst.width)
		x2 = op->dst.width;
	if (y2 > op->dst.height)
		y2 = op->dst.height;

	if (x2 <= x1 || y2 <= y1)
		return;

	sna_blt_fill_one(sna, &op->u.blt, x1, y1, x2-x1, y2-y1);
}

fastcall
static void blt_composite_fill__cpu(struct sna *sna,
				    const struct sna_composite_op *op,
				    const struct sna_composite_rectangles *r)
{
	int x1, x2, y1, y2;

	x1 = r->dst.x + op->dst.x;
	y1 = r->dst.y + op->dst.y;
	x2 = x1 + r->width;
	y2 = y1 + r->height;

	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;

	if (x2 > op->dst.width)
		x2 = op->dst.width;
	if (y2 > op->dst.height)
		y2 = op->dst.height;

	if (x2 <= x1 || y2 <= y1)
		return;

	assert(op->dst.pixmap->devPrivate.ptr);
	assert(op->dst.pixmap->devKind);
	pixman_fill(op->dst.pixmap->devPrivate.ptr,
		    op->dst.pixmap->devKind / sizeof(uint32_t),
		    op->dst.pixmap->drawable.bitsPerPixel,
		    x1, y1, x2-x1, y2-y1,
		    op->u.blt.pixel);
}

fastcall static void
blt_composite_fill_box_no_offset__cpu(struct sna *sna,
				      const struct sna_composite_op *op,
				      const BoxRec *box)
{
	assert(box->x1 >= 0);
	assert(box->y1 >= 0);
	assert(box->x2 <= op->dst.pixmap->drawable.width);
	assert(box->y2 <= op->dst.pixmap->drawable.height);

	assert(op->dst.pixmap->devPrivate.ptr);
	assert(op->dst.pixmap->devKind);
	pixman_fill(op->dst.pixmap->devPrivate.ptr,
		    op->dst.pixmap->devKind / sizeof(uint32_t),
		    op->dst.pixmap->drawable.bitsPerPixel,
		    box->x1, box->y1, box->x2-box->x1, box->y2-box->y1,
		    op->u.blt.pixel);
}

static void
blt_composite_fill_boxes_no_offset__cpu(struct sna *sna,
					const struct sna_composite_op *op,
					const BoxRec *box, int n)
{
	do {
		assert(box->x1 >= 0);
		assert(box->y1 >= 0);
		assert(box->x2 <= op->dst.pixmap->drawable.width);
		assert(box->y2 <= op->dst.pixmap->drawable.height);

		assert(op->dst.pixmap->devPrivate.ptr);
		assert(op->dst.pixmap->devKind);
		pixman_fill(op->dst.pixmap->devPrivate.ptr,
			    op->dst.pixmap->devKind / sizeof(uint32_t),
			    op->dst.pixmap->drawable.bitsPerPixel,
			    box->x1, box->y1, box->x2-box->x1, box->y2-box->y1,
			    op->u.blt.pixel);
		box++;
	} while (--n);
}

fastcall static void
blt_composite_fill_box__cpu(struct sna *sna,
			    const struct sna_composite_op *op,
			    const BoxRec *box)
{
	assert(box->x1 + op->dst.x >= 0);
	assert(box->y1 + op->dst.y >= 0);
	assert(box->x2 + op->dst.x <= op->dst.pixmap->drawable.width);
	assert(box->y2 + op->dst.y <= op->dst.pixmap->drawable.height);

	assert(op->dst.pixmap->devPrivate.ptr);
	assert(op->dst.pixmap->devKind);
	pixman_fill(op->dst.pixmap->devPrivate.ptr,
		    op->dst.pixmap->devKind / sizeof(uint32_t),
		    op->dst.pixmap->drawable.bitsPerPixel,
		    box->x1 + op->dst.x, box->y1 + op->dst.y,
		    box->x2 - box->x1, box->y2 - box->y1,
		    op->u.blt.pixel);
}

static void
blt_composite_fill_boxes__cpu(struct sna *sna,
			      const struct sna_composite_op *op,
			      const BoxRec *box, int n)
{
	do {
		assert(box->x1 + op->dst.x >= 0);
		assert(box->y1 + op->dst.y >= 0);
		assert(box->x2 + op->dst.x <= op->dst.pixmap->drawable.width);
		assert(box->y2 + op->dst.y <= op->dst.pixmap->drawable.height);

		assert(op->dst.pixmap->devPrivate.ptr);
		assert(op->dst.pixmap->devKind);
		pixman_fill(op->dst.pixmap->devPrivate.ptr,
			    op->dst.pixmap->devKind / sizeof(uint32_t),
			    op->dst.pixmap->drawable.bitsPerPixel,
			    box->x1 + op->dst.x, box->y1 + op->dst.y,
			    box->x2 - box->x1, box->y2 - box->y1,
			    op->u.blt.pixel);
		box++;
	} while (--n);
}

inline static void _sna_blt_fill_box(struct sna *sna,
				     const struct sna_blt_state *blt,
				     const BoxRec *box)
{
	struct kgem *kgem = &sna->kgem;
	uint32_t *b;

	DBG(("%s: (%d, %d), (%d, %d): %08x\n", __FUNCTION__,
	     box->x1, box->y1, box->x2, box->y2,
	     blt->pixel));

	assert(box->x1 >= 0);
	assert(box->y1 >= 0);
	assert(box->y2 * blt->bo[0]->pitch <= kgem_bo_size(blt->bo[0]));

	if (!kgem_check_batch(kgem, 3))
		sna_blt_fill_begin(sna, blt);

	assert(sna->kgem.mode == KGEM_BLT);
	b = kgem->batch + kgem->nbatch;
	kgem->nbatch += 3;
	assert(kgem->nbatch < kgem->surface);

	b[0] = blt->cmd;
	*(uint64_t *)(b+1) = *(const uint64_t *)box;
}

inline static void _sna_blt_fill_boxes(struct sna *sna,
				       const struct sna_blt_state *blt,
				       const BoxRec *box,
				       int nbox)
{
	struct kgem *kgem = &sna->kgem;
	uint32_t cmd = blt->cmd;

	DBG(("%s: %08x x %d\n", __FUNCTION__, blt->pixel, nbox));

	if (!kgem_check_batch(kgem, 3))
		sna_blt_fill_begin(sna, blt);

	do {
		uint32_t *b = kgem->batch + kgem->nbatch;
		int nbox_this_time;

		assert(sna->kgem.mode == KGEM_BLT);
		nbox_this_time = nbox;
		if (3*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
			nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 3;
		assert(nbox_this_time);
		nbox -= nbox_this_time;

		kgem->nbatch += 3 * nbox_this_time;
		assert(kgem->nbatch < kgem->surface);
		while (nbox_this_time >= 8) {
			b[0] = cmd; *(uint64_t *)(b+1) = *(const uint64_t *)box++;
			b[3] = cmd; *(uint64_t *)(b+4) = *(const uint64_t *)box++;
			b[6] = cmd; *(uint64_t *)(b+7) = *(const uint64_t *)box++;
			b[9] = cmd; *(uint64_t *)(b+10) = *(const uint64_t *)box++;
			b[12] = cmd; *(uint64_t *)(b+13) = *(const uint64_t *)box++;
			b[15] = cmd; *(uint64_t *)(b+16) = *(const uint64_t *)box++;
			b[18] = cmd; *(uint64_t *)(b+19) = *(const uint64_t *)box++;
			b[21] = cmd; *(uint64_t *)(b+22) = *(const uint64_t *)box++;
			b += 24;
			nbox_this_time -= 8;
		}
		if (nbox_this_time & 4) {
			b[0] = cmd; *(uint64_t *)(b+1) = *(const uint64_t *)box++;
			b[3] = cmd; *(uint64_t *)(b+4) = *(const uint64_t *)box++;
			b[6] = cmd; *(uint64_t *)(b+7) = *(const uint64_t *)box++;
			b[9] = cmd; *(uint64_t *)(b+10) = *(const uint64_t *)box++;
			b += 12;
		}
		if (nbox_this_time & 2) {
			b[0] = cmd; *(uint64_t *)(b+1) = *(const uint64_t *)box++;
			b[3] = cmd; *(uint64_t *)(b+4) = *(const uint64_t *)box++;
			b += 6;
		}
		if (nbox_this_time & 1) {
			b[0] = cmd; *(uint64_t *)(b+1) = *(const uint64_t *)box++;
		}

		if (!nbox)
			return;

		sna_blt_fill_begin(sna, blt);
	} while (1);
}

static inline void _sna_blt_maybe_clear(const struct sna_composite_op *op, const BoxRec *box)
{
	if (box->x2 - box->x1 >= op->dst.width &&
	    box->y2 - box->y1 >= op->dst.height) {
		struct sna_pixmap *priv = sna_pixmap(op->dst.pixmap);
		if (op->dst.bo == priv->gpu_bo) {
			priv->clear = true;
			priv->clear_color = op->u.blt.pixel;
			DBG(("%s: pixmap=%ld marking clear [%08x]\n",
			     __FUNCTION__,
			     op->dst.pixmap->drawable.serialNumber,
			     op->u.blt.pixel));
		}
	}
}

fastcall static void blt_composite_fill_box_no_offset(struct sna *sna,
						      const struct sna_composite_op *op,
						      const BoxRec *box)
{
	_sna_blt_fill_box(sna, &op->u.blt, box);
	_sna_blt_maybe_clear(op, box);
}

static void blt_composite_fill_boxes_no_offset(struct sna *sna,
					       const struct sna_composite_op *op,
					       const BoxRec *box, int n)
{
	_sna_blt_fill_boxes(sna, &op->u.blt, box, n);
}

static void blt_composite_fill_boxes_no_offset__thread(struct sna *sna,
						       const struct sna_composite_op *op,
						       const BoxRec *box, int nbox)
{
	struct kgem *kgem = &sna->kgem;
	const struct sna_blt_state *blt = &op->u.blt;
	uint32_t cmd = blt->cmd;

	DBG(("%s: %08x x %d\n", __FUNCTION__, blt->pixel, nbox));

	sna_vertex_lock(&sna->render);
	assert(kgem->mode == KGEM_BLT);
	if (!kgem_check_batch(kgem, 3)) {
		sna_vertex_wait__locked(&sna->render);
		sna_blt_fill_begin(sna, blt);
	}

	do {
		uint32_t *b = kgem->batch + kgem->nbatch;
		int nbox_this_time;

		assert(sna->kgem.mode == KGEM_BLT);
		nbox_this_time = nbox;
		if (3*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
			nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 3;
		assert(nbox_this_time);
		nbox -= nbox_this_time;

		kgem->nbatch += 3 * nbox_this_time;
		assert(kgem->nbatch < kgem->surface);
		sna_vertex_acquire__locked(&sna->render);
		sna_vertex_unlock(&sna->render);

		while (nbox_this_time >= 8) {
			b[0] = cmd; *(uint64_t *)(b+1) = *(const uint64_t *)box++;
			b[3] = cmd; *(uint64_t *)(b+4) = *(const uint64_t *)box++;
			b[6] = cmd; *(uint64_t *)(b+7) = *(const uint64_t *)box++;
			b[9] = cmd; *(uint64_t *)(b+10) = *(const uint64_t *)box++;
			b[12] = cmd; *(uint64_t *)(b+13) = *(const uint64_t *)box++;
			b[15] = cmd; *(uint64_t *)(b+16) = *(const uint64_t *)box++;
			b[18] = cmd; *(uint64_t *)(b+19) = *(const uint64_t *)box++;
			b[21] = cmd; *(uint64_t *)(b+22) = *(const uint64_t *)box++;
			b += 24;
			nbox_this_time -= 8;
		}
		if (nbox_this_time & 4) {
			b[0] = cmd; *(uint64_t *)(b+1) = *(const uint64_t *)box++;
			b[3] = cmd; *(uint64_t *)(b+4) = *(const uint64_t *)box++;
			b[6] = cmd; *(uint64_t *)(b+7) = *(const uint64_t *)box++;
			b[9] = cmd; *(uint64_t *)(b+10) = *(const uint64_t *)box++;
			b += 12;
		}
		if (nbox_this_time & 2) {
			b[0] = cmd; *(uint64_t *)(b+1) = *(const uint64_t *)box++;
			b[3] = cmd; *(uint64_t *)(b+4) = *(const uint64_t *)box++;
			b += 6;
		}
		if (nbox_this_time & 1) {
			b[0] = cmd; *(uint64_t *)(b+1) = *(const uint64_t *)box++;
		}

		sna_vertex_lock(&sna->render);
		sna_vertex_release__locked(&sna->render);
		if (!nbox)
			break;

		sna_vertex_wait__locked(&sna->render);
		sna_blt_fill_begin(sna, blt);
	} while (1);
	sna_vertex_unlock(&sna->render);
}

fastcall static void blt_composite_fill_box(struct sna *sna,
					    const struct sna_composite_op *op,
					    const BoxRec *box)
{
	sna_blt_fill_one(sna, &op->u.blt,
			 box->x1 + op->dst.x,
			 box->y1 + op->dst.y,
			 box->x2 - box->x1,
			 box->y2 - box->y1);
	_sna_blt_maybe_clear(op, box);
}

static void blt_composite_fill_boxes(struct sna *sna,
				     const struct sna_composite_op *op,
				     const BoxRec *box, int n)
{
	do {
		sna_blt_fill_one(sna, &op->u.blt,
				 box->x1 + op->dst.x, box->y1 + op->dst.y,
				 box->x2 - box->x1, box->y2 - box->y1);
		box++;
	} while (--n);
}

static inline uint64_t add4(const BoxRec *b, int16_t x, int16_t y)
{
	union {
		uint64_t v;
		int16_t i[4];
	} vi;
	vi.v = *(uint64_t *)b;
	vi.i[0] += x;
	vi.i[1] += y;
	vi.i[2] += x;
	vi.i[3] += y;
	return vi.v;
}

static void blt_composite_fill_boxes__thread(struct sna *sna,
					     const struct sna_composite_op *op,
					     const BoxRec *box, int nbox)
{
	struct kgem *kgem = &sna->kgem;
	const struct sna_blt_state *blt = &op->u.blt;
	uint32_t cmd = blt->cmd;
	int16_t dx = op->dst.x;
	int16_t dy = op->dst.y;

	DBG(("%s: %08x x %d\n", __FUNCTION__, blt->pixel, nbox));

	sna_vertex_lock(&sna->render);
	assert(kgem->mode == KGEM_BLT);
	if (!kgem_check_batch(kgem, 3)) {
		sna_vertex_wait__locked(&sna->render);
		sna_blt_fill_begin(sna, blt);
	}

	do {
		uint32_t *b = kgem->batch + kgem->nbatch;
		int nbox_this_time;

		assert(sna->kgem.mode == KGEM_BLT);
		nbox_this_time = nbox;
		if (3*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
			nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 3;
		assert(nbox_this_time);
		nbox -= nbox_this_time;

		kgem->nbatch += 3 * nbox_this_time;
		assert(kgem->nbatch < kgem->surface);
		sna_vertex_acquire__locked(&sna->render);
		sna_vertex_unlock(&sna->render);

		while (nbox_this_time >= 8) {
			b[0] = cmd; *(uint64_t *)(b+1) = add4(box++, dx, dy);
			b[3] = cmd; *(uint64_t *)(b+4) = add4(box++, dx, dy);
			b[6] = cmd; *(uint64_t *)(b+7) = add4(box++, dx, dy);
			b[9] = cmd; *(uint64_t *)(b+10) = add4(box++, dx, dy);
			b[12] = cmd; *(uint64_t *)(b+13) = add4(box++, dx, dy);
			b[15] = cmd; *(uint64_t *)(b+16) = add4(box++, dx, dy);
			b[18] = cmd; *(uint64_t *)(b+19) = add4(box++, dx, dy);
			b[21] = cmd; *(uint64_t *)(b+22) = add4(box++, dx, dy);
			b += 24;
			nbox_this_time -= 8;
		}
		if (nbox_this_time & 4) {
			b[0] = cmd; *(uint64_t *)(b+1) = add4(box++, dx, dy);
			b[3] = cmd; *(uint64_t *)(b+4) = add4(box++, dx, dy);
			b[6] = cmd; *(uint64_t *)(b+7) = add4(box++, dx, dy);
			b[9] = cmd; *(uint64_t *)(b+10) = add4(box++, dx, dy);
			b += 12;
		}
		if (nbox_this_time & 2) {
			b[0] = cmd; *(uint64_t *)(b+1) = add4(box++, dx, dy);
			b[3] = cmd; *(uint64_t *)(b+4) = add4(box++, dx, dy);
			b += 6;
		}
		if (nbox_this_time & 1) {
			b[0] = cmd; *(uint64_t *)(b+1) = add4(box++, dx, dy);
		}

		sna_vertex_lock(&sna->render);
		sna_vertex_release__locked(&sna->render);
		if (!nbox)
			break;

		sna_vertex_wait__locked(&sna->render);
		sna_blt_fill_begin(sna, blt);
	} while (1);
	sna_vertex_unlock(&sna->render);
}

fastcall
static void blt_composite_nop(struct sna *sna,
			       const struct sna_composite_op *op,
			       const struct sna_composite_rectangles *r)
{
}

fastcall static void blt_composite_nop_box(struct sna *sna,
					   const struct sna_composite_op *op,
					   const BoxRec *box)
{
}

static void blt_composite_nop_boxes(struct sna *sna,
				    const struct sna_composite_op *op,
				    const BoxRec *box, int n)
{
}

static bool
begin_blt(struct sna *sna,
	  struct sna_composite_op *op)
{
	if (!kgem_check_bo_fenced(&sna->kgem, op->dst.bo)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_bo_fenced(&sna->kgem, op->dst.bo))
			return false;

		_kgem_set_mode(&sna->kgem, KGEM_BLT);
	}

	return true;
}

static bool
prepare_blt_nop(struct sna *sna,
		struct sna_composite_op *op)
{
	DBG(("%s\n", __FUNCTION__));

	op->blt   = blt_composite_nop;
	op->box   = blt_composite_nop_box;
	op->boxes = blt_composite_nop_boxes;
	op->done  = nop_done;
	return true;
}

static bool
prepare_blt_clear(struct sna *sna,
		  struct sna_composite_op *op)
{
	DBG(("%s\n", __FUNCTION__));

	if (op->dst.bo == NULL) {
		op->blt   = blt_composite_fill__cpu;
		if (op->dst.x|op->dst.y) {
			op->box   = blt_composite_fill_box__cpu;
			op->boxes = blt_composite_fill_boxes__cpu;
			op->thread_boxes = blt_composite_fill_boxes__cpu;
		} else {
			op->box   = blt_composite_fill_box_no_offset__cpu;
			op->boxes = blt_composite_fill_boxes_no_offset__cpu;
			op->thread_boxes = blt_composite_fill_boxes_no_offset__cpu;
		}
		op->done  = nop_done;
		op->u.blt.pixel = 0;
		return true;
	}

	op->blt = blt_composite_fill;
	if (op->dst.x|op->dst.y) {
		op->box   = blt_composite_fill_box;
		op->boxes = blt_composite_fill_boxes;
		op->thread_boxes = blt_composite_fill_boxes__thread;
	} else {
		op->box   = blt_composite_fill_box_no_offset;
		op->boxes = blt_composite_fill_boxes_no_offset;
		op->thread_boxes = blt_composite_fill_boxes_no_offset__thread;
	}
	op->done = nop_done;

	if (!sna_blt_fill_init(sna, &op->u.blt,
			       op->dst.bo,
			       op->dst.pixmap->drawable.bitsPerPixel,
			       GXclear, 0))
		return false;

	return begin_blt(sna, op);
}

static bool
prepare_blt_fill(struct sna *sna,
		 struct sna_composite_op *op,
		 uint32_t pixel)
{
	DBG(("%s\n", __FUNCTION__));

	if (op->dst.bo == NULL) {
		op->u.blt.pixel = pixel;
		op->blt = blt_composite_fill__cpu;
		if (op->dst.x|op->dst.y) {
			op->box   = blt_composite_fill_box__cpu;
			op->boxes = blt_composite_fill_boxes__cpu;
			op->thread_boxes = blt_composite_fill_boxes__cpu;
		} else {
			op->box   = blt_composite_fill_box_no_offset__cpu;
			op->boxes = blt_composite_fill_boxes_no_offset__cpu;
			op->thread_boxes = blt_composite_fill_boxes_no_offset__cpu;
		}
		op->done = nop_done;
		return true;
	}

	op->blt = blt_composite_fill;
	if (op->dst.x|op->dst.y) {
		op->box   = blt_composite_fill_box;
		op->boxes = blt_composite_fill_boxes;
		op->thread_boxes = blt_composite_fill_boxes__thread;
	} else {
		op->box   = blt_composite_fill_box_no_offset;
		op->boxes = blt_composite_fill_boxes_no_offset;
		op->thread_boxes = blt_composite_fill_boxes_no_offset__thread;
	}
	op->done = nop_done;

	if (!sna_blt_fill_init(sna, &op->u.blt, op->dst.bo,
			       op->dst.pixmap->drawable.bitsPerPixel,
			       GXcopy, pixel))
		return false;

	return begin_blt(sna, op);
}

fastcall static void
blt_composite_copy(struct sna *sna,
		   const struct sna_composite_op *op,
		   const struct sna_composite_rectangles *r)
{
	int x1, x2, y1, y2;
	int src_x, src_y;

	DBG(("%s: src=(%d, %d), dst=(%d, %d), size=(%d, %d)\n",
	     __FUNCTION__,
	     r->src.x, r->src.y,
	     r->dst.x, r->dst.y,
	     r->width, r->height));

	/* XXX higher layer should have clipped? */

	x1 = r->dst.x + op->dst.x;
	y1 = r->dst.y + op->dst.y;
	x2 = x1 + r->width;
	y2 = y1 + r->height;

	src_x = r->src.x - x1 + op->u.blt.sx;
	src_y = r->src.y - y1 + op->u.blt.sy;

	/* clip against dst */
	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;

	if (x2 > op->dst.width)
		x2 = op->dst.width;

	if (y2 > op->dst.height)
		y2 = op->dst.height;

	DBG(("%s: box=(%d, %d), (%d, %d)\n", __FUNCTION__, x1, y1, x2, y2));

	if (x2 <= x1 || y2 <= y1)
		return;

	sna_blt_copy_one(sna, &op->u.blt,
			 x1 + src_x, y1 + src_y,
			 x2 - x1, y2 - y1,
			 x1, y1);
}

fastcall static void blt_composite_copy_box(struct sna *sna,
					    const struct sna_composite_op *op,
					    const BoxRec *box)
{
	DBG(("%s: box (%d, %d), (%d, %d)\n",
	     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));
	sna_blt_copy_one(sna, &op->u.blt,
			 box->x1 + op->u.blt.sx,
			 box->y1 + op->u.blt.sy,
			 box->x2 - box->x1,
			 box->y2 - box->y1,
			 box->x1 + op->dst.x,
			 box->y1 + op->dst.y);
}

static void blt_composite_copy_boxes(struct sna *sna,
				     const struct sna_composite_op *op,
				     const BoxRec *box, int nbox)
{
	DBG(("%s: nbox=%d\n", __FUNCTION__, nbox));
	do {
		DBG(("%s: box (%d, %d), (%d, %d)\n",
		     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));
		sna_blt_copy_one(sna, &op->u.blt,
				 box->x1 + op->u.blt.sx, box->y1 + op->u.blt.sy,
				 box->x2 - box->x1, box->y2 - box->y1,
				 box->x1 + op->dst.x, box->y1 + op->dst.y);
		box++;
	} while(--nbox);
}

static inline uint32_t add2(uint32_t v, int16_t x, int16_t y)
{
	x += v & 0xffff;
	y += v >> 16;
	return (uint16_t)y << 16 | x;
}

static void blt_composite_copy_boxes__thread(struct sna *sna,
					     const struct sna_composite_op *op,
					     const BoxRec *box, int nbox)
{
	struct kgem *kgem = &sna->kgem;
	int dst_dx = op->dst.x;
	int dst_dy = op->dst.y;
	int src_dx = op->src.offset[0];
	int src_dy = op->src.offset[1];
	uint32_t cmd = op->u.blt.cmd;
	uint32_t br13 = op->u.blt.br13;
	struct kgem_bo *src_bo = op->u.blt.bo[0];
	struct kgem_bo *dst_bo = op->u.blt.bo[1];
	int src_pitch = op->u.blt.pitch[0];

	DBG(("%s: nbox=%d\n", __FUNCTION__, nbox));

	sna_vertex_lock(&sna->render);

	if ((dst_dx | dst_dy) == 0) {
		uint64_t hdr = (uint64_t)br13 << 32 | cmd;
		do {
			int nbox_this_time;

			nbox_this_time = nbox;
			if (8*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
				nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
			if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
				nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc)/2;
			assert(nbox_this_time);
			nbox -= nbox_this_time;

			assert(sna->kgem.mode == KGEM_BLT);
			do {
				uint32_t *b = kgem->batch + kgem->nbatch;

				DBG(("  %s: box=(%d, %d)x(%d, %d)\n",
				     __FUNCTION__,
				     box->x1, box->y1,
				     box->x2 - box->x1, box->y2 - box->y1));

				assert(box->x1 + src_dx >= 0);
				assert(box->y1 + src_dy >= 0);
				assert(box->x1 + src_dx <= INT16_MAX);
				assert(box->y1 + src_dy <= INT16_MAX);

				assert(box->x1 >= 0);
				assert(box->y1 >= 0);

				*(uint64_t *)&b[0] = hdr;
				*(uint64_t *)&b[2] = *(const uint64_t *)box;
				b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, dst_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = add2(b[2], src_dx, src_dy);
				b[6] = src_pitch;
				b[7] = kgem_add_reloc(kgem, kgem->nbatch + 7, src_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      KGEM_RELOC_FENCED,
						      0);
				kgem->nbatch += 8;
				assert(kgem->nbatch < kgem->surface);
				box++;
			} while (--nbox_this_time);

			if (!nbox)
				break;

			_kgem_submit(kgem);
			_kgem_set_mode(kgem, KGEM_BLT);
		} while (1);
	} else {
		do {
			int nbox_this_time;

			nbox_this_time = nbox;
			if (8*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
				nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
			if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
				nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc)/2;
			assert(nbox_this_time);
			nbox -= nbox_this_time;

			assert(sna->kgem.mode == KGEM_BLT);
			do {
				uint32_t *b = kgem->batch + kgem->nbatch;

				DBG(("  %s: box=(%d, %d)x(%d, %d)\n",
				     __FUNCTION__,
				     box->x1, box->y1,
				     box->x2 - box->x1, box->y2 - box->y1));

				assert(box->x1 + src_dx >= 0);
				assert(box->y1 + src_dy >= 0);

				assert(box->x1 + dst_dx >= 0);
				assert(box->y1 + dst_dy >= 0);

				b[0] = cmd;
				b[1] = br13;
				b[2] = ((box->y1 + dst_dy) << 16) | (box->x1 + dst_dx);
				b[3] = ((box->y2 + dst_dy) << 16) | (box->x2 + dst_dx);
				b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, dst_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = ((box->y1 + src_dy) << 16) | (box->x1 + src_dx);
				b[6] = src_pitch;
				b[7] = kgem_add_reloc(kgem, kgem->nbatch + 7, src_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      KGEM_RELOC_FENCED,
						      0);
				kgem->nbatch += 8;
				assert(kgem->nbatch < kgem->surface);
				box++;
			} while (--nbox_this_time);

			if (!nbox)
				break;

			_kgem_submit(kgem);
			_kgem_set_mode(kgem, KGEM_BLT);
		} while (1);
	}
	sna_vertex_unlock(&sna->render);
}

static void blt_composite_copy_boxes__thread64(struct sna *sna,
					       const struct sna_composite_op *op,
					       const BoxRec *box, int nbox)
{
	struct kgem *kgem = &sna->kgem;
	int dst_dx = op->dst.x;
	int dst_dy = op->dst.y;
	int src_dx = op->src.offset[0];
	int src_dy = op->src.offset[1];
	uint32_t cmd = op->u.blt.cmd;
	uint32_t br13 = op->u.blt.br13;
	struct kgem_bo *src_bo = op->u.blt.bo[0];
	struct kgem_bo *dst_bo = op->u.blt.bo[1];
	int src_pitch = op->u.blt.pitch[0];

	DBG(("%s: nbox=%d\n", __FUNCTION__, nbox));

	sna_vertex_lock(&sna->render);

	if ((dst_dx | dst_dy) == 0) {
		uint64_t hdr = (uint64_t)br13 << 32 | cmd;
		do {
			int nbox_this_time;

			nbox_this_time = nbox;
			if (10*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
				nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 10;
			if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
				nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc)/2;
			assert(nbox_this_time);
			nbox -= nbox_this_time;

			assert(kgem->mode == KGEM_BLT);
			do {
				uint32_t *b = kgem->batch + kgem->nbatch;

				DBG(("  %s: box=(%d, %d)x(%d, %d)\n",
				     __FUNCTION__,
				     box->x1, box->y1,
				     box->x2 - box->x1, box->y2 - box->y1));

				assert(box->x1 + src_dx >= 0);
				assert(box->y1 + src_dy >= 0);
				assert(box->x1 + src_dx <= INT16_MAX);
				assert(box->y1 + src_dy <= INT16_MAX);

				assert(box->x1 >= 0);
				assert(box->y1 >= 0);

				*(uint64_t *)&b[0] = hdr;
				*(uint64_t *)&b[2] = *(const uint64_t *)box;
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(kgem, kgem->nbatch + 4, dst_bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 I915_GEM_DOMAIN_RENDER |
							 KGEM_RELOC_FENCED,
							 0);
				b[6] = add2(b[2], src_dx, src_dy);
				b[7] = src_pitch;
				*(uint64_t *)(b+8) =
					kgem_add_reloc64(kgem, kgem->nbatch + 8, src_bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 KGEM_RELOC_FENCED,
							 0);
				kgem->nbatch += 10;
				assert(kgem->nbatch < kgem->surface);
				box++;
			} while (--nbox_this_time);

			if (!nbox)
				break;

			_kgem_submit(kgem);
			_kgem_set_mode(kgem, KGEM_BLT);
		} while (1);
	} else {
		do {
			int nbox_this_time;

			nbox_this_time = nbox;
			if (10*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
				nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 10;
			if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
				nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc)/2;
			assert(nbox_this_time);
			nbox -= nbox_this_time;

			assert(kgem->mode == KGEM_BLT);
			do {
				uint32_t *b = kgem->batch + kgem->nbatch;

				DBG(("  %s: box=(%d, %d)x(%d, %d)\n",
				     __FUNCTION__,
				     box->x1, box->y1,
				     box->x2 - box->x1, box->y2 - box->y1));

				assert(box->x1 + src_dx >= 0);
				assert(box->y1 + src_dy >= 0);

				assert(box->x1 + dst_dx >= 0);
				assert(box->y1 + dst_dy >= 0);

				b[0] = cmd;
				b[1] = br13;
				b[2] = ((box->y1 + dst_dy) << 16) | (box->x1 + dst_dx);
				b[3] = ((box->y2 + dst_dy) << 16) | (box->x2 + dst_dx);
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(kgem, kgem->nbatch + 4, dst_bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 I915_GEM_DOMAIN_RENDER |
							 KGEM_RELOC_FENCED,
							 0);
				b[6] = ((box->y1 + src_dy) << 16) | (box->x1 + src_dx);
				b[7] = src_pitch;
				*(uint64_t *)(b+8) =
					kgem_add_reloc64(kgem, kgem->nbatch + 8, src_bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 KGEM_RELOC_FENCED,
							 0);
				kgem->nbatch += 10;
				assert(kgem->nbatch < kgem->surface);
				box++;
			} while (--nbox_this_time);

			if (!nbox)
				break;

			_kgem_submit(kgem);
			_kgem_set_mode(kgem, KGEM_BLT);
		} while (1);
	}
	sna_vertex_unlock(&sna->render);
}

fastcall static void
blt_composite_copy_with_alpha(struct sna *sna,
			      const struct sna_composite_op *op,
			      const struct sna_composite_rectangles *r)
{
	int x1, x2, y1, y2;
	int src_x, src_y;

	DBG(("%s: src=(%d, %d), dst=(%d, %d), size=(%d, %d)\n",
	     __FUNCTION__,
	     r->src.x, r->src.y,
	     r->dst.x, r->dst.y,
	     r->width, r->height));

	/* XXX higher layer should have clipped? */

	x1 = r->dst.x + op->dst.x;
	y1 = r->dst.y + op->dst.y;
	x2 = x1 + r->width;
	y2 = y1 + r->height;

	src_x = r->src.x - x1 + op->u.blt.sx;
	src_y = r->src.y - y1 + op->u.blt.sy;

	/* clip against dst */
	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;

	if (x2 > op->dst.width)
		x2 = op->dst.width;

	if (y2 > op->dst.height)
		y2 = op->dst.height;

	DBG(("%s: box=(%d, %d), (%d, %d)\n", __FUNCTION__, x1, y1, x2, y2));

	if (x2 <= x1 || y2 <= y1)
		return;

	sna_blt_alpha_fixup_one(sna, &op->u.blt,
				x1 + src_x, y1 + src_y,
				x2 - x1, y2 - y1,
				x1, y1);
}

fastcall static void
blt_composite_copy_box_with_alpha(struct sna *sna,
				  const struct sna_composite_op *op,
				  const BoxRec *box)
{
	DBG(("%s: box (%d, %d), (%d, %d)\n",
	     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));
	sna_blt_alpha_fixup_one(sna, &op->u.blt,
				box->x1 + op->u.blt.sx,
				box->y1 + op->u.blt.sy,
				box->x2 - box->x1,
				box->y2 - box->y1,
				box->x1 + op->dst.x,
				box->y1 + op->dst.y);
}

static void
blt_composite_copy_boxes_with_alpha(struct sna *sna,
				    const struct sna_composite_op *op,
				    const BoxRec *box, int nbox)
{
	DBG(("%s: nbox=%d\n", __FUNCTION__, nbox));
	do {
		DBG(("%s: box (%d, %d), (%d, %d)\n",
		     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));
		sna_blt_alpha_fixup_one(sna, &op->u.blt,
					box->x1 + op->u.blt.sx, box->y1 + op->u.blt.sy,
					box->x2 - box->x1, box->y2 - box->y1,
					box->x1 + op->dst.x, box->y1 + op->dst.y);
		box++;
	} while(--nbox);
}

static bool
prepare_blt_copy(struct sna *sna,
		 struct sna_composite_op *op,
		 struct kgem_bo *bo,
		 uint32_t alpha_fixup)
{
	PixmapPtr src = op->u.blt.src_pixmap;

	assert(op->dst.bo);
	assert(kgem_bo_can_blt(&sna->kgem, op->dst.bo));
	assert(kgem_bo_can_blt(&sna->kgem, bo));

	kgem_set_mode(&sna->kgem, KGEM_BLT, op->dst.bo);
	if (!kgem_check_many_bo_fenced(&sna->kgem, op->dst.bo, bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_many_bo_fenced(&sna->kgem,
					       op->dst.bo, bo, NULL)) {
			DBG(("%s: fallback -- no room in aperture\n", __FUNCTION__));
			return sna_tiling_blt_composite(sna, op, bo,
							src->drawable.bitsPerPixel,
							alpha_fixup);
		}
		_kgem_set_mode(&sna->kgem, KGEM_BLT);
	}

	DBG(("%s\n", __FUNCTION__));

	if (sna->kgem.gen >= 060 && op->dst.bo == bo)
		op->done = gen6_blt_copy_done;
	else
		op->done = nop_done;

	if (alpha_fixup) {
		op->blt   = blt_composite_copy_with_alpha;
		op->box   = blt_composite_copy_box_with_alpha;
		op->boxes = blt_composite_copy_boxes_with_alpha;

		if (!sna_blt_alpha_fixup_init(sna, &op->u.blt, bo, op->dst.bo,
					      src->drawable.bitsPerPixel,
					      alpha_fixup))
			return false;
	} else {
		op->blt   = blt_composite_copy;
		op->box   = blt_composite_copy_box;
		op->boxes = blt_composite_copy_boxes;
		if (sna->kgem.gen >= 0100)
			op->thread_boxes = blt_composite_copy_boxes__thread64;
		else
			op->thread_boxes = blt_composite_copy_boxes__thread;

		if (!sna_blt_copy_init(sna, &op->u.blt, bo, op->dst.bo,
				       src->drawable.bitsPerPixel,
				       GXcopy))
			return false;
	}

	return true;
}

fastcall static void
blt_put_composite__cpu(struct sna *sna,
		       const struct sna_composite_op *op,
		       const struct sna_composite_rectangles *r)
{
	PixmapPtr dst = op->dst.pixmap;
	PixmapPtr src = op->u.blt.src_pixmap;
	assert(src->devPrivate.ptr);
	assert(src->devKind);
	assert(dst->devPrivate.ptr);
	assert(dst->devKind);
	memcpy_blt(src->devPrivate.ptr, dst->devPrivate.ptr,
		   src->drawable.bitsPerPixel, src->devKind, dst->devKind,
		   r->src.x + op->u.blt.sx, r->src.y + op->u.blt.sy,
		   r->dst.x + op->dst.x, r->dst.y + op->dst.y,
		   r->width, r->height);
}

fastcall static void
blt_put_composite_box__cpu(struct sna *sna,
			   const struct sna_composite_op *op,
			   const BoxRec *box)
{
	PixmapPtr dst = op->dst.pixmap;
	PixmapPtr src = op->u.blt.src_pixmap;
	assert(src->devPrivate.ptr);
	assert(src->devKind);
	assert(dst->devPrivate.ptr);
	assert(dst->devKind);
	memcpy_blt(src->devPrivate.ptr, dst->devPrivate.ptr,
		   src->drawable.bitsPerPixel, src->devKind, dst->devKind,
		   box->x1 + op->u.blt.sx, box->y1 + op->u.blt.sy,
		   box->x1 + op->dst.x, box->y1 + op->dst.y,
		   box->x2-box->x1, box->y2-box->y1);
}

static void
blt_put_composite_boxes__cpu(struct sna *sna,
			     const struct sna_composite_op *op,
			     const BoxRec *box, int n)
{
	PixmapPtr dst = op->dst.pixmap;
	PixmapPtr src = op->u.blt.src_pixmap;
	assert(src->devPrivate.ptr);
	assert(src->devKind);
	assert(dst->devPrivate.ptr);
	assert(dst->devKind);
	do {
		memcpy_blt(src->devPrivate.ptr, dst->devPrivate.ptr,
			   src->drawable.bitsPerPixel, src->devKind, dst->devKind,
			   box->x1 + op->u.blt.sx, box->y1 + op->u.blt.sy,
			   box->x1 + op->dst.x, box->y1 + op->dst.y,
			   box->x2-box->x1, box->y2-box->y1);
		box++;
	} while (--n);
}

fastcall static void
blt_put_composite_with_alpha__cpu(struct sna *sna,
				  const struct sna_composite_op *op,
				  const struct sna_composite_rectangles *r)
{
	PixmapPtr dst = op->dst.pixmap;
	PixmapPtr src = op->u.blt.src_pixmap;
	assert(src->devPrivate.ptr);
	assert(src->devKind);
	assert(dst->devPrivate.ptr);
	assert(dst->devKind);
	memcpy_xor(src->devPrivate.ptr, dst->devPrivate.ptr,
		   src->drawable.bitsPerPixel, src->devKind, dst->devKind,
		   r->src.x + op->u.blt.sx, r->src.y + op->u.blt.sy,
		   r->dst.x + op->dst.x, r->dst.y + op->dst.y,
		   r->width, r->height,
		   0xffffffff, op->u.blt.pixel);

}

fastcall static void
blt_put_composite_box_with_alpha__cpu(struct sna *sna,
				      const struct sna_composite_op *op,
				      const BoxRec *box)
{
	PixmapPtr dst = op->dst.pixmap;
	PixmapPtr src = op->u.blt.src_pixmap;
	assert(src->devPrivate.ptr);
	assert(src->devKind);
	assert(dst->devPrivate.ptr);
	assert(dst->devKind);
	memcpy_xor(src->devPrivate.ptr, dst->devPrivate.ptr,
		   src->drawable.bitsPerPixel, src->devKind, dst->devKind,
		   box->x1 + op->u.blt.sx, box->y1 + op->u.blt.sy,
		   box->x1 + op->dst.x, box->y1 + op->dst.y,
		   box->x2-box->x1, box->y2-box->y1,
		   0xffffffff, op->u.blt.pixel);
}

static void
blt_put_composite_boxes_with_alpha__cpu(struct sna *sna,
					const struct sna_composite_op *op,
					const BoxRec *box, int n)
{
	PixmapPtr dst = op->dst.pixmap;
	PixmapPtr src = op->u.blt.src_pixmap;
	assert(src->devPrivate.ptr);
	assert(src->devKind);
	assert(dst->devPrivate.ptr);
	assert(dst->devKind);
	do {
		memcpy_xor(src->devPrivate.ptr, dst->devPrivate.ptr,
			   src->drawable.bitsPerPixel, src->devKind, dst->devKind,
			   box->x1 + op->u.blt.sx, box->y1 + op->u.blt.sy,
			   box->x1 + op->dst.x, box->y1 + op->dst.y,
			   box->x2-box->x1, box->y2-box->y1,
			   0xffffffff, op->u.blt.pixel);
		box++;
	} while (--n);
}

fastcall static void
blt_put_composite(struct sna *sna,
		  const struct sna_composite_op *op,
		  const struct sna_composite_rectangles *r)
{
	PixmapPtr dst = op->dst.pixmap;
	PixmapPtr src = op->u.blt.src_pixmap;
	struct sna_pixmap *dst_priv = sna_pixmap(dst);
	int pitch = src->devKind;
	char *data = src->devPrivate.ptr;
	int bpp = src->drawable.bitsPerPixel;

	int16_t dst_x = r->dst.x + op->dst.x;
	int16_t dst_y = r->dst.y + op->dst.y;
	int16_t src_x = r->src.x + op->u.blt.sx;
	int16_t src_y = r->src.y + op->u.blt.sy;

	if (!dst_priv->pinned &&
	    dst_x <= 0 && dst_y <= 0 &&
	    dst_x + r->width >= op->dst.width &&
	    dst_y + r->height >= op->dst.height) {
		data += (src_x - dst_x) * bpp / 8;
		data += (src_y - dst_y) * pitch;

		assert(op->dst.bo == dst_priv->gpu_bo);
		sna_replace(sna, op->dst.pixmap, data, pitch);
	} else {
		BoxRec box;
		bool ok;

		box.x1 = dst_x;
		box.y1 = dst_y;
		box.x2 = dst_x + r->width;
		box.y2 = dst_y + r->height;

		ok = sna_write_boxes(sna, dst,
				     dst_priv->gpu_bo, 0, 0,
				     data, pitch, src_x, src_y,
				     &box, 1);
		assert(ok);
		(void)ok;
	}
}

fastcall static void blt_put_composite_box(struct sna *sna,
					   const struct sna_composite_op *op,
					   const BoxRec *box)
{
	PixmapPtr src = op->u.blt.src_pixmap;
	struct sna_pixmap *dst_priv = sna_pixmap(op->dst.pixmap);

	DBG(("%s: src=(%d, %d), dst=(%d, %d)\n", __FUNCTION__,
	     op->u.blt.sx, op->u.blt.sy,
	     op->dst.x, op->dst.y));

	assert(src->devPrivate.ptr);
	assert(src->devKind);
	if (!dst_priv->pinned &&
	    box->x2 - box->x1 == op->dst.width &&
	    box->y2 - box->y1 == op->dst.height) {
		int pitch = src->devKind;
		int bpp = src->drawable.bitsPerPixel / 8;
		char *data = src->devPrivate.ptr;

		data += (box->y1 + op->u.blt.sy) * pitch;
		data += (box->x1 + op->u.blt.sx) * bpp;

		assert(op->dst.bo == dst_priv->gpu_bo);
		sna_replace(sna, op->dst.pixmap, data, pitch);
	} else {
		bool ok;

		ok = sna_write_boxes(sna, op->dst.pixmap,
				     op->dst.bo, op->dst.x, op->dst.y,
				     src->devPrivate.ptr,
				     src->devKind,
				     op->u.blt.sx, op->u.blt.sy,
				     box, 1);
		assert(ok);
		(void)ok;
	}
}

static void blt_put_composite_boxes(struct sna *sna,
				    const struct sna_composite_op *op,
				    const BoxRec *box, int n)
{
	PixmapPtr src = op->u.blt.src_pixmap;
	struct sna_pixmap *dst_priv = sna_pixmap(op->dst.pixmap);

	DBG(("%s: src=(%d, %d), dst=(%d, %d), [(%d, %d), (%d, %d) x %d]\n", __FUNCTION__,
	     op->u.blt.sx, op->u.blt.sy,
	     op->dst.x, op->dst.y,
	     box->x1, box->y1, box->x2, box->y2, n));

	assert(src->devPrivate.ptr);
	assert(src->devKind);
	if (n == 1 && !dst_priv->pinned &&
	    box->x2 - box->x1 == op->dst.width &&
	    box->y2 - box->y1 == op->dst.height) {
		int pitch = src->devKind;
		int bpp = src->drawable.bitsPerPixel / 8;
		char *data = src->devPrivate.ptr;

		data += (box->y1 + op->u.blt.sy) * pitch;
		data += (box->x1 + op->u.blt.sx) * bpp;

		assert(op->dst.bo == dst_priv->gpu_bo);
		sna_replace(sna, op->dst.pixmap, data, pitch);
	} else {
		bool ok;

		ok = sna_write_boxes(sna, op->dst.pixmap,
				     op->dst.bo, op->dst.x, op->dst.y,
				     src->devPrivate.ptr,
				     src->devKind,
				     op->u.blt.sx, op->u.blt.sy,
				     box, n);
		assert(ok);
		(void)ok;
	}
}

fastcall static void
blt_put_composite_with_alpha(struct sna *sna,
			     const struct sna_composite_op *op,
			     const struct sna_composite_rectangles *r)
{
	PixmapPtr dst = op->dst.pixmap;
	PixmapPtr src = op->u.blt.src_pixmap;
	struct sna_pixmap *dst_priv = sna_pixmap(dst);
	int pitch = src->devKind;
	char *data = src->devPrivate.ptr;

	int16_t dst_x = r->dst.x + op->dst.x;
	int16_t dst_y = r->dst.y + op->dst.y;
	int16_t src_x = r->src.x + op->u.blt.sx;
	int16_t src_y = r->src.y + op->u.blt.sy;

	assert(src->devPrivate.ptr);
	assert(src->devKind);

	if (!dst_priv->pinned &&
	    dst_x <= 0 && dst_y <= 0 &&
	    dst_x + r->width >= op->dst.width &&
	    dst_y + r->height >= op->dst.height) {
		int bpp = dst->drawable.bitsPerPixel / 8;

		data += (src_x - dst_x) * bpp;
		data += (src_y - dst_y) * pitch;

		assert(op->dst.bo == dst_priv->gpu_bo);
		sna_replace__xor(sna, op->dst.pixmap, data, pitch,
				 0xffffffff, op->u.blt.pixel);
	} else {
		BoxRec box;

		box.x1 = dst_x;
		box.y1 = dst_y;
		box.x2 = dst_x + r->width;
		box.y2 = dst_y + r->height;

		sna_write_boxes__xor(sna, dst,
				     dst_priv->gpu_bo, 0, 0,
				     data, pitch, src_x, src_y,
				     &box, 1,
				     0xffffffff, op->u.blt.pixel);
	}
}

fastcall static void
blt_put_composite_box_with_alpha(struct sna *sna,
				 const struct sna_composite_op *op,
				 const BoxRec *box)
{
	PixmapPtr src = op->u.blt.src_pixmap;
	struct sna_pixmap *dst_priv = sna_pixmap(op->dst.pixmap);

	DBG(("%s: src=(%d, %d), dst=(%d, %d)\n", __FUNCTION__,
	     op->u.blt.sx, op->u.blt.sy,
	     op->dst.x, op->dst.y));

	assert(src->devPrivate.ptr);
	assert(src->devKind);

	if (!dst_priv->pinned &&
	    box->x2 - box->x1 == op->dst.width &&
	    box->y2 - box->y1 == op->dst.height) {
		int pitch = src->devKind;
		int bpp = src->drawable.bitsPerPixel / 8;
		char *data = src->devPrivate.ptr;

		data += (box->y1 + op->u.blt.sy) * pitch;
		data += (box->x1 + op->u.blt.sx) * bpp;

		assert(op->dst.bo == dst_priv->gpu_bo);
		sna_replace__xor(sna, op->dst.pixmap, data, pitch,
				 0xffffffff, op->u.blt.pixel);
	} else {
		sna_write_boxes__xor(sna, op->dst.pixmap,
				     op->dst.bo, op->dst.x, op->dst.y,
				     src->devPrivate.ptr,
				     src->devKind,
				     op->u.blt.sx, op->u.blt.sy,
				     box, 1,
				     0xffffffff, op->u.blt.pixel);
	}
}

static void
blt_put_composite_boxes_with_alpha(struct sna *sna,
				   const struct sna_composite_op *op,
				   const BoxRec *box, int n)
{
	PixmapPtr src = op->u.blt.src_pixmap;
	struct sna_pixmap *dst_priv = sna_pixmap(op->dst.pixmap);

	DBG(("%s: src=(%d, %d), dst=(%d, %d), [(%d, %d), (%d, %d) x %d]\n", __FUNCTION__,
	     op->u.blt.sx, op->u.blt.sy,
	     op->dst.x, op->dst.y,
	     box->x1, box->y1, box->x2, box->y2, n));

	assert(src->devPrivate.ptr);
	assert(src->devKind);

	if (n == 1 && !dst_priv->pinned &&
	    box->x2 - box->x1 == op->dst.width &&
	    box->y2 - box->y1 == op->dst.height) {
		int pitch = src->devKind;
		int bpp = src->drawable.bitsPerPixel / 8;
		char *data = src->devPrivate.ptr;

		data += (box->y1 + op->u.blt.sy) * pitch;
		data += (box->x1 + op->u.blt.sx) * bpp;

		assert(dst_priv->gpu_bo == op->dst.bo);
		sna_replace__xor(sna, op->dst.pixmap, data, pitch,
				 0xffffffff, op->u.blt.pixel);
	} else {
		sna_write_boxes__xor(sna, op->dst.pixmap,
				     op->dst.bo, op->dst.x, op->dst.y,
				     src->devPrivate.ptr,
				     src->devKind,
				     op->u.blt.sx, op->u.blt.sy,
				     box, n,
				     0xffffffff, op->u.blt.pixel);
	}
}

static bool
prepare_blt_put(struct sna *sna,
		struct sna_composite_op *op,
		uint32_t alpha_fixup)
{
	DBG(("%s\n", __FUNCTION__));

	assert(!sna_pixmap(op->dst.pixmap)->clear);

	if (op->dst.bo) {
		assert(op->dst.bo == sna_pixmap(op->dst.pixmap)->gpu_bo);
		if (alpha_fixup) {
			op->u.blt.pixel = alpha_fixup;
			op->blt   = blt_put_composite_with_alpha;
			op->box   = blt_put_composite_box_with_alpha;
			op->boxes = blt_put_composite_boxes_with_alpha;
		} else {
			op->blt   = blt_put_composite;
			op->box   = blt_put_composite_box;
			op->boxes = blt_put_composite_boxes;
		}
	} else {
		if (alpha_fixup) {
			op->u.blt.pixel = alpha_fixup;
			op->blt   = blt_put_composite_with_alpha__cpu;
			op->box   = blt_put_composite_box_with_alpha__cpu;
			op->boxes = blt_put_composite_boxes_with_alpha__cpu;
		} else {
			op->blt   = blt_put_composite__cpu;
			op->box   = blt_put_composite_box__cpu;
			op->boxes = blt_put_composite_boxes__cpu;
		}
	}
	op->done = nop_done;

	return true;
}

static bool
is_clear(PixmapPtr pixmap)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	return priv && priv->clear;
}

static inline uint32_t
over(uint32_t src, uint32_t dst)
{
	uint32_t a = ~src >> 24;

#define G_SHIFT 8
#define RB_MASK 0xff00ff
#define RB_ONE_HALF 0x800080
#define RB_MASK_PLUS_ONE 0x10000100

#define UN8_rb_MUL_UN8(x, a, t) do {				\
	t  = ((x) & RB_MASK) * (a);				\
	t += RB_ONE_HALF;					\
	x = (t + ((t >> G_SHIFT) & RB_MASK)) >> G_SHIFT;	\
	x &= RB_MASK;						\
} while (0)

#define UN8_rb_ADD_UN8_rb(x, y, t) do {				\
	t = ((x) + (y));					\
	t |= RB_MASK_PLUS_ONE - ((t >> G_SHIFT) & RB_MASK);	\
	x = (t & RB_MASK);					\
} while (0)

#define UN8x4_MUL_UN8_ADD_UN8x4(x, a, y) do {			\
	uint32_t r1__, r2__, r3__, t__;				\
	\
	r1__ = (x);						\
	r2__ = (y) & RB_MASK;					\
	UN8_rb_MUL_UN8(r1__, (a), t__);				\
	UN8_rb_ADD_UN8_rb(r1__, r2__, t__);			\
	\
	r2__ = (x) >> G_SHIFT;					\
	r3__ = ((y) >> G_SHIFT) & RB_MASK;			\
	UN8_rb_MUL_UN8(r2__, (a), t__);				\
	UN8_rb_ADD_UN8_rb(r2__, r3__, t__);			\
	\
	(x) = r1__ | (r2__ << G_SHIFT);				\
} while (0)

	UN8x4_MUL_UN8_ADD_UN8x4(dst, a, src);

	return dst;
}

static inline uint32_t
add(uint32_t src, uint32_t dst)
{
#define UN8x4_ADD_UN8x4(x, y) do {				\
	uint32_t r1__, r2__, r3__, t__;				\
	\
	r1__ = (x) & RB_MASK;					\
	r2__ = (y) & RB_MASK;					\
	UN8_rb_ADD_UN8_rb(r1__, r2__, t__);			\
	\
	r2__ = ((x) >> G_SHIFT) & RB_MASK;			\
	r3__ = ((y) >> G_SHIFT) & RB_MASK;			\
	UN8_rb_ADD_UN8_rb(r2__, r3__, t__);			\
	\
	x = r1__ | (r2__ << G_SHIFT);				\
} while (0)

	UN8x4_ADD_UN8x4(src, dst);
	return src;
}

bool
sna_blt_composite(struct sna *sna,
		  uint32_t op,
		  PicturePtr src,
		  PicturePtr dst,
		  int16_t x, int16_t y,
		  int16_t dst_x, int16_t dst_y,
		  int16_t width, int16_t height,
		  unsigned flags,
		  struct sna_composite_op *tmp)
{
	PictFormat src_format = src->format;
	PixmapPtr src_pixmap;
	struct kgem_bo *bo;
	int16_t tx, ty;
	BoxRec dst_box, src_box;
	uint32_t alpha_fixup;
	uint32_t color, hint;
	bool was_clear;
	bool ret;

#if DEBUG_NO_BLT || NO_BLT_COMPOSITE
	return false;
#endif
	DBG(("%s (%d, %d), (%d, %d), %dx%d\n",
	     __FUNCTION__, x, y, dst_x, dst_y, width, height));

	switch (dst->pDrawable->bitsPerPixel) {
	case 8:
	case 16:
	case 32:
		break;
	default:
		DBG(("%s: unhandled bpp: %d\n", __FUNCTION__,
		     dst->pDrawable->bitsPerPixel));
		return false;
	}

	tmp->dst.pixmap = get_drawable_pixmap(dst->pDrawable);
	was_clear = is_clear(tmp->dst.pixmap);

	if (width | height) {
		dst_box.x1 = dst_x;
		dst_box.x2 = bound(dst_x, width);
		dst_box.y1 = dst_y;
		dst_box.y2 = bound(dst_y, height);
	} else
		sna_render_picture_extents(dst, &dst_box);

	tmp->dst.format = dst->format;
	tmp->dst.width = tmp->dst.pixmap->drawable.width;
	tmp->dst.height = tmp->dst.pixmap->drawable.height;
	get_drawable_deltas(dst->pDrawable, tmp->dst.pixmap,
			    &tmp->dst.x, &tmp->dst.y);

	if (op == PictOpClear) {
clear:
		if (was_clear && sna_pixmap(tmp->dst.pixmap)->clear_color == 0) {
			sna_pixmap(tmp->dst.pixmap)->clear = true;
			return prepare_blt_nop(sna, tmp);
		}

		hint = 0;
		if (can_render(sna)) {
			hint |= PREFER_GPU;
			if ((flags & COMPOSITE_PARTIAL) == 0) {
				hint |= IGNORE_DAMAGE;
				if (width  == tmp->dst.pixmap->drawable.width &&
				    height == tmp->dst.pixmap->drawable.height)
					hint |= REPLACES;
			}
		}
		tmp->dst.bo = sna_drawable_use_bo(dst->pDrawable, hint,
						  &dst_box, &tmp->damage);
		if (tmp->dst.bo) {
			if (!kgem_bo_can_blt(&sna->kgem, tmp->dst.bo)) {
				DBG(("%s: can not blit to dst, tiling? %d, pitch? %d\n",
				     __FUNCTION__, tmp->dst.bo->tiling, tmp->dst.bo->pitch));
				return false;
			}
			if (hint & REPLACES)
				kgem_bo_undo(&sna->kgem, tmp->dst.bo);
		} else {
			RegionRec region;

			region.extents = dst_box;
			region.data = NULL;

			hint = MOVE_WRITE | MOVE_INPLACE_HINT;
			if (flags & COMPOSITE_PARTIAL)
				hint |= MOVE_READ;
			if (!sna_drawable_move_region_to_cpu(dst->pDrawable, &region, hint))
				return false;
		}

		return prepare_blt_clear(sna, tmp);
	}

	if (is_solid(src)) {
		if ((op == PictOpOver || op == PictOpAdd) && is_transparent(src)) {
			sna_pixmap(tmp->dst.pixmap)->clear = was_clear;
			return prepare_blt_nop(sna, tmp);
		}
		if (op == PictOpOver && is_opaque_solid(src))
			op = PictOpSrc;
		if (op == PictOpAdd && is_white(src))
			op = PictOpSrc;
		if (was_clear && (op == PictOpAdd || op == PictOpOver)) {
			if (sna_pixmap(tmp->dst.pixmap)->clear_color == 0)
				op = PictOpSrc;
			if (op == PictOpOver) {
				color = over(get_solid_color(src, PICT_a8r8g8b8),
					     color_convert(sna_pixmap(tmp->dst.pixmap)->clear_color,
							   dst->format, PICT_a8r8g8b8));
				op = PictOpSrc;
				DBG(("%s: precomputing solid OVER (%08x, %08x) -> %08x\n",
				     __FUNCTION__, get_solid_color(src, PICT_a8r8g8b8),
				     color_convert(sna_pixmap(tmp->dst.pixmap)->clear_color,
						   dst->format, PICT_a8r8g8b8),
				     color));
			}
			if (op == PictOpAdd) {
				color = add(get_solid_color(src, PICT_a8r8g8b8),
					    color_convert(sna_pixmap(tmp->dst.pixmap)->clear_color,
							  dst->format, PICT_a8r8g8b8));
				op = PictOpSrc;
				DBG(("%s: precomputing solid ADD (%08x, %08x) -> %08x\n",
				     __FUNCTION__, get_solid_color(src, PICT_a8r8g8b8),
				     color_convert(sna_pixmap(tmp->dst.pixmap)->clear_color,
						   dst->format, PICT_a8r8g8b8),
				     color));
			}
		}
		if (op == PictOpOutReverse && is_opaque_solid(src))
			goto clear;

		if (op != PictOpSrc) {
			DBG(("%s: unsupported op [%d] for blitting\n",
			     __FUNCTION__, op));
			return false;
		}

		color = get_solid_color(src, tmp->dst.format);
fill:
		if (color == 0)
			goto clear;

		if (was_clear && sna_pixmap(tmp->dst.pixmap)->clear_color == color) {
			sna_pixmap(tmp->dst.pixmap)->clear = true;
			return prepare_blt_nop(sna, tmp);
		}

		hint = 0;
		if (can_render(sna)) {
			hint |= PREFER_GPU;
			if ((flags & COMPOSITE_PARTIAL) == 0) {
				hint |= IGNORE_DAMAGE;
				if (width  == tmp->dst.pixmap->drawable.width &&
				    height == tmp->dst.pixmap->drawable.height)
					hint |= REPLACES;
			}
		}
		tmp->dst.bo = sna_drawable_use_bo(dst->pDrawable, hint,
						  &dst_box, &tmp->damage);
		if (tmp->dst.bo) {
			if (!kgem_bo_can_blt(&sna->kgem, tmp->dst.bo)) {
				DBG(("%s: can not blit to dst, tiling? %d, pitch? %d\n",
				     __FUNCTION__, tmp->dst.bo->tiling, tmp->dst.bo->pitch));
				return false;
			}
			if (hint & REPLACES)
				kgem_bo_undo(&sna->kgem, tmp->dst.bo);
		} else {
			RegionRec region;

			region.extents = dst_box;
			region.data = NULL;

			hint = MOVE_WRITE | MOVE_INPLACE_HINT;
			if (flags & COMPOSITE_PARTIAL)
				hint |= MOVE_READ;
			if (!sna_drawable_move_region_to_cpu(dst->pDrawable, &region, hint))
				return false;
		}

		return prepare_blt_fill(sna, tmp, color);
	}

	if (!src->pDrawable) {
		DBG(("%s: unsupported procedural source\n",
		     __FUNCTION__));
		return false;
	}

	if (src->filter == PictFilterConvolution) {
		DBG(("%s: convolutions filters not handled\n",
		     __FUNCTION__));
		return false;
	}

	if (op == PictOpOver && PICT_FORMAT_A(src_format) == 0)
		op = PictOpSrc;

	if (op != PictOpSrc) {
		DBG(("%s: unsupported op [%d] for blitting\n",
		     __FUNCTION__, op));
		return false;
	}

	if (!sna_transform_is_imprecise_integer_translation(src->transform, src->filter,
							    dst->polyMode == PolyModePrecise,
							    &tx, &ty)) {
		DBG(("%s: source transform is not an integer translation\n",
		     __FUNCTION__));
		return false;
	}
	DBG(("%s: converting transform to integer translation? (%d, %d)\n",
	     __FUNCTION__, src->transform != NULL, tx, ty));
	x += tx;
	y += ty;

	if ((x >= src->pDrawable->width ||
	     y >= src->pDrawable->height ||
	     x + width  <= 0 ||
	     y + height <= 0) &&
	    (!src->repeat || src->repeatType == RepeatNone)) {
		DBG(("%s: source is outside of valid area, converting to clear\n",
		     __FUNCTION__));
		goto clear;
	}

	src_pixmap = get_drawable_pixmap(src->pDrawable);
	if (is_clear(src_pixmap)) {
		if (src->repeat ||
		    (x >= 0 && y >= 0 &&
		     x + width  < src_pixmap->drawable.width &&
		     y + height < src_pixmap->drawable.height)) {
			color = color_convert(sna_pixmap(src_pixmap)->clear_color,
					      src->format, tmp->dst.format);
			goto fill;
		}
	}

	alpha_fixup = 0;
	if (!(dst->format == src_format ||
	      dst->format == alphaless(src_format) ||
	      (alphaless(dst->format) == alphaless(src_format) &&
	       sna_get_pixel_from_rgba(&alpha_fixup,
				       0, 0, 0, 0xffff,
				       dst->format)))) {
		DBG(("%s: incompatible src/dst formats src=%08x, dst=%08x\n",
		     __FUNCTION__, (unsigned)src_format, dst->format));
		return false;
	}

	/* XXX tiling? fixup extend none? */
	if (x < 0 || y < 0 ||
	    x + width  > src->pDrawable->width ||
	    y + height > src->pDrawable->height) {
		DBG(("%s: source extends outside (%d, %d), (%d, %d) of valid drawable %dx%d, repeat=%d\n",
		     __FUNCTION__,
		     x, y, x+width, y+width, src->pDrawable->width, src->pDrawable->height, src->repeatType));
		if (src->repeat && src->repeatType == RepeatNormal) {
			x = x % src->pDrawable->width;
			y = y % src->pDrawable->height;
			if (x < 0)
				x += src->pDrawable->width;
			if (y < 0)
				y += src->pDrawable->height;
			if (x + width  > src->pDrawable->width ||
			    y + height > src->pDrawable->height)
				return false;
		} else
			return false;
	}

	get_drawable_deltas(src->pDrawable, src_pixmap, &tx, &ty);
	x += tx + src->pDrawable->x;
	y += ty + src->pDrawable->y;
	if (x < 0 || y < 0 ||
	    x + width  > src_pixmap->drawable.width ||
	    y + height > src_pixmap->drawable.height) {
		DBG(("%s: source extends outside (%d, %d), (%d, %d) of valid pixmap %dx%d\n",
		     __FUNCTION__,
		     x, y, x+width, y+width, src_pixmap->drawable.width, src_pixmap->drawable.height));
		return false;
	}

	tmp->u.blt.src_pixmap = src_pixmap;
	tmp->u.blt.sx = x - dst_x;
	tmp->u.blt.sy = y - dst_y;
	DBG(("%s: blt dst offset (%d, %d), source offset (%d, %d), with alpha fixup? %x\n",
	     __FUNCTION__,
	     tmp->dst.x, tmp->dst.y, tmp->u.blt.sx, tmp->u.blt.sy, alpha_fixup));

	src_box.x1 = x;
	src_box.y1 = y;
	src_box.x2 = x + width;
	src_box.y2 = y + height;
	bo = __sna_render_pixmap_bo(sna, src_pixmap, &src_box, true);
	if (bo && !kgem_bo_can_blt(&sna->kgem, bo)) {
		DBG(("%s: can not blit from src size=%dx%d, tiling? %d, pitch? %d\n",
		     __FUNCTION__,
		     src_pixmap->drawable.width  < sna->render.max_3d_size,
		     src_pixmap->drawable.height < sna->render.max_3d_size,
		     bo->tiling, bo->pitch));

		if (src_pixmap->drawable.width  <= sna->render.max_3d_size &&
		    src_pixmap->drawable.height <= sna->render.max_3d_size &&
		    bo->pitch <= sna->render.max_3d_pitch &&
		    (flags & COMPOSITE_FALLBACK) == 0)
		{
			return false;
		}

		bo = NULL;
	}

	hint = 0;
	if (bo || can_render(sna)) {
		hint |= PREFER_GPU;
		if ((flags & COMPOSITE_PARTIAL) == 0) {
			hint |= IGNORE_DAMAGE;
			if (width  == tmp->dst.pixmap->drawable.width &&
			    height == tmp->dst.pixmap->drawable.height)
				hint |= REPLACES;
		}
		if (bo)
			hint |= FORCE_GPU;
	}
	tmp->dst.bo = sna_drawable_use_bo(dst->pDrawable, hint,
					  &dst_box, &tmp->damage);

	if (tmp->dst.bo && hint & REPLACES) {
		struct sna_pixmap *priv = sna_pixmap(tmp->dst.pixmap);
		kgem_bo_pair_undo(&sna->kgem, priv->gpu_bo, priv->cpu_bo);
	}

	if (tmp->dst.pixmap == src_pixmap)
		bo = __sna_render_pixmap_bo(sna, src_pixmap, &src_box, true);

	ret = false;
	if (bo) {
		if (!tmp->dst.bo) {
			DBG(("%s: fallback -- unaccelerated read back\n",
			     __FUNCTION__));
fallback:
			if (flags & COMPOSITE_FALLBACK || !kgem_bo_is_busy(bo))
				goto put;
		} else if (!kgem_bo_can_blt(&sna->kgem, bo)) {
			DBG(("%s: fallback -- cannot blit from source\n",
			     __FUNCTION__));
			goto fallback;
		} else if (bo->snoop && tmp->dst.bo->snoop) {
			DBG(("%s: fallback -- can not copy between snooped bo\n",
			     __FUNCTION__));
			goto put;
		} else if (!kgem_bo_can_blt(&sna->kgem, tmp->dst.bo)) {
			DBG(("%s: fallback -- unaccelerated upload\n",
			     __FUNCTION__));
			goto fallback;
		} else {
			ret = prepare_blt_copy(sna, tmp, bo, alpha_fixup);
			if (!ret)
				goto fallback;
		}
	} else {
		RegionRec region;

put:
		if (tmp->dst.bo == sna_pixmap(tmp->dst.pixmap)->cpu_bo) {
			DBG(("%s: dropping upload into CPU bo\n", __FUNCTION__));
			tmp->dst.bo = NULL;
			tmp->damage = NULL;
		}

		if (tmp->dst.bo == NULL) {
			hint = MOVE_INPLACE_HINT | MOVE_WRITE;
			if (flags & COMPOSITE_PARTIAL)
				hint |= MOVE_READ;

			region.extents = dst_box;
			region.data = NULL;
			if (!sna_drawable_move_region_to_cpu(dst->pDrawable,
							     &region, hint))
				return false;

			assert(tmp->damage == NULL);
		}

		region.extents = src_box;
		region.data = NULL;
		if (!sna_drawable_move_region_to_cpu(&src_pixmap->drawable,
						     &region, MOVE_READ))
			return false;

		ret = prepare_blt_put(sna, tmp, alpha_fixup);
	}

	return ret;
}

static void convert_done(struct sna *sna, const struct sna_composite_op *op)
{
	struct kgem *kgem = &sna->kgem;

	assert(kgem->nbatch <= KGEM_BATCH_SIZE(kgem));
	if (kgem->nexec > 1 && __kgem_ring_empty(kgem)) {
		DBG(("%s: flushing BLT operation on empty ring\n", __FUNCTION__));
		_kgem_submit(kgem);
	}

	kgem_bo_destroy(kgem, op->src.bo);
	sna_render_composite_redirect_done(sna, op);
}

static void gen6_convert_done(struct sna *sna, const struct sna_composite_op *op)
{
	struct kgem *kgem = &sna->kgem;

	if (kgem_check_batch(kgem, 3)) {
		uint32_t *b = kgem->batch + kgem->nbatch;
		assert(sna->kgem.mode == KGEM_BLT);
		b[0] = XY_SETUP_CLIP;
		b[1] = b[2] = 0;
		kgem->nbatch += 3;
		assert(kgem->nbatch < kgem->surface);
	}

	convert_done(sna, op);
}

bool
sna_blt_composite__convert(struct sna *sna,
			   int x, int y,
			   int width, int height,
			   struct sna_composite_op *tmp)
{
	uint32_t alpha_fixup;
	int sx, sy;
	uint8_t op;

#if DEBUG_NO_BLT || NO_BLT_COMPOSITE
	return false;
#endif

	DBG(("%s src=%d, dst=%d (redirect? %d)\n", __FUNCTION__,
	     tmp->src.bo->handle, tmp->dst.bo->handle,
	     tmp->redirect.real_bo ? tmp->redirect.real_bo->handle : 0));

	if (!kgem_bo_can_blt(&sna->kgem, tmp->dst.bo) ||
	    !kgem_bo_can_blt(&sna->kgem, tmp->src.bo)) {
		DBG(("%s: cannot blt from src or to dst\n", __FUNCTION__));
		return false;
	}

	if (tmp->src.transform) {
		DBG(("%s: transforms not handled by the BLT\n", __FUNCTION__));
		return false;
	}

	if (tmp->src.filter == PictFilterConvolution) {
		DBG(("%s: convolutions filters not handled\n",
		     __FUNCTION__));
		return false;
	}

	op = tmp->op;
	if (op == PictOpOver && PICT_FORMAT_A(tmp->src.pict_format) == 0)
		op = PictOpSrc;
	if (op != PictOpSrc) {
		DBG(("%s: unsupported op [%d] for blitting\n",
		     __FUNCTION__, op));
		return false;
	}

	alpha_fixup = 0;
	if (!(tmp->dst.format == tmp->src.pict_format ||
	      tmp->dst.format == alphaless(tmp->src.pict_format) ||
	      (alphaless(tmp->dst.format) == alphaless(tmp->src.pict_format) &&
	       sna_get_pixel_from_rgba(&alpha_fixup,
				       0, 0, 0, 0xffff,
				       tmp->dst.format)))) {
		DBG(("%s: incompatible src/dst formats src=%08x, dst=%08x\n",
		     __FUNCTION__,
		     (unsigned)tmp->src.pict_format,
		     (unsigned)tmp->dst.format));
		return false;
	}

	sx = tmp->src.offset[0];
	sy = tmp->src.offset[1];

	x += sx;
	y += sy;
	if (x < 0 || y < 0 ||
	    x + width  > tmp->src.width ||
	    y + height > tmp->src.height) {
		DBG(("%s: source extends outside (%d, %d), (%d, %d) of valid drawable %dx%d\n",
		     __FUNCTION__,
		     x, y, x+width, y+width, tmp->src.width, tmp->src.height));
		if (tmp->src.repeat == RepeatNormal) {
			int xx = x % tmp->src.width;
			int yy = y % tmp->src.height;
			if (xx < 0)
				xx += tmp->src.width;
			if (yy < 0)
				yy += tmp->src.height;
			if (xx + width  > tmp->src.width ||
			    yy + height > tmp->src.height)
				return false;

			sx += xx - x;
			sy += yy - y;
		} else
			return false;
	}

	DBG(("%s: blt dst offset (%d, %d), source offset (%d, %d), with alpha fixup? %x\n",
	     __FUNCTION__,
	     tmp->dst.x, tmp->dst.y, sx, sy, alpha_fixup));

	tmp->u.blt.src_pixmap = NULL;
	tmp->u.blt.sx = sx;
	tmp->u.blt.sy = sy;

	kgem_set_mode(&sna->kgem, KGEM_BLT, tmp->dst.bo);
	if (!kgem_check_many_bo_fenced(&sna->kgem, tmp->dst.bo, tmp->src.bo, NULL)) {
		kgem_submit(&sna->kgem);
		if (!kgem_check_many_bo_fenced(&sna->kgem,
					       tmp->dst.bo, tmp->src.bo, NULL)) {
			DBG(("%s: fallback -- no room in aperture\n", __FUNCTION__));
			return sna_tiling_blt_composite(sna, tmp, tmp->src.bo,
							PICT_FORMAT_BPP(tmp->src.pict_format),
							alpha_fixup);
		}
		_kgem_set_mode(&sna->kgem, KGEM_BLT);
	}

	if (alpha_fixup) {
		tmp->blt   = blt_composite_copy_with_alpha;
		tmp->box   = blt_composite_copy_box_with_alpha;
		tmp->boxes = blt_composite_copy_boxes_with_alpha;

		if (!sna_blt_alpha_fixup_init(sna, &tmp->u.blt,
					      tmp->src.bo, tmp->dst.bo,
					      PICT_FORMAT_BPP(tmp->src.pict_format),
					      alpha_fixup))
			return false;
	} else {
		tmp->blt   = blt_composite_copy;
		tmp->box   = blt_composite_copy_box;
		tmp->boxes = blt_composite_copy_boxes;
		tmp->thread_boxes = blt_composite_copy_boxes__thread;

		if (!sna_blt_copy_init(sna, &tmp->u.blt,
				       tmp->src.bo, tmp->dst.bo,
				       PICT_FORMAT_BPP(tmp->src.pict_format),
				       GXcopy))
			return false;
	}

	tmp->done = convert_done;
	if (sna->kgem.gen >= 060 && tmp->src.bo == tmp->dst.bo)
		tmp->done = gen6_convert_done;

	return true;
}

static void sna_blt_fill_op_blt(struct sna *sna,
				const struct sna_fill_op *op,
				int16_t x, int16_t y,
				int16_t width, int16_t height)
{
	if (sna->blt_state.fill_bo != op->base.u.blt.bo[0]->unique_id) {
		const struct sna_blt_state *blt = &op->base.u.blt;

		sna_blt_fill_begin(sna, blt);

		sna->blt_state.fill_bo = blt->bo[0]->unique_id;
		sna->blt_state.fill_pixel = blt->pixel;
		sna->blt_state.fill_alu = blt->alu;
	}

	sna_blt_fill_one(sna, &op->base.u.blt, x, y, width, height);
}

fastcall static void sna_blt_fill_op_box(struct sna *sna,
					 const struct sna_fill_op *op,
					 const BoxRec *box)
{
	if (sna->blt_state.fill_bo != op->base.u.blt.bo[0]->unique_id) {
		const struct sna_blt_state *blt = &op->base.u.blt;

		sna_blt_fill_begin(sna, blt);

		sna->blt_state.fill_bo = blt->bo[0]->unique_id;
		sna->blt_state.fill_pixel = blt->pixel;
		sna->blt_state.fill_alu = blt->alu;
	}

	_sna_blt_fill_box(sna, &op->base.u.blt, box);
}

fastcall static void sna_blt_fill_op_boxes(struct sna *sna,
					   const struct sna_fill_op *op,
					   const BoxRec *box,
					   int nbox)
{
	if (sna->blt_state.fill_bo != op->base.u.blt.bo[0]->unique_id) {
		const struct sna_blt_state *blt = &op->base.u.blt;

		sna_blt_fill_begin(sna, blt);

		sna->blt_state.fill_bo = blt->bo[0]->unique_id;
		sna->blt_state.fill_pixel = blt->pixel;
		sna->blt_state.fill_alu = blt->alu;
	}

	_sna_blt_fill_boxes(sna, &op->base.u.blt, box, nbox);
}

static inline uint64_t pt_add(uint32_t cmd, const DDXPointRec *pt, int16_t dx, int16_t dy)
{
	union {
		DDXPointRec pt;
		uint32_t i;
	} u;

	u.pt.x = pt->x + dx;
	u.pt.y = pt->y + dy;

	return cmd | (uint64_t)u.i<<32;
}

fastcall static void sna_blt_fill_op_points(struct sna *sna,
					    const struct sna_fill_op *op,
					    int16_t dx, int16_t dy,
					    const DDXPointRec *p, int n)
{
	const struct sna_blt_state *blt = &op->base.u.blt;
	struct kgem *kgem = &sna->kgem;
	uint32_t cmd;

	DBG(("%s: %08x x %d\n", __FUNCTION__, blt->pixel, n));

	if (sna->blt_state.fill_bo != op->base.u.blt.bo[0]->unique_id) {
		sna_blt_fill_begin(sna, blt);

		sna->blt_state.fill_bo = blt->bo[0]->unique_id;
		sna->blt_state.fill_pixel = blt->pixel;
		sna->blt_state.fill_alu = blt->alu;
	}

	if (!kgem_check_batch(kgem, 2))
		sna_blt_fill_begin(sna, blt);

	cmd = XY_PIXEL_BLT;
	if (kgem->gen >= 040 && op->base.u.blt.bo[0]->tiling)
		cmd |= BLT_DST_TILED;

	do {
		uint32_t *b = kgem->batch + kgem->nbatch;
		int n_this_time;

		assert(sna->kgem.mode == KGEM_BLT);
		n_this_time = n;
		if (2*n_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
			n_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 2;
		assert(n_this_time);
		n -= n_this_time;

		kgem->nbatch += 2 * n_this_time;
		assert(kgem->nbatch < kgem->surface);

		if ((dx|dy) == 0) {
			while (n_this_time >= 8) {
				*((uint64_t *)b + 0) = pt_add(cmd, p+0, 0, 0);
				*((uint64_t *)b + 1) = pt_add(cmd, p+1, 0, 0);
				*((uint64_t *)b + 2) = pt_add(cmd, p+2, 0, 0);
				*((uint64_t *)b + 3) = pt_add(cmd, p+3, 0, 0);
				*((uint64_t *)b + 4) = pt_add(cmd, p+4, 0, 0);
				*((uint64_t *)b + 5) = pt_add(cmd, p+5, 0, 0);
				*((uint64_t *)b + 6) = pt_add(cmd, p+6, 0, 0);
				*((uint64_t *)b + 7) = pt_add(cmd, p+7, 0, 0);
				b += 16;
				n_this_time -= 8;
				p += 8;
			}
			if (n_this_time & 4) {
				*((uint64_t *)b + 0) = pt_add(cmd, p+0, 0, 0);
				*((uint64_t *)b + 1) = pt_add(cmd, p+1, 0, 0);
				*((uint64_t *)b + 2) = pt_add(cmd, p+2, 0, 0);
				*((uint64_t *)b + 3) = pt_add(cmd, p+3, 0, 0);
				b += 8;
				p += 4;
			}
			if (n_this_time & 2) {
				*((uint64_t *)b + 0) = pt_add(cmd, p+0, 0, 0);
				*((uint64_t *)b + 1) = pt_add(cmd, p+1, 0, 0);
				b += 4;
				p += 2;
			}
			if (n_this_time & 1)
				*((uint64_t *)b + 0) = pt_add(cmd, p++, 0, 0);
		} else {
			while (n_this_time >= 8) {
				*((uint64_t *)b + 0) = pt_add(cmd, p+0, dx, dy);
				*((uint64_t *)b + 1) = pt_add(cmd, p+1, dx, dy);
				*((uint64_t *)b + 2) = pt_add(cmd, p+2, dx, dy);
				*((uint64_t *)b + 3) = pt_add(cmd, p+3, dx, dy);
				*((uint64_t *)b + 4) = pt_add(cmd, p+4, dx, dy);
				*((uint64_t *)b + 5) = pt_add(cmd, p+5, dx, dy);
				*((uint64_t *)b + 6) = pt_add(cmd, p+6, dx, dy);
				*((uint64_t *)b + 7) = pt_add(cmd, p+7, dx, dy);
				b += 16;
				n_this_time -= 8;
				p += 8;
			}
			if (n_this_time & 4) {
				*((uint64_t *)b + 0) = pt_add(cmd, p+0, dx, dy);
				*((uint64_t *)b + 1) = pt_add(cmd, p+1, dx, dy);
				*((uint64_t *)b + 2) = pt_add(cmd, p+2, dx, dy);
				*((uint64_t *)b + 3) = pt_add(cmd, p+3, dx, dy);
				b += 8;
				p += 8;
			}
			if (n_this_time & 2) {
				*((uint64_t *)b + 0) = pt_add(cmd, p+0, dx, dy);
				*((uint64_t *)b + 1) = pt_add(cmd, p+1, dx, dy);
				b += 4;
				p += 2;
			}
			if (n_this_time & 1)
				*((uint64_t *)b + 0) = pt_add(cmd, p++, dx, dy);
		}

		if (!n)
			return;

		sna_blt_fill_begin(sna, blt);
	} while (1);
}

bool sna_blt_fill(struct sna *sna, uint8_t alu,
		  struct kgem_bo *bo, int bpp,
		  uint32_t pixel,
		  struct sna_fill_op *fill)
{
#if DEBUG_NO_BLT || NO_BLT_FILL
	return false;
#endif

	DBG(("%s(alu=%d, pixel=%x, bpp=%d)\n", __FUNCTION__, alu, pixel, bpp));

	if (!kgem_bo_can_blt(&sna->kgem, bo)) {
		DBG(("%s: rejected due to incompatible Y-tiling\n",
		     __FUNCTION__));
		return false;
	}

	if (!sna_blt_fill_init(sna, &fill->base.u.blt,
			       bo, bpp, alu, pixel))
		return false;

	fill->blt   = sna_blt_fill_op_blt;
	fill->box   = sna_blt_fill_op_box;
	fill->boxes = sna_blt_fill_op_boxes;
	fill->points = sna_blt_fill_op_points;
	fill->done  =
		(void (*)(struct sna *, const struct sna_fill_op *))nop_done;
	return true;
}

static void sna_blt_copy_op_blt(struct sna *sna,
				const struct sna_copy_op *op,
				int16_t src_x, int16_t src_y,
				int16_t width, int16_t height,
				int16_t dst_x, int16_t dst_y)
{
	sna_blt_copy_one(sna, &op->base.u.blt,
			 src_x, src_y,
			 width, height,
			 dst_x, dst_y);
}

bool sna_blt_copy(struct sna *sna, uint8_t alu,
		  struct kgem_bo *src,
		  struct kgem_bo *dst,
		  int bpp,
		  struct sna_copy_op *op)
{
#if DEBUG_NO_BLT || NO_BLT_COPY
	return false;
#endif

	if (!kgem_bo_can_blt(&sna->kgem, src))
		return false;

	if (!kgem_bo_can_blt(&sna->kgem, dst))
		return false;

	if (!sna_blt_copy_init(sna, &op->base.u.blt,
			       src, dst,
			       bpp, alu))
		return false;

	op->blt  = sna_blt_copy_op_blt;
	if (sna->kgem.gen >= 060 && src == dst)
		op->done = (void (*)(struct sna *, const struct sna_copy_op *))
			    gen6_blt_copy_done;
	else
		op->done = (void (*)(struct sna *, const struct sna_copy_op *))
			    nop_done;
	return true;
}

static bool sna_blt_fill_box(struct sna *sna, uint8_t alu,
			     struct kgem_bo *bo, int bpp,
			     uint32_t color,
			     const BoxRec *box)
{
	struct kgem *kgem = &sna->kgem;
	uint32_t br13, cmd, *b;
	bool overwrites;

	assert(kgem_bo_can_blt (kgem, bo));

	DBG(("%s: box=((%d, %d), (%d, %d))\n", __FUNCTION__,
	     box->x1, box->y1, box->x2, box->y2));

	assert(box->x1 >= 0);
	assert(box->y1 >= 0);

	cmd = XY_COLOR_BLT | (kgem->gen >= 0100 ? 5 : 4);
	br13 = bo->pitch;
	if (kgem->gen >= 040 && bo->tiling) {
		cmd |= BLT_DST_TILED;
		br13 >>= 2;
	}
	assert(br13 <= MAXSHORT);

	br13 |= fill_ROP[alu] << 16;
	switch (bpp) {
	default: assert(0);
	case 32: cmd |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
		 br13 |= 1 << 25; /* RGB8888 */
	case 16: br13 |= 1 << 24; /* RGB565 */
	case 8: break;
	}

	/* All too frequently one blt completely overwrites the previous */
	overwrites = alu == GXcopy || alu == GXclear || alu == GXset;
	if (overwrites) {
		if (sna->kgem.gen >= 0100) {
			if (kgem->nbatch >= 7 &&
			    kgem->batch[kgem->nbatch-7] == cmd &&
			    *(uint64_t *)&kgem->batch[kgem->nbatch-5] == *(const uint64_t *)box &&
			    kgem->reloc[kgem->nreloc-1].target_handle == bo->target_handle) {
				DBG(("%s: replacing last fill\n", __FUNCTION__));
				kgem->batch[kgem->nbatch-6] = br13;
				kgem->batch[kgem->nbatch-1] = color;
				return true;
			}
			if (kgem->nbatch >= 10 &&
			    (kgem->batch[kgem->nbatch-10] & 0xffc00000) == XY_SRC_COPY_BLT_CMD &&
			    *(uint64_t *)&kgem->batch[kgem->nbatch-8] == *(const uint64_t *)box &&
			    kgem->reloc[kgem->nreloc-2].target_handle == bo->target_handle) {
				DBG(("%s: replacing last copy\n", __FUNCTION__));
				kgem->batch[kgem->nbatch-10] = cmd;
				kgem->batch[kgem->nbatch-8] = br13;
				kgem->batch[kgem->nbatch-4] = color;
				/* Keep the src bo as part of the execlist, just remove
				 * its relocation entry.
				 */
				kgem->nreloc--;
				kgem->nbatch -= 3;
				return true;
			}
		} else {
			if (kgem->nbatch >= 6 &&
			    kgem->batch[kgem->nbatch-6] == cmd &&
			    *(uint64_t *)&kgem->batch[kgem->nbatch-4] == *(const uint64_t *)box &&
			    kgem->reloc[kgem->nreloc-1].target_handle == bo->target_handle) {
				DBG(("%s: replacing last fill\n", __FUNCTION__));
				kgem->batch[kgem->nbatch-5] = br13;
				kgem->batch[kgem->nbatch-1] = color;
				return true;
			}
			if (kgem->nbatch >= 8 &&
			    (kgem->batch[kgem->nbatch-8] & 0xffc00000) == XY_SRC_COPY_BLT_CMD &&
			    *(uint64_t *)&kgem->batch[kgem->nbatch-6] == *(const uint64_t *)box &&
			    kgem->reloc[kgem->nreloc-2].target_handle == bo->target_handle) {
				DBG(("%s: replacing last copy\n", __FUNCTION__));
				kgem->batch[kgem->nbatch-8] = cmd;
				kgem->batch[kgem->nbatch-7] = br13;
				kgem->batch[kgem->nbatch-3] = color;
				/* Keep the src bo as part of the execlist, just remove
				 * its relocation entry.
				 */
				kgem->nreloc--;
				kgem->nbatch -= 2;
				return true;
			}
		}
	}

	/* If we are currently emitting SCANLINES, keep doing so */
	if (sna->blt_state.fill_bo == bo->unique_id &&
	    sna->blt_state.fill_pixel == color &&
	    (sna->blt_state.fill_alu == alu ||
	     sna->blt_state.fill_alu == ~alu)) {
		DBG(("%s: matching last fill, converting to scanlines\n",
		     __FUNCTION__));
		return false;
	}

	kgem_set_mode(kgem, KGEM_BLT, bo);
	if (!kgem_check_batch(kgem, 7) ||
	    !kgem_check_reloc(kgem, 1) ||
	    !kgem_check_bo_fenced(kgem, bo)) {
		kgem_submit(kgem);
		if (!kgem_check_bo_fenced(&sna->kgem, bo))
			return false;

		_kgem_set_mode(kgem, KGEM_BLT);
	}

	assert(kgem_check_batch(kgem, 6));
	assert(kgem_check_reloc(kgem, 1));

	assert(sna->kgem.mode == KGEM_BLT);
	b = kgem->batch + kgem->nbatch;
	b[0] = cmd;
	b[1] = br13;
	*(uint64_t *)(b+2) = *(const uint64_t *)box;
	if (kgem->gen >= 0100) {
		*(uint64_t *)(b+4) =
			kgem_add_reloc64(kgem, kgem->nbatch + 4, bo,
					 I915_GEM_DOMAIN_RENDER << 16 |
					 I915_GEM_DOMAIN_RENDER |
					 KGEM_RELOC_FENCED,
					 0);
		b[6] = color;
		kgem->nbatch += 7;
	} else {
		b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, bo,
				      I915_GEM_DOMAIN_RENDER << 16 |
				      I915_GEM_DOMAIN_RENDER |
				      KGEM_RELOC_FENCED,
				      0);
		b[5] = color;
		kgem->nbatch += 6;
	}
	assert(kgem->nbatch < kgem->surface);

	sna->blt_state.fill_bo = bo->unique_id;
	sna->blt_state.fill_pixel = color;
	sna->blt_state.fill_alu = ~alu;
	return true;
}

bool sna_blt_fill_boxes(struct sna *sna, uint8_t alu,
			struct kgem_bo *bo, int bpp,
			uint32_t pixel,
			const BoxRec *box, int nbox)
{
	struct kgem *kgem = &sna->kgem;
	uint32_t br13, cmd;

#if DEBUG_NO_BLT || NO_BLT_FILL_BOXES
	return false;
#endif

	DBG(("%s (%d, %08x, %d) x %d\n",
	     __FUNCTION__, bpp, pixel, alu, nbox));

	if (!kgem_bo_can_blt(kgem, bo)) {
		DBG(("%s: fallback -- cannot blt to dst\n", __FUNCTION__));
		return false;
	}

	if (alu == GXclear)
		pixel = 0;
	else if (alu == GXcopy) {
		if (pixel == 0)
			alu = GXclear;
		else if (pixel == -1)
			alu = GXset;
	}

	if (nbox == 1 && sna_blt_fill_box(sna, alu, bo, bpp, pixel, box))
		return true;

	br13 = bo->pitch;
	cmd = XY_SCANLINE_BLT;
	if (kgem->gen >= 040 && bo->tiling) {
		cmd |= 1 << 11;
		br13 >>= 2;
	}
	assert(br13 <= MAXSHORT);

	br13 |= 1<<31 | fill_ROP[alu] << 16;
	switch (bpp) {
	default: assert(0);
	case 32: br13 |= 1 << 25; /* RGB8888 */
	case 16: br13 |= 1 << 24; /* RGB565 */
	case 8: break;
	}

	kgem_set_mode(kgem, KGEM_BLT, bo);
	if (!kgem_check_batch(kgem, 14) ||
	    !kgem_check_bo_fenced(kgem, bo)) {
		kgem_submit(kgem);
		if (!kgem_check_bo_fenced(&sna->kgem, bo))
			return false;
		_kgem_set_mode(kgem, KGEM_BLT);
	}

	if (sna->blt_state.fill_bo != bo->unique_id ||
	    sna->blt_state.fill_pixel != pixel ||
	    sna->blt_state.fill_alu != alu)
	{
		uint32_t *b;

		if (!kgem_check_reloc(kgem, 1)) {
			_kgem_submit(kgem);
			if (!kgem_check_bo_fenced(&sna->kgem, bo))
				return false;
			_kgem_set_mode(kgem, KGEM_BLT);
		}

		assert(sna->kgem.mode == KGEM_BLT);
		b = kgem->batch + kgem->nbatch;
		if (kgem->gen >= 0100) {
			b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 8;
			if (bpp == 32)
				b[0] |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
			if (bo->tiling)
				b[0] |= BLT_DST_TILED;
			b[1] = br13;
			b[2] = 0;
			b[3] = 0;
			*(uint64_t *)(b+4) =
				kgem_add_reloc64(kgem, kgem->nbatch + 4, bo,
						 I915_GEM_DOMAIN_RENDER << 16 |
						 I915_GEM_DOMAIN_RENDER |
						 KGEM_RELOC_FENCED,
						 0);
			b[6] = pixel;
			b[7] = pixel;
			b[8] = 0;
			b[9] = 0;
			kgem->nbatch += 10;
		} else {
			b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 7;
			if (bpp == 32)
				b[0] |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
			if (bo->tiling && kgem->gen >= 040)
				b[0] |= BLT_DST_TILED;
			b[1] = br13;
			b[2] = 0;
			b[3] = 0;
			b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, bo,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      I915_GEM_DOMAIN_RENDER |
					      KGEM_RELOC_FENCED,
					      0);
			b[5] = pixel;
			b[6] = pixel;
			b[7] = 0;
			b[8] = 0;
			kgem->nbatch += 9;
		}
		assert(kgem->nbatch < kgem->surface);

		sna->blt_state.fill_bo = bo->unique_id;
		sna->blt_state.fill_pixel = pixel;
		sna->blt_state.fill_alu = alu;
	}

	do {
		int nbox_this_time;

		nbox_this_time = nbox;
		if (3*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
			nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 3;
		assert(nbox_this_time);
		nbox -= nbox_this_time;

		assert(sna->kgem.mode == KGEM_BLT);
		do {
			uint32_t *b;

			DBG(("%s: (%d, %d), (%d, %d): %08x\n",
			     __FUNCTION__,
			     box->x1, box->y1,
			     box->x2, box->y2,
			     pixel));

			assert(box->x1 >= 0);
			assert(box->y1 >= 0);
			assert(box->y2 * bo->pitch <= kgem_bo_size(bo));

			b = kgem->batch + kgem->nbatch;
			kgem->nbatch += 3;
			assert(kgem->nbatch < kgem->surface);
			b[0] = cmd;
			*(uint64_t *)(b+1) = *(const uint64_t *)box;
			box++;
		} while (--nbox_this_time);

		if (nbox) {
			uint32_t *b;

			_kgem_submit(kgem);
			_kgem_set_mode(kgem, KGEM_BLT);

			assert(sna->kgem.mode == KGEM_BLT);
			b = kgem->batch + kgem->nbatch;
			if (kgem->gen >= 0100) {
				b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 8;
				if (bpp == 32)
					b[0] |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
				if (bo->tiling)
					b[0] |= BLT_DST_TILED;
				b[1] = br13;
				b[2] = 0;
				b[3] = 0;
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(kgem, kgem->nbatch + 4, bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 I915_GEM_DOMAIN_RENDER |
							 KGEM_RELOC_FENCED,
							 0);
				b[6] = pixel;
				b[7] = pixel;
				b[8] = 0;
				b[9] = 0;
				kgem->nbatch += 10;
			} else {
				b[0] = XY_SETUP_MONO_PATTERN_SL_BLT | 7;
				if (bpp == 32)
					b[0] |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
				if (bo->tiling && kgem->gen >= 040)
					b[0] |= BLT_DST_TILED;
				b[1] = br13;
				b[2] = 0;
				b[3] = 0;
				b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = pixel;
				b[6] = pixel;
				b[7] = 0;
				b[8] = 0;
				kgem->nbatch += 9;
			}
			assert(kgem->nbatch < kgem->surface);
		}
	} while (nbox);

	if (kgem->nexec > 1 && __kgem_ring_empty(kgem)) {
		DBG(("%s: flushing BLT operation on empty ring\n", __FUNCTION__));
		_kgem_submit(kgem);
	}

	return true;
}

bool sna_blt_copy_boxes(struct sna *sna, uint8_t alu,
			struct kgem_bo *src_bo, int16_t src_dx, int16_t src_dy,
			struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
			int bpp, const BoxRec *box, int nbox)
{
	struct kgem *kgem = &sna->kgem;
	unsigned src_pitch, br13, cmd;

#if DEBUG_NO_BLT || NO_BLT_COPY_BOXES
	return false;
#endif

	DBG(("%s src=(%d, %d) -> (%d, %d) x %d, tiling=(%d, %d), pitch=(%d, %d)\n",
	     __FUNCTION__, src_dx, src_dy, dst_dx, dst_dy, nbox,
	    src_bo->tiling, dst_bo->tiling,
	    src_bo->pitch, dst_bo->pitch));
	assert(nbox);

	if (wedged(sna) || !kgem_bo_can_blt(kgem, src_bo) || !kgem_bo_can_blt(kgem, dst_bo)) {
		DBG(("%s: cannot blt to src? %d or dst? %d\n",
		     __FUNCTION__,
		     kgem_bo_can_blt(kgem, src_bo),
		     kgem_bo_can_blt(kgem, dst_bo)));
		return false;
	}

	cmd = XY_SRC_COPY_BLT_CMD;
	if (bpp == 32)
		cmd |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;

	src_pitch = src_bo->pitch;
	if (kgem->gen >= 040 && src_bo->tiling) {
		cmd |= BLT_SRC_TILED;
		src_pitch >>= 2;
	}
	assert(src_pitch <= MAXSHORT);

	br13 = dst_bo->pitch;
	if (kgem->gen >= 040 && dst_bo->tiling) {
		cmd |= BLT_DST_TILED;
		br13 >>= 2;
	}
	assert(br13 <= MAXSHORT);

	br13 |= copy_ROP[alu] << 16;
	switch (bpp) {
	default: assert(0);
	case 32: br13 |= 1 << 25; /* RGB8888 */
	case 16: br13 |= 1 << 24; /* RGB565 */
	case 8: break;
	}

	/* Compare first box against a previous fill */
	if ((alu == GXcopy || alu == GXclear || alu == GXset) &&
	    kgem->reloc[kgem->nreloc-1].target_handle == dst_bo->target_handle) {
		if (kgem->gen >= 0100) {
			if (kgem->nbatch >= 7 &&
			    kgem->batch[kgem->nbatch-7] == (XY_COLOR_BLT | (cmd & (BLT_DST_TILED | BLT_WRITE_ALPHA | BLT_WRITE_RGB)) | 5) &&
			    kgem->batch[kgem->nbatch-5] == ((uint32_t)(box->y1 + dst_dy) << 16 | (uint16_t)(box->x1 + dst_dx)) &&
			    kgem->batch[kgem->nbatch-4] == ((uint32_t)(box->y2 + dst_dy) << 16 | (uint16_t)(box->x2 + dst_dx))) {
				DBG(("%s: deleting last fill\n", __FUNCTION__));
				kgem->nbatch -= 7;
				kgem->nreloc--;
			}
		} else {
			if (kgem->nbatch >= 6 &&
			    kgem->batch[kgem->nbatch-6] == (XY_COLOR_BLT | (cmd & (BLT_DST_TILED | BLT_WRITE_ALPHA | BLT_WRITE_RGB)) | 4) &&
			    kgem->batch[kgem->nbatch-4] == ((uint32_t)(box->y1 + dst_dy) << 16 | (uint16_t)(box->x1 + dst_dx)) &&
			    kgem->batch[kgem->nbatch-3] == ((uint32_t)(box->y2 + dst_dy) << 16 | (uint16_t)(box->x2 + dst_dx))) {
				DBG(("%s: deleting last fill\n", __FUNCTION__));
				kgem->nbatch -= 6;
				kgem->nreloc--;
			}
		}
	}

	kgem_set_mode(kgem, KGEM_BLT, dst_bo);
	if (!kgem_check_batch(kgem, 10) ||
	    !kgem_check_reloc(kgem, 2) ||
	    !kgem_check_many_bo_fenced(kgem, dst_bo, src_bo, NULL)) {
		kgem_submit(kgem);
		if (!kgem_check_many_bo_fenced(kgem, dst_bo, src_bo, NULL)) {
			DBG(("%s: not enough room in aperture, fallback to tiling copy\n", __FUNCTION__));
			return sna_tiling_blt_copy_boxes(sna, alu,
							 src_bo, src_dx, src_dy,
							 dst_bo, dst_dx, dst_dy,
							 bpp, box, nbox);
		}
		_kgem_set_mode(kgem, KGEM_BLT);
	}

	if ((dst_dx | dst_dy) == 0) {
		if (kgem->gen >= 0100) {
			uint64_t hdr = (uint64_t)br13 << 32 | cmd | 8;
			do {
				int nbox_this_time;

				nbox_this_time = nbox;
				if (10*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
					nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
				if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
					nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc)/2;
				assert(nbox_this_time);
				nbox -= nbox_this_time;

				assert(sna->kgem.mode == KGEM_BLT);
				do {
					uint32_t *b = kgem->batch + kgem->nbatch;

					DBG(("  %s: box=(%d, %d)x(%d, %d)\n",
					     __FUNCTION__,
					     box->x1, box->y1,
					     box->x2 - box->x1, box->y2 - box->y1));

					assert(box->x1 + src_dx >= 0);
					assert(box->y1 + src_dy >= 0);
					assert(box->x1 + src_dx <= INT16_MAX);
					assert(box->y1 + src_dy <= INT16_MAX);

					assert(box->x1 >= 0);
					assert(box->y1 >= 0);

					*(uint64_t *)&b[0] = hdr;
					*(uint64_t *)&b[2] = *(const uint64_t *)box;
					*(uint64_t *)(b+4) =
						kgem_add_reloc64(kgem, kgem->nbatch + 4, dst_bo,
								 I915_GEM_DOMAIN_RENDER << 16 |
								 I915_GEM_DOMAIN_RENDER |
								 KGEM_RELOC_FENCED,
								 0);
					b[6] = add2(b[2], src_dx, src_dy);
					b[7] = src_pitch;
					*(uint64_t *)(b+8) =
						kgem_add_reloc64(kgem, kgem->nbatch + 8, src_bo,
								 I915_GEM_DOMAIN_RENDER << 16 |
								 KGEM_RELOC_FENCED,
								 0);
					kgem->nbatch += 10;
					assert(kgem->nbatch < kgem->surface);
					box++;
				} while (--nbox_this_time);

				if (!nbox)
					break;

				_kgem_submit(kgem);
				_kgem_set_mode(kgem, KGEM_BLT);
			} while (1);
		} else {
			uint64_t hdr = (uint64_t)br13 << 32 | cmd | 6;
			do {
				int nbox_this_time;

				nbox_this_time = nbox;
				if (8*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
					nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
				if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
					nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc)/2;
				assert(nbox_this_time);
				nbox -= nbox_this_time;

				assert(sna->kgem.mode == KGEM_BLT);
				do {
					uint32_t *b = kgem->batch + kgem->nbatch;

					DBG(("  %s: box=(%d, %d)x(%d, %d)\n",
					     __FUNCTION__,
					     box->x1, box->y1,
					     box->x2 - box->x1, box->y2 - box->y1));

					assert(box->x1 + src_dx >= 0);
					assert(box->y1 + src_dy >= 0);
					assert(box->x1 + src_dx <= INT16_MAX);
					assert(box->y1 + src_dy <= INT16_MAX);

					assert(box->x1 >= 0);
					assert(box->y1 >= 0);

					*(uint64_t *)&b[0] = hdr;
					*(uint64_t *)&b[2] = *(const uint64_t *)box;
					b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, dst_bo,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      I915_GEM_DOMAIN_RENDER |
							      KGEM_RELOC_FENCED,
							      0);
					b[5] = add2(b[2], src_dx, src_dy);
					b[6] = src_pitch;
					b[7] = kgem_add_reloc(kgem, kgem->nbatch + 7, src_bo,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      KGEM_RELOC_FENCED,
							      0);
					kgem->nbatch += 8;
					assert(kgem->nbatch < kgem->surface);
					box++;
				} while (--nbox_this_time);

				if (!nbox)
					break;

				_kgem_submit(kgem);
				_kgem_set_mode(kgem, KGEM_BLT);
			} while (1);
		}
	} else {
		if (kgem->gen >= 0100) {
			cmd |= 8;
			do {
				int nbox_this_time;

				nbox_this_time = nbox;
				if (10*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
					nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
				if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
					nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc)/2;
				assert(nbox_this_time);
				nbox -= nbox_this_time;

				assert(sna->kgem.mode == KGEM_BLT);
				do {
					uint32_t *b = kgem->batch + kgem->nbatch;

					DBG(("  %s: box=(%d, %d)x(%d, %d)\n",
					     __FUNCTION__,
					     box->x1, box->y1,
					     box->x2 - box->x1, box->y2 - box->y1));

					assert(box->x1 + src_dx >= 0);
					assert(box->y1 + src_dy >= 0);

					assert(box->x1 + dst_dx >= 0);
					assert(box->y1 + dst_dy >= 0);

					b[0] = cmd;
					b[1] = br13;
					b[2] = ((box->y1 + dst_dy) << 16) | (box->x1 + dst_dx);
					b[3] = ((box->y2 + dst_dy) << 16) | (box->x2 + dst_dx);
					*(uint64_t *)(b+4) =
						kgem_add_reloc64(kgem, kgem->nbatch + 4, dst_bo,
								 I915_GEM_DOMAIN_RENDER << 16 |
								 I915_GEM_DOMAIN_RENDER |
								 KGEM_RELOC_FENCED,
								 0);
					b[6] = ((box->y1 + src_dy) << 16) | (box->x1 + src_dx);
					b[7] = src_pitch;
					*(uint64_t *)(b+8) =
						kgem_add_reloc64(kgem, kgem->nbatch + 8, src_bo,
								 I915_GEM_DOMAIN_RENDER << 16 |
								 KGEM_RELOC_FENCED,
								 0);
					kgem->nbatch += 10;
					assert(kgem->nbatch < kgem->surface);
					box++;
				} while (--nbox_this_time);

				if (!nbox)
					break;

				_kgem_submit(kgem);
				_kgem_set_mode(kgem, KGEM_BLT);
			} while (1);
		} else {
			cmd |= 6;
			do {
				int nbox_this_time;

				nbox_this_time = nbox;
				if (8*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
					nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
				if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
					nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc)/2;
				assert(nbox_this_time);
				nbox -= nbox_this_time;

				assert(sna->kgem.mode == KGEM_BLT);
				do {
					uint32_t *b = kgem->batch + kgem->nbatch;

					DBG(("  %s: box=(%d, %d)x(%d, %d)\n",
					     __FUNCTION__,
					     box->x1, box->y1,
					     box->x2 - box->x1, box->y2 - box->y1));

					assert(box->x1 + src_dx >= 0);
					assert(box->y1 + src_dy >= 0);

					assert(box->x1 + dst_dx >= 0);
					assert(box->y1 + dst_dy >= 0);

					b[0] = cmd;
					b[1] = br13;
					b[2] = ((box->y1 + dst_dy) << 16) | (box->x1 + dst_dx);
					b[3] = ((box->y2 + dst_dy) << 16) | (box->x2 + dst_dx);
					b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, dst_bo,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      I915_GEM_DOMAIN_RENDER |
							      KGEM_RELOC_FENCED,
							      0);
					b[5] = ((box->y1 + src_dy) << 16) | (box->x1 + src_dx);
					b[6] = src_pitch;
					b[7] = kgem_add_reloc(kgem, kgem->nbatch + 7, src_bo,
							      I915_GEM_DOMAIN_RENDER << 16 |
							      KGEM_RELOC_FENCED,
							      0);
					kgem->nbatch += 8;
					assert(kgem->nbatch < kgem->surface);
					box++;
				} while (--nbox_this_time);

				if (!nbox)
					break;

				_kgem_submit(kgem);
				_kgem_set_mode(kgem, KGEM_BLT);
			} while (1);
		}
	}

	if (kgem->nexec > 1 && __kgem_ring_empty(kgem)) {
		DBG(("%s: flushing BLT operation on empty ring\n", __FUNCTION__));
		_kgem_submit(kgem);
	} else if (kgem->gen >= 060 && src_bo == dst_bo && kgem_check_batch(kgem, 3)) {
		uint32_t *b = kgem->batch + kgem->nbatch;
		assert(sna->kgem.mode == KGEM_BLT);
		b[0] = XY_SETUP_CLIP;
		b[1] = b[2] = 0;
		kgem->nbatch += 3;
		assert(kgem->nbatch < kgem->surface);
	}

	sna->blt_state.fill_bo = 0;
	return true;
}

bool sna_blt_copy_boxes__with_alpha(struct sna *sna, uint8_t alu,
				    struct kgem_bo *src_bo, int16_t src_dx, int16_t src_dy,
				    struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
				    int bpp, int alpha_fixup,
				    const BoxRec *box, int nbox)
{
	struct kgem *kgem = &sna->kgem;
	unsigned src_pitch, br13, cmd;

#if DEBUG_NO_BLT || NO_BLT_COPY_BOXES
	return false;
#endif

	DBG(("%s src=(%d, %d) -> (%d, %d) x %d, tiling=(%d, %d), pitch=(%d, %d)\n",
	     __FUNCTION__, src_dx, src_dy, dst_dx, dst_dy, nbox,
	    src_bo->tiling, dst_bo->tiling,
	    src_bo->pitch, dst_bo->pitch));

	if (wedged(sna) || !kgem_bo_can_blt(kgem, src_bo) || !kgem_bo_can_blt(kgem, dst_bo)) {
		DBG(("%s: cannot blt to src? %d or dst? %d\n",
		     __FUNCTION__,
		     kgem_bo_can_blt(kgem, src_bo),
		     kgem_bo_can_blt(kgem, dst_bo)));
		return false;
	}

	cmd = XY_FULL_MONO_PATTERN_BLT | (kgem->gen >= 0100 ? 12 : 10);
	src_pitch = src_bo->pitch;
	if (kgem->gen >= 040 && src_bo->tiling) {
		cmd |= BLT_SRC_TILED;
		src_pitch >>= 2;
	}
	assert(src_pitch <= MAXSHORT);

	br13 = dst_bo->pitch;
	if (kgem->gen >= 040 && dst_bo->tiling) {
		cmd |= BLT_DST_TILED;
		br13 >>= 2;
	}
	assert(br13 <= MAXSHORT);

	br13 |= copy_ROP[alu] << 16;
	switch (bpp) {
	default: assert(0);
	case 32: br13 |= 1 << 25; /* RGB8888 */
	case 16: br13 |= 1 << 24; /* RGB565 */
	case 8: break;
	}

	kgem_set_mode(kgem, KGEM_BLT, dst_bo);
	if (!kgem_check_many_bo_fenced(kgem, dst_bo, src_bo, NULL)) {
		DBG(("%s: cannot fit src+dst into aperture\n", __FUNCTION__));
		return false;
	}

	/* Compare first box against a previous fill */
	if ((alu == GXcopy || alu == GXclear || alu == GXset) &&
	    kgem->reloc[kgem->nreloc-1].target_handle == dst_bo->target_handle) {
		if (kgem->gen >= 0100) {
			if (kgem->nbatch >= 7 &&
			    kgem->batch[kgem->nbatch-7] == (XY_COLOR_BLT | (cmd & (BLT_WRITE_ALPHA | BLT_WRITE_RGB)) | 5) &&
			    kgem->batch[kgem->nbatch-5] == ((uint32_t)(box->y1 + dst_dy) << 16 | (uint16_t)(box->x1 + dst_dx)) &&
			    kgem->batch[kgem->nbatch-4] == ((uint32_t)(box->y2 + dst_dy) << 16 | (uint16_t)(box->x2 + dst_dx))) {
				DBG(("%s: deleting last fill\n", __FUNCTION__));
				kgem->nbatch -= 7;
				kgem->nreloc--;
			}
		} else {
			if (kgem->nbatch >= 6 &&
			    kgem->batch[kgem->nbatch-6] == (XY_COLOR_BLT | (cmd & (BLT_WRITE_ALPHA | BLT_WRITE_RGB)) | 4) &&
			    kgem->batch[kgem->nbatch-4] == ((uint32_t)(box->y1 + dst_dy) << 16 | (uint16_t)(box->x1 + dst_dx)) &&
			    kgem->batch[kgem->nbatch-3] == ((uint32_t)(box->y2 + dst_dy) << 16 | (uint16_t)(box->x2 + dst_dx))) {
				DBG(("%s: deleting last fill\n", __FUNCTION__));
				kgem->nbatch -= 6;
				kgem->nreloc--;
			}
		}
	}

	while (nbox--) {
		uint32_t *b;

		if (!kgem_check_batch(kgem, 14) ||
		    !kgem_check_reloc(kgem, 2)) {
			_kgem_submit(kgem);
			_kgem_set_mode(kgem, KGEM_BLT);
		}

		assert(sna->kgem.mode == KGEM_BLT);
		b = kgem->batch + kgem->nbatch;
		b[0] = cmd;
		b[1] = br13;
		b[2] = (box->y1 + dst_dy) << 16 | (box->x1 + dst_dx);
		b[3] = (box->y2 + dst_dy) << 16 | (box->x2 + dst_dx);
		if (sna->kgem.gen >= 0100) {
			*(uint64_t *)(b+4) =
				kgem_add_reloc64(kgem, kgem->nbatch + 4, dst_bo,
						 I915_GEM_DOMAIN_RENDER << 16 |
						 I915_GEM_DOMAIN_RENDER |
						 KGEM_RELOC_FENCED,
						 0);
			b[6] = src_pitch;
			b[7] = (box->y1 + src_dy) << 16 | (box->x1 + src_dx);
			*(uint64_t *)(b+8) =
				kgem_add_reloc64(kgem, kgem->nbatch + 8, src_bo,
						 I915_GEM_DOMAIN_RENDER << 16 |
						 KGEM_RELOC_FENCED,
						 0);
			b[10] = alpha_fixup;
			b[11] = alpha_fixup;
			b[12] = 0;
			b[13] = 0;
			kgem->nbatch += 14;
		} else {
			b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, dst_bo,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      I915_GEM_DOMAIN_RENDER |
					      KGEM_RELOC_FENCED,
					      0);
			b[5] = src_pitch;
			b[6] = (box->y1 + src_dy) << 16 | (box->x1 + src_dx);
			b[7] = kgem_add_reloc(kgem, kgem->nbatch + 7, src_bo,
					      I915_GEM_DOMAIN_RENDER << 16 |
					      KGEM_RELOC_FENCED,
					      0);
			b[8] = alpha_fixup;
			b[9] = alpha_fixup;
			b[10] = 0;
			b[11] = 0;
			kgem->nbatch += 12;
		}
		assert(kgem->nbatch < kgem->surface);
		box++;
	}

	if (kgem->nexec > 1 && __kgem_ring_empty(kgem)) {
		DBG(("%s: flushing BLT operation on empty ring\n", __FUNCTION__));
		_kgem_submit(kgem);
	}

	sna->blt_state.fill_bo = 0;
	return true;
}

static void box_extents(const BoxRec *box, int n, BoxRec *extents)
{
	*extents = *box;
	while (--n) {
		box++;
		if (box->x1 < extents->x1)
			extents->x1 = box->x1;
		if (box->y1 < extents->y1)
			extents->y1 = box->y1;

		if (box->x2 > extents->x2)
			extents->x2 = box->x2;
		if (box->y2 > extents->y2)
			extents->y2 = box->y2;
	}
}

bool sna_blt_copy_boxes_fallback(struct sna *sna, uint8_t alu,
				 const DrawableRec *src, struct kgem_bo *src_bo, int16_t src_dx, int16_t src_dy,
				 const DrawableRec *dst, struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
				 const BoxRec *box, int nbox)
{
	struct kgem_bo *free_bo = NULL;
	bool ret;

	DBG(("%s: alu=%d, n=%d\n", __FUNCTION__, alu, nbox));

	if (!sna_blt_compare_depth(src, dst)) {
		DBG(("%s: mismatching depths %d -> %d\n",
		     __FUNCTION__, src->depth, dst->depth));
		return false;
	}

	if (src_bo == dst_bo) {
		DBG(("%s: dst == src\n", __FUNCTION__));

		if (src_bo->tiling == I915_TILING_Y &&
		    kgem_bo_blt_pitch_is_ok(&sna->kgem, src_bo)) {
			struct kgem_bo *bo;

			DBG(("%s: src is Y-tiled\n", __FUNCTION__));

			if (src->type != DRAWABLE_PIXMAP)
				return false;

			assert(sna_pixmap((PixmapPtr)src)->gpu_bo == src_bo);
			bo = sna_pixmap_change_tiling((PixmapPtr)src, I915_TILING_X);
			if (bo == NULL) {
				BoxRec extents;

				DBG(("%s: y-tiling conversion failed\n",
				     __FUNCTION__));

				box_extents(box, nbox, &extents);
				free_bo = kgem_create_2d(&sna->kgem,
							 extents.x2 - extents.x1,
							 extents.y2 - extents.y1,
							 src->bitsPerPixel,
							 I915_TILING_X, 0);
				if (free_bo == NULL) {
					DBG(("%s: fallback -- temp allocation failed\n",
					     __FUNCTION__));
					return false;
				}

				if (!sna_blt_copy_boxes(sna, GXcopy,
							src_bo, src_dx, src_dy,
							free_bo, -extents.x1, -extents.y1,
							src->bitsPerPixel,
							box, nbox)) {
					DBG(("%s: fallback -- temp copy failed\n",
					     __FUNCTION__));
					kgem_bo_destroy(&sna->kgem, free_bo);
					return false;
				}

				src_dx = -extents.x1;
				src_dy = -extents.y1;
				src_bo = free_bo;
			} else
				dst_bo = src_bo = bo;
		}
	} else {
		if (src_bo->tiling == I915_TILING_Y &&
		    kgem_bo_blt_pitch_is_ok(&sna->kgem, src_bo)) {
			DBG(("%s: src is y-tiled\n", __FUNCTION__));
			if (src->type != DRAWABLE_PIXMAP)
				return false;
			assert(sna_pixmap((PixmapPtr)src)->gpu_bo == src_bo);
			src_bo = sna_pixmap_change_tiling((PixmapPtr)src, I915_TILING_X);
			if (src_bo == NULL) {
				DBG(("%s: fallback -- src y-tiling conversion failed\n",
				     __FUNCTION__));
				return false;
			}
		}

		if (dst_bo->tiling == I915_TILING_Y &&
		    kgem_bo_blt_pitch_is_ok(&sna->kgem, dst_bo)) {
			DBG(("%s: dst is y-tiled\n", __FUNCTION__));
			if (dst->type != DRAWABLE_PIXMAP)
				return false;
			assert(sna_pixmap((PixmapPtr)dst)->gpu_bo == dst_bo);
			dst_bo = sna_pixmap_change_tiling((PixmapPtr)dst, I915_TILING_X);
			if (dst_bo == NULL) {
				DBG(("%s: fallback -- dst y-tiling conversion failed\n",
				     __FUNCTION__));
				return false;
			}
		}
	}

	ret =  sna_blt_copy_boxes(sna, alu,
				  src_bo, src_dx, src_dy,
				  dst_bo, dst_dx, dst_dy,
				  dst->bitsPerPixel,
				  box, nbox);

	if (free_bo)
		kgem_bo_destroy(&sna->kgem, free_bo);

	return ret;
}
