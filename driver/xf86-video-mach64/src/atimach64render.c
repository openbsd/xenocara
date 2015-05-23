/*
 * Copyright 2006 George Sapountzis
 * All Rights Reserved.
 *
 * Based on the mach64 DRI and DRM drivers:
 * Copyright 2000 Gareth Hughes
 * Copyright 2002-2003 Leif Delgass
 * All Rights Reserved.
 *
 * Based on the ati hw/kdrive driver:
 * Copyright 2003 Eric Anholt, Anders Carlsson
 *
 * Based on the via hw/xfree86 driver:
 * Copyright 2006 Thomas Hellstrom. All Rights Reserved.
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
 *    George Sapountzis <gsap7@yahoo.gr>
 */

/*
 * Interesting cases for RENDER acceleration:
 *
 * cursor      : ARGB8888 (24x24)   Over
 *               RGB565
 *
 * glyph       : A8       (9x10)    Add
 *               A8       (420x13)
 * glyph set   : ARGB8888 (1x1 R)   In
 *               A8       (420x13)  Over
 *               RGB565
 *
 * shadow      : ARGB8888 (1x1 R)   In
 *               A8       (670x362) Over
 *               RGB565
 * translucent : RGB565   (652x344) In
 *               A8       (1x1 R)   Over
 *               RGB565
 *
 * In all interesting cases one of src/mask is "1x1 R".
 */

/*
 * Assumptions and limitations of mach64 RENDER acceleration:
 *
 * RENDER acceleration is supported for GTPRO and later chips using the 3D
 * triangle setup, i.e. the VERTEX_? registers (see the dri driver). According
 * to atiregs.h, SCALE_3D_CNTL and TEX_?_OFF appear in GT, thus chips as old
 * as GT should be capable of RENDER acceleration, using the S_?_INC, T_?_INC
 * registers for texture mapping (see the directfb driver).
 *
 * GTPRO added a triangle setup engine and multitexturing. However, it seems
 * that none of the 8bpp mach64 formats expands the 8bit value to the alpha
 * channel in texture mapping, RGB8 appears to expand to (I,I,I,0). This makes
 * GTPRO multitexturing unsuitable for emulating the IN operation. Moreover,
 * it seems that GT/GTPRO has a muxltiplexer instead of a blender for computing
 * the final alpha channel which forbids destinations with an alpha channel and
 * generic two-pass compositing.
 *
 * A texture unit combines the fragment color (VERTEX_?_ARGB) coming in from
 * triangle rasterization with the texel from the texture according to the
 * texture environment (TEX_LIGHT_FCN_). "1x1 R" textures may come in as frag-
 * ment colors, eliminating the need for multitexturing in all interesting
 * cases (via also uses this optimization).
 *
 * Texture registers are saved/restored and cached (see atimach64.c). TEX_CNTL
 * cannot be cached because it flushes the texture cache. TEX_?_OFF are also
 * not cached because I am not sure whether writing at some offset register
 * affects the value at another offset.
 *
 * Vertex registers are not saved/restored. This shouldn't be a problem though
 * either for DRI or VT switch because vertex registers are set and used within
 * a signle acceleration hook. Synchronization between the DDX and DRI is based
 * on calling ATIDRISync() at the beginning of each DDX acceleration hook,
 * which suggests the assumption that individual acceleration hooks are not
 * interrupted.
 */

#include <string.h>
#include <stdio.h>

/*
 * Helper functions copied from exa and via.
 */

