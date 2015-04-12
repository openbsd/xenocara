
/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *
 * Add ARGB HW cursor support:
 *   Alan Hourihane <alanh@tungstengraphics.com>
 *
 */

/*
 * This server does not support these XFree86 4.0 features yet
 * shadowFb (if requested or acceleration is off)
 * Overlay planes
 * DGA
 */

#include <math.h>
#include <string.h>
#include <unistd.h>

/*
 * These are X and server generic header files.
 */
#include "xorg-server.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86cmap.h"

#include "compiler.h"
#include "vgaHW.h"
#include "mipointer.h"
#include "micmap.h"

#include "fb.h"
#include "miscstruct.h"
#include "xf86xv.h"
#include <X11/extensions/Xv.h>
#include "vbe.h"
#include "xf86fbman.h"

#include "i810.h"

#ifdef HAVE_DRI1
#include "dri.h"
#endif

#include "../legacy.h"

static Bool I810PreInit(ScrnInfoPtr pScrn, int flags);
static Bool I810ScreenInit(SCREEN_INIT_ARGS_DECL);
static Bool I810EnterVT(VT_FUNC_ARGS_DECL);
static void I810LeaveVT(VT_FUNC_ARGS_DECL);
static Bool I810CloseScreen(CLOSE_SCREEN_ARGS_DECL);
static Bool I810SaveScreen(ScreenPtr pScreen, Bool unblank);
static void I810FreeScreen(FREE_SCREEN_ARGS_DECL);
static void I810DisplayPowerManagementSet(ScrnInfoPtr pScrn,
					  int PowerManagermentMode,
					  int flags);
static ModeStatus I810ValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode,
				Bool verbose, int flags);

typedef enum {
   OPTION_NOACCEL,
   OPTION_SW_CURSOR,
   OPTION_COLOR_KEY,
   OPTION_CACHE_LINES,
   OPTION_DAC_6BIT,
   OPTION_DRI,
   OPTION_NO_DDC,
   OPTION_SHOW_CACHE,
   OPTION_XVMC_SURFACES,
   OPTION_PAGEFLIP
} I810Opts;
 
static const OptionInfoRec I810Options[] = {
   {OPTION_NOACCEL,		"NoAccel",	OPTV_BOOLEAN,	{0}, FALSE},
   {OPTION_SW_CURSOR,		"SWcursor",	OPTV_BOOLEAN,	{0}, FALSE},
   {OPTION_COLOR_KEY,		"ColorKey",	OPTV_INTEGER,	{0}, FALSE},
   {OPTION_CACHE_LINES,		"CacheLines",	OPTV_INTEGER,	{0}, FALSE},
   {OPTION_DAC_6BIT,		"Dac6Bit",	OPTV_BOOLEAN,	{0}, FALSE},
   {OPTION_DRI,			"DRI",		OPTV_BOOLEAN,	{0}, FALSE},
   {OPTION_NO_DDC,		"NoDDC",	OPTV_BOOLEAN,	{0}, FALSE},
   {OPTION_SHOW_CACHE,		"ShowCache",	OPTV_BOOLEAN,	{0}, FALSE},
   {OPTION_XVMC_SURFACES,	"XvMCSurfaces",	OPTV_INTEGER,	{0}, FALSE},
   {OPTION_PAGEFLIP,            "PageFlip",     OPTV_BOOLEAN, {0},   FALSE},
   {-1,				NULL,		OPTV_NONE,	{0}, FALSE}
};
/* *INDENT-ON* */

#ifndef I810_DEBUG
int I810_DEBUG = (0
/*     		  | DEBUG_ALWAYS_SYNC  */
/*    		  | DEBUG_VERBOSE_ACCEL */
/*  		  | DEBUG_VERBOSE_SYNC */
/*  		  | DEBUG_VERBOSE_VGA */
/*  		  | DEBUG_VERBOSE_RING */
/*  		  | DEBUG_VERBOSE_OUTREG */
/*  		  | DEBUG_VERBOSE_MEMORY */
/*  		  | DEBUG_VERBOSE_CURSOR */
      );
#endif

#ifdef HAVE_DRI1
static int i810_pitches[] = {
   512,
   1024,
   2048,
   4096,
   0
};
#endif

/*
 * I810GetRec and I810FreeRec --
 *
 * Private data for the driver is stored in the screen structure.
 * These two functions create and destroy that private data.
 *
 */
static Bool
I810GetRec(ScrnInfoPtr scrn)
{
   if (((uintptr_t)scrn->driverPrivate & 3) == 0)
      return TRUE;

   scrn->driverPrivate = xnfcalloc(sizeof(I810Rec), 1);
   return TRUE;
}

static void
I810FreeRec(ScrnInfoPtr scrn)
{
   if (!scrn)
      return;
   if (!scrn->driverPrivate)
      return;
   free(scrn->driverPrivate);
   scrn->driverPrivate = NULL;
}

struct pci_device *
intel_host_bridge (void)
{
    static const struct pci_slot_match bridge_match = {
	0, 0, 0, PCI_MATCH_ANY, 0
    };
    struct pci_device_iterator	*slot_iterator;
    struct pci_device		*bridge;

    slot_iterator = pci_slot_match_iterator_create (&bridge_match);
    bridge = pci_device_next (slot_iterator);
    pci_iterator_destroy (slot_iterator);
    return bridge;
}

static void
I810ProbeDDC(ScrnInfoPtr scrn, int index)
{
   vbeInfoPtr pVbe;

   if (xf86LoadSubModule(scrn, "vbe")) {
      pVbe = VBEInit(NULL, index);
      ConfiguredMonitor = vbeDoEDID(pVbe, NULL);
      vbeFree(pVbe);
   }
}

static xf86MonPtr
I810DoDDC(ScrnInfoPtr scrn, int index)
{
   vbeInfoPtr pVbe;
   xf86MonPtr MonInfo = NULL;
   I810Ptr pI810 = I810PTR(scrn);

   /* Honour Option "noDDC" */
   if (xf86ReturnOptValBool(pI810->Options, OPTION_NO_DDC, FALSE)) {
      return MonInfo;
   }

   if (xf86LoadSubModule(scrn, "vbe") && (pVbe = VBEInit(NULL, index))) {
      MonInfo = vbeDoEDID(pVbe, NULL);
      xf86PrintEDID(MonInfo);
      xf86SetDDCproperties(scrn, MonInfo);
      vbeFree(pVbe);
   } else {
      xf86DrvMsg(scrn->scrnIndex, X_INFO,
		 "this driver cannot do DDC without VBE\n");
   }

   return MonInfo;
}

/*
 * I810PreInit --
 *
 * Do initial setup of the board before we know what resolution we will
 * be running at.
 *
 */
static Bool
I810PreInit(ScrnInfoPtr scrn, int flags)
{
   I810Ptr pI810;
   ClockRangePtr clockRanges;
   int i;
   MessageType from;
   int flags24;
   rgb defaultWeight = { 0, 0, 0 };
   int mem;
   Bool enable;

   if (scrn->numEntities != 1)
      return FALSE;

   /* Allocate driverPrivate */
   if (!I810GetRec(scrn))
      return FALSE;

   pI810 = I810PTR(scrn);

   pI810->pEnt = xf86GetEntityInfo(scrn->entityList[0]);
   if (pI810->pEnt == NULL || pI810->pEnt->location.type != BUS_PCI)
      return FALSE;

   if (flags & PROBE_DETECT) {
      I810ProbeDDC(scrn, pI810->pEnt->index);
      return TRUE;
   }

   /* The vgahw module should be loaded here when needed */
   if (!xf86LoadSubModule(scrn, "vgahw"))
      return FALSE;

   /* Allocate a vgaHWRec */
   if (!vgaHWGetHWRec(scrn))
      return FALSE;

   pI810->PciInfo = xf86GetPciInfoForEntity(pI810->pEnt->index);

   /* Set scrn->monitor */
   scrn->monitor = scrn->confScreen->monitor;

   flags24 = Support24bppFb | PreferConvert32to24 | SupportConvert32to24;
   if (!xf86SetDepthBpp(scrn, 16, 0, 16, flags24)) {
      return FALSE;
   } else {
      switch (scrn->depth) {
      case 8:
      case 15:
      case 16:
      case 24:
	 break;
      default:
	 xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		    "Given depth (%d) is not supported by i810 driver\n",
		    scrn->depth);
	 return FALSE;
      }
   }
   xf86PrintDepthBpp(scrn);

   switch (scrn->bitsPerPixel) {
   case 8:
   case 16:
   case 24:
      break;
   default:
      xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		 "Given bpp (%d) is not supported by i810 driver\n",
		 scrn->bitsPerPixel);
      return FALSE;
   }

   if (!xf86SetWeight(scrn, defaultWeight, defaultWeight))
      return FALSE;

   if (!xf86SetDefaultVisual(scrn, -1))
      return FALSE;

   /* We use a programmable clock */
   scrn->progClock = TRUE;

   pI810->cpp = scrn->bitsPerPixel / 8;

   /* Process the options */
   xf86CollectOptions(scrn, NULL);
   if (!(pI810->Options = malloc(sizeof(I810Options))))
      return FALSE;
   memcpy(pI810->Options, I810Options, sizeof(I810Options));
   xf86ProcessOptions(scrn->scrnIndex, scrn->options, pI810->Options);

   scrn->rgbBits = 8;
   if (xf86ReturnOptValBool(pI810->Options, OPTION_DAC_6BIT, FALSE))
      scrn->rgbBits = 6;

   if (xf86ReturnOptValBool(pI810->Options, OPTION_SHOW_CACHE, FALSE))
     pI810->showCache = TRUE;
   else
     pI810->showCache = FALSE;

   /* 6-BIT dac isn't reasonable for modes with > 8bpp */
   if (xf86ReturnOptValBool(pI810->Options, OPTION_DAC_6BIT, FALSE) &&
       scrn->bitsPerPixel > 8) {
      OptionInfoPtr ptr;

      ptr = xf86TokenToOptinfo(pI810->Options, OPTION_DAC_6BIT);
      ptr->found = FALSE;
   }

   if (xf86ReturnOptValBool(pI810->Options, OPTION_NOACCEL, FALSE))
      pI810->noAccel = TRUE;

   if (!pI810->noAccel && !xf86LoadSubModule(scrn, "xaa"))
      pI810->noAccel = TRUE;
   
