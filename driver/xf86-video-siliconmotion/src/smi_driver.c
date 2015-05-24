/* Header:   //Mercury/Projects/archives/XFree86/4.0/smi_driver.c-arc   1.42   03 Jan 2001 13:52:16   Frido  $ */

/*
Copyright (C) 1994-1999 The XFree86 Project, Inc.  All Rights Reserved.
Copyright (C) 2000 Silicon Motion, Inc.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the names of The XFree86 Project and
Silicon Motion shall not be used in advertising or otherwise to promote the
sale, use or other dealings in this Software without prior written
authorization from The XFree86 Project or Silicon Motion.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86DDC.h"
#include "xf86int10.h"
#include "vbe.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#include "xf86RAC.h"
#endif

#include "smi.h"
#include "smi_501.h"
#include "smilynx.h"
#include "smi_crtc.h"

#include "globals.h"
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif


/*
 * Internals
 */
static Bool SMI_MapMmio(ScrnInfoPtr pScrn);
static Bool SMI_DetectMem(ScrnInfoPtr pScrn);
static void SMI_EnableMmio(ScrnInfoPtr pScrn);
static void SMI_DisableMmio(ScrnInfoPtr pScrn);
static Bool SMI_HWInit(ScrnInfoPtr pScrn);

/*
 * Forward definitions for the functions that make up the driver.
 */

static const OptionInfoRec * SMI_AvailableOptions(int chipid, int busid);
static void SMI_Identify(int flags);
static Bool SMI_Probe(DriverPtr drv, int flags);
static Bool SMI_PreInit(ScrnInfoPtr pScrn, int flags);
static Bool SMI_EnterVT(VT_FUNC_ARGS_DECL);
static void SMI_LeaveVT(VT_FUNC_ARGS_DECL);
static Bool SMI_ScreenInit(SCREEN_INIT_ARGS_DECL);
static void SMI_DisableVideo(ScrnInfoPtr pScrn);
static void SMI_EnableVideo(ScrnInfoPtr pScrn);
static Bool SMI_CloseScreen(CLOSE_SCREEN_ARGS_DECL);
static Bool SMI_SaveScreen(ScreenPtr pScreen, int mode);
static void SMI_FreeScreen(FREE_SCREEN_ARGS_DECL);
static void SMI_ProbeDDC(ScrnInfoPtr pScrn, int index);
static void SMI_DetectPanelSize(ScrnInfoPtr pScrn);
static void SMI_DetectMCLK(ScrnInfoPtr pScrn);

/*
 * xf86VDrvMsgVerb prints up to 14 characters prefix, where prefix has the
 * format "%s(%d): " so, use name "SMI" instead of "Silicon Motion"
 */
#define SILICONMOTION_NAME          "SMI"
#define SILICONMOTION_DRIVER_NAME   "siliconmotion"
#define SILICONMOTION_VERSION_NAME  PACKAGE_VERSION
#define SILICONMOTION_VERSION_MAJOR PACKAGE_VERSION_MAJOR
#define SILICONMOTION_VERSION_MINOR PACKAGE_VERSION_MINOR
#define SILICONMOTION_PATCHLEVEL    PACKAGE_VERSION_PATCHLEVEL
#define SILICONMOTION_DRIVER_VERSION ((SILICONMOTION_VERSION_MAJOR << 24) | \
                                      (SILICONMOTION_VERSION_MINOR << 16) | \
                                      (SILICONMOTION_PATCHLEVEL))

#if SMI_DEBUG
int smi_indent = 1;
#endif

/* for dualhead */
int gSMIEntityIndex = -1;

/*
 * This contains the functions needed by the server after loading the
 * driver module.  It must be supplied, and gets added the driver list by
 * the Module Setup funtion in the dynamic case.  In the static case a
 * reference to this is compiled in, and this requires that the name of
 * this DriverRec be an upper-case version of the driver name.
 */

_X_EXPORT DriverRec SILICONMOTION =
{
    SILICONMOTION_DRIVER_VERSION,
    SILICONMOTION_DRIVER_NAME,
    SMI_Identify,
    SMI_Probe,
    SMI_AvailableOptions,
    NULL,
    0
};

/* Supported chipsets */
static SymTabRec SMIChipsets[] =
{
    { PCI_CHIP_SMI910, "Lynx"    },
    { PCI_CHIP_SMI810, "LynxE"   },
    { PCI_CHIP_SMI820, "Lynx3D"  },
    { PCI_CHIP_SMI710, "LynxEM"  },
    { PCI_CHIP_SMI712, "LynxEM+" },
    { PCI_CHIP_SMI720, "Lynx3DM" },
    { PCI_CHIP_SMI731, "Cougar3DR" },
    { PCI_CHIP_SMI501, "MSOC"	 },
    { -1,             NULL      }
};

static PciChipsets SMIPciChipsets[] =
{
    /* numChipset,	PciID,			Resource */
    { PCI_CHIP_SMI910,	PCI_CHIP_SMI910,	RES_SHARED_VGA },
    { PCI_CHIP_SMI810,	PCI_CHIP_SMI810,	RES_SHARED_VGA },
    { PCI_CHIP_SMI820,	PCI_CHIP_SMI820,	RES_SHARED_VGA },
    { PCI_CHIP_SMI710,	PCI_CHIP_SMI710,	RES_SHARED_VGA },
    { PCI_CHIP_SMI712,	PCI_CHIP_SMI712,	RES_SHARED_VGA },
    { PCI_CHIP_SMI720,	PCI_CHIP_SMI720,	RES_SHARED_VGA },
    { PCI_CHIP_SMI731,	PCI_CHIP_SMI731,	RES_SHARED_VGA },
    { PCI_CHIP_SMI501,	PCI_CHIP_SMI501,	RES_UNDEFINED  },
    { -1,		-1,			RES_UNDEFINED  }
};

typedef enum
{
    OPTION_PCI_BURST,
    OPTION_PCI_RETRY,
    OPTION_NOACCEL,
    OPTION_MCLK,
    OPTION_MXCLK,
    OPTION_SWCURSOR,
    OPTION_HWCURSOR,
    OPTION_VIDEOKEY,
    OPTION_BYTESWAP,
    /* CZ 26.10.2001: interlaced video */
    OPTION_INTERLACED,
    /* end CZ */
    OPTION_USEBIOS,
    OPTION_DUALHEAD,
    OPTION_ACCELMETHOD,
    OPTION_PANEL_SIZE,
    OPTION_USE_FBDEV,
    OPTION_CSCVIDEO,
    NUMBER_OF_OPTIONS
} SMIOpts;

static const OptionInfoRec SMIOptions[] =
{
    { OPTION_PCI_BURST,	     "pci_burst",	  OPTV_BOOLEAN, {0}, TRUE },
    { OPTION_PCI_RETRY,	     "pci_retry",	  OPTV_BOOLEAN, {0}, TRUE },
    { OPTION_NOACCEL,	     "NoAccel",		  OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_MCLK,	     "MCLK",		  OPTV_FREQ,	{0}, FALSE },
    { OPTION_MXCLK,	     "MXCLK",		  OPTV_FREQ,	{0}, FALSE },
    { OPTION_HWCURSOR,	     "HWCursor",	  OPTV_BOOLEAN, {0}, TRUE },
    { OPTION_SWCURSOR,	     "SWCursor",	  OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_VIDEOKEY,	     "VideoKey",	  OPTV_INTEGER, {0}, FALSE },
    { OPTION_BYTESWAP,	     "ByteSwap",	  OPTV_BOOLEAN, {0}, FALSE },
    /* CZ 26.10.2001: interlaced video */
    { OPTION_INTERLACED,     "Interlaced",        OPTV_BOOLEAN, {0}, FALSE },
    /* end CZ */
    { OPTION_USEBIOS,	     "UseBIOS",		  OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_DUALHEAD,	     "Dualhead",	  OPTV_BOOLEAN,	{0}, TRUE },
    { OPTION_ACCELMETHOD,    "AccelMethod",       OPTV_STRING,  {0}, FALSE },
    { OPTION_PANEL_SIZE,     "PanelSize",	  OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_USE_FBDEV,	     "UseFBDev",	  OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CSCVIDEO,	     "CSCVideo",	  OPTV_BOOLEAN, {0}, TRUE },
    { -1,		     NULL,		  OPTV_NONE,	{0}, FALSE }
};

#ifdef XFree86LOADER

static MODULESETUPPROTO(siliconmotionSetup);

static XF86ModuleVersionInfo SMIVersRec =
{
    "siliconmotion",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    SILICONMOTION_VERSION_MAJOR,
    SILICONMOTION_VERSION_MINOR,
    SILICONMOTION_PATCHLEVEL,
    ABI_CLASS_VIDEODRV,
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_VIDEODRV,
    {0, 0, 0, 0}
};

/*
 * This is the module init data for XFree86 modules.
 *
 * Its name has to be the driver name followed by ModuleData.
 */
_X_EXPORT XF86ModuleData siliconmotionModuleData =
{
    &SMIVersRec,
    siliconmotionSetup,
    NULL
};

static pointer
siliconmotionSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    static Bool setupDone = FALSE;

    if (!setupDone) {
	setupDone = TRUE;
	xf86AddDriver(&SILICONMOTION, module, 0);

	/*
	 * The return value must be non-NULL on success even though there
	 * is no TearDownProc.
	 */
	return (pointer) 1;

    } else {
	if (errmaj) {
	    *errmaj = LDR_ONCEONLY;
	}
	return NULL;
    }
}

