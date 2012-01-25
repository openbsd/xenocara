/*
 * Copyright 2004 Eric Anholt
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
 *    Hui Yu <hyu@ati.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#ifdef USE_XAA

#include "dixstruct.h"

#include "xaa.h"
#include "xaalocal.h"

#ifndef RENDER_GENERIC_HELPER
#define RENDER_GENERIC_HELPER

struct blendinfo {
	Bool dst_alpha;
	Bool src_alpha;
	uint32_t blend_cntl;
};

/* The first part of blend_cntl corresponds to Fa from the render "protocol"
 * document, and the second part to Fb.
 */
static const struct blendinfo RadeonBlendOp[] = {
    /* Clear */
    {0, 0, RADEON_SRC_BLEND_GL_ZERO |
	   RADEON_DST_BLEND_GL_ZERO},
    /* Src */
    {0, 0, RADEON_SRC_BLEND_GL_ONE |
	   RADEON_DST_BLEND_GL_ZERO},
    /* Dst */
    {0, 0, RADEON_SRC_BLEND_GL_ZERO |
	   RADEON_DST_BLEND_GL_ONE},
    /* Over */
    {0, 1, RADEON_SRC_BLEND_GL_ONE |
	   RADEON_DST_BLEND_GL_ONE_MINUS_SRC_ALPHA},
    /* OverReverse */
    {1, 0, RADEON_SRC_BLEND_GL_ONE_MINUS_DST_ALPHA |
	   RADEON_DST_BLEND_GL_ONE},
    /* In */
    {1, 0, RADEON_SRC_BLEND_GL_DST_ALPHA |
	   RADEON_DST_BLEND_GL_ZERO},
    /* InReverse */
    {0, 1, RADEON_SRC_BLEND_GL_ZERO |
	   RADEON_DST_BLEND_GL_SRC_ALPHA},
    /* Out */
    {1, 0, RADEON_SRC_BLEND_GL_ONE_MINUS_DST_ALPHA |
	   RADEON_DST_BLEND_GL_ZERO},
    /* OutReverse */
    {0, 1, RADEON_SRC_BLEND_GL_ZERO |
	   RADEON_DST_BLEND_GL_ONE_MINUS_SRC_ALPHA},
    /* Atop */
    {1, 1, RADEON_SRC_BLEND_GL_DST_ALPHA |
	   RADEON_DST_BLEND_GL_ONE_MINUS_SRC_ALPHA},
    /* AtopReverse */
    {1, 1, RADEON_SRC_BLEND_GL_ONE_MINUS_DST_ALPHA |
	   RADEON_DST_BLEND_GL_SRC_ALPHA},
    /* Xor */
    {1, 1, RADEON_SRC_BLEND_GL_ONE_MINUS_DST_ALPHA |
	   RADEON_DST_BLEND_GL_ONE_MINUS_SRC_ALPHA},
    /* Add */
    {0, 0, RADEON_SRC_BLEND_GL_ONE |
	   RADEON_DST_BLEND_GL_ONE},
    /* Saturate */
    {1, 1, RADEON_SRC_BLEND_GL_SRC_ALPHA_SATURATE |
	   RADEON_DST_BLEND_GL_ONE},
    {0, 0, 0},
    {0, 0, 0},
    /* DisjointClear */
    {0, 0, RADEON_SRC_BLEND_GL_ZERO |
	   RADEON_DST_BLEND_GL_ZERO},
    /* DisjointSrc */
    {0, 0, RADEON_SRC_BLEND_GL_ONE |
	   RADEON_DST_BLEND_GL_ZERO},
    /* DisjointDst */
    {0, 0, RADEON_SRC_BLEND_GL_ZERO |
	   RADEON_DST_BLEND_GL_ONE},
    /* DisjointOver unsupported */
    {0, 0, 0},
    /* DisjointOverReverse */
    {1, 1, RADEON_SRC_BLEND_GL_SRC_ALPHA_SATURATE |
	   RADEON_DST_BLEND_GL_ONE},
    /* DisjointIn unsupported */
    {0, 0, 0},
    /* DisjointInReverse unsupported */
    {0, 0, 0},
    /* DisjointOut unsupported */
    {1, 1, RADEON_SRC_BLEND_GL_SRC_ALPHA_SATURATE |
	   RADEON_DST_BLEND_GL_ZERO},
    /* DisjointOutReverse unsupported */
    {0, 0, 0},
    /* DisjointAtop unsupported */
    {0, 0, 0},
    /* DisjointAtopReverse unsupported */
    {0, 0, 0},
    /* DisjointXor unsupported */
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    /* ConjointClear */
    {0, 0, RADEON_SRC_BLEND_GL_ZERO |
	   RADEON_DST_BLEND_GL_ZERO},
    /* ConjointSrc */
    {0, 0, RADEON_SRC_BLEND_GL_ONE |
	   RADEON_DST_BLEND_GL_ZERO},
    /* ConjointDst */
    {0, 0, RADEON_SRC_BLEND_GL_ZERO |
	   RADEON_DST_BLEND_GL_ONE},
};
#define RadeonOpMax (sizeof(RadeonBlendOp) / sizeof(RadeonBlendOp[0]))

