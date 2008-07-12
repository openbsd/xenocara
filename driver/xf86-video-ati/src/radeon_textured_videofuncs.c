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

#define VTX_DWORD_COUNT 4

#ifdef ACCEL_CP

#define VTX_OUT(_dstX, _dstY, _srcX, _srcY)	\
do {								\
    OUT_VIDEO_RING_F(_dstX);						\
    OUT_VIDEO_RING_F(_dstY);						\
    OUT_VIDEO_RING_F(_srcX);						\
    OUT_VIDEO_RING_F(_srcY);						\
} while (0)

#else /* ACCEL_CP */

#define VTX_OUT(_dstX, _dstY, _srcX, _srcY)	\
do {								\
    OUT_VIDEO_REG_F(RADEON_SE_PORT_DATA0, _dstX);		\
    OUT_VIDEO_REG_F(RADEON_SE_PORT_DATA0, _dstY);		\
    OUT_VIDEO_REG_F(RADEON_SE_PORT_DATA0, _srcX);		\
    OUT_VIDEO_REG_F(RADEON_SE_PORT_DATA0, _srcY);		\
} while (0)

#endif /* !ACCEL_CP */

static void
FUNC_NAME(RADEONDisplayTexturedVideo)(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    PixmapPtr pPixmap = pPriv->pPixmap;
    uint32_t txformat;
    uint32_t txfilter, txformat0, txformat1, txoffset, txpitch;
    uint32_t dst_offset, dst_pitch, dst_format;
    uint32_t txenable, colorpitch;
    uint32_t blendcntl;
    int dstxoff, dstyoff, pixel_shift;
    BoxPtr pBox = REGION_RECTS(&pPriv->clip);
    int nBox = REGION_NUM_RECTS(&pPriv->clip);
    VIDEO_PREAMBLE();

    pixel_shift = pPixmap->drawable.bitsPerPixel >> 4;

#ifdef USE_EXA
    if (info->useEXA) {
	dst_offset = exaGetPixmapOffset(pPixmap) + info->fbLocation + pScrn->fbOffset;
	dst_pitch = exaGetPixmapPitch(pPixmap);
    } else
#endif
	{
	    dst_offset = (pPixmap->devPrivate.ptr - info->FB) +
		info->fbLocation + pScrn->fbOffset;
	    dst_pitch = pPixmap->devKind;
	}

#ifdef COMPOSITE
    dstxoff = -pPixmap->screen_x + pPixmap->drawable.x;
    dstyoff = -pPixmap->screen_y + pPixmap->drawable.y;
#else
    dstxoff = 0;
    dstyoff = 0;
#endif

    if (!info->XInited3D)
	RADEONInit3DEngine(pScrn);

    /* we can probably improve this */
    BEGIN_VIDEO(2);
    if (IS_R300_3D || IS_R500_3D)
	OUT_VIDEO_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D);
    else
	OUT_VIDEO_REG(RADEON_RB3D_DSTCACHE_CTLSTAT, RADEON_RB3D_DC_FLUSH);
    /* We must wait for 3d to idle, in case source was just written as a dest. */
    OUT_VIDEO_REG(RADEON_WAIT_UNTIL,
		  RADEON_WAIT_HOST_IDLECLEAN |
		  RADEON_WAIT_2D_IDLECLEAN |
		  RADEON_WAIT_3D_IDLECLEAN |
		  RADEON_WAIT_DMA_GUI_IDLE);
    FINISH_VIDEO();

    if (IS_R300_3D || IS_R500_3D) {
	uint32_t output_fmt;

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
	    return;
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

	if (pPriv->id == FOURCC_UYVY)
	    txformat1 = R300_TX_FORMAT_YVYU422;
	else
	    txformat1 = R300_TX_FORMAT_VYUY422;

	txformat1 |= R300_TX_FORMAT_YUV_TO_RGB_CLAMP;

	txformat0 = ((((pPriv->w - 1) & 0x7ff) << R300_TXWIDTH_SHIFT) |
		     (((pPriv->h - 1) & 0x7ff) << R300_TXHEIGHT_SHIFT));

	txformat0 |= R300_TXPITCH_EN;

	info->texW[0] = pPriv->w;
	info->texH[0] = pPriv->h;

	txfilter = (R300_TX_CLAMP_S(R300_TX_CLAMP_CLAMP_LAST) |
		    R300_TX_CLAMP_T(R300_TX_CLAMP_CLAMP_LAST) |
		    R300_TX_MAG_FILTER_LINEAR | R300_TX_MIN_FILTER_LINEAR);

	/* pitch is in pixels */
	txpitch = pPriv->src_pitch / 2;
	txpitch -= 1;

	if (IS_R500_3D && ((pPriv->w - 1) & 0x800))
	    txpitch |= R500_TXWIDTH_11;

	if (IS_R500_3D && ((pPriv->h - 1) & 0x800))
	    txpitch |= R500_TXHEIGHT_11;

	txoffset = pPriv->src_offset;

	BEGIN_VIDEO(6);
	OUT_VIDEO_REG(R300_TX_FILTER0_0, txfilter);
	OUT_VIDEO_REG(R300_TX_FILTER1_0, 0);
	OUT_VIDEO_REG(R300_TX_FORMAT0_0, txformat0);
	OUT_VIDEO_REG(R300_TX_FORMAT1_0, txformat1);
	OUT_VIDEO_REG(R300_TX_FORMAT2_0, txpitch);
	OUT_VIDEO_REG(R300_TX_OFFSET_0, txoffset);
	FINISH_VIDEO();

	txenable = R300_TEX_0_ENABLE;

	/* setup the VAP */
	if (info->has_tcl)
	    BEGIN_VIDEO(6);
	else
	    BEGIN_VIDEO(4);

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
	OUT_VIDEO_REG(R300_VAP_PROG_STREAM_CNTL_0,
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
	if (info->has_tcl) {
	    OUT_VIDEO_REG(R300_VAP_PVS_CODE_CNTL_0,
			  ((5 << R300_PVS_FIRST_INST_SHIFT) |
			   (6 << R300_PVS_XYZW_VALID_INST_SHIFT) |
			   (6 << R300_PVS_LAST_INST_SHIFT)));
	    OUT_VIDEO_REG(R300_VAP_PVS_CODE_CNTL_1,
			  (6 << R300_PVS_LAST_VTX_SRC_INST_SHIFT));
	}

	/* Position and one set of 2 texture coordinates */
	OUT_VIDEO_REG(R300_VAP_OUT_VTX_FMT_0, R300_VTX_POS_PRESENT);
	OUT_VIDEO_REG(R300_VAP_OUT_VTX_FMT_1, (2 << R300_TEX_0_COMP_CNT_SHIFT));
	OUT_VIDEO_REG(R300_US_OUT_FMT_0, output_fmt);
	FINISH_VIDEO();

	/* setup pixel shader */
	if (IS_R300_3D) {
	    BEGIN_VIDEO(8);
	    /* 2 components: 2 for tex0 */
	    OUT_VIDEO_REG(R300_RS_COUNT,
			  ((2 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));
	    /* R300_INST_COUNT_RS - highest RS instruction used */
	    OUT_VIDEO_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(0) | R300_TX_OFFSET_RS(6));

	    OUT_VIDEO_REG(R300_US_CODE_OFFSET,
			  (R300_ALU_CODE_OFFSET(0) |
			   R300_ALU_CODE_SIZE(1) |
			   R300_TEX_CODE_OFFSET(0) |
			   R300_TEX_CODE_SIZE(1)));

	    OUT_VIDEO_REG(R300_US_CODE_ADDR_3,
			  (R300_ALU_START(0) |
			   R300_ALU_SIZE(0) |
			   R300_TEX_START(0) |
			   R300_TEX_SIZE(0) |
			   R300_RGBA_OUT));

	    /* tex inst is preloaded in RADEONInit3DEngine() */

	    /* ALU inst */
	    /* RGB */
	    OUT_VIDEO_REG(R300_US_ALU_RGB_ADDR_0,
			  (R300_ALU_RGB_ADDR0(0) |
			   R300_ALU_RGB_ADDR1(0) |
			   R300_ALU_RGB_ADDR2(0) |
			   R300_ALU_RGB_ADDRD(0) |
			   R300_ALU_RGB_OMASK((R300_ALU_RGB_MASK_R |
					       R300_ALU_RGB_MASK_G |
					       R300_ALU_RGB_MASK_B)) |
			   R300_ALU_RGB_TARGET_A));
	    OUT_VIDEO_REG(R300_US_ALU_RGB_INST_0,
			  (R300_ALU_RGB_SEL_A(R300_ALU_RGB_SRC0_RGB) |
			   R300_ALU_RGB_MOD_A(R300_ALU_RGB_MOD_NOP) |
			   R300_ALU_RGB_SEL_B(R300_ALU_RGB_1_0) |
			   R300_ALU_RGB_MOD_B(R300_ALU_RGB_MOD_NOP) |
			   R300_ALU_RGB_SEL_C(R300_ALU_RGB_0_0) |
			   R300_ALU_RGB_MOD_C(R300_ALU_RGB_MOD_NOP) |
			   R300_ALU_RGB_OP(R300_ALU_RGB_OP_MAD) |
			   R300_ALU_RGB_OMOD(R300_ALU_RGB_OMOD_NONE) |
			   R300_ALU_RGB_CLAMP));
	    /* Alpha */
	    OUT_VIDEO_REG(R300_US_ALU_ALPHA_ADDR_0,
			  (R300_ALU_ALPHA_ADDR0(0) |
			   R300_ALU_ALPHA_ADDR1(0) |
			   R300_ALU_ALPHA_ADDR2(0) |
			   R300_ALU_ALPHA_ADDRD(0) |
			   R300_ALU_ALPHA_OMASK(R300_ALU_ALPHA_MASK_A) |
			   R300_ALU_ALPHA_TARGET_A |
			   R300_ALU_ALPHA_OMASK_W(R300_ALU_ALPHA_MASK_NONE)));
	    OUT_VIDEO_REG(R300_US_ALU_ALPHA_INST_0,
			  (R300_ALU_ALPHA_SEL_A(R300_ALU_ALPHA_SRC0_A) |
			   R300_ALU_ALPHA_MOD_A(R300_ALU_ALPHA_MOD_NOP) |
			   R300_ALU_ALPHA_SEL_B(R300_ALU_ALPHA_1_0) |
			   R300_ALU_ALPHA_MOD_B(R300_ALU_ALPHA_MOD_NOP) |
			   R300_ALU_ALPHA_SEL_C(R300_ALU_ALPHA_0_0) |
			   R300_ALU_ALPHA_MOD_C(R300_ALU_ALPHA_MOD_NOP) |
			   R300_ALU_ALPHA_OP(R300_ALU_ALPHA_OP_MAD) |
			   R300_ALU_ALPHA_OMOD(R300_ALU_ALPHA_OMOD_NONE) |
			   R300_ALU_ALPHA_CLAMP));
	    FINISH_VIDEO();
	} else {
	    BEGIN_VIDEO(18);
	    /* 2 components: 2 for tex0 */
	    OUT_VIDEO_REG(R300_RS_COUNT,
			  ((2 << R300_RS_COUNT_IT_COUNT_SHIFT) |
			   R300_RS_COUNT_HIRES_EN));

	    /* R300_INST_COUNT_RS - highest RS instruction used */
	    OUT_VIDEO_REG(R300_RS_INST_COUNT, R300_INST_COUNT_RS(0) | R300_TX_OFFSET_RS(6));

	    OUT_VIDEO_REG(R500_US_CODE_ADDR, (R500_US_CODE_START_ADDR(0) |
					      R500_US_CODE_END_ADDR(1)));
	    OUT_VIDEO_REG(R500_US_CODE_RANGE, (R500_US_CODE_RANGE_ADDR(0) |
					       R500_US_CODE_RANGE_SIZE(1)));
	    OUT_VIDEO_REG(R500_US_CODE_OFFSET, 0);
	    OUT_VIDEO_REG(R500_GA_US_VECTOR_INDEX, 0);

	    /* tex inst */
	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_TEX |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_RGB_WMASK_R |
						   R500_INST_RGB_WMASK_G |
						   R500_INST_RGB_WMASK_B |
						   R500_INST_ALPHA_WMASK |
						   R500_INST_RGB_CLAMP |
						   R500_INST_ALPHA_CLAMP));

	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_ID(0) |
						   R500_TEX_INST_LD |
						   R500_TEX_SEM_ACQUIRE |
						   R500_TEX_IGNORE_UNCOVERED));

	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, (R500_TEX_SRC_ADDR(0) |
						   R500_TEX_SRC_S_SWIZ_R |
						   R500_TEX_SRC_T_SWIZ_G |
						   R500_TEX_DST_ADDR(0) |
						   R500_TEX_DST_R_SWIZ_R |
						   R500_TEX_DST_G_SWIZ_G |
						   R500_TEX_DST_B_SWIZ_B |
						   R500_TEX_DST_A_SWIZ_A));
	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, (R500_DX_ADDR(0) |
						   R500_DX_S_SWIZ_R |
						   R500_DX_T_SWIZ_R |
						   R500_DX_R_SWIZ_R |
						   R500_DX_Q_SWIZ_R |
						   R500_DY_ADDR(0) |
						   R500_DY_S_SWIZ_R |
						   R500_DY_T_SWIZ_R |
						   R500_DY_R_SWIZ_R |
						   R500_DY_Q_SWIZ_R));
	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, 0x00000000);
	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, 0x00000000);

	    /* ALU inst */
	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, (R500_INST_TYPE_OUT |
						   R500_INST_TEX_SEM_WAIT |
						   R500_INST_LAST |
						   R500_INST_RGB_OMASK_R |
						   R500_INST_RGB_OMASK_G |
						   R500_INST_RGB_OMASK_B |
						   R500_INST_ALPHA_OMASK |
						   R500_INST_RGB_CLAMP |
						   R500_INST_ALPHA_CLAMP));

	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, (R500_RGB_ADDR0(0) |
						   R500_RGB_ADDR1(0) |
						   R500_RGB_ADDR1_CONST |
						   R500_RGB_ADDR2(0) |
						   R500_RGB_ADDR2_CONST));
	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_ADDR0(0) |
						   R500_ALPHA_ADDR1(0) |
						   R500_ALPHA_ADDR1_CONST |
						   R500_ALPHA_ADDR2(0) |
						   R500_ALPHA_ADDR2_CONST));

	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGB_SEL_A_SRC0 |
						   R500_ALU_RGB_R_SWIZ_A_R |
						   R500_ALU_RGB_G_SWIZ_A_G |
						   R500_ALU_RGB_B_SWIZ_A_B |
						   R500_ALU_RGB_SEL_B_SRC0 |
						   R500_ALU_RGB_R_SWIZ_B_1 |
						   R500_ALU_RGB_B_SWIZ_B_1 |
						   R500_ALU_RGB_G_SWIZ_B_1));

	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, (R500_ALPHA_OP_MAD |
						   R500_ALPHA_SWIZ_A_A |
						   R500_ALPHA_SWIZ_B_1));

	    OUT_VIDEO_REG(R500_GA_US_VECTOR_DATA, (R500_ALU_RGBA_OP_MAD |
						   R500_ALU_RGBA_R_SWIZ_0 |
						   R500_ALU_RGBA_G_SWIZ_0 |
						   R500_ALU_RGBA_B_SWIZ_0 |
						   R500_ALU_RGBA_A_SWIZ_0));
	    FINISH_VIDEO();
	}

	BEGIN_VIDEO(5);
	OUT_VIDEO_REG(R300_TX_INVALTAGS, 0);
	OUT_VIDEO_REG(R300_TX_ENABLE, txenable);

	OUT_VIDEO_REG(R300_RB3D_COLOROFFSET0, dst_offset);
	OUT_VIDEO_REG(R300_RB3D_COLORPITCH0, colorpitch);

	blendcntl = RADEON_SRC_BLEND_GL_ONE | RADEON_DST_BLEND_GL_ZERO;
	/* no need to enable blending */
	OUT_VIDEO_REG(R300_RB3D_BLENDCNTL, blendcntl);
	FINISH_VIDEO();

	BEGIN_VIDEO(1);
	OUT_VIDEO_REG(R300_VAP_VTX_SIZE, VTX_DWORD_COUNT);
	FINISH_VIDEO();

    } else {

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
	    return;
	}

	if (pPriv->id == FOURCC_UYVY)
	    txformat = RADEON_TXFORMAT_YVYU422;
	else
	    txformat = RADEON_TXFORMAT_VYUY422;

	txformat |= RADEON_TXFORMAT_NON_POWER2;

	colorpitch = dst_pitch >> pixel_shift;

	if (RADEONTilingEnabled(pScrn, pPixmap))
	    colorpitch |= RADEON_COLOR_TILE_ENABLE;

	BEGIN_VIDEO(5);

	OUT_VIDEO_REG(RADEON_PP_CNTL,
		    RADEON_TEX_0_ENABLE | RADEON_TEX_BLEND_0_ENABLE);
	OUT_VIDEO_REG(RADEON_RB3D_CNTL,
		    dst_format | RADEON_ALPHA_BLEND_ENABLE);
	OUT_VIDEO_REG(RADEON_RB3D_COLOROFFSET, dst_offset);

	OUT_VIDEO_REG(RADEON_RB3D_COLORPITCH, colorpitch);

	OUT_VIDEO_REG(RADEON_RB3D_BLENDCNTL,
		    RADEON_SRC_BLEND_GL_ONE | RADEON_DST_BLEND_GL_ZERO);

	FINISH_VIDEO();


	if ((info->ChipFamily == CHIP_FAMILY_RV250) ||
	    (info->ChipFamily == CHIP_FAMILY_RV280) ||
	    (info->ChipFamily == CHIP_FAMILY_RS300) ||
	    (info->ChipFamily == CHIP_FAMILY_R200)) {

	    info->texW[0] = pPriv->w;
	    info->texH[0] = pPriv->h;

	    BEGIN_VIDEO(12);

	    OUT_VIDEO_REG(R200_SE_VTX_FMT_0, R200_VTX_XY);
	    OUT_VIDEO_REG(R200_SE_VTX_FMT_1,
			(2 << R200_VTX_TEX0_COMP_CNT_SHIFT));

	    OUT_VIDEO_REG(R200_PP_TXFILTER_0,
			R200_MAG_FILTER_LINEAR |
			R200_MIN_FILTER_LINEAR |
			R200_YUV_TO_RGB);
	    OUT_VIDEO_REG(R200_PP_TXFORMAT_0, txformat);
	    OUT_VIDEO_REG(R200_PP_TXFORMAT_X_0, 0);
	    OUT_VIDEO_REG(R200_PP_TXSIZE_0,
			(pPriv->w - 1) |
			((pPriv->h - 1) << RADEON_TEX_VSIZE_SHIFT));
	    OUT_VIDEO_REG(R200_PP_TXPITCH_0, pPriv->src_pitch - 32);

	    OUT_VIDEO_REG(R200_PP_TXOFFSET_0, pPriv->src_offset);

	    OUT_VIDEO_REG(R200_PP_TXCBLEND_0,
			R200_TXC_ARG_A_ZERO |
			R200_TXC_ARG_B_ZERO |
			R200_TXC_ARG_C_R0_COLOR |
			R200_TXC_OP_MADD);
	    OUT_VIDEO_REG(R200_PP_TXCBLEND2_0,
			R200_TXC_CLAMP_0_1 | R200_TXC_OUTPUT_REG_R0);
	    OUT_VIDEO_REG(R200_PP_TXABLEND_0,
			R200_TXA_ARG_A_ZERO |
			R200_TXA_ARG_B_ZERO |
			R200_TXA_ARG_C_R0_ALPHA |
			R200_TXA_OP_MADD);
	    OUT_VIDEO_REG(R200_PP_TXABLEND2_0,
			R200_TXA_CLAMP_0_1 | R200_TXA_OUTPUT_REG_R0);
	    FINISH_VIDEO();
	} else {

	    info->texW[0] = 1;
	    info->texH[0] = 1;

	    BEGIN_VIDEO(8);

	    OUT_VIDEO_REG(RADEON_SE_VTX_FMT, RADEON_SE_VTX_FMT_XY |
			RADEON_SE_VTX_FMT_ST0);

	    OUT_VIDEO_REG(RADEON_PP_TXFILTER_0, RADEON_MAG_FILTER_LINEAR |
			RADEON_MIN_FILTER_LINEAR |
			RADEON_YUV_TO_RGB);
	    OUT_VIDEO_REG(RADEON_PP_TXFORMAT_0, txformat);
	    OUT_VIDEO_REG(RADEON_PP_TXOFFSET_0, pPriv->src_offset);
	    OUT_VIDEO_REG(RADEON_PP_TXCBLEND_0,
			RADEON_COLOR_ARG_A_ZERO |
			RADEON_COLOR_ARG_B_ZERO |
			RADEON_COLOR_ARG_C_T0_COLOR |
			RADEON_BLEND_CTL_ADD |
			RADEON_CLAMP_TX);
	    OUT_VIDEO_REG(RADEON_PP_TXABLEND_0,
			RADEON_ALPHA_ARG_A_ZERO |
			RADEON_ALPHA_ARG_B_ZERO |
			RADEON_ALPHA_ARG_C_T0_ALPHA |
			RADEON_BLEND_CTL_ADD |
			RADEON_CLAMP_TX);

	    OUT_VIDEO_REG(RADEON_PP_TEX_SIZE_0,
			(pPriv->w - 1) |
			((pPriv->h - 1) << RADEON_TEX_VSIZE_SHIFT));
	    OUT_VIDEO_REG(RADEON_PP_TEX_PITCH_0,
			pPriv->src_pitch - 32);
	    FINISH_VIDEO();
	}
    }

    while (nBox--) {
	int srcX, srcY, srcw, srch;
	int dstX, dstY, dstw, dsth;
	xPointFixed srcTopLeft, srcTopRight, srcBottomLeft, srcBottomRight;
	dstX = pBox->x1 + dstxoff;
	dstY = pBox->y1 + dstyoff;
	dstw = pBox->x2 - pBox->x1;
	dsth = pBox->y2 - pBox->y1;

	srcX = ((pBox->x1 - pPriv->drw_x) *
		pPriv->src_w) / pPriv->dst_w;
	srcY = ((pBox->y1 - pPriv->drw_y) *
		pPriv->src_h) / pPriv->dst_h;

	srcw = (pPriv->src_w * dstw) / pPriv->dst_w;
	srch = (pPriv->src_h * dsth) / pPriv->dst_h;

	srcTopLeft.x     = IntToxFixed(srcX);
	srcTopLeft.y     = IntToxFixed(srcY);
	srcTopRight.x    = IntToxFixed(srcX + srcw);
	srcTopRight.y    = IntToxFixed(srcY);
	srcBottomLeft.x  = IntToxFixed(srcX);
	srcBottomLeft.y  = IntToxFixed(srcY + srch);
	srcBottomRight.x = IntToxFixed(srcX + srcw);
	srcBottomRight.y = IntToxFixed(srcY + srch);


#if 0
	ErrorF("dst: %d, %d, %d, %d\n", dstX, dstY, dstw, dsth);
	ErrorF("src: %d, %d, %d, %d\n", srcX, srcY, srcw, srch);
#endif

#ifdef ACCEL_CP
	if (info->ChipFamily < CHIP_FAMILY_R200) {
	    BEGIN_RING(4 * VTX_DWORD_COUNT + 3);
	    OUT_RING(CP_PACKET3(RADEON_CP_PACKET3_3D_DRAW_IMMD,
				4 * VTX_DWORD_COUNT + 1));
	    OUT_RING(RADEON_CP_VC_FRMT_XY |
		     RADEON_CP_VC_FRMT_ST0);
	    OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_TRI_FAN |
		     RADEON_CP_VC_CNTL_PRIM_WALK_RING |
		     RADEON_CP_VC_CNTL_MAOS_ENABLE |
		     RADEON_CP_VC_CNTL_VTX_FMT_RADEON_MODE |
		     (4 << RADEON_CP_VC_CNTL_NUM_SHIFT));
	} else {
	    if (IS_R300_3D || IS_R500_3D)
		BEGIN_RING(4 * VTX_DWORD_COUNT + 4);
	    else
		BEGIN_RING(4 * VTX_DWORD_COUNT + 2);
	    OUT_RING(CP_PACKET3(R200_CP_PACKET3_3D_DRAW_IMMD_2,
				4 * VTX_DWORD_COUNT));
	    OUT_RING(RADEON_CP_VC_CNTL_PRIM_TYPE_TRI_FAN |
		     RADEON_CP_VC_CNTL_PRIM_WALK_RING |
		     (4 << RADEON_CP_VC_CNTL_NUM_SHIFT));
	}
