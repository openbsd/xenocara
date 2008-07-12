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

const char *MonTypeName[10] = {
  "AUTO",
  "NONE",
  "CRT",
  "LVDS",
  "TMDS",
  "CTV",
  "STV",
  "CV",
  "HDMI",
  "DP"
};

const RADEONMonitorType MonTypeID[10] = {
  MT_UNKNOWN, /* this is just a dummy value for AUTO DETECTION */
  MT_NONE,    /* NONE -> NONE */
  MT_CRT,     /* CRT -> CRT */
  MT_LCD,     /* Laptop LCDs are driven via LVDS port */
  MT_DFP,     /* DFPs are driven via TMDS */
  MT_CTV,     /* CTV -> CTV */
  MT_STV,     /* STV -> STV */
  MT_CV,
  MT_HDMI,
  MT_DP
};

const char *TMDSTypeName[6] = {
  "None",
  "Internal",
  "External",
  "LVTMA",
  "DDIA",
  "UNIPHY"
};

const char *DACTypeName[4] = {
  "None",
  "Primary",
  "TVDAC/ExtDAC",
  "ExtDac"
};

const char *ConnectorTypeName[17] = {
  "None",
  "VGA",
  "DVI-I",
  "DVI-D",
  "DVI-A",
  "STV",
  "CTV",
  "LVDS",
  "Digital",
  "SCART",
  "HDMI-A",
  "HDMI-B",
  "Unsupported",
  "Unsupported",
  "DIN",
  "DisplayPort",
  "Unsupported"
};

const char *OutputType[11] = {
    "None",
    "VGA",
    "DVI",
    "DVI",
    "DVI",
    "LVDS",
    "S-video",
    "Composite",
    "Component",
    "HDMI",
    "DisplayPort",
};

