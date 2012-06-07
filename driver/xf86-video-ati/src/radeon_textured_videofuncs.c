/*
 * Copyright 2008 Alex Deucher
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
 *
 * Based on radeon_exa_render.c and kdrive ati_video.c by Eric Anholt, et al.
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

#ifdef ACCEL_CP

#define VTX_OUT_6(_dstX, _dstY, _srcX, _srcY, _maskX, _maskY)	\
do {								\
    OUT_RING_F(_dstX);						\
    OUT_RING_F(_dstY);						\
    OUT_RING_F(_srcX);						\
    OUT_RING_F(_srcY);						\
    OUT_RING_F(_maskX);						\
    OUT_RING_F(_maskY);						\
} while (0)

#define VTX_OUT_4(_dstX, _dstY, _srcX, _srcY)			\
do {								\
    OUT_RING_F(_dstX);						\
    OUT_RING_F(_dstY);						\
    OUT_RING_F(_srcX);						\
    OUT_RING_F(_srcY);						\
} while (0)

#else /* ACCEL_CP */

#define VTX_OUT_6(_dstX, _dstY, _srcX, _srcY, _maskX, _maskY)		\
do {									\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _dstX);			\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _dstY);			\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _srcX);			\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _srcY);			\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _maskX);			\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _maskY);			\
} while (0)

#define VTX_OUT_4(_dstX, _dstY, _srcX, _srcY)			\
do {								\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _dstX);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _dstY);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _srcX);		\
    OUT_ACCEL_REG_F(RADEON_SE_PORT_DATA0, _srcY);		\
} while (0)

#endif /* !ACCEL_CP */

static Bool
FUNC_NAME(RADEONPrepareTexturedVideo)(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    PixmapPtr pPixmap = pPriv->pPixmap;
    struct radeon_exa_pixmap_priv *driver_priv;
    struct radeon_bo *src_bo = pPriv->src_bo[pPriv->currentBuffer];
    uint32_t txformat, txsize, txpitch, txoffset;
    uint32_t dst_pitch, dst_format;
    uint32_t colorpitch;
    int pixel_shift;
    int scissor_w = MIN(pPixmap->drawable.width, 2047);
    int scissor_h = MIN(pPixmap->drawable.height, 2047);
    ACCEL_PREAMBLE();

#ifdef XF86DRM_MODE
    if (info->cs) {
	int ret;

	radeon_cs_space_reset_bos(info->cs);
        radeon_cs_space_add_persistent_bo(info->cs, src_bo, RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM, 0);

	if (pPriv->bicubic_enabled)
	    radeon_cs_space_add_persistent_bo(info->cs, info->bicubic_bo, RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM, 0);

	driver_priv = exaGetPixmapDriverPrivate(pPixmap);
	radeon_cs_space_add_persistent_bo(info->cs, driver_priv->bo, 0, RADEON_GEM_DOMAIN_VRAM);

	ret = radeon_cs_space_check(info->cs);
	if (ret) {
	    ErrorF("Not enough RAM to hw accel xv operation\n");
	    return FALSE;
	}
    }
#else
    (void)src_bo;
#endif

    pixel_shift = pPixmap->drawable.bitsPerPixel >> 4;


#ifdef USE_EXA
    if (info->useEXA) {
	dst_pitch = exaGetPixmapPitch(pPixmap);
    } else
#endif
    {
        dst_pitch = pPixmap->devKind;
    }

#ifdef USE_EXA
    if (info->useEXA) {
	RADEON_SWITCH_TO_3D();
    } else
#endif
    {
	BEGIN_ACCEL(2);
	OUT_ACCEL_REG(RADEON_RB3D_DSTCACHE_CTLSTAT, RADEON_RB3D_DC_FLUSH);
	/* We must wait for 3d to idle, in case source was just written as a dest. */
	OUT_ACCEL_REG(RADEON_WAIT_UNTIL,
		      RADEON_WAIT_HOST_IDLECLEAN |
		      RADEON_WAIT_2D_IDLECLEAN |
		      RADEON_WAIT_3D_IDLECLEAN |
		      RADEON_WAIT_DMA_GUI_IDLE);
	FINISH_ACCEL();

	if (!info->accel_state->XInited3D)
	    RADEONInit3DEngine(pScrn);
    }

    /* Same for R100/R200 */
    switch (pPixmap->drawable.bitsPerPixel) {
    case 16:
	if (pPixmap->drawable.depth == 15)
	    dst_format = RADEON_COLOR_FORMAT_ARGB1555;
	else
	    dst_format = RADEON_COLOR_FORMAT_RGB565;
	break;
    case 32:
	dst_format = RADEON_COLOR_FORMAT_ARGB8888;
	break;
    default:
	return FALSE;
    }

    if (pPriv->id == FOURCC_I420 || pPriv->id == FOURCC_YV12) {
	pPriv->is_planar = TRUE;
	txformat = RADEON_TXFORMAT_Y8;
    } else {
	pPriv->is_planar = FALSE;
	if (pPriv->id == FOURCC_UYVY)
	    txformat = RADEON_TXFORMAT_YVYU422;
	else
	    txformat = RADEON_TXFORMAT_VYUY422;
    }

    txformat |= RADEON_TXFORMAT_NON_POWER2;

    colorpitch = dst_pitch >> pixel_shift;

    if (RADEONTilingEnabled(pScrn, pPixmap))
	colorpitch |= RADEON_COLOR_TILE_ENABLE;

    txoffset = info->cs ? 0 : pPriv->src_offset;

    BEGIN_ACCEL_RELOC(4,2);

    OUT_ACCEL_REG(RADEON_RB3D_CNTL, dst_format);
    EMIT_WRITE_OFFSET(RADEON_RB3D_COLOROFFSET, 0, pPixmap);
    EMIT_COLORPITCH(RADEON_RB3D_COLORPITCH, colorpitch, pPixmap);
    OUT_ACCEL_REG(RADEON_RB3D_BLENDCNTL,
		  RADEON_SRC_BLEND_GL_ONE | RADEON_DST_BLEND_GL_ZERO);

    FINISH_ACCEL();

    if (pPriv->is_planar) {
	/* need 2 texcoord sets (even though they are identical) due
	   to denormalization! hw apparently can't premultiply
	   same coord set by different texture size */
	pPriv->vtx_count = 6;

	txsize = (((((pPriv->w + 1 ) >> 1) - 1) & 0x7ff) |
		  (((((pPriv->h + 1 ) >> 1) - 1) & 0x7ff) << RADEON_TEX_VSIZE_SHIFT));
	txpitch = RADEON_ALIGN(pPriv->src_pitch >> 1, 64);
	txpitch -= 32;

	BEGIN_ACCEL_RELOC(23, 3);

	OUT_ACCEL_REG(RADEON_SE_VTX_FMT, (RADEON_SE_VTX_FMT_XY |
					  RADEON_SE_VTX_FMT_ST0 |
					  RADEON_SE_VTX_FMT_ST1));

	OUT_ACCEL_REG(RADEON_PP_CNTL, (RADEON_TEX_0_ENABLE | RADEON_TEX_BLEND_0_ENABLE |
				       RADEON_TEX_1_ENABLE | RADEON_TEX_BLEND_1_ENABLE |
				       RADEON_TEX_2_ENABLE | RADEON_TEX_BLEND_2_ENABLE |
				       RADEON_PLANAR_YUV_ENABLE));

	/* Y */
	OUT_ACCEL_REG(RADEON_PP_TXFILTER_0,
		      RADEON_MAG_FILTER_LINEAR |
		      RADEON_MIN_FILTER_LINEAR |
		      RADEON_CLAMP_S_CLAMP_LAST |
		      RADEON_CLAMP_T_CLAMP_LAST |
		      RADEON_YUV_TO_RGB);
	OUT_ACCEL_REG(RADEON_PP_TXFORMAT_0, txformat | RADEON_TXFORMAT_ST_ROUTE_STQ0);
	OUT_TEXTURE_REG(RADEON_PP_TXOFFSET_0, txoffset, src_bo);
	OUT_ACCEL_REG(RADEON_PP_TXCBLEND_0,
		      RADEON_COLOR_ARG_A_ZERO |
		      RADEON_COLOR_ARG_B_ZERO |
		      RADEON_COLOR_ARG_C_T0_COLOR |
		      RADEON_BLEND_CTL_ADD |
		      RADEON_CLAMP_TX);
	OUT_ACCEL_REG(RADEON_PP_TXABLEND_0,
		      RADEON_ALPHA_ARG_A_ZERO |
		      RADEON_ALPHA_ARG_B_ZERO |
		      RADEON_ALPHA_ARG_C_T0_ALPHA |
		      RADEON_BLEND_CTL_ADD |
		      RADEON_CLAMP_TX);

	OUT_ACCEL_REG(RADEON_PP_TEX_SIZE_0,
		      (pPriv->w - 1) |
		      ((pPriv->h - 1) << RADEON_TEX_VSIZE_SHIFT));
	OUT_ACCEL_REG(RADEON_PP_TEX_PITCH_0,
		      pPriv->src_pitch - 32);

	/* U */
	OUT_ACCEL_REG(RADEON_PP_TXFILTER_1,
		      RADEON_MAG_FILTER_LINEAR |
		      RADEON_MIN_FILTER_LINEAR |
		      RADEON_CLAMP_S_CLAMP_LAST |
		      RADEON_CLAMP_T_CLAMP_LAST);
	OUT_ACCEL_REG(RADEON_PP_TXFORMAT_1, txformat | RADEON_TXFORMAT_ST_ROUTE_STQ1);
	OUT_TEXTURE_REG(RADEON_PP_TXOFFSET_1, txoffset + pPriv->planeu_offset, src_bo);
	OUT_ACCEL_REG(RADEON_PP_TXCBLEND_1,
		      RADEON_COLOR_ARG_A_ZERO |
		      RADEON_COLOR_ARG_B_ZERO |
		      RADEON_COLOR_ARG_C_T0_COLOR |
		      RADEON_BLEND_CTL_ADD |
		      RADEON_CLAMP_TX);
	OUT_ACCEL_REG(RADEON_PP_TXABLEND_1,
		      RADEON_ALPHA_ARG_A_ZERO |
		      RADEON_ALPHA_ARG_B_ZERO |
		      RADEON_ALPHA_ARG_C_T0_ALPHA |
		      RADEON_BLEND_CTL_ADD |
		      RADEON_CLAMP_TX);

	OUT_ACCEL_REG(RADEON_PP_TEX_SIZE_1, txsize);
	OUT_ACCEL_REG(RADEON_PP_TEX_PITCH_1, txpitch);

	/* V */
	OUT_ACCEL_REG(RADEON_PP_TXFILTER_2,
		      RADEON_MAG_FILTER_LINEAR |
		      RADEON_MIN_FILTER_LINEAR |
		      RADEON_CLAMP_S_CLAMP_LAST |
		      RADEON_CLAMP_T_CLAMP_LAST);
	OUT_ACCEL_REG(RADEON_PP_TXFORMAT_2, txformat | RADEON_TXFORMAT_ST_ROUTE_STQ1);
	OUT_TEXTURE_REG(RADEON_PP_TXOFFSET_2, txoffset + pPriv->planev_offset, src_bo);
	OUT_ACCEL_REG(RADEON_PP_TXCBLEND_2,
		      RADEON_COLOR_ARG_A_ZERO |
		      RADEON_COLOR_ARG_B_ZERO |
		      RADEON_COLOR_ARG_C_T0_COLOR |
		      RADEON_BLEND_CTL_ADD |
		      RADEON_CLAMP_TX);
	OUT_ACCEL_REG(RADEON_PP_TXABLEND_2,
		      RADEON_ALPHA_ARG_A_ZERO |
		      RADEON_ALPHA_ARG_B_ZERO |
		      RADEON_ALPHA_ARG_C_T0_ALPHA |
		      RADEON_BLEND_CTL_ADD |
		      RADEON_CLAMP_TX);

	OUT_ACCEL_REG(RADEON_PP_TEX_SIZE_2, txsize);
	OUT_ACCEL_REG(RADEON_PP_TEX_PITCH_2, txpitch);
	FINISH_ACCEL();
    } else {
	pPriv->vtx_count = 4;
	BEGIN_ACCEL_RELOC(9, 1);

	OUT_ACCEL_REG(RADEON_SE_VTX_FMT, (RADEON_SE_VTX_FMT_XY |
					  RADEON_SE_VTX_FMT_ST0));

	OUT_ACCEL_REG(RADEON_PP_CNTL, RADEON_TEX_0_ENABLE | RADEON_TEX_BLEND_0_ENABLE);

	OUT_ACCEL_REG(RADEON_PP_TXFILTER_0,
		      RADEON_MAG_FILTER_LINEAR |
		      RADEON_MIN_FILTER_LINEAR |
		      RADEON_CLAMP_S_CLAMP_LAST |
		      RADEON_CLAMP_T_CLAMP_LAST |
		      RADEON_YUV_TO_RGB);
	OUT_ACCEL_REG(RADEON_PP_TXFORMAT_0, txformat | RADEON_TXFORMAT_ST_ROUTE_STQ0);
	OUT_TEXTURE_REG(RADEON_PP_TXOFFSET_0, txoffset, src_bo);
	OUT_ACCEL_REG(RADEON_PP_TXCBLEND_0,
		      RADEON_COLOR_ARG_A_ZERO |
		      RADEON_COLOR_ARG_B_ZERO |
		      RADEON_COLOR_ARG_C_T0_COLOR |
		      RADEON_BLEND_CTL_ADD |
		      RADEON_CLAMP_TX);
	OUT_ACCEL_REG(RADEON_PP_TXABLEND_0,
		      RADEON_ALPHA_ARG_A_ZERO |
		      RADEON_ALPHA_ARG_B_ZERO |
		      RADEON_ALPHA_ARG_C_T0_ALPHA |
		      RADEON_BLEND_CTL_ADD |
		      RADEON_CLAMP_TX);

	OUT_ACCEL_REG(RADEON_PP_TEX_SIZE_0,
		      (pPriv->w - 1) |
		      ((pPriv->h - 1) << RADEON_TEX_VSIZE_SHIFT));
	OUT_ACCEL_REG(RADEON_PP_TEX_PITCH_0,
		      pPriv->src_pitch - 32);
	FINISH_ACCEL();
    }

    BEGIN_ACCEL(2);
    OUT_ACCEL_REG(RADEON_RE_TOP_LEFT, 0);
    OUT_ACCEL_REG(RADEON_RE_WIDTH_HEIGHT, ((scissor_w << RADEON_RE_WIDTH_SHIFT) |
					   (scissor_h << RADEON_RE_HEIGHT_SHIFT)));
    FINISH_ACCEL();

    if (pPriv->vsync) {
	xf86CrtcPtr crtc;
	if (pPriv->desired_crtc)
	    crtc = pPriv->desired_crtc;
	else
	    crtc = radeon_pick_best_crtc(pScrn,
					 pPriv->drw_x,
					 pPriv->drw_x + pPriv->dst_w,
					 pPriv->drw_y,
					 pPriv->drw_y + pPriv->dst_h);
	if (crtc)
	    FUNC_NAME(RADEONWaitForVLine)(pScrn, pPixmap,
					  crtc,
					  pPriv->drw_y - crtc->y,
					  (pPriv->drw_y - crtc->y) + pPriv->dst_h);
    }

    return TRUE;
}