#if 0
static void
Mach64ExaCompositePictDesc(PicturePtr pict, char *string, int n)
{
    char format[20];
    char size[20];

    if (!pict) {
        snprintf(string, n, "None");
        return;
    }

    switch (pict->format) {
    case PICT_x8r8g8b8:
        snprintf(format, 20, "RGB8888 ");
        break;
    case PICT_x8b8g8r8:
        snprintf(format, 20, "BGR8888 ");
        break;
    case PICT_a8r8g8b8:
        snprintf(format, 20, "ARGB8888");
        break;
    case PICT_a8b8g8r8:
        snprintf(format, 20, "ABGR8888");
        break;
    case PICT_r5g6b5:
        snprintf(format, 20, "RGB565  ");
        break;
    case PICT_x1r5g5b5:
        snprintf(format, 20, "RGB555  ");
        break;
    case PICT_a8:
        snprintf(format, 20, "A8      ");
        break;
    case PICT_a1:
        snprintf(format, 20, "A1      ");
        break;
    default:
        snprintf(format, 20, "0x%x", (int)pict->format);
        break;
    }

    snprintf(size, 20, "%dx%d%s%s",
        pict->pDrawable->width,
        pict->pDrawable->height,
        pict->repeat ? " R" : "",
        pict->componentAlpha ? " C" : ""
    );

    snprintf(string, n, "%-10p: fmt %s (%s)", (void *)pict->pDrawable, format, size);
}

static void
Mach64ExaPrintComposite(CARD8 op,
    PicturePtr pSrc, PicturePtr pMask, PicturePtr pDst, char *string)
{
    char sop[20];
    char srcdesc[40], maskdesc[40], dstdesc[40];

    switch (op) {
    case PictOpSrc:
        sprintf(sop, "Src");
        break;
    case PictOpOver:
        sprintf(sop, "Over");
        break;
    case PictOpInReverse:
        sprintf(sop, "InR");
        break;
    case PictOpOutReverse:
        sprintf(sop, "OutR");
        break;
    case PictOpAdd:
        sprintf(sop, "Add");
        break;
    default:
        sprintf(sop, "0x%x", (int)op);
        break;
    }

    Mach64ExaCompositePictDesc(pSrc, srcdesc, 40);
    Mach64ExaCompositePictDesc(pMask, maskdesc, 40);
    Mach64ExaCompositePictDesc(pDst, dstdesc, 40);

    sprintf(string, "op %s, \n"
        "                src  %s\n"
        "                mask %s\n"
        "                dst  %s\n", sop, srcdesc, maskdesc, dstdesc);
}
#endif

static __inline__ CARD32
viaBitExpandHelper(CARD32 component, CARD32 bits)
{
    CARD32 tmp, mask;

    mask = (1 << (8 - bits)) - 1;
    tmp = component << (8 - bits);
    return ((component & 1) ? tmp | mask : tmp);
}

static __inline__ void
Mach64PixelARGB(PixmapPtr pPixmap, CARD32 format, CARD32 *argb)
{
    CARD32 pixel;
    CARD8  comp;
    int    bits, shift;

    pixel = exaGetPixmapFirstPixel(pPixmap);

    /* exaGetRGBAFromPixel()/viaPixelARGB8888() */

    switch (PICT_FORMAT_TYPE(format)) {
    case PICT_TYPE_A:
        shift = 0;
        bits = PICT_FORMAT_A(format);
        comp = (pixel >> shift) & ((1 << bits) - 1);
        comp = viaBitExpandHelper(comp, bits);
        *argb = comp << 24;
        break;
    case PICT_TYPE_ARGB:
        shift = 0;
        bits = PICT_FORMAT_B(format);
        comp = (pixel >> shift) & ((1 << bits) - 1);
        comp = viaBitExpandHelper(comp, bits);
        *argb = comp;

        shift += bits;
        bits = PICT_FORMAT_G(format);
        comp = (pixel >> shift) & ((1 << bits) - 1);
        comp = viaBitExpandHelper(comp, bits);
        *argb |= comp << 8;

        shift += bits;
        bits = PICT_FORMAT_R(format);
        comp = (pixel >> shift) & ((1 << bits) - 1);
        comp = viaBitExpandHelper(comp, bits);
        *argb |= comp << 16;

        shift += bits;
        bits = PICT_FORMAT_A(format);
        if (bits) {
            comp = (pixel >> shift) & ((1 << bits) - 1);
            comp = viaBitExpandHelper(comp, bits);
        } else {
            comp = 0xff;
        }
        *argb |= comp << 24;
        break;
    case PICT_TYPE_ABGR:
        break;
    default:
        break;
    }
}

