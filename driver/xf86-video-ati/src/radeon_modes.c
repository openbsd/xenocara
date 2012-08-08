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

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *   Alan Hourihane <alanh@fairlite.demon.co.uk>
 */

#include <string.h>
#include <stdio.h>

#include "xf86.h"
				/* Driver data structures */
#include "randrstr.h"
#include "radeon_probe.h"
#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_version.h"
#include "radeon_atombios.h"

#include "xf86Modes.h"
				/* DDC support */
#include "xf86DDC.h"
#include <randrstr.h>

void RADEONSetPitch (ScrnInfoPtr pScrn)
{
    int  dummy = pScrn->virtualX;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int pitch_mask = 0;
    int align_large;

    align_large = info->allowColorTiling || IS_AVIVO_VARIANT;

    /* FIXME: May need to validate line pitch here */
    if (info->ChipFamily < CHIP_FAMILY_R600) {
	switch (pScrn->depth / 8) {
	case 1: pitch_mask = align_large ? 256 : 128;
	    break;
	case 2: pitch_mask = align_large ? 128 : 32;
	    break;
	case 3:
	case 4: pitch_mask = align_large ? 64 : 16;
	    break;
	}
    } else
	pitch_mask = 256; /* r6xx/r7xx need 256B alignment for accel */

    dummy = RADEON_ALIGN(pScrn->virtualX, pitch_mask);
    pScrn->displayWidth = dummy;
    info->CurrentLayout.displayWidth = pScrn->displayWidth;

}

static DisplayModePtr
RADEONTVModes(xf86OutputPtr output)
{
    DisplayModePtr new  = NULL;

    /* just a place holder */
    new = xf86CVTMode(800, 600, 60.00, FALSE, FALSE);
    new->type = M_T_DRIVER | M_T_PREFERRED;

    return new;
}

static DisplayModePtr
RADEONATOMTVModes(xf86OutputPtr output)
{
    DisplayModePtr  last       = NULL;
    DisplayModePtr  new        = NULL;
    DisplayModePtr  first      = NULL;
    int i;
    /* Add some common sizes */
    int widths[5] =  {640, 720, 800, 848, 1024};
    int heights[5] = {480, 480, 600, 480,  768};

    for (i = 0; i < 5; i++) {
	new = xf86CVTMode(widths[i], heights[i], 60.0, FALSE, FALSE);

	new->type       = M_T_DRIVER;

	new->next       = NULL;
	new->prev       = last;

	if (last) last->next = new;
	last = new;
	if (!first) first = new;
    }

    if (last) {
	last->next   = NULL; //first;
	first->prev  = NULL; //last;
    }

    return first;
}

/* This is used only when no mode is specified for FP and no ddc is
 * available.  We force it to native mode, if possible.
 */
static DisplayModePtr RADEONFPNativeMode(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_native_mode_ptr native_mode = &radeon_output->native_mode;
    DisplayModePtr  new   = NULL;
    char            stmp[32];

    if (native_mode->PanelXRes != 0 &&
	native_mode->PanelYRes != 0 &&
	native_mode->DotClock != 0) {

	new             = xnfcalloc(1, sizeof (DisplayModeRec));
	sprintf(stmp, "%dx%d", native_mode->PanelXRes, native_mode->PanelYRes);
	new->name       = xnfalloc(strlen(stmp) + 1);
	strcpy(new->name, stmp);
	new->HDisplay   = native_mode->PanelXRes;
	new->VDisplay   = native_mode->PanelYRes;

	new->HTotal     = new->HDisplay + native_mode->HBlank;
	new->HSyncStart = new->HDisplay + native_mode->HOverPlus;
	new->HSyncEnd   = new->HSyncStart + native_mode->HSyncWidth;
	new->VTotal     = new->VDisplay + native_mode->VBlank;
	new->VSyncStart = new->VDisplay + native_mode->VOverPlus;
	new->VSyncEnd   = new->VSyncStart + native_mode->VSyncWidth;

	new->Clock      = native_mode->DotClock;
	new->Flags      = native_mode->Flags;

	if (new) {
	    new->type       = M_T_DRIVER | M_T_PREFERRED;

	    new->next       = NULL;
	    new->prev       = NULL;
	}

	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Added native panel mode: %dx%d\n",
		   native_mode->PanelXRes, native_mode->PanelYRes);
    } else if (native_mode->PanelXRes != 0 &&
	       native_mode->PanelYRes != 0) {

	new = xf86CVTMode(native_mode->PanelXRes, native_mode->PanelYRes, 60.0, TRUE, FALSE);

	if (new) {
	    new->type       = M_T_DRIVER | M_T_PREFERRED;

	    new->next       = NULL;
	    new->prev       = NULL;
	}

	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Added native panel mode using CVT: %dx%d\n",
		   native_mode->PanelXRes, native_mode->PanelYRes);
    }

    return new;
}

