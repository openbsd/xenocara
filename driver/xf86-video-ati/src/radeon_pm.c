/*
 * Copyright 2009 Advanced Micro Devices, Inc.
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
 * NON-INFRINGEMENT.  IN NO EVENT SHALL THE COPYRIGHT HOLDER(S) OR
 * AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Author: Alex Deucher <alexander.deucher@amd.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

				/* Driver data structures */
#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_atombios.h"

#include "ati_pciids_gen.h"

/* 10 khz */
static uint32_t calc_eng_mem_clock(ScrnInfoPtr pScrn,
				   uint32_t req_clock,
				   int ref_div,
				   int *fb_div,
				   int *post_div)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONPLLPtr pll = &info->pll;

    if (req_clock < 15000) {
	*post_div = 8;
	req_clock *= 8;
    } else if (req_clock < 30000) {
	*post_div = 4;
	req_clock *= 4;
    } else if (req_clock < 60000) {
	*post_div = 2;
	req_clock *= 2;
    } else
	*post_div = 1;

    req_clock *= ref_div;
    req_clock += pll->reference_freq;
    req_clock /= (2 * pll->reference_freq);

    *fb_div = req_clock & 0xff;

    req_clock = (req_clock & 0xffff) << 1;
    req_clock *= pll->reference_freq;
    req_clock /= ref_div;
    req_clock /= *post_div;

    return req_clock;

}

static void
RADEONSetEngineClock(ScrnInfoPtr pScrn, uint32_t eng_clock)
{
    uint32_t tmp;
    int ref_div, fb_div, post_div;

    RADEONWaitForIdleMMIO(pScrn);

    tmp = INPLL(pScrn, RADEON_M_SPLL_REF_FB_DIV);
    ref_div = tmp & RADEON_M_SPLL_REF_DIV_MASK;

    eng_clock = calc_eng_mem_clock(pScrn, eng_clock, ref_div, &fb_div, &post_div);

    tmp = INPLL(pScrn, RADEON_CLK_PIN_CNTL);
    tmp &= ~RADEON_DONT_USE_XTALIN;
    OUTPLL(pScrn, RADEON_CLK_PIN_CNTL, tmp);

    tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
    tmp &= ~RADEON_SCLK_SRC_SEL_MASK;
    OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);

    usleep(10);

    tmp = INPLL(pScrn, RADEON_SPLL_CNTL);
    tmp |= RADEON_SPLL_SLEEP;
    OUTPLL(pScrn, RADEON_SPLL_CNTL, tmp);

    usleep(2);

    tmp = INPLL(pScrn, RADEON_SPLL_CNTL);
    tmp |= RADEON_SPLL_RESET;
    OUTPLL(pScrn, RADEON_SPLL_CNTL, tmp);

    usleep(200);

    tmp = INPLL(pScrn, RADEON_M_SPLL_REF_FB_DIV);
    tmp &= ~(RADEON_SPLL_FB_DIV_MASK << RADEON_SPLL_FB_DIV_SHIFT);
    tmp |= (fb_div & RADEON_SPLL_FB_DIV_MASK) << RADEON_SPLL_FB_DIV_SHIFT;
    OUTPLL(pScrn, RADEON_M_SPLL_REF_FB_DIV, tmp);

    /* XXX: verify on different asics */
    tmp = INPLL(pScrn, RADEON_SPLL_CNTL);
    tmp &= ~RADEON_SPLL_PVG_MASK;
    if ((eng_clock * post_div) >= 90000)
	tmp |= (0x7 << RADEON_SPLL_PVG_SHIFT);
    else
	tmp |= (0x4 << RADEON_SPLL_PVG_SHIFT);
    OUTPLL(pScrn, RADEON_SPLL_CNTL, tmp);

    tmp = INPLL(pScrn, RADEON_SPLL_CNTL);
    tmp &= ~RADEON_SPLL_SLEEP;
    OUTPLL(pScrn, RADEON_SPLL_CNTL, tmp);

    usleep(2);

    tmp = INPLL(pScrn, RADEON_SPLL_CNTL);
    tmp &= ~RADEON_SPLL_RESET;
    OUTPLL(pScrn, RADEON_SPLL_CNTL, tmp);

    usleep(200);

    tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
    tmp &= ~RADEON_SCLK_SRC_SEL_MASK;
    switch (post_div) {
    case 1:
    default:
	tmp |= 1;
	break;
    case 2:
	tmp |= 2;
	break;
    case 4:
	tmp |= 3;
	break;
    case 8:
	tmp |= 4;
	break;
    }
    OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);

    usleep(20);

    tmp = INPLL(pScrn, RADEON_CLK_PIN_CNTL);
    tmp |= RADEON_DONT_USE_XTALIN;
    OUTPLL(pScrn, RADEON_CLK_PIN_CNTL, tmp);

    usleep(10);

}

