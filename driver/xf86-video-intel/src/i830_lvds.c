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
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "xf86.h"
#include "i830.h"
#include "i830_bios.h"
#include "i830_display.h"
#include "X11/Xatom.h"

/*
 * Three panel fitting modes:
 * CENTER - center image on screen, don't scale
 * FULL_ASPECT - scale image to fit screen, but preserve aspect ratio
 * FULL - scale image to fit screen without regard to aspect ratio
 */
enum pfit_mode {
    CENTER = 0,
    FULL_ASPECT,
    FULL,
};

struct i830_lvds_priv {
    /* The panel is in DPMS off */
    Bool           dpmsoff;

    /* restore backlight to this value */
    int		    backlight_duty_cycle;

    void (*set_backlight)(xf86OutputPtr output, int level);
    int (*get_backlight)(xf86OutputPtr output);
    int backlight_max;
    enum pfit_mode fitting_mode;
    uint32_t pfit_control;
    uint32_t pfit_pgm_ratios;
};

#define BACKLIGHT_CLASS "/sys/class/backlight"

/*
 * List of available kernel interfaces in priority order
 */
static char *backlight_interfaces[] = {
    "asus-laptop",
    "eeepc",
    "thinkpad_screen",
    "acpi_video1",
    "acpi_video0",
    "fujitsu-laptop",
    "sony",
    "samsung",
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

enum lid_status {
    LID_UNKNOWN = -1,
    LID_OPEN,
    LID_CLOSE,
};

#define ACPI_BUTTON "/proc/acpi/button/"
#define ACPI_LID "/proc/acpi/button/lid/"

static Bool
i830_kernel_backlight_available(xf86OutputPtr output)
{
    ScrnInfoPtr scrn = output->scrn;
    struct stat buf;
    char path[BACKLIGHT_PATH_LEN];
    int i;

    for (i = 0; backlight_interfaces[i] != NULL; i++) {
	sprintf(path, "%s/%s", BACKLIGHT_CLASS, backlight_interfaces[i]);
	if (!stat(path, &buf)) {
	    backlight_index = i;
	    xf86DrvMsg(scrn->scrnIndex, X_INFO, "found backlight control "
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
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t blc_pwm_ctl, blc_pwm_ctl2;
    enum backlight_control method = BCM_NATIVE; /* Default to native */

    if (i830_kernel_backlight_available(output)) {
	    method = BCM_KERNEL;
#if 0
    } else if (IS_IGDNG(intel)) {
	method = BCM_IRONLAKE_NULL;
#endif
    } else if (IS_I965GM(intel) || IS_GM45(intel)) {
	blc_pwm_ctl2 = INREG(BLC_PWM_CTL2);
	if (blc_pwm_ctl2 & BLM_LEGACY_MODE2)
	    method = BCM_COMBO;
    } else {
	blc_pwm_ctl = INREG(BLC_PWM_CTL);
	if (blc_pwm_ctl & BLM_LEGACY_MODE)
	    method = BCM_COMBO;
    }

    intel->backlight_control_method = method;
}

/*
 * Native methods
 */
static void
i830_lvds_set_backlight_native(xf86OutputPtr output, int level)
{
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t blc_pwm_ctl, reg;

    if (IS_IGDNG(intel))
      reg = BLC_PWM_CPU_CTL;
    else
      reg = BLC_PWM_CTL;

    blc_pwm_ctl = INREG(reg);
    blc_pwm_ctl &= ~BACKLIGHT_DUTY_CYCLE_MASK;
    OUTREG(reg, blc_pwm_ctl | (level << BACKLIGHT_DUTY_CYCLE_SHIFT));
}

static int
i830_lvds_get_backlight_native(xf86OutputPtr output)
{
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t blc_pwm_ctl;

    blc_pwm_ctl = INREG(BLC_PWM_CTL);
    blc_pwm_ctl &= BACKLIGHT_DUTY_CYCLE_MASK;
    return blc_pwm_ctl;
}

static int
i830_lvds_get_backlight_max_native(xf86OutputPtr output)
{
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t pwm_ctl;
    int val, reg;

    if (IS_IGDNG(intel))
      reg = BLC_PWM_PCH_CTL2;
    else
      reg = BLC_PWM_CTL;

    pwm_ctl = INREG(reg);

    if (IS_I965GM(intel) || IS_GM45(intel)) {
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
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);

    pci_device_cfg_write_u8(intel->PciInfo, level,
			    LEGACY_BACKLIGHT_BRIGHTNESS);
}

static int
i830_lvds_get_backlight_legacy(xf86OutputPtr output)
{
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint8_t lbb;

    pci_device_cfg_read_u8(intel->PciInfo, &lbb, LEGACY_BACKLIGHT_BRIGHTNESS);

    return lbb;
}

/*
 * Combo methods
 */
static void
i830_lvds_set_backlight_combo(xf86OutputPtr output, int level)
{
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t blc_pwm_ctl;
    uint8_t lbb;

    pci_device_cfg_read_u8(intel->PciInfo, &lbb, LEGACY_BACKLIGHT_BRIGHTNESS);
    /*
     * If LBB is zero and we're shooting for a non-zero brightness level,
     * we have to increase LBB by at least 1.
     */
    if (!lbb && level) {
	pci_device_cfg_write_u8(intel->PciInfo, 1,
				LEGACY_BACKLIGHT_BRIGHTNESS);
    }

    /*
     * Don't set the lowest bit in combo configs since it can act as a flag for
     * max brightness.
     */
    level <<= 1;

    blc_pwm_ctl = INREG(BLC_PWM_CTL);
    blc_pwm_ctl &= ~BACKLIGHT_DUTY_CYCLE_MASK;
    OUTREG(BLC_PWM_CTL, blc_pwm_ctl | (level << BACKLIGHT_DUTY_CYCLE_SHIFT));
}

static int
i830_lvds_get_backlight_combo(xf86OutputPtr output)
{
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t blc_pwm_ctl;

    blc_pwm_ctl = INREG(BLC_PWM_CTL);
    blc_pwm_ctl &= BACKLIGHT_DUTY_CYCLE_MASK;

    /* Since we don't use the low bit when using combo, the value is halved */

    return blc_pwm_ctl >> 1;
}

static int
i830_lvds_get_backlight_max_combo(xf86OutputPtr output)
{
    /* Since we don't set the low bit when using combo, the range is halved */
    return i830_lvds_get_backlight_max_native(output) >> 1;
}

/* null methods */
static int
i830_lvds_get_backlight_null(xf86OutputPtr output)
{
    return 1;
}

static void
i830_lvds_set_backlight_null(xf86OutputPtr output, int level)
{
}

/*
 * Kernel methods
 */
static void
i830_lvds_set_backlight_kernel(xf86OutputPtr output, int level)
{
    ScrnInfoPtr scrn = output->scrn;
    char path[BACKLIGHT_PATH_LEN], val[BACKLIGHT_VALUE_LEN];
    int fd, len, ret;

    len = snprintf(val, BACKLIGHT_VALUE_LEN, "%d\n", level);
    if (len > BACKLIGHT_VALUE_LEN) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR, "backlight value too large: %d\n",
		   level);
	return;
    }

    sprintf(path, "%s/%s/brightness", BACKLIGHT_CLASS,
	    backlight_interfaces[backlight_index]);
    fd = open(path, O_RDWR);
    if (fd == -1) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR, "failed to open %s for backlight "
		   "control: %s\n", path, strerror(errno));
	return;
    }

    ret = write(fd, val, len);
    if (ret == -1) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR, "write to %s for backlight "
		   "control failed: %s\n", path, strerror(errno));
    }

    close(fd);
}

