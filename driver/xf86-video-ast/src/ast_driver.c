/*
 * Copyright (c) 2005 ASPEED Technology Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the authors not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The authors makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Resources.h"
#include "xf86RAC.h"
#include "xf86cmap.h"
#include "compiler.h"
#include "mibstore.h"
#include "vgaHW.h"
#include "mipointer.h"
#include "micmap.h"

#include "fb.h"
#include "regionstr.h"
#include "xf86xv.h"
#include <X11/extensions/Xv.h>
#include "vbe.h"

#include "xf86PciInfo.h"
#include "xf86Pci.h"

/* framebuffer offscreen manager */
#include "xf86fbman.h"

/* include xaa includes */
#include "xaa.h"
#include "xaarop.h"

/* H/W cursor support */
#include "xf86Cursor.h"

/* Driver specific headers */
#include "ast.h"

/* external reference fucntion */
extern Bool ASTMapMem(ScrnInfoPtr pScrn);
extern Bool ASTUnmapMem(ScrnInfoPtr pScrn);
extern Bool ASTMapMMIO(ScrnInfoPtr pScrn);
extern void ASTUnmapMMIO(ScrnInfoPtr pScrn);

extern void vASTOpenKey(ScrnInfoPtr pScrn);
extern Bool bASTRegInit(ScrnInfoPtr pScrn);
extern ULONG GetVRAMInfo(ScrnInfoPtr pScrn);
extern ULONG GetMaxDCLK(ScrnInfoPtr pScrn);
extern void vASTLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors, VisualPtr pVisual);
extern void ASTDisplayPowerManagementSet(ScrnInfoPtr pScrn, int PowerManagementMode, int flags);
extern void vSetStartAddressCRT1(ASTRecPtr pAST, ULONG base);
extern Bool ASTSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode);

extern Bool bInitCMDQInfo(ScrnInfoPtr pScrn, ASTRecPtr pAST);
extern Bool bEnableCMDQ(ScrnInfoPtr pScrn, ASTRecPtr pAST);
extern void vDisable2D(ScrnInfoPtr pScrn, ASTRecPtr pAST);

extern Bool ASTAccelInit(ScreenPtr pScreen);

extern Bool ASTCursorInit(ScreenPtr pScreen);

/* Mandatory functions */
static void ASTIdentify(int flags);
const OptionInfoRec *ASTAvailableOptions(int chipid, int busid);
static Bool ASTProbe(DriverPtr drv, int flags);
static Bool ASTPreInit(ScrnInfoPtr pScrn, int flags);
static Bool ASTScreenInit(int Index, ScreenPtr pScreen, int argc, char **argv);
Bool ASTSwitchMode(int scrnIndex, DisplayModePtr mode, int flags);
void ASTAdjustFrame(int scrnIndex, int x, int y, int flags);
static Bool ASTEnterVT(int scrnIndex, int flags);
static void ASTLeaveVT(int scrnIndex, int flags);
static void ASTFreeScreen(int scrnIndex, int flags);
static ModeStatus ASTValidMode(int scrnIndex, DisplayModePtr mode, Bool verbose, int flags);

/* Internally used functions */
static Bool ASTGetRec(ScrnInfoPtr pScrn);
static void ASTFreeRec(ScrnInfoPtr pScrn);
static Bool ASTSaveScreen(ScreenPtr pScreen, Bool unblack);
static Bool ASTCloseScreen(int scrnIndex, ScreenPtr pScreen);
static void ASTSave(ScrnInfoPtr pScrn);
static void ASTRestore(ScrnInfoPtr pScrn);
static void ASTProbeDDC(ScrnInfoPtr pScrn, int index);
static xf86MonPtr ASTDoDDC(ScrnInfoPtr pScrn, int index);
static void vFillASTModeInfo (ScrnInfoPtr pScrn);
static Bool ASTModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);

/*
 * This is intentionally screen-independent.  It indicates the binding
 * choice made in the first PreInit.
 */
DriverRec AST = {
   AST_VERSION,
   AST_DRIVER_NAME,
   ASTIdentify,
   ASTProbe,
   ASTAvailableOptions,
   NULL,
   0
};

/* Chipsets */
static SymTabRec ASTChipsets[] = {
   {PCI_CHIP_AST2000,	"AST2000"},
   {-1,			NULL}
};

static PciChipsets ASTPciChipsets[] = {
   {PCI_CHIP_AST2000,		PCI_CHIP_AST2000,	RES_SHARED_VGA},
   {-1,				-1, 			RES_UNDEFINED }
};

typedef enum {
   OPTION_NOACCEL,
   OPTION_MMIO2D,   
   OPTION_SW_CURSOR,
   OPTION_HWC_NUM,
   OPTION_ENG_CAPS,   
   OPTION_DBG_SELECT,
   OPTION_NO_DDC
} ASTOpts;

