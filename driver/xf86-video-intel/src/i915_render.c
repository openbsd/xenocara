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
#include "i830.h"
#include "i915_reg.h"
#include "i915_3d.h"

#define PIXEL_CENTRE_SAMPLE 0

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
	{PICT_a8r8g8b8, MAPSURF_32BIT | MT_32BIT_ARGB8888},
	{PICT_x8r8g8b8, MAPSURF_32BIT | MT_32BIT_XRGB8888},
	{PICT_a8b8g8r8, MAPSURF_32BIT | MT_32BIT_ABGR8888},
	{PICT_x8b8g8r8, MAPSURF_32BIT | MT_32BIT_XBGR8888},
	{PICT_r5g6b5, MAPSURF_16BIT | MT_16BIT_RGB565},
	{PICT_a1r5g5b5, MAPSURF_16BIT | MT_16BIT_ARGB1555},
	{PICT_x1r5g5b5, MAPSURF_16BIT | MT_16BIT_ARGB1555},
	{PICT_a4r4g4b4, MAPSURF_16BIT | MT_16BIT_ARGB4444},
	{PICT_x4r4g4b4, MAPSURF_16BIT | MT_16BIT_ARGB4444},
	{PICT_a8, MAPSURF_8BIT | MT_8BIT_A8},
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

	return (sblend << S6_CBUF_SRC_BLEND_FACT_SHIFT) |
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
	case PICT_a8:
		*dst_format = COLR_BUF_8BIT;
		break;
	case PICT_a4r4g4b4:
	case PICT_x4r4g4b4:
		*dst_format = COLR_BUF_ARGB4444;
		break;
	default:
		scrn = xf86Screens[dest_picture->pDrawable->pScreen->myNum];
		intel_debug_fallback(scrn,
				     "Unsupported dest format 0x%x\n",
				     (int)dest_picture->format);
		return FALSE;
	}
#if PIXEL_CENTRE_SAMPLE
	*dst_format |= DSTORG_HORT_BIAS(0x8) | DSTORG_VERT_BIAS(0x8);
#endif
	return TRUE;
}

static Bool i915_check_composite_texture(ScrnInfoPtr scrn, PicturePtr picture,
					 int unit)
{
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
		int w, h, i;

		w = picture->pDrawable->width;
		h = picture->pDrawable->height;
		if ((w > 2048) || (h > 2048)) {
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
			intel_debug_fallback(scrn, "Unsupported picture format "
					     "0x%x\n",
					     (int)picture->format);
			return FALSE;
		}
	}

	return TRUE;
}

Bool
i915_check_composite(int op, PicturePtr source_picture, PicturePtr mask_picture,
		     PicturePtr dest_picture)
{
	ScrnInfoPtr scrn = xf86Screens[dest_picture->pDrawable->pScreen->myNum];
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
			intel_debug_fallback(scrn,
					     "Component alpha not supported "
					     "with source alpha and source "
					     "value blending.\n");
			return FALSE;
		}
	}

	if (!i915_check_composite_texture(scrn, source_picture, 0)) {
		intel_debug_fallback(scrn, "Check Src picture texture\n");
		return FALSE;
	}
	if (mask_picture != NULL
	    && !i915_check_composite_texture(scrn, mask_picture, 1)) {
		intel_debug_fallback(scrn, "Check Mask picture texture\n");
		return FALSE;
	}

	if (!i915_get_dest_format(dest_picture, &tmp1)) {
		intel_debug_fallback(scrn, "Get Color buffer format\n");
		return FALSE;
	}

	return TRUE;
}