/*
 * RENDER acceleration for mach64
 */

typedef struct {
    Bool supported;
    CARD32 scale_3d_cntl;
} Mach64BlendOp;

static Mach64BlendOp Mach64BlendOps[] = {
    /* Clear */
    {1, MACH64_ALPHA_BLEND_SRC_ZERO        | MACH64_ALPHA_BLEND_DST_ZERO},
    /* Src */
    {1, MACH64_ALPHA_BLEND_SRC_ONE         | MACH64_ALPHA_BLEND_DST_ZERO},
    /* Dst */
    {1, MACH64_ALPHA_BLEND_SRC_ZERO        | MACH64_ALPHA_BLEND_DST_ONE},
    /* Over */
    {1, MACH64_ALPHA_BLEND_SRC_ONE         | MACH64_ALPHA_BLEND_DST_INVSRCALPHA},
    /* OverReverse */
    {1, MACH64_ALPHA_BLEND_SRC_INVDSTALPHA | MACH64_ALPHA_BLEND_DST_ONE},
    /* In */
    {1, MACH64_ALPHA_BLEND_SRC_DSTALPHA    | MACH64_ALPHA_BLEND_DST_ZERO},
    /* InReverse */
    {1, MACH64_ALPHA_BLEND_SRC_ZERO        | MACH64_ALPHA_BLEND_DST_SRCALPHA},
    /* Out */
    {1, MACH64_ALPHA_BLEND_SRC_INVDSTALPHA | MACH64_ALPHA_BLEND_DST_ZERO},
    /* OutReverse */
    {1, MACH64_ALPHA_BLEND_SRC_ZERO        | MACH64_ALPHA_BLEND_DST_INVSRCALPHA},
    /* Atop */
    {0, MACH64_ALPHA_BLEND_SRC_DSTALPHA    | MACH64_ALPHA_BLEND_DST_INVSRCALPHA},
    /* AtopReverse */
    {0, MACH64_ALPHA_BLEND_SRC_INVDSTALPHA | MACH64_ALPHA_BLEND_DST_SRCALPHA},
    /* Xor */
    {1, MACH64_ALPHA_BLEND_SRC_INVDSTALPHA | MACH64_ALPHA_BLEND_DST_INVSRCALPHA},
    /* Add */
    {1, MACH64_ALPHA_BLEND_SRC_ONE         | MACH64_ALPHA_BLEND_DST_ONE}
};

#define MACH64_NR_BLEND_OPS \
    (sizeof(Mach64BlendOps) / sizeof(Mach64BlendOps[0]))

typedef struct {
    CARD32 pictFormat;
    CARD32 dstFormat;
    CARD32 texFormat;
} Mach64TexFormat;

static Mach64TexFormat Mach64TexFormats[] = {
    {PICT_a8r8g8b8, -1,                       MACH64_DATATYPE_ARGB8888},
    {PICT_x8r8g8b8, MACH64_DATATYPE_ARGB8888, MACH64_DATATYPE_ARGB8888},
    {PICT_a1r5g5b5, -1,                       MACH64_DATATYPE_ARGB1555},
    {PICT_x1r5g5b5, MACH64_DATATYPE_ARGB1555, MACH64_DATATYPE_ARGB1555},
    {PICT_r5g6b5,   MACH64_DATATYPE_RGB565,   MACH64_DATATYPE_RGB565  },
    {PICT_a8,       MACH64_DATATYPE_RGB8,     MACH64_DATATYPE_RGB8    }
};

#define MACH64_NR_TEX_FORMATS \
    (sizeof(Mach64TexFormats) / sizeof(Mach64TexFormats[0]))

#define MACH64_PICT_IS_1x1R(_pPict)      \
    ((_pPict) &&                         \
     (_pPict)->pDrawable->width == 1 &&  \
     (_pPict)->pDrawable->height == 1 && \
     (_pPict)->repeat)

