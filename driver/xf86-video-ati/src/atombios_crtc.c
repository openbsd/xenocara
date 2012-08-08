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
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include <math.h>
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

    if (RHDAtomBiosFunc(atomBIOS->pScrn, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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

    if (RHDAtomBiosFunc(atomBIOS->pScrn, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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

    if (RHDAtomBiosFunc(atomBIOS->pScrn, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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

    if (RHDAtomBiosFunc(atomBIOS->pScrn, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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
	atombios_enable_crtc(info->atomBIOS, radeon_crtc->crtc_id, 1);
	if (IS_DCE3_VARIANT)
	    atombios_enable_crtc_memreq(info->atomBIOS, radeon_crtc->crtc_id, 1);
	atombios_blank_crtc(info->atomBIOS, radeon_crtc->crtc_id, 0);
	break;
    case DPMSModeStandby:
    case DPMSModeSuspend:
    case DPMSModeOff:
	atombios_blank_crtc(info->atomBIOS, radeon_crtc->crtc_id, 1);
	if (IS_DCE3_VARIANT)
	    atombios_enable_crtc_memreq(info->atomBIOS, radeon_crtc->crtc_id, 0);
	atombios_enable_crtc(info->atomBIOS, radeon_crtc->crtc_id, 0);
	break;
    }
}

static AtomBiosResult
atombios_set_crtc_timing(xf86CrtcPtr crtc, DisplayModePtr mode)
{
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr  info = RADEONPTR(crtc->scrn);
    AtomBiosArgRec data;
    unsigned char *space;
    uint16_t misc = 0;
    SET_CRTC_TIMING_PARAMETERS_PS_ALLOCATION param;
    memset(&param, 0, sizeof(param));

    param.usH_Total		= cpu_to_le16(mode->CrtcHTotal);
    param.usH_Disp		= cpu_to_le16(mode->CrtcHDisplay);
    param.usH_SyncStart		= cpu_to_le16(mode->CrtcHSyncStart);
    param.usH_SyncWidth		= cpu_to_le16(mode->CrtcHSyncEnd - mode->CrtcHSyncStart);
    param.usV_Total		= cpu_to_le16(mode->CrtcVTotal);
    param.usV_Disp		= cpu_to_le16(mode->CrtcVDisplay);
    param.usV_SyncStart		= cpu_to_le16(mode->CrtcVSyncStart);
    param.usV_SyncWidth		= cpu_to_le16(mode->CrtcVSyncEnd - mode->CrtcVSyncStart);

    if (mode->Flags & V_NVSYNC)
	misc |= ATOM_VSYNC_POLARITY;

    if (mode->Flags & V_NHSYNC)
	misc |= ATOM_HSYNC_POLARITY;

    if (mode->Flags & V_CSYNC)
	misc |= ATOM_COMPOSITESYNC;

    if (mode->Flags & V_INTERLACE)
	misc |= ATOM_INTERLACE;

    if (mode->Flags & V_DBLSCAN)
	misc |= ATOM_DOUBLE_CLOCK_MODE;

    param.susModeMiscInfo.usAccess      = cpu_to_le16(misc);
    param.ucCRTC			= radeon_crtc->crtc_id;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, SetCRTC_Timing);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &param;

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Set CRTC Timing success\n");
	return ATOM_SUCCESS ;
    }

    ErrorF("Set CRTC Timing failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

static AtomBiosResult
atombios_set_crtc_dtd_timing(xf86CrtcPtr crtc, DisplayModePtr mode)
{
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr  info = RADEONPTR(crtc->scrn);
    AtomBiosArgRec data;
    unsigned char *space;
    uint16_t misc = 0;
    SET_CRTC_USING_DTD_TIMING_PARAMETERS param;
    memset(&param, 0, sizeof(param));

    param.usH_Size          = cpu_to_le16(mode->CrtcHDisplay);
    param.usH_Blanking_Time = cpu_to_le16(mode->CrtcHBlankEnd - mode->CrtcHDisplay);
    param.usV_Size          = cpu_to_le16(mode->CrtcVDisplay);
    param.usV_Blanking_Time = cpu_to_le16(mode->CrtcVBlankEnd - mode->CrtcVDisplay);
    param.usH_SyncOffset    = cpu_to_le16(mode->CrtcHSyncStart - mode->CrtcHDisplay);
    param.usH_SyncWidth     = cpu_to_le16(mode->CrtcHSyncEnd - mode->CrtcHSyncStart);
    param.usV_SyncOffset    = cpu_to_le16(mode->CrtcVSyncStart - mode->CrtcVDisplay);
    param.usV_SyncWidth     = cpu_to_le16(mode->CrtcVSyncEnd - mode->CrtcVSyncStart);

    if (mode->Flags & V_NVSYNC)
	misc |= ATOM_VSYNC_POLARITY;

    if (mode->Flags & V_NHSYNC)
	misc |= ATOM_HSYNC_POLARITY;

    if (mode->Flags & V_CSYNC)
	misc |= ATOM_COMPOSITESYNC;

    if (mode->Flags & V_INTERLACE)
	misc |= ATOM_INTERLACE;

    if (mode->Flags & V_DBLSCAN)
	misc |= ATOM_DOUBLE_CLOCK_MODE;

    param.susModeMiscInfo.usAccess = cpu_to_le16(misc);
    param.ucCRTC= radeon_crtc->crtc_id;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, SetCRTC_UsingDTDTiming);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &param;

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Set DTD CRTC Timing success\n");
	return ATOM_SUCCESS ;
    }

    ErrorF("Set DTD CRTC Timing failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

static void
atombios_pick_pll(xf86CrtcPtr crtc)
{
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr info = RADEONPTR(crtc->scrn);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    xf86OutputPtr output;
    RADEONOutputPrivatePtr radeon_output;
    int o, c;
    uint32_t pll_use_mask = 0;
    Bool is_dp = FALSE;

    if (IS_DCE4_VARIANT) {
	for (o = 0; o < xf86_config->num_output; o++) {
	    output = xf86_config->output[o];
	    if (output->crtc == crtc) {
		int mode = atombios_get_encoder_mode(output);
		radeon_output = output->driver_private;

		if (mode == ATOM_ENCODER_MODE_DP) {
		    is_dp = TRUE;
		    break;
		} else {
		    for (c = 0; c < xf86_config->num_crtc; c++) {
			xf86CrtcPtr test_crtc = xf86_config->crtc[c];
			RADEONCrtcPrivatePtr radeon_test_crtc = test_crtc->driver_private;

			if (crtc != test_crtc && (radeon_test_crtc->pll_id >= 0))
			    pll_use_mask |= (1 << radeon_test_crtc->pll_id);

		    }
		}
	    }
	}
	/* DP clock comes from DCPLL, DP PHY CLK comes from ext source
	 * setting ATOM_PPLL_INVALID skips the PPLL programming for DP
	 */
	if (is_dp)
	    radeon_crtc->pll_id = ATOM_PPLL_INVALID;
	else if (!(pll_use_mask & 1))
	    radeon_crtc->pll_id = ATOM_PPLL1;
	else
	    radeon_crtc->pll_id = ATOM_PPLL2;
    } else
	radeon_crtc->pll_id = radeon_crtc->crtc_id;

    ErrorF("Picked PLL %d\n", radeon_crtc->pll_id);

    for (o = 0; o < xf86_config->num_output; o++) {
	output = xf86_config->output[o];
	if (output->crtc == crtc) {
	    radeon_output = output->driver_private;
	    radeon_output->pll_id = radeon_crtc->pll_id;
	}
    }
}

union adjust_pixel_clock {
	ADJUST_DISPLAY_PLL_PS_ALLOCATION v1;
	ADJUST_DISPLAY_PLL_PS_ALLOCATION_V3 v3;
};

static uint32_t atombios_adjust_pll(xf86CrtcPtr crtc, DisplayModePtr mode, int *pll_flags_p)
{
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    uint32_t adjusted_clock = mode->Clock;
    RADEONOutputPrivatePtr radeon_output = NULL;
    radeon_encoder_ptr radeon_encoder = NULL;
    xf86OutputPtr output;
    int pll_flags = 0;
    int i;

    if (IS_AVIVO_VARIANT) {
	if (xf86ReturnOptValBool(info->Options, OPTION_NEW_PLL, TRUE))
	    radeon_crtc->pll_algo = RADEON_PLL_NEW;
	else
	    radeon_crtc->pll_algo = RADEON_PLL_OLD;
    } else {
	if (xf86ReturnOptValBool(info->Options, OPTION_NEW_PLL, FALSE))
	    radeon_crtc->pll_algo = RADEON_PLL_NEW;
	else
	    radeon_crtc->pll_algo = RADEON_PLL_OLD;
    }

    if (IS_AVIVO_VARIANT) {
	if ((info->ChipFamily == CHIP_FAMILY_RS600) ||
	    (info->ChipFamily == CHIP_FAMILY_RS690) ||
	    (info->ChipFamily == CHIP_FAMILY_RS740))
	    pll_flags |= /*RADEON_PLL_USE_FRAC_FB_DIV |*/
		RADEON_PLL_PREFER_CLOSEST_LOWER;
	if (IS_DCE32_VARIANT && mode->Clock > 200000) /* range limits??? */
	    pll_flags |= RADEON_PLL_PREFER_HIGH_FB_DIV;
	else
	    pll_flags |= RADEON_PLL_PREFER_LOW_REF_DIV;
    } else {
	pll_flags |= RADEON_PLL_LEGACY;

	if (mode->Clock > 200000) /* range limits??? */
	    pll_flags |= RADEON_PLL_PREFER_HIGH_FB_DIV;
	else
	    pll_flags |= RADEON_PLL_PREFER_LOW_REF_DIV;
    }
    
    for (i = 0; i < xf86_config->num_output; i++) {
	output = xf86_config->output[i];
	if (output->crtc == crtc) {
	    radeon_output = output->driver_private;
	    radeon_encoder = radeon_get_encoder(output);
	    if (IS_AVIVO_VARIANT) {
		/* DVO wants 2x pixel clock if the DVO chip is in 12 bit mode */
		if (radeon_encoder &&
		    (radeon_encoder->encoder_id == ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DVO1) &&
		    !IS_DCE3_VARIANT)
		    adjusted_clock *= 2;
		if (radeon_output->active_device &
		    (ATOM_DEVICE_CV_SUPPORT | ATOM_DEVICE_TV_SUPPORT)) {
		    pll_flags |= RADEON_PLL_PREFER_CLOSEST_LOWER;
		    radeon_crtc->pll_algo = RADEON_PLL_OLD;
		}
	    } else {
		if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT |
						    ATOM_DEVICE_DFP_SUPPORT))
		    pll_flags |= RADEON_PLL_NO_ODD_POST_DIV;
		if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT))
		    pll_flags |= (RADEON_PLL_USE_BIOS_DIVS | RADEON_PLL_USE_REF_DIV);
	    }
    	    if (IS_DCE3_VARIANT)
		break;
	}
    }
    
    if (IS_DCE3_VARIANT) {
	union adjust_pixel_clock args;
	int major, minor, index;
	AtomBiosArgRec data;
	unsigned char *space;

	memset(&args, 0, sizeof(args));

	index = GetIndexIntoMasterTable(COMMAND, AdjustDisplayPll);

	atombios_get_command_table_version(info->atomBIOS, index, &major, &minor);

	data.exec.index = index;
	data.exec.dataSpace = (void *)&space;
	data.exec.pspace = &args;

	switch(major) {
	case 1:
	    switch(minor) {
	    case 1:
	    case 2:
		args.v1.usPixelClock = cpu_to_le16(adjusted_clock / 10);
		args.v1.ucTransmitterID = radeon_encoder->encoder_id;
		args.v1.ucEncodeMode = atombios_get_encoder_mode(output);

		ErrorF("before %d\n", args.v1.usPixelClock);
		if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
		    adjusted_clock = le16_to_cpu(args.v1.usPixelClock) * 10;
		}
		ErrorF("after %d\n", args.v1.usPixelClock);
		break;
	    case 3:
		args.v3.sInput.usPixelClock = cpu_to_le16(adjusted_clock / 10);
		args.v3.sInput.ucTransmitterID = radeon_encoder->encoder_id;
		args.v3.sInput.ucEncodeMode = atombios_get_encoder_mode(output);
		args.v3.sInput.ucDispPllConfig = 0;
		if (radeon_output->coherent_mode || (args.v3.sInput.ucEncodeMode == ATOM_ENCODER_MODE_DP))
		    args.v3.sInput.ucDispPllConfig |= DISPPLL_CONFIG_COHERENT_MODE;
		if (adjusted_clock > 165000)
		    args.v3.sInput.ucDispPllConfig |= DISPPLL_CONFIG_DUAL_LINK;
		// if SS
		//    args.v3.sInput.ucDispPllConfig |= DISPPLL_CONFIG_SS_ENABLE;

		ErrorF("before %d 0x%x\n", args.v3.sInput.usPixelClock, args.v3.sInput.ucDispPllConfig);
		if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
		    adjusted_clock = args.v3.sOutput.ulDispPllFreq * 10;
		    if (args.v3.sOutput.ucRefDiv) {
			pll_flags |= RADEON_PLL_USE_REF_DIV;
			info->pll.reference_div = args.v3.sOutput.ucRefDiv;
		    }
		    if (args.v3.sOutput.ucPostDiv) {
			pll_flags |= RADEON_PLL_USE_POST_DIV;
			info->pll.post_div = args.v3.sOutput.ucPostDiv;
		    }
		    ErrorF("after %d %d %d\n", args.v3.sOutput.ulDispPllFreq,
			   args.v3.sOutput.ucRefDiv, args.v3.sOutput.ucPostDiv);
		}
		break;
	    default:
		ErrorF("%s: Unknown table version %d %d\n", __func__, major, minor);
		goto out;
	    }
	    break;
	default:
	    ErrorF("%s: Unknown table version %d %d\n", __func__, major, minor);
	    goto out;
	}
    }
out:
    *pll_flags_p = pll_flags;
    return adjusted_clock;
}
				    