/* Note on texture formats:
 * TXFORMAT_Y8 expands to (Y,Y,Y,1).  TXFORMAT_I8 expands to (I,I,I,I)
 * The RADEON and R200 TXFORMATS we use are the same on r100/r200.
 */

static CARD32 RADEONTextureFormats[] = {
    PICT_a8r8g8b8,
    PICT_a8,
    PICT_x8r8g8b8,
    PICT_r5g6b5,
    PICT_a1r5g5b5,
    PICT_x1r5g5b5,
    0
};

static CARD32 RADEONDstFormats[] = {
    PICT_a8r8g8b8,
    PICT_x8r8g8b8,
    PICT_r5g6b5,
    PICT_a1r5g5b5,
    PICT_x1r5g5b5,
    0
};

static uint32_t
RadeonGetTextureFormat(uint32_t format)
{
    switch (format) {
    case PICT_a8r8g8b8:
	return RADEON_TXFORMAT_ARGB8888 | RADEON_TXFORMAT_ALPHA_IN_MAP;
    case PICT_a8:
	return RADEON_TXFORMAT_I8 | RADEON_TXFORMAT_ALPHA_IN_MAP;
    case PICT_x8r8g8b8:
	return RADEON_TXFORMAT_ARGB8888;
    case PICT_r5g6b5:
	return RADEON_TXFORMAT_RGB565;
    case PICT_a1r5g5b5:
	return RADEON_TXFORMAT_ARGB1555 | RADEON_TXFORMAT_ALPHA_IN_MAP;
    case PICT_x1r5g5b5:
	return RADEON_TXFORMAT_ARGB1555;
    default:
	return 0;
    }
}

static uint32_t
RadeonGetColorFormat(uint32_t format)
{
    switch (format) {
    case PICT_a8r8g8b8:
    case PICT_x8r8g8b8:
	return RADEON_COLOR_FORMAT_ARGB8888;
    case PICT_r5g6b5:
	return RADEON_COLOR_FORMAT_RGB565;
    case PICT_a1r5g5b5:
    case PICT_x1r5g5b5:
	return RADEON_COLOR_FORMAT_ARGB1555;
    default:
	return 0;
    }
}

/* Returns a RADEON_RB3D_BLENDCNTL value, or 0 if the operation is not
 * supported
 */
static uint32_t
RadeonGetBlendCntl(uint8_t op, uint32_t dstFormat)
{
    uint32_t blend_cntl;

    if (op >= RadeonOpMax || RadeonBlendOp[op].blend_cntl == 0)
	return 0;

    blend_cntl = RadeonBlendOp[op].blend_cntl;
	
    if (RadeonBlendOp[op].dst_alpha && !PICT_FORMAT_A(dstFormat)) {
	uint32_t srcblend = blend_cntl & RADEON_SRC_BLEND_MASK;

	/* If there's no destination alpha channel, we need to wire the blending
	 * to treat the alpha channel as always 1.
	 */
	if (srcblend == RADEON_SRC_BLEND_GL_ONE_MINUS_DST_ALPHA ||
	    srcblend == RADEON_SRC_BLEND_GL_SRC_ALPHA_SATURATE)
	    blend_cntl = (blend_cntl & ~RADEON_SRC_BLEND_MASK) |
			 RADEON_SRC_BLEND_GL_ZERO;
	else if (srcblend == RADEON_SRC_BLEND_GL_DST_ALPHA)
	    blend_cntl = (blend_cntl & ~RADEON_SRC_BLEND_MASK) |
			 RADEON_SRC_BLEND_GL_ONE;
    }

    return blend_cntl;
}

static __inline__ uint32_t F_TO_DW(float val)
{
    union {
	float f;
	uint32_t l;
    } tmp;
    tmp.f = val;
    return tmp.l;
}

/* Compute log base 2 of val. */
static __inline__ int
ATILog2(int val)
{
	int bits;
#if (defined __i386__ || defined __x86_64__) && (defined __GNUC__)
	__asm volatile("bsrl	%1, %0"
		: "=r" (bits)
		: "c" (val)
	);
	return bits;
#else
	for (bits = 0; val != 0; val >>= 1, ++bits)
		;
	return bits - 1;
#endif
}

static void
RemoveLinear (FBLinearPtr linear)
{
   RADEONInfoPtr info = (RADEONInfoPtr)(linear->devPrivate.ptr);

   info->accel_state->RenderTex = NULL; 
}

