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

#include "xf86.h"
#include "i830.h"
#include "xf86Modes.h"
#include "i830_display.h"

static void
i830_crt_dpms(xf86OutputPtr output, int mode)
{
    ScrnInfoPtr	    scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t	    temp;

    temp = INREG(ADPA);
    temp &= ~(ADPA_HSYNC_CNTL_DISABLE | ADPA_VSYNC_CNTL_DISABLE);
    temp &= ~ADPA_DAC_ENABLE;

    switch(mode) {
    case DPMSModeOn:
	temp |= ADPA_DAC_ENABLE;
	break;
    case DPMSModeStandby:
	temp |= ADPA_DAC_ENABLE | ADPA_HSYNC_CNTL_DISABLE;
	break;
    case DPMSModeSuspend:
	temp |= ADPA_DAC_ENABLE | ADPA_VSYNC_CNTL_DISABLE;
	break;
    case DPMSModeOff:
	temp |= ADPA_HSYNC_CNTL_DISABLE | ADPA_VSYNC_CNTL_DISABLE;
	break;
    }

    OUTREG(ADPA, temp);
}

static void
i830_crt_save (xf86OutputPtr output)
{
    ScrnInfoPtr	scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);

    intel->saveADPA = INREG(ADPA);
}

static void
i830_crt_restore (xf86OutputPtr output)
{
    ScrnInfoPtr	scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);

    OUTREG(ADPA, intel->saveADPA);
}

static int
i830_crt_mode_valid(xf86OutputPtr output, DisplayModePtr pMode)
{
    ScrnInfoPtr scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    int		maxclock;

    if (pMode->Flags & V_DBLSCAN)
	return MODE_NO_DBLESCAN;

    if (pMode->Clock < 25000)
	return MODE_CLOCK_LOW;

    if (!IS_I9XX(intel))
	maxclock = 350000;
    else
	maxclock = 400000;

    if (pMode->Clock > maxclock)
	return MODE_CLOCK_HIGH;

    return MODE_OK;
}

static Bool
i830_crt_mode_fixup(xf86OutputPtr output, DisplayModePtr mode,
		    DisplayModePtr adjusted_mode)
{
    return TRUE;
}

static void
i830_crt_mode_set(xf86OutputPtr output, DisplayModePtr mode,
		  DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr		    scrn = output->scrn;
    intel_screen_private   *intel = intel_get_screen_private(scrn);
    xf86CrtcPtr		    crtc = output->crtc;
    I830CrtcPrivatePtr	    i830_crtc = crtc->driver_private;
    int			    dpll_md_reg;
    uint32_t		    adpa, dpll_md;

    if (i830_crtc->pipe == 0) 
	dpll_md_reg = DPLL_A_MD;
    else
	dpll_md_reg = DPLL_B_MD;
    /*
     * Disable separate mode multiplier used when cloning SDVO to CRT
     * XXX this needs to be adjusted when we really are cloning
     */
    if (IS_I965G(intel))
    {
	dpll_md = INREG(dpll_md_reg);
	OUTREG(dpll_md_reg, dpll_md & ~DPLL_MD_UDI_MULTIPLIER_MASK);
    }

    adpa = 0;
    if (adjusted_mode->Flags & V_PHSYNC)
	adpa |= ADPA_HSYNC_ACTIVE_HIGH;
    if (adjusted_mode->Flags & V_PVSYNC)
	adpa |= ADPA_VSYNC_ACTIVE_HIGH;

    if (i830_crtc->pipe == 0)
    {
	adpa |= ADPA_PIPE_A_SELECT;
	OUTREG(BCLRPAT_A, 0);
    }
    else
    {
	adpa |= ADPA_PIPE_B_SELECT;
	OUTREG(BCLRPAT_B, 0);
    }

    OUTREG(ADPA, adpa);
}

/**
 * Uses CRT_HOTPLUG_EN and CRT_HOTPLUG_STAT to detect CRT presence.
 *
 * Only for I945G/GM.
 *
 * \return TRUE if CRT is connected.
 * \return FALSE if CRT is disconnected.
 */
