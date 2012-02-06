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
 *    Alex Deucher <alexdeucher@gmail.com>
 *
 */

/*
 * avivo output handling functions.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
/* DPMS */
#ifdef HAVE_X11_EXTENSIONS_DPMSCONST_H
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif
#include <unistd.h>

#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_atombios.h"

#include "ati_pciids_gen.h"

const char *device_name[12] = {
    "CRT1",
    "LCD1",
    "TV1",
    "DFP1",
    "CRT2",
    "LCD2",
    "TV2",
    "DFP2",
    "CV",
    "DFP3",
    "DFP4",
    "DFP5",
};

static int
atombios_output_dac_setup(xf86OutputPtr output, int action)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
    radeon_tvout_ptr tvout = &radeon_output->tvout;
    DAC_ENCODER_CONTROL_PS_ALLOCATION disp_data;
    AtomBiosArgRec data;
    unsigned char *space;
    int index = 0, num = 0;
    int clock = radeon_output->pixel_clock;

    if (radeon_encoder == NULL)
	return ATOM_NOT_IMPLEMENTED;

    memset(&disp_data,0, sizeof(disp_data));

    switch (radeon_encoder->encoder_id) {
    case ENCODER_OBJECT_ID_INTERNAL_DAC1:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1:
	index = GetIndexIntoMasterTable(COMMAND, DAC1EncoderControl);
	num = 1;
	break;
    case ENCODER_OBJECT_ID_INTERNAL_DAC2:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2:
	index = GetIndexIntoMasterTable(COMMAND, DAC2EncoderControl);
	num = 2;
	break;
    }

    disp_data.ucAction =action;

    if (radeon_output->active_device & (ATOM_DEVICE_CRT_SUPPORT))
	disp_data.ucDacStandard = ATOM_DAC1_PS2;
    else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
	disp_data.ucDacStandard = ATOM_DAC1_CV;
    else {
	switch (tvout->tvStd) {
	case TV_STD_PAL:
	case TV_STD_PAL_M:
	case TV_STD_SCART_PAL:
	case TV_STD_SECAM:
	case TV_STD_PAL_CN:
	    disp_data.ucDacStandard = ATOM_DAC1_PAL;
	    break;
	case TV_STD_NTSC:
	case TV_STD_NTSC_J:
	case TV_STD_PAL_60:
	default:
	    disp_data.ucDacStandard = ATOM_DAC1_NTSC;
	    break;
	}
    }
    disp_data.usPixelClock = cpu_to_le16(clock / 10);

    data.exec.index = index;
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &disp_data;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Output DAC%d setup success\n", num);
	return ATOM_SUCCESS;
    }

    ErrorF("Output DAC%d setup failed\n", num);
    return ATOM_NOT_IMPLEMENTED;

}

static int
atombios_output_tv_setup(xf86OutputPtr output, int action)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_tvout_ptr tvout = &radeon_output->tvout;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    TV_ENCODER_CONTROL_PS_ALLOCATION disp_data;
    AtomBiosArgRec data;
    unsigned char *space;
    int clock = radeon_output->pixel_clock;

    memset(&disp_data,0, sizeof(disp_data));

    disp_data.sTVEncoder.ucAction = action;

    if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
	disp_data.sTVEncoder.ucTvStandard = ATOM_TV_CV;
    else {
	switch (tvout->tvStd) {
	case TV_STD_NTSC:
	    disp_data.sTVEncoder.ucTvStandard = ATOM_TV_NTSC;
	    break;
	case TV_STD_PAL:
	    disp_data.sTVEncoder.ucTvStandard = ATOM_TV_PAL;
	    break;
	case TV_STD_PAL_M:
	    disp_data.sTVEncoder.ucTvStandard = ATOM_TV_PALM;
	    break;
	case TV_STD_PAL_60:
	    disp_data.sTVEncoder.ucTvStandard = ATOM_TV_PAL60;
	    break;
	case TV_STD_NTSC_J:
	    disp_data.sTVEncoder.ucTvStandard = ATOM_TV_NTSCJ;
	    break;
	case TV_STD_SCART_PAL:
	    disp_data.sTVEncoder.ucTvStandard = ATOM_TV_PAL; /* ??? */
	    break;
	case TV_STD_SECAM:
	    disp_data.sTVEncoder.ucTvStandard = ATOM_TV_SECAM;
	    break;
	case TV_STD_PAL_CN:
	    disp_data.sTVEncoder.ucTvStandard = ATOM_TV_PALCN;
	    break;
	default:
	    disp_data.sTVEncoder.ucTvStandard = ATOM_TV_NTSC;
	    break;
	}
    }

    disp_data.sTVEncoder.usPixelClock = cpu_to_le16(clock / 10);
    data.exec.index = GetIndexIntoMasterTable(COMMAND, TVEncoderControl);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &disp_data;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Output TV setup success\n");
	return ATOM_SUCCESS;
    }

    ErrorF("Output TV setup failed\n");
    return ATOM_NOT_IMPLEMENTED;

}