static void
RenderCallback (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);

    if ((currentTime.milliseconds > info->accel_state->RenderTimeout) &&
	info->accel_state->RenderTex) {
	xf86FreeOffscreenLinear(info->accel_state->RenderTex);
	info->accel_state->RenderTex = NULL;
    }

    if (!info->accel_state->RenderTex)
	info->accel_state->RenderCallback = NULL;
}

static Bool
AllocateLinear (
   ScrnInfoPtr pScrn,
   int sizeNeeded
){
   RADEONInfoPtr  info       = RADEONPTR(pScrn);
   int cpp = info->CurrentLayout.bitsPerPixel / 8;

   info->accel_state->RenderTimeout = currentTime.milliseconds + 30000;
   info->accel_state->RenderCallback = RenderCallback;

   /* XAA allocates in units of pixels at the screen bpp, so adjust size
    * appropriately.
    */
   sizeNeeded = (sizeNeeded + cpp - 1) / cpp;

   if (info->accel_state->RenderTex) {
	if (info->accel_state->RenderTex->size >= sizeNeeded)
	   return TRUE;
	else {
	   if (xf86ResizeOffscreenLinear(info->accel_state->RenderTex, sizeNeeded))
		return TRUE;

	   xf86FreeOffscreenLinear(info->accel_state->RenderTex);
	   info->accel_state->RenderTex = NULL;
	}
   }

   info->accel_state->RenderTex = xf86AllocateOffscreenLinear(pScrn->pScreen, sizeNeeded, 32,
							      NULL, RemoveLinear, info);

   return (info->accel_state->RenderTex != NULL);
}

#if X_BYTE_ORDER == X_BIG_ENDIAN
static Bool RADEONSetupRenderByteswap(ScrnInfoPtr pScrn, int tex_bytepp)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t swapper = info->ModeReg->surface_cntl;

    swapper &= ~(RADEON_NONSURF_AP0_SWP_16BPP | RADEON_NONSURF_AP1_SWP_16BPP |
		 RADEON_NONSURF_AP0_SWP_32BPP | RADEON_NONSURF_AP1_SWP_32BPP);

    /* Set up byte swapping for the framebuffer aperture as needed */
    switch (tex_bytepp) {
    case 1:
	break;
    case 2:
	swapper |= RADEON_NONSURF_AP0_SWP_16BPP | RADEON_NONSURF_AP1_SWP_16BPP;
	break;
    case 4:
	swapper |= RADEON_NONSURF_AP0_SWP_32BPP | RADEON_NONSURF_AP1_SWP_32BPP;
	break;
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "%s: Don't know what to do for "
		   "tex_bytepp == %d!\n", __func__, tex_bytepp);
	return FALSE;
    }
    OUTREG(RADEON_SURFACE_CNTL, swapper);
    return TRUE;
}

static void RADEONRestoreByteswap(RADEONInfoPtr info)
{
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG(RADEON_SURFACE_CNTL, info->ModeReg->surface_cntl);
}
#endif	/* X_BYTE_ORDER == X_BIG_ENDIAN */

#endif	/* RENDER_GENERIC_HELPER */

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

static Bool FUNC_NAME(R100SetupTexture)(
	ScrnInfoPtr pScrn,
	uint32_t format,
	uint8_t *src,
	int src_pitch,
	unsigned int width,
	unsigned int height,
	int flags)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint8_t *dst;
    uint32_t tex_size = 0, txformat;
    int dst_pitch, offset, size, tex_bytepp;
#ifdef ACCEL_CP
    uint32_t buf_pitch, dst_pitch_off;
    int x, y;
    unsigned int hpass;
    uint8_t *tmp_dst;
#endif
    ACCEL_PREAMBLE();

    if ((width > 2047) || (height > 2047))
	return FALSE;

    txformat = RadeonGetTextureFormat(format);
    tex_bytepp = PICT_FORMAT_BPP(format) >> 3;

    dst_pitch = RADEON_ALIGN(width * tex_bytepp, 64);
    size = dst_pitch * height;

    if ((flags & XAA_RENDER_REPEAT) && (height != 1) &&
	(RADEON_ALIGN(width * tex_bytepp, 32) != dst_pitch))
	return FALSE;

#ifndef ACCEL_CP

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (!RADEONSetupRenderByteswap(pScrn, tex_bytepp)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "%s: RADEONSetupRenderByteswap() "
		   "failed!\n", __func__);
	return FALSE;
    }
#endif

#endif

    if (!AllocateLinear(pScrn, size))
	return FALSE;

    if (flags & XAA_RENDER_REPEAT) {
	txformat |= ATILog2(width) << RADEON_TXFORMAT_WIDTH_SHIFT;
	txformat |= ATILog2(height) << RADEON_TXFORMAT_HEIGHT_SHIFT;
    } else {
	tex_size = (height << 16) | width;
	txformat |= RADEON_TXFORMAT_NON_POWER2;
    }

    offset = info->accel_state->RenderTex->offset * pScrn->bitsPerPixel / 8;
    dst = (uint8_t*)(info->FB + offset);

    /* Upload texture to card. */

