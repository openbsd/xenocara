/*
 * Creator, Creator3D and Elite3D framebuffer driver.
 *
 * Copyright (C) 2000 Jakub Jelinek (jakub@redhat.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * JAKUB JELINEK BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "mipointer.h"
#include "micmap.h"
#include "fb.h"

#include "xf86cmap.h"

#include "ffb.h"

static const OptionInfoRec * FFBAvailableOptions(int chipid, int busid);
static void	FFBIdentify(int flags);
static Bool	FFBProbe(DriverPtr drv, int flags);
static Bool	FFBPreInit(ScrnInfoPtr pScrn, int flags);
static Bool	FFBScreenInit(SCREEN_INIT_ARGS_DECL);
static Bool	FFBEnterVT(VT_FUNC_ARGS_DECL);
static void	FFBLeaveVT(VT_FUNC_ARGS_DECL);
static Bool	FFBCloseScreen(CLOSE_SCREEN_ARGS_DECL);
static Bool	FFBSaveScreen(ScreenPtr pScreen, int mode);
static void	FFBDPMSSet(ScrnInfoPtr pScrn, int mode, int flags);

/* Required if the driver supports mode switching */
static Bool	FFBSwitchMode(SWITCH_MODE_ARGS_DECL);
/* Required if the driver supports moving the viewport */
static void	FFBAdjustFrame(ADJUST_FRAME_ARGS_DECL);

/* Optional functions */
static void	FFBFreeScreen(FREE_SCREEN_ARGS_DECL);
static ModeStatus FFBValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode,
			       Bool verbose, int flags);
static void     FFBDPMSMode(ScrnInfoPtr pScrn, int DPMSMode, int flags);
/* ffb_dga.c */
extern void FFB_InitDGA(ScreenPtr pScreen);

void FFBSync(ScrnInfoPtr pScrn);

#define FFB_VERSION 4000
#define FFB_NAME "SUNFFB"
#define FFB_DRIVER_NAME "sunffb"
#define FFB_MAJOR_VERSION PACKAGE_VERSION_MAJOR
#define FFB_MINOR_VERSION PACKAGE_VERSION_MINOR
#define FFB_PATCHLEVEL PACKAGE_VERSION_PATCHLEVEL

/* 
 * This contains the functions needed by the server after loading the driver
 * module.  It must be supplied, and gets passed back by the SetupProc
 * function in the dynamic case.  In the static case, a reference to this
 * is compiled in, and this requires that the name of this DriverRec be
 * an upper-case version of the driver name.
 */

_X_EXPORT DriverRec SUNFFB = {
    FFB_VERSION,
    FFB_DRIVER_NAME,
    FFBIdentify,
    FFBProbe,
    FFBAvailableOptions,
    NULL,
    0
};

typedef enum {
    OPTION_SW_CURSOR,
    OPTION_HW_CURSOR,
    OPTION_NOACCEL
} FFBOpts;

static const OptionInfoRec FFBOptions[] = {
    { OPTION_SW_CURSOR,		"SWcursor",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_HW_CURSOR,		"HWcursor",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_NOACCEL,		"NoAccel",	OPTV_BOOLEAN,	{0}, FALSE },
    { -1,			NULL,		OPTV_NONE,	{0}, FALSE }
};

#ifdef XFree86LOADER

static MODULESETUPPROTO(ffbSetup);

static XF86ModuleVersionInfo sunffbVersRec =
{
	"sunffb",
	MODULEVENDORSTRING,
	MODINFOSTRING1,
	MODINFOSTRING2,
	XORG_VERSION_CURRENT,
	FFB_MAJOR_VERSION, FFB_MINOR_VERSION, FFB_PATCHLEVEL,
	ABI_CLASS_VIDEODRV,
	ABI_VIDEODRV_VERSION,
	MOD_CLASS_VIDEODRV,
	{0,0,0,0}
};

_X_EXPORT XF86ModuleData sunffbModuleData = { &sunffbVersRec, ffbSetup, NULL };

