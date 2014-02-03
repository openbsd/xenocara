/*
 * Copyright © 2012 Intel Corporation
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
#include "gen8_vertex.h"

void gen8_vertex_align(struct sna *sna, const struct sna_composite_op *op)
{
	int vertex_index;

	assert(op->floats_per_rect == 3*op->floats_per_vertex);

	vertex_index = (sna->render.vertex_used + op->floats_per_vertex - 1) / op->floats_per_vertex;
	if ((int)sna->render.vertex_size - vertex_index * op->floats_per_vertex < 2*op->floats_per_rect) {
		DBG(("%s: flushing vertex buffer: new index=%d, max=%d\n",
		     __FUNCTION__, vertex_index, sna->render.vertex_size / op->floats_per_vertex));
		if (gen8_vertex_finish(sna) < 2*op->floats_per_rect) {
			kgem_submit(&sna->kgem);
			_kgem_set_mode(&sna->kgem, KGEM_RENDER);
		}

		vertex_index = (sna->render.vertex_used + op->floats_per_vertex - 1) / op->floats_per_vertex;
		assert(vertex_index * op->floats_per_vertex <= sna->render.vertex_size);
	}

	sna->render.vertex_index = vertex_index;
	sna->render.vertex_used = vertex_index * op->floats_per_vertex;
}

void gen8_vertex_flush(struct sna *sna)
{
	DBG(("%s[%x] = %d\n", __FUNCTION__,
	     4*sna->render.vertex_offset,
	     sna->render.vertex_index - sna->render.vertex_start));

	assert(sna->render.vertex_offset);
	assert(sna->render.vertex_offset <= sna->kgem.nbatch);
	assert(sna->render.vertex_index > sna->render.vertex_start);
	assert(sna->render.vertex_used <= sna->render.vertex_size);

	sna->kgem.batch[sna->render.vertex_offset] =
		sna->render.vertex_index - sna->render.vertex_start;
	sna->render.vertex_offset = 0;
}

int gen8_vertex_finish(struct sna *sna)
{
	struct kgem_bo *bo;
	unsigned int i;
	unsigned hint, size;

	DBG(("%s: used=%d / %d\n", __FUNCTION__,
	     sna->render.vertex_used, sna->render.vertex_size));
	assert(sna->render.vertex_offset == 0);
	assert(sna->render.vertex_used);
	assert(sna->render.vertex_used <= sna->render.vertex_size);

	sna_vertex_wait__locked(&sna->render);

	/* Note: we only need dword alignment (currently) */

	hint = CREATE_GTT_MAP;

	bo = sna->render.vbo;
	if (bo) {
		for (i = 0; i < sna->render.nvertex_reloc; i++) {
			DBG(("%s: reloc[%d] = %d\n", __FUNCTION__,
			     i, sna->render.vertex_reloc[i]));

			*(uint64_t *)(sna->kgem.batch+sna->render.vertex_reloc[i]) =
				kgem_add_reloc64(&sna->kgem,
						 sna->render.vertex_reloc[i], bo,
						 I915_GEM_DOMAIN_VERTEX << 16,
						 0);
		}

		assert(!sna->render.active);
		sna->render.nvertex_reloc = 0;
		sna->render.vertex_used = 0;
		sna->render.vertex_index = 0;
		sna->render.vbo = NULL;
		sna->render.vb_id = 0;

		kgem_bo_destroy(&sna->kgem, bo);
		hint |= CREATE_CACHED | CREATE_NO_THROTTLE;
	} else {
		if (kgem_is_idle(&sna->kgem)) {
			sna->render.vertices = sna->render.vertex_data;
			sna->render.vertex_size = ARRAY_SIZE(sna->render.vertex_data);
			return 0;
		}
	}

	size = 256*1024;
	assert(!sna->render.active);
	sna->render.vertices = NULL;
	sna->render.vbo = kgem_create_linear(&sna->kgem, size, hint);
	while (sna->render.vbo == NULL && size > 16*1024) {
		size /= 2;
		sna->render.vbo = kgem_create_linear(&sna->kgem, size, hint);
	}
	if (sna->render.vbo == NULL)
		sna->render.vbo = kgem_create_linear(&sna->kgem,
						     256*1024, CREATE_GTT_MAP);
	if (sna->render.vbo)
		sna->render.vertices = kgem_bo_map(&sna->kgem, sna->render.vbo);
	if (sna->render.vertices == NULL) {
		if (sna->render.vbo) {
			kgem_bo_destroy(&sna->kgem, sna->render.vbo);
			sna->render.vbo = NULL;
		}
		sna->render.vertices = sna->render.vertex_data;
		sna->render.vertex_size = ARRAY_SIZE(sna->render.vertex_data);
		return 0;
	}

	if (sna->render.vertex_used) {
		DBG(("%s: copying initial buffer x %d to handle=%d\n",
		     __FUNCTION__,
		     sna->render.vertex_used,
		     sna->render.vbo->handle));
		assert(sizeof(float)*sna->render.vertex_used <=
		       __kgem_bo_size(sna->render.vbo));
		memcpy(sna->render.vertices,
		       sna->render.vertex_data,
		       sizeof(float)*sna->render.vertex_used);
	}

	size = __kgem_bo_size(sna->render.vbo)/4;
	if (size >= UINT16_MAX)
		size = UINT16_MAX - 1;

	DBG(("%s: create vbo handle=%d, size=%d\n",
	     __FUNCTION__, sna->render.vbo->handle, size));

	sna->render.vertex_size = size;
	return sna->render.vertex_size - sna->render.vertex_used;
}