int
atombios_external_tmds_setup(xf86OutputPtr output, int action)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    ENABLE_EXTERNAL_TMDS_ENCODER_PS_ALLOCATION disp_data;
    AtomBiosArgRec data;
    unsigned char *space;
    int clock = radeon_output->pixel_clock;

    memset(&disp_data,0, sizeof(disp_data));

    disp_data.sXTmdsEncoder.ucEnable = action;

    if (clock > 165000)
	disp_data.sXTmdsEncoder.ucMisc = PANEL_ENCODER_MISC_DUAL;

    if (pScrn->rgbBits == 8)
	disp_data.sXTmdsEncoder.ucMisc |= (1 << 1);

    data.exec.index = GetIndexIntoMasterTable(COMMAND, DVOEncoderControl);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &disp_data;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("External TMDS setup success\n");
	return ATOM_SUCCESS;
    }

    ErrorF("External TMDS setup failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

static int
atombios_output_ddia_setup(xf86OutputPtr output, int action)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    DVO_ENCODER_CONTROL_PS_ALLOCATION disp_data;
    AtomBiosArgRec data;
    unsigned char *space;
    int clock = radeon_output->pixel_clock;

    memset(&disp_data,0, sizeof(disp_data));

    disp_data.sDVOEncoder.ucAction = action;
    disp_data.sDVOEncoder.usPixelClock = cpu_to_le16(clock / 10);

    if (clock > 165000)
	disp_data.sDVOEncoder.usDevAttr.sDigAttrib.ucAttribute = PANEL_ENCODER_MISC_DUAL;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, DVOEncoderControl);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &disp_data;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("DDIA setup success\n");
	return ATOM_SUCCESS;
    }

    ErrorF("DDIA setup failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

static int
atombios_output_digital_setup(xf86OutputPtr output, int action)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
    LVDS_ENCODER_CONTROL_PS_ALLOCATION disp_data;
    LVDS_ENCODER_CONTROL_PS_ALLOCATION_V2 disp_data2;
    AtomBiosArgRec data;
    unsigned char *space;
    int index = 0;
    int major, minor;
    int lvds_misc = 0;
    int clock = radeon_output->pixel_clock;

    if (radeon_encoder == NULL)
	return ATOM_NOT_IMPLEMENTED;

    if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT)) {
	radeon_lvds_ptr lvds = (radeon_lvds_ptr)radeon_encoder->dev_priv;
	if (lvds == NULL)
	    return ATOM_NOT_IMPLEMENTED;
	lvds_misc = lvds->lvds_misc;
    }

    memset(&disp_data,0, sizeof(disp_data));
    memset(&disp_data2,0, sizeof(disp_data2));

    switch (radeon_encoder->encoder_id) {
    case ENCODER_OBJECT_ID_INTERNAL_LVDS:
	index = GetIndexIntoMasterTable(COMMAND, LVDSEncoderControl);
	break;
    case ENCODER_OBJECT_ID_INTERNAL_TMDS1:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_TMDS1:
	index = GetIndexIntoMasterTable(COMMAND, TMDS1EncoderControl);
	break;
    case ENCODER_OBJECT_ID_INTERNAL_LVTM1:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
	if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT))
	    index = GetIndexIntoMasterTable(COMMAND, LVDSEncoderControl);
	else
	    index = GetIndexIntoMasterTable(COMMAND, TMDS2EncoderControl);
	break;
    }

    atombios_get_command_table_version(info->atomBIOS, index, &major, &minor);

    /*ErrorF("table is %d %d\n", major, minor);*/
    switch (major) {
    case 0:
    case 1:
    case 2:
	switch (minor) {
	case 1:
	    disp_data.ucMisc = 0;
	    disp_data.ucAction = action;
	    if ((radeon_output->ConnectorType == CONNECTOR_HDMI_TYPE_A) ||
		(radeon_output->ConnectorType == CONNECTOR_HDMI_TYPE_B))
		disp_data.ucMisc |= PANEL_ENCODER_MISC_HDMI_TYPE;
	    disp_data.usPixelClock = cpu_to_le16(clock / 10);
	    if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT)) {
		if (lvds_misc & (1 << 0))
		    disp_data.ucMisc |= PANEL_ENCODER_MISC_DUAL;
		if (lvds_misc & (1 << 1))
		    disp_data.ucMisc |= (1 << 1);
	    } else {
		if (radeon_output->linkb)
		    disp_data.ucMisc |= PANEL_ENCODER_MISC_TMDS_LINKB;
		if (clock > 165000)
		    disp_data.ucMisc |= PANEL_ENCODER_MISC_DUAL;
		if (pScrn->rgbBits == 8)
		    disp_data.ucMisc |= (1 << 1);
	    }
	    data.exec.pspace = &disp_data;
	    break;
	case 2:
	case 3:
	    disp_data2.ucMisc = 0;
	    disp_data2.ucAction = action;
	    if (minor == 3) {
		if (radeon_output->coherent_mode) {
		    disp_data2.ucMisc |= PANEL_ENCODER_MISC_COHERENT;
		    xf86DrvMsg(output->scrn->scrnIndex, X_INFO, "Coherent Mode enabled\n");
		}
	    }
	    if ((radeon_output->ConnectorType == CONNECTOR_HDMI_TYPE_A) ||
		(radeon_output->ConnectorType == CONNECTOR_HDMI_TYPE_B))
		disp_data2.ucMisc |= PANEL_ENCODER_MISC_HDMI_TYPE;
	    disp_data2.usPixelClock = cpu_to_le16(clock / 10);
	    disp_data2.ucTruncate = 0;
	    disp_data2.ucSpatial = 0;
	    disp_data2.ucTemporal = 0;
	    disp_data2.ucFRC = 0;
	    if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT)) {
		if (lvds_misc & (1 << 0))
		    disp_data2.ucMisc |= PANEL_ENCODER_MISC_DUAL;
		if (lvds_misc & (1 << 5)) {
		    disp_data2.ucSpatial = PANEL_ENCODER_SPATIAL_DITHER_EN;
		    if (lvds_misc & (1 << 1))
			disp_data2.ucSpatial |= PANEL_ENCODER_SPATIAL_DITHER_DEPTH;
		}
		if (lvds_misc & (1 << 6)) {
		    disp_data2.ucTemporal = PANEL_ENCODER_TEMPORAL_DITHER_EN;
		    if (lvds_misc & (1 << 1))
			disp_data2.ucTemporal |= PANEL_ENCODER_TEMPORAL_DITHER_DEPTH;
		    if (((lvds_misc >> 2) & 0x3) == 2)
			disp_data2.ucTemporal |= PANEL_ENCODER_TEMPORAL_LEVEL_4;
		}
	    } else {
		if (radeon_output->linkb)
		    disp_data2.ucMisc |= PANEL_ENCODER_MISC_TMDS_LINKB;
		if (clock > 165000)
		    disp_data2.ucMisc |= PANEL_ENCODER_MISC_DUAL;
	    }
	    data.exec.pspace = &disp_data2;
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

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Output digital setup success\n");
	return ATOM_SUCCESS;
    }

    ErrorF("Output digital setup failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

static int
atombios_maybe_hdmi_mode(xf86OutputPtr output)
{
#ifndef EDID_COMPLETE_RAWDATA
    /* there's no getting this right unless we have complete EDID */
    return ATOM_ENCODER_MODE_HDMI;
#else
    if (output && xf86MonitorIsHDMI(output->MonInfo))
	return ATOM_ENCODER_MODE_HDMI;

    return ATOM_ENCODER_MODE_DVI;
#endif
}

int
atombios_get_encoder_mode(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    /* DVI should really be atombios_maybe_hdmi_mode() as well */
    switch (radeon_output->ConnectorType) {
    case CONNECTOR_DVI_I:
	if (radeon_output->active_device & (ATOM_DEVICE_DFP_SUPPORT))
	    return ATOM_ENCODER_MODE_DVI;
	else
	    return ATOM_ENCODER_MODE_CRT;
	break;
    case CONNECTOR_DVI_D:
    default:
	return ATOM_ENCODER_MODE_DVI;
	break;
    case CONNECTOR_HDMI_TYPE_A:
    case CONNECTOR_HDMI_TYPE_B:
	return atombios_maybe_hdmi_mode(output);
	break;
    case CONNECTOR_LVDS:
	return ATOM_ENCODER_MODE_LVDS;
	break;
    case CONNECTOR_DISPLAY_PORT:
	if (radeon_output->MonType == MT_DP)
	    return ATOM_ENCODER_MODE_DP;
	else
	    return atombios_maybe_hdmi_mode(output);
	break;
    case CONNECTOR_DVI_A:
    case CONNECTOR_VGA:
    case CONNECTOR_STV:
    case CONNECTOR_CTV:
    case CONNECTOR_DIN:
	if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
	    return ATOM_ENCODER_MODE_TV;
	else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
	    return ATOM_ENCODER_MODE_CV;
	else
	    return ATOM_ENCODER_MODE_CRT;
	break;
    }

}

static const int dp_clocks[] = {
    16200,
    27000,
    32400,
    54000,
    0,
    0,
    64800,
    108000,
};
static const int num_dp_clocks = sizeof(dp_clocks) / sizeof(int);

static int
dp_lanes_for_mode_clock(int mode_clock)
{
    int i;
    
    for (i = 0; i < num_dp_clocks; i++)
	if (dp_clocks[i] > (mode_clock / 10))
	    return (i / 2) + 1;

    return 0;
}

static int
dp_link_clock_for_mode_clock(int mode_clock)
{
    int i;

    for (i = 0; i < num_dp_clocks; i++)
	if (dp_clocks[i] > (mode_clock / 10))
	    return (dp_clocks[i % 2]);

    return 0;
}

static int
atombios_output_dig_encoder_setup(xf86OutputPtr output, int action)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
    DIG_ENCODER_CONTROL_PS_ALLOCATION disp_data;
    AtomBiosArgRec data;
    unsigned char *space;
    int index = 0, major, minor, num = 0;
    int clock = radeon_output->pixel_clock;
    int dig_block = radeon_output->dig_block;

    if (radeon_encoder == NULL)
	return ATOM_NOT_IMPLEMENTED;

    memset(&disp_data,0, sizeof(disp_data));

    if (IS_DCE32_VARIANT) {
	if (dig_block)
	    index = GetIndexIntoMasterTable(COMMAND, DIG2EncoderControl);
	else
	    index = GetIndexIntoMasterTable(COMMAND, DIG1EncoderControl);
	num = dig_block + 1;
    } else {
	switch (radeon_encoder->encoder_id) {
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    index = GetIndexIntoMasterTable(COMMAND, DIG1EncoderControl);
	    num = 1;
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
	    index = GetIndexIntoMasterTable(COMMAND, DIG2EncoderControl);
	    num = 2;
	    break;
	}
    }

    atombios_get_command_table_version(info->atomBIOS, index, &major, &minor);

    disp_data.ucAction = action;
    disp_data.usPixelClock = cpu_to_le16(clock / 10);

    if (IS_DCE32_VARIANT) {
	switch (radeon_encoder->encoder_id) {
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    disp_data.ucConfig = ATOM_ENCODER_CONFIG_V2_TRANSMITTER1;
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY1:
	    disp_data.ucConfig = ATOM_ENCODER_CONFIG_V2_TRANSMITTER2;
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY2:
	    disp_data.ucConfig = ATOM_ENCODER_CONFIG_V2_TRANSMITTER3;
	    break;
	}
    } else {
	switch (radeon_encoder->encoder_id) {
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    disp_data.ucConfig = ATOM_ENCODER_CONFIG_UNIPHY;
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
	    disp_data.ucConfig = ATOM_ENCODER_CONFIG_LVTMA;
	    break;
	}
    }

    disp_data.ucEncoderMode = atombios_get_encoder_mode(output);

    if (disp_data.ucEncoderMode == ATOM_ENCODER_MODE_DP) {
	if (radeon_output->linkb)
	    disp_data.ucConfig |= ATOM_ENCODER_CONFIG_LINKB;
	else
	    disp_data.ucConfig |= ATOM_ENCODER_CONFIG_LINKA;

	if (dp_link_clock_for_mode_clock(clock) == 27000)
	    disp_data.ucConfig |= ATOM_ENCODER_CONFIG_DPLINKRATE_2_70GHZ;

	disp_data.ucLaneNum = dp_lanes_for_mode_clock(clock);
    } else if (clock > 165000) {
	disp_data.ucConfig |= ATOM_ENCODER_CONFIG_LINKA_B;
	disp_data.ucLaneNum = 8;
    } else {
	if (radeon_output->linkb)
	    disp_data.ucConfig |= ATOM_ENCODER_CONFIG_LINKB;
	else
	    disp_data.ucConfig |= ATOM_ENCODER_CONFIG_LINKA;

	disp_data.ucLaneNum = 4;
    }

    data.exec.index = index;
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &disp_data;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Output DIG%d encoder setup success\n", num);
	return ATOM_SUCCESS;
    }

    ErrorF("Output DIG%d setup failed\n", num);
    return ATOM_NOT_IMPLEMENTED;

}

