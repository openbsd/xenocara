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
#include "radeon_tv.h"
#include "radeon_atombios.h"

static RADEONMonitorType radeon_detect_tv(ScrnInfoPtr pScrn);
static RADEONMonitorType radeon_detect_primary_dac(ScrnInfoPtr pScrn, Bool color);
static RADEONMonitorType radeon_detect_tv_dac(ScrnInfoPtr pScrn, Bool color);
static RADEONMonitorType radeon_detect_ext_dac(ScrnInfoPtr pScrn);

void
RADEONRestoreDACRegisters(ScrnInfoPtr pScrn,
			  RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (IS_R300_VARIANT)
	OUTREGP(RADEON_GPIOPAD_A, restore->gpiopad_a, ~1);

    OUTREGP(RADEON_DAC_CNTL,
	    restore->dac_cntl,
	    RADEON_DAC_RANGE_CNTL |
	    RADEON_DAC_BLANKING);

    OUTREG(RADEON_DAC_CNTL2, restore->dac2_cntl);

    if ((info->ChipFamily != CHIP_FAMILY_RADEON) &&
	(info->ChipFamily != CHIP_FAMILY_R200))
    OUTREG (RADEON_TV_DAC_CNTL, restore->tv_dac_cntl);

    OUTREG(RADEON_DISP_OUTPUT_CNTL, restore->disp_output_cntl);

    if ((info->ChipFamily == CHIP_FAMILY_R200) ||
	IS_R300_VARIANT) {
	OUTREG(RADEON_DISP_TV_OUT_CNTL, restore->disp_tv_out_cntl);
    } else {
	OUTREG(RADEON_DISP_HW_DEBUG, restore->disp_hw_debug);
    }

    OUTREG(RADEON_DAC_MACRO_CNTL, restore->dac_macro_cntl);

    /* R200 DAC connected via DVO */
    if (info->ChipFamily == CHIP_FAMILY_R200)
	OUTREG(RADEON_FP2_GEN_CNTL, restore->fp2_gen_cntl);
}


/* Write TMDS registers */
void
RADEONRestoreFPRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG(RADEON_TMDS_PLL_CNTL,        restore->tmds_pll_cntl);
    OUTREG(RADEON_TMDS_TRANSMITTER_CNTL,restore->tmds_transmitter_cntl);
    OUTREG(RADEON_FP_GEN_CNTL,          restore->fp_gen_cntl);

    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480)) {
	OUTREG(RS400_FP_2ND_GEN_CNTL, restore->fp_2nd_gen_cntl);
	/*OUTREG(RS400_TMDS2_CNTL, restore->tmds2_cntl);*/
	OUTREG(RS400_TMDS2_TRANSMITTER_CNTL, restore->tmds2_transmitter_cntl);
    }

    /* old AIW Radeon has some BIOS initialization problem
     * with display buffer underflow, only occurs to DFP
     */
    if (!pRADEONEnt->HasCRTC2)
	OUTREG(RADEON_GRPH_BUFFER_CNTL,
	       INREG(RADEON_GRPH_BUFFER_CNTL) & ~0x7f0000);

}

/* Write FP2 registers */
void
RADEONRestoreFP2Registers(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG(RADEON_FP2_GEN_CNTL,         restore->fp2_gen_cntl);

    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480))
	OUTREG(RS400_FP2_2_GEN_CNTL, restore->fp2_2_gen_cntl);
}

/* Write RMX registers */
void
RADEONRestoreRMXRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG(RADEON_FP_HORZ_STRETCH,      restore->fp_horz_stretch);
    OUTREG(RADEON_FP_VERT_STRETCH,      restore->fp_vert_stretch);
    OUTREG(RADEON_CRTC_MORE_CNTL,       restore->crtc_more_cntl);
    OUTREG(RADEON_FP_HORZ_VERT_ACTIVE,  restore->fp_horz_vert_active);
    OUTREG(RADEON_FP_H_SYNC_STRT_WID,   restore->fp_h_sync_strt_wid);
    OUTREG(RADEON_FP_V_SYNC_STRT_WID,   restore->fp_v_sync_strt_wid);
    OUTREG(RADEON_FP_CRTC_H_TOTAL_DISP, restore->fp_crtc_h_total_disp);
    OUTREG(RADEON_FP_CRTC_V_TOTAL_DISP, restore->fp_crtc_v_total_disp);

}

/* Write LVDS registers */
void
RADEONRestoreLVDSRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (info->IsMobility) {
	OUTREG(RADEON_LVDS_GEN_CNTL,  restore->lvds_gen_cntl);
	/*OUTREG(RADEON_LVDS_PLL_CNTL,  restore->lvds_pll_cntl);*/

	if (info->ChipFamily == CHIP_FAMILY_RV410) {
	    OUTREG(RADEON_CLOCK_CNTL_INDEX, 0);
	}
    }

}

void
RADEONSaveDACRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    save->dac_cntl              = INREG(RADEON_DAC_CNTL);
    save->dac2_cntl             = INREG(RADEON_DAC_CNTL2);
    save->tv_dac_cntl           = INREG(RADEON_TV_DAC_CNTL);
    save->disp_output_cntl      = INREG(RADEON_DISP_OUTPUT_CNTL);
    save->disp_tv_out_cntl      = INREG(RADEON_DISP_TV_OUT_CNTL);
    save->disp_hw_debug         = INREG(RADEON_DISP_HW_DEBUG);
    save->dac_macro_cntl        = INREG(RADEON_DAC_MACRO_CNTL);
    save->gpiopad_a             = INREG(RADEON_GPIOPAD_A);

}

/* Read flat panel registers */
void
RADEONSaveFPRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    save->fp_gen_cntl          = INREG(RADEON_FP_GEN_CNTL);
    save->fp2_gen_cntl          = INREG (RADEON_FP2_GEN_CNTL);
    save->fp_horz_stretch      = INREG(RADEON_FP_HORZ_STRETCH);
    save->fp_vert_stretch      = INREG(RADEON_FP_VERT_STRETCH);
    save->fp_horz_vert_active  = INREG(RADEON_FP_HORZ_VERT_ACTIVE);
    save->crtc_more_cntl       = INREG(RADEON_CRTC_MORE_CNTL);
    save->lvds_gen_cntl        = INREG(RADEON_LVDS_GEN_CNTL);
    save->lvds_pll_cntl        = INREG(RADEON_LVDS_PLL_CNTL);
    save->tmds_pll_cntl        = INREG(RADEON_TMDS_PLL_CNTL);
    save->tmds_transmitter_cntl= INREG(RADEON_TMDS_TRANSMITTER_CNTL);

    save->fp_h_sync_strt_wid   = INREG(RADEON_FP_H_SYNC_STRT_WID);
    save->fp_v_sync_strt_wid   = INREG(RADEON_FP_V_SYNC_STRT_WID);
    save->fp_crtc_h_total_disp = INREG(RADEON_FP_CRTC_H_TOTAL_DISP);
    save->fp_crtc_v_total_disp = INREG(RADEON_FP_CRTC_V_TOTAL_DISP);

    if (info->ChipFamily == CHIP_FAMILY_RV280) {
	/* bit 22 of TMDS_PLL_CNTL is read-back inverted */
	save->tmds_pll_cntl ^= (1 << 22);
    }

    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480)) {
	save->fp_2nd_gen_cntl         = INREG(RS400_FP_2ND_GEN_CNTL);
	save->fp2_2_gen_cntl          = INREG(RS400_FP2_2_GEN_CNTL);
	save->tmds2_cntl              = INREG(RS400_TMDS2_CNTL);
	save->tmds2_transmitter_cntl  = INREG(RS400_TMDS2_TRANSMITTER_CNTL);
    }

}

Bool
RADEONDVOReadByte(I2CDevPtr dvo, int addr, uint8_t *ch)
{
    if (!xf86I2CReadByte(dvo, addr, ch)) {
	xf86DrvMsg(dvo->pI2CBus->scrnIndex, X_ERROR,
		   "Unable to read from %s Slave %d.\n",
		   dvo->pI2CBus->BusName, dvo->SlaveAddr);
	return FALSE;
    }
    return TRUE;
}

Bool
RADEONDVOWriteByte(I2CDevPtr dvo, int addr, uint8_t ch)
{
    if (!xf86I2CWriteByte(dvo, addr, ch)) {
	xf86DrvMsg(dvo->pI2CBus->scrnIndex, X_ERROR,
		   "Unable to write to %s Slave %d.\n",
		   dvo->pI2CBus->BusName, dvo->SlaveAddr);
	return FALSE;
    }
    return TRUE;
}

I2CDevPtr
RADEONDVODeviceInit(I2CBusPtr b, I2CSlaveAddr addr)
{
    I2CDevPtr dvo;

    dvo = xcalloc(1, sizeof(I2CDevRec));
    if (dvo == NULL)
	return NULL;

    dvo->DevName = "RADEON DVO Controller";
    dvo->SlaveAddr = addr;
    dvo->pI2CBus = b;
    dvo->StartTimeout = b->StartTimeout;
    dvo->BitTimeout = b->BitTimeout;
    dvo->AcknTimeout = b->AcknTimeout;
    dvo->ByteTimeout = b->ByteTimeout;

    if (xf86I2CDevInit(dvo)) {
	return dvo;
    }

    xfree(dvo);
    return NULL;
}

static void
RADEONRestoreDVOChip(ScrnInfoPtr pScrn, xf86OutputPtr output)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if (!radeon_output->DVOChip)
	return;

    OUTREG(radeon_output->dvo_i2c.mask_clk_reg,
	   INREG(radeon_output->dvo_i2c.mask_clk_reg) &
	   (uint32_t)~(RADEON_GPIO_A_0 | RADEON_GPIO_A_1));

    if (!RADEONInitExtTMDSInfoFromBIOS(output)) {
	if (radeon_output->DVOChip) {
	    switch(info->ext_tmds_chip) {
	    case RADEON_SIL_164:
		RADEONDVOWriteByte(radeon_output->DVOChip, 0x08, 0x30);
		RADEONDVOWriteByte(radeon_output->DVOChip, 0x09, 0x00);
		RADEONDVOWriteByte(radeon_output->DVOChip, 0x0a, 0x90);
		RADEONDVOWriteByte(radeon_output->DVOChip, 0x0c, 0x89);
		RADEONDVOWriteByte(radeon_output->DVOChip, 0x08, 0x3b);
		break;
#if 0
		/* needs work see bug 10418 */
	    case RADEON_SIL_1178:
		RADEONDVOWriteByte(radeon_output->DVOChip, 0x0f, 0x44);
		RADEONDVOWriteByte(radeon_output->DVOChip, 0x0f, 0x4c);
		RADEONDVOWriteByte(radeon_output->DVOChip, 0x0e, 0x01);
		RADEONDVOWriteByte(radeon_output->DVOChip, 0x0a, 0x80);
                RADEONDVOWriteByte(radeon_output->DVOChip, 0x09, 0x30);
                RADEONDVOWriteByte(radeon_output->DVOChip, 0x0c, 0xc9);
                RADEONDVOWriteByte(radeon_output->DVOChip, 0x0d, 0x70);
                RADEONDVOWriteByte(radeon_output->DVOChip, 0x08, 0x32);
                RADEONDVOWriteByte(radeon_output->DVOChip, 0x08, 0x33);
		break;
#endif
	    default:
		break;
	    }
	}
    }
}

