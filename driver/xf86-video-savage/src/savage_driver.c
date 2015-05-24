/*
 * Copyright (C) 1994-2000 The XFree86 Project, Inc.  All Rights Reserved.
 * Copyright (c) 2003-2006, X.Org Foundation
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
 * FITESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */

/**
 * \file savage_driver.c
 * 
 * \author Tim Roberts <timr@probo.com>
 * \author Ani Joshi <ajoshi@unixbox.com>
 *
 * \todo Add credits for the 3.3.x authors.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <unistd.h>
#include <errno.h>

#include "shadowfb.h"

#include "globals.h"
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif


#include "xf86xv.h"
#include "xf86Modes.h"

#include "savage_driver.h"
#include "savage_pciids.h"
#include "savage_regs.h"
#include "savage_bci.h"
#include "savage_streams.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86RAC.h"
#endif

#define TRANSPARENCY_KEY 0xff;

#ifdef SAVAGEDRI
#define _XF86DRI_SERVER_
#include "savage_dri.h"
#include "savage_sarea.h"
#endif

/*
 * prototypes
 */
static void SavageEnableMMIO(ScrnInfoPtr pScrn);
static void SavageDisableMMIO(ScrnInfoPtr pScrn);

static const OptionInfoRec * SavageAvailableOptions(int chipid, int busid);
static void SavageIdentify(int flags);
#ifdef XSERVER_LIBPCIACCESS
static Bool SavagePciProbe(DriverPtr drv, int entity_num,
			   struct pci_device *dev, intptr_t match_data);
#else
static Bool SavageProbe(DriverPtr drv, int flags);
static int LookupChipID(PciChipsets* pset, int ChipID);
#endif
static Bool SavagePreInit(ScrnInfoPtr pScrn, int flags);

static Bool SavageEnterVT(VT_FUNC_ARGS_DECL);
static void SavageLeaveVT(VT_FUNC_ARGS_DECL);
static void SavageSave(ScrnInfoPtr pScrn);
static void SavageWriteMode(ScrnInfoPtr pScrn, vgaRegPtr, SavageRegPtr, Bool);

static void SavageInitStatus(ScrnInfoPtr pScrn);
static void SavageInitShadowStatus(ScrnInfoPtr pScrn);

static Bool SavageScreenInit(SCREEN_INIT_ARGS_DECL);
static int SavageInternalScreenInit(ScreenPtr pScreen);
static ModeStatus SavageValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode,
				  Bool verbose, int flags);

void SavageDGAInit(ScreenPtr);
static Bool SavageMapMem(ScrnInfoPtr pScrn);
static void SavageUnmapMem(ScrnInfoPtr pScrn, int All);
static Bool SavageModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
static Bool SavageCloseScreen(CLOSE_SCREEN_ARGS_DECL);
static Bool SavageSaveScreen(ScreenPtr pScreen, int mode);
static void SavageLoadPalette(ScrnInfoPtr pScrn, int numColors,
			      int *indicies, LOCO *colors,
			      VisualPtr pVisual);
static void SavageLoadPaletteSavage4(ScrnInfoPtr pScrn, int numColors,
			      int *indicies, LOCO *colors,
			      VisualPtr pVisual);
static void SavageUpdateKey(ScrnInfoPtr pScrn, int r, int g, int b);
static void SavageCalcClock(long freq, int min_m, int min_n1, int max_n1,
			   int min_n2, int max_n2, long freq_min,
			   long freq_max, unsigned int *mdiv,
			   unsigned int *ndiv, unsigned int *r);
void SavageGEReset(ScrnInfoPtr pScrn, int from_timeout, int line, char *file);
void SavagePrintRegs(ScrnInfoPtr pScrn);
static void SavageDPMS(ScrnInfoPtr pScrn, int mode, int flags);
static Bool SavageDDC1(ScrnInfoPtr pScrn);
static unsigned int SavageDDC1Read(ScrnInfoPtr pScrn);
static void SavageProbeDDC(ScrnInfoPtr pScrn, int index);
static void SavageGetTvMaxSize(SavagePtr psav);
static Bool SavagePanningCheck(ScrnInfoPtr pScrn, DisplayModePtr pMode);
#ifdef SAVAGEDRI
static Bool SavageCheckAvailableRamFor3D(ScrnInfoPtr pScrn);
#endif
static void SavageResetStreams(ScrnInfoPtr pScrn);

extern ScrnInfoPtr gpScrn;

#define iabs(a)	((int)(a)>0?(a):(-(a)))

/*#define TRACEON*/
#ifdef TRACEON
#define TRACE(prms)	ErrorF prms
#else
#define TRACE(prms)  
#endif

int gSavageEntityIndex = -1;

#ifdef XSERVER_LIBPCIACCESS
#define SAVAGE_DEVICE_MATCH(d, i) \
    { 0x5333, (d), PCI_MATCH_ANY, PCI_MATCH_ANY, 0, 0, (i) }

static const struct pci_id_match savage_device_match[] = {
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SAVAGE4,         S3_SAVAGE4),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SAVAGE3D,        S3_SAVAGE3D),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SAVAGE3D_MV,     S3_SAVAGE3D),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SAVAGE2000,      S3_SAVAGE2000),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SAVAGE_MX_MV,    S3_SAVAGE_MX),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SAVAGE_MX,       S3_SAVAGE_MX),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SAVAGE_IX_MV,    S3_SAVAGE_MX),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SAVAGE_IX,       S3_SAVAGE_MX),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_PROSAVAGE_PM,    S3_PROSAVAGE),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_PROSAVAGE_KM,    S3_PROSAVAGE),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_S3TWISTER_P,     S3_TWISTER),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_S3TWISTER_K,     S3_TWISTER),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SUPSAV_MX128,    S3_SUPERSAVAGE),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SUPSAV_MX64,     S3_SUPERSAVAGE),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SUPSAV_MX64C,    S3_SUPERSAVAGE),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SUPSAV_IX128SDR, S3_SUPERSAVAGE),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SUPSAV_IX128DDR, S3_SUPERSAVAGE),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SUPSAV_IX64SDR,  S3_SUPERSAVAGE),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SUPSAV_IX64DDR,  S3_SUPERSAVAGE),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SUPSAV_IXCSDR,   S3_SUPERSAVAGE),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_SUPSAV_IXCDDR,   S3_SUPERSAVAGE),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_PROSAVAGE_DDR,   S3_PROSAVAGEDDR),
    SAVAGE_DEVICE_MATCH(PCI_CHIP_PROSAVAGE_DDRK,  S3_PROSAVAGEDDR),

    { 0, 0, 0 },
};
#endif

/* Supported chipsets */

static SymTabRec SavageChips[] = {
    { PCI_CHIP_SAVAGE4,		"Savage4" },
    { PCI_CHIP_SAVAGE3D,	"Savage3D" },
    { PCI_CHIP_SAVAGE3D_MV,	"Savage3D-MV" },
    { PCI_CHIP_SAVAGE2000,	"Savage2000" },
    { PCI_CHIP_SAVAGE_MX_MV,	"Savage/MX-MV" },
    { PCI_CHIP_SAVAGE_MX,	"Savage/MX" },
    { PCI_CHIP_SAVAGE_IX_MV,	"Savage/IX-MV" },
    { PCI_CHIP_SAVAGE_IX,	"Savage/IX" },
    { PCI_CHIP_PROSAVAGE_PM,	"ProSavage PM133" },
    { PCI_CHIP_PROSAVAGE_KM,	"ProSavage KM133" },
    { PCI_CHIP_S3TWISTER_P,	"Twister PN133" },
    { PCI_CHIP_S3TWISTER_K,	"Twister KN133" },
    { PCI_CHIP_SUPSAV_MX128,	"SuperSavage/MX 128" },
    { PCI_CHIP_SUPSAV_MX64,	"SuperSavage/MX 64" },
    { PCI_CHIP_SUPSAV_MX64C,	"SuperSavage/MX 64C" },
    { PCI_CHIP_SUPSAV_IX128SDR,	"SuperSavage/IX 128" },
    { PCI_CHIP_SUPSAV_IX128DDR,	"SuperSavage/IX 128" },
    { PCI_CHIP_SUPSAV_IX64SDR,	"SuperSavage/IX 64" },
    { PCI_CHIP_SUPSAV_IX64DDR,	"SuperSavage/IX 64" },
    { PCI_CHIP_SUPSAV_IXCSDR,	"SuperSavage/IXC 64" },
    { PCI_CHIP_SUPSAV_IXCDDR,	"SuperSavage/IXC 64" },
    { PCI_CHIP_PROSAVAGE_DDR,	"ProSavage DDR" },
    { PCI_CHIP_PROSAVAGE_DDRK,	"ProSavage DDR-K" },
    { -1,			NULL }
};

static SymTabRec SavageChipsets[] = {
    { S3_SAVAGE3D,	"Savage3D" },
    { S3_SAVAGE4,	"Savage4" },
    { S3_SAVAGE2000,	"Savage2000" },
    { S3_SAVAGE_MX,	"MobileSavage" },
    { S3_PROSAVAGE,	"ProSavage" },
    { S3_TWISTER,       "Twister"},
    { S3_PROSAVAGEDDR,  "ProSavageDDR"},
    { S3_SUPERSAVAGE,   "SuperSavage" },
    { -1,		NULL }
};

#ifndef XSERVER_LIBPCIACCESS
/* This table maps a PCI device ID to a chipset family identifier. */

static PciChipsets SavagePciChipsets[] = {
    { S3_SAVAGE3D,	PCI_CHIP_SAVAGE3D,	RES_SHARED_VGA },
    { S3_SAVAGE3D,	PCI_CHIP_SAVAGE3D_MV, 	RES_SHARED_VGA },
    { S3_SAVAGE4,	PCI_CHIP_SAVAGE4,	RES_SHARED_VGA },
    { S3_SAVAGE2000,	PCI_CHIP_SAVAGE2000,	RES_SHARED_VGA },
    { S3_SAVAGE_MX,	PCI_CHIP_SAVAGE_MX_MV,	RES_SHARED_VGA },
    { S3_SAVAGE_MX,	PCI_CHIP_SAVAGE_MX,	RES_SHARED_VGA },
    { S3_SAVAGE_MX,	PCI_CHIP_SAVAGE_IX_MV,	RES_SHARED_VGA },
    { S3_SAVAGE_MX,	PCI_CHIP_SAVAGE_IX,	RES_SHARED_VGA },
    { S3_PROSAVAGE,	PCI_CHIP_PROSAVAGE_PM,	RES_SHARED_VGA },
    { S3_PROSAVAGE,	PCI_CHIP_PROSAVAGE_KM,	RES_SHARED_VGA },
    { S3_TWISTER,	PCI_CHIP_S3TWISTER_P,	RES_SHARED_VGA },
    { S3_TWISTER,	PCI_CHIP_S3TWISTER_K,	RES_SHARED_VGA },
    { S3_PROSAVAGEDDR,	PCI_CHIP_PROSAVAGE_DDR,	RES_SHARED_VGA },
    { S3_PROSAVAGEDDR,	PCI_CHIP_PROSAVAGE_DDRK,	RES_SHARED_VGA },
    { S3_SUPERSAVAGE,	PCI_CHIP_SUPSAV_MX128,	RES_SHARED_VGA },
    { S3_SUPERSAVAGE,	PCI_CHIP_SUPSAV_MX64,	RES_SHARED_VGA },
    { S3_SUPERSAVAGE,	PCI_CHIP_SUPSAV_MX64C,	RES_SHARED_VGA },
    { S3_SUPERSAVAGE,	PCI_CHIP_SUPSAV_IX128SDR,	RES_SHARED_VGA },
    { S3_SUPERSAVAGE,	PCI_CHIP_SUPSAV_IX128DDR,	RES_SHARED_VGA },
    { S3_SUPERSAVAGE,	PCI_CHIP_SUPSAV_IX64SDR,	RES_SHARED_VGA },
    { S3_SUPERSAVAGE,	PCI_CHIP_SUPSAV_IX64DDR,	RES_SHARED_VGA },
    { S3_SUPERSAVAGE,	PCI_CHIP_SUPSAV_IXCSDR,	RES_SHARED_VGA },
    { S3_SUPERSAVAGE,	PCI_CHIP_SUPSAV_IXCDDR,	RES_SHARED_VGA },
    { -1,		-1,			RES_UNDEFINED }
};
#endif

typedef enum {
     OPTION_PCI_BURST
    ,OPTION_PCI_RETRY
    ,OPTION_NOACCEL
    ,OPTION_ACCELMETHOD
    ,OPTION_LCD_CENTER
    ,OPTION_LCDCLOCK
    ,OPTION_MCLK
    ,OPTION_REFCLK
    ,OPTION_SHOWCACHE
    ,OPTION_SWCURSOR
    ,OPTION_HWCURSOR
    ,OPTION_SHADOW_FB
    ,OPTION_ROTATE
    ,OPTION_USEBIOS
    ,OPTION_SHADOW_STATUS
    ,OPTION_CRT_ONLY
    ,OPTION_TV_ON
    ,OPTION_TV_PAL
    ,OPTION_FORCE_INIT
    ,OPTION_OVERLAY
    ,OPTION_T_KEY
    ,OPTION_DISABLE_XVMC
    ,OPTION_DISABLE_TILE
    ,OPTION_DISABLE_COB
    ,OPTION_BCI_FOR_XV
    ,OPTION_DVI
    ,OPTION_BUS_TYPE
    ,OPTION_DMA_TYPE
    ,OPTION_DMA_MODE
    ,OPTION_AGP_MODE
    ,OPTION_AGP_SIZE
    ,OPTION_DRI
    ,OPTION_IGNORE_EDID
    ,OPTION_AGP_FOR_XV
} SavageOpts;


static const OptionInfoRec SavageOptions[] =
{
    { OPTION_NOACCEL,	"NoAccel",	OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_ACCELMETHOD, "AccelMethod", OPTV_STRING,	{0}, FALSE },
    { OPTION_HWCURSOR,	"HWCursor",	OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_SWCURSOR,	"SWCursor",	OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_SHADOW_FB,	"ShadowFB",	OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_ROTATE,	"Rotate",	OPTV_ANYSTR, {0}, FALSE },
    { OPTION_USEBIOS,	"UseBIOS",	OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_LCDCLOCK,	"LCDClock",	OPTV_FREQ,    {0}, FALSE },
    { OPTION_SHADOW_STATUS, "ShadowStatus", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_CRT_ONLY,  "CrtOnly",      OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_TV_ON,     "TvOn",         OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_TV_PAL,    "PAL",          OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_FORCE_INIT,"ForceInit",    OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_OVERLAY,	"Overlay",	OPTV_ANYSTR, {0}, FALSE },
    { OPTION_T_KEY,	"TransparencyKey",	OPTV_ANYSTR, {0}, FALSE },
    { OPTION_FORCE_INIT,   "ForceInit",   OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DISABLE_XVMC, "DisableXVMC", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DISABLE_TILE, "DisableTile", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DISABLE_COB,  "DisableCOB",  OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_BCI_FOR_XV,   "BCIforXv",    OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DVI,          "DVI",       OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_IGNORE_EDID,  "IgnoreEDID",  OPTV_BOOLEAN, {0}, FALSE },
#ifdef SAVAGEDRI
    { OPTION_BUS_TYPE,	"BusType",	OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_DMA_TYPE,	"DmaType",	OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_DMA_MODE,  "DmaMode",	OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_AGP_MODE,	"AGPMode",	OPTV_INTEGER, {0}, FALSE },
    { OPTION_AGP_SIZE,	"AGPSize",	OPTV_INTEGER, {0}, FALSE },
    { OPTION_DRI,       "DRI",          OPTV_BOOLEAN, {0}, TRUE },
    { OPTION_AGP_FOR_XV,   "AGPforXv",    OPTV_BOOLEAN, {0}, FALSE },
#endif
    { -1,		NULL,		OPTV_NONE,    {0}, FALSE }
};

_X_EXPORT DriverRec SAVAGE =
{
    SAVAGE_VERSION,
    SAVAGE_DRIVER_NAME,
    SavageIdentify,
#ifdef XSERVER_LIBPCIACCESS
    NULL,
#else
    SavageProbe,
#endif
    SavageAvailableOptions,
    NULL,
    0,
    NULL,

#ifdef XSERVER_LIBPCIACCESS
    savage_device_match,
    SavagePciProbe
#endif
};

#ifdef XFree86LOADER

static MODULESETUPPROTO(SavageSetup);

static XF86ModuleVersionInfo SavageVersRec = {
    "savage",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    SAVAGE_VERSION_MAJOR, SAVAGE_VERSION_MINOR, SAVAGE_PATCHLEVEL,
    ABI_CLASS_VIDEODRV,
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_VIDEODRV,
    {0, 0, 0, 0}
};

_X_EXPORT XF86ModuleData savageModuleData = {
    &SavageVersRec,
    SavageSetup,
    NULL
};

static pointer SavageSetup(pointer module, pointer opts, int *errmaj,
			   int *errmin)
{
    static Bool setupDone = FALSE;

    if (!setupDone) {
	setupDone = TRUE;
	xf86AddDriver(&SAVAGE, module, 1);
	return (pointer) 1;
    } else {
	if (errmaj)
	    *errmaj = LDR_ONCEONLY;
	return NULL;
    }
}

#endif /* XFree86LOADER */

static SavageEntPtr SavageEntPriv(ScrnInfoPtr pScrn)
{
    DevUnion     *pPriv;
    SavagePtr  psav   = SAVPTR(pScrn);
    pPriv = xf86GetEntityPrivate(psav->pEnt->index,
                                 gSavageEntityIndex);
    return pPriv->ptr;
}


/*
 * I'd rather have these wait macros be inline, but S3 has made it 
 * darned near impossible.  The bit fields are in a different place in
 * all three families, the status register has a different address in the
 * three families, and even the idle vs busy sense flipped in the Sav2K.
 */

static void
ResetBCI2K( SavagePtr psav )
{
    CARD32 cob = INREG( 0x48c18 );
    /* if BCI is enabled and BCI is busy... */

    if( 
	(cob & 0x00000008) &&
	! (ALT_STATUS_WORD0 & 0x00200000)
    )
    {
	ErrorF( "Resetting BCI, stat = %08lx...\n",
		(unsigned long) ALT_STATUS_WORD0);
	/* Turn off BCI */
	OUTREG( 0x48c18, cob & ~8 );
	usleep(10000);
	/* Turn it back on */
	OUTREG( 0x48c18, cob );
	usleep(10000);
    }
}

static Bool
ShadowWait( SavagePtr psav )
{
    BCI_GET_PTR;
    int loop = 0;

    if( !psav->NoPCIRetry )
	return 0;

    psav->ShadowCounter = (psav->ShadowCounter + 1) & 0xffff;
    if (psav->ShadowCounter == 0)
	psav->ShadowCounter++; /* 0 is reserved for the BIOS
				  to avoid confusion in the DRM */
    BCI_SEND( psav->dwBCIWait2DIdle );
    BCI_SEND( 0x98000000 + psav->ShadowCounter );

    while(
	(int)(psav->ShadowVirtual[psav->eventStatusReg] & 0xffff) !=
	psav->ShadowCounter && (loop++ < MAXLOOP)
    )
	;

    return loop >= MAXLOOP;
}

static Bool
ShadowWaitQueue( SavagePtr psav, int v )
{
    int loop = 0;
    CARD32 slots = MAXFIFO - v;

    if (slots >= psav->bciThresholdHi)
	slots = psav->bciThresholdHi;
    else
	return ShadowWait( psav );

    /* Savage 2000 reports only entries filled in the COB, not the on-chip
     * queue. Also it reports in qword units instead of dwords. */
    if (psav->Chipset == S3_SAVAGE2000)
	slots = (slots - 32) / 4;

    while( ((psav->ShadowVirtual[0] & psav->bciUsedMask) >= slots) && (loop++ < MAXLOOP))
	;

    return loop >= MAXLOOP;
}

/* Wait until "v" queue entries are free */

static int
WaitQueue3D( SavagePtr psav, int v )
{
    int loop = 0;
    CARD32 slots = MAXFIFO - v;

    mem_barrier();
    if( psav->ShadowVirtual )
    {
	psav->WaitQueue = ShadowWaitQueue;
	return ShadowWaitQueue(psav, v);
    }
    else
    {
	loop &= STATUS_WORD0;
	while( ((STATUS_WORD0 & 0x0000ffff) > slots) && (loop++ < MAXLOOP))
	    ;
    }
    return loop >= MAXLOOP;
}

static int
WaitQueue4( SavagePtr psav, int v )
{
    int loop = 0;
    CARD32 slots = MAXFIFO - v;

    if( !psav->NoPCIRetry )
	return 0;
    mem_barrier();
    if( psav->ShadowVirtual )
    {
	psav->WaitQueue = ShadowWaitQueue;
	return ShadowWaitQueue(psav, v);
    }
    else
	while( ((ALT_STATUS_WORD0 & 0x001fffff) > slots) && (loop++ < MAXLOOP));
    return loop >= MAXLOOP;
}

static int
WaitQueue2K( SavagePtr psav, int v )
{
    int loop = 0;
    CARD32 slots = (MAXFIFO - v) / 4;

    if( !psav->NoPCIRetry )
	return 0;
    mem_barrier();
    if( psav->ShadowVirtual )
    {
	psav->WaitQueue = ShadowWaitQueue;
	return ShadowWaitQueue(psav, v);
    }
    else
	while( ((ALT_STATUS_WORD0 & 0x000fffff) > slots) && (loop++ < MAXLOOP))
	    ;
    if( loop >= MAXLOOP )
	ResetBCI2K(psav);
    return loop >= MAXLOOP;
}