static void
FUNC_NAME(RADEONDisplayTexturedVideo)(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    PixmapPtr pPixmap = pPriv->pPixmap;
    int dstxoff, dstyoff;
    BoxPtr pBox = REGION_RECTS(&pPriv->clip);
    int nBox = REGION_NUM_RECTS(&pPriv->clip);
    ACCEL_PREAMBLE();

#ifdef COMPOSITE
    dstxoff = -pPixmap->screen_x + pPixmap->drawable.x;
    dstyoff = -pPixmap->screen_y + pPixmap->drawable.y;
#else
    dstxoff = 0;
    dstyoff = 0;
#endif

    if (!FUNC_NAME(RADEONPrepareTexturedVideo)(pScrn, pPriv))
	return;

    /*
     * Rendering of the actual polygon is done in two different
     * ways depending on chip generation:
     *
     * < R300:
     *
     *     These chips can render a rectangle in one pass, so
     *     handling is pretty straight-forward.
     *
     * >= R300:
     *
     *     These chips can accept a quad, but will render it as
     *     two triangles which results in a diagonal tear. Instead
     *     We render a single, large triangle and use the scissor
     *     functionality to restrict it to the desired rectangle.
     *     Due to guardband limits on r3xx/r4xx, we can only use
     *     the single triangle up to 2560/4021 pixels; above that we
     *     render as a quad.
     */
#ifdef ACCEL_CP
    while (nBox) {
	int draw_size = 3 * pPriv->vtx_count + 5;
	int loop_boxes;

	if (draw_size > radeon_cs_space_remaining(pScrn)) {
	    if (info->cs)
		radeon_cs_flush_indirect(pScrn);
	    else
		RADEONCPFlushIndirect(pScrn, 1);
	    if (!FUNC_NAME(RADEONPrepareTexturedVideo)(pScrn, pPriv))
		return;
	}
	loop_boxes = MIN(radeon_cs_space_remaining(pScrn) / draw_size, nBox);
	nBox -= loop_boxes;

	BEGIN_RING(loop_boxes * 3 * pPriv->vtx_count + 5);
	OUT_RING(CP_PACKET3(RADEON_CP_PACKET3_3D_DRAW_IMMD,
			    loop_boxes * 3 * pPriv->vtx_count + 1));
	if (pPriv->is_planar)
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
		 ((loop_boxes * 3) << RADEON_CP_VC_CNTL_NUM_SHIFT));

	while (loop_boxes--) {
	    float srcX, srcY, srcw, srch;
	    int dstX, dstY, dstw, dsth;
	    dstX = pBox->x1 + dstxoff;
	    dstY = pBox->y1 + dstyoff;
	    dstw = pBox->x2 - pBox->x1;
	    dsth = pBox->y2 - pBox->y1;

	    srcX = pPriv->src_x;
	    srcX += ((pBox->x1 - pPriv->drw_x) *
		     pPriv->src_w) / (float)pPriv->dst_w;
	    srcY = pPriv->src_y;
	    srcY += ((pBox->y1 - pPriv->drw_y) *
		     pPriv->src_h) / (float)pPriv->dst_h;

	    srcw = (pPriv->src_w * dstw) / (float)pPriv->dst_w;
	    srch = (pPriv->src_h * dsth) / (float)pPriv->dst_h;


	    if (pPriv->is_planar) {
		/*
		 * Just render a rect (using three coords).
		 */
		VTX_OUT_6((float)dstX,                     (float)(dstY + dsth),
			  (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h,
			  (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h);
		VTX_OUT_6((float)(dstX + dstw),            (float)(dstY + dsth),
			  (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h,
			  (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h);
		VTX_OUT_6((float)(dstX + dstw),            (float)dstY,
			  (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h,
			  (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h);
	    } else {
		/*
		 * Just render a rect (using three coords).
		 */
		VTX_OUT_4((float)dstX,                     (float)(dstY + dsth),
			  (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h);
		VTX_OUT_4((float)(dstX + dstw),            (float)(dstY + dsth),
			  (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h);
		VTX_OUT_4((float)(dstX + dstw),            (float)dstY,
			  (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h);
	    }

	    pBox++;
	}

	OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);
	ADVANCE_RING();
    }
#else /* ACCEL_CP */
    BEGIN_ACCEL(nBox * pPriv->vtx_count * 3 + 2);
    OUT_ACCEL_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_RECTANGLE_LIST |
				      RADEON_VF_PRIM_WALK_DATA |
				      RADEON_VF_RADEON_MODE |
				      ((nBox * 3) << RADEON_VF_NUM_VERTICES_SHIFT)));
    while (nBox--) {
	float srcX, srcY, srcw, srch;
	int dstX, dstY, dstw, dsth;
	dstX = pBox->x1 + dstxoff;
	dstY = pBox->y1 + dstyoff;
	dstw = pBox->x2 - pBox->x1;
	dsth = pBox->y2 - pBox->y1;

	srcX = pPriv->src_x;
	srcX += ((pBox->x1 - pPriv->drw_x) *
		 pPriv->src_w) / (float)pPriv->dst_w;
	srcY = pPriv->src_y;
	srcY += ((pBox->y1 - pPriv->drw_y) *
		 pPriv->src_h) / (float)pPriv->dst_h;

	srcw = (pPriv->src_w * dstw) / (float)pPriv->dst_w;
	srch = (pPriv->src_h * dsth) / (float)pPriv->dst_h;


	if (pPriv->is_planar) {
	    /*
	     * Just render a rect (using three coords).
	     */
	    VTX_OUT_6((float)dstX,                     (float)(dstY + dsth),
		      (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h,
		      (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h);
	    VTX_OUT_6((float)(dstX + dstw),            (float)(dstY + dsth),
		      (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h,
		      (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h);
	    VTX_OUT_6((float)(dstX + dstw),            (float)dstY,
		      (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h,
		      (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h);
	} else {
	    /*
	     * Just render a rect (using three coords).
	     */
	    VTX_OUT_4((float)dstX,                     (float)(dstY + dsth),
		      (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h);
	    VTX_OUT_4((float)(dstX + dstw),            (float)(dstY + dsth),
		      (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h);
	    VTX_OUT_4((float)(dstX + dstw),            (float)dstY,
		      (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h);
	}

	pBox++;
    }

    OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);
    FINISH_ACCEL();
#endif /* !ACCEL_CP */

    DamageDamageRegion(pPriv->pDraw, &pPriv->clip);
}

static Bool
FUNC_NAME(R200PrepareTexturedVideo)(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    PixmapPtr pPixmap = pPriv->pPixmap;
    struct radeon_exa_pixmap_priv *driver_priv;
    struct radeon_bo *src_bo = pPriv->src_bo[pPriv->currentBuffer];
    uint32_t txformat;
    uint32_t txfilter, txsize, txpitch, txoffset;
    uint32_t dst_pitch, dst_format;
    uint32_t colorpitch;
    int pixel_shift;
    int scissor_w = MIN(pPixmap->drawable.width, 2047);
    int scissor_h = MIN(pPixmap->drawable.height, 2047);
    /* note: in contrast to r300, use input biasing on uv components */
    const float Loff = -0.0627;
    float uvcosf, uvsinf;
    float yco, yoff;
    float uco[3], vco[3];
    float bright, cont, sat;
    int ref = pPriv->transform_index;
    float ucscale = 0.25, vcscale = 0.25;
    Bool needux8 = FALSE, needvx8 = FALSE;
    ACCEL_PREAMBLE();

#ifdef XF86DRM_MODE
    if (info->cs) {
	int ret;

	radeon_cs_space_reset_bos(info->cs);
        radeon_cs_space_add_persistent_bo(info->cs, src_bo, RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM, 0);

	if (pPriv->bicubic_enabled)
	    radeon_cs_space_add_persistent_bo(info->cs, info->bicubic_bo, RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM, 0);

	driver_priv = exaGetPixmapDriverPrivate(pPixmap);
	radeon_cs_space_add_persistent_bo(info->cs, driver_priv->bo, 0, RADEON_GEM_DOMAIN_VRAM);

	ret = radeon_cs_space_check(info->cs);
	if (ret) {
	    ErrorF("Not enough RAM to hw accel xv operation\n");
	    return FALSE;
	}
    }
#else
    (void)src_bo;
#endif

    pixel_shift = pPixmap->drawable.bitsPerPixel >> 4;

#ifdef USE_EXA
    if (info->useEXA) {
	dst_pitch = exaGetPixmapPitch(pPixmap);
    } else
#endif
    {
	dst_pitch = pPixmap->devKind;
    }

#ifdef USE_EXA
    if (info->useEXA) {
	RADEON_SWITCH_TO_3D();
    } else
#endif
    {
	BEGIN_ACCEL(2);
	OUT_ACCEL_REG(RADEON_RB3D_DSTCACHE_CTLSTAT, RADEON_RB3D_DC_FLUSH);
	/* We must wait for 3d to idle, in case source was just written as a dest. */
	OUT_ACCEL_REG(RADEON_WAIT_UNTIL,
		      RADEON_WAIT_HOST_IDLECLEAN |
		      RADEON_WAIT_2D_IDLECLEAN |
		      RADEON_WAIT_3D_IDLECLEAN |
		      RADEON_WAIT_DMA_GUI_IDLE);
	FINISH_ACCEL();

	if (!info->accel_state->XInited3D)
	    RADEONInit3DEngine(pScrn);
    }

    /* Same for R100/R200 */
    switch (pPixmap->drawable.bitsPerPixel) {
    case 16:
	if (pPixmap->drawable.depth == 15)
	    dst_format = RADEON_COLOR_FORMAT_ARGB1555;
	else
	    dst_format = RADEON_COLOR_FORMAT_RGB565;
	break;
    case 32:
	dst_format = RADEON_COLOR_FORMAT_ARGB8888;
	break;
    default:
	return FALSE;
    }

    if (pPriv->id == FOURCC_I420 || pPriv->id == FOURCC_YV12) {
	pPriv->is_planar = TRUE;
	txformat = RADEON_TXFORMAT_I8;
    } else {
	pPriv->is_planar = FALSE;
	if (pPriv->id == FOURCC_UYVY)
	    txformat = RADEON_TXFORMAT_YVYU422;
	else
	    txformat = RADEON_TXFORMAT_VYUY422;
    }

    txformat |= RADEON_TXFORMAT_NON_POWER2;

    colorpitch = dst_pitch >> pixel_shift;

    if (RADEONTilingEnabled(pScrn, pPixmap))
	colorpitch |= RADEON_COLOR_TILE_ENABLE;

    BEGIN_ACCEL_RELOC(4,2);

    OUT_ACCEL_REG(RADEON_RB3D_CNTL, dst_format);
    EMIT_WRITE_OFFSET(RADEON_RB3D_COLOROFFSET, 0, pPixmap);
    EMIT_COLORPITCH(RADEON_RB3D_COLORPITCH, colorpitch, pPixmap);

    OUT_ACCEL_REG(RADEON_RB3D_BLENDCNTL,
		  RADEON_SRC_BLEND_GL_ONE | RADEON_DST_BLEND_GL_ZERO);

    FINISH_ACCEL();

    txfilter =  R200_MAG_FILTER_LINEAR |
	R200_MIN_FILTER_LINEAR |
	R200_CLAMP_S_CLAMP_LAST |
	R200_CLAMP_T_CLAMP_LAST;

    /* contrast can cause constant overflow, clamp */
    cont = RTFContrast(pPriv->contrast);
    if (cont * trans[ref].RefLuma > 2.0)
	cont = 2.0 / trans[ref].RefLuma;
    /* brightness is only from -0.5 to 0.5 should be safe */
    bright = RTFBrightness(pPriv->brightness);
    /* saturation can also cause overflow, clamp */
    sat = RTFSaturation(pPriv->saturation);
    if (sat * trans[ref].RefBCb > 4.0)
	sat = 4.0 / trans[ref].RefBCb;
    uvcosf = sat * cos(RTFHue(pPriv->hue));
    uvsinf = sat * sin(RTFHue(pPriv->hue));

    yco = trans[ref].RefLuma * cont;
    uco[0] = -trans[ref].RefRCr * uvsinf;
    uco[1] = trans[ref].RefGCb * uvcosf - trans[ref].RefGCr * uvsinf;
    uco[2] = trans[ref].RefBCb * uvcosf;
    vco[0] = trans[ref].RefRCr * uvcosf;
    vco[1] = trans[ref].RefGCb * uvsinf + trans[ref].RefGCr * uvcosf;
    vco[2] = trans[ref].RefBCb * uvsinf;
    yoff = Loff * yco + bright;

    if ((uco[0] > 2.0) || (uco[2] > 2.0)) {
	needux8 = TRUE;
	ucscale = 0.125;
    }
    if ((vco[0] > 2.0) || (vco[2] > 2.0)) {
	needvx8 = TRUE;
	vcscale = 0.125;
    }

    txoffset = info->cs ? 0 : pPriv->src_offset;

    if (pPriv->is_planar) {
	/* need 2 texcoord sets (even though they are identical) due
	   to denormalization! hw apparently can't premultiply
	   same coord set by different texture size */
	pPriv->vtx_count = 6;

	txsize = (((((pPriv->w + 1 ) >> 1) - 1) & 0x7ff) |
		  (((((pPriv->h + 1 ) >> 1) - 1) & 0x7ff) << RADEON_TEX_VSIZE_SHIFT));
	txpitch = RADEON_ALIGN(pPriv->src_pitch >> 1, 64);
	txpitch -= 32;

	BEGIN_ACCEL_RELOC(36, 3);

	OUT_ACCEL_REG(RADEON_PP_CNTL,
		      RADEON_TEX_0_ENABLE | RADEON_TEX_1_ENABLE | RADEON_TEX_2_ENABLE |
		      RADEON_TEX_BLEND_0_ENABLE |
		      RADEON_TEX_BLEND_1_ENABLE |
		      RADEON_TEX_BLEND_2_ENABLE);

	OUT_ACCEL_REG(R200_SE_VTX_FMT_0, R200_VTX_XY);
	OUT_ACCEL_REG(R200_SE_VTX_FMT_1,
		      (2 << R200_VTX_TEX0_COMP_CNT_SHIFT) |
		      (2 << R200_VTX_TEX1_COMP_CNT_SHIFT));

	OUT_ACCEL_REG(R200_PP_TXFILTER_0, txfilter);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_0, txformat);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_X_0, 0);
	OUT_ACCEL_REG(R200_PP_TXSIZE_0,
		      (pPriv->w - 1) |
		      ((pPriv->h - 1) << RADEON_TEX_VSIZE_SHIFT));
	OUT_ACCEL_REG(R200_PP_TXPITCH_0, pPriv->src_pitch - 32);
	OUT_TEXTURE_REG(R200_PP_TXOFFSET_0, txoffset, src_bo);

	OUT_ACCEL_REG(R200_PP_TXFILTER_1, txfilter);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_1, txformat | R200_TXFORMAT_ST_ROUTE_STQ1);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_X_1, 0);
	OUT_ACCEL_REG(R200_PP_TXSIZE_1, txsize);
	OUT_ACCEL_REG(R200_PP_TXPITCH_1, txpitch);
	OUT_TEXTURE_REG(R200_PP_TXOFFSET_1, txoffset + pPriv->planeu_offset, src_bo);

	OUT_ACCEL_REG(R200_PP_TXFILTER_2, txfilter);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_2, txformat | R200_TXFORMAT_ST_ROUTE_STQ1);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_X_2, 0);
	OUT_ACCEL_REG(R200_PP_TXSIZE_2, txsize);
	OUT_ACCEL_REG(R200_PP_TXPITCH_2, txpitch);
	OUT_TEXTURE_REG(R200_PP_TXOFFSET_2, txoffset + pPriv->planev_offset, src_bo);

	/* similar to r300 code. Note the big problem is that hardware constants
	 * are 8 bits only, representing 0.0-1.0. We can get that up (using bias
	 * + scale) to -1.0-1.0 (but precision will suffer). AFAIK the hw actually
	 * has 12 bits fractional precision (plus 1 sign bit, 3 range bits) but
	 * the constants not. To get larger range can use output scale, but for
	 * that 2.018 value we need a total scale by 8, which means the constants
	 * really have no accuracy whatsoever (5 fractional bits only).
	 * The only direct way to get high  precision "constants" into the fragment
	 * pipe I know of is to use the texcoord interpolator (not color, this one
	 * is 8 bit only too), which seems a bit expensive. We're lucky though it
	 * seems the values we need seem to fit better than worst case (get about
	 * 6 fractional bits for this instead of 5, at least when not correcting for
	 * hue/saturation/contrast/brightness, which is the same as for vco - yco and
	 * yoff get 8 fractional bits). Try to preserve as much accuracy as possible
	 * even with non-default saturation/hue/contrast/brightness adjustments,
	 * it gets a little crazy and ultimately precision might still be lacking.
	 *
	 * A higher precision (8 fractional bits) version might just put uco into
	 * a texcoord, and calculate a new vcoconst in the shader, like so:
	 * cohelper = {1.0, 0.0, 0.0} - shouldn't use 0.5 since not exactly representable
	 * vco = {1.5958 - 1.0, -0.8129 + 1.0, 1.0}
	 * vcocalc = ADD temp, bias/scale(cohelper), vco
	 * would in total use 4 tex units, 4 instructions which seems fairly
	 * balanced for this architecture (instead of 3 + 3 for the solution here)
	 *
	 * temp = MAD(yco, yuv.yyyy, yoff)
	 * temp = MAD(uco, yuv.uuuu, temp)
	 * result = MAD(vco, yuv.vvvv, temp)
	 *
	 * note first mad produces actually scalar, hence we transform
	 * it into a dp2a to get 8 bit precision of yco instead of 7 -
	 * That's assuming hw correctly expands consts to internal precision.
	 * (y * 1 + y * (yco - 1) + yoff)
	 * temp = DP2A / 2 (yco, yuv.yyyy, yoff)
	 * temp = MAD (uco / 4, yuv.uuuu * 2, temp)
	 * result = MAD x2 (vco / 2, yuv.vvvv, temp)
	 *
	 * vco, uco need bias (and hence scale too)
	 *
	 */

	/* MAD temp0 / 2, const0.a * 2, temp0, -const0.rgb */
	OUT_ACCEL_REG(R200_PP_TXCBLEND_0,
		      R200_TXC_ARG_A_TFACTOR_COLOR |
		      R200_TXC_ARG_B_R0_COLOR |
		      R200_TXC_ARG_C_TFACTOR_COLOR |
		      (yoff < 0 ? R200_TXC_NEG_ARG_C : 0) |
		      R200_TXC_OP_DOT2_ADD);
	OUT_ACCEL_REG(R200_PP_TXCBLEND2_0,
		      (0 << R200_TXC_TFACTOR_SEL_SHIFT) |
		      R200_TXC_SCALE_INV2 |
		      R200_TXC_CLAMP_8_8 | R200_TXC_OUTPUT_REG_R0);
	OUT_ACCEL_REG(R200_PP_TXABLEND_0,
		      R200_TXA_ARG_A_ZERO |
		      R200_TXA_ARG_B_ZERO |
		      R200_TXA_ARG_C_ZERO |
		      R200_TXA_OP_MADD);
	OUT_ACCEL_REG(R200_PP_TXABLEND2_0,
		      R200_TXA_OUTPUT_REG_NONE);

	/* MAD temp0, (const1 - 0.5) * 2, (temp1 - 0.5) * 2, temp0 */
	OUT_ACCEL_REG(R200_PP_TXCBLEND_1,
		      R200_TXC_ARG_A_TFACTOR_COLOR |
		      R200_TXC_BIAS_ARG_A |
		      R200_TXC_SCALE_ARG_A |
		      R200_TXC_ARG_B_R1_COLOR |
		      R200_TXC_BIAS_ARG_B |
		      (needux8 ? R200_TXC_SCALE_ARG_B : 0) |
		      R200_TXC_ARG_C_R0_COLOR |
		      R200_TXC_OP_MADD);
	OUT_ACCEL_REG(R200_PP_TXCBLEND2_1,
		      (1 << R200_TXC_TFACTOR_SEL_SHIFT) |
		      R200_TXC_CLAMP_8_8 | R200_TXC_OUTPUT_REG_R0);
	OUT_ACCEL_REG(R200_PP_TXABLEND_1,
		      R200_TXA_ARG_A_ZERO |
		      R200_TXA_ARG_B_ZERO |
		      R200_TXA_ARG_C_ZERO |
		      R200_TXA_OP_MADD);
	OUT_ACCEL_REG(R200_PP_TXABLEND2_1,
		      R200_TXA_OUTPUT_REG_NONE);

	/* MAD temp0 x 2, (const2 - 0.5) * 2, (temp2 - 0.5), temp0 */
	OUT_ACCEL_REG(R200_PP_TXCBLEND_2,
		      R200_TXC_ARG_A_TFACTOR_COLOR |
		      R200_TXC_BIAS_ARG_A |
		      R200_TXC_SCALE_ARG_A |
		      R200_TXC_ARG_B_R2_COLOR |
		      R200_TXC_BIAS_ARG_B |
		      (needvx8 ? R200_TXC_SCALE_ARG_B : 0) |
		      R200_TXC_ARG_C_R0_COLOR |
		      R200_TXC_OP_MADD);
	OUT_ACCEL_REG(R200_PP_TXCBLEND2_2,
		      (2 << R200_TXC_TFACTOR_SEL_SHIFT) |
		      R200_TXC_SCALE_2X |
		      R200_TXC_CLAMP_0_1 | R200_TXC_OUTPUT_REG_R0);
	OUT_ACCEL_REG(R200_PP_TXABLEND_2,
		      R200_TXA_ARG_A_ZERO |
		      R200_TXA_ARG_B_ZERO |
		      R200_TXA_ARG_C_ZERO |
		      R200_TXA_COMP_ARG_C |
		      R200_TXA_OP_MADD);
	OUT_ACCEL_REG(R200_PP_TXABLEND2_2,
		      R200_TXA_CLAMP_0_1 | R200_TXA_OUTPUT_REG_R0);

	/* shader constants */
	OUT_ACCEL_REG(R200_PP_TFACTOR_0, float4touint(yco > 1.0 ? 1.0 : 0.0, /* range special [0, 2] */
						      yco > 1.0 ? yco - 1.0: yco,
						      yoff < 0 ? -yoff : yoff, /* range special [-1, 1] */
						      0.0));
	OUT_ACCEL_REG(R200_PP_TFACTOR_1, float4touint(uco[0] * ucscale + 0.5, /* range [-4, 4] */
						      uco[1] * ucscale + 0.5, /* or [-2, 2] */
						      uco[2] * ucscale + 0.5,
						      0.0));
	OUT_ACCEL_REG(R200_PP_TFACTOR_2, float4touint(vco[0] * vcscale + 0.5, /* range [-2, 2] */
						      vco[1] * vcscale + 0.5, /* or [-4, 4] */
						      vco[2] * vcscale + 0.5,
						      0.0));

	FINISH_ACCEL();
    } else {
	pPriv->vtx_count = 4;

	BEGIN_ACCEL_RELOC(24, 1);

	OUT_ACCEL_REG(RADEON_PP_CNTL,
		      RADEON_TEX_0_ENABLE |
		      RADEON_TEX_BLEND_0_ENABLE | RADEON_TEX_BLEND_1_ENABLE |
		      RADEON_TEX_BLEND_2_ENABLE);

	OUT_ACCEL_REG(R200_SE_VTX_FMT_0, R200_VTX_XY);
	OUT_ACCEL_REG(R200_SE_VTX_FMT_1,
		      (2 << R200_VTX_TEX0_COMP_CNT_SHIFT));

	OUT_ACCEL_REG(R200_PP_TXFILTER_0, txfilter);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_0, txformat);
	OUT_ACCEL_REG(R200_PP_TXFORMAT_X_0, 0);
	OUT_ACCEL_REG(R200_PP_TXSIZE_0,
		      (pPriv->w - 1) |
		      ((pPriv->h - 1) << RADEON_TEX_VSIZE_SHIFT));
	OUT_ACCEL_REG(R200_PP_TXPITCH_0, pPriv->src_pitch - 32);
	OUT_TEXTURE_REG(R200_PP_TXOFFSET_0, txoffset, src_bo);

	/* MAD temp1 / 2, const0.a * 2, temp0.ggg, -const0.rgb */
	OUT_ACCEL_REG(R200_PP_TXCBLEND_0,
		      R200_TXC_ARG_A_TFACTOR_COLOR |
		      R200_TXC_ARG_B_R0_COLOR |
		      R200_TXC_ARG_C_TFACTOR_COLOR |
		      (yoff < 0 ? R200_TXC_NEG_ARG_C : 0) |
		      R200_TXC_OP_DOT2_ADD);
	OUT_ACCEL_REG(R200_PP_TXCBLEND2_0,
		      (0 << R200_TXC_TFACTOR_SEL_SHIFT) |
		      R200_TXC_SCALE_INV2 |
		      (R200_TXC_REPL_GREEN << R200_TXC_REPL_ARG_B_SHIFT) |
		      R200_TXC_CLAMP_8_8 | R200_TXC_OUTPUT_REG_R1);
	OUT_ACCEL_REG(R200_PP_TXABLEND_0,
		      R200_TXA_ARG_A_ZERO |
		      R200_TXA_ARG_B_ZERO |
		      R200_TXA_ARG_C_ZERO |
		      R200_TXA_OP_MADD);
	OUT_ACCEL_REG(R200_PP_TXABLEND2_0,
		      R200_TXA_OUTPUT_REG_NONE);

	/* MAD temp1, (const1 - 0.5) * 2, (temp0.rrr - 0.5) * 2, temp1 */
	OUT_ACCEL_REG(R200_PP_TXCBLEND_1,
		      R200_TXC_ARG_A_TFACTOR_COLOR |
		      R200_TXC_BIAS_ARG_A |
		      R200_TXC_SCALE_ARG_A |
		      R200_TXC_ARG_B_R0_COLOR |
		      R200_TXC_BIAS_ARG_B |
		      (needux8 ? R200_TXC_SCALE_ARG_B : 0) |
		      R200_TXC_ARG_C_R1_COLOR |
		      R200_TXC_OP_MADD);
	OUT_ACCEL_REG(R200_PP_TXCBLEND2_1,
		      (1 << R200_TXC_TFACTOR_SEL_SHIFT) |
		      (R200_TXC_REPL_BLUE << R200_TXC_REPL_ARG_B_SHIFT) |
		      R200_TXC_CLAMP_8_8 | R200_TXC_OUTPUT_REG_R1);
	OUT_ACCEL_REG(R200_PP_TXABLEND_1,
		      R200_TXA_ARG_A_ZERO |
		      R200_TXA_ARG_B_ZERO |
		      R200_TXA_ARG_C_ZERO |
		      R200_TXA_OP_MADD);
	OUT_ACCEL_REG(R200_PP_TXABLEND2_1,
		      R200_TXA_OUTPUT_REG_NONE);

	/* MAD temp0 x 2, (const2 - 0.5) * 2, (temp0.bbb - 0.5), temp1 */
	OUT_ACCEL_REG(R200_PP_TXCBLEND_2,
		      R200_TXC_ARG_A_TFACTOR_COLOR |
		      R200_TXC_BIAS_ARG_A |
		      R200_TXC_SCALE_ARG_A |
		      R200_TXC_ARG_B_R0_COLOR |
		      R200_TXC_BIAS_ARG_B |
		      (needvx8 ? R200_TXC_SCALE_ARG_B : 0) |
		      R200_TXC_ARG_C_R1_COLOR |
		      R200_TXC_OP_MADD);
	OUT_ACCEL_REG(R200_PP_TXCBLEND2_2,
		      (2 << R200_TXC_TFACTOR_SEL_SHIFT) |
		      R200_TXC_SCALE_2X |
		      (R200_TXC_REPL_RED << R200_TXC_REPL_ARG_B_SHIFT) |
		      R200_TXC_CLAMP_0_1 | R200_TXC_OUTPUT_REG_R0);
	OUT_ACCEL_REG(R200_PP_TXABLEND_2,
		      R200_TXA_ARG_A_ZERO |
		      R200_TXA_ARG_B_ZERO |
		      R200_TXA_ARG_C_ZERO |
		      R200_TXA_COMP_ARG_C |
		      R200_TXA_OP_MADD);
	OUT_ACCEL_REG(R200_PP_TXABLEND2_2,
		      R200_TXA_CLAMP_0_1 | R200_TXA_OUTPUT_REG_R0);

	/* shader constants */
	OUT_ACCEL_REG(R200_PP_TFACTOR_0, float4touint(yco > 1.0 ? 1.0 : 0.0, /* range special [0, 2] */
						      yco > 1.0 ? yco - 1.0: yco,
						      yoff < 0 ? -yoff : yoff, /* range special [-1, 1] */
						      0.0));
	OUT_ACCEL_REG(R200_PP_TFACTOR_1, float4touint(uco[0] * ucscale + 0.5, /* range [-4, 4] */
						      uco[1] * ucscale + 0.5, /* or [-2, 2] */
						      uco[2] * ucscale + 0.5,
						      0.0));
	OUT_ACCEL_REG(R200_PP_TFACTOR_2, float4touint(vco[0] * vcscale + 0.5, /* range [-2, 2] */
						      vco[1] * vcscale + 0.5, /* or [-4, 4] */
						      vco[2] * vcscale + 0.5,
						      0.0));

	FINISH_ACCEL();
    }

    BEGIN_ACCEL(2);
    OUT_ACCEL_REG(RADEON_RE_TOP_LEFT, 0);
    OUT_ACCEL_REG(RADEON_RE_WIDTH_HEIGHT, ((scissor_w << RADEON_RE_WIDTH_SHIFT) |
					   (scissor_h << RADEON_RE_HEIGHT_SHIFT)));
    FINISH_ACCEL();

    if (pPriv->vsync) {
	xf86CrtcPtr crtc;
	if (pPriv->desired_crtc)
	    crtc = pPriv->desired_crtc;
	else
	    crtc = radeon_pick_best_crtc(pScrn,
					 pPriv->drw_x,
					 pPriv->drw_x + pPriv->dst_w,
					 pPriv->drw_y,
					 pPriv->drw_y + pPriv->dst_h);
	if (crtc)
	    FUNC_NAME(RADEONWaitForVLine)(pScrn, pPixmap,
					  crtc,
					  pPriv->drw_y - crtc->y,
					  (pPriv->drw_y - crtc->y) + pPriv->dst_h);
    }

    return TRUE;
}

static void
FUNC_NAME(R200DisplayTexturedVideo)(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    PixmapPtr pPixmap = pPriv->pPixmap;
    int dstxoff, dstyoff;
    BoxPtr pBox = REGION_RECTS(&pPriv->clip);
    int nBox = REGION_NUM_RECTS(&pPriv->clip);
    ACCEL_PREAMBLE();

#ifdef COMPOSITE
    dstxoff = -pPixmap->screen_x + pPixmap->drawable.x;
    dstyoff = -pPixmap->screen_y + pPixmap->drawable.y;
#else
    dstxoff = 0;
    dstyoff = 0;
#endif

    if (!FUNC_NAME(R200PrepareTexturedVideo)(pScrn, pPriv))
	return;

    /*
     * Rendering of the actual polygon is done in two different
     * ways depending on chip generation:
     *
     * < R300:
     *
     *     These chips can render a rectangle in one pass, so
     *     handling is pretty straight-forward.
     *
     * >= R300:
     *
     *     These chips can accept a quad, but will render it as
     *     two triangles which results in a diagonal tear. Instead
     *     We render a single, large triangle and use the scissor
     *     functionality to restrict it to the desired rectangle.
     *     Due to guardband limits on r3xx/r4xx, we can only use
     *     the single triangle up to 2560/4021 pixels; above that we
     *     render as a quad.
     */

#ifdef ACCEL_CP
    while (nBox) {
	int draw_size = 3 * pPriv->vtx_count + 4;
	int loop_boxes;

	if (draw_size > radeon_cs_space_remaining(pScrn)) {
	    if (info->cs)
		radeon_cs_flush_indirect(pScrn);
	    else
		RADEONCPFlushIndirect(pScrn, 1);
	    if (!FUNC_NAME(R200PrepareTexturedVideo)(pScrn, pPriv))
		return;
	}
	loop_boxes = MIN(radeon_cs_space_remaining(pScrn) / draw_size, nBox);
	nBox -= loop_boxes;

	BEGIN_RING(loop_boxes * 3 * pPriv->vtx_count + 4);
	OUT_RING(CP_PACKET3(R200_CP_PACKET3_3D_DRAW_IMMD_2,
			    loop_boxes * 3 * pPriv->vtx_count));
	OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_RECT_LIST |
		 RADEON_CP_VC_CNTL_PRIM_WALK_RING |
		 ((loop_boxes * 3) << RADEON_CP_VC_CNTL_NUM_SHIFT));

	while (loop_boxes--) {
	    float srcX, srcY, srcw, srch;
	    int dstX, dstY, dstw, dsth;
	    dstX = pBox->x1 + dstxoff;
	    dstY = pBox->y1 + dstyoff;
	    dstw = pBox->x2 - pBox->x1;
	    dsth = pBox->y2 - pBox->y1;

	    srcX = pPriv->src_x;
	    srcX += ((pBox->x1 - pPriv->drw_x) *
		     pPriv->src_w) / (float)pPriv->dst_w;
	    srcY = pPriv->src_y;
	    srcY += ((pBox->y1 - pPriv->drw_y) *
		     pPriv->src_h) / (float)pPriv->dst_h;

	    srcw = (pPriv->src_w * dstw) / (float)pPriv->dst_w;
	    srch = (pPriv->src_h * dsth) / (float)pPriv->dst_h;

	    if (pPriv->is_planar) {
		/*
		 * Just render a rect (using three coords).
		 */
		VTX_OUT_6((float)dstX,                     (float)(dstY + dsth),
			  (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h,
			  (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h);
		VTX_OUT_6((float)(dstX + dstw),            (float)(dstY + dsth),
			  (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h,
			  (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h);
		VTX_OUT_6((float)(dstX + dstw),            (float)dstY,
			  (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h,
			  (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h);
	    } else {
		/*
		 * Just render a rect (using three coords).
		 */
		VTX_OUT_4((float)dstX,                     (float)(dstY + dsth),
			  (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h);
		VTX_OUT_4((float)(dstX + dstw),            (float)(dstY + dsth),
			  (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h);
		VTX_OUT_4((float)(dstX + dstw),            (float)dstY,
			  (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h);
	    }

	    pBox++;
	}

	OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);
	ADVANCE_RING();
    }
#else /* ACCEL_CP */
    BEGIN_ACCEL(nBox * 3 * pPriv->vtx_count + 2);
    OUT_ACCEL_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_RECTANGLE_LIST |
				      RADEON_VF_PRIM_WALK_DATA |
				      ((nBox * 3) << RADEON_VF_NUM_VERTICES_SHIFT)));
    while (nBox--) {
	float srcX, srcY, srcw, srch;
	int dstX, dstY, dstw, dsth;
	dstX = pBox->x1 + dstxoff;
	dstY = pBox->y1 + dstyoff;
	dstw = pBox->x2 - pBox->x1;
	dsth = pBox->y2 - pBox->y1;

	srcX = pPriv->src_x;
	srcX += ((pBox->x1 - pPriv->drw_x) *
		 pPriv->src_w) / (float)pPriv->dst_w;
	srcY = pPriv->src_y;
	srcY += ((pBox->y1 - pPriv->drw_y) *
		 pPriv->src_h) / (float)pPriv->dst_h;

	srcw = (pPriv->src_w * dstw) / (float)pPriv->dst_w;
	srch = (pPriv->src_h * dsth) / (float)pPriv->dst_h;

	if (pPriv->is_planar) {
	    /*
	     * Just render a rect (using three coords).
	     */
	    VTX_OUT_6((float)dstX,                     (float)(dstY + dsth),
		      (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h,
		      (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h);
	    VTX_OUT_6((float)(dstX + dstw),            (float)(dstY + dsth),
		      (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h,
		      (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h);
	    VTX_OUT_6((float)(dstX + dstw),            (float)dstY,
		      (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h,
		      (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h);
	} else {
	    /*
	     * Just render a rect (using three coords).
	     */
	    VTX_OUT_4((float)dstX,                     (float)(dstY + dsth),
		      (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h);
	    VTX_OUT_4((float)(dstX + dstw),            (float)(dstY + dsth),
		      (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h);
	    VTX_OUT_4((float)(dstX + dstw),            (float)dstY,
		      (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h);
	}

	pBox++;
    }

    OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);
    FINISH_ACCEL();
#endif /* !ACCEL_CP */

    DamageDamageRegion(pPriv->pDraw, &pPriv->clip);
}

static Bool
FUNC_NAME(R300PrepareTexturedVideo)(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    PixmapPtr pPixmap = pPriv->pPixmap;
    struct radeon_exa_pixmap_priv *driver_priv;
    struct radeon_bo *src_bo = pPriv->src_bo[pPriv->currentBuffer];
    uint32_t txfilter, txformat0, txformat1, txoffset, txpitch;
    uint32_t dst_pitch, dst_format;
    uint32_t txenable, colorpitch, bicubic_offset;
    uint32_t output_fmt;
    int pixel_shift;
    ACCEL_PREAMBLE();

#ifdef XF86DRM_MODE
    if (info->cs) {
	int ret;

	radeon_cs_space_reset_bos(info->cs);
	radeon_cs_space_add_persistent_bo(info->cs, src_bo, RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM, 0);

	if (pPriv->bicubic_enabled)
	  radeon_cs_space_add_persistent_bo(info->cs, info->bicubic_bo, RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM, 0);

	driver_priv = exaGetPixmapDriverPrivate(pPixmap);
	radeon_cs_space_add_persistent_bo(info->cs, driver_priv->bo, 0, RADEON_GEM_DOMAIN_VRAM);

	ret = radeon_cs_space_check(info->cs);
	if (ret) {
	    ErrorF("Not enough RAM to hw accel xv operation\n");
	    return FALSE;
	}
    }
#else
    (void)src_bo;
#endif

    pixel_shift = pPixmap->drawable.bitsPerPixel >> 4;

#ifdef USE_EXA
    if (info->useEXA) {
	dst_pitch = exaGetPixmapPitch(pPixmap);
    } else
#endif
    {
	dst_pitch = pPixmap->devKind;
    }

#ifdef USE_EXA
    if (info->useEXA) {
	RADEON_SWITCH_TO_3D();
    } else
#endif
    {
	BEGIN_ACCEL(2);
	OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D);
	/* We must wait for 3d to idle, in case source was just written as a dest. */
	OUT_ACCEL_REG(RADEON_WAIT_UNTIL,
		      RADEON_WAIT_HOST_IDLECLEAN |
		      RADEON_WAIT_2D_IDLECLEAN |
		      RADEON_WAIT_3D_IDLECLEAN |
		      RADEON_WAIT_DMA_GUI_IDLE);
	FINISH_ACCEL();

	if (!info->accel_state->XInited3D)
	    RADEONInit3DEngine(pScrn);
    }

    if (pPriv->bicubic_enabled)
	pPriv->vtx_count = 6;
    else
	pPriv->vtx_count = 4;

    switch (pPixmap->drawable.bitsPerPixel) {
    case 16:
	if (pPixmap->drawable.depth == 15)
	    dst_format = R300_COLORFORMAT_ARGB1555;
	else
	    dst_format = R300_COLORFORMAT_RGB565;
	break;
    case 32:
	dst_format = R300_COLORFORMAT_ARGB8888;
	break;
    default:
	return FALSE;
    }

    output_fmt = (R300_OUT_FMT_C4_8 |
		  R300_OUT_FMT_C0_SEL_BLUE |
		  R300_OUT_FMT_C1_SEL_GREEN |
		  R300_OUT_FMT_C2_SEL_RED |
		  R300_OUT_FMT_C3_SEL_ALPHA);

    colorpitch = dst_pitch >> pixel_shift;
    colorpitch |= dst_format;

    if (RADEONTilingEnabled(pScrn, pPixmap))
	colorpitch |= R300_COLORTILE;


    if (((pPriv->bicubic_state == BICUBIC_OFF)) &&
	(pPriv->id == FOURCC_I420 || pPriv->id == FOURCC_YV12))
	pPriv->is_planar = TRUE;
    else
	pPriv->is_planar = FALSE;

    if (pPriv->is_planar) {
	txformat1 = R300_TX_FORMAT_X8 | R300_TX_FORMAT_CACHE_HALF_REGION_0;
	txpitch = pPriv->src_pitch;
    } else {
	if (pPriv->id == FOURCC_UYVY)
	    txformat1 = R300_TX_FORMAT_YVYU422;
	else
	    txformat1 = R300_TX_FORMAT_VYUY422;

	if (pPriv->bicubic_state != BICUBIC_OFF)
	    txformat1 |= R300_TX_FORMAT_YUV_TO_RGB_CLAMP;

	/* pitch is in pixels */
	txpitch = pPriv->src_pitch / 2;
    }
    txpitch -= 1;

    txformat0 = ((((pPriv->w - 1) & 0x7ff) << R300_TXWIDTH_SHIFT) |
		 (((pPriv->h - 1) & 0x7ff) << R300_TXHEIGHT_SHIFT) |
		 R300_TXPITCH_EN);

    txfilter = (R300_TX_CLAMP_S(R300_TX_CLAMP_CLAMP_LAST) |
		R300_TX_CLAMP_T(R300_TX_CLAMP_CLAMP_LAST) |
		R300_TX_MAG_FILTER_LINEAR |
		R300_TX_MIN_FILTER_LINEAR |
		(0 << R300_TX_ID_SHIFT));

    txoffset = info->cs ? 0 : pPriv->src_offset;

    BEGIN_ACCEL_RELOC(6, 1);
    OUT_ACCEL_REG(R300_TX_FILTER0_0, txfilter);
    OUT_ACCEL_REG(R300_TX_FILTER1_0, 0);
    OUT_ACCEL_REG(R300_TX_FORMAT0_0, txformat0);
    if (pPriv->is_planar)
	OUT_ACCEL_REG(R300_TX_FORMAT1_0, txformat1 | R300_TX_FORMAT_CACHE_HALF_REGION_0);
    else
	OUT_ACCEL_REG(R300_TX_FORMAT1_0, txformat1);
    OUT_ACCEL_REG(R300_TX_FORMAT2_0, txpitch);
    OUT_TEXTURE_REG(R300_TX_OFFSET_0, txoffset, src_bo);
    FINISH_ACCEL();

    txenable = R300_TEX_0_ENABLE;

    if (pPriv->is_planar) {
	txformat0 = ((((((pPriv->w + 1 ) >> 1) - 1) & 0x7ff) << R300_TXWIDTH_SHIFT) |
		     (((((pPriv->h + 1 ) >> 1 ) - 1) & 0x7ff) << R300_TXHEIGHT_SHIFT) |
		     R300_TXPITCH_EN);
	txpitch = RADEON_ALIGN(pPriv->src_pitch >> 1, 64);
	txpitch -= 1;
	txfilter = (R300_TX_CLAMP_S(R300_TX_CLAMP_CLAMP_LAST) |
		    R300_TX_CLAMP_T(R300_TX_CLAMP_CLAMP_LAST) |
		    R300_TX_MIN_FILTER_LINEAR |
		    R300_TX_MAG_FILTER_LINEAR);

	BEGIN_ACCEL_RELOC(12, 2);
	OUT_ACCEL_REG(R300_TX_FILTER0_1, txfilter | (1 << R300_TX_ID_SHIFT));
	OUT_ACCEL_REG(R300_TX_FILTER1_1, 0);
	OUT_ACCEL_REG(R300_TX_FORMAT0_1, txformat0);
	OUT_ACCEL_REG(R300_TX_FORMAT1_1, R300_TX_FORMAT_X8 | R300_TX_FORMAT_CACHE_FOURTH_REGION_2);
	OUT_ACCEL_REG(R300_TX_FORMAT2_1, txpitch);
	OUT_TEXTURE_REG(R300_TX_OFFSET_1, txoffset + pPriv->planeu_offset, src_bo);
	OUT_ACCEL_REG(R300_TX_FILTER0_2, txfilter | (2 << R300_TX_ID_SHIFT));
	OUT_ACCEL_REG(R300_TX_FILTER1_2, 0);
	OUT_ACCEL_REG(R300_TX_FORMAT0_2, txformat0);
	OUT_ACCEL_REG(R300_TX_FORMAT1_2, R300_TX_FORMAT_X8 | R300_TX_FORMAT_CACHE_FOURTH_REGION_3);
	OUT_ACCEL_REG(R300_TX_FORMAT2_2, txpitch);
	OUT_TEXTURE_REG(R300_TX_OFFSET_2, txoffset + pPriv->planev_offset, src_bo);
	FINISH_ACCEL();
	txenable |= R300_TEX_1_ENABLE | R300_TEX_2_ENABLE;
    }

    if (pPriv->bicubic_enabled) {
	/* Size is 128x1 */
	txformat0 = ((0x7f << R300_TXWIDTH_SHIFT) |
		     (0x0 << R300_TXHEIGHT_SHIFT) |
		     R300_TXPITCH_EN);
	/* Format is 32-bit floats, 4bpp */
	txformat1 = R300_EASY_TX_FORMAT(Z, Y, X, W, FL_R16G16B16A16);
	/* Pitch is 127 (128-1) */
	txpitch = 0x7f;
	/* Tex filter */
	txfilter = (R300_TX_CLAMP_S(R300_TX_CLAMP_WRAP) |
		    R300_TX_CLAMP_T(R300_TX_CLAMP_WRAP) |
		    R300_TX_MIN_FILTER_NEAREST |
		    R300_TX_MAG_FILTER_NEAREST |
		    (1 << R300_TX_ID_SHIFT));

	if (info->cs)
	    bicubic_offset = 0;
	else
	    bicubic_offset = pPriv->bicubic_src_offset;

	BEGIN_ACCEL_RELOC(6, 1);
	OUT_ACCEL_REG(R300_TX_FILTER0_1, txfilter);
	OUT_ACCEL_REG(R300_TX_FILTER1_1, 0);
	OUT_ACCEL_REG(R300_TX_FORMAT0_1, txformat0);
	OUT_ACCEL_REG(R300_TX_FORMAT1_1, txformat1);
	OUT_ACCEL_REG(R300_TX_FORMAT2_1, txpitch);
	OUT_TEXTURE_REG(R300_TX_OFFSET_1, bicubic_offset, info->bicubic_bo);
	FINISH_ACCEL();

	/* Enable tex 1 */
	txenable |= R300_TEX_1_ENABLE;
    }

    /* setup the VAP */
    if (info->accel_state->has_tcl) {
	if (pPriv->bicubic_enabled)
	    BEGIN_ACCEL(7);
	else
	    BEGIN_ACCEL(6);
    } else {
	if (pPriv->bicubic_enabled)
	    BEGIN_ACCEL(5);
	else
	    BEGIN_ACCEL(4);
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
    if (pPriv->bicubic_enabled) {
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
    } else {
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
    }

    /* load the vertex shader
     * We pre-load vertex programs in RADEONInit3DEngine():
     * - exa
     * - Xv
     * - Xv bicubic
     * Here we select the offset of the vertex program we want to use
     */
    if (info->accel_state->has_tcl) {
	if (pPriv->bicubic_enabled) {
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_0,
			  ((11 << R300_PVS_FIRST_INST_SHIFT) |
			   (13 << R300_PVS_XYZW_VALID_INST_SHIFT) |
			   (13 << R300_PVS_LAST_INST_SHIFT)));
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_1,
			  (13 << R300_PVS_LAST_VTX_SRC_INST_SHIFT));
	} else {
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_0,
			  ((9 << R300_PVS_FIRST_INST_SHIFT) |
			   (10 << R300_PVS_XYZW_VALID_INST_SHIFT) |
			   (10 << R300_PVS_LAST_INST_SHIFT)));
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_1,
			  (10 << R300_PVS_LAST_VTX_SRC_INST_SHIFT));
	}
    }

    /* Position and one set of 2 texture coordinates */
    OUT_ACCEL_REG(R300_VAP_OUT_VTX_FMT_0, R300_VTX_POS_PRESENT);
    if (pPriv->bicubic_enabled)
	OUT_ACCEL_REG(R300_VAP_OUT_VTX_FMT_1, ((2 << R300_TEX_0_COMP_CNT_SHIFT) |
					       (2 << R300_TEX_1_COMP_CNT_SHIFT)));
    else
	OUT_ACCEL_REG(R300_VAP_OUT_VTX_FMT_1, (2 << R300_TEX_0_COMP_CNT_SHIFT));

    OUT_ACCEL_REG(R300_US_OUT_FMT_0, output_fmt);
    FINISH_ACCEL();

    /* setup pixel shader */
    if (pPriv->bicubic_state != BICUBIC_OFF) {
	if (pPriv->bicubic_enabled) {
	    BEGIN_ACCEL(79);

	    /* 4 components: 2 for tex0 and 2 for tex1 */
	    OUT_ACCEL_REG(R300_RS_COUNT, ((4 << R300_RS_COUNT_IT_COUNT_SHIFT) |
					  R300_RS_COUNT_HIRES_EN));

	    /* R300_INST_COUNT_RS - highest RS instruction used */
	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(1));

	    /* Pixel stack frame size. */
	    OUT_ACCEL_REG(R300_US_PIXSIZE, 5);

	    /* Indirection levels */
	    OUT_ACCEL_REG(R300_US_CONFIG, ((2 << R300_NLEVEL_SHIFT) |
					   R300_FIRST_TEX));

	    /* Set nodes. */
	    OUT_ACCEL_REG(R300_US_CODE_OFFSET, (R300_ALU_CODE_OFFSET(0) |
						R300_ALU_CODE_SIZE(14) |
						R300_TEX_CODE_OFFSET(0) |
						R300_TEX_CODE_SIZE(6)));

	    /* Nodes are allocated highest first, but executed lowest first */
	    OUT_ACCEL_REG(R300_US_CODE_ADDR_0, 0);
	    OUT_ACCEL_REG(R300_US_CODE_ADDR_1, (R300_ALU_START(0) |
						R300_ALU_SIZE(0) |
						R300_TEX_START(0) |
						R300_TEX_SIZE(0)));
	    OUT_ACCEL_REG(R300_US_CODE_ADDR_2, (R300_ALU_START(1) |
						R300_ALU_SIZE(9) |
						R300_TEX_START(1) |
						R300_TEX_SIZE(0)));
	    OUT_ACCEL_REG(R300_US_CODE_ADDR_3, (R300_ALU_START(11) |
						R300_ALU_SIZE(2) |
						R300_TEX_START(2) |
						R300_TEX_SIZE(3) |
						R300_RGBA_OUT));

	    /* ** BICUBIC FP ** */

	    /* texcoord0 => temp0
	     * texcoord1 => temp1 */

	    // first node
	    /* TEX temp2, temp1.rrr0, tex1, 1D */
	    OUT_ACCEL_REG(R300_US_TEX_INST(0), (R300_TEX_INST(R300_TEX_INST_LD) |
						R300_TEX_ID(1) |
						R300_TEX_SRC_ADDR(1) |
						R300_TEX_DST_ADDR(2)));

	    /* MOV temp1.r, temp1.ggg0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(0), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_GGG) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_1_0) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(0), (R300_ALU_RGB_ADDR0(1) |
						    R300_ALU_RGB_ADDRD(1) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(0), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(0), (R300_ALU_ALPHA_ADDRD(1) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));


	    // second node
	    /* TEX temp1, temp1, tex1, 1D */
	    OUT_ACCEL_REG(R300_US_TEX_INST(1), (R300_TEX_INST(R300_TEX_INST_LD) |
						R300_TEX_ID(1) |
						R300_TEX_SRC_ADDR(1) |
						R300_TEX_DST_ADDR(1)));

	    /* MUL temp3.rg, temp2.ggg0, const0.rgb0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(1), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_GGG) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_RGB) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(1), (R300_ALU_RGB_ADDR0(2) |
						    R300_ALU_RGB_ADDR1(R300_ALU_RGB_CONST(0)) |
						    R300_ALU_RGB_ADDRD(3) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R | R300_ALU_RGB_MASK_G)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(1), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(1), (R300_ALU_ALPHA_ADDRD(3) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));


	    /* MUL temp2.rg, temp2.rrr0, const0.rgb */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(2), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RRR) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_RGB) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(2), (R300_ALU_RGB_ADDR0(2) |
						    R300_ALU_RGB_ADDR1(R300_ALU_RGB_CONST(0)) |
						    R300_ALU_RGB_ADDRD(2) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R | R300_ALU_RGB_MASK_G)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(2), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(2), (R300_ALU_ALPHA_ADDRD(2) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));

	    /* MAD temp4.rg, temp1.ggg0, const1.rgb, temp3.rgb0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(3), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_GGG) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_RGB) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(3), (R300_ALU_RGB_ADDR0(1) |
						    R300_ALU_RGB_ADDR1(R300_ALU_RGB_CONST(1)) |
						    R300_ALU_RGB_ADDR2(3) |
						    R300_ALU_RGB_ADDRD(4) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R | R300_ALU_RGB_MASK_G)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(3), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(3), (R300_ALU_ALPHA_ADDRD(4) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));

	    /* MAD temp5.rg, temp1.ggg0, const1.rgb, temp2.rgb0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(4), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_GGG) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_RGB) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(4), (R300_ALU_RGB_ADDR0(1) |
						    R300_ALU_RGB_ADDR1(R300_ALU_RGB_CONST(1)) |
						    R300_ALU_RGB_ADDR2(2) |
						    R300_ALU_RGB_ADDRD(5) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R | R300_ALU_RGB_MASK_G)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(4), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(4), (R300_ALU_ALPHA_ADDRD(5) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));

	    /* MAD temp3.rg, temp1.rrr0, const1.rgb, temp3.rgb0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(5), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RRR) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_RGB) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(5), (R300_ALU_RGB_ADDR0(1) |
						    R300_ALU_RGB_ADDR1(R300_ALU_RGB_CONST(1)) |
						    R300_ALU_RGB_ADDR2(3) |
						    R300_ALU_RGB_ADDRD(3) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R | R300_ALU_RGB_MASK_G)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(5), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(5), (R300_ALU_ALPHA_ADDRD(3) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));

	    /* MAD temp1.rg, temp1.rrr0, const1.rgb, temp2.rgb0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(6), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RRR) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_RGB) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(6), (R300_ALU_RGB_ADDR0(1) |
						    R300_ALU_RGB_ADDR1(R300_ALU_RGB_CONST(1)) |
						    R300_ALU_RGB_ADDR2(2) |
						    R300_ALU_RGB_ADDRD(1) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R | R300_ALU_RGB_MASK_G)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(6), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(6), (R300_ALU_ALPHA_ADDRD(1) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));

	    /* ADD temp1.rg, temp0.rgb0, temp1.rgb0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(7), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_1_0) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(7), (R300_ALU_RGB_ADDR0(0) |
						    R300_ALU_RGB_ADDR2(1) |
						    R300_ALU_RGB_ADDRD(1) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R | R300_ALU_RGB_MASK_G)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(7), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(7), (R300_ALU_ALPHA_ADDRD(1) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));

	    /* ADD temp2.rg, temp0.rgb0, temp3.rgb0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(8), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_1_0) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(8), (R300_ALU_RGB_ADDR0(0) |
						    R300_ALU_RGB_ADDR2(3) |
						    R300_ALU_RGB_ADDRD(2) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R | R300_ALU_RGB_MASK_G)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(8), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(8), (R300_ALU_ALPHA_ADDRD(2) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));

	    /* ADD temp3.rg, temp0.rgb0, temp5.rgb0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(9), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_1_0) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(9), (R300_ALU_RGB_ADDR0(0) |
						    R300_ALU_RGB_ADDR2(5) |
						    R300_ALU_RGB_ADDRD(3) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R | R300_ALU_RGB_MASK_G)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(9), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(9), (R300_ALU_ALPHA_ADDRD(3) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));

	    /* ADD temp0.rg, temp0.rgb0, temp4.rgb0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(10), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						     R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
						     R300_ALU_RGB_SEL_B(R300_ALU_RGB_1_0) |
						     R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(10), (R300_ALU_RGB_ADDR0(0) |
						     R300_ALU_RGB_ADDR2(4) |
						     R300_ALU_RGB_ADDRD(0) |
						     R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R | R300_ALU_RGB_MASK_G)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(10), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						       R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						       R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						       R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(10), (R300_ALU_ALPHA_ADDRD(0) |
						       R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));


	    // third node
	    /* TEX temp4, temp1.rg--, tex0, 1D */
	    OUT_ACCEL_REG(R300_US_TEX_INST(2), (R300_TEX_INST(R300_TEX_INST_LD) |
						R300_TEX_ID(0) |
						R300_TEX_SRC_ADDR(1) |
						R300_TEX_DST_ADDR(4)));

	    /* TEX temp3, temp3.rg--, tex0, 1D */
	    OUT_ACCEL_REG(R300_US_TEX_INST(3), (R300_TEX_INST(R300_TEX_INST_LD) |
						R300_TEX_ID(0) |
						R300_TEX_SRC_ADDR(3) |
						R300_TEX_DST_ADDR(3)));

	    /* TEX temp5, temp2.rg--, tex0, 1D */
	    OUT_ACCEL_REG(R300_US_TEX_INST(4), (R300_TEX_INST(R300_TEX_INST_LD) |
						R300_TEX_ID(0) |
						R300_TEX_SRC_ADDR(2) |
						R300_TEX_DST_ADDR(5)));

	    /* TEX temp0, temp0.rg--, tex0, 1D */
	    OUT_ACCEL_REG(R300_US_TEX_INST(5), (R300_TEX_INST(R300_TEX_INST_LD) |
						R300_TEX_ID(0) |
						R300_TEX_SRC_ADDR(0) |
						R300_TEX_DST_ADDR(0)));

	    /* LRP temp3, temp1.bbbb, temp4, temp3 ->
	     * - PRESUB temps, temp4 - temp3
	     * - MAD temp3, temp1.bbbb, temps, temp3 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(11), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						     R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC2_BBB) |
						     R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRCP_RGB) |
						     R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC0_RGB) |
						     R300_ALU_RGB_SRCP_OP(R300_ALU_RGB_SRCP_OP_RGB1_MINUS_RGB0)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(11), (R300_ALU_RGB_ADDR0(3) |
						     R300_ALU_RGB_ADDR1(4) |
						     R300_ALU_RGB_ADDR2(1) |
						     R300_ALU_RGB_ADDRD(3) |
						     R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(11), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						       R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC2_B) |
						       R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_SRCP_A) |
						       R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_SRC0_A)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(11), (R300_ALU_ALPHA_ADDR0(3) |
						       R300_ALU_ALPHA_ADDR1(4) |
						       R300_ALU_ALPHA_ADDR2(1) |
						       R300_ALU_ALPHA_ADDRD(3) |
						       R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_A)));

	    /* LRP temp0, temp1.bbbb, temp5, temp0 ->
	     * - PRESUB temps, temp5 - temp0
	     * - MAD temp0, temp1.bbbb, temps, temp0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(12), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						     R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC2_BBB) |
						     R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRCP_RGB) |
						     R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC0_RGB) |
						     R300_ALU_RGB_SRCP_OP(R300_ALU_RGB_SRCP_OP_RGB1_MINUS_RGB0) |
						     R300_ALU_RGB_INSERT_NOP));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(12), (R300_ALU_RGB_ADDR0(0) |
						     R300_ALU_RGB_ADDR1(5) |
						     R300_ALU_RGB_ADDR2(1) |
						     R300_ALU_RGB_ADDRD(0) |
						     R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(12), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						       R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC2_B) |
						       R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_SRCP_A) |
						       R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_SRC0_A)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(12), (R300_ALU_ALPHA_ADDR0(0) |
						       R300_ALU_ALPHA_ADDR1(5) |
						       R300_ALU_ALPHA_ADDR2(1) |
						       R300_ALU_ALPHA_ADDRD(0) |
						       R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_A)));

	    /* LRP output, temp2.bbbb, temp3, temp0 ->
	     * - PRESUB temps, temp3 - temp0
	     * - MAD output, temp2.bbbb, temps, temp0 */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(13), (R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						     R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC2_BBB) |
						     R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRCP_RGB) |
						     R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC0_RGB) |
						     R300_ALU_RGB_SRCP_OP(R300_ALU_RGB_SRCP_OP_RGB1_MINUS_RGB0)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(13), (R300_ALU_RGB_ADDR0(0) |
						     R300_ALU_RGB_ADDR1(3) |
						     R300_ALU_RGB_ADDR2(2) |
						     R300_ALU_RGB_OMASK(R300_ALU_RGB_MASK_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(13), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						       R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC2_B) |
						       R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_SRCP_A) |
						       R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_SRC0_A)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(13), (R300_ALU_ALPHA_ADDR0(0) |
						       R300_ALU_ALPHA_ADDR1(3) |
						       R300_ALU_ALPHA_ADDR2(2) |
						       R300_ALU_ALPHA_OMASK(R300_ALU_ALPHA_MASK_A)));

	    /* Shader constants. */
	    OUT_ACCEL_REG(R300_US_ALU_CONST_R(0), F_TO_24(1.0/(float)pPriv->w));
	    OUT_ACCEL_REG(R300_US_ALU_CONST_G(0), 0);
	    OUT_ACCEL_REG(R300_US_ALU_CONST_B(0), 0);
	    OUT_ACCEL_REG(R300_US_ALU_CONST_A(0), 0);

	    OUT_ACCEL_REG(R300_US_ALU_CONST_R(1), 0);
	    OUT_ACCEL_REG(R300_US_ALU_CONST_G(1), F_TO_24(1.0/(float)pPriv->h));
	    OUT_ACCEL_REG(R300_US_ALU_CONST_B(1), 0);
	    OUT_ACCEL_REG(R300_US_ALU_CONST_A(1), 0);

	    FINISH_ACCEL();
	} else {
	    BEGIN_ACCEL(11);
	    /* 2 components: 2 for tex0 */
	    OUT_ACCEL_REG(R300_RS_COUNT,
                          ((2 << R300_RS_COUNT_IT_COUNT_SHIFT) |
                           R300_RS_COUNT_HIRES_EN));
	    /* R300_INST_COUNT_RS - highest RS instruction used */
	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(0));

	    OUT_ACCEL_REG(R300_US_PIXSIZE, 0); /* highest temp used */

	    /* Indirection levels */
	    OUT_ACCEL_REG(R300_US_CONFIG, ((0 << R300_NLEVEL_SHIFT) |
					   R300_FIRST_TEX));

	    OUT_ACCEL_REG(R300_US_CODE_OFFSET, (R300_ALU_CODE_OFFSET(0) |
						R300_ALU_CODE_SIZE(1) |
						R300_TEX_CODE_OFFSET(0) |
						R300_TEX_CODE_SIZE(1)));

	    OUT_ACCEL_REG(R300_US_CODE_ADDR_3, (R300_ALU_START(0) |
						R300_ALU_SIZE(0) |
						R300_TEX_START(0) |
						R300_TEX_SIZE(0) |
						R300_RGBA_OUT));

	    /* tex inst */
	    OUT_ACCEL_REG(R300_US_TEX_INST_0, (R300_TEX_SRC_ADDR(0) |
					       R300_TEX_DST_ADDR(0) |
					       R300_TEX_ID(0) |
					       R300_TEX_INST(R300_TEX_INST_LD)));

	    /* ALU inst */
	    /* RGB */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR_0, (R300_ALU_RGB_ADDR0(0) |
                                                   R300_ALU_RGB_ADDR1(0) |
                                                   R300_ALU_RGB_ADDR2(0) |
                                                   R300_ALU_RGB_ADDRD(0) |
                                                   R300_ALU_RGB_OMASK((R300_ALU_RGB_MASK_R |
								       R300_ALU_RGB_MASK_G |
								       R300_ALU_RGB_MASK_B)) |
                                                   R300_ALU_RGB_TARGET_A));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST_0, (R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
                                                   R300_ALU_RGB_MOD_A(R300_ALU_RGB_MOD_NOP) |
                                                   R300_ALU_RGB_SEL_B(R300_ALU_RGB_1_0) |
						   R300_ALU_RGB_MOD_B(R300_ALU_RGB_MOD_NOP) |
                                                   R300_ALU_RGB_SEL_C(R300_ALU_RGB_0_0) |
                                                   R300_ALU_RGB_MOD_C(R300_ALU_RGB_MOD_NOP) |
                                                   R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
                                                   R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE) |
                                                   R300_ALU_RGB_CLAMP));
	    /* Alpha */
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR_0, (R300_ALU_ALPHA_ADDR0(0) |
						     R300_ALU_ALPHA_ADDR1(0) |
						     R300_ALU_ALPHA_ADDR2(0) |
						     R300_ALU_ALPHA_ADDRD(0) |
						     R300_ALU_ALPHA_OMASK(R300_ALU_ALPHA_MASK_A) |
						     R300_ALU_ALPHA_TARGET_A |
						     R300_ALU_ALPHA_OMASK_W(R300_ALU_ALPHA_MASK_NONE)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST_0, (R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_A) |
						     R300_ALU_ALPHA_MOD_A(R300_ALU_ALPHA_MOD_NOP) |
						     R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_1_0) |
						     R300_ALU_ALPHA_MOD_B(R300_ALU_ALPHA_MOD_NOP) |
						     R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0) |
						     R300_ALU_ALPHA_MOD_C(R300_ALU_ALPHA_MOD_NOP) |
						     R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						     R300_ALU_ALPHA_OMOD(R300_ALU_ALPHA_OMOD_NONE) |
						     R300_ALU_ALPHA_CLAMP));
	    FINISH_ACCEL();
	}
    } else {
	/*
	 * y' = y - .0625
	 * u' = u - .5
	 * v' = v - .5;
	 *
	 * r = 1.1643 * y' + 0.0     * u' + 1.5958  * v'
	 * g = 1.1643 * y' - 0.39173 * u' - 0.81290 * v'
	 * b = 1.1643 * y' + 2.017   * u' + 0.0     * v'
	 *
	 * DP3 might look like the straightforward solution
	 * but we'd need to move the texture yuv values in
	 * the same reg for this to work. Therefore use MADs.
	 * Brightness just adds to the off constant.
	 * Contrast is multiplication of luminance.
	 * Saturation and hue change the u and v coeffs.
	 * Default values (before adjustments - depend on colorspace):
	 * yco = 1.1643
	 * uco = 0, -0.39173, 2.017
	 * vco = 1.5958, -0.8129, 0
	 * off = -0.0625 * yco + -0.5 * uco[r] + -0.5 * vco[r],
	 *       -0.0625 * yco + -0.5 * uco[g] + -0.5 * vco[g],
	 *       -0.0625 * yco + -0.5 * uco[b] + -0.5 * vco[b],
	 *
	 * temp = MAD(yco, yuv.yyyy, off)
	 * temp = MAD(uco, yuv.uuuu, temp)
	 * result = MAD(vco, yuv.vvvv, temp)
	 */
	/* TODO: don't recalc consts always */
	const float Loff = -0.0627;
	const float Coff = -0.502;
	float uvcosf, uvsinf;
	float yco;
	float uco[3], vco[3], off[3];
	float bright, cont, gamma;
	int ref = pPriv->transform_index;
	Bool needgamma = FALSE;

	cont = RTFContrast(pPriv->contrast);
	bright = RTFBrightness(pPriv->brightness);
	gamma = (float)pPriv->gamma / 1000.0;
	uvcosf = RTFSaturation(pPriv->saturation) * cos(RTFHue(pPriv->hue));
	uvsinf = RTFSaturation(pPriv->saturation) * sin(RTFHue(pPriv->hue));
	/* overlay video also does pre-gamma contrast/sat adjust, should we? */

	yco = trans[ref].RefLuma * cont;
	uco[0] = -trans[ref].RefRCr * uvsinf;
	uco[1] = trans[ref].RefGCb * uvcosf - trans[ref].RefGCr * uvsinf;
	uco[2] = trans[ref].RefBCb * uvcosf;
	vco[0] = trans[ref].RefRCr * uvcosf;
	vco[1] = trans[ref].RefGCb * uvsinf + trans[ref].RefGCr * uvcosf;
	vco[2] = trans[ref].RefBCb * uvsinf;
	off[0] = Loff * yco + Coff * (uco[0] + vco[0]) + bright;
	off[1] = Loff * yco + Coff * (uco[1] + vco[1]) + bright;
	off[2] = Loff * yco + Coff * (uco[2] + vco[2]) + bright;

	if (gamma != 1.0) {
	    needgamma = TRUE;
	    /* note: gamma correction is out = in ^ gamma;
	       gpu can only do LG2/EX2 therefore we transform into
	       in ^ gamma = 2 ^ (log2(in) * gamma).
	       Lots of scalar ops, unfortunately (better solution?) -
	       without gamma that's 3 inst, with gamma it's 10...
	       could use different gamma factors per channel,
	       if that's of any use. */
	}

	if (pPriv->is_planar) {
	    BEGIN_ACCEL(needgamma ? 28 + 33 : 33);
	    /* 2 components: same 2 for tex0/1/2 */
	    OUT_ACCEL_REG(R300_RS_COUNT,
			  ((2 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));
	    /* R300_INST_COUNT_RS - highest RS instruction used */
	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(0));

	    OUT_ACCEL_REG(R300_US_PIXSIZE, 2); /* highest temp used */

	    /* Indirection levels */
	    OUT_ACCEL_REG(R300_US_CONFIG, ((0 << R300_NLEVEL_SHIFT) |
					   R300_FIRST_TEX));

	    OUT_ACCEL_REG(R300_US_CODE_OFFSET, (R300_ALU_CODE_OFFSET(0) |
						R300_ALU_CODE_SIZE(needgamma ? 7 + 3 : 3) |
						R300_TEX_CODE_OFFSET(0) |
						R300_TEX_CODE_SIZE(3)));

	    OUT_ACCEL_REG(R300_US_CODE_ADDR_3, (R300_ALU_START(0) |
						R300_ALU_SIZE(needgamma ? 7 + 2 : 2) |
						R300_TEX_START(0) |
						R300_TEX_SIZE(2) |
						R300_RGBA_OUT));

	    /* tex inst */
	    OUT_ACCEL_REG(R300_US_TEX_INST_0, (R300_TEX_SRC_ADDR(0) |
					       R300_TEX_DST_ADDR(2) |
					       R300_TEX_ID(0) |
					       R300_TEX_INST(R300_TEX_INST_LD)));
	    OUT_ACCEL_REG(R300_US_TEX_INST_1, (R300_TEX_SRC_ADDR(0) |
					       R300_TEX_DST_ADDR(1) |
					       R300_TEX_ID(1) |
					       R300_TEX_INST(R300_TEX_INST_LD)));
	    OUT_ACCEL_REG(R300_US_TEX_INST_2, (R300_TEX_SRC_ADDR(0) |
					       R300_TEX_DST_ADDR(0) |
					       R300_TEX_ID(2) |
					       R300_TEX_INST(R300_TEX_INST_LD)));

	    /* ALU inst */
	    /* MAD temp2.rgb, const0.aaa, temp2.rgb, const0.rgb */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(0), (R300_ALU_RGB_ADDR0(R300_ALU_RGB_CONST(0)) |
						    R300_ALU_RGB_ADDR1(2) |
						    R300_ALU_RGB_ADDR2(0) |
						    R300_ALU_RGB_ADDRD(2) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(0), (R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_AAA) |
						    R300_ALU_RGB_MOD_A(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_RGB) |
						    R300_ALU_RGB_MOD_B(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC0_RGB) |
						    R300_ALU_RGB_MOD_C(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE)));
	    /* alpha nop, but need to set up alpha source for rgb usage */
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(0), (R300_ALU_ALPHA_ADDR0(R300_ALU_ALPHA_CONST(0)) |
						      R300_ALU_ALPHA_ADDR1(2) |
						      R300_ALU_ALPHA_ADDR2(0) |
						      R300_ALU_ALPHA_ADDRD(2) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(0), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

	    /* MAD temp2.rgb, const1.rgb, temp1.rgb, temp2.rgb */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(1), (R300_ALU_RGB_ADDR0(R300_ALU_RGB_CONST(1)) |
						    R300_ALU_RGB_ADDR1(1) |
						    R300_ALU_RGB_ADDR2(2) |
						    R300_ALU_RGB_ADDRD(2) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(1), (R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
						    R300_ALU_RGB_MOD_A(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_RGB) |
						    R300_ALU_RGB_MOD_B(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB) |
						    R300_ALU_RGB_MOD_C(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE)));
	    /* alpha nop */
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(1), (R300_ALU_ALPHA_ADDRD(2) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(1), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

	    /* MAD result.rgb, const2.rgb, temp0.rgb, temp2.rgb */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(2), (R300_ALU_RGB_ADDR0(R300_ALU_RGB_CONST(2)) |
						    R300_ALU_RGB_ADDR1(0) |
						    R300_ALU_RGB_ADDR2(2) |
						    R300_ALU_RGB_ADDRD(0) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_RGB) |
						    (needgamma ? 0 : R300_ALU_RGB_OMASK(R300_ALU_RGB_MASK_RGB))));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(2), (R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
						    R300_ALU_RGB_MOD_A(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_RGB) |
						    R300_ALU_RGB_MOD_B(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB) |
						    R300_ALU_RGB_MOD_C(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE) |
						    R300_ALU_RGB_CLAMP));
	    /* write alpha 1 */
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(2), (R300_ALU_ALPHA_ADDRD(0) |
						      R300_ALU_ALPHA_OMASK(R300_ALU_ALPHA_MASK_A) |
						      R300_ALU_ALPHA_TARGET_A));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(2), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_1_0)));

	    if (needgamma) {
		/* rgb temp0.r = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(3), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(3),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha lg2 temp0, temp0.r */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(3), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(3), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_LN2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_R) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* rgb temp0.g = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(4), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_G)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(4),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha lg2 temp0, temp0.g */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(4), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(4), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_LN2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_G) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* rgb temp0.b = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(5), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_B)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(5),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha lg2 temp0, temp0.b */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(5), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(5), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_LN2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_B) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* MUL const1, temp1, temp0 */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(6), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_ADDR1(0) |
							R300_ALU_RGB_ADDR2(0) |
							R300_ALU_RGB_ADDRD(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_RGB)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(6), (R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
							R300_ALU_RGB_MOD_A(R300_ALU_RGB_MOD_NOP) |
							R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC0_AAA) |
							R300_ALU_RGB_MOD_B(R300_ALU_RGB_MOD_NOP) |
							R300_ALU_RGB_SEL_C(R300_ALU_RGB_0_0) |
							R300_ALU_RGB_MOD_C(R300_ALU_RGB_MOD_NOP) |
							R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
							R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE)));
		/* alpha nop, but set up const1 */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(6), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_ADDR0(R300_ALU_ALPHA_CONST(1)) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(6), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* rgb out0.r = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(7), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R) |
							R300_ALU_RGB_OMASK(R300_ALU_RGB_MASK_R)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(7),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha ex2 temp0, temp0.r */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(7), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(7), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_EX2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_R) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* rgb out0.g = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(8), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_G) |
							R300_ALU_RGB_OMASK(R300_ALU_RGB_MASK_G)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(8),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha ex2 temp0, temp0.g */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(8), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(8), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_EX2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_G) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* rgb out0.b = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(9), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_B) |
							R300_ALU_RGB_OMASK(R300_ALU_RGB_MASK_B)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(9),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha ex2 temp0, temp0.b */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(9), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(9), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_EX2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_B) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    }
	} else {
	    BEGIN_ACCEL(needgamma ? 28 + 31 : 31);
	    /* 2 components */
	    OUT_ACCEL_REG(R300_RS_COUNT,
			  ((2 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));
	    /* R300_INST_COUNT_RS - highest RS instruction used */
	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(0));

	    OUT_ACCEL_REG(R300_US_PIXSIZE, 1); /* highest temp used */

	    /* Indirection levels */
	    OUT_ACCEL_REG(R300_US_CONFIG, ((0 << R300_NLEVEL_SHIFT) |
					   R300_FIRST_TEX));

	    OUT_ACCEL_REG(R300_US_CODE_OFFSET, (R300_ALU_CODE_OFFSET(0) |
						R300_ALU_CODE_SIZE(needgamma ? 7 + 3 : 3) |
						R300_TEX_CODE_OFFSET(0) |
						R300_TEX_CODE_SIZE(1)));

	    OUT_ACCEL_REG(R300_US_CODE_ADDR_3, (R300_ALU_START(0) |
						R300_ALU_SIZE(needgamma ? 7 + 2 : 2) |
						R300_TEX_START(0) |
						R300_TEX_SIZE(0) |
						R300_RGBA_OUT));

	    /* tex inst */
	    OUT_ACCEL_REG(R300_US_TEX_INST_0, (R300_TEX_SRC_ADDR(0) |
					       R300_TEX_DST_ADDR(0) |
					       R300_TEX_ID(0) |
					       R300_TEX_INST(R300_TEX_INST_LD)));

	    /* ALU inst */
	    /* MAD temp1.rgb, const0.aaa, temp0.ggg, const0.rgb */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(0), (R300_ALU_RGB_ADDR0(R300_ALU_RGB_CONST(0)) |
						    R300_ALU_RGB_ADDR1(0) |
						    R300_ALU_RGB_ADDR2(0) |
						    R300_ALU_RGB_ADDRD(1) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(0), (R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_AAA) |
						    R300_ALU_RGB_MOD_A(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_GGG) |
						    R300_ALU_RGB_MOD_B(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC0_RGB) |
						    R300_ALU_RGB_MOD_C(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE)));
	    /* alpha nop, but need to set up alpha source for rgb usage */
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(0), (R300_ALU_ALPHA_ADDR0(R300_ALU_ALPHA_CONST(0)) |
						      R300_ALU_ALPHA_ADDR1(0) |
						      R300_ALU_ALPHA_ADDR2(0) |
						      R300_ALU_ALPHA_ADDRD(0) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(0), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

	    /* MAD temp1.rgb, const1.rgb, temp0.bbb, temp1.rgb */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(1), (R300_ALU_RGB_ADDR0(R300_ALU_RGB_CONST(1)) |
						    R300_ALU_RGB_ADDR1(0) |
						    R300_ALU_RGB_ADDR2(1) |
						    R300_ALU_RGB_ADDRD(1) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_RGB)));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(1), (R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
						    R300_ALU_RGB_MOD_A(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_BBB) |
						    R300_ALU_RGB_MOD_B(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB) |
						    R300_ALU_RGB_MOD_C(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE)));
	    /* alpha nop */
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(1), (R300_ALU_ALPHA_ADDRD(0) |
						      R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(1), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

	    /* MAD result.rgb, const2.rgb, temp0.rrr, temp1.rgb */
	    OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(2), (R300_ALU_RGB_ADDR0(R300_ALU_RGB_CONST(2)) |
						    R300_ALU_RGB_ADDR1(0) |
						    R300_ALU_RGB_ADDR2(1) |
						    R300_ALU_RGB_ADDRD(0) |
						    R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_RGB) |
						    (needgamma ? 0 : R300_ALU_RGB_OMASK(R300_ALU_RGB_MASK_RGB))));
	    OUT_ACCEL_REG(R300_US_ALU_RGB_INST(2), (R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
						    R300_ALU_RGB_MOD_A(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC1_RRR) |
						    R300_ALU_RGB_MOD_B(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_SEL_C(R300_ALU_RGB_SRC2_RGB) |
						    R300_ALU_RGB_MOD_C(R300_ALU_RGB_MOD_NOP) |
						    R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
						    R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE) |
						    R300_ALU_RGB_CLAMP));
	    /* write alpha 1 */
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(2), (R300_ALU_ALPHA_ADDRD(0) |
						      R300_ALU_ALPHA_OMASK(R300_ALU_ALPHA_MASK_A) |
						      R300_ALU_ALPHA_TARGET_A));
	    OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(2), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
						      R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
						      R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_1_0)));

	    if (needgamma) {
		/* rgb temp0.r = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(3), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(3),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha lg2 temp0, temp0.r */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(3), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(3), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_LN2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_R) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* rgb temp0.g = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(4), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_G)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(4),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha lg2 temp0, temp0.g */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(4), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(4), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_LN2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_G) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* rgb temp0.b = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(5), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_B)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(5),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha lg2 temp0, temp0.b */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(5), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(5), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_LN2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_B) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* MUL const1, temp1, temp0 */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(6), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_ADDR1(0) |
							R300_ALU_RGB_ADDR2(0) |
							R300_ALU_RGB_ADDRD(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_RGB)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(6), (R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
							R300_ALU_RGB_MOD_A(R300_ALU_RGB_MOD_NOP) |
							R300_ALU_RGB_SEL_B(R300_ALU_RGB_SRC0_AAA) |
							R300_ALU_RGB_MOD_B(R300_ALU_RGB_MOD_NOP) |
							R300_ALU_RGB_SEL_C(R300_ALU_RGB_0_0) |
							R300_ALU_RGB_MOD_C(R300_ALU_RGB_MOD_NOP) |
							R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
							R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE)));
		/* alpha nop, but set up const1 */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(6), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_ADDR0(R300_ALU_ALPHA_CONST(1)) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(6), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* rgb out0.r = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(7), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_R) |
							R300_ALU_RGB_OMASK(R300_ALU_RGB_MASK_R)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(7),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha ex2 temp0, temp0.r */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(7), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(7), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_EX2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_R) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* rgb out0.g = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(8), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_G) |
							R300_ALU_RGB_OMASK(R300_ALU_RGB_MASK_G)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(8),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha ex2 temp0, temp0.g */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(8), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(8), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_EX2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_G) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));

		/* rgb out0.b = op_sop, set up src0 reg */
		OUT_ACCEL_REG(R300_US_ALU_RGB_ADDR(9), (R300_ALU_RGB_ADDR0(0) |
							R300_ALU_RGB_WMASK(R300_ALU_RGB_MASK_B) |
							R300_ALU_RGB_OMASK(R300_ALU_RGB_MASK_B)));
		OUT_ACCEL_REG(R300_US_ALU_RGB_INST(9),
			      R300_ALU_RGB_OP(R300_ALU_RGB_OP_SOP) |
			      R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE));
		/* alpha ex2 temp0, temp0.b */
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_ADDR(9), (R300_ALU_ALPHA_ADDRD(0) |
							  R300_ALU_ALPHA_WMASK(R300_ALU_ALPHA_MASK_NONE)));
		OUT_ACCEL_REG(R300_US_ALU_ALPHA_INST(9), (R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_EX2) |
							  R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_B) |
							  R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_0_0) |
							  R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0)));
	    }
	}

	/* Shader constants. */
	/* constant 0: off, yco */
	OUT_ACCEL_REG(R300_US_ALU_CONST_R(0), F_TO_24(off[0]));
	OUT_ACCEL_REG(R300_US_ALU_CONST_G(0), F_TO_24(off[1]));
	OUT_ACCEL_REG(R300_US_ALU_CONST_B(0), F_TO_24(off[2]));
	OUT_ACCEL_REG(R300_US_ALU_CONST_A(0), F_TO_24(yco));
	/* constant 1: uco */
	OUT_ACCEL_REG(R300_US_ALU_CONST_R(1), F_TO_24(uco[0]));
	OUT_ACCEL_REG(R300_US_ALU_CONST_G(1), F_TO_24(uco[1]));
	OUT_ACCEL_REG(R300_US_ALU_CONST_B(1), F_TO_24(uco[2]));
	OUT_ACCEL_REG(R300_US_ALU_CONST_A(1), F_TO_24(gamma));
	/* constant 2: vco */
	OUT_ACCEL_REG(R300_US_ALU_CONST_R(2), F_TO_24(vco[0]));
	OUT_ACCEL_REG(R300_US_ALU_CONST_G(2), F_TO_24(vco[1]));
	OUT_ACCEL_REG(R300_US_ALU_CONST_B(2), F_TO_24(vco[2]));
	OUT_ACCEL_REG(R300_US_ALU_CONST_A(2), F_TO_24(0.0));

	FINISH_ACCEL();
    }

    BEGIN_ACCEL_RELOC(6, 2);
    OUT_ACCEL_REG(R300_TX_INVALTAGS, 0);
    OUT_ACCEL_REG(R300_TX_ENABLE, txenable);

    EMIT_WRITE_OFFSET(R300_RB3D_COLOROFFSET0, 0, pPixmap);
    EMIT_COLORPITCH(R300_RB3D_COLORPITCH0, colorpitch, pPixmap);

    /* no need to enable blending */
    OUT_ACCEL_REG(R300_RB3D_BLENDCNTL, RADEON_SRC_BLEND_GL_ONE | RADEON_DST_BLEND_GL_ZERO);

    OUT_ACCEL_REG(R300_VAP_VTX_SIZE, pPriv->vtx_count);
    FINISH_ACCEL();

    if (pPriv->vsync) {
	xf86CrtcPtr crtc;
	if (pPriv->desired_crtc)
	    crtc = pPriv->desired_crtc;
	else
	    crtc = radeon_pick_best_crtc(pScrn,
					 pPriv->drw_x,
					 pPriv->drw_x + pPriv->dst_w,
					 pPriv->drw_y,
					 pPriv->drw_y + pPriv->dst_h);
	if (crtc)
	    FUNC_NAME(RADEONWaitForVLine)(pScrn, pPixmap,
					  crtc,
					  pPriv->drw_y - crtc->y,
					  (pPriv->drw_y - crtc->y) + pPriv->dst_h);
    }

    return TRUE;
}