#if 0
static RADEONMonitorType
RADEONCrtIsPhysicallyConnected(ScrnInfoPtr pScrn, int IsCrtDac)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int		  bConnected = 0;

    /* the monitor either wasn't connected or it is a non-DDC CRT.
     * try to probe it
     */
    if(IsCrtDac) {
	unsigned long ulOrigVCLK_ECP_CNTL;
	unsigned long ulOrigDAC_CNTL;
	unsigned long ulOrigDAC_MACRO_CNTL;
	unsigned long ulOrigDAC_EXT_CNTL;
	unsigned long ulOrigCRTC_EXT_CNTL;
	unsigned long ulData;
	unsigned long ulMask;

	ulOrigVCLK_ECP_CNTL = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);

	ulData              = ulOrigVCLK_ECP_CNTL;
	ulData             &= ~(RADEON_PIXCLK_ALWAYS_ONb
				| RADEON_PIXCLK_DAC_ALWAYS_ONb);
	ulMask              = ~(RADEON_PIXCLK_ALWAYS_ONb
				|RADEON_PIXCLK_DAC_ALWAYS_ONb);
	OUTPLLP(pScrn, RADEON_VCLK_ECP_CNTL, ulData, ulMask);

	ulOrigCRTC_EXT_CNTL = INREG(RADEON_CRTC_EXT_CNTL);
	ulData              = ulOrigCRTC_EXT_CNTL;
	ulData             |= RADEON_CRTC_CRT_ON;
	OUTREG(RADEON_CRTC_EXT_CNTL, ulData);

	ulOrigDAC_EXT_CNTL = INREG(RADEON_DAC_EXT_CNTL);
	ulData             = ulOrigDAC_EXT_CNTL;
	ulData            &= ~RADEON_DAC_FORCE_DATA_MASK;
	ulData            |=  (RADEON_DAC_FORCE_BLANK_OFF_EN
			       |RADEON_DAC_FORCE_DATA_EN
			       |RADEON_DAC_FORCE_DATA_SEL_MASK);
	if ((info->ChipFamily == CHIP_FAMILY_RV250) ||
	    (info->ChipFamily == CHIP_FAMILY_RV280))
	    ulData |= (0x01b6 << RADEON_DAC_FORCE_DATA_SHIFT);
	else
	    ulData |= (0x01ac << RADEON_DAC_FORCE_DATA_SHIFT);

	OUTREG(RADEON_DAC_EXT_CNTL, ulData);

	/* turn on power so testing can go through */
	ulOrigDAC_CNTL = INREG(RADEON_DAC_CNTL);
	ulOrigDAC_CNTL &= ~RADEON_DAC_PDWN;
	OUTREG(RADEON_DAC_CNTL, ulOrigDAC_CNTL);

	ulOrigDAC_MACRO_CNTL = INREG(RADEON_DAC_MACRO_CNTL);
	ulOrigDAC_MACRO_CNTL &= ~(RADEON_DAC_PDWN_R | RADEON_DAC_PDWN_G |
				  RADEON_DAC_PDWN_B);
	OUTREG(RADEON_DAC_MACRO_CNTL, ulOrigDAC_MACRO_CNTL);

	/* Enable comparators and set DAC range to PS2 (VGA) output level */
	ulData = ulOrigDAC_CNTL;
	ulData |= RADEON_DAC_CMP_EN;
	ulData &= ~RADEON_DAC_RANGE_CNTL_MASK;
	ulData |= 0x2;
	OUTREG(RADEON_DAC_CNTL, ulData);

	/* Settle down */
	usleep(10000);

	/* Read comparators */
	ulData     = INREG(RADEON_DAC_CNTL);
	bConnected =  (RADEON_DAC_CMP_OUTPUT & ulData)?1:0;

	/* Restore things */
	ulData    = ulOrigVCLK_ECP_CNTL;
	ulMask    = 0xFFFFFFFFL;
	OUTPLLP(pScrn, RADEON_VCLK_ECP_CNTL, ulData, ulMask);

	OUTREG(RADEON_DAC_CNTL,      ulOrigDAC_CNTL     );
	OUTREG(RADEON_DAC_EXT_CNTL,  ulOrigDAC_EXT_CNTL );
	OUTREG(RADEON_CRTC_EXT_CNTL, ulOrigCRTC_EXT_CNTL);

	if (!bConnected) {
	    /* Power DAC down if CRT is not connected */
            ulOrigDAC_MACRO_CNTL = INREG(RADEON_DAC_MACRO_CNTL);
            ulOrigDAC_MACRO_CNTL |= (RADEON_DAC_PDWN_R | RADEON_DAC_PDWN_G |
	    	RADEON_DAC_PDWN_B);
            OUTREG(RADEON_DAC_MACRO_CNTL, ulOrigDAC_MACRO_CNTL);

	    ulData = INREG(RADEON_DAC_CNTL);
	    ulData |= RADEON_DAC_PDWN;
	    OUTREG(RADEON_DAC_CNTL, ulData);
    	}
    } else { /* TV DAC */

        /* This doesn't seem to work reliably (maybe worse on some OEM cards),
           for now we always return false. If one wants to connected a
           non-DDC monitor on the DVI port when CRT port is also connected,
           he will need to explicitly tell the driver in the config file
           with Option MonitorLayout.
        */
        bConnected = FALSE;

#if 0
	if (info->ChipFamily == CHIP_FAMILY_R200) {
	    unsigned long ulOrigGPIO_MONID;
	    unsigned long ulOrigFP2_GEN_CNTL;
	    unsigned long ulOrigDISP_OUTPUT_CNTL;
	    unsigned long ulOrigCRTC2_GEN_CNTL;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_A;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_B;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_C;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_D;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_E;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_F;
	    unsigned long ulOrigCRTC2_H_TOTAL_DISP;
	    unsigned long ulOrigCRTC2_V_TOTAL_DISP;
	    unsigned long ulOrigCRTC2_H_SYNC_STRT_WID;
	    unsigned long ulOrigCRTC2_V_SYNC_STRT_WID;
	    unsigned long ulData, i;

	    ulOrigGPIO_MONID = INREG(RADEON_GPIO_MONID);
	    ulOrigFP2_GEN_CNTL = INREG(RADEON_FP2_GEN_CNTL);
	    ulOrigDISP_OUTPUT_CNTL = INREG(RADEON_DISP_OUTPUT_CNTL);
	    ulOrigCRTC2_GEN_CNTL = INREG(RADEON_CRTC2_GEN_CNTL);
	    ulOrigDISP_LIN_TRANS_GRPH_A = INREG(RADEON_DISP_LIN_TRANS_GRPH_A);
	    ulOrigDISP_LIN_TRANS_GRPH_B = INREG(RADEON_DISP_LIN_TRANS_GRPH_B);
	    ulOrigDISP_LIN_TRANS_GRPH_C = INREG(RADEON_DISP_LIN_TRANS_GRPH_C);
	    ulOrigDISP_LIN_TRANS_GRPH_D = INREG(RADEON_DISP_LIN_TRANS_GRPH_D);
	    ulOrigDISP_LIN_TRANS_GRPH_E = INREG(RADEON_DISP_LIN_TRANS_GRPH_E);
	    ulOrigDISP_LIN_TRANS_GRPH_F = INREG(RADEON_DISP_LIN_TRANS_GRPH_F);

	    ulOrigCRTC2_H_TOTAL_DISP = INREG(RADEON_CRTC2_H_TOTAL_DISP);
	    ulOrigCRTC2_V_TOTAL_DISP = INREG(RADEON_CRTC2_V_TOTAL_DISP);
	    ulOrigCRTC2_H_SYNC_STRT_WID = INREG(RADEON_CRTC2_H_SYNC_STRT_WID);
	    ulOrigCRTC2_V_SYNC_STRT_WID = INREG(RADEON_CRTC2_V_SYNC_STRT_WID);

	    ulData     = INREG(RADEON_GPIO_MONID);
	    ulData    &= ~RADEON_GPIO_A_0;
	    OUTREG(RADEON_GPIO_MONID, ulData);

	    OUTREG(RADEON_FP2_GEN_CNTL, 0x0a000c0c);

	    OUTREG(RADEON_DISP_OUTPUT_CNTL, 0x00000012);

	    OUTREG(RADEON_CRTC2_GEN_CNTL, 0x06000000);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_A, 0x00000000);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_B, 0x000003f0);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_C, 0x00000000);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_D, 0x000003f0);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_E, 0x00000000);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_F, 0x000003f0);
	    OUTREG(RADEON_CRTC2_H_TOTAL_DISP, 0x01000008);
	    OUTREG(RADEON_CRTC2_H_SYNC_STRT_WID, 0x00000800);
	    OUTREG(RADEON_CRTC2_V_TOTAL_DISP, 0x00080001);
	    OUTREG(RADEON_CRTC2_V_SYNC_STRT_WID, 0x00000080);

	    for (i = 0; i < 200; i++) {
		ulData     = INREG(RADEON_GPIO_MONID);
		bConnected = (ulData & RADEON_GPIO_Y_0)?1:0;
		if (!bConnected) break;

		usleep(1000);
	    }

	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_A, ulOrigDISP_LIN_TRANS_GRPH_A);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_B, ulOrigDISP_LIN_TRANS_GRPH_B);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_C, ulOrigDISP_LIN_TRANS_GRPH_C);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_D, ulOrigDISP_LIN_TRANS_GRPH_D);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_E, ulOrigDISP_LIN_TRANS_GRPH_E);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_F, ulOrigDISP_LIN_TRANS_GRPH_F);
	    OUTREG(RADEON_CRTC2_H_TOTAL_DISP, ulOrigCRTC2_H_TOTAL_DISP);
	    OUTREG(RADEON_CRTC2_V_TOTAL_DISP, ulOrigCRTC2_V_TOTAL_DISP);
	    OUTREG(RADEON_CRTC2_H_SYNC_STRT_WID, ulOrigCRTC2_H_SYNC_STRT_WID);
	    OUTREG(RADEON_CRTC2_V_SYNC_STRT_WID, ulOrigCRTC2_V_SYNC_STRT_WID);
	    OUTREG(RADEON_CRTC2_GEN_CNTL, ulOrigCRTC2_GEN_CNTL);
	    OUTREG(RADEON_DISP_OUTPUT_CNTL, ulOrigDISP_OUTPUT_CNTL);
	    OUTREG(RADEON_FP2_GEN_CNTL, ulOrigFP2_GEN_CNTL);
	    OUTREG(RADEON_GPIO_MONID, ulOrigGPIO_MONID);
        } else {
	    unsigned long ulOrigPIXCLKSDATA;
	    unsigned long ulOrigTV_MASTER_CNTL;
	    unsigned long ulOrigTV_DAC_CNTL;
	    unsigned long ulOrigTV_PRE_DAC_MUX_CNTL;
	    unsigned long ulOrigDAC_CNTL2;
	    unsigned long ulData;
	    unsigned long ulMask;

	    ulOrigPIXCLKSDATA = INPLL(pScrn, RADEON_PIXCLKS_CNTL);

	    ulData            = ulOrigPIXCLKSDATA;
	    ulData           &= ~(RADEON_PIX2CLK_ALWAYS_ONb
				  | RADEON_PIX2CLK_DAC_ALWAYS_ONb);
	    ulMask            = ~(RADEON_PIX2CLK_ALWAYS_ONb
			  | RADEON_PIX2CLK_DAC_ALWAYS_ONb);
	    OUTPLLP(pScrn, RADEON_PIXCLKS_CNTL, ulData, ulMask);

	    ulOrigTV_MASTER_CNTL = INREG(RADEON_TV_MASTER_CNTL);
	    ulData               = ulOrigTV_MASTER_CNTL;
	    ulData              &= ~RADEON_TVCLK_ALWAYS_ONb;
	    OUTREG(RADEON_TV_MASTER_CNTL, ulData);

	    ulOrigDAC_CNTL2 = INREG(RADEON_DAC_CNTL2);
	    ulData          = ulOrigDAC_CNTL2;
	    ulData          &= ~RADEON_DAC2_DAC2_CLK_SEL;
	    OUTREG(RADEON_DAC_CNTL2, ulData);

	    ulOrigTV_DAC_CNTL = INREG(RADEON_TV_DAC_CNTL);

	    ulData  = 0x00880213;
	    OUTREG(RADEON_TV_DAC_CNTL, ulData);

	    ulOrigTV_PRE_DAC_MUX_CNTL = INREG(RADEON_TV_PRE_DAC_MUX_CNTL);

	    ulData  =  (RADEON_Y_RED_EN
			| RADEON_C_GRN_EN
			| RADEON_CMP_BLU_EN
			| RADEON_RED_MX_FORCE_DAC_DATA
			| RADEON_GRN_MX_FORCE_DAC_DATA
			| RADEON_BLU_MX_FORCE_DAC_DATA);
            if (IS_R300_VARIANT)
		ulData |= 0x180 << RADEON_TV_FORCE_DAC_DATA_SHIFT;
	    else
		ulData |= 0x1f5 << RADEON_TV_FORCE_DAC_DATA_SHIFT;
	    OUTREG(RADEON_TV_PRE_DAC_MUX_CNTL, ulData);

	    usleep(10000);

	    ulData     = INREG(RADEON_TV_DAC_CNTL);
	    bConnected = (ulData & RADEON_TV_DAC_CMPOUT)?1:0;

	    ulData    = ulOrigPIXCLKSDATA;
	    ulMask    = 0xFFFFFFFFL;
	    OUTPLLP(pScrn, RADEON_PIXCLKS_CNTL, ulData, ulMask);

	    OUTREG(RADEON_TV_MASTER_CNTL, ulOrigTV_MASTER_CNTL);
	    OUTREG(RADEON_DAC_CNTL2, ulOrigDAC_CNTL2);
	    OUTREG(RADEON_TV_DAC_CNTL, ulOrigTV_DAC_CNTL);
	    OUTREG(RADEON_TV_PRE_DAC_MUX_CNTL, ulOrigTV_PRE_DAC_MUX_CNTL);
	}
