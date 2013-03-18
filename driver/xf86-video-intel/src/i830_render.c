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
#include "i830_reg.h"

struct blendinfo {
	Bool dst_alpha;
	Bool src_alpha;
	uint32_t src_blend;
	uint32_t dst_blend;
};

struct formatinfo {
	int fmt;
	uint32_t card_fmt;
};

#define TB0C_LAST_STAGE	(1 << 31)
#define TB0C_RESULT_SCALE_1X		(0 << 29)
#define TB0C_RESULT_SCALE_2X		(1 << 29)
#define TB0C_RESULT_SCALE_4X		(2 << 29)
#define TB0C_OP_MODULE			(3 << 25)
#define TB0C_OUTPUT_WRITE_CURRENT	(0 << 24)
#define TB0C_OUTPUT_WRITE_ACCUM		(1 << 24)
#define TB0C_ARG3_REPLICATE_ALPHA 	(1<<23)
#define TB0C_ARG3_INVERT		(1<<22)
#define TB0C_ARG3_SEL_XXX
#define TB0C_ARG2_REPLICATE_ALPHA 	(1<<17)
#define TB0C_ARG2_INVERT		(1<<16)
#define TB0C_ARG2_SEL_ONE		(0 << 12)
#define TB0C_ARG2_SEL_FACTOR		(1 << 12)
#define TB0C_ARG2_SEL_TEXEL0		(6 << 12)
#define TB0C_ARG2_SEL_TEXEL1		(7 << 12)
#define TB0C_ARG2_SEL_TEXEL2		(8 << 12)
#define TB0C_ARG2_SEL_TEXEL3		(9 << 12)
#define TB0C_ARG1_REPLICATE_ALPHA 	(1<<11)
#define TB0C_ARG1_INVERT		(1<<10)
#define TB0C_ARG1_SEL_ONE		(0 << 6)
#define TB0C_ARG1_SEL_TEXEL0		(6 << 6)
#define TB0C_ARG1_SEL_TEXEL1		(7 << 6)
#define TB0C_ARG1_SEL_TEXEL2		(8 << 6)
#define TB0C_ARG1_SEL_TEXEL3		(9 << 6)
#define TB0C_ARG0_REPLICATE_ALPHA 	(1<<5)
#define TB0C_ARG0_SEL_XXX

#define TB0A_CTR_STAGE_ENABLE 		(1<<31)
#define TB0A_RESULT_SCALE_1X		(0 << 29)
#define TB0A_RESULT_SCALE_2X		(1 << 29)
#define TB0A_RESULT_SCALE_4X		(2 << 29)
#define TB0A_OP_MODULE			(3 << 25)
#define TB0A_OUTPUT_WRITE_CURRENT	(0<<24)
#define TB0A_OUTPUT_WRITE_ACCUM		(1<<24)
#define TB0A_CTR_STAGE_SEL_BITS_XXX
#define TB0A_ARG3_SEL_XXX
#define TB0A_ARG3_INVERT		(1<<17)
#define TB0A_ARG2_INVERT		(1<<16)
#define TB0A_ARG2_SEL_ONE		(0 << 12)
#define TB0A_ARG2_SEL_TEXEL0		(6 << 12)
#define TB0A_ARG2_SEL_TEXEL1		(7 << 12)
#define TB0A_ARG2_SEL_TEXEL2		(8 << 12)
#define TB0A_ARG2_SEL_TEXEL3		(9 << 12)
#define TB0A_ARG1_INVERT		(1<<10)
#define TB0A_ARG1_SEL_ONE		(0 << 6)
#define TB0A_ARG1_SEL_TEXEL0		(6 << 6)
#define TB0A_ARG1_SEL_TEXEL1		(7 << 6)
#define TB0A_ARG1_SEL_TEXEL2		(8 << 6)
#define TB0A_ARG1_SEL_TEXEL3		(9 << 6)