static void
FUNC_NAME(R300DisplayTexturedVideo)(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    PixmapPtr pPixmap = pPriv->pPixmap;
    int dstxoff, dstyoff;
    BoxPtr pBox = REGION_RECTS(&pPriv->clip);
    int nBox = REGION_NUM_RECTS(&pPriv->clip);
    ACCEL_PREAMBLE();

#ifdef COMPOSITE
    dstxoff = -pPixmap->screen_x + pPixmap->drawable.x;
    dstyoff = -pPixmap->screen_y + pPixmap->drawable.y;
#else
    dstxoff = 0;
    dstyoff = 0;
#endif

    if (!FUNC_NAME(R300PrepareTexturedVideo)(pScrn, pPriv))
	return;

    /*
     * Rendering of the actual polygon is done in two different
     * ways depending on chip generation:
     *
     * < R300:
     *
     *     These chips can render a rectangle in one pass, so
     *     handling is pretty straight-forward.
     *
     * >= R300:
     *
     *     These chips can accept a quad, but will render it as
     *     two triangles which results in a diagonal tear. Instead
     *     We render a single, large triangle and use the scissor
     *     functionality to restrict it to the desired rectangle.
     *     Due to guardband limits on r3xx/r4xx, we can only use
     *     the single triangle up to 2560/4021 pixels; above that we
     *     render as a quad.
     */

    while (nBox--) {
	float srcX, srcY, srcw, srch;
	int dstX, dstY, dstw, dsth;
	Bool use_quad = FALSE;
#ifdef ACCEL_CP
	int draw_size = 4 * pPriv->vtx_count + 4 + 2 + 3;

	if (draw_size > radeon_cs_space_remaining(pScrn)) {
	    if (info->cs)
		radeon_cs_flush_indirect(pScrn);
	    else
		RADEONCPFlushIndirect(pScrn, 1);
	    if (!FUNC_NAME(R300PrepareTexturedVideo)(pScrn, pPriv))
		return;
	}
#endif

	dstX = pBox->x1 + dstxoff;
	dstY = pBox->y1 + dstyoff;
	dstw = pBox->x2 - pBox->x1;
	dsth = pBox->y2 - pBox->y1;

	srcX = pPriv->src_x;
	srcX += ((pBox->x1 - pPriv->drw_x) *
		 pPriv->src_w) / (float)pPriv->dst_w;
	srcY = pPriv->src_y;
	srcY += ((pBox->y1 - pPriv->drw_y) *
		 pPriv->src_h) / (float)pPriv->dst_h;

	srcw = (pPriv->src_w * dstw) / (float)pPriv->dst_w;
	srch = (pPriv->src_h * dsth) / (float)pPriv->dst_h;

	if (IS_R400_3D) {
	    if ((dstw+dsth) > 4021)
		use_quad = TRUE;
	} else {
	    if ((dstw+dsth) > 2560)
		use_quad = TRUE;
	}
	/*
	 * Set up the scissor area to that of the output size.
	 */
	BEGIN_ACCEL(2);
	/* R300 has an offset */
	OUT_ACCEL_REG(R300_SC_SCISSOR0, (((dstX + 1440) << R300_SCISSOR_X_SHIFT) |
					 ((dstY + 1440) << R300_SCISSOR_Y_SHIFT)));
	OUT_ACCEL_REG(R300_SC_SCISSOR1, (((dstX + dstw + 1440 - 1) << R300_SCISSOR_X_SHIFT) |
					 ((dstY + dsth + 1440 - 1) << R300_SCISSOR_Y_SHIFT)));
	FINISH_ACCEL();

#ifdef ACCEL_CP
	if (use_quad) {
	    BEGIN_RING(4 * pPriv->vtx_count + 4);
	    OUT_RING(CP_PACKET3(R200_CP_PACKET3_3D_DRAW_IMMD_2,
				4 * pPriv->vtx_count));
	    OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_QUAD_LIST |
		     RADEON_CP_VC_CNTL_PRIM_WALK_RING |
		     (4 << RADEON_CP_VC_CNTL_NUM_SHIFT));
	} else {
	    BEGIN_RING(3 * pPriv->vtx_count + 4);
	    OUT_RING(CP_PACKET3(R200_CP_PACKET3_3D_DRAW_IMMD_2,
				3 * pPriv->vtx_count));
	    OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_TRI_LIST |
		     RADEON_CP_VC_CNTL_PRIM_WALK_RING |
		     (3 << RADEON_CP_VC_CNTL_NUM_SHIFT));
	}
