/*
 *
 * Copyright 1990,91 by Thomas Roell, Dinkelscherben, Germany.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Thomas Roell not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Thomas Roell makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THOMAS ROELL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THOMAS ROELL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * Author:  Thomas Roell, roell@informatik.tu-muenchen.de
 *          ET6000 and ET4000W32 16/24/32 bpp and acceleration support by Koen Gadeyne
 *
 * Large parts rewritten for XFree86 4.0 by Koen Gadeyne.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*** Generic includes ***/

#include "tseng.h"		       /* this includes most of the generic ones as well */

#include "xf86PciInfo.h"

/* All drivers initialising the SW cursor need this */
#include "mipointer.h"

#include "fb.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86RAC.h"
#include "xf86Resources.h"
#endif
#include "xf86int10.h"

#include "xf86xv.h"
#include <X11/extensions/Xv.h>

/*
 * Forward definitions for the functions that make up the driver.
 */

/* Mandatory functions */
static const OptionInfoRec * TsengAvailableOptions(int chipid, int busid);
static void TsengIdentify(int flags);
static Bool TsengProbe(DriverPtr drv, int flags);
static Bool TsengPreInit(ScrnInfoPtr pScrn, int flags);
static Bool TsengScreenInit(SCREEN_INIT_ARGS_DECL);
static Bool TsengEnterVT(VT_FUNC_ARGS_DECL);
static void TsengLeaveVT(VT_FUNC_ARGS_DECL);
static Bool TsengCloseScreen(CLOSE_SCREEN_ARGS_DECL);
static Bool TsengSaveScreen(ScreenPtr pScreen, int mode);

/* Required if the driver supports mode switching */
static Bool TsengSwitchMode(SWITCH_MODE_ARGS_DECL);

/* Optional functions */
static void TsengFreeScreen(FREE_SCREEN_ARGS_DECL);

/* If driver-specific config file entries are needed, this must be defined */
/*static Bool   TsengParseConfig(ParseInfoPtr raw); */

/* Internally used functions (some are defined in tseng.h) */
static Bool TsengMapMem(ScrnInfoPtr pScrn);
static Bool TsengUnmapMem(ScrnInfoPtr pScrn);
static void TsengUnlock(ScrnInfoPtr pScrn);
static void TsengLock(ScrnInfoPtr pScrn);

/*
 * This is intentionally screen-independent.  It indicates the binding
 * choice made in the first PreInit.
 */
static int pix24bpp = 0;
 
#define TSENG_NAME "TSENG"
#define TSENG_DRIVER_NAME "tseng"
#define TSENG_MAJOR_VERSION 1
#define TSENG_MINOR_VERSION 1
#define TSENG_PATCHLEVEL 0
#define TSENG_VERSION (TSENG_MAJOR_VERSION << 24) | (TSENG_MINOR_VERSION << 16) | TSENG_PATCHLEVEL

/* CRTC timing limits */
#define Tseng_HMAX (4096-8)
#define Tseng_VMAX (2048-1)

/* 
 * This contains the functions needed by the server after loading the
 * driver module.  It must be supplied, and gets added the driver list by
 * the Module Setup funtion in the dynamic case.  In the static case a
 * reference to this is compiled in, and this requires that the name of
 * this DriverRec be an upper-case version of the driver name.
 */

_X_EXPORT DriverRec TSENG =
{
    TSENG_VERSION,
    TSENG_DRIVER_NAME,
    TsengIdentify,
    TsengProbe,
    TsengAvailableOptions,
    NULL,
    0
};

/* sub-revisions are now dealt with in the ChipRev variable */
static SymTabRec TsengChipsets[] =
{
    {ET4000, "ET4000W32p"},
    {ET6000, "ET6000"},
    {-1, NULL}
};

/* Convert PCI ID to chipset name */
static PciChipsets TsengPciChipsets[] =
{
    {ET4000, PCI_CHIP_ET4000_W32P_A, RES_SHARED_VGA},
    {ET4000, PCI_CHIP_ET4000_W32P_B, RES_SHARED_VGA},
    {ET4000, PCI_CHIP_ET4000_W32P_C, RES_SHARED_VGA},
    {ET4000, PCI_CHIP_ET4000_W32P_D, RES_SHARED_VGA},
    {ET6000, PCI_CHIP_ET6000,        RES_SHARED_VGA},
    {-1,     -1,                     RES_UNDEFINED}
};

typedef enum {
    OPTION_HIBIT_HIGH,
    OPTION_HIBIT_LOW,
    OPTION_SW_CURSOR,
    OPTION_HW_CURSOR,
    OPTION_PCI_BURST,
    OPTION_SLOW_DRAM,
    OPTION_MED_DRAM,
    OPTION_FAST_DRAM,
    OPTION_W32_INTERLEAVE,
    OPTION_NOACCEL,
    OPTION_SHOWCACHE,
    OPTION_PCI_RETRY
} TsengOpts;

static const OptionInfoRec TsengOptions[] =
{
    {OPTION_HIBIT_HIGH, "hibit_high", OPTV_BOOLEAN,
	{0}, FALSE},
    {OPTION_HIBIT_LOW, "hibit_low", OPTV_BOOLEAN,
	{0}, FALSE},
    {OPTION_SW_CURSOR, "SWcursor", OPTV_BOOLEAN,
	{0}, FALSE},
    {OPTION_HW_CURSOR, "HWcursor", OPTV_BOOLEAN,
	{0}, FALSE},
    {OPTION_PCI_BURST, "pci_burst", OPTV_BOOLEAN,
	{0}, FALSE},
    {OPTION_SLOW_DRAM, "slow_dram", OPTV_BOOLEAN,
	{0}, FALSE},
    {OPTION_MED_DRAM, "med_dram", OPTV_BOOLEAN,
	{0}, FALSE},
    {OPTION_FAST_DRAM, "fast_dram", OPTV_BOOLEAN,
	{0}, FALSE},
    {OPTION_W32_INTERLEAVE, "w32_interleave", OPTV_BOOLEAN,
	{0}, FALSE},
    {OPTION_NOACCEL, "NoAccel", OPTV_BOOLEAN,
	{0}, FALSE},
    {OPTION_SHOWCACHE, "ShowCache", OPTV_BOOLEAN,
	{0}, FALSE},
    {OPTION_PCI_RETRY, "PciRetry", OPTV_BOOLEAN,
	{0}, FALSE},
    {-1, NULL, OPTV_NONE,
	{0}, FALSE}
};

#ifdef XFree86LOADER

static MODULESETUPPROTO(tsengSetup);

static XF86ModuleVersionInfo tsengVersRec =
{
    "tseng",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    TSENG_MAJOR_VERSION, TSENG_MINOR_VERSION, TSENG_PATCHLEVEL,
    ABI_CLASS_VIDEODRV,		       /* This is a video driver */
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_VIDEODRV,
    {0, 0, 0, 0}
};

