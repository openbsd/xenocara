/**************************************************************************

 Copyright 2006 Dave Airlie <airlied@linux.ie>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
on the rights to use, copy, modify, merge, publish, distribute, sub
license, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice (including the next
paragraph) shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
THE COPYRIGHT HOLDERS AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

/** @file
 * SDVO support for i915 and newer chipsets.
 *
 * The SDVO outputs send digital display data out over the PCIE bus to display
 * cards implementing a defined interface.  These cards may have DVI, TV, CRT,
 * or other outputs on them.
 *
 * The system has two SDVO channels, which may be used for SDVO chips on the
 * motherboard, or in the external cards.  The two channels may also be used
 * in a ganged mode to provide higher bandwidth to a single output.  Currently,
 * this code doesn't deal with either ganged mode or more than one SDVO output.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"
#include "i830.h"
#include "i830_display.h"
#include "i810_reg.h"
#include "i830_sdvo_regs.h"
#include "X11/Xatom.h"

/** SDVO driver private structure. */
struct i830_sdvo_priv {
    /** SDVO device on SDVO I2C bus. */
    I2CDevRec d;

    /** Register for the SDVO device: SDVOB or SDVOC */
    int output_device;

    /** Active outputs controlled by this SDVO output */
    uint16_t controlled_output;

    /**
     * Capabilities of the SDVO device returned by i830_sdvo_get_capabilities()
     */
    struct i830_sdvo_caps caps;

    /**
     * For multiple function SDVO device, this is for current attached outputs.
     */
    uint16_t attached_output;

    /* Current output type name */
    char *name;

    /** Pixel clock limitations reported by the SDVO device, in kHz */
    int pixel_clock_min, pixel_clock_max;

    /**
     * This is set if we're going to treat the device as TV-out.
     *
     * While we have these nice friendly flags for output types that ought to
     * decide this for us, the S-Video output on our HDMI+S-Video card shows
     * up as RGB1 (VGA).
     */
    Bool is_tv;

    /**
     * This is set if we treat the device as HDMI, instead of DVI.
     */
    Bool is_hdmi;
    /**
     * This is set if we detect output of sdvo device as LVDS.
     */
    Bool is_lvds;
    DisplayModePtr sdvo_lvds_fixed_mode;
    /**
     *This is set if output is LVDS or TV.
     */
    uint8_t sdvo_flags;

    /**
     * Returned SDTV resolutions allowed for the current format, if the
     * device reported it.
     */
    struct i830_sdvo_sdtv_resolution_reply sdtv_resolutions;

    /**
     * Current selected TV format.
     *
     * This is stored in the same structure that's passed to the device, for
     * convenience.
     */
    struct i830_sdvo_tv_format tv_format;

    /** supported encoding mode, used to determine whether HDMI is supported */
    struct i830_sdvo_encode encode;

    /** DDC bus used by this SDVO output */
    uint8_t ddc_bus;
    /* Default 0 for full RGB range 0-255, 1 is for RGB range 16-235 */
    uint32_t broadcast_rgb;

    /** This flag means if we should switch ddc bus before next i2c Start */
    Bool ddc_bus_switch;

    /** State for save/restore */
    /** @{ */
    int save_sdvo_mult;
    uint16_t save_active_outputs;
    struct i830_sdvo_dtd save_input_dtd_1, save_input_dtd_2;
    struct i830_sdvo_dtd save_output_dtd[16];
    uint32_t save_SDVOX;
    /** @} */
};

static Atom broadcast_atom;

static void
i830_sdvo_dump(ScrnInfoPtr scrn);

/**
 * Writes the SDVOB or SDVOC with the given value, but always writes both
 * SDVOB and SDVOC to work around apparent hardware issues (according to
 * comments in the BIOS).
 */
static void i830_sdvo_write_sdvox(xf86OutputPtr output, uint32_t val)
{
    ScrnInfoPtr		    scrn = output->scrn;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    uint32_t		    bval = val, cval = val;
    int			    i;

    if (dev_priv->output_device == SDVOB)
	cval = INREG(SDVOC);
    else
	bval = INREG(SDVOB);
    
    /*
     * Write the registers twice for luck. Sometimes,
     * writing them only once doesn't appear to 'stick'.
     * The BIOS does this too. Yay, magic
     */
    for (i = 0; i < 2; i++)
    {
	OUTREG(SDVOB, bval);
	POSTING_READ(SDVOB);
	OUTREG(SDVOC, cval);
	POSTING_READ(SDVOC);
    }
}

/** Read a single byte from the given address on the SDVO device. */
static Bool i830_sdvo_read_byte(xf86OutputPtr output, int addr,
				unsigned char *ch)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;

    if (!xf86I2CReadByte(&dev_priv->d, addr, ch)) {
	xf86DrvMsg(intel_output->pI2CBus->scrnIndex, X_ERROR,
		   "Unable to read from %s slave 0x%02x.\n",
		   intel_output->pI2CBus->BusName, dev_priv->d.SlaveAddr);
	return FALSE;
    }
    return TRUE;
}

/** Read a single byte from the given address on the SDVO device. */
static Bool i830_sdvo_read_byte_quiet(xf86OutputPtr output, int addr,
				      unsigned char *ch)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;

    return xf86I2CReadByte(&dev_priv->d, addr, ch);
}

/** Write a single byte to the given address on the SDVO device. */
static Bool i830_sdvo_write_byte(xf86OutputPtr output,
				 int addr, unsigned char ch)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;

    if (!xf86I2CWriteByte(&dev_priv->d, addr, ch)) {
	xf86DrvMsg(intel_output->pI2CBus->scrnIndex, X_ERROR,
		   "Unable to write to %s Slave 0x%02x.\n",
		   intel_output->pI2CBus->BusName, dev_priv->d.SlaveAddr);
	return FALSE;
    }
    return TRUE;
}


#define SDVO_CMD_NAME_ENTRY(cmd) {cmd, #cmd}
/** Mapping of command numbers to names, for debug output */
const static struct _sdvo_cmd_name {
    uint8_t cmd;
    char *name;
} sdvo_cmd_names[] = {
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_RESET),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_DEVICE_CAPS),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_FIRMWARE_REV),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_TRAINED_INPUTS),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_ACTIVE_OUTPUTS),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_ACTIVE_OUTPUTS),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_IN_OUT_MAP),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_IN_OUT_MAP),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_ATTACHED_DISPLAYS),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_HOT_PLUG_SUPPORT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_ACTIVE_HOT_PLUG),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_ACTIVE_HOT_PLUG),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_INTERRUPT_EVENT_SOURCE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_TARGET_INPUT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_TARGET_OUTPUT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_INPUT_TIMINGS_PART1),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_INPUT_TIMINGS_PART2),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_INPUT_TIMINGS_PART1),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_INPUT_TIMINGS_PART2),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_INPUT_TIMINGS_PART1),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_OUTPUT_TIMINGS_PART1),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_OUTPUT_TIMINGS_PART2),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_OUTPUT_TIMINGS_PART1),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_OUTPUT_TIMINGS_PART2),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_CREATE_PREFERRED_INPUT_TIMING),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_PREFERRED_INPUT_TIMING_PART1),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_PREFERRED_INPUT_TIMING_PART2),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_INPUT_PIXEL_CLOCK_RANGE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_OUTPUT_PIXEL_CLOCK_RANGE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_SUPPORTED_CLOCK_RATE_MULTS),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_CLOCK_RATE_MULT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_CLOCK_RATE_MULT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_SUPPORTED_TV_FORMATS),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_TV_FORMAT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_TV_FORMAT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_SUPPORTED_POWER_STATES),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_POWER_STATE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_ENCODER_POWER_STATE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_DISPLAY_POWER_STATE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_CONTROL_BUS_SWITCH),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_SDTV_RESOLUTION_SUPPORT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_SCALED_HDTV_RESOLUTION_SUPPORT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_SUPPORTED_ENHANCEMENTS),
    /* HDMI op code */
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_SUPP_ENCODE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_ENCODE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_ENCODE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_PIXEL_REPLI),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_PIXEL_REPLI),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_COLORIMETRY_CAP),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_COLORIMETRY),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_COLORIMETRY),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_AUDIO_ENCRYPT_PREFER),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_AUDIO_STAT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_AUDIO_STAT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_HBUF_INDEX),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_HBUF_INDEX),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_HBUF_INFO),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_HBUF_AV_SPLIT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_HBUF_AV_SPLIT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_HBUF_TXRATE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_HBUF_TXRATE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_HBUF_DATA),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_HBUF_DATA),
};

static I2CSlaveAddr slaveAddr;

#define SDVO_NAME(dev_priv) ((dev_priv)->output_device == SDVOB ? "SDVOB" : "SDVOC")
#define SDVO_PRIV(output)   ((struct i830_sdvo_priv *) (output)->dev_priv)

/**
 * Writes out the data given in args (up to 8 bytes), followed by the opcode.
 */