/* Wait until GP is idle and queue is empty */

static int
WaitIdleEmpty3D(SavagePtr psav)
{
    int loop = 0;
    mem_barrier();
    if( psav->ShadowVirtual )
    {
	psav->WaitIdleEmpty = ShadowWait;
	return ShadowWait(psav);
    }
    loop &= STATUS_WORD0;
    while( ((STATUS_WORD0 & 0x0008ffff) != 0x80000) && (loop++ < MAXLOOP) );
    return loop >= MAXLOOP;
}

static int
WaitIdleEmpty4(SavagePtr psav)
{
    int loop = 0;
    mem_barrier();
    if( psav->ShadowVirtual )
    {
	psav->WaitIdleEmpty = ShadowWait;
	return ShadowWait(psav);
    }
	/* which is right?*/
    /*while( ((ALT_STATUS_WORD0 & 0x00a1ffff) != 0x00a00000) && (loop++ < MAXLOOP) );*/ /* tim */
    while (((ALT_STATUS_WORD0 & 0x00e1ffff) != 0x00e00000) && (loop++ < MAXLOOP)); /* S3 */
    return loop >= MAXLOOP;
}

static int
WaitIdleEmpty2K(SavagePtr psav)
{
    int loop = 0;
    mem_barrier();
    if( psav->ShadowVirtual )
    {
	psav->WaitIdleEmpty = ShadowWait;
	return ShadowWait(psav);
    }
    loop &= ALT_STATUS_WORD0;
    while( ((ALT_STATUS_WORD0 & 0x009fffff) != 0) && (loop++ < MAXLOOP) );
    if( loop >= MAXLOOP )
	ResetBCI2K(psav);
    return loop >= MAXLOOP;
}

/* Wait until GP is idle */

static int
WaitIdle3D(SavagePtr psav)
{
    int loop = 0;
    mem_barrier();
    if( psav->ShadowVirtual )
    {
	psav->WaitIdle = ShadowWait;
	return ShadowWait(psav);
    }
    while( (!(STATUS_WORD0 & 0x00080000)) && (loop++ < MAXLOOP) );
    return loop >= MAXLOOP;
}

static int
WaitIdle4(SavagePtr psav)
{
    int loop = 0;
    mem_barrier();
    if( psav->ShadowVirtual )
    {
	psav->WaitIdle = ShadowWait;
	return ShadowWait(psav);
    }
	/* which is right?*/
    /*while( (!(ALT_STATUS_WORD0 & 0x00800000)) && (loop++ < MAXLOOP) );*/ /* tim */
    while (((ALT_STATUS_WORD0 & 0x00E00000)!=0x00E00000) && (loop++ < MAXLOOP)); /* S3 */
    return loop >= MAXLOOP;
}

static int
WaitIdle2K(SavagePtr psav)
{
    int loop = 0;
    mem_barrier();
    if( psav->ShadowVirtual )
    {
	psav->WaitIdle = ShadowWait;
	return ShadowWait(psav);
    }
    loop &= ALT_STATUS_WORD0;
    while( (ALT_STATUS_WORD0 & 0x00900000) && (loop++ < MAXLOOP) );
    return loop >= MAXLOOP;
}


static Bool SavageGetRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate)
	return TRUE;

    pScrn->driverPrivate = xnfcalloc(sizeof(SavageRec), 1);
    return TRUE;
}


static void SavageFreeRec(ScrnInfoPtr pScrn)
{
    TRACE(( "SavageFreeRec(%p)\n", pScrn->driverPrivate ));
    if (!pScrn->driverPrivate)
	return;
    SavageUnmapMem(pScrn, 1);
    free(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
}


static const OptionInfoRec * SavageAvailableOptions(int chipid, int busid)
{
    return SavageOptions;
}


static void SavageIdentify(int flags)
{
    xf86PrintChipsets("SAVAGE", 
		      "driver (version " SAVAGE_DRIVER_VERSION ") for S3 Savage chipsets",
		      SavageChips);
}


#ifdef XSERVER_LIBPCIACCESS
static Bool SavagePciProbe(DriverPtr drv, int entity_num,
			   struct pci_device *dev, intptr_t match_data)
{
    ScrnInfoPtr pScrn;


    if ((match_data < S3_SAVAGE3D) || (match_data > S3_SAVAGE2000)) {
 	return FALSE;
    }

    pScrn = xf86ConfigPciEntity(NULL, 0, entity_num, NULL,
				NULL, NULL, NULL, NULL, NULL);
    if (pScrn != NULL) {
	EntityInfoPtr pEnt;
	SavagePtr psav;


	pScrn->driverVersion = SAVAGE_VERSION;
	pScrn->driverName = SAVAGE_DRIVER_NAME;
	pScrn->name = "SAVAGE";
	pScrn->Probe = NULL;
	pScrn->PreInit = SavagePreInit;
	pScrn->ScreenInit = SavageScreenInit;
	pScrn->SwitchMode = SavageSwitchMode;
	pScrn->AdjustFrame = SavageAdjustFrame;
	pScrn->EnterVT = SavageEnterVT;
	pScrn->LeaveVT = SavageLeaveVT;
	pScrn->FreeScreen = NULL;
	pScrn->ValidMode = SavageValidMode;

	if (!SavageGetRec(pScrn))
	    return FALSE;

	psav = SAVPTR(pScrn);

	psav->PciInfo = dev;
	psav->Chipset = match_data;

	pEnt = xf86GetEntityInfo(entity_num);

	/* MX, IX, SuperSavage cards support Dual-Head, mark the entity as
	 * sharable.
	 */
	if (pEnt->chipset == S3_SAVAGE_MX || pEnt->chipset == S3_SUPERSAVAGE) {
	    DevUnion   *pPriv;
	    SavageEntPtr pSavageEnt;

	    xf86SetEntitySharable(entity_num);

	    if (gSavageEntityIndex == -1)
	        gSavageEntityIndex = xf86AllocateEntityPrivateIndex();

	    pPriv = xf86GetEntityPrivate(pEnt->index, gSavageEntityIndex);
	    if (!pPriv->ptr) {
		int j;
		int instance = xf86GetNumEntityInstances(pEnt->index);

		for (j = 0; j < instance; j++)
		    xf86SetEntityInstanceForScreen(pScrn, pEnt->index, j);

		pPriv->ptr = xnfcalloc(sizeof(SavageEntRec), 1);
		pSavageEnt = pPriv->ptr;
		pSavageEnt->HasSecondary = FALSE;
	    } else {
		pSavageEnt = pPriv->ptr;
		pSavageEnt->HasSecondary = TRUE;
	    }
	}
    }

    return (pScrn != NULL);
}

#else

static Bool SavageProbe(DriverPtr drv, int flags)
{
    int i;
    GDevPtr *devSections = NULL;
    int *usedChips;
    int numDevSections;
    int numUsed;
    Bool foundScreen = FALSE;

    /* sanity checks */
    if ((numDevSections = xf86MatchDevice("savage", &devSections)) <= 0)
	return FALSE;
    if (xf86GetPciVideoInfo() == NULL) {
        if (devSections)
	    free(devSections);
        return FALSE;
    }

    numUsed = xf86MatchPciInstances("SAVAGE", PCI_VENDOR_S3,
				    SavageChipsets, SavagePciChipsets,
				    devSections, numDevSections, drv,
				    &usedChips);
    if (devSections)
	free(devSections);
    devSections = NULL;
    if (numUsed <= 0)
	return FALSE;

    if (flags & PROBE_DETECT)
	foundScreen = TRUE;
    else
	for (i=0; i<numUsed; i++) {
            EntityInfoPtr pEnt = xf86GetEntityInfo(usedChips[i]);;
            ScrnInfoPtr pScrn = xf86ConfigPciEntity(NULL, 0, usedChips[i],
						    NULL, RES_SHARED_VGA, 
						    NULL, NULL, NULL, NULL);

            if (pScrn != NULL) {
		SavagePtr psav;

 	        pScrn->driverVersion = SAVAGE_VERSION;
	        pScrn->driverName = SAVAGE_DRIVER_NAME;
	        pScrn->name = "SAVAGE";
	        pScrn->Probe = SavageProbe;
	        pScrn->PreInit = SavagePreInit;
	        pScrn->ScreenInit = SavageScreenInit;
	        pScrn->SwitchMode = SavageSwitchMode;
	        pScrn->AdjustFrame = SavageAdjustFrame;
	        pScrn->EnterVT = SavageEnterVT;
	        pScrn->LeaveVT = SavageLeaveVT;
	        pScrn->FreeScreen = NULL;
	        pScrn->ValidMode = SavageValidMode;
	        foundScreen = TRUE;

		if (!SavageGetRec(pScrn))
		    return FALSE;

		psav = SAVPTR(pScrn);

		psav->PciInfo = xf86GetPciInfoForEntity(pEnt->index);
		if (pEnt->device->chipset && *pEnt->device->chipset) {
		    psav->Chipset = xf86StringToToken(SavageChipsets,
						      pEnt->device->chipset);
		} else if (pEnt->device->chipID >= 0) {
		    psav->Chipset = LookupChipID(SavagePciChipsets,
						 pEnt->device->chipID);
		} else {
		    psav->Chipset = LookupChipID(SavagePciChipsets, 
						 psav->PciInfo->chipType);
		}
	    }

            pEnt = xf86GetEntityInfo(usedChips[i]);

            /* MX, IX, SuperSavage cards support Dual-Head, mark the entity as sharable*/
            if(pEnt->chipset == S3_SAVAGE_MX || pEnt->chipset == S3_SUPERSAVAGE)
            {
		DevUnion   *pPriv;
		SavageEntPtr pSavageEnt;

		xf86SetEntitySharable(usedChips[i]);

		if (gSavageEntityIndex == -1)
		    gSavageEntityIndex = xf86AllocateEntityPrivateIndex();

		pPriv = xf86GetEntityPrivate(pEnt->index,
					     gSavageEntityIndex);

		if (!pPriv->ptr) {
		    int j;
		    int instance = xf86GetNumEntityInstances(pEnt->index);

		    for (j = 0; j < instance; j++)
			xf86SetEntityInstanceForScreen(pScrn, pEnt->index, j);

		    pPriv->ptr = xnfcalloc(sizeof(SavageEntRec), 1);
		    pSavageEnt = pPriv->ptr;
		    pSavageEnt->HasSecondary = FALSE;
		} else {
		    pSavageEnt = pPriv->ptr;
		    pSavageEnt->HasSecondary = TRUE;
		}
	    }
	    free(pEnt);
	}


    free(usedChips);
    return foundScreen;
}

static int LookupChipID( PciChipsets* pset, int ChipID )
{
    /* Is there a function to do this for me? */
    while( pset->numChipset >= 0 )
    {
        if( pset->PCIid == ChipID )
	    return pset->numChipset;
	pset++;
    }

    return -1;
}
#endif

static void SavageDoDDC(ScrnInfoPtr pScrn)
{
    SavagePtr psav= SAVPTR(pScrn);
    pointer ddc;

    /* Do the DDC dance. */ /* S3/VIA's DDC code */
    ddc = xf86LoadSubModule(pScrn, "ddc");
    if (ddc) {
        switch( psav->Chipset ) {
            case S3_SAVAGE3D:
            case S3_SAVAGE_MX:
            case S3_SUPERSAVAGE:
	    case S3_SAVAGE2000:
		psav->DDCPort = 0xAA;
                psav->I2CPort = 0xA0;
                break;

            case S3_SAVAGE4:
            case S3_PROSAVAGE:
            case S3_TWISTER:
            case S3_PROSAVAGEDDR:
                psav->DDCPort = 0xB1;
                psav->I2CPort = 0xA0;
                break;
        }

        if (!SavageDDC1(pScrn)) {
            /* DDC1 failed,switch to DDC2 */
            if (xf86LoadSubModule(pScrn, "i2c")) {
                if (SavageI2CInit(pScrn)) {
                    unsigned char tmp;
                    xf86MonPtr pMon;
                    
                    InI2CREG(tmp,psav->DDCPort);
                    OutI2CREG(tmp | 0x13,psav->DDCPort);
                    pMon = xf86PrintEDID(xf86DoEDID_DDC2(XF86_SCRN_ARG(pScrn),psav->I2C));
                    if (!psav->IgnoreEDID) xf86SetDDCproperties(pScrn, pMon);
                    OutI2CREG(tmp,psav->DDCPort);
                }
            }
        }
    }
}

/* Copied from ddc/Property.c via nv */
static DisplayModePtr
SavageModesAdd(DisplayModePtr Modes, DisplayModePtr Additions)
{
    if (!Modes) {
        if (Additions)
            return Additions;
        else
            return NULL;
    }

    if (Additions) {
        DisplayModePtr Mode = Modes;

        while (Mode->next)
            Mode = Mode->next;
        
        Mode->next = Additions;
        Additions->prev = Mode;
    }

    return Modes;
}

/* borrowed from nv */
static void
SavageAddPanelMode(ScrnInfoPtr pScrn)
{
    SavagePtr psav= SAVPTR(pScrn);
    DisplayModePtr  Mode  = NULL;

    Mode = xf86CVTMode(psav->PanelX, psav->PanelY, 60.00, TRUE, FALSE);
    Mode->type = M_T_DRIVER | M_T_PREFERRED;
    pScrn->monitor->Modes = SavageModesAdd(pScrn->monitor->Modes, Mode);

    if ((pScrn->monitor->nHsync == 0) && 
        (pScrn->monitor->nVrefresh == 0)) {
	if (!Mode->HSync)
	    Mode->HSync = ((float) Mode->Clock ) / ((float) Mode->HTotal);
	if (!Mode->VRefresh)
	    Mode->VRefresh = (1000.0 * ((float) Mode->Clock)) /
		((float) (Mode->HTotal * Mode->VTotal));

	if (Mode->HSync < pScrn->monitor->hsync[0].lo)
	    pScrn->monitor->hsync[0].lo = Mode->HSync;
	if (Mode->HSync > pScrn->monitor->hsync[0].hi)
	    pScrn->monitor->hsync[0].hi = Mode->HSync;
	if (Mode->VRefresh < pScrn->monitor->vrefresh[0].lo)
	    pScrn->monitor->vrefresh[0].lo = Mode->VRefresh;
	if (Mode->VRefresh > pScrn->monitor->vrefresh[0].hi)
	    pScrn->monitor->vrefresh[0].hi = Mode->VRefresh;

	pScrn->monitor->nHsync = 1;
	pScrn->monitor->nVrefresh = 1;
    }
}

static void SavageGetPanelInfo(ScrnInfoPtr pScrn)
{
    SavagePtr psav= SAVPTR(pScrn);
    vgaHWPtr hwp;
    unsigned char cr6b;
    int panelX, panelY;
    char * sTechnology = "Unknown";
    enum ACTIVE_DISPLAYS { /* These are the bits in CR6B */
	ActiveCRT = 0x01,
	ActiveLCD = 0x02,
	ActiveTV = 0x04,
	ActiveCRT2 = 0x20,
	ActiveDUO = 0x80
    };

    hwp = VGAHWPTR(pScrn);

    /* Check LCD panel information */

    cr6b = hwp->readCrtc( hwp, 0x6b );

    panelX = (hwp->readSeq(hwp, 0x61) + 
	    ((hwp->readSeq(hwp, 0x66) & 0x02) << 7) + 1) * 8;
    panelY = hwp->readSeq(hwp, 0x69) + 
	    ((hwp->readSeq(hwp, 0x6e) & 0x70) << 4) + 1;


	/* OK, I admit it.  I don't know how to limit the max dot clock
	 * for LCD panels of various sizes.  I thought I copied the formula
	 * from the BIOS, but many users have informed me of my folly.
	 *
	 * Instead, I'll abandon any attempt to automatically limit the 
	 * clock, and add an LCDClock option to XF86Config.  Some day,
	 * I should come back to this.
	 */


    if( (hwp->readSeq( hwp, 0x39 ) & 0x03) == 0 )
    {
	sTechnology = "TFT";
    }
    else if( (hwp->readSeq( hwp, 0x30 ) & 0x01) == 0 )
    {
	sTechnology = "DSTN";
    }
    else
    {
	sTechnology = "STN";
    }

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		   "%dx%d %s LCD panel detected %s\n", 
		   panelX, panelY, sTechnology,
		   cr6b & ActiveLCD ? "and active" : "but not active");

    if( cr6b & ActiveLCD ) {
	    /* If the LCD is active and panel expansion is enabled, */
	    /* we probably want to kill the HW cursor. */

	xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		       "- Limiting video mode to %dx%d\n",
		       panelX, panelY );
	    
	psav->PanelX = panelX;
	psav->PanelY = panelY;

	do {
	    DisplayModePtr native = xf86CVTMode(panelX, panelY, 60.0, 0, 0);
	    if (!native)
		break;

	    if (!pScrn->monitor->nHsync) {
		pScrn->monitor->nHsync = 1;
		pScrn->monitor->hsync[0].lo = 31.5;
		pScrn->monitor->hsync[0].hi = (float)native->Clock /
					      (float)native->HTotal;
	    }
	    if (!pScrn->monitor->nVrefresh) {
		pScrn->monitor->nVrefresh = 1;
		pScrn->monitor->vrefresh[0].lo = 56.0;
		pScrn->monitor->vrefresh[0].hi = (float)native->Clock * 1000.0 /
						 (float)native->HTotal /
						 (float)native->VTotal;
	    }
	    if (!pScrn->monitor->maxPixClock)
		pScrn->monitor->maxPixClock = native->Clock;

	    free(native);
	} while (0);

	if( psav->LCDClock > 0.0 )
	{
	    psav->maxClock = psav->LCDClock * 1000.0;
	    xf86DrvMsg( pScrn->scrnIndex, X_CONFIG,
			    "- Limiting dot clock to %1.2f MHz\n",
			    psav->LCDClock );
	}
    } else {
        psav->DisplayType = MT_CRT;
    }
}