union dig_transmitter_control {
    DIG_TRANSMITTER_CONTROL_PS_ALLOCATION v1;
    DIG_TRANSMITTER_CONTROL_PARAMETERS_V2 v2;
};

static int
atombios_output_dig_transmitter_setup(xf86OutputPtr output, int action)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
    union dig_transmitter_control disp_data;
    AtomBiosArgRec data;
    unsigned char *space;
    int index = 0, num = 0;
    int major, minor;
    int clock = radeon_output->pixel_clock;
    int dig_block = radeon_output->dig_block;

    if (radeon_encoder == NULL)
        return ATOM_NOT_IMPLEMENTED;

    memset(&disp_data,0, sizeof(disp_data));

    if (IS_DCE32_VARIANT)
	index = GetIndexIntoMasterTable(COMMAND, UNIPHYTransmitterControl);
    else {
	switch (radeon_encoder->encoder_id) {
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    index = GetIndexIntoMasterTable(COMMAND, DIG1TransmitterControl);
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
	    index = GetIndexIntoMasterTable(COMMAND, DIG2TransmitterControl);
	    break;
	}
    }

    atombios_get_command_table_version(info->atomBIOS, index, &major, &minor);

    disp_data.v1.ucAction = action;

    if (IS_DCE32_VARIANT) {
	if (radeon_output->MonType == MT_DP) {
	    disp_data.v2.usPixelClock =
		cpu_to_le16(dp_link_clock_for_mode_clock(clock));
	    disp_data.v2.acConfig.fDPConnector = 1;
	} else if (clock > 165000) {
	    disp_data.v2.usPixelClock = cpu_to_le16((clock * 10 * 2) / 100);
	    disp_data.v2.acConfig.fDualLinkConnector = 1;
	} else {
	    disp_data.v2.usPixelClock = cpu_to_le16((clock * 10 * 4) / 100);
	}
	if (dig_block)
	    disp_data.v2.acConfig.ucEncoderSel = 1;

	switch (radeon_encoder->encoder_id) {
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    disp_data.v2.acConfig.ucTransmitterSel = 0;
	    num = 0;
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY1:
	    disp_data.v2.acConfig.ucTransmitterSel = 1;
	    num = 1;
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY2:
	    disp_data.v2.acConfig.ucTransmitterSel = 2;
	    num = 2;
	    break;
	}

	if (radeon_output->active_device & (ATOM_DEVICE_DFP_SUPPORT)) {
	    if (radeon_output->coherent_mode) {
		disp_data.v2.acConfig.fCoherentMode = 1;
		xf86DrvMsg(output->scrn->scrnIndex, X_INFO, "UNIPHY%d transmitter: Coherent Mode enabled\n",disp_data.v2.acConfig.ucTransmitterSel);
	    } else
		xf86DrvMsg(output->scrn->scrnIndex, X_INFO, "UNIPHY%d transmitter: Coherent Mode disabled\n",disp_data.v2.acConfig.ucTransmitterSel);
	}
    } else {
	disp_data.v1.ucConfig = ATOM_TRANSMITTER_CONFIG_CLKSRC_PPLL;

	if (radeon_output->MonType == MT_DP)
	    disp_data.v1.usPixelClock =
		cpu_to_le16(dp_link_clock_for_mode_clock(clock));
	else
	    disp_data.v1.usPixelClock = cpu_to_le16((clock) / 10);

	switch (radeon_encoder->encoder_id) {
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_DIG1_ENCODER;
	    if (info->IsIGP) {
		if (clock > 165000) {
		    disp_data.v1.ucConfig |= (ATOM_TRANSMITTER_CONFIG_8LANE_LINK |
					      ATOM_TRANSMITTER_CONFIG_LINKA_B);

		    if (radeon_output->igp_lane_info & 0x3)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_0_7;
		    else if (radeon_output->igp_lane_info & 0xc)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_8_15;
		} else {
		    disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LINKA;
		    if (radeon_output->igp_lane_info & 0x1)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_0_3;
		    else if (radeon_output->igp_lane_info & 0x2)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_4_7;
		    else if (radeon_output->igp_lane_info & 0x4)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_8_11;
		    else if (radeon_output->igp_lane_info & 0x8)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_12_15;
		}
	    } else {
		if (clock > 165000)
		    disp_data.v1.ucConfig |= (ATOM_TRANSMITTER_CONFIG_8LANE_LINK |
					      ATOM_TRANSMITTER_CONFIG_LINKA_B |
					      ATOM_TRANSMITTER_CONFIG_LANE_0_7);
		else {
		    /* XXX */
		    if (radeon_output->linkb)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LINKB | ATOM_TRANSMITTER_CONFIG_LANE_0_3;
		    else
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LINKA | ATOM_TRANSMITTER_CONFIG_LANE_0_3;
		}
	    }
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
	    disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_DIG2_ENCODER;
	    if (clock > 165000)
		disp_data.v1.ucConfig |= (ATOM_TRANSMITTER_CONFIG_8LANE_LINK |
					  ATOM_TRANSMITTER_CONFIG_LINKA_B |
					  ATOM_TRANSMITTER_CONFIG_LANE_0_7);
	    else {
		/* XXX */
		if (radeon_output->linkb)
		    disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LINKB | ATOM_TRANSMITTER_CONFIG_LANE_0_3;
		else
		    disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LINKA | ATOM_TRANSMITTER_CONFIG_LANE_0_3;
	    }
	    break;
	}

	if (radeon_output->active_device & (ATOM_DEVICE_DFP_SUPPORT)) {
	    if (radeon_output->coherent_mode &&
		radeon_output->MonType != MT_DP) {
		disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_COHERENT;
		xf86DrvMsg(output->scrn->scrnIndex, X_INFO,
			"DIG%d transmitter: Coherent Mode enabled\n", num);
	    } else {
		xf86DrvMsg(output->scrn->scrnIndex, X_INFO,
			"DIG%d transmitter: Coherent Mode disabled\n", num);
	    }
	}
    }

    data.exec.index = index;
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &disp_data;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	if (IS_DCE32_VARIANT)
	    ErrorF("Output UNIPHY%d transmitter setup success\n", num);
	else
	   ErrorF("Output DIG%d transmitter setup success\n", num);
	return ATOM_SUCCESS;
    }

    ErrorF("Output DIG%d transmitter setup failed\n", num);
    return ATOM_NOT_IMPLEMENTED;

}

