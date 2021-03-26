/*
 * Copyright 2003 Eric Anholt
 * Copyright 2003 Anders Carlsson
 * Copyright 2012 Connor Behan
 * Copyright 2012 Michel Dänzer
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
 *    Anders Carlsson <andersca@gnome.org>
 *    Eric Anholt <anholt@FreeBSD.org>
 *    Connor Behan <connor.behan@gmail.com>
 *    Michel Dänzer <michel.daenzer@amd.com>
 *
 */

/* The following is based on the kdrive ATI driver. */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(R128DRI) && defined(RENDER)
#include "xf86.h"
#include "exa.h"

#include "r128.h"
#include "r128_reg.h"
#include "r128_rop.h"


static struct {
    Bool dst_alpha;
    Bool src_alpha;
    uint32_t sblend;
    uint32_t dblend;
} R128BlendOp[] = {
    /* Clear */
    {0, 0, R128_ALPHA_BLEND_ZERO        , R128_ALPHA_BLEND_ZERO},
    /* Src */
    {0, 0, R128_ALPHA_BLEND_ONE         , R128_ALPHA_BLEND_ZERO},
    /* Dst */
    {0, 0, R128_ALPHA_BLEND_ZERO        , R128_ALPHA_BLEND_ONE},
    /* Over */
    {0, 1, R128_ALPHA_BLEND_ONE         , R128_ALPHA_BLEND_INVSRCALPHA},
    /* OverReverse */
    {1, 0, R128_ALPHA_BLEND_INVDSTALPHA , R128_ALPHA_BLEND_ONE},
    /* In */
    {1, 0, R128_ALPHA_BLEND_DSTALPHA    , R128_ALPHA_BLEND_ZERO},
    /* InReverse */
    {0, 1, R128_ALPHA_BLEND_ZERO        , R128_ALPHA_BLEND_SRCALPHA},
    /* Out */
    {1, 0, R128_ALPHA_BLEND_INVDSTALPHA , R128_ALPHA_BLEND_ZERO},
    /* OutReverse */
    {0, 1, R128_ALPHA_BLEND_ZERO        , R128_ALPHA_BLEND_INVSRCALPHA},
    /* Atop */
    {1, 1, R128_ALPHA_BLEND_DSTALPHA    , R128_ALPHA_BLEND_INVSRCALPHA},
    /* AtopReverse */
    {1, 1, R128_ALPHA_BLEND_INVDSTALPHA , R128_ALPHA_BLEND_SRCALPHA},
    /* Xor */
    {1, 1, R128_ALPHA_BLEND_INVDSTALPHA , R128_ALPHA_BLEND_INVSRCALPHA},
    /* Add */
    {0, 0, R128_ALPHA_BLEND_ONE         , R128_ALPHA_BLEND_ONE},
};

static Bool
R128TransformAffineOrScaled(PictTransformPtr t)
{
    if (t == NULL) return TRUE;

    /* the shaders don't handle scaling either */
    return t->matrix[2][0] == 0 && t->matrix[2][1] == 0 && t->matrix[2][2] == IntToxFixed(1);
}

static PixmapPtr
R128GetDrawablePixmap(DrawablePtr pDrawable)
{
    if (pDrawable->type == DRAWABLE_WINDOW)
	return pDrawable->pScreen->GetWindowPixmap((WindowPtr)pDrawable);
    else
	return (PixmapPtr)pDrawable;
}

