/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/ati/radeon_accelfuncs.c,v 1.7tsi Exp $ */
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

static void FUNC_NAME(RADEONInit3DEngine)(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    ACCEL_PREAMBLE();

    if (info->ChipFamily >= CHIP_FAMILY_R300) {
	/* Unimplemented */
    } else if ((info->ChipFamily == CHIP_FAMILY_RV250) || 
	       (info->ChipFamily == CHIP_FAMILY_RV280) || 
	       (info->ChipFamily == CHIP_FAMILY_RS300) || 
	       (info->ChipFamily == CHIP_FAMILY_R200)) {

	BEGIN_ACCEL(7);
        if (info->ChipFamily == CHIP_FAMILY_RS300) {
            OUT_ACCEL_REG(R200_SE_VAP_CNTL_STATUS, RADEON_TCL_BYPASS);
        } else {
            OUT_ACCEL_REG(R200_SE_VAP_CNTL_STATUS, 0);
        }
	OUT_ACCEL_REG(R200_PP_CNTL_X, 0);
	OUT_ACCEL_REG(R200_PP_TXMULTI_CTL_0, 0);
	OUT_ACCEL_REG(R200_SE_VTX_STATE_CNTL, 0);
	OUT_ACCEL_REG(R200_RE_CNTL, 0x0);
	/* XXX: correct?  Want it to be like RADEON_VTX_ST?_NONPARAMETRIC */
	OUT_ACCEL_REG(R200_SE_VTE_CNTL, R200_VTX_ST_DENORMALIZED);
	OUT_ACCEL_REG(R200_SE_VAP_CNTL, R200_VAP_FORCE_W_TO_ONE |
	    R200_VAP_VF_MAX_VTX_NUM);
	FINISH_ACCEL();
    } else {
	BEGIN_ACCEL(2);
        if ((info->ChipFamily == CHIP_FAMILY_RADEON) ||
            (info->ChipFamily == CHIP_FAMILY_RV200))
            OUT_ACCEL_REG(RADEON_SE_CNTL_STATUS, 0);
        else
            OUT_ACCEL_REG(RADEON_SE_CNTL_STATUS, RADEON_TCL_BYPASS);
	OUT_ACCEL_REG(RADEON_SE_COORD_FMT,
	    RADEON_VTX_XY_PRE_MULT_1_OVER_W0 |
	    RADEON_VTX_ST0_NONPARAMETRIC |
	    RADEON_VTX_ST1_NONPARAMETRIC |
	    RADEON_TEX1_W_ROUTING_USE_W0);
	FINISH_ACCEL();
    }

    BEGIN_ACCEL(5);
    OUT_ACCEL_REG(RADEON_RE_TOP_LEFT, 0);
    OUT_ACCEL_REG(RADEON_RE_WIDTH_HEIGHT, 0x07ff07ff);
    OUT_ACCEL_REG(RADEON_AUX_SC_CNTL, 0);
    OUT_ACCEL_REG(RADEON_RB3D_PLANEMASK, 0xffffffff);
    OUT_ACCEL_REG(RADEON_SE_CNTL, RADEON_DIFFUSE_SHADE_GOURAUD |
				  RADEON_BFACE_SOLID | 
				  RADEON_FFACE_SOLID |
				  RADEON_VTX_PIX_CENTER_OGL |
				  RADEON_ROUND_MODE_ROUND |
				  RADEON_ROUND_PREC_4TH_PIX);
    FINISH_ACCEL();
}


/* MMIO:
 *
 * Wait for the graphics engine to be completely idle: the FIFO has
 * drained, the Pixel Cache is flushed, and the engine is idle.  This is
 * a standard "sync" function that will make the hardware "quiescent".
 *
 * CP:
 *
 * Wait until the CP is completely idle: the FIFO has drained and the CP
 * is idle.
 */
void FUNC_NAME(RADEONWaitForIdle)(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int            i    = 0;

#ifdef ACCEL_CP
    /* Make sure the CP is idle first */
    if (info->CPStarted) {
	int  ret;

	FLUSH_RING();

	for (;;) {
	    do {
		ret = drmCommandNone(info->drmFD, DRM_RADEON_CP_IDLE);
		if (ret && ret != -EBUSY) {
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			       "%s: CP idle %d\n", __FUNCTION__, ret);
		}
	    } while ((ret == -EBUSY) && (i++ < RADEON_TIMEOUT));

	    if (ret == 0) return;

	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Idle timed out, resetting engine...\n");
	    RADEONEngineReset(pScrn);
	    RADEONEngineRestore(pScrn);

	    /* Always restart the engine when doing CP 2D acceleration */
	    RADEONCP_RESET(pScrn, info);
	    RADEONCP_START(pScrn, info);
	}
    }
#endif

#if 0
    RADEONTRACE(("WaitForIdle (entering): %d entries, stat=0x%08x\n",
		     INREG(RADEON_RBBM_STATUS) & RADEON_RBBM_FIFOCNT_MASK,
		     INREG(RADEON_RBBM_STATUS)));
#endif

    /* Wait for the engine to go idle */
    RADEONWaitForFifoFunction(pScrn, 64);

    for (;;) {
	for (i = 0; i < RADEON_TIMEOUT; i++) {
	    if (!(INREG(RADEON_RBBM_STATUS) & RADEON_RBBM_ACTIVE)) {
		RADEONEngineFlush(pScrn);
		return;
	    }
	}
	RADEONTRACE(("Idle timed out: %u entries, stat=0x%08x\n",
		     INREG(RADEON_RBBM_STATUS) & RADEON_RBBM_FIFOCNT_MASK,
		     INREG(RADEON_RBBM_STATUS)));
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Idle timed out, resetting engine...\n");
	RADEONEngineReset(pScrn);
	RADEONEngineRestore(pScrn);
#ifdef XF86DRI
	if (info->directRenderingEnabled) {
	    RADEONCP_RESET(pScrn, info);
	    RADEONCP_START(pScrn, info);
	}
#endif
    }
}
