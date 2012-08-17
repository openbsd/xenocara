/*
 * Copyright 1997 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of Marc Aurele La France not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Marc Aurele La France makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as-is" without express or implied warranty.
 *
 * MARC AURELE LA FRANCE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO
 * EVENT SHALL MARC AURELE LA FRANCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * DRI support by:
 *    Manuel Teira
 *    Leif Delgass <ldelgass@retinalburn.net>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xorgVersion.h"
#include "ati.h"
#include "atii2c.h"
#include "atilock.h"
#include "atimach64.h"
#include "atimach64io.h"
#include "atimode.h"
#include "atistruct.h"
#include "ativga.h"
#include "atividmem.h"
#include "aticonsole.h"

#ifdef XF86DRI_DEVEL
#include "mach64_common.h"
#include "atidri.h"
#endif

#ifdef TV_OUT

#include "atichip.h"
#include "atiprint.h"
#include "atioption.h"
#include "vbe.h"

#endif /* TV_OUT */

/*
 * ATISaveScreen --
 *
 * This function is a screen saver hook for DIX.
 */
Bool
ATISaveScreen
(
    ScreenPtr pScreen,
    int       Mode
)
{
    ScrnInfoPtr pScreenInfo;
    ATIPtr      pATI;

    if ((Mode != SCREEN_SAVER_ON) && (Mode != SCREEN_SAVER_CYCLE))
        SetTimeSinceLastInputEvent();

    if (!pScreen)
        return TRUE;

    pScreenInfo = xf86ScreenToScrn(pScreen);
    if (!pScreenInfo->vtSema)
        return TRUE;

    pATI = ATIPTR(pScreenInfo);
    {
            ATIMach64SaveScreen(pATI, Mode);
    }

    return TRUE;
}

/*
 * ATISetDPMSMode --
 *
 * This function sets the adapter's VESA Display Power Management Signaling
 * mode.
 */
void
ATISetDPMSMode
(
    ScrnInfoPtr pScreenInfo,
    int         DPMSMode,
    int         flags
)
{
    ATIPtr pATI;

    if (!pScreenInfo || !pScreenInfo->vtSema)
        return;

    pATI = ATIPTR(pScreenInfo);

    {
            ATIMach64SetDPMSMode(pScreenInfo, pATI, DPMSMode);
    }
}

#ifdef TV_OUT

