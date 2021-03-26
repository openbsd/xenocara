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

#include "xf86.h"
#include "xf86Modes.h"

#ifdef HAVE_XEXTPROTO_71
#include "X11/extensions/dpmsconst.h"
#else
#define DPMS_SERVER
#include "X11/extensions/dpms.h"
#endif

#include "r128.h"
#include "r128_probe.h"
#include "r128_reg.h"

static void R128ConnectorFindMonitor(ScrnInfoPtr pScrn, xf86OutputPtr output);

/* Define DAC registers for the requested video mode. */
void R128InitDACRegisters(R128SavePtr orig, R128SavePtr save, xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    R128InfoPtr info = R128PTR(pScrn);
    xf86CrtcPtr crtc = output->crtc;
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;

    save->dac_cntl = (R128_DAC_MASK_ALL | R128_DAC_VGA_ADR_EN |
                      (!r128_crtc->crtc_id ? 0 : R128_DAC_CRT_SEL_CRTC2) |
                      (info->dac6bits      ? 0 : R128_DAC_8BIT_EN));
}

/* Write DAC registers */
void R128RestoreDACRegisters(ScrnInfoPtr pScrn, R128SavePtr restore)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    OUTREGP(R128_DAC_CNTL, restore->dac_cntl,
        R128_DAC_RANGE_CNTL | R128_DAC_BLANKING);
}

static void r128_dpms(xf86OutputPtr output, int mode)
{
    switch(mode) {
    case DPMSModeOn:
        R128DPMSSetOn(output);
        break;
    case DPMSModeStandby:
    case DPMSModeSuspend:
    case DPMSModeOff:
        R128DPMSSetOff(output);
        break;
    }
}

static void r128_save(xf86OutputPtr output)
{
}

static void r128_restore(xf86OutputPtr output)
{
}

static int r128_mode_valid(xf86OutputPtr output, DisplayModePtr mode)
{
    return MODE_OK;
}

static Bool r128_mode_fixup(xf86OutputPtr output, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
    return TRUE;
}

static void r128_mode_prepare(xf86OutputPtr output)
{
    r128_dpms(output, DPMSModeOff);
}

static void r128_mode_set(xf86OutputPtr output, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    R128InfoPtr info = R128PTR(pScrn);
    R128OutputPrivatePtr r128_output = output->driver_private;
    xf86CrtcPtr crtc = output->crtc;
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;

    if (r128_crtc->crtc_id == 0 && !info->isPro2)
        R128InitRMXRegisters(&info->SavedReg, &info->ModeReg, output, adjusted_mode);

    if (r128_output->MonType == MT_DFP)
        R128InitFPRegisters(&info->SavedReg, &info->ModeReg, output);
    else if (r128_output->MonType == MT_LCD)
        R128InitLVDSRegisters(&info->SavedReg, &info->ModeReg, output);
    else if (r128_output->MonType == MT_CRT)
        R128InitDACRegisters(&info->SavedReg, &info->ModeReg, output);

    if (r128_crtc->crtc_id == 0 && !info->isPro2)
        R128RestoreRMXRegisters(pScrn, &info->ModeReg);

    if (r128_output->MonType == MT_DFP)
        R128RestoreFPRegisters(pScrn, &info->ModeReg);
    else if (r128_output->MonType == MT_LCD)
        R128RestoreLVDSRegisters(pScrn, &info->ModeReg);
    else if (r128_output->MonType == MT_CRT)
        R128RestoreDACRegisters(pScrn, &info->ModeReg);
}

static void r128_mode_commit(xf86OutputPtr output)
{
    r128_dpms(output, DPMSModeOn);
}

static xf86OutputStatus r128_detect(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    R128OutputPrivatePtr r128_output = output->driver_private;

    r128_output->MonType = MT_UNKNOWN;
    R128ConnectorFindMonitor(pScrn, output);

    if (r128_output->MonType == MT_UNKNOWN) {
        output->subpixel_order = SubPixelUnknown;
        return XF86OutputStatusUnknown;
    } else if (r128_output->MonType == MT_NONE) {
        output->subpixel_order = SubPixelUnknown;
        return XF86OutputStatusDisconnected;
    } else {
        switch(r128_output->MonType) {
        case MT_LCD:
        case MT_DFP:
            output->subpixel_order = SubPixelHorizontalRGB;
            break;
        default:
            output->subpixel_order = SubPixelNone;
            break;
        }

        return XF86OutputStatusConnected;
    }
}

static DisplayModePtr r128_get_modes(xf86OutputPtr output)
{
    DisplayModePtr modes;
    modes = R128ProbeOutputModes(output);
    return modes;
}

static void r128_destroy(xf86OutputPtr output)
{
    if (output->driver_private)
        free(output->driver_private);
}

