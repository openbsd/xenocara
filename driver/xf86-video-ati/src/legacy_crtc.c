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

#include <string.h>
#include <stdio.h>

/* X and server generic header files */
#include "xf86.h"
#include "xf86_OSproc.h"
#include "vgaHW.h"
#include "xf86Modes.h"

/* Driver data structures */
#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_probe.h"
#include "radeon_version.h"
#include "radeon_atombios.h"

#ifdef XF86DRI
#define _XF86DRI_SERVER_
#include "radeon_drm.h"
#include "sarea.h"
#ifdef DRM_IOCTL_MODESET_CTL
#include <sys/ioctl.h>
#endif
#endif

/* Write common registers */
void
RADEONRestoreCommonRegisters(ScrnInfoPtr pScrn,
			     RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (info->IsSecondary)
      return;

    OUTREG(RADEON_OVR_CLR,            restore->ovr_clr);
    OUTREG(RADEON_OVR_WID_LEFT_RIGHT, restore->ovr_wid_left_right);
    OUTREG(RADEON_OVR_WID_TOP_BOTTOM, restore->ovr_wid_top_bottom);
    OUTREG(RADEON_OV0_SCALE_CNTL,     restore->ov0_scale_cntl);
    OUTREG(RADEON_SUBPIC_CNTL,        restore->subpic_cntl);
    OUTREG(RADEON_VIPH_CONTROL,       restore->viph_control);
    OUTREG(RADEON_I2C_CNTL_1,         restore->i2c_cntl_1);
    OUTREG(RADEON_GEN_INT_CNTL,       restore->gen_int_cntl);
    OUTREG(RADEON_CAP0_TRIG_CNTL,     restore->cap0_trig_cntl);
    OUTREG(RADEON_CAP1_TRIG_CNTL,     restore->cap1_trig_cntl);
    OUTREG(RADEON_BUS_CNTL,           restore->bus_cntl);
    OUTREG(RADEON_SURFACE_CNTL,       restore->surface_cntl);

    if ((info->ChipFamily == CHIP_FAMILY_RS400)  ||
	(info->ChipFamily == CHIP_FAMILY_RS480)) {
	OUTREG(RS400_DISP2_REQ_CNTL1, restore->disp2_req_cntl1);
	OUTREG(RS400_DISP2_REQ_CNTL2, restore->disp2_req_cntl2);
	OUTREG(RS400_DMIF_MEM_CNTL1,  restore->dmif_mem_cntl1);
	OUTREG(RS400_DISP1_REQ_CNTL1, restore->disp1_req_cntl1);
    }

    /* Workaround for the VT switching problem in dual-head mode.  This
     * problem only occurs on RV style chips, typically when a FP and
     * CRT are connected.
     */
    if (pRADEONEnt->HasCRTC2 &&
	info->ChipFamily != CHIP_FAMILY_R200 &&
	!IS_R300_VARIANT) {
	uint32_t tmp;

	tmp = INREG(RADEON_DAC_CNTL2);
	OUTREG(RADEON_DAC_CNTL2, tmp & ~RADEON_DAC2_DAC_CLK_SEL);
	usleep(100000);
    }
}

void
RADEONRestoreCrtcBase(ScrnInfoPtr pScrn,
		      RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (IS_R300_VARIANT)
	OUTREG(R300_CRTC_TILE_X0_Y0,    restore->crtc_tile_x0_y0);
    OUTREG(RADEON_CRTC_OFFSET_CNTL,     restore->crtc_offset_cntl);
    OUTREG(RADEON_CRTC_OFFSET,          restore->crtc_offset);
}

void
RADEONRestoreCrtc2Base(ScrnInfoPtr pScrn,
		       RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (IS_R300_VARIANT)
	OUTREG(R300_CRTC2_TILE_X0_Y0,    restore->crtc2_tile_x0_y0);
    OUTREG(RADEON_CRTC2_OFFSET_CNTL,     restore->crtc2_offset_cntl);
    OUTREG(RADEON_CRTC2_OFFSET,          restore->crtc2_offset);
}

/* Write CRTC registers */
void
RADEONRestoreCrtcRegisters(ScrnInfoPtr pScrn,
			   RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Programming CRTC1, offset: 0x%08x\n",
		   (unsigned)restore->crtc_offset);

    /* We prevent the CRTC from hitting the memory controller until
     * fully programmed
     */
    OUTREG(RADEON_CRTC_GEN_CNTL, restore->crtc_gen_cntl |
	   RADEON_CRTC_DISP_REQ_EN_B);

    OUTREGP(RADEON_CRTC_EXT_CNTL,
	    restore->crtc_ext_cntl,
	    RADEON_CRTC_VSYNC_DIS |
	    RADEON_CRTC_HSYNC_DIS |
	    RADEON_CRTC_DISPLAY_DIS);

    OUTREG(RADEON_CRTC_H_TOTAL_DISP,    restore->crtc_h_total_disp);
    OUTREG(RADEON_CRTC_H_SYNC_STRT_WID, restore->crtc_h_sync_strt_wid);
    OUTREG(RADEON_CRTC_V_TOTAL_DISP,    restore->crtc_v_total_disp);
    OUTREG(RADEON_CRTC_V_SYNC_STRT_WID, restore->crtc_v_sync_strt_wid);

    RADEONRestoreCrtcBase(pScrn, restore);

    OUTREG(RADEON_CRTC_PITCH,           restore->crtc_pitch);
    OUTREG(RADEON_DISP_MERGE_CNTL,      restore->disp_merge_cntl);

    if (info->IsDellServer) {
	OUTREG(RADEON_TV_DAC_CNTL, restore->tv_dac_cntl);
	OUTREG(RADEON_DISP_HW_DEBUG, restore->disp_hw_debug);
	OUTREG(RADEON_DAC_CNTL2, restore->dac2_cntl);
	OUTREG(RADEON_CRTC2_GEN_CNTL, restore->crtc2_gen_cntl);
    }

    OUTREG(RADEON_CRTC_GEN_CNTL, restore->crtc_gen_cntl);
}

/* Write CRTC2 registers */
void
RADEONRestoreCrtc2Registers(ScrnInfoPtr pScrn,
			    RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    /*    uint32_t	   crtc2_gen_cntl;*/

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Programming CRTC2, offset: 0x%08x\n",
		   (unsigned)restore->crtc2_offset);

    /* We prevent the CRTC from hitting the memory controller until
     * fully programmed
     */
    OUTREG(RADEON_CRTC2_GEN_CNTL,
	   restore->crtc2_gen_cntl | RADEON_CRTC2_VSYNC_DIS |
	   RADEON_CRTC2_HSYNC_DIS | RADEON_CRTC2_DISP_DIS |
	   RADEON_CRTC2_DISP_REQ_EN_B);

    OUTREG(RADEON_CRTC2_H_TOTAL_DISP,    restore->crtc2_h_total_disp);
    OUTREG(RADEON_CRTC2_H_SYNC_STRT_WID, restore->crtc2_h_sync_strt_wid);
    OUTREG(RADEON_CRTC2_V_TOTAL_DISP,    restore->crtc2_v_total_disp);
    OUTREG(RADEON_CRTC2_V_SYNC_STRT_WID, restore->crtc2_v_sync_strt_wid);

    OUTREG(RADEON_FP_H2_SYNC_STRT_WID,   restore->fp_h2_sync_strt_wid);
    OUTREG(RADEON_FP_V2_SYNC_STRT_WID,   restore->fp_v2_sync_strt_wid);

    RADEONRestoreCrtc2Base(pScrn, restore);

    OUTREG(RADEON_CRTC2_PITCH,           restore->crtc2_pitch);
    OUTREG(RADEON_DISP2_MERGE_CNTL,      restore->disp2_merge_cntl);

    OUTREG(RADEON_CRTC2_GEN_CNTL, restore->crtc2_gen_cntl);

}

static void
RADEONPLLWaitForReadUpdateComplete(ScrnInfoPtr pScrn)
{
    int i = 0;

    /* FIXME: Certain revisions of R300 can't recover here.  Not sure of
       the cause yet, but this workaround will mask the problem for now.
       Other chips usually will pass at the very first test, so the
       workaround shouldn't have any effect on them. */
    for (i = 0;
	 (i < 10000 &&
	  INPLL(pScrn, RADEON_PPLL_REF_DIV) & RADEON_PPLL_ATOMIC_UPDATE_R);
	 i++);
}

static void
RADEONPLLWriteUpdate(ScrnInfoPtr pScrn)
{
    while (INPLL(pScrn, RADEON_PPLL_REF_DIV) & RADEON_PPLL_ATOMIC_UPDATE_R);

    OUTPLLP(pScrn, RADEON_PPLL_REF_DIV,
	    RADEON_PPLL_ATOMIC_UPDATE_W,
	    ~(RADEON_PPLL_ATOMIC_UPDATE_W));
}

static void
RADEONPLL2WaitForReadUpdateComplete(ScrnInfoPtr pScrn)
{
    int i = 0;

    /* FIXME: Certain revisions of R300 can't recover here.  Not sure of
       the cause yet, but this workaround will mask the problem for now.
       Other chips usually will pass at the very first test, so the
       workaround shouldn't have any effect on them. */
    for (i = 0;
	 (i < 10000 &&
	  INPLL(pScrn, RADEON_P2PLL_REF_DIV) & RADEON_P2PLL_ATOMIC_UPDATE_R);
	 i++);
}

static void
RADEONPLL2WriteUpdate(ScrnInfoPtr pScrn)
{
    while (INPLL(pScrn, RADEON_P2PLL_REF_DIV) & RADEON_P2PLL_ATOMIC_UPDATE_R);

    OUTPLLP(pScrn, RADEON_P2PLL_REF_DIV,
	    RADEON_P2PLL_ATOMIC_UPDATE_W,
	    ~(RADEON_P2PLL_ATOMIC_UPDATE_W));
}

