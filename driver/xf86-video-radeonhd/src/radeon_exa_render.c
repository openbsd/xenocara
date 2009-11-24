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
 *    Alex Deucher <alexander.deucher@amd.com>
 *
 */
#if defined(IS_RADEON_DRIVER) || defined(IS_QUICK_AND_DIRTY)
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

#define VAR_PREAMBLE(pScreen) \
        ScrnInfoPtr pScrn = xf86Screens[(pScreen)->myNum]; \
        RHDPtr info = RHDPTR(pScrn)
#define THREEDSTATE_PREAMBLE() struct rhdAccel *accel_state = info->accel_state
#define HAS_TCL IS_R500_3D
#define FB_OFFSET (info->FbIntAddress + info->FbScanoutStart)

#endif

#else /* IS_RADEON_DRIVER */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include "rhd.h"
#include "rhd_cs.h"

#include "r5xx_regs.h"
#include "r5xx_accel.h"
#include "r5xx_3dregs.h"

#include "exa.h"

#define VAR_PREAMBLE(pScreen) RHDPtr rhdPtr = RHDPTR(xf86Screens[(pScreen)->myNum])
#define THREEDSTATE_PREAMBLE() struct R5xx3D *accel_state = rhdPtr->ThreeDPrivate
/*
 * Map the macros.
 */
#define ACCEL_PREAMBLE() struct RhdCS *CS = rhdPtr->CS

#define BEGIN_ACCEL(Count) RHDCSGrab(CS, 2 * (Count))
#define OUT_ACCEL_REG(Reg, Value) RHDCSRegWrite(CS, (Reg), (Value))
#define FINISH_ACCEL()

#define BEGIN_RING(Count) RHDCSGrab(CS, (Count))
#define OUT_RING(Value) RHDCSWrite(CS, (Value))
#define OUT_RING_F(x) OUT_RING(F_TO_DW(x))
#define ADVANCE_RING() RHDCSAdvance(CS)

#define uint32_t CARD32

#define IS_R300_3D \
    ((rhdPtr->ChipSet == RHD_RS690) || \
     (rhdPtr->ChipSet == RHD_RS600) || \
     (rhdPtr->ChipSet == RHD_RS740))

#define IS_R500_3D \
    ((rhdPtr->ChipSet != RHD_RS690) && \
     (rhdPtr->ChipSet != RHD_RS600) && \
     (rhdPtr->ChipSet != RHD_RS740))

#define HAS_TCL IS_R500_3D

#define ONLY_ONCE 1 /* we're always only once in the radeonhd driver */
#define ACCEL_CP 1

/* RADEON_FALLBACK is not only error messages, but some things are meant to
 * happen, which can make it unbelievably noisy. */
#if 0
/* i don't like ErrorF's as they don't tell you which driver and
   which device, but no scrnIndex can be had in some of the calls here */
#define RADEON_FALLBACK(x)     		\
do {					\
	ErrorF("%s: ", __FUNCTION__);	\
	ErrorF x;			\
	return FALSE;			\
} while (0)
#else
#define RADEON_FALLBACK(x) return FALSE
#endif

#if !defined(UNIXCPP) || defined(ANSICPP)
#define FUNC_NAME_CAT(prefix,suffix) prefix##suffix
#else
#define FUNC_NAME_CAT(prefix,suffix) prefix/**/suffix
#endif

#define FUNC_NAME(prefix) FUNC_NAME_CAT(RHD,prefix)

#define TRACE
#define ENTER_DRAW(x)
#define LEAVE_DRAW(x)

#define RADEONPixmapIsColortiled(x) FALSE

#define FB_OFFSET (rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart)

static __inline__ uint32_t
F_TO_DW(float val)
{
    union {
	float f;
	uint32_t l;
    } tmp;
    tmp.f = val;
    return tmp.l;
}

#define xFixedToFloat(f) (((float) (f)) / 65536)

#define RADEON_SWITCH_TO_3D() R5xxEngineWaitIdle2D(rhdPtr->CS)
#define RADEONInit3DEngine(x) R5xx3DSetup(rhdPtr->scrnIndex)

#endif /* IS_RADEON_DRIVER */

/* Only include the following (generic) bits once. */
#ifdef ONLY_ONCE

struct blendinfo {
    Bool dst_alpha;
    Bool src_alpha;
    uint32_t blend_cntl;
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
    unsigned int fmt;
    uint32_t card_fmt;
};

/* Note on texture formats:
 * TXFORMAT_Y8 expands to (Y,Y,Y,1).  TXFORMAT_I8 expands to (I,I,I,I)
 */
#ifdef IS_RADEON_DRIVER
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
#endif /* IS_RADEON_DRIVER */

static struct formatinfo R300TexFormats[] = {
    {PICT_a8r8g8b8,	R300_EASY_TX_FORMAT(X, Y, Z, W, W8Z8Y8X8)},
    {PICT_x8r8g8b8,	R300_EASY_TX_FORMAT(X, Y, Z, ONE, W8Z8Y8X8)},
    {PICT_a8b8g8r8,	R300_EASY_TX_FORMAT(Z, Y, X, W, W8Z8Y8X8)},
    {PICT_x8b8g8r8,	R300_EASY_TX_FORMAT(Z, Y, X, ONE, W8Z8Y8X8)},
    {PICT_r5g6b5,	R300_EASY_TX_FORMAT(X, Y, Z, ONE, Z5Y6X5)},
    {PICT_a1r5g5b5,	R300_EASY_TX_FORMAT(X, Y, Z, W, W1Z5Y5X5)},
    {PICT_x1r5g5b5,	R300_EASY_TX_FORMAT(X, Y, Z, ONE, W1Z5Y5X5)},
    {PICT_a8,		R300_EASY_TX_FORMAT(ZERO, ZERO, ZERO, X, X8)},
};

/* Common Radeon setup code */
#ifdef IS_RADEON_DRIVER
static Bool RADEONGetDestFormat(PicturePtr pDstPicture, uint32_t *dst_format)
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
#endif /* IS_RADEON_DRIVER */

static Bool R300GetDestFormat(PicturePtr pDstPicture, uint32_t *dst_format)
{
    switch (pDstPicture->format) {
    case PICT_a8r8g8b8:
    case PICT_x8r8g8b8:
	*dst_format = R300_COLORFORMAT_ARGB8888;
	break;
    case PICT_r5g6b5:
	*dst_format = R300_COLORFORMAT_RGB565;
	break;
    case PICT_a1r5g5b5:
    case PICT_x1r5g5b5:
	*dst_format = R300_COLORFORMAT_ARGB1555;
	break;
    case PICT_a8:
	*dst_format = R300_COLORFORMAT_I8;
	break;
    default:
	RADEON_FALLBACK(("Unsupported dest format 0x%x\n",
	       (int)pDstPicture->format));
    }
    return TRUE;
}

static uint32_t RADEONGetBlendCntl(int op, PicturePtr pMask, uint32_t dst_format)
{
    uint32_t sblend, dblend;

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
    uint32_t i;
};

/* Check if we need a software-fallback because of a repeating
 *   non-power-of-two texture.
 *
 * canTile: whether we can emulate a repeat by drawing in tiles:
 *   possible for the source, but not for the mask. (Actually
 *   we could do tiling for the mask too, but dealing with the
 *   combination of a tiled mask and a tiled source would be
 *   a lot of complexity, so we handle only the most common
 *   case of a repeating mask.)
 */
static Bool RADEONCheckTexturePOT(PicturePtr pPict, Bool canTile)
{
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;

    if (pPict->repeat && ((w & (w - 1)) != 0 || (h & (h - 1)) != 0) &&
	!(!pPict->transform && canTile))
	RADEON_FALLBACK(("NPOT repeating %s unsupported (%dx%d), transform=%d\n",
			 canTile ? "source" : "mask", w, h, pPict->transform != 0));

    return TRUE;
}

/* Determine if the pitch of the pixmap meets the criteria for being
 * used as a repeating texture: no padding or only a single line texture.
 */
static Bool RADEONPitchMatches(PixmapPtr pPix)
{
    int w = pPix->drawable.width;
    int h = pPix->drawable.height;
    uint32_t txpitch = exaGetPixmapPitch(pPix);

    if (h > 1 && (unsigned int)((w * pPix->drawable.bitsPerPixel / 8 + 31) & ~31) != txpitch)
	return FALSE;

    return TRUE;
}

/* We can't turn on repeats normally for a non-power-of-two dimension,
 * but if the source isn't transformed, we can get the same effect
 * by drawing the image in multiple tiles. (A common case that it's
 * important to get right is drawing a strip of a NPOTxPOT texture
 * repeating in the POT direction. With tiling, this ends up as a
 * a single tile on R300 and newer, which is perfect.)
 *
 * canTile1d: On R300 and newer, we can repeat a texture that is NPOT in
 *   one direction and POT in the other in the POT direction; on
 *   older chips we can only repeat at all if the texture is POT in
 *   both directions.
 *
 * needMatchingPitch: On R100/R200, we can only repeat horizontally if
 *   there is no padding in the texture. Textures with small POT widths
 *   (1,2,4,8) thus can't be tiled.
 */
static Bool RADEONSetupSourceTile(PicturePtr pPict,
				  PixmapPtr pPix,
				  Bool canTile1d,
				  Bool needMatchingPitch)
{
    VAR_PREAMBLE(pPix->drawable.pScreen);
    THREEDSTATE_PREAMBLE();

    accel_state->need_src_tile_x = accel_state->need_src_tile_y = FALSE;
    accel_state->src_tile_width = accel_state->src_tile_height = 65536; /* "infinite" */

    if (pPict->repeat) {
	Bool badPitch = needMatchingPitch && !RADEONPitchMatches(pPix);

	int w = pPict->pDrawable->width;
	int h = pPict->pDrawable->height;

	if (pPict->transform) {
	    if (badPitch)
		RADEON_FALLBACK(("Width %d and pitch %u not compatible for repeat\n",
				 w, (unsigned)exaGetPixmapPitch(pPix)));
	} else {
	    accel_state->need_src_tile_x = (w & (w - 1)) != 0 || badPitch;
	    accel_state->need_src_tile_y = (h & (h - 1)) != 0;

	    if (!canTile1d)
		accel_state->need_src_tile_x = accel_state->need_src_tile_y
		    = accel_state->need_src_tile_x || accel_state->need_src_tile_y;
	}

	if (accel_state->need_src_tile_x)
	    accel_state->src_tile_width = w;
	if (accel_state->need_src_tile_y)
	  accel_state->src_tile_height = h;
    }

    return TRUE;
}