static Bool
i830_crt_detect_hotplug(xf86OutputPtr output)
{
    ScrnInfoPtr	scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t	hotplug_en, temp;
    const int	timeout_ms = 1000;
    int		starttime, curtime;
    int		tries = 1;
    int		try;

    /* On 4 series desktop, CRT detect sequence need to be done twice
     * to get a reliable result. */
    if (IS_G4X(intel) && !IS_GM45(intel))
	tries = 2;
    else
	tries = 1;

    hotplug_en = INREG(PORT_HOTPLUG_EN);

    hotplug_en &= CRT_FORCE_HOTPLUG_MASK;

    /* This starts the detection sequence */
    hotplug_en |= CRT_HOTPLUG_FORCE_DETECT;

    /* GM45 requires a longer activation period to reliably
     * detect CRT
     */
    if (IS_G4X(intel))
	hotplug_en |= CRT_HOTPLUG_ACTIVATION_PERIOD_64;

    /* Use the default voltage value */
    hotplug_en |= CRT_HOTPLUG_VOLTAGE_COMPARE_50;

    for (try = 0; try < tries; try++) {
	/* turn FORCE_DETECT on */
	OUTREG(PORT_HOTPLUG_EN, hotplug_en);

	/* wait for FORCE_DETECT to go off */
	for (curtime = starttime = GetTimeInMillis();
	     (curtime - starttime) < timeout_ms;
	     curtime = GetTimeInMillis())
	{
	    temp = INREG(PORT_HOTPLUG_EN);

	    if ((temp & CRT_HOTPLUG_FORCE_DETECT) == 0)
		break;
	}
    }

    /* Check the status to see if both blue and green are on now */
    temp = INREG(PORT_HOTPLUG_STAT);
    return ((temp & CRT_HOTPLUG_MONITOR_MASK) ==
	    CRT_HOTPLUG_MONITOR_COLOR);
}

/**
 * Detects CRT presence by checking for load.
 *
 * Requires that the current pipe's DPLL is active.  This will cause flicker
 * on the CRT, so it should not be used while the display is being used.  Only
 * color (not monochrome) displays are detected.
 *
 * \return TRUE if CRT is connected.
 * \return FALSE if CRT is disconnected.
 */
static Bool
i830_crt_detect_load (xf86CrtcPtr	    crtc,
		      xf86OutputPtr    output)
{
    ScrnInfoPtr		    scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    I830CrtcPrivatePtr	    i830_crtc = I830CrtcPrivate(crtc);
    uint32_t		    save_bclrpat;
    uint32_t		    save_vtotal;
    uint32_t		    vtotal, vactive;
    uint32_t		    vsample;
    uint32_t		    vblank, vblank_start, vblank_end;
    uint32_t		    dsl;
    uint8_t		    st00;
    int			    bclrpat_reg, pipeconf_reg, pipe_dsl_reg;
    int			    vtotal_reg, vblank_reg, vsync_reg;
    int			    pipe = i830_crtc->pipe;
    Bool		    present;

    if (pipe == 0) 
    {
	bclrpat_reg = BCLRPAT_A;
	vtotal_reg = VTOTAL_A;
	vblank_reg = VBLANK_A;
	vsync_reg = VSYNC_A;
	pipeconf_reg = PIPEACONF;
	pipe_dsl_reg = PIPEA_DSL;
    }
    else 
    {
	bclrpat_reg = BCLRPAT_B;
	vtotal_reg = VTOTAL_B;
	vblank_reg = VBLANK_B;
	vsync_reg = VSYNC_B;
	pipeconf_reg = PIPEBCONF;
	pipe_dsl_reg = PIPEB_DSL;
    }

    save_bclrpat = INREG(bclrpat_reg);
    save_vtotal = INREG(vtotal_reg);
    vblank = INREG(vblank_reg);
    
    vtotal = ((save_vtotal >> 16) & 0xfff) + 1;
    vactive = (save_vtotal & 0x7ff) + 1;

    vblank_start = (vblank & 0xfff) + 1;
    vblank_end = ((vblank >> 16) & 0xfff) + 1;
    
    /* Set the border color to purple. */
    OUTREG(bclrpat_reg, 0x500050);
    
    if (IS_I9XX (intel))
    {
	uint32_t	pipeconf = INREG(pipeconf_reg);
	OUTREG(pipeconf_reg, pipeconf | PIPECONF_FORCE_BORDER);
        /* Wait for next Vblank to substitue border color for Color info */
        i830WaitForVblank (scrn);
	st00 = intel->readStandard (intel, 0x3c2);
	present = (st00 & (1 << 4)) != 0;
	OUTREG(pipeconf_reg, pipeconf);
    }
    else
    {
	Bool	restore_vblank = FALSE;
	int	count, detect;

	/*
	 * If there isn't any border, add some.
	 * Yes, this will flicker
	 */
	if (vblank_start <= vactive && vblank_end >= vtotal)
	{
	    uint32_t  vsync = INREG(vsync_reg);
	    uint32_t  vsync_start = (vsync & 0xffff) + 1;

	    vblank_start = vsync_start;
	    OUTREG(vblank_reg, (vblank_start - 1) | ((vblank_end - 1) << 16));
	    restore_vblank = TRUE;
	}
	
	/* sample in the vertical border, selecting the larger one */
	if (vblank_start - vactive >= vtotal - vblank_end)
	    vsample = (vblank_start + vactive) >> 1;
	else
	    vsample = (vtotal + vblank_end) >> 1;

	/*
	 * Wait for the border to be displayed
	 */
	while (INREG(pipe_dsl_reg) >= vactive)
	    ;
	while ((dsl = INREG(pipe_dsl_reg)) <= vsample)
	    ;
	/*
	 * Watch ST00 for an entire scanline
	 */
	detect = 0;
	count = 0;
	do {
	    count++;
	    /* Read the ST00 VGA status register */
	    st00 = intel->readStandard(intel, 0x3c2);
	    if (st00 & (1 << 4))
		detect++;
	} while ((INREG(pipe_dsl_reg) == dsl));
	
	/* restore vblank if necessary */
	if (restore_vblank)
	    OUTREG(vblank_reg, vblank);
	/*
	 * If more than 3/4 of the scanline detected a monitor,
	 * then it is assumed to be present. This works even on i830,
	 * where there isn't any way to force the border color across
	 * the screen
	 */
	present = detect * 4 > count * 3;
    }

    /* Restore previous settings */
    OUTREG(bclrpat_reg, save_bclrpat);

    return present;
}