#ifdef ACCEL_CP

    RADEONHostDataParams( pScrn, dst, dst_pitch, tex_bytepp, &dst_pitch_off, &x, &y );

    while ( height )
    {
    	tmp_dst = RADEONHostDataBlit( pScrn, tex_bytepp, width,
				      dst_pitch_off, &buf_pitch,
				      x, &y, &height, &hpass );
	RADEONHostDataBlitCopyPass( pScrn, tex_bytepp, tmp_dst, src,
				    hpass, buf_pitch, src_pitch );
	src += hpass * src_pitch;
    }

    RADEON_PURGE_CACHE();
    RADEON_WAIT_UNTIL_IDLE();

#else

    if (info->accel_state->accel->NeedToSync)
	info->accel_state->accel->Sync(pScrn);

    while (height--) {
	memcpy(dst, src, width * tex_bytepp);
	src += src_pitch;
	dst += dst_pitch;
    }

#if X_BYTE_ORDER == X_BIG_ENDIAN
    RADEONRestoreByteswap(info);
#endif

#endif	/* ACCEL_CP */

    BEGIN_ACCEL(5);
    OUT_ACCEL_REG(RADEON_PP_TXFORMAT_0, txformat);
    OUT_ACCEL_REG(RADEON_PP_TEX_SIZE_0, tex_size);
    OUT_ACCEL_REG(RADEON_PP_TEX_PITCH_0, dst_pitch - 32);
    OUT_ACCEL_REG(RADEON_PP_TXOFFSET_0, offset + info->fbLocation +
					pScrn->fbOffset);
    OUT_ACCEL_REG(RADEON_PP_TXFILTER_0, RADEON_MAG_FILTER_LINEAR |
					RADEON_MIN_FILTER_LINEAR |
					RADEON_CLAMP_S_WRAP |
					RADEON_CLAMP_T_WRAP);
    FINISH_ACCEL();

    return TRUE;
}

static Bool
FUNC_NAME(R100SetupForCPUToScreenAlphaTexture) (
	ScrnInfoPtr	pScrn,
	int		op,
	CARD16		red,
	CARD16		green,
	CARD16		blue,
	CARD16		alpha,
	CARD32		maskFormat,
	CARD32		dstFormat,
	CARD8		*alphaPtr,
	int		alphaPitch,
	int		width,
	int		height,
	int		flags
) 
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t colorformat, srccolor, blend_cntl;
    ACCEL_PREAMBLE();

    blend_cntl = RadeonGetBlendCntl(op, dstFormat);
    if (blend_cntl == 0)
	return FALSE;

    if (!info->accel_state->XInited3D)
	RADEONInit3DEngine(pScrn);

    if (!FUNC_NAME(R100SetupTexture)(pScrn, maskFormat, alphaPtr, alphaPitch,
				     width, height, flags))
	return FALSE;

    colorformat = RadeonGetColorFormat(dstFormat);

    srccolor = ((alpha & 0xff00) << 16) | ((red & 0xff00) << 8) | (blue >> 8) |
	(green & 0xff00);

    BEGIN_ACCEL(7);
    OUT_ACCEL_REG(RADEON_RB3D_CNTL, colorformat | RADEON_ALPHA_BLEND_ENABLE);
    OUT_ACCEL_REG(RADEON_PP_CNTL, RADEON_TEX_0_ENABLE |
				  RADEON_TEX_BLEND_0_ENABLE);
    OUT_ACCEL_REG(RADEON_PP_TFACTOR_0, srccolor);
    OUT_ACCEL_REG(RADEON_PP_TXCBLEND_0, RADEON_COLOR_ARG_A_TFACTOR_COLOR |
					RADEON_COLOR_ARG_B_T0_ALPHA);
    OUT_ACCEL_REG(RADEON_PP_TXABLEND_0, RADEON_ALPHA_ARG_A_TFACTOR_ALPHA |
					RADEON_ALPHA_ARG_B_T0_ALPHA);
    OUT_ACCEL_REG(RADEON_SE_VTX_FMT, RADEON_SE_VTX_FMT_XY |
				     RADEON_SE_VTX_FMT_ST0);
    OUT_ACCEL_REG(RADEON_RB3D_BLENDCNTL, blend_cntl);
    FINISH_ACCEL();

    return TRUE;
}