static int
i830_lvds_get_backlight_kernel(xf86OutputPtr output)
{
    ScrnInfoPtr scrn = output->scrn;
    char path[BACKLIGHT_PATH_LEN], val[BACKLIGHT_VALUE_LEN];
    int fd;

    sprintf(path, "%s/%s/actual_brightness", BACKLIGHT_CLASS,
	    backlight_interfaces[backlight_index]);
    fd = open(path, O_RDONLY);
    if (fd == -1) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR, "failed to open %s for backlight "
		   "control: %s\n", path, strerror(errno));
	return 0;
    }

    memset(val, 0, sizeof(val));
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
    ScrnInfoPtr scrn = output->scrn;
    char path[BACKLIGHT_PATH_LEN], val[BACKLIGHT_VALUE_LEN];
    int fd, max = 0;

    sprintf(path, "%s/%s/max_brightness", BACKLIGHT_CLASS,
	    backlight_interfaces[backlight_index]);
    fd = open(path, O_RDONLY);
    if (fd == -1) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR, "failed to open %s for backlight "
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
 *  Get lid state from ACPI button driver
 */
static int
i830_lvds_acpi_lid_open(xf86OutputPtr output)
{
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    int fd;
    DIR *button_dir;
    DIR *lid_dir;
    struct dirent *lid_dent;
    char *state_name;
    char state[64];
    enum lid_status ret = LID_UNKNOWN;

    if (intel->quirk_flag & QUIRK_BROKEN_ACPI_LID)
	goto out;

    button_dir = opendir(ACPI_BUTTON);
    /* If acpi button driver is not loaded, bypass ACPI check method */
    if (button_dir == NULL)
	goto out;
    closedir(button_dir);

    lid_dir = opendir(ACPI_LID);

    /* no acpi lid object found */
    if (lid_dir == NULL)
	goto out;

    while (1) {
	lid_dent = readdir(lid_dir);
	if (lid_dent == NULL) {
	    /* no LID object */
	    closedir(lid_dir);
	    goto out;
	}
	if (strcmp(lid_dent->d_name, ".") &&
		strcmp(lid_dent->d_name, "..")) {
	    break;
	}
    }
    state_name = malloc(strlen(ACPI_LID) + strlen(lid_dent->d_name) + 7);
    memset(state_name, 0, sizeof(state_name));
    strcat(state_name, ACPI_LID);
    strcat(state_name, lid_dent->d_name);
    strcat(state_name, "/state");

    closedir(lid_dir);

    if ((fd = open(state_name, O_RDONLY)) == -1) {
	free(state_name);
	goto out;
    }
    free(state_name);
    if (read(fd, state, 64) == -1) {
	close(fd);
	goto out;
    }
    close(fd);
    if (strstr(state, "open"))
	ret = LID_OPEN;
    else if (strstr(state, "closed"))
	ret = LID_CLOSE;
    else /* "unsupported" */
	ret = LID_UNKNOWN;

out:
    if (intel->debug_modes && (ret != LID_UNKNOWN))
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		"LID switch detect %s with ACPI button\n",
		ret ? "closed" : "open");

    return ret;
}

/**
 * Get LID switch close state from SWF
 */
static Bool
i830_lvds_swf_lid_close(xf86OutputPtr output)
{
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t swf14 = INREG(SWF14);
    Bool ret;

    if (swf14 & SWF14_LID_SWITCH_EN)
	ret = TRUE;
    else
	ret = FALSE;

    if (intel->debug_modes)
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		"LID switch detect %s with SWF14 0x%8x\n",
		ret ? "closed" : "open", swf14);

    return ret;
}

/**
 * Sets the power state for the panel.
 */