static uint8_t
RADEONComputePLLGain(uint16_t reference_freq, uint16_t ref_div,
		     uint16_t fb_div)
{
    unsigned vcoFreq;

    if (!ref_div)
	return 1;

    vcoFreq = ((unsigned)reference_freq * fb_div) / ref_div;

    /*
     * This is horribly crude: the VCO frequency range is divided into
     * 3 parts, each part having a fixed PLL gain value.
     */
    if (vcoFreq >= 30000)
	/*
	 * [300..max] MHz : 7
	 */
	return 7;
    else if (vcoFreq >= 18000)
	/*
	 * [180..300) MHz : 4
	 */
        return 4;
    else
	/*
	 * [0..180) MHz : 1
	 */
        return 1;
}

/* Write PLL registers */
void
RADEONRestorePLLRegisters(ScrnInfoPtr pScrn,
			  RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint8_t pllGain;

#if defined(__powerpc__)
    /* apparently restoring the pll causes a hang??? */
    if (info->MacModel == RADEON_MAC_IBOOK)
	return;
#endif

    pllGain = RADEONComputePLLGain(info->pll.reference_freq,
				   restore->ppll_ref_div & RADEON_PPLL_REF_DIV_MASK,
				   restore->ppll_div_3 & RADEON_PPLL_FB3_DIV_MASK);

    if (info->IsMobility) {
        /* A temporal workaround for the occational blanking on certain laptop panels.
           This appears to related to the PLL divider registers (fail to lock?).
	   It occurs even when all dividers are the same with their old settings.
           In this case we really don't need to fiddle with PLL registers.
           By doing this we can avoid the blanking problem with some panels.
        */
        if ((restore->ppll_ref_div == (INPLL(pScrn, RADEON_PPLL_REF_DIV) & RADEON_PPLL_REF_DIV_MASK)) &&
	    (restore->ppll_div_3 == (INPLL(pScrn, RADEON_PPLL_DIV_3) & 
				     (RADEON_PPLL_POST3_DIV_MASK | RADEON_PPLL_FB3_DIV_MASK)))) {
	    OUTREGP(RADEON_CLOCK_CNTL_INDEX,
		    RADEON_PLL_DIV_SEL,
		    ~(RADEON_PLL_DIV_SEL));
	    RADEONPllErrataAfterIndex(info);
	    return;
	}
    }

    OUTPLLP(pScrn, RADEON_VCLK_ECP_CNTL,
	    RADEON_VCLK_SRC_SEL_CPUCLK,
	    ~(RADEON_VCLK_SRC_SEL_MASK));

    OUTPLLP(pScrn,
	    RADEON_PPLL_CNTL,
	    RADEON_PPLL_RESET
	    | RADEON_PPLL_ATOMIC_UPDATE_EN
	    | RADEON_PPLL_VGA_ATOMIC_UPDATE_EN
	    | ((uint32_t)pllGain << RADEON_PPLL_PVG_SHIFT),
	    ~(RADEON_PPLL_RESET
	      | RADEON_PPLL_ATOMIC_UPDATE_EN
	      | RADEON_PPLL_VGA_ATOMIC_UPDATE_EN
	      | RADEON_PPLL_PVG_MASK));

    OUTREGP(RADEON_CLOCK_CNTL_INDEX,
	    RADEON_PLL_DIV_SEL,
	    ~(RADEON_PLL_DIV_SEL));
    RADEONPllErrataAfterIndex(info);

    if (IS_R300_VARIANT ||
	(info->ChipFamily == CHIP_FAMILY_RS300) ||
	(info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480)) {
	if (restore->ppll_ref_div & R300_PPLL_REF_DIV_ACC_MASK) {
	    /* When restoring console mode, use saved PPLL_REF_DIV
	     * setting.
	     */
	    OUTPLLP(pScrn, RADEON_PPLL_REF_DIV,
		    restore->ppll_ref_div,
		    0);
	} else {
	    /* R300 uses ref_div_acc field as real ref divider */
	    OUTPLLP(pScrn, RADEON_PPLL_REF_DIV,
		    (restore->ppll_ref_div << R300_PPLL_REF_DIV_ACC_SHIFT),
		    ~R300_PPLL_REF_DIV_ACC_MASK);
	}
    } else {
	OUTPLLP(pScrn, RADEON_PPLL_REF_DIV,
		restore->ppll_ref_div,
		~RADEON_PPLL_REF_DIV_MASK);
    }

    OUTPLLP(pScrn, RADEON_PPLL_DIV_3,
	    restore->ppll_div_3,
	    ~RADEON_PPLL_FB3_DIV_MASK);

    OUTPLLP(pScrn, RADEON_PPLL_DIV_3,
	    restore->ppll_div_3,
	    ~RADEON_PPLL_POST3_DIV_MASK);

    RADEONPLLWriteUpdate(pScrn);
    RADEONPLLWaitForReadUpdateComplete(pScrn);

    OUTPLL(pScrn, RADEON_HTOTAL_CNTL, restore->htotal_cntl);

    OUTPLLP(pScrn, RADEON_PPLL_CNTL,
	    0,
	    ~(RADEON_PPLL_RESET
	      | RADEON_PPLL_SLEEP
	      | RADEON_PPLL_ATOMIC_UPDATE_EN
	      | RADEON_PPLL_VGA_ATOMIC_UPDATE_EN));

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Wrote: 0x%08x 0x%08x 0x%08x (0x%08x)\n",
		   restore->ppll_ref_div,
		   restore->ppll_div_3,
		   (unsigned)restore->htotal_cntl,
		   INPLL(pScrn, RADEON_PPLL_CNTL));
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Wrote: rd=%d, fd=%d, pd=%d\n",
		   restore->ppll_ref_div & RADEON_PPLL_REF_DIV_MASK,
		   restore->ppll_div_3 & RADEON_PPLL_FB3_DIV_MASK,
		   (restore->ppll_div_3 & RADEON_PPLL_POST3_DIV_MASK) >> 16);

    usleep(50000); /* Let the clock to lock */

    OUTPLLP(pScrn, RADEON_VCLK_ECP_CNTL,
	    RADEON_VCLK_SRC_SEL_PPLLCLK,
	    ~(RADEON_VCLK_SRC_SEL_MASK));

    /*OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, restore->vclk_ecp_cntl);*/

    ErrorF("finished PLL1\n");

}

/* Write PLL2 registers */
void
RADEONRestorePLL2Registers(ScrnInfoPtr pScrn,
			   RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    uint8_t pllGain;

    pllGain = RADEONComputePLLGain(info->pll.reference_freq,
                                   restore->p2pll_ref_div & RADEON_P2PLL_REF_DIV_MASK,
                                   restore->p2pll_div_0 & RADEON_P2PLL_FB0_DIV_MASK);


    OUTPLLP(pScrn, RADEON_PIXCLKS_CNTL,
	    RADEON_PIX2CLK_SRC_SEL_CPUCLK,
	    ~(RADEON_PIX2CLK_SRC_SEL_MASK));

    OUTPLLP(pScrn,
	    RADEON_P2PLL_CNTL,
	    RADEON_P2PLL_RESET
	    | RADEON_P2PLL_ATOMIC_UPDATE_EN
	    | ((uint32_t)pllGain << RADEON_P2PLL_PVG_SHIFT),
	    ~(RADEON_P2PLL_RESET
	      | RADEON_P2PLL_ATOMIC_UPDATE_EN
	      | RADEON_P2PLL_PVG_MASK));


    OUTPLLP(pScrn, RADEON_P2PLL_REF_DIV,
	    restore->p2pll_ref_div,
	    ~RADEON_P2PLL_REF_DIV_MASK);

    OUTPLLP(pScrn, RADEON_P2PLL_DIV_0,
	    restore->p2pll_div_0,
	    ~RADEON_P2PLL_FB0_DIV_MASK);

    OUTPLLP(pScrn, RADEON_P2PLL_DIV_0,
	    restore->p2pll_div_0,
	    ~RADEON_P2PLL_POST0_DIV_MASK);

    RADEONPLL2WriteUpdate(pScrn);
    RADEONPLL2WaitForReadUpdateComplete(pScrn);

    OUTPLL(pScrn, RADEON_HTOTAL2_CNTL, restore->htotal_cntl2);

    OUTPLLP(pScrn, RADEON_P2PLL_CNTL,
	    0,
	    ~(RADEON_P2PLL_RESET
	      | RADEON_P2PLL_SLEEP
	      | RADEON_P2PLL_ATOMIC_UPDATE_EN));

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Wrote2: 0x%08x 0x%08x 0x%08x (0x%08x)\n",
		   (unsigned)restore->p2pll_ref_div,
		   (unsigned)restore->p2pll_div_0,
		   (unsigned)restore->htotal_cntl2,
		   INPLL(pScrn, RADEON_P2PLL_CNTL));
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Wrote2: rd=%u, fd=%u, pd=%u\n",
		   (unsigned)restore->p2pll_ref_div & RADEON_P2PLL_REF_DIV_MASK,
		   (unsigned)restore->p2pll_div_0 & RADEON_P2PLL_FB0_DIV_MASK,
		   (unsigned)((restore->p2pll_div_0 &
			       RADEON_P2PLL_POST0_DIV_MASK) >>16));

    usleep(5000); /* Let the clock to lock */

    OUTPLLP(pScrn, RADEON_PIXCLKS_CNTL,
	    RADEON_PIX2CLK_SRC_SEL_P2PLLCLK,
	    ~(RADEON_PIX2CLK_SRC_SEL_MASK));

    OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, restore->pixclks_cntl);

    ErrorF("finished PLL2\n");

}

