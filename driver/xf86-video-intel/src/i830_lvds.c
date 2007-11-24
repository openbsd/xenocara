/*
 * Copyright © 2006 Intel Corporation
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "xf86.h"
#include "i830.h"
#include "i830_bios.h"
#include "i830_display.h"
#include "X11/Xatom.h"

struct i830_lvds_priv {
    /* The BIOS's fixed timings for the LVDS */
    DisplayModePtr panel_fixed_mode;
    
    /* The panel needs dithering enabled */
    Bool	    panel_wants_dither;

    /* restore backlight to this value */
    int		    backlight_duty_cycle;

    void (*set_backlight)(xf86OutputPtr output, int level);
    int (*get_backlight)(xf86OutputPtr output);
    int backlight_max;
};

#define BACKLIGHT_CLASS "/sys/class/backlight"

/*
 * List of available kernel interfaces in priority order
 */
static char *backlight_interfaces[] = {
    "thinkpad_screen",
    "acpi_video1",
    "acpi_video0",
    NULL,
};

/*
 * Must be long enough for BACKLIGHT_CLASS + '/' + longest in above table +
 * '/' + "max_backlight"
 */
#define BACKLIGHT_PATH_LEN 80
/* Enough for 8 digits of backlight + '\n' + '\0' */
#define BACKLIGHT_VALUE_LEN 10

static int backlight_index;

static Bool
i830_kernel_backlight_available(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    struct stat buf;
    char path[BACKLIGHT_PATH_LEN];
    int i;

    for (i = 0; backlight_interfaces[i] != NULL; i++) {
	sprintf(path, "%s/%s", BACKLIGHT_CLASS, backlight_interfaces[i]);
	if (!stat(path, &buf)) {
	    backlight_index = i;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "found backlight control "
		       "method %s\n", path);
	    return 1;
	}
    }

    return 0;
}

/* Try to figure out which backlight control method to use */
static void
i830_set_lvds_backlight_method(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    CARD32 blc_pwm_ctl, blc_pwm_ctl2;
    enum backlight_control method = NATIVE; /* Default to native */

    if (i830_kernel_backlight_available(output)) {
	    method = KERNEL;
    } else if (IS_I965GM(pI830)) {
	blc_pwm_ctl2 = INREG(BLC_PWM_CTL2);
	if (blc_pwm_ctl2 & BLM_LEGACY_MODE2)
	    method = COMBO;
    } else {
	blc_pwm_ctl = INREG(BLC_PWM_CTL);
	if (blc_pwm_ctl & BLM_LEGACY_MODE)
	    method = COMBO;
    }

    pI830->backlight_control_method = method;
}

/*
 * Native methods
 */
static void
i830_lvds_set_backlight_native(xf86OutputPtr output, int level)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    CARD32 blc_pwm_ctl;

    blc_pwm_ctl = INREG(BLC_PWM_CTL);
    blc_pwm_ctl &= ~BACKLIGHT_DUTY_CYCLE_MASK;
    OUTREG(BLC_PWM_CTL, blc_pwm_ctl | (level << BACKLIGHT_DUTY_CYCLE_SHIFT));
}

static int
i830_lvds_get_backlight_native(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    CARD32 blc_pwm_ctl;

    blc_pwm_ctl = INREG(BLC_PWM_CTL);
    blc_pwm_ctl &= BACKLIGHT_DUTY_CYCLE_MASK;
    return blc_pwm_ctl;
}

static int
i830_lvds_get_backlight_max_native(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    CARD32 pwm_ctl = INREG(BLC_PWM_CTL);
    int val;

    if (IS_I965GM(pI830)) {
	val = ((pwm_ctl & BACKLIGHT_MODULATION_FREQ_MASK2) >>
	       BACKLIGHT_MODULATION_FREQ_SHIFT2);
    } else {
	val = ((pwm_ctl & BACKLIGHT_MODULATION_FREQ_MASK) >>
	       BACKLIGHT_MODULATION_FREQ_SHIFT) * 2;
    }

    return val;
}

/*
 * Legacy methods
 */
