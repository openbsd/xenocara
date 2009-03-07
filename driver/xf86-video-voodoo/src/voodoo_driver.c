/* 
 *  XFree86 driver for the Voodoo 2 using native X support and no
 * Glide2. This is done for two reasons, firstly Glide2 is not portable
 * to other than x86_32 which is becoming an issue, and secondly to get
 * accelerations that Glide does not expose.  The Voodoo 2 hardware has
 * bit blit (screen->screen, cpu->screen), some colour expansion and 
 * also alpha (so could do hw render even!). Also can in theory use
 * texture ram and engine to do arbitary Xv support as we have
 * colour match on the 2D blit (ie 3D blit to back, 2D blit to front)
 * along with alpha on the Xv 8) and with some care rotation of Xv.
 * 
 * Alan Cox <alan@redhat.com>
 * 
 * Derived from:
 *
 * XFree86 driver for Glide(tm). (Mainly for Voodoo 1 and 2 cards)
 *  Author: 
 *   Henrik Harmsen (hch@cd.chalmers.se or Henrik.Harmsen@erv.ericsson.se) 
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of Red Hat, Alan Cox and Henrik Harmsen
 * not be used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.  Th authors make no 
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL RICHARD HECKER BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 * 
 * THIS SOFTWARE IS NOT DESIGNED FOR USE IN SAFETY CRITICAL SYSTEMS OF
 * ANY KIND OR FORM.
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "fb.h"
#include "mibank.h"
#include "micmap.h"
#include "mipointer.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xorgVersion.h"
#include "xf86PciInfo.h"
#include "xf86Pci.h"
#include "xf86cmap.h"
#include "shadowfb.h"
#include "vgaHW.h"
#include "xf86RAC.h"
#include "xf86Resources.h"
#include "compiler.h"
#include "xaa.h"

#include "voodoo.h"

#define _XF86DGA_SERVER_
#include <X11/extensions/xf86dgastr.h>

#include "opaque.h"
#define DPMS_SERVER
#include <X11/extensions/dpms.h>

static const OptionInfoRec * VoodooAvailableOptions(int chipid, int busid);
static void	VoodooIdentify(int flags);
static Bool	VoodooProbe(DriverPtr drv, int flags);
static Bool	VoodooPreInit(ScrnInfoPtr pScrn, int flags);
static Bool	VoodooScreenInit(int Index, ScreenPtr pScreen, int argc, char **argv);
static Bool	VoodooEnterVT(int scrnIndex, int flags);
static void	VoodooLeaveVT(int scrnIndex, int flags);
static Bool	VoodooCloseScreen(int scrnIndex, ScreenPtr pScreen);
static Bool	VoodooSaveScreen(ScreenPtr pScreen, int mode);
static void     VoodooFreeScreen(int scrnIndex, int flags);
static void     VoodooRefreshArea16(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
static void     VoodooRefreshArea24(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
static Bool	VoodooSwitchMode(int scrnIndex, DisplayModePtr mode, int flags);
static Bool     VoodooModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
static void     VoodooRestore(ScrnInfoPtr pScrn, Bool Closing);

static void	VoodooDisplayPowerManagementSet(ScrnInfoPtr pScrn,
                                               int PowerManagementMode,
                                               int flags);

/* 
 * This contains the functions needed by the server after loading the
 * driver module.  It must be supplied, and gets added the driver list by
 * the Module Setup funtion in the dynamic case.  In the static case a
 * reference to this is compiled in, and this requires that the name of
 * this DriverRec be an upper-case version of the driver name.
 */

_X_EXPORT DriverRec VOODOO = {
  VOODOO_VERSION,
  VOODOO_DRIVER_NAME,
  VoodooIdentify,
  VoodooProbe,
  VoodooAvailableOptions,
  NULL,
  0
};

typedef enum {
  OPTION_NOACCEL,
  OPTION_SHADOW_FB,
  OPTION_PASS_THROUGH,
} VoodooOpts;

