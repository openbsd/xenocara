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

static int uxa_pixmap_index;

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
	int pitch, pitch_align;
	int size;

	if (*tiling != I915_TILING_NONE) {
		/* First check whether tiling is necessary. */
		pitch_align = intel->accel_pixmap_pitch_alignment;
		size = ROUND_TO((w * pixmap->drawable.bitsPerPixel + 7) / 8,
				pitch_align) * ALIGN (h, 2);
		if (!IS_I965G(intel)) {
			/* Older hardware requires fences to be pot size
			 * aligned with a minimum of 1 MiB, so causes
			 * massive overallocation for small textures.
			 */
			if (size < 1024*1024/2)
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
  repeat:
	if (*tiling == I915_TILING_NONE) {
		pitch_align = intel->accel_pixmap_pitch_alignment;
	} else {
		pitch_align = 512;
	}

	*stride = ROUND_TO(pitch, pitch_align);

	if (*tiling == I915_TILING_NONE) {
		/* Round the height up so that the GPU's access to a 2x2 aligned
		 * subspan doesn't address an invalid page offset beyond the
		 * end of the GTT.
		 */
		size = *stride * ALIGN(h, 2);
	} else {
		int aligned_h = h;
		if (*tiling == I915_TILING_X)
			aligned_h = ALIGN(h, 8);
		else
			aligned_h = ALIGN(h, 32);

		*stride = i830_get_fence_pitch(intel, *stride, *tiling);
		/* Round the object up to the size of the fence it will live in
		 * if necessary.  We could potentially make the kernel allocate
		 * a larger aperture space and just bind the subset of pages in,
		 * but this is easier and also keeps us out of trouble (as much)
		 * with drm_intel_bufmgr_check_aperture().
		 */
		size = i830_get_fence_size(intel, *stride * aligned_h);
		assert(size >= *stride * aligned_h);
	}

	if (*tiling != I915_TILING_NONE && size > intel->max_tiling_size) {
		*tiling = I915_TILING_NONE;
		goto repeat;
	}

	return size;
}

/**
 * Sets up hardware state for a series of solid fills.
 */
static Bool
i830_uxa_prepare_solid(PixmapPtr pixmap, int alu, Pixel planemask, Pixel fg)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	unsigned long pitch;
	drm_intel_bo *bo_table[] = {
		NULL,		/* batch_bo */
		i830_get_pixmap_bo(pixmap),
	};

	if (!UXA_PM_IS_SOLID(&pixmap->drawable, planemask)) {
		intel_debug_fallback(scrn, "planemask is not solid\n");
		return FALSE;
	}

	if (pixmap->drawable.bitsPerPixel == 24) {
		intel_debug_fallback(scrn, "solid 24bpp unsupported!\n");
		return FALSE;
	}

	if (pixmap->drawable.bitsPerPixel < 8) {
		intel_debug_fallback(scrn, "under 8bpp pixmaps unsupported\n");
		return FALSE;
	}

	if (!intel_check_pitch_2d(pixmap))
		return FALSE;

	pitch = i830_pixmap_pitch(pixmap);

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
		if (pixmap->drawable.depth == 24)
		    fg |= 0xff000000;
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

	if (!UXA_PM_IS_SOLID(&source->drawable, planemask)) {
		intel_debug_fallback(scrn, "planemask is not solid");
		return FALSE;
	}

	if (dest->drawable.bitsPerPixel < 8) {
		intel_debug_fallback(scrn, "under 8bpp pixmaps unsupported\n");
		return FALSE;
	}

	if (!i830_get_aperture_space(scrn, bo_table, ARRAY_SIZE(bo_table)))
		return FALSE;

	if (!intel_check_pitch_2d(source))
		return FALSE;
	if (!intel_check_pitch_2d(dest))
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

struct intel_pixmap *i830_get_pixmap_intel(PixmapPtr pixmap)
{
	return dixLookupPrivate(&pixmap->devPrivates, &uxa_pixmap_index);
}

static void i830_uxa_set_pixmap_intel(PixmapPtr pixmap, struct intel_pixmap *intel)
{
	dixSetPrivate(&pixmap->devPrivates, &uxa_pixmap_index, intel);
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

	if (priv != NULL) {
		if (list_is_empty(&priv->batch)) {
			dri_bo_unreference(priv->bo);
		} else {
			list_add(&priv->in_flight, &intel->in_flight);
			priv = NULL;
		}
	}

	if (bo != NULL) {
		uint32_t swizzle_mode;
		int ret;

		if (priv == NULL) {
			priv = xcalloc(1, sizeof (struct intel_pixmap));
			if (priv == NULL)
				goto BAIL;

			list_init(&priv->batch);
			list_init(&priv->flush);
		}

		dri_bo_reference(bo);
		priv->bo = bo;
		priv->stride = i830_pixmap_pitch(pixmap);

		ret = drm_intel_bo_get_tiling(bo,
					      &priv->tiling,
					      &swizzle_mode);
		if (ret != 0) {
			FatalError("Couldn't get tiling on bo %p: %s\n",
				   bo, strerror(-ret));
		}
	} else {
		if (priv != NULL) {
			xfree(priv);
			priv = NULL;
		}
	}

  BAIL:
	i830_uxa_set_pixmap_intel(pixmap, priv);
}

static Bool i830_uxa_prepare_access(PixmapPtr pixmap, uxa_access_t access)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_pixmap *priv = i830_get_pixmap_intel(pixmap);
	dri_bo *bo = priv->bo;
	int ret;

	if (!list_is_empty(&priv->batch) &&
	    (access == UXA_ACCESS_RW || priv->batch_write_domain))
		intel_batch_submit(scrn);

	if (bo) {
		if (intel->kernel_exec_fencing) {
			if (bo->size > intel->max_gtt_map_size || !intel->have_gem) {
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
		} else {
			if (drm_intel_bo_pin(bo, 4096) != 0)
				return FALSE;
			drm_intel_gem_bo_start_gtt_access(bo, access ==
			    UXA_ACCESS_RW);
			pixmap->devPrivate.ptr = intel->FbBase + bo->offset;
		}
	} else
		i830_wait_ring_idle(scrn);

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

	if (intel->kernel_exec_fencing) {
		if (bo->size > intel->max_gtt_map_size)
			dri_bo_unmap(bo);
		else
			drm_intel_gem_bo_unmap_gtt(bo);
		pixmap->devPrivate.ptr = NULL;
	}
}

static Bool i830_bo_put_image(PixmapPtr pixmap, dri_bo *bo, char *src, int src_pitch, int w, int h)
{
	int stride = i830_pixmap_pitch(pixmap);

	/* fill alpha channel */
	if (pixmap->drawable.depth == 24) {
		pixman_image_t *src_image, *dst_image;

		src_image = pixman_image_create_bits (PIXMAN_x8r8g8b8,
						      w, h,
						      (uint32_t *) src, src_pitch);

		dst_image = pixman_image_create_bits (PIXMAN_a8r8g8b8,
						      w, h,
						      (uint32_t *) bo->virtual, stride);

		if (src_image && dst_image)
			pixman_image_composite (PictOpSrc,
						src_image, NULL, dst_image,
						0, 0,
						0, 0,
						0, 0,
						w, h);

		if (src_image)
			pixman_image_unref (src_image);

		if (dst_image)
			pixman_image_unref (dst_image);

		if (src_image == NULL || dst_image == NULL)
			return FALSE;
	} else if (src_pitch == stride) {
		memcpy (bo->virtual, src, stride * h);
	} else {
		char *dst = bo->virtual;
		int row_length = w * pixmap->drawable.bitsPerPixel/8;
		int num_rows = h;
		while (num_rows--) {
			memcpy (dst, src, row_length);
			src += src_pitch;
			dst += stride;
		}
	}

	return TRUE;
}

static Bool
i830_uxa_pixmap_swap_bo_with_image(PixmapPtr pixmap,
				   char *src, int src_pitch)
{
	ScrnInfoPtr scrn = xf86Screens[pixmap->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	struct intel_pixmap *priv;
	dri_bo *bo;
	uint32_t tiling = I915_TILING_X;
	int stride;
	int w = pixmap->drawable.width;
	int h = pixmap->drawable.height;
	Bool ret;

	priv = i830_get_pixmap_intel(pixmap);

	if (priv->batch_read_domains || drm_intel_bo_busy(priv->bo)) {
		unsigned int size;

		size = i830_uxa_pixmap_compute_size (pixmap, w, h,
						     &tiling, &stride);
		if (size > intel->max_gtt_map_size)
			return FALSE;

		bo = drm_intel_bo_alloc(intel->bufmgr, "pixmap", size, 0);
		if (bo == NULL)
			return FALSE;

		if (tiling != I915_TILING_NONE)
			drm_intel_bo_set_tiling(bo, &tiling, stride);

		dri_bo_unreference(priv->bo);
		priv->bo = bo;
		priv->tiling = tiling;
		priv->batch_read_domains = priv->batch_write_domain = 0;
		priv->flush_read_domains = priv->flush_write_domain = 0;
		list_del(&priv->batch);
		list_del(&priv->flush);
		pixmap->drawable.pScreen->ModifyPixmapHeader(pixmap,
							     w, h,
							     0, 0,
							     stride, NULL);
	} else {
		bo = priv->bo;
		stride = i830_pixmap_pitch(pixmap);
	}

	if (drm_intel_gem_bo_map_gtt(bo)) {
		xf86DrvMsg(scrn->scrnIndex, X_WARNING,
			   "%s: bo map failed\n", __FUNCTION__);
		return FALSE;
	}

	ret = i830_bo_put_image(pixmap, bo, src, src_pitch, w, h);

	drm_intel_gem_bo_unmap_gtt(bo);

	return ret;
}

static Bool i830_uxa_put_image(PixmapPtr pixmap,
			       int x, int y,
			       int w, int h,
			       char *src, int src_pitch)
{
	ScreenPtr screen = pixmap->drawable.pScreen;
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	PixmapPtr scratch;
	struct intel_pixmap *priv;
	GCPtr gc;
	Bool ret;

	if (x == 0 && y == 0 &&
	    w == pixmap->drawable.width &&
	    h == pixmap->drawable.height)
	{
		/* Replace GPU hot bo with new CPU data. */
		return i830_uxa_pixmap_swap_bo_with_image(pixmap,
							  src, src_pitch);
	}

	priv = i830_get_pixmap_intel(pixmap);
	if (priv->batch_read_domains ||
	    drm_intel_bo_busy(priv->bo) ||
	    pixmap->drawable.depth == 24) {
		dri_bo *bo;

		/* Partial replacement, copy incoming image to a bo and blit. */
		scratch = (*screen->CreatePixmap)(screen, w, h,
						  pixmap->drawable.depth,
						  UXA_CREATE_PIXMAP_FOR_MAP);
		if (!scratch)
			return FALSE;

		bo = i830_get_pixmap_bo(scratch);
		if (drm_intel_gem_bo_map_gtt(bo)) {
			(*screen->DestroyPixmap) (scratch);
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "%s: bo map failed\n", __FUNCTION__);
			return FALSE;
		}

		ret = i830_bo_put_image(scratch, bo, src, src_pitch, w, h);

		drm_intel_gem_bo_unmap_gtt(bo);

		if (ret) {
			gc = GetScratchGC(pixmap->drawable.depth, screen);
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
	} else {
		/* bo is not busy so can be mapped without a stall, upload in-place. */
		if (drm_intel_gem_bo_map_gtt(priv->bo)) {
			xf86DrvMsg(scrn->scrnIndex, X_WARNING,
				   "%s: bo map failed\n", __FUNCTION__);
			return FALSE;
		}

		pixman_blt((uint32_t *)src, priv->bo->virtual,
			   src_pitch / sizeof(uint32_t),
			   pixmap->devKind / sizeof(uint32_t),
			   pixmap->drawable.bitsPerPixel,
			   pixmap->drawable.bitsPerPixel,
			   0, 0,
			   x, y,
			   w, h);

		drm_intel_gem_bo_unmap_gtt(priv->bo);
		ret = TRUE;
	}

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
		if (usage == UXA_CREATE_PIXMAP_FOR_MAP)
			tiling = I915_TILING_NONE;

		if (tiling != I915_TILING_NONE) {
		    if (h <= 4)
			tiling = I915_TILING_NONE;
		    if (h <= 16 && tiling == I915_TILING_Y)
			tiling = I915_TILING_X;
		}
		size = i830_uxa_pixmap_compute_size(pixmap, w, h, &tiling, &stride);

		/* Fail very large allocations on 32-bit systems.  Large BOs will
		 * tend to hit SW fallbacks frequently, and also will tend to fail
		 * to successfully map when doing SW fallbacks because we overcommit
		 * address space for BO access.
		 *
		 * Note that size should fit in 32 bits.  We throw out >32767x32767x4,
		 * and pitch alignment could get us up to 32768x32767x4.
		 */
		if (sizeof(unsigned long) == 4 &&
		    size > (unsigned int)(1024 * 1024 * 1024)) {
			fbDestroyPixmap(pixmap);
			return NullPixmap;
		}

		/* Perform a premilinary search for an in-flight bo */
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
				if (priv->tiling == tiling &&
				    priv->stride >= stride &&
				    priv->bo->size >= priv->stride * aligned_h) {
					list_del(&priv->in_flight);
					screen->ModifyPixmapHeader(pixmap, w, h, 0, 0, priv->stride, NULL);
					i830_uxa_set_pixmap_intel(pixmap, priv);
					return pixmap;
				}
			}
		}

		priv = xcalloc(1, sizeof (struct intel_pixmap));
		if (priv == NULL) {
			fbDestroyPixmap(pixmap);
			return NullPixmap;
		}

		if (usage == UXA_CREATE_PIXMAP_FOR_MAP)
			priv->bo = drm_intel_bo_alloc(intel->bufmgr,
						      "pixmap", size, 0);
		else
			priv->bo = drm_intel_bo_alloc_for_render(intel->bufmgr,
								 "pixmap",
								 size, 0);
		if (!priv->bo) {
			xfree(priv);
			fbDestroyPixmap(pixmap);
			if (errno == EFBIG)
				return fbCreatePixmap(screen, w, h, depth, usage);
			return NullPixmap;
		}

		priv->stride = stride;
		priv->tiling = tiling;
		if (priv->tiling != I915_TILING_NONE)
			drm_intel_bo_set_tiling(priv->bo,
						&priv->tiling,
						stride);

		screen->ModifyPixmapHeader(pixmap, w, h, 0, 0, stride, NULL);

		list_init(&priv->batch);
		list_init(&priv->flush);
		i830_uxa_set_pixmap_intel(pixmap, priv);
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
	}
}