/*
 * This is the module init data for XFree86 modules.
 *
 * Its name has to be the driver name followed by ModuleData.
 */
_X_EXPORT XF86ModuleData tsengModuleData = { &tsengVersRec, tsengSetup, NULL };

static pointer
tsengSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    static Bool setupDone = FALSE;

    if (!setupDone) {
	setupDone = TRUE;
	xf86AddDriver(&TSENG, module, 0);

	/*
	 * The return value must be non-NULL on success even though there
	 * is no TearDownProc.
	 */
	return (pointer) 1;
    } else {
	if (errmaj)
	    *errmaj = LDR_ONCEONLY;
	return NULL;
    }
}

#endif /* XFree86LOADER */

static Bool
TsengGetRec(ScrnInfoPtr pScrn)
{
    TsengPtr pTseng;

    PDEBUG("	TsengGetRec\n");

    /*
     * Allocate an TsengRec, and hook it into pScrn->driverPrivate.
     * pScrn->driverPrivate is initialised to NULL, so we can check if
     * the allocation has already been done.
     */
    if (pScrn->driverPrivate != NULL)
	return TRUE;

    pScrn->driverPrivate = xnfcalloc(sizeof(TsengRec), 1);

    
    /* Initialise it here when needed (or possible) */
    pTseng = TsengPTR(pScrn);

    pTseng->SavedReg.RAMDAC = NULL;

    return TRUE;
}

static void
TsengFreeRec(ScrnInfoPtr pScrn)
{
    TsengPtr pTseng;

    PDEBUG("	TsengFreeRec\n");

    if (pScrn->driverPrivate == NULL)
	return;

    pTseng = TsengPTR(pScrn);
    
    if (pTseng->SavedReg.RAMDAC)
        free(pTseng->SavedReg.RAMDAC);

    free(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
}

static const OptionInfoRec *
TsengAvailableOptions(int chipid, int busid)
{
    return TsengOptions;
}

static void
TsengIdentify(int flags)
{
    xf86Msg(X_INFO, TSENG_NAME ": driver for TsengLabs ET4000W32p, ET6000 and"
            " ET6100 chips.\n");
}

/* unlock ET4000 using KEY register */
static void
TsengUnlock(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD8 tmp;

    PDEBUG("	TsengUnlock\n");

    vgaHWHerculesSecondPage(hwp, TRUE);
    vgaHWWriteModeControl(hwp, 0xA0);

    tmp = hwp->readCrtc(hwp, 0x11);
    hwp->writeCrtc(hwp, 0x11, tmp & 0x7F);
}

/* lock ET4000 using KEY register. FIXME: should restore old lock status instead */
static void
TsengLock(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD8 tmp;

    PDEBUG("	TsengLock\n");

    tmp = hwp->readCrtc(hwp, 0x11);
    hwp->writeCrtc(hwp, 0x11, tmp | 0x80);

    vgaHWWriteModeControl(hwp, 0x00);
    vgaHWWriteModeControl(hwp, 0x29);
    vgaHWHerculesSecondPage(hwp, FALSE);
}

static Bool
TsengProbe(DriverPtr drv, int flags)
{
    int i;
    GDevPtr *devSections;
    int numDevSections;
    int numUsed;
    int *usedChips = NULL;
    Bool foundScreen = FALSE;
    
    
    PDEBUG("	TsengProbe\n");
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
     * Find the config file Device sections that match this
     * driver, and return if there are none.
     */
    if ((numDevSections = xf86MatchDevice(TSENG_DRIVER_NAME,
		&devSections)) <= 0) {
	return FALSE;
    }

#ifndef XSERVER_LIBPCIACCESS
    /* PCI only driver now. */
    if (!xf86GetPciVideoInfo())
        return FALSE;
#endif

    /* XXX maybe this can go some time soon */
    /*
     * for the Tseng server, there can only be one matching
     * device section. So issue a warning if more than one show up.
     * Multiple Tseng cards in the same machine are not possible.
     */
    numUsed = xf86MatchPciInstances(TSENG_NAME, PCI_VENDOR_TSENG,
                                    TsengChipsets, TsengPciChipsets, 
                                    devSections,numDevSections, drv,
                                    &usedChips);
    if (numUsed > 0) {
        if (flags & PROBE_DETECT)
            foundScreen = TRUE;
        else for (i = 0; i < numUsed; i++) {
            /* Allocate a ScrnInfoRec  */
            ScrnInfoPtr pScrn = NULL;
            if ((pScrn = xf86ConfigPciEntity(pScrn,0,usedChips[i],
                                             TsengPciChipsets,NULL,
                                             NULL,NULL,NULL,NULL))) {
                pScrn->driverVersion = TSENG_VERSION;
                pScrn->driverName = TSENG_DRIVER_NAME;
                pScrn->name = TSENG_NAME;
                pScrn->Probe = TsengProbe;
                pScrn->PreInit = TsengPreInit;
                pScrn->ScreenInit = TsengScreenInit;
                pScrn->SwitchMode = TsengSwitchMode;
                pScrn->AdjustFrame = TsengAdjustFrame;
                pScrn->EnterVT = TsengEnterVT;
                pScrn->LeaveVT = TsengLeaveVT;
                pScrn->FreeScreen = TsengFreeScreen;
                pScrn->ValidMode = TsengValidMode;

                foundScreen = TRUE;
            }
        }
        free(usedChips);
    }
    
    free(devSections);
    return foundScreen;
}

/* The PCI part of TsengPreInit() */
static Bool
TsengPreInitPCI(ScrnInfoPtr pScrn)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    PDEBUG("	TsengPreInitPCI\n");

    /* This is PCI, we should be able to trust it */

    /* Set up ChipType, ChipRev and pScrn->chipset.
     * This last one is usually not done manually, but
     * it's for informative use only anyway. */
    switch (PCI_DEV_DEVICE_ID(pTseng->PciInfo)) {
    case PCI_CHIP_ET4000_W32P_A:
	pTseng->ChipType = ET4000;
	pTseng->ChipRev = REV_A;
        pScrn->chipset = "ET4000/W32P (rev A)";
	break;
    case PCI_CHIP_ET4000_W32P_B:
	pTseng->ChipType = ET4000;
	pTseng->ChipRev = REV_B;
	pScrn->chipset = "ET4000/W32P (rev B)";
        break;
    case PCI_CHIP_ET4000_W32P_C:
	pTseng->ChipType = ET4000;
	pTseng->ChipRev = REV_C;
        pScrn->chipset = "ET4000/W32P (rev C)";
	break;
    case PCI_CHIP_ET4000_W32P_D:
	pTseng->ChipType = ET4000;
	pTseng->ChipRev = REV_D;
        pScrn->chipset = "ET4000/W32P (rev D)";
	break;
    case PCI_CHIP_ET6000:
	pTseng->ChipType = ET6000;

        if (PCI_DEV_REVISION(pTseng->PciInfo) < 0x70) {
            pScrn->chipset = "ET6000";
            pTseng->ChipRev = REV_ET6000;
        } else {
            pScrn->chipset = "ET6100";
            pTseng->ChipRev = REV_ET6100;
        }
	break;
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unknown Tseng PCI ID: %X\n",
                   PCI_DEV_DEVICE_ID(pTseng->PciInfo));
	return FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Chipset: \"%s\"\n", pScrn->chipset);