static const xf86OutputFuncsRec r128_output_funcs = {
    .dpms = r128_dpms,
    .save = r128_save,
    .restore = r128_restore,
    .mode_valid = r128_mode_valid,
    .mode_fixup = r128_mode_fixup,
    .prepare = r128_mode_prepare,
    .mode_set = r128_mode_set,
    .commit = r128_mode_commit,
    .detect = r128_detect,
    .get_modes = r128_get_modes,
    .destroy = r128_destroy,
};

void R128DPMSSetOn(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    R128InfoPtr info = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    R128OutputPrivatePtr r128_output = output->driver_private;
    R128MonitorType MonType = r128_output->MonType;
    R128SavePtr save = &info->ModeReg;

    switch(MonType) {
    case MT_LCD:
        OUTREGP(R128_LVDS_GEN_CNTL, R128_LVDS_BLON, ~R128_LVDS_BLON);
        usleep(r128_output->PanelPwrDly * 1000);
        OUTREGP(R128_LVDS_GEN_CNTL, R128_LVDS_ON, ~R128_LVDS_ON);
        save->lvds_gen_cntl |=     (R128_LVDS_ON | R128_LVDS_BLON);
        break;
    case MT_DFP:
        OUTREGP(R128_FP_GEN_CNTL,  (R128_FP_FPON | R128_FP_TMDS_EN), ~(R128_FP_FPON | R128_FP_TMDS_EN));
        save->fp_gen_cntl   |=     (R128_FP_FPON | R128_FP_TMDS_EN);
        break;
    case MT_CRT:
        OUTREGP(R128_CRTC_EXT_CNTL, R128_CRTC_CRT_ON, ~R128_CRTC_CRT_ON);
        save->crtc_ext_cntl |=      R128_CRTC_CRT_ON;
        break;
    default:
        break;
    }
}

void R128DPMSSetOff(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    R128InfoPtr info = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    R128OutputPrivatePtr r128_output = output->driver_private;
    R128MonitorType MonType = r128_output->MonType;
    R128SavePtr save = &info->ModeReg;

    switch(MonType) {
    case MT_LCD:
        OUTREGP(R128_LVDS_GEN_CNTL, 0, ~(R128_LVDS_BLON | R128_LVDS_ON));
        save->lvds_gen_cntl &=         ~(R128_LVDS_BLON | R128_LVDS_ON);
        break;
    case MT_DFP:
        OUTREGP(R128_FP_GEN_CNTL,   0, ~(R128_FP_FPON | R128_FP_TMDS_EN));
        save->fp_gen_cntl   &=         ~(R128_FP_FPON | R128_FP_TMDS_EN);
        break;
    case MT_CRT:
        OUTREGP(R128_CRTC_EXT_CNTL, 0, ~(R128_CRTC_CRT_ON));
        save->crtc_ext_cntl &=         ~(R128_CRTC_CRT_ON);
        break;
    default:
        break;
    }
}

static R128MonitorType R128DisplayDDCConnected(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    R128InfoPtr info = R128PTR(pScrn);
    R128EntPtr pR128Ent = R128EntPriv(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    R128OutputPrivatePtr r128_output = output->driver_private;

    R128MonitorType MonType = MT_CRT;
    xf86MonPtr *MonInfo = &output->MonInfo;
    uint32_t mask1, mask2;

    if (r128_output->type == OUTPUT_LVDS) {
        return MT_LCD;
    } else if (r128_output->type == OUTPUT_VGA) {
        mask1 = R128_GPIO_MONID_MASK_1 | (pR128Ent->HasCRTC2 ? R128_GPIO_MONID_MASK_3 : R128_GPIO_MONID_MASK_2);
        mask2 = R128_GPIO_MONID_A_1    | (pR128Ent->HasCRTC2 ? R128_GPIO_MONID_A_3    : R128_GPIO_MONID_A_2);
    } else {
        mask1 = R128_GPIO_MONID_MASK_0 | R128_GPIO_MONID_MASK_3;
        mask2 = R128_GPIO_MONID_A_0    | R128_GPIO_MONID_A_3;
    }

    if (r128_output->pI2CBus) {
        R128I2CBusPtr pR128I2CBus = &(r128_output->ddc_i2c);

        /* XXX: Radeon does something here to appease old monitors. */
        OUTREG(pR128I2CBus->ddc_reg, INREG(pR128I2CBus->ddc_reg)  |  mask1);
        OUTREG(pR128I2CBus->ddc_reg, INREG(pR128I2CBus->ddc_reg)  & ~mask2);
        *MonInfo = xf86DoEDID_DDC2(XF86_SCRN_ARG(pScrn), r128_output->pI2CBus);
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "DDC2/I2C is not properly initialized\n");
        return MT_NONE;
    }

    if (*MonInfo) {
        if (r128_output->type == OUTPUT_VGA) {
            MonType = MT_CRT;
        } else {
            if ((*MonInfo)->rawData[0x14] & 0x80)
                MonType = MT_DFP;
            else
                MonType = MT_CRT;
        }
    }

    return MonType;
}