Bool i830_uxa_init(ScreenPtr screen)
{
	ScrnInfoPtr scrn = xf86Screens[screen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (!dixRequestPrivate(&uxa_pixmap_index, 0))
		return FALSE;

	intel->uxa_driver = uxa_driver_alloc();
	if (intel->uxa_driver == NULL)
		return FALSE;

	memset(intel->uxa_driver, 0, sizeof(*intel->uxa_driver));

	intel->bufferOffset = 0;
	intel->uxa_driver->uxa_major = 1;
	intel->uxa_driver->uxa_minor = 0;

	/* Solid fill */
	intel->uxa_driver->prepare_solid = i830_uxa_prepare_solid;
	intel->uxa_driver->solid = i830_uxa_solid;
	intel->uxa_driver->done_solid = i830_uxa_done_solid;

	/* Copy */
	intel->uxa_driver->prepare_copy = i830_uxa_prepare_copy;
	intel->uxa_driver->copy = i830_uxa_copy;
	intel->uxa_driver->done_copy = i830_uxa_done_copy;

	/* Composite */
	if (!IS_I9XX(intel)) {
		intel->uxa_driver->check_composite = i830_check_composite;
		intel->uxa_driver->prepare_composite = i830_prepare_composite;
		intel->uxa_driver->composite = i830_composite;
		intel->uxa_driver->done_composite = i830_done_composite;
	} else if (IS_I915G(intel) || IS_I915GM(intel) ||
		   IS_I945G(intel) || IS_I945GM(intel) || IS_G33CLASS(intel)) {
		intel->uxa_driver->check_composite = i915_check_composite;
		intel->uxa_driver->prepare_composite = i915_prepare_composite;
		intel->uxa_driver->composite = i915_composite;
		intel->uxa_driver->done_composite = i830_done_composite;
	} else {
		intel->uxa_driver->check_composite = i965_check_composite;
		intel->uxa_driver->prepare_composite = i965_prepare_composite;
		intel->uxa_driver->composite = i965_composite;
		intel->uxa_driver->done_composite = i830_done_composite;
	}

	/* PutImage */
	intel->uxa_driver->put_image = i830_uxa_put_image;

	intel->uxa_driver->prepare_access = i830_uxa_prepare_access;
	intel->uxa_driver->finish_access = i830_uxa_finish_access;
	intel->uxa_driver->pixmap_is_offscreen = i830_uxa_pixmap_is_offscreen;

	if (!uxa_driver_init(screen, intel->uxa_driver)) {
		xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			   "UXA initialization failed\n");
		xfree(intel->uxa_driver);
		return FALSE;
	}

	screen->CreatePixmap = i830_uxa_create_pixmap;
	screen->DestroyPixmap = i830_uxa_destroy_pixmap;

	uxa_set_fallback_debug(screen, intel->fallback_debug);

	return TRUE;
}
