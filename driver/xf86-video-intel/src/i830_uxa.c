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
#include "i830.h"
#include "i810_reg.h"
#include "i915_drm.h"
#include "brw_defines.h"
#include <string.h>
#include <sys/mman.h>
#include <errno.h>

const int I830CopyROP[16] = {
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

const int I830PatternROP[16] = {
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
ironlake_blt_workaround(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* Ironlake has a limitation that a 3D or Media command can't
	 * be the first command after a BLT, unless it's
	 * non-pipelined.  Instead of trying to track it and emit a
	 * command at the right time, we just emit a dummy
	 * non-pipelined 3D instruction after each blit.
	 */

	if (IS_IGDNG(intel)) {
		BEGIN_BATCH(2);
		OUT_BATCH(CMD_POLY_STIPPLE_OFFSET << 16);
		OUT_BATCH(0);
		ADVANCE_BATCH();
	}
}

Bool
i830_get_aperture_space(ScrnInfoPtr scrn, drm_intel_bo ** bo_table,
			int num_bos)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->batch_bo == NULL) {
		intel_debug_fallback(scrn, "VT inactive\n");
		return FALSE;
	}

	bo_table[0] = intel->batch_bo;
	if (drm_intel_bufmgr_check_aperture_space(bo_table, num_bos) != 0) {
		intel_batch_submit(scrn, FALSE);
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

static unsigned long i830_pixmap_pitch(PixmapPtr pixmap)
{
	return pixmap->devKind;
}

static int i830_pixmap_pitch_is_aligned(PixmapPtr pixmap)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

	return i830_pixmap_pitch(pixmap) %
	    intel->accel_pixmap_pitch_alignment == 0;
}

static unsigned int
i830_uxa_pixmap_compute_size(PixmapPtr pixmap,
			     int w, int h,
			     uint32_t *tiling,
			     int *stride)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int pitch, size;

	if (*tiling != I915_TILING_NONE) {
		/* First check whether tiling is necessary. */
		pitch = (w * pixmap->drawable.bitsPerPixel + 7) / 8;
		pitch = ROUND_TO(pitch, intel->accel_pixmap_pitch_alignment);
		size = pitch * ALIGN (h, 2);
		if (!IS_I965G(intel)) {
			/* Older hardware requires fences to be pot size
			 * aligned with a minimum of 1 MiB, so causes
			 * massive overallocation for small textures.
			 */
			if (size < 1024*1024/2)
				*tiling = I915_TILING_NONE;

			/* Gen 2/3 has a maximum stride for tiling of
			 * 8192 bytes.
			 */
			if (pitch > KB(8))
				*tiling = I915_TILING_NONE;
		} else if (size <= 4096) {
			/* Disable tiling beneath a page size, we will not see
			 * any benefit from reducing TLB misses and instead
			 * just incur extra cost when we require a fence.
			 */
			*tiling = I915_TILING_NONE;
		}
	}

	pitch = (w * pixmap->drawable.bitsPerPixel + 7) / 8;
	if (pitch <= 256)
		*tiling = I915_TILING_NONE;

	if (*tiling != I915_TILING_NONE) {
		int aligned_h;

		if (*tiling == I915_TILING_X)
			aligned_h = ALIGN(h, 8);
		else
			aligned_h = ALIGN(h, 32);

		*stride = i830_get_fence_pitch(intel,
					       ROUND_TO(pitch, 512),
					       *tiling);

		/* Round the object up to the size of the fence it will live in
		 * if necessary.  We could potentially make the kernel allocate
		 * a larger aperture space and just bind the subset of pages in,
		 * but this is easier and also keeps us out of trouble (as much)
		 * with drm_intel_bufmgr_check_aperture().
		 */
		size = i830_get_fence_size(intel, *stride * aligned_h);

		if (size > intel->max_tiling_size)
			*tiling = I915_TILING_NONE;
	}

	if (*tiling == I915_TILING_NONE) {
		/* Round the height up so that the GPU's access to a 2x2 aligned
		 * subspan doesn't address an invalid page offset beyond the
		 * end of the GTT.
		 */
		*stride = ROUND_TO(pitch, intel->accel_pixmap_pitch_alignment);
		size = *stride * ALIGN(h, 2);
	}

	return size;
}