#else /* ACCEL_CP */
	if (use_quad)
	    BEGIN_ACCEL(2 + pPriv->vtx_count * 4);
	else
	    BEGIN_ACCEL(2 + pPriv->vtx_count * 3);

	if (use_quad)
	    OUT_ACCEL_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_QUAD_LIST |
					      RADEON_VF_PRIM_WALK_DATA |
					      (4 << RADEON_VF_NUM_VERTICES_SHIFT)));
	else
	    OUT_ACCEL_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_TRIANGLE_LIST |
					      RADEON_VF_PRIM_WALK_DATA |
					      (3 << RADEON_VF_NUM_VERTICES_SHIFT)));
#endif
	if (pPriv->bicubic_enabled) {
		/*
		 * This code is only executed on >= R300, so we don't
		 * have to deal with the legacy handling.
		 */
	    if (use_quad) {
		VTX_OUT_6((float)dstX,                     (float)dstY,
			  (float)srcX / pPriv->w,          (float)srcY / pPriv->h,
			  (float)srcX + 0.5,               (float)srcY + 0.5);
		VTX_OUT_6((float)dstX,                     (float)(dstY + dsth),
			  (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h,
			  (float)srcX + 0.5,               (float)(srcY + srch) + 0.5);
		VTX_OUT_6((float)(dstX + dstw),            (float)(dstY + dsth),
			  (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h,
			  (float)(srcX + srcw) + 0.5,      (float)(srcY + srch) + 0.5);
		VTX_OUT_6((float)(dstX + dstw),            (float)dstY,
			  (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h,
			  (float)(srcX + srcw) + 0.5,      (float)srcY + 0.5);
	    } else {
		VTX_OUT_6((float)dstX,                     (float)dstY,
			  (float)srcX / pPriv->w,          (float)srcY / pPriv->h,
			  (float)srcX + 0.5,               (float)srcY + 0.5);
		VTX_OUT_6((float)dstX,                     (float)(dstY + dstw + dsth),
			  (float)srcX / pPriv->w,
			  ((float)srcY + (float)srch * (((float)dstw / (float)dsth) + 1.0)) / pPriv->h,
			  (float)srcX + 0.5,
			  (float)srcY + (float)srch * (((float)dstw / (float)dsth) + 1.0) + 0.5);
		VTX_OUT_6((float)(dstX + dstw + dsth),     (float)dstY,
			  ((float)srcX + (float)srcw * (((float)dsth / (float)dstw) + 1.0)) / pPriv->w,
			  (float)srcY / pPriv->h,
			  (float)srcX + (float)srcw * (((float)dsth / (float)dstw) + 1.0) + 0.5,
			  (float)srcY + 0.5);
	    }
	} else {
	    if (use_quad) {
		VTX_OUT_4((float)dstX,                     (float)dstY,
			  (float)srcX / pPriv->w,          (float)srcY / pPriv->h);
		VTX_OUT_4((float)dstX,                     (float)(dstY + dsth),
			  (float)srcX / pPriv->w,          (float)(srcY + srch) / pPriv->h);
		VTX_OUT_4((float)(dstX + dstw),            (float)(dstY + dsth),
			  (float)(srcX + srcw) / pPriv->w, (float)(srcY + srch) / pPriv->h);
		VTX_OUT_4((float)(dstX + dstw),            (float)dstY,
			  (float)(srcX + srcw) / pPriv->w, (float)srcY / pPriv->h);
	    } else {
		/*
		 * Render a big, scissored triangle. This means
		 * increasing the triangle size and adjusting
		 * texture coordinates.
		 */
		VTX_OUT_4((float)dstX,                 (float)dstY,
			  (float)srcX / pPriv->w,      (float)srcY / pPriv->h);
		VTX_OUT_4((float)dstX,                 (float)(dstY + dsth + dstw),
			  (float)srcX / pPriv->w,
			  ((float)srcY + (float)srch * (((float)dstw / (float)dsth) + 1.0)) / pPriv->h);
		VTX_OUT_4((float)(dstX + dstw + dsth), (float)dstY,
			  ((float)srcX + (float)srcw * (((float)dsth / (float)dstw) + 1.0)) / pPriv->w,
			  (float)srcY / pPriv->h);
	    }
	}

	/* flushing is pipelined, free/finish is not */
	OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D);

#ifdef ACCEL_CP
	ADVANCE_RING();
#else
	FINISH_ACCEL();
#endif /* !ACCEL_CP */

	pBox++;
    }

    BEGIN_ACCEL(3);
    OUT_ACCEL_REG(R300_SC_CLIP_RULE, 0xAAAA);
    OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_RB3D_DC_FLUSH_ALL);
    OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);
    FINISH_ACCEL();

    DamageDamageRegion(pPriv->pDraw, &pPriv->clip);
}