static const OptionInfoRec VoodooOptions[] = {
  { OPTION_NOACCEL,	"NoAccel",	OPTV_BOOLEAN,	{0}, FALSE },
  { OPTION_SHADOW_FB,	"ShadowFB",	OPTV_BOOLEAN,	{0}, FALSE },
  { OPTION_PASS_THROUGH,"PassThrough",  OPTV_BOOLEAN,   {0}, FALSE },
  { -1,	                NULL,           OPTV_NONE,      {0}, FALSE }
};

/* Supported chipsets */
static SymTabRec VoodooChipsets[] = {
  { PCI_CHIP_VOODOO1, "Voodoo 1" },
  { PCI_CHIP_VOODOO2, "Voodoo 2" },
  {-1, NULL }
};


/*
 * List of symbols from other modules that this module references.  This
 * list is used to tell the loader that it is OK for symbols here to be
 * unresolved providing that it hasn't been told that they haven't been
 * told that they are essential via a call to xf86LoaderReqSymbols() or
 * xf86LoaderReqSymLists().  The purpose is this is to avoid warnings about
 * unresolved symbols that are not required.
 */

static const char *fbSymbols[] = {
  "fbScreenInit",
  "fbPictureInit",
  NULL
};

static const char *xaaSymbols[] = {
    "XAACreateInfoRec",
    "XAAInit",
    "XAADestroyInfoRec",
    NULL
};

static const char *shadowSymbols[] = {
  "ShadowFBInit",
  NULL
};

#ifdef XFree86LOADER

static XF86ModuleVersionInfo voodooVersRec =
{
  "voodoo",
  MODULEVENDORSTRING,
  MODINFOSTRING1,
  MODINFOSTRING2,
  XORG_VERSION_CURRENT,
  VOODOO_MAJOR_VERSION, VOODOO_MINOR_VERSION, VOODOO_PATCHLEVEL,
  ABI_CLASS_VIDEODRV,			/* This is a video driver */
  ABI_VIDEODRV_VERSION,
  MOD_CLASS_VIDEODRV,
  {0,0,0,0}
};

static pointer voodooSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
  static int setupDone = FALSE;
  if(errmaj)
      *errmaj = LDR_ONCEONLY;
  if(setupDone == FALSE)
  {
    setupDone = TRUE;
    xf86AddDriver(&VOODOO, module, 0);
    LoaderRefSymLists(fbSymbols, shadowSymbols, xaaSymbols,NULL);
    return (pointer)1;    
  }
  return NULL;
}

_X_EXPORT XF86ModuleData voodooModuleData = {
  &voodooVersRec,
  voodooSetup,
  NULL
};

#endif /* XFree86LOADER */

static Bool
VoodooGetRec(ScrnInfoPtr pScrn)
{
  /*
   * Allocate an VoodooRec, and hook it into pScrn->driverPrivate.
   * pScrn->driverPrivate is initialised to NULL, so we can check if
   * the allocation has already been done.
     */
  if (pScrn->driverPrivate != NULL)
    return TRUE;

  pScrn->driverPrivate = xnfcalloc(sizeof(VoodooRec), 1);

  /* Initialize it */
  /* No init here yet */
  return TRUE;
}

static void
VoodooFreeRec(ScrnInfoPtr pScrn)
{
  if (pScrn->driverPrivate == NULL)
    return;
  xfree(pScrn->driverPrivate);
  pScrn->driverPrivate = NULL;
}


static const OptionInfoRec *
VoodooAvailableOptions(int chipid, int busid)
{
   return VoodooOptions;
}

/* Mandatory */
static void
VoodooIdentify(int flags)
{
  xf86PrintChipsets(VOODOO_NAME, "driver for Voodoo1/Voodoo2", VoodooChipsets);
}

static PciChipsets VoodooPCIChipsets[] = {
    { PCI_CHIP_VOODOO1, PCI_CHIP_VOODOO1, 0 },
    { PCI_CHIP_VOODOO2, PCI_CHIP_VOODOO2, 0 },
    { -1,	    -1, RES_UNDEFINED }
};

