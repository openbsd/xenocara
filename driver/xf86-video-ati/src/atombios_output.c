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
#ifdef HAVE_XEXTPROTO_71
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

#define AUX_NATIVE_WRITE                    0x8
#define AUX_NATIVE_READ                     0x9

#define AUX_I2C_WRITE                       0x0
#define AUX_I2C_READ                        0x1
#define AUX_I2C_STATUS                      0x2
#define AUX_I2C_MOT                         0x4

#define DP_DPCD_REV                         0x0
#define DP_MAX_LINK_RATE                    0x1
#define DP_MAX_LANE_COUNT                   0x2
#define DP_MAX_DOWNSPREAD                   0x3
#define DP_NORP                             0x4
#define DP_DOWNSTREAMPORT_PRESENT           0x5
#define DP_MAIN_LINK_CHANNEL_CONFIG         0x6
#define DP_DP11_DOWNSTREAM_PORT_COUNT       0x7

/* from intel i830_dp.h */
#define DP_LINK_BW_SET                      0x100
//# define DP_LINK_BW_1_62                    0x06
//# define DP_LINK_BW_2_7                     0x0a
#define DP_LANE_COUNT_SET                   0x101
# define DP_LANE_COUNT_MASK                 0x0f
# define DP_LANE_COUNT_ENHANCED_FRAME_EN    (1 << 7)

#define DP_TRAINING_PATTERN_SET             0x102

# define DP_TRAINING_PATTERN_DISABLE        0
# define DP_TRAINING_PATTERN_1              1
# define DP_TRAINING_PATTERN_2              2
# define DP_TRAINING_PATTERN_MASK           0x3

# define DP_LINK_QUAL_PATTERN_DISABLE       (0 << 2)
# define DP_LINK_QUAL_PATTERN_D10_2         (1 << 2)
# define DP_LINK_QUAL_PATTERN_ERROR_RATE    (2 << 2)
# define DP_LINK_QUAL_PATTERN_PRBS7         (3 << 2)
# define DP_LINK_QUAL_PATTERN_MASK          (3 << 2)
# define DP_RECOVERED_CLOCK_OUT_EN          (1 << 4)
# define DP_LINK_SCRAMBLING_DISABLE         (1 << 5)

# define DP_SYMBOL_ERROR_COUNT_BOTH         (0 << 6)
# define DP_SYMBOL_ERROR_COUNT_DISPARITY    (1 << 6)
# define DP_SYMBOL_ERROR_COUNT_SYMBOL       (2 << 6)
# define DP_SYMBOL_ERROR_COUNT_MASK         (3 << 6)

#define DP_TRAINING_LANE0_SET               0x103
#define DP_TRAINING_LANE1_SET               0x104
#define DP_TRAINING_LANE2_SET               0x105
#define DP_TRAINING_LANE3_SET               0x106
# define DP_TRAIN_VOLTAGE_SWING_MASK        0x3
# define DP_TRAIN_VOLTAGE_SWING_SHIFT       0
# define DP_TRAIN_MAX_SWING_REACHED         (1 << 2)
# define DP_TRAIN_VOLTAGE_SWING_400         (0 << 0)
# define DP_TRAIN_VOLTAGE_SWING_600         (1 << 0)
# define DP_TRAIN_VOLTAGE_SWING_800         (2 << 0)
# define DP_TRAIN_VOLTAGE_SWING_1200        (3 << 0)

# define DP_TRAIN_PRE_EMPHASIS_MASK         (3 << 3)
# define DP_TRAIN_PRE_EMPHASIS_0            (0 << 3)
# define DP_TRAIN_PRE_EMPHASIS_3_5          (1 << 3)
# define DP_TRAIN_PRE_EMPHASIS_6            (2 << 3)
# define DP_TRAIN_PRE_EMPHASIS_9_5          (3 << 3)

# define DP_TRAIN_PRE_EMPHASIS_SHIFT        3
# define DP_TRAIN_MAX_PRE_EMPHASIS_REACHED  (1 << 5)
#define DP_DOWNSPREAD_CTRL                  0x107
# define DP_SPREAD_AMP_0_5                  (1 << 4)

#define DP_MAIN_LINK_CHANNEL_CODING_SET     0x108
# define DP_SET_ANSI_8B10B                  (1 << 0)

#define DP_LANE0_1_STATUS                   0x202
#define DP_LANE2_3_STATUS                   0x203

# define DP_LANE_CR_DONE                    (1 << 0)
# define DP_LANE_CHANNEL_EQ_DONE            (1 << 1)
# define DP_LANE_SYMBOL_LOCKED              (1 << 2)

#define DP_LANE_ALIGN_STATUS_UPDATED        0x204
#define DP_INTERLANE_ALIGN_DONE             (1 << 0)
#define DP_DOWNSTREAM_PORT_STATUS_CHANGED   (1 << 6)
#define DP_LINK_STATUS_UPDATED              (1 << 7)

#define DP_SINK_STATUS                      0x205

#define DP_RECEIVE_PORT_0_STATUS            (1 << 0)
#define DP_RECEIVE_PORT_1_STATUS            (1 << 1)

#define DP_ADJUST_REQUEST_LANE0_1           0x206
#define DP_ADJUST_REQUEST_LANE2_3           0x207

#define DP_ADJUST_VOLTAGE_SWING_LANE0_MASK  0x03
#define DP_ADJUST_VOLTAGE_SWING_LANE0_SHIFT 0
#define DP_ADJUST_PRE_EMPHASIS_LANE0_MASK   0x0c
#define DP_ADJUST_PRE_EMPHASIS_LANE0_SHIFT  2
#define DP_ADJUST_VOLTAGE_SWING_LANE1_MASK  0x30
#define DP_ADJUST_VOLTAGE_SWING_LANE1_SHIFT 4
#define DP_ADJUST_PRE_EMPHASIS_LANE1_MASK   0xc0
#define DP_ADJUST_PRE_EMPHASIS_LANE1_SHIFT  6

#define DP_LINK_STATUS_SIZE                 6
#define DP_LINK_CONFIGURATION_SIZE          9

#define DP_SET_POWER_D0  0x1
#define DP_SET_POWER_D3  0x2

static void do_displayport_link_train(xf86OutputPtr output);

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

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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
    return ATOM_ENCODER_MODE_DVI;
#else
    if (output && xf86MonitorIsHDMI(output->MonInfo))
	return ATOM_ENCODER_MODE_HDMI;

    return ATOM_ENCODER_MODE_DVI;
#endif
}

int
atombios_get_encoder_mode(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info       = RADEONPTR(pScrn);
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
	if (IS_DCE4_VARIANT)
	    return ATOM_ENCODER_MODE_DVI;
	else
	    return atombios_maybe_hdmi_mode(output);
	break;
    case CONNECTOR_LVDS:
	return ATOM_ENCODER_MODE_LVDS;
	break;
    case CONNECTOR_DISPLAY_PORT:
    case CONNECTOR_EDP:
	if (radeon_output->MonType == MT_DP)
	    return ATOM_ENCODER_MODE_DP;
	else {
	    if (IS_DCE4_VARIANT)
	        return ATOM_ENCODER_MODE_DVI;
	    else
	        return atombios_maybe_hdmi_mode(output);
	}
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
    5400,  // 1 lane, 1.62 Ghz
    9000,  // 1 lane, 2.70 Ghz
    10800, // 2 lane, 1.62 Ghz
    18000, // 2 lane, 2.70 Ghz
    21600, // 4 lane, 1.62 Ghz
    36000, // 4 lane, 2.70 Ghz
};
static const int num_dp_clocks = sizeof(dp_clocks) / sizeof(int);

# define DP_LINK_BW_1_62                    0x06
# define DP_LINK_BW_2_7                     0x0a
static int radeon_dp_max_lane_count(xf86OutputPtr output);

static int
dp_lanes_for_mode_clock(xf86OutputPtr output, int mode_clock)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int i;
    int max_link_bw = radeon_output->dpcd[1];
    int max_lane_count = radeon_dp_max_lane_count(output);

    switch (max_link_bw) {
    case DP_LINK_BW_1_62:
    default:
	for (i = 0; i < num_dp_clocks; i++) {
		if (i % 2)
			continue;
		switch (max_lane_count) {
		case 1:
			if (i > 1)
				return 0;
			break;
		case 2:
			if (i > 3)
				return 0;
			break;
		case 4:
		default:
			break;
		}
		if (dp_clocks[i] > (mode_clock/10)) {
			if (i < 2)
				return 1;
			else if (i < 4)
				return 2;
			else
				return 4;
		}
	}
	break;
    case DP_LINK_BW_2_7:
	for (i = 0; i < num_dp_clocks; i++) {
		switch (max_lane_count) {
		case 1:
			if (i > 1)
				return 0;
			break;
		case 2:
			if (i > 3)
				return 0;
			break;
		case 4:
		default:
			break;
		}
		if (dp_clocks[i] > (mode_clock/10)) {
			if (i < 2)
				return 1;
			else if (i < 4)
				return 2;
			else
				return 4;
		}
	}
        break;
    }

    return 0;
}

