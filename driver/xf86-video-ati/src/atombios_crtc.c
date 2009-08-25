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
#include "radeon_drm.h"
#include "sarea.h"
#endif

extern int
atombios_get_encoder_mode(xf86OutputPtr output);

extern void
RADEONInitCrtcBase(xf86CrtcPtr crtc, RADEONSavePtr save,
		   int x, int y);
extern void
RADEONInitCrtc2Base(xf86CrtcPtr crtc, RADEONSavePtr save,
		    int x, int y);
extern void
RADEONRestoreCrtcBase(ScrnInfoPtr pScrn,
		      RADEONSavePtr restore);
extern void
RADEONRestoreCrtc2Base(ScrnInfoPtr pScrn,
		       RADEONSavePtr restore);
extern void
RADEONInitCommonRegisters(RADEONSavePtr save, RADEONInfoPtr info);
extern void
RADEONInitSurfaceCntl(xf86CrtcPtr crtc, RADEONSavePtr save);

AtomBiosResult
atombios_lock_crtc(atomBiosHandlePtr atomBIOS, int crtc, int lock)
{
    ENABLE_CRTC_PS_ALLOCATION crtc_data;
    AtomBiosArgRec data;
    unsigned char *space;

    crtc_data.ucCRTC = crtc;
    crtc_data.ucEnable = lock;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, UpdateCRTC_DoubleBufferRegisters);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &crtc_data;

    if (RHDAtomBiosFunc(atomBIOS->scrnIndex, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("%s CRTC %d success\n", lock? "Lock":"Unlock", crtc);
	return ATOM_SUCCESS ;
    }

    ErrorF("Lock CRTC failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

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

    data.exec.index = GetIndexIntoMasterTable(COMMAND, BlankCRTC);
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
    SET_CRTC_TIMING_PARAMETERS_PS_ALLOCATION conv_param;

    conv_param.usH_Total		= cpu_to_le16(crtc_param->usH_Total);
    conv_param.usH_Disp			= cpu_to_le16(crtc_param->usH_Disp);
    conv_param.usH_SyncStart		= cpu_to_le16(crtc_param->usH_SyncStart);
    conv_param.usH_SyncWidth		= cpu_to_le16(crtc_param->usH_SyncWidth);
    conv_param.usV_Total		= cpu_to_le16(crtc_param->usV_Total);
    conv_param.usV_Disp			= cpu_to_le16(crtc_param->usV_Disp);
    conv_param.usV_SyncStart		= cpu_to_le16(crtc_param->usV_SyncStart);
    conv_param.usV_SyncWidth		= cpu_to_le16(crtc_param->usV_SyncWidth);
    conv_param.susModeMiscInfo.usAccess = cpu_to_le16(crtc_param->susModeMiscInfo.usAccess);
    conv_param.ucCRTC			= crtc_param->ucCRTC;
    conv_param.ucOverscanRight		= crtc_param->ucOverscanRight;
    conv_param.ucOverscanLeft		= crtc_param->ucOverscanLeft;
    conv_param.ucOverscanBottom		= crtc_param->ucOverscanBottom;
    conv_param.ucOverscanTop		= crtc_param->ucOverscanTop;
    conv_param.ucReserved		= crtc_param->ucReserved;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, SetCRTC_Timing);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &conv_param;

    if (RHDAtomBiosFunc(atomBIOS->scrnIndex, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Set CRTC Timing success\n");
	return ATOM_SUCCESS ;
    }

    ErrorF("Set CRTC Timing failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

static AtomBiosResult
atombios_set_crtc_dtd_timing(atomBiosHandlePtr atomBIOS, SET_CRTC_USING_DTD_TIMING_PARAMETERS *crtc_param)
{
    AtomBiosArgRec data;
    unsigned char *space;
    SET_CRTC_USING_DTD_TIMING_PARAMETERS conv_param;

    conv_param.usH_Size        = cpu_to_le16(crtc_param->usH_Size);
    conv_param.usH_Blanking_Time= cpu_to_le16(crtc_param->usH_Blanking_Time);
    conv_param.usV_Size        = cpu_to_le16(crtc_param->usV_Size);
    conv_param.usV_Blanking_Time= cpu_to_le16(crtc_param->usV_Blanking_Time);
    conv_param.usH_SyncOffset= cpu_to_le16(crtc_param->usH_SyncOffset);
    conv_param.usH_SyncWidth= cpu_to_le16(crtc_param->usH_SyncWidth);
    conv_param.usV_SyncOffset= cpu_to_le16(crtc_param->usV_SyncOffset);
    conv_param.usV_SyncWidth= cpu_to_le16(crtc_param->usV_SyncWidth);
    conv_param.susModeMiscInfo.usAccess = cpu_to_le16(crtc_param->susModeMiscInfo.usAccess);
    conv_param.ucCRTC= crtc_param->ucCRTC;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, SetCRTC_UsingDTDTiming);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &conv_param;

    if (RHDAtomBiosFunc(atomBIOS->scrnIndex, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Set DTD CRTC Timing success\n");
	return ATOM_SUCCESS ;
    }

    ErrorF("Set DTD CRTC Timing failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

void
atombios_crtc_set_pll(xf86CrtcPtr crtc, DisplayModePtr mode)
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
    radeon_encoder_ptr radeon_encoder = NULL;
    int pll_flags = 0;
    uint32_t temp;

    void *ptr;
    AtomBiosArgRec data;
    unsigned char *space;

    memset(&spc_param, 0, sizeof(spc_param));
    if (IS_AVIVO_VARIANT) {
	if (IS_DCE3_VARIANT && mode->Clock > 200000) /* range limits??? */
	    pll_flags |= RADEON_PLL_PREFER_HIGH_FB_DIV;
	else
	    pll_flags |= RADEON_PLL_PREFER_LOW_REF_DIV;

	/* disable spread spectrum clocking for now -- thanks Hedy Lamarr */
	if (radeon_crtc->crtc_id == 0) {
	    temp = INREG(AVIVO_P1PLL_INT_SS_CNTL);
	    OUTREG(AVIVO_P1PLL_INT_SS_CNTL, temp & ~1);
	} else {
	    temp = INREG(AVIVO_P2PLL_INT_SS_CNTL);
	    OUTREG(AVIVO_P2PLL_INT_SS_CNTL, temp & ~1);
	}
    } else {
	pll_flags |= RADEON_PLL_LEGACY;

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

	if (mode->Clock > 200000) /* range limits??? */
	    pll_flags |= RADEON_PLL_PREFER_HIGH_FB_DIV;
	else
	    pll_flags |= RADEON_PLL_PREFER_LOW_REF_DIV;
    }

    RADEONComputePLL(&info->pll, mode->Clock, &temp, &fb_div, &ref_div, &post_div, pll_flags);
    sclock = temp;

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
	    radeon_encoder = radeon_get_encoder(output);
	    break;
	}
    }

    if (radeon_output == NULL) {
	xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR, "No output assigned to crtc!\n");
	return;
    }

    if (radeon_encoder == NULL) {
	xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR, "No encoder assigned to output!\n");
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
	    spc2_ptr->usPixelClock = cpu_to_le16(sclock);
	    spc2_ptr->usRefDiv = cpu_to_le16(ref_div);
	    spc2_ptr->usFbDiv = cpu_to_le16(fb_div);
	    spc2_ptr->ucPostDiv = post_div;
	    spc2_ptr->ucPpll = radeon_crtc->crtc_id ? ATOM_PPLL2 : ATOM_PPLL1;
	    spc2_ptr->ucCRTC = radeon_crtc->crtc_id;
	    spc2_ptr->ucRefDivSrc = 1;
	    ptr = &spc_param;
	    break;
	case 3:
	    spc3_ptr = (PIXEL_CLOCK_PARAMETERS_V3*)&spc_param.sPCLKInput;
	    spc3_ptr->usPixelClock = cpu_to_le16(sclock);
	    spc3_ptr->usRefDiv = cpu_to_le16(ref_div);
	    spc3_ptr->usFbDiv = cpu_to_le16(fb_div);
	    spc3_ptr->ucPostDiv = post_div;
	    spc3_ptr->ucPpll = radeon_crtc->crtc_id ? ATOM_PPLL2 : ATOM_PPLL1;
	    spc3_ptr->ucMiscInfo = (radeon_crtc->crtc_id << 2);
	    spc3_ptr->ucTransmitterId = radeon_encoder->encoder_id;
	    spc3_ptr->ucEncoderMode = atombios_get_encoder_mode(output);

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
	ErrorF("Set CRTC %d PLL success\n", radeon_crtc->crtc_id);
	return;
    }

    ErrorF("Set CRTC %d PLL failed\n", radeon_crtc->crtc_id);
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
    SET_CRTC_USING_DTD_TIMING_PARAMETERS crtc_dtd_timing;
    Bool tilingChanged = FALSE;
    memset(&crtc_timing, 0, sizeof(crtc_timing));
    memset(&crtc_dtd_timing, 0, sizeof(crtc_dtd_timing));

    if (info->allowColorTiling) {
        radeon_crtc->can_tile = (adjusted_mode->Flags & (V_DBLSCAN | V_INTERLACE)) ? FALSE : TRUE;
	tilingChanged = RADEONSetTiling(pScrn);
    }

    for (i = 0; i < xf86_config->num_output; i++) {
	xf86OutputPtr output = xf86_config->output[i];
	RADEONOutputPrivatePtr radeon_output = output->driver_private;
	radeon_tvout_ptr tvout = &radeon_output->tvout;

	if (output->crtc == crtc) {
	    if (radeon_output->MonType == MT_STV || radeon_output->MonType == MT_CTV) {
		if (tvout->tvStd == TV_STD_NTSC ||
		    tvout->tvStd == TV_STD_NTSC_J ||
		    tvout->tvStd == TV_STD_PAL_M)
		    need_tv_timings = 1;
		else
		    need_tv_timings = 2;

	    }
	}
    }

    crtc_timing.ucCRTC = radeon_crtc->crtc_id;
    if (need_tv_timings) {
	ret = RADEONATOMGetTVTimings(pScrn, need_tv_timings - 1, &crtc_timing, &adjusted_mode->Clock);
	if (ret == FALSE) {
	    need_tv_timings = 0;
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

	if (!IS_AVIVO_VARIANT && (radeon_crtc->crtc_id == 0)) {
	    crtc_dtd_timing.ucCRTC = radeon_crtc->crtc_id;
	    crtc_dtd_timing.usH_Size = adjusted_mode->CrtcHDisplay;
	    crtc_dtd_timing.usV_Size = adjusted_mode->CrtcVDisplay;
	    crtc_dtd_timing.usH_Blanking_Time = adjusted_mode->CrtcHBlankEnd - adjusted_mode->CrtcHDisplay;
	    crtc_dtd_timing.usV_Blanking_Time = adjusted_mode->CrtcVBlankEnd - adjusted_mode->CrtcVDisplay;
	    crtc_dtd_timing.usH_SyncOffset = adjusted_mode->CrtcHSyncStart - adjusted_mode->CrtcHDisplay;
	    crtc_dtd_timing.usV_SyncOffset = adjusted_mode->CrtcVSyncStart - adjusted_mode->CrtcVDisplay;
	    crtc_dtd_timing.usH_SyncWidth = adjusted_mode->CrtcHSyncEnd - adjusted_mode->CrtcHSyncStart;
	    crtc_dtd_timing.usV_SyncWidth = adjusted_mode->CrtcVSyncEnd - adjusted_mode->CrtcVSyncStart;
	    ErrorF("%d %d %d %d %d %d %d %d\n", crtc_dtd_timing.usH_Size, crtc_dtd_timing.usH_SyncOffset,
		   crtc_dtd_timing.usH_SyncWidth, crtc_dtd_timing.usH_Blanking_Time,
		   crtc_dtd_timing.usV_Size, crtc_dtd_timing.usV_SyncOffset,
		   crtc_dtd_timing.usV_SyncWidth, crtc_dtd_timing.usV_Blanking_Time);

	    if (adjusted_mode->Flags & V_NVSYNC)
		crtc_dtd_timing.susModeMiscInfo.usAccess |= ATOM_VSYNC_POLARITY;

	    if (adjusted_mode->Flags & V_NHSYNC)
		crtc_dtd_timing.susModeMiscInfo.usAccess |= ATOM_HSYNC_POLARITY;

	    if (adjusted_mode->Flags & V_CSYNC)
		crtc_dtd_timing.susModeMiscInfo.usAccess |= ATOM_COMPOSITESYNC;

	    if (adjusted_mode->Flags & V_INTERLACE)
		crtc_dtd_timing.susModeMiscInfo.usAccess |= ATOM_INTERLACE;

	    if (adjusted_mode->Flags & V_DBLSCAN)
		crtc_dtd_timing.susModeMiscInfo.usAccess |= ATOM_DOUBLE_CLOCK_MODE;
	}
    }

    ErrorF("Mode %dx%d - %d %d %d\n", adjusted_mode->CrtcHDisplay, adjusted_mode->CrtcVDisplay,
	   adjusted_mode->CrtcHTotal, adjusted_mode->CrtcVTotal, adjusted_mode->Flags);

    RADEONInitMemMapRegisters(pScrn, info->ModeReg, info);
    RADEONRestoreMemMapRegisters(pScrn, info->ModeReg);

    atombios_crtc_set_pll(crtc, adjusted_mode);
    atombios_set_crtc_timing(info->atomBIOS, &crtc_timing);
    if (!IS_AVIVO_VARIANT && (radeon_crtc->crtc_id == 0))
	atombios_set_crtc_dtd_timing(info->atomBIOS, &crtc_dtd_timing);

    if (IS_AVIVO_VARIANT) {
	uint32_t fb_format;
#if X_BYTE_ORDER == X_BIG_ENDIAN
	uint32_t fb_swap = R600_D1GRPH_SWAP_ENDIAN_NONE;
#endif

	switch (crtc->scrn->bitsPerPixel) {
	case 15:
	    fb_format = AVIVO_D1GRPH_CONTROL_DEPTH_16BPP | AVIVO_D1GRPH_CONTROL_16BPP_ARGB1555;
	    break;
	case 16:
	    fb_format = AVIVO_D1GRPH_CONTROL_DEPTH_16BPP | AVIVO_D1GRPH_CONTROL_16BPP_RGB565;
#if X_BYTE_ORDER == X_BIG_ENDIAN
	    fb_swap = R600_D1GRPH_SWAP_ENDIAN_16BIT;
#endif
	    break;
	case 24:
	case 32:
	    fb_format = AVIVO_D1GRPH_CONTROL_DEPTH_32BPP | AVIVO_D1GRPH_CONTROL_32BPP_ARGB8888;
#if X_BYTE_ORDER == X_BIG_ENDIAN
	    fb_swap = R600_D1GRPH_SWAP_ENDIAN_32BIT;
#endif
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

	OUTREG(AVIVO_D1GRPH_PRIMARY_SURFACE_ADDRESS + radeon_crtc->crtc_offset, fb_location);
	OUTREG(AVIVO_D1GRPH_SECONDARY_SURFACE_ADDRESS + radeon_crtc->crtc_offset, fb_location);
	OUTREG(AVIVO_D1GRPH_CONTROL + radeon_crtc->crtc_offset, fb_format);

#if X_BYTE_ORDER == X_BIG_ENDIAN
	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    OUTREG(R600_D1GRPH_SWAP_CONTROL + radeon_crtc->crtc_offset, fb_swap);
#endif

	OUTREG(AVIVO_D1GRPH_SURFACE_OFFSET_X + radeon_crtc->crtc_offset, 0);
	OUTREG(AVIVO_D1GRPH_SURFACE_OFFSET_Y + radeon_crtc->crtc_offset, 0);
	OUTREG(AVIVO_D1GRPH_X_START + radeon_crtc->crtc_offset, 0);
	OUTREG(AVIVO_D1GRPH_Y_START + radeon_crtc->crtc_offset, 0);
	OUTREG(AVIVO_D1GRPH_X_END + radeon_crtc->crtc_offset, info->virtualX);
	OUTREG(AVIVO_D1GRPH_Y_END + radeon_crtc->crtc_offset, info->virtualY);
	OUTREG(AVIVO_D1GRPH_PITCH + radeon_crtc->crtc_offset,
	       crtc->scrn->displayWidth);
	OUTREG(AVIVO_D1GRPH_ENABLE + radeon_crtc->crtc_offset, 1);

	OUTREG(AVIVO_D1MODE_DESKTOP_HEIGHT + radeon_crtc->crtc_offset, mode->VDisplay);
	x &= ~3;
	y &= ~1;
	OUTREG(AVIVO_D1MODE_VIEWPORT_START + radeon_crtc->crtc_offset, (x << 16) | y);
	OUTREG(AVIVO_D1MODE_VIEWPORT_SIZE + radeon_crtc->crtc_offset,
	       (mode->HDisplay << 16) | mode->VDisplay);

	if (adjusted_mode->Flags & V_INTERLACE)
	    OUTREG(AVIVO_D1MODE_DATA_FORMAT + radeon_crtc->crtc_offset,
		   AVIVO_D1MODE_INTERLEAVE_EN);
	else
	    OUTREG(AVIVO_D1MODE_DATA_FORMAT + radeon_crtc->crtc_offset,
		   0);
    } else {
	int format = 0;
	uint32_t crtc_gen_cntl, crtc2_gen_cntl, crtc_pitch;


	RADEONInitCommonRegisters(info->ModeReg, info);
	RADEONInitSurfaceCntl(crtc, info->ModeReg);
	ErrorF("restore common\n");
	RADEONRestoreCommonRegisters(pScrn, info->ModeReg);

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
	}

	crtc_pitch  = (((pScrn->displayWidth * pScrn->bitsPerPixel) +
			((pScrn->bitsPerPixel * 8) -1)) /
		       (pScrn->bitsPerPixel * 8));
	crtc_pitch |= crtc_pitch << 16;

	switch (radeon_crtc->crtc_id) {
	case 0:
	    ErrorF("init crtc1\n");
	    crtc_gen_cntl = INREG(RADEON_CRTC_GEN_CNTL) & 0xfffff0ff;
	    crtc_gen_cntl |= (format << 8);
	    OUTREG(RADEON_CRTC_GEN_CNTL, crtc_gen_cntl);
	    OUTREG(RADEON_CRTC_PITCH, crtc_pitch);
	    RADEONInitCrtcBase(crtc, info->ModeReg, x, y);
	    RADEONRestoreCrtcBase(pScrn, info->ModeReg);
	    break;
	case 1:
	    crtc2_gen_cntl = INREG(RADEON_CRTC2_GEN_CNTL) & 0xfffff0ff;
	    crtc2_gen_cntl |= (format << 8);
	    OUTREG(RADEON_CRTC2_GEN_CNTL, crtc2_gen_cntl);
	    OUTREG(RADEON_CRTC2_PITCH, crtc_pitch);
	    RADEONInitCrtc2Base(crtc, info->ModeReg, x, y);
	    RADEONRestoreCrtc2Base(pScrn, info->ModeReg);
	    OUTREG(RADEON_FP_H2_SYNC_STRT_WID,   INREG(RADEON_CRTC2_H_SYNC_STRT_WID));
	    OUTREG(RADEON_FP_V2_SYNC_STRT_WID,   INREG(RADEON_CRTC2_V_SYNC_STRT_WID));
	    break;
	}
    }

    if (info->DispPriority)
	RADEONInitDispBandwidth(pScrn);

    radeon_crtc->initialized = TRUE;

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

/* Calculate display buffer watermark to prevent buffer underflow */
void
RADEONInitDispBandwidthAVIVO(ScrnInfoPtr pScrn,
			      DisplayModePtr mode1, int pixel_bytes1,
			      DisplayModePtr mode2, int pixel_bytes2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt   = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    uint32_t dc_lb_memory_split;
    float mem_bw, peak_disp_bw;
    float min_mem_eff = 0.8; /* XXX: taken from legacy method */
    float pix_clk, pix_clk2; /* in MHz */

    /*
     * Set display0/1 priority up in the memory controller for
     * modes if the user specifies HIGH for displaypriority
     * option.
     */
    if (info->DispPriority == 2) {
	uint32_t mc_init_misc_lat_timer = 0;
	if (info->ChipFamily == CHIP_FAMILY_RV515)
	    mc_init_misc_lat_timer = INMC(pScrn, RV515_MC_INIT_MISC_LAT_TIMER);
	else if (info->ChipFamily == CHIP_FAMILY_RS690)
	    mc_init_misc_lat_timer = INMC(pScrn, RS690_MC_INIT_MISC_LAT_TIMER);

	mc_init_misc_lat_timer &= ~(R300_MC_DISP1R_INIT_LAT_MASK << R300_MC_DISP1R_INIT_LAT_SHIFT);
	mc_init_misc_lat_timer &= ~(R300_MC_DISP0R_INIT_LAT_MASK << R300_MC_DISP0R_INIT_LAT_SHIFT);

	if (pRADEONEnt->pCrtc[1]->enabled)
	    mc_init_misc_lat_timer |= (1 << R300_MC_DISP1R_INIT_LAT_SHIFT); /* display 1 */
	if (pRADEONEnt->pCrtc[0]->enabled)
	    mc_init_misc_lat_timer |= (1 << R300_MC_DISP0R_INIT_LAT_SHIFT); /* display 0 */

	if (info->ChipFamily == CHIP_FAMILY_RV515)
	    OUTMC(pScrn, RV515_MC_INIT_MISC_LAT_TIMER, mc_init_misc_lat_timer);
	else if (info->ChipFamily == CHIP_FAMILY_RS690)
	    OUTMC(pScrn, RS690_MC_INIT_MISC_LAT_TIMER, mc_init_misc_lat_timer);
    }

    /* XXX: fix me for AVIVO
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

    /*
     * Line Buffer Setup
     * There is a single line buffer shared by both display controllers.
     * DC_LB_MEMORY_SPLIT controls how that line buffer is shared between the display
     * controllers.  The paritioning can either be done manually or via one of four
     * preset allocations specified in bits 1:0:
     * 0 - line buffer is divided in half and shared between each display controller
     * 1 - D1 gets 3/4 of the line buffer, D2 gets 1/4
     * 2 - D1 gets the whole buffer
     * 3 - D1 gets 1/4 of the line buffer, D2 gets 3/4
     * Setting bit 2 of DC_LB_MEMORY_SPLIT controls switches to manual allocation mode.
     * In manual allocation mode, D1 always starts at 0, D1 end/2 is specified in bits
     * 14:4; D2 allocation follows D1.
     */

    /* is auto or manual better ? */
    dc_lb_memory_split = INREG(AVIVO_DC_LB_MEMORY_SPLIT) & ~AVIVO_DC_LB_MEMORY_SPLIT_MASK;
    dc_lb_memory_split &= ~AVIVO_DC_LB_MEMORY_SPLIT_SHIFT_MODE;
#if 1
    /* auto */
    if (mode1 && mode2) {
	if (mode1->HDisplay > mode2->HDisplay) {
	    if (mode1->HDisplay > 2560)
		dc_lb_memory_split |= AVIVO_DC_LB_MEMORY_SPLIT_D1_3Q_D2_1Q;
	    else
		dc_lb_memory_split |= AVIVO_DC_LB_MEMORY_SPLIT_D1HALF_D2HALF;
	} else if (mode2->HDisplay > mode1->HDisplay) {
	    if (mode2->HDisplay > 2560)
		dc_lb_memory_split |= AVIVO_DC_LB_MEMORY_SPLIT_D1_1Q_D2_3Q;
	    else
		dc_lb_memory_split |= AVIVO_DC_LB_MEMORY_SPLIT_D1HALF_D2HALF;
	} else
	    dc_lb_memory_split |= AVIVO_DC_LB_MEMORY_SPLIT_D1HALF_D2HALF;
    } else if (mode1) {
	dc_lb_memory_split |= AVIVO_DC_LB_MEMORY_SPLIT_D1_ONLY;
    } else if (mode2) {
	dc_lb_memory_split |= AVIVO_DC_LB_MEMORY_SPLIT_D1_1Q_D2_3Q;
    }
#else
    /* manual */
    dc_lb_memory_split |= AVIVO_DC_LB_MEMORY_SPLIT_SHIFT_MODE;
    dc_lb_memory_split &= ~(AVIVO_DC_LB_DISP1_END_ADR_MASK << AVIVO_DC_LB_DISP1_END_ADR_SHIFT);
    if (mode1) {
	dc_lb_memory_split |= ((((mode1->HDisplay / 2) + 64 /*???*/) & AVIVO_DC_LB_DISP1_END_ADR_MASK)
			       << AVIVO_DC_LB_DISP1_END_ADR_SHIFT);
    } else if (mode2) {
	dc_lb_memory_split |= (0 << AVIVO_DC_LB_DISP1_END_ADR_SHIFT);
    }
    OUTREG(AVIVO_DC_LB_MEMORY_SPLIT, dc_lb_memory_split);
#endif

    /*
     * Watermark setup
     * TODO...
     * Unforunately, I haven't been able to dig up the avivo watermark programming
     * guide yet. -AGD
     */

}