pointer
ffbSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    static Bool setupDone = FALSE;

    if (!setupDone) {
	setupDone = TRUE;
	xf86AddDriver(&SUNFFB, module, 0);

	/*
	 * Modules that this driver always requires can be loaded here
	 * by calling LoadSubModule().
	 */

	/*
	 * The return value must be non-NULL on success even though there
	 * is no TearDownProc.
	 */
	return (pointer)TRUE;
    } else {
	if (errmaj) *errmaj = LDR_ONCEONLY;
	return NULL;
    }
}

#endif /* XFree86LOADER */

static Bool
FFBGetRec(ScrnInfoPtr pScrn)
{
    /*
     * Allocate an FFBRec, and hook it into pScrn->driverPrivate.
     * pScrn->driverPrivate is initialised to NULL, so we can check if
     * the allocation has already been done.
     */
    if (pScrn->driverPrivate != NULL)
	return TRUE;

    pScrn->driverPrivate = xnfcalloc(sizeof(FFBRec), 1);
    return TRUE;
}

static void
FFBFreeRec(ScrnInfoPtr pScrn)
{
    FFBPtr pFfb;

    if (pScrn->driverPrivate == NULL)
	return;

    pFfb = GET_FFB_FROM_SCRN(pScrn);

    free(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;

    return;
}

static const OptionInfoRec *
FFBAvailableOptions(int chipid, int busid)
{
    return FFBOptions;
}

/* Mandatory */
static void
FFBIdentify(int flags)
{
    xf86Msg(X_INFO, "%s: driver for Creator, Creator 3D and Elite 3D\n", FFB_NAME);
}


/* Mandatory */
static Bool
FFBProbe(DriverPtr drv, int flags)
{
    int i;
    GDevPtr *devSections;
    int *usedChips;
    int numDevSections;
    int numUsed;
    Bool foundScreen = FALSE;
    EntityInfoPtr pEnt;

    /*
     * The aim here is to find all cards that this driver can handle,
     * and for the ones not already claimed by another driver, claim the
     * slot, and allocate a ScrnInfoRec.
     *
     * This should be a minimal probe, and it should under no circumstances
     * change the state of the hardware.  Because a device is found, don't
     * assume that it will be used.  Don't do any initialisations other than
     * the required ScrnInfoRec initialisations.  Don't allocate any new
     * data structures.
     */

    /*
     * Next we check, if there has been a chipset override in the config file.
     * For this we must find out if there is an active device section which
     * is relevant, i.e., which has no driver specified or has THIS driver
     * specified.
     */

    if ((numDevSections = xf86MatchDevice(FFB_DRIVER_NAME,
					  &devSections)) <= 0) {
	/*
	 * There's no matching device section in the config file, so quit
	 * now.
	 */
	return FALSE;
    }

    /*
     * We need to probe the hardware first.  We then need to see how this
     * fits in with what is given in the config file, and allow the config
     * file info to override any contradictions.
     */

    numUsed = xf86MatchSbusInstances(FFB_NAME, SBUS_DEVICE_FFB,
		   devSections, numDevSections,
		   drv, &usedChips);
				    
    free(devSections);
    if (numUsed <= 0)
	return FALSE;

    if (flags & PROBE_DETECT)
	foundScreen = TRUE;
    else for (i = 0; i < numUsed; i++) {
	pEnt = xf86GetEntityInfo(usedChips[i]);

	/*
	 * Check that nothing else has claimed the slots.
	 */
	if(pEnt->active) {
	    ScrnInfoPtr pScrn;
	    
	    /* Allocate a ScrnInfoRec and claim the slot */
	    pScrn = xf86AllocateScreen(drv, 0);

	    /* Fill in what we can of the ScrnInfoRec */
	    pScrn->driverVersion = FFB_VERSION;
	    pScrn->driverName	 = FFB_DRIVER_NAME;
	    pScrn->name		 = FFB_NAME;
	    pScrn->Probe	 = FFBProbe;
	    pScrn->PreInit	 = FFBPreInit;
	    pScrn->ScreenInit	 = FFBScreenInit;
  	    pScrn->SwitchMode	 = FFBSwitchMode;
  	    pScrn->AdjustFrame	 = FFBAdjustFrame;
	    pScrn->EnterVT	 = FFBEnterVT;
	    pScrn->LeaveVT	 = FFBLeaveVT;
	    pScrn->FreeScreen	 = FFBFreeScreen;
	    pScrn->ValidMode	 = FFBValidMode;
	    xf86AddEntityToScreen(pScrn, pEnt->index);
	    foundScreen = TRUE;
	}
	free(pEnt);
    }
    free(usedChips);
    return foundScreen;
}

/* Mandatory */
static Bool
FFBPreInit(ScrnInfoPtr pScrn, int flags)
{
    FFBPtr pFfb;
    sbusDevicePtr psdp;
    MessageType from;
    int i;

    if (flags & PROBE_DETECT) return FALSE;

    /*
     * Note: This function is only called once at server startup, and
     * not at the start of each server generation.  This means that
     * only things that are persistent across server generations can
     * be initialised here.  xf86Screens[] is (pScrn is a pointer to one
     * of these).  Privates allocated using xf86AllocateScrnInfoPrivateIndex()  
     * are too, and should be used for data that must persist across
     * server generations.
     *
     * Per-generation data should be allocated with
     * AllocateScreenPrivateIndex() from the ScreenInit() function.
     */

    /* Allocate the FFBRec driverPrivate */
    if (!FFBGetRec(pScrn))
	return FALSE;

    pFfb = GET_FFB_FROM_SCRN(pScrn);
    
    /* Set pScrn->monitor */
    pScrn->monitor = pScrn->confScreen->monitor;

    /* This driver doesn't expect more than one entity per screen */
    if (pScrn->numEntities > 1)
	return FALSE;
    /* This is the general case */
    for (i = 0; i < pScrn->numEntities; i++) {
	EntityInfoPtr pEnt = xf86GetEntityInfo(pScrn->entityList[i]);

	/* FFB is purely UPA (but we handle it as SBUS) */
	if (pEnt->location.type == BUS_SBUS) {
	    psdp = xf86GetSbusInfoForEntity(pEnt->index);
	    pFfb->psdp = psdp;
	} else
	    return FALSE;
    }

    /*********************
    deal with depth
    *********************/
    
    if (!xf86SetDepthBpp(pScrn, 24, 0, 32, Support32bppFb)) {
	return FALSE;
    } else {
	/* Check that the returned depth is one we support */
	switch (pScrn->depth) {
	case 24:
	    /* OK */
	    break;
	default:
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Given depth (%d) is not supported by this driver\n",
		       pScrn->depth);
	    return FALSE;
	}
    }

    /* Collect all of the relevant option flags (fill in pScrn->options) */
    xf86CollectOptions(pScrn, NULL);
    /* Process the options */
    if (!(pFfb->Options = malloc(sizeof(FFBOptions))))
	return FALSE;
    memcpy(pFfb->Options, FFBOptions, sizeof(FFBOptions));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pFfb->Options);
    
    /*
     * This must happen after pScrn->display has been set because
     * xf86SetWeight references it.
     */
    if (pScrn->depth > 8) {
	rgb weight = {8, 8, 8};
	rgb mask = {0xff, 0xff00, 0xff0000};

	if (!xf86SetWeight(pScrn, weight, mask)) {
	    return FALSE;
	}
    }

    if (!xf86SetDefaultVisual(pScrn, -1))
	return FALSE;

    /*
     * The new cmap code requires this to be initialised.
     */

    {
	Gamma zeros = {0.0, 0.0, 0.0};

	if (!xf86SetGamma(pScrn, zeros)) {
	    return FALSE;
	}
    }

    /* Set the bits per RGB for 8bpp mode */
    from = X_DEFAULT;

    /* determine whether we use hardware or software cursor */
    
    pFfb->HWCursor = TRUE;
    if (xf86GetOptValBool(pFfb->Options, OPTION_HW_CURSOR, &pFfb->HWCursor))
	from = X_CONFIG;
    if (xf86ReturnOptValBool(pFfb->Options, OPTION_SW_CURSOR, FALSE)) {
	from = X_CONFIG;
	pFfb->HWCursor = FALSE;
    }
    
    xf86DrvMsg(pScrn->scrnIndex, from, "Using %s cursor\n",
		pFfb->HWCursor ? "HW" : "SW");

    if (xf86ReturnOptValBool(pFfb->Options, OPTION_NOACCEL, FALSE)) {
	pFfb->NoAccel = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Acceleration disabled\n");
    }
        
    if (xf86LoadSubModule(pScrn, "fb") == NULL) {
	FFBFreeRec(pScrn);
	return FALSE;
    }

    if (!pFfb->NoAccel) {
        if (xf86LoadSubModule(pScrn, "xaa") == NULL) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "Loading XAA failed, acceleration disabled\n");
            pFfb->NoAccel = TRUE;
        }
    }

    if (pFfb->HWCursor && xf86LoadSubModule(pScrn, "ramdac") == NULL) {
	FFBFreeRec(pScrn);
	return FALSE;
    }

    if (xf86LoadSubModule(pScrn, "dbe") == NULL) {
	FFBFreeRec(pScrn);
	return FALSE;
    }


    /*********************
    set up clock and mode stuff
    *********************/
    
    pScrn->progClock = TRUE;

    if(pScrn->display->virtualX || pScrn->display->virtualY) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "FFB does not support a virtual desktop\n");
	pScrn->display->virtualX = 0;
	pScrn->display->virtualY = 0;
    }

    xf86SbusUseBuiltinMode(pScrn, pFfb->psdp);
    pScrn->currentMode = pScrn->modes;
    pScrn->displayWidth = pScrn->virtualX;

    /* Set display resolution */
    xf86SetDpi(pScrn, 0, 0);

    return TRUE;
}