static void LegacySetClockGating(ScrnInfoPtr pScrn, Bool enable)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t tmp;

    if (enable) {
	if (!pRADEONEnt->HasCRTC2) {
	    tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
	    if ((INREG(RADEON_CONFIG_CNTL) & RADEON_CFG_ATI_REV_ID_MASK) >
		RADEON_CFG_ATI_REV_A13) {
		tmp &= ~(RADEON_SCLK_FORCE_CP | RADEON_SCLK_FORCE_RB);
	    }
	    tmp &= ~(RADEON_SCLK_FORCE_HDP  | RADEON_SCLK_FORCE_DISP1 |
		     RADEON_SCLK_FORCE_TOP  | RADEON_SCLK_FORCE_SE   |
		     RADEON_SCLK_FORCE_IDCT | RADEON_SCLK_FORCE_RE   |
		     RADEON_SCLK_FORCE_PB   | RADEON_SCLK_FORCE_TAM  |
		     RADEON_SCLK_FORCE_TDM);
	    OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);
	} else if (IS_R300_VARIANT) {
	    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
		(info->ChipFamily == CHIP_FAMILY_RS480)) {
		tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
		tmp &= ~(RADEON_SCLK_FORCE_DISP2 | RADEON_SCLK_FORCE_CP      |
			 RADEON_SCLK_FORCE_HDP   | RADEON_SCLK_FORCE_DISP1   |
			 RADEON_SCLK_FORCE_TOP   | RADEON_SCLK_FORCE_E2      |
			 R300_SCLK_FORCE_VAP     | RADEON_SCLK_FORCE_IDCT    |
			 RADEON_SCLK_FORCE_VIP   | R300_SCLK_FORCE_SR        |
			 R300_SCLK_FORCE_PX      | R300_SCLK_FORCE_TX        |
			 R300_SCLK_FORCE_US      | RADEON_SCLK_FORCE_TV_SCLK |
			 R300_SCLK_FORCE_SU      | RADEON_SCLK_FORCE_OV0);
		tmp |=  RADEON_DYN_STOP_LAT_MASK;
		tmp |= RADEON_SCLK_FORCE_TOP | RADEON_SCLK_FORCE_VIP;
		OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);

		tmp = INPLL(pScrn, RADEON_SCLK_MORE_CNTL);
		tmp &= ~RADEON_SCLK_MORE_FORCEON;
		tmp |=  RADEON_SCLK_MORE_MAX_DYN_STOP_LAT;
		OUTPLL(pScrn, RADEON_SCLK_MORE_CNTL, tmp);

		tmp = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);
		tmp |= (RADEON_PIXCLK_ALWAYS_ONb |
			RADEON_PIXCLK_DAC_ALWAYS_ONb);
		OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, tmp);

		tmp = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
		tmp |= (RADEON_PIX2CLK_ALWAYS_ONb         |
			RADEON_PIX2CLK_DAC_ALWAYS_ONb     |
			RADEON_DISP_TVOUT_PIXCLK_TV_ALWAYS_ONb |
			R300_DVOCLK_ALWAYS_ONb            |
			RADEON_PIXCLK_BLEND_ALWAYS_ONb    |
			RADEON_PIXCLK_GV_ALWAYS_ONb       |
			R300_PIXCLK_DVO_ALWAYS_ONb        |
			RADEON_PIXCLK_LVDS_ALWAYS_ONb     |
			RADEON_PIXCLK_TMDS_ALWAYS_ONb     |
			R300_PIXCLK_TRANS_ALWAYS_ONb      |
			R300_PIXCLK_TVO_ALWAYS_ONb        |
			R300_P2G2CLK_ALWAYS_ONb           |
			R300_P2G2CLK_DAC_ALWAYS_ONb);
		OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmp);
	    } else if (info->ChipFamily >= CHIP_FAMILY_RV350) {
		tmp = INPLL(pScrn, R300_SCLK_CNTL2);
		tmp &= ~(R300_SCLK_FORCE_TCL |
			 R300_SCLK_FORCE_GA  |
			 R300_SCLK_FORCE_CBA);
		tmp |=  (R300_SCLK_TCL_MAX_DYN_STOP_LAT |
			 R300_SCLK_GA_MAX_DYN_STOP_LAT  |
			 R300_SCLK_CBA_MAX_DYN_STOP_LAT);
		OUTPLL(pScrn, R300_SCLK_CNTL2, tmp);

		tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
		tmp &= ~(RADEON_SCLK_FORCE_DISP2 | RADEON_SCLK_FORCE_CP      |
			 RADEON_SCLK_FORCE_HDP   | RADEON_SCLK_FORCE_DISP1   |
			 RADEON_SCLK_FORCE_TOP   | RADEON_SCLK_FORCE_E2      |
			 R300_SCLK_FORCE_VAP     | RADEON_SCLK_FORCE_IDCT    |
			 RADEON_SCLK_FORCE_VIP   | R300_SCLK_FORCE_SR        |
			 R300_SCLK_FORCE_PX      | R300_SCLK_FORCE_TX        |
			 R300_SCLK_FORCE_US      | RADEON_SCLK_FORCE_TV_SCLK |
			 R300_SCLK_FORCE_SU      | RADEON_SCLK_FORCE_OV0);
		tmp |=  RADEON_DYN_STOP_LAT_MASK;
		OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);

		tmp = INPLL(pScrn, RADEON_SCLK_MORE_CNTL);
		tmp &= ~RADEON_SCLK_MORE_FORCEON;
		tmp |=  RADEON_SCLK_MORE_MAX_DYN_STOP_LAT;
		OUTPLL(pScrn, RADEON_SCLK_MORE_CNTL, tmp);

		tmp = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);
		tmp |= (RADEON_PIXCLK_ALWAYS_ONb |
			RADEON_PIXCLK_DAC_ALWAYS_ONb);
		OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, tmp);

		tmp = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
		tmp |= (RADEON_PIX2CLK_ALWAYS_ONb         |
			RADEON_PIX2CLK_DAC_ALWAYS_ONb     |
			RADEON_DISP_TVOUT_PIXCLK_TV_ALWAYS_ONb |
			R300_DVOCLK_ALWAYS_ONb            |
			RADEON_PIXCLK_BLEND_ALWAYS_ONb    |
			RADEON_PIXCLK_GV_ALWAYS_ONb       |
			R300_PIXCLK_DVO_ALWAYS_ONb        |
			RADEON_PIXCLK_LVDS_ALWAYS_ONb     |
			RADEON_PIXCLK_TMDS_ALWAYS_ONb     |
			R300_PIXCLK_TRANS_ALWAYS_ONb      |
			R300_PIXCLK_TVO_ALWAYS_ONb        |
			R300_P2G2CLK_ALWAYS_ONb           |
			R300_P2G2CLK_DAC_ALWAYS_ONb);
		OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmp);

		tmp = INPLL(pScrn, RADEON_MCLK_MISC);
		tmp |= (RADEON_MC_MCLK_DYN_ENABLE |
			RADEON_IO_MCLK_DYN_ENABLE);
		OUTPLL(pScrn, RADEON_MCLK_MISC, tmp);

		tmp = INPLL(pScrn, RADEON_MCLK_CNTL);
		tmp |= (RADEON_FORCEON_MCLKA |
			RADEON_FORCEON_MCLKB);

		tmp &= ~(RADEON_FORCEON_YCLKA  |
			 RADEON_FORCEON_YCLKB  |
			 RADEON_FORCEON_MC);

		/* Some releases of vbios have set DISABLE_MC_MCLKA
		   and DISABLE_MC_MCLKB bits in the vbios table.  Setting these
		   bits will cause H/W hang when reading video memory with dynamic clocking
		   enabled. */
		if ((tmp & R300_DISABLE_MC_MCLKA) &&
		    (tmp & R300_DISABLE_MC_MCLKB)) {
		    /* If both bits are set, then check the active channels */
		    tmp = INPLL(pScrn, RADEON_MCLK_CNTL);
		    if (info->RamWidth == 64) {
			if (INREG(RADEON_MEM_CNTL) & R300_MEM_USE_CD_CH_ONLY)
			    tmp &= ~R300_DISABLE_MC_MCLKB;
			else
			    tmp &= ~R300_DISABLE_MC_MCLKA;
		    } else {
			tmp &= ~(R300_DISABLE_MC_MCLKA |
				 R300_DISABLE_MC_MCLKB);
		    }
		}

		OUTPLL(pScrn, RADEON_MCLK_CNTL, tmp);
	    } else {
		tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
		tmp &= ~(R300_SCLK_FORCE_VAP);
		tmp |= RADEON_SCLK_FORCE_CP;
		OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);
		usleep(15000);

		tmp = INPLL(pScrn, R300_SCLK_CNTL2);
		tmp &= ~(R300_SCLK_FORCE_TCL |
			 R300_SCLK_FORCE_GA  |
			 R300_SCLK_FORCE_CBA);
		OUTPLL(pScrn, R300_SCLK_CNTL2, tmp);
	    }
	} else {
	    tmp = INPLL(pScrn, RADEON_CLK_PWRMGT_CNTL);

	    tmp &= ~(RADEON_ACTIVE_HILO_LAT_MASK     |
		     RADEON_DISP_DYN_STOP_LAT_MASK   |
		     RADEON_DYN_STOP_MODE_MASK);

	    tmp |= (RADEON_ENGIN_DYNCLK_MODE |
		    (0x01 << RADEON_ACTIVE_HILO_LAT_SHIFT));
	    OUTPLL(pScrn, RADEON_CLK_PWRMGT_CNTL, tmp);
	    usleep(15000);

	    tmp = INPLL(pScrn, RADEON_CLK_PIN_CNTL);
	    tmp |= RADEON_SCLK_DYN_START_CNTL;
	    OUTPLL(pScrn, RADEON_CLK_PIN_CNTL, tmp);
	    usleep(15000);

	    /* When DRI is enabled, setting DYN_STOP_LAT to zero can cause some R200
	       to lockup randomly, leave them as set by BIOS.
	    */
	    tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
	    /*tmp &= RADEON_SCLK_SRC_SEL_MASK;*/
	    tmp &= ~RADEON_SCLK_FORCEON_MASK;

	    /*RAGE_6::A11 A12 A12N1 A13, RV250::A11 A12, R300*/
	    if (((info->ChipFamily == CHIP_FAMILY_RV250) &&
		 ((INREG(RADEON_CONFIG_CNTL) & RADEON_CFG_ATI_REV_ID_MASK) <
		  RADEON_CFG_ATI_REV_A13)) ||
		((info->ChipFamily == CHIP_FAMILY_RV100) &&
		 ((INREG(RADEON_CONFIG_CNTL) & RADEON_CFG_ATI_REV_ID_MASK) <=
		  RADEON_CFG_ATI_REV_A13))) {
		tmp |= RADEON_SCLK_FORCE_CP;
		tmp |= RADEON_SCLK_FORCE_VIP;
	    }

	    OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);

	    if ((info->ChipFamily == CHIP_FAMILY_RV200) ||
		(info->ChipFamily == CHIP_FAMILY_RV250) ||
		(info->ChipFamily == CHIP_FAMILY_RV280)) {
		tmp = INPLL(pScrn, RADEON_SCLK_MORE_CNTL);
		tmp &= ~RADEON_SCLK_MORE_FORCEON;

		/* RV200::A11 A12 RV250::A11 A12 */
		if (((info->ChipFamily == CHIP_FAMILY_RV200) ||
		     (info->ChipFamily == CHIP_FAMILY_RV250)) &&
		    ((INREG(RADEON_CONFIG_CNTL) & RADEON_CFG_ATI_REV_ID_MASK) <
		     RADEON_CFG_ATI_REV_A13)) {
		    tmp |= RADEON_SCLK_MORE_FORCEON;
		}
		OUTPLL(pScrn, RADEON_SCLK_MORE_CNTL, tmp);
		usleep(15000);
	    }

	    /* RV200::A11 A12, RV250::A11 A12 */
	    if (((info->ChipFamily == CHIP_FAMILY_RV200) ||
		 (info->ChipFamily == CHIP_FAMILY_RV250)) &&
		((INREG(RADEON_CONFIG_CNTL) & RADEON_CFG_ATI_REV_ID_MASK) <
		 RADEON_CFG_ATI_REV_A13)) {
		tmp = INPLL(pScrn, RADEON_PLL_PWRMGT_CNTL);
		tmp |= RADEON_TCL_BYPASS_DISABLE;
		OUTPLL(pScrn, RADEON_PLL_PWRMGT_CNTL, tmp);
	    }
	    usleep(15000);

	    /*enable dynamic mode for display clocks (PIXCLK and PIX2CLK)*/
	    tmp = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
	    tmp |=  (RADEON_PIX2CLK_ALWAYS_ONb         |
		     RADEON_PIX2CLK_DAC_ALWAYS_ONb     |
		     RADEON_PIXCLK_BLEND_ALWAYS_ONb    |
		     RADEON_PIXCLK_GV_ALWAYS_ONb       |
		     RADEON_PIXCLK_DIG_TMDS_ALWAYS_ONb |
		     RADEON_PIXCLK_LVDS_ALWAYS_ONb     |
		     RADEON_PIXCLK_TMDS_ALWAYS_ONb);

	    OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmp);
	    usleep(15000);

	    tmp = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);
	    tmp |= (RADEON_PIXCLK_ALWAYS_ONb  |
		    RADEON_PIXCLK_DAC_ALWAYS_ONb);

	    OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, tmp);
	    usleep(15000);
	}
    } else {
	/* Turn everything OFF (ForceON to everything)*/
	if ( !pRADEONEnt->HasCRTC2 ) {
	    tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
	    tmp |= (RADEON_SCLK_FORCE_CP   | RADEON_SCLK_FORCE_HDP |
		    RADEON_SCLK_FORCE_DISP1 | RADEON_SCLK_FORCE_TOP |
		    RADEON_SCLK_FORCE_E2   | RADEON_SCLK_FORCE_SE  |
		    RADEON_SCLK_FORCE_IDCT | RADEON_SCLK_FORCE_VIP |
		    RADEON_SCLK_FORCE_RE   | RADEON_SCLK_FORCE_PB  |
		    RADEON_SCLK_FORCE_TAM  | RADEON_SCLK_FORCE_TDM |
		    RADEON_SCLK_FORCE_RB);
	    OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);
	} else if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
		   (info->ChipFamily == CHIP_FAMILY_RS480)) {
	    tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
	    tmp |= (RADEON_SCLK_FORCE_DISP2 | RADEON_SCLK_FORCE_CP      |
		    RADEON_SCLK_FORCE_HDP   | RADEON_SCLK_FORCE_DISP1   |
		    RADEON_SCLK_FORCE_TOP   | RADEON_SCLK_FORCE_E2      |
		    R300_SCLK_FORCE_VAP     | RADEON_SCLK_FORCE_IDCT    |
		    RADEON_SCLK_FORCE_VIP   | R300_SCLK_FORCE_SR        |
		    R300_SCLK_FORCE_PX      | R300_SCLK_FORCE_TX        |
		    R300_SCLK_FORCE_US      | RADEON_SCLK_FORCE_TV_SCLK |
		    R300_SCLK_FORCE_SU      | RADEON_SCLK_FORCE_OV0);
	    OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);

	    tmp = INPLL(pScrn, RADEON_SCLK_MORE_CNTL);
	    tmp |= RADEON_SCLK_MORE_FORCEON;
	    OUTPLL(pScrn, RADEON_SCLK_MORE_CNTL, tmp);

	    tmp = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);
	    tmp &= ~(RADEON_PIXCLK_ALWAYS_ONb  |
		     RADEON_PIXCLK_DAC_ALWAYS_ONb |
		     R300_DISP_DAC_PIXCLK_DAC_BLANK_OFF);
	    OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, tmp);

	    tmp = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
	    tmp &= ~(RADEON_PIX2CLK_ALWAYS_ONb         |
		     RADEON_PIX2CLK_DAC_ALWAYS_ONb     |
		     RADEON_DISP_TVOUT_PIXCLK_TV_ALWAYS_ONb |
		     R300_DVOCLK_ALWAYS_ONb            |
		     RADEON_PIXCLK_BLEND_ALWAYS_ONb    |
		     RADEON_PIXCLK_GV_ALWAYS_ONb       |
		     R300_PIXCLK_DVO_ALWAYS_ONb        |
		     RADEON_PIXCLK_LVDS_ALWAYS_ONb     |
		     RADEON_PIXCLK_TMDS_ALWAYS_ONb     |
		     R300_PIXCLK_TRANS_ALWAYS_ONb      |
		     R300_PIXCLK_TVO_ALWAYS_ONb        |
		     R300_P2G2CLK_ALWAYS_ONb           |
		     R300_P2G2CLK_DAC_ALWAYS_ONb       |
		     R300_DISP_DAC_PIXCLK_DAC2_BLANK_OFF);
	    OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmp);
	} else if (info->ChipFamily >= CHIP_FAMILY_RV350) {
	    /* for RV350/M10, no delays are required. */
	    tmp = INPLL(pScrn, R300_SCLK_CNTL2);
	    tmp |= (R300_SCLK_FORCE_TCL |
		    R300_SCLK_FORCE_GA  |
		    R300_SCLK_FORCE_CBA);
	    OUTPLL(pScrn, R300_SCLK_CNTL2, tmp);

	    tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
	    tmp |= (RADEON_SCLK_FORCE_DISP2 | RADEON_SCLK_FORCE_CP      |
		    RADEON_SCLK_FORCE_HDP   | RADEON_SCLK_FORCE_DISP1   |
		    RADEON_SCLK_FORCE_TOP   | RADEON_SCLK_FORCE_E2      |
		    R300_SCLK_FORCE_VAP     | RADEON_SCLK_FORCE_IDCT    |
		    RADEON_SCLK_FORCE_VIP   | R300_SCLK_FORCE_SR        |
		    R300_SCLK_FORCE_PX      | R300_SCLK_FORCE_TX        |
		    R300_SCLK_FORCE_US      | RADEON_SCLK_FORCE_TV_SCLK |
		    R300_SCLK_FORCE_SU      | RADEON_SCLK_FORCE_OV0);
	    OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);

	    tmp = INPLL(pScrn, RADEON_SCLK_MORE_CNTL);
	    tmp |= RADEON_SCLK_MORE_FORCEON;
	    OUTPLL(pScrn, RADEON_SCLK_MORE_CNTL, tmp);

	    tmp = INPLL(pScrn, RADEON_MCLK_CNTL);
	    tmp |= (RADEON_FORCEON_MCLKA |
		    RADEON_FORCEON_MCLKB |
		    RADEON_FORCEON_YCLKA |
		    RADEON_FORCEON_YCLKB |
		    RADEON_FORCEON_MC);
	    OUTPLL(pScrn, RADEON_MCLK_CNTL, tmp);

	    tmp = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);
	    tmp &= ~(RADEON_PIXCLK_ALWAYS_ONb  |
		     RADEON_PIXCLK_DAC_ALWAYS_ONb |
		     R300_DISP_DAC_PIXCLK_DAC_BLANK_OFF);
	    OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, tmp);

	    tmp = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
	    tmp &= ~(RADEON_PIX2CLK_ALWAYS_ONb         |
		     RADEON_PIX2CLK_DAC_ALWAYS_ONb     |
		     RADEON_DISP_TVOUT_PIXCLK_TV_ALWAYS_ONb |
		     R300_DVOCLK_ALWAYS_ONb            |
		     RADEON_PIXCLK_BLEND_ALWAYS_ONb    |
		     RADEON_PIXCLK_GV_ALWAYS_ONb       |
		     R300_PIXCLK_DVO_ALWAYS_ONb        |
		     RADEON_PIXCLK_LVDS_ALWAYS_ONb     |
		     RADEON_PIXCLK_TMDS_ALWAYS_ONb     |
		     R300_PIXCLK_TRANS_ALWAYS_ONb      |
		     R300_PIXCLK_TVO_ALWAYS_ONb        |
		     R300_P2G2CLK_ALWAYS_ONb           |
		     R300_P2G2CLK_DAC_ALWAYS_ONb       |
		     R300_DISP_DAC_PIXCLK_DAC2_BLANK_OFF);
	    OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmp);
	}  else {
	    tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
	    tmp |= (RADEON_SCLK_FORCE_CP | RADEON_SCLK_FORCE_E2);
	    tmp |= RADEON_SCLK_FORCE_SE;

	    if ( !pRADEONEnt->HasCRTC2 ) {
		tmp |= ( RADEON_SCLK_FORCE_RB    |
			 RADEON_SCLK_FORCE_TDM   |
			 RADEON_SCLK_FORCE_TAM   |
			 RADEON_SCLK_FORCE_PB    |
			 RADEON_SCLK_FORCE_RE    |
			 RADEON_SCLK_FORCE_VIP   |
			 RADEON_SCLK_FORCE_IDCT  |
			 RADEON_SCLK_FORCE_TOP   |
			 RADEON_SCLK_FORCE_DISP1 |
			 RADEON_SCLK_FORCE_DISP2 |
			 RADEON_SCLK_FORCE_HDP    );
	    } else if ((info->ChipFamily == CHIP_FAMILY_R300) ||
		       (info->ChipFamily == CHIP_FAMILY_R350)) {
		tmp |= ( RADEON_SCLK_FORCE_HDP   |
			 RADEON_SCLK_FORCE_DISP1 |
			 RADEON_SCLK_FORCE_DISP2 |
			 RADEON_SCLK_FORCE_TOP   |
			 RADEON_SCLK_FORCE_IDCT  |
			 RADEON_SCLK_FORCE_VIP);
	    }
	    OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);

	    usleep(16000);

	    if ((info->ChipFamily == CHIP_FAMILY_R300) ||
		(info->ChipFamily == CHIP_FAMILY_R350)) {
		tmp = INPLL(pScrn, R300_SCLK_CNTL2);
		tmp |= ( R300_SCLK_FORCE_TCL |
			 R300_SCLK_FORCE_GA  |
			 R300_SCLK_FORCE_CBA);
		OUTPLL(pScrn, R300_SCLK_CNTL2, tmp);
		usleep(16000);
	    }

	    if (info->IsIGP) {
		tmp = INPLL(pScrn, RADEON_MCLK_CNTL);
		tmp &= ~(RADEON_FORCEON_MCLKA |
			 RADEON_FORCEON_YCLKA);
		OUTPLL(pScrn, RADEON_MCLK_CNTL, tmp);
		usleep(16000);
	    }

	    if ((info->ChipFamily == CHIP_FAMILY_RV200) ||
		(info->ChipFamily == CHIP_FAMILY_RV250) ||
		(info->ChipFamily == CHIP_FAMILY_RV280)) {
		tmp = INPLL(pScrn, RADEON_SCLK_MORE_CNTL);
		tmp |= RADEON_SCLK_MORE_FORCEON;
		OUTPLL(pScrn, RADEON_SCLK_MORE_CNTL, tmp);
		usleep(16000);
	    }

	    tmp = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
	    tmp &= ~(RADEON_PIX2CLK_ALWAYS_ONb         |
		     RADEON_PIX2CLK_DAC_ALWAYS_ONb     |
		     RADEON_PIXCLK_BLEND_ALWAYS_ONb    |
		     RADEON_PIXCLK_GV_ALWAYS_ONb       |
		     RADEON_PIXCLK_DIG_TMDS_ALWAYS_ONb |
		     RADEON_PIXCLK_LVDS_ALWAYS_ONb     |
		     RADEON_PIXCLK_TMDS_ALWAYS_ONb);

	    OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmp);
	    usleep(16000);

	    tmp = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);
	    tmp &= ~(RADEON_PIXCLK_ALWAYS_ONb  |
		     RADEON_PIXCLK_DAC_ALWAYS_ONb);
	    OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, tmp);
	}
    }
}