static void
i830_lvds_set_backlight_legacy(xf86OutputPtr output, int level)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);

#if XSERVER_LIBPCIACCESS
    pci_device_cfg_write_u8(pI830->PciInfo, level,
			    LEGACY_BACKLIGHT_BRIGHTNESS);
#else
    pciWriteByte(pI830->PciTag, LEGACY_BACKLIGHT_BRIGHTNESS, level);
#endif
}

static int
i830_lvds_get_backlight_legacy(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    CARD8 lbb;

#if XSERVER_LIBPCIACCESS
    pci_device_cfg_read_u8(pI830->PciInfo, &lbb, LEGACY_BACKLIGHT_BRIGHTNESS);
#else
    lbb = pciReadByte(pI830->PciTag, LEGACY_BACKLIGHT_BRIGHTNESS);
#endif

    return lbb;
}

/*
 * Combo methods
 */
static void
i830_lvds_set_backlight_combo(xf86OutputPtr output, int level)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    CARD32 blc_pwm_ctl;
    CARD8 lbb;

#if XSERVER_LIBPCIACCESS
    pci_device_cfg_read_u8(pI830->PciInfo, &lbb, LEGACY_BACKLIGHT_BRIGHTNESS);
#else
    lbb = pciReadByte(pI830->PciTag, LEGACY_BACKLIGHT_BRIGHTNESS);
#endif
    /*
     * If LBB is zero and we're shooting for a non-zero brightness level,
     * we have to increase LBB by at least 1.
     */
    if (!lbb && level) {
#if XSERVER_LIBPCIACCESS
	pci_device_cfg_write_u8(pI830->PciInfo, 1,
				LEGACY_BACKLIGHT_BRIGHTNESS);
#else
	pciWriteByte(pI830->PciTag, LEGACY_BACKLIGHT_BRIGHTNESS, 1);
#endif
    }

    blc_pwm_ctl = INREG(BLC_PWM_CTL);
    blc_pwm_ctl &= ~BACKLIGHT_DUTY_CYCLE_MASK;
    OUTREG(BLC_PWM_CTL, blc_pwm_ctl | (level << BACKLIGHT_DUTY_CYCLE_SHIFT));
}

static int
i830_lvds_get_backlight_combo(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    I830Ptr pI830 = I830PTR(pScrn);
    CARD32 blc_pwm_ctl;

    blc_pwm_ctl = INREG(BLC_PWM_CTL);
    blc_pwm_ctl &= BACKLIGHT_DUTY_CYCLE_MASK;
    return blc_pwm_ctl;
}

/*
 * Kernel methods
 */
static void
i830_lvds_set_backlight_kernel(xf86OutputPtr output, int level)
{
    ScrnInfoPtr pScrn = output->scrn;
    char path[BACKLIGHT_PATH_LEN], val[BACKLIGHT_VALUE_LEN];
    int fd, len, ret;

    len = snprintf(val, BACKLIGHT_VALUE_LEN, "%d\n", level);
    if (len > BACKLIGHT_VALUE_LEN) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "backlight value too large: %d\n",
		   level);
	return;
    }

    sprintf(path, "%s/%s/brightness", BACKLIGHT_CLASS,
	    backlight_interfaces[backlight_index]);
    fd = open(path, O_RDWR);
    if (fd == -1) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "failed to open %s for backlight "
		   "control: %s\n", path, strerror(errno));
	return;
    }

    ret = write(fd, val, len);
    if (ret == -1) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "write to %s for backlight "
		   "control failed: %s\n", path, strerror(errno));
    }

    close(fd);
}

static int
i830_lvds_get_backlight_kernel(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    char path[BACKLIGHT_PATH_LEN], val[BACKLIGHT_VALUE_LEN];
    int fd;

    sprintf(path, "%s/%s/actual_brightness", BACKLIGHT_CLASS,
	    backlight_interfaces[backlight_index]);
    fd = open(path, O_RDONLY);
    if (fd == -1) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "failed to open %s for backlight "
		   "control: %s\n", path, strerror(errno));
	return 0;
    }

    if (read(fd, val, BACKLIGHT_VALUE_LEN) == -1)
	goto out_err;

    close(fd);
    return atoi(val);