/* Mandatory */
static Bool
VoodooProbe(DriverPtr drv, int flags)
{
    int i, numDevSections, numUsed, *usedChips;
    GDevPtr *devSections;
    Bool foundScreen = FALSE;

    /*
     * Look for config file Device sections with this driver specified.
     */
    if ((numDevSections = xf86MatchDevice(VOODOO_DRIVER_NAME,
						&devSections)) <= 0) {
#ifdef DEBUG
   	xf86ErrorFVerb(3,"%s: No Device section found.\n",VOODOO_NAME);
#endif
	/*
	 * There's no matching device section in the config file, so quit
	 * now.
	 */
	return FALSE;
    }

    /* PCI BUS */
#ifndef XSERVER_LIBPCIACCESS
    if (xf86GetPciVideoInfo() )
#endif
    {
	numUsed = xf86MatchPciInstances(VOODOO_NAME, PCI_VENDOR_3DFX,
					VoodooChipsets, VoodooPCIChipsets, 
					devSections,numDevSections,
					drv, &usedChips);

	if (numUsed > 0) {
	    if (flags & PROBE_DETECT)
		foundScreen = TRUE;
	    else for (i = 0; i < numUsed; i++) {
		ScrnInfoPtr pScrn = NULL;
		EntityInfoPtr pEnt;
		
		/* Allocate a ScrnInfoRec and claim the slot */
		if ((pScrn = xf86ConfigPciEntity(pScrn, 0, usedChips[i],
						       VoodooPCIChipsets,NULL,
						       NULL, NULL, NULL,
						       NULL))) {
		    pScrn->driverVersion = VOODOO_VERSION;
		    pScrn->driverName    = VOODOO_DRIVER_NAME;
		    pScrn->name          = VOODOO_NAME;
		    pScrn->Probe	 = VoodooProbe;
		    pScrn->PreInit	 = VoodooPreInit;
		    pScrn->ScreenInit    = VoodooScreenInit;
		    pScrn->SwitchMode	 = VoodooSwitchMode;
		    pScrn->EnterVT	 = VoodooEnterVT;
		    pScrn->LeaveVT	 = VoodooLeaveVT;
		    pScrn->FreeScreen    = VoodooFreeScreen;
		    foundScreen = TRUE;
		}
		pEnt = xf86GetEntityInfo(usedChips[i]);
	    }
	    xfree(usedChips);
	}
    }
    xfree(devSections);
    return foundScreen;
}
	



/* Mandatory */
static Bool
VoodooPreInit(ScrnInfoPtr pScrn, int flags)
{
  VoodooPtr pVoo;
  int i;
  ClockRangePtr clockRanges;
  MessageType from;
  int maxwidth;

  if (flags & PROBE_DETECT)
      return FALSE;

  /* Check the number of entities, and fail if it isn't one. */
  if (pScrn->numEntities != 1)
    return FALSE;

  /* Set pScrn->monitor */
  pScrn->monitor = pScrn->confScreen->monitor;

  if (!xf86SetDepthBpp(pScrn, 16, 0, 0, Support32bppFb)) {
    return FALSE;
  }

  /* Check that the returned depth is one we support */
  switch (pScrn->depth) {
  case 16:
  case 24:
  case 32:
    break;
  default:
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
               "Given depth (%d) is not supported by this driver\n",
               pScrn->depth);
    return FALSE;
  }
  xf86PrintDepthBpp(pScrn);
  
  if(pScrn->depth == 32)
  	pScrn->depth = 24;
  
  /*
   * This must happen after pScrn->display has been set because
   * xf86SetWeight references it.
   */

  if (pScrn->depth > 8) {
    /* The defaults are OK for us */
    rgb zeros = {0, 0, 0};

    if (!xf86SetWeight(pScrn, zeros, zeros)) {
      return FALSE;
    } else {
      /* XXX check that weight returned is supported */
      ;
    }
  }

  /* Set the default visual. */
  if (!xf86SetDefaultVisual(pScrn, -1)) {
    return FALSE;
  }
  /* We don't support DirectColor at > 8bpp */
  if (pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Given default visual"
               " (%s) is not supported at depth %d\n",
               xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
    return FALSE;
  }

  /* Set default gamma */
  {
    Gamma zeros = {0.0, 0.0, 0.0};

    if (!xf86SetGamma(pScrn, zeros)) {
      return FALSE;
    }
  }

  /* We use a programmable clock */
  pScrn->progClock = TRUE;

  /* Allocate the VoodooRec driverPrivate */
  if (!VoodooGetRec(pScrn)) {
    return FALSE;
  }

  pVoo = VoodooPTR(pScrn);

  /* Get the entity */
  pVoo->pEnt = xf86GetEntityInfo(pScrn->entityList[0]);
  
  pVoo->PciInfo = xf86GetPciInfoForEntity(pVoo->pEnt->index);