static const OptionInfoRec ASTOptions[] = {
   {OPTION_NOACCEL,	"NoAccel",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_MMIO2D,	"MMIO2D",	OPTV_BOOLEAN,	{0},	FALSE},   
   {OPTION_SW_CURSOR,	"SWCursor",	OPTV_BOOLEAN,	{0},	FALSE},	
   {OPTION_HWC_NUM,	"HWCNumber",	OPTV_INTEGER,	{0},	FALSE},
   {OPTION_ENG_CAPS,	"ENGCaps",	OPTV_INTEGER,	{0},	FALSE},	          	  
   {OPTION_DBG_SELECT,	"DBGSelect",	OPTV_INTEGER,	{0},	FALSE},	       
   {OPTION_NO_DDC,	"NoDDC",	OPTV_BOOLEAN,	{0}, 	FALSE},   
   {-1,			NULL,		OPTV_NONE,	{0}, 	FALSE}
};

const char *vgahwSymbols[] = {
   "vgaHWFreeHWRec",
   "vgaHWGetHWRec",
   "vgaHWGetIOBase",
   "vgaHWGetIndex",
   "vgaHWInit",
   "vgaHWLock",
   "vgaHWMapMem",
   "vgaHWProtect",
   "vgaHWRestore",
   "vgaHWSave",
   "vgaHWSaveScreen",
   "vgaHWSetMmioFuncs",
   "vgaHWUnlock",
   "vgaHWUnmapMem",
   NULL
};

const char *fbSymbols[] = {
   "fbPictureInit",
   "fbScreenInit",
   NULL
};

const char *vbeSymbols[] = {
   "VBEInit",
   "VBEFreeModeInfo",
   "VBEFreeVBEInfo",
   "VBEGetModeInfo",
   "VBEGetModePool",
   "VBEGetVBEInfo",
   "VBEGetVBEMode",
   "VBEPrintModes",
   "VBESaveRestore",
   "VBESetDisplayStart",
   "VBESetGetDACPaletteFormat",
   "VBESetGetLogicalScanlineLength",
   "VBESetGetPaletteData",
   "VBESetModeNames",
   "VBESetModeParameters",
   "VBESetVBEMode",
   "VBEValidateModes",
   "vbeDoEDID",
   "vbeFree",
   NULL
};

#ifdef XFree86LOADER
static const char *vbeOptionalSymbols[] = {
   "VBEDPMSSet",
   "VBEGetPixelClock",
   NULL
};
#endif

const char *ddcSymbols[] = {
   "xf86PrintEDID",
   "xf86SetDDCproperties",
   NULL
};

const char *int10Symbols[] = {
   "xf86ExecX86int10",
   "xf86InitInt10",
   "xf86Int10AllocPages",
   "xf86int10Addr",
   NULL
};

const char *xaaSymbols[] = {
   "XAACreateInfoRec",
   "XAADestroyInfoRec",
   "XAAInit",
   "XAACopyROP",
   "XAAPatternROP",
   NULL
};

const char *ramdacSymbols[] = {
   "xf86CreateCursorInfoRec",
   "xf86DestroyCursorInfoRec",
   "xf86InitCursor",
   NULL
};


#ifdef XFree86LOADER

static MODULESETUPPROTO(astSetup);

static XF86ModuleVersionInfo astVersRec = {
   AST_DRIVER_NAME,
   MODULEVENDORSTRING,
   MODINFOSTRING1,
   MODINFOSTRING2,
   XORG_VERSION_CURRENT,
   AST_MAJOR_VERSION, AST_MINOR_VERSION, AST_PATCH_VERSION,
   ABI_CLASS_VIDEODRV,
   ABI_VIDEODRV_VERSION,
   MOD_CLASS_VIDEODRV,
   {0, 0, 0, 0}
};

XF86ModuleData astModuleData = { &astVersRec, astSetup, NULL };

static pointer
astSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
   static Bool setupDone = FALSE;

   /* This module should be loaded only once, but check to be sure.
    */
   if (!setupDone) {
      setupDone = TRUE;
      xf86AddDriver(&AST, module, 0);

      /*
       * Tell the loader about symbols from other modules that this module
       * might refer to.
       */
      LoaderRefSymLists(vgahwSymbols,
			fbSymbols, xaaSymbols, ramdacSymbols,
			vbeSymbols, vbeOptionalSymbols,
			ddcSymbols, int10Symbols, NULL);

      /*
       * The return value must be non-NULL on success even though there
       * is no TearDownProc.
       */
      return (pointer) TRUE;
   } else {
      if (errmaj)
	 *errmaj = LDR_ONCEONLY;
      return NULL;
   }
}

#endif	/* XFree86LOADER */

/*
 * ASTIdentify --
 *
 * Returns the string name for the driver based on the chipset. In this
 * case it will always be an AST, so we can return a static string.
 *
 */
static void
ASTIdentify(int flags)
{
   xf86PrintChipsets(AST_NAME, "Driver for ASPEED Graphics Chipsets",
		     ASTChipsets);
}

const OptionInfoRec *
ASTAvailableOptions(int chipid, int busid)
{
	
   return ASTOptions;

}