static void atom_rv515_force_tv_scaler(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG(0x659C,0x0);
    OUTREG(0x6594,0x705);
    OUTREG(0x65A4,0x10001);
    OUTREG(0x65D8,0x0);
    OUTREG(0x65B0,0x0);
    OUTREG(0x65C0,0x0);
    OUTREG(0x65D4,0x0);
    OUTREG(0x6578,0x0);
    OUTREG(0x657C,0x841880A8);
    OUTREG(0x6578,0x1);
    OUTREG(0x657C,0x84208680);
    OUTREG(0x6578,0x2);
    OUTREG(0x657C,0xBFF880B0);
    OUTREG(0x6578,0x100);
    OUTREG(0x657C,0x83D88088);
    OUTREG(0x6578,0x101);
    OUTREG(0x657C,0x84608680);
    OUTREG(0x6578,0x102);
    OUTREG(0x657C,0xBFF080D0);
    OUTREG(0x6578,0x200);
    OUTREG(0x657C,0x83988068);
    OUTREG(0x6578,0x201);
    OUTREG(0x657C,0x84A08680);
    OUTREG(0x6578,0x202);
    OUTREG(0x657C,0xBFF080F8);
    OUTREG(0x6578,0x300);
    OUTREG(0x657C,0x83588058);
    OUTREG(0x6578,0x301);
    OUTREG(0x657C,0x84E08660);
    OUTREG(0x6578,0x302);
    OUTREG(0x657C,0xBFF88120);
    OUTREG(0x6578,0x400);
    OUTREG(0x657C,0x83188040);
    OUTREG(0x6578,0x401);
    OUTREG(0x657C,0x85008660);
    OUTREG(0x6578,0x402);
    OUTREG(0x657C,0xBFF88150);
    OUTREG(0x6578,0x500);
    OUTREG(0x657C,0x82D88030);
    OUTREG(0x6578,0x501);
    OUTREG(0x657C,0x85408640);
    OUTREG(0x6578,0x502);
    OUTREG(0x657C,0xBFF88180);
    OUTREG(0x6578,0x600);
    OUTREG(0x657C,0x82A08018);
    OUTREG(0x6578,0x601);
    OUTREG(0x657C,0x85808620);
    OUTREG(0x6578,0x602);
    OUTREG(0x657C,0xBFF081B8);
    OUTREG(0x6578,0x700);
    OUTREG(0x657C,0x82608010);
    OUTREG(0x6578,0x701);
    OUTREG(0x657C,0x85A08600);
    OUTREG(0x6578,0x702);
    OUTREG(0x657C,0x800081F0);
    OUTREG(0x6578,0x800);
    OUTREG(0x657C,0x8228BFF8);
    OUTREG(0x6578,0x801);
    OUTREG(0x657C,0x85E085E0);
    OUTREG(0x6578,0x802);
    OUTREG(0x657C,0xBFF88228);
    OUTREG(0x6578,0x10000);
    OUTREG(0x657C,0x82A8BF00);
    OUTREG(0x6578,0x10001);
    OUTREG(0x657C,0x82A08CC0);
    OUTREG(0x6578,0x10002);
    OUTREG(0x657C,0x8008BEF8);
    OUTREG(0x6578,0x10100);
    OUTREG(0x657C,0x81F0BF28);
    OUTREG(0x6578,0x10101);
    OUTREG(0x657C,0x83608CA0);
    OUTREG(0x6578,0x10102);
    OUTREG(0x657C,0x8018BED0);
    OUTREG(0x6578,0x10200);
    OUTREG(0x657C,0x8148BF38);
    OUTREG(0x6578,0x10201);
    OUTREG(0x657C,0x84408C80);
    OUTREG(0x6578,0x10202);
    OUTREG(0x657C,0x8008BEB8);
    OUTREG(0x6578,0x10300);
    OUTREG(0x657C,0x80B0BF78);
    OUTREG(0x6578,0x10301);
    OUTREG(0x657C,0x85008C20);
    OUTREG(0x6578,0x10302);
    OUTREG(0x657C,0x8020BEA0);
    OUTREG(0x6578,0x10400);
    OUTREG(0x657C,0x8028BF90);
    OUTREG(0x6578,0x10401);
    OUTREG(0x657C,0x85E08BC0);
    OUTREG(0x6578,0x10402);
    OUTREG(0x657C,0x8018BE90);
    OUTREG(0x6578,0x10500);
    OUTREG(0x657C,0xBFB8BFB0);
    OUTREG(0x6578,0x10501);
    OUTREG(0x657C,0x86C08B40);
    OUTREG(0x6578,0x10502);
    OUTREG(0x657C,0x8010BE90);
    OUTREG(0x6578,0x10600);
    OUTREG(0x657C,0xBF58BFC8);
    OUTREG(0x6578,0x10601);
    OUTREG(0x657C,0x87A08AA0);
    OUTREG(0x6578,0x10602);
    OUTREG(0x657C,0x8010BE98);
    OUTREG(0x6578,0x10700);
    OUTREG(0x657C,0xBF10BFF0);
    OUTREG(0x6578,0x10701);
    OUTREG(0x657C,0x886089E0);
    OUTREG(0x6578,0x10702);
    OUTREG(0x657C,0x8018BEB0);
    OUTREG(0x6578,0x10800);
    OUTREG(0x657C,0xBED8BFE8);
    OUTREG(0x6578,0x10801);
    OUTREG(0x657C,0x89408940);
    OUTREG(0x6578,0x10802);
    OUTREG(0x657C,0xBFE8BED8);
    OUTREG(0x6578,0x20000);
    OUTREG(0x657C,0x80008000);
    OUTREG(0x6578,0x20001);
    OUTREG(0x657C,0x90008000);
    OUTREG(0x6578,0x20002);
    OUTREG(0x657C,0x80008000);
    OUTREG(0x6578,0x20003);
    OUTREG(0x657C,0x80008000);
    OUTREG(0x6578,0x20100);
    OUTREG(0x657C,0x80108000);
    OUTREG(0x6578,0x20101);
    OUTREG(0x657C,0x8FE0BF70);
    OUTREG(0x6578,0x20102);
    OUTREG(0x657C,0xBFE880C0);
    OUTREG(0x6578,0x20103);
    OUTREG(0x657C,0x80008000);
    OUTREG(0x6578,0x20200);
    OUTREG(0x657C,0x8018BFF8);
    OUTREG(0x6578,0x20201);
    OUTREG(0x657C,0x8F80BF08);
    OUTREG(0x6578,0x20202);
    OUTREG(0x657C,0xBFD081A0);
    OUTREG(0x6578,0x20203);
    OUTREG(0x657C,0xBFF88000);
    OUTREG(0x6578,0x20300);
    OUTREG(0x657C,0x80188000);
    OUTREG(0x6578,0x20301);
    OUTREG(0x657C,0x8EE0BEC0);
    OUTREG(0x6578,0x20302);
    OUTREG(0x657C,0xBFB082A0);
    OUTREG(0x6578,0x20303);
    OUTREG(0x657C,0x80008000);
    OUTREG(0x6578,0x20400);
    OUTREG(0x657C,0x80188000);
    OUTREG(0x6578,0x20401);
    OUTREG(0x657C,0x8E00BEA0);
    OUTREG(0x6578,0x20402);
    OUTREG(0x657C,0xBF8883C0);
    OUTREG(0x6578,0x20403);
    OUTREG(0x657C,0x80008000);
    OUTREG(0x6578,0x20500);
    OUTREG(0x657C,0x80188000);
    OUTREG(0x6578,0x20501);
    OUTREG(0x657C,0x8D00BE90);
    OUTREG(0x6578,0x20502);
    OUTREG(0x657C,0xBF588500);
    OUTREG(0x6578,0x20503);
    OUTREG(0x657C,0x80008008);
    OUTREG(0x6578,0x20600);
    OUTREG(0x657C,0x80188000);
    OUTREG(0x6578,0x20601);
    OUTREG(0x657C,0x8BC0BE98);
    OUTREG(0x6578,0x20602);
    OUTREG(0x657C,0xBF308660);
    OUTREG(0x6578,0x20603);
    OUTREG(0x657C,0x80008008);
    OUTREG(0x6578,0x20700);
    OUTREG(0x657C,0x80108000);
    OUTREG(0x6578,0x20701);
    OUTREG(0x657C,0x8A80BEB0);
    OUTREG(0x6578,0x20702);
    OUTREG(0x657C,0xBF0087C0);
    OUTREG(0x6578,0x20703);
    OUTREG(0x657C,0x80008008);
    OUTREG(0x6578,0x20800);
    OUTREG(0x657C,0x80108000);
    OUTREG(0x6578,0x20801);
    OUTREG(0x657C,0x8920BED0);
    OUTREG(0x6578,0x20802);
    OUTREG(0x657C,0xBED08920);
    OUTREG(0x6578,0x20803);
    OUTREG(0x657C,0x80008010);
    OUTREG(0x6578,0x30000);
    OUTREG(0x657C,0x90008000);
    OUTREG(0x6578,0x30001);
    OUTREG(0x657C,0x80008000);
    OUTREG(0x6578,0x30100);
    OUTREG(0x657C,0x8FE0BF90);
    OUTREG(0x6578,0x30101);
    OUTREG(0x657C,0xBFF880A0);
    OUTREG(0x6578,0x30200);
    OUTREG(0x657C,0x8F60BF40);
    OUTREG(0x6578,0x30201);
    OUTREG(0x657C,0xBFE88180);
    OUTREG(0x6578,0x30300);
    OUTREG(0x657C,0x8EC0BF00);
    OUTREG(0x6578,0x30301);
    OUTREG(0x657C,0xBFC88280);
    OUTREG(0x6578,0x30400);
    OUTREG(0x657C,0x8DE0BEE0);
    OUTREG(0x6578,0x30401);
    OUTREG(0x657C,0xBFA083A0);
    OUTREG(0x6578,0x30500);
    OUTREG(0x657C,0x8CE0BED0);
    OUTREG(0x6578,0x30501);
    OUTREG(0x657C,0xBF7884E0);
    OUTREG(0x6578,0x30600);
    OUTREG(0x657C,0x8BA0BED8);
    OUTREG(0x6578,0x30601);
    OUTREG(0x657C,0xBF508640);
    OUTREG(0x6578,0x30700);
    OUTREG(0x657C,0x8A60BEE8);
    OUTREG(0x6578,0x30701);
    OUTREG(0x657C,0xBF2087A0);
    OUTREG(0x6578,0x30800);
    OUTREG(0x657C,0x8900BF00);
    OUTREG(0x6578,0x30801);
    OUTREG(0x657C,0xBF008900);
}