static PixmapPtr
R128SolidPixmap(ScreenPtr pScreen, uint32_t solid)
{
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);
    PixmapPtr	  pPix	    = pScreen->CreatePixmap(pScreen, 1, 1, 32, 0);

    exaMoveInPixmap(pPix);
    if (!exaDrawableIsOffscreen(&pPix->drawable)) {
        pScreen->DestroyPixmap(pPix);
	return NULL;
    }
    info->ExaDriver->WaitMarker(pScreen, 0);

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (pScrn->bitsPerPixel == 32)
	R128CopySwap(info->ExaDriver->memoryBase + exaGetPixmapOffset(pPix), (uint8_t*)&solid, 4,
		     APER_0_BIG_ENDIAN_32BPP_SWAP);
    else if (pScrn->bitsPerPixel == 16)
	R128CopySwap(info->ExaDriver->memoryBase + exaGetPixmapOffset(pPix), (uint8_t*)&solid, 4,
		     APER_0_BIG_ENDIAN_16BPP_SWAP);
    else
	/* Fall through for 8 bpp */
#endif
    memcpy(info->ExaDriver->memoryBase + exaGetPixmapOffset(pPix), &solid, 4);

    return pPix;
}

static Bool
R128GetDatatypePict1(uint32_t format, uint32_t *type)
{
    switch(format) {
    case PICT_r5g6b5:
	*type = R128_DATATYPE_RGB565;
	return TRUE;
    case PICT_x1r5g5b5:
	*type = R128_DATATYPE_ARGB1555;
	return TRUE;
    case PICT_x8r8g8b8:
	*type = R128_DATATYPE_ARGB8888;
	return TRUE;
    default:
        return FALSE;
    }
}

static Bool
R128GetDatatypePict2(uint32_t format, uint32_t *type)
{
    switch(format) {
    case PICT_a8:
        *type = R128_DATATYPE_RGB8;
	return TRUE;
    case PICT_r5g6b5:
	*type = R128_DATATYPE_RGB565;
	return TRUE;
    case PICT_a8r8g8b8:
	*type = R128_DATATYPE_ARGB8888;
	return TRUE;
    default:
        return FALSE;
    }
}

static Bool
R128CheckCompositeTexture(PicturePtr pPict, PicturePtr pDstPict, int op)
{
#if R128_DEBUG
    ScreenPtr     pScreen   = pDstPict->pDrawable->pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
#endif

    unsigned int repeatType = pPict->repeat ? pPict->repeatType : RepeatNone;
    uint32_t tmp1;

    if (!R128GetDatatypePict2(pPict->format, &tmp1)) return FALSE;

    if (pPict->pDrawable) {
        int w = pPict->pDrawable->width;
        int h = pPict->pDrawable->height;

        if (pPict->repeat && ((w & (w - 1)) != 0 || (h & (h - 1)) != 0)) {
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                "NPOT repeat unsupported (%dx%d)\n", w, h));
	    return FALSE;
        }
    }

    if (pPict->filter != PictFilterNearest && pPict->filter != PictFilterBilinear) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Unsupported filter 0x%x\n",
                            pPict->filter));
	return FALSE;
    }

    /* The radeon driver has a long explanation about this part that I don't really understand */
    if (pPict->transform != 0 && repeatType == RepeatNone && PICT_FORMAT_A(pPict->format) == 0) {
	if (!(((op == PictOpSrc) || (op == PictOpClear)) && (PICT_FORMAT_A(pDstPict->format) == 0))) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "REPEAT_NONE unsupported for "
                            "transformed xRGB source\n"));
	    return FALSE;
	}
    }
    if (!R128TransformAffineOrScaled(pPict->transform)) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Non-affine transforms not supported\n"));
	return FALSE;
    }

    return TRUE;
}