static void
i830_sdvo_write_cmd(xf86OutputPtr output, uint8_t cmd, void *args,
		    int args_len)
{
    intel_screen_private    *intel = intel_get_screen_private(output->scrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    int			    i;

    if (slaveAddr && slaveAddr != dev_priv->d.SlaveAddr)
	ErrorF ("Mismatch slave addr %x != %x\n", slaveAddr, dev_priv->d.SlaveAddr);

    /* Write the SDVO command logging */
    if (intel->debug_modes) {
	xf86DrvMsg(intel_output->pI2CBus->scrnIndex, X_INFO, "%s: W: %02X ",
		   SDVO_NAME(dev_priv), cmd);
	for (i = 0; i < args_len; i++)
	    LogWrite(1, "%02X ", ((uint8_t *)args)[i]);
	for (; i < 8; i++)
	    LogWrite(1, "   ");
	for (i = 0; i < sizeof(sdvo_cmd_names) / sizeof(sdvo_cmd_names[0]);
	     i++)
	{
	    if (cmd == sdvo_cmd_names[i].cmd) {
		LogWrite(1, "(%s)", sdvo_cmd_names[i].name);
		break;
	    }
	}
	if (i == sizeof(sdvo_cmd_names) / sizeof(sdvo_cmd_names[0]))
	    LogWrite(1, "(%02X)", cmd);
	LogWrite(1, "\n");
    }

    /* send the output regs */
    for (i = 0; i < args_len; i++) {
	i830_sdvo_write_byte(output, SDVO_I2C_ARG_0 - i, ((uint8_t *)args)[i]);
    }
    /* blast the command reg */
    i830_sdvo_write_byte(output, SDVO_I2C_OPCODE, cmd);
}

static const char *cmd_status_names[] = {
	"Power on",
	"Success",
	"Not supported",
	"Invalid arg",
	"Pending",
	"Target not specified",
	"Scaling not supported"
};

/**
 * Reads back response_len bytes from the SDVO device, and returns the status.
 */
static uint8_t
i830_sdvo_read_response(xf86OutputPtr output, void *response, int response_len)
{
    intel_screen_private    *intel = intel_get_screen_private(output->scrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    int			    i;
    uint8_t		    status;
    uint8_t		    retry = 50;

    while (retry--) {
    	/* Read the command response */
    	for (i = 0; i < response_len; i++) {
	    i830_sdvo_read_byte(output, SDVO_I2C_RETURN_0 + i,
			    &((uint8_t *)response)[i]);
    	}

    	/* Read the return status */
    	i830_sdvo_read_byte(output, SDVO_I2C_CMD_STATUS, &status);

    	/* Write the SDVO command logging */
    	if (intel->debug_modes) {
	    xf86DrvMsg(intel_output->pI2CBus->scrnIndex, X_INFO,
		   "%s: R: ", SDVO_NAME(SDVO_PRIV(intel_output)));
	    for (i = 0; i < response_len; i++)
	    	LogWrite(1, "%02X ", ((uint8_t *)response)[i]);
	    for (; i < 8; i++)
	    	LogWrite(1, "   ");
	    if (status <= SDVO_CMD_STATUS_SCALING_NOT_SUPP) {
	    	LogWrite(1, "(%s)", cmd_status_names[status]);
	    } else {
	    	LogWrite(1, "(??? %d)", status);
	    }
	    LogWrite(1, "\n");
    	}

	if (status != SDVO_CMD_STATUS_PENDING)
	    return status;

        intel_output->pI2CBus->I2CUDelay(intel_output->pI2CBus, 50);
    }

    return status;
}

static int
i830_sdvo_get_pixel_multiplier(DisplayModePtr pMode)
{
    if (pMode->Clock >= 100000)
	return 1;
    else if (pMode->Clock >= 50000)
	return 2;
    else
	return 4;
}

/* Sets the control bus switch to either point at one of the DDC buses or the
 * PROM.  It resets from the DDC bus back to internal registers at the next I2C
 * STOP.  PROM access is terminated by accessing an internal register.
 */
static void
i830_sdvo_set_control_bus_switch(xf86OutputPtr output, uint8_t target)
{
    i830_sdvo_write_cmd(output, SDVO_CMD_SET_CONTROL_BUS_SWITCH, &target, 1);
}

static Bool
i830_sdvo_set_target_input(xf86OutputPtr output, Bool target_0, Bool target_1)
{
    struct i830_sdvo_set_target_input_args targets = {0};
    uint8_t status;

    if (target_0 && target_1)
	return SDVO_CMD_STATUS_NOTSUPP;

    if (target_1)
	targets.target_1 = 1;

    i830_sdvo_write_cmd(output, SDVO_CMD_SET_TARGET_INPUT, &targets,
			sizeof(targets));

    status = i830_sdvo_read_response(output, NULL, 0);

    return (status == SDVO_CMD_STATUS_SUCCESS);
}

/**
 * Return whether each input is trained.
 *
 * This function is making an assumption about the layout of the response,
 * which should be checked against the docs.
 */
static Bool
i830_sdvo_get_trained_inputs(xf86OutputPtr output, Bool *input_1, Bool *input_2)
{
    struct i830_sdvo_get_trained_inputs_response response;
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_TRAINED_INPUTS, NULL, 0);

    status = i830_sdvo_read_response(output, &response, sizeof(response));
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    *input_1 = response.input0_trained;
    *input_2 = response.input1_trained;

    return TRUE;
}

static Bool
i830_sdvo_get_active_outputs(xf86OutputPtr output,
			     uint16_t *outputs)
{
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_ACTIVE_OUTPUTS, NULL, 0);
    status = i830_sdvo_read_response(output, outputs, sizeof(*outputs));

    return (status == SDVO_CMD_STATUS_SUCCESS);
}

static Bool
i830_sdvo_set_active_outputs(xf86OutputPtr output,
			     uint16_t outputs)
{
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_SET_ACTIVE_OUTPUTS, &outputs,
			sizeof(outputs));
    status = i830_sdvo_read_response(output, NULL, 0);

    return (status == SDVO_CMD_STATUS_SUCCESS);
}

static Bool
i830_sdvo_set_encoder_power_state(xf86OutputPtr output, int mode)
{
    uint8_t status;
    uint8_t state;

    switch (mode) {
    case DPMSModeOn:
	state = SDVO_ENCODER_STATE_ON;
	break;
    case DPMSModeStandby:
	state = SDVO_ENCODER_STATE_STANDBY;
	break;
    case DPMSModeSuspend:
	state = SDVO_ENCODER_STATE_SUSPEND;
	break;
    case DPMSModeOff:
	state = SDVO_ENCODER_STATE_OFF;
	break;
    }

    i830_sdvo_write_cmd(output, SDVO_CMD_SET_ENCODER_POWER_STATE, &state,
			sizeof(state));
    status = i830_sdvo_read_response(output, NULL, 0);

    return (status == SDVO_CMD_STATUS_SUCCESS);
}

/**
 * Returns the pixel clock range limits of the current target input in kHz.
 */
static Bool
i830_sdvo_get_input_pixel_clock_range(xf86OutputPtr output, int *clock_min,
				      int *clock_max)
{
    struct i830_sdvo_pixel_clock_range clocks;
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_INPUT_PIXEL_CLOCK_RANGE, NULL, 0);

    status = i830_sdvo_read_response(output, &clocks, sizeof(clocks));

    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    /* Convert the values from units of 10 kHz to kHz. */
    *clock_min = clocks.min * 10;
    *clock_max = clocks.max * 10;

    return TRUE;
}

static Bool
i830_sdvo_set_target_output(xf86OutputPtr output, uint16_t outputs)
{
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_SET_TARGET_OUTPUT, &outputs,
			sizeof(outputs));

    status = i830_sdvo_read_response(output, NULL, 0);

    return (status == SDVO_CMD_STATUS_SUCCESS);
}

/** Fetches either input or output timings to *dtd, depending on cmd. */
static Bool
i830_sdvo_get_timing(xf86OutputPtr output, uint8_t cmd, struct i830_sdvo_dtd *dtd)
{
    uint8_t status;

    i830_sdvo_write_cmd(output, cmd, NULL, 0);

    status = i830_sdvo_read_response(output, &dtd->part1, sizeof(dtd->part1));
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    i830_sdvo_write_cmd(output, cmd + 1, NULL, 0);

    status = i830_sdvo_read_response(output, &dtd->part2, sizeof(dtd->part2));
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    return TRUE;
}

static Bool
i830_sdvo_get_input_timing(xf86OutputPtr output, struct i830_sdvo_dtd *dtd)
{
    return i830_sdvo_get_timing(output, SDVO_CMD_GET_INPUT_TIMINGS_PART1, dtd);
}

static Bool
i830_sdvo_get_output_timing(xf86OutputPtr output, struct i830_sdvo_dtd *dtd)
{
    return i830_sdvo_get_timing(output, SDVO_CMD_GET_OUTPUT_TIMINGS_PART1, dtd);
}

/** Sets either input or output timings from *dtd, depending on cmd. */
static Bool
i830_sdvo_set_timing(xf86OutputPtr output, uint8_t cmd,
		     struct i830_sdvo_dtd *dtd)
{
    uint8_t status;

    i830_sdvo_write_cmd(output, cmd, &dtd->part1, sizeof(dtd->part1));
    status = i830_sdvo_read_response(output, NULL, 0);
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    i830_sdvo_write_cmd(output, cmd + 1, &dtd->part2, sizeof(dtd->part2));
    status = i830_sdvo_read_response(output, NULL, 0);
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    return TRUE;
}

static Bool
i830_sdvo_set_input_timing(xf86OutputPtr output, struct i830_sdvo_dtd *dtd)
{
    return i830_sdvo_set_timing(output, SDVO_CMD_SET_INPUT_TIMINGS_PART1, dtd);
}

static Bool
i830_sdvo_set_output_timing(xf86OutputPtr output, struct i830_sdvo_dtd *dtd)
{
    return i830_sdvo_set_timing(output, SDVO_CMD_SET_OUTPUT_TIMINGS_PART1, dtd);
}

static Bool
i830_sdvo_create_preferred_input_timing(xf86OutputPtr output, uint16_t clock,
					uint16_t width, uint16_t height)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    struct i830_sdvo_preferred_input_timing_args args;
    uint8_t status;

    memset(&args, 0, sizeof(args));
    args.clock = clock;
    args.width = width;
    args.height = height;
    args.interlace = 0;
    if (dev_priv->is_lvds &&
        (dev_priv->sdvo_lvds_fixed_mode->HDisplay != width ||
        (dev_priv->sdvo_lvds_fixed_mode->VDisplay != height)))
        args.scaled = 1;

    i830_sdvo_write_cmd(output, SDVO_CMD_CREATE_PREFERRED_INPUT_TIMING,
			&args, sizeof(args));
    status = i830_sdvo_read_response(output, NULL, 0);
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    return TRUE;
}

static Bool
i830_sdvo_get_preferred_input_timing(xf86OutputPtr output,
				     struct i830_sdvo_dtd *dtd)
{
    Bool status;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_PREFERRED_INPUT_TIMING_PART1,
			NULL, 0);

    status = i830_sdvo_read_response(output, &dtd->part1, sizeof(dtd->part1));
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_PREFERRED_INPUT_TIMING_PART2,
			NULL, 0);

    status = i830_sdvo_read_response(output, &dtd->part2, sizeof(dtd->part2));
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    return TRUE;
}

/** Returns the SDVO_CLOCK_RATE_MULT_* for the current clock multiplier */
static int
i830_sdvo_get_clock_rate_mult(xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    uint8_t response;
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_CLOCK_RATE_MULT, NULL, 0);
    status = i830_sdvo_read_response(output, &response, 1);

    if (status != SDVO_CMD_STATUS_SUCCESS) {
	xf86DrvMsg(dev_priv->d.pI2CBus->scrnIndex, X_ERROR,
		   "Couldn't get SDVO clock rate multiplier\n");
	return SDVO_CLOCK_RATE_MULT_1X;
    } else {
	xf86DrvMsg(dev_priv->d.pI2CBus->scrnIndex, X_INFO,
		   "Current clock rate multiplier: %d\n", response);
    }

    return response;
}

