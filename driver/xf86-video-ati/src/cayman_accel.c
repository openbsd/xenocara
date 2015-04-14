/*
 * Copyright 2011 Advanced Micro Devices, Inc.
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
 * Authors: Alex Deucher <alexander.deucher@amd.com>
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include <errno.h>

#include "radeon.h"
#include "radeon_reg.h"
#include "cayman_reg.h"
#include "evergreen_state.h"

#include "radeon_vbo.h"
#include "radeon_exa_shared.h"

/*
 * Setup of default state
 */

void
cayman_set_default_state(ScrnInfoPtr pScrn)
{
    tex_resource_t tex_res;
    shader_config_t fs_conf;
    int i;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    if (accel_state->XInited3D)
	return;

    memset(&tex_res, 0, sizeof(tex_resource_t));
    memset(&fs_conf, 0, sizeof(shader_config_t));

    accel_state->XInited3D = TRUE;

    evergreen_start_3d(pScrn);

    BEGIN_BATCH(21);
    EREG(SQ_LDS_ALLOC_PS, 0);

    PACK0(SQ_ESGS_RING_ITEMSIZE, 6);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);

    PACK0(SQ_GS_VERT_ITEMSIZE, 4);
    E32(0);
    E32(0);
    E32(0);
    E32(0);

    PACK0(SQ_VTX_BASE_VTX_LOC, 2);
    E32(0);
    E32(0);
    END_BATCH();

    /* DB */
    BEGIN_BATCH(3 + 2);
    EREG(DB_Z_INFO,                           0);
    RELOC_BATCH(accel_state->shaders_bo, RADEON_GEM_DOMAIN_VRAM, 0);
    END_BATCH();

    BEGIN_BATCH(3 + 2);
    EREG(DB_STENCIL_INFO,                     0);
    RELOC_BATCH(accel_state->shaders_bo, RADEON_GEM_DOMAIN_VRAM, 0);
    END_BATCH();

    BEGIN_BATCH(3 + 2);
    EREG(DB_HTILE_DATA_BASE,                    0);
    RELOC_BATCH(accel_state->shaders_bo, RADEON_GEM_DOMAIN_VRAM, 0);
    END_BATCH();

    BEGIN_BATCH(52);
    EREG(DB_DEPTH_INFO,                       0);
    EREG(DB_DEPTH_CONTROL,                    0);

    PACK0(PA_SC_VPORT_ZMIN_0, 2);
    EFLOAT(0.0); // PA_SC_VPORT_ZMIN_0
    EFLOAT(1.0); // PA_SC_VPORT_ZMAX_0

    PACK0(DB_RENDER_CONTROL, 5);
    E32(STENCIL_COMPRESS_DISABLE_bit | DEPTH_COMPRESS_DISABLE_bit); // DB_RENDER_CONTROL
    E32(0); // DB_COUNT_CONTROL
    E32(0); // DB_DEPTH_VIEW
    E32(0x2a); // DB_RENDER_OVERRIDE
    E32(0); // DB_RENDER_OVERRIDE2

    PACK0(DB_STENCIL_CLEAR, 2);
    E32(0); // DB_STENCIL_CLEAR
    E32(0); // DB_DEPTH_CLEAR

    EREG(DB_ALPHA_TO_MASK,                    ((2 << ALPHA_TO_MASK_OFFSET0_shift)	|
					       (2 << ALPHA_TO_MASK_OFFSET1_shift)	|
					       (2 << ALPHA_TO_MASK_OFFSET2_shift)	|
					       (2 << ALPHA_TO_MASK_OFFSET3_shift)));

    EREG(DB_SHADER_CONTROL, ((EARLY_Z_THEN_LATE_Z << Z_ORDER_shift) |
			     DUAL_EXPORT_ENABLE_bit)); /* Only useful if no depth export */

    // SX
    EREG(SX_MISC,               0);

    // CB
    PACK0(SX_ALPHA_TEST_CONTROL, 5);
    E32(0); // SX_ALPHA_TEST_CONTROL
    E32(0x00000000); //CB_BLEND_RED
    E32(0x00000000); //CB_BLEND_GREEN
    E32(0x00000000); //CB_BLEND_BLUE
    E32(0x00000000); //CB_BLEND_ALPHA

    EREG(CB_SHADER_MASK,                      OUTPUT0_ENABLE_mask);

    // SC
    EREG(PA_SC_WINDOW_OFFSET,                 ((0 << WINDOW_X_OFFSET_shift) |
					       (0 << WINDOW_Y_OFFSET_shift)));
    EREG(PA_SC_CLIPRECT_RULE,                 CLIP_RULE_mask);
    EREG(PA_SC_EDGERULE,             0xAAAAAAAA);
    EREG(PA_SU_HARDWARE_SCREEN_OFFSET, 0);
    END_BATCH();

    /* clip boolean is set to always visible -> doesn't matter */
    for (i = 0; i < PA_SC_CLIPRECT_0_TL_num; i++)
	evergreen_set_clip_rect (pScrn, i, 0, 0, 8192, 8192);

    for (i = 0; i < PA_SC_VPORT_SCISSOR_0_TL_num; i++)
	evergreen_set_vport_scissor (pScrn, i, 0, 0, 8192, 8192);

    BEGIN_BATCH(73);
    PACK0(PA_SC_MODE_CNTL_0, 2);
    E32(0); // PA_SC_MODE_CNTL_0
    E32(0); // PA_SC_MODE_CNTL_1

    PACK0(PA_SC_CENTROID_PRIORITY_0, 27);
    E32((0 << DISTANCE_0_shift) |
	(1 << DISTANCE_1_shift) |
	(2 << DISTANCE_2_shift) |
	(3 << DISTANCE_3_shift) |
	(4 << DISTANCE_4_shift) |
	(5 << DISTANCE_5_shift) |
	(6 << DISTANCE_6_shift) |
	(7 << DISTANCE_7_shift)); // PA_SC_CENTROID_PRIORITY_0
    E32((8 << DISTANCE_8_shift) |
	(9 << DISTANCE_9_shift) |
	(10 << DISTANCE_10_shift) |
	(11 << DISTANCE_11_shift) |
	(12 << DISTANCE_12_shift) |
	(13 << DISTANCE_13_shift) |
	(14 << DISTANCE_14_shift) |
	(15 << DISTANCE_15_shift)); // PA_SC_CENTROID_PRIORITY_1
    E32(0); // PA_SC_LINE_CNTL
    E32(0); // PA_SC_AA_CONFIG
    E32(((X_ROUND_TO_EVEN << PA_SU_VTX_CNTL__ROUND_MODE_shift) |
	 PIX_CENTER_bit)); // PA_SU_VTX_CNTL
    EFLOAT(1.0);						// PA_CL_GB_VERT_CLIP_ADJ
    EFLOAT(1.0);						// PA_CL_GB_VERT_DISC_ADJ
    EFLOAT(1.0);						// PA_CL_GB_HORZ_CLIP_ADJ
    EFLOAT(1.0);						// PA_CL_GB_HORZ_DISC_ADJ
    E32(0); // PA_SC_AA_SAMPLE_LOCS_PIXEL_*
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0); // PA_SC_AA_SAMPLE_LOCS__PIXEL_*
    E32(0xFFFFFFFF); // PA_SC_AA_MASK_*
    E32(0xFFFFFFFF); // PA_SC_AA_MASK_*

    // CL
    PACK0(PA_CL_CLIP_CNTL, 8);
    E32(CLIP_DISABLE_bit); // PA_CL_CLIP_CNTL
    E32(FACE_bit); // PA_SU_SC_MODE_CNTL
    E32(VTX_XY_FMT_bit); // PA_CL_VTE_CNTL
    E32(0); // PA_CL_VS_OUT_CNTL
    E32(0); // PA_CL_NANINF_CNTL
    E32(0); // PA_SU_LINE_STIPPLE_CNTL
    E32(0); // PA_SU_LINE_STIPPLE_SCALE
    E32(0); // PA_SU_PRIM_FILTER_CNTL

    // SU
    PACK0(PA_SU_POLY_OFFSET_DB_FMT_CNTL, 6);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);

    /* src = semantic id 0; mask = semantic id 1 */
    EREG(SPI_VS_OUT_ID_0, ((0 << SEMANTIC_0_shift) |
			   (1 << SEMANTIC_1_shift)));
    PACK0(SPI_PS_INPUT_CNTL_0 + (0 << 2), 2);
    /* SPI_PS_INPUT_CNTL_0 maps to GPR[0] - load with semantic id 0 */
    E32(((0    << SEMANTIC_shift)	|
	 (0x01 << DEFAULT_VAL_shift)));
    /* SPI_PS_INPUT_CNTL_1 maps to GPR[1] - load with semantic id 1 */
    E32(((1    << SEMANTIC_shift)	|
	 (0x01 << DEFAULT_VAL_shift)));

    PACK0(SPI_INPUT_Z, 13);
    E32(0); // SPI_INPUT_Z
    E32(0); // SPI_FOG_CNTL
    E32(LINEAR_CENTROID_ENA__X_ON_AT_CENTROID << LINEAR_CENTROID_ENA_shift); // SPI_BARYC_CNTL
    E32(0); // SPI_PS_IN_CONTROL_2
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0); // SPI_GPR_MGMT
    E32(0); // SPI_LDS_MGMT
    E32(0); // SPI_STACK_MGMT
    E32(0); // SPI_WAVE_MGMT_1
    E32(0); // SPI_WAVE_MGMT_2
    END_BATCH();

    // clear FS
    fs_conf.bo = accel_state->shaders_bo;
    evergreen_fs_setup(pScrn, &fs_conf, RADEON_GEM_DOMAIN_VRAM);

    // VGT
    BEGIN_BATCH(46);

    PACK0(VGT_MAX_VTX_INDX, 4);
    E32(0xffffff);
    E32(0);
    E32(0);
    E32(0);

    PACK0(VGT_INSTANCE_STEP_RATE_0, 2);
    E32(0);
    E32(0);

    PACK0(VGT_REUSE_OFF, 2);
    E32(0);
    E32(0);

    PACK0(PA_SU_POINT_SIZE, 17);
    E32(0); // PA_SU_POINT_SIZE
    E32(0); // PA_SU_POINT_MINMAX
    E32((8 << PA_SU_LINE_CNTL__WIDTH_shift)); /* Line width 1 pixel */ // PA_SU_LINE_CNTL
    E32(0); // PA_SC_LINE_STIPPLE
    E32(0); // VGT_OUTPUT_PATH_CNTL
    E32(0); // VGT_HOS_CNTL
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0); // VGT_GS_MODE

    EREG(VGT_PRIMITIVEID_EN,                  0);
    EREG(VGT_MULTI_PRIM_IB_RESET_EN,          0);
    EREG(VGT_SHADER_STAGES_EN,          0);

    PACK0(VGT_STRMOUT_CONFIG, 2);
    E32(0);
    E32(0);
    END_BATCH();
}