static struct blendinfo i830_blend_op[] = {
	/* Clear */
	{0, 0, BLENDFACTOR_ZERO, BLENDFACTOR_ZERO},
	/* Src */
	{0, 0, BLENDFACTOR_ONE, BLENDFACTOR_ZERO},
	/* Dst */
	{0, 0, BLENDFACTOR_ZERO, BLENDFACTOR_ONE},
	/* Over */
	{0, 1, BLENDFACTOR_ONE, BLENDFACTOR_INV_SRC_ALPHA},
	/* OverReverse */
	{1, 0, BLENDFACTOR_INV_DST_ALPHA, BLENDFACTOR_ONE},
	/* In */
	{1, 0, BLENDFACTOR_DST_ALPHA, BLENDFACTOR_ZERO},
	/* InReverse */
	{0, 1, BLENDFACTOR_ZERO, BLENDFACTOR_SRC_ALPHA},
	/* Out */
	{1, 0, BLENDFACTOR_INV_DST_ALPHA, BLENDFACTOR_ZERO},
	/* OutReverse */
	{0, 1, BLENDFACTOR_ZERO, BLENDFACTOR_INV_SRC_ALPHA},
	/* Atop */
	{1, 1, BLENDFACTOR_DST_ALPHA, BLENDFACTOR_INV_SRC_ALPHA},
	/* AtopReverse */
	{1, 1, BLENDFACTOR_INV_DST_ALPHA, BLENDFACTOR_SRC_ALPHA},
	/* Xor */
	{1, 1, BLENDFACTOR_INV_DST_ALPHA, BLENDFACTOR_INV_SRC_ALPHA},
	/* Add */
	{0, 0, BLENDFACTOR_ONE, BLENDFACTOR_ONE},
};

static struct formatinfo i830_tex_formats[] = {
	{PICT_a8, MAPSURF_8BIT | MT_8BIT_A8},
	{PICT_a8r8g8b8, MAPSURF_32BIT | MT_32BIT_ARGB8888},
	{PICT_a8b8g8r8, MAPSURF_32BIT | MT_32BIT_ABGR8888},
	{PICT_r5g6b5, MAPSURF_16BIT | MT_16BIT_RGB565},
	{PICT_a1r5g5b5, MAPSURF_16BIT | MT_16BIT_ARGB1555},
	{PICT_a4r4g4b4, MAPSURF_16BIT | MT_16BIT_ARGB4444},
};

static struct formatinfo i855_tex_formats[] = {
	{PICT_x8r8g8b8, MAPSURF_32BIT | MT_32BIT_XRGB8888},
	{PICT_x8b8g8r8, MAPSURF_32BIT | MT_32BIT_XBGR8888},
};

static Bool i830_get_dest_format(PicturePtr dest_picture, uint32_t * dst_format)
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
	case PICT_a8:
		*dst_format = COLR_BUF_8BIT;
		break;
	case PICT_a4r4g4b4:
	case PICT_x4r4g4b4:
		*dst_format = COLR_BUF_ARGB4444;
		break;
	default:
		scrn = xf86ScreenToScrn(dest_picture->pDrawable->pScreen);
		intel_debug_fallback(scrn, "Unsupported dest format 0x%x\n",
				     (int)dest_picture->format);
		return FALSE;
	}
	*dst_format |= DSTORG_HORT_BIAS(0x8) | DSTORG_VERT_BIAS(0x8);
	return TRUE;
}

static Bool i830_get_blend_cntl(ScrnInfoPtr scrn, int op, PicturePtr mask,
				uint32_t dst_format, uint32_t * blendctl)
{
	uint32_t sblend, dblend;

	sblend = i830_blend_op[op].src_blend;
	dblend = i830_blend_op[op].dst_blend;

	/* If there's no dst alpha channel, adjust the blend op so that we'll treat
	 * it as always 1.
	 */
	if (PICT_FORMAT_A(dst_format) == 0 && i830_blend_op[op].dst_alpha) {
		if (sblend == BLENDFACTOR_DST_ALPHA)
			sblend = BLENDFACTOR_ONE;
		else if (sblend == BLENDFACTOR_INV_DST_ALPHA)
			sblend = BLENDFACTOR_ZERO;
	}

	/* For blending purposes, COLR_BUF_8BIT values show up in the green
	 * channel.  So we can't use the alpha channel.
	 */
	if (dst_format == PICT_a8 && ((sblend == BLENDFACTOR_DST_ALPHA ||
				       sblend == BLENDFACTOR_INV_DST_ALPHA))) {
		intel_debug_fallback(scrn, "Can't do dst alpha blending with "
				     "PICT_a8 dest.\n");
		return FALSE;
	}

	/* If the source alpha is being used, then we should only be in a case
	 * where the source blend factor is 0, and the source blend value is the
	 * mask channels multiplied by the source picture's alpha.
	 */
	if (mask && mask->componentAlpha && PICT_FORMAT_RGB(mask->format)
	    && i830_blend_op[op].src_alpha) {
		if (dblend == BLENDFACTOR_SRC_ALPHA) {
			dblend = BLENDFACTOR_SRC_COLR;
		} else if (dblend == BLENDFACTOR_INV_SRC_ALPHA) {
			dblend = BLENDFACTOR_INV_SRC_COLR;
		}
	}

	*blendctl = (sblend << S8_SRC_BLEND_FACTOR_SHIFT) |
	    (dblend << S8_DST_BLEND_FACTOR_SHIFT);

	return TRUE;
}

