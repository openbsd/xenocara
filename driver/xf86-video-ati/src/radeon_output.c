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
#include <fcntl.h>

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

const char *encoder_name[34] = {
    "NONE",
    "INTERNAL_LVDS",
    "INTERNAL_TMDS1",
    "INTERNAL_TMDS2",
    "INTERNAL_DAC1",
    "INTERNAL_DAC2",
    "INTERNAL_SDVOA",
    "INTERNAL_SDVOB",
    "SI170B",
    "CH7303",
    "CH7301",
    "INTERNAL_DVO1",
    "EXTERNAL_SDVOA",
    "EXTERNAL_SDVOB",
    "TITFP513",
    "INTERNAL_LVTM1",
    "VT1623",
    "HDMI_SI1930",
    "HDMI_INTERNAL",
    "INTERNAL_KLDSCP_TMDS1",
    "INTERNAL_KLDSCP_DVO1",
    "INTERNAL_KLDSCP_DAC1",
    "INTERNAL_KLDSCP_DAC2",
    "SI178",
    "MVPU_FPGA",
    "INTERNAL_DDI",
    "VT1625",
    "HDMI_SI1932",
    "DP_AN9801",
    "DP_DP501",
    "INTERNAL_UNIPHY",
    "INTERNAL_KLDSCP_LVTMA",
    "INTERNAL_UNIPHY1",
    "INTERNAL_UNIPHY2",
};

const char *ConnectorTypeName[18] = {
  "None",
  "VGA",
  "DVI-I",
  "DVI-D",
  "DVI-A",
  "S-video",
  "Composite",
  "LVDS",
  "Digital",
  "SCART",
  "HDMI-A",
  "HDMI-B",
  "Unsupported",
  "Unsupported",
  "DIN",
  "DisplayPort",
  "eDP",
  "Unsupported"
};

extern void atombios_output_mode_set(xf86OutputPtr output,
				     DisplayModePtr mode,
				     DisplayModePtr adjusted_mode);
extern void atombios_output_dpms(xf86OutputPtr output, int mode);
extern RADEONMonitorType atombios_dac_detect(xf86OutputPtr output);
extern AtomBiosResult
atombios_lock_crtc(atomBiosHandlePtr atomBIOS, int crtc, int lock);
static void
radeon_bios_output_dpms(xf86OutputPtr output, int mode);
static void
radeon_bios_output_crtc(xf86OutputPtr output);
static void
radeon_bios_output_lock(xf86OutputPtr output, Bool lock);
extern void
atombios_pick_dig_encoder(xf86OutputPtr output);

void RADEONPrintPortMap(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    RADEONOutputPrivatePtr radeon_output;
    xf86OutputPtr output;
    int o;

    for (o = 0; o < xf86_config->num_output; o++) {
	output = xf86_config->output[o];
	radeon_output = output->driver_private;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Port%d:\n", o);
	ErrorF("  XRANDR name: %s\n", output->name);
	ErrorF("  Connector: %s\n", ConnectorTypeName[radeon_output->ConnectorType]);
	if (radeon_output->devices & ATOM_DEVICE_CRT1_SUPPORT)
	    ErrorF("  CRT1: %s\n", encoder_name[info->encoders[ATOM_DEVICE_CRT1_INDEX]->encoder_id]);
	if (radeon_output->devices & ATOM_DEVICE_CRT2_SUPPORT)
	    ErrorF("  CRT2: %s\n", encoder_name[info->encoders[ATOM_DEVICE_CRT2_INDEX]->encoder_id]);
	if (radeon_output->devices & ATOM_DEVICE_LCD1_SUPPORT)
	    ErrorF("  LCD1: %s\n", encoder_name[info->encoders[ATOM_DEVICE_LCD1_INDEX]->encoder_id]);
	if (radeon_output->devices & ATOM_DEVICE_DFP1_SUPPORT)
	    ErrorF("  DFP1: %s\n", encoder_name[info->encoders[ATOM_DEVICE_DFP1_INDEX]->encoder_id]);
	if (radeon_output->devices & ATOM_DEVICE_DFP2_SUPPORT)
	    ErrorF("  DFP2: %s\n", encoder_name[info->encoders[ATOM_DEVICE_DFP2_INDEX]->encoder_id]);
	if (radeon_output->devices & ATOM_DEVICE_DFP3_SUPPORT)
	    ErrorF("  DFP3: %s\n", encoder_name[info->encoders[ATOM_DEVICE_DFP3_INDEX]->encoder_id]);
	if (radeon_output->devices & ATOM_DEVICE_DFP4_SUPPORT)
	    ErrorF("  DFP4: %s\n", encoder_name[info->encoders[ATOM_DEVICE_DFP4_INDEX]->encoder_id]);
	if (radeon_output->devices & ATOM_DEVICE_DFP5_SUPPORT)
	    ErrorF("  DFP5: %s\n", encoder_name[info->encoders[ATOM_DEVICE_DFP5_INDEX]->encoder_id]);
	if (radeon_output->devices & ATOM_DEVICE_TV1_SUPPORT)
	    ErrorF("  TV1: %s\n", encoder_name[info->encoders[ATOM_DEVICE_TV1_INDEX]->encoder_id]);
	if (radeon_output->devices & ATOM_DEVICE_CV_SUPPORT)
	    ErrorF("  CV: %s\n", encoder_name[info->encoders[ATOM_DEVICE_CV_INDEX]->encoder_id]);
	ErrorF("  DDC reg: 0x%x\n",(unsigned int)radeon_output->ddc_i2c.mask_clk_reg);
    }

}