static void R128ConnectorFindMonitor(ScrnInfoPtr pScrn, xf86OutputPtr output)
{
    R128OutputPrivatePtr r128_output = output->driver_private;

    /* XXX: We should figure out how the DAC and BIOS scratch registers work
     * to handle the non-DDC case. */
    if (r128_output->MonType == MT_UNKNOWN)
        r128_output->MonType = R128DisplayDDCConnected(output);
}

DisplayModePtr R128ProbeOutputModes(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    R128OutputPrivatePtr r128_output = output->driver_private;
    DisplayModePtr modes = NULL;
    DisplayModePtr mode;
    xf86MonPtr edid_mon;

    if (r128_output->pI2CBus) {
        edid_mon = xf86OutputGetEDID(output, r128_output->pI2CBus);
        xf86OutputSetEDID(output, edid_mon);
        modes = xf86OutputGetEDIDModes(output);
    }

    /* Letting this function return NULL would be a bad idea. With old cards
     * like r128, users often specify a small resolution in order to get DRI.
     * If the X server has to guess modes, the list it comes up with includes
     * high resolutions.
     */
    if (!modes)
        modes = xf86GetDefaultModes();

    for (mode = modes; mode != NULL; mode = mode->next) {
        if (r128_output->type == OUTPUT_DVI) {
            if (mode->type & (M_T_DRIVER | M_T_PREFERRED)) {
                r128_output->PanelXRes = mode->HDisplay;
                r128_output->PanelYRes = mode->VDisplay;
            }
        }

        xf86SetModeCrtc(mode, INTERLACE_HALVE_V);
        if (mode->status == MODE_OK)
            mode->status = R128DoValidMode(output, mode, MODECHECK_FINAL);
    }

    xf86ValidateModesUserConfig(pScrn, modes);
    xf86PruneInvalidModes(pScrn, &modes, FALSE);

    return modes;
}

static xf86OutputPtr R128OutputCreate(ScrnInfoPtr pScrn, const char *name, int i)
{
    char buf[32];
    sprintf(buf, name, i);
    return xf86OutputCreate(pScrn, &r128_output_funcs, buf);
}

static void R128I2CGetBits(I2CBusPtr b, int *Clock, int *data)
{
    ScrnInfoPtr   pScrn       = xf86Screens[b->scrnIndex];
    R128InfoPtr   info        = R128PTR(pScrn);
    unsigned long val;
    unsigned char *R128MMIO   = info->MMIO;
    R128I2CBusPtr pR128I2CBus = b->DriverPrivate.ptr;

    /* Get the result. */
    val = INREG(pR128I2CBus->ddc_reg);
    *Clock = (val & pR128I2CBus->get_clk_mask)  != 0;
    *data  = (val & pR128I2CBus->get_data_mask) != 0;
}

static void R128I2CPutBits(I2CBusPtr b, int Clock, int data)
{
    ScrnInfoPtr   pScrn       = xf86Screens[b->scrnIndex];
    R128InfoPtr   info        = R128PTR(pScrn);
    unsigned long val;
    unsigned char *R128MMIO   = info->MMIO;
    R128I2CBusPtr pR128I2CBus = b->DriverPrivate.ptr;

    val = INREG(pR128I2CBus->ddc_reg)
              & ~(uint32_t)(pR128I2CBus->put_clk_mask | pR128I2CBus->put_data_mask);
    val |= (Clock ? 0 : pR128I2CBus->put_clk_mask);
    val |= (data  ? 0 : pR128I2CBus->put_data_mask);
    OUTREG(pR128I2CBus->ddc_reg, val);
}

static Bool R128I2CInit(xf86OutputPtr output, I2CBusPtr *bus_ptr, char *name)
{
    ScrnInfoPtr pScrn = output->scrn;
    R128OutputPrivatePtr r128_output = output->driver_private;
    R128I2CBusPtr pR128I2CBus = &(r128_output->ddc_i2c);
    I2CBusPtr pI2CBus;

    pI2CBus = xf86CreateI2CBusRec();
    if(!pI2CBus) return FALSE;

    pI2CBus->BusName     = name;
    pI2CBus->scrnIndex   = pScrn->scrnIndex;
    pI2CBus->I2CPutBits  = R128I2CPutBits;
    pI2CBus->I2CGetBits  = R128I2CGetBits;
    pI2CBus->AcknTimeout = 5;

    pI2CBus->DriverPrivate.ptr = (pointer)pR128I2CBus;
    if (!xf86I2CBusInit(pI2CBus)) return FALSE;

    *bus_ptr = pI2CBus;
    return TRUE;
}