Bool
R128CCECheckComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture, PicturePtr pDstPicture)
{
#if R128_DEBUG
    ScreenPtr     pScreen   = pDstPicture->pDrawable->pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
#endif

    PixmapPtr pSrcPixmap, pDstPixmap;
    uint32_t tmp1;

    /* Check for unsupported compositing operations. */
    if (op >= sizeof(R128BlendOp) / sizeof(R128BlendOp[0])) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Unsupported Composite op 0x%x\n", op));
	return FALSE;
    }

    pDstPixmap = R128GetDrawablePixmap(pDstPicture->pDrawable);
    if (pDstPixmap->drawable.width > 1024 || pDstPixmap->drawable.height > 1024) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Dest w/h too large (%d,%d).\n",
                            pDstPixmap->drawable.width,
                            pDstPixmap->drawable.height));
	return FALSE;
    }

    if (pSrcPicture->pDrawable) {
        pSrcPixmap = R128GetDrawablePixmap(pSrcPicture->pDrawable);
        if (pSrcPixmap->drawable.width > 1024 || pSrcPixmap->drawable.height > 1024) {
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                "Source w/h too large (%d,%d).\n",
                                pSrcPixmap->drawable.width,
                                pSrcPixmap->drawable.height));
	    return FALSE;
        }
    } else if (pSrcPicture->pSourcePict->type != SourcePictTypeSolidFill) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Gradient pictures not supported yet\n"));
	return FALSE;
    }

    if (pDstPicture->format == PICT_a8) {
        if (R128BlendOp[op].src_alpha || R128BlendOp[op].dst_alpha || pMaskPicture != NULL) {
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                "Alpha blending unsupported with "
                                "A8 dst?\n"));
	    return FALSE;
	}
    } else {
        if (!R128GetDatatypePict1(pDstPicture->format, &tmp1)) return FALSE;
    }

    if (pMaskPicture) {
        PixmapPtr pMaskPixmap;

        if (pMaskPicture->pDrawable) {
	    pMaskPixmap = R128GetDrawablePixmap(pMaskPicture->pDrawable);
            if (pMaskPixmap->drawable.width > 1024 || pMaskPixmap->drawable.height > 1024) {
                DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                    "Mask w/h too large (%d,%d).\n",
                                    pMaskPixmap->drawable.width,
                                    pMaskPixmap->drawable.height));
	        return FALSE;
            }
	} else if (pMaskPicture->pSourcePict->type != SourcePictTypeSolidFill) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Gradient pictures not supported yet\n"));
	    return FALSE;
	}

	if (pMaskPicture->componentAlpha && R128BlendOp[op].src_alpha) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Component alpha not supported with "
                            "source alpha blending\n"));
	    return FALSE;
	}

	if (!R128CheckCompositeTexture(pMaskPicture, pDstPicture, op)) return FALSE;
    }

    if (!R128CheckCompositeTexture(pSrcPicture, pDstPicture, op)) return FALSE;
    return TRUE;
}

static Bool
R128TextureSetup(PicturePtr pPict, PixmapPtr pPix, int unit, uint32_t *txsize, uint32_t *tex_cntl_c, Bool trying_solid)
{
    ScreenPtr     pScreen   = pPix->drawable.pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);

    int w, h, bytepp, shift, l2w, l2h, l2p, pitch;

    if (pPict->pDrawable) {
	w = pPict->pDrawable->width;
	h = pPict->pDrawable->height;
    } else {
	w = h = 1;
    }

    pitch = exaGetPixmapPitch(pPix);
    if ((pitch & (pitch - 1)) != 0) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "NPOT pitch 0x%x unsupported\n", pitch));
	return FALSE;
    }

    if (!R128GetDatatypePict2(pPict->format, tex_cntl_c)) return FALSE;

    bytepp = PICT_FORMAT_BPP(pPict->format) / 8;
    *tex_cntl_c |= R128_MIP_MAP_DISABLE;

    if (pPict->filter == PictFilterBilinear) {
        *tex_cntl_c |= R128_MIN_BLEND_LINEAR | R128_MAG_BLEND_LINEAR;
    } else if (pPict->filter == PictFilterNearest) {
	*tex_cntl_c |= R128_MIN_BLEND_NEAREST | R128_MAG_BLEND_NEAREST;
    } else {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Bad filter 0x%x\n", pPict->filter));
	return FALSE;
    }

    if (unit == 0) {
        shift = 0;
    } else {
        shift = 16;
        *tex_cntl_c |= R128_SEC_SELECT_SEC_ST;
    }

    l2w = R128MinBits(w) - 1;
    l2h = R128MinBits(h) - 1;
    l2p = R128MinBits(pitch / bytepp) - 1;

    if (pPict->repeat && w == 1 && h == 1) {
        l2p = 0;
    } else if (pPict->repeat && l2p != l2w) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Repeat not supported for pitch != "
                            "width\n"));
	return FALSE;
    }

    l2w = l2p;
    /* This is required to handle NPOT height */
    if ((unit == 1) || (unit == 0 && !pPict->repeat && !trying_solid)) l2h++;

    info->state_2d.widths[unit] = 1 << l2w;
    info->state_2d.heights[unit] = 1 << l2h;
    *txsize |= l2p << (R128_TEX_PITCH_SHIFT + shift);
    *txsize |= ((w > h) ? l2w : l2h) << (R128_TEX_SIZE_SHIFT + shift);
    *txsize |= l2h << (R128_TEX_HEIGHT_SHIFT + shift);

    if (pPict->transform != 0) {
        info->state_2d.is_transform[unit] = TRUE;
        info->state_2d.transform[unit] = pPict->transform;
    } else {
        info->state_2d.is_transform[unit] = FALSE;
    }

    return TRUE;
}

