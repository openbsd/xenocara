/*
 *	Copyright 2001	Ani Joshi <ajoshi@unixbox.com>
 *
 *	XFree86 4.x driver for S3 chipsets
 *
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation and
 * that the name of Ani Joshi not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Ani Joshi makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as-is" without express or implied warranty.
 *
 * ANI JOSHI DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL ANI JOSHI BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 *
 *	Credits:
 *		Thomas Roell <roell@informatik.tu-muenchen.de>
 *		Mark Vojkovich <markv@valinux.com>
 *		Kevin E. Martin <martin@valinux.com>
 *		   - and others for their work on the 3.x S3 driver
 *
 *		Dominik Behr
 *		   - for various hardware donations
 *
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Pci.h"
#include "xf86PciInfo.h"
#include "xf86fbman.h"
#include "xf86cmap.h"
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#include "xf86RAC.h"
#endif
#include "compiler.h"
#include "mipointer.h"
#include "micmap.h"
#include "mibstore.h"
#include "fb.h"
#include "inputstr.h"
#include "shadowfb.h"
#include "IBM.h"
#include "TI.h"

#include "s3.h"
#include "s3_reg.h"

#define TRIO64_RAMDAC	0x8811


short s3alu[16] =
{
	MIX_0,
	MIX_AND,
	MIX_SRC_AND_NOT_DST,
	MIX_SRC,
	MIX_NOT_SRC_AND_DST,
	MIX_DST,
	MIX_XOR,
	MIX_OR,
	MIX_NOR,
	MIX_XNOR,
	MIX_NOT_DST,
	MIX_SRC_OR_NOT_DST,
	MIX_NOT_SRC,
	MIX_NOT_SRC_OR_DST,
	MIX_NAND,
	MIX_1,
};


/*
 * Prototypes
 */
static const OptionInfoRec * S3AvailableOptions(int chipid, int busid);
static void S3Identify(int flags);
static Bool S3Probe(DriverPtr drv, int flags);
static Bool S3PreInit(ScrnInfoPtr pScrn, int flags);
static Bool S3EnterVT(VT_FUNC_ARGS_DECL);
static void S3LeaveVT(VT_FUNC_ARGS_DECL);
static void S3Save(ScrnInfoPtr pScrn);
static Bool S3ScreenInit(SCREEN_INIT_ARGS_DECL);
static Bool S3MapMem(ScrnInfoPtr pScrn);
static void S3UnmapMem(ScrnInfoPtr pScrn);
static Bool S3ModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
static void S3AdjustFrame(ADJUST_FRAME_ARGS_DECL);
Bool S3CloseScreen(CLOSE_SCREEN_ARGS_DECL);
Bool S3SaveScreen(ScreenPtr pScreen, int mode);
static void S3FreeScreen(FREE_SCREEN_ARGS_DECL);
static void S3GenericLoadPalette(ScrnInfoPtr pScrn, int numColors,
                                 int *indicies, LOCO *colors,
                                 VisualPtr pVisual);
static void S3Restore(ScrnInfoPtr pScrn);
void S3BankZero(ScrnInfoPtr pScrn);
void S3Regdump(ScrnInfoPtr pScrn);
static void S3DisplayPowerManagementSet(ScrnInfoPtr pScrn,
					int PowerManagementMode, int flags);



_X_EXPORT DriverRec S3 =
{
	S3_VERSION,
	DRIVER_NAME,
	S3Identify,
	S3Probe,
	S3AvailableOptions,
	NULL,
	0
};

/* supported chipsets */
static SymTabRec S3Chipsets[] = {
	{ PCI_CHIP_964_0,	"964-0"},
	{ PCI_CHIP_964_1,	"964-1"},
	{ PCI_CHIP_968,		"968" },
	{ PCI_CHIP_TRIO, 	"Trio32/64" },
	{ PCI_CHIP_AURORA64VP,	"Aurora64V+" },
	{ PCI_CHIP_TRIO64UVP, 		"Trio64UV+" },
	{ PCI_CHIP_TRIO64V2_DXGX,	"Trio64V2/DX/GX" },
	{ -1, NULL }
};


static PciChipsets S3PciChipsets[] = {
	{ PCI_CHIP_964_0,	PCI_CHIP_964_0,		RES_SHARED_VGA },
	{ PCI_CHIP_964_1,	PCI_CHIP_964_1,		RES_SHARED_VGA },
	{ PCI_CHIP_968, 	PCI_CHIP_968, 		RES_SHARED_VGA },
	{ PCI_CHIP_TRIO, 	PCI_CHIP_TRIO, 		RES_SHARED_VGA },
	{ PCI_CHIP_AURORA64VP,	PCI_CHIP_AURORA64VP, 	RES_SHARED_VGA },
	{ PCI_CHIP_TRIO64UVP,	PCI_CHIP_TRIO64UVP, 	RES_SHARED_VGA },
	{ PCI_CHIP_TRIO64V2_DXGX,	PCI_CHIP_TRIO64V2_DXGX, 	RES_SHARED_VGA },
	{ -1,			-1,	      		RES_UNDEFINED }
};

typedef enum {
	OPTION_NOACCEL,
	OPTION_HWCURS,
	OPTION_SLOW_DRAM_REFRESH,
	OPTION_SLOW_DRAM,
	OPTION_SLOW_EDODRAM,
	OPTION_SLOW_VRAM,
	OPTION_XVIDEO,
	OPTION_SHADOW_FB,
	OPTION_ROTATE
} S3Opts;

static OptionInfoRec S3Options[] = {
	{ OPTION_NOACCEL, "noaccel", OPTV_BOOLEAN, {0}, FALSE },
	{ OPTION_HWCURS, "hwcursor", OPTV_BOOLEAN, {0}, FALSE },
	{ OPTION_SLOW_DRAM_REFRESH, "slow_dram_refresh", OPTV_BOOLEAN, {0}, FALSE },
	{ OPTION_SLOW_DRAM, "slow_dram", OPTV_BOOLEAN, {0}, FALSE },
	{ OPTION_SLOW_EDODRAM, "slow_edodram", OPTV_BOOLEAN, {0}, FALSE },
	{ OPTION_SLOW_VRAM, "slow_vram", OPTV_BOOLEAN, {0}, FALSE },
	{ OPTION_XVIDEO, "XVideo", OPTV_BOOLEAN, {0}, FALSE },
	{ OPTION_SHADOW_FB, "ShadowFB", OPTV_BOOLEAN, {0}, FALSE },
	{ OPTION_ROTATE, "Rotate", OPTV_ANYSTR, {0}, FALSE },
	{ -1, NULL, OPTV_NONE, {0}, FALSE }
};

RamDacSupportedInfoRec S3IBMRamdacs[] = {
	{ IBM524_RAMDAC },
	{ IBM524A_RAMDAC },
	{ IBM526_RAMDAC },
	{ IBM526DB_RAMDAC },
	{ -1 }
};

static int s3AccelLinePitches[] = { 640, 800, 1024, 1280, 1600 };

#ifdef XFree86LOADER

MODULESETUPPROTO(S3Setup);

static XF86ModuleVersionInfo S3VersRec = {
        "s3",
        MODULEVENDORSTRING,
        MODINFOSTRING1,
        MODINFOSTRING2,
        XORG_VERSION_CURRENT,
        VERSION_MAJOR, VERSION_MINOR, PATCHLEVEL,
        ABI_CLASS_VIDEODRV,
        ABI_VIDEODRV_VERSION,
        MOD_CLASS_VIDEODRV,
        {0, 0, 0, 0}
};


_X_EXPORT XF86ModuleData s3ModuleData = { &S3VersRec, S3Setup, NULL };

pointer S3Setup (pointer module, pointer opts, int *errmaj, int *errmin)
{
	static Bool setupDone = FALSE;

        if (!setupDone) {  
                setupDone = TRUE;
                xf86AddDriver(&S3, module, 0);
                return (pointer) 1;
        } else {
                if (errmaj)  
                        *errmaj = LDR_ONCEONLY;
                return NULL;
        }
}               

#endif /* XFree86LOADER */


static Bool S3GetRec(ScrnInfoPtr pScrn)
{       
        if (pScrn->driverPrivate)
                return TRUE;
                
        pScrn->driverPrivate = xnfcalloc(sizeof(S3Rec), 1);
                
        return TRUE;
}
                        
static void S3FreeRec(ScrnInfoPtr pScrn)
{
        free(pScrn->driverPrivate);
        pScrn->driverPrivate = NULL;
}

static const OptionInfoRec * S3AvailableOptions(int chipid, int busid)
{
        return S3Options;
}
                