#ifdef HAVE_DRI1
   pI810->directRenderingDisabled =
     !xf86ReturnOptValBool(pI810->Options, OPTION_DRI, TRUE);

   if (!pI810->directRenderingDisabled) {
     if (scrn->depth!=16) {
       xf86DrvMsg(scrn->scrnIndex, X_WARNING, "DRI is disabled because it "
		  "runs only at 16-bit depth.\n");
       pI810->directRenderingDisabled=TRUE;
     }
   }
#endif

   /* Get DDC info from monitor */
   /* after xf86ProcessOptions,
    * because it is controlled by options [no]vbe and [no]ddc
    */
   I810DoDDC(scrn, pI810->pEnt->index);

   intel_detect_chipset(scrn, pI810->pEnt);

   pI810->LinearAddr = pI810->PciInfo->regions[0].base_addr;
   xf86DrvMsg(scrn->scrnIndex, X_PROBED, "Linear framebuffer at 0x%lX\n",
	      (unsigned long)pI810->LinearAddr);

   pI810->MMIOAddr = pI810->PciInfo->regions[1].base_addr;
   xf86DrvMsg(scrn->scrnIndex, X_PROBED, "IO registers at addr 0x%lX\n",
	      (unsigned long)pI810->MMIOAddr);

   /* AGP GART support is required.  Don't proceed any further if it isn't
    * present.
    */
   if (!xf86AgpGARTSupported()) {
      xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		 "AGP GART support is not available.  Make sure your kernel has\n"
		 "\tagpgart support or that the agpgart kernel module is loaded.\n");
      return FALSE;
   }

   /* Find out memory bus frequency.
    */
   {
      uint32_t whtcfg_pamr_drp;
    
      pci_device_cfg_read_u32(pI810->PciInfo, & whtcfg_pamr_drp, WHTCFG_PAMR_DRP);

      /* Need this for choosing watermarks.
       */
      if ((whtcfg_pamr_drp & LM_FREQ_MASK) == LM_FREQ_133)
	 pI810->LmFreqSel = 133;
      else
	 pI810->LmFreqSel = 100;
   }

   /* Default to 4MB framebuffer, which is sufficient for all
    * supported 2d resolutions.  If the user has specified a different
    * size in the XF86Config, use that amount instead.
    *
    *  Changed to 8 Meg so we can have acceleration by default (Mark).
    */
   mem = I810CheckAvailableMemory(scrn);
   if (pI810->directRenderingDisabled || mem < 131072)  /* < 128 MB */
       scrn->videoRam = 8192;
   else if (mem < 196608)
       scrn->videoRam = 16384;  /* < 192 MB */
   else
       scrn->videoRam = 24576;
   
   from = X_DEFAULT;
   
   if (pI810->pEnt->device->videoRam) {
      scrn->videoRam = pI810->pEnt->device->videoRam;
      from = X_CONFIG;
   }

   if (mem > 0 && mem < scrn->videoRam) {
      xf86DrvMsg(scrn->scrnIndex, X_WARNING, "%dk of memory was requested,"
		 " but the\n\t maximum AGP memory available is %dk.\n",
		 scrn->videoRam, mem);
      from = X_PROBED;
      if (mem > (6 * 1024)) {
	 xf86DrvMsg(scrn->scrnIndex, X_INFO,
		    "Reducing video memory to 4MB\n");
	 scrn->videoRam = 4096;
      } else {
	 xf86DrvMsg(scrn->scrnIndex, X_ERROR, "Less than 6MB of AGP memory"
		    " is available. Cannot proceed.\n");
	 I810FreeRec(scrn);
	 return FALSE;
      }
   }

   xf86DrvMsg(scrn->scrnIndex, from,
	      "Will alloc AGP framebuffer: %d kByte\n", scrn->videoRam);

   /* Calculate Fixed Offsets depending on graphics aperture size */
   {
      struct pci_device *bridge = intel_host_bridge ();
      uint32_t   smram_miscc;
      
      pci_device_cfg_read_u32 (bridge, & smram_miscc, SMRAM_MISCC);

      if ((smram_miscc & GFX_MEM_WIN_SIZE) == GFX_MEM_WIN_32M) {
	 pI810->FbMapSize = 0x1000000;
	 pI810->DepthOffset = 0x1000000;
	 pI810->BackOffset = 0x1800000;
      } else {
	 pI810->FbMapSize = 0x3000000;
	 pI810->DepthOffset = 0x3000000;
	 pI810->BackOffset = 0x3800000;
      }
   }

   /*
    * If the driver can do gamma correction, it should call xf86SetGamma()
    * here.
    */
   {
      Gamma zeros = { 0.0, 0.0, 0.0 };

      if (!xf86SetGamma(scrn, zeros)) {
	 return FALSE;
      }
   }

   pI810->MaxClock = 0;
   if (pI810->pEnt->device->dacSpeeds[0]) {
      switch (scrn->bitsPerPixel) {
      case 8:
	 pI810->MaxClock = pI810->pEnt->device->dacSpeeds[DAC_BPP8];
	 break;
      case 16:
	 pI810->MaxClock = pI810->pEnt->device->dacSpeeds[DAC_BPP16];
	 break;
      case 24:
	 pI810->MaxClock = pI810->pEnt->device->dacSpeeds[DAC_BPP24];
	 break;
      case 32:				/* not supported */
	 pI810->MaxClock = pI810->pEnt->device->dacSpeeds[DAC_BPP32];
	 break;
      }
      if (!pI810->MaxClock)
	 pI810->MaxClock = pI810->pEnt->device->dacSpeeds[0];
   } else {
      switch (scrn->bitsPerPixel) {
      case 8:
	 pI810->MaxClock = 203000;
	 break;
      case 16:
	 pI810->MaxClock = 163000;
	 break;
      case 24:
	 pI810->MaxClock = 136000;
	 break;
      case 32:				/* not supported */
	 pI810->MaxClock = 86000;
      }
   }
   clockRanges = xnfcalloc(sizeof(ClockRange), 1);
   clockRanges->next = NULL;
   /* 9.4MHz appears to be the smallest that works. */
   clockRanges->minClock = 9500;
   clockRanges->maxClock = pI810->MaxClock;
   clockRanges->clockIndex = -1;
   clockRanges->interlaceAllowed = TRUE;
   clockRanges->doubleScanAllowed = FALSE;

   i = xf86ValidateModes(scrn, scrn->monitor->Modes,
			 scrn->display->modes, clockRanges,
#ifndef HAVE_DRI1
			 0, 320, 1600, 64 * scrn->bitsPerPixel,
#else
			 i810_pitches, 0, 0, 64 * scrn->bitsPerPixel,
#endif
			 200, 1200,
			 scrn->display->virtualX, scrn->display->virtualY,
			 scrn->videoRam * 1024, LOOKUP_BEST_REFRESH);

   if (i == -1) {
      I810FreeRec(scrn);
      return FALSE;
   }

   xf86PruneDriverModes(scrn);

   if (!i || !scrn->modes) {
      xf86DrvMsg(scrn->scrnIndex, X_ERROR, "No valid modes found\n");
      I810FreeRec(scrn);
      return FALSE;
   }

   xf86SetCrtcForModes(scrn, INTERLACE_HALVE_V);

   scrn->currentMode = scrn->modes;

   xf86PrintModes(scrn);

   xf86SetDpi(scrn, 0, 0);

   if (!xf86LoadSubModule(scrn, "fb")) {
      I810FreeRec(scrn);
      return FALSE;
   }

   if (!xf86ReturnOptValBool(pI810->Options, OPTION_SW_CURSOR, FALSE)) {
      if (!xf86LoadSubModule(scrn, "ramdac")) {
	 I810FreeRec(scrn);
	 return FALSE;
      }
   }

   if (xf86GetOptValInteger
       (pI810->Options, OPTION_COLOR_KEY, &(pI810->colorKey))) {
      xf86DrvMsg(scrn->scrnIndex, X_CONFIG,
		 "video overlay key set to 0x%x\n", pI810->colorKey);
   } else {
      pI810->colorKey = (1 << scrn->offset.red) |
	    (1 << scrn->offset.green) |
	    (((scrn->mask.blue >> scrn->offset.blue) -
	      1) << scrn->offset.blue);
   }

   pI810->allowPageFlip=FALSE;
   enable = xf86ReturnOptValBool(pI810->Options, OPTION_PAGEFLIP, FALSE);   

#ifdef HAVE_DRI1
   if (!pI810->directRenderingDisabled) {
     pI810->allowPageFlip = enable;
     if (pI810->allowPageFlip == TRUE)
     {
       if (!xf86LoadSubModule(scrn, "shadowfb")) {
	 pI810->allowPageFlip = 0;
	 xf86DrvMsg(scrn->scrnIndex, X_ERROR, 
		    "Couldn't load shadowfb module:\n");
       }
     }
     
     xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "page flipping %s\n",
		enable ? "enabled" : "disabled");
     
   }