/*
 * ASTProbe --
 *
 * Look through the PCI bus to find cards that are AST boards.
 * Setup the dispatch table for the rest of the driver functions.
 *
 */
static Bool
ASTProbe(DriverPtr drv, int flags)
{
    int i, numUsed, numDevSections, *usedChips;
    Bool foundScreen = FALSE;
    GDevPtr *devSections;   

   /*
    * Find the config file Device sections that match this
    * driver, and return if there are none.
    */
    if ((numDevSections =
	xf86MatchDevice(AST_DRIVER_NAME, &devSections)) <= 0) {
      return FALSE;
    }

   /*
    * This probing is just checking the PCI data the server already
    * collected.
    */
    if (xf86GetPciVideoInfo() == NULL) {
	return FALSE;
    }

    numUsed = xf86MatchPciInstances(AST_NAME, PCI_VENDOR_AST,
				   ASTChipsets, ASTPciChipsets,
				   devSections, numDevSections,
				   drv, &usedChips);

    xfree(devSections);

    if (flags & PROBE_DETECT) {
        if (numUsed > 0)
	    foundScreen = TRUE;
    } else {
        for (i = 0; i < numUsed; i++) {
	    ScrnInfoPtr pScrn = NULL;

	    /* Allocate new ScrnInfoRec and claim the slot */
	    if ((pScrn = xf86ConfigPciEntity(pScrn, 0, usedChips[i],
					     ASTPciChipsets, 0, 0, 0, 0, 0)))
            {
	        EntityInfoPtr pEnt;

	        pEnt = xf86GetEntityInfo(usedChips[i]);

	        pScrn->driverVersion = AST_VERSION;
	        pScrn->driverName = AST_DRIVER_NAME;
	        pScrn->name = AST_NAME;
	    
	        pScrn->Probe = ASTProbe;
	        pScrn->PreInit = ASTPreInit;
	        pScrn->ScreenInit = ASTScreenInit;
	        pScrn->SwitchMode = ASTSwitchMode;
	        pScrn->AdjustFrame = ASTAdjustFrame;   
	        pScrn->EnterVT = ASTEnterVT;
	        pScrn->LeaveVT = ASTLeaveVT;
	        pScrn->FreeScreen = ASTFreeScreen;
	        pScrn->ValidMode = ASTValidMode;
	    
	        foundScreen = TRUE;	    

	    } /* end of if */
        }  /* end of for-loop */
    } /* end of if flags */	   

    xfree(usedChips);

    return foundScreen;
}

/*
 * ASTPreInit --
 *
 * Do initial setup of the board before we know what resolution we will
 * be running at.
 *
 */