static void S3Identify(int flags)
{
        xf86PrintChipsets("S3", "driver (version " DRIVER_VERSION " for S3 chipset",
                          S3Chipsets);  
}

static Bool S3Probe(DriverPtr drv, int flags)
{
	GDevPtr *devSections;
	int i, *usedChips, numDevSections, numUsed;
	Bool foundScreen = FALSE;

	/* sanity check */
	if ((numDevSections = xf86MatchDevice("s3", &devSections)) <= 0)
                return FALSE;

	/* XXX do ISA later...  some day in the distant future... */
	numUsed = xf86MatchPciInstances("s3", PCI_VENDOR_S3,
					S3Chipsets, S3PciChipsets,
					devSections, numDevSections,
					drv, &usedChips);

	free(devSections);

	if (numUsed <= 0)
		return FALSE;

	if (flags & PROBE_DETECT)
		foundScreen = TRUE;
	else for (i=0; i<numUsed; i++) {
		ScrnInfoPtr pScrn = NULL;
                pScrn = xf86ConfigPciEntity(pScrn, 0, usedChips[i], S3PciChipsets,
                                          NULL, NULL, NULL, NULL, NULL);

		pScrn->driverVersion = VERSION_MAJOR;
		pScrn->driverName = DRIVER_NAME;
                pScrn->name = "s3";
                pScrn->Probe = S3Probe;
                pScrn->PreInit = S3PreInit;
                pScrn->ScreenInit = S3ScreenInit;  
                pScrn->SwitchMode = S3SwitchMode;
                pScrn->AdjustFrame = S3AdjustFrame;
                pScrn->EnterVT = S3EnterVT;
                pScrn->LeaveVT = S3LeaveVT;
		pScrn->FreeScreen = S3FreeScreen;

		foundScreen = TRUE;
        }
                
        free(usedChips);
                
        return foundScreen;
}

static Bool S3PreInit(ScrnInfoPtr pScrn, int flags)
{
	EntityInfoPtr pEnt;
	S3Ptr pS3;
	vgaHWPtr hwp;
	ClockRangePtr clockRanges;
	vbeInfoPtr pVBE;
	rgb zeros = {0, 0, 0};
	Gamma gzeros = {0.0, 0.0, 0.0};
	int i, vgaCRIndex, vgaCRReg;
	unsigned char tmp;
	char *s;

        if (flags & PROBE_DETECT)
                return FALSE;
        
        if (!xf86LoadSubModule(pScrn, "vgahw"))
                return FALSE;
        
        if (!vgaHWGetHWRec(pScrn))
                return FALSE;
        
        hwp = VGAHWPTR(pScrn);
	vgaHWSetStdFuncs(hwp);
        vgaHWGetIOBase(hwp);
        
        pScrn->monitor = pScrn->confScreen->monitor;
        
        if (!xf86SetDepthBpp(pScrn, 0, 0, 0, Support24bppFb | Support32bppFb))
	    return FALSE;

        switch (pScrn->depth) {
	case 8:
	case 15:
	case 16:
	case 24:
		/* OK */
		break;
	default:
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Given depth (%d) is not  supported by this driver\n",
			   pScrn->depth);
		return FALSE;
        }

        xf86PrintDepthBpp(pScrn);
        
        if (pScrn->depth > 8) {
                if (!xf86SetWeight(pScrn, zeros, zeros))
                        return FALSE;
        }

        if (!xf86SetDefaultVisual(pScrn, -1))
                return FALSE;
                                           
        pScrn->progClock = TRUE;
                 
        if (!S3GetRec(pScrn))
                return FALSE;

	pS3 = S3PTR(pScrn);

	pS3->s3Bpp = (pScrn->bitsPerPixel >> 3);

        xf86CollectOptions(pScrn, NULL);
        xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, S3Options);
         
	pS3->XVideo = xf86ReturnOptValBool(S3Options, OPTION_XVIDEO, TRUE);
	pS3->NoAccel = xf86ReturnOptValBool(S3Options, OPTION_NOACCEL, FALSE);
	pS3->HWCursor = xf86ReturnOptValBool(S3Options, OPTION_HWCURS, FALSE);
	pS3->SlowDRAMRefresh = xf86ReturnOptValBool(S3Options, OPTION_SLOW_DRAM_REFRESH, FALSE);
	pS3->SlowDRAM = xf86ReturnOptValBool(S3Options, OPTION_SLOW_DRAM, FALSE);
	pS3->SlowEDODRAM = xf86ReturnOptValBool(S3Options, OPTION_SLOW_EDODRAM, FALSE);
	pS3->SlowVRAM = xf86ReturnOptValBool(S3Options, OPTION_SLOW_VRAM, FALSE);


    
	if (xf86GetOptValBool(S3Options, OPTION_SHADOW_FB, &pS3->shadowFB))
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ShadowFB %s.\n",
			   pS3->shadowFB ? "enabled" : "disabled");
    
	pS3->rotate = 0;
	if ((s = xf86GetOptValString(S3Options, OPTION_ROTATE))) {
		if(!xf86NameCmp(s, "CW")) {
			/* accel is disabled below for shadowFB */
			pS3->shadowFB = TRUE;
			pS3->rotate = 1;
			xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, 
				   "Rotating screen clockwise - acceleration disabled\n");
		} else if(!xf86NameCmp(s, "CCW")) {
			pS3->shadowFB = TRUE;
			pS3->rotate = -1;
			xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,  "Rotating screen"
				   "counter clockwise - acceleration disabled\n");
		} else {
			xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "\"%s\" is not a valid"
				   "value for Option \"Rotate\"\n", s);
			xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
				   "Valid options are \"CW\" or \"CCW\"\n");
		}
	}
	
	if(pS3->shadowFB && !pS3->NoAccel) {
		pS3->NoAccel = TRUE;
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "HW acceleration not supported with \"shadowFB\".\n");
	}
    
	
	if (pS3->rotate && pS3->HWCursor) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "HW cursor not supported with \"rotate\".\n");
		pS3->HWCursor = FALSE;
	}
    
        
        if (pScrn->numEntities > 1) {      
                S3FreeRec(pScrn);
                return FALSE;
        }
                
        pEnt = xf86GetEntityInfo(pScrn->entityList[0]);
#ifndef XSERVER_LIBPCIACCESS
        if (pEnt->resources) {
                free(pEnt);
                S3FreeRec(pScrn);
                return FALSE;
        }
#endif

	if (xf86LoadSubModule(pScrn, "int10")) {
		pS3->pInt10 = xf86InitInt10(pEnt->index);
	}

	if (xf86LoadSubModule(pScrn, "vbe")) {
		pVBE = VBEInit(pS3->pInt10, pEnt->index);
		vbeFree(pVBE);
	}
	
	xf86LoadSubModule(pScrn, "fb");

	if (!xf86LoadSubModule(pScrn, "xaa")) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "Falling back to shadowfb\n");
		pS3->NoAccel = TRUE;
		pS3->shadowFB = TRUE;
	}

	if (pS3->shadowFB) {
		if (!xf86LoadSubModule(pScrn, "shadowfb")) {
			S3FreeRec(pScrn);
			return FALSE;
		}
	}

	if (!xf86SetGamma(pScrn, gzeros))
		return FALSE;

        pS3->PciInfo = xf86GetPciInfoForEntity(pEnt->index);
#ifndef XSERVER_LIBPCIACCESS
	/* don't disable PIO funcs */
        xf86SetOperatingState(resVgaMemShared, pEnt->index, ResDisableOpr);
#endif
        if (pEnt->device->chipset && *pEnt->device->chipset) {
                pScrn->chipset = pEnt->device->chipset;
                pS3->Chipset = xf86StringToToken(S3Chipsets, pScrn->chipset);
        } else if (pEnt->device->chipID >= 0) {            
                pS3->Chipset = pEnt->device->chipID;
                pScrn->chipset = (char *)xf86TokenToString(S3Chipsets,
                                                           pS3->Chipset);
                xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, 
			   "ChipID override: 0x%04X\n", pS3->Chipset);
        } else {
  	        pS3->Chipset = PCI_DEV_DEVICE_ID(pS3->PciInfo);
                pScrn->chipset = (char *)xf86TokenToString(S3Chipsets,
                                                           pS3->Chipset);
        }                                                  
        if (pEnt->device->chipRev >= 0) {
                pS3->ChipRev = pEnt->device->chipRev;
                xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipRev override: %d\n",
                           pS3->ChipRev);
        } else
	        pS3->ChipRev = PCI_DEV_REVISION(pS3->PciInfo);
        
        free(pEnt);
        
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Chipset: \"%s\"\n", 
		   pScrn->chipset);
        
#ifndef XSERVER_LIBPCIACCESS
        pS3->PciTag = pciTag(pS3->PciInfo->bus, pS3->PciInfo->device,
                             pS3->PciInfo->func);        
