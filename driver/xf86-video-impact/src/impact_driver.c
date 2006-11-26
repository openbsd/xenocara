/*
 * impact_driver.c 2005/07/12 23:24:15, Copyright (c) 2005 peter fuerst
 *
 * Driver for the SGI Impact/ImpactSR graphics card
 *
 * This driver is based on the
 * newport driver, (c) 2000,2001 Guido Guenther <agx@sigxcpu.org>,
 * # newport_driver.c,v 1.2 2000/11/29 20:58:10 agx Exp #
 * # xc/programs/Xserver/hw/xfree86/drivers/newport/newport_driver.c,v
 *   1.19 2002/01/04 21:22:33 tsi Exp #
 *
 * The newport driver is based on the newport.c & newport_con.c kernel code
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is fur-
 * nished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
 * NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CON-
 * NECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the XFree86 Project shall not
 * be used in advertising or otherwise to promote the sale, use or other deal-
 * ings in this Software without prior written authorization from the XFree86
 * Project.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* [un]mmap */
#include <sys/mman.h>

/* function prototypes, common data structures & generic includes */
#include "impact.h"

/* Drivers using the mi SW cursor need: */
#include "mipointer.h"
/* Drivers using the mi implementation of backing store need: */
#include "mibstore.h"
/* Drivers using the mi colourmap code need: */
#include "micmap.h"

/* Drivers using fb need: */
#include "fb.h"

/* Drivers using the shadow frame buffer need: */
#include "shadowfb.h"

/* Xv Extension */
#include "xf86xv.h"
#include <X11/extensions/Xv.h>

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#define IMPACT_MAX_BOARDS 3

#define IMPACT_FB_MMAP_OFF(pool) ((pool)<<24)


#define IMPACT_VERSION			4000
#define IMPACT_NAME		"IMPACT"
#define IMPACT_DRIVER_NAME	"impact"
#define IMPACT_MAJOR_VERSION	0
#define IMPACT_MINOR_VERSION	2
#define IMPACT_PATCHLEVEL	0

struct probed_id {
	unsigned char id;
	unsigned char sr;
};


/* Prototypes ------------------------------------------------------- */
static void	ImpactIdentify(int flags);
static const OptionInfoRec * ImpactAvailableOptions(int chipid, int busid);
static Bool ImpactProbe(DriverPtr drv, int flags);
static Bool ImpactPreInit(ScrnInfoPtr pScrn, int flags);
static Bool ImpactScreenInit(int Index, ScreenPtr pScreen, int argc, char **argv);
static Bool ImpactEnterVT(int scrnIndex, int flags);
static void ImpactLeaveVT(int scrnIndex, int flags);
static Bool ImpactCloseScreen(int scrnIndex, ScreenPtr pScreen);
static Bool ImpactSaveScreen(ScreenPtr pScreen, int mode);
static unsigned ImpactHWProbe(struct probed_id probedIDs[],int lim);	/* return number of found boards */
static Bool ImpactModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
static void ImpactRestore(ScrnInfoPtr pScrn, Bool Closing);
static Bool ImpactMapRegs(ScrnInfoPtr pScrn);
static Bool ImpactMapShadowFB(ScrnInfoPtr pScrn);
static void ImpactUnmapRegs(ScrnInfoPtr pScrn);
static void ImpactUnmapShadowFB(ScrnInfoPtr pScrn);
static Bool ImpactProbeCardInfo(ScrnInfoPtr pScrn);
/* ------------------------------------------------------------------ */

DriverRec IMPACT = {
	IMPACT_VERSION,
	IMPACT_DRIVER_NAME,
	ImpactIdentify,
	ImpactProbe,
	ImpactAvailableOptions,
	NULL,
	0
};

/* Supported "chipsets" */
#define CHIP_MG_3		0x1
#define CHIP_MG_4		0x2

static SymTabRec ImpactChipsets[] = {
	{ CHIP_MG_3, "MardiGrasHQ3" },
	{ CHIP_MG_4, "MardiGrasHQ4" },
	{ -1, NULL }
};

/* List of Symbols from other modules that this module references */

static const char *fbSymbols[] = {
	"fbPictureInit",
	"fbScreenInit",
	NULL
};

static const char *shadowSymbols[] = {
	"ShadowFBInit",
	NULL
};

