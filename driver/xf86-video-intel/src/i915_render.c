/*
 * Copyright © 2006 Intel Corporation
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
 *    Wang Zhenyu <zhenyu.z.wang@intel.com>
 *    Eric Anholt <eric@anholt.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "intel.h"
#include "i915_reg.h"
#include "i915_3d.h"

struct formatinfo {
	int fmt;
	uint32_t card_fmt;
};

struct blendinfo {
	Bool dst_alpha;
	Bool src_alpha;
	uint32_t src_blend;
	uint32_t dst_blend;
};

static struct blendinfo i915_blend_op[] = {
	/* Clear */
	{0, 0, BLENDFACT_ZERO, BLENDFACT_ZERO},
	/* Src */
	{0, 0, BLENDFACT_ONE, BLENDFACT_ZERO},
	/* Dst */
	{0, 0, BLENDFACT_ZERO, BLENDFACT_ONE},
	/* Over */
	{0, 1, BLENDFACT_ONE, BLENDFACT_INV_SRC_ALPHA},
	/* OverReverse */
	{1, 0, BLENDFACT_INV_DST_ALPHA, BLENDFACT_ONE},
	/* In */
	{1, 0, BLENDFACT_DST_ALPHA, BLENDFACT_ZERO},
	/* InReverse */
	{0, 1, BLENDFACT_ZERO, BLENDFACT_SRC_ALPHA},
	/* Out */
	{1, 0, BLENDFACT_INV_DST_ALPHA, BLENDFACT_ZERO},
	/* OutReverse */
	{0, 1, BLENDFACT_ZERO, BLENDFACT_INV_SRC_ALPHA},
	/* Atop */
	{1, 1, BLENDFACT_DST_ALPHA, BLENDFACT_INV_SRC_ALPHA},
	/* AtopReverse */
	{1, 1, BLENDFACT_INV_DST_ALPHA, BLENDFACT_SRC_ALPHA},
	/* Xor */
	{1, 1, BLENDFACT_INV_DST_ALPHA, BLENDFACT_INV_SRC_ALPHA},
	/* Add */
	{0, 0, BLENDFACT_ONE, BLENDFACT_ONE},
};

static struct formatinfo i915_tex_formats[] = {
	{PICT_a8, MAPSURF_8BIT | MT_8BIT_A8},
	{PICT_a8r8g8b8, MAPSURF_32BIT | MT_32BIT_ARGB8888},
	{PICT_x8r8g8b8, MAPSURF_32BIT | MT_32BIT_XRGB8888},
	{PICT_a8b8g8r8, MAPSURF_32BIT | MT_32BIT_ABGR8888},
	{PICT_x8b8g8r8, MAPSURF_32BIT | MT_32BIT_XBGR8888},
#if XORG_VERSION_CURRENT >= 10699900
	{PICT_a2r10g10b10, MAPSURF_32BIT | MT_32BIT_ARGB2101010},
	{PICT_a2b10g10r10, MAPSURF_32BIT | MT_32BIT_ABGR2101010},
#endif
	{PICT_r5g6b5, MAPSURF_16BIT | MT_16BIT_RGB565},
	{PICT_a1r5g5b5, MAPSURF_16BIT | MT_16BIT_ARGB1555},
	{PICT_a4r4g4b4, MAPSURF_16BIT | MT_16BIT_ARGB4444},
};

static uint32_t i915_get_blend_cntl(int op, PicturePtr mask,
				    uint32_t dst_format)
{
	uint32_t sblend, dblend;

	sblend = i915_blend_op[op].src_blend;
	dblend = i915_blend_op[op].dst_blend;

	/* If there's no dst alpha channel, adjust the blend op so that we'll
	 * treat it as always 1.
	 */
	if (PICT_FORMAT_A(dst_format) == 0 && i915_blend_op[op].dst_alpha) {
		if (sblend == BLENDFACT_DST_ALPHA)
			sblend = BLENDFACT_ONE;
		else if (sblend == BLENDFACT_INV_DST_ALPHA)
			sblend = BLENDFACT_ZERO;
	}

	/* i915 engine reads 8bit color buffer into green channel in cases
	   like color buffer blending .etc, and also writes back green channel.
	   So with dst_alpha blend we should use color factor. See spec on
	   "8-bit rendering" */
	if ((dst_format == PICT_a8) && i915_blend_op[op].dst_alpha) {
		if (sblend == BLENDFACT_DST_ALPHA)
			sblend = BLENDFACT_DST_COLR;
		else if (sblend == BLENDFACT_INV_DST_ALPHA)
			sblend = BLENDFACT_INV_DST_COLR;
	}

	/* If the source alpha is being used, then we should only be in a case
	 * where the source blend factor is 0, and the source blend value is the
	 * mask channels multiplied by the source picture's alpha.
	 */
	if (mask && mask->componentAlpha && PICT_FORMAT_RGB(mask->format) &&
	    i915_blend_op[op].src_alpha) {
		if (dblend == BLENDFACT_SRC_ALPHA) {
			dblend = BLENDFACT_SRC_COLR;
		} else if (dblend == BLENDFACT_INV_SRC_ALPHA) {
			dblend = BLENDFACT_INV_SRC_COLR;
		}
	}

	return S6_CBUF_BLEND_ENABLE | S6_COLOR_WRITE_ENABLE |
		(BLENDFUNC_ADD << S6_CBUF_BLEND_FUNC_SHIFT) |
		(sblend << S6_CBUF_SRC_BLEND_FACT_SHIFT) |
		(dblend << S6_CBUF_DST_BLEND_FACT_SHIFT);
}

