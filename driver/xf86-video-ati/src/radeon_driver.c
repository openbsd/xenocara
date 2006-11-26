/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/ati/radeon_driver.c,v 1.117 2004/02/19 22:38:12 tsi Exp $ */
/* $XdotOrg: driver/xf86-video-ati/src/radeon_driver.c,v 1.116 2006/04/29 21:30:23 daenzer Exp $ */
/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *   Alan Hourihane <alanh@fairlite.demon.co.uk>
 *
 * Credits:
 *
 *   Thanks to Ani Joshi <ajoshi@shell.unixbox.com> for providing source
 *   code to his Radeon driver.  Portions of this file are based on the
 *   initialization code for that driver.
 *
 * References:
 *
 * !!!! FIXME !!!!
 *   RAGE 128 VR/ RAGE 128 GL Register Reference Manual (Technical
 *   Reference Manual P/N RRG-G04100-C Rev. 0.04), ATI Technologies: April
 *   1999.
 *
 *   RAGE 128 Software Development Manual (Technical Reference Manual P/N
 *   SDK-G04000 Rev. 0.01), ATI Technologies: June 1999.
 *
 * This server does not yet support these XFree86 4.0 features:
 * !!!! FIXME !!!!
 *   DDC1 & DDC2
 *   shadowfb (Note: dri uses shadowfb for another purpose in radeon_dri.c)
 *   overlay planes
 *
 * Modified by Marc Aurele La France (tsi@xfree86.org) for ATI driver merge.
 *
 * Mergedfb and pseudo xinerama support added by Alex Deucher (agd5f@yahoo.com)
 * based on the sis driver by Thomas Winischhofer.
 *
 */

#include <string.h>
#include <stdio.h>

				/* Driver data structures */
#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_probe.h"
#include "radeon_version.h"
#include "radeon_mergedfb.h"

#ifdef XF86DRI
#define _XF86DRI_SERVER_
#include "radeon_dri.h"
#include "radeon_sarea.h"
#include "sarea.h"
#endif

#include "fb.h"

				/* colormap initialization */
#include "micmap.h"
#include "dixstruct.h"

				/* X and server generic header files */
#include "xf86.h"
#include "xf86_ansic.h"		/* For xf86getsecs() */
#include "xf86_OSproc.h"
#include "xf86RAC.h"
#include "xf86Resources.h"
#include "xf86cmap.h"
#include "vbe.h"

				/* fbdevhw * vgaHW definitions */
#ifdef WITH_VGAHW
#include "vgaHW.h"
#endif
#include "fbdevhw.h"

#define DPMS_SERVER
#include <X11/extensions/dpms.h>

#include "atipciids.h"
#include "radeon_chipset.h"

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a)>(b)?(b):(a))
#endif

				/* Forward definitions for driver functions */
static Bool RADEONCloseScreen(int scrnIndex, ScreenPtr pScreen);
static Bool RADEONSaveScreen(ScreenPtr pScreen, int mode);
static void RADEONSave(ScrnInfoPtr pScrn);
static void RADEONRestore(ScrnInfoPtr pScrn);
static Bool RADEONModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
static void RADEONDisplayPowerManagementSet(ScrnInfoPtr pScrn,
					    int PowerManagementMode,
					    int flags);
static void RADEONInitDispBandwidth(ScrnInfoPtr pScrn);

static void RADEONGetMergedFBOptions(ScrnInfoPtr pScrn);
static int RADEONValidateMergeModes(ScrnInfoPtr pScrn);
static void RADEONSetDynamicClock(ScrnInfoPtr pScrn, int mode);
static void RADEONForceSomeClocks(ScrnInfoPtr pScrn);
static void RADEONUpdatePanelSize(ScrnInfoPtr pScrn);
static void RADEONSaveMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save);

#ifdef XF86DRI
static void RADEONAdjustMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save);
#endif

/* psuedo xinerama support */

extern Bool 		RADEONnoPanoramiXExtension;

static const OptionInfoRec RADEONOptions[] = {
    { OPTION_NOACCEL,        "NoAccel",          OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_SW_CURSOR,      "SWcursor",         OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DAC_6BIT,       "Dac6Bit",          OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DAC_8BIT,       "Dac8Bit",          OPTV_BOOLEAN, {0}, TRUE  },
#ifdef XF86DRI
    { OPTION_BUS_TYPE,       "BusType",          OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_CP_PIO,         "CPPIOMode",        OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_USEC_TIMEOUT,   "CPusecTimeout",    OPTV_INTEGER, {0}, FALSE },
    { OPTION_AGP_MODE,       "AGPMode",          OPTV_INTEGER, {0}, FALSE },
    { OPTION_AGP_FW,         "AGPFastWrite",     OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_GART_SIZE_OLD,  "AGPSize",          OPTV_INTEGER, {0}, FALSE },
    { OPTION_GART_SIZE,      "GARTSize",         OPTV_INTEGER, {0}, FALSE },
    { OPTION_RING_SIZE,      "RingSize",         OPTV_INTEGER, {0}, FALSE },
    { OPTION_BUFFER_SIZE,    "BufferSize",       OPTV_INTEGER, {0}, FALSE },
    { OPTION_DEPTH_MOVE,     "EnableDepthMoves", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_PAGE_FLIP,      "EnablePageFlip",   OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_NO_BACKBUFFER,  "NoBackBuffer",     OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_XV_DMA,         "DMAForXv",         OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_FBTEX_PERCENT,  "FBTexPercent",     OPTV_INTEGER, {0}, FALSE },
    { OPTION_DEPTH_BITS,     "DepthBits",        OPTV_INTEGER, {0}, FALSE },
#ifdef USE_EXA
    { OPTION_ACCEL_DFS,      "AccelDFS",         OPTV_BOOLEAN, {0}, FALSE },
#endif
#endif
    { OPTION_PANEL_OFF,      "PanelOff",         OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DDC_MODE,       "DDCMode",          OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_MONITOR_LAYOUT, "MonitorLayout",    OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_IGNORE_EDID,    "IgnoreEDID",       OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_FBDEV,          "UseFBDev",         OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_MERGEDFB,	     "MergedFB",      	 OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_CRT2HSYNC,	     "CRT2HSync",        OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_CRT2VREFRESH,   "CRT2VRefresh",     OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_CRT2POS,        "CRT2Position",	 OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_METAMODES,      "MetaModes",        OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_MERGEDDPI,	     "MergedDPI", 	 OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_RADEONXINERAMA, "MergedXinerama", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_CRT2ISSCRN0,    "MergedXineramaCRT2IsScreen0", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_MERGEDFBNONRECT, "MergedNonRectangular", OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_MERGEDFBMOUSER,  "MergedMouseRestriction", OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_DISP_PRIORITY,  "DisplayPriority",  OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_PANEL_SIZE,     "PanelSize",        OPTV_ANYSTR,  {0}, FALSE },
    { OPTION_MIN_DOTCLOCK,   "ForceMinDotClock", OPTV_FREQ,    {0}, FALSE },
    { OPTION_COLOR_TILING,   "ColorTiling",      OPTV_BOOLEAN, {0}, FALSE },
#ifdef XvExtension
    { OPTION_VIDEO_KEY,                   "VideoKey",                 OPTV_INTEGER, {0}, FALSE },
    { OPTION_RAGE_THEATRE_CRYSTAL,        "RageTheatreCrystal",       OPTV_INTEGER, {0}, FALSE },
    { OPTION_RAGE_THEATRE_TUNER_PORT,     "RageTheatreTunerPort",     OPTV_INTEGER, {0}, FALSE },
    { OPTION_RAGE_THEATRE_COMPOSITE_PORT, "RageTheatreCompositePort", OPTV_INTEGER, {0}, FALSE },
    { OPTION_RAGE_THEATRE_SVIDEO_PORT,    "RageTheatreSVideoPort",    OPTV_INTEGER, {0}, FALSE },
    { OPTION_TUNER_TYPE,                  "TunerType",                OPTV_INTEGER, {0}, FALSE },
	{ OPTION_RAGE_THEATRE_MICROC_PATH,	"RageTheatreMicrocPath",	 OPTV_STRING, {0}, FALSE },
	{ OPTION_RAGE_THEATRE_MICROC_TYPE, 	"RageTheatreMicrocType",	 OPTV_STRING, {0}, FALSE },
#endif
#ifdef RENDER
    { OPTION_RENDER_ACCEL,   "RenderAccel",      OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_SUBPIXEL_ORDER, "SubPixelOrder",    OPTV_ANYSTR,  {0}, FALSE },
#endif
    { OPTION_SHOWCACHE,      "ShowCache",        OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DYNAMIC_CLOCKS, "DynamicClocks",    OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_BIOS_HOTKEYS,   "BIOSHotkeys",      OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_VGA_ACCESS,     "VGAAccess",        OPTV_BOOLEAN, {0}, TRUE  },
    { OPTION_REVERSE_DDC,    "ReverseDDC",       OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_LVDS_PROBE_PLL, "LVDSProbePLL",     OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_ACCELMETHOD,    "AccelMethod",      OPTV_STRING,  {0}, FALSE },
    { OPTION_CONSTANTDPI,    "ConstantDPI",	 OPTV_BOOLEAN,	{0}, FALSE },
    { -1,                    NULL,               OPTV_NONE,    {0}, FALSE }
};

_X_EXPORT const OptionInfoRec *RADEONOptionsWeak(void) { return RADEONOptions; }

#ifdef WITH_VGAHW
static const char *vgahwSymbols[] = {
    "vgaHWFreeHWRec",
    "vgaHWGetHWRec",
    "vgaHWGetIndex",
    "vgaHWLock",
    "vgaHWRestore",
    "vgaHWSave",
    "vgaHWUnlock",
    "vgaHWGetIOBase",
    NULL
};
#endif

static const char *fbdevHWSymbols[] = {
    "fbdevHWInit",
    "fbdevHWUseBuildinMode",

    "fbdevHWGetVidmem",

    "fbdevHWDPMSSet",

    /* colormap */
    "fbdevHWLoadPalette",
    /* ScrnInfo hooks */
    "fbdevHWAdjustFrame",
    "fbdevHWEnterVT",
    "fbdevHWLeaveVT",
    "fbdevHWModeInit",
    "fbdevHWRestore",
    "fbdevHWSave",
    "fbdevHWSwitchMode",
    "fbdevHWValidModeWeak",

    "fbdevHWMapMMIO",
    "fbdevHWMapVidmem",
    "fbdevHWUnmapMMIO",
    "fbdevHWUnmapVidmem",

    NULL
};

static const char *ddcSymbols[] = {
    "xf86PrintEDID",
    "xf86DoEDID_DDC1",
    "xf86DoEDID_DDC2",
    NULL
};

static const char *fbSymbols[] = {
    "fbScreenInit",
    "fbPictureInit",
    NULL
};


#ifdef USE_EXA
static const char *exaSymbols[] = {
    "exaDriverAlloc",
    "exaDriverInit",
    "exaDriverFini",
    "exaOffscreenAlloc",
    "exaOffscreenFree",
    "exaGetPixmapOffset",
    "exaGetPixmapPitch",
    "exaGetPixmapSize",
    "exaMarkSync",
    "exaWaitSync",
    NULL
};
#endif /* USE_EXA */

#ifdef USE_XAA
static const char *xaaSymbols[] = {
    "XAACreateInfoRec",
    "XAADestroyInfoRec",
    "XAAInit",
    NULL
};
#endif /* USE_XAA */

#if 0
static const char *xf8_32bppSymbols[] = {
    "xf86Overlay8Plus32Init",
    NULL
};
#endif

static const char *ramdacSymbols[] = {
    "xf86CreateCursorInfoRec",
    "xf86DestroyCursorInfoRec",
    "xf86ForceHWCursor",
    "xf86InitCursor",
    NULL
};

#ifdef XF86DRI
static const char *drmSymbols[] = {
    "drmGetInterruptFromBusID",
    "drmCtlInstHandler",
    "drmCtlUninstHandler",
    "drmAddBufs",
    "drmAddMap",
    "drmAgpAcquire",
    "drmAgpAlloc",
    "drmAgpBase",
    "drmAgpBind",
    "drmAgpDeviceId",
    "drmAgpEnable",
    "drmAgpFree",
    "drmAgpGetMode",
    "drmAgpRelease",
    "drmAgpUnbind",
    "drmAgpVendorId",
    "drmCommandNone",
    "drmCommandRead",
    "drmCommandWrite",
    "drmCommandWriteRead",
    "drmDMA",
    "drmFreeVersion",
    "drmGetLibVersion",
    "drmGetVersion",
    "drmMap",
    "drmMapBufs",
    "drmRadeonCleanupCP",
    "drmRadeonClear",
    "drmRadeonFlushIndirectBuffer",
    "drmRadeonInitCP",
    "drmRadeonResetCP",
    "drmRadeonStartCP",
    "drmRadeonStopCP",
    "drmRadeonWaitForIdleCP",
    "drmScatterGatherAlloc",
    "drmScatterGatherFree",
    "drmUnmap",
    "drmUnmapBufs",
    NULL
};

static const char *driSymbols[] = {
    "DRICloseScreen",
    "DRICreateInfoRec",
    "DRIDestroyInfoRec",
    "DRIFinishScreenInit",
    "DRIGetContext",
    "DRIGetDeviceInfo",
    "DRIGetSAREAPrivate",
    "DRILock",
    "DRIQueryVersion",
    "DRIScreenInit",
    "DRIUnlock",
    "GlxSetVisualConfigs",
    "DRICreatePCIBusID",
    NULL
};

static const char *driShadowFBSymbols[] = {
    "ShadowFBInit",
    NULL
};
#endif

static const char *vbeSymbols[] = {
    "VBEInit",
    "vbeDoEDID",
    NULL
};

static const char *int10Symbols[] = {
    "xf86InitInt10",
    "xf86FreeInt10",
    "xf86int10Addr",
    "xf86ExecX86int10",
    NULL
};

static const char *i2cSymbols[] = {
    "xf86CreateI2CBusRec",
    "xf86I2CBusInit",
    NULL
};

void RADEONLoaderRefSymLists(void)
{
    /*
     * Tell the loader about symbols from other modules that this module might
     * refer to.
     */
    xf86LoaderRefSymLists(
#ifdef WITH_VGAHW
			  vgahwSymbols,
#endif
			  fbSymbols,
#ifdef USE_EXA
			  exaSymbols,
#endif
#ifdef USE_XAA
			  xaaSymbols,
#endif
#if 0
			  xf8_32bppSymbols,
#endif
			  ramdacSymbols,
#ifdef XF86DRI
			  drmSymbols,
			  driSymbols,
			  driShadowFBSymbols,
#endif
			  fbdevHWSymbols,
			  vbeSymbols,
			  int10Symbols,
			  i2cSymbols,
			  ddcSymbols,
			  NULL);
}

/* Established timings from EDID standard */
static struct
{
    int hsize;
    int vsize;
    int refresh;
} est_timings[] = {
    {1280, 1024, 75},
    {1024, 768, 75},
    {1024, 768, 70},
    {1024, 768, 60},
    {1024, 768, 87},
    {832, 624, 75},
    {800, 600, 75},
    {800, 600, 72},
    {800, 600, 60},
    {800, 600, 56},
    {640, 480, 75},
    {640, 480, 72},
    {640, 480, 67},
    {640, 480, 60},
    {720, 400, 88},
    {720, 400, 70},
};

static const RADEONTMDSPll default_tmds_pll[CHIP_FAMILY_LAST][4] =
{
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},				/*CHIP_FAMILY_UNKNOW*/
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},				/*CHIP_FAMILY_LEGACY*/
    {{12000, 0xa1b}, {0xffffffff, 0xa3f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RADEON*/
    {{12000, 0xa1b}, {0xffffffff, 0xa3f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RV100*/
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},				/*CHIP_FAMILY_RS100*/
    {{15000, 0xa1b}, {0xffffffff, 0xa3f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RV200*/
    {{12000, 0xa1b}, {0xffffffff, 0xa3f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RS200*/
    {{15000, 0xa1b}, {0xffffffff, 0xa3f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_R200*/
    {{15500, 0x81b}, {0xffffffff, 0x83f}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RV250*/
    {{0, 0}, {0, 0}, {0, 0}, {0, 0}},				/*CHIP_FAMILY_RS300*/
    {{13000, 0x400f4}, {15000, 0x400f7}, {0xffffffff, 0x40111}, {0, 0}}, /*CHIP_FAMILY_RV280*/
    {{0xffffffff, 0xb01cb}, {0, 0}, {0, 0}, {0, 0}},		/*CHIP_FAMILY_R300*/
    {{0xffffffff, 0xb01cb}, {0, 0}, {0, 0}, {0, 0}},		/*CHIP_FAMILY_R350*/
    {{15000, 0xb0155}, {0xffffffff, 0xb01cb}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RV350*/
    {{15000, 0xb0155}, {0xffffffff, 0xb01cb}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RV380*/
    {{0xffffffff, 0xb01cb}, {0, 0}, {0, 0}, {0, 0}},		/*CHIP_FAMILY_R420*/
    {{0xffffffff, 0xb01cb}, {0, 0}, {0, 0}, {0, 0}},		/*CHIP_FAMILY_RV410*/ /* FIXME: just values from r420 used... */
    {{15000, 0xb0155}, {0xffffffff, 0xb01cb}, {0, 0}, {0, 0}},	/*CHIP_FAMILY_RS400*/ /* FIXME: just values from rv380 used... */
};

#ifdef XFree86LOADER
static int getRADEONEntityIndex(void)
{
    int *radeon_entity_index = LoaderSymbol("gRADEONEntityIndex");
    if (!radeon_entity_index)
        return -1;
    else
        return *radeon_entity_index;
}
#else
extern int gRADEONEntityIndex;
static int getRADEONEntityIndex(void)
{
    return gRADEONEntityIndex;
}
#endif

struct RADEONInt10Save {
	CARD32 MEM_CNTL;
	CARD32 MEMSIZE;
	CARD32 MPP_TB_CONFIG;
};

static Bool RADEONMapMMIO(ScrnInfoPtr pScrn);
static Bool RADEONUnmapMMIO(ScrnInfoPtr pScrn);

RADEONEntPtr RADEONEntPriv(ScrnInfoPtr pScrn)
{
    DevUnion     *pPriv;
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    pPriv = xf86GetEntityPrivate(info->pEnt->index,
                                 getRADEONEntityIndex());
    return pPriv->ptr;
}

static void
RADEONPreInt10Save(ScrnInfoPtr pScrn, void **pPtr)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32 CardTmp;
    static struct RADEONInt10Save SaveStruct = { 0, 0, 0 };

    /* Save the values and zap MEM_CNTL */
    SaveStruct.MEM_CNTL = INREG(RADEON_MEM_CNTL);
    SaveStruct.MEMSIZE = INREG(RADEON_CONFIG_MEMSIZE);
    SaveStruct.MPP_TB_CONFIG = INREG(RADEON_MPP_TB_CONFIG);

    /*
     * Zap MEM_CNTL and set MPP_TB_CONFIG<31:24> to 4
     */
    OUTREG(RADEON_MEM_CNTL, 0);
    CardTmp = SaveStruct.MPP_TB_CONFIG & 0x00ffffffu;
    CardTmp |= 0x04 << 24;
    OUTREG(RADEON_MPP_TB_CONFIG, CardTmp);

    *pPtr = (void *)&SaveStruct;
}

static void
RADEONPostInt10Check(ScrnInfoPtr pScrn, void *ptr)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    struct RADEONInt10Save *pSave = ptr;
    CARD32 CardTmp;

    /* If we don't have a valid (non-zero) saved MEM_CNTL, get out now */
    if (!pSave || !pSave->MEM_CNTL)
	return;

    /*
     * If either MEM_CNTL is currently zero or inconistent (configured for
     * two channels with the two channels configured differently), restore
     * the saved registers.
     */
    CardTmp = INREG(RADEON_MEM_CNTL);
    if (!CardTmp ||
	((CardTmp & 1) &&
	 (((CardTmp >> 8) & 0xff) != ((CardTmp >> 24) & 0xff)))) {
	/* Restore the saved registers */
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Restoring MEM_CNTL (%08lx), setting to %08lx\n",
		   (unsigned long)CardTmp, (unsigned long)pSave->MEM_CNTL);
	OUTREG(RADEON_MEM_CNTL, pSave->MEM_CNTL);

	CardTmp = INREG(RADEON_CONFIG_MEMSIZE);
	if (CardTmp != pSave->MEMSIZE) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Restoring CONFIG_MEMSIZE (%08lx), setting to %08lx\n",
		       (unsigned long)CardTmp, (unsigned long)pSave->MEMSIZE);
	    OUTREG(RADEON_CONFIG_MEMSIZE, pSave->MEMSIZE);
	}
    }

    CardTmp = INREG(RADEON_MPP_TB_CONFIG);
    if ((CardTmp & 0xff000000u) != (pSave->MPP_TB_CONFIG & 0xff000000u)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	           "Restoring MPP_TB_CONFIG<31:24> (%02lx), setting to %02lx\n",
	 	   (unsigned long)CardTmp >> 24,
		   (unsigned long)pSave->MPP_TB_CONFIG >> 24);
	CardTmp &= 0x00ffffffu;
	CardTmp |= (pSave->MPP_TB_CONFIG & 0xff000000u);
	OUTREG(RADEON_MPP_TB_CONFIG, CardTmp);
    }
}

/* Allocate our private RADEONInfoRec */
static Bool RADEONGetRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate) return TRUE;

    pScrn->driverPrivate = xnfcalloc(sizeof(RADEONInfoRec), 1);
    return TRUE;
}

/* Free our private RADEONInfoRec */
static void RADEONFreeRec(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    if(info->CRT2HSync) xfree(info->CRT2HSync);
    info->CRT2HSync = NULL;
    if(info->CRT2VRefresh) xfree(info->CRT2VRefresh);
    info->CRT2VRefresh = NULL;
    if(info->MetaModes) xfree(info->MetaModes);
    info->MetaModes = NULL;
    if(info->CRT2pScrn) {
       if(info->CRT2pScrn->modes) {
          while(info->CRT2pScrn->modes)
             xf86DeleteMode(&info->CRT2pScrn->modes, info->CRT2pScrn->modes);
       }
       if(info->CRT2pScrn->monitor) {
          if(info->CRT2pScrn->monitor->Modes) {
	     while(info->CRT2pScrn->monitor->Modes)
	        xf86DeleteMode(&info->CRT2pScrn->monitor->Modes, info->CRT2pScrn->monitor->Modes);
	  }
	  if(info->CRT2pScrn->monitor->DDC) xfree(info->CRT2pScrn->monitor->DDC);
          xfree(info->CRT2pScrn->monitor);
       }
       xfree(info->CRT2pScrn);
       info->CRT2pScrn = NULL;
    }
    if(info->CRT1Modes) {
       if(info->CRT1Modes != pScrn->modes) {
          if(pScrn->modes) {
             pScrn->currentMode = pScrn->modes;
             do {
                DisplayModePtr p = pScrn->currentMode->next;
                if(pScrn->currentMode->Private)
                   xfree(pScrn->currentMode->Private);
                xfree(pScrn->currentMode);
                pScrn->currentMode = p;
             } while(pScrn->currentMode != pScrn->modes);
          }
          pScrn->currentMode = info->CRT1CurrentMode;
          pScrn->modes = info->CRT1Modes;
          info->CRT1CurrentMode = NULL;
          info->CRT1Modes = NULL;
       }
    }

    if (!pScrn || !pScrn->driverPrivate) return;
    xfree(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
}

/* Memory map the MMIO region.  Used during pre-init and by RADEONMapMem,
 * below
 */
static Bool RADEONMapMMIO(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (info->FBDev) {
	info->MMIO = fbdevHWMapMMIO(pScrn);
    } else {
	info->MMIO = xf86MapPciMem(pScrn->scrnIndex,
				   VIDMEM_MMIO | VIDMEM_READSIDEEFFECT,
				   info->PciTag,
				   info->MMIOAddr,
				   info->MMIOSize);
    }

    if (!info->MMIO) return FALSE;
    return TRUE;
}

/* Unmap the MMIO region.  Used during pre-init and by RADEONUnmapMem,
 * below
 */
static Bool RADEONUnmapMMIO(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (info->FBDev)
	fbdevHWUnmapMMIO(pScrn);
    else {
	xf86UnMapVidMem(pScrn->scrnIndex, info->MMIO, info->MMIOSize);
    }
    info->MMIO = NULL;
    return TRUE;
}

/* Memory map the frame buffer.  Used by RADEONMapMem, below. */
static Bool RADEONMapFB(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (info->FBDev) {
	info->FB = fbdevHWMapVidmem(pScrn);
    } else {
	RADEONTRACE(("Map: 0x%08lx, 0x%08lx\n", info->LinearAddr, info->FbMapSize));
	info->FB = xf86MapPciMem(pScrn->scrnIndex,
				 VIDMEM_FRAMEBUFFER,
				 info->PciTag,
				 info->LinearAddr,
				 info->FbMapSize);
    }

    if (!info->FB) return FALSE;
    return TRUE;
}

/* Unmap the frame buffer.  Used by RADEONUnmapMem, below. */
static Bool RADEONUnmapFB(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (info->FBDev)
	fbdevHWUnmapVidmem(pScrn);
    else
	xf86UnMapVidMem(pScrn->scrnIndex, info->FB, info->FbMapSize);
    info->FB = NULL;
    return TRUE;
}

/* Memory map the MMIO region and the frame buffer */
static Bool RADEONMapMem(ScrnInfoPtr pScrn)
{
    if (!RADEONMapMMIO(pScrn)) return FALSE;
    if (!RADEONMapFB(pScrn)) {
	RADEONUnmapMMIO(pScrn);
	return FALSE;
    }
    return TRUE;
}

/* Unmap the MMIO region and the frame buffer */
static Bool RADEONUnmapMem(ScrnInfoPtr pScrn)
{
    if (!RADEONUnmapMMIO(pScrn) || !RADEONUnmapFB(pScrn)) return FALSE;
    return TRUE;
}

void RADEONPllErrataAfterIndex(RADEONInfoPtr info)
{
    unsigned char *RADEONMMIO = info->MMIO;
	
    if (!(info->ChipErrata & CHIP_ERRATA_PLL_DUMMYREADS))
	return;

    /* This workaround is necessary on rv200 and RS200 or PLL
     * reads may return garbage (among others...)
     */
    (void)INREG(RADEON_CLOCK_CNTL_DATA);
    (void)INREG(RADEON_CRTC_GEN_CNTL);
}

void RADEONPllErrataAfterData(RADEONInfoPtr info)
{
    unsigned char *RADEONMMIO = info->MMIO;

    /* This workarounds is necessary on RV100, RS100 and RS200 chips
     * or the chip could hang on a subsequent access
     */
    if (info->ChipErrata & CHIP_ERRATA_PLL_DELAY) {
	/* we can't deal with posted writes here ... */
	usleep(5000);
    }

    /* This function is required to workaround a hardware bug in some (all?)
     * revisions of the R300.  This workaround should be called after every
     * CLOCK_CNTL_INDEX register access.  If not, register reads afterward
     * may not be correct.
     */
    if (info->ChipErrata & CHIP_ERRATA_R300_CG) {
	CARD32         save, tmp;

	save = INREG(RADEON_CLOCK_CNTL_INDEX);
	tmp = save & ~(0x3f | RADEON_PLL_WR_EN);
	OUTREG(RADEON_CLOCK_CNTL_INDEX, tmp);
	tmp = INREG(RADEON_CLOCK_CNTL_DATA);
	OUTREG(RADEON_CLOCK_CNTL_INDEX, save);
    }
}

/* Read PLL register */
unsigned RADEONINPLL(ScrnInfoPtr pScrn, int addr)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32         data;

    OUTREG8(RADEON_CLOCK_CNTL_INDEX, addr & 0x3f);
    RADEONPllErrataAfterIndex(info);
    data = INREG(RADEON_CLOCK_CNTL_DATA);
    RADEONPllErrataAfterData(info);

    return data;
}

/* Write PLL information */
void RADEONOUTPLL(ScrnInfoPtr pScrn, int addr, CARD32 data)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG8(RADEON_CLOCK_CNTL_INDEX, (((addr) & 0x3f) |
				      RADEON_PLL_WR_EN));
    RADEONPllErrataAfterIndex(info);
    OUTREG(RADEON_CLOCK_CNTL_DATA, data);
    RADEONPllErrataAfterData(info);
}


#if 0
/* Read PAL information (only used for debugging) */
static int RADEONINPAL(int idx)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG(RADEON_PALETTE_INDEX, idx << 16);
    return INREG(RADEON_PALETTE_DATA);
}
#endif

/* Wait for vertical sync on primary CRTC */
void RADEONWaitForVerticalSync(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32         crtc_gen_cntl;
    int            i;

    crtc_gen_cntl = INREG(RADEON_CRTC_GEN_CNTL);
    if ((crtc_gen_cntl & RADEON_CRTC_DISP_REQ_EN_B) ||
	!(crtc_gen_cntl & RADEON_CRTC_EN))
	return;

    /* Clear the CRTC_VBLANK_SAVE bit */
    OUTREG(RADEON_CRTC_STATUS, RADEON_CRTC_VBLANK_SAVE_CLEAR);

    /* Wait for it to go back up */
    for (i = 0; i < RADEON_TIMEOUT/1000; i++) {
	if (INREG(RADEON_CRTC_STATUS) & RADEON_CRTC_VBLANK_SAVE) break;
	usleep(1);
    }
}

/* Wait for vertical sync on secondary CRTC */
void RADEONWaitForVerticalSync2(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32         crtc2_gen_cntl;
    int            i;
 
    crtc2_gen_cntl = INREG(RADEON_CRTC2_GEN_CNTL);
    if ((crtc2_gen_cntl & RADEON_CRTC2_DISP_REQ_EN_B) ||
	!(crtc2_gen_cntl & RADEON_CRTC2_EN))
	return;

    /* Clear the CRTC2_VBLANK_SAVE bit */
    OUTREG(RADEON_CRTC2_STATUS, RADEON_CRTC2_VBLANK_SAVE_CLEAR);

    /* Wait for it to go back up */
    for (i = 0; i < RADEON_TIMEOUT/1000; i++) {
	if (INREG(RADEON_CRTC2_STATUS) & RADEON_CRTC2_VBLANK_SAVE) break;
	usleep(1);
    }
}

/* Blank screen */
static void RADEONBlank(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (!info->IsSecondary) {
	switch(info->DisplayType) {
	case MT_LCD:
	case MT_CRT:
	case MT_DFP:
	    OUTREGP(RADEON_CRTC_EXT_CNTL,
		    RADEON_CRTC_DISPLAY_DIS,
		    ~(RADEON_CRTC_DISPLAY_DIS));
	    break;

	case MT_NONE:
	default:
	    break;
	}
	if (info->MergedFB)
	    OUTREGP(RADEON_CRTC2_GEN_CNTL,
		    RADEON_CRTC2_DISP_DIS,
		    ~(RADEON_CRTC2_DISP_DIS));
    } else {
	OUTREGP(RADEON_CRTC2_GEN_CNTL,
		RADEON_CRTC2_DISP_DIS,
		~(RADEON_CRTC2_DISP_DIS));
    }
}

/* Unblank screen */
static void RADEONUnblank(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (!info->IsSecondary) {
	switch (info->DisplayType) {
	case MT_LCD:
	case MT_CRT:
	case MT_DFP:
	    OUTREGP(RADEON_CRTC_EXT_CNTL,
		    RADEON_CRTC_CRT_ON,
		    ~(RADEON_CRTC_DISPLAY_DIS));
	    break;

	case MT_NONE:
	default:
	    break;
	}
	if (info->MergedFB)
	    OUTREGP(RADEON_CRTC2_GEN_CNTL,
		    0,
		    ~(RADEON_CRTC2_DISP_DIS));
    } else {
	switch (info->DisplayType) {
	case MT_LCD:
	case MT_DFP:
	case MT_CRT:
	    OUTREGP(RADEON_CRTC2_GEN_CNTL,
		    0,
		    ~(RADEON_CRTC2_DISP_DIS));
	    break;

	case MT_NONE:
	default:
	    break;
	}
    }
}

/* Compute log base 2 of val */
int RADEONMinBits(int val)
{
    int  bits;

    if (!val) return 1;
    for (bits = 0; val; val >>= 1, ++bits);
    return bits;
}

/* Compute n/d with rounding */
static int RADEONDiv(int n, int d)
{
    return (n + (d / 2)) / d;
}

static RADEONMonitorType RADEONDisplayDDCConnected(ScrnInfoPtr pScrn, RADEONDDCType DDCType, RADEONConnector* port)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    unsigned long DDCReg;
    RADEONMonitorType MonType = MT_NONE;
    xf86MonPtr* MonInfo = &port->MonInfo;
    int i, j;

    DDCReg = info->DDCReg;
    switch(DDCType)
    {
    case DDC_MONID:
	info->DDCReg = RADEON_GPIO_MONID;
	break;
    case DDC_DVI:
	info->DDCReg = RADEON_GPIO_DVI_DDC;
	break;
    case DDC_VGA:
	info->DDCReg = RADEON_GPIO_VGA_DDC;
	break;
    case DDC_CRT2:
	info->DDCReg = RADEON_GPIO_CRT2_DDC;
	break;
    default:
	info->DDCReg = DDCReg;
	return MT_NONE;
    }

    /* Read and output monitor info using DDC2 over I2C bus */
    if (info->pI2CBus && info->ddc2) {
	OUTREG(info->DDCReg, INREG(info->DDCReg) &
	       (CARD32)~(RADEON_GPIO_A_0 | RADEON_GPIO_A_1));

	/* For some old monitors (like Compaq Presario FP500), we need
	 * following process to initialize/stop DDC
	 */
	OUTREG(info->DDCReg, INREG(info->DDCReg) & ~(RADEON_GPIO_EN_1));
	for (j = 0; j < 3; j++) {
	    OUTREG(info->DDCReg,
		   INREG(info->DDCReg) & ~(RADEON_GPIO_EN_0));
	    usleep(13000);

	    OUTREG(info->DDCReg,
		   INREG(info->DDCReg) & ~(RADEON_GPIO_EN_1));
	    for (i = 0; i < 10; i++) {
		usleep(15000);
		if (INREG(info->DDCReg) & RADEON_GPIO_Y_1)
		    break;
	    }
	    if (i == 10) continue;

	    usleep(15000);

	    OUTREG(info->DDCReg, INREG(info->DDCReg) | RADEON_GPIO_EN_0);
	    usleep(15000);

	    OUTREG(info->DDCReg, INREG(info->DDCReg) | RADEON_GPIO_EN_1);
	    usleep(15000);
	    OUTREG(info->DDCReg,
		   INREG(info->DDCReg) & ~(RADEON_GPIO_EN_0));
	    usleep(15000);
	    *MonInfo = xf86DoEDID_DDC2(pScrn->scrnIndex, info->pI2CBus);

	    OUTREG(info->DDCReg, INREG(info->DDCReg) | RADEON_GPIO_EN_1);
	    OUTREG(info->DDCReg, INREG(info->DDCReg) | RADEON_GPIO_EN_0);
	    usleep(15000);
	    OUTREG(info->DDCReg,
		   INREG(info->DDCReg) & ~(RADEON_GPIO_EN_1));
	    for (i = 0; i < 5; i++) {
		usleep(15000);
		if (INREG(info->DDCReg) & RADEON_GPIO_Y_1)
		    break;
	    }
	    usleep(15000);
	    OUTREG(info->DDCReg,
		   INREG(info->DDCReg) & ~(RADEON_GPIO_EN_0));
	    usleep(15000);

	    OUTREG(info->DDCReg, INREG(info->DDCReg) | RADEON_GPIO_EN_1);
	    OUTREG(info->DDCReg, INREG(info->DDCReg) | RADEON_GPIO_EN_0);
	    usleep(15000);
	    if(*MonInfo) break;
	}
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "DDC2/I2C is not properly initialized\n");
	MonType = MT_NONE;
    }

    OUTREG(info->DDCReg, INREG(info->DDCReg) &
	   ~(RADEON_GPIO_EN_0 | RADEON_GPIO_EN_1));

    if (*MonInfo) {
	if ((*MonInfo)->rawData[0x14] & 0x80) {
	    /* Note some laptops have a DVI output that uses internal TMDS,
	     * when its DVI is enabled by hotkey, LVDS panel is not used.
	     * In this case, the laptop is configured as DVI+VGA as a normal 
	     * desktop card.
	     * Also for laptop, when X starts with lid closed (no DVI connection)
	     * both LDVS and TMDS are disable, we still need to treat it as a LVDS panel.
	     */
	    if (port->TMDSType == TMDS_EXT) MonType = MT_DFP;
	    else {
		if ((INREG(RADEON_FP_GEN_CNTL) & (1<<7)) || !info->IsMobility)
		    MonType = MT_DFP;
		else 
		    MonType = MT_LCD;
	    }
	} else MonType = MT_CRT;
    } else MonType = MT_NONE;

    info->DDCReg = DDCReg;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "DDC Type: %d, Detected Type: %d\n", DDCType, MonType);

    return MonType;
}

static RADEONMonitorType
RADEONCrtIsPhysicallyConnected(ScrnInfoPtr pScrn, int IsCrtDac)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int		  bConnected = 0;

    /* the monitor either wasn't connected or it is a non-DDC CRT.
     * try to probe it
     */
    if(IsCrtDac) {
	unsigned long ulOrigVCLK_ECP_CNTL;
	unsigned long ulOrigDAC_CNTL;
	unsigned long ulOrigDAC_MACRO_CNTL;
	unsigned long ulOrigDAC_EXT_CNTL;
	unsigned long ulOrigCRTC_EXT_CNTL;
	unsigned long ulData;
	unsigned long ulMask;

	ulOrigVCLK_ECP_CNTL = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);

	ulData              = ulOrigVCLK_ECP_CNTL;
	ulData             &= ~(RADEON_PIXCLK_ALWAYS_ONb
				| RADEON_PIXCLK_DAC_ALWAYS_ONb);
	ulMask              = ~(RADEON_PIXCLK_ALWAYS_ONb
				|RADEON_PIXCLK_DAC_ALWAYS_ONb);
	OUTPLLP(pScrn, RADEON_VCLK_ECP_CNTL, ulData, ulMask);

	ulOrigCRTC_EXT_CNTL = INREG(RADEON_CRTC_EXT_CNTL);
	ulData              = ulOrigCRTC_EXT_CNTL;
	ulData             |= RADEON_CRTC_CRT_ON;
	OUTREG(RADEON_CRTC_EXT_CNTL, ulData);

	ulOrigDAC_EXT_CNTL = INREG(RADEON_DAC_EXT_CNTL);
	ulData             = ulOrigDAC_EXT_CNTL;
	ulData            &= ~RADEON_DAC_FORCE_DATA_MASK;
	ulData            |=  (RADEON_DAC_FORCE_BLANK_OFF_EN
			       |RADEON_DAC_FORCE_DATA_EN
			       |RADEON_DAC_FORCE_DATA_SEL_MASK);
	if ((info->ChipFamily == CHIP_FAMILY_RV250) ||
	    (info->ChipFamily == CHIP_FAMILY_RV280))
	    ulData |= (0x01b6 << RADEON_DAC_FORCE_DATA_SHIFT);
	else
	    ulData |= (0x01ac << RADEON_DAC_FORCE_DATA_SHIFT);

	OUTREG(RADEON_DAC_EXT_CNTL, ulData);

	ulOrigDAC_CNTL     = INREG(RADEON_DAC_CNTL);

	if (ulOrigDAC_CNTL & RADEON_DAC_PDWN) {
	    /* turn on power so testing can go through */
	    ulOrigDAC_MACRO_CNTL = INREG(RADEON_DAC_MACRO_CNTL);
	    ulOrigDAC_MACRO_CNTL &= ~(RADEON_DAC_PDWN_R | RADEON_DAC_PDWN_G |
		RADEON_DAC_PDWN_B);
	    OUTREG(RADEON_DAC_MACRO_CNTL, ulOrigDAC_MACRO_CNTL);
	}

	ulData             = ulOrigDAC_CNTL;
	ulData            |= RADEON_DAC_CMP_EN;
	ulData            &= ~(RADEON_DAC_RANGE_CNTL_MASK
			       | RADEON_DAC_PDWN);
	ulData            |= 0x2;
	OUTREG(RADEON_DAC_CNTL, ulData);

	usleep(10000);

	ulData     = INREG(RADEON_DAC_CNTL);
	bConnected =  (RADEON_DAC_CMP_OUTPUT & ulData)?1:0;

	ulData    = ulOrigVCLK_ECP_CNTL;
	ulMask    = 0xFFFFFFFFL;
	OUTPLLP(pScrn, RADEON_VCLK_ECP_CNTL, ulData, ulMask);

	OUTREG(RADEON_DAC_CNTL,      ulOrigDAC_CNTL     );
	OUTREG(RADEON_DAC_EXT_CNTL,  ulOrigDAC_EXT_CNTL );
	OUTREG(RADEON_CRTC_EXT_CNTL, ulOrigCRTC_EXT_CNTL);

	if (!bConnected) {
	    /* Power DAC down if CRT is not connected */
            ulOrigDAC_MACRO_CNTL = INREG(RADEON_DAC_MACRO_CNTL);
            ulOrigDAC_MACRO_CNTL |= (RADEON_DAC_PDWN_R | RADEON_DAC_PDWN_G |
	    	RADEON_DAC_PDWN_B);
            OUTREG(RADEON_DAC_MACRO_CNTL, ulOrigDAC_MACRO_CNTL);

	    ulData     = INREG(RADEON_DAC_CNTL);
	    ulData     |= RADEON_DAC_PDWN ;
	    OUTREG(RADEON_DAC_CNTL, ulData);
    	}
    } else { /* TV DAC */

        /* This doesn't seem to work reliably (maybe worse on some OEM cards),
           for now we always return false. If one wants to connected a
           non-DDC monitor on the DVI port when CRT port is also connected,
           he will need to explicitly tell the driver in the config file
           with Option MonitorLayout.
        */
        bConnected = FALSE;

#if 0
	if (info->ChipFamily == CHIP_FAMILY_R200) {
	    unsigned long ulOrigGPIO_MONID;
	    unsigned long ulOrigFP2_GEN_CNTL;
	    unsigned long ulOrigDISP_OUTPUT_CNTL;
	    unsigned long ulOrigCRTC2_GEN_CNTL;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_A;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_B;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_C;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_D;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_E;
	    unsigned long ulOrigDISP_LIN_TRANS_GRPH_F;
	    unsigned long ulOrigCRTC2_H_TOTAL_DISP;
	    unsigned long ulOrigCRTC2_V_TOTAL_DISP;
	    unsigned long ulOrigCRTC2_H_SYNC_STRT_WID;
	    unsigned long ulOrigCRTC2_V_SYNC_STRT_WID;
	    unsigned long ulData, i;

	    ulOrigGPIO_MONID = INREG(RADEON_GPIO_MONID);
	    ulOrigFP2_GEN_CNTL = INREG(RADEON_FP2_GEN_CNTL);
	    ulOrigDISP_OUTPUT_CNTL = INREG(RADEON_DISP_OUTPUT_CNTL);
	    ulOrigCRTC2_GEN_CNTL = INREG(RADEON_CRTC2_GEN_CNTL);
	    ulOrigDISP_LIN_TRANS_GRPH_A = INREG(RADEON_DISP_LIN_TRANS_GRPH_A);
	    ulOrigDISP_LIN_TRANS_GRPH_B = INREG(RADEON_DISP_LIN_TRANS_GRPH_B);
	    ulOrigDISP_LIN_TRANS_GRPH_C = INREG(RADEON_DISP_LIN_TRANS_GRPH_C);
	    ulOrigDISP_LIN_TRANS_GRPH_D = INREG(RADEON_DISP_LIN_TRANS_GRPH_D);
	    ulOrigDISP_LIN_TRANS_GRPH_E = INREG(RADEON_DISP_LIN_TRANS_GRPH_E);
	    ulOrigDISP_LIN_TRANS_GRPH_F = INREG(RADEON_DISP_LIN_TRANS_GRPH_F);

	    ulOrigCRTC2_H_TOTAL_DISP = INREG(RADEON_CRTC2_H_TOTAL_DISP);
	    ulOrigCRTC2_V_TOTAL_DISP = INREG(RADEON_CRTC2_V_TOTAL_DISP);
	    ulOrigCRTC2_H_SYNC_STRT_WID = INREG(RADEON_CRTC2_H_SYNC_STRT_WID);
	    ulOrigCRTC2_V_SYNC_STRT_WID = INREG(RADEON_CRTC2_V_SYNC_STRT_WID);

	    ulData     = INREG(RADEON_GPIO_MONID);
	    ulData    &= ~RADEON_GPIO_A_0;
	    OUTREG(RADEON_GPIO_MONID, ulData);

	    OUTREG(RADEON_FP2_GEN_CNTL, 0x0a000c0c);

	    OUTREG(RADEON_DISP_OUTPUT_CNTL, 0x00000012);

	    OUTREG(RADEON_CRTC2_GEN_CNTL, 0x06000000);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_A, 0x00000000);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_B, 0x000003f0);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_C, 0x00000000);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_D, 0x000003f0);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_E, 0x00000000);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_F, 0x000003f0);
	    OUTREG(RADEON_CRTC2_H_TOTAL_DISP, 0x01000008);
	    OUTREG(RADEON_CRTC2_H_SYNC_STRT_WID, 0x00000800);
	    OUTREG(RADEON_CRTC2_V_TOTAL_DISP, 0x00080001);
	    OUTREG(RADEON_CRTC2_V_SYNC_STRT_WID, 0x00000080);

	    for (i = 0; i < 200; i++) {
		ulData     = INREG(RADEON_GPIO_MONID);
		bConnected = (ulData & RADEON_GPIO_Y_0)?1:0;
		if (!bConnected) break;

		usleep(1000);
	    }

	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_A, ulOrigDISP_LIN_TRANS_GRPH_A);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_B, ulOrigDISP_LIN_TRANS_GRPH_B);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_C, ulOrigDISP_LIN_TRANS_GRPH_C);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_D, ulOrigDISP_LIN_TRANS_GRPH_D);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_E, ulOrigDISP_LIN_TRANS_GRPH_E);
	    OUTREG(RADEON_DISP_LIN_TRANS_GRPH_F, ulOrigDISP_LIN_TRANS_GRPH_F);
	    OUTREG(RADEON_CRTC2_H_TOTAL_DISP, ulOrigCRTC2_H_TOTAL_DISP);
	    OUTREG(RADEON_CRTC2_V_TOTAL_DISP, ulOrigCRTC2_V_TOTAL_DISP);
	    OUTREG(RADEON_CRTC2_H_SYNC_STRT_WID, ulOrigCRTC2_H_SYNC_STRT_WID);
	    OUTREG(RADEON_CRTC2_V_SYNC_STRT_WID, ulOrigCRTC2_V_SYNC_STRT_WID);
	    OUTREG(RADEON_CRTC2_GEN_CNTL, ulOrigCRTC2_GEN_CNTL);
	    OUTREG(RADEON_DISP_OUTPUT_CNTL, ulOrigDISP_OUTPUT_CNTL);
	    OUTREG(RADEON_FP2_GEN_CNTL, ulOrigFP2_GEN_CNTL);
	    OUTREG(RADEON_GPIO_MONID, ulOrigGPIO_MONID);
        } else {
	    unsigned long ulOrigPIXCLKSDATA;
	    unsigned long ulOrigTV_MASTER_CNTL;
	    unsigned long ulOrigTV_DAC_CNTL;
	    unsigned long ulOrigTV_PRE_DAC_MUX_CNTL;
	    unsigned long ulOrigDAC_CNTL2;
	    unsigned long ulData;
	    unsigned long ulMask;

	    ulOrigPIXCLKSDATA = INPLL(pScrn, RADEON_PIXCLKS_CNTL);

	    ulData            = ulOrigPIXCLKSDATA;
	    ulData           &= ~(RADEON_PIX2CLK_ALWAYS_ONb
				  | RADEON_PIX2CLK_DAC_ALWAYS_ONb);
	    ulMask            = ~(RADEON_PIX2CLK_ALWAYS_ONb
			  | RADEON_PIX2CLK_DAC_ALWAYS_ONb);
	    OUTPLLP(pScrn, RADEON_PIXCLKS_CNTL, ulData, ulMask);

	    ulOrigTV_MASTER_CNTL = INREG(RADEON_TV_MASTER_CNTL);
	    ulData               = ulOrigTV_MASTER_CNTL;
	    ulData              &= ~RADEON_TVCLK_ALWAYS_ONb;
	    OUTREG(RADEON_TV_MASTER_CNTL, ulData);

	    ulOrigDAC_CNTL2 = INREG(RADEON_DAC_CNTL2);
	    ulData          = ulOrigDAC_CNTL2;
	    ulData          &= ~RADEON_DAC2_DAC2_CLK_SEL;
	    OUTREG(RADEON_DAC_CNTL2, ulData);

	    ulOrigTV_DAC_CNTL = INREG(RADEON_TV_DAC_CNTL);

	    ulData  = 0x00880213;
	    OUTREG(RADEON_TV_DAC_CNTL, ulData);

	    ulOrigTV_PRE_DAC_MUX_CNTL = INREG(RADEON_TV_PRE_DAC_MUX_CNTL);

	    ulData  =  (RADEON_Y_RED_EN
			| RADEON_C_GRN_EN
			| RADEON_CMP_BLU_EN
			| RADEON_RED_MX_FORCE_DAC_DATA
			| RADEON_GRN_MX_FORCE_DAC_DATA
			| RADEON_BLU_MX_FORCE_DAC_DATA);
            if (IS_R300_VARIANT)
		ulData |= 0x180 << RADEON_TV_FORCE_DAC_DATA_SHIFT;
	    else
		ulData |= 0x1f5 << RADEON_TV_FORCE_DAC_DATA_SHIFT;
	    OUTREG(RADEON_TV_PRE_DAC_MUX_CNTL, ulData);

	    usleep(10000);

	    ulData     = INREG(RADEON_TV_DAC_CNTL);
	    bConnected = (ulData & RADEON_TV_DAC_CMPOUT)?1:0;

	    ulData    = ulOrigPIXCLKSDATA;
	    ulMask    = 0xFFFFFFFFL;
	    OUTPLLP(pScrn, RADEON_PIXCLKS_CNTL, ulData, ulMask);

	    OUTREG(RADEON_TV_MASTER_CNTL, ulOrigTV_MASTER_CNTL);
	    OUTREG(RADEON_DAC_CNTL2, ulOrigDAC_CNTL2);
	    OUTREG(RADEON_TV_DAC_CNTL, ulOrigTV_DAC_CNTL);
	    OUTREG(RADEON_TV_PRE_DAC_MUX_CNTL, ulOrigTV_PRE_DAC_MUX_CNTL);
	}
#endif
    }

    return(bConnected ? MT_CRT : MT_NONE);
}

static Bool RADEONProbePLLParameters(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONPLLPtr  pll  = &info->pll;
    unsigned char *RADEONMMIO = info->MMIO;
    unsigned char ppll_div_sel;
    unsigned mpll_fb_div, spll_fb_div, M;
    unsigned xclk, tmp, ref_div;
    int hTotal, vTotal, num, denom, m, n;
    float hz, prev_xtal, vclk, xtal, mpll, spll;
    long start_secs, start_usecs, stop_secs, stop_usecs, total_usecs;
    long to1_secs, to1_usecs, to2_secs, to2_usecs;
    unsigned int f1, f2, f3;
    int tries = 0;

    prev_xtal = 0;
 again:
    xtal = 0;
    if (++tries > 10)
           goto failed;

    xf86getsecs(&to1_secs, &to1_usecs);
    f1 = INREG(RADEON_CRTC_CRNT_FRAME);
    for (;;) {
       f2 = INREG(RADEON_CRTC_CRNT_FRAME);
       if (f1 != f2)
	    break;
       xf86getsecs(&to2_secs, &to2_usecs);
       if ((to2_secs - to1_secs) > 1) {
           xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Clock not counting...\n");
           goto failed;
       }
    }
    xf86getsecs(&start_secs, &start_usecs);
    for(;;) {
       f3 = INREG(RADEON_CRTC_CRNT_FRAME);
       if (f3 != f2)
	    break;
       xf86getsecs(&to2_secs, &to2_usecs);
       if ((to2_secs - start_secs) > 1)
           goto failed;
    }
    xf86getsecs(&stop_secs, &stop_usecs);

    if ((stop_secs - start_secs) != 0)
           goto again;
    total_usecs = abs(stop_usecs - start_usecs);
    if (total_usecs == 0)
           goto again;
    hz = 1000000.0/(float)total_usecs;

    hTotal = ((INREG(RADEON_CRTC_H_TOTAL_DISP) & 0x3ff) + 1) * 8;
    vTotal = ((INREG(RADEON_CRTC_V_TOTAL_DISP) & 0xfff) + 1);
    vclk = (float)(hTotal * (float)(vTotal * hz));

    switch((INPLL(pScrn, RADEON_PPLL_REF_DIV) & 0x30000) >> 16) {
    case 0:
    default:
        num = 1;
        denom = 1;
        break;
    case 1:
        n = ((INPLL(pScrn, RADEON_X_MPLL_REF_FB_DIV) >> 16) & 0xff);
        m = (INPLL(pScrn, RADEON_X_MPLL_REF_FB_DIV) & 0xff);
        num = 2*n;
        denom = 2*m;
        break;
    case 2:
        n = ((INPLL(pScrn, RADEON_X_MPLL_REF_FB_DIV) >> 8) & 0xff);
        m = (INPLL(pScrn, RADEON_X_MPLL_REF_FB_DIV) & 0xff);
        num = 2*n;
        denom = 2*m;
        break;
     }

    ppll_div_sel = INREG8(RADEON_CLOCK_CNTL_INDEX + 1) & 0x3;
    RADEONPllErrataAfterIndex(info);

    n = (INPLL(pScrn, RADEON_PPLL_DIV_0 + ppll_div_sel) & 0x7ff);
    m = (INPLL(pScrn, RADEON_PPLL_REF_DIV) & 0x3ff);

    num *= n;
    denom *= m;

    switch ((INPLL(pScrn, RADEON_PPLL_DIV_0 + ppll_div_sel) >> 16) & 0x7) {
    case 1:
        denom *= 2;
        break;
    case 2:
        denom *= 4;
        break;
    case 3:
        denom *= 8;
        break;
    case 4:
        denom *= 3;
        break;
    case 6:
        denom *= 6;
        break;
    case 7:
        denom *= 12;
        break;
    }

    xtal = (int)(vclk *(float)denom/(float)num);

    if ((xtal > 26900000) && (xtal < 27100000))
        xtal = 2700;
    else if ((xtal > 14200000) && (xtal < 14400000))
        xtal = 1432;
    else if ((xtal > 29400000) && (xtal < 29600000))
        xtal = 2950;
    else
       goto again;
 failed:
    if (xtal == 0) {
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Failed to probe xtal value ! "
                  "Using default 27Mhz\n");
       xtal = 2700;
    } else {
       if (prev_xtal == 0) {
           prev_xtal = xtal;
           tries = 0;
           goto again;
       } else if (prev_xtal != xtal) {
           prev_xtal = 0;
           goto again;
       }
    }

    tmp = INPLL(pScrn, RADEON_X_MPLL_REF_FB_DIV);
    ref_div = INPLL(pScrn, RADEON_PPLL_REF_DIV) & 0x3ff;

    /* Some sanity check based on the BIOS code .... */
    if (ref_div < 2) {
       CARD32 tmp;
       tmp = INPLL(pScrn, RADEON_PPLL_REF_DIV);
       if (IS_R300_VARIANT || (info->ChipFamily == CHIP_FAMILY_RS300))
           ref_div = (tmp & R300_PPLL_REF_DIV_ACC_MASK) >>
                   R300_PPLL_REF_DIV_ACC_SHIFT;
       else
           ref_div = tmp & RADEON_PPLL_REF_DIV_MASK;
       if (ref_div < 2)
           ref_div = 12;
    }

    /* Calculate "base" xclk straight from MPLL, though that isn't
     * really useful (hopefully). This isn't called XCLK anymore on
     * radeon's...
     */
    mpll_fb_div = (tmp & 0xff00) >> 8;
    spll_fb_div = (tmp & 0xff0000) >> 16;
    M = (tmp & 0xff);
    xclk = RADEONDiv((2 * mpll_fb_div * xtal), (M));

    /*
     * Calculate MCLK based on MCLK-A
     */
    mpll = (2.0 * (float)mpll_fb_div * (xtal / 100.0)) / (float)M;
    spll = (2.0 * (float)spll_fb_div * (xtal / 100.0)) / (float)M;

    tmp = INPLL(pScrn, RADEON_MCLK_CNTL) & 0x7;
    switch(tmp) {
    case 1: info->mclk = mpll; break;
    case 2: info->mclk = mpll / 2.0; break;
    case 3: info->mclk = mpll / 4.0; break;
    case 4: info->mclk = mpll / 8.0; break;
    case 7: info->mclk = spll; break;
    default:
           info->mclk = 200.00;
           xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Unsupported MCLKA source"
                      " setting %d, can't probe MCLK value !\n", tmp);
    }

    /*
     * Calculate SCLK
     */
    tmp = INPLL(pScrn, RADEON_SCLK_CNTL) & 0x7;
    switch(tmp) {
    case 1: info->sclk = spll; break;
    case 2: info->sclk = spll / 2.0; break;
    case 3: info->sclk = spll / 4.0; break;
    case 4: info->sclk = spll / 8.0; break;
    case 7: info->sclk = mpll;
    default:
           info->sclk = 200.00;
           xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Unsupported SCLK source"
                      " setting %d, can't probe SCLK value !\n", tmp);
    }

    /* we're done, hopefully these are sane values */
    pll->reference_div = ref_div;
    pll->xclk = xclk;
    pll->reference_freq = xtal;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Probed PLL values: xtal: %f Mhz, "
              "sclk: %f Mhz, mclk: %f Mhz\n", xtal/100.0, info->sclk, info->mclk);

    return TRUE;
}

static void RADEONGetPanelInfoFromReg (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info     = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32 fp_vert_stretch = INREG(RADEON_FP_VERT_STRETCH);
    CARD32 fp_horz_stretch = INREG(RADEON_FP_HORZ_STRETCH);

    info->PanelPwrDly = 200;
    if (fp_vert_stretch & RADEON_VERT_STRETCH_ENABLE) {
	info->PanelYRes = (fp_vert_stretch>>12) + 1;
    } else {
	info->PanelYRes = (INREG(RADEON_CRTC_V_TOTAL_DISP)>>16) + 1;
    }
    if (fp_horz_stretch & RADEON_HORZ_STRETCH_ENABLE) {
	info->PanelXRes = ((fp_horz_stretch>>16) + 1) * 8;
    } else {
	info->PanelXRes = ((INREG(RADEON_CRTC_H_TOTAL_DISP)>>16) + 1) * 8;
    }
    
    if ((info->PanelXRes < 640) || (info->PanelYRes < 480)) {
	info->PanelXRes = 640;
	info->PanelYRes = 480;
    }

    if (xf86ReturnOptValBool(info->Options, OPTION_LVDS_PROBE_PLL, TRUE)) {
           CARD32 ppll_div_sel, ppll_val;

           ppll_div_sel = INREG8(RADEON_CLOCK_CNTL_INDEX + 1) & 0x3;
	   RADEONPllErrataAfterIndex(info);
	   ppll_val = INPLL(pScrn, RADEON_PPLL_DIV_0 + ppll_div_sel);
           if ((ppll_val & 0x000707ff) == 0x1bb)
		   goto noprobe;
	   info->FeedbackDivider = ppll_val & 0x7ff;
	   info->PostDivider = (ppll_val >> 16) & 0x7;
	   info->RefDivider = info->pll.reference_div;
	   info->UseBiosDividers = TRUE;

           xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                      "Existing panel PLL dividers will be used.\n");
    }
 noprobe:

    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
	       "Panel size %dx%d is derived, this may not be correct.\n"
		   "If not, use PanelSize option to overwrite this setting\n",
	       info->PanelXRes, info->PanelYRes);
}

static Bool RADEONGetLVDSInfo (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info     = RADEONPTR(pScrn);

    if (!RADEONGetLVDSInfoFromBIOS(pScrn))
        RADEONGetPanelInfoFromReg(pScrn);

    /* The panel size we collected from BIOS may not be the
     * maximum size supported by the panel.  If not, we update
     * it now.  These will be used if no matching mode can be
     * found from EDID data.
     */
    RADEONUpdatePanelSize(pScrn);

    /* No timing information for the native mode,
     * use whatever specified in the Modeline.
     * If no Modeline specified, we'll just pick
     * the VESA mode at 60Hz refresh rate which
     * is likely to be the best for a flat panel.
     */
    if (info->DotClock == 0) {
        RADEONEntPtr pRADEONEnt   = RADEONEntPriv(pScrn);
        DisplayModePtr  tmp_mode = NULL;
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "No valid timing info from BIOS.\n");
        tmp_mode = pScrn->monitor->Modes;
        while(tmp_mode) {
            if ((tmp_mode->HDisplay == info->PanelXRes) &&
                (tmp_mode->VDisplay == info->PanelYRes)) {

                float  refresh =
                    (float)tmp_mode->Clock * 1000.0 / tmp_mode->HTotal / tmp_mode->VTotal;
                if ((abs(60.0 - refresh) < 1.0) ||
                    (tmp_mode->type == 0)) {
                    info->HBlank     = tmp_mode->HTotal - tmp_mode->HDisplay;
                    info->HOverPlus  = tmp_mode->HSyncStart - tmp_mode->HDisplay;
                    info->HSyncWidth = tmp_mode->HSyncEnd - tmp_mode->HSyncStart;
                    info->VBlank     = tmp_mode->VTotal - tmp_mode->VDisplay;
                    info->VOverPlus  = tmp_mode->VSyncStart - tmp_mode->VDisplay;
                    info->VSyncWidth = tmp_mode->VSyncEnd - tmp_mode->VSyncStart;
                    info->DotClock   = tmp_mode->Clock;
                    info->Flags = 0;
                    break;
                }
            }
            tmp_mode = tmp_mode->next;
        }
        if ((info->DotClock == 0) && !pRADEONEnt->PortInfo[0].MonInfo) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Panel size is not correctly detected.\n"
                       "Please try to use PanelSize option for correct settings.\n");
            return FALSE;
        }
    }

    return TRUE;
}

static void RADEONGetTMDSInfo(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    int i;

    for (i=0; i<4; i++) {
        info->tmds_pll[i].value = 0;
        info->tmds_pll[i].freq = 0;
    }

    if (RADEONGetTMDSInfoFromBIOS(pScrn)) return;

    for (i=0; i<4; i++) {
        info->tmds_pll[i].value = default_tmds_pll[info->ChipFamily][i].value;
        info->tmds_pll[i].freq = default_tmds_pll[info->ChipFamily][i].freq;
    }
}

static void RADEONGetPanelInfo (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info     = RADEONPTR(pScrn);
    char* s;

    if((s = xf86GetOptValString(info->Options, OPTION_PANEL_SIZE))) {
        info->PanelPwrDly = 200;
        if (sscanf (s, "%dx%d", &info->PanelXRes, &info->PanelYRes) != 2) {
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Invalid PanelSize option: %s\n", s);
            RADEONGetPanelInfoFromReg(pScrn);
        }
    } else {

        if(info->DisplayType == MT_LCD) {
            RADEONGetLVDSInfo(pScrn);
        } else if ((info->DisplayType == MT_DFP) || (info->MergeType == MT_DFP)) {
            RADEONGetTMDSInfo(pScrn);
            if (!pScrn->monitor->DDC)
                RADEONGetHardCodedEDIDFromBIOS(pScrn);
            else if (!info->IsSecondary)
               RADEONUpdatePanelSize(pScrn);
        }
    }
}

static void RADEONGetClockInfo(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    RADEONPLLPtr pll = &info->pll;
    double min_dotclock;

    if (RADEONGetClockInfoFromBIOS(pScrn)) {
	if (pll->reference_div < 2) {
	    /* retrive it from register setting for fitting into current PLL algorithm.
	       We'll probably need a new routine to calculate the best ref_div from BIOS 
	       provided min_input_pll and max_input_pll 
	    */
	    CARD32 tmp;
	    tmp = INPLL(pScrn, RADEON_PPLL_REF_DIV);
	    if (IS_R300_VARIANT ||
		(info->ChipFamily == CHIP_FAMILY_RS300)) {
		pll->reference_div = (tmp & R300_PPLL_REF_DIV_ACC_MASK) >> R300_PPLL_REF_DIV_ACC_SHIFT;
	    } else {
		pll->reference_div = tmp & RADEON_PPLL_REF_DIV_MASK;
	    }

	    if (pll->reference_div < 2) pll->reference_div = 12;
	}
	
    } else {
	xf86DrvMsg (pScrn->scrnIndex, X_WARNING,
		    "Video BIOS not detected, using default clock settings!\n");

       /* Default min/max PLL values */
       if (info->ChipFamily == CHIP_FAMILY_R420 || info->ChipFamily == CHIP_FAMILY_RV410) {
           pll->min_pll_freq = 20000;
           pll->max_pll_freq = 50000;
       } else {
           pll->min_pll_freq = 12500;
           pll->max_pll_freq = 35000;
       }

       if (RADEONProbePLLParameters(pScrn))
            return;

	if (info->IsIGP)
	    pll->reference_freq = 1432;
	else
	    pll->reference_freq = 2700;

	pll->reference_div = 12;
	pll->xclk = 10300;

        info->sclk = 200.00;
        info->mclk = 200.00;
    }

    if (info->ChipFamily == CHIP_FAMILY_RV100 && !info->HasCRTC2) {
        /* Avoid RN50 corruption due to memory bandwidth starvation.
         * 18 is an empirical value based on the databook and Windows driver.
         *
	 * Empirical value changed to 24 to raise pixel clock limit and
	 * allow higher resolution modes on capable monitors
	 */
        pll->max_pll_freq = min(pll->max_pll_freq,
                               24 * info->mclk * 100 / pScrn->bitsPerPixel *
                               info->RamWidth / 16);
    }

    xf86DrvMsg (pScrn->scrnIndex, X_INFO,
		"PLL parameters: rf=%d rd=%d min=%ld max=%ld; xclk=%d\n",
		pll->reference_freq,
		pll->reference_div,
		pll->min_pll_freq, pll->max_pll_freq, pll->xclk);

    /* (Some?) Radeon BIOSes seem too lie about their minimum dot
     * clocks.  Allow users to override the detected minimum dot clock
     * value (e.g., and allow it to be suitable for TV sets).
     */
    if (xf86GetOptValFreq(info->Options, OPTION_MIN_DOTCLOCK,
			  OPTUNITS_MHZ, &min_dotclock)) {
	if (min_dotclock < 12 || min_dotclock*100 >= pll->max_pll_freq) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Illegal minimum dotclock specified %.2f MHz "
		       "(option ignored)\n",
		       min_dotclock);
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Forced minimum dotclock to %.2f MHz "
		       "(instead of detected %.2f MHz)\n",
		       min_dotclock, ((double)pll->min_pll_freq/1000));
	    pll->min_pll_freq = min_dotclock * 1000;
	}
    }
}

static BOOL RADEONQueryConnectedMonitors(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt  = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    const char *s;
    Bool ignore_edid = FALSE;
    int i = 0, second = 0, max_mt;

    const char *MonTypeName[7] =
    {
	"AUTO",
	"NONE",
	"CRT",
	"LVDS",
	"TMDS",
	"CTV",
	"STV"
    };

    const RADEONMonitorType MonTypeID[7] =
    {
	MT_UNKNOWN, /* this is just a dummy value for AUTO DETECTION */
	MT_NONE,    /* NONE -> NONE */
	MT_CRT,     /* CRT -> CRT */
	MT_LCD,     /* Laptop LCDs are driven via LVDS port */
	MT_DFP,     /* DFPs are driven via TMDS */
	MT_CTV,     /* CTV -> CTV */
	MT_STV,     /* STV -> STV */
    };

    const char *TMDSTypeName[3] =
    {
	"NONE",
	"Internal",
    "External"
    };

    const char *DDCTypeName[5] =
    {
	"NONE",
	"MONID",
	"DVI_DDC",
	"VGA_DDC",
    "CRT2_DDC"
    };

    const char *DACTypeName[3] =
    {
	"Unknown",
	"Primary",
	"TVDAC/ExtDAC",
    };

    const char *ConnectorTypeName[8] =
    {
	"None",
	"Proprietary",
	"VGA",
	"DVI-I",
	"DVI-D",
	"CTV",
	"STV",
	"Unsupported"
    };

    const char *ConnectorTypeNameATOM[10] =
    {
	"None",
	"VGA",
	"DVI-I",
	"DVI-D",
	"DVI-A",
	"STV",
	"CTV",
	"LVDS",
	"Digital",
	"Unsupported"
    };

    max_mt = 5;

    if(info->IsSecondary) {
	info->DisplayType = (RADEONMonitorType)pRADEONEnt->MonType2;
	if(info->DisplayType == MT_NONE) return FALSE;
	return TRUE;
    }


    /* We first get the information about all connectors from BIOS.
     * This is how the card is phyiscally wired up.
     * The information should be correct even on a OEM card.
     * If not, we may have problem -- need to use MonitorLayout option.
     */
    for (i = 0; i < 2; i++) {
	pRADEONEnt->PortInfo[i].MonType = MT_UNKNOWN;
	pRADEONEnt->PortInfo[i].MonInfo = NULL;
	pRADEONEnt->PortInfo[i].DDCType = DDC_NONE_DETECTED;
	pRADEONEnt->PortInfo[i].DACType = DAC_UNKNOWN;
	pRADEONEnt->PortInfo[i].TMDSType = TMDS_UNKNOWN;
	pRADEONEnt->PortInfo[i].ConnectorType = CONNECTOR_NONE;
    }

    if (!RADEONGetConnectorInfoFromBIOS(pScrn)) {
	/* Below is the most common setting, but may not be true */
	pRADEONEnt->PortInfo[0].MonType = MT_UNKNOWN;
	pRADEONEnt->PortInfo[0].MonInfo = NULL;
	pRADEONEnt->PortInfo[0].DDCType = DDC_DVI;
	pRADEONEnt->PortInfo[0].DACType = DAC_TVDAC;
	pRADEONEnt->PortInfo[0].TMDSType = TMDS_INT;
	pRADEONEnt->PortInfo[0].ConnectorType = CONNECTOR_DVI_D;

	pRADEONEnt->PortInfo[1].MonType = MT_UNKNOWN;
	pRADEONEnt->PortInfo[1].MonInfo = NULL;
	pRADEONEnt->PortInfo[1].DDCType = DDC_VGA;
	pRADEONEnt->PortInfo[1].DACType = DAC_PRIMARY;
	pRADEONEnt->PortInfo[1].TMDSType = TMDS_EXT;
	pRADEONEnt->PortInfo[1].ConnectorType = CONNECTOR_CRT;

       /* Some cards have the DDC lines swapped and we have no way to
        * detect it yet (Mac cards)
        */
       if (xf86ReturnOptValBool(info->Options, OPTION_REVERSE_DDC, FALSE)) {
           pRADEONEnt->PortInfo[0].DDCType = DDC_VGA;
           pRADEONEnt->PortInfo[1].DDCType = DDC_DVI;
        }
    }

    /* always make TMDS_INT port first*/
    if (pRADEONEnt->PortInfo[1].TMDSType == TMDS_INT) {
        RADEONConnector connector;
        connector = pRADEONEnt->PortInfo[0];
        pRADEONEnt->PortInfo[0] = pRADEONEnt->PortInfo[1];
        pRADEONEnt->PortInfo[1] = connector;
    } else if ((pRADEONEnt->PortInfo[0].TMDSType != TMDS_INT &&
                pRADEONEnt->PortInfo[1].TMDSType != TMDS_INT)) {
        /* no TMDS_INT port, make primary DAC port first */
	/* On my Inspiron 8600 both internal and external ports are
	   marked DAC_PRIMARY in BIOS. So be extra careful - only
	   swap when the first port is not DAC_PRIMARY */
        if ( (pRADEONEnt->PortInfo[1].DACType == DAC_PRIMARY) &&
	     (pRADEONEnt->PortInfo[0].DACType != DAC_PRIMARY)) {
            RADEONConnector connector;
            connector = pRADEONEnt->PortInfo[0];
            pRADEONEnt->PortInfo[0] = pRADEONEnt->PortInfo[1];
            pRADEONEnt->PortInfo[1] = connector;
        }
    }

    if (info->HasSingleDAC) {
        /* For RS300/RS350/RS400 chips, there is no primary DAC. Force VGA port to use TVDAC*/
        if (pRADEONEnt->PortInfo[0].ConnectorType == CONNECTOR_CRT) {
            pRADEONEnt->PortInfo[0].DACType = DAC_TVDAC;
            pRADEONEnt->PortInfo[1].DACType = DAC_PRIMARY;
        } else {
            pRADEONEnt->PortInfo[1].DACType = DAC_TVDAC;
            pRADEONEnt->PortInfo[0].DACType = DAC_PRIMARY;
        }
    } else if (!info->HasCRTC2) {
        pRADEONEnt->PortInfo[0].DACType = DAC_PRIMARY;
    }

    /* IgnoreEDID option is different from the NoDDCxx options used by DDC module
     * When IgnoreEDID is used, monitor detection will still use DDC
     * detection, but all EDID data will not be used in mode validation.
     * You can use this option when you have a DDC monitor but want specify your own
     * monitor timing parameters by using HSync, VRefresh and Modeline,
     */
    if (xf86GetOptValBool(info->Options, OPTION_IGNORE_EDID, &ignore_edid)) {
        if (ignore_edid)
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                       "IgnoreEDID is specified, EDID data will be ignored\n");
    }

    /*
     * MonitorLayout option takes a string for two monitors connected in following format:
     * Option "MonitorLayout" "primary-port-display, secondary-port-display"
     * primary and secondary port displays can have one of following:
     *    NONE, CRT, LVDS, TMDS
     * With this option, driver will bring up monitors as specified,
     * not using auto-detection routines to probe monitors.
     *
     * This option can be used when the false monitor detection occurs.
     *
     * This option can also be used to disable one connected display.
     * For example, if you have a laptop connected to an external CRT
     * and you want to disable the internal LCD panel, you can specify
     * Option "MonitorLayout" "NONE, CRT"
     *
     * This option can also used to disable Clone mode. One there is only
     * one monitor is specified, clone mode will be turned off automatically
     * even you have two monitors connected.
     *
     * Another usage of this option is you want to config the server
     * to start up with a certain monitor arrangement even one monitor
     * is not plugged in when server starts.
     */
    if ((s = xf86GetOptValString(info->Options, OPTION_MONITOR_LAYOUT))) {
        char s1[5], s2[5];
        i = 0;
        /* When using user specified monitor types, we will not do DDC detection
         *
         */
        do {
            switch(*s) {
            case ',':
                s1[i] = '\0';
                i = 0;
                second = 1;
                break;
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                break;
            default:
                if (second)
                    s2[i] = *s;
                else
                    s1[i] = *s;
                i++;
                break;
            }
            if (i > 4) i = 4;
        } while(*s++);
        s2[i] = '\0';

	for (i = 0; i < max_mt; i++) {
	    if (strcmp(s1, MonTypeName[i]) == 0) {
		pRADEONEnt->PortInfo[0].MonType = MonTypeID[i];
		break;
	    }
	}
	for (i = 0; i < max_mt; i++) {
	    if (strcmp(s2, MonTypeName[i]) == 0) {
		pRADEONEnt->PortInfo[1].MonType = MonTypeID[i];
		break;
	    }
	}

	if (i ==  max_mt)
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Invalid Monitor type specified for 2nd port \n");

	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "MonitorLayout Option: \n\tMonitor1--Type %s, Monitor2--Type %s\n\n", s1, s2);
#if 0
	if (pRADEONEnt->PortInfo[1].MonType == MT_CRT) {
	    pRADEONEnt->PortInfo[1].DACType = DAC_PRIMARY;
	    pRADEONEnt->PortInfo[1].TMDSType = TMDS_UNKNOWN;
	    pRADEONEnt->PortInfo[1].DDCType = DDC_VGA;
	    pRADEONEnt->PortInfo[1].ConnectorType = CONNECTOR_CRT;
	    pRADEONEnt->PortInfo[0].DACType = DAC_TVDAC;
	    pRADEONEnt->PortInfo[0].TMDSType = TMDS_UNKNOWN;
	    pRADEONEnt->PortInfo[0].DDCType = DDC_NONE_DETECTED;
	    pRADEONEnt->PortInfo[0].ConnectorType = pRADEONEnt->PortInfo[0].MonType+1;
	    pRADEONEnt->PortInfo[0].MonInfo = NULL;
        }
#endif

        if (!ignore_edid) {
            if ((pRADEONEnt->PortInfo[0].MonType > MT_NONE) &&
                (pRADEONEnt->PortInfo[0].MonType < MT_STV))
		RADEONDisplayDDCConnected(pScrn, pRADEONEnt->PortInfo[0].DDCType,
					  &pRADEONEnt->PortInfo[0]);
            if ((pRADEONEnt->PortInfo[1].MonType > MT_NONE) &&
                (pRADEONEnt->PortInfo[1].MonType < MT_STV))
		RADEONDisplayDDCConnected(pScrn, pRADEONEnt->PortInfo[1].DDCType,
					  &pRADEONEnt->PortInfo[1]);
        }

    }

    if(((!info->HasCRTC2) || info->IsDellServer)) {
	if (pRADEONEnt->PortInfo[0].MonType == MT_UNKNOWN) {
	    if((pRADEONEnt->PortInfo[0].MonType = RADEONDisplayDDCConnected(pScrn, DDC_DVI, &pRADEONEnt->PortInfo[0])));
	    else if((pRADEONEnt->PortInfo[0].MonType = RADEONDisplayDDCConnected(pScrn, DDC_VGA, &pRADEONEnt->PortInfo[0])));
	    else if((pRADEONEnt->PortInfo[0].MonType = RADEONDisplayDDCConnected(pScrn, DDC_CRT2, &pRADEONEnt->PortInfo[0])));
	    else
		pRADEONEnt->PortInfo[0].MonType = MT_CRT;
	}

	if (!ignore_edid) {
	    if (pRADEONEnt->PortInfo[0].MonInfo) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Monitor1 EDID data ---------------------------\n");
		xf86PrintEDID(pRADEONEnt->PortInfo[0].MonInfo );
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "End of Monitor1 EDID data --------------------\n");
	    }
	}

	pRADEONEnt->MonType1 = pRADEONEnt->PortInfo[0].MonType;
	pRADEONEnt->MonInfo1 = pRADEONEnt->PortInfo[0].MonInfo;
	pRADEONEnt->MonType2 = MT_NONE;
	pRADEONEnt->MonInfo2 = NULL;
	info->MergeType = MT_NONE;
	info->DisplayType = pRADEONEnt->MonType1;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Primary:\n Monitor   -- %s\n Connector -- %s\n DAC Type  -- %s\n TMDS Type -- %s\n DDC Type  -- %s\n",
		   MonTypeName[pRADEONEnt->PortInfo[0].MonType+1],
		   info->IsAtomBios ?
		   ConnectorTypeNameATOM[pRADEONEnt->PortInfo[0].ConnectorType]:
		   ConnectorTypeName[pRADEONEnt->PortInfo[0].ConnectorType],
		   DACTypeName[pRADEONEnt->PortInfo[0].DACType+1],
		   TMDSTypeName[pRADEONEnt->PortInfo[0].TMDSType+1],
		   DDCTypeName[pRADEONEnt->PortInfo[0].DDCType]);

	return TRUE;
    }

    if (pRADEONEnt->PortInfo[0].MonType == MT_UNKNOWN || pRADEONEnt->PortInfo[1].MonType == MT_UNKNOWN) {

	/* Primary Head (DVI or Laptop Int. panel)*/
	/* A ddc capable display connected on DVI port */
	if (pRADEONEnt->PortInfo[0].MonType == MT_UNKNOWN) {
	    if((pRADEONEnt->PortInfo[0].MonType = RADEONDisplayDDCConnected(pScrn, pRADEONEnt->PortInfo[0].DDCType, &pRADEONEnt->PortInfo[0])));
	    else if (info->IsMobility &&
		     (INREG(RADEON_BIOS_4_SCRATCH) & 4)) {
		/* non-DDC laptop panel connected on primary */
		pRADEONEnt->PortInfo[0].MonType = MT_LCD;
	    } else {
		/* CRT on DVI, TODO: not reliable, make it always return false for now*/
		pRADEONEnt->PortInfo[0].MonType = RADEONCrtIsPhysicallyConnected(pScrn, !(pRADEONEnt->PortInfo[0].DACType));
	    }
	}

	/* Secondary Head (mostly VGA, can be DVI on some OEM boards)*/
	if (pRADEONEnt->PortInfo[1].MonType == MT_UNKNOWN) {
	    if((pRADEONEnt->PortInfo[1].MonType =
                RADEONDisplayDDCConnected(pScrn, pRADEONEnt->PortInfo[1].DDCType, &pRADEONEnt->PortInfo[1])));
            else if (info->IsMobility &&
                     (INREG(RADEON_FP2_GEN_CNTL) & RADEON_FP2_ON)) {
                /* non-DDC TMDS panel connected through DVO */
                pRADEONEnt->PortInfo[1].MonType = MT_DFP;
            } else
                pRADEONEnt->PortInfo[1].MonType = RADEONCrtIsPhysicallyConnected(pScrn, !(pRADEONEnt->PortInfo[1].DACType));
        }
    }

    if(ignore_edid) {
        pRADEONEnt->PortInfo[0].MonInfo = NULL;
        pRADEONEnt->PortInfo[1].MonInfo = NULL;
    } else {
        if (pRADEONEnt->PortInfo[0].MonInfo) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "EDID data from the display on port 1 ----------------------\n");
            xf86PrintEDID(pRADEONEnt->PortInfo[0].MonInfo );
        }

        if (pRADEONEnt->PortInfo[1].MonInfo) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "EDID data from the display on port 2-----------------------\n");
            xf86PrintEDID(pRADEONEnt->PortInfo[1].MonInfo );
        }
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "\n");

    pRADEONEnt->MonType1 = pRADEONEnt->PortInfo[0].MonType;
    pRADEONEnt->MonInfo1 = pRADEONEnt->PortInfo[0].MonInfo;
    pRADEONEnt->MonType2 = pRADEONEnt->PortInfo[1].MonType;
    pRADEONEnt->MonInfo2 = pRADEONEnt->PortInfo[1].MonInfo;
    if (pRADEONEnt->PortInfo[0].MonType == MT_NONE) {
	if (pRADEONEnt->PortInfo[1].MonType == MT_NONE) {
	    pRADEONEnt->MonType1 = MT_CRT;
	    pRADEONEnt->MonInfo1 = NULL;
	} else {
	    RADEONConnector tmp;
	    pRADEONEnt->MonType1 = pRADEONEnt->PortInfo[1].MonType;
	    pRADEONEnt->MonInfo1 = pRADEONEnt->PortInfo[1].MonInfo;
	    tmp = pRADEONEnt->PortInfo[0];
	    pRADEONEnt->PortInfo[0] = pRADEONEnt->PortInfo[1];
	    pRADEONEnt->PortInfo[1] = tmp;
	}
	pRADEONEnt->MonType2 = MT_NONE;
	pRADEONEnt->MonInfo2 = NULL;
    }

    info->DisplayType = pRADEONEnt->MonType1;
    pRADEONEnt->ReversedDAC = FALSE;
    info->OverlayOnCRTC2 = FALSE;
    info->MergeType = MT_NONE;
    if (pRADEONEnt->MonType2 != MT_NONE) {
	if(!pRADEONEnt->HasSecondary) {
	    info->MergeType = pRADEONEnt->MonType2;
	}

	if (pRADEONEnt->PortInfo[1].DACType == DAC_TVDAC) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Reversed DAC decteced\n");
	    pRADEONEnt->ReversedDAC = TRUE;
	}
    } else {
	pRADEONEnt->HasSecondary = FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Primary:\n Monitor   -- %s\n Connector -- %s\n DAC Type  -- %s\n TMDS Type -- %s\n DDC Type  -- %s\n",
	       MonTypeName[pRADEONEnt->PortInfo[0].MonType+1],
	       info->IsAtomBios ?
	       ConnectorTypeNameATOM[pRADEONEnt->PortInfo[0].ConnectorType]:
	       ConnectorTypeName[pRADEONEnt->PortInfo[0].ConnectorType],
	       DACTypeName[pRADEONEnt->PortInfo[0].DACType+1],
	       TMDSTypeName[pRADEONEnt->PortInfo[0].TMDSType+1],
	       DDCTypeName[pRADEONEnt->PortInfo[0].DDCType]);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Secondary:\n Monitor   -- %s\n Connector -- %s\n DAC Type  -- %s\n TMDS Type -- %s\n DDC Type  -- %s\n",
	       MonTypeName[pRADEONEnt->PortInfo[1].MonType+1],
	       info->IsAtomBios ?
	       ConnectorTypeNameATOM[pRADEONEnt->PortInfo[1].ConnectorType]:
	       ConnectorTypeName[pRADEONEnt->PortInfo[1].ConnectorType],
	       DACTypeName[pRADEONEnt->PortInfo[1].DACType+1],
	       TMDSTypeName[pRADEONEnt->PortInfo[1].TMDSType+1],
	       DDCTypeName[pRADEONEnt->PortInfo[1].DDCType]);

    return TRUE;
}


/* This is called by RADEONPreInit to set up the default visual */
static Bool RADEONPreInitVisual(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (!xf86SetDepthBpp(pScrn, 0, 0, 0, Support32bppFb))
	return FALSE;

    switch (pScrn->depth) {
    case 8:
    case 15:
    case 16:
    case 24:
	break;

    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Given depth (%d) is not supported by %s driver\n",
		   pScrn->depth, RADEON_DRIVER_NAME);
	return FALSE;
    }

    xf86PrintDepthBpp(pScrn);

    info->fifo_slots                 = 0;
    info->pix24bpp                   = xf86GetBppFromDepth(pScrn,
							   pScrn->depth);
    info->CurrentLayout.bitsPerPixel = pScrn->bitsPerPixel;
    info->CurrentLayout.depth        = pScrn->depth;
    info->CurrentLayout.pixel_bytes  = pScrn->bitsPerPixel / 8;
    info->CurrentLayout.pixel_code   = (pScrn->bitsPerPixel != 16
				       ? pScrn->bitsPerPixel
				       : pScrn->depth);

    if (info->pix24bpp == 24) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Radeon does NOT support 24bpp\n");
	return FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Pixel depth = %d bits stored in %d byte%s (%d bpp pixmaps)\n",
	       pScrn->depth,
	       info->CurrentLayout.pixel_bytes,
	       info->CurrentLayout.pixel_bytes > 1 ? "s" : "",
	       info->pix24bpp);

    if (!xf86SetDefaultVisual(pScrn, -1)) return FALSE;

    if (pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Default visual (%s) is not supported at depth %d\n",
		   xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
	return FALSE;
    }
    return TRUE;
}

/* This is called by RADEONPreInit to handle all color weight issues */
static Bool RADEONPreInitWeight(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

				/* Save flag for 6 bit DAC to use for
				   setting CRTC registers.  Otherwise use
				   an 8 bit DAC, even if xf86SetWeight sets
				   pScrn->rgbBits to some value other than
				   8. */
    info->dac6bits = FALSE;

    if (pScrn->depth > 8) {
	rgb  defaultWeight = { 0, 0, 0 };

	if (!xf86SetWeight(pScrn, defaultWeight, defaultWeight)) return FALSE;
    } else {
	pScrn->rgbBits = 8;
	if (xf86ReturnOptValBool(info->Options, OPTION_DAC_6BIT, FALSE)) {
	    pScrn->rgbBits = 6;
	    info->dac6bits = TRUE;
	}
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d bits per RGB (%d bit DAC)\n",
	       pScrn->rgbBits, info->dac6bits ? 6 : 8);

    return TRUE;
}

static void RADEONInitMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save,
				      RADEONInfoPtr info)
{
    save->mc_fb_location = info->mc_fb_location;
    save->mc_agp_location = info->mc_agp_location;
    save->display_base_addr = info->fbLocation;
    save->display2_base_addr = info->fbLocation;
    save->ov0_base_addr = info->fbLocation;
}

static void RADEONInitMemoryMap(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    unsigned long mem_size;
    CARD32 aper_size;

    /* Default to existing values */
    info->mc_fb_location = INREG(RADEON_MC_FB_LOCATION);
    info->mc_agp_location = INREG(RADEON_MC_AGP_LOCATION);

    /* We shouldn't use info->videoRam here which might have been clipped
     * but the real video RAM instead
     */
    mem_size = INREG(RADEON_CONFIG_MEMSIZE);
    aper_size = INREG(RADEON_CONFIG_APER_SIZE);
    if (mem_size == 0)
	    mem_size = 0x800000;

    /* Fix for RN50, M6, M7 with 8/16/32(??) MBs of VRAM - 
       Novell bug 204882 + along with lots of ubuntu ones */
    if (aper_size > mem_size)
	mem_size = aper_size;

#ifdef XF86DRI
    /* Apply memory map limitation if using an old DRI */
    if (info->directRenderingEnabled && !info->newMemoryMap) {
	    if (aper_size < mem_size)
		mem_size = aper_size;
    }
#endif

    /* We won't try to change MC_FB_LOCATION when using fbdev */
    if (!info->FBDev) {
	if (info->IsIGP)
	    info->mc_fb_location = INREG(RADEON_NB_TOM);
	else
#ifdef XF86DRI
	/* Old DRI has restrictions on the memory map */
	if ( info->directRenderingEnabled &&
	     info->pKernelDRMVersion->version_minor < 10 )
	    info->mc_fb_location = (mem_size - 1) & 0xffff0000U;
	else
#endif
	{
	    CARD32 aper0_base = INREG(RADEON_CONFIG_APER_0_BASE);

	    /* Recent chips have an "issue" with the memory controller, the
	     * location must be aligned to the size. We just align it down,
	     * too bad if we walk over the top of system memory, we don't
	     * use DMA without a remapped anyway.
	     * Affected chips are rv280, all r3xx, and all r4xx, but not IGP
	     */
	    if (info->ChipFamily == CHIP_FAMILY_RV280 ||
		info->ChipFamily == CHIP_FAMILY_R300 ||
		info->ChipFamily == CHIP_FAMILY_R350 ||
		info->ChipFamily == CHIP_FAMILY_RV350 ||
		info->ChipFamily == CHIP_FAMILY_RV380 ||
		info->ChipFamily == CHIP_FAMILY_R420 ||
		info->ChipFamily == CHIP_FAMILY_RV410)
		    aper0_base &= ~(mem_size - 1);

	    info->mc_fb_location = (aper0_base >> 16) |
		    ((aper0_base + mem_size - 1) & 0xffff0000U);
	}
    }
    info->fbLocation = (info->mc_fb_location & 0xffff) << 16;
   
    /* Just disable the damn AGP apertures for now, it may be
     * re-enabled later by the DRM
     */
    info->mc_agp_location = 0xffffffc0;

    RADEONTRACE(("RADEONInitMemoryMap() : \n"));
    RADEONTRACE(("  mem_size         : 0x%08lx\n", mem_size));
    RADEONTRACE(("  MC_FB_LOCATION   : 0x%08lx\n", info->mc_fb_location));
    RADEONTRACE(("  MC_AGP_LOCATION  : 0x%08lx\n", info->mc_agp_location));
}

static void RADEONGetVRamType(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32 tmp;
 
    if (info->IsIGP || (info->ChipFamily >= CHIP_FAMILY_R300) ||
	(INREG(RADEON_MEM_SDRAM_MODE_REG) & (1<<30))) 
	info->IsDDR = TRUE;
    else
	info->IsDDR = FALSE;

    tmp = INREG(RADEON_MEM_CNTL);
    if (IS_R300_VARIANT) {
	tmp &=  R300_MEM_NUM_CHANNELS_MASK;
	switch (tmp) {
	case 0: info->RamWidth = 64; break;
	case 1: info->RamWidth = 128; break;
	case 2: info->RamWidth = 256; break;
	default: info->RamWidth = 128; break;
	}
    } else if ((info->ChipFamily == CHIP_FAMILY_RV100) ||
	       (info->ChipFamily == CHIP_FAMILY_RS100) ||
	       (info->ChipFamily == CHIP_FAMILY_RS200)){
	if (tmp & RV100_HALF_MODE) info->RamWidth = 32;
	else info->RamWidth = 64;
       if (!info->HasCRTC2) {
           info->RamWidth /= 4;
           info->IsDDR = TRUE;
       }
    } else {
	if (tmp & RADEON_MEM_NUM_CHANNELS_MASK) info->RamWidth = 128;
	else info->RamWidth = 64;
    }

    /* This may not be correct, as some cards can have half of channel disabled 
     * ToDo: identify these cases
     */
}

/*
 * Depending on card genertation, chipset bugs, etc... the amount of vram
 * accessible to the CPU can vary. This function is our best shot at figuring
 * it out. Returns a value in KB.
 */
static CARD32 RADEONGetAccessibleVRAM(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32	   aper_size = INREG(RADEON_CONFIG_APER_SIZE) / 1024;

#ifdef XF86DRI
    /* If we use the DRI, we need to check if it's a version that has the
     * bug of always cropping MC_FB_LOCATION to one aperture, in which case
     * we need to limit the amount of accessible video memory
     */
    if (info->directRenderingEnabled &&
	info->pKernelDRMVersion->version_minor < 23) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "[dri] limiting video memory to one aperture of %ldK\n",
		   aper_size);
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "[dri] detected radeon kernel module version 1.%d but"
		   " 1.23 or newer is required for full memory mapping.\n",
		   info->pKernelDRMVersion->version_minor);
	info->newMemoryMap = FALSE;
	return aper_size;
    }
    info->newMemoryMap = TRUE;
#endif /* XF86DRI */

    /* Set HDP_APER_CNTL only on cards that are known not to be broken,
     * that is has the 2nd generation multifunction PCI interface
     */
    if (info->ChipFamily == CHIP_FAMILY_RV280 ||
	info->ChipFamily == CHIP_FAMILY_RV350 ||
	info->ChipFamily == CHIP_FAMILY_RV380 ||
	info->ChipFamily == CHIP_FAMILY_R420 ||
	info->ChipFamily == CHIP_FAMILY_RV410) {
	    OUTREGP (RADEON_HOST_PATH_CNTL, RADEON_HDP_APER_CNTL,
		     ~RADEON_HDP_APER_CNTL);
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Generation 2 PCI interface, using max accessible memory\n");
	    return aper_size * 2;
    }

    /* Older cards have all sorts of funny issues to deal with. First
     * check if it's a multifunction card by reading the PCI config
     * header type... Limit those to one aperture size
     */
    if (pciReadByte(info->PciTag, 0xe) & 0x80) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Generation 1 PCI interface in multifunction mode"
		   ", accessible memory limited to one aperture\n");
	return aper_size;
    }

    /* Single function older card. We read HDP_APER_CNTL to see how the BIOS
     * have set it up. We don't write this as it's broken on some ASICs but
     * we expect the BIOS to have done the right thing (might be too optimistic...)
     */
    if (INREG(RADEON_HOST_PATH_CNTL) & RADEON_HDP_APER_CNTL)
        return aper_size * 2;
    
    return aper_size;
}

static Bool RADEONPreInitVRAM(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    EntityInfoPtr  pEnt   = info->pEnt;
    GDevPtr        dev    = pEnt->device;
    unsigned char *RADEONMMIO = info->MMIO;
    MessageType    from = X_PROBED;

    if (info->FBDev)
	pScrn->videoRam      = fbdevHWGetVidmem(pScrn) / 1024;
    else if ((info->ChipFamily == CHIP_FAMILY_RS100) ||
	     (info->ChipFamily == CHIP_FAMILY_RS200) ||
	     (info->ChipFamily == CHIP_FAMILY_RS300)) {
        CARD32 tom = INREG(RADEON_NB_TOM);

	pScrn->videoRam = (((tom >> 16) -
			    (tom & 0xffff) + 1) << 6);

	OUTREG(RADEON_CONFIG_MEMSIZE, pScrn->videoRam * 1024);
    } else {
	CARD32 accessible;
	CARD32 bar_size;

	/* Read VRAM size from card */
        pScrn->videoRam      = INREG(RADEON_CONFIG_MEMSIZE) / 1024;

	/* Some production boards of m6 will return 0 if it's 8 MB */
	if (pScrn->videoRam == 0) {
	    pScrn->videoRam = 8192;
	    OUTREG(RADEON_CONFIG_MEMSIZE, 0x800000);
	}

	/* Get accessible memory */
	accessible = RADEONGetAccessibleVRAM(pScrn);

	/* Crop it to the size of the PCI BAR */
	bar_size = (1ul << info->PciInfo->size[0]) / 1024;
	if (bar_size == 0)
	    bar_size = 0x20000;
	if (accessible > bar_size)
	    accessible = bar_size;

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Detected total video RAM=%dK, accessible=%ldK "
		   "(PCI BAR=%ldK)\n",
	       pScrn->videoRam, accessible, bar_size);
	if (pScrn->videoRam > accessible)
	    pScrn->videoRam = accessible;
    }

    info->MemCntl            = INREG(RADEON_SDRAM_MODE_REG);
    info->BusCntl            = INREG(RADEON_BUS_CNTL);

    RADEONGetVRamType(pScrn);

    if (dev->videoRam) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Video RAM override, using %d kB instead of %d kB\n",
		   dev->videoRam,
		   pScrn->videoRam);
	from             = X_CONFIG;
	pScrn->videoRam  = dev->videoRam;
    }

    xf86DrvMsg(pScrn->scrnIndex, from,
	       "Mapped VideoRAM: %d kByte (%d bit %s SDRAM)\n", pScrn->videoRam, info->RamWidth, info->IsDDR?"DDR":"SDR");

    /* FIXME: For now, split FB into two equal sections. This should
     * be able to be adjusted by user with a config option. */
    if (info->IsPrimary) {
        pScrn->videoRam /= 2;
	info->MergedFB = FALSE;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
		"Using %dk of videoram for primary head\n",
		pScrn->videoRam);
    }

    if (info->IsSecondary) {  
        pScrn->videoRam /= 2;
        info->LinearAddr += pScrn->videoRam * 1024;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
		"Using %dk of videoram for secondary head\n",
		pScrn->videoRam);
    }

    pScrn->videoRam  &= ~1023;
    info->FbMapSize  = pScrn->videoRam * 1024;

    /* if the card is PCI Express reserve the last 32k for the gart table */
#ifdef XF86DRI
    if (info->cardType == CARD_PCIE && info->directRenderingEnabled)
        info->FbSecureSize = RADEON_PCIGART_TABLE_SIZE;
    else
#endif
	info->FbSecureSize = 0;

    return TRUE;
}


/* This is called by RADEONPreInit to handle config file overrides for
 * things like chipset and memory regions.  Also determine memory size
 * and type.  If memory type ever needs an override, put it in this
 * routine.
 */
static Bool RADEONPreInitChipType(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    EntityInfoPtr  pEnt   = info->pEnt;
    GDevPtr        dev    = pEnt->device;
    unsigned char *RADEONMMIO = info->MMIO;
    MessageType    from = X_PROBED;
#ifdef XF86DRI
    const char *s;
#endif

    /* Chipset */
    from = X_PROBED;
    if (dev->chipset && *dev->chipset) {
	info->Chipset  = xf86StringToToken(RADEONChipsets, dev->chipset);
	from           = X_CONFIG;
    } else if (dev->chipID >= 0) {
	info->Chipset  = dev->chipID;
	from           = X_CONFIG;
    } else {
	info->Chipset = info->PciInfo->chipType;
    }

    pScrn->chipset = (char *)xf86TokenToString(RADEONChipsets, info->Chipset);
    if (!pScrn->chipset) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "ChipID 0x%04x is not recognized\n", info->Chipset);
	return FALSE;
    }
    if (info->Chipset < 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Chipset \"%s\" is not recognized\n", pScrn->chipset);
	return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, from,
	       "Chipset: \"%s\" (ChipID = 0x%04x)\n",
	       pScrn->chipset,
	       info->Chipset);

    info->HasCRTC2 = TRUE;
    info->IsMobility = FALSE;
    info->IsIGP = FALSE;
    info->IsDellServer = FALSE;
    info->HasSingleDAC = FALSE;
    switch (info->Chipset) {
    case PCI_CHIP_RADEON_LY:
    case PCI_CHIP_RADEON_LZ:
	info->IsMobility = TRUE;
	info->ChipFamily = CHIP_FAMILY_RV100;
	break;

    case PCI_CHIP_RN50_515E:  /* RN50 is based on the RV100 but 3D isn't guaranteed to work.  YMMV. */
    case PCI_CHIP_RN50_5969:
        info->HasCRTC2 = FALSE;
    case PCI_CHIP_RV100_QY:
    case PCI_CHIP_RV100_QZ:
	info->ChipFamily = CHIP_FAMILY_RV100;

	/* DELL triple-head configuration. */
	if ((info->PciInfo->subsysVendor == PCI_VENDOR_DELL) &&
	    ((info->PciInfo->subsysCard  == 0x016c) ||
	    (info->PciInfo->subsysCard   == 0x016d) ||
	    (info->PciInfo->subsysCard   == 0x016e) ||
	    (info->PciInfo->subsysCard   == 0x016f) ||
	    (info->PciInfo->subsysCard   == 0x0170) ||
	    (info->PciInfo->subsysCard   == 0x017d) ||
	    (info->PciInfo->subsysCard   == 0x017e) ||
	    (info->PciInfo->subsysCard   == 0x0183) ||
	    (info->PciInfo->subsysCard   == 0x018a) ||
	    (info->PciInfo->subsysCard   == 0x019a))) {
	    info->IsDellServer = TRUE;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "DELL server detected, force to special setup\n");
	}

	break;

    case PCI_CHIP_RS100_4336:
	info->IsMobility = TRUE;
    case PCI_CHIP_RS100_4136:
	info->ChipFamily = CHIP_FAMILY_RS100;
	info->IsIGP = TRUE;
	break;

    case PCI_CHIP_RS200_4337:
	info->IsMobility = TRUE;
    case PCI_CHIP_RS200_4137:
	info->ChipFamily = CHIP_FAMILY_RS200;
	info->IsIGP = TRUE;
	break;

    case PCI_CHIP_RS250_4437:
	info->IsMobility = TRUE;
    case PCI_CHIP_RS250_4237:
	info->ChipFamily = CHIP_FAMILY_RS200;
	info->IsIGP = TRUE;
	break;

    case PCI_CHIP_R200_BB:
    case PCI_CHIP_R200_BC:
    case PCI_CHIP_R200_QH:
    case PCI_CHIP_R200_QL:
    case PCI_CHIP_R200_QM:
	info->ChipFamily = CHIP_FAMILY_R200;
	break;

    case PCI_CHIP_RADEON_LW:
    case PCI_CHIP_RADEON_LX:
	info->IsMobility = TRUE;
    case PCI_CHIP_RV200_QW: /* RV200 desktop */
    case PCI_CHIP_RV200_QX:
	info->ChipFamily = CHIP_FAMILY_RV200;
	break;

    case PCI_CHIP_RV250_Ld:
    case PCI_CHIP_RV250_Lf:
    case PCI_CHIP_RV250_Lg:
	info->IsMobility = TRUE;
    case PCI_CHIP_RV250_If:
    case PCI_CHIP_RV250_Ig:
	info->ChipFamily = CHIP_FAMILY_RV250;
	break;

    case PCI_CHIP_RS300_5835:
    case PCI_CHIP_RS350_7835:
	info->IsMobility = TRUE;
    case PCI_CHIP_RS300_5834:
    case PCI_CHIP_RS350_7834:
	info->ChipFamily = CHIP_FAMILY_RS300;
	info->IsIGP = TRUE;
	info->HasSingleDAC = TRUE;
	break;

    case PCI_CHIP_RV280_5C61:
    case PCI_CHIP_RV280_5C63:
	info->IsMobility = TRUE;
    case PCI_CHIP_RV280_5960:
    case PCI_CHIP_RV280_5961:
    case PCI_CHIP_RV280_5962:
    case PCI_CHIP_RV280_5964:
    case PCI_CHIP_RV280_5965:
	info->ChipFamily = CHIP_FAMILY_RV280;
	break;

    case PCI_CHIP_R300_AD:
    case PCI_CHIP_R300_AE:
    case PCI_CHIP_R300_AF:
    case PCI_CHIP_R300_AG:
    case PCI_CHIP_R300_ND:
    case PCI_CHIP_R300_NE:
    case PCI_CHIP_R300_NF:
    case PCI_CHIP_R300_NG:
	info->ChipFamily = CHIP_FAMILY_R300;
        break;

    case PCI_CHIP_RV350_NP:
    case PCI_CHIP_RV350_NQ:
    case PCI_CHIP_RV350_NR:
    case PCI_CHIP_RV350_NS:
    case PCI_CHIP_RV350_NT:
    case PCI_CHIP_RV350_NV:
	info->IsMobility = TRUE;
    case PCI_CHIP_RV350_AP:
    case PCI_CHIP_RV350_AQ:
    case PCI_CHIP_RV360_AR:
    case PCI_CHIP_RV350_AS:
    case PCI_CHIP_RV350_AT:
    case PCI_CHIP_RV350_AV:
    case PCI_CHIP_RV350_4155:
	info->ChipFamily = CHIP_FAMILY_RV350;
        break;

    case PCI_CHIP_R350_AH:
    case PCI_CHIP_R350_AI:
    case PCI_CHIP_R350_AJ:
    case PCI_CHIP_R350_AK:
    case PCI_CHIP_R350_NH:
    case PCI_CHIP_R350_NI:
    case PCI_CHIP_R350_NK:
    case PCI_CHIP_R360_NJ:
	info->ChipFamily = CHIP_FAMILY_R350;
        break;

    case PCI_CHIP_RV380_3150:
    case PCI_CHIP_RV380_3152:
    case PCI_CHIP_RV380_3154:
        info->IsMobility = TRUE;
    case PCI_CHIP_RV380_3E50:
    case PCI_CHIP_RV380_3E54:
        info->ChipFamily = CHIP_FAMILY_RV380;
        break;

    case PCI_CHIP_RV370_5460:
    case PCI_CHIP_RV370_5462:
    case PCI_CHIP_RV370_5464:
        info->IsMobility = TRUE;
    case PCI_CHIP_RV370_5B60:
    case PCI_CHIP_RV370_5B62:
    case PCI_CHIP_RV370_5B63:
    case PCI_CHIP_RV370_5B64:
    case PCI_CHIP_RV370_5B65:
        info->ChipFamily = CHIP_FAMILY_RV380;
        break;

    case PCI_CHIP_RS400_5A42:
    case PCI_CHIP_RC410_5A62:
    case PCI_CHIP_RS480_5955:
    case PCI_CHIP_RS482_5975:
        info->IsMobility = TRUE;
    case PCI_CHIP_RS400_5A41:
    case PCI_CHIP_RC410_5A61:
    case PCI_CHIP_RS480_5954:
    case PCI_CHIP_RS482_5974:
	info->ChipFamily = CHIP_FAMILY_RS400;
	info->IsIGP = TRUE;
	/*info->HasSingleDAC = TRUE;*/ /* ??? */
        break;

    case PCI_CHIP_RV410_564A:
    case PCI_CHIP_RV410_564B:
    case PCI_CHIP_RV410_564F:
    case PCI_CHIP_RV410_5652:
    case PCI_CHIP_RV410_5653:
        info->IsMobility = TRUE;
    case PCI_CHIP_RV410_5E48:
    case PCI_CHIP_RV410_5E4B:
    case PCI_CHIP_RV410_5E4A:
    case PCI_CHIP_RV410_5E4D:
    case PCI_CHIP_RV410_5E4C:
    case PCI_CHIP_RV410_5E4F:
        info->ChipFamily = CHIP_FAMILY_RV410;
        break;

    case PCI_CHIP_R420_JN:
        info->IsMobility = TRUE;
    case PCI_CHIP_R420_JH:
    case PCI_CHIP_R420_JI:
    case PCI_CHIP_R420_JJ:
    case PCI_CHIP_R420_JK:
    case PCI_CHIP_R420_JL:
    case PCI_CHIP_R420_JM:
    case PCI_CHIP_R420_JP:
    case PCI_CHIP_R420_4A4F:
        info->ChipFamily = CHIP_FAMILY_R420;
        break;

    case PCI_CHIP_R423_UH:
    case PCI_CHIP_R423_UI:
    case PCI_CHIP_R423_UJ:
    case PCI_CHIP_R423_UK:
    case PCI_CHIP_R423_UQ:
    case PCI_CHIP_R423_UR:
    case PCI_CHIP_R423_UT:
    case PCI_CHIP_R423_5D57:
    case PCI_CHIP_R423_5550:
        info->ChipFamily = CHIP_FAMILY_R420;
        break;

    case PCI_CHIP_R430_5D49:
    case PCI_CHIP_R430_5D4A:
    case PCI_CHIP_R430_5D48:
        info->IsMobility = TRUE;
    case PCI_CHIP_R430_554F:
    case PCI_CHIP_R430_554D:
    case PCI_CHIP_R430_554E:
    case PCI_CHIP_R430_554C:
        info->ChipFamily = CHIP_FAMILY_R420; /*CHIP_FAMILY_R430*/
        break;

    case PCI_CHIP_R480_5D4C:
    case PCI_CHIP_R480_5D50:
    case PCI_CHIP_R480_5D4E:
    case PCI_CHIP_R480_5D4F:
    case PCI_CHIP_R480_5D52:
    case PCI_CHIP_R480_5D4D:
    case PCI_CHIP_R481_4B4B:
    case PCI_CHIP_R481_4B4A:
    case PCI_CHIP_R481_4B49:
    case PCI_CHIP_R481_4B4C:
        info->ChipFamily = CHIP_FAMILY_R420; /*CHIP_FAMILY_R480*/
        break;

    default:
	/* Original Radeon/7200 */
	info->ChipFamily = CHIP_FAMILY_RADEON;
	info->HasCRTC2 = FALSE;
    }


    from               = X_PROBED;
    info->LinearAddr   = info->PciInfo->memBase[0] & 0xfe000000;
    pScrn->memPhysBase = info->LinearAddr;
    if (dev->MemBase) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Linear address override, using 0x%08lx instead of 0x%08lx\n",
		   dev->MemBase,
		   info->LinearAddr);
	info->LinearAddr = dev->MemBase;
	from             = X_CONFIG;
    } else if (!info->LinearAddr) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "No valid linear framebuffer address\n");
	return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, from,
	       "Linear framebuffer at 0x%08lx\n", info->LinearAddr);

				/* BIOS */
    from              = X_PROBED;
    info->BIOSAddr    = info->PciInfo->biosBase & 0xfffe0000;
    if (dev->BiosBase) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "BIOS address override, using 0x%08lx instead of 0x%08lx\n",
		   dev->BiosBase,
		   info->BIOSAddr);
	info->BIOSAddr = dev->BiosBase;
	from           = X_CONFIG;
    }
    if (info->BIOSAddr) {
	xf86DrvMsg(pScrn->scrnIndex, from,
		   "BIOS at 0x%08lx\n", info->BIOSAddr);
    }

				/* Read registers used to determine options */
    /* Check chip errata */
    info->ChipErrata = 0;

    if (info->ChipFamily == CHIP_FAMILY_R300 &&
	(INREG(RADEON_CONFIG_CNTL) & RADEON_CFG_ATI_REV_ID_MASK)
	== RADEON_CFG_ATI_REV_A11)
	    info->ChipErrata |= CHIP_ERRATA_R300_CG;

    if (info->ChipFamily == CHIP_FAMILY_RV200 ||
	info->ChipFamily == CHIP_FAMILY_RS200)
	    info->ChipErrata |= CHIP_ERRATA_PLL_DUMMYREADS;

    if (info->ChipFamily == CHIP_FAMILY_RV100 ||
	info->ChipFamily == CHIP_FAMILY_RS100 ||
	info->ChipFamily == CHIP_FAMILY_RS200)
	    info->ChipErrata |= CHIP_ERRATA_PLL_DELAY;

#ifdef XF86DRI
				/* AGP/PCI */
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

    info->cardType = CARD_PCI;

    if (pciReadLong(info->PciTag, PCI_CMD_STAT_REG) & RADEON_CAP_LIST) {
	CARD32 cap_ptr, cap_id;
	
	cap_ptr = pciReadLong(info->PciTag,
			      RADEON_CAPABILITIES_PTR_PCI_CONFIG)
	    & RADEON_CAP_PTR_MASK;

	while(cap_ptr != RADEON_CAP_ID_NULL) {
	    cap_id = pciReadLong(info->PciTag, cap_ptr);
	    if ((cap_id & 0xff)== RADEON_CAP_ID_AGP) {
		info->cardType = CARD_AGP;
		break;
	    }
	    if ((cap_id & 0xff)== RADEON_CAP_ID_EXP) {
		info->cardType = CARD_PCIE;
		break;
	    }
	    cap_ptr = (cap_id >> 8) & RADEON_CAP_PTR_MASK;
	}
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%s card detected\n",
	       (info->cardType==CARD_PCI) ? "PCI" :
		(info->cardType==CARD_PCIE) ? "PCIE" : "AGP");

    if ((s = xf86GetOptValString(info->Options, OPTION_BUS_TYPE))) {
	if (strcmp(s, "AGP") == 0) {
	    info->cardType = CARD_AGP;
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Forced into AGP mode\n");
	} else if (strcmp(s, "PCI") == 0) {
	    info->cardType = CARD_PCI;
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Forced into PCI mode\n");
	} else if (strcmp(s, "PCIE") == 0) {
	    info->cardType = CARD_PCIE;
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Forced into PCI Express mode\n");
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		       "Invalid BusType option, using detected type\n");
	}
    }
#endif
    xf86GetOptValBool(info->Options, OPTION_SHOWCACHE, &info->showCache);
    if (info->showCache)
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Option ShowCache enabled\n");

#ifdef RENDER
    info->RenderAccel = xf86ReturnOptValBool(info->Options, OPTION_RENDER_ACCEL,
					     info->Chipset != PCI_CHIP_RN50_515E &&
					     info->Chipset != PCI_CHIP_RN50_5969);
#endif

    return TRUE;
}

static void RADEONI2CGetBits(I2CBusPtr b, int *Clock, int *data)
{
    ScrnInfoPtr    pScrn      = xf86Screens[b->scrnIndex];
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned long  val;
    unsigned char *RADEONMMIO = info->MMIO;

    /* Get the result */
    val = INREG(info->DDCReg);

    *Clock = (val & RADEON_GPIO_Y_1) != 0;
    *data  = (val & RADEON_GPIO_Y_0) != 0;
}

static void RADEONI2CPutBits(I2CBusPtr b, int Clock, int data)
{
    ScrnInfoPtr    pScrn      = xf86Screens[b->scrnIndex];
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned long  val;
    unsigned char *RADEONMMIO = info->MMIO;

    val = INREG(info->DDCReg) & (CARD32)~(RADEON_GPIO_EN_0 | RADEON_GPIO_EN_1);
    val |= (Clock ? 0:RADEON_GPIO_EN_1);
    val |= (data ? 0:RADEON_GPIO_EN_0);
    OUTREG(info->DDCReg, val);

    /* read back to improve reliability on some cards. */
    val = INREG(info->DDCReg);
}

static Bool RADEONI2cInit(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    info->pI2CBus = xf86CreateI2CBusRec();
    if (!info->pI2CBus) return FALSE;

    info->pI2CBus->BusName    = "DDC";
    info->pI2CBus->scrnIndex  = pScrn->scrnIndex;
    info->pI2CBus->I2CPutBits = RADEONI2CPutBits;
    info->pI2CBus->I2CGetBits = RADEONI2CGetBits;
    info->pI2CBus->AcknTimeout = 5;

    if (!xf86I2CBusInit(info->pI2CBus)) return FALSE;
    return TRUE;
}

static void RADEONPreInitDDC(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
 /* vbeInfoPtr     pVbe; */

    info->ddc1     = FALSE;
    info->ddc_bios = FALSE;
    if (!xf86LoadSubModule(pScrn, "ddc")) {
	info->ddc2 = FALSE;
    } else {
	xf86LoaderReqSymLists(ddcSymbols, NULL);
	info->ddc2 = TRUE;
    }

    /* DDC can use I2C bus */
    /* Load I2C if we have the code to use it */
    if (info->ddc2) {
	if (xf86LoadSubModule(pScrn, "i2c")) {
	    xf86LoaderReqSymLists(i2cSymbols,NULL);
	    info->ddc2 = RADEONI2cInit(pScrn);
	}
	else info->ddc2 = FALSE;
    }
}


/* BIOS may not have right panel size, we search through all supported
 * DDC modes looking for the maximum panel size.
 */
static void RADEONUpdatePanelSize(ScrnInfoPtr pScrn)
{
    int             j;
    RADEONInfoPtr   info = RADEONPTR (pScrn);
    xf86MonPtr      ddc  = pScrn->monitor->DDC;
    DisplayModePtr  p;

    if ((info->UseBiosDividers && info->DotClock != 0) || (ddc == NULL))
       return;

    /* Go thru detailed timing table first */
    for (j = 0; j < 4; j++) {
	if (ddc->det_mon[j].type == 0) {
	    struct detailed_timings *d_timings =
		&ddc->det_mon[j].section.d_timings;
           int match = 0;

           /* If we didn't get a panel clock or guessed one, try to match the
            * mode with the panel size. We do that because we _need_ a panel
            * clock, or ValidateFPModes will fail, even when UseBiosDividers
            * is set.
            */
           if (info->DotClock == 0 &&
               info->PanelXRes == d_timings->h_active &&
               info->PanelYRes == d_timings->v_active)
               match = 1;

           /* If we don't have a BIOS provided panel data with fixed dividers,
            * check for a larger panel size
            */
	    if (info->PanelXRes < d_timings->h_active &&
               info->PanelYRes < d_timings->v_active &&
               !info->UseBiosDividers)
               match = 1;

             if (match) {
		info->PanelXRes  = d_timings->h_active;
		info->PanelYRes  = d_timings->v_active;
		info->DotClock   = d_timings->clock / 1000;
		info->HOverPlus  = d_timings->h_sync_off;
		info->HSyncWidth = d_timings->h_sync_width;
		info->HBlank     = d_timings->h_blanking;
		info->VOverPlus  = d_timings->v_sync_off;
		info->VSyncWidth = d_timings->v_sync_width;
		info->VBlank     = d_timings->v_blanking;
                info->Flags      = (d_timings->interlaced ? V_INTERLACE : 0);
                if (d_timings->sync == 3) {
                   switch (d_timings->misc) {
                   case 0: info->Flags |= V_NHSYNC | V_NVSYNC; break;
                   case 1: info->Flags |= V_PHSYNC | V_NVSYNC; break;
                   case 2: info->Flags |= V_NHSYNC | V_PVSYNC; break;
                   case 3: info->Flags |= V_PHSYNC | V_PVSYNC; break;
                   }
                }
                xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel infos found from DDC detailed: %dx%d\n",
                           info->PanelXRes, info->PanelYRes);
	    }
	}
    }

    if (info->UseBiosDividers && info->DotClock != 0)
       return;

    /* Search thru standard VESA modes from EDID */
    for (j = 0; j < 8; j++) {
	if ((info->PanelXRes < ddc->timings2[j].hsize) &&
	    (info->PanelYRes < ddc->timings2[j].vsize)) {
	    for (p = pScrn->monitor->Modes; p && p->next; p = p->next->next) {
		if ((ddc->timings2[j].hsize == p->HDisplay) &&
		    (ddc->timings2[j].vsize == p->VDisplay)) {
		    float  refresh =
			(float)p->Clock * 1000.0 / p->HTotal / p->VTotal;

		    if (abs((float)ddc->timings2[j].refresh - refresh) < 1.0) {
			/* Is this good enough? */
			info->PanelXRes  = ddc->timings2[j].hsize;
			info->PanelYRes  = ddc->timings2[j].vsize;
			info->HBlank     = p->HTotal - p->HDisplay;
			info->HOverPlus  = p->HSyncStart - p->HDisplay;
			info->HSyncWidth = p->HSyncEnd - p->HSyncStart;
			info->VBlank     = p->VTotal - p->VDisplay;
			info->VOverPlus  = p->VSyncStart - p->VDisplay;
			info->VSyncWidth = p->VSyncEnd - p->VSyncStart;
			info->DotClock   = p->Clock;
                        info->Flags      = p->Flags;
                        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel infos found from DDC VESA/EDID: %dx%d\n",
                                   info->PanelXRes, info->PanelYRes);
		    }
		}
	    }
	}
    }
}

/* This function will sort all modes according to their resolution.
 * Highest resolution first.
 */
static void RADEONSortModes(DisplayModePtr *new, DisplayModePtr *first,
			    DisplayModePtr *last)
{
    DisplayModePtr  p;

    p = *last;
    while (p) {
	if ((((*new)->HDisplay < p->HDisplay) &&
	     ((*new)->VDisplay < p->VDisplay)) ||
	    (((*new)->HDisplay == p->HDisplay) &&
	     ((*new)->VDisplay == p->VDisplay) &&
	     ((*new)->Clock < p->Clock))) {

	    if (p->next) p->next->prev = *new;
	    (*new)->prev = p;
	    (*new)->next = p->next;
	    p->next = *new;
	    if (!((*new)->next)) *last = *new;
	    break;
	}
	if (!p->prev) {
	    (*new)->prev = NULL;
	    (*new)->next = p;
	    p->prev = *new;
	    *first = *new;
	    break;
	}
	p = p->prev;
    }

    if (!*first) {
	*first = *new;
	(*new)->prev = NULL;
	(*new)->next = NULL;
	*last = *new;
    }
}

static void RADEONSetPitch (ScrnInfoPtr pScrn)
{
    int  dummy = pScrn->virtualX;
    RADEONInfoPtr info = RADEONPTR(pScrn);

    /* FIXME: May need to validate line pitch here */
    switch (pScrn->depth / 8) {
    case 1: if (info->allowColorTiling) dummy = (pScrn->virtualX + 255) & ~255;
	else dummy = (pScrn->virtualX + 127) & ~127;
	break;
    case 2: if (info->allowColorTiling) dummy = (pScrn->virtualX + 127) & ~127;
	else dummy = (pScrn->virtualX + 31) & ~31;
	break;
    case 3:
    case 4: if (info->allowColorTiling) dummy = (pScrn->virtualX + 63) & ~63;
	else dummy = (pScrn->virtualX + 15) & ~15;
	break;
    }
    pScrn->displayWidth = dummy;
}

/* When no mode provided in config file, this will add all modes supported in
 * DDC date the pScrn->modes list
 */
static DisplayModePtr RADEONDDCModes(ScrnInfoPtr pScrn)
{
    DisplayModePtr  p;
    DisplayModePtr  last  = NULL;
    DisplayModePtr  new   = NULL;
    DisplayModePtr  first = NULL;
    int             count = 0;
    int             j, tmp;
    char            stmp[32];
    xf86MonPtr      ddc   = pScrn->monitor->DDC;

    /* Go thru detailed timing table first */
    for (j = 0; j < 4; j++) {
	if (ddc->det_mon[j].type == 0) {
	    struct detailed_timings *d_timings =
		&ddc->det_mon[j].section.d_timings;

	    if (d_timings->h_active == 0 || d_timings->v_active == 0) break;

	    new = xnfcalloc(1, sizeof (DisplayModeRec));
	    memset(new, 0, sizeof (DisplayModeRec));

	    new->HDisplay   = d_timings->h_active;
	    new->VDisplay   = d_timings->v_active;

	    sprintf(stmp, "%dx%d", new->HDisplay, new->VDisplay);
	    new->name       = xnfalloc(strlen(stmp) + 1);
	    strcpy(new->name, stmp);

	    new->HTotal     = new->HDisplay + d_timings->h_blanking;
	    new->HSyncStart = new->HDisplay + d_timings->h_sync_off;
	    new->HSyncEnd   = new->HSyncStart + d_timings->h_sync_width;
	    new->VTotal     = new->VDisplay + d_timings->v_blanking;
	    new->VSyncStart = new->VDisplay + d_timings->v_sync_off;
	    new->VSyncEnd   = new->VSyncStart + d_timings->v_sync_width;
	    new->Clock      = d_timings->clock / 1000;
	    new->Flags      = (d_timings->interlaced ? V_INTERLACE : 0);
	    new->status     = MODE_OK;
	    new->type       = M_T_DEFAULT;

	    if (d_timings->sync == 3) {
		switch (d_timings->misc) {
		case 0: new->Flags |= V_NHSYNC | V_NVSYNC; break;
		case 1: new->Flags |= V_PHSYNC | V_NVSYNC; break;
		case 2: new->Flags |= V_NHSYNC | V_PVSYNC; break;
		case 3: new->Flags |= V_PHSYNC | V_PVSYNC; break;
		}
	    }
	    count++;

	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Valid Mode from Detailed timing table: %s\n",
		       new->name);

	    RADEONSortModes(&new, &first, &last);
	}
    }

    /* Search thru standard VESA modes from EDID */
    for (j = 0; j < 8; j++) {
        if (ddc->timings2[j].hsize == 0 || ddc->timings2[j].vsize == 0)
               continue;
	for (p = pScrn->monitor->Modes; p && p->next; p = p->next->next) {
	    /* Ignore all double scan modes */
	    if ((ddc->timings2[j].hsize == p->HDisplay) &&
		(ddc->timings2[j].vsize == p->VDisplay)) {
		float  refresh =
		    (float)p->Clock * 1000.0 / p->HTotal / p->VTotal;

		if (abs((float)ddc->timings2[j].refresh - refresh) < 1.0) {
		    /* Is this good enough? */
		    new = xnfcalloc(1, sizeof (DisplayModeRec));
		    memcpy(new, p, sizeof(DisplayModeRec));
		    new->name = xnfalloc(strlen(p->name) + 1);
		    strcpy(new->name, p->name);
		    new->status = MODE_OK;
		    new->type   = M_T_DEFAULT;

		    count++;

		    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			       "Valid Mode from standard timing table: %s\n",
			       new->name);

		    RADEONSortModes(&new, &first, &last);
		    break;
		}
	    }
	}
    }

    /* Search thru established modes from EDID */
    tmp = (ddc->timings1.t1 << 8) | ddc->timings1.t2;
    for (j = 0; j < 16; j++) {
	if (tmp & (1 << j)) {
	    for (p = pScrn->monitor->Modes; p && p->next; p = p->next->next) {
		if ((est_timings[j].hsize == p->HDisplay) &&
		    (est_timings[j].vsize == p->VDisplay)) {
		    float  refresh =
			(float)p->Clock * 1000.0 / p->HTotal / p->VTotal;

		    if (abs((float)est_timings[j].refresh - refresh) < 1.0) {
			/* Is this good enough? */
			new = xnfcalloc(1, sizeof (DisplayModeRec));
			memcpy(new, p, sizeof(DisplayModeRec));
			new->name = xnfalloc(strlen(p->name) + 1);
			strcpy(new->name, p->name);
			new->status = MODE_OK;
			new->type   = M_T_DEFAULT;

			count++;

			xf86DrvMsg(pScrn->scrnIndex, X_INFO,
				   "Valid Mode from established timing "
				   "table: %s\n", new->name);

			RADEONSortModes(&new, &first, &last);
			break;
		    }
		}
	    }
	}
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Total of %d mode(s) found.\n", count);

    return first;
}

/* XFree86's xf86ValidateModes routine doesn't work well with DDC modes,
 * so here is our own validation routine.
 */
static int RADEONValidateDDCModes(ScrnInfoPtr pScrn1, char **ppModeName,
				  RADEONMonitorType DisplayType, int crtc2)
{
    RADEONInfoPtr   info       = RADEONPTR(pScrn1);
    DisplayModePtr  p;
    DisplayModePtr  last       = NULL;
    DisplayModePtr  first      = NULL;
    DisplayModePtr  ddcModes   = NULL;
    int             count      = 0;
    int             i, width, height;
    ScrnInfoPtr pScrn = pScrn1;

    if (crtc2)
	pScrn = info->CRT2pScrn;

    pScrn->virtualX = pScrn1->display->virtualX;
    pScrn->virtualY = pScrn1->display->virtualY;

    if (pScrn->monitor->DDC) {
	int  maxVirtX = pScrn->virtualX;
	int  maxVirtY = pScrn->virtualY;

	/* Collect all of the DDC modes */
	first = last = ddcModes = RADEONDDCModes(pScrn);

	for (p = ddcModes; p; p = p->next) {

	    /* If primary head is a flat panel, use RMX by default */
	    if ((!info->IsSecondary && DisplayType != MT_CRT) &&
		(!info->ddc_mode) && (!crtc2)) {
		/* These values are effective values after expansion.
		 * They are not really used to set CRTC registers.
		 */
		p->HTotal     = info->PanelXRes + info->HBlank;
		p->HSyncStart = info->PanelXRes + info->HOverPlus;
		p->HSyncEnd   = p->HSyncStart + info->HSyncWidth;
		p->VTotal     = info->PanelYRes + info->VBlank;
		p->VSyncStart = info->PanelYRes + info->VOverPlus;
		p->VSyncEnd   = p->VSyncStart + info->VSyncWidth;
		p->Clock      = info->DotClock;

		p->Flags     |= RADEON_USE_RMX;
	    }

	    maxVirtX = MAX(maxVirtX, p->HDisplay);
	    maxVirtY = MAX(maxVirtY, p->VDisplay);
	    count++;

	    last = p;
	}

	/* Match up modes that are specified in the XF86Config file */
	if (ppModeName[0]) {
	    DisplayModePtr  next;

	    /* Reset the max virtual dimensions */
	    maxVirtX = pScrn->virtualX;
	    maxVirtY = pScrn->virtualY;

	    /* Reset list */
	    first = last = NULL;

	    for (i = 0; ppModeName[i]; i++) {
		/* FIXME: Use HDisplay and VDisplay instead of mode string */
		if (sscanf(ppModeName[i], "%dx%d", &width, &height) == 2) {
		    for (p = ddcModes; p; p = next) {
			next = p->next;

			if (p->HDisplay == width && p->VDisplay == height) {
			    /* We found a DDC mode that matches the one
                               requested in the XF86Config file */
			    p->type |= M_T_USERDEF;

			    /* Update  the max virtual setttings */
			    maxVirtX = MAX(maxVirtX, width);
			    maxVirtY = MAX(maxVirtY, height);

			    /* Unhook from DDC modes */
			    if (p->prev) p->prev->next = p->next;
			    if (p->next) p->next->prev = p->prev;
			    if (p == ddcModes) ddcModes = p->next;

			    /* Add to used modes */
			    if (last) {
				last->next = p;
				p->prev = last;
			    } else {
				first = p;
				p->prev = NULL;
			    }
			    p->next = NULL;
			    last = p;

			    break;
			}
		    }
		}
	    }

	    /*
	     * Add remaining DDC modes if they're smaller than the user
	     * specified modes
	     */
	    for (p = ddcModes; p; p = next) {
		next = p->next;
		if (p->HDisplay <= maxVirtX && p->VDisplay <= maxVirtY) {
		    /* Unhook from DDC modes */
		    if (p->prev) p->prev->next = p->next;
		    if (p->next) p->next->prev = p->prev;
		    if (p == ddcModes) ddcModes = p->next;

		    /* Add to used modes */
		    if (last) {
			last->next = p;
			p->prev = last;
		    } else {
			first = p;
			p->prev = NULL;
		    }
		    p->next = NULL;
		    last = p;
		}
	    }

	    /* Delete unused modes */
	    while (ddcModes)
		xf86DeleteMode(&ddcModes, ddcModes);
	} else {
	    /*
	     * No modes were configured, so we make the DDC modes
	     * available for the user to cycle through.
	     */
	    for (p = ddcModes; p; p = p->next)
		p->type |= M_T_USERDEF;
	}

        if (crtc2) {
            pScrn->virtualX = maxVirtX;
            pScrn->virtualY = maxVirtY;
	} else {
	    pScrn->virtualX = pScrn->display->virtualX = maxVirtX;
	    pScrn->virtualY = pScrn->display->virtualY = maxVirtY;
	}
    }

    /* Close the doubly-linked mode list, if we found any usable modes */
    if (last) {
	last->next   = first;
	first->prev  = last;
	pScrn->modes = first;
	RADEONSetPitch(pScrn);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Total number of valid DDC mode(s) found: %d\n", count);

    return count;
}

/* This is used only when no mode is specified for FP and no ddc is
 * available.  We force it to native mode, if possible.
 */
static DisplayModePtr RADEONFPNativeMode(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr   info  = RADEONPTR(pScrn);
    DisplayModePtr  new   = NULL;
    char            stmp[32];

    if (info->PanelXRes != 0 &&
	info->PanelYRes != 0 &&
	info->DotClock != 0) {

	/* Add native panel size */
	new             = xnfcalloc(1, sizeof (DisplayModeRec));
	sprintf(stmp, "%dx%d", info->PanelXRes, info->PanelYRes);
	new->name       = xnfalloc(strlen(stmp) + 1);
	strcpy(new->name, stmp);
	new->HDisplay   = info->PanelXRes;
	new->VDisplay   = info->PanelYRes;

	new->HTotal     = new->HDisplay + info->HBlank;
	new->HSyncStart = new->HDisplay + info->HOverPlus;
	new->HSyncEnd   = new->HSyncStart + info->HSyncWidth;
	new->VTotal     = new->VDisplay + info->VBlank;
	new->VSyncStart = new->VDisplay + info->VOverPlus;
	new->VSyncEnd   = new->VSyncStart + info->VSyncWidth;

	new->Clock      = info->DotClock;
	new->Flags      = 0;
	new->type       = M_T_USERDEF;

	new->next       = NULL;
	new->prev       = NULL;

	pScrn->display->virtualX =
	    pScrn->virtualX = MAX(pScrn->virtualX, info->PanelXRes);
	pScrn->display->virtualY =
	    pScrn->virtualY = MAX(pScrn->virtualY, info->PanelYRes);

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "No valid mode specified, force to native mode\n");
    }

    return new;
}

/* FP mode initialization routine for using on-chip RMX to scale
 */
static int RADEONValidateFPModes(ScrnInfoPtr pScrn, char **ppModeName)
{
    RADEONInfoPtr   info       = RADEONPTR(pScrn);
    DisplayModePtr  last       = NULL;
    DisplayModePtr  new        = NULL;
    DisplayModePtr  first      = NULL;
    DisplayModePtr  p, tmp;
    int             count      = 0;
    int             i, width, height;

    pScrn->virtualX = pScrn->display->virtualX;
    pScrn->virtualY = pScrn->display->virtualY;

    /* We have a flat panel connected to the primary display, and we
     * don't have any DDC info.
     */
    for (i = 0; ppModeName[i] != NULL; i++) {

	if (sscanf(ppModeName[i], "%dx%d", &width, &height) != 2) continue;

	/* Note: We allow all non-standard modes as long as they do not
	 * exceed the native resolution of the panel.  Since these modes
	 * need the internal RMX unit in the video chips (and there is
	 * only one per card), this will only apply to the primary head.
	 */
	if (width < 320 || width > info->PanelXRes ||
	    height < 200 || height > info->PanelYRes) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Mode %s is out of range.\n", ppModeName[i]);
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Valid modes must be between 320x200-%dx%d\n",
		       info->PanelXRes, info->PanelYRes);
	    continue;
	}

	new             = xnfcalloc(1, sizeof(DisplayModeRec));
	new->name       = xnfalloc(strlen(ppModeName[i]) + 1);
	strcpy(new->name, ppModeName[i]);
	new->HDisplay   = width;
	new->VDisplay   = height;

	/* These values are effective values after expansion They are
	 * not really used to set CRTC registers.
	 */
	new->HTotal     = info->PanelXRes + info->HBlank;
	new->HSyncStart = info->PanelXRes + info->HOverPlus;
	new->HSyncEnd   = new->HSyncStart + info->HSyncWidth;
	new->VTotal     = info->PanelYRes + info->VBlank;
	new->VSyncStart = info->PanelYRes + info->VOverPlus;
	new->VSyncEnd   = new->VSyncStart + info->VSyncWidth;
	new->Clock      = info->DotClock;
	new->Flags     |= RADEON_USE_RMX;

	new->type      |= M_T_USERDEF;

	new->next       = NULL;
	new->prev       = last;

	if (last) last->next = new;
	last = new;
	if (!first) first = new;

	pScrn->display->virtualX =
	    pScrn->virtualX = MAX(pScrn->virtualX, width);
	pScrn->display->virtualY =
	    pScrn->virtualY = MAX(pScrn->virtualY, height);
	count++;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Valid mode using on-chip RMX: %s\n", new->name);
    }

    /* If all else fails, add the native mode */
    if (!count) {
	first = last = RADEONFPNativeMode(pScrn);
	if (first) count = 1;
    }

    /* add in all default vesa modes smaller than panel size, used for randr*/
    for (p = pScrn->monitor->Modes; p && p->next; p = p->next->next) {
	if ((p->HDisplay <= info->PanelXRes) && (p->VDisplay <= info->PanelYRes)) {
	    tmp = first;
	    while (tmp) {
		if ((p->HDisplay == tmp->HDisplay) && (p->VDisplay == tmp->VDisplay)) break;
		tmp = tmp->next;
	    }
	    if (!tmp) {
		new             = xnfcalloc(1, sizeof(DisplayModeRec));
		new->name       = xnfalloc(strlen(p->name) + 1);
		strcpy(new->name, p->name);
		new->HDisplay   = p->HDisplay;
		new->VDisplay   = p->VDisplay;

		/* These values are effective values after expansion They are
		 * not really used to set CRTC registers.
		 */
		new->HTotal     = info->PanelXRes + info->HBlank;
		new->HSyncStart = info->PanelXRes + info->HOverPlus;
		new->HSyncEnd   = new->HSyncStart + info->HSyncWidth;
		new->VTotal     = info->PanelYRes + info->VBlank;
		new->VSyncStart = info->PanelYRes + info->VOverPlus;
		new->VSyncEnd   = new->VSyncStart + info->VSyncWidth;
		new->Clock      = info->DotClock;
		new->Flags     |= RADEON_USE_RMX;

		new->type      |= M_T_DEFAULT;

		new->next       = NULL;
		new->prev       = last;

		if (last) last->next = new;
		last = new;
		if (!first) first = new;
	    }
	}
    }

    /* Close the doubly-linked mode list, if we found any usable modes */
    if (last) {
	last->next   = first;
	first->prev  = last;
	pScrn->modes = first;
	RADEONSetPitch(pScrn);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Total number of valid FP mode(s) found: %d\n", count);

    return count;
}

/* This is called by RADEONPreInit to initialize gamma correction */
static Bool RADEONPreInitGamma(ScrnInfoPtr pScrn)
{
    Gamma  zeros = { 0.0, 0.0, 0.0 };

    if (!xf86SetGamma(pScrn, zeros)) return FALSE;
    return TRUE;
}

static void RADEONSetSyncRangeFromEdid(ScrnInfoPtr pScrn, int flag)
{
    MonPtr      mon = pScrn->monitor;
    xf86MonPtr  ddc = mon->DDC;
    int         i;

    if (flag) { /* HSync */
	for (i = 0; i < 4; i++) {
	    if (ddc->det_mon[i].type == DS_RANGES) {
		mon->nHsync = 1;
		mon->hsync[0].lo = ddc->det_mon[i].section.ranges.min_h;
		mon->hsync[0].hi = ddc->det_mon[i].section.ranges.max_h;
		return;
	    }
	}
	/* If no sync ranges detected in detailed timing table, let's
	 * try to derive them from supported VESA modes.  Are we doing
	 * too much here!!!?  */
	i = 0;
	if (ddc->timings1.t1 & 0x02) { /* 800x600@56 */
	    mon->hsync[i].lo = mon->hsync[i].hi = 35.2;
	    i++;
	}
	if (ddc->timings1.t1 & 0x04) { /* 640x480@75 */
	    mon->hsync[i].lo = mon->hsync[i].hi = 37.5;
	    i++;
	}
	if ((ddc->timings1.t1 & 0x08) || (ddc->timings1.t1 & 0x01)) {
	    mon->hsync[i].lo = mon->hsync[i].hi = 37.9;
	    i++;
	}
	if (ddc->timings1.t2 & 0x40) {
	    mon->hsync[i].lo = mon->hsync[i].hi = 46.9;
	    i++;
	}
	if ((ddc->timings1.t2 & 0x80) || (ddc->timings1.t2 & 0x08)) {
	    mon->hsync[i].lo = mon->hsync[i].hi = 48.1;
	    i++;
	}
	if (ddc->timings1.t2 & 0x04) {
	    mon->hsync[i].lo = mon->hsync[i].hi = 56.5;
	    i++;
	}
	if (ddc->timings1.t2 & 0x02) {
	    mon->hsync[i].lo = mon->hsync[i].hi = 60.0;
	    i++;
	}
	if (ddc->timings1.t2 & 0x01) {
	    mon->hsync[i].lo = mon->hsync[i].hi = 64.0;
	    i++;
	}
	mon->nHsync = i;
    } else {  /* Vrefresh */
	for (i = 0; i < 4; i++) {
	    if (ddc->det_mon[i].type == DS_RANGES) {
		mon->nVrefresh = 1;
		mon->vrefresh[0].lo = ddc->det_mon[i].section.ranges.min_v;
		mon->vrefresh[0].hi = ddc->det_mon[i].section.ranges.max_v;
		return;
	    }
	}

	i = 0;
	if (ddc->timings1.t1 & 0x02) { /* 800x600@56 */
	    mon->vrefresh[i].lo = mon->vrefresh[i].hi = 56;
	    i++;
	}
	if ((ddc->timings1.t1 & 0x01) || (ddc->timings1.t2 & 0x08)) {
	    mon->vrefresh[i].lo = mon->vrefresh[i].hi = 60;
	    i++;
	}
	if (ddc->timings1.t2 & 0x04) {
	    mon->vrefresh[i].lo = mon->vrefresh[i].hi = 70;
	    i++;
	}
	if ((ddc->timings1.t1 & 0x08) || (ddc->timings1.t2 & 0x80)) {
	    mon->vrefresh[i].lo = mon->vrefresh[i].hi = 72;
	    i++;
	}
	if ((ddc->timings1.t1 & 0x04) || (ddc->timings1.t2 & 0x40) ||
	    (ddc->timings1.t2 & 0x02) || (ddc->timings1.t2 & 0x01)) {
	    mon->vrefresh[i].lo = mon->vrefresh[i].hi = 75;
	    i++;
	}
	mon->nVrefresh = i;
    }
}

static int RADEONValidateMergeModes(ScrnInfoPtr pScrn1)
{
    RADEONInfoPtr   info             = RADEONPTR(pScrn1);
    ClockRangePtr   clockRanges;
    int             modesFound;
    ScrnInfoPtr pScrn = info->CRT2pScrn;

    /* fill in pScrn2 */
    pScrn->videoRam = pScrn1->videoRam;
    pScrn->depth = pScrn1->depth;
    pScrn->numClocks = pScrn1->numClocks;
    pScrn->progClock = pScrn1->progClock;
    pScrn->fbFormat = pScrn1->fbFormat;
    pScrn->videoRam = pScrn1->videoRam;
    pScrn->maxHValue = pScrn1->maxHValue;
    pScrn->maxVValue = pScrn1->maxVValue;
    pScrn->xInc = pScrn1->xInc;

    if (info->NoVirtual) {
	pScrn1->display->virtualX = 0;
        pScrn1->display->virtualY = 0;
    }

    if (pScrn->monitor->DDC) {
        /* If we still don't know sync range yet, let's try EDID.
         *
         * Note that, since we can have dual heads, Xconfigurator
         * may not be able to probe both monitors correctly through
         * vbe probe function (RADEONProbeDDC). Here we provide an
         * additional way to auto-detect sync ranges if they haven't
         * been added to XF86Config manually.
         */
        if (pScrn->monitor->nHsync <= 0)
            RADEONSetSyncRangeFromEdid(pScrn, 1);
        if (pScrn->monitor->nVrefresh <= 0)
            RADEONSetSyncRangeFromEdid(pScrn, 0);
    }

    /* Get mode information */
    pScrn->progClock               = TRUE;
    clockRanges                    = xnfcalloc(sizeof(*clockRanges), 1);
    clockRanges->next              = NULL;
    clockRanges->minClock          = info->pll.min_pll_freq;
    clockRanges->maxClock          = info->pll.max_pll_freq * 10;
    clockRanges->clockIndex        = -1;
    clockRanges->interlaceAllowed  = (info->MergeType == MT_CRT);
    clockRanges->doubleScanAllowed = (info->MergeType == MT_CRT);

    /* We'll use our own mode validation routine for DFP/LCD, since
     * xf86ValidateModes does not work correctly with the DFP/LCD modes
     * 'stretched' from their native mode.
     */
    if (info->MergeType == MT_CRT && !info->ddc_mode) {
 
	modesFound =
	    xf86ValidateModes(pScrn,
			      pScrn->monitor->Modes,
			      pScrn1->display->modes,
			      clockRanges,
			      NULL,                  /* linePitches */
			      8 * 64,                /* minPitch */
			      8 * 1024,              /* maxPitch */
			      info->allowColorTiling ? 2048 :
			          64 * pScrn1->bitsPerPixel, /* pitchInc */
			      128,                   /* minHeight */
			      info->MaxLines,        /* maxHeight */
			      pScrn1->display->virtualX ? pScrn1->virtualX : 0,
			      pScrn1->display->virtualY ? pScrn1->virtualY : 0,
			      info->FbMapSize,
			      LOOKUP_BEST_REFRESH);

	if (modesFound == -1) return 0;

	xf86PruneDriverModes(pScrn);
	if (!modesFound || !pScrn->modes) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
	    return 0;
	}

    } else {
	/* First, free any allocated modes during configuration, since
	 * we don't need them
	 */
	while (pScrn->modes)
	    xf86DeleteMode(&pScrn->modes, pScrn->modes);
	while (pScrn->modePool)
	    xf86DeleteMode(&pScrn->modePool, pScrn->modePool);

	/* Next try to add DDC modes */
	modesFound = RADEONValidateDDCModes(pScrn, pScrn1->display->modes,
					    info->MergeType, 1);

	/* If that fails and we're connect to a flat panel, then try to
         * add the flat panel modes
	 */
	if (info->MergeType != MT_CRT) {
	    
	    /* some panels have DDC, but don't have internal scaler.
	     * in this case, we need to validate additional modes
	     * by using on-chip RMX.
	     */
	    int user_modes_asked = 0, user_modes_found = 0, i;
	    DisplayModePtr  tmp_mode = pScrn->modes;
	    while (pScrn1->display->modes[user_modes_asked]) user_modes_asked++;	    
	    if (tmp_mode) {
		for (i = 0; i < modesFound; i++) {
		    if (tmp_mode->type & M_T_USERDEF) user_modes_found++;
		    tmp_mode = tmp_mode->next;
		}
	    }

 	    if ((modesFound <= 1) || (user_modes_found < user_modes_asked)) {
		/* when panel size is not valid, try to validate 
		 * mode using xf86ValidateModes routine
		 * This can happen when DDC is disabled.
		 */
		/* if (info->PanelXRes < 320 || info->PanelYRes < 200) */
		    modesFound =
			xf86ValidateModes(pScrn,
					  pScrn->monitor->Modes,
					  pScrn1->display->modes,
					  clockRanges,
					  NULL,                  /* linePitches */
					  8 * 64,                /* minPitch */
					  8 * 1024,              /* maxPitch */
					  info->allowColorTiling ? 2048 :
					      64 * pScrn1->bitsPerPixel, /* pitchInc */
					  128,                   /* minHeight */
					  info->MaxLines,        /* maxHeight */
					  pScrn1->display->virtualX,
					  pScrn1->display->virtualY,
					  info->FbMapSize,
					  LOOKUP_BEST_REFRESH);

	    } 
        }

	/* Setup the screen's clockRanges for the VidMode extension */
	if (!pScrn->clockRanges) {
	    pScrn->clockRanges = xnfcalloc(sizeof(*(pScrn->clockRanges)), 1);
	    memcpy(pScrn->clockRanges, clockRanges, sizeof(*clockRanges));
	    pScrn->clockRanges->strategy = LOOKUP_BEST_REFRESH;
	}

	/* Fail if we still don't have any valid modes */
	if (modesFound < 1) {
	    if (info->MergeType == MT_CRT) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "No valid DDC modes found for this CRT\n");
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Try turning off the \"DDCMode\" option\n");
	    } else {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "No valid mode found for this DFP/LCD\n");
	    }
	    return 0;
	}
    }
    return modesFound;
}


/* This is called by RADEONPreInit to validate modes and compute
 * parameters for all of the valid modes.
 */
static Bool RADEONPreInitModes(ScrnInfoPtr pScrn, xf86Int10InfoPtr pInt10)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    ClockRangePtr  clockRanges;
    int            modesFound;
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    char           *s;

    /* This option has two purposes:
     *
     * 1. For CRT, if this option is on, xf86ValidateModes (to
     *    LOOKUP_BEST_REFRESH) is not going to be used for mode
     *    validation.  Instead, we'll validate modes by matching exactly
     *    the modes supported from the DDC data.  This option can be
     *    used (a) to enable non-standard modes listed in the Detailed
     *    Timings block of EDID, like 2048x1536 (not included in
     *    xf86DefModes), (b) to avoid unstable modes for some flat
     *    panels working in analog mode (some modes validated by
     *    xf86ValidateModes don't really work with these panels).
     *
     * 2. For DFP on primary head, with this option on, the validation
     *    routine will try to use supported modes from DDC data first
     *    before trying on-chip RMX streching.  By default, native mode
     *    + RMX streching is used for all non-native modes, it appears
     *    more reliable. Some non-native modes listed in the DDC data
     *    may not work properly if they are used directly. This seems to
     *    only happen to a few panels (haven't nailed this down yet, it
     *    may related to the incorrect setting in TMDS_PLL_CNTL when
     *    pixel clock is changed).  Use this option may give you better
     *    refresh rate for some non-native modes.  The 2nd DVI port will
     *    always use DDC modes directly (only have one on-chip RMX
     *    unit).
     *
     * Note: This option will be dismissed if no DDC data is available.
     */

    if (info->MergedFB) {
	if (!(pScrn->display->virtualX))
	    info->NoVirtual = TRUE;
	else
	    info->NoVirtual = FALSE;
    }

    info->ddc_mode =
	xf86ReturnOptValBool(info->Options, OPTION_DDC_MODE, FALSE);

    /* don't use RMX if we have a dual-tmds panels */
   if (pRADEONEnt->MonType2 == MT_DFP)
	info->ddc_mode = TRUE;
   /* don't use RMX if we are Dell Server */  
   if (info->IsDellServer)
   {
       info->ddc_mode = TRUE;
   }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Validating modes on %s head ---------\n",
	       info->IsSecondary ? "Secondary" : "Primary");

    if (info->IsSecondary)
        pScrn->monitor->DDC = pRADEONEnt->MonInfo2;
    else
        pScrn->monitor->DDC = pRADEONEnt->MonInfo1;

    if (!pScrn->monitor->DDC && info->ddc_mode) {
	info->ddc_mode = FALSE;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "No DDC data available, DDCMode option is dismissed\n");
    }

    if ((info->DisplayType == MT_DFP) ||
	(info->DisplayType == MT_LCD)) {
	if ((s = xf86GetOptValString(info->Options, OPTION_PANEL_SIZE))) {
	    int PanelX, PanelY;
	    DisplayModePtr  tmp_mode         = NULL;
	    if (sscanf(s, "%dx%d", &PanelX, &PanelY) == 2) {
		info->PanelXRes = PanelX;
		info->PanelYRes = PanelY;
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			   "Panel size is forced to: %s\n", s);

		/* We can't trust BIOS or DDC timings anymore, 
		   Use whatever specified in the Modeline.
		   If no Modeline specified, we'll just pick the VESA mode at 
		   60Hz refresh rate which is likely to be the best for a flat panel.
		*/ 
		info->ddc_mode = FALSE;
		pScrn->monitor->DDC = NULL;
		tmp_mode = pScrn->monitor->Modes;
		while(tmp_mode) {
		    if ((tmp_mode->HDisplay == PanelX) && 
			(tmp_mode->VDisplay == PanelY)) {

			float  refresh =
			    (float)tmp_mode->Clock * 1000.0 / tmp_mode->HTotal / tmp_mode->VTotal;
			if ((abs(60.0 - refresh) < 1.0) ||
			    (tmp_mode->type == 0)) {
			    info->HBlank     = tmp_mode->HTotal - tmp_mode->HDisplay;
			    info->HOverPlus  = tmp_mode->HSyncStart - tmp_mode->HDisplay;
			    info->HSyncWidth = tmp_mode->HSyncEnd - tmp_mode->HSyncStart;
			    info->VBlank     = tmp_mode->VTotal - tmp_mode->VDisplay;
			    info->VOverPlus  = tmp_mode->VSyncStart - tmp_mode->VDisplay;
			    info->VSyncWidth = tmp_mode->VSyncEnd - tmp_mode->VSyncStart;
			    info->DotClock   = tmp_mode->Clock;
			    info->Flags = 0;
			    break;
			}
		    }
		    tmp_mode = tmp_mode->next;
		}
		if (info->DotClock == 0) {
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			       "No valid timing info for specified panel size.\n"
			       "Please specify the Modeline for this panel\n");
		    return FALSE;
		}
	    } else {
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			   "Invalid PanelSize value: %s\n", s);
	    }
        } else
            RADEONGetPanelInfo(pScrn);
    }

    if (pScrn->monitor->DDC) {
        /* If we still don't know sync range yet, let's try EDID.
         *
         * Note that, since we can have dual heads, Xconfigurator
         * may not be able to probe both monitors correctly through
         * vbe probe function (RADEONProbeDDC). Here we provide an
         * additional way to auto-detect sync ranges if they haven't
         * been added to XF86Config manually.
         */
        if (pScrn->monitor->nHsync <= 0)
            RADEONSetSyncRangeFromEdid(pScrn, 1);
        if (pScrn->monitor->nVrefresh <= 0)
            RADEONSetSyncRangeFromEdid(pScrn, 0);
    }

    /* Get mode information */
    pScrn->progClock               = TRUE;
    clockRanges                    = xnfcalloc(sizeof(*clockRanges), 1);
    clockRanges->next              = NULL;
    clockRanges->minClock          = info->pll.min_pll_freq;
    clockRanges->maxClock          = info->pll.max_pll_freq * 10;
    clockRanges->clockIndex        = -1;
    clockRanges->interlaceAllowed  = (info->DisplayType == MT_CRT);
    clockRanges->doubleScanAllowed = (info->DisplayType == MT_CRT);

    /* We'll use our own mode validation routine for DFP/LCD, since
     * xf86ValidateModes does not work correctly with the DFP/LCD modes
     * 'stretched' from their native mode.
     */
    if (info->DisplayType == MT_CRT && !info->ddc_mode) {

	modesFound =
	    xf86ValidateModes(pScrn,
			      pScrn->monitor->Modes,
			      pScrn->display->modes,
			      clockRanges,
			      NULL,                  /* linePitches */
			      8 * 64,                /* minPitch */
			      8 * 1024,              /* maxPitch */
			      info->allowColorTiling ? 2048 :
			          64 * pScrn->bitsPerPixel, /* pitchInc */
			      128,                   /* minHeight */
			      info->MaxLines,        /* maxHeight */
			      pScrn->display->virtualX,
			      pScrn->display->virtualY,
			      info->FbMapSize,
			      LOOKUP_BEST_REFRESH);

	if (modesFound < 1 && info->FBDev) {
	    fbdevHWUseBuildinMode(pScrn);
	    pScrn->displayWidth = fbdevHWGetLineLength(pScrn)
				/ info->CurrentLayout.pixel_bytes;
	    modesFound = 1;
	}

	if (modesFound == -1) return FALSE;

	xf86PruneDriverModes(pScrn);
	if (!modesFound || !pScrn->modes) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
	    return FALSE;
	}

    } else {
	/* First, free any allocated modes during configuration, since
	 * we don't need them
	 */
	while (pScrn->modes)
	    xf86DeleteMode(&pScrn->modes, pScrn->modes);
	while (pScrn->modePool)
	    xf86DeleteMode(&pScrn->modePool, pScrn->modePool);

	/* Next try to add DDC modes */
	modesFound = RADEONValidateDDCModes(pScrn, pScrn->display->modes,
					    info->DisplayType, 0);

	/* If that fails and we're connect to a flat panel, then try to
         * add the flat panel modes
	 */
	if (info->DisplayType != MT_CRT) {

	    /* some panels have DDC, but don't have internal scaler.
	     * in this case, we need to validate additional modes
	     * by using on-chip RMX.
	     */
	    int user_modes_asked = 0, user_modes_found = 0, i;
	    DisplayModePtr  tmp_mode = pScrn->modes;
	    while (pScrn->display->modes[user_modes_asked]) user_modes_asked++;
	    if (tmp_mode) {
		for (i = 0; i < modesFound; i++) {
		    if (tmp_mode->type & M_T_USERDEF) user_modes_found++;
		    tmp_mode = tmp_mode->next;
		}
	    }

	    if ((modesFound <= 1) || (user_modes_found < user_modes_asked)) {
		/* when panel size is not valid, try to validate
		 * mode using xf86ValidateModes routine
		 * This can happen when DDC is disabled.
		 */
		if (info->PanelXRes < 320 || info->PanelYRes < 200)
		    modesFound =
			xf86ValidateModes(pScrn,
					  pScrn->monitor->Modes,
					  pScrn->display->modes,
					  clockRanges,
					  NULL,                  /* linePitches */
					  8 * 64,                /* minPitch */
					  8 * 1024,              /* maxPitch */
					  info->allowColorTiling ? 2048 :
					      64 * pScrn->bitsPerPixel, /* pitchInc */
					  128,                   /* minHeight */
					  info->MaxLines,        /* maxHeight */
					  pScrn->display->virtualX,
					  pScrn->display->virtualY,
					  info->FbMapSize,
					  LOOKUP_BEST_REFRESH);
		else if (!info->IsSecondary)
		    modesFound = RADEONValidateFPModes(pScrn, pScrn->display->modes);
	    }
        }

	/* Setup the screen's clockRanges for the VidMode extension */
	if (!pScrn->clockRanges) {
	    pScrn->clockRanges = xnfcalloc(sizeof(*(pScrn->clockRanges)), 1);
	    memcpy(pScrn->clockRanges, clockRanges, sizeof(*clockRanges));
	    pScrn->clockRanges->strategy = LOOKUP_BEST_REFRESH;
	}

	/* Fail if we still don't have any valid modes */
	if (modesFound < 1) {
	    if (info->DisplayType == MT_CRT) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "No valid DDC modes found for this CRT\n");
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Try turning off the \"DDCMode\" option\n");
	    } else {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "No valid mode found for this DFP/LCD\n");
	    }
	    return FALSE;
	}
    }

    xf86SetCrtcForModes(pScrn, 0);

    if (info->HasCRTC2) {
	if (info->MergedFB) {

	    /* If we have 2 screens from the config file, we don't need
	     * to do clone thing, let each screen handles one head.
	     */
	    if (!pRADEONEnt->HasSecondary) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "Validating CRTC2 modes for MergedFB ------------ \n");

		modesFound = RADEONValidateMergeModes(pScrn);
		if (modesFound < 1) {
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			       "No valid mode found for CRTC2, disabling MergedFB\n");
		    info->MergedFB = FALSE;
		}
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "Total of %d CRTC2 modes found for MergedFB------------ \n",
			   modesFound);
	    }
	}
    }

    pScrn->currentMode = pScrn->modes;
    if(info->MergedFB) {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
       	   "Modes for CRT1: ********************\n");
    }
    xf86PrintModes(pScrn);

    if(info->MergedFB) {

       xf86SetCrtcForModes(info->CRT2pScrn, INTERLACE_HALVE_V);

       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
           "Modes for CRT2: ********************\n");

       xf86PrintModes(info->CRT2pScrn);

       info->CRT1Modes = pScrn->modes;
       info->CRT1CurrentMode = pScrn->currentMode;

       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Generating MergedFB mode list\n");

       if (info->NoVirtual) {
           pScrn->display->virtualX = 0;
           pScrn->display->virtualY = 0;
       }
       pScrn->modes = RADEONGenerateModeList(pScrn, info->MetaModes,
	            	                  info->CRT1Modes, info->CRT2pScrn->modes,
					  info->CRT2Position);

       if(!pScrn->modes) {

	  xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	      "Failed to parse MetaModes or no modes found. MergeFB mode disabled.\n");
	  if(info->CRT2pScrn) {
	     if(info->CRT2pScrn->modes) {
	        while(info->CRT2pScrn->modes)
		   xf86DeleteMode(&info->CRT2pScrn->modes, info->CRT2pScrn->modes);
	     }
	     if(info->CRT2pScrn->monitor) {
	        if(info->CRT2pScrn->monitor->Modes) {
	           while(info->CRT2pScrn->monitor->Modes)
		      xf86DeleteMode(&info->CRT2pScrn->monitor->Modes, info->CRT2pScrn->monitor->Modes);
	        }
		if(info->CRT2pScrn->monitor->DDC) xfree(info->CRT2pScrn->monitor->DDC);
	        xfree(info->CRT2pScrn->monitor);
	     }
             xfree(info->CRT2pScrn);
	     info->CRT2pScrn = NULL;
	  }
	  pScrn->modes = info->CRT1Modes;
	  info->CRT1Modes = NULL;
	  info->MergedFB = FALSE;

       }
    }

    if (info->MergedFB) {
       /* If no virtual dimension was given by the user,
        * calculate a sane one now. Adapts pScrn->virtualX,
	* pScrn->virtualY and pScrn->displayWidth.
	*/
       RADEONRecalcDefaultVirtualSize(pScrn);
       info->CRT2pScrn->virtualX = pScrn->virtualX;
       info->CRT2pScrn->virtualY = pScrn->virtualY;
       RADEONSetPitch(pScrn);
       RADEONSetPitch(info->CRT2pScrn);

       pScrn->modes = pScrn->modes->next;  /* We get the last from GenerateModeList() */
       pScrn->currentMode = pScrn->modes;

       /* Update CurrentLayout */
       info->CurrentLayout.mode = pScrn->currentMode;
       info->CurrentLayout.displayWidth = pScrn->displayWidth;
    }

				/* Set DPI */
    /* xf86SetDpi(pScrn, 0, 0); */

    if (info->MergedFB) {
	RADEONMergedFBSetDpi(pScrn, info->CRT2pScrn, info->CRT2Position);
    } else {
	xf86SetDpi(pScrn, 0, 0);
        info->RADEONDPIVX = pScrn->virtualX;
        info->RADEONDPIVY = pScrn->virtualY;
    }

				/* Get ScreenInit function */
    if (!xf86LoadSubModule(pScrn, "fb")) return FALSE;

    xf86LoaderReqSymLists(fbSymbols, NULL);

    info->CurrentLayout.displayWidth = pScrn->displayWidth;
    info->CurrentLayout.mode = pScrn->currentMode;

    return TRUE;
}

/* This is called by RADEONPreInit to initialize the hardware cursor */
static Bool RADEONPreInitCursor(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (!xf86ReturnOptValBool(info->Options, OPTION_SW_CURSOR, FALSE)) {
	if (!xf86LoadSubModule(pScrn, "ramdac")) return FALSE;
	xf86LoaderReqSymLists(ramdacSymbols, NULL);
    }
    return TRUE;
}

/* This is called by RADEONPreInit to initialize hardware acceleration */
static Bool RADEONPreInitAccel(ScrnInfoPtr pScrn)
{
#ifdef XFree86LOADER
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    MessageType from;
#if defined(USE_EXA) && defined(USE_XAA)
    char *optstr;
#endif

    info->useEXA = FALSE;

    if (!xf86ReturnOptValBool(info->Options, OPTION_NOACCEL, FALSE)) {
	int errmaj = 0, errmin = 0;

	from = X_DEFAULT;
#if defined(USE_EXA)
#if defined(USE_XAA)
	optstr = (char *)xf86GetOptValString(info->Options, OPTION_ACCELMETHOD);
	if (optstr != NULL) {
	    if (xf86NameCmp(optstr, "EXA") == 0) {
		from = X_CONFIG;
		info->useEXA = TRUE;
	    } else if (xf86NameCmp(optstr, "XAA") == 0) {
		from = X_CONFIG;
	    }
	}
#else /* USE_XAA */
	info->useEXA = TRUE;
#endif /* !USE_XAA */
#endif /* USE_EXA */
	xf86DrvMsg(pScrn->scrnIndex, from,
	    "Using %s acceleration architecture\n",
	    info->useEXA ? "EXA" : "XAA");

#ifdef USE_EXA
	if (info->useEXA) {
	    info->exaReq.majorversion = 2;
	    info->exaReq.minorversion = 0;

	    if (!LoadSubModule(pScrn->module, "exa", NULL, NULL, NULL,
			       &info->exaReq, &errmaj, &errmin)) {
		LoaderErrorMsg(NULL, "exa", errmaj, errmin);
		return FALSE;
	    }
	    xf86LoaderReqSymLists(exaSymbols, NULL);
	}
#endif /* USE_EXA */
#ifdef USE_XAA
	if (!info->useEXA) {
	    info->xaaReq.majorversion = 1;
	    info->xaaReq.minorversion = 2;

	    if (!LoadSubModule(pScrn->module, "xaa", NULL, NULL, NULL,
			   &info->xaaReq, &errmaj, &errmin)) {
		info->xaaReq.minorversion = 1;

		if (!LoadSubModule(pScrn->module, "xaa", NULL, NULL, NULL,
			       &info->xaaReq, &errmaj, &errmin)) {
		    info->xaaReq.minorversion = 0;

		    if (!LoadSubModule(pScrn->module, "xaa", NULL, NULL, NULL,
			       &info->xaaReq, &errmaj, &errmin)) {
			LoaderErrorMsg(NULL, "xaa", errmaj, errmin);
			return FALSE;
		    }
		}
	    }
	    xf86LoaderReqSymLists(xaaSymbols, NULL);
	}
#endif /* USE_XAA */
    }
#endif /* XFree86Loader */

    return TRUE;
}

static Bool RADEONPreInitInt10(ScrnInfoPtr pScrn, xf86Int10InfoPtr *ppInt10)
{
#if !defined(__powerpc__)
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32 fp2_gen_ctl_save   = 0;

    if (xf86LoadSubModule(pScrn, "int10")) {
	xf86LoaderReqSymLists(int10Symbols, NULL);

	/* The VGA BIOS on the RV100/QY cannot be read when the digital output
	 * is enabled.  Clear and restore FP2_ON around int10 to avoid this.
	 */
	if (info->PciInfo->chipType == PCI_CHIP_RV100_QY) {
	    fp2_gen_ctl_save = INREG(RADEON_FP2_GEN_CNTL);
	    if (fp2_gen_ctl_save & RADEON_FP2_ON) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "disabling digital out\n");
		OUTREG(RADEON_FP2_GEN_CNTL, fp2_gen_ctl_save & ~RADEON_FP2_ON);
	    }
	}
	
	xf86DrvMsg(pScrn->scrnIndex,X_INFO,"initializing int10\n");
	*ppInt10 = xf86InitInt10(info->pEnt->index);

	if (fp2_gen_ctl_save & RADEON_FP2_ON) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "re-enabling digital out\n");
	    OUTREG(RADEON_FP2_GEN_CNTL, fp2_gen_ctl_save);	
	}
    }
#endif
    return TRUE;
}

#ifdef XF86DRI
static Bool RADEONPreInitDRI(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    MessageType    from;

    info->directRenderingEnabled = FALSE;
    info->directRenderingInited = FALSE;
    info->CPInUse = FALSE;
    info->CPStarted = FALSE;
    info->pLibDRMVersion = NULL;
    info->pKernelDRMVersion = NULL;

    if (xf86IsEntityShared(info->pEnt->index)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Direct Rendering Disabled -- "
		   "Dual-head configuration is not working with "
		   "DRI at present.\n"
		   "Please use the radeon MergedFB option if you "
		   "want Dual-head with DRI.\n");
	return FALSE;
    }
    if (info->IsSecondary)
        return FALSE;

    if (info->Chipset == PCI_CHIP_RN50_515E ||
	info->Chipset == PCI_CHIP_RN50_5969) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Direct rendering not supported on RN50\n");
	return FALSE;
    }

    if (info->Chipset == PCI_CHIP_RS400_5A41 ||
	info->Chipset == PCI_CHIP_RS400_5A42 ||
	info->Chipset == PCI_CHIP_RC410_5A61 ||
	info->Chipset == PCI_CHIP_RC410_5A62 ||
	info->Chipset == PCI_CHIP_RS480_5954 ||
	info->Chipset == PCI_CHIP_RS480_5955 ||
	info->Chipset == PCI_CHIP_RS482_5974 ||
	info->Chipset == PCI_CHIP_RS482_5975) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Direct rendering broken on XPRESS 200 and 200M\n");
	return FALSE;
    }

    if (xf86ReturnOptValBool(info->Options, OPTION_NOACCEL, FALSE)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "[dri] Acceleration disabled, not initializing the DRI\n");
	return FALSE;
    }

    if (!RADEONDRIGetVersion(pScrn))
	return FALSE;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "[dri] Found DRI library version %d.%d.%d and kernel"
	       " module version %d.%d.%d\n",
	       info->pLibDRMVersion->version_major,
	       info->pLibDRMVersion->version_minor,
	       info->pLibDRMVersion->version_patchlevel,
	       info->pKernelDRMVersion->version_major,
	       info->pKernelDRMVersion->version_minor,
	       info->pKernelDRMVersion->version_patchlevel);

    if (xf86ReturnOptValBool(info->Options, OPTION_CP_PIO, FALSE)) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Forcing CP into PIO mode\n");
	info->CPMode = RADEON_DEFAULT_CP_PIO_MODE;
    } else {
	info->CPMode = RADEON_DEFAULT_CP_BM_MODE;
    }

    info->agpMode       = RADEON_DEFAULT_AGP_MODE;
    info->gartSize      = RADEON_DEFAULT_GART_SIZE;
    info->ringSize      = RADEON_DEFAULT_RING_SIZE;
    info->bufSize       = RADEON_DEFAULT_BUFFER_SIZE;
    info->gartTexSize   = RADEON_DEFAULT_GART_TEX_SIZE;
    info->agpFastWrite  = RADEON_DEFAULT_AGP_FAST_WRITE;

    info->CPusecTimeout = RADEON_DEFAULT_CP_TIMEOUT;

    if (info->cardType==CARD_AGP) {
	if (xf86GetOptValInteger(info->Options,
				 OPTION_AGP_MODE, &(info->agpMode))) {
	    if (info->agpMode < 1 || info->agpMode > RADEON_AGP_MAX_MODE) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Illegal AGP Mode: %dx, set to default %dx mode\n",
			   info->agpMode, RADEON_DEFAULT_AGP_MODE);
		info->agpMode = RADEON_DEFAULT_AGP_MODE;
	    }

	    /* AGP_MAX_MODE is changed to allow v3 8x mode.
	     * At this time we don't know if the AGP bridge supports
	     * 8x mode. This will later be verified on both 
	     * AGP master and target sides.
	     */
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		       "AGP %dx mode is configured\n", info->agpMode);
	}

	if ((info->agpFastWrite = xf86ReturnOptValBool(info->Options,
						       OPTION_AGP_FW,
						       FALSE))) {
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		       "Enabling AGP Fast Write\n");
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "AGP Fast Write disabled by default\n");
	}
    }

    if ((xf86GetOptValInteger(info->Options,
			     OPTION_GART_SIZE, (int *)&(info->gartSize))) ||
			     (xf86GetOptValInteger(info->Options,
			     OPTION_GART_SIZE_OLD, (int *)&(info->gartSize)))) {
	switch (info->gartSize) {
	case 4:
	case 8:
	case 16:
	case 32:
	case 64:
	case 128:
	case 256:
	    break;

	default:
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Illegal GART size: %d MB\n", info->gartSize);
	    return FALSE;
	}
    }

    if (xf86GetOptValInteger(info->Options,
			     OPTION_RING_SIZE, &(info->ringSize))) {
	if (info->ringSize < 1 || info->ringSize >= (int)info->gartSize) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Illegal ring buffer size: %d MB\n",
		       info->ringSize);
	    return FALSE;
	}
    }

    if (xf86GetOptValInteger(info->Options,
			     OPTION_BUFFER_SIZE, &(info->bufSize))) {
	if (info->bufSize < 1 || info->bufSize >= (int)info->gartSize) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Illegal vertex/indirect buffers size: %d MB\n",
		       info->bufSize);
	    return FALSE;
	}
	if (info->bufSize > 2) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Illegal vertex/indirect buffers size: %d MB\n",
		       info->bufSize);
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Clamping vertex/indirect buffers size to 2 MB\n");
	    info->bufSize = 2;
	}
    }

    if (info->ringSize + info->bufSize + info->gartTexSize >
	(int)info->gartSize) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Buffers are too big for requested GART space\n");
	return FALSE;
    }

    info->gartTexSize = info->gartSize - (info->ringSize + info->bufSize);

    if (xf86GetOptValInteger(info->Options, OPTION_USEC_TIMEOUT,
			     &(info->CPusecTimeout))) {
	/* This option checked by the RADEON DRM kernel module */
    }

    /* Two options to try and squeeze as much texture memory as possible
     * for dedicated 3d rendering boxes
     */
    info->noBackBuffer = xf86ReturnOptValBool(info->Options,
					      OPTION_NO_BACKBUFFER,
					      FALSE);

#ifdef XF86DRI
    if (info->noBackBuffer) {
	info->allowPageFlip = 0;
    } else if (!xf86LoadSubModule(pScrn, "shadowfb")) {
	info->allowPageFlip = 0;
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Couldn't load shadowfb module:\n");
    } else {
	xf86LoaderReqSymLists(driShadowFBSymbols, NULL);

	info->allowPageFlip = xf86ReturnOptValBool(info->Options,
						   OPTION_PAGE_FLIP,
						   FALSE);
	if (info->allowPageFlip && info->useEXA) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Page flipping not allowed with EXA, disabling.\n");
	    info->allowPageFlip = FALSE;
	}
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Page flipping %sabled\n",
	       info->allowPageFlip ? "en" : "dis");
#endif

    info->DMAForXv = TRUE;
    from = xf86GetOptValBool(info->Options, OPTION_XV_DMA, &info->DMAForXv)
	 ? X_CONFIG : X_INFO;
    xf86DrvMsg(pScrn->scrnIndex, from,
	       "Will %stry to use DMA for Xv image transfers\n",
	       info->DMAForXv ? "" : "not ");

    return TRUE;
}
#endif /* XF86DRI */

static void RADEONPreInitColorTiling(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    info->allowColorTiling = xf86ReturnOptValBool(info->Options,
				        OPTION_COLOR_TILING, TRUE);
    if (IS_R300_VARIANT) {
	info->MaxSurfaceWidth = 3968; /* one would have thought 4096...*/
	info->MaxLines = 4096;
    } else {
	info->MaxSurfaceWidth = 2048;
	info->MaxLines = 2048;
    }

    if (!info->allowColorTiling)
	return;

#ifdef XF86DRI
    if (info->directRenderingEnabled &&
	info->pKernelDRMVersion->version_minor < 14) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "[dri] color tiling disabled because of version "
		   "mismatch.\n"
		   "[dri] radeon.o kernel module version is %d.%d.%d but "
		   "1.14.0 or later is required for color tiling.\n",
		   info->pKernelDRMVersion->version_major,
		   info->pKernelDRMVersion->version_minor,
		   info->pKernelDRMVersion->version_patchlevel);
	   info->allowColorTiling = FALSE;
	   return;	   
    }
#endif /* XF86DRI */

    if ((info->allowColorTiling) && (info->IsSecondary)) {
	/* can't have tiling on the 2nd head (as long as it can't use drm).
	 * We'd never get the surface save/restore (vt switching) right...
	 */
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Color tiling disabled for 2nd head\n");
	info->allowColorTiling = FALSE;
    }
    else if ((info->allowColorTiling) && (info->FBDev)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Color tiling not supported with UseFBDev option\n");
	info->allowColorTiling = FALSE;
    }
    else if (info->allowColorTiling) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Color tiling enabled by default\n");
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Color tiling disabled\n");
    }
}


static Bool RADEONPreInitXv(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    CARD16 mm_table;
    CARD16 bios_header;
    CARD16 pll_info_block;

    /* Rescue MM_TABLE before VBIOS is freed */
    info->MM_TABLE_valid = FALSE;
    
    if((info->VBIOS==NULL)||(info->VBIOS[0]!=0x55)||(info->VBIOS[1]!=0xaa)){
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Cannot access BIOS or it is not valid.\n"
               "\t\tIf your card is TV-in capable you will need to specify options RageTheatreCrystal, RageTheatreTunerPort, \n"
               "\t\tRageTheatreSVideoPort and TunerType in /etc/X11/xorg.conf.\n"
               );
       info->MM_TABLE_valid = FALSE;
       return TRUE;
       }

    bios_header=info->VBIOS[0x48];
    bios_header+=(((int)info->VBIOS[0x49]+0)<<8);           
        
    mm_table=info->VBIOS[bios_header+0x38];
    if(mm_table==0)
    {
        xf86DrvMsg(pScrn->scrnIndex,X_INFO,"No MM_TABLE found - assuming CARD is not TV-in capable.\n");
        info->MM_TABLE_valid = FALSE;
        return TRUE;
    }
    mm_table+=(((int)info->VBIOS[bios_header+0x39]+0)<<8)-2;
    
    if(mm_table>0)
    {
        memcpy(&(info->MM_TABLE), &(info->VBIOS[mm_table]), sizeof(info->MM_TABLE));
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "MM_TABLE: %02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x-%02x\n",
            info->MM_TABLE.table_revision,
            info->MM_TABLE.table_size,
            info->MM_TABLE.tuner_type,
            info->MM_TABLE.audio_chip,
            info->MM_TABLE.product_id,
            info->MM_TABLE.tuner_voltage_teletext_fm,
            info->MM_TABLE.i2s_config,
            info->MM_TABLE.video_decoder_type,
            info->MM_TABLE.video_decoder_host_config,
            info->MM_TABLE.input[0],
            info->MM_TABLE.input[1],
            info->MM_TABLE.input[2],
            info->MM_TABLE.input[3],
            info->MM_TABLE.input[4]);
	    
	  /* Is it an MM_TABLE we know about ? */
	  if(info->MM_TABLE.table_size != 0xc){
	       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "This card has MM_TABLE we do not recognize.\n"
			"\t\tIf your card is TV-in capable you will need to specify options RageTheatreCrystal, RageTheatreTunerPort, \n"
			"\t\tRageTheatreSVideoPort and TunerType in /etc/X11/xorg.conf.\n"
			);
		info->MM_TABLE_valid = FALSE;
		return TRUE;
	  	}
        info->MM_TABLE_valid = TRUE;
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "No MM_TABLE found - assuming card is not TV-in capable (mm_table=%d).\n", mm_table);
        info->MM_TABLE_valid = FALSE;
    }

    pll_info_block=info->VBIOS[bios_header+0x30];
    pll_info_block+=(((int)info->VBIOS[bios_header+0x31]+0)<<8);
       
    info->video_decoder_type=info->VBIOS[pll_info_block+0x08];
    info->video_decoder_type+=(((int)info->VBIOS[pll_info_block+0x09]+0)<<8);
    
    return TRUE;
}

static void
RADEONProbeDDC(ScrnInfoPtr pScrn, int indx)
{
    vbeInfoPtr  pVbe;

    if (xf86LoadSubModule(pScrn, "vbe")) {
	pVbe = VBEInit(NULL,indx);
	ConfiguredMonitor = vbeDoEDID(pVbe, NULL);
    }
}

_X_EXPORT Bool RADEONPreInit(ScrnInfoPtr pScrn, int flags)
{
    RADEONInfoPtr     info;
    xf86Int10InfoPtr  pInt10 = NULL;
    void *int10_save = NULL;
    const char *s;
	 char* microc_path = NULL;
	 char* microc_type = NULL;
    MessageType from;

    RADEONTRACE(("RADEONPreInit\n"));
    if (pScrn->numEntities != 1) return FALSE;

    if (!RADEONGetRec(pScrn)) return FALSE;

    info               = RADEONPTR(pScrn);
    info->IsSecondary  = FALSE;
    info->IsPrimary     = FALSE;
    info->MergedFB     = FALSE;
    info->IsSwitching  = FALSE;
    info->MMIO         = NULL;

    info->pEnt         = xf86GetEntityInfo(pScrn->entityList[pScrn->numEntities - 1]);
    if (info->pEnt->location.type != BUS_PCI) goto fail;

    info->PciInfo = xf86GetPciInfoForEntity(info->pEnt->index);
    info->PciTag  = pciTag(info->PciInfo->bus,
			   info->PciInfo->device,
			   info->PciInfo->func);
    info->MMIOAddr   = info->PciInfo->memBase[2] & 0xffffff00;
    info->MMIOSize  = (1 << info->PciInfo->size[2]);
    if (info->pEnt->device->IOBase) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "MMIO address override, using 0x%08lx instead of 0x%08lx\n",
		   info->pEnt->device->IOBase,
		   info->MMIOAddr);
	info->MMIOAddr = info->pEnt->device->IOBase;
    } else if (!info->MMIOAddr) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid MMIO address\n");
	goto fail1;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "MMIO registers at 0x%08lx: size %ldKB\n", info->MMIOAddr, info->MMIOSize / 1024);

    if(!RADEONMapMMIO(pScrn)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Memory map the MMIO region failed\n");
	goto fail1;
    }

#if !defined(__alpha__)
    if (xf86GetPciDomain(info->PciTag) ||
	!xf86IsPrimaryPci(info->PciInfo))
	RADEONPreInt10Save(pScrn, &int10_save);
#else
    /* [Alpha] On the primary, the console already ran the BIOS and we're
     *         going to run it again - so make sure to "fix up" the card
     *         so that (1) we can read the BIOS ROM and (2) the BIOS will
     *         get the memory config right.
     */
    RADEONPreInt10Save(pScrn, &int10_save);
#endif

    if (xf86IsEntityShared(info->pEnt->index)) {
	if (xf86IsPrimInitDone(info->pEnt->index)) {

	    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

	    info->IsSecondary = TRUE;
	    if (!pRADEONEnt->HasSecondary) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "Only one monitor detected, Second screen "
			   "will NOT be created\n");
		goto fail2;
	    }
	    pRADEONEnt->pSecondaryScrn = pScrn;
	} else {
	    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

	    info->IsPrimary = TRUE;

	    xf86SetPrimInitDone(info->pEnt->index);

	    pRADEONEnt->pPrimaryScrn        = pScrn;
	    pRADEONEnt->RestorePrimary      = FALSE;
	    pRADEONEnt->IsSecondaryRestored = FALSE;
	}
    }

    if (flags & PROBE_DETECT) {
	RADEONProbeDDC(pScrn, info->pEnt->index);
	RADEONPostInt10Check(pScrn, int10_save);
	if(info->MMIO) RADEONUnmapMMIO(pScrn);
	return TRUE;
    }


    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "PCI bus %d card %d func %d\n",
	       info->PciInfo->bus,
	       info->PciInfo->device,
	       info->PciInfo->func);

    if (xf86RegisterResources(info->pEnt->index, 0, ResExclusive))
	goto fail;

    if (xf86SetOperatingState(resVga, info->pEnt->index, ResUnusedOpr))
	goto fail;

    pScrn->racMemFlags = RAC_FB | RAC_COLORMAP | RAC_VIEWPORT | RAC_CURSOR;
    pScrn->monitor     = pScrn->confScreen->monitor;

    if (!RADEONPreInitVisual(pScrn))
	goto fail;

				/* We can't do this until we have a
				   pScrn->display. */
    xf86CollectOptions(pScrn, NULL);
    if (!(info->Options = xalloc(sizeof(RADEONOptions))))
	goto fail;

    memcpy(info->Options, RADEONOptions, sizeof(RADEONOptions));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, info->Options);

    /* By default, don't do VGA IOs on ppc */
#if defined(__powerpc__) || !defined(WITH_VGAHW)
    info->VGAAccess = FALSE;
#else
    info->VGAAccess = TRUE;
#endif

#ifdef WITH_VGAHW
    xf86GetOptValBool(info->Options, OPTION_VGA_ACCESS, &info->VGAAccess);
    if (info->VGAAccess) {
       if (!xf86LoadSubModule(pScrn, "vgahw"))
           info->VGAAccess = FALSE;
        else {
           xf86LoaderReqSymLists(vgahwSymbols, NULL);
            if (!vgaHWGetHWRec(pScrn))
               info->VGAAccess = FALSE;
       }
       if (!info->VGAAccess)
           xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Loading VGA module failed,"
                      " trying to run without it\n");
    } else
           xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VGAAccess option set to FALSE,"
                      " VGA module load skipped\n");
    if (info->VGAAccess)
        vgaHWGetIOBase(VGAHWPTR(pScrn));
#endif


    if (!RADEONPreInitWeight(pScrn))
	goto fail;

#ifdef XvExtension
    if (xf86GetOptValInteger(info->Options, OPTION_VIDEO_KEY,
			     &(info->videoKey))) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "video key set to 0x%x\n",
		   info->videoKey);
    } else {
	info->videoKey = 0x1E;
    }
    
    if(xf86GetOptValInteger(info->Options, OPTION_RAGE_THEATRE_CRYSTAL, &(info->RageTheatreCrystal))) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Rage Theatre Crystal frequency was specified as %d.%d Mhz\n",
                                info->RageTheatreCrystal/100, info->RageTheatreCrystal % 100);
    } else {
    	info->RageTheatreCrystal=-1;
    }

    if(xf86GetOptValInteger(info->Options, OPTION_RAGE_THEATRE_TUNER_PORT, &(info->RageTheatreTunerPort))) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Rage Theatre tuner port was specified as %d\n",
                                info->RageTheatreTunerPort);
    } else {
    	info->RageTheatreTunerPort=-1;
    }
    
    if(info->RageTheatreTunerPort>5){
         xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Attempt to assign Rage Theatre tuner port to invalid value. Disabling setting\n");
	 info->RageTheatreTunerPort=-1;
	 }

    if(xf86GetOptValInteger(info->Options, OPTION_RAGE_THEATRE_COMPOSITE_PORT, &(info->RageTheatreCompositePort))) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Rage Theatre composite port was specified as %d\n",
                                info->RageTheatreCompositePort);
    } else {
    	info->RageTheatreCompositePort=-1;
    }

    if(info->RageTheatreCompositePort>6){
         xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Attempt to assign Rage Theatre composite port to invalid value. Disabling setting\n");
	 info->RageTheatreCompositePort=-1;
	 }

    if(xf86GetOptValInteger(info->Options, OPTION_RAGE_THEATRE_SVIDEO_PORT, &(info->RageTheatreSVideoPort))) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Rage Theatre SVideo Port was specified as %d\n",
                                info->RageTheatreSVideoPort);
    } else {
    	info->RageTheatreSVideoPort=-1;
    }

    if(info->RageTheatreSVideoPort>6){
         xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Attempt to assign Rage Theatre SVideo port to invalid value. Disabling setting\n");
	 info->RageTheatreSVideoPort=-1;
	 }

    if(xf86GetOptValInteger(info->Options, OPTION_TUNER_TYPE, &(info->tunerType))) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Tuner type was specified as %d\n",
                                info->tunerType);
    } else {
    	info->tunerType=-1;
    }

    if(info->tunerType>31){
         xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Attempt to set tuner type to invalid value. Disabling setting\n");
	 info->tunerType=-1;
	 }

	if((microc_path = xf86GetOptValString(info->Options, OPTION_RAGE_THEATRE_MICROC_PATH)) != NULL) 
	{
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Rage Theatre Microcode path was specified as %s\n", microc_path);
		info->RageTheatreMicrocPath = microc_path;
    } else {
		info->RageTheatreMicrocPath= NULL;
    }

	if((microc_type = xf86GetOptValString(info->Options, OPTION_RAGE_THEATRE_MICROC_TYPE)) != NULL) 
	{
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Rage Theatre Microcode type was specified as %s\n", microc_type);
		info->RageTheatreMicrocType = microc_type;
	} else {
		info->RageTheatreMicrocType= NULL;
	}
	 
#endif

    info->DispPriority = 1; 
    if ((s = xf86GetOptValString(info->Options, OPTION_DISP_PRIORITY))) {
	if (strcmp(s, "AUTO") == 0) {
	    info->DispPriority = 1;
	} else if (strcmp(s, "BIOS") == 0) {
	    info->DispPriority = 0;
	} else if (strcmp(s, "HIGH") == 0) {
	    info->DispPriority = 2;
	} else
	    info->DispPriority = 1; 
    }

    info->constantDPI = -1;
    from = X_DEFAULT;
    if (xf86GetOptValBool(info->Options, OPTION_CONSTANTDPI, &info->constantDPI)) {
       from = X_CONFIG;
    } else {
       if (monitorResolution > 0) {
	  info->constantDPI = TRUE;
	  from = X_CMDLINE;
	  xf86DrvMsg(pScrn->scrnIndex, from,
		"\"-dpi %d\" given in command line, assuming \"ConstantDPI\" set\n",
		monitorResolution);
       } else {
	  info->constantDPI = FALSE;
       }
    }
    xf86DrvMsg(pScrn->scrnIndex, from,
	"X server will %skeep DPI constant for all screen sizes\n",
	info->constantDPI ? "" : "not ");

    if (xf86ReturnOptValBool(info->Options, OPTION_FBDEV, FALSE)) {
	/* check for Linux framebuffer device */

	if (xf86LoadSubModule(pScrn, "fbdevhw")) {
	    xf86LoaderReqSymLists(fbdevHWSymbols, NULL);

	    if (fbdevHWInit(pScrn, info->PciInfo, NULL)) {
		pScrn->ValidMode     = fbdevHWValidModeWeak();
		info->FBDev = TRUE;
		xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
			   "Using framebuffer device\n");
	    } else {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "fbdevHWInit failed, not using framebuffer device\n");
	    }
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Couldn't load fbdevhw module, not using framebuffer device\n");
	}
    }

    if (!info->FBDev)
	if (!RADEONPreInitInt10(pScrn, &pInt10))
	    goto fail;

    RADEONPostInt10Check(pScrn, int10_save);

    if (!RADEONPreInitChipType(pScrn))
	goto fail;

#ifdef XF86DRI
    /* PreInit DRI first of all since we need that for getting a proper
     * memory map
     */
    info->directRenderingEnabled = RADEONPreInitDRI(pScrn);
#endif

    if (!RADEONPreInitVRAM(pScrn))
	goto fail;

    RADEONPreInitColorTiling(pScrn);

    RADEONPreInitDDC(pScrn);

    RADEONGetBIOSInfo(pScrn, pInt10);
    if (!RADEONQueryConnectedMonitors(pScrn))    goto fail;
    RADEONGetClockInfo(pScrn);

    /* collect MergedFB options */
    /* only parse mergedfb options on the primary head. 
       Mergedfb is already disabled in xinerama/screen based
       multihead */
    if (!info->IsSecondary)
	RADEONGetMergedFBOptions(pScrn);

    if (!RADEONPreInitGamma(pScrn))              goto fail;

    if (!RADEONPreInitModes(pScrn, pInt10))      goto fail;

    if (!RADEONPreInitCursor(pScrn))             goto fail;

    if (!RADEONPreInitAccel(pScrn))              goto fail;

    if (!RADEONPreInitXv(pScrn))                 goto fail;

    /* Free the video bios (if applicable) */
    if (info->VBIOS) {
	xfree(info->VBIOS);
	info->VBIOS = NULL;
    }

				/* Free int10 info */
    if (pInt10)
	xf86FreeInt10(pInt10);

    if(info->MMIO) RADEONUnmapMMIO(pScrn);
    info->MMIO = NULL;

    xf86DrvMsg(pScrn->scrnIndex, X_NOTICE,
	       "For information on using the multimedia capabilities\n\tof this"
	       " adapter, please see http://gatos.sf.net.\n");

    return TRUE;

fail:
				/* Pre-init failed. */
    if (info->IsSecondary) {
        RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
	pRADEONEnt->HasSecondary = FALSE;
    }
				/* Free the video bios (if applicable) */
    if (info->VBIOS) {
	xfree(info->VBIOS);
	info->VBIOS = NULL;
    }

				/* Free int10 info */
    if (pInt10)
	xf86FreeInt10(pInt10);

#ifdef WITH_VGAHW
    if (info->VGAAccess)
           vgaHWFreeHWRec(pScrn);
#endif

 fail2:
    if(info->MMIO) RADEONUnmapMMIO(pScrn);
    info->MMIO = NULL;

 fail1:
    RADEONFreeRec(pScrn);

    return FALSE;
}

/* Load a palette */
static void RADEONLoadPalette(ScrnInfoPtr pScrn, int numColors,
			      int *indices, LOCO *colors, VisualPtr pVisual)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int            i;
    int            idx, j;
    unsigned char  r, g, b;

#ifdef XF86DRI
    if (info->CPStarted && pScrn->pScreen) DRILock(pScrn->pScreen, 0);
#endif

    if (info->accelOn && pScrn->pScreen)
        RADEON_SYNC(info, pScrn);

    if (info->FBDev) {
	fbdevHWLoadPalette(pScrn, numColors, indices, colors, pVisual);
    } else {
	/* If the second monitor is connected, we also need to deal with
	 * the secondary palette
	 */
	if (info->IsSecondary) j = 1;
	else j = 0;

	PAL_SELECT(j);

	if (info->CurrentLayout.depth == 15) {
	    /* 15bpp mode.  This sends 32 values. */
	    for (i = 0; i < numColors; i++) {
		idx = indices[i];
		r   = colors[idx].red;
		g   = colors[idx].green;
		b   = colors[idx].blue;
		OUTPAL(idx * 8, r, g, b);
	    }
	} else if (info->CurrentLayout.depth == 16) {
	    /* 16bpp mode.  This sends 64 values.
	     *
	     * There are twice as many green values as there are values
	     * for red and blue.  So, we take each red and blue pair,
	     * and combine it with each of the two green values.
	     */
	    for (i = 0; i < numColors; i++) {
		idx = indices[i];
		r   = colors[idx / 2].red;
		g   = colors[idx].green;
		b   = colors[idx / 2].blue;
		RADEONWaitForFifo(pScrn, 32); /* delay */
		OUTPAL(idx * 4, r, g, b);

		/* AH - Added to write extra green data - How come this isn't
		 * needed on R128?  We didn't load the extra green data in the
		 * other routine
		 */
		if (idx <= 31) {
		    r   = colors[idx].red;
		    g   = colors[(idx * 2) + 1].green;
		    b   = colors[idx].blue;
		    RADEONWaitForFifo(pScrn, 32); /* delay */
		    OUTPAL(idx * 8, r, g, b);
		}
	    }
	} else {
	    /* 8bpp mode.  This sends 256 values. */
	    for (i = 0; i < numColors; i++) {
		idx = indices[i];
		r   = colors[idx].red;
		b   = colors[idx].blue;
		g   = colors[idx].green;
		RADEONWaitForFifo(pScrn, 32); /* delay */
		OUTPAL(idx, r, g, b);
	    }
	}

	if (info->MergedFB) {
	    PAL_SELECT(1);
	    if (info->CurrentLayout.depth == 15) {
		/* 15bpp mode.  This sends 32 values. */
		for (i = 0; i < numColors; i++) {
		    idx = indices[i];
		    r   = colors[idx].red;
		    g   = colors[idx].green;
		    b   = colors[idx].blue;
		    OUTPAL(idx * 8, r, g, b);
		}
	    } else if (info->CurrentLayout.depth == 16) {
		/* 16bpp mode.  This sends 64 values.
		 *
		 * There are twice as many green values as there are values
		 * for red and blue.  So, we take each red and blue pair,
		 * and combine it with each of the two green values.
		 */
		for (i = 0; i < numColors; i++) {
		    idx = indices[i];
		    r   = colors[idx / 2].red;
		    g   = colors[idx].green;
		    b   = colors[idx / 2].blue;
		    OUTPAL(idx * 4, r, g, b);

		    /* AH - Added to write extra green data - How come
		     * this isn't needed on R128?  We didn't load the
		     * extra green data in the other routine.
		     */
		    if (idx <= 31) {
			r   = colors[idx].red;
			g   = colors[(idx * 2) + 1].green;
			b   = colors[idx].blue;
			OUTPAL(idx * 8, r, g, b);
		    }
		}
	    } else {
		/* 8bpp mode.  This sends 256 values. */
		for (i = 0; i < numColors; i++) {
		    idx = indices[i];
		    r   = colors[idx].red;
		    b   = colors[idx].blue;
		    g   = colors[idx].green;
		    OUTPAL(idx, r, g, b);
		}
	    }
	}
    }

#ifdef XF86DRI
    if (info->CPStarted && pScrn->pScreen) DRIUnlock(pScrn->pScreen);
#endif
}

static void RADEONBlockHandler(int i, pointer blockData,
			       pointer pTimeout, pointer pReadmask)
{
    ScreenPtr      pScreen = screenInfo.screens[i];
    ScrnInfoPtr    pScrn   = xf86Screens[i];
    RADEONInfoPtr  info    = RADEONPTR(pScrn);

#ifdef XF86DRI
    if (info->directRenderingInited) {
	FLUSH_RING();
    }
#endif
#ifdef USE_EXA
    info->engineMode = EXA_ENGINEMODE_UNKNOWN;
#endif
    pScreen->BlockHandler = info->BlockHandler;
    (*pScreen->BlockHandler) (i, blockData, pTimeout, pReadmask);
    pScreen->BlockHandler = RADEONBlockHandler;

    if (info->VideoTimerCallback)
	(*info->VideoTimerCallback)(pScrn, currentTime.milliseconds);

#if defined(RENDER) && defined(USE_XAA)
    if(info->RenderCallback)
	(*info->RenderCallback)(pScrn);
#endif
}


#ifdef USE_XAA
#ifdef XF86DRI
Bool RADEONSetupMemXAA_DRI(int scrnIndex, ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    int            cpp = info->CurrentLayout.pixel_bytes;
    int            depthCpp = (info->depthBits - 8) / 4;
    int            width_bytes = pScrn->displayWidth * cpp;
    int            bufferSize;
    int            depthSize;
    int            l;
    int            scanlines;
    int            texsizerequest;
    BoxRec         MemBox;
    FBAreaPtr      fbarea;

    info->frontOffset = 0;
    info->frontPitch = pScrn->displayWidth;
    info->backPitch = pScrn->displayWidth;

    /* make sure we use 16 line alignment for tiling (8 might be enough).
     * Might need that for non-XF86DRI too?
     */
    if (info->allowColorTiling) {
	bufferSize = (((pScrn->virtualY + 15) & ~15) * width_bytes
		      + RADEON_BUFFER_ALIGN) & ~RADEON_BUFFER_ALIGN;
    } else {
        bufferSize = (pScrn->virtualY * width_bytes
		      + RADEON_BUFFER_ALIGN) & ~RADEON_BUFFER_ALIGN;
    }

    /* Due to tiling, the Z buffer pitch must be a multiple of 32 pixels,
     * which is always the case if color tiling is used due to color pitch
     * but not necessarily otherwise, and its height a multiple of 16 lines.
     */
    info->depthPitch = (pScrn->displayWidth + 31) & ~31;
    depthSize = ((((pScrn->virtualY + 15) & ~15) * info->depthPitch
		  * depthCpp + RADEON_BUFFER_ALIGN) & ~RADEON_BUFFER_ALIGN);

    switch (info->CPMode) {
    case RADEON_DEFAULT_CP_PIO_MODE:
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CP in PIO mode\n");
	break;
    case RADEON_DEFAULT_CP_BM_MODE:
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CP in BM mode\n");
	break;
    default:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CP in UNKNOWN mode\n");
	break;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB GART aperture\n", info->gartSize);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB for the ring buffer\n", info->ringSize);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB for vertex/indirect buffers\n", info->bufSize);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Using %d MB for GART textures\n", info->gartTexSize);

    /* Try for front, back, depth, and three framebuffers worth of
     * pixmap cache.  Should be enough for a fullscreen background
     * image plus some leftovers.
     * If the FBTexPercent option was used, try to achieve that percentage instead,
     * but still have at least one pixmap buffer (get problems with xvideo/render
     * otherwise probably), and never reserve more than 3 offscreen buffers as it's
     * probably useless for XAA.
     */
    if (info->textureSize >= 0) {
	texsizerequest = ((int)info->FbMapSize - 2 * bufferSize - depthSize
			 - 2 * width_bytes - 16384 - info->FbSecureSize)
	/* first divide, then multiply or we'll get an overflow (been there...) */
			 / 100 * info->textureSize;
    }
    else {
	texsizerequest = (int)info->FbMapSize / 2;
    }
    info->textureSize = info->FbMapSize - info->FbSecureSize - 5 * bufferSize - depthSize;

    /* If that gives us less than the requested memory, let's
     * be greedy and grab some more.  Sorry, I care more about 3D
     * performance than playing nicely, and you'll get around a full
     * framebuffer's worth of pixmap cache anyway.
     */
    if (info->textureSize < texsizerequest) {
        info->textureSize = info->FbMapSize - 4 * bufferSize - depthSize;
    }
    if (info->textureSize < texsizerequest) {
        info->textureSize = info->FbMapSize - 3 * bufferSize - depthSize;
    }

    /* If there's still no space for textures, try without pixmap cache, but
     * never use the reserved space, the space hw cursor and PCIGART table might
     * use.
     */
    if (info->textureSize < 0) {
	info->textureSize = info->FbMapSize - 2 * bufferSize - depthSize
	                    - 2 * width_bytes - 16384 - info->FbSecureSize;
    }

    /* Check to see if there is more room available after the 8192nd
     * scanline for textures
     */
    /* FIXME: what's this good for? condition is pretty much impossible to meet */
    if ((int)info->FbMapSize - 8192*width_bytes - bufferSize - depthSize
	> info->textureSize) {
	info->textureSize =
		info->FbMapSize - 8192*width_bytes - bufferSize - depthSize;
    }

    /* If backbuffer is disabled, don't allocate memory for it */
    if (info->noBackBuffer) {
	info->textureSize += bufferSize;
    }

    /* RADEON_BUFFER_ALIGN is not sufficient for backbuffer!
       At least for pageflip + color tiling, need to make sure it's 16 scanlines aligned,
       otherwise the copy-from-front-to-back will fail (width_bytes * 16 will also guarantee
       it's still 4kb aligned for tiled case). Need to round up offset (might get into cursor
       area otherwise).
       This might cause some space at the end of the video memory to be unused, since it
       can't be used (?) due to that log_tex_granularity thing???
       Could use different copyscreentoscreen function for the pageflip copies
       (which would use different src and dst offsets) to avoid this. */   
    if (info->allowColorTiling && !info->noBackBuffer) {
	info->textureSize = info->FbMapSize - ((info->FbMapSize - info->textureSize +
			  width_bytes * 16 - 1) / (width_bytes * 16)) * (width_bytes * 16);
    }
    if (info->textureSize > 0) {
	l = RADEONMinBits((info->textureSize-1) / RADEON_NR_TEX_REGIONS);
	if (l < RADEON_LOG_TEX_GRANULARITY)
	    l = RADEON_LOG_TEX_GRANULARITY;
	/* Round the texture size up to the nearest whole number of
	 * texture regions.  Again, be greedy about this, don't
	 * round down.
	 */
	info->log2TexGran = l;
	info->textureSize = (info->textureSize >> l) << l;
    } else {
	info->textureSize = 0;
    }

    /* Set a minimum usable local texture heap size.  This will fit
     * two 256x256x32bpp textures.
     */
    if (info->textureSize < 512 * 1024) {
	info->textureOffset = 0;
	info->textureSize = 0;
    }

    if (info->allowColorTiling && !info->noBackBuffer) {
	info->textureOffset = ((info->FbMapSize - info->textureSize) /
			       (width_bytes * 16)) * (width_bytes * 16);
    }
    else {
	/* Reserve space for textures */
	info->textureOffset = ((info->FbMapSize - info->textureSize +
				RADEON_BUFFER_ALIGN) &
			       ~(CARD32)RADEON_BUFFER_ALIGN);
    }

    /* Reserve space for the shared depth
     * buffer.
     */
    info->depthOffset = ((info->textureOffset - depthSize +
			  RADEON_BUFFER_ALIGN) &
			 ~(CARD32)RADEON_BUFFER_ALIGN);

    /* Reserve space for the shared back buffer */
    if (info->noBackBuffer) {
       info->backOffset = info->depthOffset;
    } else {
       info->backOffset = ((info->depthOffset - bufferSize +
			    RADEON_BUFFER_ALIGN) &
			   ~(CARD32)RADEON_BUFFER_ALIGN);
    }

    info->backY = info->backOffset / width_bytes;
    info->backX = (info->backOffset - (info->backY * width_bytes)) / cpp;

    scanlines = (info->FbMapSize-info->FbSecureSize) / width_bytes;
    if (scanlines > 8191)
	scanlines = 8191;

    MemBox.x1 = 0;
    MemBox.y1 = 0;
    MemBox.x2 = pScrn->displayWidth;
    MemBox.y2 = scanlines;

    if (!xf86InitFBManager(pScreen, &MemBox)) {
        xf86DrvMsg(scrnIndex, X_ERROR,
		   "Memory manager initialization to "
		   "(%d,%d) (%d,%d) failed\n",
		   MemBox.x1, MemBox.y1, MemBox.x2, MemBox.y2);
	return FALSE;
    } else {
	int  width, height;

	xf86DrvMsg(scrnIndex, X_INFO,
		   "Memory manager initialized to (%d,%d) (%d,%d)\n",
		   MemBox.x1, MemBox.y1, MemBox.x2, MemBox.y2);
	/* why oh why can't we just request modes which are guaranteed to be 16 lines
	   aligned... sigh */
	if ((fbarea = xf86AllocateOffscreenArea(pScreen,
						pScrn->displayWidth,
						info->allowColorTiling ? 
						((pScrn->virtualY + 15) & ~15)
						- pScrn->virtualY + 2 : 2,
						0, NULL, NULL,
						NULL))) {
	    xf86DrvMsg(scrnIndex, X_INFO,
		       "Reserved area from (%d,%d) to (%d,%d)\n",
		       fbarea->box.x1, fbarea->box.y1,
		       fbarea->box.x2, fbarea->box.y2);
	} else {
	    xf86DrvMsg(scrnIndex, X_ERROR, "Unable to reserve area\n");
	}

	RADEONDRIAllocatePCIGARTTable(pScreen);

	if (xf86QueryLargestOffscreenArea(pScreen, &width,
					  &height, 0, 0, 0)) {
	    xf86DrvMsg(scrnIndex, X_INFO,
		       "Largest offscreen area available: %d x %d\n",
		       width, height);

	    /* Lines in offscreen area needed for depth buffer and
	     * textures
	     */
	    info->depthTexLines = (scanlines
				   - info->depthOffset / width_bytes);
	    info->backLines	    = (scanlines
				       - info->backOffset / width_bytes
				       - info->depthTexLines);
	    info->backArea	    = NULL;
	} else {
	    xf86DrvMsg(scrnIndex, X_ERROR,
		       "Unable to determine largest offscreen area "
		       "available\n");
	    return FALSE;
	}
    }

    xf86DrvMsg(scrnIndex, X_INFO,
	       "Will use back buffer at offset 0x%x\n",
	       info->backOffset);
    xf86DrvMsg(scrnIndex, X_INFO,
	       "Will use depth buffer at offset 0x%x\n",
	       info->depthOffset);
    if (info->cardType==CARD_PCIE)
    	xf86DrvMsg(scrnIndex, X_INFO,
	           "Will use %d kb for PCI GART table at offset 0x%lx\n",
		   info->pciGartSize/1024, info->pciGartOffset);
    xf86DrvMsg(scrnIndex, X_INFO,
	       "Will use %d kb for textures at offset 0x%x\n",
	       info->textureSize/1024, info->textureOffset);

    info->frontPitchOffset = (((info->frontPitch * cpp / 64) << 22) |
			      ((info->frontOffset + info->fbLocation) >> 10));

    info->backPitchOffset = (((info->backPitch * cpp / 64) << 22) |
			     ((info->backOffset + info->fbLocation) >> 10));

    info->depthPitchOffset = (((info->depthPitch * depthCpp / 64) << 22) |
			      ((info->depthOffset + info->fbLocation) >> 10));
    return TRUE;
}
#endif /* XF86DRI */

Bool RADEONSetupMemXAA(int scrnIndex, ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    BoxRec         MemBox;
    int            y2;

    int width_bytes = pScrn->displayWidth * info->CurrentLayout.pixel_bytes;

    MemBox.x1 = 0;
    MemBox.y1 = 0;
    MemBox.x2 = pScrn->displayWidth;
    y2 = info->FbMapSize / width_bytes;
    if (y2 >= 32768)
	y2 = 32767; /* because MemBox.y2 is signed short */
    MemBox.y2 = y2;
    
    /* The acceleration engine uses 14 bit
     * signed coordinates, so we can't have any
     * drawable caches beyond this region.
     */
    if (MemBox.y2 > 8191)
	MemBox.y2 = 8191;

    if (!xf86InitFBManager(pScreen, &MemBox)) {
	xf86DrvMsg(scrnIndex, X_ERROR,
		   "Memory manager initialization to "
		   "(%d,%d) (%d,%d) failed\n",
		   MemBox.x1, MemBox.y1, MemBox.x2, MemBox.y2);
	return FALSE;
    } else {
	int       width, height;
	FBAreaPtr fbarea;

	xf86DrvMsg(scrnIndex, X_INFO,
		   "Memory manager initialized to (%d,%d) (%d,%d)\n",
		   MemBox.x1, MemBox.y1, MemBox.x2, MemBox.y2);
	if ((fbarea = xf86AllocateOffscreenArea(pScreen,
						pScrn->displayWidth,
						info->allowColorTiling ? 
						((pScrn->virtualY + 15) & ~15)
						- pScrn->virtualY + 2 : 2,
						0, NULL, NULL,
						NULL))) {
	    xf86DrvMsg(scrnIndex, X_INFO,
		       "Reserved area from (%d,%d) to (%d,%d)\n",
		       fbarea->box.x1, fbarea->box.y1,
		       fbarea->box.x2, fbarea->box.y2);
	} else {
	    xf86DrvMsg(scrnIndex, X_ERROR, "Unable to reserve area\n");
	}
	if (xf86QueryLargestOffscreenArea(pScreen, &width, &height,
					      0, 0, 0)) {
	    xf86DrvMsg(scrnIndex, X_INFO,
		       "Largest offscreen area available: %d x %d\n",
		       width, height);
	}
	return TRUE;
    }    
}
#endif /* USE_XAA */

/* Called at the start of each server generation. */
_X_EXPORT Bool RADEONScreenInit(int scrnIndex, ScreenPtr pScreen,
                                int argc, char **argv)
{
    ScrnInfoPtr    pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    int            hasDRI = 0;
#ifdef RENDER
    int            subPixelOrder = SubPixelUnknown;
    char*          s;
#endif

    RADEONTRACE(("RADEONScreenInit %lx %ld\n",
		 pScrn->memPhysBase, pScrn->fbOffset));

    info->accelOn      = FALSE;
#ifdef USE_XAA
    info->accel        = NULL;
#endif
    pScrn->fbOffset    = 0;
    if (info->IsSecondary) pScrn->fbOffset = pScrn->videoRam * 1024;
    if (!RADEONMapMem(pScrn)) return FALSE;

#ifdef XF86DRI
    info->fbX = 0;
    info->fbY = 0;
#endif

    info->PaletteSavedOnVT = FALSE;

    RADEONSave(pScrn);

    if ((!info->IsSecondary) && info->IsMobility) {
        if (xf86ReturnOptValBool(info->Options, OPTION_DYNAMIC_CLOCKS, FALSE)) {
	    RADEONSetDynamicClock(pScrn, 1);
        } else {
	    RADEONSetDynamicClock(pScrn, 0);
        }
    }

    if ((!info->IsSecondary) && (IS_R300_VARIANT || IS_RV100_VARIANT))
      RADEONForceSomeClocks(pScrn);

    if (info->allowColorTiling && (pScrn->virtualX > info->MaxSurfaceWidth)) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Color tiling not supported with virtual x resolutions larger than %d, disabling\n",
		    info->MaxSurfaceWidth);
	info->allowColorTiling = FALSE;
    }
    if (info->allowColorTiling) {
	if (info->MergedFB) {
	    if ((((RADEONMergedDisplayModePtr)pScrn->currentMode->Private)->CRT1->Flags &
		(V_DBLSCAN | V_INTERLACE)) ||
		(((RADEONMergedDisplayModePtr)pScrn->currentMode->Private)->CRT2->Flags &
		(V_DBLSCAN | V_INTERLACE)))
		info->tilingEnabled = FALSE;
	    else info->tilingEnabled = TRUE;
	}
	else {
            info->tilingEnabled = (pScrn->currentMode->Flags & (V_DBLSCAN | V_INTERLACE)) ? FALSE : TRUE;
	}
    }

    /* Visual setup */
    miClearVisualTypes();
    if (!miSetVisualTypes(pScrn->depth,
			  miGetDefaultVisualMask(pScrn->depth),
			  pScrn->rgbBits,
			  pScrn->defaultVisual)) return FALSE;
    miSetPixmapDepths ();

#ifdef XF86DRI
    if (info->directRenderingEnabled) {
	MessageType from;

	info->depthBits = pScrn->depth;

	from = xf86GetOptValInteger(info->Options, OPTION_DEPTH_BITS,
				    &info->depthBits)
	     ? X_CONFIG : X_DEFAULT;

	if (info->depthBits != 16 && info->depthBits != 24) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Value for Option \"DepthBits\" must be 16 or 24\n");
	    info->depthBits = pScrn->depth;
	    from = X_DEFAULT;
	}

	xf86DrvMsg(pScrn->scrnIndex, from,
		   "Using %d bit depth buffer\n", info->depthBits);
    }

    /* Setup DRI after visuals have been established, but before fbScreenInit is
     * called.  fbScreenInit will eventually call the driver's InitGLXVisuals
     * call back. */
    if (info->directRenderingEnabled) {
	/* FIXME: When we move to dynamic allocation of back and depth
	 * buffers, we will want to revisit the following check for 3
	 * times the virtual size of the screen below.
	 */
	int  width_bytes = (pScrn->displayWidth *
			    info->CurrentLayout.pixel_bytes);
	int  maxy        = info->FbMapSize / width_bytes;

	if (maxy <= pScrn->virtualY * 3) {
	    xf86DrvMsg(scrnIndex, X_ERROR,
		       "Static buffer allocation failed.  Disabling DRI.\n");
	    xf86DrvMsg(scrnIndex, X_ERROR,
		       "At least %d kB of video memory needed at this "
		       "resolution and depth.\n",
		       (pScrn->displayWidth * pScrn->virtualY *
			info->CurrentLayout.pixel_bytes * 3 + 1023) / 1024);
	    info->directRenderingEnabled = FALSE;
	} else {
	    info->directRenderingEnabled = RADEONDRIScreenInit(pScreen);
	}
    }

    /* Tell DRI about new memory map */
    if (info->directRenderingEnabled && info->newMemoryMap) {
	drmRadeonSetParam  radeonsetparam;
	RADEONTRACE(("DRI New memory map param\n"));
	memset(&radeonsetparam, 0, sizeof(drmRadeonSetParam));
	radeonsetparam.param = RADEON_SETPARAM_NEW_MEMMAP;
	radeonsetparam.value = 1;
	if (drmCommandWrite(info->drmFD, DRM_RADEON_SETPARAM,
			    &radeonsetparam, sizeof(drmRadeonSetParam)) < 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "[drm] failed to enable new memory map\n");
		RADEONDRICloseScreen(pScreen);
		info->directRenderingEnabled = FALSE;		
	}
    }

    hasDRI = info->directRenderingEnabled;
#endif /* XF86DRI */

    /* Initialize the memory map, this basically calculates the values
     * we'll use later on for MC_FB_LOCATION & MC_AGP_LOCATION
     */
    RADEONInitMemoryMap(pScrn);

    if (!info->IsSecondary) {
	/* empty the surfaces */
	unsigned char *RADEONMMIO = info->MMIO;
	unsigned int i;
	for (i = 0; i < 8; i++) {
	    OUTREG(RADEON_SURFACE0_INFO + 16 * i, 0);
	    OUTREG(RADEON_SURFACE0_LOWER_BOUND + 16 * i, 0);
	    OUTREG(RADEON_SURFACE0_UPPER_BOUND + 16 * i, 0);
	}
    }

    if (info->FBDev) {
	unsigned char *RADEONMMIO = info->MMIO;

	if (!fbdevHWModeInit(pScrn, pScrn->currentMode)) return FALSE;
	pScrn->displayWidth = fbdevHWGetLineLength(pScrn)
		/ info->CurrentLayout.pixel_bytes;
	RADEONSaveMemMapRegisters(pScrn, &info->ModeReg);
	info->fbLocation = (info->ModeReg.mc_fb_location & 0xffff) << 16;
	info->ModeReg.surface_cntl = INREG(RADEON_SURFACE_CNTL);
	info->ModeReg.surface_cntl &= ~RADEON_SURF_TRANSLATION_DIS;
    } else {
	if (!RADEONModeInit(pScrn, pScrn->currentMode)) return FALSE;
    }

    RADEONSaveScreen(pScreen, SCREEN_SAVER_ON);

    pScrn->AdjustFrame(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

#ifdef XF86DRI
    /* Depth moves are disabled by default since they are extremely slow */
    info->depthMoves = xf86ReturnOptValBool(info->Options,
						 OPTION_DEPTH_MOVE, FALSE);
    if (info->depthMoves && info->allowColorTiling) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Enabling depth moves\n");
    } else if (info->depthMoves) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Depth moves don't work without color tiling, disabled\n");
	info->depthMoves = FALSE;
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Depth moves disabled by default\n");
    }
#endif

    /* Initial setup of surfaces */
    if (!info->IsSecondary) {
	RADEONTRACE(("Setting up initial surfaces\n"));
	RADEONChangeSurfaces(pScrn);
    }

    RADEONTRACE(("Initializing fb layer\n"));

    /* Init fb layer */
    if (!fbScreenInit(pScreen, info->FB,
		      pScrn->virtualX, pScrn->virtualY,
		      pScrn->xDpi, pScrn->yDpi, pScrn->displayWidth,
		      pScrn->bitsPerPixel))
	return FALSE;

    xf86SetBlackWhitePixels(pScreen);

    if (pScrn->bitsPerPixel > 8) {
	VisualPtr  visual;

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

    /* Must be after RGB order fixed */
    fbPictureInit (pScreen, 0, 0);

#ifdef RENDER
    if ((s = xf86GetOptValString(info->Options, OPTION_SUBPIXEL_ORDER))) {
	if (strcmp(s, "RGB") == 0) subPixelOrder = SubPixelHorizontalRGB;
	else if (strcmp(s, "BGR") == 0) subPixelOrder = SubPixelHorizontalBGR;
	else if (strcmp(s, "NONE") == 0) subPixelOrder = SubPixelNone;
	PictureSetSubpixelOrder (pScreen, subPixelOrder);
    } 

    if (PictureGetSubpixelOrder (pScreen) == SubPixelUnknown) {
	switch (info->DisplayType) {
	case MT_NONE:	subPixelOrder = SubPixelUnknown; break;
	case MT_LCD:	subPixelOrder = SubPixelHorizontalRGB; break;
	case MT_DFP:	subPixelOrder = SubPixelHorizontalRGB; break;
	default:	subPixelOrder = SubPixelNone; break;
	}
	PictureSetSubpixelOrder (pScreen, subPixelOrder);
    }
#endif
				/* Memory manager setup */

    RADEONTRACE(("Setting up accel memmap\n"));

#ifdef USE_EXA
    if (info->useEXA) {
#ifdef XF86DRI
	MessageType from = X_DEFAULT;

	if (hasDRI) {
	    info->accelDFS = info->cardType != CARD_AGP;

	    if (xf86GetOptValInteger(info->Options, OPTION_ACCEL_DFS,
				     &info->accelDFS)) {
		from = X_CONFIG;
	    }

	    /* Reserve approx. half of offscreen memory for local textures by
	     * default, can be overridden with Option "FBTexPercent".
	     * Round down to a whole number of texture regions.
	     */
	    info->textureSize = 50;

	    if (xf86GetOptValInteger(info->Options, OPTION_FBTEX_PERCENT,
				     &(info->textureSize))) {
		if (info->textureSize < 0 || info->textureSize > 100) {
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			       "Illegal texture memory percentage: %dx, setting to default 50%%\n",
			       info->textureSize);
		    info->textureSize = 50;
		}
	    }
	}

	xf86DrvMsg(pScrn->scrnIndex, from,
		   "%ssing accelerated EXA DownloadFromScreen hook\n",
		   info->accelDFS ? "U" : "Not u");
#endif /* XF86DRI */

	if (!RADEONSetupMemEXA(pScreen))
	    return FALSE;
    }
#endif

#if defined(XF86DRI) && defined(USE_XAA)
    if (!info->useEXA && hasDRI) {
	info->textureSize = -1;
	if (xf86GetOptValInteger(info->Options, OPTION_FBTEX_PERCENT,
				 &(info->textureSize))) {
	    if (info->textureSize < 0 || info->textureSize > 100) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Illegal texture memory percentage: %dx, using default behaviour\n",
			   info->textureSize);
		info->textureSize = -1;
	    }
	}
	if (!RADEONSetupMemXAA_DRI(scrnIndex, pScreen))
	    return FALSE;
    }
#endif

#ifdef USE_XAA
    if (!info->useEXA && !hasDRI && !RADEONSetupMemXAA(scrnIndex, pScreen))
	return FALSE;
#endif

    info->dst_pitch_offset = (((pScrn->displayWidth * info->CurrentLayout.pixel_bytes / 64)
			       << 22) | ((info->fbLocation + pScrn->fbOffset) >> 10));

    /* Backing store setup */
    RADEONTRACE(("Initializing backing store\n"));
    miInitializeBackingStore(pScreen);
    xf86SetBackingStore(pScreen);

    /* DRI finalisation */
#ifdef XF86DRI
    if (info->directRenderingEnabled && info->cardType==CARD_PCIE &&
	info->pciGartOffset && info->pKernelDRMVersion->version_minor >= 19)
    {
      drmRadeonSetParam  radeonsetparam;
      RADEONTRACE(("DRI PCIGART param\n"));
      memset(&radeonsetparam, 0, sizeof(drmRadeonSetParam));
      radeonsetparam.param = RADEON_SETPARAM_PCIGART_LOCATION;
      radeonsetparam.value = info->pciGartOffset;
      if (drmCommandWrite(info->drmFD, DRM_RADEON_SETPARAM,
			  &radeonsetparam, sizeof(drmRadeonSetParam)) < 0)
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[drm] failed set pci gart location\n");
    }
    if (info->directRenderingEnabled) {
        RADEONTRACE(("DRI Finishing init !\n"));
	info->directRenderingEnabled = RADEONDRIFinishScreenInit(pScreen);
    }
    if (info->directRenderingEnabled) {
	/* DRI final init might have changed the memory map, we need to adjust
	 * our local image to make sure we restore them properly on mode
	 * changes or VT switches
	 */
	RADEONAdjustMemMapRegisters(pScrn, &info->ModeReg);

	if ((info->DispPriority == 1) && (info->cardType==CARD_AGP)) {
	    /* we need to re-calculate bandwidth because of AGPMode difference. */ 
	    RADEONInitDispBandwidth(pScrn);
	}
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Direct rendering enabled\n");

	/* we might already be in tiled mode, tell drm about it */
	if (info->directRenderingEnabled && info->tilingEnabled) {
	    drmRadeonSetParam  radeonsetparam;
	    memset(&radeonsetparam, 0, sizeof(drmRadeonSetParam));
	    radeonsetparam.param = RADEON_SETPARAM_SWITCH_TILING;
	    radeonsetparam.value = info->tilingEnabled ? 1 : 0; 
	    if (drmCommandWrite(info->drmFD, DRM_RADEON_SETPARAM,
		&radeonsetparam, sizeof(drmRadeonSetParam)) < 0)
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "[drm] failed changing tiling status\n");
	}
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
		   "Direct rendering disabled\n");
    }
#endif

    /* Make sure surfaces are allright since DRI setup may have changed them */
    if (!info->IsSecondary) {
	RADEONTRACE(("Setting up final surfaces\n"));
	RADEONChangeSurfaces(pScrn);
    }

    if(info->MergedFB)
	/* need this here to fix up sarea values */
	RADEONAdjustFrameMerged(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

    /* Enable aceleration */
    if (!xf86ReturnOptValBool(info->Options, OPTION_NOACCEL, FALSE)) {
	 RADEONTRACE(("Initializing Acceleration\n"));
	if (RADEONAccelInit(pScreen)) {
	    xf86DrvMsg(scrnIndex, X_INFO, "Acceleration enabled\n");
	    info->accelOn = TRUE;
	} else {
	    xf86DrvMsg(scrnIndex, X_ERROR,
		       "Acceleration initialization failed\n");
	    xf86DrvMsg(scrnIndex, X_INFO, "Acceleration disabled\n");
	    info->accelOn = FALSE;
	}
    } else {
	xf86DrvMsg(scrnIndex, X_INFO, "Acceleration disabled\n");
	info->accelOn = FALSE;
    }

#ifdef XF86DRI
    /* Init page flipping if enabled now */
    if (info->allowPageFlip) {
	RADEONTRACE(("Initializing Page Flipping\n"));
	RADEONDRIInitPageFlip(pScreen);
    }
#endif

    /* Init DPMS */
    RADEONTRACE(("Initializing DPMS\n"));
    xf86DPMSInit(pScreen, RADEONDisplayPowerManagementSet, 0);

    RADEONTRACE(("Initializing Cursor\n"));

    /* Set Silken Mouse */
    xf86SetSilkenMouse(pScreen);

    /* Cursor setup */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    /* Hardware cursor setup */
    if (!xf86ReturnOptValBool(info->Options, OPTION_SW_CURSOR, FALSE)) {
	if (RADEONCursorInit(pScreen)) {
#ifdef USE_XAA
	    if (!info->useEXA) {
		int  width, height;

		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "Using hardware cursor (scanline %ld)\n",
			   info->cursor_offset / pScrn->displayWidth
			   / info->CurrentLayout.pixel_bytes);
		if (xf86QueryLargestOffscreenArea(pScreen, &width, &height,
					      0, 0, 0)) {
		    xf86DrvMsg(scrnIndex, X_INFO,
			       "Largest offscreen area available: %d x %d\n",
			       width, height);
		}
	    }
#endif /* USE_XAA */
	} else {
	    xf86DrvMsg(scrnIndex, X_ERROR,
		       "Hardware cursor initialization failed\n");
	    xf86DrvMsg(scrnIndex, X_INFO, "Using software cursor\n");
	}
    } else {
	info->cursor_offset = 0;
	xf86DrvMsg(scrnIndex, X_INFO, "Using software cursor\n");
    }

    /* Colormap setup */
    RADEONTRACE(("Initializing color map\n"));
    if (!miCreateDefColormap(pScreen)) return FALSE;
    if (!xf86HandleColormaps(pScreen, 256, info->dac6bits ? 6 : 8,
			     RADEONLoadPalette, NULL,
			     CMAP_PALETTED_TRUECOLOR
#if 0 /* This option messes up text mode! (eich@suse.de) */
			     | CMAP_LOAD_EVEN_IF_OFFSCREEN
#endif
			     | CMAP_RELOAD_ON_MODE_SWITCH)) return FALSE;

    /* DGA setup */
    RADEONTRACE(("Initializing DGA\n"));
    RADEONDGAInit(pScreen);

    /* Wrap some funcs for MergedFB */
    if(info->MergedFB) {
       info->PointerMoved = pScrn->PointerMoved;
       pScrn->PointerMoved = RADEONMergePointerMoved;
       /* Psuedo xinerama */
       if(info->UseRADEONXinerama) {
          RADEONnoPanoramiXExtension = FALSE;
          RADEONXineramaExtensionInit(pScrn);
       } else {
	  info->MouseRestrictions = FALSE;
       }
    }

    /* Init Xv */
    RADEONTRACE(("Initializing Xv\n"));
    RADEONInitVideo(pScreen);

    if(info->MergedFB)
	/* need this here to fix up sarea values */
	RADEONAdjustFrameMerged(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

    /* Provide SaveScreen & wrap BlockHandler and CloseScreen */
    /* Wrap CloseScreen */
    info->CloseScreen    = pScreen->CloseScreen;
    pScreen->CloseScreen = RADEONCloseScreen;
    pScreen->SaveScreen  = RADEONSaveScreen;
    info->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = RADEONBlockHandler;

    /* Note unused options */
    if (serverGeneration == 1)
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);

    RADEONTRACE(("RADEONScreenInit finished\n"));

    return TRUE;
}

/* Write memory mapping registers */
static void RADEONRestoreMemMapRegisters(ScrnInfoPtr pScrn,
					 RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int timeout;

    RADEONTRACE(("RADEONRestoreMemMapRegisters() : \n"));
    RADEONTRACE(("  MC_FB_LOCATION   : 0x%08lx\n", restore->mc_fb_location));
    RADEONTRACE(("  MC_AGP_LOCATION  : 0x%08lx\n", restore->mc_agp_location));

    /* Write memory mapping registers only if their value change
     * since we must ensure no access is done while they are
     * reprogrammed
     */
    if (INREG(RADEON_MC_FB_LOCATION) != restore->mc_fb_location ||
	INREG(RADEON_MC_AGP_LOCATION) != restore->mc_agp_location) {
	CARD32 crtc_ext_cntl, crtc_gen_cntl, crtc2_gen_cntl=0, ov0_scale_cntl;
	CARD32 old_mc_status, status_idle;

	RADEONTRACE(("  Map Changed ! Applying ...\n"));

	/* Make sure engine is idle. We assume the CCE is stopped
	 * at this point
	 */
	RADEONWaitForIdleMMIO(pScrn);

	if (info->IsIGP)
		goto igp_no_mcfb;

	/* Capture MC_STATUS in case things go wrong ... */
	old_mc_status = INREG(RADEON_MC_STATUS);

	/* Stop display & memory access */
	ov0_scale_cntl = INREG(RADEON_OV0_SCALE_CNTL);
	OUTREG(RADEON_OV0_SCALE_CNTL, ov0_scale_cntl & ~RADEON_SCALER_ENABLE);
	crtc_ext_cntl = INREG(RADEON_CRTC_EXT_CNTL);
	OUTREG(RADEON_CRTC_EXT_CNTL, crtc_ext_cntl | RADEON_CRTC_DISPLAY_DIS);
	crtc_gen_cntl = INREG(RADEON_CRTC_GEN_CNTL);
	RADEONWaitForVerticalSync(pScrn);
	OUTREG(RADEON_CRTC_GEN_CNTL,
	       (crtc_gen_cntl
		& ~(RADEON_CRTC_CUR_EN | RADEON_CRTC_ICON_EN))
	       | RADEON_CRTC_DISP_REQ_EN_B | RADEON_CRTC_EXT_DISP_EN);

 	if (info->HasCRTC2) {
	    crtc2_gen_cntl = INREG(RADEON_CRTC2_GEN_CNTL);
	    RADEONWaitForVerticalSync2(pScrn);
	    OUTREG(RADEON_CRTC2_GEN_CNTL,
		   (crtc2_gen_cntl
		    & ~(RADEON_CRTC2_CUR_EN | RADEON_CRTC2_ICON_EN))
		   | RADEON_CRTC2_DISP_REQ_EN_B);
	}

 	/* Make sure the chip settles down (paranoid !) */ 
 	usleep(100000);

	/* Wait for MC idle */
	if (IS_R300_VARIANT)
	    status_idle = R300_MC_IDLE;
	else
	    status_idle = RADEON_MC_IDLE;

	timeout = 0;
	while (!(INREG(RADEON_MC_STATUS) & status_idle)) {
	    if (++timeout > 1000000) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "Timeout trying to update memory controller settings !\n");
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "MC_STATUS = 0x%08x (on entry = 0x%08x)\n",
			   INREG(RADEON_MC_STATUS), (unsigned int)old_mc_status);
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "You will probably crash now ... \n");
		/* Nothing we can do except maybe try to kill the server,
		 * let's wait 2 seconds to leave the above message a chance
		 * to maybe hit the disk and continue trying to setup despite
		 * the MC being non-idle
		 */
		usleep(2000000);
	    }
	    usleep(10);
	}

	/* Update maps, first clearing out AGP to make sure we don't get
	 * a temporary overlap
	 */
 	OUTREG(RADEON_MC_AGP_LOCATION, 0xfffffffc);
	OUTREG(RADEON_MC_FB_LOCATION, restore->mc_fb_location);
    igp_no_mcfb:
 	OUTREG(RADEON_MC_AGP_LOCATION, restore->mc_agp_location);
	/* Make sure map fully reached the chip */
	(void)INREG(RADEON_MC_FB_LOCATION);

	RADEONTRACE(("  Map applied, resetting engine ...\n"));

	/* Reset the engine and HDP */
	RADEONEngineReset(pScrn);

	/* Make sure we have sane offsets before re-enabling the CRTCs, disable
	 * stereo, clear offsets, and wait for offsets to catch up with hw
	 */

	OUTREG(RADEON_CRTC_OFFSET_CNTL, RADEON_CRTC_OFFSET_FLIP_CNTL);
	OUTREG(RADEON_CRTC_OFFSET, 0);
	OUTREG(RADEON_CUR_OFFSET, 0);
	timeout = 0;
	while(INREG(RADEON_CRTC_OFFSET) & RADEON_CRTC_OFFSET__GUI_TRIG_OFFSET) {
	    if (timeout++ > 1000000) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Timeout waiting for CRTC offset to update !\n");
		break;
	    }
	    usleep(1000);
	}
	if (info->HasCRTC2) {
	    OUTREG(RADEON_CRTC2_OFFSET_CNTL, RADEON_CRTC2_OFFSET_FLIP_CNTL);
	    OUTREG(RADEON_CRTC2_OFFSET, 0);
	    OUTREG(RADEON_CUR2_OFFSET, 0);
	    timeout = 0;
	    while(INREG(RADEON_CRTC2_OFFSET) & RADEON_CRTC2_OFFSET__GUI_TRIG_OFFSET) {
		if (timeout++ > 1000000) {
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			       "Timeout waiting for CRTC2 offset to update !\n");
		    break;
		}
		usleep(1000);
	    }
	}
    }

    RADEONTRACE(("Updating display base addresses...\n"));

    OUTREG(RADEON_DISPLAY_BASE_ADDR, restore->display_base_addr);
    if (info->HasCRTC2)
        OUTREG(RADEON_DISPLAY2_BASE_ADDR, restore->display2_base_addr);
    OUTREG(RADEON_OV0_BASE_ADDR, restore->ov0_base_addr);
    (void)INREG(RADEON_OV0_BASE_ADDR);

    /* More paranoia delays, wait 100ms */
    usleep(100000);

    RADEONTRACE(("Memory map updated.\n"));
 }

#ifdef XF86DRI
static void RADEONAdjustMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32 fb, agp;
    int fb_loc_changed;

    fb = INREG(RADEON_MC_FB_LOCATION);
    agp = INREG(RADEON_MC_AGP_LOCATION);
    fb_loc_changed = (fb != info->mc_fb_location);

    if (fb_loc_changed || agp != info->mc_agp_location) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "DRI init changed memory map, adjusting ...\n");
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "  MC_FB_LOCATION  was: 0x%08lx is: 0x%08lx\n",
		       info->mc_fb_location, fb);
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "  MC_AGP_LOCATION was: 0x%08lx is: 0x%08lx\n",
		       info->mc_agp_location, agp);
	    info->mc_fb_location = fb;
	    info->mc_agp_location = agp;
	    info->fbLocation = (save->mc_fb_location & 0xffff) << 16;
	    info->dst_pitch_offset =
		    (((pScrn->displayWidth * info->CurrentLayout.pixel_bytes / 64)
		      << 22) | ((info->fbLocation + pScrn->fbOffset) >> 10));


	    RADEONInitMemMapRegisters(pScrn, save, info);

	    /* If MC_FB_LOCATION was changed, adjust the various offsets */
	    if (fb_loc_changed)
		    RADEONRestoreMemMapRegisters(pScrn, save);
    }

#ifdef USE_EXA
    if (info->accelDFS)
    {
	drmRadeonGetParam gp;
	int gart_base;

	memset(&gp, 0, sizeof(gp));
	gp.param = RADEON_PARAM_GART_BASE;
	gp.value = &gart_base;

	if (drmCommandWriteRead(info->drmFD, DRM_RADEON_GETPARAM, &gp,
				sizeof(gp)) < 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Failed to determine GART area MC location, not using "
		       "accelerated DownloadFromScreen hook!\n");
	    info->accelDFS = FALSE;
	} else {
	    info->gartLocation = gart_base;
	}
    }
#endif /* USE_EXA */
}
#endif

/* Write common registers */
static void RADEONRestoreCommonRegisters(ScrnInfoPtr pScrn,
					 RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG(RADEON_OVR_CLR,            restore->ovr_clr);
    OUTREG(RADEON_OVR_WID_LEFT_RIGHT, restore->ovr_wid_left_right);
    OUTREG(RADEON_OVR_WID_TOP_BOTTOM, restore->ovr_wid_top_bottom);
    OUTREG(RADEON_OV0_SCALE_CNTL,     restore->ov0_scale_cntl);
    OUTREG(RADEON_SUBPIC_CNTL,        restore->subpic_cntl);
    OUTREG(RADEON_VIPH_CONTROL,       restore->viph_control);
    OUTREG(RADEON_I2C_CNTL_1,         restore->i2c_cntl_1);
    OUTREG(RADEON_GEN_INT_CNTL,       restore->gen_int_cntl);
    OUTREG(RADEON_CAP0_TRIG_CNTL,     restore->cap0_trig_cntl);
    OUTREG(RADEON_CAP1_TRIG_CNTL,     restore->cap1_trig_cntl);
    OUTREG(RADEON_BUS_CNTL,           restore->bus_cntl);
    OUTREG(RADEON_SURFACE_CNTL,       restore->surface_cntl);

    /* Workaround for the VT switching problem in dual-head mode.  This
     * problem only occurs on RV style chips, typically when a FP and
     * CRT are connected.
     */
    if (info->HasCRTC2 &&
	!info->IsSwitching &&
	info->ChipFamily != CHIP_FAMILY_R200 &&
	!IS_R300_VARIANT) {
	CARD32        tmp;
        RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

	if (pRADEONEnt->HasSecondary || info->MergedFB) {
	    tmp = INREG(RADEON_DAC_CNTL2);
	    OUTREG(RADEON_DAC_CNTL2, tmp & ~RADEON_DAC2_DAC_CLK_SEL);
	    usleep(100000);
	}
    }
}

/* Write miscellaneous registers which might have been destroyed by an fbdevHW
 * call
 */
static void RADEONRestoreFBDevRegisters(ScrnInfoPtr pScrn,
					 RADEONSavePtr restore)
{
#ifdef XF86DRI
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    /* Restore register for vertical blank interrupts */
    if (info->irq) {
	OUTREG(RADEON_GEN_INT_CNTL, restore->gen_int_cntl);
    }

    /* Restore registers for page flipping */
    if (info->allowPageFlip) {
	OUTREG(RADEON_CRTC_OFFSET_CNTL, restore->crtc_offset_cntl);
	if (info->HasCRTC2) {
	    OUTREG(RADEON_CRTC2_OFFSET_CNTL, restore->crtc2_offset_cntl);
	}
    }
#endif
}

/* Write CRTC registers */
static void RADEONRestoreCrtcRegisters(ScrnInfoPtr pScrn,
				       RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    RADEONTRACE(("Programming CRTC1, offset: 0x%08lx\n",
		 restore->crtc_offset));

    /* We prevent the CRTC from hitting the memory controller until
     * fully programmed
     */
    OUTREG(RADEON_CRTC_GEN_CNTL, restore->crtc_gen_cntl |
	   RADEON_CRTC_DISP_REQ_EN_B);

    OUTREGP(RADEON_CRTC_EXT_CNTL,
	    restore->crtc_ext_cntl,
	    RADEON_CRTC_VSYNC_DIS |
	    RADEON_CRTC_HSYNC_DIS |
	    RADEON_CRTC_DISPLAY_DIS);

    OUTREGP(RADEON_DAC_CNTL,
	    restore->dac_cntl,
	    RADEON_DAC_RANGE_CNTL |
	    RADEON_DAC_BLANKING);

    OUTREG(RADEON_CRTC_H_TOTAL_DISP,    restore->crtc_h_total_disp);
    OUTREG(RADEON_CRTC_H_SYNC_STRT_WID, restore->crtc_h_sync_strt_wid);
    OUTREG(RADEON_CRTC_V_TOTAL_DISP,    restore->crtc_v_total_disp);
    OUTREG(RADEON_CRTC_V_SYNC_STRT_WID, restore->crtc_v_sync_strt_wid);
    OUTREG(RADEON_CRTC_OFFSET,          restore->crtc_offset);
    OUTREG(RADEON_CRTC_OFFSET_CNTL,     restore->crtc_offset_cntl);
    OUTREG(RADEON_CRTC_PITCH,           restore->crtc_pitch);
    OUTREG(RADEON_DISP_MERGE_CNTL,      restore->disp_merge_cntl);
    OUTREG(RADEON_CRTC_MORE_CNTL,       restore->crtc_more_cntl);

    if (info->IsDellServer) {
	OUTREG(RADEON_TV_DAC_CNTL, restore->tv_dac_cntl);
	OUTREG(RADEON_DISP_HW_DEBUG, restore->disp_hw_debug);
	OUTREG(RADEON_DAC_CNTL2, restore->dac2_cntl);
	OUTREG(RADEON_CRTC2_GEN_CNTL, restore->crtc2_gen_cntl);
    }

    OUTREG(RADEON_CRTC_GEN_CNTL, restore->crtc_gen_cntl);
}

/* Write CRTC2 registers */
static void RADEONRestoreCrtc2Registers(ScrnInfoPtr pScrn,
					RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32	   crtc2_gen_cntl;

    RADEONTRACE(("Programming CRTC2, offset: 0x%08lx\n",
		 restore->crtc2_offset));

    crtc2_gen_cntl = INREG(RADEON_CRTC2_GEN_CNTL) &
	    (RADEON_CRTC2_VSYNC_DIS |
	     RADEON_CRTC2_HSYNC_DIS |
	     RADEON_CRTC2_DISP_DIS);
    crtc2_gen_cntl |= restore->crtc2_gen_cntl;

    /* We prevent the CRTC from hitting the memory controller until
     * fully programmed
     */
    OUTREG(RADEON_CRTC2_GEN_CNTL,
	   crtc2_gen_cntl | RADEON_CRTC2_DISP_REQ_EN_B);

    OUTREG(RADEON_DAC_CNTL2, restore->dac2_cntl);

    OUTREG(RADEON_TV_DAC_CNTL, 0x00280203);
    if ((info->ChipFamily == CHIP_FAMILY_R200) ||
	IS_R300_VARIANT) {
	OUTREG(RADEON_DISP_OUTPUT_CNTL, restore->disp_output_cntl);
    } else {
	OUTREG(RADEON_DISP_HW_DEBUG, restore->disp_hw_debug);
    }

    OUTREG(RADEON_CRTC2_H_TOTAL_DISP,    restore->crtc2_h_total_disp);
    OUTREG(RADEON_CRTC2_H_SYNC_STRT_WID, restore->crtc2_h_sync_strt_wid);
    OUTREG(RADEON_CRTC2_V_TOTAL_DISP,    restore->crtc2_v_total_disp);
    OUTREG(RADEON_CRTC2_V_SYNC_STRT_WID, restore->crtc2_v_sync_strt_wid);
    OUTREG(RADEON_CRTC2_OFFSET,          restore->crtc2_offset);
    OUTREG(RADEON_CRTC2_OFFSET_CNTL,     restore->crtc2_offset_cntl);
    OUTREG(RADEON_CRTC2_PITCH,           restore->crtc2_pitch);
    OUTREG(RADEON_DISP2_MERGE_CNTL,      restore->disp2_merge_cntl);

    if ((info->DisplayType == MT_DFP && info->IsSecondary) || 
	info->MergeType == MT_DFP) {	
	OUTREG(RADEON_FP_H2_SYNC_STRT_WID, restore->fp2_h_sync_strt_wid);
	OUTREG(RADEON_FP_V2_SYNC_STRT_WID, restore->fp2_v_sync_strt_wid);
	OUTREG(RADEON_FP2_GEN_CNTL,        restore->fp2_gen_cntl);
    }

    OUTREG(RADEON_CRTC2_GEN_CNTL, crtc2_gen_cntl);

#if 0
    /* Hack for restoring text mode -- fixed elsewhere */
    usleep(100000);
#endif
}

/* Write flat panel registers */
static void RADEONRestoreFPRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    unsigned long  tmp;

    OUTREG(RADEON_FP_CRTC_H_TOTAL_DISP, restore->fp_crtc_h_total_disp);
    OUTREG(RADEON_FP_CRTC_V_TOTAL_DISP, restore->fp_crtc_v_total_disp);
    OUTREG(RADEON_FP_H_SYNC_STRT_WID,   restore->fp_h_sync_strt_wid);
    OUTREG(RADEON_FP_V_SYNC_STRT_WID,   restore->fp_v_sync_strt_wid);
    OUTREG(RADEON_TMDS_PLL_CNTL,        restore->tmds_pll_cntl);
    OUTREG(RADEON_TMDS_TRANSMITTER_CNTL,restore->tmds_transmitter_cntl);
    OUTREG(RADEON_FP_HORZ_STRETCH,      restore->fp_horz_stretch);
    OUTREG(RADEON_FP_VERT_STRETCH,      restore->fp_vert_stretch);
    OUTREG(RADEON_FP_GEN_CNTL,          restore->fp_gen_cntl);

    /* old AIW Radeon has some BIOS initialization problem
     * with display buffer underflow, only occurs to DFP
     */
    if (!info->HasCRTC2)
	OUTREG(RADEON_GRPH_BUFFER_CNTL,
	       INREG(RADEON_GRPH_BUFFER_CNTL) & ~0x7f0000);

    if (info->IsMobility) {
	OUTREG(RADEON_BIOS_4_SCRATCH, restore->bios_4_scratch);
	OUTREG(RADEON_BIOS_5_SCRATCH, restore->bios_5_scratch);
	OUTREG(RADEON_BIOS_6_SCRATCH, restore->bios_6_scratch);
    }

    if (info->DisplayType != MT_DFP) {
	unsigned long tmpPixclksCntl = INPLL(pScrn, RADEON_PIXCLKS_CNTL);

	if (info->IsMobility || info->IsIGP) {
	    /* Asic bug, when turning off LVDS_ON, we have to make sure
	       RADEON_PIXCLK_LVDS_ALWAYS_ON bit is off
	    */
	    if (!(restore->lvds_gen_cntl & RADEON_LVDS_ON)) {
		OUTPLLP(pScrn, RADEON_PIXCLKS_CNTL, 0, ~RADEON_PIXCLK_LVDS_ALWAYS_ONb);
	    }
	}

	tmp = INREG(RADEON_LVDS_GEN_CNTL);
	if ((tmp & (RADEON_LVDS_ON | RADEON_LVDS_BLON)) ==
	    (restore->lvds_gen_cntl & (RADEON_LVDS_ON | RADEON_LVDS_BLON))) {
	    OUTREG(RADEON_LVDS_GEN_CNTL, restore->lvds_gen_cntl);
	} else {
	    if (restore->lvds_gen_cntl & (RADEON_LVDS_ON | RADEON_LVDS_BLON)) {
		usleep(RADEONPTR(pScrn)->PanelPwrDly * 1000);
		OUTREG(RADEON_LVDS_GEN_CNTL, restore->lvds_gen_cntl);
	    } else {
		OUTREG(RADEON_LVDS_GEN_CNTL,
		       restore->lvds_gen_cntl | RADEON_LVDS_BLON);
		usleep(RADEONPTR(pScrn)->PanelPwrDly * 1000);
		OUTREG(RADEON_LVDS_GEN_CNTL, restore->lvds_gen_cntl);
	    }
	}

	if (info->IsMobility || info->IsIGP) {
	    if (!(restore->lvds_gen_cntl & RADEON_LVDS_ON)) {
		OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmpPixclksCntl);
	    }
	}
    }
}

static void RADEONPLLWaitForReadUpdateComplete(ScrnInfoPtr pScrn)
{
    int i = 0;

    /* FIXME: Certain revisions of R300 can't recover here.  Not sure of
       the cause yet, but this workaround will mask the problem for now.
       Other chips usually will pass at the very first test, so the
       workaround shouldn't have any effect on them. */
    for (i = 0;
	 (i < 10000 &&
	  INPLL(pScrn, RADEON_PPLL_REF_DIV) & RADEON_PPLL_ATOMIC_UPDATE_R);
	 i++);
}

static void RADEONPLLWriteUpdate(ScrnInfoPtr pScrn)
{
    while (INPLL(pScrn, RADEON_PPLL_REF_DIV) & RADEON_PPLL_ATOMIC_UPDATE_R);

    OUTPLLP(pScrn, RADEON_PPLL_REF_DIV,
	    RADEON_PPLL_ATOMIC_UPDATE_W,
	    ~(RADEON_PPLL_ATOMIC_UPDATE_W));
}

static void RADEONPLL2WaitForReadUpdateComplete(ScrnInfoPtr pScrn)
{
    int i = 0;

    /* FIXME: Certain revisions of R300 can't recover here.  Not sure of
       the cause yet, but this workaround will mask the problem for now.
       Other chips usually will pass at the very first test, so the
       workaround shouldn't have any effect on them. */
    for (i = 0;
	 (i < 10000 &&
	  INPLL(pScrn, RADEON_P2PLL_REF_DIV) & RADEON_P2PLL_ATOMIC_UPDATE_R);
	 i++);
}

static void RADEONPLL2WriteUpdate(ScrnInfoPtr pScrn)
{
    while (INPLL(pScrn, RADEON_P2PLL_REF_DIV) & RADEON_P2PLL_ATOMIC_UPDATE_R);

    OUTPLLP(pScrn, RADEON_P2PLL_REF_DIV,
	    RADEON_P2PLL_ATOMIC_UPDATE_W,
	    ~(RADEON_P2PLL_ATOMIC_UPDATE_W));
}

/* Write PLL registers */
static void RADEONRestorePLLRegisters(ScrnInfoPtr pScrn,
				      RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (info->IsMobility) {
        /* A temporal workaround for the occational blanking on certain laptop panels.
           This appears to related to the PLL divider registers (fail to lock?).
	   It occurs even when all dividers are the same with their old settings.
           In this case we really don't need to fiddle with PLL registers.
           By doing this we can avoid the blanking problem with some panels.
        */
        if ((restore->ppll_ref_div == (INPLL(pScrn, RADEON_PPLL_REF_DIV) & RADEON_PPLL_REF_DIV_MASK)) &&
	    (restore->ppll_div_3 == (INPLL(pScrn, RADEON_PPLL_DIV_3) & 
				     (RADEON_PPLL_POST3_DIV_MASK | RADEON_PPLL_FB3_DIV_MASK)))) {
	    OUTREGP(RADEON_CLOCK_CNTL_INDEX,
		    RADEON_PLL_DIV_SEL,
		    ~(RADEON_PLL_DIV_SEL));
	    RADEONPllErrataAfterIndex(info);
	    return;
	}
    }

    OUTPLLP(pScrn, RADEON_VCLK_ECP_CNTL,
	    RADEON_VCLK_SRC_SEL_CPUCLK,
	    ~(RADEON_VCLK_SRC_SEL_MASK));

    OUTPLLP(pScrn,
	    RADEON_PPLL_CNTL,
	    RADEON_PPLL_RESET
	    | RADEON_PPLL_ATOMIC_UPDATE_EN
	    | RADEON_PPLL_VGA_ATOMIC_UPDATE_EN,
	    ~(RADEON_PPLL_RESET
	      | RADEON_PPLL_ATOMIC_UPDATE_EN
	      | RADEON_PPLL_VGA_ATOMIC_UPDATE_EN));

    OUTREGP(RADEON_CLOCK_CNTL_INDEX,
	    RADEON_PLL_DIV_SEL,
	    ~(RADEON_PLL_DIV_SEL));
    RADEONPllErrataAfterIndex(info);

    if (IS_R300_VARIANT ||
	(info->ChipFamily == CHIP_FAMILY_RS300)) {
	if (restore->ppll_ref_div & R300_PPLL_REF_DIV_ACC_MASK) {
	    /* When restoring console mode, use saved PPLL_REF_DIV
	     * setting.
	     */
	    OUTPLLP(pScrn, RADEON_PPLL_REF_DIV,
		    restore->ppll_ref_div,
		    0);
	} else {
	    /* R300 uses ref_div_acc field as real ref divider */
	    OUTPLLP(pScrn, RADEON_PPLL_REF_DIV,
		    (restore->ppll_ref_div << R300_PPLL_REF_DIV_ACC_SHIFT),
		    ~R300_PPLL_REF_DIV_ACC_MASK);
	}
    } else {
	OUTPLLP(pScrn, RADEON_PPLL_REF_DIV,
		restore->ppll_ref_div,
		~RADEON_PPLL_REF_DIV_MASK);
    }

    OUTPLLP(pScrn, RADEON_PPLL_DIV_3,
	    restore->ppll_div_3,
	    ~RADEON_PPLL_FB3_DIV_MASK);

    OUTPLLP(pScrn, RADEON_PPLL_DIV_3,
	    restore->ppll_div_3,
	    ~RADEON_PPLL_POST3_DIV_MASK);

    RADEONPLLWriteUpdate(pScrn);
    RADEONPLLWaitForReadUpdateComplete(pScrn);

    OUTPLL(pScrn, RADEON_HTOTAL_CNTL, restore->htotal_cntl);

    OUTPLLP(pScrn, RADEON_PPLL_CNTL,
	    0,
	    ~(RADEON_PPLL_RESET
	      | RADEON_PPLL_SLEEP
	      | RADEON_PPLL_ATOMIC_UPDATE_EN
	      | RADEON_PPLL_VGA_ATOMIC_UPDATE_EN));

    RADEONTRACE(("Wrote: 0x%08x 0x%08x 0x%08lx (0x%08x)\n",
	       restore->ppll_ref_div,
	       restore->ppll_div_3,
	       restore->htotal_cntl,
	       INPLL(pScrn, RADEON_PPLL_CNTL)));
    RADEONTRACE(("Wrote: rd=%d, fd=%d, pd=%d\n",
	       restore->ppll_ref_div & RADEON_PPLL_REF_DIV_MASK,
	       restore->ppll_div_3 & RADEON_PPLL_FB3_DIV_MASK,
	       (restore->ppll_div_3 & RADEON_PPLL_POST3_DIV_MASK) >> 16));

    usleep(50000); /* Let the clock to lock */

    OUTPLLP(pScrn, RADEON_VCLK_ECP_CNTL,
	    RADEON_VCLK_SRC_SEL_PPLLCLK,
	    ~(RADEON_VCLK_SRC_SEL_MASK));
}


/* Write PLL2 registers */
static void RADEONRestorePLL2Registers(ScrnInfoPtr pScrn,
				       RADEONSavePtr restore)
{
    OUTPLLP(pScrn, RADEON_PIXCLKS_CNTL,
	    RADEON_PIX2CLK_SRC_SEL_CPUCLK,
	    ~(RADEON_PIX2CLK_SRC_SEL_MASK));

    OUTPLLP(pScrn,
	    RADEON_P2PLL_CNTL,
	    RADEON_P2PLL_RESET
	    | RADEON_P2PLL_ATOMIC_UPDATE_EN
	    | RADEON_P2PLL_VGA_ATOMIC_UPDATE_EN,
	    ~(RADEON_P2PLL_RESET
	      | RADEON_P2PLL_ATOMIC_UPDATE_EN
	      | RADEON_P2PLL_VGA_ATOMIC_UPDATE_EN));

    OUTPLLP(pScrn, RADEON_P2PLL_REF_DIV,
	    restore->p2pll_ref_div,
	    ~RADEON_P2PLL_REF_DIV_MASK);

    OUTPLLP(pScrn, RADEON_P2PLL_DIV_0,
	    restore->p2pll_div_0,
	    ~RADEON_P2PLL_FB0_DIV_MASK);

    OUTPLLP(pScrn, RADEON_P2PLL_DIV_0,
	    restore->p2pll_div_0,
	    ~RADEON_P2PLL_POST0_DIV_MASK);

    RADEONPLL2WriteUpdate(pScrn);
    RADEONPLL2WaitForReadUpdateComplete(pScrn);

    OUTPLL(pScrn, RADEON_HTOTAL2_CNTL, restore->htotal_cntl2);

    OUTPLLP(pScrn, RADEON_P2PLL_CNTL,
	    0,
	    ~(RADEON_P2PLL_RESET
	      | RADEON_P2PLL_SLEEP
	      | RADEON_P2PLL_ATOMIC_UPDATE_EN
	      | RADEON_P2PLL_VGA_ATOMIC_UPDATE_EN));

    RADEONTRACE(("Wrote: 0x%08lx 0x%08lx 0x%08lx (0x%08x)\n",
	       restore->p2pll_ref_div,
	       restore->p2pll_div_0,
	       restore->htotal_cntl2,
	       INPLL(pScrn, RADEON_P2PLL_CNTL)));
    RADEONTRACE(("Wrote: rd=%ld, fd=%ld, pd=%ld\n",
	       restore->p2pll_ref_div & RADEON_P2PLL_REF_DIV_MASK,
	       restore->p2pll_div_0 & RADEON_P2PLL_FB0_DIV_MASK,
	       (restore->p2pll_div_0 & RADEON_P2PLL_POST0_DIV_MASK) >>16));

    usleep(5000); /* Let the clock to lock */

    OUTPLLP(pScrn, RADEON_PIXCLKS_CNTL,
	    RADEON_PIX2CLK_SRC_SEL_P2PLLCLK,
	    ~(RADEON_PIX2CLK_SRC_SEL_MASK));
}


/* restore original surface info (for fb console). */
static void RADEONRestoreSurfaces(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr      info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    unsigned int surfnr;
    
    for ( surfnr = 0; surfnr < 8; surfnr++ ) {
	OUTREG(RADEON_SURFACE0_INFO + 16 * surfnr, restore->surfaces[surfnr][0]);
	OUTREG(RADEON_SURFACE0_LOWER_BOUND + 16 * surfnr, restore->surfaces[surfnr][1]);
	OUTREG(RADEON_SURFACE0_UPPER_BOUND + 16 * surfnr, restore->surfaces[surfnr][2]);
    }
}

/* save original surface info (for fb console). */
static void RADEONSaveSurfaces(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr      info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    unsigned int surfnr;
    
    for ( surfnr = 0; surfnr < 8; surfnr++ ) {
	save->surfaces[surfnr][0] = INREG(RADEON_SURFACE0_INFO + 16 * surfnr);
	save->surfaces[surfnr][1] = INREG(RADEON_SURFACE0_LOWER_BOUND + 16 * surfnr);
	save->surfaces[surfnr][2] = INREG(RADEON_SURFACE0_UPPER_BOUND + 16 * surfnr);
    }
}

void RADEONChangeSurfaces(ScrnInfoPtr pScrn)
{
   /* the idea here is to only set up front buffer as tiled, and back/depth buffer when needed.
      Everything else is left as untiled. This means we need to use eplicit src/dst pitch control
      when blitting, based on the src/target address, and can no longer use a default offset.
      But OTOH we don't need to dynamically change surfaces (for xv for instance), and some
      ugly offset / fb reservation (cursor) is gone. And as a bonus, everything actually works...
      For simplicity, just always update everything (just let the ioctl fail - could do better).
      All surface addresses are relative to RADEON_MC_FB_LOCATION */
  
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    int cpp = info->CurrentLayout.pixel_bytes;
    /* depth/front/back pitch must be identical (and the same as displayWidth) */
    int width_bytes = pScrn->displayWidth * cpp;
    int bufferSize = ((((pScrn->virtualY + 15) & ~15) * width_bytes
        + RADEON_BUFFER_ALIGN) & ~RADEON_BUFFER_ALIGN);
    unsigned int color_pattern, swap_pattern;

    if (!info->allowColorTiling)
	return;

    swap_pattern = 0;
#if X_BYTE_ORDER == X_BIG_ENDIAN
    switch (pScrn->bitsPerPixel) {
    case 16:
	swap_pattern = RADEON_SURF_AP0_SWP_16BPP | RADEON_SURF_AP1_SWP_16BPP;
	break;

    case 32:
	swap_pattern = RADEON_SURF_AP0_SWP_32BPP | RADEON_SURF_AP1_SWP_32BPP;
	break;
    }
#endif
    if (info->ChipFamily < CHIP_FAMILY_R200) {
	color_pattern = RADEON_SURF_TILE_COLOR_MACRO;
    } else if (IS_R300_VARIANT) {
       color_pattern = R300_SURF_TILE_COLOR_MACRO;
    } else {
	color_pattern = R200_SURF_TILE_COLOR_MACRO;
    }   
#ifdef XF86DRI
    if (info->directRenderingInited) {
	drmRadeonSurfaceFree drmsurffree;
	drmRadeonSurfaceAlloc drmsurfalloc;
	int retvalue;
	int depthCpp = (info->depthBits - 8) / 4;
	int depth_width_bytes = pScrn->displayWidth * depthCpp;
	int depthBufferSize = ((((pScrn->virtualY + 15) & ~15) * depth_width_bytes
				+ RADEON_BUFFER_ALIGN) & ~RADEON_BUFFER_ALIGN);
	unsigned int depth_pattern;

	drmsurffree.address = info->frontOffset;
	retvalue = drmCommandWrite(info->drmFD, DRM_RADEON_SURF_FREE,
	    &drmsurffree, sizeof(drmsurffree));

	if ((info->ChipFamily != CHIP_FAMILY_RV100) || 
	    (info->ChipFamily != CHIP_FAMILY_RS100) ||
	    (info->ChipFamily != CHIP_FAMILY_RS200)) {
	    drmsurffree.address = info->depthOffset;
	    retvalue = drmCommandWrite(info->drmFD, DRM_RADEON_SURF_FREE,
		&drmsurffree, sizeof(drmsurffree));
	}

	if (!info->noBackBuffer) {
	    drmsurffree.address = info->backOffset;
	    retvalue = drmCommandWrite(info->drmFD, DRM_RADEON_SURF_FREE,
		&drmsurffree, sizeof(drmsurffree));
	}

	drmsurfalloc.size = bufferSize;
	drmsurfalloc.address = info->frontOffset;
	drmsurfalloc.flags = swap_pattern;

	if (info->tilingEnabled) {
	    if (IS_R300_VARIANT)
		drmsurfalloc.flags |= (width_bytes / 8) | color_pattern;
	    else
		drmsurfalloc.flags |= (width_bytes / 16) | color_pattern;
	}
	retvalue = drmCommandWrite(info->drmFD, DRM_RADEON_SURF_ALLOC,
				   &drmsurfalloc, sizeof(drmsurfalloc));
	if (retvalue < 0)
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "drm: could not allocate surface for front buffer!\n");
	
	if ((info->have3DWindows) && (!info->noBackBuffer)) {
	    drmsurfalloc.address = info->backOffset;
	    retvalue = drmCommandWrite(info->drmFD, DRM_RADEON_SURF_ALLOC,
				       &drmsurfalloc, sizeof(drmsurfalloc));
	    if (retvalue < 0)
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "drm: could not allocate surface for back buffer!\n");
	}

	if (info->ChipFamily < CHIP_FAMILY_R200) {
	    if (depthCpp == 2)
		depth_pattern = RADEON_SURF_TILE_DEPTH_16BPP;
	    else
		depth_pattern = RADEON_SURF_TILE_DEPTH_32BPP;
	} else if (IS_R300_VARIANT) {
	    if (depthCpp == 2)
		depth_pattern = R300_SURF_TILE_COLOR_MACRO;
	    else
		depth_pattern = R300_SURF_TILE_COLOR_MACRO | R300_SURF_TILE_DEPTH_32BPP;
	} else {
	    if (depthCpp == 2)
		depth_pattern = R200_SURF_TILE_DEPTH_16BPP;
	    else
		depth_pattern = R200_SURF_TILE_DEPTH_32BPP;
	}

	/* rv100 and probably the derivative igps don't have depth tiling on all the time? */
	if (info->have3DWindows && ((info->ChipFamily != CHIP_FAMILY_RV100) || 
	    (info->ChipFamily != CHIP_FAMILY_RS100) ||
	    (info->ChipFamily != CHIP_FAMILY_RS200))) {
	    drmRadeonSurfaceAlloc drmsurfalloc;
	    drmsurfalloc.size = depthBufferSize;
	    drmsurfalloc.address = info->depthOffset;
            if (IS_R300_VARIANT)
                drmsurfalloc.flags = swap_pattern | (depth_width_bytes / 8) | depth_pattern;
            else
                drmsurfalloc.flags = swap_pattern | (depth_width_bytes / 16) | depth_pattern;
	    retvalue = drmCommandWrite(info->drmFD, DRM_RADEON_SURF_ALLOC,
		&drmsurfalloc, sizeof(drmsurfalloc));
	    if (retvalue < 0)
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "drm: could not allocate surface for depth buffer!\n");
	}
    }
    else
#endif
    {
	unsigned int surf_info = swap_pattern;
	unsigned char *RADEONMMIO = info->MMIO;
	/* we don't need anything like WaitForFifo, no? */
	if (!info->IsSecondary) {
	    if (info->tilingEnabled) {
		if (IS_R300_VARIANT)
		   surf_info |= (width_bytes / 8) | color_pattern;
		else
		   surf_info |= (width_bytes / 16) | color_pattern;
	    }
	    OUTREG(RADEON_SURFACE0_INFO, surf_info);
	    OUTREG(RADEON_SURFACE0_LOWER_BOUND, 0);
	    OUTREG(RADEON_SURFACE0_UPPER_BOUND, bufferSize - 1);
/*	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"surface0 set to %x, LB 0x%x UB 0x%x\n",
		surf_info, 0, bufferSize - 1024);*/
	}
    }

    /* Update surface images */
    RADEONSaveSurfaces(pScrn, &info->ModeReg);
}

#if 0
/* Write palette data */
static void RADEONRestorePalette(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int            i;

    if (!restore->palette_valid) return;

    PAL_SELECT(1);
    OUTPAL_START(0);
    for (i = 0; i < 256; i++) {
	RADEONWaitForFifo(pScrn, 32); /* delay */
	OUTPAL_NEXT_CARD32(restore->palette2[i]);
    }

    PAL_SELECT(0);
    OUTPAL_START(0);
    for (i = 0; i < 256; i++) {
	RADEONWaitForFifo(pScrn, 32); /* delay */
	OUTPAL_NEXT_CARD32(restore->palette[i]);
    }
}
#endif

/* Write out state to define a new video mode */
static void RADEONRestoreMode(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr      info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    static RADEONSaveRec  restore0;

    RADEONTRACE(("RADEONRestoreMode()\n"));

    /* For Non-dual head card, we don't have private field in the Entity */
    if (!info->HasCRTC2) {
	RADEONRestoreMemMapRegisters(pScrn, restore);
	RADEONRestoreCommonRegisters(pScrn, restore);
	RADEONRestoreCrtcRegisters(pScrn, restore);
	RADEONRestoreFPRegisters(pScrn, restore);
	RADEONRestorePLLRegisters(pScrn, restore);
	return;
    }

    RADEONTRACE(("RADEONRestoreMode(%p)\n", restore));

    /* When changing mode with Dual-head card, care must be taken for
     * the special order in setting registers. CRTC2 has to be set
     * before changing CRTC_EXT register.  In the dual-head setup, X
     * server calls this routine twice with primary and secondary pScrn
     * pointers respectively. The calls can come with different
     * order. Regardless the order of X server issuing the calls, we
     * have to ensure we set registers in the right order!!!  Otherwise
     * we may get a blank screen.
     *
     * We always restore MemMap first, the saverec should be up to date
     * in all cases
     */
    if (info->IsSecondary) {
	RADEONRestoreMemMapRegisters(pScrn, restore);
	RADEONRestoreCommonRegisters(pScrn, restore);
	RADEONRestoreCrtc2Registers(pScrn, restore);
	RADEONRestorePLL2Registers(pScrn, restore);

	if (info->IsSwitching)
	    return;

	pRADEONEnt->IsSecondaryRestored = TRUE;

	if (pRADEONEnt->RestorePrimary) {
	    pRADEONEnt->RestorePrimary = FALSE;

	    RADEONRestoreCrtcRegisters(pScrn, &restore0);
	    RADEONRestoreFPRegisters(pScrn, &restore0);
	    RADEONRestorePLLRegisters(pScrn, &restore0);
	    pRADEONEnt->IsSecondaryRestored = FALSE;
	}
    } else {
	RADEONRestoreMemMapRegisters(pScrn, restore);
	RADEONRestoreCommonRegisters(pScrn, restore);
	if (info->MergedFB) {
	    RADEONRestoreCrtc2Registers(pScrn, restore);
	    RADEONRestorePLL2Registers(pScrn, restore);
	}

	if (!pRADEONEnt->HasSecondary || pRADEONEnt->IsSecondaryRestored ||
	    info->IsSwitching) {
	    pRADEONEnt->IsSecondaryRestored = FALSE;

	    RADEONRestoreCrtcRegisters(pScrn, restore);
	    RADEONRestoreFPRegisters(pScrn, restore);
	    RADEONRestorePLLRegisters(pScrn, restore);
	} else {
	    memcpy(&restore0, restore, sizeof(restore0));
	    pRADEONEnt->RestorePrimary = TRUE;
	}
    }

#if 0
    RADEONRestorePalette(pScrn, &info->SavedReg);
#endif
}

/* Read memory map */
static void RADEONSaveMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    save->mc_fb_location     = INREG(RADEON_MC_FB_LOCATION);
    save->mc_agp_location    = INREG(RADEON_MC_AGP_LOCATION);
    save->display_base_addr  = INREG(RADEON_DISPLAY_BASE_ADDR);
    save->display2_base_addr = INREG(RADEON_DISPLAY2_BASE_ADDR);
    save->ov0_base_addr      = INREG(RADEON_OV0_BASE_ADDR);
}

/* Read common registers */
static void RADEONSaveCommonRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    save->ovr_clr            = INREG(RADEON_OVR_CLR);
    save->ovr_wid_left_right = INREG(RADEON_OVR_WID_LEFT_RIGHT);
    save->ovr_wid_top_bottom = INREG(RADEON_OVR_WID_TOP_BOTTOM);
    save->ov0_scale_cntl     = INREG(RADEON_OV0_SCALE_CNTL);
    save->subpic_cntl        = INREG(RADEON_SUBPIC_CNTL);
    save->viph_control       = INREG(RADEON_VIPH_CONTROL);
    save->i2c_cntl_1         = INREG(RADEON_I2C_CNTL_1);
    save->gen_int_cntl       = INREG(RADEON_GEN_INT_CNTL);
    save->cap0_trig_cntl     = INREG(RADEON_CAP0_TRIG_CNTL);
    save->cap1_trig_cntl     = INREG(RADEON_CAP1_TRIG_CNTL);
    save->bus_cntl           = INREG(RADEON_BUS_CNTL);
    save->surface_cntl	     = INREG(RADEON_SURFACE_CNTL);
    save->grph_buffer_cntl   = INREG(RADEON_GRPH_BUFFER_CNTL);
    save->grph2_buffer_cntl  = INREG(RADEON_GRPH2_BUFFER_CNTL);
}

/* Read miscellaneous registers which might be destroyed by an fbdevHW call */
static void RADEONSaveFBDevRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
#ifdef XF86DRI
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    /* Save register for vertical blank interrupts */
    if (info->irq) {
	save->gen_int_cntl = INREG(RADEON_GEN_INT_CNTL);
    }

    /* Save registers for page flipping */
    if (info->allowPageFlip) {
	save->crtc_offset_cntl = INREG(RADEON_CRTC_OFFSET_CNTL);
	if (info->HasCRTC2) {
	    save->crtc2_offset_cntl = INREG(RADEON_CRTC2_OFFSET_CNTL);
	}
    }
#endif
}

/* Read CRTC registers */
static void RADEONSaveCrtcRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    save->crtc_gen_cntl        = INREG(RADEON_CRTC_GEN_CNTL);
    save->crtc_ext_cntl        = INREG(RADEON_CRTC_EXT_CNTL);
    save->dac_cntl             = INREG(RADEON_DAC_CNTL);
    save->crtc_h_total_disp    = INREG(RADEON_CRTC_H_TOTAL_DISP);
    save->crtc_h_sync_strt_wid = INREG(RADEON_CRTC_H_SYNC_STRT_WID);
    save->crtc_v_total_disp    = INREG(RADEON_CRTC_V_TOTAL_DISP);
    save->crtc_v_sync_strt_wid = INREG(RADEON_CRTC_V_SYNC_STRT_WID);
    save->crtc_offset          = INREG(RADEON_CRTC_OFFSET);
    save->crtc_offset_cntl     = INREG(RADEON_CRTC_OFFSET_CNTL);
    save->crtc_pitch           = INREG(RADEON_CRTC_PITCH);
    save->disp_merge_cntl      = INREG(RADEON_DISP_MERGE_CNTL);
    save->crtc_more_cntl       = INREG(RADEON_CRTC_MORE_CNTL);

    if (info->IsDellServer) {
	save->tv_dac_cntl      = INREG(RADEON_TV_DAC_CNTL);
	save->dac2_cntl        = INREG(RADEON_DAC_CNTL2);
	save->disp_hw_debug    = INREG (RADEON_DISP_HW_DEBUG);
	save->crtc2_gen_cntl   = INREG(RADEON_CRTC2_GEN_CNTL);
    }
}

/* Read flat panel registers */
static void RADEONSaveFPRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    save->fp_crtc_h_total_disp = INREG(RADEON_FP_CRTC_H_TOTAL_DISP);
    save->fp_crtc_v_total_disp = INREG(RADEON_FP_CRTC_V_TOTAL_DISP);
    save->fp_gen_cntl          = INREG(RADEON_FP_GEN_CNTL);
    save->fp_h_sync_strt_wid   = INREG(RADEON_FP_H_SYNC_STRT_WID);
    save->fp_horz_stretch      = INREG(RADEON_FP_HORZ_STRETCH);
    save->fp_v_sync_strt_wid   = INREG(RADEON_FP_V_SYNC_STRT_WID);
    save->fp_vert_stretch      = INREG(RADEON_FP_VERT_STRETCH);
    save->lvds_gen_cntl        = INREG(RADEON_LVDS_GEN_CNTL);
    save->lvds_pll_cntl        = INREG(RADEON_LVDS_PLL_CNTL);
    save->tmds_pll_cntl        = INREG(RADEON_TMDS_PLL_CNTL);
    save->tmds_transmitter_cntl= INREG(RADEON_TMDS_TRANSMITTER_CNTL);
    save->bios_4_scratch       = INREG(RADEON_BIOS_4_SCRATCH);
    save->bios_5_scratch       = INREG(RADEON_BIOS_5_SCRATCH);
    save->bios_6_scratch       = INREG(RADEON_BIOS_6_SCRATCH);

    if (info->ChipFamily == CHIP_FAMILY_RV280) {
	/* bit 22 of TMDS_PLL_CNTL is read-back inverted */
	save->tmds_pll_cntl ^= (1 << 22);
    }
}

/* Read CRTC2 registers */
static void RADEONSaveCrtc2Registers(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    save->dac2_cntl             = INREG(RADEON_DAC_CNTL2);
    save->disp_output_cntl      = INREG(RADEON_DISP_OUTPUT_CNTL);
    save->disp_hw_debug         = INREG (RADEON_DISP_HW_DEBUG);

    save->crtc2_gen_cntl        = INREG(RADEON_CRTC2_GEN_CNTL);
    save->crtc2_h_total_disp    = INREG(RADEON_CRTC2_H_TOTAL_DISP);
    save->crtc2_h_sync_strt_wid = INREG(RADEON_CRTC2_H_SYNC_STRT_WID);
    save->crtc2_v_total_disp    = INREG(RADEON_CRTC2_V_TOTAL_DISP);
    save->crtc2_v_sync_strt_wid = INREG(RADEON_CRTC2_V_SYNC_STRT_WID);
    save->crtc2_offset          = INREG(RADEON_CRTC2_OFFSET);
    save->crtc2_offset_cntl     = INREG(RADEON_CRTC2_OFFSET_CNTL);
    save->crtc2_pitch           = INREG(RADEON_CRTC2_PITCH);

    save->fp2_h_sync_strt_wid   = INREG (RADEON_FP_H2_SYNC_STRT_WID);
    save->fp2_v_sync_strt_wid   = INREG (RADEON_FP_V2_SYNC_STRT_WID);
    save->fp2_gen_cntl          = INREG (RADEON_FP2_GEN_CNTL);
    save->disp2_merge_cntl      = INREG(RADEON_DISP2_MERGE_CNTL);
}

/* Read PLL registers */
static void RADEONSavePLLRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    save->ppll_ref_div = INPLL(pScrn, RADEON_PPLL_REF_DIV);
    save->ppll_div_3   = INPLL(pScrn, RADEON_PPLL_DIV_3);
    save->htotal_cntl  = INPLL(pScrn, RADEON_HTOTAL_CNTL);

    RADEONTRACE(("Read: 0x%08x 0x%08x 0x%08lx\n",
		 save->ppll_ref_div,
		 save->ppll_div_3,
		 save->htotal_cntl));
    RADEONTRACE(("Read: rd=%d, fd=%d, pd=%d\n",
		 save->ppll_ref_div & RADEON_PPLL_REF_DIV_MASK,
		 save->ppll_div_3 & RADEON_PPLL_FB3_DIV_MASK,
		 (save->ppll_div_3 & RADEON_PPLL_POST3_DIV_MASK) >> 16));
}

/* Read PLL registers */
static void RADEONSavePLL2Registers(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    save->p2pll_ref_div = INPLL(pScrn, RADEON_P2PLL_REF_DIV);
    save->p2pll_div_0   = INPLL(pScrn, RADEON_P2PLL_DIV_0);
    save->htotal_cntl2  = INPLL(pScrn, RADEON_HTOTAL2_CNTL);

    RADEONTRACE(("Read: 0x%08lx 0x%08lx 0x%08lx\n",
		 save->p2pll_ref_div,
		 save->p2pll_div_0,
		 save->htotal_cntl2));
    RADEONTRACE(("Read: rd=%ld, fd=%ld, pd=%ld\n",
		 save->p2pll_ref_div & RADEON_P2PLL_REF_DIV_MASK,
		 save->p2pll_div_0 & RADEON_P2PLL_FB0_DIV_MASK,
		 (save->p2pll_div_0 & RADEON_P2PLL_POST0_DIV_MASK) >> 16));
}

/* Read palette data */
static void RADEONSavePalette(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int            i;

#ifdef ENABLE_FLAT_PANEL
    /* Select palette 0 (main CRTC) if using FP-enabled chip */
 /* if (info->Port1 == MT_DFP) PAL_SELECT(1); */
#endif
    PAL_SELECT(1);
    INPAL_START(0);
    for (i = 0; i < 256; i++) save->palette2[i] = INPAL_NEXT();
    PAL_SELECT(0);
    INPAL_START(0);
    for (i = 0; i < 256; i++) save->palette[i] = INPAL_NEXT();
    save->palette_valid = TRUE;
}

/* Save state that defines current video mode */
static void RADEONSaveMode(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    RADEONTRACE(("RADEONSaveMode(%p)\n", save));
    RADEONSaveMemMapRegisters(pScrn, save);
    RADEONSaveCommonRegisters(pScrn, save);
    if (info->IsSecondary) {
	RADEONSaveCrtc2Registers(pScrn, save);
	RADEONSavePLL2Registers(pScrn, save);
    } else {
	RADEONSavePLLRegisters(pScrn, save);
	RADEONSaveCrtcRegisters(pScrn, save);
	RADEONSaveFPRegisters(pScrn, save);

	if (info->MergedFB) {
	    RADEONSaveCrtc2Registers(pScrn, save);
	    RADEONSavePLL2Registers(pScrn, save);
	}
     /* RADEONSavePalette(pScrn, save); */
    }

    RADEONTRACE(("RADEONSaveMode returns %p\n", save));
}

/* Save everything needed to restore the original VC state */
static void RADEONSave(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONSavePtr  save       = &info->SavedReg;

    RADEONTRACE(("RADEONSave\n"));
    if (info->FBDev) {
	RADEONSaveMemMapRegisters(pScrn, save);
	fbdevHWSave(pScrn);
	return;
    }

    if (!info->IsSecondary) {
#ifdef WITH_VGAHW
        if (info->VGAAccess) {
           vgaHWPtr hwp = VGAHWPTR(pScrn);

            vgaHWUnlock(hwp);
# if defined(__powerpc__)
           /* temporary hack to prevent crashing on PowerMacs when trying to
            * read VGA fonts and colormap, will find a better solution
            * in the future. TODO: Check if there's actually some VGA stuff
            * setup in the card at all !!
            */
           vgaHWSave(pScrn, &hwp->SavedReg, VGA_SR_MODE); /* Save mode only */
# else
           /* Save mode * & fonts & cmap */
           vgaHWSave(pScrn, &hwp->SavedReg, VGA_SR_MODE | VGA_SR_FONTS);
# endif
           vgaHWLock(hwp);
       }
#endif
	save->dp_datatype      = INREG(RADEON_DP_DATATYPE);
	save->rbbm_soft_reset  = INREG(RADEON_RBBM_SOFT_RESET);
	save->clock_cntl_index = INREG(RADEON_CLOCK_CNTL_INDEX);
	RADEONPllErrataAfterIndex(info);
    }

    RADEONSaveMode(pScrn, save);
    if (!info->IsSecondary)
	RADEONSaveSurfaces(pScrn, save);
}

/* Restore the original (text) mode */
static void RADEONRestore(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONSavePtr  restore    = &info->SavedReg;

    RADEONTRACE(("RADEONRestore\n"));

#if X_BYTE_ORDER == X_BIG_ENDIAN
    RADEONWaitForFifo(pScrn, 1);
    OUTREG(RADEON_RBBM_GUICNTL, RADEON_HOST_DATA_SWAP_NONE);
#endif

    if (info->FBDev) {
	fbdevHWRestore(pScrn);
	return;
    }
    RADEONBlank(pScrn);

    OUTREG(RADEON_CLOCK_CNTL_INDEX, restore->clock_cntl_index);
    RADEONPllErrataAfterIndex(info);
    OUTREG(RADEON_RBBM_SOFT_RESET,  restore->rbbm_soft_reset);
    OUTREG(RADEON_DP_DATATYPE,      restore->dp_datatype);
    OUTREG(RADEON_GRPH_BUFFER_CNTL, restore->grph_buffer_cntl);
    OUTREG(RADEON_GRPH2_BUFFER_CNTL, restore->grph2_buffer_cntl);

#if 0
    /* M6 card has trouble restoring text mode for its CRT.
     * This is fixed elsewhere and will be removed in the future.
     */
    if ((xf86IsEntityShared(info->pEnt->index) || info->MergedFB)
	&& info->IsM6)
	OUTREG(RADEON_DAC_CNTL2, restore->dac2_cntl);
#endif

    RADEONRestoreMode(pScrn, restore);
    if (!info->IsSecondary)
	RADEONRestoreSurfaces(pScrn, restore);

#if 0
    /* Temp fix to "solve" VT switch problems.  When switching VTs on
     * some systems, the console can either hang or the fonts can be
     * corrupted.  This hack solves the problem 99% of the time.  A
     * correct fix is being worked on.
     */
    usleep(100000);
#endif

#ifdef WITH_VGAHW
    if (info->VGAAccess) {
       vgaHWPtr hwp = VGAHWPTR(pScrn);
        if (!info->IsSecondary) {
            vgaHWUnlock(hwp);
# if defined(__powerpc__)
           /* Temporary hack to prevent crashing on PowerMacs when trying to
            * write VGA fonts, will find a better solution in the future
            */
           vgaHWRestore(pScrn, &hwp->SavedReg, VGA_SR_MODE );
# else
           vgaHWRestore(pScrn, &hwp->SavedReg, VGA_SR_MODE | VGA_SR_FONTS );
# endif
           vgaHWLock(hwp);
        } else {
            RADEONEntPtr  pRADEONEnt = RADEONEntPriv(pScrn);
           ScrnInfoPtr   pScrn0 = pRADEONEnt->pPrimaryScrn;
            RADEONInfoPtr info0 = RADEONPTR(pScrn0);
           vgaHWPtr      hwp0;

           if (info0->VGAAccess) {
               hwp0 = VGAHWPTR(pScrn0);
               vgaHWUnlock(hwp0);
#if defined(__powerpc__)
               vgaHWRestore(pScrn0, &hwp0->SavedReg, VGA_SR_MODE);
#else
               vgaHWRestore(pScrn0, &hwp0->SavedReg, VGA_SR_MODE | VGA_SR_FONTS );
#endif
               vgaHWLock(hwp0);
           }
       }
    }
#endif
    RADEONUnblank(pScrn);

#if 0
    RADEONWaitForVerticalSync(pScrn);
#endif
}

/* Define common registers for requested video mode */
static void RADEONInitCommonRegisters(RADEONSavePtr save, RADEONInfoPtr info)
{
    save->ovr_clr            = 0;
    save->ovr_wid_left_right = 0;
    save->ovr_wid_top_bottom = 0;
    save->ov0_scale_cntl     = 0;
    save->subpic_cntl        = 0;
    save->viph_control       = 0;
    save->i2c_cntl_1         = 0;
    save->rbbm_soft_reset    = 0;
    save->cap0_trig_cntl     = 0;
    save->cap1_trig_cntl     = 0;
    save->bus_cntl           = info->BusCntl;
    /*
     * If bursts are enabled, turn on discards
     * Radeon doesn't have write bursts
     */
    if (save->bus_cntl & (RADEON_BUS_READ_BURST))
	save->bus_cntl |= RADEON_BUS_RD_DISCARD_EN;
}


/* Calculate display buffer watermark to prevent buffer underflow */
static void RADEONInitDispBandwidth(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt   = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONInfoPtr  info2 = NULL;

    DisplayModePtr mode1, mode2;

    CARD32 temp, data, mem_trcd, mem_trp, mem_tras, mem_trbs=0;
    float mem_tcas;
    int k1, c;
    CARD32 MemTrcdExtMemCntl[4]     = {1, 2, 3, 4};
    CARD32 MemTrpExtMemCntl[4]      = {1, 2, 3, 4};
    CARD32 MemTrasExtMemCntl[8]     = {1, 2, 3, 4, 5, 6, 7, 8};

    CARD32 MemTrcdMemTimingCntl[8]     = {1, 2, 3, 4, 5, 6, 7, 8};
    CARD32 MemTrpMemTimingCntl[8]      = {1, 2, 3, 4, 5, 6, 7, 8};
    CARD32 MemTrasMemTimingCntl[16]    = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

    float MemTcas[8]  = {0, 1, 2, 3, 0, 1.5, 2.5, 0};
    float MemTcas2[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    float MemTrbs[8]  = {1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5};

    float mem_bw, peak_disp_bw;
    float min_mem_eff = 0.8;
    float sclk_eff, sclk_delay;
    float mc_latency_mclk, mc_latency_sclk, cur_latency_mclk, cur_latency_sclk;
    float disp_latency, disp_latency_overhead, disp_drain_rate, disp_drain_rate2;
    float pix_clk, pix_clk2; /* in MHz */
    int cur_size = 16;       /* in octawords */
    int critical_point, critical_point2;
    int stop_req, max_stop_req;
    float read_return_rate, time_disp1_drop_priority;

    /* 
     * Set display0/1 priority up on r3/4xx in the memory controller for 
     * high res modes if the user specifies HIGH for displaypriority 
     * option.
     */
    if ((info->DispPriority == 2) && IS_R300_VARIANT) {
        CARD32 mc_init_misc_lat_timer = INREG(R300_MC_INIT_MISC_LAT_TIMER);
	if (info->MergedFB || pRADEONEnt->HasSecondary) {
	    mc_init_misc_lat_timer |= 0x1100; /* display 0 and 1 */
	} else {
	    mc_init_misc_lat_timer |= 0x0100; /* display 0 only */
	}
	OUTREG(R300_MC_INIT_MISC_LAT_TIMER, mc_init_misc_lat_timer);
    }


    /* R420 and RV410 family not supported yet */
    if (info->ChipFamily == CHIP_FAMILY_R420 || info->ChipFamily == CHIP_FAMILY_RV410) return; 

    if (pRADEONEnt->pSecondaryScrn) {
	if (info->IsSecondary) return;
	info2 = RADEONPTR(pRADEONEnt->pSecondaryScrn);
    }  else if (info->MergedFB) info2 = info;

    /*
     * Determine if there is enough bandwidth for current display mode
     */
    mem_bw = info->mclk * (info->RamWidth / 8) * (info->IsDDR ? 2 : 1);

    mode1 = info->CurrentLayout.mode;
    if (info->MergedFB) {
	mode1 = ((RADEONMergedDisplayModePtr)info->CurrentLayout.mode->Private)->CRT1;
	mode2 = ((RADEONMergedDisplayModePtr)info->CurrentLayout.mode->Private)->CRT2;
    } else if ((pRADEONEnt->HasSecondary) && info2) {
	mode2 = info2->CurrentLayout.mode;
    } else {
	mode2 = NULL;
    }

    pix_clk = mode1->Clock/1000.0;
    if (mode2)
	pix_clk2 = mode2->Clock/1000.0;
    else
	pix_clk2 = 0;

    peak_disp_bw = (pix_clk * info->CurrentLayout.pixel_bytes);
    if (info2) 
	peak_disp_bw +=	(pix_clk2 * info2->CurrentLayout.pixel_bytes);

    if (peak_disp_bw >= mem_bw * min_mem_eff) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
		   "You may not have enough display bandwidth for current mode\n"
		   "If you have flickering problem, try to lower resolution, refresh rate, or color depth\n");
    } 

    /*  CRTC1
        Set GRPH_BUFFER_CNTL register using h/w defined optimal values.
    	GRPH_STOP_REQ <= MIN[ 0x7C, (CRTC_H_DISP + 1) * (bit depth) / 0x10 ]
    */
    stop_req = mode1->HDisplay * info->CurrentLayout.pixel_bytes / 16;

    /* setup Max GRPH_STOP_REQ default value */
    if (IS_RV100_VARIANT)
	max_stop_req = 0x5c;
    else
	max_stop_req  = 0x7c;
    if (stop_req > max_stop_req)
	stop_req = max_stop_req;
      
    /*  Get values from the EXT_MEM_CNTL register...converting its contents. */
    temp = INREG(RADEON_MEM_TIMING_CNTL);
    if ((info->ChipFamily == CHIP_FAMILY_RV100) || info->IsIGP) { /* RV100, M6, IGPs */
	mem_trcd      = MemTrcdExtMemCntl[(temp & 0x0c) >> 2];
	mem_trp       = MemTrpExtMemCntl[ (temp & 0x03) >> 0];
	mem_tras      = MemTrasExtMemCntl[(temp & 0x70) >> 4];
    } else { /* RV200 and later */
	mem_trcd      = MemTrcdMemTimingCntl[(temp & 0x07) >> 0];
	mem_trp       = MemTrpMemTimingCntl[ (temp & 0x700) >> 8];
	mem_tras      = MemTrasMemTimingCntl[(temp & 0xf000) >> 12];
    }
    
    /* Get values from the MEM_SDRAM_MODE_REG register...converting its */ 
    temp = INREG(RADEON_MEM_SDRAM_MODE_REG);
    data = (temp & (7<<20)) >> 20;
    if ((info->ChipFamily == CHIP_FAMILY_RV100) || info->IsIGP) { /* RV100, M6, IGPs */
	mem_tcas = MemTcas [data];
    } else {
	mem_tcas = MemTcas2 [data];
    }

    if (IS_R300_VARIANT) {

	/* on the R300, Tcas is included in Trbs.
	*/
	temp = INREG(RADEON_MEM_CNTL);
	data = (R300_MEM_NUM_CHANNELS_MASK & temp);
	if (data == 1) {
	    if (R300_MEM_USE_CD_CH_ONLY & temp) {
		temp  = INREG(R300_MC_IND_INDEX);
		temp &= ~R300_MC_IND_ADDR_MASK;
		temp |= R300_MC_READ_CNTL_CD_mcind;
		OUTREG(R300_MC_IND_INDEX, temp);
		temp  = INREG(R300_MC_IND_DATA);
		data = (R300_MEM_RBS_POSITION_C_MASK & temp);
	    } else {
		temp = INREG(R300_MC_READ_CNTL_AB);
		data = (R300_MEM_RBS_POSITION_A_MASK & temp);
	    }
	} else {
	    temp = INREG(R300_MC_READ_CNTL_AB);
	    data = (R300_MEM_RBS_POSITION_A_MASK & temp);
	}

	mem_trbs = MemTrbs[data];
	mem_tcas += mem_trbs;
    }

    if ((info->ChipFamily == CHIP_FAMILY_RV100) || info->IsIGP) { /* RV100, M6, IGPs */
	/* DDR64 SCLK_EFF = SCLK for analysis */
	sclk_eff = info->sclk;
    } else {
#ifdef XF86DRI
	if (info->directRenderingEnabled)
	    sclk_eff = info->sclk - (info->agpMode * 50.0 / 3.0);
	else
#endif
	    sclk_eff = info->sclk;
    }

    /* Find the memory controller latency for the display client.
    */
    if (IS_R300_VARIANT) {
	/*not enough for R350 ???*/
	/*
	if (!mode2) sclk_delay = 150;
	else {
	    if (info->RamWidth == 256) sclk_delay = 87;
	    else sclk_delay = 97;
	}
	*/
	sclk_delay = 250;
    } else {
	if ((info->ChipFamily == CHIP_FAMILY_RV100) ||
	    info->IsIGP) {
	    if (info->IsDDR) sclk_delay = 41;
	    else sclk_delay = 33;
	} else {
	    if (info->RamWidth == 128) sclk_delay = 57;
	    else sclk_delay = 41;
	}
    }

    mc_latency_sclk = sclk_delay / sclk_eff;
	
    if (info->IsDDR) {
	if (info->RamWidth == 32) {
	    k1 = 40;
	    c  = 3;
	} else {
	    k1 = 20;
	    c  = 1;
	}
    } else {
	k1 = 40;
	c  = 3;
    }
    mc_latency_mclk = ((2.0*mem_trcd + mem_tcas*c + 4.0*mem_tras + 4.0*mem_trp + k1) /
		       info->mclk) + (4.0 / sclk_eff);

    /*
      HW cursor time assuming worst case of full size colour cursor.
    */
    cur_latency_mclk = (mem_trp + MAX(mem_tras, (mem_trcd + 2*(cur_size - (info->IsDDR+1))))) / info->mclk;
    cur_latency_sclk = cur_size / sclk_eff;

    /*
      Find the total latency for the display data.
    */
    disp_latency_overhead = 8.0 / info->sclk;
    mc_latency_mclk = mc_latency_mclk + disp_latency_overhead + cur_latency_mclk;
    mc_latency_sclk = mc_latency_sclk + disp_latency_overhead + cur_latency_sclk;
    disp_latency = MAX(mc_latency_mclk, mc_latency_sclk);

    /*
      Find the drain rate of the display buffer.
    */
    disp_drain_rate = pix_clk / (16.0/info->CurrentLayout.pixel_bytes);
    if (info2)
	disp_drain_rate2 = pix_clk2 / (16.0/info2->CurrentLayout.pixel_bytes);
    else
	disp_drain_rate2 = 0;

    /*
      Find the critical point of the display buffer.
    */
    critical_point= (CARD32)(disp_drain_rate * disp_latency + 0.5); 

    /* ???? */
    /*
    temp = (info->SavedReg.grph_buffer_cntl & RADEON_GRPH_CRITICAL_POINT_MASK) >> RADEON_GRPH_CRITICAL_POINT_SHIFT;
    if (critical_point < temp) critical_point = temp;
    */
    if (info->DispPriority == 2) {
	critical_point = 0;
    }

    /*
      The critical point should never be above max_stop_req-4.  Setting
      GRPH_CRITICAL_CNTL = 0 will thus force high priority all the time.
    */
    if (max_stop_req - critical_point < 4) critical_point = 0; 

    if (critical_point == 0 && mode2 && info->ChipFamily == CHIP_FAMILY_R300) {
	/* some R300 cards have problem with this set to 0, when CRTC2 is enabled.*/
	critical_point = 0x10;
    }

    temp = info->SavedReg.grph_buffer_cntl;
    temp &= ~(RADEON_GRPH_STOP_REQ_MASK);
    temp |= (stop_req << RADEON_GRPH_STOP_REQ_SHIFT);
    temp &= ~(RADEON_GRPH_START_REQ_MASK);
    if ((info->ChipFamily == CHIP_FAMILY_R350) &&
	(stop_req > 0x15)) {
	stop_req -= 0x10;
    }
    temp |= (stop_req << RADEON_GRPH_START_REQ_SHIFT);

    temp |= RADEON_GRPH_BUFFER_SIZE;
    temp &= ~(RADEON_GRPH_CRITICAL_CNTL   |
	      RADEON_GRPH_CRITICAL_AT_SOF |
	      RADEON_GRPH_STOP_CNTL);
    /*
      Write the result into the register.
    */
    OUTREG(RADEON_GRPH_BUFFER_CNTL, ((temp & ~RADEON_GRPH_CRITICAL_POINT_MASK) |
				     (critical_point << RADEON_GRPH_CRITICAL_POINT_SHIFT)));

    RADEONTRACE(("GRPH_BUFFER_CNTL from %x to %x\n",
		 (unsigned int)info->SavedReg.grph_buffer_cntl, INREG(RADEON_GRPH_BUFFER_CNTL)));

    if (mode2) {
	stop_req = mode2->HDisplay * info2->CurrentLayout.pixel_bytes / 16;

	if (stop_req > max_stop_req) stop_req = max_stop_req;

	temp = info->SavedReg.grph2_buffer_cntl;
	temp &= ~(RADEON_GRPH_STOP_REQ_MASK);
	temp |= (stop_req << RADEON_GRPH_STOP_REQ_SHIFT);
	temp &= ~(RADEON_GRPH_START_REQ_MASK);
	if ((info->ChipFamily == CHIP_FAMILY_R350) &&
	    (stop_req > 0x15)) {
	    stop_req -= 0x10;
	}
	temp |= (stop_req << RADEON_GRPH_START_REQ_SHIFT);
	temp |= RADEON_GRPH_BUFFER_SIZE;
	temp &= ~(RADEON_GRPH_CRITICAL_CNTL   |
		  RADEON_GRPH_CRITICAL_AT_SOF |
		  RADEON_GRPH_STOP_CNTL);

	if ((info->ChipFamily == CHIP_FAMILY_RS100) || 
	    (info->ChipFamily == CHIP_FAMILY_RS200))
	    critical_point2 = 0;
	else {
	    read_return_rate = MIN(info->sclk, info->mclk*(info->RamWidth*(info->IsDDR+1)/128));
	    time_disp1_drop_priority = critical_point / (read_return_rate - disp_drain_rate);

	    critical_point2 = (CARD32)((disp_latency + time_disp1_drop_priority + 
					disp_latency) * disp_drain_rate2 + 0.5);

	    if (info->DispPriority == 2) {
		critical_point2 = 0;
	    }

	    if (max_stop_req - critical_point2 < 4) critical_point2 = 0;

	}

	if (critical_point2 == 0 && info->ChipFamily == CHIP_FAMILY_R300) {
	    /* some R300 cards have problem with this set to 0 */
	    critical_point2 = 0x10;
	}

	OUTREG(RADEON_GRPH2_BUFFER_CNTL, ((temp & ~RADEON_GRPH_CRITICAL_POINT_MASK) |
					  (critical_point2 << RADEON_GRPH_CRITICAL_POINT_SHIFT)));

	RADEONTRACE(("GRPH2_BUFFER_CNTL from %x to %x\n",
		     (unsigned int)info->SavedReg.grph2_buffer_cntl, INREG(RADEON_GRPH2_BUFFER_CNTL)));
    }
}   

/* Define CRTC registers for requested video mode */
static Bool RADEONInitCrtcRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save,
				  DisplayModePtr mode, RADEONInfoPtr info)
{
    unsigned char *RADEONMMIO = info->MMIO;

    int  format;
    int  hsync_start;
    int  hsync_wid;
    int  vsync_wid;

    switch (info->CurrentLayout.pixel_code) {
    case 4:  format = 1; break;
    case 8:  format = 2; break;
    case 15: format = 3; break;      /*  555 */
    case 16: format = 4; break;      /*  565 */
    case 24: format = 5; break;      /*  RGB */
    case 32: format = 6; break;      /* xRGB */
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Unsupported pixel depth (%d)\n",
		   info->CurrentLayout.bitsPerPixel);
	return FALSE;
    }

    if ((info->DisplayType == MT_DFP) ||
	(info->DisplayType == MT_LCD)) {
	if (mode->Flags & RADEON_USE_RMX) {
#if 0
	    mode->CrtcHDisplay   = info->PanelXRes;
	    mode->CrtcVDisplay   = info->PanelYRes;
#endif
	    mode->CrtcHTotal     = mode->CrtcHDisplay + info->HBlank;
	    mode->CrtcHSyncStart = mode->CrtcHDisplay + info->HOverPlus;
	    mode->CrtcHSyncEnd   = mode->CrtcHSyncStart + info->HSyncWidth;
	    mode->CrtcVTotal     = mode->CrtcVDisplay + info->VBlank;
	    mode->CrtcVSyncStart = mode->CrtcVDisplay + info->VOverPlus;
	    mode->CrtcVSyncEnd   = mode->CrtcVSyncStart + info->VSyncWidth;
	    mode->Clock          = info->DotClock;
	    mode->Flags          = info->Flags | RADEON_USE_RMX;
	}
    }

    save->crtc_gen_cntl = (RADEON_CRTC_EXT_DISP_EN
			   | RADEON_CRTC_EN
			   | (format << 8)
			   | ((mode->Flags & V_DBLSCAN)
			      ? RADEON_CRTC_DBL_SCAN_EN
			      : 0)
			   | ((mode->Flags & V_CSYNC)
			      ? RADEON_CRTC_CSYNC_EN
			      : 0)
			   | ((mode->Flags & V_INTERLACE)
			      ? RADEON_CRTC_INTERLACE_EN
			      : 0));

    /* Don't try to be smart and unconditionally enable the analog output
     * for now as the dodgy code to handle it for the second head doesn't
     * work. This will be correctly fixed when Alex' megapatch gets in that
     * reworks the whole output mapping
     */
#if 0
    if ((info->DisplayType == MT_DFP) ||
	(info->DisplayType == MT_LCD)) {
	save->crtc_ext_cntl = RADEON_VGA_ATI_LINEAR | RADEON_XCRT_CNT_EN;
	save->crtc_gen_cntl &= ~(RADEON_CRTC_DBL_SCAN_EN |
				 RADEON_CRTC_CSYNC_EN |
				 RADEON_CRTC_INTERLACE_EN);
    } else
#endif
    {
	save->crtc_ext_cntl = (RADEON_VGA_ATI_LINEAR |
			       RADEON_XCRT_CNT_EN |
			       RADEON_CRTC_CRT_ON);
    }

    save->dac_cntl = (RADEON_DAC_MASK_ALL
		      | RADEON_DAC_VGA_ADR_EN
		      | (info->dac6bits ? 0 : RADEON_DAC_8BIT_EN));

    save->crtc_h_total_disp = ((((mode->CrtcHTotal / 8) - 1) & 0x3ff)
			       | ((((mode->CrtcHDisplay / 8) - 1) & 0x1ff)
				  << 16));

    hsync_wid = (mode->CrtcHSyncEnd - mode->CrtcHSyncStart) / 8;
    if (!hsync_wid) hsync_wid = 1;
    hsync_start = mode->CrtcHSyncStart - 8;

    save->crtc_h_sync_strt_wid = ((hsync_start & 0x1fff)
				  | ((hsync_wid & 0x3f) << 16)
				  | ((mode->Flags & V_NHSYNC)
				     ? RADEON_CRTC_H_SYNC_POL
				     : 0));

#if 1
				/* This works for double scan mode. */
    save->crtc_v_total_disp = (((mode->CrtcVTotal - 1) & 0xffff)
			       | ((mode->CrtcVDisplay - 1) << 16));
#else
				/* This is what cce/nbmode.c example code
				 * does -- is this correct?
				 */
    save->crtc_v_total_disp = (((mode->CrtcVTotal - 1) & 0xffff)
			       | ((mode->CrtcVDisplay
				   * ((mode->Flags & V_DBLSCAN) ? 2 : 1) - 1)
				  << 16));
#endif

    vsync_wid = mode->CrtcVSyncEnd - mode->CrtcVSyncStart;
    if (!vsync_wid) vsync_wid = 1;

    save->crtc_v_sync_strt_wid = (((mode->CrtcVSyncStart - 1) & 0xfff)
				  | ((vsync_wid & 0x1f) << 16)
				  | ((mode->Flags & V_NVSYNC)
				     ? RADEON_CRTC_V_SYNC_POL
				     : 0));

    save->crtc_offset      = pScrn->fbOffset;
    save->crtc_offset_cntl = INREG(RADEON_CRTC_OFFSET_CNTL);
    if (info->tilingEnabled) {
       if (IS_R300_VARIANT)
          save->crtc_offset_cntl |= (R300_CRTC_X_Y_MODE_EN |
				     R300_CRTC_MICRO_TILE_BUFFER_DIS |
				     R300_CRTC_MACRO_TILE_EN);
       else
          save->crtc_offset_cntl |= RADEON_CRTC_TILE_EN;
    }
    else {
       if (IS_R300_VARIANT)
          save->crtc_offset_cntl &= ~(R300_CRTC_X_Y_MODE_EN |
				      R300_CRTC_MICRO_TILE_BUFFER_DIS |
				      R300_CRTC_MACRO_TILE_EN);
       else
          save->crtc_offset_cntl &= ~RADEON_CRTC_TILE_EN;
    }

    save->crtc_pitch  = (((pScrn->displayWidth * pScrn->bitsPerPixel) +
			  ((pScrn->bitsPerPixel * 8) -1)) /
			 (pScrn->bitsPerPixel * 8));
    save->crtc_pitch |= save->crtc_pitch << 16;
    
    save->crtc_more_cntl = 0;
    if ((info->ChipFamily == CHIP_FAMILY_RS100) ||
        (info->ChipFamily == CHIP_FAMILY_RS200)) {
        /* This is to workaround the asic bug for RMX, some versions
           of BIOS dosen't have this register initialized correctly.
	*/
        save->crtc_more_cntl |= RADEON_CRTC_H_CUTOFF_ACTIVE_EN;
    }

    save->surface_cntl = 0;
    save->disp_merge_cntl = info->SavedReg.disp_merge_cntl;
    save->disp_merge_cntl &= ~RADEON_DISP_RGB_OFFSET_EN;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* We must set both apertures as they can be both used to map the entire
     * video memory. -BenH.
     */
    switch (pScrn->bitsPerPixel) {
    case 16:
	save->surface_cntl |= RADEON_NONSURF_AP0_SWP_16BPP;
	save->surface_cntl |= RADEON_NONSURF_AP1_SWP_16BPP;
	break;

    case 32:
	save->surface_cntl |= RADEON_NONSURF_AP0_SWP_32BPP;
	save->surface_cntl |= RADEON_NONSURF_AP1_SWP_32BPP;
	break;
    }
#endif

    if (info->IsDellServer) {
	save->dac2_cntl = info->SavedReg.dac2_cntl;
	save->tv_dac_cntl = info->SavedReg.tv_dac_cntl;
	save->crtc2_gen_cntl = info->SavedReg.crtc2_gen_cntl;
	save->disp_hw_debug = info->SavedReg.disp_hw_debug;

	save->dac2_cntl &= ~RADEON_DAC2_DAC_CLK_SEL;
	save->dac2_cntl |= RADEON_DAC2_DAC2_CLK_SEL;

	/* For CRT on DAC2, don't turn it on if BIOS didn't
	   enable it, even it's detected.
	*/
	save->disp_hw_debug |= RADEON_CRT2_DISP1_SEL;
	save->tv_dac_cntl &= ~((1<<2) | (3<<8) | (7<<24) | (0xff<<16));
	save->tv_dac_cntl |= (0x03 | (2<<8) | (0x58<<16));
    }

    RADEONTRACE(("Pitch = %ld bytes (virtualX = %d, displayWidth = %d)\n",
		 save->crtc_pitch, pScrn->virtualX,
		 info->CurrentLayout.displayWidth));
    return TRUE;
}

/* Define CRTC2 registers for requested video mode */
static Bool RADEONInitCrtc2Registers(ScrnInfoPtr pScrn, RADEONSavePtr save,
				     DisplayModePtr mode, RADEONInfoPtr info)
{
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONEntPtr pRADEONEnt   = RADEONEntPriv(pScrn);

    int  format;
    int  hsync_start;
    int  hsync_wid;
    int  vsync_wid;

    switch (info->CurrentLayout.pixel_code) {
    case 4:  format = 1; break;
    case 8:  format = 2; break;
    case 15: format = 3; break;      /*  555 */
    case 16: format = 4; break;      /*  565 */
    case 24: format = 5; break;      /*  RGB */
    case 32: format = 6; break;      /* xRGB */
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Unsupported pixel depth (%d)\n",
		   info->CurrentLayout.bitsPerPixel);
	return FALSE;
    }

    save->crtc2_gen_cntl = (RADEON_CRTC2_EN
			    | RADEON_CRTC2_CRT2_ON
			    | (format << 8)
			    | ((mode->Flags & V_DBLSCAN)
			       ? RADEON_CRTC2_DBL_SCAN_EN
			       : 0)
			    | ((mode->Flags & V_CSYNC)
			       ? RADEON_CRTC2_CSYNC_EN
			       : 0)
			    | ((mode->Flags & V_INTERLACE)
			       ? RADEON_CRTC2_INTERLACE_EN
			       : 0));

    /* Turn CRT on in case the first head is a DFP */
    save->dac2_cntl = info->SavedReg.dac2_cntl;
    /* always let TVDAC drive CRT2, we don't support tvout yet */
    save->dac2_cntl |= RADEON_DAC2_DAC2_CLK_SEL;
    save->disp_output_cntl = info->SavedReg.disp_output_cntl;
    if (info->ChipFamily == CHIP_FAMILY_R200 ||
	IS_R300_VARIANT) {
	save->disp_output_cntl &= ~(RADEON_DISP_DAC_SOURCE_MASK |
				    RADEON_DISP_DAC2_SOURCE_MASK);
	if (pRADEONEnt->MonType1 != MT_CRT) {
	    save->disp_output_cntl |= (RADEON_DISP_DAC_SOURCE_CRTC2 |
				       RADEON_DISP_DAC2_SOURCE_CRTC2);
	} else {
	    if (pRADEONEnt->ReversedDAC) {
		save->disp_output_cntl |= RADEON_DISP_DAC2_SOURCE_CRTC2;
	    } else {
		save->disp_output_cntl |= RADEON_DISP_DAC_SOURCE_CRTC2;
	    }
	}
    } else {
	save->disp_hw_debug = info->SavedReg.disp_hw_debug;
	/* Turn on 2nd CRT */
	if (pRADEONEnt->MonType1 != MT_CRT) {
	    /* This is for some sample boards with the VGA port
	       connected to the TVDAC, but BIOS doesn't reflect this.
	       Here we configure both DACs to use CRTC2.
	       Not sure if this happens in any retail board.
	    */
	    save->disp_hw_debug &= ~RADEON_CRT2_DISP1_SEL;
	    save->dac2_cntl |= RADEON_DAC2_DAC_CLK_SEL;
	} else {
	    if (pRADEONEnt->ReversedDAC) {
		save->disp_hw_debug &= ~RADEON_CRT2_DISP1_SEL;
		save->dac2_cntl &= ~RADEON_DAC2_DAC_CLK_SEL;
	    } else {
		save->disp_hw_debug |= RADEON_CRT2_DISP1_SEL;
		save->dac2_cntl |= RADEON_DAC2_DAC_CLK_SEL;
	    }
	}
    }

    save->crtc2_h_total_disp =
	((((mode->CrtcHTotal / 8) - 1) & 0x3ff)
	 | ((((mode->CrtcHDisplay / 8) - 1) & 0x1ff) << 16));

    hsync_wid = (mode->CrtcHSyncEnd - mode->CrtcHSyncStart) / 8;
    if (!hsync_wid) hsync_wid = 1;
    hsync_start = mode->CrtcHSyncStart - 8;

    save->crtc2_h_sync_strt_wid = ((hsync_start & 0x1fff)
				   | ((hsync_wid & 0x3f) << 16)
				   | ((mode->Flags & V_NHSYNC)
				      ? RADEON_CRTC_H_SYNC_POL
				      : 0));

#if 1
				/* This works for double scan mode. */
    save->crtc2_v_total_disp = (((mode->CrtcVTotal - 1) & 0xffff)
				| ((mode->CrtcVDisplay - 1) << 16));
#else
				/* This is what cce/nbmode.c example code
				 * does -- is this correct?
				 */
    save->crtc2_v_total_disp = (((mode->CrtcVTotal - 1) & 0xffff)
				| ((mode->CrtcVDisplay
				    * ((mode->Flags & V_DBLSCAN) ? 2 : 1) - 1)
				   << 16));
#endif

    vsync_wid = mode->CrtcVSyncEnd - mode->CrtcVSyncStart;
    if (!vsync_wid) vsync_wid = 1;

    save->crtc2_v_sync_strt_wid = (((mode->CrtcVSyncStart - 1) & 0xfff)
				   | ((vsync_wid & 0x1f) << 16)
				   | ((mode->Flags & V_NVSYNC)
				      ? RADEON_CRTC2_V_SYNC_POL
				      : 0));

    /* It seems all fancy options apart from pflip can be safely disabled
     */
    save->crtc2_offset      = pScrn->fbOffset;
    save->crtc2_offset_cntl = INREG(RADEON_CRTC2_OFFSET_CNTL) & RADEON_CRTC_OFFSET_FLIP_CNTL;
    if (info->tilingEnabled) {
       if (IS_R300_VARIANT)
          save->crtc2_offset_cntl |= (R300_CRTC_X_Y_MODE_EN |
				      R300_CRTC_MICRO_TILE_BUFFER_DIS |
				      R300_CRTC_MACRO_TILE_EN);
       else
          save->crtc2_offset_cntl |= RADEON_CRTC_TILE_EN;
    }
    else {
       if (IS_R300_VARIANT)
          save->crtc2_offset_cntl &= ~(R300_CRTC_X_Y_MODE_EN |
				      R300_CRTC_MICRO_TILE_BUFFER_DIS |
				      R300_CRTC_MACRO_TILE_EN);
       else
          save->crtc2_offset_cntl &= ~RADEON_CRTC_TILE_EN;
    }

    /* this should be right */
    if (info->MergedFB) {
    save->crtc2_pitch  = (((info->CRT2pScrn->displayWidth * pScrn->bitsPerPixel) +
			   ((pScrn->bitsPerPixel * 8) -1)) /
			  (pScrn->bitsPerPixel * 8));
    save->crtc2_pitch |= save->crtc2_pitch << 16;
    } else {
    save->crtc2_pitch  = (((pScrn->displayWidth * pScrn->bitsPerPixel) +
			   ((pScrn->bitsPerPixel * 8) -1)) /
			  (pScrn->bitsPerPixel * 8));
    save->crtc2_pitch |= save->crtc2_pitch << 16;
    }
    save->disp2_merge_cntl = info->SavedReg.disp2_merge_cntl;
    save->disp2_merge_cntl &= ~(RADEON_DISP2_RGB_OFFSET_EN);

    if ((info->DisplayType == MT_DFP && info->IsSecondary) || 
	info->MergeType == MT_DFP) {
	save->crtc2_gen_cntl      = (RADEON_CRTC2_EN | (format << 8));
	save->fp2_h_sync_strt_wid = save->crtc2_h_sync_strt_wid;
	save->fp2_v_sync_strt_wid = save->crtc2_v_sync_strt_wid;
	save->fp2_gen_cntl        = info->SavedReg.fp2_gen_cntl | RADEON_FP2_ON;
	save->fp2_gen_cntl	  &= ~(RADEON_FP2_BLANK_EN);

	if ((info->ChipFamily == CHIP_FAMILY_R200) ||
	    IS_R300_VARIANT) {
	    save->fp2_gen_cntl   &= ~(R200_FP2_SOURCE_SEL_MASK |
				      RADEON_FP2_DVO_RATE_SEL_SDR);

	    save->fp2_gen_cntl |= (R200_FP2_SOURCE_SEL_CRTC2 | 
				   RADEON_FP2_DVO_EN);
	} else {
	    save->fp2_gen_cntl &= ~RADEON_FP2_SRC_SEL_MASK;
	    save->fp2_gen_cntl |= RADEON_FP2_SRC_SEL_CRTC2;
	}

	if (pScrn->rgbBits == 8)
	    save->fp2_gen_cntl |= RADEON_FP2_PANEL_FORMAT; /* 24 bit format */
	else
	    save->fp2_gen_cntl &= ~RADEON_FP2_PANEL_FORMAT;/* 18 bit format */

    }

    /* We must set SURFACE_CNTL properly on the second screen too */
    save->surface_cntl = 0;
#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* We must set both apertures as they can be both used to map the entire
     * video memory. -BenH.
     */
    switch (pScrn->bitsPerPixel) {
    case 16:
       save->surface_cntl |= RADEON_NONSURF_AP0_SWP_16BPP;
       save->surface_cntl |= RADEON_NONSURF_AP1_SWP_16BPP;
       break;

    case 32:
       save->surface_cntl |= RADEON_NONSURF_AP0_SWP_32BPP;
       save->surface_cntl |= RADEON_NONSURF_AP1_SWP_32BPP;
       break;
    }
#endif

    RADEONTRACE(("Pitch = %ld bytes (virtualX = %d, displayWidth = %d)\n",
		 save->crtc2_pitch, pScrn->virtualX,
		 info->CurrentLayout.displayWidth));

    return TRUE;
}

/* Define CRTC registers for requested video mode */
static void RADEONInitFPRegisters(ScrnInfoPtr pScrn, RADEONSavePtr orig,
				  RADEONSavePtr save, DisplayModePtr mode,
				  RADEONInfoPtr info)
{
    int    xres = mode->HDisplay;
    int    yres = mode->VDisplay;
    float  Hratio, Vratio;

    /* If the FP registers have been initialized before for a panel,
     * but the primary port is a CRT, we need to reinitialize
     * FP registers in order for CRT to work properly
     */

    if ((info->DisplayType != MT_DFP) && (info->DisplayType != MT_LCD)) {
        save->fp_crtc_h_total_disp = orig->fp_crtc_h_total_disp;
        save->fp_crtc_v_total_disp = orig->fp_crtc_v_total_disp;
        save->fp_gen_cntl          = 0;
        save->fp_h_sync_strt_wid   = orig->fp_h_sync_strt_wid;
        save->fp_horz_stretch      = 0;
        save->fp_v_sync_strt_wid   = orig->fp_v_sync_strt_wid;
        save->fp_vert_stretch      = 0;
        save->lvds_gen_cntl        = orig->lvds_gen_cntl;
        save->lvds_pll_cntl        = orig->lvds_pll_cntl;
        save->tmds_pll_cntl        = orig->tmds_pll_cntl;
        save->tmds_transmitter_cntl= orig->tmds_transmitter_cntl;

        save->lvds_gen_cntl |= ( RADEON_LVDS_DISPLAY_DIS | (1 << 23));
        save->lvds_gen_cntl &= ~(RADEON_LVDS_BLON | RADEON_LVDS_ON);
        save->fp_gen_cntl &= ~(RADEON_FP_FPON | RADEON_FP_TMDS_EN);

        return;
    }

    if (info->PanelXRes == 0 || info->PanelYRes == 0) {
	Hratio = 1.0;
	Vratio = 1.0;
    } else {
	if (xres > info->PanelXRes) xres = info->PanelXRes;
	if (yres > info->PanelYRes) yres = info->PanelYRes;

	Hratio = (float)xres/(float)info->PanelXRes;
	Vratio = (float)yres/(float)info->PanelYRes;
    }

    if (Hratio == 1.0 || !(mode->Flags & RADEON_USE_RMX)) {
	save->fp_horz_stretch = orig->fp_horz_stretch;
	save->fp_horz_stretch &= ~(RADEON_HORZ_STRETCH_BLEND |
				   RADEON_HORZ_STRETCH_ENABLE);
	save->fp_horz_stretch &= ~(RADEON_HORZ_AUTO_RATIO |
				   RADEON_HORZ_PANEL_SIZE);
	save->fp_horz_stretch |= ((xres/8-1)<<16);

    } else {
	save->fp_horz_stretch =
	    ((((unsigned long)(Hratio * RADEON_HORZ_STRETCH_RATIO_MAX +
			       0.5)) & RADEON_HORZ_STRETCH_RATIO_MASK)) |
	    (orig->fp_horz_stretch & (RADEON_HORZ_PANEL_SIZE |
				      RADEON_HORZ_FP_LOOP_STRETCH |
				      RADEON_HORZ_AUTO_RATIO_INC));
	save->fp_horz_stretch |= (RADEON_HORZ_STRETCH_BLEND |
				  RADEON_HORZ_STRETCH_ENABLE);

	save->fp_horz_stretch &= ~(RADEON_HORZ_AUTO_RATIO |
				   RADEON_HORZ_PANEL_SIZE);
	save->fp_horz_stretch |= ((info->PanelXRes / 8 - 1) << 16);

    }

    if (Vratio == 1.0 || !(mode->Flags & RADEON_USE_RMX)) {
	save->fp_vert_stretch = orig->fp_vert_stretch;
	save->fp_vert_stretch &= ~(RADEON_VERT_STRETCH_ENABLE|
				   RADEON_VERT_STRETCH_BLEND);
	save->fp_vert_stretch &= ~(RADEON_VERT_AUTO_RATIO_EN |
				   RADEON_VERT_PANEL_SIZE);
	save->fp_vert_stretch |= ((yres-1) << 12);
    } else {
	save->fp_vert_stretch =
	    (((((unsigned long)(Vratio * RADEON_VERT_STRETCH_RATIO_MAX +
				0.5)) & RADEON_VERT_STRETCH_RATIO_MASK)) |
	     (orig->fp_vert_stretch & (RADEON_VERT_PANEL_SIZE |
				       RADEON_VERT_STRETCH_RESERVED)));
	save->fp_vert_stretch |= (RADEON_VERT_STRETCH_ENABLE |
				  RADEON_VERT_STRETCH_BLEND);

	save->fp_vert_stretch &= ~(RADEON_VERT_AUTO_RATIO_EN |
				   RADEON_VERT_PANEL_SIZE);
	save->fp_vert_stretch |= ((info->PanelYRes-1) << 12);

    }

    save->fp_gen_cntl = (orig->fp_gen_cntl & (CARD32)
			 ~(RADEON_FP_SEL_CRTC2 |
			   RADEON_FP_RMX_HVSYNC_CONTROL_EN |
			   RADEON_FP_DFP_SYNC_SEL |
			   RADEON_FP_CRT_SYNC_SEL |
			   RADEON_FP_CRTC_LOCK_8DOT |
			   RADEON_FP_USE_SHADOW_EN |
			   RADEON_FP_CRTC_USE_SHADOW_VEND |
			   RADEON_FP_CRT_SYNC_ALT));
    save->fp_gen_cntl |= (RADEON_FP_CRTC_DONT_SHADOW_VPAR |
			  RADEON_FP_CRTC_DONT_SHADOW_HEND );

    if (pScrn->rgbBits == 8)
        save->fp_gen_cntl |= RADEON_FP_PANEL_FORMAT;  /* 24 bit format */
    else
        save->fp_gen_cntl &= ~RADEON_FP_PANEL_FORMAT;/* 18 bit format */

    if (IS_R300_VARIANT ||
	(info->ChipFamily == CHIP_FAMILY_R200)) {
	save->fp_gen_cntl &= ~R200_FP_SOURCE_SEL_MASK;
	if (mode->Flags & RADEON_USE_RMX) 
	    save->fp_gen_cntl |= R200_FP_SOURCE_SEL_RMX;
	else
	    save->fp_gen_cntl |= R200_FP_SOURCE_SEL_CRTC1;
    } else 
	save->fp_gen_cntl |= RADEON_FP_SEL_CRTC1;

    save->lvds_gen_cntl = orig->lvds_gen_cntl;
    save->lvds_pll_cntl = orig->lvds_pll_cntl;

    info->PanelOff = FALSE;
    /* This option is used to force the ONLY DEVICE in XFConfig to use
     * CRT port, instead of default DVI port.
     */
    if (xf86ReturnOptValBool(info->Options, OPTION_PANEL_OFF, FALSE)) {
	info->PanelOff = TRUE;
    }

    save->tmds_pll_cntl = orig->tmds_pll_cntl;
    save->tmds_transmitter_cntl= orig->tmds_transmitter_cntl;
    if (info->PanelOff && info->MergedFB) {
	info->OverlayOnCRTC2 = TRUE;
	if (info->DisplayType == MT_LCD) {
	    /* Turning off LVDS_ON seems to make panel white blooming.
	     * For now we just turn off display data ???
	     */
	    save->lvds_gen_cntl |= (RADEON_LVDS_DISPLAY_DIS);
	    save->lvds_gen_cntl &= ~(RADEON_LVDS_BLON | RADEON_LVDS_ON);

	} else if (info->DisplayType == MT_DFP)
	    save->fp_gen_cntl &= ~(RADEON_FP_FPON | RADEON_FP_TMDS_EN);
    } else {
	if (info->DisplayType == MT_LCD) {

	    save->lvds_gen_cntl |= (RADEON_LVDS_ON | RADEON_LVDS_BLON);
	    save->fp_gen_cntl   &= ~(RADEON_FP_FPON | RADEON_FP_TMDS_EN);

	} else if (info->DisplayType == MT_DFP) {
	    int i;
	    CARD32 tmp = orig->tmds_pll_cntl & 0xfffff;
	    for (i=0; i<4; i++) {
		if (info->tmds_pll[i].freq == 0) break;
		if (save->dot_clock_freq < info->tmds_pll[i].freq) {
		    tmp = info->tmds_pll[i].value ;
		    break;
		}
	    }
	    if (IS_R300_VARIANT ||
		(info->ChipFamily == CHIP_FAMILY_RV280)) {
		if (tmp & 0xfff00000)
		    save->tmds_pll_cntl = tmp;
		else
		    save->tmds_pll_cntl = (orig->tmds_pll_cntl & 0xfff00000) | tmp;
	    } else save->tmds_pll_cntl = tmp;

	    RADEONTRACE(("TMDS_PLL from %lx to %lx\n", 
			 orig->tmds_pll_cntl, 
			 save->tmds_pll_cntl));

            save->tmds_transmitter_cntl &= ~(RADEON_TMDS_TRANSMITTER_PLLRST);
            if (IS_R300_VARIANT ||
		(info->ChipFamily == CHIP_FAMILY_R200) || !info->HasCRTC2)
		save->tmds_transmitter_cntl &= ~(RADEON_TMDS_TRANSMITTER_PLLEN);
            else /* weird, RV chips got this bit reversed? */
                save->tmds_transmitter_cntl |= (RADEON_TMDS_TRANSMITTER_PLLEN);

	    save->fp_gen_cntl   |= (RADEON_FP_FPON | RADEON_FP_TMDS_EN);
        }
    }

    info->BiosHotkeys = FALSE;
    /*
     * Allow the bios to toggle outputs. see below for more.
     */
    if (xf86ReturnOptValBool(info->Options, OPTION_BIOS_HOTKEYS, FALSE)) {
	info->BiosHotkeys = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "BIOS HotKeys Enabled\n");
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "BIOS HotKeys Disabled\n");
    }

    if (info->IsMobility && (!info->BiosHotkeys)) {
	RADEONEntPtr pRADEONEnt   = RADEONEntPriv(pScrn);

	/* To work correctly with laptop hotkeys.
	 * Since there is no machnism for accessing ACPI evnets
	 * and the driver currently doesn't know how to validate
	 * a mode dynamically, we have to tell BIOS don't do
	 * display switching after X has started.  
	 * If LCD is on, lid close/open should still work 
	 * with below settings
	 */
	if (info->DisplayType == MT_LCD) {
	    if (pRADEONEnt->MonType2 == MT_CRT)
		save->bios_5_scratch = 0x0201;
	    else if (pRADEONEnt->MonType2 == MT_DFP)
		save->bios_5_scratch = 0x0801;
	    else
		save->bios_5_scratch = orig->bios_5_scratch;
	} else {
	    if (pRADEONEnt->MonType2 == MT_CRT)
		save->bios_5_scratch = 0x0200;
	    else if (pRADEONEnt->MonType2 == MT_DFP)
		save->bios_5_scratch = 0x0800;
	    else
		save->bios_5_scratch = 0x0; 
	}
	save->bios_4_scratch = 0x4;
	save->bios_6_scratch = orig->bios_6_scratch | 0x40000000;

    } else if (info->IsMobility && (info->DisplayType == MT_LCD)) {
	RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

	/* BIOS will use this setting to reset displays upon lid close/open.
	 * Here we let BIOS controls LCD, but the driver will control the external CRT.
	 */
	if (info->MergedFB || pRADEONEnt->HasSecondary)
	    save->bios_5_scratch = 0x01020201;
	else
	    save->bios_5_scratch = orig->bios_5_scratch;

    	save->bios_4_scratch = orig->bios_4_scratch;
	save->bios_6_scratch = orig->bios_6_scratch;

    }

    save->fp_crtc_h_total_disp = save->crtc_h_total_disp;
    save->fp_crtc_v_total_disp = save->crtc_v_total_disp;
    save->fp_h_sync_strt_wid   = save->crtc_h_sync_strt_wid;
    save->fp_v_sync_strt_wid   = save->crtc_v_sync_strt_wid;
}

/* Define PLL registers for requested video mode */
static void RADEONInitPLLRegisters(ScrnInfoPtr pScrn, RADEONInfoPtr info,
				   RADEONSavePtr save, RADEONPLLPtr pll,
				   double dot_clock)
{
    unsigned long  freq = dot_clock * 100;

    struct {
	int divider;
	int bitvalue;
    } *post_div, post_divs[]   = {
				/* From RAGE 128 VR/RAGE 128 GL Register
				 * Reference Manual (Technical Reference
				 * Manual P/N RRG-G04100-C Rev. 0.04), page
				 * 3-17 (PLL_DIV_[3:0]).
				 */
	{  1, 0 },              /* VCLK_SRC                 */
	{  2, 1 },              /* VCLK_SRC/2               */
	{  4, 2 },              /* VCLK_SRC/4               */
	{  8, 3 },              /* VCLK_SRC/8               */
	{  3, 4 },              /* VCLK_SRC/3               */
	{ 16, 5 },              /* VCLK_SRC/16              */
	{  6, 6 },              /* VCLK_SRC/6               */
	{ 12, 7 },              /* VCLK_SRC/12              */
	{  0, 0 }
    };

    if (info->UseBiosDividers) {
       save->ppll_ref_div = info->RefDivider;
       save->ppll_div_3   = info->FeedbackDivider | (info->PostDivider << 16);
       save->htotal_cntl  = 0;
       return;
    }

    if (freq > pll->max_pll_freq)      freq = pll->max_pll_freq;
    if (freq * 12 < pll->min_pll_freq) freq = pll->min_pll_freq / 12;

    for (post_div = &post_divs[0]; post_div->divider; ++post_div) {
	save->pll_output_freq = post_div->divider * freq;

	if (save->pll_output_freq >= pll->min_pll_freq
	    && save->pll_output_freq <= pll->max_pll_freq) break;
    }

    if (!post_div->divider) {
	save->pll_output_freq = freq;
	post_div = &post_divs[0];
    }

    save->dot_clock_freq = freq;
    save->feedback_div   = RADEONDiv(pll->reference_div
				     * save->pll_output_freq,
				     pll->reference_freq);
    save->post_div       = post_div->divider;

    RADEONTRACE(("dc=%ld, of=%ld, fd=%d, pd=%d\n",
	       save->dot_clock_freq,
	       save->pll_output_freq,
	       save->feedback_div,
	       save->post_div));

    save->ppll_ref_div   = pll->reference_div;
    save->ppll_div_3     = (save->feedback_div | (post_div->bitvalue << 16));
    save->htotal_cntl    = 0;
}

/* Define PLL2 registers for requested video mode */
static void RADEONInitPLL2Registers(ScrnInfoPtr pScrn, RADEONSavePtr save,
				    RADEONPLLPtr pll, double dot_clock,
				    int no_odd_postdiv)
{
    unsigned long  freq = dot_clock * 100;

    struct {
	int divider;
	int bitvalue;
    } *post_div, post_divs[]   = {
				/* From RAGE 128 VR/RAGE 128 GL Register
				 * Reference Manual (Technical Reference
				 * Manual P/N RRG-G04100-C Rev. 0.04), page
				 * 3-17 (PLL_DIV_[3:0]).
				 */
	{  1, 0 },              /* VCLK_SRC                 */
	{  2, 1 },              /* VCLK_SRC/2               */
	{  4, 2 },              /* VCLK_SRC/4               */
	{  8, 3 },              /* VCLK_SRC/8               */
	{  3, 4 },              /* VCLK_SRC/3               */
	{  6, 6 },              /* VCLK_SRC/6               */
	{ 12, 7 },              /* VCLK_SRC/12              */
	{  0, 0 }
    };

    if (freq > pll->max_pll_freq)      freq = pll->max_pll_freq;
    if (freq * 12 < pll->min_pll_freq) freq = pll->min_pll_freq / 12;

    for (post_div = &post_divs[0]; post_div->divider; ++post_div) {
       /* Odd post divider value don't work properly on the second digital
        * output
        */
       if (no_odd_postdiv && (post_div->divider & 1))
           continue;
	save->pll_output_freq_2 = post_div->divider * freq;
	if (save->pll_output_freq_2 >= pll->min_pll_freq
	    && save->pll_output_freq_2 <= pll->max_pll_freq) break;
    }

    if (!post_div->divider) {
	save->pll_output_freq_2 = freq;
	post_div = &post_divs[0];
    }

    save->dot_clock_freq_2 = freq;
    save->feedback_div_2   = RADEONDiv(pll->reference_div
				       * save->pll_output_freq_2,
				       pll->reference_freq);
    save->post_div_2       = post_div->divider;

    RADEONTRACE(("dc=%ld, of=%ld, fd=%d, pd=%d\n",
	       save->dot_clock_freq_2,
	       save->pll_output_freq_2,
	       save->feedback_div_2,
	       save->post_div_2));

    save->p2pll_ref_div    = pll->reference_div;
    save->p2pll_div_0      = (save->feedback_div_2 |
			      (post_div->bitvalue << 16));
    save->htotal_cntl2     = 0;
}

#if 0
/* Define initial palette for requested video mode.  This doesn't do
 * anything for XFree86 4.0.
 */
static void RADEONInitPalette(RADEONSavePtr save)
{
    save->palette_valid = FALSE;
}
#endif

/* Define registers for a requested video mode */
static Bool RADEONInit(ScrnInfoPtr pScrn, DisplayModePtr mode,
		       RADEONSavePtr save)
{
    RADEONInfoPtr  info      = RADEONPTR(pScrn);
    double         dot_clock = mode->Clock/1000.0;

#if RADEON_DEBUG
    ErrorF("%-12.12s %7.2f  %4d %4d %4d %4d  %4d %4d %4d %4d (%d,%d)",
	   mode->name,
	   dot_clock,

	   mode->HDisplay,
	   mode->HSyncStart,
	   mode->HSyncEnd,
	   mode->HTotal,

	   mode->VDisplay,
	   mode->VSyncStart,
	   mode->VSyncEnd,
	   mode->VTotal,
	   pScrn->depth,
	   pScrn->bitsPerPixel);
    if (mode->Flags & V_DBLSCAN)   ErrorF(" D");
    if (mode->Flags & V_CSYNC)     ErrorF(" C");
    if (mode->Flags & V_INTERLACE) ErrorF(" I");
    if (mode->Flags & V_PHSYNC)    ErrorF(" +H");
    if (mode->Flags & V_NHSYNC)    ErrorF(" -H");
    if (mode->Flags & V_PVSYNC)    ErrorF(" +V");
    if (mode->Flags & V_NVSYNC)    ErrorF(" -V");
    ErrorF("\n");
    ErrorF("%-12.12s %7.2f  %4d %4d %4d %4d  %4d %4d %4d %4d (%d,%d)",
	   mode->name,
	   dot_clock,

	   mode->CrtcHDisplay,
	   mode->CrtcHSyncStart,
	   mode->CrtcHSyncEnd,
	   mode->CrtcHTotal,

	   mode->CrtcVDisplay,
	   mode->CrtcVSyncStart,
	   mode->CrtcVSyncEnd,
	   mode->CrtcVTotal,
	   pScrn->depth,
	   pScrn->bitsPerPixel);
    if (mode->Flags & V_DBLSCAN)   ErrorF(" D");
    if (mode->Flags & V_CSYNC)     ErrorF(" C");
    if (mode->Flags & V_INTERLACE) ErrorF(" I");
    if (mode->Flags & V_PHSYNC)    ErrorF(" +H");
    if (mode->Flags & V_NHSYNC)    ErrorF(" -H");
    if (mode->Flags & V_PVSYNC)    ErrorF(" +V");
    if (mode->Flags & V_NVSYNC)    ErrorF(" -V");
    ErrorF("\n");
#endif

    info->Flags = mode->Flags;

    RADEONInitMemMapRegisters(pScrn, save, info);
    RADEONInitCommonRegisters(save, info);
    if (info->IsSecondary) {
	if (!RADEONInitCrtc2Registers(pScrn, save, mode, info))
	    return FALSE;
	RADEONInitPLL2Registers(pScrn, save, &info->pll, dot_clock, info->DisplayType != MT_CRT);
    } else if (info->MergedFB) {
        if (!RADEONInitCrtcRegisters(pScrn, save, 
			((RADEONMergedDisplayModePtr)mode->Private)->CRT1, info))
            return FALSE;
        dot_clock = (((RADEONMergedDisplayModePtr)mode->Private)->CRT1)->Clock / 1000.0;
        if (dot_clock) {
		RADEONInitPLLRegisters(pScrn, info, save, &info->pll, dot_clock);
        } else {
            save->ppll_ref_div = info->SavedReg.ppll_ref_div;
            save->ppll_div_3   = info->SavedReg.ppll_div_3;
            save->htotal_cntl  = info->SavedReg.htotal_cntl;
        }
        RADEONInitCrtc2Registers(pScrn, save, 
			((RADEONMergedDisplayModePtr)mode->Private)->CRT2, info);
        dot_clock = (((RADEONMergedDisplayModePtr)mode->Private)->CRT2)->Clock / 1000.0;
        RADEONInitPLL2Registers(pScrn, save, &info->pll, dot_clock, info->MergeType != MT_CRT);
    } else {
	if (!RADEONInitCrtcRegisters(pScrn, save, mode, info))
	    return FALSE;
	dot_clock = mode->Clock/1000.0;
	if (dot_clock) {
		RADEONInitPLLRegisters(pScrn, info, save, &info->pll, dot_clock);
	} else {
	    save->ppll_ref_div = info->SavedReg.ppll_ref_div;
	    save->ppll_div_3   = info->SavedReg.ppll_div_3;
	    save->htotal_cntl  = info->SavedReg.htotal_cntl;
	}

	/* Not used for now: */
     /* if (!info->PaletteSavedOnVT) RADEONInitPalette(save); */
    }

    /* make RMX work for mergedfb modes on the LCD */
    if (info->MergedFB) {
	if ((info->MergeType == MT_LCD) || (info->MergeType == MT_DFP)) {
            /* I suppose crtc2 could drive the FP as well... */
	    RADEONInitFPRegisters(pScrn, &info->SavedReg, save, 
		((RADEONMergedDisplayModePtr)mode->Private)->CRT2, info);
	}
	else {
	    RADEONInitFPRegisters(pScrn, &info->SavedReg, save, 
		((RADEONMergedDisplayModePtr)mode->Private)->CRT1, info);
	}
    }
    else {
    	RADEONInitFPRegisters(pScrn, &info->SavedReg, save, mode, info);
    }

    RADEONTRACE(("RADEONInit returns %p\n", save));
    return TRUE;
}

/* Initialize a new mode */
static Bool RADEONModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    RADEONTRACE(("RADEONModeInit()\n"));

    if (!RADEONInit(pScrn, mode, &info->ModeReg)) return FALSE;

    pScrn->vtSema = TRUE;
    RADEONBlank(pScrn);
    RADEONRestoreMode(pScrn, &info->ModeReg);
    RADEONUnblank(pScrn);

    info->CurrentLayout.mode = mode;

    if (info->DispPriority)
	RADEONInitDispBandwidth(pScrn);

    return TRUE;
}

static Bool RADEONSaveScreen(ScreenPtr pScreen, int mode)
{
    ScrnInfoPtr  pScrn = xf86Screens[pScreen->myNum];
    Bool         unblank;

    RADEONTRACE(("RADEONSaveScreen(%d)\n", mode));

    unblank = xf86IsUnblank(mode);
    if (unblank) SetTimeSinceLastInputEvent();

    if ((pScrn != NULL) && pScrn->vtSema) {
	if (unblank)  RADEONUnblank(pScrn);
	else          RADEONBlank(pScrn);
    }
    return TRUE;
}

static void
RADEONResetDPI(ScrnInfoPtr pScrn, Bool force)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    ScreenPtr pScreen = screenInfo.screens[pScrn->scrnIndex];

    if(force					||
       (info->RADEONDPIVX != pScrn->virtualX)	||
       (info->RADEONDPIVY != pScrn->virtualY)
					  ) {

       pScreen->mmWidth = (pScrn->virtualX * 254 + pScrn->xDpi * 5) / (pScrn->xDpi * 10);
       pScreen->mmHeight = (pScrn->virtualY * 254 + pScrn->yDpi * 5) / (pScrn->yDpi * 10);

       info->RADEONDPIVX = pScrn->virtualX;
       info->RADEONDPIVY = pScrn->virtualY;

    }
}

_X_EXPORT Bool RADEONSwitchMode(int scrnIndex, DisplayModePtr mode, int flags)
{
    ScrnInfoPtr    pScrn       = xf86Screens[scrnIndex];
    RADEONInfoPtr  info        = RADEONPTR(pScrn);
    Bool           tilingOld   = info->tilingEnabled;
    Bool           ret;
#ifdef XF86DRI
    Bool           CPStarted   = info->CPStarted;

    if (CPStarted) {
	DRILock(pScrn->pScreen, 0);
	RADEONCP_STOP(pScrn, info);
    }
#endif

    RADEONTRACE(("RADEONSwitchMode() !n"));

    if (info->allowColorTiling) {
	if (info->MergedFB) {
	    if ((((RADEONMergedDisplayModePtr)mode->Private)->CRT1->Flags &
		(V_DBLSCAN | V_INTERLACE)) ||
		(((RADEONMergedDisplayModePtr)mode->Private)->CRT2->Flags &
		(V_DBLSCAN | V_INTERLACE)))
		info->tilingEnabled = FALSE;
	    else info->tilingEnabled = TRUE;
	}
	else {
            info->tilingEnabled = (mode->Flags & (V_DBLSCAN | V_INTERLACE)) ? FALSE : TRUE;
	}
#ifdef XF86DRI	
	if (info->directRenderingEnabled && (info->tilingEnabled != tilingOld)) {
	    RADEONSAREAPrivPtr pSAREAPriv;
	    drmRadeonSetParam  radeonsetparam;
	    memset(&radeonsetparam, 0, sizeof(drmRadeonSetParam));
	    radeonsetparam.param = RADEON_SETPARAM_SWITCH_TILING;
	    radeonsetparam.value = info->tilingEnabled ? 1 : 0;
	    if (drmCommandWrite(info->drmFD, DRM_RADEON_SETPARAM,
		&radeonsetparam, sizeof(drmRadeonSetParam)) < 0)
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		    "[drm] failed changing tiling status\n");
	    pSAREAPriv = DRIGetSAREAPrivate(pScrn->pScreen);
	    info->tilingEnabled = pSAREAPriv->tiling_enabled ? TRUE : FALSE;
	}
#endif
    }

    if (info->accelOn)
        RADEON_SYNC(info, pScrn);

    if (info->FBDev) {
	RADEONSaveFBDevRegisters(pScrn, &info->ModeReg);

	ret = fbdevHWSwitchMode(scrnIndex, mode, flags);
	pScrn->displayWidth = fbdevHWGetLineLength(pScrn)
		/ info->CurrentLayout.pixel_bytes;

	RADEONRestoreFBDevRegisters(pScrn, &info->ModeReg);
    } else {
	info->IsSwitching = TRUE;
	ret = RADEONModeInit(xf86Screens[scrnIndex], mode);
	info->IsSwitching = FALSE;
    }

    if (info->tilingEnabled != tilingOld) {
	/* need to redraw front buffer, I guess this can be considered a hack ? */
	xf86EnableDisableFBAccess(scrnIndex, FALSE);
	RADEONChangeSurfaces(pScrn);
	xf86EnableDisableFBAccess(scrnIndex, TRUE);
	/* xf86SetRootClip would do, but can't access that here */
    }

    if (info->accelOn) {
        RADEON_SYNC(info, pScrn);
	RADEONEngineRestore(pScrn);
    }

#ifdef XF86DRI
    if (CPStarted) {
	RADEONCP_START(pScrn, info);
	DRIUnlock(pScrn->pScreen);
    }
#endif

    /* Since RandR (indirectly) uses SwitchMode(), we need to
     * update our Xinerama info here, too, in case of resizing
     */
    if (info->MergedFB) {
        RADEONMergedFBResetDpi(pScrn, FALSE);
        RADEONUpdateXineramaScreenInfo(pScrn);
    } else if(info->constantDPI) {
       RADEONResetDPI(pScrn, FALSE);
    }

    return ret;
}

#ifdef X_XF86MiscPassMessage
_X_EXPORT Bool RADEONHandleMessage(int scrnIndex, const char* msgtype,
                                   const char* msgval, char** retmsg)
{
    ErrorF("RADEONHandleMessage(%d, \"%s\", \"%s\", retmsg)\n", scrnIndex,
		    msgtype, msgval);
    *retmsg = "";
    return 0;
}
#endif

/* Used to disallow modes that are not supported by the hardware */
_X_EXPORT ModeStatus RADEONValidMode(int scrnIndex, DisplayModePtr mode,
                                     Bool verbose, int flag)
{
    /* There are problems with double scan mode at high clocks
     * They're likely related PLL and display buffer settings.
     * Disable these modes for now.
     */
    if (mode->Flags & V_DBLSCAN) {
	if ((mode->CrtcHDisplay >= 1024) || (mode->CrtcVDisplay >= 768))
	    return MODE_CLOCK_RANGE;
    }
    return MODE_OK;
}

/* Adjust viewport into virtual desktop such that (0,0) in viewport
 * space is (x,y) in virtual space.
 */
void RADEONDoAdjustFrame(ScrnInfoPtr pScrn, int x, int y, int clone)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int            reg, Base, regcntl, crtcoffsetcntl, xytilereg, crtcxytile = 0;
#ifdef XF86DRI
    RADEONSAREAPrivPtr pSAREAPriv;
    XF86DRISAREAPtr pSAREA;
#endif

#if 0 /* Verbose */
    RADEONTRACE(("RADEONDoAdjustFrame(%d,%d,%d)\n", x, y, clone));
#endif

    if (info->showCache && y) {
	        int lastline = info->FbMapSize /
		    ((pScrn->displayWidth * pScrn->bitsPerPixel) / 8);

		lastline -= pScrn->currentMode->VDisplay;
		y += (pScrn->virtualY - 1) * (y / 3 + 1);
		if (y > lastline) y = lastline;
    }

    Base = pScrn->fbOffset;

  /* note we cannot really simply use the info->ModeReg.crtc_offset_cntl value, since the
     drm might have set FLIP_CNTL since we wrote that. Unfortunately FLIP_CNTL causes
     flickering when scrolling vertically in a virtual screen, possibly because crtc will
     pick up the new offset value at the end of each scanline, but the new offset_cntl value
     only after a vsync. We'd probably need to wait (in drm) for vsync and only then update
     OFFSET and OFFSET_CNTL, if the y coord has changed. Seems hard to fix. */
    if (clone || info->IsSecondary) {
        reg = RADEON_CRTC2_OFFSET;
	regcntl = RADEON_CRTC2_OFFSET_CNTL;
	xytilereg = R300_CRTC2_TILE_X0_Y0;
    } else {
        reg = RADEON_CRTC_OFFSET;
	regcntl = RADEON_CRTC_OFFSET_CNTL;
	xytilereg = R300_CRTC_TILE_X0_Y0;
    }
    crtcoffsetcntl = INREG(regcntl) & ~0xf;
#if 0
    /* try to get rid of flickering when scrolling at least for 2d */
#ifdef XF86DRI
    if (!info->have3DWindows)
#endif
    crtcoffsetcntl &= ~RADEON_CRTC_OFFSET_FLIP_CNTL;
#endif
    if (info->tilingEnabled) {
        if (IS_R300_VARIANT) {
	/* On r300/r400 when tiling is enabled crtc_offset is set to the address of
	 * the surface.  the x/y offsets are handled by the X_Y tile reg for each crtc
	 * Makes tiling MUCH easier.
	 */
             crtcxytile = x | (y << 16);
             Base &= ~0x7ff;
         } else {
             int byteshift = info->CurrentLayout.bitsPerPixel >> 4;
             /* crtc uses 256(bytes)x8 "half-tile" start addresses? */
             int tile_addr = (((y >> 3) * info->CurrentLayout.displayWidth + x) >> (8 - byteshift)) << 11;
             Base += tile_addr + ((x << byteshift) % 256) + ((y % 8) << 8);
             crtcoffsetcntl = crtcoffsetcntl | (y % 16);
         }
    }
    else {
       int offset = y * info->CurrentLayout.displayWidth + x;
       switch (info->CurrentLayout.pixel_code) {
       case 15:
       case 16: offset *= 2; break;
       case 24: offset *= 3; break;
       case 32: offset *= 4; break;
       }
       Base += offset;
    }

    Base &= ~7;                 /* 3 lower bits are always 0 */

#ifdef XF86DRI
    if (info->directRenderingInited) {
	/* note cannot use pScrn->pScreen since this is unitialized when called from
	   RADEONScreenInit, and we need to call from there to get mergedfb + pageflip working */
        /*** NOTE: r3/4xx will need sarea and drm pageflip updates to handle the xytile regs for
	 *** pageflipping!
	 ***/
	pSAREAPriv = DRIGetSAREAPrivate(screenInfo.screens[pScrn->scrnIndex]);
	/* can't get at sarea in a semi-sane way? */
	pSAREA = (void *)((char*)pSAREAPriv - sizeof(XF86DRISAREARec));

	if (clone || info->IsSecondary) {
	    pSAREAPriv->crtc2_base = Base;
	}
	else {
	    pSAREA->frame.x = (Base  / info->CurrentLayout.pixel_bytes)
		% info->CurrentLayout.displayWidth;
	    pSAREA->frame.y = (Base / info->CurrentLayout.pixel_bytes)
		/ info->CurrentLayout.displayWidth;
	    pSAREA->frame.width = pScrn->frameX1 - x + 1;
	    pSAREA->frame.height = pScrn->frameY1 - y + 1;
	}

	if (pSAREAPriv->pfCurrentPage == 1) {
	    Base += info->backOffset;
	}
    }
#endif

    OUTREG(reg, Base);

    if (IS_R300_VARIANT) {
        OUTREG(xytilereg, crtcxytile);
    } else {
        OUTREG(regcntl, crtcoffsetcntl);
    }

}

_X_EXPORT void RADEONAdjustFrame(int scrnIndex, int x, int y, int flags)
{
    ScrnInfoPtr    pScrn      = xf86Screens[scrnIndex];
    RADEONInfoPtr  info       = RADEONPTR(pScrn);

#ifdef XF86DRI
    if (info->CPStarted && pScrn->pScreen) DRILock(pScrn->pScreen, 0);
#endif

    if (info->accelOn)
        RADEON_SYNC(info, pScrn);

    if(info->MergedFB) {
    	RADEONAdjustFrameMerged(scrnIndex, x, y, flags);
    } else if (info->FBDev) {
	fbdevHWAdjustFrame(scrnIndex, x, y, flags);
    } else {
	RADEONDoAdjustFrame(pScrn, x, y, FALSE);
    }

#ifdef XF86DRI
	if (info->CPStarted && pScrn->pScreen) DRIUnlock(pScrn->pScreen);
#endif
}

/* Called when VT switching back to the X server.  Reinitialize the
 * video mode.
 */
_X_EXPORT Bool RADEONEnterVT(int scrnIndex, int flags)
{
    ScrnInfoPtr    pScrn = xf86Screens[scrnIndex];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    RADEONTRACE(("RADEONEnterVT\n"));

    if (INREG(RADEON_CONFIG_MEMSIZE) == 0) { /* Softboot V_BIOS */
       xf86Int10InfoPtr pInt;
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                  "zero MEMSIZE, probably at D3cold. Re-POSTing via int10.\n");
       pInt = xf86InitInt10 (info->pEnt->index);
       if (pInt) {
           pInt->num = 0xe6;
           xf86ExecX86int10 (pInt);
           xf86FreeInt10 (pInt);
       }
    }

    /* Makes sure the engine is idle before doing anything */
    RADEONWaitForIdleMMIO(pScrn);

    if (info->FBDev) {
	unsigned char *RADEONMMIO = info->MMIO;
	if (!fbdevHWEnterVT(scrnIndex,flags)) return FALSE;
	info->PaletteSavedOnVT = FALSE;
	info->ModeReg.surface_cntl = INREG(RADEON_SURFACE_CNTL);

	RADEONRestoreFBDevRegisters(pScrn, &info->ModeReg);
    } else
	if (!RADEONModeInit(pScrn, pScrn->currentMode)) return FALSE;

    if (!info->IsSecondary)
	RADEONRestoreSurfaces(pScrn, &info->ModeReg);
#ifdef XF86DRI
    if (info->directRenderingEnabled) {
    	if (info->cardType == CARD_PCIE && info->pKernelDRMVersion->version_minor >= 19 && info->FbSecureSize)
    	{
      		/* we need to backup the PCIE GART TABLE from fb memory */
     	 memcpy(info->FB + info->pciGartOffset, info->pciGartBackup, info->pciGartSize);
    	}

	/* get the DRI back into shape after resume */
	RADEONDRIResume(pScrn->pScreen);
	RADEONAdjustMemMapRegisters(pScrn, &info->ModeReg);
    }
#endif
    /* this will get XVideo going again, but only if XVideo was initialised
       during server startup (hence the info->adaptor if). */
    if (info->adaptor)
	RADEONResetVideo(pScrn);

    if (info->accelOn)
	RADEONEngineRestore(pScrn);

#ifdef XF86DRI
    if (info->directRenderingEnabled) {
	RADEONCP_START(pScrn, info);
	DRIUnlock(pScrn->pScreen);
    }
#endif

    pScrn->AdjustFrame(scrnIndex, pScrn->frameX0, pScrn->frameY0, 0);

    return TRUE;
}

/* Called when VT switching away from the X server.  Restore the
 * original text mode.
 */
_X_EXPORT void RADEONLeaveVT(int scrnIndex, int flags)
{
    ScrnInfoPtr    pScrn = xf86Screens[scrnIndex];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    RADEONSavePtr  save  = &info->ModeReg;

    RADEONTRACE(("RADEONLeaveVT\n"));
#ifdef XF86DRI
    if (RADEONPTR(pScrn)->directRenderingInited) {
	DRILock(pScrn->pScreen, 0);
	RADEONCP_STOP(pScrn, info);

        if (info->cardType == CARD_PCIE && info->pKernelDRMVersion->version_minor >= 19 && info->FbSecureSize)
        {
            /* we need to backup the PCIE GART TABLE from fb memory */
            memcpy(info->pciGartBackup, (info->FB + info->pciGartOffset), info->pciGartSize);
        }
    }
#endif

    if (info->FBDev) {
	RADEONSavePalette(pScrn, save);
	info->PaletteSavedOnVT = TRUE;

	RADEONSaveFBDevRegisters(pScrn, &info->ModeReg);

	fbdevHWLeaveVT(scrnIndex,flags);
    }

    RADEONRestore(pScrn);

    RADEONTRACE(("Ok, leaving now...\n"));
}

/* Called at the end of each server generation.  Restore the original
 * text mode, unmap video memory, and unwrap and call the saved
 * CloseScreen function.
 */
static Bool RADEONCloseScreen(int scrnIndex, ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86Screens[scrnIndex];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);

    RADEONTRACE(("RADEONCloseScreen\n"));

    /* Mark acceleration as stopped or we might try to access the engine at
     * wrong times, especially if we had DRI, after DRI has been stopped
     */
    info->accelOn = FALSE;

#ifdef XF86DRI
    RADEONDRIStop(pScreen);
#endif

#ifdef USE_XAA
    if(!info->useEXA && info->RenderTex) {
        xf86FreeOffscreenLinear(info->RenderTex);
        info->RenderTex = NULL;
    }
#endif /* USE_XAA */

    if (pScrn->vtSema) {
	RADEONDisplayPowerManagementSet(pScrn, DPMSModeOn, 0);
	RADEONRestore(pScrn);
    }

    RADEONTRACE(("Disposing accel...\n"));
#ifdef USE_EXA
    if (info->exa) {
	exaDriverFini(pScreen);
	xfree(info->exa);
	info->exa = NULL;
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!info->useEXA) {
	if (info->accel)
		XAADestroyInfoRec(info->accel);
	info->accel = NULL;

	if (info->scratch_save)
	    xfree(info->scratch_save);
	info->scratch_save = NULL;
    }
#endif /* USE_XAA */

    RADEONTRACE(("Disposing cusor info\n"));
    if (info->cursor) xf86DestroyCursorInfoRec(info->cursor);
    info->cursor = NULL;

    RADEONTRACE(("Disposing DGA\n"));
    if (info->DGAModes) xfree(info->DGAModes);
    info->DGAModes = NULL;
    RADEONTRACE(("Unmapping memory\n"));
    RADEONUnmapMem(pScrn);

    pScrn->vtSema = FALSE;

    xf86ClearPrimInitDone(info->pEnt->index);

    pScreen->BlockHandler = info->BlockHandler;
    pScreen->CloseScreen = info->CloseScreen;
    return (*pScreen->CloseScreen)(scrnIndex, pScreen);
}

_X_EXPORT void RADEONFreeScreen(int scrnIndex, int flags)
{
    ScrnInfoPtr  pScrn = xf86Screens[scrnIndex];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    
    RADEONTRACE(("RADEONFreeScreen\n"));

    /* when server quits at PreInit, we don't need do this anymore*/
    if (!info) return;

    if(info->MergedFB) {
       if(pScrn->modes) {
          pScrn->currentMode = pScrn->modes;
          do {
            DisplayModePtr p = pScrn->currentMode->next;
            if(pScrn->currentMode->Private)
                xfree(pScrn->currentMode->Private);
            xfree(pScrn->currentMode);
            pScrn->currentMode = p;
          } while(pScrn->currentMode != pScrn->modes);
       }
       pScrn->currentMode = info->CRT1CurrentMode;
       pScrn->modes = info->CRT1Modes;
       info->CRT1CurrentMode = NULL;
       info->CRT1Modes = NULL;

       if(info->CRT2pScrn) {
          if(info->CRT2pScrn->modes) {
             while(info->CRT2pScrn->modes)
                xf86DeleteMode(&info->CRT2pScrn->modes, info->CRT2pScrn->modes);
          }
          if(info->CRT2pScrn->monitor) {
	     if(info->CRT2pScrn->monitor->Modes) {
	        while(info->CRT2pScrn->monitor->Modes)
		   xf86DeleteMode(&info->CRT2pScrn->monitor->Modes, info->CRT2pScrn->monitor->Modes);
	     }
	     if(info->CRT2pScrn->monitor->DDC) xfree(info->CRT2pScrn->monitor->DDC);
             xfree(info->CRT2pScrn->monitor);
	  }
          xfree(info->CRT2pScrn);
          info->CRT2pScrn = NULL;
       }
    }

#ifdef WITH_VGAHW
    if (info->VGAAccess && xf86LoaderCheckSymbol("vgaHWFreeHWRec"))
	vgaHWFreeHWRec(pScrn);
#endif
    RADEONFreeRec(pScrn);
}

/*
 * Powering done DAC, needed for DPMS problem with ViewSonic P817 (or its variant).
 *
 * Note for current DAC mapping when calling this function:
 * For most of cards:
 * single CRT:  Driver doesn't change the existing CRTC->DAC mapping, 
 *              CRTC1 could be driving either DAC or both DACs.
 * CRT+CRT:     CRTC1->TV DAC, CRTC2->Primary DAC
 * DFP/LCD+CRT: CRTC2->TV DAC, CRTC2->Primary DAC.
 * Some boards have two DACs reversed or don't even have a primary DAC,
 * this is reflected in pRADEONEnt->ReversedDAC. And radeon 7200 doesn't 
 * have a second DAC.
 * It's kind of messy, we'll need to redo DAC mapping part some day.
 */
static void RADEONDacPowerSet(ScrnInfoPtr pScrn, Bool IsOn, Bool IsPrimaryDAC)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (IsPrimaryDAC) {
	CARD32 dac_cntl;
	CARD32 dac_macro_cntl = 0;
	dac_cntl = INREG(RADEON_DAC_CNTL);
	if ((!info->IsMobility) || (info->ChipFamily == CHIP_FAMILY_RV350)) 
	    dac_macro_cntl = INREG(RADEON_DAC_MACRO_CNTL);
	if (IsOn) {
	    dac_cntl &= ~RADEON_DAC_PDWN;
	    dac_macro_cntl &= ~(RADEON_DAC_PDWN_R |
				RADEON_DAC_PDWN_G |
				RADEON_DAC_PDWN_B);
	} else {
	    dac_cntl |= RADEON_DAC_PDWN;
	    dac_macro_cntl |= (RADEON_DAC_PDWN_R |
			       RADEON_DAC_PDWN_G |
			       RADEON_DAC_PDWN_B);
	}
	OUTREG(RADEON_DAC_CNTL, dac_cntl);
	if ((!info->IsMobility) || (info->ChipFamily == CHIP_FAMILY_RV350)) 
	    OUTREG(RADEON_DAC_MACRO_CNTL, dac_macro_cntl);
    } else {
	if (info->ChipFamily != CHIP_FAMILY_R200) {
	    CARD32 tv_dac_cntl = INREG(RADEON_TV_DAC_CNTL);
	    if (IsOn) {
		tv_dac_cntl &= ~(RADEON_TV_DAC_RDACPD |
				 RADEON_TV_DAC_GDACPD |
				 RADEON_TV_DAC_BDACPD |
				 RADEON_TV_DAC_BGSLEEP);
	    } else {
		tv_dac_cntl |= (RADEON_TV_DAC_RDACPD |
				RADEON_TV_DAC_GDACPD |
				RADEON_TV_DAC_BDACPD |
				RADEON_TV_DAC_BGSLEEP);
	    }
	    OUTREG(RADEON_TV_DAC_CNTL, tv_dac_cntl);
	} else {
	    CARD32 fp2_gen_cntl = INREG(RADEON_FP2_GEN_CNTL);
	    if (IsOn) {
		fp2_gen_cntl |= RADEON_FP2_DVO_EN;
	    } else {
		fp2_gen_cntl &= ~RADEON_FP2_DVO_EN;
	    }
	    OUTREG(RADEON_FP2_GEN_CNTL, fp2_gen_cntl);
	}
    }
}

/* Sets VESA Display Power Management Signaling (DPMS) Mode */
static void RADEONDisplayPowerManagementSet(ScrnInfoPtr pScrn,
					    int PowerManagementMode,
					    int flags)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt   = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (!pScrn->vtSema) return;

    RADEONTRACE(("RADEONDisplayPowerManagementSet(%d,0x%x)\n", PowerManagementMode, flags));

#ifdef XF86DRI
    if (info->CPStarted) DRILock(pScrn->pScreen, 0);
#endif

    if (info->accelOn)
        RADEON_SYNC(info, pScrn);

    if (info->FBDev) {
	fbdevHWDPMSSet(pScrn, PowerManagementMode, flags);
    } else {
	int             mask1     = (RADEON_CRTC_DISPLAY_DIS |
				     RADEON_CRTC_HSYNC_DIS |
				     RADEON_CRTC_VSYNC_DIS);
	int             mask2     = (RADEON_CRTC2_DISP_DIS |
				     RADEON_CRTC2_VSYNC_DIS |
				     RADEON_CRTC2_HSYNC_DIS);

	switch (PowerManagementMode) {
	case DPMSModeOn:
	    /* Screen: On; HSync: On, VSync: On */
	    if (info->IsSecondary)
		OUTREGP(RADEON_CRTC2_GEN_CNTL, 0, ~mask2);
	    else {
		if (info->MergedFB)
		    OUTREGP(RADEON_CRTC2_GEN_CNTL, 0, ~mask2);
		OUTREGP(RADEON_CRTC_EXT_CNTL, 0, ~mask1);
	    }
	    break;

	case DPMSModeStandby:
	    /* Screen: Off; HSync: Off, VSync: On */
	    if (info->IsSecondary)
		OUTREGP(RADEON_CRTC2_GEN_CNTL,
			(RADEON_CRTC2_DISP_DIS | RADEON_CRTC2_HSYNC_DIS),
			~mask2);
	    else {
		if (info->MergedFB)
		    OUTREGP(RADEON_CRTC2_GEN_CNTL,
			    (RADEON_CRTC2_DISP_DIS | RADEON_CRTC2_HSYNC_DIS),
			    ~mask2);
		OUTREGP(RADEON_CRTC_EXT_CNTL,
			(RADEON_CRTC_DISPLAY_DIS | RADEON_CRTC_HSYNC_DIS),
			~mask1);
	    }
	    break;

	case DPMSModeSuspend:
	    /* Screen: Off; HSync: On, VSync: Off */
	    if (info->IsSecondary)
		OUTREGP(RADEON_CRTC2_GEN_CNTL,
			(RADEON_CRTC2_DISP_DIS | RADEON_CRTC2_VSYNC_DIS),
			~mask2);
	    else {
		if (info->MergedFB)
		    OUTREGP(RADEON_CRTC2_GEN_CNTL,
			    (RADEON_CRTC2_DISP_DIS | RADEON_CRTC2_VSYNC_DIS),
			    ~mask2);
		OUTREGP(RADEON_CRTC_EXT_CNTL,
			(RADEON_CRTC_DISPLAY_DIS | RADEON_CRTC_VSYNC_DIS),
			~mask1);
	    }
	    break;

	case DPMSModeOff:
	    /* Screen: Off; HSync: Off, VSync: Off */
	    if (info->IsSecondary)
		OUTREGP(RADEON_CRTC2_GEN_CNTL, mask2, ~mask2);
	    else {
		if (info->MergedFB)
		    OUTREGP(RADEON_CRTC2_GEN_CNTL, mask2, ~mask2);
		OUTREGP(RADEON_CRTC_EXT_CNTL, mask1, ~mask1);
	    }
	    break;
	}

	if (PowerManagementMode == DPMSModeOn) {
	    if (info->IsSecondary) {
		if (info->DisplayType == MT_DFP) {
		    OUTREGP (RADEON_FP2_GEN_CNTL, 0, ~RADEON_FP2_BLANK_EN);
		    OUTREGP (RADEON_FP2_GEN_CNTL, RADEON_FP2_ON, ~RADEON_FP2_ON);
		    if (info->ChipFamily >= CHIP_FAMILY_R200) {
			OUTREGP (RADEON_FP2_GEN_CNTL, RADEON_FP2_DVO_EN, ~RADEON_FP2_DVO_EN);
		    }
		} else if (info->DisplayType == MT_CRT) {
		    RADEONDacPowerSet(pScrn, TRUE, !pRADEONEnt->ReversedDAC);
		}
	    } else {
		if ((info->MergedFB) && (info->MergeType == MT_DFP)) {
		    OUTREGP (RADEON_FP2_GEN_CNTL, 0, ~RADEON_FP2_BLANK_EN);
		    OUTREGP (RADEON_FP2_GEN_CNTL, RADEON_FP2_ON, ~RADEON_FP2_ON);
		    if (info->ChipFamily >= CHIP_FAMILY_R200) {
			OUTREGP (RADEON_FP2_GEN_CNTL, RADEON_FP2_DVO_EN, ~RADEON_FP2_DVO_EN);
		    }
		}
		if (info->DisplayType == MT_DFP) {
		    OUTREGP (RADEON_FP_GEN_CNTL, (RADEON_FP_FPON | RADEON_FP_TMDS_EN),
			     ~(RADEON_FP_FPON | RADEON_FP_TMDS_EN));
		} else if (info->DisplayType == MT_LCD) {

		    OUTREGP (RADEON_LVDS_GEN_CNTL, RADEON_LVDS_BLON, ~RADEON_LVDS_BLON);
		    usleep (info->PanelPwrDly * 1000);
		    OUTREGP (RADEON_LVDS_GEN_CNTL, RADEON_LVDS_ON, ~RADEON_LVDS_ON);
		} else if (info->DisplayType == MT_CRT) {
		    if ((pRADEONEnt->HasSecondary) || info->MergedFB) {
			RADEONDacPowerSet(pScrn, TRUE, pRADEONEnt->ReversedDAC);
		    } else {
			RADEONDacPowerSet(pScrn, TRUE, TRUE);
			if (info->HasCRTC2)
			    RADEONDacPowerSet(pScrn, TRUE, FALSE);
		    }
		}
	    }
	} else if ((PowerManagementMode == DPMSModeOff) ||
		   (PowerManagementMode == DPMSModeSuspend) ||
		   (PowerManagementMode == DPMSModeStandby)) {
	    if (info->IsSecondary) {
		if (info->DisplayType == MT_DFP) {
		    OUTREGP (RADEON_FP2_GEN_CNTL, RADEON_FP2_BLANK_EN, ~RADEON_FP2_BLANK_EN);
		    OUTREGP (RADEON_FP2_GEN_CNTL, 0, ~RADEON_FP2_ON);
		    if (info->ChipFamily >= CHIP_FAMILY_R200) {
			OUTREGP (RADEON_FP2_GEN_CNTL, 0, ~RADEON_FP2_DVO_EN);
		    }
		} else if (info->DisplayType == MT_CRT) {
		    RADEONDacPowerSet(pScrn, FALSE, !pRADEONEnt->ReversedDAC);
		}
	    } else {
		if ((info->MergedFB) && (info->MergeType == MT_DFP)) {
		    OUTREGP (RADEON_FP2_GEN_CNTL, RADEON_FP2_BLANK_EN, ~RADEON_FP2_BLANK_EN);
		    OUTREGP (RADEON_FP2_GEN_CNTL, 0, ~RADEON_FP2_ON);
		    if (info->ChipFamily >= CHIP_FAMILY_R200) {
			OUTREGP (RADEON_FP2_GEN_CNTL, 0, ~RADEON_FP2_DVO_EN);
		    }
		}
		if (info->DisplayType == MT_DFP) {
		    OUTREGP (RADEON_FP_GEN_CNTL, 0, ~(RADEON_FP_FPON | RADEON_FP_TMDS_EN));
		} else if (info->DisplayType == MT_LCD) {
		    unsigned long tmpPixclksCntl = INPLL(pScrn, RADEON_PIXCLKS_CNTL);

		    if (info->IsMobility || info->IsIGP) {
			/* Asic bug, when turning off LVDS_ON, we have to make sure
			   RADEON_PIXCLK_LVDS_ALWAYS_ON bit is off
			*/
			OUTPLLP(pScrn, RADEON_PIXCLKS_CNTL, 0, ~RADEON_PIXCLK_LVDS_ALWAYS_ONb);
		    }

		    OUTREGP (RADEON_LVDS_GEN_CNTL, 0,
			     ~(RADEON_LVDS_BLON | RADEON_LVDS_ON));

		    if (info->IsMobility || info->IsIGP) {
			OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmpPixclksCntl);
		    }
		} else if (info->DisplayType == MT_CRT) {
		    if ((pRADEONEnt->HasSecondary) || info->MergedFB) {
			RADEONDacPowerSet(pScrn, FALSE, pRADEONEnt->ReversedDAC);
		    } else {
			/* single CRT, turning both DACs off, we don't really know 
			 * which DAC is actually connected.
			 */
			RADEONDacPowerSet(pScrn, FALSE, TRUE);
			if (info->HasCRTC2) /* don't apply this to old radeon (singel CRTC) card */
			    RADEONDacPowerSet(pScrn, FALSE, FALSE);
		    }
		}
	    }
	}
    }

#ifdef XF86DRI
    if (info->CPStarted) DRIUnlock(pScrn->pScreen);
#endif
}

static void
RADEONGetMergedFBOptions(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr      info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt   = RADEONEntPriv(pScrn);
    char        *strptr;
    char	*default_hsync = "28-33";
    char	*default_vrefresh = "43-72";
    Bool	val;
    Bool	default_range = FALSE;
    static const char *mybadparm = "\"%s\" is is not a valid parameter for option \"%s\"\n";

    if (info->FBDev == TRUE) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	    "MergedFB does not work with Option UseFBDev, MergedFB mode is disabled\n");
	info->MergedFB = FALSE;
	return;
    }

			/* collect MergedFB options */
    info->MergedFB = TRUE;
    info->UseRADEONXinerama = TRUE;
    info->CRT2IsScrn0 = FALSE;
    info->CRT2Position = radeonClone;
    info->MergedFBXDPI = info->MergedFBYDPI = 0;
    info->CRT1XOffs = info->CRT1YOffs = info->CRT2XOffs = info->CRT2YOffs = 0;
    info->NonRect = info->HaveNonRect = info->HaveOffsRegions = FALSE;
    info->MBXNR1XMAX = info->MBXNR1YMAX = info->MBXNR2XMAX = info->MBXNR2YMAX = 65536;
    info->MouseRestrictions = TRUE;

    if (info->MergeType == MT_NONE) {
	info->MergedFB = FALSE;
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
        "Failed to detect secondary monitor, MergedFB/Clone mode disabled\n");
    } else if (!pRADEONEnt->MonInfo2) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	"Failed to detect secondary monitor DDC, default HSync and VRefresh used\n");
	default_range = TRUE;
    }

    if (xf86GetOptValBool(info->Options, OPTION_MERGEDFB, &val)) {
        if (val) {
	    info->MergedFB = TRUE;
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	    "MergedFB mode forced on.\n");
        } else {
	    info->MergedFB = FALSE;
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	    "MergedFB mode forced off.\n");
        }
    }

    /* Do some MergedFB mode initialisation */
    if(info->MergedFB) {
       info->CRT2pScrn = xalloc(sizeof(ScrnInfoRec));
       if(!info->CRT2pScrn) {
          xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	  	"Failed to allocate memory for merged pScrn, MergedFB mode is disabled\n");
	  info->MergedFB = FALSE;
       } else {
          memcpy(info->CRT2pScrn, pScrn, sizeof(ScrnInfoRec));
       }
    }
    if(info->MergedFB) {
	  int result, ival;
	  Bool valid = FALSE;
	  char *tempstr;
	  strptr = (char *)xf86GetOptValString(info->Options, OPTION_CRT2POS);
	  if (strptr) {
	      tempstr = xalloc(strlen(strptr) + 1);
	      result = sscanf(strptr, "%s %d", tempstr, &ival);
	  } else { 	      /* Not specified - default is "Clone" */
	      tempstr = NULL;
	      result = 0;
	      info->CRT2Position = radeonClone;
	      valid = TRUE;
	  }
	  if(result >= 1) {
       	        if(!xf86NameCmp(tempstr,"LeftOf")) {
                   info->CRT2Position = radeonLeftOf;
		   valid = TRUE;
		   if(result == 2) {
		      if(ival < 0) info->CRT1YOffs = -ival;
		      else info->CRT2YOffs = ival;
		   }
		   info->CRT2IsScrn0 = TRUE;
 	        } else if(!xf86NameCmp(tempstr,"RightOf")) {
                   info->CRT2Position = radeonRightOf;
		   valid = TRUE;
		   if(result == 2) {
		      if(ival < 0) info->CRT1YOffs = -ival;
		      else info->CRT2YOffs = ival;
		   }
		   info->CRT2IsScrn0 = FALSE;
	        } else if(!xf86NameCmp(tempstr,"Above")) {
                   info->CRT2Position = radeonAbove;
		   valid = TRUE;
		   if(result == 2) {
		      if(ival < 0) info->CRT1XOffs = -ival;
		      else info->CRT2XOffs = ival;
		   }
		   info->CRT2IsScrn0 = FALSE;
	        } else if(!xf86NameCmp(tempstr,"Below")) {
                   info->CRT2Position = radeonBelow;
		   valid = TRUE;
		   if(result == 2) {
		      if(ival < 0) info->CRT1XOffs = -ival;
		      else info->CRT2XOffs = ival;
		   }
		   info->CRT2IsScrn0 = TRUE;
	        } else if(!xf86NameCmp(tempstr,"Clone")) {
                   info->CRT2Position = radeonClone;
		   if(result == 1) valid = TRUE;
		   /*info->CRT2IsScrn0 = TRUE;*/
	        } 
	  }
	  if(!valid) {
	        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	            "\"%s\" is not a valid parameter for Option \"CRT2Position\"\n", strptr);
	    	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	            "Valid parameters are \"RightOf\", \"LeftOf\", \"Above\", \"Below\", or \"Clone\"\n");
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	            "Except for \"Clone\", the parameter may be followed by an integer.\n");
	  }
	  xfree(tempstr);

	  strptr = (char *)xf86GetOptValString(info->Options, OPTION_METAMODES);
	  if(strptr) {
	     info->MetaModes = xalloc(strlen(strptr) + 1);
	     if(info->MetaModes) memcpy(info->MetaModes, strptr, strlen(strptr) + 1);
	  }
	  strptr = (char *)xf86GetOptValString(info->Options, OPTION_CRT2HSYNC);
	  if(strptr) {
	      info->CRT2HSync = xalloc(strlen(strptr) + 1);
	      if(info->CRT2HSync) memcpy(info->CRT2HSync, strptr, strlen(strptr) + 1);
	  }
	  strptr = (char *)xf86GetOptValString(info->Options, OPTION_CRT2VREFRESH);
	  if(strptr) {
	      info->CRT2VRefresh = xalloc(strlen(strptr) + 1);
	      if(info->CRT2VRefresh) memcpy(info->CRT2VRefresh, strptr, strlen(strptr) + 1);
	  }

	if(xf86GetOptValBool(info->Options, OPTION_RADEONXINERAMA, &val)) {
	    if (!val)
		info->UseRADEONXinerama = FALSE;
	}
	if(info->UseRADEONXinerama) {
	        if(xf86GetOptValBool(info->Options, OPTION_CRT2ISSCRN0, &val)) {
		   if(val) info->CRT2IsScrn0 = TRUE;
		   else    info->CRT2IsScrn0 = FALSE;
		}
                if(xf86GetOptValBool(info->Options, OPTION_MERGEDFBNONRECT, &val)) {
  	            info->NonRect = val ? TRUE : FALSE;
  	        }
  	        if(xf86GetOptValBool(info->Options, OPTION_MERGEDFBMOUSER, &val)) {
  	            info->MouseRestrictions = val ? TRUE : FALSE;
  	        }
	}
	strptr = (char *)xf86GetOptValString(info->Options, OPTION_MERGEDDPI);
	if(strptr) {
	    int val1 = 0, val2 = 0;
	    sscanf(strptr, "%d %d", &val1, &val2);
	    if(val1 && val2) {
	        info->MergedFBXDPI = val1;
		info->MergedFBYDPI = val2;
	     } else {
	        xf86DrvMsg(pScrn->scrnIndex, X_WARNING, mybadparm, strptr, "MergedDPI");
	     }
 	}
    }

    if(info->MergedFB) {
          /* fill in monitor */
       info->CRT2pScrn->monitor = xalloc(sizeof(MonRec));
       if(info->CRT2pScrn->monitor) {
          DisplayModePtr tempm = NULL, currentm = NULL, newm = NULL;
          memcpy(info->CRT2pScrn->monitor, pScrn->monitor, sizeof(MonRec));
          info->CRT2pScrn->monitor->DDC = NULL;
	  info->CRT2pScrn->monitor->Modes = NULL;
	  info->CRT2pScrn->monitor->id = "CRT2 Monitor";
	  tempm = pScrn->monitor->Modes;
	  while(tempm) {
	         if(!(newm = xalloc(sizeof(DisplayModeRec)))) break;
	         memcpy(newm, tempm, sizeof(DisplayModeRec));
	         if(!(newm->name = xalloc(strlen(tempm->name) + 1))) {
	            xfree(newm);
		    break;
	         }
	         strcpy(newm->name, tempm->name);
	         if(!info->CRT2pScrn->monitor->Modes) 
		    info->CRT2pScrn->monitor->Modes = newm;
	         if(currentm) {
	            currentm->next = newm;
		    newm->prev = currentm;
	         }
	         currentm = newm;
	         tempm = tempm->next;
	  }

	  /* xf86SetDDCproperties(info->CRT2pScrn, pRADEONEnt->MonInfo2); */
          info->CRT2pScrn->monitor->DDC = pRADEONEnt->MonInfo2;
          if (default_range) {
             RADEONStrToRanges(info->CRT2pScrn->monitor->hsync, default_hsync, MAX_HSYNC);
             RADEONStrToRanges(info->CRT2pScrn->monitor->vrefresh, default_vrefresh, MAX_VREFRESH);
          }
          if(info->CRT2HSync) {
             info->CRT2pScrn->monitor->nHsync =
	    	RADEONStrToRanges(info->CRT2pScrn->monitor->hsync, info->CRT2HSync, MAX_HSYNC);
          }
          if(info->CRT2VRefresh) {
             info->CRT2pScrn->monitor->nVrefresh =
	    	RADEONStrToRanges(info->CRT2pScrn->monitor->vrefresh, info->CRT2VRefresh, MAX_VREFRESH);
          }

       } else {
          xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
	  	"Failed to allocate memory for CRT2 monitor, MergedFB mode disabled.\n");
	  if(info->CRT2pScrn) xfree(info->CRT2pScrn);
    	  info->CRT2pScrn = NULL;
	  info->MergedFB = FALSE;
       }
    }
}

static void RADEONForceSomeClocks(ScrnInfoPtr pScrn)
{
    /* It appears from r300 and rv100 may need some clocks forced-on */
     CARD32 tmp;

     tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
     tmp |= RADEON_SCLK_FORCE_CP | RADEON_SCLK_FORCE_VIP;
     OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);
}

static void RADEONSetDynamicClock(ScrnInfoPtr pScrn, int mode)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32 tmp;
    switch(mode) {
        case 0: /* Turn everything OFF (ForceON to everything)*/
            if ( !info->HasCRTC2 ) {
                tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
                tmp |= (RADEON_SCLK_FORCE_CP   | RADEON_SCLK_FORCE_HDP |
			RADEON_SCLK_FORCE_DISP1 | RADEON_SCLK_FORCE_TOP |
                        RADEON_SCLK_FORCE_E2   | RADEON_SCLK_FORCE_SE  |
			RADEON_SCLK_FORCE_IDCT | RADEON_SCLK_FORCE_VIP |
			RADEON_SCLK_FORCE_RE   | RADEON_SCLK_FORCE_PB  |
			RADEON_SCLK_FORCE_TAM  | RADEON_SCLK_FORCE_TDM |
                        RADEON_SCLK_FORCE_RB);
                OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);
            } else if (info->ChipFamily == CHIP_FAMILY_RV350) {
                /* for RV350/M10, no delays are required. */
                tmp = INPLL(pScrn, R300_SCLK_CNTL2);
                tmp |= (R300_SCLK_FORCE_TCL |
                        R300_SCLK_FORCE_GA  |
			R300_SCLK_FORCE_CBA);
                OUTPLL(pScrn, R300_SCLK_CNTL2, tmp);

                tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
                tmp |= (RADEON_SCLK_FORCE_DISP2 | RADEON_SCLK_FORCE_CP      |
                        RADEON_SCLK_FORCE_HDP   | RADEON_SCLK_FORCE_DISP1   |
                        RADEON_SCLK_FORCE_TOP   | RADEON_SCLK_FORCE_E2      |
                        R300_SCLK_FORCE_VAP     | RADEON_SCLK_FORCE_IDCT    |
			RADEON_SCLK_FORCE_VIP   | R300_SCLK_FORCE_SR        |
			R300_SCLK_FORCE_PX      | R300_SCLK_FORCE_TX        |
			R300_SCLK_FORCE_US      | RADEON_SCLK_FORCE_TV_SCLK |
                        R300_SCLK_FORCE_SU      | RADEON_SCLK_FORCE_OV0);
                OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);

                tmp = INPLL(pScrn, RADEON_SCLK_MORE_CNTL);
                tmp |= RADEON_SCLK_MORE_FORCEON;
                OUTPLL(pScrn, RADEON_SCLK_MORE_CNTL, tmp);

                tmp = INPLL(pScrn, RADEON_MCLK_CNTL);
                tmp |= (RADEON_FORCEON_MCLKA |
                        RADEON_FORCEON_MCLKB |
                        RADEON_FORCEON_YCLKA |
			RADEON_FORCEON_YCLKB |
                        RADEON_FORCEON_MC);
                OUTPLL(pScrn, RADEON_MCLK_CNTL, tmp);

                tmp = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);
                tmp &= ~(RADEON_PIXCLK_ALWAYS_ONb  | 
                         RADEON_PIXCLK_DAC_ALWAYS_ONb | 
			 R300_DISP_DAC_PIXCLK_DAC_BLANK_OFF); 
                OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, tmp);

                tmp = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
                tmp &= ~(RADEON_PIX2CLK_ALWAYS_ONb         | 
			 RADEON_PIX2CLK_DAC_ALWAYS_ONb     | 
			 RADEON_DISP_TVOUT_PIXCLK_TV_ALWAYS_ONb | 
			 R300_DVOCLK_ALWAYS_ONb            | 
			 RADEON_PIXCLK_BLEND_ALWAYS_ONb    | 
			 RADEON_PIXCLK_GV_ALWAYS_ONb       | 
			 R300_PIXCLK_DVO_ALWAYS_ONb        | 
			 RADEON_PIXCLK_LVDS_ALWAYS_ONb     | 
			 RADEON_PIXCLK_TMDS_ALWAYS_ONb     | 
			 R300_PIXCLK_TRANS_ALWAYS_ONb      | 
			 R300_PIXCLK_TVO_ALWAYS_ONb        | 
			 R300_P2G2CLK_ALWAYS_ONb            | 
			 R300_P2G2CLK_ALWAYS_ONb           | 
			 R300_DISP_DAC_PIXCLK_DAC2_BLANK_OFF); 
                OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmp);
            }  else {
                tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
                tmp |= (RADEON_SCLK_FORCE_CP | RADEON_SCLK_FORCE_E2);
                tmp |= RADEON_SCLK_FORCE_SE;

		if ( !info->HasCRTC2 ) {
                     tmp |= ( RADEON_SCLK_FORCE_RB    |
			      RADEON_SCLK_FORCE_TDM   |
			      RADEON_SCLK_FORCE_TAM   |
			      RADEON_SCLK_FORCE_PB    |
			      RADEON_SCLK_FORCE_RE    |
			      RADEON_SCLK_FORCE_VIP   |
			      RADEON_SCLK_FORCE_IDCT  |
			      RADEON_SCLK_FORCE_TOP   |
			      RADEON_SCLK_FORCE_DISP1 |
			      RADEON_SCLK_FORCE_DISP2 |
			      RADEON_SCLK_FORCE_HDP    );
		} else if ((info->ChipFamily == CHIP_FAMILY_R300) ||
			   (info->ChipFamily == CHIP_FAMILY_R350)) {
		    tmp |= ( RADEON_SCLK_FORCE_HDP   |
			     RADEON_SCLK_FORCE_DISP1 |
			     RADEON_SCLK_FORCE_DISP2 |
			     RADEON_SCLK_FORCE_TOP   |
			     RADEON_SCLK_FORCE_IDCT  |
			     RADEON_SCLK_FORCE_VIP);
		}
                OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);
            
                usleep(16000);

		if ((info->ChipFamily == CHIP_FAMILY_R300) ||
		    (info->ChipFamily == CHIP_FAMILY_R350)) {
                    tmp = INPLL(pScrn, R300_SCLK_CNTL2);
                    tmp |= ( R300_SCLK_FORCE_TCL |
			     R300_SCLK_FORCE_GA  |
			     R300_SCLK_FORCE_CBA);
                    OUTPLL(pScrn, R300_SCLK_CNTL2, tmp);
		    usleep(16000);
		}

                if (info->IsIGP) {
                    tmp = INPLL(pScrn, RADEON_MCLK_CNTL);
                    tmp &= ~(RADEON_FORCEON_MCLKA |
			     RADEON_FORCEON_YCLKA);
                    OUTPLL(pScrn, RADEON_MCLK_CNTL, tmp);
		    usleep(16000);
		}
  
		if ((info->ChipFamily == CHIP_FAMILY_RV200) ||
		    (info->ChipFamily == CHIP_FAMILY_RV250) ||
		    (info->ChipFamily == CHIP_FAMILY_RV280)) {
                    tmp = INPLL(pScrn, RADEON_SCLK_MORE_CNTL);
		    tmp |= RADEON_SCLK_MORE_FORCEON;
                    OUTPLL(pScrn, RADEON_SCLK_MORE_CNTL, tmp);
		    usleep(16000);
		}

                tmp = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
                tmp &= ~(RADEON_PIX2CLK_ALWAYS_ONb         |
                         RADEON_PIX2CLK_DAC_ALWAYS_ONb     |
                         RADEON_PIXCLK_BLEND_ALWAYS_ONb    |
                         RADEON_PIXCLK_GV_ALWAYS_ONb       |
                         RADEON_PIXCLK_DIG_TMDS_ALWAYS_ONb |
                         RADEON_PIXCLK_LVDS_ALWAYS_ONb     |
                         RADEON_PIXCLK_TMDS_ALWAYS_ONb);

		OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmp);
		usleep(16000);

                tmp = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);
                tmp &= ~(RADEON_PIXCLK_ALWAYS_ONb  |
			 RADEON_PIXCLK_DAC_ALWAYS_ONb); 
                OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, tmp);
	    }
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Dynamic Clock Scaling Disabled\n");
            break;
        case 1:
            if (!info->HasCRTC2) {
                tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
		if ((INREG(RADEON_CONFIG_CNTL) & RADEON_CFG_ATI_REV_ID_MASK) >
		    RADEON_CFG_ATI_REV_A13) { 
                    tmp &= ~(RADEON_SCLK_FORCE_CP | RADEON_SCLK_FORCE_RB);
                }
                tmp &= ~(RADEON_SCLK_FORCE_HDP  | RADEON_SCLK_FORCE_DISP1 |
			 RADEON_SCLK_FORCE_TOP  | RADEON_SCLK_FORCE_SE   |
			 RADEON_SCLK_FORCE_IDCT | RADEON_SCLK_FORCE_RE   |
			 RADEON_SCLK_FORCE_PB   | RADEON_SCLK_FORCE_TAM  |
			 RADEON_SCLK_FORCE_TDM);
                OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);
	    } else if ((info->ChipFamily == CHIP_FAMILY_R300) ||
		       (info->ChipFamily == CHIP_FAMILY_R350) ||
		       (info->ChipFamily == CHIP_FAMILY_RV350)) {
		if (info->ChipFamily == CHIP_FAMILY_RV350) {
		    tmp = INPLL(pScrn, R300_SCLK_CNTL2);
		    tmp &= ~(R300_SCLK_FORCE_TCL |
			     R300_SCLK_FORCE_GA  |
			     R300_SCLK_FORCE_CBA);
		    tmp |=  (R300_SCLK_TCL_MAX_DYN_STOP_LAT |
			     R300_SCLK_GA_MAX_DYN_STOP_LAT  |
			     R300_SCLK_CBA_MAX_DYN_STOP_LAT);
		    OUTPLL(pScrn, R300_SCLK_CNTL2, tmp);

		    tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
		    tmp &= ~(RADEON_SCLK_FORCE_DISP2 | RADEON_SCLK_FORCE_CP      |
			     RADEON_SCLK_FORCE_HDP   | RADEON_SCLK_FORCE_DISP1   |
			     RADEON_SCLK_FORCE_TOP   | RADEON_SCLK_FORCE_E2      |
			     R300_SCLK_FORCE_VAP     | RADEON_SCLK_FORCE_IDCT    |
			     RADEON_SCLK_FORCE_VIP   | R300_SCLK_FORCE_SR        |
			     R300_SCLK_FORCE_PX      | R300_SCLK_FORCE_TX        |
			     R300_SCLK_FORCE_US      | RADEON_SCLK_FORCE_TV_SCLK |
			     R300_SCLK_FORCE_SU      | RADEON_SCLK_FORCE_OV0);
		    tmp |=  RADEON_DYN_STOP_LAT_MASK;
		    OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);

		    tmp = INPLL(pScrn, RADEON_SCLK_MORE_CNTL);
		    tmp &= ~RADEON_SCLK_MORE_FORCEON;
		    tmp |=  RADEON_SCLK_MORE_MAX_DYN_STOP_LAT;
		    OUTPLL(pScrn, RADEON_SCLK_MORE_CNTL, tmp);

		    tmp = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);
		    tmp |= (RADEON_PIXCLK_ALWAYS_ONb |
			    RADEON_PIXCLK_DAC_ALWAYS_ONb);   
		    OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, tmp);

		    tmp = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
		    tmp |= (RADEON_PIX2CLK_ALWAYS_ONb         |
			    RADEON_PIX2CLK_DAC_ALWAYS_ONb     |
			    RADEON_DISP_TVOUT_PIXCLK_TV_ALWAYS_ONb |
			    R300_DVOCLK_ALWAYS_ONb            |   
			    RADEON_PIXCLK_BLEND_ALWAYS_ONb    |
			    RADEON_PIXCLK_GV_ALWAYS_ONb       |
			    R300_PIXCLK_DVO_ALWAYS_ONb        | 
			    RADEON_PIXCLK_LVDS_ALWAYS_ONb     |
			    RADEON_PIXCLK_TMDS_ALWAYS_ONb     |
			    R300_PIXCLK_TRANS_ALWAYS_ONb      |
			    R300_PIXCLK_TVO_ALWAYS_ONb        |
			    R300_P2G2CLK_ALWAYS_ONb           |
			    R300_P2G2CLK_ALWAYS_ONb);
		    OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmp);

		    tmp = INPLL(pScrn, RADEON_MCLK_MISC);
		    tmp |= (RADEON_MC_MCLK_DYN_ENABLE |
			    RADEON_IO_MCLK_DYN_ENABLE);
		    OUTPLL(pScrn, RADEON_MCLK_MISC, tmp);

		    tmp = INPLL(pScrn, RADEON_MCLK_CNTL);
		    tmp |= (RADEON_FORCEON_MCLKA |
			    RADEON_FORCEON_MCLKB);

		    tmp &= ~(RADEON_FORCEON_YCLKA  |
			     RADEON_FORCEON_YCLKB  |
			     RADEON_FORCEON_MC);

		    /* Some releases of vbios have set DISABLE_MC_MCLKA
		       and DISABLE_MC_MCLKB bits in the vbios table.  Setting these
		       bits will cause H/W hang when reading video memory with dynamic clocking
		       enabled. */
		    if ((tmp & R300_DISABLE_MC_MCLKA) &&
			(tmp & R300_DISABLE_MC_MCLKB)) {
			/* If both bits are set, then check the active channels */
			tmp = INPLL(pScrn, RADEON_MCLK_CNTL);
			if (info->RamWidth == 64) {
			    if (INREG(RADEON_MEM_CNTL) & R300_MEM_USE_CD_CH_ONLY)
				tmp &= ~R300_DISABLE_MC_MCLKB;
			    else
				tmp &= ~R300_DISABLE_MC_MCLKA;
			} else {
			    tmp &= ~(R300_DISABLE_MC_MCLKA |
				     R300_DISABLE_MC_MCLKB);
			}
		    }

		    OUTPLL(pScrn, RADEON_MCLK_CNTL, tmp);
		} else {
		    tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
		    tmp &= ~(R300_SCLK_FORCE_VAP);
		    tmp |= RADEON_SCLK_FORCE_CP;
		    OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);
		    usleep(15000);

		    tmp = INPLL(pScrn, R300_SCLK_CNTL2);
		    tmp &= ~(R300_SCLK_FORCE_TCL |
			     R300_SCLK_FORCE_GA  |
			     R300_SCLK_FORCE_CBA);
		    OUTPLL(pScrn, R300_SCLK_CNTL2, tmp);
		}
	    } else {
                tmp = INPLL(pScrn, RADEON_CLK_PWRMGT_CNTL);

                tmp &= ~(RADEON_ACTIVE_HILO_LAT_MASK     | 
			 RADEON_DISP_DYN_STOP_LAT_MASK   | 
			 RADEON_DYN_STOP_MODE_MASK); 

                tmp |= (RADEON_ENGIN_DYNCLK_MODE |
			(0x01 << RADEON_ACTIVE_HILO_LAT_SHIFT));
                OUTPLL(pScrn, RADEON_CLK_PWRMGT_CNTL, tmp);
		usleep(15000);

                tmp = INPLL(pScrn, RADEON_CLK_PIN_CNTL);
                tmp |= RADEON_SCLK_DYN_START_CNTL; 
                OUTPLL(pScrn, RADEON_CLK_PIN_CNTL, tmp);
		usleep(15000);

		/* When DRI is enabled, setting DYN_STOP_LAT to zero can cause some R200 
		   to lockup randomly, leave them as set by BIOS.
		*/
                tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
                /*tmp &= RADEON_SCLK_SRC_SEL_MASK;*/
		tmp &= ~RADEON_SCLK_FORCEON_MASK;

                /*RAGE_6::A11 A12 A12N1 A13, RV250::A11 A12, R300*/
		if (((info->ChipFamily == CHIP_FAMILY_RV250) &&
		     ((INREG(RADEON_CONFIG_CNTL) & RADEON_CFG_ATI_REV_ID_MASK) <
		      RADEON_CFG_ATI_REV_A13)) || 
		    ((info->ChipFamily == CHIP_FAMILY_RV100) &&
		     ((INREG(RADEON_CONFIG_CNTL) & RADEON_CFG_ATI_REV_ID_MASK) <=
		      RADEON_CFG_ATI_REV_A13))){
                    tmp |= RADEON_SCLK_FORCE_CP;
                    tmp |= RADEON_SCLK_FORCE_VIP;
                }

                OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);

		if ((info->ChipFamily == CHIP_FAMILY_RV200) ||
		    (info->ChipFamily == CHIP_FAMILY_RV250) ||
		    (info->ChipFamily == CHIP_FAMILY_RV280)) {
                    tmp = INPLL(pScrn, RADEON_SCLK_MORE_CNTL);
                    tmp &= ~RADEON_SCLK_MORE_FORCEON;

                    /* RV200::A11 A12 RV250::A11 A12 */
		    if (((info->ChipFamily == CHIP_FAMILY_RV200) ||
			 (info->ChipFamily == CHIP_FAMILY_RV250)) &&
			((INREG(RADEON_CONFIG_CNTL) & RADEON_CFG_ATI_REV_ID_MASK) <
			 RADEON_CFG_ATI_REV_A13)) {
                        tmp |= RADEON_SCLK_MORE_FORCEON;
		    }
                    OUTPLL(pScrn, RADEON_SCLK_MORE_CNTL, tmp);
		    usleep(15000);
                }

                /* RV200::A11 A12, RV250::A11 A12 */
                if (((info->ChipFamily == CHIP_FAMILY_RV200) ||
		     (info->ChipFamily == CHIP_FAMILY_RV250)) &&
		    ((INREG(RADEON_CONFIG_CNTL) & RADEON_CFG_ATI_REV_ID_MASK) <
		     RADEON_CFG_ATI_REV_A13)) {
                    tmp = INPLL(pScrn, RADEON_PLL_PWRMGT_CNTL);
                    tmp |= RADEON_TCL_BYPASS_DISABLE;
                    OUTPLL(pScrn, RADEON_PLL_PWRMGT_CNTL, tmp);
                }
		usleep(15000);

                /*enable dynamic mode for display clocks (PIXCLK and PIX2CLK)*/
		tmp = INPLL(pScrn, RADEON_PIXCLKS_CNTL);
		tmp |=  (RADEON_PIX2CLK_ALWAYS_ONb         |
			 RADEON_PIX2CLK_DAC_ALWAYS_ONb     |
			 RADEON_PIXCLK_BLEND_ALWAYS_ONb    |
			 RADEON_PIXCLK_GV_ALWAYS_ONb       |
			 RADEON_PIXCLK_DIG_TMDS_ALWAYS_ONb |
			 RADEON_PIXCLK_LVDS_ALWAYS_ONb     |
			 RADEON_PIXCLK_TMDS_ALWAYS_ONb);

		OUTPLL(pScrn, RADEON_PIXCLKS_CNTL, tmp);
		usleep(15000);

		tmp = INPLL(pScrn, RADEON_VCLK_ECP_CNTL);
		tmp |= (RADEON_PIXCLK_ALWAYS_ONb  |
		        RADEON_PIXCLK_DAC_ALWAYS_ONb); 

                OUTPLL(pScrn, RADEON_VCLK_ECP_CNTL, tmp);
		usleep(15000);
            }    
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Dynamic Clock Scaling Enabled\n");
	    break;
        default:
	    break;
    }
}

_X_EXPORT void RADEONFillInScreenInfo(ScrnInfoPtr pScrn)
{
    pScrn->driverVersion = RADEON_VERSION_CURRENT;
    pScrn->driverName    = RADEON_DRIVER_NAME;
    pScrn->name          = RADEON_NAME;
    pScrn->PreInit       = RADEONPreInit;
    pScrn->ScreenInit    = RADEONScreenInit;
    pScrn->SwitchMode    = RADEONSwitchMode;
#ifdef X_XF86MiscPassMessage
    pScrn->HandleMessage = RADEONHandleMessage;
#endif
    pScrn->AdjustFrame   = RADEONAdjustFrame;
    pScrn->EnterVT       = RADEONEnterVT;
    pScrn->LeaveVT       = RADEONLeaveVT;
    pScrn->FreeScreen    = RADEONFreeScreen;
    pScrn->ValidMode     = RADEONValidMode;
}
