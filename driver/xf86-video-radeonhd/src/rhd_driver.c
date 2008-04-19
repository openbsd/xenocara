/*
 * Copyright 2007  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007  Egbert Eich   <eich@novell.com>
 * Copyright 2007  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef ATOM_BIOS_PARSER
# define ATOM_ASIC_INIT
#endif

#define MODULEVENDORSTRING "AMD GPG"  /* @@@ */
#include "xf86.h"
#include "xf86_OSproc.h"

/* For PIO/MMIO */
#include "compiler.h"

#include "xf86Resources.h"

#include "xf86PciInfo.h"
/* do we need to access PCI config space directly? */
#include "xf86Pci.h"

/* This is used for module versioning */
#include "xf86Version.h"

/* Memory manager */
#include "xf86fbman.h"

/* For SW cursor */
#include "mipointer.h"

/* For HW cursor */
#include "xf86Cursor.h"

/* optional backing store */
#include "mibstore.h"

/* mi colormap manipulation */
#include "micmap.h"

#include "xf86cmap.h"

#include "fb.h"

/* Needed by Resources Access Control (RAC) */
#include "xf86RAC.h"

#define DPMS_SERVER
#include "X11/extensions/dpms.h"

/* int10 - for now at least */
#include "xf86int10.h"
#include "vbe.h"

/* Needed for Device Data Channel (DDC) support */
#include "xf86DDC.h"

#include "picturestr.h"

#if HAVE_XF86_ANSIC_H
# include "xf86_ansic.h"
#else
# include <sys/types.h>
# include <sys/stat.h>
# include <string.h>
# include <unistd.h>
# define stat_t struct stat
#endif

/*
 * Driver data structures.
 */
#include "rhd.h"
#include "rhd_regs.h"
#include "rhd_cursor.h"
#include "rhd_atombios.h"
#include "rhd_connector.h"
#include "rhd_output.h"
#include "rhd_pll.h"
#include "rhd_vga.h"
#include "rhd_mc.h"
#include "rhd_monitor.h"
#include "rhd_crtc.h"
#include "rhd_modes.h"
#include "rhd_lut.h"
#include "rhd_i2c.h"
#include "rhd_shadow.h"
#include "rhd_card.h"
#include "rhd_randr.h"
#include "r5xx_accel.h"

/* ??? */
#include "servermd.h"

/* Mandatory functions */
static const OptionInfoRec *	RHDAvailableOptions(int chipid, int busid);
#ifdef XSERVER_LIBPCIACCESS
static Bool RHDPciProbe(DriverPtr drv, int entityNum,
			struct pci_device *dev, intptr_t matchData);
#else
static Bool     RHDProbe(DriverPtr drv, int flags);
#endif
static Bool     RHDPreInit(ScrnInfoPtr pScrn, int flags);
static Bool     RHDScreenInit(int Index, ScreenPtr pScreen, int argc,
                                  char **argv);
static Bool     RHDEnterVT(int scrnIndex, int flags);
static void     RHDLeaveVT(int scrnIndex, int flags);
static Bool     RHDCloseScreen(int scrnIndex, ScreenPtr pScreen);
static void     RHDFreeScreen(int scrnIndex, int flags);
static Bool     RHDSwitchMode(int scrnIndex, DisplayModePtr mode, int flags);
static void     RHDAdjustFrame(int scrnIndex, int x, int y, int flags);
static void     RHDDisplayPowerManagementSet(ScrnInfoPtr pScrn,
                                             int PowerManagementMode,
                                             int flags);
static void     RHDLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices,
                               LOCO *colors, VisualPtr pVisual);
static Bool     RHDSaveScreen(ScreenPtr pScrn, int on);

static void     rhdProcessOptions(ScrnInfoPtr pScrn);
static void     rhdSave(RHDPtr rhdPtr);
static void     rhdRestore(RHDPtr rhdPtr);
static Bool     rhdModeLayoutSelect(RHDPtr rhdPtr);
static void     rhdModeLayoutPrint(RHDPtr rhdPtr);
static void     rhdModeDPISet(ScrnInfoPtr pScrn);
static void	rhdPrepareMode(RHDPtr rhdPtr);
static void     rhdModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
static void	rhdSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode);
static Bool     rhdMapMMIO(RHDPtr rhdPtr);
static void     rhdUnmapMMIO(RHDPtr rhdPtr);
static Bool     rhdMapFB(RHDPtr rhdPtr);
static void     rhdUnmapFB(RHDPtr rhdPtr);
static CARD32   rhdGetVideoRamSize(RHDPtr rhdPtr);
static void     rhdFbOffscreenGrab(ScrnInfoPtr pScrn);
static void	rhdGetIGPNorthBridgeInfo(RHDPtr rhdPtr);

/* rhd_id.c */
extern SymTabRec RHDChipsets[];
extern PciChipsets RHDPCIchipsets[];
extern void RHDIdentify(int flags);
extern struct rhdCard *RHDCardIdentify(ScrnInfoPtr pScrn);
#ifdef XSERVER_LIBPCIACCESS
extern const struct pci_id_match RHDDeviceMatch[];
#endif

/* keep accross drivers */
static int pix24bpp = 0;

/* required for older X.Org releases */
#ifndef _X_EXPORT
#define _X_EXPORT
#endif

#ifdef __linux__
# define FGLRX_SYS_PATH "/sys/module/fglrx"
#endif

static const char *xaaSymbols[] = {
    "XAACreateInfoRec",
    "XAADestroyInfoRec",
    "XAAInit",
    NULL
};

#ifdef USE_EXA
static const char *exaSymbols[] = {
    "exaDriverAlloc",
    "exaDriverFini",
    "exaDriverInit",
    "exaGetPixmapOffset",
    "exaGetPixmapPitch",
    "exaMarkSync",
    "exaWaitSync",
    NULL
};
#endif /* USE_EXA */

_X_EXPORT DriverRec RADEONHD = {
    RHD_VERSION,
    RHD_DRIVER_NAME,
    RHDIdentify,
#ifdef XSERVER_LIBPCIACCESS
    NULL,
#else
    RHDProbe,
#endif
    RHDAvailableOptions,
    NULL,
    0,
    NULL,
#ifdef XSERVER_LIBPCIACCESS
    RHDDeviceMatch,
    RHDPciProbe
#endif
};

typedef enum {
    OPTION_NOACCEL,
    OPTION_ACCELMETHOD,
    OPTION_OFFSCREENSIZE,
    OPTION_SW_CURSOR,
    OPTION_IGNORECONNECTOR,
    OPTION_FORCEREDUCED,
    OPTION_FORCEDPI,
    OPTION_USECONFIGUREDMONITOR,
    OPTION_HPD,
    OPTION_NORANDR,
    OPTION_RRUSEXF86EDID,
    OPTION_RROUTPUTORDER,
    OPTION_TV_MODE
} RHDOpts;

static const OptionInfoRec RHDOptions[] = {
    { OPTION_NOACCEL,              "NoAccel",              OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_ACCELMETHOD,          "AccelMethod",          OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_OFFSCREENSIZE,        "offscreensize",        OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_SW_CURSOR,            "SWcursor",             OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_IGNORECONNECTOR,      "ignoreconnector",      OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_FORCEREDUCED,         "forcereduced",         OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_FORCEDPI,             "forcedpi",             OPTV_INTEGER, {0}, FALSE },
    { OPTION_USECONFIGUREDMONITOR, "useconfiguredmonitor", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_HPD,                  "HPD",                  OPTV_STRING,  {0}, FALSE },
    { OPTION_NORANDR,              "NoRandr",              OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_RRUSEXF86EDID,        "RRUseXF86Edid",        OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_RROUTPUTORDER,        "RROutputOrder",        OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_TV_MODE,		   "TVMode",	           OPTV_ANYSTR,  {0}, FALSE },
    { -1, NULL, OPTV_NONE,	{0}, FALSE }
};

static MODULESETUPPROTO(rhdSetup);

static XF86ModuleVersionInfo rhdVersRec =
{
	"radeonhd",
	MODULEVENDORSTRING,
	MODINFOSTRING1,
	MODINFOSTRING2,
	XORG_VERSION_CURRENT,
	RHD_MAJOR_VERSION, RHD_MINOR_VERSION, RHD_PATCHLEVEL,
	ABI_CLASS_VIDEODRV,
	ABI_VIDEODRV_VERSION,
	MOD_CLASS_VIDEODRV,
	{0,0,0,0}
};

_X_EXPORT XF86ModuleData radeonhdModuleData = { &rhdVersRec, rhdSetup, NULL };

static pointer
rhdSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    static Bool setupDone = FALSE;

    if (!setupDone) {
	setupDone = TRUE;
        xf86AddDriver(&RADEONHD, module, HaveDriverFuncs);
#if 0  /* @@@ */
	LoaderRefSymLists(NULL);
#endif
        /* return non-NULL even with no teardown */
  	return (pointer)1;
    } else {
	if (errmaj) *errmaj = LDR_ONCEONLY;
	return NULL;
    }
}

static Bool
RHDGetRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate != NULL)
	return TRUE;

    pScrn->driverPrivate = xnfcalloc(sizeof(RHDRec), 1);

    if (pScrn->driverPrivate == NULL)
	return FALSE;

    RHDPTR(pScrn)->scrnIndex = pScrn->scrnIndex;

    return TRUE;
}

static void
RHDFreeRec(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr;

    if (pScrn->driverPrivate == NULL)
	return;

    rhdPtr = RHDPTR(pScrn);

    xfree(rhdPtr->Options);

    RHDMCDestroy(rhdPtr);
    RHDVGADestroy(rhdPtr);
    RHDPLLsDestroy(rhdPtr);
    RHDLUTsDestroy(rhdPtr);
    RHDOutputsDestroy(rhdPtr);
    RHDConnectorsDestroy(rhdPtr);
    RHDCursorsDestroy(rhdPtr);
    RHDCrtcsDestroy(rhdPtr);
    RHDI2CFunc(pScrn->scrnIndex, rhdPtr->I2C, RHD_I2C_TEARDOWN, NULL);
#ifdef ATOM_BIOS
    RHDAtomBiosFunc(pScrn->scrnIndex, rhdPtr->atomBIOS,
		    ATOMBIOS_TEARDOWN, NULL);
#endif
    RHDShadowDestroy(rhdPtr);
    if (rhdPtr->CursorInfo)
        xf86DestroyCursorInfoRec(rhdPtr->CursorInfo);

    xfree(pScrn->driverPrivate);	/* == rhdPtr */
    pScrn->driverPrivate = NULL;
}

static const OptionInfoRec *
RHDAvailableOptions(int chipid, int busid)
{
    return RHDOptions;
}

/*
 *
 */
#ifdef XSERVER_LIBPCIACCESS
static Bool
RHDPciProbe(DriverPtr drv, int entityNum,
	    struct pci_device *dev, intptr_t matchData)
{
    ScrnInfoPtr pScrn;
    RHDPtr rhdPtr;