static int
dp_link_clock_for_mode_clock(xf86OutputPtr output, int mode_clock)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int i;
    int max_link_bw = radeon_output->dpcd[1];
    int max_lane_count = radeon_dp_max_lane_count(output);

    switch (max_link_bw) {
    case DP_LINK_BW_1_62:
    default:
	for (i = 0; i < num_dp_clocks; i++) {
		if (i % 2)
			continue;
		switch (max_lane_count) {
		case 1:
			if (i > 1)
				return 0;
			break;
		case 2:
			if (i > 3)
				return 0;
			break;
		case 4:
		default:
			break;
		}
		if (dp_clocks[i] > (mode_clock/10))
			return 16200;
	}
	break;
    case DP_LINK_BW_2_7:
	for (i = 0; i < num_dp_clocks; i++) {
		switch (max_lane_count) {
		case 1:
			if (i > 1)
				return 0;
			break;
		case 2:
			if (i > 3)
				return 0;
			break;
		case 4:
		default:
			break;
		}
		if (dp_clocks[i] > (mode_clock/10))
			return (i % 2) ? 27000 : 16200;
	}
        break;
    }

    return 0;
}

/*
 * DIG Encoder/Transmitter Setup
 *
 * DCE 3.0/3.1
 * - 2 DIG transmitter blocks. UNIPHY (links A and B) and LVTMA.
 * Supports up to 3 digital outputs
 * - 2 DIG encoder blocks.
 * DIG1 can drive UNIPHY link A or link B
 * DIG2 can drive UNIPHY link B or LVTMA
 *
 * DCE 3.2
 * - 3 DIG transmitter blocks. UNIPHY0/1/2 (links A and B).
 * Supports up to 5 digital outputs
 * - 2 DIG encoder blocks.
 * DIG1/2 can drive UNIPHY0/1/2 link A or link B
 *
 * DCE 4.0
 * - 3 DIG transmitter blocks UNPHY0/1/2 (links A and B).
 * Supports up to 6 digital outputs
 * - 6 DIG encoder blocks.
 * - DIG to PHY mapping is hardcoded
 * DIG1 drives UNIPHY0 link A, A+B
 * DIG2 drives UNIPHY0 link B
 * DIG3 drives UNIPHY1 link A, A+B
 * DIG4 drives UNIPHY1 link B
 * DIG5 drives UNIPHY2 link A, A+B
 * DIG6 drives UNIPHY2 link B
 *
 * Routing
 * crtc -> dig encoder -> UNIPHY/LVTMA (1 or 2 links)
 * Examples:
 * crtc0 -> dig2 -> LVTMA links A+B
 * crtc1 -> dig1 -> UNIPHY0 link B
 * crtc0 -> dig1 -> UNIPHY2 link  A   -> LVDS
 * crtc1 -> dig2 -> UNIPHY1 link  B+A -> TMDS/HDMI
 */

union dig_encoder_control {
	DIG_ENCODER_CONTROL_PS_ALLOCATION v1;
	DIG_ENCODER_CONTROL_PARAMETERS_V2 v2;
	DIG_ENCODER_CONTROL_PARAMETERS_V3 v3;
};

static int
atombios_output_dig_encoder_setup(xf86OutputPtr output, int action)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
    union dig_encoder_control disp_data;
    AtomBiosArgRec data;
    unsigned char *space;
    int index = 0, major, minor;
    int clock = radeon_output->pixel_clock;

    if (radeon_encoder == NULL)
	return ATOM_NOT_IMPLEMENTED;

    memset(&disp_data,0, sizeof(disp_data));

    if (IS_DCE4_VARIANT)
	index = GetIndexIntoMasterTable(COMMAND, DIGxEncoderControl);
    else if (radeon_output->dig_encoder)
        index = GetIndexIntoMasterTable(COMMAND, DIG2EncoderControl);
    else
        index = GetIndexIntoMasterTable(COMMAND, DIG1EncoderControl);

    atombios_get_command_table_version(info->atomBIOS, index, &major, &minor);

    disp_data.v1.ucAction = action;
    disp_data.v1.usPixelClock = cpu_to_le16(clock / 10);
    disp_data.v1.ucEncoderMode = atombios_get_encoder_mode(output);

    if (disp_data.v1.ucEncoderMode == ATOM_ENCODER_MODE_DP) {
	if (dp_link_clock_for_mode_clock(output, clock) == 27000)
	    disp_data.v1.ucConfig |= ATOM_ENCODER_CONFIG_DPLINKRATE_2_70GHZ;
	disp_data.v1.ucLaneNum = dp_lanes_for_mode_clock(output, clock);
    } else if (clock > 165000)
	disp_data.v1.ucLaneNum = 8;
    else
	disp_data.v1.ucLaneNum = 4;

    if (IS_DCE4_VARIANT) {
	disp_data.v3.acConfig.ucDigSel = radeon_output->dig_encoder;
	disp_data.v3.ucBitPerColor = PANEL_8BIT_PER_COLOR;
    } else {
	switch (radeon_encoder->encoder_id) {
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    disp_data.v1.ucConfig = ATOM_ENCODER_CONFIG_V2_TRANSMITTER1;
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY1:
	case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
	    disp_data.v1.ucConfig = ATOM_ENCODER_CONFIG_V2_TRANSMITTER2;
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY2:
	    disp_data.v1.ucConfig = ATOM_ENCODER_CONFIG_V2_TRANSMITTER3;
	    break;
	}
	if (radeon_output->linkb)
	    disp_data.v1.ucConfig |= ATOM_ENCODER_CONFIG_LINKB;
	else
	    disp_data.v1.ucConfig |= ATOM_ENCODER_CONFIG_LINKA;
    }

    data.exec.index = index;
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &disp_data;

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	ErrorF("Output DIG%d encoder setup success\n", radeon_output->dig_encoder);
	return ATOM_SUCCESS;
    }

    ErrorF("Output DIG%d setup failed\n", radeon_output->dig_encoder);
    return ATOM_NOT_IMPLEMENTED;

}

union dig_transmitter_control {
    DIG_TRANSMITTER_CONTROL_PS_ALLOCATION v1;
    DIG_TRANSMITTER_CONTROL_PARAMETERS_V2 v2;
    DIG_TRANSMITTER_CONTROL_PARAMETERS_V3 v3;
};

static int
atombios_output_dig_transmitter_setup(xf86OutputPtr output, int action, uint8_t lane_num, uint8_t lane_set)
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

    if (radeon_encoder == NULL)
        return ATOM_NOT_IMPLEMENTED;

    memset(&disp_data,0, sizeof(disp_data));

    if (IS_DCE32_VARIANT || IS_DCE4_VARIANT)
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
    if (action == ATOM_TRANSMITTER_ACTION_INIT) {
        disp_data.v1.usInitInfo = radeon_output->connector_object_id;
    } else if (action == ATOM_TRANSMITTER_ACTION_SETUP_VSEMPH) {
	disp_data.v1.asMode.ucLaneSel = lane_num;
	disp_data.v1.asMode.ucLaneSet = lane_set;
    } else {
	if (radeon_output->MonType == MT_DP) 
	    disp_data.v1.usPixelClock =
		cpu_to_le16(dp_link_clock_for_mode_clock(output, clock));
	else if (clock > 165000)
	    disp_data.v1.usPixelClock = cpu_to_le16((clock / 2) / 10);
	else
	    disp_data.v1.usPixelClock = cpu_to_le16(clock / 10);
    }

    if (IS_DCE4_VARIANT) {
	if (radeon_output->MonType == MT_DP)
	    disp_data.v3.ucLaneNum = dp_lanes_for_mode_clock(output, clock);
	else if (clock > 165000)
	    disp_data.v3.ucLaneNum = 8;
	else
	    disp_data.v3.ucLaneNum = 4;

	if (radeon_output->linkb) {
	    disp_data.v3.acConfig.ucLinkSel = 1;
	    disp_data.v2.acConfig.ucEncoderSel = 1;
	}

	// select the PLL for the UNIPHY
	if (radeon_output->MonType == MT_DP && info->dp_extclk)
	    disp_data.v3.acConfig.ucRefClkSource = 2; /* ext clk */
	else
	    disp_data.v3.acConfig.ucRefClkSource = radeon_output->pll_id;

	switch (radeon_encoder->encoder_id) {
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    disp_data.v3.acConfig.ucTransmitterSel = 0;
	    num = 0;
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY1:
	    disp_data.v3.acConfig.ucTransmitterSel = 1;
	    num = 1;
	    break;
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY2:
	    disp_data.v3.acConfig.ucTransmitterSel = 2;
	    num = 2;
	    break;
	}

	if (radeon_output->MonType == MT_DP)
	    disp_data.v3.acConfig.fCoherentMode = 1; /* DP requires coherent */
	else if (radeon_output->active_device & (ATOM_DEVICE_DFP_SUPPORT)) {
	    if (radeon_output->coherent_mode)
		disp_data.v3.acConfig.fCoherentMode = 1;
	    if (clock > 165000)
		disp_data.v3.acConfig.fDualLinkConnector = 1;
	}
    } else if (IS_DCE32_VARIANT) {
	if (radeon_output->dig_encoder)
	    disp_data.v2.acConfig.ucEncoderSel = 1;

	if (radeon_output->linkb)
	    disp_data.v2.acConfig.ucLinkSel = 1;

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

	if (radeon_output->MonType == MT_DP)
	    disp_data.v2.acConfig.fCoherentMode = 1; /* DP requires coherent */
	else if (radeon_output->active_device & (ATOM_DEVICE_DFP_SUPPORT)) {
	    if (radeon_output->coherent_mode)
		disp_data.v2.acConfig.fCoherentMode = 1;
	    if (clock > 165000)
		disp_data.v2.acConfig.fDualLinkConnector = 1;
	}
    } else {
	disp_data.v1.ucConfig = ATOM_TRANSMITTER_CONFIG_CLKSRC_PPLL;

	if (radeon_output->dig_encoder)
	    disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_DIG2_ENCODER;
	else
	    disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_DIG1_ENCODER;

	switch (radeon_encoder->encoder_id) {
	case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    if (info->IsIGP) {
		if (clock > 165000) {
		    if (radeon_output->igp_lane_info & 0x3)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_0_7;
		    else if (radeon_output->igp_lane_info & 0xc)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_8_15;
		} else {
		    if (radeon_output->igp_lane_info & 0x1)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_0_3;
		    else if (radeon_output->igp_lane_info & 0x2)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_4_7;
		    else if (radeon_output->igp_lane_info & 0x4)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_8_11;
		    else if (radeon_output->igp_lane_info & 0x8)
			disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LANE_12_15;
		}
	    }
	    break;
	}
	if (radeon_output->linkb)
	    disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LINKB;
	else
	    disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_LINKA;

	if (radeon_output->MonType == MT_DP)
	    disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_COHERENT;  /* DP requires coherent */
	else if (radeon_output->active_device & (ATOM_DEVICE_DFP_SUPPORT)) {
	    if (radeon_output->coherent_mode)
		disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_COHERENT;
	    if (clock > 165000)
		disp_data.v1.ucConfig |= ATOM_TRANSMITTER_CONFIG_8LANE_LINK;
	}
    }

    data.exec.index = index;
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &disp_data;

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	if (IS_DCE32_VARIANT)
	    ErrorF("Output UNIPHY%d transmitter setup success\n", num);
	else
	   ErrorF("Output DIG%d transmitter setup success\n", num);
	return ATOM_SUCCESS;
    }

    ErrorF("Output DIG%d transmitter setup failed\n", num);
    return ATOM_NOT_IMPLEMENTED;

}