/* The composite preparation commands that are the same every time can
 * just be written once.
 */
#define COMPOSITE_SETUP()				\
do {							\
    BEGIN_RING( 10 );					\
							\
    OUT_RING_REG(R128_SCALE_3D_CNTL,			\
		    R128_SCALE_3D_TEXMAP_SHADE |	\
		    R128_SCALE_PIX_REPLICATE |		\
		    R128_TEX_CACHE_SPLIT |		\
		    R128_TEX_MAP_ALPHA_IN_TEXTURE |	\
		    R128_TEX_CACHE_LINE_SIZE_4QW);	\
    OUT_RING_REG(R128_SETUP_CNTL,			\
		    R128_COLOR_GOURAUD |		\
		    R128_PRIM_TYPE_TRI |		\
		    R128_TEXTURE_ST_MULT_W |		\
		    R128_STARTING_VERTEX_1 |		\
		    R128_ENDING_VERTEX_3 |		\
		    R128_SUB_PIX_4BITS);		\
    OUT_RING_REG(R128_PM4_VC_FPU_SETUP,			\
		    R128_FRONT_DIR_CCW |		\
		    R128_BACKFACE_SOLID |		\
		    R128_FRONTFACE_SOLID |		\
		    R128_FPU_COLOR_GOURAUD |		\
		    R128_FPU_SUB_PIX_4BITS |		\
		    R128_FPU_MODE_3D |			\
		    R128_TRAP_BITS_DISABLE |		\
		    R128_XFACTOR_2 |			\
		    R128_YFACTOR_2 |			\
		    R128_FLAT_SHADE_VERTEX_OGL |	\
		    R128_FPU_ROUND_TRUNCATE |		\
		    R128_WM_SEL_8DW);			\
    OUT_RING_REG(R128_PLANE_3D_MASK_C, 0xffffffff);	\
    OUT_RING_REG(R128_CONSTANT_COLOR_C, 0xff000000);	\
							\
    ADVANCE_RING();					\
} while(0)