#endif
	return MT_UNKNOWN;
    }

    return(bConnected ? MT_CRT : MT_NONE);
}
#endif

RADEONMonitorType
legacy_dac_detect(ScrnInfoPtr pScrn, xf86OutputPtr output)
{
    RADEONInfoPtr info      = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONMonitorType found = MT_NONE;

    if (OUTPUT_IS_TV) {
	if (xf86ReturnOptValBool(info->Options, OPTION_FORCE_TVOUT, FALSE)) {
	    if (radeon_output->type == OUTPUT_STV)
		found = MT_STV;
	    else
		found = MT_CTV;
	} else {
	    if (info->InternalTVOut) {
		if (radeon_output->load_detection)
		    found = radeon_detect_tv(pScrn);
		else
		    found = MT_NONE;
	    }
	}
    } else {
	if (radeon_output->DACType == DAC_PRIMARY) {
	    if (radeon_output->load_detection)
		found = radeon_detect_primary_dac(pScrn, TRUE);
	} else if (radeon_output->DACType == DAC_TVDAC) {
	    if (radeon_output->load_detection) {
		if (info->ChipFamily == CHIP_FAMILY_R200)
		    found = radeon_detect_ext_dac(pScrn);
		else
		    found = radeon_detect_tv_dac(pScrn, TRUE);
	    } else
		found = MT_NONE;
	}
    }

    return found;
}

/*
 * Powering done DAC, needed for DPMS problem with ViewSonic P817 (or its variant).
 *
 */
static void
RADEONDacPowerSet(ScrnInfoPtr pScrn, Bool IsOn, Bool IsPrimaryDAC)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (IsPrimaryDAC) {
	uint32_t dac_cntl;
	uint32_t dac_macro_cntl = 0;
	dac_cntl = INREG(RADEON_DAC_CNTL);
	dac_macro_cntl = INREG(RADEON_DAC_MACRO_CNTL);
	if (IsOn) {
	    dac_cntl &= ~RADEON_DAC_PDWN;
	    dac_macro_cntl &= ~(RADEON_DAC_PDWN_R |
				RADEON_DAC_PDWN_G |
				RADEON_DAC_PDWN_B);
	} else {
	    dac_cntl |= RADEON_DAC_PDWN;
	    dac_macro_cntl |= (RADEON_DAC_PDWN_R |
			       RADEON_DAC_PDWN_G |
			       RADEON_DAC_PDWN_B);
	}
	OUTREG(RADEON_DAC_CNTL, dac_cntl);
	OUTREG(RADEON_DAC_MACRO_CNTL, dac_macro_cntl);
    } else {
	uint32_t tv_dac_cntl;
	uint32_t fp2_gen_cntl;

	switch(info->ChipFamily)
	{
	case CHIP_FAMILY_R420:
	case CHIP_FAMILY_RV410:
	    tv_dac_cntl = INREG(RADEON_TV_DAC_CNTL);
	    if (IsOn) {
		tv_dac_cntl &= ~(R420_TV_DAC_RDACPD |
				 R420_TV_DAC_GDACPD |
				 R420_TV_DAC_BDACPD |
				 RADEON_TV_DAC_BGSLEEP);
	    } else {
		tv_dac_cntl |= (R420_TV_DAC_RDACPD |
				R420_TV_DAC_GDACPD |
				R420_TV_DAC_BDACPD |
				RADEON_TV_DAC_BGSLEEP);
	    }
	    OUTREG(RADEON_TV_DAC_CNTL, tv_dac_cntl);
	    break;
	case CHIP_FAMILY_R200:
	    fp2_gen_cntl = INREG(RADEON_FP2_GEN_CNTL);
	    if (IsOn) {
		fp2_gen_cntl |= RADEON_FP2_DVO_EN;
	    } else {
		fp2_gen_cntl &= ~RADEON_FP2_DVO_EN;
	    }
	    OUTREG(RADEON_FP2_GEN_CNTL, fp2_gen_cntl);
	    break;

	default:
	    tv_dac_cntl = INREG(RADEON_TV_DAC_CNTL);
	    if (IsOn) {
		tv_dac_cntl &= ~(RADEON_TV_DAC_RDACPD |
				 RADEON_TV_DAC_GDACPD |
				 RADEON_TV_DAC_BDACPD |
				 RADEON_TV_DAC_BGSLEEP);
	    } else {
		tv_dac_cntl |= (RADEON_TV_DAC_RDACPD |
				RADEON_TV_DAC_GDACPD |
				RADEON_TV_DAC_BDACPD |
				RADEON_TV_DAC_BGSLEEP);
	    }
	    OUTREG(RADEON_TV_DAC_CNTL, tv_dac_cntl);
	    break;
	}
    }
}