static void atom_rv515_force_tv_scaler(ScrnInfoPtr pScrn, RADEONCrtcPrivatePtr radeon_crtc)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int index_reg = 0x6578, data_reg = 0x657c;

    index_reg += radeon_crtc->crtc_offset;
    data_reg += radeon_crtc->crtc_offset;

    OUTREG(0x659C + radeon_crtc->crtc_offset, 0x0);
    OUTREG(0x6594 + radeon_crtc->crtc_offset, 0x705);
    OUTREG(0x65A4 + radeon_crtc->crtc_offset, 0x10001);
    OUTREG(0x65D8 + radeon_crtc->crtc_offset, 0x0);
    OUTREG(0x65B0 + radeon_crtc->crtc_offset, 0x0);
    OUTREG(0x65C0 + radeon_crtc->crtc_offset, 0x0);
    OUTREG(0x65D4 + radeon_crtc->crtc_offset, 0x0);
    OUTREG(index_reg,0x0);
    OUTREG(data_reg,0x841880A8);
    OUTREG(index_reg,0x1);
    OUTREG(data_reg,0x84208680);
    OUTREG(index_reg,0x2);
    OUTREG(data_reg,0xBFF880B0);
    OUTREG(index_reg,0x100);
    OUTREG(data_reg,0x83D88088);
    OUTREG(index_reg,0x101);
    OUTREG(data_reg,0x84608680);
    OUTREG(index_reg,0x102);
    OUTREG(data_reg,0xBFF080D0);
    OUTREG(index_reg,0x200);
    OUTREG(data_reg,0x83988068);
    OUTREG(index_reg,0x201);
    OUTREG(data_reg,0x84A08680);
    OUTREG(index_reg,0x202);
    OUTREG(data_reg,0xBFF080F8);
    OUTREG(index_reg,0x300);
    OUTREG(data_reg,0x83588058);
    OUTREG(index_reg,0x301);
    OUTREG(data_reg,0x84E08660);
    OUTREG(index_reg,0x302);
    OUTREG(data_reg,0xBFF88120);
    OUTREG(index_reg,0x400);
    OUTREG(data_reg,0x83188040);
    OUTREG(index_reg,0x401);
    OUTREG(data_reg,0x85008660);
    OUTREG(index_reg,0x402);
    OUTREG(data_reg,0xBFF88150);
    OUTREG(index_reg,0x500);
    OUTREG(data_reg,0x82D88030);
    OUTREG(index_reg,0x501);
    OUTREG(data_reg,0x85408640);
    OUTREG(index_reg,0x502);
    OUTREG(data_reg,0xBFF88180);
    OUTREG(index_reg,0x600);
    OUTREG(data_reg,0x82A08018);
    OUTREG(index_reg,0x601);
    OUTREG(data_reg,0x85808620);
    OUTREG(index_reg,0x602);
    OUTREG(data_reg,0xBFF081B8);
    OUTREG(index_reg,0x700);
    OUTREG(data_reg,0x82608010);
    OUTREG(index_reg,0x701);
    OUTREG(data_reg,0x85A08600);
    OUTREG(index_reg,0x702);
    OUTREG(data_reg,0x800081F0);
    OUTREG(index_reg,0x800);
    OUTREG(data_reg,0x8228BFF8);
    OUTREG(index_reg,0x801);
    OUTREG(data_reg,0x85E085E0);
    OUTREG(index_reg,0x802);
    OUTREG(data_reg,0xBFF88228);
    OUTREG(index_reg,0x10000);
    OUTREG(data_reg,0x82A8BF00);
    OUTREG(index_reg,0x10001);
    OUTREG(data_reg,0x82A08CC0);
    OUTREG(index_reg,0x10002);
    OUTREG(data_reg,0x8008BEF8);
    OUTREG(index_reg,0x10100);
    OUTREG(data_reg,0x81F0BF28);
    OUTREG(index_reg,0x10101);
    OUTREG(data_reg,0x83608CA0);
    OUTREG(index_reg,0x10102);
    OUTREG(data_reg,0x8018BED0);
    OUTREG(index_reg,0x10200);
    OUTREG(data_reg,0x8148BF38);
    OUTREG(index_reg,0x10201);
    OUTREG(data_reg,0x84408C80);
    OUTREG(index_reg,0x10202);
    OUTREG(data_reg,0x8008BEB8);
    OUTREG(index_reg,0x10300);
    OUTREG(data_reg,0x80B0BF78);
    OUTREG(index_reg,0x10301);
    OUTREG(data_reg,0x85008C20);
    OUTREG(index_reg,0x10302);
    OUTREG(data_reg,0x8020BEA0);
    OUTREG(index_reg,0x10400);
    OUTREG(data_reg,0x8028BF90);
    OUTREG(index_reg,0x10401);
    OUTREG(data_reg,0x85E08BC0);
    OUTREG(index_reg,0x10402);
    OUTREG(data_reg,0x8018BE90);
    OUTREG(index_reg,0x10500);
    OUTREG(data_reg,0xBFB8BFB0);
    OUTREG(index_reg,0x10501);
    OUTREG(data_reg,0x86C08B40);
    OUTREG(index_reg,0x10502);
    OUTREG(data_reg,0x8010BE90);
    OUTREG(index_reg,0x10600);
    OUTREG(data_reg,0xBF58BFC8);
    OUTREG(index_reg,0x10601);
    OUTREG(data_reg,0x87A08AA0);
    OUTREG(index_reg,0x10602);
    OUTREG(data_reg,0x8010BE98);
    OUTREG(index_reg,0x10700);
    OUTREG(data_reg,0xBF10BFF0);
    OUTREG(index_reg,0x10701);
    OUTREG(data_reg,0x886089E0);
    OUTREG(index_reg,0x10702);
    OUTREG(data_reg,0x8018BEB0);
    OUTREG(index_reg,0x10800);
    OUTREG(data_reg,0xBED8BFE8);
    OUTREG(index_reg,0x10801);
    OUTREG(data_reg,0x89408940);
    OUTREG(index_reg,0x10802);
    OUTREG(data_reg,0xBFE8BED8);
    OUTREG(index_reg,0x20000);
    OUTREG(data_reg,0x80008000);
    OUTREG(index_reg,0x20001);
    OUTREG(data_reg,0x90008000);
    OUTREG(index_reg,0x20002);
    OUTREG(data_reg,0x80008000);
    OUTREG(index_reg,0x20003);
    OUTREG(data_reg,0x80008000);
    OUTREG(index_reg,0x20100);
    OUTREG(data_reg,0x80108000);
    OUTREG(index_reg,0x20101);
    OUTREG(data_reg,0x8FE0BF70);
    OUTREG(index_reg,0x20102);
    OUTREG(data_reg,0xBFE880C0);
    OUTREG(index_reg,0x20103);
    OUTREG(data_reg,0x80008000);
    OUTREG(index_reg,0x20200);
    OUTREG(data_reg,0x8018BFF8);
    OUTREG(index_reg,0x20201);
    OUTREG(data_reg,0x8F80BF08);
    OUTREG(index_reg,0x20202);
    OUTREG(data_reg,0xBFD081A0);
    OUTREG(index_reg,0x20203);
    OUTREG(data_reg,0xBFF88000);
    OUTREG(index_reg,0x20300);
    OUTREG(data_reg,0x80188000);
    OUTREG(index_reg,0x20301);
    OUTREG(data_reg,0x8EE0BEC0);
    OUTREG(index_reg,0x20302);
    OUTREG(data_reg,0xBFB082A0);
    OUTREG(index_reg,0x20303);
    OUTREG(data_reg,0x80008000);
    OUTREG(index_reg,0x20400);
    OUTREG(data_reg,0x80188000);
    OUTREG(index_reg,0x20401);
    OUTREG(data_reg,0x8E00BEA0);
    OUTREG(index_reg,0x20402);
    OUTREG(data_reg,0xBF8883C0);
    OUTREG(index_reg,0x20403);
    OUTREG(data_reg,0x80008000);
    OUTREG(index_reg,0x20500);
    OUTREG(data_reg,0x80188000);
    OUTREG(index_reg,0x20501);
    OUTREG(data_reg,0x8D00BE90);
    OUTREG(index_reg,0x20502);
    OUTREG(data_reg,0xBF588500);
    OUTREG(index_reg,0x20503);
    OUTREG(data_reg,0x80008008);
    OUTREG(index_reg,0x20600);
    OUTREG(data_reg,0x80188000);
    OUTREG(index_reg,0x20601);
    OUTREG(data_reg,0x8BC0BE98);
    OUTREG(index_reg,0x20602);
    OUTREG(data_reg,0xBF308660);
    OUTREG(index_reg,0x20603);
    OUTREG(data_reg,0x80008008);
    OUTREG(index_reg,0x20700);
    OUTREG(data_reg,0x80108000);
    OUTREG(index_reg,0x20701);
    OUTREG(data_reg,0x8A80BEB0);
    OUTREG(index_reg,0x20702);
    OUTREG(data_reg,0xBF0087C0);
    OUTREG(index_reg,0x20703);
    OUTREG(data_reg,0x80008008);
    OUTREG(index_reg,0x20800);
    OUTREG(data_reg,0x80108000);
    OUTREG(index_reg,0x20801);
    OUTREG(data_reg,0x8920BED0);
    OUTREG(index_reg,0x20802);
    OUTREG(data_reg,0xBED08920);
    OUTREG(index_reg,0x20803);
    OUTREG(data_reg,0x80008010);
    OUTREG(index_reg,0x30000);
    OUTREG(data_reg,0x90008000);
    OUTREG(index_reg,0x30001);
    OUTREG(data_reg,0x80008000);
    OUTREG(index_reg,0x30100);
    OUTREG(data_reg,0x8FE0BF90);
    OUTREG(index_reg,0x30101);
    OUTREG(data_reg,0xBFF880A0);
    OUTREG(index_reg,0x30200);
    OUTREG(data_reg,0x8F60BF40);
    OUTREG(index_reg,0x30201);
    OUTREG(data_reg,0xBFE88180);
    OUTREG(index_reg,0x30300);
    OUTREG(data_reg,0x8EC0BF00);
    OUTREG(index_reg,0x30301);
    OUTREG(data_reg,0xBFC88280);
    OUTREG(index_reg,0x30400);
    OUTREG(data_reg,0x8DE0BEE0);
    OUTREG(index_reg,0x30401);
    OUTREG(data_reg,0xBFA083A0);
    OUTREG(index_reg,0x30500);
    OUTREG(data_reg,0x8CE0BED0);
    OUTREG(index_reg,0x30501);
    OUTREG(data_reg,0xBF7884E0);
    OUTREG(index_reg,0x30600);
    OUTREG(data_reg,0x8BA0BED8);
    OUTREG(index_reg,0x30601);
    OUTREG(data_reg,0xBF508640);
    OUTREG(index_reg,0x30700);
    OUTREG(data_reg,0x8A60BEE8);
    OUTREG(index_reg,0x30701);
    OUTREG(data_reg,0xBF2087A0);
    OUTREG(index_reg,0x30800);
    OUTREG(data_reg,0x8900BF00);
    OUTREG(index_reg,0x30801);
    OUTREG(data_reg,0xBF008900);
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

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {

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

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
	if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT | ATOM_DEVICE_TV_SUPPORT)
	    && info->ChipFamily >= CHIP_FAMILY_RV515 && info->ChipFamily <= CHIP_FAMILY_RV570) {
	    ErrorF("forcing TV scaler\n");
	    atom_rv515_force_tv_scaler(output->scrn, radeon_crtc);
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
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t reg = 0;

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
	if (IS_DCE32_VARIANT)
	    index = GetIndexIntoMasterTable(COMMAND, DAC1OutputControl);
	else {
	    if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
		index = GetIndexIntoMasterTable(COMMAND, TV1OutputControl);
	    else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
		index = GetIndexIntoMasterTable(COMMAND, CV1OutputControl);
	    else
		index = GetIndexIntoMasterTable(COMMAND, DAC1OutputControl);
	}
	break;
    case ENCODER_OBJECT_ID_INTERNAL_DAC2:
    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2:
	if (IS_DCE32_VARIANT)
	    index = GetIndexIntoMasterTable(COMMAND, DAC2OutputControl);
	else {
	    if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
		index = GetIndexIntoMasterTable(COMMAND, TV1OutputControl);
	    else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
		index = GetIndexIntoMasterTable(COMMAND, CV1OutputControl);
	    else
		index = GetIndexIntoMasterTable(COMMAND, DAC2OutputControl);
	}
	break;
    }

    switch (mode) {
    case DPMSModeOn:
	radeon_encoder->devices |= radeon_output->active_device;
	if (is_dig) {
	    atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_ENABLE_OUTPUT, 0, 0);
	    if (((radeon_output->ConnectorType == CONNECTOR_DISPLAY_PORT) ||
		 (radeon_output->ConnectorType == CONNECTOR_EDP)) &&
		(radeon_output->MonType == MT_DP)) {
		do_displayport_link_train(output);
		if (IS_DCE4_VARIANT)
		    atombios_output_dig_encoder_setup(output, ATOM_ENCODER_CMD_DP_VIDEO_ON);
	    }
	}
	else {
	    disp_data.ucAction = ATOM_ENABLE;
	    data.exec.index = index;
	    data.exec.dataSpace = (void *)&space;
	    data.exec.pspace = &disp_data;

	    /* workaround for DVOOutputControl on some RS690 systems */
	    if (radeon_encoder->encoder_id == ENCODER_OBJECT_ID_INTERNAL_DDI) {
		reg = INREG(RADEON_BIOS_3_SCRATCH);
		OUTREG(RADEON_BIOS_3_SCRATCH, reg & ~ATOM_S3_DFP2I_ACTIVE);
	    }
	    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS)
		ErrorF("Output %s enable success\n",
		       device_name[radeon_get_device_index(radeon_output->active_device)]);
	    else
		ErrorF("Output %s enable failed\n",
		       device_name[radeon_get_device_index(radeon_output->active_device)]);
	    if (radeon_encoder->encoder_id == ENCODER_OBJECT_ID_INTERNAL_DDI)
		OUTREG(RADEON_BIOS_3_SCRATCH, reg);
	}
	/* at least for TV atom fails to reassociate the correct crtc source at dpms on */
	if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
		atombios_set_output_crtc_source(output);
	break;
    case DPMSModeStandby:
    case DPMSModeSuspend:
    case DPMSModeOff:
	radeon_encoder->devices &= ~(radeon_output->active_device);
	if (!radeon_encoder->devices) {
	    if (is_dig) {
		atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_DISABLE_OUTPUT, 0, 0);
		if (((radeon_output->ConnectorType == CONNECTOR_DISPLAY_PORT) ||
		     (radeon_output->ConnectorType == CONNECTOR_EDP)) &&
		    (radeon_output->MonType == MT_DP)) {
		    if (IS_DCE4_VARIANT)
			atombios_output_dig_encoder_setup(output, ATOM_ENCODER_CMD_DP_VIDEO_OFF);
		}
	    } else {
		disp_data.ucAction = ATOM_DISABLE;
		data.exec.index = index;
		data.exec.dataSpace = (void *)&space;
		data.exec.pspace = &disp_data;

		if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data)
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