Bool
R128CCEPrepareComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
    PicturePtr pDstPicture, PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
    ScreenPtr     pScreen   = pDst->drawable.pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    RING_LOCALS;

    Bool add_src = FALSE;
    Bool add_msk = FALSE;
    uint32_t txsize = 0, prim_tex_cntl_c, sec_tex_cntl_c = 0, dstDatatype;
    uint32_t src_pitch_offset, dst_pitch_offset, color_factor, in_color_factor, alpha_comb;
    uint32_t sblend, dblend, blend_cntl, window_offset;
    int i;

    if (pDstPicture->format == PICT_a8) {
        if (R128BlendOp[op].dst_alpha) {
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                "Can't dst alpha blend A8\n"));
	    return FALSE;
        }
        dstDatatype = R128_DATATYPE_Y8;
    } else {
        if (!R128GetDatatypePict1(pDstPicture->format, &dstDatatype)) return FALSE;
    }

    if (!pSrc) {
	pSrc = R128SolidPixmap(pScreen, cpu_to_le32(pSrcPicture->pSourcePict->solidFill.color));
	if (!pSrc) {
        DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Failed to create solid scratch "
                            "pixmap\n"));
	    return FALSE;
	}
	add_src = TRUE;
    }
    if (pMaskPicture) {
        info->state_2d.has_mask = TRUE;
        if (!pMask) {
	    pMask = R128SolidPixmap(pScreen, cpu_to_le32(pMaskPicture->pSourcePict->solidFill.color));
	    if (!pMask) {
	        if (!pSrcPicture->pDrawable)
		    pScreen->DestroyPixmap(pSrc);
		info->state_2d.has_mask = FALSE;
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                "Failed to create "
                                "solid scratch pixmap\n"));
	        return FALSE;
	    }
	    add_msk = TRUE;
	}
    } else {
        info->state_2d.has_mask = FALSE;
    }

    if (!R128TextureSetup(pSrcPicture, pSrc, 0, &txsize, &prim_tex_cntl_c, (add_src || add_msk))) return FALSE;

    if (pMask != NULL) {
	info->state_2d.has_mask = TRUE;
        if (!R128TextureSetup(pMaskPicture, pMask, 1, &txsize, &sec_tex_cntl_c, (add_src || add_msk))) return FALSE;
    } else {
	info->state_2d.has_mask = FALSE;
	info->state_2d.is_transform[1] = FALSE;
    }

    if (!R128GetPixmapOffsetPitch(pDst, &dst_pitch_offset)) return FALSE;
    if (!R128GetPixmapOffsetPitch(pSrc, &src_pitch_offset)) return FALSE;

    info->state_2d.in_use = TRUE;
    if (add_src) info->state_2d.src_pix = pSrc;
    if (add_msk) info->state_2d.msk_pix = pMask;
    sblend = R128BlendOp[op].sblend;
    dblend = R128BlendOp[op].dblend;
    if (PICT_FORMAT_A(pDstPicture->format) == 0 && R128BlendOp[op].dst_alpha) {
        if (sblend == R128_ALPHA_BLEND_DSTALPHA)
	    sblend = R128_ALPHA_BLEND_ONE;
	else if (sblend == R128_ALPHA_BLEND_INVDSTALPHA)
	    sblend = R128_ALPHA_BLEND_ZERO;
    }
    blend_cntl = (sblend << R128_ALPHA_BLEND_SRC_SHIFT) |
		 (dblend << R128_ALPHA_BLEND_DST_SHIFT);

    R128CCE_REFRESH( pScrn, info );

    if (!info->state_2d.composite_setup) {
        COMPOSITE_SETUP();
        info->state_2d.composite_setup = TRUE;
    }

    /* We cannot guarantee that this register will stay zero - DRI needs it too. */
    if (info->have3DWindows)
        info->ExaDriver->WaitMarker(pScreen, 0);
    window_offset = INREG(R128_WINDOW_XY_OFFSET);
    info->state_2d.x_offset = (window_offset & 0xfff00000) >> R128_WINDOW_X_SHIFT;
    info->state_2d.y_offset = (window_offset & 0x000fffff) >> R128_WINDOW_Y_SHIFT;

    info->state_2d.dp_gui_master_cntl = (R128_GMC_DST_PITCH_OFFSET_CNTL |
        R128_GMC_BRUSH_SOLID_COLOR |
        (dstDatatype >> 8) |
        R128_GMC_SRC_DATATYPE_COLOR |
        R128_ROP[3].rop |
        R128_DP_SRC_SOURCE_MEMORY |
        R128_GMC_3D_FCN_EN |
        R128_GMC_CLR_CMP_CNTL_DIS |
        R128_GMC_AUX_CLIP_DIS |
        R128_GMC_WR_MSK_DIS);
    info->state_2d.dp_cntl = (R128_DST_X_LEFT_TO_RIGHT | R128_DST_Y_TOP_TO_BOTTOM);
    info->state_2d.dp_brush_frgd_clr = 0xffffffff;
    info->state_2d.dp_brush_bkgd_clr = 0x00000000;
    info->state_2d.dp_src_frgd_clr = 0xffffffff;
    info->state_2d.dp_src_bkgd_clr = 0x00000000;
    info->state_2d.dp_write_mask = 0xffffffff;
    info->state_2d.dst_pitch_offset = dst_pitch_offset;
    info->state_2d.src_pitch_offset = src_pitch_offset;
    info->state_2d.default_sc_bottom_right = (R128_DEFAULT_SC_RIGHT_MAX | R128_DEFAULT_SC_BOTTOM_MAX);
    EmitCCE2DState(pScrn);

    BEGIN_RING( 6 );
    OUT_RING_REG(R128_MISC_3D_STATE_CNTL_REG,
        R128_MISC_SCALE_3D_TEXMAP_SHADE |
        R128_MISC_SCALE_PIX_REPLICATE |
        R128_ALPHA_COMB_ADD_CLAMP |
        blend_cntl);
    OUT_RING_REG(R128_TEX_CNTL_C,
        R128_TEXMAP_ENABLE |
        ((pMask != NULL) ? R128_SEC_TEXMAP_ENABLE : 0) |
        R128_ALPHA_ENABLE |
        R128_TEX_CACHE_FLUSH);
    OUT_RING_REG(R128_PC_GUI_CTLSTAT, R128_PC_FLUSH_GUI);
    ADVANCE_RING();

    /* IN operator: Without a mask, only the first texture unit is enabled.
     * With a mask, we put the source in the first unit and have it pass
     * through as input to the 2nd.  The 2nd unit takes the incoming source
     * pixel and modulates it with either the alpha or each of the channels
     * in the mask, depending on componentAlpha.
     */
    BEGIN_RING( 15 );
    /* R128_PRIM_TEX_CNTL_C,
     * R128_PRIM_TEXTURE_COMBINE_CNTL_C,
     * R128_TEX_SIZE_PITCH_C,
     * R128_PRIM_TEX_0_OFFSET_C - R128_PRIM_TEX_10_OFFSET_C
     */
    OUT_RING(CCE_PACKET0(R128_PRIM_TEX_CNTL_C, 13));
    OUT_RING(prim_tex_cntl_c);

    /* If this is the only stage and the dest is a8, route the alpha result
     * to the color (red channel, in particular), too.  Otherwise, be sure
     * to zero out color channels of an a8 source.
     */
    if (pMaskPicture == NULL && pDstPicture->format == PICT_a8)
        color_factor = R128_COLOR_FACTOR_ALPHA;
    else if (pSrcPicture->format == PICT_a8)
        color_factor = R128_COLOR_FACTOR_CONST_COLOR;
    else
        color_factor = R128_COLOR_FACTOR_TEX;

    if (PICT_FORMAT_A(pSrcPicture->format) == 0)
        alpha_comb = R128_COMB_ALPHA_COPY_INP;
    else
        alpha_comb = R128_COMB_ALPHA_DIS;

    OUT_RING(R128_COMB_COPY |
        color_factor |
        R128_INPUT_FACTOR_INT_COLOR |
        alpha_comb |
        R128_ALPHA_FACTOR_TEX_ALPHA |
        R128_INP_FACTOR_A_CONST_ALPHA);
    OUT_RING(txsize);
    /* We could save some output by only writing the offset register that
     * will actually be used.  On the other hand, this is easy.
     */
    for (i = 0; i <= 10; i++)
        OUT_RING(exaGetPixmapOffset(pSrc));
    ADVANCE_RING();

    if (pMask != NULL) {
        BEGIN_RING( 14 );
	/* R128_SEC_TEX_CNTL_C,
	 * R128_SEC_TEXTURE_COMBINE_CNTL_C,
	 * R128_SEC_TEX_0_OFFSET_C - R128_SEC_TEX_10_OFFSET_C
	 */
        OUT_RING(CCE_PACKET0(R128_SEC_TEX_CNTL_C, 12));
        OUT_RING(sec_tex_cntl_c);

        if (pDstPicture->format == PICT_a8) {
            color_factor = R128_COLOR_FACTOR_ALPHA;
            in_color_factor = R128_INPUT_FACTOR_PREV_ALPHA;
        } else if (pMaskPicture->componentAlpha) {
            color_factor = R128_COLOR_FACTOR_TEX;
            in_color_factor = R128_INPUT_FACTOR_PREV_COLOR;
        } else {
            color_factor = R128_COLOR_FACTOR_ALPHA;
            in_color_factor = R128_INPUT_FACTOR_PREV_COLOR;
        }

        OUT_RING(R128_COMB_MODULATE |
            color_factor |
            in_color_factor |
            R128_COMB_ALPHA_MODULATE |
            R128_ALPHA_FACTOR_TEX_ALPHA |
            R128_INP_FACTOR_A_PREV_ALPHA);
        for (i = 0; i <= 10; i++)
            OUT_RING(exaGetPixmapOffset(pMask));
	ADVANCE_RING();
    }

    return TRUE;
}