static Bool
FUNC_NAME(R500PrepareTexturedVideo)(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    PixmapPtr pPixmap = pPriv->pPixmap;
    struct radeon_exa_pixmap_priv *driver_priv;
    struct radeon_bo *src_bo = pPriv->src_bo[pPriv->currentBuffer];
    uint32_t txfilter, txformat0, txformat1, txoffset, txpitch, us_format = 0;
    uint32_t dst_pitch, dst_format;
    uint32_t txenable, colorpitch, bicubic_offset;
    uint32_t output_fmt;
    int pixel_shift, out_size = 6;
    ACCEL_PREAMBLE();

#ifdef XF86DRM_MODE
    if (info->cs) {
	int ret;

	radeon_cs_space_reset_bos(info->cs);
	radeon_cs_space_add_persistent_bo(info->cs, src_bo, RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM, 0);

	if (pPriv->bicubic_enabled)
	    radeon_cs_space_add_persistent_bo(info->cs, info->bicubic_bo, RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM, 0);

	driver_priv = exaGetPixmapDriverPrivate(pPixmap);
	radeon_cs_space_add_persistent_bo(info->cs, driver_priv->bo, 0, RADEON_GEM_DOMAIN_VRAM);

	ret = radeon_cs_space_check(info->cs);
	if (ret) {
	    ErrorF("Not enough RAM to hw accel xv operation\n");
	    return FALSE;
	}
    }
#else
    (void)src_bo;
#endif

    pixel_shift = pPixmap->drawable.bitsPerPixel >> 4;

#ifdef USE_EXA
    if (info->useEXA) {
	dst_pitch = exaGetPixmapPitch(pPixmap);
    } else
#endif
    {
	dst_pitch = pPixmap->devKind;
    }

#ifdef USE_EXA
    if (info->useEXA) {
	RADEON_SWITCH_TO_3D();
    } else
#endif
    {
	BEGIN_ACCEL(2);
	OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D);
	/* We must wait for 3d to idle, in case source was just written as a dest. */
	OUT_ACCEL_REG(RADEON_WAIT_UNTIL,
		      RADEON_WAIT_HOST_IDLECLEAN |
		      RADEON_WAIT_2D_IDLECLEAN |
		      RADEON_WAIT_3D_IDLECLEAN |
		      RADEON_WAIT_DMA_GUI_IDLE);
	FINISH_ACCEL();

	if (!info->accel_state->XInited3D)
	    RADEONInit3DEngine(pScrn);
    }

    if (pPriv->bicubic_enabled)
	pPriv->vtx_count = 6;
    else
	pPriv->vtx_count = 4;

    switch (pPixmap->drawable.bitsPerPixel) {
    case 16:
	if (pPixmap->drawable.depth == 15)
	    dst_format = R300_COLORFORMAT_ARGB1555;
	else
	    dst_format = R300_COLORFORMAT_RGB565;
	break;
    case 32:
	dst_format = R300_COLORFORMAT_ARGB8888;
	break;
    default:
	return FALSE;
    }

    output_fmt = (R300_OUT_FMT_C4_8 |
		  R300_OUT_FMT_C0_SEL_BLUE |
		  R300_OUT_FMT_C1_SEL_GREEN |
		  R300_OUT_FMT_C2_SEL_RED |
		  R300_OUT_FMT_C3_SEL_ALPHA);

    colorpitch = dst_pitch >> pixel_shift;
    colorpitch |= dst_format;

    if (RADEONTilingEnabled(pScrn, pPixmap))
	colorpitch |= R300_COLORTILE;

    if (((pPriv->bicubic_state == BICUBIC_OFF)) &&
        (pPriv->id == FOURCC_I420 || pPriv->id == FOURCC_YV12))
	pPriv->is_planar = TRUE;
    else
	pPriv->is_planar = FALSE;

    if (pPriv->is_planar) {
	txformat1 = R300_TX_FORMAT_X8;
	txpitch = pPriv->src_pitch;
    } else {
	if (pPriv->id == FOURCC_UYVY)
	    txformat1 = R300_TX_FORMAT_YVYU422;
	else
	    txformat1 = R300_TX_FORMAT_VYUY422;

	if (pPriv->bicubic_state != BICUBIC_OFF)
	    txformat1 |= R300_TX_FORMAT_YUV_TO_RGB_CLAMP;

	/* pitch is in pixels */
	txpitch = pPriv->src_pitch / 2;
    }
    txpitch -= 1;

    txformat0 = ((((pPriv->w - 1) & 0x7ff) << R300_TXWIDTH_SHIFT) |
		 (((pPriv->h - 1) & 0x7ff) << R300_TXHEIGHT_SHIFT) |
		 R300_TXPITCH_EN);

    txfilter = (R300_TX_CLAMP_S(R300_TX_CLAMP_CLAMP_LAST) |
		R300_TX_CLAMP_T(R300_TX_CLAMP_CLAMP_LAST) |
		R300_TX_MAG_FILTER_LINEAR |
		R300_TX_MIN_FILTER_LINEAR |
		(0 << R300_TX_ID_SHIFT));


    if ((pPriv->w - 1) & 0x800)
	txpitch |= R500_TXWIDTH_11;

    if ((pPriv->h - 1) & 0x800)
	txpitch |= R500_TXHEIGHT_11;

    if (info->ChipFamily == CHIP_FAMILY_R520) {
	unsigned us_width = (pPriv->w - 1) & 0x7ff;
	unsigned us_height = (pPriv->h - 1) & 0x7ff;
	unsigned us_depth = 0;

	if (pPriv->w > 2048) {
	    us_width = (0x7ff + us_width) >> 1;
	    us_depth |= 0x0d;
	}
	if (pPriv->h > 2048) {
	    us_height = (0x7ff + us_height) >> 1;
	    us_depth |= 0x0e;
	}
	us_format = (us_width << R300_TXWIDTH_SHIFT) |
		    (us_height << R300_TXHEIGHT_SHIFT) |
		    (us_depth << R300_TXDEPTH_SHIFT);
	out_size++;
    }

    txoffset = info->cs ? 0 : pPriv->src_offset;

    BEGIN_ACCEL_RELOC(out_size, 1);
    OUT_ACCEL_REG(R300_TX_FILTER0_0, txfilter);
    OUT_ACCEL_REG(R300_TX_FILTER1_0, 0);
    OUT_ACCEL_REG(R300_TX_FORMAT0_0, txformat0);
    OUT_ACCEL_REG(R300_TX_FORMAT1_0, txformat1);
    OUT_ACCEL_REG(R300_TX_FORMAT2_0, txpitch);
    OUT_TEXTURE_REG(R300_TX_OFFSET_0, txoffset, src_bo);
    if (info->ChipFamily == CHIP_FAMILY_R520)
	OUT_ACCEL_REG(R500_US_FORMAT0_0, us_format);
    FINISH_ACCEL();

    txenable = R300_TEX_0_ENABLE;

    if (pPriv->is_planar) {
	txformat0 = ((((((pPriv->w + 1 ) >> 1) - 1) & 0x7ff) << R300_TXWIDTH_SHIFT) |
		     (((((pPriv->h + 1 ) >> 1 ) - 1) & 0x7ff) << R300_TXHEIGHT_SHIFT) |
		     R300_TXPITCH_EN);
	txpitch = RADEON_ALIGN(pPriv->src_pitch >> 1, 64);
	txpitch -= 1;
	txfilter = (R300_TX_CLAMP_S(R300_TX_CLAMP_CLAMP_LAST) |
		    R300_TX_CLAMP_T(R300_TX_CLAMP_CLAMP_LAST) |
		    R300_TX_MIN_FILTER_LINEAR |
		    R300_TX_MAG_FILTER_LINEAR);

	BEGIN_ACCEL_RELOC(12, 2);
	OUT_ACCEL_REG(R300_TX_FILTER0_1, txfilter | (1 << R300_TX_ID_SHIFT));
	OUT_ACCEL_REG(R300_TX_FILTER1_1, 0);
	OUT_ACCEL_REG(R300_TX_FORMAT0_1, txformat0);
	OUT_ACCEL_REG(R300_TX_FORMAT1_1, R300_TX_FORMAT_X8);
	OUT_ACCEL_REG(R300_TX_FORMAT2_1, txpitch);
	OUT_TEXTURE_REG(R300_TX_OFFSET_1, txoffset + pPriv->planeu_offset, src_bo);
	OUT_ACCEL_REG(R300_TX_FILTER0_2, txfilter | (2 << R300_TX_ID_SHIFT));
	OUT_ACCEL_REG(R300_TX_FILTER1_2, 0);
	OUT_ACCEL_REG(R300_TX_FORMAT0_2, txformat0);
	OUT_ACCEL_REG(R300_TX_FORMAT1_2, R300_TX_FORMAT_X8);
	OUT_ACCEL_REG(R300_TX_FORMAT2_2, txpitch);
	OUT_TEXTURE_REG(R300_TX_OFFSET_2, txoffset + pPriv->planev_offset, src_bo);
	FINISH_ACCEL();
	txenable |= R300_TEX_1_ENABLE | R300_TEX_2_ENABLE;
    }

    if (pPriv->bicubic_enabled) {
	/* Size is 128x1 */
	txformat0 = ((0x7f << R300_TXWIDTH_SHIFT) |
		     (0x0 << R300_TXHEIGHT_SHIFT) |
		     R300_TXPITCH_EN);
	/* Format is 32-bit floats, 4bpp */
	txformat1 = R300_EASY_TX_FORMAT(Z, Y, X, W, FL_R16G16B16A16);
	/* Pitch is 127 (128-1) */
	txpitch = 0x7f;
	/* Tex filter */
	txfilter = (R300_TX_CLAMP_S(R300_TX_CLAMP_WRAP) |
		    R300_TX_CLAMP_T(R300_TX_CLAMP_WRAP) |
		    R300_TX_MIN_FILTER_NEAREST |
		    R300_TX_MAG_FILTER_NEAREST |
		    (1 << R300_TX_ID_SHIFT));

	if (info->cs)
	    bicubic_offset = 0;
	else
	    bicubic_offset = pPriv->bicubic_src_offset;

	BEGIN_ACCEL_RELOC(6, 1);
	OUT_ACCEL_REG(R300_TX_FILTER0_1, txfilter);
	OUT_ACCEL_REG(R300_TX_FILTER1_1, 0);
	OUT_ACCEL_REG(R300_TX_FORMAT0_1, txformat0);
	OUT_ACCEL_REG(R300_TX_FORMAT1_1, txformat1);
	OUT_ACCEL_REG(R300_TX_FORMAT2_1, txpitch);
	OUT_TEXTURE_REG(R300_TX_OFFSET_1, bicubic_offset, info->bicubic_bo);
	FINISH_ACCEL();

	/* Enable tex 1 */
	txenable |= R300_TEX_1_ENABLE;
    }

    /* setup the VAP */
    if (info->accel_state->has_tcl) {
	if (pPriv->bicubic_enabled)
	    BEGIN_ACCEL(7);
	else
	    BEGIN_ACCEL(6);
    } else {
	if (pPriv->bicubic_enabled)
	    BEGIN_ACCEL(5);
	else
	    BEGIN_ACCEL(4);
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
    if (pPriv->bicubic_enabled) {
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
    } else {
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
    }

    /* load the vertex shader
     * We pre-load vertex programs in RADEONInit3DEngine():
     * - exa
     * - Xv
     * - Xv bicubic
     * Here we select the offset of the vertex program we want to use
     */
    if (info->accel_state->has_tcl) {
	if (pPriv->bicubic_enabled) {
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_0,
			  ((11 << R300_PVS_FIRST_INST_SHIFT) |
			   (13 << R300_PVS_XYZW_VALID_INST_SHIFT) |
			   (13 << R300_PVS_LAST_INST_SHIFT)));
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_1,
			  (13 << R300_PVS_LAST_VTX_SRC_INST_SHIFT));
	} else {
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_0,
			  ((9 << R300_PVS_FIRST_INST_SHIFT) |
			   (10 << R300_PVS_XYZW_VALID_INST_SHIFT) |
			   (10 << R300_PVS_LAST_INST_SHIFT)));
	    OUT_ACCEL_REG(R300_VAP_PVS_CODE_CNTL_1,
			  (10 << R300_PVS_LAST_VTX_SRC_INST_SHIFT));
	}
    }

    /* Position and one set of 2 texture coordinates */
    OUT_ACCEL_REG(R300_VAP_OUT_VTX_FMT_0, R300_VTX_POS_PRESENT);
    if (pPriv->bicubic_enabled)
	OUT_ACCEL_REG(R300_VAP_OUT_VTX_FMT_1, ((2 << R300_TEX_0_COMP_CNT_SHIFT) |
					       (2 << R300_TEX_1_COMP_CNT_SHIFT)));
    else
	OUT_ACCEL_REG(R300_VAP_OUT_VTX_FMT_1, (2 << R300_TEX_0_COMP_CNT_SHIFT));

    OUT_ACCEL_REG(R300_US_OUT_FMT_0, output_fmt);
    FINISH_ACCEL();

    /* setup pixel shader */
    if (pPriv->bicubic_state != BICUBIC_OFF) {
	if (pPriv->bicubic_enabled) {
	    BEGIN_ACCEL(7);

	    /* 4 components: 2 for tex0 and 2 for tex1 */
	    OUT_ACCEL_REG(R300_RS_COUNT,
			  ((4 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));

	    /* R300_INST_COUNT_RS - highest RS instruction used */
	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(1));

	    /* Pixel stack frame size. */
	    OUT_ACCEL_REG(R300_US_PIXSIZE, 5);

	    /* FP length. */
	    OUT_ACCEL_REG(R500_US_CODE_ADDR, (R500_US_CODE_START_ADDR(0) |
					      R500_US_CODE_END_ADDR(13)));
	    OUT_ACCEL_REG(R500_US_CODE_RANGE, (R500_US_CODE_RANGE_ADDR(0) |
					       R500_US_CODE_RANGE_SIZE(13)));

	    /* Prepare for FP emission. */
	    OUT_ACCEL_REG(R500_US_CODE_OFFSET, 0);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_INDEX, R500_US_VECTOR_INST_INDEX(0));
	    FINISH_ACCEL();

	    BEGIN_ACCEL(89);
	    /* Pixel shader.
	     * I've gone ahead and annotated each instruction, since this
	     * thing is MASSIVE. :3
	     * Note: In order to avoid buggies with temps and multiple
	     * inputs, all temps are offset by 2. temp0 -> register2. */

	    /* TEX temp2, input1.xxxx, tex1, 1D */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_TEX |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_ID(1) |
						   R500_TEX_INST_LD |
						   R500_TEX_IGNORE_UNCOVERED));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(1) |
						   R500_TEX_SRC_S_SWIZ_R |
						   R500_TEX_SRC_T_SWIZ_R |
						   R500_TEX_SRC_R_SWIZ_R |
						   R500_TEX_SRC_Q_SWIZ_R |
						   R500_TEX_DST_ADDR(2) |
						   R500_TEX_DST_R_SWIZ_R |
						   R500_TEX_DST_G_SWIZ_G |
						   R500_TEX_DST_B_SWIZ_B |
						   R500_TEX_DST_A_SWIZ_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);

	    /* TEX temp5, input1.yyyy, tex1, 1D */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_TEX |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_ID(1) |
						   R500_TEX_INST_LD |
						   R500_TEX_SEM_ACQUIRE |
						   R500_TEX_IGNORE_UNCOVERED));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(1) |
						   R500_TEX_SRC_S_SWIZ_G |
						   R500_TEX_SRC_T_SWIZ_G |
						   R500_TEX_SRC_R_SWIZ_G |
						   R500_TEX_SRC_Q_SWIZ_G |
						   R500_TEX_DST_ADDR(5) |
						   R500_TEX_DST_R_SWIZ_R |
						   R500_TEX_DST_G_SWIZ_G |
						   R500_TEX_DST_B_SWIZ_B |
						   R500_TEX_DST_A_SWIZ_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);

	    /* MUL temp4, const0.x0x0, temp2.yyxx */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_ALU |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(0) |
						   R500_RGB_ADDR0_CONST |
						   R500_RGB_ADDR1(2)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(0) |
						   R500_ALPHA_ADDR0_CONST |
						   R500_ALPHA_ADDR1(2)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
						   R500_ALU_RGB_R_SWIZ_A_R |
						   R500_ALU_RGB_G_SWIZ_A_0 |
						   R500_ALU_RGB_B_SWIZ_A_R |
						   R500_ALU_RGB_SEL_B_SRC1 |
						   R500_ALU_RGB_R_SWIZ_B_G |
						   R500_ALU_RGB_G_SWIZ_B_G |
						   R500_ALU_RGB_B_SWIZ_B_R));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDRD(4) |
						   R500_ALPHA_OP_MAD |
						   R500_ALPHA_SEL_A_SRC0 |
						   R500_ALPHA_SWIZ_A_0 |
						   R500_ALPHA_SEL_B_SRC1 |
						   R500_ALPHA_SWIZ_B_R));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_ADDRD(4) |
						   R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_R_SWIZ_0 |
						   R500_ALU_RGBA_G_SWIZ_0 |
						   R500_ALU_RGBA_B_SWIZ_0 |
						   R500_ALU_RGBA_A_SWIZ_0));

	    /* MAD temp3, const0.0y0y, temp5.xxxx, temp4 */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_ALU |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(0) |
						   R500_RGB_ADDR0_CONST |
						   R500_RGB_ADDR1(5) |
						   R500_RGB_ADDR2(4)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(0) |
						   R500_ALPHA_ADDR0_CONST |
						   R500_ALPHA_ADDR1(5) |
						   R500_ALPHA_ADDR2(4)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
						   R500_ALU_RGB_R_SWIZ_A_0 |
						   R500_ALU_RGB_G_SWIZ_A_G |
						   R500_ALU_RGB_B_SWIZ_A_0 |
						   R500_ALU_RGB_SEL_B_SRC1 |
						   R500_ALU_RGB_R_SWIZ_B_R |
						   R500_ALU_RGB_G_SWIZ_B_R |
						   R500_ALU_RGB_B_SWIZ_B_R));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDRD(3) |
						   R500_ALPHA_OP_MAD |
						   R500_ALPHA_SEL_A_SRC0 |
						   R500_ALPHA_SWIZ_A_G |
						   R500_ALPHA_SEL_B_SRC1 |
						   R500_ALPHA_SWIZ_B_R));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_ADDRD(3) |
						   R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_SEL_C_SRC2 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_B |
						   R500_ALU_RGBA_A_SWIZ_A));

	    /* ADD temp3, temp3, input0.xyxy */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_ALU |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR1(3) |
						   R500_RGB_ADDR2(0)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR1(3) |
						   R500_ALPHA_ADDR2(0)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_R_SWIZ_A_1 |
						   R500_ALU_RGB_G_SWIZ_A_1 |
						   R500_ALU_RGB_B_SWIZ_A_1 |
						   R500_ALU_RGB_SEL_B_SRC1 |
						   R500_ALU_RGB_R_SWIZ_B_R |
						   R500_ALU_RGB_G_SWIZ_B_G |
						   R500_ALU_RGB_B_SWIZ_B_B));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDRD(3) |
						   R500_ALPHA_OP_MAD |
						   R500_ALPHA_SWIZ_A_1 |
						   R500_ALPHA_SEL_B_SRC1 |
						   R500_ALPHA_SWIZ_B_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_ADDRD(3) |
						   R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_SEL_C_SRC2 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_R |
						   R500_ALU_RGBA_A_SWIZ_G));

	    /* TEX temp1, temp3.zwxy, tex0, 2D */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_TEX |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_ID(0) |
						   R500_TEX_INST_LD |
						   R500_TEX_IGNORE_UNCOVERED));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(3) |
						   R500_TEX_SRC_S_SWIZ_B |
						   R500_TEX_SRC_T_SWIZ_A |
						   R500_TEX_SRC_R_SWIZ_R |
						   R500_TEX_SRC_Q_SWIZ_G |
						   R500_TEX_DST_ADDR(1) |
						   R500_TEX_DST_R_SWIZ_R |
						   R500_TEX_DST_G_SWIZ_G |
						   R500_TEX_DST_B_SWIZ_B |
						   R500_TEX_DST_A_SWIZ_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);

	    /* TEX temp3, temp3.xyzw, tex0, 2D */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_TEX |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_ID(0) |
						   R500_TEX_INST_LD |
						   R500_TEX_SEM_ACQUIRE |
						   R500_TEX_IGNORE_UNCOVERED));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(3) |
						   R500_TEX_SRC_S_SWIZ_R |
						   R500_TEX_SRC_T_SWIZ_G |
						   R500_TEX_SRC_R_SWIZ_B |
						   R500_TEX_SRC_Q_SWIZ_A |
						   R500_TEX_DST_ADDR(3) |
						   R500_TEX_DST_R_SWIZ_R |
						   R500_TEX_DST_G_SWIZ_G |
						   R500_TEX_DST_B_SWIZ_B |
						   R500_TEX_DST_A_SWIZ_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);

	    /* MAD temp4, const0.0y0y, temp5.yyyy, temp4 */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_ALU |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(0) |
						   R500_RGB_ADDR0_CONST |
						   R500_RGB_ADDR1(5) |
						   R500_RGB_ADDR2(4)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(0) |
						   R500_ALPHA_ADDR0_CONST |
						   R500_ALPHA_ADDR1(5) |
						   R500_ALPHA_ADDR2(4)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
						   R500_ALU_RGB_R_SWIZ_A_0 |
						   R500_ALU_RGB_G_SWIZ_A_G |
						   R500_ALU_RGB_B_SWIZ_A_0 |
						   R500_ALU_RGB_SEL_B_SRC1 |
						   R500_ALU_RGB_R_SWIZ_B_G |
						   R500_ALU_RGB_G_SWIZ_B_G |
						   R500_ALU_RGB_B_SWIZ_B_G));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDRD(4) |
						   R500_ALPHA_OP_MAD |
						   R500_ALPHA_SEL_A_SRC0 |
						   R500_ALPHA_SWIZ_A_G |
						   R500_ALPHA_SEL_B_SRC1 |
						   R500_ALPHA_SWIZ_B_G));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_ADDRD(4) |
						   R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_SEL_C_SRC2 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_B |
						   R500_ALU_RGBA_A_SWIZ_A));

	    /* ADD temp0, temp4, input0.xyxy */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_ALU |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR1(4) |
						   R500_RGB_ADDR2(0)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR1(4) |
						   R500_ALPHA_ADDR2(0)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_R_SWIZ_A_1 |
						   R500_ALU_RGB_G_SWIZ_A_1 |
						   R500_ALU_RGB_B_SWIZ_A_1 |
						   R500_ALU_RGB_SEL_B_SRC1 |
						   R500_ALU_RGB_R_SWIZ_B_R |
						   R500_ALU_RGB_G_SWIZ_B_G |
						   R500_ALU_RGB_B_SWIZ_B_B));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDRD(0) |
						   R500_ALPHA_OP_MAD |
						   R500_ALPHA_SWIZ_A_1 |
						   R500_ALPHA_SEL_B_SRC1 |
						   R500_ALPHA_SWIZ_B_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_ADDRD(0) |
						   R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_SEL_C_SRC2 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_R |
						   R500_ALU_RGBA_A_SWIZ_G));

	    /* TEX temp4, temp0.zwzw, tex0, 2D */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_TEX |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_ID(0) |
						   R500_TEX_INST_LD |
						   R500_TEX_IGNORE_UNCOVERED));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(0) |
						   R500_TEX_SRC_S_SWIZ_B |
						   R500_TEX_SRC_T_SWIZ_A |
						   R500_TEX_SRC_R_SWIZ_B |
						   R500_TEX_SRC_Q_SWIZ_A |
						   R500_TEX_DST_ADDR(4) |
						   R500_TEX_DST_R_SWIZ_R |
						   R500_TEX_DST_G_SWIZ_G |
						   R500_TEX_DST_B_SWIZ_B |
						   R500_TEX_DST_A_SWIZ_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);

	    /* TEX temp0, temp0.xyzw, tex0, 2D */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_TEX |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_ID(0) |
						   R500_TEX_INST_LD |
						   R500_TEX_SEM_ACQUIRE |
						   R500_TEX_IGNORE_UNCOVERED));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(0) |
						   R500_TEX_SRC_S_SWIZ_R |
						   R500_TEX_SRC_T_SWIZ_G |
						   R500_TEX_SRC_R_SWIZ_B |
						   R500_TEX_SRC_Q_SWIZ_A |
						   R500_TEX_DST_ADDR(0) |
						   R500_TEX_DST_R_SWIZ_R |
						   R500_TEX_DST_G_SWIZ_G |
						   R500_TEX_DST_B_SWIZ_B |
						   R500_TEX_DST_A_SWIZ_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, 0x00000000);

	    /* LRP temp3, temp2.zzzz, temp1, temp3 ->
	     * - PRESUB temps, temp1 - temp3
	     * - MAD temp2.zzzz, temps, temp3 */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_ALU |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(3) |
						   R500_RGB_SRCP_OP_RGB1_MINUS_RGB0 |
						   R500_RGB_ADDR1(1) |
						   R500_RGB_ADDR2(2)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(3) |
						   R500_ALPHA_SRCP_OP_A1_MINUS_A0 |
						   R500_ALPHA_ADDR1(1) |
						   R500_ALPHA_ADDR2(2)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC2 |
						   R500_ALU_RGB_R_SWIZ_A_B |
						   R500_ALU_RGB_G_SWIZ_A_B |
						   R500_ALU_RGB_B_SWIZ_A_B |
						   R500_ALU_RGB_SEL_B_SRCP |
						   R500_ALU_RGB_R_SWIZ_B_R |
						   R500_ALU_RGB_G_SWIZ_B_G |
						   R500_ALU_RGB_B_SWIZ_B_B));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDRD(3) |
						   R500_ALPHA_OP_MAD |
						   R500_ALPHA_SEL_A_SRC2 |
						   R500_ALPHA_SWIZ_A_B |
						   R500_ALPHA_SEL_B_SRCP |
						   R500_ALPHA_SWIZ_B_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_ADDRD(3) |
						   R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_SEL_C_SRC0 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_B |
						   R500_ALU_RGBA_A_SWIZ_A));

	    /* LRP temp0, temp2.zzzz, temp4, temp0 ->
	     * - PRESUB temps, temp4 - temp1
	     * - MAD temp2.zzzz, temps, temp0 */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_ALU |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(0) |
						   R500_RGB_SRCP_OP_RGB1_MINUS_RGB0 |
						   R500_RGB_ADDR1(4) |
						   R500_RGB_ADDR2(2)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(0) |
						   R500_ALPHA_SRCP_OP_A1_MINUS_A0 |
						   R500_ALPHA_ADDR1(4) |
						   R500_ALPHA_ADDR2(2)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC2 |
						   R500_ALU_RGB_R_SWIZ_A_B |
						   R500_ALU_RGB_G_SWIZ_A_B |
						   R500_ALU_RGB_B_SWIZ_A_B |
						   R500_ALU_RGB_SEL_B_SRCP |
						   R500_ALU_RGB_R_SWIZ_B_R |
						   R500_ALU_RGB_G_SWIZ_B_G |
						   R500_ALU_RGB_B_SWIZ_B_B));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDRD(0) |
						   R500_ALPHA_OP_MAD |
						   R500_ALPHA_SEL_A_SRC2 |
						   R500_ALPHA_SWIZ_A_B |
						   R500_ALPHA_SEL_B_SRCP |
						   R500_ALPHA_SWIZ_B_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_ADDRD(0) |
						   R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_SEL_C_SRC0 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_B |
						   R500_ALU_RGBA_A_SWIZ_A));

	    /* LRP output, temp5.zzzz, temp3, temp0 ->
	     * - PRESUB temps, temp3 - temp0
	     * - MAD temp5.zzzz, temps, temp0 */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_OUT |
						   R500_INST_LAST |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK |
						   R500_INST_RGB_OMASK_R |
						   R500_INST_RGB_OMASK_G |
						   R500_INST_RGB_OMASK_B |
						   R500_INST_ALPHA_OMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(0) |
						   R500_RGB_SRCP_OP_RGB1_MINUS_RGB0 |
						   R500_RGB_ADDR1(3) |
						   R500_RGB_ADDR2(5)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(0) |
						   R500_ALPHA_SRCP_OP_A1_MINUS_A0 |
						   R500_ALPHA_ADDR1(3) |
						   R500_ALPHA_ADDR2(5)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC2 |
						   R500_ALU_RGB_R_SWIZ_A_B |
						   R500_ALU_RGB_G_SWIZ_A_B |
						   R500_ALU_RGB_B_SWIZ_A_B |
						   R500_ALU_RGB_SEL_B_SRCP |
						   R500_ALU_RGB_R_SWIZ_B_R |
						   R500_ALU_RGB_G_SWIZ_B_G |
						   R500_ALU_RGB_B_SWIZ_B_B));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDRD(0) |
						   R500_ALPHA_OP_MAD |
						   R500_ALPHA_SEL_A_SRC2 |
						   R500_ALPHA_SWIZ_A_B |
						   R500_ALPHA_SEL_B_SRCP |
						   R500_ALPHA_SWIZ_B_A));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_ADDRD(0) |
						   R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_SEL_C_SRC0 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_B |
						   R500_ALU_RGBA_A_SWIZ_A));

	    /* Shader constants. */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_INDEX, R500_US_VECTOR_CONST_INDEX(0));

	    /* const0 = {1 / texture[0].width, 1 / texture[0].height, 0, 0} */
	    OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, (1.0/(float)pPriv->w));
	    OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, (1.0/(float)pPriv->h));
	    OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, 0x0);
	    OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, 0x0);

	    FINISH_ACCEL();
	} else {
	    BEGIN_ACCEL(19);
	    /* 2 components: 2 for tex0 */
	    OUT_ACCEL_REG(R300_RS_COUNT,
			  ((2 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));

	    /* R300_INST_COUNT_RS - highest RS instruction used */
	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(0));

	    /* Pixel stack frame size. */
	    OUT_ACCEL_REG(R300_US_PIXSIZE, 0); /* highest temp used */

	    /* FP length. */
	    OUT_ACCEL_REG(R500_US_CODE_ADDR, (R500_US_CODE_START_ADDR(0) |
					      R500_US_CODE_END_ADDR(1)));
	    OUT_ACCEL_REG(R500_US_CODE_RANGE, (R500_US_CODE_RANGE_ADDR(0) |
					       R500_US_CODE_RANGE_SIZE(1)));

	    /* Prepare for FP emission. */
	    OUT_ACCEL_REG(R500_US_CODE_OFFSET, 0);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_INDEX, R500_US_VECTOR_INST_INDEX(0));

	    /* tex inst */
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

	    /* ALU inst */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_OUT |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_LAST |
						   R500_INST_RGB_OMASK_R |
						   R500_INST_RGB_OMASK_G |
						   R500_INST_RGB_OMASK_B |
						   R500_INST_ALPHA_OMASK |
						   R500_INST_RGB_CLAMP |
						   R500_INST_ALPHA_CLAMP));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(0) |
						   R500_RGB_ADDR1(0) |
						   R500_RGB_ADDR1_CONST |
						   R500_RGB_ADDR2(0) |
						   R500_RGB_ADDR2_CONST));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(0) |
						   R500_ALPHA_ADDR1(0) |
						   R500_ALPHA_ADDR1_CONST |
						   R500_ALPHA_ADDR2(0) |
						   R500_ALPHA_ADDR2_CONST));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
						   R500_ALU_RGB_R_SWIZ_A_R |
						   R500_ALU_RGB_G_SWIZ_A_G |
						   R500_ALU_RGB_B_SWIZ_A_B |
						   R500_ALU_RGB_SEL_B_SRC0 |
						   R500_ALU_RGB_R_SWIZ_B_1 |
						   R500_ALU_RGB_B_SWIZ_B_1 |
						   R500_ALU_RGB_G_SWIZ_B_1));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_OP_MAD |
						   R500_ALPHA_SWIZ_A_A |
						   R500_ALPHA_SWIZ_B_1));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_R_SWIZ_0 |
						   R500_ALU_RGBA_G_SWIZ_0 |
						   R500_ALU_RGBA_B_SWIZ_0 |
						   R500_ALU_RGBA_A_SWIZ_0));
	    FINISH_ACCEL();
	}
    } else {
	/*
	 * y' = y - .0625
	 * u' = u - .5
	 * v' = v - .5;
	 *
	 * r = 1.1643 * y' + 0.0     * u' + 1.5958  * v'
	 * g = 1.1643 * y' - 0.39173 * u' - 0.81290 * v'
	 * b = 1.1643 * y' + 2.017   * u' + 0.0     * v'
	 *
	 * DP3 might look like the straightforward solution
	 * but we'd need to move the texture yuv values in
	 * the same reg for this to work. Therefore use MADs.
	 * Brightness just adds to the off constant.
	 * Contrast is multiplication of luminance.
	 * Saturation and hue change the u and v coeffs.
	 * Default values (before adjustments - depend on colorspace):
	 * yco = 1.1643
	 * uco = 0, -0.39173, 2.017
	 * vco = 1.5958, -0.8129, 0
	 * off = -0.0625 * yco + -0.5 * uco[r] + -0.5 * vco[r],
	 *       -0.0625 * yco + -0.5 * uco[g] + -0.5 * vco[g],
	 *       -0.0625 * yco + -0.5 * uco[b] + -0.5 * vco[b],
	 *
	 * temp = MAD(yco, yuv.yyyy, off)
	 * temp = MAD(uco, yuv.uuuu, temp)
	 * result = MAD(vco, yuv.vvvv, temp)
	 */
	/* TODO: don't recalc consts always */
	const float Loff = -0.0627;
	const float Coff = -0.502;
	float uvcosf, uvsinf;
	float yco;
	float uco[3], vco[3], off[3];
	float bright, cont, gamma;
	int ref = pPriv->transform_index;
	Bool needgamma = FALSE;

	cont = RTFContrast(pPriv->contrast);
	bright = RTFBrightness(pPriv->brightness);
	gamma = (float)pPriv->gamma / 1000.0;
	uvcosf = RTFSaturation(pPriv->saturation) * cos(RTFHue(pPriv->hue));
	uvsinf = RTFSaturation(pPriv->saturation) * sin(RTFHue(pPriv->hue));
	/* overlay video also does pre-gamma contrast/sat adjust, should we? */

	yco = trans[ref].RefLuma * cont;
	uco[0] = -trans[ref].RefRCr * uvsinf;
	uco[1] = trans[ref].RefGCb * uvcosf - trans[ref].RefGCr * uvsinf;
	uco[2] = trans[ref].RefBCb * uvcosf;
	vco[0] = trans[ref].RefRCr * uvcosf;
	vco[1] = trans[ref].RefGCb * uvsinf + trans[ref].RefGCr * uvcosf;
	vco[2] = trans[ref].RefBCb * uvsinf;
	off[0] = Loff * yco + Coff * (uco[0] + vco[0]) + bright;
	off[1] = Loff * yco + Coff * (uco[1] + vco[1]) + bright;
	off[2] = Loff * yco + Coff * (uco[2] + vco[2]) + bright;

	//XXX gamma

	if (gamma != 1.0) {
	    needgamma = TRUE;
	    /* note: gamma correction is out = in ^ gamma;
	       gpu can only do LG2/EX2 therefore we transform into
	       in ^ gamma = 2 ^ (log2(in) * gamma).
	       Lots of scalar ops, unfortunately (better solution?) -
	       without gamma that's 3 inst, with gamma it's 10...
	       could use different gamma factors per channel,
	       if that's of any use. */
	}

	if (pPriv->is_planar) {
	    BEGIN_ACCEL(56);
	    /* 2 components: 2 for tex0 */
	    OUT_ACCEL_REG(R300_RS_COUNT,
			  ((2 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));

	    /* R300_INST_COUNT_RS - highest RS instruction used */
	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(0));

	    /* Pixel stack frame size. */
	    OUT_ACCEL_REG(R300_US_PIXSIZE, 2); /* highest temp used */

	    /* FP length. */
	    OUT_ACCEL_REG(R500_US_CODE_ADDR, (R500_US_CODE_START_ADDR(0) |
					      R500_US_CODE_END_ADDR(5)));
	    OUT_ACCEL_REG(R500_US_CODE_RANGE, (R500_US_CODE_RANGE_ADDR(0) |
					       R500_US_CODE_RANGE_SIZE(5)));

	    /* Prepare for FP emission. */
	    OUT_ACCEL_REG(R500_US_CODE_OFFSET, 0);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_INDEX, R500_US_VECTOR_INST_INDEX(0));

	    /* tex inst */
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
						   R500_TEX_IGNORE_UNCOVERED));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(0) |
						   R500_TEX_SRC_S_SWIZ_R |
						   R500_TEX_SRC_T_SWIZ_G |
						   R500_TEX_DST_ADDR(2) |
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

	    /* tex inst */
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
						   R500_TEX_IGNORE_UNCOVERED));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(0) |
						   R500_TEX_SRC_S_SWIZ_R |
						   R500_TEX_SRC_T_SWIZ_G |
						   R500_TEX_DST_ADDR(1) |
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

	    /* tex inst */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_TEX |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK |
						   R500_INST_RGB_CLAMP |
						   R500_INST_ALPHA_CLAMP));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_ID(2) |
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

	    /* ALU inst */
	    /* MAD temp2.rgb, const0.aaa, temp2.rgb, const0.rgb */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_ALU |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(0) |
						   R500_RGB_ADDR0_CONST |
						   R500_RGB_ADDR1(2) |
						   R500_RGB_ADDR2(0) |
						   R500_RGB_ADDR2_CONST));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(0) |
						   R500_ALPHA_ADDR0_CONST |
						   R500_ALPHA_ADDR1(2) |
						   R500_ALPHA_ADDR2(0) |
						   R500_ALPHA_ADDR2_CONST));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
						   R500_ALU_RGB_R_SWIZ_A_A |
						   R500_ALU_RGB_G_SWIZ_A_A |
						   R500_ALU_RGB_B_SWIZ_A_A |
						   R500_ALU_RGB_SEL_B_SRC1 |
						   R500_ALU_RGB_R_SWIZ_B_R |
						   R500_ALU_RGB_B_SWIZ_B_G |
						   R500_ALU_RGB_G_SWIZ_B_B));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_OP_MAD |
						   R500_ALPHA_ADDRD(2) |
						   R500_ALPHA_SWIZ_A_0 |
						   R500_ALPHA_SWIZ_B_0));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_ADDRD(2) |
						   R500_ALU_RGBA_SEL_C_SRC0 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_B |
						   R500_ALU_RGBA_ALPHA_SEL_C_SRC0 |
						   R500_ALU_RGBA_A_SWIZ_0));

	    /* MAD temp2.rgb, const1.rgb, temp1.rgb, temp2.rgb */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_ALU |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(1) |
						   R500_RGB_ADDR0_CONST |
						   R500_RGB_ADDR1(1) |
						   R500_RGB_ADDR2(2)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(1) |
						   R500_ALPHA_ADDR0_CONST |
						   R500_ALPHA_ADDR1(1) |
						   R500_ALPHA_ADDR2(2)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
						   R500_ALU_RGB_R_SWIZ_A_R |
						   R500_ALU_RGB_G_SWIZ_A_G |
						   R500_ALU_RGB_B_SWIZ_A_B |
						   R500_ALU_RGB_SEL_B_SRC1 |
						   R500_ALU_RGB_R_SWIZ_B_R |
						   R500_ALU_RGB_B_SWIZ_B_G |
						   R500_ALU_RGB_G_SWIZ_B_B));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_OP_MAD |
						   R500_ALPHA_ADDRD(2) |
						   R500_ALPHA_SWIZ_A_0 |
						   R500_ALPHA_SWIZ_B_0));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_ADDRD(2) |
						   R500_ALU_RGBA_SEL_C_SRC2 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_B |
						   R500_ALU_RGBA_ALPHA_SEL_C_SRC0 |
						   R500_ALU_RGBA_A_SWIZ_0));

	    /* MAD result.rgb, const2.rgb, temp0.rgb, temp2.rgb */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_OUT |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_LAST |
						   R500_INST_RGB_OMASK_R |
						   R500_INST_RGB_OMASK_G |
						   R500_INST_RGB_OMASK_B |
						   R500_INST_ALPHA_OMASK |
						   R500_INST_RGB_CLAMP |
						   R500_INST_ALPHA_CLAMP));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(2) |
						   R500_RGB_ADDR0_CONST |
						   R500_RGB_ADDR1(0) |
						   R500_RGB_ADDR2(2)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(2) |
						   R500_ALPHA_ADDR0_CONST |
						   R500_ALPHA_ADDR1(0) |
						   R500_ALPHA_ADDR2(2)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
						   R500_ALU_RGB_R_SWIZ_A_R |
						   R500_ALU_RGB_G_SWIZ_A_G |
						   R500_ALU_RGB_B_SWIZ_A_B |
						   R500_ALU_RGB_SEL_B_SRC1 |
						   R500_ALU_RGB_R_SWIZ_B_R |
						   R500_ALU_RGB_B_SWIZ_B_G |
						   R500_ALU_RGB_G_SWIZ_B_B));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_OP_MAD |
						   R500_ALPHA_ADDRD(0) |
						   R500_ALPHA_SWIZ_A_0 |
						   R500_ALPHA_SWIZ_B_0));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_ADDRD(0) |
						   R500_ALU_RGBA_SEL_C_SRC2 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_B |
						   R500_ALU_RGBA_ALPHA_SEL_C_SRC0 |
						   R500_ALU_RGBA_A_SWIZ_1));

	} else {
	    BEGIN_ACCEL(44);
	    /* 2 components: 2 for tex0/1/2 */
	    OUT_ACCEL_REG(R300_RS_COUNT,
			  ((2 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));

	    /* R300_INST_COUNT_RS - highest RS instruction used */
	    OUT_ACCEL_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(0));

	    /* Pixel stack frame size. */
	    OUT_ACCEL_REG(R300_US_PIXSIZE, 1); /* highest temp used */

	    /* FP length. */
	    OUT_ACCEL_REG(R500_US_CODE_ADDR, (R500_US_CODE_START_ADDR(0) |
					      R500_US_CODE_END_ADDR(3)));
	    OUT_ACCEL_REG(R500_US_CODE_RANGE, (R500_US_CODE_RANGE_ADDR(0) |
					       R500_US_CODE_RANGE_SIZE(3)));

	    /* Prepare for FP emission. */
	    OUT_ACCEL_REG(R500_US_CODE_OFFSET, 0);
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_INDEX, R500_US_VECTOR_INST_INDEX(0));

	    /* tex inst */
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

	    /* ALU inst */
	    /* MAD temp1.rgb, const0.aaa, temp0.ggg, const0.rgb */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_ALU |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(0) |
						   R500_RGB_ADDR0_CONST |
						   R500_RGB_ADDR1(0) |
						   R500_RGB_ADDR2(0) |
						   R500_RGB_ADDR2_CONST));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(0) |
						   R500_ALPHA_ADDR0_CONST |
						   R500_ALPHA_ADDR1(0) |
						   R500_ALPHA_ADDR2(0) |
						   R500_ALPHA_ADDR2_CONST));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
						   R500_ALU_RGB_R_SWIZ_A_A |
						   R500_ALU_RGB_G_SWIZ_A_A |
						   R500_ALU_RGB_B_SWIZ_A_A |
						   R500_ALU_RGB_SEL_B_SRC1 |
						   R500_ALU_RGB_R_SWIZ_B_G |
						   R500_ALU_RGB_B_SWIZ_B_G |
						   R500_ALU_RGB_G_SWIZ_B_G));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_OP_MAD |
						   R500_ALPHA_ADDRD(1) |
						   R500_ALPHA_SWIZ_A_0 |
						   R500_ALPHA_SWIZ_B_0));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_ADDRD(1) |
						   R500_ALU_RGBA_SEL_C_SRC0 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_B |
						   R500_ALU_RGBA_ALPHA_SEL_C_SRC0 |
						   R500_ALU_RGBA_A_SWIZ_0));

	    /* MAD temp1.rgb, const1.rgb, temp0.bbb, temp1.rgb */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_ALU |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(1) |
						   R500_RGB_ADDR0_CONST |
						   R500_RGB_ADDR1(0) |
						   R500_RGB_ADDR2(1)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(1) |
						   R500_ALPHA_ADDR0_CONST |
						   R500_ALPHA_ADDR1(0) |
						   R500_ALPHA_ADDR2(1)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
						   R500_ALU_RGB_R_SWIZ_A_R |
						   R500_ALU_RGB_G_SWIZ_A_G |
						   R500_ALU_RGB_B_SWIZ_A_B |
						   R500_ALU_RGB_SEL_B_SRC1 |
						   R500_ALU_RGB_R_SWIZ_B_B |
						   R500_ALU_RGB_B_SWIZ_B_B |
						   R500_ALU_RGB_G_SWIZ_B_B));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_OP_MAD |
						   R500_ALPHA_ADDRD(1) |
						   R500_ALPHA_SWIZ_A_0 |
						   R500_ALPHA_SWIZ_B_0));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_ADDRD(1) |
						   R500_ALU_RGBA_SEL_C_SRC2 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_B |
						   R500_ALU_RGBA_ALPHA_SEL_C_SRC0 |
						   R500_ALU_RGBA_A_SWIZ_0));

	    /* MAD result.rgb, const2.rgb, temp0.rrr, temp1.rgb */
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_OUT |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_LAST |
						   R500_INST_RGB_OMASK_R |
						   R500_INST_RGB_OMASK_G |
						   R500_INST_RGB_OMASK_B |
						   R500_INST_ALPHA_OMASK |
						   R500_INST_RGB_CLAMP |
						   R500_INST_ALPHA_CLAMP));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(2) |
						   R500_RGB_ADDR0_CONST |
						   R500_RGB_ADDR1(0) |
						   R500_RGB_ADDR2(1)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(1) |
						   R500_ALPHA_ADDR0_CONST |
						   R500_ALPHA_ADDR1(0) |
						   R500_ALPHA_ADDR2(1)));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
						   R500_ALU_RGB_R_SWIZ_A_R |
						   R500_ALU_RGB_G_SWIZ_A_G |
						   R500_ALU_RGB_B_SWIZ_A_B |
						   R500_ALU_RGB_SEL_B_SRC1 |
						   R500_ALU_RGB_R_SWIZ_B_R |
						   R500_ALU_RGB_B_SWIZ_B_R |
						   R500_ALU_RGB_G_SWIZ_B_R));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_OP_MAD |
						   R500_ALPHA_ADDRD(1) |
						   R500_ALPHA_SWIZ_A_0 |
						   R500_ALPHA_SWIZ_B_0));
	    OUT_ACCEL_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_ADDRD(1) |
						   R500_ALU_RGBA_SEL_C_SRC2 |
						   R500_ALU_RGBA_R_SWIZ_R |
						   R500_ALU_RGBA_G_SWIZ_G |
						   R500_ALU_RGBA_B_SWIZ_B |
						   R500_ALU_RGBA_ALPHA_SEL_C_SRC0 |
						   R500_ALU_RGBA_A_SWIZ_1));
	}

	/* Shader constants. */
	OUT_ACCEL_REG(R500_GA_US_VECTOR_INDEX, R500_US_VECTOR_CONST_INDEX(0));

	/* constant 0: off, yco */
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, off[0]);
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, off[1]);
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, off[2]);
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, yco);
	/* constant 1: uco */
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, uco[0]);
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, uco[1]);
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, uco[2]);
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, gamma);
	/* constant 2: vco */
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, vco[0]);
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, vco[1]);
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, vco[2]);
	OUT_ACCEL_REG_F(R500_GA_US_VECTOR_DATA, 0.0);

	FINISH_ACCEL();
    }

    BEGIN_ACCEL_RELOC(6, 2);
    OUT_ACCEL_REG(R300_TX_INVALTAGS, 0);
    OUT_ACCEL_REG(R300_TX_ENABLE, txenable);

    EMIT_WRITE_OFFSET(R300_RB3D_COLOROFFSET0, 0, pPixmap);
    EMIT_COLORPITCH(R300_RB3D_COLORPITCH0, colorpitch, pPixmap);

    /* no need to enable blending */
    OUT_ACCEL_REG(R300_RB3D_BLENDCNTL, RADEON_SRC_BLEND_GL_ONE | RADEON_DST_BLEND_GL_ZERO);

    OUT_ACCEL_REG(R300_VAP_VTX_SIZE, pPriv->vtx_count);
    FINISH_ACCEL();

    if (pPriv->vsync) {
	xf86CrtcPtr crtc;
	if (pPriv->desired_crtc)
	    crtc = pPriv->desired_crtc;
	else
	    crtc = radeon_pick_best_crtc(pScrn,
					 pPriv->drw_x,
					 pPriv->drw_x + pPriv->dst_w,
					 pPriv->drw_y,
					 pPriv->drw_y + pPriv->dst_h);
	if (crtc)
	    FUNC_NAME(RADEONWaitForVLine)(pScrn, pPixmap,
					  crtc,
					  pPriv->drw_y - crtc->y,
					  (pPriv->drw_y - crtc->y) + pPriv->dst_h);
    }

    return TRUE;
}