#ifdef XFree86LOADER

static MODULESETUPPROTO(impactSetup);

static XF86ModuleVersionInfo impactVersRec =
{
	"impact",
	MODULEVENDORSTRING,
	MODINFOSTRING1,
	MODINFOSTRING2,
	XF86_VERSION_CURRENT,
	IMPACT_MAJOR_VERSION, IMPACT_MINOR_VERSION, IMPACT_PATCHLEVEL,
	ABI_CLASS_VIDEODRV,
	ABI_VIDEODRV_VERSION,
	MOD_CLASS_VIDEODRV,
	{0,0,0,0}
};

XF86ModuleData impactModuleData = { &impactVersRec, impactSetup, NULL };

static pointer
impactSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
	static Bool setupDone = FALSE;

	/* This module should be loaded only once, but check to be sure. */
	if (!setupDone) {
		/*
		 * Modules that this driver always requires may be loaded
		 * here  by calling LoadSubModule().
		 */
		setupDone = TRUE;
		xf86AddDriver(&IMPACT, module, 0);
		/*
		 * Tell the loader about symbols from other modules that this module
		 * might refer to.
		 */
		LoaderRefSymLists(fbSymbols, shadowSymbols, NULL);
		/*
		 * The return value must be non-NULL on success even though
		 * there is no TearDownProc.
		 */
		return (pointer)1;
	} else {
		if (errmaj) *errmaj = LDR_ONCEONLY;
		return 0;
	}
}

#endif /* XFree86LOADER */

typedef enum {
	OPTION_BITPLANES,
	OPTION_BUS_ID
} ImpactOpts;

/* Supported options */
static const OptionInfoRec ImpactOptions [] = {
	{ OPTION_BITPLANES, "Depth", OPTV_INTEGER, {0}, FALSE },
	{ OPTION_BUS_ID, "BusID", OPTV_INTEGER, {0}, FALSE },
	{ -1, NULL, OPTV_NONE, {0}, FALSE }
};

/* ------------------------------------------------------------------ */

static Bool
ImpactGetRec(ScrnInfoPtr pScrn)
{
	ImpactPtr pImpact;
	int i, j;

	if (pScrn->driverPrivate)
		return TRUE;

	pScrn->driverPrivate = xnfcalloc(sizeof(ImpactRec), 1);
	pImpact = IMPACTPTR(pScrn);
	memset(pImpact, 0, sizeof(*pImpact));

	for (i = j = 0; i < 256; i++, j += 0x010101)
		pImpact->pseudo_palette[i] = j;

	return TRUE;
}

static Bool
ImpactFreeRec(ScrnInfoPtr pScrn)
{
	if (!pScrn->driverPrivate)
		return TRUE;
	xfree(pScrn->driverPrivate);
	pScrn->driverPrivate = 0;
	return TRUE;
}

static void
ImpactIdentify(int flags)
{
	xf86PrintChipsets(
		IMPACT_NAME, "experimental driver for Impact Graphics Card",
		ImpactChipsets);
}