static const RADEONTMDSPll default_tmds_pll[CHIP_FAMILY_LAST][4] =
{
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},				/*CHIP_FAMILY_UNKNOW*/
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},				/*CHIP_FAMILY_LEGACY*/
    {{12000, 0xa1b}, {0xffffffff, 0xa3f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RADEON*/
    {{12000, 0xa1b}, {0xffffffff, 0xa3f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RV100*/
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},				/*CHIP_FAMILY_RS100*/
    {{15000, 0xa1b}, {0xffffffff, 0xa3f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RV200*/
    {{12000, 0xa1b}, {0xffffffff, 0xa3f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RS200*/
    {{15000, 0xa1b}, {0xffffffff, 0xa3f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_R200*/
    {{15500, 0x81b}, {0xffffffff, 0x83f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RV250*/
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},				/*CHIP_FAMILY_RS300*/
    {{13000, 0x400f4}, {15000, 0x400f7}, {0xffffffff, 0x40111}, {0, 0}}, /*CHIP_FAMILY_RV280*/
    {{0xffffffff, 0xb01cb}, {0, 0}, {0, 0}, {0, 0}},		/*CHIP_FAMILY_R300*/
    {{0xffffffff, 0xb01cb}, {0, 0}, {0, 0}, {0, 0}},		/*CHIP_FAMILY_R350*/
    {{15000, 0xb0155}, {0xffffffff, 0xb01cb}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RV350*/
    {{15000, 0xb0155}, {0xffffffff, 0xb01cb}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RV380*/
    {{0xffffffff, 0xb01cb}, {0, 0}, {0, 0}, {0, 0}},		/*CHIP_FAMILY_R420*/
    {{0xffffffff, 0xb01cb}, {0, 0}, {0, 0}, {0, 0}},		/*CHIP_FAMILY_RV410*/ /* FIXME: just values from r420 used... */
    {{15000, 0xb0155}, {0xffffffff, 0xb01cb}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RS400*/ /* FIXME: just values from rv380 used... */
    {{15000, 0xb0155}, {0xffffffff, 0xb01cb}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RS480*/ /* FIXME: just values from rv380 used... */
};

static const uint32_t default_tvdac_adj [CHIP_FAMILY_LAST] =
{
    0x00000000,   /* unknown */
    0x00000000,   /* legacy */
    0x00000000,   /* r100 */
    0x00280000,   /* rv100 */
    0x00000000,   /* rs100 */
    0x00880000,   /* rv200 */
    0x00000000,   /* rs200 */
    0x00000000,   /* r200 */
    0x00770000,   /* rv250 */
    0x00290000,   /* rs300 */
    0x00560000,   /* rv280 */
    0x00780000,   /* r300 */
    0x00770000,   /* r350 */
    0x00780000,   /* rv350 */
    0x00780000,   /* rv380 */
    0x01080000,   /* r420 */
    0x01080000,   /* rv410 */ /* FIXME: just values from r420 used... */
    0x00780000,   /* rs400 */ /* FIXME: just values from rv380 used... */
    0x00780000,   /* rs480 */ /* FIXME: just values from rv380 used... */
};


static void RADEONUpdatePanelSize(xf86OutputPtr output);
static void RADEONGetTMDSInfoFromTable(xf86OutputPtr output);
#define AVIVO_I2C_DISABLE 0
#define AVIVO_I2C_ENABLE 1
static Bool AVIVOI2CDoLock(xf86OutputPtr output, int lock_state);

extern void atombios_output_mode_set(xf86OutputPtr output,
				     DisplayModePtr mode,
				     DisplayModePtr adjusted_mode);
extern void atombios_output_dpms(xf86OutputPtr output, int mode);
extern RADEONMonitorType atombios_dac_detect(ScrnInfoPtr pScrn, xf86OutputPtr output);
extern int atombios_external_tmds_setup(xf86OutputPtr output, DisplayModePtr mode);
static void
radeon_bios_output_dpms(xf86OutputPtr output, int mode);
static void
radeon_bios_output_crtc(xf86OutputPtr output);
static void
radeon_bios_output_lock(xf86OutputPtr output, Bool lock);

void RADEONPrintPortMap(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    RADEONOutputPrivatePtr radeon_output;
    xf86OutputPtr output;
    int o;

    for (o = 0; o < xf86_config->num_output; o++) {
	output = xf86_config->output[o];
	radeon_output = output->driver_private;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Port%d:\n Monitor   -- %s\n Connector -- %s\n DAC Type  -- %s\n TMDS Type -- %s\n DDC Type  -- 0x%x\n", 
		   o,
		   MonTypeName[radeon_output->MonType+1],
		   ConnectorTypeName[radeon_output->ConnectorType],
		   DACTypeName[radeon_output->DACType],
		   TMDSTypeName[radeon_output->TMDSType],
		   (unsigned int)radeon_output->ddc_i2c.mask_clk_reg);
    }

}

static xf86MonPtr
radeon_do_ddc(xf86OutputPtr output)
{
    RADEONInfoPtr info = RADEONPTR(output->scrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t DDCReg;
    xf86MonPtr MonInfo = NULL;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int i, j;

    if (radeon_output->pI2CBus) {
	DDCReg = radeon_output->ddc_i2c.mask_clk_reg;

	if (IS_AVIVO_VARIANT) {
	    AVIVOI2CDoLock(output, AVIVO_I2C_ENABLE);
	    MonInfo = xf86OutputGetEDID(output, radeon_output->pI2CBus);
	    AVIVOI2CDoLock(output, AVIVO_I2C_DISABLE);
	} else if ((DDCReg == RADEON_LCD_GPIO_MASK) || (DDCReg == RADEON_MDGPIO_EN_REG)) {
	    MonInfo = xf86OutputGetEDID(output, radeon_output->pI2CBus);
	} else {
	    OUTREG(DDCReg, INREG(DDCReg) &
		   (uint32_t)~(RADEON_GPIO_A_0 | RADEON_GPIO_A_1));

	    /* For some old monitors (like Compaq Presario FP500), we need
	     * following process to initialize/stop DDC
	     */
	    OUTREG(DDCReg, INREG(DDCReg) & ~(RADEON_GPIO_EN_1));
	    for (j = 0; j < 3; j++) {
		OUTREG(DDCReg,
		       INREG(DDCReg) & ~(RADEON_GPIO_EN_0));
		usleep(13000);

		OUTREG(DDCReg,
		       INREG(DDCReg) & ~(RADEON_GPIO_EN_1));
		for (i = 0; i < 10; i++) {
		    usleep(15000);
		    if (INREG(DDCReg) & RADEON_GPIO_Y_1)
			break;
		}
		if (i == 10) continue;

		usleep(15000);

		OUTREG(DDCReg, INREG(DDCReg) | RADEON_GPIO_EN_0);
		usleep(15000);

		OUTREG(DDCReg, INREG(DDCReg) | RADEON_GPIO_EN_1);
		usleep(15000);
		OUTREG(DDCReg,
		       INREG(DDCReg) & ~(RADEON_GPIO_EN_0));
		usleep(15000);

		MonInfo = xf86OutputGetEDID(output, radeon_output->pI2CBus);

		OUTREG(DDCReg, INREG(DDCReg) | RADEON_GPIO_EN_1);
		OUTREG(DDCReg, INREG(DDCReg) | RADEON_GPIO_EN_0);
		usleep(15000);
		OUTREG(DDCReg,
		       INREG(DDCReg) & ~(RADEON_GPIO_EN_1));
		for (i = 0; i < 5; i++) {
		    usleep(15000);
		    if (INREG(DDCReg) & RADEON_GPIO_Y_1)
			break;
		}
		usleep(15000);
		OUTREG(DDCReg,
		       INREG(DDCReg) & ~(RADEON_GPIO_EN_0));
		usleep(15000);

		OUTREG(DDCReg, INREG(DDCReg) | RADEON_GPIO_EN_1);
		OUTREG(DDCReg, INREG(DDCReg) | RADEON_GPIO_EN_0);
		usleep(15000);
		if (MonInfo)  break;
	    }
	    OUTREG(DDCReg, INREG(DDCReg) &
		   ~(RADEON_GPIO_EN_0 | RADEON_GPIO_EN_1));
	}
    }

    return MonInfo;
}

static RADEONMonitorType
radeon_ddc_connected(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn        = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONMonitorType MonType = MT_NONE;
    xf86MonPtr MonInfo = NULL;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if (radeon_output->pI2CBus)
	MonInfo = radeon_do_ddc(output);
    if (MonInfo) {
	if (!xf86ReturnOptValBool(info->Options, OPTION_IGNORE_EDID, FALSE))
	    xf86OutputSetEDID(output, MonInfo);
	if (radeon_output->type == OUTPUT_LVDS)
	    MonType = MT_LCD;
	else if (radeon_output->type == OUTPUT_DVI_D)
	    MonType = MT_DFP;
	else if (radeon_output->type == OUTPUT_HDMI)
	    MonType = MT_DFP;
	else if (radeon_output->type == OUTPUT_DP)
	    MonType = MT_DFP;
	else if (radeon_output->type == OUTPUT_DVI_I && (MonInfo->rawData[0x14] & 0x80)) /* if it's digital and DVI */
	    MonType = MT_DFP;
	else
	    MonType = MT_CRT;
    } else
	MonType = MT_NONE;
    
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Output: %s, Detected Monitor Type: %d\n", output->name, MonType);

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
RADEONConnectorFindMonitor(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn        = output->scrn;
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if (radeon_output->MonType == MT_UNKNOWN) {
	radeon_output->MonType = radeon_ddc_connected(output);
	if (!radeon_output->MonType) {
	    if (radeon_output->type == OUTPUT_LVDS) {
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
		    radeon_output->MonType = atombios_dac_detect(pScrn, output);
		else
		    radeon_output->MonType = legacy_dac_detect(pScrn, output);
	    }
	}
    }

    /* update panel info for RMX */
    if (radeon_output->MonType == MT_LCD || radeon_output->MonType == MT_DFP)
	RADEONUpdatePanelSize(output);

    /* panel is probably busted or not connected */
    if ((radeon_output->MonType == MT_LCD) &&
	((radeon_output->PanelXRes == 0) || (radeon_output->PanelYRes == 0)))
	radeon_output->MonType = MT_NONE;

    if (output->MonInfo) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "EDID data from the display on output: %s ----------------------\n",
		   output->name);
	xf86PrintEDID( output->MonInfo );
    }
}

static void
radeon_dpms(xf86OutputPtr output, int mode)
{
    RADEONInfoPtr info = RADEONPTR(output->scrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if ((mode == DPMSModeOn) && radeon_output->enabled)
	return;

    if (IS_AVIVO_VARIANT) {
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

    if (OUTPUT_IS_TV) {
	/* FIXME: Update when more modes are added */
	if (IS_AVIVO_VARIANT) {
	    int max_v;

	    /* tv-scaler can scale horizontal width
	     * but frame ends must match tv_pll
	     * for now cap v size
	     */
	    if (radeon_output->tvStd == TV_STD_NTSC ||
		radeon_output->tvStd == TV_STD_NTSC_J ||
		radeon_output->tvStd == TV_STD_PAL_M)
		max_v = 480;
	    else
		max_v = 600;

	    if (pMode->VDisplay == max_v)
		return MODE_OK;
	    else
		return MODE_CLOCK_RANGE;
	} else {
	    if (pMode->HDisplay == 800 && pMode->VDisplay == 600)
		return MODE_OK;
	    else
		return MODE_CLOCK_RANGE;
	}
    }

    if (radeon_output->type == OUTPUT_LVDS) {
	if (radeon_output->rmx_type == RMX_OFF) {
	    if (pMode->HDisplay != radeon_output->PanelXRes ||
		pMode->VDisplay != radeon_output->PanelYRes)
		return MODE_PANEL;
	}
	if (pMode->HDisplay > radeon_output->PanelXRes ||
	    pMode->VDisplay > radeon_output->PanelYRes)
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

    radeon_output->Flags &= ~RADEON_USE_RMX;

    /* decide if we are using RMX */
    if ((radeon_output->MonType == MT_LCD || radeon_output->MonType == MT_DFP)
	&& radeon_output->rmx_type != RMX_OFF) {
	xf86CrtcPtr crtc = output->crtc;
	RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;

	if (IS_AVIVO_VARIANT || radeon_crtc->crtc_id == 0) {
	    if (mode->HDisplay < radeon_output->PanelXRes ||
		mode->VDisplay < radeon_output->PanelYRes) {
		radeon_output->Flags |= RADEON_USE_RMX;
		if (IS_AVIVO_VARIANT) {
		    /* set to the panel's native mode */
		    adjusted_mode->HDisplay = radeon_output->PanelXRes;
		    adjusted_mode->HDisplay = radeon_output->PanelYRes;
		    adjusted_mode->HTotal = radeon_output->PanelXRes + radeon_output->HBlank;
		    adjusted_mode->HSyncStart = radeon_output->PanelXRes + radeon_output->HOverPlus;
		    adjusted_mode->HSyncEnd = adjusted_mode->HSyncStart + radeon_output->HSyncWidth;
		    adjusted_mode->VTotal = radeon_output->PanelYRes + radeon_output->VBlank;
		    adjusted_mode->VSyncStart = radeon_output->PanelYRes + radeon_output->VOverPlus;
		    adjusted_mode->VSyncEnd = adjusted_mode->VSyncStart + radeon_output->VSyncWidth;
		    /* update crtc values */
		    xf86SetModeCrtc(adjusted_mode, INTERLACE_HALVE_V);
		    /* adjust crtc values */
		    adjusted_mode->CrtcHDisplay = radeon_output->PanelXRes;
		    adjusted_mode->CrtcVDisplay = radeon_output->PanelYRes;
		    adjusted_mode->CrtcHTotal = adjusted_mode->CrtcHDisplay + radeon_output->HBlank;
		    adjusted_mode->CrtcHSyncStart = adjusted_mode->CrtcHDisplay + radeon_output->HOverPlus;
		    adjusted_mode->CrtcHSyncEnd = adjusted_mode->CrtcHSyncStart + radeon_output->HSyncWidth;
		    adjusted_mode->CrtcVTotal = adjusted_mode->CrtcVDisplay + radeon_output->VBlank;
		    adjusted_mode->CrtcVSyncStart = adjusted_mode->CrtcVDisplay + radeon_output->VOverPlus;
		    adjusted_mode->CrtcVSyncEnd = adjusted_mode->CrtcVSyncStart + radeon_output->VSyncWidth;
		} else {
		    /* set to the panel's native mode */
		    adjusted_mode->HTotal = radeon_output->PanelXRes + radeon_output->HBlank;
		    adjusted_mode->HSyncStart = radeon_output->PanelXRes + radeon_output->HOverPlus;
		    adjusted_mode->HSyncEnd = adjusted_mode->HSyncStart + radeon_output->HSyncWidth;
		    adjusted_mode->VTotal = radeon_output->PanelYRes + radeon_output->VBlank;
		    adjusted_mode->VSyncStart = radeon_output->PanelYRes + radeon_output->VOverPlus;
		    adjusted_mode->VSyncEnd = adjusted_mode->VSyncStart + radeon_output->VSyncWidth;
		    adjusted_mode->Clock = radeon_output->DotClock;
		    /* update crtc values */
		    xf86SetModeCrtc(adjusted_mode, INTERLACE_HALVE_V);
		    /* adjust crtc values */
		    adjusted_mode->CrtcHTotal = adjusted_mode->CrtcHDisplay + radeon_output->HBlank;
		    adjusted_mode->CrtcHSyncStart = adjusted_mode->CrtcHDisplay + radeon_output->HOverPlus;
		    adjusted_mode->CrtcHSyncEnd = adjusted_mode->CrtcHSyncStart + radeon_output->HSyncWidth;
		    adjusted_mode->CrtcVTotal = adjusted_mode->CrtcVDisplay + radeon_output->VBlank;
		    adjusted_mode->CrtcVSyncStart = adjusted_mode->CrtcVDisplay + radeon_output->VOverPlus;
		    adjusted_mode->CrtcVSyncEnd = adjusted_mode->CrtcVSyncStart + radeon_output->VSyncWidth;
		}
		adjusted_mode->Clock = radeon_output->DotClock;
		adjusted_mode->Flags = radeon_output->Flags;
	    }
	}
    }

    return TRUE;
}

static void
radeon_mode_prepare(xf86OutputPtr output)
{
    xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR (output->scrn);
    int o;

    for (o = 0; o < config->num_output; o++) {
	xf86OutputPtr loop_output = config->output[o];
	if (loop_output == output)
	    continue;
	else if (loop_output->crtc) {
	    xf86CrtcPtr other_crtc = loop_output->crtc;
	    if (other_crtc->enabled) {
		radeon_dpms(loop_output, DPMSModeOff);
		radeon_crtc_dpms(other_crtc, DPMSModeOff);
	    }
	}
    }

    radeon_bios_output_lock(output, TRUE);
    radeon_dpms(output, DPMSModeOff);
    radeon_crtc_dpms(output->crtc, DPMSModeOff);

}

static void
radeon_mode_set(xf86OutputPtr output, DisplayModePtr mode,
		DisplayModePtr adjusted_mode)
{
    RADEONInfoPtr info = RADEONPTR(output->scrn);

    if (IS_AVIVO_VARIANT)
	atombios_output_mode_set(output, mode, adjusted_mode);
    else
	legacy_output_mode_set(output, mode, adjusted_mode);
    radeon_bios_output_crtc(output);

}

static void
radeon_mode_commit(xf86OutputPtr output)
{
    xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR (output->scrn);
    int o;

    for (o = 0; o < config->num_output; o++) {
	xf86OutputPtr loop_output = config->output[o];
	if (loop_output == output)
	    continue;
	else if (loop_output->crtc) {
	    xf86CrtcPtr other_crtc = loop_output->crtc;
	    if (other_crtc->enabled) {
		radeon_dpms(loop_output, DPMSModeOn);		
		radeon_crtc_dpms(other_crtc, DPMSModeOn);
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
	if (mode == DPMSModeOn) {
	    if (radeon_output->MonType == MT_STV ||
		radeon_output->MonType == MT_CTV) {
		if (radeon_output->devices & ATOM_DEVICE_TV1_SUPPORT) {
		    save->bios_2_scratch &= ~ATOM_S2_TV1_DPMS_STATE;
		    save->bios_3_scratch |= ATOM_S3_TV1_ACTIVE;
		}
	    } else if (radeon_output->MonType == MT_CV) {
		if (radeon_output->devices & ATOM_DEVICE_CV_SUPPORT) {
		    save->bios_2_scratch &= ~ATOM_S2_CV_DPMS_STATE;
		    save->bios_3_scratch |= ATOM_S3_CV_ACTIVE;
		}
	    } else if (radeon_output->MonType == MT_CRT) {
		if (radeon_output->devices & ATOM_DEVICE_CRT1_SUPPORT) {
		    save->bios_2_scratch &= ~ATOM_S2_CRT1_DPMS_STATE;
		    save->bios_3_scratch |= ATOM_S3_CRT1_ACTIVE;
		} else if (radeon_output->devices & ATOM_DEVICE_CRT2_SUPPORT) {
		    save->bios_2_scratch &= ~ATOM_S2_CRT2_DPMS_STATE;
		    save->bios_3_scratch |= ATOM_S3_CRT2_ACTIVE;
		}
	    } else if (radeon_output->MonType == MT_LCD) {
		if (radeon_output->devices & ATOM_DEVICE_LCD1_SUPPORT) {
		    save->bios_2_scratch &= ~ATOM_S2_LCD1_DPMS_STATE;
		    save->bios_3_scratch |= ATOM_S3_LCD1_ACTIVE;
		}
	    } else if (radeon_output->MonType == MT_DFP) {
		if (radeon_output->devices & ATOM_DEVICE_DFP1_SUPPORT) {
		    save->bios_2_scratch &= ~ATOM_S2_DFP1_DPMS_STATE;
		    save->bios_3_scratch |= ATOM_S3_DFP1_ACTIVE;
		} else if (radeon_output->devices & ATOM_DEVICE_DFP2_SUPPORT) {
		    save->bios_2_scratch &= ~ATOM_S2_DFP2_DPMS_STATE;
		    save->bios_3_scratch |= ATOM_S3_DFP2_ACTIVE;
		} else if (radeon_output->devices & ATOM_DEVICE_DFP3_SUPPORT) {
		    save->bios_2_scratch &= ~ATOM_S2_DFP3_DPMS_STATE;
		    save->bios_3_scratch |= ATOM_S3_DFP3_ACTIVE;
		}
	    }
	} else {
	    if (radeon_output->MonType == MT_STV ||
		radeon_output->MonType == MT_CTV) {
		if (radeon_output->devices & ATOM_DEVICE_TV1_SUPPORT) {
		    save->bios_2_scratch |= ATOM_S2_TV1_DPMS_STATE;
		    save->bios_3_scratch &= ~ATOM_S3_TV1_ACTIVE;
		}
	    } else if (radeon_output->MonType == MT_CV) {
		if (radeon_output->devices & ATOM_DEVICE_CV_SUPPORT) {
		    save->bios_2_scratch |= ATOM_S2_CV_DPMS_STATE;
		    save->bios_3_scratch &= ~ATOM_S3_CV_ACTIVE;
		}
	    } else if (radeon_output->MonType == MT_CRT) {
		if (radeon_output->devices & ATOM_DEVICE_CRT1_SUPPORT) {
		    save->bios_2_scratch |= ATOM_S2_CRT1_DPMS_STATE;
		    save->bios_3_scratch &= ~ATOM_S3_CRT1_ACTIVE;
		} else if (radeon_output->devices & ATOM_DEVICE_CRT2_SUPPORT) {
		    save->bios_2_scratch |= ATOM_S2_CRT2_DPMS_STATE;
		    save->bios_3_scratch &= ~ATOM_S3_CRT2_ACTIVE;
		}
	    } else if (radeon_output->MonType == MT_LCD) {
		if (radeon_output->devices & ATOM_DEVICE_LCD1_SUPPORT) {
		    save->bios_2_scratch |= ATOM_S2_LCD1_DPMS_STATE;
		    save->bios_3_scratch &= ~ATOM_S3_LCD1_ACTIVE;
		}
	    } else if (radeon_output->MonType == MT_DFP) {
		if (radeon_output->devices & ATOM_DEVICE_DFP1_SUPPORT) {
		    save->bios_2_scratch |= ATOM_S2_DFP1_DPMS_STATE;
		    save->bios_3_scratch &= ~ATOM_S3_DFP1_ACTIVE;
		} else if (radeon_output->devices & ATOM_DEVICE_DFP2_SUPPORT) {
		    save->bios_2_scratch |= ATOM_S2_DFP2_DPMS_STATE;
		    save->bios_3_scratch &= ~ATOM_S3_DFP2_ACTIVE;
		} else if (radeon_output->devices & ATOM_DEVICE_DFP3_SUPPORT) {
		    save->bios_2_scratch |= ATOM_S2_DFP3_DPMS_STATE;
		    save->bios_3_scratch &= ~ATOM_S3_DFP3_ACTIVE;
		}
	    }
	}
	if (info->ChipFamily >= CHIP_FAMILY_R600) {
	    OUTREG(R600_BIOS_2_SCRATCH, save->bios_2_scratch);
	    OUTREG(R600_BIOS_3_SCRATCH, save->bios_3_scratch);
	} else {
	    OUTREG(RADEON_BIOS_2_SCRATCH, save->bios_2_scratch);
	    OUTREG(RADEON_BIOS_3_SCRATCH, save->bios_3_scratch);
	}
    } else {
	if (mode == DPMSModeOn) {
	    save->bios_6_scratch &= ~(RADEON_DPMS_MASK | RADEON_SCREEN_BLANKING);
	    save->bios_6_scratch |= RADEON_DPMS_ON;
	    if (radeon_output->MonType == MT_STV ||
		radeon_output->MonType == MT_CTV) {
		save->bios_5_scratch |= RADEON_TV1_ON;
		save->bios_6_scratch |= RADEON_TV_DPMS_ON;
	    } else if (radeon_output->MonType == MT_CRT) {
		if (radeon_output->DACType == DAC_PRIMARY)
		    save->bios_5_scratch |= RADEON_CRT1_ON;
		else
		    save->bios_5_scratch |= RADEON_CRT2_ON;
		save->bios_6_scratch |= RADEON_CRT_DPMS_ON;
	    } else if (radeon_output->MonType == MT_LCD) {
		save->bios_5_scratch |= RADEON_LCD1_ON;
		save->bios_6_scratch |= RADEON_LCD_DPMS_ON;
	    } else if (radeon_output->MonType == MT_DFP) {
		if (radeon_output->TMDSType == TMDS_INT)
		    save->bios_5_scratch |= RADEON_DFP1_ON;
		else
		    save->bios_5_scratch |= RADEON_DFP2_ON;
		save->bios_6_scratch |= RADEON_DFP_DPMS_ON;
	    }
	} else {
	    save->bios_6_scratch &= ~RADEON_DPMS_MASK;
	    save->bios_6_scratch |= (RADEON_DPMS_OFF | RADEON_SCREEN_BLANKING);
	    if (radeon_output->MonType == MT_STV ||
		radeon_output->MonType == MT_CTV) {
		save->bios_5_scratch &= ~RADEON_TV1_ON;
		save->bios_6_scratch &= ~RADEON_TV_DPMS_ON;
	    } else if (radeon_output->MonType == MT_CRT) {
		if (radeon_output->DACType == DAC_PRIMARY)
		    save->bios_5_scratch &= ~RADEON_CRT1_ON;
		else
		    save->bios_5_scratch &= ~RADEON_CRT2_ON;
		save->bios_6_scratch &= ~RADEON_CRT_DPMS_ON;
	    } else if (radeon_output->MonType == MT_LCD) {
		save->bios_5_scratch &= ~RADEON_LCD1_ON;
		save->bios_6_scratch &= ~RADEON_LCD_DPMS_ON;
	    } else if (radeon_output->MonType == MT_DFP) {
		if (radeon_output->TMDSType == TMDS_INT)
		    save->bios_5_scratch &= ~RADEON_DFP1_ON;
		else
		    save->bios_5_scratch &= ~RADEON_DFP2_ON;
		save->bios_6_scratch &= ~RADEON_DFP_DPMS_ON;
	    }
	}
	OUTREG(RADEON_BIOS_5_SCRATCH, save->bios_5_scratch);
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

    if (info->IsAtomBios) {
	if (radeon_output->MonType == MT_STV ||
	    radeon_output->MonType == MT_CTV) {
	    if (radeon_output->devices & ATOM_DEVICE_TV1_SUPPORT) {
		save->bios_3_scratch &= ~ATOM_S3_TV1_CRTC_ACTIVE;
		save->bios_3_scratch |= (radeon_crtc->crtc_id << 18);
	    }
	} else if (radeon_output->MonType == MT_CV) {
	    if (radeon_output->devices & ATOM_DEVICE_CV_SUPPORT) {
		save->bios_2_scratch &= ~ATOM_S3_CV_CRTC_ACTIVE;
		save->bios_3_scratch |= (radeon_crtc->crtc_id << 24);
	    }
	} else if (radeon_output->MonType == MT_CRT) {
	    if (radeon_output->devices & ATOM_DEVICE_CRT1_SUPPORT) {
		save->bios_2_scratch &= ~ATOM_S3_CRT1_CRTC_ACTIVE;
		save->bios_3_scratch |= (radeon_crtc->crtc_id << 16);
	    } else if (radeon_output->devices & ATOM_DEVICE_CRT2_SUPPORT) {
		save->bios_2_scratch &= ~ATOM_S3_CRT2_CRTC_ACTIVE;
		save->bios_3_scratch |= (radeon_crtc->crtc_id << 20);
	    }
	} else if (radeon_output->MonType == MT_LCD) {
	    if (radeon_output->devices & ATOM_DEVICE_LCD1_SUPPORT) {
		save->bios_2_scratch &= ~ATOM_S3_LCD1_CRTC_ACTIVE;
		save->bios_3_scratch |= (radeon_crtc->crtc_id << 17);
	    }
	} else if (radeon_output->MonType == MT_DFP) {
	    if (radeon_output->devices & ATOM_DEVICE_DFP1_SUPPORT) {
		save->bios_2_scratch &= ~ATOM_S3_DFP1_CRTC_ACTIVE;
		save->bios_3_scratch |= (radeon_crtc->crtc_id << 19);
	    } else if (radeon_output->devices & ATOM_DEVICE_DFP2_SUPPORT) {
		save->bios_2_scratch &= ~ATOM_S3_DFP2_CRTC_ACTIVE;
		save->bios_3_scratch |= (radeon_crtc->crtc_id << 23);
	    } else if (radeon_output->devices & ATOM_DEVICE_DFP3_SUPPORT) {
		save->bios_2_scratch &= ~ATOM_S3_DFP3_CRTC_ACTIVE;
		save->bios_3_scratch |= (radeon_crtc->crtc_id << 25);
	    }
	}
	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    OUTREG(R600_BIOS_3_SCRATCH, save->bios_3_scratch);
	else
	    OUTREG(RADEON_BIOS_3_SCRATCH, save->bios_3_scratch);
    } else {
	if (radeon_output->MonType == MT_STV ||
	    radeon_output->MonType == MT_CTV) {
	    save->bios_5_scratch &= ~RADEON_TV1_CRTC_MASK;
	    save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_TV1_CRTC_SHIFT);
	} else if (radeon_output->MonType == MT_CRT) {
	    if (radeon_output->DACType == DAC_PRIMARY) {
		save->bios_5_scratch &= ~RADEON_CRT1_CRTC_MASK;
		save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_CRT1_CRTC_SHIFT);
	    } else {
		save->bios_5_scratch &= ~RADEON_CRT2_CRTC_MASK;
		save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_CRT2_CRTC_SHIFT);
	    }
	} else if (radeon_output->MonType == MT_LCD) {
	    save->bios_5_scratch &= ~RADEON_LCD1_CRTC_MASK;
	    save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_LCD1_CRTC_SHIFT);
	} else if (radeon_output->MonType == MT_DFP) {
	    if (radeon_output->TMDSType == TMDS_INT) {
		save->bios_5_scratch &= ~RADEON_DFP1_CRTC_MASK;
		save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_DFP1_CRTC_SHIFT);
	    } else {
		save->bios_5_scratch &= ~RADEON_DFP2_CRTC_MASK;
		save->bios_5_scratch |= (radeon_crtc->crtc_id << RADEON_DFP2_CRTC_SHIFT);
	    }
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

    if (info->ChipFamily >= CHIP_FAMILY_R600)
	return;

    if (info->IsAtomBios) {
	if (connected) {
	    if (radeon_output->MonType == MT_STV) {
		/* taken care of by load detection */
	    } else if (radeon_output->MonType == MT_CTV) {
		/* taken care of by load detection */
	    } else if (radeon_output->MonType == MT_CV) {
		/* taken care of by load detection */
	    } else if (radeon_output->MonType == MT_CRT) {
		if (radeon_output->devices & ATOM_DEVICE_CRT1_SUPPORT)
		    save->bios_0_scratch |= ATOM_S0_CRT1_COLOR;
		else if (radeon_output->devices & ATOM_DEVICE_CRT2_SUPPORT)
		    save->bios_0_scratch |= ATOM_S0_CRT2_COLOR;
	    } else if (radeon_output->MonType == MT_LCD) {
		if (radeon_output->devices & ATOM_DEVICE_LCD1_SUPPORT)
		    save->bios_0_scratch |= ATOM_S0_LCD1;
	    } else if (radeon_output->MonType == MT_DFP) {
		if (radeon_output->devices & ATOM_DEVICE_DFP1_SUPPORT)
		    save->bios_0_scratch |= ATOM_S0_DFP1;
		else if (radeon_output->devices & ATOM_DEVICE_DFP2_SUPPORT)
		    save->bios_0_scratch |= ATOM_S0_DFP2;
		else if (radeon_output->devices & ATOM_DEVICE_DFP3_SUPPORT)
		    save->bios_0_scratch |= ATOM_S0_DFP3;
	    }
	} else {
	    if (OUTPUT_IS_TV) {
		if (radeon_output->devices & ATOM_DEVICE_TV1_SUPPORT)
		    save->bios_0_scratch &= ~ATOM_S0_TV1_MASK;
	    }
	    if (radeon_output->type == OUTPUT_CV) {
		if (radeon_output->devices & ATOM_DEVICE_CV_SUPPORT)
		    save->bios_0_scratch &= ~ATOM_S0_CV_MASK;
	    }
	    if (radeon_output->DACType) {
		if (radeon_output->devices & ATOM_DEVICE_CRT1_SUPPORT)
		    save->bios_0_scratch &= ~ATOM_S0_CRT1_MASK;
		else if (radeon_output->devices & ATOM_DEVICE_CRT2_SUPPORT)
		    save->bios_0_scratch &= ~ATOM_S0_CRT2_MASK;
	    }
	    if (radeon_output->type == OUTPUT_LVDS) {
		if (radeon_output->devices & ATOM_DEVICE_LCD1_SUPPORT)
		    save->bios_0_scratch &= ~ATOM_S0_LCD1;
	    }
	    if (radeon_output->TMDSType) {
		if (radeon_output->devices & ATOM_DEVICE_DFP1_SUPPORT)
		    save->bios_0_scratch &= ~ATOM_S0_DFP1;
		else if (radeon_output->devices & ATOM_DEVICE_DFP2_SUPPORT)
		    save->bios_0_scratch &= ~ATOM_S0_DFP2;
		else if (radeon_output->devices & ATOM_DEVICE_DFP3_SUPPORT)
		    save->bios_0_scratch &= ~ATOM_S0_DFP3;
	    }
	}
	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    OUTREG(R600_BIOS_0_SCRATCH, save->bios_0_scratch);
	else
	    OUTREG(RADEON_BIOS_0_SCRATCH, save->bios_0_scratch);
    } else {
	if (connected) {
	    if (radeon_output->MonType == MT_STV)
		save->bios_4_scratch |= RADEON_TV1_ATTACHED_SVIDEO;
	    else if (radeon_output->MonType == MT_CTV)
		save->bios_4_scratch |= RADEON_TV1_ATTACHED_COMP;
	    else if (radeon_output->MonType == MT_CRT) {
		if (radeon_output->DACType == DAC_PRIMARY)
		    save->bios_4_scratch |= RADEON_CRT1_ATTACHED_COLOR;
		else
		    save->bios_4_scratch |= RADEON_CRT2_ATTACHED_COLOR;
	    } else if (radeon_output->MonType == MT_LCD)
		save->bios_4_scratch |= RADEON_LCD1_ATTACHED;
	    else if (radeon_output->MonType == MT_DFP) {
		if (radeon_output->TMDSType == TMDS_INT)
		    save->bios_4_scratch |= RADEON_DFP1_ATTACHED;
		else
		    save->bios_4_scratch |= RADEON_DFP2_ATTACHED;
	    }
	} else {
	    if (OUTPUT_IS_TV)
		save->bios_4_scratch &= ~RADEON_TV1_ATTACHED_MASK;
	    else if (radeon_output->DACType == DAC_TVDAC)
		save->bios_4_scratch &= ~RADEON_CRT2_ATTACHED_MASK;
	    if (radeon_output->DACType == DAC_PRIMARY)
		save->bios_4_scratch &= ~RADEON_CRT1_ATTACHED_MASK;
	    if (radeon_output->type == OUTPUT_LVDS)
		save->bios_4_scratch &= ~RADEON_LCD1_ATTACHED;
	    if (radeon_output->TMDSType == TMDS_INT)
		save->bios_4_scratch &= ~RADEON_DFP1_ATTACHED;
	    if (radeon_output->TMDSType == TMDS_EXT)
		save->bios_4_scratch &= ~RADEON_DFP2_ATTACHED;
	}
	OUTREG(RADEON_BIOS_4_SCRATCH, save->bios_4_scratch);
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
    RADEONConnectorFindMonitor(output);

    /* nothing connected, light up some defaults so the server comes up */
    if (radeon_output->MonType == MT_NONE &&
	info->first_load_no_devices) {
	if (info->IsMobility) {
	    if (radeon_output->type == OUTPUT_LVDS) {
		radeon_output->MonType = MT_LCD;
		info->first_load_no_devices = FALSE;
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Using LVDS default\n");
	    }
	} else {
	    if (radeon_output->type == OUTPUT_VGA ||
		radeon_output->type == OUTPUT_DVI_I) {
		radeon_output->MonType = MT_CRT;
		info->first_load_no_devices = FALSE;
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Using VGA default\n");
	    } else if (radeon_output->type == OUTPUT_DVI_D) {
		radeon_output->MonType = MT_DFP;
		info->first_load_no_devices = FALSE;
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Using DVI default\n");
	    }
	}
    }

    radeon_bios_output_connected(output, TRUE);

    /* set montype so users can force outputs on even if detection fails */
    if (radeon_output->MonType == MT_NONE) {
	connected = FALSE;
	if (radeon_output->type == OUTPUT_LVDS)
	    radeon_output->MonType = MT_LCD;
	else if (radeon_output->type == OUTPUT_VGA)
            radeon_output->MonType = MT_CRT;
	else if (radeon_output->type == OUTPUT_STV)
            radeon_output->MonType = MT_STV;
	else if (radeon_output->type == OUTPUT_CTV)
            radeon_output->MonType = MT_CTV;
	else if (radeon_output->type == OUTPUT_CV)
            radeon_output->MonType = MT_CV;
	else if (radeon_output->type == OUTPUT_DVI_D)
	    radeon_output->MonType = MT_DFP;
	else if (radeon_output->type == OUTPUT_HDMI)
	    radeon_output->MonType = MT_DFP;
	else if (radeon_output->type == OUTPUT_DVI_A)
	    radeon_output->MonType = MT_CRT;
	else if (radeon_output->type == OUTPUT_DVI_I) {
	    if (radeon_output->DVIType == DVI_ANALOG)
		radeon_output->MonType = MT_CRT;
	    else if (radeon_output->DVIType == DVI_DIGITAL)
		radeon_output->MonType = MT_DFP;
	}
    }

    if (radeon_output->MonType == MT_UNKNOWN) {
        output->subpixel_order = SubPixelUnknown;
	return XF86OutputStatusUnknown;
    } else {

      switch(radeon_output->MonType) {
      case MT_LCD:
      case MT_DFP:
	  output->subpixel_order = SubPixelHorizontalRGB;
	  break;
      default:
	  output->subpixel_order = SubPixelNone;
	  break;
      }

      if (connected)
	  return XF86OutputStatusConnected;
      else
	  return XF86OutputStatusDisconnected;
    }

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
        xfree(output->driver_private);
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

    if (radeon_output->DACType == DAC_PRIMARY ||
	radeon_output->DACType == DAC_TVDAC) {
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
	    data = 1; /* user forces on tv dac load detection */
	else
	    data = 0; /* shared tvdac between vga/dvi/tv */

	err = RRChangeOutputProperty(output->randr_output, load_detection_atom,
				     XA_INTEGER, 32, PropModeReplace, 1, &data,
				     FALSE, TRUE);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
	}
    }

    if (IS_DCE3_VARIANT &&
	(OUTPUT_IS_DVI || (radeon_output->type == OUTPUT_HDMI))) {
	coherent_mode_atom = MAKE_ATOM("coherent_mode");

	range[0] = 0; /* off */
	range[1] = 1; /* on */
	err = RRConfigureOutputProperty(output->randr_output, coherent_mode_atom,
					FALSE, TRUE, FALSE, 2, range);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", err);
	}

	data = 1; /* use coherent mode by default */

	err = RRChangeOutputProperty(output->randr_output, coherent_mode_atom,
				     XA_INTEGER, 32, PropModeReplace, 1, &data,
				     FALSE, TRUE);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
	}
    }

    if (OUTPUT_IS_DVI && radeon_output->TMDSType == TMDS_INT) {
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
    if (radeon_output->type == OUTPUT_LVDS || OUTPUT_IS_DVI) {
	rmx_atom = MAKE_ATOM("scaler");

	err = RRConfigureOutputProperty(output->randr_output, rmx_atom,
					FALSE, FALSE, FALSE, 0, NULL);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", err);
	}
	/* Set the current value of the property */
	if (radeon_output->type == OUTPUT_LVDS)
	    s = "full";
	else
	    s = "off";
	err = RRChangeOutputProperty(output->randr_output, rmx_atom,
				     XA_STRING, 8, PropModeReplace, strlen(s), (pointer)s,
				     FALSE, FALSE);
	if (err != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", err);
	}
    }

    /* force auto/analog/digital for DVI-I ports */
    if (radeon_output->type == OUTPUT_DVI_I) {
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

    if (OUTPUT_IS_TV) {
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
	switch (radeon_output->tvStd) {
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
	    if (!xf86CrtcSetMode(crtc, &crtc->desiredMode, crtc->desiredRotation,
				 crtc->desiredX, crtc->desiredY)) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Failed to set mode after propery change!\n");
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
	const char *s;
	if (value->type != XA_STRING || value->format != 8)
	    return FALSE;
	s = (char*)value->data;
	if (value->size == strlen("bios") && !strncmp("bios", s, strlen("bios"))) {
	    if (!RADEONGetTMDSInfoFromBIOS(output))
		RADEONGetTMDSInfoFromTable(output);
	} else if (value->size == strlen("driver") && !strncmp("driver", s, strlen("driver"))) {
	    RADEONGetTMDSInfoFromTable(output);
	} else
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
	if (value->type != XA_INTEGER ||
	    value->format != 32 ||
	    value->size != 1) {
	    return FALSE;
	}

	val = *(INT32 *)value->data;
	if (val < -MAX_H_SIZE || val > MAX_H_SIZE)
	    return FALSE;

	radeon_output->hSize = val;
	if (radeon_output->tv_on && !IS_AVIVO_VARIANT)
	    RADEONUpdateHVPosition(output, &output->crtc->mode);

    } else if (property == tv_hpos_atom) {
	if (value->type != XA_INTEGER ||
	    value->format != 32 ||
	    value->size != 1) {
	    return FALSE;
	}

	val = *(INT32 *)value->data;
	if (val < -MAX_H_POSITION || val > MAX_H_POSITION)
	    return FALSE;

	radeon_output->hPos = val;
	if (radeon_output->tv_on && !IS_AVIVO_VARIANT)
	    RADEONUpdateHVPosition(output, &output->crtc->mode);

    } else if (property == tv_vpos_atom) {
	if (value->type != XA_INTEGER ||
	    value->format != 32 ||
	    value->size != 1) {
	    return FALSE;
	}

	val = *(INT32 *)value->data;
	if (val < -MAX_H_POSITION || val > MAX_H_POSITION)
	    return FALSE;

	radeon_output->vPos = val;
	if (radeon_output->tv_on && !IS_AVIVO_VARIANT)
	    RADEONUpdateHVPosition(output, &output->crtc->mode);

    } else if (property == tv_std_atom) {
	const char *s;
	TVStd std = radeon_output->tvStd;

	if (value->type != XA_STRING || value->format != 8)
	    return FALSE;
	s = (char*)value->data;
	if (value->size == strlen("ntsc") && !strncmp("ntsc", s, strlen("ntsc"))) {
	    radeon_output->tvStd = TV_STD_NTSC;
	} else if (value->size == strlen("pal") && !strncmp("pal", s, strlen("pal"))) {
	    radeon_output->tvStd = TV_STD_PAL;
	} else if (value->size == strlen("pal-m") && !strncmp("pal-m", s, strlen("pal-m"))) {
	    radeon_output->tvStd = TV_STD_PAL_M;
	} else if (value->size == strlen("pal-60") && !strncmp("pal-60", s, strlen("pal-60"))) {
	    radeon_output->tvStd = TV_STD_PAL_60;
	} else if (value->size == strlen("ntsc-j") && !strncmp("ntsc-j", s, strlen("ntsc-j"))) {
	    radeon_output->tvStd = TV_STD_NTSC_J;
	} else if (value->size == strlen("scart-pal") && !strncmp("scart-pal", s, strlen("scart-pal"))) {
	    radeon_output->tvStd = TV_STD_SCART_PAL;
	} else if (value->size == strlen("pal-cn") && !strncmp("pal-cn", s, strlen("pal-cn"))) {
	    radeon_output->tvStd = TV_STD_PAL_CN;
	} else if (value->size == strlen("secam") && !strncmp("secam", s, strlen("secam"))) {
	    radeon_output->tvStd = TV_STD_SECAM;
	} else
	    return FALSE;

	if (!radeon_set_mode_for_property(output)) {
	    radeon_output->tvStd = std;
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

void RADEONSetOutputType(ScrnInfoPtr pScrn, RADEONOutputPrivatePtr radeon_output)
{
    RADEONOutputType output = OUTPUT_NONE;

    switch(radeon_output->ConnectorType) {
    case CONNECTOR_VGA:
	output = OUTPUT_VGA; break;
    case CONNECTOR_DVI_I:
	output = OUTPUT_DVI_I; break;
    case CONNECTOR_DVI_D:
	output = OUTPUT_DVI_D; break;
    case CONNECTOR_DVI_A:
	output = OUTPUT_DVI_A; break;
    case CONNECTOR_DIN:
	if (radeon_output->devices & ATOM_DEVICE_CV_SUPPORT)
	    output = OUTPUT_CV;
	else if (radeon_output->devices & ATOM_DEVICE_TV1_SUPPORT)
	    output = OUTPUT_STV;
	break;
    case CONNECTOR_STV:
	output = OUTPUT_STV; break;
    case CONNECTOR_CTV:
	output = OUTPUT_CTV; break;
    case CONNECTOR_LVDS:
	output = OUTPUT_LVDS; break;
    case CONNECTOR_HDMI_TYPE_A:
    case CONNECTOR_HDMI_TYPE_B:
	output = OUTPUT_HDMI; break;
    case CONNECTOR_DISPLAY_PORT:
	output = OUTPUT_DP; break;
    case CONNECTOR_DIGITAL:
    case CONNECTOR_NONE:
    case CONNECTOR_UNSUPPORTED:
    default:
	output = OUTPUT_NONE; break;
    }
    radeon_output->type = output;
}

#if 0
static
Bool AVIVOI2CReset(ScrnInfoPtr pScrn)
{
  RADEONInfoPtr info = RADEONPTR(pScrn);
  unsigned char *RADEONMMIO = info->MMIO;

  OUTREG(AVIVO_I2C_STOP, 1);
  INREG(AVIVO_I2C_STOP);
  OUTREG(AVIVO_I2C_STOP, 0x0);
  return TRUE;
}
#endif

static
Bool AVIVOI2CDoLock(xf86OutputPtr output, int lock_state)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONI2CBusPtr pRADEONI2CBus = radeon_output->pI2CBus->DriverPrivate.ptr;
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t temp;

    temp = INREG(pRADEONI2CBus->mask_clk_reg);
    if (lock_state == AVIVO_I2C_ENABLE)
	temp |= (pRADEONI2CBus->put_clk_mask);
    else
	temp &= ~(pRADEONI2CBus->put_clk_mask);
    OUTREG(pRADEONI2CBus->mask_clk_reg, temp);
    temp = INREG(pRADEONI2CBus->mask_clk_reg);

    temp = INREG(pRADEONI2CBus->mask_data_reg);
    if (lock_state == AVIVO_I2C_ENABLE)
	temp |= (pRADEONI2CBus->put_data_mask);
    else
	temp &= ~(pRADEONI2CBus->put_data_mask);
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

static Bool
RADEONI2CInit(xf86OutputPtr output, I2CBusPtr *bus_ptr, char *name, Bool dvo)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    I2CBusPtr pI2CBus;
    RADEONI2CBusPtr pRADEONI2CBus;

    pI2CBus = xf86CreateI2CBusRec();
    if (!pI2CBus) return FALSE;

    pI2CBus->BusName    = name;
    pI2CBus->scrnIndex  = pScrn->scrnIndex;
    pI2CBus->I2CPutBits = RADEONI2CPutBits;
    pI2CBus->I2CGetBits = RADEONI2CGetBits;
    pI2CBus->AcknTimeout = 5;

    if (dvo) {
	pRADEONI2CBus = &(radeon_output->dvo_i2c);
    } else {
	pRADEONI2CBus = &(radeon_output->ddc_i2c);
    }

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

    i2c.mask_clk_mask = RADEON_GPIO_EN_1 | RADEON_GPIO_Y_1;
    i2c.mask_data_mask =  RADEON_GPIO_EN_0 | RADEON_GPIO_Y_0;
    i2c.put_clk_mask = RADEON_GPIO_EN_1;
    i2c.put_data_mask = RADEON_GPIO_EN_0;
    i2c.get_clk_mask = RADEON_GPIO_Y_1;
    i2c.get_data_mask = RADEON_GPIO_Y_0;
    if ((ddc_line == RADEON_LCD_GPIO_MASK) ||
	(ddc_line == RADEON_MDGPIO_EN_REG)) {
	i2c.mask_clk_reg = ddc_line;
	i2c.mask_data_reg = ddc_line;
	i2c.put_clk_reg = ddc_line;
	i2c.put_data_reg = ddc_line;
	i2c.get_clk_reg = ddc_line + 4;
	i2c.get_data_reg = ddc_line + 4;
    } else {
	i2c.mask_clk_reg = ddc_line;
	i2c.mask_data_reg = ddc_line;
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

    if (ddc_line == AVIVO_GPIO_0) {
	i2c.put_clk_mask = (1 << 19);
	i2c.put_data_mask = (1 << 18);
	i2c.get_clk_mask = (1 << 19);
	i2c.get_data_mask = (1 << 18);
	i2c.mask_clk_mask = (1 << 19);
	i2c.mask_data_mask = (1 << 18);
    } else {
	i2c.put_clk_mask = (1 << 0);
	i2c.put_data_mask = (1 << 8);
	i2c.get_clk_mask = (1 << 0);
	i2c.get_data_mask = (1 << 8);
	i2c.mask_clk_mask = (1 << 0);
	i2c.mask_data_mask = (1 << 8);
    }
    i2c.mask_clk_reg = ddc_line;
    i2c.mask_data_reg = ddc_line;
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
RADEONGetPanelInfoFromReg (xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t fp_vert_stretch = INREG(RADEON_FP_VERT_STRETCH);
    uint32_t fp_horz_stretch = INREG(RADEON_FP_HORZ_STRETCH);

    radeon_output->PanelPwrDly = 200;
    if (fp_vert_stretch & RADEON_VERT_STRETCH_ENABLE) {
	radeon_output->PanelYRes = ((fp_vert_stretch & RADEON_VERT_PANEL_SIZE) >>
				    RADEON_VERT_PANEL_SHIFT) + 1;
    } else {
	radeon_output->PanelYRes = (INREG(RADEON_CRTC_V_TOTAL_DISP)>>16) + 1;
    }
    if (fp_horz_stretch & RADEON_HORZ_STRETCH_ENABLE) {
	radeon_output->PanelXRes = (((fp_horz_stretch & RADEON_HORZ_PANEL_SIZE) >>
				     RADEON_HORZ_PANEL_SHIFT) + 1) * 8;
    } else {
	radeon_output->PanelXRes = ((INREG(RADEON_CRTC_H_TOTAL_DISP)>>16) + 1) * 8;
    }
    
    if ((radeon_output->PanelXRes < 640) || (radeon_output->PanelYRes < 480)) {
	radeon_output->PanelXRes = 640;
	radeon_output->PanelYRes = 480;
    }

    // move this to crtc function
    if (xf86ReturnOptValBool(info->Options, OPTION_LVDS_PROBE_PLL, TRUE)) {
           uint32_t ppll_div_sel, ppll_val;

           ppll_div_sel = INREG8(RADEON_CLOCK_CNTL_INDEX + 1) & 0x3;
	   RADEONPllErrataAfterIndex(info);
	   ppll_val = INPLL(pScrn, RADEON_PPLL_DIV_0 + ppll_div_sel);
           if ((ppll_val & 0x000707ff) == 0x1bb)
		   goto noprobe;
	   info->FeedbackDivider = ppll_val & 0x7ff;
	   info->PostDivider = (ppll_val >> 16) & 0x7;
	   info->RefDivider = info->pll.reference_div;
	   info->UseBiosDividers = TRUE;

           xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                      "Existing panel PLL dividers will be used.\n");
    }
 noprobe:

    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
	       "Panel size %dx%d is derived, this may not be correct.\n"
		   "If not, use PanelSize option to overwrite this setting\n",
	       radeon_output->PanelXRes, radeon_output->PanelYRes);
}

/* BIOS may not have right panel size, we search through all supported
 * DDC modes looking for the maximum panel size.
 */
static void
RADEONUpdatePanelSize(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int             j;
    /* XXX: fixme */
    //xf86MonPtr      ddc  = pScrn->monitor->DDC;
    xf86MonPtr ddc = output->MonInfo;
    DisplayModePtr  p;

    // crtc should handle?
    if ((info->UseBiosDividers && radeon_output->DotClock != 0) || (ddc == NULL))
       return;

    /* Go thru detailed timing table first */
    for (j = 0; j < 4; j++) {
	if (ddc->det_mon[j].type == 0) {
	    struct detailed_timings *d_timings =
		&ddc->det_mon[j].section.d_timings;
           int match = 0;

           /* If we didn't get a panel clock or guessed one, try to match the
            * mode with the panel size. We do that because we _need_ a panel
            * clock, or ValidateFPModes will fail, even when UseBiosDividers
            * is set.
            */
           if (radeon_output->DotClock == 0 &&
               radeon_output->PanelXRes == d_timings->h_active &&
               radeon_output->PanelYRes == d_timings->v_active)
               match = 1;

           /* If we don't have a BIOS provided panel data with fixed dividers,
            * check for a larger panel size
            */
	    if (radeon_output->PanelXRes < d_timings->h_active &&
               radeon_output->PanelYRes < d_timings->v_active &&
               !info->UseBiosDividers)
               match = 1;

             if (match) {
		radeon_output->PanelXRes  = d_timings->h_active;
		radeon_output->PanelYRes  = d_timings->v_active;
		radeon_output->DotClock   = d_timings->clock / 1000;
		radeon_output->HOverPlus  = d_timings->h_sync_off;
		radeon_output->HSyncWidth = d_timings->h_sync_width;
		radeon_output->HBlank     = d_timings->h_blanking;
		radeon_output->VOverPlus  = d_timings->v_sync_off;
		radeon_output->VSyncWidth = d_timings->v_sync_width;
		radeon_output->VBlank     = d_timings->v_blanking;
                radeon_output->Flags      = (d_timings->interlaced ? V_INTERLACE : 0);
                switch (d_timings->misc) {
                case 0: radeon_output->Flags |= V_NHSYNC | V_NVSYNC; break;
                case 1: radeon_output->Flags |= V_PHSYNC | V_NVSYNC; break;
                case 2: radeon_output->Flags |= V_NHSYNC | V_PVSYNC; break;
                case 3: radeon_output->Flags |= V_PHSYNC | V_PVSYNC; break;
                }
                xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel infos found from DDC detailed: %dx%d\n",
                           radeon_output->PanelXRes, radeon_output->PanelYRes);
	    }
	}
    }

    if (info->UseBiosDividers && radeon_output->DotClock != 0)
       return;

    /* Search thru standard VESA modes from EDID */
    for (j = 0; j < 8; j++) {
	if ((radeon_output->PanelXRes < ddc->timings2[j].hsize) &&
	    (radeon_output->PanelYRes < ddc->timings2[j].vsize)) {
	    for (p = pScrn->monitor->Modes; p; p = p->next) {
		if ((ddc->timings2[j].hsize == p->HDisplay) &&
		    (ddc->timings2[j].vsize == p->VDisplay)) {
		    float  refresh =
			(float)p->Clock * 1000.0 / p->HTotal / p->VTotal;

		    if (abs((float)ddc->timings2[j].refresh - refresh) < 1.0) {
			/* Is this good enough? */
			radeon_output->PanelXRes  = ddc->timings2[j].hsize;
			radeon_output->PanelYRes  = ddc->timings2[j].vsize;
			radeon_output->HBlank     = p->HTotal - p->HDisplay;
			radeon_output->HOverPlus  = p->HSyncStart - p->HDisplay;
			radeon_output->HSyncWidth = p->HSyncEnd - p->HSyncStart;
			radeon_output->VBlank     = p->VTotal - p->VDisplay;
			radeon_output->VOverPlus  = p->VSyncStart - p->VDisplay;
			radeon_output->VSyncWidth = p->VSyncEnd - p->VSyncStart;
			radeon_output->DotClock   = p->Clock;
                        radeon_output->Flags      = p->Flags;
                        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel infos found from DDC VESA/EDID: %dx%d\n",
                                   radeon_output->PanelXRes, radeon_output->PanelYRes);
		    }
		}
	    }
	}
    }
}

static Bool
RADEONGetLVDSInfo (xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    char* s;

    if (!RADEONGetLVDSInfoFromBIOS(output))
	RADEONGetPanelInfoFromReg(output);

    if ((s = xf86GetOptValString(info->Options, OPTION_PANEL_SIZE))) {
	radeon_output->PanelPwrDly = 200;
	if (sscanf (s, "%dx%d", &radeon_output->PanelXRes, &radeon_output->PanelYRes) != 2) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Invalid PanelSize option: %s\n", s);
	    RADEONGetPanelInfoFromReg(output);
	}
    }

    /* The panel size we collected from BIOS may not be the
     * maximum size supported by the panel.  If not, we update
     * it now.  These will be used if no matching mode can be
     * found from EDID data.
     */
    RADEONUpdatePanelSize(output);

    if (radeon_output->DotClock == 0) {
	DisplayModePtr  tmp_mode = NULL;
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "No valid timing info from BIOS.\n");
	/* No timing information for the native mode,
	   use whatever specified in the Modeline.
	   If no Modeline specified, we'll just pick
	   the VESA mode at 60Hz refresh rate which
	   is likely to be the best for a flat panel.
	*/
	tmp_mode = pScrn->monitor->Modes;
	while(tmp_mode) {
	    if ((tmp_mode->HDisplay == radeon_output->PanelXRes) &&
		(tmp_mode->VDisplay == radeon_output->PanelYRes)) {
		    
		float  refresh =
		    (float)tmp_mode->Clock * 1000.0 / tmp_mode->HTotal / tmp_mode->VTotal;
		if ((abs(60.0 - refresh) < 1.0) ||
		    (tmp_mode->type == 0)) {
		    radeon_output->HBlank     = tmp_mode->HTotal - tmp_mode->HDisplay;
		    radeon_output->HOverPlus  = tmp_mode->HSyncStart - tmp_mode->HDisplay;
		    radeon_output->HSyncWidth = tmp_mode->HSyncEnd - tmp_mode->HSyncStart;
		    radeon_output->VBlank     = tmp_mode->VTotal - tmp_mode->VDisplay;
		    radeon_output->VOverPlus  = tmp_mode->VSyncStart - tmp_mode->VDisplay;
		    radeon_output->VSyncWidth = tmp_mode->VSyncEnd - tmp_mode->VSyncStart;
		    radeon_output->DotClock   = tmp_mode->Clock;
		    radeon_output->Flags = 0;
		    break;
		}
	    }

	    tmp_mode = tmp_mode->next;

	    if (tmp_mode == pScrn->monitor->Modes)
		break;
	}
	if ((radeon_output->DotClock == 0) && !output->MonInfo) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Panel size is not correctly detected.\n"
		       "Please try to use PanelSize option for correct settings.\n");
	    return FALSE;
	}
    }

    return TRUE;
}

static void
RADEONGetTMDSInfoFromTable(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int i;

    for (i=0; i<4; i++) {
        radeon_output->tmds_pll[i].value = default_tmds_pll[info->ChipFamily][i].value;
        radeon_output->tmds_pll[i].freq = default_tmds_pll[info->ChipFamily][i].freq;
    }
}

static void
RADEONGetTMDSInfo(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int i;

    for (i=0; i<4; i++) {
        radeon_output->tmds_pll[i].value = 0;
        radeon_output->tmds_pll[i].freq = 0;
    }

    if (!RADEONGetTMDSInfoFromBIOS(output))
	RADEONGetTMDSInfoFromTable(output);

}

static void
RADEONGetTVInfo(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    char *optstr;

    radeon_output->hPos = 0;
    radeon_output->vPos = 0;
    radeon_output->hSize = 0;

    if (!RADEONGetTVInfoFromBIOS(output)) {
	/* set some reasonable defaults */
	radeon_output->default_tvStd = TV_STD_NTSC;
	radeon_output->tvStd = TV_STD_NTSC;
	radeon_output->TVRefClk = 27.000000000;
	radeon_output->SupportedTVStds = TV_STD_NTSC | TV_STD_PAL;
    }

    optstr = (char *)xf86GetOptValString(info->Options, OPTION_TVSTD);
    if (optstr) {
	if (!strncmp("ntsc", optstr, strlen("ntsc")))
	    radeon_output->tvStd = TV_STD_NTSC;
	else if (!strncmp("pal", optstr, strlen("pal")))
	    radeon_output->tvStd = TV_STD_PAL;
	else if (!strncmp("pal-m", optstr, strlen("pal-m")))
	    radeon_output->tvStd = TV_STD_PAL_M;
	else if (!strncmp("pal-60", optstr, strlen("pal-60")))
	    radeon_output->tvStd = TV_STD_PAL_60;
	else if (!strncmp("ntsc-j", optstr, strlen("ntsc-j")))
	    radeon_output->tvStd = TV_STD_NTSC_J;
	else if (!strncmp("scart-pal", optstr, strlen("scart-pal")))
	    radeon_output->tvStd = TV_STD_SCART_PAL;
	else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Invalid TV Standard: %s\n", optstr);
	}
    }

}

static void
RADEONGetTVDacAdjInfo(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if (!RADEONGetDAC2InfoFromBIOS(output)) {
	radeon_output->ps2_tvdac_adj = default_tvdac_adj[info->ChipFamily];
	if (info->IsMobility) { /* some mobility chips may different */
	    if (info->ChipFamily == CHIP_FAMILY_RV250)
		radeon_output->ps2_tvdac_adj = 0x00880000;
	}
	radeon_output->pal_tvdac_adj = radeon_output->ps2_tvdac_adj;
	radeon_output->ntsc_tvdac_adj = radeon_output->ps2_tvdac_adj;
    }

}

void RADEONInitConnector(xf86OutputPtr output)
{
    ScrnInfoPtr	    pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if (info->IsAtomBios &&
	((radeon_output->DACType == DAC_PRIMARY) ||
	 (radeon_output->DACType == DAC_TVDAC)))
	radeon_output->load_detection = 1;
    else if (radeon_output->DACType == DAC_PRIMARY)
	radeon_output->load_detection = 1; /* primary dac, only drives vga */
    else if ((radeon_output->DACType == DAC_TVDAC) &&
	     (xf86ReturnOptValBool(info->Options, OPTION_TVDAC_LOAD_DETECT, FALSE)))
	radeon_output->load_detection = 1; /* shared tvdac between vga/dvi/tv */
    else
	radeon_output->load_detection = 0;

    if (radeon_output->type == OUTPUT_LVDS) {
	radeon_output->rmx_type = RMX_FULL;
	RADEONGetLVDSInfo(output);
    }

    if (OUTPUT_IS_DVI) {
	I2CBusPtr pDVOBus;
	radeon_output->rmx_type = RMX_OFF;
	if ((!info->IsAtomBios) && radeon_output->TMDSType == TMDS_EXT) {
#if defined(__powerpc__)
	    radeon_output->dvo_i2c = legacy_setup_i2c_bus(RADEON_GPIO_MONID);
	    radeon_output->dvo_i2c_slave_addr = 0x70;
#else
	    if (!RADEONGetExtTMDSInfoFromBIOS(output)) {
		radeon_output->dvo_i2c = legacy_setup_i2c_bus(RADEON_GPIO_CRT2_DDC);
		radeon_output->dvo_i2c_slave_addr = 0x70;
	    }
#endif
	    if (RADEONI2CInit(output, &pDVOBus, "DVO", TRUE)) {
		radeon_output->DVOChip =
		    RADEONDVODeviceInit(pDVOBus,
					radeon_output->dvo_i2c_slave_addr);
		if (!radeon_output->DVOChip)
		    xfree(pDVOBus);
	    }
	} else
	    RADEONGetTMDSInfo(output);
    }

    if (OUTPUT_IS_TV)
	RADEONGetTVInfo(output);

    if (radeon_output->DACType == DAC_TVDAC) {
	radeon_output->tv_on = FALSE;
	RADEONGetTVDacAdjInfo(output);
    }

    if (OUTPUT_IS_DVI || (radeon_output->type == OUTPUT_HDMI))
	radeon_output->coherent_mode = TRUE;

    if (radeon_output->ddc_i2c.valid)
	RADEONI2CInit(output, &radeon_output->pI2CBus, output->name, FALSE);

}

#if defined(__powerpc__)
static Bool RADEONSetupAppleConnectors(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);


    switch (info->MacModel) {
    case RADEON_MAC_IBOOK:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	info->BiosConnector[0].DACType = DAC_NONE;
	info->BiosConnector[0].TMDSType = TMDS_NONE;
	info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
	info->BiosConnector[0].valid = TRUE;

	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[1].DACType = DAC_TVDAC;
	info->BiosConnector[1].TMDSType = TMDS_NONE;
	info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
	info->BiosConnector[1].valid = TRUE;

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].DACType = DAC_TVDAC;
	info->BiosConnector[2].TMDSType = TMDS_NONE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
	return TRUE;
    case RADEON_MAC_POWERBOOK_EXTERNAL:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	info->BiosConnector[0].DACType = DAC_NONE;
	info->BiosConnector[0].TMDSType = TMDS_NONE;
	info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
	info->BiosConnector[0].valid = TRUE;

	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[1].DACType = DAC_PRIMARY;
	info->BiosConnector[1].TMDSType = TMDS_EXT;
	info->BiosConnector[1].ConnectorType = CONNECTOR_DVI_I;
	info->BiosConnector[1].valid = TRUE;

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].DACType = DAC_TVDAC;
	info->BiosConnector[2].TMDSType = TMDS_NONE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
	return TRUE;

    case RADEON_MAC_POWERBOOK_INTERNAL:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	info->BiosConnector[0].DACType = DAC_NONE;
	info->BiosConnector[0].TMDSType = TMDS_NONE;
	info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
	info->BiosConnector[0].valid = TRUE;

	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[1].DACType = DAC_PRIMARY;
	info->BiosConnector[1].TMDSType = TMDS_INT;
	info->BiosConnector[1].ConnectorType = CONNECTOR_DVI_I;
	info->BiosConnector[1].valid = TRUE;

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].DACType = DAC_TVDAC;
	info->BiosConnector[2].TMDSType = TMDS_NONE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
	return TRUE;
    case RADEON_MAC_POWERBOOK_VGA:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	info->BiosConnector[0].DACType = DAC_NONE;
	info->BiosConnector[0].TMDSType = TMDS_NONE;
	info->BiosConnector[0].ConnectorType = CONNECTOR_VGA;
	info->BiosConnector[0].valid = TRUE;

	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[1].DACType = DAC_PRIMARY;
	info->BiosConnector[1].TMDSType = TMDS_INT;
	info->BiosConnector[1].ConnectorType = CONNECTOR_DVI_I;
	info->BiosConnector[1].valid = TRUE;

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].DACType = DAC_TVDAC;
	info->BiosConnector[2].TMDSType = TMDS_NONE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
	return TRUE;
    case RADEON_MAC_MINI_EXTERNAL:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_CRT2_DDC);
	info->BiosConnector[0].DACType = DAC_TVDAC;
	info->BiosConnector[0].TMDSType = TMDS_EXT;
	info->BiosConnector[0].ConnectorType = CONNECTOR_DVI_I;
	info->BiosConnector[0].valid = TRUE;

	info->BiosConnector[1].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[1].DACType = DAC_TVDAC;
	info->BiosConnector[1].TMDSType = TMDS_NONE;
	info->BiosConnector[1].ddc_i2c.valid = FALSE;
	info->BiosConnector[1].valid = TRUE;
	return TRUE;
    case RADEON_MAC_MINI_INTERNAL:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_CRT2_DDC);
	info->BiosConnector[0].DACType = DAC_TVDAC;
	info->BiosConnector[0].TMDSType = TMDS_INT;
	info->BiosConnector[0].ConnectorType = CONNECTOR_DVI_I;
	info->BiosConnector[0].valid = TRUE;

	info->BiosConnector[1].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[1].DACType = DAC_TVDAC;
	info->BiosConnector[1].TMDSType = TMDS_NONE;
	info->BiosConnector[1].ddc_i2c.valid = FALSE;
	info->BiosConnector[1].valid = TRUE;
	return TRUE;
    case RADEON_MAC_IMAC_G5_ISIGHT:
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_MONID);
	info->BiosConnector[0].DACType = DAC_NONE;
	info->BiosConnector[0].TMDSType = TMDS_INT;
	info->BiosConnector[0].ConnectorType = CONNECTOR_DVI_D;
	info->BiosConnector[0].valid = TRUE;

	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	info->BiosConnector[1].DACType = DAC_TVDAC;
	info->BiosConnector[1].TMDSType = TMDS_NONE;
	info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
	info->BiosConnector[1].valid = TRUE;

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].DACType = DAC_TVDAC;
	info->BiosConnector[2].TMDSType = TMDS_NONE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].valid = TRUE;
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

    if (!pRADEONEnt->HasCRTC2) {
	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	info->BiosConnector[0].DACType = DAC_PRIMARY;
	info->BiosConnector[0].TMDSType = TMDS_NONE;
	info->BiosConnector[0].ConnectorType = CONNECTOR_VGA;
	info->BiosConnector[0].valid = TRUE;
	return;
    }

    if (IS_AVIVO_VARIANT) {
	if (info->IsMobility) {
	    info->BiosConnector[0].ddc_i2c = atom_setup_i2c_bus(0x7e60);
	    info->BiosConnector[0].DACType = DAC_NONE;
	    info->BiosConnector[0].TMDSType = TMDS_NONE;
	    info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
	    info->BiosConnector[0].devices = ATOM_DEVICE_LCD1_SUPPORT;
	    info->BiosConnector[0].valid = TRUE;

	    info->BiosConnector[1].ddc_i2c = atom_setup_i2c_bus(0x7e40);
	    info->BiosConnector[1].DACType = DAC_PRIMARY;
	    info->BiosConnector[1].TMDSType = TMDS_NONE;
	    info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
	    info->BiosConnector[1].devices = ATOM_DEVICE_CRT1_SUPPORT;
	    info->BiosConnector[1].valid = TRUE;
	} else {
	    info->BiosConnector[0].ddc_i2c = atom_setup_i2c_bus(0x7e50);
	    info->BiosConnector[0].DACType = DAC_TVDAC;
	    info->BiosConnector[0].TMDSType = TMDS_INT;
	    info->BiosConnector[0].ConnectorType = CONNECTOR_DVI_I;
	    info->BiosConnector[0].devices = ATOM_DEVICE_CRT2_SUPPORT | ATOM_DEVICE_DFP1_SUPPORT;
	    info->BiosConnector[0].valid = TRUE;

	    info->BiosConnector[1].ddc_i2c = atom_setup_i2c_bus(0x7e40);
	    info->BiosConnector[1].DACType = DAC_PRIMARY;
	    info->BiosConnector[1].TMDSType = TMDS_NONE;
	    info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
	    info->BiosConnector[1].devices = ATOM_DEVICE_CRT1_SUPPORT;
	    info->BiosConnector[1].valid = TRUE;
	}

	info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	info->BiosConnector[2].DACType = DAC_TVDAC;
	info->BiosConnector[2].TMDSType = TMDS_NONE;
	info->BiosConnector[2].ddc_i2c.valid = FALSE;
	info->BiosConnector[2].devices = ATOM_DEVICE_TV1_SUPPORT;
	info->BiosConnector[2].valid = TRUE;
    } else {
	if (info->IsMobility) {
	    /* Below is the most common setting, but may not be true */
	    if (info->IsIGP) {
		info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_LCD_GPIO_MASK);
		info->BiosConnector[0].DACType = DAC_NONE;
		info->BiosConnector[0].TMDSType = TMDS_NONE;
		info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
		info->BiosConnector[0].valid = TRUE;

		/* IGP only has TVDAC */
		if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
		    (info->ChipFamily == CHIP_FAMILY_RS480))
		    info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_CRT2_DDC);
		else
		    info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
		info->BiosConnector[1].DACType = DAC_TVDAC;
		info->BiosConnector[1].TMDSType = TMDS_NONE;
		info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
		info->BiosConnector[1].valid = TRUE;
	    } else {
#if defined(__powerpc__)
		info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
#else
		info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_LCD_GPIO_MASK);
#endif
		info->BiosConnector[0].DACType = DAC_NONE;
		info->BiosConnector[0].TMDSType = TMDS_NONE;
		info->BiosConnector[0].ConnectorType = CONNECTOR_LVDS;
		info->BiosConnector[0].valid = TRUE;

		info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
		info->BiosConnector[1].DACType = DAC_PRIMARY;
		info->BiosConnector[1].TMDSType = TMDS_NONE;
		info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
		info->BiosConnector[1].valid = TRUE;
	    }
	} else {
	    /* Below is the most common setting, but may not be true */
	    if (info->IsIGP) {
		if ((info->ChipFamily == CHIP_FAMILY_RS400) ||
		    (info->ChipFamily == CHIP_FAMILY_RS480))
		    info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_CRT2_DDC);
		else
		    info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
		info->BiosConnector[0].DACType = DAC_TVDAC;
		info->BiosConnector[0].TMDSType = TMDS_NONE;
		info->BiosConnector[0].ConnectorType = CONNECTOR_VGA;
		info->BiosConnector[0].valid = TRUE;

		/* not sure what a good default DDCType for DVI on
		 * IGP desktop chips is
		 */
		info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_MONID); /* DDC_DVI? */
		info->BiosConnector[1].DACType = DAC_NONE;
		info->BiosConnector[1].TMDSType = TMDS_EXT;
		info->BiosConnector[1].ConnectorType = CONNECTOR_DVI_D;
		info->BiosConnector[1].valid = TRUE;
	    } else {
		info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
		info->BiosConnector[0].DACType = DAC_TVDAC;
		info->BiosConnector[0].TMDSType = TMDS_INT;
		info->BiosConnector[0].ConnectorType = CONNECTOR_DVI_I;
		info->BiosConnector[0].valid = TRUE;

#if defined(__powerpc__)
		info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
		info->BiosConnector[1].DACType = DAC_PRIMARY;
		info->BiosConnector[1].TMDSType = TMDS_EXT;
		info->BiosConnector[1].ConnectorType = CONNECTOR_DVI_I;
		info->BiosConnector[1].valid = TRUE;
#else
		info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
		info->BiosConnector[1].DACType = DAC_PRIMARY;
		info->BiosConnector[1].TMDSType = TMDS_EXT;
		info->BiosConnector[1].ConnectorType = CONNECTOR_VGA;
		info->BiosConnector[1].valid = TRUE;
#endif
	    }
	}

	if (info->InternalTVOut) {
	    info->BiosConnector[2].ConnectorType = CONNECTOR_STV;
	    info->BiosConnector[2].DACType = DAC_TVDAC;
	    info->BiosConnector[2].TMDSType = TMDS_NONE;
	    info->BiosConnector[2].ddc_i2c.valid = FALSE;
	    info->BiosConnector[2].valid = TRUE;
	}

	/* Some cards have the DDC lines swapped and we have no way to
	 * detect it yet (Mac cards)
	 */
	if (xf86ReturnOptValBool(info->Options, OPTION_REVERSE_DDC, FALSE)) {
	    info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_VGA_DDC);
	    info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(RADEON_GPIO_DVI_DDC);
	}
    }
}