static void
i830SetLVDSPanelPower(xf86OutputPtr output, Bool on)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;
    ScrnInfoPtr		    scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    uint32_t		    pp_status, ctl_reg, status_reg, lvds_reg;

    if (IS_IGDNG(intel)) {
	ctl_reg = PCH_PP_CONTROL;
	status_reg = PCH_PP_STATUS;
	lvds_reg = PCH_LVDS;
    } else {
	ctl_reg = PP_CONTROL;
	status_reg = PP_STATUS;
	lvds_reg = LVDS;
    }

    if (on) {
	OUTREG(lvds_reg, INREG(lvds_reg) | LVDS_PORT_EN);
	INREG(lvds_reg);

	if (!(INREG(ctl_reg) & POWER_TARGET_ON) &&
	    dev_priv->backlight_duty_cycle == 0 &&
	    intel->backlight_control_method < BCM_KERNEL)
	    dev_priv->backlight_duty_cycle = dev_priv->backlight_max;

	OUTREG(ctl_reg, INREG(ctl_reg) | POWER_TARGET_ON);
	INREG(ctl_reg);
	do {
	    pp_status = INREG(status_reg);
	} while ((pp_status & PP_ON) == 0);

	/* set backlight */
	dev_priv->set_backlight(output, dev_priv->backlight_duty_cycle);
	dev_priv->dpmsoff = FALSE;
    } else {
	if ((INREG(ctl_reg) & POWER_TARGET_ON) && !dev_priv->dpmsoff)
	    dev_priv->backlight_duty_cycle = dev_priv->get_backlight(output);
	dev_priv->set_backlight(output, 0);

	OUTREG(ctl_reg, INREG(ctl_reg) & ~POWER_TARGET_ON);
	INREG(ctl_reg);
	do {
	    pp_status = INREG(status_reg);
	} while (pp_status & PP_ON);
	
	OUTREG(lvds_reg, INREG(lvds_reg) & ~LVDS_PORT_EN);
	INREG(lvds_reg);
	dev_priv->dpmsoff = TRUE;
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
    ScrnInfoPtr		    scrn = output->scrn;
   intel_screen_private    *intel = intel_get_screen_private(scrn);
   uint32_t pp_on_reg, pp_off_reg, pp_ctl_reg, pp_div_reg, pwm_ctl_reg;

    if (IS_IGDNG(intel)) {
	pp_on_reg = PCH_PP_ON_DELAYS;
	pp_off_reg = PCH_PP_OFF_DELAYS;
	pp_ctl_reg = PCH_PP_CONTROL;
	pp_div_reg = PCH_PP_DIVISOR;
	pwm_ctl_reg = BLC_PWM_CPU_CTL;
    } else {
	pp_on_reg = PP_ON_DELAYS;
	pp_off_reg = PP_OFF_DELAYS;
	pp_ctl_reg = PP_CONTROL;
	pp_div_reg = PP_DIVISOR;
	pwm_ctl_reg = BLC_PWM_CTL;
    }

    if (IS_I965GM(intel) || IS_GM45(intel))
	intel->saveBLC_PWM_CTL2 = INREG(BLC_PWM_CTL2);
    intel->savePP_ON = INREG(pp_on_reg);
    intel->savePP_OFF = INREG(pp_off_reg);
    intel->savePP_CONTROL = INREG(pp_ctl_reg);
    intel->savePP_DIVISOR = INREG(pp_div_reg);
    intel->saveBLC_PWM_CTL = INREG(pwm_ctl_reg);
    if ((INREG(PP_CONTROL) & POWER_TARGET_ON) && !dev_priv->dpmsoff) 
	dev_priv->backlight_duty_cycle = dev_priv->get_backlight(output);
}

static void
i830_lvds_restore(xf86OutputPtr output)
{
    ScrnInfoPtr	scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t pp_on_reg, pp_off_reg, pp_ctl_reg, pp_div_reg;
    uint32_t pwm_ctl_reg;

    if (IS_IGDNG(intel)) {
	pp_on_reg = PCH_PP_ON_DELAYS;
	pp_off_reg = PCH_PP_OFF_DELAYS;
	pp_ctl_reg = PCH_PP_CONTROL;
	pp_div_reg = PCH_PP_DIVISOR;
	pwm_ctl_reg = BLC_PWM_CPU_CTL;
    } else {
	pp_on_reg = PP_ON_DELAYS;
	pp_off_reg = PP_OFF_DELAYS;
	pp_ctl_reg = PP_CONTROL;
	pp_div_reg = PP_DIVISOR;
	pwm_ctl_reg = BLC_PWM_CTL;
    }

    if (IS_I965GM(intel) || IS_GM45(intel))
	OUTREG(BLC_PWM_CTL2, intel->saveBLC_PWM_CTL2);
    OUTREG(pwm_ctl_reg, intel->saveBLC_PWM_CTL);
    OUTREG(pp_on_reg, intel->savePP_ON);
    OUTREG(pp_off_reg, intel->savePP_OFF);
    OUTREG(pp_div_reg, intel->savePP_DIVISOR);
    OUTREG(pp_ctl_reg, intel->savePP_CONTROL);

    if (intel->savePP_CONTROL & POWER_TARGET_ON)
	i830SetLVDSPanelPower(output, TRUE);
    else
	i830SetLVDSPanelPower(output, FALSE);
}