static Bool i915_texture_setup(PicturePtr picture, PixmapPtr pixmap, int unit)
{
	ScrnInfoPtr scrn = xf86Screens[picture->pDrawable->pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	uint32_t format, pitch, filter;
	int w, h, i;
	uint32_t wrap_mode, tiling_bits;

	pitch = intel_get_pixmap_pitch(pixmap);
	w = picture->pDrawable->width;
	h = picture->pDrawable->height;
	intel->scale_units[unit][0] = pixmap->drawable.width;
	intel->scale_units[unit][1] = pixmap->drawable.height;

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
		filter = 0;
		intel_debug_fallback(scrn, "Bad filter 0x%x\n",
				     picture->filter);
		return FALSE;
	}

	/* offset filled in at emit time */
	if (i830_pixmap_tiled(pixmap)) {
		tiling_bits = MS3_TILED_SURFACE;
		if (i830_get_pixmap_intel(pixmap)->tiling
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

Bool
i915_prepare_composite(int op, PicturePtr source_picture,
		       PicturePtr mask_picture, PicturePtr dest_picture,
		       PixmapPtr source, PixmapPtr mask, PixmapPtr dest)
{
	ScrnInfoPtr scrn = xf86Screens[source_picture->pDrawable->pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	drm_intel_bo *bo_table[] = {
		NULL,		/* batch_bo */
		i830_get_pixmap_bo(source),
		mask ? i830_get_pixmap_bo(mask) : NULL,
		i830_get_pixmap_bo(dest),
	};
	int tex_unit = 0;

	intel->render_source_picture = source_picture;
	intel->render_source = source;
	intel->render_mask_picture = mask_picture;
	intel->render_mask = mask;
	intel->render_dest_picture = dest_picture;
	intel->render_dest = dest;

	intel->render_source_is_solid =
	    source_picture->pDrawable &&
	    source_picture->pDrawable->width == 1 &&
	    source_picture->pDrawable->height == 1 &&
	    source_picture->repeat;

	if (intel->render_source_is_solid) {
	    if (! uxa_get_color_for_pixmap (source,
					    source_picture->format,
					    PICT_a8r8g8b8,
					    &intel->render_source_solid))
		intel->render_source_is_solid = FALSE;
	}
	if (!intel->render_source_is_solid && !intel_check_pitch_3d(source))
		return FALSE;


	intel->render_mask_is_solid = TRUE; /* mask == NULL => opaque */
	if (mask) {
	    intel->render_mask_is_solid =
		mask_picture->pDrawable &&
		mask_picture->pDrawable->width == 1 &&
		mask_picture->pDrawable->height == 1 &&
		mask_picture->repeat;
	    if (intel->render_mask_is_solid) {
		if (! uxa_get_color_for_pixmap (mask,
						mask_picture->format,
						PICT_a8r8g8b8,
						&intel->render_mask_solid))
		    intel->render_mask_is_solid = FALSE;
	    }
	    if (!intel->render_mask_is_solid && !intel_check_pitch_3d(mask))
		    return FALSE;
	}

	if (!intel_check_pitch_3d(dest))
		return FALSE;

	if (!i915_get_dest_format(dest_picture,
				  &intel->i915_render_state.dst_format))
		return FALSE;

	if (!i830_get_aperture_space(scrn, bo_table, ARRAY_SIZE(bo_table)))
		return FALSE;
	intel->dst_coord_adjust = 0;
	intel->src_coord_adjust = 0;
	intel->mask_coord_adjust = 0;

	intel->transform[0] = NULL;
	intel->scale_units[0][0] = -1;
	intel->scale_units[0][1] = -1;
	intel->transform[1] = NULL;
	intel->scale_units[1][0] = -1;
	intel->scale_units[1][1] = -1;

	if (! intel->render_source_is_solid) {
		if (!i915_texture_setup(source_picture, source, tex_unit++)) {
			intel_debug_fallback(scrn, "fail to setup src texture\n");
			return FALSE;
		}

		if (source_picture->filter == PictFilterNearest) {
#if PIXEL_CENTRE_SAMPLE
			intel->src_coord_adjust = 0.375;
#else
			intel->dst_coord_adjust = -0.125;
#endif
		}
	}

	if (mask != NULL) {
		if (! intel->render_mask_is_solid) {
			if (!i915_texture_setup(mask_picture, mask, tex_unit++)) {
				intel_debug_fallback(scrn,
						"fail to setup mask texture\n");
				return FALSE;
			}

			if (mask_picture->filter == PictFilterNearest) {
#if PIXEL_CENTRE_SAMPLE
			    intel->mask_coord_adjust = 0.375;
#else
			    intel->dst_coord_adjust = -0.125;
#endif
			}
		}
	}

	intel->i915_render_state.op = op;

	if(i830_uxa_pixmap_is_dirty(source) ||
	   (mask && i830_uxa_pixmap_is_dirty(mask)))
		intel_batch_emit_flush(scrn);

	intel->needs_render_state_emit = TRUE;

	return TRUE;
}

static void i915_emit_composite_setup(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	int op = intel->i915_render_state.op;
	PicturePtr mask_picture = intel->render_mask_picture;
	PicturePtr dest_picture = intel->render_dest_picture;
	PixmapPtr mask = intel->render_mask;
	PixmapPtr dest = intel->render_dest;
	uint32_t dst_format = intel->i915_render_state.dst_format, dst_pitch;
	uint32_t blendctl, tiling_bits;
	Bool is_affine_src, is_affine_mask;
	Bool is_solid_src, is_solid_mask;
	int tex_count, t;

	intel->needs_render_state_emit = FALSE;

	IntelEmitInvarientState(scrn);
	intel->last_3d = LAST_3D_RENDER;

	dst_pitch = intel_get_pixmap_pitch(dest);

	is_affine_src = i830_transform_is_affine(intel->transform[0]);
	is_affine_mask = i830_transform_is_affine(intel->transform[1]);

	is_solid_src = intel->render_source_is_solid;
	is_solid_mask = intel->render_mask_is_solid;

	tex_count = 0;
	tex_count += ! is_solid_src;
	tex_count += mask && ! is_solid_mask;

	t = 15;
	if (tex_count)
	    t += 6 * tex_count + 4;
	if (is_solid_src)
	    t += 2;
	if (mask && is_solid_mask)
	    t += 2;
	ATOMIC_BATCH (t);

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

	if (is_solid_src) {
	    OUT_BATCH (_3DSTATE_DFLT_DIFFUSE_CMD);
	    OUT_BATCH (intel->render_source_solid);
	}
	if (mask && is_solid_mask) {
	    OUT_BATCH (_3DSTATE_DFLT_SPEC_CMD);
	    OUT_BATCH (intel->render_mask_solid);
	}

	if (i830_pixmap_tiled(dest)) {
		tiling_bits = BUF_3D_TILED_SURFACE;
		if (i830_get_pixmap_intel(dest)->tiling
				== I915_TILING_Y)
			tiling_bits |= BUF_3D_TILE_WALK_Y;
	} else
		tiling_bits = 0;

	OUT_BATCH(_3DSTATE_BUF_INFO_CMD);
	OUT_BATCH(BUF_3D_ID_COLOR_BACK | tiling_bits |
		  BUF_3D_PITCH(dst_pitch));
	OUT_RELOC_PIXMAP(dest, I915_GEM_DOMAIN_RENDER,
			 I915_GEM_DOMAIN_RENDER, 0);

	OUT_BATCH(_3DSTATE_DST_BUF_VARS_CMD);
	OUT_BATCH(dst_format);

	{
		uint32_t ss2;

		OUT_BATCH(_3DSTATE_LOAD_STATE_IMMEDIATE_1 | I1_LOAD_S(2) |
			  I1_LOAD_S(4) | I1_LOAD_S(5) | I1_LOAD_S(6) | 3);
		ss2 = ~0;
		t = 0;
		if (! is_solid_src) {
		    ss2 &= ~S2_TEXCOORD_FMT(t, TEXCOORDFMT_NOT_PRESENT);
		    ss2 |= S2_TEXCOORD_FMT(t,
					   is_affine_src ? TEXCOORDFMT_2D :
					   TEXCOORDFMT_4D);
		    t++;
		}
		if (mask && ! is_solid_mask) {
		    ss2 &= ~S2_TEXCOORD_FMT(t, TEXCOORDFMT_NOT_PRESENT);
		    ss2 |= S2_TEXCOORD_FMT(t,
			    is_affine_mask ? TEXCOORDFMT_2D :
			    TEXCOORDFMT_4D);
		    t++;
		}
		OUT_BATCH(ss2);
		OUT_BATCH((1 << S4_POINT_WIDTH_SHIFT) | S4_LINE_WIDTH_ONE |
			  S4_CULLMODE_NONE | S4_VFMT_XY);
		blendctl =
		    i915_get_blend_cntl(op, mask_picture, dest_picture->format);
		OUT_BATCH(0x00000000);	/* Disable stencil buffer */
		OUT_BATCH(S6_CBUF_BLEND_ENABLE | S6_COLOR_WRITE_ENABLE |
			  (BLENDFUNC_ADD << S6_CBUF_BLEND_FUNC_SHIFT) |
			  blendctl);

		/* draw rect is unconditional */
		OUT_BATCH(_3DSTATE_DRAW_RECT_CMD);
		OUT_BATCH(0x00000000);
		OUT_BATCH(0x00000000);	/* ymin, xmin */
		OUT_BATCH(DRAW_YMAX(dest->drawable.height - 1) |
			  DRAW_XMAX(dest->drawable.width - 1));
		/* yorig, xorig (relate to color buffer?) */
		OUT_BATCH(0x00000000);
	}

	ADVANCE_BATCH();

	{
	    FS_LOCALS(20);
	    int src_reg, mask_reg, out_reg = FS_OC;

	    FS_BEGIN();

	    if (dst_format == COLR_BUF_8BIT)
		out_reg = FS_U0;

	    /* Declare the registers necessary for our program.  */
	    t = 0;
	    if (is_solid_src) {
		i915_fs_dcl(FS_T8);
		src_reg = FS_T8;
	    } else {
		i915_fs_dcl(FS_T0);
		i915_fs_dcl(FS_S0);
		t++;
	    }
	    if (mask) {
		if (is_solid_mask) {
		    i915_fs_dcl(FS_T9);
		    mask_reg = FS_T9;
		} else {
		    i915_fs_dcl(FS_T0 + t);
		    i915_fs_dcl(FS_S0 + t);
		}
	    }

	    /* Load the source_picture texel */
	    if (! is_solid_src) {
		if (is_affine_src) {
		    i915_fs_texld(FS_R0, FS_S0, FS_T0);
		} else {
		    i915_fs_texldp(FS_R0, FS_S0, FS_T0);
		}

		src_reg = FS_R0;
	    }

	    if (!mask) {
		/* No mask, so move to output color */
		i915_fs_mov(out_reg, i915_fs_operand_reg(src_reg));
	    } else {
		if (! is_solid_mask) {
		    /* Load the mask_picture texel */
		    if (is_affine_mask) {
			i915_fs_texld(FS_R1, FS_S0 + t, FS_T0 + t);
		    } else {
			i915_fs_texldp(FS_R1, FS_S0 + t, FS_T0 + t);
		    }

		    mask_reg = FS_R1;
		}

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
			i915_fs_mul(out_reg,
				    i915_fs_operand(src_reg, W, W, W, W),
				    i915_fs_operand_reg(mask_reg));
		    } else {
			    i915_fs_mul(out_reg,
					i915_fs_operand_reg(src_reg),
					i915_fs_operand_reg(mask_reg));
		    }
		} else {
		    i915_fs_mul(out_reg,
				i915_fs_operand_reg(src_reg),
				i915_fs_operand(mask_reg, W, W, W, W));
		}
	    }
	    if (dst_format == COLR_BUF_8BIT)
		i915_fs_mov(FS_OC, i915_fs_operand(out_reg, W, W, W, W));

	    FS_END();
	}
}

/* Emit the vertices for a single composite rectangle.
 *
 * This function is no longer shared between i830 and i915 generation code.
 */
static void
i915_emit_composite_primitive(PixmapPtr dest,
			      int srcX, int srcY,
			      int maskX, int maskY,
			      int dstX, int dstY, int w, int h)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);
	Bool is_affine_src, is_affine_mask = TRUE;
	int per_vertex, num_floats;
	int tex_unit = 0;
	int src_unit = -1, mask_unit = -1;
	float src_x[3], src_y[3], src_w[3], mask_x[3], mask_y[3], mask_w[3];

	per_vertex = 2;		/* dest x/y */

	if (! intel->render_source_is_solid) {
		float x = srcX + intel->src_coord_adjust;
		float y = srcY + intel->src_coord_adjust;

		src_unit = tex_unit++;

		is_affine_src = i830_transform_is_affine(intel->transform[src_unit]);
		if (is_affine_src) {
			if (!i830_get_transformed_coordinates(x, y,
							      intel->
							      transform[src_unit],
							      &src_x[0],
							      &src_y[0]))
				return;

			if (!i830_get_transformed_coordinates(x, y + h,
							      intel->
							      transform[src_unit],
							      &src_x[1],
							      &src_y[1]))
				return;

			if (!i830_get_transformed_coordinates(x + w, y + h,
							      intel->
							      transform[src_unit],
							      &src_x[2],
							      &src_y[2]))
				return;

			per_vertex += 2;	/* src x/y */
		} else {
			if (!i830_get_transformed_coordinates_3d(x, y,
								 intel->
								 transform[src_unit],
								 &src_x[0],
								 &src_y[0],
								 &src_w[0]))
				return;

			if (!i830_get_transformed_coordinates_3d(x, y + h,
								 intel->
								 transform[src_unit],
								 &src_x[1],
								 &src_y[1],
								 &src_w[1]))
				return;

			if (!i830_get_transformed_coordinates_3d(x + w, y + h,
								 intel->
								 transform[src_unit],
								 &src_x[2],
								 &src_y[2],
								 &src_w[2]))
				return;

			per_vertex += 4;	/* src x/y/z/w */
		}
	}

	if (intel->render_mask && ! intel->render_mask_is_solid) {
		float x = maskX + intel->mask_coord_adjust;
		float y = maskY + intel->mask_coord_adjust;

		mask_unit = tex_unit++;

		is_affine_mask = i830_transform_is_affine(intel->transform[mask_unit]);
		if (is_affine_mask) {
			if (!i830_get_transformed_coordinates(x, y,
							      intel->
							      transform[mask_unit],
							      &mask_x[0],
							      &mask_y[0]))
				return;

			if (!i830_get_transformed_coordinates(x, y + h,
							      intel->
							      transform[mask_unit],
							      &mask_x[1],
							      &mask_y[1]))
				return;

			if (!i830_get_transformed_coordinates(x + w, y + h,
							      intel->
							      transform[mask_unit],
							      &mask_x[2],
							      &mask_y[2]))
				return;

			per_vertex += 2;	/* mask x/y */
		} else {
			if (!i830_get_transformed_coordinates_3d(x, y,
								 intel->
								 transform[mask_unit],
								 &mask_x[0],
								 &mask_y[0],
								 &mask_w[0]))
				return;

			if (!i830_get_transformed_coordinates_3d(x, y + h,
								 intel->
								 transform[mask_unit],
								 &mask_x[1],
								 &mask_y[1],
								 &mask_w[1]))
				return;

			if (!i830_get_transformed_coordinates_3d(x + w, y + h,
								 intel->
								 transform[mask_unit],
								 &mask_x[2],
								 &mask_y[2],
								 &mask_w[2]))
				return;

			per_vertex += 4;	/* mask x/y/z/w */
		}
	}

	num_floats = 3 * per_vertex;

	ATOMIC_BATCH(1 + num_floats);

	OUT_BATCH(PRIM3D_INLINE | PRIM3D_RECTLIST | (num_floats - 1));
	OUT_BATCH_F(intel->dst_coord_adjust + dstX + w);
	OUT_BATCH_F(intel->dst_coord_adjust + dstY + h);
	if (! intel->render_source_is_solid) {
	    OUT_BATCH_F(src_x[2] / intel->scale_units[src_unit][0]);
	    OUT_BATCH_F(src_y[2] / intel->scale_units[src_unit][1]);
	    if (!is_affine_src) {
		OUT_BATCH_F(0.0);
		OUT_BATCH_F(src_w[2]);
	    }
	}
	if (intel->render_mask && ! intel->render_mask_is_solid) {
		OUT_BATCH_F(mask_x[2] / intel->scale_units[mask_unit][0]);
		OUT_BATCH_F(mask_y[2] / intel->scale_units[mask_unit][1]);
		if (!is_affine_mask) {
			OUT_BATCH_F(0.0);
			OUT_BATCH_F(mask_w[2]);
		}
	}

	OUT_BATCH_F(intel->dst_coord_adjust + dstX);
	OUT_BATCH_F(intel->dst_coord_adjust + dstY + h);
	if (! intel->render_source_is_solid) {
	    OUT_BATCH_F(src_x[1] / intel->scale_units[src_unit][0]);
	    OUT_BATCH_F(src_y[1] / intel->scale_units[src_unit][1]);
	    if (!is_affine_src) {
		OUT_BATCH_F(0.0);
		OUT_BATCH_F(src_w[1]);
	    }
	}
	if (intel->render_mask && ! intel->render_mask_is_solid) {
		OUT_BATCH_F(mask_x[1] / intel->scale_units[mask_unit][0]);
		OUT_BATCH_F(mask_y[1] / intel->scale_units[mask_unit][1]);
		if (!is_affine_mask) {
			OUT_BATCH_F(0.0);
			OUT_BATCH_F(mask_w[1]);
		}
	}

	OUT_BATCH_F(intel->dst_coord_adjust + dstX);
	OUT_BATCH_F(intel->dst_coord_adjust + dstY);
	if (! intel->render_source_is_solid) {
	    OUT_BATCH_F(src_x[0] / intel->scale_units[src_unit][0]);
	    OUT_BATCH_F(src_y[0] / intel->scale_units[src_unit][1]);
	    if (!is_affine_src) {
		OUT_BATCH_F(0.0);
		OUT_BATCH_F(src_w[0]);
	    }
	}
	if (intel->render_mask && ! intel->render_mask_is_solid) {
		OUT_BATCH_F(mask_x[0] / intel->scale_units[mask_unit][0]);
		OUT_BATCH_F(mask_y[0] / intel->scale_units[mask_unit][1]);
		if (!is_affine_mask) {
			OUT_BATCH_F(0.0);
			OUT_BATCH_F(mask_w[0]);
		}
	}

	ADVANCE_BATCH();
}

void
i915_composite(PixmapPtr dest, int srcX, int srcY, int maskX, int maskY,
	       int dstX, int dstY, int w, int h)
{
	ScrnInfoPtr scrn = xf86Screens[dest->drawable.pScreen->myNum];
	intel_screen_private *intel = intel_get_screen_private(scrn);

	/* 28 + 16 + 10 + 20 + 32 + 16 */
	intel_batch_start_atomic(scrn, 150);

	if (intel->needs_render_state_emit)
		i915_emit_composite_setup(scrn);

	i915_emit_composite_primitive(dest, srcX, srcY, maskX, maskY, dstX,
				      dstY, w, h);

	intel_batch_end_atomic(scrn);
}

void i915_batch_flush_notify(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	intel->needs_render_state_emit = TRUE;
}