static Bool
FUNC_NAME(R100SetupForCPUToScreenTexture) (
	ScrnInfoPtr	pScrn,
	int		op,
	CARD32		srcFormat,
	CARD32		dstFormat,
	CARD8		*texPtr,
	int		texPitch,
	int		width,
	int		height,
	int		flags
)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t colorformat, blend_cntl;
    ACCEL_PREAMBLE();

    blend_cntl = RadeonGetBlendCntl(op, dstFormat);
    if (blend_cntl == 0)
	return FALSE;
    
    if (!info->accel_state->XInited3D)
	RADEONInit3DEngine(pScrn);

    if (!FUNC_NAME(R100SetupTexture)(pScrn, srcFormat, texPtr, texPitch, width,
				     height, flags))
	return FALSE;

    colorformat = RadeonGetColorFormat(dstFormat);
    
    BEGIN_ACCEL(6);
    OUT_ACCEL_REG(RADEON_RB3D_CNTL, colorformat | RADEON_ALPHA_BLEND_ENABLE);
    OUT_ACCEL_REG(RADEON_PP_CNTL, RADEON_TEX_0_ENABLE |
				  RADEON_TEX_BLEND_0_ENABLE);
    if (srcFormat != PICT_a8)
	OUT_ACCEL_REG(RADEON_PP_TXCBLEND_0, RADEON_COLOR_ARG_C_T0_COLOR);
    else
	OUT_ACCEL_REG(RADEON_PP_TXCBLEND_0, RADEON_COLOR_ARG_C_ZERO);
    OUT_ACCEL_REG(RADEON_PP_TXABLEND_0, RADEON_ALPHA_ARG_C_T0_ALPHA);
    OUT_ACCEL_REG(RADEON_SE_VTX_FMT, RADEON_SE_VTX_FMT_XY |
				     RADEON_SE_VTX_FMT_ST0);
    OUT_ACCEL_REG(RADEON_RB3D_BLENDCNTL, blend_cntl);
    FINISH_ACCEL();

    return TRUE;
}


static void
FUNC_NAME(R100SubsequentCPUToScreenTexture) (
	ScrnInfoPtr	pScrn,
	int		dstx,
	int		dsty,
	int		srcx,
	int		srcy,
	int		width,
	int		height
)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    int byteshift;
    uint32_t fboffset;
    float l, t, r, b, fl, fr, ft, fb;

    ACCEL_PREAMBLE();

    /* Note: we can't simply set up the 3D surface at the same location as the
     * front buffer, because the 2048x2048 limit on coordinates may be smaller
     * than the (MergedFB) screen.
     * Can't use arbitrary offsets for color tiling
     */ 
    if (info->tilingEnabled) {
       /* can't play tricks with x coordinate, or could we - tiling is disabled anyway in that case */
       fboffset = info->fbLocation + pScrn->fbOffset +
          (pScrn->displayWidth * (dsty & ~15) * (pScrn->bitsPerPixel >> 3));
       l = dstx;
       t = (dsty % 16);
    }
    else {
       byteshift = (pScrn->bitsPerPixel >> 4);
       fboffset = (info->fbLocation + pScrn->fbOffset +
		((pScrn->displayWidth * dsty + dstx) << byteshift)) & ~15;
       l = ((dstx << byteshift) % 16) >> byteshift;
       t = 0.0;
    }

    r = width + l;
    b = height + t;
    fl = srcx;
    fr = srcx + width;
    ft = srcy;
    fb = srcy + height;

#ifdef ACCEL_CP
    BEGIN_RING(25);

    OUT_ACCEL_REG(RADEON_RB3D_COLORPITCH, pScrn->displayWidth |
	((info->tilingEnabled && (dsty <= pScrn->virtualY)) ? RADEON_COLOR_TILE_ENABLE : 0));
    OUT_ACCEL_REG(RADEON_RB3D_COLOROFFSET, fboffset);
    OUT_RING(CP_PACKET3(RADEON_CP_PACKET3_3D_DRAW_IMMD, 17));
    /* RADEON_SE_VTX_FMT */
    OUT_RING(RADEON_CP_VC_FRMT_XY |
	     RADEON_CP_VC_FRMT_ST0);
    /* SE_VF_CNTL */
    OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_TRI_FAN |
	     RADEON_CP_VC_CNTL_PRIM_WALK_RING |
	     RADEON_CP_VC_CNTL_MAOS_ENABLE |
	     RADEON_CP_VC_CNTL_VTX_FMT_RADEON_MODE |
	     (4 << RADEON_CP_VC_CNTL_NUM_SHIFT));

    OUT_RING(F_TO_DW(l));
    OUT_RING(F_TO_DW(t));
    OUT_RING(F_TO_DW(fl));
    OUT_RING(F_TO_DW(ft));

    OUT_RING(F_TO_DW(r));
    OUT_RING(F_TO_DW(t));
    OUT_RING(F_TO_DW(fr));
    OUT_RING(F_TO_DW(ft));

    OUT_RING(F_TO_DW(r));
    OUT_RING(F_TO_DW(b));
    OUT_RING(F_TO_DW(fr));
    OUT_RING(F_TO_DW(fb));

    OUT_RING(F_TO_DW(l));
    OUT_RING(F_TO_DW(b));
    OUT_RING(F_TO_DW(fl));
    OUT_RING(F_TO_DW(fb));

    OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);

    ADVANCE_RING();