    pScrn = xf86ConfigPciEntity(NULL, 0, entityNum, NULL,
				RES_SHARED_VGA, NULL, NULL, NULL, NULL);
    if (pScrn != NULL) {

	pScrn->driverVersion = RHD_VERSION;
	pScrn->driverName    = RHD_DRIVER_NAME;
	pScrn->name          = RHD_NAME;
	pScrn->Probe         = NULL;
	pScrn->PreInit       = RHDPreInit;
	pScrn->ScreenInit    = RHDScreenInit;
	pScrn->SwitchMode    = RHDSwitchMode;
	pScrn->AdjustFrame   = RHDAdjustFrame;
	pScrn->EnterVT       = RHDEnterVT;
	pScrn->LeaveVT       = RHDLeaveVT;
	pScrn->FreeScreen    = RHDFreeScreen;
	pScrn->ValidMode     = NULL; /* we do our own validation */

	if (!RHDGetRec(pScrn))
	    return FALSE;

	rhdPtr = RHDPTR(pScrn);

	rhdPtr->PciInfo = dev;
	rhdPtr->ChipSet = matchData;
    }

    return (pScrn != NULL);
}

#else

static Bool
RHDProbe(DriverPtr drv, int flags)
{
    Bool foundScreen = FALSE;
    int numDevSections, numUsed;
    GDevPtr *devSections;
    int *usedChips;
    int i;

    if ((numDevSections = xf86MatchDevice(RHD_DRIVER_NAME,
					  &devSections)) <= 0) {
	return FALSE;
    }

    /* PCI BUS */
    if (xf86GetPciVideoInfo() ) {
	numUsed = xf86MatchPciInstances(RHD_NAME, PCI_VENDOR_ATI,
					RHDChipsets, RHDPCIchipsets,
					devSections,numDevSections,
					drv, &usedChips);

	if (numUsed > 0) {
	    if (flags & PROBE_DETECT)
		foundScreen = TRUE;
	    else for (i = 0; i < numUsed; i++) {
		ScrnInfoPtr pScrn = NULL;

		if ((pScrn = xf86ConfigPciEntity(pScrn, 0, usedChips[i],
					      RHDPCIchipsets,NULL, NULL,
					      NULL, NULL, NULL))) {
		    pScrn->driverVersion = RHD_VERSION;
		    pScrn->driverName    = RHD_DRIVER_NAME;
		    pScrn->name          = RHD_NAME;
		    pScrn->Probe         = RHDProbe;
		    pScrn->PreInit       = RHDPreInit;
		    pScrn->ScreenInit    = RHDScreenInit;
		    pScrn->SwitchMode    = RHDSwitchMode;
		    pScrn->AdjustFrame   = RHDAdjustFrame;
		    pScrn->EnterVT       = RHDEnterVT;
		    pScrn->LeaveVT       = RHDLeaveVT;
		    pScrn->FreeScreen    = RHDFreeScreen;
		    pScrn->ValidMode     = NULL; /* we do our own validation */
		    foundScreen = TRUE;
		}
	    }
	    xfree(usedChips);
	}
    }

    xfree(devSections);
    return foundScreen;
}
#endif

/*
 *
 */
static Bool
RHDPreInit(ScrnInfoPtr pScrn, int flags)
{
    RHDPtr rhdPtr;
    EntityInfoPtr pEnt = NULL;
    Bool ret = FALSE;
    RHDI2CDataArg i2cArg;
    DisplayModePtr Modes;		/* Non-RandR-case only */
    stat_t statbuf;

    if (flags & PROBE_DETECT)  {
        /* do dynamic mode probing */
	return TRUE;
    }

#ifdef FGLRX_SYS_PATH
    /* check for fglrx kernel module */
    if (stat (FGLRX_SYS_PATH, &statbuf) == 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "The fglrx kernel module is loaded. This can have obvious\n"
		   "     or subtle side effects. See radeonhd(4) for details.\n");
    }
#endif

#ifndef XSERVER_LIBPCIACCESS
    /*
     * Allocate the RhdRec driverPrivate
     * (for the PCIACCESS case this is done in Probe already)
     */
    if (!RHDGetRec(pScrn)) {
	return FALSE;
    }
#endif
    rhdPtr = RHDPTR(pScrn);

    /* Get server verbosity level */
    rhdPtr->verbosity = xf86GetVerbosity();

    /* This driver doesn't expect more than one entity per screen */
    if (pScrn->numEntities > 1) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Driver doesn't support more than one entity per screen\n");
	goto error0;
    }
    /* @@@ move to Probe? */
    if (!(pEnt = xf86GetEntityInfo(pScrn->entityList[0]))) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Unable to get entity info\n");
	goto error0;
    }

    if (pEnt->resources) {
        xfree(pEnt);
	goto error0;
    }

    pScrn->videoRam = pEnt->device->videoRam;
    rhdPtr->entityIndex = pEnt->index;

#ifndef XSERVER_LIBPCIACCESS
    rhdPtr->ChipSet = pEnt->chipset;

    rhdPtr->PciInfo = xf86GetPciInfoForEntity(pEnt->index);
    rhdPtr->PciTag = pciTag(rhdPtr->PciInfo->bus,
                            rhdPtr->PciInfo->device,
                            rhdPtr->PciInfo->func);
/* #else:  RHDPciProbe() did this for us already */
#endif

    xfree(pEnt);

    if (RHDIsIGP(rhdPtr->ChipSet))
	rhdGetIGPNorthBridgeInfo(rhdPtr);

    pScrn->chipset = (char *)xf86TokenToString(RHDChipsets, rhdPtr->ChipSet);

    /* We will disable access to VGA legacy resources emulation and
       save/restore VGA thru MMIO when necessary */
    if (xf86RegisterResources(rhdPtr->entityIndex, NULL, ResNone))
	goto error0;

#ifndef  ATOM_ASIC_INIT
    if (xf86LoadSubModule(pScrn, "int10")) {
	xf86Int10InfoPtr Int10;
	xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Initializing INT10\n");
	if ((Int10 = xf86InitInt10(rhdPtr->entityIndex))) {
	    /*
	     * here we kludge to get a copy of V_BIOS for
	     * the AtomBIOS code. After POSTing a PCI BIOS
	     * is not accessible any more. On a non-primary
	     * card it's lost after we do xf86FreeInt10(),
	     * so we save it here before we kill int10.
	     * This still begs the question what to do
	     * on a non-primary card that has been POSTed
	     * by an earlier Xserver start.
	     */
	    if ((rhdPtr->BIOSCopy = xalloc(RHD_VBIOS_SIZE))) {
		(void)memcpy(rhdPtr->BIOSCopy,
			     xf86int10Addr(Int10, Int10->BIOSseg << 4),
			     RHD_VBIOS_SIZE);
	    }
	    xf86FreeInt10(Int10);
	}
    }
#endif

    /* xf86CollectOptions cluelessly depends on these and
       will SIGSEGV otherwise */
    pScrn->monitor = pScrn->confScreen->monitor;

    if (!xf86SetDepthBpp(pScrn, 24, 0, 0, Support32bppFb)) {
	goto error0;
    } else {
	/* Check that the returned depth is one we support */
	switch (pScrn->depth) {
	case 8:
	case 15:
	case 16:
	case 24:
	    break;
	default:
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Given depth (%d) is not supported by this driver\n",
		       pScrn->depth);
	    goto error0;
	}
    }
    xf86PrintDepthBpp(pScrn);

    rhdProcessOptions(pScrn);

    /* Now check whether we know this card */
    rhdPtr->Card = RHDCardIdentify(pScrn);
    if (rhdPtr->Card)
	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Detected an %s on a %s\n",
		   pScrn->chipset, rhdPtr->Card->name);
    else
	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Detected an %s on an "
		   "unidentified card\n", pScrn->chipset);
    if (rhdPtr->Card && rhdPtr->Card->flags & RHD_CARD_FLAG_HPDSWAP &&
	rhdPtr->hpdUsage == RHD_HPD_USAGE_AUTO)
	rhdPtr->hpdUsage = RHD_HPD_USAGE_AUTO_SWAP;
    if (rhdPtr->Card && rhdPtr->Card->flags & RHD_CARD_FLAG_HPDOFF &&
	rhdPtr->hpdUsage == RHD_HPD_USAGE_AUTO)
	rhdPtr->hpdUsage = RHD_HPD_USAGE_AUTO_OFF;

    /* We need access to IO space already */
    if (!rhdMapMMIO(rhdPtr)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to map MMIO.\n");
	goto error0;
    }

#ifdef ATOM_BIOS
    {
	AtomBiosArgRec atomBiosArg;

	if (RHDAtomBiosFunc(pScrn->scrnIndex, NULL, ATOMBIOS_INIT, &atomBiosArg)
	    == ATOM_SUCCESS) {
	    rhdPtr->atomBIOS = atomBiosArg.atomhandle;
	}
    }
#else
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	       "**************************************************\n");
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	       "** Code has been built without AtomBIOS support **\n");
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	       "** this may seriously affect the functionality ***\n");
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	       "**              of this driver                 ***\n");
    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	       "**************************************************\n");
#endif
    rhdPtr->tvMode = RHD_TV_NONE;
    {
	const struct { char *name; enum RHD_TV_MODE mode; }
	rhdTVModeMapName[] = {
	    {"NTSC", RHD_TV_NTSC},
	    {"NTSCJ", RHD_TV_NTSCJ},
	    {"PAL", RHD_TV_PAL},
	    {"PALM", RHD_TV_PALN},
	    {"PALCN", RHD_TV_PALCN},
	    {"PAL60", RHD_TV_PAL60},
	    {"SECAM", RHD_TV_SECAM},
	    {NULL, RHD_TV_NONE}
	};


	if (rhdPtr->tvModeName.set) {
	    int i = 0;

	    while (rhdTVModeMapName[i].name) {
		if (!strcmp(rhdTVModeMapName[i].name, rhdPtr->tvModeName.val.string)) {
		    rhdPtr->tvMode = rhdTVModeMapName[i].mode;
		    break;
		}
		i++;
	    }
	    if (rhdPtr->tvMode == RHD_TV_NONE) {
		xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
			   "Specified TV Mode %s is invalid\n", rhdPtr->tvModeName.val.string);
	    }
	}
#ifdef ATOM_BIOS
	if (rhdPtr->tvMode == RHD_TV_NONE) {
	    AtomBiosArgRec atomBiosArg;

	    int i = 0;

	    if (RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
				ATOM_ANALOG_TV_DEFAULT_MODE, &atomBiosArg)
		== ATOM_SUCCESS) {
		rhdPtr->tvMode = atomBiosArg.tvMode;
		while (rhdTVModeMapName[i].name) {
		    if (rhdTVModeMapName[i].mode == rhdPtr->tvMode) {
			xf86DrvMsg(rhdPtr->scrnIndex, X_INFO,
				   "Found default TV Mode %s\n",rhdTVModeMapName[i].name);
			break;
		    }
		    i++;
		}
	    }
	}