/* Read common registers */
void
RADEONSaveCommonRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    save->ovr_clr            = INREG(RADEON_OVR_CLR);
    save->ovr_wid_left_right = INREG(RADEON_OVR_WID_LEFT_RIGHT);
    save->ovr_wid_top_bottom = INREG(RADEON_OVR_WID_TOP_BOTTOM);
    save->ov0_scale_cntl     = INREG(RADEON_OV0_SCALE_CNTL);
    save->subpic_cntl        = INREG(RADEON_SUBPIC_CNTL);
    save->viph_control       = INREG(RADEON_VIPH_CONTROL);
    save->i2c_cntl_1         = INREG(RADEON_I2C_CNTL_1);
    save->gen_int_cntl       = INREG(RADEON_GEN_INT_CNTL);
    save->cap0_trig_cntl     = INREG(RADEON_CAP0_TRIG_CNTL);
    save->cap1_trig_cntl     = INREG(RADEON_CAP1_TRIG_CNTL);
    save->bus_cntl           = INREG(RADEON_BUS_CNTL);
    save->surface_cntl	     = INREG(RADEON_SURFACE_CNTL);
    save->grph_buffer_cntl   = INREG(RADEON_GRPH_BUFFER_CNTL);
    save->grph2_buffer_cntl  = INREG(RADEON_GRPH2_BUFFER_CNTL);

    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480)) {
	save->disp2_req_cntl1 = INREG(RS400_DISP2_REQ_CNTL1);
	save->disp2_req_cntl2 = INREG(RS400_DISP2_REQ_CNTL2);
	save->dmif_mem_cntl1  = INREG(RS400_DMIF_MEM_CNTL1);
	save->disp1_req_cntl1 = INREG(RS400_DISP1_REQ_CNTL1);
    }
}

/* Read CRTC registers */
void
RADEONSaveCrtcRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    save->crtc_gen_cntl        = INREG(RADEON_CRTC_GEN_CNTL);
    save->crtc_ext_cntl        = INREG(RADEON_CRTC_EXT_CNTL);
    save->crtc_h_total_disp    = INREG(RADEON_CRTC_H_TOTAL_DISP);
    save->crtc_h_sync_strt_wid = INREG(RADEON_CRTC_H_SYNC_STRT_WID);
    save->crtc_v_total_disp    = INREG(RADEON_CRTC_V_TOTAL_DISP);
    save->crtc_v_sync_strt_wid = INREG(RADEON_CRTC_V_SYNC_STRT_WID);

    save->crtc_offset          = INREG(RADEON_CRTC_OFFSET);
    save->crtc_offset_cntl     = INREG(RADEON_CRTC_OFFSET_CNTL);
    save->crtc_pitch           = INREG(RADEON_CRTC_PITCH);
    save->disp_merge_cntl      = INREG(RADEON_DISP_MERGE_CNTL);

    if (IS_R300_VARIANT)
	save->crtc_tile_x0_y0 =  INREG(R300_CRTC_TILE_X0_Y0);

    if (info->IsDellServer) {
	save->tv_dac_cntl      = INREG(RADEON_TV_DAC_CNTL);
	save->dac2_cntl        = INREG(RADEON_DAC_CNTL2);
	save->disp_hw_debug    = INREG (RADEON_DISP_HW_DEBUG);
	save->crtc2_gen_cntl   = INREG(RADEON_CRTC2_GEN_CNTL);
    }

    /* track if the crtc is enabled for text restore */
    if (save->crtc_ext_cntl & RADEON_CRTC_DISPLAY_DIS)
	info->crtc_on = FALSE;
    else
	info->crtc_on = TRUE;

}

/* Read CRTC2 registers */
void
RADEONSaveCrtc2Registers(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    save->crtc2_gen_cntl        = INREG(RADEON_CRTC2_GEN_CNTL);
    save->crtc2_h_total_disp    = INREG(RADEON_CRTC2_H_TOTAL_DISP);
    save->crtc2_h_sync_strt_wid = INREG(RADEON_CRTC2_H_SYNC_STRT_WID);
    save->crtc2_v_total_disp    = INREG(RADEON_CRTC2_V_TOTAL_DISP);
    save->crtc2_v_sync_strt_wid = INREG(RADEON_CRTC2_V_SYNC_STRT_WID);

    save->crtc2_offset          = INREG(RADEON_CRTC2_OFFSET);
    save->crtc2_offset_cntl     = INREG(RADEON_CRTC2_OFFSET_CNTL);
    save->crtc2_pitch           = INREG(RADEON_CRTC2_PITCH);

    if (IS_R300_VARIANT)
	save->crtc2_tile_x0_y0 =  INREG(R300_CRTC2_TILE_X0_Y0);

    save->fp_h2_sync_strt_wid   = INREG (RADEON_FP_H2_SYNC_STRT_WID);
    save->fp_v2_sync_strt_wid   = INREG (RADEON_FP_V2_SYNC_STRT_WID);

    save->disp2_merge_cntl      = INREG(RADEON_DISP2_MERGE_CNTL);

    /* track if the crtc is enabled for text restore */
    if (save->crtc2_gen_cntl & RADEON_CRTC2_DISP_DIS)
	info->crtc2_on = FALSE;
    else
	info->crtc2_on = TRUE;

}

/* Read PLL registers */
void
RADEONSavePLLRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    save->ppll_ref_div = INPLL(pScrn, RADEON_PPLL_REF_DIV);
    save->ppll_div_3   = INPLL(pScrn, RADEON_PPLL_DIV_3);
    save->htotal_cntl  = INPLL(pScrn, RADEON_HTOTAL_CNTL);
    save->vclk_ecp_cntl = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Read: 0x%08x 0x%08x 0x%08x\n",
		   save->ppll_ref_div,
		   save->ppll_div_3,
		   (unsigned)save->htotal_cntl);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Read: rd=%d, fd=%d, pd=%d\n",
		   save->ppll_ref_div & RADEON_PPLL_REF_DIV_MASK,
		   save->ppll_div_3 & RADEON_PPLL_FB3_DIV_MASK,
		   (save->ppll_div_3 & RADEON_PPLL_POST3_DIV_MASK) >> 16);
}

/* Read PLL registers */
void
RADEONSavePLL2Registers(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    save->p2pll_ref_div = INPLL(pScrn, RADEON_P2PLL_REF_DIV);
    save->p2pll_div_0   = INPLL(pScrn, RADEON_P2PLL_DIV_0);
    save->htotal_cntl2  = INPLL(pScrn, RADEON_HTOTAL2_CNTL);
    save->pixclks_cntl  = INPLL(pScrn, RADEON_PIXCLKS_CNTL);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Read: 0x%08x 0x%08x 0x%08x\n",
		   (unsigned)save->p2pll_ref_div,
		   (unsigned)save->p2pll_div_0,
		   (unsigned)save->htotal_cntl2);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Read: rd=%u, fd=%u, pd=%u\n",
		   (unsigned)(save->p2pll_ref_div & RADEON_P2PLL_REF_DIV_MASK),
		   (unsigned)(save->p2pll_div_0 & RADEON_P2PLL_FB0_DIV_MASK),
		   (unsigned)((save->p2pll_div_0 & RADEON_P2PLL_POST0_DIV_MASK)
			      >> 16));
}

void
radeon_crtc_modeset_ioctl(xf86CrtcPtr crtc, Bool post)
{
#if defined(XF86DRI) && defined(DRM_IOCTL_MODESET_CTL)
    RADEONInfoPtr info = RADEONPTR(crtc->scrn);
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    struct drm_modeset_ctl modeset;
    unsigned char *RADEONMMIO = info->MMIO;

    if (!info->directRenderingEnabled)
	return;

    if (info->ChipFamily >= CHIP_FAMILY_R600)
	return;

    modeset.crtc = radeon_crtc->crtc_id;
    modeset.cmd = post ? _DRM_POST_MODESET : _DRM_PRE_MODESET;

    ioctl(info->dri->drmFD, DRM_IOCTL_MODESET_CTL, &modeset);

    info->ModeReg->gen_int_cntl = INREG( RADEON_GEN_INT_CNTL );
#endif
}

void
legacy_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
    uint32_t mask;
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(crtc->scrn);
    unsigned char *RADEONMMIO = pRADEONEnt->MMIO;

    if (radeon_crtc->crtc_id)
	mask = (RADEON_CRTC2_DISP_DIS |
		RADEON_CRTC2_VSYNC_DIS |
		RADEON_CRTC2_HSYNC_DIS |
		RADEON_CRTC2_DISP_REQ_EN_B);
    else
	mask = (RADEON_CRTC_DISPLAY_DIS |
		RADEON_CRTC_HSYNC_DIS |
		RADEON_CRTC_VSYNC_DIS);

    switch(mode) {
    case DPMSModeOn:
	if (radeon_crtc->crtc_id) {
	    OUTREGP(RADEON_CRTC2_GEN_CNTL, RADEON_CRTC2_EN, ~(RADEON_CRTC2_EN | mask));
	} else {
	    OUTREGP(RADEON_CRTC_GEN_CNTL, RADEON_CRTC_EN, ~(RADEON_CRTC_EN | RADEON_CRTC_DISP_REQ_EN_B));
	    OUTREGP(RADEON_CRTC_EXT_CNTL, 0, ~mask);
	}
	break;
    case DPMSModeStandby:
    case DPMSModeSuspend:
    case DPMSModeOff:
	if (radeon_crtc->crtc_id) {
	    OUTREGP(RADEON_CRTC2_GEN_CNTL, mask, ~(RADEON_CRTC2_EN | mask));
	} else {
	    OUTREGP(RADEON_CRTC_GEN_CNTL, RADEON_CRTC_DISP_REQ_EN_B, ~(RADEON_CRTC_EN | RADEON_CRTC_DISP_REQ_EN_B));
	    OUTREGP(RADEON_CRTC_EXT_CNTL, mask, ~mask);
	}
	break;
    }
}


/* Define common registers for requested video mode */
void
RADEONInitCommonRegisters(RADEONSavePtr save, RADEONInfoPtr info)
{
    save->ovr_clr            = 0;
    save->ovr_wid_left_right = 0;
    save->ovr_wid_top_bottom = 0;
    save->ov0_scale_cntl     = 0;
    save->subpic_cntl        = 0;
    save->viph_control       = 0;
    save->i2c_cntl_1         = 0;
    save->rbbm_soft_reset    = 0;
    save->cap0_trig_cntl     = 0;
    save->cap1_trig_cntl     = 0;
    save->bus_cntl           = info->BusCntl;

    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480)) {
	save->disp2_req_cntl1 = info->SavedReg->disp2_req_cntl1;
	save->disp2_req_cntl2 = info->SavedReg->disp2_req_cntl2;
	save->dmif_mem_cntl1  = info->SavedReg->dmif_mem_cntl1;
	save->disp1_req_cntl1 = info->SavedReg->disp1_req_cntl1;
    }

    /*
     * If bursts are enabled, turn on discards
     * Radeon doesn't have write bursts
     */
    if (save->bus_cntl & (RADEON_BUS_READ_BURST))
	save->bus_cntl |= RADEON_BUS_RD_DISCARD_EN;
}

