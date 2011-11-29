/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
All Rights Reserved.
Copyright (c) 2005 Jesse Barnes <jbarnes@virtuousgeek.org>
  Based on code from i830_xaa.c.

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

**************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xaarop.h"
#include "intel.h"
#include "i830_reg.h"
#include "i915_drm.h"
#include "brw_defines.h"
#include <string.h>
#include <sys/mman.h>
#include <errno.h>

static const int I830CopyROP[16] = {
	ROP_0,			/* GXclear */
	ROP_DSa,		/* GXand */
	ROP_SDna,		/* GXandReverse */
	ROP_S,			/* GXcopy */
	ROP_DSna,		/* GXandInverted */
	ROP_D,			/* GXnoop */
	ROP_DSx,		/* GXxor */
	ROP_DSo,		/* GXor */
	ROP_DSon,		/* GXnor */
	ROP_DSxn,		/* GXequiv */
	ROP_Dn,			/* GXinvert */
	ROP_SDno,		/* GXorReverse */
	ROP_Sn,			/* GXcopyInverted */
	ROP_DSno,		/* GXorInverted */
	ROP_DSan,		/* GXnand */
	ROP_1			/* GXset */
};

static const int I830PatternROP[16] = {
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

#if HAS_DEVPRIVATEKEYREC
DevPrivateKeyRec uxa_pixmap_index;
#else
int uxa_pixmap_index;
#endif

static void
gen6_context_switch(intel_screen_private *intel,
		    int new_mode)
{
	intel_batch_submit(intel->scrn);
}

static void
gen5_context_switch(intel_screen_private *intel,
		    int new_mode)
{
	/* Ironlake has a limitation that a 3D or Media command can't
	 * be the first command after a BLT, unless it's
	 * non-pipelined.  Instead of trying to track it and emit a
	 * command at the right time, we just emit a dummy
	 * non-pipelined 3D instruction after each blit.
	 */

	if (new_mode == I915_EXEC_BLT) {
		OUT_BATCH(MI_FLUSH |
			  MI_STATE_INSTRUCTION_CACHE_FLUSH |
			  MI_INHIBIT_RENDER_CACHE_FLUSH);
	} else {
		OUT_BATCH(CMD_POLY_STIPPLE_OFFSET << 16);
		OUT_BATCH(0);
	}
}

static void
gen4_context_switch(intel_screen_private *intel,
		    int new_mode)
{
	if (new_mode == I915_EXEC_BLT) {
		OUT_BATCH(MI_FLUSH |
			  MI_STATE_INSTRUCTION_CACHE_FLUSH |
			  MI_INHIBIT_RENDER_CACHE_FLUSH);
	}
}

Bool
intel_get_aperture_space(ScrnInfoPtr scrn, drm_intel_bo ** bo_table,
			 int num_bos)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->batch_bo == NULL) {
		intel_debug_fallback(scrn, "VT inactive\n");
		return FALSE;
	}

	bo_table[0] = intel->batch_bo;
	if (drm_intel_bufmgr_check_aperture_space(bo_table, num_bos) != 0) {
		intel_batch_submit(scrn);
		bo_table[0] = intel->batch_bo;
		if (drm_intel_bufmgr_check_aperture_space(bo_table, num_bos) !=
		    0) {
			intel_debug_fallback(scrn, "Couldn't get aperture "
					    "space for BOs\n");
			return FALSE;
		}
	}
	return TRUE;
}

static unsigned int
intel_uxa_pixmap_compute_size(PixmapPtr pixmap,
			      int w, int h,
			      uint32_t *tiling,
			      int *stride,
			      unsigned usage)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int pitch, size;

	if (*tiling != I915_TILING_NONE) {
		/* First check whether tiling is necessary. */
		pitch = (w * pixmap->drawable.bitsPerPixel + 7) / 8;
		pitch = ALIGN(pitch, 64);
		size = pitch * ALIGN (h, 2);
		if (INTEL_INFO(intel)->gen < 40) {
			/* Gen 2/3 has a maximum stride for tiling of
			 * 8192 bytes.
			 */
			if (pitch > KB(8))
				*tiling = I915_TILING_NONE;

			/* Narrower than half a tile? */
			if (pitch < 256)
				*tiling = I915_TILING_NONE;

			/* Older hardware requires fences to be pot size
			 * aligned with a minimum of 1 MiB, so causes
			 * massive overallocation for small textures.
			 */
			if (size < 1024*1024/2 && !intel->has_relaxed_fencing)
				*tiling = I915_TILING_NONE;
		} else if (!(usage & INTEL_CREATE_PIXMAP_DRI2) && size <= 4096) {
			/* Disable tiling beneath a page size, we will not see
			 * any benefit from reducing TLB misses and instead
			 * just incur extra cost when we require a fence.
			 */
			*tiling = I915_TILING_NONE;
		}
	}

	pitch = (w * pixmap->drawable.bitsPerPixel + 7) / 8;
	if (!(usage & INTEL_CREATE_PIXMAP_DRI2) && pitch <= 256)
		*tiling = I915_TILING_NONE;

	if (*tiling != I915_TILING_NONE) {
		int aligned_h, tile_height;

		if (IS_GEN2(intel))
			tile_height = 16;
		else if (*tiling == I915_TILING_X)
			tile_height = 8;
		else
			tile_height = 32;
		aligned_h = ALIGN(h, tile_height);

		*stride = intel_get_fence_pitch(intel,
						ALIGN(pitch, 512),
						*tiling);

		/* Round the object up to the size of the fence it will live in
		 * if necessary.  We could potentially make the kernel allocate
		 * a larger aperture space and just bind the subset of pages in,
		 * but this is easier and also keeps us out of trouble (as much)
		 * with drm_intel_bufmgr_check_aperture().
		 */
		size = intel_get_fence_size(intel, *stride * aligned_h);

		if (size > intel->max_tiling_size)
			*tiling = I915_TILING_NONE;
	}

	if (*tiling == I915_TILING_NONE) {
		/* Round the height up so that the GPU's access to a 2x2 aligned
		 * subspan doesn't address an invalid page offset beyond the
		 * end of the GTT.
		 */
		*stride = ALIGN(pitch, 64);
		size = *stride * ALIGN(h, 2);
	}

	return size;
}