#endif
    }
    /* We can use a register which is programmed by the BIOS to find out the
       size of our framebuffer */
    if (!pScrn->videoRam) {
	pScrn->videoRam = rhdGetVideoRamSize(rhdPtr);
	if (!pScrn->videoRam) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No Video RAM detected.\n");
	    goto error1;
	}
    }
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "VideoRAM: %d kByte\n",
	       pScrn->videoRam);

    rhdPtr->FbFreeStart = 0;
    rhdPtr->FbFreeSize = pScrn->videoRam * 1024;

#ifdef ATOM_BIOS
    if (rhdPtr->atomBIOS) { 	/* for testing functions */

        AtomBiosArgRec atomBiosArg;

        atomBiosArg.fb.start = rhdPtr->FbFreeStart;
        atomBiosArg.fb.size = rhdPtr->FbFreeSize;
        if (RHDAtomBiosFunc(pScrn->scrnIndex, rhdPtr->atomBIOS, ATOMBIOS_ALLOCATE_FB_SCRATCH,
			  &atomBiosArg) == ATOM_SUCCESS) {
	    rhdPtr->FbFreeStart = atomBiosArg.fb.start;
	    rhdPtr->FbFreeSize = atomBiosArg.fb.size;
	}
	RHDAtomBiosFunc(pScrn->scrnIndex, rhdPtr->atomBIOS, GET_DEFAULT_ENGINE_CLOCK,
			&atomBiosArg);
	RHDAtomBiosFunc(pScrn->scrnIndex, rhdPtr->atomBIOS, GET_DEFAULT_MEMORY_CLOCK,
			&atomBiosArg);
	RHDAtomBiosFunc(pScrn->scrnIndex, rhdPtr->atomBIOS,
			GET_MAX_PIXEL_CLOCK_PLL_OUTPUT, &atomBiosArg);
	RHDAtomBiosFunc(pScrn->scrnIndex, rhdPtr->atomBIOS,
			GET_MIN_PIXEL_CLOCK_PLL_OUTPUT, &atomBiosArg);
	RHDAtomBiosFunc(pScrn->scrnIndex, rhdPtr->atomBIOS,
			GET_MAX_PIXEL_CLOCK_PLL_INPUT, &atomBiosArg);
	RHDAtomBiosFunc(pScrn->scrnIndex, rhdPtr->atomBIOS,
			    GET_MIN_PIXEL_CLOCK_PLL_INPUT, &atomBiosArg);
	RHDAtomBiosFunc(pScrn->scrnIndex, rhdPtr->atomBIOS,
			    GET_MAX_PIXEL_CLK, &atomBiosArg);
	RHDAtomBiosFunc(pScrn->scrnIndex, rhdPtr->atomBIOS,
			GET_REF_CLOCK, &atomBiosArg);
    }
#endif

    if (xf86LoadSubModule(pScrn, "i2c")) {
	if (RHDI2CFunc(pScrn->scrnIndex, NULL, RHD_I2C_INIT, &i2cArg)
	    == RHD_I2C_SUCCESS) {
	    rhdPtr->I2C = i2cArg.I2CBusList;

	    if (!xf86LoadSubModule(pScrn, "ddc")) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "%s: Failed to load DDC module\n",__func__);
		goto error1;
	    }
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "I2C init failed\n");
	    goto error1;
	}
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "%s: Failed to load I2C module\n",__func__);
	goto error1;
    }

    /* Init modesetting structures */
    RHDVGAInit(rhdPtr);
    RHDMCInit(rhdPtr);
    RHDCrtcsInit(rhdPtr);
    RHDPLLsInit(rhdPtr);
    RHDLUTsInit(rhdPtr);
    RHDCursorsInit(rhdPtr); /* do this irrespective of hw/sw cursor setting */

    if (!RHDConnectorsInit(rhdPtr, rhdPtr->Card)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Card information has invalid connector information\n");
	goto error1;
    }

    RHDRandrPreInit(pScrn);

    if (! rhdPtr->randr) {
	/* Pick anything for now */
	if (!rhdModeLayoutSelect(rhdPtr)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Failed to detect a connected monitor\n");
	    goto error1;
	}

	/* set up rhdPtr->ConfigMonitor */
	if (!xf86GetOptValBool(rhdPtr->Options, OPTION_USECONFIGUREDMONITOR, &ret))
	    ret = FALSE;
	RHDConfigMonitorSet(pScrn->scrnIndex, ret);

	rhdModeLayoutPrint(rhdPtr);
    }

    /* @@@ rgb bits boilerplate */
    if (pScrn->depth == 8)
	pScrn->rgbBits = 8;

    /* Get the depth24 pixmap format */
    if (pScrn->depth == 24 && pix24bpp == 0)
	pix24bpp = xf86GetBppFromDepth(pScrn, 24);

    /*
     * This must happen after pScrn->display has been set because
     * xf86SetWeight references it.
     */
    if (pScrn->depth > 8) {
	/* The defaults are OK for us */
	rgb zeros = {0, 0, 0};

	if (!xf86SetWeight(pScrn, zeros, zeros)) {
	    goto error1;
	} else {
	    /* XXX check that weight returned is supported */
	    ;
	}
    }

    if (!xf86SetDefaultVisual(pScrn, -1)) {
	goto error1;
    } else {
        /* We don't currently support DirectColor at > 8bpp */
        if (pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Given default visual"
                       " (%s) is not supported at depth %d\n",
                       xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
	    goto error1;
        }
    }

    if (pScrn->depth > 1) {
	Gamma zeros = {0.0, 0.0, 0.0};

        /* @@@ */
	if (!xf86SetGamma(pScrn, zeros)) {
	    goto error1;
	}
    }

    /* @@@ need this? */
    pScrn->progClock = TRUE;

    /* For Virtual selection, the scanout area is all the free space we have. */
    rhdPtr->FbScanoutStart = rhdPtr->FbFreeStart;
    rhdPtr->FbScanoutSize = rhdPtr->FbFreeSize;

    if (pScrn->display->virtualX && pScrn->display->virtualY)
        if (!RHDGetVirtualFromConfig(pScrn)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Unable to find valid framebuffer dimensions\n");
	    goto error1;
	}

    if (! rhdPtr->randr) {
	Modes = RHDModesPoolCreate(pScrn, FALSE);
	if (!Modes) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
	    goto error1;
	}

	if (!pScrn->virtualX || !pScrn->virtualY)
	    RHDGetVirtualFromModesAndFilter(pScrn, Modes, FALSE);

	RHDModesAttach(pScrn, Modes);

	rhdModeDPISet(pScrn);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "Using %dx%d Framebuffer with %d pitch\n", pScrn->virtualX,
               pScrn->virtualY, pScrn->displayWidth);
    /* grab the real scanout area and adjust the free space */
    rhdPtr->FbScanoutStart = rhdPtr->FbFreeStart;
    rhdPtr->FbScanoutSize = RHD_FB_CHUNK(pScrn->displayWidth * pScrn->bitsPerPixel *
					 pScrn->virtualY / 8);
    RHDDebug(pScrn->scrnIndex, "ScanoutBuffer at offset 0x%08X (size = 0x%08X)\n",
	     rhdPtr->FbScanoutStart, rhdPtr->FbScanoutSize);

    rhdPtr->FbFreeStart = rhdPtr->FbScanoutStart + rhdPtr->FbScanoutSize;
    rhdPtr->FbFreeSize -= rhdPtr->FbScanoutSize;

    if (!rhdPtr->randr)
	xf86PrintModes(pScrn);
    else
	/* If monitor resolution is set on the command line, use it */
	xf86SetDpi(pScrn, 0, 0);

    if (xf86LoadSubModule(pScrn, "fb") == NULL) {
	goto error1;
    }

    if (!rhdPtr->swCursor.val.bool) {
	if (!xf86LoadSubModule(pScrn, "ramdac")) {
	    goto error1;
	}
    }

    /* try to load the XAA module here */
    if (rhdPtr->AccelMethod == RHD_ACCEL_XAA) {
	if (!xf86LoadSubModule(pScrn, "xaa")) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to load XAA module."
		       " Falling back to ShadowFB.\n");
	    rhdPtr->AccelMethod = RHD_ACCEL_SHADOWFB;
	} else
	    xf86LoaderReqSymLists(xaaSymbols, NULL);
    }

#ifdef USE_EXA
    /* try to load the EXA module here */
    if (rhdPtr->AccelMethod == RHD_ACCEL_EXA) {
	if (!xf86LoadSubModule(pScrn, "exa")) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to load EXA module."
		       " Falling back to ShadowFB.\n");
	    rhdPtr->AccelMethod = RHD_ACCEL_SHADOWFB;
	} else
	    xf86LoaderReqSymLists(exaSymbols, NULL);
    }
#endif /* USE_EXA */

    /* Last resort: try shadowFB */
    if (rhdPtr->AccelMethod == RHD_ACCEL_SHADOWFB)
	RHDShadowPreInit(pScrn);

    if ((rhdPtr->AccelMethod == RHD_ACCEL_XAA) ||
	(rhdPtr->AccelMethod == RHD_ACCEL_EXA))
	rhdFbOffscreenGrab(pScrn);

    RHDDebug(pScrn->scrnIndex, "Free FB offset 0x%08X (size = 0x%08X)\n",
	     rhdPtr->FbFreeStart, rhdPtr->FbFreeSize);

    ret = TRUE;

 error1:
    rhdUnmapMMIO(rhdPtr);
 error0:
    if (!ret)
	RHDFreeRec(pScrn);

    return ret;
}

/* Mandatory */
static Bool
RHDScreenInit(int scrnIndex, ScreenPtr pScreen, int argc, char **argv)
{
    ScrnInfoPtr pScrn;
    RHDPtr rhdPtr;
    VisualPtr visual;
    unsigned int racflag = 0;

    pScrn = xf86Screens[pScreen->myNum];
    rhdPtr = RHDPTR(pScrn);
    RHDFUNC(pScrn);

    /* map IO and FB */
    if (!rhdMapMMIO(rhdPtr)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to map MMIO.\n");
	return FALSE;
    }

    if (!rhdMapFB(rhdPtr)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to map FB.\n");
	return FALSE;
    }

    /* save previous mode */
    rhdSave(rhdPtr);

    /* init DIX */
    miClearVisualTypes();

    /* Setup the visuals we support. */
    if (!miSetVisualTypes(pScrn->depth, miGetDefaultVisualMask(pScrn->depth),
			  pScrn->rgbBits, pScrn->defaultVisual))
         return FALSE;

    if (!miSetPixmapDepths())
	return FALSE;

    /* Setup memory to which we draw; either shadow (RAM) or scanout (FB) */
    if (rhdPtr->AccelMethod == RHD_ACCEL_SHADOWFB) {
	if (!RHDShadowScreenInit(pScreen)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "ShadowFB initialisation failed."
		       " Continuing without ShadowFB.\n");
	    rhdPtr->AccelMethod = RHD_ACCEL_NONE;
	}
    }

    /* shadowfb is allowed to fail gracefully too */
    if ((rhdPtr->AccelMethod != RHD_ACCEL_SHADOWFB) &&
	!fbScreenInit(pScreen, (CARD8 *) rhdPtr->FbBase + rhdPtr->FbScanoutStart,
		      pScrn->virtualX, pScrn->virtualY, pScrn->xDpi, pScrn->yDpi,
		      pScrn->displayWidth, pScrn->bitsPerPixel)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "%s: fbScreenInit failed.\n", __func__);
	return FALSE;
    }

    if (pScrn->depth > 8) {
        /* Fixup RGB ordering */
        visual = pScreen->visuals + pScreen->numVisuals;
        while (--visual >= pScreen->visuals) {
	    if ((visual->class | DynamicClass) == DirectColor
		&& visual->nplanes > 8) {
		visual->offsetRed = pScrn->offset.red;
		visual->offsetGreen = pScrn->offset.green;
		visual->offsetBlue = pScrn->offset.blue;
		visual->redMask = pScrn->mask.red;
		visual->greenMask = pScrn->mask.green;
		visual->blueMask = pScrn->mask.blue;
	    }
	}
    }

    /* must be after RGB ordering fixed */
    fbPictureInit(pScreen, 0, 0);
    xf86SetBlackWhitePixels(pScreen);

    if (rhdPtr->AccelMethod == RHD_ACCEL_SHADOWFB) {
	if (!RHDShadowSetup(pScreen))
	    /* No safetynet anymore */
	    return FALSE;
    } else if (rhdPtr->AccelMethod == RHD_ACCEL_XAA) {
	if (rhdPtr->ChipSet < RHD_R600)
	    R5xxXAAInit(pScrn, pScreen);
    }