static int
atombios_output_yuv_setup(xf86OutputPtr output, Bool enable)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    RADEONCrtcPrivatePtr radeon_crtc = output->crtc->driver_private;
    ENABLE_YUV_PS_ALLOCATION disp_data;
    AtomBiosArgRec data;
    unsigned char *space;
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t temp, reg;

    if (info->ChipFamily >= CHIP_FAMILY_R600)
	reg = R600_BIOS_3_SCRATCH;
    else
	reg = RADEON_BIOS_3_SCRATCH;

    //fix up scratch reg handling
    temp = INREG(reg);
    if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
	OUTREG(reg, (ATOM_S3_TV1_ACTIVE |
		     (radeon_crtc->crtc_id << 18)));
    else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
	OUTREG(reg, (ATOM_S3_CV_ACTIVE |
		     (radeon_crtc->crtc_id << 24)));
    else
	OUTREG(reg, 0);

    memset(&disp_data, 0, sizeof(disp_data));

    if (enable)
	disp_data.ucEnable = ATOM_ENABLE;
    disp_data.ucCRTC = radeon_crtc->crtc_id;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, EnableYUV);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &disp_data;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {

	OUTREG(reg, temp);

	ErrorF("crtc %d YUV %s setup success\n", radeon_crtc->crtc_id, enable ? "enable" : "disable");
	return ATOM_SUCCESS;
    }

    OUTREG(reg, temp);

    ErrorF("crtc %d YUV %s setup failed\n", radeon_crtc->crtc_id, enable ? "enable" : "disable");
    return ATOM_NOT_IMPLEMENTED;

}

static int
atombios_output_overscan_setup(xf86OutputPtr output, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONCrtcPrivatePtr radeon_crtc = output->crtc->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    SET_CRTC_OVERSCAN_PS_ALLOCATION overscan_param;
    AtomBiosArgRec data;
    unsigned char *space;
    memset(&overscan_param, 0, sizeof(overscan_param));

    overscan_param.usOverscanRight = 0;
    overscan_param.usOverscanLeft = 0;
    overscan_param.usOverscanBottom = 0;
    overscan_param.usOverscanTop = 0;
    overscan_param.ucCRTC = radeon_crtc->crtc_id;

    if (radeon_output->Flags & RADEON_USE_RMX) {
	if (radeon_output->rmx_type == RMX_FULL) {
	    overscan_param.usOverscanRight = 0;
	    overscan_param.usOverscanLeft = 0;
	    overscan_param.usOverscanBottom = 0;
	    overscan_param.usOverscanTop = 0;
	} else if (radeon_output->rmx_type == RMX_CENTER) {
	    overscan_param.usOverscanTop = (adjusted_mode->CrtcVDisplay - mode->CrtcVDisplay) / 2;
	    overscan_param.usOverscanBottom = (adjusted_mode->CrtcVDisplay - mode->CrtcVDisplay) / 2;
	    overscan_param.usOverscanLeft = (adjusted_mode->CrtcHDisplay - mode->CrtcHDisplay) / 2;
	    overscan_param.usOverscanRight = (adjusted_mode->CrtcHDisplay - mode->CrtcHDisplay) / 2;
	} else if (radeon_output->rmx_type == RMX_ASPECT) {
	    int a1 = mode->CrtcVDisplay * adjusted_mode->CrtcHDisplay;
	    int a2 = adjusted_mode->CrtcVDisplay * mode->CrtcHDisplay;

	    if (a1 > a2) {
		overscan_param.usOverscanLeft = (adjusted_mode->CrtcHDisplay - (a2 / mode->CrtcVDisplay)) / 2;
		overscan_param.usOverscanRight = (adjusted_mode->CrtcHDisplay - (a2 / mode->CrtcVDisplay)) / 2;
	    } else if (a2 > a1) {
		overscan_param.usOverscanLeft = (adjusted_mode->CrtcVDisplay - (a1 / mode->CrtcHDisplay)) / 2;
		overscan_param.usOverscanRight = (adjusted_mode->CrtcVDisplay - (a1 / mode->CrtcHDisplay)) / 2;
	    }
	}
    }

    data.exec.index = GetIndexIntoMasterTable(COMMAND, SetCRTC_OverScan);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &overscan_param;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Set CRTC %d Overscan success\n", radeon_crtc->crtc_id);
	return ATOM_SUCCESS ;
    }

    ErrorF("Set CRTC %d Overscan failed\n", radeon_crtc->crtc_id);
    return ATOM_NOT_IMPLEMENTED;
}