static Bool
i830_uxa_check_solid(DrawablePtr drawable, int alu, Pixel planemask)
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
i830_uxa_prepare_solid(PixmapPtr pixmap, int alu, Pixel planemask, Pixel fg)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *bo_table[] = {
		NULL,		/* batch_bo */
		i830_get_pixmap_bo(pixmap),
	};

	if (!intel_check_pitch_2d(pixmap))
		return FALSE;

	if (!i830_pixmap_pitch_is_aligned(pixmap)) {
		intel_debug_fallback(scrn, "pixmap pitch not aligned");
		return FALSE;
	}

	if (!i830_get_aperture_space(scrn, bo_table, ARRAY_SIZE(bo_table)))
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

static void i830_uxa_solid(PixmapPtr pixmap, int x1, int y1, int x2, int y2)
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

	pitch = i830_pixmap_pitch(pixmap);

	{
		BEGIN_BATCH(6);

		cmd = XY_COLOR_BLT_CMD;

		if (pixmap->drawable.bitsPerPixel == 32)
			cmd |=
			    XY_COLOR_BLT_WRITE_ALPHA | XY_COLOR_BLT_WRITE_RGB;

		if (IS_I965G(intel) && i830_pixmap_tiled(pixmap)) {
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

	ironlake_blt_workaround(scrn);
}

static void i830_uxa_done_solid(PixmapPtr pixmap)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];

	i830_debug_flush(scrn);
}

/**
 * TODO:
 *   - support planemask using FULL_BLT_CMD?
 */
static Bool
i830_uxa_check_copy(PixmapPtr source, PixmapPtr dest,
		    int alu, Pixel planemask)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);


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
i830_uxa_prepare_copy(PixmapPtr source, PixmapPtr dest, int xdir,
		      int ydir, int alu, Pixel planemask)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *bo_table[] = {
		NULL,		/* batch_bo */
		i830_get_pixmap_bo(source),
		i830_get_pixmap_bo(dest),
	};

	if (!i830_get_aperture_space(scrn, bo_table, ARRAY_SIZE(bo_table)))
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
i830_uxa_copy(PixmapPtr dest, int src_x1, int src_y1, int dst_x1,
	      int dst_y1, int w, int h)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	uint32_t cmd;
	int dst_x2, dst_y2;
	unsigned int dst_pitch, src_pitch;

	dst_x2 = dst_x1 + w;
	dst_y2 = dst_y1 + h;

	dst_pitch = i830_pixmap_pitch(dest);
	src_pitch = i830_pixmap_pitch(intel->render_source);

	{
		BEGIN_BATCH(8);

		cmd = XY_SRC_COPY_BLT_CMD;

		if (dest->drawable.bitsPerPixel == 32)
			cmd |=
			    XY_SRC_COPY_BLT_WRITE_ALPHA |
			    XY_SRC_COPY_BLT_WRITE_RGB;

		if (IS_I965G(intel)) {
			if (i830_pixmap_tiled(dest)) {
				assert((dst_pitch % 512) == 0);
				dst_pitch >>= 2;
				cmd |= XY_SRC_COPY_BLT_DST_TILED;
			}

			if (i830_pixmap_tiled(intel->render_source)) {
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
	ironlake_blt_workaround(scrn);
}

static void i830_uxa_done_copy(PixmapPtr dest)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];

	i830_debug_flush(scrn);
}

/**
 * Do any cleanup from the Composite operation.
 *
 * This is shared between i830 through i965.
 */
void i830_done_composite(PixmapPtr dest)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->vertex_flush)
		intel->vertex_flush(intel);

	i830_debug_flush(scrn);
}

#define xFixedToFloat(val) \
	((float)xFixedToInt(val) + ((float)xFixedFrac(val) / 65536.0))