#endif /* XFree86LOADER */

static Bool
SMI_GetRec(ScrnInfoPtr pScrn)
{
    ENTER();

    /*
     * Allocate an 'Chip'Rec, and hook it into pScrn->driverPrivate.
     * pScrn->driverPrivate is initialised to NULL, so we can check if
     * the allocation has already been done.
     */
    if (pScrn->driverPrivate == NULL) {
	pScrn->driverPrivate = xnfcalloc(sizeof(SMIRec), 1);
    }

    LEAVE(TRUE);
}

static void
SMI_FreeRec(ScrnInfoPtr pScrn)
{
    SMIPtr	pSmi = SMIPTR(pScrn);

    ENTER();

    if (pSmi) {
	free(pSmi->save);
	free(pSmi->mode);
	free(pScrn->driverPrivate);
	pScrn->driverPrivate = NULL;
    }

    LEAVE();
}

static const OptionInfoRec *
SMI_AvailableOptions(int chipid, int busid)
{
    ENTER();

    LEAVE(SMIOptions);
}

static void
SMI_Identify(int flags)
{
    ENTER();

    xf86PrintChipsets(SILICONMOTION_NAME, "driver (version "
		SILICONMOTION_VERSION_NAME ") for Silicon Motion Lynx chipsets",
		SMIChipsets);

    LEAVE();
}

static Bool
SMI_Probe(DriverPtr drv, int flags)
{
    int i;
    GDevPtr *devSections;
    int *usedChips;
    int numDevSections;
    int numUsed;
    Bool foundScreen = FALSE;

    ENTER();

    numDevSections = xf86MatchDevice(SILICONMOTION_DRIVER_NAME, &devSections);

    if (numDevSections <= 0)
	/* There's no matching device section in the config file, so quit now. */
	LEAVE(FALSE);

#ifndef XSERVER_LIBPCIACCESS
    if (xf86GetPciVideoInfo() == NULL)
	LEAVE(FALSE);
#endif

    numUsed = xf86MatchPciInstances(SILICONMOTION_NAME, PCI_SMI_VENDOR_ID,
				    SMIChipsets, SMIPciChipsets, devSections,
				    numDevSections, drv, &usedChips);

    /* Free it since we don't need that list after this */
    free(devSections);
    if (numUsed <= 0)
	LEAVE(FALSE);

    if (flags & PROBE_DETECT)
	foundScreen = TRUE;
    else {
	ScrnInfoPtr	pScrn;
	EntityInfoPtr	pEnt;

	for (i = 0; i < numUsed; i++) {
	    if ((pScrn = xf86ConfigPciEntity(NULL, 0, usedChips[i],
					     SMIPciChipsets, NULL,
					     NULL, NULL, NULL, NULL))) {
		pScrn->driverVersion = SILICONMOTION_DRIVER_VERSION;
		pScrn->driverName    = SILICONMOTION_DRIVER_NAME;
		pScrn->name	     = SILICONMOTION_NAME;
		pScrn->Probe	     = SMI_Probe;
		pScrn->PreInit	     = SMI_PreInit;
		pScrn->ScreenInit    = SMI_ScreenInit;
		pScrn->SwitchMode    = SMI_SwitchMode;
		pScrn->AdjustFrame   = SMI_AdjustFrame;

		if ((pEnt = xf86GetEntityInfo(usedChips[i]))) {
			pScrn->EnterVT   = SMI_EnterVT;
			pScrn->LeaveVT   = SMI_LeaveVT;
		    free(pEnt);
		}
		pScrn->FreeScreen    = SMI_FreeScreen;
		foundScreen	     = TRUE;
	    }
	}
    }
    free(usedChips);

    LEAVE(foundScreen);
}

static Bool
SMI_PreInit(ScrnInfoPtr pScrn, int flags)
{
    EntityInfoPtr pEnt;
    SMIPtr pSmi;
    MessageType from;
    vgaHWPtr hwp;
	
    ENTER();

    /* Ignoring the Type list for now.  It might be needed when multiple cards
     * are supported.
     */
    if (pScrn->numEntities > 1)
	LEAVE(FALSE);

    /* Allocate the SMIRec driverPrivate */
    if (!SMI_GetRec(pScrn))
	LEAVE(FALSE);
    pSmi = SMIPTR(pScrn);

    /* Find the PCI slot for this screen */
    pEnt = xf86GetEntityInfo(pScrn->entityList[0]);

    pSmi->PciInfo = xf86GetPciInfoForEntity(pEnt->index);
    pSmi->Chipset = PCI_DEV_DEVICE_ID(pSmi->PciInfo);

    if (IS_MSOC(pSmi)) {
	pSmi->Save = SMI501_Save;
	pSmi->save = xnfcalloc(sizeof(MSOCRegRec), 1);
	pSmi->mode = xnfcalloc(sizeof(MSOCRegRec), 1);
    }
    else {
	pSmi->Save = SMILynx_Save;
	pSmi->save = xnfcalloc(sizeof(SMIRegRec), 1);
	pSmi->mode = xnfcalloc(sizeof(SMIRegRec), 1);
    }

    if (flags & PROBE_DETECT) {
	if (!IS_MSOC(pSmi))
	    SMI_ProbeDDC(pScrn, xf86GetEntityInfo(pScrn->entityList[0])->index);
	LEAVE(TRUE);
    }

    if (pEnt->location.type != BUS_PCI) {
	free(pEnt);
	SMI_FreeRec(pScrn);
	LEAVE(FALSE);
    }
    pSmi->PciInfo = xf86GetPciInfoForEntity(pEnt->index);

    /* Set pScrn->monitor */
    pScrn->monitor = pScrn->confScreen->monitor;

    if (!IS_MSOC(pSmi)) {
	/* The vgahw module should be loaded here when needed */
	if (!xf86LoadSubModule(pScrn, "vgahw"))
	    LEAVE(FALSE);

	/*
	 * Allocate a vgaHWRec
	 */
	if (!vgaHWGetHWRec(pScrn))
	    LEAVE(FALSE);

	hwp = VGAHWPTR(pScrn);
	vgaHWSetStdFuncs(hwp);
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 12
	pSmi->PIOBase = hwp->PIOOffset;
#else
	pSmi->PIOBase = 0;
#endif

	xf86ErrorFVerb(VERBLEV, "\tSMI_PreInit vgaCRIndex=%x, vgaIOBase=%x, "
		       "MMIOBase=%p\n", hwp->IOBase + VGA_CRTC_INDEX_OFFSET,
		       hwp->IOBase, hwp->MMIOBase);
    }

    /*
     * The first thing we should figure out is the depth, bpp, etc.
     */
    if (!xf86SetDepthBpp(pScrn, 0, 0, 0, Support32bppFb))
	LEAVE(FALSE);

    /* Check that the returned depth is one we support */
    if (pScrn->depth != 8 && pScrn->depth != 16 && pScrn->depth != 24) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Given depth (%d) is not supported by this driver\n",
		   pScrn->depth);
	LEAVE(FALSE);
    }


    if(pScrn->bitsPerPixel != 8 && pScrn->bitsPerPixel != 16 &&
       pScrn->bitsPerPixel != 24 && pScrn->bitsPerPixel != 32){
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Given bpp (%d) is not supported by this driver\n",
		   pScrn->bitsPerPixel);
	LEAVE(FALSE);
    }

    xf86PrintDepthBpp(pScrn);

    pSmi->Bpp = pScrn->bitsPerPixel >> 3;

    /*
     * This must happen after pScrn->display has been set because
     * xf86SetWeight references it.
     */
    if (pScrn->depth > 8) {
	/* The defaults are OK for us */
	rgb zeros = {0, 0, 0};
#if __BYTE_ORDER == __BIG_ENDIAN
	rgb masks = {0xff00,0xff0000,0xff000000};
#else
	rgb masks = {0, 0, 0};
#endif

	if (!xf86SetWeight(pScrn, zeros, masks))
	    LEAVE(FALSE);
    }

    if (!xf86SetDefaultVisual(pScrn, -1))
	LEAVE(FALSE);

    /* We don't currently support DirectColor at > 8bpp */
    if (pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Given default visual (%s) "
		   "is not supported at depth %d\n",
		   xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
	LEAVE(FALSE);
    }

    /* We use a programmable clock */
    pScrn->progClock = TRUE;

    /* Collect all of the relevant option flags (fill in pScrn->options) */
    xf86CollectOptions(pScrn, NULL);

    /* Set the bits per RGB for 8bpp mode */
    if (pScrn->depth == 8){
	pScrn->rgbBits = IS_MSOC(pSmi) ? 8 : 6;
    }else if(pScrn->depth == 16){
	/* Use 8 bit LUT for gamma correction*/
	pScrn->rgbBits = 8;
    }

    /* Process the options */
    if (!(pSmi->Options = malloc(sizeof(SMIOptions))))
	LEAVE(FALSE);

    memcpy(pSmi->Options, SMIOptions, sizeof(SMIOptions));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pSmi->Options);

    /* Enable pci burst by default */
    from = X_DEFAULT;
    pSmi->PCIBurst = TRUE;
    if (xf86GetOptValBool(pSmi->Options, OPTION_PCI_BURST, &pSmi->PCIBurst))
	from = X_CONFIG;
    xf86DrvMsg(pScrn->scrnIndex, from, "PCI Burst %sabled\n",
	       pSmi->PCIBurst ? "en" : "dis");

    /* Pci retry enabled by default if pci burst also enabled */
    from = X_DEFAULT;
    pSmi->PCIRetry = pSmi->PCIBurst ? TRUE : FALSE;
    if (xf86GetOptValBool(pSmi->Options, OPTION_PCI_RETRY, &pSmi->PCIRetry)) {
	from = X_CONFIG;
	if (pSmi->PCIRetry && !pSmi->PCIBurst) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "\"pci_retry\" option requires \"pci_burst\".\n");
	    pSmi->PCIRetry = FALSE;
	}
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "PCI Retry %sabled\n",
	       pSmi->PCIRetry ? "en" : "dis");

    if (xf86ReturnOptValBool(pSmi->Options, OPTION_NOACCEL, FALSE)) {
	pSmi->NoAccel = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Option: NoAccel - Acceleration "
		   "disabled\n");
    } else {
	pSmi->NoAccel = FALSE;
    }

    if (IS_MSOC(pSmi)) {
	from = X_DEFAULT;
	if (xf86GetOptValBool(pSmi->Options, OPTION_USE_FBDEV, &pSmi->UseFBDev))
	    from = X_CONFIG;
	xf86DrvMsg(pScrn->scrnIndex, from, "UseFBDev %s.\n",
		   pSmi->UseFBDev ? "enabled" : "disabled");
    }

    from = X_CONFIG;
    pSmi->HwCursor = TRUE;
    /* SWCursor overrides HWCusor if both specified */
    if (xf86ReturnOptValBool(pSmi->Options, OPTION_SWCURSOR, FALSE))
	pSmi->HwCursor = FALSE;
    else if (!xf86GetOptValBool(pSmi->Options, OPTION_HWCURSOR, &pSmi->HwCursor))
	from = X_DEFAULT;

    xf86DrvMsg(pScrn->scrnIndex, from, "Using %sware Cursor\n",
	       pSmi->HwCursor ? "Hard" : "Soft");

    if (xf86GetOptValInteger(pSmi->Options, OPTION_VIDEOKEY, &pSmi->videoKey)) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Option: Video key set to "
		   "0x%08X\n", pSmi->videoKey);
    } else {
	pSmi->videoKey = (1 << pScrn->offset.red) |
			 (1 << pScrn->offset.green) |
			 (((pScrn->mask.blue >> pScrn->offset.blue) - 1)
			 << pScrn->offset.blue);
    }

    if (xf86ReturnOptValBool(pSmi->Options, OPTION_BYTESWAP, FALSE)) {
	pSmi->ByteSwap = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Option: ByteSwap enabled.\n");
    } else {
	pSmi->ByteSwap = FALSE;
    }

    /* CZ 26.10.2001: interlaced video */
    if (xf86ReturnOptValBool(pSmi->Options, OPTION_INTERLACED, FALSE)) {
	pSmi->interlaced = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Option: Interlaced enabled.\n");
    } else {
	pSmi->interlaced = FALSE;
    }
    /* end CZ */

    if (IS_MSOC(pSmi))
	pSmi->useBIOS = FALSE;
    else if (xf86GetOptValBool(pSmi->Options, OPTION_USEBIOS, &pSmi->useBIOS)) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Option: UseBIOS %s.\n",
		   pSmi->useBIOS ? "enabled" : "disabled");
    }
    else if(pSmi->Chipset == SMI_LYNX3DM){
	/* Default to UseBIOS disabled. */
	pSmi->useBIOS = FALSE;
    }
    else {
	/* Default to UseBIOS enabled. */
	pSmi->useBIOS = TRUE;
    }

    if (pSmi->useBIOS) {
	if (xf86LoadSubModule(pScrn,"int10")) {
	    pSmi->pInt10 = xf86InitInt10(pEnt->index);
	}

	if (pSmi->pInt10 && xf86LoadSubModule(pScrn, "vbe")) {
	    pSmi->pVbe = VBEInit(pSmi->pInt10, pEnt->index);
	}

	if(!pSmi->pVbe){
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "VBE initialization failed: falling back to UseBIOS disabled.\n");
	    pSmi->useBIOS = FALSE;
	}
    }