#else
    BEGIN_ACCEL(20);
    
    OUT_ACCEL_REG(RADEON_RB3D_COLORPITCH, pScrn->displayWidth |
	((info->tilingEnabled && (dsty <= pScrn->virtualY)) ? RADEON_COLOR_TILE_ENABLE : 0));
    OUT_ACCEL_REG(RADEON_RB3D_COLOROFFSET, fboffset);

    OUT_ACCEL_REG(RADEON_SE_VF_CNTL, RADEON_VF_PRIM_TYPE_TRIANGLE_FAN |
				     RADEON_VF_PRIM_WALK_DATA |
				     RADEON_VF_RADEON_MODE |
				     (4 << RADEON_VF_NUM_VERTICES_SHIFT));
	
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(l));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(t));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fl));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(ft));

    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(r));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(t));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fr));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(ft));

    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(r));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(b));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fr));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fb));

    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(l));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(b));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fl));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fb));

    OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);
    FINISH_ACCEL();
#endif

}

static Bool FUNC_NAME(R200SetupTexture)(
	ScrnInfoPtr pScrn,
	uint32_t format,
	uint8_t *src,
	int src_pitch,
	unsigned int width,
	unsigned int height,
	int flags)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint8_t *dst;
    uint32_t tex_size = 0, txformat;
    int dst_pitch, offset, size, tex_bytepp;
#ifdef ACCEL_CP
    uint32_t buf_pitch, dst_pitch_off;
    int x, y;
    unsigned int hpass;
    uint8_t *tmp_dst;
#endif
    ACCEL_PREAMBLE();

    if ((width > 2048) || (height > 2048))
	return FALSE;

    txformat = RadeonGetTextureFormat(format);
    tex_bytepp = PICT_FORMAT_BPP(format) >> 3;

    dst_pitch = RADEON_ALIGN(width * tex_bytepp, 64);
    size = dst_pitch * height;

    if ((flags & XAA_RENDER_REPEAT) && (height != 1) &&
	(RADEON_ALIGN(width * tex_bytepp, 32) != dst_pitch))
	return FALSE;

#ifndef ACCEL_CP

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (!RADEONSetupRenderByteswap(pScrn, tex_bytepp)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "%s: RADEONSetupRenderByteswap() "
		   "failed!\n", __func__);
	return FALSE;
    }
#endif

#endif

    if (!AllocateLinear(pScrn, size))
	return FALSE;

    if (flags & XAA_RENDER_REPEAT) {
	txformat |= ATILog2(width) << R200_TXFORMAT_WIDTH_SHIFT;
	txformat |= ATILog2(height) << R200_TXFORMAT_HEIGHT_SHIFT;
    } else {
	tex_size = ((height - 1) << 16) | (width - 1);
	txformat |= RADEON_TXFORMAT_NON_POWER2;
    }

    info->accel_state->texW[0] = width;
    info->accel_state->texH[0] = height;

    offset = info->accel_state->RenderTex->offset * pScrn->bitsPerPixel / 8;
    dst = (uint8_t*)(info->FB + offset);

    /* Upload texture to card. */

#ifdef ACCEL_CP

    RADEONHostDataParams( pScrn, dst, dst_pitch, tex_bytepp, &dst_pitch_off, &x, &y );

    while ( height )
    {
        tmp_dst = RADEONHostDataBlit( pScrn, tex_bytepp, width,
				      dst_pitch_off, &buf_pitch,
				      x, &y, &height, &hpass );
	RADEONHostDataBlitCopyPass( pScrn, tex_bytepp, tmp_dst, src,
				    hpass, buf_pitch, src_pitch );
	src += hpass * src_pitch;
    }

    RADEON_PURGE_CACHE();
    RADEON_WAIT_UNTIL_IDLE();

#else

    if (info->accel_state->accel->NeedToSync)
	info->accel_state->accel->Sync(pScrn);

    while (height--) {
	memcpy(dst, src, width * tex_bytepp);
	src += src_pitch;
	dst += dst_pitch;
    }

#if X_BYTE_ORDER == X_BIG_ENDIAN
    RADEONRestoreByteswap(info);
#endif

#endif	/* ACCEL_CP */

    BEGIN_ACCEL(6);
    OUT_ACCEL_REG(R200_PP_TXFORMAT_0, txformat);
    OUT_ACCEL_REG(R200_PP_TXFORMAT_X_0, 0);
    OUT_ACCEL_REG(R200_PP_TXSIZE_0, tex_size);
    OUT_ACCEL_REG(R200_PP_TXPITCH_0, dst_pitch - 32);
    OUT_ACCEL_REG(R200_PP_TXOFFSET_0, offset + info->fbLocation +
				      pScrn->fbOffset);
    OUT_ACCEL_REG(R200_PP_TXFILTER_0, R200_MAG_FILTER_NEAREST |
				      R200_MIN_FILTER_NEAREST |
				      R200_CLAMP_S_WRAP |
				      R200_CLAMP_T_WRAP);
    FINISH_ACCEL();

    return TRUE;
}