static int
i830_lvds_mode_valid(xf86OutputPtr output, DisplayModePtr pMode)
{
    ScrnInfoPtr	scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    DisplayModePtr	    pFixedMode = intel->lvds_fixed_mode;

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
    ScrnInfoPtr		    scrn = output->scrn;
    xf86CrtcConfigPtr	    xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
    I830CrtcPrivatePtr	    intel_crtc = output->crtc->driver_private;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    uint32_t		    pfit_control = 0, pfit_pgm_ratios = 0;
    float		    panel_ratio, desired_ratio, vert_scale, horiz_scale;
    float		    horiz_ratio, vert_ratio;
    int left_border = 0, right_border = 0, top_border = 0, bottom_border = 0;
    int i;
    uint32_t		    hsync_width, vsync_width;
    uint32_t		    hblank_width, vblank_width;
    uint32_t		    hsync_pos, vsync_pos;
    Bool border = 0;

    for (i = 0; i < xf86_config->num_output; i++) {
	xf86OutputPtr other_output = xf86_config->output[i];

	if (other_output != output && other_output->crtc == output->crtc) {
	    xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		       "Can't enable LVDS and another output on the same "
		       "pipe\n");
	    return FALSE;
	}
    }

    if (!IS_IGDNG(intel) && intel_crtc->pipe == 0) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "Can't support LVDS on pipe A\n");
	return FALSE;
    }

    /* If we don't have a panel mode there's not much we can do */
    if (intel->lvds_fixed_mode == NULL)
	return TRUE;

    /* If we have timings from the BIOS for the panel, put them in
     * to the adjusted mode.  The CRTC will be set up for this mode,
     * with the panel scaling set up to source from the H/VDisplay
     * of the original mode.
     */
    adjusted_mode->HDisplay = intel->lvds_fixed_mode->HDisplay;
    adjusted_mode->HSyncStart = intel->lvds_fixed_mode->HSyncStart;
    adjusted_mode->HSyncEnd = intel->lvds_fixed_mode->HSyncEnd;
    adjusted_mode->HTotal = intel->lvds_fixed_mode->HTotal;
    adjusted_mode->VDisplay = intel->lvds_fixed_mode->VDisplay;
    adjusted_mode->VSyncStart = intel->lvds_fixed_mode->VSyncStart;
    adjusted_mode->VSyncEnd = intel->lvds_fixed_mode->VSyncEnd;
    adjusted_mode->VTotal = intel->lvds_fixed_mode->VTotal;
    adjusted_mode->Clock = intel->lvds_fixed_mode->Clock;
    xf86SetModeCrtc(adjusted_mode, INTERLACE_HALVE_V);

    /* Make sure pre-965s set dither correctly */
    if (!IS_I965G(intel) && intel->lvds_dither)
	pfit_control |= PANEL_8TO6_DITHER_ENABLE;

    /* Native modes don't need fitting */
    if (adjusted_mode->HDisplay == mode->HDisplay &&
	adjusted_mode->VDisplay == mode->VDisplay) {
	pfit_pgm_ratios = 0;
	border = 0;
	goto out;
    }

    /* only full screen scale for now */
    if (IS_IGDNG(intel))
	goto out;

    /* 965+ wants fuzzy fitting */
    if (IS_I965G(intel))
	pfit_control |= (intel_crtc->pipe << PFIT_PIPE_SHIFT) |
	    PFIT_FILTER_FUZZY;

    hsync_width = adjusted_mode->CrtcHSyncEnd - adjusted_mode->CrtcHSyncStart;
    vsync_width = adjusted_mode->CrtcVSyncEnd - adjusted_mode->CrtcVSyncStart;
    hblank_width = adjusted_mode->CrtcHBlankEnd -
					adjusted_mode->CrtcHBlankStart;
    vblank_width = adjusted_mode->CrtcVBlankEnd -
					adjusted_mode->CrtcVBlankStart;
    /*
     * Deal with panel fitting options.  Figure out how to stretch the image
     * based on its aspect ratio & the current panel fitting mode.
     */
    panel_ratio = (float)adjusted_mode->HDisplay /
 	(float)adjusted_mode->VDisplay;
    desired_ratio = (float)mode->HDisplay /
	(float)mode->VDisplay;

    /*
     * Enable automatic panel scaling for non-native modes so that they fill
     * the screen.  Should be enabled before the pipe is enabled, according to
     * register description and PRM.
     */
    /* Change the value here to see the borders for debugging */
    if (!IS_IGDNG(intel)) {
	    OUTREG(BCLRPAT_A, 0);
	    OUTREG(BCLRPAT_B, 0);
    }
    switch (dev_priv->fitting_mode) {
    case CENTER:
	/*
	 * For centered modes, we have to calculate border widths & heights and
	 * modify the values programmed into the CRTC.  Also need to make sure
	 * LVDS borders are enabled (see i830_display.c).
	 */
	left_border =
	    (intel->lvds_fixed_mode->HDisplay - mode->HDisplay) / 2;
	right_border = left_border;
	if (mode->HDisplay & 1)
	    right_border++;
	top_border =
	    (intel->lvds_fixed_mode->VDisplay - mode->VDisplay) / 2;
	bottom_border = top_border;
	if (mode->VDisplay & 1)
	    bottom_border++;

	/* Set active & border values */
	adjusted_mode->CrtcHDisplay = mode->HDisplay;
	/* keep the horizontal border be even */
	if (right_border & 1)
		right_border++;
	/* use the border directly instead of border minus one */
	adjusted_mode->CrtcHBlankStart = mode->HDisplay + right_border;
	/* keep the blank width constant */
	adjusted_mode->CrtcHBlankEnd = adjusted_mode->CrtcHBlankStart +
					hblank_width;
	/* get the hsync start position relative to hblank start */
	hsync_pos = (hblank_width - hsync_width) / 2;
	/* keep the hsync width constant and hsync start be even */
	if (hsync_pos & 1)
		hsync_pos++;
	adjusted_mode->CrtcHSyncStart = adjusted_mode->CrtcHBlankStart +
						hsync_pos;
	adjusted_mode->CrtcHSyncEnd = adjusted_mode->CrtcHSyncStart +
						hsync_width;
	adjusted_mode->CrtcVDisplay = mode->VDisplay;
	/* use the border instead of border minus one */
	adjusted_mode->CrtcVBlankStart = mode->VDisplay + bottom_border;
	adjusted_mode->CrtcVBlankEnd = adjusted_mode->CrtcVBlankStart +
						vblank_width;
	/* get the vsync start position relative to vblank start */
	vsync_pos = (vblank_width - vsync_width) / 2;
	adjusted_mode->CrtcVSyncStart = adjusted_mode->CrtcVBlankStart +
						vsync_pos;
	adjusted_mode->CrtcVSyncEnd = adjusted_mode->CrtcVSyncStart +
						vsync_width;
	border = 1;
	break;
    case FULL_ASPECT:
	/* Scale but preserve aspect ratio */
	pfit_control |= PFIT_ENABLE;
	if (IS_I965G(intel)) {
	    /*
	     * 965+ is easy, it does everything in hw
	     */
	    if (panel_ratio > desired_ratio)
		pfit_control |= PFIT_SCALING_PILLAR;
	    else if (panel_ratio < desired_ratio)
		pfit_control |= PFIT_SCALING_LETTER;
	    else
		pfit_control |= PFIT_SCALING_AUTO;
	} else {
	    /*
	     * For earlier chips we have to calculate the scaling ratio
	     * by hand and program it into the PFIT_PGM_RATIOS reg.
	     */
	    uint32_t horiz_bits, vert_bits, bits = 12;

	    horiz_ratio = ((float)mode->HDisplay) /
		((float)adjusted_mode->HDisplay);
	    vert_ratio = ((float)mode->VDisplay) /
		((float)adjusted_mode->VDisplay);

	    horiz_scale = ((float)adjusted_mode->HDisplay) /
		((float)mode->HDisplay);
	    vert_scale = ((float)adjusted_mode->VDisplay) /
		((float)mode->VDisplay);

	    /* Retain aspect ratio */
	    if (panel_ratio > desired_ratio) { /* Pillar */
		unsigned long scaled_width = (float)mode->HDisplay * vert_scale;

		horiz_ratio = vert_ratio;
		pfit_control |= VERT_AUTO_SCALE | VERT_INTERP_BILINEAR |
		    HORIZ_INTERP_BILINEAR;

		/* Pillar will have left/right borders */
		left_border =  (intel->lvds_fixed_mode->HDisplay -
				scaled_width) / 2;
		right_border = left_border;
		if (mode->HDisplay & 1) /* odd resolutions */
		    right_border++;

		/* keep the border be even */
		if (right_border & 1)
			right_border++;
		adjusted_mode->CrtcHDisplay = scaled_width;
		adjusted_mode->CrtcHBlankStart = scaled_width + right_border;
		adjusted_mode->CrtcHBlankEnd = adjusted_mode->CrtcHBlankStart +
							hblank_width;
		/* get the hsync start position relative to hblank start */
		hsync_pos = (hblank_width - hsync_width) / 2;
		/* keep the hsync start be even */
		if (hsync_pos & 1)
			hsync_pos++;
		adjusted_mode->CrtcHSyncStart = adjusted_mode->CrtcHBlankStart +
							hsync_pos;
		adjusted_mode->CrtcHSyncEnd = adjusted_mode->CrtcHSyncStart +
							hsync_width;
		border = 1;
	    } else if (panel_ratio < desired_ratio) { /* Letter */
		unsigned long scaled_height = (float)mode->VDisplay *
		    horiz_scale;

		vert_ratio = horiz_ratio;
		pfit_control |= HORIZ_AUTO_SCALE | VERT_INTERP_BILINEAR |
		    HORIZ_INTERP_BILINEAR;

		/* Letterbox will have top/bottom borders */
		top_border = (intel->lvds_fixed_mode->VDisplay -
			      scaled_height) / 2;
		bottom_border = top_border;
		if (mode->VDisplay & 1)
		    bottom_border++;

		adjusted_mode->CrtcVDisplay = scaled_height;
		/* use the border instead of border minus one */
		adjusted_mode->CrtcVBlankStart = scaled_height +
		    bottom_border;
		/* keep the Vblank width constant */
		adjusted_mode->CrtcVBlankEnd = adjusted_mode->CrtcVBlankStart +
							vblank_width;
		/* get the vsync start position relative to vblank start */
		vsync_pos = (vblank_width - vsync_width) / 2;
		adjusted_mode->CrtcVSyncStart = adjusted_mode->CrtcVBlankStart +
							vsync_pos;
		adjusted_mode->CrtcVSyncEnd = adjusted_mode->CrtcVBlankStart +
							vsync_width;
		border = 1;
	    } else { /* Aspects match, let hw scale both directions */
		pfit_control |= VERT_AUTO_SCALE | HORIZ_AUTO_SCALE |
		    VERT_INTERP_BILINEAR | HORIZ_INTERP_BILINEAR;
	    }

	    horiz_bits = 0.5 + (1 << bits) * horiz_ratio;
	    vert_bits = 0.5 + (1 << bits) * vert_ratio;

	    pfit_pgm_ratios = (((vert_bits << PFIT_VERT_SCALE_SHIFT) &
				PFIT_VERT_SCALE_MASK) |
			       ((horiz_bits << PFIT_HORIZ_SCALE_SHIFT) &
				PFIT_HORIZ_SCALE_MASK));
	}
	break;
    case FULL:
	/*
	 * Full scaling, even if it changes the aspect ratio.  Fortunately
	 * this is all done for us in hw.
	 */
	pfit_control |= PFIT_ENABLE;
	if (IS_I965G(intel))
	    pfit_control |= PFIT_SCALING_AUTO;
	else
	    pfit_control |= VERT_AUTO_SCALE | HORIZ_AUTO_SCALE |
		VERT_INTERP_BILINEAR | HORIZ_INTERP_BILINEAR;
	break;
    default:
	/* shouldn't happen */
	xf86DrvMsg(scrn->scrnIndex, X_ERROR, "error: bad fitting mode\n");
	break;
    }
  