static Bool
ImpactProbe(DriverPtr drv, int flags)
{
	int numDevSections, numUsed, i, j, busID;
	Bool foundScreen = FALSE;
	GDevPtr *devSections;
	GDevPtr dev = 0;
	resRange range[] = { {ResExcMemBlock,0,0}, _END };
	struct probed_id probedIDs[IMPACT_MAX_BOARDS];
	memType base;

	if ((numDevSections = xf86MatchDevice(IMPACT_DRIVER_NAME,&devSections))
		<= 0)
		return FALSE;
	numUsed = ImpactHWProbe(probedIDs, IMPACT_MAX_BOARDS);
	if (numUsed <= 0)
		return FALSE;

	if (flags & PROBE_DETECT)
		foundScreen = TRUE;
	else {
		for (i = 0; i < numDevSections; i++) {
			dev = devSections[i];
			busID = xf86SetIntOption(dev->options, "BusID", 0);

			for (j = 0; j < numUsed; j++)
				if (busID == probedIDs[j].id) {
					int entity;
					ScrnInfoPtr pScrn = 0;
					/*
					 * This is a hack because don't have the RAC info (and
					 * don't want it).  Set it as an ISA entity to get the
					 * entity field set up right.
					 */
					entity = xf86ClaimIsaSlot(drv, 0, dev, TRUE);
					if (probedIDs[j].sr) {
						base = (IMPACTSR_BASE_ADDR0 + busID * IMPACTSR_BASE_OFFSET);
						RANGE(range[0], base,
							base + sizeof(ImpactSRRegs), ResExcMemBlock);
					} else {
						base = (IMPACTI2_BASE_ADDR0 + busID * IMPACTI2_BASE_OFFSET);
						RANGE(range[0], base,
							base + sizeof(ImpactI2Regs), ResExcMemBlock);
					}
					pScrn = xf86ConfigIsaEntity(pScrn, 0, entity, NULL,
									range, NULL, NULL, NULL, NULL);
					/* Allocate a ScrnInfoRec */
					pScrn->driverVersion = IMPACT_VERSION;
					pScrn->driverName = IMPACT_DRIVER_NAME;
					pScrn->name = IMPACT_NAME;
					pScrn->Probe = ImpactProbe;
					pScrn->PreInit = ImpactPreInit;
					pScrn->ScreenInit = ImpactScreenInit;
					pScrn->EnterVT = ImpactEnterVT;
					pScrn->LeaveVT = ImpactLeaveVT;
					pScrn->driverPrivate =
						(void*)(busID | (long)probedIDs[j].sr<<16);
					foundScreen = TRUE;
					break;
				}
		}
	}
	xfree(devSections);
	return foundScreen;
}