static Bool SavagePreInit(ScrnInfoPtr pScrn, int flags)
{
    EntityInfoPtr pEnt;
    SavagePtr psav;
    MessageType from = X_DEFAULT;
    int i;
    ClockRangePtr clockRanges;
    const char *s = NULL;
    unsigned char config1, m, n, n1, n2, sr8, cr66 = 0, tmp;
    int mclk;
    vgaHWPtr hwp;
    int vgaCRIndex, vgaCRReg;
    Bool dvi;

    TRACE(("SavagePreInit(%d)\n", flags));

    gpScrn = pScrn;

    if (flags & PROBE_DETECT) {
	SavageProbeDDC( pScrn, xf86GetEntityInfo(pScrn->entityList[0])->index );
	return TRUE;
    }

    if (!xf86LoadSubModule(pScrn, "vgahw"))
	return FALSE;

    if (!vgaHWGetHWRec(pScrn))
	return FALSE;

#if 0
    /* Here we can alter the number of registers saved and restored by the
     * standard vgaHWSave and Restore routines.
     */
    vgaHWSetRegCounts( pScrn, VGA_NUM_CRTC, VGA_NUM_SEQ, VGA_NUM_GFX, VGA_NUM_ATTR );
#endif

    pScrn->monitor = pScrn->confScreen->monitor;

    /*
     * We support depths of 8, 15, 16 and 24.
     * We support bpp of 8, 16, and 32.
     */

    if (!xf86SetDepthBpp(pScrn, 0, 0, 0, Support32bppFb))
	return FALSE;
    else {
        int requiredBpp;
	int altBpp = 0;

	switch (pScrn->depth) {
	case 8:
	case 16:
	    requiredBpp = pScrn->depth;
	    break;
	case 15:
	    requiredBpp = 16;
	    break;
	case 24:
	    requiredBpp = 32;
	    altBpp = 24;
	    break;

	default:
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Given depth (%d) is not supported by this driver\n",
			pScrn->depth);
	    return FALSE;
	}

	if( 
	    (pScrn->bitsPerPixel != requiredBpp) &&
	    (pScrn->bitsPerPixel != altBpp) 
	) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Depth %d must specify %d bpp; %d was given\n",
		       pScrn->depth, requiredBpp, pScrn->bitsPerPixel );
	    return FALSE;
	}
    }

    xf86PrintDepthBpp(pScrn);

    if (pScrn->depth > 8) {
	rgb zeros = {0, 0, 0};

	if (!xf86SetWeight(pScrn, zeros, zeros))
	    return FALSE;
	else {
	    /* TODO check weight returned is supported */
	    ;
	}
    }

    if (!xf86SetDefaultVisual(pScrn, -1)) {
	return FALSE;
    } else {
	/* We don't currently support DirectColor at 16bpp */
	if (pScrn->bitsPerPixel == 16 && pScrn->defaultVisual != TrueColor) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Given default visual"
		       " (%s) is not supported at depth %d\n",
		       xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
	    return FALSE;
	}
    }

    pScrn->progClock = TRUE;

    if (!SavageGetRec(pScrn))
	return FALSE;
    psav = SAVPTR(pScrn);

    hwp = VGAHWPTR(pScrn);
    vgaHWSetStdFuncs(hwp);
    vgaHWGetIOBase(hwp);
    psav->vgaIOBase = hwp->IOBase;

    xf86CollectOptions(pScrn, NULL);

    if (pScrn->depth == 8)
	pScrn->rgbBits = 8;

    if (!(psav->Options = malloc(sizeof(SavageOptions))))
	return FALSE;
    memcpy(psav->Options, SavageOptions, sizeof(SavageOptions));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, psav->Options);

    xf86GetOptValBool(psav->Options, OPTION_IGNORE_EDID, &psav->IgnoreEDID);
    xf86GetOptValBool(psav->Options, OPTION_PCI_BURST, &psav->pci_burst);

    if (psav->pci_burst) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Option: pci_burst - PCI burst read enabled\n");
    }

    psav->NoPCIRetry = 1;		/* default */
    if (xf86ReturnOptValBool(psav->Options, OPTION_PCI_RETRY, FALSE)) {
	if (xf86ReturnOptValBool(psav->Options, OPTION_PCI_BURST, FALSE)) {
	    psav->NoPCIRetry = 0;
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Option: pci_retry\n");
	} else
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "\"pci_retry\" option requires \"pci_burst\"\n");
    }

    xf86GetOptValBool( psav->Options, OPTION_SHADOW_FB, &psav->shadowFB );
    if (psav->shadowFB) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Option: shadow FB enabled\n");
    }

    psav->primStreamBpp = pScrn->bitsPerPixel;

    if ((s = xf86GetOptValString(psav->Options, OPTION_ROTATE))) {
	if(!xf86NameCmp(s, "CW")) {
	    /* accel is disabled below for shadowFB */
             /* RandR is disabled when the Rotate option is used (does
              * not work well together and scrambles the screen) */

	    psav->shadowFB = TRUE;
	    psav->rotate = 1;
            xf86DisableRandR();
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, 
		       "Rotating screen clockwise"
                       "- acceleration and RandR disabled\n");
	} else if(!xf86NameCmp(s, "CCW")) {
	    psav->shadowFB = TRUE;
	    psav->rotate = -1;
            xf86DisableRandR();
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                   "Rotating screen counter clockwise"
                   " - acceleration and RandR disabled\n");

	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "\"%s\" is not a valid"
		       "value for Option \"Rotate\"\n", s);
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
		       "Valid options are \"CW\" or \"CCW\"\n");
	}
    }

    if (xf86GetOptValBool(psav->Options, OPTION_NOACCEL, &psav->NoAccel))
	xf86DrvMsg( pScrn->scrnIndex, X_CONFIG,
		    "Option: NoAccel - Acceleration Disabled\n");

    if (psav->shadowFB && !psav->NoAccel) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "HW acceleration not supported with \"shadowFB\".\n");
	psav->NoAccel = TRUE;
    }

    if(!psav->NoAccel) {
        from = X_DEFAULT;
	char *strptr;
#ifdef HAVE_XAA_H
        if((strptr = (char *)xf86GetOptValString(psav->Options, OPTION_ACCELMETHOD))) {
	    if(!xf86NameCmp(strptr,"XAA")) {
	        from = X_CONFIG;
	        psav->useEXA = FALSE;
	    } else if(!xf86NameCmp(strptr,"EXA")) {
	       from = X_CONFIG;
	       psav->useEXA = TRUE;
	    }
        }
#else
	psav->useEXA = TRUE;
#endif
       xf86DrvMsg(pScrn->scrnIndex, from, "Using %s acceleration architecture\n",
		psav->useEXA ? "EXA" : "XAA");
    }

    if ((s = xf86GetOptValString(psav->Options, OPTION_OVERLAY))) {
	
	if (psav->shadowFB) {
	    xf86DrvMsg(pScrn->scrnIndex,X_INFO,
		       "Option \"Overlay\" not supported with shadowFB\n");
	} else {
	    if (pScrn->depth == 8) {
		if (!*s || !xf86NameCmp(s, "24")) {
		    psav->overlayDepth = 24;
		    psav->NoAccel = TRUE; /* Preliminary */
		    pScrn->colorKey = TRANSPARENCY_KEY;
		    pScrn->overlayFlags = OVERLAY_8_32_DUALFB;
		} else if (!xf86NameCmp(s, "16")) {
		    psav->overlayDepth = 16;
		    psav->NoAccel = TRUE; /* Preliminary */
		    pScrn->colorKey = TRANSPARENCY_KEY;
		    pScrn->overlayFlags = OVERLAY_8_32_DUALFB;
		} else {
		    xf86DrvMsg(pScrn->scrnIndex,X_WARNING,"Wrong argument: "
			       "\"%s\" Ingnoring\n",s);
		}
	    } else if (pScrn->depth != 15) {
		psav->overlayDepth = 8;
		psav->NoAccel = TRUE; /* Preliminary */
		pScrn->colorKey = TRANSPARENCY_KEY;
		pScrn->overlayFlags = OVERLAY_8_32_DUALFB;
		if (*s && (xf86NameCmp(s, "8")))
		    xf86DrvMsg(pScrn->scrnIndex,X_WARNING,"Wrong argument: "
			       "\"%s\" for depth %i overlay depth must be 8\n",
			       s,pScrn->depth);
	    } else {
		 xf86DrvMsg(pScrn->scrnIndex,X_WARNING,"Overlay not "
			       "supported for depth 15\n");
	    }
	    if (psav->overlayDepth) {
		xf86DrvMsg(pScrn->scrnIndex,X_INFO,"%i/%i Overlay enabled\n",
			   pScrn->depth,psav->overlayDepth);
		psav->primStreamBpp = 8;
	    } 
	}
    }

    if (pScrn->bitsPerPixel == 24 && !psav->NoAccel) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "HW acceleration not possible with depth 32 and bpp 24.\n");
	psav->NoAccel = TRUE;
    }

    /*
     * The SWCursor setting takes priority over HWCursor.  The default
     * if neither is specified is HW, unless ShadowFB is specified,
     * then SW.
     */

    from = X_DEFAULT;
    psav->hwcursor = psav->shadowFB ? FALSE : TRUE;
    if (xf86GetOptValBool(psav->Options, OPTION_HWCURSOR, &psav->hwcursor))
	from = X_CONFIG;
    if (xf86ReturnOptValBool(psav->Options, OPTION_SWCURSOR, FALSE)) {
	psav->hwcursor = FALSE;
	from = X_CONFIG;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Using %s cursor\n",
        psav->hwcursor ? "HW" : "SW");

    from = X_DEFAULT;
    psav->UseBIOS = TRUE;
    if (xf86GetOptValBool(psav->Options, OPTION_USEBIOS, &psav->UseBIOS) )
	from = X_CONFIG;
    xf86DrvMsg(pScrn->scrnIndex, from, "%ssing video BIOS to set modes\n",
        psav->UseBIOS ? "U" : "Not u" );

    psav->LCDClock = 0.0;
    if( xf86GetOptValFreq( psav->Options, OPTION_LCDCLOCK, OPTUNITS_MHZ, &psav->LCDClock ) )
	xf86DrvMsg( pScrn->scrnIndex, X_CONFIG, 
		    "Option: LCDClock %1.2f MHz\n", psav->LCDClock );

    if( xf86GetOptValBool( psav->Options, OPTION_SHADOW_STATUS, &psav->ShadowStatus)) {
	xf86DrvMsg( pScrn->scrnIndex, X_CONFIG,
		    "Option: ShadowStatus %sabled\n", psav->ShadowStatus ? "en" : "dis" );
	psav->ForceShadowStatus = TRUE;
    } else
	psav->ForceShadowStatus = FALSE;
    /* If ShadowStatus is off it will be automatically enabled for DRI.
     * If DRI initialization fails fall back to ConfigShadowStatus. */
    psav->ConfigShadowStatus = psav->ShadowStatus;

    if( xf86GetOptValBool( psav->Options, OPTION_CRT_ONLY, &psav->CrtOnly))
	xf86DrvMsg( pScrn->scrnIndex, X_CONFIG,
		    "Option: CrtOnly enabled\n" );

    if( xf86GetOptValBool( psav->Options, OPTION_TV_ON, &psav->TvOn)) {
        psav->PAL = FALSE;
        SavageGetTvMaxSize(psav);
    }

    if( xf86GetOptValBool( psav->Options, OPTION_TV_PAL, &psav->PAL)) {
        SavageGetTvMaxSize(psav);
	psav->TvOn = TRUE;
    }

    if( psav->TvOn )
	xf86DrvMsg( pScrn->scrnIndex, X_CONFIG,
		    "TV enabled in %s format\n",
		    psav->PAL ? "PAL" : "NTSC" );

    psav->ForceInit = 0;
    if( xf86GetOptValBool( psav->Options, OPTION_FORCE_INIT, &psav->ForceInit))
	xf86DrvMsg( pScrn->scrnIndex, X_CONFIG,
		    "Option: ForceInit enabled\n" );

    if (pScrn->numEntities > 1) {
	SavageFreeRec(pScrn);
	return FALSE;
    }

    pEnt = xf86GetEntityInfo(pScrn->entityList[0]);
#ifndef XSERVER_LIBPCIACCESS
    if (pEnt->resources) {
	free(pEnt);
	SavageFreeRec(pScrn);
	return FALSE;
    }
#endif
    psav->EntityIndex = pEnt->index;

    if (xf86LoadSubModule(pScrn, "vbe")) {
	psav->pVbe = VBEInit(NULL, pEnt->index);
    }

#ifndef XSERVER_LIBPCIACCESS
    xf86RegisterResources(pEnt->index, NULL, ResNone);
    xf86SetOperatingState(resVgaIo, pEnt->index, ResUnusedOpr);
    xf86SetOperatingState(resVgaMem, pEnt->index, ResDisableOpr);
#endif

    from = X_DEFAULT;
    if (pEnt->device->chipset && *pEnt->device->chipset) {
	pScrn->chipset = pEnt->device->chipset;
	psav->ChipId = pEnt->device->chipID;
	from = X_CONFIG;
    } else if (pEnt->device->chipID >= 0) {
	psav->ChipId = pEnt->device->chipID;
	pScrn->chipset = (char *)xf86TokenToString(SavageChipsets,
						   psav->Chipset);
	from = X_CONFIG;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipID override: 0x%04X\n",
		   pEnt->device->chipID);
    } else {
	from = X_PROBED;
	psav->ChipId = DEVICE_ID(psav->PciInfo);
	pScrn->chipset = (char *)xf86TokenToString(SavageChipsets,
						   psav->Chipset);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Chip: id %04x, \"%s\"\n",
	       psav->ChipId, xf86TokenToString( SavageChips, psav->ChipId ) );

    if (pEnt->device->chipRev >= 0) {
	psav->ChipRev = pEnt->device->chipRev;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipRev override: %d\n",
		   psav->ChipRev);
    } else
	psav->ChipRev = CHIP_REVISION(psav->PciInfo);

    xf86DrvMsg(pScrn->scrnIndex, from, "Engine: \"%s\"\n", pScrn->chipset);

    if (pEnt->device->videoRam != 0)
    	pScrn->videoRam = pEnt->device->videoRam;

    free(pEnt);

#ifndef XSERVER_LIBPCIACCESS
    psav->PciTag = pciTag(psav->PciInfo->bus, psav->PciInfo->device,
			  psav->PciInfo->func);
#endif


    /* Set AGP Mode from config */
    /* We support 1X 2X and 4X  */
#ifdef SAVAGEDRI
#ifdef XSERVER_LIBPCIACCESS
    /* Try to read the AGP capabilty block from the device.  If there is
     * no AGP info, the device is PCI.
     */

    psav->IsPCI = (pci_device_get_agp_info(psav->PciInfo) == NULL);
#else
				/* AGP/PCI (FK: copied from radeon_driver.c) */
    /* Proper autodetection of an AGP capable device requires examining
     * PCI config registers to determine if the device implements extended
     * PCI capabilities, and then walking the capability list as indicated
     * in the PCI 2.2 and AGP 2.0 specifications, to determine if AGP
     * capability is present.  The procedure is outlined as follows:
     *
     * 1) Test bit 4 (CAP_LIST) of the PCI status register of the device
     *    to determine wether or not this device implements any extended
     *    capabilities.  If this bit is zero, then the device is a PCI 2.1
     *    or earlier device and is not AGP capable, and we can conclude it
     *    to be a PCI device.
     *
     * 2) If bit 4 of the status register is set, then the device implements
     *    extended capabilities.  There is an 8 bit wide capabilities pointer
     *    register located at offset 0x34 in PCI config space which points to
     *    the first capability in a linked list of extended capabilities that
     *    this device implements.  The lower two bits of this register are
     *    reserved and MBZ so must be masked out.
     *
     * 3) The extended capabilities list is formed by one or more extended
     *    capabilities structures which are aligned on DWORD boundaries.
     *    The first byte of the structure is the capability ID (CAP_ID)
     *    indicating what extended capability this structure refers to.  The
     *    second byte of the structure is an offset from the beginning of
     *    PCI config space pointing to the next capability in the linked
     *    list (NEXT_PTR) or NULL (0x00) at the end of the list.  The lower
     *    two bits of this pointer are reserved and MBZ.  By examining the
     *    CAP_ID of each capability and walking through the list, we will
     *    either find the AGP_CAP_ID (0x02) indicating this device is an
     *    AGP device, or we'll reach the end of the list, indicating it is
     *    a PCI device.
     *
     * Mike A. Harris <mharris@redhat.com>
     *
     * References:
     *	- PCI Local Bus Specification Revision 2.2, Chapter 6
     *	- AGP Interface Specification Revision 2.0, Section 6.1.5
     */

    psav->IsPCI = TRUE;

    if (pciReadLong(psav->PciTag, PCI_CMD_STAT_REG) & SAVAGE_CAP_LIST) {
	CARD32 cap_ptr, cap_id;

	cap_ptr = pciReadLong(psav->PciTag,
			      SAVAGE_CAPABILITIES_PTR_PCI_CONFIG)
	    & SAVAGE_CAP_PTR_MASK;

	while(cap_ptr != SAVAGE_CAP_ID_NULL) {
	    cap_id = pciReadLong(psav->PciTag, cap_ptr);
	    if ((cap_id & 0xff) == SAVAGE_CAP_ID_AGP) {
		psav->IsPCI = FALSE;
		break;
	    }
	    cap_ptr = (cap_id >> 8) & SAVAGE_CAP_PTR_MASK;
	}
    }
#endif

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "%s card detected\n",
	       (psav->IsPCI) ? "PCI" : "AGP");

    if ((s = xf86GetOptValString(psav->Options, OPTION_BUS_TYPE))) {
	if (strcmp(s, "AGP") == 0) {
	    if (psav->IsPCI) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "BusType AGP not available on PCI card\n");
	    } else {
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "BusType set to AGP\n");
	    }
	} else if (strcmp(s, "PCI") == 0) {
	    psav->IsPCI = TRUE;
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "BusType set to PCI\n");
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Invalid BusType option, using %s DMA\n",
		       psav->IsPCI ? "PCI" : "AGP");
	}
    }

    psav->AgpDMA = !psav->IsPCI;
    if ((s = xf86GetOptValString(psav->Options, OPTION_DMA_TYPE))) {
	if (strcmp(s, "AGP") == 0) {
	    if (psav->IsPCI) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "AGP DMA not available on PCI card, using PCI DMA\n");
	    } else {
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Using AGP DMA\n");
	    }
	} else if (strcmp(s, "PCI") == 0) {
	    psav->AgpDMA = FALSE;
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Using PCI DMA\n");
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Invalid DmaType option, using %s DMA\n",
		       psav->AgpDMA ? "AGP" : "PCI");
	}
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT,
		   "Using %s DMA\n", psav->AgpDMA ? "AGP" : "PCI");
    }

    psav->CommandDMA = TRUE;
    psav->VertexDMA = TRUE;
    from = X_DEFAULT;
    if ((s = xf86GetOptValString(psav->Options, OPTION_DMA_MODE))) {
	from = X_CONFIG;
	if (strcmp(s, "Command") == 0)
	    psav->VertexDMA = FALSE;
	else if (strcmp(s, "Vertex") == 0)
	    psav->CommandDMA = FALSE;
	else if (strcmp(s, "None") == 0)
	    psav->VertexDMA = psav->CommandDMA = FALSE;
	else if (strcmp(s, "Any") != 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Invalid DmaMode option\n");
	    from = X_DEFAULT;
	}
    }
    if (psav->CommandDMA && S3_SAVAGE3D_SERIES(psav->Chipset)) {
	xf86DrvMsg(pScrn->scrnIndex, from == X_CONFIG ? X_WARNING : X_INFO,
		   "Savage3D/MX/IX does not support command DMA.\n");
	psav->CommandDMA = FALSE;
    }
    if ((psav->CommandDMA || psav->VertexDMA) &&
	psav->Chipset == S3_SUPERSAVAGE) {
	xf86DrvMsg(pScrn->scrnIndex, from == X_CONFIG ? X_WARNING : X_INFO,
		   "DMA is not supported on SuperSavages.\n");
	psav->CommandDMA = psav->VertexDMA = FALSE;
    }
    if (psav->CommandDMA && psav->VertexDMA)
	xf86DrvMsg(pScrn->scrnIndex, from,
		   "Will try command and vertex DMA mode\n");
    else if (psav->CommandDMA && !psav->VertexDMA)
	xf86DrvMsg(pScrn->scrnIndex, from,
		   "Will try only command DMA mode\n");
    else if (!psav->CommandDMA && psav->VertexDMA)
	xf86DrvMsg(pScrn->scrnIndex, from,
		   "Will try only vertex DMA mode\n");
    else
	xf86DrvMsg(pScrn->scrnIndex, from,
		   "DMA disabled\n");

    if (!psav->IsPCI) {
	from = X_DEFAULT;
	psav->agpMode = SAVAGE_DEFAULT_AGP_MODE;
	/*psav->agpMode = SAVAGE_MAX_AGP_MODE;*/
	psav->agpSize = 16;
    
	if (xf86GetOptValInteger(psav->Options,
				 OPTION_AGP_MODE, &(psav->agpMode))) {
	    if (psav->agpMode < 1) {
		psav->agpMode = 1;
	    }
	    if (psav->agpMode > SAVAGE_MAX_AGP_MODE) {
		psav->agpMode = SAVAGE_MAX_AGP_MODE;
	    }
	    if ((psav->agpMode > 2) && 
		(psav->Chipset == S3_SAVAGE3D ||
		 psav->Chipset == S3_SAVAGE_MX))
		psav->agpMode = 2; /* old savages only support 2x */
	    from = X_CONFIG;
	}

	xf86DrvMsg(pScrn->scrnIndex, from, "Using AGP %dx mode\n",
		   psav->agpMode);

	from = X_DEFAULT;
	if (xf86GetOptValInteger(psav->Options,
				 OPTION_AGP_SIZE, (int *)&(psav->agpSize))) {
	    switch (psav->agpSize) {
	    case 4:
	    case 8:
	    case 16:
	    case 32:
	    case 64:
	    case 128:
	    case 256:
		from = X_CONFIG;
		break;
	    default:
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Illegal AGP size: %d MB, defaulting to 16 MB\n", psav->agpSize);
		psav->agpSize = 16;
	    }
	}

	xf86DrvMsg(pScrn->scrnIndex, from,
		   "Using %d MB AGP aperture\n", psav->agpSize);
    } else {
	psav->agpMode = 0;
	psav->agpSize = 0;
    }

#endif

    /* we can use Option "DisableTile TRUE" to disable tile mode */
    psav->bDisableTile = FALSE; 
    if (xf86GetOptValBool(psav->Options, OPTION_DISABLE_TILE,&psav->bDisableTile)) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                   "Option: %s Tile Mode and Program it \n",(psav->bDisableTile?"Disable":"Enable"));
    }

#ifdef SAVAGEDRI
    /* disabled by default...doesn't seem to work */
    psav->bDisableXvMC = TRUE; /* if you want to free up more mem for DRI,etc. */
    if (xf86GetOptValBool(psav->Options, OPTION_DISABLE_XVMC, &psav->bDisableXvMC)) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                   "Option: %s Hardware XvMC support\n",(psav->bDisableXvMC?"Disable":"Enable"));
    }
#endif

    psav->disableCOB = FALSE; /* if you are having problems on savage4+ */
    if (xf86GetOptValBool(psav->Options, OPTION_DISABLE_COB, &psav->disableCOB)) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                   "Option: %s the COB\n",(psav->disableCOB?"Disable":"Enable"));
    }
    if (psav->Chipset == S3_PROSAVAGE ||
	psav->Chipset == S3_TWISTER   ||
	psav->Chipset == S3_PROSAVAGEDDR)
	psav->BCIforXv = TRUE;
    else
    	psav->BCIforXv = FALSE; /* use the BCI for Xv */
    if (xf86GetOptValBool(psav->Options, OPTION_BCI_FOR_XV, &psav->BCIforXv)) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                   "Option: %s use of the BCI for Xv\n",(psav->BCIforXv?"Enable":"Disable"));
    }
    psav->dvi = FALSE;
    if (xf86GetOptValBool(psav->Options, OPTION_DVI, &psav->dvi)) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                   "%s DVI port support (Savage4 only)\n",(psav->dvi?"Force":"Disable"));
    }

#ifdef SAVAGEDRI
    psav->AGPforXv = FALSE;
    if (xf86GetOptValBool(psav->Options, OPTION_AGP_FOR_XV, &psav->AGPforXv)) {
        if (psav->AGPforXv) {
            if (psav->agpSize == 0) {
                psav->AGPforXv = FALSE;
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "AGP not available, cannot use AGP for Xv\n");
            }
        }
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                   "Option: %s use of AGP buffer for Xv\n",(psav->AGPforXv?"Enable":"Disable"));
    }