#endif

   if (xf86GetOptValInteger(pI810->Options, OPTION_XVMC_SURFACES,
			    &(pI810->numSurfaces))) {
      xf86DrvMsg(scrn->scrnIndex, X_CONFIG, "%d XvMC Surfaces Requested.\n",
		 pI810->numSurfaces);
      if (pI810->numSurfaces > 7) {
	 xf86DrvMsg(scrn->scrnIndex, X_PROBED,
		    "Using 7 XvMC Surfaces (Maximum Allowed).\n");
	 pI810->numSurfaces = 7;
      }
      if (pI810->numSurfaces < 6) {
	 xf86DrvMsg(scrn->scrnIndex, X_PROBED,
		    "Using 6 XvMC Surfaces (Minimum Allowed).\n");
	 pI810->numSurfaces = 6;
      }
   } else {
      xf86DrvMsg(scrn->scrnIndex, X_INFO,
		 "XvMC is Disabled: use XvMCSurfaces config option to enable.\n");
      pI810->numSurfaces = 0;
   }

#ifdef HAVE_DRI1
   /* Load the dri module if requested. */
   if (xf86ReturnOptValBool(pI810->Options, OPTION_DRI, FALSE)) {
      xf86LoadSubModule(scrn, "dri");
   }
#endif

   return TRUE;
}

static Bool
I810MapMMIO(ScrnInfoPtr scrn)
{
   I810Ptr pI810 = I810PTR(scrn);
   struct pci_device *const device = pI810->PciInfo;
   int err;

   err = pci_device_map_range (device,
			       pI810->MMIOAddr,
			       I810_REG_SIZE,
			       PCI_DEV_MAP_FLAG_WRITABLE,
			       (void **) &pI810->MMIOBase);
   if (err) 
   {
      xf86DrvMsg (scrn->scrnIndex, X_ERROR,
		  "Unable to map mmio BAR. %s (%d)\n",
		  strerror (err), err);
      return FALSE;
   }
   return TRUE;
}

static Bool
I810MapMem(ScrnInfoPtr scrn)
{
   I810Ptr pI810 = I810PTR(scrn);
   struct pci_device *const device = pI810->PciInfo;
   int err;

   if (!I810MapMMIO(scrn))
      return FALSE;

   err = pci_device_map_range (device,
			       pI810->LinearAddr,
			       pI810->FbMapSize,
			       PCI_DEV_MAP_FLAG_WRITABLE | PCI_DEV_MAP_FLAG_WRITE_COMBINE,
			       (void **) &pI810->FbBase);
   if (err) 
   {
      xf86DrvMsg (scrn->scrnIndex, X_ERROR,
		  "Unable to map frame buffer BAR. %s (%d)\n",
		  strerror (err), err);
      return FALSE;
   }

   pI810->LpRing->virtual_start = pI810->FbBase + pI810->LpRing->mem.Start;

   return TRUE;
}

static void
I810UnmapMMIO(ScrnInfoPtr scrn)
{
   I810Ptr pI810 = I810PTR(scrn);

   pci_device_unmap_range (pI810->PciInfo, pI810->MMIOBase, I810_REG_SIZE);
   pI810->MMIOBase = NULL;
}

static Bool
I810UnmapMem(ScrnInfoPtr scrn)
{
   I810Ptr pI810 = I810PTR(scrn);

   pci_device_unmap_range (pI810->PciInfo, pI810->FbBase, pI810->FbMapSize);
   pI810->FbBase = NULL;
   I810UnmapMMIO(scrn);
   return TRUE;
}

/* Famous last words
 */
void
I810PrintErrorState(ScrnInfoPtr scrn)
{
   I810Ptr pI810 = I810PTR(scrn);

   ErrorF("pgetbl_ctl: 0x%lx pgetbl_err: 0x%lx\n",
	  (unsigned long) INREG(PGETBL_CTL), (unsigned long) INREG(PGE_ERR));

   ErrorF("ipeir: %lx iphdr: %lx\n", (unsigned long) INREG(IPEIR), 
	  (unsigned long) INREG(IPEHR));

   ErrorF("LP ring tail: %lx head: %lx len: %lx start %lx\n",
	  (unsigned long) INREG(LP_RING + RING_TAIL),
	  (unsigned long) INREG(LP_RING + RING_HEAD) & HEAD_ADDR,
	  (unsigned long) INREG(LP_RING + RING_LEN), 
	  (unsigned long) INREG(LP_RING + RING_START));

   ErrorF("eir: %x esr: %x emr: %x\n",
	  INREG16(EIR), INREG16(ESR), INREG16(EMR));

   ErrorF("instdone: %x instpm: %x\n", INREG16(INST_DONE), INREG8(INST_PM));

   ErrorF("memmode: %lx instps: %lx\n", (unsigned long) INREG(MEMMODE), 
	  (unsigned long) INREG(INST_PS));

   ErrorF("hwstam: %x ier: %x imr: %x iir: %x\n",
	  INREG16(HWSTAM), INREG16(IER), INREG16(IMR), INREG16(IIR));
}

/*
 * I810Save --
 *
 * This function saves the video state.  It reads all of the SVGA registers
 * into the vgaI810Rec data structure.  There is in general no need to
 * mask out bits here - just read the registers.
 */
static void
DoSave(ScrnInfoPtr scrn, vgaRegPtr vgaReg, I810RegPtr i810Reg,
       Bool saveFonts)
{
   I810Ptr pI810;
   vgaHWPtr hwp;
   int i;

   pI810 = I810PTR(scrn);
   hwp = VGAHWPTR(scrn);

   /*
    * This function will handle creating the data structure and filling
    * in the generic VGA portion.
    */
   if (saveFonts)
      vgaHWSave(scrn, vgaReg, VGA_SR_MODE | VGA_SR_FONTS | VGA_SR_CMAP);
   else
      vgaHWSave(scrn, vgaReg, VGA_SR_MODE | VGA_SR_CMAP);

   /*
    * The port I/O code necessary to read in the extended registers
    * into the fields of the vgaI810Rec structure goes here.
    */
   i810Reg->IOControl = hwp->readCrtc(hwp, IO_CTNL);
   i810Reg->AddressMapping = hwp->readGr(hwp, ADDRESS_MAPPING);
   i810Reg->BitBLTControl = INREG8(BITBLT_CNTL);
   i810Reg->VideoClk2_M = INREG16(VCLK2_VCO_M);
   i810Reg->VideoClk2_N = INREG16(VCLK2_VCO_N);
   i810Reg->VideoClk2_DivisorSel = INREG8(VCLK2_VCO_DIV_SEL);

   i810Reg->ExtVertTotal = hwp->readCrtc(hwp, EXT_VERT_TOTAL);
   i810Reg->ExtVertDispEnd = hwp->readCrtc(hwp, EXT_VERT_DISPLAY);
   i810Reg->ExtVertSyncStart = hwp->readCrtc(hwp, EXT_VERT_SYNC_START);
   i810Reg->ExtVertBlankStart = hwp->readCrtc(hwp, EXT_VERT_BLANK_START);
   i810Reg->ExtHorizTotal = hwp->readCrtc(hwp, EXT_HORIZ_TOTAL);
   i810Reg->ExtHorizBlank = hwp->readCrtc(hwp, EXT_HORIZ_BLANK);
   i810Reg->ExtOffset = hwp->readCrtc(hwp, EXT_OFFSET);
   i810Reg->InterlaceControl = hwp->readCrtc(hwp, INTERLACE_CNTL);

   i810Reg->PixelPipeCfg0 = INREG8(PIXPIPE_CONFIG_0);
   i810Reg->PixelPipeCfg1 = INREG8(PIXPIPE_CONFIG_1);
   i810Reg->PixelPipeCfg2 = INREG8(PIXPIPE_CONFIG_2);
   i810Reg->DisplayControl = INREG8(DISPLAY_CNTL);
   i810Reg->LMI_FIFO_Watermark = INREG(FWATER_BLC);

   for (i = 0; i < 8; i++)
      i810Reg->Fence[i] = INREG(FENCE + i * 4);

   i810Reg->LprbTail = INREG(LP_RING + RING_TAIL);
   i810Reg->LprbHead = INREG(LP_RING + RING_HEAD);
   i810Reg->LprbStart = INREG(LP_RING + RING_START);
   i810Reg->LprbLen = INREG(LP_RING + RING_LEN);

   if ((i810Reg->LprbTail & TAIL_ADDR) != (i810Reg->LprbHead & HEAD_ADDR) &&
       i810Reg->LprbLen & RING_VALID) {
      I810PrintErrorState(scrn);
      FatalError("Active ring not flushed\n");
   }
}

static void
I810Save(ScrnInfoPtr scrn)
{
   vgaHWPtr hwp;
   I810Ptr pI810;
   uint32_t temp;

   hwp = VGAHWPTR(scrn);
   pI810 = I810PTR(scrn);
   DoSave(scrn, &hwp->SavedReg, &pI810->SavedReg, TRUE);

   temp = INREG(MEMMODE);
   temp |= 4;
   OUTREG(MEMMODE, temp);
}