#ifndef XSERVER_LIBPCIACCESS
    xf86RegisterResources(pEnt->index, NULL, ResExclusive);
#endif
    /*
     * Set the Chipset and ChipRev, allowing config file entries to
     * override.
     */
    if (pEnt->device->chipset && *pEnt->device->chipset) {
	pScrn->chipset = pEnt->device->chipset;
	pSmi->Chipset = xf86StringToToken(SMIChipsets, pScrn->chipset);
	from = X_CONFIG;
    }
    else if (pEnt->device->chipID >= 0) {
	pSmi->Chipset = pEnt->device->chipID;
	pScrn->chipset = (char *) xf86TokenToString(SMIChipsets, pSmi->Chipset);
	from = X_CONFIG;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipID override: 0x%04X\n",
		   pSmi->Chipset);
    }
    else {
	from = X_PROBED;
	pSmi->Chipset = PCI_DEV_DEVICE_ID(pSmi->PciInfo);
	pScrn->chipset = (char *) xf86TokenToString(SMIChipsets, pSmi->Chipset);
    }

    if (pEnt->device->chipRev >= 0) {
	pSmi->ChipRev = pEnt->device->chipRev;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipRev override: %d\n",
		   pSmi->ChipRev);
    }
    else
        pSmi->ChipRev = PCI_DEV_REVISION(pSmi->PciInfo);
    free(pEnt);

    /*
     * This shouldn't happen because such problems should be caught in
     * SMI_Probe(), but check it just in case.
     */
    if (pScrn->chipset == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "ChipID 0x%04X is not "
				"recognised\n", pSmi->Chipset);
	LEAVE(FALSE);
    }

    if (pSmi->Chipset < 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Chipset \"%s\" is not "
		   "recognised\n", pScrn->chipset);
	LEAVE(FALSE);
    }

    xf86DrvMsg(pScrn->scrnIndex, from, "Chipset: \"%s\"\n", pScrn->chipset);

#ifndef XSERVER_LIBPCIACCESS
    pSmi->PciTag = pciTag(pSmi->PciInfo->bus, pSmi->PciInfo->device,
		   	  pSmi->PciInfo->func);