#endif

    /* Add more options here. */


    psav               = SAVPTR(pScrn);
    psav->IsSecondary  = FALSE;
    psav->IsPrimary    = FALSE;
    psav->pEnt         = xf86GetEntityInfo(pScrn->entityList[pScrn->numEntities - 1]);

    if (xf86IsEntityShared(psav->pEnt->index)) {
	if (xf86IsPrimInitDone(psav->pEnt->index)) {

	    SavageEntPtr pSavageEnt = SavageEntPriv(pScrn);

	    psav->IsSecondary = TRUE;
	    pSavageEnt->pSecondaryScrn = pScrn;
	    psav->TvOn = pSavageEnt->TvOn;
	} else {
	    SavageEntPtr pSavageEnt = SavageEntPriv(pScrn);

	    xf86SetPrimInitDone(psav->pEnt->index);

	    psav->IsPrimary = TRUE;
	    pSavageEnt->pPrimaryScrn        = pScrn;
	    pSavageEnt->TvOn = psav->TvOn;
	}
    }

    switch(psav->Chipset) {
	case S3_SAVAGE_MX:
	case S3_SUPERSAVAGE:
	    psav->HasCRTC2 = TRUE;
	    break;
        default: 
            psav->HasCRTC2 = FALSE;
    }

    if ((psav->IsSecondary || psav->IsPrimary) && !psav->UseBIOS) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "BIOS currently required for Dualhead mode setting.\n");
	return FALSE;	
    }

    if (psav->IsSecondary &&
	(pScrn->bitsPerPixel > 16) &&
	!psav->NoAccel &&
	(psav->Chipset == S3_SAVAGE_MX)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No acceleration in Dualhead mode at depth 24\n");
	return FALSE;
    }

    /* maybe throw in some more sanity checks here */

    if (!SavageMapMem(pScrn)) {
	SavageFreeRec(pScrn);
        vbeFree(psav->pVbe);
	psav->pVbe = NULL;
	return FALSE;
    }

    vgaCRIndex = psav->vgaIOBase + 4;
    vgaCRReg = psav->vgaIOBase + 5;

    xf86EnableIO();
    /* unprotect CRTC[0-7] */
    VGAOUT8(vgaCRIndex, 0x11);
    tmp = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRReg, tmp & 0x7f);

    /* unlock extended regs */
    VGAOUT16(vgaCRIndex, 0x4838);
    VGAOUT16(vgaCRIndex, 0xa039);
    VGAOUT16(0x3c4, 0x0608);

    VGAOUT8(vgaCRIndex, 0x40);
    tmp = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRReg, tmp & ~0x01);

    /* unlock sys regs */
    VGAOUT8(vgaCRIndex, 0x38);
    VGAOUT8(vgaCRReg, 0x48);

    {
	Gamma zeros = {0.0, 0.0, 0.0};

	if (!xf86SetGamma(pScrn, zeros)) {
	    vbeFree(psav->pVbe);
	    psav->pVbe = NULL;
	    SavageFreeRec(pScrn);
	    return FALSE;
	}
    }

    /* Unlock system registers. */
    VGAOUT16(vgaCRIndex, 0x4838);

    /* Next go on to detect amount of installed ram */

    VGAOUT8(vgaCRIndex, 0x36);            /* for register CR36 (CONFG_REG1), */
    config1 = VGAIN8(vgaCRReg);           /* get amount of vram installed */

    /* Compute the amount of video memory and offscreen memory. */

    if (!pScrn->videoRam) {
	static const unsigned char RamSavage3D[] = { 8, 4, 4, 2 };
	static unsigned char RamSavage4[] =  { 2, 4, 8, 12, 16, 32, 64, 32 };
	static const unsigned char RamSavageMX[] = { 2, 8, 4, 16, 8, 16, 4, 16 };
	static const unsigned char RamSavageNB[] = { 0, 2, 4, 8, 16, 32, 16, 2 };

	switch( psav->Chipset ) {
	case S3_SAVAGE3D:
	    pScrn->videoRam = RamSavage3D[ (config1 & 0xC0) >> 6 ] * 1024;
	    break;

	case S3_SAVAGE4:
	    /* 
	     * The Savage4 has one ugly special case to consider.  On
	     * systems with 4 banks of 2Mx32 SDRAM, the BIOS says 4MB
	     * when it really means 8MB.  Why do it the same when you
	     * can do it different...
	     */
	    VGAOUT8(vgaCRIndex, 0x68);	/* memory control 1 */
	    if( (VGAIN8(vgaCRReg) & 0xC0) == (0x01 << 6) )
		RamSavage4[1] = 8;

	    /*FALLTHROUGH*/

	case S3_SAVAGE2000:
	    pScrn->videoRam = RamSavage4[ (config1 & 0xE0) >> 5 ] * 1024;
	    break;

	case S3_SAVAGE_MX:
	case S3_SUPERSAVAGE:
	    pScrn->videoRam = RamSavageMX[ (config1 & 0x0E) >> 1 ] * 1024;
	    break;

	case S3_PROSAVAGE:
	case S3_PROSAVAGEDDR:
	case S3_TWISTER:
	    pScrn->videoRam = RamSavageNB[ (config1 & 0xE0) >> 5 ] * 1024;
	    break;

	default:
	    /* How did we get here? */
	    pScrn->videoRam = 0;
	    break;
	}

	psav->videoRambytes = pScrn->videoRam * 1024;

	xf86DrvMsg(pScrn->scrnIndex, X_PROBED, 
		"probed videoram:  %dk\n",
		pScrn->videoRam);
    } else {
	psav->videoRambytes = pScrn->videoRam * 1024;

	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
	       "videoram =  %dk\n",
		pScrn->videoRam);
    }

    /* Get video RAM */
    if( !pScrn->videoRam && psav->pVbe )
    {
        /* If VBE is available, ask it about onboard memory. */

	VbeInfoBlock* vib;

	vib = VBEGetVBEInfo( psav->pVbe );
	pScrn->videoRam = vib->TotalMemory * 64;
	VBEFreeVBEInfo( vib );

	/* VBE often cuts 64k off of the RAM total. */

	if( pScrn->videoRam & 64 )
	    pScrn->videoRam += 64;

	psav->videoRambytes = pScrn->videoRam * 1024;
    }

    
    /*
     * If we're running with acceleration, compute the command overflow
     * buffer location.  The command overflow buffer must END at a
     * 4MB boundary; for all practical purposes, that means the very
     * end of the frame buffer.
     */
    if (psav->NoAccel) {
        psav->cobIndex = 0;
        psav->cobSize = 0;
    } 
    else if( ((S3_SAVAGE4_SERIES(psav->Chipset)) ||
             (S3_SUPERSAVAGE == psav->Chipset)) && psav->disableCOB ) {
        /*
         * The Savage4 and ProSavage have COB coherency bugs which render 
         * the buffer useless.
         */
	/*
        psav->cobIndex = 2;
        psav->cobSize = 0x8000 << psav->cobIndex;
	*/
        psav->cobIndex = 0;
        psav->cobSize = 0;
	psav->bciThresholdHi = 32;
	psav->bciThresholdLo = 0;
    } else {
        /* We use 128kB for the COB on all other chips. */        
        psav->cobSize = 0x20000;
	if (S3_SAVAGE3D_SERIES(psav->Chipset) ||
	    psav->Chipset == S3_SAVAGE2000) {
	    psav->cobIndex = 7; /* rev.A savage4 apparently also uses 7 */
	} else {
	    psav->cobIndex = 2;
	}
	/* max command size: 2560 entries */
	psav->bciThresholdHi = psav->cobSize/4 + 32 - 2560;
	psav->bciThresholdLo = psav->bciThresholdHi - 2560;
    }
             
    /* align cob to 128k */
    psav->cobOffset = (psav->videoRambytes - psav->cobSize) & ~0x1ffff;
    
    /* The cursor must be aligned on a 4k boundary. */
    psav->CursorKByte = (psav->cobOffset >> 10) - 4;
    psav->endfb = (psav->CursorKByte << 10) - 1;

    if (psav->IsPrimary) {
        pScrn->videoRam /= 2;
	psav->videoRambytes = pScrn->videoRam * 1024;
	psav->CursorKByte = (psav->videoRambytes >> 10) - 4;
	psav->endfb = (psav->CursorKByte << 10) - 1;
	psav->videoRambytes *= 2;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
		"Using %dk of videoram for primary head\n",
		pScrn->videoRam);
    }

    if(psav->IsSecondary)
    {  
        pScrn->videoRam /= 2;
	/*psav->videoRambytes = pScrn->videoRam * 1024;*/
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
		"Using %dk of videoram for secondary head\n",
		pScrn->videoRam);
    }

    pScrn->fbOffset = (psav->IsSecondary)
      ? pScrn->videoRam * 1024 : 0;

    /* reset graphics engine to avoid memory corruption */
    VGAOUT8(vgaCRIndex, 0x66);
    cr66 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRReg, cr66 | 0x02);
    usleep(10000);

    VGAOUT8(vgaCRIndex, 0x66);
    VGAOUT8(vgaCRReg, cr66 & ~0x02);	/* clear reset flag */
    usleep(10000);

    /* Set status word positions based on chip type. */
    SavageInitStatus(pScrn);

    /* check for DVI/flat panel */
    dvi = FALSE;
    if (psav->Chipset == S3_SAVAGE4) {
	unsigned char sr30 = 0x00; 
	VGAOUT8(0x3c4, 0x30);
	/* clear bit 1 */
	VGAOUT8(0x3c5, VGAIN8(0x3c5) & ~0x02);
    	sr30 = VGAIN8(0x3c5);
    	if (sr30 & 0x02 /*0x04 */) {
            dvi = TRUE;
	    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Digital Flat Panel Detected\n");
	}
    }

    if( (S3_SAVAGE_MOBILE_SERIES(psav->Chipset) ||
	S3_MOBILE_TWISTER_SERIES(psav->Chipset)) && !psav->CrtOnly ) {
	psav->DisplayType = MT_LCD;
    } else if (dvi || ((psav->Chipset == S3_SAVAGE4) && psav->dvi)) {
	psav->DisplayType = MT_DFP;
    } else {
	psav->DisplayType = MT_CRT;
    }
    
    if (psav->IsSecondary)
	psav->DisplayType = MT_CRT;

    /* Do the DDC dance. */ 
    SavageDoDDC(pScrn);

    /* set up ramdac max clock - might be altered by SavageGetPanelInfo */
    if (pScrn->bitsPerPixel >= 24)
        psav->maxClock = 220000;
    else
        psav->maxClock = 250000;

    /* detect current mclk */
    VGAOUT8(0x3c4, 0x08);
    sr8 = VGAIN8(0x3c5);
    VGAOUT8(0x3c5, 0x06);
    VGAOUT8(0x3c4, 0x10);
    n = VGAIN8(0x3c5);
    VGAOUT8(0x3c4, 0x11);
    m = VGAIN8(0x3c5);
    VGAOUT8(0x3c4, 0x08);
    VGAOUT8(0x3c5, sr8);
    m &= 0x7f;
    n1 = n & 0x1f;
    n2 = (n >> 5) & 0x03;
    mclk = ((1431818 * (m+2)) / (n1+2) / (1 << n2) + 50) / 100;
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Detected current MCLK value of %1.3f MHz\n",
	       mclk / 1000.0);

    pScrn->maxHValue = 2048 << 3;	/* 11 bits of h_total 8-pixel units */
    pScrn->maxVValue = 2048;		/* 11 bits of v_total */
    pScrn->virtualX = pScrn->display->virtualX;
    pScrn->virtualY = pScrn->display->virtualY;

    /* Check LCD panel information */

    if(psav->DisplayType == MT_LCD)
	SavageGetPanelInfo(pScrn);

    /* DisplayType will be reset if panel is not active */
    if(psav->DisplayType == MT_LCD)
	SavageAddPanelMode(pScrn);
  
#if 0
    if (psav->CrtOnly && !psav->UseBIOS) {
	VGAOUT8(0x3c4, 0x31); /* SR31 bit 4 - FP enable */
	VGAOUT8(0x3c5, VGAIN8(0x3c5) & ~0x10); /* disable FP */
        if (S3_SAVAGE_MOBILE_SERIES(psav->Chipset) /*|| 
	    S3_MOBILE_TWISTER_SERIES(psav->Chipset)*/) { /* not sure this works on mobile prosavage */
		VGAOUT8(0x3c4, 0x31); 
		VGAOUT8(0x3c5, VGAIN8(0x3c5) & ~0x04); /* make sure crtc1 is crt source */
    	}
    }
#endif

    if( psav->UseBIOS )
    {
	/* Go probe the BIOS for all the modes and refreshes at this depth. */

	if( psav->ModeTable )
	{
	    SavageFreeBIOSModeTable( psav, &psav->ModeTable );
	}

	psav->ModeTable = SavageGetBIOSModeTable( psav, psav->primStreamBpp );

	if( !psav->ModeTable || !psav->ModeTable->NumModes ) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, 
		       "Failed to fetch any BIOS modes.  Disabling BIOS.\n");
	    psav->UseBIOS = FALSE;
	}
	else
	/*if( xf86Verbose )*/
	{
	    SavageModeEntryPtr pmt;

	    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		       "Found %d modes at this depth:\n",
		       psav->ModeTable->NumModes);

	    for(
		i = 0, pmt = psav->ModeTable->Modes; 
		i < psav->ModeTable->NumModes; 
		i++, pmt++ )
	    {
		int j;
		ErrorF( "    [%03x] %d x %d", 
			pmt->VesaMode, pmt->Width, pmt->Height );
		for( j = 0; j < pmt->RefreshCount; j++ )
		{
		    ErrorF( ", %dHz", pmt->RefreshRate[j] );
		}
		ErrorF( "\n");
	    }
	}
    }

    clockRanges = xnfalloc(sizeof(ClockRange));
    clockRanges->next = NULL;
    clockRanges->minClock = 10000;
    clockRanges->maxClock = psav->maxClock;
    clockRanges->clockIndex = -1;
    clockRanges->interlaceAllowed = TRUE;
    clockRanges->doubleScanAllowed = TRUE;
    clockRanges->ClockDivFactor = 1.0;
    clockRanges->ClockMulFactor = 1.0;

    i = xf86ValidateModes(pScrn, pScrn->monitor->Modes,
			  pScrn->display->modes, clockRanges, NULL, 
			  256, 2048, 16 * pScrn->bitsPerPixel,
			  128, 2048, 
			  pScrn->virtualX, pScrn->virtualY,
			  psav->videoRambytes, LOOKUP_BEST_REFRESH);

    if (i == -1) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "xf86ValidateModes failure\n");
	SavageFreeRec(pScrn);
	vbeFree(psav->pVbe);
	psav->pVbe = NULL;
	return FALSE;
    }

    xf86PruneDriverModes(pScrn);

    if (i == 0 || pScrn->modes == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
	SavageFreeRec(pScrn);
	vbeFree(psav->pVbe);
	psav->pVbe = NULL;
	return FALSE;
    }

    xf86SetCrtcForModes(pScrn, INTERLACE_HALVE_V);
    pScrn->currentMode = pScrn->modes;
    xf86PrintModes(pScrn);
    xf86SetDpi(pScrn, 0, 0);

    if (xf86LoadSubModule(pScrn, "fb") == NULL) {
	SavageFreeRec(pScrn);
	vbeFree(psav->pVbe);
	psav->pVbe = NULL;
	return FALSE;
    }

    if( !psav->NoAccel ) {
        char *modName = NULL;

	if (psav->useEXA) {
	    modName = "exa";
	    XF86ModReqInfo req;
	    int errmaj, errmin;
	    memset(&req, 0, sizeof(req));
	    req.majorversion = 2;
	    req.minorversion = 0;
	    
	    if( !LoadSubModule(pScrn->module, modName, 
		NULL, NULL, NULL, &req, &errmaj, &errmin) ) {
		LoaderErrorMsg(NULL, modName, errmaj, errmin);
	    	SavageFreeRec(pScrn);
	    	vbeFree(psav->pVbe);
	    	psav->pVbe = NULL;
	    	return FALSE;
	    }
	} else {
	    modName = "xaa";
	    if( !xf86LoadSubModule(pScrn, modName) ) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "Falling back to shadowfb\n");
		psav->NoAccel = 1;
		psav->shadowFB = 1;
	    } 
	}
    }

    if (psav->hwcursor) {
	if (!xf86LoadSubModule(pScrn, "ramdac")) {
	    SavageFreeRec(pScrn);
	    vbeFree(psav->pVbe);
	    psav->pVbe = NULL;
	    return FALSE;
	}
    }

    if (psav->shadowFB) {
	if (!xf86LoadSubModule(pScrn, "shadowfb")) {
	    SavageFreeRec(pScrn);
	    vbeFree(psav->pVbe);
	    psav->pVbe = NULL;
	    return FALSE;
	}
    }
    vbeFree(psav->pVbe);

    psav->pVbe = NULL;

    return TRUE;
}


static Bool SavageEnterVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
#ifdef SAVAGEDRI
    SavagePtr psav = SAVPTR(pScrn);
    ScreenPtr pScreen;
#endif

    gpScrn = pScrn;
    SavageEnableMMIO(pScrn);

#ifdef SAVAGEDRI
    if (psav->directRenderingEnabled) {
        pScreen = xf86ScrnToScreen(pScrn);
        SAVAGEDRIResume(pScreen);
        DRIUnlock(pScreen);
        psav->LockHeld = 0;
    }
#endif
    if (!SAVPTR(pScrn)->IsSecondary)
    	SavageSave(pScrn);
    if(SavageModeInit(pScrn, pScrn->currentMode)) {
	/* some BIOSes seem to enable HW cursor on PM resume */
	if (!SAVPTR(pScrn)->hwc_on)
	    SavageHideCursor( pScrn ); 
	return TRUE;
    }

    return FALSE;
}


static void SavageLeaveVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    SavagePtr psav = SAVPTR(pScrn);
    vgaRegPtr vgaSavePtr = &hwp->SavedReg;
    SavageRegPtr SavageSavePtr = &psav->SavedReg;
#ifdef SAVAGEDRI
    ScreenPtr pScreen;
#endif

    TRACE(("SavageLeaveVT()\n"));
    gpScrn = pScrn;

#ifdef SAVAGEDRI
    if (psav->directRenderingEnabled) {
        pScreen = xf86ScrnToScreen(pScrn);
        DRILock(pScreen, 0);
        psav->LockHeld = 1;
    }
#endif
    if (psav->FBStart2nd || (psav->videoFlags & VF_STREAMS_ON))
        SavageStreamsOff(pScrn);
    SavageWriteMode(pScrn, vgaSavePtr, SavageSavePtr, FALSE);
    SavageResetStreams(pScrn);
    SavageDisableMMIO(pScrn);

}


static void SavageSave(ScrnInfoPtr pScrn)
{
    unsigned char cr3a, cr53, cr66;
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaSavePtr = &hwp->SavedReg;
    SavagePtr psav = SAVPTR(pScrn);
    SavageRegPtr save = &psav->SavedReg;
    unsigned short vgaCRReg = psav->vgaIOBase + 5;
    unsigned short vgaCRIndex = psav->vgaIOBase + 4;

    TRACE(("SavageSave()\n"));

    VGAOUT16(vgaCRIndex, 0x4838);
    VGAOUT16(vgaCRIndex, 0xa039);
    VGAOUT16(0x3c4, 0x0608);

    VGAOUT8(vgaCRIndex, 0x66);
    cr66 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRReg, cr66 | 0x80);
    VGAOUT8(vgaCRIndex, 0x3a);
    cr3a = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRReg, cr3a | 0x80);
    VGAOUT8(vgaCRIndex, 0x53);
    cr53 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRReg, cr53 & 0x7f);

    if (xf86IsPrimaryPci(psav->PciInfo))
	vgaHWSave(pScrn, vgaSavePtr, VGA_SR_ALL);
    else
	vgaHWSave(pScrn, vgaSavePtr, VGA_SR_MODE);

    VGAOUT8(vgaCRIndex, 0x66);
    VGAOUT8(vgaCRReg, cr66);
    VGAOUT8(vgaCRIndex, 0x3a);
    VGAOUT8(vgaCRReg, cr3a);

    VGAOUT8(vgaCRIndex, 0x66);
    VGAOUT8(vgaCRReg, cr66);
    VGAOUT8(vgaCRIndex, 0x3a);
    VGAOUT8(vgaCRReg, cr3a);

    /* unlock extended seq regs */
    VGAOUT8(0x3c4, 0x08);
    save->SR08 = VGAIN8(0x3c5);
    VGAOUT8(0x3c5, 0x06);

    /* now save all the extended regs we need */
    VGAOUT8(vgaCRIndex, 0x31);
    save->CR31 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x32);
    save->CR32 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x34);
    save->CR34 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x36);
    save->CR36 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x3a);
    save->CR3A = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x40);
    save->CR40 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x42);
    save->CR42 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x45);
    save->CR45 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x50);
    save->CR50 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x51);
    save->CR51 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x53);
    save->CR53 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x58);
    save->CR58 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x60);
    save->CR60 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x66);
    save->CR66 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x67);
    save->CR67 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x68);
    save->CR68 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x69);
    save->CR69 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x6f);
    save->CR6F = VGAIN8(vgaCRReg);

    VGAOUT8(vgaCRIndex, 0x33);
    save->CR33 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x86);
    save->CR86 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x88);
    save->CR88 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x90);
    save->CR90 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x91);
    save->CR91 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0xb0);
    save->CRB0 = VGAIN8(vgaCRReg) | 0x80;

    /* extended mode timing regs */
    VGAOUT8(vgaCRIndex, 0x3b);
    save->CR3B = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x3c);
    save->CR3C = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x43);
    save->CR43 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x5d);
    save->CR5D = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x5e);
    save->CR5E = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRIndex, 0x65);
    save->CR65 = VGAIN8(vgaCRReg);

    /* save seq extended regs for DCLK PLL programming */
    VGAOUT8(0x3c4, 0x0e);
    save->SR0E = VGAIN8(0x3c5);
    VGAOUT8(0x3c4, 0x0f);
    save->SR0F = VGAIN8(0x3c5);
    VGAOUT8(0x3c4, 0x10);
    save->SR10 = VGAIN8(0x3c5);
    VGAOUT8(0x3c4, 0x11);
    save->SR11 = VGAIN8(0x3c5);
    VGAOUT8(0x3c4, 0x12);
    save->SR12 = VGAIN8(0x3c5);
    VGAOUT8(0x3c4, 0x13);
    save->SR13 = VGAIN8(0x3c5);
    VGAOUT8(0x3c4, 0x29);
    save->SR29 = VGAIN8(0x3c5);

    VGAOUT8(0x3c4, 0x15);
    save->SR15 = VGAIN8(0x3c5);
    VGAOUT8(0x3c4, 0x30);
    save->SR30 = VGAIN8(0x3c5);
    VGAOUT8(0x3c4, 0x18);
    save->SR18 = VGAIN8(0x3c5);
    VGAOUT8(0x3c4, 0x1b);
    save->SR1B = VGAIN8(0x3c5);

    /* Save flat panel expansion registers. */

    if( S3_SAVAGE_MOBILE_SERIES(psav->Chipset) ||
	S3_MOBILE_TWISTER_SERIES(psav->Chipset)) {
	int i;
	for( i = 0; i < 8; i++ ) {
	    VGAOUT8(0x3c4, 0x54+i);
	    save->SR54[i] = VGAIN8(0x3c5);
	}
    }

    VGAOUT8(vgaCRIndex, 0x66);
    cr66 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRReg, cr66 | 0x80);
    VGAOUT8(vgaCRIndex, 0x3a);
    cr3a = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRReg, cr3a | 0x80);

    /* now save MIU regs */
    if( ! S3_SAVAGE_MOBILE_SERIES(psav->Chipset) ) {
	save->MMPR0 = INREG(FIFO_CONTROL_REG);
	save->MMPR1 = INREG(MIU_CONTROL_REG);
	save->MMPR2 = INREG(STREAMS_TIMEOUT_REG);
	save->MMPR3 = INREG(MISC_TIMEOUT_REG);
    }

    VGAOUT8(vgaCRIndex, 0x3a);
    VGAOUT8(vgaCRReg, cr3a);
    VGAOUT8(vgaCRIndex, 0x66);
    VGAOUT8(vgaCRReg, cr66);

    if (!psav->ModeStructInit) {
	vgaHWCopyReg(&hwp->ModeReg, vgaSavePtr);
	memcpy(&psav->ModeReg, save, sizeof(SavageRegRec));
	psav->ModeStructInit = TRUE;
    }