static Bool
intel_uxa_check_solid(DrawablePtr drawable, int alu, Pixel planemask)
{
	ScrnInfoPtr scrn = xf86Screens[drawable->pScreen->myNum];

	if (!UXA_PM_IS_SOLID(drawable, planemask)) {
		intel_debug_fallback(scrn, "planemask is not solid\n");
		return FALSE;
	}

	switch (drawable->bitsPerPixel) {
	case 8:
	case 16:
	case 32:
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

/**
 * Sets up hardware state for a series of solid fills.
 */
static Bool
intel_uxa_prepare_solid(PixmapPtr pixmap, int alu, Pixel planemask, Pixel fg)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *bo_table[] = {
		NULL,		/* batch_bo */
		intel_get_pixmap_bo(pixmap),
	};

	if (!intel_check_pitch_2d(pixmap))
		return FALSE;

	if (!intel_get_aperture_space(scrn, bo_table, ARRAY_SIZE(bo_table)))
		return FALSE;

	intel->BR[13] = (I830PatternROP[alu] & 0xff) << 16;
	switch (pixmap->drawable.bitsPerPixel) {
	case 8:
		break;
	case 16:
		/* RGB565 */
		intel->BR[13] |= (1 << 24);
		break;
	case 32:
		/* RGB8888 */
		intel->BR[13] |= ((1 << 24) | (1 << 25));
		break;
	}
	intel->BR[16] = fg;

	return TRUE;
}

static void intel_uxa_solid(PixmapPtr pixmap, int x1, int y1, int x2, int y2)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	unsigned long pitch;
	uint32_t cmd;

	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;
	if (x2 > pixmap->drawable.width)
		x2 = pixmap->drawable.width;
	if (y2 > pixmap->drawable.height)
		y2 = pixmap->drawable.height;

	if (x2 <= x1 || y2 <= y1)
		return;

	pitch = intel_pixmap_pitch(pixmap);

	{
		BEGIN_BATCH_BLT(6);

		cmd = XY_COLOR_BLT_CMD;

		if (pixmap->drawable.bitsPerPixel == 32)
			cmd |=
			    XY_COLOR_BLT_WRITE_ALPHA | XY_COLOR_BLT_WRITE_RGB;

		if (INTEL_INFO(intel)->gen >= 40 && intel_pixmap_tiled(pixmap)) {
			assert((pitch % 512) == 0);
			pitch >>= 2;
			cmd |= XY_COLOR_BLT_TILED;
		}

		OUT_BATCH(cmd);

		OUT_BATCH(intel->BR[13] | pitch);
		OUT_BATCH((y1 << 16) | (x1 & 0xffff));
		OUT_BATCH((y2 << 16) | (x2 & 0xffff));
		OUT_RELOC_PIXMAP_FENCED(pixmap, I915_GEM_DOMAIN_RENDER,
					I915_GEM_DOMAIN_RENDER, 0);
		OUT_BATCH(intel->BR[16]);
		ADVANCE_BATCH();
	}
}

static void intel_uxa_done_solid(PixmapPtr pixmap)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];

	intel_debug_flush(scrn);
}

/**
 * TODO:
 *   - support planemask using FULL_BLT_CMD?
 */
static Bool
intel_uxa_check_copy(PixmapPtr source, PixmapPtr dest,
		    int alu, Pixel planemask)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];

	if (!UXA_PM_IS_SOLID(&source->drawable, planemask)) {
		intel_debug_fallback(scrn, "planemask is not solid");
		return FALSE;
	}

	if (source->drawable.bitsPerPixel != dest->drawable.bitsPerPixel) {
		intel_debug_fallback(scrn, "mixed bpp copies unsupported\n");
		return FALSE;
	}
	switch (source->drawable.bitsPerPixel) {
	case 8:
	case 16:
	case 32:
		break;
	default:
		return FALSE;
	}

	if (!intel_check_pitch_2d(source))
		return FALSE;
	if (!intel_check_pitch_2d(dest))
		return FALSE;

	return TRUE;
}