static void
FUNC_NAME(R500DisplayTexturedVideo)(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    PixmapPtr pPixmap = pPriv->pPixmap;
    int dstxoff, dstyoff;
    BoxPtr pBox = REGION_RECTS(&pPriv->clip);
    int nBox = REGION_NUM_RECTS(&pPriv->clip);
    ACCEL_PREAMBLE();

#ifdef COMPOSITE
    dstxoff = -pPixmap->screen_x + pPixmap->drawable.x;
    dstyoff = -pPixmap->screen_y + pPixmap->drawable.y;
#else
    dstxoff = 0;
    dstyoff = 0;
#endif

    if (!FUNC_NAME(R500PrepareTexturedVideo)(pScrn, pPriv))
	return;

    /*
     * Rendering of the actual polygon is done in two different
     * ways depending on chip generation:
     *
     * < R300:
     *
     *     These chips can render a rectangle in one pass, so
     *     handling is pretty straight-forward.
     *
     * >= R300:
     *
     *     These chips can accept a quad, but will render it as
     *     two triangles which results in a diagonal tear. Instead
     *     We render a single, large triangle and use the scissor
     *     functionality to restrict it to the desired rectangle.
     *     Due to guardband limits on r3xx/r4xx, we can only use
     *     the single triangle up to 2880 pixels; above that we
     *     render as a quad.
     */

    while (nBox--) {
	float srcX, srcY, srcw, srch;
	int dstX, dstY, dstw, dsth;
#ifdef ACCEL_CP
	int draw_size = 3 * pPriv->vtx_count + 4 + 2 + 3;

	if (draw_size > radeon_cs_space_remaining(pScrn)) {
	    if (info->cs)
		radeon_cs_flush_indirect(pScrn);
	    else
		RADEONCPFlushIndirect(pScrn, 1);
	    if (!FUNC_NAME(R500PrepareTexturedVideo)(pScrn, pPriv))
		return;
	}
#endif

	dstX = pBox->x1 + dstxoff;
	dstY = pBox->y1 + dstyoff;
	dstw = pBox->x2 - pBox->x1;
	dsth = pBox->y2 - pBox->y1;

	srcX = pPriv->src_x;
	srcX += ((pBox->x1 - pPriv->drw_x) *
		 pPriv->src_w) / (float)pPriv->dst_w;
	srcY = pPriv->src_y;
	srcY += ((pBox->y1 - pPriv->drw_y) *
		 pPriv->src_h) / (float)pPriv->dst_h;

	srcw = (pPriv->src_w * dstw) / (float)pPriv->dst_w;
	srch = (pPriv->src_h * dsth) / (float)pPriv->dst_h;

	BEGIN_ACCEL(2);
	OUT_ACCEL_REG(R300_SC_SCISSOR0, (((dstX) << R300_SCISSOR_X_SHIFT) |
					 ((dstY) << R300_SCISSOR_Y_SHIFT)));
	OUT_ACCEL_REG(R300_SC_SCISSOR1, (((dstX + dstw - 1) << R300_SCISSOR_X_SHIFT) |
					 ((dstY + dsth - 1) << R300_SCISSOR_Y_SHIFT)));
	FINISH_ACCEL();

#ifdef ACCEL_CP
	BEGIN_RING(3 * pPriv->vtx_count + 4);
	OUT_RING(CP_PACKET3(R200_CP_PACKET3_3D_DRAW_IMMD_2,
			    3 * pPriv->vtx_count));
	OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_TRI_LIST |
		 RADEON_CP_VC_CNTL_PRIM_WALK_RING |
		 (3 << RADEON_CP_VC_CNTL_NUM_SHIFT));
#else /* ACCEL_CP */
	BEGIN_ACCEL(2 + pPriv->vtx_count * 3);
	OUT_ACCEL_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_TRIANGLE_LIST |
					  RADEON_VF_PRIM_WALK_DATA |
					  (3 << RADEON_VF_NUM_VERTICES_SHIFT)));