static void
i810PrintMode(vgaRegPtr vgaReg, I810RegPtr mode)
{
   int i;

   ErrorF("   MiscOut: %x\n", vgaReg->MiscOutReg);

   ErrorF("SEQ: ");
   for (i = 0; i < vgaReg->numSequencer; i++) {
      if ((i & 7) == 0)
	 ErrorF("\n");
      ErrorF("   %d: %x", i, vgaReg->Sequencer[i]);
   }
   ErrorF("\n");

   ErrorF("CRTC: ");
   for (i = 0; i < vgaReg->numCRTC; i++) {
      if ((i & 3) == 0)
	 ErrorF("\n");
      ErrorF("   %d: %x", i, vgaReg->CRTC[i]);
   }
   ErrorF("\n");

   ErrorF("GFX: ");
   for (i = 0; i < vgaReg->numGraphics; i++) {
      if ((i & 7) == 0)
	 ErrorF("\n");
      ErrorF("   %d: %x", i, vgaReg->Graphics[i]);
   }
   ErrorF("\n");

   ErrorF("ATTR: ");
   for (i = 0; i < vgaReg->numAttribute; i++) {
      if ((i & 7) == 0)
	 ErrorF("\n");
      ErrorF("   %d: %x", i, vgaReg->Attribute[i]);
   }
   ErrorF("\n");

   ErrorF("   DisplayControl: %x\n", mode->DisplayControl);
   ErrorF("   PixelPipeCfg0: %x\n", mode->PixelPipeCfg0);
   ErrorF("   PixelPipeCfg1: %x\n", mode->PixelPipeCfg1);
   ErrorF("   PixelPipeCfg2: %x\n", mode->PixelPipeCfg2);
   ErrorF("   VideoClk2_M: %x\n", mode->VideoClk2_M);
   ErrorF("   VideoClk2_N: %x\n", mode->VideoClk2_N);
   ErrorF("   VideoClk2_DivisorSel: %x\n", mode->VideoClk2_DivisorSel);
   ErrorF("   AddressMapping: %x\n", mode->AddressMapping);
   ErrorF("   IOControl: %x\n", mode->IOControl);
   ErrorF("   BitBLTControl: %x\n", mode->BitBLTControl);
   ErrorF("   ExtVertTotal: %x\n", mode->ExtVertTotal);
   ErrorF("   ExtVertDispEnd: %x\n", mode->ExtVertDispEnd);
   ErrorF("   ExtVertSyncStart: %x\n", mode->ExtVertSyncStart);
   ErrorF("   ExtVertBlankStart: %x\n", mode->ExtVertBlankStart);
   ErrorF("   ExtHorizTotal: %x\n", mode->ExtHorizTotal);
   ErrorF("   ExtHorizBlank: %x\n", mode->ExtHorizBlank);
   ErrorF("   ExtOffset: %x\n", mode->ExtOffset);
   ErrorF("   InterlaceControl: %x\n", mode->InterlaceControl);
   ErrorF("   LMI_FIFO_Watermark: %x\n", mode->LMI_FIFO_Watermark);
   ErrorF("   LprbTail: %x\n", mode->LprbTail);
   ErrorF("   LprbHead: %x\n", mode->LprbHead);
   ErrorF("   LprbStart: %x\n", mode->LprbStart);
   ErrorF("   LprbLen: %x\n", mode->LprbLen);
}

static void
DoRestore(ScrnInfoPtr scrn, vgaRegPtr vgaReg, I810RegPtr i810Reg,
	  Bool restoreFonts)
{
   I810Ptr pI810;
   vgaHWPtr hwp;
   unsigned char temp;
   unsigned int itemp;
   int i;

   pI810 = I810PTR(scrn);
   hwp = VGAHWPTR(scrn);

   if (I810_DEBUG & DEBUG_VERBOSE_VGA) {
      ErrorF("Setting mode in I810Restore:\n");
      i810PrintMode(vgaReg, i810Reg);
   }

   vgaHWProtect(scrn, TRUE);

   usleep(50000);

   /* Turn off DRAM Refresh */
   temp = INREG8(DRAM_ROW_CNTL_HI);
   temp &= ~DRAM_REFRESH_RATE;
   temp |= DRAM_REFRESH_DISABLE;
   OUTREG8(DRAM_ROW_CNTL_HI, temp);

   usleep(1000);			/* Wait 1 ms */

   /* Write the M, N and P values */
   OUTREG16(VCLK2_VCO_M, i810Reg->VideoClk2_M);
   OUTREG16(VCLK2_VCO_N, i810Reg->VideoClk2_N);
   OUTREG8(VCLK2_VCO_DIV_SEL, i810Reg->VideoClk2_DivisorSel);

   /*
    * Turn on 8 bit dac mode, if requested.  This is needed to make
    * sure that vgaHWRestore writes the values into the DAC properly.
    * The problem occurs if 8 bit dac mode is requested and the HW is
    * in 6 bit dac mode.  If this happens, all the values are
    * automatically shifted left twice by the HW and incorrect colors
    * will be displayed on the screen.  The only time this can happen
    * is at server startup time and when switching back from a VT.
    */
   temp = INREG8(PIXPIPE_CONFIG_0);
   temp &= 0x7F;			/* Save all but the 8 bit dac mode bit */
   temp |= (i810Reg->PixelPipeCfg0 & DAC_8_BIT);
   OUTREG8(PIXPIPE_CONFIG_0, temp);

   /*
    * Code to restore any SVGA registers that have been saved/modified
    * goes here.  Note that it is allowable, and often correct, to
    * only modify certain bits in a register by a read/modify/write cycle.
    *
    * A special case - when using an external clock-setting program,
    * this function must not change bits associated with the clock
    * selection.  This condition can be checked by the condition:
    *
    *   if (i810Reg->std.NoClock >= 0)
    *           restore clock-select bits.
    */
   if (restoreFonts)
      vgaHWRestore(scrn, vgaReg, VGA_SR_FONTS | VGA_SR_MODE | VGA_SR_CMAP);
   else
      vgaHWRestore(scrn, vgaReg, VGA_SR_MODE | VGA_SR_CMAP);

   hwp->writeCrtc(hwp, EXT_VERT_TOTAL, i810Reg->ExtVertTotal);
   hwp->writeCrtc(hwp, EXT_VERT_DISPLAY, i810Reg->ExtVertDispEnd);
   hwp->writeCrtc(hwp, EXT_VERT_SYNC_START, i810Reg->ExtVertSyncStart);
   hwp->writeCrtc(hwp, EXT_VERT_BLANK_START, i810Reg->ExtVertBlankStart);
   hwp->writeCrtc(hwp, EXT_HORIZ_TOTAL, i810Reg->ExtHorizTotal);
   hwp->writeCrtc(hwp, EXT_HORIZ_BLANK, i810Reg->ExtHorizBlank);
   hwp->writeCrtc(hwp, EXT_OFFSET, i810Reg->ExtOffset);

   temp = hwp->readCrtc(hwp, INTERLACE_CNTL);
   temp &= ~INTERLACE_ENABLE;
   temp |= i810Reg->InterlaceControl;
   hwp->writeCrtc(hwp, INTERLACE_CNTL, temp);

   temp = hwp->readGr(hwp, ADDRESS_MAPPING);
   temp &= 0xE0;			/* Save reserved bits 7:5 */
   temp |= i810Reg->AddressMapping;
   hwp->writeGr(hwp, ADDRESS_MAPPING, temp);

   /* Setting the OVRACT Register for video overlay */
   {
       uint32_t LCD_TV_Control = INREG(LCD_TV_C);
       uint32_t TV_HTotal = INREG(LCD_TV_HTOTAL);
       uint32_t ActiveStart, ActiveEnd;

       if((LCD_TV_Control & LCD_TV_ENABLE)
	  && !(LCD_TV_Control & LCD_TV_VGAMOD)
	   && TV_HTotal) {
	   ActiveStart = ((TV_HTotal >> 16) & 0xfff) - 31;
	   ActiveEnd = (TV_HTotal & 0x3ff) - 31;
       } else {
	   ActiveStart = i810Reg->OverlayActiveStart;
	   ActiveEnd = i810Reg->OverlayActiveEnd;
       }
       OUTREG(LCD_TV_OVRACT,
	      (ActiveEnd << 16) | ActiveStart);
   }

   /* Turn on DRAM Refresh */
   temp = INREG8(DRAM_ROW_CNTL_HI);
   temp &= ~DRAM_REFRESH_RATE;
   temp |= DRAM_REFRESH_60HZ;
   OUTREG8(DRAM_ROW_CNTL_HI, temp);

   temp = INREG8(BITBLT_CNTL);
   temp &= ~COLEXP_MODE;
   temp |= i810Reg->BitBLTControl;
   OUTREG8(BITBLT_CNTL, temp);

   temp = INREG8(DISPLAY_CNTL);
   temp &= ~(VGA_WRAP_MODE | GUI_MODE);
   temp |= i810Reg->DisplayControl;
   OUTREG8(DISPLAY_CNTL, temp);

   temp = INREG8(PIXPIPE_CONFIG_0);
   temp &= 0x64;			/* Save reserved bits 6:5,2 */
   temp |= i810Reg->PixelPipeCfg0;
   OUTREG8(PIXPIPE_CONFIG_0, temp);

   temp = INREG8(PIXPIPE_CONFIG_2);
   temp &= 0xF3;			/* Save reserved bits 7:4,1:0 */
   temp |= i810Reg->PixelPipeCfg2;
   OUTREG8(PIXPIPE_CONFIG_2, temp);

   temp = INREG8(PIXPIPE_CONFIG_1);
   temp &= ~DISPLAY_COLOR_MODE;
   temp &= 0xEF;			/* Restore the CRT control bit */
   temp |= i810Reg->PixelPipeCfg1;
   OUTREG8(PIXPIPE_CONFIG_1, temp);

   OUTREG16(EIR, 0);

   itemp = INREG(FWATER_BLC);
   itemp &= ~(LM_BURST_LENGTH | LM_FIFO_WATERMARK |
	      MM_BURST_LENGTH | MM_FIFO_WATERMARK);
   itemp |= i810Reg->LMI_FIFO_Watermark;
   OUTREG(FWATER_BLC, itemp);

   for (i = 0; i < 8; i++) {
      OUTREG(FENCE + i * 4, i810Reg->Fence[i]);
      if (I810_DEBUG & DEBUG_VERBOSE_VGA)
	 ErrorF("Fence Register : %x\n", i810Reg->Fence[i]);
   }

   /* First disable the ring buffer (Need to wait for empty first?, if so
    * should probably do it before entering this section)
    */
   itemp = INREG(LP_RING + RING_LEN);
   itemp &= ~RING_VALID_MASK;
   OUTREG(LP_RING + RING_LEN, itemp);

   /* Set up the low priority ring buffer.
    */
   OUTREG(LP_RING + RING_TAIL, 0);
   OUTREG(LP_RING + RING_HEAD, 0);

   pI810->LpRing->head = 0;
   pI810->LpRing->tail = 0;

   itemp = INREG(LP_RING + RING_START);
   itemp &= ~(START_ADDR);
   itemp |= i810Reg->LprbStart;
   OUTREG(LP_RING + RING_START, itemp);

   itemp = INREG(LP_RING + RING_LEN);
   itemp &= ~(RING_NR_PAGES | RING_REPORT_MASK | RING_VALID_MASK);
   itemp |= i810Reg->LprbLen;
   OUTREG(LP_RING + RING_LEN, itemp);

   if (!(vgaReg->Attribute[0x10] & 0x1)) {
      usleep(50000);
      if (restoreFonts)
	 vgaHWRestore(scrn, vgaReg,
		      VGA_SR_FONTS | VGA_SR_MODE | VGA_SR_CMAP);
      else
	 vgaHWRestore(scrn, vgaReg, VGA_SR_MODE | VGA_SR_CMAP);
   }

   vgaHWProtect(scrn, FALSE);

   temp = hwp->readCrtc(hwp, IO_CTNL);
   temp &= ~(EXTENDED_ATTR_CNTL | EXTENDED_CRTC_CNTL);
   temp |= i810Reg->IOControl;
   hwp->writeCrtc(hwp, IO_CTNL, temp);
}