static Bool
intel_uxa_prepare_copy(PixmapPtr source, PixmapPtr dest, int xdir,
		      int ydir, int alu, Pixel planemask)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *bo_table[] = {
		NULL,		/* batch_bo */
		intel_get_pixmap_bo(source),
		intel_get_pixmap_bo(dest),
	};

	if (!intel_get_aperture_space(scrn, bo_table, ARRAY_SIZE(bo_table)))
		return FALSE;

	intel->render_source = source;

	intel->BR[13] = I830CopyROP[alu] << 16;
	switch (source->drawable.bitsPerPixel) {
	case 8:
		break;
	case 16:
		intel->BR[13] |= (1 << 24);
		break;
	case 32:
		intel->BR[13] |= ((1 << 25) | (1 << 24));
		break;
	}

	return TRUE;
}

static void
intel_uxa_copy(PixmapPtr dest, int src_x1, int src_y1, int dst_x1,
	      int dst_y1, int w, int h)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	uint32_t cmd;
	int dst_x2, dst_y2, src_x2, src_y2;
	unsigned int dst_pitch, src_pitch;

	dst_x2 = dst_x1 + w;
	dst_y2 = dst_y1 + h;

	/* XXX Fixup extents as a lamentable workaround for missing
	 * source clipping in the upper layers.
	 */
	if (dst_x1 < 0)
		src_x1 -= dst_x1, dst_x1 = 0;
	if (dst_y1 < 0)
		src_y1 -= dst_y1, dst_y1 = 0;
	if (dst_x2 > dest->drawable.width)
		dst_x2 = dest->drawable.width;
	if (dst_y2 > dest->drawable.height)
		dst_y2 = dest->drawable.height;

	src_x2 = src_x1 + (dst_x2 - dst_x1);
	src_y2 = src_y1 + (dst_y2 - dst_y1);

	if (src_x1 < 0)
		dst_x1 -= src_x1, src_x1 = 0;
	if (src_y1 < 0)
		dst_y1 -= src_y1, src_y1 = 0;
	if (src_x2 > intel->render_source->drawable.width)
		dst_x2 -= src_x2 - intel->render_source->drawable.width;
	if (src_y2 > intel->render_source->drawable.height)
		dst_y2 -= src_y2 - intel->render_source->drawable.height;

	if (dst_x2 <= dst_x1 || dst_y2 <= dst_y1)
		return;

	dst_pitch = intel_pixmap_pitch(dest);
	src_pitch = intel_pixmap_pitch(intel->render_source);

	{
		BEGIN_BATCH_BLT(8);

		cmd = XY_SRC_COPY_BLT_CMD;

		if (dest->drawable.bitsPerPixel == 32)
			cmd |=
			    XY_SRC_COPY_BLT_WRITE_ALPHA |
			    XY_SRC_COPY_BLT_WRITE_RGB;

		if (INTEL_INFO(intel)->gen >= 40) {
			if (intel_pixmap_tiled(dest)) {
				assert((dst_pitch % 512) == 0);
				dst_pitch >>= 2;
				cmd |= XY_SRC_COPY_BLT_DST_TILED;
			}

			if (intel_pixmap_tiled(intel->render_source)) {
				assert((src_pitch % 512) == 0);
				src_pitch >>= 2;
				cmd |= XY_SRC_COPY_BLT_SRC_TILED;
			}
		}

		OUT_BATCH(cmd);

		OUT_BATCH(intel->BR[13] | dst_pitch);
		OUT_BATCH((dst_y1 << 16) | (dst_x1 & 0xffff));
		OUT_BATCH((dst_y2 << 16) | (dst_x2 & 0xffff));
		OUT_RELOC_PIXMAP_FENCED(dest,
					I915_GEM_DOMAIN_RENDER,
					I915_GEM_DOMAIN_RENDER,
					0);
		OUT_BATCH((src_y1 << 16) | (src_x1 & 0xffff));
		OUT_BATCH(src_pitch);
		OUT_RELOC_PIXMAP_FENCED(intel->render_source,
					I915_GEM_DOMAIN_RENDER, 0,
					0);

		ADVANCE_BATCH();
	}
}

static void intel_uxa_done_copy(PixmapPtr dest)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];

	intel_debug_flush(scrn);
}

/**
 * Do any cleanup from the Composite operation.
 *
 * This is shared between i830 through i965.
 */
static void i830_done_composite(PixmapPtr dest)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->vertex_flush)
		intel->vertex_flush(intel);

	intel_debug_flush(scrn);
}

#define xFixedToFloat(val) \
	((float)xFixedToInt(val) + ((float)xFixedFrac(val) / 65536.0))

static Bool
_intel_transform_point(PictTransformPtr transform,
		       float x, float y, float result[3])
{
	int j;

	for (j = 0; j < 3; j++) {
		result[j] = (xFixedToFloat(transform->matrix[j][0]) * x +
			     xFixedToFloat(transform->matrix[j][1]) * y +
			     xFixedToFloat(transform->matrix[j][2]));
	}
	if (!result[2])
		return FALSE;
	return TRUE;
}

/**
 * Returns the floating-point coordinates transformed by the given transform.
 *
 * transform may be null.
 */