/* most of this is from DESIGN.TXT s20.3.6 */
static Bool
ImpactPreInit(ScrnInfoPtr pScrn, int flags)
{
	long busID;
	int i;
	ImpactPtr pImpact;
	MessageType from;
	ClockRangePtr clockRanges;
	pointer pFbMod = 0;

	if (flags & PROBE_DETECT) return FALSE;

	if (pScrn->numEntities != 1)
		return FALSE;

	busID = (long)pScrn->driverPrivate;
	pScrn->driverPrivate = 0;

	/* Fill in the monitor field */
	pScrn->monitor = pScrn->confScreen->monitor;

	if ( !xf86SetDepthBpp(pScrn, 8, 8, 8, ImpactDepth24Flags()) )
		return FALSE;

	switch (pScrn->depth) {
		/* check if the returned depth is one we support */
		case 8:
		case 24:
			/* OK */
			break;
		default:
			xf86DrvMsg(
				pScrn->scrnIndex, X_ERROR,
				"Given depth (%d) is not supported by Impact driver\n",
				pScrn->depth
			);
			return FALSE;
	}
	/* Set bits per RGB for 8bpp */
	if (8 == pScrn->depth)
		pScrn->rgbBits = 8;

	/* Set Default Weight */
	if (pScrn->depth > 8) {
		rgb weights = {8, 8, 8};
		rgb masks = {0xFF, 0xFF00, 0xFF0000};
		if ( !xf86SetWeight(pScrn, weights, masks) )
			return FALSE;
	}

	xf86PrintDepthBpp(pScrn);

	if ( !xf86SetDefaultVisual(pScrn,-1) )
		return FALSE;

	if (pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Given default visual"
		" (%s) is not supported at depth %d\n",
		xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
		return FALSE;
	}

	{	/* Set default Gamma */
		Gamma zeros = {0.0, 0.0, 0.0};
		if ( !xf86SetGamma(pScrn,zeros) )
			return FALSE;
	}

	/* Allocate the ImpactRec driverPrivate */
	if ( !ImpactGetRec(pScrn) )
		return FALSE;

	/* ...and initialize it. */
	pImpact = IMPACTPTR(pScrn);
	pImpact->busID = busID & 0xffff;
	pImpact->isSR  = busID >> 16;
	pImpact->FlushBoxCache = 0;

	if (pImpact->isSR) {
		pImpact->WaitCfifoEmpty = &ImpactSRWaitCfifoEmpty;
		pImpact->WaitDMAOver = &ImpactSRWaitDMAOver;
		pImpact->WaitDMAReady = &ImpactSRWaitDMAReady;
		pImpact->Vc3Get = &ImpactSRVc3Get;
		pImpact->Vc3Set = &ImpactSRVc3Set;
		pImpact->XmapGetModeRegister = &ImpactSRXmapGetModeRegister;
		pImpact->XmapSetModeRegister = &ImpactSRXmapSetModeRegister;
		pImpact->RefreshArea8  = &ImpactSRRefreshArea8;
		pImpact->RefreshArea32 = &ImpactSRRefreshArea32;
		pImpact->base_addr0 = IMPACTSR_BASE_ADDR0;
		pImpact->base_offset = IMPACTSR_BASE_OFFSET;
	} else {
		pImpact->WaitCfifoEmpty = &ImpactI2WaitCfifoEmpty;
		pImpact->WaitDMAOver = &ImpactI2WaitDMAOver;
		pImpact->WaitDMAReady = &ImpactI2WaitDMAReady;
		pImpact->Vc3Get = &ImpactI2Vc3Get;
		pImpact->Vc3Set = &ImpactI2Vc3Set;
		pImpact->XmapGetModeRegister = &ImpactI2XmapGetModeRegister;
		pImpact->XmapSetModeRegister = &ImpactI2XmapSetModeRegister;
		pImpact->RefreshArea8  = &ImpactI2RefreshArea8;
		pImpact->RefreshArea32 = &ImpactI2RefreshArea32;
		pImpact->base_addr0 = IMPACTI2_BASE_ADDR0;
		pImpact->base_offset = IMPACTI2_BASE_OFFSET;
	}

	/* We use a programmable clock */
	pScrn->progClock = TRUE;

	/* Fill in pScrn->options) */
	xf86CollectOptions(pScrn, NULL);
	if ( !(pImpact->Options = xalloc(sizeof(ImpactOptions))) )
		goto out_freerec;

	memcpy(pImpact->Options, ImpactOptions, sizeof(ImpactOptions));
	xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pImpact->Options);

	/* Set fields in ScreenInfoRec && ImpactRec */
	pScrn->videoRam = IMPACT_FIXED_W_SCRN * (pScrn->bitsPerPixel >> 3);

	/* get revisions of REX3, etc. */
	if ( !ImpactMapRegs(pScrn) )
		goto out_freeopt;
		
	ImpactProbeCardInfo(pScrn);
	ImpactUnmapRegs(pScrn);

	from = X_PROBED;
	xf86DrvMsg(
		pScrn->scrnIndex, from,
		"Impact Graphics Revisions: Board: %d, Rex: %c, Cmap: %c, Xmap: %c\n",
		pImpact->board_rev, pImpact->rex_rev,
		pImpact->cmap_rev, pImpact->xmap_rev);

	if ( xf86GetOptValInteger(
			pImpact->Options, OPTION_BITPLANES, &pImpact->bitplanes) )
		from = X_CONFIG;
	xf86DrvMsg(pScrn->scrnIndex, from, "Impact has %d bitplanes\n",
			pImpact->bitplanes);

	if (pScrn->depth > pImpact->bitplanes) {
		xf86DrvMsg(
			pScrn->scrnIndex, X_ERROR,
			"Display depth(%d) > number of bitplanes on Impact board(%d)\n",
			pScrn->depth, pImpact->bitplanes);
		goto out_freeopt;
	}
	if (pImpact->bitplanes != 8 && pImpact->bitplanes != 24) {
		xf86DrvMsg(
			pScrn->scrnIndex, X_ERROR,
			"Number of bitplanes on impact must be either 8 or 24 not %d\n",
			pImpact->bitplanes);
		goto out_freeopt;
	}

	/* Set up clock ranges that are alway ok */

	/* XXX: Use information from VC2 here */
	clockRanges = xnfalloc(sizeof(ClockRange));
	clockRanges->next = 0;
	clockRanges->minClock = 10000;
	clockRanges->maxClock = 300000;
	clockRanges->clockIndex = -1;	/* programmable */
	clockRanges->interlaceAllowed = TRUE;
	clockRanges->doubleScanAllowed = TRUE;

	/* see above note */
	/* There is currently only an 1280x1024 mode */
	i = xf86ValidateModes(
			pScrn, pScrn->monitor->Modes,
			pScrn->display->modes, clockRanges,
			NULL, 256, 2048,
			pScrn->bitsPerPixel, 128, 2048,
			pScrn->display->virtualX,
			pScrn->display->virtualY,
			pScrn->videoRam * 1024,
			LOOKUP_BEST_REFRESH);

	if (-1 == i)
		goto out_freeopt;

	xf86PruneDriverModes(pScrn);
	if (!i || !pScrn->modes) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
		goto out_freeopt;
	}

	/* unnecessary, but do it to get a valid ScrnInfoRec */
	xf86SetCrtcForModes(pScrn, INTERLACE_HALVE_V);

	/* Set the current mode to the first in the list */
	pScrn->currentMode = pScrn->modes;

	/* Print the list of modes being used */
	xf86PrintModes(pScrn);
	xf86SetDpi(pScrn, 0, 0);

	/* Load FB module */
	if ( !(pFbMod = xf86LoadSubModule(pScrn,"fb")) )
		goto out_freeopt;

	xf86LoaderReqSymLists( fbSymbols, NULL);

	/* Load ShadowFB module */
	if ( !xf86LoadSubModule(pScrn,"shadowfb") )
		goto out_freemod;

	xf86LoaderReqSymLists(shadowSymbols, NULL);

	return TRUE;