out_err:
    close(fd);
    return 0;
}

static int
i830_lvds_get_backlight_max_kernel(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    char path[BACKLIGHT_PATH_LEN], val[BACKLIGHT_VALUE_LEN];
    int fd, max = 0;

    sprintf(path, "%s/%s/max_brightness", BACKLIGHT_CLASS,
	    backlight_interfaces[backlight_index]);
    fd = open(path, O_RDONLY);
    if (fd == -1) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "failed to open %s for backlight "
		   "control: %s\n", path, strerror(errno));
	return 0;
    }

    if (read(fd, val, BACKLIGHT_VALUE_LEN) == -1)
	goto out_err;

    close(fd);

    max = atoi(val);
    
    return max;

out_err:
    close(fd);
    return 0;
}

/**
 * Sets the power state for the panel.
 */
static void
i830SetLVDSPanelPower(xf86OutputPtr output, Bool on)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    CARD32		    pp_status;

    if (on) {
	OUTREG(PP_CONTROL, INREG(PP_CONTROL) | POWER_TARGET_ON);
	do {
	    pp_status = INREG(PP_STATUS);
	} while ((pp_status & PP_ON) == 0);

	dev_priv->set_backlight(output, dev_priv->backlight_duty_cycle);
    } else {
	dev_priv->set_backlight(output, 0);

	OUTREG(PP_CONTROL, INREG(PP_CONTROL) & ~POWER_TARGET_ON);
	do {
	    pp_status = INREG(PP_STATUS);
	} while (pp_status & PP_ON);
    }
}

static void
i830_lvds_dpms (xf86OutputPtr output, int mode)
{
    if (mode == DPMSModeOn)
	i830SetLVDSPanelPower(output, TRUE);
    else
	i830SetLVDSPanelPower(output, FALSE);

    /* XXX: We never power down the LVDS pairs. */
}

static void
i830_lvds_save (xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);

    if (IS_I965GM(pI830))
	pI830->saveBLC_PWM_CTL2 = INREG(BLC_PWM_CTL2);
    pI830->savePP_ON = INREG(LVDSPP_ON);
    pI830->savePP_OFF = INREG(LVDSPP_OFF);
    pI830->savePP_CONTROL = INREG(PP_CONTROL);
    pI830->savePP_CYCLE = INREG(PP_CYCLE);
    pI830->saveBLC_PWM_CTL = INREG(BLC_PWM_CTL);
    dev_priv->backlight_duty_cycle = dev_priv->get_backlight(output);

    /*
     * If the light is off at server startup, just make it full brightness
     */
    if (dev_priv->backlight_duty_cycle == 0)
	dev_priv->backlight_duty_cycle = dev_priv->backlight_max;
}

static void
i830_lvds_restore(xf86OutputPtr output)
{
    ScrnInfoPtr	pScrn = output->scrn;
    I830Ptr	pI830 = I830PTR(pScrn);

    if (IS_I965GM(pI830))
	OUTREG(BLC_PWM_CTL2, pI830->saveBLC_PWM_CTL2);
    OUTREG(BLC_PWM_CTL, pI830->saveBLC_PWM_CTL);
    OUTREG(LVDSPP_ON, pI830->savePP_ON);
    OUTREG(LVDSPP_OFF, pI830->savePP_OFF);
    OUTREG(PP_CYCLE, pI830->savePP_CYCLE);
    OUTREG(PP_CONTROL, pI830->savePP_CONTROL);
    if (pI830->savePP_CONTROL & POWER_TARGET_ON)
	i830SetLVDSPanelPower(output, TRUE);
    else
	i830SetLVDSPanelPower(output, FALSE);
}

static int
i830_lvds_mode_valid(xf86OutputPtr output, DisplayModePtr pMode)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;
    DisplayModePtr	    pFixedMode = dev_priv->panel_fixed_mode;

    if (pFixedMode)
    {
	if (pMode->HDisplay > pFixedMode->HDisplay)
	    return MODE_PANEL;
	if (pMode->VDisplay > pFixedMode->VDisplay)
	    return MODE_PANEL;
    }

    return MODE_OK;
}