/* Determine the FFB/AFB board type.  We need this information even
 * if acceleration is disabled because the ramdac support layer needs
 * to know what kind of FFB/AFB this is.
 */
static void
FFBProbeBoardType(FFBPtr pFfb)
{
	ffb_fbcPtr ffb = pFfb->regs;
	volatile unsigned int *afb_fem;
	unsigned int val;

	afb_fem = ((volatile unsigned int *) ((char *)ffb + 0x1540));
	val = *afb_fem;
	val &= 0x7f;

	xf86Msg(X_INFO, "%s: ", pFfb->psdp->device);
	if (val == 0x3f || val == 0x07 || val == 0x01) {
		/* When firmware has not been loaded onto AFB we
		 * just assume it is an M6 board.
		 */
		if (val == 0x3f || val != 0x07) {
			pFfb->ffb_type = afb_m6;
			ErrorF("AFB: Detected Elite3D/M6.\n");
		} else {
			pFfb->ffb_type = afb_m3;
			ErrorF("AFB: Detected Elite3D/M3.\n");
		}

		/* These attributes are invariant on AFB. */
		pFfb->has_double_res = 0;
		pFfb->has_z_buffer = 1;
		pFfb->has_double_buffer = 1;
	} else {
		unsigned char sbits;

		/* Read the board strapping bits twice, because sometimes
		 * the strapping pins can get misrouted to the bus interface
		 * on the first attempt.  The second attempt will get the
		 * correct value.
		 */
		sbits = *((volatile unsigned char *)pFfb->strapping_bits);
		sbits = *((volatile unsigned char *)pFfb->strapping_bits);
		switch (sbits & 0x78) {
		case (0x0 << 5) | (0x0 << 3):
			pFfb->ffb_type = ffb1_prototype;
			ErrorF("Detected FFB1 pre-FCS prototype, ");
			break;
		case (0x0 << 5) | (0x1 << 3):
			pFfb->ffb_type = ffb1_standard;
			ErrorF("Detected FFB1, ");
			break;
		case (0x0 << 5) | (0x3 << 3):
			pFfb->ffb_type = ffb1_speedsort;
			ErrorF("Detected FFB1-SpeedSort, ");
			break;
		case (0x1 << 5) | (0x0 << 3):
			pFfb->ffb_type = ffb2_prototype;
			ErrorF("Detected FFB2/vertical pre-FCS prototype, ");
			break;
		case (0x1 << 5) | (0x1 << 3):
			pFfb->ffb_type = ffb2_vertical;
			ErrorF("Detected FFB2/vertical, ");
			break;
		case (0x1 << 5) | (0x2 << 3):
			pFfb->ffb_type = ffb2_vertical_plus;
			ErrorF("Detected FFB2+/vertical, ");
			break;
		case (0x2 << 5) | (0x0 << 3):
			pFfb->ffb_type = ffb2_horizontal;
			ErrorF("Detected FFB2/horizontal, ");
			break;
		case (0x2 << 5) | (0x2 << 3):
			pFfb->ffb_type = ffb2_horizontal;
			ErrorF("Detected FFB2+/horizontal, ");
			break;
		default:
			pFfb->ffb_type = ffb2_vertical;
			ErrorF("Unknown boardID[%08x], assuming FFB2, ", sbits);
			break;
		};

		if (sbits & (1 << 2)) {
			ErrorF("DoubleRES, ");
			pFfb->has_double_res = 1;
		} else {
			pFfb->has_double_res = 0;
		}
		if (sbits & (1 << 1)) {
			ErrorF("Z-buffer, ");
			pFfb->has_z_buffer = 1;
		} else {
			pFfb->has_z_buffer = 0;
		}
		if (sbits & (1 << 0)) {
			/* This state really means to the driver that the double
			 * buffers are available for hw accelerate Dbe.  When the
			 * FFB is in high-resolution mode, the buffers are combined
			 * into one single large framebuffer.  So in high-resolution
			 * hw accelerated double-buffering is not available.
			 */
			if ((ffb->fbcfg0 & FFB_FBCFG0_RES_MASK) != FFB_FBCFG0_RES_HIGH)
				pFfb->has_double_buffer = 1;
			else
				pFfb->has_double_buffer = 0;
		} else {
			pFfb->has_double_buffer = 0;
		}
		if (pFfb->has_double_buffer)
			ErrorF("Double-buffered.\n");
		else
			ErrorF("Single-buffered.\n");
	}
}