#if defined(__powerpc__)

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
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR (pScrn);
    int			o;
    int			index_mask = 0;

    if (IS_DCE3_VARIANT)
	return index_mask;

    /* LVDS is too wacky */
    if (radeon_output->type == OUTPUT_LVDS)
	return index_mask;

    for (o = 0; o < config->num_output; o++) {
	xf86OutputPtr clone = config->output[o];
	RADEONOutputPrivatePtr radeon_clone = clone->driver_private;
	if (output == clone) /* don't clone yourself */
	    continue;
	else if (radeon_clone->type == OUTPUT_LVDS) /* LVDS */
	    continue;
	else if ((radeon_output->DACType != DAC_NONE) &&
		 (radeon_output->DACType == radeon_clone->DACType)) /* shared dac */
	    continue;
	else if ((radeon_output->TMDSType != TMDS_NONE) &&
		 (radeon_output->TMDSType == radeon_clone->TMDSType)) /* shared tmds */
	    continue;
	else
	    index_mask |= (1 << o);
    }

    return index_mask;
}

/*
 * initialise the static data sos we don't have to re-do at randr change */
Bool RADEONSetupConnectors(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt  = RADEONEntPriv(pScrn);
    xf86OutputPtr output;
    char *optstr;
    int i = 0;
    int num_vga = 0;
    int num_dvi = 0;
    int num_hdmi = 0;

    /* We first get the information about all connectors from BIOS.
     * This is how the card is phyiscally wired up.
     * The information should be correct even on a OEM card.
     */
    for (i = 0; i < RADEON_MAX_BIOS_CONNECTOR; i++) {
	info->BiosConnector[i].valid = FALSE;
	info->BiosConnector[i].ddc_i2c.valid = FALSE;
	info->BiosConnector[i].DACType = DAC_NONE;
	info->BiosConnector[i].TMDSType = TMDS_NONE;
	info->BiosConnector[i].ConnectorType = CONNECTOR_NONE;
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

    if (!pRADEONEnt->HasCRTC2) {
	for (i = 0; i < RADEON_MAX_BIOS_CONNECTOR; i++) {
	    if (info->BiosConnector[i].ConnectorType == CONNECTOR_VGA)
		info->BiosConnector[i].DACType = DAC_PRIMARY;
	}
    }

    /* parse connector table option */
    optstr = (char *)xf86GetOptValString(info->Options, OPTION_CONNECTORTABLE);

    if (optstr) {
	unsigned int ddc_line[2];

	for (i = 2; i < RADEON_MAX_BIOS_CONNECTOR; i++) {
	    info->BiosConnector[i].valid = FALSE;
	}
	info->BiosConnector[0].valid = TRUE;
	info->BiosConnector[1].valid = TRUE;
	if (sscanf(optstr, "%u,%u,%u,%u,%u,%u,%u,%u",
		   &ddc_line[0],
		   &info->BiosConnector[0].DACType,
		   &info->BiosConnector[0].TMDSType,
		   &info->BiosConnector[0].ConnectorType,
		   &ddc_line[1],
		   &info->BiosConnector[1].DACType,
		   &info->BiosConnector[1].TMDSType,
		   &info->BiosConnector[1].ConnectorType) != 8) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Invalid ConnectorTable option: %s\n", optstr);
	    return FALSE;
	}

	info->BiosConnector[0].ddc_i2c = legacy_setup_i2c_bus(ddc_line[0]);
	info->BiosConnector[1].ddc_i2c = legacy_setup_i2c_bus(ddc_line[1]);
    }

    info->tvdac_use_count = 0;
    for (i = 0; i < RADEON_MAX_BIOS_CONNECTOR; i++) {
	if (info->BiosConnector[i].valid) {
	    if (info->BiosConnector[i].DACType == DAC_TVDAC)
		info->tvdac_use_count++;

	    if ((info->BiosConnector[i].ConnectorType == CONNECTOR_DVI_D) ||
		(info->BiosConnector[i].ConnectorType == CONNECTOR_DVI_I) ||
		(info->BiosConnector[i].ConnectorType == CONNECTOR_DVI_A)) {
		num_dvi++;
	    } else if (info->BiosConnector[i].ConnectorType == CONNECTOR_VGA) {
		num_vga++;
	    } else if ((info->BiosConnector[i].ConnectorType == CONNECTOR_HDMI_TYPE_A) ||
		       (info->BiosConnector[i].ConnectorType == CONNECTOR_HDMI_TYPE_B)) {
		num_hdmi++;
	    }
	}
    }

    /* clear the enable masks */
    info->output_crt1 = 0;
    info->output_crt2 = 0;
    info->output_dfp1 = 0;
    info->output_dfp2 = 0;
    info->output_lcd1 = 0;
    info->output_tv1 = 0;

    for (i = 0 ; i < RADEON_MAX_BIOS_CONNECTOR; i++) {
	if (info->BiosConnector[i].valid) {
	    RADEONOutputPrivatePtr radeon_output;

	    if (info->BiosConnector[i].ConnectorType == CONNECTOR_NONE)
		continue;

	    radeon_output = xnfcalloc(sizeof(RADEONOutputPrivateRec), 1);
	    if (!radeon_output) {
		return FALSE;
	    }
	    radeon_output->MonType = MT_UNKNOWN;
	    radeon_output->ConnectorType = info->BiosConnector[i].ConnectorType;
	    radeon_output->devices = info->BiosConnector[i].devices;
	    radeon_output->output_id = info->BiosConnector[i].output_id;
	    radeon_output->ddc_i2c = info->BiosConnector[i].ddc_i2c;
	    radeon_output->igp_lane_info = info->BiosConnector[i].igp_lane_info;

	    if (radeon_output->ConnectorType == CONNECTOR_DVI_D)
		radeon_output->DACType = DAC_NONE;
	    else
		radeon_output->DACType = info->BiosConnector[i].DACType;

	    if (radeon_output->ConnectorType == CONNECTOR_VGA)
		radeon_output->TMDSType = TMDS_NONE;
	    else
		radeon_output->TMDSType = info->BiosConnector[i].TMDSType;

	    RADEONSetOutputType(pScrn, radeon_output);
	    if ((info->BiosConnector[i].ConnectorType == CONNECTOR_DVI_D) ||
		(info->BiosConnector[i].ConnectorType == CONNECTOR_DVI_I) ||
		(info->BiosConnector[i].ConnectorType == CONNECTOR_DVI_A)) {
		if (num_dvi > 1) {
		    output = xf86OutputCreate(pScrn, &radeon_output_funcs, "DVI-1");
		    num_dvi--;
		} else {
		    output = xf86OutputCreate(pScrn, &radeon_output_funcs, "DVI-0");
		}
	    } else if (info->BiosConnector[i].ConnectorType == CONNECTOR_VGA) {
		if (num_vga > 1) {
		    output = xf86OutputCreate(pScrn, &radeon_output_funcs, "VGA-1");
		    num_vga--;
		} else {
		    output = xf86OutputCreate(pScrn, &radeon_output_funcs, "VGA-0");
		}
	    } else if ((info->BiosConnector[i].ConnectorType == CONNECTOR_HDMI_TYPE_A) ||
		(info->BiosConnector[i].ConnectorType == CONNECTOR_HDMI_TYPE_B)) {
		if (num_hdmi > 1) {
		    output = xf86OutputCreate(pScrn, &radeon_output_funcs, "HDMI-1");
		    num_hdmi--;
		} else {
		    output = xf86OutputCreate(pScrn, &radeon_output_funcs, "HDMI-0");
		}
	    } else
		output = xf86OutputCreate(pScrn, &radeon_output_funcs, OutputType[radeon_output->type]);

	    if (!output) {
		return FALSE;
	    }
	    output->driver_private = radeon_output;
	    output->possible_crtcs = 1;
	    /* crtc2 can drive LVDS, it just doesn't have RMX */
	    if (radeon_output->type != OUTPUT_LVDS)
		output->possible_crtcs |= 2;

	    /* we can clone the DACs, and probably TV-out, 
	       but I'm not sure it's worth the trouble */
	    output->possible_clones = 0;

	    RADEONInitConnector(output);
	}
    }

    for (i = 0; i < xf86_config->num_output; i++) {
	xf86OutputPtr output = xf86_config->output[i];

	output->possible_clones = radeon_output_clones(pScrn, output);
    }

    return TRUE;
}