#ifndef XSERVER_LIBPCIACCESS
  pVoo->PciTag = pciTag(pVoo->PciInfo->bus, pVoo->PciInfo->device, pVoo->PciInfo->func);
#endif

  /* Collect all of the relevant option flags (fill in pScrn->options) */
  xf86CollectOptions(pScrn, NULL);

  /* Process the options */
  if (!(pVoo->Options = xalloc(sizeof(VoodooOptions))))
    return FALSE;
  memcpy(pVoo->Options, VoodooOptions, sizeof(VoodooOptions));
  xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pVoo->Options);

  /* Need to do rotation some day */

  if(pVoo->pEnt->chipset == PCI_CHIP_VOODOO2)
  {
  	pVoo->Voodoo2 = 1;	/* We have 2D accel, interlace, double */
  	pVoo->Accel = 1;
  }
  else
  {
  	pVoo->Voodoo2 = 0;
  	pVoo->ShadowFB = 1;
  	xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT, "Using shadowFB with Voodoo1 hardware.\n");
  }

  from = X_DEFAULT;

  if (xf86ReturnOptValBool(pVoo->Options, OPTION_SHADOW_FB,  FALSE)) {
  	pVoo->ShadowFB = 1;
  	pVoo->Accel = 0;
  }
  
  if (xf86ReturnOptValBool(pVoo->Options, OPTION_PASS_THROUGH,  FALSE))
      pVoo->PassThrough = 1;

  if (xf86ReturnOptValBool(pVoo->Options, OPTION_NOACCEL, FALSE)) {
  	pVoo->ShadowFB = 1;
  	pVoo->Accel = 0;
  }
  
  if(pScrn->depth == 24 && !pVoo->ShadowFB)
  {
      xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT, "ShadowFB is required for 24/32bit modes.\n");
      pVoo->ShadowFB = 1;
      pVoo->Accel = 0;
  }

  /* MMIO at 0 , FB at 4Mb, Texture at 8Mb */
  pVoo->PhysBase = PCI_REGION_BASE(pVoo->PciInfo, 0, REGION_MEM) + 0x400000;

#ifndef XSERVER_LIBPCIACCESS
  pVoo->MMIO = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO, pVoo->PciTag,
			     pVoo->PciInfo->memBase[0], 0x400000);
  pVoo->FBBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO, pVoo->PciTag,
			       pVoo->PciInfo->memBase[0] + 0x400000, 0x400000);
  		
#else
  {
    void** result = (void**)&pVoo->MMIO;
    int err = pci_device_map_range(pVoo->PciInfo,
				   PCI_REGION_BASE(pVoo->PciInfo, 0, REGION_MEM),
				   0x400000,
				   PCI_DEV_MAP_FLAG_WRITABLE,
				   result);
    if (err)
      return FALSE;
  }

  {
    void** result = (void**)&pVoo->FBBase;
    int err = pci_device_map_range(pVoo->PciInfo,
				   PCI_REGION_BASE(pVoo->PciInfo, 0, REGION_MEM) + 0x400000,
				   0x400000,
				   PCI_DEV_MAP_FLAG_WRITABLE|
				   PCI_DEV_MAP_FLAG_WRITE_COMBINE,
				   result);
    if (err)
      return FALSE;
  }
