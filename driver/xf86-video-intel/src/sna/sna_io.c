/*
 * Copyright (c) 2011 Intel Corporation
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
#include "sna_reg.h"

#include <sys/mman.h>

#define PITCH(x, y) ALIGN((x)*(y), 4)

#define FORCE_INPLACE 0 /* 1 upload directly, -1 force indirect */

/* XXX Need to avoid using GTT fenced access for I915_TILING_Y on 855GM */

static inline bool upload_too_large(struct sna *sna, int width, int height)
{
	return width * height * 4 > sna->kgem.max_upload_tile_size;
}

static inline bool must_tile(struct sna *sna, int width, int height)
{
	return (width  > sna->render.max_3d_size ||
		height > sna->render.max_3d_size ||
		upload_too_large(sna, width, height));
}

static bool download_inplace__cpu(struct kgem *kgem,
				  PixmapPtr p, struct kgem_bo *bo,
				  const BoxRec *box, int nbox)
{
	BoxRec extents;

	switch (bo->tiling) {
	case I915_TILING_X:
		if (!kgem->memcpy_from_tiled_x)
			return false;
	case I915_TILING_NONE:
		break;
	default:
		return false;
	}

	if (!kgem_bo_can_map__cpu(kgem, bo, false))
		return false;

	if (kgem->has_llc)
		return true;

	extents = *box;
	while (--nbox) {
		++box;
		if (box->x1 < extents.x1)
			extents.x1 = box->x1;
		if (box->x2 > extents.x2)
			extents.x2 = box->x2;
		extents.y2 = box->y2;
	}

	if (extents.x2 - extents.x1 == p->drawable.width &&
	    extents.y2 - extents.y1 == p->drawable.height)
		return true;

	return __kgem_bo_size(bo) <= PAGE_SIZE;
}