#define DSTORG_HORT_BIAS(x)             ((x)<<20)
#define DSTORG_VERT_BIAS(x)             ((x)<<16)

static Bool i915_get_dest_format(PicturePtr dest_picture, uint32_t * dst_format)
{
	ScrnInfoPtr scrn;

	switch (dest_picture->format) {
	case PICT_a8r8g8b8:
	case PICT_x8r8g8b8:
		*dst_format = COLR_BUF_ARGB8888;
		break;
	case PICT_r5g6b5:
		*dst_format = COLR_BUF_RGB565;
		break;
	case PICT_a1r5g5b5:
	case PICT_x1r5g5b5:
		*dst_format = COLR_BUF_ARGB1555;
		break;
#if XORG_VERSION_CURRENT >= 10699900
	case PICT_a2r10g10b10:
	case PICT_x2r10g10b10:
		*dst_format = COLR_BUF_ARGB2AAA;
		break;
#endif
	case PICT_a8:
		*dst_format = COLR_BUF_8BIT;
		break;
	case PICT_a4r4g4b4:
	case PICT_x4r4g4b4:
		*dst_format = COLR_BUF_ARGB4444;
		break;
	default:
		scrn = xf86ScreenToScrn(dest_picture->pDrawable->pScreen);
		intel_debug_fallback(scrn,
				     "Unsupported dest format 0x%x\n",
				     (int)dest_picture->format);
		return FALSE;
	}
	*dst_format |= DSTORG_HORT_BIAS(0x8) | DSTORG_VERT_BIAS(0x8);
	return TRUE;
}

Bool
i915_check_composite(int op,
		     PicturePtr source_picture,
		     PicturePtr mask_picture,
		     PicturePtr dest_picture,
		     int width, int height)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(dest_picture->pDrawable->pScreen);
	uint32_t tmp1;

	/* Check for unsupported compositing operations. */
	if (op >= sizeof(i915_blend_op) / sizeof(i915_blend_op[0])) {
		intel_debug_fallback(scrn, "Unsupported Composite op 0x%x\n",
				     op);
		return FALSE;
	}
	if (mask_picture != NULL && mask_picture->componentAlpha &&
	    PICT_FORMAT_RGB(mask_picture->format)) {
		/* Check if it's component alpha that relies on a source alpha
		 * and on the source value.  We can only get one of those
		 * into the single source value that we get to blend with.
		 */
		if (i915_blend_op[op].src_alpha &&
		    (i915_blend_op[op].src_blend != BLENDFACT_ZERO)) {
			if (op != PictOpOver) {
				intel_debug_fallback(scrn,
						     "Component alpha not supported "
						     "with source alpha and source "
						     "value blending.\n");
				return FALSE;
			}
		}
	}

	if (!i915_get_dest_format(dest_picture, &tmp1)) {
		intel_debug_fallback(scrn, "Get Color buffer format\n");
		return FALSE;
	}

	if (width > 2048 || height > 2048)
		return FALSE;

	return TRUE;
}

Bool
i915_check_composite_target(PixmapPtr pixmap)
{
	if (pixmap->drawable.width > 2048 || pixmap->drawable.height > 2048)
		return FALSE;

	if(!intel_check_pitch_3d(pixmap))
		return FALSE;

	return TRUE;
}