/**
 * Sets the current clock multiplier.
 *
 * This has to match with the settings in the DPLL/SDVO reg when the output
 * is actually turned on.
 */
static Bool
i830_sdvo_set_clock_rate_mult(xf86OutputPtr output, uint8_t val)
{
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_SET_CLOCK_RATE_MULT, &val, 1);
    status = i830_sdvo_read_response(output, NULL, 0);
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    return TRUE;
}

static void
i830_sdvo_get_dtd_from_mode(struct i830_sdvo_dtd *dtd, DisplayModePtr mode)
{
    uint16_t width, height;
    uint16_t h_blank_len, h_sync_len, v_blank_len, v_sync_len;
    uint16_t h_sync_offset, v_sync_offset;

    width = mode->CrtcHDisplay;
    height = mode->CrtcVDisplay;

    /* do some mode translations */
    h_blank_len = mode->CrtcHBlankEnd - mode->CrtcHBlankStart;
    h_sync_len = mode->CrtcHSyncEnd - mode->CrtcHSyncStart;

    v_blank_len = mode->CrtcVBlankEnd - mode->CrtcVBlankStart;
    v_sync_len = mode->CrtcVSyncEnd - mode->CrtcVSyncStart;

    h_sync_offset = mode->CrtcHSyncStart - mode->CrtcHBlankStart;
    v_sync_offset = mode->CrtcVSyncStart - mode->CrtcVBlankStart;

    dtd->part1.clock = mode->Clock / 10;
    dtd->part1.h_active = width & 0xff;
    dtd->part1.h_blank = h_blank_len & 0xff;
    dtd->part1.h_high = (((width >> 8) & 0xf) << 4) |
	((h_blank_len >> 8) & 0xf);
    dtd->part1.v_active = height & 0xff;
    dtd->part1.v_blank = v_blank_len & 0xff;
    dtd->part1.v_high = (((height >> 8) & 0xf) << 4) |
	((v_blank_len >> 8) & 0xf);

    dtd->part2.h_sync_off = h_sync_offset & 0xff;
    dtd->part2.h_sync_width = h_sync_len & 0xff;
    dtd->part2.v_sync_off_width = (v_sync_offset & 0xf) << 4 |
	(v_sync_len & 0xf);
    dtd->part2.sync_off_width_high = ((h_sync_offset & 0x300) >> 2) |
	((h_sync_len & 0x300) >> 4) | ((v_sync_offset & 0x30) >> 2) |
	((v_sync_len & 0x30) >> 4);

    dtd->part2.dtd_flags = 0x18;
    if (mode->Flags & V_PHSYNC)
	dtd->part2.dtd_flags |= 0x2;
    if (mode->Flags & V_PVSYNC)
	dtd->part2.dtd_flags |= 0x4;

    dtd->part2.sdvo_flags = 0;
    dtd->part2.v_sync_off_high = v_sync_offset & 0xc0;
    dtd->part2.reserved = 0;
}

static void
i830_sdvo_get_mode_from_dtd(DisplayModePtr mode, struct i830_sdvo_dtd *dtd)
{
    mode->HDisplay = dtd->part1.h_active;
    mode->HDisplay += ((dtd->part1.h_high >> 4) & 0x0f) << 8;
    mode->HSyncStart = mode->HDisplay + dtd->part2.h_sync_off;
    mode->HSyncStart += (dtd->part2.sync_off_width_high & 0xc0) << 2;
    mode->HSyncEnd = mode->HSyncStart + dtd->part2.h_sync_width;
    mode->HSyncEnd += (dtd->part2.sync_off_width_high & 0x30) << 4;
    mode->HTotal = mode->HDisplay + dtd->part1.h_blank;
    mode->HTotal += (dtd->part1.h_high & 0xf) << 8;

    mode->VDisplay = dtd->part1.v_active;
    mode->VDisplay += ((dtd->part1.v_high >> 4) & 0x0f) << 8;
    mode->VSyncStart = mode->VDisplay;
    mode->VSyncStart += (dtd->part2.v_sync_off_width >> 4) & 0xf;
    mode->VSyncStart += (dtd->part2.sync_off_width_high & 0x0c) << 2;
    mode->VSyncStart += dtd->part2.v_sync_off_high & 0xc0;
    mode->VSyncEnd = mode->VSyncStart + (dtd->part2.v_sync_off_width & 0xf);
    mode->VSyncEnd += (dtd->part2.sync_off_width_high & 0x3) << 4;
    mode->VTotal = mode->VDisplay + dtd->part1.v_blank;
    mode->VTotal += (dtd->part1.v_high & 0xf) << 8;

    mode->Clock = dtd->part1.clock * 10;

    mode->Flags &= ~(V_PHSYNC | V_PVSYNC);
    if (dtd->part2.dtd_flags & 0x2)
	mode->Flags |= V_PHSYNC;
    if (dtd->part2.dtd_flags & 0x4)
	mode->Flags |= V_PVSYNC;
}

static Bool
i830_sdvo_get_supp_encode(xf86OutputPtr output, struct i830_sdvo_encode *encode)
{
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_SUPP_ENCODE, NULL, 0);
    status = i830_sdvo_read_response(output, encode, sizeof(*encode));
    if (status != SDVO_CMD_STATUS_SUCCESS) { /* non-support means DVI */
	memset(encode, 0, sizeof(*encode));
	return FALSE;
    }

    return TRUE;
}

static Bool
i830_sdvo_get_digital_encoding_mode(xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    uint8_t status;

    i830_sdvo_set_target_output(output, dev_priv->controlled_output);

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_ENCODE, NULL, 0);
    status = i830_sdvo_read_response(output, &dev_priv->is_hdmi, 1);
    if (status != SDVO_CMD_STATUS_SUCCESS) {
	dev_priv->is_hdmi = FALSE;
	return FALSE;
    }
    return TRUE;
}

static Bool
i830_sdvo_set_encode(xf86OutputPtr output, uint8_t mode)
{
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_SET_ENCODE, &mode, 1);
    status = i830_sdvo_read_response(output, NULL, 0);

    return (status == SDVO_CMD_STATUS_SUCCESS);
}

static Bool
i830_sdvo_set_colorimetry(xf86OutputPtr output, uint8_t mode)
{
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_SET_COLORIMETRY, &mode, 1);
    status = i830_sdvo_read_response(output, NULL, 0);

    return (status == SDVO_CMD_STATUS_SUCCESS);
}

#if 0
static Bool
i830_sdvo_set_pixel_repli(xf86OutputPtr output, uint8_t repli)
{
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_SET_PIXEL_REPLI, &repli, 1);
    status = i830_sdvo_read_response(output, NULL, 0);

    return (status == SDVO_CMD_STATUS_SUCCESS);
}
#endif

static void i830_sdvo_dump_hdmi_buf(xf86OutputPtr output)
{
    int i, j;
    uint8_t set_buf_index[2];
    uint8_t av_split;
    uint8_t buf_size;
    uint8_t buf[48];
    uint8_t *pos;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_HBUF_AV_SPLIT, NULL, 0);
    i830_sdvo_read_response(output, &av_split, 1);

    for (i = 0; i <= av_split; i++) {
	set_buf_index[0] = i; set_buf_index[1] = 0;
	i830_sdvo_write_cmd(output, SDVO_CMD_SET_HBUF_INDEX,
				set_buf_index, 2);
	i830_sdvo_write_cmd(output, SDVO_CMD_GET_HBUF_INFO, NULL, 0);
	i830_sdvo_read_response(output, &buf_size, 1);

	pos = buf;
	for (j = 0; j <= buf_size; j += 8) {
	    i830_sdvo_write_cmd(output, SDVO_CMD_GET_HBUF_DATA, NULL, 0);
	    i830_sdvo_read_response(output, pos, 8);
	    pos += 8;
	}
    }
}

static void i830_sdvo_set_hdmi_buf(xf86OutputPtr output, int index,
				uint8_t *data, int8_t size, uint8_t tx_rate)
{
    uint8_t set_buf_index[2];

    set_buf_index[0] = index;
    set_buf_index[1] = 0;

    i830_sdvo_write_cmd(output, SDVO_CMD_SET_HBUF_INDEX, set_buf_index, 2);

    for (; size > 0; size -= 8) {
	i830_sdvo_write_cmd(output, SDVO_CMD_SET_HBUF_DATA, data, 8);
	data += 8;
    }

    i830_sdvo_write_cmd(output, SDVO_CMD_SET_HBUF_TXRATE, &tx_rate, 1);
}

static uint8_t i830_sdvo_calc_hbuf_csum(uint8_t *data, uint8_t size)
{
    uint8_t csum = 0;
    int i;

    for (i = 0; i < size; i++)
	csum += data[i];

    return 0x100 - csum;
}

#define DIP_TYPE_AVI	0x82
#define DIP_VERSION_AVI	0x2
#define DIP_LEN_AVI	13

struct dip_infoframe {
    uint8_t type;
    uint8_t version;
    uint8_t len;
    uint8_t checksum;
    union {
	struct {
	    /* Packet Byte #1 */
	    uint8_t S:2;
	    uint8_t B:2;
	    uint8_t A:1;
	    uint8_t Y:2;
	    uint8_t rsvd1:1;
	    /* Packet Byte #2 */
	    uint8_t R:4;
	    uint8_t M:2;
	    uint8_t C:2;
	    /* Packet Byte #3 */
	    uint8_t SC:2;
	    uint8_t Q:2;
	    uint8_t EC:3;
	    uint8_t ITC:1;
	    /* Packet Byte #4 */
	    uint8_t VIC:7;
	    uint8_t rsvd2:1;
	    /* Packet Byte #5 */
	    uint8_t PR:4;
	    uint8_t rsvd3:4;
	    /* Packet Byte #6~13 */
	    uint16_t top_bar_end;
	    uint16_t bottom_bar_start;
	    uint16_t left_bar_end;
	    uint16_t right_bar_start;
	} avi;
	struct {
	    /* Packet Byte #1 */
	    uint8_t CC:3;
	    uint8_t rsvd1:1;
	    uint8_t CT:4;
	    /* Packet Byte #2 */
	    uint8_t SS:2;
	    uint8_t SF:3;
	    uint8_t rsvd2:3;
	    /* Packet Byte #3 */
	    uint8_t CXT:5;
	    uint8_t rsvd3:3;
	    /* Packet Byte #4 */
	    uint8_t CA;
	    /* Packet Byte #5 */
	    uint8_t rsvd4:3;
	    uint8_t LSV:4;
	    uint8_t DM_INH:1;
	} audio;
	uint8_t payload[28];
    } __attribute__ ((packed)) u;
} __attribute__((packed));