static bool
read_boxes_inplace__cpu(struct kgem *kgem,
			PixmapPtr pixmap, struct kgem_bo *bo,
			const BoxRec *box, int n)
{
	int bpp = pixmap->drawable.bitsPerPixel;
	void *src, *dst = pixmap->devPrivate.ptr;
	int src_pitch = bo->pitch;
	int dst_pitch = pixmap->devKind;

	if (!download_inplace__cpu(kgem, dst, bo, box, n))
		return false;

	assert(kgem_bo_can_map__cpu(kgem, bo, false));
	assert(bo->tiling != I915_TILING_Y);

	src = kgem_bo_map__cpu(kgem, bo);
	if (src == NULL)
		return false;

	kgem_bo_sync__cpu_full(kgem, bo, 0);

	if (sigtrap_get())
		return false;

	DBG(("%s x %d\n", __FUNCTION__, n));

	if (bo->tiling == I915_TILING_X) {
		do {
			memcpy_from_tiled_x(kgem, src, dst, bpp, src_pitch, dst_pitch,
					    box->x1, box->y1,
					    box->x1, box->y1,
					    box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);
	} else {
		do {
			memcpy_blt(src, dst, bpp, src_pitch, dst_pitch,
				   box->x1, box->y1,
				   box->x1, box->y1,
				   box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);
	}

	sigtrap_put();
	return true;
}

static void read_boxes_inplace(struct kgem *kgem,
			       PixmapPtr pixmap, struct kgem_bo *bo,
			       const BoxRec *box, int n)
{
	int bpp = pixmap->drawable.bitsPerPixel;
	void *src, *dst = pixmap->devPrivate.ptr;
	int src_pitch = bo->pitch;
	int dst_pitch = pixmap->devKind;

	if (read_boxes_inplace__cpu(kgem, pixmap, bo, box, n))
		return;

	DBG(("%s x %d, tiling=%d\n", __FUNCTION__, n, bo->tiling));

	if (!kgem_bo_can_map(kgem, bo))
		return;

	kgem_bo_submit(kgem, bo);

	src = kgem_bo_map(kgem, bo);
	if (src == NULL)
		return;

	if (sigtrap_get())
		return;

	assert(src != dst);
	do {
		DBG(("%s: copying box (%d, %d), (%d, %d)\n",
		     __FUNCTION__, box->x1, box->y1, box->x2, box->y2));

		assert(box->x2 > box->x1);
		assert(box->y2 > box->y1);

		assert(box->x1 >= 0);
		assert(box->y1 >= 0);
		assert(box->x2 <= pixmap->drawable.width);
		assert(box->y2 <= pixmap->drawable.height);

		assert(box->x1 >= 0);
		assert(box->y1 >= 0);
		assert(box->x2 <= pixmap->drawable.width);
		assert(box->y2 <= pixmap->drawable.height);

		memcpy_blt(src, dst, bpp,
			   src_pitch, dst_pitch,
			   box->x1, box->y1,
			   box->x1, box->y1,
			   box->x2 - box->x1, box->y2 - box->y1);
		box++;
	} while (--n);

	sigtrap_put();
}

static bool download_inplace(struct kgem *kgem,
			     PixmapPtr p, struct kgem_bo *bo,
			     const BoxRec *box, int nbox)
{
	bool cpu;

	if (unlikely(kgem->wedged))
		return true;

	cpu = download_inplace__cpu(kgem, p, bo, box, nbox);
	if (!cpu && !kgem_bo_can_map(kgem, bo))
		return false;

	if (FORCE_INPLACE)
		return FORCE_INPLACE > 0;

	if (cpu)
		return true;

	if (kgem->can_blt_cpu && kgem->max_cpu_size)
		return false;

	return !__kgem_bo_is_busy(kgem, bo);
}

void sna_read_boxes(struct sna *sna, PixmapPtr dst, struct kgem_bo *src_bo,
		    const BoxRec *box, int nbox)
{
	struct kgem *kgem = &sna->kgem;
	struct kgem_bo *dst_bo;
	BoxRec extents;
	const BoxRec *tmp_box;
	int tmp_nbox;
	void *ptr;
	int src_pitch, cpp, offset;
	int n, cmd, br13;
	bool can_blt;

	DBG(("%s x %d, src=(handle=%d), dst=(size=(%d, %d)\n",
	     __FUNCTION__, nbox, src_bo->handle,
	     dst->drawable.width, dst->drawable.height));

#ifndef NDEBUG
	for (n = 0; n < nbox; n++) {
		if (box[n].x1 < 0 || box[n].y1 < 0 ||
		    box[n].x2 * dst->drawable.bitsPerPixel/8 > src_bo->pitch ||
		    box[n].y2 * src_bo->pitch > kgem_bo_size(src_bo))
		{
			FatalError("source out-of-bounds box[%d]=(%d, %d), (%d, %d), pitch=%d, size=%d\n", n,
				   box[n].x1, box[n].y1,
				   box[n].x2, box[n].y2,
				   src_bo->pitch, kgem_bo_size(src_bo));
		}
	}
#endif

	/* XXX The gpu is faster to perform detiling in bulk, but takes
	 * longer to setup and retrieve the results, with an additional
	 * copy. The long term solution is to use snoopable bo and avoid
	 * this path.
	 */

	if (download_inplace(kgem, dst, src_bo, box, nbox)) {
fallback:
		read_boxes_inplace(kgem, dst, src_bo, box, nbox);
		return;
	}

	can_blt = kgem_bo_can_blt(kgem, src_bo) &&
		(box[0].x2 - box[0].x1) * dst->drawable.bitsPerPixel < 8 * (MAXSHORT - 4);
	extents = box[0];
	for (n = 1; n < nbox; n++) {
		if (box[n].x1 < extents.x1)
			extents.x1 = box[n].x1;
		if (box[n].x2 > extents.x2)
			extents.x2 = box[n].x2;

		if (can_blt)
			can_blt = (box[n].x2 - box[n].x1) * dst->drawable.bitsPerPixel < 8 * (MAXSHORT - 4);

		if (box[n].y1 < extents.y1)
			extents.y1 = box[n].y1;
		if (box[n].y2 > extents.y2)
			extents.y2 = box[n].y2;
	}
	if (kgem_bo_can_map(kgem, src_bo)) {
		/* Is it worth detiling? */
		if ((extents.y2 - extents.y1 - 1) * src_bo->pitch < 4096)
			goto fallback;
	}

	/* Try to avoid switching rings... */
	if (!can_blt || kgem->ring == KGEM_RENDER ||
	    upload_too_large(sna, extents.x2 - extents.x1, extents.y2 - extents.y1)) {
		DrawableRec tmp;

		tmp.width  = extents.x2 - extents.x1;
		tmp.height = extents.y2 - extents.y1;
		tmp.depth  = dst->drawable.depth;
		tmp.bitsPerPixel = dst->drawable.bitsPerPixel;

		assert(tmp.width);
		assert(tmp.height);

		if (must_tile(sna, tmp.width, tmp.height)) {
			BoxRec tile, stack[64], *clipped, *c;
			int step;

			if (n > ARRAY_SIZE(stack)) {
				clipped = malloc(sizeof(BoxRec) * n);
				if (clipped == NULL)
					goto fallback;
			} else
				clipped = stack;

			step = MIN(sna->render.max_3d_size,
				   8*(MAXSHORT&~63) / dst->drawable.bitsPerPixel);
			while (step * step * 4 > sna->kgem.max_upload_tile_size)
				step /= 2;

			DBG(("%s: tiling download, using %dx%d tiles\n",
			     __FUNCTION__, step, step));
			assert(step);

			for (tile.y1 = extents.y1; tile.y1 < extents.y2; tile.y1 = tile.y2) {
				int y2 = tile.y1 + step;
				if (y2 > extents.y2)
					y2 = extents.y2;
				tile.y2 = y2;

				for (tile.x1 = extents.x1; tile.x1 < extents.x2; tile.x1 = tile.x2) {
					int x2 = tile.x1 + step;
					if (x2 > extents.x2)
						x2 = extents.x2;
					tile.x2 = x2;

					tmp.width  = tile.x2 - tile.x1;
					tmp.height = tile.y2 - tile.y1;

					c = clipped;
					for (n = 0; n < nbox; n++) {
						*c = box[n];
						if (!box_intersect(c, &tile))
							continue;

						DBG(("%s: box(%d, %d), (%d, %d),, dst=(%d, %d)\n",
						     __FUNCTION__,
						     c->x1, c->y1,
						     c->x2, c->y2,
						     c->x1 - tile.x1,
						     c->y1 - tile.y1));
						c++;
					}
					if (c == clipped)
						continue;

					dst_bo = kgem_create_buffer_2d(kgem,
								       tmp.width,
								       tmp.height,
								       tmp.bitsPerPixel,
								       KGEM_BUFFER_LAST,
								       &ptr);
					if (!dst_bo) {
						if (clipped != stack)
							free(clipped);
						goto fallback;
					}

					if (!sna->render.copy_boxes(sna, GXcopy,
								    &dst->drawable, src_bo, 0, 0,
								    &tmp, dst_bo, -tile.x1, -tile.y1,
								    clipped, c-clipped, COPY_LAST)) {
						kgem_bo_destroy(&sna->kgem, dst_bo);
						if (clipped != stack)
							free(clipped);
						goto fallback;
					}

					kgem_bo_submit(&sna->kgem, dst_bo);
					kgem_buffer_read_sync(kgem, dst_bo);

					if (sigtrap_get() == 0) {
						while (c-- != clipped) {
							memcpy_blt(ptr, dst->devPrivate.ptr, tmp.bitsPerPixel,
								   dst_bo->pitch, dst->devKind,
								   c->x1 - tile.x1,
								   c->y1 - tile.y1,
								   c->x1, c->y1,
								   c->x2 - c->x1,
								   c->y2 - c->y1);
						}
						sigtrap_put();
					}

					kgem_bo_destroy(&sna->kgem, dst_bo);
				}
			}

			if (clipped != stack)
				free(clipped);
		} else {
			dst_bo = kgem_create_buffer_2d(kgem,
						       tmp.width,
						       tmp.height,
						       tmp.bitsPerPixel,
						       KGEM_BUFFER_LAST,
						       &ptr);
			if (!dst_bo)
				goto fallback;

			if (!sna->render.copy_boxes(sna, GXcopy,
						    &dst->drawable, src_bo, 0, 0,
						    &tmp, dst_bo, -extents.x1, -extents.y1,
						    box, nbox, COPY_LAST)) {
				kgem_bo_destroy(&sna->kgem, dst_bo);
				goto fallback;
			}

			kgem_bo_submit(&sna->kgem, dst_bo);
			kgem_buffer_read_sync(kgem, dst_bo);

			if (sigtrap_get() == 0) {
				for (n = 0; n < nbox; n++) {
					memcpy_blt(ptr, dst->devPrivate.ptr, tmp.bitsPerPixel,
						   dst_bo->pitch, dst->devKind,
						   box[n].x1 - extents.x1,
						   box[n].y1 - extents.y1,
						   box[n].x1, box[n].y1,
						   box[n].x2 - box[n].x1,
						   box[n].y2 - box[n].y1);
				}
				sigtrap_put();
			}

			kgem_bo_destroy(&sna->kgem, dst_bo);
		}
		return;
	}

	/* count the total number of bytes to be read and allocate a bo */
	cpp = dst->drawable.bitsPerPixel / 8;
	offset = 0;
	for (n = 0; n < nbox; n++) {
		int height = box[n].y2 - box[n].y1;
		int width = box[n].x2 - box[n].x1;
		offset += PITCH(width, cpp) * height;
	}

	DBG(("    read buffer size=%d\n", offset));

	dst_bo = kgem_create_buffer(kgem, offset, KGEM_BUFFER_LAST, &ptr);
	if (!dst_bo) {
		read_boxes_inplace(kgem, dst, src_bo, box, nbox);
		return;
	}

	cmd = XY_SRC_COPY_BLT_CMD;
	src_pitch = src_bo->pitch;
	if (kgem->gen >= 040 && src_bo->tiling) {
		cmd |= BLT_SRC_TILED;
		src_pitch >>= 2;
	}

	br13 = 0xcc << 16;
	switch (cpp) {
	default:
	case 4: cmd |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
		br13 |= 1 << 25; /* RGB8888 */
	case 2: br13 |= 1 << 24; /* RGB565 */
	case 1: break;
	}

	kgem_set_mode(kgem, KGEM_BLT, dst_bo);
	if (!kgem_check_batch(kgem, 10) ||
	    !kgem_check_reloc_and_exec(kgem, 2) ||
	    !kgem_check_many_bo_fenced(kgem, dst_bo, src_bo, NULL)) {
		kgem_submit(kgem);
		if (!kgem_check_many_bo_fenced(kgem, dst_bo, src_bo, NULL))
			goto fallback;
		_kgem_set_mode(kgem, KGEM_BLT);
	}

	tmp_nbox = nbox;
	tmp_box = box;
	offset = 0;
	if (sna->kgem.gen >= 0100) {
		cmd |= 8;
		do {
			int nbox_this_time;

			nbox_this_time = tmp_nbox;
			if (10*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
				nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
			if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
				nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc) / 2;
			assert(nbox_this_time);
			tmp_nbox -= nbox_this_time;

			assert(kgem->mode == KGEM_BLT);
			for (n = 0; n < nbox_this_time; n++) {
				int height = tmp_box[n].y2 - tmp_box[n].y1;
				int width = tmp_box[n].x2 - tmp_box[n].x1;
				int pitch = PITCH(width, cpp);
				uint32_t *b = kgem->batch + kgem->nbatch;

				DBG(("    blt offset %x: (%d, %d) x (%d, %d), pitch=%d\n",
				     offset,
				     tmp_box[n].x1, tmp_box[n].y1,
				     width, height, pitch));

				assert(tmp_box[n].x1 >= 0);
				assert(tmp_box[n].x2 * dst->drawable.bitsPerPixel/8 <= src_bo->pitch);
				assert(tmp_box[n].y1 >= 0);
				assert(tmp_box[n].y2 * src_bo->pitch <= kgem_bo_size(src_bo));

				b[0] = cmd;
				b[1] = br13 | pitch;
				b[2] = 0;
				b[3] = height << 16 | width;
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(kgem, kgem->nbatch + 4, dst_bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 I915_GEM_DOMAIN_RENDER |
							 KGEM_RELOC_FENCED,
							 offset);
				b[6] = tmp_box[n].y1 << 16 | tmp_box[n].x1;
				b[7] = src_pitch;
				*(uint64_t *)(b+8) =
					kgem_add_reloc64(kgem, kgem->nbatch + 8, src_bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 KGEM_RELOC_FENCED,
							 0);
				kgem->nbatch += 10;

				offset += pitch * height;
			}

			_kgem_submit(kgem);
			if (!tmp_nbox)
				break;

			_kgem_set_mode(kgem, KGEM_BLT);
			tmp_box += nbox_this_time;
		} while (1);
	} else {
		cmd |= 6;
		do {
			int nbox_this_time;

			nbox_this_time = tmp_nbox;
			if (8*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
				nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
			if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
				nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc) / 2;
			assert(nbox_this_time);
			tmp_nbox -= nbox_this_time;

			assert(kgem->mode == KGEM_BLT);
			for (n = 0; n < nbox_this_time; n++) {
				int height = tmp_box[n].y2 - tmp_box[n].y1;
				int width = tmp_box[n].x2 - tmp_box[n].x1;
				int pitch = PITCH(width, cpp);
				uint32_t *b = kgem->batch + kgem->nbatch;

				DBG(("    blt offset %x: (%d, %d) x (%d, %d), pitch=%d\n",
				     offset,
				     tmp_box[n].x1, tmp_box[n].y1,
				     width, height, pitch));

				assert(tmp_box[n].x1 >= 0);
				assert(tmp_box[n].x2 * dst->drawable.bitsPerPixel/8 <= src_bo->pitch);
				assert(tmp_box[n].y1 >= 0);
				assert(tmp_box[n].y2 * src_bo->pitch <= kgem_bo_size(src_bo));

				b[0] = cmd;
				b[1] = br13 | pitch;
				b[2] = 0;
				b[3] = height << 16 | width;
				b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, dst_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      offset);
				b[5] = tmp_box[n].y1 << 16 | tmp_box[n].x1;
				b[6] = src_pitch;
				b[7] = kgem_add_reloc(kgem, kgem->nbatch + 7, src_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      KGEM_RELOC_FENCED,
						      0);
				kgem->nbatch += 8;

				offset += pitch * height;
			}

			_kgem_submit(kgem);
			if (!tmp_nbox)
				break;

			_kgem_set_mode(kgem, KGEM_BLT);
			tmp_box += nbox_this_time;
		} while (1);
	}
	assert(offset == __kgem_buffer_size(dst_bo));

	kgem_buffer_read_sync(kgem, dst_bo);

	if (sigtrap_get() == 0) {
		char *src = ptr;
		do {
			int height = box->y2 - box->y1;
			int width  = box->x2 - box->x1;
			int pitch = PITCH(width, cpp);

			DBG(("    copy offset %lx [%08x...%08x...%08x]: (%d, %d) x (%d, %d), src pitch=%d, dst pitch=%d, bpp=%d\n",
			     (long)((char *)src - (char *)ptr),
			     *(uint32_t*)src, *(uint32_t*)(src+pitch*height/2 + pitch/2 - 4), *(uint32_t*)(src+pitch*height - 4),
			     box->x1, box->y1,
			     width, height,
			     pitch, dst->devKind, cpp*8));

			assert(box->x1 >= 0);
			assert(box->x2 <= dst->drawable.width);
			assert(box->y1 >= 0);
			assert(box->y2 <= dst->drawable.height);

			memcpy_blt(src, dst->devPrivate.ptr, cpp*8,
				   pitch, dst->devKind,
				   0, 0,
				   box->x1, box->y1,
				   width, height);
			box++;

			src += pitch * height;
		} while (--nbox);
		assert(src - (char *)ptr == __kgem_buffer_size(dst_bo));
		sigtrap_put();
	}
	kgem_bo_destroy(kgem, dst_bo);
	sna->blt_state.fill_bo = 0;
}

static bool upload_inplace__tiled(struct kgem *kgem, struct kgem_bo *bo)
{
	DBG(("%s: tiling=%d\n", __FUNCTION__, bo->tiling));
	switch (bo->tiling) {
	case I915_TILING_Y:
		return false;
	case I915_TILING_X:
		if (!kgem->memcpy_to_tiled_x)
			return false;
	default:
		break;
	}

	return kgem_bo_can_map__cpu(kgem, bo, true);
}

static bool
write_boxes_inplace__tiled(struct kgem *kgem,
                           const uint8_t *src, int stride, int bpp, int16_t src_dx, int16_t src_dy,
                           struct kgem_bo *bo, int16_t dst_dx, int16_t dst_dy,
                           const BoxRec *box, int n)
{
	uint8_t *dst;

	assert(kgem_bo_can_map__cpu(kgem, bo, true));
	assert(bo->tiling != I915_TILING_Y);

	dst = kgem_bo_map__cpu(kgem, bo);
	if (dst == NULL)
		return false;

	kgem_bo_sync__cpu(kgem, bo);

	if (sigtrap_get())
		return false;

	if (bo->tiling) {
		do {
			memcpy_to_tiled_x(kgem, src, dst, bpp, stride, bo->pitch,
					  box->x1 + src_dx, box->y1 + src_dy,
					  box->x1 + dst_dx, box->y1 + dst_dy,
					  box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);
	} else {
		do {
			memcpy_blt(src, dst, bpp, stride, bo->pitch,
				   box->x1 + src_dx, box->y1 + src_dy,
				   box->x1 + dst_dx, box->y1 + dst_dy,
				   box->x2 - box->x1, box->y2 - box->y1);
			box++;
		} while (--n);
	}

	sigtrap_put();
	return true;
}

static bool write_boxes_inplace(struct kgem *kgem,
				const void *src, int stride, int bpp, int16_t src_dx, int16_t src_dy,
				struct kgem_bo *bo, int16_t dst_dx, int16_t dst_dy,
				const BoxRec *box, int n)
{
	void *dst;

	DBG(("%s x %d, handle=%d, tiling=%d\n",
	     __FUNCTION__, n, bo->handle, bo->tiling));

	if (upload_inplace__tiled(kgem, bo) &&
	    write_boxes_inplace__tiled(kgem, src, stride, bpp, src_dx, src_dy,
				       bo, dst_dx, dst_dy, box, n))
		return true;

	if (!kgem_bo_can_map(kgem, bo))
		return false;

	kgem_bo_submit(kgem, bo);

	dst = kgem_bo_map(kgem, bo);
	if (dst == NULL)
		return false;

	assert(dst != src);

	if (sigtrap_get())
		return false;

	do {
		DBG(("%s: (%d, %d) -> (%d, %d) x (%d, %d) [bpp=%d, src_pitch=%d, dst_pitch=%d]\n", __FUNCTION__,
		     box->x1 + src_dx, box->y1 + src_dy,
		     box->x1 + dst_dx, box->y1 + dst_dy,
		     box->x2 - box->x1, box->y2 - box->y1,
		     bpp, stride, bo->pitch));

		assert(box->x2 > box->x1);
		assert(box->y2 > box->y1);

		assert(box->x1 + dst_dx >= 0);
		assert((box->x2 + dst_dx)*bpp <= 8*bo->pitch);
		assert(box->y1 + dst_dy >= 0);
		assert((box->y2 + dst_dy)*bo->pitch <= kgem_bo_size(bo));

		assert(box->x1 + src_dx >= 0);
		assert((box->x2 + src_dx)*bpp <= 8*stride);
		assert(box->y1 + src_dy >= 0);

		memcpy_blt(src, dst, bpp,
			   stride, bo->pitch,
			   box->x1 + src_dx, box->y1 + src_dy,
			   box->x1 + dst_dx, box->y1 + dst_dy,
			   box->x2 - box->x1, box->y2 - box->y1);
		box++;
	} while (--n);

	sigtrap_put();
	return true;
}

static bool __upload_inplace(struct kgem *kgem,
			     struct kgem_bo *bo,
			     const BoxRec *box,
			     int n, int bpp)
{
	unsigned int bytes;

	if (FORCE_INPLACE)
		return FORCE_INPLACE > 0;

	/* If we are writing through the GTT, check first if we might be
	 * able to almagamate a series of small writes into a single
	 * operation.
	 */
	bytes = 0;
	while (n--) {
		bytes += (box->x2 - box->x1) * (box->y2 - box->y1);
		box++;
	}
	if (__kgem_bo_is_busy(kgem, bo))
		return bytes * bpp >> 12 >= kgem->half_cpu_cache_pages;
	else
		return bytes * bpp >> 12;
}

static bool upload_inplace(struct kgem *kgem,
			   struct kgem_bo *bo,
			   const BoxRec *box,
			   int n, int bpp)
{
	if (unlikely(kgem->wedged))
		return true;

	if (!kgem_bo_can_map(kgem, bo) && !upload_inplace__tiled(kgem, bo))
		return false;

	return __upload_inplace(kgem, bo, box, n,bpp);
}

bool sna_write_boxes(struct sna *sna, PixmapPtr dst,
		     struct kgem_bo * const dst_bo, int16_t const dst_dx, int16_t const dst_dy,
		     const void * const src, int const stride, int16_t const src_dx, int16_t const src_dy,
		     const BoxRec *box, int nbox)
{
	struct kgem *kgem = &sna->kgem;
	struct kgem_bo *src_bo;
	BoxRec extents;
	void *ptr;
	int offset;
	int n, cmd, br13;
	bool can_blt;

	DBG(("%s x %d, src stride=%d,  src dx=(%d, %d)\n", __FUNCTION__, nbox, stride, src_dx, src_dy));

	if (upload_inplace(kgem, dst_bo, box, nbox, dst->drawable.bitsPerPixel) &&
	    write_boxes_inplace(kgem,
				src, stride, dst->drawable.bitsPerPixel, src_dx, src_dy,
				dst_bo, dst_dx, dst_dy,
				box, nbox))
		return true;

	if (wedged(sna))
		return false;

	can_blt = kgem_bo_can_blt(kgem, dst_bo) &&
		(box[0].x2 - box[0].x1) * dst->drawable.bitsPerPixel < 8 * (MAXSHORT - 4);
	extents = box[0];
	for (n = 1; n < nbox; n++) {
		if (box[n].x1 < extents.x1)
			extents.x1 = box[n].x1;
		if (box[n].x2 > extents.x2)
			extents.x2 = box[n].x2;

		if (can_blt)
			can_blt = (box[n].x2 - box[n].x1) * dst->drawable.bitsPerPixel < 8 * (MAXSHORT - 4);

		if (box[n].y1 < extents.y1)
			extents.y1 = box[n].y1;
		if (box[n].y2 > extents.y2)
			extents.y2 = box[n].y2;
	}

	/* Try to avoid switching rings... */
	if (!can_blt || kgem->ring == KGEM_RENDER ||
	    upload_too_large(sna, extents.x2 - extents.x1, extents.y2 - extents.y1)) {
		DrawableRec tmp;

		tmp.width  = extents.x2 - extents.x1;
		tmp.height = extents.y2 - extents.y1;
		tmp.depth  = dst->drawable.depth;
		tmp.bitsPerPixel = dst->drawable.bitsPerPixel;

		assert(tmp.width);
		assert(tmp.height);

		DBG(("%s: upload (%d, %d)x(%d, %d), max %dx%d\n",
		     __FUNCTION__,
		     extents.x1, extents.y1,
		     tmp.width, tmp.height,
		     sna->render.max_3d_size, sna->render.max_3d_size));
		if (must_tile(sna, tmp.width, tmp.height)) {
			BoxRec tile, stack[64], *clipped;
			int cpp, step;

tile:
			cpp = dst->drawable.bitsPerPixel / 8;
			step = MIN(sna->render.max_3d_size,
				   (MAXSHORT&~63) / cpp);
			while (step * step * cpp > sna->kgem.max_upload_tile_size)
				step /= 2;

			if (step * cpp > 4096)
				step = 4096 / cpp;
			assert(step);

			DBG(("%s: tiling upload, using %dx%d tiles\n",
			     __FUNCTION__, step, step));

			if (n > ARRAY_SIZE(stack)) {
				clipped = malloc(sizeof(BoxRec) * n);
				if (clipped == NULL)
					goto fallback;
			} else
				clipped = stack;

			for (tile.y1 = extents.y1; tile.y1 < extents.y2; tile.y1 = tile.y2) {
				int y2 = tile.y1 + step;
				if (y2 > extents.y2)
					y2 = extents.y2;
				tile.y2 = y2;

				for (tile.x1 = extents.x1; tile.x1 < extents.x2; tile.x1 = tile.x2) {
					int x2 = tile.x1 + step;
					if (x2 > extents.x2)
						x2 = extents.x2;
					tile.x2 = x2;

					tmp.width  = tile.x2 - tile.x1;
					tmp.height = tile.y2 - tile.y1;

					src_bo = kgem_create_buffer_2d(kgem,
								       tmp.width,
								       tmp.height,
								       tmp.bitsPerPixel,
								       KGEM_BUFFER_WRITE_INPLACE,
								       &ptr);
					if (!src_bo) {
						if (clipped != stack)
							free(clipped);
						goto fallback;
					}

					if (sigtrap_get() == 0) {
						BoxRec *c = clipped;
						for (n = 0; n < nbox; n++) {
							*c = box[n];
							if (!box_intersect(c, &tile))
								continue;

							DBG(("%s: box(%d, %d), (%d, %d), src=(%d, %d), dst=(%d, %d)\n",
							     __FUNCTION__,
							     c->x1, c->y1,
							     c->x2, c->y2,
							     src_dx, src_dy,
							     c->x1 - tile.x1,
							     c->y1 - tile.y1));
							memcpy_blt(src, ptr, tmp.bitsPerPixel,
								   stride, src_bo->pitch,
								   c->x1 + src_dx,
								   c->y1 + src_dy,
								   c->x1 - tile.x1,
								   c->y1 - tile.y1,
								   c->x2 - c->x1,
								   c->y2 - c->y1);
							c++;
						}

						if (c != clipped)
							n = sna->render.copy_boxes(sna, GXcopy,
										   &tmp, src_bo, -tile.x1, -tile.y1,
										   &dst->drawable, dst_bo, dst_dx, dst_dy,
										   clipped, c - clipped, 0);
						else
							n = 1;
						sigtrap_put();
					} else
						n = 0;

					kgem_bo_destroy(&sna->kgem, src_bo);

					if (!n) {
						if (clipped != stack)
							free(clipped);
						goto fallback;
					}
				}
			}

			if (clipped != stack)
				free(clipped);
		} else {
			src_bo = kgem_create_buffer_2d(kgem,
						       tmp.width,
						       tmp.height,
						       tmp.bitsPerPixel,
						       KGEM_BUFFER_WRITE_INPLACE,
						       &ptr);
			if (!src_bo)
				goto fallback;

			if (sigtrap_get() == 0) {
				for (n = 0; n < nbox; n++) {
					DBG(("%s: box(%d, %d), (%d, %d), src=(%d, %d), dst=(%d, %d)\n",
					     __FUNCTION__,
					     box[n].x1, box[n].y1,
					     box[n].x2, box[n].y2,
					     src_dx, src_dy,
					     box[n].x1 - extents.x1,
					     box[n].y1 - extents.y1));
					memcpy_blt(src, ptr, tmp.bitsPerPixel,
						   stride, src_bo->pitch,
						   box[n].x1 + src_dx,
						   box[n].y1 + src_dy,
						   box[n].x1 - extents.x1,
						   box[n].y1 - extents.y1,
						   box[n].x2 - box[n].x1,
						   box[n].y2 - box[n].y1);
				}

				n = sna->render.copy_boxes(sna, GXcopy,
							   &tmp, src_bo, -extents.x1, -extents.y1,
							   &dst->drawable, dst_bo, dst_dx, dst_dy,
							   box, nbox, 0);
				sigtrap_put();
			} else
				n = 0;

			kgem_bo_destroy(&sna->kgem, src_bo);

			if (!n)
				goto tile;
		}

		return true;
	}

	cmd = XY_SRC_COPY_BLT_CMD;
	br13 = dst_bo->pitch;
	if (kgem->gen >= 040 && dst_bo->tiling) {
		cmd |= BLT_DST_TILED;
		br13 >>= 2;
	}
	br13 |= 0xcc << 16;
	switch (dst->drawable.bitsPerPixel) {
	default:
	case 32: cmd |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
		 br13 |= 1 << 25; /* RGB8888 */
	case 16: br13 |= 1 << 24; /* RGB565 */
	case 8: break;
	}

	kgem_set_mode(kgem, KGEM_BLT, dst_bo);
	if (!kgem_check_batch(kgem, 10) ||
	    !kgem_check_reloc_and_exec(kgem, 2) ||
	    !kgem_check_bo_fenced(kgem, dst_bo)) {
		kgem_submit(kgem);
		if (!kgem_check_bo_fenced(kgem, dst_bo))
			goto fallback;
		_kgem_set_mode(kgem, KGEM_BLT);
	}

	if (kgem->gen >= 0100) {
		cmd |= 8;
		do {
			int nbox_this_time;

			nbox_this_time = nbox;
			if (10*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
				nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
			if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
				nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc) / 2;
			assert(nbox_this_time);
			nbox -= nbox_this_time;

			/* Count the total number of bytes to be read and allocate a
			 * single buffer large enough. Or if it is very small, combine
			 * with other allocations. */
			offset = 0;
			for (n = 0; n < nbox_this_time; n++) {
				int height = box[n].y2 - box[n].y1;
				int width = box[n].x2 - box[n].x1;
				offset += PITCH(width, dst->drawable.bitsPerPixel >> 3) * height;
			}

			src_bo = kgem_create_buffer(kgem, offset,
						    KGEM_BUFFER_WRITE_INPLACE | (nbox ? KGEM_BUFFER_LAST : 0),
						    &ptr);
			if (!src_bo)
				break;

			if (sigtrap_get() == 0) {
				offset = 0;
				do {
					int height = box->y2 - box->y1;
					int width = box->x2 - box->x1;
					int pitch = PITCH(width, dst->drawable.bitsPerPixel >> 3);
					uint32_t *b;

					DBG(("  %s: box src=(%d, %d), dst=(%d, %d) size=(%d, %d), dst offset=%d, dst pitch=%d\n",
					     __FUNCTION__,
					     box->x1 + src_dx, box->y1 + src_dy,
					     box->x1 + dst_dx, box->y1 + dst_dy,
					     width, height,
					     offset, pitch));

					assert(box->x1 + src_dx >= 0);
					assert((box->x2 + src_dx)*dst->drawable.bitsPerPixel <= 8*stride);
					assert(box->y1 + src_dy >= 0);

					assert(box->x1 + dst_dx >= 0);
					assert(box->y1 + dst_dy >= 0);

					memcpy_blt(src, (char *)ptr + offset,
						   dst->drawable.bitsPerPixel,
						   stride, pitch,
						   box->x1 + src_dx, box->y1 + src_dy,
						   0, 0,
						   width, height);

					assert(kgem->mode == KGEM_BLT);
					b = kgem->batch + kgem->nbatch;
					b[0] = cmd;
					b[1] = br13;
					b[2] = (box->y1 + dst_dy) << 16 | (box->x1 + dst_dx);
					b[3] = (box->y2 + dst_dy) << 16 | (box->x2 + dst_dx);
					*(uint64_t *)(b+4) =
						kgem_add_reloc64(kgem, kgem->nbatch + 4, dst_bo,
								 I915_GEM_DOMAIN_RENDER << 16 |
								 I915_GEM_DOMAIN_RENDER |
								 KGEM_RELOC_FENCED,
								 0);
					b[6] = 0;
					b[7] = pitch;
					*(uint64_t *)(b+8) =
						kgem_add_reloc64(kgem, kgem->nbatch + 8, src_bo,
								 I915_GEM_DOMAIN_RENDER << 16 |
								 KGEM_RELOC_FENCED,
								 offset);
					kgem->nbatch += 10;

					box++;
					offset += pitch * height;
				} while (--nbox_this_time);
				assert(offset == __kgem_buffer_size(src_bo));
				sigtrap_put();
			}

			if (nbox) {
				_kgem_submit(kgem);
				_kgem_set_mode(kgem, KGEM_BLT);
			}

			kgem_bo_destroy(kgem, src_bo);
		} while (nbox);
	} else {
		cmd |= 6;
		do {
			int nbox_this_time;

			nbox_this_time = nbox;
			if (8*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
				nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
			if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
				nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc) / 2;
			assert(nbox_this_time);
			nbox -= nbox_this_time;

			/* Count the total number of bytes to be read and allocate a
			 * single buffer large enough. Or if it is very small, combine
			 * with other allocations. */
			offset = 0;
			for (n = 0; n < nbox_this_time; n++) {
				int height = box[n].y2 - box[n].y1;
				int width = box[n].x2 - box[n].x1;
				offset += PITCH(width, dst->drawable.bitsPerPixel >> 3) * height;
			}

			src_bo = kgem_create_buffer(kgem, offset,
						    KGEM_BUFFER_WRITE_INPLACE | (nbox ? KGEM_BUFFER_LAST : 0),
						    &ptr);
			if (!src_bo)
				break;

			if (sigtrap_get()) {
				kgem_bo_destroy(kgem, src_bo);
				goto fallback;
			}

			offset = 0;
			do {
				int height = box->y2 - box->y1;
				int width = box->x2 - box->x1;
				int pitch = PITCH(width, dst->drawable.bitsPerPixel >> 3);
				uint32_t *b;

				DBG(("  %s: box src=(%d, %d), dst=(%d, %d) size=(%d, %d), dst offset=%d, dst pitch=%d\n",
				     __FUNCTION__,
				     box->x1 + src_dx, box->y1 + src_dy,
				     box->x1 + dst_dx, box->y1 + dst_dy,
				     width, height,
				     offset, pitch));

				assert(box->x1 + src_dx >= 0);
				assert((box->x2 + src_dx)*dst->drawable.bitsPerPixel <= 8*stride);
				assert(box->y1 + src_dy >= 0);

				assert(box->x1 + dst_dx >= 0);
				assert(box->y1 + dst_dy >= 0);

				memcpy_blt(src, (char *)ptr + offset,
					   dst->drawable.bitsPerPixel,
					   stride, pitch,
					   box->x1 + src_dx, box->y1 + src_dy,
					   0, 0,
					   width, height);

				assert(kgem->mode == KGEM_BLT);
				b = kgem->batch + kgem->nbatch;
				b[0] = cmd;
				b[1] = br13;
				b[2] = (box->y1 + dst_dy) << 16 | (box->x1 + dst_dx);
				b[3] = (box->y2 + dst_dy) << 16 | (box->x2 + dst_dx);
				b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, dst_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = 0;
				b[6] = pitch;
				b[7] = kgem_add_reloc(kgem, kgem->nbatch + 7, src_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      KGEM_RELOC_FENCED,
						      offset);
				kgem->nbatch += 8;

				box++;
				offset += pitch * height;
			} while (--nbox_this_time);
			assert(offset == __kgem_buffer_size(src_bo));
			sigtrap_put();

			if (nbox) {
				_kgem_submit(kgem);
				_kgem_set_mode(kgem, KGEM_BLT);
			}

			kgem_bo_destroy(kgem, src_bo);
		} while (nbox);
	}

	sna->blt_state.fill_bo = 0;
	return true;

fallback:
	return write_boxes_inplace(kgem,
				   src, stride, dst->drawable.bitsPerPixel, src_dx, src_dy,
				   dst_bo, dst_dx, dst_dy,
				   box, nbox);
}

static bool
write_boxes_inplace__xor(struct kgem *kgem,
			 const void *src, int stride, int bpp, int16_t src_dx, int16_t src_dy,
			 struct kgem_bo *bo, int16_t dst_dx, int16_t dst_dy,
			 const BoxRec *box, int n,
			 uint32_t and, uint32_t or)
{
	void *dst;

	DBG(("%s x %d, tiling=%d\n", __FUNCTION__, n, bo->tiling));

	if (!kgem_bo_can_map(kgem, bo))
		return false;

	kgem_bo_submit(kgem, bo);

	dst = kgem_bo_map(kgem, bo);
	if (dst == NULL)
		return false;

	if (sigtrap_get())
		return false;

	do {
		DBG(("%s: (%d, %d) -> (%d, %d) x (%d, %d) [bpp=%d, src_pitch=%d, dst_pitch=%d]\n", __FUNCTION__,
		     box->x1 + src_dx, box->y1 + src_dy,
		     box->x1 + dst_dx, box->y1 + dst_dy,
		     box->x2 - box->x1, box->y2 - box->y1,
		     bpp, stride, bo->pitch));

		assert(box->x2 > box->x1);
		assert(box->y2 > box->y1);

		assert(box->x1 + dst_dx >= 0);
		assert((box->x2 + dst_dx)*bpp <= 8*bo->pitch);
		assert(box->y1 + dst_dy >= 0);
		assert((box->y2 + dst_dy)*bo->pitch <= kgem_bo_size(bo));

		assert(box->x1 + src_dx >= 0);
		assert((box->x2 + src_dx)*bpp <= 8*stride);
		assert(box->y1 + src_dy >= 0);

		memcpy_xor(src, dst, bpp,
			   stride, bo->pitch,
			   box->x1 + src_dx, box->y1 + src_dy,
			   box->x1 + dst_dx, box->y1 + dst_dy,
			   box->x2 - box->x1, box->y2 - box->y1,
			   and, or);
		box++;
	} while (--n);

	sigtrap_put();
	return true;
}

static bool upload_inplace__xor(struct kgem *kgem,
				struct kgem_bo *bo,
				const BoxRec *box,
				int n, int bpp)
{
	if (unlikely(kgem->wedged))
		return true;

	if (!kgem_bo_can_map(kgem, bo))
		return false;

	return __upload_inplace(kgem, bo, box, n, bpp);
}

bool sna_write_boxes__xor(struct sna *sna, PixmapPtr dst,
			  struct kgem_bo *dst_bo, int16_t dst_dx, int16_t dst_dy,
			  const void *src, int stride, int16_t src_dx, int16_t src_dy,
			  const BoxRec *box, int nbox,
			  uint32_t and, uint32_t or)
{
	struct kgem *kgem = &sna->kgem;
	struct kgem_bo *src_bo;
	BoxRec extents;
	bool can_blt;
	void *ptr;
	int offset;
	int n, cmd, br13;

	DBG(("%s x %d\n", __FUNCTION__, nbox));

	if (upload_inplace__xor(kgem, dst_bo, box, nbox, dst->drawable.bitsPerPixel) &&
	    write_boxes_inplace__xor(kgem,
				     src, stride, dst->drawable.bitsPerPixel, src_dx, src_dy,
				     dst_bo, dst_dx, dst_dy,
				     box, nbox,
				     and, or))
		return true;

	if (wedged(sna))
		return false;

	can_blt = kgem_bo_can_blt(kgem, dst_bo) &&
		(box[0].x2 - box[0].x1) * dst->drawable.bitsPerPixel < 8 * (MAXSHORT - 4);
	extents = box[0];
	for (n = 1; n < nbox; n++) {
		if (box[n].x1 < extents.x1)
			extents.x1 = box[n].x1;
		if (box[n].x2 > extents.x2)
			extents.x2 = box[n].x2;

		if (can_blt)
			can_blt = (box[n].x2 - box[n].x1) * dst->drawable.bitsPerPixel < 8 * (MAXSHORT - 4);

		if (box[n].y1 < extents.y1)
			extents.y1 = box[n].y1;
		if (box[n].y2 > extents.y2)
			extents.y2 = box[n].y2;
	}

	/* Try to avoid switching rings... */
	if (!can_blt || kgem->ring == KGEM_RENDER ||
	    upload_too_large(sna, extents.x2 - extents.x1, extents.y2 - extents.y1)) {
		DrawableRec tmp;

		tmp.width  = extents.x2 - extents.x1;
		tmp.height = extents.y2 - extents.y1;
		tmp.depth  = dst->drawable.depth;
		tmp.bitsPerPixel = dst->drawable.bitsPerPixel;

		assert(tmp.width);
		assert(tmp.height);

		DBG(("%s: upload (%d, %d)x(%d, %d), max %dx%d\n",
		     __FUNCTION__,
		     extents.x1, extents.y1,
		     tmp.width, tmp.height,
		     sna->render.max_3d_size, sna->render.max_3d_size));
		if (must_tile(sna, tmp.width, tmp.height)) {
			BoxRec tile, stack[64], *clipped;
			int step;

tile:
			step = MIN(sna->render.max_3d_size - 4096 / dst->drawable.bitsPerPixel,
				   8*(MAXSHORT&~63) / dst->drawable.bitsPerPixel);
			while (step * step * 4 > sna->kgem.max_upload_tile_size)
				step /= 2;

			DBG(("%s: tiling upload, using %dx%d tiles\n",
			     __FUNCTION__, step, step));
			assert(step);

			if (n > ARRAY_SIZE(stack)) {
				clipped = malloc(sizeof(BoxRec) * n);
				if (clipped == NULL)
					goto fallback;
			} else
				clipped = stack;

			for (tile.y1 = extents.y1; tile.y1 < extents.y2; tile.y1 = tile.y2) {
				int y2 = tile.y1 + step;
				if (y2 > extents.y2)
					y2 = extents.y2;
				tile.y2 = y2;

				for (tile.x1 = extents.x1; tile.x1 < extents.x2; tile.x1 = tile.x2) {
					int x2 = tile.x1 + step;
					if (x2 > extents.x2)
						x2 = extents.x2;
					tile.x2 = x2;

					tmp.width  = tile.x2 - tile.x1;
					tmp.height = tile.y2 - tile.y1;

					src_bo = kgem_create_buffer_2d(kgem,
								       tmp.width,
								       tmp.height,
								       tmp.bitsPerPixel,
								       KGEM_BUFFER_WRITE_INPLACE,
								       &ptr);
					if (!src_bo) {
						if (clipped != stack)
							free(clipped);
						goto fallback;
					}

					if (sigtrap_get() == 0) {
						BoxRec *c = clipped;
						for (n = 0; n < nbox; n++) {
							*c = box[n];
							if (!box_intersect(c, &tile))
								continue;

							DBG(("%s: box(%d, %d), (%d, %d), src=(%d, %d), dst=(%d, %d)\n",
							     __FUNCTION__,
							     c->x1, c->y1,
							     c->x2, c->y2,
							     src_dx, src_dy,
							     c->x1 - tile.x1,
							     c->y1 - tile.y1));
							memcpy_xor(src, ptr, tmp.bitsPerPixel,
								   stride, src_bo->pitch,
								   c->x1 + src_dx,
								   c->y1 + src_dy,
								   c->x1 - tile.x1,
								   c->y1 - tile.y1,
								   c->x2 - c->x1,
								   c->y2 - c->y1,
								   and, or);
							c++;
						}

						if (c != clipped)
							n = sna->render.copy_boxes(sna, GXcopy,
										   &tmp, src_bo, -tile.x1, -tile.y1,
										   &dst->drawable, dst_bo, dst_dx, dst_dy,
										   clipped, c - clipped, 0);
						else
							n = 1;

						sigtrap_put();
					} else
						n = 0;

					kgem_bo_destroy(&sna->kgem, src_bo);

					if (!n) {
						if (clipped != stack)
							free(clipped);
						goto fallback;
					}
				}
			}

			if (clipped != stack)
				free(clipped);
		} else {
			src_bo = kgem_create_buffer_2d(kgem,
						       tmp.width,
						       tmp.height,
						       tmp.bitsPerPixel,
						       KGEM_BUFFER_WRITE_INPLACE,
						       &ptr);
			if (!src_bo)
				goto fallback;

			if (sigtrap_get() == 0) {
				for (n = 0; n < nbox; n++) {
					DBG(("%s: box(%d, %d), (%d, %d), src=(%d, %d), dst=(%d, %d)\n",
					     __FUNCTION__,
					     box[n].x1, box[n].y1,
					     box[n].x2, box[n].y2,
					     src_dx, src_dy,
					     box[n].x1 - extents.x1,
					     box[n].y1 - extents.y1));
					memcpy_xor(src, ptr, tmp.bitsPerPixel,
						   stride, src_bo->pitch,
						   box[n].x1 + src_dx,
						   box[n].y1 + src_dy,
						   box[n].x1 - extents.x1,
						   box[n].y1 - extents.y1,
						   box[n].x2 - box[n].x1,
						   box[n].y2 - box[n].y1,
						   and, or);
				}

				n = sna->render.copy_boxes(sna, GXcopy,
							   &tmp, src_bo, -extents.x1, -extents.y1,
							   &dst->drawable, dst_bo, dst_dx, dst_dy,
							   box, nbox, 0);
				sigtrap_put();
			} else
				n = 0;

			kgem_bo_destroy(&sna->kgem, src_bo);

			if (!n)
				goto tile;
		}

		return true;
	}

	cmd = XY_SRC_COPY_BLT_CMD;
	br13 = dst_bo->pitch;
	if (kgem->gen >= 040 && dst_bo->tiling) {
		cmd |= BLT_DST_TILED;
		br13 >>= 2;
	}
	br13 |= 0xcc << 16;
	switch (dst->drawable.bitsPerPixel) {
	default:
	case 32: cmd |= BLT_WRITE_ALPHA | BLT_WRITE_RGB;
		 br13 |= 1 << 25; /* RGB8888 */
	case 16: br13 |= 1 << 24; /* RGB565 */
	case 8: break;
	}

	kgem_set_mode(kgem, KGEM_BLT, dst_bo);
	if (!kgem_check_batch(kgem, 10) ||
	    !kgem_check_reloc_and_exec(kgem, 2) ||
	    !kgem_check_bo_fenced(kgem, dst_bo)) {
		kgem_submit(kgem);
		if (!kgem_check_bo_fenced(kgem, dst_bo))
			goto fallback;
		_kgem_set_mode(kgem, KGEM_BLT);
	}

	if (sna->kgem.gen >= 0100) {
		cmd |= 8;
		do {
			int nbox_this_time;

			nbox_this_time = nbox;
			if (10*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
				nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
			if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
				nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc) / 2;
			assert(nbox_this_time);
			nbox -= nbox_this_time;

			/* Count the total number of bytes to be read and allocate a
			 * single buffer large enough. Or if it is very small, combine
			 * with other allocations. */
			offset = 0;
			for (n = 0; n < nbox_this_time; n++) {
				int height = box[n].y2 - box[n].y1;
				int width = box[n].x2 - box[n].x1;
				offset += PITCH(width, dst->drawable.bitsPerPixel >> 3) * height;
			}

			src_bo = kgem_create_buffer(kgem, offset,
						    KGEM_BUFFER_WRITE_INPLACE | (nbox ? KGEM_BUFFER_LAST : 0),
						    &ptr);
			if (!src_bo)
				goto fallback;

			if (sigtrap_get()) {
				kgem_bo_destroy(kgem, src_bo);
				goto fallback;
			}

			offset = 0;
			do {
				int height = box->y2 - box->y1;
				int width = box->x2 - box->x1;
				int pitch = PITCH(width, dst->drawable.bitsPerPixel >> 3);
				uint32_t *b;

				DBG(("  %s: box src=(%d, %d), dst=(%d, %d) size=(%d, %d), dst offset=%d, dst pitch=%d\n",
				     __FUNCTION__,
				     box->x1 + src_dx, box->y1 + src_dy,
				     box->x1 + dst_dx, box->y1 + dst_dy,
				     width, height,
				     offset, pitch));

				assert(box->x1 + src_dx >= 0);
				assert((box->x2 + src_dx)*dst->drawable.bitsPerPixel <= 8*stride);
				assert(box->y1 + src_dy >= 0);

				assert(box->x1 + dst_dx >= 0);
				assert(box->y1 + dst_dy >= 0);

				memcpy_xor(src, (char *)ptr + offset,
					   dst->drawable.bitsPerPixel,
					   stride, pitch,
					   box->x1 + src_dx, box->y1 + src_dy,
					   0, 0,
					   width, height,
					   and, or);

				assert(kgem->mode == KGEM_BLT);
				b = kgem->batch + kgem->nbatch;
				b[0] = cmd;
				b[1] = br13;
				b[2] = (box->y1 + dst_dy) << 16 | (box->x1 + dst_dx);
				b[3] = (box->y2 + dst_dy) << 16 | (box->x2 + dst_dx);
				*(uint64_t *)(b+4) =
					kgem_add_reloc64(kgem, kgem->nbatch + 4, dst_bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 I915_GEM_DOMAIN_RENDER |
							 KGEM_RELOC_FENCED,
							 0);
				b[6] = 0;
				b[7] = pitch;
				*(uint64_t *)(b+8) =
					kgem_add_reloc64(kgem, kgem->nbatch + 8, src_bo,
							 I915_GEM_DOMAIN_RENDER << 16 |
							 KGEM_RELOC_FENCED,
							 offset);
				kgem->nbatch += 10;

				box++;
				offset += pitch * height;
			} while (--nbox_this_time);
			assert(offset == __kgem_buffer_size(src_bo));
			sigtrap_put();

			if (nbox) {
				_kgem_submit(kgem);
				_kgem_set_mode(kgem, KGEM_BLT);
			}

			kgem_bo_destroy(kgem, src_bo);
		} while (nbox);
	} else {
		cmd |= 6;
		do {
			int nbox_this_time;

			nbox_this_time = nbox;
			if (8*nbox_this_time > kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED)
				nbox_this_time = (kgem->surface - kgem->nbatch - KGEM_BATCH_RESERVED) / 8;
			if (2*nbox_this_time > KGEM_RELOC_SIZE(kgem) - kgem->nreloc)
				nbox_this_time = (KGEM_RELOC_SIZE(kgem) - kgem->nreloc) / 2;
			assert(nbox_this_time);
			nbox -= nbox_this_time;

			/* Count the total number of bytes to be read and allocate a
			 * single buffer large enough. Or if it is very small, combine
			 * with other allocations. */
			offset = 0;
			for (n = 0; n < nbox_this_time; n++) {
				int height = box[n].y2 - box[n].y1;
				int width = box[n].x2 - box[n].x1;
				offset += PITCH(width, dst->drawable.bitsPerPixel >> 3) * height;
			}

			src_bo = kgem_create_buffer(kgem, offset,
						    KGEM_BUFFER_WRITE_INPLACE | (nbox ? KGEM_BUFFER_LAST : 0),
						    &ptr);
			if (!src_bo)
				goto fallback;

			if (sigtrap_get()) {
				kgem_bo_destroy(kgem, src_bo);
				goto fallback;
			}

			offset = 0;
			do {
				int height = box->y2 - box->y1;
				int width = box->x2 - box->x1;
				int pitch = PITCH(width, dst->drawable.bitsPerPixel >> 3);
				uint32_t *b;

				DBG(("  %s: box src=(%d, %d), dst=(%d, %d) size=(%d, %d), dst offset=%d, dst pitch=%d\n",
				     __FUNCTION__,
				     box->x1 + src_dx, box->y1 + src_dy,
				     box->x1 + dst_dx, box->y1 + dst_dy,
				     width, height,
				     offset, pitch));

				assert(box->x1 + src_dx >= 0);
				assert((box->x2 + src_dx)*dst->drawable.bitsPerPixel <= 8*stride);
				assert(box->y1 + src_dy >= 0);

				assert(box->x1 + dst_dx >= 0);
				assert(box->y1 + dst_dy >= 0);

				memcpy_xor(src, (char *)ptr + offset,
					   dst->drawable.bitsPerPixel,
					   stride, pitch,
					   box->x1 + src_dx, box->y1 + src_dy,
					   0, 0,
					   width, height,
					   and, or);

				assert(kgem->mode == KGEM_BLT);
				b = kgem->batch + kgem->nbatch;
				b[0] = cmd;
				b[1] = br13;
				b[2] = (box->y1 + dst_dy) << 16 | (box->x1 + dst_dx);
				b[3] = (box->y2 + dst_dy) << 16 | (box->x2 + dst_dx);
				b[4] = kgem_add_reloc(kgem, kgem->nbatch + 4, dst_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      I915_GEM_DOMAIN_RENDER |
						      KGEM_RELOC_FENCED,
						      0);
				b[5] = 0;
				b[6] = pitch;
				b[7] = kgem_add_reloc(kgem, kgem->nbatch + 7, src_bo,
						      I915_GEM_DOMAIN_RENDER << 16 |
						      KGEM_RELOC_FENCED,
						      offset);
				kgem->nbatch += 8;

				box++;
				offset += pitch * height;
			} while (--nbox_this_time);
			assert(offset == __kgem_buffer_size(src_bo));
			sigtrap_put();

			if (nbox) {
				_kgem_submit(kgem);
				_kgem_set_mode(kgem, KGEM_BLT);
			}

			kgem_bo_destroy(kgem, src_bo);
		} while (nbox);
	}

	sna->blt_state.fill_bo = 0;
	return true;

fallback:
	return write_boxes_inplace__xor(kgem,
					src, stride, dst->drawable.bitsPerPixel, src_dx, src_dy,
					dst_bo, dst_dx, dst_dy,
					box, nbox,
					and, or);
}

static bool
indirect_replace(struct sna *sna,
		 PixmapPtr pixmap,
		 struct kgem_bo *bo,
		 const void *src, int stride)
{
	struct kgem *kgem = &sna->kgem;
	struct kgem_bo *src_bo;
	BoxRec box;
	void *ptr;
	bool ret;

	DBG(("%s: size=%d vs %d\n",
	     __FUNCTION__,
	     stride * pixmap->drawable.height >> 12,
	     kgem->half_cpu_cache_pages));

	if (stride * pixmap->drawable.height >> 12 > kgem->half_cpu_cache_pages)
		return false;

	if (!kgem_bo_can_blt(kgem, bo) &&
	    must_tile(sna, pixmap->drawable.width, pixmap->drawable.height))
		return false;

	src_bo = kgem_create_buffer_2d(kgem,
				       pixmap->drawable.width,
				       pixmap->drawable.height,
				       pixmap->drawable.bitsPerPixel,
				       KGEM_BUFFER_WRITE_INPLACE,
				       &ptr);
	if (!src_bo)
		return false;

	if (sigtrap_get() == 0) {
		memcpy_blt(src, ptr, pixmap->drawable.bitsPerPixel,
			   stride, src_bo->pitch,
			   0, 0,
			   0, 0,
			   pixmap->drawable.width,
			   pixmap->drawable.height);

		box.x1 = box.y1 = 0;
		box.x2 = pixmap->drawable.width;
		box.y2 = pixmap->drawable.height;

		ret = sna->render.copy_boxes(sna, GXcopy,
					     &pixmap->drawable, src_bo, 0, 0,
					     &pixmap->drawable, bo, 0, 0,
					     &box, 1, 0);
		sigtrap_put();
	} else
		ret = false;

	kgem_bo_destroy(kgem, src_bo);

	return ret;
}

bool sna_replace(struct sna *sna, PixmapPtr pixmap,
		 const void *src, int stride)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	struct kgem_bo *bo = priv->gpu_bo;
	void *dst;

	assert(bo);
	DBG(("%s(handle=%d, %dx%d, bpp=%d, tiling=%d) busy?=%d\n",
	     __FUNCTION__, bo->handle,
	     pixmap->drawable.width,
	     pixmap->drawable.height,
	     pixmap->drawable.bitsPerPixel,
	     bo->tiling,
	     __kgem_bo_is_busy(&sna->kgem, bo)));

	assert(!priv->pinned);

	kgem_bo_undo(&sna->kgem, bo);

	if (__kgem_bo_is_busy(&sna->kgem, bo)) {
		struct kgem_bo *new_bo;

		if (indirect_replace(sna, pixmap, bo, src, stride))
			return true;

		new_bo = kgem_create_2d(&sna->kgem,
					pixmap->drawable.width,
					pixmap->drawable.height,
					pixmap->drawable.bitsPerPixel,
					bo->tiling,
					CREATE_GTT_MAP | CREATE_INACTIVE);
		if (new_bo)
			bo = new_bo;
	}

	if (bo->tiling == I915_TILING_NONE && bo->pitch == stride &&
	    kgem_bo_write(&sna->kgem, bo, src,
			  (pixmap->drawable.height-1)*stride + pixmap->drawable.width*pixmap->drawable.bitsPerPixel/8))
			goto done;

	if (upload_inplace__tiled(&sna->kgem, bo)) {
		BoxRec box;

		box.x1 = box.y1 = 0;
		box.x2 = pixmap->drawable.width;
		box.y2 = pixmap->drawable.height;

		if (write_boxes_inplace__tiled(&sna->kgem, src,
					       stride, pixmap->drawable.bitsPerPixel, 0, 0,
					       bo, 0, 0, &box, 1))
			goto done;
	}

	if (kgem_bo_can_map(&sna->kgem, bo) &&
	    (dst = kgem_bo_map(&sna->kgem, bo)) != NULL &&
	    sigtrap_get() == 0) {
		memcpy_blt(src, dst, pixmap->drawable.bitsPerPixel,
			   stride, bo->pitch,
			   0, 0,
			   0, 0,
			   pixmap->drawable.width,
			   pixmap->drawable.height);
		sigtrap_put();
	} else {
		BoxRec box;

		if (bo != priv->gpu_bo) {
			kgem_bo_destroy(&sna->kgem, bo);
			bo = priv->gpu_bo;
		}

		box.x1 = box.y1 = 0;
		box.x2 = pixmap->drawable.width;
		box.y2 = pixmap->drawable.height;

		if (!sna_write_boxes(sna, pixmap,
				     bo, 0, 0,
				     src, stride, 0, 0,
				     &box, 1))
			return false;
	}

done:
	if (bo != priv->gpu_bo) {
		sna_pixmap_unmap(pixmap, priv);
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = bo;
	}

	return true;
}

bool
sna_replace__xor(struct sna *sna, PixmapPtr pixmap,
		 const void *src, int stride,
		 uint32_t and, uint32_t or)
{
	struct sna_pixmap *priv = sna_pixmap(pixmap);
	struct kgem_bo *bo = priv->gpu_bo;
	void *dst;

	DBG(("%s(handle=%d, %dx%d, bpp=%d, tiling=%d)\n",
	     __FUNCTION__, bo->handle,
	     pixmap->drawable.width,
	     pixmap->drawable.height,
	     pixmap->drawable.bitsPerPixel,
	     bo->tiling));

	assert(!priv->pinned);

	kgem_bo_undo(&sna->kgem, bo);

	if (!kgem_bo_can_map(&sna->kgem, bo) ||
	    __kgem_bo_is_busy(&sna->kgem, bo)) {
		struct kgem_bo *new_bo;

		new_bo = kgem_create_2d(&sna->kgem,
					pixmap->drawable.width,
					pixmap->drawable.height,
					pixmap->drawable.bitsPerPixel,
					bo->tiling,
					CREATE_GTT_MAP | CREATE_INACTIVE);
		if (new_bo)
			bo = new_bo;
	}

	if (kgem_bo_can_map(&sna->kgem, bo) &&
	    (dst = kgem_bo_map(&sna->kgem, bo)) != NULL &&
	    sigtrap_get() == 0) {
		memcpy_xor(src, dst, pixmap->drawable.bitsPerPixel,
			   stride, bo->pitch,
			   0, 0,
			   0, 0,
			   pixmap->drawable.width,
			   pixmap->drawable.height,
			   and, or);
		sigtrap_put();
	} else {
		BoxRec box;

		if (bo != priv->gpu_bo) {
			kgem_bo_destroy(&sna->kgem, bo);
			bo = priv->gpu_bo;
		}

		box.x1 = box.y1 = 0;
		box.x2 = pixmap->drawable.width;
		box.y2 = pixmap->drawable.height;

		if (!sna_write_boxes__xor(sna, pixmap,
					  bo, 0, 0,
					  src, stride, 0, 0,
					  &box, 1,
					  and, or))
			return false;
	}

	if (bo != priv->gpu_bo) {
		sna_pixmap_unmap(pixmap, priv);
		kgem_bo_destroy(&sna->kgem, priv->gpu_bo);
		priv->gpu_bo = bo;
	}

	return true;
}