#endif

    from = X_DEFAULT;
    if(pSmi->Chipset == SMI_LYNX3DM &&
       pScrn->bitsPerPixel == 16)
	pSmi->Dualhead = TRUE;
    else
	pSmi->Dualhead = FALSE;

    if (xf86GetOptValBool(pSmi->Options, OPTION_DUALHEAD, &pSmi->Dualhead))
	from = X_CONFIG;

    if (IS_MSOC(pSmi)) {
	pSmi->lcd = TRUE;
	if (pSmi->Dualhead && pSmi->UseFBDev) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Dual head disabled in fbdev mode\n");
	    pSmi->Dualhead = FALSE;
	}
	/* FIXME Randr cursor code only works properly when argb cursors
	 * are also supported.
	 * FIXME This probably is a randr cursor bug, and since access to
	 * hw/xfree86/ramdac/xf86CursorPriv.h:xf86CursorScreenRec.SWCursor
	 * field is not available, one cannot easily workaround the problem,
	 * so, just disable it...
	 * TODO Check with a X Server newer then 1.4.0.90 (that is being
	 * used in the 502 OEM image).
	 * */
	if (pSmi->Dualhead && pSmi->HwCursor) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "HW Cursor disabled in dual head mode\n");
	    pSmi->HwCursor = FALSE;
	}
    }
    else if (SMI_LYNXM_SERIES(pSmi->Chipset)) {
	/* tweak options for dualhead */
	if (pSmi->Dualhead) {
	    pSmi->useBIOS = FALSE;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "UseBIOS disabled in dualhead mode\n");
	    pSmi->HwCursor = FALSE;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "No hardware cursor in dualhead mode\n");
	    if (pScrn->bitsPerPixel != 16) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Dualhead only supported at "
			   "depth 16\n");
		LEAVE(FALSE);
	    }
	}
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Dual head %sabled\n",
	       pSmi->Dualhead ? "en" : "dis");

    if (!pSmi->NoAccel) {
	char *strptr;

	from = X_DEFAULT;
	if ((strptr = (char *)xf86GetOptValString(pSmi->Options,
						  OPTION_ACCELMETHOD))) {
	    if (!xf86NameCmp(strptr,"XAA")) {
		from = X_CONFIG;
		pSmi->useEXA = FALSE;
	    } else if(!xf86NameCmp(strptr,"EXA")) {
		from = X_CONFIG;
		pSmi->useEXA = TRUE;
	    }
	}

	xf86DrvMsg(pScrn->scrnIndex, from, "Using %s acceleration architecture\n",
		pSmi->useEXA ? "EXA" : "XAA");
    }

    if (IS_MSOC(pSmi)) {
	pSmi->CSCVideo = !pSmi->useEXA || !pSmi->Dualhead;
	from = X_DEFAULT;
	if (xf86GetOptValBool(pSmi->Options, OPTION_CSCVIDEO, &pSmi->CSCVideo)) {
	    from = X_CONFIG;

	    /* FIXME */
	    if (pSmi->CSCVideo && pSmi->useEXA && pSmi->Dualhead) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "CSCVideo requires XAA or EXA in single head mode.\n");
		pSmi->CSCVideo = FALSE;
	    }
	}

	xf86DrvMsg(pScrn->scrnIndex, from, "CSC Video %sabled\n",
		   pSmi->CSCVideo ? "en" : "dis");
    }

    SMI_MapMmio(pScrn);
    SMI_DetectMem(pScrn);
    SMI_MapMem(pScrn);
    SMI_DisableVideo(pScrn);

    /* detect the panel size */
    SMI_DetectPanelSize(pScrn);

    if(!IS_MSOC(pSmi)){
	if (xf86LoadSubModule(pScrn, "i2c")) {
	    SMI_I2CInit(pScrn);
	}
	xf86LoadSubModule(pScrn, "ddc");
    }

    /*
     * If the driver can do gamma correction, it should call xf86SetGamma()
     */
    {
	Gamma zeros = { 0.0, 0.0, 0.0 };

	if (!xf86SetGamma(pScrn, zeros)) {
	    SMI_EnableVideo(pScrn);
	    SMI_UnmapMem(pScrn);
	    LEAVE(FALSE);
	}
    }

    SMI_DetectMCLK(pScrn);

    /*
     * Setup the ClockRanges, which describe what clock ranges are available,
     * and what sort of modes they can be used for.
     */
    pSmi->clockRange.next = NULL;
    pSmi->clockRange.minClock = 20000;

    if (SMI_LYNXM_SERIES(pSmi->Chipset) ||
	IS_MSOC(pSmi))
	pSmi->clockRange.maxClock = 200000;
    else
        pSmi->clockRange.maxClock = 135000;

    pSmi->clockRange.clockIndex = -1;
    pSmi->clockRange.interlaceAllowed = FALSE;
    pSmi->clockRange.doubleScanAllowed = FALSE;

    if(!SMI_CrtcPreInit(pScrn))
	LEAVE(FALSE);

    if(!SMI_OutputPreInit(pScrn))
	LEAVE(FALSE);

    /* Only allow growing the screen dimensions if EXA is being used */
    if (!xf86InitialConfiguration (pScrn, !pSmi->NoAccel && pSmi->useEXA)){
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");

	SMI_EnableVideo(pScrn);
	SMI_UnmapMem(pScrn);
	LEAVE(FALSE);
    }

    /* Ensure that the framebuffer size just set fits in video memory. */
    {
	int aligned_pitch = (pScrn->virtualX*pSmi->Bpp + 15) & ~15;

	if(aligned_pitch * pScrn->virtualY > pSmi->FBReserved){
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Not enough video memory "
		       "for the configured screen size (%dx%d) and color depth.\n",
                       pScrn->virtualX, pScrn->virtualY);

	    SMI_EnableVideo(pScrn);
	    SMI_UnmapMem(pScrn);
	    LEAVE(FALSE);
	}
    }


    SMI_EnableVideo(pScrn);
    SMI_UnmapMem(pScrn);

    if(pSmi->pVbe){
       vbeFree(pSmi->pVbe);
       pSmi->pVbe = NULL;
    }
    if(pSmi->pInt10){
       xf86FreeInt10(pSmi->pInt10);
       pSmi->pInt10 = NULL;
    }

    /* Set display resolution */
    xf86SetDpi(pScrn, 0, 0);

    if (xf86LoadSubModule(pScrn, "fb") == NULL) {
	SMI_FreeRec(pScrn);
	LEAVE(FALSE);
    }

    /* Load XAA or EXA if needed */
    if (!pSmi->NoAccel) {
	if (!pSmi->useEXA) {
	    if (!xf86LoadSubModule(pScrn, "xaa")) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "No acceleration\n");
		pSmi->NoAccel = 1;
	    }
	} else {
	    XF86ModReqInfo req;
	    int errmaj, errmin;

	    memset(&req, 0, sizeof(XF86ModReqInfo));
	    req.majorversion = 2;
	    req.minorversion = 1;
			
	    if (!LoadSubModule(pScrn->module, "exa", NULL, NULL, NULL,
				&req, &errmaj, &errmin)) {
		LoaderErrorMsg(NULL, "exa", errmaj, errmin);
		SMI_FreeRec(pScrn);
		LEAVE(FALSE);
	    }
	}
    }

    /* Load ramdac if needed */
    if (pSmi->HwCursor) {
	if (!xf86LoadSubModule(pScrn, "ramdac")) {
	    SMI_FreeRec(pScrn);
	    LEAVE(FALSE);
	}
    }

    LEAVE(TRUE);
}

/*
 * This is called when VT switching back to the X server.  Its job is to
 * reinitialise the video mode. We may wish to unmap video/MMIO memory too.
 */

static Bool
SMI_EnterVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    /* Enable MMIO and map memory */
    SMI_MapMem(pScrn);

    pSmi->Save(pScrn);

    /* FBBase may have changed after remapping the memory */
    pScrn->pScreen->ModifyPixmapHeader(pScrn->pScreen->GetScreenPixmap(pScrn->pScreen),
				       -1,-1,-1,-1,-1, pSmi->FBBase + pSmi->FBOffset);
#if (XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1, 9, 99, 1, 0))
    pScrn->pixmapPrivate.ptr=pSmi->FBBase + pSmi->FBOffset;
#endif

    if(pSmi->useEXA)
       pSmi->EXADriverPtr->memoryBase=pSmi->FBBase;

    /* Do the CRTC independent initialization */
    if(!SMI_HWInit(pScrn))
	LEAVE(FALSE);

    /* Initialize the chosen modes */
    if (!xf86SetDesiredModes(pScrn))
	LEAVE(FALSE);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		   "Done writing mode.  Register dump:\n");
    SMI_PrintRegs(pScrn);

    /* Reset the grapics engine */
    if (!pSmi->NoAccel)
	SMI_EngineReset(pScrn);

    LEAVE(TRUE);
}

/*
 * This is called when VT switching away from the X server.  Its job is to
 * restore the previous (text) mode. We may wish to remap video/MMIO memory
 * too.
 */

static void
SMI_LeaveVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    SMIPtr	pSmi = SMIPTR(pScrn);

    ENTER();

    SMI_AccelSync(pScrn);

    /* Ensure that the rotation BlockHandler is unwrapped, and the shadow
       pixmaps are deallocated, as the video memory is going to be
       unmapped.  */
    xf86RotateCloseScreen(pScrn->pScreen);

    /* Pixmaps that by chance get allocated near the former aperture
       address shouldn't be considered offscreen. */
    if(pSmi->useEXA)
       pSmi->EXADriverPtr->memoryBase=NULL;

    /* Clear frame buffer */
    memset(pSmi->FBBase, 0, pSmi->videoRAMBytes);

    if (!IS_MSOC(pSmi)) {
	vgaHWPtr	hwp = VGAHWPTR(pScrn);

	SMILynx_WriteMode(pScrn, &hwp->SavedReg, pSmi->save);
    }
    else
	SMI501_WriteMode(pScrn, pSmi->save);

    SMI_UnmapMem(pScrn);

    LEAVE();
}

static void
SMI_DetectPanelSize(ScrnInfoPtr pScrn)
{
    const char	*s;
    int		 width, height;
    SMIPtr	 pSmi = SMIPTR(pScrn);

    pSmi->lcdWidth  = 0;
    pSmi->lcdHeight = 0;
    if ((s = xf86GetOptValString(pSmi->Options, OPTION_PANEL_SIZE)) != NULL) {
	if (sscanf(s, "%dx%d", &width, &height) != 2)
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Invalid PanelSize option: %s\n", s);
	else {
	    pSmi->lcdWidth  = width;
	    pSmi->lcdHeight = height;
	}
    }

    if (pSmi->lcdWidth == 0 || pSmi->lcdHeight == 0) {
	/* panel size detection ... requires BIOS call on 730 hardware */
	if (pSmi->Chipset == SMI_COUGAR3DR) {
	    if (pSmi->pInt10 != NULL) {
		pSmi->pInt10->num = 0x10;
		pSmi->pInt10->ax  = 0x5F00;
		pSmi->pInt10->bx  = 0;
		pSmi->pInt10->cx  = 0;
		pSmi->pInt10->dx  = 0;
		xf86ExecX86int10(pSmi->pInt10);
		if (pSmi->pInt10->ax == 0x005F) {
		    switch (pSmi->pInt10->cx & 0x0F) {
			case PANEL_640x480:
			    pSmi->lcdWidth  = 640;
			    pSmi->lcdHeight = 480;
			    break;
			case PANEL_800x600:
			    pSmi->lcdWidth  = 800;
			    pSmi->lcdHeight = 600;
			    break;
			case PANEL_1024x768:
			    pSmi->lcdWidth  = 1024;
			    pSmi->lcdHeight = 768;
			    break;
			case PANEL_1280x1024:
			    pSmi->lcdWidth  = 1280;
			    pSmi->lcdHeight = 1024;
			    break;
			case PANEL_1600x1200:
			    pSmi->lcdWidth  = 1600;
			    pSmi->lcdHeight = 1200;
			    break;
			case PANEL_1400x1050:
			    pSmi->lcdWidth  = 1400;
			    pSmi->lcdHeight = 1050;
			    break;
		    }

		    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			       "Detected panel size via BIOS: %d x %d\n",
			       pSmi->lcdWidth, pSmi->lcdHeight);
		}
		else
		    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			       "BIOS error during 730 panel detection!\n");
	    }
	    else  {
		/* int10 support isn't setup on the second call to this function,
		  o if this is the second call, don't do detection again */
		if (pSmi->lcd == 0)
		    /* If we get here, int10 support is not loaded or not working */ 
		    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			       "No BIOS support for 730 panel detection!\n");
	    }

	    /* Set this to indicate that we've done the detection */
	    pSmi->lcd = 1;
	}
	else if (IS_MSOC(pSmi)) {
	    pSmi->lcdWidth  = (READ_SCR(pSmi, PANEL_WWIDTH)  >> 16) & 2047;
	    pSmi->lcdHeight = (READ_SCR(pSmi, PANEL_WHEIGHT) >> 16) & 2047;
	}
	else {
	    /* panel size detection for hardware other than 730 */
	    pSmi->lcd = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA,
				     0x31) & 0x01;

	    if (VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA,
			     0x30) & 0x01) {
		pSmi->lcd <<= 1;
	    }
	    switch (VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA,
				 0x30) & 0x0C) {
		case 0x00:
		    pSmi->lcdWidth  = 640;
		    pSmi->lcdHeight = 480;
		    break;
		case 0x04:
		    pSmi->lcdWidth  = 800;
		    pSmi->lcdHeight = 600;
		    break;
		case 0x08:
		    if (VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA,
				     0x74) & 0x02) {
			pSmi->lcdWidth  = 1024;
			pSmi->lcdHeight = 600;
		    }
		    else {
			pSmi->lcdWidth  = 1024;
			pSmi->lcdHeight = 768;
		    }
		    break;
		case 0x0C:
		    pSmi->lcdWidth  = 1280;
		    pSmi->lcdHeight = 1024;
		    break;
	    }
	}
    }

    if (!pSmi->lcdWidth && (pSmi->lcdWidth = pScrn->virtualX) == 0)
	pSmi->lcdWidth = 1024;
    if (!pSmi->lcdHeight && (pSmi->lcdHeight = pScrn->virtualY) == 0)
	pSmi->lcdHeight = 768;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%s Panel Size = %dx%d\n",
	       (pSmi->lcd == 0) ? "OFF" : (pSmi->lcd == 1) ? "TFT" : "DSTN",
	       pSmi->lcdWidth, pSmi->lcdHeight);

}