static void RADEONPMQuirks(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    uint32_t tmp;

    RADEONWaitForIdleMMIO(pScrn);

    if (info->ChipFamily < CHIP_FAMILY_RV515) {
	tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
	if (IS_R300_VARIANT || IS_RV100_VARIANT)
	    tmp |= RADEON_SCLK_FORCE_CP | RADEON_SCLK_FORCE_VIP;
	if ((info->ChipFamily == CHIP_FAMILY_RV250) || (info->ChipFamily == CHIP_FAMILY_RV280))
	    tmp |= RADEON_SCLK_FORCE_DISP1 | RADEON_SCLK_FORCE_DISP2;
	if ((info->ChipFamily == CHIP_FAMILY_RV350) || (info->ChipFamily == CHIP_FAMILY_RV380))
	    tmp |= R300_SCLK_FORCE_VAP;
	if (info->ChipFamily == CHIP_FAMILY_R420)
	    tmp |= R300_SCLK_FORCE_PX | R300_SCLK_FORCE_TX;
	OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);
    } else if (info->ChipFamily < CHIP_FAMILY_R600) {
	tmp = INPLL(pScrn, AVIVO_CP_DYN_CNTL);
	tmp |= AVIVO_CP_FORCEON;
	OUTPLL(pScrn, AVIVO_CP_DYN_CNTL, tmp);

	tmp = INPLL(pScrn, AVIVO_E2_DYN_CNTL);
	tmp |= AVIVO_E2_FORCEON;
	OUTPLL(pScrn, AVIVO_E2_DYN_CNTL, tmp);

	tmp = INPLL(pScrn, AVIVO_IDCT_DYN_CNTL);
	tmp |= AVIVO_IDCT_FORCEON;
	OUTPLL(pScrn, AVIVO_IDCT_DYN_CNTL, tmp);
    }
}