static void i830_sdvo_set_avi_infoframe(xf86OutputPtr output,
					DisplayModePtr mode)
{
    struct dip_infoframe avi_if = {
	.type = DIP_TYPE_AVI,
	.version = DIP_VERSION_AVI,
	.len = DIP_LEN_AVI,
    };

    avi_if.checksum = i830_sdvo_calc_hbuf_csum((uint8_t *)&avi_if,
					4 + avi_if.len);
    i830_sdvo_set_hdmi_buf(output, 1, (uint8_t *)&avi_if, 4 + avi_if.len,
			SDVO_HBUF_TX_VSYNC);
}

static void
i830_sdvo_set_tv_format(xf86OutputPtr output)
{
    ScrnInfoPtr scrn = output->scrn;
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;
    struct i830_sdvo_tv_format *format, unset;
    uint8_t status;

    format = &dev_priv->tv_format;
    memset(&unset, 0, sizeof(unset));
    if (memcmp(format, &unset, sizeof(*format))) {
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "%s: Choosing default TV format of NTSC-M\n",
		   SDVO_NAME(dev_priv));
	format->ntsc_m = 1;
	i830_sdvo_write_cmd(output, SDVO_CMD_SET_TV_FORMAT, format,
		sizeof(*format));
	status = i830_sdvo_read_response(output, NULL, 0);
	if (status != SDVO_CMD_STATUS_SUCCESS)
	    xf86DrvMsg(scrn->scrnIndex, X_INFO,
		    "%s: Fail to set TV format\n", SDVO_NAME(dev_priv));
    }
}

static Bool
i830_sdvo_mode_fixup(xf86OutputPtr output, DisplayModePtr mode,
		     DisplayModePtr adjusted_mode)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;

    if (dev_priv->is_tv) {
	struct i830_sdvo_dtd output_dtd;
	Bool success;

	/* We need to construct preferred input timings based on our output
	 * timings.  To do that, we have to set the output timings, even
	 * though this isn't really the right place in the sequence to do it.
	 * Oh well.
	 */

	ErrorF("output modeline:\n");
	xf86PrintModeline(0, mode);

	/* Set output timings */
	i830_sdvo_get_dtd_from_mode(&output_dtd, mode);
	i830_sdvo_set_target_output(output, dev_priv->controlled_output);
	i830_sdvo_set_output_timing(output, &output_dtd);

	/* Set the input timing to the screen. Assume always input 0. */
	i830_sdvo_set_target_input(output, TRUE, FALSE);

	success = i830_sdvo_create_preferred_input_timing(output,
							  mode->Clock / 10,
							  mode->HDisplay,
							  mode->VDisplay);
	if (success) {
	    struct i830_sdvo_dtd input_dtd;

	    i830_sdvo_get_preferred_input_timing(output, &input_dtd);

	    i830_sdvo_get_mode_from_dtd(adjusted_mode, &input_dtd);
	    dev_priv->sdvo_flags = input_dtd.part2.sdvo_flags;

	    xf86SetModeCrtc(adjusted_mode, 0);

	    ErrorF("input modeline:\n");
	    xf86PrintModeline(0, adjusted_mode);

	    /* adjust origin mode's clock for current input,
	       for correct pixel mulitiplier setting. */
	    mode->Clock = adjusted_mode->Clock;

	    /* Clock range is required to be in 100-200Mhz */
	    adjusted_mode->Clock *= i830_sdvo_get_pixel_multiplier(mode);
	} else {
	    return FALSE;
	}
    } else if (dev_priv->is_lvds) {
        struct i830_sdvo_dtd output_dtd;
        Bool success;

        /* Set output timings */
        i830_sdvo_get_dtd_from_mode(&output_dtd,
                                    dev_priv->sdvo_lvds_fixed_mode);
        i830_sdvo_set_target_output(output, dev_priv->controlled_output);
        i830_sdvo_set_output_timing(output, &output_dtd);

        /* Set the input timing to the screen. Assume always input 0. */
        i830_sdvo_set_target_input(output, TRUE, FALSE);


        success = i830_sdvo_create_preferred_input_timing(output,
                                                          mode->Clock / 10,
                                                          mode->HDisplay,
                                                          mode->VDisplay);
        if (success) {
            struct i830_sdvo_dtd input_dtd;

            i830_sdvo_get_preferred_input_timing(output, &input_dtd);

            i830_sdvo_get_mode_from_dtd(adjusted_mode, &input_dtd);
            dev_priv->sdvo_flags = input_dtd.part2.sdvo_flags;

            xf86SetModeCrtc(adjusted_mode, 0);

            /* adjust origin mode's clock for current input,
              for correct pixel mulitiplier setting. */
            mode->Clock = adjusted_mode->Clock;

            /* Clock range is required to be in 100-200Mhz */
            adjusted_mode->Clock *= i830_sdvo_get_pixel_multiplier(mode);
        } else
            return FALSE;
    } else
	/* Make the CRTC code factor in the SDVO pixel multiplier.  The SDVO
	 * device will be told of the multiplier during mode_set.
	 */
	adjusted_mode->Clock *= i830_sdvo_get_pixel_multiplier(mode);

    return TRUE;
}

static void
i830_sdvo_mode_set(xf86OutputPtr output, DisplayModePtr mode,
		   DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    xf86CrtcPtr	    crtc = output->crtc;
    I830CrtcPrivatePtr	    intel_crtc = crtc->driver_private;
    uint32_t sdvox = 0;
    int sdvo_pixel_multiply;
    struct i830_sdvo_in_out_map in_out;
    struct i830_sdvo_dtd input_dtd;
    uint8_t status;

    if (!mode)
	return;

    /* First, set the input mapping for the first input to our controlled
     * output. This is only correct if we're a single-input device, in
     * which case the first input is the output from the appropriate SDVO
     * channel on the motherboard.  In a two-input device, the first input
     * will be SDVOB and the second SDVOC.
     */
    in_out.in0 = dev_priv->controlled_output;
    in_out.in1 = 0;

    i830_sdvo_write_cmd(output, SDVO_CMD_SET_IN_OUT_MAP,
			&in_out, sizeof(in_out));
    status = i830_sdvo_read_response(output, NULL, 0);

    if (dev_priv->is_hdmi) {
	i830_sdvo_set_avi_infoframe(output, mode);
	sdvox |= SDVO_AUDIO_ENABLE;
    }

    /* We have tried to get input timing in mode_fixup, and filled into
       adjusted_mode */
    if (dev_priv->is_tv || dev_priv->is_lvds) {
	i830_sdvo_get_dtd_from_mode(&input_dtd, adjusted_mode);
        input_dtd.part2.sdvo_flags = dev_priv->sdvo_flags;
    } else
	i830_sdvo_get_dtd_from_mode(&input_dtd, mode);

    /* If it's a TV, we already set the output timing in mode_fixup.
     * Otherwise, the output timing is equal to the input timing.
     */
    i830_sdvo_set_target_output(output, dev_priv->controlled_output);
    /* Set the input timing to the screen. Assume always input 0. */
    i830_sdvo_set_target_input(output, TRUE, FALSE);

    if (dev_priv->is_tv)
	i830_sdvo_set_tv_format(output);

    if (!dev_priv->is_tv && !dev_priv->is_lvds) {
	/* Set the output timing to the screen */
	i830_sdvo_set_output_timing(output, &input_dtd);
    }

    /* We would like to use i830_sdvo_create_preferred_input_timing() to
     * provide the device with a timing it can support, if it supports that
     * feature.  However, presumably we would need to adjust the CRTC to output
     * the preferred timing, and we don't support that currently.
     */
#if 0
    success = i830_sdvo_create_preferred_input_timing(output, clock,
						      width, height);
    if (success) {
	struct i830_sdvo_dtd *input_dtd;

	i830_sdvo_get_preferred_input_timing(output, &input_dtd);
	i830_sdvo_set_input_timing(output, &input_dtd);
    }
#else
    i830_sdvo_set_input_timing(output, &input_dtd);
#endif

    switch (i830_sdvo_get_pixel_multiplier(mode)) {
    case 1:
	i830_sdvo_set_clock_rate_mult(output, SDVO_CLOCK_RATE_MULT_1X);
	break;
    case 2:
	i830_sdvo_set_clock_rate_mult(output, SDVO_CLOCK_RATE_MULT_2X);
	break;
    case 4:
	i830_sdvo_set_clock_rate_mult(output, SDVO_CLOCK_RATE_MULT_4X);
	break;
    }

    /* Set the SDVO control regs. */
    if (IS_I965G(intel)) {
	sdvox |= SDVO_BORDER_ENABLE |
		SDVO_VSYNC_ACTIVE_HIGH |
		SDVO_HSYNC_ACTIVE_HIGH;
    } else {
	sdvox |= INREG(dev_priv->output_device);
	switch (dev_priv->output_device) {
	case SDVOB:
	    sdvox &= SDVOB_PRESERVE_MASK;
	    break;
	case SDVOC:
	    sdvox &= SDVOC_PRESERVE_MASK;
	    break;
	}
	sdvox |= (9 << 19) | SDVO_BORDER_ENABLE;
    }
    if (intel_crtc->pipe == 1)
	sdvox |= SDVO_PIPE_B_SELECT;

    sdvo_pixel_multiply = i830_sdvo_get_pixel_multiplier(mode);
    if (IS_I965G(intel)) {
	/* done in crtc_mode_set as the dpll_md reg must be written early */
    } else if (IS_I945G(intel) || IS_I945GM(intel) || IS_G33CLASS(intel)) {
	/* done in crtc_mode_set as it lives inside the dpll register */
    } else {
	sdvox |= (sdvo_pixel_multiply - 1) << SDVO_PORT_MULTIPLY_SHIFT;
    }
    if (dev_priv->sdvo_flags & SDVO_STALL_FLAG)
        sdvox |= SDVO_STALL_SELECT;

    i830_sdvo_write_sdvox(output, sdvox);

    if (0)
	i830_sdvo_dump(scrn);
}