static void
ATIProbeAndSetActiveDisplays
(
    ScrnInfoPtr pScreenInfo,
    ATIPtr      pATI
)
{
    vbeInfoPtr pVbe;
    Bool tv_attached, crt_attached, lcd_attached;
    int disp_request;
    ATITVStandard tv_std, tv_std_request;

    if (xf86GetVerbosity() > 3) {
	xf86ErrorFVerb(4, "\n Before TV-Out queries\n\n");
	ATIPrintRegisters(pATI);
    }

    pATI->tvActive = FALSE;
    pVbe = pATI->pVBE;
    if (pVbe) {
	/* LT Pro, XL, Mobility specific BIOS functions */
	if (pATI->Chip == ATI_CHIP_264LTPRO ||
	    pATI->Chip == ATI_CHIP_264XL || 
	    pATI->Chip == ATI_CHIP_MOBILITY) {
    
	    /* Get attached display(s) - LTPro, XL, Mobility */
	    pVbe->pInt10->num = 0x10;
	    pVbe->pInt10->ax = 0xa083;
	    pVbe->pInt10->cx = 0x0700; /* ch=0x07 - probe all, 0x01 CRT, 0x02 TV, 0x04 LCD */
	    xf86ExecX86int10(pVbe->pInt10);

	    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
		       "Attached displays: ax=0x%04x, cx=0x%04x\n",
		       pVbe->pInt10->ax, pVbe->pInt10->cx);

	    tv_attached = crt_attached = lcd_attached = FALSE;
	    if (pVbe->pInt10->ax & 0xff00) {
		xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
			   "Failed to detect attached displays\n");
	    } else {
			
		if (pVbe->pInt10->cx & 0x3)
		{
			xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
				   "   CRT attached\n");
			crt_attached = TRUE;
		}
		else
		    crt_attached = FALSE;

		if ((pVbe->pInt10->cx >> 2) & 0x3)
		{
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "   DFP/LCD attached\n");
		    lcd_attached = TRUE;
		}
		else
		    lcd_attached = FALSE;

		switch ((pVbe->pInt10->cx >> 4) & 0x3) {
		case 0:
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "   No TV attached\n");
		    break;
		case 1:
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "   TV attached (composite connector)\n");
		    tv_attached = TRUE;
		    break;
		case 2:
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "   TV attached (S-video connector)\n");
		    tv_attached = TRUE;
		    break;
		case 3:
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "   TV attached (S-video/composite connectors)\n");
		    tv_attached = TRUE;
		    break;
		default:
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
			       "Unrecognized return code: 0x%04x\n", 
			       pVbe->pInt10->cx);
		}

	    }

	    /* Get active display  - LTPro, XL, Mobility */
	    pVbe->pInt10->num = 0x10;
	    pVbe->pInt10->ax = 0xa084;
	    pVbe->pInt10->bx = 0x0000; /* bh=0x00 get active, bh=0x01 set active */
	    xf86ExecX86int10(pVbe->pInt10);

	    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
		       "Active displays: ax=0x%04x, bx=0x%04x, cx=0x%04x\n",
		       pVbe->pInt10->ax, pVbe->pInt10->bx, pVbe->pInt10->cx);

	    if (pVbe->pInt10->ax & 0xff00) {
		xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
			   "Failed to detect active display\n");
	    } else {
		if (pVbe->pInt10->bx & 0x1) 
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 		       
			       "   DFP/LCD is active\n");

		if (pVbe->pInt10->bx & 0x2) 
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "   CRT is active\n");

		if (pVbe->pInt10->bx & 0x4) {
		    
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "   TV is active\n");

		    if (!tv_attached) {
			/* tv not connected - disable tv */
			disp_request = 0x00;
			if (crt_attached)
			    disp_request |= 0x02; /* enable CRT */
			if (lcd_attached && pATI->OptionPanelDisplay)
			    disp_request |= 0x01; /* enable DFP/LCD */

			pVbe->pInt10->num = 0x10;
			pVbe->pInt10->ax = 0xa084;
			pVbe->pInt10->bx = 0x0100; /* bh=0x01 set active */
			pVbe->pInt10->cx = disp_request; 
			xf86ExecX86int10(pVbe->pInt10);

			xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
				   "TV not present, disabling: ax=0x%04x, bx=0x%04x, cx=0x%04x\n",
				   pVbe->pInt10->ax, pVbe->pInt10->bx, pVbe->pInt10->cx);
			if (pVbe->pInt10->ax & 0xff00) {
			    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
				       "Disabling TV failed\n");
			}
		    } else {
			pATI->tvActive = TRUE;
		    }

		} else if (tv_attached && (pVbe->pInt10->bx & 0x0400)) {
		    /* tv connected and available - enable TV */
		    disp_request = 0x04;          /* enable TV */

#if 0
		    /* This works, but CRT image is vertically compressed */
		    if (crt_attached)
			    disp_request |= 0x02; /* enable CRT */
		    /* NOTE: For me, LCD+TV does NOT work */
		    /*if (lcd_attached && pATI->OptionPanelDisplay)
			    disp_request |= 0x01; * enable DFP/LCD */
#endif

		    pVbe->pInt10->num = 0x10;
		    pVbe->pInt10->ax = 0xa084;
		    pVbe->pInt10->bx = 0x0100; /* bh=0x01 set active */
		    pVbe->pInt10->cx = disp_request; /* try to activate TV */
		    xf86ExecX86int10(pVbe->pInt10);
		    
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "Setting TV active: ax=0x%04x, bx=0x%04x, cx=0x%04x\n",
			       pVbe->pInt10->ax, pVbe->pInt10->bx, pVbe->pInt10->cx);
		    if (pVbe->pInt10->ax & 0xff00) {
			xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
				   "Setting TV active failed\n");
		    } else {
			pATI->tvActive = TRUE;
		    }
		}
	    }

	} else { /* pATI->Chip < ATI_CHIP_264LTPRO */
	    /* TVOut Hooks - Check for TVOut BIOS/hardware */
	    pVbe->pInt10->num = 0x10;
	    pVbe->pInt10->ax = 0xa019;
	    pVbe->pInt10->cx = 0x0000; /* TVOut BIOS query */
	    xf86ExecX86int10(pVbe->pInt10);

	    tv_attached = FALSE;

	    if (pVbe->pInt10->ax & 0xff00) {
		xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
			   "Failed to detect TV-Out BIOS\n");
	    } else {
		switch (pVbe->pInt10->ax & 0x0003) {
		case 3:
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "TV-Out BIOS detected and active\n");

		    /* TV attached query */
		    pVbe->pInt10->num = 0x10;
		    pVbe->pInt10->ax = 0xa070;
		    pVbe->pInt10->bx = 0x0002; /* Sub-function: return tv attached info */
		    xf86ExecX86int10(pVbe->pInt10);

		    if (pVbe->pInt10->ax & 0xff00) {
			xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
				   "Failed to detect if TV is attached\n");
		    } else {
			switch (pVbe->pInt10->cx & 0x0003) {
			case 3:
			    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
				       "TV attached to composite and S-video connectors\n");
			    tv_attached = TRUE;
			    break;
			case 2:
			    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
				       "TV attached to S-video connector\n");
			    tv_attached = TRUE;
			    break;
			case 1:
			    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
				       "TV attached to composite connector\n");
			    tv_attached = TRUE;
			    break;
			default:
			    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
				       "TV is not attached\n");
			}
		    }
		    break;
		case 1:
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "TV-Out BIOS service is not available due to" 
			       "a system BIOS error or TV-Out hardware not being installed\n");
		    break;
		default:
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "No TV-Out BIOS or hardware detected\n");
		}
	    }
	}

	/* Return TV-Out configuration 
	 * see Programmer's Guide under "TV Out Specific Functions"
	 * It's not clear exactly which adapters support these
	 */
	pVbe->pInt10->num = 0x10;
	pVbe->pInt10->ax = 0xa070;
	pVbe->pInt10->bx = 0x00;
	xf86ExecX86int10(pVbe->pInt10);

	xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
		   "TV-Out query: ax=0x%04x, bx=0x%04x, cx=0x%04x, dx=0x%04x\n",
		   pVbe->pInt10->ax, pVbe->pInt10->bx, pVbe->pInt10->cx, pVbe->pInt10->dx);

	if (pVbe->pInt10->ax & 0xff00) {

	    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
		       "Failed to detect TV-Out configuration.\n");

	} else if (pVbe->pInt10->bx == 0) {
	    if (pVbe->pInt10->dx == 0) {
		xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			   "TV-Out is not detected.\n");
	    } else {
		xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			   "TV-Out is detected but not supported.\n");
	    }

	} else if ((pVbe->pInt10->cx & 0xff) == 0) {

	    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
		       "TV-Out is currently disabled.\n");
	    if (tv_attached && pATI->Chip < ATI_CHIP_264LTPRO) {
		/* Try to enable TV-Out */
		pVbe->pInt10->num = 0x10;
		pVbe->pInt10->ax = 0xa070;
		pVbe->pInt10->bx = 0x0001; /* Sub-function: Select TV Out */
		/* cl=0x001 enable, cl=0x000 disable, 
		 * cl=0x080 disable with feature connector bit preserved 
		 */
		pVbe->pInt10->cx = 0x0001;
			
		xf86ExecX86int10(pVbe->pInt10);
		xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			   "Setting TV active: ax=0x%04x, bx=0x%04x, cx=0x%04x\n",
			   pVbe->pInt10->ax, pVbe->pInt10->bx, pVbe->pInt10->cx);

		if (pVbe->pInt10->ax & 0xff00) {
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
			       "Setting TV active failed\n");
		} else {
		    pATI->tvActive = TRUE;
		}
	    }

	} else {
	    pATI->tvActive = TRUE;

	    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
		       "TV-Out is currently enabled (TV-Out revision code: %d).\n",
		       (pVbe->pInt10->dx >> 8) & 0xff);

	    switch ((pVbe->pInt10->cx >> 8) & 0xff) {
	    case 0:
		xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, "Reference frequency 29.49892\n");
		break;
	    case 1:
		xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, "Reference frequency 28.63636\n");
		break;
	    case 2:
		xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, "Reference frequency 14.31818\n");
		break;
	    case 3:
		xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, "Reference frequency 27.00000\n");
		break;
	    default:
		xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
			   "Unknown reference frequency cx=0x%04x\n", pVbe->pInt10->cx);
		    
	    }

	    /* Return TV standard
	     * see Programmer's Guide under "TV Out Specific Functions"
	     * It's not clear exactly which adapters support these
	     */
	    pVbe->pInt10->num = 0x10;
	    pVbe->pInt10->ax = 0xa071;
	    pVbe->pInt10->bx = 0x00;
	    xf86ExecX86int10(pVbe->pInt10);

	    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
		       "TV standard query result: ax=0x%04x, bx=0x%04x, cx=0x%04x\n",
		       pVbe->pInt10->ax, pVbe->pInt10->bx, pVbe->pInt10->cx);

	    if (pVbe->pInt10->ax & 0xff00) {

		xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
			   "Failed to return TV standard.\n");
	    } else {
		tv_std = pVbe->pInt10->cx & 0x00ff;
		switch (tv_std) {
		case ATI_TV_STD_NTSC:
		case ATI_TV_STD_PAL:
		case ATI_TV_STD_PALM:
		case ATI_TV_STD_PAL60:
		case ATI_TV_STD_NTSCJ:
		case ATI_TV_STD_PALCN:
		case ATI_TV_STD_PALN:
		case ATI_TV_STD_SCARTPAL:
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, 
			       "Current TV standard: %s\n", ATITVStandardNames[tv_std]);
		    break;
		default:
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
			       "Unrecognized TV standard return code cx=0x%04x\n", 
			       pVbe->pInt10->cx);
		}

		tv_std_request = pATI->OptionTvStd;
		if (tv_std_request < 0 || 
		    tv_std_request > ATI_TV_STD_NONE || 
		    tv_std_request == ATI_TV_STD_RESERVED1 || 
		    tv_std_request == ATI_TV_STD_RESERVED2) {
		    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
			       "Invalid TV standard requested, please check configuration file\n");
		} else if (tv_std_request != ATI_TV_STD_NONE) {
		    /* Set TV standard if requested (LT Pro not supported) */
		    if (pATI->Chip != ATI_CHIP_264LTPRO &&
			tv_std_request != tv_std) {
				
			pVbe->pInt10->num = 0x10;
			pVbe->pInt10->ax = 0xa070;
			pVbe->pInt10->bx = 0x0003; /* sub-function: set TV standard */
			pVbe->pInt10->cx = tv_std_request;
			xf86ExecX86int10(pVbe->pInt10);
			if (pVbe->pInt10->ax & 0xff00)
			    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
				       "Failed to set TV standard\n");
			else
			    xf86DrvMsg(pScreenInfo->scrnIndex, X_CONFIG, 
				       "Set TV standard to %s\n", ATITVStandardNames[tv_std_request]);
		    } else {
			xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, 
				   "Setting TV standard not supported on ATI Rage LT Pro\n");
		    }
		}
	    }
	    
	}
    } else {
	xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING, "VBE module not loaded\n");
    }
}

