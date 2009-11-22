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
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#include "xf86RAC.h"
#endif
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
extern void GetDRAMInfo(ScrnInfoPtr pScrn);
extern ULONG GetVRAMInfo(ScrnInfoPtr pScrn);
extern ULONG GetMaxDCLK(ScrnInfoPtr pScrn);
extern void GetChipType(ScrnInfoPtr pScrn);
extern void vASTLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors, VisualPtr pVisual);
extern void ASTDisplayPowerManagementSet(ScrnInfoPtr pScrn, int PowerManagementMode, int flags);
extern void vSetStartAddressCRT1(ASTRecPtr pAST, ULONG base);
extern Bool ASTSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode);
extern Bool GetVGA2EDID(ScrnInfoPtr pScrn, unsigned char *pEDIDBuffer);
extern void vInitDRAMReg(ScrnInfoPtr pScrn);
extern Bool bIsVGAEnabled(ScrnInfoPtr pScrn);
extern void ASTBlankScreen(ScrnInfoPtr pScreen, Bool unblack);
extern Bool InitVGA(ScrnInfoPtr pScrn);

extern Bool bInitCMDQInfo(ScrnInfoPtr pScrn, ASTRecPtr pAST);
extern Bool bEnableCMDQ(ScrnInfoPtr pScrn, ASTRecPtr pAST);
extern void vDisable2D(ScrnInfoPtr pScrn, ASTRecPtr pAST);

extern Bool ASTAccelInit(ScreenPtr pScreen);

extern Bool ASTCursorInit(ScreenPtr pScreen);
extern void ASTHideCursor(ScrnInfoPtr pScrn);

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
_X_EXPORT DriverRec AST = {
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
   {PCI_CHIP_AST2000,	"ASPEED Graphics Family"},
   {PCI_CHIP_AST2100,	"ASPEED Graphics Family"},
   {-1,			NULL}
};

static PciChipsets ASTPciChipsets[] = {
   {PCI_CHIP_AST2000,		PCI_CHIP_AST2000,	RES_SHARED_VGA},
   {PCI_CHIP_AST2100,		PCI_CHIP_AST2100,	RES_SHARED_VGA},
   {-1,				-1, 			RES_UNDEFINED }
};

typedef enum {
   OPTION_NOACCEL,
   OPTION_MMIO2D,   
   OPTION_SW_CURSOR,
   OPTION_HWC_NUM,
   OPTION_ENG_CAPS,   
   OPTION_DBG_SELECT,
   OPTION_NO_DDC,
   OPTION_VGA2_CLONE
} ASTOpts;

static const OptionInfoRec ASTOptions[] = {
   {OPTION_NOACCEL,	"NoAccel",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_MMIO2D,	"MMIO2D",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_SW_CURSOR,	"SWCursor",	OPTV_BOOLEAN,	{0},	FALSE},
   {OPTION_HWC_NUM,	"HWCNumber",	OPTV_INTEGER,	{0},	FALSE},
   {OPTION_ENG_CAPS,	"ENGCaps",	OPTV_INTEGER,	{0},	FALSE},
   {OPTION_DBG_SELECT,	"DBGSelect",	OPTV_INTEGER,	{0},	FALSE},
   {OPTION_NO_DDC,	"NoDDC",	OPTV_BOOLEAN,	{0}, 	FALSE},
   {OPTION_VGA2_CLONE,	"VGA2Clone",	OPTV_BOOLEAN,	{0}, 	FALSE},
   {-1,			NULL,		OPTV_NONE,	{0}, 	FALSE}
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
#ifdef PATCH_ABI_VERSION
   ABI_VIDEODRV_VERSION_PATCH,
#else 
   ABI_VIDEODRV_VERSION,
#endif
   MOD_CLASS_VIDEODRV,
   {0, 0, 0, 0}
};

_X_EXPORT XF86ModuleData astModuleData = { &astVersRec, astSetup, NULL };

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

#ifndef XSERVER_LIBPCIACCESS
   /*
    * This probing is just checking the PCI data the server already
    * collected.
    */
    if (xf86GetPciVideoInfo() == NULL) {
	return FALSE;
    }
#endif

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

#ifndef XSERVER_LIBPCIACCESS
   if (xf86RegisterResources(pEnt->index, 0, ResExclusive))
       return FALSE;