static void
SMI_DetectMCLK(ScrnInfoPtr pScrn)
{
    double		real;
    MSOCClockRec	clock;
    int			mclk, mxclk;
    SMIPtr		pSmi = SMIPTR(pScrn);

    /* MCLK defaults */
    if (pSmi->Chipset == SMI_LYNXEMplus){
	/* The SM712 can be safely clocked up to 157MHz, according to
	   Silicon Motion engineers. */
	pSmi->MCLK = 157000;

    } else if (IS_MSOC(pSmi)) {
       /* Set some sane defaults for the clock settings if we are on a
          SM502 and it's likely to be uninitialized. */

       if (!xf86IsPrimaryPci(pSmi->PciInfo) &&
           (READ_SCR(pSmi, DEVICE_ID) & 0xFF) >= 0xC0) {
          pSmi->MCLK = 112000;
          pSmi->MXCLK = 144000;
       }

    } else {
        pSmi->MCLK = 0;
        pSmi->MXCLK = 0;
    }

    /* MCLK from user settings */
    if (xf86GetOptValFreq(pSmi->Options, OPTION_MCLK, OPTUNITS_MHZ, &real)) {
	if (IS_MSOC(pSmi) || (int)real <= 120) {
	    pSmi->MCLK = (int)(real * 1000.0);
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Memory Clock %1.3f MHz larger than limit of 120 MHz\n",
		       real);
	}
    }
    mclk = pSmi->MCLK;

    if (IS_MSOC(pSmi)) {
	clock.value = READ_SCR(pSmi, CURRENT_CLOCK);
	if (xf86GetOptValFreq(pSmi->Options, OPTION_MXCLK,
			      OPTUNITS_MHZ, &real))
	    pSmi->MXCLK = (int)(real * 1000.0);
    }

    /* Already programmed MCLK */
    if (pSmi->MCLK == 0) {
	if (IS_MSOC(pSmi))
	    mclk = ((clock.f.m_select ? 336 : 288) /
		    ((clock.f.m_divider ? 3 : 1) <<
		     (unsigned)clock.f.m_shift)) * 1000;
	else {
	    unsigned char	shift, m, n;

	    m = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6A);
	    n = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x6B);
	    switch (n >> 6) {
		case 1:
		    shift = 4;
		    break;
		case 2:
		    shift = 2;
		    break;
		default:
		    shift = 1;
		    break;
	    }
	    n &= 0x3F;
	    mclk = ((1431818 * m) / n / shift + 50) / 100;
	}
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "MCLK = %1.3f\n", mclk / 1000.0);
    if (IS_MSOC(pSmi)) {
	if (pSmi->MXCLK == 0) {
	    mxclk = ((clock.f.m1_select ? 336 : 288) /
		     ((clock.f.m1_divider ? 3 : 1) <<
		      (unsigned)clock.f.m1_shift)) * 1000;
	}
	else
	    mxclk = pSmi->MXCLK;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "MXCLK = %1.3f\n", mxclk / 1000.0);
    }
}

static Bool
SMI_MapMmio(ScrnInfoPtr pScrn)
{
    SMIPtr	pSmi = SMIPTR(pScrn);
    CARD32	memBase;

    SMI_EnableMmio(pScrn);

    switch (pSmi->Chipset) {
	case SMI_COUGAR3DR:
	    memBase = PCI_REGION_BASE(pSmi->PciInfo, 1, REGION_MEM);
	    pSmi->MapSize = 0x200000;
	    break;
	case SMI_LYNX3D:
	    memBase = PCI_REGION_BASE(pSmi->PciInfo, 0, REGION_MEM) + 0x680000;
	    pSmi->MapSize = 0x180000;
	    break;
	case SMI_LYNXEM:
	case SMI_LYNXEMplus:
	    memBase = PCI_REGION_BASE(pSmi->PciInfo, 0, REGION_MEM) + 0x400000;
	    pSmi->MapSize = 0x400000;
	    break;
	case SMI_LYNX3DM:
	    memBase = PCI_REGION_BASE(pSmi->PciInfo, 0, REGION_MEM);
	    pSmi->MapSize = 0x200000;
	    break;
	case SMI_MSOC:
	    memBase = PCI_REGION_BASE(pSmi->PciInfo, 1, REGION_MEM);
	    pSmi->MapSize = 0x200000;
	    break;
	default:
	    memBase = PCI_REGION_BASE(pSmi->PciInfo, 0, REGION_MEM) + 0x400000;
	    pSmi->MapSize = 0x10000;
	    break;
    }

#ifndef XSERVER_LIBPCIACCESS
    pSmi->MapBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO, pSmi->PciTag,
				  memBase, pSmi->MapSize);
#else
    {
	void	**result = (void**)&pSmi->MapBase;
	int	  err = pci_device_map_range(pSmi->PciInfo,
					     memBase,
					     pSmi->MapSize,
					     PCI_DEV_MAP_FLAG_WRITABLE,
					     result);

	if (err)
	    return (FALSE);
    }
#endif

    if (pSmi->MapBase == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Internal error: could not map "
		   "MMIO registers.\n");
	return (FALSE);
    }

    switch (pSmi->Chipset) {
	case SMI_COUGAR3DR:
	    pSmi->DPRBase = pSmi->MapBase + 0x000000;
	    pSmi->VPRBase = pSmi->MapBase + 0x000800;
	    pSmi->CPRBase = pSmi->MapBase + 0x001000;
	    pSmi->FPRBase = pSmi->MapBase + 0x005800;
	    pSmi->IOBase  = pSmi->MapBase + 0x0C0000;
	    pSmi->DataPortBase = pSmi->MapBase + 0x100000;
	    pSmi->DataPortSize = 0x100000;
	    break;
	case SMI_LYNX3D:
	    pSmi->DPRBase = pSmi->MapBase + 0x000000;
	    pSmi->VPRBase = pSmi->MapBase + 0x000800;
	    pSmi->CPRBase = pSmi->MapBase + 0x001000;
	    pSmi->IOBase  = pSmi->MapBase + 0x040000;
	    pSmi->DataPortBase = pSmi->MapBase + 0x080000;
	    pSmi->DataPortSize = 0x100000;
	    break;
	case SMI_LYNXEM:
	case SMI_LYNXEMplus:
	    pSmi->DPRBase = pSmi->MapBase + 0x008000;
	    pSmi->VPRBase = pSmi->MapBase + 0x00C000;
	    pSmi->CPRBase = pSmi->MapBase + 0x00E000;
	    pSmi->IOBase  = pSmi->MapBase + 0x300000;
	    pSmi->DataPortBase = pSmi->MapBase /*+ 0x100000*/;
	    pSmi->DataPortSize = 0x8000 /*0x200000*/;
	    break;
	case SMI_LYNX3DM:
	    pSmi->DPRBase = pSmi->MapBase + 0x000000;
	    pSmi->VPRBase = pSmi->MapBase + 0x000800;
	    pSmi->CPRBase = pSmi->MapBase + 0x001000;
	    pSmi->IOBase  = pSmi->MapBase + 0x0C0000;
	    pSmi->DataPortBase = pSmi->MapBase + 0x100000;
	    pSmi->DataPortSize = 0x100000;
	    break;
	case SMI_MSOC:
	    pSmi->DPRBase = pSmi->MapBase + 0x100000;
	    pSmi->VPRBase = pSmi->MapBase + 0x000000;
	    pSmi->CPRBase = pSmi->MapBase + 0x090000;
	    pSmi->DCRBase = pSmi->MapBase + 0x080000;
	    pSmi->SCRBase = pSmi->MapBase + 0x000000;
	    pSmi->IOBase = 0;
	    pSmi->DataPortBase = pSmi->MapBase + 0x110000;
	    pSmi->DataPortSize = 0x10000;
	    break;
	default:
	    pSmi->DPRBase = pSmi->MapBase + 0x8000;
	    pSmi->VPRBase = pSmi->MapBase + 0xC000;
	    pSmi->CPRBase = pSmi->MapBase + 0xE000;
	    pSmi->IOBase  = NULL;
	    pSmi->DataPortBase = pSmi->MapBase;
	    pSmi->DataPortSize = 0x8000;
	    break;
    }

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		   "Physical MMIO at 0x%08lX\n", (unsigned long)memBase);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		   "Logical MMIO at %p - %p\n", pSmi->MapBase,
		   pSmi->MapBase + pSmi->MapSize - 1);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		   "DPR=%p, VPR=%p, IOBase=%p\n",
		   pSmi->DPRBase, pSmi->VPRBase, pSmi->IOBase);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		   "DataPort=%p - %p\n", pSmi->DataPortBase,
		   pSmi->DataPortBase + pSmi->DataPortSize - 1);

    return (TRUE);
}