#if 0
    if (xf86GetVerbosity() > 1)
	SavagePrintRegs(pScrn);
#endif

    return;
}


static void SavageWriteMode(ScrnInfoPtr pScrn, vgaRegPtr vgaSavePtr,
			    SavageRegPtr restore, Bool Entering)
{
    unsigned char tmp, cr3a, cr66;
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    SavagePtr psav = SAVPTR(pScrn);
    int vgaCRIndex, vgaCRReg, vgaIOBase;


    vgaIOBase = hwp->IOBase;
    vgaCRIndex = vgaIOBase + 4;
    vgaCRReg = vgaIOBase + 5;
    
    TRACE(("SavageWriteMode(%x)\n", restore->mode));

#ifdef SAVAGEDRI
    if (psav->directRenderingEnabled) {
        DRILock(xf86ScrnToScreen(pScrn), 0);
        psav->LockHeld = 1;
    }
#endif

    if (psav->IsSecondary) {
	/* Set up the mode.  Don't clear video RAM. */
	SavageSetVESAMode( psav, restore->mode | 0x8000, restore->refresh );
	SavageSetGBD(pScrn);
	return;
    }

    if( Entering && 
	(!S3_SAVAGE_MOBILE_SERIES(psav->Chipset) || (psav->ForceInit))
    )
	SavageInitialize2DEngine(pScrn);

    /*
     * If we figured out a VESA mode number for this timing, just use
     * the S3 BIOS to do the switching, with a few additional tweaks.
     */

    if( psav->UseBIOS && restore->mode > 0x13 )
    {
	int width;
	unsigned short cr6d;
	unsigned short cr79 = 0;

	/* Set up the mode.  Don't clear video RAM. */
	SavageSetVESAMode( psav, restore->mode | 0x8000, restore->refresh );

	/* Restore the DAC. */
	vgaHWRestore(pScrn, vgaSavePtr, VGA_SR_CMAP);

	/* Unlock the extended registers. */

#if 0
	/* Which way is better? */
	hwp->writeCrtc( hwp, 0x38, 0x48 );
	hwp->writeCrtc( hwp, 0x39, 0xa0 );
	hwp->writeSeq( hwp, 0x08, 0x06 );
#endif

	VGAOUT16(vgaCRIndex, 0x4838);
	VGAOUT16(vgaCRIndex, 0xA039);
	VGAOUT16(0x3c4, 0x0608);

	/* Enable linear addressing. */

	VGAOUT16(vgaCRIndex, 0x1358);

	/* Disable old MMIO. */

	VGAOUT8(vgaCRIndex, 0x53);
	VGAOUT8(vgaCRReg, VGAIN8(vgaCRReg) & ~0x10);

	/* Disable HW cursor */

	VGAOUT16(vgaCRIndex, 0x0045);

	/* Set the color mode. */

	VGAOUT8(vgaCRIndex, 0x67);
	VGAOUT8(vgaCRReg, restore->CR67);

	/* Enable gamma correction, set CLUT to 8 bit */

	VGAOUT8(0x3c4, 0x1b);
	if( (pScrn->bitsPerPixel == 32) && !psav->DGAactive 
	    && ! psav->FBStart2nd )
		VGAOUT8(0x3c5, 0x18 );
	else
		VGAOUT8(0x3c5, 0x10 );

	/* We may need TV/panel fixups here.  See s3bios.c line 2904. */

	/* Set FIFO fetch delay. */
	VGAOUT8(vgaCRIndex, 0x85);
	VGAOUT8(vgaCRReg, (VGAIN8(vgaCRReg) & 0xf8) | 0x03);

	/* Patch CR79.  These values are magical. */

	if( !S3_SAVAGE_MOBILE_SERIES(psav->Chipset) )
	{
	    VGAOUT8(vgaCRIndex, 0x6d);
	    cr6d = VGAIN8(vgaCRReg);

	    cr79 = 0x04;

	    if( pScrn->displayWidth >= 1024 )
	    {
		if(psav->primStreamBpp == 32 )
		{
		    if( restore->refresh >= 130 )
			cr79 = 0x03;
		    else if( pScrn->displayWidth >= 1280 )
			cr79 = 0x02;
		    else if(
			(pScrn->displayWidth == 1024) &&
			(restore->refresh >= 75)
		    )
		    {
			if( cr6d & LCD_ACTIVE )
			    cr79 = 0x05;
			else
			    cr79 = 0x08;
		    }
		}
		else if( psav->primStreamBpp == 16)
		{

/* The windows driver uses 0x13 for 16-bit 130Hz, but I see terrible
 * screen artifacts with that value.  Let's keep it low for now.
 *		if( restore->refresh >= 130 )
 *		    cr79 = 0x13;
 *		else
 */
		    if( pScrn->displayWidth == 1024 )
		    {
			if( cr6d & LCD_ACTIVE )
			    cr79 = 0x08;
			else
			    cr79 = 0x0e;
		    }
		}
	    }
	}

        if( (psav->Chipset != S3_SAVAGE2000) && 
	    !S3_SAVAGE_MOBILE_SERIES(psav->Chipset) )
	    VGAOUT16(vgaCRIndex, (cr79 << 8) | 0x79);

	/* Make sure 16-bit memory access is enabled. */

	VGAOUT16(vgaCRIndex, 0x0c31);

	/* Enable the graphics engine. */

	VGAOUT16(vgaCRIndex, 0x0140);

	/* Handle the pitch. */

        VGAOUT8(vgaCRIndex, 0x50);
        VGAOUT8(vgaCRReg, VGAIN8(vgaCRReg) | 0xC1);

	width = (pScrn->displayWidth * (psav->primStreamBpp / 8)) >> 3;
	VGAOUT16(vgaCRIndex, ((width & 0xff) << 8) | 0x13 );
	VGAOUT16(vgaCRIndex, ((width & 0x300) << 4) | 0x51 );

	/* Some non-S3 BIOSes enable block write even on non-SGRAM devices. */

	switch( psav->Chipset )
	{
	    case S3_SAVAGE2000:
		VGAOUT8(vgaCRIndex, 0x73);
		VGAOUT8(vgaCRReg, VGAIN8(vgaCRReg) & 0xdf );
		break;

	    case S3_SAVAGE3D:
	    case S3_SAVAGE4:
		VGAOUT8(vgaCRIndex, 0x68);
		if( !(VGAIN8(vgaCRReg) & 0x80) )
		{
		    /* Not SGRAM; disable block write. */
		    VGAOUT8(vgaCRIndex, 0x88);
		    VGAOUT8(vgaCRReg, VGAIN8(vgaCRReg) | 0x10);
		}
		break;
	}

	/* set the correct clock for some BIOSes */
	VGAOUT8(VGA_MISC_OUT_W, 
		VGAIN8(VGA_MISC_OUT_R) | 0x0C);
	/* Some BIOSes turn on clock doubling on non-doubled modes */
	if (pScrn->bitsPerPixel < 24) {
	    VGAOUT8(vgaCRIndex, 0x67);
	    if (!(VGAIN8(vgaCRReg) & 0x10)) {
		VGAOUT8(0x3c4, 0x15);
		VGAOUT8(0x3c5, VGAIN8(0x3C5) & ~0x10);
		VGAOUT8(0x3c4, 0x18);
		VGAOUT8(0x3c5, VGAIN8(0x3c5) & ~0x80);
	    }
	}

	SavageInitialize2DEngine(pScrn);

	VGAOUT16(vgaCRIndex, 0x0140);

	SavageSetGBD(pScrn);


#ifdef SAVAGEDRI
    	if (psav->directRenderingEnabled)
    	    DRIUnlock(xf86ScrnToScreen(pScrn));
    	psav->LockHeld = 0;
#endif

	return;
    }

    VGAOUT8(0x3c2, 0x23);
    VGAOUT16(vgaCRIndex, 0x4838);
    VGAOUT16(vgaCRIndex, 0xa039);
    VGAOUT16(0x3c4, 0x0608);

    vgaHWProtect(pScrn, TRUE);

    /* will we be reenabling STREAMS for the new mode? */
    psav->STREAMSRunning = 0;

    /* reset GE to make sure nothing is going on */
    VGAOUT8(vgaCRIndex, 0x66);
    if(VGAIN8(vgaCRReg) & 0x01)
	SavageGEReset(pScrn,0,__LINE__,__FILE__);

    /*
     * Some Savage/MX and /IX systems go nuts when trying to exit the
     * server after WindowMaker has displayed a gradient background.  I
     * haven't been able to find what causes it, but a non-destructive
     * switch to mode 3 here seems to eliminate the issue.
     */

    if( ((restore->CR31 & 0x0a) == 0) && psav->pVbe ) {
	SavageSetTextMode( psav );
    }

    VGAOUT8(vgaCRIndex, 0x67);
    (void) VGAIN8(vgaCRReg);
    /*VGAOUT8(vgaCRReg, restore->CR67 & ~0x0c);*/ /* no STREAMS yet */
    VGAOUT8(vgaCRReg, restore->CR67 & ~0x0e); /* no STREAMS yet old and new */

    /* restore extended regs */
    VGAOUT8(vgaCRIndex, 0x66);
    VGAOUT8(vgaCRReg, restore->CR66);
    VGAOUT8(vgaCRIndex, 0x3a);
    VGAOUT8(vgaCRReg, restore->CR3A);
    VGAOUT8(vgaCRIndex, 0x31);
    VGAOUT8(vgaCRReg, restore->CR31);
    VGAOUT8(vgaCRIndex, 0x32);
    VGAOUT8(vgaCRReg, restore->CR32);
    VGAOUT8(vgaCRIndex, 0x58);
    VGAOUT8(vgaCRReg, restore->CR58);
    VGAOUT8(vgaCRIndex, 0x53);
    VGAOUT8(vgaCRReg, restore->CR53 & 0x7f);

    VGAOUT16(0x3c4, 0x0608);

    /* Restore DCLK registers. */

    VGAOUT8(0x3c4, 0x0e);
    VGAOUT8(0x3c5, restore->SR0E);
    VGAOUT8(0x3c4, 0x0f);
    VGAOUT8(0x3c5, restore->SR0F);
    VGAOUT8(0x3c4, 0x29);
    VGAOUT8(0x3c5, restore->SR29);
    VGAOUT8(0x3c4, 0x15);
    VGAOUT8(0x3c5, restore->SR15);

    /* Restore flat panel expansion registers. */
    if( S3_SAVAGE_MOBILE_SERIES(psav->Chipset) ||
	S3_MOBILE_TWISTER_SERIES(psav->Chipset)) {
	int i;
	for( i = 0; i < 8; i++ ) {
	    VGAOUT8(0x3c4, 0x54+i);
	    VGAOUT8(0x3c5, restore->SR54[i]);
	}
    }

    /* restore the standard vga regs */
    if (xf86IsPrimaryPci(psav->PciInfo))
	vgaHWRestore(pScrn, vgaSavePtr, VGA_SR_ALL);
    else
	vgaHWRestore(pScrn, vgaSavePtr, VGA_SR_MODE);

    /* extended mode timing registers */
    VGAOUT8(vgaCRIndex, 0x53);
    VGAOUT8(vgaCRReg, restore->CR53);
    VGAOUT8(vgaCRIndex, 0x5d);
    VGAOUT8(vgaCRReg, restore->CR5D);
    VGAOUT8(vgaCRIndex, 0x5e);
    VGAOUT8(vgaCRReg, restore->CR5E);
    VGAOUT8(vgaCRIndex, 0x3b);
    VGAOUT8(vgaCRReg, restore->CR3B);
    VGAOUT8(vgaCRIndex, 0x3c);
    VGAOUT8(vgaCRReg, restore->CR3C);
    VGAOUT8(vgaCRIndex, 0x43);
    VGAOUT8(vgaCRReg, restore->CR43);
    VGAOUT8(vgaCRIndex, 0x65);
    VGAOUT8(vgaCRReg, restore->CR65);

    /* restore the desired video mode with cr67 */
    VGAOUT8(vgaCRIndex, 0x67);
    /*VGAOUT8(vgaCRReg, restore->CR67 & ~0x0c);*/ /* no STREAMS yet */
    VGAOUT8(vgaCRReg, restore->CR67 & ~0x0e); /* no streams for new and old streams engines */

    /* other mode timing and extended regs */
    VGAOUT8(vgaCRIndex, 0x34);
    VGAOUT8(vgaCRReg, restore->CR34);
    VGAOUT8(vgaCRIndex, 0x40);
    VGAOUT8(vgaCRReg, restore->CR40);
    VGAOUT8(vgaCRIndex, 0x42);
    VGAOUT8(vgaCRReg, restore->CR42);
    VGAOUT8(vgaCRIndex, 0x45);
    VGAOUT8(vgaCRReg, restore->CR45);
    VGAOUT8(vgaCRIndex, 0x50);
    VGAOUT8(vgaCRReg, restore->CR50);
    VGAOUT8(vgaCRIndex, 0x51);
    VGAOUT8(vgaCRReg, restore->CR51);

    /* memory timings */
    VGAOUT8(vgaCRIndex, 0x36);
    VGAOUT8(vgaCRReg, restore->CR36);
    VGAOUT8(vgaCRIndex, 0x60);
    VGAOUT8(vgaCRReg, restore->CR60);
    VGAOUT8(vgaCRIndex, 0x68);
    VGAOUT8(vgaCRReg, restore->CR68);
    VerticalRetraceWait();
    VGAOUT8(vgaCRIndex, 0x69);
    VGAOUT8(vgaCRReg, restore->CR69);
    VGAOUT8(vgaCRIndex, 0x6f);
    VGAOUT8(vgaCRReg, restore->CR6F);

    VGAOUT8(vgaCRIndex, 0x33);
    VGAOUT8(vgaCRReg, restore->CR33);
    VGAOUT8(vgaCRIndex, 0x86);
    VGAOUT8(vgaCRReg, restore->CR86);
    VGAOUT8(vgaCRIndex, 0x88);
    VGAOUT8(vgaCRReg, restore->CR88);
    VGAOUT8(vgaCRIndex, 0x90);
    VGAOUT8(vgaCRReg, restore->CR90);
    VGAOUT8(vgaCRIndex, 0x91);
    VGAOUT8(vgaCRReg, restore->CR91);
    if( psav->Chipset == S3_SAVAGE4 )
    {
	VGAOUT8(vgaCRIndex, 0xb0);
	VGAOUT8(vgaCRReg, restore->CRB0);
    }

    VGAOUT8(vgaCRIndex, 0x32);
    VGAOUT8(vgaCRReg, restore->CR32);

    /* unlock extended seq regs */
    VGAOUT8(0x3c4, 0x08);
    VGAOUT8(0x3c5, 0x06);

    /* Restore extended sequencer regs for MCLK. SR10 == 255 indicates that 
     * we should leave the default SR10 and SR11 values there.
     */
    if (restore->SR10 != 255) {
	VGAOUT8(0x3c4, 0x10);
	VGAOUT8(0x3c5, restore->SR10);
	VGAOUT8(0x3c4, 0x11);
	VGAOUT8(0x3c5, restore->SR11);
    }

    /* restore extended seq regs for dclk */
    VGAOUT8(0x3c4, 0x0e);
    VGAOUT8(0x3c5, restore->SR0E);
    VGAOUT8(0x3c4, 0x0f);
    VGAOUT8(0x3c5, restore->SR0F);
    VGAOUT8(0x3c4, 0x12);
    VGAOUT8(0x3c5, restore->SR12);
    VGAOUT8(0x3c4, 0x13);
    VGAOUT8(0x3c5, restore->SR13);
    VGAOUT8(0x3c4, 0x29);
    VGAOUT8(0x3c5, restore->SR29);

    VGAOUT8(0x3c4, 0x18);
    VGAOUT8(0x3c5, restore->SR18);
    VGAOUT8(0x3c4, 0x1b);
    if( psav->DGAactive )
	VGAOUT8(0x3c5, restore->SR1B & ~0x08 );
    else
	VGAOUT8(0x3c5, restore->SR1B);

    /* load new m, n pll values for dclk & mclk */
    VGAOUT8(0x3c4, 0x15);
    tmp = VGAIN8(0x3c5) & ~0x21;

    VGAOUT8(0x3c5, tmp | 0x03);
    VGAOUT8(0x3c5, tmp | 0x23);
    VGAOUT8(0x3c5, tmp | 0x03);
    VGAOUT8(0x3c5, restore->SR15);
    usleep( 100 );

    VGAOUT8(0x3c4, 0x30);
    VGAOUT8(0x3c5, restore->SR30);
    VGAOUT8(0x3c4, 0x08);
    VGAOUT8(0x3c5, restore->SR08);

    /* now write out cr67 in full, possibly starting STREAMS */
    VerticalRetraceWait();
    VGAOUT8(vgaCRIndex, 0x67);
#if 0
    VGAOUT8(vgaCRReg, 0x50);
    usleep(10000);
    VGAOUT8(vgaCRIndex, 0x67);
#endif
    VGAOUT8(vgaCRReg, restore->CR67);

    VGAOUT8(vgaCRIndex, 0x66);
    cr66 = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRReg, cr66 | 0x80);
    VGAOUT8(vgaCRIndex, 0x3a);
    cr3a = VGAIN8(vgaCRReg);
    VGAOUT8(vgaCRReg, cr3a | 0x80);

    if (Entering)
	SavageGEReset(pScrn,0,__LINE__,__FILE__);

    if( !S3_SAVAGE_MOBILE_SERIES(psav->Chipset) )
    {
	VerticalRetraceWait();
	OUTREG(FIFO_CONTROL_REG, restore->MMPR0);
	OUTREG(MIU_CONTROL_REG, restore->MMPR1);
	OUTREG(STREAMS_TIMEOUT_REG, restore->MMPR2);
	OUTREG(MISC_TIMEOUT_REG, restore->MMPR3);
    }

    /* If we're going into graphics mode and acceleration was enabled, */
    /* go set up the BCI buffer and the global bitmap descriptor. */

#if 0
    if( Entering && (!psav->NoAccel) )
    {
	VGAOUT8(vgaCRIndex, 0x50);
	VGAOUT8(vgaCRReg, VGAIN8(vgaCRReg) | 0xC1);
	SavageInitialize2DEngine(pScrn);
    }
#endif

    VGAOUT8(vgaCRIndex, 0x66);
    VGAOUT8(vgaCRReg, cr66);
    VGAOUT8(vgaCRIndex, 0x3a);
    VGAOUT8(vgaCRReg, cr3a);

    if( Entering ) {
	SavageInitialize2DEngine(pScrn);

	VGAOUT16(vgaCRIndex, 0x0140);

	SavageSetGBD(pScrn);
    }

    vgaHWProtect(pScrn, FALSE);


#ifdef SAVAGEDRI
    if (psav->directRenderingEnabled)
        DRIUnlock(xf86ScrnToScreen(pScrn));
    psav->LockHeld = 0;