static int
atombios_output_scaler_setup(xf86OutputPtr output)
{
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_tvout_ptr tvout = &radeon_output->tvout;
    RADEONCrtcPrivatePtr radeon_crtc = output->crtc->driver_private;
    ENABLE_SCALER_PS_ALLOCATION disp_data;
    AtomBiosArgRec data;
    unsigned char *space;

    if (!IS_AVIVO_VARIANT && radeon_crtc->crtc_id)
	return ATOM_SUCCESS;

    memset(&disp_data, 0, sizeof(disp_data));

    disp_data.ucScaler = radeon_crtc->crtc_id;

    if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT)) {
	switch (tvout->tvStd) {
	case TV_STD_NTSC:
	    disp_data.ucTVStandard = ATOM_TV_NTSC;
	    break;
	case TV_STD_PAL:
	    disp_data.ucTVStandard = ATOM_TV_PAL;
	    break;
	case TV_STD_PAL_M:
	    disp_data.ucTVStandard = ATOM_TV_PALM;
	    break;
	case TV_STD_PAL_60:
	    disp_data.ucTVStandard = ATOM_TV_PAL60;
	    break;
	case TV_STD_NTSC_J:
	    disp_data.ucTVStandard = ATOM_TV_NTSCJ;
	    break;
	case TV_STD_SCART_PAL:
	    disp_data.ucTVStandard = ATOM_TV_PAL; /* ??? */
	    break;
	case TV_STD_SECAM:
	    disp_data.ucTVStandard = ATOM_TV_SECAM;
	    break;
	case TV_STD_PAL_CN:
	    disp_data.ucTVStandard = ATOM_TV_PALCN;
	    break;
	default:
	    disp_data.ucTVStandard = ATOM_TV_NTSC;
	    break;
	}
	disp_data.ucEnable = SCALER_ENABLE_MULTITAP_MODE;
        ErrorF("Using TV scaler %x %x\n", disp_data.ucTVStandard, disp_data.ucEnable);
    } else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT)) {
	disp_data.ucTVStandard = ATOM_TV_CV;
	disp_data.ucEnable = SCALER_ENABLE_MULTITAP_MODE;
        ErrorF("Using CV scaler %x %x\n", disp_data.ucTVStandard, disp_data.ucEnable);
    } else if (radeon_output->Flags & RADEON_USE_RMX) {
	ErrorF("Using RMX\n");
	if (radeon_output->rmx_type == RMX_FULL)
	    disp_data.ucEnable = ATOM_SCALER_EXPANSION;
	else if (radeon_output->rmx_type == RMX_CENTER)
	    disp_data.ucEnable = ATOM_SCALER_CENTER;
	else if (radeon_output->rmx_type == RMX_ASPECT)
	    disp_data.ucEnable = ATOM_SCALER_EXPANSION;
    } else {
	ErrorF("Not using RMX\n");
	if (IS_AVIVO_VARIANT)
	    disp_data.ucEnable = ATOM_SCALER_DISABLE;
	else
	    disp_data.ucEnable = ATOM_SCALER_CENTER;
    }

    data.exec.index = GetIndexIntoMasterTable(COMMAND, EnableScaler);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &disp_data;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT | ATOM_DEVICE_TV_SUPPORT)
	    && info->ChipFamily >= CHIP_FAMILY_RV515 && info->ChipFamily <= CHIP_FAMILY_RV570) {
	    ErrorF("forcing TV scaler\n");
	    atom_rv515_force_tv_scaler(output->scrn);
	}
	ErrorF("scaler %d setup success\n", radeon_crtc->crtc_id);
	return ATOM_SUCCESS;
    }

    ErrorF("scaler %d setup failed\n", radeon_crtc->crtc_id);
    return ATOM_NOT_IMPLEMENTED;

}

void
atombios_output_dpms(xf86OutputPtr output, int mode)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    DISPLAY_DEVICE_OUTPUT_CONTROL_PS_ALLOCATION disp_data;
    AtomBiosArgRec data;
    unsigned char *space;
    int index = 0;
    Bool is_dig = FALSE;

    if (radeon_encoder == NULL)
        return;

    switch (radeon_encoder->encoder_id) {
    case ENCODER_OBJECT_ID_INTERNAL_TMDS1:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_TMDS1:
	index = GetIndexIntoMasterTable(COMMAND, TMDSAOutputControl);
	break;
    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY1:
    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY2:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
	is_dig = TRUE;
	break;
    case ENCODER_OBJECT_ID_INTERNAL_DVO1:
    case ENCODER_OBJECT_ID_INTERNAL_DDI:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DVO1:
	index = GetIndexIntoMasterTable(COMMAND, DVOOutputControl);
	break;
    case ENCODER_OBJECT_ID_INTERNAL_LVDS:
	index = GetIndexIntoMasterTable(COMMAND, LCD1OutputControl);
	break;
    case ENCODER_OBJECT_ID_INTERNAL_LVTM1:
	if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT))
	    index = GetIndexIntoMasterTable(COMMAND, LCD1OutputControl);
	else
	    index = GetIndexIntoMasterTable(COMMAND, LVTMAOutputControl);
	break;
    case ENCODER_OBJECT_ID_INTERNAL_DAC1:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1:
	if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
	    index = GetIndexIntoMasterTable(COMMAND, TV1OutputControl);
	else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
	    index = GetIndexIntoMasterTable(COMMAND, CV1OutputControl);
	else
	    index = GetIndexIntoMasterTable(COMMAND, DAC1OutputControl);
	break;
    case ENCODER_OBJECT_ID_INTERNAL_DAC2:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2:
	if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
	    index = GetIndexIntoMasterTable(COMMAND, TV1OutputControl);
	else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
	    index = GetIndexIntoMasterTable(COMMAND, CV1OutputControl);
	else
	    index = GetIndexIntoMasterTable(COMMAND, DAC2OutputControl);
	break;
    }

    switch (mode) {
    case DPMSModeOn:
	radeon_encoder->devices |= radeon_output->active_device;
	if (is_dig)
	    atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_ENABLE_OUTPUT);
	else {
	    disp_data.ucAction = ATOM_ENABLE;
	    data.exec.index = index;
	    data.exec.dataSpace = (void *)&space;
	    data.exec.pspace = &disp_data;

	    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS)
		ErrorF("Output %s enable success\n",
		       device_name[radeon_get_device_index(radeon_output->active_device)]);
	    else
		ErrorF("Output %s enable failed\n",
		       device_name[radeon_get_device_index(radeon_output->active_device)]);
	}
	break;
    case DPMSModeStandby:
    case DPMSModeSuspend:
    case DPMSModeOff:
	radeon_encoder->devices &= ~(radeon_output->active_device);
	if (!radeon_encoder->devices) {
	    if (is_dig)
		atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_DISABLE_OUTPUT);
	    else {
		disp_data.ucAction = ATOM_DISABLE;
		data.exec.index = index;
		data.exec.dataSpace = (void *)&space;
		data.exec.pspace = &disp_data;

		if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data)
		    == ATOM_SUCCESS)
		    ErrorF("Output %s disable success\n",
			   device_name[radeon_get_device_index(radeon_output->active_device)]);
		else
		    ErrorF("Output %s disable failed\n",
			   device_name[radeon_get_device_index(radeon_output->active_device)]);
	    }
	}
	break;
    }
}