static Bool
i830_lvds_mode_fixup(xf86OutputPtr output, DisplayModePtr mode,
		     DisplayModePtr adjusted_mode)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;
    ScrnInfoPtr		    pScrn = output->scrn;
    xf86CrtcConfigPtr	    xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    I830CrtcPrivatePtr	    intel_crtc = output->crtc->driver_private;
    int i;

    for (i = 0; i < xf86_config->num_output; i++) {
	xf86OutputPtr other_output = xf86_config->output[i];

	if (other_output != output && other_output->crtc == output->crtc) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Can't enable LVDS and another output on the same "
		       "pipe\n");
	    return FALSE;
	}
    }

    if (intel_crtc->pipe == 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Can't support LVDS on pipe A\n");
	return FALSE;
    }

    /* If we have timings from the BIOS for the panel, put them in
     * to the adjusted mode.  The CRTC will be set up for this mode,
     * with the panel scaling set up to source from the H/VDisplay
     * of the original mode.
     */
    if (dev_priv->panel_fixed_mode != NULL) {
	adjusted_mode->HDisplay = dev_priv->panel_fixed_mode->HDisplay;
	adjusted_mode->HSyncStart = dev_priv->panel_fixed_mode->HSyncStart;
	adjusted_mode->HSyncEnd = dev_priv->panel_fixed_mode->HSyncEnd;
	adjusted_mode->HTotal = dev_priv->panel_fixed_mode->HTotal;
	adjusted_mode->VDisplay = dev_priv->panel_fixed_mode->VDisplay;
	adjusted_mode->VSyncStart = dev_priv->panel_fixed_mode->VSyncStart;
	adjusted_mode->VSyncEnd = dev_priv->panel_fixed_mode->VSyncEnd;
	adjusted_mode->VTotal = dev_priv->panel_fixed_mode->VTotal;
	adjusted_mode->Clock = dev_priv->panel_fixed_mode->Clock;
	xf86SetModeCrtc(adjusted_mode, INTERLACE_HALVE_V);
    }

    /* XXX: It would be nice to support lower refresh rates on the
     * panels to reduce power consumption, and perhaps match the
     * user's requested refresh rate.
     */

    return TRUE;
}

static void
i830_lvds_mode_set(xf86OutputPtr output, DisplayModePtr mode,
		   DisplayModePtr adjusted_mode)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    I830CrtcPrivatePtr	    intel_crtc = output->crtc->driver_private;
    CARD32		    pfit_control;

    /* The LVDS pin pair will already have been turned on in
     * i830_crtc_mode_set since it has a large impact on the DPLL settings.
     */

    /* Enable automatic panel scaling for non-native modes so that they fill
     * the screen.  Should be enabled before the pipe is enabled, according to
     * register description and PRM.
     */
    if (mode->HDisplay != adjusted_mode->HDisplay ||
	mode->VDisplay != adjusted_mode->VDisplay)
    {
	pfit_control = PFIT_ENABLE |
	    VERT_AUTO_SCALE | HORIZ_AUTO_SCALE |
	    VERT_INTERP_BILINEAR | HORIZ_INTERP_BILINEAR;
    } else {
	pfit_control = 0;
    }

    if (!IS_I965G(pI830)) {
	if (dev_priv->panel_wants_dither)
	    pfit_control |= PANEL_8TO6_DITHER_ENABLE;
    } else {
	pfit_control |= intel_crtc->pipe << PFIT_PIPE_SHIFT;
    }

    OUTREG(PFIT_CONTROL, pfit_control);
}

/**
 * Detect the LVDS connection.
 *
 * This always returns OUTPUT_STATUS_CONNECTED.  This output should only have
 * been set up if the LVDS was actually connected anyway.
 */
static xf86OutputStatus
i830_lvds_detect(xf86OutputPtr output)
{
    return XF86OutputStatusConnected;
}

/**
 * Return the list of DDC modes if available, or the BIOS fixed mode otherwise.
 */