static void
i830_sdvo_dpms(xf86OutputPtr output, int mode)
{
    ScrnInfoPtr		    scrn = output->scrn;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    uint32_t  temp;

    if (mode != DPMSModeOn) {
	i830_sdvo_set_active_outputs(output, 0);
	if (0)
	    i830_sdvo_set_encoder_power_state(output, mode);

	if (mode == DPMSModeOff) {
	    temp = INREG(dev_priv->output_device);
	    if ((temp & SDVO_ENABLE) != 0) {
		i830_sdvo_write_sdvox(output, temp & ~SDVO_ENABLE);
	    }
	}
    } else {
	Bool input1, input2;
	int i;
	uint8_t status;

	temp = INREG(dev_priv->output_device);
	if ((temp & SDVO_ENABLE) == 0)
	    i830_sdvo_write_sdvox(output, temp | SDVO_ENABLE);
	for (i = 0; i < 2; i++)
	    i830WaitForVblank(scrn);

	status = i830_sdvo_get_trained_inputs(output, &input1, &input2);

	/* Warn if the device reported failure to sync. */
	if (status == SDVO_CMD_STATUS_SUCCESS && !input1) {
	    xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		       "First %s output reported failure to sync\n",
		       SDVO_NAME(dev_priv));
	}

	if (0)
	    i830_sdvo_set_encoder_power_state(output, mode);
	i830_sdvo_set_active_outputs(output, dev_priv->controlled_output);
    }
}

static void
i830_sdvo_save(xf86OutputPtr output)
{
    ScrnInfoPtr		    scrn = output->scrn;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    int			    o;

    /* XXX: We should save the in/out mapping. */

    dev_priv->save_sdvo_mult = i830_sdvo_get_clock_rate_mult(output);
    i830_sdvo_get_active_outputs(output, &dev_priv->save_active_outputs);

    i830_sdvo_set_target_input(output, TRUE, FALSE);
    i830_sdvo_get_input_timing(output, &dev_priv->save_input_dtd_1);

    if (dev_priv->caps.sdvo_input_count >= 2) {
       i830_sdvo_set_target_input(output, FALSE, TRUE);
       i830_sdvo_get_input_timing(output, &dev_priv->save_input_dtd_2);
    }

    for (o = SDVO_OUTPUT_FIRST; o <= SDVO_OUTPUT_LAST; o++)
    {
	uint16_t this_output = (1 << o);
	if (dev_priv->caps.output_flags & this_output)
	{
	    i830_sdvo_set_target_output(output, this_output);
	    i830_sdvo_get_output_timing(output, &dev_priv->save_output_dtd[o]);
	}
    }
    if (dev_priv->is_tv) {
	/* XXX: Save TV format/enhancements. */
    }

    dev_priv->save_SDVOX = INREG(dev_priv->output_device);
}

static void
i830_sdvo_restore(xf86OutputPtr output)
{
    ScrnInfoPtr		    scrn = output->scrn;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    int			    o;
    int			    i;
    Bool		    input1, input2;
    uint8_t		    status;

    i830_sdvo_set_active_outputs(output, 0);

    for (o = SDVO_OUTPUT_FIRST; o <= SDVO_OUTPUT_LAST; o++)
    {
	uint16_t this_output = (1 << o);
	if (dev_priv->caps.output_flags & this_output)
	{
	    i830_sdvo_set_target_output(output, this_output);
	    i830_sdvo_set_output_timing(output, &dev_priv->save_output_dtd[o]);
	}
    }

    i830_sdvo_set_target_input(output, TRUE, FALSE);
    i830_sdvo_set_input_timing(output, &dev_priv->save_input_dtd_1);

    if (dev_priv->caps.sdvo_input_count >= 2) {
       i830_sdvo_set_target_input(output, FALSE, TRUE);
       i830_sdvo_set_input_timing(output, &dev_priv->save_input_dtd_2);
    }

    i830_sdvo_set_clock_rate_mult(output, dev_priv->save_sdvo_mult);

    if (dev_priv->is_tv) {
	/* XXX: Restore TV format/enhancements. */
    }

    i830_sdvo_write_sdvox(output, dev_priv->save_SDVOX);

    if (dev_priv->save_SDVOX & SDVO_ENABLE)
    {
	for (i = 0; i < 2; i++)
	    i830WaitForVblank(scrn);
	status = i830_sdvo_get_trained_inputs(output, &input1, &input2);
	if (status == SDVO_CMD_STATUS_SUCCESS && !input1)
	    xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		       "First %s output reported failure to sync\n",
		       SDVO_NAME(dev_priv));
    }
    
    i830_sdvo_set_active_outputs(output, dev_priv->save_active_outputs);
}

static int
i830_sdvo_mode_valid(xf86OutputPtr output, DisplayModePtr pMode)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;

    if (pMode->Flags & V_DBLSCAN)
	return MODE_NO_DBLESCAN;

    if (dev_priv->pixel_clock_min > pMode->Clock)
	return MODE_CLOCK_LOW;

    if (dev_priv->pixel_clock_max < pMode->Clock)
	return MODE_CLOCK_HIGH;
    if (dev_priv->is_lvds) {
        if (dev_priv->sdvo_lvds_fixed_mode == NULL)
            return MODE_PANEL;

        if (pMode->HDisplay > dev_priv->sdvo_lvds_fixed_mode->HDisplay)
            return MODE_PANEL;

        if (pMode->VDisplay > dev_priv->sdvo_lvds_fixed_mode->VDisplay)
            return MODE_PANEL;
    }

    return MODE_OK;
}

static Bool
i830_sdvo_get_capabilities(xf86OutputPtr output, struct i830_sdvo_caps *caps)
{
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_DEVICE_CAPS, NULL, 0);
    status = i830_sdvo_read_response(output, caps, sizeof(*caps));
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    return TRUE;
}

/** Forces the device over to the real I2C bus and uses its GetByte */
static Bool
i830_sdvo_ddc_i2c_get_byte(I2CDevPtr d, I2CByte *data, Bool last)
{
    xf86OutputPtr	    output = d->pI2CBus->DriverPrivate.ptr;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    I2CBusPtr		    i2cbus = intel_output->pI2CBus, savebus;
    Bool		    ret;

    savebus = d->pI2CBus;
    d->pI2CBus = i2cbus;
    ret = i2cbus->I2CGetByte(d, data, last);
    d->pI2CBus = savebus;

    return ret;
}

/** Forces the device over to the real I2C bus and uses its PutByte */
static Bool
i830_sdvo_ddc_i2c_put_byte(I2CDevPtr d, I2CByte c)
{
    xf86OutputPtr	    output = d->pI2CBus->DriverPrivate.ptr;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    I2CBusPtr		    i2cbus = intel_output->pI2CBus, savebus;
    Bool		    ret;

    savebus = d->pI2CBus;
    d->pI2CBus = i2cbus;
    ret = i2cbus->I2CPutByte(d, c);
    d->pI2CBus = savebus;

    return ret;
}

/**
 * Sets the control bus over to DDC before sending the start on the real I2C
 * bus.
 *
 * The control bus will flip back at the stop following the start executed
 * here.
 */
static Bool
i830_sdvo_ddc_i2c_start(I2CBusPtr b, int timeout)
{
    xf86OutputPtr	    output = b->DriverPrivate.ptr;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    I2CBusPtr		    i2cbus = intel_output->pI2CBus;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;

    if (dev_priv->ddc_bus_switch) {
        i830_sdvo_set_control_bus_switch(output, dev_priv->ddc_bus);
        dev_priv->ddc_bus_switch = FALSE;
    }
    return i2cbus->I2CStart(i2cbus, timeout);
}

/** Forces the device over to the real SDVO bus and sends a stop to it. */
static void
i830_sdvo_ddc_i2c_stop(I2CDevPtr d)
{
    xf86OutputPtr	    output = d->pI2CBus->DriverPrivate.ptr;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    I2CBusPtr		    i2cbus = intel_output->pI2CBus, savebus;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;

    savebus = d->pI2CBus;
    d->pI2CBus = i2cbus;
    i2cbus->I2CStop(d);
    d->pI2CBus = savebus;
    dev_priv->ddc_bus_switch = TRUE;
}

/**
 * Mirrors xf86i2c I2CAddress, using the bus's (wrapped) methods rather than
 * the default methods.
 *
 * This ensures that our start commands always get wrapped with control bus
 * switches.  xf86i2c should probably be fixed to do this.
 */
static Bool
i830_sdvo_ddc_i2c_address(I2CDevPtr d, I2CSlaveAddr addr)
{
    if (d->pI2CBus->I2CStart(d->pI2CBus, d->StartTimeout)) {
	if (d->pI2CBus->I2CPutByte(d, addr & 0xFF)) {
	    if ((addr & 0xF8) != 0xF0 &&
		(addr & 0xFE) != 0x00)
		return TRUE;

	    if (d->pI2CBus->I2CPutByte(d, (addr >> 8) & 0xFF))
		return TRUE;
	}

	d->pI2CBus->I2CStop(d);
    }

    return FALSE;
}

static void
i830_sdvo_dump_cmd(xf86OutputPtr output, int opcode)
{
    uint8_t response[8];

    i830_sdvo_write_cmd(output, opcode, NULL, 0);
    i830_sdvo_read_response(output, response, 8);
}

static void
i830_sdvo_dump_device(xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;

    ErrorF("Dump %s\n", dev_priv->d.DevName);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_DEVICE_CAPS);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_FIRMWARE_REV);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_TRAINED_INPUTS);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_ACTIVE_OUTPUTS);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_IN_OUT_MAP);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_ATTACHED_DISPLAYS);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_HOT_PLUG_SUPPORT);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_ACTIVE_HOT_PLUG);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_INTERRUPT_EVENT_SOURCE);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_INPUT_TIMINGS_PART1);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_INPUT_TIMINGS_PART2);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_OUTPUT_TIMINGS_PART1);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_OUTPUT_TIMINGS_PART2);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_PREFERRED_INPUT_TIMING_PART1);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_PREFERRED_INPUT_TIMING_PART2);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_INPUT_PIXEL_CLOCK_RANGE);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_OUTPUT_PIXEL_CLOCK_RANGE);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_SUPPORTED_CLOCK_RATE_MULTS);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_CLOCK_RATE_MULT);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_SUPPORTED_TV_FORMATS);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_TV_FORMAT);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_SDTV_RESOLUTION_SUPPORT);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_SCALED_HDTV_RESOLUTION_SUPPORT);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_SUPPORTED_ENHANCEMENTS);

    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_SUPP_ENCODE);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_ENCODE);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_PIXEL_REPLI);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_COLORIMETRY_CAP);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_COLORIMETRY);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_AUDIO_ENCRYPT_PREFER);
    i830_sdvo_dump_cmd(output, SDVO_CMD_GET_AUDIO_STAT);
    i830_sdvo_dump_hdmi_buf(output);
}

static void
i830_sdvo_dump(ScrnInfoPtr scrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    int	i;

    for (i = 0; i < xf86_config->num_output; i++) 
    {
	xf86OutputPtr	output = xf86_config->output[i];
	I830OutputPrivatePtr	intel_output = output->driver_private;
	
	if (intel_output->type == I830_OUTPUT_SDVO)
	    i830_sdvo_dump_device(output);
    }
}