#ifndef XSERVER_LIBPCIACCESS
    pTseng->PciTag = pciTag(pTseng->PciInfo->bus, pTseng->PciInfo->device,
	pTseng->PciInfo->func);
#endif

    /* only the ET6000 implements a PCI IO address */
    if (pTseng->ChipType == ET6000) {
        if (!PCI_REGION_BASE(pTseng->PciInfo, 1, REGION_IO)) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "No valid PCI I/O address in PCI config space\n");
            return FALSE;
        }

        pTseng->ET6000IOAddress = PCI_REGION_BASE(pTseng->PciInfo, 1, REGION_IO);
        
        xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "ET6000 PCI I/O registers at 0x%lX\n",
                   (unsigned long)pTseng->ET6000IOAddress);
    }

    return TRUE;
}

/*
 * The 8*32kb ET6000 MDRAM granularity causes the more general probe to
 * detect too much memory in some configurations, because that code has a
 * 8-bank (=256k) granularity. E.g. it fails to recognize 2.25 MB of memory
 * (detects 2.5 instead). This function goes to check if the RAM is actually
 * there. MDRAM comes in multiples of 4 banks (16, 24, 32, 36, 40, 64, 72,
 * 80, ... 32kb-banks), so checking each 64k block should be enough granularity.
 *
 * No more than the amount of refreshed RAM is checked. Non-refreshed RAM
 * won't work anyway.
 *
 * The same code could be used on other Tseng chips, or even on ANY
 * VGA board, but probably only in case of trouble.
 *
 * FIXME: this should be done using linear memory
 */
#define VIDMEM ((volatile CARD32*)check_vgabase)
#define SEGSIZE (64)		       /* kb */

#define ET6K_SETSEG(seg) \
    vgaHWWriteBank(hwp, ((seg) & 0x30) | ((seg) >> 4));\
    vgaHWWriteSegment(hwp, ((seg) & 0x0f) | ((seg) << 4));

static int
et6000_check_videoram(ScrnInfoPtr pScrn, int ram)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    unsigned char oldSegSel1, oldSegSel2, oldGR5, oldGR6, oldSEQ2, oldSEQ4;
    int segment, i;
    int real_ram = 0;
    pointer check_vgabase;
    Bool fooled = FALSE;
    int save_vidmem;

    PDEBUG("	et6000_check_videoram\n");
    if (ram > 4096) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	    "Detected more than 4096 kb of video RAM. Clipped to 4096kb\n");
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	    "    (Tseng VGA chips can only use 4096kb).\n");
	ram = 4096;
    }
    if (!vgaHWMapMem(pScrn)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	    "Could not map VGA memory to check for video memory.\n");
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	    "    Detected amount may be wrong.\n");
	return ram;
    }
    check_vgabase = (VGAHWPTR(pScrn)->Base);

    /*
     * We need to set the VGA controller in VGA graphics mode, or else we won't
     * be able to access the full 4MB memory range. First, we save the
     * registers we modify, of course.
     */

    oldSegSel1 = vgaHWReadSegment(hwp);
    oldSegSel2 = vgaHWReadBank(hwp);

    oldGR5 = hwp->readGr(hwp, 0x05);
    oldGR6 = hwp->readGr(hwp, 0x06);

    oldSEQ2 = hwp->readSeq(hwp, 0x02);
    oldSEQ4 = hwp->readSeq(hwp, 0x04);

    /* set graphics mode */
    hwp->writeGr(hwp, 0x06, 0x05);
    hwp->writeGr(hwp, 0x05, 0x40);
    hwp->writeSeq(hwp, 0x02, 0x0F);
    hwp->writeSeq(hwp, 0x04, 0x0E);

    /*
     * count down from presumed amount of memory in SEGSIZE steps, and
     * look at each segment for real RAM.
     *
     * To select a segment, we cannot use ET4000W32SetReadWrite(), since
     * that requires the ScreenPtr, which we don't have here.
     */

    for (segment = (ram / SEGSIZE) - 1; segment >= 0; segment--) {
	/* select the segment */
	ET6K_SETSEG(segment);

	/* save contents of memory probing location */
	save_vidmem = *(VIDMEM);

	/* test with pattern */
	*VIDMEM = 0xAAAA5555;
	if (*VIDMEM != 0xAAAA5555) {
	    *VIDMEM = save_vidmem;
	    continue;
	}
	/* test with inverted pattern */
	*VIDMEM = 0x5555AAAA;
	if (*VIDMEM != 0x5555AAAA) {
	    *VIDMEM = save_vidmem;
	    continue;
	}
	/*
	 * If we get here, the memory seems to be writable/readable
	 * Now check if we aren't fooled by address wrapping (mirroring)
	 */
	fooled = FALSE;
	for (i = segment - 1; i >= 0; i--) {
	    /* select the segment */
	    ET6K_SETSEG(i);

            /* again? */
	    vgaHWWriteBank(hwp, (i & 0x30) | (i >> 4));
	    vgaHWWriteSegment(hwp, (i & 0x0f) | (i << 4));

	    if (*VIDMEM == 0x5555AAAA) {
		/*
		 * Seems like address wrap, but there could of course be
		 * 0x5555AAAA in here by accident, so we check with another
		 * pattern again.
		 */
		ET6K_SETSEG(segment);
		/* test with other pattern again */
		*VIDMEM = 0xAAAA5555;
		ET6K_SETSEG(i);
		if (*VIDMEM == 0xAAAA5555) {
		    /* now we're sure: this is not real memory */
		    fooled = TRUE;
		    break;
		}
	    }
	}
	if (!fooled) {
	    real_ram = (segment + 1) * SEGSIZE;
	    break;
	}
	/* restore old contents again */
	ET6K_SETSEG(segment);
	*VIDMEM = save_vidmem;
    }

    /* restore original register contents */
    vgaHWWriteSegment(hwp, oldSegSel1);
    vgaHWWriteBank(hwp, oldSegSel2);
                 
    hwp->writeGr(hwp, 0x05, oldGR5);
    hwp->writeGr(hwp, 0x06, oldGR6);
    hwp->writeSeq(hwp, 0x02, oldSEQ2);
    hwp->writeSeq(hwp, 0x04, oldSEQ4);

    vgaHWUnmapMem(pScrn);
    return real_ram;
}

/*
 * Handle amount of allowed memory: some combinations can't use all
 * available memory. Should we still allow the user to override this?
 *
 * This must be called AFTER the decision has been made to use linear mode
 * and/or acceleration, or the memory limit code won't be able to work.
 */