#endif /* TV_OUT */

/*
 * ATIEnterGraphics --
 *
 * This function sets the hardware to a graphics video state.
 */
Bool
ATIEnterGraphics
(
    ScreenPtr   pScreen,
    ScrnInfoPtr pScreenInfo,
    ATIPtr      pATI
)
{
    /* Map apertures */
    if (!ATIMapApertures(pScreenInfo->scrnIndex, pATI))
        return FALSE;

    /* Unlock device */
    ATIUnlock(pATI);

    /* Calculate hardware data */
    if (pScreen &&
        !ATIModeCalculate(pScreenInfo->scrnIndex, pATI, &pATI->NewHW,
            pScreenInfo->currentMode))
        return FALSE;

    pScreenInfo->vtSema = TRUE;

#ifdef TV_OUT
    if (pATI->OptionTvOut) {

	if (pATI->pVBE) {
	    if (VBEGetVBEMode(pATI->pVBE, &pATI->vbemode)) {
		xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO, "Saving VESA mode: 0x%x\n", 
			   pATI->vbemode);
	    }
	}
    }
#endif /* TV_OUT */

    /* Save current state */
    ATIModeSave(pScreenInfo, pATI, &pATI->OldHW);

#ifdef TV_OUT
    if (pATI->OptionTvOut) 
	ATIProbeAndSetActiveDisplays(pScreenInfo, pATI);