/**
 * Detects CRT presence by probing for a response on the DDC address.
 *
 * This takes approximately 5ms in testing on an i915GM, with CRT connected or
 * not.
 *
 * \return TRUE if the CRT is connected and responded to DDC.
 * \return FALSE if no DDC response was detected.
 */
static Bool
i830_crt_detect_ddc(xf86OutputPtr output)
{
    ScrnInfoPtr		    scrn = output->scrn;
    I830OutputPrivatePtr    i830_output = output->driver_private;
    Bool detect;

    /* CRT should always be at 0, but check anyway */
    if (i830_output->type != I830_OUTPUT_ANALOG)
	return FALSE;

    I830I2CInit(scrn, &i830_output->pDDCBus, GPIOA, "CRTDDC_A");
    detect = xf86I2CProbeAddress(i830_output->pDDCBus, 0x00A0);
    xf86DestroyI2CBusRec(i830_output->pDDCBus, TRUE, TRUE);

    return detect;
}

/**
 * Attempts to detect CRT presence through any method available.
 *
 * @param allow_disturb enables detection methods that may cause flickering
 *        on active displays.
 */
static xf86OutputStatus
i830_crt_detect(xf86OutputPtr output)
{
    ScrnInfoPtr		    scrn = output->scrn;
    intel_screen_private    *intel = intel_get_screen_private(scrn);
    xf86CrtcPtr		    crtc;
    int			    dpms_mode;
    xf86OutputStatus	    status;
    Bool		    connected;

    /*
     * Try hotplug detection where supported
     */
    if (IS_I945G(intel) || IS_I945GM(intel) || IS_I965G(intel) ||
	    IS_G33CLASS(intel)) {
	if (i830_crt_detect_hotplug(output))
	    status = XF86OutputStatusConnected;
	else
	    status = XF86OutputStatusDisconnected;

	goto done;
    }

    /*
     * DDC is next best, no flicker
     */
    crtc = i830GetLoadDetectPipe (output, NULL, &dpms_mode);
    if (!crtc)
	return XF86OutputStatusUnknown;

    if (i830_crt_detect_ddc(output)) {
	status = XF86OutputStatusConnected;
	goto out_release_pipe;
    }

    /* Use the load-detect method if we have no other way of telling. */
    connected = i830_crt_detect_load (crtc, output);
    if (connected)
	status = XF86OutputStatusConnected;
    else
	status = XF86OutputStatusDisconnected;

out_release_pipe:
    i830ReleaseLoadDetectPipe (output, dpms_mode);

done:
    return status;
}