PixmapPtr
RADEONGetDrawablePixmap(DrawablePtr pDrawable)
{
    if (pDrawable->type == DRAWABLE_WINDOW)
	return pDrawable->pScreen->GetWindowPixmap((WindowPtr)pDrawable);
    else
	return (PixmapPtr)pDrawable;
}

#endif /* ONLY_ONCE */

#ifdef IS_RADEON_DRIVER

/* R100-specific code */
# ifdef ONLY_ONCE
static Bool R100CheckCompositeTexture(PicturePtr pPict, int unit)
{
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    unsigned int i;

    if ((w > 0x7ff) || (h > 0x7ff))
	RADEON_FALLBACK(("Picture w/h too large (%dx%d)\n", w, h));

    for (i = 0; i < sizeof(R100TexFormats) / sizeof(R100TexFormats[0]); i++) {
	if (R100TexFormats[i].fmt == pPict->format)
	    break;
    }
    if (i == sizeof(R100TexFormats) / sizeof(R100TexFormats[0]))
	RADEON_FALLBACK(("Unsupported picture format 0x%x\n",
			(int)pPict->format));

    if (!RADEONCheckTexturePOT(pPict, unit == 0))
	return FALSE;

    if (pPict->filter != PictFilterNearest &&
	pPict->filter != PictFilterBilinear)
    {
	RADEON_FALLBACK(("Unsupported filter 0x%x\n", pPict->filter));
    }

    if (pPict->repeat && pPict->repeatType != RepeatNormal)
    {
	RADEON_FALLBACK(("Unsupported repeat type %d\n", pPict->repeat));
    }

    return TRUE;
}
# endif ONLY_ONCE

static Bool FUNC_NAME(R100TextureSetup)(PicturePtr pPict, PixmapPtr pPix,
					int unit)
{
    VAR_PREAMBLE(pPix->drawable.pScreen);
    THREEDSTATE_PREAMBLE();
    uint32_t txfilter, txformat, txoffset, txpitch;
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    Bool repeat = pPict->repeat && !(unit == 0 && (accel_state->need_src_tile_x || accel_state->need_src_tile_y));
    unsigned int i;
    ACCEL_PREAMBLE();

    txpitch = exaGetPixmapPitch(pPix);
    txoffset = exaGetPixmapOffset(pPix) + FB_OFFSET;

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

    if (repeat) {
	if (!RADEONPitchMatches(pPix))
	    RADEON_FALLBACK(("Width %d and pitch %u not compatible for repeat\n",
			     w, (unsigned)txpitch));

	txformat |= RADEONLog2(w) << RADEON_TXFORMAT_WIDTH_SHIFT;
	txformat |= RADEONLog2(h) << RADEON_TXFORMAT_HEIGHT_SHIFT;
    } else
	txformat |= RADEON_TXFORMAT_NON_POWER2;
    txformat |= unit << 24; /* RADEON_TXFORMAT_ST_ROUTE_STQX */

    accel_state->texW[unit] = 1;
    accel_state->texH[unit] = 1;

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

    if (repeat)
      txfilter |= RADEON_CLAMP_S_WRAP | RADEON_CLAMP_T_WRAP;

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
	accel_state->is_transform[unit] = TRUE;
	accel_state->transform[unit] = pPict->transform;
    } else {
	accel_state->is_transform[unit] = FALSE;
    }

    return TRUE;
}

# ifdef ONLY_ONCE

static Bool R100CheckComposite(int op, PicturePtr pSrcPicture,
			       PicturePtr pMaskPicture, PicturePtr pDstPicture)
{
    PixmapPtr pSrcPixmap, pDstPixmap;
    uint32_t tmp1;

    /* Check for unsupported compositing operations. */
    if ((unsigned int)op >= sizeof(RadeonBlendOp) / sizeof(RadeonBlendOp[0]))
	RADEON_FALLBACK(("Unsupported Composite op 0x%x\n", op));

    if (!pSrcPicture->pDrawable)
	return FALSE;

    pSrcPixmap = RADEONGetDrawablePixmap(pSrcPicture->pDrawable);

    if (pSrcPixmap->drawable.width >= 2048 ||
	pSrcPixmap->drawable.height >= 2048) {
	RADEON_FALLBACK(("Source w/h too large (%d,%d).\n",
			 pSrcPixmap->drawable.width,
			 pSrcPixmap->drawable.height));
    }

    pDstPixmap = RADEONGetDrawablePixmap(pDstPicture->pDrawable);

    if (pDstPixmap->drawable.width >= 2048 ||
	pDstPixmap->drawable.height >= 2048) {
	RADEON_FALLBACK(("Dest w/h too large (%d,%d).\n",
			 pDstPixmap->drawable.width,
			 pDstPixmap->drawable.height));
    }

    if (pMaskPicture) {
	PixmapPtr pMaskPixmap = RADEONGetDrawablePixmap(pMaskPicture->pDrawable);

	if (pMaskPixmap->drawable.width >= 2048 ||
	    pMaskPixmap->drawable.height >= 2048) {
	    RADEON_FALLBACK(("Mask w/h too large (%d,%d).\n",
			     pMaskPixmap->drawable.width,
			     pMaskPixmap->drawable.height));
	}

	if (pMaskPicture->componentAlpha) {
	    /* Check if it's component alpha that relies on a source alpha and
	     * on the source value.  We can only get one of those into the
	     * single source value that we get to blend with.
	     */
	    if (RadeonBlendOp[op].src_alpha &&
		(RadeonBlendOp[op].blend_cntl & RADEON_SRC_BLEND_MASK) !=
		RADEON_SRC_BLEND_GL_ZERO) {
		RADEON_FALLBACK(("Component alpha not supported with source "
				 "alpha and source value blending.\n"));
	    }
	}

	if (!R100CheckCompositeTexture(pMaskPicture, 1))
	    return FALSE;
    }

    if (!R100CheckCompositeTexture(pSrcPicture, 0))
	return FALSE;

    if (!RADEONGetDestFormat(pDstPicture, &tmp1))
	return FALSE;

    return TRUE;
}
# endif /* ONLY_ONCE */

static Bool FUNC_NAME(R100PrepareComposite)(int op,
					    PicturePtr pSrcPicture,
					    PicturePtr pMaskPicture,
					    PicturePtr pDstPicture,
					    PixmapPtr pSrc,
					    PixmapPtr pMask,
					    PixmapPtr pDst)
{
    VAR_PREAMBLE(pDst->drawable.pScreen);
    THREEDSTATE_PREAMBLE();
    uint32_t dst_format, dst_offset, dst_pitch, colorpitch;
    uint32_t pp_cntl, blendcntl, cblend, ablend;
    int pixel_shift;
    ACCEL_PREAMBLE();

    TRACE;

    if (!accel_state->XHas3DEngineState)
	RADEONInit3DEngine(pScrn);

    if (!RADEONGetDestFormat(pDstPicture, &dst_format))
	return FALSE;

    if (pMask)
	accel_state->has_mask = TRUE;
    else
	accel_state->has_mask = FALSE;

    pixel_shift = pDst->drawable.bitsPerPixel >> 4;

    dst_offset = exaGetPixmapOffset(pDst) + FB_OFFSET;
    dst_pitch = exaGetPixmapPitch(pDst);
    colorpitch = dst_pitch >> pixel_shift;
    if (RADEONPixmapIsColortiled(pDst))
	colorpitch |= RADEON_COLOR_TILE_ENABLE;

    dst_offset = exaGetPixmapOffset(pDst) + FB_OFFSET;
    dst_pitch = exaGetPixmapPitch(pDst);
    if ((dst_offset & 0x0f) != 0)
	RADEON_FALLBACK(("Bad destination offset 0x%x\n", (int)dst_offset));
    if (((dst_pitch >> pixel_shift) & 0x7) != 0)
	RADEON_FALLBACK(("Bad destination pitch 0x%x\n", (int)dst_pitch));

    if (!RADEONSetupSourceTile(pSrcPicture, pSrc, FALSE, TRUE))
	return FALSE;

    if (!FUNC_NAME(R100TextureSetup)(pSrcPicture, pSrc, 0))
	return FALSE;
    pp_cntl = RADEON_TEX_0_ENABLE | RADEON_TEX_BLEND_0_ENABLE;

    if (pMask != NULL) {
	if (!FUNC_NAME(R100TextureSetup)(pMaskPicture, pMask, 1))
	    return FALSE;
	pp_cntl |= RADEON_TEX_1_ENABLE;
    } else {
	accel_state->is_transform[1] = FALSE;
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
    if (pMask)
	OUT_ACCEL_REG(RADEON_SE_VTX_FMT, (RADEON_SE_VTX_FMT_XY |
					  RADEON_SE_VTX_FMT_ST0 |
					  RADEON_SE_VTX_FMT_ST1));
    else
	OUT_ACCEL_REG(RADEON_SE_VTX_FMT, (RADEON_SE_VTX_FMT_XY |
					  RADEON_SE_VTX_FMT_ST0));
    /* Op operator. */
    blendcntl = RADEONGetBlendCntl(op, pMaskPicture, pDstPicture->format);

    OUT_ACCEL_REG(RADEON_RB3D_BLENDCNTL, blendcntl);
    FINISH_ACCEL();

    return TRUE;
}

# ifdef ONLY_ONCE
static Bool R200CheckCompositeTexture(PicturePtr pPict, int unit)
{
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    unsigned int i;

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

    if (!RADEONCheckTexturePOT(pPict, unit == 0))
	return FALSE;

    if (pPict->filter != PictFilterNearest &&
	pPict->filter != PictFilterBilinear)
	RADEON_FALLBACK(("Unsupported filter 0x%x\n", pPict->filter));

    if (pPict->repeat && pPict->repeatType != RepeatNormal)
    {
	RADEON_FALLBACK(("Unsupported repeat type %d\n", pPict->repeat));
    }

    return TRUE;
}
# endif /* ONLY_ONCE */

static Bool FUNC_NAME(R200TextureSetup)(PicturePtr pPict, PixmapPtr pPix,
					int unit)
{
    VAR_PREAMBLE(pPix->drawable.pScreen);
    THREEDSTATE_PREAMBLE();
    uint32_t txfilter, txformat, txoffset, txpitch;
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    Bool repeat = pPict->repeat && !(unit == 0 && (accel_state->need_src_tile_x || accel_state->need_src_tile_y));
    unsigned int i;
    ACCEL_PREAMBLE();

    txpitch = exaGetPixmapPitch(pPix);
    txoffset = exaGetPixmapOffset(pPix) + FB_OFFSET;

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

    if (repeat) {
	if (!RADEONPitchMatches(pPix))
	    RADEON_FALLBACK(("Width %d and pitch %u not compatible for repeat\n",
			     w, (unsigned)txpitch));

	txformat |= RADEONLog2(w) << R200_TXFORMAT_WIDTH_SHIFT;
	txformat |= RADEONLog2(h) << R200_TXFORMAT_HEIGHT_SHIFT;
    } else
	txformat |= R200_TXFORMAT_NON_POWER2;
    txformat |= unit << R200_TXFORMAT_ST_ROUTE_SHIFT;

    accel_state->texW[unit] = w;
    accel_state->texH[unit] = h;

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

    if (repeat)
      txfilter |= R200_CLAMP_S_WRAP | R200_CLAMP_T_WRAP;

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
	accel_state->is_transform[unit] = TRUE;
	accel_state->transform[unit] = pPict->transform;
    } else {
	accel_state->is_transform[unit] = FALSE;
    }

    return TRUE;
}

