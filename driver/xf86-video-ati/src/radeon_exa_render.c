/*
 * Copyright 2005 Eric Anholt
 * Copyright 2005 Benjamin Herrenschmidt
 * All Rights Reserved.
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
 *    Eric Anholt <anholt@FreeBSD.org>
 *    Zack Rusin <zrusin@trolltech.com>
 *    Benjamin Herrenschmidt <benh@kernel.crashing.org>
 *
 */

#if defined(ACCEL_MMIO) && defined(ACCEL_CP)
#error Cannot define both MMIO and CP acceleration!
#endif

#if !defined(UNIXCPP) || defined(ANSICPP)
#define FUNC_NAME_CAT(prefix,suffix) prefix##suffix
#else
#define FUNC_NAME_CAT(prefix,suffix) prefix/**/suffix
#endif

#ifdef ACCEL_MMIO
#define FUNC_NAME(prefix) FUNC_NAME_CAT(prefix,MMIO)
#else
#ifdef ACCEL_CP
#define FUNC_NAME(prefix) FUNC_NAME_CAT(prefix,CP)
#else
#error No accel type defined!
#endif
#endif

#ifndef ACCEL_CP
#define ONLY_ONCE
#endif

/* Only include the following (generic) bits once. */
#ifdef ONLY_ONCE
static Bool is_transform[2];
static PictTransform *transform[2];

struct blendinfo {
    Bool dst_alpha;
    Bool src_alpha;
    CARD32 blend_cntl;
};

static struct blendinfo RadeonBlendOp[] = {
    /* Clear */
    {0, 0, RADEON_SRC_BLEND_GL_ZERO	      | RADEON_DST_BLEND_GL_ZERO},
    /* Src */
    {0, 0, RADEON_SRC_BLEND_GL_ONE	      | RADEON_DST_BLEND_GL_ZERO},
    /* Dst */
    {0, 0, RADEON_SRC_BLEND_GL_ZERO	      | RADEON_DST_BLEND_GL_ONE},
    /* Over */
    {0, 1, RADEON_SRC_BLEND_GL_ONE	      | RADEON_DST_BLEND_GL_ONE_MINUS_SRC_ALPHA},
    /* OverReverse */
    {1, 0, RADEON_SRC_BLEND_GL_ONE_MINUS_DST_ALPHA | RADEON_DST_BLEND_GL_ONE},
    /* In */
    {1, 0, RADEON_SRC_BLEND_GL_DST_ALPHA     | RADEON_DST_BLEND_GL_ZERO},
    /* InReverse */
    {0, 1, RADEON_SRC_BLEND_GL_ZERO	      | RADEON_DST_BLEND_GL_SRC_ALPHA},
    /* Out */
    {1, 0, RADEON_SRC_BLEND_GL_ONE_MINUS_DST_ALPHA | RADEON_DST_BLEND_GL_ZERO},
    /* OutReverse */
    {0, 1, RADEON_SRC_BLEND_GL_ZERO	      | RADEON_DST_BLEND_GL_ONE_MINUS_SRC_ALPHA},
    /* Atop */
    {1, 1, RADEON_SRC_BLEND_GL_DST_ALPHA     | RADEON_DST_BLEND_GL_ONE_MINUS_SRC_ALPHA},
    /* AtopReverse */
    {1, 1, RADEON_SRC_BLEND_GL_ONE_MINUS_DST_ALPHA | RADEON_DST_BLEND_GL_SRC_ALPHA},
    /* Xor */
    {1, 1, RADEON_SRC_BLEND_GL_ONE_MINUS_DST_ALPHA | RADEON_DST_BLEND_GL_ONE_MINUS_SRC_ALPHA},
    /* Add */
    {0, 0, RADEON_SRC_BLEND_GL_ONE	      | RADEON_DST_BLEND_GL_ONE},
};

struct formatinfo {
    int fmt;
    CARD32 card_fmt;
};

/* Note on texture formats:
 * TXFORMAT_Y8 expands to (Y,Y,Y,1).  TXFORMAT_I8 expands to (I,I,I,I)
 */
static struct formatinfo R100TexFormats[] = {
	{PICT_a8r8g8b8,	RADEON_TXFORMAT_ARGB8888 | RADEON_TXFORMAT_ALPHA_IN_MAP},
	{PICT_x8r8g8b8,	RADEON_TXFORMAT_ARGB8888},
	{PICT_r5g6b5,	RADEON_TXFORMAT_RGB565},
	{PICT_a1r5g5b5,	RADEON_TXFORMAT_ARGB1555 | RADEON_TXFORMAT_ALPHA_IN_MAP},
	{PICT_x1r5g5b5,	RADEON_TXFORMAT_ARGB1555},
	{PICT_a8,	RADEON_TXFORMAT_I8 | RADEON_TXFORMAT_ALPHA_IN_MAP},
};