static void
radeon_set_active_device(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    radeon_output->active_device = 0;

    switch (radeon_output->MonType) {
    case MT_DP:
    case MT_DFP:
	if (radeon_output->devices & ATOM_DEVICE_DFP1_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_DFP1_SUPPORT;
	else if (radeon_output->devices & ATOM_DEVICE_DFP2_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_DFP2_SUPPORT;
	else if (radeon_output->devices & ATOM_DEVICE_DFP3_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_DFP3_SUPPORT;
	else if (radeon_output->devices & ATOM_DEVICE_DFP4_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_DFP4_SUPPORT;
	else if (radeon_output->devices & ATOM_DEVICE_DFP5_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_DFP5_SUPPORT;
	else if (radeon_output->devices & ATOM_DEVICE_LCD1_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_LCD1_SUPPORT;
	else if (radeon_output->devices & ATOM_DEVICE_LCD2_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_LCD2_SUPPORT;
	break;
    case MT_CRT:
	if (radeon_output->devices & ATOM_DEVICE_CRT1_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_CRT1_SUPPORT;
	else if (radeon_output->devices & ATOM_DEVICE_CRT2_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_CRT2_SUPPORT;
	break;
    case MT_LCD:
	if (radeon_output->devices & ATOM_DEVICE_LCD1_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_LCD1_SUPPORT;
	else if (radeon_output->devices & ATOM_DEVICE_LCD2_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_LCD2_SUPPORT;
	break;
    case MT_STV:
    case MT_CTV:
	if (radeon_output->devices & ATOM_DEVICE_TV1_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_TV1_SUPPORT;
	else if (radeon_output->devices & ATOM_DEVICE_TV2_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_TV2_SUPPORT;
	break;
    case MT_CV:
	if (radeon_output->devices & ATOM_DEVICE_CV_SUPPORT)
	    radeon_output->active_device = ATOM_DEVICE_CV_SUPPORT;
	break;
    default:
	ErrorF("Unhandled monitor type %d\n", radeon_output->MonType);
	radeon_output->active_device = 0;
    }
}

static Bool
monitor_is_digital(xf86MonPtr MonInfo)
{
    return (MonInfo->rawData[0x14] & 0x80) != 0;
}

static void
RADEONGetHardCodedEDIDFromFile(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    char *EDIDlist = (char *)xf86GetOptValString(info->Options, OPTION_CUSTOM_EDID);

    radeon_output->custom_edid = FALSE;
    radeon_output->custom_mon = NULL;

    if (EDIDlist != NULL) {
	unsigned char* edid = xnfcalloc(128, 1);
	char *name = output->name;
	char *outputEDID = strstr(EDIDlist, name);

	if (outputEDID != NULL) {
	    char *end;
	    char *colon;
	    char *command = NULL;
	    int fd;

	    outputEDID += strlen(name) + 1;
	    end = strstr(outputEDID, ";");
	    if (end != NULL)
		*end = 0;

	    colon = strstr(outputEDID, ":");
	    if (colon != NULL) {
		*colon = 0;
		command = colon + 1;
	    }

	    fd = open (outputEDID, O_RDONLY);
	    if (fd >= 0) {
		read(fd, edid, 128);
		close(fd);
		if (edid[1] == 0xff) {
		    radeon_output->custom_mon = xf86InterpretEDID(output->scrn->scrnIndex, edid);
		    radeon_output->custom_edid = TRUE;
		    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			       "Successfully read Custom EDID data for output %s from %s.\n",
			       name, outputEDID);
		    if (command != NULL) {
			if (!strcmp(command, "digital")) {
			    radeon_output->custom_mon->rawData[0x14] |= 0x80;
			    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
				       "Forcing digital output for output %s.\n", name);
			} else if (!strcmp(command, "analog")) {
			    radeon_output->custom_mon->rawData[0x14] &= ~0x80;
			    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
				       "Forcing analog output for output %s.\n", name);
			} else {
			    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
				       "Unknown custom EDID command: '%s'.\n",
				       command);
			}
		    }
		} else {
		    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			       "Custom EDID data for %s read from %s was invalid.\n",
			       name, outputEDID);
		}
	    } else {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "Could not read custom EDID for output %s from file %s.\n",
			   name, outputEDID);
	    }
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Could not find EDID file name for output %s; using auto detection.\n",
		       name);
	}
    }
}


static RADEONMonitorType
radeon_ddc_connected(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn        = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONMonitorType MonType = MT_NONE;
    xf86MonPtr MonInfo = NULL;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int ret;

    if (radeon_output->custom_edid) {
	MonInfo = xnfcalloc(sizeof(xf86Monitor), 1);
	*MonInfo = *radeon_output->custom_mon;
    } else if ((radeon_output->ConnectorType == CONNECTOR_DISPLAY_PORT) ||
	       (radeon_output->ConnectorType == CONNECTOR_EDP)) {
	ret = RADEON_DP_GetSinkType(output);
	if (ret == CONNECTOR_OBJECT_ID_DISPLAYPORT ||
	    ret == CONNECTOR_OBJECT_ID_eDP) {
		MonInfo = xf86OutputGetEDID(output, radeon_output->dp_pI2CBus);
	}
	if (MonInfo == NULL) {
	    if (radeon_output->pI2CBus) {
		RADEONI2CDoLock(output, radeon_output->pI2CBus, TRUE);
		MonInfo = xf86OutputGetEDID(output, radeon_output->pI2CBus);
		RADEONI2CDoLock(output, radeon_output->pI2CBus, FALSE);
	    }
	}
    } else if (radeon_output->pI2CBus) {
	if (info->get_hardcoded_edid_from_bios)
	    MonInfo = RADEONGetHardCodedEDIDFromBIOS(output);
	if (MonInfo == NULL) {
	    RADEONI2CDoLock(output, radeon_output->pI2CBus, TRUE);
	    MonInfo = xf86OutputGetEDID(output, radeon_output->pI2CBus);
	    RADEONI2CDoLock(output, radeon_output->pI2CBus, FALSE);
	}
    }
    if (MonInfo) {
	switch (radeon_output->ConnectorType) {
	case CONNECTOR_LVDS:
	    MonType = MT_LCD;
	    break;
	case CONNECTOR_DVI_D:
	case CONNECTOR_HDMI_TYPE_A:
	    if (radeon_output->shared_ddc) {
		xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR (output->scrn);
		int i;

		if (monitor_is_digital(MonInfo))
		    MonType = MT_DFP;
		else
		    MonType = MT_NONE;

		for (i = 0; i < config->num_output; i++) {
		    if (output != config->output[i]) {
			RADEONOutputPrivatePtr other_radeon_output =
			    config->output[i]->driver_private;
			if (radeon_output->devices & other_radeon_output->devices) {
#ifndef EDID_COMPLETE_RAWDATA
			    if (radeon_output->ConnectorType == CONNECTOR_HDMI_TYPE_A) {
				MonType = MT_NONE;
				break;
			    }
#else
			    if (xf86MonitorIsHDMI(MonInfo)) {
				if (radeon_output->ConnectorType == CONNECTOR_DVI_D) {
				    MonType = MT_NONE;
				    break;
				}
			    } else {
				if (radeon_output->ConnectorType == CONNECTOR_HDMI_TYPE_A) {
				    MonType = MT_NONE;
				    break;
				}
			    }
#endif
			}
		    }
		}
	    } else
		MonType = MT_DFP;
	    break;
	case CONNECTOR_DISPLAY_PORT:
	case CONNECTOR_EDP:
	    /*
	     * XXX wrong. need to infer based on whether we got DDC from I2C
	     * or AUXCH.
	     */
	    ret = RADEON_DP_GetSinkType(output);

	    if ((ret == CONNECTOR_OBJECT_ID_DISPLAYPORT) ||
		(ret == CONNECTOR_OBJECT_ID_eDP)) {
		MonType = MT_DP;
		RADEON_DP_GetDPCD(output);
	    } else
		MonType = MT_DFP;
	    break;
	case CONNECTOR_HDMI_TYPE_B:
	case CONNECTOR_DVI_I:
	    if (monitor_is_digital(MonInfo))
		MonType = MT_DFP;
	    else
		MonType = MT_CRT;
	    break;
	case CONNECTOR_VGA:
	case CONNECTOR_DVI_A:
	default:
	    if (radeon_output->shared_ddc) {
		if (monitor_is_digital(MonInfo))
		    MonType = MT_NONE;
		else
		    MonType = MT_CRT;
	    } else
		MonType = MT_CRT;
	    break;
	}

	if (MonType != MT_NONE) {
	    if (!xf86ReturnOptValBool(info->Options, OPTION_IGNORE_EDID, FALSE))
		xf86OutputSetEDID(output, MonInfo);
	} else
	    free(MonInfo);
    } else
	MonType = MT_NONE;

    return MonType;
}

#ifndef __powerpc__

static RADEONMonitorType
RADEONDetectLidStatus(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONMonitorType MonType = MT_NONE;
#ifdef __linux__
    char lidline[50];  /* 50 should be sufficient for our purposes */
    FILE *f = fopen ("/proc/acpi/button/lid/LID/state", "r");

    if (f != NULL) {
	while (fgets(lidline, sizeof lidline, f)) {
	    if (!strncmp(lidline, "state:", strlen ("state:"))) {
		if (strstr(lidline, "open")) {
		    fclose(f);
		    ErrorF("proc lid open\n");
		    return MT_LCD;
		}
		else if (strstr(lidline, "closed")) {
		    fclose(f);
		    ErrorF("proc lid closed\n");
		    return MT_NONE;
		}
	    }
	}
	fclose(f);
    }
#endif

    if (!info->IsAtomBios) {
	unsigned char *RADEONMMIO = info->MMIO;

	/* see if the lid is closed -- only works at boot */
	if (INREG(RADEON_BIOS_6_SCRATCH) & 0x10)
	    MonType = MT_NONE;
	else
	    MonType = MT_LCD;
    } else
	MonType = MT_LCD;

    return MonType;
}

#endif /* __powerpc__ */

static void
radeon_dpms(xf86OutputPtr output, int mode)
{
    RADEONInfoPtr info = RADEONPTR(output->scrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if ((mode == DPMSModeOn) && radeon_output->enabled)
	return;

    if ((mode != DPMSModeOn) && radeon_output->shared_ddc) {
	xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR (output->scrn);
	int i;

	for (i = 0; i < config->num_output; i++) {
	    if (output != config->output[i]) {
		RADEONOutputPrivatePtr other_radeon_output =
		    config->output[i]->driver_private;
		if (radeon_output->devices & other_radeon_output->devices) {
		    if (output->status == XF86OutputStatusDisconnected)
			return;
		}
	    }
	}
    }

    if (IS_AVIVO_VARIANT || info->r4xx_atom) {
	atombios_output_dpms(output, mode);
    } else {
	legacy_output_dpms(output, mode);
    }
    radeon_bios_output_dpms(output, mode);

    if (mode == DPMSModeOn)
	radeon_output->enabled = TRUE;
    else
	radeon_output->enabled = FALSE;

}

static void
radeon_save(xf86OutputPtr output)
{

}

static void
radeon_restore(xf86OutputPtr restore)
{

}

static int
radeon_mode_valid(xf86OutputPtr output, DisplayModePtr pMode)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_native_mode_ptr native_mode = &radeon_output->native_mode;
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

    /*
     * RN50 has effective maximum mode bandwidth of about 300MiB/s.
     * XXX should really do this for all chips by properly computing
     * memory bandwidth and an overhead factor.
     */
    if (info->ChipFamily == CHIP_FAMILY_RV100 && !pRADEONEnt->HasCRTC2) {
	if (xf86ModeBandwidth(pMode, pScrn->bitsPerPixel) > 300)
	    return MODE_BANDWIDTH;
    }

    if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT)) {
	if (IS_AVIVO_VARIANT)
	    return MODE_OK;
	else {
	    /* FIXME: Update when more modes are added */
	    if (pMode->HDisplay == 800 && pMode->VDisplay == 600)
		return MODE_OK;
	    else
		return MODE_CLOCK_RANGE;
	}
    }

    /* clocks over 135 MHz have heat issues with DVI on RV100 */
    if ((radeon_output->MonType == MT_DFP) &&
	(info->ChipFamily == CHIP_FAMILY_RV100) &&
	(pMode->Clock > 135000))
	    return MODE_CLOCK_HIGH;

    /* single link DVI check */
    if (pMode->Clock > 165000 && radeon_output->MonType == MT_DFP) {
	/* DP->DVI converter */
	if (radeon_output->ConnectorType == CONNECTOR_DISPLAY_PORT)
	    return MODE_CLOCK_HIGH;

	if (radeon_output->ConnectorType == CONNECTOR_EDP)
	    return MODE_CLOCK_HIGH;

	/* XXX some HDMI can do better than 165MHz on a link */
	if (radeon_output->ConnectorType == CONNECTOR_HDMI_TYPE_A)
	    return MODE_CLOCK_HIGH;

	/* XXX some R300 and R400 can actually do this */
	if (!IS_AVIVO_VARIANT)
	    return MODE_CLOCK_HIGH;

	/* XXX and some AVIVO can't */
    }

    if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT)) {
	if (radeon_output->rmx_type == RMX_OFF) {
	    if (pMode->HDisplay != native_mode->PanelXRes ||
		pMode->VDisplay != native_mode->PanelYRes)
		return MODE_PANEL;
	}
	if (pMode->HDisplay > native_mode->PanelXRes ||
	    pMode->VDisplay > native_mode->PanelYRes)
	    return MODE_PANEL;
    }

    return MODE_OK;
}

static Bool
radeon_mode_fixup(xf86OutputPtr output, DisplayModePtr mode,
		    DisplayModePtr adjusted_mode)
{
    RADEONInfoPtr info = RADEONPTR(output->scrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_native_mode_ptr native_mode = &radeon_output->native_mode;
    xf86CrtcPtr crtc = output->crtc;
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;

    radeon_output->Flags &= ~RADEON_USE_RMX;
    radeon_crtc->scaler_enabled = FALSE;

    /*
     *  Refresh the Crtc values without INTERLACE_HALVE_V
     *  Should we use output->scrn->adjustFlags like xf86RandRModeConvert() does?
     */
    xf86SetModeCrtc(adjusted_mode, 0);

    /* decide if we are using RMX */
    if ((radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT | ATOM_DEVICE_DFP_SUPPORT))
	&& radeon_output->rmx_type != RMX_OFF) {

	if (IS_AVIVO_VARIANT || radeon_crtc->crtc_id == 0) {
	    if (mode->HDisplay < native_mode->PanelXRes ||
		mode->VDisplay < native_mode->PanelYRes) {
		radeon_output->Flags |= RADEON_USE_RMX;
		radeon_crtc->scaler_enabled = TRUE;
		if (IS_AVIVO_VARIANT) {
		    radeon_crtc->hsc = (float)mode->HDisplay / (float)native_mode->PanelXRes;
		    radeon_crtc->vsc = (float)mode->VDisplay / (float)native_mode->PanelYRes;
		    /* set to the panel's native mode */
		    adjusted_mode->HDisplay = native_mode->PanelXRes;
		    adjusted_mode->VDisplay = native_mode->PanelYRes;
		    adjusted_mode->HTotal = native_mode->PanelXRes + native_mode->HBlank;
		    adjusted_mode->HSyncStart = native_mode->PanelXRes + native_mode->HOverPlus;
		    adjusted_mode->HSyncEnd = adjusted_mode->HSyncStart + native_mode->HSyncWidth;
		    adjusted_mode->VTotal = native_mode->PanelYRes + native_mode->VBlank;
		    adjusted_mode->VSyncStart = native_mode->PanelYRes + native_mode->VOverPlus;
		    adjusted_mode->VSyncEnd = adjusted_mode->VSyncStart + native_mode->VSyncWidth;
		    /* update crtc values */
		    xf86SetModeCrtc(adjusted_mode, INTERLACE_HALVE_V);
		    /* adjust crtc values */
		    adjusted_mode->CrtcHDisplay = native_mode->PanelXRes;
		    adjusted_mode->CrtcVDisplay = native_mode->PanelYRes;
		    adjusted_mode->CrtcHTotal = adjusted_mode->CrtcHDisplay + native_mode->HBlank;
		    adjusted_mode->CrtcHSyncStart = adjusted_mode->CrtcHDisplay + native_mode->HOverPlus;
		    adjusted_mode->CrtcHSyncEnd = adjusted_mode->CrtcHSyncStart + native_mode->HSyncWidth;
		    adjusted_mode->CrtcVTotal = adjusted_mode->CrtcVDisplay + native_mode->VBlank;
		    adjusted_mode->CrtcVSyncStart = adjusted_mode->CrtcVDisplay + native_mode->VOverPlus;
		    adjusted_mode->CrtcVSyncEnd = adjusted_mode->CrtcVSyncStart + native_mode->VSyncWidth;
		} else {
		    /* set to the panel's native mode */
		    adjusted_mode->HTotal = native_mode->PanelXRes + native_mode->HBlank;
		    adjusted_mode->HSyncStart = native_mode->PanelXRes + native_mode->HOverPlus;
		    adjusted_mode->HSyncEnd = adjusted_mode->HSyncStart + native_mode->HSyncWidth;
		    adjusted_mode->VTotal = native_mode->PanelYRes + native_mode->VBlank;
		    adjusted_mode->VSyncStart = native_mode->PanelYRes + native_mode->VOverPlus;
		    adjusted_mode->VSyncEnd = adjusted_mode->VSyncStart + native_mode->VSyncWidth;
		    adjusted_mode->Clock = native_mode->DotClock;
		    /* update crtc values */
		    xf86SetModeCrtc(adjusted_mode, INTERLACE_HALVE_V);
		    /* adjust crtc values */
		    adjusted_mode->CrtcHTotal = adjusted_mode->CrtcHDisplay + native_mode->HBlank;
		    adjusted_mode->CrtcHSyncStart = adjusted_mode->CrtcHDisplay + native_mode->HOverPlus;
		    adjusted_mode->CrtcHSyncEnd = adjusted_mode->CrtcHSyncStart + native_mode->HSyncWidth;
		    adjusted_mode->CrtcVTotal = adjusted_mode->CrtcVDisplay + native_mode->VBlank;
		    adjusted_mode->CrtcVSyncStart = adjusted_mode->CrtcVDisplay + native_mode->VOverPlus;
		    adjusted_mode->CrtcVSyncEnd = adjusted_mode->CrtcVSyncStart + native_mode->VSyncWidth;
		}
		adjusted_mode->Clock = native_mode->DotClock;
		adjusted_mode->Flags = native_mode->Flags;
	    }
	}
    }

    /* FIXME: vsc/hsc */
    if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT | ATOM_DEVICE_CV_SUPPORT)) {
	radeon_crtc->scaler_enabled = TRUE;
	radeon_crtc->hsc = (float)mode->HDisplay / (float)640;
	radeon_crtc->vsc = (float)mode->VDisplay / (float)480;
    }

    if (IS_AVIVO_VARIANT) {
	/* hw bug */
	if ((mode->Flags & V_INTERLACE)
	    && (adjusted_mode->CrtcVSyncStart < (adjusted_mode->CrtcVDisplay + 2)))
	    adjusted_mode->CrtcVSyncStart = adjusted_mode->CrtcVDisplay + 2;
    }

    if (IS_AVIVO_VARIANT || info->r4xx_atom) {
	if (radeon_output->MonType == MT_STV || radeon_output->MonType == MT_CTV) {
	    radeon_tvout_ptr tvout = &radeon_output->tvout;
	    ScrnInfoPtr pScrn = output->scrn;

	    if (tvout->tvStd == TV_STD_NTSC ||
		tvout->tvStd == TV_STD_NTSC_J ||
		tvout->tvStd == TV_STD_PAL_M)
		RADEONATOMGetTVTimings(pScrn, 0, adjusted_mode);
	    else
		RADEONATOMGetTVTimings(pScrn, 1, adjusted_mode);
	}
    }

    if (((radeon_output->ConnectorType == CONNECTOR_DISPLAY_PORT) ||
	 (radeon_output->ConnectorType == CONNECTOR_EDP)) &&
	(radeon_output->MonType == MT_DP)) {
      radeon_dp_mode_fixup(output, mode, adjusted_mode);
    }
    return TRUE;
}

static void
radeon_mode_prepare(xf86OutputPtr output)
{
    RADEONInfoPtr info = RADEONPTR(output->scrn);
    xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR (output->scrn);
    int o;

    for (o = 0; o < config->num_output; o++) {
	xf86OutputPtr loop_output = config->output[o];
	if (loop_output == output)
	    continue;
	else if (loop_output->crtc) {
	    xf86CrtcPtr other_crtc = loop_output->crtc;
	    RADEONCrtcPrivatePtr other_radeon_crtc = other_crtc->driver_private;
	    if (other_crtc->enabled) {
		if (other_radeon_crtc->initialized) {
		    radeon_crtc_dpms(other_crtc, DPMSModeOff);
		    if (IS_AVIVO_VARIANT || info->r4xx_atom)
			atombios_lock_crtc(info->atomBIOS, other_radeon_crtc->crtc_id, 1);
		    radeon_dpms(loop_output, DPMSModeOff);
		}
	    }
	}
    }

    radeon_bios_output_lock(output, TRUE);
    if (IS_AVIVO_VARIANT)
	atombios_pick_dig_encoder(output);
    radeon_dpms(output, DPMSModeOff);
    radeon_crtc_dpms(output->crtc, DPMSModeOff);

    if (IS_AVIVO_VARIANT || info->r4xx_atom)
        atombios_set_output_crtc_source(output);

}

static void
radeon_mode_set(xf86OutputPtr output, DisplayModePtr mode,
		DisplayModePtr adjusted_mode)
{
    RADEONInfoPtr info = RADEONPTR(output->scrn);

    if (IS_AVIVO_VARIANT || info->r4xx_atom)
	atombios_output_mode_set(output, mode, adjusted_mode);
    else
	legacy_output_mode_set(output, mode, adjusted_mode);
    radeon_bios_output_crtc(output);

}

static void
radeon_mode_commit(xf86OutputPtr output)
{
    RADEONInfoPtr info = RADEONPTR(output->scrn);
    xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR (output->scrn);
    int o;

    for (o = 0; o < config->num_output; o++) {
	xf86OutputPtr loop_output = config->output[o];
	if (loop_output == output)
	    continue;
	else if (loop_output->crtc) {
	    xf86CrtcPtr other_crtc = loop_output->crtc;
	    RADEONCrtcPrivatePtr other_radeon_crtc = other_crtc->driver_private;
	    if (other_crtc->enabled) {
		if (other_radeon_crtc->initialized) {
		    radeon_crtc_dpms(other_crtc, DPMSModeOn);
		    if (IS_AVIVO_VARIANT || info->r4xx_atom)
			atombios_lock_crtc(info->atomBIOS, other_radeon_crtc->crtc_id, 0);
		    radeon_dpms(loop_output, DPMSModeOn);
		}
	    }
	}
    }

    radeon_dpms(output, DPMSModeOn);
    radeon_crtc_dpms(output->crtc, DPMSModeOn);
    radeon_bios_output_lock(output, FALSE);
}

static void
radeon_bios_output_lock(xf86OutputPtr output, Bool lock)
{
    ScrnInfoPtr	    pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONSavePtr save = info->ModeReg;

    if (info->IsAtomBios) {
	if (lock) {
	    save->bios_6_scratch |= ATOM_S6_CRITICAL_STATE;
	} else {
	    save->bios_6_scratch &= ~ATOM_S6_CRITICAL_STATE;
	}
    } else {
	if (lock) {
	    save->bios_6_scratch |= RADEON_DRIVER_CRITICAL;
	} else {
	    save->bios_6_scratch &= ~RADEON_DRIVER_CRITICAL;
	}
    }
    if (info->ChipFamily >= CHIP_FAMILY_R600)
	OUTREG(R600_BIOS_6_SCRATCH, save->bios_6_scratch);
    else
	OUTREG(RADEON_BIOS_6_SCRATCH, save->bios_6_scratch);
}