#endif

	switch (pS3->Chipset) {
	case PCI_CHIP_964_0:
	case PCI_CHIP_964_1:
	case PCI_CHIP_TRIO:
		if (pS3->ChipRev >= 0x40) { /* S3 Trio64V+ has the New MMIO */
			pS3->S3NewMMIO = TRUE;
			break;
		}
	case PCI_CHIP_AURORA64VP:		/* ??? */
		pS3->S3NewMMIO = FALSE;
		break;
	case PCI_CHIP_TRIO64V2_DXGX:
	case PCI_CHIP_TRIO64UVP:
	case PCI_CHIP_968:
		pS3->S3NewMMIO = TRUE;
		break;
	}

	/* TODO: Streams Processor and Xv for Old MMIO */

	if (((pS3->Chipset == PCI_CHIP_AURORA64VP) ||
	     (pS3->Chipset == PCI_CHIP_TRIO64UVP) ||
	     (pS3->Chipset == PCI_CHIP_TRIO64V2_DXGX) ||
	     ((pS3->Chipset == PCI_CHIP_TRIO) && (pS3->ChipRev >= 0x40)))
	    && (pS3->S3NewMMIO))
        	pS3->hasStreams = TRUE;
        else
        	pS3->hasStreams = FALSE;
 
	pS3->FBAddress = PCI_REGION_BASE(pS3->PciInfo, 0, REGION_MEM);
	pScrn->memPhysBase = pS3->FBAddress;
	pScrn->fbOffset = 0;
	
	if (pS3->S3NewMMIO)
		pS3->IOAddress = pS3->FBAddress + S3_NEWMMIO_REGBASE;

	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Framebuffer @ 0x%lx\n",
		   pS3->FBAddress);
	if (pS3->S3NewMMIO)
		xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "MMIO @ 0x%lx\n",
			   pS3->IOAddress);

	pS3->PCIRetry = FALSE;		/* not supported yet */

	pS3->vgaCRIndex = vgaCRIndex = hwp->IOBase + 4;
	pS3->vgaCRReg = vgaCRReg = hwp->IOBase + 5;

	/* unlock sys regs */
	outb(vgaCRIndex, 0x38);
	outb(vgaCRReg, 0x48);
	outb(vgaCRIndex, 0x39);
	outb(vgaCRReg, 0xa5);

	outb(vgaCRIndex, 0x40);
	tmp = inb(vgaCRReg) | 0x01;
	outb(vgaCRReg, tmp);
	outb(vgaCRIndex, 0x35);
	tmp = inb(vgaCRReg) & ~0x30;
	outb(vgaCRReg, tmp);

	outb(0x3c4, 0x08);
	outb(0x3c5, 0x06);
	outb(vgaCRIndex, 0x33);
	tmp = (inb(vgaCRReg) & ~(0x2 | 0x10 | 0x40)) | 0x20;
	outb(vgaCRReg, tmp);

	/* unprotect CRTC[0-7] */
	outb(vgaCRIndex, 0x11);
	tmp = inb(vgaCRReg) & 0x7f;
	outb(vgaCRReg, tmp);

	/* wake up */
	outb(0x46e8, 0x10);
	outb(0x102, 0x01);
	outb(0x46e8, 0x08);

	if (pS3->Chipset == PCI_CHIP_TRIO64V2_DXGX)
	{
          /* disable DAC power saving to avoid bright left edge */
	  outb (0x3d4, 0x86);
	  outb (0x3d5, 0x80);
	  /* disable the stream display fetch length control */
	  outb (0x3d4, 0x90);
	  outb (0x3d5, 0x00);
	}

	if (!pScrn->videoRam) {
		/* probe videoram */
		outb(vgaCRIndex, 0x36);
		tmp = inb(vgaCRReg);

		switch ((tmp & 0xe0) >> 5) {
		case 0:
			pScrn->videoRam = 4096;
			break;
		case 2:
			pScrn->videoRam = 3072;
			break;
		case 3:
			pScrn->videoRam = 8192;
			break;
		case 4:
			pScrn->videoRam = 2048;
			break;
		case 5:
			pScrn->videoRam = 6144;
			break;
		case 6:
			pScrn->videoRam = 1024;
			break;
		}
		xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			   "videoRam = %d Kb\n", pScrn->videoRam);
	} else {
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			   "videoRam = %d Kb\n", pScrn->videoRam);
	}

	if (!xf86LoadSubModule(pScrn, "ramdac"))
		return FALSE;

	pScrn->rgbBits = 8;	/* set default */

	/* probe for dac */
	if (S3TiDACProbe(pScrn)) {
		pS3->DacPreInit = S3TiDAC_PreInit;
		pS3->DacInit = S3TiDAC_Init;
		pS3->DacSave = S3TiDAC_Save;
		pS3->DacRestore = S3TiDAC_Restore;
#if 0
		/* FIXME, cursor is drawn in wrong position */
		pS3->CursorInit = S3Ti_CursorInit;
#endif
		pS3->MaxClock = 135000;
		pScrn->rgbBits = 8;
		if (pScrn->bitsPerPixel > 8)
			pS3->LoadPalette = S3TiLoadPalette;
		else
			pS3->LoadPalette = S3GenericLoadPalette;
	}
	if (S3ProbeIBMramdac(pScrn)) {
		pS3->DacPreInit = S3IBMRGB_PreInit;
		pS3->DacInit = S3IBMRGB_Init;
		pS3->DacSave = S3IBMRGB_Save;
		pS3->DacRestore = S3IBMRGB_Restore;
		pS3->CursorInit = S3IBMRGB_CursorInit;
		pS3->RamDac->SetBpp = IBMramdac526SetBppWeak();
		pS3->MaxClock = 170000;
		pScrn->rgbBits = 8;
		pS3->LoadPalette = S3GenericLoadPalette;
	}
	if (S3Trio64DACProbe(pScrn)) {
		pS3->DacPreInit = S3Trio64DAC_PreInit;
		pS3->DacInit = S3Trio64DAC_Init;
		pS3->DacSave = S3Trio64DAC_Save;
		pS3->DacRestore = S3Trio64DAC_Restore;
#if 0
		pS3->CursorInit = S3_CursorInit;	/* FIXME broken */
#endif
		switch(pScrn->bitsPerPixel) {
		case 8:
			if (pS3->Chipset == PCI_CHIP_TRIO64V2_DXGX)
				pS3->MaxClock = 170000;
			else
				pS3->MaxClock = 135000;

			pScrn->rgbBits = 6;
			break;
		case 16:
			pS3->MaxClock = 80000;
			pScrn->rgbBits = 6;
			break;
		case 24:
		case 32:
			if (pS3->Chipset == PCI_CHIP_TRIO64V2_DXGX)
				pS3->MaxClock = 56700;
			else
				pS3->MaxClock = 50000;

			pScrn->rgbBits = 8;
			break;
		}

		pS3->LoadPalette = S3GenericLoadPalette;
	}

	if (pS3->RamDac == NULL) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Ramdac probe failed\n");
		return FALSE;
	}

	if (!pS3->HWCursor)
		pS3->CursorInit = NULL;

	pS3->RefClock = S3GetRefClock(pScrn);

	if (pS3->DacPreInit)
		pS3->DacPreInit(pScrn);

	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "RefClock: %d\n",
		   pS3->RefClock);
	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Max pixel clock at this depth is %d Mhz\n",
		   pS3->MaxClock / 1000);

	clockRanges = xnfcalloc(sizeof(ClockRange), 1);
	clockRanges->next = NULL;
	clockRanges->minClock = 15600;
	clockRanges->maxClock = pS3->MaxClock;
	clockRanges->clockIndex = -1;
	clockRanges->interlaceAllowed = TRUE;	/* not yet */
	clockRanges->doubleScanAllowed = TRUE;	/* not yet */
	
        i = xf86ValidateModes(pScrn, pScrn->monitor->Modes,
                              pScrn->display->modes, clockRanges,
                              pS3->NoAccel ? NULL : s3AccelLinePitches,
			      256, 2048,
			      pScrn->bitsPerPixel, 128, 2048,
			      pScrn->display->virtualX,
			      pScrn->display->virtualY, pScrn->videoRam * 1024,
                              LOOKUP_BEST_REFRESH);

        if (i == -1) {
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "no valid modes left\n");
                S3FreeRec(pScrn);
                return FALSE; 
        }
        
        xf86PruneDriverModes(pScrn);

        if (i == 0 || pScrn->modes == NULL) {
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "no valid modes found\n");
                S3FreeRec(pScrn);
                return FALSE;
        }

        xf86SetCrtcForModes(pScrn, INTERLACE_HALVE_V);
        pScrn->currentMode = pScrn->modes;
        xf86PrintModes(pScrn);
        xf86SetDpi(pScrn, 0, 0);
 
	return TRUE;
}