/* This is to be used enable/disable displays dynamically */
static void
RADEONEnableDisplay(xf86OutputPtr output, BOOL bEnable)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONSavePtr save = info->ModeReg;
    unsigned char * RADEONMMIO = info->MMIO;
    unsigned long tmp;
    RADEONOutputPrivatePtr radeon_output;
    int tv_dac_change = 0, o;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);

    radeon_output = output->driver_private;
    for (o = 0; o < xf86_config->num_output; o++) {
	if (output == xf86_config->output[o]) {
	    break;
	}
    }

    if (bEnable) {
	/*ErrorF("enable montype: %d\n", radeon_output->MonType);*/
	if (radeon_output->MonType == MT_CRT) {
	    if (radeon_output->DACType == DAC_PRIMARY) {
		info->output_crt1 |= (1 << o);
		tmp = INREG(RADEON_CRTC_EXT_CNTL);
		tmp |= RADEON_CRTC_CRT_ON;
		OUTREG(RADEON_CRTC_EXT_CNTL, tmp);
		save->crtc_ext_cntl |= RADEON_CRTC_CRT_ON;
		RADEONDacPowerSet(pScrn, bEnable, (radeon_output->DACType == DAC_PRIMARY));
	    } else if (radeon_output->DACType == DAC_TVDAC) {
		info->output_crt2 |= (1 << o);
		if (info->ChipFamily == CHIP_FAMILY_R200) {
		    tmp = INREG(RADEON_FP2_GEN_CNTL);
		    tmp |= (RADEON_FP2_ON | RADEON_FP2_DVO_EN);
		    OUTREG(RADEON_FP2_GEN_CNTL, tmp);
		    save->fp2_gen_cntl |= (RADEON_FP2_ON | RADEON_FP2_DVO_EN);
		} else {
		    tmp = INREG(RADEON_CRTC2_GEN_CNTL);
		    tmp |= RADEON_CRTC2_CRT2_ON;
		    OUTREG(RADEON_CRTC2_GEN_CNTL, tmp);
		    save->crtc2_gen_cntl |= RADEON_CRTC2_CRT2_ON;
		}
		tv_dac_change = 1;
		/* IGP chips seem to use a mix of Primary and TVDAC controls */
		if (info->IsIGP) {
		    tmp = INREG(RADEON_CRTC_EXT_CNTL);
		    tmp |= RADEON_CRTC_CRT_ON;
		    OUTREG(RADEON_CRTC_EXT_CNTL, tmp);
		    save->crtc_ext_cntl |= RADEON_CRTC_CRT_ON;
		    RADEONDacPowerSet(pScrn, bEnable, TRUE);
		}
	    }
	} else if (radeon_output->MonType == MT_DFP) {
	    if (radeon_output->TMDSType == TMDS_INT) {
		info->output_dfp1 |= (1 << o);
		tmp = INREG(RADEON_FP_GEN_CNTL);
		tmp |= (RADEON_FP_FPON | RADEON_FP_TMDS_EN);
		OUTREG(RADEON_FP_GEN_CNTL, tmp);
		save->fp_gen_cntl |= (RADEON_FP_FPON | RADEON_FP_TMDS_EN);
		if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
		    (info->ChipFamily == CHIP_FAMILY_RS480)) {
		    tmp = INREG(RS400_FP_2ND_GEN_CNTL);
		    tmp |= (RS400_FP_2ND_ON | RS400_TMDS_2ND_EN);
		    OUTREG(RS400_FP_2ND_GEN_CNTL, tmp);
		    save->fp_2nd_gen_cntl |= (RS400_FP_2ND_ON |
					      RS400_TMDS_2ND_EN);
		}
	    } else if (radeon_output->TMDSType == TMDS_EXT) {
		info->output_dfp2 |= (1 << o);
		tmp = INREG(RADEON_FP2_GEN_CNTL);
		tmp &= ~RADEON_FP2_BLANK_EN;
		tmp |= (RADEON_FP2_ON | RADEON_FP2_DVO_EN);
		OUTREG(RADEON_FP2_GEN_CNTL, tmp);
		save->fp2_gen_cntl |= (RADEON_FP2_ON | RADEON_FP2_DVO_EN);
		save->fp2_gen_cntl &= ~RADEON_FP2_BLANK_EN;
		if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
		    (info->ChipFamily == CHIP_FAMILY_RS480)) {
		    tmp = INREG(RS400_FP2_2_GEN_CNTL);
		    tmp &= ~RS400_FP2_2_BLANK_EN;
		    tmp |= (RS400_FP2_2_ON | RS400_FP2_2_DVO2_EN);
		    OUTREG(RS400_FP2_2_GEN_CNTL, tmp);
		    save->fp2_2_gen_cntl |= (RS400_FP2_2_ON | RS400_FP2_2_DVO2_EN);
		    save->fp2_2_gen_cntl &= ~RS400_FP2_2_BLANK_EN;
		}
	    }
	} else if (radeon_output->MonType == MT_LCD) {
	    info->output_lcd1 |= (1 << o);
	    tmp = INREG(RADEON_LVDS_GEN_CNTL);
	    tmp |= (RADEON_LVDS_ON | RADEON_LVDS_BLON | RADEON_LVDS_EN);
	    tmp &= ~(RADEON_LVDS_DISPLAY_DIS);
	    usleep (radeon_output->PanelPwrDly * 1000);
	    OUTREG(RADEON_LVDS_GEN_CNTL, tmp);
	    save->lvds_gen_cntl |= (RADEON_LVDS_ON | RADEON_LVDS_BLON | RADEON_LVDS_EN);
	    save->lvds_gen_cntl &= ~(RADEON_LVDS_DISPLAY_DIS);
	} else if (radeon_output->MonType == MT_STV ||
		   radeon_output->MonType == MT_CTV) {
	    info->output_tv1 |= (1 << o);
	    tmp = INREG(RADEON_TV_MASTER_CNTL);
	    tmp |= RADEON_TV_ON;
	    OUTREG(RADEON_TV_MASTER_CNTL, tmp);
	    tv_dac_change = 2;
	    radeon_output->tv_on = TRUE;
	}
    } else {
	/*ErrorF("disable montype: %d\n", radeon_output->MonType);*/
	if (radeon_output->MonType == MT_CRT) {
	    if (radeon_output->DACType == DAC_PRIMARY) {
		info->output_crt1 &= ~(1 << o);
		if (!info->output_crt1) {
		    tmp = INREG(RADEON_CRTC_EXT_CNTL);
		    tmp &= ~RADEON_CRTC_CRT_ON;
		    OUTREG(RADEON_CRTC_EXT_CNTL, tmp);
		    save->crtc_ext_cntl &= ~RADEON_CRTC_CRT_ON;
		    RADEONDacPowerSet(pScrn, bEnable, (radeon_output->DACType == DAC_PRIMARY));
		}
	    } else if (radeon_output->DACType == DAC_TVDAC) {
		info->output_crt2 &= ~(1 << o);
		tv_dac_change = 1;
		if (!info->output_crt2) {
		    if (info->ChipFamily == CHIP_FAMILY_R200) {
			tmp = INREG(RADEON_FP2_GEN_CNTL);
			tmp &= ~(RADEON_FP2_ON | RADEON_FP2_DVO_EN);
			OUTREG(RADEON_FP2_GEN_CNTL, tmp);
			save->fp2_gen_cntl &= ~(RADEON_FP2_ON | RADEON_FP2_DVO_EN);
		    } else {
			tmp = INREG(RADEON_CRTC2_GEN_CNTL);
			tmp &= ~RADEON_CRTC2_CRT2_ON;
			OUTREG(RADEON_CRTC2_GEN_CNTL, tmp);
			save->crtc2_gen_cntl &= ~RADEON_CRTC2_CRT2_ON;
		    }
		}
		/* IGP chips seem to use a mix of Primary and TVDAC controls */
		if (info->IsIGP) {
		    tmp = INREG(RADEON_CRTC_EXT_CNTL);
		    tmp &= ~RADEON_CRTC_CRT_ON;
		    OUTREG(RADEON_CRTC_EXT_CNTL, tmp);
		    save->crtc_ext_cntl &= ~RADEON_CRTC_CRT_ON;
		    RADEONDacPowerSet(pScrn, bEnable, TRUE);
		}
	    }
	} else if (radeon_output->MonType == MT_DFP) {
	    if (radeon_output->TMDSType == TMDS_INT) {
		info->output_dfp1 &= ~(1 << o);
		if (!info->output_dfp1) {
		    tmp = INREG(RADEON_FP_GEN_CNTL);
		    tmp &= ~(RADEON_FP_FPON | RADEON_FP_TMDS_EN);
		    OUTREG(RADEON_FP_GEN_CNTL, tmp);
		    save->fp_gen_cntl &= ~(RADEON_FP_FPON | RADEON_FP_TMDS_EN);
		    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
			(info->ChipFamily == CHIP_FAMILY_RS480)) {
			tmp = INREG(RS400_FP_2ND_GEN_CNTL);
			tmp &= ~(RS400_FP_2ND_ON | RS400_TMDS_2ND_EN);
			OUTREG(RS400_FP_2ND_GEN_CNTL, tmp);
			save->fp_2nd_gen_cntl &= ~(RS400_FP_2ND_ON |
						   RS400_TMDS_2ND_EN);
		    }
		}
	    } else if (radeon_output->TMDSType == TMDS_EXT) {
		info->output_dfp2 &= ~(1 << o);
		if (!info->output_dfp2) {
		    tmp = INREG(RADEON_FP2_GEN_CNTL);
		    tmp |= RADEON_FP2_BLANK_EN;
		    tmp &= ~(RADEON_FP2_ON | RADEON_FP2_DVO_EN);
		    OUTREG(RADEON_FP2_GEN_CNTL, tmp);
		    save->fp2_gen_cntl &= ~(RADEON_FP2_ON | RADEON_FP2_DVO_EN);
		    save->fp2_gen_cntl |= RADEON_FP2_BLANK_EN;
		    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
			(info->ChipFamily == CHIP_FAMILY_RS480)) {
			tmp = INREG(RS400_FP2_2_GEN_CNTL);
			tmp |= RS400_FP2_2_BLANK_EN;
			tmp &= ~(RS400_FP2_2_ON | RS400_FP2_2_DVO2_EN);
			OUTREG(RS400_FP2_2_GEN_CNTL, tmp);
			save->fp2_2_gen_cntl &= ~(RS400_FP2_2_ON | RS400_FP2_2_DVO2_EN);
			save->fp2_2_gen_cntl |= RS400_FP2_2_BLANK_EN;
		    }
		}
	    }
	} else if (radeon_output->MonType == MT_LCD) {
	    info->output_lcd1 &= ~(1 << o);
	    if (!info->output_lcd1) {
		unsigned long tmpPixclksCntl = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
		if (info->IsMobility || info->IsIGP) {
		    /* Asic bug, when turning off LVDS_ON, we have to make sure
		       RADEON_PIXCLK_LVDS_ALWAYS_ON bit is off
		    */
		    OUTPLLP(pScrn, RADEON_PIXCLKS_CNTL, 0, ~RADEON_PIXCLK_LVDS_ALWAYS_ONb);
		}
		tmp = INREG(RADEON_LVDS_GEN_CNTL);
		tmp |= RADEON_LVDS_DISPLAY_DIS;
		tmp &= ~(RADEON_LVDS_ON | RADEON_LVDS_BLON | RADEON_LVDS_EN);
		OUTREG(RADEON_LVDS_GEN_CNTL, tmp);
		save->lvds_gen_cntl |= RADEON_LVDS_DISPLAY_DIS;
		save->lvds_gen_cntl &= ~(RADEON_LVDS_ON | RADEON_LVDS_BLON | RADEON_LVDS_EN);
		if (info->IsMobility || info->IsIGP) {
		    OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmpPixclksCntl);
		}
	    }
	} else if (radeon_output->MonType == MT_STV || radeon_output->MonType == MT_CTV) {
	    info->output_tv1 &= ~(1 << o);
	    tv_dac_change = 2;
	    if (!info->output_tv1) {
		tmp = INREG(RADEON_TV_MASTER_CNTL);
		tmp &= ~RADEON_TV_ON;
		OUTREG(RADEON_TV_MASTER_CNTL, tmp);
		radeon_output->tv_on = FALSE;
	    }
	}
    }

    if (tv_dac_change) {
	if (bEnable)
	    info->tv_dac_enable_mask |= tv_dac_change;
	else
	    info->tv_dac_enable_mask &= ~tv_dac_change;

	if (bEnable && info->tv_dac_enable_mask)
	    RADEONDacPowerSet(pScrn, bEnable, (radeon_output->DACType == DAC_PRIMARY));
	else if (!bEnable && info->tv_dac_enable_mask == 0)
	    RADEONDacPowerSet(pScrn, bEnable, (radeon_output->DACType == DAC_PRIMARY));

    }
}

void
legacy_output_dpms(xf86OutputPtr output, int mode)
{
    switch(mode) {
    case DPMSModeOn:
	RADEONEnableDisplay(output, TRUE);
	break;
    case DPMSModeOff:
    case DPMSModeSuspend:
    case DPMSModeStandby:
	RADEONEnableDisplay(output, FALSE);
	break;
    }
}