/* Mandatory */

/* This gets called at the start of each server generation */

static Bool
FFBScreenInit(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr pScrn;
    FFBPtr pFfb;
    int ret;
    unsigned int afb_fem;
    VisualPtr visual;

    /* 
     * First get the ScrnInfoRec
     */
    pScrn = xf86ScreenToScrn(pScreen);

    pFfb = GET_FFB_FROM_SCRN(pScrn);

    /* Map the FFB framebuffer, for each view. */

    /* 24-bit RGB Dumb view */
    pFfb->fb = pFfb->dfb24 =
	xf86MapSbusMem (pFfb->psdp, FFB_DFB24_VOFF, 0x1000000);

    if (! pFfb->dfb24)
	return FALSE;

    /* 8-bit R Dumb view */
    pFfb->dfb8r =
	xf86MapSbusMem (pFfb->psdp, FFB_DFB8R_VOFF, 0x400000);

    if (! pFfb->dfb8r)
	return FALSE;

    /* 8-bit X Dumb view */
    pFfb->dfb8x =
	xf86MapSbusMem (pFfb->psdp, FFB_DFB8X_VOFF, 0x400000);

    if (! pFfb->dfb8x)
	return FALSE;

    /* 32-bit RGB Smart view */
    pFfb->sfb32 =
	xf86MapSbusMem (pFfb->psdp, FFB_SFB32_VOFF, 0x1000000);

    if (!pFfb->sfb32)
	return FALSE;

    /* 8-bit R Smart view */
    pFfb->sfb8r =
	xf86MapSbusMem(pFfb->psdp, FFB_SFB8R_VOFF, 0x400000);

    if (!pFfb->sfb8r)
	return FALSE;

    /* 8-bit X Smart view */
    pFfb->sfb8x =
	xf86MapSbusMem(pFfb->psdp, FFB_SFB8X_VOFF, 0x400000);

    if (!pFfb->sfb8x)
	return FALSE;

    /* Map the rendering pipeline */
    pFfb->regs =
	xf86MapSbusMem (pFfb->psdp, FFB_FBC_REGS_VOFF, 16384);

    if (! pFfb->regs)
	return FALSE;

    /* Map the ramdac */
    pFfb->dac =
	xf86MapSbusMem (pFfb->psdp, FFB_DAC_VOFF, 8192);

    if (! pFfb->dac)
	return FALSE;

    /* Map the board strapping bits */
    pFfb->strapping_bits = (volatile unsigned int *)
	    xf86MapSbusMem(pFfb->psdp, FFB_EXP_VOFF, 8192);

    if (! pFfb->strapping_bits)
	return FALSE;

    /* Probe for the type of FFB/AFB we have. */
    FFBProbeBoardType(pFfb);

    /* Now that we have the board type, we can init the ramdac layer. */
    if (FFBDacInit(pFfb) == FALSE)
	return FALSE;

    /* OK, a fun gross hack to detect if this is
     * AFB and if so whether the correct firmware
     * has been loaded.  The machine will flatline
     * if you try to use certain acceleration features
     * without the full firmware loaded.
     *
     * The bootup Elite3D/AFB firmware is minimal, and
     * will leave the FloatEnableMask register at a
     * value of 0x01.  Creator{,3D} lacks the FEM register
     * and will return a "nonsense" value on attempts to
     * read this location.  After experimentation, an
     * appropriate definition for "nonsense" seems to
     * be anything with all low 7 bits not 0x3f, 0x07,
     * of 0x01.
     *
     * If the FEM register is non-zero and is some value
     * other than 0x1 (usually 0x3f or 0x7 depending upon
     * whether the card has 3 or 6 floats) we can assume
     * the correct firmware has been loaded. -DaveM
     */
    afb_fem = *(unsigned int *)((char *)pFfb->regs + 0x1540);
    if ((afb_fem & 0x7f) != 0x3f &&
	(afb_fem & 0x7f) != 0x07 &&
	(afb_fem & 0x7f) != 0x01)
	xf86Msg(X_INFO, "%s: Detected Creator/Creator3D\n", pFfb->psdp->device);
    else {
	xf86Msg(X_INFO, "%s: Detected Elite3D M3/M6, checking firmware...\n", pFfb->psdp->device);
	if (afb_fem == 0x1) {
	    xf86Msg(X_INFO, "%s: ... AFB firmware not loaded\n", pFfb->psdp->device);
	    if (!pFfb->NoAccel) {
		xf86Msg(X_WARNING, "%s: Forcing no acceleration on Elite3D M3/M6\n", pFfb->psdp->device);
		pFfb->NoAccel = TRUE;
	    }
	} else
	    xf86Msg(X_INFO, "%s: ... AFB firmware is loaded\n", pFfb->psdp->device);
    }

    /* Darken the screen for aesthetic reasons and set the viewport */
    FFBSaveScreen(pScreen, SCREEN_SAVER_ON);

    /*
     * The next step is to setup the screen's visuals, and initialise the
     * framebuffer code.  In cases where the framebuffer's default
     * choices for things like visual layouts and bits per RGB are OK,
     * this may be as simple as calling the framebuffer's ScreenInit()
     * function.  If not, the visuals will need to be setup before calling
     * a fb ScreenInit() function and fixed up after.
     */

    /*
     * Reset visual list.
     */
    miClearVisualTypes();

    /* Setup the visuals we support. */
    if (!miSetVisualTypes(24, TrueColorMask,
			  pScrn->rgbBits, TrueColor))
	    return FALSE;

    if (!miSetPixmapDepths())
        return FALSE;

    /*
     * Call the framebuffer layer's ScreenInit function, and fill in other
     * pScreen fields.
     */
    ret = fbScreenInit(pScreen, (pFfb->NoAccel ? pFfb->dfb24 : pFfb->sfb32),
		       pScrn->virtualX, pScrn->virtualY,
		       pScrn->xDpi, pScrn->yDpi,
		       2048, 32);

    if (!ret)
	return FALSE;

    if (pScrn->bitsPerPixel > 8) {
        /* Fixup RGB ordering */
        visual = pScreen->visuals + pScreen->numVisuals;
        while (--visual >= pScreen->visuals) {
	    if ((visual->class | DynamicClass) == DirectColor) {
		visual->offsetRed = pScrn->offset.red;
		visual->offsetGreen = pScrn->offset.green;
		visual->offsetBlue = pScrn->offset.blue;
		visual->redMask = pScrn->mask.red;
		visual->greenMask = pScrn->mask.green;
		visual->blueMask = pScrn->mask.blue;
	    }
	}
    }

    if (!fbPictureInit(pScreen, NULL, 0) &&
	(serverGeneration == 1))
      xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		 "RENDER extension initialisation failed.\n");

    xf86SetBlackWhitePixels(pScreen);

    if (!pFfb->NoAccel) {
	if (!FFBAccelInit(pScreen, pFfb))
	    return FALSE;
	xf86Msg(X_INFO, "%s: Using acceleration\n", pFfb->psdp->device);
    }


    xf86SetBackingStore(pScreen);
    xf86SetSilkenMouse(pScreen);

    /* Initialise cursor functions */
    miDCInitialize (pScreen, xf86GetPointerScreenFuncs());

    /* Initialize HW cursor layer. 
     * Must follow software cursor initialization.
     */
    if (pFfb->HWCursor) { 
	if(!FFBHWCursorInit(pScreen)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, 
		       "Hardware cursor initialization failed\n");
	    return(FALSE);
	}
	xf86SbusHideOsHwCursor(pFfb->psdp);
    }

    /* Initialise default colourmap. */
    if (!miCreateDefColormap(pScreen))
	return FALSE;

    /* Initialize colormap layer.
     * Must follow initialization of the default colormap.
     */
    if (!xf86HandleColormaps(pScreen, 256, 8,
			     FFBDacLoadPalette, NULL,
			     CMAP_LOAD_EVEN_IF_OFFSCREEN |
			     CMAP_RELOAD_ON_MODE_SWITCH))
	return FALSE;

    /* Setup DGA support. */
    if (!pFfb->NoAccel)
	    FFB_InitDGA(pScreen);

    xf86DPMSInit(pScreen, FFBDPMSSet, 0);

    pFfb->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = FFBCloseScreen;
    pScreen->SaveScreen = FFBSaveScreen;

    /* Report any unused options (only for the first generation) */
    if (serverGeneration == 1) {
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);
    }

    /* unblank the screen */
    FFBSaveScreen(pScreen, SCREEN_SAVER_OFF);

    /* Done */
    return TRUE;
}