out:
    dev_priv->pfit_control = pfit_control;
    dev_priv->pfit_pgm_ratios = pfit_pgm_ratios;

    if (border)
	intel_output->lvds_bits |= LVDS_BORDER_ENABLE;
    else
	intel_output->lvds_bits &= ~LVDS_BORDER_ENABLE;
    /* XXX: It would be nice to support lower refresh rates on the
     * panels to reduce power consumption, and perhaps match the
     * user's requested refresh rate.
     */

    return TRUE;
}

static void
i830_lvds_prepare(xf86OutputPtr output)
{
    i830_lvds_dpms(output, DPMSModeOff);
}

static void
i830_lvds_mode_set(xf86OutputPtr output, DisplayModePtr mode,
		   DisplayModePtr adjusted_mode)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;
    ScrnInfoPtr		    scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);

    /*
     * The LVDS pin pair will already have been turned on in the
     * main modesetting function since it has a large impact on the
     * DPLL settings.
     */

    if (IS_IGDNG(intel))
	return;

    /*
     * PFIT must be enabled/disabled while LVDS is on but pipes are still off
     */
    OUTREG(PFIT_PGM_RATIOS, dev_priv->pfit_pgm_ratios);
    OUTREG(PFIT_CONTROL, dev_priv->pfit_control);
}

/**
 * Detect the LVDS connection.
 */
static xf86OutputStatus
i830_lvds_detect(xf86OutputPtr output)
{
    enum lid_status lid;

    /* Fallback to origin, mark LVDS always connected.
     * From wider tests, we have seen both broken cases with
     * ACPI lid and SWF bit. So disable them for now until we
     * get a reliable way for LVDS detect.
     */
    return XF86OutputStatusConnected;

    lid = i830_lvds_acpi_lid_open(output);
    if (lid == LID_OPEN)
	return XF86OutputStatusConnected;
    else if (lid == LID_CLOSE)
	return XF86OutputStatusDisconnected;

    if (i830_lvds_swf_lid_close(output))
	return XF86OutputStatusDisconnected;

    return XF86OutputStatusConnected;
}

static void fill_detailed_block(struct detailed_monitor_section *det_mon,
                                DisplayModePtr mode)
{
    struct detailed_timings *timing = &det_mon->section.d_timings;
    det_mon->type = DT;
    timing->clock = mode->Clock * 1000;
    timing->h_active = mode->HDisplay;
    timing->h_blanking = mode->HTotal - mode->HDisplay;
    timing->v_active = mode->VDisplay;
    timing->v_blanking = mode->VTotal - mode->VDisplay;
    timing->h_sync_off = mode->HSyncStart - mode->HDisplay;
    timing->h_sync_width = mode->HSyncEnd - mode->HSyncStart;
    timing->v_sync_off = mode->VSyncStart - mode->VDisplay;
    timing->v_sync_width = mode->VSyncEnd - mode->VSyncStart;

    if (mode->Flags & V_PVSYNC)
        timing->misc |= 0x02;

    if (mode->Flags & V_PHSYNC)
        timing->misc |= 0x01;
}

/* X Server pre-1.5 compatibility */
#ifndef DS_VENDOR
#define DS_VENDOR 0x101
#endif

/**
 * Return the list of DDC modes if available, or the BIOS fixed mode otherwise.
 */