#endif

   /* The vgahw module should be loaded here when needed */
   if (!xf86LoadSubModule(pScrn, "vgahw"))
      return FALSE;

   /* The fb module should be loaded here when needed */
   if (!xf86LoadSubModule(pScrn, "fb"))
      return FALSE;
   	
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
#ifndef XSERVER_LIBPCIACCESS
   pScrn->racMemFlags = RAC_FB | RAC_COLORMAP | RAC_CURSOR | RAC_VIEWPORT;
   pScrn->racIoFlags = RAC_COLORMAP | RAC_CURSOR | RAC_VIEWPORT;   
#endif
      
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
#ifndef XSERVER_LIBPCIACCESS
   pAST->PciTag  = pciTag(pAST->PciInfo->bus, pAST->PciInfo->device,
			  pAST->PciInfo->func);
#endif

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
						 PCI_DEV_DEVICE_ID(pAST->PciInfo));
   }
   if (pAST->pEnt->device->chipRev >= 0) {
      xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipRev override: %d\n",
		 pAST->pEnt->device->chipRev);
   }

   xf86DrvMsg(pScrn->scrnIndex, from, "Chipset: \"%s\"\n",
	      (pScrn->chipset != NULL) ? pScrn->chipset : "Unknown ast");

   /* Resource Allocation */
    pAST->IODBase = pScrn->domainIOBase;  
    /* "Patch" the PIOOffset inside vgaHW in order to force
     * the vgaHW module to use our relocated i/o ports.
     */
    VGAHWPTR(pScrn)->PIOOffset = pAST->PIOOffset = pAST->IODBase + PCI_REGION_BASE(pAST->PciInfo, 2, REGION_IO) - 0x380;
	
    pAST->RelocateIO = (IOADDRESS)(PCI_REGION_BASE(pAST->PciInfo, 2, REGION_IO) + pAST->IODBase);
	
   if (pAST->pEnt->device->MemBase != 0) {
      pAST->FBPhysAddr = pAST->pEnt->device->MemBase;
      from = X_CONFIG;
   } else {
      if (PCI_REGION_BASE(pAST->PciInfo, 0, REGION_MEM) != 0) {
	 pAST->FBPhysAddr = PCI_REGION_BASE(pAST->PciInfo, 0, REGION_MEM) & 0xFFF00000;
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
      if (PCI_REGION_BASE(pAST->PciInfo, 1, REGION_MEM)) {
	 pAST->MMIOPhysAddr = PCI_REGION_BASE(pAST->PciInfo, 1, REGION_MEM) & 0xFFFF0000;
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
	      
   /* Map MMIO */
   pAST->MMIOMapSize = DEFAULT_MMIO_SIZE; 
   if (!ASTMapMMIO(pScrn)) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Map Memory Map IO Failed \n");      	
      return FALSE;
   }

   /* Init VGA Adapter */
   if (!xf86IsPrimaryPci(pAST->PciInfo))
   {
       InitVGA(pScrn);      	
   }

   vASTOpenKey(pScrn);
   bASTRegInit(pScrn);

   /* Get Chip Type */
   if (PCI_DEV_REVISION(pAST->PciInfo) >= 0x10)
       GetChipType(pScrn);
   else
       pAST->jChipType = AST2000;

   /* Get DRAM Info */
   GetDRAMInfo(pScrn);
      
   /* Map Framebuffer */
   pScrn->videoRam = GetVRAMInfo(pScrn) / 1024;
   from = X_DEFAULT;

   if (pAST->pEnt->device->videoRam) {
      pScrn->videoRam = pAST->pEnt->device->videoRam;
      from = X_CONFIG;
   }
   
   pAST->FbMapSize = pScrn->videoRam * 1024;
   
#if 0   
   if (!ASTMapMem(pScrn)) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Map FB Memory Failed \n");      	
      return FALSE;
   }
#endif

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
   
   /* Add for AST2100, ycchen@061807 */
   if ((pAST->jChipType == AST2100) || (pAST->jChipType == AST2200))
       i = xf86ValidateModes(pScrn, pScrn->monitor->Modes,
			 pScrn->display->modes, clockRanges,
			 0, 320, 1920, 8 * pScrn->bitsPerPixel,
			 200, 1200,
			 pScrn->display->virtualX, pScrn->display->virtualY,
			 pAST->FbMapSize, LOOKUP_BEST_REFRESH);
   else
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
      
      pAST->noHWC = FALSE;  
      pAST->HWCInfo.HWC_NUM = DEFAULT_HWC_NUM;
      if (!xf86GetOptValInteger(pAST->Options, OPTION_HWC_NUM, &pAST->HWCInfo.HWC_NUM)) {
          xf86DrvMsg(pScrn->scrnIndex, X_INFO, "No HWC_NUM options found\n");      	
      }	
             
   }    