/* Usually mandatory */
static Bool
FFBSwitchMode(SWITCH_MODE_ARGS_DECL)
{
    return TRUE;
}


/*
 * This function is used to initialize the Start Address - the first
 * displayed location in the video memory.
 */
/* Usually mandatory */
static void 
FFBAdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
    /* we don't support virtual desktops */
    return;
}

/*
 * This is called when VT switching back to the X server.  Its job is
 * to reinitialise the video mode.
 */

/* Mandatory */
static Bool
FFBEnterVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);

    pFfb->vtSema = FALSE;
    if (!pFfb->NoAccel)
	CreatorVtChange (pScrn->pScreen, TRUE);
    if (pFfb->HWCursor)
	xf86SbusHideOsHwCursor (pFfb->psdp);

    FFBDacEnterVT(pFfb);

    return TRUE;
}


/*
 * This is called when VT switching away from the X server.
 */

/* Mandatory */
static void
FFBLeaveVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);

    FFBDacLeaveVT(pFfb);

    if (!pFfb->NoAccel)
	CreatorVtChange (pScrn->pScreen, FALSE);

    if (pFfb->HWCursor)
	xf86SbusHideOsHwCursor (pFfb->psdp);

    pFfb->vtSema = TRUE;
    return;
}


/*
 * This is called at the end of each server generation.  It restores the
 * original (text) mode.  It should really also unmap the video memory too.
 */