static void
RADEONSetPCIELanes(ScrnInfoPtr pScrn, int lanes)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t link_width_cntl, mask, target_reg;

    if (info->IsIGP)
	return;

    /* don't change lanes on multi-gpu cards for now */
    if ((info->Chipset == PCI_CHIP_RV770_9441) ||
	(info->Chipset == PCI_CHIP_RV770_9443) ||
	(info->Chipset == PCI_CHIP_RV770_944B) ||
	(info->Chipset == PCI_CHIP_RV670_9506) ||
	(info->Chipset == PCI_CHIP_RV670_9509) ||
	(info->Chipset == PCI_CHIP_RV670_950F))
	return;

    RADEONWaitForIdleMMIO(pScrn);

    switch (lanes) {
    case 0:
	mask = RADEON_PCIE_LC_LINK_WIDTH_X0;
	break;
    case 1:
	mask = RADEON_PCIE_LC_LINK_WIDTH_X1;
	break;
    case 2:
	mask = RADEON_PCIE_LC_LINK_WIDTH_X2;
	break;
    case 4:
	mask = RADEON_PCIE_LC_LINK_WIDTH_X4;
	break;
    case 8:
	mask = RADEON_PCIE_LC_LINK_WIDTH_X8;
	break;
    case 12:
	mask = RADEON_PCIE_LC_LINK_WIDTH_X12;
	break;
    case 16:
    default:
	mask = RADEON_PCIE_LC_LINK_WIDTH_X16;
	break;
    }

    if (info->ChipFamily >= CHIP_FAMILY_R600) {
	link_width_cntl = INPCIE_P(pScrn, RADEON_PCIE_LC_LINK_WIDTH_CNTL);

	if ((link_width_cntl & RADEON_PCIE_LC_LINK_WIDTH_RD_MASK) ==
	    (mask << RADEON_PCIE_LC_LINK_WIDTH_RD_SHIFT))
	    return;

	link_width_cntl &= ~(RADEON_PCIE_LC_LINK_WIDTH_MASK |
			     RADEON_PCIE_LC_RECONFIG_NOW |
			     R600_PCIE_LC_RECONFIG_ARC_MISSING_ESCAPE |
			     R600_PCIE_LC_SHORT_RECONFIG_EN |
			     R600_PCIE_LC_RENEGOTIATE_EN);
	link_width_cntl |= mask;

#if 0
	/* some northbridges can renegotiate the link rather than requiring
	 * a complete re-config.
	 * e.g., AMD 780/790 northbridges (pci ids: 0x5956, 0x5957, 0x5958, etc.)
	 */
	if (northbridge can renegotiate)
	    link_width_cntl |= R600_PCIE_LC_RENEGOTIATE_EN;
	else
#endif
	    link_width_cntl |= R600_PCIE_LC_RECONFIG_ARC_MISSING_ESCAPE;

	OUTPCIE_P(pScrn, RADEON_PCIE_LC_LINK_WIDTH_CNTL, link_width_cntl);
	OUTPCIE_P(pScrn, RADEON_PCIE_LC_LINK_WIDTH_CNTL, link_width_cntl | RADEON_PCIE_LC_RECONFIG_NOW);

	if (info->ChipFamily >= CHIP_FAMILY_RV770)
	    target_reg = R700_TARGET_AND_CURRENT_PROFILE_INDEX;
	else
	    target_reg = R600_TARGET_AND_CURRENT_PROFILE_INDEX;

	/* wait for lane set to complete */
	link_width_cntl = INREG(target_reg);
	while (link_width_cntl == 0xffffffff)
	    link_width_cntl = INREG(target_reg);

    } else {
	link_width_cntl = INPCIE(pScrn, RADEON_PCIE_LC_LINK_WIDTH_CNTL);

	if ((link_width_cntl & RADEON_PCIE_LC_LINK_WIDTH_RD_MASK) ==
	    (mask << RADEON_PCIE_LC_LINK_WIDTH_RD_SHIFT))
	    return;

	link_width_cntl &= ~(RADEON_PCIE_LC_LINK_WIDTH_MASK |
			     RADEON_PCIE_LC_RECONFIG_NOW |
			     RADEON_PCIE_LC_RECONFIG_LATER |
			     RADEON_PCIE_LC_SHORT_RECONFIG_EN);
	link_width_cntl |= mask;
	OUTPCIE(pScrn, RADEON_PCIE_LC_LINK_WIDTH_CNTL, link_width_cntl);
	OUTPCIE(pScrn, RADEON_PCIE_LC_LINK_WIDTH_CNTL, link_width_cntl | RADEON_PCIE_LC_RECONFIG_NOW);

	/* wait for lane set to complete */
	link_width_cntl = INPCIE(pScrn, RADEON_PCIE_LC_LINK_WIDTH_CNTL);
	while (link_width_cntl == 0xffffffff)
	    link_width_cntl = INPCIE(pScrn, RADEON_PCIE_LC_LINK_WIDTH_CNTL);

    }

}