static Bool S3ScreenInit(SCREEN_INIT_ARGS_DECL)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	S3Ptr pS3 = S3PTR(pScrn);
	BoxRec ScreenArea;
	int width, height, displayWidth;
	
	if (pS3->rotate) {
		height = pScrn->virtualX;
		width = pScrn->virtualY;
	} else {
		width = pScrn->virtualX;
		height = pScrn->virtualY;
	}


	pScrn->fbOffset = 0;

	if (!S3MapMem(pScrn)) {
		S3FreeRec(pScrn);
		return FALSE;
	}

	S3Save(pScrn);

	vgaHWBlankScreen(pScrn, TRUE);

	if (!S3ModeInit(pScrn, pScrn->currentMode))
		return FALSE;
#if 0
	S3Regdump(pScrn);
#endif
	pScrn->vtSema = TRUE;
	
	S3SaveScreen(pScreen, SCREEN_SAVER_ON);

        miClearVisualTypes();
        if (pScrn->bitsPerPixel > 8) {
                if (!miSetVisualTypes(pScrn->depth, TrueColorMask,
                                      pScrn->rgbBits, pScrn->defaultVisual))
                        return FALSE;
        } else {
                if (!miSetVisualTypes(pScrn->depth, miGetDefaultVisualMask(pScrn->depth),
                                      pScrn->rgbBits, pScrn->defaultVisual))
                        return FALSE; 
        }       
        
        miSetPixmapDepths ();

        /* no screen rotation assumed */
        if(pS3->shadowFB) {
        	pS3->ShadowPitch = BitmapBytePad(pScrn->bitsPerPixel * width);
        	pS3->ShadowPtr = malloc(pS3->ShadowPitch * height);
		displayWidth = pS3->ShadowPitch / (pScrn->bitsPerPixel >> 3);
        } else {
        	pS3->ShadowPtr = NULL;
		displayWidth = pScrn->displayWidth;
        }
        
        if (!fbScreenInit(pScreen, (pS3->shadowFB ? pS3->ShadowPtr : pS3->FBBase), 
			  pScrn->virtualX,
                          pScrn->virtualY, pScrn->xDpi, pScrn->yDpi,
                          pScrn->displayWidth, pScrn->bitsPerPixel))
                return FALSE;
        
        xf86SetBlackWhitePixels(pScreen);
                        
        if (pScrn->bitsPerPixel > 8) {
                VisualPtr pVis;
                         
                pVis = pScreen->visuals + pScreen->numVisuals;
                while (--pVis >= pScreen->visuals) {
                        if ((pVis->class | DynamicClass) == DirectColor) {
                                pVis->offsetRed = pScrn->offset.red;
                                pVis->offsetGreen = pScrn->offset.green;
                                pVis->offsetBlue = pScrn->offset.blue;
                                pVis->redMask = pScrn->mask.red;
                                pVis->greenMask = pScrn->mask.green;
                                pVis->blueMask = pScrn->mask.blue;
                        }
                } 
        }
	fbPictureInit (pScreen, 0, 0);
	S3DGAInit(pScreen);

        miInitializeBackingStore(pScreen);
        xf86SetBackingStore(pScreen);

	/* framebuffer manager setup */
	ScreenArea.x1 = 0;
	ScreenArea.y1 = 0;
	ScreenArea.x2 = pScrn->displayWidth;
	ScreenArea.y2 = (pScrn->videoRam * 1024) / pS3->s3BppDisplayWidth;

	if (!xf86InitFBManager(pScreen, &ScreenArea)) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Memory manager initialization to (%d,%d) (%d,%d) failed\n",
			   ScreenArea.x1, ScreenArea.y1,
			   ScreenArea.x2, ScreenArea.y2);
		return FALSE;
	} else
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "Memory manager initialized to (%d,%d) (%d,%d)\n",
			   ScreenArea.x1, ScreenArea.y1, 
			   ScreenArea.x2, ScreenArea.y2);
	
	/* 2D acceleration setup */

	if (pS3->NoAccel)
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, 
			   "Acceleration disabled (by option)\n");

	/* It seems that acceleration isn't supported for 24-bit packed
	   colour. So disable it. Using shadowFB is recommended in this mode. */
	if (!pS3->NoAccel && (pScrn->bitsPerPixel == 24)) {
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "HW acceleration isn't supported for 24 bpp. Disabled.\n");
		pS3->NoAccel = TRUE;
	}


	if (!pS3->NoAccel) {
		if (pS3->S3NewMMIO)
			if (S3AccelInitNewMMIO(pScreen)) {
				xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
					   "Acceleration enabled\n");
				xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
					   "Using NewMMIO\n");
			} else {
				xf86DrvMsg(pScrn->scrnIndex, X_ERROR, 
					   "Acceleration initialization failed\n");
				xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
					   "Acceleration disabled\n");
			}
		else {
			if (S3AccelInitPIO(pScreen)) {
				xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
					   "Acceleration enabled\n");
				xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
					   "Using PIO\n");
			} else {
				xf86DrvMsg(pScrn->scrnIndex, X_ERROR, 
					   "Acceleration initialization failed\n");
				xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
					   "Acceleration disabled\n");
			}
		}
	}
	
        miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

	/* HW cursor setup */

	if (pS3->CursorInit) {
		if (pS3->CursorInit(pScreen))
			xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using HW cursor\n");
		else {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "HW cursor initialization failed\n");
			xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using SW cursor\n");
		}
	} else
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using SW cursor\n");


	/* Shadow framebuffer setup */

	if (pS3->shadowFB) {
		RefreshAreaFuncPtr refreshArea = S3RefreshArea;
		
		if (pS3->rotate) {
			if (!pS3->PointerMoved) {
				pS3->PointerMoved = pScrn->PointerMoved;
				pScrn->PointerMoved = S3PointerMoved;
			}
			
			switch (pScrn->bitsPerPixel) {
			case 8: refreshArea = S3RefreshArea8; break;
			case 16: refreshArea = S3RefreshArea16; break;
			case 24: refreshArea = S3RefreshArea24; break;
			case 32: refreshArea = S3RefreshArea32; break;
			}
		}
		
		ShadowFBInit(pScreen, refreshArea);
	}



        if (!miCreateDefColormap(pScreen))
                return FALSE;

        if (!xf86HandleColormaps(pScreen, 256, pScrn->rgbBits,
				 pS3->LoadPalette, NULL,
                                 CMAP_RELOAD_ON_MODE_SWITCH))
                return FALSE;

	vgaHWBlankScreen(pScrn, FALSE);

        pScreen->SaveScreen = S3SaveScreen;
        pS3->CloseScreen = pScreen->CloseScreen;
        pScreen->CloseScreen = S3CloseScreen;

	xf86DPMSInit(pScreen, S3DisplayPowerManagementSet, 0);

#ifndef XSERVER_LIBPCIACCESS
	/* XXX Check if I/O and Mem flags need to be the same. */
	pScrn->racIoFlags = pScrn->racMemFlags = RAC_COLORMAP
	    | RAC_FB | RAC_VIEWPORT | RAC_CURSOR;
#endif

	if (pS3->SlowEDODRAM)
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
			   "SlowEDODRAM: Setting 2-cycle EDO\n");

	if (pS3->SlowVRAM)
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
			   "SlowVRAM: -RAS low time is 4.5 MCLKs\n");

	if (pS3->SlowDRAM)
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
			   "SlowDRAM: -RAS precharge time is 3.5 MCLKs\n");

	if (pS3->SlowDRAMRefresh)
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
			   "SlowDRAMRefresh: three refresh cycles per scanline\n");

	/* XVideo setup */

	if (pS3->XVideo) {
		if (!pS3->hasStreams) {
			xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Overlay video isn't supported by video hardware. Disabled.\n");
			pS3->XVideo = FALSE;
		} else if (pScrn->bitsPerPixel < 16) {
			xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Overlay video isn't supported for %d bpp. Disabled.\n", pScrn->bitsPerPixel);
			pS3->XVideo = FALSE;
		}
	} else
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, 
			   "Overlay video disabled by option\n");
	
	/* At present time we support XV only for chips with New MMIO */
	if ((pS3->XVideo) && (pS3->S3NewMMIO))
		S3InitVideo(pScreen);
	
	switch (pScrn->bitsPerPixel) {
	case 8:
		pS3->Streams_FIFO = FIFO_PS16_SS8;
		break;
	case 15:
	case 16:
		pS3->Streams_FIFO = FIFO_PS12_SS12;
		break;
	case 24:
	case 32: 
		pS3->Streams_FIFO = FIFO_PS8_SS16;
		break;
	}
		
        return TRUE;
}                 