static struct formatinfo R200TexFormats[] = {
    {PICT_a8r8g8b8,	R200_TXFORMAT_ARGB8888 | R200_TXFORMAT_ALPHA_IN_MAP},
    {PICT_x8r8g8b8,	R200_TXFORMAT_ARGB8888},
    {PICT_a8b8g8r8,	R200_TXFORMAT_ABGR8888 | R200_TXFORMAT_ALPHA_IN_MAP},
    {PICT_x8b8g8r8,	R200_TXFORMAT_ABGR8888},
    {PICT_r5g6b5,	R200_TXFORMAT_RGB565},
    {PICT_a1r5g5b5,	R200_TXFORMAT_ARGB1555 | R200_TXFORMAT_ALPHA_IN_MAP},
    {PICT_x1r5g5b5,	R200_TXFORMAT_ARGB1555},
    {PICT_a8,		R200_TXFORMAT_I8 | R200_TXFORMAT_ALPHA_IN_MAP},
};

/* Common Radeon setup code */

static Bool RADEONGetDestFormat(PicturePtr pDstPicture, CARD32 *dst_format)
{
    switch (pDstPicture->format) {
    case PICT_a8r8g8b8:
    case PICT_x8r8g8b8:
	*dst_format = RADEON_COLOR_FORMAT_ARGB8888;
	break;
    case PICT_r5g6b5:
	*dst_format = RADEON_COLOR_FORMAT_RGB565;
	break;
    case PICT_a1r5g5b5:
    case PICT_x1r5g5b5:
	*dst_format = RADEON_COLOR_FORMAT_ARGB1555;
	break;
    case PICT_a8:
	*dst_format = RADEON_COLOR_FORMAT_RGB8;
	break;
    default:
	RADEON_FALLBACK(("Unsupported dest format 0x%x\n",
			(int)pDstPicture->format));
    }

    return TRUE;
}

static CARD32 RADEONGetBlendCntl(int op, PicturePtr pMask, CARD32 dst_format)
{
    CARD32 sblend, dblend;

    sblend = RadeonBlendOp[op].blend_cntl & RADEON_SRC_BLEND_MASK;
    dblend = RadeonBlendOp[op].blend_cntl & RADEON_DST_BLEND_MASK;

    /* If there's no dst alpha channel, adjust the blend op so that we'll treat
     * it as always 1.
     */
    if (PICT_FORMAT_A(dst_format) == 0 && RadeonBlendOp[op].dst_alpha) {
	if (sblend == RADEON_SRC_BLEND_GL_DST_ALPHA)
	    sblend = RADEON_SRC_BLEND_GL_ONE;
	else if (sblend == RADEON_SRC_BLEND_GL_ONE_MINUS_DST_ALPHA)
	    sblend = RADEON_SRC_BLEND_GL_ZERO;
    }

    /* If the source alpha is being used, then we should only be in a case where
     * the source blend factor is 0, and the source blend value is the mask
     * channels multiplied by the source picture's alpha.
     */
    if (pMask && pMask->componentAlpha && RadeonBlendOp[op].src_alpha) {
	if (dblend == RADEON_DST_BLEND_GL_SRC_ALPHA) {
	    dblend = RADEON_DST_BLEND_GL_SRC_COLOR;
	} else if (dblend == RADEON_DST_BLEND_GL_ONE_MINUS_SRC_ALPHA) {
	    dblend = RADEON_DST_BLEND_GL_ONE_MINUS_SRC_COLOR;
	}
    }

    return sblend | dblend;
}

union intfloat {
    float f;
    CARD32 i;
};

/* R100-specific code */

static Bool R100CheckCompositeTexture(PicturePtr pPict, int unit)
{
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    int i;

    if ((w > 0x7ff) || (h > 0x7ff))
	RADEON_FALLBACK(("Picture w/h too large (%dx%d)\n", w, h));

    for (i = 0; i < sizeof(R100TexFormats) / sizeof(R100TexFormats[0]); i++) {
	if (R100TexFormats[i].fmt == pPict->format)
	    break;
    }
    if (i == sizeof(R100TexFormats) / sizeof(R100TexFormats[0]))
	RADEON_FALLBACK(("Unsupported picture format 0x%x\n",
			(int)pPict->format));

    if (pPict->repeat && ((w & (w - 1)) != 0 || (h & (h - 1)) != 0))
	RADEON_FALLBACK(("NPOT repeat unsupported (%dx%d)\n", w, h));

    if (pPict->filter != PictFilterNearest &&
	pPict->filter != PictFilterBilinear)
    {
	RADEON_FALLBACK(("Unsupported filter 0x%x\n", pPict->filter));
    }

    return TRUE;
}