#endif

    return;
}


static Bool SavageMapMem(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    int err;

    TRACE(("SavageMapMem()\n"));

    if( S3_SAVAGE3D_SERIES(psav->Chipset) ) {
#ifdef XSERVER_LIBPCIACCESS
        psav->MmioRegion.base = SAVAGE_NEWMMIO_REGBASE_S3
            + psav->PciInfo->regions[0].base_addr;
        psav->FbRegion.base = psav->PciInfo->regions[0].base_addr;
#else
        psav->MmioRegion.base = SAVAGE_NEWMMIO_REGBASE_S3
            + psav->PciInfo->memBase[0];
        psav->FbRegion.base = psav->PciInfo->memBase[0];
#endif
    } else {
#ifdef XSERVER_LIBPCIACCESS
        psav->MmioRegion.base = SAVAGE_NEWMMIO_REGBASE_S4
            + psav->PciInfo->regions[0].base_addr;
        psav->FbRegion.base = psav->PciInfo->regions[1].base_addr;
#else
        psav->MmioRegion.base = SAVAGE_NEWMMIO_REGBASE_S4 
            + psav->PciInfo->memBase[0];
        psav->FbRegion.base = psav->PciInfo->memBase[1];
#endif
    }

    psav->MmioRegion.size = SAVAGE_NEWMMIO_REGSIZE;
    psav->FbRegion.size = psav->videoRambytes;

    /* On Paramount and Savage 2000, aperture 0 is PCI base 2.  On other
     * chipsets it's in the same BAR as the framebuffer.
     */

    psav->ApertureRegion.size = (psav->IsPrimary || psav->IsSecondary)
        ? (0x01000000 * 2) : (0x01000000 * 5);

    if ((psav->Chipset == S3_SUPERSAVAGE) 
        || (psav->Chipset == S3_SAVAGE2000)) {
#ifdef XSERVER_LIBPCIACCESS
        psav->ApertureRegion.base = psav->PciInfo->regions[2].base_addr;
        if (psav->ApertureRegion.size > psav->PciInfo->regions[2].size)
            psav->ApertureRegion.size = psav->PciInfo->regions[2].size;
#else
        psav->ApertureRegion.base = psav->PciInfo->memBase[2];
#endif
    } else {
        psav->ApertureRegion.base = psav->FbRegion.base + 0x02000000;
    }



    if (psav->FbRegion.size != 0) {
#ifdef XSERVER_LIBPCIACCESS
        err = pci_device_map_range(psav->PciInfo, psav->FbRegion.base,
                                   psav->FbRegion.size,
                                   (PCI_DEV_MAP_FLAG_WRITABLE
                                    | PCI_DEV_MAP_FLAG_WRITE_COMBINE),
                                   & psav->FbRegion.memory);
#else
        psav->FbRegion.memory = 
            xf86MapPciMem(pScrn->scrnIndex, VIDMEM_FRAMEBUFFER,
                          psav->PciTag, psav->FbRegion.base,
                          psav->FbRegion.size);
        err = (psav->FbRegion.memory == NULL) ? errno : 0;
#endif
        if (err) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Internal error: could not map framebuffer range (%d, %s).\n",
                       err, strerror(err));
            return FALSE;
        }

        psav->FBBase = psav->FbRegion.memory;
        psav->FBStart = (psav->IsSecondary)
            ? psav->FBBase + 0x1000000 : psav->FBBase;
    }

    if (psav->ApertureRegion.memory == NULL) {
#ifdef XSERVER_LIBPCIACCESS
        err = pci_device_map_range(psav->PciInfo, psav->ApertureRegion.base,
                                   psav->ApertureRegion.size,
                                   (PCI_DEV_MAP_FLAG_WRITABLE
                                    | PCI_DEV_MAP_FLAG_WRITE_COMBINE),
                                   & psav->ApertureRegion.memory);
#else
        psav->ApertureRegion.memory = 
            xf86MapPciMem(pScrn->scrnIndex, VIDMEM_FRAMEBUFFER,
                          psav->PciTag, psav->ApertureRegion.base,
                          psav->ApertureRegion.size);
        err = (psav->ApertureRegion.memory == NULL) ? errno : 0;
#endif
        if (err) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Internal error: could not map aperture range (%d, %s).\n",
                       err, strerror(err));
            return FALSE;
        }

        psav->ApertureMap = (psav->IsSecondary)
            ? psav->ApertureRegion.memory + 0x1000000
            : psav->ApertureRegion.memory;
    }

    if (psav->MmioRegion.memory == NULL) {
#ifdef XSERVER_LIBPCIACCESS
        err = pci_device_map_range(psav->PciInfo, psav->MmioRegion.base,
                                   psav->MmioRegion.size,
                                   (PCI_DEV_MAP_FLAG_WRITABLE),
                                   & psav->MmioRegion.memory);
#else
        psav->MmioRegion.memory = 
            xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO,
                          psav->PciTag, psav->MmioRegion.base,
                          psav->MmioRegion.size);
        err = (psav->MmioRegion.memory == NULL) ? errno : 0;
#endif
        if (err) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Internal error: could not map MMIO range (%d, %s).\n",
                       err, strerror(err));
            return FALSE;
        }

        psav->MapBase = psav->MmioRegion.memory;
        psav->BciMem = psav->MapBase + 0x10000;

        SavageEnableMMIO(pScrn);
    }

    pScrn->memPhysBase = psav->FbRegion.base;
    return TRUE;
}


static void SavageUnmapMem(ScrnInfoPtr pScrn, int All)
{
    SavagePtr psav = SAVPTR(pScrn);

    TRACE(("SavageUnmapMem(%p,%p)\n", psav->MapBase, psav->FBBase));

    if (psav->PrimaryVidMapped) {
        vgaHWUnmapMem(pScrn);
        psav->PrimaryVidMapped = FALSE;
    }

    SavageDisableMMIO(pScrn);

    if (All && (psav->MmioRegion.memory != NULL)) {
#ifdef XSERVER_LIBPCIACCESS
        pci_device_unmap_range(psav->PciInfo,
                               psav->MmioRegion.memory,
                               psav->MmioRegion.size);
#else
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer)psav->MapBase,
                        SAVAGE_NEWMMIO_REGSIZE);
#endif

        psav->MmioRegion.memory = NULL;
        psav->MapBase = 0;
        psav->BciMem = 0;
    }

    if (psav->FbRegion.memory != NULL) {
#ifdef XSERVER_LIBPCIACCESS
        pci_device_unmap_range(psav->PciInfo,
                               psav->FbRegion.memory,
                               psav->FbRegion.size);
#else
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer)psav->FbRegion.base,
                        psav->FbRegion.size);
#endif
    }

    if (psav->ApertureRegion.memory != NULL) {
#ifdef XSERVER_LIBPCIACCESS
        pci_device_unmap_range(psav->PciInfo,
                               psav->ApertureRegion.memory,
                               psav->ApertureRegion.size);
#else
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer)psav->ApertureRegion.base,
                        psav->ApertureRegion.size);
#endif
    }

    psav->FbRegion.memory = NULL;
    psav->ApertureRegion.memory = NULL;
    psav->FBBase = 0;
    psav->FBStart = 0;
    psav->ApertureMap = 0;

    return;
}

#ifdef SAVAGEDRI
static Bool SavageCheckAvailableRamFor3D(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    int cpp = pScrn->bitsPerPixel / 8;
    int tiledBufferSize, RamNeededFor3D;

    if (cpp == 2) {
        tiledBufferSize = ((pScrn->virtualX+63)/64)*((pScrn->virtualY+15)/16) * 2048;
    } else {
        tiledBufferSize = ((pScrn->virtualX+31)/32)*((pScrn->virtualY+15)/16) * 2048;
    }

    RamNeededFor3D = 4096 + /* hw cursor*/
                     psav->cobSize + /*COB*/
                     tiledBufferSize + /* front buffer */
                     tiledBufferSize + /* back buffer */
                     tiledBufferSize; /* depth buffer */

    xf86DrvMsg(pScrn->scrnIndex,X_INFO,
		"%d kB of Videoram needed for 3D; %d kB of Videoram available\n", 
		RamNeededFor3D/1024, psav->videoRambytes/1024);

    if (RamNeededFor3D <= psav->videoRambytes) {
        xf86DrvMsg(pScrn->scrnIndex,X_INFO,"Sufficient Videoram available for 3D\n");
	return TRUE;
    } else {
        xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"Insufficient Videoram available for 3D -- "
					"Try a lower color depth or smaller desktop.  "
			"For integrated savages try increasing the videoram in the BIOS.\n");
	return FALSE;
    }
}
#endif

static void SavageInitStatus(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);

    switch( psav->Chipset ) {
	case S3_SAVAGE3D:
	case S3_SAVAGE_MX:
	    psav->WaitQueue	= WaitQueue3D;
	    psav->WaitIdle	= WaitIdle3D;
	    psav->WaitIdleEmpty	= WaitIdleEmpty3D;
	    psav->bciUsedMask   = 0x1ffff;
	    psav->eventStatusReg= 1;
	    break;

	case S3_SAVAGE4:
	case S3_PROSAVAGE:
	case S3_SUPERSAVAGE:
	case S3_PROSAVAGEDDR:
	case S3_TWISTER:
	    psav->WaitQueue	= WaitQueue4;
	    psav->WaitIdle	= WaitIdle4;
	    psav->WaitIdleEmpty	= WaitIdleEmpty4;
	    psav->bciUsedMask   = 0x1fffff;
	    psav->eventStatusReg= 1;
	    break;

	case S3_SAVAGE2000:
	    psav->WaitQueue	= WaitQueue2K;
	    psav->WaitIdle	= WaitIdle2K;
	    psav->WaitIdleEmpty	= WaitIdleEmpty2K;
	    psav->bciUsedMask   = 0xfffff;
	    psav->eventStatusReg= 2;
	    break;
    }
}

static void SavageInitShadowStatus(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);

    psav->ShadowStatus = psav->ConfigShadowStatus;

    SavageInitStatus(pScrn);

    if( psav->ShadowStatus ) {
	psav->ShadowPhysical = 
	    psav->FbRegion.base + psav->CursorKByte*1024 + 4096 - 32;
	
	psav->ShadowVirtual = (CARD32 *)
	    (psav->FBBase + psav->CursorKByte*1024 + 4096 - 32);
	
	xf86DrvMsg( pScrn->scrnIndex, X_PROBED,
		    "Shadow area physical %08lx, linear %p\n",
		    psav->ShadowPhysical, (void *)psav->ShadowVirtual );

	psav->WaitQueue = ShadowWaitQueue;
	psav->WaitIdle = ShadowWait;
	psav->WaitIdleEmpty = ShadowWait;
    }

    if( psav->Chipset == S3_SAVAGE2000 )
	psav->dwBCIWait2DIdle = 0xc0040000;
    else
	psav->dwBCIWait2DIdle = 0xc0020000;
}

static Bool SavageScreenInit(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    SavagePtr psav;
    EntityInfoPtr pEnt;
    int ret;
    int colormapFlags;

    TRACE(("SavageScreenInit()\n"));

    psav = SAVPTR(pScrn);

    pEnt = xf86GetEntityInfo(pScrn->entityList[0]); 
    if (!psav->pVbe)
	psav->pVbe = VBEInit(NULL, pEnt->index);
 
    SavageEnableMMIO(pScrn);

    if (!SavageMapMem(pScrn))
	return FALSE;

    psav->FBStart2nd = 0;

    if (psav->overlayDepth) {
	if ((pScrn->virtualX * pScrn->virtualY *
	     (DEPTH_BPP(DEPTH_2ND(pScrn))) >> 3)
	     > (psav->CursorKByte * 1024))
	    xf86DrvMsg(pScrn->scrnIndex,X_WARNING,
		       "Not enough memory for overlay mode: disabling\n");
	else psav->FBStart2nd  = psav->FBStart
		 + ((pScrn->virtualX * pScrn->virtualY + 0xff) & ~0xff);

    }

    SavageInitShadowStatus(pScrn);
    psav->ShadowCounter = 0;

    SavageSave(pScrn);

    vgaHWBlankScreen(pScrn, TRUE);

#ifdef SAVAGEDRI
    if (!xf86ReturnOptValBool(psav->Options, OPTION_DRI, TRUE)) {
	psav->directRenderingEnabled = FALSE;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Direct rendering forced off\n");
    } else if (psav->IsSecondary) {
	psav->directRenderingEnabled = FALSE;
    } else if (xf86IsEntityShared(psav->pEnt->index)) {
	    /* Xinerama has sync problem with DRI, disable it for now */
	    psav->directRenderingEnabled = FALSE;
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			"Direct Rendering Disabled -- "
			"Dual-head configuration is not working with "
			"DRI at present.\n");
    } else if (/*!psav->bTiled*/psav->bDisableTile) {
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
	    		"Direct Rendering requires a tiled framebuffer -- "
			"Set Option \"DisableTile\" \"false\"\n");
    } else if (psav->cobSize == 0) {
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
	    		"Direct Rendering requires the COB -- "
			"Set Option \"DisableCOB\" \"false\"\n");
    } else if (((psav->Chipset == S3_TWISTER)
        || (psav->Chipset == S3_PROSAVAGE)
        || (psav->Chipset == S3_SAVAGE4)
        || (psav->Chipset == S3_SAVAGE_MX)
	|| (psav->Chipset == S3_SAVAGE3D)
	|| (psav->Chipset == S3_SUPERSAVAGE)
        || (psav->Chipset == S3_PROSAVAGEDDR))
	&& (!psav->NoAccel)
	&& (SavageCheckAvailableRamFor3D(pScrn))) {
        /* Setup DRI after visuals have been established */
        psav->directRenderingEnabled = SAVAGEDRIScreenInit(pScreen);
	/* If DRI init failed, reset shadow status. */
	if (!psav->directRenderingEnabled &&
	    psav->ShadowStatus != psav->ConfigShadowStatus) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Resetting ShadowStatus.\n");
	    SavageInitShadowStatus(pScrn);
	}
	/* If shadow status was enabled for DRI, hook up the shadow
	 * waiting functions now. */
	else if (psav->ShadowStatus && !psav->ConfigShadowStatus) {
	    psav->WaitQueue = ShadowWaitQueue;
	    psav->WaitIdle = ShadowWait;
	    psav->WaitIdleEmpty = ShadowWait;
	}
    } else
        psav->directRenderingEnabled = FALSE;
    
    if(psav->directRenderingEnabled) {
        xf86DrvMsg(pScrn->scrnIndex,X_CONFIG,"DRI is enabled\n");
    }
    else {
        xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"DRI isn't enabled\n");
    }
#endif

    if (!SavageModeInit(pScrn, pScrn->currentMode))
	return FALSE;

    miClearVisualTypes();

    {
 	int visual;

 	visual = ((psav->FBStart2nd && pScrn->bitsPerPixel > 8)
		   || pScrn->bitsPerPixel == 16) ? TrueColorMask
	    : miGetDefaultVisualMask(DEPTH_BPP(pScrn->depth));
 	if (!miSetVisualTypes(pScrn->depth, visual,
  			      pScrn->rgbBits, pScrn->defaultVisual))
  	    return FALSE;
 	
 	if (psav->FBStart2nd) {/* we have overlay */	
 	    visual = psav->overlayDepth > 8 ? TrueColorMask :
 		miGetDefaultVisualMask(DEPTH_BPP(psav->overlayDepth));
 	    if (!miSetVisualTypes(psav->overlayDepth, visual,
 				  psav->overlayDepth > 8 ? 8 : 6, 
 				  pScrn->defaultVisual))
 		return FALSE;
 	}
     }
     if (!miSetPixmapDepths ())
	 return FALSE;

    ret = SavageInternalScreenInit(pScreen);
    if (!ret)
	return FALSE;

    xf86SetBlackWhitePixels(pScreen);

    {
	VisualPtr visual;
	visual = pScreen->visuals + pScreen->numVisuals;
	while (--visual >= pScreen->visuals) {
	    if ((visual->class | DynamicClass) == DirectColor
		&& visual->nplanes > MAX_PSEUDO_DEPTH) {
		if (visual->nplanes == pScrn->depth) {
		    visual->offsetRed = pScrn->offset.red;
		    visual->offsetGreen = pScrn->offset.green;
		    visual->offsetBlue = pScrn->offset.blue;
		    visual->redMask = pScrn->mask.red;
		    visual->greenMask = pScrn->mask.green;
		    visual->blueMask = pScrn->mask.blue;
		} else if (visual->offsetRed > 8 
			   || visual->offsetGreen > 8
			   || visual->offsetBlue > 8) {
	/*
	 * mi has set these wrong. fix it here -- we cannot use pScrn
	 * as this is set up for the default depth 8.
	 */
		    int tmp;
		    int c_s = 0;
		    
		    tmp = visual->offsetBlue;
		    visual->offsetBlue = visual->offsetRed;
		    visual->offsetRed = tmp;
		    tmp = visual->blueMask;
		    visual->blueMask = visual->redMask;
		    visual->redMask = tmp;
		    switch (DEPTH_2ND(pScrn)) {
			case 16:
			    visual->offsetRed = 11;
			    visual->offsetGreen = 5;
			    visual->offsetBlue = 0;
			    visual->redMask = 0xF800;
			    visual->greenMask = 0x7E0;
			    visual->blueMask = 0x1F;
			    break;
			case 24:
			    visual->offsetRed = 16;
			    visual->offsetGreen = 8;
			    visual->offsetBlue = 0;
			    visual->redMask = 0xFF0000;
			    visual->greenMask = 0xFF00;
			    visual->blueMask = 0xFF;
			    c_s = 2;
			    break;
		    }
		    psav->overlay.redMask = visual->redMask;
		    psav->overlay.greenMask = visual->greenMask;
		    psav->overlay.blueMask = visual->blueMask;
		    psav->overlay.redShift = visual->offsetRed + c_s;
		    psav->overlay.greenShift = visual->offsetGreen + c_s;
		    psav->overlay.blueShift = visual->offsetBlue + c_s;
		}
	    }
	}
    }

    /* must be after RGB ordering fixed */
    fbPictureInit (pScreen, 0, 0);

    if( !psav->NoAccel ) {
	SavageInitAccel(pScreen);
    }

    xf86SetBackingStore(pScreen);

    if( !psav->shadowFB && !psav->useEXA )
	SavageDGAInit(pScreen);

    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    if (psav->hwcursor)
	if (!SavageHWCursorInit(pScreen))
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	       "Hardware cursor initialization failed\n");

    if (psav->shadowFB) {
	RefreshAreaFuncPtr refreshArea = SavageRefreshArea;
      
	if(psav->rotate) {
	    if (!psav->PointerMoved) {
		psav->PointerMoved = pScrn->PointerMoved;
		pScrn->PointerMoved = SavagePointerMoved;
	    }

	    switch(pScrn->bitsPerPixel) {
	    case 8:	refreshArea = SavageRefreshArea8;	break;
	    case 16:	refreshArea = SavageRefreshArea16;	break;
	    case 24:	refreshArea = SavageRefreshArea24;	break;
	    case 32:	refreshArea = SavageRefreshArea32;	break;
	    }
	}
      
	ShadowFBInit(pScreen, refreshArea);
    }

    if (!miCreateDefColormap(pScreen))
	    return FALSE;

    colormapFlags =  CMAP_RELOAD_ON_MODE_SWITCH
	| ((psav->FBStart2nd) ? 0 : CMAP_PALETTED_TRUECOLOR);
    
    if (psav->Chipset == S3_SAVAGE4) {
        if (!xf86HandleColormaps(pScreen, 256, pScrn->rgbBits, SavageLoadPaletteSavage4,
				 NULL, colormapFlags ))
	    return FALSE;
    } else {
        if (!xf86HandleColormaps(pScreen, 256, pScrn->rgbBits, SavageLoadPalette, NULL,
				 colormapFlags ))
 	    return FALSE;
    }

    vgaHWBlankScreen(pScrn, FALSE);

    psav->CloseScreen = pScreen->CloseScreen;
    pScreen->SaveScreen = SavageSaveScreen;
    pScreen->CloseScreen = SavageCloseScreen;

    if (xf86DPMSInit(pScreen, SavageDPMS, 0) == FALSE)
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "DPMS initialization failed\n");

#ifdef SAVAGEDRI
    if (psav->directRenderingEnabled) {
        /* complete the DRI setup.*/
        psav->directRenderingEnabled = SAVAGEDRIFinishScreenInit(pScreen);
	/* If DRI initialization failed, reset shadow status and
	 * reinitialize 2D engine. */
	if (!psav->directRenderingEnabled &&
	    psav->ShadowStatus != psav->ConfigShadowStatus) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Resetting ShadowStatus.\n");
	    SavageInitShadowStatus(pScrn);
	    SavageInitialize2DEngine(pScrn);
	}
    }
    if (psav->directRenderingEnabled) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Direct rendering enabled\n");
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Direct rendering disabled\n");
    }
#endif

    SavagePanningCheck(pScrn, pScrn->currentMode);
#ifdef XvExtension
    if( !psav->FBStart2nd && !psav->NoAccel  /*&& !SavagePanningCheck(pScrn)*/ ) {
	if (psav->IsSecondary)
            /* Xv should work on crtc2, but I haven't gotten there yet.  -- AGD */
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Xv currently disabled for crtc2.\n");
	else
	    SavageInitVideo( pScreen );
    }
#endif