#endif  		
  VoodooHardwareInit(pVoo);
  
  /*
   * If the user has specified the amount of memory in the XF86Config
   * file, we respect that setting.
   */
  if (pVoo->pEnt->device->videoRam != 0) {
    pScrn->videoRam = pVoo->pEnt->device->videoRam;
    from = X_CONFIG;
  } else {
    pScrn->videoRam = VoodooMemorySize(pVoo) * 1024 ;	/* Sizer reports Mbytes */
    from = X_PROBED;
  }
  xf86DrvMsg(pScrn->scrnIndex, from, "Video RAM: %d kB\n",
             pScrn->videoRam);

  /* Set up clock ranges so that the xf86ValidateModes() function will not fail a mode because of the clock
     requirement (because we don't use the clock value anyway) */
  clockRanges = xnfcalloc(sizeof(ClockRange), 1);
  clockRanges->next = NULL;
  clockRanges->minClock = 10000;
  clockRanges->maxClock = 250000;	/* 250MHz DAC */
  clockRanges->clockIndex = -1;		/* programmable */
  
  if(pVoo->Voodoo2)
  {
    clockRanges->interlaceAllowed = TRUE;
    clockRanges->doubleScanAllowed = TRUE;
    maxwidth = min(1024, pScrn->display->virtualX);
  }
  else
  {
    clockRanges->interlaceAllowed = FALSE;
    clockRanges->doubleScanAllowed = FALSE;
    maxwidth = min(800, pScrn->display->virtualX);
  }

  /* Select valid modes from those available */
  i = xf86ValidateModes(pScrn, pScrn->monitor->Modes,
                        pScrn->display->modes, clockRanges,
                        NULL, 256, 2048,
                        pScrn->bitsPerPixel, 128, 768,
                        pScrn->display->virtualX,
                        pScrn->display->virtualY,
                        pScrn->videoRam * 1024,
                        LOOKUP_BEST_REFRESH);
    
  if (i == -1) {
    VoodooFreeRec(pScrn);
    return FALSE;
  }

  /* Prune the modes marked as invalid */
  xf86PruneDriverModes(pScrn);

  /* If no valid modes, return */
  if (i == 0 || pScrn->modes == NULL) {
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
    VoodooFreeRec(pScrn);
    return FALSE;
  }

  /* Set the current mode to the first in the list */
  xf86SetCrtcForModes(pScrn, 0);
  pScrn->currentMode = pScrn->modes;

  /* Do some checking, we will not support a virtual framebuffer larger than
     the visible screen. */
  if (pScrn->currentMode->HDisplay != pScrn->virtualX || 
      pScrn->currentMode->VDisplay != pScrn->virtualY ||
      pScrn->displayWidth != pScrn->virtualX)
  {
    /* FIXME: In this case we could use shadowfb and clip the drawing into
       the physical buffer */
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
               "Virtual size doesn't equal display size. Forcing virtual size to equal display size.\n");
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
               "(Virtual size: %dx%d, Display size: %dx%d)\n", pScrn->virtualX, pScrn->virtualY,
               pScrn->currentMode->HDisplay, pScrn->currentMode->VDisplay);
    /* I'm not entirely sure this is "legal" but I hope so. */
    pScrn->virtualX = pScrn->currentMode->HDisplay;
    pScrn->virtualY = pScrn->currentMode->VDisplay;
    pScrn->displayWidth = pScrn->virtualX;
  }

  /* Print the list of modes being used */
  xf86PrintModes(pScrn);

  /* Set display resolution */
  xf86SetDpi(pScrn, 0, 0);
    
  /* Load fb */
  if (xf86LoadSubModule(pScrn, "fb") == NULL) {
    VoodooFreeRec(pScrn);
    return FALSE;
  }

  xf86LoaderReqSymLists(fbSymbols, NULL);

  if (!xf86LoadSubModule(pScrn, "xaa")) {
    VoodooFreeRec(pScrn);
    return FALSE;
  }
  
  xf86LoaderReqSymLists(xaaSymbols, NULL);
  
  if(pVoo->ShadowFB)
  {
    /* Load the shadow framebuffer */
    if (!xf86LoadSubModule(pScrn, "shadowfb")) {
      VoodooFreeRec(pScrn);
      return FALSE;
    }
    xf86LoaderReqSymLists(shadowSymbols, NULL);
  }
  return TRUE;
}