static void S3Save(ScrnInfoPtr pScrn)
{
	S3Ptr pS3 = S3PTR(pScrn);
	S3RegPtr save = &pS3->SavedRegs;
	vgaHWPtr hwp = VGAHWPTR(pScrn);
        vgaRegPtr pVga = &hwp->SavedReg;
	int vgaCRIndex = pS3->vgaCRIndex, vgaCRReg = pS3->vgaCRReg;
	int i;
	unsigned char cr5c = 0;

	S3BankZero(pScrn);

	save->clock = inb(0x3cc);

	vgaHWSave(pScrn, pVga, VGA_SR_ALL);

	if (pS3->RamDac->RamDacType == TI3025_RAMDAC) {
		outb(vgaCRIndex, 0x5c);
		cr5c = inb(vgaCRReg);
	}

	pS3->DacSave(pScrn);

	for(i=0; i<5; i++) {
		outb(vgaCRIndex, 0x30 + i);
		save->s3save[i] = inb(vgaCRReg);
		outb(vgaCRIndex, 0x38 + i);
		save->s3save[5 + i] = inb(vgaCRReg);
	}

	for (i=0; i<16; i++) {
		outb(vgaCRIndex, 0x40 + i);
		save->s3syssave[i] = inb(vgaCRReg);
	}

	outb(vgaCRIndex, 0x45);
	inb(vgaCRReg);
	outb(vgaCRIndex, 0x4a);
	for(i=0; i<4; i++) {
		save->color_stack[i] = inb(vgaCRReg);
		outb(vgaCRReg, save->color_stack[i]);
	}

	outb(vgaCRIndex, 0x45);
	inb(vgaCRReg);
	outb(vgaCRIndex, 0x4b);
	for(i=4; i<8; i++) {
		save->color_stack[i] = inb(vgaCRReg);
		outb(vgaCRReg, save->color_stack[i]);
	}

	for(i=0; i<16; i++) {
		for (i=0; i<16; i++) {
			if (!((1 << i) & 0x673b))
				continue;
			outb(vgaCRIndex, 0x50 + i);
			save->s3syssave[i + 16] = inb(vgaCRReg);
		}
	}

	if (pS3->RamDac->RamDacType == TI3025_RAMDAC)
		save->s3syssave[0x0c + 16] = cr5c;	

	for(i=32; i<46; i++) {
		outb(vgaCRIndex, 0x40 + i);
		save->s3syssave[i] = inb(vgaCRReg);
	}
}


Bool S3SaveScreen(ScreenPtr pScreen, int mode)
{
	return vgaHWSaveScreen(pScreen, mode);
}


static void S3FreeScreen(FREE_SCREEN_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);
                           
        vgaHWFreeHWRec(pScrn);
        
        S3FreeRec(pScrn);
}


Bool S3CloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
        ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
        S3Ptr pS3 = S3PTR(pScrn);
        vgaHWPtr hwp = VGAHWPTR(pScrn);

        if (pScrn->vtSema) {
                vgaHWUnlock(hwp);
		S3Restore(pScrn);
                vgaHWLock(hwp);
                S3UnmapMem(pScrn);
        }

	free(pS3->DGAModes);
	pS3->DGAModes = NULL;

        pScrn->vtSema = FALSE;
        pScreen->CloseScreen = pS3->CloseScreen;
                
        return (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);
}


Bool S3SwitchMode(SWITCH_MODE_ARGS_DECL)
{    
	SCRN_INFO_PTR(arg);
	return S3ModeInit(pScrn, mode);
}


static void S3GenericLoadPalette(ScrnInfoPtr pScrn, int numColors,
                                 int *indicies, LOCO *colors,
                                 VisualPtr pVisual)
{               
        int i, index;
	
        for (i=0; i<numColors; i++) {
                index = indicies[i];
                outb(0x3c8, index);
                outb(0x3c9, colors[index].red);
                outb(0x3c9, colors[index].green);
                outb(0x3c9, colors[index].blue);
        }
}


static Bool S3MapMem(ScrnInfoPtr pScrn)
{
	S3Ptr pS3 = S3PTR(pScrn);

	if (pS3->S3NewMMIO) {


#ifndef XSERVER_LIBPCIACCESS
		pS3->MMIOBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO,
					      pS3->PciTag, pS3->IOAddress,
					      S3_NEWMMIO_REGSIZE);
#else
		{
			void** result = (void**)&pS3->MMIOBase;
			int err = pci_device_map_range(pS3->PciInfo,
						       pS3->IOAddress,
						       S3_NEWMMIO_REGSIZE,
						       PCI_DEV_MAP_FLAG_WRITABLE,
						       result);
			
			if (err) 
				return FALSE;
		}
#endif
		if (!pS3->MMIOBase) {
			xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
				   "Could not map MMIO\n");
			return FALSE;
		}
	}

#ifndef XSERVER_LIBPCIACCESS
	pS3->FBBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_FRAMEBUFFER,
				    pS3->PciTag, pS3->FBAddress,
				    pScrn->videoRam * 1024);

#else
	{
		void** result = (void**)&pS3->FBBase;
		int err = pci_device_map_range(pS3->PciInfo,
					       pS3->FBAddress,
					       pScrn->videoRam * 1024,
					       PCI_DEV_MAP_FLAG_WRITABLE |
					       PCI_DEV_MAP_FLAG_WRITE_COMBINE,
					       result);
		
		if (err) 
			return FALSE;
	}
#endif
	if (!pS3->FBBase) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Could not map framebuffer\n");
		return FALSE;
	}

	pS3->FBCursorOffset = pScrn->videoRam - 1;

	return TRUE;
}


static void S3UnmapMem(ScrnInfoPtr pScrn)
{
	S3Ptr pS3 = S3PTR(pScrn);

	if (pS3->S3NewMMIO) {
#ifndef XSERVER_LIBPCIACCESS
		xf86UnMapVidMem(pScrn->scrnIndex, (pointer)pS3->MMIOBase,
				S3_NEWMMIO_REGSIZE);
#else
		pci_device_unmap_range(pS3->PciInfo, pS3->MMIOBase, S3_NEWMMIO_REGSIZE);
#endif
	}
	
#ifndef XSERVER_LIBPCIACCESS
	xf86UnMapVidMem(pScrn->scrnIndex, (pointer)pS3->FBBase,
			pScrn->videoRam * 1024);
#else
	pci_device_unmap_range(pS3->PciInfo, pS3->FBBase, pScrn->videoRam * 1024);
#endif

	return;
}


static int S3GetPixMuxShift(ScrnInfoPtr pScrn)
{
	S3Ptr pS3 = S3PTR(pScrn);
	int shift = 0;

	if (pS3->Chipset == PCI_CHIP_968)
		shift = 1;	/* XXX IBMRGB */
	else if (pS3->Chipset == PCI_CHIP_TRIO || 
	         pS3->Chipset == PCI_CHIP_TRIO64UVP || 
	         pS3->Chipset == PCI_CHIP_TRIO64V2_DXGX) {
		if (pS3->s3Bpp == 2)
			shift = -1; 
		else 
			shift = 0;
	}

	return shift;
}