static void
radeon_bios_output_dpms(xf86OutputPtr output, int mode)
{
    ScrnInfoPtr	    pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONSavePtr save = info->ModeReg;

    if (info->IsAtomBios) {
	if (radeon_output->active_device & ATOM_DEVICE_TV1_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_2_scratch &= ~ATOM_S2_TV1_DPMS_STATE;
	    else
		save->bios_2_scratch |= ATOM_S2_TV1_DPMS_STATE;
	} else if (radeon_output->active_device & ATOM_DEVICE_CV_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_2_scratch &= ~ATOM_S2_CV_DPMS_STATE;
	    else
		save->bios_2_scratch |= ATOM_S2_CV_DPMS_STATE;
	} else if (radeon_output->active_device & ATOM_DEVICE_CRT1_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_2_scratch &= ~ATOM_S2_CRT1_DPMS_STATE;
	    else
		save->bios_2_scratch |= ATOM_S2_CRT1_DPMS_STATE;
	} else if (radeon_output->active_device & ATOM_DEVICE_CRT2_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_2_scratch &= ~ATOM_S2_CRT2_DPMS_STATE;
	    else
		save->bios_2_scratch |= ATOM_S2_CRT2_DPMS_STATE;
	} else if (radeon_output->active_device & ATOM_DEVICE_LCD1_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_2_scratch &= ~ATOM_S2_LCD1_DPMS_STATE;
	    else
		save->bios_2_scratch |= ATOM_S2_LCD1_DPMS_STATE;
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP1_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_2_scratch &= ~ATOM_S2_DFP1_DPMS_STATE;
	    else
		save->bios_2_scratch |= ATOM_S2_DFP1_DPMS_STATE;
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP2_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_2_scratch &= ~ATOM_S2_DFP2_DPMS_STATE;
	    else
		save->bios_2_scratch |= ATOM_S2_DFP2_DPMS_STATE;
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP3_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_2_scratch &= ~ATOM_S2_DFP3_DPMS_STATE;
	    else
		save->bios_2_scratch |= ATOM_S2_DFP3_DPMS_STATE;
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP4_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_2_scratch &= ~ATOM_S2_DFP4_DPMS_STATE;
	    else
		save->bios_2_scratch |= ATOM_S2_DFP4_DPMS_STATE;
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP5_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_2_scratch &= ~ATOM_S2_DFP5_DPMS_STATE;
	    else
		save->bios_2_scratch |= ATOM_S2_DFP5_DPMS_STATE;
	}
	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    OUTREG(R600_BIOS_2_SCRATCH, save->bios_2_scratch);
	else
	    OUTREG(RADEON_BIOS_2_SCRATCH, save->bios_2_scratch);
    } else {
	if (mode == DPMSModeOn) {
	    save->bios_6_scratch &= ~(RADEON_DPMS_MASK | RADEON_SCREEN_BLANKING);
	    save->bios_6_scratch |= RADEON_DPMS_ON;
	} else {
	    save->bios_6_scratch &= ~RADEON_DPMS_MASK;
	    save->bios_6_scratch |= (RADEON_DPMS_OFF | RADEON_SCREEN_BLANKING);
	}
	if (radeon_output->active_device & ATOM_DEVICE_TV1_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_6_scratch |= RADEON_TV_DPMS_ON;
	    else
		save->bios_6_scratch &= ~RADEON_TV_DPMS_ON;
	} else if (radeon_output->active_device & ATOM_DEVICE_CRT1_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_6_scratch |= RADEON_CRT_DPMS_ON;
	    else
		save->bios_6_scratch &= ~RADEON_CRT_DPMS_ON;
	} else if (radeon_output->active_device & ATOM_DEVICE_CRT2_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_6_scratch |= RADEON_CRT_DPMS_ON;
	    else
		save->bios_6_scratch &= ~RADEON_CRT_DPMS_ON;
	} else if (radeon_output->active_device & ATOM_DEVICE_LCD1_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_6_scratch |= RADEON_LCD_DPMS_ON;
	    else
		save->bios_6_scratch &= ~RADEON_LCD_DPMS_ON;
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP1_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_6_scratch |= RADEON_DFP_DPMS_ON;
	    else
		save->bios_6_scratch &= ~RADEON_DFP_DPMS_ON;
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP2_SUPPORT) {
	    if (mode == DPMSModeOn)
		save->bios_6_scratch |= RADEON_DFP_DPMS_ON;
	    else
		save->bios_6_scratch &= ~RADEON_DFP_DPMS_ON;
	}
	OUTREG(RADEON_BIOS_6_SCRATCH, save->bios_6_scratch);
    }
}

static void
radeon_bios_output_crtc(xf86OutputPtr output)
{
    ScrnInfoPtr	    pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONSavePtr save = info->ModeReg;
    xf86CrtcPtr crtc = output->crtc;
    RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;

    /* no need to update crtc routing scratch regs on DCE4 */
    if (IS_DCE4_VARIANT)
	return;

    if (info->IsAtomBios) {
	if (radeon_output->active_device & ATOM_DEVICE_TV1_SUPPORT) {
	    save->bios_3_scratch &= ~ATOM_S3_TV1_CRTC_ACTIVE;
	    save->bios_3_scratch |= (radeon_crtc->crtc_id << 18);
	} else if (radeon_output->active_device & ATOM_DEVICE_CV_SUPPORT) {
	    save->bios_3_scratch &= ~ATOM_S3_CV_CRTC_ACTIVE;
	    save->bios_3_scratch |= (radeon_crtc->crtc_id << 24);
	} else if (radeon_output->active_device & ATOM_DEVICE_CRT1_SUPPORT) {
	    save->bios_3_scratch &= ~ATOM_S3_CRT1_CRTC_ACTIVE;
	    save->bios_3_scratch |= (radeon_crtc->crtc_id << 16);
	} else if (radeon_output->active_device & ATOM_DEVICE_CRT2_SUPPORT) {
	    save->bios_3_scratch &= ~ATOM_S3_CRT2_CRTC_ACTIVE;
	    save->bios_3_scratch |= (radeon_crtc->crtc_id << 20);
	} else if (radeon_output->active_device & ATOM_DEVICE_LCD1_SUPPORT) {
	    save->bios_3_scratch &= ~ATOM_S3_LCD1_CRTC_ACTIVE;
	    save->bios_3_scratch |= (radeon_crtc->crtc_id << 17);
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP1_SUPPORT) {
	    save->bios_3_scratch &= ~ATOM_S3_DFP1_CRTC_ACTIVE;
	    save->bios_3_scratch |= (radeon_crtc->crtc_id << 19);
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP2_SUPPORT) {
	    save->bios_3_scratch &= ~ATOM_S3_DFP2_CRTC_ACTIVE;
	    save->bios_3_scratch |= (radeon_crtc->crtc_id << 23);
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP3_SUPPORT) {
	    save->bios_3_scratch &= ~ATOM_S3_DFP3_CRTC_ACTIVE;
	    save->bios_3_scratch |= (radeon_crtc->crtc_id << 25);
	}
	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    OUTREG(R600_BIOS_3_SCRATCH, save->bios_3_scratch);
	else
	    OUTREG(RADEON_BIOS_3_SCRATCH, save->bios_3_scratch);
    } else {
	if (radeon_output->active_device & ATOM_DEVICE_TV1_SUPPORT) {
	    save->bios_5_scratch &= ~RADEON_TV1_CRTC_MASK;
	    save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_TV1_CRTC_SHIFT);
	} else if (radeon_output->active_device & ATOM_DEVICE_CRT1_SUPPORT) {
	    save->bios_5_scratch &= ~RADEON_CRT1_CRTC_MASK;
	    save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_CRT1_CRTC_SHIFT);
	} else if (radeon_output->active_device & ATOM_DEVICE_CRT2_SUPPORT) {
	    save->bios_5_scratch &= ~RADEON_CRT2_CRTC_MASK;
	    save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_CRT2_CRTC_SHIFT);
	} else if (radeon_output->active_device & ATOM_DEVICE_LCD1_SUPPORT) {
	    save->bios_5_scratch &= ~RADEON_LCD1_CRTC_MASK;
	    save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_LCD1_CRTC_SHIFT);
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP1_SUPPORT) {
	    save->bios_5_scratch &= ~RADEON_DFP1_CRTC_MASK;
	    save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_DFP1_CRTC_SHIFT);
	} else if (radeon_output->active_device & ATOM_DEVICE_DFP2_SUPPORT) {
	    save->bios_5_scratch &= ~RADEON_DFP2_CRTC_MASK;
	    save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_DFP2_CRTC_SHIFT);
	}
	OUTREG(RADEON_BIOS_5_SCRATCH, save->bios_5_scratch);
    }
}

static void
radeon_bios_output_connected(xf86OutputPtr output, Bool connected)
{
    ScrnInfoPtr	    pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONSavePtr save = info->ModeReg;

    if (info->IsAtomBios) {
	switch (radeon_output->active_device) {
	case ATOM_DEVICE_TV1_SUPPORT:
	    if (connected)
		save->bios_3_scratch |= ATOM_S3_TV1_ACTIVE;
	    else {
		save->bios_0_scratch &= ~ATOM_S0_TV1_MASK;
		save->bios_3_scratch &= ~ATOM_S3_TV1_ACTIVE;
	    }
	    break;
	case ATOM_DEVICE_CV_SUPPORT:
	    if (connected)
		save->bios_3_scratch |= ATOM_S3_CV_ACTIVE;
	    else {
		save->bios_0_scratch &= ~ATOM_S0_CV_MASK;
		save->bios_3_scratch &= ~ATOM_S3_CV_ACTIVE;
	    }
	    break;
	case ATOM_DEVICE_LCD1_SUPPORT:
	    if (connected) {
		save->bios_0_scratch |= ATOM_S0_LCD1;
		save->bios_3_scratch |= ATOM_S3_LCD1_ACTIVE;
	    } else {
		save->bios_0_scratch &= ~ATOM_S0_LCD1;
		save->bios_3_scratch &= ~ATOM_S3_LCD1_ACTIVE;
	    }
	    break;
	case ATOM_DEVICE_CRT1_SUPPORT:
	    if (connected) {
		save->bios_0_scratch |= ATOM_S0_CRT1_COLOR;
		save->bios_3_scratch |= ATOM_S3_CRT1_ACTIVE;
	    } else {
		save->bios_0_scratch &= ~ATOM_S0_CRT1_MASK;
		save->bios_3_scratch &= ~ATOM_S3_CRT1_ACTIVE;
	    }
	    break;
	case ATOM_DEVICE_CRT2_SUPPORT:
	    if (connected) {
		save->bios_0_scratch |= ATOM_S0_CRT2_COLOR;
		save->bios_3_scratch |= ATOM_S3_CRT2_ACTIVE;
	    } else {
		save->bios_0_scratch &= ~ATOM_S0_CRT2_MASK;
		save->bios_3_scratch &= ~ATOM_S3_CRT2_ACTIVE;
	    }
	    break;
	case ATOM_DEVICE_DFP1_SUPPORT:
	    if (connected) {
		save->bios_0_scratch |= ATOM_S0_DFP1;
		save->bios_3_scratch |= ATOM_S3_DFP1_ACTIVE;
	    } else {
		save->bios_0_scratch &= ~ATOM_S0_DFP1;
		save->bios_3_scratch &= ~ATOM_S3_DFP1_ACTIVE;
	    }
	    break;
	case ATOM_DEVICE_DFP2_SUPPORT:
	    if (connected) {
		save->bios_0_scratch |= ATOM_S0_DFP2;
		save->bios_3_scratch |= ATOM_S3_DFP2_ACTIVE;
	    } else {
		save->bios_0_scratch &= ~ATOM_S0_DFP2;
		save->bios_3_scratch &= ~ATOM_S3_DFP2_ACTIVE;
	    }
	    break;
	case ATOM_DEVICE_DFP3_SUPPORT:
	    if (connected) {
		save->bios_0_scratch |= ATOM_S0_DFP3;
		save->bios_3_scratch |= ATOM_S3_DFP3_ACTIVE;
	    } else {
		save->bios_0_scratch &= ~ATOM_S0_DFP3;
		save->bios_3_scratch &= ~ATOM_S3_DFP3_ACTIVE;
	    }
	    break;
	case ATOM_DEVICE_DFP4_SUPPORT:
	    if (connected) {
		save->bios_0_scratch |= ATOM_S0_DFP4;
		save->bios_3_scratch |= ATOM_S3_DFP4_ACTIVE;
	    } else {
		save->bios_0_scratch &= ~ATOM_S0_DFP4;
		save->bios_3_scratch &= ~ATOM_S3_DFP4_ACTIVE;
	    }
	    break;
	case ATOM_DEVICE_DFP5_SUPPORT:
	    if (connected) {
		save->bios_0_scratch |= ATOM_S0_DFP5;
		save->bios_3_scratch |= ATOM_S3_DFP5_ACTIVE;
	    } else {
		save->bios_0_scratch &= ~ATOM_S0_DFP5;
		save->bios_3_scratch &= ~ATOM_S3_DFP5_ACTIVE;
	    }
	    break;
	}
	if (info->ChipFamily >= CHIP_FAMILY_R600) {
	    OUTREG(R600_BIOS_0_SCRATCH, save->bios_0_scratch);
	    OUTREG(R600_BIOS_3_SCRATCH, save->bios_3_scratch);
	} else {
	    OUTREG(RADEON_BIOS_0_SCRATCH, save->bios_0_scratch);
	    OUTREG(RADEON_BIOS_3_SCRATCH, save->bios_3_scratch);
	}
    } else {
	switch (radeon_output->active_device) {
	case ATOM_DEVICE_TV1_SUPPORT:
	    if (connected) {
		if (radeon_output->MonType == MT_STV)
		    save->bios_4_scratch |= RADEON_TV1_ATTACHED_SVIDEO;
		else if (radeon_output->MonType == MT_CTV)
		    save->bios_4_scratch |= RADEON_TV1_ATTACHED_COMP;
		save->bios_5_scratch |= RADEON_TV1_ON;
	    } else {
		save->bios_4_scratch &= ~RADEON_TV1_ATTACHED_MASK;
		save->bios_5_scratch &= ~RADEON_TV1_ON;
	    }
	    break;
	case ATOM_DEVICE_LCD1_SUPPORT:
	    if (connected) {
		save->bios_4_scratch |= RADEON_LCD1_ATTACHED;
		save->bios_5_scratch |= RADEON_LCD1_ON;
	    } else {
		save->bios_4_scratch &= ~RADEON_LCD1_ATTACHED;
		save->bios_5_scratch &= ~RADEON_LCD1_ON;
	    }
	    break;
	case ATOM_DEVICE_CRT1_SUPPORT:
	    if (connected) {
		save->bios_4_scratch |= RADEON_CRT1_ATTACHED_COLOR;
		save->bios_5_scratch |= RADEON_CRT1_ON;
	    } else {
		save->bios_4_scratch &= ~RADEON_CRT1_ATTACHED_MASK;
		save->bios_5_scratch &= ~RADEON_CRT1_ON;
	    }
	    break;
	case ATOM_DEVICE_CRT2_SUPPORT:
	    if (connected) {
		save->bios_4_scratch |= RADEON_CRT2_ATTACHED_COLOR;
		save->bios_5_scratch |= RADEON_CRT2_ON;
	    } else {
		save->bios_4_scratch &= ~RADEON_CRT2_ATTACHED_MASK;
		save->bios_5_scratch &= ~RADEON_CRT2_ON;
	    }
	    break;
	case ATOM_DEVICE_DFP1_SUPPORT:
	    if (connected) {
		save->bios_4_scratch |= RADEON_DFP1_ATTACHED;
		save->bios_5_scratch |= RADEON_DFP1_ON;
	    } else {
		save->bios_4_scratch &= ~RADEON_DFP1_ATTACHED;
		save->bios_5_scratch &= ~RADEON_DFP1_ON;
	    }
	    break;
	case ATOM_DEVICE_DFP2_SUPPORT:
	    if (connected) {
		save->bios_4_scratch |= RADEON_DFP2_ATTACHED;
		save->bios_5_scratch |= RADEON_DFP2_ON;
	    } else {
		save->bios_4_scratch &= ~RADEON_DFP2_ATTACHED;
		save->bios_5_scratch &= ~RADEON_DFP2_ON;
	    }
	    break;
	}
	OUTREG(RADEON_BIOS_4_SCRATCH, save->bios_4_scratch);
	OUTREG(RADEON_BIOS_5_SCRATCH, save->bios_5_scratch);
    }

}

