/*
 * Copyright © 2007 Intel Corporation
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "i830.h"
#include "xf86Modes.h"
#include "i830_display.h"

struct i830_hdmi_priv {
    uint32_t output_reg;

    uint32_t save_SDVO;
};

static int
i830_hdmi_mode_valid(xf86OutputPtr output, DisplayModePtr mode)
{
    if (mode->Clock > 165000)
	return MODE_CLOCK_HIGH;

    if (mode->Clock < 20000)
	return MODE_CLOCK_LOW;

    return MODE_OK;
}

static Bool
i830_hdmi_mode_fixup(xf86OutputPtr output, DisplayModePtr mode,
		     DisplayModePtr adjusted_mode)
{
    /* The HDMI output doesn't need the pixel multiplication that SDVO does,
     * so no fixup.
     */
    return TRUE;
}

static void
i830_hdmi_mode_set(xf86OutputPtr output, DisplayModePtr mode,
		   DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_hdmi_priv *dev_priv = intel_output->dev_priv;
    I830Ptr pI830 = I830PTR(pScrn);
    xf86CrtcPtr crtc = output->crtc;
    I830CrtcPrivatePtr intel_crtc = crtc->driver_private;
    uint32_t sdvox;

    sdvox = SDVO_ENCODING_HDMI |
	SDVO_BORDER_ENABLE |
	SDVO_VSYNC_ACTIVE_HIGH |
	SDVO_HSYNC_ACTIVE_HIGH;
    if (intel_crtc->pipe == 1)
	sdvox |= SDVO_PIPE_B_SELECT;

    OUTREG(dev_priv->output_reg, sdvox);
    POSTING_READ(dev_priv->output_reg);
}

static void
i830_hdmi_dpms(xf86OutputPtr output, int mode)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_hdmi_priv *dev_priv = intel_output->dev_priv;
    I830Ptr pI830 = I830PTR(pScrn);
    uint32_t  temp;

    if (mode == DPMSModeOff) {
	temp = INREG(dev_priv->output_reg);
	OUTREG(dev_priv->output_reg, temp & ~SDVO_ENABLE);
    } else {
	temp = INREG(dev_priv->output_reg);
	OUTREG(dev_priv->output_reg, temp | SDVO_ENABLE);
    }
}

static void
i830_hdmi_save(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_hdmi_priv *dev_priv = intel_output->dev_priv;
    I830Ptr pI830 = I830PTR(pScrn);

    dev_priv->save_SDVO = INREG(dev_priv->output_reg);
}

static void
i830_hdmi_restore(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_hdmi_priv *dev_priv = intel_output->dev_priv;
    I830Ptr pI830 = I830PTR(pScrn);

    OUTREG(dev_priv->output_reg, dev_priv->save_SDVO);
}

/**
 * Uses CRT_HOTPLUG_EN and CRT_HOTPLUG_STAT to detect HDMI connection.
 *
 * \return TRUE if HDMI port is connected.
 * \return FALSE if HDMI port is disconnected.
 */
static xf86OutputStatus
i830_hdmi_detect(xf86OutputPtr output)
{
    ScrnInfoPtr	pScrn = output->scrn;
    I830OutputPrivatePtr intel_output = output->driver_private;
    struct i830_hdmi_priv *dev_priv = intel_output->dev_priv;
    I830Ptr pI830 = I830PTR(pScrn);
    uint32_t temp, bit;

    /* For G4X, PEG_BAND_GAP_DATA 3:0 must first be written 0xd.
     * Failure to do so will result in spurious interrupts being
     * generated on the port when a cable is not attached.
     */
    if (IS_G4X(pI830)) {
	temp = INREG(PEG_BAND_GAP_DATA);
	OUTREG(PEG_BAND_GAP_DATA, (temp & ~0xf) | 0xd);
    }

    temp = INREG(PORT_HOTPLUG_EN);

    OUTREG(PORT_HOTPLUG_EN,
	   temp |
	   HDMIB_HOTPLUG_INT_EN |
	   HDMIC_HOTPLUG_INT_EN |
	   HDMID_HOTPLUG_INT_EN);

    POSTING_READ(PORT_HOTPLUG_EN);

    switch (dev_priv->output_reg) {
    case SDVOB:
	bit = HDMIB_HOTPLUG_INT_STATUS;
	break;
    case SDVOC:
	bit = HDMIC_HOTPLUG_INT_STATUS;
	break;
    default:
	return XF86OutputStatusUnknown;
    }

    if ((INREG(PORT_HOTPLUG_STAT) & bit) != 0)
	return XF86OutputStatusConnected;
    else
	return XF86OutputStatusDisconnected;
}

static void
i830_hdmi_destroy (xf86OutputPtr output)
{
    I830OutputPrivatePtr intel_output = output->driver_private;

    if (intel_output != NULL) {
	xf86DestroyI2CBusRec(intel_output->pDDCBus, FALSE, FALSE);
	xfree(intel_output);
    }
}

static const xf86OutputFuncsRec i830_hdmi_output_funcs = {
    .dpms = i830_hdmi_dpms,
    .save = i830_hdmi_save,
    .restore = i830_hdmi_restore,
    .mode_valid = i830_hdmi_mode_valid,
    .mode_fixup = i830_hdmi_mode_fixup,
    .prepare = i830_output_prepare,
    .mode_set = i830_hdmi_mode_set,
    .commit = i830_output_commit,
    .detect = i830_hdmi_detect,
    .get_modes = i830_ddc_get_modes,
    .destroy = i830_hdmi_destroy
};

void
i830_hdmi_init(ScrnInfoPtr pScrn, int output_reg)
{
    xf86OutputPtr output;
    I830OutputPrivatePtr intel_output;
    struct i830_hdmi_priv *dev_priv;

    output = xf86OutputCreate(pScrn, &i830_hdmi_output_funcs,
			      (output_reg == SDVOB) ? "HDMI-1" : "HDMI-2");
    if (!output)
	return;
    intel_output = xnfcalloc(sizeof (I830OutputPrivateRec) +
			     sizeof (struct i830_hdmi_priv), 1);
    if (intel_output == NULL) {
	xf86OutputDestroy(output);
	return;
    }
    output->driver_private = intel_output;
    output->interlaceAllowed = FALSE;
    output->doubleScanAllowed = FALSE;

    dev_priv = (struct i830_hdmi_priv *)(intel_output + 1);
    dev_priv->output_reg = output_reg;

    intel_output->dev_priv = dev_priv;
    intel_output->type = I830_OUTPUT_HDMI;
    intel_output->pipe_mask = ((1 << 0) | (1 << 1));
    intel_output->clone_mask = (1 << I830_OUTPUT_HDMI);

    /* Set up the DDC bus. */
    if (output_reg == SDVOB)
	I830I2CInit(pScrn, &intel_output->pDDCBus, GPIOE, "HDMIDDC_B");
    else
	I830I2CInit(pScrn, &intel_output->pDDCBus, GPIOD, "HDMIDDC_C");

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "HDMI output %d detected\n",
	       (output_reg == SDVOB) ? 1 : 2);
}