static void
i830_sdvo_set_hdmi_encode (xf86OutputPtr output)
{
    /* enable hdmi encoding mode if supported */
    i830_sdvo_set_encode(output, SDVO_ENCODE_HDMI);
    i830_sdvo_set_colorimetry(output, SDVO_COLORIMETRY_RGB256);
}

/**
 * Determine if current TMDS encoding is HDMI.
 * Return TRUE if found HDMI encoding is used, otherwise return FALSE.
 */
static Bool
i830_sdvo_check_hdmi_encode (xf86OutputPtr output)
{
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;

    if (i830_sdvo_get_supp_encode(output, &dev_priv->encode) &&
	    i830_sdvo_get_digital_encoding_mode(output) &&
	    dev_priv->is_hdmi)
    {
	i830_sdvo_set_hdmi_encode(output);
	return TRUE;
    } else
	return FALSE;
}

/* This function will try to fetch native modes for sdvo lvds output*/
static DisplayModePtr i830_sdvo_lvds_fetch_modes(xf86OutputPtr  output)
{
    I830OutputPrivatePtr intel_output =output->driver_private;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;
    intel_screen_private    *intel = intel_get_screen_private(output->scrn);
    DisplayModePtr          modes;

    /*
     * Attempt to get the mode list from DDC.
     * Assume that the preferred modes are
     * arranged in priority order,
     */
    modes = i830_ddc_get_modes(output);
    if (modes != NULL)
        goto end;

    if (intel->sdvo_lvds_fixed_mode != NULL)
        modes = xf86DuplicateModes(output->scrn, intel->sdvo_lvds_fixed_mode);

end:
    /* Guarantee the the first preferred mode is chosen by xserver */
    if (modes != NULL) {
        dev_priv->sdvo_lvds_fixed_mode = xf86DuplicateMode(modes);
        modes->type |= (M_T_DRIVER | M_T_PREFERRED);
        xf86SetModeCrtc(dev_priv->sdvo_lvds_fixed_mode, 0);
    }
    return modes;
}

static void i830_sdvo_select_ddc_bus(struct i830_sdvo_priv *dev_priv);

static Bool
i830_sdvo_output_setup (xf86OutputPtr output, uint16_t flag)
{
    I830OutputPrivatePtr  intel_output = output->driver_private;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;
    char                  *name_prefix;
    char                  *name_suffix;

    if (dev_priv->output_device == SDVOB)
	name_suffix = "-1";
    else
	name_suffix = "-2";

    /* clear up privates */
    dev_priv->is_tv = FALSE;
    intel_output->needs_tv_clock = FALSE;
    dev_priv->is_lvds = FALSE;

    if (flag & (SDVO_OUTPUT_TMDS0 | SDVO_OUTPUT_TMDS1))
    {
	if (flag & SDVO_OUTPUT_TMDS0)
	    dev_priv->controlled_output = SDVO_OUTPUT_TMDS0;
	else
	    dev_priv->controlled_output = SDVO_OUTPUT_TMDS1;
	output->subpixel_order = SubPixelHorizontalRGB;
	name_prefix="TMDS";

	if (i830_sdvo_check_hdmi_encode (output))
	    name_prefix = "HDMI";
    }
    else if (flag & SDVO_OUTPUT_SVID0)
    {
	dev_priv->controlled_output = SDVO_OUTPUT_SVID0;
	output->subpixel_order = SubPixelHorizontalRGB; /* XXX */
	name_prefix="TV";
	dev_priv->is_tv = TRUE;
	intel_output->needs_tv_clock = TRUE;
    }
    else if (flag & SDVO_OUTPUT_CVBS0)
    {
	dev_priv->controlled_output = SDVO_OUTPUT_CVBS0;
	output->subpixel_order = SubPixelHorizontalRGB; /* XXX */
	name_prefix="TV";
	dev_priv->is_tv = TRUE;
	intel_output->needs_tv_clock = TRUE;
    }
    else if (flag & SDVO_OUTPUT_RGB0)
    {
	dev_priv->controlled_output = SDVO_OUTPUT_RGB0;
	output->subpixel_order = SubPixelHorizontalRGB;
	name_prefix="VGA";
    }
    else if (flag & SDVO_OUTPUT_RGB1)
    {
	dev_priv->controlled_output = SDVO_OUTPUT_RGB1;
	output->subpixel_order = SubPixelHorizontalRGB;
	name_prefix="VGA";
    } else if (flag & (SDVO_OUTPUT_LVDS0 | SDVO_OUTPUT_LVDS1)) {
        if (flag & SDVO_OUTPUT_LVDS0)
            dev_priv->controlled_output = SDVO_OUTPUT_LVDS0;
        else
            dev_priv->controlled_output = SDVO_OUTPUT_LVDS1;
        output->subpixel_order = SubPixelHorizontalRGB;
        name_prefix="LVDS";
        dev_priv->is_lvds = TRUE;
    } else {
	unsigned char	bytes[2];

	dev_priv->controlled_output = 0;
	memcpy (bytes, &flag, 2);
	xf86DrvMsg(intel_output->pI2CBus->scrnIndex, X_WARNING,
		   "%s: Unknown SDVO output type (0x%02x%02x)\n",
		   SDVO_NAME(dev_priv),
		   bytes[1], bytes[0]);
	name_prefix="Unknown";
    }

    /* if exist origin name it will be freed in xf86OutputRename() */
    dev_priv->name = malloc(strlen(name_prefix) + strlen(name_suffix) + 1);
    strcpy (dev_priv->name, name_prefix);
    strcat (dev_priv->name, name_suffix);

    if (!xf86OutputRename (output, dev_priv->name))
    {
	xf86DrvMsg(intel_output->pI2CBus->scrnIndex, X_WARNING,
		"%s: Failed to rename output to %s\n",
		SDVO_NAME(dev_priv), dev_priv->name);
	xf86OutputDestroy (output);
	return FALSE;
    }

    /* update randr_output's name */
    if (output->randr_output) {
	int nameLength = strlen(dev_priv->name);
	RROutputPtr randr_output = output->randr_output;
	char *name = malloc(nameLength + 1);

	if (name) {
	    if (randr_output->name != (char *) (randr_output + 1))
		free(randr_output->name);
	    randr_output->name = name;
	    randr_output->nameLength = nameLength;
	    memcpy(randr_output->name, dev_priv->name, nameLength);
	    randr_output->name[nameLength] = '\0';
	} else
	    xf86DrvMsg(intel_output->pI2CBus->scrnIndex, X_WARNING,
		   "%s: Failed to update RandR output name to %s\n",
		   SDVO_NAME(dev_priv), dev_priv->name);
    }

    i830_sdvo_select_ddc_bus(dev_priv);

    return TRUE;
}

static Bool
i830_sdvo_multifunc_encoder(xf86OutputPtr output)
{
    I830OutputPrivatePtr  intel_output = output->driver_private;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;
    int caps = 0;

    if (dev_priv->caps.output_flags & (SDVO_OUTPUT_TMDS0 |
		SDVO_OUTPUT_TMDS1))
	caps++;
    if (dev_priv->caps.output_flags & (SDVO_OUTPUT_RGB0 |
		SDVO_OUTPUT_RGB1))
	caps++;
    if (dev_priv->caps.output_flags & (SDVO_OUTPUT_CVBS0 |
		SDVO_OUTPUT_SVID0 | SDVO_OUTPUT_YPRPB0 |
		SDVO_OUTPUT_SCART0 | SDVO_OUTPUT_CVBS1 |
		SDVO_OUTPUT_SVID1 | SDVO_OUTPUT_YPRPB1 |
		SDVO_OUTPUT_SCART1))
	caps++;
    if (dev_priv->caps.output_flags & (SDVO_OUTPUT_LVDS0 |
		SDVO_OUTPUT_LVDS1))
	caps++;
    return (caps > 1);
}

/**
 * Asks the SDVO device if any displays are currently connected.
 *
 * This interface will need to be augmented, since we could potentially have
 * multiple displays connected, and the caller will also probably want to know
 * what type of display is connected.  But this is enough for the moment.
 *
 * Takes 14ms on average on my i945G.
 */
static xf86OutputStatus
i830_sdvo_detect(xf86OutputPtr output)
{
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;
    uint16_t response;
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_ATTACHED_DISPLAYS, NULL, 0);
    status = i830_sdvo_read_response(output, &response, 2);

    if (status != SDVO_CMD_STATUS_SUCCESS)
	return XF86OutputStatusUnknown;

    if (response == 0)
	return XF86OutputStatusDisconnected;

    if (i830_sdvo_multifunc_encoder(output)) {
	if (dev_priv->attached_output != response) {
	    if (!i830_sdvo_output_setup(output, response))
		return XF86OutputStatusUnknown;
	    dev_priv->attached_output = response;
	}
    }

    if (response & (SDVO_OUTPUT_TMDS0 | SDVO_OUTPUT_TMDS1))
    {
	xf86MonPtr edid_mon;
	/* Check EDID in DVI-I case */
	edid_mon = xf86OutputGetEDID (output, intel_output->pDDCBus);
	if (edid_mon && !DIGITAL(edid_mon->features.input_type)) {
	    free(edid_mon);
	    return XF86OutputStatusDisconnected;
	}
	free(edid_mon);
    }
    return XF86OutputStatusConnected;
}

static DisplayModePtr
i830_sdvo_get_ddc_modes(xf86OutputPtr output)
{
    ScrnInfoPtr scrn = output->scrn;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    DisplayModePtr modes = NULL;
    xf86OutputPtr crt;
    I830OutputPrivatePtr intel_output =output->driver_private;
    xf86MonPtr edid_mon = NULL;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;

    if (dev_priv->is_lvds)
        modes = i830_sdvo_lvds_fetch_modes(output);
    else
        modes = i830_ddc_get_modes(output);

    if (modes != NULL)
	goto check_hdmi;

    /* Mac mini hack.  On this device, I get DDC through the analog, which
     * load-detects as disconnected.  I fail to DDC through the SDVO DDC,
     * but it does load-detect as connected.  So, just steal the DDC bits from
     * analog when we fail at finding it the right way.
     */
    crt = xf86_config->output[0];
    intel_output = crt->driver_private;
    if (intel_output->type == I830_OUTPUT_ANALOG &&
	crt->funcs->detect(crt) == XF86OutputStatusDisconnected) {
	I830I2CInit(scrn, &intel_output->pDDCBus, GPIOA, "CRTDDC_A");
	edid_mon = xf86OutputGetEDID(crt, intel_output->pDDCBus);
	xf86DestroyI2CBusRec(intel_output->pDDCBus, TRUE, TRUE);
    }
    if (edid_mon) {
	xf86OutputSetEDID(output, edid_mon);
	modes = xf86OutputGetEDIDModes(output);
    }

check_hdmi:
    /* Check if HDMI encode, setup it and set the flag for HDMI audio */
    if (dev_priv->caps.output_flags & (SDVO_OUTPUT_TMDS0 | SDVO_OUTPUT_TMDS1))
    {
	if (!i830_sdvo_check_hdmi_encode(output)) {
	    /* check EDID HDMI info for monitor */
	    if (output->MonInfo && xf86LoaderCheckSymbol("xf86MonitorIsHDMI")
		    && xf86MonitorIsHDMI(output->MonInfo)) {
		dev_priv->is_hdmi = TRUE;
		i830_sdvo_set_hdmi_encode (output);
	    } else
		dev_priv->is_hdmi = FALSE;
	}
    }
    return modes;
}