static Bool S3ModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
	S3Ptr pS3 = S3PTR(pScrn);
	S3RegPtr new = &pS3->ModeRegs;
        vgaHWPtr hwp = VGAHWPTR(pScrn);
        vgaRegPtr pVga = &hwp->ModeReg;
        int vgaCRIndex = pS3->vgaCRIndex, vgaCRReg = pS3->vgaCRReg;
	int vgaIOBase = hwp->IOBase;
	int r, n, m;
	unsigned char tmp;

	pS3->pixMuxShift = S3GetPixMuxShift(pScrn);

	pS3->s3BppDisplayWidth = pScrn->displayWidth * pS3->s3Bpp;
	pS3->hwCursor = (mode->Flags & V_DBLSCAN) ? FALSE : TRUE;
	pS3->HDisplay = mode->HDisplay;

	pS3->s3ScissB = ((pScrn->videoRam * 1024) / pS3->s3BppDisplayWidth) - 1;
	pS3->s3ScissR = pScrn->displayWidth - 1;

	/* 
	   Set correct blanking for S3 Trio64V2. It's also needed
	   to clear cr33_5.
	*/
	if (pS3->Chipset == PCI_CHIP_TRIO64V2_DXGX)
		mode->CrtcHBlankStart = mode->CrtcHDisplay + 8;

	if ((mode->HTotal == mode->CrtcHTotal) && (pS3->pixMuxShift != 0)) {
		if (pS3->pixMuxShift > 0) {
			mode->CrtcHTotal >>= pS3->pixMuxShift;
			mode->CrtcHDisplay >>= pS3->pixMuxShift;
			mode->CrtcHBlankStart >>= pS3->pixMuxShift;
			mode->CrtcHBlankEnd >>= pS3->pixMuxShift;
			mode->CrtcHSyncStart >>= pS3->pixMuxShift;
			mode->CrtcHSyncEnd >>= pS3->pixMuxShift;
			mode->CrtcHSkew >>= pS3->pixMuxShift;
		} else if (pS3->pixMuxShift < 0) {
			mode->CrtcHTotal <<= -pS3->pixMuxShift;
			mode->CrtcHDisplay <<= -pS3->pixMuxShift;
			mode->CrtcHBlankStart <<= -pS3->pixMuxShift;
			mode->CrtcHBlankEnd <<= -pS3->pixMuxShift;
			mode->CrtcHSyncStart <<= -pS3->pixMuxShift;
			mode->CrtcHSyncEnd <<= -pS3->pixMuxShift;
			mode->CrtcHSkew <<= -pS3->pixMuxShift;
		}
	}

        if (!vgaHWInit(pScrn, mode))
                return FALSE;



	pVga->MiscOutReg |= 0x0c;
	pVga->Sequencer[0] = 0x03;
	pVga->CRTC[19] = pS3->s3BppDisplayWidth >> 3;
	pVga->CRTC[23] = 0xe3;
	pVga->Attribute[0x11] = 0xff;

	if (vgaIOBase == 0x3b0)
		pVga->MiscOutReg &= 0xfe;
	else
		pVga->MiscOutReg |= 0x01;

	/* ok i give up also, i'm writing in here */

	vgaHWProtect(pScrn, TRUE);


	if (pS3->RamDac->RamDacType == TI3025_RAMDAC) {
		outb(vgaCRIndex, 0x5c);
		tmp = inb(vgaCRReg);
		outb(vgaCRReg, tmp & 0xdf);

		S3OutTiIndReg(pScrn, TIDAC_ind_curs_ctrl, 0x7f, 0x00);
	}

	pS3->DacInit(pScrn, mode);

	outb(0x3c2, pVga->MiscOutReg);

	for(r=1; r<5; r++) {
		outw(0x3c4, (pVga->Sequencer[r] << 8) | r);
	}

	/* We need to set this first - S3 *is* broken */
	outw(vgaCRIndex, (pVga->CRTC[17] << 8) | 17); 
	for(r=0; r<25; r++)
		outw(vgaCRIndex, (pVga->CRTC[r] << 8) | r);

	for(r=0; r<9; r++)
		outw(0x3ce, (pVga->Graphics[r] << 8) | r);

	inb(vgaIOBase + 0x0a);

	for(r=0; r<16; r++) {
		outb(0x3c0, r);
		outb(0x3c0, pVga->Attribute[r]);
	}
	for(r=16; r<21; r++) {
		outb(0x3c0, r | 0x20);
		outb(0x3c0, pVga->Attribute[r]);
	}


	new->cr31 = 0x8d;
	outb(vgaCRIndex, 0x31);
	outb(vgaCRReg, new->cr31);

	new->cr32 = 0x00;
	outb(vgaCRIndex, 0x32);
	outb(vgaCRReg, new->cr32);

	outb(vgaCRIndex, 0x33);
	new->cr33 = inb(vgaCRReg) | 0x20;
	if ((pS3->Chipset == PCI_CHIP_964_0) ||
	    (pS3->Chipset == PCI_CHIP_964_1))
		new->cr33 = 0x20;
	else if (pS3->Chipset == PCI_CHIP_TRIO64V2_DXGX)
		new->cr33 &= ~0x20;
	outb(vgaCRReg, new->cr33);
	
	new->cr34 = 0x10;
	outb(vgaCRIndex, 0x34);
	outb(vgaCRReg, new->cr34);

	if (pS3->Chipset != PCI_CHIP_AURORA64VP) {
		new->cr3b = (pVga->CRTC[0] + pVga->CRTC[4] + 1) / 2;
		outb(vgaCRIndex, 0x3b);
		outb(vgaCRReg, new->cr3b);
	}

	new->cr3c = pVga->CRTC[0] / 2;
	outb(vgaCRIndex, 0x3c);
	outb(vgaCRReg, new->cr3c);

	outb(vgaCRIndex, 0x40);
	tmp = inb(vgaCRReg);
	new->cr40 = (tmp & 0xf2) | 0x05;
	outb(vgaCRReg, new->cr40);

	outb(vgaCRIndex, 0x43);
	switch (pScrn->bitsPerPixel) {
	case 24:
	case 32:
		new->cr43 = inb(vgaCRReg);
		break;
	case 15:
	case 16:
		if ((pS3->RamDac->RamDacType == IBM524_RAMDAC) ||
		    (pS3->RamDac->RamDacType == IBM524A_RAMDAC) ||
		    (pS3->RamDac->RamDacType == TI3025_RAMDAC))
			new->cr43 = 0x10;
		else if (pS3->RamDac->RamDacType == TRIO64_RAMDAC)
			new->cr43 = 0x09;
		break;
	case 8:
	default:
		new->cr43 = 0x00;
		break;
	}
	outb(vgaCRReg, new->cr43);

	new->cr44 = 0x00;
	outb(vgaCRIndex, 0x44);
	outb(vgaCRReg, new->cr44);

	outb(vgaCRIndex, 0x45);
	new->cr45 = inb(vgaCRReg) & 0xf2;
	outb(vgaCRReg, new->cr45);

	outb(vgaCRIndex, 0x50);
	tmp = inb(vgaCRReg) & ~0xf1;
	switch (pScrn->bitsPerPixel) {
	case 8:
		break;
	case 16:
		tmp |= 0x10;
		break;
	case 24:
		tmp |= 0x20; /* there is no such value in spec s3.txt */
		break;
	case 32:
		tmp |= 0x30;
		break;
	}

	switch (pScrn->displayWidth) {
	case 640:
		tmp |= 0x40;
		break;
	case 800:
		tmp |= 0x80;
		break;
	case 1152:
		tmp |= 0x01;
		break;
	case 1280:
		tmp |= 0xc0;
		break;
	case 1600:
		tmp |= 0x81;
		break;
	}
	new->cr50 = tmp;
	outb(vgaCRReg, new->cr50);


	outb(vgaCRIndex, 0x51);
	new->cr51 = (inb(vgaCRReg) & 0xc0) |
		    ((pS3->s3BppDisplayWidth >> 7) & 0x30);
	outb(vgaCRReg, new->cr51);

	outb(vgaCRIndex, 0x53);
	new->cr53 = inb(vgaCRReg);
	if (pS3->S3NewMMIO)
		new->cr53 |= 0x18;
	else
		new->cr53 &= ~0x18;
	outb(vgaCRReg, new->cr53);

	n = 255;
	outb(vgaCRIndex, 0x54);
	{
		int clock2, mclk;

		clock2 = mode->Clock * pS3->s3Bpp;
		if (pScrn->videoRam < 2048)
			clock2 *= 2;
		mclk = pS3->mclk;
		m = (int)((mclk/1000.0*.72+16.867)*89.736/(clock2/1000.0+39)-21.1543);
		if (pScrn->videoRam < 2048)
			m /= 2;
		if (m >31)
			m = 31;
		else if (m < 0) {
			m = 0;
			n = 16;
		}
	}
	new->cr54 = m << 3;
	outb(vgaCRReg, new->cr54);

	if (n < 0)
		n = 0;
	else if (n > 255)
		n = 255;
	outb(vgaCRIndex, 0x60);
	new->cr60 = n;
	outb(vgaCRReg, new->cr60);

	if (pS3->hasStreams) {
		new->cr60 = 255;		
		outb(vgaCRIndex, 0x60);
		outb(vgaCRReg, new->cr60);		

		new->cr54 = 31 << 3;
		outb(vgaCRIndex, 0x54);
		outb(vgaCRReg, new->cr54);		
	}

	outb(vgaCRIndex, 0x55);
	new->cr55 = (inb(vgaCRReg) & 0x08) | 0x40;
	outb(vgaCRReg, new->cr55);

	outb(vgaCRIndex, 0x5e);
	new->cr5e = (((mode->CrtcVTotal - 2) & 0x400) >> 10)	|
		    (((mode->CrtcVDisplay - 1) & 0x400) >> 9)	|
		    (((mode->CrtcVSyncStart) & 0x400) >> 8)	|
		    (((mode->CrtcVSyncStart) & 0x400) >> 6)	| 0x40;
	outb(vgaCRReg, new->cr5e);

	{
		int i;
		unsigned int j;
		
		i = ((((mode->CrtcHTotal >> 3) - 5) & 0x100) >> 8)      |
		    ((((mode->CrtcHDisplay >> 3) - 1) & 0x100) >> 7)    |
		    ((((mode->CrtcHSyncStart >> 3) - 1) & 0x100) >> 6)  |
		    ((mode->CrtcHSyncStart & 0x800) >> 7);
		if ((mode->CrtcHSyncEnd >> 3) - (mode->CrtcHSyncStart >> 3) > 64)
			i |= 0x08;
		if ((mode->CrtcHSyncEnd >> 3) - (mode->CrtcHSyncStart >> 3) > 32)
			i |= 0x20;

		outb(vgaCRIndex, 0x3b);
		j = ((pVga->CRTC[0] + ((i & 0x01) << 8) +
		      pVga->CRTC[4] + ((i & 0x10) << 4) + 1) / 2);
	
            	if (j - (pVga->CRTC[4] + ((i & 0x10) << 4)) < 4) {
                	if (pVga->CRTC[4] + ((i & 0x10) << 4) + 4 <= pVga->CRTC[0] + ((i & 0x01) << 8))
                    		j = pVga->CRTC[4] + ((i & 0x10) << 4) + 4;
                	else
                    		j = pVga->CRTC[0] + ((i & 0x01) << 8) + 1;
		}
		if (pS3->Chipset == PCI_CHIP_AURORA64VP) {
			outb(vgaCRReg, 0x00);
			i &= ~0x40;
		} else {
			new->cr3b = j & 0xff;
			outb(vgaCRReg, new->cr3b);
			i |= (j & 0x100) >> 2;
		}

		outb(vgaCRIndex, 0x3c);
		new->cr3c = (pVga->CRTC[0] + ((i & 0x01) << 8)) / 2;
		outb(vgaCRReg, new->cr3c);

		outb(vgaCRIndex, 0x5d);
		new->cr5d = (inb(vgaCRReg) & 0x80) | i;	
		outb(vgaCRReg, new->cr5d);
	}

	{
		int i;

		if (pScrn->videoRam > 1024)
			i = mode->HDisplay * pS3->s3Bpp / 8 + 1;
		else
			i = mode->HDisplay * pS3->s3Bpp / 4 + 1;

		outb(vgaCRIndex, 0x61);
		tmp = 0x80 | (inb(vgaCRReg) & 0x60) | (i >> 8);
		new->cr61 = tmp;
		outb(vgaCRReg, new->cr61);
		outb(vgaCRIndex, 0x62);
		new->cr62 = i & 0xff;
		outb(vgaCRReg, new->cr62);
	}

	if (mode->Flags & V_INTERLACE) {
		outb(vgaCRIndex, 0x42);
		new->cr42 = inb(vgaCRReg) | 0x20;
		outb(vgaCRReg, new->cr42);
	} else {
		outb(vgaCRIndex, 0x42);
		new->cr42 = inb(vgaCRReg) & ~0x20;
		outb(vgaCRReg, new->cr42);
	}

	if (pS3->hasStreams) {
		unsigned char a;

		outb(vgaCRIndex, 0x67);
		a = inb(vgaCRReg) & 0xfe;

		switch (pScrn->depth) {
		case 8:
			break;
		case 15:
			a |= (3 << 4);
			break;
		case 16:
			a |= (5 << 4);
			break;
		case 24:
			a |= (13 << 4);
			break;		 
		}

		if (pS3->hasStreams)
			a |= (3 << 2);

		WaitVSync(); /* Wait for VSync before setting mode */
		outb(vgaCRReg, a);
	}	    

	if (pS3->Chipset == PCI_CHIP_968) {
		unsigned char a;

		outb(vgaCRIndex, 0x67);
		a = inb(vgaCRReg) & 0xfe;
#if 0
		switch (pScrn->depth) {
			case 8:
				break;
			case 15:
				a |= (3 << 4);
				break;
			case 16:
				a |= (5 << 4);
				a |= (3 << 2);	/* streams */
				break;
			case 24:
				a |= (13 << 4);
				a |= (3 << 2);	/* streams */
				break;
		}
#endif
		outb(vgaCRReg, a);

		outb(vgaCRIndex, 0x6d);
		outb(vgaCRReg, 0x00);
	}

	if ((pS3->Chipset == PCI_CHIP_964_0) ||
	    (pS3->Chipset == PCI_CHIP_964_1)) {
		unsigned char bdelay;

		outb(vgaCRIndex, 0x6d);
		bdelay = inb(vgaCRReg);

		if (pS3->RamDac->RamDacType == TI3025_RAMDAC) {
			if (pS3->s3Bpp == 1) {
				if (mode->Clock > 80000)
					bdelay = 0x02;
				else
					bdelay = 0x03;
			} else if (pS3->s3Bpp == 2) {
				if (mode->Clock > 80000)
					bdelay = 0x00;
				else
					bdelay = 0x01;
			} else
				bdelay = 0x00;
		}

		outb(vgaCRReg, bdelay);
	}

	outb(vgaCRIndex, 0x66);
	new->cr66 = inb(vgaCRReg);
	if (pS3->S3NewMMIO)
		new->cr66 |= 0x88;
	else
		new->cr66 |= 0x80;
	outb(vgaCRReg, new->cr66);


	if (pS3->SlowDRAMRefresh)
		new->cr3a = 0xb7;
       	else
		new->cr3a = 0xb5;
	outb(vgaCRIndex, 0x3a);
	outb(vgaCRReg, new->cr3a);

	if (pS3->SlowVRAM) {
		/*
		 * some Diamond Stealth 64 VRAM cards have a problem with
		 * VRAM timing, increas -RAS low timing from 3.5 MCLKs
		 * to 4.5 MCLKs
		 */
		outb(vgaCRIndex, 0x39);
		outb(vgaCRReg, 0xa5);
		outb(vgaCRIndex, 0x68);
		tmp = inb(vgaCRReg);
		if (tmp & 0x30)				/* 3.5 MCLKs */
			outb(vgaCRReg, tmp & 0xef);	/* 4.5 MCLKs */
	}

	if (pS3->SlowDRAM) {
		/*
		 * fixes some pixel errors for a SPEA Trio64V+ card
		 * increase -RAS precharge timing from 2.5 MCLKs
		 * to 3.5 MCLKs
		 */
		outb(vgaCRIndex, 0x39);
		outb(vgaCRReg, 0xa5);
		outb(vgaCRIndex, 0x68);
		tmp = inb(vgaCRReg) & 0xf7;
		outb(vgaCRReg, tmp);			/* 3.5 MCLKs */
	}

	if (pS3->SlowEDODRAM) {
		/*
		 * fixes some pixel errors for a SPEA Trio64V+ card
		 * increase from 1-cycle to 2-cycle EDO mode
		 */
		outb(vgaCRIndex, 0x39);

		outb(vgaCRReg, 0xa5);
		outb(vgaCRIndex, 0x36);
		tmp = inb(vgaCRReg);
		if (!(tmp & 0x0c))			/* 1-cycle EDO */
			outb(vgaCRReg, tmp | 0x08);	/* 2-cycle EDO */
	}

	if (pS3->Chipset == PCI_CHIP_AURORA64VP) {
		outb(0x3c4, 0x08);
		outb(0x3c5, 0x06);
#if 0
		outb(0x3c4, 0x54);
		outb(0x3c5, 0x10);
		outb(0x3c4, 0x55);
		outb(0x3c5, 0x00);
		outb(0x3c4, 0x56);
		outb(0x3c5, 0x1c);
		outb(0x3c4, 0x57);
		outb(0x3c5, 0x00);
#else
		outb(0x3c4, 0x54);
		outb(0x3c5, 0x1f);
		outb(0x3c4, 0x55);
		outb(0x3c5, 0x1f);
		outb(0x3c4, 0x56);
		outb(0x3c5, 0x1f);
		outb(0x3c4, 0x57);
		outb(0x3c5, 0x1f);
#endif

		outb(0x3c4, 0x08);
		outb(0x3c5, 0x00);
	}

	pScrn->AdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));

       	vgaHWProtect(pScrn, FALSE);

	if (pScrn->displayWidth == 1024)
		outw(ADVFUNC_CNTL, 0x0007);
	else
		outw(ADVFUNC_CNTL, 0x0003);

	outb(0x3c6, 0x00);

	outw(SUBSYS_CNTL, 0x8000 | 0x1000);
	outw(SUBSYS_CNTL, 0x4000 | 0x1000);

	inw(SUBSYS_STAT);

	outw(0xbee8, 0x5000 | 0x0004 | 0x000c);	

	outb(0x3c6, 0xff);

	new->cr59 = pS3->FBAddress >> 24;
	new->cr5a = pS3->FBAddress >> 16;

	if (pScrn->videoRam <= 1024)
		new->cr58 = 0x15;
	else if (pScrn->videoRam <= 2048)
		new->cr58 = 0x16;
	else
		new->cr58 = 0x17;

	if ((pS3->Chipset == PCI_CHIP_968) ||
	    (pS3->Chipset == PCI_CHIP_964_0) ||
	    (pS3->Chipset == PCI_CHIP_964_1))
		new->cr58 |= 0x40;

	outb(vgaCRIndex, 0x59);
	outb(vgaCRReg, new->cr59);
	outb(vgaCRIndex, 0x5a);
	outb(vgaCRReg, new->cr5a);
	outb(vgaCRIndex, 0x58);
	outb(vgaCRReg, new->cr58);

	WaitQueue(5);
	SET_SCISSORS(0, 0, pS3->s3ScissR, pS3->s3ScissB);

	if (pS3->hasStreams)
		S3InitStreams(pScrn, mode);

	return TRUE;
}