static void
I810SetRingRegs(ScrnInfoPtr scrn)
{
   unsigned int itemp;
   I810Ptr pI810 = I810PTR(scrn);

   OUTREG(LP_RING + RING_TAIL, 0);
   OUTREG(LP_RING + RING_HEAD, 0);

   itemp = INREG(LP_RING + RING_START);
   itemp &= ~(START_ADDR);
   itemp |= pI810->LpRing->mem.Start;
   OUTREG(LP_RING + RING_START, itemp);

   itemp = INREG(LP_RING + RING_LEN);
   itemp &= ~(RING_NR_PAGES | RING_REPORT_MASK | RING_VALID_MASK);
   itemp |= ((pI810->LpRing->mem.Size - 4096) | RING_NO_REPORT | RING_VALID);
   OUTREG(LP_RING + RING_LEN, itemp);
}

static void
I810Restore(ScrnInfoPtr scrn)
{
   vgaHWPtr hwp;
   I810Ptr pI810;

   hwp = VGAHWPTR(scrn);
   pI810 = I810PTR(scrn);

   DoRestore(scrn, &hwp->SavedReg, &pI810->SavedReg, TRUE);
}

/*
 * I810CalcVCLK --
 *
 * Determine the closest clock frequency to the one requested.
 */

#define MAX_VCO_FREQ 600.0
#define TARGET_MAX_N 30
#define REF_FREQ 24.0

#define CALC_VCLK(m,n,p) \
    (double)m / ((double)n * (1 << p)) * 4 * REF_FREQ

static void
I810CalcVCLK(ScrnInfoPtr scrn, double freq)
{
   I810Ptr pI810 = I810PTR(scrn);
   I810RegPtr i810Reg = &pI810->ModeReg;
   int m, n, p;
   double f_out;
   double f_err;
   double f_vco;
   int m_best = 0, n_best = 0, p_best = 0;
   double f_target = freq;
   double err_max = 0.005;
   double err_target = 0.001;
   double err_best = 999999.0;

   p_best = p = log(MAX_VCO_FREQ / f_target) / log((double)2);
   /* Make sure p is within range. */
   if (p_best > 5) {
      p_best = p = 5;
   }

   f_vco = f_target * (1 << p);

   n = 2;
   do {
      n++;
      m = f_vco / (REF_FREQ / (double)n) / (double)4.0 + 0.5;
      if (m < 3)
	 m = 3;
      f_out = CALC_VCLK(m, n, p);
      f_err = 1.0 - (f_target / f_out);
      if (fabs(f_err) < err_max) {
	 m_best = m;
	 n_best = n;
	 err_best = f_err;
      }
   } while ((fabs(f_err) >= err_target) &&
	    ((n <= TARGET_MAX_N) || (fabs(err_best) > err_max)));

   if (fabs(f_err) < err_target) {
      m_best = m;
      n_best = n;
   }

   i810Reg->VideoClk2_M = (m_best - 2) & 0x3FF;
   i810Reg->VideoClk2_N = (n_best - 2) & 0x3FF;
   i810Reg->VideoClk2_DivisorSel = (p_best << 4);

   xf86DrvMsgVerb(scrn->scrnIndex, X_INFO, 3,
		  "Setting dot clock to %.1f MHz " "[ 0x%x 0x%x 0x%x ] "
		  "[ %d %d %d ]\n", CALC_VCLK(m_best, n_best, p_best),
		  i810Reg->VideoClk2_M, i810Reg->VideoClk2_N,
		  i810Reg->VideoClk2_DivisorSel, m_best, n_best, p_best);
}

static Bool
I810SetMode(ScrnInfoPtr scrn, DisplayModePtr mode)
{
   I810Ptr pI810 = I810PTR(scrn);
   I810RegPtr i810Reg = &pI810->ModeReg;
   vgaRegPtr pVga = &VGAHWPTR(scrn)->ModeReg;
   double dclk = mode->Clock / 1000.0;

   switch (scrn->bitsPerPixel) {
   case 8:
      pVga->CRTC[0x13] = scrn->displayWidth >> 3;
      i810Reg->ExtOffset = scrn->displayWidth >> 11;
      i810Reg->PixelPipeCfg1 = DISPLAY_8BPP_MODE;
      i810Reg->BitBLTControl = COLEXP_8BPP;
      break;
   case 16:
      if (scrn->weight.green == 5) {
	 i810Reg->PixelPipeCfg1 = DISPLAY_15BPP_MODE;
      } else {
	 i810Reg->PixelPipeCfg1 = DISPLAY_16BPP_MODE;
      }
      pVga->CRTC[0x13] = scrn->displayWidth >> 2;
      i810Reg->ExtOffset = scrn->displayWidth >> 10;
      i810Reg->BitBLTControl = COLEXP_16BPP;

      /* Enable Palette Programming for Direct Color visuals. -jens */
      i810Reg->PixelPipeCfg2 = DISPLAY_GAMMA_ENABLE;
      break;
   case 24:
      pVga->CRTC[0x13] = (scrn->displayWidth * 3) >> 3;
      i810Reg->ExtOffset = (scrn->displayWidth * 3) >> 11;

      i810Reg->PixelPipeCfg1 = DISPLAY_24BPP_MODE;
      i810Reg->BitBLTControl = COLEXP_24BPP;

      /* Enable Palette Programming for Direct Color visuals. -jens */
      i810Reg->PixelPipeCfg2 = DISPLAY_GAMMA_ENABLE;
      break;
   default:
      break;
   }

   /* Turn on 8 bit dac if requested */
   if (xf86ReturnOptValBool(pI810->Options, OPTION_DAC_6BIT, FALSE))
      i810Reg->PixelPipeCfg0 = DAC_6_BIT;
   else
      i810Reg->PixelPipeCfg0 = DAC_8_BIT;

   /* Do not delay CRT Blank: needed for video overlay */
   i810Reg->PixelPipeCfg1 |= 0x10;

   /* Turn on Extended VGA Interpretation */
   i810Reg->IOControl = EXTENDED_CRTC_CNTL;

   /* Turn on linear and page mapping */
   i810Reg->AddressMapping = (LINEAR_MODE_ENABLE | GTT_MEM_MAP_ENABLE);

   /* Turn on GUI mode */
   i810Reg->DisplayControl = HIRES_MODE;

   /* Calculate the extended CRTC regs */
   i810Reg->ExtVertTotal = (mode->CrtcVTotal - 2) >> 8;
   i810Reg->ExtVertDispEnd = (mode->CrtcVDisplay - 1) >> 8;
   i810Reg->ExtVertSyncStart = mode->CrtcVSyncStart >> 8;
   i810Reg->ExtVertBlankStart = mode->CrtcVBlankStart >> 8;
   i810Reg->ExtHorizTotal = ((mode->CrtcHTotal >> 3) - 5) >> 8;
   i810Reg->ExtHorizBlank = (((mode->CrtcHBlankEnd >> 3) - 1) & 0x40) >> 6;

   /*
    * the KGA fix in vgaHW.c results in the first
    * scanline and the first character clock (8 pixels)
    * of each scanline thereafter on display with an i810
    * to be blank. Restoring CRTC 3, 5, & 22 to their
    * "theoretical" values corrects the problem. KAO.
    */
   pVga->CRTC[3] = (((mode->CrtcHBlankEnd >> 3) - 1) & 0x1F) | 0x80;
   pVga->CRTC[5] = ((((mode->CrtcHBlankEnd >> 3) - 1) & 0x20) << 2)
	 | (((mode->CrtcHSyncEnd >> 3)) & 0x1F);
   pVga->CRTC[22] = (mode->CrtcVBlankEnd - 1) & 0xFF;

   i810Reg->ExtHorizBlank = vgaHWHBlankKGA(mode, pVga, 7, 0);
   vgaHWVBlankKGA(mode, pVga, 8, 0);

   /*
    * The following workarounds are needed to get video overlay working
    * at 1024x768 and 1280x1024 display resolutions.
    */
   if ((mode->CrtcVDisplay == 768) && (i810Reg->ExtVertBlankStart == 3)) {
      i810Reg->ExtVertBlankStart = 2;
   }
   if ((mode->CrtcVDisplay == 1024) && (i810Reg->ExtVertBlankStart == 4)) {
      i810Reg->ExtVertBlankStart = 3;
   }

   /* OVRACT Register */
   i810Reg->OverlayActiveStart = mode->CrtcHTotal - 32;
   i810Reg->OverlayActiveEnd = mode->CrtcHDisplay - 32;

   /* Turn on interlaced mode if necessary */
   if (mode->Flags & V_INTERLACE) {
      i810Reg->InterlaceControl = INTERLACE_ENABLE;
      i810Reg->ExtVertDispEnd *= 2;
   } else
      i810Reg->InterlaceControl = INTERLACE_DISABLE;

   /*
    * Set the overscan color to 0.
    * NOTE: This only affects >8bpp mode.
    */
   pVga->Attribute[0x11] = 0;

   /*
    * Calculate the VCLK that most closely matches the requested dot
    * clock.
    */
   I810CalcVCLK(scrn, dclk);

   /* Since we program the clocks ourselves, always use VCLK2. */
   pVga->MiscOutReg |= 0x0C;

   /* Calculate the FIFO Watermark and Burst Length. */
   i810Reg->LMI_FIFO_Watermark = I810CalcWatermark(scrn, dclk, FALSE);

   /* Setup the ring buffer */
   i810Reg->LprbTail = 0;
   i810Reg->LprbHead = 0;
   i810Reg->LprbStart = pI810->LpRing->mem.Start;

   if (i810Reg->LprbStart)
      i810Reg->LprbLen = ((pI810->LpRing->mem.Size - 4096) |
			  RING_NO_REPORT | RING_VALID);
   else
      i810Reg->LprbLen = RING_INVALID;

   return TRUE;
}