static void
RADEONSetClockGating(ScrnInfoPtr pScrn, Bool enable)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);

    RADEONWaitForIdleMMIO(pScrn);

    if (info->ChipFamily >= CHIP_FAMILY_R600)
	atombios_static_pwrmgt_setup(pScrn, enable);
    else {
	if (info->IsAtomBios) {
	    atombios_static_pwrmgt_setup(pScrn, enable);
	    atombios_clk_gating_setup(pScrn, enable);
	} else if (info->IsMobility)
	    LegacySetClockGating(pScrn, enable);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Dynamic Clock Gating %sabled\n",
	       enable ? "En" : "Dis");
}

static void RADEONSetStaticPowerMode(ScrnInfoPtr pScrn, RADEONPMType type)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    int i;

    for (i = 0; i < info->pm.num_modes; i++) {
	if (info->pm.mode[i].type == type)
	    break;
    }

    if (i == info->pm.num_modes)
	return;

    if (i == info->pm.current_mode)
	return;

    RADEONWaitForIdleMMIO(pScrn);

    if (info->IsAtomBios)
	atombios_set_engine_clock(pScrn, info->pm.mode[i].sclk);
    else
	RADEONSetEngineClock(pScrn, info->pm.mode[i].sclk);

    if (info->cardType == CARD_PCIE)
	RADEONSetPCIELanes(pScrn, info->pm.mode[i].pcie_lanes);

    info->pm.current_mode = i;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Power Mode Switch\n");
}