#endif /* ONLY_ONCE */

static Bool FUNC_NAME(R100TextureSetup)(PicturePtr pPict, PixmapPtr pPix,
					int unit)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);
    CARD32 txfilter, txformat, txoffset, txpitch;
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    int i;
    ACCEL_PREAMBLE();

    txpitch = exaGetPixmapPitch(pPix);
    txoffset = exaGetPixmapOffset(pPix) + info->fbLocation;

    if ((txoffset & 0x1f) != 0)
	RADEON_FALLBACK(("Bad texture offset 0x%x\n", (int)txoffset));
    if ((txpitch & 0x1f) != 0)
	RADEON_FALLBACK(("Bad texture pitch 0x%x\n", (int)txpitch));
    
    for (i = 0; i < sizeof(R100TexFormats) / sizeof(R100TexFormats[0]); i++)
    {
	if (R100TexFormats[i].fmt == pPict->format)
	    break;
    }
    txformat = R100TexFormats[i].card_fmt;
    if (RADEONPixmapIsColortiled(pPix))
	txoffset |= RADEON_TXO_MACRO_TILE;

    if (pPict->repeat) {
	txformat |= RADEONLog2(w) << RADEON_TXFORMAT_WIDTH_SHIFT;
	txformat |= RADEONLog2(h) << RADEON_TXFORMAT_HEIGHT_SHIFT;
    } else 
	txformat |= RADEON_TXFORMAT_NON_POWER2;
    txformat |= unit << 24; /* RADEON_TXFORMAT_ST_ROUTE_STQX */
 
    switch (pPict->filter) {
    case PictFilterNearest:
	txfilter = (RADEON_MAG_FILTER_NEAREST | RADEON_MIN_FILTER_NEAREST);
	break;
    case PictFilterBilinear:
	txfilter = (RADEON_MAG_FILTER_LINEAR | RADEON_MIN_FILTER_LINEAR);
	break;
    default:
	RADEON_FALLBACK(("Bad filter 0x%x\n", pPict->filter));
    }

    BEGIN_ACCEL(5);
    if (unit == 0) {
	OUT_ACCEL_REG(RADEON_PP_TXFILTER_0, txfilter);
	OUT_ACCEL_REG(RADEON_PP_TXFORMAT_0, txformat);
	OUT_ACCEL_REG(RADEON_PP_TXOFFSET_0, txoffset);
	OUT_ACCEL_REG(RADEON_PP_TEX_SIZE_0,
	    (pPix->drawable.width - 1) |
	    ((pPix->drawable.height - 1) << RADEON_TEX_VSIZE_SHIFT));
	OUT_ACCEL_REG(RADEON_PP_TEX_PITCH_0, txpitch - 32);
    } else {
	OUT_ACCEL_REG(RADEON_PP_TXFILTER_1, txfilter);
	OUT_ACCEL_REG(RADEON_PP_TXFORMAT_1, txformat);
	OUT_ACCEL_REG(RADEON_PP_TXOFFSET_1, txoffset);
	OUT_ACCEL_REG(RADEON_PP_TEX_SIZE_1,
	    (pPix->drawable.width - 1) |
	    ((pPix->drawable.height - 1) << RADEON_TEX_VSIZE_SHIFT));
	OUT_ACCEL_REG(RADEON_PP_TEX_PITCH_1, txpitch - 32);
    }
    FINISH_ACCEL();

    if (pPict->transform != 0) {
	is_transform[unit] = TRUE;
	transform[unit] = pPict->transform;
    } else {
	is_transform[unit] = FALSE;
    }

    return TRUE;
}

#ifdef ONLY_ONCE
static Bool R100CheckComposite(int op, PicturePtr pSrcPicture,
			       PicturePtr pMaskPicture, PicturePtr pDstPicture)
{
    CARD32 tmp1;

    /* Check for unsupported compositing operations. */
    if (op >= sizeof(RadeonBlendOp) / sizeof(RadeonBlendOp[0]))
	RADEON_FALLBACK(("Unsupported Composite op 0x%x\n", op));

    if (pMaskPicture != NULL && pMaskPicture->componentAlpha) {
	/* Check if it's component alpha that relies on a source alpha and on
	 * the source value.  We can only get one of those into the single
	 * source value that we get to blend with.
	 */
	if (RadeonBlendOp[op].src_alpha &&
	    (RadeonBlendOp[op].blend_cntl & RADEON_SRC_BLEND_MASK) !=
	     RADEON_SRC_BLEND_GL_ZERO)
	{
	    RADEON_FALLBACK(("Component alpha not supported with source "
			    "alpha and source value blending.\n"));
	}
    }

    if (pDstPicture->pDrawable->width >= (1 << 11) ||
	pDstPicture->pDrawable->height >= (1 << 11))
    {
	RADEON_FALLBACK(("Dest w/h too large (%d,%d).\n",
			pDstPicture->pDrawable->width,
			pDstPicture->pDrawable->height));
    }

    if (!R100CheckCompositeTexture(pSrcPicture, 0))
	return FALSE;
    if (pMaskPicture != NULL && !R100CheckCompositeTexture(pMaskPicture, 1))
	return FALSE;

    if (!RADEONGetDestFormat(pDstPicture, &tmp1))
	return FALSE;

    return TRUE;
}
#endif /* ONLY_ONCE */