static DisplayModePtr
i830_lvds_get_modes(xf86OutputPtr output)
{
    ScrnInfoPtr	scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    xf86MonPtr		    edid_mon;
    DisplayModePtr	    modes;

    edid_mon = xf86OutputGetEDID (output, intel_output->pDDCBus);

    /* Our LVDS scaler can hit any size, so mark the EDID data as
     * supporting continuous timings
     */
    if (edid_mon) {
	int i, j = -1;
	edid_mon->features.msc |= 0x1;

	/* Either find a DS_RANGES block, or replace a DS_VENDOR block,
	 * smashing it into a DS_RANGES block with wide open refresh to
	 * match all default modes
	 */
	for (i = 0; i < sizeof (edid_mon->det_mon) / sizeof (edid_mon->det_mon[0]); i++)
	{
	    if (edid_mon->det_mon[i].type >= DS_VENDOR && j == -1)
		j = i;
	    if (edid_mon->det_mon[i].type == DS_RANGES) {
		j = i;
		break;
	    }
	}
	if (j != -1) {
	    struct monitor_ranges   *ranges = &edid_mon->det_mon[j].section.ranges;
	    edid_mon->det_mon[j].type = DS_RANGES;
	    ranges->min_v = 0;
	    ranges->max_v = 200;
	    ranges->min_h = 0;
	    ranges->max_h = 200;
	}
    }
    xf86OutputSetEDID (output, edid_mon);

    modes = xf86OutputGetEDIDModes (output);
    if (modes != NULL)
	return modes;

    if (!output->MonInfo)
    {
	edid_mon = calloc (1, sizeof (xf86Monitor));
	if (edid_mon)
	{
	    struct detailed_monitor_section *det_mon = edid_mon->det_mon;
	    /*support DPM, instead of DPMS*/
	    edid_mon->features.dpms |= 0x1;
	    /*defaultly support RGB color display*/
	    edid_mon->features.display_type |= 0x1;
	    /*defaultly display support continuous-freqencey*/
	    edid_mon->features.msc |= 0x1;
	    /*defaultly  the EDID version is 1.4 */
	    edid_mon->ver.version = 1;
	    edid_mon->ver.revision = 4;

	    if (intel->lvds_fixed_mode != NULL) {
		/* now we construct new EDID monitor,
		 *  so filled one detailed timing block
		 */
		fill_detailed_block(det_mon, intel->lvds_fixed_mode);
		/* the filed timing block should be set preferred*/
		edid_mon->features.msc |= 0x2;
		det_mon = det_mon + 1;
	    }

	    /* Set wide sync ranges so we get all modes
	     * handed to valid_mode for checking
	     */
	    det_mon->type = DS_RANGES;
	    det_mon->section.ranges.min_v = 0;
	    det_mon->section.ranges.max_v = 200;
	    det_mon->section.ranges.min_h = 0;
	    det_mon->section.ranges.max_h = 200;
	    output->MonInfo = edid_mon;
	}
    }

    if (intel->lvds_fixed_mode != NULL)
	return xf86DuplicateMode(intel->lvds_fixed_mode);

    return NULL;
}

static void
i830_lvds_destroy (xf86OutputPtr output)
{
    ScrnInfoPtr	scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;

    xf86DeleteMode (&intel->lvds_fixed_mode, intel->lvds_fixed_mode);
    if (intel_output)
	free (intel_output);
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
#define NUM_BACKLIGHT_CONTROL_METHODS 5
static char *backlight_control_names[] = {
    "native",
    "legacy",
    "combination",
    "kernel",
    "null",
};
static Atom backlight_control_atom;
static Atom backlight_control_name_atoms[NUM_BACKLIGHT_CONTROL_METHODS];

#define PANEL_FITTING_NAME "PANEL_FITTING"
#define NUM_PANEL_FITTING_TYPES 3
static char *panel_fitting_names[] = {
    "center",
    "full_aspect",
    "full",
};
static Atom panel_fitting_atom;
static Atom panel_fitting_name_atoms[NUM_PANEL_FITTING_TYPES];


static int
i830_backlight_control_lookup(const char *name)
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
    ScrnInfoPtr		    scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;

    switch (intel->backlight_control_method) {
    case BCM_NATIVE:
	dev_priv->set_backlight = i830_lvds_set_backlight_native;
	dev_priv->get_backlight = i830_lvds_get_backlight_native;
	dev_priv->backlight_max =
	    i830_lvds_get_backlight_max_native(output);
	break;
    case BCM_LEGACY:
	dev_priv->set_backlight = i830_lvds_set_backlight_legacy;
	dev_priv->get_backlight = i830_lvds_get_backlight_legacy;
	dev_priv->backlight_max = 0xff;
	break;
    case BCM_COMBO:
	dev_priv->set_backlight = i830_lvds_set_backlight_combo;
	dev_priv->get_backlight = i830_lvds_get_backlight_combo;
	dev_priv->backlight_max =
	    i830_lvds_get_backlight_max_combo(output);
	break;
    case BCM_KERNEL:
	dev_priv->set_backlight = i830_lvds_set_backlight_kernel;
	dev_priv->get_backlight = i830_lvds_get_backlight_kernel;
	dev_priv->backlight_max =
	    i830_lvds_get_backlight_max_kernel(output);
	break;
    case BCM_IRONLAKE_NULL:
	dev_priv->set_backlight = i830_lvds_set_backlight_null;
	dev_priv->get_backlight = i830_lvds_get_backlight_null;
	dev_priv->backlight_max = 1;
	break;
    default:
	/*
	 * Should be impossible to get here unless the caller set a bogus
	 * backlight_control_method
	 */
	xf86DrvMsg(scrn->scrnIndex, X_ERROR, "error: bad backlight control "
		   "method\n");
	break;
    }

    return Success;
}

static int
i830_panel_fitting_lookup(const char *name)
{
    int i;

    for (i = 0; i < NUM_PANEL_FITTING_TYPES; i++)
	if (!strcmp(name, panel_fitting_names[i]))
	    return i;

    return -1;
}
#endif /* RANDR_12_INTERFACE */