static Bool
_i830_transform_point(PictTransformPtr transform,
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
i830_get_transformed_coordinates(int x, int y, PictTransformPtr transform,
				 float *x_out, float *y_out)
{
	if (transform == NULL) {
		*x_out = x;
		*y_out = y;
	} else {
		float result[3];

		if (!_i830_transform_point
		    (transform, (float)x, (float)y, result))
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
i830_get_transformed_coordinates_3d(int x, int y, PictTransformPtr transform,
				    float *x_out, float *y_out, float *w_out)
{
	if (transform == NULL) {
		*x_out = x;
		*y_out = y;
		*w_out = 1;
	} else {
		float result[3];

		if (!_i830_transform_point
		    (transform, (float)x, (float)y, result))
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
Bool i830_transform_is_affine(PictTransformPtr t)
{
	if (t == NULL)
		return TRUE;
	return t->matrix[2][0] == 0 && t->matrix[2][1] == 0;
}

dri_bo *i830_get_pixmap_bo(PixmapPtr pixmap)
{
	struct intel_pixmap *intel;

	intel = i830_get_pixmap_intel(pixmap);
	if (intel == NULL)
		return NULL;

	return intel->bo;
}

void i830_set_pixmap_bo(PixmapPtr pixmap, dri_bo * bo)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_pixmap *priv;

	priv = i830_get_pixmap_intel(pixmap);

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
		priv->stride = i830_pixmap_pitch(pixmap);

		ret = drm_intel_bo_get_tiling(bo,
					      &tiling,
					      &swizzle_mode);
		if (ret != 0) {
			FatalError("Couldn't get tiling on bo %p: %s\n",
				   bo, strerror(-ret));
		}

		priv->tiling = tiling;
		priv->busy = -1;
	} else {
		if (priv != NULL) {
			free(priv);
			priv = NULL;
		}
	}

  BAIL:
	i830_set_pixmap_intel(pixmap, priv);
}

static Bool i830_uxa_prepare_access(PixmapPtr pixmap, uxa_access_t access)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_pixmap *priv = i830_get_pixmap_intel(pixmap);
	dri_bo *bo = priv->bo;
	int ret;

	if (!list_is_empty(&priv->batch) &&
	    (access == UXA_ACCESS_RW || priv->batch_write))
		intel_batch_submit(scrn, FALSE);

	if (bo->size > intel->max_gtt_map_size) {
		ret = dri_bo_map(bo, access == UXA_ACCESS_RW);
		if (ret != 0) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "%s: bo map failed: %s\n",
				   __FUNCTION__,
				   strerror(-ret));
			return FALSE;
		}
	} else {
		ret = drm_intel_gem_bo_map_gtt(bo);
		if (ret != 0) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "%s: gtt bo map failed: %s\n",
				   __FUNCTION__,
				   strerror(-ret));
			return FALSE;
		}
	}
	pixmap->devPrivate.ptr = bo->virtual;
	priv->busy = 0;

	return TRUE;
}

static void i830_uxa_finish_access(PixmapPtr pixmap)
{
	dri_bo *bo = i830_get_pixmap_bo(pixmap);
	ScreenPtr screen = pixmap->drawable.pScreen;
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (bo == intel->front_buffer->bo)
		intel->need_flush = TRUE;

	if (bo->size > intel->max_gtt_map_size)
		dri_bo_unmap(bo);
	else
		drm_intel_gem_bo_unmap_gtt(bo);
	pixmap->devPrivate.ptr = NULL;
}

