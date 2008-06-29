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

/** SDVO driver private structure. */
struct i830_sdvo_priv {
    /** SDVO device on SDVO I2C bus. */
    I2CDevRec d;

    /** Register for the SDVO device: SDVOB or SDVOC */
    int output_device;

    /** Active outputs controlled by this SDVO output */
    uint16_t active_outputs;

    /**
     * Capabilities of the SDVO device returned by i830_sdvo_get_capabilities()
     */
    struct i830_sdvo_caps caps;

    /** Pixel clock limitations reported by the SDVO device, in kHz */
    int pixel_clock_min, pixel_clock_max;

    /** State for save/restore */
    /** @{ */
    int save_sdvo_mult;
    uint16_t save_active_outputs;
    struct i830_sdvo_dtd save_input_dtd_1, save_input_dtd_2;
    struct i830_sdvo_dtd save_output_dtd[16];
    uint32_t save_SDVOX;
    /** @} */
};

/**
 * Writes the SDVOB or SDVOC with the given value, but always writes both
 * SDVOB and SDVOC to work around apparent hardware issues (according to
 * comments in the BIOS).
 */
static void i830_sdvo_write_sdvox(xf86OutputPtr output, uint32_t val)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    I830Ptr		    pI830 = I830PTR(pScrn);
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
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_GET_ENCODER_POWER_STATE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_ENCODER_POWER_STATE),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_TV_RESOLUTION_SUPPORT),
    SDVO_CMD_NAME_ENTRY(SDVO_CMD_SET_CONTROL_BUS_SWITCH),
};

static I2CSlaveAddr slaveAddr;

#define SDVO_NAME(dev_priv) ((dev_priv)->output_device == SDVOB ? "SDVO" : "SDVO")
#define SDVO_PRIV(output)   ((struct i830_sdvo_priv *) (output)->dev_priv)

/**
 * Writes out the data given in args (up to 8 bytes), followed by the opcode.
 */