/* Mandatory */
static Bool
FFBCloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);

	/* Disable hardware cursor. */
	FFBDacCursorEnableDisable(pFfb, 0);

	/* Restore kernel ramdac state before we unmap registers. */
	FFBDacFini(pFfb);

	pScrn->vtSema = FALSE;

	xf86UnmapSbusMem(pFfb->psdp, pFfb->dfb24, 0x1000000);
	xf86UnmapSbusMem(pFfb->psdp, pFfb->dfb8r, 0x400000);
	xf86UnmapSbusMem(pFfb->psdp, pFfb->dfb8x, 0x400000);
	xf86UnmapSbusMem(pFfb->psdp, pFfb->sfb32, 0x1000000);
	xf86UnmapSbusMem(pFfb->psdp, pFfb->sfb8r, 0x400000);
	xf86UnmapSbusMem(pFfb->psdp, pFfb->sfb8x, 0x400000);
	xf86UnmapSbusMem(pFfb->psdp, pFfb->regs, 16384);
	xf86UnmapSbusMem(pFfb->psdp, pFfb->dac, 8192);
	xf86UnmapSbusMem(pFfb->psdp, (void *)pFfb->strapping_bits, 8192);

	if (pFfb->HWCursor)
		xf86SbusHideOsHwCursor (pFfb->psdp);

	pScreen->CloseScreen = pFfb->CloseScreen;
	return (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);
}