static Bool
ASTPreInit(ScrnInfoPtr pScrn, int flags)
{
   EntityInfoPtr pEnt;
   vgaHWPtr hwp;
   int flags24;
   rgb defaultWeight = { 0, 0, 0 };
      
   ASTRecPtr pAST;
   
   ClockRangePtr clockRanges;
   int i;
   MessageType from;

   /* Suport one adapter only now */
   if (pScrn->numEntities != 1)
       return FALSE;

   pEnt = xf86GetEntityInfo(pScrn->entityList[0]);

   if (flags & PROBE_DETECT) {
       ASTProbeDDC(pScrn, pEnt->index);
       return TRUE;
   }

   if (pEnt->location.type != BUS_PCI)
       return FALSE;

   if (xf86RegisterResources(pEnt->index, 0, ResExclusive))
       return FALSE;

   /* The vgahw module should be loaded here when needed */
   if (!xf86LoadSubModule(pScrn, "vgahw"))
      return FALSE;
   xf86LoaderReqSymLists(vgahwSymbols, NULL);

   /* The fb module should be loaded here when needed */
   if (!xf86LoadSubModule(pScrn, "fb"))
      return FALSE;
   xf86LoaderReqSymLists(fbSymbols, NULL);      
   	
   /* Allocate a vgaHWRec */
   if (!vgaHWGetHWRec(pScrn))
       return FALSE;
   hwp = VGAHWPTR(pScrn);

   /* Color Depth Check */
   flags24 = Support32bppFb;
   if (!xf86SetDepthBpp(pScrn, 0, 0, 0, flags24)) {
      return FALSE;
   } else {
      switch (pScrn->depth) {
      case 8:
      case 16:
      case 24:
	 break;
      default:
	 xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "Given depth (%d) is not supported by ast driver\n",
		    pScrn->depth);
	 return FALSE;
      }
   }
   xf86PrintDepthBpp(pScrn);

   switch (pScrn->bitsPerPixel) {
   case 8:
   case 16:
   case 32:
      break;
   default:
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "Given bpp (%d) is not supported by ast driver\n",
		 pScrn->bitsPerPixel);
      return FALSE;
   }
   
   /* fill pScrn misc. */
   pScrn->progClock = TRUE;
   pScrn->rgbBits = 6;
   pScrn->monitor = pScrn->confScreen->monitor; /* should be initialized before set gamma */
   pScrn->racMemFlags = RAC_FB | RAC_COLORMAP | RAC_CURSOR | RAC_VIEWPORT;
   pScrn->racIoFlags = RAC_COLORMAP | RAC_CURSOR | RAC_VIEWPORT;   
      
   /*
    * If the driver can do gamma correction, it should call xf86SetGamma()
    * here.
    */
   {
      Gamma zeros = { 0.0, 0.0, 0.0 };

      if (!xf86SetGamma(pScrn, zeros)) {
         xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "call xf86SetGamma failed \n");      	
	 return FALSE;
      }
   }


   if (!xf86SetWeight(pScrn, defaultWeight, defaultWeight)) {
       return FALSE;
   }    

   if (!xf86SetDefaultVisual(pScrn, -1)) {
       return FALSE;
   }      

   /* Allocate driverPrivate */
   if (!ASTGetRec(pScrn)) {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "call ASTGetRec failed \n");   	
       return FALSE;
   }    

   /* Fill AST Info */
   pAST = ASTPTR(pScrn);
   pAST->pEnt    = xf86GetEntityInfo(pScrn->entityList[0]);
   pAST->PciInfo = xf86GetPciInfoForEntity(pAST->pEnt->index);
   pAST->PciTag  = pciTag(pAST->PciInfo->bus, pAST->PciInfo->device,
			  pAST->PciInfo->func);

   /* Process the options
    * pScrn->confScreen, pScrn->display, pScrn->monitor, pScrn->numEntities, 
    * and pScrn->entityList should be initialized before
    */
   xf86CollectOptions(pScrn, NULL);   
   if (!(pAST->Options = xalloc(sizeof(ASTOptions))))
   {  	
      ASTFreeRec(pScrn);   	
      return FALSE;
   }      
   memcpy(pAST->Options, ASTOptions, sizeof(ASTOptions));
   xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pAST->Options);
    
   /*
    * Set the Chipset and ChipRev, allowing config file entries to
    * override.
    */
   if (pAST->pEnt->device->chipset && *pAST->pEnt->device->chipset) {
      pScrn->chipset = pAST->pEnt->device->chipset;
      from = X_CONFIG;
   } else if (pAST->pEnt->device->chipID >= 0) {
      pScrn->chipset = (char *)xf86TokenToString(ASTChipsets,
						 pAST->pEnt->device->chipID);
      from = X_CONFIG;
      xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipID override: 0x%04X\n",
		 pAST->pEnt->device->chipID);
   } else {
      from = X_PROBED;
      pScrn->chipset = (char *)xf86TokenToString(ASTChipsets,
						 pAST->PciInfo->chipType);
   }
   if (pAST->pEnt->device->chipRev >= 0) {
      xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipRev override: %d\n",
		 pAST->pEnt->device->chipRev);
   }

   xf86DrvMsg(pScrn->scrnIndex, from, "Chipset: \"%s\"\n",
	      (pScrn->chipset != NULL) ? pScrn->chipset : "Unknown ast");

   /* Resource Allocation */
#if XF86_VERSION_CURRENT < XF86_VERSION_NUMERIC(4,2,99,0,0)
    pAST->IODBase = 0;
#else
    pAST->IODBase = pScrn->domainIOBase;  