Bool
intel_get_transformed_coordinates(int x, int y, PictTransformPtr transform,
				  float *x_out, float *y_out)
{
	if (transform == NULL) {
		*x_out = x;
		*y_out = y;
	} else {
		float result[3];

		if (!_intel_transform_point(transform,
					    x, y,
					    result))
			return FALSE;
		*x_out = result[0] / result[2];
		*y_out = result[1] / result[2];
	}
	return TRUE;
}

/**
 * Returns the un-normalized floating-point coordinates transformed by the given transform.
 *
 * transform may be null.
 */
Bool
intel_get_transformed_coordinates_3d(int x, int y, PictTransformPtr transform,
				     float *x_out, float *y_out, float *w_out)
{
	if (transform == NULL) {
		*x_out = x;
		*y_out = y;
		*w_out = 1;
	} else {
		float result[3];

		if (!_intel_transform_point(transform,
					    x, y,
					    result))
			return FALSE;
		*x_out = result[0];
		*y_out = result[1];
		*w_out = result[2];
	}
	return TRUE;
}

/**
 * Returns whether the provided transform is affine.
 *
 * transform may be null.
 */
Bool intel_transform_is_affine(PictTransformPtr t)
{
	if (t == NULL)
		return TRUE;
	return t->matrix[2][0] == 0 && t->matrix[2][1] == 0;
}

dri_bo *intel_get_pixmap_bo(PixmapPtr pixmap)
{
	struct intel_pixmap *intel;

	intel = intel_get_pixmap_private(pixmap);
	if (intel == NULL)
		return NULL;

	return intel->bo;
}

void intel_set_pixmap_bo(PixmapPtr pixmap, dri_bo * bo)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_pixmap *priv;

	priv = intel_get_pixmap_private(pixmap);

	if (priv == NULL && bo == NULL)
	    return;

	if (priv != NULL) {
		if (priv->bo == bo)
			return;

		if (list_is_empty(&priv->batch)) {
			dri_bo_unreference(priv->bo);
		} else if (!drm_intel_bo_is_reusable(priv->bo)) {
			dri_bo_unreference(priv->bo);
			list_del(&priv->batch);
			list_del(&priv->flush);
		} else {
			list_add(&priv->in_flight, &intel->in_flight);
			priv = NULL;
		}

		if (intel->render_current_dest == pixmap)
		    intel->render_current_dest = NULL;
	}

	if (bo != NULL) {
		uint32_t tiling;
		uint32_t swizzle_mode;
		int ret;

		if (priv == NULL) {
			priv = calloc(1, sizeof (struct intel_pixmap));
			if (priv == NULL)
				goto BAIL;

			list_init(&priv->batch);
			list_init(&priv->flush);
		}

		dri_bo_reference(bo);
		priv->bo = bo;
		priv->stride = intel_pixmap_pitch(pixmap);

		ret = drm_intel_bo_get_tiling(bo, &tiling, &swizzle_mode);
		if (ret != 0) {
			FatalError("Couldn't get tiling on bo %p: %s\n",
				   bo, strerror(-ret));
		}

		priv->tiling = tiling;
		priv->busy = -1;
		priv->offscreen = 1;
	} else {
		if (priv != NULL) {
			free(priv);
			priv = NULL;
		}
	}

  BAIL:
	intel_set_pixmap_private(pixmap, priv);
}

static Bool intel_uxa_pixmap_is_offscreen(PixmapPtr pixmap)
{
	struct intel_pixmap *priv = intel_get_pixmap_private(pixmap);
	return priv && priv->offscreen;
}

static Bool intel_uxa_prepare_access(PixmapPtr pixmap, uxa_access_t access)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_pixmap *priv = intel_get_pixmap_private(pixmap);
	dri_bo *bo = priv->bo;
	int ret;

	if (!list_is_empty(&priv->batch) &&
	    (access == UXA_ACCESS_RW || priv->batch_write))
		intel_batch_submit(scrn);

	if (priv->tiling || bo->size <= intel->max_gtt_map_size)
		ret = drm_intel_gem_bo_map_gtt(bo);
	else
		ret = dri_bo_map(bo, access == UXA_ACCESS_RW);
	if (ret) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "%s: bo map failed: %s\n",
			   __FUNCTION__,
			   strerror(-ret));
		return FALSE;
	}
	pixmap->devPrivate.ptr = bo->virtual;
	priv->busy = 0;

	return TRUE;
}

static Bool intel_uxa_pixmap_put_image(PixmapPtr pixmap,
				       char *src, int src_pitch,
				       int x, int y, int w, int h)
{
	struct intel_pixmap *priv = intel_get_pixmap_private(pixmap);
	int stride = intel_pixmap_pitch(pixmap);
	int cpp = pixmap->drawable.bitsPerPixel/8;
	int ret = FALSE;

	if (priv == NULL || priv->bo == NULL)
		return FALSE;

	if (priv->tiling == I915_TILING_NONE &&
	    (h == 1 || (src_pitch == stride && w == pixmap->drawable.width))) {
		return drm_intel_bo_subdata(priv->bo, y*stride + x*cpp, stride*(h-1) + w*cpp, src) == 0;
	} else if (drm_intel_gem_bo_map_gtt(priv->bo) == 0) {
		char *dst = priv->bo->virtual;
		int row_length = w * cpp;
		int num_rows = h;
		if (row_length == src_pitch && src_pitch == stride)
			num_rows = 1, row_length *= h;
		dst += y * stride + x * cpp;
		do {
			memcpy (dst, src, row_length);
			src += src_pitch;
			dst += stride;
		} while (--num_rows);
		drm_intel_gem_bo_unmap_gtt(priv->bo);
		ret = TRUE;
	}

	return ret;
}