static Bool
I810ModeInit(ScrnInfoPtr scrn, DisplayModePtr mode)
{
   vgaHWPtr hwp;
   I810Ptr pI810;

   hwp = VGAHWPTR(scrn);
   pI810 = I810PTR(scrn);

   vgaHWUnlock(hwp);

   if (!vgaHWInit(scrn, mode))
      return FALSE;

   scrn->vtSema = TRUE;

   if (!I810SetMode(scrn, mode))
      return FALSE;

#ifdef HAVE_DRI1
   if (pI810->directRenderingEnabled) {
      DRILock(xf86ScrnToScreen(scrn), 0);
      pI810->LockHeld = 1;
   }
#endif

   DoRestore(scrn, &hwp->ModeReg, &pI810->ModeReg, FALSE);

#ifdef HAVE_DRI1
   if (pI810->directRenderingEnabled) {
      DRIUnlock(xf86ScrnToScreen(scrn));
      pI810->LockHeld = 0;
   }
#endif

   return TRUE;
}

static void
I810LoadPalette15(ScrnInfoPtr scrn, int numColors, int *indices,
		  LOCO * colors, VisualPtr pVisual)
{
   vgaHWPtr hwp;
   int i, j, index;
   unsigned char r, g, b;

   hwp = VGAHWPTR(scrn);

   for (i = 0; i < numColors; i++) {
      index = indices[i];
      r = colors[index].red;
      g = colors[index].green;
      b = colors[index].blue;
      for (j = 0; j < 8; j++) {
	 hwp->writeDacWriteAddr(hwp, (index << 3) + j);
	 hwp->writeDacData(hwp, r);
	 hwp->writeDacData(hwp, g);
	 hwp->writeDacData(hwp, b);
      }
   }
}

static void
I810LoadPalette16(ScrnInfoPtr scrn, int numColors, int *indices,
		  LOCO * colors, VisualPtr pVisual)
{
   vgaHWPtr hwp;
   int i, index;
   unsigned char r, g, b;

   hwp = VGAHWPTR(scrn);

   /* Load all four entries in each of the 64 color ranges.  -jens */
   for (i = 0; i < numColors; i++) {
      index = indices[i / 2];
      r = colors[index].red;
      b = colors[index].blue;
      index = indices[i];
      g = colors[index].green;

      hwp->writeDacWriteAddr(hwp, index << 2);
      hwp->writeDacData(hwp, r);
      hwp->writeDacData(hwp, g);
      hwp->writeDacData(hwp, b);

      hwp->writeDacWriteAddr(hwp, (index << 2) + 1);
      hwp->writeDacData(hwp, r);
      hwp->writeDacData(hwp, g);
      hwp->writeDacData(hwp, b);

      hwp->writeDacWriteAddr(hwp, (index << 2) + 2);
      hwp->writeDacData(hwp, r);
      hwp->writeDacData(hwp, g);
      hwp->writeDacData(hwp, b);

      hwp->writeDacWriteAddr(hwp, (index << 2) + 3);
      hwp->writeDacData(hwp, r);
      hwp->writeDacData(hwp, g);
      hwp->writeDacData(hwp, b);

      i++;
      index = indices[i];
      g = colors[index].green;

      hwp->writeDacWriteAddr(hwp, index << 2);
      hwp->writeDacData(hwp, r);
      hwp->writeDacData(hwp, g);
      hwp->writeDacData(hwp, b);

      hwp->writeDacWriteAddr(hwp, (index << 2) + 1);
      hwp->writeDacData(hwp, r);
      hwp->writeDacData(hwp, g);
      hwp->writeDacData(hwp, b);

      hwp->writeDacWriteAddr(hwp, (index << 2) + 2);
      hwp->writeDacData(hwp, r);
      hwp->writeDacData(hwp, g);
      hwp->writeDacData(hwp, b);

      hwp->writeDacWriteAddr(hwp, (index << 2) + 3);
      hwp->writeDacData(hwp, r);
      hwp->writeDacData(hwp, g);
      hwp->writeDacData(hwp, b);
   }
}

static void
I810LoadPalette24(ScrnInfoPtr scrn, int numColors, int *indices,
		  LOCO * colors, VisualPtr pVisual)
{
   vgaHWPtr hwp;
   int i, index;
   unsigned char r, g, b;

   hwp = VGAHWPTR(scrn);

   for (i = 0; i < numColors; i++) {
      index = indices[i];
      r = colors[index].red;
      g = colors[index].green;
      b = colors[index].blue;
      hwp->writeDacWriteAddr(hwp, index);
      hwp->writeDacData(hwp, r);
      hwp->writeDacData(hwp, g);
      hwp->writeDacData(hwp, b);
   }
}

Bool
I810AllocateFront(ScrnInfoPtr scrn)
{
   I810Ptr pI810 = I810PTR(scrn);
   int cache_lines = -1;

   if (pI810->DoneFrontAlloc)
      return TRUE;

   memset(&(pI810->FbMemBox), 0, sizeof(BoxRec));
   /* Alloc FrontBuffer/Ring/Accel memory */
   pI810->FbMemBox.x1 = 0;
   pI810->FbMemBox.x2 = scrn->displayWidth;
   pI810->FbMemBox.y1 = 0;
   pI810->FbMemBox.y2 = scrn->virtualY;

   xf86GetOptValInteger(pI810->Options, OPTION_CACHE_LINES, &cache_lines);

   if (cache_lines < 0) {
      /* make sure there is enough for two DVD sized YUV buffers */
      cache_lines = (scrn->depth == 24) ? 256 : 384;
      if (scrn->displayWidth <= 1024)
	 cache_lines *= 2;
   }
   /* Make sure there's enough space for cache_lines.
    *
    * Had a bug here where maxCacheLines was computed to be less than 0.
    * Not sure why 256 was initially subtracted from videoRam in the
    * maxCacheLines calculation, but that was causing a problem
    * for configurations that have exactly enough Ram for the framebuffer.
    * Common code should catch the case where there isn't enough space for
    * framebuffer, we'll just check for no space for cache_lines.  -jens
    *
    */
   {
      int maxCacheLines;

      maxCacheLines = (scrn->videoRam * 1024 /
		       (scrn->bitsPerPixel / 8) /
		       scrn->displayWidth) - scrn->virtualY;
      if (maxCacheLines < 0)
	 maxCacheLines = 0;
      if (cache_lines > maxCacheLines)
	 cache_lines = maxCacheLines;
   }
   pI810->FbMemBox.y2 += cache_lines;

   xf86DrvMsg(scrn->scrnIndex, X_INFO,
	      "Adding %i scanlines for pixmap caching\n", cache_lines);

   /* Reserve room for the framebuffer and pixcache.  Put at the top
    * of memory so we can have nice alignment for the tiled regions at
    * the start of memory.
    */

   if (!I810AllocLow(&(pI810->FrontBuffer),
		     &(pI810->SysMem),
		     ALIGN((pI810->FbMemBox.x2 * pI810->FbMemBox.y2 * pI810->cpp), 4096))) {
      xf86DrvMsg(scrn->scrnIndex,
		 X_WARNING, "Framebuffer allocation failed\n");
      return FALSE;
   }

   memset(pI810->LpRing, 0, sizeof(I810RingBuffer));
   if (I810AllocLow(&(pI810->LpRing->mem), &(pI810->SysMem), 16 * 4096)) {
      pI810->LpRing->tail_mask = pI810->LpRing->mem.Size - 1;
      pI810->LpRing->virtual_start = pI810->FbBase + pI810->LpRing->mem.Start;
      pI810->LpRing->head = 0;
      pI810->LpRing->tail = 0;
      pI810->LpRing->space = 0;
   } else {
      xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		 "Ring buffer allocation failed\n");
      return (FALSE);
   }

   if (I810AllocLow(&pI810->Scratch, &(pI810->SysMem), 64 * 1024) ||
       I810AllocLow(&pI810->Scratch, &(pI810->SysMem), 16 * 1024)) {
      xf86DrvMsg(scrn->scrnIndex, X_INFO, "Allocated Scratch Memory\n");
   } else {
      xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		 "Scratch memory allocation failed\n");
      return (FALSE);
   }

   pI810->DoneFrontAlloc = TRUE;
   return TRUE;
}