#ifdef SAVAGEDRI
    if ((psav->directRenderingEnabled) && (!psav->bDisableXvMC)) {
        if (SAVAGEInitMC(pScreen))
            xf86DrvMsg(pScrn->scrnIndex,X_CONFIG,"XvMC is enabled\n");
        else
            xf86DrvMsg(pScrn->scrnIndex,X_CONFIG,"XvMC is not enabled\n");
    }

    if (!psav->directRenderingEnabled && psav->AGPforXv) {
        xf86DrvMsg(pScrn->scrnIndex,X_ERROR,"AGPforXV requires DRI to be enabled.\n");
	psav->AGPforXv = FALSE;
    }
#endif

    if (serverGeneration == 1)
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);

    return TRUE;
}


static int SavageInternalScreenInit(ScreenPtr pScreen)
{
    int ret = TRUE;
    ScrnInfoPtr pScrn;
    SavagePtr psav;
    int width, height, displayWidth;
    unsigned char *FBStart;

    TRACE(("SavageInternalScreenInit()\n"));

    pScrn = xf86ScreenToScrn(pScreen);
    psav = SAVPTR(pScrn);

    displayWidth = pScrn->displayWidth;

    if (psav->rotate) {
	height = pScrn->virtualX;
	width = pScrn->virtualY;
    } else {
	width = pScrn->virtualX;
	height = pScrn->virtualY;
    }
  
  
    if(psav->shadowFB) {
	psav->ShadowPitch = BitmapBytePad(pScrn->bitsPerPixel * width);
	psav->ShadowPtr = malloc(psav->ShadowPitch * height);
	displayWidth = psav->ShadowPitch / (pScrn->bitsPerPixel >> 3);
	FBStart = psav->ShadowPtr;
    } else {
	psav->ShadowPtr = NULL;
	FBStart = psav->FBStart;
    }

    if (!psav->FBStart2nd) {

        ret = fbScreenInit(pScreen, FBStart, width, height,
                           pScrn->xDpi, pScrn->yDpi,
                           psav->ulAperturePitch / (pScrn->bitsPerPixel >> 3), /*displayWidth,*/
                           pScrn->bitsPerPixel);

    } else { 
	FbOverlayScrPrivPtr pScrPriv;
	int Depth2nd = DEPTH_2ND(pScrn);
	if (!fbSetupScreen (pScreen, FBStart, width, height, 
			    pScrn->xDpi, pScrn->yDpi, displayWidth, 8))
	    return FALSE;
	if (pScrn->depth == 8) {
	    ret = fbOverlayFinishScreenInit (pScreen, FBStart, 
					     psav->FBStart2nd, width, 
					     height,pScrn->xDpi, pScrn->yDpi,
					     displayWidth,displayWidth,
					     8, DEPTH_BPP(Depth2nd),
					     8, Depth2nd);
	    pScrPriv = fbOverlayGetScrPriv(pScreen);
	    pScrPriv->layer[0].key = pScrn->colorKey;
	} else {
	    ret = fbOverlayFinishScreenInit (pScreen, psav->FBStart2nd,
					     FBStart, 
					     width, height,pScrn->xDpi, 
					     pScrn->yDpi,
					     displayWidth,displayWidth,
					     DEPTH_BPP(Depth2nd), 8,
					     Depth2nd, 8);
	    pScrPriv = fbOverlayGetScrPriv(pScreen);
	    pScrPriv->layer[1].key = pScrn->colorKey;
	}
    }
    return ret;
}


static int SavageGetRefresh(DisplayModePtr mode)
{
    int refresh = (mode->Clock * 1000) / (mode->HTotal * mode->VTotal);
    if (mode->Flags & V_INTERLACE)
	refresh *= 2.0;
    if (mode->Flags & V_DBLSCAN)
	refresh /= 2.0;
    if (mode->VScan > 1)
	refresh /= mode->VScan;
    return refresh;
}


static ModeStatus SavageValidMode(SCRN_ARG_TYPE arg, DisplayModePtr pMode,
				  Bool verbose, int flags)
{
    SCRN_INFO_PTR(arg);
    SavagePtr psav = SAVPTR(pScrn);
    int refresh;

    TRACE(("SavageValidMode\n"));

    /* We prohibit modes bigger than the LCD panel. */
    /* TODO We should do this only if the panel is active. */

    if( psav->TvOn )
    {
	if( pMode->HDisplay > psav->TVSizeX )
	    return MODE_VIRTUAL_X;

	if( pMode->VDisplay > psav->TVSizeY )
	    return MODE_VIRTUAL_Y;

    }

    if((psav->DisplayType == MT_LCD) &&
      ((pMode->HDisplay > psav->PanelX) ||
       (pMode->VDisplay > psav->PanelY)))
	    return MODE_PANEL;

    if (psav->UseBIOS) {
	refresh = SavageGetRefresh(pMode);
        return (SavageMatchBiosMode(pScrn,pMode->HDisplay,
                                   pMode->VDisplay,
                                   refresh,NULL,NULL));
    }

    return MODE_OK;
}

static Bool SavageModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    SavagePtr psav = SAVPTR(pScrn);
    int width, dclk, i, j; /*, refresh; */
    unsigned int m, n, r;
    unsigned char tmp = 0;
    SavageRegPtr new = &psav->ModeReg;
    vgaRegPtr vganew = &hwp->ModeReg;
    int vgaCRIndex, vgaCRReg, vgaIOBase;
    int refresh;
    unsigned int newmode=0, newrefresh=0;

    vgaIOBase = hwp->IOBase;
    vgaCRIndex = vgaIOBase + 4;
    vgaCRReg = vgaIOBase + 5;

    TRACE(("SavageModeInit(%dx%d, %dkHz)\n", 
	mode->HDisplay, mode->VDisplay, mode->Clock));
    
#if 0
    ErrorF("Clock = %d, HDisplay = %d, HSStart = %d\n",
	    mode->Clock, mode->HDisplay, mode->HSyncStart);
    ErrorF("HSEnd = %d, HSkew = %d\n",
	    mode->HSyncEnd, mode->HSkew);
    ErrorF("VDisplay - %d, VSStart = %d, VSEnd = %d\n",
	    mode->VDisplay, mode->VSyncStart, mode->VSyncEnd);
    ErrorF("VTotal = %d\n",
	    mode->VTotal);
    ErrorF("HDisplay = %d, HSStart = %d\n",
	    mode->CrtcHDisplay, mode->CrtcHSyncStart);
    ErrorF("HSEnd = %d, HSkey = %d\n",
	    mode->CrtcHSyncEnd, mode->CrtcHSkew);
    ErrorF("VDisplay - %d, VSStart = %d, VSEnd = %d\n",
	    mode->CrtcVDisplay, mode->CrtcVSyncStart, mode->CrtcVSyncEnd);
    ErrorF("VTotal = %d\n",
	    mode->CrtcVTotal);
#endif

    if (psav->IsSecondary) {
	refresh = SavageGetRefresh(mode);

        SavageMatchBiosMode(pScrn,mode->HDisplay,mode->VDisplay,refresh,
                            &newmode,&newrefresh);
	new->mode = newmode;
	new->refresh = newrefresh;

        /* do it! */
        SavageWriteMode(pScrn, vganew, new, TRUE);
	pScrn->currentMode = mode;

        if (psav->FBStart2nd) {
	    SavageStreamsOn(pScrn);
	    SavageInitSecondaryStream(pScrn);
        }

        SavageAdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));
	return TRUE;
    }


    if (pScrn->bitsPerPixel == 8)
	psav->HorizScaleFactor = 1;
    else if (pScrn->bitsPerPixel == 16)
	psav->HorizScaleFactor = 1;	/* I don't think we ever want 2 */
    else
	psav->HorizScaleFactor = 1;

    if (psav->HorizScaleFactor == 2)
	if (!mode->CrtcHAdjusted) {
	    mode->CrtcHDisplay *= 2;
	    mode->CrtcHSyncStart *= 2;
	    mode->CrtcHSyncEnd *= 2;
	    mode->CrtcHBlankStart *= 2;
	    mode->CrtcHBlankEnd *= 2;
	    mode->CrtcHTotal *= 2;
	    mode->CrtcHSkew *= 2;
	    mode->CrtcHAdjusted = TRUE;
	}
    
    if (!vgaHWInit(pScrn, mode))
	return FALSE;

    new->mode = 0;

    /* We need to set CR67 whether or not we use the BIOS. */

    dclk = mode->Clock;
    new->CR67 = 0x00;

    switch( pScrn->depth ) {
    case 8:
	if( (psav->Chipset == S3_SAVAGE2000) && (dclk >= 230000) )
	    new->CR67 = 0x10;	/* 8bpp, 2 pixels/clock */
	else
	    new->CR67 = 0x00;	/* 8bpp, 1 pixel/clock */
	break;
    case 15:
	if( 
	    S3_SAVAGE_MOBILE_SERIES(psav->Chipset) ||
	    ((psav->Chipset == S3_SAVAGE2000) && (dclk >= 230000))
	)
	    new->CR67 = 0x30;	/* 15bpp, 2 pixel/clock */
	else
	    new->CR67 = 0x20;	/* 15bpp, 1 pixels/clock */
	break;
    case 16:
	if( 
	    S3_SAVAGE_MOBILE_SERIES(psav->Chipset) ||
	    ((psav->Chipset == S3_SAVAGE2000) && (dclk >= 230000))
	)
	    new->CR67 = 0x50;	/* 16bpp, 2 pixel/clock */
	else
	    new->CR67 = 0x40;	/* 16bpp, 1 pixels/clock */
	break;
    case 24:
	if (psav->primStreamBpp == 24 )
	    new->CR67 = 0x70;
	else
	    new->CR67 = 0xd0;
	break;
    }


    if( psav->UseBIOS ) {
	int refresh;
	unsigned int newmode=0, newrefresh=0;

	refresh = SavageGetRefresh(mode);

        SavageMatchBiosMode(pScrn,mode->HDisplay,mode->VDisplay,refresh,
                            &newmode,&newrefresh);
	new->mode = newmode;
	new->refresh = newrefresh;
    }

    if( !new->mode ) {
	/* 
	 * Either BIOS use is disabled, or we failed to find a suitable
	 * match.  Fall back to traditional register-crunching.
	 */

	VGAOUT8(vgaCRIndex, 0x3a);
	tmp = VGAIN8(vgaCRReg);
	if (psav->pci_burst)
	    new->CR3A = (tmp & 0x7f) | 0x15;
	else
	    new->CR3A = tmp | 0x95;

	new->CR53 = 0x00;
	new->CR31 = 0x8c;
	new->CR66 = 0x89;

	VGAOUT8(vgaCRIndex, 0x58);
	new->CR58 = VGAIN8(vgaCRReg) & 0x80;
	new->CR58 |= 0x13;

#if 0
	VGAOUT8(vgaCRIndex, 0x55);
	new->CR55 = VGAIN8(vgaCRReg);
	if (psav->hwcursor)
		new->CR55 |= 0x10;
#endif

	new->SR15 = 0x03 | 0x80;
	new->SR18 = 0x00;


	/* enable gamma correction */
	if( pScrn->depth == 24 )
	    new->SR1B = 0x18;
	else
	    new->SR1B = 0x00;

	/* set 8-bit CLUT */
	new->SR1B |= 0x10;
	
	new->CR43 = new->CR45 = new->CR65 = 0x00;

	VGAOUT8(vgaCRIndex, 0x40);
	new->CR40 = VGAIN8(vgaCRReg) & ~0x01;

	new->MMPR0 = 0x010400;
	new->MMPR1 = 0x00;
	new->MMPR2 = 0x0808;
	new->MMPR3 = 0x08080810;

	if (psav->fifo_aggressive || psav->fifo_moderate ||
	    psav->fifo_conservative) {
		new->MMPR1 = 0x0200;
		new->MMPR2 = 0x1808;
		new->MMPR3 = 0x08081810;
	}

	if (psav->MCLK <= 0) {
		new->SR10 = 255;
		new->SR11 = 255;
	}

	psav->NeedSTREAMS = FALSE;

	SavageCalcClock(dclk, 1, 1, 127, 0, 4, 180000, 360000,
			&m, &n, &r);
	new->SR12 = (r << 6) | (n & 0x3f);
	new->SR13 = m & 0xff;
	new->SR29 = (r & 4) | (m & 0x100) >> 5 | (n & 0x40) >> 2;

	if (psav->fifo_moderate) {
	    if (psav->primStreamBpp < 24)
		new->MMPR0 -= 0x8000;
	    else
		new->MMPR0 -= 0x4000;
	} else if (psav->fifo_aggressive) {
	    if (psav->primStreamBpp < 24)
		new->MMPR0 -= 0xc000;
	    else
		new->MMPR0 -= 0x6000;
	}

	if (mode->Flags & V_INTERLACE)
	    new->CR42 = 0x20;
	else
	    new->CR42 = 0x00;

	new->CR34 = 0x10;

	i = ((((mode->CrtcHTotal >> 3) - 5) & 0x100) >> 8) |
	    ((((mode->CrtcHDisplay >> 3) - 1) & 0x100) >> 7) |
	    ((((mode->CrtcHSyncStart >> 3) - 1) & 0x100) >> 6) |
	    ((mode->CrtcHSyncStart & 0x800) >> 7);

	if ((mode->CrtcHSyncEnd >> 3) - (mode->CrtcHSyncStart >> 3) > 64)
	    i |= 0x08;
	if ((mode->CrtcHSyncEnd >> 3) - (mode->CrtcHSyncStart >> 3) > 32)
	    i |= 0x20;
	j = (vganew->CRTC[0] + ((i & 0x01) << 8) +
	     vganew->CRTC[4] + ((i & 0x10) << 4) + 1) / 2;
	if (j - (vganew->CRTC[4] + ((i & 0x10) << 4)) < 4) {
	    if (vganew->CRTC[4] + ((i & 0x10) << 4) + 4 <= 
	        vganew->CRTC[0] + ((i & 0x01) << 8))
		j = vganew->CRTC[4] + ((i & 0x10) << 4) + 4;
	    else
		j = vganew->CRTC[0] + ((i & 0x01) << 8) + 1;
	}

	new->CR3B = j & 0xff;
	i |= (j & 0x100) >> 2;
	new->CR3C = (vganew->CRTC[0] + ((i & 0x01) << 8))  / 2 ;
	new->CR5D = i;
	new->CR5E = (((mode->CrtcVTotal - 2) & 0x400) >> 10) |
		    (((mode->CrtcVDisplay - 1) & 0x400) >> 9) |
		    (((mode->CrtcVSyncStart) & 0x400) >> 8) |
		    (((mode->CrtcVSyncStart) & 0x400) >> 6) | 0x40;
	width = (pScrn->displayWidth * (psav->primStreamBpp / 8)) >> 3;
	new->CR91 = vganew->CRTC[19] = 0xff & width;
	new->CR51 = (0x300 & width) >> 4;
	new->CR90 = 0x80 | (width >> 8);
	vganew->MiscOutReg |= 0x0c;

	/* Set frame buffer description. */

	if (psav->primStreamBpp <= 8)
	    new->CR50 = 0;
	else if (psav->primStreamBpp <= 16)
	    new->CR50 = 0x10;
	else
	    new->CR50 = 0x30;

	if (pScrn->displayWidth == 640)
	    new->CR50 |= 0x40;
	else if (pScrn->displayWidth == 800)
	    new->CR50 |= 0x80;
	else if (pScrn->displayWidth == 1024)
	    new->CR50 |= 0x00;
	else if (pScrn->displayWidth == 1152)
	    new->CR50 |= 0x01;
	else if (pScrn->displayWidth == 1280)
	    new->CR50 |= 0xc0;
	else if (pScrn->displayWidth == 1600)
	    new->CR50 |= 0x81;
	else
	    new->CR50 |= 0xc1;	/* Use GBD */

	if( S3_SAVAGE_MOBILE_SERIES(psav->Chipset) )
	    new->CR33 = 0x00;
	else
	    new->CR33 = 0x08;
	     
	vganew->CRTC[0x17] = 0xeb;

	new->CR67 |= 1;

	VGAOUT8(vgaCRIndex, 0x36);
	new->CR36 = VGAIN8(vgaCRReg);
	VGAOUT8(vgaCRIndex, 0x68);
	new->CR68 = VGAIN8(vgaCRReg);

	new->CR69 = 0;
	VGAOUT8(vgaCRIndex, 0x6f);
	new->CR6F = VGAIN8(vgaCRReg);
	VGAOUT8(vgaCRIndex, 0x86);
	new->CR86 = VGAIN8(vgaCRReg) | 0x08;
	VGAOUT8(vgaCRIndex, 0x88);
	new->CR88 = VGAIN8(vgaCRReg) | DISABLE_BLOCK_WRITE_2D;
	VGAOUT8(vgaCRIndex, 0xb0);
	new->CRB0 = VGAIN8(vgaCRReg) | 0x80;
    }

    pScrn->vtSema = TRUE;

    /* do it! */
    SavageWriteMode(pScrn, vganew, new, TRUE);
    pScrn->currentMode = mode;

    if (psav->FBStart2nd) {
        SavageStreamsOn(pScrn);
	SavageInitSecondaryStream(pScrn);
    }

    SavageAdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));

    return TRUE;
}


static Bool SavageCloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    SavagePtr psav = SAVPTR(pScrn);
    vgaRegPtr vgaSavePtr = &hwp->SavedReg;
    SavageRegPtr SavageSavePtr = &psav->SavedReg;

    TRACE(("SavageCloseScreen\n"));

#ifdef SAVAGEDRI
    if (psav->directRenderingEnabled) {
        SAVAGEDRICloseScreen(pScreen);
	/* reset shadow values */
	SavageInitShadowStatus(pScrn);
        psav->directRenderingEnabled=FALSE;
    }
#endif

    if (psav->EXADriverPtr) {
	exaDriverFini(pScreen);
	psav->EXADriverPtr = NULL;
    }

#ifdef HAVE_XAA_H
    if( psav->AccelInfoRec ) {
        XAADestroyInfoRec( psav->AccelInfoRec );
	psav->AccelInfoRec = NULL;
    }
#endif

    if( psav->DGAModes ) {
	free( psav->DGAModes );
	psav->DGAModes = NULL;
	psav->numDGAModes = 0;
    }

    if (pScrn->vtSema) {
        if (psav->FBStart2nd) 
	    SavageStreamsOff(pScrn);
	SavageWriteMode(pScrn, vgaSavePtr, SavageSavePtr, FALSE);
        SavageResetStreams(pScrn);
	vgaHWLock(hwp);
	SavageUnmapMem(pScrn, 0);
    }

    if (psav->pVbe)
      vbeFree(psav->pVbe);
    psav->pVbe = NULL;

    pScrn->vtSema = FALSE;
    pScreen->CloseScreen = psav->CloseScreen;

    return (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);
}


static Bool SavageSaveScreen(ScreenPtr pScreen, int mode)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);

    TRACE(("SavageSaveScreen(0x%x)\n", mode));

    if( pScrn->vtSema && SAVPTR(pScrn)->hwcursor && SAVPTR(pScrn)->hwc_on )
    {
	if( xf86IsUnblank(mode) )
	    SavageShowCursor( pScrn );
	else
	    SavageHideCursor( pScrn );
	SAVPTR(pScrn)->hwc_on = TRUE; /*restore */
    }

    return vgaHWSaveScreen(pScreen, mode);
}

void SavageAdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    SavagePtr psav = SAVPTR(pScrn);

    if (psav->IsSecondary) {
	SavageDoAdjustFrame(pScrn, x, y, TRUE);
    } else {
	SavageDoAdjustFrame(pScrn, x, y, FALSE);
    }

}

void
SavageDoAdjustFrame(ScrnInfoPtr pScrn, int x, int y, int crtc2)
{
    SavagePtr psav = SAVPTR(pScrn);
    int address=0,top=0,left=0,tile_height,tile_size;
    
    TRACE(("SavageDoAdjustFrame(%d,%d,%d)\n", x, y, crtc2));

    if (psav->Chipset == S3_SAVAGE2000) {
        tile_height = TILEHEIGHT_2000; /* 32 */
        tile_size = TILE_SIZE_BYTE_2000; /* 4096 */
    } else {
        tile_height = TILEHEIGHT; /* 16 */
        tile_size = TILE_SIZE_BYTE; /* 2048 */
    }

    if (!psav->bTiled) {
        left = x - x % 64;
        top = y;
        address = (top * psav->lDelta) + left * (pScrn->bitsPerPixel >> 3);
        address = (address >> 5) << 5;
    } else {
        top = y - y % tile_height;
        if (pScrn->bitsPerPixel == 16) {
            left = x - x % TILEWIDTH_16BPP;
            address = top * psav->lDelta + left * tile_size / TILEWIDTH_16BPP;
        } else if (pScrn->bitsPerPixel == 32) {
            left = x - x % TILEWIDTH_32BPP;
            address = top * psav->lDelta + left * tile_size / TILEWIDTH_32BPP;
        }
    }
    
    address += pScrn->fbOffset;

    if (psav->Chipset == S3_SAVAGE_MX) {
	if (!crtc2) {
            OUTREG32(PRI_STREAM_FBUF_ADDR0, address & 0xFFFFFFFC);
            OUTREG32(PRI_STREAM_FBUF_ADDR1, address & 0xFFFFFFFC);/* IGA1 */
        } else { 
            OUTREG32(PRI_STREAM2_FBUF_ADDR0, address & 0xFFFFFFFC);/* IGA2 */
            OUTREG32(PRI_STREAM2_FBUF_ADDR1, address & 0xFFFFFFFC);
	}
    } else if (psav->Chipset == S3_SUPERSAVAGE) {
	if (!crtc2) {
            /* IGA1 */
            OUTREG32(PRI_STREAM_FBUF_ADDR0, 0x80000000);
            OUTREG32(PRI_STREAM_FBUF_ADDR1, address & 0xFFFFFFF8);
        } else {
            /* IGA2 */
            OUTREG32(PRI_STREAM2_FBUF_ADDR0, ((address & 0xFFFFFFF8) | 0x80000000));
            OUTREG32(PRI_STREAM2_FBUF_ADDR1, address & 0xFFFFFFF8);
	}
    } else if (psav->Chipset == S3_SAVAGE2000) {
        /*  certain Y values seems to cause havoc, not sure why */
        OUTREG32(PRI_STREAM_FBUF_ADDR0, (address & 0xFFFFFFF8));
        OUTREG32(PRI_STREAM2_FBUF_ADDR0, (address & 0xFFFFFFF8));
    } else {
        OUTREG32(PRI_STREAM_FBUF_ADDR0,address |  0xFFFFFFFC);
        OUTREG32(PRI_STREAM_FBUF_ADDR1,address |  0x80000000);
    }
   
    return;
}