static Bool intel_uxa_put_image(PixmapPtr pixmap,
				int x, int y,
				int w, int h,
				char *src, int src_pitch)
{
	struct intel_pixmap *priv;

	priv = intel_get_pixmap_private(pixmap);
	if (!intel_pixmap_is_busy(priv)) {
		/* bo is not busy so can be replaced without a stall, upload in-place. */
		return intel_uxa_pixmap_put_image(pixmap, src, src_pitch, x, y, w, h);
	} else {
		ScreenPtr screen = pixmap->drawable.pScreen;

		if (!priv->pinned &&
		    x == 0 && y == 0 &&
		    w == pixmap->drawable.width &&
		    h == pixmap->drawable.height)
		{
			intel_screen_private *intel = intel_get_screen_private(xf86Screens[screen->myNum]);
			uint32_t tiling = priv->tiling;
			int size, stride;
			dri_bo *bo;

			/* Replace busy bo. */
			size = intel_uxa_pixmap_compute_size (pixmap, w, h,
							      &tiling, &stride, 0);
			if (size > intel->max_gtt_map_size)
				return FALSE;

			bo = drm_intel_bo_alloc(intel->bufmgr, "pixmap", size, 0);
			if (bo == NULL)
				return FALSE;

			if (tiling != I915_TILING_NONE)
				drm_intel_bo_set_tiling(bo, &tiling, stride);
			priv->stride = stride;
			priv->tiling = tiling;

			screen->ModifyPixmapHeader(pixmap,
						   w, h,
						   0, 0,
						   stride, NULL);
			intel_set_pixmap_bo(pixmap, bo);
			dri_bo_unreference(bo);

			return intel_uxa_pixmap_put_image(pixmap, src, src_pitch, 0, 0, w, h);
		}
		else
		{
			PixmapPtr scratch;
			Bool ret;

			/* Upload to a linear buffer and queue a blit.  */
			scratch = (*screen->CreatePixmap)(screen, w, h,
							  pixmap->drawable.depth,
							  UXA_CREATE_PIXMAP_FOR_MAP);
			if (!scratch)
				return FALSE;

			if (!intel_uxa_pixmap_is_offscreen(scratch)) {
				screen->DestroyPixmap(scratch);
				return FALSE;
			}

			ret = intel_uxa_pixmap_put_image(scratch, src, src_pitch, 0, 0, w, h);
			if (ret) {
				GCPtr gc = GetScratchGC(pixmap->drawable.depth, screen);
				if (gc) {
					ValidateGC(&pixmap->drawable, gc);

					(*gc->ops->CopyArea)(&scratch->drawable,
							     &pixmap->drawable,
							     gc, 0, 0, w, h, x, y);

					FreeScratchGC(gc);
				} else
					ret = FALSE;
			}

			(*screen->DestroyPixmap)(scratch);
			return ret;
		}
	}
}

static Bool intel_uxa_pixmap_get_image(PixmapPtr pixmap,
				       int x, int y, int w, int h,
				       char *dst, int dst_pitch)
{
	struct intel_pixmap *priv = intel_get_pixmap_private(pixmap);
	int stride = intel_pixmap_pitch(pixmap);
	int cpp = pixmap->drawable.bitsPerPixel/8;

	/* assert(priv->tiling == I915_TILING_NONE); */
	if (h == 1 || (dst_pitch == stride && w == pixmap->drawable.width)) {
		return drm_intel_bo_get_subdata(priv->bo, y*stride + x*cpp, (h-1)*stride + w*cpp, dst) == 0;
	} else {
		char *src;

		if (drm_intel_gem_bo_map_gtt(priv->bo))
		    return FALSE;

		src = (char *) priv->bo->virtual + y * stride + x * cpp;
		w *= cpp;
		do {
			memcpy(dst, src, w);
			src += stride;
			dst += dst_pitch;
		} while (--h);

		drm_intel_gem_bo_unmap_gtt(priv->bo);

		return TRUE;
	}
}