static int
TsengDoMemLimit(ScrnInfoPtr pScrn, int ram, int limit, char *reason)
{
    if (ram > limit) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Only %d kb of memory can be used %s.\n",
	    limit, reason);
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Reducing video memory to %d kb.\n", limit);
	ram = limit;
    }
    return ram;
}

static int
TsengLimitMem(ScrnInfoPtr pScrn, int ram)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    if (pTseng->UseAccel) {
	if (pTseng->ChipType == ET4000) {
	    /* <= W32p_ab :
	     *   2 MB direct access + 2*512kb via apertures MBP0 and MBP1
	     * == W32p_cd :
	     *   2*1MB via apertures MBP0 and MBP1
	     */
	    if ((pTseng->ChipRev == REV_C) || (pTseng->ChipRev == REV_D))
		ram = TsengDoMemLimit(pScrn, ram, 2048,
				      "in linear + accelerated mode "
				      "on W32p rev c and d");

	    ram = TsengDoMemLimit(pScrn, ram, 2048 + 1024,
				  "in linear + accelerated mode "
				  "on W32/W32i/W32p");

	    /*
	     * upper 516kb of 4MB linear map used for
	     *  "externally mapped registers"
	     */
	    ram = TsengDoMemLimit(pScrn, ram, 4096 - 516,
				  "in linear + accelerated mode "
				  "on W32/W32i/W32p");
	} else {
	    /*
	     * upper 8kb used for externally mapped and
	     * memory mapped registers
	     */
	    ram = TsengDoMemLimit(pScrn, ram, 4096 - 8,
				  "in linear + accelerated mode "
				  "on ET6000/6100");
	}
    }
    ram = TsengDoMemLimit(pScrn, ram, 4096, "on any Tseng card");
    return ram;
}

/*
 * TsengDetectMem --
 *      try to find amount of video memory installed.
 *
 */
static int
TsengDetectMem(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    TsengPtr pTseng = TsengPTR(pScrn);
    unsigned char config;
    int ramtype = 0;
    int ram = 0;

    PDEBUG("	TsengDetectMem\n");
    if (pTseng->ChipType == ET6000) {
	ramtype = hwp->readST00(hwp) & 0x03;
	switch (ramtype) {
	case 0x03:		       /* MDRAM */
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Video memory type: Multibank DRAM (MDRAM).\n");
	    ram = ((ET6000IORead(pTseng, 0x47) & 0x07) + 1) * 8 * 32;	/* number of 8 32kb banks  */
	    if (ET6000IORead(pTseng, 0x45) & 0x04) {
		ram <<= 1;
	    }
	    /*
	     * 8*32kb MDRAM refresh control granularity in the ET6000 fails to
	     * recognize 2.25 MB of memory (detects 2.5 instead)
	     */
	    ram = et6000_check_videoram(pScrn, ram);
	    break;
	case 0x00:		       /* DRAM -- VERY unlikely on ET6000 cards, IMPOSSIBLE on ET6100 */
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Video memory type: Standard DRAM.\n");
	    ram = 1024 << (ET6000IORead(pTseng, 0x45) & 0x03);
	    break;
	default:		       /* unknown RAM type */
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"Unknown ET6000 video memory type %d -- assuming 1 MB (unless specified)\n",
		ramtype);
	    ram = 1024;
	}
    } else {
	config = hwp->readCrtc(hwp, 0x37);
        
	ram = 128 << (config & 0x03);

	if (config & 0x80)
	    ram <<= 1;

	/* Check for interleaving on W32i/p. */
        config = hwp->readCrtc(hwp, 0x32);
        if (config & 0x80) {
            ram <<= 1;
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "Video memory type: Interleaved DRAM.\n");
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "Video memory type: Standard DRAM.\n");
        }
    }
    return ram;
}

static Bool
TsengProcessHibit(ScrnInfoPtr pScrn)
{
    TsengPtr pTseng = TsengPTR(pScrn);
    MessageType from = X_CONFIG;
    int hibit_mode_width;

    PDEBUG("	TsengProcessHibit\n");
    if (xf86IsOptionSet(pTseng->Options, OPTION_HIBIT_HIGH)) {
	if (xf86IsOptionSet(pTseng->Options, OPTION_HIBIT_LOW)) {
	    xf86Msg(X_ERROR, "\nOptions \"hibit_high\" and \"hibit_low\" are incompatible;\n");
	    xf86Msg(X_ERROR, "    specify only one (not both) in X configuration file\n");
	    return FALSE;
	}
	pTseng->save_divide = 0x40;
    } else if (xf86IsOptionSet(pTseng->Options, OPTION_HIBIT_HIGH)) {
	pTseng->save_divide = 0;
    } else {
        vgaHWPtr hwp = VGAHWPTR(pScrn);

	from = X_PROBED;

	/* first check to see if hibit is probed from low-res mode */
	hibit_mode_width = hwp->readCrtc(hwp, 0x01) + 1;

	if (hibit_mode_width > 82) {
	    xf86Msg(X_WARNING, "Non-standard VGA text or graphics mode while probing for hibit:\n");
	    xf86Msg(X_WARNING, "    probed 'hibit' value may be wrong.\n");
	    xf86Msg(X_WARNING, "    Preferably run probe from 80x25 textmode,\n");
	    xf86Msg(X_WARNING, "    or specify correct value in X configuration file.\n");
	}

	/* Check for initial state of divide flag */
	pTseng->save_divide = hwp->readSeq(hwp, 0x07) & 0x40;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Initial ET4000 hibit state: %s\n",
	pTseng->save_divide & 0x40 ? "high" : "low");
    return TRUE;
}