/**
 * Constructs a DisplayModeRec for the given widht/height/refresh, which will
 * be programmed into the display pipe.  The TV encoder's scaler will filter
 * this to the format actually required by the display.
 */
static void
i830_sdvo_get_tv_mode(DisplayModePtr *head, int width, int height,
		      float refresh)
{
    DisplayModePtr mode;

    mode = calloc(1, sizeof(*mode));
    if (mode == NULL)
	return;

    mode->name = XNFprintf("%dx%d@%.2f", width, height, refresh);
    mode->HDisplay = width;
    mode->HSyncStart = width + 1;
    mode->HSyncEnd = width + 64;
    mode->HTotal = width + 96;

    mode->VDisplay = height;
    mode->VSyncStart = height + 1;
    mode->VSyncEnd = height + 32;
    mode->VTotal = height + 33;

    mode->Clock = (int) (refresh * mode->VTotal * mode->HTotal / 1000.0);
    mode->type = M_T_DRIVER;
    mode->next = NULL;
    mode->prev = NULL;

    mode->next = *head;
    mode->prev = NULL;
    if (*head != NULL)
	(*head)->prev = mode;
    *head = mode;
}

/**
 * This function checks the current TV format, and chooses a default if
 * it hasn't been set.
 */
static void
i830_sdvo_check_tv_format(xf86OutputPtr output)
{
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;
    struct i830_sdvo_tv_format format;
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_TV_FORMAT, NULL, 0);
    status = i830_sdvo_read_response(output, &format, sizeof(format));
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return;

    memcpy(&dev_priv->tv_format, &format, sizeof(format));
}

static DisplayModePtr
i830_sdvo_get_tv_modes(xf86OutputPtr output)
{
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;
    DisplayModePtr modes = NULL;
    struct i830_sdvo_sdtv_resolution_reply *res = &dev_priv->sdtv_resolutions;
    struct i830_sdvo_sdtv_resolution_request tv_res;
    uint8_t status;
    float refresh = 60; /* XXX */

    i830_sdvo_check_tv_format(output);

    /* Read the list of supported input resolutions for the selected TV format.
     */
    memset(&tv_res, 0, sizeof(tv_res));
    memcpy(&tv_res, &dev_priv->tv_format, sizeof(tv_res));
    i830_sdvo_write_cmd(output, SDVO_CMD_GET_SDTV_RESOLUTION_SUPPORT,
	    &tv_res, sizeof(tv_res));
    status = i830_sdvo_read_response(output, res, sizeof(*res));
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return NULL;

    if (res->res_320x200) i830_sdvo_get_tv_mode(&modes, 320, 200, refresh);
    if (res->res_320x240) i830_sdvo_get_tv_mode(&modes, 320, 240, refresh);
    if (res->res_400x300) i830_sdvo_get_tv_mode(&modes, 400, 300, refresh);
    if (res->res_640x350) i830_sdvo_get_tv_mode(&modes, 640, 350, refresh);
    if (res->res_640x400) i830_sdvo_get_tv_mode(&modes, 640, 400, refresh);
    if (res->res_640x480) i830_sdvo_get_tv_mode(&modes, 640, 480, refresh);
    if (res->res_704x480) i830_sdvo_get_tv_mode(&modes, 704, 480, refresh);
    if (res->res_704x576) i830_sdvo_get_tv_mode(&modes, 704, 576, refresh);
    if (res->res_720x350) i830_sdvo_get_tv_mode(&modes, 720, 350, refresh);
    if (res->res_720x400) i830_sdvo_get_tv_mode(&modes, 720, 400, refresh);
    if (res->res_720x480) i830_sdvo_get_tv_mode(&modes, 720, 480, refresh);
    if (res->res_720x540) i830_sdvo_get_tv_mode(&modes, 720, 540, refresh);
    if (res->res_720x576) i830_sdvo_get_tv_mode(&modes, 720, 576, refresh);
    if (res->res_800x600) i830_sdvo_get_tv_mode(&modes, 800, 600, refresh);
    if (res->res_832x624) i830_sdvo_get_tv_mode(&modes, 832, 624, refresh);
    if (res->res_920x766) i830_sdvo_get_tv_mode(&modes, 920, 766, refresh);
    if (res->res_1024x768) i830_sdvo_get_tv_mode(&modes, 1024, 768, refresh);
    if (res->res_1280x1024) i830_sdvo_get_tv_mode(&modes, 1280, 1024, refresh);

    return modes;
}

static DisplayModePtr
i830_sdvo_get_modes(xf86OutputPtr output)
{
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;

    if (dev_priv->is_tv)
	return i830_sdvo_get_tv_modes(output);
    else
	return i830_sdvo_get_ddc_modes(output);
}

static void
i830_sdvo_destroy (xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;

    if (intel_output)
    {
	struct i830_sdvo_priv	*dev_priv = intel_output->dev_priv;

	xf86DestroyI2CBusRec (intel_output->pDDCBus, FALSE, FALSE);
	xf86DestroyI2CDevRec (&dev_priv->d, FALSE);
	xf86DestroyI2CBusRec (dev_priv->d.pI2CBus, TRUE, TRUE);
	free(dev_priv->name);

	if (output->randr_output) {
	    RROutputPtr	randr_output = output->randr_output;
	    if (randr_output->name &&
		    randr_output->name != (char *) (randr_output + 1))
		free(randr_output->name);
	}

	if (dev_priv->sdvo_lvds_fixed_mode)
		xf86DeleteMode(&dev_priv->sdvo_lvds_fixed_mode,
			dev_priv->sdvo_lvds_fixed_mode);

	free (intel_output);
    }
}

#ifdef RANDR_GET_CRTC_INTERFACE
static xf86CrtcPtr
i830_sdvo_get_crtc(xf86OutputPtr output)
{
    ScrnInfoPtr	scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;
    int pipe = !!(INREG(dev_priv->output_device) & SDVO_PIPE_B_SELECT);
   
    return i830_pipe_to_crtc(scrn, pipe);
}
#endif

static void
i830_sdvo_create_resources(xf86OutputPtr output)
{
    ScrnInfoPtr                 scrn = output->scrn;
    intel_screen_private        *intel = intel_get_screen_private(scrn);
    I830OutputPrivatePtr        intel_output = output->driver_private;
    struct i830_sdvo_priv       *dev_priv = intel_output->dev_priv;
    INT32			broadcast_range[2];
    int                         err;

    /* only R G B are 8bit color mode */
    if (scrn->depth != 24 ||
        /* only 965G and G4X platform */
        !(IS_I965G(intel) || IS_G4X(intel)) ||
        /* only TMDS encoding */
        !(strstr(output->name, "TMDS") || strstr(output->name, "HDMI")))
        return;

    broadcast_atom =
        MakeAtom("BROADCAST_RGB", sizeof("BROADCAST_RGB") - 1, TRUE);

    broadcast_range[0] = 0;
    broadcast_range[1] = 1;
    err = RRConfigureOutputProperty(output->randr_output,
                                    broadcast_atom,
                                    FALSE, TRUE, FALSE, 2, broadcast_range);
    if (err != 0) {
        xf86DrvMsg(scrn->scrnIndex, X_ERROR,
                   "RRConfigureOutputProperty error, %d\n", err);
        return;
    }
    /* Set the current value of the broadcast property as full range */
    dev_priv->broadcast_rgb = 0;
    err = RRChangeOutputProperty(output->randr_output,
                                 broadcast_atom,
                                 XA_INTEGER, 32, PropModeReplace,
                                 1, &dev_priv->broadcast_rgb,
                                 FALSE, TRUE);
    if (err != 0) {
        xf86DrvMsg(scrn->scrnIndex, X_ERROR,
                   "RRChangeOutputProperty error, %d\n", err);
        return;
    }
}

static Bool
i830_sdvo_set_property(xf86OutputPtr output, Atom property,
		       RRPropertyValuePtr value)
{
    ScrnInfoPtr             scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    uint32_t temp;

    if (property == broadcast_atom) {
        uint32_t val;

        if (value->type != XA_INTEGER || value->format != 32 ||
            value->size != 1)
        {
            return FALSE;
        }

        val = *(INT32 *)value->data;
        if (val < 0 || val > 1)
        {
            return FALSE;
        }
        if (val == dev_priv->broadcast_rgb)
            return TRUE;

        temp = INREG(dev_priv->output_device);

        if (val == 1)
            temp |= SDVO_COLOR_NOT_FULL_RANGE;
        else if (val == 0)
            temp &= ~SDVO_COLOR_NOT_FULL_RANGE;

        i830_sdvo_write_sdvox(output, temp);

        dev_priv->broadcast_rgb = val;
    }
    return TRUE;
}

static const xf86OutputFuncsRec i830_sdvo_output_funcs = {
    .create_resources = i830_sdvo_create_resources,
    .dpms = i830_sdvo_dpms,
    .save = i830_sdvo_save,
    .restore = i830_sdvo_restore,
    .mode_valid = i830_sdvo_mode_valid,
    .mode_fixup = i830_sdvo_mode_fixup,
    .prepare = i830_output_prepare,
    .mode_set = i830_sdvo_mode_set,
    .commit = i830_output_commit,
    .detect = i830_sdvo_detect,
    .get_modes = i830_sdvo_get_modes,
    .set_property = i830_sdvo_set_property,
    .destroy = i830_sdvo_destroy,
#ifdef RANDR_GET_CRTC_INTERFACE
    .get_crtc = i830_sdvo_get_crtc,
#endif
};

static unsigned int count_bits(uint32_t mask)
{
    unsigned int n;

    for (n = 0; mask; n++)
	mask &= mask - 1;

    return n;
}