# ifdef ONLY_ONCE
static Bool R200CheckComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
			       PicturePtr pDstPicture)
{
    PixmapPtr pSrcPixmap, pDstPixmap;
    uint32_t tmp1;

    TRACE;

    if (!pSrcPicture->pDrawable)
	return FALSE;

    pSrcPixmap = RADEONGetDrawablePixmap(pSrcPicture->pDrawable);

    if (pSrcPixmap->drawable.width >= 2048 ||
	pSrcPixmap->drawable.height >= 2048) {
	RADEON_FALLBACK(("Source w/h too large (%d,%d).\n",
			 pSrcPixmap->drawable.width,
			 pSrcPixmap->drawable.height));
    }

    pDstPixmap = RADEONGetDrawablePixmap(pDstPicture->pDrawable);

    if (pDstPixmap->drawable.width >= 2048 ||
	pDstPixmap->drawable.height >= 2048) {
	RADEON_FALLBACK(("Dest w/h too large (%d,%d).\n",
			 pDstPixmap->drawable.width,
			 pDstPixmap->drawable.height));
    }

    if (pMaskPicture) {
	PixmapPtr pMaskPixmap = RADEONGetDrawablePixmap(pMaskPicture->pDrawable);

	if (pMaskPixmap->drawable.width >= 2048 ||
	    pMaskPixmap->drawable.height >= 2048) {
	    RADEON_FALLBACK(("Mask w/h too large (%d,%d).\n",
			     pMaskPixmap->drawable.width,
			     pMaskPixmap->drawable.height));
	}

	if (pMaskPicture->componentAlpha) {
	    /* Check if it's component alpha that relies on a source alpha and
	     * on the source value.  We can only get one of those into the
	     * single source value that we get to blend with.
	     */
	    if (RadeonBlendOp[op].src_alpha &&
		(RadeonBlendOp[op].blend_cntl & RADEON_SRC_BLEND_MASK) !=
		RADEON_SRC_BLEND_GL_ZERO) {
		RADEON_FALLBACK(("Component alpha not supported with source "
				 "alpha and source value blending.\n"));
	    }
	}

	if (!R200CheckCompositeTexture(pMaskPicture, 1))
	    return FALSE;
    }

    if (!R200CheckCompositeTexture(pSrcPicture, 0))
	return FALSE;

    if (!RADEONGetDestFormat(pDstPicture, &tmp1))
	return FALSE;

    return TRUE;
}
# endif /* ONLY_ONCE */

