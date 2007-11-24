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
#include "i830_reg.h"

#ifdef I830DEBUG
#define DEBUG_I830FALLBACK 1
#endif

#ifdef DEBUG_I830FALLBACK
#define I830FALLBACK(s, arg...)				\
do {							\
	DPRINTF(PFX, "EXA fallback: " s "\n", ##arg); 	\
	return FALSE;					\
} while(0)
#else
#define I830FALLBACK(s, arg...) 			\
do { 							\
	return FALSE;					\
} while(0)
#endif

struct blendinfo {
    Bool dst_alpha;
    Bool src_alpha;
    CARD32 src_blend;
    CARD32 dst_blend;
};

struct formatinfo {
    int fmt;
    CARD32 card_fmt;
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
    {0, 0, BLENDFACTOR_ZERO, 		BLENDFACTOR_ZERO},
    /* Src */
    {0, 0, BLENDFACTOR_ONE, 		BLENDFACTOR_ZERO},
    /* Dst */
    {0, 0, BLENDFACTOR_ZERO,		BLENDFACTOR_ONE},
    /* Over */
    {0, 1, BLENDFACTOR_ONE,		BLENDFACTOR_INV_SRC_ALPHA},
    /* OverReverse */
    {1, 0, BLENDFACTOR_INV_DST_ALPHA,	BLENDFACTOR_ONE},
    /* In */
    {1, 0, BLENDFACTOR_DST_ALPHA,	BLENDFACTOR_ZERO},
    /* InReverse */
    {0, 1, BLENDFACTOR_ZERO,		BLENDFACTOR_SRC_ALPHA},
    /* Out */
    {1, 0, BLENDFACTOR_INV_DST_ALPHA,	BLENDFACTOR_ZERO},
    /* OutReverse */
    {0, 1, BLENDFACTOR_ZERO,		BLENDFACTOR_INV_SRC_ALPHA},
    /* Atop */
    {1, 1, BLENDFACTOR_DST_ALPHA,	BLENDFACTOR_INV_SRC_ALPHA},
    /* AtopReverse */
    {1, 1, BLENDFACTOR_INV_DST_ALPHA,	BLENDFACTOR_SRC_ALPHA},
    /* Xor */
    {1, 1, BLENDFACTOR_INV_DST_ALPHA,	BLENDFACTOR_INV_SRC_ALPHA},
    /* Add */
    {0, 0, BLENDFACTOR_ONE, 		BLENDFACTOR_ONE},
};

static struct formatinfo i830_tex_formats[] = {
    {PICT_a8r8g8b8, MT_32BIT_ARGB8888 },
    {PICT_x8r8g8b8, MT_32BIT_XRGB8888 },
    {PICT_a8b8g8r8, MT_32BIT_ABGR8888 },
    {PICT_x8b8g8r8, MT_32BIT_XBGR8888 },
    {PICT_r5g6b5,   MT_16BIT_RGB565   },
    {PICT_a1r5g5b5, MT_16BIT_ARGB1555 },
    {PICT_x1r5g5b5, MT_16BIT_ARGB1555 },
    {PICT_a8,       MT_8BIT_A8        },
};

static Bool i830_get_dest_format(PicturePtr pDstPicture, CARD32 *dst_format)
{
    switch (pDstPicture->format) {
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
	/*
    case PICT_a8:
        *dst_format = COLR_BUF_8BIT;
        break;
	*/
    case PICT_a4r4g4b4:
    case PICT_x4r4g4b4:
	*dst_format = COLR_BUF_ARGB4444;
	break;
    default:
        I830FALLBACK("Unsupported dest format 0x%x\n",
		     (int)pDstPicture->format);
    }

    return TRUE;
}


static CARD32 i830_get_blend_cntl(int op, PicturePtr pMask, CARD32 dst_format)
{
    CARD32 sblend, dblend;

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

    /* If the source alpha is being used, then we should only be in a case
     * where the source blend factor is 0, and the source blend value is the
     * mask channels multiplied by the source picture's alpha.
     */
    if (pMask && pMask->componentAlpha && PICT_FORMAT_RGB(pMask->format) 
	    && i830_blend_op[op].src_alpha) {
        if (dblend == BLENDFACTOR_SRC_ALPHA) {
            dblend = BLENDFACTOR_SRC_COLR;
        } else if (dblend == BLENDFACTOR_INV_SRC_ALPHA) {
            dblend = BLENDFACTOR_INV_SRC_COLR;
        }
    }

    return (sblend << S8_SRC_BLEND_FACTOR_SHIFT) |
	(dblend << S8_DST_BLEND_FACTOR_SHIFT);
}

static Bool i830_check_composite_texture(PicturePtr pPict, int unit)
{
    ScrnInfoPtr pScrn = xf86Screens[pPict->pDrawable->pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    int i;

    if ((w > 0x7ff) || (h > 0x7ff))
        I830FALLBACK("Picture w/h too large (%dx%d)\n", w, h);

    for (i = 0; i < sizeof(i830_tex_formats) / sizeof(i830_tex_formats[0]);
	 i++)
    {
        if (i830_tex_formats[i].fmt == pPict->format)
            break;
    }
    if (i == sizeof(i830_tex_formats) / sizeof(i830_tex_formats[0]))
        I830FALLBACK("Unsupported picture format 0x%x\n",
		     (int)pPict->format);

    if (IS_I830(pI830) || IS_845G(pI830)) {
	if (pPict->format == PICT_x8r8g8b8 || 
		pPict->format == PICT_x8b8g8r8 || 
		pPict->format == PICT_a8)
	    I830FALLBACK("830/845G don't support a8, x8r8g8b8, x8b8g8r8\n");
    }

    if (pPict->repeat && pPict->repeatType != RepeatNormal)
	I830FALLBACK("unsupport repeat type\n");

    if (pPict->filter != PictFilterNearest &&
        pPict->filter != PictFilterBilinear)
    {
        I830FALLBACK("Unsupported filter 0x%x\n", pPict->filter);
    }

    return TRUE;
}

static CARD32
i8xx_get_card_format(PicturePtr pPict)
{
    int i;
    for (i = 0; i < sizeof(i830_tex_formats) / sizeof(i830_tex_formats[0]);
	    i++)
    {
	if (i830_tex_formats[i].fmt == pPict->format)
	    break;
    }
    return i830_tex_formats[i].card_fmt;
}

static Bool
i830_texture_setup(PicturePtr pPict, PixmapPtr pPix, int unit)
{

    ScrnInfoPtr pScrn = xf86Screens[pPict->pDrawable->pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    CARD32 format, offset, pitch, filter;
    CARD32 wrap_mode = TEXCOORDMODE_CLAMP_BORDER;

    offset = intel_get_pixmap_offset(pPix);
    pitch = intel_get_pixmap_pitch(pPix);
    pI830->scale_units[unit][0] = pPix->drawable.width;
    pI830->scale_units[unit][1] = pPix->drawable.height;
    pI830->transform[unit] = pPict->transform;

    format = i8xx_get_card_format(pPict);

    if (pPict->repeat)
	wrap_mode = TEXCOORDMODE_WRAP;

    switch (pPict->filter) {
    case PictFilterNearest:
        filter = ((FILTER_NEAREST<<TM0S3_MAG_FILTER_SHIFT) |
		  (FILTER_NEAREST<<TM0S3_MIN_FILTER_SHIFT));
        break;
    case PictFilterBilinear:
        filter = ((FILTER_LINEAR<<TM0S3_MAG_FILTER_SHIFT) |
		  (FILTER_LINEAR<<TM0S3_MIN_FILTER_SHIFT));
        break;
    default:
	filter = 0;
        I830FALLBACK("Bad filter 0x%x\n", pPict->filter);
    }
    filter |= (MIPFILTER_NONE << TM0S3_MIP_FILTER_SHIFT); 

    {
	if (pPix->drawable.bitsPerPixel == 8)
	    format |= MAPSURF_8BIT;
	else if (pPix->drawable.bitsPerPixel == 16)
	    format |= MAPSURF_16BIT;
	else
	    format |= MAPSURF_32BIT;

	BEGIN_LP_RING(10);
	OUT_RING(_3DSTATE_LOAD_STATE_IMMEDIATE_2 | LOAD_TEXTURE_MAP(unit) | 4);
	OUT_RING((offset & TM0S0_ADDRESS_MASK) | TM0S0_USE_FENCE); 
	OUT_RING(((pPix->drawable.height - 1) << TM0S1_HEIGHT_SHIFT) |
		((pPix->drawable.width - 1) << TM0S1_WIDTH_SHIFT) | format);
	OUT_RING((pitch/4 - 1) << TM0S2_PITCH_SHIFT | TM0S2_MAP_2D);
	OUT_RING(filter);
	OUT_RING(0); /* default color */
	OUT_RING(_3DSTATE_MAP_COORD_SET_CMD | TEXCOORD_SET(unit) |
		 ENABLE_TEXCOORD_PARAMS | TEXCOORDS_ARE_NORMAL |
		 TEXCOORDTYPE_CARTESIAN | ENABLE_ADDR_V_CNTL |
		 TEXCOORD_ADDR_V_MODE(wrap_mode) |
		 ENABLE_ADDR_U_CNTL | TEXCOORD_ADDR_U_MODE(wrap_mode));
	/* map texel stream */
	OUT_RING(_3DSTATE_MAP_COORD_SETBIND_CMD);
	if (unit == 0)
	    OUT_RING(TEXBIND_SET0(TEXCOORDSRC_VTXSET_0) |
		    TEXBIND_SET1(TEXCOORDSRC_KEEP) |
		    TEXBIND_SET2(TEXCOORDSRC_KEEP) |
		    TEXBIND_SET3(TEXCOORDSRC_KEEP));
	else
	    OUT_RING(TEXBIND_SET0(TEXCOORDSRC_VTXSET_0) |
		    TEXBIND_SET1(TEXCOORDSRC_VTXSET_1) |
		    TEXBIND_SET2(TEXCOORDSRC_KEEP) |
		    TEXBIND_SET3(TEXCOORDSRC_KEEP));
	OUT_RING(_3DSTATE_MAP_TEX_STREAM_CMD | (unit << 16) |
		DISABLE_TEX_STREAM_BUMP | 
		ENABLE_TEX_STREAM_COORD_SET |
		TEX_STREAM_COORD_SET(unit) |
		ENABLE_TEX_STREAM_MAP_IDX |
		TEX_STREAM_MAP_IDX(unit));
	ADVANCE_LP_RING();
     }

#ifdef I830DEBUG
    ErrorF("try to sync to show any errors...");
    I830Sync(pScrn);
#endif

    return TRUE;
}

Bool
i830_check_composite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
		     PicturePtr pDstPicture)
{
    CARD32 tmp1;

    /* Check for unsupported compositing operations. */
    if (op >= sizeof(i830_blend_op) / sizeof(i830_blend_op[0]))
        I830FALLBACK("Unsupported Composite op 0x%x\n", op);

    if (pMaskPicture != NULL && pMaskPicture->componentAlpha &&
	    PICT_FORMAT_RGB(pMaskPicture->format)) {
        /* Check if it's component alpha that relies on a source alpha and on
         * the source value.  We can only get one of those into the single
         * source value that we get to blend with.
         */
        if (i830_blend_op[op].src_alpha &&
            (i830_blend_op[op].src_blend != BLENDFACTOR_ZERO))
            	I830FALLBACK("Component alpha not supported with source "
			     "alpha and source value blending.\n");
    }

    if (!i830_check_composite_texture(pSrcPicture, 0))
        I830FALLBACK("Check Src picture texture\n");
    if (pMaskPicture != NULL && !i830_check_composite_texture(pMaskPicture, 1))
        I830FALLBACK("Check Mask picture texture\n");

    if (!i830_get_dest_format(pDstPicture, &tmp1))
	I830FALLBACK("Get Color buffer format\n");

    return TRUE;
}

Bool
i830_prepare_composite(int op, PicturePtr pSrcPicture,
		       PicturePtr pMaskPicture, PicturePtr pDstPicture,
		       PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
    ScrnInfoPtr pScrn = xf86Screens[pSrcPicture->pDrawable->pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    CARD32 dst_format, dst_offset, dst_pitch;

    IntelEmitInvarientState(pScrn);
    *pI830->last_3d = LAST_3D_RENDER;

    i830_get_dest_format(pDstPicture, &dst_format);
    dst_offset = intel_get_pixmap_offset(pDst);
    dst_pitch = intel_get_pixmap_pitch(pDst);

    if (!i830_texture_setup(pSrcPicture, pSrc, 0))
	I830FALLBACK("fail to setup src texture\n");
    if (pMask != NULL) {
	if (!i830_texture_setup(pMaskPicture, pMask, 1))
	    I830FALLBACK("fail to setup mask texture\n");
    } else {
	pI830->transform[1] = NULL;
	pI830->scale_units[1][0] = -1;
	pI830->scale_units[1][1] = -1;
    }

    {
	CARD32 cblend, ablend, blendctl, vf2;

	BEGIN_LP_RING(30);

	/* color buffer */
	OUT_RING(_3DSTATE_BUF_INFO_CMD);
	OUT_RING(BUF_3D_ID_COLOR_BACK| BUF_3D_USE_FENCE |
			BUF_3D_PITCH(dst_pitch));
	OUT_RING(BUF_3D_ADDR(dst_offset));
	OUT_RING(MI_NOOP);

	OUT_RING(_3DSTATE_DST_BUF_VARS_CMD);
	OUT_RING(dst_format);

	/* defaults */
	OUT_RING(_3DSTATE_DFLT_Z_CMD);
	OUT_RING(0);

	OUT_RING(_3DSTATE_DFLT_DIFFUSE_CMD);
	OUT_RING(0);

	OUT_RING(_3DSTATE_DFLT_SPEC_CMD);
	OUT_RING(0);

	OUT_RING(_3DSTATE_DRAW_RECT_CMD);
	OUT_RING(0);
	OUT_RING(0); /* ymin, xmin */
	OUT_RING(DRAW_YMAX(pDst->drawable.height - 1) |
		DRAW_XMAX(pDst->drawable.width - 1));
	OUT_RING(0); /* yorig, xorig */

	OUT_RING(_3DSTATE_LOAD_STATE_IMMEDIATE_1 | I1_LOAD_S(2) | 
		I1_LOAD_S(3) | 1);
	if (pMask)
	    vf2 = 2 << 12; /* 2 texture coord sets */
	else
	    vf2 = 1 << 12;
	OUT_RING(vf2); /* TEXCOORDFMT_2D */
	OUT_RING(S3_CULLMODE_NONE | S3_VERTEXHAS_XY);

	/* We use two pipes for color and alpha, and do (src In mask)
	   in one stage. Arg1 is from src pict, and arg2 is from mask pict.
	   Be sure to force 1.0 when src or mask pict has no alpha channel.
	 */
	cblend = TB0C_LAST_STAGE | TB0C_RESULT_SCALE_1X | TB0C_OP_MODULE |
		 TB0C_OUTPUT_WRITE_CURRENT;
	ablend = TB0A_RESULT_SCALE_1X | TB0A_OP_MODULE |
		 TB0A_OUTPUT_WRITE_CURRENT;

	if (PICT_FORMAT_A(pSrcPicture->format) != 0) {
	    ablend |= TB0A_ARG1_SEL_TEXEL0;
	    cblend |= TB0C_ARG1_SEL_TEXEL0;
	} else {
	    ablend |= TB0A_ARG1_SEL_ONE;
	    if (pMask && pMaskPicture->componentAlpha 
		    && PICT_FORMAT_RGB(pMaskPicture->format)
		    && i830_blend_op[op].src_alpha)
		cblend |= TB0C_ARG1_SEL_ONE;
	    else
		cblend |= TB0C_ARG1_SEL_TEXEL0;
	}

	if (pMask) {
	    if (pMaskPicture->componentAlpha && 
		    PICT_FORMAT_RGB(pMaskPicture->format)) {
		if (i830_blend_op[op].src_alpha)
		    cblend |= (TB0C_ARG2_SEL_TEXEL1 | 
			    TB0C_ARG1_REPLICATE_ALPHA);
		else 
		    cblend |= TB0C_ARG2_SEL_TEXEL1;
	    } else {
		if (PICT_FORMAT_A(pMaskPicture->format) != 0)
		    cblend |= (TB0C_ARG2_SEL_TEXEL1 | 
			    TB0C_ARG2_REPLICATE_ALPHA);
		else
		    cblend |= TB0C_ARG2_SEL_ONE;
	    }
	    if (PICT_FORMAT_A(pMaskPicture->format) != 0)
		ablend |= TB0A_ARG2_SEL_TEXEL1;
	    else
		ablend |= TB0A_ARG2_SEL_ONE;
	} else {
	    cblend |= TB0C_ARG2_SEL_ONE;
	    ablend |= TB0A_ARG2_SEL_ONE;
	}

	OUT_RING(_3DSTATE_LOAD_STATE_IMMEDIATE_2 |
		 LOAD_TEXTURE_BLEND_STAGE(0)|1);
	OUT_RING(cblend);
	OUT_RING(ablend);
	OUT_RING(0);

	blendctl = i830_get_blend_cntl(op, pMaskPicture, pDstPicture->format);
	OUT_RING(_3DSTATE_INDPT_ALPHA_BLEND_CMD | DISABLE_INDPT_ALPHA_BLEND);
	OUT_RING(MI_NOOP);
	OUT_RING(_3DSTATE_LOAD_STATE_IMMEDIATE_1 | I1_LOAD_S(8) | 0);
	OUT_RING(S8_ENABLE_COLOR_BLEND | S8_BLENDFUNC_ADD | blendctl | 
		 S8_ENABLE_COLOR_BUFFER_WRITE);

	OUT_RING(_3DSTATE_ENABLES_1_CMD | DISABLE_LOGIC_OP | 
		DISABLE_STENCIL_TEST | DISABLE_DEPTH_BIAS | 
		DISABLE_SPEC_ADD | DISABLE_FOG | DISABLE_ALPHA_TEST | 
		ENABLE_COLOR_BLEND | DISABLE_DEPTH_TEST);
	/* We have to explicitly say we don't want write disabled */
	OUT_RING(_3DSTATE_ENABLES_2_CMD | ENABLE_COLOR_MASK |
		DISABLE_STENCIL_WRITE | ENABLE_TEX_CACHE |
		DISABLE_DITHER | ENABLE_COLOR_WRITE |
		DISABLE_DEPTH_WRITE);
	ADVANCE_LP_RING();
    }

#ifdef I830DEBUG
    Error("try to sync to show any errors...");
    I830Sync(pScrn);
#endif

    return TRUE;
}


/**
 * Do a single rectangle composite operation.
 *
 * This function is shared between i830 and i915 generation code.
 */
void
i830_composite(PixmapPtr pDst, int srcX, int srcY, int maskX, int maskY,
	       int dstX, int dstY, int w, int h)
{
    ScrnInfoPtr pScrn = xf86Screens[pDst->drawable.pScreen->myNum];
    I830Ptr pI830 = I830PTR(pScrn);
    Bool has_mask;
    float src_x[3], src_y[3], mask_x[3], mask_y[3];

    i830_get_transformed_coordinates(srcX, srcY,
				     pI830->transform[0],
				     &src_x[0], &src_y[0]);
    i830_get_transformed_coordinates(srcX, srcY + h,
				     pI830->transform[0],
				     &src_x[1], &src_y[1]);
    i830_get_transformed_coordinates(srcX + w, srcY + h,
				     pI830->transform[0],
				     &src_x[2], &src_y[2]);

    if (pI830->scale_units[1][0] == -1 || pI830->scale_units[1][1] == -1) {
	has_mask = FALSE;
    } else {
	has_mask = TRUE;
	i830_get_transformed_coordinates(maskX, maskY,
					 pI830->transform[1],
					 &mask_x[0], &mask_y[0]);
	i830_get_transformed_coordinates(maskX, maskY + h,
					 pI830->transform[1],
					 &mask_x[1], &mask_y[1]);
	i830_get_transformed_coordinates(maskX + w, maskY + h,
					 pI830->transform[1],
					 &mask_x[2], &mask_y[2]);
    }

    {
	int vertex_count;

	if (has_mask)
		vertex_count = 3*6;
	else
		vertex_count = 3*4;

	BEGIN_LP_RING(6+vertex_count);

	OUT_RING(MI_NOOP);
	OUT_RING(MI_NOOP);
	OUT_RING(MI_NOOP);
	OUT_RING(MI_NOOP);
	OUT_RING(MI_NOOP);

	OUT_RING(PRIM3D_INLINE | PRIM3D_RECTLIST | (vertex_count-1));

	OUT_RING_F(-0.125 + dstX + w);
	OUT_RING_F(-0.125 + dstY + h);
	OUT_RING_F(src_x[2] / pI830->scale_units[0][0]);
	OUT_RING_F(src_y[2] / pI830->scale_units[0][1]);
	if (has_mask) {
	    OUT_RING_F(mask_x[2] / pI830->scale_units[1][0]);
	    OUT_RING_F(mask_y[2] / pI830->scale_units[1][1]);
	}

	OUT_RING_F(-0.125 + dstX);
	OUT_RING_F(-0.125 + dstY + h);
	OUT_RING_F(src_x[1] / pI830->scale_units[0][0]);
	OUT_RING_F(src_y[1] / pI830->scale_units[0][1]);
	if (has_mask) {
	    OUT_RING_F(mask_x[1] / pI830->scale_units[1][0]);
	    OUT_RING_F(mask_y[1] / pI830->scale_units[1][1]);
	}

	OUT_RING_F(-0.125 + dstX);
	OUT_RING_F(-0.125 + dstY);
	OUT_RING_F(src_x[0] / pI830->scale_units[0][0]);
	OUT_RING_F(src_y[0] / pI830->scale_units[0][1]);
	if (has_mask) {
	    OUT_RING_F(mask_x[0] / pI830->scale_units[1][0]);
	    OUT_RING_F(mask_y[0] / pI830->scale_units[1][1]);
	}
	ADVANCE_LP_RING();
    }
}

/**
 * Do any cleanup from the Composite operation.
 *
 * This is shared between i830 through i965.
 */
void
i830_done_composite(PixmapPtr pDst)
{
    /* NO-OP */
}
