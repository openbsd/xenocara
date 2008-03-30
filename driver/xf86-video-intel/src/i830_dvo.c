/**************************************************************************

Copyright 2006 Dave Airlie <airlied@linux.ie>

All Rights Reserved.

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

******
********************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "i830.h"
#include "i830_display.h"
#include "i810_reg.h"

#include "sil164/sil164.h"
#include "ch7xxx/ch7xxx.h"
#include "tfp410/tfp410.h"

static const char *SIL164Symbols[] = {
    "Sil164VidOutput",
    NULL
};
static const char *TFP410Symbols[] = {
    "Tfp410VidOutput",
    NULL
};
static const char *CH7xxxSymbols[] = {
    "CH7xxxVidOutput",
    NULL
};
static const char *ivch_symbols[] = {
    "ivch_methods",
    NULL
};

static const char *ch7017_symbols[] = {
    "ch7017_methods",
    NULL
};

/* driver list */
struct _I830DVODriver i830_dvo_drivers[] =
{
    {
	.type = I830_OUTPUT_DVO_TMDS,
	.modulename = "sil164",
	.fntablename = "SIL164VidOutput",
	.dvo_reg = DVOC,
	.address = (SIL164_ADDR_1<<1),
	.symbols = SIL164Symbols
    },
    {
	.type = I830_OUTPUT_DVO_TMDS,
	.modulename = "ch7xxx",
	.fntablename = "CH7xxxVidOutput",
	.dvo_reg = DVOC,
	.address = (CH7xxx_ADDR_1<<1),
	.symbols = CH7xxxSymbols
    },
    {
	.type = I830_OUTPUT_DVO_LVDS,
	.modulename = "ivch",
	.fntablename = "ivch_methods",
	.dvo_reg = DVOA,
	.address = 0x04, /* Might also be 0x44, 0x84, 0xc4 */
	.symbols = ivch_symbols
    },
    {
	.type = I830_OUTPUT_DVO_TMDS,
	.modulename = "tfp410",
	.fntablename = "TFP410VidOutput",
	.dvo_reg = DVOC,
	.address = (TFP410_ADDR_1<<1),
	.symbols = TFP410Symbols
    },
    {
	.type = I830_OUTPUT_DVO_LVDS,
	.modulename = "ch7017",
	.fntablename = "ch7017_methods",
	.dvo_reg = DVOC,
	.address = 0xea,
	.symbols = ch7017_symbols,
	.gpio = GPIOE,
    }
};

#define I830_NUM_DVO_DRIVERS (sizeof(i830_dvo_drivers)/sizeof(struct _I830DVODriver))

static void
i830_dvo_dpms(xf86OutputPtr output, int mode)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct _I830DVODriver   *drv = intel_output->i2c_drv;
    void *		    dev_priv = drv->dev_priv;
    unsigned int	    dvo_reg = drv->dvo_reg;

    if (mode == DPMSModeOn) {
	OUTREG(dvo_reg, INREG(dvo_reg) | DVO_ENABLE);
	POSTING_READ(dvo_reg);
	(*intel_output->i2c_drv->vid_rec->dpms)(dev_priv, mode);
    } else {
	(*intel_output->i2c_drv->vid_rec->dpms)(dev_priv, mode);
	OUTREG(dvo_reg, INREG(dvo_reg) & ~DVO_ENABLE);
	POSTING_READ(dvo_reg);
    }
}

static void
i830_dvo_save(xf86OutputPtr output)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    void *		    dev_priv = intel_output->i2c_drv->dev_priv;

    /* Each output should probably just save the registers it touches, but for
     * now, use more overkill.
     */
    pI830->saveDVOA = INREG(DVOA);
    pI830->saveDVOB = INREG(DVOB);
    pI830->saveDVOC = INREG(DVOC);

    (*intel_output->i2c_drv->vid_rec->save)(dev_priv);
}

static void
i830_dvo_restore(xf86OutputPtr output)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr    intel_output = output->driver_private;
    void *		    dev_priv = intel_output->i2c_drv->dev_priv;

    (*intel_output->i2c_drv->vid_rec->restore)(dev_priv);

    OUTREG(DVOA, pI830->saveDVOA);
    OUTREG(DVOB, pI830->saveDVOB);
    OUTREG(DVOC, pI830->saveDVOC);
}

static int
i830_dvo_mode_valid(xf86OutputPtr output, DisplayModePtr pMode)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct _I830DVODriver   *drv = intel_output->i2c_drv;
    void		    *dev_priv = intel_output->i2c_drv->dev_priv;

    if (pMode->Flags & V_DBLSCAN)
	return MODE_NO_DBLESCAN;

    /* XXX: Validate clock range */

    if (drv->panel_fixed_mode) {
	if (pMode->HDisplay > drv->panel_fixed_mode->HDisplay)
	    return MODE_PANEL;
	if (pMode->VDisplay > drv->panel_fixed_mode->VDisplay)
	    return MODE_PANEL;
    }

    return intel_output->i2c_drv->vid_rec->mode_valid(dev_priv, pMode);
}