#endif /* TV_OUT */

    /* Set graphics state */
    ATIModeSet(pScreenInfo, pATI, &pATI->NewHW);

    /* Possibly blank the screen */
    if (pScreen)
       (void)ATISaveScreen(pScreen, SCREEN_SAVER_ON);

    /* Position the screen */
    (*pScreenInfo->AdjustFrame)(ADJUST_FRAME_ARGS(pScreenInfo,
        pScreenInfo->frameX0, pScreenInfo->frameY0));

    SetTimeSinceLastInputEvent();

    return TRUE;
}

/*
 * ATILeaveGraphics --
 *
 * This function restores the hardware to its previous state.
 */
void
ATILeaveGraphics
(
    ScrnInfoPtr pScreenInfo,
    ATIPtr      pATI
)
{
    if (pScreenInfo->vtSema)
    {
        /* If not exiting, save graphics video state */
        if (!xf86ServerIsExiting())
            ATIModeSave(pScreenInfo, pATI, &pATI->NewHW);

#ifdef TV_OUT
	if (pATI->OptionTvOut) 
	    ATIProbeAndSetActiveDisplays(pScreenInfo, pATI);
#endif /* TV_OUT */

        /* Restore mode in effect on server entry */
        ATIModeSet(pScreenInfo, pATI, &pATI->OldHW);

        pScreenInfo->vtSema = FALSE;
    }

    /* Lock device */
    ATILock(pATI);

    /* Unmap apertures */

#ifdef AVOID_DGA

    if (!pATI->Closeable)

#else /* AVOID_DGA */

    if (!pATI->Closeable || !pATI->nDGAMode)

#endif /* AVOID_DGA */

        ATIUnmapApertures(pScreenInfo->scrnIndex, pATI);

    SetTimeSinceLastInputEvent();
}