out_freemod:
	/* xf86UnloadSubModule(pFbMod); */ /* Unload FB module */
out_freeopt:
	xfree(pImpact->Options);
out_freerec:
	ImpactFreeRec(pScrn);
	return FALSE;
}

static Bool
ImpactScreenInit(int index, ScreenPtr pScreen, int argc, char **argv)
{
	ScrnInfoPtr pScrn;
	ImpactPtr pImpact;
	VisualPtr visual;
	BOOL ret;
	int i;

	/* First get a pointer to our private info */
	pScrn = xf86Screens[pScreen->myNum];
	pImpact = IMPACTPTR(pScrn);

	/* map the Impactregs until the server dies */
	if ( !ImpactMapRegs(pScrn) )
		return FALSE;

	/* map the ShadowFB until the server dies */
	if ( !ImpactMapShadowFB(pScrn) ) {
		ImpactUnmapRegs(pScrn);
		return FALSE;
	}
	/* Reset visual list. */
	miClearVisualTypes();

	if ( !miSetVisualTypes(
			pScrn->depth,
			pScrn->depth != 8
				? TrueColorMask
				: miGetDefaultVisualMask(pScrn->depth),
			pScrn->rgbBits, pScrn->defaultVisual) )
		goto out_freerec;

	miSetPixmapDepths();

	pImpact->Bpp = pScrn->bitsPerPixel >> 3;
	/* Setup the stuff for the shadow framebuffer */
	pImpact->ShadowPitch = (pScrn->virtualX * pImpact->Bpp + 3) & ~3L;

	if ( !ImpactModeInit(pScrn,pScrn->currentMode) )
		goto out_freerec;

	ret = fbScreenInit(
				pScreen, pImpact->ShadowPtr,
				pScrn->virtualX, pScrn->virtualY,
				pScrn->xDpi, pScrn->yDpi,
				pScrn->displayWidth,
				pScrn->bitsPerPixel);
	if (!ret)
		goto out_freerec;

	/* we need rgb ordering if bitsPerPixel > 8 */
	if (pScrn->bitsPerPixel > 8)
		for (i = 0, visual = pScreen->visuals;
				i < pScreen->numVisuals; i++, visual++)
			if ((visual->class | DynamicClass) == DirectColor) {
				visual->offsetRed = pScrn->offset.red;
				visual->offsetGreen = pScrn->offset.green;
				visual->offsetBlue = pScrn->offset.blue;
				visual->redMask = pScrn->mask.red;
				visual->greenMask = pScrn->mask.green;
				visual->blueMask = pScrn->mask.blue;
			}

	/* must be after RGB ordering fixed */
	fbPictureInit (pScreen, 0, 0);

	miInitializeBackingStore(pScreen);
	xf86SetBackingStore(pScreen);

	xf86SetBlackWhitePixels(pScreen);

	/* Initialize software cursor */
	if ( !miDCInitialize(pScreen,xf86GetPointerScreenFuncs()) )
		goto out_freerec;

	/* Initialise default colourmap */
	if ( !miCreateDefColormap(pScreen) )
		goto out_freerec;

	/* Install our LoadPalette funciton */
	if ( !xf86HandleColormaps(pScreen, 256, 8, ImpactLoadPalette,
					0, CMAP_RELOAD_ON_MODE_SWITCH) )
		goto out_freerec;

	/* Initialise shadow frame buffer */
	ShadowFBInit(pScreen,
		1 == pImpact->Bpp ? pImpact->RefreshArea8 : pImpact->RefreshArea32);

#ifdef XvExtension
	{
		XF86VideoAdaptorPtr *ptr;
		int n;

		n = xf86XVListGenericAdaptors(pScrn,&ptr);
		if (n)
			xf86XVScreenInit(pScreen, ptr, n);
	}
#endif
	pScreen->SaveScreen = ImpactSaveScreen;
	/* Wrap the current CloseScreen function */
	pImpact->CloseScreen = pScreen->CloseScreen;
	pScreen->CloseScreen = ImpactCloseScreen;

	if (1 == serverGeneration)
		xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);

	return TRUE;

