/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *   Alan Hourihane <alanh@fairlite.demon.co.uk>
 *
 * Credits:
 *
 *   Thanks to Ani Joshi <ajoshi@shell.unixbox.com> for providing source
 *   code to his Radeon driver.  Portions of this file are based on the
 *   initialization code for that driver.
 *
 * References:
 *
 * !!!! FIXME !!!!
 *   RAGE 128 VR/ RAGE 128 GL Register Reference Manual (Technical
 *   Reference Manual P/N RRG-G04100-C Rev. 0.04), ATI Technologies: April
 *   1999.
 *
 *   RAGE 128 Software Development Manual (Technical Reference Manual P/N
 *   SDK-G04000 Rev. 0.01), ATI Technologies: June 1999.
 *
 * Notes on unimplemented XAA optimizations:
 *
 *   SetClipping:   This has been removed as XAA expects 16bit registers
 *                  for full clipping.
 *   TwoPointLine:  The Radeon supports this. Not Bresenham.
 *   DashedLine with non-power-of-two pattern length: Apparently, there is
 *                  no way to set the length of the pattern -- it is always
 *                  assumed to be 8 or 32 (or 1024?).
 *   ScreenToScreenColorExpandFill: See p. 4-17 of the Technical Reference
 *                  Manual where it states that monochrome expansion of frame
 *                  buffer data is not supported.
 *   CPUToScreenColorExpandFill, direct: The implementation here uses a hybrid
 *                  direct/indirect method.  If we had more data registers,
 *                  then we could do better.  If XAA supported a trigger write
 *                  address, the code would be simpler.
 *   Color8x8PatternFill: Apparently, an 8x8 color brush cannot take an 8x8
 *                  pattern from frame buffer memory.
 *   ImageWrites:   Same as CPUToScreenColorExpandFill
 *
 */

#include <errno.h>
#include <string.h>
#include <assert.h>
				/* Driver data structures */
#include "radeon.h"
#include "radeon_glamor.h"
#include "radeon_reg.h"
#include "r600_reg.h"
#include "radeon_probe.h"
#include "radeon_version.h"

#include "ati_pciids_gen.h"

				/* Line support */
#include "miline.h"

				/* X and server generic header files */
#include "xf86.h"

static int RADEONDRMGetNumPipes(ScrnInfoPtr pScrn, int *num_pipes)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct drm_radeon_info np2;
    np2.value = (unsigned long)num_pipes;
    np2.request = RADEON_INFO_NUM_GB_PIPES;
    return drmCommandWriteRead(info->dri2.drm_fd, DRM_RADEON_INFO, &np2, sizeof(np2));
}

/* Initialize the acceleration hardware */
void RADEONEngineInit(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    info->accel_state->num_gb_pipes = 0;

    if (info->directRenderingEnabled && (IS_R300_3D || IS_R500_3D)) {
	int num_pipes;

	if(RADEONDRMGetNumPipes(pScrn, &num_pipes) < 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Failed to determine num pipes from DRM, falling back to "
		       "manual look-up!\n");
	    info->accel_state->num_gb_pipes = 0;
	} else {
	    info->accel_state->num_gb_pipes = num_pipes;
	}
    }
}

int radeon_cs_space_remaining(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    return (info->cs->ndw - info->cs->cdw);
}

void RADEONCopySwap(uint8_t *dst, uint8_t *src, unsigned int size, int swap)
{
    switch(swap) {
    case RADEON_HOST_DATA_SWAP_HDW:
        {
	    unsigned int *d = (unsigned int *)dst;
	    unsigned int *s = (unsigned int *)src;
	    unsigned int nwords = size >> 2;

	    for (; nwords > 0; --nwords, ++d, ++s)
		*d = ((*s & 0xffff) << 16) | ((*s >> 16) & 0xffff);
	    return;
        }
    case RADEON_HOST_DATA_SWAP_32BIT:
        {
	    unsigned int *d = (unsigned int *)dst;
	    unsigned int *s = (unsigned int *)src;
	    unsigned int nwords = size >> 2;

	    for (; nwords > 0; --nwords, ++d, ++s)
#ifdef __powerpc__
		asm volatile("stwbrx %0,0,%1" : : "r" (*s), "r" (d));
#else
		*d = ((*s >> 24) & 0xff) | ((*s >> 8) & 0xff00)
			| ((*s & 0xff00) << 8) | ((*s & 0xff) << 24);
#endif
	    return;
        }
    case RADEON_HOST_DATA_SWAP_16BIT:
        {
	    unsigned short *d = (unsigned short *)dst;
	    unsigned short *s = (unsigned short *)src;
	    unsigned int nwords = size >> 1;

	    for (; nwords > 0; --nwords, ++d, ++s)
#ifdef __powerpc__
		asm volatile("sthbrx %0,0,%1" : : "r" (*s), "r" (d));
#else
	        *d = (*s >> 8) | (*s << 8);
#endif
	    return;
	}
    }
    if (src != dst)
	memcpy(dst, src, size);
}