/* HACK - In some cases the BIOS hasn't filled in the "scratchpad
   registers" (SR71) with the right amount of memory installed (e.g. MIPS
   platform). Probe it manually. */
static unsigned long
SMI_ProbeMem(ScrnInfoPtr pScrn, unsigned long mem_skip, unsigned long mem_max)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    unsigned long mem_probe = 1024*1024;
    unsigned long aperture_base;
    void* mem;

    ENTER();

    aperture_base = PCI_REGION_BASE(pSmi->PciInfo, 0, REGION_MEM) + mem_skip;
    mem_max = min(mem_max , PCI_REGION_SIZE(pSmi->PciInfo, 0) - mem_skip);

#ifndef XSERVER_LIBPCIACCESS
    mem = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO, pSmi->PciTag,
			aperture_base, mem_max);

    if(!mem)
	LEAVE(0);
#else
    if(pci_device_map_range(pSmi->PciInfo, aperture_base, mem_max,
			    PCI_DEV_MAP_FLAG_WRITABLE, &mem))
	LEAVE(0);
#endif

    while(mem_probe <= mem_max){
	MMIO_OUT32(mem, mem_probe-4, 0x55555555);
	if(MMIO_IN32(mem, mem_probe-4) != 0x55555555)
	    break;

	MMIO_OUT32(mem, mem_probe-4, 0xAAAAAAAA);
	if(MMIO_IN32(mem, mem_probe-4) != 0xAAAAAAAA)
	    break;

	mem_probe <<= 1;
    }

#ifndef XSERVER_LIBPCIACCESS
    xf86UnMapVidMem(pScrn->scrnIndex, mem, mem_max);
#else
    pci_device_unmap_range(pSmi->PciInfo, mem, mem_max);
#endif

    LEAVE(mem_probe >> 1);
}

static Bool
SMI_DetectMem(ScrnInfoPtr pScrn)
{
    SMIPtr	pSmi = SMIPTR(pScrn);
    MessageType from;

    if ((pScrn->videoRam = pScrn->confScreen->device->videoRam)){
	pSmi->videoRAMKBytes = pScrn->videoRam;
	from = X_CONFIG;
    }
    else {
	unsigned char	 config;
	static int	 lynx3d_table[4]  = {  0, 2, 4, 6 };
	static int	 lynx3dm_table[4] = { 16, 2, 4, 8 };
	static int	 msoc_table[8]    = {  4, 8, 16, 32, 64, 2, 0, 0 };
	static int	 default_table[4] = {  1, 2, 4, 0 };

	if (IS_MSOC(pSmi)) {
	    config = (READ_SCR(pSmi, DRAM_CTL) >> 13) & 7;
	    pSmi->videoRAMKBytes = msoc_table[config] * 1024 -
		SHARED_USB_DMA_BUFFER_SIZE;
	}
	else {
	    config = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x71);
	    switch (pSmi->Chipset) {
		case SMI_LYNX3D:
		    pSmi->videoRAMKBytes = lynx3d_table[config >> 6] * 1024 +
			512;
		    break;
		case SMI_LYNXEMplus:
		    pSmi->videoRAMKBytes = SMI_ProbeMem(pScrn, 0, 0x400000) / 1024;
		    break;
		case SMI_LYNX3DM:
		    pSmi->videoRAMKBytes = lynx3dm_table[config >> 6] * 1024;
		    break;
		case SMI_COUGAR3DR:
		    /* DANGER - Cougar3DR BIOS is broken - hardcode video ram
		     * size per instructions from Silicon Motion engineers */
		    pSmi->videoRAMKBytes = 16 * 1024;
		    break;
		default:
		    pSmi->videoRAMKBytes = default_table[config >> 6] * 1024;
		    break;
	    }
	}
	from = X_PROBED;
    }

    pSmi->videoRAMBytes = pSmi->videoRAMKBytes * 1024;
    pScrn->videoRam     = pSmi->videoRAMKBytes;
    xf86DrvMsg(pScrn->scrnIndex, from,
	       "videoram: %dkB\n", pSmi->videoRAMKBytes);

    return (TRUE);
}

Bool
SMI_MapMem(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    vgaHWPtr hwp;

    ENTER();

    if (pSmi->MapBase == NULL && SMI_MapMmio(pScrn) == FALSE)
	LEAVE(FALSE);

    pScrn->memPhysBase = PCI_REGION_BASE(pSmi->PciInfo, 0, REGION_MEM);

    if (pSmi->Chipset == SMI_LYNX3DM)
	pSmi->fbMapOffset = 0x200000;
    else
	pSmi->fbMapOffset = 0x0;

#ifndef XSERVER_LIBPCIACCESS
    pSmi->FBBase = xf86MapPciMem(pScrn->scrnIndex,
				 VIDMEM_FRAMEBUFFER,
				 pSmi->PciTag,
				 pScrn->memPhysBase + pSmi->fbMapOffset,
				 pSmi->videoRAMBytes);
#else
    {
	void	**result = (void**)&pSmi->FBBase;
	int	  err = pci_device_map_range(pSmi->PciInfo,
					     pScrn->memPhysBase +
					     pSmi->fbMapOffset,
					     pSmi->videoRAMBytes,
					     PCI_DEV_MAP_FLAG_WRITABLE |
					     PCI_DEV_MAP_FLAG_WRITE_COMBINE,
					     result);

	if (err)
	    LEAVE(FALSE);
    }
#endif

    if (pSmi->FBBase == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Internal error: could not map framebuffer.\n");
	LEAVE(FALSE);
    }

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		   "Physical frame buffer at 0x%08lX offset: 0x%08lX\n",
		   pScrn->memPhysBase, (unsigned long)pSmi->fbMapOffset);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		   "Logical frame buffer at %p - %p\n", pSmi->FBBase,
		   pSmi->FBBase + pSmi->videoRAMBytes - 1);

    if (IS_MSOC(pSmi)) {
	/* Reserve space for panel cursr, and crt if in dual head mode */
#if SMI_CURSOR_ALPHA_PLANE
	pSmi->FBReserved = pSmi->FBCursorOffset = pSmi->videoRAMBytes -
	    (pSmi->Dualhead ? SMI501_CURSOR_SIZE << 1 : SMI501_ARGB_CURSOR_SIZE);
#else
	pSmi->FBReserved = pSmi->FBCursorOffset = pSmi->videoRAMBytes -
	    (pSmi->Dualhead ? SMI501_CURSOR_SIZE << 1 : SMI501_CURSOR_SIZE);

# if SMI501_CLI_DEBUG
	if (pSmi->useEXA) {
	    pSmi->batch_active = FALSE;
	    pSmi->batch_length = 4096;
	    pSmi->FBReserved -= pSmi->batch_length << 3;
	    pSmi->batch_offset = pSmi->FBReserved;
	    pSmi->batch_handle = (int64_t *)(pSmi->FBBase + pSmi->batch_offset);
	    pSmi->batch_finish = -1;
	    pSmi->batch_index = 0;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Using command list interpreter debug code\n");
	}
# endif

#endif
    }
    else {
	/* Set up offset to hwcursor memory area, at the end of
	 * the frame buffer.
	 */
	pSmi->FBCursorOffset = pSmi->videoRAMBytes - SMILYNX_CURSOR_SIZE;
	/* set up the fifo reserved space */
	if (VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x30) & 0x01)/* #1074 */ {
	    CARD32 fifoOffset = 0;
	    fifoOffset |= VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA,
				       0x46) << 3;
	    fifoOffset |= VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA,
				       0x47) << 11;
	    fifoOffset |= (VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA,
					0x49) & 0x1C) << 17;
	    pSmi->FBReserved = fifoOffset;	/* PDR#1074 */
	}
	else
	    pSmi->FBReserved = pSmi->videoRAMBytes - 2048;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Cursor Offset: %08lX\n",
		   (unsigned long)pSmi->FBCursorOffset);

	/* Assign hwp->MemBase & IOBase here */
	hwp = VGAHWPTR(pScrn);
	if (pSmi->IOBase != NULL)
	    vgaHWSetMmioFuncs(hwp, pSmi->MapBase, pSmi->IOBase - pSmi->MapBase);
	vgaHWGetIOBase(hwp);

	/* Map the VGA memory when the primary video */
	if (xf86IsPrimaryPci(pSmi->PciInfo)) {
	    hwp->MapSize = 0x10000;
	    if (!vgaHWMapMem(pScrn))
		LEAVE(FALSE);
	    pSmi->PrimaryVidMapped = TRUE;
	}
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Reserved: %08lX\n",
	       (unsigned long)pSmi->FBReserved);

    LEAVE(TRUE);
}