/**
 * Choose the appropriate DDC bus for control bus switch command for this
 * SDVO output based on the controlled output.
 *
 * DDC bus number assignment is in a priority order of RGB outputs, then TMDS
 * outputs, then LVDS outputs.
 */
static void
i830_sdvo_select_ddc_bus(struct i830_sdvo_priv *dev_priv)
{
    uint16_t mask = 0;
    unsigned int num_bits;

    /* Make a mask of outputs less than or equal to our own priority in the
     * list.
     */
    switch (dev_priv->controlled_output) {
    case SDVO_OUTPUT_LVDS1:
	mask |= SDVO_OUTPUT_LVDS1;
    case SDVO_OUTPUT_LVDS0:
	mask |= SDVO_OUTPUT_LVDS0;
    case SDVO_OUTPUT_TMDS1:
	mask |= SDVO_OUTPUT_TMDS1;
    case SDVO_OUTPUT_TMDS0:
	mask |= SDVO_OUTPUT_TMDS0;
    case SDVO_OUTPUT_RGB1:
	mask |= SDVO_OUTPUT_RGB1;
    case SDVO_OUTPUT_RGB0:
	mask |= SDVO_OUTPUT_RGB0;
	break;
    }

    /* Count bits to find what number we are in the priority list. */
    mask &= dev_priv->caps.output_flags;
    num_bits = count_bits(mask);
    if (num_bits > 3) {
	/* if more than 3 outputs, default to DDC bus 3 for now */
	num_bits = 3;
    }

    /* Corresponds to SDVO_CONTROL_BUS_DDCx */
    dev_priv->ddc_bus = 1 << num_bits;
}
/**
 * find the slave address for the given SDVO port based on the info
 * parsed in general definition blocks
 * If the slave address is found in the SDVO device info parsed from
 * VBT,it will be returned. Otherwise it will return the slave address
 * by the following steps.
 * and 0x72 for SDVOC port.
 * a. If one SDVO device info is found in another DVO port, it will return
 * the slave address that is not used. For example: if 0x70 is used,
 * then 0x72 is returned.
 * b. If no SDVO device info is found in another DVO port, it will return
 * 0x70 for SDVOB and 0x72 for SDVOC port.
 */
static
void i830_find_sdvo_slave(ScrnInfoPtr scrn, int output_device,
			  uint8_t *slave_addr)
{
    uint8_t temp_slave_addr;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint8_t dvo_port, dvo2_port;
    struct sdvo_device_mapping *p_mapping;

    if (output_device == SDVOB) {
	/* DEVICE_PORT_DVOB */
	dvo_port = 0;
	dvo2_port = 1;
    } else {
	/* DEVICE_POTR_DVOC */
	dvo_port = 1;
	dvo2_port = 0;
    }

    p_mapping = &(intel->sdvo_mappings[dvo_port]);
    temp_slave_addr = p_mapping->slave_addr;
    if (temp_slave_addr) {
	/* slave address is found . return it */
	*slave_addr = temp_slave_addr;
	return ;
    }
    /* Check whether the SDVO device info is found in another dvo port */
    p_mapping = &(intel->sdvo_mappings[dvo2_port]);
    temp_slave_addr = p_mapping->slave_addr;
    if (!temp_slave_addr) {
	/* no SDVO device is found in another DVO port */
	/* it will return 0x70 for SDVOB and 0x72 for SDVOC */
	if (output_device == SDVOB)
		temp_slave_addr = 0x70;
	else
		temp_slave_addr = 0x72;
	*slave_addr = temp_slave_addr;
	return ;
    }
    /* return the slave address that is not used.
     * If the 0x70 is used, then 0x72 is returned.
     * If the 0x72 is used, then 0x70 is returned.
     */
    if (temp_slave_addr == 0x70)
	temp_slave_addr = 0x72;
    else
	temp_slave_addr = 0x70;

    *slave_addr = temp_slave_addr;
    return ;
}
Bool
i830_sdvo_init(ScrnInfoPtr scrn, int output_device)
{
    xf86OutputPtr	    output;
    I830OutputPrivatePtr    intel_output;
    struct i830_sdvo_priv   *dev_priv;
    int			    i;
    unsigned char	    ch[0x40];
    I2CBusPtr		    i2cbus = NULL, ddcbus;
    uint8_t slave_addr;

    slave_addr = 0;
    i830_find_sdvo_slave(scrn, output_device, &slave_addr);

    output = xf86OutputCreate (scrn, &i830_sdvo_output_funcs,NULL);
    if (!output)
	return FALSE;
    intel_output = xnfcalloc (sizeof (I830OutputPrivateRec) +
			      sizeof (struct i830_sdvo_priv), 1);
    if (!intel_output)
    {
	xf86OutputDestroy (output);
	return FALSE;
    }
    output->driver_private = intel_output;
    dev_priv = (struct i830_sdvo_priv *) (intel_output + 1);
    intel_output->dev_priv = dev_priv;

    output->interlaceAllowed = FALSE;
    output->doubleScanAllowed = FALSE;
    
    intel_output->type = I830_OUTPUT_SDVO;
    intel_output->pipe_mask = ((1 << 0) | (1 << 1));
    intel_output->clone_mask = (1 << I830_OUTPUT_SDVO);

    /* While it's the same bus, we just initialize a new copy to avoid trouble
     * with tracking refcounting ourselves, since the XFree86 DDX bits don't.
     */
    if (output_device == SDVOB)
	I830I2CInit(scrn, &i2cbus, GPIOE, "SDVOCTRL_E for SDVOB");
    else
	I830I2CInit(scrn, &i2cbus, GPIOE, "SDVOCTRL_E for SDVOC");

    if (i2cbus == NULL)
    {
	xf86OutputDestroy (output);
	return FALSE;
    }
    if (output_device == SDVOB) {
	dev_priv->d.DevName = "SDVO Controller B";
    } else {
	dev_priv->d.DevName = "SDVO Controller C";
    }
    dev_priv->d.SlaveAddr = slave_addr;
    dev_priv->d.pI2CBus = i2cbus;
    dev_priv->d.DriverPrivate.ptr = output;
    dev_priv->output_device = output_device;

    if (!xf86I2CDevInit(&dev_priv->d)) 
    {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "Failed to initialize %s I2C device\n",
		   SDVO_NAME(dev_priv));
	xf86OutputDestroy (output);
	return FALSE;
    }

    intel_output->pI2CBus = i2cbus;

    /* Read the regs to test if we can talk to the device */
    for (i = 0; i < 0x40; i++) {
	if (!i830_sdvo_read_byte_quiet(output, i, &ch[i])) {
	    xf86DrvMsg(scrn->scrnIndex, X_INFO,
		       "No SDVO device found on SDVO%c\n",
		       output_device == SDVOB ? 'B' : 'C');
	    xf86OutputDestroy (output);
	    return FALSE;
	}
    }

    /* Set up our wrapper I2C bus for DDC.  It acts just like the regular I2C
     * bus, except that it does the control bus switch to DDC mode before every
     * Start.  While we only need to do it at Start after every Stop after a
     * Start, extra attempts should be harmless.
     */
    ddcbus = xf86CreateI2CBusRec();
    if (ddcbus == NULL) 
    {
	xf86OutputDestroy (output);
	return FALSE;
    }
    if (output_device == SDVOB)
        ddcbus->BusName = "SDVOB DDC Bus";
    else
        ddcbus->BusName = "SDVOC DDC Bus";
    ddcbus->scrnIndex = i2cbus->scrnIndex;
    ddcbus->I2CGetByte = i830_sdvo_ddc_i2c_get_byte;
    ddcbus->I2CPutByte = i830_sdvo_ddc_i2c_put_byte;
    ddcbus->I2CStart = i830_sdvo_ddc_i2c_start;
    ddcbus->I2CStop = i830_sdvo_ddc_i2c_stop;
    ddcbus->I2CAddress = i830_sdvo_ddc_i2c_address;
    ddcbus->DriverPrivate.ptr = output;
    dev_priv->ddc_bus_switch = TRUE;
    
    if (!xf86I2CBusInit(ddcbus)) 
    {
	xf86OutputDestroy (output);
	return FALSE;
    }

    intel_output->pI2CBus = i2cbus;
    intel_output->pDDCBus = ddcbus;
    intel_output->dev_priv = dev_priv;

    if (!i830_sdvo_get_capabilities(output, &dev_priv->caps))
    {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "Failed to get %s capabilities\n",
		   SDVO_NAME(dev_priv));
	xf86OutputDestroy (output);
	return FALSE;
    }

    if (!i830_sdvo_output_setup (output, dev_priv->caps.output_flags))
	return FALSE;

    /* Set the input timing to the screen. Assume always input 0. */
    i830_sdvo_set_target_input(output, TRUE, FALSE);

    i830_sdvo_get_input_pixel_clock_range(output, &dev_priv->pixel_clock_min,
					  &dev_priv->pixel_clock_max);

    xf86DrvMsg(scrn->scrnIndex, X_INFO,
	       "%s: device VID/DID: %02X:%02X.%02X, "
	       "clock range %.1fMHz - %.1fMHz\n",
	       SDVO_NAME(dev_priv),
	       dev_priv->caps.vendor_id, dev_priv->caps.device_id,
	       dev_priv->caps.device_rev_id,
	       dev_priv->pixel_clock_min / 1000.0,
	       dev_priv->pixel_clock_max / 1000.0);

    xf86DrvMsg(scrn->scrnIndex, X_INFO,
	       "%s: %d input channel%s\n",
	       SDVO_NAME(dev_priv), dev_priv->caps.sdvo_input_count,
	       dev_priv->caps.sdvo_input_count >= 2 ? "s" : "");

#define REPORT_OUTPUT_FLAG(flag, name) do {				\
    if (dev_priv->caps.output_flags & flag) {				\
	xf86DrvMsg(scrn->scrnIndex, X_INFO, "%s: %s output reported\n", \
		   SDVO_NAME(dev_priv), name);				\
    }									\
} while (0)

    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_TMDS0, "TMDS0");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_RGB0, "RGB0");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_CVBS0, "CVBS0");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_SVID0, "SVID0");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_YPRPB0, "YPRPB0");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_SCART0, "SCART0");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_LVDS0, "LVDS0");

    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_TMDS1, "TMDS1");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_RGB1, "RGB1");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_CVBS1, "CVBS1");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_SVID1, "SVID1");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_YPRPB1, "YPRPB1");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_SCART1, "SCART1");
    REPORT_OUTPUT_FLAG(SDVO_OUTPUT_LVDS1, "LVDS1");

    return TRUE;
}