#else /* ACCEL_CP */
	if (IS_R300_3D || IS_R500_3D)
	    BEGIN_VIDEO(2 + VTX_DWORD_COUNT * 4);
	else
	    BEGIN_VIDEO(1 + VTX_DWORD_COUNT * 4);

	if (info->ChipFamily < CHIP_FAMILY_R200) {
	    OUT_VIDEO_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_TRIANGLE_FAN |
					      RADEON_VF_PRIM_WALK_DATA |
					      RADEON_VF_RADEON_MODE |
					      4 << RADEON_VF_NUM_VERTICES_SHIFT));
	} else {
	    OUT_VIDEO_REG(RADEON_SE_VF_CNTL, (RADEON_VF_PRIM_TYPE_QUAD_LIST |
					      RADEON_VF_PRIM_WALK_DATA |
					      4 << RADEON_VF_NUM_VERTICES_SHIFT));
	}
#endif

	VTX_OUT((float)dstX,                                      (float)dstY,
		xFixedToFloat(srcTopLeft.x) / info->texW[0],      xFixedToFloat(srcTopLeft.y) / info->texH[0]);
	VTX_OUT((float)dstX,                                      (float)(dstY + dsth),
		xFixedToFloat(srcBottomLeft.x) / info->texW[0],   xFixedToFloat(srcBottomLeft.y) / info->texH[0]);
	VTX_OUT((float)(dstX + dstw),                                (float)(dstY + dsth),
		xFixedToFloat(srcBottomRight.x) / info->texW[0],  xFixedToFloat(srcBottomRight.y) / info->texH[0]);
	VTX_OUT((float)(dstX + dstw),                                (float)dstY,
		xFixedToFloat(srcTopRight.x) / info->texW[0],     xFixedToFloat(srcTopRight.y) / info->texH[0]);

	if (IS_R300_3D || IS_R500_3D)
	    /* flushing is pipelined, free/finish is not */
	    OUT_VIDEO_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D);

#ifdef ACCEL_CP
	ADVANCE_RING();
#else
	FINISH_VIDEO();
#endif /* !ACCEL_CP */

	pBox++;
    }

    if (IS_R300_3D || IS_R500_3D) {
	BEGIN_VIDEO(2);
	OUT_VIDEO_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_RB3D_DC_FLUSH_ALL);
    } else
	BEGIN_VIDEO(1);
    OUT_VIDEO_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_3D_IDLECLEAN);
    FINISH_VIDEO();

    DamageDamageRegion(pPriv->pDraw, &pPriv->clip);
}

#undef VTX_OUT
#undef FUNC_NAME