#ifdef USE_EXA
    else if (rhdPtr->AccelMethod == RHD_ACCEL_EXA) {
 	if (rhdPtr->ChipSet < RHD_R600)
 	    R5xxEXAInit(pScrn, pScreen);
    }
#endif /* USE_EXA */

    miInitializeBackingStore(pScreen);
    xf86SetBackingStore(pScreen);
    xf86SetSilkenMouse(pScreen);

    /* init randr */
    if (rhdPtr->randr && !RHDRandrScreenInit (pScreen)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "RandrScreenInit failed. Try Option \"noRandr\"\n");
	return FALSE;
    }

    /* now init the new mode */
    if (rhdPtr->randr)
	RHDRandrModeInit(pScrn);
    else
	rhdModeInit(pScrn, pScrn->currentMode);

    /* fix viewport */
    RHDAdjustFrame(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

    /* Initialise cursor functions */
    miDCInitialize (pScreen, xf86GetPointerScreenFuncs());

    /* Inititalize HW cursor */
    if (!rhdPtr->swCursor.val.bool)
        if (!RHDxf86InitCursor(pScreen))
            xf86DrvMsg(scrnIndex, X_ERROR,
                       "Hardware cursor initialization failed\n");

    /* default colormap */
    if(!miCreateDefColormap(pScreen))
	return FALSE;
    /* fixme */
    if (!xf86HandleColormaps(pScreen, 256, pScrn->rgbBits,
                         RHDLoadPalette, NULL,
                         CMAP_PALETTED_TRUECOLOR | CMAP_RELOAD_ON_MODE_SWITCH))
	return FALSE;

    pScrn->racIoFlags = pScrn->racMemFlags = racflag;

    /* @@@@ initialize video overlays here */

    /* Function to unblank, so that we don't show an uninitialised FB */
    pScreen->SaveScreen = RHDSaveScreen;

    /* Setup DPMS mode */

    xf86DPMSInit(pScreen, (DPMSSetProcPtr)RHDDisplayPowerManagementSet,0);

    /* Wrap the current CloseScreen function */
    rhdPtr->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = RHDCloseScreen;

    /* Report any unused options (only for the first generation) */
    if (serverGeneration == 1) {
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);
    }

    return TRUE;
}

/* Mandatory */
static Bool
RHDCloseScreen(int scrnIndex, ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    RHDPtr rhdPtr = RHDPTR(pScrn);

    if(pScrn->vtSema) {
	struct rhdCrtc *Crtc;
	int i;

	/* stop scanout */
	for (i = 0; i < 2; i++) {
	    Crtc = rhdPtr->Crtc[i];
	    if (scrnIndex == Crtc->scrnIndex)
		Crtc->Power(Crtc, RHD_POWER_RESET);
	}

	/* TODO: Invalidate the cached acceleration registers */
	if ((rhdPtr->ChipSet < RHD_R600) && rhdPtr->TwoDInfo)
	    R5xx2DIdle(pScrn);

	if (!RHDMCIdle(rhdPtr, 1000))
	    xf86DrvMsg(scrnIndex, X_WARNING, "MC not idle\n");

	rhdRestore(rhdPtr);
    }

    if (rhdPtr->AccelMethod == RHD_ACCEL_SHADOWFB)
	RHDShadowCloseScreen(pScreen);
#ifdef USE_EXA
    else if (rhdPtr->AccelMethod == RHD_ACCEL_EXA) {
	if (rhdPtr->ChipSet < RHD_R600)
	    R5xxEXACloseScreen(pScreen);
    }
#endif /* USE_EXA */
    /* nothing for XAA: handled in FreeRec */

    rhdUnmapFB(rhdPtr);
    rhdUnmapMMIO(rhdPtr);

    pScrn->vtSema = FALSE;
    pScreen->CloseScreen = rhdPtr->CloseScreen;
    return (*pScreen->CloseScreen)(scrnIndex, pScreen);
}

/* Optional */
static void
RHDFreeScreen(int scrnIndex, int flags)
{
    RHDFreeRec(xf86Screens[scrnIndex]);
}

/* Mandatory */
static Bool
RHDEnterVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    RHDPtr rhdPtr = RHDPTR(pScrn);

    RHDFUNC(rhdPtr);

    rhdSave(rhdPtr);

    if ((rhdPtr->ChipSet < RHD_R600) && rhdPtr->TwoDInfo)
	R5xx2DIdle(pScrn);

    if (rhdPtr->randr)
	RHDRandrModeInit(pScrn);
    else
	rhdModeInit(pScrn, pScrn->currentMode);

    /* @@@ video overlays can be initialized here */

    if (rhdPtr->CursorInfo)
	rhdReloadCursor(pScrn);
    /* rhdShowCursor() done by AdjustFrame */
    RHDAdjustFrame(pScrn->scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

    if ((rhdPtr->ChipSet < RHD_R600) && rhdPtr->TwoDInfo)
	R5xx2DSetup(pScrn);

    return TRUE;
}

/* Mandatory */
static void
RHDLeaveVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct rhdCrtc *Crtc;
    int i;

    RHDFUNC(rhdPtr);

    /* stop scanout */
    for (i = 0; i < 2; i++) {
	Crtc = rhdPtr->Crtc[i];
	if (scrnIndex == Crtc->scrnIndex)
	    Crtc->Power(Crtc, RHD_POWER_RESET);
    }

    /* TODO: Invalidate the cached acceleration registers */
    if ((rhdPtr->ChipSet < RHD_R600) && rhdPtr->TwoDInfo)
	R5xx2DIdle(pScrn);

    if (!RHDMCIdle(rhdPtr, 1000))
	xf86DrvMsg(scrnIndex, X_WARNING, "MC not idle\n");

    rhdRestore(rhdPtr);
}

static Bool
RHDSwitchMode(int scrnIndex, DisplayModePtr mode, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    RHDPtr rhdPtr = RHDPTR(pScrn);

    RHDFUNC(rhdPtr);

    if ((rhdPtr->ChipSet < RHD_R600) && rhdPtr->TwoDInfo)
	R5xx2DIdle(pScrn);

    if (rhdPtr->randr)
	RHDRandrSwitchMode(pScrn, mode);
    else {
	rhdPrepareMode(rhdPtr);
	rhdSetMode(xf86Screens[scrnIndex], mode);
    }

    return TRUE;
}

/*
 * High level bit banging functions
 */

static void
RHDAdjustFrame(int scrnIndex, int x, int y, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct rhdCrtc *Crtc;

    if (! rhdPtr->randr) {
	Crtc = rhdPtr->Crtc[0];
	if ((Crtc->scrnIndex == scrnIndex) && Crtc->Active)
	    Crtc->FrameSet(Crtc, x, y);

	Crtc = rhdPtr->Crtc[1];
	if ((Crtc->scrnIndex == scrnIndex) && Crtc->Active)
	    Crtc->FrameSet(Crtc, x, y);
    }

    if (rhdPtr->CursorInfo)
	rhdShowCursor(pScrn);
}

static void
RHDDisplayPowerManagementSet(ScrnInfoPtr pScrn,
                             int PowerManagementMode,
			     int flags)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct rhdOutput *Output;
    struct rhdCrtc *Crtc1, *Crtc2;

    RHDFUNC(rhdPtr);

    if (!pScrn->vtSema)
	return;

    Crtc1 = rhdPtr->Crtc[0];
    Crtc2 = rhdPtr->Crtc[1];

    switch (PowerManagementMode) {
    case DPMSModeOn:
	if (Crtc1->Active) {
	    Crtc1->Power(Crtc1, RHD_POWER_ON);

	    for (Output = rhdPtr->Outputs; Output; Output = Output->Next)
		if (Output->Power && Output->Active && (Output->Crtc == Crtc1))
		    Output->Power(Output, RHD_POWER_ON);

	    Crtc1->Blank(Crtc1, FALSE);
	}

	if (Crtc2->Active) {
	    Crtc2->Power(Crtc2, RHD_POWER_ON);

	    for (Output = rhdPtr->Outputs; Output; Output = Output->Next)
		if (Output->Power && Output->Active && (Output->Crtc == Crtc2))
		    Output->Power(Output, RHD_POWER_ON);

	    Crtc2->Blank(Crtc2, FALSE);
	}
	break;
    case DPMSModeStandby:
    case DPMSModeSuspend:
    case DPMSModeOff:
	if (Crtc1->Active) {
	    Crtc1->Blank(Crtc1, TRUE);

	    for (Output = rhdPtr->Outputs; Output; Output = Output->Next)
		if (Output->Power && Output->Active && (Output->Crtc == Crtc1))
		    Output->Power(Output, RHD_POWER_RESET);

	    Crtc1->Power(Crtc1, RHD_POWER_RESET);
	}

	if (Crtc2->Active) {
	    Crtc2->Blank(Crtc2, TRUE);

	    for (Output = rhdPtr->Outputs; Output; Output = Output->Next)
		if (Output->Power && Output->Active && (Output->Crtc == Crtc2))
		    Output->Power(Output, RHD_POWER_RESET);

	    Crtc2->Power(Crtc2, RHD_POWER_RESET);
	}
	break;
    }
}

/*
 *
 */
static void
RHDLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors,
	       VisualPtr pVisual)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct rhdCrtc *Crtc;

    Crtc = rhdPtr->Crtc[0];
    if ((pScrn->scrnIndex == Crtc->scrnIndex) && Crtc->Active)
	Crtc->LUT->Set(Crtc->LUT, numColors, indices, colors);

    Crtc = rhdPtr->Crtc[1];
    if ((pScrn->scrnIndex == Crtc->scrnIndex) && Crtc->Active)
	Crtc->LUT->Set(Crtc->LUT, numColors, indices, colors);
}