void RADEONPMInit(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);

    if (xf86ReturnOptValBool(info->Options, OPTION_CLOCK_GATING, FALSE)) {
	info->pm.clock_gating_enabled = TRUE;
	RADEONSetClockGating(pScrn, info->pm.clock_gating_enabled);
    } else
	info->pm.clock_gating_enabled = FALSE;

    info->pm.mode[0].type = POWER_DEFAULT;
    info->pm.mode[0].sclk = (uint32_t)info->sclk * 100; /* 10 khz */
    info->pm.mode[0].mclk = (uint32_t)info->mclk * 100; /* 10 khz */
    info->pm.mode[0].pcie_lanes = 16; /* XXX: read back current lane config */
    info->pm.current_mode = 0;
    info->pm.num_modes = 1;

    if (xf86ReturnOptValBool(info->Options, OPTION_DYNAMIC_PM, FALSE)) {
	info->pm.dynamic_mode_enabled = TRUE;
	info->pm.mode[1].type = POWER_LOW;
	info->pm.mode[1].sclk = info->pm.mode[0].sclk / 4;
	info->pm.mode[1].mclk = info->pm.mode[0].mclk / 4;
	info->pm.mode[1].pcie_lanes = 1;

	info->pm.mode[2].type = POWER_HIGH;
	info->pm.mode[2].sclk = info->pm.mode[0].sclk;
	info->pm.mode[2].mclk = info->pm.mode[0].mclk;
	info->pm.mode[2].pcie_lanes = 16;

	info->pm.num_modes += 2;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Dynamic Power Management Enabled\n");
    } else {
	info->pm.dynamic_mode_enabled = FALSE;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Dynamic Power Management Disabled\n");
    }

    if (xf86ReturnOptValBool(info->Options, OPTION_FORCE_LOW_POWER, FALSE)) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Force Low Power Mode Enabled\n");
	info->pm.force_low_power_enabled = TRUE;
	if (info->pm.dynamic_mode_enabled) {
	    info->pm.mode[2].type = POWER_HIGH;
	    info->pm.mode[2].sclk = info->pm.mode[0].sclk / 2;
	    info->pm.mode[2].mclk = info->pm.mode[0].mclk / 2;
	    info->pm.mode[2].pcie_lanes = 4;
	} else {
	    info->pm.mode[1].type = POWER_HIGH;
	    info->pm.mode[1].sclk = info->pm.mode[0].sclk / 2;
	    info->pm.mode[1].mclk = info->pm.mode[0].mclk / 2;
	    info->pm.mode[1].pcie_lanes = 4;
	    info->pm.num_modes += 1;
	}
	RADEONSetStaticPowerMode(pScrn, POWER_HIGH);
    } else
	info->pm.force_low_power_enabled = FALSE;

    RADEONPMQuirks(pScrn);
}