static Bool
TsengProcessOptions(ScrnInfoPtr pScrn)
{
    MessageType from;
    TsengPtr pTseng = TsengPTR(pScrn);

    PDEBUG("	TsengProcessOptions\n");

    /* Collect all of the relevant option flags (fill in pScrn->options) */
    xf86CollectOptions(pScrn, NULL);

    /* Process the options */
    if (!(pTseng->Options = malloc(sizeof(TsengOptions))))
	return FALSE;
    memcpy(pTseng->Options, TsengOptions, sizeof(TsengOptions));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pTseng->Options);

    from = X_DEFAULT;
    pTseng->HWCursor = FALSE;	       /* default */
    if (xf86GetOptValBool(pTseng->Options, OPTION_HW_CURSOR, &pTseng->HWCursor))
	from = X_CONFIG;
    if (xf86ReturnOptValBool(pTseng->Options, OPTION_SW_CURSOR, FALSE)) {
	from = X_CONFIG;
	pTseng->HWCursor = FALSE;
    }
    if ((pTseng->ChipType == ET4000) && pTseng->HWCursor) {
	xf86DrvMsg(pScrn->scrnIndex, from,
		"Hardware Cursor not supported on this chipset\n");
	pTseng->HWCursor = FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, from, "Using %s cursor\n",
	pTseng->HWCursor ? "HW" : "SW");

    if (pScrn->bitsPerPixel >= 8) {
        pTseng->UseAccel = TRUE;
	if (xf86ReturnOptValBool(pTseng->Options, OPTION_NOACCEL, FALSE)) {
	    pTseng->UseAccel = FALSE;
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Acceleration disabled\n");
	}
    } else
	pTseng->UseAccel = FALSE;  /* 1bpp and 4bpp are always non-accelerated */

    pTseng->SlowDram = FALSE;
    if (xf86IsOptionSet(pTseng->Options, OPTION_SLOW_DRAM)) {
	pTseng->SlowDram = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Using slow DRAM access\n");
    }
    pTseng->MedDram = FALSE;
    if (xf86IsOptionSet(pTseng->Options, OPTION_MED_DRAM)) {
	pTseng->MedDram = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Using Medium-speed DRAM access\n");
    }
    pTseng->FastDram = FALSE;
    if (xf86IsOptionSet(pTseng->Options, OPTION_FAST_DRAM)) {
	pTseng->FastDram = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Using fast DRAM access\n");
    }
    if ((pTseng->SetW32Interleave = 
	xf86GetOptValBool(pTseng->Options, OPTION_W32_INTERLEAVE, &pTseng->W32Interleave)) )
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Forcing W32p memory interleave %s.\n",
	    pTseng->W32Interleave ? "ON" : "OFF");
    if ((pTseng->SetPCIBurst = 
	xf86GetOptValBool(pTseng->Options, OPTION_PCI_BURST, &pTseng->PCIBurst)) )
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Forcing PCI burst mode %s.\n",
	    pTseng->PCIBurst ? "ON" : "OFF");

    pTseng->ShowCache = FALSE;
    if (xf86ReturnOptValBool(pTseng->Options, OPTION_SHOWCACHE, FALSE)) {
	pTseng->ShowCache = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "(for debugging only:) Visible off-screen memory\n");
    }

    pTseng->UsePCIRetry = FALSE;
    if (xf86ReturnOptValBool(pTseng->Options, OPTION_PCI_RETRY, FALSE)) {
	pTseng->UsePCIRetry = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "PCI retry enabled\n");
    }
    return TRUE;
}

static Bool
TsengGetFbAddress(ScrnInfoPtr pScrn)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    PDEBUG("	TsengGetFbAddress\n");

    /* base0 is the framebuffer and base1 is the PCI IO space. */
    if (!PCI_REGION_BASE(pTseng->PciInfo, 0, REGION_MEM)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "No valid Framebuffer address in PCI config space;\n");
        return FALSE;
    } else
        pTseng->FbAddress = PCI_REGION_BASE(pTseng->PciInfo, 0, REGION_MEM);


#ifndef XSERVER_LIBPCIACCESS
    if (xf86RegisterResources(pTseng->pEnt->index,NULL,ResNone)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Cannot register FB memory.\n");
        return FALSE;
    }
#endif

    /* The W32 linear map address space is always 4Mb (mainly because the
     * memory-mapped registers are located near the top of the 4MB area). 
     * The ET6000 maps out 16 Meg, but still uses only 4Mb of that. 
     * However, since all mmap()-ed space is also reflected in the "ps"
     * listing for the Xserver, many users will be worried by a server that
     * always eats 16MB of memory, even if it's not "real" memory, just
     * address space. Not mapping all of the 16M may be a potential problem
     * though: if another board is mapped on top of the remaining part of
     * the 16M... Boom!
     */
    if (pTseng->ChipType == ET6000)
	pTseng->FbMapSize = 16384 * 1024;
    else
	pTseng->FbMapSize = 4096 * 1024;

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Framebuffer at 0x%lX\n",
               (unsigned long)pTseng->FbAddress);

    return TRUE;
}

static Bool
TsengPreInit(ScrnInfoPtr pScrn, int flags)
{
    TsengPtr pTseng;
    MessageType from;
    int i;

    if (flags & PROBE_DETECT) return FALSE;

    PDEBUG("	TsengPreInit\n");
    
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

    /* The vgahw module should be loaded here when needed */
    
    /* This driver doesn't expect more than one entity per screen */
    if (pScrn->numEntities > 1) 
	    return FALSE;

    /* Allocate the TsengRec driverPrivate */
    if (!TsengGetRec(pScrn)) {
	return FALSE;
    }
    pTseng = TsengPTR(pScrn);

    /* This is the general case */
    pTseng->pEnt = xf86GetEntityInfo(*pScrn->entityList);

#if 1
    if (xf86LoadSubModule(pScrn, "int10")) {
 	xf86Int10InfoPtr pInt;
#if 1
	xf86DrvMsg(pScrn->scrnIndex,X_INFO,"initializing int10\n");
	pInt = xf86InitInt10(pTseng->pEnt->index);
	xf86FreeInt10(pInt);
#endif
    }
#endif
    
    if (!xf86LoadSubModule(pScrn, "vgahw"))
	return FALSE;
    /*
     * Allocate a vgaHWRec
     */
    if (!vgaHWGetHWRec(pScrn))
	return FALSE;

    vgaHWGetIOBase(VGAHWPTR(pScrn));
    /*
     * Since, the capabilities are determined by the chipset, the very first
     * thing to do is to figure out the chipset and its capabilities.
     */

    TsengUnlock(pScrn);

    pTseng->PciInfo = xf86GetPciInfoForEntity(pTseng->pEnt->index);
    if (!TsengPreInitPCI(pScrn)) {
        TsengFreeRec(pScrn);
        return FALSE;
    }

    if (!TsengRAMDACProbe(pScrn)) {
        TsengFreeRec(pScrn);
	return FALSE;
    }

    pScrn->progClock = TRUE;

    /*
     * Now we can check what depth we support.
     */

    /* Set pScrn->monitor */
    pScrn->monitor = pScrn->confScreen->monitor;

    /*
     * The first thing we should figure out is the depth, bpp, etc.
     * Our default depth is 8, so pass it to the helper function.
     * Our preference for depth 24 is 24bpp, so tell it that too.
     */
    if (!xf86SetDepthBpp(pScrn, 8, 8, 8, Support24bppFb | Support32bppFb |
                         SupportConvert32to24 | PreferConvert32to24)) {
	return FALSE;
    } else {
        switch (pScrn->depth) {
        case 8:
        case 16:
        case 24:
        case 32:
            /* OK */
            break;
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Given depth (%d) is not supported by this driver\n",
                       pScrn->depth);
            return FALSE;
        }
    }
    xf86PrintDepthBpp(pScrn);

    /* Get the depth24 pixmap format */
    if (pScrn->depth == 24 && pix24bpp == 0)
	pix24bpp = xf86GetBppFromDepth(pScrn, 24);

    if (pScrn->bitsPerPixel > 8)
	pTseng->Bytesperpixel = pScrn->bitsPerPixel / 8;
    else
	pTseng->Bytesperpixel = 1;  /* this is fake for < 8bpp, but simplifies other code */

    /* hardware limits */
    pScrn->maxHValue = Tseng_HMAX;
    pScrn->maxVValue = Tseng_VMAX;

    /*
     * This must happen after pScrn->display has been set because
     * xf86SetWeight references it.
     */

    /* Set weight/mask/offset for depth > 8 */
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
    if (!xf86SetDefaultVisual(pScrn, -1)) 
	return FALSE;

    /* The gamma fields must be initialised when using the new cmap code */
    if (pScrn->depth > 1) {
	Gamma zeros = {0.0, 0.0, 0.0};

	if (!xf86SetGamma(pScrn, zeros)) {
	    return FALSE;
	}
    }

    /* Set the bits per RGB for 8bpp mode */
    if (pScrn->depth == 8) {
	/* Default to 6, because most Tseng chips/RAMDACs don't support it */
	pScrn->rgbBits = 6;
    }
    if (!TsengProcessOptions(pScrn))   /* must be done _after_ we know what chip this is */
	return FALSE;

    if (!TsengGetFbAddress(pScrn))
        return FALSE;

    pScrn->memPhysBase = pTseng->FbAddress;
    pScrn->fbOffset = 0;

    if (pTseng->UseAccel)
	VGAHWPTR(pScrn)->MapSize = 0x20000;  /* accelerator apertures and MMIO */
    else
	VGAHWPTR(pScrn)->MapSize = 0x10000;