/*
 *
 */
static Bool
RHDSaveScreen(ScreenPtr pScreen, int on)
{
    ScrnInfoPtr pScrn;
    RHDPtr rhdPtr;
    struct rhdCrtc *Crtc;
    Bool unblank;

    unblank = xf86IsUnblank(on);

    if (unblank)
	SetTimeSinceLastInputEvent();

    if (pScreen == NULL)
	return TRUE;

    pScrn = xf86Screens[pScreen->myNum];

    if (pScrn == NULL)
	return TRUE;

    RHDFUNC(pScrn);

    rhdPtr = RHDPTR(pScrn);

    if (!pScrn->vtSema)
	return TRUE;

    Crtc = rhdPtr->Crtc[0];
    if (pScreen->myNum == Crtc->scrnIndex)
	Crtc->Blank(Crtc, !unblank);

    Crtc = rhdPtr->Crtc[1];
    if (pScreen->myNum == Crtc->scrnIndex)
	Crtc->Blank(Crtc, !unblank);

    return TRUE;
}

/*
 *
 */
static Bool
rhdMapMMIO(RHDPtr rhdPtr)
{
    RHDFUNC(rhdPtr);

#ifdef XSERVER_LIBPCIACCESS

    rhdPtr->MMIOMapSize = rhdPtr->PciInfo->regions[RHD_MMIO_BAR].size;

    if (pci_device_map_range(rhdPtr->PciInfo,
			     rhdPtr->PciInfo->regions[RHD_MMIO_BAR].base_addr,
			     rhdPtr->MMIOMapSize,
			     PCI_DEV_MAP_FLAG_WRITABLE,
			     &rhdPtr->MMIOBase))
	rhdPtr->MMIOBase = NULL;

#else

    rhdPtr->MMIOMapSize = 1 << rhdPtr->PciInfo->size[RHD_MMIO_BAR];
    rhdPtr->MMIOBase =
        xf86MapPciMem(rhdPtr->scrnIndex, VIDMEM_MMIO, rhdPtr->PciTag,
		      rhdPtr->PciInfo->memBase[RHD_MMIO_BAR],
		      rhdPtr->MMIOMapSize);
#endif
    if (!rhdPtr->MMIOBase)
        return FALSE;

    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Mapped IO at %p (size 0x%08X)\n",
	       rhdPtr->MMIOBase, rhdPtr->MMIOMapSize);

    return TRUE;
}

/*
 *
 */
static void
rhdUnmapMMIO(RHDPtr rhdPtr)
{
    RHDFUNC(rhdPtr);

#ifdef XSERVER_LIBPCIACCESS
    pci_device_unmap_range(rhdPtr->PciInfo, (pointer)rhdPtr->MMIOBase,
			    rhdPtr->MMIOMapSize);
#else
    xf86UnMapVidMem(rhdPtr->scrnIndex, (pointer)rhdPtr->MMIOBase,
                    rhdPtr->MMIOMapSize);
#endif
    rhdPtr->MMIOBase = 0;
}

/*
 *
 */
static CARD32
rhdGetVideoRamSize(RHDPtr rhdPtr)
{
    CARD32 RamSize, BARSize;

    RHDFUNC(rhdPtr);

    if (rhdPtr->ChipSet < RHD_R600)
	RamSize = (RHDRegRead(rhdPtr, R5XX_CONFIG_MEMSIZE)) >> 10;
    else
	RamSize = (RHDRegRead(rhdPtr, R6XX_CONFIG_MEMSIZE)) >> 10;
#ifdef XSERVER_LIBPCIACCESS
    BARSize = rhdPtr->PciInfo->regions[RHD_FB_BAR].size;
#else
    BARSize = 1 << (rhdPtr->PciInfo->size[RHD_FB_BAR] - 10);
#endif
    if (RamSize > BARSize) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "The detected amount of videoram"
		   " exceeds the PCI BAR aperture.\n");
	xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Using only %dkB of the total "
		   "%dkB.\n", (int) BARSize, (int) RamSize);
	return BARSize;
    } else
	return RamSize;
}

/*
 *
 */
static Bool
rhdMapFB(RHDPtr rhdPtr)
{
    unsigned long membase;
    RHDFUNC(rhdPtr);

#ifdef XSERVER_LIBPCIACCESS

    rhdPtr->FbMapSize = rhdPtr->PciInfo->regions[RHD_FB_BAR].size;
    membase = rhdPtr->PciInfo->regions[RHD_FB_BAR].base_addr; /* @@@ */

    if (pci_device_map_range(rhdPtr->PciInfo,
			     membase,
			     rhdPtr->FbMapSize,
			     PCI_DEV_MAP_FLAG_WRITABLE
			     | PCI_DEV_MAP_FLAG_WRITE_COMBINE
			     | PCI_DEV_MAP_FLAG_CACHABLE,
			     &rhdPtr->FbBase))
	rhdPtr->FbBase = NULL;

#else

    rhdPtr->FbMapSize = 1 << rhdPtr->PciInfo->size[RHD_FB_BAR];
    rhdPtr->FbBase =
        xf86MapPciMem(rhdPtr->scrnIndex, VIDMEM_FRAMEBUFFER, rhdPtr->PciTag,
		      rhdPtr->PciInfo->memBase[RHD_FB_BAR], rhdPtr->FbMapSize);
    membase = rhdPtr->PciInfo->memBase[RHD_FB_BAR];

#endif

    if (!rhdPtr->FbBase)
        return FALSE;

    /* These devices have an internal address reference, which some other
     * address registers in there also use. This can be different from the
     * address in the BAR */
    if (rhdPtr->ChipSet < RHD_R600)
	rhdPtr->FbIntAddress = RHDRegRead(rhdPtr, HDP_FB_LOCATION)
			       << 16;
    else
	rhdPtr->FbIntAddress = RHDRegRead(rhdPtr, R6XX_CONFIG_FB_BASE);

    if (rhdPtr->FbIntAddress != membase)
	    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "PCI FB Address (BAR) is at "
		       "0x%08X while card Internal Address is 0x%08X\n",
		       (unsigned int) membase,
		       rhdPtr->FbIntAddress);
    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Mapped FB at %p (size 0x%08X)\n",
	       rhdPtr->FbBase, rhdPtr->FbMapSize);
    return TRUE;
}

/*
 *
 */
static void
rhdUnmapFB(RHDPtr rhdPtr)
{
    RHDFUNC(rhdPtr);

    if (!rhdPtr->FbBase)
	return;
#ifdef XSERVER_LIBPCIACCESS
    pci_device_unmap_range(rhdPtr->PciInfo, (pointer)rhdPtr->FbBase,
			   rhdPtr->FbMapSize);
#else
    xf86UnMapVidMem(rhdPtr->scrnIndex, (pointer)rhdPtr->FbBase,
                    rhdPtr->FbMapSize);
#endif
    rhdPtr->FbBase = 0;
}

/*
 *
 */
static void
rhdFbOffscreenGrab(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    RHDOpt Option = rhdPtr->OffscreenOption;
    int tmp;
    unsigned int size = 0;

    if (Option.set) {
	if ((sscanf(Option.val.string, "%dm", &tmp) == 1) ||
	    (sscanf(Option.val.string, "%dM", &tmp) == 1)) {
	    /* "...m" or "...M" means megabyte. */
	    size = tmp << 20;
	} else if (sscanf(Option.val.string, "%d%%", &tmp) == 1) {
	    /* "...%" of total framebuffer memory */
	    size = tmp * pScrn->videoRam / 100;
	} else
	    xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING, "Option OffscreenSize: "
		       "Unable to parse \"%s\".\n", Option.val.string);
    }

    if (!size)
	size = pScrn->videoRam * 1024 / 10;

    if (size > rhdPtr->FbFreeSize)
	size = rhdPtr->FbFreeSize;

    /* calculate the number of lines our frontbuffer and offscreen have */
    tmp = rhdPtr->FbScanoutSize + size;
    tmp /= (pScrn->displayWidth * pScrn->bitsPerPixel >> 3);

    if (rhdPtr->ChipSet < RHD_R600) {
	if (tmp > 0x1FFF) /* cannot go beyond 8k lines on R5xx */
	    tmp = 0x1FFF;
    } else {
	if (tmp > 0x7FFF) /* X limit (signed short). */
	    tmp = 0x7FFF;
    }

    tmp -= pScrn->virtualY;

    /* get our actual size */
    tmp *= (pScrn->displayWidth * pScrn->bitsPerPixel >> 3);

    tmp = RHD_FB_CHUNK(tmp);

    rhdPtr->FbOffscreenStart = rhdPtr->FbFreeStart;
    rhdPtr->FbOffscreenSize = tmp;

    rhdPtr->FbFreeStart += rhdPtr->FbOffscreenSize;
    rhdPtr->FbFreeSize -= rhdPtr->FbOffscreenSize;

    RHDDebug(pScrn->scrnIndex, "Offscreen Buffer at offset 0x%08X (size = 0x%08X)\n",
	     rhdPtr->FbOffscreenStart, rhdPtr->FbOffscreenSize);
}

/*
 *
 */
static void
rhdOutputConnectorCheck(struct rhdConnector *Connector)
{
    struct rhdOutput *Output;
    int i;

    /* First, try to sense */
    for (i = 0; i < 2; i++) {
	Output = Connector->Output[i];
	if (Output && Output->Sense
	    && (Output->SensedType = Output->Sense(Output, Connector->Type))) {
	    RHDOutputPrintSensedType(Output);
	    Output->Connector = Connector;
	    break;
	}
    }

    if (i == 2) {
	/* now just enable the ones without sensing */
	for (i = 0; i < 2; i++) {
	    Output = Connector->Output[i];
	    if (Output && !Output->Sense) {
		Output->Connector = Connector;
		break;
	    }
	}
    }
}

/*
 *
 */