void gen8_vertex_close(struct sna *sna)
{
	struct kgem_bo *bo, *free_bo = NULL;
	unsigned int i, delta = 0;

	assert(sna->render.vertex_offset == 0);
	if (!sna->render.vb_id)
		return;

	DBG(("%s: used=%d, vbo active? %d, vb=%x, nreloc=%d\n",
	     __FUNCTION__, sna->render.vertex_used, sna->render.vbo ? sna->render.vbo->handle : 0,
	     sna->render.vb_id, sna->render.nvertex_reloc));

	assert(!sna->render.active);

	bo = sna->render.vbo;
	if (bo) {
		if (sna->render.vertex_size - sna->render.vertex_used < 64) {
			DBG(("%s: discarding vbo (full), handle=%d\n", __FUNCTION__, sna->render.vbo->handle));
			sna->render.vbo = NULL;
			sna->render.vertices = sna->render.vertex_data;
			sna->render.vertex_size = ARRAY_SIZE(sna->render.vertex_data);
			free_bo = bo;
		} else if (!sna->kgem.has_llc && sna->render.vertices == MAP(bo->map__cpu)) {
			DBG(("%s: converting CPU map to GTT\n", __FUNCTION__));
			sna->render.vertices =
				kgem_bo_map__gtt(&sna->kgem, sna->render.vbo);
			if (sna->render.vertices == NULL) {
				sna->render.vbo = NULL;
				sna->render.vertices = sna->render.vertex_data;
				sna->render.vertex_size = ARRAY_SIZE(sna->render.vertex_data);
				free_bo = bo;
			}

		}
	} else {
		int size;

		size  = sna->kgem.nbatch;
		size += sna->kgem.batch_size - sna->kgem.surface;
		size += sna->render.vertex_used;

		if (size <= 1024) {
			DBG(("%s: copy to batch: %d @ %d\n", __FUNCTION__,
			     sna->render.vertex_used, sna->kgem.nbatch));
			assert(sna->kgem.nbatch + sna->render.vertex_used <= sna->kgem.surface);
			memcpy(sna->kgem.batch + sna->kgem.nbatch,
			       sna->render.vertex_data,
			       sna->render.vertex_used * 4);
			delta = sna->kgem.nbatch * 4;
			bo = NULL;
			sna->kgem.nbatch += sna->render.vertex_used;
		} else {
			size = 256 * 1024;
			do {
				bo = kgem_create_linear(&sna->kgem, size,
							CREATE_GTT_MAP | CREATE_NO_RETIRE | CREATE_NO_THROTTLE | CREATE_CACHED);
			} while (bo == NULL && (size>>=1) > sizeof(float)*sna->render.vertex_used);

			sna->render.vertices = NULL;
			if (bo)
				sna->render.vertices = kgem_bo_map(&sna->kgem, bo);
			if (sna->render.vertices != NULL) {
				DBG(("%s: new vbo: %d / %d\n", __FUNCTION__,
				     sna->render.vertex_used, __kgem_bo_size(bo)/4));

				assert(sizeof(float)*sna->render.vertex_used <= __kgem_bo_size(bo));
				memcpy(sna->render.vertices,
				       sna->render.vertex_data,
				       sizeof(float)*sna->render.vertex_used);

				size = __kgem_bo_size(bo)/4;
				if (size >= UINT16_MAX)
					size = UINT16_MAX - 1;

				sna->render.vbo = bo;
				sna->render.vertex_size = size;
			} else {
				DBG(("%s: tmp vbo: %d\n", __FUNCTION__,
				     sna->render.vertex_used));

				if (bo)
					kgem_bo_destroy(&sna->kgem, bo);

				bo = kgem_create_linear(&sna->kgem,
							4*sna->render.vertex_used,
							CREATE_NO_THROTTLE);
				if (bo && !kgem_bo_write(&sna->kgem, bo,
							 sna->render.vertex_data,
							 4*sna->render.vertex_used)) {
					kgem_bo_destroy(&sna->kgem, bo);
					bo = NULL;
				}

				assert(sna->render.vbo == NULL);
				sna->render.vertices = sna->render.vertex_data;
				sna->render.vertex_size = ARRAY_SIZE(sna->render.vertex_data);
				free_bo = bo;
			}
		}
	}

	assert(sna->render.nvertex_reloc);
	for (i = 0; i < sna->render.nvertex_reloc; i++) {
		DBG(("%s: reloc[%d] = %d\n", __FUNCTION__,
		     i, sna->render.vertex_reloc[i]));

		*(uint64_t *)(sna->kgem.batch+sna->render.vertex_reloc[i]) =
			kgem_add_reloc64(&sna->kgem,
					 sna->render.vertex_reloc[i], bo,
					 I915_GEM_DOMAIN_VERTEX << 16,
					 delta);
	}
	sna->render.nvertex_reloc = 0;
	sna->render.vb_id = 0;

	if (sna->render.vbo == NULL) {
		assert(!sna->render.active);
		sna->render.vertex_used = 0;
		sna->render.vertex_index = 0;
		assert(sna->render.vertices == sna->render.vertex_data);
		assert(sna->render.vertex_size == ARRAY_SIZE(sna->render.vertex_data));
	}

	if (free_bo)
		kgem_bo_destroy(&sna->kgem, free_bo);
}