static DisplayModePtr
i830_lvds_get_modes(xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;
    xf86MonPtr		    edid_mon;
    DisplayModePtr	    modes;

    edid_mon = xf86OutputGetEDID (output, intel_output->pDDCBus);
    xf86OutputSetEDID (output, edid_mon);
    
    modes = xf86OutputGetEDIDModes (output);
    if (modes != NULL)
	return modes;

    if (!output->MonInfo)
    {
	edid_mon = xcalloc (1, sizeof (xf86Monitor));
	if (edid_mon)
	{
	    /* Set wide sync ranges so we get all modes
	     * handed to valid_mode for checking
	     */
	    edid_mon->det_mon[0].type = DS_RANGES;
	    edid_mon->det_mon[0].section.ranges.min_v = 0;
	    edid_mon->det_mon[0].section.ranges.max_v = 200;
	    edid_mon->det_mon[0].section.ranges.min_h = 0;
	    edid_mon->det_mon[0].section.ranges.max_h = 200;
	    
	    output->MonInfo = edid_mon;
	}
    }

    if (dev_priv->panel_fixed_mode != NULL)
	return xf86DuplicateMode(dev_priv->panel_fixed_mode);

    return NULL;
}

static void
i830_lvds_destroy (xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;

    if (intel_output)
    {
	struct i830_lvds_priv	*dev_priv = intel_output->dev_priv;
	
        xf86DeleteMode (&dev_priv->panel_fixed_mode, dev_priv->panel_fixed_mode);
	xfree (intel_output);
    }
}

#ifdef RANDR_12_INTERFACE
#define BACKLIGHT_NAME	"BACKLIGHT"
static Atom backlight_atom;

/*
 * Backlight control lets the user select how the driver should manage
 * backlight changes:  using the legacy interface, the native interface,
 * or not at all.
 */
#define BACKLIGHT_CONTROL_NAME "BACKLIGHT_CONTROL"
#define NUM_BACKLIGHT_CONTROL_METHODS 4
static char *backlight_control_names[] = {
    "native",
    "legacy",
    "combination",
    "kernel",
};
static Atom backlight_control_atom;
static Atom backlight_control_name_atoms[NUM_BACKLIGHT_CONTROL_METHODS];

static int
i830_backlight_control_lookup(char *name)
{
    int i;

    for (i = 0; i < NUM_BACKLIGHT_CONTROL_METHODS; i++)
	if (!strcmp(name, backlight_control_names[i]))
	    return i;

    return -1;
}

static Bool
i830_lvds_set_backlight_control(xf86OutputPtr output)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;

    switch (pI830->backlight_control_method) {
    case NATIVE:
	dev_priv->set_backlight = i830_lvds_set_backlight_native;
	dev_priv->get_backlight = i830_lvds_get_backlight_native;
	dev_priv->backlight_max =
	    i830_lvds_get_backlight_max_native(output);
	break;
    case LEGACY:
	dev_priv->set_backlight = i830_lvds_set_backlight_legacy;
	dev_priv->get_backlight = i830_lvds_get_backlight_legacy;
	dev_priv->backlight_max = 0xff;
	break;
    case COMBO:
	dev_priv->set_backlight = i830_lvds_set_backlight_combo;
	dev_priv->get_backlight = i830_lvds_get_backlight_combo;
	dev_priv->backlight_max =
	    i830_lvds_get_backlight_max_native(output);
	break;
    case KERNEL:
	dev_priv->set_backlight = i830_lvds_set_backlight_kernel;
	dev_priv->get_backlight = i830_lvds_get_backlight_kernel;
	dev_priv->backlight_max =
	    i830_lvds_get_backlight_max_kernel(output);
	break;
    default:
	/*
	 * Should be impossible to get here unless the caller set a bogus
	 * backlight_control_method
	 */
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "error: bad backlight control "
		   "method\n");
	break;
    }

    return Success;
}
#endif /* RANDR_12_INTERFACE */