static Bool FUNC_NAME(R100PrepareComposite)(int op,
					    PicturePtr pSrcPicture,
					    PicturePtr pMaskPicture,
					    PicturePtr pDstPicture,
					    PixmapPtr pSrc,
					    PixmapPtr pMask,
					    PixmapPtr pDst)
{
    RINFO_FROM_SCREEN(pDst->drawable.pScreen);
    CARD32 dst_format, dst_offset, dst_pitch, colorpitch;
    CARD32 pp_cntl, blendcntl, cblend, ablend;
    int pixel_shift;
    ACCEL_PREAMBLE();

    TRACE;

    if (!info->XInited3D)
	RADEONInit3DEngine(pScrn);

    RADEONGetDestFormat(pDstPicture, &dst_format);
    pixel_shift = pDst->drawable.bitsPerPixel >> 4;

    dst_offset = exaGetPixmapOffset(pDst) + info->fbLocation;
    dst_pitch = exaGetPixmapPitch(pDst);
    colorpitch = dst_pitch >> pixel_shift;
    if (RADEONPixmapIsColortiled(pDst))
	colorpitch |= RADEON_COLOR_TILE_ENABLE;

    dst_offset = exaGetPixmapOffset(pDst) + info->fbLocation;
    dst_pitch = exaGetPixmapPitch(pDst);
    if ((dst_offset & 0x0f) != 0)
	RADEON_FALLBACK(("Bad destination offset 0x%x\n", (int)dst_offset));
    if (((dst_pitch >> pixel_shift) & 0x7) != 0)
	RADEON_FALLBACK(("Bad destination pitch 0x%x\n", (int)dst_pitch));

    if (!FUNC_NAME(R100TextureSetup)(pSrcPicture, pSrc, 0))
	return FALSE;
    pp_cntl = RADEON_TEX_0_ENABLE | RADEON_TEX_BLEND_0_ENABLE;

    if (pMask != NULL) {
	if (!FUNC_NAME(R100TextureSetup)(pMaskPicture, pMask, 1))
	    return FALSE;
	pp_cntl |= RADEON_TEX_1_ENABLE;
    } else {
	is_transform[1] = FALSE;
    }

    RADEON_SWITCH_TO_3D();

    BEGIN_ACCEL(8);
    OUT_ACCEL_REG(RADEON_PP_CNTL, pp_cntl);
    OUT_ACCEL_REG(RADEON_RB3D_CNTL, dst_format | RADEON_ALPHA_BLEND_ENABLE);
    OUT_ACCEL_REG(RADEON_RB3D_COLOROFFSET, dst_offset);
    OUT_ACCEL_REG(RADEON_RB3D_COLORPITCH, colorpitch);

    /* IN operator: Multiply src by mask components or mask alpha.
     * BLEND_CTL_ADD is A * B + C.
     * If a source is a8, we have to explicitly zero its color values.
     * If the destination is a8, we have to route the alpha to red, I think.
     * If we're doing component alpha where the source for blending is going to
     * be the source alpha (and there's no source value used), we have to zero
     * the source's color values.
     */
    cblend = RADEON_BLEND_CTL_ADD | RADEON_CLAMP_TX | RADEON_COLOR_ARG_C_ZERO;
    ablend = RADEON_BLEND_CTL_ADD | RADEON_CLAMP_TX | RADEON_ALPHA_ARG_C_ZERO;

    if (pDstPicture->format == PICT_a8 ||
	(pMask && pMaskPicture->componentAlpha && RadeonBlendOp[op].src_alpha))
    {
	cblend |= RADEON_COLOR_ARG_A_T0_ALPHA;
    } else if (pSrcPicture->format == PICT_a8)
	cblend |= RADEON_COLOR_ARG_A_ZERO;
    else
	cblend |= RADEON_COLOR_ARG_A_T0_COLOR;
    ablend |= RADEON_ALPHA_ARG_A_T0_ALPHA;

    if (pMask) {
	if (pMaskPicture->componentAlpha &&
	    pDstPicture->format != PICT_a8)
	    cblend |= RADEON_COLOR_ARG_B_T1_COLOR;
	else
	    cblend |= RADEON_COLOR_ARG_B_T1_ALPHA;
	ablend |= RADEON_ALPHA_ARG_B_T1_ALPHA;
    } else {
	cblend |= RADEON_COLOR_ARG_B_ZERO | RADEON_COMP_ARG_B;
	ablend |= RADEON_ALPHA_ARG_B_ZERO | RADEON_COMP_ARG_B;
    }

    OUT_ACCEL_REG(RADEON_PP_TXCBLEND_0, cblend);
    OUT_ACCEL_REG(RADEON_PP_TXABLEND_0, ablend);
    OUT_ACCEL_REG(RADEON_SE_VTX_FMT, RADEON_SE_VTX_FMT_XY |
				     RADEON_SE_VTX_FMT_ST0 |
				     RADEON_SE_VTX_FMT_ST1);
    /* Op operator. */
    blendcntl = RADEONGetBlendCntl(op, pMaskPicture, pDstPicture->format);

    OUT_ACCEL_REG(RADEON_RB3D_BLENDCNTL, blendcntl);
    FINISH_ACCEL();

    return TRUE;
}