static Bool intel_uxa_get_image(PixmapPtr pixmap,
				int x, int y,
				int w, int h,
				char *dst, int dst_pitch)
{
	struct intel_pixmap *priv;
	PixmapPtr scratch = NULL;
	Bool ret;

	/* The presumption is that we wish to keep the target hot, so
	 * copy to a new bo and move that to the CPU in preference to
	 * causing ping-pong of the original.
	 *
	 * Also the gpu is much faster at detiling.
	 */

	priv = intel_get_pixmap_private(pixmap);
	if (intel_pixmap_is_busy(priv) || priv->tiling != I915_TILING_NONE) {
		ScreenPtr screen = pixmap->drawable.pScreen;
		GCPtr gc;

		/* Copy to a linear buffer and pull.  */
		scratch = screen->CreatePixmap(screen, w, h,
					       pixmap->drawable.depth,
					       INTEL_CREATE_PIXMAP_TILING_NONE);
		if (!scratch)
			return FALSE;

		if (!intel_uxa_pixmap_is_offscreen(scratch)) {
			screen->DestroyPixmap(scratch);
			return FALSE;
		}

		gc = GetScratchGC(pixmap->drawable.depth, screen);
		if (!gc) {
			screen->DestroyPixmap(scratch);
			return FALSE;
		}

		ValidateGC(&pixmap->drawable, gc);

		gc->ops->CopyArea(&pixmap->drawable,
				  &scratch->drawable,
				  gc, x, y, w, h, 0, 0);

		FreeScratchGC(gc);

		intel_batch_submit(xf86Screens[screen->myNum]);

		x = y = 0;
		pixmap = scratch;
	}

	ret = intel_uxa_pixmap_get_image(pixmap, x, y, w, h, dst, dst_pitch);

	if (scratch)
		scratch->drawable.pScreen->DestroyPixmap(scratch);

	return ret;
}

static void intel_flush_rendering(intel_screen_private *intel)
{
	if (intel->needs_flush == 0)
		return;

	if (intel->has_kernel_flush) {
		intel_batch_submit(intel->scrn);
		drm_intel_bo_busy(intel->front_buffer);
	} else {
		intel_batch_emit_flush(intel->scrn);
		intel_batch_submit(intel->scrn);
	}

	intel->needs_flush = 0;
}

void intel_uxa_block_handler(intel_screen_private *intel)
{
	/* don't call this if switche,d doesn't matter and we won't complain
	that way */
	if (!intel->scrn->vtSema)
		return;
	if (intel->shadow_damage &&
	    pixman_region_not_empty(DamageRegion(intel->shadow_damage))) {
		intel_shadow_blt(intel);
		DamageEmpty(intel->shadow_damage);
	}

	/* Emit a flush of the rendering cache, or on the 965
	 * and beyond rendering results may not hit the
	 * framebuffer until significantly later.
	 */
	intel_flush_rendering(intel);
}

static PixmapPtr
intel_uxa_create_pixmap(ScreenPtr screen, int w, int h, int depth,
			unsigned usage)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	PixmapPtr pixmap;

	if (w > 32767 || h > 32767)
		return NullPixmap;

	if (depth == 1 || intel->force_fallback)
		return fbCreatePixmap(screen, w, h, depth, usage);

	if (intel->use_shadow && (usage & INTEL_CREATE_PIXMAP_DRI2) == 0)
		return fbCreatePixmap(screen, w, h, depth, usage);

	if (usage == CREATE_PIXMAP_USAGE_GLYPH_PICTURE && w <= 32 && h <= 32)
		return fbCreatePixmap(screen, w, h, depth, usage);

	pixmap = fbCreatePixmap(screen, 0, 0, depth, usage);

	if (w && h) {
		struct intel_pixmap *priv;
		unsigned int size, tiling;
		int stride;

		/* Always attempt to tile, compute_size() will remove the
		 * tiling for pixmaps that are either too large or too small
		 * to be effectively tiled.
		 */
		tiling = I915_TILING_X;
		if (usage & INTEL_CREATE_PIXMAP_TILING_Y)
			tiling = I915_TILING_Y;
		if (usage == UXA_CREATE_PIXMAP_FOR_MAP || usage & INTEL_CREATE_PIXMAP_TILING_NONE)
			tiling = I915_TILING_NONE;

		/* if tiling is off force to none */
		if (!intel->tiling)
			tiling = I915_TILING_NONE;

		if (tiling != I915_TILING_NONE && !(usage & INTEL_CREATE_PIXMAP_DRI2)) {
		    if (h <= 4)
			tiling = I915_TILING_NONE;
		    if (h <= 16 && tiling == I915_TILING_Y)
			tiling = I915_TILING_X;
		}
		size = intel_uxa_pixmap_compute_size(pixmap, w, h, &tiling, &stride, usage);

		/* Fail very large allocations.  Large BOs will tend to hit SW fallbacks
		 * frequently, and also will tend to fail to successfully map when doing
		 * SW fallbacks because we overcommit address space for BO access.
		 */
		if (size > intel->max_bo_size || stride >= KB(32)) {
			fbDestroyPixmap(pixmap);
			return fbCreatePixmap(screen, w, h, depth, usage);
		}

		/* Perform a preliminary search for an in-flight bo */
		if (usage != UXA_CREATE_PIXMAP_FOR_MAP) {
			int aligned_h;

			if (tiling == I915_TILING_X)
				aligned_h = ALIGN(h, 8);
			else if (tiling == I915_TILING_Y)
				aligned_h = ALIGN(h, 32);
			else
				aligned_h = ALIGN(h, 2);

			list_foreach_entry(priv, struct intel_pixmap,
					   &intel->in_flight,
					   in_flight) {
				if (priv->tiling != tiling)
					continue;

				if (tiling == I915_TILING_NONE) {
				    if (priv->bo->size < size)
					    continue;

					priv->stride = stride;
				} else {
					if (priv->stride < stride ||
					    priv->bo->size < priv->stride * aligned_h)
						continue;

					stride = priv->stride;
				}

				list_del(&priv->in_flight);
				screen->ModifyPixmapHeader(pixmap, w, h, 0, 0, stride, NULL);
				intel_set_pixmap_private(pixmap, priv);
				return pixmap;
			}
		}

		priv = calloc(1, sizeof (struct intel_pixmap));
		if (priv == NULL) {
			fbDestroyPixmap(pixmap);
			return NullPixmap;
		}

		if (usage == UXA_CREATE_PIXMAP_FOR_MAP) {
			priv->busy = 0;
			priv->bo = drm_intel_bo_alloc(intel->bufmgr,
						      "pixmap", size, 0);
		} else {
			priv->busy = -1;
			priv->bo = drm_intel_bo_alloc_for_render(intel->bufmgr,
								 "pixmap",
								 size, 0);
		}
		if (!priv->bo) {
			free(priv);
			fbDestroyPixmap(pixmap);
			if (errno == EFBIG)
				return fbCreatePixmap(screen, w, h, depth, usage);
			return NullPixmap;
		}

		if (tiling != I915_TILING_NONE)
			drm_intel_bo_set_tiling(priv->bo, &tiling, stride);
		priv->stride = stride;
		priv->tiling = tiling;
		priv->offscreen = 1;

		screen->ModifyPixmapHeader(pixmap, w, h, 0, 0, stride, NULL);

		list_init(&priv->batch);
		list_init(&priv->flush);
		intel_set_pixmap_private(pixmap, priv);
	}

	return pixmap;
}