#ifndef XSERVER_LIBPCIACCESS
    /*
     * XXX At least part of this range does appear to be disabled,
     * but to play safe, it is marked as "unused" for now.
     * Changed this to "disable". Otherwise it might interfere with DGA.
     */
    xf86SetOperatingState(resVgaMem, pTseng->pEnt->index, ResDisableOpr);
#endif
    /* hibit processing (TsengProcessOptions() must have been called first) */
    pTseng->save_divide = 0x40;	       /* default */
    if (pTseng->ChipType == ET4000) {
	if (!TsengProcessHibit(pScrn))
	    return FALSE;
    }
    /*
     * If the user has specified the amount of memory in the XF86Config
     * file, we respect that setting.
     */
    if (pTseng->pEnt->device->videoRam != 0) {
	pScrn->videoRam = pTseng->pEnt->device->videoRam;
	from = X_CONFIG;
    } else {
	from = X_PROBED;
	pScrn->videoRam = TsengDetectMem(pScrn);
    }
    pScrn->videoRam = TsengLimitMem(pScrn, pScrn->videoRam);

    xf86DrvMsg(pScrn->scrnIndex, from, "VideoRAM: %d kByte.\n",
               pScrn->videoRam);

     TsengSetupClockRange(pScrn);
    
    /*
     * xf86ValidateModes will check that the mode HTotal and VTotal values
     * don't exceed the chipset's limit if pScrn->maxHValue and
     * pScrn->maxVValue are set.  Since our TsengValidMode() already takes
     * care of this, we don't worry about setting them here.
     */

    /* Select valid modes from those available */
    i = xf86ValidateModes(pScrn, pScrn->monitor->Modes,
	pScrn->display->modes, &pTseng->clockRange,
	NULL, 32, pScrn->maxHValue, 8*pTseng->Bytesperpixel, /* H limits */
	0, pScrn->maxVValue,	       /* V limits */
	pScrn->display->virtualX,
	pScrn->display->virtualY,
	pTseng->FbMapSize,
	LOOKUP_BEST_REFRESH);	       /* LOOKUP_CLOSEST_CLOCK | LOOKUP_CLKDIV2 when no programmable clock ? */

    if (i == -1) {
	TsengFreeRec(pScrn);
	return FALSE;
    }
    /* Prune the modes marked as invalid */
    xf86PruneDriverModes(pScrn);

    if (i == 0 || pScrn->modes == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
	TsengFreeRec(pScrn);
	return FALSE;
    }
    /*
     * Set the CRTC parameters for all of the modes based on the type
     * of mode, and the chipset's interlace requirements.
     *
     * Calling this is required if the mode->Crtc* values are used by the
     * driver and if the driver doesn't provide code to set them.  They
     * are not pre-initialised at all.
     */
    xf86SetCrtcForModes(pScrn, 0);

    /* Set the current mode to the first in the list */
    pScrn->currentMode = pScrn->modes;

    /* Print the list of modes being used */
    xf86PrintModes(pScrn);

    /* Set display resolution */
    xf86SetDpi(pScrn, 0, 0);

    /* Load bpp-specific modules */
    switch (pScrn->bitsPerPixel) {
    case 1:
	if (xf86LoadSubModule(pScrn, "xf1bpp") == NULL) {
	  TsengFreeRec(pScrn);
	  return FALSE;
	}
	break;
    case 4:
	if (xf86LoadSubModule(pScrn, "xf4bpp") == NULL) {
	  TsengFreeRec(pScrn);
	  return FALSE;
	}
	break;
    default:
	if (xf86LoadSubModule(pScrn, "fb") == NULL) {
	  TsengFreeRec(pScrn);
	  return FALSE;
	}
	break;
    }

    /* Load XAA if needed */
    if (pTseng->UseAccel) {
	if (!xf86LoadSubModule(pScrn, "xaa")) {
	    TsengFreeRec(pScrn);
	    return FALSE;
	}
    }
    /* Load ramdac if needed */
    if (pTseng->HWCursor) {
	if (!xf86LoadSubModule(pScrn, "ramdac")) {
	    TsengFreeRec(pScrn);
	    return FALSE;
	}
    }
/*    TsengLock(pScrn); */

    return TRUE;
}