#endif
    /* "Patch" the PIOOffset inside vgaHW in order to force
     * the vgaHW module to use our relocated i/o ports.
     */
    VGAHWPTR(pScrn)->PIOOffset = pAST->PIOOffset = pAST->IODBase + pAST->PciInfo->ioBase[2] - 0x380;
	
    pAST->RelocateIO = (IOADDRESS)(pAST->PciInfo->ioBase[2] + pAST->IODBase);
	
   if (pAST->pEnt->device->MemBase != 0) {
      pAST->FBPhysAddr = pAST->pEnt->device->MemBase;
      from = X_CONFIG;
   } else {
      if (pAST->PciInfo->memBase[0] != 0) {
	 pAST->FBPhysAddr = pAST->PciInfo->memBase[0] & 0xFFF00000;
	 from = X_PROBED;
      } else {
	 xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "No valid FB address in PCI config space\n");
	 ASTFreeRec(pScrn);
	 return FALSE;
      }
   }
   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Linear framebuffer at 0x%lX\n",
	      (unsigned long) pAST->FBPhysAddr);

   if (pAST->pEnt->device->IOBase != 0) {
      pAST->MMIOPhysAddr = pAST->pEnt->device->IOBase;
      from = X_CONFIG;
   } else {
      if (pAST->PciInfo->memBase[1]) {
	 pAST->MMIOPhysAddr = pAST->PciInfo->memBase[1] & 0xFFFF0000;
	 from = X_PROBED;
      } else {
	 xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		    "No valid MMIO address in PCI config space\n");
	 ASTFreeRec(pScrn);
	 return FALSE;
      }
   }
   xf86DrvMsg(pScrn->scrnIndex, X_INFO, "IO registers at addr 0x%lX\n",
	      (unsigned long) pAST->MMIOPhysAddr);
	      
   pScrn->videoRam = GetVRAMInfo(pScrn) / 1024;
   from = X_DEFAULT;


   if (pAST->pEnt->device->videoRam) {
      pScrn->videoRam = pAST->pEnt->device->videoRam;
      from = X_CONFIG;
   }

   pAST->FbMapSize = pScrn->videoRam * 1024;
   pAST->MMIOMapSize = DEFAULT_MMIO_SIZE;

   /* Map resource */
   if (!ASTMapMem(pScrn)) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Map FB Memory Failed \n");      	
      return FALSE;
   }
   
   if (!ASTMapMMIO(pScrn)) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Map Memory Map IO Failed \n");      	
      return FALSE;
   }

   pScrn->memPhysBase = (ULONG)pAST->FBPhysAddr;
   pScrn->fbOffset = 0;

   /* Do DDC 
    * should be done after xf86CollectOptions
    */
   pScrn->monitor->DDC = ASTDoDDC(pScrn, pAST->pEnt->index);    

   /* Mode Valid */
   clockRanges = xnfcalloc(sizeof(ClockRange), 1);
   clockRanges->next = NULL;
   clockRanges->minClock = 9500;
   clockRanges->maxClock = GetMaxDCLK(pScrn) * 1000;   
   clockRanges->clockIndex = -1;
   clockRanges->interlaceAllowed = FALSE;
   clockRanges->doubleScanAllowed = FALSE;
   
   i = xf86ValidateModes(pScrn, pScrn->monitor->Modes,
			 pScrn->display->modes, clockRanges,
			 0, 320, 1600, 8 * pScrn->bitsPerPixel,
			 200, 1200,
			 pScrn->display->virtualX, pScrn->display->virtualY,
			 pAST->FbMapSize, LOOKUP_BEST_REFRESH);

   if (i == -1) {
      ASTFreeRec(pScrn);
      return FALSE;
   }

   xf86PruneDriverModes(pScrn);

   if (!i || !pScrn->modes) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
      ASTFreeRec(pScrn);
      return FALSE;
   }

   xf86SetCrtcForModes(pScrn, INTERLACE_HALVE_V);

   pScrn->currentMode = pScrn->modes;

   xf86PrintModes(pScrn);

   xf86SetDpi(pScrn, 0, 0);

   /* Accelaration Check */
   pAST->noAccel = TRUE;
   pAST->AccelInfoPtr = NULL; 
   pAST->pCMDQPtr = NULL;   
   pAST->CMDQInfo.ulCMDQSize = 0;      
#ifdef	Accel_2D
   if (!xf86ReturnOptValBool(pAST->Options, OPTION_NOACCEL, FALSE))
   {
       if (!xf86LoadSubModule(pScrn, "xaa")) {
	   ASTFreeRec(pScrn);
	   return FALSE;
       }       
       xf86LoaderReqSymLists(xaaSymbols, NULL);
       
       pAST->noAccel = FALSE; 
       
       pAST->MMIO2D = TRUE;
#ifndef	MMIO_2D                   
       if (!xf86ReturnOptValBool(pAST->Options, OPTION_MMIO2D, FALSE)) {
           pAST->CMDQInfo.ulCMDQSize = DEFAULT_CMDQ_SIZE;       
           pAST->MMIO2D = FALSE;    	
       }	
#endif

       pAST->ENGCaps = ENG_CAP_ALL;
       if (!xf86GetOptValInteger(pAST->Options, OPTION_ENG_CAPS, &pAST->ENGCaps)) {
           xf86DrvMsg(pScrn->scrnIndex, X_INFO, "No ENG Capability options found\n");      	
       }
       
       pAST->DBGSelect = 0;
       if (!xf86GetOptValInteger(pAST->Options, OPTION_DBG_SELECT, &pAST->DBGSelect)) {
           xf86DrvMsg(pScrn->scrnIndex, X_INFO, "No DBG Seleclt options found\n");      	
       }	       
   }
#endif   

   /* HW Cursor Check */
   pAST->noHWC = TRUE; 
   pAST->HWCInfoPtr = NULL;
   pAST->pHWCPtr = NULL;    
#ifdef	HWC   
   if (!xf86ReturnOptValBool(pAST->Options, OPTION_SW_CURSOR, FALSE)) {
      if (!xf86LoadSubModule(pScrn, "ramdac")) {
	 ASTFreeRec(pScrn);
	 return FALSE;
      }
      xf86LoaderReqSymLists(ramdacSymbols, NULL);
      
      pAST->noHWC = FALSE;  
      pAST->HWCInfo.HWC_NUM = DEFAULT_HWC_NUM;
      if (!xf86GetOptValInteger(pAST->Options, OPTION_HWC_NUM, &pAST->HWCInfo.HWC_NUM)) {
          xf86DrvMsg(pScrn->scrnIndex, X_INFO, "No HWC_NUM options found\n");      	
      }	
             
   }    