#ifdef ONLY_ONCE

static Bool R200CheckCompositeTexture(PicturePtr pPict, int unit)
{
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    int i;

    if ((w > 0x7ff) || (h > 0x7ff))
	RADEON_FALLBACK(("Picture w/h too large (%dx%d)\n", w, h));

    for (i = 0; i < sizeof(R200TexFormats) / sizeof(R200TexFormats[0]); i++)
    {
	if (R200TexFormats[i].fmt == pPict->format)
	    break;
    }
    if (i == sizeof(R200TexFormats) / sizeof(R200TexFormats[0]))
	RADEON_FALLBACK(("Unsupported picture format 0x%x\n",
			 (int)pPict->format));

    if (pPict->repeat && ((w & (w - 1)) != 0 || (h & (h - 1)) != 0))
	RADEON_FALLBACK(("NPOT repeat unsupported (%dx%d)\n", w, h));

    if (pPict->filter != PictFilterNearest &&
	pPict->filter != PictFilterBilinear)
	RADEON_FALLBACK(("Unsupported filter 0x%x\n", pPict->filter));

    return TRUE;
}

#endif /* ONLY_ONCE */

static Bool FUNC_NAME(R200TextureSetup)(PicturePtr pPict, PixmapPtr pPix,
					int unit)
{
    RINFO_FROM_SCREEN(pPix->drawable.pScreen);
    CARD32 txfilter, txformat, txoffset, txpitch;
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    int i;
    ACCEL_PREAMBLE();

    txpitch = exaGetPixmapPitch(pPix);
    txoffset = exaGetPixmapOffset(pPix) + info->fbLocation;

    if ((txoffset & 0x1f) != 0)
	RADEON_FALLBACK(("Bad texture offset 0x%x\n", (int)txoffset));
    if ((txpitch & 0x1f) != 0)
	RADEON_FALLBACK(("Bad texture pitch 0x%x\n", (int)txpitch));
    
    for (i = 0; i < sizeof(R200TexFormats) / sizeof(R200TexFormats[0]); i++)
    {
	if (R200TexFormats[i].fmt == pPict->format)
	    break;
    }
    txformat = R200TexFormats[i].card_fmt;
    if (RADEONPixmapIsColortiled(pPix))
	txoffset |= R200_TXO_MACRO_TILE;

    if (pPict->repeat) {
	txformat |= RADEONLog2(w) << R200_TXFORMAT_WIDTH_SHIFT;
	txformat |= RADEONLog2(h) << R200_TXFORMAT_HEIGHT_SHIFT;
    } else
	txformat |= R200_TXFORMAT_NON_POWER2;
    txformat |= unit << R200_TXFORMAT_ST_ROUTE_SHIFT;

    switch (pPict->filter) {
    case PictFilterNearest:
	txfilter = (R200_MAG_FILTER_NEAREST |
		    R200_MIN_FILTER_NEAREST);
	break;
    case PictFilterBilinear:
	txfilter = (R200_MAG_FILTER_LINEAR |
		    R200_MIN_FILTER_LINEAR);
	break;
    default:
	RADEON_FALLBACK(("Bad filter 0x%x\n", pPict->filter));
    }

    BEGIN_ACCEL(6);
    if (unit == 0) {
	OUT_ACCEL_REG(R200_PP_TXFILTER_0, txfilter);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_0, txformat);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_X_0, 0);
	OUT_ACCEL_REG(R200_PP_TXSIZE_0, (pPix->drawable.width - 1) |
		      ((pPix->drawable.height - 1) << RADEON_TEX_VSIZE_SHIFT));
	OUT_ACCEL_REG(R200_PP_TXPITCH_0, txpitch - 32);
	OUT_ACCEL_REG(R200_PP_TXOFFSET_0, txoffset);
    } else {
	OUT_ACCEL_REG(R200_PP_TXFILTER_1, txfilter);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_1, txformat);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_X_1, 0);
	OUT_ACCEL_REG(R200_PP_TXSIZE_1, (pPix->drawable.width - 1) |
		      ((pPix->drawable.height - 1) << RADEON_TEX_VSIZE_SHIFT));
	OUT_ACCEL_REG(R200_PP_TXPITCH_1, txpitch - 32);
	OUT_ACCEL_REG(R200_PP_TXOFFSET_1, txoffset);
    }
    FINISH_ACCEL();

    if (pPict->transform != 0) {
	is_transform[unit] = TRUE;
	transform[unit] = pPict->transform;
    } else {
	is_transform[unit] = FALSE;
    }

    return TRUE;
}