static void 
TsengSetupAccelMemory(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    TsengPtr pTseng = TsengPTR(pScrn);
    int offscreen_videoram, videoram_end, req_videoram;
    int i;
    int v;

    /* XXX Hack to suppress messages in subsequent generations. */
    if (serverGeneration == 1)
	v = 1;
    else
	v = 100;
    /*
     * The accelerator requires free off-screen video memory to operate. The
     * more there is, the more it can accelerate.
     */

    videoram_end = pScrn->videoRam * 1024;
    offscreen_videoram = videoram_end -
	pScrn->displayWidth * pScrn->virtualY * pTseng->Bytesperpixel;
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, v, "Available off-screen memory: %d bytes.\n",
	offscreen_videoram);

    /*
     * The HW cursor requires 1kb of off-screen memory, aligned to 1kb
     * (256 DWORDS). Setting up its memory first ensures the alignment.
     */
    if (pTseng->HWCursor) {
	req_videoram = 1024;
	if (offscreen_videoram < req_videoram) {
	    xf86DrvMsgVerb(pScrn->scrnIndex, X_WARNING, v,
		"Hardware Cursor disabled. It requires %d bytes of free video memory\n",
		req_videoram);
	    pTseng->HWCursor = FALSE;
	    pTseng->HWCursorBufferOffset = 0;
	} else {
	    offscreen_videoram -= req_videoram;
	    videoram_end -= req_videoram;
	    pTseng->HWCursorBufferOffset = videoram_end;
	}
    } else {
	pTseng->HWCursorBufferOffset = 0;
    }

    /*
     * Acceleration memory setup. Do this only if acceleration is enabled.
     */
    if (!pTseng->UseAccel) return;

    /*
     * Basic acceleration needs storage for FG, BG and PAT colors in
     * off-screen memory. Each color requires 2(ping-pong)*8 bytes.
     */
    req_videoram = 2 * 8 * 3;
    if (offscreen_videoram < req_videoram) {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_WARNING, v,
	    "Acceleration disabled. It requires AT LEAST %d bytes of free video memory\n",
	    req_videoram);
	pTseng->UseAccel = FALSE;
	pTseng->AccelColorBufferOffset = 0;
	goto end_memsetup;	      /* no basic acceleration means none at all */
    } else {
	offscreen_videoram -= req_videoram;
	videoram_end -= req_videoram;
	pTseng->AccelColorBufferOffset = videoram_end;
    }

    /*
     * Color expansion (using triple buffering) requires 3 non-expanded
     * scanlines, DWORD padded.
     */
    req_videoram = 3 * ((pScrn->virtualX + 31) / 32) * 4;
    if (offscreen_videoram < req_videoram) {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_WARNING, v,
	    "Accelerated color expansion disabled (%d more bytes of free video memory required)\n",
	    req_videoram - offscreen_videoram);
	pTseng->AccelColorExpandBufferOffsets[0] = 0;
    } else {
	offscreen_videoram -= req_videoram;
	for (i = 0; i < 3; i++) {
	    videoram_end -= req_videoram / 3;
	    pTseng->AccelColorExpandBufferOffsets[i] = videoram_end;
	}
    }

    /*
     * XAA ImageWrite support needs two entire line buffers. The
     * current code assumes buffer 1 lies in the same 8kb aperture as
     * buffer 0.
     *
     * [ FIXME: aren't we forgetting the DWORD padding here ? ]
     * [ FIXME: why here double-buffering and in colexp triple-buffering? ]
     */
    req_videoram = 2 * (pScrn->virtualX * pTseng->Bytesperpixel);

    if (offscreen_videoram < req_videoram) {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_WARNING, v,
	    "Accelerated ImageWrites disabled (%d more bytes of free video memory required)\n",
	    req_videoram - offscreen_videoram);
	pTseng->AccelImageWriteBufferOffsets[0] = 0;
    } else {
	offscreen_videoram -= req_videoram;
	for (i = 0; i < 2; i++) {
	    videoram_end -= req_videoram / 2;
	    pTseng->AccelImageWriteBufferOffsets[i] = videoram_end;
	}
    }

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, v,
	"Remaining off-screen memory available for pixmap cache: %d bytes.\n",
	offscreen_videoram);

end_memsetup:
    pScrn->videoRam = videoram_end / 1024;
}

static Bool
TsengScreenInit(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr pScrn;
    TsengPtr pTseng;
    int ret;
    VisualPtr visual;
    
    PDEBUG("	TsengScreenInit\n");

    /* 
     * First get the ScrnInfoRec
     */
    pScrn = xf86ScreenToScrn(pScreen);

    pTseng = TsengPTR(pScrn);
    /* Map the Tseng memory areas */
    if (!TsengMapMem(pScrn))
	return FALSE;

    /* Save the current state */
    TsengSave(pScrn);

    /* Initialise the first mode */
    TsengModeInit(pScrn, pScrn->currentMode);

    /* Darken the screen for aesthetic reasons and set the viewport */
    TsengSaveScreen(pScreen, SCREEN_SAVER_ON);

    TsengAdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));
    /* XXX Fill the screen with black */

    /*
     * Reset visual list.
     */
    miClearVisualTypes();

    /* Setup the visuals we support. */

    /*
     * For bpp > 8, the default visuals are not acceptable because we only
     * support TrueColor and not DirectColor.  To deal with this, call
     * miSetVisualTypes for each visual supported.
     */
    if (!miSetVisualTypes(pScrn->depth, miGetDefaultVisualMask(pScrn->depth), 
			  pScrn->rgbBits, pScrn->defaultVisual))
      return FALSE;

    miSetPixmapDepths ();

    /*
     * Call the framebuffer layer's ScreenInit function, and fill in other
     * pScreen fields.
     */

    switch (pScrn->bitsPerPixel) {
#if HAVE_XF1BPP
    case 1:
	ret = xf1bppScreenInit(pScreen, pTseng->FbBase,
			pScrn->virtualX, pScrn->virtualY,
			pScrn->xDpi, pScrn->yDpi,
			pScrn->displayWidth);
	break;
#endif
#if HAVE_XF4BPP
    case 4:
	ret = xf4bppScreenInit(pScreen, pTseng->FbBase,
			pScrn->virtualX, pScrn->virtualY,
			pScrn->xDpi, pScrn->yDpi,
			pScrn->displayWidth);
	break;
#endif
    default:
        ret  = fbScreenInit(pScreen, pTseng->FbBase,
			pScrn->virtualX, pScrn->virtualY,
			pScrn->xDpi, pScrn->yDpi,
			pScrn->displayWidth, pScrn->bitsPerPixel);
	break;
    }

    if (!ret)
	return FALSE;

    xf86SetBlackWhitePixels(pScreen);

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

#if HAVE_XF1BPP
    /* must be after RGB ordering fixed */
    if (pScrn->bitsPerPixel > 4)
#endif
	fbPictureInit(pScreen, 0, 0);

    if (pScrn->depth >= 8)
        TsengDGAInit(pScreen);

    /*
     * Initialize the acceleration interface.
     */
    TsengSetupAccelMemory(pScreen);
    if (pTseng->UseAccel) {
	tseng_init_acl(pScrn);	/* set up accelerator */
	if (!TsengXAAInit(pScreen)) {	/* set up XAA interface */
	    return FALSE;
	}
    }

    xf86SetSilkenMouse(pScreen);
    /* Initialise cursor functions */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    /* Hardware Cursor layer */
    if (pTseng->HWCursor) {
	if (!TsengHWCursorInit(pScreen))
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"Hardware cursor initialization failed\n");
    }

    /* Initialise default colourmap */
    if (!miCreateDefColormap(pScreen))
	return FALSE;

    if (pScrn->depth == 4 || pScrn->depth == 8) { /* fb and xf4bpp */
	vgaHWHandleColormaps(pScreen);
    }
#ifndef XSERVER_LIBPCIACCESS
    pScrn->racIoFlags = RAC_FB | RAC_COLORMAP | RAC_CURSOR | RAC_VIEWPORT;
    pScrn->racMemFlags = pScrn->racIoFlags;
