/**************************************************************************

Copyright 2010 Intel Corporation
All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xaarop.h"
#include "intel.h"
#include "i830_reg.h"

static dri_bo *
intel_shadow_create_bo(intel_screen_private *intel,
		       int16_t x1, int16_t y1,
		       int16_t x2, int16_t y2,
		       int *pitch)
{
	int w = x2 - x1, h = y2 - y1;
	int size = h * w * intel->cpp;
	dri_bo *bo;

	bo = drm_intel_bo_alloc(intel->bufmgr, "shadow", size, 0);
	if (bo && drm_intel_gem_bo_map_gtt(bo) == 0) {
		char *dst = bo->virtual;
		char *src = intel->shadow_buffer;
		int src_pitch = intel->shadow_stride;
		int row_length = w * intel->cpp;
		int num_rows = h;
		src += y1 * src_pitch + x1 * intel->cpp;
		do {
			memcpy (dst, src, row_length);
			src += src_pitch;
			dst += row_length;
		} while (--num_rows);
		drm_intel_gem_bo_unmap_gtt(bo);
	}

	*pitch = w * intel->cpp;
	return bo;
}

static void intel_shadow_memcpy(intel_screen_private *intel)
{
	char *src_data, *dst_data;
	unsigned int src_pitch, dst_pitch;
	RegionPtr region;
	BoxPtr box;
	int n;

	if (drm_intel_gem_bo_map_gtt(intel->front_buffer))
		return;

	src_data = intel->shadow_buffer;
	dst_data = intel->front_buffer->virtual;

	src_pitch = intel->shadow_stride;
	dst_pitch = intel->front_pitch;

	region = DamageRegion(intel->shadow_damage);
	box = REGION_RECTS(region);
	n = REGION_NUM_RECTS(region);
	while (n--) {
		char *src = src_data + box->y1*src_pitch + box->x1*intel->cpp;
		char *dst = dst_data + box->y1*dst_pitch + box->x1*intel->cpp;
		int len = (box->x2 - box->x1)*intel->cpp;
		int row = box->y2 - box->y1;
		while (row--) {
			memcpy(dst, src, len);
			src += src_pitch;
			dst += dst_pitch;
		}
		box++;
	}
}

void intel_shadow_blt(intel_screen_private *intel)
{
	ScrnInfoPtr scrn = intel->scrn;
	unsigned int dst_pitch;
	uint32_t blt, br13;
	RegionPtr region;
	BoxPtr box;
	int n;

	/* Can we trust the BLT? Otherwise do an uncached mmecy. */
	if (!intel->can_blt || IS_GEN2(intel)) {
		intel_shadow_memcpy(intel);
		return;
	}

	dst_pitch = intel->front_pitch;

	blt = XY_SRC_COPY_BLT_CMD;
	if (intel->cpp == 4)
		blt |= (XY_SRC_COPY_BLT_WRITE_ALPHA |
				XY_SRC_COPY_BLT_WRITE_RGB);

	if (INTEL_INFO(intel)->gen >= 40) {
		if (intel->front_tiling) {
			dst_pitch >>= 2;
			blt |= XY_SRC_COPY_BLT_DST_TILED;
		}
	}

	br13 = ROP_S << 16 | dst_pitch;
	switch (intel->cpp) {
		default:
		case 4: br13 |= 1 << 25; /* RGB8888 */
		case 2: br13 |= 1 << 24; /* RGB565 */
		case 1: break;
	}

	region = DamageRegion(intel->shadow_damage);
	box = REGION_RECTS(region);
	n = REGION_NUM_RECTS(region);
	while (n--) {
		int pitch;
		dri_bo *bo;

		bo = intel_shadow_create_bo(intel,
					    box->x1, box->y1,
					    box->x2, box->y2,
					    &pitch);
		if (bo == NULL)
			return;

		BEGIN_BATCH_BLT(8);
		OUT_BATCH(blt);
		OUT_BATCH(br13);
		OUT_BATCH(box->y1 << 16 | box->x1);
		OUT_BATCH(box->y2 << 16 | box->x2);
		OUT_RELOC_FENCED(intel->front_buffer,
				I915_GEM_DOMAIN_RENDER,
				I915_GEM_DOMAIN_RENDER,
				0);
		OUT_BATCH(0);
		OUT_BATCH(pitch);
		OUT_RELOC(bo, I915_GEM_DOMAIN_RENDER, 0, 0);

		ADVANCE_BATCH();

		drm_intel_bo_unreference(bo);
		box++;
	}
}

void intel_shadow_create(struct intel_screen_private *intel)
{
	ScrnInfoPtr scrn = intel->scrn;
	ScreenPtr screen = scrn->pScreen;
	PixmapPtr pixmap;
	int stride;
	void *buffer;

	pixmap = screen->GetScreenPixmap(screen);
	stride = intel->cpp*scrn->virtualX;
	buffer = malloc(stride * scrn->virtualY);
	if (buffer &&
	    screen->ModifyPixmapHeader(pixmap,
				       scrn->virtualX, scrn->virtualY,
				       -1, -1,
				       stride, buffer)) {
		free(intel->shadow_buffer);
		intel->shadow_buffer = buffer;
	} else
		stride = intel->shadow_stride;

	if (!intel->shadow_damage) {
		intel->shadow_damage =
			DamageCreate(NULL, NULL,
				     DamageReportNone, TRUE,
				     screen, intel);
		DamageRegister(&pixmap->drawable, intel->shadow_damage);
		DamageSetReportAfterOp(intel->shadow_damage, TRUE);
	}

	scrn->displayWidth = stride / intel->cpp;
	intel->shadow_stride = stride;
}