static xf86OutputStatus
radeon_detect(xf86OutputPtr output)
{
    ScrnInfoPtr	    pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    Bool connected = TRUE;

    radeon_output->MonType = MT_UNKNOWN;
    radeon_bios_output_connected(output, FALSE);
    radeon_output->MonType = radeon_ddc_connected(output);
    if (!radeon_output->MonType) {
	if (radeon_output->devices & (ATOM_DEVICE_LCD_SUPPORT)) {
	    if (xf86ReturnOptValBool(info->Options, OPTION_IGNORE_LID_STATUS, TRUE))
		radeon_output->MonType = MT_LCD;
	    else
#if defined(__powerpc__)
		radeon_output->MonType = MT_LCD;
#else
	        radeon_output->MonType = RADEONDetectLidStatus(pScrn);
#endif
	} else {
	    if (info->IsAtomBios)
		radeon_output->MonType = atombios_dac_detect(output);
	    else
		radeon_output->MonType = legacy_dac_detect(output);
	}
    }

    // if size is zero panel probably broken or not connected
    if (radeon_output->devices & (ATOM_DEVICE_LCD_SUPPORT)) {
	radeon_encoder_ptr radeon_encoder = info->encoders[ATOM_DEVICE_LCD1_INDEX];
	if (radeon_encoder) {
	    radeon_lvds_ptr lvds = (radeon_lvds_ptr)radeon_encoder->dev_priv;
	    if (lvds) {
		if ((lvds->native_mode.PanelXRes == 0) || (lvds->native_mode.PanelYRes == 0))
		    radeon_output->MonType = MT_NONE;
	    }
	}
    }


    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Output: %s, Detected Monitor Type: %d\n", output->name, radeon_output->MonType);
    if (output->MonInfo) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "EDID data from the display on output: %s ----------------------\n",
		   output->name);
	xf86PrintEDID( output->MonInfo );
    }

    /* nothing connected, light up some defaults so the server comes up */
    if (radeon_output->MonType == MT_NONE &&
	info->first_load_no_devices) {
	if (info->IsMobility) {
	    if (radeon_output->devices & (ATOM_DEVICE_LCD_SUPPORT)) {
		radeon_output->MonType = MT_LCD;
		info->first_load_no_devices = FALSE;
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Using LCD default\n");
	    }
	} else {
	    if (radeon_output->devices & (ATOM_DEVICE_CRT_SUPPORT)) {
		radeon_output->MonType = MT_CRT;
		info->first_load_no_devices = FALSE;
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Using CRT default\n");
	    } else if (radeon_output->devices & (ATOM_DEVICE_DFP_SUPPORT)) {
		radeon_output->MonType = MT_DFP;
		info->first_load_no_devices = FALSE;
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Using DFP default\n");
	    }
	}
    }

    radeon_bios_output_connected(output, TRUE);

    /* set montype so users can force outputs on even if detection fails */
    if (radeon_output->MonType == MT_NONE) {
	connected = FALSE;
	switch (radeon_output->ConnectorType) {
	case CONNECTOR_LVDS:
	    radeon_output->MonType = MT_LCD;
	    break;
	case CONNECTOR_DVI_D:
	case CONNECTOR_HDMI_TYPE_A:
	case CONNECTOR_HDMI_TYPE_B:
	    radeon_output->MonType = MT_DFP;
	    break;
	case CONNECTOR_VGA:
	case CONNECTOR_DVI_A:
	default:
	    radeon_output->MonType = MT_CRT;
	    break;
	case CONNECTOR_DVI_I:
	    if (radeon_output->DVIType == DVI_ANALOG)
		radeon_output->MonType = MT_CRT;
	    else if (radeon_output->DVIType == DVI_DIGITAL)
		radeon_output->MonType = MT_DFP;
	    break;
	case CONNECTOR_STV:
            radeon_output->MonType = MT_STV;
	    break;
	case CONNECTOR_CTV:
            radeon_output->MonType = MT_CTV;
	    break;
	case CONNECTOR_DIN:
            radeon_output->MonType = MT_CV;
	    break;
	case CONNECTOR_DISPLAY_PORT:
	case CONNECTOR_EDP:
	    radeon_output->MonType = MT_DP;
	    break;
	}
    }

    radeon_set_active_device(output);

    if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT | ATOM_DEVICE_DFP_SUPPORT))
	output->subpixel_order = SubPixelHorizontalRGB;
    else
	output->subpixel_order = SubPixelNone;

    if (connected)
	return XF86OutputStatusConnected;
    else
	return XF86OutputStatusDisconnected;
}

static DisplayModePtr
radeon_get_modes(xf86OutputPtr output)
{
  DisplayModePtr modes;
  modes = RADEONProbeOutputModes(output);
  return modes;
}

static void
radeon_destroy (xf86OutputPtr output)
{
    if (output->driver_private)
        free(output->driver_private);
}

static void
radeon_set_backlight_level(xf86OutputPtr output, int level)
{
#if 0
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char * RADEONMMIO = info->MMIO;
    uint32_t lvds_gen_cntl;

    lvds_gen_cntl = INREG(RADEON_LVDS_GEN_CNTL);
    lvds_gen_cntl |= RADEON_LVDS_BL_MOD_EN;
    lvds_gen_cntl &= ~RADEON_LVDS_BL_MOD_LEVEL_MASK;
    lvds_gen_cntl |= (level << RADEON_LVDS_BL_MOD_LEVEL_SHIFT) & RADEON_LVDS_BL_MOD_LEVEL_MASK;
    //usleep (radeon_output->PanelPwrDly * 1000);
    OUTREG(RADEON_LVDS_GEN_CNTL, lvds_gen_cntl);
    lvds_gen_cntl &= ~RADEON_LVDS_BL_MOD_EN;
    //usleep (radeon_output->PanelPwrDly * 1000);
    OUTREG(RADEON_LVDS_GEN_CNTL, lvds_gen_cntl);
#endif
}

static Atom backlight_atom;
static Atom tmds_pll_atom;
static Atom rmx_atom;
static Atom monitor_type_atom;
static Atom load_detection_atom;
static Atom coherent_mode_atom;
static Atom tv_hsize_atom;
static Atom tv_hpos_atom;
static Atom tv_vpos_atom;
static Atom tv_std_atom;
#define RADEON_MAX_BACKLIGHT_LEVEL 255

static void
radeon_create_resources(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    INT32 range[2];
    int data, err;
    const char *s;

#if 0
    /* backlight control */
    if (radeon_output->type == OUTPUT_LVDS) {
	backlight_atom = MAKE_ATOM("backlight");

	range[0] = 0;
	range[1] = RADEON_MAX_BACKLIGHT_LEVEL;
	err = RRConfigureOutputProperty(output->randr_output, backlight_atom,
					FALSE, TRUE, FALSE, 2, range);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", err);
	}
	/* Set the current value of the backlight property */
	//data = (info->SavedReg->lvds_gen_cntl & RADEON_LVDS_BL_MOD_LEVEL_MASK) >> RADEON_LVDS_BL_MOD_LEVEL_SHIFT;
	data = RADEON_MAX_BACKLIGHT_LEVEL;
	err = RRChangeOutputProperty(output->randr_output, backlight_atom,
				     XA_INTEGER, 32, PropModeReplace, 1, &data,
				     FALSE, TRUE);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
	}
    }
#endif

    if (radeon_output->devices & (ATOM_DEVICE_CRT_SUPPORT | ATOM_DEVICE_TV_SUPPORT | ATOM_DEVICE_CV_SUPPORT)) {
	load_detection_atom = MAKE_ATOM("load_detection");

	range[0] = 0; /* off */
	range[1] = 1; /* on */
	err = RRConfigureOutputProperty(output->randr_output, load_detection_atom,
					FALSE, TRUE, FALSE, 2, range);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", err);
	}

	if (radeon_output->load_detection)
	    data = 1;
	else
	    data = 0;

	err = RRChangeOutputProperty(output->randr_output, load_detection_atom,
				     XA_INTEGER, 32, PropModeReplace, 1, &data,
				     FALSE, TRUE);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
	}
    }

    if (IS_AVIVO_VARIANT && (radeon_output->devices & (ATOM_DEVICE_DFP_SUPPORT))) {
	coherent_mode_atom = MAKE_ATOM("coherent_mode");

	range[0] = 0; /* off */
	range[1] = 1; /* on */
	err = RRConfigureOutputProperty(output->randr_output, coherent_mode_atom,
					FALSE, TRUE, FALSE, 2, range);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", err);
	}

	data = 1; /* coherent mode on by default */

	err = RRChangeOutputProperty(output->randr_output, coherent_mode_atom,
				     XA_INTEGER, 32, PropModeReplace, 1, &data,
				     FALSE, TRUE);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
	}
    }

    if ((!IS_AVIVO_VARIANT) && (radeon_output->devices & (ATOM_DEVICE_DFP1_SUPPORT))) {
	tmds_pll_atom = MAKE_ATOM("tmds_pll");

	err = RRConfigureOutputProperty(output->randr_output, tmds_pll_atom,
					FALSE, FALSE, FALSE, 0, NULL);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", err);
	}
	/* Set the current value of the property */
#if defined(__powerpc__)
	s = "driver";
#else
	s = "bios";
#endif
	if (xf86ReturnOptValBool(info->Options, OPTION_DEFAULT_TMDS_PLL, FALSE)) {
	    s = "driver";
	}

	err = RRChangeOutputProperty(output->randr_output, tmds_pll_atom,
				     XA_STRING, 8, PropModeReplace, strlen(s), (pointer)s,
				     FALSE, FALSE);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
	}

    }

    /* RMX control - fullscreen, centered, keep ratio, off */
    /* actually more of a crtc property as only crtc1 has rmx */
    if (radeon_output->devices & (ATOM_DEVICE_DFP_SUPPORT | ATOM_DEVICE_LCD_SUPPORT)) {
	rmx_atom = MAKE_ATOM("scaler");

	err = RRConfigureOutputProperty(output->randr_output, rmx_atom,
					FALSE, FALSE, FALSE, 0, NULL);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", err);
	}
	/* Set the current value of the property */
	switch (radeon_output->rmx_type) {
	case RMX_OFF:
	default:
	    s = "off";
	    break;
	case RMX_FULL:
	    s = "full";
	    break;
	case RMX_CENTER:
	    s = "center";
	    break;
	case RMX_ASPECT:
	    s = "aspect";
	    break;
	}
	err = RRChangeOutputProperty(output->randr_output, rmx_atom,
				     XA_STRING, 8, PropModeReplace, strlen(s), (pointer)s,
				     FALSE, FALSE);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
	}
    }

    /* force auto/analog/digital for DVI-I ports */
    if ((radeon_output->devices & (ATOM_DEVICE_CRT_SUPPORT)) &&
	(radeon_output->devices & (ATOM_DEVICE_DFP_SUPPORT))){
	monitor_type_atom = MAKE_ATOM("dvi_monitor_type");

	err = RRConfigureOutputProperty(output->randr_output, monitor_type_atom,
					FALSE, FALSE, FALSE, 0, NULL);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", err);
	}
	/* Set the current value of the backlight property */
	s = "auto";
	err = RRChangeOutputProperty(output->randr_output, monitor_type_atom,
				     XA_STRING, 8, PropModeReplace, strlen(s), (pointer)s,
				     FALSE, FALSE);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
	}
    }

    if (radeon_output->devices & (ATOM_DEVICE_TV_SUPPORT)) {
	radeon_tvout_ptr tvout = &radeon_output->tvout;
	if (!IS_AVIVO_VARIANT) {
	    tv_hsize_atom = MAKE_ATOM("tv_horizontal_size");

	    range[0] = -MAX_H_SIZE;
	    range[1] = MAX_H_SIZE;
	    err = RRConfigureOutputProperty(output->randr_output, tv_hsize_atom,
					    FALSE, TRUE, FALSE, 2, range);
	    if (err != 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "RRConfigureOutputProperty error, %d\n", err);
	    }
	    data = 0;
	    err = RRChangeOutputProperty(output->randr_output, tv_hsize_atom,
					 XA_INTEGER, 32, PropModeReplace, 1, &data,
					 FALSE, TRUE);
	    if (err != 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "RRChangeOutputProperty error, %d\n", err);
	    }

	    tv_hpos_atom = MAKE_ATOM("tv_horizontal_position");

	    range[0] = -MAX_H_POSITION;
	    range[1] = MAX_H_POSITION;
	    err = RRConfigureOutputProperty(output->randr_output, tv_hpos_atom,
					    FALSE, TRUE, FALSE, 2, range);
	    if (err != 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "RRConfigureOutputProperty error, %d\n", err);
	    }
	    data = 0;
	    err = RRChangeOutputProperty(output->randr_output, tv_hpos_atom,
					 XA_INTEGER, 32, PropModeReplace, 1, &data,
					 FALSE, TRUE);
	    if (err != 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "RRChangeOutputProperty error, %d\n", err);
	    }

	    tv_vpos_atom = MAKE_ATOM("tv_vertical_position");

	    range[0] = -MAX_V_POSITION;
	    range[1] = MAX_V_POSITION;
	    err = RRConfigureOutputProperty(output->randr_output, tv_vpos_atom,
					    FALSE, TRUE, FALSE, 2, range);
	    if (err != 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "RRConfigureOutputProperty error, %d\n", err);
	    }
	    data = 0;
	    err = RRChangeOutputProperty(output->randr_output, tv_vpos_atom,
					 XA_INTEGER, 32, PropModeReplace, 1, &data,
					 FALSE, TRUE);
	    if (err != 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "RRChangeOutputProperty error, %d\n", err);
	    }
	}

	tv_std_atom = MAKE_ATOM("tv_standard");

	err = RRConfigureOutputProperty(output->randr_output, tv_std_atom,
					FALSE, FALSE, FALSE, 0, NULL);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", err);
	}

	/* Set the current value of the property */
	switch (tvout->tvStd) {
	case TV_STD_PAL:
	    s = "pal";
	    break;
	case TV_STD_PAL_M:
	    s = "pal-m";
	    break;
	case TV_STD_PAL_60:
	    s = "pal-60";
	    break;
	case TV_STD_NTSC_J:
	    s = "ntsc-j";
	    break;
	case TV_STD_SCART_PAL:
	    s = "scart-pal";
	    break;
	case TV_STD_NTSC:
	default:
	    s = "ntsc";
	    break;
	}

	err = RRChangeOutputProperty(output->randr_output, tv_std_atom,
				     XA_STRING, 8, PropModeReplace, strlen(s), (pointer)s,
				     FALSE, FALSE);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
	}
    }
}