static void
i830_lvds_create_resources(xf86OutputPtr output)
{
#ifdef RANDR_12_INTERFACE
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;
    INT32		    backlight_range[2];
    int			    data, err, i;

    /* Set up the backlight property, which takes effect immediately
     * and accepts values only within the backlight_range.
     *
     * XXX: Currently, RandR doesn't verify that properties set are
     * within the backlight_range.
     */
    backlight_atom = MakeAtom(BACKLIGHT_NAME, sizeof(BACKLIGHT_NAME) - 1,
	TRUE);

    backlight_range[0] = 0;
    backlight_range[1] = dev_priv->backlight_max;
    err = RRConfigureOutputProperty(output->randr_output, backlight_atom,
				    FALSE, TRUE, FALSE, 2, backlight_range);
    if (err != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "RRConfigureOutputProperty error, %d\n", err);
    }
    /* Set the current value of the backlight property */
    data = dev_priv->backlight_duty_cycle;
    err = RRChangeOutputProperty(output->randr_output, backlight_atom,
				 XA_INTEGER, 32, PropModeReplace, 1, &data,
				 FALSE, TRUE);
    if (err != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "RRChangeOutputProperty error, %d\n", err);
    }

    /*
     * Now setup the control selection property
     */
    backlight_control_atom = MakeAtom(BACKLIGHT_CONTROL_NAME,
				      sizeof(BACKLIGHT_CONTROL_NAME) - 1, TRUE);
    for (i = 0; i < NUM_BACKLIGHT_CONTROL_METHODS; i++) {
	backlight_control_name_atoms[i] =
	    MakeAtom(backlight_control_names[i],
		     strlen(backlight_control_names[i]), TRUE);
    }
    err = RRConfigureOutputProperty(output->randr_output,
				    backlight_control_atom, TRUE, FALSE, FALSE,
				    NUM_BACKLIGHT_CONTROL_METHODS,
				    (INT32 *)backlight_control_name_atoms);
    if (err != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "RRConfigureOutputProperty error, %d\n", err);
    }
    err = RRChangeOutputProperty(output->randr_output, backlight_control_atom,
				 XA_ATOM, 32, PropModeReplace, 1,
				 &backlight_control_name_atoms[pI830->backlight_control_method],
				 FALSE, TRUE);
    if (err != 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "failed to set backlight control, %d\n", err);
    }
#endif /* RANDR_12_INTERFACE */
}

#ifdef RANDR_12_INTERFACE
static Bool
i830_lvds_set_property(xf86OutputPtr output, Atom property,
		       RRPropertyValuePtr value)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;
    
    if (property == backlight_atom) {
	INT32 val;

	if (value->type != XA_INTEGER || value->format != 32 ||
	    value->size != 1)
	{
	    return FALSE;
	}

	val = *(INT32 *)value->data;
	if (val < 0 || val > dev_priv->backlight_max)
	    return FALSE;

	if (val != dev_priv->backlight_duty_cycle) {
	    dev_priv->set_backlight(output, val);
	    dev_priv->backlight_duty_cycle = val;
	}
	return TRUE;
    } else if (property == backlight_control_atom) {
	INT32		    	backlight_range[2];
	Atom			atom;
	char			*name;
	int			ret, data;

	if (value->type != XA_ATOM || value->format != 32 || value->size != 1)
	    return FALSE;

	memcpy(&atom, value->data, 4);
	name = NameForAtom(atom);
	
	ret = i830_backlight_control_lookup(name);
	if (ret < 0)
	    return FALSE;

	pI830->backlight_control_method = ret;
	i830_lvds_set_backlight_control(output);

	/*
	 * Update the backlight atom since the range and value may have changed
	 */
	backlight_range[0] = 0;
	backlight_range[1] = dev_priv->backlight_max;
	ret = RRConfigureOutputProperty(output->randr_output, backlight_atom,
					FALSE, TRUE, FALSE, 2, backlight_range);
	if (ret != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", ret);
	}
	/* Set the current value of the backlight property */
	data = dev_priv->get_backlight(output);
	ret = RRChangeOutputProperty(output->randr_output, backlight_atom,
				     XA_INTEGER, 32, PropModeReplace, 1, &data,
				     FALSE, TRUE);
	if (ret != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", ret);
	}
	return TRUE;
    }

    return TRUE;
}
#endif /* RANDR_12_INTERFACE */