static Bool S3EnterVT(VT_FUNC_ARGS_DECL)
{       
	SCRN_INFO_PTR(arg);
        vgaHWPtr hwp = VGAHWPTR(pScrn);

        vgaHWUnlock(hwp);
        if (!S3ModeInit(pScrn, pScrn->currentMode))
                return FALSE;
        
        return TRUE;
}               


static void S3Restore(ScrnInfoPtr pScrn)
{
        S3Ptr pS3 = S3PTR(pScrn);
        S3RegPtr restore = &pS3->SavedRegs;
        vgaHWPtr hwp = VGAHWPTR(pScrn);
        int vgaCRIndex = pS3->vgaCRIndex, vgaCRReg = pS3->vgaCRReg;
	int i;

	vgaHWProtect(pScrn, TRUE);
	WaitQueue(8);

	S3BankZero(pScrn);

	outw(ADVFUNC_CNTL, 0x0000);

	if (pS3->S3NewMMIO) {
		outb(vgaCRIndex, 0x53);
		outb(vgaCRReg, 0x00);
	}

	pS3->DacRestore(pScrn);

	if (pS3->RamDac->RamDacType == TI3025_RAMDAC) {
		outb(vgaCRIndex, 0x5c);
		outb(vgaCRReg, restore->s3syssave[0x0c + 16]);
	}

	for(i=32; i<46; i++) {
		outb(vgaCRIndex, 0x40 + i);
		outb(vgaCRReg, restore->s3syssave[i]);
	}

	for(i=0; i<16; i++) {
		if (!((1 << i) & 0x673b))
			continue;
		outb(vgaCRIndex, 0x50 + i);
		outb(vgaCRReg, restore->s3syssave[i+16]);
	}

	for(i=0; i<5; i++) {
		outb(vgaCRIndex, 0x30 + i);
		outb(vgaCRReg, restore->s3save[i]);
		outb(vgaCRIndex, 0x38 + i);
		outb(vgaCRReg, restore->s3save[i + 5]);
	}

	for(i=0; i<16; i++) {
		outb(vgaCRIndex, 0x40 + i);
		outb(vgaCRReg, restore->s3syssave[i]);
	}

	outb(vgaCRIndex, 0x45);
	inb(vgaCRReg);
	outb(vgaCRIndex, 0x4a);
	for(i=0; i<4; i++)
		outb(vgaCRReg, restore->color_stack[i]);

	outb(vgaCRIndex, 0x45);
	inb(vgaCRReg);
	outb(vgaCRIndex, 0x4b);
	for(i=4; i<8; i++)
		outb(vgaCRReg, restore->color_stack[i]);

        vgaHWRestore(pScrn, &hwp->SavedReg, VGA_SR_ALL);

	outb(0x3c2, restore->clock);

	vgaHWProtect(pScrn, FALSE);

}