static void
RADEONInitFPRegisters(xf86OutputPtr output, RADEONSavePtr save,
		      DisplayModePtr mode, BOOL IsPrimary)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONEntPtr  pRADEONEnt = RADEONEntPriv(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int i;
    uint32_t tmp = info->SavedReg->tmds_pll_cntl & 0xfffff;

    for (i=0; i<4; i++) {
	if (radeon_output->tmds_pll[i].freq == 0) break;
	if ((uint32_t)(mode->Clock/10) < radeon_output->tmds_pll[i].freq) {
	    tmp = radeon_output->tmds_pll[i].value ;
	    break;
	}
    }

    if (IS_R300_VARIANT || (info->ChipFamily == CHIP_FAMILY_RV280)) {
	if (tmp & 0xfff00000)
	    save->tmds_pll_cntl = tmp;
	else {
	    save->tmds_pll_cntl = info->SavedReg->tmds_pll_cntl & 0xfff00000;
	    save->tmds_pll_cntl |= tmp;
	}
    } else save->tmds_pll_cntl = tmp;

    save->tmds_transmitter_cntl = info->SavedReg->tmds_transmitter_cntl &
					~(RADEON_TMDS_TRANSMITTER_PLLRST);

    if (IS_R300_VARIANT || (info->ChipFamily == CHIP_FAMILY_R200) || !pRADEONEnt->HasCRTC2)
	save->tmds_transmitter_cntl &= ~(RADEON_TMDS_TRANSMITTER_PLLEN);
    else /* weird, RV chips got this bit reversed? */
	save->tmds_transmitter_cntl |= (RADEON_TMDS_TRANSMITTER_PLLEN);

    save->fp_gen_cntl = info->SavedReg->fp_gen_cntl |
			 (RADEON_FP_CRTC_DONT_SHADOW_VPAR |
			  RADEON_FP_CRTC_DONT_SHADOW_HEND );

    save->fp_gen_cntl &= ~(RADEON_FP_FPON | RADEON_FP_TMDS_EN);

    if (pScrn->rgbBits == 8)
	save->fp_gen_cntl |= RADEON_FP_PANEL_FORMAT;  /* 24 bit format */
    else
	save->fp_gen_cntl &= ~RADEON_FP_PANEL_FORMAT;/* 18 bit format */


    if (IsPrimary) {
	if ((IS_R300_VARIANT) || (info->ChipFamily == CHIP_FAMILY_R200)) {
	    save->fp_gen_cntl &= ~R200_FP_SOURCE_SEL_MASK;
	    if (radeon_output->Flags & RADEON_USE_RMX)
		save->fp_gen_cntl |= R200_FP_SOURCE_SEL_RMX;
	    else
		save->fp_gen_cntl |= R200_FP_SOURCE_SEL_CRTC1;
	} else
	    save->fp_gen_cntl |= RADEON_FP_SEL_CRTC1;
    } else {
	if ((IS_R300_VARIANT) || (info->ChipFamily == CHIP_FAMILY_R200)) {
	    save->fp_gen_cntl &= ~R200_FP_SOURCE_SEL_MASK;
	    save->fp_gen_cntl |= R200_FP_SOURCE_SEL_CRTC2;
	} else
	    save->fp_gen_cntl |= RADEON_FP_SEL_CRTC2;
    }

    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480)) {
	save->tmds2_transmitter_cntl = info->SavedReg->tmds2_transmitter_cntl &
	    ~(RS400_TMDS2_PLLRST);
	save->tmds2_transmitter_cntl &= ~(RS400_TMDS2_PLLEN);

	save->fp_2nd_gen_cntl = info->SavedReg->fp_2nd_gen_cntl;

	if (pScrn->rgbBits == 8)
	    save->fp_2nd_gen_cntl |= RS400_PANEL_FORMAT_2ND;  /* 24 bit format */
	else
	    save->fp_2nd_gen_cntl &= ~RS400_PANEL_FORMAT_2ND;/* 18 bit format */

	save->fp_2nd_gen_cntl &= ~RS400_FP_2ND_SOURCE_SEL_MASK;

	if (IsPrimary) {
	    if (radeon_output->Flags & RADEON_USE_RMX)
		save->fp_2nd_gen_cntl |= RS400_FP_2ND_SOURCE_SEL_RMX;
	    else
		save->fp_2nd_gen_cntl |= RS400_FP_2ND_SOURCE_SEL_CRTC1;
	} else
	    save->fp_2nd_gen_cntl |= RS400_FP_2ND_SOURCE_SEL_CRTC2;
    }

}

static void
RADEONInitFP2Registers(xf86OutputPtr output, RADEONSavePtr save,
		       DisplayModePtr mode, BOOL IsPrimary)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if (pScrn->rgbBits == 8)
	save->fp2_gen_cntl = info->SavedReg->fp2_gen_cntl |
				RADEON_FP2_PANEL_FORMAT; /* 24 bit format, */
    else
	save->fp2_gen_cntl = info->SavedReg->fp2_gen_cntl &
				~RADEON_FP2_PANEL_FORMAT;/* 18 bit format, */

    save->fp2_gen_cntl &= ~(RADEON_FP2_ON |
			    RADEON_FP2_DVO_EN |
			    RADEON_FP2_DVO_RATE_SEL_SDR);


    /* XXX: these may be oem specific */
    if (IS_R300_VARIANT) {
	save->fp2_gen_cntl |= RADEON_FP2_PAD_FLOP_EN | R300_FP2_DVO_CLOCK_MODE_SINGLE;
#if 0
	if (mode->Clock > 165000)
	    save->fp2_gen_cntl |= R300_FP2_DVO_DUAL_CHANNEL_EN;
#endif
    }

    if (IsPrimary) {
	if ((info->ChipFamily == CHIP_FAMILY_R200) || IS_R300_VARIANT) {
	    save->fp2_gen_cntl &= ~R200_FP2_SOURCE_SEL_MASK;
	    if (radeon_output->Flags & RADEON_USE_RMX)
		save->fp2_gen_cntl |= R200_FP2_SOURCE_SEL_RMX;
	    else
		save->fp2_gen_cntl |= R200_FP2_SOURCE_SEL_CRTC1;
	} else {
	    save->fp2_gen_cntl &= ~RADEON_FP2_SRC_SEL_CRTC2;
	}
    } else {
	if ((info->ChipFamily == CHIP_FAMILY_R200) || IS_R300_VARIANT) {
	    save->fp2_gen_cntl &= ~R200_FP2_SOURCE_SEL_MASK;
	    save->fp2_gen_cntl |= R200_FP2_SOURCE_SEL_CRTC2;
	} else {
	    save->fp2_gen_cntl |= RADEON_FP2_SRC_SEL_CRTC2;
	}
    }

    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480)) {
	if (pScrn->rgbBits == 8)
	    save->fp2_2_gen_cntl = info->SavedReg->fp2_2_gen_cntl |
		RS400_FP2_2_PANEL_FORMAT; /* 24 bit format, */
	else
	    save->fp2_2_gen_cntl = info->SavedReg->fp2_2_gen_cntl &
		~RS400_FP2_2_PANEL_FORMAT;/* 18 bit format, */

	save->fp2_2_gen_cntl &= ~(RS400_FP2_2_ON |
				  RS400_FP2_2_DVO2_EN |
				  RS400_FP2_2_SOURCE_SEL_MASK);

	if (IsPrimary) {
	    if (radeon_output->Flags & RADEON_USE_RMX)
		save->fp2_2_gen_cntl |= RS400_FP2_2_SOURCE_SEL_RMX;
	    else
		save->fp2_2_gen_cntl |= RS400_FP2_2_SOURCE_SEL_CRTC1;
	} else
	    save->fp2_2_gen_cntl |= RS400_FP2_2_SOURCE_SEL_CRTC2;
    }

}

static void
RADEONInitLVDSRegisters(xf86OutputPtr output, RADEONSavePtr save,
			DisplayModePtr mode, BOOL IsPrimary)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    save->lvds_pll_cntl = (info->SavedReg->lvds_pll_cntl |
			   RADEON_LVDS_PLL_EN);

    save->lvds_pll_cntl &= ~RADEON_LVDS_PLL_RESET;

    save->lvds_gen_cntl = info->SavedReg->lvds_gen_cntl;
    save->lvds_gen_cntl |= RADEON_LVDS_DISPLAY_DIS;
    save->lvds_gen_cntl &= ~(RADEON_LVDS_ON |
			     RADEON_LVDS_BLON |
			     RADEON_LVDS_EN |
			     RADEON_LVDS_RST_FM);

    if (IS_R300_VARIANT)
	save->lvds_pll_cntl &= ~(R300_LVDS_SRC_SEL_MASK);

    if (IsPrimary) {
	if (IS_R300_VARIANT) {
	    if (radeon_output->Flags & RADEON_USE_RMX)
		save->lvds_pll_cntl |= R300_LVDS_SRC_SEL_RMX;
	} else
	    save->lvds_gen_cntl &= ~RADEON_LVDS_SEL_CRTC2;
    } else {
	if (IS_R300_VARIANT) {
	    save->lvds_pll_cntl |= R300_LVDS_SRC_SEL_CRTC2;
	} else
	    save->lvds_gen_cntl |= RADEON_LVDS_SEL_CRTC2;
    }

}