#endif

#ifndef XSERVER_LIBPCIACCESS
   /*  We won't be using the VGA access after the probe */
   xf86SetOperatingState(resVgaIo, pAST->pEnt->index, ResUnusedOpr);
   xf86SetOperatingState(resVgaMem, pAST->pEnt->index, ResDisableOpr);
#endif

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

   if (!ASTMapMem(pScrn)) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Map FB Memory Failed \n");      	
      return FALSE;
   }
      
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
   ASTHideCursor(pScrn);
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

   /* Fixed suspend can't resume issue */
   if (!bIsVGAEnabled(pScrn))
   {
       InitVGA(pScrn);      	   	
       ASTRestore(pScrn);
   }   

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
   ASTHideCursor(pScrn);
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

   ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
   ASTRecPtr   pAST  = ASTPTR(pScrn);
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

   /* Add for AST2100, ycchen@061807 */
   if ((pAST->jChipType == AST2100) || (pAST->jChipType == AST2200))
   {
       if ( (mode->CrtcHDisplay == 1920) && (mode->CrtcVDisplay == 1200) )
           return MODE_OK;
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
   /* replacement of vgaHWBlankScreen(pScrn, unblank) without seq reset */
   /* return vgaHWSaveScreen(pScreen, unblack); */   
   ScrnInfoPtr pScrn = NULL;

   if (pScreen != NULL)
      pScrn = xf86Screens[pScreen->myNum];

   if ((pScrn != NULL) && pScrn->vtSema) {
     ASTBlankScreen(pScrn, unblack);
   }
   return (TRUE);   
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
   ASTHideCursor(pScrn);
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
   int i, icount=0;

   pAST = ASTPTR(pScrn);
   vgaReg = &VGAHWPTR(pScrn)->SavedReg;
   astReg = &pAST->SavedReg;
    
   /* do save */    
   if (xf86IsPrimaryPci(pAST->PciInfo)) {
       vgaHWSave(pScrn, vgaReg, VGA_SR_ALL);
   }
   else {
       vgaHWSave(pScrn, vgaReg, VGA_SR_MODE);
   }
   
   /* Ext. Save */
   vASTOpenKey(pScrn);
   
   /* fixed Console Switch Refresh Rate Incorrect issue, ycchen@051106 */   
   for (i=0x81; i<=0xB6; i++)
       GetIndexReg(CRTC_PORT, (UCHAR) (i), astReg->ExtCRTC[icount++]);
   for (i=0xBC; i<=0xC1; i++)
       GetIndexReg(CRTC_PORT, (UCHAR) (i), astReg->ExtCRTC[icount++]);
   GetIndexReg(CRTC_PORT, (UCHAR) (0xBB), astReg->ExtCRTC[icount]);

}