#ifdef ONLY_ONCE
static Bool R200CheckComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
			       PicturePtr pDstPicture)
{
    CARD32 tmp1;

    TRACE;

    /* Check for unsupported compositing operations. */
    if (op >= sizeof(RadeonBlendOp) / sizeof(RadeonBlendOp[0]))
	RADEON_FALLBACK(("Unsupported Composite op 0x%x\n", op));

    if (pMaskPicture != NULL && pMaskPicture->componentAlpha) {
	/* Check if it's component alpha that relies on a source alpha and on
	 * the source value.  We can only get one of those into the single
	 * source value that we get to blend with.
	 */
	if (RadeonBlendOp[op].src_alpha &&
	    (RadeonBlendOp[op].blend_cntl & RADEON_SRC_BLEND_MASK) !=
	     RADEON_SRC_BLEND_GL_ZERO)
	{
	    RADEON_FALLBACK(("Component alpha not supported with source "
			    "alpha and source value blending.\n"));
	}
    }

    if (!R200CheckCompositeTexture(pSrcPicture, 0))
	return FALSE;
    if (pMaskPicture != NULL && !R200CheckCompositeTexture(pMaskPicture, 1))
	return FALSE;

    if (!RADEONGetDestFormat(pDstPicture, &tmp1))
	return FALSE;

    return TRUE;
}
#endif /* ONLY_ONCE */