static Bool
radeon_set_mode_for_property(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;

    if (output->crtc) {
	xf86CrtcPtr crtc = output->crtc;

	if (crtc->enabled) {
#ifdef RANDR_14_INTERFACE
	    xf86CrtcSetRec crtc_set_rec;

	    crtc_set_rec.flags = (XF86CrtcSetMode |
				  XF86CrtcSetOutput |
				  XF86CrtcSetOrigin |
				  XF86CrtcSetRotation);
	    crtc_set_rec.mode = &crtc->desiredMode;
	    crtc_set_rec.rotation = crtc->desiredRotation;
	    crtc_set_rec.transform = NULL;
	    crtc_set_rec.x = crtc->desiredX;
	    crtc_set_rec.y = crtc->desiredY;
	    if (!xf86CrtcSet(crtc, &crtc_set_rec)) {
#else
	    if (!xf86CrtcSetMode(crtc, &crtc->desiredMode, crtc->desiredRotation,
				 crtc->desiredX, crtc->desiredY)) {
#endif
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Failed to set mode after property change!\n");
		return FALSE;
	    }
	}
    }
    return TRUE;
}

static Bool
radeon_set_property(xf86OutputPtr output, Atom property,
		       RRPropertyValuePtr value)
{
    RADEONInfoPtr info = RADEONPTR(output->scrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    INT32 val;


    if (property == backlight_atom) {
	if (value->type != XA_INTEGER ||
	    value->format != 32 ||
	    value->size != 1) {
	    return FALSE;
	}

	val = *(INT32 *)value->data;
	if (val < 0 || val > RADEON_MAX_BACKLIGHT_LEVEL)
	    return FALSE;

#if defined(__powerpc__)
	val = RADEON_MAX_BACKLIGHT_LEVEL - val;
#endif

	radeon_set_backlight_level(output, val);

    } else if (property == load_detection_atom) {
	if (value->type != XA_INTEGER ||
	    value->format != 32 ||
	    value->size != 1) {
	    return FALSE;
	}

	val = *(INT32 *)value->data;
	if (val < 0 || val > 1)
	    return FALSE;

	radeon_output->load_detection = val;

    } else if (property == coherent_mode_atom) {
	Bool coherent_mode = radeon_output->coherent_mode;

	if (value->type != XA_INTEGER ||
	    value->format != 32 ||
	    value->size != 1) {
	    return FALSE;
	}

	val = *(INT32 *)value->data;
	if (val < 0 || val > 1)
	    return FALSE;

	radeon_output->coherent_mode = val;
	if (!radeon_set_mode_for_property(output)) {
	    radeon_output->coherent_mode = coherent_mode;
	    (void)radeon_set_mode_for_property(output);
	    return FALSE;
	}

    } else if (property == rmx_atom) {
	const char *s;
	RADEONRMXType rmx = radeon_output->rmx_type;

	if (value->type != XA_STRING || value->format != 8)
	    return FALSE;
	s = (char*)value->data;
	if (value->size == strlen("full") && !strncmp("full", s, strlen("full"))) {
	    radeon_output->rmx_type = RMX_FULL;
	} else if (value->size == strlen("center") && !strncmp("center", s, strlen("center"))) {
	    radeon_output->rmx_type = RMX_CENTER;
	} else if (value->size == strlen("aspect") && !strncmp("aspect", s, strlen("aspect"))) {
	    if (IS_AVIVO_VARIANT)
		radeon_output->rmx_type = RMX_ASPECT;
	    else
		return FALSE;
	} else if (value->size == strlen("off") && !strncmp("off", s, strlen("off"))) {
	    radeon_output->rmx_type = RMX_OFF;
	} else
	    return FALSE;

	if (!radeon_set_mode_for_property(output)) {
	    radeon_output->rmx_type = rmx;
	    (void)radeon_set_mode_for_property(output);
	    return FALSE;
	}
    } else if (property == tmds_pll_atom) {
	radeon_tmds_ptr tmds = NULL;
	const char *s;

	if (info->encoders[ATOM_DEVICE_DFP1_INDEX] && info->encoders[ATOM_DEVICE_DFP1_INDEX]->dev_priv)
	    tmds = (radeon_tmds_ptr)info->encoders[ATOM_DEVICE_DFP1_INDEX]->dev_priv;
	else
	    return FALSE;

	if (value->type != XA_STRING || value->format != 8)
	    return FALSE;
	s = (char*)value->data;
	if (value->size == strlen("bios") && !strncmp("bios", s, strlen("bios"))) {
	    if (!RADEONGetTMDSInfoFromBIOS(output->scrn, tmds))
		RADEONGetTMDSInfoFromTable(output->scrn, tmds);
	} else if (value->size == strlen("driver") && !strncmp("driver", s, strlen("driver")))
	    RADEONGetTMDSInfoFromTable(output->scrn, tmds);
	else
	    return FALSE;

	return radeon_set_mode_for_property(output);
    } else if (property == monitor_type_atom) {
	const char *s;
	if (value->type != XA_STRING || value->format != 8)
	    return FALSE;
	s = (char*)value->data;
	if (value->size == strlen("auto") && !strncmp("auto", s, strlen("auto"))) {
	    radeon_output->DVIType = DVI_AUTO;
	    return TRUE;
	} else if (value->size == strlen("analog") && !strncmp("analog", s, strlen("analog"))) {
	    radeon_output->DVIType = DVI_ANALOG;
	    return TRUE;
	} else if (value->size == strlen("digital") && !strncmp("digital", s, strlen("digital"))) {
	    radeon_output->DVIType = DVI_DIGITAL;
	    return TRUE;
	} else
	    return FALSE;
    } else if (property == tv_hsize_atom) {
	radeon_tvout_ptr tvout = &radeon_output->tvout;
	if (value->type != XA_INTEGER ||
	    value->format != 32 ||
	    value->size != 1) {
	    return FALSE;
	}

	val = *(INT32 *)value->data;
	if (val < -MAX_H_SIZE || val > MAX_H_SIZE)
	    return FALSE;

	tvout->hSize = val;
	if (tvout->tv_on && !IS_AVIVO_VARIANT)
	    RADEONUpdateHVPosition(output, &output->crtc->mode);

    } else if (property == tv_hpos_atom) {
	radeon_tvout_ptr tvout = &radeon_output->tvout;
	if (value->type != XA_INTEGER ||
	    value->format != 32 ||
	    value->size != 1) {
	    return FALSE;
	}

	val = *(INT32 *)value->data;
	if (val < -MAX_H_POSITION || val > MAX_H_POSITION)
	    return FALSE;

	tvout->hPos = val;
	if (tvout->tv_on && !IS_AVIVO_VARIANT)
	    RADEONUpdateHVPosition(output, &output->crtc->mode);

    } else if (property == tv_vpos_atom) {
	radeon_tvout_ptr tvout = &radeon_output->tvout;
	if (value->type != XA_INTEGER ||
	    value->format != 32 ||
	    value->size != 1) {
	    return FALSE;
	}

	val = *(INT32 *)value->data;
	if (val < -MAX_H_POSITION || val > MAX_H_POSITION)
	    return FALSE;

	tvout->vPos = val;
	if (tvout->tv_on && !IS_AVIVO_VARIANT)
	    RADEONUpdateHVPosition(output, &output->crtc->mode);

    } else if (property == tv_std_atom) {
	const char *s;
	radeon_tvout_ptr tvout = &radeon_output->tvout;
	TVStd std = tvout->tvStd;

	if (value->type != XA_STRING || value->format != 8)
	    return FALSE;
	s = (char*)value->data;
	if (value->size == strlen("ntsc") && !strncmp("ntsc", s, strlen("ntsc"))) {
	    tvout->tvStd = TV_STD_NTSC;
	} else if (value->size == strlen("pal") && !strncmp("pal", s, strlen("pal"))) {
	    tvout->tvStd = TV_STD_PAL;
	} else if (value->size == strlen("pal-m") && !strncmp("pal-m", s, strlen("pal-m"))) {
	    tvout->tvStd = TV_STD_PAL_M;
	} else if (value->size == strlen("pal-60") && !strncmp("pal-60", s, strlen("pal-60"))) {
	    tvout->tvStd = TV_STD_PAL_60;
	} else if (value->size == strlen("ntsc-j") && !strncmp("ntsc-j", s, strlen("ntsc-j"))) {
	    tvout->tvStd = TV_STD_NTSC_J;
	} else if (value->size == strlen("scart-pal") && !strncmp("scart-pal", s, strlen("scart-pal"))) {
	    tvout->tvStd = TV_STD_SCART_PAL;
	} else if (value->size == strlen("pal-cn") && !strncmp("pal-cn", s, strlen("pal-cn"))) {
	    tvout->tvStd = TV_STD_PAL_CN;
	} else if (value->size == strlen("secam") && !strncmp("secam", s, strlen("secam"))) {
	    tvout->tvStd = TV_STD_SECAM;
	} else
	    return FALSE;

	if (!radeon_set_mode_for_property(output)) {
	    tvout->tvStd = std;
	    (void)radeon_set_mode_for_property(output);
	    return FALSE;
	}
    }

    return TRUE;
}

static const xf86OutputFuncsRec radeon_output_funcs = {
    .create_resources = radeon_create_resources,
    .dpms = radeon_dpms,
    .save = radeon_save,
    .restore = radeon_restore,
    .mode_valid = radeon_mode_valid,
    .mode_fixup = radeon_mode_fixup,
    .prepare = radeon_mode_prepare,
    .mode_set = radeon_mode_set,
    .commit = radeon_mode_commit,
    .detect = radeon_detect,
    .get_modes = radeon_get_modes,
    .set_property = radeon_set_property,
    .destroy = radeon_destroy
};

Bool
RADEONI2CDoLock(xf86OutputPtr output, I2CBusPtr b, int lock_state)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONI2CBusPtr pRADEONI2CBus = b->DriverPrivate.ptr;
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t temp;

    if (lock_state) {
	/* RV410 appears to have a bug where the hw i2c in reset
	 * holds the i2c port in a bad state - switch hw i2c away before
	 * doing DDC - do this for all r200s/r300s for safety sakes */
	if ((info->ChipFamily >= CHIP_FAMILY_R200) && (!IS_AVIVO_VARIANT)) {
	    if (pRADEONI2CBus->mask_clk_reg == RADEON_GPIO_MONID)
                OUTREG(RADEON_DVI_I2C_CNTL_0, (RADEON_I2C_SOFT_RST |
					       R200_DVI_I2C_PIN_SEL(R200_SEL_DDC1)));
	    else
                OUTREG(RADEON_DVI_I2C_CNTL_0, (RADEON_I2C_SOFT_RST |
					       R200_DVI_I2C_PIN_SEL(R200_SEL_DDC3)));
	}

	/* set the pad in ddc mode */
	if (IS_DCE3_VARIANT &&
	    pRADEONI2CBus->hw_capable) {
	    temp = INREG(pRADEONI2CBus->mask_clk_reg);
	    temp &= ~(1 << 16);
	    OUTREG(pRADEONI2CBus->mask_clk_reg, temp);
	}

	temp = INREG(pRADEONI2CBus->a_clk_reg);
	temp &= ~(pRADEONI2CBus->a_clk_mask);
	OUTREG(pRADEONI2CBus->a_clk_reg, temp);

	temp = INREG(pRADEONI2CBus->a_data_reg);
	temp &= ~(pRADEONI2CBus->a_data_mask);
	OUTREG(pRADEONI2CBus->a_data_reg, temp);
    }

    temp = INREG(pRADEONI2CBus->mask_clk_reg);
    if (lock_state)
	temp |= (pRADEONI2CBus->mask_clk_mask);
    else
	temp &= ~(pRADEONI2CBus->mask_clk_mask);
    OUTREG(pRADEONI2CBus->mask_clk_reg, temp);
    temp = INREG(pRADEONI2CBus->mask_clk_reg);

    temp = INREG(pRADEONI2CBus->mask_data_reg);
    if (lock_state)
	temp |= (pRADEONI2CBus->mask_data_mask);
    else
	temp &= ~(pRADEONI2CBus->mask_data_mask);
    OUTREG(pRADEONI2CBus->mask_data_reg, temp);
    temp = INREG(pRADEONI2CBus->mask_data_reg);

    return TRUE;
}

static void RADEONI2CGetBits(I2CBusPtr b, int *Clock, int *data)
{
    ScrnInfoPtr    pScrn      = xf86Screens[b->scrnIndex];
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned long  val;
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONI2CBusPtr pRADEONI2CBus = b->DriverPrivate.ptr;

    /* Get the result */
    val = INREG(pRADEONI2CBus->get_clk_reg);
    *Clock = (val & pRADEONI2CBus->get_clk_mask) != 0;
    val = INREG(pRADEONI2CBus->get_data_reg);
    *data  = (val & pRADEONI2CBus->get_data_mask) != 0;

}

static void RADEONI2CPutBits(I2CBusPtr b, int Clock, int data)
{
    ScrnInfoPtr    pScrn      = xf86Screens[b->scrnIndex];
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned long  val;
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONI2CBusPtr pRADEONI2CBus = b->DriverPrivate.ptr;

    val = INREG(pRADEONI2CBus->put_clk_reg) & (uint32_t)~(pRADEONI2CBus->put_clk_mask);
    val |= (Clock ? 0:pRADEONI2CBus->put_clk_mask);
    OUTREG(pRADEONI2CBus->put_clk_reg, val);
    /* read back to improve reliability on some cards. */
    val = INREG(pRADEONI2CBus->put_clk_reg);

    val = INREG(pRADEONI2CBus->put_data_reg) & (uint32_t)~(pRADEONI2CBus->put_data_mask);
    val |= (data ? 0:pRADEONI2CBus->put_data_mask);
    OUTREG(pRADEONI2CBus->put_data_reg, val);
    /* read back to improve reliability on some cards. */
    val = INREG(pRADEONI2CBus->put_data_reg);

}

Bool
RADEONI2CInit(ScrnInfoPtr pScrn, I2CBusPtr *bus_ptr, char *name, RADEONI2CBusPtr pRADEONI2CBus)
{
    I2CBusPtr pI2CBus;

    pI2CBus = xf86CreateI2CBusRec();
    if (!pI2CBus) return FALSE;

    pI2CBus->BusName    = name;
    pI2CBus->scrnIndex  = pScrn->scrnIndex;
    pI2CBus->I2CPutBits = RADEONI2CPutBits;
    pI2CBus->I2CGetBits = RADEONI2CGetBits;
    pI2CBus->AcknTimeout = 5;

    pI2CBus->DriverPrivate.ptr = (pointer)pRADEONI2CBus;

    if (!xf86I2CBusInit(pI2CBus))
	return FALSE;

    *bus_ptr = pI2CBus;
    return TRUE;
}

RADEONI2CBusRec
legacy_setup_i2c_bus(int ddc_line)
{
    RADEONI2CBusRec i2c;

    i2c.hw_line = 0;
    i2c.hw_capable = FALSE;
    i2c.mask_clk_mask = RADEON_GPIO_EN_1;
    i2c.mask_data_mask = RADEON_GPIO_EN_0;
    i2c.a_clk_mask = RADEON_GPIO_A_1;
    i2c.a_data_mask = RADEON_GPIO_A_0;
    i2c.put_clk_mask = RADEON_GPIO_EN_1;
    i2c.put_data_mask = RADEON_GPIO_EN_0;
    i2c.get_clk_mask = RADEON_GPIO_Y_1;
    i2c.get_data_mask = RADEON_GPIO_Y_0;
    if ((ddc_line == RADEON_LCD_GPIO_MASK) ||
	(ddc_line == RADEON_MDGPIO_EN_REG)) {
	i2c.mask_clk_reg = ddc_line;
	i2c.mask_data_reg = ddc_line;
	i2c.a_clk_reg = ddc_line;
	i2c.a_data_reg = ddc_line;
	i2c.put_clk_reg = ddc_line;
	i2c.put_data_reg = ddc_line;
	i2c.get_clk_reg = ddc_line + 4;
	i2c.get_data_reg = ddc_line + 4;
    } else {
	i2c.mask_clk_reg = ddc_line;
	i2c.mask_data_reg = ddc_line;
	i2c.a_clk_reg = ddc_line;
	i2c.a_data_reg = ddc_line;
	i2c.put_clk_reg = ddc_line;
	i2c.put_data_reg = ddc_line;
	i2c.get_clk_reg = ddc_line;
	i2c.get_data_reg = ddc_line;
    }

    if (ddc_line)
	i2c.valid = TRUE;
    else
	i2c.valid = FALSE;

    return i2c;
}

RADEONI2CBusRec
atom_setup_i2c_bus(int ddc_line)
{
    RADEONI2CBusRec i2c;

    i2c.hw_line = 0;
    i2c.hw_capable = FALSE;
    if (ddc_line == AVIVO_GPIO_0) {
	i2c.put_clk_mask = (1 << 19);
	i2c.put_data_mask = (1 << 18);
	i2c.get_clk_mask = (1 << 19);
	i2c.get_data_mask = (1 << 18);
	i2c.mask_clk_mask = (1 << 19);
	i2c.mask_data_mask = (1 << 18);
	i2c.a_clk_mask = (1 << 19);
	i2c.a_data_mask = (1 << 18);
    } else {
	i2c.put_clk_mask = (1 << 0);
	i2c.put_data_mask = (1 << 8);
	i2c.get_clk_mask = (1 << 0);
	i2c.get_data_mask = (1 << 8);
	i2c.mask_clk_mask = (1 << 0);
	i2c.mask_data_mask = (1 << 8);
	i2c.a_clk_mask = (1 << 0);
	i2c.a_data_mask = (1 << 8);
    }
    i2c.mask_clk_reg = ddc_line;
    i2c.mask_data_reg = ddc_line;
    i2c.a_clk_reg = ddc_line + 0x4;
    i2c.a_data_reg = ddc_line + 0x4;
    i2c.put_clk_reg = ddc_line + 0x8;
    i2c.put_data_reg = ddc_line + 0x8;
    i2c.get_clk_reg = ddc_line + 0xc;
    i2c.get_data_reg = ddc_line + 0xc;
    if (ddc_line)
	i2c.valid = TRUE;
    else
	i2c.valid = FALSE;

    return i2c;
}

static void
RADEONGetTVInfo(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_tvout_ptr tvout = &radeon_output->tvout;
    char *optstr;

    tvout->hPos = 0;
    tvout->vPos = 0;
    tvout->hSize = 0;
    tvout->tv_on = FALSE;

    if (!RADEONGetTVInfoFromBIOS(output)) {
	/* set some reasonable defaults */
	tvout->default_tvStd = TV_STD_NTSC;
	tvout->tvStd = TV_STD_NTSC;
	tvout->TVRefClk = 27.000000000;
	tvout->SupportedTVStds = TV_STD_NTSC | TV_STD_PAL;
    }

    optstr = (char *)xf86GetOptValString(info->Options, OPTION_TVSTD);
    if (optstr) {
	if (!strncmp("ntsc", optstr, strlen("ntsc")))
	    tvout->tvStd = TV_STD_NTSC;
	else if (!strncmp("pal", optstr, strlen("pal")))
	    tvout->tvStd = TV_STD_PAL;
	else if (!strncmp("pal-m", optstr, strlen("pal-m")))
	    tvout->tvStd = TV_STD_PAL_M;
	else if (!strncmp("pal-60", optstr, strlen("pal-60")))
	    tvout->tvStd = TV_STD_PAL_60;
	else if (!strncmp("ntsc-j", optstr, strlen("ntsc-j")))
	    tvout->tvStd = TV_STD_NTSC_J;
	else if (!strncmp("scart-pal", optstr, strlen("scart-pal")))
	    tvout->tvStd = TV_STD_SCART_PAL;
	else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Invalid TV Standard: %s\n", optstr);
	}
    }

}