static Bool intel_uxa_destroy_pixmap(PixmapPtr pixmap)
{
	if (pixmap->refcnt == 1)
		intel_set_pixmap_bo(pixmap, NULL);
	fbDestroyPixmap(pixmap);
	return TRUE;
}

Bool intel_uxa_create_screen_resources(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	dri_bo *bo = intel->front_buffer;

	if (!uxa_resources_init(screen))
		return FALSE;

	drm_intel_gem_bo_map_gtt(bo);

	if (intel->use_shadow) {
		intel_shadow_create(intel);
	} else {
		PixmapPtr pixmap = screen->GetScreenPixmap(screen);
		intel_set_pixmap_bo(pixmap, bo);
		intel_get_pixmap_private(pixmap)->pinned = 1;
		screen->ModifyPixmapHeader(pixmap,
					   scrn->virtualX,
					   scrn->virtualY,
					   -1, -1,
					   intel->front_pitch,
					   NULL);
		scrn->displayWidth = intel->front_pitch / intel->cpp;
	}

	return TRUE;
}

static void
intel_limits_init(intel_screen_private *intel)
{
	/* Limits are described in the BLT engine chapter under Graphics Data Size
	 * Limitations, and the descriptions of SURFACE_STATE, 3DSTATE_BUFFER_INFO,
	 * 3DSTATE_DRAWING_RECTANGLE, 3DSTATE_MAP_INFO, and 3DSTATE_MAP_INFO.
	 *
	 * i845 through i965 limits 2D rendering to 65536 lines and pitch of 32768.
	 *
	 * i965 limits 3D surface to (2*element size)-aligned offset if un-tiled.
	 * i965 limits 3D surface to 4kB-aligned offset if tiled.
	 * i965 limits 3D surfaces to w,h of ?,8192.
	 * i965 limits 3D surface to pitch of 1B - 128kB.
	 * i965 limits 3D surface pitch alignment to 1 or 2 times the element size.
	 * i965 limits 3D surface pitch alignment to 512B if tiled.
	 * i965 limits 3D destination drawing rect to w,h of 8192,8192.
	 *
	 * i915 limits 3D textures to 4B-aligned offset if un-tiled.
	 * i915 limits 3D textures to ~4kB-aligned offset if tiled.
	 * i915 limits 3D textures to width,height of 2048,2048.
	 * i915 limits 3D textures to pitch of 16B - 8kB, in dwords.
	 * i915 limits 3D destination to ~4kB-aligned offset if tiled.
	 * i915 limits 3D destination to pitch of 16B - 8kB, in dwords, if un-tiled.
	 * i915 limits 3D destination to pitch 64B-aligned if used with depth.
	 * i915 limits 3D destination to pitch of 512B - 8kB, in tiles, if tiled.
	 * i915 limits 3D destination to POT aligned pitch if tiled.
	 * i915 limits 3D destination drawing rect to w,h of 2048,2048.
	 *
	 * i845 limits 3D textures to 4B-aligned offset if un-tiled.
	 * i845 limits 3D textures to ~4kB-aligned offset if tiled.
	 * i845 limits 3D textures to width,height of 2048,2048.
	 * i845 limits 3D textures to pitch of 4B - 8kB, in dwords.
	 * i845 limits 3D destination to 4B-aligned offset if un-tiled.
	 * i845 limits 3D destination to ~4kB-aligned offset if tiled.
	 * i845 limits 3D destination to pitch of 8B - 8kB, in dwords.
	 * i845 limits 3D destination drawing rect to w,h of 2048,2048.
	 *
	 * For the tiled issues, the only tiled buffer we draw to should be
	 * the front, which will have an appropriate pitch/offset already set up,
	 * so UXA doesn't need to worry.
	 */
	if (INTEL_INFO(intel)->gen >= 40) {
		intel->accel_pixmap_offset_alignment = 4 * 2;
		intel->accel_max_x = 8192;
		intel->accel_max_y = 8192;
	} else {
		intel->accel_pixmap_offset_alignment = 4;
		intel->accel_max_x = 2048;
		intel->accel_max_y = 2048;
	}
}