static Bool FUNC_NAME(R200PrepareComposite)(int op, PicturePtr pSrcPicture,
				PicturePtr pMaskPicture, PicturePtr pDstPicture,
				PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
    VAR_PREAMBLE(pDst->drawable.pScreen);
    THREEDSTATE_PREAMBLE();
    uint32_t dst_format, dst_offset, dst_pitch;
    uint32_t pp_cntl, blendcntl, cblend, ablend, colorpitch;
    int pixel_shift;
    ACCEL_PREAMBLE();

    TRACE;

    if (!accel_state->XHas3DEngineState)
	RADEONInit3DEngine(pScrn);

    if (!RADEONGetDestFormat(pDstPicture, &dst_format))
	return FALSE;

    if (pMask)
	accel_state->has_mask = TRUE;
    else
	accel_state->has_mask = FALSE;

    pixel_shift = pDst->drawable.bitsPerPixel >> 4;

    dst_offset = exaGetPixmapOffset(pDst) + FB_OFFSET;
    dst_pitch = exaGetPixmapPitch(pDst);
    colorpitch = dst_pitch >> pixel_shift;
    if (RADEONPixmapIsColortiled(pDst))
	colorpitch |= RADEON_COLOR_TILE_ENABLE;

    if ((dst_offset & 0x0f) != 0)
	RADEON_FALLBACK(("Bad destination offset 0x%x\n", (int)dst_offset));
    if (((dst_pitch >> pixel_shift) & 0x7) != 0)
	RADEON_FALLBACK(("Bad destination pitch 0x%x\n", (int)dst_pitch));

    if (!RADEONSetupSourceTile(pSrcPicture, pSrc, FALSE, TRUE))
	return FALSE;

    if (!FUNC_NAME(R200TextureSetup)(pSrcPicture, pSrc, 0))
	return FALSE;
    pp_cntl = RADEON_TEX_0_ENABLE | RADEON_TEX_BLEND_0_ENABLE;

    if (pMask != NULL) {
	if (!FUNC_NAME(R200TextureSetup)(pMaskPicture, pMask, 1))
	    return FALSE;
	pp_cntl |= RADEON_TEX_1_ENABLE;
    } else {
	accel_state->is_transform[1] = FALSE;
    }

    RADEON_SWITCH_TO_3D();

    BEGIN_ACCEL(11);

    OUT_ACCEL_REG(RADEON_PP_CNTL, pp_cntl);
    OUT_ACCEL_REG(RADEON_RB3D_CNTL, dst_format | RADEON_ALPHA_BLEND_ENABLE);
    OUT_ACCEL_REG(RADEON_RB3D_COLOROFFSET, dst_offset);

    OUT_ACCEL_REG(R200_SE_VTX_FMT_0, R200_VTX_XY);
    if (pMask)
	OUT_ACCEL_REG(R200_SE_VTX_FMT_1,
		      (2 << R200_VTX_TEX0_COMP_CNT_SHIFT) |
		      (2 << R200_VTX_TEX1_COMP_CNT_SHIFT));
    else
	OUT_ACCEL_REG(R200_SE_VTX_FMT_1,
		      (2 << R200_VTX_TEX0_COMP_CNT_SHIFT));

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
#endif /* IS_RADEON_DRIVER */

#ifdef ONLY_ONCE

static Bool R300CheckCompositeTexture(PicturePtr pPict,
				      PicturePtr pDstPict,
				      int op,
				      int unit,
				      Bool is_r500)
{
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    unsigned int i;
    int max_tex_w, max_tex_h;

    if (is_r500) {
	max_tex_w = 4096;
	max_tex_h = 4096;
    } else {
	max_tex_w = 2048;
	max_tex_h = 2048;
    }

    if ((w > max_tex_w) || (h > max_tex_h))
	RADEON_FALLBACK(("Picture w/h too large (%dx%d)\n", w, h));

    for (i = 0; i < sizeof(R300TexFormats) / sizeof(R300TexFormats[0]); i++)
    {
	if (R300TexFormats[i].fmt == pPict->format)
	    break;
    }
    if (i == sizeof(R300TexFormats) / sizeof(R300TexFormats[0]))
	RADEON_FALLBACK(("Unsupported picture format 0x%x\n",
			 (int)pPict->format));

    if (!RADEONCheckTexturePOT(pPict, unit == 0))
	return FALSE;

    if (pPict->filter != PictFilterNearest &&
	pPict->filter != PictFilterBilinear)
	RADEON_FALLBACK(("Unsupported filter 0x%x\n", pPict->filter));

    /* for REPEAT_NONE, Render semantics are that sampling outside the source
     * picture results in alpha=0 pixels. We can implement this with a border color
     * *if* our source texture has an alpha channel, otherwise we need to fall
     * back. If we're not transformed then we hope that upper layers have clipped
     * rendering to the bounds of the source drawable, in which case it doesn't
     * matter. I have not, however, verified that the X server always does such
     * clipping.
     */
    if (pPict->transform != 0 && !pPict->repeat && PICT_FORMAT_A(pPict->format) == 0) {
	if (!(((op == PictOpSrc) || (op == PictOpClear)) && (PICT_FORMAT_A(pDstPict->format) == 0)))
	    RADEON_FALLBACK(("REPEAT_NONE unsupported for transformed xRGB source\n"));
    }

    if (pPict->repeat && pPict->repeatType != RepeatNormal)
    {
	RADEON_FALLBACK(("Unsupported repeat type %d\n", pPict->repeat));
    }

    return TRUE;
}

#endif /* ONLY_ONCE */

static Bool FUNC_NAME(R300TextureSetup)(PicturePtr pPict, PixmapPtr pPix,
					int unit)
{
    VAR_PREAMBLE(pPix->drawable.pScreen);
    THREEDSTATE_PREAMBLE();
    uint32_t txfilter, txformat0, txformat1, txoffset, txpitch;
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    unsigned int i;
    int pixel_shift;
    ACCEL_PREAMBLE();

    TRACE;

    txpitch = exaGetPixmapPitch(pPix);
    txoffset = exaGetPixmapOffset(pPix) + FB_OFFSET;

    if ((txoffset & 0x1f) != 0)
	RADEON_FALLBACK(("Bad texture offset 0x%x\n", (int)txoffset));
    if ((txpitch & 0x1f) != 0)
	RADEON_FALLBACK(("Bad texture pitch 0x%x\n", (int)txpitch));

    /* TXPITCH = pixels (texels) per line - 1 */
    pixel_shift = pPix->drawable.bitsPerPixel >> 4;
    txpitch >>= pixel_shift;
    txpitch -= 1;

    if (RADEONPixmapIsColortiled(pPix))
	txoffset |= R300_MACRO_TILE;

    for (i = 0; i < sizeof(R300TexFormats) / sizeof(R300TexFormats[0]); i++)
    {
	if (R300TexFormats[i].fmt == pPict->format)
	    break;
    }

    txformat1 = R300TexFormats[i].card_fmt;

    txformat0 = ((((w - 1) & 0x7ff) << R300_TXWIDTH_SHIFT) |
		 (((h - 1) & 0x7ff) << R300_TXHEIGHT_SHIFT));

    if (IS_R500_3D && ((w - 1) & 0x800))
	txpitch |= R500_TXWIDTH_11;

    if (IS_R500_3D && ((h - 1) & 0x800))
	txpitch |= R500_TXHEIGHT_11;

    /* Use TXPITCH instead of TXWIDTH for address computations: we could
     * omit this if there is no padding, but there is no apparent advantage
     * in doing so.
     */
    txformat0 |= R300_TXPITCH_EN;

    accel_state->texW[unit] = w;
    accel_state->texH[unit] = h;

    if (pPict->repeat && !(unit == 0 && accel_state->need_src_tile_x))
      txfilter = R300_TX_CLAMP_S(R300_TX_CLAMP_WRAP);
    else
      txfilter = R300_TX_CLAMP_S(R300_TX_CLAMP_CLAMP_GL);

    if (pPict->repeat && !(unit == 0 && accel_state->need_src_tile_y))
      txfilter |= R300_TX_CLAMP_T(R300_TX_CLAMP_WRAP);
    else
      txfilter |= R300_TX_CLAMP_T(R300_TX_CLAMP_CLAMP_GL);

    txfilter |= (unit << R300_TX_ID_SHIFT);

    switch (pPict->filter) {
    case PictFilterNearest:
	txfilter |= (R300_TX_MAG_FILTER_NEAREST | R300_TX_MIN_FILTER_NEAREST);
	break;
    case PictFilterBilinear:
	txfilter |= (R300_TX_MAG_FILTER_LINEAR | R300_TX_MIN_FILTER_LINEAR);
	break;
    default:
	RADEON_FALLBACK(("Bad filter 0x%x\n", pPict->filter));
    }

    BEGIN_ACCEL(pPict->repeat ? 6 : 7);
    OUT_ACCEL_REG(R300_TX_FILTER0_0 + (unit * 4), txfilter);
    OUT_ACCEL_REG(R300_TX_FILTER1_0 + (unit * 4), 0);
    OUT_ACCEL_REG(R300_TX_FORMAT0_0 + (unit * 4), txformat0);
    OUT_ACCEL_REG(R300_TX_FORMAT1_0 + (unit * 4), txformat1);
    OUT_ACCEL_REG(R300_TX_FORMAT2_0 + (unit * 4), txpitch);
    OUT_ACCEL_REG(R300_TX_OFFSET_0 + (unit * 4), txoffset);
    if (!pPict->repeat)
	OUT_ACCEL_REG(R300_TX_BORDER_COLOR_0 + (unit * 4), 0);
    FINISH_ACCEL();

    if (pPict->transform != 0) {
	accel_state->is_transform[unit] = TRUE;
	accel_state->transform[unit] = pPict->transform;
    } else {
	accel_state->is_transform[unit] = FALSE;
    }

    return TRUE;
}

#ifdef ONLY_ONCE

static Bool R300CheckComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
			       PicturePtr pDstPicture)
{
    VAR_PREAMBLE(pSrcPicture->pDrawable->pScreen);
    uint32_t tmp1;
    PixmapPtr pSrcPixmap, pDstPixmap;
    int max_tex_w, max_tex_h, max_dst_w, max_dst_h;

    TRACE;

    /* Check for unsupported compositing operations. */
    if ((unsigned int)op >= sizeof(RadeonBlendOp) / sizeof(RadeonBlendOp[0]))
	RADEON_FALLBACK(("Unsupported Composite op 0x%x\n", op));

    pSrcPixmap = RADEONGetDrawablePixmap(pSrcPicture->pDrawable);

    if (IS_R500_3D) {
	max_tex_w = 4096;
	max_tex_h = 4096;
	max_dst_w = 4096;
	max_dst_h = 4096;
    } else {
	max_tex_w = 2048;
	max_tex_h = 2048;
	max_dst_w = 2560;
	max_dst_h = 2560;
    }

    if (pSrcPixmap->drawable.width >= max_tex_w ||
	pSrcPixmap->drawable.height >= max_tex_h) {
	RADEON_FALLBACK(("Source w/h too large (%d,%d).\n",
			 pSrcPixmap->drawable.width,
			 pSrcPixmap->drawable.height));
    }

    pDstPixmap = RADEONGetDrawablePixmap(pDstPicture->pDrawable);

    if (pDstPixmap->drawable.width >= max_dst_w ||
	pDstPixmap->drawable.height >= max_dst_h) {
	RADEON_FALLBACK(("Dest w/h too large (%d,%d).\n",
			 pDstPixmap->drawable.width,
			 pDstPixmap->drawable.height));
    }

    if (pMaskPicture) {
	PixmapPtr pMaskPixmap = RADEONGetDrawablePixmap(pMaskPicture->pDrawable);

	if (pMaskPixmap->drawable.width >= max_tex_w ||
	    pMaskPixmap->drawable.height >= max_tex_h) {
	    RADEON_FALLBACK(("Mask w/h too large (%d,%d).\n",
			     pMaskPixmap->drawable.width,
			     pMaskPixmap->drawable.height));
	}

	if (pMaskPicture->componentAlpha) {
	    /* Check if it's component alpha that relies on a source alpha and
	     * on the source value.  We can only get one of those into the
	     * single source value that we get to blend with.
	     */
	    if (RadeonBlendOp[op].src_alpha &&
		(RadeonBlendOp[op].blend_cntl & RADEON_SRC_BLEND_MASK) !=
		RADEON_SRC_BLEND_GL_ZERO) {
		RADEON_FALLBACK(("Component alpha not supported with source "
				 "alpha and source value blending.\n"));
	    }
	}

	if (!R300CheckCompositeTexture(pMaskPicture, pDstPicture, op, 1, IS_R500_3D))
	    return FALSE;
    }

    if (!R300CheckCompositeTexture(pSrcPicture, pDstPicture, op, 0, IS_R500_3D))
	return FALSE;

    if (!R300GetDestFormat(pDstPicture, &tmp1))
	return FALSE;

    return TRUE;

}
#endif /* ONLY_ONCE */