union crtc_source_param {
    SELECT_CRTC_SOURCE_PS_ALLOCATION v1;
    SELECT_CRTC_SOURCE_PARAMETERS_V2 v2;
};

void
atombios_set_output_crtc_source(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONCrtcPrivatePtr radeon_crtc = output->crtc->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
    AtomBiosArgRec data;
    unsigned char *space;
    union crtc_source_param args;
    int index = GetIndexIntoMasterTable(COMMAND, SelectCRTC_Source);
    int major, minor;

    if (radeon_encoder == NULL)
	return;

    memset(&args, 0, sizeof(args));

    atombios_get_command_table_version(info->atomBIOS, index, &major, &minor);

    /*ErrorF("select crtc source table is %d %d\n", major, minor);*/

    switch(major) {
    case 1:
	switch(minor) {
	case 0:
	case 1:
	default:
	    if (IS_AVIVO_VARIANT)
		args.v1.ucCRTC = radeon_crtc->crtc_id;
	    else {
		if (radeon_encoder->encoder_id == ENCODER_OBJECT_ID_INTERNAL_DAC1)
		    args.v1.ucCRTC = radeon_crtc->crtc_id;
		else
		    args.v1.ucCRTC = radeon_crtc->crtc_id << 2;
	    }
	    switch (radeon_encoder->encoder_id) {
	    case ENCODER_OBJECT_ID_INTERNAL_TMDS1:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_TMDS1:
		args.v1.ucDevice = ATOM_DEVICE_DFP1_INDEX;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_LVDS:
	    case ENCODER_OBJECT_ID_INTERNAL_LVTM1:
		if (radeon_output->active_device & ATOM_DEVICE_LCD1_SUPPORT)
		    args.v1.ucDevice = ATOM_DEVICE_LCD1_INDEX;
		else
		    args.v1.ucDevice = ATOM_DEVICE_DFP3_INDEX;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_DVO1:
	    case ENCODER_OBJECT_ID_INTERNAL_DDI:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DVO1:
		args.v1.ucDevice = ATOM_DEVICE_DFP2_INDEX;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_DAC1:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1:
		if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
		    args.v1.ucDevice = ATOM_DEVICE_TV1_INDEX;
		else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
		    args.v1.ucDevice = ATOM_DEVICE_CV_INDEX;
		else
		    args.v1.ucDevice = ATOM_DEVICE_CRT1_INDEX;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_DAC2:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2:
		if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
		    args.v1.ucDevice = ATOM_DEVICE_TV1_INDEX;
		else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
		    args.v1.ucDevice = ATOM_DEVICE_CV_INDEX;
		else
		    args.v1.ucDevice = ATOM_DEVICE_CRT2_INDEX;
		break;
	    }
	    /*ErrorF("device sourced: 0x%x\n", args.v1.ucDevice);*/
	    break;
	case 2:
	    args.v2.ucCRTC = radeon_crtc->crtc_id;
	    args.v2.ucEncodeMode = atombios_get_encoder_mode(output);
	    switch (radeon_encoder->encoder_id) {
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY1:
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY2:
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA:
 		switch (radeon_output->dig_encoder) {
 		case 0:
 		    args.v2.ucEncoderID = ASIC_INT_DIG1_ENCODER_ID;
 		    break;
 		case 1:
 		    args.v2.ucEncoderID = ASIC_INT_DIG2_ENCODER_ID;
 		    break;
 		case 2:
 		    args.v2.ucEncoderID = ASIC_INT_DIG3_ENCODER_ID;
 		    break;
 		case 3:
 		    args.v2.ucEncoderID = ASIC_INT_DIG4_ENCODER_ID;
 		    break;
 		case 4:
 		    args.v2.ucEncoderID = ASIC_INT_DIG5_ENCODER_ID;
 		    break;
 		case 5:
 		    args.v2.ucEncoderID = ASIC_INT_DIG6_ENCODER_ID;
 		    break;
 		}
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DVO1:
		args.v2.ucEncoderID = ASIC_INT_DVO_ENCODER_ID;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC1:
		if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
		    args.v2.ucEncoderID = ASIC_INT_TV_ENCODER_ID;
		else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
		    args.v2.ucEncoderID = ASIC_INT_TV_ENCODER_ID;
		else
		    args.v2.ucEncoderID = ASIC_INT_DAC1_ENCODER_ID;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_KLDSCP_DAC2:
		if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT))
		    args.v2.ucEncoderID = ASIC_INT_TV_ENCODER_ID;
		else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT))
		    args.v2.ucEncoderID = ASIC_INT_TV_ENCODER_ID;
		else
		    args.v2.ucEncoderID = ASIC_INT_DAC2_ENCODER_ID;
		break;
	    }
	    /*ErrorF("device sourced: 0x%x\n", args.v2.ucEncoderID);*/
	    break;
	}
	break;
    default:
	ErrorF("Unknown table version\n");
	exit(-1);
    }

    data.exec.pspace = &args;
    data.exec.index = index;
    data.exec.dataSpace = (void *)&space;

    if (RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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
    if (!(radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT | ATOM_DEVICE_TV_SUPPORT))) {
	if (IS_AVIVO_VARIANT && (mode->Flags & V_INTERLACE))
	    OUTREG(AVIVO_D1MODE_DATA_FORMAT + radeon_crtc->crtc_offset, AVIVO_D1MODE_INTERLEAVE_EN);
    }

    if (IS_DCE32_VARIANT &&
	(!IS_DCE4_VARIANT) &&
	(radeon_output->active_device & (ATOM_DEVICE_DFP_SUPPORT))) {
	radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
	if (radeon_encoder == NULL)
	    return;
	/* XXX: need to sort out why transmitter control table sometimes sets this to a
	 * different golden value.
	 */
	if (radeon_encoder->encoder_id == ENCODER_OBJECT_ID_INTERNAL_UNIPHY2) {
	    OUTREG(0x7ec4, 0x00824002);
	}
    }
}