/*
 * ATISwitchMode --
 *
 * This function switches to another graphics video state.
 */
Bool
ATISwitchMode(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    ATIPtr      pATI        = ATIPTR(pScreenInfo);

    /* Calculate new hardware data */
    if (!ATIModeCalculate(pScreenInfo->scrnIndex, pATI, &pATI->NewHW, pMode))
        return FALSE;

    /* Set new hardware state */
    if (pScreenInfo->vtSema)
    {
        pScreenInfo->currentMode = pMode;

#ifdef XF86DRI_DEVEL

        if (pATI->directRenderingEnabled) 
        {
            DRILock(pScreenInfo->pScreen,0);
	    ATIDRIWaitForIdle(pATI);
        }

#endif /* XF86DRI_DEVEL */

        /* XXX Workaround for X server not hiding the cursor for Xcursor (but
         * only for core cursor), leaving a 64x64 garbage upper-left.
         */
        if (pATI->pCursorInfo)
            (*pATI->pCursorInfo->HideCursor)(pScreenInfo);

        ATIModeSet(pScreenInfo, pATI, &pATI->NewHW);

#ifdef XF86DRI_DEVEL

        if (pATI->directRenderingEnabled) 
        {
            DRIUnlock(pScreenInfo->pScreen);
        }

#endif /* XF86DRI_DEVEL */

    }

    SetTimeSinceLastInputEvent();

    return TRUE;
}