/*
 * CheckComposite hook helper functions.
 */
static __inline__ Bool
Mach64GetOrder(int val, int *shift)
{
    *shift = 0;

    while (val > (1 << *shift))
        (*shift)++;

    return (val == (1 << *shift));
}

static Bool
Mach64CheckTexture(PicturePtr pPict)
{
    int h,w;
    int l2w, l2h, level, i;

    if (pPict->pDrawable == NULL)
	    return FALSE;
    w = pPict->pDrawable->width;
    h = pPict->pDrawable->height;
    for (i = 0; i < MACH64_NR_TEX_FORMATS; i++) {
        if (Mach64TexFormats[i].pictFormat == pPict->format)
            break;
    }

    if (i == MACH64_NR_TEX_FORMATS)
        MACH64_FALLBACK(("Unsupported picture format 0x%x\n",
                        (int)pPict->format));

    /* l2w equals l2p (pitch) for all interesting cases (w >= 64) */
    Mach64GetOrder(w, &l2w);
    Mach64GetOrder(h, &l2h);

    level = (l2w > l2h) ? l2w : l2h;

    if (level > 10)
        MACH64_FALLBACK(("Picture w/h too large (%dx%d)\n", w, h));

    return TRUE;
}

/*
 * CheckComposite acceleration hook.
 */
Bool
Mach64CheckComposite
(
    int        op,
    PicturePtr pSrcPicture,
    PicturePtr pMaskPicture,
    PicturePtr pDstPicture
)
{
    Bool src_solid, mask_solid, mask_comp, op_comp;
    int i;

    if (op >= MACH64_NR_BLEND_OPS || !Mach64BlendOps[op].supported)
        return FALSE;

    if (!Mach64CheckTexture(pSrcPicture))
        return FALSE;

    if (pMaskPicture && !Mach64CheckTexture(pMaskPicture))
        return FALSE;

    /* Check destination format */

    for (i = 0; i < MACH64_NR_TEX_FORMATS; i++) {
        if (Mach64TexFormats[i].pictFormat == pDstPicture->format)
            break;
    }

    if (i == MACH64_NR_TEX_FORMATS || Mach64TexFormats[i].dstFormat == -1)
        MACH64_FALLBACK(("Unsupported dst format 0x%x\n",
                        (int)pDstPicture->format));

    /* Check that A8 src/dst appears only as "A8 ADD A8" */

    if (pDstPicture->format == PICT_a8) {
        if (pMaskPicture || pSrcPicture->format != PICT_a8 || op != PictOpAdd)
            MACH64_FALLBACK(("A8 dst with mask or non-A8 src.\n"));
    }

    if (pDstPicture->format != PICT_a8) {
        if (pSrcPicture->format == PICT_a8)
            MACH64_FALLBACK(("A8 src with non-A8 dst.\n"));
    }

    /* Check that one of src/mask can come in as the fragment color. */

    src_solid = MACH64_PICT_IS_1x1R(pSrcPicture);

    mask_solid = MACH64_PICT_IS_1x1R(pMaskPicture);

    mask_comp = pMaskPicture && pMaskPicture->componentAlpha;

    op_comp = op == PictOpAdd ||
              op == PictOpInReverse ||
              op == PictOpOutReverse;

    if (mask_solid && src_solid)
        MACH64_FALLBACK(("Bad one-pixel IN composite operation.\n"));

    if (pMaskPicture) {
        if (!mask_solid && !src_solid)
            MACH64_FALLBACK(("Multitexturing required.\n"));

        if (!mask_solid && !op_comp)
            MACH64_FALLBACK(("Non-solid mask.\n"));

        if (mask_comp && !src_solid)
            MACH64_FALLBACK(("Component-alpha mask.\n"));

        if (!mask_comp && pMaskPicture->format != PICT_a8)
            MACH64_FALLBACK(("Non-A8 mask.\n"));

        if (mask_comp && pMaskPicture->format != PICT_a8r8g8b8)
            MACH64_FALLBACK(("Non-ARGB mask.\n"));
    }

    return TRUE;
}