/* UnMapMem - contains half of pre-4.0 EnterLeave function.  The EnterLeave
 * function which en/disable access to IO ports and ext. regs
 */

void
SMI_UnmapMem(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    /* Unmap VGA mem if mapped. */
    if (pSmi->PrimaryVidMapped) {
	vgaHWUnmapMem(pScrn);
	pSmi->PrimaryVidMapped = FALSE;
    }

    SMI_DisableMmio(pScrn);

    if (pSmi->MapBase) {
#ifndef XSERVER_LIBPCIACCESS
	xf86UnMapVidMem(pScrn->scrnIndex, (pointer)pSmi->MapBase,
			pSmi->MapSize);
#else
	pci_device_unmap_range(pSmi->PciInfo, (pointer)pSmi->MapBase,
			       pSmi->MapSize);
#endif
	pSmi->MapBase = NULL;
    }

    if (pSmi->FBBase) {
#ifndef XSERVER_LIBPCIACCESS
	xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pSmi->FBBase,
			pSmi->videoRAMBytes);
#else
	pci_device_unmap_range(pSmi->PciInfo, (pointer)pSmi->FBBase,
			       pSmi->videoRAMBytes);
#endif
	pSmi->FBBase = NULL;
    }

    LEAVE();
}

/* This gets called at the start of each server generation. */

static Bool
SMI_ScreenInit(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr		pScrn = xf86ScreenToScrn(pScreen);
    SMIPtr		pSmi = SMIPTR(pScrn);
    EntityInfoPtr	pEnt;
	
    ENTER();

    /* Map MMIO regs and framebuffer */
    if (!SMI_MapMem(pScrn))
	LEAVE(FALSE);

    pEnt = xf86GetEntityInfo(pScrn->entityList[0]);

    if (!pSmi->pInt10 && pSmi->useBIOS) {
	pSmi->pInt10 = xf86InitInt10(pEnt->index);
    }
    if (!pSmi->pVbe && pSmi->pInt10 && xf86LoaderCheckSymbol("VBEInit")) {
	pSmi->pVbe = VBEInit(pSmi->pInt10, pEnt->index);
    }

    /* Save the chip/graphics state */
    pSmi->Save(pScrn);

    /* Fill in some needed pScrn fields */
    pScrn->vtSema = TRUE;
    pScrn->pScreen = pScreen;

    pScrn->displayWidth = ((pScrn->virtualX * pSmi->Bpp + 15) & ~15) / pSmi->Bpp;

    pSmi->fbArea = NULL;
    pSmi->FBOffset = 0;
    pScrn->fbOffset = pSmi->FBOffset + pSmi->fbMapOffset;

    /* Clear frame buffer */
    memset(pSmi->FBBase, 0, pSmi->videoRAMBytes);

    /*
     * The next step is to setup the screen's visuals, and initialise the
     * framebuffer code.  In cases where the framebuffer's default choises for
     * things like visual layouts and bits per RGB are OK, this may be as simple
     * as calling the framebuffer's ScreenInit() function.  If not, the visuals
     * will need to be setup before calling a fb ScreenInit() function and fixed
     * up after.
     */

    /*
     * Reset the visual list.
     */
    miClearVisualTypes();

    /* Setup the visuals we support. */

    if (!miSetVisualTypes(pScrn->depth, miGetDefaultVisualMask(pScrn->depth),
			  pScrn->rgbBits, pScrn->defaultVisual))
	LEAVE(FALSE);

    if (!miSetPixmapDepths ())
	LEAVE(FALSE);

    /*
     * Call the framebuffer layer's ScreenInit function
     */

    DEBUG("\tInitializing FB @ 0x%08X for %dx%d (%d)\n",
	  pSmi->FBBase, pScrn->virtualX, pScrn->virtualY, pScrn->displayWidth);
    if(!fbScreenInit(pScreen, pSmi->FBBase, pScrn->virtualX, pScrn->virtualY, pScrn->xDpi,
		     pScrn->yDpi, pScrn->displayWidth, pScrn->bitsPerPixel))
	LEAVE(FALSE);

    xf86SetBlackWhitePixels(pScreen);

    if (pScrn->bitsPerPixel > 8) {
	VisualPtr visual;
	/* Fixup RGB ordering */
	visual = pScreen->visuals + pScreen->numVisuals;
	while (--visual >= pScreen->visuals) {
	    if ((visual->class | DynamicClass) == DirectColor) {
		visual->offsetRed   = pScrn->offset.red;
		visual->offsetGreen = pScrn->offset.green;
		visual->offsetBlue  = pScrn->offset.blue;
		visual->redMask     = pScrn->mask.red;
		visual->greenMask   = pScrn->mask.green;
		visual->blueMask    = pScrn->mask.blue;
	    }
	}
    }

    /* must be after RGB ordering fixed */
    fbPictureInit(pScreen, 0, 0);
 
    /* Do the CRTC independent initialization */
    if(!SMI_HWInit(pScrn))
	LEAVE(FALSE);

    /* Unless using EXA, regardless or using XAA or not, needs offscreen
     * management at least for video. */
    if (pSmi->NoAccel || !pSmi->useEXA) {
	int		numLines;
	BoxRec		AvailFBArea;

	numLines = pSmi->FBReserved / (pScrn->displayWidth * pSmi->Bpp);
	AvailFBArea.x1 = 0;
	AvailFBArea.y1 = 0;
	AvailFBArea.x2 = pScrn->virtualX;
	AvailFBArea.y2 = numLines;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "FrameBuffer Box: %d,%d - %d,%d\n",
		   AvailFBArea.x1, AvailFBArea.y1, AvailFBArea.x2,
		   AvailFBArea.y2);

	xf86InitFBManager(pScreen, &AvailFBArea);
    }

    /* Initialize acceleration layer */
    if (!pSmi->NoAccel) {
	if (pSmi->useEXA && !SMI_EXAInit(pScreen))
	    LEAVE(FALSE);
	else if (!pSmi->useEXA && !SMI_XAAInit(pScreen))
	    LEAVE(FALSE);
    }

    /* Initialize the chosen modes */
    if (!xf86SetDesiredModes(pScrn))
	    LEAVE(FALSE);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		   "Done writing mode.  Register dump:\n");
    SMI_PrintRegs(pScrn);

#ifdef HAVE_XMODES
    xf86DiDGAInit(pScreen, (unsigned long)(pSmi->FBBase + pScrn->fbOffset));
#endif

    /* Initialise cursor functions */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    /* Initialize HW cursor layer.  Must follow software cursor
     * initialization.
     */
    if (pSmi->HwCursor) {
	int	size, flags;

	if (IS_MSOC(pSmi)) {
	    size = SMI501_MAX_CURSOR;
	    flags = (HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_1 |
		     HARDWARE_CURSOR_SWAP_SOURCE_AND_MASK);
#if SMI_CURSOR_ALPHA_PLANE
	    if (!pSmi->Dualhead)
		flags |= HARDWARE_CURSOR_ARGB;
#endif
	}
	else {
	    size = SMILYNX_MAX_CURSOR;
	    flags = (HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_8 |
		     HARDWARE_CURSOR_SWAP_SOURCE_AND_MASK |
		     HARDWARE_CURSOR_AND_SOURCE_WITH_MASK |
		     HARDWARE_CURSOR_BIT_ORDER_MSBFIRST |
		     HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
		     HARDWARE_CURSOR_INVERT_MASK);
	}

	if (!xf86_cursors_init(pScreen, size, size, flags))
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Hardware cursor initialization failed\n");
    }

    /* Initialise default colormap */
    if (!miCreateDefColormap(pScreen))
	LEAVE(FALSE);

    /* Initialize colormap layer.  Must follow initialization of the default
     * colormap.  And SetGamma call, else it will load palette with solid white.
     */
    if (!xf86HandleColormaps(pScreen, 256, pScrn->rgbBits,SMI_LoadPalette, NULL,
			     CMAP_RELOAD_ON_MODE_SWITCH | CMAP_PALETTED_TRUECOLOR))
	LEAVE(FALSE);

    pScreen->SaveScreen = SMI_SaveScreen;
    pSmi->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = SMI_CloseScreen;

    if ((IS_MSOC(pSmi) &&
	 !xf86DPMSInit(pScreen, SMI501_DisplayPowerManagementSet, 0)) ||
	(!IS_MSOC(pSmi) &&
	 !xf86DPMSInit(pScreen, SMILynx_DisplayPowerManagementSet, 0)))
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "DPMS initialization failed!\n");

    SMI_InitVideo(pScreen);

    if(!xf86CrtcScreenInit(pScreen))
	LEAVE(FALSE);

    /* Report any unused options (only for the first generation) */
    if (serverGeneration == 1) {
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);
    }

    LEAVE(TRUE);
}