union set_pixel_clock {
	SET_PIXEL_CLOCK_PS_ALLOCATION base;
	PIXEL_CLOCK_PARAMETERS v1;
	PIXEL_CLOCK_PARAMETERS_V2 v2;
	PIXEL_CLOCK_PARAMETERS_V3 v3;
	PIXEL_CLOCK_PARAMETERS_V5 v5;
};

static void
atombios_crtc_set_dcpll(xf86CrtcPtr crtc)
{
    RADEONInfoPtr  info = RADEONPTR(crtc->scrn);
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    xf86OutputPtr output = NULL;
    RADEONOutputPrivatePtr radeon_output = NULL;
    radeon_encoder_ptr radeon_encoder = NULL;
    int index;
    int major, minor, i;
    union set_pixel_clock args;
    AtomBiosArgRec data;
    unsigned char *space;

    memset(&args, 0, sizeof(args));

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

    index = GetIndexIntoMasterTable(COMMAND, SetPixelClock);
    atombios_get_command_table_version(info->atomBIOS, index, &major, &minor);

    /*ErrorF("table is %d %d\n", major, minor);*/
    switch(major) {
    case 1:
	switch(minor) {
	case 5:
	    args.v5.ucCRTC = ATOM_CRTC_INVALID;
	    /* XXX: get this from the firmwareinfo table */
	    args.v5.usPixelClock = info->default_dispclk;
	    args.v5.ucPpll = ATOM_DCPLL;
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

    data.exec.index = index;
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &args;

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Set DCPLL success\n");
	return;
    }

    ErrorF("Set DCPLL failed\n");
    return;
}

static void
atombios_crtc_set_pll(xf86CrtcPtr crtc, DisplayModePtr mode)
{
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int index;
    uint32_t sclock;
    uint32_t ref_div = 0, fb_div = 0, frac_fb_div = 0, post_div = 0;
    int major, minor;
    union set_pixel_clock args;
    xf86OutputPtr output = NULL;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    radeon_encoder_ptr radeon_encoder = NULL;
    int pll_flags = 0;
    uint32_t temp;
    AtomBiosArgRec data;
    unsigned char *space;
    int i;

    memset(&args, 0, sizeof(args));

    if (IS_DCE4_VARIANT) {
	/* XXX 6 crtcs, but only 2 plls */
	switch (radeon_crtc->pll_id) {
	case ATOM_PPLL1:
	    temp = INREG(EVERGREEN_P1PLL_SS_CNTL);
	    OUTREG(EVERGREEN_P1PLL_SS_CNTL, temp & ~EVERGREEN_PxPLL_SS_EN);
	    break;
	case ATOM_PPLL2:
	    temp = INREG(EVERGREEN_P2PLL_SS_CNTL);
	    OUTREG(EVERGREEN_P2PLL_SS_CNTL, temp & ~EVERGREEN_PxPLL_SS_EN);
	    break;
	}
    } else {
	if (radeon_crtc->crtc_id == 0) {
	    temp = INREG(AVIVO_P1PLL_INT_SS_CNTL);
	    OUTREG(AVIVO_P1PLL_INT_SS_CNTL, temp & ~1);
	} else {
	    temp = INREG(AVIVO_P2PLL_INT_SS_CNTL);
	    OUTREG(AVIVO_P2PLL_INT_SS_CNTL, temp & ~1);
	}
    }

    if (IS_DCE3_VARIANT) {
	for (i = 0; i < xf86_config->num_output; i++) {
	    output = xf86_config->output[i];
	    if (output->crtc == crtc) {
		radeon_encoder = radeon_get_encoder(output);
		break;
	    }
	}

	if (output->driver_private == NULL) {
	    xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR, "No output assigned to crtc!\n");
	    return;
	}
	if (radeon_encoder == NULL) {
	    xf86DrvMsg(crtc->scrn->scrnIndex, X_ERROR, "No encoder assigned to output!\n");
	    return;
	}
    }

    sclock = atombios_adjust_pll(crtc, mode, &pll_flags);

    RADEONComputePLL(crtc, &info->pll, sclock, &temp,
		     &fb_div, &frac_fb_div, &ref_div, &post_div, pll_flags);
    sclock = temp; /* 10 khz */

    xf86DrvMsg(crtc->scrn->scrnIndex, X_INFO,
	       "crtc(%d) Clock: mode %d, PLL %lu\n",
	       radeon_crtc->crtc_id, mode->Clock, (long unsigned int)sclock * 10);
    xf86DrvMsg(crtc->scrn->scrnIndex, X_INFO,
	       "crtc(%d) PLL  : refdiv %u, fbdiv 0x%X(%u), fracfbdiv %u, pdiv %u\n",
	       radeon_crtc->crtc_id, (unsigned int)ref_div, (unsigned int)fb_div,
	       (unsigned int)fb_div, (unsigned int)frac_fb_div, (unsigned int)post_div);

    index = GetIndexIntoMasterTable(COMMAND, SetPixelClock);
    atombios_get_command_table_version(info->atomBIOS, index, &major, &minor);

    /*ErrorF("table is %d %d\n", major, minor);*/
    switch(major) {
    case 1:
	switch(minor) {
	case 1:
	case 2:
	    args.v2.usPixelClock = cpu_to_le16(mode->Clock / 10);
	    args.v2.usRefDiv = cpu_to_le16(ref_div);
	    args.v2.usFbDiv = cpu_to_le16(fb_div);
	    args.v2.ucFracFbDiv = frac_fb_div;
	    args.v2.ucPostDiv = post_div;
	    args.v2.ucPpll = radeon_crtc->pll_id;
	    args.v2.ucCRTC = radeon_crtc->crtc_id;
	    args.v2.ucRefDivSrc = 1;
	    break;
	case 3:
	    args.v3.usPixelClock = cpu_to_le16(mode->Clock / 10);
	    args.v3.usRefDiv = cpu_to_le16(ref_div);
	    args.v3.usFbDiv = cpu_to_le16(fb_div);
	    args.v3.ucFracFbDiv = frac_fb_div;
	    args.v3.ucPostDiv = post_div;
	    args.v3.ucPpll = radeon_crtc->pll_id;
	    args.v3.ucMiscInfo = (radeon_crtc->crtc_id << 2);
	    args.v3.ucTransmitterId = radeon_encoder->encoder_id;
	    args.v3.ucEncoderMode = atombios_get_encoder_mode(output);
	    break;
	case 5:
	    args.v5.ucCRTC = radeon_crtc->crtc_id;
	    args.v5.usPixelClock = cpu_to_le16(mode->Clock / 10);
	    args.v5.ucRefDiv = ref_div;
	    args.v5.usFbDiv = cpu_to_le16(fb_div);
	    args.v5.ulFbDivDecFrac = cpu_to_le32(frac_fb_div * 100000);
	    args.v5.ucPostDiv = post_div;
	    args.v5.ucPpll = radeon_crtc->pll_id;
	    args.v5.ucMiscInfo = 0; //HDMI depth
	    args.v5.ucTransmitterID = radeon_encoder->encoder_id;
	    args.v5.ucEncoderMode = atombios_get_encoder_mode(output);
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

    data.exec.index = index;
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &args;

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Set CRTC %d PLL success\n", radeon_crtc->crtc_id);
	return;
    }

    ErrorF("Set CRTC %d PLL failed\n", radeon_crtc->crtc_id);
    return;
}

static void evergreen_set_base_format(xf86CrtcPtr crtc,
				      DisplayModePtr mode,
				      DisplayModePtr adjusted_mode,
				      int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint64_t fb_location = crtc->scrn->fbOffset + info->fbLocation;
    uint32_t fb_format;
    uint32_t fb_swap = EVERGREEN_GRPH_ENDIAN_SWAP(EVERGREEN_GRPH_ENDIAN_NONE);

    switch (crtc->scrn->bitsPerPixel) {
    case 15:
	fb_format = (EVERGREEN_GRPH_DEPTH(EVERGREEN_GRPH_DEPTH_16BPP) |
		     EVERGREEN_GRPH_FORMAT(EVERGREEN_GRPH_FORMAT_ARGB1555));
	break;
    case 16:
	fb_format = (EVERGREEN_GRPH_DEPTH(EVERGREEN_GRPH_DEPTH_16BPP) |
		     EVERGREEN_GRPH_FORMAT(EVERGREEN_GRPH_FORMAT_ARGB565));
#if X_BYTE_ORDER == X_BIG_ENDIAN
	fb_swap = EVERGREEN_GRPH_ENDIAN_SWAP(EVERGREEN_GRPH_ENDIAN_8IN16);
#endif
	break;
    case 24:
    case 32:
	fb_format = (EVERGREEN_GRPH_DEPTH(EVERGREEN_GRPH_DEPTH_32BPP) |
		     EVERGREEN_GRPH_FORMAT(EVERGREEN_GRPH_FORMAT_ARGB8888));
#if X_BYTE_ORDER == X_BIG_ENDIAN
	fb_swap = EVERGREEN_GRPH_ENDIAN_SWAP(EVERGREEN_GRPH_ENDIAN_8IN32);
#endif
	break;
    default:
	FatalError("Unsupported screen depth: %d\n", xf86GetDepth());
    }

    switch (radeon_crtc->crtc_id) {
    case 0:
    default:
	OUTREG(AVIVO_D1VGA_CONTROL, 0);
	break;
    case 1:
	OUTREG(AVIVO_D2VGA_CONTROL, 0);
	break;
    case 2:
	OUTREG(EVERGREEN_D3VGA_CONTROL, 0);
	break;
    case 3:
	OUTREG(EVERGREEN_D4VGA_CONTROL, 0);
	break;
    case 4:
	OUTREG(EVERGREEN_D5VGA_CONTROL, 0);
	break;
    case 5:
	OUTREG(EVERGREEN_D6VGA_CONTROL, 0);
	break;
    }

    /* setup fb format and location
     */
    if (crtc->rotatedData != NULL) {
	/* x/y offset is already included */
	x = 0;
	y = 0;
	fb_location = fb_location + (char *)crtc->rotatedData - (char *)info->FB;
    }


    OUTREG(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS_HIGH + radeon_crtc->crtc_offset,
	   (fb_location >> 32) & 0xf);
    OUTREG(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS_HIGH + radeon_crtc->crtc_offset,
	   (fb_location >> 32) & 0xf);
    OUTREG(EVERGREEN_GRPH_PRIMARY_SURFACE_ADDRESS + radeon_crtc->crtc_offset,
	   fb_location & EVERGREEN_GRPH_SURFACE_ADDRESS_MASK);
    OUTREG(EVERGREEN_GRPH_SECONDARY_SURFACE_ADDRESS + radeon_crtc->crtc_offset,
	   fb_location & EVERGREEN_GRPH_SURFACE_ADDRESS_MASK);
    OUTREG(EVERGREEN_GRPH_CONTROL + radeon_crtc->crtc_offset, fb_format);
    OUTREG(EVERGREEN_GRPH_SWAP_CONTROL + radeon_crtc->crtc_offset, fb_swap);

    OUTREG(EVERGREEN_GRPH_SURFACE_OFFSET_X + radeon_crtc->crtc_offset, 0);
    OUTREG(EVERGREEN_GRPH_SURFACE_OFFSET_Y + radeon_crtc->crtc_offset, 0);
    OUTREG(EVERGREEN_GRPH_X_START + radeon_crtc->crtc_offset, 0);
    OUTREG(EVERGREEN_GRPH_Y_START + radeon_crtc->crtc_offset, 0);
    OUTREG(EVERGREEN_GRPH_X_END + radeon_crtc->crtc_offset, info->virtualX);
    OUTREG(EVERGREEN_GRPH_Y_END + radeon_crtc->crtc_offset, info->virtualY);
    OUTREG(EVERGREEN_GRPH_PITCH + radeon_crtc->crtc_offset,
	   crtc->scrn->displayWidth);
    OUTREG(EVERGREEN_GRPH_ENABLE + radeon_crtc->crtc_offset, 1);

    OUTREG(EVERGREEN_DESKTOP_HEIGHT + radeon_crtc->crtc_offset, mode->VDisplay);
    x &= ~3;
    y &= ~1;
    OUTREG(EVERGREEN_VIEWPORT_START + radeon_crtc->crtc_offset, (x << 16) | y);
    OUTREG(EVERGREEN_VIEWPORT_SIZE + radeon_crtc->crtc_offset, (mode->HDisplay << 16) | mode->VDisplay);

    if (adjusted_mode->Flags & V_INTERLACE)
	OUTREG(EVERGREEN_DATA_FORMAT + radeon_crtc->crtc_offset, EVERGREEN_INTERLEAVE_EN);
    else
	OUTREG(EVERGREEN_DATA_FORMAT + radeon_crtc->crtc_offset, 0);

}

static void avivo_set_base_format(xf86CrtcPtr crtc,
				  DisplayModePtr mode,
				  DisplayModePtr adjusted_mode,
				  int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint64_t fb_location = crtc->scrn->fbOffset + info->fbLocation;
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

    if (info->ChipFamily >= CHIP_FAMILY_RV770) {
	if (radeon_crtc->crtc_id) {
	    OUTREG(R700_D2GRPH_PRIMARY_SURFACE_ADDRESS_HIGH, (fb_location >> 32) & 0xf);
	    OUTREG(R700_D2GRPH_SECONDARY_SURFACE_ADDRESS_HIGH, (fb_location >> 32) & 0xf);
	} else {
	    OUTREG(R700_D1GRPH_PRIMARY_SURFACE_ADDRESS_HIGH, (fb_location >> 32) & 0xf);
	    OUTREG(R700_D1GRPH_SECONDARY_SURFACE_ADDRESS_HIGH, (fb_location >> 32) & 0xf);
	}
    }
    OUTREG(AVIVO_D1GRPH_PRIMARY_SURFACE_ADDRESS + radeon_crtc->crtc_offset,
	   fb_location & 0xffffffff);
    OUTREG(AVIVO_D1GRPH_SECONDARY_SURFACE_ADDRESS + radeon_crtc->crtc_offset,
	   fb_location & 0xffffffff);
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

    if (mode->Flags & V_INTERLACE)
	OUTREG(AVIVO_D1MODE_DATA_FORMAT + radeon_crtc->crtc_offset,
	       AVIVO_D1MODE_INTERLEAVE_EN);
    else
	OUTREG(AVIVO_D1MODE_DATA_FORMAT + radeon_crtc->crtc_offset, 0);
}

static void legacy_set_base_format(xf86CrtcPtr crtc,
				   DisplayModePtr mode,
				   DisplayModePtr adjusted_mode,
				   int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int format = 0;
    uint32_t crtc_gen_cntl, crtc2_gen_cntl, crtc_pitch;

    RADEONInitCommonRegisters(info->ModeReg, info);
    RADEONInitSurfaceCntl(crtc, info->ModeReg);
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

void
atombios_crtc_mode_set(xf86CrtcPtr crtc,
		       DisplayModePtr mode,
		       DisplayModePtr adjusted_mode,
		       int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    Bool tilingChanged = FALSE;

    if (info->allowColorTiling) {
	radeon_crtc->can_tile = (mode->Flags & (V_DBLSCAN | V_INTERLACE)) ? FALSE : TRUE;
	tilingChanged = RADEONSetTiling(pScrn);
    }

    ErrorF("Mode %dx%d - %d %d %d\n", adjusted_mode->CrtcHDisplay, adjusted_mode->CrtcVDisplay,
	   adjusted_mode->CrtcHTotal, adjusted_mode->CrtcVTotal, adjusted_mode->Flags);

    RADEONInitMemMapRegisters(pScrn, info->ModeReg, info);
    RADEONRestoreMemMapRegisters(pScrn, info->ModeReg);

    if (IS_DCE4_VARIANT)
	atombios_crtc_set_dcpll(crtc);
    atombios_pick_pll(crtc);
    atombios_crtc_set_pll(crtc, adjusted_mode);
    if (IS_DCE4_VARIANT)
	atombios_set_crtc_dtd_timing(crtc, adjusted_mode);
    else {
	atombios_set_crtc_timing(crtc, adjusted_mode);
	if (!IS_AVIVO_VARIANT && (radeon_crtc->crtc_id == 0))
	    atombios_set_crtc_dtd_timing(crtc, adjusted_mode);
    }

    if (IS_DCE4_VARIANT)
	evergreen_set_base_format(crtc, mode, adjusted_mode, x, y);
    else if (IS_AVIVO_VARIANT)
	avivo_set_base_format(crtc, mode, adjusted_mode, x, y);
    else
	legacy_set_base_format(crtc, mode, adjusted_mode, x, y);

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
    float available_bandwidth = 0;
    float read_delay_latency = 1000;
    int i;
    Bool sideport = FALSE;

    /*
     * Set display0/1 priority up in the memory controller for
     * modes if the user specifies HIGH for displaypriority
     * option.
     */
    if (info->DispPriority == 2) {
	uint32_t mc_init_misc_lat_timer = 0;
	if (info->ChipFamily == CHIP_FAMILY_RV515)
	    mc_init_misc_lat_timer = INMC(pScrn, RV515_MC_INIT_MISC_LAT_TIMER);
	else if ((info->ChipFamily == CHIP_FAMILY_RS690) ||
		 (info->ChipFamily == CHIP_FAMILY_RS740))
	    mc_init_misc_lat_timer = INMC(pScrn, RS690_MC_INIT_MISC_LAT_TIMER);

	mc_init_misc_lat_timer &= ~(R300_MC_DISP1R_INIT_LAT_MASK << R300_MC_DISP1R_INIT_LAT_SHIFT);
	mc_init_misc_lat_timer &= ~(R300_MC_DISP0R_INIT_LAT_MASK << R300_MC_DISP0R_INIT_LAT_SHIFT);

	if (pRADEONEnt->pCrtc[1]->enabled)
	    mc_init_misc_lat_timer |= (1 << R300_MC_DISP1R_INIT_LAT_SHIFT); /* display 1 */
	if (pRADEONEnt->pCrtc[0]->enabled)
	    mc_init_misc_lat_timer |= (1 << R300_MC_DISP0R_INIT_LAT_SHIFT); /* display 0 */

	if (info->ChipFamily == CHIP_FAMILY_RV515)
	    OUTMC(pScrn, RV515_MC_INIT_MISC_LAT_TIMER, mc_init_misc_lat_timer);
	else if ((info->ChipFamily == CHIP_FAMILY_RS690) ||
		 (info->ChipFamily == CHIP_FAMILY_RS740))
	    OUTMC(pScrn, RS690_MC_INIT_MISC_LAT_TIMER, mc_init_misc_lat_timer);
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

    dc_lb_memory_split = INREG(AVIVO_DC_LB_MEMORY_SPLIT) & ~AVIVO_DC_LB_MEMORY_SPLIT_MASK;
    dc_lb_memory_split &= ~AVIVO_DC_LB_MEMORY_SPLIT_SHIFT_MODE;
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
    OUTREG(AVIVO_DC_LB_MEMORY_SPLIT, dc_lb_memory_split);
#if 0
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

    /* fixme
     * Still need to implement the actual watermark calculation
     * for rs600.  This just allows us to force high display
     * priority.
     */
    if (info->ChipFamily == CHIP_FAMILY_RS600) {
	if (info->DispPriority == 2) {
	    uint32_t priority_cnt;

	    if (mode1) {
		priority_cnt = INREG(AVIVO_D1MODE_PRIORITY_A_CNT);
		priority_cnt |= AVIVO_DxMODE_PRIORITY_ALWAYS_ON;
		OUTREG(AVIVO_D1MODE_PRIORITY_A_CNT, priority_cnt);

		priority_cnt = INREG(AVIVO_D1MODE_PRIORITY_B_CNT);
		priority_cnt |= AVIVO_DxMODE_PRIORITY_ALWAYS_ON;
		OUTREG(AVIVO_D1MODE_PRIORITY_B_CNT, priority_cnt);
	    }

	    if (mode2) {
		priority_cnt = INREG(AVIVO_D2MODE_PRIORITY_A_CNT);
		priority_cnt |= AVIVO_DxMODE_PRIORITY_ALWAYS_ON;
		OUTREG(AVIVO_D2MODE_PRIORITY_A_CNT, priority_cnt);

		priority_cnt = INREG(AVIVO_D2MODE_PRIORITY_B_CNT);
		priority_cnt |= AVIVO_DxMODE_PRIORITY_ALWAYS_ON;
		OUTREG(AVIVO_D2MODE_PRIORITY_B_CNT, priority_cnt);
	    }
	}
	return;
    }

    /* IGP bandwidth - get from integrated systems table
     * SYSTEM_MEMORY_BANDWIDTH (Mbyte/s) = SYSTEM_MEMORY_CLOCK (MHz) * (1+DDR) * 8 * EFF * Num of channels
     * SIDEPORT_MEMORY_BANDWIDTH = SIDEPORT_MEMORY_CLOCK * 2(byte) * 2(DDR) * 0.7(Eff)
     * CORE_CLOCK_BANDWIDTH (Mbyte/s) = SCLK (MHz) * 16 / Dynamic Engine clock Divider
     * HT_LINK_BANDWIDTH = HT_LINK_CLOCK * 2 * HT_LINK_WIDTH/8 * HT_LINK_EFF
     * system read delay
     * READ_DLY_MAX_LATENCY: 5000 ns
     * sideport read delay
     * READ_DLY_MAX_LATENCY: 370 * MCLK + 800 ns
     * MCLK is the sideport memory clock period in ns (MCLK = 1000 / MCLKfreq MHz)
     */

    if (info->IsIGP) {
	float core_clock_bandwidth = ((float)info->pm.mode[info->pm.current_mode].sclk / 100) * 16 / 1;

	if (sideport) {
	    float sideport_memory_bandwidth = (info->igp_sideport_mclk / 2) * 2 * 2 * 0.7;
	    float mclk = 1000 / info->igp_sideport_mclk;
	    read_delay_latency = 370 * mclk * 800;
	    available_bandwidth = MIN(sideport_memory_bandwidth, core_clock_bandwidth);
	} else {
	    float system_memory_bandwidth = (info->igp_system_mclk / 2) * (1 + 1) * 8 * 0.5 * 1;
	    float ht_link_bandwidth = info->igp_ht_link_clk * 2 * (info->igp_ht_link_width / 8) * 0.8;
	    read_delay_latency = 5000;
	    available_bandwidth = MIN(system_memory_bandwidth, MIN(ht_link_bandwidth, core_clock_bandwidth));
	}
    }

    /* calculate for each display */
    for (i = 0; i < 2; i++) {
	DisplayModePtr current = NULL;
	//RADEONCrtcPrivatePtr radeon_crtc = pRADEONEnt->Controller[i];
	float pclk, sclk, sclkfreq = 0;
	float consumption_time, consumption_rate;
	int num_line_pair, request_fifo_depth, lb_request_fifo_depth;
	int max_req;
	uint32_t lb_max_req_outstanding, priority_cnt;
	float line_time, active_time, chunk_time;
	float worst_case_latency, tolerable_latency;
	float fill_rate;
	int priority_mark_max, priority_mark, priority_mark2;
	int width, estimated_width;
	/* FIXME: handle the scalers better */
	Bool d1_scale_en = pRADEONEnt->Controller[0]->scaler_enabled;
	Bool d2_scale_en = pRADEONEnt->Controller[1]->scaler_enabled;
	float vtaps1 = 2; /* XXX */
	float vsc1 = pRADEONEnt->Controller[0]->vsc;
	float hsc1 = pRADEONEnt->Controller[0]->hsc;
	float vtaps2 = 2; /* XXX */
	float vsc2 = pRADEONEnt->Controller[1]->vsc;
	float hsc2 = pRADEONEnt->Controller[1]->hsc;

	if (i == 0)
	    current = mode1;
	else
	    current = mode2;

	if (current == NULL)
	    continue;

	/* Determine consumption rate
	   pclk = pixel clock period(ns)
	   vtaps = number of vertical taps,
	   vsc = vertical scaling ratio, defined as source/destination
	   hsc = horizontal scaling ration, defined as source/destination
	*/

	pclk = 1000 / ((float)current->Clock / 1000);

	if (i == 0) {
	    if (d1_scale_en)
		consumption_time = pclk / ((MAX(vtaps1, vsc1) * hsc1) / vtaps1);
	    else
		consumption_time = pclk;
	} else {
	    if (d2_scale_en)
		consumption_time = pclk / ((MAX(vtaps2, vsc2) * hsc2) / vtaps2);
	    else
		consumption_time = pclk;
	}

	consumption_rate = 1 / consumption_time;

	/* Determine request line buffer fifo depth
	   NumLinePair = Number of line pairs to request(1 = 2 lines, 2 = 4 lines)
	   LBRequestFifoDepth = Number of chunk requests the LB can put into the request FIFO for a display
	   width = viewport width in pixels
	*/
	if (i == 0) {
	    if (vsc1 > 2)
		num_line_pair = 2;
	    else
		num_line_pair = 1;
	} else {
	    if (vsc2 > 2)
		num_line_pair = 2;
	    else
		num_line_pair = 1;
	}

	width = current->CrtcHDisplay;
	request_fifo_depth = ceil(width/256) * num_line_pair;
	if (request_fifo_depth < 4)
	    lb_request_fifo_depth = 4;
	else
	    lb_request_fifo_depth = request_fifo_depth;

	if (info->IsIGP) {
	    if ((info->ChipFamily == CHIP_FAMILY_RS690) ||
		(info->ChipFamily == CHIP_FAMILY_RS740))
		OUTREG(RS690_DCP_CONTROL, 0);
	    else if ((info->ChipFamily == CHIP_FAMILY_RS780) ||
		     (info->ChipFamily == CHIP_FAMILY_RS880))
		OUTREG(RS690_DCP_CONTROL, 2);
	    max_req = lb_request_fifo_depth - 1;
	} else
	    max_req = lb_request_fifo_depth;

	/*ErrorF("max_req %d: 0x%x\n", i, max_req);*/

	lb_max_req_outstanding = INREG(AVIVO_LB_MAX_REQ_OUTSTANDING);
	if (i == 0) {
	    lb_max_req_outstanding &= ~(AVIVO_LB_D1_MAX_REQ_OUTSTANDING_MASK << AVIVO_LB_D1_MAX_REQ_OUTSTANDING_SHIFT);
	    lb_max_req_outstanding |= (max_req & AVIVO_LB_D1_MAX_REQ_OUTSTANDING_MASK) << AVIVO_LB_D1_MAX_REQ_OUTSTANDING_SHIFT;
	} else {
	    lb_max_req_outstanding &= ~(AVIVO_LB_D2_MAX_REQ_OUTSTANDING_MASK << AVIVO_LB_D2_MAX_REQ_OUTSTANDING_SHIFT);
	    lb_max_req_outstanding |= (max_req & AVIVO_LB_D2_MAX_REQ_OUTSTANDING_MASK) << AVIVO_LB_D2_MAX_REQ_OUTSTANDING_SHIFT;
	}
	OUTREG(AVIVO_LB_MAX_REQ_OUTSTANDING, lb_max_req_outstanding);

	/* Determine line time
	   LineTime = total time for one line of displayhtotal = total number of horizontal pixels
	   pclk = pixel clock period(ns)
	*/
	line_time = current->CrtcHTotal * pclk;

	/* Determine active time
	   ActiveTime = time of active region of display within one line,
	   hactive = total number of horizontal active pixels
	   htotal = total number of horizontal pixels
	*/
	active_time = line_time * current->CrtcHDisplay / current->CrtcHTotal;

	/* Determine chunk time
	   ChunkTime = the time it takes the DCP to send one chunk of data
	   to the LB which consists of pipeline delay and inter chunk gap
	   sclk = system clock(ns)
	*/
	if (info->IsIGP) {
	    sclk = 1000 / (available_bandwidth / 16);
	    /* Sclkfreq = sclk in MHz = 1000/sclk (because sclk is in ns). */
	    sclkfreq = 1000 / sclk;
	    chunk_time = sclk * 256 * 1.3;
	} else {
	    sclk = 1000 / ((float)info->pm.mode[info->pm.current_mode].sclk / 100);
	    chunk_time = sclk * 600;
	}

	/* Determine the worst case latency
	   NumLinePair = Number of line pairs to request(1 = 2 lines, 2 = 4 lines)
	   WorstCaseLatency = The worst case time from urgent to when the MC starts
	   to return data
	   READ_DELAY_IDLE_MAX = constant of 1us
	   ChunkTime = the time it takes the DCP to send one chunk of data to the LB
	   which consists of pipeline delay and
	   inter chunk gap
	*/
	if (info->IsIGP) {
	    if (num_line_pair > 1)
		worst_case_latency = read_delay_latency + 3 * chunk_time;
	    else
		worst_case_latency = read_delay_latency + 2 * chunk_time;
	} else {
	    if (num_line_pair > 1)
		worst_case_latency = read_delay_latency + 3 * chunk_time;
	    else
		worst_case_latency = read_delay_latency + chunk_time;
	}

	/* Determine the tolerable latency
	   TolerableLatency = Any given request has only 1 line time for the data to be returned
	   LBRequestFifoDepth = Number of chunk requests the LB can put into the request FIFO for a display
	   LineTime = total time for one line of display
	   ChunkTime = the time it takes the DCP to send one chunk of data to the LB which consists of
	   pipeline delay and inter chunk gap
	*/
	if ((2 + lb_request_fifo_depth) >= request_fifo_depth)
	    tolerable_latency = line_time;
	else
	    tolerable_latency = line_time - (request_fifo_depth - lb_request_fifo_depth - 2) * chunk_time;

	if (mode1 && mode2) {
	    int d1bpp, d2bpp;
	    int d1_graph_enable = 1;
	    int d2_graph_enable = 1;
	    int d1_ovl_enable = 0;
	    int d2_ovl_enable = 0;
	    int d1grph_depth, d2grph_depth;
	    int d1ovl_depth = 0;
	    int d2ovl_depth = 0;
	    int d1_num_line_pair, d2_num_line_pair;
	    float d1_fill_rate_coeff, d2_fill_rate_coeff;

	    switch (pixel_bytes1) {
	    case 2:
		d1grph_depth = 1;
		break;
	    case 4:
		d1grph_depth = 2;
		break;
	    default:
		d1grph_depth = 0;
		break;
	    }

	    switch (pixel_bytes2) {
	    case 2:
		d2grph_depth = 1;
		break;
	    case 4:
		d2grph_depth = 2;
		break;
	    default:
		d2grph_depth = 0;
		break;
	    }

	    /* If both displays are active, determine line buffer fill rate */
	    if (d1_scale_en && (vsc1 > 2))
		d1_num_line_pair = 2;
	    else
		d1_num_line_pair = 1;

	    if (d2_scale_en && (vsc2 > 2))
		d2_num_line_pair = 2;
	    else
		d2_num_line_pair = 1;

	    if (info->IsIGP) {
		d1bpp = (d1_graph_enable * pow(2, d1grph_depth) * 8) + (d1_ovl_enable * pow(2, d1ovl_depth) * 8);
		d2bpp = (d2_graph_enable * pow(2, d2grph_depth) * 8) + (d2_ovl_enable * pow(2, d2ovl_depth) * 8);

		if (d1bpp > 64)
		    d1_fill_rate_coeff = d1bpp * d1_num_line_pair;
		else
		    d1_fill_rate_coeff = d1_num_line_pair;

		if (d2bpp > 64)
		    d2_fill_rate_coeff = d2bpp * d2_num_line_pair;
		else
		    d2_fill_rate_coeff = d2_num_line_pair;

		fill_rate = sclkfreq / (d1_fill_rate_coeff + d2_fill_rate_coeff);
	    } else {
		d1bpp = (d1grph_depth + d1ovl_depth) * 16;
		d2bpp = (d2grph_depth + d2ovl_depth) * 16;

		if (d1bpp > 64)
		    d1_fill_rate_coeff = d1bpp / d1_num_line_pair;
		else
		    d1_fill_rate_coeff = d1_num_line_pair;

		if (d2bpp > 64)
		    d2_fill_rate_coeff = d2bpp / d2_num_line_pair;
		else
		    d2_fill_rate_coeff = d2_num_line_pair;

		fill_rate = sclk / (d1_fill_rate_coeff + d2_fill_rate_coeff);

		/* Convert line buffer fill rate from period to frequency */
		fill_rate = 1 / fill_rate;
	    }
	} else {
	    int dxbpp;
	    int dx_grph_enable = 1;
	    int dx_ovl_enable = 0;
	    int dxgrph_depth;
	    int dxovl_depth = 0;
	    int cpp;

	    if (i == 0)
		cpp = pixel_bytes1;
	    else
		cpp = pixel_bytes2;

	    switch (cpp) {
	    case 2:
		dxgrph_depth = 1;
		break;
	    case 4:
		dxgrph_depth = 2;
		break;
	    default:
		dxgrph_depth = 0;
		break;
	    }

	    /* If only one display active, the line buffer fill rate becomes */
	    if (info->IsIGP) {
		dxbpp = (dx_grph_enable * pow(2, dxgrph_depth) * 8) + (dx_ovl_enable * pow(2, dxovl_depth) * 8);
		if (dxbpp > 64)
		    fill_rate = sclkfreq / dxbpp / num_line_pair;
		else
		    fill_rate = sclkfreq / num_line_pair;
	    } else {
		dxbpp = (dxgrph_depth + dxovl_depth) * 16;

		if (dxbpp > 64)
		    fill_rate = sclk / dxbpp / num_line_pair;
		else
		    fill_rate = sclk / num_line_pair;

		/* Convert line buffer fill rate from period to frequency */
		fill_rate = 1 / fill_rate;
	    }
	}

	/* Determine the maximum priority mark
	   width = viewport width in pixels
	*/
	priority_mark_max = ceil(width/16);

	/* Determine estimated width */
	estimated_width = (tolerable_latency - worst_case_latency) / consumption_time;

	/* Determine priority mark based on active time */
	if (info->IsIGP) {
	    if (estimated_width > width)
		priority_mark = 10;
	    else
		priority_mark = priority_mark_max - ceil(estimated_width / 16);
	} else {
	    if (estimated_width > width)
		priority_mark = priority_mark_max;
	    else
		priority_mark = priority_mark_max - ceil(estimated_width / 16);
	}

	/* Determine priority mark 2 based on worst case latency,
	   consumption rate, fill rate and active time
	*/
	if (info->IsIGP) {
	    if (consumption_rate > fill_rate)
		priority_mark2 = ceil((worst_case_latency * consumption_rate + (consumption_rate - fill_rate) * active_time) / 1000 / 16);
	    else
		priority_mark2 = ceil(worst_case_latency * consumption_rate / 1000 / 16);
	} else {
	    if (consumption_rate > fill_rate)
		priority_mark2 = ceil(worst_case_latency * consumption_rate + (consumption_rate - fill_rate) * active_time / 16);
	    else
		priority_mark2 = ceil(worst_case_latency * consumption_rate / 16);
	}

	/* Determine final priority mark and clamp if necessary */
	priority_mark = max(priority_mark, priority_mark2);
	if (priority_mark < 0)
	    priority_mark = 0;
	else if (priority_mark > priority_mark_max)
	    priority_mark = priority_mark_max;

	priority_cnt = priority_mark & AVIVO_DxMODE_PRIORITY_MARK_MASK;

	if (info->DispPriority == 2)
	    priority_cnt |= AVIVO_DxMODE_PRIORITY_ALWAYS_ON;

	/*ErrorF("priority_mark %d: 0x%x\n", i, priority_mark);*/

	/* Determine which display to program priority mark for */
	/* FIXME: program DxMODE_PRIORITY_B_CNT for slower sclk */
	if (i == 0) {
	    OUTREG(AVIVO_D1MODE_PRIORITY_A_CNT, priority_cnt);
	    OUTREG(AVIVO_D1MODE_PRIORITY_B_CNT, priority_cnt);
	} else {
	    OUTREG(AVIVO_D2MODE_PRIORITY_A_CNT, priority_cnt);
	    OUTREG(AVIVO_D2MODE_PRIORITY_B_CNT, priority_cnt);
	}
    }

}