/*
 * This function setups the fragment color from a solid pixmap in the presence
 * of a mask.
 */
static __inline__ Bool
Mach64PrepareMask
(
    Mach64ContextRegs3D *m3d,
    int        op,
    PicturePtr pSrcPicture,
    PicturePtr pMaskPicture,
    PixmapPtr  pSrc,
    PixmapPtr  pMask
)
{
    Bool mask_solid, src_solid;
    CARD32 argb = 0;

    mask_solid = MACH64_PICT_IS_1x1R(pMaskPicture);

    src_solid = MACH64_PICT_IS_1x1R(pSrcPicture);

    if (mask_solid) {
        Mach64PixelARGB(pMask, pMaskPicture->format, &argb);
        argb >>= 24;
        argb &= 0xff;

        m3d->frag_mask = TRUE;
        m3d->frag_color = (argb << 24) | (argb << 16) | (argb << 8) | argb;
        return TRUE;
    }

    if (src_solid) {
        /* We can only handle cases where either the src color (e.g. ADD) or
         * the src alpha (e.g. IN_REV, OUT_REV) is used but not both.
         *
         * (ARGB8888 IN A8) OVER RGB565 is implemented as:
         * (ARGB8888 IN A8) ADD ((ARGB8888 IN A8) OUT_REV RGB565).
         */
        if (op == PictOpInReverse || op == PictOpOutReverse) {
            Mach64PixelARGB(pSrc, pSrcPicture->format, &argb);
            argb >>= 24;
            argb &= 0xff;

            m3d->frag_src = TRUE;
            m3d->frag_color = (argb << 24) | (argb << 16) | (argb << 8) | argb;
            m3d->color_alpha = TRUE;
            return TRUE;
        }

        if (op == PictOpAdd) {
            Mach64PixelARGB(pSrc, pSrcPicture->format, &argb);

            m3d->frag_src = TRUE;
            m3d->frag_color = argb;
            return TRUE;
        }
    }

    return FALSE;
}

/*
 * This function setups the texturing and blending environments. It also
 * manipulates blend control for non-solid masks.
 */
static void __inline__
Mach64BlendCntl(Mach64ContextRegs3D *m3d, int op)
{
    m3d->scale_3d_cntl |= MACH64_SCALE_PIX_EXPAND_DYNAMIC_RANGE |
                          MACH64_SCALE_DITHER_2D_TABLE |
                          MACH64_DITHER_INIT_RESET;

    m3d->scale_3d_cntl |= Mach64BlendOps[op].scale_3d_cntl;

    if (m3d->color_alpha) {
        /* A8 uses RGB8 which expands to (I,I,I,0). Thus, we use the color
         * channels instead of the alpha channel as the alpha factor. We also
         * use the color channels for ARGB8888 masks with component-alpha.
         */
        CARD32 Ad = m3d->scale_3d_cntl & MACH64_ALPHA_BLEND_DST_MASK;

        /* InReverse */
        if (Ad == MACH64_ALPHA_BLEND_DST_SRCALPHA) {
            m3d->scale_3d_cntl &= ~MACH64_ALPHA_BLEND_DST_MASK;
            m3d->scale_3d_cntl |=  MACH64_ALPHA_BLEND_DST_SRCCOLOR;
        }

        /* OutReverse */
        if (Ad == MACH64_ALPHA_BLEND_DST_INVSRCALPHA) {
            m3d->scale_3d_cntl &= ~MACH64_ALPHA_BLEND_DST_MASK;
            m3d->scale_3d_cntl |=  MACH64_ALPHA_BLEND_DST_INVSRCCOLOR;
        }
    }

    /* Can't color mask and blend at the same time */
    m3d->dp_write_mask = 0xffffffff;

    /* Can't fog and blend at the same time */
    m3d->scale_3d_cntl |= MACH64_ALPHA_FOG_EN_ALPHA;

    /* Enable texture mapping mode */
    m3d->scale_3d_cntl |= MACH64_SCALE_3D_FCN_TEXTURE;
    m3d->scale_3d_cntl |= MACH64_MIP_MAP_DISABLE;

    /* Setup the texture environment */
    m3d->scale_3d_cntl |= MACH64_TEX_LIGHT_FCN_MODULATE;

    /* Initialize texture unit */
    m3d->tex_cntl |= MACH64_TEX_ST_DIRECT |
                     MACH64_TEX_SRC_LOCAL |
                     MACH64_TEX_UNCOMPRESSED |
                     MACH64_TEX_CACHE_FLUSH |
                     MACH64_TEX_CACHE_SIZE_4K;
}