void RADEONInitConnector(xf86OutputPtr output)
{
    ScrnInfoPtr	    pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if (radeon_output->devices & (ATOM_DEVICE_LCD_SUPPORT))
	radeon_output->rmx_type = RMX_FULL;
    else
	radeon_output->rmx_type = RMX_OFF;

    if (!IS_AVIVO_VARIANT) {
	if (radeon_output->devices & (ATOM_DEVICE_CRT2_SUPPORT)) {
	    if (xf86ReturnOptValBool(info->Options, OPTION_TVDAC_LOAD_DETECT, FALSE))
		radeon_output->load_detection = 1;
	}
    }

    if (radeon_output->devices & (ATOM_DEVICE_TV_SUPPORT))
	RADEONGetTVInfo(output);

    if (radeon_output->devices & (ATOM_DEVICE_DFP_SUPPORT))
	radeon_output->coherent_mode = TRUE;

    if (radeon_output->ConnectorType == CONNECTOR_DISPLAY_PORT) {
	strcpy(radeon_output->dp_bus_name, output->name);
	strcat(radeon_output->dp_bus_name, "-DP");
	RADEON_DP_I2CInit(pScrn, &radeon_output->dp_pI2CBus, radeon_output->dp_bus_name, output);
	RADEON_DP_GetSinkType(output);
    }

    if (radeon_output->ConnectorType == CONNECTOR_EDP) {
	strcpy(radeon_output->dp_bus_name, output->name);
	strcat(radeon_output->dp_bus_name, "-eDP");
	RADEON_DP_I2CInit(pScrn, &radeon_output->dp_pI2CBus, radeon_output->dp_bus_name, output);
	RADEON_DP_GetSinkType(output);
    }

    if (radeon_output->ddc_i2c.valid)
	RADEONI2CInit(pScrn, &radeon_output->pI2CBus, output->name, &radeon_output->ddc_i2c);

}

#if defined(__powerpc__)
static Bool RADEONSetupAppleConnectors(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);


    switch (info->MacModel) {
    case RADEON_MAC_IBOOK:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
	info->BiosConnector[0].valid = TRUE;
	info->BiosConnector[0].devices = ATOM_DEVICE_LCD1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_LCD1_SUPPORT,
									    0),
				ATOM_DEVICE_LCD1_SUPPORT))
	    return FALSE;

	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[1].load_detection = FALSE;
	info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
	info->BiosConnector[1].valid = TRUE;
	info->BiosConnector[1].devices = ATOM_DEVICE_CRT2_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_CRT2_SUPPORT,
									    2),
				ATOM_DEVICE_CRT2_SUPPORT))
	    return FALSE;

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].load_detection = FALSE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
	info->BiosConnector[2].devices = ATOM_DEVICE_TV1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_TV1_SUPPORT,
									    2),
				ATOM_DEVICE_TV1_SUPPORT))
	    return FALSE;
	return TRUE;
    case RADEON_MAC_POWERBOOK_EXTERNAL:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
	info->BiosConnector[0].valid = TRUE;
	info->BiosConnector[0].devices = ATOM_DEVICE_LCD1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_LCD1_SUPPORT,
									    0),
				ATOM_DEVICE_LCD1_SUPPORT))
	    return FALSE;

	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[1].ConnectorType = CONNECTOR_DVI_I;
	info->BiosConnector[1].valid = TRUE;
	info->BiosConnector[1].devices = ATOM_DEVICE_CRT1_SUPPORT | ATOM_DEVICE_DFP2_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_CRT1_SUPPORT,
									    1),
				ATOM_DEVICE_CRT1_SUPPORT))
	    return FALSE;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_DFP2_SUPPORT,
									    0),
				ATOM_DEVICE_DFP2_SUPPORT))
	    return FALSE;

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].load_detection = FALSE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
	info->BiosConnector[2].devices = ATOM_DEVICE_TV1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_TV1_SUPPORT,
									    2),
				ATOM_DEVICE_TV1_SUPPORT))
	    return FALSE;
	return TRUE;
    case RADEON_MAC_POWERBOOK_INTERNAL:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
	info->BiosConnector[0].valid = TRUE;
	info->BiosConnector[0].devices = ATOM_DEVICE_LCD1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_LCD1_SUPPORT,
									    0),
				ATOM_DEVICE_LCD1_SUPPORT))
	    return FALSE;

	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[1].ConnectorType = CONNECTOR_DVI_I;
	info->BiosConnector[1].valid = TRUE;
	info->BiosConnector[1].devices = ATOM_DEVICE_CRT1_SUPPORT | ATOM_DEVICE_DFP1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_CRT1_SUPPORT,
									    1),
				ATOM_DEVICE_CRT1_SUPPORT))
	    return FALSE;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_DFP1_SUPPORT,
									    0),
				ATOM_DEVICE_DFP1_SUPPORT))
	    return FALSE;

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].load_detection = FALSE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
	info->BiosConnector[2].devices = ATOM_DEVICE_TV1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_TV1_SUPPORT,
									    2),
				ATOM_DEVICE_TV1_SUPPORT))
	    return FALSE;
	return TRUE;
    case RADEON_MAC_POWERBOOK_VGA:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
	info->BiosConnector[0].valid = TRUE;
	info->BiosConnector[0].devices = ATOM_DEVICE_LCD1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_LCD1_SUPPORT,
									    0),
				ATOM_DEVICE_LCD1_SUPPORT))
	    return FALSE;

	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
	info->BiosConnector[1].valid = TRUE;
	info->BiosConnector[1].devices = ATOM_DEVICE_CRT1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_CRT1_SUPPORT,
									    1),
				ATOM_DEVICE_CRT1_SUPPORT))
	    return FALSE;

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].load_detection = FALSE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
	info->BiosConnector[2].devices = ATOM_DEVICE_TV1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_TV1_SUPPORT,
									    2),
				ATOM_DEVICE_TV1_SUPPORT))
	    return FALSE;
	return TRUE;
    case RADEON_MAC_MINI_EXTERNAL:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_CRT2_DDC);
	info->BiosConnector[0].load_detection = FALSE;
	info->BiosConnector[0].ConnectorType = CONNECTOR_DVI_I;
	info->BiosConnector[0].valid = TRUE;
	info->BiosConnector[0].devices = ATOM_DEVICE_CRT2_SUPPORT | ATOM_DEVICE_DFP2_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_CRT2_SUPPORT,
									    2),
				ATOM_DEVICE_CRT2_SUPPORT))
	    return FALSE;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_DFP2_SUPPORT,
									    0),
				ATOM_DEVICE_DFP2_SUPPORT))
	    return FALSE;

	info->BiosConnector[1].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[1].load_detection = FALSE;
	info->BiosConnector[1].ddc_i2c.valid = FALSE;
	info->BiosConnector[1].valid = TRUE;
	info->BiosConnector[1].devices = ATOM_DEVICE_TV1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_TV1_SUPPORT,
									    2),
				ATOM_DEVICE_TV1_SUPPORT))
	    return FALSE;
	return TRUE;
    case RADEON_MAC_MINI_INTERNAL:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_CRT2_DDC);
	info->BiosConnector[0].load_detection = FALSE;
	info->BiosConnector[0].ConnectorType = CONNECTOR_DVI_I;
	info->BiosConnector[0].valid = TRUE;
	info->BiosConnector[0].devices = ATOM_DEVICE_CRT2_SUPPORT | ATOM_DEVICE_DFP1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_CRT2_SUPPORT,
									    2),
				ATOM_DEVICE_CRT2_SUPPORT))
	    return FALSE;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_DFP1_SUPPORT,
									    0),
				ATOM_DEVICE_DFP1_SUPPORT))
	    return FALSE;

	info->BiosConnector[1].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[1].load_detection = FALSE;
	info->BiosConnector[1].ddc_i2c.valid = FALSE;
	info->BiosConnector[1].valid = TRUE;
	info->BiosConnector[1].devices = ATOM_DEVICE_TV1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_TV1_SUPPORT,
									    2),
				ATOM_DEVICE_TV1_SUPPORT))
	    return FALSE;
	return TRUE;
    case RADEON_MAC_IMAC_G5_ISIGHT:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_MONID);
	info->BiosConnector[0].ConnectorType = CONNECTOR_DVI_D;
	info->BiosConnector[0].valid = TRUE;
	info->BiosConnector[0].devices = ATOM_DEVICE_DFP1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_DFP1_SUPPORT,
									    0),
				ATOM_DEVICE_DFP1_SUPPORT))
	    return FALSE;

	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	info->BiosConnector[1].load_detection = FALSE;
	info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
	info->BiosConnector[1].valid = TRUE;
	info->BiosConnector[1].devices = ATOM_DEVICE_CRT2_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_CRT2_SUPPORT,
									    2),
				ATOM_DEVICE_CRT2_SUPPORT))
	    return FALSE;

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].load_detection = FALSE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
	info->BiosConnector[2].devices = ATOM_DEVICE_TV1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_TV1_SUPPORT,
									    2),
				ATOM_DEVICE_TV1_SUPPORT))
	    return FALSE;
	return TRUE;
    case RADEON_MAC_EMAC:
	/* eMac G4 800/1.0 with radeon 7500, no EDID on internal monitor
	 * later eMac's (G4 1.25/1.42) with radeon 9200 and 9600 may have
	 * different ddc setups.  need to verify
	 */
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[0].ConnectorType = CONNECTOR_VGA;
	info->BiosConnector[0].valid = TRUE;
	info->BiosConnector[0].devices = ATOM_DEVICE_CRT1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_CRT1_SUPPORT,
									    1),
				ATOM_DEVICE_CRT1_SUPPORT))
	    return FALSE;

	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_CRT2_DDC);
	info->BiosConnector[1].load_detection = FALSE;
	info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
	info->BiosConnector[1].valid = TRUE;
	info->BiosConnector[1].devices = ATOM_DEVICE_CRT2_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_CRT2_SUPPORT,
									    2),
				ATOM_DEVICE_CRT2_SUPPORT))
	    return FALSE;

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].load_detection = FALSE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
	info->BiosConnector[2].devices = ATOM_DEVICE_TV1_SUPPORT;
	if (!radeon_add_encoder(pScrn,
				radeon_get_encoder_id_from_supported_device(pScrn,
									    ATOM_DEVICE_TV1_SUPPORT,
									    2),
				ATOM_DEVICE_TV1_SUPPORT))
	    return FALSE;
	return TRUE;
    default:
	return FALSE;
    }

    return FALSE;
}
#endif