static Bool
i830_dvo_mode_fixup(xf86OutputPtr output, DisplayModePtr mode,
		    DisplayModePtr adjusted_mode)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct _I830DVODriver   *drv = intel_output->i2c_drv;

    /* If we have timings from the BIOS for the panel, put them in
     * to the adjusted mode.  The CRTC will be set up for this mode,
     * with the panel scaling set up to source from the H/VDisplay
     * of the original mode.
     */
    if (drv->panel_fixed_mode != NULL) {
	adjusted_mode->HDisplay = drv->panel_fixed_mode->HDisplay;
	adjusted_mode->HSyncStart = drv->panel_fixed_mode->HSyncStart;
	adjusted_mode->HSyncEnd = drv->panel_fixed_mode->HSyncEnd;
	adjusted_mode->HTotal = drv->panel_fixed_mode->HTotal;
	adjusted_mode->VDisplay = drv->panel_fixed_mode->VDisplay;
	adjusted_mode->VSyncStart = drv->panel_fixed_mode->VSyncStart;
	adjusted_mode->VSyncEnd = drv->panel_fixed_mode->VSyncEnd;
	adjusted_mode->VTotal = drv->panel_fixed_mode->VTotal;
	adjusted_mode->Clock = drv->panel_fixed_mode->Clock;
	xf86SetModeCrtc(adjusted_mode, INTERLACE_HALVE_V);
    }

    if (intel_output->i2c_drv->vid_rec->mode_fixup)
	return intel_output->i2c_drv->vid_rec->mode_fixup (intel_output->i2c_drv->dev_priv,
							   mode, adjusted_mode);
    return TRUE;
}

static void
i830_dvo_mode_set(xf86OutputPtr output, DisplayModePtr mode,
		  DisplayModePtr adjusted_mode)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830Ptr		    pI830 = I830PTR(pScrn);
    xf86CrtcPtr	    crtc = output->crtc;
    I830CrtcPrivatePtr	    intel_crtc = crtc->driver_private;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct _I830DVODriver   *drv = intel_output->i2c_drv;
    int			    pipe = intel_crtc->pipe;
    CARD32		    dvo;
    unsigned int	    dvo_reg = drv->dvo_reg, dvo_srcdim_reg;
    int			    dpll_reg = (pipe == 0) ? DPLL_A : DPLL_B;

    switch (dvo_reg) {
    case DVOA:
    default:
	dvo_srcdim_reg = DVOA_SRCDIM;
	break;
    case DVOB:
	dvo_srcdim_reg = DVOB_SRCDIM;
	break;
    case DVOC:
	dvo_srcdim_reg = DVOC_SRCDIM;
	break;
    }

    intel_output->i2c_drv->vid_rec->mode_set(intel_output->i2c_drv->dev_priv,
					     mode, adjusted_mode);

    /* Save the data order, since I don't know what it should be set to. */
    dvo = INREG(dvo_reg) & (DVO_PRESERVE_MASK | DVO_DATA_ORDER_GBRG);
    dvo |= DVO_DATA_ORDER_FP | DVO_BORDER_ENABLE | DVO_BLANK_ACTIVE_HIGH;

    if (pipe == 1)
	dvo |= DVO_PIPE_B_SELECT;
    dvo |= DVO_PIPE_STALL;
    if (adjusted_mode->Flags & V_PHSYNC)
	dvo |= DVO_HSYNC_ACTIVE_HIGH;
    if (adjusted_mode->Flags & V_PVSYNC)
	dvo |= DVO_VSYNC_ACTIVE_HIGH;

    OUTREG(dpll_reg, INREG(dpll_reg) | DPLL_DVO_HIGH_SPEED);

    /*OUTREG(DVOB_SRCDIM,
      (adjusted_mode->HDisplay << DVO_SRCDIM_HORIZONTAL_SHIFT) |
      (adjusted_mode->VDisplay << DVO_SRCDIM_VERTICAL_SHIFT));*/
    OUTREG(dvo_srcdim_reg,
	   (adjusted_mode->HDisplay << DVO_SRCDIM_HORIZONTAL_SHIFT) |
	   (adjusted_mode->VDisplay << DVO_SRCDIM_VERTICAL_SHIFT));
    /*OUTREG(DVOB, dvo);*/
    OUTREG(dvo_reg, dvo);
}

/**
 * Detect the output connection on our DVO device.
 *
 * Unimplemented.
 */