Bool intel_uxa_init(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

#if HAS_DIXREGISTERPRIVATEKEY
	if (!dixRegisterPrivateKey(&uxa_pixmap_index, PRIVATE_PIXMAP, 0))
#else
	if (!dixRequestPrivate(&uxa_pixmap_index, 0))
#endif
		return FALSE;

	intel_limits_init(intel);

	intel->uxa_driver = uxa_driver_alloc();
	if (intel->uxa_driver == NULL)
		return FALSE;

	memset(intel->uxa_driver, 0, sizeof(*intel->uxa_driver));

	intel->bufferOffset = 0;
	intel->uxa_driver->uxa_major = 1;
	intel->uxa_driver->uxa_minor = 0;

	intel->render_current_dest = NULL;
	intel->prim_offset = 0;
	intel->vertex_count = 0;
	intel->vertex_offset = 0;
	intel->vertex_used = 0;
	intel->floats_per_vertex = 0;
	intel->last_floats_per_vertex = 0;
	intel->vertex_bo = NULL;
	intel->surface_used = 0;
	intel->surface_reloc = 0;

	/* Solid fill */
	intel->uxa_driver->check_solid = intel_uxa_check_solid;
	intel->uxa_driver->prepare_solid = intel_uxa_prepare_solid;
	intel->uxa_driver->solid = intel_uxa_solid;
	intel->uxa_driver->done_solid = intel_uxa_done_solid;

	/* Copy */
	intel->uxa_driver->check_copy = intel_uxa_check_copy;
	intel->uxa_driver->prepare_copy = intel_uxa_prepare_copy;
	intel->uxa_driver->copy = intel_uxa_copy;
	intel->uxa_driver->done_copy = intel_uxa_done_copy;

	/* Composite */
	if (IS_GEN2(intel)) {
		intel->uxa_driver->check_composite = i830_check_composite;
		intel->uxa_driver->check_composite_target = i830_check_composite_target;
		intel->uxa_driver->check_composite_texture = i830_check_composite_texture;
		intel->uxa_driver->prepare_composite = i830_prepare_composite;
		intel->uxa_driver->composite = i830_composite;
		intel->uxa_driver->done_composite = i830_done_composite;

		intel->vertex_flush = i830_vertex_flush;
		intel->batch_commit_notify = i830_batch_commit_notify;
	} else if (IS_GEN3(intel)) {
		intel->uxa_driver->check_composite = i915_check_composite;
		intel->uxa_driver->check_composite_target = i915_check_composite_target;
		intel->uxa_driver->check_composite_texture = i915_check_composite_texture;
		intel->uxa_driver->prepare_composite = i915_prepare_composite;
		intel->uxa_driver->composite = i915_composite;
		intel->uxa_driver->done_composite = i830_done_composite;

		intel->vertex_flush = i915_vertex_flush;
		intel->batch_commit_notify = i915_batch_commit_notify;
	} else {
		intel->uxa_driver->check_composite = i965_check_composite;
		intel->uxa_driver->check_composite_texture = i965_check_composite_texture;
		intel->uxa_driver->prepare_composite = i965_prepare_composite;
		intel->uxa_driver->composite = i965_composite;
		intel->uxa_driver->done_composite = i830_done_composite;

		intel->vertex_flush = i965_vertex_flush;
		intel->batch_flush = i965_batch_flush;
		intel->batch_commit_notify = i965_batch_commit_notify;

		if (IS_GEN4(intel)) {
			intel->context_switch = gen4_context_switch;
		} else if (IS_GEN5(intel)) {
			intel->context_switch = gen5_context_switch;
		} else {
			intel->context_switch = gen6_context_switch;
		}
	}

	/* PutImage */
	intel->uxa_driver->put_image = intel_uxa_put_image;
	intel->uxa_driver->get_image = intel_uxa_get_image;

	intel->uxa_driver->prepare_access = intel_uxa_prepare_access;
	intel->uxa_driver->pixmap_is_offscreen = intel_uxa_pixmap_is_offscreen;

	screen->CreatePixmap = intel_uxa_create_pixmap;
	screen->DestroyPixmap = intel_uxa_destroy_pixmap;

	if (!uxa_driver_init(screen, intel->uxa_driver)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "UXA initialization failed\n");
		free(intel->uxa_driver);
		return FALSE;
	}

	uxa_set_fallback_debug(screen, intel->fallback_debug);
	uxa_set_force_fallback(screen, intel->force_fallback);

	return TRUE;
}