static Bool
rhdModeLayoutSelect(RHDPtr rhdPtr)
{
    struct rhdOutput *Output;
    struct rhdConnector *Connector;
    Bool Found = FALSE;
    char *ignore = NULL;
    Bool ConnectorIsDMS59 = FALSE;
    int i = 0;

    RHDFUNC(rhdPtr);

    /* housekeeping */
    rhdPtr->Crtc[0]->PLL = rhdPtr->PLLs[0];
    rhdPtr->Crtc[0]->LUT = rhdPtr->LUT[0];

    rhdPtr->Crtc[1]->PLL = rhdPtr->PLLs[1];
    rhdPtr->Crtc[1]->LUT = rhdPtr->LUT[1];

    /* start layout afresh */
    for (Output = rhdPtr->Outputs; Output; Output = Output->Next) {
	Output->Active = FALSE;
	Output->Crtc = NULL;
	Output->Connector = NULL;
    }

    /* quick and dirty option so that some output choice exists */
    ignore = xf86GetOptValString(rhdPtr->Options, OPTION_IGNORECONNECTOR);

    /* handle cards with DMS-59 connectors appropriately. The DMS-59 to VGA
       adapter does not raise HPD at all, so we need a fallback there. */
    if (rhdPtr->Card) {
	ConnectorIsDMS59 = rhdPtr->Card->flags & RHD_CARD_FLAG_DMS59;
	if (ConnectorIsDMS59)
	    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Card %s has a DMS-59"
		       " connector.\n", rhdPtr->Card->name);
    }

    /* Check on the basis of Connector->HPD */
    for (i = 0; i < RHD_CONNECTORS_MAX; i++) {
	Connector = rhdPtr->Connector[i];

	if (!Connector)
	    continue;

	if (ignore && !strcasecmp(Connector->Name, ignore)) {
	    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO,
		       "Skipping connector \"%s\"\n", ignore);
	    continue;
	}

	if (Connector->HPDCheck) {
	    if (Connector->HPDCheck(Connector)) {
		Connector->HPDAttached = TRUE;

		rhdOutputConnectorCheck(Connector);
	    } else {
		Connector->HPDAttached = FALSE;
		if (ConnectorIsDMS59)
		    rhdOutputConnectorCheck(Connector);
	    }
	} else
	    rhdOutputConnectorCheck(Connector);
    }

    i = 0; /* counter for CRTCs */
    for (Output = rhdPtr->Outputs; Output; Output = Output->Next)
	if (Output->Connector) {
	    struct rhdMonitor *Monitor = NULL;

	    Connector = Output->Connector;

	    Monitor = RHDMonitorInit(Connector);

	    if (!Monitor && (Connector->Type == RHD_CONNECTOR_PANEL)) {
		xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING, "Unable to attach a"
			   " monitor to connector \"%s\"\n", Connector->Name);
		Output->Active = FALSE;
	    } else {
		Connector->Monitor = Monitor;

		Output->Active = TRUE;

		Output->Crtc = rhdPtr->Crtc[i & 1]; /* ;) */
		i++;

		Output->Crtc->Active = TRUE;

		Found = TRUE;

		if (Monitor) {
		    /* If this is a DVI attached monitor, enable reduced blanking.
		     * TODO: iiyama vm pro 453: CRT with DVI-D == No reduced.
		     */
		    if ((Output->Id == RHD_OUTPUT_TMDSA) ||
			(Output->Id == RHD_OUTPUT_LVTMA) ||
			(Output->Id == RHD_OUTPUT_KLDSKP_LVTMA) ||
			(Output->Id == RHD_OUTPUT_UNIPHYA) ||
			(Output->Id == RHD_OUTPUT_UNIPHYB))
			Monitor->ReducedAllowed = TRUE;

		    /* allow user to override settings globally */
		    if (rhdPtr->forceReduced.set)
			Monitor->ReducedAllowed = rhdPtr->forceReduced.val.bool;

		    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO,
			       "Connector \"%s\" uses Monitor \"%s\":\n",
			       Connector->Name, Monitor->Name);
		    RHDMonitorPrint(Monitor);
		} else
		    xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING,
			       "Connector \"%s\": Failed to retrieve Monitor"
			       " information.\n", Connector->Name);
	    }
	}

    return Found;
}

/*
 * Calculating DPI will never be good. But here we attempt to make it work,
 * somewhat, with multiple monitors.
 *
 * The real solution for the DPI problem cannot be something statically,
 * as DPI varies with resolutions chosen and with displays attached/detached.
 */
static void
rhdModeDPISet(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);

    /* first cleanse the option to at least be reasonable */
    if (rhdPtr->forceDPI.set)
	if ((rhdPtr->forceDPI.val.integer < 20) ||
	    (rhdPtr->forceDPI.val.integer > 1000)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Option ForceDPI got passed"
		       " an insane value: %d. Ignoring.\n",
		       rhdPtr->forceDPI.val.integer);
	    rhdPtr->forceDPI.set = FALSE;
	}

    /* monitorResolution is the DPI that was passed as a server option.
     * This has been available all the way back to the original Xorg import */
    if (monitorResolution > 0) {
	RHDDebug(pScrn->scrnIndex, "%s: Forcing DPI through xserver argument.\n",
		 __func__);

	pScrn->xDpi = monitorResolution;
	pScrn->yDpi = monitorResolution;
    } else if (rhdPtr->forceDPI.set) {
	RHDDebug(pScrn->scrnIndex, "%s: Forcing DPI through configuration option.\n",
		 __func__);

	pScrn->xDpi = rhdPtr->forceDPI.val.integer;
	pScrn->yDpi = rhdPtr->forceDPI.val.integer;
    } else {
	/* go over the monitors */
	struct rhdCrtc *Crtc;
	struct rhdOutput *Output;
	struct rhdMonitor *Monitor;
	/* we need to use split counters, x or y might fail separately */
	int i, xcount, ycount;

	pScrn->xDpi = 0;
	pScrn->yDpi = 0;
	xcount = 0;
	ycount = 0;

	for (i = 0; i < 2; i++) {
	    Crtc = rhdPtr->Crtc[i];
	    if (Crtc->Active) {
		for (Output = rhdPtr->Outputs; Output; Output = Output->Next) {
		    if (Output->Active && (Output->Crtc == Crtc)) {
			if (Output->Connector && Output->Connector->Monitor) {
			    Monitor = Output->Connector->Monitor;

			    if (Monitor->xDpi) {
				pScrn->xDpi += (Monitor->xDpi - pScrn->xDpi) / (xcount + 1);
				xcount++;
			    }

			    if (Monitor->yDpi) {
				pScrn->yDpi += (Monitor->yDpi - pScrn->yDpi) / (ycount + 1);
				ycount++;
			    }
			}
		    }
		}
	    }
	}

	/* make sure that we have at least some value */
	if (!pScrn->xDpi || !pScrn->yDpi) {
	    if (pScrn->xDpi)
		pScrn->yDpi = pScrn->xDpi;
	    else if (pScrn->yDpi)
		pScrn->xDpi = pScrn->yDpi;
	    else {
		pScrn->xDpi = 96;
		pScrn->yDpi = 96;
	    }
	}
    }

#ifndef MMPERINCH
#define MMPERINCH 25.4
#endif
    pScrn->widthmm = pScrn->virtualX * MMPERINCH / pScrn->xDpi;
    pScrn->heightmm = pScrn->virtualY * MMPERINCH / pScrn->yDpi;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using %dx%d DPI.\n",
	       pScrn->xDpi, pScrn->yDpi);
}


/*
 *
 */
static void
rhdModeLayoutPrint(RHDPtr rhdPtr)
{
    struct rhdCrtc *Crtc;
    struct rhdOutput *Output;
    Bool Found;

    xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Listing modesetting layout:\n\n");

    /* CRTC 1 */
    Crtc = rhdPtr->Crtc[0];
    if (Crtc->Active) {
	xf86Msg(X_NONE, "\t%s: tied to %s and %s:\n",
		Crtc->Name, Crtc->PLL->Name, Crtc->LUT->Name);

	Found = FALSE;
	for (Output = rhdPtr->Outputs; Output; Output = Output->Next)
	    if (Output->Active && (Output->Crtc == Crtc)) {
		if (!Found) {
		    xf86Msg(X_NONE, "\t\tOutputs: %s (%s)",
			    Output->Name, Output->Connector->Name);
		    Found = TRUE;
		} else
		    xf86Msg(X_NONE, ", %s (%s)", Output->Name,
			    Output->Connector->Name);
	    }

	if (!Found)
	    xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		       "%s is active without outputs\n", Crtc->Name);
	else
	     xf86Msg(X_NONE, "\n");
    } else
	xf86Msg(X_NONE, "\t%s: unused\n", Crtc->Name);
    xf86Msg(X_NONE, "\n");

    /* CRTC 2 */
    Crtc = rhdPtr->Crtc[1];
    if (Crtc->Active) {
	xf86Msg(X_NONE, "\t%s: tied to %s and %s:\n",
		Crtc->Name, Crtc->PLL->Name, Crtc->LUT->Name);

	Found = FALSE;
	for (Output = rhdPtr->Outputs; Output; Output = Output->Next)
	    if (Output->Active && (Output->Crtc == Crtc)) {
		if (!Found) {
		    xf86Msg(X_NONE, "\t\tOutputs: %s (%s)",
			    Output->Name, Output->Connector->Name);
		    Found = TRUE;
		} else
		    xf86Msg(X_NONE, ", %s (%s)", Output->Name,
			    Output->Connector->Name);
	    }

	if (!Found)
	    xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR,
		       "%s is active without outputs\n", Crtc->Name);
	else
	    xf86Msg(X_NONE, "\n");
    } else
	xf86Msg(X_NONE, "\t%s: unused\n", Crtc->Name);
    xf86Msg(X_NONE, "\n");

    /* Print out unused Outputs */
    Found = FALSE;
    for (Output = rhdPtr->Outputs; Output; Output = Output->Next)
	if (!Output->Active) {
	    if (!Found) {
		xf86Msg(X_NONE, "\t\tUnused Outputs: %s", Output->Name);
		Found = TRUE;
	    } else
		xf86Msg(X_NONE, ", %s", Output->Name);
	}

    if (Found)
	xf86Msg(X_NONE, "\n");
    xf86Msg(X_NONE, "\n");
}

/*
 *
 */
static void
rhdPrepareMode(RHDPtr rhdPtr)
{
    RHDFUNC(rhdPtr);

    /* no active outputs == no mess */
    RHDOutputsPower(rhdPtr, RHD_POWER_RESET);

    /* Disable CRTCs to stop noise from appearing. */
    rhdPtr->Crtc[0]->Power(rhdPtr->Crtc[0], RHD_POWER_RESET);
    rhdPtr->Crtc[1]->Power(rhdPtr->Crtc[1], RHD_POWER_RESET);
}

/*
 *
 */
static void
rhdModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);

    RHDFUNC(rhdPtr);
    pScrn->vtSema = TRUE;

    /* Stop crap from being shown: gets reenabled through SaveScreen */
    rhdPtr->Crtc[0]->Blank(rhdPtr->Crtc[0], TRUE);
    rhdPtr->Crtc[1]->Blank(rhdPtr->Crtc[1], TRUE);

    rhdPrepareMode(rhdPtr);

    /* now disable our VGA Mode */
    RHDVGADisable(rhdPtr);

    /* now set up the MC */
    RHDMCSetup(rhdPtr);

    rhdSetMode(pScrn, mode);
}

/*
 *
 */