/*
 * ATIEnterVT --
 *
 * This function sets the server's virtual console to a graphics video state.
 */
Bool
ATIEnterVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    ScreenPtr   pScreen     = pScreenInfo->pScreen;
    ATIPtr      pATI        = ATIPTR(pScreenInfo);
    PixmapPtr   pScreenPixmap;
#if (XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 9, 99, 1, 0))
    DevUnion    PixmapPrivate;
#endif
    Bool        Entered;

    if (!ATIEnterGraphics(NULL, pScreenInfo, pATI))
        return FALSE;

    /* The rest of this isn't needed for shadowfb */
    if (pATI->OptionShadowFB)
    {

#ifdef XF86DRI_DEVEL

        if (pATI->directRenderingEnabled) 
        {
            /* get the Mach64 back into shape after resume */
            ATIDRIResume(pScreen);
            DRIUnlock(pScreen);
        }

#endif /* XF86DRI_DEVEL */

        return TRUE;
    }

    pScreenPixmap = (*pScreen->GetScreenPixmap)(pScreen);

#if (XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 9, 99, 1, 0))
    PixmapPrivate = pScreenPixmap->devPrivate;
    if (!PixmapPrivate.ptr)
        pScreenPixmap->devPrivate = pScreenInfo->pixmapPrivate;
#endif

    /* Tell framebuffer about remapped aperture */
    Entered = (*pScreen->ModifyPixmapHeader)(pScreenPixmap,
        -1, -1, -1, -1, -1, pATI->pMemory);

#if (XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 9, 99, 1, 0))
    if (!PixmapPrivate.ptr)
    {
        pScreenInfo->pixmapPrivate = pScreenPixmap->devPrivate;
        pScreenPixmap->devPrivate.ptr = NULL;
    }
#endif

#ifdef XF86DRI_DEVEL

    if (pATI->directRenderingEnabled) 
    {
        /* get the Mach64 back into shape after resume */
        ATIDRIResume(pScreen);
        DRIUnlock(pScreen);
    }

#endif /* XF86DRI_DEVEL */

    return Entered;
}

/*
 * ATILeaveVT --
 *
 * This function restores the server's virtual console to its state on server
 * entry.
 */
void
ATILeaveVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    ScreenPtr   pScreen     = pScreenInfo->pScreen;
    ATIPtr      pATI        = ATIPTR(pScreenInfo);

#ifdef XF86DRI_DEVEL

    if (pATI->directRenderingEnabled) 
    {
        DRILock(pScreen,0);
        ATIDRIWaitForIdle(pATI);
    }

#endif /* XF86DRI_DEVEL */

    ATILeaveGraphics(pScreenInfo, ATIPTR(pScreenInfo));
}

/*
 * ATIFreeScreen --
 *
 * This function frees all driver data related to a screen.
 */
void
ATIFreeScreen(FREE_SCREEN_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    ATIPtr      pATI        = ATIPTR(pScreenInfo);

    ATII2CFreeScreen(pScreenInfo->scrnIndex);

#ifndef AVOID_CPIO

    free(pATI->OldHW.frame_buffer);
    free(pATI->NewHW.frame_buffer);

#endif /* AVOID_CPIO */

    free(pATI->pShadow);

#ifndef AVOID_DGA

    free(pATI->pDGAMode);

#endif /* AVOID_DGA */

    free(pATI);
    pScreenInfo->driverPrivate = NULL;
}