static Bool FUNC_NAME(R300PrepareComposite)(int op, PicturePtr pSrcPicture,
				PicturePtr pMaskPicture, PicturePtr pDstPicture,
				PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
    VAR_PREAMBLE(pDst->drawable.pScreen);
    THREEDSTATE_PREAMBLE();
    uint32_t dst_format, dst_offset, dst_pitch;
    uint32_t txenable, colorpitch;
    uint32_t blendcntl;
    int pixel_shift;
    ACCEL_PREAMBLE();

    TRACE;

    if (!accel_state->XHas3DEngineState)
	RADEONInit3DEngine(pScrn);

    if (!R300GetDestFormat(pDstPicture, &dst_format))
	return FALSE;

    if (pMask)
	accel_state->has_mask = TRUE;
    else
	accel_state->has_mask = FALSE;

    pixel_shift = pDst->drawable.bitsPerPixel >> 4;

    dst_offset = exaGetPixmapOffset(pDst) + FB_OFFSET;
    dst_pitch = exaGetPixmapPitch(pDst);
    colorpitch = dst_pitch >> pixel_shift;

    if (RADEONPixmapIsColortiled(pDst))
	colorpitch |= R300_COLORTILE;

    colorpitch |= dst_format;

    if ((dst_offset & 0x0f) != 0)
	RADEON_FALLBACK(("Bad destination offset 0x%x\n", (int)dst_offset));
    if (((dst_pitch >> pixel_shift) & 0x7) != 0)
	RADEON_FALLBACK(("Bad destination pitch 0x%x\n", (int)dst_pitch));

    if (!RADEONSetupSourceTile(pSrcPicture, pSrc, TRUE, FALSE))
	return FALSE;

    if (!FUNC_NAME(R300TextureSetup)(pSrcPicture, pSrc, 0))
	return FALSE;
    txenable = R300_TEX_0_ENABLE;

    if (pMask != NULL) {
	if (!FUNC_NAME(R300TextureSetup)(pMaskPicture, pMask, 1))
	    return FALSE;
	txenable |= R300_TEX_1_ENABLE;
    } else {
	accel_state->is_transform[1] = FALSE;
    }

    RADEON_SWITCH_TO_3D();

    /* setup the VAP */
    if (HAS_TCL) {
	if (pMask)
	    BEGIN_ACCEL(8);
	else
	    BEGIN_ACCEL(7);
    } else {
	if (pMask)
	    BEGIN_ACCEL(6);
	else
	    BEGIN_ACCEL(5);
    }

    /* These registers define the number, type, and location of data submitted
     * to the PVS unit of GA input (when PVS is disabled)
     * DST_VEC_LOC is the slot in the PVS input vector memory when PVS/TCL is
     * enabled.  This memory provides the imputs to the vertex shader program
     * and ordering is not important.  When PVS/TCL is disabled, this field maps
     * directly to the GA input memory and the order is signifigant.  In
     * PVS_BYPASS mode the order is as follows:
     * Position
     * Point Size
     * Color 0-3
     * Textures 0-7
     * Fog
     */
    if (pMask) {
	OUT_ACCEL_REG(R300_VAP_PROG_STREAM_CNTL_0,
		      ((R300_DATA_TYPE_FLOAT_2 << R300_DATA_TYPE_0_SHIFT) |
		       (0 << R300_SKIP_DWORDS_0_SHIFT) |
		       (0 << R300_DST_VEC_LOC_0_SHIFT) |
		       R300_SIGNED_0 |
		       (R300_DATA_TYPE_FLOAT_2 << R300_DATA_TYPE_1_SHIFT) |
		       (0 << R300_SKIP_DWORDS_1_SHIFT) |
		       (6 << R300_DST_VEC_LOC_1_SHIFT) |
		       R300_SIGNED_1));
	OUT_ACCEL_REG(R300_VAP_PROG_STREAM_CNTL_1,
		      ((R300_DATA_TYPE_FLOAT_2 << R300_DATA_TYPE_2_SHIFT) |
		       (0 << R300_SKIP_DWORDS_2_SHIFT) |
		       (7 << R300_DST_VEC_LOC_2_SHIFT) |
		       R300_LAST_VEC_2 |
		       R300_SIGNED_2));
    } else
	OUT_ACCEL_REG(R300_VAP_PROG_STREAM_CNTL_0,
		      ((R300_DATA_TYPE_FLOAT_2 << R300_DATA_TYPE_0_SHIFT) |
		       (0 << R300_SKIP_DWORDS_0_SHIFT) |
		       (0 << R300_DST_VEC_LOC_0_SHIFT) |
		       R300_SIGNED_0 |
		       (R300_DATA_TYPE_FLOAT_2 << R300_DATA_TYPE_1_SHIFT) |
		       (0 << R300_SKIP_DWORDS_1_SHIFT) |
		       (6 << R300_DST_VEC_LOC_1_SHIFT) |
		       R300_LAST_VEC_1 |
		       R300_SIGNED_1));

    /* load the vertex shader
     * We pre-load vertex programs in RADEONInit3DEngine():
     * - exa no mask
     * - exa mask
     * - Xv
     * Here we select the offset of the vertex program we want to use
     */
    if (HAS_TCL) {
	if (pMask) {
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_0,
			  ((0 << R300_PVS_FIRST_INST_SHIFT) |
			   (2 << R300_PVS_XYZW_VALID_INST_SHIFT) |
			   (2 << R300_PVS_LAST_INST_SHIFT)));
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_1,
			  (2 << R300_PVS_LAST_VTX_SRC_INST_SHIFT));
	} else {
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_0,
			  ((3 << R300_PVS_FIRST_INST_SHIFT) |
			   (4 << R300_PVS_XYZW_VALID_INST_SHIFT) |
			   (4 << R300_PVS_LAST_INST_SHIFT)));
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_1,
			  (4 << R300_PVS_LAST_VTX_SRC_INST_SHIFT));
	}
    }

    /* Position and one or two sets of 2 texture coordinates */
    OUT_ACCEL_REG(R300_VAP_OUT_VTX_FMT_0, R300_VTX_POS_PRESENT);
    if (pMask)
	OUT_ACCEL_REG(R300_VAP_OUT_VTX_FMT_1,
		      ((2 << R300_TEX_0_COMP_CNT_SHIFT) |
		       (2 << R300_TEX_1_COMP_CNT_SHIFT)));
    else
	OUT_ACCEL_REG(R300_VAP_OUT_VTX_FMT_1,
		      (2 << R300_TEX_0_COMP_CNT_SHIFT));

    OUT_ACCEL_REG(R300_TX_INVALTAGS, 0x0);
    OUT_ACCEL_REG(R300_TX_ENABLE, txenable);
    FINISH_ACCEL();

    /* setup pixel shader */
    if (IS_R300_3D) {
	uint32_t output_fmt;
	int src_color, src_alpha;
	int mask_color, mask_alpha;

	if (PICT_FORMAT_RGB(pSrcPicture->format) == 0)
	    src_color = R300_ALU_RGB_0_0;
	else
	    src_color = R300_ALU_RGB_SRC0_RGB;

	if (PICT_FORMAT_A(pSrcPicture->format) == 0)
	    src_alpha = R300_ALU_ALPHA_1_0;
	else
	    src_alpha = R300_ALU_ALPHA_SRC0_A;

	if (pMask && pMaskPicture->componentAlpha) {
	    if (RadeonBlendOp[op].src_alpha) {
		if (PICT_FORMAT_A(pSrcPicture->format) == 0) {
		    src_color = R300_ALU_RGB_1_0;
		    src_alpha = R300_ALU_ALPHA_1_0;
		} else {
		    src_color = R300_ALU_RGB_SRC0_AAA;
		    src_alpha = R300_ALU_ALPHA_SRC0_A;
		}

		mask_color = R300_ALU_RGB_SRC1_RGB;

		if (PICT_FORMAT_A(pMaskPicture->format) == 0)
		    mask_alpha = R300_ALU_ALPHA_1_0;
		else
		    mask_alpha = R300_ALU_ALPHA_SRC1_A;

	    } else {
		src_color = R300_ALU_RGB_SRC0_RGB;

		if (PICT_FORMAT_A(pSrcPicture->format) == 0)
		    src_alpha = R300_ALU_ALPHA_1_0;
		else
		    src_alpha = R300_ALU_ALPHA_SRC0_A;

		mask_color = R300_ALU_RGB_SRC1_RGB;

		if (PICT_FORMAT_A(pMaskPicture->format) == 0)
		    mask_alpha = R300_ALU_ALPHA_1_0;
		else
		    mask_alpha = R300_ALU_ALPHA_SRC1_A;

	    }
	} else if (pMask) {
	    if (PICT_FORMAT_A(pMaskPicture->format) == 0)
		mask_color = R300_ALU_RGB_1_0;
	    else
		mask_color = R300_ALU_RGB_SRC1_AAA;

	    if (PICT_FORMAT_A(pMaskPicture->format) == 0)
		mask_alpha = R300_ALU_ALPHA_1_0;
	    else
		mask_alpha = R300_ALU_ALPHA_SRC1_A;
	} else {
	    mask_color = R300_ALU_RGB_1_0;
	    mask_alpha = R300_ALU_ALPHA_1_0;
	}

	/* shader output swizzling */
	switch (pDstPicture->format) {
	case PICT_a8r8g8b8:
	case PICT_x8r8g8b8:
	default:
	    output_fmt = (R300_OUT_FMT_C4_8 |
			  R300_OUT_FMT_C0_SEL_BLUE |
			  R300_OUT_FMT_C1_SEL_GREEN |
			  R300_OUT_FMT_C2_SEL_RED |
			  R300_OUT_FMT_C3_SEL_ALPHA);
	    break;
	case PICT_a8b8g8r8:
	case PICT_x8b8g8r8:
	    output_fmt = (R300_OUT_FMT_C4_8 |
			  R300_OUT_FMT_C0_SEL_RED |
			  R300_OUT_FMT_C1_SEL_GREEN |
			  R300_OUT_FMT_C2_SEL_BLUE |
			  R300_OUT_FMT_C3_SEL_ALPHA);
	    break;
	case PICT_a8:
	    output_fmt = (R300_OUT_FMT_C4_8 |
			  R300_OUT_FMT_C0_SEL_ALPHA);
	    break;
	}


	/* setup the rasterizer, load FS */
	BEGIN_ACCEL(9);
	if (pMask) {
	    /* 4 components: 2 for tex0, 2 for tex1 */
	    OUT_ACCEL_REG(R300_RS_COUNT,
			  ((4 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));

	    /* R300_INST_COUNT_RS - highest RS instruction used */
	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(1));

	    OUT_ACCEL_REG(R300_US_CODE_OFFSET, (R300_ALU_CODE_OFFSET(0) |
						R300_ALU_CODE_SIZE(0) |
						R300_TEX_CODE_OFFSET(0) |
						R300_TEX_CODE_SIZE(1)));

	    OUT_ACCEL_REG(R300_US_CODE_ADDR_3,
			  (R300_ALU_START(0) |
			   R300_ALU_SIZE(0) |
			   R300_TEX_START(0) |
			   R300_TEX_SIZE(1) |
			   R300_RGBA_OUT));
	} else {
	    /* 2 components: 2 for tex0 */
	    OUT_ACCEL_REG(R300_RS_COUNT,
			  ((2 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));

	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(0));

	    OUT_ACCEL_REG(R300_US_CODE_OFFSET, (R300_ALU_CODE_OFFSET(0) |
						R300_ALU_CODE_SIZE(0) |
						R300_TEX_CODE_OFFSET(0) |
						R300_TEX_CODE_SIZE(0)));

	    OUT_ACCEL_REG(R300_US_CODE_ADDR_3,
			  (R300_ALU_START(0) |
			   R300_ALU_SIZE(0) |
			   R300_TEX_START(0) |
			   R300_TEX_SIZE(0) |
			   R300_RGBA_OUT));
	}

	/* shader output swizzling */
	OUT_ACCEL_REG(R300_US_OUT_FMT_0, output_fmt);

	/* tex inst for src texture is pre-loaded in RADEONInit3DEngine() */
	/* tex inst for mask texture is pre-loaded in RADEONInit3DEngine() */

	/* RGB inst
	 * temp addresses for texture inputs
	 * ALU_RGB_ADDR0 is src tex (temp 0)
	 * ALU_RGB_ADDR1 is mask tex (temp 1)
	 * R300_ALU_RGB_OMASK - output components to write
	 * R300_ALU_RGB_TARGET_A - render target
	 */
	OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR_0,
		      (R300_ALU_RGB_ADDR0(0) |
		       R300_ALU_RGB_ADDR1(1) |
		       R300_ALU_RGB_ADDR2(0) |
		       R300_ALU_RGB_ADDRD(0) |
		       R300_ALU_RGB_OMASK((R300_ALU_RGB_MASK_R |
					   R300_ALU_RGB_MASK_G |
					   R300_ALU_RGB_MASK_B)) |
		       R300_ALU_RGB_TARGET_A));
	/* RGB inst
	 * ALU operation
	 */
	OUT_ACCEL_REG(R300_US_ALU_RGB_INST_0,
		      (R300_ALU_RGB_SEL_A(src_color) |
		       R300_ALU_RGB_MOD_A(R300_ALU_RGB_MOD_NOP) |
		       R300_ALU_RGB_SEL_B(mask_color) |
		       R300_ALU_RGB_MOD_B(R300_ALU_RGB_MOD_NOP) |
		       R300_ALU_RGB_SEL_C(R300_ALU_RGB_0_0) |
		       R300_ALU_RGB_MOD_C(R300_ALU_RGB_MOD_NOP) |
		       R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
		       R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE) |
		       R300_ALU_RGB_CLAMP));
	/* Alpha inst
	 * temp addresses for texture inputs
	 * ALU_ALPHA_ADDR0 is src tex (0)
	 * ALU_ALPHA_ADDR1 is mask tex (1)
	 * R300_ALU_ALPHA_OMASK - output components to write
	 * R300_ALU_ALPHA_TARGET_A - render target
	 */
	OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR_0,
		      (R300_ALU_ALPHA_ADDR0(0) |
		       R300_ALU_ALPHA_ADDR1(1) |
		       R300_ALU_ALPHA_ADDR2(0) |
		       R300_ALU_ALPHA_ADDRD(0) |
		       R300_ALU_ALPHA_OMASK(R300_ALU_ALPHA_MASK_A) |
		       R300_ALU_ALPHA_TARGET_A |
		       R300_ALU_ALPHA_OMASK_W(R300_ALU_ALPHA_MASK_NONE)));
	/* Alpha inst
	 * ALU operation
	 */
	OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST_0,
		      (R300_ALU_ALPHA_SEL_A(src_alpha) |
		       R300_ALU_ALPHA_MOD_A(R300_ALU_ALPHA_MOD_NOP) |
		       R300_ALU_ALPHA_SEL_B(mask_alpha) |
		       R300_ALU_ALPHA_MOD_B(R300_ALU_ALPHA_MOD_NOP) |
		       R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0) |
		       R300_ALU_ALPHA_MOD_C(R300_ALU_ALPHA_MOD_NOP) |
		       R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
		       R300_ALU_ALPHA_OMOD(R300_ALU_ALPHA_OMOD_NONE) |
		       R300_ALU_ALPHA_CLAMP));
	FINISH_ACCEL();
    } else {
	uint32_t output_fmt;
	uint32_t src_color, src_alpha;
	uint32_t mask_color, mask_alpha;

	if (PICT_FORMAT_RGB(pSrcPicture->format) == 0)
	    src_color = (R500_ALU_RGB_R_SWIZ_A_0 |
			 R500_ALU_RGB_G_SWIZ_A_0 |
			 R500_ALU_RGB_B_SWIZ_A_0);
	else
	    src_color = (R500_ALU_RGB_R_SWIZ_A_R |
			 R500_ALU_RGB_G_SWIZ_A_G |
			 R500_ALU_RGB_B_SWIZ_A_B);

	if (PICT_FORMAT_A(pSrcPicture->format) == 0)
	    src_alpha = R500_ALPHA_SWIZ_A_1;
	else
	    src_alpha = R500_ALPHA_SWIZ_A_A;

	if (pMask && pMaskPicture->componentAlpha) {
	    if (RadeonBlendOp[op].src_alpha) {
		if (PICT_FORMAT_A(pSrcPicture->format) == 0) {
		    src_color = (R500_ALU_RGB_R_SWIZ_A_1 |
				 R500_ALU_RGB_G_SWIZ_A_1 |
				 R500_ALU_RGB_B_SWIZ_A_1);
		    src_alpha = R500_ALPHA_SWIZ_A_1;
		} else {
		    src_color = (R500_ALU_RGB_R_SWIZ_A_A |
				 R500_ALU_RGB_G_SWIZ_A_A |
				 R500_ALU_RGB_B_SWIZ_A_A);
		    src_alpha = R500_ALPHA_SWIZ_A_A;
		}

		mask_color = (R500_ALU_RGB_R_SWIZ_B_R |
			      R500_ALU_RGB_G_SWIZ_B_G |
			      R500_ALU_RGB_B_SWIZ_B_B);

		if (PICT_FORMAT_A(pMaskPicture->format) == 0)
		    mask_alpha = R500_ALPHA_SWIZ_B_1;
		else
		    mask_alpha = R500_ALPHA_SWIZ_B_A;

	    } else {
		src_color = (R500_ALU_RGB_R_SWIZ_A_R |
			     R500_ALU_RGB_G_SWIZ_A_G |
			     R500_ALU_RGB_B_SWIZ_A_B);

		if (PICT_FORMAT_A(pSrcPicture->format) == 0)
		    src_alpha = R500_ALPHA_SWIZ_A_1;
		else
		    src_alpha = R500_ALPHA_SWIZ_A_A;

		mask_color = (R500_ALU_RGB_R_SWIZ_B_R |
			      R500_ALU_RGB_G_SWIZ_B_G |
			      R500_ALU_RGB_B_SWIZ_B_B);

		if (PICT_FORMAT_A(pMaskPicture->format) == 0)
		    mask_alpha = R500_ALPHA_SWIZ_B_1;
		else
		    mask_alpha = R500_ALPHA_SWIZ_B_A;

	    }
	} else if (pMask) {
	    if (PICT_FORMAT_A(pMaskPicture->format) == 0)
		mask_color = (R500_ALU_RGB_R_SWIZ_B_1 |
			      R500_ALU_RGB_G_SWIZ_B_1 |
			      R500_ALU_RGB_B_SWIZ_B_1);
	    else
		mask_color = (R500_ALU_RGB_R_SWIZ_B_A |
			      R500_ALU_RGB_G_SWIZ_B_A |
			      R500_ALU_RGB_B_SWIZ_B_A);

	    if (PICT_FORMAT_A(pMaskPicture->format) == 0)
		mask_alpha = R500_ALPHA_SWIZ_B_1;
	    else
		mask_alpha = R500_ALPHA_SWIZ_B_A;
	} else {
	    mask_color = (R500_ALU_RGB_R_SWIZ_B_1 |
			  R500_ALU_RGB_G_SWIZ_B_1 |
			  R500_ALU_RGB_B_SWIZ_B_1);
	    mask_alpha = R500_ALPHA_SWIZ_B_1;
	}

	/* shader output swizzling */
	switch (pDstPicture->format) {
	case PICT_a8r8g8b8:
	case PICT_x8r8g8b8:
	default:
	    output_fmt = (R300_OUT_FMT_C4_8 |
			  R300_OUT_FMT_C0_SEL_BLUE |
			  R300_OUT_FMT_C1_SEL_GREEN |
			  R300_OUT_FMT_C2_SEL_RED |
			  R300_OUT_FMT_C3_SEL_ALPHA);
	    break;
	case PICT_a8b8g8r8:
	case PICT_x8b8g8r8:
	    output_fmt = (R300_OUT_FMT_C4_8 |
			  R300_OUT_FMT_C0_SEL_RED |
			  R300_OUT_FMT_C1_SEL_GREEN |
			  R300_OUT_FMT_C2_SEL_BLUE |
			  R300_OUT_FMT_C3_SEL_ALPHA);
	    break;
	case PICT_a8:
	    output_fmt = (R300_OUT_FMT_C4_8 |
			  R300_OUT_FMT_C0_SEL_ALPHA);
	    break;
	}

	BEGIN_ACCEL(6);
	if (pMask) {
	    /* 4 components: 2 for tex0, 2 for tex1 */
	    OUT_ACCEL_REG(R300_RS_COUNT,
			  ((4 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));

	    /* 2 RS instructions: 1 for tex0 (src), 1 for tex1 (mask) */
	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(1));

	    OUT_ACCEL_REG(R500_US_CODE_ADDR, (R500_US_CODE_START_ADDR(0) |
					      R500_US_CODE_END_ADDR(2)));
	    OUT_ACCEL_REG(R500_US_CODE_RANGE, (R500_US_CODE_RANGE_ADDR(0) |
					       R500_US_CODE_RANGE_SIZE(2)));
	    OUT_ACCEL_REG(R500_US_CODE_OFFSET, 0);
	} else {
	    OUT_ACCEL_REG(R300_RS_COUNT,
			  ((2 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));

	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(0));

	    OUT_ACCEL_REG(R500_US_CODE_ADDR, (R500_US_CODE_START_ADDR(0) |
					      R500_US_CODE_END_ADDR(1)));
	    OUT_ACCEL_REG(R500_US_CODE_RANGE, (R500_US_CODE_RANGE_ADDR(0) |
					       R500_US_CODE_RANGE_SIZE(1)));
	    OUT_ACCEL_REG(R500_US_CODE_OFFSET, 0);
	}

	OUT_ACCEL_REG(R300_US_OUT_FMT_0, output_fmt);
	FINISH_ACCEL();

	if (pMask) {
	    BEGIN_ACCEL(19);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_INDEX, 0);
	    /* tex inst for src texture */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_TEX |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK |
						   R500_INST_RGB_CLAMP |
						   R500_INST_ALPHA_CLAMP));

	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_ID(0) |
						   R500_TEX_INST_LD |
						   R500_TEX_IGNORE_UNCOVERED));

	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(0) |
						   R500_TEX_SRC_S_SWIZ_R |
						   R500_TEX_SRC_T_SWIZ_G |
						   R500_TEX_DST_ADDR(0) |
						   R500_TEX_DST_R_SWIZ_R |
						   R500_TEX_DST_G_SWIZ_G |
						   R500_TEX_DST_B_SWIZ_B |
						   R500_TEX_DST_A_SWIZ_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_DX_ADDR(0) |
						   R500_DX_S_SWIZ_R |
						   R500_DX_T_SWIZ_R |
						   R500_DX_R_SWIZ_R |
						   R500_DX_Q_SWIZ_R |
						   R500_DY_ADDR(0) |
						   R500_DY_S_SWIZ_R |
						   R500_DY_T_SWIZ_R |
						   R500_DY_R_SWIZ_R |
						   R500_DY_Q_SWIZ_R));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);

	    /* tex inst for mask texture */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_TEX |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK |
						   R500_INST_RGB_CLAMP |
						   R500_INST_ALPHA_CLAMP));

	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_ID(1) |
						   R500_TEX_INST_LD |
						   R500_TEX_SEM_ACQUIRE |
						   R500_TEX_IGNORE_UNCOVERED));

	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(1) |
						   R500_TEX_SRC_S_SWIZ_R |
						   R500_TEX_SRC_T_SWIZ_G |
						   R500_TEX_DST_ADDR(1) |
						   R500_TEX_DST_R_SWIZ_R |
						   R500_TEX_DST_G_SWIZ_G |
						   R500_TEX_DST_B_SWIZ_B |
						   R500_TEX_DST_A_SWIZ_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_DX_ADDR(1) |
						   R500_DX_S_SWIZ_R |
						   R500_DX_T_SWIZ_R |
						   R500_DX_R_SWIZ_R |
						   R500_DX_Q_SWIZ_R |
						   R500_DY_ADDR(1) |
						   R500_DY_S_SWIZ_R |
						   R500_DY_T_SWIZ_R |
						   R500_DY_R_SWIZ_R |
						   R500_DY_Q_SWIZ_R));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	} else {
	    BEGIN_ACCEL(13);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_INDEX, 0);
	    /* tex inst for src texture */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_TEX |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK |
						   R500_INST_RGB_CLAMP |
						   R500_INST_ALPHA_CLAMP));

	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_ID(0) |
						   R500_TEX_INST_LD |
						   R500_TEX_SEM_ACQUIRE |
						   R500_TEX_IGNORE_UNCOVERED));

	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(0) |
						   R500_TEX_SRC_S_SWIZ_R |
						   R500_TEX_SRC_T_SWIZ_G |
						   R500_TEX_DST_ADDR(0) |
						   R500_TEX_DST_R_SWIZ_R |
						   R500_TEX_DST_G_SWIZ_G |
						   R500_TEX_DST_B_SWIZ_B |
						   R500_TEX_DST_A_SWIZ_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_DX_ADDR(0) |
						   R500_DX_S_SWIZ_R |
						   R500_DX_T_SWIZ_R |
						   R500_DX_R_SWIZ_R |
						   R500_DX_Q_SWIZ_R |
						   R500_DY_ADDR(0) |
						   R500_DY_S_SWIZ_R |
						   R500_DY_T_SWIZ_R |
						   R500_DY_R_SWIZ_R |
						   R500_DY_Q_SWIZ_R));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	}

	/* ALU inst */
	/* *_OMASK* - output component write mask */
	OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_OUT |
					       R500_INST_TEX_SEM_WAIT |
					       R500_INST_LAST |
					       R500_INST_RGB_OMASK_R |
					       R500_INST_RGB_OMASK_G |
					       R500_INST_RGB_OMASK_B |
					       R500_INST_ALPHA_OMASK |
					       R500_INST_RGB_CLAMP |
					       R500_INST_ALPHA_CLAMP));
	/* ALU inst
	 * temp addresses for texture inputs
	 * RGB_ADDR0 is src tex (temp 0)
	 * RGB_ADDR1 is mask tex (temp 1)
	 */
	OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(0) |
					       R500_RGB_ADDR1(1) |
					       R500_RGB_ADDR2(0)));
	/* ALU inst
	 * temp addresses for texture inputs
	 * ALPHA_ADDR0 is src tex (temp 0)
	 * ALPHA_ADDR1 is mask tex (temp 1)
	 */
	OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(0) |
					       R500_ALPHA_ADDR1(1) |
					       R500_ALPHA_ADDR2(0)));

	/* R500_ALU_RGB_TARGET - RGB render target */
	OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
					       src_color |
					       R500_ALU_RGB_SEL_B_SRC1 |
					       mask_color |
					       R500_ALU_RGB_TARGET(0)));

	/* R500_ALPHA_RGB_TARGET - alpha render target */
	OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_OP_MAD |
					       R500_ALPHA_ADDRD(0) |
					       R500_ALPHA_SEL_A_SRC0 |
					       src_alpha |
					       R500_ALPHA_SEL_B_SRC1 |
					       mask_alpha |
					       R500_ALPHA_TARGET(0)));

	OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_OP_MAD |
					       R500_ALU_RGBA_ADDRD(0) |
					       R500_ALU_RGBA_R_SWIZ_0 |
					       R500_ALU_RGBA_G_SWIZ_0 |
					       R500_ALU_RGBA_B_SWIZ_0 |
					       R500_ALU_RGBA_A_SWIZ_0));
	FINISH_ACCEL();
    }

    BEGIN_ACCEL(3);

    OUT_ACCEL_REG(R300_RB3D_COLOROFFSET0, dst_offset);
    OUT_ACCEL_REG(R300_RB3D_COLORPITCH0, colorpitch);

    blendcntl = RADEONGetBlendCntl(op, pMaskPicture, pDstPicture->format);
    OUT_ACCEL_REG(R300_RB3D_BLENDCNTL, blendcntl | R300_ALPHA_BLEND_ENABLE | R300_READ_ENABLE);

    FINISH_ACCEL();

    return TRUE;
}