static void
i830_lvds_create_resources(xf86OutputPtr output)
{
#ifdef RANDR_12_INTERFACE
    ScrnInfoPtr		    scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
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
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "RRConfigureOutputProperty error, %d\n", err);
    }
    /* Set the current value of the backlight property */
    data = dev_priv->backlight_duty_cycle;
    err = RRChangeOutputProperty(output->randr_output, backlight_atom,
				 XA_INTEGER, 32, PropModeReplace, 1, &data,
				 FALSE, TRUE);
    if (err != 0) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
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
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "RRConfigureOutputProperty error, %d\n", err);
    }
    err = RRChangeOutputProperty(output->randr_output, backlight_control_atom,
				 XA_ATOM, 32, PropModeReplace, 1,
				 &backlight_control_name_atoms[intel->backlight_control_method],
				 FALSE, TRUE);
    if (err != 0) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "failed to set backlight control, %d\n", err);
    }

    /*
     * Panel fitting control
     */

    /* Disable panel fitting setting on untested pre-915 chips */
    if (!IS_I9XX(intel) && !(intel->quirk_flag & QUIRK_PFIT_SAFE))
	return;

    panel_fitting_atom = MakeAtom(PANEL_FITTING_NAME,
				  sizeof(PANEL_FITTING_NAME) - 1, TRUE);
    for (i = 0; i < NUM_PANEL_FITTING_TYPES; i++) {
	panel_fitting_name_atoms[i] = MakeAtom(panel_fitting_names[i],
					       strlen(panel_fitting_names[i]),
					       TRUE);
    }
    err = RRConfigureOutputProperty(output->randr_output,
				    panel_fitting_atom, TRUE, FALSE, FALSE,
				    NUM_PANEL_FITTING_TYPES,
				    (INT32 *)panel_fitting_name_atoms);
    if (err != 0) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "RRConfigureOutputProperty error, %d\n", err);
    }
    err = RRChangeOutputProperty(output->randr_output, panel_fitting_atom,
				 XA_ATOM, 32, PropModeReplace, 1,
				 &panel_fitting_name_atoms[dev_priv->fitting_mode],
				 FALSE, TRUE);
    if (err != 0) {
	xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		   "failed to set panel fitting mode, %d\n", err);
    }   
#endif /* RANDR_12_INTERFACE */
}

#ifdef RANDR_12_INTERFACE
static Bool
i830_lvds_set_property(xf86OutputPtr output, Atom property,
		       RRPropertyValuePtr value)
{
    ScrnInfoPtr		    scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
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
	const char		*name;
	int			ret, data;

	if (value->type != XA_ATOM || value->format != 32 || value->size != 1)
	    return FALSE;

	memcpy(&atom, value->data, 4);
	name = NameForAtom(atom);
	
	ret = i830_backlight_control_lookup(name);
	if (ret < 0)
	    return FALSE;

	intel->backlight_control_method = ret;
	i830_lvds_set_backlight_control(output);

	/*
	 * Update the backlight atom since the range and value may have changed
	 */
	backlight_range[0] = 0;
	backlight_range[1] = dev_priv->backlight_max;
	ret = RRConfigureOutputProperty(output->randr_output, backlight_atom,
					FALSE, TRUE, FALSE, 2, backlight_range);
	if (ret != 0) {
	    xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		       "RRConfigureOutputProperty error, %d\n", ret);
	}
	/* Set the current value of the backlight property */
	if ((INREG(PP_CONTROL) & POWER_TARGET_ON) && !dev_priv->dpmsoff) 
	    data = dev_priv->get_backlight(output);
	else
	    data = dev_priv->backlight_duty_cycle;
	ret = RRChangeOutputProperty(output->randr_output, backlight_atom,
				     XA_INTEGER, 32, PropModeReplace, 1, &data,
				     FALSE, TRUE);
	if (ret != 0) {
	    xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		       "RRChangeOutputProperty error, %d\n", ret);
	}
	return TRUE;
    } else if (property == panel_fitting_atom) {
	Atom			atom;
	const char		*name;
	int			ret;

	if (value->type != XA_ATOM || value->format != 32 || value->size != 1)
	    return FALSE;

	memcpy(&atom, value->data, 4);
	name = NameForAtom(atom);
	
	ret = i830_panel_fitting_lookup(name);
	if (ret < 0)
	    return FALSE;

	if (dev_priv->fitting_mode == ret)
	    return TRUE;

	dev_priv->fitting_mode = ret;

	if (output->crtc) {
	    xf86CrtcPtr crtc = output->crtc;
	    if (crtc->enabled) {
		if (!xf86CrtcSetMode(crtc, &crtc->desiredMode,
				     crtc->desiredRotation,
				     crtc->desiredX, crtc->desiredY)) {
		    xf86DrvMsg(scrn->scrnIndex, X_ERROR,
			       "Failed to set mode after panel fitting change!\n");
		    return FALSE;
		}
	    }
	}
	return TRUE;
    }

    return TRUE;
}
#endif /* RANDR_12_INTERFACE */

#ifdef RANDR_13_INTERFACE
static Bool
i830_lvds_get_property(xf86OutputPtr output, Atom property)
{
    ScrnInfoPtr		    scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct i830_lvds_priv   *dev_priv = intel_output->dev_priv;
    int ret;

    /*
     * Only need to update properties that might change out from under
     * us.  The others will be cached by the randr core code.
     */
    if (property == backlight_atom) {
	int val;
	if ((INREG(PP_CONTROL) & POWER_TARGET_ON) && !dev_priv->dpmsoff) {
	    val = dev_priv->get_backlight(output);
	    dev_priv->backlight_duty_cycle = val;
	} else
	    val = dev_priv->backlight_duty_cycle;
	ret = RRChangeOutputProperty(output->randr_output, backlight_atom,
				     XA_INTEGER, 32, PropModeReplace, 1, &val,
				     FALSE, TRUE);
	if (ret != Success)
	    return FALSE;
    }

    return TRUE;
}
#endif /* RANDR_13_INTERFACE */

#ifdef RANDR_GET_CRTC_INTERFACE
static xf86CrtcPtr
i830_lvds_get_crtc(xf86OutputPtr output)
{
    ScrnInfoPtr	scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    int pipe = !!(INREG(LVDS) & LVDS_PIPEB_SELECT);
   
    return i830_pipe_to_crtc(scrn, pipe);
}
#endif

static const xf86OutputFuncsRec i830_lvds_output_funcs = {
    .create_resources = i830_lvds_create_resources,
    .dpms = i830_lvds_dpms,
    .save = i830_lvds_save,
    .restore = i830_lvds_restore,
    .mode_valid = i830_lvds_mode_valid,
    .mode_fixup = i830_lvds_mode_fixup,
    .prepare = i830_lvds_prepare,
    .mode_set = i830_lvds_mode_set,
    .commit = i830_output_commit,
    .detect = i830_lvds_detect,
    .get_modes = i830_lvds_get_modes,
#ifdef RANDR_12_INTERFACE
    .set_property = i830_lvds_set_property,
#endif
#ifdef RANDR_13_INTERFACE
    .get_property = i830_lvds_get_property,
#endif
    .destroy = i830_lvds_destroy,
#ifdef RANDR_GET_CRTC_INTERFACE
    .get_crtc = i830_lvds_get_crtc,
#endif
};