static void RADEONSetupGenericConnectors(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt  = RADEONEntPriv(pScrn);

    if (IS_AVIVO_VARIANT)
	return;

    if (!pRADEONEnt->HasCRTC2) {
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[0].ConnectorType = CONNECTOR_VGA;
	info->BiosConnector[0].valid = TRUE;
	info->BiosConnector[0].devices = ATOM_DEVICE_CRT1_SUPPORT;
	radeon_add_encoder(pScrn,
			   radeon_get_encoder_id_from_supported_device(pScrn,
								       ATOM_DEVICE_CRT1_SUPPORT,
								       1),
			   ATOM_DEVICE_CRT1_SUPPORT);
	return;
    }

    if (info->IsMobility) {
	/* Below is the most common setting, but may not be true */
	if (info->IsIGP) {
	    info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_LCD_GPIO_MASK);
	    info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
	    info->BiosConnector[0].valid = TRUE;
	    info->BiosConnector[0].devices = ATOM_DEVICE_LCD1_SUPPORT;
	    radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_LCD1_SUPPORT,
									   0),
			       ATOM_DEVICE_LCD1_SUPPORT);

	    /* IGP only has TVDAC */
	    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
		(info->ChipFamily == CHIP_FAMILY_RS480))
		info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_CRT2_DDC);
	    else
		info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	    info->BiosConnector[1].load_detection = FALSE;
	    info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
	    info->BiosConnector[1].valid = TRUE;
	    info->BiosConnector[1].devices = ATOM_DEVICE_CRT1_SUPPORT;
	    radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_CRT1_SUPPORT,
									   2),
			       ATOM_DEVICE_CRT1_SUPPORT);
	} else {
#if defined(__powerpc__)
	    info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
#else
	    info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_LCD_GPIO_MASK);
#endif
	    info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
	    info->BiosConnector[0].valid = TRUE;
	    info->BiosConnector[0].devices = ATOM_DEVICE_LCD1_SUPPORT;
	    radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_LCD1_SUPPORT,
									   0),
			       ATOM_DEVICE_LCD1_SUPPORT);

	    info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	    info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
	    info->BiosConnector[1].valid = TRUE;
	    info->BiosConnector[1].devices = ATOM_DEVICE_CRT1_SUPPORT;
	    radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_CRT1_SUPPORT,
									   1),
			       ATOM_DEVICE_CRT1_SUPPORT);
	}
    } else {
	/* Below is the most common setting, but may not be true */
	if (info->IsIGP) {
	    if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
		(info->ChipFamily == CHIP_FAMILY_RS480))
		info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_CRT2_DDC);
	    else
		info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	    info->BiosConnector[0].load_detection = FALSE;
	    info->BiosConnector[0].ConnectorType = CONNECTOR_VGA;
	    info->BiosConnector[0].valid = TRUE;
	    info->BiosConnector[0].devices = ATOM_DEVICE_CRT1_SUPPORT;
	    radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_CRT1_SUPPORT,
									   1),
			       ATOM_DEVICE_CRT1_SUPPORT);

	    /* not sure what a good default DDCType for DVI on
	     * IGP desktop chips is
	     */
	    info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_MONID); /* DDC_DVI? */
	    info->BiosConnector[1].ConnectorType = CONNECTOR_DVI_D;
	    info->BiosConnector[1].valid = TRUE;
	    info->BiosConnector[1].devices = ATOM_DEVICE_DFP1_SUPPORT;
	    radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_DFP1_SUPPORT,
									   0),
			       ATOM_DEVICE_DFP1_SUPPORT);
	} else {
	    info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	    info->BiosConnector[0].load_detection = FALSE;
	    info->BiosConnector[0].ConnectorType = CONNECTOR_DVI_I;
	    info->BiosConnector[0].valid = TRUE;
	    info->BiosConnector[0].devices = ATOM_DEVICE_CRT2_SUPPORT | ATOM_DEVICE_DFP1_SUPPORT;
	    radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_CRT2_SUPPORT,
									   2),
			       ATOM_DEVICE_CRT2_SUPPORT);
	    radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_DFP1_SUPPORT,
									   0),
			       ATOM_DEVICE_DFP1_SUPPORT);

#if defined(__powerpc__)
	    info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	    info->BiosConnector[1].ConnectorType = CONNECTOR_DVI_I;
	    info->BiosConnector[1].valid = TRUE;
	    info->BiosConnector[1].devices = ATOM_DEVICE_CRT1_SUPPORT | ATOM_DEVICE_DFP2_SUPPORT;
	    radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_CRT1_SUPPORT,
									   1),
			       ATOM_DEVICE_CRT1_SUPPORT);
	    radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_DFP2_SUPPORT,
									   0),
			       ATOM_DEVICE_DFP2_SUPPORT);
#else
	    info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	    info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
	    info->BiosConnector[1].valid = TRUE;
	    info->BiosConnector[1].devices = ATOM_DEVICE_CRT1_SUPPORT;
	    radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_CRT1_SUPPORT,
									   1),
			       ATOM_DEVICE_CRT1_SUPPORT);
#endif
	}
    }

    if (info->InternalTVOut) {
	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].load_detection = FALSE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
	info->BiosConnector[2].devices = ATOM_DEVICE_TV1_SUPPORT;
	radeon_add_encoder(pScrn,
			       radeon_get_encoder_id_from_supported_device(pScrn,
									   ATOM_DEVICE_TV1_SUPPORT,
									   2),
			       ATOM_DEVICE_TV1_SUPPORT);
    }

    /* Some cards have the DDC lines swapped and we have no way to
     * detect it yet (Mac cards)
     */
    if (xf86ReturnOptValBool(info->Options, OPTION_REVERSE_DDC, FALSE)) {
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
    }
}

#if defined(__powerpc__)

#ifdef __OpenBSD__
#include <sys/param.h>
#include <sys/sysctl.h>
#endif

/*
 * Returns RADEONMacModel or 0 based on lines 'detected as' and 'machine'
 * in /proc/cpuinfo (on Linux) */
static RADEONMacModel RADEONDetectMacModel(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONMacModel ret = 0;
#ifdef __linux__
    char cpuline[50];  /* 50 should be sufficient for our purposes */
    FILE *f = fopen ("/proc/cpuinfo", "r");

    /* Some macs (minis and powerbooks) use internal tmds, others use external tmds
     * and not just for dual-link TMDS, it shows up with single-link as well.
     * Unforunately, there doesn't seem to be any good way to figure it out.
     */

    /*
     * PowerBook5,[1-5]: external tmds, single-link
     * PowerBook5,[789]: external tmds, dual-link
     * PowerBook5,6:     external tmds, single-link or dual-link
     * need to add another option to specify the external tmds chip
     * or find out what's used and add it.
     */


    if (f != NULL) {
	while (fgets(cpuline, sizeof cpuline, f)) {
	    if (!strncmp(cpuline, "machine", strlen ("machine"))) {
		if (strstr(cpuline, "PowerBook5,1") ||
		    strstr(cpuline, "PowerBook5,2") ||
		    strstr(cpuline, "PowerBook5,3") ||
		    strstr(cpuline, "PowerBook5,4") ||
		    strstr(cpuline, "PowerBook5,5")) {
		    ret = RADEON_MAC_POWERBOOK_EXTERNAL; /* single link */
		    info->ext_tmds_chip = RADEON_SIL_164; /* works on 5,2 */
		    break;
		}

		if (strstr(cpuline, "PowerBook5,6")) {
		    ret = RADEON_MAC_POWERBOOK_EXTERNAL; /* dual or single link */
		    break;
		}

		if (strstr(cpuline, "PowerBook5,7") ||
		    strstr(cpuline, "PowerBook5,8") ||
		    strstr(cpuline, "PowerBook5,9")) {
		    ret = RADEON_MAC_POWERBOOK_EXTERNAL; /* dual link */
		    info->ext_tmds_chip = RADEON_SIL_1178; /* guess */
		    break;
		}

		if (strstr(cpuline, "PowerBook3,3")) {
		    ret = RADEON_MAC_POWERBOOK_VGA; /* vga rather than dvi */
		    break;
		}

		if (strstr(cpuline, "PowerMac10,1")) {
		    ret = RADEON_MAC_MINI_INTERNAL; /* internal tmds */
		    break;
		}
		if (strstr(cpuline, "PowerMac10,2")) {
		    ret = RADEON_MAC_MINI_EXTERNAL; /* external tmds */
		    break;
		}
	    } else if (!strncmp(cpuline, "detected as", strlen("detected as"))) {
		if (strstr(cpuline, "iBook")) {
		    ret = RADEON_MAC_IBOOK;
		    break;
		} else if (strstr(cpuline, "PowerBook")) {
		    ret = RADEON_MAC_POWERBOOK_INTERNAL; /* internal tmds */
		    break;
		} else if (strstr(cpuline, "iMac G5 (iSight)")) {
		    ret = RADEON_MAC_IMAC_G5_ISIGHT;
		    break;
		} else if (strstr(cpuline, "eMac")) {
		    ret = RADEON_MAC_EMAC;
		    break;
		}

		/* No known PowerMac model detected */
		break;
	    }
	}

	fclose (f);
    } else
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Cannot detect PowerMac model because /proc/cpuinfo not "
		   "readable.\n");

#endif /* __linux */

#ifdef __OpenBSD__
    char model[32];
    int mib[2];
    size_t len;

    mib[0] = CTL_HW;
    mib[1] = HW_PRODUCT;
    len = sizeof(model);
    if (sysctl(mib, 2, model, &len, NULL, 0) >= 0) {
	if (strcmp(model, "PowerBook5,1") == 0 ||
	    strcmp(model, "PowerBook5,2") == 0 ||
	    strcmp(model, "PowerBook5,3") == 0 ||
	    strcmp(model, "PowerBook5,4") == 0 ||
	    strcmp(model, "PowerBook5,5") == 0) {
	    ret = RADEON_MAC_POWERBOOK_EXTERNAL; /* single link */
	    info->ext_tmds_chip = RADEON_SIL_164; /* works on 5,2 */
	}

	if (strcmp(model, "PowerBook5,6") == 0) {
	    ret = RADEON_MAC_POWERBOOK_EXTERNAL; /* dual or single link */
	}

	if (strcmp(model, "PowerBook5,7") ||
	    strcmp(model, "PowerBook5,8") == 0 ||
	    strcmp(model, "PowerBook5,9") == 0) {
	    ret = RADEON_MAC_POWERBOOK_EXTERNAL; /* dual link */
	    info->ext_tmds_chip = RADEON_SIL_1178; /* guess */
	}

	if (strcmp(model, "PowerBook3,3") == 0) {
	    ret = RADEON_MAC_POWERBOOK_VGA; /* vga rather than dvi */
	}

	if (strcmp(model, "PowerMac10,1") == 0) {
	    ret = RADEON_MAC_MINI_INTERNAL; /* internal tmds */
	}

	if (strcmp(model, "PowerMac10,2") == 0) {
	    ret = RADEON_MAC_MINI_EXTERNAL; /* external tmds */
	}

	if (strcmp(model, "PowerBook2,1") == 0 ||
	    strcmp(model, "PowerBook2,2") == 0 ||
	    strcmp(model, "PowerBook4,1") == 0 ||
	    strcmp(model, "PowerBook4,2") == 0 ||
	    strcmp(model, "PowerBook4,3") == 0 ||
	    strcmp(model, "PowerBook6,3") == 0 ||
	    strcmp(model, "PowerBook6,5") == 0 ||
	    strcmp(model, "PowerBook6,7") == 0) {
	    ret = RADEON_MAC_IBOOK;
	}

	if (strcmp(model, "PowerBook1,1") == 0 ||
	    strcmp(model, "PowerBook3,1") == 0 ||
	    strcmp(model, "PowerBook3,2") == 0 ||
	    strcmp(model, "PowerBook3,4") == 0 ||
	    strcmp(model, "PowerBook3,5") == 0) {
	    ret = RADEON_MAC_POWERBOOK_INTERNAL;
	}

	if (strcmp(model, "PowerMac12,1") == 0) {
	    ret = RADEON_MAC_IMAC_G5_ISIGHT;
	}
    }