/* Mandatory */
/* This gets called at the start of each server generation */
static Bool
VoodooScreenInit(int scrnIndex, ScreenPtr pScreen, int argc, char **argv)
{
  ScrnInfoPtr pScrn;
  VoodooPtr pVoo;
  int ret;
  VisualPtr visual;
  void *FBStart;
  CARD32 displayWidth;

  /* 
   * First get the ScrnInfoRec
   */
  pScrn = xf86Screens[pScreen->myNum];

  pVoo = VoodooPTR(pScrn);

  if (!VoodooModeInit(pScrn, pScrn->currentMode))
    return FALSE;
    
  VoodooClear(pVoo);

  /*
   * The next step is to setup the screen's visuals, and initialise the
   * framebuffer code.  In cases where the framebuffer's default
   * choices for things like visual layouts and bits per RGB are OK,
   * this may be as simple as calling the framebuffer's ScreenInit()
   * function.  If not, the visuals will need to be setup before calling
   * a fb ScreenInit() function and fixed up after.
   *
   * For most PC hardware at depths >= 8, the defaults that fb uses
   * are not appropriate.  In this driver, we fixup the visuals after.
   */

  /*
   * Reset the visual list.
   */
  miClearVisualTypes();

  /* Setup the visuals we support. Only TrueColor. */
  if (!miSetVisualTypes(pScrn->depth, miGetDefaultVisualMask(pScrn->depth), pScrn->rgbBits, pScrn->defaultVisual))
    return FALSE;

  miSetPixmapDepths ();

  if(pVoo->ShadowFB)
  {
    pVoo->ShadowPitch = ((pScrn->virtualX * pScrn->bitsPerPixel >> 3) + 3) & ~3L;
    pVoo->ShadowPtr = xnfalloc(pVoo->ShadowPitch * pScrn->virtualY);
    FBStart = pVoo->ShadowPtr;
    displayWidth = pScrn->virtualX;
  }
  else
  {
    FBStart = pVoo->FBBase;
    displayWidth = 1024;
  }
    
  if(pScrn->depth == 16)
    pVoo->Pitch = 2048;
  else
    pVoo->Pitch = 4096;

  /*
   * Call the framebuffer layer's ScreenInit function, and fill in other
   * pScreen fields.
   */
  ret = fbScreenInit(pScreen, FBStart,
		     pScrn->virtualX, pScrn->virtualY,
		     pScrn->xDpi, pScrn->yDpi,
		     displayWidth,
		     pScrn->bitsPerPixel);

  if (!ret)
    return FALSE;

  xf86SetBlackWhitePixels(pScreen);

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

  /* must be after RGB ordering fixed */
  fbPictureInit (pScreen, 0, 0);
  if(!pVoo->ShadowFB)
    VoodooDGAInit(pScrn, pScreen);

  /* Activate accelerations */
  if(pVoo->Accel)
  	Voodoo2XAAInit(pScreen);

  miInitializeBackingStore(pScreen);
  xf86SetBackingStore(pScreen);
  

  /* Initialize software cursor.  
     Must precede creation of the default colormap */
  miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

  /* Initialise default colourmap */
  if (!miCreateDefColormap(pScreen))
    return FALSE;

  if(pVoo->ShadowFB)
  {
  	if(pScrn->depth == 16)
	  	ShadowFBInit(pScreen, VoodooRefreshArea16);
	  else
	  	ShadowFBInit(pScreen, VoodooRefreshArea24);
  }
  
  xf86DPMSInit(pScreen, VoodooDisplayPowerManagementSet, 0);
  
  pScrn->memPhysBase = pVoo->PhysBase;
  pScrn->fbOffset = 0;

  pScreen->SaveScreen = VoodooSaveScreen;

  /* Wrap the current CloseScreen function */
  pVoo->CloseScreen = pScreen->CloseScreen;
  pScreen->CloseScreen = VoodooCloseScreen;

  /* Report any unused options (only for the first generation) */
  if (serverGeneration == 1) {
    xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);
  }
  
  /* Done */
  return TRUE;
}