static Bool
I810ScreenInit(SCREEN_INIT_ARGS_DECL)
{
   ScrnInfoPtr scrn;
   vgaHWPtr hwp;
   I810Ptr pI810;
   VisualPtr visual;

   scrn = xf86ScreenToScrn(screen);
   pI810 = I810PTR(scrn);
   hwp = VGAHWPTR(scrn);

   pI810->LpRing = calloc(sizeof(I810RingBuffer),1);
   if (!pI810->LpRing) {
     xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		"Could not allocate lpring data structure.\n");
     return FALSE;
   }

   miClearVisualTypes();

   /* Re-implemented Direct Color support, -jens */
   if (!miSetVisualTypes(scrn->depth, miGetDefaultVisualMask(scrn->depth),
			 scrn->rgbBits, scrn->defaultVisual))
      return FALSE;

   if (!miSetPixmapDepths())
      return FALSE;

   {
      I810RegPtr i810Reg = &pI810->ModeReg;
      int i;

      for (i = 0; i < 8; i++)
	 i810Reg->Fence[i] = 0;
   }

   /* Have to init the DRM earlier than in other drivers to get agp
    * memory.  Wonder if this is going to be a problem...
    */

#ifdef HAVE_DRI1
   /*
    * Setup DRI after visuals have been established, but before fbScreenInit
    * is called.   fbScreenInit will eventually call into the drivers
    * InitGLXVisuals call back.
    */
   /*
    * pI810->directRenderingDisabled is set once in PreInit.  Reinitialise
    * pI810->directRenderingEnabled based on it each generation.
    */
   pI810->directRenderingEnabled = !pI810->directRenderingDisabled;

   if (pI810->directRenderingEnabled==TRUE)
     pI810->directRenderingEnabled = I810DRIScreenInit(screen);

#else
   pI810->directRenderingEnabled = FALSE;
   if (!I810AllocateGARTMemory(scrn))
      return FALSE;
   if (!I810AllocateFront(scrn))
      return FALSE;
#endif

   if (!I810MapMem(scrn))
      return FALSE;

   scrn->memPhysBase = (unsigned long)pI810->LinearAddr;
   scrn->fbOffset = 0;

   vgaHWSetMmioFuncs(hwp, pI810->MMIOBase, 0);
   vgaHWGetIOBase(hwp);
   if (!vgaHWMapMem(scrn))
      return FALSE;

   I810Save(scrn);
   if (!I810ModeInit(scrn, scrn->currentMode))
      return FALSE;

   I810SaveScreen(screen, FALSE);
   I810AdjustFrame(ADJUST_FRAME_ARGS(scrn, scrn->frameX0, scrn->frameY0));

   if (!fbScreenInit(screen, pI810->FbBase + scrn->fbOffset,
		     scrn->virtualX, scrn->virtualY,
		     scrn->xDpi, scrn->yDpi,
		     scrn->displayWidth, scrn->bitsPerPixel))
      return FALSE;

   if (scrn->bitsPerPixel > 8) {
      /* Fixup RGB ordering */
      visual = screen->visuals + screen->numVisuals;
      while (--visual >= screen->visuals) {
	 if ((visual->class | DynamicClass) == DirectColor) {
	    visual->offsetRed = scrn->offset.red;
	    visual->offsetGreen = scrn->offset.green;
	    visual->offsetBlue = scrn->offset.blue;
	    visual->redMask = scrn->mask.red;
	    visual->greenMask = scrn->mask.green;
	    visual->blueMask = scrn->mask.blue;
	 }
      }
   }

   fbPictureInit(screen, NULL, 0);

   xf86SetBlackWhitePixels(screen);

#ifdef HAVE_DRI1
   if (pI810->LpRing->mem.Start == 0 && pI810->directRenderingEnabled) {
      pI810->directRenderingEnabled = FALSE;
      I810DRICloseScreen(screen);
   }

   if (!pI810->directRenderingEnabled) {
      pI810->DoneFrontAlloc = FALSE;
      if (!I810AllocateGARTMemory(scrn))
	 return FALSE;
      if (!I810AllocateFront(scrn))
	 return FALSE;
   }
#endif

#ifdef HAVE_DGAPROC_H
   I810DGAInit(screen);
#endif

   if (!xf86InitFBManager(screen, &(pI810->FbMemBox))) {
      xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		 "Failed to init memory manager\n");
      return FALSE;
   }

   if (pI810->LpRing->mem.Size != 0) {
      I810SetRingRegs(scrn);

      if (!pI810->noAccel && !I810AccelInit(screen)) {
	 xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		    "Hardware acceleration initialization failed\n");
      }

      I810EmitFlush(scrn);
   }

   xf86SetBackingStore(screen);
   xf86SetSilkenMouse(screen);

   miDCInitialize(screen, xf86GetPointerScreenFuncs());

   if (!xf86ReturnOptValBool(pI810->Options, OPTION_SW_CURSOR, FALSE)) {
      if (!I810CursorInit(screen)) {
	 xf86DrvMsg(scrn->scrnIndex, X_ERROR,
		    "Hardware cursor initialization failed\n");
      }
   }

   if (!miCreateDefColormap(screen))
      return FALSE;

   /* Use driver specific palette load routines for Direct Color support. -jens */
   if (scrn->bitsPerPixel == 16) {
      if (scrn->depth == 15) {
	 if (!xf86HandleColormaps(screen, 256, 8, I810LoadPalette15, NULL,
				  CMAP_PALETTED_TRUECOLOR |
				  CMAP_RELOAD_ON_MODE_SWITCH))
	    return FALSE;
      } else {
	 if (!xf86HandleColormaps(screen, 256, 8, I810LoadPalette16, NULL,
				  CMAP_PALETTED_TRUECOLOR |
				  CMAP_RELOAD_ON_MODE_SWITCH))
	    return FALSE;
      }
   } else {
      if (!xf86HandleColormaps(screen, 256, 8, I810LoadPalette24, NULL,
			       CMAP_PALETTED_TRUECOLOR |
			       CMAP_RELOAD_ON_MODE_SWITCH))
	 return FALSE;
   }

   xf86DPMSInit(screen, I810DisplayPowerManagementSet, 0);

   I810InitVideo(screen);

#ifdef HAVE_DRI1
   if (pI810->directRenderingEnabled) {
      /* Now that mi, fb, drm and others have done their thing,
       * complete the DRI setup.
       */
      pI810->directRenderingEnabled = I810DRIFinishScreenInit(screen);
   }
#ifdef XvMCExtension
   if ((pI810->directRenderingEnabled) && (pI810->numSurfaces)) {
      /* Initialize the hardware motion compensation code */
      I810InitMC(screen);
   }
#endif
#endif

   if (pI810->directRenderingEnabled) {
      xf86DrvMsg(scrn->scrnIndex, X_INFO, "Direct rendering enabled\n");
   } else {
      xf86DrvMsg(scrn->scrnIndex, X_WARNING, "Direct rendering disabled\n");
   }

   screen->SaveScreen = I810SaveScreen;
   pI810->CloseScreen = screen->CloseScreen;
   screen->CloseScreen = I810CloseScreen;

   if (serverGeneration == 1)
      xf86ShowUnusedOptions(scrn->scrnIndex, scrn->options);

   return TRUE;
}

Bool
I810SwitchMode(SWITCH_MODE_ARGS_DECL)
{
   SCRN_INFO_PTR(arg);
#if 0
   I810Ptr pI810 = I810PTR(scrn);
#endif
   if (I810_DEBUG & DEBUG_VERBOSE_CURSOR)
      ErrorF("I810SwitchMode %p\n", (void *)mode);

#if 0
/* 
 * This has been added to prevent lockups on mode switch by modeling
 * it after I810Leave()/I810Enter() but the call to I810DRILeave() 
 * was missing so it caused the opposite. 
 * The version below works but it is doubtful it does any good.
 * If lockups on mode switch are still seen revisit this code. (EE)
 */

# ifdef HAVE_DRI1
   if (pI810->directRenderingEnabled) {
      if (I810_DEBUG & DEBUG_VERBOSE_DRI)
	 ErrorF("calling dri lock\n");
      DRILock(screenInfo.screens[scrnIndex], 0);
      pI810->LockHeld = 1;
   }
# endif
   if (pI810->AccelInfoRec != NULL) {
      I810RefreshRing(scrn);
      I810Sync(scrn);
      pI810->AccelInfoRec->NeedToSync = FALSE;
   }
   I810Restore(scrn);

# ifdef HAVE_DRI1
   if (pI810->directRenderingEnabled) {
       if (!I810DRILeave(scrn))
	   return FALSE;
       if (!I810DRIEnter(scrn))
	   return FALSE;

       if (I810_DEBUG & DEBUG_VERBOSE_DRI)
	   ErrorF("calling dri unlock\n");
       DRIUnlock(screenInfo.screens[scrnIndex]);
       pI810->LockHeld = 0;
   }
# endif
#endif
   return I810ModeInit(scrn, mode);
}