#endif /* __OpenBSD__ */

    if (ret) {
	xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT, "Detected %s.\n",
		   ret == RADEON_MAC_POWERBOOK_EXTERNAL ? "PowerBook with external DVI" :
		   ret == RADEON_MAC_POWERBOOK_INTERNAL ? "PowerBook with integrated DVI" :
		   ret == RADEON_MAC_POWERBOOK_VGA ? "PowerBook with VGA" :
		   ret == RADEON_MAC_IBOOK ? "iBook" :
		   ret == RADEON_MAC_MINI_EXTERNAL ? "Mac Mini with external DVI" :
		   ret == RADEON_MAC_MINI_INTERNAL ? "Mac Mini with integrated DVI" :
		   "iMac G5 iSight");
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "If this is not correct, try Option \"MacModel\" and "
		   "consider reporting to the\n");
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "xorg-driver-ati@lists.x.org mailing list"
#ifdef __linux__
		   " with the contents of /proc/cpuinfo"
#endif
		   ".\n");
    }

    return ret;
}

#endif /* __powerpc__ */

static int
radeon_output_clones (ScrnInfoPtr pScrn, xf86OutputPtr output)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR (pScrn);
    int			o;
    int			index_mask = 0;

    /* no cloning with zaphod */
    if (info->IsPrimary || info->IsSecondary)
	return index_mask;

    /* DIG routing gets problematic */
    if (info->ChipFamily >= CHIP_FAMILY_R600)
	return index_mask;

    /* LVDS is too wacky */
    if (radeon_output->devices & (ATOM_DEVICE_LCD_SUPPORT))
	return index_mask;

    /* TV requires very specific timing */
    if (radeon_output->devices & (ATOM_DEVICE_TV_SUPPORT))
	return index_mask;

    /* DVO requires 2x ppll clocks depending on the tmds chip */
    if (radeon_output->devices & (ATOM_DEVICE_DFP2_SUPPORT))
	return index_mask;

    for (o = 0; o < config->num_output; o++) {
	xf86OutputPtr clone = config->output[o];
	RADEONOutputPrivatePtr radeon_clone = clone->driver_private;

	if (output == clone) /* don't clone yourself */
	    continue;
	else if (radeon_clone->devices & (ATOM_DEVICE_LCD_SUPPORT)) /* LVDS */
	    continue;
	else if (radeon_clone->devices & (ATOM_DEVICE_TV_SUPPORT)) /* TV */
	    continue;
	else
	    index_mask |= (1 << o);
    }

    return index_mask;
}

static xf86OutputPtr
RADEONOutputCreate(ScrnInfoPtr pScrn, const char *name, int i)
{
    char buf[32];
    sprintf(buf, name, i);
    return xf86OutputCreate(pScrn, &radeon_output_funcs, buf);
}

/*
 * initialise the static data sos we don't have to re-do at randr change */
Bool RADEONSetupConnectors(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    xf86OutputPtr output;
    char *optstr;
    int i;
    int num_vga = 0;
    int num_dvi = 0;
    int num_hdmi = 0;
    int num_dp = 0;
    int num_edp = 0;

    /* We first get the information about all connectors from BIOS.
     * This is how the card is phyiscally wired up.
     * The information should be correct even on a OEM card.
     */
    for (i = 0; i < RADEON_MAX_BIOS_CONNECTOR; i++) {
	info->encoders[i] = NULL;
	info->BiosConnector[i].valid = FALSE;
	info->BiosConnector[i].load_detection = TRUE;
	info->BiosConnector[i].shared_ddc = FALSE;
	info->BiosConnector[i].ddc_i2c.valid = FALSE;
	info->BiosConnector[i].ConnectorType = CONNECTOR_NONE;
	info->BiosConnector[i].devices = 0;
    }

#if defined(__powerpc__)
    info->MacModel = 0;
    optstr = (char *)xf86GetOptValString(info->Options, OPTION_MAC_MODEL);
    if (optstr) {
	if (!strncmp("ibook", optstr, strlen("ibook")))
	    info->MacModel = RADEON_MAC_IBOOK;
	else if (!strncmp("powerbook-duallink", optstr, strlen("powerbook-duallink"))) /* alias */
	    info->MacModel = RADEON_MAC_POWERBOOK_EXTERNAL;
	else if (!strncmp("powerbook-external", optstr, strlen("powerbook-external")))
	    info->MacModel = RADEON_MAC_POWERBOOK_EXTERNAL;
	else if (!strncmp("powerbook-internal", optstr, strlen("powerbook-internal")))
	    info->MacModel = RADEON_MAC_POWERBOOK_INTERNAL;
	else if (!strncmp("powerbook-vga", optstr, strlen("powerbook-vga")))
	    info->MacModel = RADEON_MAC_POWERBOOK_VGA;
	else if (!strncmp("powerbook", optstr, strlen("powerbook"))) /* alias */
	    info->MacModel = RADEON_MAC_POWERBOOK_INTERNAL;
	else if (!strncmp("mini-internal", optstr, strlen("mini-internal")))
	    info->MacModel = RADEON_MAC_MINI_INTERNAL;
	else if (!strncmp("mini-external", optstr, strlen("mini-external")))
	    info->MacModel = RADEON_MAC_MINI_EXTERNAL;
	else if (!strncmp("mini", optstr, strlen("mini"))) /* alias */
	    info->MacModel = RADEON_MAC_MINI_EXTERNAL;
	else if (!strncmp("imac-g5-isight", optstr, strlen("imac-g5-isight")))
	    info->MacModel = RADEON_MAC_IMAC_G5_ISIGHT;
	else if (!strncmp("emac", optstr, strlen("emac")))
	    info->MacModel = RADEON_MAC_EMAC;
	else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Invalid Mac Model: %s\n", optstr);
	}
    }

    if (!info->MacModel) {
	info->MacModel = RADEONDetectMacModel(pScrn);
    }

    if (info->MacModel){
	if (!RADEONSetupAppleConnectors(pScrn))
	    RADEONSetupGenericConnectors(pScrn);
    } else
#endif
    if (xf86ReturnOptValBool(info->Options, OPTION_DEFAULT_CONNECTOR_TABLE, FALSE)) {
	RADEONSetupGenericConnectors(pScrn);
    } else {
	if (!RADEONGetConnectorInfoFromBIOS(pScrn))
	    RADEONSetupGenericConnectors(pScrn);
    }

    /* parse connector table option */
    optstr = (char *)xf86GetOptValString(info->Options, OPTION_CONNECTORTABLE);

    if (optstr) {
	unsigned int ddc_line[2];
	int DACType[2], TMDSType[2];

	for (i = 2; i < RADEON_MAX_BIOS_CONNECTOR; i++) {
	    info->BiosConnector[i].valid = FALSE;
	}

	if (sscanf(optstr, "%u,%u,%u,%u,%u,%u,%u,%u",
		   &ddc_line[0],
		   &DACType[0],
		   &TMDSType[0],
		   &info->BiosConnector[0].ConnectorType,
		   &ddc_line[1],
		   &DACType[1],
		   &TMDSType[1],
		   &info->BiosConnector[1].ConnectorType) != 8) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Invalid ConnectorTable option: %s\n", optstr);
	    return FALSE;
	}

	for (i = 0; i < 2; i++) {
	    info->BiosConnector[i].valid = TRUE;
	    info->BiosConnector[i].ddc_i2c = legacy_setup_i2c_bus(ddc_line[i]);
	    switch (DACType[i]) {
	    case 1:
		info->BiosConnector[i].devices |= ATOM_DEVICE_CRT1_SUPPORT;
		if (!radeon_add_encoder(pScrn,
					radeon_get_encoder_id_from_supported_device(pScrn,
										    ATOM_DEVICE_CRT1_SUPPORT,
										    1),
					ATOM_DEVICE_CRT1_SUPPORT))
		    return FALSE;
		info->BiosConnector[i].load_detection = TRUE;
		break;
	    case 2:
		info->BiosConnector[i].devices |= ATOM_DEVICE_CRT2_SUPPORT;
		if (!radeon_add_encoder(pScrn,
					radeon_get_encoder_id_from_supported_device(pScrn,
										    ATOM_DEVICE_CRT1_SUPPORT,
										    2),
					ATOM_DEVICE_CRT1_SUPPORT))
		    return FALSE;
		info->BiosConnector[i].load_detection = FALSE;
		break;
	    }
	    switch (TMDSType[i]) {
	    case 1:
		info->BiosConnector[i].devices |= ATOM_DEVICE_DFP1_SUPPORT;
		if (!radeon_add_encoder(pScrn,
					radeon_get_encoder_id_from_supported_device(pScrn,
										    ATOM_DEVICE_DFP1_SUPPORT,
										    0),
					ATOM_DEVICE_DFP1_SUPPORT))
		    return FALSE;
		break;
	    case 2:
		info->BiosConnector[i].devices |= ATOM_DEVICE_DFP2_SUPPORT;
		if (!radeon_add_encoder(pScrn,
					radeon_get_encoder_id_from_supported_device(pScrn,
										    ATOM_DEVICE_DFP2_SUPPORT,
										    0),
					ATOM_DEVICE_DFP2_SUPPORT))
		    return FALSE;
		break;
	    }
	}
    }

    for (i = 0; i < RADEON_MAX_BIOS_CONNECTOR; i++) {
	if (info->BiosConnector[i].valid) {
	    RADEONConnectorType conntype = info->BiosConnector[i].ConnectorType;
	    if ((conntype == CONNECTOR_DVI_D) ||
		(conntype == CONNECTOR_DVI_I) ||
		(conntype == CONNECTOR_DVI_A) ||
		(conntype == CONNECTOR_HDMI_TYPE_B)) {
		num_dvi++;
	    } else if (conntype == CONNECTOR_VGA) {
		num_vga++;
	    } else if (conntype == CONNECTOR_HDMI_TYPE_A) {
		num_hdmi++;
	    } else if (conntype == CONNECTOR_DISPLAY_PORT) {
		num_dp++;
	    } else if (conntype == CONNECTOR_EDP) {
		num_edp++;
	    }
	}
    }

    for (i = 0 ; i < RADEON_MAX_BIOS_CONNECTOR; i++) {
	if (info->BiosConnector[i].valid) {
	    RADEONOutputPrivatePtr radeon_output;
	    RADEONConnectorType conntype = info->BiosConnector[i].ConnectorType;

	    if (conntype == CONNECTOR_NONE)
		continue;

	    radeon_output = xnfcalloc(sizeof(RADEONOutputPrivateRec), 1);
	    if (!radeon_output) {
		return FALSE;
	    }
	    radeon_output->MonType = MT_UNKNOWN;
	    radeon_output->ConnectorType = conntype;
	    radeon_output->devices = info->BiosConnector[i].devices;
	    radeon_output->ddc_i2c = info->BiosConnector[i].ddc_i2c;
	    radeon_output->igp_lane_info = info->BiosConnector[i].igp_lane_info;
	    radeon_output->shared_ddc = info->BiosConnector[i].shared_ddc;
	    radeon_output->load_detection = info->BiosConnector[i].load_detection;
	    radeon_output->linkb = info->BiosConnector[i].linkb;
	    radeon_output->dig_encoder = -1;
	    radeon_output->connector_id = info->BiosConnector[i].connector_object;
	    radeon_output->connector_object_id = info->BiosConnector[i].connector_object_id;
	    radeon_output->ucI2cId = info->BiosConnector[i].ucI2cId;
	    radeon_output->hpd_id = info->BiosConnector[i].hpd_id;

	    /* Technically HDMI-B is a glorfied DL DVI so the bios is correct,
	     * but this can be confusing to users when it comes to output names,
	     * so call it DVI
	     */
	    if ((conntype == CONNECTOR_DVI_D) ||
		(conntype == CONNECTOR_DVI_I) ||
		(conntype == CONNECTOR_DVI_A) ||
		(conntype == CONNECTOR_HDMI_TYPE_B)) {
		output = RADEONOutputCreate(pScrn, "DVI-%d", --num_dvi);
	    } else if (conntype == CONNECTOR_VGA) {
		output = RADEONOutputCreate(pScrn, "VGA-%d", --num_vga);
	    } else if (conntype == CONNECTOR_HDMI_TYPE_A) {
		output = RADEONOutputCreate(pScrn, "HDMI-%d", --num_hdmi);
	    } else if (conntype == CONNECTOR_DISPLAY_PORT) {
		output = RADEONOutputCreate(pScrn, "DisplayPort-%d", --num_dp);
	    } else if (conntype == CONNECTOR_EDP) {
		output = RADEONOutputCreate(pScrn, "eDP-%d", --num_edp);
	    } else {
		output = RADEONOutputCreate(pScrn,
					    ConnectorTypeName[conntype], 0);
	    }

	    if (!output) {
		return FALSE;
	    }
	    output->interlaceAllowed = TRUE;
	    output->doubleScanAllowed = TRUE;
	    output->driver_private = radeon_output;
	    if (IS_DCE4_VARIANT) {
		output->possible_crtcs = 0x3f;
	    } else {
		output->possible_crtcs = 1;
		/* crtc2 can drive LVDS, it just doesn't have RMX */
		if (!(radeon_output->devices & (ATOM_DEVICE_LCD_SUPPORT)))
		    output->possible_crtcs |= 2;
	    }

	    /* we can clone the DACs, and probably TV-out,
	       but I'm not sure it's worth the trouble */
	    output->possible_clones = 0;

	    RADEONInitConnector(output);
	}
    }

    for (i = 0; i < xf86_config->num_output; i++) {
	xf86OutputPtr output = xf86_config->output[i];

	output->possible_clones = radeon_output_clones(pScrn, output);
	RADEONGetHardCodedEDIDFromFile(output);
    }

    return TRUE;
}