void
atombios_pick_dig_encoder(xf86OutputPtr output)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(output->scrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    radeon_encoder_ptr radeon_encoder = NULL;
    Bool is_lvtma = FALSE;
    int i, mode;
    uint32_t dig_enc_use_mask = 0;

    /* non digital encoders don't need a dig block */
    mode = atombios_get_encoder_mode(output);
    if (mode == ATOM_ENCODER_MODE_CRT ||
        mode == ATOM_ENCODER_MODE_TV ||
        mode == ATOM_ENCODER_MODE_CV)
        return;

    if (IS_DCE4_VARIANT) {
        radeon_encoder = radeon_get_encoder(output);

	if (IS_DCE41_VARIANT) {
	    if (radeon_output->linkb)
		radeon_output->dig_encoder = 1;
	    else
		radeon_output->dig_encoder = 0;
	} else {
	    switch (radeon_encoder->encoder_id) {
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY:
		if (radeon_output->linkb)
		    radeon_output->dig_encoder = 1;
		else
		    radeon_output->dig_encoder = 0;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY1:
		if (radeon_output->linkb)
		    radeon_output->dig_encoder = 3;
		else
		    radeon_output->dig_encoder = 2;
		break;
	    case ENCODER_OBJECT_ID_INTERNAL_UNIPHY2:
		if (radeon_output->linkb)
		    radeon_output->dig_encoder = 5;
		else
		    radeon_output->dig_encoder = 4;
		break;
	    default:
		ErrorF("Unknown encoder\n");
		break;
	    }
	}
	return;
    }

    if (IS_DCE32_VARIANT) {
        RADEONCrtcPrivatePtr radeon_crtc = output->crtc->driver_private;
        radeon_output->dig_encoder = radeon_crtc->crtc_id;
        return;
    }

    for (i = 0; i < xf86_config->num_output; i++) {
        xf86OutputPtr test = xf86_config->output[i];
        RADEONOutputPrivatePtr radeon_test = test->driver_private;
        radeon_encoder = radeon_get_encoder(test);

        if (!radeon_encoder || !test->crtc)
            continue;

        if (output == test && radeon_encoder->encoder_id == ENCODER_OBJECT_ID_INTERNAL_KLDSCP_LVTMA)
            is_lvtma = TRUE;
        if (output != test && (radeon_test->dig_encoder >= 0))
            dig_enc_use_mask |= (1 << radeon_test->dig_encoder);

    }
    if (is_lvtma) {
        if (dig_enc_use_mask & 0x2)
            ErrorF("Need digital encoder 2 for LVTMA and it isn't free - stealing\n");
        radeon_output->dig_encoder = 1;
        return;
    }
    if (!(dig_enc_use_mask & 1))
        radeon_output->dig_encoder = 0;
    else
        radeon_output->dig_encoder = 1;
}
void
atombios_output_mode_set(xf86OutputPtr output,
			 DisplayModePtr mode,
			 DisplayModePtr adjusted_mode)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    if (radeon_encoder == NULL)
	return;

    radeon_output->pixel_clock = adjusted_mode->Clock;
    atombios_output_overscan_setup(output, mode, adjusted_mode);
    atombios_output_scaler_setup(output);
    atombios_set_output_crtc_source(output);

    if (IS_AVIVO_VARIANT && !IS_DCE4_VARIANT) {
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
	/* setup and enable the encoder and transmitter */
	if (IS_DCE4_VARIANT) {
	    atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_DISABLE, 0, 0);
	    atombios_output_dig_encoder_setup(output, ATOM_ENCODER_CMD_SETUP);
	    atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_INIT, 0, 0);
	    atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_ENABLE, 0, 0);
	} else {
	    atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_DISABLE, 0, 0);
	    atombios_output_dig_encoder_setup(output, ATOM_DISABLE);
	    atombios_output_dig_encoder_setup(output, ATOM_ENABLE);

	    atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_INIT, 0, 0);
	    atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_SETUP, 0, 0);
	    atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_ENABLE, 0, 0);
	}
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
	if (radeon_output->devices & (ATOM_DEVICE_TV_SUPPORT | ATOM_DEVICE_CV_SUPPORT)) {
		if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT | ATOM_DEVICE_CV_SUPPORT))
			atombios_output_tv_setup(output, ATOM_ENABLE);
		else
			atombios_output_tv_setup(output, ATOM_DISABLE);
	}
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

    if (RHDAtomBiosFunc(atomBIOS->pScrn, atomBIOS, ATOMBIOS_EXEC, &data) == ATOM_SUCCESS) {
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
    RADEONSavePtr save = info->ModeReg;

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
	    save->bios_0_scratch = INREG(R600_BIOS_0_SCRATCH);
	else
	    save->bios_0_scratch = INREG(RADEON_BIOS_0_SCRATCH);
	/*ErrorF("DAC connect %08X\n", (unsigned int)save->bios_0_scratch);*/

	if (radeon_output->devices & ATOM_DEVICE_CRT1_SUPPORT) {
	    if (save->bios_0_scratch & ATOM_S0_CRT1_MASK)
		MonType = MT_CRT;
	} else if (radeon_output->devices & ATOM_DEVICE_CRT2_SUPPORT) {
	    if (save->bios_0_scratch & ATOM_S0_CRT2_MASK)
		MonType = MT_CRT;
	} else if (radeon_output->devices & ATOM_DEVICE_CV_SUPPORT) {
	    if (save->bios_0_scratch & (ATOM_S0_CV_MASK | ATOM_S0_CV_MASK_A))
		MonType = MT_CV;
	} else if (radeon_output->devices & ATOM_DEVICE_TV1_SUPPORT) {
	    if (save->bios_0_scratch & (ATOM_S0_TV1_COMPOSITE | ATOM_S0_TV1_COMPOSITE_A))
		MonType = MT_CTV;
	    else if (save->bios_0_scratch & (ATOM_S0_TV1_SVIDEO | ATOM_S0_TV1_SVIDEO_A))
		MonType = MT_STV;
	}
    }

    return MonType;
}