/*
 * This is called at the end of each server generation.  It restores the
 * original (text) mode.  It should also unmap the video memory, and free any
 * per-generation data allocated by the driver.  It should finish by unwrapping
 * and calling the saved CloseScreen function.
 */

static Bool
SMI_CloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr	pScrn = xf86ScreenToScrn(pScreen);
    SMIPtr	pSmi = SMIPTR(pScrn);
    Bool	ret;
	
    ENTER();

    if (pSmi->HwCursor)
	xf86_cursors_fini(pScreen);

    if (pScrn->vtSema)
	/* Restore console mode and unmap framebuffer */
        SMI_LeaveVT(VT_FUNC_ARGS);

#ifdef HAVE_XAA_H
    if (pSmi->XAAInfoRec != NULL) {
	XAADestroyInfoRec(pSmi->XAAInfoRec);
    }
#endif
    if (pSmi->EXADriverPtr) {
	exaDriverFini(pScreen);
	pSmi->EXADriverPtr = NULL;
    }
    if (pSmi->pVbe != NULL) {
	vbeFree(pSmi->pVbe);
	pSmi->pVbe = NULL;
    }
    if (pSmi->pInt10 != NULL) {
	xf86FreeInt10(pSmi->pInt10);
	pSmi->pInt10 = NULL;
    }
    if (pSmi->ptrAdaptor != NULL) {
	free(pSmi->ptrAdaptor);
    }
    if (pSmi->BlockHandler != NULL) {
	pScreen->BlockHandler = pSmi->BlockHandler;
    }

    pScrn->vtSema = FALSE;
    pScreen->CloseScreen = pSmi->CloseScreen;
    ret = (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);

    LEAVE(ret);
}

static void
SMI_FreeScreen(FREE_SCREEN_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    SMI_FreeRec(pScrn);
}

static Bool
SMI_SaveScreen(ScreenPtr pScreen, int mode)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);

    ENTER();

    if(xf86IsUnblank(mode)){
	pScrn->DPMSSet(pScrn, DPMSModeOn, 0);
    }else{
	pScrn->DPMSSet(pScrn, DPMSModeOff, 0);
    }

    LEAVE(TRUE);
}

void
SMI_AdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    xf86CrtcConfigPtr crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86CrtcPtr compat_crtc = crtcConf->output[crtcConf->compat_output]->crtc;

    ENTER();

    SMICRTC(compat_crtc)->adjust_frame(compat_crtc,x,y);

    LEAVE();
}

Bool
SMI_SwitchMode(SWITCH_MODE_ARGS_DECL)
{
    Bool ret;
    SCRN_INFO_PTR(arg);
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    ret = xf86SetSingleMode(pScrn, mode, RR_Rotate_0);

    if (!pSmi->NoAccel)
	SMI_EngineReset(pScrn);

    LEAVE(ret);
}

void
SMI_LoadPalette(ScrnInfoPtr pScrn, int numColors, int *indicies,
		LOCO *colors, VisualPtr pVisual)
{
    xf86CrtcConfigPtr crtcConf = XF86_CRTC_CONFIG_PTR(pScrn);
    int crtc_idx,i,j;

    ENTER();

    if(pScrn->bitsPerPixel == 16){
	/* Expand the RGB 565 palette into the 256-elements LUT */

	for(crtc_idx=0; crtc_idx<crtcConf->num_crtc; crtc_idx++){
	    SMICrtcPrivatePtr crtcPriv = SMICRTC(crtcConf->crtc[crtc_idx]);

	    for(i=0; i<numColors; i++){
		int idx = indicies[i];

		if(idx<32){
		    for(j=0; j<8; j++){
			crtcPriv->lut_r[idx*8 + j] = colors[idx].red << 8;
			crtcPriv->lut_b[idx*8 + j] = colors[idx].blue << 8;
		    }
		}

		for(j=0; j<4; j++)
		    crtcPriv->lut_g[idx*4 + j] = colors[idx].green << 8;
	    }

	    crtcPriv->load_lut(crtcConf->crtc[crtc_idx]);
    }
    }else{
	for(crtc_idx=0; crtc_idx<crtcConf->num_crtc; crtc_idx++){
	    SMICrtcPrivatePtr crtcPriv = SMICRTC(crtcConf->crtc[crtc_idx]);

    for(i = 0; i < numColors; i++) {
		int idx = indicies[i];

		crtcPriv->lut_r[idx] = colors[idx].red << 8;
		crtcPriv->lut_g[idx] = colors[idx].green << 8;
		crtcPriv->lut_b[idx] = colors[idx].blue << 8;
	    }

	    crtcPriv->load_lut(crtcConf->crtc[crtc_idx]);
	}
    }

    LEAVE();
}

static void
SMI_DisableVideo(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    CARD8 tmp;

    if (!IS_MSOC(pSmi)) {
	if (!(tmp = VGAIN8(pSmi, VGA_DAC_MASK)))
	    return;
	pSmi->DACmask = tmp;
	VGAOUT8(pSmi, VGA_DAC_MASK, 0);
    }
}

static void
SMI_EnableVideo(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    if (!IS_MSOC(pSmi)) {
	VGAOUT8(pSmi, VGA_DAC_MASK, pSmi->DACmask);
    }
}


void
SMI_EnableMmio(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    if (!IS_MSOC(pSmi)) {
	vgaHWPtr hwp = VGAHWPTR(pScrn);
	CARD8 tmp;

	/*
	 * Enable chipset (seen on uninitialized secondary cards) might not be
	 * needed once we use the VGA softbooter
	 */
	vgaHWSetStdFuncs(hwp);

	/* Enable linear mode */
	outb(pSmi->PIOBase + VGA_SEQ_INDEX, 0x18);
	tmp = inb(pSmi->PIOBase + VGA_SEQ_DATA);
	pSmi->SR18Value = tmp;					/* PDR#521 */
	outb(pSmi->PIOBase + VGA_SEQ_DATA, tmp | 0x11);

	/* Enable 2D/3D Engine and Video Processor */
	outb(pSmi->PIOBase + VGA_SEQ_INDEX, 0x21);
	tmp = inb(pSmi->PIOBase + VGA_SEQ_DATA);
	pSmi->SR21Value = tmp;					/* PDR#521 */
	outb(pSmi->PIOBase + VGA_SEQ_DATA, tmp & ~0x03);
    }

    LEAVE();
}

void
SMI_DisableMmio(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    if (!IS_MSOC(pSmi)) {
	vgaHWPtr hwp = VGAHWPTR(pScrn);

	vgaHWSetStdFuncs(hwp);

	/* Disable 2D/3D Engine and Video Processor */
	outb(pSmi->PIOBase + VGA_SEQ_INDEX, 0x21);
	outb(pSmi->PIOBase + VGA_SEQ_DATA, pSmi->SR21Value);	/* PDR#521 */

	/* Disable linear mode */
	outb(pSmi->PIOBase + VGA_SEQ_INDEX, 0x18);
	outb(pSmi->PIOBase + VGA_SEQ_DATA, pSmi->SR18Value);	/* PDR#521 */
    }

    LEAVE();
}

static void
SMI_ProbeDDC(ScrnInfoPtr pScrn, int index)
{
    vbeInfoPtr pVbe;
    if (xf86LoadSubModule(pScrn, "vbe")) {
	pVbe = VBEInit(NULL, index);
	ConfiguredMonitor = vbeDoEDID(pVbe, NULL);
	vbeFree(pVbe);
    }
}

static Bool
SMI_HWInit(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    ENTER();

    if(IS_MSOC(pSmi))
	LEAVE(SMI501_HWInit(pScrn));
    else
	LEAVE(SMILynx_HWInit(pScrn));
}

void
SMI_PrintRegs(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);
    int i;

    ENTER();

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		"START register dump ------------------\n");

    if(IS_MSOC(pSmi))
	SMI501_PrintRegs(pScrn);
    else
	SMILynx_PrintRegs(pScrn);


    xf86ErrorFVerb(VERBLEV, "\n\nDPR    x0       x4       x8       xC");
    for (i = 0x00; i <= 0x44; i += 4) {
	if ((i & 0xF) == 0x0) xf86ErrorFVerb(VERBLEV, "\n%02X|", i);
	xf86ErrorFVerb(VERBLEV, " %08lX", (unsigned long)READ_DPR(pSmi, i));
    }

    xf86ErrorFVerb(VERBLEV, "\n\nVPR    x0       x4       x8       xC");
    for (i = 0x00; i <= 0x60; i += 4) {
	if ((i & 0xF) == 0x0) xf86ErrorFVerb(VERBLEV, "\n%02X|", i);
	xf86ErrorFVerb(VERBLEV, " %08lX", (unsigned long)READ_VPR(pSmi, i));
    }

    xf86ErrorFVerb(VERBLEV, "\n\nCPR    x0       x4       x8       xC");
    for (i = 0x00; i <= 0x18; i += 4) {
	if ((i & 0xF) == 0x0) xf86ErrorFVerb(VERBLEV, "\n%02X|", i);
	xf86ErrorFVerb(VERBLEV, " %08lX", (unsigned long)READ_CPR(pSmi, i));
    }

    xf86ErrorFVerb(VERBLEV, "\n\n");
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, VERBLEV,
		"END register dump --------------------\n");

    LEAVE();
}