Bool RADEONAccelInit(ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr  info  = RADEONPTR(pScrn);

    if (info->directRenderingEnabled) {
	if (info->use_glamor) {
	    if (!radeon_glamor_init(pScreen)) {
		info->use_glamor = FALSE;
		return FALSE;
	    }
	} else if (info->ChipFamily >= CHIP_FAMILY_CEDAR) {
	    if (!EVERGREENDrawInit(pScreen))
		return FALSE;
	} else
	    if (info->ChipFamily >= CHIP_FAMILY_R600) {
		if (!R600DrawInit(pScreen))
		    return FALSE;
	    } else {
		if (!RADEONDrawInit(pScreen))
		    return FALSE;
	    }
    }
    return TRUE;
}

static void RADEONInit3DEngineInternal(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    uint32_t gb_tile_config, vap_cntl;

    info->accel_state->texW[0] = info->accel_state->texH[0] =
	info->accel_state->texW[1] = info->accel_state->texH[1] = 1;

    if (IS_R300_3D || IS_R500_3D) {

	gb_tile_config = (R300_ENABLE_TILING | R300_TILE_SIZE_16);

	switch(info->accel_state->num_gb_pipes) {
	case 2: gb_tile_config |= R300_PIPE_COUNT_R300; break;
	case 3: gb_tile_config |= R300_PIPE_COUNT_R420_3P; break;
	case 4: gb_tile_config |= R300_PIPE_COUNT_R420; break;
	default:
	case 1: gb_tile_config |= R300_PIPE_COUNT_RV350; break;
	}

	BEGIN_RING(2*3);
	OUT_RING_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D | R300_DC_FREE_3D);
	OUT_RING_REG(R300_RB3D_ZCACHE_CTLSTAT, R300_ZC_FLUSH | R300_ZC_FREE);
	OUT_RING_REG(RADEON_WAIT_UNTIL, RADEON_WAIT_2D_IDLECLEAN | RADEON_WAIT_3D_IDLECLEAN);
	ADVANCE_RING();

	BEGIN_RING(2*3);
	OUT_RING_REG(R300_GB_AA_CONFIG, 0);
	OUT_RING_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D | R300_DC_FREE_3D);
	OUT_RING_REG(R300_RB3D_ZCACHE_CTLSTAT, R300_ZC_FLUSH | R300_ZC_FREE);
	ADVANCE_RING();

	BEGIN_RING(2*4);
	OUT_RING_REG(R300_GA_POLY_MODE, R300_FRONT_PTYPE_TRIANGE | R300_BACK_PTYPE_TRIANGE);
	OUT_RING_REG(R300_GA_ROUND_MODE, (R300_GEOMETRY_ROUND_NEAREST |
					   R300_COLOR_ROUND_NEAREST));
	OUT_RING_REG(R300_GA_COLOR_CONTROL, (R300_RGB0_SHADING_GOURAUD |
					      R300_ALPHA0_SHADING_GOURAUD |
					      R300_RGB1_SHADING_GOURAUD |
					      R300_ALPHA1_SHADING_GOURAUD |
					      R300_RGB2_SHADING_GOURAUD |
					      R300_ALPHA2_SHADING_GOURAUD |
					      R300_RGB3_SHADING_GOURAUD |
					      R300_ALPHA3_SHADING_GOURAUD));
	OUT_RING_REG(R300_GA_OFFSET, 0);
	ADVANCE_RING();

	BEGIN_RING(2*5);
	OUT_RING_REG(R300_SU_TEX_WRAP, 0);
	OUT_RING_REG(R300_SU_POLY_OFFSET_ENABLE, 0);
	OUT_RING_REG(R300_SU_CULL_MODE, R300_FACE_NEG);
	OUT_RING_REG(R300_SU_DEPTH_SCALE, 0x4b7fffff);
	OUT_RING_REG(R300_SU_DEPTH_OFFSET, 0);
	ADVANCE_RING();

	/* setup the VAP */
	if (info->accel_state->has_tcl)
	    vap_cntl = ((5 << R300_PVS_NUM_SLOTS_SHIFT) |
			(5 << R300_PVS_NUM_CNTLRS_SHIFT) |
			(9 << R300_VF_MAX_VTX_NUM_SHIFT));
	else
	    vap_cntl = ((10 << R300_PVS_NUM_SLOTS_SHIFT) |
			(5 << R300_PVS_NUM_CNTLRS_SHIFT) |
			(5 << R300_VF_MAX_VTX_NUM_SHIFT));

	if ((info->ChipFamily == CHIP_FAMILY_R300) ||
	    (info->ChipFamily == CHIP_FAMILY_R350))
	    vap_cntl |= (4 << R300_PVS_NUM_FPUS_SHIFT);
	else if (info->ChipFamily == CHIP_FAMILY_RV530)
	    vap_cntl |= (5 << R300_PVS_NUM_FPUS_SHIFT);
	else if ((info->ChipFamily == CHIP_FAMILY_RV410) ||
		 (info->ChipFamily == CHIP_FAMILY_R420))
	    vap_cntl |= (6 << R300_PVS_NUM_FPUS_SHIFT);
	else if ((info->ChipFamily == CHIP_FAMILY_R520) ||
		 (info->ChipFamily == CHIP_FAMILY_R580) ||
		 (info->ChipFamily == CHIP_FAMILY_RV560) ||
		 (info->ChipFamily == CHIP_FAMILY_RV570))
	    vap_cntl |= (8 << R300_PVS_NUM_FPUS_SHIFT);
	else
	    vap_cntl |= (2 << R300_PVS_NUM_FPUS_SHIFT);

	if (info->accel_state->has_tcl)
	    BEGIN_RING(2*15);
	else
	    BEGIN_RING(2*9);
	OUT_RING_REG(R300_VAP_VTX_STATE_CNTL, 0);
	OUT_RING_REG(R300_VAP_PVS_STATE_FLUSH_REG, 0);

	if (info->accel_state->has_tcl)
	    OUT_RING_REG(R300_VAP_CNTL_STATUS, 0);
	else
	    OUT_RING_REG(R300_VAP_CNTL_STATUS, R300_PVS_BYPASS);
	OUT_RING_REG(R300_VAP_CNTL, vap_cntl);
	OUT_RING_REG(R300_VAP_PVS_STATE_FLUSH_REG, 0);
	OUT_RING_REG(R300_VAP_VTE_CNTL, R300_VTX_XY_FMT | R300_VTX_Z_FMT);
	OUT_RING_REG(R300_VAP_PSC_SGN_NORM_CNTL, 0);

	OUT_RING_REG(R300_VAP_PROG_STREAM_CNTL_EXT_0,
		      ((R300_SWIZZLE_SELECT_X << R300_SWIZZLE_SELECT_X_0_SHIFT) |
		       (R300_SWIZZLE_SELECT_Y << R300_SWIZZLE_SELECT_Y_0_SHIFT) |
		       (R300_SWIZZLE_SELECT_Z << R300_SWIZZLE_SELECT_Z_0_SHIFT) |
		       (R300_SWIZZLE_SELECT_W << R300_SWIZZLE_SELECT_W_0_SHIFT) |
		       ((R300_WRITE_ENA_X | R300_WRITE_ENA_Y | R300_WRITE_ENA_Z | R300_WRITE_ENA_W)
			<< R300_WRITE_ENA_0_SHIFT) |
		       (R300_SWIZZLE_SELECT_X << R300_SWIZZLE_SELECT_X_1_SHIFT) |
		       (R300_SWIZZLE_SELECT_Y << R300_SWIZZLE_SELECT_Y_1_SHIFT) |
		       (R300_SWIZZLE_SELECT_Z << R300_SWIZZLE_SELECT_Z_1_SHIFT) |
		       (R300_SWIZZLE_SELECT_W << R300_SWIZZLE_SELECT_W_1_SHIFT) |
		       ((R300_WRITE_ENA_X | R300_WRITE_ENA_Y | R300_WRITE_ENA_Z | R300_WRITE_ENA_W)
			<< R300_WRITE_ENA_1_SHIFT)));
	OUT_RING_REG(R300_VAP_PROG_STREAM_CNTL_EXT_1,
		      ((R300_SWIZZLE_SELECT_X << R300_SWIZZLE_SELECT_X_2_SHIFT) |
		       (R300_SWIZZLE_SELECT_Y << R300_SWIZZLE_SELECT_Y_2_SHIFT) |
		       (R300_SWIZZLE_SELECT_Z << R300_SWIZZLE_SELECT_Z_2_SHIFT) |
		       (R300_SWIZZLE_SELECT_W << R300_SWIZZLE_SELECT_W_2_SHIFT) |
		       ((R300_WRITE_ENA_X | R300_WRITE_ENA_Y | R300_WRITE_ENA_Z | R300_WRITE_ENA_W)
			<< R300_WRITE_ENA_2_SHIFT)));

	if (info->accel_state->has_tcl) {
	    OUT_RING_REG(R300_VAP_PVS_FLOW_CNTL_OPC, 0);
	    OUT_RING_REG(R300_VAP_GB_VERT_CLIP_ADJ, 0x3f800000);
	    OUT_RING_REG(R300_VAP_GB_VERT_DISC_ADJ, 0x3f800000);
	    OUT_RING_REG(R300_VAP_GB_HORZ_CLIP_ADJ, 0x3f800000);
	    OUT_RING_REG(R300_VAP_GB_HORZ_DISC_ADJ, 0x3f800000);
	    OUT_RING_REG(R300_VAP_CLIP_CNTL, R300_CLIP_DISABLE);
	}
	ADVANCE_RING();

	/* pre-load the vertex shaders */
	if (info->accel_state->has_tcl) {
	    BEGIN_RING(2*37);
	    /* exa composite shader program */
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_INDX_REG, R300_PVS_VECTOR_INST_INDEX(0));
	    /* PVS inst 0 - dst X,Y */
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_ADD) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(0) |
			   R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
			   R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_1)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    /* PVS inst 1 - src X */
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_DOT_PRODUCT) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_TEMPORARY) |
			   R300_PVS_DST_OFFSET(0) |
			   R300_PVS_DST_WE_X));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_1) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_CONSTANT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    /* PVS inst 2 - src Y */
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_DOT_PRODUCT) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_TEMPORARY) |
			   R300_PVS_DST_OFFSET(0) |
			   R300_PVS_DST_WE_Y));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_1) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_CONSTANT) |
			   R300_PVS_SRC_OFFSET(1) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    /* PVS inst 3 - src X / w */
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_MULTIPLY) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(1) |
			   R300_PVS_DST_WE_X));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_TEMPORARY) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_CONSTANT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_W) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    /* PVS inst 4 - src y / h */
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_MULTIPLY) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(1) |
			   R300_PVS_DST_WE_Y));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_TEMPORARY) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_CONSTANT) |
			   R300_PVS_SRC_OFFSET(1) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_W) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    /* PVS inst 5 - mask X */
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_DOT_PRODUCT) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_TEMPORARY) |
			   R300_PVS_DST_OFFSET(0) |
			   R300_PVS_DST_WE_Z));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(7) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_1) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_CONSTANT) |
			   R300_PVS_SRC_OFFSET(2) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(7) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    /* PVS inst 6 - mask Y */
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_DOT_PRODUCT) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_TEMPORARY) |
			   R300_PVS_DST_OFFSET(0) |
			   R300_PVS_DST_WE_W));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(7) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_1) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_CONSTANT) |
			   R300_PVS_SRC_OFFSET(3) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(7) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    /* PVS inst 7 - mask X / w */
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_MULTIPLY) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(2) |
			   R300_PVS_DST_WE_X));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_TEMPORARY) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_Z) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_CONSTANT) |
			   R300_PVS_SRC_OFFSET(2) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_W) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    /* PVS inst 8 - mask y / h */
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_MULTIPLY) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(2) |
			   R300_PVS_DST_WE_Y));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_TEMPORARY) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_W) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_CONSTANT) |
			   R300_PVS_SRC_OFFSET(3) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_W) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    ADVANCE_RING();

	    /* Xv shader program */
	    BEGIN_RING(2*9);
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_INDX_REG, R300_PVS_VECTOR_INST_INDEX(9));

	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_ADD) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(0) |
			   R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
			   R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_1)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(0) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_DST_OPCODE(R300_VE_ADD) |
			   R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(1) |
			   R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_1)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
			   R300_PVS_SRC_OFFSET(6) |
			   R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    ADVANCE_RING();

            /* Xv bicubic shader program */
	    BEGIN_RING(2*13);
            OUT_RING_REG(R300_VAP_PVS_VECTOR_INDX_REG, R300_PVS_VECTOR_INST_INDEX(11));
            /* PVS inst 0 */
            OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
                          (R300_PVS_DST_OPCODE(R300_VE_ADD) |
                           R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
			   R300_PVS_DST_OFFSET(0) |
                           R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
                           R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
                           R300_PVS_SRC_OFFSET(0) |
                           R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
                           R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
                           R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_1)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
                          (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
                           R300_PVS_SRC_OFFSET(0) |
                           R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
                           R300_PVS_SRC_OFFSET(0) |
                           R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

            /* PVS inst 1 */
            OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
                          (R300_PVS_DST_OPCODE(R300_VE_ADD) |
                           R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
                           R300_PVS_DST_OFFSET(1) |
                           R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
                           R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
            OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
                          (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
                           R300_PVS_SRC_OFFSET(6) |
                           R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
                           R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
                           R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_1)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
                          (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
                           R300_PVS_SRC_OFFSET(6) |
                           R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
	    OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
			  (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
                           R300_PVS_SRC_OFFSET(6) |
                           R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
			   R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));

            /* PVS inst 2 */
            OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
                          (R300_PVS_DST_OPCODE(R300_VE_ADD) |
                           R300_PVS_DST_REG_TYPE(R300_PVS_DST_REG_OUT) |
                           R300_PVS_DST_OFFSET(2) |
                           R300_PVS_DST_WE_X | R300_PVS_DST_WE_Y |
                           R300_PVS_DST_WE_Z | R300_PVS_DST_WE_W));
            OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
                          (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
                           R300_PVS_SRC_OFFSET(7) |
                           R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_X) |
                           R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_Y) |
                           R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_1)));
            OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
                          (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
                           R300_PVS_SRC_OFFSET(7) |
                           R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
            OUT_RING_REG(R300_VAP_PVS_VECTOR_DATA_REG,
                          (R300_PVS_SRC_REG_TYPE(R300_PVS_SRC_REG_INPUT) |
                           R300_PVS_SRC_OFFSET(7) |
                           R300_PVS_SRC_SWIZZLE_X(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_Y(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_Z(R300_PVS_SRC_SELECT_FORCE_0) |
                           R300_PVS_SRC_SWIZZLE_W(R300_PVS_SRC_SELECT_FORCE_0)));
            ADVANCE_RING();
	}

	/* pre-load the RS instructions */
	BEGIN_RING(2*4);
	if (IS_R300_3D) {
	    /* rasterizer source table
	     * R300_RS_TEX_PTR is the offset into the input RS stream
	     * 0,1 are tex0
	     * 2,3 are tex1
	     */
	    OUT_RING_REG(R300_RS_IP_0,
			  (R300_RS_TEX_PTR(0) |
			   R300_RS_SEL_S(R300_RS_SEL_C0) |
			   R300_RS_SEL_T(R300_RS_SEL_C1) |
			   R300_RS_SEL_R(R300_RS_SEL_K0) |
			   R300_RS_SEL_Q(R300_RS_SEL_K1)));
	    OUT_RING_REG(R300_RS_IP_1,
			  (R300_RS_TEX_PTR(2) |
			   R300_RS_SEL_S(R300_RS_SEL_C0) |
			   R300_RS_SEL_T(R300_RS_SEL_C1) |
			   R300_RS_SEL_R(R300_RS_SEL_K0) |
			   R300_RS_SEL_Q(R300_RS_SEL_K1)));
	    /* src tex */
	    /* R300_INST_TEX_ID - select the RS source table entry
	     * R300_INST_TEX_ADDR - the FS temp register for the texture data
	     */
	    OUT_RING_REG(R300_RS_INST_0, (R300_INST_TEX_ID(0) |
					   R300_RS_INST_TEX_CN_WRITE |
					   R300_INST_TEX_ADDR(0)));
	    /* mask tex */
	    OUT_RING_REG(R300_RS_INST_1, (R300_INST_TEX_ID(1) |
					   R300_RS_INST_TEX_CN_WRITE |
					   R300_INST_TEX_ADDR(1)));

	} else {
	    /* rasterizer source table
	     * R300_RS_TEX_PTR is the offset into the input RS stream
	     * 0,1 are tex0
	     * 2,3 are tex1
	     */
	    OUT_RING_REG(R500_RS_IP_0, ((0 << R500_RS_IP_TEX_PTR_S_SHIFT) |
					 (1 << R500_RS_IP_TEX_PTR_T_SHIFT) |
					 (R500_RS_IP_PTR_K0 << R500_RS_IP_TEX_PTR_R_SHIFT) |
					 (R500_RS_IP_PTR_K1 << R500_RS_IP_TEX_PTR_Q_SHIFT)));

	    OUT_RING_REG(R500_RS_IP_1, ((2 << R500_RS_IP_TEX_PTR_S_SHIFT) |
					 (3 << R500_RS_IP_TEX_PTR_T_SHIFT) |
					 (R500_RS_IP_PTR_K0 << R500_RS_IP_TEX_PTR_R_SHIFT) |
					 (R500_RS_IP_PTR_K1 << R500_RS_IP_TEX_PTR_Q_SHIFT)));
	    /* src tex */
	    /* R500_RS_INST_TEX_ID_SHIFT - select the RS source table entry
	     * R500_RS_INST_TEX_ADDR_SHIFT - the FS temp register for the texture data
	     */
	    OUT_RING_REG(R500_RS_INST_0, ((0 << R500_RS_INST_TEX_ID_SHIFT) |
					   R500_RS_INST_TEX_CN_WRITE |
					   (0 << R500_RS_INST_TEX_ADDR_SHIFT)));
	    /* mask tex */
	    OUT_RING_REG(R500_RS_INST_1, ((1 << R500_RS_INST_TEX_ID_SHIFT) |
					   R500_RS_INST_TEX_CN_WRITE |
					   (1 << R500_RS_INST_TEX_ADDR_SHIFT)));
	}
	ADVANCE_RING();

	if (IS_R300_3D)
	    BEGIN_RING(2*4);
	else {
	    BEGIN_RING(2*6);
	    OUT_RING_REG(R300_US_CONFIG, R500_ZERO_TIMES_ANYTHING_EQUALS_ZERO);
	    OUT_RING_REG(R500_US_FC_CTRL, 0);
	}
	OUT_RING_REG(R300_US_W_FMT, 0);
	OUT_RING_REG(R300_US_OUT_FMT_1, (R300_OUT_FMT_UNUSED |
					  R300_OUT_FMT_C0_SEL_BLUE |
					  R300_OUT_FMT_C1_SEL_GREEN |
					  R300_OUT_FMT_C2_SEL_RED |
					  R300_OUT_FMT_C3_SEL_ALPHA));
	OUT_RING_REG(R300_US_OUT_FMT_2, (R300_OUT_FMT_UNUSED |
					  R300_OUT_FMT_C0_SEL_BLUE |
					  R300_OUT_FMT_C1_SEL_GREEN |
					  R300_OUT_FMT_C2_SEL_RED |
					  R300_OUT_FMT_C3_SEL_ALPHA));
	OUT_RING_REG(R300_US_OUT_FMT_3, (R300_OUT_FMT_UNUSED |
					  R300_OUT_FMT_C0_SEL_BLUE |
					  R300_OUT_FMT_C1_SEL_GREEN |
					  R300_OUT_FMT_C2_SEL_RED |
					  R300_OUT_FMT_C3_SEL_ALPHA));
	ADVANCE_RING();


	BEGIN_RING(2*3);
	OUT_RING_REG(R300_FG_DEPTH_SRC, 0);
	OUT_RING_REG(R300_FG_FOG_BLEND, 0);
	OUT_RING_REG(R300_FG_ALPHA_FUNC, 0);
	ADVANCE_RING();

	BEGIN_RING(2*13);
	OUT_RING_REG(R300_RB3D_ABLENDCNTL, 0);
	OUT_RING_REG(R300_RB3D_ZSTENCILCNTL, 0);
	OUT_RING_REG(R300_RB3D_ZCACHE_CTLSTAT, R300_ZC_FLUSH | R300_ZC_FREE);
	OUT_RING_REG(R300_RB3D_BW_CNTL, 0);
	OUT_RING_REG(R300_RB3D_ZCNTL, 0);
	OUT_RING_REG(R300_RB3D_ZTOP, 0);
	OUT_RING_REG(R300_RB3D_ROPCNTL, 0);

	OUT_RING_REG(R300_RB3D_AARESOLVE_CTL, 0);
	OUT_RING_REG(R300_RB3D_COLOR_CHANNEL_MASK, (R300_BLUE_MASK_EN |
						     R300_GREEN_MASK_EN |
						     R300_RED_MASK_EN |
						     R300_ALPHA_MASK_EN));
	OUT_RING_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D | R300_DC_FREE_3D);
	OUT_RING_REG(R300_RB3D_CCTL, 0);
	OUT_RING_REG(R300_RB3D_DITHER_CTL, 0);
	OUT_RING_REG(R300_RB3D_DSTCACHE_CTLSTAT, R300_DC_FLUSH_3D | R300_DC_FREE_3D);
	ADVANCE_RING();

	BEGIN_RING(2*5);
	OUT_RING_REG(R300_SC_EDGERULE, 0xA5294A5);
	if (IS_R300_3D) {
	    /* clip has offset 1440 */
	    OUT_RING_REG(R300_SC_CLIP_0_A, ((1440 << R300_CLIP_X_SHIFT) |
					     (1440 << R300_CLIP_Y_SHIFT)));
	    OUT_RING_REG(R300_SC_CLIP_0_B, ((4080 << R300_CLIP_X_SHIFT) |
					     (4080 << R300_CLIP_Y_SHIFT)));
	} else {
	    OUT_RING_REG(R300_SC_CLIP_0_A, ((0 << R300_CLIP_X_SHIFT) |
					     (0 << R300_CLIP_Y_SHIFT)));
	    OUT_RING_REG(R300_SC_CLIP_0_B, ((4080 << R300_CLIP_X_SHIFT) |
					     (4080 << R300_CLIP_Y_SHIFT)));
	}
	OUT_RING_REG(R300_SC_CLIP_RULE, 0xAAAA);
	OUT_RING_REG(R300_SC_SCREENDOOR, 0xffffff);
	ADVANCE_RING();
    } else if (IS_R200_3D) {

	BEGIN_RING(2*6);
	if (info->ChipFamily == CHIP_FAMILY_RS300) {
	    OUT_RING_REG(R200_SE_VAP_CNTL_STATUS, RADEON_TCL_BYPASS);
	} else {
	    OUT_RING_REG(R200_SE_VAP_CNTL_STATUS, 0);
	}
	OUT_RING_REG(R200_PP_CNTL_X, 0);
	OUT_RING_REG(R200_PP_TXMULTI_CTL_0, 0);
	OUT_RING_REG(R200_SE_VTX_STATE_CNTL, 0);
	OUT_RING_REG(R200_SE_VTE_CNTL, 0);
	OUT_RING_REG(R200_SE_VAP_CNTL, R200_VAP_FORCE_W_TO_ONE |
	    R200_VAP_VF_MAX_VTX_NUM);
	ADVANCE_RING();

	BEGIN_RING(2*4);
	OUT_RING_REG(R200_RE_AUX_SCISSOR_CNTL, 0);
	OUT_RING_REG(R200_RE_CNTL, 0);
	OUT_RING_REG(RADEON_RB3D_PLANEMASK, 0xffffffff);
	OUT_RING_REG(RADEON_SE_CNTL, (RADEON_DIFFUSE_SHADE_GOURAUD |
				       RADEON_BFACE_SOLID |
				       RADEON_FFACE_SOLID |
				       RADEON_VTX_PIX_CENTER_OGL |
				       RADEON_ROUND_MODE_ROUND |
				       RADEON_ROUND_PREC_4TH_PIX));
	ADVANCE_RING();
    } else {
	BEGIN_RING(2*2);
	if ((info->ChipFamily == CHIP_FAMILY_RADEON) ||
	    (info->ChipFamily == CHIP_FAMILY_RV200))
	    OUT_RING_REG(RADEON_SE_CNTL_STATUS, 0);
	else
	    OUT_RING_REG(RADEON_SE_CNTL_STATUS, RADEON_TCL_BYPASS);
	OUT_RING_REG(RADEON_SE_COORD_FMT,
		      RADEON_VTX_XY_PRE_MULT_1_OVER_W0 |
		      RADEON_VTX_ST0_PRE_MULT_1_OVER_W0 |
		      RADEON_VTX_ST1_PRE_MULT_1_OVER_W0 |
		      RADEON_TEX1_W_ROUTING_USE_W0);
	ADVANCE_RING();

	BEGIN_RING(2*2);
	OUT_RING_REG(RADEON_RB3D_PLANEMASK, 0xffffffff);
	OUT_RING_REG(RADEON_SE_CNTL, (RADEON_DIFFUSE_SHADE_GOURAUD |
				       RADEON_BFACE_SOLID |
				       RADEON_FFACE_SOLID |
				       RADEON_VTX_PIX_CENTER_OGL |
				       RADEON_ROUND_MODE_ROUND |
				       RADEON_ROUND_PREC_4TH_PIX));
	ADVANCE_RING();
    }

}