static inline int atom_dp_get_encoder_id(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int ret = 0;
    if (radeon_output->dig_encoder)
        ret |= ATOM_DP_CONFIG_DIG2_ENCODER;
    else
        ret |= ATOM_DP_CONFIG_DIG1_ENCODER;
    if (radeon_output->linkb)
        ret |= ATOM_DP_CONFIG_LINK_B;
    else
        ret |= ATOM_DP_CONFIG_LINK_A;
    return ret;
}

union aux_channel_transaction {
    PROCESS_AUX_CHANNEL_TRANSACTION_PS_ALLOCATION v1;
    PROCESS_AUX_CHANNEL_TRANSACTION_PARAMETERS_V2 v2;
};

Bool
RADEONProcessAuxCH(xf86OutputPtr output, uint8_t *req_bytes, uint8_t num_bytes,
		   uint8_t *read_byte, uint8_t read_buf_len, uint8_t delay)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    RADEONInfoPtr info       = RADEONPTR(output->scrn);
    union aux_channel_transaction args;
    AtomBiosArgRec data;
    unsigned char *space;
    unsigned char *base;
    int retry_count = 0;

    memset(&args, 0, sizeof(args));
    if (info->atomBIOS->fbBase)
	base = info->FB + info->atomBIOS->fbBase;
    else if (info->atomBIOS->scratchBase)
	base = (unsigned char *)info->atomBIOS->scratchBase;
    else
	return FALSE;

retry:
    memcpy(base, req_bytes, num_bytes);

    args.v1.lpAuxRequest = 0;
    args.v1.lpDataOut = 16;
    args.v1.ucDataOutLen = 0;
    args.v1.ucChannelID = radeon_output->ucI2cId;
    args.v1.ucDelay = delay / 10; /* 10 usec */
    if (IS_DCE4_VARIANT)
	args.v2.ucHPD_ID = radeon_output->hpd_id;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, ProcessAuxChannelTransaction);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &args;

    RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data);
    if (args.v1.ucReplyStatus && !args.v1.ucDataOutLen) {
	if (args.v1.ucReplyStatus == 0x20 && retry_count++ < 10)
		goto retry;
	ErrorF("failed to get auxch %02x%02x %02x %02x %02x after %d retries\n",
	       req_bytes[1], req_bytes[0], req_bytes[2], req_bytes[3], args.v1.ucReplyStatus, retry_count);
	return FALSE;
    }
    if (args.v1.ucDataOutLen && read_byte && read_buf_len) {
	if (read_buf_len < args.v1.ucDataOutLen) {
	    ErrorF("%s: Buffer too small for return answer %d %d\n", __func__, read_buf_len, args.v1.ucDataOutLen);
	    return FALSE;
	}
	{
	    int len = read_buf_len < args.v1.ucDataOutLen ? read_buf_len : args.v1.ucDataOutLen;
	    memcpy(read_byte, base+16, len);
	}
    }
    return TRUE;
}

static int
RADEONDPEncoderService(xf86OutputPtr output, int action, uint8_t ucconfig, uint8_t lane_num)
{
    RADEONInfoPtr info = RADEONPTR(output->scrn);
    DP_ENCODER_SERVICE_PARAMETERS args;
    AtomBiosArgRec data;
    unsigned char *space;

    memset(&args, 0, sizeof(args));

    args.ucLinkClock = 0;
    args.ucConfig = ucconfig;
    args.ucAction = action;
    args.ucLaneNum = lane_num;
    args.ucStatus = 0;

    data.exec.index = GetIndexIntoMasterTable(COMMAND, DPEncoderService);
    data.exec.dataSpace = (void *)&space;
    data.exec.pspace = &args;

    RHDAtomBiosFunc(info->atomBIOS->pScrn, info->atomBIOS, ATOMBIOS_EXEC, &data);

    ErrorF("%s: %d %d\n", __func__, action, args.ucStatus);
    return args.ucStatus;
}

int RADEON_DP_GetSinkType(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    return RADEONDPEncoderService(output, ATOM_DP_ACTION_GET_SINK_TYPE, radeon_output->ucI2cId, 0);
}

static Bool atom_dp_aux_native_write(xf86OutputPtr output, uint16_t address,
				     uint8_t send_bytes, uint8_t *send)
{
    uint8_t msg[20];
    uint8_t msg_len, dp_msg_len;
    int ret;

    dp_msg_len = 4;
    msg[0] = address;
    msg[1] = address >> 8;
    msg[2] = AUX_NATIVE_WRITE << 4;
    dp_msg_len += send_bytes;
    msg[3] = (dp_msg_len << 4)| (send_bytes - 1);

    if (0)
	ErrorF("writing %02x %02x %02x, %d, %d\n", msg[0], msg[1], msg[3], send_bytes, dp_msg_len);
    if (send_bytes > 16)
	return FALSE;

    memcpy(&msg[4], send, send_bytes);
    msg_len = 4 + send_bytes;
    ret = RADEONProcessAuxCH(output, msg, msg_len, NULL, 0, 0);
    return ret;
}

static Bool atom_dp_aux_native_read(xf86OutputPtr output, uint16_t address,
				    uint8_t delay,
				    uint8_t expected_bytes, uint8_t *read_p)
{
    uint8_t msg[20];
    uint8_t msg_len, dp_msg_len;
    int ret;

    msg_len = 4;
    dp_msg_len = 4;
    msg[0] = address;
    msg[1] = address >> 8;
    msg[2] = AUX_NATIVE_READ << 4;
    msg[3] = (dp_msg_len) << 4;
    msg[3] |= expected_bytes - 1;

    if (0)
	ErrorF("reading %02x %02x %02x, %d, %d\n", msg[0], msg[1], msg[3], expected_bytes, dp_msg_len);
    ret = RADEONProcessAuxCH(output, msg, msg_len, read_p, expected_bytes, delay);
    return ret;
}

/* fill out the DPCD structure */
void RADEON_DP_GetDPCD(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    uint8_t msg[25];
    int ret;

    ret = atom_dp_aux_native_read(output, DP_DPCD_REV, 0, 8, msg);
    if (ret) {
	memcpy(radeon_output->dpcd, msg, 8);
	if (0) {
	    int i;
	    ErrorF("DPCD: ");
	    for (i = 0; i < 8; i++)
		ErrorF("%02x ", radeon_output->dpcd[i]);
	    ErrorF("\n");
	}
	ret = atom_dp_aux_native_read(output, DP_LINK_BW_SET, 0, 2, msg);
	if (0) {
	    ErrorF("0x200: %02x %02x\n", msg[0], msg[1]);
	}
	return;
    }
    radeon_output->dpcd[0] = 0;
    return;
}


enum dp_aux_i2c_mode {
    dp_aux_i2c_start,
    dp_aux_i2c_write,
    dp_aux_i2c_read,
    dp_aux_i2c_stop,
};