/*
 * This is called when VT switching back to the X server.  Its job is
 * to reinitialise the video mode.
 *
 * We may wish to unmap video/MMIO memory too.
 */

/* Mandatory */
static Bool
VoodooEnterVT(int scrnIndex, int flags)
{
  ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
  return VoodooModeInit(pScrn, pScrn->currentMode);
}

/*
 * This is called when VT switching away from the X server.  Its job is
 * to restore the previous (text) mode.
 *
 * We may wish to remap video/MMIO memory too.
 */

/* Mandatory */
static void
VoodooLeaveVT(int scrnIndex, int flags)
{
  ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
  VoodooRestore(pScrn, FALSE);
}

/*
 * This is called at the end of each server generation.  It restores the
 * original (text) mode.  It should also unmap the video memory, and free
 * any per-generation data allocated by the driver.  It should finish
 * by unwrapping and calling the saved CloseScreen function.
 */

/* Mandatory */
static Bool
VoodooCloseScreen(int scrnIndex, ScreenPtr pScreen)
{
  ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
  VoodooPtr pVoo = VoodooPTR(pScrn);

  if (pScrn->vtSema)
      VoodooRestore(pScrn, TRUE);
  if(pVoo->ShadowPtr)
      xfree(pVoo->ShadowPtr);
  if(pVoo->AccelInfoRec)
      xfree(pVoo->AccelInfoRec);
  if (pVoo->pDGAMode) {
    xfree(pVoo->pDGAMode);
    pVoo->pDGAMode = NULL;
    pVoo->nDGAMode = 0;
  }

  pScrn->vtSema = FALSE;

  pScreen->CloseScreen = pVoo->CloseScreen;
  return (*pScreen->CloseScreen)(scrnIndex, pScreen);
}


/* Free up any persistent data structures */

/* Optional */
static void
VoodooFreeScreen(int scrnIndex, int flags)
{
  ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
  VoodooPtr pVoo = VoodooPTR(pScrn);
  /*
   * This only gets called when a screen is being deleted.  It does not
   * get called routinely at the end of a server generation.
   */
  if (pVoo && pVoo->ShadowPtr)
    xfree(pVoo->ShadowPtr);
  VoodooFreeRec(xf86Screens[scrnIndex]);
}


/* Do screen blanking */
/* Mandatory */
static Bool
VoodooSaveScreen(ScreenPtr pScreen, int mode)
{
  ScrnInfoPtr pScrn;
  VoodooPtr pVoo;
  Bool unblank;
  
  unblank = xf86IsUnblank(mode);
  if(pScreen != NULL)
  {
    pScrn = xf86Screens[pScreen->myNum];
    pVoo = VoodooPTR(pScrn);
    
    if(pScrn->vtSema && (unblank == pVoo->Blanked))
    {
      if (unblank)
        VoodooModeInit(pScrn, pScrn->currentMode);
      else
        VoodooBlank(pVoo);
      pVoo->Blanked = !unblank;
    }
  }
  return TRUE;
}

static Bool
VoodooModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
  VoodooPtr pVoo;
  int width, height;

  pVoo = VoodooPTR(pScrn);

    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Clock : %x\n", mode->Clock);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Display : %x\n", mode->CrtcHDisplay);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Blank Start : %x\n", mode->CrtcHBlankStart);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Sync Start : %x\n", mode->CrtcHSyncStart);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Sync End : %x\n", mode->CrtcHSyncEnd);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Blank End : %x\n", mode->CrtcHBlankEnd);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Total : %x\n", mode->CrtcHTotal);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz Skew : %x\n", mode->CrtcHSkew);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Hz HAdjusted : %x\n", mode->CrtcHAdjusted);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Display : %x\n", mode->CrtcVDisplay);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Blank Start : %x\n", mode->CrtcVBlankStart);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Sync Start : %x\n", mode->CrtcVSyncStart);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Sync End : %x\n", mode->CrtcVSyncEnd);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Blank End : %x\n", mode->CrtcVBlankEnd);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt Total : %x\n", mode->CrtcVTotal);
    xf86DrvMsg(pScrn->scrnIndex,X_INFO, "Vt VAdjusted : %x\n", mode->CrtcVAdjusted);
  if ((mode->Flags & (V_INTERLACE|V_DBLSCAN)) && !pVoo->Voodoo2)
  {
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Interlaced/doublescan modes not supported\n");
    return FALSE;
  }

  width = mode->HDisplay;
  height = mode->VDisplay;

  /* Initialize the video card */
  if(VoodooMode(pScrn, mode))
  {
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Cannot set chosen mode.\n");
    return FALSE;
  }
  
  pVoo->Blanked = FALSE;
  return TRUE;
}