void
RADEONInitSurfaceCntl(xf86CrtcPtr crtc, RADEONSavePtr save)
{
    save->surface_cntl = 0;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* We must set both apertures as they can be both used to map the entire
     * video memory. -BenH.
     */
    switch (crtc->scrn->bitsPerPixel) {
    case 16:
	save->surface_cntl |= RADEON_NONSURF_AP0_SWP_16BPP;
	save->surface_cntl |= RADEON_NONSURF_AP1_SWP_16BPP;
	break;

    case 32:
	save->surface_cntl |= RADEON_NONSURF_AP0_SWP_32BPP;
	save->surface_cntl |= RADEON_NONSURF_AP1_SWP_32BPP;
	break;
    }
#endif

}

void
RADEONInitCrtcBase(xf86CrtcPtr crtc, RADEONSavePtr save,
		   int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    int    Base;
#ifdef XF86DRI
    drm_radeon_sarea_t *pSAREAPriv;
    XF86DRISAREAPtr pSAREA;
#endif

    save->crtc_offset      = pScrn->fbOffset;
#ifdef XF86DRI
    if (info->dri && info->dri->allowPageFlip)
	save->crtc_offset_cntl = RADEON_CRTC_OFFSET_FLIP_CNTL;
    else
#endif
	save->crtc_offset_cntl = 0;

    if (info->tilingEnabled && (crtc->rotatedData == NULL)) {
       if (IS_R300_VARIANT)
          save->crtc_offset_cntl |= (R300_CRTC_X_Y_MODE_EN |
				     R300_CRTC_MICRO_TILE_BUFFER_DIS |
				     R300_CRTC_MACRO_TILE_EN);
       else
          save->crtc_offset_cntl |= RADEON_CRTC_TILE_EN;
    }
    else {
       if (IS_R300_VARIANT)
          save->crtc_offset_cntl &= ~(R300_CRTC_X_Y_MODE_EN |
				      R300_CRTC_MICRO_TILE_BUFFER_DIS |
				      R300_CRTC_MACRO_TILE_EN);
       else
          save->crtc_offset_cntl &= ~RADEON_CRTC_TILE_EN;
    }

    Base = pScrn->fbOffset;

    if (info->tilingEnabled && (crtc->rotatedData == NULL)) {
        if (IS_R300_VARIANT) {
	/* On r300/r400 when tiling is enabled crtc_offset is set to the address of
	 * the surface.  the x/y offsets are handled by the X_Y tile reg for each crtc
	 * Makes tiling MUCH easier.
	 */
             save->crtc_tile_x0_y0 = x | (y << 16);
             Base &= ~0x7ff;
         } else {
	     /* note we cannot really simply use the info->ModeReg.crtc_offset_cntl value, since the
		drm might have set FLIP_CNTL since we wrote that. Unfortunately FLIP_CNTL causes
		flickering when scrolling vertically in a virtual screen, possibly because crtc will
		pick up the new offset value at the end of each scanline, but the new offset_cntl value
		only after a vsync. We'd probably need to wait (in drm) for vsync and only then update
		OFFSET and OFFSET_CNTL, if the y coord has changed. Seems hard to fix. */
	     /*save->crtc_offset_cntl = INREG(RADEON_CRTC_OFFSET_CNTL) & ~0xf;*/
#if 0
	     /* try to get rid of flickering when scrolling at least for 2d */
#ifdef XF86DRI
	     if (!info->have3DWindows)
#endif
		 save->crtc_offset_cntl &= ~RADEON_CRTC_OFFSET_FLIP_CNTL;
#endif
	     
             int byteshift = info->CurrentLayout.bitsPerPixel >> 4;
             /* crtc uses 256(bytes)x8 "half-tile" start addresses? */
             int tile_addr = (((y >> 3) * info->CurrentLayout.displayWidth + x) >> (8 - byteshift)) << 11;
             Base += tile_addr + ((x << byteshift) % 256) + ((y % 8) << 8);
             save->crtc_offset_cntl = save->crtc_offset_cntl | (y % 16);
         }
    }
    else {
       int offset = y * info->CurrentLayout.displayWidth + x;
       switch (info->CurrentLayout.pixel_code) {
       case 15:
       case 16: offset *= 2; break;
       case 24: offset *= 3; break;
       case 32: offset *= 4; break;
       }
       Base += offset;
    }

    if (crtc->rotatedData != NULL) {
	Base = pScrn->fbOffset + (char *)crtc->rotatedData - (char *)info->FB;
    }

    Base &= ~7;                 /* 3 lower bits are always 0 */


#ifdef XF86DRI
    if (info->directRenderingInited) {
	/* note cannot use pScrn->pScreen since this is unitialized when called from
	   RADEONScreenInit, and we need to call from there to get mergedfb + pageflip working */
        /*** NOTE: r3/4xx will need sarea and drm pageflip updates to handle the xytile regs for
	 *** pageflipping!
	 ***/
	pSAREAPriv = DRIGetSAREAPrivate(xf86ScrnToScreen(pScrn));
	/* can't get at sarea in a semi-sane way? */
	pSAREA = (void *)((char*)pSAREAPriv - sizeof(XF86DRISAREARec));

	pSAREA->frame.x = (Base  / info->CurrentLayout.pixel_bytes)
	    % info->CurrentLayout.displayWidth;
	pSAREA->frame.y = (Base / info->CurrentLayout.pixel_bytes)
	    / info->CurrentLayout.displayWidth;
	pSAREA->frame.width = pScrn->frameX1 - x + 1;
	pSAREA->frame.height = pScrn->frameY1 - y + 1;

	if (pSAREAPriv->pfCurrentPage == 1) {
	    Base += info->dri->backOffset - info->dri->frontOffset;
	}
    }
#endif
    save->crtc_offset = Base;

}

/* Define CRTC registers for requested video mode */
static Bool
RADEONInitCrtcRegisters(xf86CrtcPtr crtc, RADEONSavePtr save,
			DisplayModePtr mode)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    int    format;
    int    hsync_start;
    int    hsync_wid;
    int    vsync_wid;

    switch (info->CurrentLayout.pixel_code) {
    case 4:  format = 1; break;
    case 8:  format = 2; break;
    case 15: format = 3; break;      /*  555 */
    case 16: format = 4; break;      /*  565 */
    case 24: format = 5; break;      /*  RGB */
    case 32: format = 6; break;      /* xRGB */
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Unsupported pixel depth (%d)\n",
		   info->CurrentLayout.bitsPerPixel);
	return FALSE;
    }

    /*save->bios_4_scratch = info->SavedReg->bios_4_scratch;*/
    save->crtc_gen_cntl = (RADEON_CRTC_EXT_DISP_EN
			   | (format << 8)
			   | ((mode->Flags & V_DBLSCAN)
			      ? RADEON_CRTC_DBL_SCAN_EN
			      : 0)
			   | ((mode->Flags & V_CSYNC)
			      ? RADEON_CRTC_CSYNC_EN
			      : 0)
			   | ((mode->Flags & V_INTERLACE)
			      ? RADEON_CRTC_INTERLACE_EN
			      : 0));

    /* 200M freezes on VT switch sometimes if CRTC is disabled */
    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480))
	save->crtc_gen_cntl |= RADEON_CRTC_EN;

    save->crtc_ext_cntl |= (RADEON_XCRT_CNT_EN|
			    RADEON_CRTC_VSYNC_DIS |
			    RADEON_CRTC_HSYNC_DIS |
			    RADEON_CRTC_DISPLAY_DIS);

    save->disp_merge_cntl = info->SavedReg->disp_merge_cntl;
    save->disp_merge_cntl &= ~RADEON_DISP_RGB_OFFSET_EN;

    save->crtc_h_total_disp = ((((mode->CrtcHTotal / 8) - 1) & 0x3ff)
			       | ((((mode->CrtcHDisplay / 8) - 1) & 0x1ff)
				  << 16));

    hsync_wid = (mode->CrtcHSyncEnd - mode->CrtcHSyncStart) / 8;
    if (!hsync_wid)       hsync_wid = 1;
    hsync_start = mode->CrtcHSyncStart - 8;

    save->crtc_h_sync_strt_wid = ((hsync_start & 0x1fff)
				  | ((hsync_wid & 0x3f) << 16)
				  | ((mode->Flags & V_NHSYNC)
				     ? RADEON_CRTC_H_SYNC_POL
				     : 0));

				/* This works for double scan mode. */
    save->crtc_v_total_disp = (((mode->CrtcVTotal - 1) & 0xffff)
			       | ((mode->CrtcVDisplay - 1) << 16));

    vsync_wid = mode->CrtcVSyncEnd - mode->CrtcVSyncStart;
    if (!vsync_wid)       vsync_wid = 1;

    save->crtc_v_sync_strt_wid = (((mode->CrtcVSyncStart - 1) & 0xfff)
				  | ((vsync_wid & 0x1f) << 16)
				  | ((mode->Flags & V_NVSYNC)
				     ? RADEON_CRTC_V_SYNC_POL
				     : 0));

    save->crtc_pitch  = (((pScrn->displayWidth * pScrn->bitsPerPixel) +
			  ((pScrn->bitsPerPixel * 8) -1)) /
			 (pScrn->bitsPerPixel * 8));
    save->crtc_pitch |= save->crtc_pitch << 16;

    if (info->IsDellServer) {
	save->dac2_cntl = info->SavedReg->dac2_cntl;
	save->tv_dac_cntl = info->SavedReg->tv_dac_cntl;
	save->crtc2_gen_cntl = info->SavedReg->crtc2_gen_cntl;
	save->disp_hw_debug = info->SavedReg->disp_hw_debug;

	save->dac2_cntl &= ~RADEON_DAC2_DAC_CLK_SEL;
	save->dac2_cntl |= RADEON_DAC2_DAC2_CLK_SEL;

	/* For CRT on DAC2, don't turn it on if BIOS didn't
	   enable it, even it's detected.
	*/
	save->disp_hw_debug |= RADEON_CRT2_DISP1_SEL;
	save->tv_dac_cntl &= ~((1<<2) | (3<<8) | (7<<24) | (0xff<<16));
	save->tv_dac_cntl |= (0x03 | (2<<8) | (0x58<<16));
    }

    return TRUE;
}