out_freerec:
	ImpactFreeRec(pScrn);
	return FALSE;
}

/* called when switching away from a VT */
static Bool
ImpactEnterVT(int scrnIndex, int flags)
{
	ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
	return ImpactModeInit(pScrn, pScrn->currentMode);
}

/* called when switching to a VT */
static void
ImpactLeaveVT(int scrnIndex, int flags)
{
	ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
	ImpactRestore(pScrn, FALSE);
}

/* called at the end of each server generation */
static Bool
ImpactCloseScreen(int scrnIndex, ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
	ImpactPtr pImpact = IMPACTPTR(pScrn);

	ImpactRestore(pScrn, TRUE);

	/* unmap the Impact's registers from memory */
	ImpactUnmapShadowFB(pScrn);
	ImpactUnmapRegs(pScrn);
	pScrn->vtSema = FALSE;

	if (pScreen->CloseScreen == ImpactCloseScreen)
		pScreen->CloseScreen = pImpact->CloseScreen;
	return (*pScreen->CloseScreen)(scrnIndex, pScreen);
}

/* Blank or unblank the screen */
static Bool
ImpactSaveScreen(ScreenPtr pScreen, int mode)
{
	ScrnInfoPtr pScrn;
	ImpactPtr pImpact;

	pScrn = xf86Screens[pScreen->myNum];
	pImpact = IMPACTPTR(pScrn);

	ImpactVc3BlankScreen(pScrn, !xf86IsUnblank(mode));
	return TRUE;
}


static const OptionInfoRec *
ImpactAvailableOptions(int chipid, int busid)
{
	return ImpactOptions;
}


/* This sets up the actual mode on the Impact */
static Bool
ImpactModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
	int width, height;

	width = mode->HDisplay;
	height = mode->VDisplay;
	if (width != IMPACT_FIXED_W_SCRN || height != IMPACT_FIXED_H_SCRN) {
		xf86DrvMsg(
			pScrn->scrnIndex, X_ERROR,
			"Width = %d and height = %d is not supported by by this driver\n",
			width, height);
		return FALSE;
	}

	pScrn->vtSema = TRUE;
	/* first backup the necessary registers... */
/*
	ImpactBackupVc3(pScrn);
	ImpactBackupRex(pScrn);
	ImpactBackupPalette(pScrn);
	ImpactBackupXmap(pScrn);
*/

	/* ...then  setup the hardware */
/*
	ImpactVc3DisableCursor(pScrn);
	ImpactXmapSet24bpp(pScrn);
*/

	/* blank the framebuffer */

	/* default drawmode */

	return TRUE;
}


/*
 * This will actually restore the saved state
 * (either when switching back to a VT or when the server is going down)
 * Closing is true if the X server is really going down
 */
static void
ImpactRestore(ScrnInfoPtr pScrn, Bool Closing)
{
	/* Restore backed up registers */
/*
	ImpactRestoreVc3(pScrn);
	ImpactRestoreRex(pScrn);
	ImpactRestorePalette(pScrn);
	ImpactRestoreXmap(pScrn);
*/
}