void RADEONPMEnterVT(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);

    if (info->pm.clock_gating_enabled)
	RADEONSetClockGating(pScrn, info->pm.clock_gating_enabled);
    RADEONPMQuirks(pScrn);
    if (info->pm.force_low_power_enabled || info->pm.dynamic_mode_enabled)
	RADEONSetStaticPowerMode(pScrn, POWER_HIGH);
}

void RADEONPMLeaveVT(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);

    if (info->pm.clock_gating_enabled)
	RADEONSetClockGating(pScrn, FALSE);
    if (info->pm.force_low_power_enabled || info->pm.dynamic_mode_enabled)
	RADEONSetStaticPowerMode(pScrn, POWER_DEFAULT);
}

void RADEONPMFini(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    if (info->pm.clock_gating_enabled)
	RADEONSetClockGating(pScrn, FALSE);
    if (info->pm.force_low_power_enabled || info->pm.dynamic_mode_enabled)
	RADEONSetStaticPowerMode(pScrn, POWER_DEFAULT);
}

void RADEONPMBlockHandler(ScrnInfoPtr pScrn)
{
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

    if ((!pRADEONEnt->Controller[0]->enabled) &&
	(!pRADEONEnt->Controller[1]->enabled))
	RADEONSetStaticPowerMode(pScrn, POWER_LOW);
    else
	RADEONSetStaticPowerMode(pScrn, POWER_HIGH);

}