void
I810AdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
   SCRN_INFO_PTR(arg);
   I810Ptr pI810 = I810PTR(scrn);
   vgaHWPtr hwp = VGAHWPTR(scrn);
   int Base;

#if 1
   if (pI810->showCache) {
     int lastline = pI810->FbMapSize / 
       ((scrn->displayWidth * scrn->bitsPerPixel) / 8);
     lastline -= scrn->currentMode->VDisplay;
     if (y > 0)
       y += scrn->currentMode->VDisplay;
     if (y > lastline) y = lastline;
   }
#endif
   Base = (y * scrn->displayWidth + x) >> 2;

   if (I810_DEBUG & DEBUG_VERBOSE_CURSOR)
      ErrorF("I810AdjustFrame %d,%d\n", x, y);

   switch (scrn->bitsPerPixel) {
   case 8:
      break;
   case 16:
      Base *= 2;
      break;
   case 24:
      /* KW: Need to do 16-pixel alignment for i810, otherwise you
       * get bad watermark problems.  Need to fixup the mouse
       * pointer positioning to take this into account.
       */
      pI810->CursorOffset = (Base & 0x3) * 4;
      Base &= ~0x3;
      Base *= 3;
      break;
   case 32:
      Base *= 4;
      break;
   }

   hwp->writeCrtc(hwp, START_ADDR_LO, Base & 0xFF);
   hwp->writeCrtc(hwp, START_ADDR_HI, (Base & 0xFF00) >> 8);
   hwp->writeCrtc(hwp, EXT_START_ADDR_HI, (Base & 0x3FC00000) >> 22);
   hwp->writeCrtc(hwp, EXT_START_ADDR,
		  ((Base & 0x00eF0000) >> 16 | EXT_START_ADDR_ENABLE));
}

/* These functions are usually called with the lock **not held**.
 */
static Bool
I810EnterVT(VT_FUNC_ARGS_DECL)
{
   SCRN_INFO_PTR(arg);

#ifdef HAVE_DRI1
   I810Ptr pI810 = I810PTR(scrn);
#endif

   if (I810_DEBUG & DEBUG_VERBOSE_DRI)
      ErrorF("\n\nENTER VT\n");

   if (!I810BindGARTMemory(scrn)) {
      return FALSE;
   }
#ifdef HAVE_DRI1
   if (!I810DRIEnter(scrn)) {
      return FALSE;
   }
   if (pI810->directRenderingEnabled) {
      if (I810_DEBUG & DEBUG_VERBOSE_DRI)
	 ErrorF("calling dri unlock\n");
      DRIUnlock(xf86ScrnToScreen(scrn));
      pI810->LockHeld = 0;
   }
#endif

   if (!I810ModeInit(scrn, scrn->currentMode))
      return FALSE;
   I810AdjustFrame(ADJUST_FRAME_ARGS(scrn, scrn->frameX0, scrn->frameY0));
   return TRUE;
}

static void
I810LeaveVT(VT_FUNC_ARGS_DECL)
{
   SCRN_INFO_PTR(arg);
   vgaHWPtr hwp = VGAHWPTR(scrn);
   I810Ptr pI810 = I810PTR(scrn);

   if (I810_DEBUG & DEBUG_VERBOSE_DRI)
      ErrorF("\n\n\nLeave VT\n");

#ifdef HAVE_DRI1
   if (pI810->directRenderingEnabled) {
      if (I810_DEBUG & DEBUG_VERBOSE_DRI)
	 ErrorF("calling dri lock\n");
      DRILock(xf86ScrnToScreen(scrn), 0);
      pI810->LockHeld = 1;
   }
#endif

#ifdef HAVE_XAA_H
   if (pI810->AccelInfoRec != NULL) {
      I810RefreshRing(scrn);
      I810Sync(scrn);
      pI810->AccelInfoRec->NeedToSync = FALSE;
   }
#endif
   I810Restore(scrn);

   if (!I810UnbindGARTMemory(scrn))
      return;
#ifdef HAVE_DRI1
   if (!I810DRILeave(scrn))
      return;
#endif

   vgaHWLock(hwp);
}

static Bool
I810CloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
   ScrnInfoPtr scrn = xf86ScreenToScrn(screen);
   vgaHWPtr hwp = VGAHWPTR(scrn);
   I810Ptr pI810 = I810PTR(scrn);
#ifdef HAVE_XAA_H
   XAAInfoRecPtr infoPtr = pI810->AccelInfoRec;
#endif

   if (scrn->vtSema == TRUE) {
#ifdef HAVE_XAA_H
      if (pI810->AccelInfoRec != NULL) {
	 I810RefreshRing(scrn);
	 I810Sync(scrn);
	 pI810->AccelInfoRec->NeedToSync = FALSE;
      }
#endif
      I810Restore(scrn);
      vgaHWLock(hwp);
   }
#ifdef HAVE_DRI1
   if (pI810->directRenderingEnabled) {
      I810DRICloseScreen(screen);
      pI810->directRenderingEnabled = FALSE;
   }
#endif

   if (scrn->vtSema == TRUE) {
      I810UnbindGARTMemory(scrn);
      I810Restore(scrn);
      vgaHWLock(hwp);
   }

   I810UnmapMem(scrn);
   vgaHWUnmapMem(scrn);

   if (pI810->ScanlineColorExpandBuffers) {
      free(pI810->ScanlineColorExpandBuffers);
      pI810->ScanlineColorExpandBuffers = NULL;
   }

#ifdef HAVE_XAA_H
   if (infoPtr) {
      if (infoPtr->ScanlineColorExpandBuffers)
	 free(infoPtr->ScanlineColorExpandBuffers);
      XAADestroyInfoRec(infoPtr);
      pI810->AccelInfoRec = NULL;
   }
#endif

   if (pI810->CursorInfoRec) {
      xf86DestroyCursorInfoRec(pI810->CursorInfoRec);
      pI810->CursorInfoRec = NULL;
   }

   /* Free all allocated video ram.
    */
   pI810->SysMem = pI810->SavedSysMem;
   pI810->DcacheMem = pI810->SavedDcacheMem;
   pI810->DoneFrontAlloc = FALSE;

   /* Need to actually close the gart fd, or the unbound memory will just sit
    * around.  Will prevent the Xserver from recycling.
    */
   xf86GARTCloseScreen(scrn->scrnIndex);

   free(pI810->LpRing);
   pI810->LpRing = NULL;

   scrn->vtSema = FALSE;
   screen->CloseScreen = pI810->CloseScreen;
   return (*screen->CloseScreen) (CLOSE_SCREEN_ARGS);
}

static void
I810FreeScreen(FREE_SCREEN_ARGS_DECL)
{
   SCRN_INFO_PTR(arg);
   I810FreeRec(scrn);
   if (xf86LoaderCheckSymbol("vgaHWFreeHWRec"))
     vgaHWFreeHWRec(scrn);
}

static ModeStatus
I810ValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode, Bool verbose, int flags)
{
   SCRN_INFO_PTR(arg);
   if (mode->Flags & V_INTERLACE) {
      if (verbose) {
	 xf86DrvMsg(scrn->scrnIndex, X_PROBED,
		    "Removing interlaced mode \"%s\"\n", mode->name);
      }
      return MODE_BAD;
   }
   return MODE_OK;
}

static Bool
I810SaveScreen(ScreenPtr screen, Bool unblack)
{
   return vgaHWSaveScreen(screen, unblack);
}

static void
I810DisplayPowerManagementSet(ScrnInfoPtr scrn, int PowerManagementMode,
			      int flags)
{
   I810Ptr pI810;
   unsigned char SEQ01 = 0;
   int DPMSSyncSelect = 0;
   vgaHWPtr hwp;

   pI810 = I810PTR(scrn);
   switch (PowerManagementMode) {
   case DPMSModeOn:
      /* Screen: On; HSync: On, VSync: On */
      SEQ01 = 0x00;
      DPMSSyncSelect = HSYNC_ON | VSYNC_ON;
      break;
   case DPMSModeStandby:
      /* Screen: Off; HSync: Off, VSync: On */
      SEQ01 = 0x20;
      DPMSSyncSelect = HSYNC_OFF | VSYNC_ON;
      break;
   case DPMSModeSuspend:
      /* Screen: Off; HSync: On, VSync: Off */
      SEQ01 = 0x20;
      DPMSSyncSelect = HSYNC_ON | VSYNC_OFF;
      break;
   case DPMSModeOff:
      /* Screen: Off; HSync: Off, VSync: Off */
      SEQ01 = 0x20;
      DPMSSyncSelect = HSYNC_OFF | VSYNC_OFF;
      break;
   }

   hwp = VGAHWPTR(scrn);

   /* Turn the screen on/off */
   SEQ01 |= hwp->readSeq(hwp, 0x01) & ~0x20;
   hwp->writeSeq(hwp, 0x01, SEQ01);

   /* Set the DPMS mode */
   OUTREG8(DPMS_SYNC_SELECT, DPMSSyncSelect);
}

const OptionInfoRec *
lg_i810_available_options(int chipid, int busid)
{
   return I810Options;
}


Bool lg_i810_init(ScrnInfoPtr scrn)
{
    scrn->PreInit = I810PreInit;
    scrn->ScreenInit = I810ScreenInit;
    scrn->SwitchMode = I810SwitchMode;
    scrn->AdjustFrame = I810AdjustFrame;
    scrn->EnterVT = I810EnterVT;
    scrn->LeaveVT = I810LeaveVT;
    scrn->FreeScreen = I810FreeScreen;
    scrn->ValidMode = I810ValidMode;
    return TRUE;
}