#endif

   /*  We won't be using the VGA access after the probe */
   xf86SetOperatingState(resVgaIo, pAST->pEnt->index, ResUnusedOpr);
   xf86SetOperatingState(resVgaMem, pAST->pEnt->index, ResDisableOpr);

   return TRUE;
}


static Bool
ASTScreenInit(int scrnIndex, ScreenPtr pScreen, int argc, char **argv)
{
   ScrnInfoPtr pScrn;
   ASTRecPtr pAST;
   vgaHWPtr hwp;   
   VisualPtr visual;
 
   /* for FB Manager */
   BoxRec FBMemBox;   
   int    AvailFBSize;     

   pScrn = xf86Screens[pScreen->myNum];
   pAST = ASTPTR(pScrn);
   hwp = VGAHWPTR(pScrn);
      
/*   if (!pAST->noAccel) */
   {
       /* AvailFBSize = pAST->FbMapSize - pAST->CMDQInfo.ulCMDQSize; */
       AvailFBSize = pAST->FbMapSize;
   
       FBMemBox.x1 = 0;
       FBMemBox.y1 = 0;
       FBMemBox.x2 = pScrn->displayWidth;
       FBMemBox.y2 = (AvailFBSize / (pScrn->displayWidth * ((pScrn->bitsPerPixel+1)/8))) - 1;

       if (!xf86InitFBManager(pScreen, &FBMemBox)) {
          xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to init memory manager\n");
          return FALSE;
       }      

   }
       
   vgaHWGetIOBase(hwp);

   vFillASTModeInfo (pScrn);      

   ASTSave(pScrn);     
   if (!ASTModeInit(pScrn, pScrn->currentMode)) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Mode Init Failed \n");      	  	
      return FALSE;
   }   

   ASTSaveScreen(pScreen, FALSE);
   ASTAdjustFrame(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

   miClearVisualTypes();

   /* Re-implemented Direct Color support, -jens */
   if (!miSetVisualTypes(pScrn->depth, miGetDefaultVisualMask(pScrn->depth),
			 pScrn->rgbBits, pScrn->defaultVisual))
      return FALSE;

   if (!miSetPixmapDepths())
   {
       ASTSaveScreen(pScreen, SCREEN_SAVER_OFF);
       return FALSE;
   }    

   switch(pScrn->bitsPerPixel) {
       case 8:
       case 16:
       case 32:
           if (!fbScreenInit(pScreen, pAST->FBVirtualAddr + pScrn->fbOffset,
  	                     pScrn->virtualX, pScrn->virtualY,
		             pScrn->xDpi, pScrn->yDpi,
		             pScrn->displayWidth, pScrn->bitsPerPixel))
               return FALSE;
           break;
       default:
           return FALSE;    
              
   }

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
     
   fbPictureInit(pScreen, 0, 0);

   xf86SetBlackWhitePixels(pScreen);

#ifdef Accel_2D
   if (!pAST->noAccel)
   {
       if (!ASTAccelInit(pScreen)) {
           xf86DrvMsg(pScrn->scrnIndex, X_ERROR,"Hardware acceleration initialization failed\n");
           pAST->noAccel = TRUE;           
       }
   }
#endif /* end of Accel_2D */
     
   miInitializeBackingStore(pScreen);
   xf86SetBackingStore(pScreen);
   xf86SetSilkenMouse(pScreen);

   miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

   if (!pAST->noHWC)
   {
       if (!ASTCursorInit(pScreen)) {
           xf86DrvMsg(pScrn->scrnIndex, X_ERROR,"Hardware cursor initialization failed\n");
           pAST->noHWC = TRUE;                      
       }
   }
   
   if (!miCreateDefColormap(pScreen))
      return FALSE;

   if(!xf86HandleColormaps(pScreen, 256, (pScrn->depth == 8) ? 8 : pScrn->rgbBits,
                    vASTLoadPalette, NULL,
                    CMAP_PALETTED_TRUECOLOR | CMAP_RELOAD_ON_MODE_SWITCH)) {
       return FALSE;
   }
   
   xf86DPMSInit(pScreen, ASTDisplayPowerManagementSet, 0);
   
   pScreen->SaveScreen = ASTSaveScreen;
   pAST->CloseScreen = pScreen->CloseScreen;
   pScreen->CloseScreen = ASTCloseScreen;

   if (serverGeneration == 1)
      xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);

   return TRUE;
   	
} /* ASTScreenInit */


Bool
ASTSwitchMode(int scrnIndex, DisplayModePtr mode, int flags)
{
   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
   ASTRecPtr pAST = ASTPTR(pScrn);

#ifdef	HWC
   if (pAST->pHWCPtr) {
       xf86FreeOffscreenLinear(pAST->pHWCPtr);		/* free HWC Cache */
       pAST->pHWCPtr = NULL;      
   }    
#endif

#ifdef Accel_2D 
   if (pAST->pCMDQPtr) {
       xf86FreeOffscreenLinear(pAST->pCMDQPtr);		/* free CMDQ */
       pAST->pCMDQPtr = NULL;             
   } 
   vDisable2D(pScrn, pAST);
#endif
   
   ASTRestore(pScrn);
   
   return ASTModeInit(pScrn, mode);

}