/*
 * This function setups the texture unit.
 */
static Bool
Mach64PrepareTexture(PicturePtr pPict, PixmapPtr pPix)
{
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pPix->drawable.pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);
    Mach64ContextRegs3D *m3d = &pATI->m3d;

    CARD32 texFormat;

    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    int l2w, l2h, l2p, level, pitch, cpp, i;

    /* Prepare picture format */
    for (i = 0; i < MACH64_NR_TEX_FORMATS; i++) {
        if (Mach64TexFormats[i].pictFormat == pPict->format)
            break;
    }
    if (i == MACH64_NR_TEX_FORMATS)
        MACH64_FALLBACK(("Unsupported picture format 0x%x\n",
                        (int)pPict->format));
    texFormat = Mach64TexFormats[i].texFormat;

    /* Prepare picture size */
    cpp = PICT_FORMAT_BPP(pPict->format) / 8;
    pitch = exaGetPixmapPitch(pPix) / cpp;

    Mach64GetOrder(w, &l2w);
    Mach64GetOrder(h, &l2h);
    Mach64GetOrder(pitch, &l2p);

    if (pPict->repeat && w == 1 && h == 1)
        l2p = 0;
    else if (pPict->repeat)
        MACH64_FALLBACK(("Repeat not supported for w,h != 1,1\n"));

    l2w = l2p;

    level = (l2w > l2h) ? l2w : l2h;

    m3d->tex_width = (1 << l2w);
    m3d->tex_height = (1 << l2h);

    /* Update hw state */
    m3d->dp_pix_width |= SetBits(texFormat, DP_SCALE_PIX_WIDTH);

    m3d->tex_size_pitch = (l2w   << 0) |
                          (level << 4) |
                          (l2h   << 8);

    m3d->tex_offset = exaGetPixmapOffset(pPix);

    if (PICT_FORMAT_A(pPict->format))
        m3d->scale_3d_cntl |= MACH64_TEX_MAP_AEN;

    switch (pPict->filter) {
    case PictFilterNearest:
        m3d->scale_3d_cntl |= MACH64_TEX_BLEND_FCN_NEAREST;
        break;
    case PictFilterBilinear:
        /* FIXME */
#if 0
        m3d->scale_3d_cntl |= MACH64_TEX_BLEND_FCN_LINEAR;
        m3d->scale_3d_cntl |= MACH64_BILINEAR_TEX_EN;
#endif
        MACH64_FALLBACK(("Bilinear filter 0x%x\n", pPict->filter));
        break;
    default:
        MACH64_FALLBACK(("Bad filter 0x%x\n", pPict->filter));
    }

    m3d->transform = pPict->transform;

    return TRUE;
}

/*
 * PrepareComposite acceleration hook.
 */