#if defined(__powerpc__)
/* Apple eMacs need special modes for the internal CRT, e.g.,
 * Modeline "640x480"    62.12   640  680  752  864  480 481 484  521 +HSync +Vsync
 * Modeline "800x600"    76.84   800  848  936 1072  600 601 604  640 +HSync +Vsync
 * Modeline "1024x768"   99.07  1024 1088 1200 1376  768 769 772  809 +HSync +Vsync
 * Modeline "1152x864"  112.36  1152 1224 1352 1552  864 865 868  905 +HSync +Vsync
 * Modeline "1280x960"  124.54  1280 1368 1504 1728  960 961 964 1001 +HSync +Vsync
 */
static DisplayModePtr RADEONeMacModes(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    DisplayModePtr last=NULL, new=NULL, first=NULL;
    int i, *modep;
    static const char *modenames[5] = {
	"640x480", "800x600", "1024x768", "1152x864", "1280x960"
    };
    static int modes[9*5] = {
	 62120,  640,  680,  752,  864, 480, 481, 484,  521,
	 76840,  800,  848,  936, 1072, 600, 601, 604,  640,
	 99070, 1024, 1088, 1200, 1376, 768, 769, 772,  809,
	112360, 1152, 1224, 1352, 1552, 864, 865, 868,  905,
	124540, 1280, 1368, 1504, 1728, 960, 961, 964, 1001
    };
    modep = modes;

    for (i=0; i<5; i++) {
	new = xnfcalloc(1, sizeof (DisplayModeRec));
	if (new) {
	    new->name       = xnfalloc(strlen(modenames[i]) + 1);
	    strcpy(new->name, modenames[i]);
	    new->Clock      = *modep++;

	    new->HDisplay   = *modep++;
	    new->HSyncStart = *modep++;
	    new->HSyncEnd   = *modep++;
	    new->HTotal     = *modep++;

	    new->VDisplay   = *modep++;
	    new->VSyncStart = *modep++;
	    new->VSyncEnd   = *modep++;
	    new->VTotal     = *modep++;

	    new->Flags      = 0;
	    new->type       = M_T_DRIVER;
	    if (i==2)
		new->type |= M_T_PREFERRED;
	    new->next       = NULL;
	    new->prev       = last;
	    if (last) last->next = new;
	    last = new;
	    if (!first) first = new;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Added eMac mode %s\n", modenames[i]);
	}
    }

    return first;
}
#endif

/* this function is basically a hack to add the screen modes */
static void RADEONAddScreenModes(xf86OutputPtr output, DisplayModePtr *modeList)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_native_mode_ptr native_mode = &radeon_output->native_mode;
    DisplayModePtr  last       = NULL;
    DisplayModePtr  new        = NULL;
    DisplayModePtr  first      = NULL;
    int             count      = 0;
    int             i, width, height;
    char **ppModeName = pScrn->display->modes;

    first = last = *modeList;

    /* We have a flat panel connected to the primary display, and we
     * don't have any DDC info.
     */
    for (i = 0; ppModeName[i] != NULL; i++) {

	if (sscanf(ppModeName[i], "%dx%d", &width, &height) != 2) continue;

	if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT)) {
	    /* already added the native mode */
	    if (width == native_mode->PanelXRes && height == native_mode->PanelYRes)
		continue;

	    /* Note: We allow all non-standard modes as long as they do not
	     * exceed the native resolution of the panel.  Since these modes
	     * need the internal RMX unit in the video chips (and there is
	     * only one per card), this will only apply to the primary head.
	     */
	    if (width < 320 || width > native_mode->PanelXRes ||
		height < 200 || height > native_mode->PanelYRes) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "Mode %s is out of range.\n", ppModeName[i]);
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "Valid FP modes must be between 320x200-%dx%d\n",
			   native_mode->PanelXRes, native_mode->PanelYRes);
		continue;
	    }
	}

	new = xf86CVTMode(width, height, 60.0, FALSE, FALSE);

	new->type      |= M_T_USERDEF;

	new->next       = NULL;
	new->prev       = last;

	if (last) last->next = new;
	last = new;
	if (!first) first = new;

	count++;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Adding Screen mode: %s\n", new->name);
    }


    /* Close the doubly-linked mode list, if we found any usable modes */
    if (last) {
	last->next   = NULL; //first;
	first->prev  = NULL; //last;
	*modeList = first;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Total number of valid Screen mode(s) added: %d\n", count);

}