void
ASTAdjustFrame(int scrnIndex, int x, int y, int flags)
{
   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
   ASTRecPtr   pAST  = ASTPTR(pScrn);
   ULONG base;
      
   base = y * pAST->VideoModeInfo.ScreenWidth + x * ((pAST->VideoModeInfo.bitsPerPixel + 1) / 8);
   base = base >> 2;				/* DW unit */

   vSetStartAddressCRT1(pAST, base);

}

/* enter into X Server */		
static Bool
ASTEnterVT(int scrnIndex, int flags)
{
   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];

   if (!ASTModeInit(pScrn, pScrn->currentMode))
      return FALSE;
   ASTAdjustFrame(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);
   
   return TRUE;
   	
}

/* leave X server */
static void
ASTLeaveVT(int scrnIndex, int flags)
{
	
   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
   vgaHWPtr hwp = VGAHWPTR(pScrn);
   ASTRecPtr pAST = ASTPTR(pScrn);

#ifdef	HWC
   if (pAST->pHWCPtr) {
       xf86FreeOffscreenLinear(pAST->pHWCPtr);		/* free HWC Cache */
       pAST->pHWCPtr = NULL;      
   }    
#endif

#ifdef Accel_2D  
   if (pAST->pCMDQPtr) {
       xf86FreeOffscreenLinear(pAST->pCMDQPtr);		/* free CMDQ */
       pAST->pCMDQPtr = NULL;             
   }    
   vDisable2D(pScrn, pAST);
#endif
      
   ASTRestore(pScrn);  
   vgaHWLock(hwp);	

}

static void
ASTFreeScreen(int scrnIndex, int flags)
{
   ASTFreeRec(xf86Screens[scrnIndex]);
   if (xf86LoaderCheckSymbol("vgaHWFreeHWRec"))
      vgaHWFreeHWRec(xf86Screens[scrnIndex]);   
}


static ModeStatus
ASTValidMode(int scrnIndex, DisplayModePtr mode, Bool verbose, int flags)
{

   Bool Flags = MODE_NOMODE;
   	
   if (mode->Flags & V_INTERLACE) {
      if (verbose) {
	 xf86DrvMsg(scrnIndex, X_PROBED,
		    "Removing interlaced mode \"%s\"\n", mode->name);
      }
      return MODE_NO_INTERLACE;
   }

   if ((mode->CrtcHDisplay > MAX_HResolution) || (mode->CrtcVDisplay > MAX_VResolution)) {
      if (verbose) {
	 xf86DrvMsg(scrnIndex, X_PROBED,
		    "Removing the mode \"%s\"\n", mode->name);
      }   	
      return Flags;   	
   }
   
   switch (mode->CrtcHDisplay)
   {
   case 640:
       if (mode->CrtcVDisplay == 480) Flags=MODE_OK;
       break;        
   case 800:
       if (mode->CrtcVDisplay == 600) Flags=MODE_OK;
       break;   
   case 1024:
       if (mode->CrtcVDisplay == 768) Flags=MODE_OK;
       break;   
   case 1280:
       if (mode->CrtcVDisplay == 1024) Flags=MODE_OK;
       break;
   case 1600:
       if (mode->CrtcVDisplay == 1200) Flags=MODE_OK;
       break;   
   default:
       return Flags;   	   	
   }
   
   return Flags;
   
}			


/* Internal used modules */
/*
 * ASTGetRec and ASTFreeRec --
 *
 * Private data for the driver is stored in the screen structure.
 * These two functions create and destroy that private data.
 *
 */
static Bool
ASTGetRec(ScrnInfoPtr pScrn)
{
   if (pScrn->driverPrivate)
      return TRUE;

   pScrn->driverPrivate = xnfcalloc(sizeof(ASTRec), 1);
   return TRUE;
}

static void
ASTFreeRec(ScrnInfoPtr pScrn)
{
   if (!pScrn)
      return;
   if (!pScrn->driverPrivate)
      return;
   xfree(pScrn->driverPrivate);
   pScrn->driverPrivate = 0;
}

static Bool
ASTSaveScreen(ScreenPtr pScreen, Bool unblack)
{
   /* more ref. SiS */	
   return vgaHWSaveScreen(pScreen, unblack);
}