static void
atombios_set_output_crtc_source(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONCrtcPrivatePtr radeon_crtc = output->crtc->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
    AtomBiosArgRec data;
    unsigned char *space;
    SELECT_CRTC_SOURCE_PS_ALLOCATION crtc_src_param;
    SELECT_CRTC_SOURCE_PARAMETERS_V2 crtc_src_param2;
    int index = GetIndexIntoMasterTable(COMMAND, SelectCRTC_Source);
    int major, minor;

    if (radeon_encoder == NULL)
	return;

    memset(&crtc_src_param, 0, sizeof(crtc_src_param));
    memset(&crtc_src_param2, 0, sizeof(crtc_src_param2));
    atombios_get_command_table_version(info->atomBIOS, index, &major, &minor);

    /*ErrorF("select crtc source table is %d %d\n", major, minor);*/

    switch(major) {
    case 1:
	switch(minor) {
	case 0:
	case 1:
	default:
	    if (IS_AVIVO_VARIANT)
		crtc_src_param.ucCRTC = radeon_crtc->crtc_id;
	    else {
		if (radeon_encoder->encoder_id == ENCODER_OBJECT_ID_INTERNAL_DAC1)
		    crtc_src_param.ucCRTC = radeon_crtc->crtc_id;
		else
		    crtc_src_param.ucCRTC = radeon_crtc->crtc_id << 2;
	    }
	    switch (radeon_encoder->encoder_id) {
	    case ENCODER_OBJECT_ID_INTERNAL_TMDS1:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_TMDS1:
		crtc_src_param.ucDevice = ATOM_DEVICE_DFP1_INDEX;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_LVDS:
	    case ENCODER_OBJECT_ID_INTERNAL_LVTM1:
		if (radeon_output->active_device & ATOM_DEVICE_LCD1_SUPPORT)
		    crtc_src_param.ucDevice = ATOM_DEVICE_LCD1_INDEX;
		else
		    crtc_src_param.ucDevice = ATOM_DEVICE_DFP3_INDEX;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_DVO1:
	    case ENCODER_OBJECT_ID_INTERNAL_DDI:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DVO1:
		crtc_src_param.ucDevice = ATOM_DEVICE_DFP2_INDEX;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_DAC1:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1:
		if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
		    crtc_src_param.ucDevice = ATOM_DEVICE_TV1_INDEX;
		else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
		    crtc_src_param.ucDevice = ATOM_DEVICE_CV_INDEX;
		else
		    crtc_src_param.ucDevice = ATOM_DEVICE_CRT1_INDEX;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_DAC2:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2:
		if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
		    crtc_src_param.ucDevice = ATOM_DEVICE_TV1_INDEX;
		else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
		    crtc_src_param.ucDevice = ATOM_DEVICE_CV_INDEX;
		else
		    crtc_src_param.ucDevice = ATOM_DEVICE_CRT2_INDEX;
		break;
	    }
	    data.exec.pspace = &crtc_src_param;
	    /*ErrorF("device sourced: 0x%x\n", crtc_src_param.ucDevice);*/
	    break;
	case 2:
	    crtc_src_param2.ucCRTC = radeon_crtc->crtc_id;
	    crtc_src_param2.ucEncodeMode = atombios_get_encoder_mode(output);
	    switch (radeon_encoder->encoder_id) {
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY1:
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY2:
		if (IS_DCE32_VARIANT) {
		    if (radeon_crtc->crtc_id)
			crtc_src_param2.ucEncoderID = ASIC_INT_DIG2_ENCODER_ID;
		    else
			crtc_src_param2.ucEncoderID = ASIC_INT_DIG1_ENCODER_ID;
		} else
		    crtc_src_param2.ucEncoderID = ASIC_INT_DIG1_ENCODER_ID;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
		crtc_src_param2.ucEncoderID = ASIC_INT_DIG2_ENCODER_ID;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1:
		if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
		    crtc_src_param2.ucEncoderID = ASIC_INT_TV_ENCODER_ID;
		else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
		    crtc_src_param2.ucEncoderID = ASIC_INT_TV_ENCODER_ID;
		else
		    crtc_src_param2.ucEncoderID = ASIC_INT_DAC1_ENCODER_ID;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2:
		if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
		    crtc_src_param2.ucEncoderID = ASIC_INT_TV_ENCODER_ID;
		else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
		    crtc_src_param2.ucEncoderID = ASIC_INT_TV_ENCODER_ID;
		else
		    crtc_src_param2.ucEncoderID = ASIC_INT_DAC2_ENCODER_ID;
		break;
	    }
	    data.exec.pspace = &crtc_src_param2;
	    /*ErrorF("device sourced: 0x%x\n", crtc_src_param2.ucEncoderID);*/
	    break;
	}
	break;
    default:
	ErrorF("Unknown table version\n");
	exit(-1);
    }

    data.exec.index = index;
    data.exec.dataSpace = (void *)&space;

    if (RHDAtomBiosFunc(info->atomBIOS->scrnIndex, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Set CRTC %d Source success\n", radeon_crtc->crtc_id);
	return;
    }

    ErrorF("Set CRTC Source failed\n");
    return;
}

static void
atombios_apply_output_quirks(xf86OutputPtr output, DisplayModePtr mode)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONCrtcPrivatePtr radeon_crtc = output->crtc->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    unsigned char *RADEONMMIO = info->MMIO;

    /* Funky macbooks */
    if ((info->Chipset == PCI_CHIP_RV530_71C5) &&
	(PCI_SUB_VENDOR_ID(info->PciInfo) == 0x106b) &&
	(PCI_SUB_DEVICE_ID(info->PciInfo) == 0x0080)) {
	if (radeon_output->MonType == MT_LCD) {
	    if (radeon_output->devices & ATOM_DEVICE_LCD1_SUPPORT) {
		uint32_t lvtma_bit_depth_control = INREG(AVIVO_LVTMA_BIT_DEPTH_CONTROL);

		lvtma_bit_depth_control &= ~AVIVO_LVTMA_BIT_DEPTH_CONTROL_TRUNCATE_EN;
		lvtma_bit_depth_control &= ~AVIVO_LVTMA_BIT_DEPTH_CONTROL_SPATIAL_DITHER_EN;

		OUTREG(AVIVO_LVTMA_BIT_DEPTH_CONTROL, lvtma_bit_depth_control);
	    }
	}
    }

    /* set scaler clears this on some chips */
    if (IS_AVIVO_VARIANT && (mode->Flags & V_INTERLACE))
	OUTREG(AVIVO_D1MODE_DATA_FORMAT + radeon_crtc->crtc_offset, AVIVO_D1MODE_INTERLEAVE_EN);
}