static Bool atom_dp_aux_i2c_transaction(xf86OutputPtr output, uint16_t address,
				       enum dp_aux_i2c_mode mode,
				       uint8_t write_byte, uint8_t *read_byte)
{
    uint8_t msg[8], msg_len, dp_msg_len;
    int ret;
    int auxch_cmd = 0;

    memset(msg, 0, 8);

    if (mode != dp_aux_i2c_stop)
	auxch_cmd = AUX_I2C_MOT;

    if (address & 1)
	auxch_cmd |= AUX_I2C_READ;
    else
    	auxch_cmd |= AUX_I2C_WRITE;

    msg[2] = auxch_cmd << 4;

    msg[4] = 0;
    msg[0] = (address >> 1);
    msg[1] = (address >> 9);

    msg_len = 4;
    dp_msg_len = 3;
    switch (mode) {
    case dp_aux_i2c_read:
	/* bottom bits is byte count - 1 so for 1 byte == 0 */
	dp_msg_len += 1;
	break;
    case dp_aux_i2c_write:
	dp_msg_len += 2;
	msg[4] = write_byte;
	msg_len++;
	break;
    default:
	break;
    }
    msg[3] = dp_msg_len << 4;

    ret = RADEONProcessAuxCH(output, msg, msg_len, read_byte, 1, 0);
    return ret;
}

static Bool
atom_dp_i2c_address(I2CDevPtr dev, I2CSlaveAddr addr)
{
    I2CBusPtr bus = dev->pI2CBus;
    xf86OutputPtr output = bus->DriverPrivate.ptr;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int ret;

    radeon_output->dp_i2c_addr = addr;
    radeon_output->dp_i2c_running = TRUE;

    /* call i2c start */
    ret = atom_dp_aux_i2c_transaction(output, radeon_output->dp_i2c_addr,
				      dp_aux_i2c_start, 0, NULL);

    return ret;
}
static Bool
atom_dp_i2c_start(I2CBusPtr bus, int timeout)
{
    ErrorF("%s\n", __func__);
    return TRUE;
}

static void
atom_dp_i2c_stop(I2CDevPtr dev)
{
    I2CBusPtr bus = dev->pI2CBus;
    xf86OutputPtr output = bus->DriverPrivate.ptr;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if (radeon_output->dp_i2c_running)
	atom_dp_aux_i2c_transaction(output, radeon_output->dp_i2c_addr,
				    dp_aux_i2c_stop, 0, NULL);
    radeon_output->dp_i2c_running = FALSE;
}


static Bool
atom_dp_i2c_put_byte(I2CDevPtr dev, I2CByte byte)
{
    I2CBusPtr bus = dev->pI2CBus;
    xf86OutputPtr output = bus->DriverPrivate.ptr;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    Bool ret;

    ret = (atom_dp_aux_i2c_transaction(output, radeon_output->dp_i2c_addr,
				       dp_aux_i2c_write, byte, NULL));
    return ret;
}

static Bool
atom_dp_i2c_get_byte(I2CDevPtr dev, I2CByte *byte_ret, Bool last)
{
    I2CBusPtr bus = dev->pI2CBus;
    xf86OutputPtr output = bus->DriverPrivate.ptr;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    Bool ret;

    ret = (atom_dp_aux_i2c_transaction(output, radeon_output->dp_i2c_addr,
				       dp_aux_i2c_read, 0, byte_ret));
    return ret;
}

Bool
RADEON_DP_I2CInit(ScrnInfoPtr pScrn, I2CBusPtr *bus_ptr, char *name, xf86OutputPtr output)
{
    I2CBusPtr pI2CBus;

    pI2CBus = xf86CreateI2CBusRec();
    if (!pI2CBus) return FALSE;

    pI2CBus->BusName = name;
    pI2CBus->scrnIndex = pScrn->scrnIndex;
    pI2CBus->I2CGetByte = atom_dp_i2c_get_byte;
    pI2CBus->I2CPutByte = atom_dp_i2c_put_byte;
    pI2CBus->I2CAddress = atom_dp_i2c_address;
    pI2CBus->I2CStart = atom_dp_i2c_start;
    pI2CBus->I2CStop = atom_dp_i2c_stop;
    pI2CBus->DriverPrivate.ptr = output;

    /*
     * These were set incorrectly in the server pre-1.3, Having
     * duplicate settings is sub-optimal, but this lets the driver
     * work with older servers
     */
    pI2CBus->ByteTimeout = 2200; /* VESA DDC spec 3 p. 43 (+10 %) */
    pI2CBus->StartTimeout = 550;
    pI2CBus->BitTimeout = 40;
    pI2CBus->AcknTimeout = 40;
    pI2CBus->RiseFallTime = 20;

    if (!xf86I2CBusInit(pI2CBus))
	return FALSE;

    *bus_ptr = pI2CBus;
    return TRUE;
}


static uint8_t dp_link_status(uint8_t link_status[DP_LINK_STATUS_SIZE], int r)
{
    return link_status[r - DP_LANE0_1_STATUS];
}

static uint8_t dp_get_lane_status(uint8_t link_status[DP_LINK_STATUS_SIZE], int lane)
{
    int i = DP_LANE0_1_STATUS + (lane >> 1);
    int s = (lane & 1) * 4;
    uint8_t l = dp_link_status(link_status, i);
    return (l >> s) & 0xf;
}

static Bool dp_clock_recovery_ok(uint8_t link_status[DP_LINK_STATUS_SIZE], int lane_count)
{
    int lane;

    uint8_t lane_status;

    for (lane = 0; lane < lane_count; lane++) {
	lane_status = dp_get_lane_status(link_status, lane);
	if ((lane_status & DP_LANE_CR_DONE) == 0)
	    return FALSE;
    }
    return TRUE;
}


/* Check to see if channel eq is done on all channels */
#define CHANNEL_EQ_BITS (DP_LANE_CR_DONE|\
			 DP_LANE_CHANNEL_EQ_DONE|\
			 DP_LANE_SYMBOL_LOCKED)
static Bool
dp_channel_eq_ok(uint8_t link_status[DP_LINK_STATUS_SIZE], int lane_count)
{
    uint8_t lane_align;
    uint8_t lane_status;
    int lane;

    lane_align = dp_link_status(link_status,
				DP_LANE_ALIGN_STATUS_UPDATED);
    if ((lane_align & DP_INTERLANE_ALIGN_DONE) == 0)
	return FALSE;
    for (lane = 0; lane < lane_count; lane++) {
	lane_status = dp_get_lane_status(link_status, lane);
	if ((lane_status & CHANNEL_EQ_BITS) != CHANNEL_EQ_BITS)
	    return FALSE;
    }
    return TRUE;
}

/*
 * Fetch AUX CH registers 0x202 - 0x207 which contain
 * link status information
 */
static Bool
atom_dp_get_link_status(xf86OutputPtr output,
			  uint8_t link_status[DP_LINK_STATUS_SIZE])
{
    ScrnInfoPtr pScrn = output->scrn;
    int ret;
    ret = atom_dp_aux_native_read(output, DP_LANE0_1_STATUS, 100,
				  DP_LINK_STATUS_SIZE, link_status);
    if (!ret) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "dp link status failed\n");
	return FALSE;
    }
    ErrorF("link status %02x %02x %02x %02x %02x %02x\n", link_status[0], link_status[1],
	   link_status[2], link_status[3], link_status[4], link_status[5]);

    return TRUE;
}

static uint8_t
dp_get_adjust_request_voltage(uint8_t link_status[DP_LINK_STATUS_SIZE],
			      int lane)

{
    int     i = DP_ADJUST_REQUEST_LANE0_1 + (lane >> 1);
    int     s = ((lane & 1) ?
                 DP_ADJUST_VOLTAGE_SWING_LANE1_SHIFT :
                 DP_ADJUST_VOLTAGE_SWING_LANE0_SHIFT);
    uint8_t l = dp_link_status(link_status, i);

    return ((l >> s) & 3) << DP_TRAIN_VOLTAGE_SWING_SHIFT;
}

static uint8_t
dp_get_adjust_request_pre_emphasis(uint8_t link_status[DP_LINK_STATUS_SIZE],
				   int lane)
{
    int     i = DP_ADJUST_REQUEST_LANE0_1 + (lane >> 1);
    int     s = ((lane & 1) ?
                 DP_ADJUST_PRE_EMPHASIS_LANE1_SHIFT :
                 DP_ADJUST_PRE_EMPHASIS_LANE0_SHIFT);
    uint8_t l = dp_link_status(link_status, i);

    return ((l >> s) & 3) << DP_TRAIN_PRE_EMPHASIS_SHIFT;
}

static char     *voltage_names[] = {
        "0.4V", "0.6V", "0.8V", "1.2V"
};
static char     *pre_emph_names[] = {
        "0dB", "3.5dB", "6dB", "9.5dB"
};

/*
 * These are source-specific values; current Intel hardware supports
 * a maximum voltage of 800mV and a maximum pre-emphasis of 6dB
 */
#define DP_VOLTAGE_MAX         DP_TRAIN_VOLTAGE_SWING_1200