static Bool FUNC_NAME(R200PrepareComposite)(int op, PicturePtr pSrcPicture,
				PicturePtr pMaskPicture, PicturePtr pDstPicture,
				PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
    RINFO_FROM_SCREEN(pDst->drawable.pScreen);
    CARD32 dst_format, dst_offset, dst_pitch;
    CARD32 pp_cntl, blendcntl, cblend, ablend, colorpitch;
    int pixel_shift;
    ACCEL_PREAMBLE();

    TRACE;

    if (!info->XInited3D)
	RADEONInit3DEngine(pScrn);

    RADEONGetDestFormat(pDstPicture, &dst_format);
    pixel_shift = pDst->drawable.bitsPerPixel >> 4;

    dst_offset = exaGetPixmapOffset(pDst) + info->fbLocation;
    dst_pitch = exaGetPixmapPitch(pDst);
    colorpitch = dst_pitch >> pixel_shift;
    if (RADEONPixmapIsColortiled(pDst))
	colorpitch |= RADEON_COLOR_TILE_ENABLE;

    if ((dst_offset & 0x0f) != 0)
	RADEON_FALLBACK(("Bad destination offset 0x%x\n", (int)dst_offset));
    if (((dst_pitch >> pixel_shift) & 0x7) != 0)
	RADEON_FALLBACK(("Bad destination pitch 0x%x\n", (int)dst_pitch));

    if (!FUNC_NAME(R200TextureSetup)(pSrcPicture, pSrc, 0))
	return FALSE;
    pp_cntl = RADEON_TEX_0_ENABLE | RADEON_TEX_BLEND_0_ENABLE;

    if (pMask != NULL) {
	if (!FUNC_NAME(R200TextureSetup)(pMaskPicture, pMask, 1))
	    return FALSE;
	pp_cntl |= RADEON_TEX_1_ENABLE;
    } else {
	is_transform[1] = FALSE;
    }

    RADEON_SWITCH_TO_3D();

    BEGIN_ACCEL(11);

    OUT_ACCEL_REG(RADEON_PP_CNTL, pp_cntl);
    OUT_ACCEL_REG(RADEON_RB3D_CNTL, dst_format | RADEON_ALPHA_BLEND_ENABLE);
    OUT_ACCEL_REG(RADEON_RB3D_COLOROFFSET, dst_offset);

    OUT_ACCEL_REG(R200_SE_VTX_FMT_0, R200_VTX_XY);
    OUT_ACCEL_REG(R200_SE_VTX_FMT_1,
		 (2 << R200_VTX_TEX0_COMP_CNT_SHIFT) |
		 (2 << R200_VTX_TEX1_COMP_CNT_SHIFT));

    OUT_ACCEL_REG(RADEON_RB3D_COLORPITCH, colorpitch);

    /* IN operator: Multiply src by mask components or mask alpha.
     * BLEND_CTL_ADD is A * B + C.
     * If a picture is a8, we have to explicitly zero its color values.
     * If the destination is a8, we have to route the alpha to red, I think.
     * If we're doing component alpha where the source for blending is going to
     * be the source alpha (and there's no source value used), we have to zero
     * the source's color values.
     */
    cblend = R200_TXC_OP_MADD | R200_TXC_ARG_C_ZERO;
    ablend = R200_TXA_OP_MADD | R200_TXA_ARG_C_ZERO;

    if (pDstPicture->format == PICT_a8 ||
	(pMask && pMaskPicture->componentAlpha && RadeonBlendOp[op].src_alpha))
    {
	cblend |= R200_TXC_ARG_A_R0_ALPHA;
    } else if (pSrcPicture->format == PICT_a8)
	cblend |= R200_TXC_ARG_A_ZERO;
    else
	cblend |= R200_TXC_ARG_A_R0_COLOR;
    ablend |= R200_TXA_ARG_A_R0_ALPHA;

    if (pMask) {
	if (pMaskPicture->componentAlpha &&
	    pDstPicture->format != PICT_a8)
	    cblend |= R200_TXC_ARG_B_R1_COLOR;
	else
	    cblend |= R200_TXC_ARG_B_R1_ALPHA;
	ablend |= R200_TXA_ARG_B_R1_ALPHA;
    } else {
	cblend |= R200_TXC_ARG_B_ZERO | R200_TXC_COMP_ARG_B;
	ablend |= R200_TXA_ARG_B_ZERO | R200_TXA_COMP_ARG_B;
    }

    OUT_ACCEL_REG(R200_PP_TXCBLEND_0, cblend);
    OUT_ACCEL_REG(R200_PP_TXCBLEND2_0,
	R200_TXC_CLAMP_0_1 | R200_TXC_OUTPUT_REG_R0);
    OUT_ACCEL_REG(R200_PP_TXABLEND_0, ablend);
    OUT_ACCEL_REG(R200_PP_TXABLEND2_0,
	R200_TXA_CLAMP_0_1 | R200_TXA_OUTPUT_REG_R0);

    /* Op operator. */
    blendcntl = RADEONGetBlendCntl(op, pMaskPicture, pDstPicture->format);
    OUT_ACCEL_REG(RADEON_RB3D_BLENDCNTL, blendcntl);
    FINISH_ACCEL();

    return TRUE;
}

#ifdef ACCEL_CP

#define VTX_DWORD_COUNT 6

#define VTX_OUT(_dstX, _dstY, _srcX, _srcY, _maskX, _maskY)	\
do {								\
    OUT_RING_F(_dstX);						\
    OUT_RING_F(_dstY);						\
    OUT_RING_F(_srcX);						\
    OUT_RING_F(_srcY);						\
    OUT_RING_F(_maskX);						\
    OUT_RING_F(_maskY);						\
} while (0)

#else /* ACCEL_CP */

#define VTX_REG_COUNT 6

#define VTX_OUT(_dstX, _dstY, _srcX, _srcY, _maskX, _maskY)	\
do {								\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _dstX);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _dstY);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _srcX);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _srcY);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _maskX);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _maskY);		\
} while (0)

#endif /* !ACCEL_CP */