/*
 *	Sync engine on mode switches. The docs are not clear if
 *	this is needed but it does no harm.
 */
 
static Bool VoodooSwitchMode(int scrnIndex, DisplayModePtr mode, int flags)
{
  ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
  VoodooSync(pScrn);
  return VoodooModeInit(xf86Screens[scrnIndex], mode);
}

static void     
VoodooRestore(ScrnInfoPtr pScrn, Bool Closing)
{
  VoodooPtr pVoo;

  pVoo = VoodooPTR(pScrn);
  pVoo->Blanked = TRUE;
  if (!Closing)
      VoodooBlank(pVoo);
  if (Closing && pVoo->PassThrough)
      VoodooRestorePassThrough(pVoo);
}

static void
VoodooRefreshArea16(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
  VoodooPtr pVoo = VoodooPTR(pScrn);
  int Bpp;
  unsigned char *src;
  signed int x1, x2;
  
  if (pVoo->Blanked)
  	return;
  
  /*
   *	Voodoo 1 lacks host to CPU blit so we must use the LFB port
   */
  
  Bpp = pScrn->bitsPerPixel >> 3;
  while(num--) {
    /* We align to an even number of pixels so we won't have to copy
       half-words over the PCI bus */
    x1 = (pbox->x1) & ~1;
    x2 = (pbox->x2 + 1) & ~1;
    src = pVoo->ShadowPtr + (pbox->y1 * pVoo->ShadowPitch) + 
          (x1 * Bpp);
    VoodooCopy16(pVoo, x1, pbox->y1,  x2-x1, pbox->y2-pbox->y1, pVoo->ShadowPitch, src);
    pbox++;
  }
} 

static void
VoodooRefreshArea24(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
  VoodooPtr pVoo = VoodooPTR(pScrn);
  int Bpp;
  unsigned char *src;
  
  if (pVoo->Blanked)
  	return;
  
  /*
   *	Voodoo 1 lacks host to CPU blit so we must use the LFB port
   */
  
  Bpp = pScrn->bitsPerPixel >> 3;
  while(num--) {
    src = pVoo->ShadowPtr + (pbox->y1 * pVoo->ShadowPitch) + 
          (pbox->x1 * Bpp);
    VoodooCopy24(pVoo, pbox->x1, pbox->y1,  pbox->x2-pbox->x1, pbox->y2-pbox->y1, pVoo->ShadowPitch, src);
    pbox++;
  }
} 

/*
 * VoodooDisplayPowerManagementSet --
 *
 * Sets VESA Display Power Management Signaling (DPMS) Mode.
 */
static void
VoodooDisplayPowerManagementSet(ScrnInfoPtr pScrn, int PowerManagementMode,
                               int flags)
{
  VoodooPtr pVoo = VoodooPTR(pScrn);
  int old = -1;


  switch (PowerManagementMode)
  {
  case DPMSModeOn:
    /* Screen: On; HSync: On, VSync: On */
    if(old != DPMSModeOn && old != -1)
    {
    	VoodooModeInit(pScrn, pScrn->currentMode);
    }
    pVoo->Blanked = FALSE;
    break;
  case DPMSModeStandby:
  case DPMSModeSuspend:
  case DPMSModeOff:
    pVoo->Blanked = TRUE;
    VoodooBlank(pVoo);
    break;
  }
  old = PowerManagementMode;
}