static void
ASTRestore(ScrnInfoPtr pScrn)
{
   ASTRecPtr pAST;
   vgaRegPtr vgaReg;
   ASTRegPtr astReg;   
   int i, icount=0;

   pAST = ASTPTR(pScrn);
   vgaReg = &VGAHWPTR(pScrn)->SavedReg;
   astReg = &pAST->SavedReg;
    
   /* do restore */    
   vgaHWProtect(pScrn, TRUE);
   if (xf86IsPrimaryPci(pAST->PciInfo))
       vgaHWRestore(pScrn, vgaReg, VGA_SR_ALL);
   else
       vgaHWRestore(pScrn, vgaReg, VGA_SR_MODE);     
   vgaHWProtect(pScrn, FALSE);   
   
   /* Ext. restore */
   vASTOpenKey(pScrn);
   
   /* fixed Console Switch Refresh Rate Incorrect issue, ycchen@051106 */
   for (i=0x81; i<=0xB6; i++)
       SetIndexReg(CRTC_PORT, (UCHAR) (i), astReg->ExtCRTC[icount++]);
   for (i=0xBC; i<=0xC1; i++)
       SetIndexReg(CRTC_PORT, (UCHAR) (i), astReg->ExtCRTC[icount++]);
   SetIndexReg(CRTC_PORT, (UCHAR) (0xBB), astReg->ExtCRTC[icount]);

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

#define SkipDT	0x00
#define DT1	0x01
#define DT2 	0x02
	
static xf86MonPtr
ASTDoDDC(ScrnInfoPtr pScrn, int index)
{
   vbeInfoPtr pVbe;
   xf86MonPtr MonInfo = NULL, MonInfo1 = NULL, MonInfo2 = NULL;
   ASTRecPtr pAST = ASTPTR(pScrn);
   unsigned long i, j, k;
   unsigned char DDC_data[128];
   struct monitor_ranges ranges, ranges1, ranges2;
   int DTSelect, dclock1=0, h_active1=0, v_active1=0, dclock2=0, h_active2=0, v_active2=0;
   struct std_timings stdtiming, *stdtiming1, *stdtiming2;
   
   /* Honour Option "noDDC" */
   if (xf86ReturnOptValBool(pAST->Options, OPTION_NO_DDC, FALSE)) {
      return MonInfo;
   }

   if (xf86LoadSubModule(pScrn, "vbe") && (pVbe = VBEInit(NULL, index))) {
      MonInfo1 = vbeDoEDID(pVbe, NULL);
      MonInfo = MonInfo1;
      
      /* For VGA2 CLONE Support, ycchen@012508 */
      if ((xf86ReturnOptValBool(pAST->Options, OPTION_VGA2_CLONE, FALSE)) || pAST->VGA2Clone) {
          if (GetVGA2EDID(pScrn, DDC_data) == TRUE) {
              xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Get VGA2 EDID Correctly!! \n");	
              MonInfo2 = xf86InterpretEDID(pScrn->scrnIndex, DDC_data);
              if (MonInfo1 == NULL)	/* No DDC1 EDID */
                  MonInfo = MonInfo2;
              else {			/* Check with VGA1 & VGA2 EDID */
                  /* Update establishment timing */
                  MonInfo->timings1.t1 = MonInfo1->timings1.t1 & MonInfo2->timings1.t1;
                  MonInfo->timings1.t2 = MonInfo1->timings1.t2 & MonInfo2->timings1.t2;
                  MonInfo->timings1.t_manu = MonInfo1->timings1.t_manu & MonInfo2->timings1.t_manu;
                  
                  /* Update Std. Timing */
                  for (i=0; i<8; i++) {
	              stdtiming.hsize = stdtiming.vsize = stdtiming.refresh = stdtiming.id = 0;	              
                      for (j=0; j<8; j++) {                      	
                          if ((MonInfo1->timings2[i].hsize == MonInfo2->timings2[j].hsize) && \
                              (MonInfo1->timings2[i].vsize == MonInfo2->timings2[j].vsize) && \
                              (MonInfo1->timings2[i].refresh == MonInfo2->timings2[j].refresh)) {
                               stdtiming = MonInfo1->timings2[i];
                               break;
                          }        
                      }
                      
                      MonInfo->timings2[i] = stdtiming;
                  } /* Std. Timing */
               
                  /* Get Detailed Timing */
                  for (i=0;i<4;i++) {
                    if (MonInfo1->det_mon[i].type == 0xFD)
                        ranges1 = MonInfo1->det_mon[i].section.ranges;
                    else if (MonInfo1->det_mon[i].type == 0xFA)
                        stdtiming1 = MonInfo1->det_mon[i].section.std_t;    
                    else if (MonInfo1->det_mon[i].type == 0x00) {
                        if (MonInfo1->det_mon[i].section.d_timings.clock > dclock1)
                            dclock1 = MonInfo1->det_mon[i].section.d_timings.clock;
                        if (MonInfo1->det_mon[i].section.d_timings.h_active > h_active1)
                            h_active1 = MonInfo1->det_mon[i].section.d_timings.h_active;
                        if (MonInfo1->det_mon[i].section.d_timings.v_active > v_active1)
                            v_active1 = MonInfo1->det_mon[i].section.d_timings.v_active;                            
                    }	
                    if (MonInfo2->det_mon[i].type == 0xFD)
                        ranges2 = MonInfo2->det_mon[i].section.ranges;
                    else if (MonInfo1->det_mon[i].type == 0xFA)
                        stdtiming2 = MonInfo2->det_mon[i].section.std_t;                        
                    else if (MonInfo2->det_mon[i].type == 0x00) {
                        if (MonInfo2->det_mon[i].section.d_timings.clock > dclock2)
                            dclock2 = MonInfo2->det_mon[i].section.d_timings.clock;
                        if (MonInfo2->det_mon[i].section.d_timings.h_active > h_active2)
                            h_active2 = MonInfo2->det_mon[i].section.d_timings.h_active;
                        if (MonInfo2->det_mon[i].section.d_timings.v_active > v_active2)
                            v_active2 = MonInfo2->det_mon[i].section.d_timings.v_active;                            
                    }                                                            	 
                  } /* Get Detailed Timing */

                  /* Chk Detailed Timing */
                  if ((dclock1 >= dclock2) && (h_active1 >= h_active2) && (v_active1 >= v_active2))
                      DTSelect = DT2;
                  else if ((dclock2 >= dclock1) && (h_active2 >= h_active1) && (v_active2 >= v_active1))
                      DTSelect = DT1;
                  else
                      DTSelect = SkipDT;

                  /* Chk Monitor Descriptor */    
                  ranges = ranges1;
                  ranges.min_h = ranges1.min_h > ranges2.min_h ? ranges1.min_h:ranges2.min_h;
                  ranges.min_v = ranges1.min_v > ranges2.min_v ? ranges1.min_v:ranges2.min_v;                  
                  ranges.max_h = ranges1.max_h < ranges2.max_h ? ranges1.max_h:ranges2.max_h;
                  ranges.max_v = ranges1.max_v < ranges2.max_v ? ranges1.max_v:ranges2.max_v;
                  ranges.max_clock = ranges1.max_clock < ranges2.max_clock ? ranges1.max_clock:ranges2.max_clock;
                  
                  /* Update Detailed Timing */
                  for (i=0; i<4; i++)
                  {
                      if (MonInfo->det_mon[i].type == 0xFD) {
                          MonInfo->det_mon[i].section.ranges = ranges;
                      }                      
                      else if (MonInfo->det_mon[i].type == 0xFA) {
                         for (j=0; j<5; j++) {
            	             stdtiming.hsize = stdtiming.vsize = stdtiming.refresh = stdtiming.id = 0;
                             for (k=0; k<5; k++) {
                                 if ((stdtiming1[j].hsize == stdtiming2[k].hsize) && \
                                     (stdtiming1[j].vsize == stdtiming2[k].vsize) && \
                                     (stdtiming1[j].refresh == stdtiming2[k].refresh)) {
                                      stdtiming = stdtiming1[j];
                                      break;
                                 }        
                             }
                             stdtiming1[j] = stdtiming;
                         } /* Std. Timing */                                                    
                      } /* FA */
                      else if (MonInfo->det_mon[i].type == 0x00) {
                          if (DTSelect == DT2)
                              MonInfo->det_mon[i] = MonInfo2->det_mon[i];
                          else if (DTSelect == DT1)
                              MonInfo->det_mon[i] = MonInfo1->det_mon[i];
                          else /* SkipDT */
                          {   /* use 1024x768 as default */
                              MonInfo->det_mon[i] = MonInfo1->det_mon[i];
                              MonInfo->det_mon[i].section.d_timings.clock = 65000000;
                              MonInfo->det_mon[i].section.d_timings.h_active = 1024;
                              MonInfo->det_mon[i].section.d_timings.h_blanking = 320;
                              MonInfo->det_mon[i].section.d_timings.v_active = 768;
                              MonInfo->det_mon[i].section.d_timings.v_blanking = 38;
                              MonInfo->det_mon[i].section.d_timings.h_sync_off = 24;
                              MonInfo->det_mon[i].section.d_timings.h_sync_width = 136;
                              MonInfo->det_mon[i].section.d_timings.v_sync_off = 3;
                              MonInfo->det_mon[i].section.d_timings.v_sync_width = 6;
                          }                                                	
                      } /* 00 */
                      else { /* use Monitor 1 as default */
                          MonInfo->det_mon[i] = MonInfo1->det_mon[i];                      
                      }
                
                  } /* Update Detailed Timing */
                  
                  /* set feature size */
                  if (DTSelect == DT2)  {
                      MonInfo->features.hsize = MonInfo2->features.hsize;
                      MonInfo->features.vsize = MonInfo2->features.vsize;                          	
                  }
                  else if (DTSelect == DT1)  {
                      MonInfo->features.hsize = MonInfo1->features.hsize;
                      MonInfo->features.vsize = MonInfo1->features.vsize;                          	
                  }
                  else	/* Skip DT */
                  {   /* use 1024x768 as default */
                      MonInfo->features.hsize = 0x20;
                      MonInfo->features.vsize = 0x18;                  	
                  }	
                                  	               	
              } /* Check with VGA1 & VGA2 EDID */
        	    
          } /* GetVGA2EDID */
          else {
              xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Can't Get VGA2 EDID Correctly!! \n");
          }    
         
      }
      
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
    /* Fixed screen pitch incorrect in some specific monitor, ycchen@071707 */
    pAST->VideoModeInfo.ScreenPitch = pScrn->displayWidth * ((pScrn->bitsPerPixel + 1) / 8) ;

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