static Bool
ASTCloseScreen(int scrnIndex, ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
   vgaHWPtr hwp = VGAHWPTR(pScrn);
   ASTRecPtr pAST = ASTPTR(pScrn);

   if (pScrn->vtSema == TRUE)
   {  
#ifdef	HWC
   if (pAST->pHWCPtr) {
       xf86FreeOffscreenLinear(pAST->pHWCPtr);		/* free HWC Cache */
       pAST->pHWCPtr = NULL;      
   }    
#endif
   	   
#ifdef Accel_2D  
   if (pAST->pCMDQPtr) {
       xf86FreeOffscreenLinear(pAST->pCMDQPtr);		/* free CMDQ */
       pAST->pCMDQPtr = NULL;      
   }
   vDisable2D(pScrn, pAST);
#endif
         
      ASTRestore(pScrn);
      vgaHWLock(hwp);
   }

   ASTUnmapMem(pScrn);
   vgaHWUnmapMem(pScrn);

   if(pAST->AccelInfoPtr) {
       XAADestroyInfoRec(pAST->AccelInfoPtr);
       pAST->AccelInfoPtr = NULL;
   }

   if(pAST->HWCInfoPtr) {
       xf86DestroyCursorInfoRec(pAST->HWCInfoPtr);
       pAST->HWCInfoPtr = NULL;
   }

   pScrn->vtSema = FALSE;
   pScreen->CloseScreen = pAST->CloseScreen;
   return (*pScreen->CloseScreen) (scrnIndex, pScreen);
}

static void
ASTSave(ScrnInfoPtr pScrn)
{
   ASTRecPtr pAST;
   vgaRegPtr vgaReg;
   ASTRegPtr astReg;   
   int i;

   pAST = ASTPTR(pScrn);
   vgaReg = &VGAHWPTR(pScrn)->SavedReg;
   astReg = &pAST->SavedReg;
    
   /* do save */    
   vgaHWSave(pScrn, vgaReg, VGA_SR_ALL);
   
   /* Ext. Save */
   vASTOpenKey(pScrn);
   
   for (i=0; i<0x50; i++)
       GetIndexReg(CRTC_PORT, (UCHAR) (i+0x80), astReg->ExtCRTC[i]);   	
   
}

static void
ASTRestore(ScrnInfoPtr pScrn)
{
   ASTRecPtr pAST;
   vgaRegPtr vgaReg;
   ASTRegPtr astReg;   
   int i;

   pAST = ASTPTR(pScrn);
   vgaReg = &VGAHWPTR(pScrn)->SavedReg;
   astReg = &pAST->SavedReg;
    
   /* do restore */    
   vgaHWProtect(pScrn, TRUE);   
   vgaHWRestore(pScrn, vgaReg, VGA_SR_ALL);	      
   vgaHWProtect(pScrn, FALSE);   
   
   /* Ext. restore */
   vASTOpenKey(pScrn);
   
   for (i=0; i<0x50; i++)
       SetIndexReg(CRTC_PORT, (UCHAR) (i+0x80), astReg->ExtCRTC[i]);   	
   
}

static void
ASTProbeDDC(ScrnInfoPtr pScrn, int index)
{
   vbeInfoPtr pVbe;

   if (xf86LoadSubModule(pScrn, "vbe")) {
      pVbe = VBEInit(NULL, index);
      ConfiguredMonitor = vbeDoEDID(pVbe, NULL);
      vbeFree(pVbe);
   }
}

static xf86MonPtr
ASTDoDDC(ScrnInfoPtr pScrn, int index)
{
   vbeInfoPtr pVbe;
   xf86MonPtr MonInfo = NULL;
   ASTRecPtr pAST = ASTPTR(pScrn);

   /* Honour Option "noDDC" */
   if (xf86ReturnOptValBool(pAST->Options, OPTION_NO_DDC, FALSE)) {
      return MonInfo;
   }

   if (xf86LoadSubModule(pScrn, "vbe") && (pVbe = VBEInit(NULL, index))) {
      xf86LoaderReqSymLists(vbeSymbols, NULL);
      MonInfo = vbeDoEDID(pVbe, NULL);
      xf86PrintEDID(MonInfo);
      xf86SetDDCproperties(pScrn, MonInfo);
      vbeFree(pVbe);
   } else {
      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		 "this driver cannot do DDC without VBE\n");
   }

   return MonInfo;
}

static void
vFillASTModeInfo (ScrnInfoPtr pScrn)
{
    ASTRecPtr pAST;
    
    pAST = ASTPTR(pScrn);
    
    pAST->VideoModeInfo.ScreenWidth = pScrn->virtualX;   
    pAST->VideoModeInfo.ScreenHeight = pScrn->virtualY;   
    pAST->VideoModeInfo.bitsPerPixel = pScrn->bitsPerPixel;   
    pAST->VideoModeInfo.ScreenPitch = pScrn->virtualX * ((pScrn->bitsPerPixel + 1) / 8) ;       

}

static Bool
ASTModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp;
    ASTRecPtr pAST;

    hwp = VGAHWPTR(pScrn);
    pAST = ASTPTR(pScrn);

    vgaHWUnlock(hwp);

    if (!vgaHWInit(pScrn, mode))
      return FALSE;

    pScrn->vtSema = TRUE;
    pAST->ModePtr = mode;

    if (!ASTSetMode(pScrn, mode))
      return FALSE;
    
    vgaHWProtect(pScrn, FALSE);

    return TRUE;
}