static void FUNC_NAME(RadeonComposite)(PixmapPtr pDst,
				     int srcX, int srcY,
				     int maskX, int maskY,
				     int dstX, int dstY,
				     int w, int h)
{
    RINFO_FROM_SCREEN(pDst->drawable.pScreen);
    int srcXend, srcYend, maskXend, maskYend;
    PictVector v;
    ACCEL_PREAMBLE();

    ENTER_DRAW(0);

    /*ErrorF("RadeonComposite (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",
	   srcX, srcY, maskX, maskY,dstX, dstY, w, h);*/

    srcXend = srcX + w;
    srcYend = srcY + h;
    maskXend = maskX + w;
    maskYend = maskY + h;
    if (is_transform[0]) {
	v.vector[0] = IntToxFixed(srcX);
	v.vector[1] = IntToxFixed(srcY);
	v.vector[2] = xFixed1;
	PictureTransformPoint(transform[0], &v);
	srcX = xFixedToInt(v.vector[0]);
	srcY = xFixedToInt(v.vector[1]);
	v.vector[0] = IntToxFixed(srcXend);
	v.vector[1] = IntToxFixed(srcYend);
	v.vector[2] = xFixed1;
	PictureTransformPoint(transform[0], &v);
	srcXend = xFixedToInt(v.vector[0]);
	srcYend = xFixedToInt(v.vector[1]);
    }
    if (is_transform[1]) {
	v.vector[0] = IntToxFixed(maskX);
	v.vector[1] = IntToxFixed(maskY);
	v.vector[2] = xFixed1;
	PictureTransformPoint(transform[1], &v);
	maskX = xFixedToInt(v.vector[0]);
	maskY = xFixedToInt(v.vector[1]);
	v.vector[0] = IntToxFixed(maskXend);
	v.vector[1] = IntToxFixed(maskYend);
	v.vector[2] = xFixed1;
	PictureTransformPoint(transform[1], &v);
	maskXend = xFixedToInt(v.vector[0]);
	maskYend = xFixedToInt(v.vector[1]);
    }

#ifdef ACCEL_CP
    if (info->ChipFamily < CHIP_FAMILY_R200) {
	BEGIN_RING(4 * VTX_DWORD_COUNT + 3);
	OUT_RING(CP_PACKET3(RADEON_CP_PACKET3_3D_DRAW_IMMD,
			    4 * VTX_DWORD_COUNT + 1));
	OUT_RING(RADEON_CP_VC_FRMT_XY |
		 RADEON_CP_VC_FRMT_ST0 |
		 RADEON_CP_VC_FRMT_ST1);
	OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_TRI_FAN |
		 RADEON_CP_VC_CNTL_PRIM_WALK_RING |
		 RADEON_CP_VC_CNTL_MAOS_ENABLE |
		 RADEON_CP_VC_CNTL_VTX_FMT_RADEON_MODE |
		 (4 << RADEON_CP_VC_CNTL_NUM_SHIFT));
    } else {
	BEGIN_RING(4 * VTX_DWORD_COUNT + 2);
	OUT_RING(CP_PACKET3(R200_CP_PACKET3_3D_DRAW_IMMD_2,
			    4 * VTX_DWORD_COUNT));
	OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_TRI_FAN |
		 RADEON_CP_VC_CNTL_PRIM_WALK_RING |
		 (4 << RADEON_CP_VC_CNTL_NUM_SHIFT));
    }

    VTX_OUT(dstX,     dstY,     srcX,    srcY,    maskX,    maskY);
    VTX_OUT(dstX,     dstY + h, srcX,    srcYend, maskX,    maskYend);
    VTX_OUT(dstX + w, dstY + h, srcXend, srcYend, maskXend, maskYend);
    VTX_OUT(dstX + w, dstY,     srcXend, srcY,    maskXend, maskY);
    ADVANCE_RING();
#else /* ACCEL_CP */
    BEGIN_ACCEL(1 + VTX_REG_COUNT * 4);
    if (info->ChipFamily < CHIP_FAMILY_R200) {
	OUT_ACCEL_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_TRIANGLE_FAN |
					  RADEON_VF_PRIM_WALK_DATA |
					  RADEON_VF_RADEON_MODE |
					  4 << RADEON_VF_NUM_VERTICES_SHIFT));
    } else {
	OUT_ACCEL_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_QUAD_LIST |
					  RADEON_VF_PRIM_WALK_DATA |
					  4 << RADEON_VF_NUM_VERTICES_SHIFT));
    }

    VTX_OUT(dstX,     dstY,     srcX,    srcY,    maskX,    maskY);
    VTX_OUT(dstX,     dstY + h, srcX,    srcYend, maskX,    maskYend);
    VTX_OUT(dstX + w, dstY + h, srcXend, srcYend, maskXend, maskYend);
    VTX_OUT(dstX + w, dstY,     srcXend, srcY,    maskXend, maskY);
    FINISH_ACCEL();
#endif /* !ACCEL_CP */

    LEAVE_DRAW(0);
}
#undef VTX_OUT

#ifdef ONLY_ONCE
static void RadeonDoneComposite(PixmapPtr pDst)
{
    ENTER_DRAW(0);
    LEAVE_DRAW(0);
}
#endif /* ONLY_ONCE */

#undef ONLY_ONCE