static xf86OutputStatus
i830_dvo_detect(xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    void *dev_priv = intel_output->i2c_drv->dev_priv;

    return intel_output->i2c_drv->vid_rec->detect(dev_priv);
}

static DisplayModePtr
i830_dvo_get_modes(xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;
    struct _I830DVODriver   *drv = intel_output->i2c_drv;
    DisplayModePtr	    modes;

    /* We should probably have an i2c driver get_modes function for those
     * devices which will have a fixed set of modes determined by the chip
     * (TV-out, for example), but for now with just TMDS and LVDS, that's not
     * the case.
     */
    modes = i830_ddc_get_modes(output);
    if (modes != NULL)
	return modes;

    if (intel_output->i2c_drv->vid_rec->get_modes)
    {
	modes = intel_output->i2c_drv->vid_rec->get_modes (intel_output->i2c_drv->dev_priv);
	if (modes != NULL)
	    return modes;
    }

    if (drv->panel_fixed_mode != NULL)
	return xf86DuplicateMode(drv->panel_fixed_mode);

    return NULL;
}

static void
i830_dvo_destroy (xf86OutputPtr output)
{
    I830OutputPrivatePtr    intel_output = output->driver_private;

    if (intel_output)
    {
	if (intel_output->i2c_drv->vid_rec->destroy)
	    intel_output->i2c_drv->vid_rec->destroy (intel_output->i2c_drv->dev_priv);
	if (intel_output->pI2CBus)
	    xf86DestroyI2CBusRec (intel_output->pI2CBus, TRUE, TRUE);
	if (intel_output->pDDCBus)
	    xf86DestroyI2CBusRec (intel_output->pDDCBus, TRUE, TRUE);
	xfree (intel_output);
    }
}

static const xf86OutputFuncsRec i830_dvo_output_funcs = {
    .dpms = i830_dvo_dpms,
    .save = i830_dvo_save,
    .restore = i830_dvo_restore,
    .mode_valid = i830_dvo_mode_valid,
    .mode_fixup = i830_dvo_mode_fixup,
    .prepare = i830_output_prepare,
    .mode_set = i830_dvo_mode_set,
    .commit = i830_output_commit,
    .detect = i830_dvo_detect,
    .get_modes = i830_dvo_get_modes,
    .destroy = i830_dvo_destroy
};

/**
 * Attempts to get a fixed panel timing for LVDS (currently only the i830).
 *
 * Other chips with DVO LVDS will need to extend this to deal with the LVDS
 * chip being on DVOB/C and having multiple pipes.
 */
DisplayModePtr
i830_dvo_get_current_mode (xf86OutputPtr output)
{
    ScrnInfoPtr		    pScrn = output->scrn;
    I830OutputPrivatePtr    intel_output = output->driver_private;
    I830Ptr		    pI830 = I830PTR(pScrn);
    struct _I830DVODriver   *drv = intel_output->i2c_drv;
    unsigned int	    dvo_reg = drv->dvo_reg;
    CARD32		    dvo = INREG(dvo_reg);
    DisplayModePtr    	    mode = NULL;

    /* If the DVO port is active, that'll be the LVDS, so we can pull out
     * its timings to get how the BIOS set up the panel.
     */
    if (dvo & DVO_ENABLE) 
    {
	xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
	int		    pipe = (dvo & DVO_PIPE_B_SELECT) ? 1 : 0;
	int		    c;

	for (c = 0; c < xf86_config->num_crtc; c++)
	{
	    xf86CrtcPtr		crtc = xf86_config->crtc[c];
	    I830CrtcPrivatePtr	intel_crtc = crtc->driver_private;

	    if (intel_crtc->pipe == pipe)
	    {
		mode = i830_crtc_mode_get(pScrn, crtc);

		if (mode)
		{
		    mode->type |= M_T_PREFERRED;

		    if (dvo & DVO_HSYNC_ACTIVE_HIGH)
			mode->Flags |= V_PHSYNC;
		    if (dvo & DVO_VSYNC_ACTIVE_HIGH)
			mode->Flags |= V_PVSYNC;
		}
		break;
	    }
	}
    }
    return mode;
}