/* BIOS may not have right panel size, we search through all supported
 * DDC modes looking for the maximum panel size.
 */
static void
RADEONUpdatePanelSize(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_native_mode_ptr native_mode = &radeon_output->native_mode;
    int             j;
    xf86MonPtr ddc = output->MonInfo;
    DisplayModePtr  p;

    // update output's native mode
    if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT)) {
	radeon_encoder_ptr radeon_encoder = radeon_get_encoder(output);
	if (radeon_encoder) {
	    radeon_lvds_ptr lvds = (radeon_lvds_ptr)radeon_encoder->dev_priv;
	    if (lvds)
		radeon_output->native_mode = lvds->native_mode;
	}
    }

    // crtc should handle?
    if ((info->UseBiosDividers && native_mode->DotClock != 0) || (ddc == NULL))
       return;

    /* Go thru detailed timing table first */
    for (j = 0; j < 4; j++) {
	if (ddc->det_mon[j].type == 0) {
	    struct detailed_timings *d_timings =
		&ddc->det_mon[j].section.d_timings;
           int match = 0;

           /* If we didn't get a panel clock or guessed one, try to match the
            * mode with the panel size. We do that because we _need_ a panel
            * clock, or ValidateFPModes will fail, even when UseBiosDividers
            * is set.
            */
           if (native_mode->DotClock == 0 &&
               native_mode->PanelXRes == d_timings->h_active &&
               native_mode->PanelYRes == d_timings->v_active)
               match = 1;

           /* If we don't have a BIOS provided panel data with fixed dividers,
            * check for a larger panel size
            */
	    if (native_mode->PanelXRes < d_timings->h_active &&
		native_mode->PanelYRes < d_timings->v_active &&
		!info->UseBiosDividers)
		match = 1;

             if (match) {
		native_mode->PanelXRes  = d_timings->h_active;
		native_mode->PanelYRes  = d_timings->v_active;
		native_mode->DotClock   = d_timings->clock / 1000;
		native_mode->HOverPlus  = d_timings->h_sync_off;
		native_mode->HSyncWidth = d_timings->h_sync_width;
		native_mode->HBlank     = d_timings->h_blanking;
		native_mode->VOverPlus  = d_timings->v_sync_off;
		native_mode->VSyncWidth = d_timings->v_sync_width;
		native_mode->VBlank     = d_timings->v_blanking;
                native_mode->Flags      = (d_timings->interlaced ? V_INTERLACE : 0);
                switch (d_timings->misc) {
                case 0: native_mode->Flags |= V_NHSYNC | V_NVSYNC; break;
                case 1: native_mode->Flags |= V_PHSYNC | V_NVSYNC; break;
                case 2: native_mode->Flags |= V_NHSYNC | V_PVSYNC; break;
                case 3: native_mode->Flags |= V_PHSYNC | V_PVSYNC; break;
                }
                xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel infos found from DDC detailed: %dx%d\n",
                           native_mode->PanelXRes, native_mode->PanelYRes);
	    }
	}
    }

    if (info->UseBiosDividers && native_mode->DotClock != 0)
       return;

    /* Search thru standard VESA modes from EDID */
    for (j = 0; j < 8; j++) {
	if ((native_mode->PanelXRes < ddc->timings2[j].hsize) &&
	    (native_mode->PanelYRes < ddc->timings2[j].vsize)) {
	    for (p = pScrn->monitor->Modes; p; p = p->next) {
		if ((ddc->timings2[j].hsize == p->HDisplay) &&
		    (ddc->timings2[j].vsize == p->VDisplay)) {
		    float  refresh =
			(float)p->Clock * 1000.0 / p->HTotal / p->VTotal;

		    if (abs((float)ddc->timings2[j].refresh - refresh) < 1.0) {
			/* Is this good enough? */
			native_mode->PanelXRes  = ddc->timings2[j].hsize;
			native_mode->PanelYRes  = ddc->timings2[j].vsize;
			native_mode->HBlank     = p->HTotal - p->HDisplay;
			native_mode->HOverPlus  = p->HSyncStart - p->HDisplay;
			native_mode->HSyncWidth = p->HSyncEnd - p->HSyncStart;
			native_mode->VBlank     = p->VTotal - p->VDisplay;
			native_mode->VOverPlus  = p->VSyncStart - p->VDisplay;
			native_mode->VSyncWidth = p->VSyncEnd - p->VSyncStart;
			native_mode->DotClock   = p->Clock;
                        native_mode->Flags      = p->Flags;
                        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel infos found from DDC VESA/EDID: %dx%d\n",
                                   native_mode->PanelXRes, native_mode->PanelYRes);
		    }
		}
	    }
	}
    }
}