/* Free up any per-generation data structures */

/* Optional */
static void
FFBFreeScreen(FREE_SCREEN_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);
	FFBFreeRec(pScrn);
}


/* Checks if a mode is suitable for the selected chipset. */

/* Optional */
static ModeStatus
FFBValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode, Bool verbose, int flags)
{
	if (mode->Flags & V_INTERLACE)
		return MODE_BAD;

	return MODE_OK;
}

/* Do screen blanking */

/* Mandatory */
static Bool
FFBSaveScreen(ScreenPtr pScreen, int mode)
    /* This function blanks the screen when mode=SCREEN_SAVER_ON and
       unblanks it when mode=SCREEN_SAVER_OFF.  It is used internally in the
       FFBScreenInit code `for aesthetic reasons,' and it is used for
       blanking if you set "xset s on s blank."  The work (such as it is) is
       done in "ffb_dac.c" `for aesthetic reasons.'
    */
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);

    return FFBDacSaveScreen(GET_FFB_FROM_SCRN(pScrn), mode);
}

static void
FFBDPMSSet(ScrnInfoPtr pScrn, int mode, int flags)
{
	FFBPtr pFfb = GET_FFB_FROM_SCRN(pScrn);

	FFBDacDPMSMode(pFfb, mode, 0);
}

/*
 * This is the implementation of the Sync() function.
 */
void
FFBSync(ScrnInfoPtr pScrn)
{
    return;
}

/*
  Hook for DPMS Mode.
*/

static void
FFBDPMSMode(ScrnInfoPtr pScrn, int DPMSMode, int flags)
{
  FFBDacDPMSMode(GET_FFB_FROM_SCRN(pScrn), DPMSMode, flags);
}