static void
i830_sdvo_write_cmd(xf86OutputPtr output, uint8_t cmd, void *args,
		    int args_len)
{
    I830Ptr pI830 = I830PTR(output->scrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    int			    i;

    if (slaveAddr && slaveAddr != dev_priv->d.SlaveAddr)
	ErrorF ("Mismatch slave addr %x != %x\n", slaveAddr, dev_priv->d.SlaveAddr);

    /* Write the SDVO command logging */
    if (pI830->debug_modes) {
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
    I830Ptr pI830 = I830PTR(output->scrn);
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
    	if (pI830->debug_modes) {
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

int
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

#if 0
static Bool
i830_sdvo_create_preferred_input_timing(xf86OutputPtr output, uint16_t clock,
					uint16_t width, uint16_t height)
{
    struct i830_sdvo_priv *dev_priv = output->dev_priv;
    struct i830_sdvo_preferred_input_timing_args args;

    args.clock = clock;
    args.width = width;
    args.height = height;
    i830_sdvo_write_cmd(output, SDVO_CMD_CREATE_PREFERRED_INPUT_TIMING,
			&args, sizeof(args));
    status = i830_sdvo_read_response(output, NULL, 0);
    if (status != SDVO_CMD_STATUS_SUCCESS)
	return FALSE;

    return TRUE;
}

static Bool
i830_sdvo_get_preferred_input_timing(I830OutputPtr output,
				     struct i830_sdvo_dtd *dtd)
{
    struct i830_sdvo_priv *dev_priv = output->dev_priv;

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
#endif

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

static Bool
i830_sdvo_mode_fixup(xf86OutputPtr output, DisplayModePtr mode,
		     DisplayModePtr adjusted_mode)
{
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
    ScrnInfoPtr pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    xf86CrtcPtr	    crtc = output->crtc;
    I830CrtcPrivatePtr	    intel_crtc = crtc->driver_private;
    uint32_t sdvox;
    int sdvo_pixel_multiply;
    uint16_t width, height;
    uint16_t h_blank_len, h_sync_len, v_blank_len, v_sync_len;
    uint16_t h_sync_offset, v_sync_offset;
    struct i830_sdvo_dtd output_dtd;
    uint16_t no_outputs;

    no_outputs = 0;

    if (!mode)
	return;
    width = mode->CrtcHDisplay;
    height = mode->CrtcVDisplay;
    
    /* do some mode translations */
    h_blank_len = mode->CrtcHBlankEnd - mode->CrtcHBlankStart;
    h_sync_len = mode->CrtcHSyncEnd - mode->CrtcHSyncStart;

    v_blank_len = mode->CrtcVBlankEnd - mode->CrtcVBlankStart;
    v_sync_len = mode->CrtcVSyncEnd - mode->CrtcVSyncStart;

    h_sync_offset = mode->CrtcHSyncStart - mode->CrtcHBlankStart;
    v_sync_offset = mode->CrtcVSyncStart - mode->CrtcVBlankStart;

    output_dtd.part1.clock = mode->Clock / 10;
    output_dtd.part1.h_active = width & 0xff;
    output_dtd.part1.h_blank = h_blank_len & 0xff;
    output_dtd.part1.h_high = (((width >> 8) & 0xf) << 4) |
	((h_blank_len >> 8) & 0xf);
    output_dtd.part1.v_active = height & 0xff;
    output_dtd.part1.v_blank = v_blank_len & 0xff;
    output_dtd.part1.v_high = (((height >> 8) & 0xf) << 4) |
	((v_blank_len >> 8) & 0xf);

    output_dtd.part2.h_sync_off = h_sync_offset;
    output_dtd.part2.h_sync_width = h_sync_len & 0xff;
    output_dtd.part2.v_sync_off_width = (v_sync_offset & 0xf) << 4 |
	(v_sync_len & 0xf);
    output_dtd.part2.sync_off_width_high = ((h_sync_offset & 0x300) >> 2) |
	((h_sync_len & 0x300) >> 4) | ((v_sync_offset & 0x30) >> 2) |
	((v_sync_len & 0x30) >> 4);

    output_dtd.part2.dtd_flags = 0x18;
    if (mode->Flags & V_PHSYNC)
	output_dtd.part2.dtd_flags |= 0x2;
    if (mode->Flags & V_PVSYNC)
	output_dtd.part2.dtd_flags |= 0x4;

    output_dtd.part2.sdvo_flags = 0;
    output_dtd.part2.v_sync_off_high = v_sync_offset & 0xc0;
    output_dtd.part2.reserved = 0;

    /* Set the output timing to the screen */
    i830_sdvo_set_target_output(output, dev_priv->active_outputs);
    i830_sdvo_set_output_timing(output, &output_dtd);

    /* Set the input timing to the screen. Assume always input 0. */
    i830_sdvo_set_target_input(output, TRUE, FALSE);

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
    i830_sdvo_set_input_timing(output, &output_dtd);
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
    if (IS_I965G(pI830)) {
	sdvox = SDVO_BORDER_ENABLE |
		SDVO_VSYNC_ACTIVE_HIGH |
		SDVO_HSYNC_ACTIVE_HIGH;
    } else {
	sdvox = INREG(dev_priv->output_device);
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
    if (IS_I965G(pI830)) {
	/* done in crtc_mode_set as the dpll_md reg must be written early */
    } else if (IS_I945G(pI830) || IS_I945GM(pI830) || IS_G33CLASS(pI830)) {
	/* done in crtc_mode_set as it lives inside the dpll register */
    } else {
	sdvox |= (sdvo_pixel_multiply - 1) << SDVO_PORT_MULTIPLY_SHIFT;
    }

    i830_sdvo_write_sdvox(output, sdvox);
}

static void
i830_sdvo_dpms(xf86OutputPtr output, int mode)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    I830Ptr pI830 = I830PTR(pScrn);
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
	    i830WaitForVblank(pScrn);

	status = i830_sdvo_get_trained_inputs(output, &input1, &input2);

	/* Warn if the device reported failure to sync. */
	if (status == SDVO_CMD_STATUS_SUCCESS && !input1) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "First %s output reported failure to sync\n",
		       SDVO_NAME(dev_priv));
	}

	if (0)
	    i830_sdvo_set_encoder_power_state(output, mode);
	i830_sdvo_set_active_outputs(output, dev_priv->active_outputs);
    }
}

static void
i830_sdvo_save(xf86OutputPtr output)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_sdvo_priv   *dev_priv = intel_output->dev_priv;
    I830Ptr		    pI830 = I830PTR(pScrn);
    int			    o;

    /* XXX: We should save the in/out mapping. */

    dev_priv->save_sdvo_mult = i830_sdvo_get_clock_rate_mult(output);
    i830_sdvo_get_active_outputs(output, &dev_priv->save_active_outputs);

    if (dev_priv->caps.sdvo_inputs_mask & 0x1) {
       i830_sdvo_set_target_input(output, TRUE, FALSE);
       i830_sdvo_get_input_timing(output, &dev_priv->save_input_dtd_1);
    }

    if (dev_priv->caps.sdvo_inputs_mask & 0x2) {
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

    dev_priv->save_SDVOX = INREG(dev_priv->output_device);
}

static void
i830_sdvo_restore(xf86OutputPtr output)
{
    ScrnInfoPtr		    pScrn = output->scrn;
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

    if (dev_priv->caps.sdvo_inputs_mask & 0x1) {
       i830_sdvo_set_target_input(output, TRUE, FALSE);
       i830_sdvo_set_input_timing(output, &dev_priv->save_input_dtd_1);
    }

    if (dev_priv->caps.sdvo_inputs_mask & 0x2) {
       i830_sdvo_set_target_input(output, FALSE, TRUE);
       i830_sdvo_set_input_timing(output, &dev_priv->save_input_dtd_2);
    }

    i830_sdvo_set_clock_rate_mult(output, dev_priv->save_sdvo_mult);

    i830_sdvo_write_sdvox(output, dev_priv->save_SDVOX);

    if (dev_priv->save_SDVOX & SDVO_ENABLE)
    {
	for (i = 0; i < 2; i++)
	    i830WaitForVblank(pScrn);
	status = i830_sdvo_get_trained_inputs(output, &input1, &input2);
	if (status == SDVO_CMD_STATUS_SUCCESS && !input1)
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
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

    i830_sdvo_set_control_bus_switch(output, SDVO_CONTROL_BUS_DDC2);
    return i2cbus->I2CStart(i2cbus, timeout);
}

/** Forces the device over to the real SDVO bus and sends a stop to it. */
static void
i830_sdvo_ddc_i2c_stop(I2CDevPtr d)
{
    xf86OutputPtr	    output = d->pI2CBus->DriverPrivate.ptr;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    I2CBusPtr		    i2cbus = intel_output->pI2CBus, savebus;

    savebus = d->pI2CBus;
    d->pI2CBus = i2cbus;
    i2cbus->I2CStop(d);
    d->pI2CBus = savebus;
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
}

void
i830_sdvo_dump(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int	i;

    for (i = 0; i < xf86_config->num_output; i++) 
    {
	xf86OutputPtr	output = xf86_config->output[i];
	I830OutputPrivatePtr	intel_output = output->driver_private;
	
	if (intel_output->type == I830_OUTPUT_SDVO)
	    i830_sdvo_dump_device(output);
    }
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
    uint8_t response[2];
    uint8_t status;

    i830_sdvo_write_cmd(output, SDVO_CMD_GET_ATTACHED_DISPLAYS, NULL, 0);
    status = i830_sdvo_read_response(output, &response, 2);

    if (status != SDVO_CMD_STATUS_SUCCESS)
	return XF86OutputStatusUnknown;

    if (response[0] != 0 || response[1] != 0)
	return XF86OutputStatusConnected;
    else
	return XF86OutputStatusDisconnected;
}

static DisplayModePtr
i830_sdvo_get_modes(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    DisplayModePtr modes = NULL;
    xf86OutputPtr crt;
    I830OutputPrivatePtr intel_output;
    xf86MonPtr edid_mon = NULL;

    modes = i830_ddc_get_modes(output);
    if (modes != NULL)
	return modes;

    /* Mac mini hack.  On this device, I get DDC through the analog, which
     * load-detects as disconnected.  I fail to DDC through the SDVO DDC,
     * but it does load-detect as connected.  So, just steal the DDC bits from
     * analog when we fail at finding it the right way.
     */
    crt = xf86_config->output[0];
    intel_output = crt->driver_private;
    if (intel_output->type == I830_OUTPUT_ANALOG &&
	crt->funcs->detect(crt) == XF86OutputStatusDisconnected) {
	edid_mon = xf86OutputGetEDID(crt, intel_output->pDDCBus);
    }
    if (edid_mon) {
	xf86OutputSetEDID(output, edid_mon);
	modes = xf86OutputGetEDIDModes(output);
    }

    return modes;
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
	xfree (intel_output);
    }
}

#ifdef RANDR_GET_CRTC_INTERFACE
static xf86CrtcPtr
i830_sdvo_get_crtc(xf86OutputPtr output)
{
    ScrnInfoPtr	pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_sdvo_priv *dev_priv = intel_output->dev_priv;
    int pipe = !!(INREG(dev_priv->output_device) & SDVO_PIPE_B_SELECT);
   
    return i830_pipe_to_crtc(pScrn, pipe);
}
#endif

static const xf86OutputFuncsRec i830_sdvo_output_funcs = {
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
    .destroy = i830_sdvo_destroy,
#ifdef RANDR_GET_CRTC_INTERFACE
    .get_crtc = i830_sdvo_get_crtc,
#endif
};

void
i830_sdvo_init(ScrnInfoPtr pScrn, int output_device)
{
    xf86OutputPtr	    output;
    I830OutputPrivatePtr    intel_output;
    struct i830_sdvo_priv   *dev_priv;
    int			    i;
    unsigned char	    ch[0x40];
    I2CBusPtr		    i2cbus = NULL, ddcbus;
    char		    name[60];
    char		    *name_prefix;
    char		    *name_suffix;

    output = xf86OutputCreate (pScrn, &i830_sdvo_output_funcs,NULL);
    if (!output)
	return;
    intel_output = xnfcalloc (sizeof (I830OutputPrivateRec) +
			      sizeof (struct i830_sdvo_priv), 1);
    if (!intel_output)
    {
	xf86OutputDestroy (output);
	return;
    }
    output->driver_private = intel_output;
    output->interlaceAllowed = FALSE;
    output->doubleScanAllowed = FALSE;
    
    dev_priv = (struct i830_sdvo_priv *) (intel_output + 1);
    intel_output->type = I830_OUTPUT_SDVO;
    intel_output->pipe_mask = ((1 << 0) | (1 << 1));
    intel_output->clone_mask = (1 << I830_OUTPUT_SDVO);

    /* While it's the same bus, we just initialize a new copy to avoid trouble
     * with tracking refcounting ourselves, since the XFree86 DDX bits don't.
     */
    if (output_device == SDVOB)
	I830I2CInit(pScrn, &i2cbus, GPIOE, "SDVOCTRL_E for SDVOB");
    else
	I830I2CInit(pScrn, &i2cbus, GPIOE, "SDVOCTRL_E for SDVOC");

    if (i2cbus == NULL)
    {
	xf86OutputDestroy (output);
	return;
    }

    if (output_device == SDVOB) {
	dev_priv->d.DevName = "SDVO Controller B";
	dev_priv->d.SlaveAddr = 0x70;
	name_suffix="-1";
    } else {
	dev_priv->d.DevName = "SDVO Controller C";
	dev_priv->d.SlaveAddr = 0x72;
	name_suffix="-2";
    }
    dev_priv->d.pI2CBus = i2cbus;
    dev_priv->d.DriverPrivate.ptr = output;
    dev_priv->output_device = output_device;

    if (!xf86I2CDevInit(&dev_priv->d)) 
    {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Failed to initialize %s I2C device\n",
		   SDVO_NAME(dev_priv));
	xf86OutputDestroy (output);
	return;
    }

    intel_output->pI2CBus = i2cbus;
    intel_output->dev_priv = dev_priv;

    /* Read the regs to test if we can talk to the device */
    for (i = 0; i < 0x40; i++) {
	if (!i830_sdvo_read_byte_quiet(output, i, &ch[i])) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "No SDVO device found on SDVO%c\n",
		       output_device == SDVOB ? 'B' : 'C');
	    xf86OutputDestroy (output);
	    return;
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
	return;
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
    
    if (!xf86I2CBusInit(ddcbus)) 
    {
	xf86OutputDestroy (output);
	return;
    }

    intel_output->pI2CBus = i2cbus;
    intel_output->pDDCBus = ddcbus;
    intel_output->dev_priv = dev_priv;

    i830_sdvo_get_capabilities(output, &dev_priv->caps);

    memset(&dev_priv->active_outputs, 0, sizeof(dev_priv->active_outputs));
    if (dev_priv->caps.output_flags & SDVO_OUTPUT_TMDS0)
    {
	dev_priv->active_outputs = SDVO_OUTPUT_TMDS0;
        output->subpixel_order = SubPixelHorizontalRGB;
	name_prefix="TMDS";
    }
    else if (dev_priv->caps.output_flags & SDVO_OUTPUT_TMDS1)
    {
	dev_priv->active_outputs = SDVO_OUTPUT_TMDS1;
        output->subpixel_order = SubPixelHorizontalRGB;
	name_prefix="TMDS";
    }
    else if (dev_priv->caps.output_flags & SDVO_OUTPUT_RGB0)
    {
	dev_priv->active_outputs = SDVO_OUTPUT_RGB0;
        output->subpixel_order = SubPixelHorizontalRGB;
	name_prefix="VGA";
    }
    else if (dev_priv->caps.output_flags & SDVO_OUTPUT_RGB1)
    {
	dev_priv->active_outputs = SDVO_OUTPUT_RGB1;
        output->subpixel_order = SubPixelHorizontalRGB;
	name_prefix="VGA";
    }
    else
    {
	unsigned char	bytes[2];

	memcpy (bytes, &dev_priv->caps.output_flags, 2);
	xf86DrvMsg(intel_output->pI2CBus->scrnIndex, X_ERROR,
		   "%s: No active TMDS outputs (0x%02x%02x)\n",
		   SDVO_NAME(dev_priv),
		   bytes[0], bytes[1]);
	name_prefix="Unknown";
    }
    strcpy (name, name_prefix);
    strcat (name, name_suffix);
    if (!xf86OutputRename (output, name))
    {
	xf86OutputDestroy (output);
	return;
    }
	
    
    /* Set the input timing to the screen. Assume always input 0. */
    i830_sdvo_set_target_input(output, TRUE, FALSE);

    i830_sdvo_get_input_pixel_clock_range(output, &dev_priv->pixel_clock_min,
					  &dev_priv->pixel_clock_max);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "%s device VID/DID: %02X:%02X.%02X, "
	       "clock range %.1fMHz - %.1fMHz, "
	       "input 1: %c, input 2: %c, "
	       "output 1: %c, output 2: %c\n",
	       SDVO_NAME(dev_priv),
	       dev_priv->caps.vendor_id, dev_priv->caps.device_id,
	       dev_priv->caps.device_rev_id,
	       dev_priv->pixel_clock_min / 1000.0,
	       dev_priv->pixel_clock_max / 1000.0,
	       (dev_priv->caps.sdvo_inputs_mask & 0x1) ? 'Y' : 'N',
	       (dev_priv->caps.sdvo_inputs_mask & 0x2) ? 'Y' : 'N',
	       dev_priv->caps.output_flags & (SDVO_OUTPUT_RGB0 | SDVO_OUTPUT_TMDS0) ? 'Y' : 'N',
	       dev_priv->caps.output_flags & (SDVO_OUTPUT_RGB1 | SDVO_OUTPUT_TMDS1) ? 'Y' : 'N');
}