void
RADEONInitCrtc2Base(xf86CrtcPtr crtc, RADEONSavePtr save,
		    int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    int    Base;
#ifdef XF86DRI
    drm_radeon_sarea_t *pSAREAPriv;
    XF86DRISAREAPtr pSAREA;
#endif

    /* It seems all fancy options apart from pflip can be safely disabled
     */
    save->crtc2_offset      = pScrn->fbOffset;
#ifdef XF86DRI
    if (info->dri && info->dri->allowPageFlip)
	save->crtc2_offset_cntl = RADEON_CRTC_OFFSET_FLIP_CNTL;
    else
#endif
	save->crtc2_offset_cntl = 0;

    if (info->tilingEnabled && (crtc->rotatedData == NULL)) {
       if (IS_R300_VARIANT)
          save->crtc2_offset_cntl |= (R300_CRTC_X_Y_MODE_EN |
				      R300_CRTC_MICRO_TILE_BUFFER_DIS |
				      R300_CRTC_MACRO_TILE_EN);
       else
          save->crtc2_offset_cntl |= RADEON_CRTC_TILE_EN;
    }
    else {
       if (IS_R300_VARIANT)
          save->crtc2_offset_cntl &= ~(R300_CRTC_X_Y_MODE_EN |
				      R300_CRTC_MICRO_TILE_BUFFER_DIS |
				      R300_CRTC_MACRO_TILE_EN);
       else
          save->crtc2_offset_cntl &= ~RADEON_CRTC_TILE_EN;
    }

    Base = pScrn->fbOffset;

    if (info->tilingEnabled && (crtc->rotatedData == NULL)) {
        if (IS_R300_VARIANT) {
	/* On r300/r400 when tiling is enabled crtc_offset is set to the address of
	 * the surface.  the x/y offsets are handled by the X_Y tile reg for each crtc
	 * Makes tiling MUCH easier.
	 */
             save->crtc2_tile_x0_y0 = x | (y << 16);
             Base &= ~0x7ff;
         } else {
	     /* note we cannot really simply use the info->ModeReg.crtc_offset_cntl value, since the
		drm might have set FLIP_CNTL since we wrote that. Unfortunately FLIP_CNTL causes
		flickering when scrolling vertically in a virtual screen, possibly because crtc will
		pick up the new offset value at the end of each scanline, but the new offset_cntl value
		only after a vsync. We'd probably need to wait (in drm) for vsync and only then update
		OFFSET and OFFSET_CNTL, if the y coord has changed. Seems hard to fix. */
	     /*save->crtc2_offset_cntl = INREG(RADEON_CRTC2_OFFSET_CNTL) & ~0xf;*/
#if 0
	     /* try to get rid of flickering when scrolling at least for 2d */
#ifdef XF86DRI
	     if (!info->have3DWindows)
#endif
		 save->crtc2_offset_cntl &= ~RADEON_CRTC_OFFSET_FLIP_CNTL;
#endif

             int byteshift = info->CurrentLayout.bitsPerPixel >> 4;
             /* crtc uses 256(bytes)x8 "half-tile" start addresses? */
             int tile_addr = (((y >> 3) * info->CurrentLayout.displayWidth + x) >> (8 - byteshift)) << 11;
             Base += tile_addr + ((x << byteshift) % 256) + ((y % 8) << 8);
             save->crtc2_offset_cntl = save->crtc_offset_cntl | (y % 16);
         }
    }
    else {
       int offset = y * info->CurrentLayout.displayWidth + x;
       switch (info->CurrentLayout.pixel_code) {
       case 15:
       case 16: offset *= 2; break;
       case 24: offset *= 3; break;
       case 32: offset *= 4; break;
       }
       Base += offset;
    }

    if (crtc->rotatedData != NULL) {
	Base = pScrn->fbOffset + (char *)crtc->rotatedData - (char *)info->FB;
    }

    Base &= ~7;                 /* 3 lower bits are always 0 */

#ifdef XF86DRI
    if (info->directRenderingInited) {
	/* note cannot use pScrn->pScreen since this is unitialized when called from
	   RADEONScreenInit, and we need to call from there to get mergedfb + pageflip working */
        /*** NOTE: r3/4xx will need sarea and drm pageflip updates to handle the xytile regs for
	 *** pageflipping!
	 ***/
	pSAREAPriv = DRIGetSAREAPrivate(xf86ScrnToScreen(pScrn));
	/* can't get at sarea in a semi-sane way? */
	pSAREA = (void *)((char*)pSAREAPriv - sizeof(XF86DRISAREARec));

	pSAREAPriv->crtc2_base = Base;

	if (pSAREAPriv->pfCurrentPage == 1) {
	    Base += info->dri->backOffset - info->dri->frontOffset;
	}
    }
#endif
    save->crtc2_offset = Base;

}


/* Define CRTC2 registers for requested video mode */
static Bool
RADEONInitCrtc2Registers(xf86CrtcPtr crtc, RADEONSavePtr save,
			 DisplayModePtr mode)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    int    format;
    int    hsync_start;
    int    hsync_wid;
    int    vsync_wid;

    switch (info->CurrentLayout.pixel_code) {
    case 4:  format = 1; break;
    case 8:  format = 2; break;
    case 15: format = 3; break;      /*  555 */
    case 16: format = 4; break;      /*  565 */
    case 24: format = 5; break;      /*  RGB */
    case 32: format = 6; break;      /* xRGB */
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Unsupported pixel depth (%d)\n",
		   info->CurrentLayout.bitsPerPixel);
	return FALSE;
    }

    save->crtc2_h_total_disp =
	((((mode->CrtcHTotal / 8) - 1) & 0x3ff)
	 | ((((mode->CrtcHDisplay / 8) - 1) & 0x1ff) << 16));

    hsync_wid = (mode->CrtcHSyncEnd - mode->CrtcHSyncStart) / 8;
    if (!hsync_wid)       hsync_wid = 1;
    hsync_start = mode->CrtcHSyncStart - 8;

    save->crtc2_h_sync_strt_wid = ((hsync_start & 0x1fff)
				   | ((hsync_wid & 0x3f) << 16)
				   | ((mode->Flags & V_NHSYNC)
				      ? RADEON_CRTC_H_SYNC_POL
				      : 0));

				/* This works for double scan mode. */
    save->crtc2_v_total_disp = (((mode->CrtcVTotal - 1) & 0xffff)
				| ((mode->CrtcVDisplay - 1) << 16));

    vsync_wid = mode->CrtcVSyncEnd - mode->CrtcVSyncStart;
    if (!vsync_wid)       vsync_wid = 1;

    save->crtc2_v_sync_strt_wid = (((mode->CrtcVSyncStart - 1) & 0xfff)
				   | ((vsync_wid & 0x1f) << 16)
				   | ((mode->Flags & V_NVSYNC)
				      ? RADEON_CRTC2_V_SYNC_POL
				      : 0));

    save->crtc2_pitch  = ((pScrn->displayWidth * pScrn->bitsPerPixel) +
			  ((pScrn->bitsPerPixel * 8) -1)) / (pScrn->bitsPerPixel * 8);
    save->crtc2_pitch |= save->crtc2_pitch << 16;

    /* check to see if TV DAC is enabled for another crtc and keep it enabled */
    if (save->crtc2_gen_cntl & RADEON_CRTC2_CRT2_ON)
	save->crtc2_gen_cntl = RADEON_CRTC2_CRT2_ON;
    else
	save->crtc2_gen_cntl = 0;

    save->crtc2_gen_cntl |= ((format << 8)
			     | RADEON_CRTC2_VSYNC_DIS
			     | RADEON_CRTC2_HSYNC_DIS
			     | RADEON_CRTC2_DISP_DIS
			     | ((mode->Flags & V_DBLSCAN)
				? RADEON_CRTC2_DBL_SCAN_EN
				: 0)
			     | ((mode->Flags & V_CSYNC)
				? RADEON_CRTC2_CSYNC_EN
				: 0)
			     | ((mode->Flags & V_INTERLACE)
				? RADEON_CRTC2_INTERLACE_EN
				: 0));

    /* 200M freezes on VT switch sometimes if CRTC is disabled */
    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480))
	save->crtc2_gen_cntl |= RADEON_CRTC2_EN;

    save->disp2_merge_cntl = info->SavedReg->disp2_merge_cntl;
    save->disp2_merge_cntl &= ~(RADEON_DISP2_RGB_OFFSET_EN);

    save->fp_h2_sync_strt_wid = save->crtc2_h_sync_strt_wid;
    save->fp_v2_sync_strt_wid = save->crtc2_v_sync_strt_wid;

    return TRUE;
}