static void
RADEONInitRMXRegisters(xf86OutputPtr output, RADEONSavePtr save,
		       DisplayModePtr mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int    xres = mode->HDisplay;
    int    yres = mode->VDisplay;
    Bool   Hscale = TRUE, Vscale = TRUE;
    int    hsync_wid;
    int    vsync_wid;
    int    hsync_start;


    save->fp_vert_stretch = info->SavedReg->fp_vert_stretch &
	                    (RADEON_VERT_STRETCH_RESERVED |
			     RADEON_VERT_AUTO_RATIO_INC);
    save->fp_horz_stretch = info->SavedReg->fp_horz_stretch &
	                    (RADEON_HORZ_FP_LOOP_STRETCH |
	                     RADEON_HORZ_AUTO_RATIO_INC);

    save->crtc_more_cntl = 0;
    if ((info->ChipFamily == CHIP_FAMILY_RS100) ||
	(info->ChipFamily == CHIP_FAMILY_RS200)) {
	/* This is to workaround the asic bug for RMX, some versions
           of BIOS dosen't have this register initialized correctly.
	*/
	save->crtc_more_cntl |= RADEON_CRTC_H_CUTOFF_ACTIVE_EN;
    }


    save->fp_crtc_h_total_disp = ((((mode->CrtcHTotal / 8) - 1) & 0x3ff)
				  | ((((mode->CrtcHDisplay / 8) - 1) & 0x1ff)
				     << 16));

    hsync_wid = (mode->CrtcHSyncEnd - mode->CrtcHSyncStart) / 8;
    if (!hsync_wid)       hsync_wid = 1;
    hsync_start = mode->CrtcHSyncStart - 8;

    save->fp_h_sync_strt_wid = ((hsync_start & 0x1fff)
				| ((hsync_wid & 0x3f) << 16)
				| ((mode->Flags & V_NHSYNC)
				   ? RADEON_CRTC_H_SYNC_POL
				   : 0));

    save->fp_crtc_v_total_disp = (((mode->CrtcVTotal - 1) & 0xffff)
				  | ((mode->CrtcVDisplay - 1) << 16));

    vsync_wid = mode->CrtcVSyncEnd - mode->CrtcVSyncStart;
    if (!vsync_wid)       vsync_wid = 1;

    save->fp_v_sync_strt_wid = (((mode->CrtcVSyncStart - 1) & 0xfff)
				| ((vsync_wid & 0x1f) << 16)
				| ((mode->Flags & V_NVSYNC)
				   ? RADEON_CRTC_V_SYNC_POL
				   : 0));

    save->fp_horz_vert_active = 0;

    if (radeon_output->MonType != MT_LCD && radeon_output->MonType != MT_DFP)
	return;

    if (radeon_output->PanelXRes == 0 || radeon_output->PanelYRes == 0) {
	Hscale = FALSE;
	Vscale = FALSE;
    } else {
	if (xres > radeon_output->PanelXRes) xres = radeon_output->PanelXRes;
	if (yres > radeon_output->PanelYRes) yres = radeon_output->PanelYRes;

	if (xres == radeon_output->PanelXRes)
	    Hscale = FALSE;
	if (yres == radeon_output->PanelYRes)
	    Vscale = FALSE;
    }

    if ((!Hscale) || (!(radeon_output->Flags & RADEON_USE_RMX)) ||
	(radeon_output->rmx_type == RMX_CENTER)) {
	save->fp_horz_stretch |= ((xres/8-1)<<16);
    } else {
	CARD32 scale, inc;
	inc = (save->fp_horz_stretch & RADEON_HORZ_AUTO_RATIO_INC) ? 1 : 0;
	scale = ((xres + inc) * RADEON_HORZ_STRETCH_RATIO_MAX)
	    / radeon_output->PanelXRes + 1;
	save->fp_horz_stretch |= (((scale) & RADEON_HORZ_STRETCH_RATIO_MASK) |
				  RADEON_HORZ_STRETCH_BLEND |
				  RADEON_HORZ_STRETCH_ENABLE |
				  ((radeon_output->PanelXRes/8-1)<<16));
    }

    if ((!Vscale) || (!(radeon_output->Flags & RADEON_USE_RMX)) ||
	(radeon_output->rmx_type == RMX_CENTER)) {
	save->fp_vert_stretch |= ((yres-1)<<12);
    } else {
	CARD32 scale, inc;
	inc = (save->fp_vert_stretch & RADEON_VERT_AUTO_RATIO_INC) ? 1 : 0;
	scale = ((yres + inc) * RADEON_VERT_STRETCH_RATIO_MAX)
	    / radeon_output->PanelYRes + 1;
	save->fp_vert_stretch |= (((scale) & RADEON_VERT_STRETCH_RATIO_MASK) |
				  RADEON_VERT_STRETCH_ENABLE |
				  RADEON_VERT_STRETCH_BLEND |
				  ((radeon_output->PanelYRes-1)<<12));
    }

    if ((radeon_output->rmx_type == RMX_CENTER) &&
	(radeon_output->Flags & RADEON_USE_RMX)) {
	int    blank_width;

	save->crtc_more_cntl |= (RADEON_CRTC_AUTO_HORZ_CENTER_EN |
				 RADEON_CRTC_AUTO_VERT_CENTER_EN);

	blank_width = (mode->CrtcHBlankEnd - mode->CrtcHBlankStart) / 8;
	if (blank_width > 110) blank_width = 110;

	save->fp_crtc_h_total_disp = (((blank_width) & 0x3ff)
				      | ((((mode->CrtcHDisplay / 8) - 1) & 0x1ff)
					 << 16));

	hsync_wid = (mode->CrtcHSyncEnd - mode->CrtcHSyncStart) / 8;
	if (!hsync_wid)       hsync_wid = 1;

	save->fp_h_sync_strt_wid = ((((mode->CrtcHSyncStart - mode->CrtcHBlankStart) / 8) & 0x1fff)
				    | ((hsync_wid & 0x3f) << 16)
				    | ((mode->Flags & V_NHSYNC)
				       ? RADEON_CRTC_H_SYNC_POL
				       : 0));

	save->fp_crtc_v_total_disp = (((mode->CrtcVBlankEnd - mode->CrtcVBlankStart) & 0xffff)
				      | ((mode->CrtcVDisplay - 1) << 16));

	vsync_wid = mode->CrtcVSyncEnd - mode->CrtcVSyncStart;
	if (!vsync_wid)       vsync_wid = 1;

	save->fp_v_sync_strt_wid = ((((mode->CrtcVSyncStart - mode->CrtcVBlankStart) & 0xfff)
				    | ((vsync_wid & 0x1f) << 16)
				    | ((mode->Flags & V_NVSYNC)
				       ? RADEON_CRTC_V_SYNC_POL
				       : 0)));

	save->fp_horz_vert_active = (((radeon_output->PanelYRes) & 0xfff) |
				     (((radeon_output->PanelXRes / 8) & 0x1ff) << 16));

    }
}

static void
RADEONInitDACRegisters(xf86OutputPtr output, RADEONSavePtr save,
		       DisplayModePtr mode, BOOL IsPrimary)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);

    if (IsPrimary) {
	if ((info->ChipFamily == CHIP_FAMILY_R200) || IS_R300_VARIANT) {
            save->disp_output_cntl = info->SavedReg->disp_output_cntl &
					~RADEON_DISP_DAC_SOURCE_MASK;
        } else {
            save->dac2_cntl = info->SavedReg->dac2_cntl & ~(RADEON_DAC2_DAC_CLK_SEL);
        }
    } else {
        if ((info->ChipFamily == CHIP_FAMILY_R200) || IS_R300_VARIANT) {
            save->disp_output_cntl = info->SavedReg->disp_output_cntl &
					~RADEON_DISP_DAC_SOURCE_MASK;
            save->disp_output_cntl |= RADEON_DISP_DAC_SOURCE_CRTC2;
        } else {
            save->dac2_cntl = info->SavedReg->dac2_cntl | RADEON_DAC2_DAC_CLK_SEL;
        }
    }
    save->dac_cntl = (RADEON_DAC_MASK_ALL
		      | RADEON_DAC_VGA_ADR_EN
		      | (info->dac6bits ? 0 : RADEON_DAC_8BIT_EN));

    save->dac_macro_cntl = info->SavedReg->dac_macro_cntl;
}

static void
RADEONInitTvDacCntl(xf86OutputPtr output, RADEONSavePtr save)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if (info->ChipFamily == CHIP_FAMILY_R420 ||
	info->ChipFamily == CHIP_FAMILY_RV410) {
	save->tv_dac_cntl = info->SavedReg->tv_dac_cntl &
			     ~(RADEON_TV_DAC_STD_MASK |
			       RADEON_TV_DAC_BGADJ_MASK |
			       R420_TV_DAC_DACADJ_MASK |
			       R420_TV_DAC_RDACPD |
			       R420_TV_DAC_GDACPD |
			       R420_TV_DAC_GDACPD |
			       R420_TV_DAC_TVENABLE);
    } else {
	save->tv_dac_cntl = info->SavedReg->tv_dac_cntl &
			     ~(RADEON_TV_DAC_STD_MASK |
			       RADEON_TV_DAC_BGADJ_MASK |
			       RADEON_TV_DAC_DACADJ_MASK |
			       RADEON_TV_DAC_RDACPD |
			       RADEON_TV_DAC_GDACPD |
			       RADEON_TV_DAC_GDACPD);
    }

    save->tv_dac_cntl |= (RADEON_TV_DAC_NBLANK |
			  RADEON_TV_DAC_NHOLD |
			  RADEON_TV_DAC_STD_PS2 |
			  radeon_output->ps2_tvdac_adj);

}

static void
RADEONInitDAC2Registers(xf86OutputPtr output, RADEONSavePtr save,
			DisplayModePtr mode, BOOL IsPrimary)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);

    /*0x0028023;*/
    RADEONInitTvDacCntl(output, save);

    if (IS_R300_VARIANT)
	save->gpiopad_a = info->SavedReg->gpiopad_a | 1;

    save->dac2_cntl = info->SavedReg->dac2_cntl | RADEON_DAC2_DAC2_CLK_SEL;

    if (IsPrimary) {
        if (IS_R300_VARIANT) {
            save->disp_output_cntl = info->SavedReg->disp_output_cntl &
					~RADEON_DISP_TVDAC_SOURCE_MASK;
            save->disp_output_cntl |= RADEON_DISP_TVDAC_SOURCE_CRTC;
        } else if (info->ChipFamily == CHIP_FAMILY_R200) {
	    save->fp2_gen_cntl = info->SavedReg->fp2_gen_cntl &
				  ~(R200_FP2_SOURCE_SEL_MASK |
				    RADEON_FP2_DVO_RATE_SEL_SDR);
	} else {
            save->disp_hw_debug = info->SavedReg->disp_hw_debug | RADEON_CRT2_DISP1_SEL;
        }
    } else {
        if (IS_R300_VARIANT) {
            save->disp_output_cntl = info->SavedReg->disp_output_cntl &
					~RADEON_DISP_TVDAC_SOURCE_MASK;
            save->disp_output_cntl |= RADEON_DISP_TVDAC_SOURCE_CRTC2;
	} else if (info->ChipFamily == CHIP_FAMILY_R200) {
	    save->fp2_gen_cntl = info->SavedReg->fp2_gen_cntl &
				  ~(R200_FP2_SOURCE_SEL_MASK |
				    RADEON_FP2_DVO_RATE_SEL_SDR);
            save->fp2_gen_cntl |= R200_FP2_SOURCE_SEL_CRTC2;
        } else {
            save->disp_hw_debug = info->SavedReg->disp_hw_debug &
					~RADEON_CRT2_DISP1_SEL;
        }
    }
}

static void
RADEONInitOutputRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save,
			  DisplayModePtr mode, xf86OutputPtr output,
			  int crtc_num)
{
    Bool IsPrimary = crtc_num == 0 ? TRUE : FALSE;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info = RADEONPTR(pScrn);

    if (crtc_num == 0)
	RADEONInitRMXRegisters(output, save, mode);

    if (radeon_output->MonType == MT_CRT) {
	if (radeon_output->DACType == DAC_PRIMARY) {
	    RADEONInitDACRegisters(output, save, mode, IsPrimary);
	} else {
	    RADEONInitDAC2Registers(output, save, mode, IsPrimary);
	    /* IGP chips seem to use a mix of primary and TVDAC controls */
	    if (info->IsIGP)
		RADEONInitDACRegisters(output, save, mode, IsPrimary);
	}
    } else if (radeon_output->MonType == MT_LCD) {
	RADEONInitLVDSRegisters(output, save, mode, IsPrimary);
    } else if (radeon_output->MonType == MT_DFP) {
	if (radeon_output->TMDSType == TMDS_INT) {
	    RADEONInitFPRegisters(output, save, mode, IsPrimary);
	} else {
	    RADEONInitFP2Registers(output, save, mode, IsPrimary);
	}
    } else if (radeon_output->MonType == MT_STV ||
	       radeon_output->MonType == MT_CTV) {
	RADEONInitTVRegisters(output, save, mode, IsPrimary);
    }
}