Bool SavageSwitchMode(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    SavagePtr psav = SAVPTR(pScrn);
    Bool success;

    TRACE(("SavageSwitchMode\n"));

    if (psav->FBStart2nd || (psav->videoFlags & VF_STREAMS_ON))
        SavageStreamsOff(pScrn);

    success = SavageModeInit(pScrn, mode);

    /* switching mode on primary will reset secondary.  it needs to be reset as well*/
    if (psav->IsPrimary) {
        DevUnion* pPriv;
        SavageEntPtr pSavEnt;
        pPriv = xf86GetEntityPrivate(pScrn->entityList[0], 
              gSavageEntityIndex);
        pSavEnt = pPriv->ptr;
        SavageModeInit(pSavEnt->pSecondaryScrn, pSavEnt->pSecondaryScrn->currentMode);
    }
    SavagePanningCheck(pScrn, mode);

    return success;
}


void SavageEnableMMIO(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    SavagePtr psav = SAVPTR(pScrn);
    int vgaCRIndex, vgaCRReg;
    unsigned char val;

    TRACE(("SavageEnableMMIO\n"));

    vgaHWSetStdFuncs(hwp);
    vgaHWSetMmioFuncs(hwp, psav->MapBase, 0x8000);
    val = VGAIN8(0x3c3);
    VGAOUT8(0x3c3, val | 0x01);
    val = VGAIN8(VGA_MISC_OUT_R);
    VGAOUT8(VGA_MISC_OUT_W, val | 0x01);
    vgaCRIndex = psav->vgaIOBase + 4;
    vgaCRReg = psav->vgaIOBase + 5;

    if( psav->Chipset >= S3_SAVAGE4 )
    {
	VGAOUT8(vgaCRIndex, 0x40);
	val = VGAIN8(vgaCRReg);
	VGAOUT8(vgaCRReg, val | 1);
    }

    return;
}


void SavageDisableMMIO(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    SavagePtr psav = SAVPTR(pScrn);
    int vgaCRIndex, vgaCRReg;
    unsigned char val;

    TRACE(("SavageDisableMMIO\n"));

    vgaCRIndex = psav->vgaIOBase + 4;
    vgaCRReg = psav->vgaIOBase + 5;

    if( psav->Chipset >= S3_SAVAGE4 )
    {
	VGAOUT8(vgaCRIndex, 0x40);
	val = VGAIN8(vgaCRReg);
	VGAOUT8(vgaCRReg, val | 1);
    }

    vgaHWSetStdFuncs(hwp);

    return;
}

void SavageLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indicies,
		       LOCO *colors, VisualPtr pVisual)
{
    SavagePtr psav = SAVPTR(pScrn);
    int i, index;
    int updateKey = -1;
    unsigned char byte = 0;

    /* choose CLUT */
    if (psav->IsPrimary) {
	/* enable CLUT 1 */
        VGAOUT8(0x3c4, 0x21);
        byte = VGAIN8(0x3c5);
        VGAOUT8(0x3c5, (byte & ~0x01));
	/* select CLUT 1 */
        VGAOUT8(0x3c4, 0x47);
        byte = VGAIN8(0x3c5);
        VGAOUT8(0x3c5, (byte & ~0x03) | 0x01); /* CLUT 1 */
    } else if (psav->IsSecondary) {
	/* enable CLUT 2 */
        VGAOUT8(0x3c4, 0x21);
        byte = VGAIN8(0x3c5);
        VGAOUT8(0x3c5, (byte & ~0x10));
	/* select CLUT 2 */
        VGAOUT8(0x3c4, 0x47);
        byte = VGAIN8(0x3c5);
        VGAOUT8(0x3c5, (byte & ~0x03) | 0x02); /* CLUT 2 */
    }
    
    for (i=0; i<numColors; i++) {
	index = indicies[i];
	if (index == pScrn->colorKey) updateKey = index;
	VGAOUT8(0x3c8, index);
	VGAOUT8(0x3c9, colors[index].red);
	VGAOUT8(0x3c9, colors[index].green);
	VGAOUT8(0x3c9, colors[index].blue);
    }

    /* restore saved CLUT index value */
    if (psav->IsPrimary || psav->IsSecondary) {
        VGAOUT8(0x3c4, 0x47);
        VGAOUT8(0x3c5, byte);
    }

    if (updateKey != -1)
	SavageUpdateKey(pScrn, colors[updateKey].red, colors[updateKey].green,
			colors[updateKey].blue);
}

#define Shift(v,d)  ((d) < 0 ? ((v) >> (-d)) : ((v) << (d)))

static void
SavageUpdateKey(ScrnInfoPtr pScrn, int r, int g, int b)
{
    ScreenPtr pScreen;
    SavagePtr psav = SAVPTR(pScrn);
    FbOverlayScrPrivPtr pScrOvlPriv;
    CARD32 key;
    int ul = 0, ol = 1;

    if (pScrn->depth != 8) {
	ul = 1;
	ol = 0;
    }
    if (!(pScreen = pScrn->pScreen) 
	|| !psav->FBStart2nd
	|| !(pScrOvlPriv = fbOverlayGetScrPriv(pScreen))) 
	return;
    key = ((Shift(r,psav->overlay.redShift) & psav->overlay.redMask)
	   | (Shift(g,psav->overlay.greenShift) & psav->overlay.greenMask)
	   | (Shift(b,psav->overlay.blueShift) & psav->overlay.blueMask));
    if (pScrOvlPriv->layer[ol].key != key) {
	pScrOvlPriv->layer[ol].key = key;
	(*pScrOvlPriv->PaintKey) (&pScrOvlPriv->layer[ol].u.run.pixmap->drawable,
				  &pScrOvlPriv->layer[ul].u.run.region,
				  pScrOvlPriv->layer[ol].key, ol);
    }
}

#if 0
#define inStatus1() (hwp->readST01( hwp ))
#endif

void SavageLoadPaletteSavage4(ScrnInfoPtr pScrn, int numColors, int *indicies,
		       LOCO *colors, VisualPtr pVisual)
{
    SavagePtr psav = SAVPTR(pScrn);
    int i, index;
    int updateKey = -1;
    
    VerticalRetraceWait();

    for (i=0; i<numColors; i++) {
          if (!(inStatus1() & 0x08))
  	    VerticalRetraceWait(); 
	index = indicies[i];
	VGAOUT8(0x3c8, index);
	VGAOUT8(0x3c9, colors[index].red);
	VGAOUT8(0x3c9, colors[index].green);
	VGAOUT8(0x3c9, colors[index].blue);
	if (index == pScrn->colorKey) updateKey = index;
    }
    if (updateKey != -1)
	SavageUpdateKey(pScrn, colors[updateKey].red, colors[updateKey].green,
			colors[updateKey].blue);
}

static void SavageCalcClock(long freq, int min_m, int min_n1, int max_n1,
			   int min_n2, int max_n2, long freq_min,
			   long freq_max, unsigned int *mdiv,
			   unsigned int *ndiv, unsigned int *r)
{
    double ffreq, ffreq_min, ffreq_max;
    double div, diff, best_diff;
    unsigned int m;
    unsigned char n1, n2, best_n1=16+2, best_n2=2, best_m=125+2;

    ffreq = freq / 1000.0 / BASE_FREQ;
    ffreq_max = freq_max / 1000.0 / BASE_FREQ;
    ffreq_min = freq_min / 1000.0 / BASE_FREQ;

    if (ffreq < ffreq_min / (1 << max_n2)) {
	    ErrorF("invalid frequency %1.3f Mhz\n",
		   ffreq*BASE_FREQ);
	    ffreq = ffreq_min / (1 << max_n2);
    }
    if (ffreq > ffreq_max / (1 << min_n2)) {
	    ErrorF("invalid frequency %1.3f Mhz\n",
		   ffreq*BASE_FREQ);
	    ffreq = ffreq_max / (1 << min_n2);
    }

    /* work out suitable timings */

    best_diff = ffreq;

    for (n2=min_n2; n2<=max_n2; n2++) {
	for (n1=min_n1+2; n1<=max_n1+2; n1++) {
	    m = (int)(ffreq * n1 * (1 << n2) + 0.5);
	    if (m < min_m+2 || m > 127+2)
		continue;
	    div = (double)(m) / (double)(n1);
	    if ((div >= ffreq_min) &&
		(div <= ffreq_max)) {
		diff = ffreq - div / (1 << n2);
		if (diff < 0.0)
			diff = -diff;
		if (diff < best_diff) {
		    best_diff = diff;
		    best_m = m;
		    best_n1 = n1;
		    best_n2 = n2;
		}
	    }
	}
    }

    *ndiv = best_n1 - 2;
    *r = best_n2;
    *mdiv = best_m - 2;
}


void SavageGEReset(ScrnInfoPtr pScrn, int from_timeout, int line, char *file)
{
    unsigned char cr66;
    int r, success = 0;
    CARD32 fifo_control = 0, miu_control = 0;
    CARD32 streams_timeout = 0, misc_timeout = 0;
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    SavagePtr psav = SAVPTR(pScrn);
    int vgaCRIndex, vgaCRReg, vgaIOBase;

    TRACE(("SavageGEReset(%d,%s)\n", line, file));

    vgaIOBase = hwp->IOBase;
    vgaCRIndex = vgaIOBase + 4;
    vgaCRReg = vgaIOBase + 5;

    if (from_timeout) {
	if (psav->GEResetCnt++ < 10 || xf86GetVerbosity() > 1)
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "SavageGEReset called from %s line %d\n", file, line);
    } else
	psav->WaitIdleEmpty(psav);

    if (from_timeout && !S3_SAVAGE_MOBILE_SERIES(psav->Chipset) ) {
	fifo_control = INREG(FIFO_CONTROL_REG);
	miu_control = INREG(MIU_CONTROL_REG);
	streams_timeout = INREG(STREAMS_TIMEOUT_REG);
	misc_timeout = INREG(MISC_TIMEOUT_REG);
    }

    VGAOUT8(vgaCRIndex, 0x66);
    cr66 = VGAIN8(vgaCRReg);

    usleep(10000);
    for (r=1; r<10; r++) {
	VGAOUT8(vgaCRReg, cr66 | 0x02);
	usleep(10000);
	VGAOUT8(vgaCRReg, cr66 & ~0x02);
	usleep(10000);

	if (!from_timeout)
	    psav->WaitIdleEmpty(psav);
	OUTREG(DEST_SRC_STR, psav->Bpl << 16 | psav->Bpl);

	usleep(10000);
	switch(psav->Chipset) {
	    case S3_SAVAGE3D:
	    case S3_SAVAGE_MX:
	      success = (STATUS_WORD0 & 0x0008ffff) == 0x00080000;
	      break;
	    case S3_SAVAGE4:
	    case S3_PROSAVAGE:
	    case S3_PROSAVAGEDDR:
	    case S3_TWISTER:
	    case S3_SUPERSAVAGE:
	      success = (ALT_STATUS_WORD0 & 0x0081ffff) == 0x00800000;
	      break;
	    case S3_SAVAGE2000:
	      success = (ALT_STATUS_WORD0 & 0x008fffff) == 0;
	      break;
	}	
	if(!success) {
	    usleep(10000);
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"restarting S3 graphics engine reset %2d ...\n", r);
	}
	else
	    break;
    }

    /* At this point, the FIFO is empty and the engine is idle. */

    if (from_timeout && !S3_SAVAGE_MOBILE_SERIES(psav->Chipset) ) {
	OUTREG(FIFO_CONTROL_REG, fifo_control);
	OUTREG(MIU_CONTROL_REG, miu_control);
	OUTREG(STREAMS_TIMEOUT_REG, streams_timeout);
	OUTREG(MISC_TIMEOUT_REG, misc_timeout);
    }

    OUTREG(SRC_BASE, 0);
    OUTREG(DEST_BASE, 0);
    OUTREG(CLIP_L_R, ((0) << 16) | pScrn->displayWidth);
    OUTREG(CLIP_T_B, ((0) << 16) | psav->ScissB);
    OUTREG(MONO_PAT_0, ~0);
    OUTREG(MONO_PAT_1, ~0);

    SavageSetGBD(pScrn);

}



/* This function is used to debug, it prints out the contents of s3 regs */

void
SavagePrintRegs(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    unsigned char i;
    int vgaCRIndex = 0x3d4;
    int vgaCRReg = 0x3d5;

    ErrorF( "SR    x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF" );

    for( i = 0; i < 0x70; i++ ) {
	if( !(i % 16) )
	    ErrorF( "\nSR%xx ", i >> 4 );
	VGAOUT8( 0x3c4, i );
	ErrorF( " %02x", VGAIN8(0x3c5) );
    }

    ErrorF( "\n\nCR    x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF" );

    for( i = 0; i < 0xB7; i++ ) {
	if( !(i % 16) )
	    ErrorF( "\nCR%xx ", i >> 4 );
	VGAOUT8( vgaCRIndex, i );
	ErrorF( " %02x", VGAIN8(vgaCRReg) );
    }

    ErrorF("\n\n");
}

static void SavageDPMS(ScrnInfoPtr pScrn, int mode, int flags)
{
    SavagePtr psav = SAVPTR(pScrn);
    unsigned char sr8 = 0x00, srd = 0x00;

    TRACE(("SavageDPMS(%d,%x)\n", mode, flags));

    if (psav->DisplayType == MT_CRT) {
    	VGAOUT8(0x3c4, 0x08);
    	sr8 = VGAIN8(0x3c5);
    	sr8 |= 0x06;
    	VGAOUT8(0x3c5, sr8);

    	VGAOUT8(0x3c4, 0x0d);
    	srd = VGAIN8(0x3c5);

    	srd &= 0x03;

    	switch (mode) {
	    case DPMSModeOn:
	    	break;
	    case DPMSModeStandby:
	    	srd |= 0x10;
	    	break;
	    case DPMSModeSuspend:
	    	srd |= 0x40;
	    	break;
	    case DPMSModeOff:
	    	srd |= 0x50;
	    	break;
	    default:
	    	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Invalid DPMS mode %d\n", mode);
	    	break;
    	}

    	VGAOUT8(0x3c4, 0x0d);
    	VGAOUT8(0x3c5, srd);
    }

    if (psav->DisplayType == MT_LCD || psav->DisplayType == MT_DFP) {
	if (S3_MOBILE_TWISTER_SERIES(psav->Chipset) && psav->UseBIOS) {
	    SavageSetPanelEnabled(psav, (mode == DPMSModeOn));
	} else {
    	    switch (mode) {
	        case DPMSModeOn:
		    VGAOUT8(0x3c4, 0x31); /* SR31 bit 4 - FP enable */
		    VGAOUT8(0x3c5, VGAIN8(0x3c5) | 0x10);
	            break;
	        case DPMSModeStandby:
	        case DPMSModeSuspend:
	        case DPMSModeOff:
		    VGAOUT8(0x3c4, 0x31); /* SR31 bit 4 - FP enable */
		    VGAOUT8(0x3c5, VGAIN8(0x3c5) & ~0x10);
	            break;
	        default:
	            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Invalid DPMS mode %d\n", mode);
	            break;
	    }
        }
    }

    return;
}

static void
SavageProbeDDC(ScrnInfoPtr pScrn, int index)
{
    vbeInfoPtr pVbe;
    
    if (xf86LoadSubModule(pScrn, "vbe")) {
	pVbe = VBEInit(NULL, index);
	ConfiguredMonitor = vbeDoEDID(pVbe, NULL);
	vbeFree(pVbe);
    }
}

static unsigned int
SavageDDC1Read(ScrnInfoPtr pScrn)
{
    register unsigned char tmp;
    SavagePtr psav = SAVPTR(pScrn);

    UnLockExtRegs();
    
    VerticalRetraceWait();
    
    InI2CREG(tmp,psav->I2CPort);
    
    return ((unsigned int) (tmp & 0x08));
}

static void
SavageDDC1SetSpeed(ScrnInfoPtr pScrn, xf86ddcSpeed speed)
{
    vgaHWddc1SetSpeed(pScrn, speed);
}

static Bool
SavageDDC1(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    unsigned char byte;
    xf86MonPtr pMon;

    UnLockExtRegs();
    
    /* initialize chipset */
    InI2CREG(byte,psav->I2CPort);
    OutI2CREG(byte | 0x12,psav->I2CPort);

    pMon = xf86DoEDID_DDC1(XF86_SCRN_ARG(pScrn), SavageDDC1SetSpeed,
			   SavageDDC1Read);
    if (!pMon)
        return FALSE;
    
    xf86PrintEDID(pMon);
    
    if (!psav->IgnoreEDID)
        xf86SetDDCproperties(pScrn,pMon);

    /* undo initialization */
    OutI2CREG(byte,psav->I2CPort);

    return TRUE;
}

static void
SavageGetTvMaxSize(SavagePtr psav)
{
    if( psav->PAL ) {
	psav->TVSizeX = 800;
	psav->TVSizeY = 600;
    }
    else {
	psav->TVSizeX = 640;
	psav->TVSizeY = 480;
    }
}


static Bool
SavagePanningCheck(ScrnInfoPtr pScrn, DisplayModePtr pMode)
{
    SavagePtr psav = SAVPTR(pScrn);
    psav->iResX = pMode->CrtcHDisplay;
    psav->iResY = pMode->CrtcVDisplay;

    if ((psav->iResX < psav->PanelX || psav->iResY < psav->PanelY))
        psav->FPExpansion = TRUE;
    else
        psav->FPExpansion = FALSE;

    if( psav->iResX < pScrn->virtualX || psav->iResY < pScrn->virtualY )
	return TRUE;
    else
	return FALSE;
}

static void
SavageResetStreams(ScrnInfoPtr pScrn)
{
    SavagePtr psav = SAVPTR(pScrn);
    unsigned char cr67;
    unsigned char cr69;

    /* disable streams */
    switch (psav->Chipset) {
        case S3_SAVAGE_MX:
        case S3_SUPERSAVAGE:
            OUTREG32(PRI_STREAM_STRIDE,0);
            OUTREG32(PRI_STREAM2_STRIDE, 0);
            OUTREG32(PRI_STREAM_FBUF_ADDR0,0x00000000);
            OUTREG32(PRI_STREAM_FBUF_ADDR1,0x00000000);
            OUTREG32(PRI_STREAM2_FBUF_ADDR0,0x00000000);
            OUTREG32(PRI_STREAM2_FBUF_ADDR1,0x00000000);
	    OUTREG8(CRT_ADDRESS_REG, 0x67);
            cr67 = INREG8(CRT_DATA_REG);
	    cr67 &= ~0x08; /* CR67[3] = 1 : Mem-mapped regs */
	    cr67 &= ~0x04; /* CR67[2] = 1 : enable stream 1 */
	    cr67 &= ~0x02; /* CR67[1] = 1 : enable stream 2 */
            OUTREG8(CRT_DATA_REG, cr67);
            break;
	case S3_SAVAGE3D:
        case S3_SAVAGE4:
        case S3_TWISTER:
        case S3_PROSAVAGE:            
        case S3_PROSAVAGEDDR:
            OUTREG32(PRI_STREAM_STRIDE,0);
            OUTREG32(PRI_STREAM_FBUF_ADDR0,0);
            OUTREG32(PRI_STREAM_FBUF_ADDR1,0);
	    OUTREG8(CRT_ADDRESS_REG, 0x67);
            cr67 = INREG8(CRT_DATA_REG);
	    cr67 &= ~0x0c; /* CR67[2] = 1 : enable stream 1 */
            OUTREG8(CRT_DATA_REG, cr67);
	    OUTREG8(CRT_ADDRESS_REG, 0x69);
            cr69 = INREG8(CRT_DATA_REG);
	    cr69 &= ~0x80; /* CR69[0] = 1 : Mem-mapped regs */
            OUTREG8(CRT_DATA_REG, cr69);
            break;
        case S3_SAVAGE2000:
            OUTREG32(PRI_STREAM_STRIDE,0);
            OUTREG32(PRI_STREAM_FBUF_ADDR0,0x00000000);
            OUTREG32(PRI_STREAM_FBUF_ADDR1,0x00000000);
	    OUTREG8(CRT_ADDRESS_REG, 0x67);
            cr67 = INREG8(CRT_DATA_REG);
	    cr67 &= ~0x08; /* CR67[3] = 1 : Mem-mapped regs */
	    cr67 &= ~0x04; /* CR67[2] = 1 : enable stream 1 */
	    cr67 &= ~0x02; /* CR67[1] = 1 : enable stream 2 */
            OUTREG8(CRT_DATA_REG, cr67);
            break;
        default:
            break;
    }

}