#endif
	if (pPriv->bicubic_enabled) {
	    VTX_OUT_6((float)dstX,            (float)dstY,
		      (float)srcX / pPriv->w, (float)srcY / pPriv->h,
		      (float)srcX + 0.5,      (float)srcY + 0.5);
	    VTX_OUT_6((float)dstX,            (float)(dstY + dstw + dsth),
		      (float)srcX / pPriv->w, ((float)srcY + (float)srch * (((float)dstw / (float)dsth) + 1.0)) / pPriv->h,
		      (float)srcX + 0.5,      (float)srcY + (float)srch * (((float)dstw / (float)dsth) + 1.0) + 0.5);
	    VTX_OUT_6((float)(dstX + dstw + dsth),                       (float)dstY,
		      ((float)srcX + (float)srcw * (((float)dsth / (float)dstw) + 1.0)) / pPriv->w,
		      (float)srcY / pPriv->h,
		      (float)srcX + (float)srcw * (((float)dsth / (float)dstw) + 1.0) + 0.5,
		      (float)srcY + 0.5);
	} else {
	    /*
	     * Render a big, scissored triangle. This means
	     * increasing the triangle size and adjusting
	     * texture coordinates.
	     */
	    VTX_OUT_4((float)dstX,            (float)dstY,
		      (float)srcX / pPriv->w, (float)srcY / pPriv->h);
	    VTX_OUT_4((float)dstX,                              (float)(dstY + dsth + dstw),
		      (float)srcX / pPriv->w, ((float)srcY + (float)srch * (((float)dstw / (float)dsth) + 1.0)) / pPriv->h);
	    VTX_OUT_4((float)(dstX + dstw + dsth),              (float)dstY,
		      ((float)srcX + (float)srcw * (((float)dsth / (float)dstw) + 1.0)) / pPriv->w,
		      (float)srcY / pPriv->h);
	}

	/* flushing is pipelined, free/finish is not */
	OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D);

#ifdef ACCEL_CP
	ADVANCE_RING();
#else
	FINISH_ACCEL();
#endif /* !ACCEL_CP */

	pBox++;
    }

    BEGIN_ACCEL(3);
    OUT_ACCEL_REG(R300_SC_CLIP_RULE, 0xAAAA);
    OUT_ACCEL_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_RB3D_DC_FLUSH_ALL);
    OUT_ACCEL_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);
    FINISH_ACCEL();

    DamageDamageRegion(pPriv->pDraw, &pPriv->clip);
}

#undef VTX_OUT_4
#undef VTX_OUT_6
#undef FUNC_NAME