Bool
Mach64PrepareComposite
(
    int        op,
    PicturePtr pSrcPicture,
    PicturePtr pMaskPicture,
    PicturePtr pDstPicture,
    PixmapPtr  pSrc,
    PixmapPtr  pMask,
    PixmapPtr  pDst
)
{
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pDst->drawable.pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);
    Mach64ContextRegs3D *m3d = &pATI->m3d;

    CARD32 dstFormat;
    int offset, i;

    ATIDRISync(pScreenInfo);

    /* Initialize state */
    m3d->dp_mix = SetBits(MIX_SRC, DP_BKGD_MIX) |
                  SetBits(MIX_SRC, DP_FRGD_MIX);

    m3d->dp_src = SetBits(SRC_SCALER_3D, DP_BKGD_SRC) |
                  SetBits(SRC_SCALER_3D, DP_FRGD_SRC) |
                  DP_MONO_SRC_ALLONES;

    Mach64GetPixmapOffsetPitch(pDst, &m3d->dst_pitch_offset);

    m3d->scale_3d_cntl = 0;
    m3d->tex_cntl = 0;

    m3d->frag_src = FALSE;
    m3d->frag_mask = FALSE;
    m3d->frag_color = 0xffffffff;

    m3d->color_alpha = FALSE;

    m3d->transform = NULL;

    /* Compute state */
    if (pMaskPicture && !Mach64PrepareMask(m3d, op, pSrcPicture, pMaskPicture,
                                           pSrc, pMask))
        return FALSE;

    Mach64BlendCntl(m3d, op);

    for (i = 0; i < MACH64_NR_TEX_FORMATS; i++) {
        if (Mach64TexFormats[i].pictFormat == pDstPicture->format)
            break;
    }
    if (i == MACH64_NR_TEX_FORMATS)
        MACH64_FALLBACK(("Unsupported picture format 0x%x\n",
                        (int)pPict->format));
    dstFormat = Mach64TexFormats[i].dstFormat;

    m3d->dp_pix_width = SetBits(dstFormat, DP_DST_PIX_WIDTH) |
                        SetBits(dstFormat, DP_SRC_PIX_WIDTH) |
                        SetBits(dstFormat, DP_HOST_PIX_WIDTH);

    if (!m3d->frag_src) {
        if (!Mach64PrepareTexture(pSrcPicture, pSrc))
            return FALSE;
    }

    if (pMaskPicture && !m3d->frag_mask) {
        if (!Mach64PrepareTexture(pMaskPicture, pMask))
            return FALSE;
    }

    offset = TEX_LEVEL(m3d->tex_size_pitch);

    /* Emit state */
    ATIMach64WaitForFIFO(pATI, 12);
    outf(DP_SRC, m3d->dp_src);
    outf(DP_MIX, m3d->dp_mix);

    outf(CLR_CMP_CNTL, CLR_CMP_FN_FALSE);
    outf(DST_CNTL, DST_X_DIR | DST_Y_DIR);
    outf(DST_OFF_PITCH, m3d->dst_pitch_offset);

    outf(SCALE_3D_CNTL, m3d->scale_3d_cntl);
    outf(DP_WRITE_MASK, m3d->dp_write_mask);
    outf(DP_PIX_WIDTH, m3d->dp_pix_width);

    outf(SETUP_CNTL, 0);

    outf(TEX_SIZE_PITCH, m3d->tex_size_pitch);
    outf(TEX_CNTL, m3d->tex_cntl);
    outf(TEX_0_OFF + offset, m3d->tex_offset);

    return TRUE;
}

/*
 * Vertex format, setup and emission.
 */
typedef struct {
    float s0;    /* normalized texture coords */
    float t0;
    float x;     /* quarter-pixels */
    float y;
    CARD32 argb; /* fragment color */
} Mach64Vertex;

#define VTX_SET(_v, _col, _dstX, _dstY, _srcX, _dx, _srcY, _dy) \
do {                                                            \
    _v.s0 = ((float)(_srcX) + _dx) / m3d->tex_width;            \
    _v.t0 = ((float)(_srcY) + _dy) / m3d->tex_height;           \
    _v.x  = ((float)(_dstX) * 4.0);                             \
    _v.y  = ((float)(_dstY) * 4.0);                             \
    _v.argb = _col;                                             \
} while (0)

static __inline__ CARD32
FVAL(float f)
{
    union { float f; CARD32 c; } fc;

    fc.f = f;
    return fc.c;
}