static Bool i830_uxa_pixmap_put_image(PixmapPtr pixmap,
				      char *src, int src_pitch,
				      int x, int y, int w, int h)
{
	struct intel_pixmap *priv = i830_get_pixmap_intel(pixmap);
	int stride = i830_pixmap_pitch(pixmap);
	int ret = FALSE;

	if (src_pitch == stride && w == pixmap->drawable.width && priv->tiling == I915_TILING_NONE) {
		ret = drm_intel_bo_subdata(priv->bo, y * stride, stride * h, src) == 0;
	} else if (drm_intel_gem_bo_map_gtt(priv->bo) == 0) {
		char *dst = priv->bo->virtual;
		int cpp = pixmap->drawable.bitsPerPixel/8;
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

static Bool i830_uxa_put_image(PixmapPtr pixmap,
			       int x, int y,
			       int w, int h,
			       char *src, int src_pitch)
{
	struct intel_pixmap *priv;

	priv = i830_get_pixmap_intel(pixmap);
	if (!intel_pixmap_is_busy(priv)) {
		/* bo is not busy so can be replaced without a stall, upload in-place. */
		return i830_uxa_pixmap_put_image(pixmap, src, src_pitch, x, y, w, h);
	} else {
		ScreenPtr screen = pixmap->drawable.pScreen;

		if (x == 0 && y == 0 &&
		    w == pixmap->drawable.width &&
		    h == pixmap->drawable.height)
		{
			intel_screen_private *intel = intel_get_screen_private(xf86Screens[screen->myNum]);
			uint32_t tiling = priv->tiling;
			int size, stride;
			dri_bo *bo;

			/* Replace busy bo. */
			size = i830_uxa_pixmap_compute_size (pixmap, w, h,
							     &tiling, &stride);
			if (size > intel->max_gtt_map_size)
				return FALSE;

			bo = drm_intel_bo_alloc(intel->bufmgr, "pixmap", size, 0);
			if (bo == NULL)
				return FALSE;

			if (tiling != I915_TILING_NONE)
				drm_intel_bo_set_tiling(bo, &tiling, stride);

			screen->ModifyPixmapHeader(pixmap,
						   w, h,
						   0, 0,
						   stride, NULL);
			i830_set_pixmap_bo(pixmap, bo);
			dri_bo_unreference(bo);

			return i830_uxa_pixmap_put_image(pixmap, src, src_pitch, 0, 0, w, h);
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

			ret = i830_uxa_pixmap_put_image(scratch, src, src_pitch, 0, 0, w, h);
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

static Bool i830_uxa_pixmap_get_image(PixmapPtr pixmap,
				      int x, int y, int w, int h,
				      char *dst, int dst_pitch)
{
	struct intel_pixmap *priv = i830_get_pixmap_intel(pixmap);
	int stride = i830_pixmap_pitch(pixmap);

	if (dst_pitch == stride && w == pixmap->drawable.width) {
		return drm_intel_bo_get_subdata(priv->bo, y * stride, stride * h, dst) == 0;
	} else {
		char *src;
		int cpp;

		if (drm_intel_bo_map(priv->bo, FALSE))
		    return FALSE;

		cpp = pixmap->drawable.bitsPerPixel/8;
		src = (char *) priv->bo->virtual + y * stride + x * cpp;
		w *= cpp;
		do {
			memcpy(dst, src, w);
			src += stride;
			dst += dst_pitch;
		} while (--h);

		drm_intel_bo_unmap(priv->bo);

		return TRUE;
	}
}

static Bool i830_uxa_get_image(PixmapPtr pixmap,
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

	priv = i830_get_pixmap_intel(pixmap);
	if (intel_pixmap_is_busy(priv) || priv->tiling != I915_TILING_NONE) {
		ScreenPtr screen = pixmap->drawable.pScreen;
		GCPtr gc;

		/* Copy to a linear buffer and pull.  */
		scratch = screen->CreatePixmap(screen, w, h,
					       pixmap->drawable.depth,
					       INTEL_CREATE_PIXMAP_TILING_NONE);
		if (!scratch)
			return FALSE;

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

		intel_batch_submit(xf86Screens[screen->myNum], FALSE);

		x = y = 0;
		pixmap = scratch;
	}

	ret = i830_uxa_pixmap_get_image(pixmap, x, y, w, h, dst, dst_pitch);

	if (scratch)
		scratch->drawable.pScreen->DestroyPixmap(scratch);

	return ret;

}

void i830_uxa_block_handler(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (intel->need_flush) {
		dri_bo_wait_rendering(intel->front_buffer->bo);
		intel->need_flush = FALSE;
	}
}

static Bool i830_uxa_pixmap_is_offscreen(PixmapPtr pixmap)
{
	ScreenPtr screen = pixmap->drawable.pScreen;

	/* The front buffer is always in memory and pinned */
	if (screen->GetScreenPixmap(screen) == pixmap)
		return TRUE;

	return i830_get_pixmap_bo(pixmap) != NULL;
}

static PixmapPtr
i830_uxa_create_pixmap(ScreenPtr screen, int w, int h, int depth,
		       unsigned usage)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	PixmapPtr pixmap;

	if (w > 32767 || h > 32767)
		return NullPixmap;

	if (depth == 1)
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
		if (usage == INTEL_CREATE_PIXMAP_TILING_Y)
			tiling = I915_TILING_Y;
		if (usage == UXA_CREATE_PIXMAP_FOR_MAP || usage == INTEL_CREATE_PIXMAP_TILING_NONE)
			tiling = I915_TILING_NONE;

		if (tiling != I915_TILING_NONE) {
		    if (h <= 4)
			tiling = I915_TILING_NONE;
		    if (h <= 16 && tiling == I915_TILING_Y)
			tiling = I915_TILING_X;
		}
		size = i830_uxa_pixmap_compute_size(pixmap, w, h, &tiling, &stride);

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
				i830_set_pixmap_intel(pixmap, priv);
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

		screen->ModifyPixmapHeader(pixmap, w, h, 0, 0, stride, NULL);

		list_init(&priv->batch);
		list_init(&priv->flush);
		i830_set_pixmap_intel(pixmap, priv);
	}

	return pixmap;
}

static Bool i830_uxa_destroy_pixmap(PixmapPtr pixmap)
{
	if (pixmap->refcnt == 1)
		i830_set_pixmap_bo(pixmap, NULL);
	fbDestroyPixmap(pixmap);
	return TRUE;
}


void i830_uxa_create_screen_resources(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	dri_bo *bo = intel->front_buffer->bo;

	if (bo != NULL) {
		PixmapPtr pixmap = screen->GetScreenPixmap(screen);
		i830_set_pixmap_bo(pixmap, bo);
		i830_get_pixmap_intel(pixmap)->busy = 1;
	}
}

Bool i830_uxa_init(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

#if HAS_DIXREGISTERPRIVATEKEY
	if (!dixRegisterPrivateKey(&uxa_pixmap_index, PRIVATE_PIXMAP, 0))
#else
	if (!dixRequestPrivate(&uxa_pixmap_index, 0))
#endif
		return FALSE;

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
	intel->floats_per_vertex = 0;
	intel->last_floats_per_vertex = 0;
	intel->vertex_bo = NULL;

	/* Solid fill */
	intel->uxa_driver->check_solid = i830_uxa_check_solid;
	intel->uxa_driver->prepare_solid = i830_uxa_prepare_solid;
	intel->uxa_driver->solid = i830_uxa_solid;
	intel->uxa_driver->done_solid = i830_uxa_done_solid;

	/* Copy */
	intel->uxa_driver->check_copy = i830_uxa_check_copy;
	intel->uxa_driver->prepare_copy = i830_uxa_prepare_copy;
	intel->uxa_driver->copy = i830_uxa_copy;
	intel->uxa_driver->done_copy = i830_uxa_done_copy;

	/* Composite */
	if (!IS_I9XX(intel)) {
		intel->uxa_driver->check_composite = i830_check_composite;
		intel->uxa_driver->check_composite_target = i830_check_composite_target;
		intel->uxa_driver->check_composite_texture = i830_check_composite_texture;
		intel->uxa_driver->prepare_composite = i830_prepare_composite;
		intel->uxa_driver->composite = i830_composite;
		intel->uxa_driver->done_composite = i830_done_composite;
	} else if (IS_I915G(intel) || IS_I915GM(intel) ||
		   IS_I945G(intel) || IS_I945GM(intel) || IS_G33CLASS(intel)) {
		intel->uxa_driver->check_composite = i915_check_composite;
		intel->uxa_driver->check_composite_target = i915_check_composite_target;
		intel->uxa_driver->check_composite_texture = i915_check_composite_texture;
		intel->uxa_driver->prepare_composite = i915_prepare_composite;
		intel->uxa_driver->composite = i915_composite;
		intel->uxa_driver->done_composite = i830_done_composite;
	} else {
		intel->uxa_driver->check_composite = i965_check_composite;
		intel->uxa_driver->check_composite_texture = i965_check_composite_texture;
		intel->uxa_driver->prepare_composite = i965_prepare_composite;
		intel->uxa_driver->composite = i965_composite;
		intel->uxa_driver->done_composite = i830_done_composite;
	}

	/* PutImage */
	intel->uxa_driver->put_image = i830_uxa_put_image;
	intel->uxa_driver->get_image = i830_uxa_get_image;

	intel->uxa_driver->prepare_access = i830_uxa_prepare_access;
	intel->uxa_driver->finish_access = i830_uxa_finish_access;
	intel->uxa_driver->pixmap_is_offscreen = i830_uxa_pixmap_is_offscreen;

	screen->CreatePixmap = i830_uxa_create_pixmap;
	screen->DestroyPixmap = i830_uxa_destroy_pixmap;

	if (!uxa_driver_init(screen, intel->uxa_driver)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "UXA initialization failed\n");
		free(intel->uxa_driver);
		return FALSE;
	}

	uxa_set_fallback_debug(screen, intel->fallback_debug);

	return TRUE;
}