static void
radeon_add_common_modes(xf86OutputPtr output, DisplayModePtr modes)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    radeon_native_mode_ptr native_mode = &radeon_output->native_mode;
    DisplayModePtr  last       = NULL;
    DisplayModePtr  new        = NULL;
    DisplayModePtr  first      = NULL;
    int i;
    /* Add some common sizes */
    int widths[15]  = {640, 800, 1024, 1152, 1280, 1280, 1280, 1280, 1280, 1440, 1400, 1680, 1600, 1920, 1920};
    int heights[15] = {480, 600,  768,  768,  720,  800,  854,  960, 1024,  900, 1050, 1050, 1200, 1080, 1200};

    for (i = 0; i < 15; i++) {
	if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT)) {
	    /* already added the native mode */
	    if (widths[i] == native_mode->PanelXRes && heights[i] == native_mode->PanelYRes)
		continue;

	    /* Note: We allow all non-standard modes as long as they do not
	     * exceed the native resolution of the panel.  Since these modes
	     * need the internal RMX unit in the video chips (and there is
	     * only one per card), this will only apply to the primary head.
	     */
	    if (widths[i] < 320 || widths[i] > native_mode->PanelXRes ||
		heights[i] < 200 || heights[i] > native_mode->PanelYRes)
		continue;
	}

	new = xf86CVTMode(widths[i], heights[i], 60.0, FALSE, FALSE);

	new->type       = M_T_DRIVER;

	new->next       = NULL;
	new->prev       = last;

	if (last) last->next = new;
	last = new;
	if (!first) first = new;
    }

    if (last) {
	last->next   = NULL; //first;
	first->prev  = NULL; //last;
    }

    xf86ModesAdd(modes, first);

}

DisplayModePtr
RADEONProbeOutputModes(xf86OutputPtr output)
{
    RADEONOutputPrivatePtr radeon_output = output->driver_private;
    ScrnInfoPtr pScrn = output->scrn;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    DisplayModePtr	    modes = NULL;
    AtomBiosArgRec atomBiosArg;
    AtomBiosResult atomBiosResult;

    if (output->status == XF86OutputStatusConnected) {
	if (radeon_output->active_device & (ATOM_DEVICE_TV_SUPPORT)) {
	    if (IS_AVIVO_VARIANT)
		modes = RADEONATOMTVModes(output);
	    else
		modes = RADEONTVModes(output);
	} else if (radeon_output->active_device & (ATOM_DEVICE_CV_SUPPORT)) {
	    atomBiosResult = RHDAtomBiosFunc(pScrn, info->atomBIOS,
					     ATOMBIOS_GET_CV_MODES, &atomBiosArg);
	    if (atomBiosResult == ATOM_SUCCESS) {
		modes = atomBiosArg.modes;
	    }
	} else {
	    if (radeon_output->active_device & (ATOM_DEVICE_DFP_SUPPORT | ATOM_DEVICE_LCD_SUPPORT))
		RADEONUpdatePanelSize(output);
	    if (output->MonInfo)
		modes = xf86OutputGetEDIDModes (output);
#if defined(__powerpc__)
	    if ((info->MacModel == RADEON_MAC_EMAC) &&
		(radeon_output->active_device & ATOM_DEVICE_CRT1_SUPPORT) &&
		(modes == NULL))
		modes = RADEONeMacModes(output);
#endif
	    if (modes == NULL) {
		if ((radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT)) && info->IsAtomBios) {
		    atomBiosResult = RHDAtomBiosFunc(pScrn,
						     info->atomBIOS,
						     ATOMBIOS_GET_PANEL_EDID, &atomBiosArg);
		    if (atomBiosResult == ATOM_SUCCESS) {
			output->MonInfo = xf86InterpretEDID(pScrn->scrnIndex,
							    atomBiosArg.EDIDBlock);
			modes = xf86OutputGetEDIDModes(output);
		    }
		}
		if (modes == NULL) {
		    if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT))
			modes = RADEONFPNativeMode(output);
		    /* add the screen modes */
		    if (modes == NULL)
			RADEONAddScreenModes(output, &modes);
		}
	    }
	}
    }

    if (radeon_output->active_device & (ATOM_DEVICE_LCD_SUPPORT))
	radeon_add_common_modes(output, modes);

    return modes;
}