Bool
i915_check_composite_texture(ScreenPtr screen, PicturePtr picture)
{
	if (picture->repeatType > RepeatReflect) {
		ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
		intel_debug_fallback(scrn, "Unsupported picture repeat %d\n",
			     picture->repeatType);
		return FALSE;
	}

	if (picture->filter != PictFilterNearest &&
	    picture->filter != PictFilterBilinear) {
		ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
		intel_debug_fallback(scrn, "Unsupported filter 0x%x\n",
				     picture->filter);
		return FALSE;
	}

	if (picture->pSourcePict)
		return FALSE;

	if (picture->pDrawable) {
		int w, h, i;

		w = picture->pDrawable->width;
		h = picture->pDrawable->height;
		if ((w > 2048) || (h > 2048)) {
			ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
			intel_debug_fallback(scrn,
					     "Picture w/h too large (%dx%d)\n",
					     w, h);
			return FALSE;
		}

		for (i = 0;
		     i < sizeof(i915_tex_formats) / sizeof(i915_tex_formats[0]);
		     i++) {
			if (i915_tex_formats[i].fmt == picture->format)
				break;
		}
		if (i == sizeof(i915_tex_formats) / sizeof(i915_tex_formats[0]))
		{
			ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
			intel_debug_fallback(scrn, "Unsupported picture format "
					     "0x%x\n",
					     (int)picture->format);
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

static Bool i915_texture_setup(PicturePtr picture, PixmapPtr pixmap, int unit)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(picture->pDrawable->pScreen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	uint32_t format, pitch, filter;
	uint32_t wrap_mode, tiling_bits;
	int i;

	pitch = intel_pixmap_pitch(pixmap);
	intel->scale_units[unit][0] = 1. / pixmap->drawable.width;
	intel->scale_units[unit][1] = 1. / pixmap->drawable.height;

	for (i = 0; i < sizeof(i915_tex_formats) / sizeof(i915_tex_formats[0]);
	     i++) {
		if (i915_tex_formats[i].fmt == picture->format)
			break;
	}
	if (i == sizeof(i915_tex_formats) / sizeof(i915_tex_formats[0])) {
		intel_debug_fallback(scrn, "unknown texture format\n");
		return FALSE;
	}
	format = i915_tex_formats[i].card_fmt;

	switch (picture->repeatType) {
	case RepeatNone:
		wrap_mode = TEXCOORDMODE_CLAMP_BORDER;
		break;
	case RepeatNormal:
		wrap_mode = TEXCOORDMODE_WRAP;
		break;
	case RepeatPad:
		wrap_mode = TEXCOORDMODE_CLAMP_EDGE;
		break;
	case RepeatReflect:
		wrap_mode = TEXCOORDMODE_MIRROR;
		break;
	default:
		FatalError("Unknown repeat type %d\n", picture->repeatType);
	}

	switch (picture->filter) {
	case PictFilterNearest:
		filter = (FILTER_NEAREST << SS2_MAG_FILTER_SHIFT) |
		    (FILTER_NEAREST << SS2_MIN_FILTER_SHIFT);
		break;
	case PictFilterBilinear:
		filter = (FILTER_LINEAR << SS2_MAG_FILTER_SHIFT) |
		    (FILTER_LINEAR << SS2_MIN_FILTER_SHIFT);
		break;
	default:
		intel_debug_fallback(scrn, "Bad filter 0x%x\n",
				     picture->filter);
		return FALSE;
	}

	/* offset filled in at emit time */
	if (intel_pixmap_tiled(pixmap)) {
		tiling_bits = MS3_TILED_SURFACE;
		if (intel_get_pixmap_private(pixmap)->tiling
				== I915_TILING_Y)
			tiling_bits |= MS3_TILE_WALK;
	} else
		tiling_bits = 0;

	intel->texture[unit] = pixmap;
	intel->mapstate[unit * 3 + 0] = 0;
	intel->mapstate[unit * 3 + 1] = format |
	    tiling_bits |
	    ((pixmap->drawable.height - 1) << MS3_HEIGHT_SHIFT) |
	    ((pixmap->drawable.width - 1) << MS3_WIDTH_SHIFT);
	intel->mapstate[unit * 3 + 2] = ((pitch / 4) - 1) << MS4_PITCH_SHIFT;

	intel->samplerstate[unit * 3 + 0] = (MIPFILTER_NONE <<
					     SS2_MIP_FILTER_SHIFT);
	intel->samplerstate[unit * 3 + 0] |= filter;
	intel->samplerstate[unit * 3 + 1] = SS3_NORMALIZED_COORDS;
	intel->samplerstate[unit * 3 + 1] |=
	    wrap_mode << SS3_TCX_ADDR_MODE_SHIFT;
	intel->samplerstate[unit * 3 + 1] |=
	    wrap_mode << SS3_TCY_ADDR_MODE_SHIFT;
	intel->samplerstate[unit * 3 + 1] |= unit << SS3_TEXTUREMAP_INDEX_SHIFT;
	intel->samplerstate[unit * 3 + 2] = 0x00000000;	/* border color */

	intel->transform[unit] = picture->transform;

	return TRUE;
}

static void
i915_emit_composite_primitive_identity_source(intel_screen_private *intel,
					      int srcX, int srcY,
					      int maskX, int maskY,
					      int dstX, int dstY,
					      int w, int h)
{
	OUT_VERTEX(dstX + w);
	OUT_VERTEX(dstY + h);
	OUT_VERTEX((srcX + w) * intel->scale_units[0][0]);
	OUT_VERTEX((srcY + h) * intel->scale_units[0][1]);

	OUT_VERTEX(dstX);
	OUT_VERTEX(dstY + h);
	OUT_VERTEX(srcX * intel->scale_units[0][0]);
	OUT_VERTEX((srcY + h) * intel->scale_units[0][1]);

	OUT_VERTEX(dstX);
	OUT_VERTEX(dstY);
	OUT_VERTEX(srcX * intel->scale_units[0][0]);
	OUT_VERTEX(srcY * intel->scale_units[0][1]);
}

static void
i915_emit_composite_primitive_affine_source(intel_screen_private *intel,
					    int srcX, int srcY,
					    int maskX, int maskY,
					    int dstX, int dstY,
					    int w, int h)
{
	float src_x[3], src_y[3];

	if (!intel_get_transformed_coordinates(srcX, srcY,
					      intel->transform[0],
					      &src_x[0],
					      &src_y[0]))
		return;

	if (!intel_get_transformed_coordinates(srcX, srcY + h,
					      intel->transform[0],
					      &src_x[1],
					      &src_y[1]))
		return;

	if (!intel_get_transformed_coordinates(srcX + w, srcY + h,
					      intel->transform[0],
					      &src_x[2],
					      &src_y[2]))
		return;

	OUT_VERTEX(dstX + w);
	OUT_VERTEX(dstY + h);
	OUT_VERTEX(src_x[2] * intel->scale_units[0][0]);
	OUT_VERTEX(src_y[2] * intel->scale_units[0][1]);

	OUT_VERTEX(dstX);
	OUT_VERTEX(dstY + h);
	OUT_VERTEX(src_x[1] * intel->scale_units[0][0]);
	OUT_VERTEX(src_y[1] * intel->scale_units[0][1]);

	OUT_VERTEX(dstX);
	OUT_VERTEX(dstY);
	OUT_VERTEX(src_x[0] * intel->scale_units[0][0]);
	OUT_VERTEX(src_y[0] * intel->scale_units[0][1]);
}

static void
i915_emit_composite_primitive_identity_source_mask(intel_screen_private *intel,
						   int srcX, int srcY,
						   int maskX, int maskY,
						   int dstX, int dstY,
						   int w, int h)
{
	OUT_VERTEX(dstX + w);
	OUT_VERTEX(dstY + h);
	OUT_VERTEX((srcX + w) * intel->scale_units[0][0]);
	OUT_VERTEX((srcY + h) * intel->scale_units[0][1]);
	OUT_VERTEX((maskX + w) * intel->scale_units[1][0]);
	OUT_VERTEX((maskY + h) * intel->scale_units[1][1]);

	OUT_VERTEX(dstX);
	OUT_VERTEX(dstY + h);
	OUT_VERTEX(srcX * intel->scale_units[0][0]);
	OUT_VERTEX((srcY + h) * intel->scale_units[0][1]);
	OUT_VERTEX(maskX * intel->scale_units[1][0]);
	OUT_VERTEX((maskY + h) * intel->scale_units[1][1]);

	OUT_VERTEX(dstX);
	OUT_VERTEX(dstY);
	OUT_VERTEX(srcX * intel->scale_units[0][0]);
	OUT_VERTEX(srcY * intel->scale_units[0][1]);
	OUT_VERTEX(maskX * intel->scale_units[1][0]);
	OUT_VERTEX(maskY * intel->scale_units[1][1]);
}

static void
i915_emit_composite_primitive(intel_screen_private *intel,
			      int srcX, int srcY,
			      int maskX, int maskY,
			      int dstX, int dstY,
			      int w, int h)
{
	Bool is_affine_src = TRUE, is_affine_mask = TRUE;
	int tex_unit = 0;
	int src_unit = -1, mask_unit = -1;
	float src_x[3], src_y[3], src_w[3], mask_x[3], mask_y[3], mask_w[3];

	src_unit = tex_unit++;

	is_affine_src = intel_transform_is_affine(intel->transform[src_unit]);
	if (is_affine_src) {
		if (!intel_get_transformed_coordinates(srcX, srcY,
						      intel->
						      transform[src_unit],
						      &src_x[0],
						      &src_y[0]))
			return;

		if (!intel_get_transformed_coordinates(srcX, srcY + h,
						      intel->
						      transform[src_unit],
						      &src_x[1],
						      &src_y[1]))
			return;

		if (!intel_get_transformed_coordinates(srcX + w, srcY + h,
						      intel->
						      transform[src_unit],
						      &src_x[2],
						      &src_y[2]))
			return;
	} else {
		if (!intel_get_transformed_coordinates_3d(srcX, srcY,
							 intel->
							 transform[src_unit],
							 &src_x[0],
							 &src_y[0],
							 &src_w[0]))
			return;

		if (!intel_get_transformed_coordinates_3d(srcX, srcY + h,
							 intel->
							 transform[src_unit],
							 &src_x[1],
							 &src_y[1],
							 &src_w[1]))
			return;

		if (!intel_get_transformed_coordinates_3d(srcX + w, srcY + h,
							 intel->
							 transform[src_unit],
							 &src_x[2],
							 &src_y[2],
							 &src_w[2]))
			return;
	}

	if (intel->render_mask) {
		mask_unit = tex_unit++;

		is_affine_mask = intel_transform_is_affine(intel->transform[mask_unit]);
		if (is_affine_mask) {
			if (!intel_get_transformed_coordinates(maskX, maskY,
							      intel->
							      transform[mask_unit],
							      &mask_x[0],
							      &mask_y[0]))
				return;

			if (!intel_get_transformed_coordinates(maskX, maskY + h,
							      intel->
							      transform[mask_unit],
							      &mask_x[1],
							      &mask_y[1]))
				return;

			if (!intel_get_transformed_coordinates(maskX + w, maskY + h,
							      intel->
							      transform[mask_unit],
							      &mask_x[2],
							      &mask_y[2]))
				return;
		} else {
			if (!intel_get_transformed_coordinates_3d(maskX, maskY,
								 intel->
								 transform[mask_unit],
								 &mask_x[0],
								 &mask_y[0],
								 &mask_w[0]))
				return;

			if (!intel_get_transformed_coordinates_3d(maskX, maskY + h,
								 intel->
								 transform[mask_unit],
								 &mask_x[1],
								 &mask_y[1],
								 &mask_w[1]))
				return;

			if (!intel_get_transformed_coordinates_3d(maskX + w, maskY + h,
								 intel->
								 transform[mask_unit],
								 &mask_x[2],
								 &mask_y[2],
								 &mask_w[2]))
				return;
		}
	}

	OUT_VERTEX(dstX + w);
	OUT_VERTEX(dstY + h);
	OUT_VERTEX(src_x[2] * intel->scale_units[src_unit][0]);
	OUT_VERTEX(src_y[2] * intel->scale_units[src_unit][1]);
	if (!is_affine_src) {
		OUT_VERTEX(0.0);
		OUT_VERTEX(src_w[2]);
	}
	if (intel->render_mask) {
		OUT_VERTEX(mask_x[2] * intel->scale_units[mask_unit][0]);
		OUT_VERTEX(mask_y[2] * intel->scale_units[mask_unit][1]);
		if (!is_affine_mask) {
			OUT_VERTEX(0.0);
			OUT_VERTEX(mask_w[2]);
		}
	}

	OUT_VERTEX(dstX);
	OUT_VERTEX(dstY + h);
	OUT_VERTEX(src_x[1] * intel->scale_units[src_unit][0]);
	OUT_VERTEX(src_y[1] * intel->scale_units[src_unit][1]);
	if (!is_affine_src) {
		OUT_VERTEX(0.0);
		OUT_VERTEX(src_w[1]);
	}
	if (intel->render_mask) {
		OUT_VERTEX(mask_x[1] * intel->scale_units[mask_unit][0]);
		OUT_VERTEX(mask_y[1] * intel->scale_units[mask_unit][1]);
		if (!is_affine_mask) {
			OUT_VERTEX(0.0);
			OUT_VERTEX(mask_w[1]);
		}
	}

	OUT_VERTEX(dstX);
	OUT_VERTEX(dstY);
	OUT_VERTEX(src_x[0] * intel->scale_units[src_unit][0]);
	OUT_VERTEX(src_y[0] * intel->scale_units[src_unit][1]);
	if (!is_affine_src) {
		OUT_VERTEX(0.0);
		OUT_VERTEX(src_w[0]);
	}
	if (intel->render_mask) {
		OUT_VERTEX(mask_x[0] * intel->scale_units[mask_unit][0]);
		OUT_VERTEX(mask_y[0] * intel->scale_units[mask_unit][1]);
		if (!is_affine_mask) {
			OUT_VERTEX(0.0);
			OUT_VERTEX(mask_w[0]);
		}
	}
}

Bool
i915_prepare_composite(int op, PicturePtr source_picture,
		       PicturePtr mask_picture, PicturePtr dest_picture,
		       PixmapPtr source, PixmapPtr mask, PixmapPtr dest)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(dest_picture->pDrawable->pScreen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *bo_table[] = {
		NULL,		/* batch_bo */
		intel_get_pixmap_bo(dest),
		intel_get_pixmap_bo(source),
		mask ? intel_get_pixmap_bo(mask) : NULL,
	};
	int tex_unit = 0;
	int floats_per_vertex;

	intel->render_source_picture = source_picture;
	intel->render_source = source;
	intel->render_mask_picture = mask_picture;
	intel->render_mask = mask;
	intel->render_dest_picture = dest_picture;
	intel->render_dest = dest;

	if (!intel_check_pitch_3d(source))
		return FALSE;

	if (mask && !intel_check_pitch_3d(mask))
		return FALSE;

	if (!intel_check_pitch_3d(dest))
		return FALSE;

	if (!i915_get_dest_format(dest_picture,
				  &intel->i915_render_state.dst_format))
		return FALSE;

	if (!intel_get_aperture_space(scrn, bo_table, ARRAY_SIZE(bo_table)))
		return FALSE;

	if (mask_picture != NULL && mask_picture->componentAlpha &&
	    PICT_FORMAT_RGB(mask_picture->format)) {
		/* Check if it's component alpha that relies on a source alpha
		 * and on the source value.  We can only get one of those
		 * into the single source value that we get to blend with.
		 */
		if (i915_blend_op[op].src_alpha &&
		    (i915_blend_op[op].src_blend != BLENDFACT_ZERO))
			return FALSE;
	}

	intel->transform[0] = NULL;
	intel->scale_units[0][0] = -1;
	intel->scale_units[0][1] = -1;
	intel->transform[1] = NULL;
	intel->scale_units[1][0] = -1;
	intel->scale_units[1][1] = -1;

	floats_per_vertex = 2;		/* dest x/y */
	if (!i915_texture_setup(source_picture, source, tex_unit++)) {
		intel_debug_fallback(scrn, "fail to setup src texture\n");
		return FALSE;
	}

	if (intel_transform_is_affine(source_picture->transform))
		floats_per_vertex += 2;	/* src x/y */
	else
		floats_per_vertex += 4;	/* src x/y/z/w */

	if (mask_picture != NULL) {
		assert(mask != NULL);
		if (!i915_texture_setup(mask_picture, mask, tex_unit++)) {
			intel_debug_fallback(scrn,
					     "fail to setup mask texture\n");
			return FALSE;
		}

		if (intel_transform_is_affine(mask_picture->transform))
			floats_per_vertex += 2;	/* mask x/y */
		else
			floats_per_vertex += 4;	/* mask x/y/z/w */
	}

	intel->i915_render_state.op = op;

	if (intel_pixmap_is_dirty(source) || intel_pixmap_is_dirty(mask))
		intel_batch_emit_flush(scrn);

	intel->needs_render_state_emit = TRUE;

	intel->prim_emit = i915_emit_composite_primitive;
	if (!mask) {
		if (intel->transform[0] == NULL)
			intel->prim_emit = i915_emit_composite_primitive_identity_source;
		else if (intel_transform_is_affine(intel->transform[0]))
			intel->prim_emit = i915_emit_composite_primitive_affine_source;
	} else {
		if (intel->transform[0] == NULL) {
			if (intel->transform[1] == NULL)
				intel->prim_emit = i915_emit_composite_primitive_identity_source_mask;
		}
	}

	if (floats_per_vertex != intel->floats_per_vertex) {
		intel->floats_per_vertex = floats_per_vertex;
		intel->needs_render_vertex_emit = TRUE;
	}

	return TRUE;
}

static void
i915_composite_emit_shader(intel_screen_private *intel, CARD8 op)
{
	PicturePtr mask_picture = intel->render_mask_picture;
	PixmapPtr mask = intel->render_mask;
	int src_reg, mask_reg;
	Bool dest_is_alpha = PIXMAN_FORMAT_RGB(intel->render_dest_picture->format) == 0;
	FS_LOCALS();

	FS_BEGIN();

	/* Declare the registers necessary for our program.  */
	i915_fs_dcl(FS_T0);
	i915_fs_dcl(FS_S0);
	if (!mask) {
		/* No mask, so load directly to output color */
		if (dest_is_alpha)
			src_reg = FS_R0;
		else
			src_reg = FS_OC;

		if (intel_transform_is_affine(intel->transform[0]))
			i915_fs_texld(src_reg, FS_S0, FS_T0);
		else
			i915_fs_texldp(src_reg, FS_S0, FS_T0);

		if (src_reg != FS_OC)
			i915_fs_mov(FS_OC, i915_fs_operand(src_reg, W, W, W, W));
	} else {
		i915_fs_dcl(FS_T1);
		i915_fs_dcl(FS_S1);

		/* Load the source_picture texel */
		if (intel_transform_is_affine(intel->transform[0]))
			i915_fs_texld(FS_R0, FS_S0, FS_T0);
		else
			i915_fs_texldp(FS_R0, FS_S0, FS_T0);

		src_reg = FS_R0;

		/* Load the mask_picture texel */
		if (intel_transform_is_affine(intel->transform[1]))
			i915_fs_texld(FS_R1, FS_S1, FS_T1);
		else
			i915_fs_texldp(FS_R1, FS_S1, FS_T1);

		mask_reg = FS_R1;

		if (dest_is_alpha) {
			i915_fs_mul(FS_OC,
				    i915_fs_operand(src_reg, W, W, W, W),
				    i915_fs_operand(mask_reg, W, W, W, W));
		} else {
			/* If component alpha is active in the mask and the blend
			 * operation uses the source alpha, then we know we don't
			 * need the source value (otherwise we would have hit a
			 * fallback earlier), so we provide the source alpha (src.A *
			 * mask.X) as output color.
			 * Conversely, if CA is set and we don't need the source alpha,
			 * then we produce the source value (src.X * mask.X) and the
			 * source alpha is unused.  Otherwise, we provide the non-CA
			 * source value (src.X * mask.A).
			 */
			if (mask_picture->componentAlpha &&
			    PICT_FORMAT_RGB(mask_picture->format)) {
				if (i915_blend_op[op].src_alpha) {
					i915_fs_mul(FS_OC,
						    i915_fs_operand(src_reg, W, W, W, W),
						    i915_fs_operand_reg(mask_reg));
				} else {
					i915_fs_mul(FS_OC,
						    i915_fs_operand_reg(src_reg),
						    i915_fs_operand_reg(mask_reg));
				}
			} else {
				i915_fs_mul(FS_OC,
					    i915_fs_operand_reg(src_reg),
					    i915_fs_operand(mask_reg, W, W, W, W));
			}
		}
	}

	FS_END();
}

static void i915_emit_composite_setup(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int op = intel->i915_render_state.op;
	PicturePtr mask_picture = intel->render_mask_picture;
	PicturePtr dest_picture = intel->render_dest_picture;
	PixmapPtr mask = intel->render_mask;
	PixmapPtr dest = intel->render_dest;
	int tex_count, t;

	intel->needs_render_state_emit = FALSE;

	IntelEmitInvarientState(scrn);
	intel->last_3d = LAST_3D_RENDER;

	tex_count = 1 + (mask != NULL);

	assert(intel->in_batch_atomic);

	if (tex_count != 0) {
	    OUT_BATCH(_3DSTATE_MAP_STATE | (3 * tex_count));
	    OUT_BATCH((1 << tex_count) - 1);
	    for (t = 0; t < tex_count; t++) {
		OUT_RELOC_PIXMAP(intel->texture[t], I915_GEM_DOMAIN_SAMPLER, 0, 0);
		OUT_BATCH(intel->mapstate[3*t + 1]);
		OUT_BATCH(intel->mapstate[3*t + 2]);
	    }

	    OUT_BATCH(_3DSTATE_SAMPLER_STATE | (3 * tex_count));
	    OUT_BATCH((1 << tex_count) - 1);
	    for (t = 0; t < tex_count; t++) {
		OUT_BATCH(intel->samplerstate[3*t + 0]);
		OUT_BATCH(intel->samplerstate[3*t + 1]);
		OUT_BATCH(intel->samplerstate[3*t + 2]);
	    }
	}

	/* BUF_INFO is an implicit flush, so avoid if the target has not changed.
	 * XXX However for reasons unfathomed, correct rendering in KDE requires
	 * at least a MI_FLUSH | INHIBIT_RENDER_CACHE_FLUSH here.
	 */
	if (1) {
		uint32_t tiling_bits;

		if (intel_pixmap_tiled(dest)) {
			tiling_bits = BUF_3D_TILED_SURFACE;
			if (intel_get_pixmap_private(dest)->tiling
			    == I915_TILING_Y)
				tiling_bits |= BUF_3D_TILE_WALK_Y;
		} else
			tiling_bits = 0;

		OUT_BATCH(_3DSTATE_BUF_INFO_CMD);
		OUT_BATCH(BUF_3D_ID_COLOR_BACK | tiling_bits |
			  BUF_3D_PITCH(intel_pixmap_pitch(dest)));
		OUT_RELOC_PIXMAP(dest, I915_GEM_DOMAIN_RENDER,
				 I915_GEM_DOMAIN_RENDER, 0);

		OUT_BATCH(_3DSTATE_DST_BUF_VARS_CMD);
		OUT_BATCH(intel->i915_render_state.dst_format);

		/* draw rect is unconditional */
		OUT_BATCH(_3DSTATE_DRAW_RECT_CMD);
		OUT_BATCH(0x00000000);
		OUT_BATCH(0x00000000);	/* ymin, xmin */
		OUT_BATCH(DRAW_YMAX(dest->drawable.height - 1) |
			  DRAW_XMAX(dest->drawable.width - 1));
		/* yorig, xorig (relate to color buffer?) */
		OUT_BATCH(0x00000000);
	}

	{
		uint32_t ss2;

		ss2 = ~0;
		ss2 &= ~S2_TEXCOORD_FMT(0, TEXCOORDFMT_NOT_PRESENT);
		ss2 |= S2_TEXCOORD_FMT(0,
				       intel_transform_is_affine(intel->transform[0]) ?
				       TEXCOORDFMT_2D : TEXCOORDFMT_4D);
		if (mask) {
		    ss2 &= ~S2_TEXCOORD_FMT(1, TEXCOORDFMT_NOT_PRESENT);
		    ss2 |= S2_TEXCOORD_FMT(1,
					   intel_transform_is_affine(intel->transform[1]) ?
					   TEXCOORDFMT_2D : TEXCOORDFMT_4D);
		}

		OUT_BATCH(_3DSTATE_LOAD_STATE_IMMEDIATE_1 | I1_LOAD_S(2) | I1_LOAD_S(6) | 1);
		OUT_BATCH(ss2);
		OUT_BATCH(i915_get_blend_cntl(op, mask_picture, dest_picture->format));
	}

	i915_composite_emit_shader(intel, op);
}

void
i915_composite(PixmapPtr dest, int srcX, int srcY, int maskX, int maskY,
	       int dstX, int dstY, int w, int h)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(dest->drawable.pScreen);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* 28 + 16 + 10 + 20 + 32 + 16 */
	intel_batch_start_atomic(scrn, 150);

	if (intel->needs_render_state_emit)
		i915_emit_composite_setup(scrn);

	if (intel->needs_render_vertex_emit ||
	    intel_vertex_space(intel) < 3*4*intel->floats_per_vertex) {
		i915_vertex_flush(intel);

		if (intel_vertex_space(intel) < 256) {
			intel_next_vertex(intel);

			OUT_BATCH(_3DSTATE_LOAD_STATE_IMMEDIATE_1 |
				  I1_LOAD_S(0) | I1_LOAD_S(1) | 1);
			OUT_RELOC(intel->vertex_bo, I915_GEM_DOMAIN_VERTEX, 0, 0);
			OUT_BATCH((intel->floats_per_vertex << S1_VERTEX_WIDTH_SHIFT) |
				  (intel->floats_per_vertex << S1_VERTEX_PITCH_SHIFT));
			intel->vertex_index = 0;
		} else if (intel->floats_per_vertex != intel->last_floats_per_vertex){
			OUT_BATCH(_3DSTATE_LOAD_STATE_IMMEDIATE_1 |
				  I1_LOAD_S(1) | 0);
			OUT_BATCH((intel->floats_per_vertex << S1_VERTEX_WIDTH_SHIFT) |
				  (intel->floats_per_vertex << S1_VERTEX_PITCH_SHIFT));

			intel->vertex_index =
				(intel->vertex_used + intel->floats_per_vertex - 1) /  intel->floats_per_vertex;
			intel->vertex_used = intel->vertex_index * intel->floats_per_vertex;
		}

		intel->last_floats_per_vertex = intel->floats_per_vertex;
		intel->needs_render_vertex_emit = FALSE;
	}

	if (intel->prim_offset == 0) {
		intel->prim_offset = intel->batch_used;
		OUT_BATCH(PRIM3D_RECTLIST | PRIM3D_INDIRECT_SEQUENTIAL);
		OUT_BATCH(intel->vertex_index);
	}
	intel->vertex_count += 3;

	intel->prim_emit(intel,
			 srcX, srcY,
			 maskX, maskY,
			 dstX, dstY,
			 w, h);

	intel_batch_end_atomic(scrn);
}

void
i915_vertex_flush(intel_screen_private *intel)
{
	if (intel->prim_offset == 0)
		return;

	intel->batch_ptr[intel->prim_offset] |= intel->vertex_count;
	intel->prim_offset = 0;

	intel->vertex_index += intel->vertex_count;
	intel->vertex_count = 0;
}

void
i915_batch_commit_notify(intel_screen_private *intel)
{
	intel->needs_render_state_emit = TRUE;
	intel->last_floats_per_vertex = 0;
}