/* inserts a wait for vline in the command stream */
void RADEONWaitForVLine(ScrnInfoPtr pScrn, PixmapPtr pPix,
			xf86CrtcPtr crtc, int start, int stop)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    drmmode_crtc_private_ptr drmmode_crtc;

    if (!crtc)
	return;

    if (!crtc->enabled)
	return;

    if (pPix != pScrn->pScreen->GetScreenPixmap(pScrn->pScreen))
        return;

    start = max(start, crtc->y);
    stop = min(stop, crtc->y + crtc->mode.VDisplay);

    if (start >= stop)
	return;

    if (!IS_AVIVO_VARIANT) {
	/* on pre-r5xx vline starts at CRTC scanout */
	start -= crtc->y;
	stop -= crtc->y;
    }

    drmmode_crtc = crtc->driver_private;

    BEGIN_RING(2*3);
    if (IS_AVIVO_VARIANT) {
	OUT_RING_REG(AVIVO_D1MODE_VLINE_START_END, /* this is just a marker */
		      ((start << AVIVO_D1MODE_VLINE_START_SHIFT) |
		       (stop << AVIVO_D1MODE_VLINE_END_SHIFT) |
		       AVIVO_D1MODE_VLINE_INV));
    } else {
	OUT_RING_REG(RADEON_CRTC_GUI_TRIG_VLINE, /* another placeholder */
		      ((start << RADEON_CRTC_GUI_TRIG_VLINE_START_SHIFT) |
		       (stop << RADEON_CRTC_GUI_TRIG_VLINE_END_SHIFT) |
		       RADEON_CRTC_GUI_TRIG_VLINE_INV |
		       RADEON_CRTC_GUI_TRIG_VLINE_STALL));
    }
    OUT_RING_REG(RADEON_WAIT_UNTIL, (RADEON_WAIT_CRTC_VLINE |
				      RADEON_ENG_DISPLAY_SELECT_CRTC0));
    
    OUT_RING(CP_PACKET3(RADEON_CP_PACKET3_NOP, 0));
    OUT_RING(drmmode_crtc->mode_crtc->crtc_id);
    ADVANCE_RING();
}


void RADEONInit3DEngine(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);

    if (info->directRenderingEnabled) {
	RADEONInit3DEngineInternal(pScrn);
    }
    info->accel_state->XInited3D = TRUE;
}