static void
rhdSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct rhdCrtc *Crtc;

    RHDFUNC(rhdPtr);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Setting up \"%s\" (%dx%d@%3.1fHz)\n",
	       mode->name, mode->CrtcHDisplay, mode->CrtcVDisplay,
	       mode->VRefresh);

    /* Set up D1 and appendages */
    Crtc = rhdPtr->Crtc[0];
    if (Crtc->Active) {
	Crtc->FBSet(Crtc, pScrn->displayWidth, pScrn->virtualX, pScrn->virtualY,
		    pScrn->depth, rhdPtr->FbScanoutStart);
	Crtc->ModeSet(Crtc, mode);
	if (Crtc->ScaleSet)
	    Crtc->ScaleSet(Crtc, RHD_CRTC_SCALE_TYPE_NONE, mode, NULL);
	RHDPLLSet(Crtc->PLL, mode->Clock);
	Crtc->PLLSelect(Crtc, Crtc->PLL);
	Crtc->LUTSelect(Crtc, Crtc->LUT);
	RHDOutputsMode(rhdPtr, Crtc, mode);
    }

    /* Set up D2 and appendages */
    Crtc = rhdPtr->Crtc[1];
    if (Crtc->Active) {
	Crtc->FBSet(Crtc, pScrn->displayWidth, pScrn->virtualX, pScrn->virtualY,
		    pScrn->depth, rhdPtr->FbScanoutStart);
	Crtc->ModeSet(Crtc, mode);
	if (Crtc->ScaleSet)
	    Crtc->ScaleSet(Crtc, RHD_CRTC_SCALE_TYPE_NONE, mode, NULL);
	RHDPLLSet(Crtc->PLL, mode->Clock);
	Crtc->PLLSelect(Crtc, Crtc->PLL);
	Crtc->LUTSelect(Crtc, Crtc->LUT);
	RHDOutputsMode(rhdPtr, Crtc, mode);
    }

    /* shut down that what we don't use */
    RHDPLLsShutdownInactive(rhdPtr);
    RHDOutputsShutdownInactive(rhdPtr);

    if (rhdPtr->Crtc[0]->Active)
	rhdPtr->Crtc[0]->Power(rhdPtr->Crtc[0], RHD_POWER_ON);
    else
	rhdPtr->Crtc[0]->Power(rhdPtr->Crtc[0], RHD_POWER_SHUTDOWN);

    if (rhdPtr->Crtc[1]->Active)
	rhdPtr->Crtc[1]->Power(rhdPtr->Crtc[1], RHD_POWER_ON);
    else
	rhdPtr->Crtc[1]->Power(rhdPtr->Crtc[1], RHD_POWER_SHUTDOWN);

    RHDOutputsPower(rhdPtr, RHD_POWER_ON);
}

/*
 *
 */
static void
rhdSave(RHDPtr rhdPtr)
{
    ScrnInfoPtr pScrn = xf86Screens[rhdPtr->scrnIndex];

    RHDFUNC(rhdPtr);

    RHDSaveMC(rhdPtr);

    RHDVGASave(rhdPtr);

    RHDOutputsSave(rhdPtr);

    RHDPLLsSave(rhdPtr);
    RHDLUTsSave(rhdPtr);

    rhdPtr->Crtc[0]->Save(rhdPtr->Crtc[0]);
    rhdPtr->Crtc[1]->Save(rhdPtr->Crtc[1]);
    rhdSaveCursor(pScrn);
}

/*
 *
 */
static void
rhdRestore(RHDPtr rhdPtr)
{
    ScrnInfoPtr pScrn = xf86Screens[rhdPtr->scrnIndex];

    RHDFUNC(rhdPtr);

    RHDRestoreMC(rhdPtr);

    if (rhdPtr->CursorInfo)
	rhdRestoreCursor(pScrn);

    RHDPLLsRestore(rhdPtr);
    RHDLUTsRestore(rhdPtr);

    RHDVGARestore(rhdPtr);

    rhdPtr->Crtc[0]->Restore(rhdPtr->Crtc[0]);
    rhdPtr->Crtc[1]->Restore(rhdPtr->Crtc[1]);

    RHDOutputsRestore(rhdPtr);
}

/*
 *
 */
CARD32
_RHDRegRead(int scrnIndex, CARD16 offset)
{
    return *(volatile CARD32 *)((CARD8 *) RHDPTR(xf86Screens[scrnIndex])->MMIOBase + offset);
}

/*
 *
 */
void
_RHDRegWrite(int scrnIndex, CARD16 offset, CARD32 value)
{
    *(volatile CARD32 *)((CARD8 *) RHDPTR(xf86Screens[scrnIndex])->MMIOBase + offset) = value;
}

/*
 * This one might seem clueless, but it is an actual lifesaver.
 */
void
_RHDRegMask(int scrnIndex, CARD16 offset, CARD32 value, CARD32 mask)
{
    CARD32 tmp;

    tmp = _RHDRegRead(scrnIndex, offset);
    tmp &= ~mask;
    tmp |= (value & mask);
    _RHDRegWrite(scrnIndex, offset, tmp);
}

#ifdef RHD_DEBUG
/*
 *
 */
CARD32
_RHDRegReadD(int scrnIndex, CARD16 offset)
{
    CARD32 tmp =  *(volatile CARD32 *)((CARD8 *) RHDPTR(xf86Screens[scrnIndex])->MMIOBase + offset);
    xf86DrvMsg(scrnIndex, X_INFO, "RHDRegRead(0x%4.4x) = 0x%4.4x\n",offset,tmp);
    return tmp;
}

/*
 *
 */
void
_RHDRegWriteD(int scrnIndex, CARD16 offset, CARD32 value)
{
    xf86DrvMsg(scrnIndex, X_INFO, "RHDRegWrite(0x%4.4x,0x%4.4x)\n",offset,tmp);
    *(volatile CARD32 *)((CARD8 *) RHDPTR(xf86Screens[scrnIndex])->MMIOBase + offset) = value;
}

/*
 *
 */
void
_RHDRegMaskD(int scrnIndex, CARD16 offset, CARD32 value, CARD32 mask)
{
    CARD32 tmp;

    tmp = _RHDRegReadD(scrnIndex, offset);
    tmp &= ~mask;
    tmp |= (value & mask);
    _RHDRegWriteD(scrnIndex, offset, tmp);
}
#endif /* RHD_DEBUG */

/* The following two are R5XX only. R6XX doesn't require these */
CARD32
_RHDReadMC(int scrnIndex, CARD32 addr)
{
    RHDPtr rhdPtr = RHDPTR(xf86Screens[scrnIndex]);
    CARD32 ret;

    if (rhdPtr->ChipSet < RHD_RS690) {
	_RHDRegWrite(scrnIndex, MC_IND_INDEX, addr);
	ret = _RHDRegRead(scrnIndex, MC_IND_DATA);
    } else {
#ifdef XSERVER_LIBPCIACCESS
	CARD32 data = addr & ~RS69_C_IND_WR_EN;
	pci_device_cfg_write(rhdPtr->NBPciInfo, &(data), RS69_MC_INDEX, 4, NULL);
	pci_device_cfg_read(rhdPtr->NBPciInfo, &ret, RS69_MC_DATA, 4, NULL);
#else
	pciWriteLong(rhdPtr->NBPciTag, RS69_MC_INDEX, addr & ~RS69_C_IND_WR_EN);
	ret = pciReadLong(rhdPtr->NBPciTag, RS69_MC_DATA);
#endif
    }

    RHDDebug(scrnIndex,"%s(0x%08X) = 0x%08X\n",__func__,(unsigned int)addr,
	     (unsigned int)ret);
    return ret;
}

void
_RHDWriteMC(int scrnIndex, CARD32 addr, CARD32 data)
{
    RHDPtr rhdPtr = RHDPTR(xf86Screens[scrnIndex]);

    RHDDebug(scrnIndex,"%s(0x%08X, 0x%08X)\n",__func__,(unsigned int)addr,
	     (unsigned int)data);

    if (rhdPtr->ChipSet < RHD_RS690) {
	_RHDRegWrite(scrnIndex, MC_IND_INDEX, addr | MC_IND_WR_EN);
	_RHDRegWrite(scrnIndex, MC_IND_DATA, data);
    } else {
#ifdef XSERVER_LIBPCIACCESS
	CARD32 tmp = addr & ~RS69_C_IND_WR_EN;
	pci_device_cfg_write(rhdPtr->NBPciInfo, &tmp, RS69_MC_INDEX, 4, NULL);
	pci_device_cfg_write(rhdPtr->NBPciInfo, &data, RS69_MC_DATA, 4, NULL);
#else
	pciWriteLong(rhdPtr->NBPciTag, RS69_MC_INDEX, addr | RS69_C_IND_WR_EN);
	pciWriteLong(rhdPtr->NBPciTag, RS69_MC_DATA, data);
#endif

    }
}

CARD32
_RHDReadPLL(int scrnIndex, CARD16 offset)
{
    _RHDRegWrite(scrnIndex, CLOCK_CNTL_INDEX, (offset & PLL_ADDR));
    return _RHDRegRead(scrnIndex, CLOCK_CNTL_DATA);
}

void
_RHDWritePLL(int scrnIndex, CARD16 offset, CARD32 data)
{
    _RHDRegWrite(scrnIndex, CLOCK_CNTL_INDEX, (offset & PLL_ADDR) | PLL_WR_EN);
    _RHDRegWrite(scrnIndex, CLOCK_CNTL_DATA, data);
}

/*
 * Apart from handling the respective option, this also tries to map out
 * what method is supported on which chips.
 */
static void
rhdAccelOptionsHandle(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    RHDOpt method, noAccel;

    /* first grab our options */
    RhdGetOptValBool(rhdPtr->Options, OPTION_NOACCEL, &noAccel, FALSE);
    RhdGetOptValString(rhdPtr->Options, OPTION_ACCELMETHOD, &method, "default");
    RhdGetOptValString (rhdPtr->Options, OPTION_OFFSCREENSIZE,
			&rhdPtr->OffscreenOption, "default");

    if (method.set) {
	if (!strcasecmp(method.val.string, "none"))
	    rhdPtr->AccelMethod = RHD_ACCEL_NONE;
	else if (!strcasecmp(method.val.string, "shadowfb"))
	    rhdPtr->AccelMethod = RHD_ACCEL_SHADOWFB;
	else if (!strcasecmp(method.val.string, "xaa"))
	    rhdPtr->AccelMethod = RHD_ACCEL_XAA;
#ifdef USE_EXA
	else if (!strcasecmp(method.val.string, "exa"))
	    rhdPtr->AccelMethod = RHD_ACCEL_EXA;
#endif /* USE_EXA */
	else if (!strcasecmp(method.val.string, "default"))
	    rhdPtr->AccelMethod = RHD_ACCEL_DEFAULT;
	else {
	    xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING, "Unknown AccelMethod \"%s\".\n",
		       method.val.string);
	    rhdPtr->AccelMethod = RHD_ACCEL_DEFAULT;
	}
    } else
	rhdPtr->AccelMethod = RHD_ACCEL_DEFAULT;

    if (rhdPtr->AccelMethod == RHD_ACCEL_DEFAULT) {
	if (rhdPtr->ChipSet < RHD_R600)
	    rhdPtr->AccelMethod = RHD_ACCEL_XAA;
	else
	    rhdPtr->AccelMethod = RHD_ACCEL_SHADOWFB;
    }

    if (noAccel.set && noAccel.val.bool &&
	(rhdPtr->AccelMethod > RHD_ACCEL_SHADOWFB)) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_INFO, "Disabling HW 2D acceleration.\n");
	rhdPtr->AccelMethod = RHD_ACCEL_SHADOWFB;
    }

    if (rhdPtr->ChipSet >= RHD_R600) {
	if (rhdPtr->AccelMethod > RHD_ACCEL_SHADOWFB) {
	    xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING, "%s: HW 2D acceleration is"
		       " not implemented yet.\n",  pScrn->chipset);
	    rhdPtr->AccelMethod = RHD_ACCEL_SHADOWFB;
	}
    }

    /* Now for some pretty print */
    switch (rhdPtr->AccelMethod) {
#ifdef USE_EXA
    case RHD_ACCEL_EXA:
	xf86DrvMsg(rhdPtr->scrnIndex, X_CONFIG, "Selected EXA 2D acceleration.\n");
	break;
#endif /* USE_EXA */
    case RHD_ACCEL_XAA:
	xf86DrvMsg(rhdPtr->scrnIndex, X_CONFIG, "Selected XAA 2D acceleration.\n");
	break;
    case RHD_ACCEL_SHADOWFB:
	xf86DrvMsg(rhdPtr->scrnIndex, X_CONFIG, "Selected ShadowFB.\n");
	break;
    case RHD_ACCEL_NONE:
    default:
	xf86DrvMsg(rhdPtr->scrnIndex, X_WARNING, /* corny */
		   "All methods of acceleration have been disabled.\n");
	break;
    }
}