#define VTX_OUT(_v, n)                    \
do {                                      \
    float w = 1.0;                        \
    CARD32 z = 0xffff << 15;              \
    CARD32 x_y = ((CARD16)_v.x << 16) |   \
                 ((CARD16)_v.y & 0xffff); \
                                          \
    ATIMach64WaitForFIFO(pATI, 6);        \
    outf(VERTEX_##n##_S, FVAL(_v.s0));    \
    outf(VERTEX_##n##_T, FVAL(_v.t0));    \
    outf(VERTEX_##n##_W, FVAL(w));        \
                                          \
    outf(VERTEX_##n##_Z, z);              \
    outf(VERTEX_##n##_ARGB, _v.argb);     \
    outf(VERTEX_##n##_X_Y, x_y);          \
} while (0)

/*
 * Composite acceleration hook.
 */
void
Mach64Composite
(
    PixmapPtr pDst,
    int       srcX,
    int       srcY,
    int       maskX,
    int       maskY,
    int       dstX,
    int       dstY,
    int       w,
    int       h
)
{
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pDst->drawable.pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);
    Mach64ContextRegs3D *m3d = &pATI->m3d;

    Mach64Vertex v0, v1, v2, v3;
    float ooa;
    CARD32 col;
    PictVector v;
    int srcXend, srcYend;
    float dxy = 0.0, dwh = 0.0;

    ATIDRISync(pScreenInfo);

    /* Disable clipping if it gets in the way */
    ATIMach64ValidateClip(pATI, dstX, dstX + w - 1, dstY, dstY + h - 1);

    /* Handle solid textures which come in as fragment color */
    col = m3d->frag_color;
    if (m3d->frag_src) {
        srcX = maskX;
        srcY = maskY;
    }

    /* Handle transform */
    srcXend = srcX + w;
    srcYend = srcY + h;
    if (m3d->transform) {
        v.vector[0] = IntToxFixed(srcX);
        v.vector[1] = IntToxFixed(srcY);
        v.vector[2] = xFixed1;
        PictureTransformPoint(m3d->transform, &v);
        srcX = xFixedToInt(v.vector[0]);
        srcY = xFixedToInt(v.vector[1]);

        v.vector[0] = IntToxFixed(srcXend);
        v.vector[1] = IntToxFixed(srcYend);
        v.vector[2] = xFixed1;
        PictureTransformPoint(m3d->transform, &v);
        srcXend = xFixedToInt(v.vector[0]);
        srcYend = xFixedToInt(v.vector[1]);

#if 0
        /* Bilinear needs manipulation of texture coordinates */
        if (m3d->scale_3d_cntl & MACH64_BILINEAR_TEX_EN) {
            dxy =  0.5;
            dwh = -1.0;
        }
#endif
    }

    /* Create vertices in clock-wise order */
    VTX_SET(v0, col, dstX,     dstY,     srcX, dxy,    srcY, dxy);
    VTX_SET(v1, col, dstX + w, dstY,     srcXend, dwh, srcY, dxy);
    VTX_SET(v2, col, dstX + w, dstY + h, srcXend, dwh, srcYend, dwh);
    VTX_SET(v3, col, dstX,     dstY + h, srcX, dxy,    srcYend, dwh);

    /* Setup upper triangle (v0, v1, v3) */
    VTX_OUT(v0, 1);
    VTX_OUT(v1, 2);
    VTX_OUT(v3, 3);

    ooa = 1.0 / (w * h);
    outf(ONE_OVER_AREA, FVAL(ooa));

    /* Setup lower triangle (v2, v1, v3) */
    VTX_OUT(v2, 1);

    ooa = -ooa;
    outf(ONE_OVER_AREA, FVAL(ooa));
}

/*
 * DoneComposite acceleration hook.
 */
void
Mach64DoneComposite(PixmapPtr pDst)
{
    ScrnInfoPtr pScreenInfo = xf86ScreenToScrn(pDst->drawable.pScreen);
    ATIPtr pATI = ATIPTR(pScreenInfo);

    ATIDRISync(pScreenInfo);

    outf(SCALE_3D_CNTL, 0);
}