static uint8_t
dp_pre_emphasis_max(uint8_t voltage_swing)
{
    switch (voltage_swing & DP_TRAIN_VOLTAGE_SWING_MASK) {
    case DP_TRAIN_VOLTAGE_SWING_400:
        return DP_TRAIN_PRE_EMPHASIS_6;
    case DP_TRAIN_VOLTAGE_SWING_600:
        return DP_TRAIN_PRE_EMPHASIS_6;
    case DP_TRAIN_VOLTAGE_SWING_800:
        return DP_TRAIN_PRE_EMPHASIS_3_5;
    case DP_TRAIN_VOLTAGE_SWING_1200:
    default:
        return DP_TRAIN_PRE_EMPHASIS_0;
    }
}

static void dp_set_training(xf86OutputPtr output, uint8_t training)
{
    atom_dp_aux_native_write(output, DP_TRAINING_PATTERN_SET, 1, &training);
}

static void dp_set_power(xf86OutputPtr output, uint8_t power_state)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;

    if (radeon_output->dpcd[0] >= 0x11) {
	atom_dp_aux_native_write(output, 0x600, 1, &power_state);
    }
}

static void
dp_get_adjust_train(xf86OutputPtr output,
		      uint8_t link_status[DP_LINK_STATUS_SIZE],
		      int lane_count,
		      uint8_t train_set[4])
{
    ScrnInfoPtr pScrn = output->scrn;
    uint8_t v = 0;
    uint8_t p = 0;
    int lane;

    for (lane = 0; lane < lane_count; lane++) {
	uint8_t this_v = dp_get_adjust_request_voltage(link_status, lane);
	uint8_t this_p = dp_get_adjust_request_pre_emphasis(link_status, lane);

	if (0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "requested signal parameters: lane %d voltage %s pre_emph %s\n",
		       lane,
		       voltage_names[this_v >> DP_TRAIN_VOLTAGE_SWING_SHIFT],
		       pre_emph_names[this_p >> DP_TRAIN_PRE_EMPHASIS_SHIFT]);
	}
	if (this_v > v)
	    v = this_v;
	if (this_p > p)
	    p = this_p;
    }

    if (v >= DP_VOLTAGE_MAX)
	v = DP_VOLTAGE_MAX | DP_TRAIN_MAX_SWING_REACHED;

    if (p >= dp_pre_emphasis_max(v))
	p = dp_pre_emphasis_max(v) | DP_TRAIN_MAX_PRE_EMPHASIS_REACHED;

    if (0) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "using signal parameters: voltage %s pre_emph %s\n",
		   voltage_names[(v & DP_TRAIN_VOLTAGE_SWING_MASK) >> DP_TRAIN_VOLTAGE_SWING_SHIFT],
		   pre_emph_names[(p & DP_TRAIN_PRE_EMPHASIS_MASK) >> DP_TRAIN_PRE_EMPHASIS_SHIFT]);
    }
    for (lane = 0; lane < 4; lane++)
	train_set[lane] = v | p;
}

static int radeon_dp_max_lane_count(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int max_lane_count = 4;

    if (radeon_output->dpcd[0] >= 0x11) {
	max_lane_count = radeon_output->dpcd[2] & 0x1f;
	switch(max_lane_count) {
	case 1: case 2: case 4:
	    break;
	default:
	    max_lane_count = 4;
	}
    }
    return max_lane_count;
}

Bool radeon_dp_mode_fixup(xf86OutputPtr output, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
	RADEONOutputPrivatePtr radeon_output = output->driver_private;
	int clock = adjusted_mode->Clock;

	radeon_output->dp_lane_count = dp_lanes_for_mode_clock(output, clock);
	radeon_output->dp_clock = dp_link_clock_for_mode_clock(output, clock);
	if (!radeon_output->dp_lane_count || !radeon_output->dp_clock)
		return FALSE;
	return TRUE;
}

static void dp_update_dpvs_emph(xf86OutputPtr output, uint8_t train_set[4])
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int i;
    for (i = 0; i < radeon_output->dp_lane_count; i++)
	atombios_output_dig_transmitter_setup(output, ATOM_TRANSMITTER_ACTION_SETUP_VSEMPH, i, train_set[i]);

    atom_dp_aux_native_write(output, DP_TRAINING_LANE0_SET, radeon_output->dp_lane_count, train_set);
}

static void do_displayport_link_train(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    int enc_id = atom_dp_get_encoder_id(output);
    Bool clock_recovery;
    uint8_t link_status[DP_LINK_STATUS_SIZE];
    uint8_t tries, voltage, ss_cntl;
    uint8_t train_set[4];
    int i;
    Bool channel_eq;
    uint8_t dp_link_configuration[DP_LINK_CONFIGURATION_SIZE];

    memset(train_set, 0, 4);

    /* set up link configuration */
    memset(dp_link_configuration, 0, DP_LINK_CONFIGURATION_SIZE);

    if (radeon_output->dp_clock == 27000)
	dp_link_configuration[0] = DP_LINK_BW_2_7;
    else
	dp_link_configuration[0] = DP_LINK_BW_1_62;
    dp_link_configuration[1] = radeon_output->dp_lane_count;

    if (radeon_output->dpcd[0] >= 0x11) {
	dp_link_configuration[1] |= DP_LANE_COUNT_ENHANCED_FRAME_EN;
    }

    /* power up to D0 */
    dp_set_power(output, DP_SET_POWER_D0);

    /* disable training */
    dp_set_training(output, DP_TRAINING_PATTERN_DISABLE);

    /* write link rate / num / eh framing */
    atom_dp_aux_native_write(output, DP_LINK_BW_SET, 2,
			     dp_link_configuration);

    /* write ss cntl */
    ss_cntl = 0;
    atom_dp_aux_native_write(output, DP_DOWNSPREAD_CTRL, 1,
			     &ss_cntl);

    /* start local training start */
    if (IS_DCE4_VARIANT) {
	atombios_output_dig_encoder_setup(output, ATOM_ENCODER_CMD_DP_LINK_TRAINING_START);
	atombios_output_dig_encoder_setup(output, ATOM_ENCODER_CMD_DP_LINK_TRAINING_PATTERN1);
    } else {
	RADEONDPEncoderService(output, ATOM_DP_ACTION_TRAINING_START, enc_id, 0);
	RADEONDPEncoderService(output, ATOM_DP_ACTION_TRAINING_PATTERN_SEL, enc_id, 0);
    }

    usleep(400);
    dp_set_training(output, DP_TRAINING_PATTERN_1);
    dp_update_dpvs_emph(output, train_set);

    /* loop around doing configuration reads and DP encoder setups */
    clock_recovery = FALSE;
    tries = 0;
    voltage = 0xff;
    for (;;) {
      	usleep(100);
	if (!atom_dp_get_link_status(output, link_status))
	    break;

	if (dp_clock_recovery_ok(link_status, radeon_output->dp_lane_count)) {
	    clock_recovery = TRUE;
	    break;
	}

	for (i = 0; i < radeon_output->dp_lane_count; i++)
	    if ((train_set[i] & DP_TRAIN_MAX_SWING_REACHED) == 0)
		break;
	if (i == radeon_output->dp_lane_count) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "clock recovery reached max voltage\n");
	    break;
	}

	/* Check to see if we've tried the same voltage 5 times */
	if ((train_set[0] & DP_TRAIN_VOLTAGE_SWING_MASK) == voltage) {
	    ++tries;
	    if (tries == 5) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "clock recovery tried 5 times\n");
		break;
	    }
	} else
	    tries = 0;

	voltage = train_set[0] & DP_TRAIN_VOLTAGE_SWING_MASK;

        dp_get_adjust_train(output, link_status, radeon_output->dp_lane_count, train_set);
	dp_update_dpvs_emph(output, train_set);

    }

    if (!clock_recovery)
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "clock recovery failed\n");

    /* channel equalization */
    tries = 0;
    channel_eq = FALSE;
    dp_set_training(output, DP_TRAINING_PATTERN_2);
    if (IS_DCE4_VARIANT)
	atombios_output_dig_encoder_setup(output, ATOM_ENCODER_CMD_DP_LINK_TRAINING_PATTERN2);
    else
	RADEONDPEncoderService(output, ATOM_DP_ACTION_TRAINING_PATTERN_SEL, enc_id, 1);

    for (;;) {
	usleep(400);
	if (!atom_dp_get_link_status(output, link_status))
	    break;

	if (dp_channel_eq_ok(link_status, radeon_output->dp_lane_count)) {
	    channel_eq = TRUE;
	    break;
	}

	/* Try 5 times */
	if (tries > 5) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "channel eq failed: 5 tries\n");
	    break;
	}

	/* Compute new train_set as requested by target */
        dp_get_adjust_train(output, link_status, radeon_output->dp_lane_count, train_set);
	dp_update_dpvs_emph(output, train_set);

	++tries;
    }

    if (!channel_eq)
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "channel eq failed\n");

    dp_set_training(output, DP_TRAINING_PATTERN_DISABLE);
    if (IS_DCE4_VARIANT)
	atombios_output_dig_encoder_setup(output, ATOM_ENCODER_CMD_DP_LINK_TRAINING_COMPLETE);
    else
	RADEONDPEncoderService(output, ATOM_DP_ACTION_TRAINING_COMPLETE, enc_id, 0);

}