static uint32_t i8xx_get_card_format(intel_screen_private *intel,
				     PicturePtr picture)
{
	int i;

	for (i = 0; i < sizeof(i830_tex_formats) / sizeof(i830_tex_formats[0]);
	     i++) {
		if (i830_tex_formats[i].fmt == picture->format)
			return i830_tex_formats[i].card_fmt;
	}

	if (!(IS_I830(intel) || IS_845G(intel))) {
		for (i = 0; i < sizeof(i855_tex_formats) / sizeof(i855_tex_formats[0]);
		     i++) {
			if (i855_tex_formats[i].fmt == picture->format)
				return i855_tex_formats[i].card_fmt;
		}
	}

	return 0;
}

static void i830_texture_setup(PicturePtr picture, PixmapPtr pixmap, int unit)
{

	ScrnInfoPtr scrn = xf86ScreenToScrn(picture->pDrawable->pScreen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	uint32_t format, tiling_bits, pitch, filter;
	uint32_t wrap_mode;
	uint32_t texcoordtype;

	pitch = intel_pixmap_pitch(pixmap);
	intel->scale_units[unit][0] = pixmap->drawable.width;
	intel->scale_units[unit][1] = pixmap->drawable.height;
	intel->transform[unit] = picture->transform;

	if (intel_transform_is_affine(intel->transform[unit]))
		texcoordtype = TEXCOORDTYPE_CARTESIAN;
	else
		texcoordtype = TEXCOORDTYPE_HOMOGENEOUS;

	switch (picture->repeatType) {
	case RepeatNone:
		wrap_mode = TEXCOORDMODE_CLAMP_BORDER;
		break;
	case RepeatNormal:
		wrap_mode = TEXCOORDMODE_WRAP;
		break;
	case RepeatPad:
		wrap_mode = TEXCOORDMODE_CLAMP;
		break;
	case RepeatReflect:
		wrap_mode = TEXCOORDMODE_MIRROR;
		break;
	default:
		FatalError("Unknown repeat type %d\n", picture->repeatType);
	}

	switch (picture->filter) {
	case PictFilterNearest:
		filter = ((FILTER_NEAREST << TM0S3_MAG_FILTER_SHIFT) |
			  (FILTER_NEAREST << TM0S3_MIN_FILTER_SHIFT));
		break;
	case PictFilterBilinear:
		filter = ((FILTER_LINEAR << TM0S3_MAG_FILTER_SHIFT) |
			  (FILTER_LINEAR << TM0S3_MIN_FILTER_SHIFT));
		break;
	default:
		FatalError("Bad filter 0x%x\n", picture->filter);
	}
	filter |= (MIPFILTER_NONE << TM0S3_MIP_FILTER_SHIFT);

	if (intel_pixmap_tiled(pixmap)) {
		tiling_bits = TM0S1_TILED_SURFACE;
		if (intel_get_pixmap_private(pixmap)->tiling
				== I915_TILING_Y)
			tiling_bits |= TM0S1_TILE_WALK;
	} else
		tiling_bits = 0;

	format = i8xx_get_card_format(intel, picture);

	assert(intel->in_batch_atomic);

	OUT_BATCH(_3DSTATE_LOAD_STATE_IMMEDIATE_2 |
		  LOAD_TEXTURE_MAP(unit) | 4);
	OUT_RELOC_PIXMAP(pixmap, I915_GEM_DOMAIN_SAMPLER, 0, 0);
	OUT_BATCH(((pixmap->drawable.height -
		    1) << TM0S1_HEIGHT_SHIFT) | ((pixmap->drawable.width -
						  1) <<
						 TM0S1_WIDTH_SHIFT) |
		  format | tiling_bits);
	OUT_BATCH((pitch / 4 - 1) << TM0S2_PITCH_SHIFT | TM0S2_MAP_2D);
	OUT_BATCH(filter);
	OUT_BATCH(0);	/* default color */
	OUT_BATCH(_3DSTATE_MAP_COORD_SET_CMD | TEXCOORD_SET(unit) |
		  ENABLE_TEXCOORD_PARAMS | TEXCOORDS_ARE_NORMAL |
		  texcoordtype | ENABLE_ADDR_V_CNTL |
		  TEXCOORD_ADDR_V_MODE(wrap_mode) |
		  ENABLE_ADDR_U_CNTL | TEXCOORD_ADDR_U_MODE(wrap_mode));
	/* map texel stream */
	OUT_BATCH(_3DSTATE_MAP_COORD_SETBIND_CMD);
	if (unit == 0)
		OUT_BATCH(TEXBIND_SET0(TEXCOORDSRC_VTXSET_0) |
			  TEXBIND_SET1(TEXCOORDSRC_KEEP) |
			  TEXBIND_SET2(TEXCOORDSRC_KEEP) |
			  TEXBIND_SET3(TEXCOORDSRC_KEEP));
	else
		OUT_BATCH(TEXBIND_SET0(TEXCOORDSRC_VTXSET_0) |
			  TEXBIND_SET1(TEXCOORDSRC_VTXSET_1) |
			  TEXBIND_SET2(TEXCOORDSRC_KEEP) |
			  TEXBIND_SET3(TEXCOORDSRC_KEEP));
	OUT_BATCH(_3DSTATE_MAP_TEX_STREAM_CMD | (unit << 16) |
		  DISABLE_TEX_STREAM_BUMP |
		  ENABLE_TEX_STREAM_COORD_SET |
		  TEX_STREAM_COORD_SET(unit) |
		  ENABLE_TEX_STREAM_MAP_IDX | TEX_STREAM_MAP_IDX(unit));
}

Bool
i830_check_composite(int op,
		     PicturePtr source_picture,
		     PicturePtr mask_picture,
		     PicturePtr dest_picture,
		     int width, int height)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(dest_picture->pDrawable->pScreen);
	uint32_t tmp1;

	/* Check for unsupported compositing operations. */
	if (op >= sizeof(i830_blend_op) / sizeof(i830_blend_op[0])) {
		intel_debug_fallback(scrn, "Unsupported Composite op 0x%x\n",
				     op);
		return FALSE;
	}

	if (mask_picture != NULL && mask_picture->componentAlpha &&
	    PICT_FORMAT_RGB(mask_picture->format)) {
		/* Check if it's component alpha that relies on a source alpha and on
		 * the source value.  We can only get one of those into the single
		 * source value that we get to blend with.
		 */
		if (i830_blend_op[op].src_alpha &&
		    (i830_blend_op[op].src_blend != BLENDFACTOR_ZERO)) {
			intel_debug_fallback(scrn, "Component alpha not "
					     "supported with source alpha and "
					     "source value blending.\n");
			return FALSE;
		}
	}

	if (!i830_get_dest_format(dest_picture, &tmp1)) {
		intel_debug_fallback(scrn, "Get Color buffer format\n");
		return FALSE;
	}

	if (width > 2048 || height > 2048) {
		intel_debug_fallback(scrn, "Operation is too large (%d, %d)\n", width, height);
		return FALSE;
	}

	return TRUE;
}