#define VTX_COUNT_MASK 6
#define VTX_COUNT 4

#ifdef ACCEL_CP

#define VTX_OUT_MASK(_dstX, _dstY, _srcX, _srcY, _maskX, _maskY)	\
do {								\
    OUT_RING_F(_dstX);						\
    OUT_RING_F(_dstY);						\
    OUT_RING_F(_srcX);						\
    OUT_RING_F(_srcY);						\
    OUT_RING_F(_maskX);						\
    OUT_RING_F(_maskY);						\
} while (0)

#define VTX_OUT(_dstX, _dstY, _srcX, _srcY)	\
do {								\
    OUT_RING_F(_dstX);						\
    OUT_RING_F(_dstY);						\
    OUT_RING_F(_srcX);						\
    OUT_RING_F(_srcY);						\
} while (0)

#else /* ACCEL_CP */

#define VTX_OUT_MASK(_dstX, _dstY, _srcX, _srcY, _maskX, _maskY)	\
do {								\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _dstX);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _dstY);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _srcX);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _srcY);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _maskX);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _maskY);		\
} while (0)

#define VTX_OUT(_dstX, _dstY, _srcX, _srcY)	\
do {								\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _dstX);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _dstY);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _srcX);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _srcY);		\
} while (0)

#endif /* !ACCEL_CP */