/* Define PLL registers for requested video mode */
static void
RADEONInitPLLRegisters(xf86CrtcPtr crtc, RADEONSavePtr save,
		       RADEONPLLPtr pll, DisplayModePtr mode,
		       int flags)
{
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    uint32_t feedback_div = 0;
    uint32_t frac_fb_div = 0;
    uint32_t reference_div = 0;
    uint32_t post_divider = 0;
    uint32_t freq = 0;

    struct {
	int divider;
	int bitvalue;
    } *post_div, post_divs[]   = {
				/* From RAGE 128 VR/RAGE 128 GL Register
				 * Reference Manual (Technical Reference
				 * Manual P/N RRG-G04100-C Rev. 0.04), page
				 * 3-17 (PLL_DIV_[3:0]).
				 */
	{  1, 0 },              /* VCLK_SRC                 */
	{  2, 1 },              /* VCLK_SRC/2               */
	{  4, 2 },              /* VCLK_SRC/4               */
	{  8, 3 },              /* VCLK_SRC/8               */
	{  3, 4 },              /* VCLK_SRC/3               */
	{ 16, 5 },              /* VCLK_SRC/16              */
	{  6, 6 },              /* VCLK_SRC/6               */
	{ 12, 7 },              /* VCLK_SRC/12              */
	{  0, 0 }
    };


    if ((flags & RADEON_PLL_USE_BIOS_DIVS) && info->UseBiosDividers) {
       save->ppll_ref_div = info->RefDivider;
       save->ppll_div_3   = info->FeedbackDivider | (info->PostDivider << 16);
       save->htotal_cntl  = 0;
       return;
    }

    if (xf86ReturnOptValBool(info->Options, OPTION_NEW_PLL, FALSE))
	radeon_crtc->pll_algo = RADEON_PLL_NEW;
    else
	radeon_crtc->pll_algo = RADEON_PLL_OLD;

    RADEONComputePLL(crtc, pll, mode->Clock, &freq,
		     &feedback_div, &frac_fb_div, &reference_div, &post_divider, flags);

    for (post_div = &post_divs[0]; post_div->divider; ++post_div) {
	if (post_div->divider == post_divider)
	    break;
    }

    if (!post_div->divider) {
	save->pll_output_freq = freq;
	post_div = &post_divs[0];
    }

    save->dot_clock_freq = freq;
    save->feedback_div   = feedback_div;
    save->reference_div  = reference_div;
    save->post_div       = post_divider;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "dc=%u, of=%u, fd=%d, rd=%d, pd=%d\n",
		   (unsigned)save->dot_clock_freq,
		   (unsigned)save->pll_output_freq,
		   save->feedback_div,
		   save->reference_div,
		   save->post_div);

    save->ppll_ref_div   = save->reference_div;

#if defined(__powerpc__)
    /* apparently programming this otherwise causes a hang??? */
    if (info->MacModel == RADEON_MAC_IBOOK)
	save->ppll_div_3 = 0x000600ad;
    else
#endif
    save->ppll_div_3     = (save->feedback_div | (post_div->bitvalue << 16));

    save->htotal_cntl    = mode->HTotal & 0x7;

    save->vclk_ecp_cntl = (info->SavedReg->vclk_ecp_cntl &
	    ~RADEON_VCLK_SRC_SEL_MASK) | RADEON_VCLK_SRC_SEL_PPLLCLK;
}

/* Define PLL2 registers for requested video mode */
static void
RADEONInitPLL2Registers(xf86CrtcPtr crtc, RADEONSavePtr save,
			RADEONPLLPtr pll, DisplayModePtr mode,
			int flags)
{
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    uint32_t feedback_div = 0;
    uint32_t frac_fb_div = 0;
    uint32_t reference_div = 0;
    uint32_t post_divider = 0;
    uint32_t freq = 0;

    struct {
	int divider;
	int bitvalue;
    } *post_div, post_divs[]   = {
				/* From RAGE 128 VR/RAGE 128 GL Register
				 * Reference Manual (Technical Reference
				 * Manual P/N RRG-G04100-C Rev. 0.04), page
				 * 3-17 (PLL_DIV_[3:0]).
				 */
	{  1, 0 },              /* VCLK_SRC                 */
	{  2, 1 },              /* VCLK_SRC/2               */
	{  4, 2 },              /* VCLK_SRC/4               */
	{  8, 3 },              /* VCLK_SRC/8               */
	{  3, 4 },              /* VCLK_SRC/3               */
	{  6, 6 },              /* VCLK_SRC/6               */
	{ 12, 7 },              /* VCLK_SRC/12              */
	{  0, 0 }
    };

    if ((flags & RADEON_PLL_USE_BIOS_DIVS) && info->UseBiosDividers) {
       save->p2pll_ref_div = info->RefDivider;
       save->p2pll_div_0   = info->FeedbackDivider | (info->PostDivider << 16);
       save->htotal_cntl2  = 0;
       return;
    }

    if (xf86ReturnOptValBool(info->Options, OPTION_NEW_PLL, FALSE))
	radeon_crtc->pll_algo = RADEON_PLL_NEW;
    else
	radeon_crtc->pll_algo = RADEON_PLL_OLD;

    RADEONComputePLL(crtc, pll, mode->Clock, &freq,
		     &feedback_div, &frac_fb_div, &reference_div, &post_divider, flags);

    for (post_div = &post_divs[0]; post_div->divider; ++post_div) {
	if (post_div->divider == post_divider)
	    break;
    }

    if (!post_div->divider) {
	save->pll_output_freq_2 = freq;
	post_div = &post_divs[0];
    }

    save->dot_clock_freq_2 = freq;
    save->feedback_div_2   = feedback_div;
    save->reference_div_2  = reference_div;
    save->post_div_2       = post_divider;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "dc=%u, of=%u, fd=%d, rd=%d, pd=%d\n",
		   (unsigned)save->dot_clock_freq_2,
		   (unsigned)save->pll_output_freq_2,
		   save->feedback_div_2,
		   save->reference_div_2,
		   save->post_div_2);

    save->p2pll_ref_div    = save->reference_div_2;

    save->p2pll_div_0      = (save->feedback_div_2 |
			      (post_div->bitvalue << 16));

    save->htotal_cntl2     = mode->HTotal & 0x7;

    save->pixclks_cntl     = ((info->SavedReg->pixclks_cntl &
			       ~(RADEON_PIX2CLK_SRC_SEL_MASK)) |
			      RADEON_PIX2CLK_SRC_SEL_P2PLLCLK);
}

static void
radeon_update_tv_routing(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    /* pixclks_cntl controls tv clock routing */
    OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, restore->pixclks_cntl);
}