typedef union { float f; uint32_t i; } fi_type;

static inline uint32_t
R128FloatAsInt(float val)
{
	fi_type fi;

	fi.f = val;
	return fi.i;
}

#define VTX_OUT_MASK(_dstX, _dstY, _srcX, _srcY, _maskX, _maskY)			\
do {											\
    OUT_RING(R128FloatAsInt((_dstX)));							\
    OUT_RING(R128FloatAsInt(((float)(_dstY)) + 0.125));					\
    OUT_RING(R128FloatAsInt(0.0));							\
    OUT_RING(R128FloatAsInt(1.0));							\
    OUT_RING(R128FloatAsInt((((float)(_srcX)) + 0.5) / (info->state_2d.widths[0])));	\
    OUT_RING(R128FloatAsInt((((float)(_srcY)) + 0.5) / (info->state_2d.heights[0])));	\
    OUT_RING(R128FloatAsInt((((float)(_maskX)) + 0.5) / (info->state_2d.widths[1])));	\
    OUT_RING(R128FloatAsInt((((float)(_maskY)) + 0.5) / (info->state_2d.heights[1])));	\
} while (0)

#define VTX_OUT(_dstX, _dstY, _srcX, _srcY)						\
do {								       			\
    OUT_RING(R128FloatAsInt((_dstX)));							\
    OUT_RING(R128FloatAsInt(((float)(_dstY)) + 0.125));					\
    OUT_RING(R128FloatAsInt(0.0));							\
    OUT_RING(R128FloatAsInt(1.0));							\
    OUT_RING(R128FloatAsInt((((float)(_srcX)) + 0.5) / (info->state_2d.widths[0])));	\
    OUT_RING(R128FloatAsInt((((float)(_srcY)) + 0.5) / (info->state_2d.heights[0])));	\
} while (0)