void
i830_lvds_init(ScrnInfoPtr scrn)
{
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    xf86OutputPtr	    output;
    I830OutputPrivatePtr    intel_output;
    DisplayModePtr	    modes, scan;
    DisplayModePtr	    lvds_ddc_mode = NULL;
    struct i830_lvds_priv   *dev_priv;
    int			    gpio = GPIOC;

    if (!intel->integrated_lvds) {
	if (intel->debug_modes)
	    xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "Skipping LVDS from driver feature BDB's LVDS config info.\n");
	return;
    }

    if (intel->quirk_flag & QUIRK_IGNORE_LVDS)
	return;

    if (IS_IGDNG(intel)) {
      if ((INREG(PCH_LVDS) & LVDS_DETECTED) == 0)
	return;
      gpio = PCH_GPIOC;
    }

    output = xf86OutputCreate (scrn, &i830_lvds_output_funcs, "LVDS");
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
    if (0 && IS_IGDNG(intel)) /* XXX put me back */
	intel_output->pipe_mask |= (1 << 0);
    intel_output->clone_mask = (1 << I830_OUTPUT_LVDS);
    
    output->driver_private = intel_output;
    output->subpixel_order = SubPixelHorizontalRGB;
    output->interlaceAllowed = FALSE;
    output->doubleScanAllowed = FALSE;

    dev_priv = (struct i830_lvds_priv *) (intel_output + 1);
    intel_output->dev_priv = dev_priv;
    
    /*
     * Mode detection algorithms for LFP:
     *  1) if EDID present, use it, done
     *  2) if VBT present, use it, done
     *  3) if current mode is programmed, use it, done
     *  4) check for Mac mini & other quirks
     *  4) fail, assume no LFP
     */

    /* Set up the LVDS DDC channel.  Most panels won't support it, but it can
     * be useful if available.
     */
    I830I2CInit(scrn, &intel_output->pDDCBus, gpio, "LVDSDDC_C");

    if (intel->skip_panel_detect) {
	xf86DrvMsg(scrn->scrnIndex, X_INFO,
		   "Skipping any attempt to determine panel fixed mode.\n");
	goto found_mode;
    }

    xf86DrvMsg(scrn->scrnIndex, X_INFO,
	       "Attempting to determine panel fixed mode.\n");

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
	lvds_ddc_mode = scan;
    }
    /* Delete the mode list */
    while (modes != NULL)
	xf86DeleteMode(&modes, modes);

    if (lvds_ddc_mode) {
	    intel->lvds_fixed_mode = lvds_ddc_mode;
	    goto found_mode;
    }

    /* Get the LVDS fixed mode out of the BIOS.  We should support LVDS with
     * the BIOS being unavailable or broken, but lack the configuration options
     * for now.
     */
    if (intel->lvds_fixed_mode)
	    goto found_mode;

    /* If we *still* don't have a mode, try checking if the panel is already
     * turned on.  If so, assume that whatever is currently programmed is the
     * correct mode.
     */
    if (!intel->lvds_fixed_mode && !IS_IGDNG(intel)) {
	uint32_t lvds = INREG(LVDS);
	int pipe = (lvds & LVDS_PIPEB_SELECT) ? 1 : 0;
	xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(scrn);
	xf86CrtcPtr crtc = xf86_config->crtc[pipe];

	if (lvds & LVDS_PORT_EN) {
	    intel->lvds_fixed_mode = i830_crtc_mode_get(scrn, crtc);
	    if (intel->lvds_fixed_mode != NULL) {
		intel->lvds_fixed_mode->type |= M_T_PREFERRED;
		goto found_mode;
	    }
	}
    }

    if (!intel->lvds_fixed_mode)
	    goto disable_exit;

found_mode:

    /* Blacklist machines with BIOSes that list an LVDS panel without actually
     * having one.
     */
    if (intel->quirk_flag & QUIRK_IGNORE_MACMINI_LVDS) {
	/* It's a Mac Mini or Macbook Pro.
	 *
	 * Apple hardware is out to get us.  The macbook pro has a real
	 * LVDS panel, but the mac mini does not, and they have the same
	 * device IDs.  We'll distinguish by panel size, on the assumption
	 * that Apple isn't about to make any machines with an 800x600
	 * display.
	 */

	if (intel->lvds_fixed_mode != NULL &&
		intel->lvds_fixed_mode->HDisplay == 800 &&
		intel->lvds_fixed_mode->VDisplay == 600)
	{
	    xf86DrvMsg(scrn->scrnIndex, X_INFO,
		    "Suspected Mac Mini, ignoring the LVDS\n");
	    goto disable_exit;
	}
    }

    i830_set_lvds_backlight_method(output);

    switch (intel->backlight_control_method) {
    case BCM_NATIVE:
	dev_priv->set_backlight = i830_lvds_set_backlight_native;
	dev_priv->get_backlight = i830_lvds_get_backlight_native;
	dev_priv->backlight_max = i830_lvds_get_backlight_max_native(output);
	break;
    case BCM_LEGACY:
	dev_priv->set_backlight = i830_lvds_set_backlight_legacy;
	dev_priv->get_backlight = i830_lvds_get_backlight_legacy;
	dev_priv->backlight_max = 0xff;
	break;
    case BCM_COMBO:
	dev_priv->set_backlight = i830_lvds_set_backlight_combo;
	dev_priv->get_backlight = i830_lvds_get_backlight_combo;
	dev_priv->backlight_max = i830_lvds_get_backlight_max_combo(output);
	break;
    case BCM_KERNEL:
	dev_priv->set_backlight = i830_lvds_set_backlight_kernel;
	dev_priv->get_backlight = i830_lvds_get_backlight_kernel;
	dev_priv->backlight_max = i830_lvds_get_backlight_max_kernel(output);
	break;
    default:
	xf86DrvMsg(scrn->scrnIndex, X_ERROR, "bad backlight control method\n");
	break;
    }

    dev_priv->backlight_duty_cycle = dev_priv->get_backlight(output);

    /*
     * Avoid munging the aspect ratio by default.
     */
    dev_priv->fitting_mode = FULL_ASPECT;

    if (IS_IGDNG(intel)) {
	CARD32 pwm;
	/* make sure PWM is enabled */
	pwm = INREG(BLC_PWM_CPU_CTL2);
	pwm |= (PWM_ENABLE | PWM_PIPE_B);
	OUTREG(BLC_PWM_CPU_CTL2, pwm);

	pwm = INREG(BLC_PWM_PCH_CTL1);
	pwm |= PWM_PCH_ENABLE;
	OUTREG(BLC_PWM_PCH_CTL1, pwm);
    }


    return;

disable_exit:
    xf86DestroyI2CBusRec(intel_output->pDDCBus, TRUE, TRUE);
    xf86OutputDestroy(output);
}