static Bool
FUNC_NAME(R200SetupForCPUToScreenAlphaTexture) (
	ScrnInfoPtr	pScrn,
	int		op,
	CARD16		red,
	CARD16		green,
	CARD16		blue,
	CARD16		alpha,
	CARD32		maskFormat,
	CARD32		dstFormat,
	CARD8		*alphaPtr,
	int		alphaPitch,
	int		width,
	int		height,
	int		flags
) 
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    uint32_t colorformat, srccolor, blend_cntl;
    ACCEL_PREAMBLE();

    blend_cntl = RadeonGetBlendCntl(op, dstFormat);
    if (blend_cntl == 0)
	return FALSE;

    if (!info->accel_state->XInited3D)
	RADEONInit3DEngine(pScrn);

    if (!FUNC_NAME(R200SetupTexture)(pScrn, maskFormat, alphaPtr, alphaPitch,
				     width, height, flags))
	return FALSE;

    colorformat = RadeonGetColorFormat(dstFormat);

    srccolor = ((alpha & 0xff00) << 16) | ((red & 0xff00) << 8) | (blue >> 8) |
	(green & 0xff00);

    BEGIN_ACCEL(10);
    OUT_ACCEL_REG(RADEON_RB3D_CNTL, colorformat | RADEON_ALPHA_BLEND_ENABLE);
    OUT_ACCEL_REG(RADEON_PP_CNTL, RADEON_TEX_0_ENABLE |
				  RADEON_TEX_BLEND_0_ENABLE);
    OUT_ACCEL_REG(R200_PP_TFACTOR_0, srccolor);
    OUT_ACCEL_REG(R200_PP_TXCBLEND_0, R200_TXC_ARG_A_TFACTOR_COLOR |
				      R200_TXC_ARG_B_R0_ALPHA);
    OUT_ACCEL_REG(R200_PP_TXCBLEND2_0, R200_TXC_OUTPUT_REG_R0);
    OUT_ACCEL_REG(R200_PP_TXABLEND_0, R200_TXA_ARG_A_TFACTOR_ALPHA |
				      R200_TXA_ARG_B_R0_ALPHA);
    OUT_ACCEL_REG(R200_PP_TXABLEND2_0, R200_TXA_OUTPUT_REG_R0);
    OUT_ACCEL_REG(R200_SE_VTX_FMT_0, 0);
    OUT_ACCEL_REG(R200_SE_VTX_FMT_1, (2 << R200_VTX_TEX0_COMP_CNT_SHIFT));
    OUT_ACCEL_REG(RADEON_RB3D_BLENDCNTL, blend_cntl);
    FINISH_ACCEL();

    return TRUE;
}

static Bool
FUNC_NAME(R200SetupForCPUToScreenTexture) (
	ScrnInfoPtr	pScrn,
	int		op,
	CARD32		srcFormat,
	CARD32		dstFormat,
	CARD8		*texPtr,
	int		texPitch,
	int		width,
	int		height,
	int		flags
)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    uint32_t colorformat, blend_cntl;
    ACCEL_PREAMBLE();

    blend_cntl = RadeonGetBlendCntl(op, dstFormat);
    if (blend_cntl == 0)
	return FALSE;

    if (!info->accel_state->XInited3D)
	RADEONInit3DEngine(pScrn);

    if (!FUNC_NAME(R200SetupTexture)(pScrn, srcFormat, texPtr, texPitch, width,
				     height, flags))
	return FALSE;

    colorformat = RadeonGetColorFormat(dstFormat);

    BEGIN_ACCEL(9);
    OUT_ACCEL_REG(RADEON_RB3D_CNTL, colorformat | RADEON_ALPHA_BLEND_ENABLE);
    OUT_ACCEL_REG(RADEON_PP_CNTL, RADEON_TEX_0_ENABLE |
				  RADEON_TEX_BLEND_0_ENABLE);
    if (srcFormat != PICT_a8)
	OUT_ACCEL_REG(R200_PP_TXCBLEND_0, R200_TXC_ARG_C_R0_COLOR);
    else
	OUT_ACCEL_REG(R200_PP_TXCBLEND_0, R200_TXC_ARG_C_ZERO);
    OUT_ACCEL_REG(R200_PP_TXCBLEND2_0, R200_TXC_OUTPUT_REG_R0);
    OUT_ACCEL_REG(R200_PP_TXABLEND_0, R200_TXA_ARG_C_R0_ALPHA);
    OUT_ACCEL_REG(R200_PP_TXABLEND2_0, R200_TXA_OUTPUT_REG_R0);
    OUT_ACCEL_REG(R200_SE_VTX_FMT_0, 0);
    OUT_ACCEL_REG(R200_SE_VTX_FMT_1, (2 << R200_VTX_TEX0_COMP_CNT_SHIFT));
    OUT_ACCEL_REG(RADEON_RB3D_BLENDCNTL, blend_cntl);
    FINISH_ACCEL();

    return TRUE;
}