/* Calculate display buffer watermark to prevent buffer underflow */
void
RADEONInitDispBandwidthLegacy(ScrnInfoPtr pScrn,
			      DisplayModePtr mode1, int pixel_bytes1,
			      DisplayModePtr mode2, int pixel_bytes2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt   = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    uint32_t temp, data, mem_trcd, mem_trp, mem_tras, mem_trbs=0;
    float mem_tcas;
    int k1, c;

    float MemTcas[8]  = {0, 1, 2, 3, 0, 1.5, 2.5, 0.0};
    float MemTcas_rs480[8]  = {0, 1, 2, 3, 0, 1.5, 2.5, 3.5};
    float MemTcas2[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    float MemTrbs[8]  = {1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5};
    float MemTrbs_r4xx[8]  = {4, 5, 6, 7, 8, 9, 10, 11};

    float mem_bw, peak_disp_bw;
    float min_mem_eff = 0.8;
    float sclk_eff, sclk_delay;
    float mc_latency_mclk, mc_latency_sclk, cur_latency_mclk, cur_latency_sclk;
    float disp_latency, disp_latency_overhead, disp_drain_rate = 0, disp_drain_rate2;
    float pix_clk, pix_clk2; /* in MHz */
    int cur_size = 16;       /* in octawords */
    int critical_point = 0, critical_point2;
    int stop_req, max_stop_req;
    float read_return_rate, time_disp1_drop_priority;

    /*
     * Set display0/1 priority up on r3/4xx in the memory controller for
     * high res modes if the user specifies HIGH for displaypriority
     * option.
     */
    if ((info->DispPriority == 2) && IS_R300_VARIANT) {
	uint32_t mc_init_misc_lat_timer = INREG(R300_MC_INIT_MISC_LAT_TIMER);
	mc_init_misc_lat_timer &= ~(R300_MC_DISP1R_INIT_LAT_MASK << R300_MC_DISP1R_INIT_LAT_SHIFT);
	mc_init_misc_lat_timer &= ~(R300_MC_DISP0R_INIT_LAT_MASK << R300_MC_DISP0R_INIT_LAT_SHIFT);
	if (pRADEONEnt->pCrtc[1]->enabled)
	    mc_init_misc_lat_timer |= (1 << R300_MC_DISP1R_INIT_LAT_SHIFT); /* display 1 */
	if (pRADEONEnt->pCrtc[0]->enabled)
	    mc_init_misc_lat_timer |= (1 << R300_MC_DISP0R_INIT_LAT_SHIFT); /* display 0 */
	OUTREG(R300_MC_INIT_MISC_LAT_TIMER, mc_init_misc_lat_timer);
    }

    /*
     * Determine if there is enough bandwidth for current display mode
     */
    mem_bw = info->mclk * (info->RamWidth / 8) * (info->IsDDR ? 2 : 1);

    pix_clk = 0;
    pix_clk2 = 0;
    peak_disp_bw = 0;
    if (mode1) {
	pix_clk = mode1->Clock/1000.0;
	peak_disp_bw += (pix_clk * pixel_bytes1);
    }
    if (mode2) {
	pix_clk2 = mode2->Clock/1000.0;
	peak_disp_bw += (pix_clk2 * pixel_bytes2);
    }

    if (peak_disp_bw >= mem_bw * min_mem_eff) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "You may not have enough display bandwidth for current mode\n"
		   "If you have flickering problem, try to lower resolution, refresh rate, or color depth\n");
    }

    /*  Get values from the EXT_MEM_CNTL register...converting its contents. */
    temp = INREG(RADEON_MEM_TIMING_CNTL);
    if ((info->ChipFamily == CHIP_FAMILY_RV100) || info->IsIGP) { /* RV100, M6, IGPs */
	mem_trcd = ((temp >> 2) & 0x3) + 1;
	mem_trp  = ((temp & 0x3)) + 1;
	mem_tras = ((temp & 0x70) >> 4) + 1;
    } else if (info->ChipFamily == CHIP_FAMILY_R300 ||
	       info->ChipFamily == CHIP_FAMILY_R350) { /* r300, r350 */
	mem_trcd = (temp & 0x7) + 1;
	mem_trp = ((temp >> 8) & 0x7) + 1;
	mem_tras = ((temp >> 11) & 0xf) + 4;
    } else if (info->ChipFamily == CHIP_FAMILY_RV350 ||
	       info->ChipFamily <= CHIP_FAMILY_RV380) {
	/* rv3x0 */
	mem_trcd = (temp & 0x7) + 3;
	mem_trp = ((temp >> 8) & 0x7) + 3;
	mem_tras = ((temp >> 11) & 0xf) + 6;
    } else if (info->ChipFamily == CHIP_FAMILY_R420 ||
	       info->ChipFamily == CHIP_FAMILY_RV410) {
	/* r4xx */
	mem_trcd = (temp & 0xf) + 3;
	if (mem_trcd > 15)
	    mem_trcd = 15;
	mem_trp = ((temp >> 8) & 0xf) + 3;
	if (mem_trp > 15)
	    mem_trp = 15;
	mem_tras = ((temp >> 12) & 0x1f) + 6;
	if (mem_tras > 31)
	    mem_tras = 31;
    } else { /* RV200, R200 */
	mem_trcd = (temp & 0x7) + 1;
	mem_trp = ((temp >> 8) & 0x7) + 1;
	mem_tras = ((temp >> 12) & 0xf) + 4;
    }

    /* Get values from the MEM_SDRAM_MODE_REG register...converting its */
    temp = INREG(RADEON_MEM_SDRAM_MODE_REG);
    data = (temp & (7<<20)) >> 20;
    if ((info->ChipFamily == CHIP_FAMILY_RV100) || info->IsIGP) { /* RV100, M6, IGPs */
	if (info->ChipFamily == CHIP_FAMILY_RS480) /* don't think rs400 */
	    mem_tcas = MemTcas_rs480[data];
	else
	    mem_tcas = MemTcas[data];
    } else {
	mem_tcas = MemTcas2 [data];
    }
    if (info->ChipFamily == CHIP_FAMILY_RS400 ||
	info->ChipFamily == CHIP_FAMILY_RS480) {
	/* extra cas latency stored in bits 23-25 0-4 clocks */
	data = (temp >> 23) & 0x7;
	if (data < 5)
	    mem_tcas += data;
    }

    if (IS_R300_VARIANT && !info->IsIGP) {
	/* on the R300, Tcas is included in Trbs.
	 */
	temp = INREG(RADEON_MEM_CNTL);
	data = (R300_MEM_NUM_CHANNELS_MASK & temp);
	if (data == 1) {
	    if (R300_MEM_USE_CD_CH_ONLY & temp) {
		temp  = INREG(R300_MC_IND_INDEX);
		temp &= ~R300_MC_IND_ADDR_MASK;
		temp |= R300_MC_READ_CNTL_CD_mcind;
		OUTREG(R300_MC_IND_INDEX, temp);
		temp  = INREG(R300_MC_IND_DATA);
		data = (R300_MEM_RBS_POSITION_C_MASK & temp);
	    } else {
		temp = INREG(R300_MC_READ_CNTL_AB);
		data = (R300_MEM_RBS_POSITION_A_MASK & temp);
	    }
	} else {
	    temp = INREG(R300_MC_READ_CNTL_AB);
	    data = (R300_MEM_RBS_POSITION_A_MASK & temp);
	}

	if (info->ChipFamily == CHIP_FAMILY_RV410 ||
	    info->ChipFamily == CHIP_FAMILY_R420)
	    mem_trbs = MemTrbs_r4xx[data];
	else
	    mem_trbs = MemTrbs[data];
	mem_tcas += mem_trbs;
    }

    if ((info->ChipFamily == CHIP_FAMILY_RV100) || info->IsIGP) { /* RV100, M6, IGPs */
	/* DDR64 SCLK_EFF = SCLK for analysis */
	sclk_eff = info->sclk;
    } else {
#ifdef XF86DRI
	if (info->directRenderingEnabled)
	    sclk_eff = info->sclk - (info->dri->agpMode * 50.0 / 3.0);
	else
#endif
	    sclk_eff = info->sclk;
    }

    /*
      Find the memory controller latency for the display client.
    */
    if (IS_R300_VARIANT) {
	/*not enough for R350 ???*/
	/*
	if (!mode2) sclk_delay = 150;
	else {
	    if (info->RamWidth == 256) sclk_delay = 87;
	    else sclk_delay = 97;
	}
	*/
	sclk_delay = 250;
    } else {
	if ((info->ChipFamily == CHIP_FAMILY_RV100) ||
	    info->IsIGP) {
	    if (info->IsDDR) sclk_delay = 41;
	    else sclk_delay = 33;
	} else {
	    if (info->RamWidth == 128) sclk_delay = 57;
	    else sclk_delay = 41;
	}
    }

    mc_latency_sclk = sclk_delay / sclk_eff;

    if (info->IsDDR) {
	if (info->RamWidth == 32) {
	    k1 = 40;
	    c  = 3;
	} else {
	    k1 = 20;
	    c  = 1;
	}
    } else {
	k1 = 40;
	c  = 3;
    }
    mc_latency_mclk = ((2.0*mem_trcd + mem_tcas*c + 4.0*mem_tras + 4.0*mem_trp + k1) /
		       info->mclk) + (4.0 / sclk_eff);

    /*
      HW cursor time assuming worst case of full size colour cursor.
    */
    cur_latency_mclk = (mem_trp + MAX(mem_tras, (mem_trcd + 2*(cur_size - (info->IsDDR+1))))) / info->mclk;
    cur_latency_sclk = cur_size / sclk_eff;

    /*
      Find the total latency for the display data.
    */
    disp_latency_overhead = 8.0 / info->sclk;
    mc_latency_mclk = mc_latency_mclk + disp_latency_overhead + cur_latency_mclk;
    mc_latency_sclk = mc_latency_sclk + disp_latency_overhead + cur_latency_sclk;
    disp_latency = MAX(mc_latency_mclk, mc_latency_sclk);

    /* setup Max GRPH_STOP_REQ default value */
    if (IS_RV100_VARIANT)
	max_stop_req = 0x5c;
    else
	max_stop_req  = 0x7c;

    if (mode1) {
	/*  CRTC1
	    Set GRPH_BUFFER_CNTL register using h/w defined optimal values.
	    GRPH_STOP_REQ <= MIN[ 0x7C, (CRTC_H_DISP + 1) * (bit depth) / 0x10 ]
	*/
	stop_req = mode1->HDisplay * pixel_bytes1 / 16;

	if (stop_req > max_stop_req)
	    stop_req = max_stop_req;

	/*
	  Find the drain rate of the display buffer.
	*/
	disp_drain_rate = pix_clk / (16.0/pixel_bytes1);

	/*
	  Find the critical point of the display buffer.
	*/
	critical_point= (uint32_t)(disp_drain_rate * disp_latency + 0.5);

	/* ???? */
	/*
	  temp = (info->SavedReg.grph_buffer_cntl & RADEON_GRPH_CRITICAL_POINT_MASK) >> RADEON_GRPH_CRITICAL_POINT_SHIFT;
	  if (critical_point < temp) critical_point = temp;
	*/
	if (info->DispPriority == 2) {
	    critical_point = 0;
	}

	/*
	  The critical point should never be above max_stop_req-4.  Setting
	  GRPH_CRITICAL_CNTL = 0 will thus force high priority all the time.
	*/
	if (max_stop_req - critical_point < 4) critical_point = 0;

	if (critical_point == 0 && mode2 && info->ChipFamily == CHIP_FAMILY_R300) {
	    /* some R300 cards have problem with this set to 0, when CRTC2 is enabled.*/
	    critical_point = 0x10;
	}

	temp = info->SavedReg->grph_buffer_cntl;
	temp &= ~(RADEON_GRPH_STOP_REQ_MASK);
	temp |= (stop_req << RADEON_GRPH_STOP_REQ_SHIFT);
	temp &= ~(RADEON_GRPH_START_REQ_MASK);
	if ((info->ChipFamily == CHIP_FAMILY_R350) &&
	    (stop_req > 0x15)) {
	    stop_req -= 0x10;
	}
	temp |= (stop_req << RADEON_GRPH_START_REQ_SHIFT);

	temp |= RADEON_GRPH_BUFFER_SIZE;
	temp &= ~(RADEON_GRPH_CRITICAL_CNTL   |
		  RADEON_GRPH_CRITICAL_AT_SOF |
		  RADEON_GRPH_STOP_CNTL);
	/*
	  Write the result into the register.
	*/
	OUTREG(RADEON_GRPH_BUFFER_CNTL, ((temp & ~RADEON_GRPH_CRITICAL_POINT_MASK) |
					 (critical_point << RADEON_GRPH_CRITICAL_POINT_SHIFT)));

#if 0
	if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
	    (info->ChipFamily == CHIP_FAMILY_RS480)) {
	    /* attempt to program RS400 disp regs correctly ??? */
	    temp = info->SavedReg->disp1_req_cntl1;
	    temp &= ~(RS400_DISP1_START_REQ_LEVEL_MASK |
		      RS400_DISP1_STOP_REQ_LEVEL_MASK);
	    OUTREG(RS400_DISP1_REQ_CNTL1, (temp |
					   (critical_point << RS400_DISP1_START_REQ_LEVEL_SHIFT) |
					   (critical_point << RS400_DISP1_STOP_REQ_LEVEL_SHIFT)));
	    temp = info->SavedReg->dmif_mem_cntl1;
	    temp &= ~(RS400_DISP1_CRITICAL_POINT_START_MASK |
		      RS400_DISP1_CRITICAL_POINT_STOP_MASK);
	    OUTREG(RS400_DMIF_MEM_CNTL1, (temp |
					  (critical_point << RS400_DISP1_CRITICAL_POINT_START_SHIFT) |
					  (critical_point << RS400_DISP1_CRITICAL_POINT_STOP_SHIFT)));
	}