void
i830_dvo_init(ScrnInfoPtr pScrn)
{
    I830Ptr pI830 = I830PTR(pScrn);
    I830OutputPrivatePtr intel_output;
    int ret;
    int i;
    void *ret_ptr;
    struct _I830DVODriver *drv;
    int gpio_inited = 0;
    I2CBusPtr pI2CBus = NULL;

    intel_output = xnfcalloc (sizeof (I830OutputPrivateRec), 1);
    if (!intel_output)
	return;

    /* Set up the DDC bus */
    ret = I830I2CInit(pScrn, &intel_output->pDDCBus, GPIOD, "DVODDC_D");
    if (!ret) {
	xfree(intel_output);
	return;
    }

    /* Now, try to find a controller */
    for (i = 0; i < I830_NUM_DVO_DRIVERS; i++) {
	int gpio;

	drv = &i830_dvo_drivers[i];
	drv->modhandle = xf86LoadSubModule(pScrn, drv->modulename);
	if (drv->modhandle == NULL)
	    continue;

	xf86LoaderReqSymLists(drv->symbols, NULL);

	ret_ptr = NULL;
	drv->vid_rec = LoaderSymbol(drv->fntablename);

	if (!strcmp(drv->modulename, "ivch") &&
	    pI830->quirk_flag & QUIRK_IVCH_NEED_DVOB) {
	    drv->dvo_reg = DVOB;
	}

	/* Allow the I2C driver info to specify the GPIO to be used in
	 * special cases, but otherwise default to what's defined in the spec.
	 */
	if (drv->gpio != 0)
	    gpio = drv->gpio;
	else if (drv->type == I830_OUTPUT_DVO_LVDS)
	    gpio = GPIOB;
	else
	    gpio = GPIOE;

	/* Set up the I2C bus necessary for the chip we're probing.  It appears
	 * that everything is on GPIOE except for panels on i830 laptops, which
	 * are on GPIOB (DVOA).
	 */
	if (gpio_inited != gpio) {
	    if (pI2CBus != NULL)
		xf86DestroyI2CBusRec(pI2CBus, TRUE, TRUE);
	    if (!I830I2CInit(pScrn, &pI2CBus, gpio,
			     gpio == GPIOB ? "DVOI2C_B" : "DVOI2C_E")) {
		continue;
	    }
	}

	if (drv->vid_rec != NULL)
	    ret_ptr = drv->vid_rec->init(pI2CBus, drv->address);

	if (ret_ptr != NULL) {
	    xf86OutputPtr output = NULL;

	    intel_output->type = drv->type;
	    switch (drv->type) {
	    case I830_OUTPUT_DVO_TMDS:
		intel_output->pipe_mask = ((1 << 0) | (1 << 1));
		intel_output->clone_mask = ((1 << I830_OUTPUT_ANALOG) |
					    (1 << I830_OUTPUT_DVO_TMDS));
		output = xf86OutputCreate(pScrn, &i830_dvo_output_funcs,
					  "TMDS");
		break;
	    case I830_OUTPUT_DVO_LVDS:
		intel_output->pipe_mask = ((1 << 0) | (1 << 1));
		intel_output->clone_mask = (1 << I830_OUTPUT_DVO_LVDS);
		output = xf86OutputCreate(pScrn, &i830_dvo_output_funcs,
					  "LVDS");
		break;
	    case I830_OUTPUT_DVO_TVOUT:
		intel_output->pipe_mask = ((1 << 0) | (1 << 1));
		intel_output->clone_mask = (1 << I830_OUTPUT_DVO_TVOUT);
		output = xf86OutputCreate(pScrn, &i830_dvo_output_funcs,
					  "TV");
		break;
	    }
	    if (output == NULL) {
		xf86DestroyI2CBusRec(pI2CBus, TRUE, TRUE);
		xf86DestroyI2CBusRec(intel_output->pDDCBus, TRUE, TRUE);
		xfree(intel_output);
		xf86UnloadSubModule(drv->modhandle);
		return;
	    }

	    output->driver_private = intel_output;
	    output->subpixel_order = SubPixelHorizontalRGB;
	    output->interlaceAllowed = FALSE;
	    output->doubleScanAllowed = FALSE;

	    drv->dev_priv = ret_ptr;
	    intel_output->i2c_drv = drv;
	    intel_output->pI2CBus = pI2CBus;

	    if (intel_output->type == I830_OUTPUT_DVO_LVDS) {
		/* For our LVDS chipsets, we should hopefully be able to
		 * dig the fixed panel mode out of the BIOS data.  However,
		 * it's in a different format from the BIOS data on chipsets
		 * with integrated LVDS (stored in AIM headers, liekly),
		 * so for now, just get the current mode being output through
		 * DVO.
		 */
		drv->panel_fixed_mode = i830_dvo_get_current_mode(output);
		drv->panel_wants_dither = TRUE;
	    }

	    return;
	}
	xf86UnloadSubModule(drv->modhandle);
    }

    /* Didn't find a chip, so tear down. */
    if (pI2CBus != NULL)
	xf86DestroyI2CBusRec(pI2CBus, TRUE, TRUE);
    xf86DestroyI2CBusRec(intel_output->pDDCBus, TRUE, TRUE);
    xfree(intel_output);
}