void
R128CCEComposite(PixmapPtr pDst, int srcX, int srcY, int maskX, int maskY, int dstX, int dstY, int w, int h)
{
    ScreenPtr     pScreen   = pDst->drawable.pScreen;
    ScrnInfoPtr   pScrn     = xf86ScreenToScrn(pScreen);
    R128InfoPtr   info      = R128PTR(pScrn);
    RING_LOCALS;

    int srcXend, srcYend, maskXend, maskYend;
    PictVector v;

    srcXend = srcX + w;
    srcYend = srcY + h;
    maskXend = maskX + w;
    maskYend = maskY + h;
    if (info->state_2d.is_transform[0]) {
        v.vector[0] = IntToxFixed(srcX);
        v.vector[1] = IntToxFixed(srcY);
        v.vector[2] = xFixed1;
        PictureTransformPoint(info->state_2d.transform[0], &v);
        srcX = xFixedToInt(v.vector[0]);
        srcY = xFixedToInt(v.vector[1]);
        v.vector[0] = IntToxFixed(srcXend);
        v.vector[1] = IntToxFixed(srcYend);
        v.vector[2] = xFixed1;
        PictureTransformPoint(info->state_2d.transform[0], &v);
        srcXend = xFixedToInt(v.vector[0]);
        srcYend = xFixedToInt(v.vector[1]);
    }
    if (info->state_2d.is_transform[1]) {
        v.vector[0] = IntToxFixed(maskX);
        v.vector[1] = IntToxFixed(maskY);
        v.vector[2] = xFixed1;
        PictureTransformPoint(info->state_2d.transform[1], &v);
        maskX = xFixedToInt(v.vector[0]);
        maskY = xFixedToInt(v.vector[1]);
        v.vector[0] = IntToxFixed(maskXend);
        v.vector[1] = IntToxFixed(maskYend);
        v.vector[2] = xFixed1;
        PictureTransformPoint(info->state_2d.transform[1], &v);
        maskXend = xFixedToInt(v.vector[0]);
        maskYend = xFixedToInt(v.vector[1]);
    }

    dstX -= info->state_2d.x_offset;
    dstY -= info->state_2d.y_offset;

    R128CCE_REFRESH( pScrn, info );

    if (info->state_2d.has_mask) {
        BEGIN_RING( 3 + 4 * 8 );
        OUT_RING(CCE_PACKET3(R128_CCE_PACKET3_3D_RNDR_GEN_PRIM, 1 + 4 * 8));

	OUT_RING(R128_CCE_VC_FRMT_RHW |
            R128_CCE_VC_FRMT_S_T |
            R128_CCE_VC_FRMT_S2_T2);
    } else {
        BEGIN_RING( 3 + 4 * 6 );
        OUT_RING(CCE_PACKET3(R128_CCE_PACKET3_3D_RNDR_GEN_PRIM, 1 + 4 * 6));

	OUT_RING(R128_CCE_VC_FRMT_RHW |
            R128_CCE_VC_FRMT_S_T);
    }

    OUT_RING(R128_CCE_VC_CNTL_PRIM_TYPE_TRI_FAN |
        R128_CCE_VC_CNTL_PRIM_WALK_RING |
        (4 << R128_CCE_VC_CNTL_NUM_SHIFT));

    if (info->state_2d.has_mask) {
	VTX_OUT_MASK(dstX,     dstY,     srcX,    srcY,    maskX,    maskY);
	VTX_OUT_MASK(dstX,     dstY + h, srcX,    srcYend, maskX,    maskYend);
	VTX_OUT_MASK(dstX + w, dstY + h, srcXend, srcYend, maskXend, maskYend);
	VTX_OUT_MASK(dstX + w, dstY,     srcXend, srcY,    maskXend, maskY);
    } else {
	VTX_OUT(dstX,     dstY,     srcX,    srcY);
	VTX_OUT(dstX,     dstY + h, srcX,    srcYend);
	VTX_OUT(dstX + w, dstY + h, srcXend, srcYend);
	VTX_OUT(dstX + w, dstY,     srcXend, srcY);
    }

    ADVANCE_RING();
}

#endif