#ifdef ONLY_ONCE
static inline void transformPoint(PictTransform *transform, xPointFixed *point)
{
    PictVector v;
    v.vector[0] = point->x;
    v.vector[1] = point->y;
    v.vector[2] = xFixed1;
    PictureTransformPoint(transform, &v);
    point->x = v.vector[0];
    point->y = v.vector[1];
}
#endif

static void FUNC_NAME(RadeonCompositeTile)(PixmapPtr pDst,
					   int srcX, int srcY,
					   int maskX, int maskY,
					   int dstX, int dstY,
					   int w, int h)
{
    VAR_PREAMBLE(pDst->drawable.pScreen);
    THREEDSTATE_PREAMBLE();
    int vtx_count;
    xPointFixed srcTopLeft, srcTopRight, srcBottomLeft, srcBottomRight;
    xPointFixed maskTopLeft, maskTopRight, maskBottomLeft, maskBottomRight;
    ACCEL_PREAMBLE();

    ENTER_DRAW(0);

    /* ErrorF("RadeonComposite (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",
       srcX, srcY, maskX, maskY,dstX, dstY, w, h); */

    srcTopLeft.x     = IntToxFixed(srcX);
    srcTopLeft.y     = IntToxFixed(srcY);
    srcTopRight.x    = IntToxFixed(srcX + w);
    srcTopRight.y    = IntToxFixed(srcY);
    srcBottomLeft.x  = IntToxFixed(srcX);
    srcBottomLeft.y  = IntToxFixed(srcY + h);
    srcBottomRight.x = IntToxFixed(srcX + w);
    srcBottomRight.y = IntToxFixed(srcY + h);

    maskTopLeft.x     = IntToxFixed(maskX);
    maskTopLeft.y     = IntToxFixed(maskY);
    maskTopRight.x    = IntToxFixed(maskX + w);
    maskTopRight.y    = IntToxFixed(maskY);
    maskBottomLeft.x  = IntToxFixed(maskX);
    maskBottomLeft.y  = IntToxFixed(maskY + h);
    maskBottomRight.x = IntToxFixed(maskX + w);
    maskBottomRight.y = IntToxFixed(maskY + h);

    if (accel_state->is_transform[0]) {
	transformPoint(accel_state->transform[0], &srcTopLeft);
	transformPoint(accel_state->transform[0], &srcTopRight);
	transformPoint(accel_state->transform[0], &srcBottomLeft);
	transformPoint(accel_state->transform[0], &srcBottomRight);
    }
    if (accel_state->is_transform[1]) {
	transformPoint(accel_state->transform[1], &maskTopLeft);
	transformPoint(accel_state->transform[1], &maskTopRight);
	transformPoint(accel_state->transform[1], &maskBottomLeft);
	transformPoint(accel_state->transform[1], &maskBottomRight);
    }

    if (accel_state->has_mask)
	vtx_count = VTX_COUNT_MASK;
    else
	vtx_count = VTX_COUNT;

    if (IS_R300_3D || IS_R500_3D) {
	BEGIN_ACCEL(1);
	OUT_ACCEL_REG(R300_VAP_VTX_SIZE, vtx_count);
	FINISH_ACCEL();
    }

#ifdef ACCEL_CP
# ifdef IS_RADEON_DRIVER
    if (info->ChipFamily < CHIP_FAMILY_R200) {
	BEGIN_RING(3 * vtx_count + 3);
	OUT_RING(CP_PACKET3(RADEON_CP_PACKET3_3D_DRAW_IMMD,
			    3 * vtx_count + 1));
	if (accel_state->has_mask)
	    OUT_RING(RADEON_CP_VC_FRMT_XY |
		     RADEON_CP_VC_FRMT_ST0 |
		     RADEON_CP_VC_FRMT_ST1);
	else
	    OUT_RING(RADEON_CP_VC_FRMT_XY |
		     RADEON_CP_VC_FRMT_ST0);
	OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_RECT_LIST |
		 RADEON_CP_VC_CNTL_PRIM_WALK_RING |
		 RADEON_CP_VC_CNTL_MAOS_ENABLE |
		 RADEON_CP_VC_CNTL_VTX_FMT_RADEON_MODE |
		 (3 << RADEON_CP_VC_CNTL_NUM_SHIFT));
    } else