#endif
    /* Wrap the current CloseScreen and SaveScreen functions */
    pScreen->SaveScreen = TsengSaveScreen;

    /* Support for DPMS, the ET4000W32Pc and newer uses a different and
     * simpler method than the older cards.
     */
    if ((pTseng->ChipType == ET4000) &&
        ((pTseng->ChipRev == REV_A) || (pTseng->ChipRev == REV_B)))
        xf86DPMSInit(pScreen, (DPMSSetProcPtr)TsengHVSyncDPMSSet, 0);
    else
	xf86DPMSInit(pScreen, (DPMSSetProcPtr)TsengCrtcDPMSSet, 0);

    pTseng->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = TsengCloseScreen;

    /* Report any unused options (only for the first generation) */
    if (serverGeneration == 1) {
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);
    }
    /* Done */
    return TRUE;
}

static Bool
TsengEnterVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    TsengPtr pTseng = TsengPTR(pScrn);

    PDEBUG("	TsengEnterVT\n");

    vgaHWUnlock(VGAHWPTR(pScrn));
    TsengUnlock(pScrn);

    if (!TsengModeInit(pScrn, pScrn->currentMode))
        return FALSE;
    if (pTseng->UseAccel) {
	tseng_init_acl(pScrn);	/* set up accelerator */
    }
    return TRUE;
}

static void
TsengLeaveVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    TsengPtr pTseng = TsengPTR(pScrn);

    PDEBUG("	TsengLeaveVT\n");
    TsengRestore(pScrn, &(VGAHWPTR(pScrn)->SavedReg),
		 &pTseng->SavedReg,VGA_SR_ALL);

    TsengLock(pScrn);
    vgaHWLock(VGAHWPTR(pScrn));
}

static Bool
TsengCloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    TsengPtr pTseng = TsengPTR(pScrn);

    PDEBUG("	TsengCloseScreen\n");

    if (pScrn->vtSema) {
    TsengRestore(pScrn, &(VGAHWPTR(pScrn)->SavedReg),
		 &(pTseng->SavedReg),VGA_SR_ALL);
    TsengUnmapMem(pScrn);
    }
#ifdef HAVE_XAA_H
    if (pTseng->AccelInfoRec)
	XAADestroyInfoRec(pTseng->AccelInfoRec);
#endif
    if (pTseng->CursorInfoRec)
	xf86DestroyCursorInfoRec(pTseng->CursorInfoRec);

    pScrn->vtSema = FALSE;

    pScreen->CloseScreen = pTseng->CloseScreen;
    return (*pScreen->CloseScreen) (CLOSE_SCREEN_ARGS);
}

/*
 * SaveScreen --
 *
 *   perform a sequencer reset.
 *
 * The ET4000 "Video System Configuration 1" register (CRTC index 0x36),
 * which is used to set linear memory mode and MMU-related stuff, is
 * partially reset to "0" when TS register index 0 bit 1 is set (synchronous
 * reset): bits 3..5 are reset during a sync. reset.
 *
 * We therefor do _not_ call vgaHWSaveScreen here, since it does a sequencer
 * reset. Instead, we do the same as in vgaHWSaveScreen except for the seq. reset.
 *
 * If this is not done, the higher level code will not be able to access the
 * framebuffer (because it is temporarily in banked mode instead of linear
 * mode) as long as SaveScreen is active (=in between a
 * SaveScreen(FALSE)/SaveScreen(TRUE) pair)
 */

static Bool
TsengSaveScreen(ScreenPtr pScreen, int mode)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    TsengPtr pTseng = TsengPTR(pScrn);
    Bool unblank;

    PDEBUG("	TsengSaveScreen\n");

    unblank = xf86IsUnblank(mode);

    if (pTseng->ChipType == ET6000) {
	return vgaHWSaveScreen(pScreen, unblank);
    } else {
       if (unblank)
	  SetTimeSinceLastInputEvent();

       if (pScrn->vtSema) {
           /* vgaHWBlankScreen without seq reset */
           CARD8 scrn;

           scrn = hwp->readSeq(hwp, 0x01);
           
           if (unblank)
               scrn &= 0xDF; /* enable screen */
           else
               scrn |= 0x20; /* blank screen */

           hwp->writeSeq(hwp, 0x01, scrn); /* change mode */
       }
       return (TRUE);
    }
}

static Bool
TsengMapMem(ScrnInfoPtr pScrn)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    PDEBUG("	TsengMapMem\n");

    /* Map the VGA memory */

    if (!vgaHWMapMem(pScrn)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	    "Could not mmap standard VGA memory aperture.\n");
	return FALSE;
    }

#ifndef XSERVER_LIBPCIACCESS
    pTseng->FbBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_FRAMEBUFFER,
                                   pTseng->PciTag,
                                   (unsigned long)pTseng->FbAddress,
                                   pTseng->FbMapSize);
#else
    {
      void** result = (void**)&pTseng->FbBase;
      int err = pci_device_map_range(pTseng->PciInfo,
				     pTseng->FbAddress,
				     pTseng->FbMapSize,
				     PCI_DEV_MAP_FLAG_WRITABLE |
				     PCI_DEV_MAP_FLAG_WRITE_COMBINE,
				     result);
      
      if (err) 
	return FALSE;
    }
#endif
    if (pTseng->FbBase == NULL) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Could not mmap linear video memory.\n");
        return FALSE;
    }

    /* need some sanity here */
    if (pTseng->UseAccel) {
#ifndef XSERVER_LIBPCIACCESS
        pTseng->MMioBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO,
                                         pTseng->PciTag,
                                         (unsigned long)pTseng->FbAddress,
                                         pTseng->FbMapSize);
#else
	pTseng->MMioBase = pTseng->FbBase;
#endif
        if (!pTseng->MMioBase) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Could not mmap mmio memory.\n");
	    return FALSE;
        }
        pTseng->MMioBase += 0x3FFF00L;
    }
    
    if (pTseng->FbBase == NULL)
	return FALSE;

    return TRUE;
}

static Bool
TsengUnmapMem(ScrnInfoPtr pScrn)
{
    TsengPtr pTseng = TsengPTR(pScrn);

    PDEBUG("	TsengUnmapMem\n");

#ifndef XSERVER_LIBPCIACCESS
    xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pTseng->FbBase, pTseng->FbMapSize);
#else
    pci_device_unmap_range(pTseng->PciInfo, pTseng->FbBase, pTseng->FbMapSize);
#endif

    vgaHWUnmapMem(pScrn);

    pTseng->FbBase = NULL;

    return TRUE;
}

static void
TsengFreeScreen(FREE_SCREEN_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    PDEBUG("	TsengFreeScreen\n");
    if (xf86LoaderCheckSymbol("vgaHWFreeHWRec"))
        vgaHWFreeHWRec(pScrn);
    TsengFreeRec(pScrn);
}

static Bool
TsengSwitchMode(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    PDEBUG("	TsengSwitchMode\n");
    return TsengModeInit(pScrn, mode);
}
