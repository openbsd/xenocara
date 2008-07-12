 /*
 * Copyright © 2007 Red Hat, Inc.
 * Copyright 2007  Advanced Micro Devices, Inc.
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
 *    Dave Airlie <airlied@redhat.com>
 *    Alex Deucher <alexander.deucher@amd.com>
 *
 */
/*
 * avivo crtc handling functions.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
/* DPMS */
#define DPMS_SERVER
#include <X11/extensions/dpms.h>

#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_atombios.h"

#ifdef XF86DRI
#define _XF86DRI_SERVER_
#include "radeon_dri.h"
#include "radeon_sarea.h"
#include "sarea.h"
#endif

static AtomBiosResult
atombios_enable_crtc(atomBiosHandlePtr atomBIOS, int crtc, int state)
{
    ENABLE_CRTC_PS_ALLOCATION crtc_data;
    AtomBiosArgRec data;
    unsigned char *space;

    crtc_data.ucCRTC = crtc;
    crtc_data.ucEnable = state;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, EnableCRTC);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &crtc_data;

    if (RHDAtomBiosFunc(atomBIOS->scrnIndex, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("%s CRTC %d success\n", state? "Enable":"Disable", crtc);
	return ATOM_SUCCESS ;
    }

    ErrorF("Enable CRTC failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

static AtomBiosResult
atombios_enable_crtc_memreq(atomBiosHandlePtr atomBIOS, int crtc, int state)
{
    ENABLE_CRTC_PS_ALLOCATION crtc_data;
    AtomBiosArgRec data;
    unsigned char *space;

    crtc_data.ucCRTC = crtc;
    crtc_data.ucEnable = state;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, EnableCRTCMemReq);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &crtc_data;

    if (RHDAtomBiosFunc(atomBIOS->scrnIndex, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("%s CRTC memreq %d success\n", state? "Enable":"Disable", crtc);
	return ATOM_SUCCESS ;
    }

    ErrorF("Enable CRTC memreq failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

static AtomBiosResult
atombios_blank_crtc(atomBiosHandlePtr atomBIOS, int crtc, int state)
{
    BLANK_CRTC_PS_ALLOCATION crtc_data;
    unsigned char *space;
    AtomBiosArgRec data;

    memset(&crtc_data, 0, sizeof(crtc_data));
    crtc_data.ucCRTC = crtc;
    crtc_data.ucBlanking = state;

    data.exec.index = offsetof(ATOM_MASTER_LIST_OF_COMMAND_TABLES, BlankCRTC) / sizeof(unsigned short);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &crtc_data;

    if (RHDAtomBiosFunc(atomBIOS->scrnIndex, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("%s CRTC %d success\n", state? "Blank":"Unblank", crtc);
	return ATOM_SUCCESS ;
    }

    ErrorF("Blank CRTC failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

void
atombios_crtc_dpms(xf86CrtcPtr crtc, int mode)
{
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr  info = RADEONPTR(crtc->scrn);
    switch (mode) {
    case DPMSModeOn:
    case DPMSModeStandby:
    case DPMSModeSuspend:
	if (IS_DCE3_VARIANT)
	    atombios_enable_crtc_memreq(info->atomBIOS, radeon_crtc->crtc_id, 1);
	atombios_enable_crtc(info->atomBIOS, radeon_crtc->crtc_id, 1);
	atombios_blank_crtc(info->atomBIOS, radeon_crtc->crtc_id, 0);
	break;
    case DPMSModeOff:
	atombios_blank_crtc(info->atomBIOS, radeon_crtc->crtc_id, 1);
	atombios_enable_crtc(info->atomBIOS, radeon_crtc->crtc_id, 0);
	if (IS_DCE3_VARIANT)
	    atombios_enable_crtc_memreq(info->atomBIOS, radeon_crtc->crtc_id, 0);
	break;
    }
}

static AtomBiosResult
atombios_set_crtc_timing(atomBiosHandlePtr atomBIOS, SET_CRTC_TIMING_PARAMETERS_PS_ALLOCATION *crtc_param)
{
    AtomBiosArgRec data;
    unsigned char *space;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, SetCRTC_Timing);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = crtc_param;

    if (RHDAtomBiosFunc(atomBIOS->scrnIndex, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Set CRTC Timing success\n");
	return ATOM_SUCCESS ;
    }

    ErrorF("Set CRTC Timing failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

void
atombios_crtc_set_pll(xf86CrtcPtr crtc, DisplayModePtr mode, int pll_flags)
{
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr  info = RADEONPTR(crtc->scrn);
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int index = GetIndexIntoMasterTable(COMMAND, SetPixelClock);
    uint32_t sclock = mode->Clock;
    uint32_t ref_div = 0, fb_div = 0, post_div = 0;
    int major, minor, i;
    SET_PIXEL_CLOCK_PS_ALLOCATION spc_param;
    PIXEL_CLOCK_PARAMETERS_V2 *spc2_ptr;
    PIXEL_CLOCK_PARAMETERS_V3 *spc3_ptr;
    xf86OutputPtr output;
    RADEONOutputPrivatePtr radeon_output = NULL;

    void *ptr;
    AtomBiosArgRec data;
    unsigned char *space;
    RADEONSavePtr save = info->ModeReg;

    if (IS_AVIVO_VARIANT) {
	uint32_t temp;

	pll_flags |= RADEON_PLL_PREFER_LOW_REF_DIV;

	RADEONComputePLL(&info->pll, mode->Clock, &temp, &fb_div, &ref_div, &post_div, pll_flags);
	sclock = temp;

	/* disable spread spectrum clocking for now -- thanks Hedy Lamarr */
	if (radeon_crtc->crtc_id == 0) {
	    temp = INREG(AVIVO_P1PLL_INT_SS_CNTL);
	    OUTREG(AVIVO_P1PLL_INT_SS_CNTL, temp & ~1);
	} else {
	    temp = INREG(AVIVO_P2PLL_INT_SS_CNTL);
	    OUTREG(AVIVO_P2PLL_INT_SS_CNTL, temp & ~1);
	}
    } else {
	sclock = save->dot_clock_freq;
	fb_div = save->feedback_div;
	post_div = save->post_div;
	ref_div = save->ppll_ref_div;
    }

    xf86DrvMsg(crtc->scrn->scrnIndex, X_INFO,
	       "crtc(%d) Clock: mode %d, PLL %lu\n",
	       radeon_crtc->crtc_id, mode->Clock, (long unsigned int)sclock * 10);
    xf86DrvMsg(crtc->scrn->scrnIndex, X_INFO,
	       "crtc(%d) PLL  : refdiv %u, fbdiv 0x%X(%u), pdiv %u\n",
	       radeon_crtc->crtc_id, (unsigned int)ref_div, (unsigned int)fb_div, (unsigned int)fb_div, (unsigned int)post_div);

    /* Can't really do cloning easily on DCE3 cards */
    for (i = 0; i < xf86_config->num_output; i++) {
	output = xf86_config->output[i];
	if (output->crtc == crtc) {
	    radeon_output = output->driver_private;
	    break;
	}
    }

    if (radeon_output == NULL) {
	xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR, "No output assigned to crtc!\n");
	return;
    }

    atombios_get_command_table_version(info->atomBIOS, index, &major, &minor);

    /*ErrorF("table is %d %d\n", major, minor);*/
    switch(major) {
    case 1:
	switch(minor) {
	case 1:
	case 2:
	    spc2_ptr = (PIXEL_CLOCK_PARAMETERS_V2*)&spc_param.sPCLKInput;
	    spc2_ptr->usPixelClock = sclock;
	    spc2_ptr->usRefDiv = ref_div;
	    spc2_ptr->usFbDiv = fb_div;
	    spc2_ptr->ucPostDiv = post_div;
	    spc2_ptr->ucPpll = radeon_crtc->crtc_id ? ATOM_PPLL2 : ATOM_PPLL1;
	    spc2_ptr->ucCRTC = radeon_crtc->crtc_id;
	    spc2_ptr->ucRefDivSrc = 1;
	    ptr = &spc_param;
	    break;
	case 3:
	    spc3_ptr = (PIXEL_CLOCK_PARAMETERS_V3*)&spc_param.sPCLKInput;
	    spc3_ptr->usPixelClock = sclock;
	    spc3_ptr->usRefDiv = ref_div;
	    spc3_ptr->usFbDiv = fb_div;
	    spc3_ptr->ucPostDiv = post_div;
	    spc3_ptr->ucPpll = radeon_crtc->crtc_id ? ATOM_PPLL2 : ATOM_PPLL1;
	    spc3_ptr->ucMiscInfo = (radeon_crtc->crtc_id << 2);

	    if (radeon_output->MonType == MT_CRT) {
		if (radeon_output->DACType == DAC_PRIMARY)
		    spc3_ptr->ucTransmitterId = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1;
		else if (radeon_output->DACType == DAC_TVDAC)
		    spc3_ptr->ucTransmitterId = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2;
		spc3_ptr->ucEncoderMode = ATOM_ENCODER_MODE_CRT;
	    } else if (radeon_output->MonType == MT_DFP) {
		if (radeon_output->devices & ATOM_DEVICE_DFP1_SUPPORT)
		    spc3_ptr->ucTransmitterId = ENCODER_OBJECT_ID_INTERNAL_UNIPHY;
		else if (radeon_output->devices & ATOM_DEVICE_DFP2_SUPPORT)
		    spc3_ptr->ucTransmitterId = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DVO1;
		else if (radeon_output->devices & ATOM_DEVICE_DFP3_SUPPORT)
		    spc3_ptr->ucTransmitterId = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA;
		if (OUTPUT_IS_DVI)
		    spc3_ptr->ucEncoderMode = ATOM_ENCODER_MODE_DVI;
		else if (radeon_output->type == OUTPUT_HDMI)
		    spc3_ptr->ucEncoderMode = ATOM_ENCODER_MODE_HDMI;
		else if (radeon_output->type == OUTPUT_DP)
		    spc3_ptr->ucEncoderMode = ATOM_ENCODER_MODE_DP;
	    } else if (radeon_output->MonType == MT_LCD) {
		if (radeon_output->devices & ATOM_DEVICE_LCD1_SUPPORT)
		    spc3_ptr->ucTransmitterId = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA;
		spc3_ptr->ucEncoderMode = ATOM_ENCODER_MODE_LVDS;
	    } else if (OUTPUT_IS_TV) {
		if (radeon_output->DACType == DAC_PRIMARY)
		    spc3_ptr->ucTransmitterId = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1;
		else if (radeon_output->DACType == DAC_TVDAC)
		    spc3_ptr->ucTransmitterId = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2;
		spc3_ptr->ucEncoderMode = ATOM_ENCODER_MODE_TV;
	    } else if (radeon_output->MonType == MT_CV) {
		if (radeon_output->DACType == DAC_PRIMARY)
		    spc3_ptr->ucTransmitterId = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1;
		else if (radeon_output->DACType == DAC_TVDAC)
		    spc3_ptr->ucTransmitterId = ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2;
		spc3_ptr->ucEncoderMode = ATOM_ENCODER_MODE_CV;
	    }

	    ptr = &spc_param;
	    break;
	default:
	    ErrorF("Unknown table version\n");
	    exit(-1);
	}
	break;
    default:
	ErrorF("Unknown table version\n");
	exit(-1);
    }

    data.exec.index = GetIndexIntoMasterTable(COMMAND, SetPixelClock);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = ptr;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Set CRTC PLL success\n");
	return;
    }

    ErrorF("Set CRTC PLL failed\n");
    return;
}

void
atombios_crtc_mode_set(xf86CrtcPtr crtc,
		       DisplayModePtr mode,
		       DisplayModePtr adjusted_mode,
		       int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    unsigned long fb_location = crtc->scrn->fbOffset + info->fbLocation;
    int need_tv_timings = 0;
    int i, ret;
    SET_CRTC_TIMING_PARAMETERS_PS_ALLOCATION crtc_timing;
    Bool tilingChanged = FALSE;
    int pll_flags = 0;
    memset(&crtc_timing, 0, sizeof(crtc_timing));

    if (info->allowColorTiling) {
        radeon_crtc->can_tile = (adjusted_mode->Flags & (V_DBLSCAN | V_INTERLACE)) ? FALSE : TRUE;
	tilingChanged = RADEONSetTiling(pScrn);
    }

    for (i = 0; i < xf86_config->num_output; i++) {
	xf86OutputPtr output = xf86_config->output[i];
	RADEONOutputPrivatePtr radeon_output = output->driver_private;

	if (output->crtc == crtc) {
	    if (radeon_output->MonType == MT_STV || radeon_output->MonType == MT_CTV) {
		if (radeon_output->tvStd == TV_STD_NTSC ||
		    radeon_output->tvStd == TV_STD_NTSC_J ||
		    radeon_output->tvStd == TV_STD_PAL_M)
		    need_tv_timings = 1;
		else
		    need_tv_timings = 2;

	    }

	    if (radeon_output->MonType == MT_LCD)
	      pll_flags |= RADEON_PLL_USE_REF_DIV;
	}
    }

    crtc_timing.ucCRTC = radeon_crtc->crtc_id;
    if (need_tv_timings) {
	ret = RADEONATOMGetTVTimings(pScrn, need_tv_timings - 1, &crtc_timing, &adjusted_mode->Clock);
	if (ret == FALSE) {
	    need_tv_timings = 0;
	} else {
	    adjusted_mode->CrtcHDisplay = crtc_timing.usH_Disp;
	    adjusted_mode->CrtcHTotal = crtc_timing.usH_Total;
	    adjusted_mode->CrtcVDisplay = crtc_timing.usV_Disp;
	    adjusted_mode->CrtcVTotal = crtc_timing.usV_Total;
	}
    }

    if (!need_tv_timings) {
	crtc_timing.usH_Total = adjusted_mode->CrtcHTotal;
	crtc_timing.usH_Disp = adjusted_mode->CrtcHDisplay;
	crtc_timing.usH_SyncStart = adjusted_mode->CrtcHSyncStart;
	crtc_timing.usH_SyncWidth = adjusted_mode->CrtcHSyncEnd - adjusted_mode->CrtcHSyncStart;

	crtc_timing.usV_Total = adjusted_mode->CrtcVTotal;
	crtc_timing.usV_Disp = adjusted_mode->CrtcVDisplay;
	crtc_timing.usV_SyncStart = adjusted_mode->CrtcVSyncStart;
	crtc_timing.usV_SyncWidth = adjusted_mode->CrtcVSyncEnd - adjusted_mode->CrtcVSyncStart;

	if (adjusted_mode->Flags & V_NVSYNC)
	    crtc_timing.susModeMiscInfo.usAccess |= ATOM_VSYNC_POLARITY;

	if (adjusted_mode->Flags & V_NHSYNC)
	    crtc_timing.susModeMiscInfo.usAccess |= ATOM_HSYNC_POLARITY;

	if (adjusted_mode->Flags & V_CSYNC)
	    crtc_timing.susModeMiscInfo.usAccess |= ATOM_COMPOSITESYNC;

	if (adjusted_mode->Flags & V_INTERLACE)
	    crtc_timing.susModeMiscInfo.usAccess |= ATOM_INTERLACE;

	if (adjusted_mode->Flags & V_DBLSCAN)
	    crtc_timing.susModeMiscInfo.usAccess |= ATOM_DOUBLE_CLOCK_MODE;

    }

    ErrorF("Mode %dx%d - %d %d %d\n", adjusted_mode->CrtcHDisplay, adjusted_mode->CrtcVDisplay,
	   adjusted_mode->CrtcHTotal, adjusted_mode->CrtcVTotal, adjusted_mode->Flags);

    RADEONInitMemMapRegisters(pScrn, info->ModeReg, info);
    RADEONRestoreMemMapRegisters(pScrn, info->ModeReg);

    if (IS_AVIVO_VARIANT) {
	uint32_t fb_format;

	switch (crtc->scrn->bitsPerPixel) {
	case 15:
	    fb_format = AVIVO_D1GRPH_CONTROL_DEPTH_16BPP | AVIVO_D1GRPH_CONTROL_16BPP_ARGB1555;
	    break;
	case 16:
	    fb_format = AVIVO_D1GRPH_CONTROL_DEPTH_16BPP | AVIVO_D1GRPH_CONTROL_16BPP_RGB565;
	    break;
	case 24:
	case 32:
	    fb_format = AVIVO_D1GRPH_CONTROL_DEPTH_32BPP | AVIVO_D1GRPH_CONTROL_32BPP_ARGB8888;
	    break;
	default:
	    FatalError("Unsupported screen depth: %d\n", xf86GetDepth());
	}

	if (info->tilingEnabled && (crtc->rotatedData == NULL)) {
	    fb_format |= AVIVO_D1GRPH_MACRO_ADDRESS_MODE;
	}

	if (radeon_crtc->crtc_id == 0)
	    OUTREG(AVIVO_D1VGA_CONTROL, 0);
	else
	    OUTREG(AVIVO_D2VGA_CONTROL, 0);

	/* setup fb format and location
	 */
	if (crtc->rotatedData != NULL) {
	    /* x/y offset is already included */
	    x = 0;
	    y = 0;
	    fb_location = fb_location + (char *)crtc->rotatedData - (char *)info->FB;
	}

	/* lock the grph regs */
	OUTREG(AVIVO_D1GRPH_UPDATE + radeon_crtc->crtc_offset, AVIVO_D1GRPH_UPDATE_LOCK);

	OUTREG(AVIVO_D1GRPH_PRIMARY_SURFACE_ADDRESS + radeon_crtc->crtc_offset, fb_location);
	OUTREG(AVIVO_D1GRPH_SECONDARY_SURFACE_ADDRESS + radeon_crtc->crtc_offset, fb_location);
	OUTREG(AVIVO_D1GRPH_CONTROL + radeon_crtc->crtc_offset, fb_format);

	OUTREG(AVIVO_D1GRPH_SURFACE_OFFSET_X + radeon_crtc->crtc_offset, 0);
	OUTREG(AVIVO_D1GRPH_SURFACE_OFFSET_Y + radeon_crtc->crtc_offset, 0);
	OUTREG(AVIVO_D1GRPH_X_START + radeon_crtc->crtc_offset, x);
	OUTREG(AVIVO_D1GRPH_Y_START + radeon_crtc->crtc_offset, y);
	OUTREG(AVIVO_D1GRPH_X_END + radeon_crtc->crtc_offset, x + mode->HDisplay);
	OUTREG(AVIVO_D1GRPH_Y_END + radeon_crtc->crtc_offset, y + mode->VDisplay);
	OUTREG(AVIVO_D1GRPH_PITCH + radeon_crtc->crtc_offset,
	       crtc->scrn->displayWidth);
	OUTREG(AVIVO_D1GRPH_ENABLE + radeon_crtc->crtc_offset, 1);

	/* unlock the grph regs */
	OUTREG(AVIVO_D1GRPH_UPDATE + radeon_crtc->crtc_offset, 0);

	/* lock the mode regs */
	OUTREG(AVIVO_D1SCL_UPDATE + radeon_crtc->crtc_offset, AVIVO_D1SCL_UPDATE_LOCK);

	OUTREG(AVIVO_D1MODE_DESKTOP_HEIGHT + radeon_crtc->crtc_offset,
	       		mode->VDisplay);
	OUTREG(AVIVO_D1MODE_VIEWPORT_START + radeon_crtc->crtc_offset, (x << 16) | y);
	OUTREG(AVIVO_D1MODE_VIEWPORT_SIZE + radeon_crtc->crtc_offset,
	       (mode->HDisplay << 16) | mode->VDisplay);
	/* unlock the mode regs */
	OUTREG(AVIVO_D1SCL_UPDATE + radeon_crtc->crtc_offset, 0);

    }

    atombios_crtc_set_pll(crtc, adjusted_mode, pll_flags);

    atombios_set_crtc_timing(info->atomBIOS, &crtc_timing);

    if (tilingChanged) {
	/* need to redraw front buffer, I guess this can be considered a hack ? */
	/* if this is called during ScreenInit() we don't have pScrn->pScreen yet */
	if (pScrn->pScreen)
	    xf86EnableDisableFBAccess(pScrn->scrnIndex, FALSE);
	RADEONChangeSurfaces(pScrn);
	if (pScrn->pScreen)
	    xf86EnableDisableFBAccess(pScrn->scrnIndex, TRUE);
	/* xf86SetRootClip would do, but can't access that here */
    }

}