Bool
i830_check_composite_target(PixmapPtr pixmap)
{
	if (pixmap->drawable.width > 2048 || pixmap->drawable.height > 2048)
		return FALSE;

	if(!intel_check_pitch_3d(pixmap))
		return FALSE;

	return TRUE;
}

Bool
i830_check_composite_texture(ScreenPtr screen, PicturePtr picture)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	if (picture->repeatType > RepeatReflect) {
		intel_debug_fallback(scrn, "Unsupported picture repeat %d\n",
			     picture->repeatType);
		return FALSE;
	}

	if (picture->filter != PictFilterNearest &&
	    picture->filter != PictFilterBilinear) {
		intel_debug_fallback(scrn, "Unsupported filter 0x%x\n",
				     picture->filter);
		return FALSE;
	}

	if (picture->pDrawable) {
		int w, h;

		w = picture->pDrawable->width;
		h = picture->pDrawable->height;
		if ((w > 2048) || (h > 2048)) {
			intel_debug_fallback(scrn,
					     "Picture w/h too large (%dx%d)\n",
					     w, h);
			return FALSE;
		}

		/* XXX we can use the xrgb32 types if there the picture covers the clip */
		if (!i8xx_get_card_format(intel, picture)) {
			intel_debug_fallback(scrn, "Unsupported picture format "
					     "0x%x\n",
					     (int)picture->format);
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

Bool
i830_prepare_composite(int op, PicturePtr source_picture,
		       PicturePtr mask_picture, PicturePtr dest_picture,
		       PixmapPtr source, PixmapPtr mask, PixmapPtr dest)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(dest_picture->pDrawable->pScreen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *bo_table[] = {
		NULL,		/* batch_bo */
		intel_get_pixmap_bo(source),
		mask ? intel_get_pixmap_bo(mask) : NULL,
		intel_get_pixmap_bo(dest),
	};

	intel->render_source_picture = source_picture;
	intel->render_source = source;
	intel->render_mask_picture = mask_picture;
	intel->render_mask = mask;
	intel->render_dest_picture = dest_picture;
	intel->render_dest = dest;

	if (!intel_check_pitch_3d(source))
		return FALSE;
	if (mask) {
		if (mask_picture->componentAlpha &&
		    PICT_FORMAT_RGB(mask_picture->format)) {
			/* Check if it's component alpha that relies on a source alpha and on
			 * the source value.  We can only get one of those into the single
			 * source value that we get to blend with.
			 */
			if (i830_blend_op[op].src_alpha &&
			    (i830_blend_op[op].src_blend != BLENDFACTOR_ZERO)) {
				intel_debug_fallback(scrn, "Component alpha not "
						     "supported with source alpha and "
						     "source value blending.\n");
				return FALSE;
			}
		}
		if (!intel_check_pitch_3d(mask))
			return FALSE;
	}
	if (!intel_check_pitch_3d(dest))
		return FALSE;

	if (!i830_get_dest_format(dest_picture, &intel->render_dest_format))
		return FALSE;

	if (!intel_get_aperture_space(scrn, bo_table, ARRAY_SIZE(bo_table)))
		return FALSE;

	if (mask) {
		intel->transform[1] = NULL;
		intel->scale_units[1][0] = -1;
		intel->scale_units[1][1] = -1;
	}

	{
		uint32_t cblend, ablend, blendctl;

		/* If component alpha is active in the mask and the blend operation
		 * uses the source alpha, then we know we don't need the source
		 * value (otherwise we would have hit a fallback earlier), so we
		 * provide the source alpha (src.A * mask.X) as output color.
		 * Conversely, if CA is set and we don't need the source alpha, then
		 * we produce the source value (src.X * mask.X) and the source alpha
		 * is unused..  Otherwise, we provide the non-CA source value
		 * (src.X * mask.A).
		 *
		 * The PICT_FORMAT_RGB(pict) == 0 fixups are not needed on 855+'s a8
		 * pictures, but we need to implement it for 830/845 and there's no
		 * harm done in leaving it in.
		 */
		cblend =
		    TB0C_LAST_STAGE | TB0C_RESULT_SCALE_1X | TB0C_OP_MODULE |
		    TB0C_OUTPUT_WRITE_CURRENT;
		ablend =
		    TB0A_RESULT_SCALE_1X | TB0A_OP_MODULE |
		    TB0A_OUTPUT_WRITE_CURRENT;

		/* Get the source picture's channels into TBx_ARG1 */
		if ((mask_picture != NULL &&
		     mask_picture->componentAlpha &&
		     PICT_FORMAT_RGB(mask_picture->format) &&
		     i830_blend_op[op].src_alpha)
		    || dest_picture->format == PICT_a8) {
			/* Producing source alpha value, so the first set of channels
			 * is src.A instead of src.X.  We also do this if the destination
			 * is a8, in which case src.G is what's written, and the other
			 * channels are ignored.
			 */
			ablend |= TB0A_ARG1_SEL_TEXEL0;
			cblend |= TB0C_ARG1_SEL_TEXEL0 | TB0C_ARG1_REPLICATE_ALPHA;
		} else {
			if (PICT_FORMAT_RGB(source_picture->format) != 0)
				cblend |= TB0C_ARG1_SEL_TEXEL0;
			else
				cblend |= TB0C_ARG1_SEL_ONE | TB0C_ARG1_INVERT;	/* 0.0 */
			ablend |= TB0A_ARG1_SEL_TEXEL0;
		}

		if (mask) {
			cblend |= TB0C_ARG2_SEL_TEXEL1;
			if (dest_picture->format == PICT_a8 ||
			    ! mask_picture->componentAlpha ||
			    ! PICT_FORMAT_RGB(mask_picture->format))
				cblend |= TB0C_ARG2_REPLICATE_ALPHA;
			ablend |= TB0A_ARG2_SEL_TEXEL1;
		} else {
			cblend |= TB0C_ARG2_SEL_ONE;
			ablend |= TB0A_ARG2_SEL_ONE;
		}

		if (!i830_get_blend_cntl
		    (scrn, op, mask_picture, dest_picture->format, &blendctl)) {
			return FALSE;
		}

		intel->cblend = cblend;
		intel->ablend = ablend;
		intel->s8_blendctl = blendctl;
	}

	if (intel_pixmap_is_dirty(source) || intel_pixmap_is_dirty(mask))
		intel_batch_emit_flush(scrn);

	intel->needs_render_state_emit = TRUE;

	return TRUE;
}

static void i830_emit_composite_state(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	uint32_t vf2, tiling_bits;
	uint32_t texcoordfmt = 0;

	intel->needs_render_state_emit = FALSE;

	IntelEmitInvarientState(scrn);
	intel->last_3d = LAST_3D_RENDER;

	assert(intel->in_batch_atomic);

	if (intel_pixmap_tiled(intel->render_dest)) {
		tiling_bits = BUF_3D_TILED_SURFACE;
		if (intel_get_pixmap_private(intel->render_dest)->tiling
				== I915_TILING_Y)
			tiling_bits |= BUF_3D_TILE_WALK_Y;
	} else
		tiling_bits = 0;

	OUT_BATCH(_3DSTATE_BUF_INFO_CMD);
	OUT_BATCH(BUF_3D_ID_COLOR_BACK | tiling_bits |
		  BUF_3D_PITCH(intel_pixmap_pitch(intel->render_dest)));
	OUT_RELOC_PIXMAP(intel->render_dest,
			 I915_GEM_DOMAIN_RENDER, I915_GEM_DOMAIN_RENDER, 0);

	OUT_BATCH(_3DSTATE_DST_BUF_VARS_CMD);
	OUT_BATCH(intel->render_dest_format);

	OUT_BATCH(_3DSTATE_DRAW_RECT_CMD);
	OUT_BATCH(0);
	OUT_BATCH(0);		/* ymin, xmin */
	OUT_BATCH(DRAW_YMAX(intel->render_dest->drawable.height - 1) |
		  DRAW_XMAX(intel->render_dest->drawable.width - 1));
	OUT_BATCH(0);		/* yorig, xorig */

	OUT_BATCH(_3DSTATE_LOAD_STATE_IMMEDIATE_1 |
		  I1_LOAD_S(2) | I1_LOAD_S(3) | I1_LOAD_S(8) | 2);
	if (intel->render_mask)
		vf2 = 2 << 12;	/* 2 texture coord sets */
	else
		vf2 = 1 << 12;
	OUT_BATCH(vf2);		/* number of coordinate sets */
	OUT_BATCH(S3_CULLMODE_NONE | S3_VERTEXHAS_XY);
	OUT_BATCH(S8_ENABLE_COLOR_BLEND | S8_BLENDFUNC_ADD | intel->
		  s8_blendctl | S8_ENABLE_COLOR_BUFFER_WRITE);

	OUT_BATCH(_3DSTATE_INDPT_ALPHA_BLEND_CMD | DISABLE_INDPT_ALPHA_BLEND);

	OUT_BATCH(_3DSTATE_LOAD_STATE_IMMEDIATE_2 |
		  LOAD_TEXTURE_BLEND_STAGE(0) | 1);
	OUT_BATCH(intel->cblend);
	OUT_BATCH(intel->ablend);

	OUT_BATCH(_3DSTATE_ENABLES_1_CMD | DISABLE_LOGIC_OP |
		  DISABLE_STENCIL_TEST | DISABLE_DEPTH_BIAS |
		  DISABLE_SPEC_ADD | DISABLE_FOG | DISABLE_ALPHA_TEST |
		  ENABLE_COLOR_BLEND | DISABLE_DEPTH_TEST);
	/* We have to explicitly say we don't want write disabled */
	OUT_BATCH(_3DSTATE_ENABLES_2_CMD | ENABLE_COLOR_MASK |
		  DISABLE_STENCIL_WRITE | ENABLE_TEX_CACHE |
		  DISABLE_DITHER | ENABLE_COLOR_WRITE | DISABLE_DEPTH_WRITE);

	if (intel_transform_is_affine(intel->render_source_picture->transform))
		texcoordfmt |= (TEXCOORDFMT_2D << 0);
	else
		texcoordfmt |= (TEXCOORDFMT_3D << 0);
	if (intel->render_mask) {
		if (intel_transform_is_affine
		    (intel->render_mask_picture->transform))
			texcoordfmt |= (TEXCOORDFMT_2D << 2);
		else
			texcoordfmt |= (TEXCOORDFMT_3D << 2);
	}
	OUT_BATCH(_3DSTATE_VERTEX_FORMAT_2_CMD | texcoordfmt);

	i830_texture_setup(intel->render_source_picture, intel->render_source, 0);
	if (intel->render_mask) {
		i830_texture_setup(intel->render_mask_picture,
				   intel->render_mask, 1);
	}
}

/* Emit the vertices for a single composite rectangle.
 *
 * This function is no longer shared between i830 and i915 generation code.
 */
static void
i830_emit_composite_primitive(PixmapPtr dest,
			      int srcX, int srcY,
			      int maskX, int maskY,
			      int dstX, int dstY, int w, int h)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(dest->drawable.pScreen);
	intel_screen_private *intel = intel_get_screen_private(scrn);
	Bool is_affine_src, is_affine_mask = TRUE;
	int per_vertex;
	float src_x[3], src_y[3], src_w[3], mask_x[3], mask_y[3], mask_w[3];

	per_vertex = 2;		/* dest x/y */

	{
		float x = srcX, y = srcY;

		is_affine_src = intel_transform_is_affine(intel->transform[0]);
		if (is_affine_src) {
			if (!intel_get_transformed_coordinates(x, y,
							      intel->
							      transform[0],
							      &src_x[0],
							      &src_y[0]))
				return;

			if (!intel_get_transformed_coordinates(x, y + h,
							      intel->
							      transform[0],
							      &src_x[1],
							      &src_y[1]))
				return;

			if (!intel_get_transformed_coordinates(x + w, y + h,
							      intel->
							      transform[0],
							      &src_x[2],
							      &src_y[2]))
				return;

			per_vertex += 2;	/* src x/y */
		} else {
			if (!intel_get_transformed_coordinates_3d(x, y,
								 intel->
								 transform[0],
								 &src_x[0],
								 &src_y[0],
								 &src_w[0]))
				return;

			if (!intel_get_transformed_coordinates_3d(x, y + h,
								 intel->
								 transform[0],
								 &src_x[1],
								 &src_y[1],
								 &src_w[1]))
				return;

			if (!intel_get_transformed_coordinates_3d(x + w, y + h,
								 intel->
								 transform[0],
								 &src_x[2],
								 &src_y[2],
								 &src_w[2]))
				return;

			per_vertex += 3;	/* src x/y/w */
		}
	}

	if (intel->render_mask) {
		float x = maskX, y = maskY;

		is_affine_mask = intel_transform_is_affine(intel->transform[1]);
		if (is_affine_mask) {
			if (!intel_get_transformed_coordinates(x, y,
							      intel->
							      transform[1],
							      &mask_x[0],
							      &mask_y[0]))
				return;

			if (!intel_get_transformed_coordinates(x, y + h,
							      intel->
							      transform[1],
							      &mask_x[1],
							      &mask_y[1]))
				return;

			if (!intel_get_transformed_coordinates(x + w, y + h,
							      intel->
							      transform[1],
							      &mask_x[2],
							      &mask_y[2]))
				return;

			per_vertex += 2;	/* mask x/y */
		} else {
			if (!intel_get_transformed_coordinates_3d(x, y,
								 intel->
								 transform[1],
								 &mask_x[0],
								 &mask_y[0],
								 &mask_w[0]))
				return;

			if (!intel_get_transformed_coordinates_3d(x, y + h,
								 intel->
								 transform[1],
								 &mask_x[1],
								 &mask_y[1],
								 &mask_w[1]))
				return;

			if (!intel_get_transformed_coordinates_3d(x + w, y + h,
								 intel->
								 transform[1],
								 &mask_x[2],
								 &mask_y[2],
								 &mask_w[2]))
				return;

			per_vertex += 3;	/* mask x/y/w */
		}
	}

	if (intel->vertex_count == 0) {
		intel->vertex_index = intel->batch_used;
		OUT_BATCH(PRIM3D_INLINE | PRIM3D_RECTLIST);
	}
	OUT_BATCH_F(dstX + w);
	OUT_BATCH_F(dstY + h);
	OUT_BATCH_F(src_x[2] / intel->scale_units[0][0]);
	OUT_BATCH_F(src_y[2] / intel->scale_units[0][1]);
	if (!is_affine_src) {
		OUT_BATCH_F(src_w[2]);
	}
	if (intel->render_mask) {
		OUT_BATCH_F(mask_x[2] / intel->scale_units[1][0]);
		OUT_BATCH_F(mask_y[2] / intel->scale_units[1][1]);
		if (!is_affine_mask) {
			OUT_BATCH_F(mask_w[2]);
		}
	}

	OUT_BATCH_F(dstX);
	OUT_BATCH_F(dstY + h);
	OUT_BATCH_F(src_x[1] / intel->scale_units[0][0]);
	OUT_BATCH_F(src_y[1] / intel->scale_units[0][1]);
	if (!is_affine_src) {
		OUT_BATCH_F(src_w[1]);
	}
	if (intel->render_mask) {
		OUT_BATCH_F(mask_x[1] / intel->scale_units[1][0]);
		OUT_BATCH_F(mask_y[1] / intel->scale_units[1][1]);
		if (!is_affine_mask) {
			OUT_BATCH_F(mask_w[1]);
		}
	}

	OUT_BATCH_F(dstX);
	OUT_BATCH_F(dstY);
	OUT_BATCH_F(src_x[0] / intel->scale_units[0][0]);
	OUT_BATCH_F(src_y[0] / intel->scale_units[0][1]);
	if (!is_affine_src) {
		OUT_BATCH_F(src_w[0]);
	}
	if (intel->render_mask) {
		OUT_BATCH_F(mask_x[0] / intel->scale_units[1][0]);
		OUT_BATCH_F(mask_y[0] / intel->scale_units[1][1]);
		if (!is_affine_mask) {
			OUT_BATCH_F(mask_w[0]);
		}
	}

	intel->vertex_count += 3 * per_vertex;

}

void i830_vertex_flush(intel_screen_private *intel)
{
	if (intel->vertex_count) {
		intel->batch_ptr[intel->vertex_index] |= intel->vertex_count - 1;
		intel->vertex_count = 0;
	}
}

/**
 * Do a single rectangle composite operation.
 */
void
i830_composite(PixmapPtr dest, int srcX, int srcY, int maskX, int maskY,
	       int dstX, int dstY, int w, int h)
{
	ScrnInfoPtr scrn = xf86ScreenToScrn(dest->drawable.pScreen);
	intel_screen_private *intel = intel_get_screen_private(scrn);

	intel_batch_start_atomic(scrn, 58 +	/* invarient */
				 22 +	/* setup */
				 20 +	/* 2 * setup_texture */
				 1 + 30 /* verts */ );

	if (intel->needs_render_state_emit)
		i830_emit_composite_state(scrn);

	i830_emit_composite_primitive(dest, srcX, srcY, maskX, maskY, dstX,
				      dstY, w, h);

	intel_batch_end_atomic(scrn);
}

void i830_batch_commit_notify(intel_screen_private *intel)
{
	intel->needs_render_state_emit = TRUE;
}