static void
FUNC_NAME(R200SubsequentCPUToScreenTexture) (
	ScrnInfoPtr	pScrn,
	int		dstx,
	int		dsty,
	int		srcx,
	int		srcy,
	int		width,
	int		height
)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    int byteshift;
    uint32_t fboffset;
    float l, t, r, b, fl, fr, ft, fb;
    ACCEL_PREAMBLE();

    /* Note: we can't simply set up the 3D surface at the same location as the
     * front buffer, because the 2048x2048 limit on coordinates may be smaller
     * than the (MergedFB) screen.
     * Can't use arbitrary offsets for color tiling
     */ 
    if (info->tilingEnabled) {
       /* can't play tricks with x coordinate, or could we - tiling is disabled anyway in that case */
       fboffset = info->fbLocation + pScrn->fbOffset +
          (pScrn->displayWidth * (dsty & ~15) * (pScrn->bitsPerPixel >> 3));
       l = dstx;
       t = (dsty % 16);
    }
    else {
       byteshift = (pScrn->bitsPerPixel >> 4);
       fboffset = (info->fbLocation + pScrn->fbOffset +
		((pScrn->displayWidth * dsty + dstx) << byteshift)) & ~15;
       l = ((dstx << byteshift) % 16) >> byteshift;
       t = 0.0;
    }
    
    r = width + l;
    b = height + t;
    fl = (float)srcx / info->accel_state->texW[0];
    fr = (float)(srcx + width) / info->accel_state->texW[0];
    ft = (float)srcy / info->accel_state->texH[0];
    fb = (float)(srcy + height) / info->accel_state->texH[0];

#ifdef ACCEL_CP
    BEGIN_RING(24);

    OUT_ACCEL_REG(RADEON_RB3D_COLORPITCH, pScrn->displayWidth |
	((info->tilingEnabled && (dsty <= pScrn->virtualY)) ? RADEON_COLOR_TILE_ENABLE : 0));
    OUT_ACCEL_REG(RADEON_RB3D_COLOROFFSET, fboffset);

    OUT_RING(CP_PACKET3(R200_CP_PACKET3_3D_DRAW_IMMD_2, 16));
    /* RADEON_SE_VF_CNTL */
    OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_TRI_FAN |
	     RADEON_CP_VC_CNTL_PRIM_WALK_RING |
	     (4 << RADEON_CP_VC_CNTL_NUM_SHIFT));

    OUT_RING(F_TO_DW(l));
    OUT_RING(F_TO_DW(t));
    OUT_RING(F_TO_DW(fl));
    OUT_RING(F_TO_DW(ft));

    OUT_RING(F_TO_DW(r));
    OUT_RING(F_TO_DW(t));
    OUT_RING(F_TO_DW(fr));
    OUT_RING(F_TO_DW(ft));

    OUT_RING(F_TO_DW(r));
    OUT_RING(F_TO_DW(b));
    OUT_RING(F_TO_DW(fr));
    OUT_RING(F_TO_DW(fb));

    OUT_RING(F_TO_DW(l));
    OUT_RING(F_TO_DW(b));
    OUT_RING(F_TO_DW(fl));
    OUT_RING(F_TO_DW(fb));

    OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);

    ADVANCE_RING();
#else
    BEGIN_ACCEL(20);
    
    /* Note: we can't simply setup 3D surface at the same location as the front buffer,
       some apps may draw offscreen pictures out of the limitation of radeon 3D surface.
    */ 
    OUT_ACCEL_REG(RADEON_RB3D_COLORPITCH, pScrn->displayWidth |
	((info->tilingEnabled && (dsty <= pScrn->virtualY)) ? RADEON_COLOR_TILE_ENABLE : 0));
    OUT_ACCEL_REG(RADEON_RB3D_COLOROFFSET, fboffset);

    OUT_ACCEL_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_QUAD_LIST |
				      RADEON_VF_PRIM_WALK_DATA |
				      4 << RADEON_VF_NUM_VERTICES_SHIFT));
	
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(l));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(t));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fl));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(ft));

    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(r));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(t));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fr));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(ft));

    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(r));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(b));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fr));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fb));

    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(l));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(b));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fl));
    OUT_ACCEL_REG(RADEON_SE_PORT_DATA0, F_TO_DW(fb));

    OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);

    FINISH_ACCEL();
#endif
}

#undef FUNC_NAME
#endif /* USE_XAA */