#endif

	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "GRPH_BUFFER_CNTL from %x to %x\n",
		       (unsigned int)info->SavedReg->grph_buffer_cntl,
		       (unsigned int)INREG(RADEON_GRPH_BUFFER_CNTL));
    }

    if (mode2) {
	stop_req = mode2->HDisplay * pixel_bytes2 / 16;

	if (stop_req > max_stop_req) stop_req = max_stop_req;

	/*
	  Find the drain rate of the display buffer.
	*/
	disp_drain_rate2 = pix_clk2 / (16.0/pixel_bytes2);

	temp = info->SavedReg->grph2_buffer_cntl;
	temp &= ~(RADEON_GRPH_STOP_REQ_MASK);
	temp |= (stop_req << RADEON_GRPH_STOP_REQ_SHIFT);
	temp &= ~(RADEON_GRPH_START_REQ_MASK);
	if ((info->ChipFamily == CHIP_FAMILY_R350) &&
	    (stop_req > 0x15)) {
	    stop_req -= 0x10;
	}
	temp |= (stop_req << RADEON_GRPH_START_REQ_SHIFT);
	temp |= RADEON_GRPH_BUFFER_SIZE;
	temp &= ~(RADEON_GRPH_CRITICAL_CNTL   |
		  RADEON_GRPH_CRITICAL_AT_SOF |
		  RADEON_GRPH_STOP_CNTL);

	if ((info->ChipFamily == CHIP_FAMILY_RS100) ||
	    (info->ChipFamily == CHIP_FAMILY_RS200))
	    critical_point2 = 0;
	else {
	    read_return_rate = MIN(info->sclk, info->mclk*(info->RamWidth*(info->IsDDR+1)/128));
	    if (mode1)
		time_disp1_drop_priority = critical_point / (read_return_rate - disp_drain_rate);
	    else
		time_disp1_drop_priority = 0;

	    critical_point2 = (uint32_t)((disp_latency + time_disp1_drop_priority + 
					disp_latency) * disp_drain_rate2 + 0.5);

	    if (info->DispPriority == 2) {
		critical_point2 = 0;
	    }

	    if (max_stop_req - critical_point2 < 4) critical_point2 = 0;

	}

	if (critical_point2 == 0 && info->ChipFamily == CHIP_FAMILY_R300) {
	    /* some R300 cards have problem with this set to 0 */
	    critical_point2 = 0x10;
	}

	OUTREG(RADEON_GRPH2_BUFFER_CNTL, ((temp & ~RADEON_GRPH_CRITICAL_POINT_MASK) |
					  (critical_point2 << RADEON_GRPH_CRITICAL_POINT_SHIFT)));

	if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
	    (info->ChipFamily == CHIP_FAMILY_RS480)) {
#if 0
	    /* attempt to program RS400 disp2 regs correctly ??? */
	    temp = info->SavedReg->disp2_req_cntl1;
	    temp &= ~(RS400_DISP2_START_REQ_LEVEL_MASK |
		      RS400_DISP2_STOP_REQ_LEVEL_MASK);
	    OUTREG(RS400_DISP2_REQ_CNTL1, (temp |
					   (critical_point2 << RS400_DISP1_START_REQ_LEVEL_SHIFT) |
					   (critical_point2 << RS400_DISP1_STOP_REQ_LEVEL_SHIFT)));
	    temp = info->SavedReg->disp2_req_cntl2;
	    temp &= ~(RS400_DISP2_CRITICAL_POINT_START_MASK |
		      RS400_DISP2_CRITICAL_POINT_STOP_MASK);
	    OUTREG(RS400_DISP2_REQ_CNTL2, (temp |
					   (critical_point2 << RS400_DISP2_CRITICAL_POINT_START_SHIFT) |
					   (critical_point2 << RS400_DISP2_CRITICAL_POINT_STOP_SHIFT)));
#endif
	    OUTREG(RS400_DISP2_REQ_CNTL1, 0x105DC1CC);
	    OUTREG(RS400_DISP2_REQ_CNTL2, 0x2749D000);
	    OUTREG(RS400_DMIF_MEM_CNTL1,  0x29CA71DC);
	    OUTREG(RS400_DISP1_REQ_CNTL1, 0x28FBC3AC);
	}

	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "GRPH2_BUFFER_CNTL from %x to %x\n",
		       (unsigned int)info->SavedReg->grph2_buffer_cntl,
		       (unsigned int)INREG(RADEON_GRPH2_BUFFER_CNTL));
    }
}

void
legacy_crtc_mode_set(xf86CrtcPtr crtc, DisplayModePtr mode,
		     DisplayModePtr adjusted_mode, int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int i = 0;
    double dot_clock = 0;
    int pll_flags = RADEON_PLL_LEGACY;
    Bool update_tv_routing = FALSE;
    Bool tilingChanged = FALSE;

    if (adjusted_mode->Clock > 200000) /* range limits??? */
	pll_flags |= RADEON_PLL_PREFER_HIGH_FB_DIV;
    else
	pll_flags |= RADEON_PLL_PREFER_LOW_REF_DIV;

    if (info->allowColorTiling) {
	radeon_crtc->can_tile = (adjusted_mode->Flags & (V_DBLSCAN | V_INTERLACE)) ? FALSE : TRUE;
	tilingChanged = RADEONSetTiling(pScrn);
    }

    for (i = 0; i < xf86_config->num_output; i++) {
	xf86OutputPtr output = xf86_config->output[i];
	RADEONOutputPrivatePtr radeon_output = output->driver_private;

	if (output->crtc == crtc) {
	    if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT |
						ATOM_DEVICE_DFP_SUPPORT))
		pll_flags |= RADEON_PLL_NO_ODD_POST_DIV;
	    if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT))
		pll_flags |= (RADEON_PLL_USE_BIOS_DIVS | RADEON_PLL_USE_REF_DIV);
	}
    }


    ErrorF("init memmap\n");
    RADEONInitMemMapRegisters(pScrn, info->ModeReg, info);
    ErrorF("init common\n");
    RADEONInitCommonRegisters(info->ModeReg, info);

    RADEONInitSurfaceCntl(crtc, info->ModeReg);

    switch (radeon_crtc->crtc_id) {
    case 0:
	ErrorF("init crtc1\n");
	RADEONInitCrtcRegisters(crtc, info->ModeReg, adjusted_mode);
	RADEONInitCrtcBase(crtc, info->ModeReg, x, y);
	dot_clock = adjusted_mode->Clock / 1000.0;
	if (dot_clock) {
	    ErrorF("init pll1\n");
	    RADEONInitPLLRegisters(crtc, info->ModeReg, &info->pll, adjusted_mode, pll_flags);
	} else {
	    info->ModeReg->ppll_ref_div = info->SavedReg->ppll_ref_div;
	    info->ModeReg->ppll_div_3   = info->SavedReg->ppll_div_3;
	    info->ModeReg->htotal_cntl  = info->SavedReg->htotal_cntl;
	}
	break;
    case 1:
	ErrorF("init crtc2\n");
	RADEONInitCrtc2Registers(crtc, info->ModeReg, adjusted_mode);
	RADEONInitCrtc2Base(crtc, info->ModeReg, x, y);
	dot_clock = adjusted_mode->Clock / 1000.0;
	if (dot_clock) {
	    ErrorF("init pll2\n");
	    RADEONInitPLL2Registers(crtc, info->ModeReg, &info->pll, adjusted_mode, pll_flags);
	}
	break;
    }

    for (i = 0; i < xf86_config->num_output; i++) {
	xf86OutputPtr output = xf86_config->output[i];
	RADEONOutputPrivatePtr radeon_output = output->driver_private;

	if (output->crtc == crtc) {
	    if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT)) {
		switch (radeon_crtc->crtc_id) {
		case 0:
		    RADEONAdjustCrtcRegistersForTV(pScrn, info->ModeReg, adjusted_mode, output);
		    RADEONAdjustPLLRegistersForTV(pScrn, info->ModeReg, adjusted_mode, output);
		    update_tv_routing = TRUE;
		    break;
		case 1:
		    RADEONAdjustCrtc2RegistersForTV(pScrn, info->ModeReg, adjusted_mode, output);
		    RADEONAdjustPLL2RegistersForTV(pScrn, info->ModeReg, adjusted_mode, output);
		    break;
		}
	    }
	}
    }

    ErrorF("restore memmap\n");
    RADEONRestoreMemMapRegisters(pScrn, info->ModeReg);
    ErrorF("restore common\n");
    RADEONRestoreCommonRegisters(pScrn, info->ModeReg);

    switch (radeon_crtc->crtc_id) {
    case 0:
	ErrorF("restore crtc1\n");
	RADEONRestoreCrtcRegisters(pScrn, info->ModeReg);
	ErrorF("restore pll1\n");
	RADEONRestorePLLRegisters(pScrn, info->ModeReg);
	break;
    case 1:
	ErrorF("restore crtc2\n");
	RADEONRestoreCrtc2Registers(pScrn, info->ModeReg);
	ErrorF("restore pll2\n");
	RADEONRestorePLL2Registers(pScrn, info->ModeReg);
	break;
    }

    /* pixclks_cntl handles tv-out clock routing */
    if (update_tv_routing)
	radeon_update_tv_routing(pScrn, info->ModeReg);

    if (info->DispPriority)
	RADEONInitDispBandwidth(pScrn);

    radeon_crtc->initialized = TRUE;

    if (tilingChanged) {
	/* need to redraw front buffer, I guess this can be considered a hack ? */
	/* if this is called during ScreenInit() we don't have pScrn->pScreen yet */
	if (pScrn->pScreen)
	    xf86EnableDisableFBAccess(XF86_ENABLEDISABLEFB_ARG(pScrn), FALSE);
	RADEONChangeSurfaces(pScrn);
	if (pScrn->pScreen)
	    xf86EnableDisableFBAccess(XF86_ENABLEDISABLEFB_ARG(pScrn), TRUE);
	/* xf86SetRootClip would do, but can't access that here */
    }

    /* reset ecp_div for Xv */
    info->ecp_div = -1;

}