# endif /* IS_RADEON_DRIVER */
 {
	if (IS_R300_3D || IS_R500_3D)
	    BEGIN_RING(4 * vtx_count + 4);
	else
	    BEGIN_RING(4 * vtx_count + 2);

	OUT_RING(CP_PACKET3(R200_CP_PACKET3_3D_DRAW_IMMD_2,
			    4 * vtx_count));
	OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_QUAD_LIST |
		 RADEON_CP_VC_CNTL_PRIM_WALK_RING |
		 (4 << RADEON_CP_VC_CNTL_NUM_SHIFT));
    }

#else /* ACCEL_CP */
    if (IS_R300_3D || IS_R500_3D)
	BEGIN_ACCEL(2 + vtx_count * 4);
# ifdef IS_RADEON_DRIVER
    else if (info->ChipFamily < CHIP_FAMILY_R200)
	BEGIN_ACCEL(1 + vtx_count * 3);
# endif /* IS_RADEON_DRIVER */
    else
	BEGIN_ACCEL(1 + vtx_count * 4);

# ifdef IS_RADEON_DRIVER
    if (info->ChipFamily < CHIP_FAMILY_R200)
	OUT_ACCEL_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_RECANGLE_LIST |
					  RADEON_VF_PRIM_WALK_DATA |
					  RADEON_VF_RADEON_MODE |
					  (3 << RADEON_VF_NUM_VERTICES_SHIFT)));
    else
# endif /* IS_RADEON_DRIVER */
	OUT_ACCEL_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_QUAD_LIST |
					  RADEON_VF_PRIM_WALK_DATA |
					  (4 << RADEON_VF_NUM_VERTICES_SHIFT)));
#endif /* ACCEL_CP */

    if (accel_state->has_mask) {

# ifdef IS_RADEON_DRIVER
	if (info->ChipFamily >= CHIP_FAMILY_R200)
# endif /* IS_RADEON_DRIVER */
	    VTX_OUT_MASK((float)dstX,                                      (float)dstY,
			 xFixedToFloat(srcTopLeft.x) / accel_state->texW[0],      xFixedToFloat(srcTopLeft.y) / accel_state->texH[0],
		xFixedToFloat(maskTopLeft.x) / accel_state->texW[1],     xFixedToFloat(maskTopLeft.y) / accel_state->texH[1]);
	VTX_OUT_MASK((float)dstX,                                      (float)(dstY + h),
		xFixedToFloat(srcBottomLeft.x) / accel_state->texW[0],   xFixedToFloat(srcBottomLeft.y) / accel_state->texH[0],
		xFixedToFloat(maskBottomLeft.x) / accel_state->texW[1],  xFixedToFloat(maskBottomLeft.y) / accel_state->texH[1]);
	VTX_OUT_MASK((float)(dstX + w),                                (float)(dstY + h),
		xFixedToFloat(srcBottomRight.x) / accel_state->texW[0],  xFixedToFloat(srcBottomRight.y) / accel_state->texH[0],
		xFixedToFloat(maskBottomRight.x) / accel_state->texW[1], xFixedToFloat(maskBottomRight.y) / accel_state->texH[1]);
	VTX_OUT_MASK((float)(dstX + w),                                (float)dstY,
		xFixedToFloat(srcTopRight.x) / accel_state->texW[0],     xFixedToFloat(srcTopRight.y) / accel_state->texH[0],
		xFixedToFloat(maskTopRight.x) / accel_state->texW[1],    xFixedToFloat(maskTopRight.y) / accel_state->texH[1]);
    } else {

# ifdef IS_RADEON_DRIVER
	if (info->ChipFamily >= CHIP_FAMILY_R200)
# endif /* IS_RADEON_DRIVER */
	    VTX_OUT((float)dstX,                                      (float)dstY,
		    xFixedToFloat(srcTopLeft.x) / accel_state->texW[0],      xFixedToFloat(srcTopLeft.y) / accel_state->texH[0]);
	VTX_OUT((float)dstX,                                      (float)(dstY + h),
		xFixedToFloat(srcBottomLeft.x) / accel_state->texW[0],   xFixedToFloat(srcBottomLeft.y) / accel_state->texH[0]);
	VTX_OUT((float)(dstX + w),                                (float)(dstY + h),
		xFixedToFloat(srcBottomRight.x) / accel_state->texW[0],  xFixedToFloat(srcBottomRight.y) / accel_state->texH[0]);
	VTX_OUT((float)(dstX + w),                                (float)dstY,
		xFixedToFloat(srcTopRight.x) / accel_state->texW[0],     xFixedToFloat(srcTopRight.y) / accel_state->texH[0]);
    }

    if (IS_R300_3D || IS_R500_3D)
	/* flushing is pipelined, free/finish is not */
	OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D);

#ifdef ACCEL_CP
    ADVANCE_RING();
#else
    FINISH_ACCEL();
#endif /* !ACCEL_CP */

    LEAVE_DRAW(0);
}
#undef VTX_OUT
#undef VTX_OUT_MASK

static void FUNC_NAME(RadeonComposite)(PixmapPtr pDst,
				       int srcX, int srcY,
				       int maskX, int maskY,
				       int dstX, int dstY,
				       int width, int height)
{
    VAR_PREAMBLE(pDst->drawable.pScreen);
    THREEDSTATE_PREAMBLE();
    int tileSrcY, tileMaskY, tileDstY;
    int remainingHeight;

    if (!accel_state->need_src_tile_x && !accel_state->need_src_tile_y) {
	FUNC_NAME(RadeonCompositeTile)(pDst,
				       srcX, srcY,
				       maskX, maskY,
				       dstX, dstY,
				       width, height);
	return;
    }

    /* Tiling logic borrowed from exaFillRegionTiled */

    modulus(srcY, accel_state->src_tile_height, tileSrcY);
    tileMaskY = maskY;
    tileDstY = dstY;

    remainingHeight = height;
    while (remainingHeight > 0) {
	int remainingWidth = width;
	int tileSrcX, tileMaskX, tileDstX;
	int h = accel_state->src_tile_height - tileSrcY;

	if (h > remainingHeight)
	    h = remainingHeight;
	remainingHeight -= h;

	modulus(srcX, accel_state->src_tile_width, tileSrcX);
	tileMaskX = maskX;
	tileDstX = dstX;

	while (remainingWidth > 0) {
	    int w = accel_state->src_tile_width - tileSrcX;
	    if (w > remainingWidth)
		w = remainingWidth;
	    remainingWidth -= w;

	    FUNC_NAME(RadeonCompositeTile)(pDst,
					   tileSrcX, tileSrcY,
					   tileMaskX, tileMaskY,
					   tileDstX, tileDstY,
					   w, h);

	    tileSrcX = 0;
	    tileMaskX += w;
	    tileDstX += w;
	}
	tileSrcY = 0;
	tileMaskY += h;
	tileDstY += h;
    }
}

static void FUNC_NAME(RadeonDoneComposite)(PixmapPtr pDst)
{
    VAR_PREAMBLE(pDst->drawable.pScreen);
    ACCEL_PREAMBLE();

    ENTER_DRAW(0);

    if (IS_R300_3D || IS_R500_3D) {
	BEGIN_ACCEL(2);
	OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_RB3D_DC_FLUSH_ALL);
    } else
	BEGIN_ACCEL(1);
    OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);
    FINISH_ACCEL();

#if defined(ACCEL_CP) && !defined(IS_RADEON_DRIVER) && !defined(IS_QUICK_AND_DIRTY)
    ADVANCE_RING();
#endif

    LEAVE_DRAW(0);
}

#if !defined(IS_RADEON_DRIVER) && !defined(IS_QUICK_AND_DIRTY)
void
R5xxExaCompositeFuncs(int scrnIndex, struct _ExaDriver *Exa)
{

    xf86DrvMsg(scrnIndex, X_INFO, "Attaching EXA Composite hooks for R5xx.\n");

    Exa->CheckComposite = R300CheckComposite;
    Exa->PrepareComposite = FUNC_NAME(R300PrepareComposite);
    Exa->Composite = FUNC_NAME(RadeonComposite);
    Exa->DoneComposite = FUNC_NAME(RadeonDoneComposite);
}
#endif /* IS_RADEON_DRIVER */

#undef ONLY_ONCE
#undef FUNC_NAME