static void S3LeaveVT(VT_FUNC_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);
        vgaHWPtr hwp = VGAHWPTR(pScrn);

        S3Restore(pScrn);
        vgaHWLock(hwp);

        return;
}       


static void S3AdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
	SCRN_INFO_PTR(arg);
        S3Ptr pS3 = S3PTR(pScrn);
	S3RegPtr regs = &pS3->ModeRegs;
        int vgaCRIndex = pS3->vgaCRIndex, vgaCRReg = pS3->vgaCRReg;
	int base, orig_base;
	unsigned char tmp;

	if (x > pScrn->displayWidth - pS3->HDisplay)
		x = pScrn->displayWidth - pS3->HDisplay;

	orig_base = (y * pScrn->displayWidth + x) * pS3->s3Bpp;
	base = (orig_base >> 2) & ~1;

	/* for IBMRGB and TI only */
	if (pS3->RamDac->RamDacType == IBM524A_RAMDAC)
	{
		int px, py, a;

		miPointerGetPosition(inputInfo.pointer, &px, &py);

		if (pS3->s3Bpp == 1)
			a = 4 - 1;
		else
			a = 8 - 1;
		if (px-x > pS3->HDisplay/2)
			base = ((orig_base + a*4) >> 2) & ~1;
		base &= ~a;
	}

	outb(vgaCRIndex, 0x31);
	outb(vgaCRReg, ((base & 0x030000) >> 12) | regs->cr31);
	regs->cr51 &= ~0x03;
	regs->cr51 |= ((base & 0x0c0000) >> 18);
	outb(vgaCRIndex, 0x51);
	tmp = (inb(vgaCRReg) & ~0x03) | (regs->cr51 & 0x03);
	outb(vgaCRReg, tmp);

	outw(vgaCRIndex, (base & 0x00ff00) | 0x0c);
	outw(vgaCRIndex, ((base & 0x00ff) << 8) | 0x0d);
}


void S3Regdump(ScrnInfoPtr pScrn)
{
        S3Ptr pS3 = S3PTR(pScrn);
        int vgaCRIndex = pS3->vgaCRIndex, vgaCRReg = pS3->vgaCRReg;
	{
		int j;

		for(j=0; j<0x100; j++) {
			outb(vgaCRIndex, j);
			ErrorF("CRTC 0x%x = 0x%x\n", j, inb(vgaCRReg));
		}
	}
#if 0
	ErrorF("DAC regs\n");

	{
		int j;

		for(j=0; j<0x100; j++)
			ErrorF("0x%x = 0x%x\n", j, S3InTiIndReg(pScrn, j));
#if 0
		outb(vgaCRIndex, 0x22);
		ErrorF("cr22 = 0x%x\n", inb(vgaCRReg));
#endif
	}
#endif
}


void S3BankZero(ScrnInfoPtr pScrn)
{
        S3Ptr pS3 = S3PTR(pScrn);
	unsigned char tmp;
        int vgaCRIndex = pS3->vgaCRIndex, vgaCRReg = pS3->vgaCRReg;

	outb(vgaCRIndex, 0x35);
	tmp = inb(vgaCRReg) & 0xf0;
	outb(vgaCRReg, tmp);

	outb(vgaCRIndex, 0x51);
	tmp = inb(vgaCRReg) & 0xf3;
	outb(vgaCRReg, tmp);
}

static void
S3DisplayPowerManagementSet(ScrnInfoPtr pScrn, int PowerManagementMode,
			    int flags)
{
     S3Ptr pS3 = S3PTR(pScrn);
     switch (pS3->Chipset) {
     case PCI_CHIP_TRIO64V2_DXGX:
     case PCI_CHIP_TRIO:
     case PCI_CHIP_AURORA64VP:
     case PCI_CHIP_TRIO64UVP:
     {
	  int srd;
      
	  outb(0x3c4, 0x08);
	  outb(0x3c5, 0x06);	  /* unlock extended sequence registers */

	  outb(0x3c4, 0x0d);
	  srd = inb(0x3c5) & 0xf;  /* clear the sync control bits */
  
	  switch (PowerManagementMode) {
	  case DPMSModeOn:
	       /* Screen: On; HSync: On, VSync: On */
	       break;
	  case DPMSModeStandby:
	       /* Screen: Off; HSync: Off, VSync: On */
	       srd |= 0x10;
	       break;
	  case DPMSModeSuspend:
	       /* Screen: Off; HSync: On, VSync: Off */
	       srd |= 0x40;
	       break;
	  case DPMSModeOff:
	       /* Screen: Off; HSync: Off, VSync: Off */
	       srd |= 0x50;
	       break;
	  }
	  outb(0x3c4, 0x0d);
	  outb(0x3c5, srd);
	  break;
     }
     default:
	  vgaHWDPMSSet(pScrn, PowerManagementMode, flags);
     }
}