void
atombios_output_mode_set(xf86OutputPtr output,
			 DisplayModePtr mode,
			 DisplayModePtr adjusted_mode)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONCrtcPrivatePtr radeon_crtc = output->crtc->driver_private;
    radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    if (radeon_encoder == NULL)
        return;

    radeon_output->pixel_clock = adjusted_mode->Clock;
    radeon_output->dig_block = radeon_crtc->crtc_id;
    atombios_output_overscan_setup(output, mode, adjusted_mode);
    atombios_output_scaler_setup(output);
    atombios_set_output_crtc_source(output);

    if (IS_AVIVO_VARIANT) {
	if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT | ATOM_DEVICE_TV_SUPPORT))
	    atombios_output_yuv_setup(output, TRUE);
	else
	    atombios_output_yuv_setup(output, FALSE);
    }

    switch (radeon_encoder->encoder_id) {
    case ENCODER_OBJECT_ID_INTERNAL_TMDS1:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_TMDS1:
    case ENCODER_OBJECT_ID_INTERNAL_LVDS:
    case ENCODER_OBJECT_ID_INTERNAL_LVTM1:
	atombios_output_digital_setup(output, PANEL_ENCODER_ACTION_ENABLE);
	break;
    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY1:
    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY2:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
	/* disable encoder and transmitter */
	atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_DISABLE);
	atombios_output_dig_encoder_setup(output, ATOM_DISABLE);

	/* setup and enable the encoder and transmitter */
	atombios_output_dig_encoder_setup(output, ATOM_ENABLE);
	atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_SETUP);
	atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_ENABLE);
	break;
    case ENCODER_OBJECT_ID_INTERNAL_DDI:
	atombios_output_ddia_setup(output, ATOM_ENABLE);
	break;
    case ENCODER_OBJECT_ID_INTERNAL_DVO1:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DVO1:
	atombios_external_tmds_setup(output, ATOM_ENABLE);
	break;
    case ENCODER_OBJECT_ID_INTERNAL_DAC1:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1:
    case ENCODER_OBJECT_ID_INTERNAL_DAC2:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2:
	atombios_output_dac_setup(output, ATOM_ENABLE);
	if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT | ATOM_DEVICE_CV_SUPPORT))
	    atombios_output_tv_setup(output, ATOM_ENABLE);
	break;
    }
    atombios_apply_output_quirks(output, adjusted_mode);
}

static AtomBiosResult
atom_bios_dac_load_detect(atomBiosHandlePtr atomBIOS, xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    DAC_LOAD_DETECTION_PS_ALLOCATION dac_data;
    AtomBiosArgRec data;
    unsigned char *space;
    int major, minor;
    int index = GetIndexIntoMasterTable(COMMAND, DAC_LoadDetection);

    atombios_get_command_table_version(info->atomBIOS, index, &major, &minor);

    dac_data.sDacload.ucMisc = 0;

    if (radeon_output->devices & ATOM_DEVICE_CRT1_SUPPORT) {
	dac_data.sDacload.usDeviceID = cpu_to_le16(ATOM_DEVICE_CRT1_SUPPORT);
	if (info->encoders[ATOM_DEVICE_CRT1_INDEX] &&
	    ((info->encoders[ATOM_DEVICE_CRT1_INDEX]->encoder_id == ENCODER_OBJECT_ID_INTERNAL_DAC1) ||
	     (info->encoders[ATOM_DEVICE_CRT1_INDEX]->encoder_id == ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1)))
	    dac_data.sDacload.ucDacType = ATOM_DAC_A;
	else
	    dac_data.sDacload.ucDacType = ATOM_DAC_B;
    } else if (radeon_output->devices & ATOM_DEVICE_CRT2_SUPPORT) {
	dac_data.sDacload.usDeviceID = cpu_to_le16(ATOM_DEVICE_CRT2_SUPPORT);
	if (info->encoders[ATOM_DEVICE_CRT2_INDEX] &&
	    ((info->encoders[ATOM_DEVICE_CRT2_INDEX]->encoder_id == ENCODER_OBJECT_ID_INTERNAL_DAC1) ||
	     (info->encoders[ATOM_DEVICE_CRT2_INDEX]->encoder_id == ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1)))
	    dac_data.sDacload.ucDacType = ATOM_DAC_A;
	else
	    dac_data.sDacload.ucDacType = ATOM_DAC_B;
    } else if (radeon_output->devices & ATOM_DEVICE_CV_SUPPORT) {
	dac_data.sDacload.usDeviceID = cpu_to_le16(ATOM_DEVICE_CV_SUPPORT);
	if (info->encoders[ATOM_DEVICE_CV_INDEX] &&
	    ((info->encoders[ATOM_DEVICE_CV_INDEX]->encoder_id == ENCODER_OBJECT_ID_INTERNAL_DAC1) ||
	     (info->encoders[ATOM_DEVICE_CV_INDEX]->encoder_id == ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1)))
	    dac_data.sDacload.ucDacType = ATOM_DAC_A;
	else
	    dac_data.sDacload.ucDacType = ATOM_DAC_B;
	if (minor >= 3)
	    dac_data.sDacload.ucMisc = DAC_LOAD_MISC_YPrPb;
    } else if (radeon_output->devices & ATOM_DEVICE_TV1_SUPPORT) {
	dac_data.sDacload.usDeviceID = cpu_to_le16(ATOM_DEVICE_TV1_SUPPORT);
	if (info->encoders[ATOM_DEVICE_TV1_INDEX] &&
	    ((info->encoders[ATOM_DEVICE_TV1_INDEX]->encoder_id == ENCODER_OBJECT_ID_INTERNAL_DAC1) ||
	     (info->encoders[ATOM_DEVICE_TV1_INDEX]->encoder_id == ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1)))
	    dac_data.sDacload.ucDacType = ATOM_DAC_A;
	else
	    dac_data.sDacload.ucDacType = ATOM_DAC_B;
	if (minor >= 3)
	    dac_data.sDacload.ucMisc = DAC_LOAD_MISC_YPrPb;
    } else
	return ATOM_NOT_IMPLEMENTED;

    data.exec.index = index;
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &dac_data;

    if (RHDAtomBiosFunc(atomBIOS->scrnIndex, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Dac detection success\n");
	return ATOM_SUCCESS ;
    }

    ErrorF("DAC detection failed\n");
    return ATOM_NOT_IMPLEMENTED;
}

RADEONMonitorType
atombios_dac_detect(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONMonitorType MonType = MT_NONE;
    AtomBiosResult ret;
    uint32_t bios_0_scratch;

    if (radeon_output->devices & ATOM_DEVICE_TV1_SUPPORT) {
	if (xf86ReturnOptValBool(info->Options, OPTION_FORCE_TVOUT, FALSE)) {
	    if (radeon_output->ConnectorType == CONNECTOR_STV)
		return MT_STV;
	    else
		return MT_CTV;
	}
    }

    ret = atom_bios_dac_load_detect(info->atomBIOS, output);
    if (ret == ATOM_SUCCESS) {
	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    bios_0_scratch = INREG(R600_BIOS_0_SCRATCH);
	else
	    bios_0_scratch = INREG(RADEON_BIOS_0_SCRATCH);
	/*ErrorF("DAC connect %08X\n", (unsigned int)bios_0_scratch);*/

	if (radeon_output->devices & ATOM_DEVICE_CRT1_SUPPORT) {
	    if (bios_0_scratch & ATOM_S0_CRT1_MASK)
		MonType = MT_CRT;
	} else if (radeon_output->devices & ATOM_DEVICE_CRT2_SUPPORT) {
	    if (bios_0_scratch & ATOM_S0_CRT2_MASK)
		MonType = MT_CRT;
	} else if (radeon_output->devices & ATOM_DEVICE_CV_SUPPORT) {
	    if (bios_0_scratch & (ATOM_S0_CV_MASK | ATOM_S0_CV_MASK_A))
		MonType = MT_CV;
	} else if (radeon_output->devices & ATOM_DEVICE_TV1_SUPPORT) {
	    if (bios_0_scratch & (ATOM_S0_TV1_COMPOSITE | ATOM_S0_TV1_COMPOSITE_A))
		MonType = MT_CTV;
	    else if (bios_0_scratch & (ATOM_S0_TV1_SVIDEO | ATOM_S0_TV1_SVIDEO_A))
		MonType = MT_STV;
	}
    }

    return MonType;
}