/*
 * breakout functions
 */
static void
rhdProcessOptions(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    RHDOpt hpd;
    /* Collect all of the relevant option flags (fill in pScrn->options) */
    xf86CollectOptions(pScrn, NULL);
    rhdPtr->Options = xnfcalloc(sizeof(RHDOptions), 1);
    memcpy(rhdPtr->Options, RHDOptions, sizeof(RHDOptions));

    /* Process the options */
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, rhdPtr->Options);

    RhdGetOptValBool   (rhdPtr->Options, OPTION_SW_CURSOR,
			&rhdPtr->swCursor, FALSE);
    RhdGetOptValBool   (rhdPtr->Options, OPTION_FORCEREDUCED,
			&rhdPtr->forceReduced, FALSE);
    RhdGetOptValInteger(rhdPtr->Options, OPTION_FORCEDPI,
			&rhdPtr->forceDPI, 0);
    RhdGetOptValString (rhdPtr->Options, OPTION_HPD,
			&hpd, "auto");
    RhdGetOptValBool   (rhdPtr->Options, OPTION_NORANDR,
			&rhdPtr->noRandr, FALSE);
    RhdGetOptValBool   (rhdPtr->Options, OPTION_RRUSEXF86EDID,
			&rhdPtr->rrUseXF86Edid, FALSE);
    RhdGetOptValString (rhdPtr->Options, OPTION_RROUTPUTORDER,
			&rhdPtr->rrOutputOrder, NULL);
    RhdGetOptValString (rhdPtr->Options, OPTION_TV_MODE,
			&rhdPtr->tvModeName, NULL);

    rhdAccelOptionsHandle(pScrn);

    rhdPtr->hpdUsage = RHD_HPD_USAGE_AUTO;
    if (strcasecmp(hpd.val.string, "off") == 0) {
	rhdPtr->hpdUsage = RHD_HPD_USAGE_OFF;
    } else if (strcasecmp(hpd.val.string, "normal") == 0) {
	rhdPtr->hpdUsage = RHD_HPD_USAGE_NORMAL;
    } else if (strcasecmp(hpd.val.string, "swap") == 0) {
	rhdPtr->hpdUsage = RHD_HPD_USAGE_SWAP;
    } else if (strcasecmp(hpd.val.string, "auto") != 0) {
	xf86DrvMsgVerb(rhdPtr->scrnIndex, X_ERROR, 0,
		       "Unknown HPD Option \"%s\"", hpd.val.string);
    }
    if (rhdPtr->hpdUsage != RHD_HPD_USAGE_AUTO)
	xf86DrvMsgVerb(rhdPtr->scrnIndex, X_WARNING, 0,
	"!!! Option HPD is set !!!\n"
	"     This shall only be used to work around broken connector tables.\n"
	"     Please report your findings to radeonhd@opensuse.org\n");
}

/*
 *  rhdDoReadPCIBios(): do the actual reading, return size and copy in ptr
 */
static unsigned int
rhdDoReadPCIBios(RHDPtr rhdPtr, unsigned char **ptr)
{
#ifdef XSERVER_LIBPCIACCESS
    unsigned int size = rhdPtr->PciInfo->rom_size;
#else
    unsigned int size = 1 << rhdPtr->PciInfo->biosSize;
    int read_len;
#endif

    if (!(*ptr = xcalloc(1, size))) {
	xf86DrvMsg(rhdPtr->scrnIndex,X_ERROR,
		   "Cannot allocate %i bytes of memory "
		   "for BIOS image\n",size);
	return 0;
    }
    xf86DrvMsg(rhdPtr->scrnIndex,X_INFO,"Getting BIOS copy from PCI ROM\n");

#ifdef XSERVER_LIBPCIACCESS
    if (pci_device_read_rom(rhdPtr->PciInfo, ptr)) {
	xf86DrvMsg(rhdPtr->scrnIndex,X_ERROR,
		   "Cannot read BIOS image\n");
	xfree(*ptr);
	return 0;
    }
#else
    if ((read_len =
	 xf86ReadPciBIOS(0, rhdPtr->PciTag, -1, *ptr, size)) < 0) {
	xf86DrvMsg(rhdPtr->scrnIndex,X_ERROR,
		   "Cannot read BIOS image\n");
	xfree(*ptr);
	return 0;
    } else if ((unsigned int)read_len != size) {
	xf86DrvMsg(rhdPtr->scrnIndex,X_WARNING,
		   "Read only %i of %i bytes of BIOS image\n",
		   read_len, size);
	return (unsigned int)read_len;
    }
#endif
    return size;
}

/*
 * rhdR5XXDoReadPCIBios(): enables access to R5xx BIOS, wraps rhdDoReadPCIBios()
 */
static unsigned int
rhdR5XXDoReadPCIBios(RHDPtr rhdPtr, unsigned char **ptr)
{
    unsigned int ret;
    CARD32 save_seprom_cntl1,
	save_gpiopad_a, save_gpiopad_en, save_gpiopad_mask,
	save_viph_cntl,
	save_bus_cntl,
	save_d1vga_control, save_d2vga_control, save_vga_render_control;

    save_seprom_cntl1 = RHDRegRead(rhdPtr, SEPROM_CNTL1);
    save_gpiopad_en = RHDRegRead(rhdPtr, GPIOPAD_EN);
    save_gpiopad_a = RHDRegRead(rhdPtr, GPIOPAD_A);
    save_gpiopad_mask = RHDRegRead(rhdPtr, GPIOPAD_MASK);
    save_viph_cntl = RHDRegRead(rhdPtr, VIPH_CONTROL);
    save_bus_cntl = RHDRegRead(rhdPtr, BUS_CNTL);
    save_d1vga_control = RHDRegRead(rhdPtr, D1VGA_CONTROL);
    save_d2vga_control = RHDRegRead(rhdPtr, D2VGA_CONTROL);
    save_vga_render_control = RHDRegRead(rhdPtr, VGA_RENDER_CONTROL);
    /* Set SPI ROM prescale value to change the SCK period */
    RHDRegMask(rhdPtr, SEPROM_CNTL1, 0x0C << 24, SCK_PRESCALE);
    /* Let chip control GPIO pads - this is the default state after power up */
    RHDRegWrite(rhdPtr, GPIOPAD_EN, 0);
    RHDRegWrite(rhdPtr, GPIOPAD_A, 0);
    /* Put GPIO pads in read mode */
    RHDRegWrite(rhdPtr, GPIOPAD_MASK, 0);
    /* Disable VIP Host port */
    RHDRegMask(rhdPtr, VIPH_CONTROL, 0, VIPH_EN);
    /* Enable BIOS ROM */
    RHDRegMask(rhdPtr, BUS_CNTL, 0, BIOS_ROM_DIS);
    /* Disable VGA and select extended timings */
    RHDRegMask(rhdPtr, D1VGA_CONTROL, 0,
	       D1VGA_MODE_ENABLE | D1VGA_TIMING_SELECT);
    RHDRegMask(rhdPtr, D2VGA_CONTROL, 0,
	       D2VGA_MODE_ENABLE | D2VGA_TIMING_SELECT);
    RHDRegMask(rhdPtr, VGA_RENDER_CONTROL, 0, VGA_VSTATUS_CNTL);

    ret = rhdDoReadPCIBios(rhdPtr, ptr);

    RHDRegWrite(rhdPtr, SEPROM_CNTL1, save_seprom_cntl1);
    RHDRegWrite(rhdPtr, GPIOPAD_EN, save_gpiopad_en);
    RHDRegWrite(rhdPtr, GPIOPAD_A, save_gpiopad_a);
    RHDRegWrite(rhdPtr, GPIOPAD_MASK, save_gpiopad_mask);
    RHDRegWrite(rhdPtr, VIPH_CONTROL, save_viph_cntl);
    RHDRegWrite(rhdPtr, BUS_CNTL, save_bus_cntl);
    RHDRegWrite(rhdPtr, D1VGA_CONTROL, save_d1vga_control);
    RHDRegWrite(rhdPtr, D2VGA_CONTROL, save_d2vga_control);
    RHDRegWrite(rhdPtr, VGA_RENDER_CONTROL, save_vga_render_control);

    return ret;
}

/*
 *
 */
static unsigned int
rhdR6XXDoReadPCIBios(RHDPtr rhdPtr, unsigned char **ptr)
{
    unsigned int ret;
    CARD32 save_600;

    save_600 = RHDRegRead(rhdPtr, 0x600);
    RHDRegMask(rhdPtr, 0x600, 0x02000000, 0x02000000);

    ret = rhdDoReadPCIBios(rhdPtr, ptr);

    RHDRegWrite(rhdPtr, 0x600, save_600);

    return ret;
}

/*
 *
 */
unsigned int
RHDReadPCIBios(RHDPtr rhdPtr, unsigned char **ptr)
{
    if (rhdPtr->ChipSet < RHD_R600)
	return rhdR5XXDoReadPCIBios(rhdPtr, ptr);
    else
	return rhdR6XXDoReadPCIBios(rhdPtr, ptr);
}

/*
 *
 */
static void
rhdGetIGPNorthBridgeInfo(RHDPtr rhdPtr)
{
    switch (rhdPtr->ChipSet) {
	case RHD_RS600:
	    break;
	case RHD_RS690:
	case RHD_RS740:
#ifdef XSERVER_LIBPCIACCESS
	    rhdPtr->NBPciInfo = pci_device_find_by_slot(0,0,0,0);
#else
	    rhdPtr->NBPciTag = pciTag(0,0,0);
#endif
	    break;
    }

}