void R128GetConnectorInfoFromBIOS(ScrnInfoPtr pScrn, R128OutputType *otypes)
{
    R128InfoPtr info = R128PTR(pScrn);
    uint16_t bios_header, offset;
    uint32_t i;

    for (i = 0; i < R128_MAX_BIOS_CONNECTOR; i++) {
        otypes[i] = OUTPUT_NONE;
    }

    /* non-x86 platform */
    if (!info->VBIOS) {
        otypes[0] = OUTPUT_VGA;
	return;
    }

    bios_header = R128_BIOS16(0x48);
    offset = R128_BIOS16(bios_header + 0x40);
    if (offset) {
        otypes[0] = OUTPUT_LVDS;
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "Found FP table, assuming FP connector.\n");
    } else {
        bios_header = R128_BIOS16(0x48);
        offset = R128_BIOS16(bios_header + 0x34);
        if (offset) {
            otypes[0] = OUTPUT_DVI;
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Found DVI table, assuming DVI connector.\n");
        }
    }

    offset = R128_BIOS16(bios_header + 0x2e);
    if (offset) {
        if (otypes[0] == OUTPUT_NONE) {
            otypes[0] = OUTPUT_VGA;
        } else {
            otypes[1] = OUTPUT_VGA;
        }

        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "Found CRT table, assuming VGA connector.\n");
    }
}

Bool R128SetupConnectors(ScrnInfoPtr pScrn)
{
    R128InfoPtr info    = R128PTR(pScrn);
    R128EntPtr pR128Ent = R128EntPriv(pScrn);

    R128OutputType otypes[R128_MAX_BIOS_CONNECTOR];
    xf86OutputPtr  output;
    int num_vga = 0;
    int num_dvi = 0;
    int i;

    R128GetConnectorInfoFromBIOS(pScrn, otypes);

    for (i = 0; i < R128_MAX_BIOS_CONNECTOR; i++) {
        if (otypes[i] == OUTPUT_VGA)
            num_vga++;
        else if (otypes[i] == OUTPUT_DVI)
            num_dvi++;
    }

    for (i = 0; i < R128_MAX_BIOS_CONNECTOR; i++) {
        if (otypes[i] == OUTPUT_NONE) continue;

        R128I2CBusRec i2c;
        R128OutputPrivatePtr r128_output;

        r128_output = xnfcalloc(sizeof(R128OutputPrivateRec), 1);
        if (!r128_output) return FALSE;

        r128_output->MonType = MT_UNKNOWN;
        r128_output->type = otypes[i];
        r128_output->num = i;

        if (otypes[i] == OUTPUT_LVDS) {
            output = R128OutputCreate(pScrn, "LVDS", 0);
        } else if (otypes[i] == OUTPUT_VGA) {
            output = R128OutputCreate(pScrn, "VGA-%d", --num_vga);
        } else {
            output = R128OutputCreate(pScrn, "DVI-%d", --num_dvi);
        }

        if (!output) return FALSE;
        output->interlaceAllowed = TRUE;
        output->doubleScanAllowed = TRUE;
        output->driver_private = r128_output;
        output->possible_clones = 0;
        if (otypes[i] == OUTPUT_LVDS || !pR128Ent->HasCRTC2)
            output->possible_crtcs = 1;
        else
            output->possible_crtcs = 2;

        if (otypes[i] != OUTPUT_LVDS && info->DDC) {
            i2c.ddc_reg      = R128_GPIO_MONID;
            if (otypes[i] == OUTPUT_VGA && !pR128Ent->HasCRTC2) {
                i2c.put_clk_mask = R128_GPIO_MONID_EN_2;
                i2c.get_clk_mask = R128_GPIO_MONID_Y_2;
            } else {
                i2c.put_clk_mask = R128_GPIO_MONID_EN_3;
                i2c.get_clk_mask = R128_GPIO_MONID_Y_3;
            }
            if (otypes[i] == OUTPUT_VGA) {
                i2c.put_data_mask = R128_GPIO_MONID_EN_1;
                i2c.get_data_mask = R128_GPIO_MONID_Y_1;
            } else {
                i2c.put_data_mask = R128_GPIO_MONID_EN_0;
                i2c.get_data_mask = R128_GPIO_MONID_Y_0;
            }
            r128_output->ddc_i2c = i2c;
            R128I2CInit(output, &r128_output->pI2CBus, output->name);
        }

        if (otypes[i] == OUTPUT_LVDS)
            R128GetPanelInfoFromBIOS(output);
    }

    return TRUE;
}