void
legacy_output_mode_set(xf86OutputPtr output, DisplayModePtr mode,
		  DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr	    pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    xf86CrtcPtr	crtc = output->crtc;
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;

    RADEONInitOutputRegisters(pScrn, info->ModeReg, adjusted_mode, output, radeon_crtc->crtc_id);

    if (radeon_crtc->crtc_id == 0)
	RADEONRestoreRMXRegisters(pScrn, info->ModeReg);

    switch(radeon_output->MonType) {
    case MT_LCD:
	ErrorF("restore LVDS\n");
	RADEONRestoreLVDSRegisters(pScrn, info->ModeReg);
	break;
    case MT_DFP:
	if (radeon_output->TMDSType == TMDS_INT) {
	    ErrorF("restore FP\n");
	    RADEONRestoreFPRegisters(pScrn, info->ModeReg);
	} else {
	    ErrorF("restore FP2\n");
	    if (info->IsAtomBios) {
		unsigned char *RADEONMMIO = info->MMIO;
		uint32_t fp2_gen_cntl;

		atombios_external_tmds_setup(output, mode);
		/* r4xx atom has hard coded crtc mappings in the atom code
		 * Fix it up here.
		 */
		fp2_gen_cntl = INREG(RADEON_FP2_GEN_CNTL) & ~R200_FP2_SOURCE_SEL_MASK;
		if (radeon_crtc->crtc_id == 1)
		    fp2_gen_cntl |= R200_FP2_SOURCE_SEL_CRTC2;
		else {
		    if (radeon_output->Flags & RADEON_USE_RMX)
			fp2_gen_cntl |= R200_FP2_SOURCE_SEL_RMX;
		    else
			fp2_gen_cntl |= R200_FP2_SOURCE_SEL_CRTC1;
		}
		OUTREG(RADEON_FP2_GEN_CNTL, fp2_gen_cntl);
	    } else {
		RADEONRestoreDVOChip(pScrn, output);
		RADEONRestoreFP2Registers(pScrn, info->ModeReg);
	    }
	}
	break;
    case MT_STV:
    case MT_CTV:
	ErrorF("restore tv\n");
	RADEONRestoreDACRegisters(pScrn, info->ModeReg);
	RADEONRestoreTVRegisters(pScrn, info->ModeReg);
	break;
    default:
	ErrorF("restore dac\n");
	RADEONRestoreDACRegisters(pScrn, info->ModeReg);
    }

}

/* the following functions are based on the load detection code
 * in the beos radeon driver by Thomas Kurschel and the existing
 * load detection code in this driver.
 */
static RADEONMonitorType
radeon_detect_primary_dac(ScrnInfoPtr pScrn, Bool color)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t vclk_ecp_cntl, crtc_ext_cntl;
    uint32_t dac_ext_cntl, dac_cntl, dac_macro_cntl, tmp;
    RADEONMonitorType found = MT_NONE;

    /* save the regs we need */
    vclk_ecp_cntl = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);
    crtc_ext_cntl = INREG(RADEON_CRTC_EXT_CNTL);
    dac_ext_cntl = INREG(RADEON_DAC_EXT_CNTL);
    dac_cntl = INREG(RADEON_DAC_CNTL);
    dac_macro_cntl = INREG(RADEON_DAC_MACRO_CNTL);

    tmp = vclk_ecp_cntl &
	~(RADEON_PIXCLK_ALWAYS_ONb | RADEON_PIXCLK_DAC_ALWAYS_ONb);
    OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, tmp);

    tmp = crtc_ext_cntl | RADEON_CRTC_CRT_ON;
    OUTREG(RADEON_CRTC_EXT_CNTL, tmp);

    tmp = RADEON_DAC_FORCE_BLANK_OFF_EN |
	RADEON_DAC_FORCE_DATA_EN;

    if (color)
	tmp |= RADEON_DAC_FORCE_DATA_SEL_RGB;
    else
	tmp |= RADEON_DAC_FORCE_DATA_SEL_G;

    if (IS_R300_VARIANT)
	tmp |= (0x1b6 << RADEON_DAC_FORCE_DATA_SHIFT);
    else
	tmp |= (0x180 << RADEON_DAC_FORCE_DATA_SHIFT);

    OUTREG(RADEON_DAC_EXT_CNTL, tmp);

    tmp = dac_cntl & ~(RADEON_DAC_RANGE_CNTL_MASK | RADEON_DAC_PDWN);
    tmp |= RADEON_DAC_RANGE_CNTL_PS2 | RADEON_DAC_CMP_EN;
    OUTREG(RADEON_DAC_CNTL, tmp);

    tmp &= ~(RADEON_DAC_PDWN_R |
	     RADEON_DAC_PDWN_G |
	     RADEON_DAC_PDWN_B);

    OUTREG(RADEON_DAC_MACRO_CNTL, tmp);

    usleep(2000);

    if (INREG(RADEON_DAC_CNTL) & RADEON_DAC_CMP_OUTPUT) {
	found = MT_CRT;
	xf86DrvMsg (pScrn->scrnIndex, X_INFO,
		    "Found %s CRT connected to primary DAC\n",
		    color ? "color" : "bw");
    }

    /* restore the regs we used */
    OUTREG(RADEON_DAC_CNTL, dac_cntl);
    OUTREG(RADEON_DAC_MACRO_CNTL, dac_macro_cntl);
    OUTREG(RADEON_DAC_EXT_CNTL, dac_ext_cntl);
    OUTREG(RADEON_CRTC_EXT_CNTL, crtc_ext_cntl);
    OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, vclk_ecp_cntl);

    return found;
}

static RADEONMonitorType
radeon_detect_ext_dac(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t gpio_monid, fp2_gen_cntl, disp_output_cntl, crtc2_gen_cntl;
    uint32_t disp_lin_trans_grph_a, disp_lin_trans_grph_b, disp_lin_trans_grph_c;
    uint32_t disp_lin_trans_grph_d, disp_lin_trans_grph_e, disp_lin_trans_grph_f;
    uint32_t tmp, crtc2_h_total_disp, crtc2_v_total_disp;
    uint32_t crtc2_h_sync_strt_wid, crtc2_v_sync_strt_wid;
    RADEONMonitorType found = MT_NONE;
    int connected = 0;
    int i = 0;

    /* save the regs we need */
    gpio_monid = INREG(RADEON_GPIO_MONID);
    fp2_gen_cntl = INREG(RADEON_FP2_GEN_CNTL);
    disp_output_cntl = INREG(RADEON_DISP_OUTPUT_CNTL);
    crtc2_gen_cntl = INREG(RADEON_CRTC2_GEN_CNTL);
    disp_lin_trans_grph_a = INREG(RADEON_DISP_LIN_TRANS_GRPH_A);
    disp_lin_trans_grph_b = INREG(RADEON_DISP_LIN_TRANS_GRPH_B);
    disp_lin_trans_grph_c = INREG(RADEON_DISP_LIN_TRANS_GRPH_C);
    disp_lin_trans_grph_d = INREG(RADEON_DISP_LIN_TRANS_GRPH_D);
    disp_lin_trans_grph_e = INREG(RADEON_DISP_LIN_TRANS_GRPH_E);
    disp_lin_trans_grph_f = INREG(RADEON_DISP_LIN_TRANS_GRPH_F);
    crtc2_h_total_disp = INREG(RADEON_CRTC2_H_TOTAL_DISP);
    crtc2_v_total_disp = INREG(RADEON_CRTC2_V_TOTAL_DISP);
    crtc2_h_sync_strt_wid = INREG(RADEON_CRTC2_H_SYNC_STRT_WID);
    crtc2_v_sync_strt_wid = INREG(RADEON_CRTC2_V_SYNC_STRT_WID);

    tmp = INREG(RADEON_GPIO_MONID);
    tmp &= ~RADEON_GPIO_A_0;
    OUTREG(RADEON_GPIO_MONID, tmp);

    OUTREG(RADEON_FP2_GEN_CNTL,
	   RADEON_FP2_ON |
	   RADEON_FP2_PANEL_FORMAT |
	   R200_FP2_SOURCE_SEL_TRANS_UNIT |
	   RADEON_FP2_DVO_EN |
	   R200_FP2_DVO_RATE_SEL_SDR);

    OUTREG(RADEON_DISP_OUTPUT_CNTL,
	   RADEON_DISP_DAC_SOURCE_RMX |
	   RADEON_DISP_TRANS_MATRIX_GRAPHICS);

    OUTREG(RADEON_CRTC2_GEN_CNTL,
	   RADEON_CRTC2_EN |
	   RADEON_CRTC2_DISP_REQ_EN_B);

    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_A, 0x00000000);
    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_B, 0x000003f0);
    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_C, 0x00000000);
    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_D, 0x000003f0);
    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_E, 0x00000000);
    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_F, 0x000003f0);

    OUTREG(RADEON_CRTC2_H_TOTAL_DISP, 0x01000008);
    OUTREG(RADEON_CRTC2_H_SYNC_STRT_WID, 0x00000800);
    OUTREG(RADEON_CRTC2_V_TOTAL_DISP, 0x00080001);
    OUTREG(RADEON_CRTC2_V_SYNC_STRT_WID, 0x00000080);

    for (i = 0; i < 200; i++) {
	tmp = INREG(RADEON_GPIO_MONID);
	if (tmp & RADEON_GPIO_Y_0)
	    connected = 1;
	else
	    connected = 0;

	if (!connected)
	    break;

	usleep(1000);
    }

    if (connected)
	found = MT_CRT;

    /* restore the regs we used */
    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_A, disp_lin_trans_grph_a);
    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_B, disp_lin_trans_grph_b);
    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_C, disp_lin_trans_grph_c);
    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_D, disp_lin_trans_grph_d);
    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_E, disp_lin_trans_grph_e);
    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_F, disp_lin_trans_grph_f);
    OUTREG(RADEON_CRTC2_H_TOTAL_DISP, crtc2_h_total_disp);
    OUTREG(RADEON_CRTC2_V_TOTAL_DISP, crtc2_v_total_disp);
    OUTREG(RADEON_CRTC2_H_SYNC_STRT_WID, crtc2_h_sync_strt_wid);
    OUTREG(RADEON_CRTC2_V_SYNC_STRT_WID, crtc2_v_sync_strt_wid);
    OUTREG(RADEON_CRTC2_GEN_CNTL, crtc2_gen_cntl);
    OUTREG(RADEON_DISP_OUTPUT_CNTL, disp_output_cntl);
    OUTREG(RADEON_FP2_GEN_CNTL, fp2_gen_cntl);
    OUTREG(RADEON_GPIO_MONID, gpio_monid);

    return found;
}