static const xf86OutputFuncsRec i830_lvds_output_funcs = {
    .create_resources = i830_lvds_create_resources,
    .dpms = i830_lvds_dpms,
    .save = i830_lvds_save,
    .restore = i830_lvds_restore,
    .mode_valid = i830_lvds_mode_valid,
    .mode_fixup = i830_lvds_mode_fixup,
    .prepare = i830_output_prepare,
    .mode_set = i830_lvds_mode_set,
    .commit = i830_output_commit,
    .detect = i830_lvds_detect,
    .get_modes = i830_lvds_get_modes,
#ifdef RANDR_12_INTERFACE
    .set_property = i830_lvds_set_property,
#endif
    .destroy = i830_lvds_destroy
};

void
i830_lvds_init(ScrnInfoPtr pScrn)
{
    I830Ptr		    pI830 = I830PTR(pScrn);
    xf86OutputPtr	    output;
    I830OutputPrivatePtr    intel_output;
    DisplayModePtr	    modes, scan, bios_mode;
    struct i830_lvds_priv   *dev_priv;

    if (pI830->quirk_flag & QUIRK_IGNORE_LVDS)
	return;

    output = xf86OutputCreate (pScrn, &i830_lvds_output_funcs, "LVDS");
    if (!output)
	return;
    intel_output = xnfcalloc (sizeof (I830OutputPrivateRec) + 
			      sizeof (struct i830_lvds_priv), 1);
    if (!intel_output)
    {
	xf86OutputDestroy (output);
	return;
    }
    intel_output->type = I830_OUTPUT_LVDS;
    intel_output->pipe_mask = (1 << 1);
    intel_output->clone_mask = (1 << I830_OUTPUT_LVDS);
    
    output->driver_private = intel_output;
    output->subpixel_order = SubPixelHorizontalRGB;
    output->interlaceAllowed = FALSE;
    output->doubleScanAllowed = FALSE;

    dev_priv = (struct i830_lvds_priv *) (intel_output + 1);
    intel_output->dev_priv = dev_priv;

    /* Set up the LVDS DDC channel.  Most panels won't support it, but it can
     * be useful if available.
     */
    I830I2CInit(pScrn, &intel_output->pDDCBus, GPIOC, "LVDSDDC_C");

    /* Attempt to get the fixed panel mode from DDC.  Assume that the preferred
     * mode is the right one.
     */
    modes = i830_ddc_get_modes(output);
    for (scan = modes; scan != NULL; scan = scan->next) {
	if (scan->type & M_T_PREFERRED)
	    break;
    }
    if (scan != NULL) {
	/* Pull our chosen mode out and make it the fixed mode */
	if (modes == scan)
	    modes = modes->next;
	if (scan->prev != NULL)
	    scan->prev = scan->next;
	if (scan->next != NULL)
	    scan->next = scan->prev;
	dev_priv->panel_fixed_mode = scan;
    }
    /* Delete the mode list */
    while (modes != NULL)
	xf86DeleteMode(&modes, modes);

    /* If we didn't get EDID, try checking if the panel is already turned on.
     * If so, assume that whatever is currently programmed is the correct mode.
     */
    if (dev_priv->panel_fixed_mode == NULL) {
	CARD32 lvds = INREG(LVDS);
	int pipe = (lvds & LVDS_PIPEB_SELECT) ? 1 : 0;
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	xf86CrtcPtr crtc = xf86_config->crtc[pipe];

	if (lvds & LVDS_PORT_EN) {
	    dev_priv->panel_fixed_mode = i830_crtc_mode_get(pScrn, crtc);
	    if (dev_priv->panel_fixed_mode != NULL)
		dev_priv->panel_fixed_mode->type |= M_T_PREFERRED;
	}
    }

    /* Get the LVDS fixed mode out of the BIOS.  We should support LVDS with
     * the BIOS being unavailable or broken, but lack the configuration options
     * for now.
     */
    bios_mode = i830_bios_get_panel_mode(pScrn, &dev_priv->panel_wants_dither);
    if (bios_mode != NULL) {
	if (dev_priv->panel_fixed_mode != NULL) {
	    /* Fixup for a 1280x768 panel with the horizontal trimmed
	     * down to 1024 for text mode.
	     */
	    if (!xf86ModesEqual(dev_priv->panel_fixed_mode, bios_mode) &&
		dev_priv->panel_fixed_mode->HDisplay == 1024 &&
		dev_priv->panel_fixed_mode->HSyncStart == 1200 &&
		dev_priv->panel_fixed_mode->HSyncEnd == 1312 &&
		dev_priv->panel_fixed_mode->HTotal == 1688 &&
		dev_priv->panel_fixed_mode->VDisplay == 768)
	    {
		dev_priv->panel_fixed_mode->HDisplay = 1280;
		dev_priv->panel_fixed_mode->HSyncStart = 1328;
		dev_priv->panel_fixed_mode->HSyncEnd = 1440;
		dev_priv->panel_fixed_mode->HTotal = 1688;
	    }

	    if (pI830->debug_modes &&
		!xf86ModesEqual(dev_priv->panel_fixed_mode, bios_mode))
	    {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "BIOS panel mode data doesn't match probed data, "
			   "continuing with probed.\n");
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "BIOS mode:\n");
		xf86PrintModeline(pScrn->scrnIndex, bios_mode);
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "probed mode:\n");
		xf86PrintModeline(pScrn->scrnIndex, dev_priv->panel_fixed_mode);
		xfree(bios_mode->name);
		xfree(bios_mode);
	    }
	}  else {
	    dev_priv->panel_fixed_mode = bios_mode;
	}
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Couldn't detect panel mode.  Disabling panel\n");
	goto disable_exit;
    }

    /* Blacklist machines with BIOSes that list an LVDS panel without actually
     * having one.
     */
    if (pI830->quirk_flag & QUIRK_IGNORE_MACMINI_LVDS) {
	/* It's a Mac Mini or Macbook Pro.
	 *
	 * Apple hardware is out to get us.  The macbook pro has a real
	 * LVDS panel, but the mac mini does not, and they have the same
	 * device IDs.  We'll distinguish by panel size, on the assumption
	 * that Apple isn't about to make any machines with an 800x600
	 * display.
	 */

	if (dev_priv->panel_fixed_mode != NULL &&
		dev_priv->panel_fixed_mode->HDisplay == 800 &&
		dev_priv->panel_fixed_mode->VDisplay == 600)
	{
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "Suspected Mac Mini, ignoring the LVDS\n");
	    goto disable_exit;
	}
    }

    i830_set_lvds_backlight_method(output);

    switch (pI830->backlight_control_method) {
    case NATIVE:
	dev_priv->set_backlight = i830_lvds_set_backlight_native;
	dev_priv->get_backlight = i830_lvds_get_backlight_native;
	dev_priv->backlight_max = i830_lvds_get_backlight_max_native(output);
	break;
    case LEGACY:
	dev_priv->set_backlight = i830_lvds_set_backlight_legacy;
	dev_priv->get_backlight = i830_lvds_get_backlight_legacy;
	dev_priv->backlight_max = 0xff;
	break;
    case COMBO:
	dev_priv->set_backlight = i830_lvds_set_backlight_combo;
	dev_priv->get_backlight = i830_lvds_get_backlight_combo;
	dev_priv->backlight_max = i830_lvds_get_backlight_max_native(output);
	break;
    case KERNEL:
	dev_priv->set_backlight = i830_lvds_set_backlight_kernel;
	dev_priv->get_backlight = i830_lvds_get_backlight_kernel;
	dev_priv->backlight_max = i830_lvds_get_backlight_max_kernel(output);
	break;
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "bad backlight control method\n");
	break;
    }

    dev_priv->backlight_duty_cycle = dev_priv->backlight_max;

    return;

disable_exit:
    xf86DestroyI2CBusRec(intel_output->pDDCBus, TRUE, TRUE);
    xf86OutputDestroy(output);
}