/* Probe for the Impact card ;) */
static unsigned
ImpactHWProbe(struct probed_id probedIDs[], int lim)
{
	FILE* fb;
	int hasImpact = 0;

	probedIDs[0].sr = probedIDs[0].id = 0;
	if (IMPACT_MAX_BOARDS < lim)
		lim = IMPACT_MAX_BOARDS;
	
	if ( (fb = fopen("/proc/fb","r")) ) {
		char line[80];
		while (hasImpact <= lim && fgets(line,sizeof(line),fb)) {
			char *s;
			unsigned i = strtoul(line,&s,10);
			if ( !strncmp(s," Impact",7) ) {
				probedIDs[hasImpact].sr = ('S' == s[7]);
				probedIDs[hasImpact].id = i;
				hasImpact++;
			}
		}
		fclose(fb);
	}
	return hasImpact;
}

/* Probe for Chipset revisions */
static Bool
ImpactProbeCardInfo(ScrnInfoPtr pScrn)
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);

	pImpact->bitplanes = 24;
	pImpact->board_rev = 0;
	pImpact->cmap_rev = '0';
	pImpact->rex_rev = '0';
	pImpact->xmap_rev = '0';

	return TRUE;
}


/*
 * To leave "xc/programs/Xserver/hw/xfree86/os-support/linux/lnx_video.c"
 * untouched for now, we do our own mapping here.
 * mapVidMem() (which is called from xf86MapVidMem(), to do the actual mapping)
 * in lnx_video.c 1) has "/dev/mem" hardcoded and 2) unconditionally opens it
 * without O_SYNC.
 * But we *need* an uncached mapping for pImpactRegs.
 * And using "/dev/fbN" at offset 0 automatically creates the appropriate
 * mapping at the right hardware-address.
 */

/* map ImpactRegs */
static Bool
ImpactMapRegs(ScrnInfoPtr pScrn)
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);

	if (pImpact->pImpactRegs)
		return TRUE;

	if (pImpact->devFD <= 0) {
		char devfb[20];
		sprintf( devfb, "/dev/fb%d", pImpact->busID );
		pImpact->devFD = open(devfb,O_RDWR);
	}

	if (pImpact->devFD < 0)
		ErrorF("ImpactMapRegs: failed to open /dev/fb%d (%s)\n",
			pImpact->busID , strerror(errno));
	else {
		size_t size = pImpact->isSR ? 0x200000:sizeof(ImpactI2Regs);
		pImpact->pImpactRegs =
			mmap((void*)0, size, PROT_READ|PROT_WRITE,
				MAP_SHARED, pImpact->devFD, 0);
		if (MAP_FAILED == pImpact->pImpactRegs) {
			ErrorF("ImpactMapRegs: Could not mmap card registers"
				" (0x%08lx,0x%x) (%s)\n", 0, size, strerror(errno));
			pImpact->pImpactRegs = 0;
		}
	}
	return pImpact->pImpactRegs ? TRUE:FALSE;
}

/* map ImpactShadowFB */
static Bool
ImpactMapShadowFB(ScrnInfoPtr pScrn)
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);

	if (pImpact->ShadowPtr)
		return TRUE;

	if (pImpact->devFD < 0)
		ErrorF("ImpactMapShadowFB: failed to open fb-dev\n");
	else {
		pImpact->ShadowPtr =
			mmap((void*)0, 0x800000, PROT_READ|PROT_WRITE,
				MAP_SHARED, pImpact->devFD, IMPACT_FB_MMAP_OFF(3));
		if (MAP_FAILED == pImpact->ShadowPtr) {
			ErrorF("ImpactMapShadowFB: Could not mmap shadow buffer"
				" (0x%08lx,0x800000) (%s)\n", IMPACT_FB_MMAP_OFF(3), strerror(errno));
			pImpact->ShadowPtr = 0;
		}
	}
	return pImpact->ShadowPtr ? TRUE:FALSE;
}

/* unmap ImpactRegs */
static void
ImpactUnmapRegs(ScrnInfoPtr pScrn)
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);
	size_t size = pImpact->isSR ? 0x200000:sizeof(ImpactI2Regs);

	if (pImpact->pImpactRegs)
		munmap( pImpact->pImpactRegs, size );
	pImpact->pImpactRegs = 0;
}

/* unmap ImpactShadowFB */
static void
ImpactUnmapShadowFB(ScrnInfoPtr pScrn)
{
	ImpactPtr pImpact = IMPACTPTR(pScrn);

	if (pImpact->ShadowPtr)
		munmap( pImpact->ShadowPtr, 0x800000 );
	pImpact->ShadowPtr = 0;
}

/* eof */