static RADEONMonitorType
radeon_detect_tv_dac(ScrnInfoPtr pScrn, Bool color)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t crtc2_gen_cntl, tv_dac_cntl, dac_cntl2, dac_ext_cntl;
    uint32_t disp_hw_debug, disp_output_cntl, gpiopad_a, pixclks_cntl, tmp;
    RADEONMonitorType found = MT_NONE;

    /* save the regs we need */
    pixclks_cntl = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
    gpiopad_a = IS_R300_VARIANT ? INREG(RADEON_GPIOPAD_A) : 0;
    disp_output_cntl = IS_R300_VARIANT ? INREG(RADEON_DISP_OUTPUT_CNTL) : 0;
    disp_hw_debug = !IS_R300_VARIANT ? INREG(RADEON_DISP_HW_DEBUG) : 0;
    crtc2_gen_cntl = INREG(RADEON_CRTC2_GEN_CNTL);
    tv_dac_cntl = INREG(RADEON_TV_DAC_CNTL);
    dac_ext_cntl = INREG(RADEON_DAC_EXT_CNTL);
    dac_cntl2 = INREG(RADEON_DAC_CNTL2);

    tmp = pixclks_cntl & ~(RADEON_PIX2CLK_ALWAYS_ONb
			   | RADEON_PIX2CLK_DAC_ALWAYS_ONb);
    OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmp);

    if (IS_R300_VARIANT) {
	OUTREGP(RADEON_GPIOPAD_A, 1, ~1 );
    }

    tmp = crtc2_gen_cntl & ~RADEON_CRTC2_PIX_WIDTH_MASK;
    tmp |= RADEON_CRTC2_CRT2_ON |
	(2 << RADEON_CRTC2_PIX_WIDTH_SHIFT);

    OUTREG(RADEON_CRTC2_GEN_CNTL, tmp);

    if (IS_R300_VARIANT) {
	tmp = disp_output_cntl & ~RADEON_DISP_TVDAC_SOURCE_MASK;
	tmp |= RADEON_DISP_TVDAC_SOURCE_CRTC2;
	OUTREG(RADEON_DISP_OUTPUT_CNTL, tmp);
    } else {
	tmp = disp_hw_debug & ~RADEON_CRT2_DISP1_SEL;
	OUTREG(RADEON_DISP_HW_DEBUG, tmp);
    }

    tmp = RADEON_TV_DAC_NBLANK |
	RADEON_TV_DAC_NHOLD |
	RADEON_TV_MONITOR_DETECT_EN |
	RADEON_TV_DAC_STD_PS2;

    OUTREG(RADEON_TV_DAC_CNTL, tmp);

    tmp = RADEON_DAC2_FORCE_BLANK_OFF_EN |
	RADEON_DAC2_FORCE_DATA_EN;

    if (color)
	tmp |= RADEON_DAC_FORCE_DATA_SEL_RGB;
    else
	tmp |= RADEON_DAC_FORCE_DATA_SEL_G;

    if (IS_R300_VARIANT)
	tmp |= (0x1b6 << RADEON_DAC_FORCE_DATA_SHIFT);
    else
	tmp |= (0x180 << RADEON_DAC_FORCE_DATA_SHIFT);

    OUTREG(RADEON_DAC_EXT_CNTL, tmp);

    tmp = dac_cntl2 | RADEON_DAC2_DAC2_CLK_SEL | RADEON_DAC2_CMP_EN;
    OUTREG(RADEON_DAC_CNTL2, tmp);

    usleep(10000);

    if (IS_R300_VARIANT) {
	if (INREG(RADEON_DAC_CNTL2) & RADEON_DAC2_CMP_OUT_B) {
	    found = MT_CRT;
	    xf86DrvMsg (pScrn->scrnIndex, X_INFO,
			"Found %s CRT connected to TV DAC\n",
			color ? "color" : "bw");
	}
    } else {
	if (INREG(RADEON_DAC_CNTL2) & RADEON_DAC2_CMP_OUTPUT) {
	    found = MT_CRT;
	    xf86DrvMsg (pScrn->scrnIndex, X_INFO,
			"Found %s CRT connected to TV DAC\n",
			color ? "color" : "bw");
	}
    }

    /* restore regs we used */
    OUTREG(RADEON_DAC_CNTL2, dac_cntl2);
    OUTREG(RADEON_DAC_EXT_CNTL, dac_ext_cntl);
    OUTREG(RADEON_TV_DAC_CNTL, tv_dac_cntl);
    OUTREG(RADEON_CRTC2_GEN_CNTL, crtc2_gen_cntl);

    if (IS_R300_VARIANT) {
	OUTREG(RADEON_DISP_OUTPUT_CNTL, disp_output_cntl);
	OUTREGP(RADEON_GPIOPAD_A, gpiopad_a, ~1 );
    } else {
	OUTREG(RADEON_DISP_HW_DEBUG, disp_hw_debug);
    }
    OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, pixclks_cntl);

    return found;
}

static RADEONMonitorType
r300_detect_tv(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t tmp, dac_cntl2, crtc2_gen_cntl, dac_ext_cntl, tv_dac_cntl;
    uint32_t gpiopad_a, disp_output_cntl;
    RADEONMonitorType found = MT_NONE;

    /* save the regs we need */
    gpiopad_a = INREG(RADEON_GPIOPAD_A);
    dac_cntl2 = INREG(RADEON_DAC_CNTL2);
    crtc2_gen_cntl = INREG(RADEON_CRTC2_GEN_CNTL);
    dac_ext_cntl = INREG(RADEON_DAC_EXT_CNTL);
    tv_dac_cntl = INREG(RADEON_TV_DAC_CNTL);
    disp_output_cntl = INREG(RADEON_DISP_OUTPUT_CNTL);

    OUTREGP(RADEON_GPIOPAD_A, 0, ~1 );

    OUTREG(RADEON_DAC_CNTL2, RADEON_DAC2_DAC2_CLK_SEL );

    OUTREG(RADEON_CRTC2_GEN_CNTL,
	   RADEON_CRTC2_CRT2_ON | RADEON_CRTC2_VSYNC_TRISTAT );

    tmp = disp_output_cntl & ~RADEON_DISP_TVDAC_SOURCE_MASK;
    tmp |= RADEON_DISP_TVDAC_SOURCE_CRTC2;
    OUTREG(RADEON_DISP_OUTPUT_CNTL, tmp);

    OUTREG(RADEON_DAC_EXT_CNTL,
	   RADEON_DAC2_FORCE_BLANK_OFF_EN |
	   RADEON_DAC2_FORCE_DATA_EN |
	   RADEON_DAC_FORCE_DATA_SEL_RGB |
	   (0xec << RADEON_DAC_FORCE_DATA_SHIFT ));

    OUTREG(RADEON_TV_DAC_CNTL,
	   RADEON_TV_DAC_STD_NTSC |
	   (8 << RADEON_TV_DAC_BGADJ_SHIFT) |
	   (6 << RADEON_TV_DAC_DACADJ_SHIFT ));

    INREG(RADEON_TV_DAC_CNTL);

    usleep(4000);

    OUTREG(RADEON_TV_DAC_CNTL,
	   RADEON_TV_DAC_NBLANK |
	   RADEON_TV_DAC_NHOLD |
	   RADEON_TV_MONITOR_DETECT_EN |
	   RADEON_TV_DAC_STD_NTSC |
	   (8 << RADEON_TV_DAC_BGADJ_SHIFT) |
	   (6 << RADEON_TV_DAC_DACADJ_SHIFT ));

    INREG(RADEON_TV_DAC_CNTL);

    usleep(6000);

    tmp = INREG(RADEON_TV_DAC_CNTL);
    if ( (tmp & RADEON_TV_DAC_GDACDET) != 0 ) {
	found = MT_STV;
	xf86DrvMsg (pScrn->scrnIndex, X_INFO,
		    "S-Video TV connection detected\n");
    } else if ( (tmp & RADEON_TV_DAC_BDACDET) != 0 ) {
	found = MT_CTV;
	xf86DrvMsg (pScrn->scrnIndex, X_INFO,
		    "Composite TV connection detected\n" );
    }

    OUTREG(RADEON_TV_DAC_CNTL, tv_dac_cntl );
    OUTREG(RADEON_DAC_EXT_CNTL, dac_ext_cntl);
    OUTREG(RADEON_CRTC2_GEN_CNTL, crtc2_gen_cntl);
    OUTREG(RADEON_DISP_OUTPUT_CNTL, disp_output_cntl);
    OUTREG(RADEON_DAC_CNTL2, dac_cntl2);
    OUTREGP(RADEON_GPIOPAD_A, gpiopad_a, ~1);

    return found;
}

static RADEONMonitorType
radeon_detect_tv(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t tmp, dac_cntl2, tv_master_cntl;
    uint32_t tv_dac_cntl, tv_pre_dac_mux_cntl, config_cntl;
    RADEONMonitorType found = MT_NONE;

    if (IS_R300_VARIANT)
	return r300_detect_tv(pScrn);

    /* save the regs we need */
    dac_cntl2 = INREG(RADEON_DAC_CNTL2);
    tv_master_cntl = INREG(RADEON_TV_MASTER_CNTL);
    tv_dac_cntl = INREG(RADEON_TV_DAC_CNTL);
    config_cntl = INREG(RADEON_CONFIG_CNTL);
    tv_pre_dac_mux_cntl = INREG(RADEON_TV_PRE_DAC_MUX_CNTL);

    tmp = dac_cntl2 & ~RADEON_DAC2_DAC2_CLK_SEL;
    OUTREG(RADEON_DAC_CNTL2, tmp);

    tmp = tv_master_cntl | RADEON_TV_ON;
    tmp &= ~(RADEON_TV_ASYNC_RST |
	     RADEON_RESTART_PHASE_FIX |
	     RADEON_CRT_FIFO_CE_EN |
	     RADEON_TV_FIFO_CE_EN |
	     RADEON_RE_SYNC_NOW_SEL_MASK);
    tmp |= RADEON_TV_FIFO_ASYNC_RST | RADEON_CRT_ASYNC_RST;

    OUTREG(RADEON_TV_MASTER_CNTL, tmp);

    tmp = RADEON_TV_DAC_NBLANK | RADEON_TV_DAC_NHOLD |
	RADEON_TV_MONITOR_DETECT_EN | RADEON_TV_DAC_STD_NTSC |
	(8 << RADEON_TV_DAC_BGADJ_SHIFT);

    if (config_cntl & RADEON_CFG_ATI_REV_ID_MASK)
	tmp |= (4 << RADEON_TV_DAC_DACADJ_SHIFT);
    else
	tmp |= (8 << RADEON_TV_DAC_DACADJ_SHIFT);

    OUTREG(RADEON_TV_DAC_CNTL, tmp);

    tmp = RADEON_C_GRN_EN | RADEON_CMP_BLU_EN |
	RADEON_RED_MX_FORCE_DAC_DATA |
	RADEON_GRN_MX_FORCE_DAC_DATA |
	RADEON_BLU_MX_FORCE_DAC_DATA |
	(0x109 << RADEON_TV_FORCE_DAC_DATA_SHIFT);

    OUTREG(RADEON_TV_PRE_DAC_MUX_CNTL, tmp);

    usleep(3000);

    tmp = INREG(RADEON_TV_DAC_CNTL);
    if (tmp & RADEON_TV_DAC_GDACDET) {
	found = MT_STV;
	xf86DrvMsg (pScrn->scrnIndex, X_INFO,
		    "S-Video TV connection detected\n");
    } else if (tmp & RADEON_TV_DAC_BDACDET) {
	found = MT_CTV;
	xf86DrvMsg (pScrn->scrnIndex, X_INFO,
		    "Composite TV connection detected\n" );
    }

    OUTREG(RADEON_TV_PRE_DAC_MUX_CNTL, tv_pre_dac_mux_cntl);
    OUTREG(RADEON_TV_DAC_CNTL, tv_dac_cntl);
    OUTREG(RADEON_TV_MASTER_CNTL, tv_master_cntl);
    OUTREG(RADEON_DAC_CNTL2, dac_cntl2);

    return found;
}