static void
i830_crt_destroy (xf86OutputPtr output)
{
    if (output->driver_private)
	xfree (output->driver_private);
}

#ifdef RANDR_GET_CRTC_INTERFACE
static xf86CrtcPtr
i830_crt_get_crtc(xf86OutputPtr output)
{
    ScrnInfoPtr	scrn = output->scrn;
    intel_screen_private *intel = intel_get_screen_private(scrn);
    int pipe = !!(INREG(ADPA) & ADPA_PIPE_SELECT_MASK);
   
    return i830_pipe_to_crtc(scrn, pipe);
}
#endif

static xf86MonPtr
i830_get_edid(xf86OutputPtr output, int gpio_reg, char *gpio_str)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    xf86MonPtr		    edid_mon = NULL;

    /* Set up the DDC bus. */
    I830I2CInit(output->scrn, &intel_output->pDDCBus, gpio_reg, gpio_str);

    edid_mon = xf86OutputGetEDID (output, intel_output->pDDCBus);

    if (!edid_mon || DIGITAL(edid_mon->features.input_type)) {
	xf86DestroyI2CBusRec(intel_output->pDDCBus, TRUE, TRUE);
	intel_output->pDDCBus = NULL;
	if (edid_mon) {
	    xfree(edid_mon);
	    edid_mon = NULL;
	}
    }

    return edid_mon;
}

static DisplayModePtr
i830_crt_get_modes (xf86OutputPtr output)
{
    DisplayModePtr	    modes;
    xf86MonPtr		    edid_mon = NULL;
    I830OutputPrivatePtr    intel_output = output->driver_private;

    /* Try to probe normal CRT port, and also digital port for output
       in DVI-I mode. */
    if ((edid_mon = i830_get_edid(output, GPIOA, "CRTDDC_A")))
	goto found;
    if ((edid_mon = i830_get_edid(output, GPIOD, "CRTDDC_D")))
	goto found;
    if ((edid_mon = i830_get_edid(output, GPIOE, "CRTDDC_E")))
	goto found;
found:
    /* Destroy DDC bus after probe, so every other new probe will
       scan all ports again */
    if (intel_output->pDDCBus)
	xf86DestroyI2CBusRec(intel_output->pDDCBus, TRUE, TRUE);

    xf86OutputSetEDID (output, edid_mon);

    modes = xf86OutputGetEDIDModes (output);
    return modes;
}

static const xf86OutputFuncsRec i830_crt_output_funcs = {
    .dpms = i830_crt_dpms,
    .save = i830_crt_save,
    .restore = i830_crt_restore,
    .mode_valid = i830_crt_mode_valid,
    .mode_fixup = i830_crt_mode_fixup,
    .prepare = i830_output_prepare,
    .mode_set = i830_crt_mode_set,
    .commit = i830_output_commit,
    .detect = i830_crt_detect,
    .get_modes = i830_crt_get_modes,
    .destroy = i830_crt_destroy,
#ifdef RANDR_GET_CRTC_INTERFACE
    .get_crtc = i830_crt_get_crtc,
#endif
};

void
i830_crt_init(ScrnInfoPtr scrn)
{
    xf86OutputPtr	    output;
    I830OutputPrivatePtr    i830_output;
    intel_screen_private    *intel = intel_get_screen_private(scrn);

    if (intel->quirk_flag & QUIRK_IGNORE_CRT)
	return;

    output = xf86OutputCreate (scrn, &i830_crt_output_funcs, "VGA");
    if (!output)
	return;
    i830_output = xnfcalloc (sizeof (I830OutputPrivateRec), 1);
    if (!i830_output)
    {
	xf86OutputDestroy (output);
	return;
    }
    i830_output->type = I830_OUTPUT_ANALOG;
    /* i830 (almador) cannot place the analog adaptor on pipe B */
    if (IS_I830(intel))
	i830_output->pipe_mask = (1 << 0);
    else
	i830_output->pipe_mask = ((1 << 0) | (1 << 1));
    i830_output->clone_mask = ((1 << I830_OUTPUT_ANALOG) |
			       (1 << I830_OUTPUT_DVO_TMDS));
    
    output->driver_private = i830_output;
    output->interlaceAllowed = FALSE;
    output->doubleScanAllowed = FALSE;
}
