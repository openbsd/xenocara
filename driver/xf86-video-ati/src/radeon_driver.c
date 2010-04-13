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
 *   shadowfb
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
#include "radeon_atombios.h"

#ifdef XF86DRI
#define _XF86DRI_SERVER_
#include "radeon_dri.h"
#include "radeon_drm.h"
#include "sarea.h"
#endif

#include "fb.h"

				/* colormap initialization */
#include "micmap.h"
#include "dixstruct.h"

				/* X and server generic header files */
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86RAC.h"
#include "xf86RandR12.h"
#include "xf86Resources.h"
#include "xf86cmap.h"
#include "vbe.h"

#include "shadow.h"
				/* vgaHW definitions */
#ifdef WITH_VGAHW
#include "vgaHW.h"
#endif

#ifdef HAVE_X11_EXTENSIONS_DPMSCONST_H
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include "atipciids.h"
#include "radeon_chipset_gen.h"


#include "radeon_chipinfo_gen.h"

				/* Forward definitions for driver functions */
static Bool RADEONCloseScreen(int scrnIndex, ScreenPtr pScreen);
static Bool RADEONSaveScreen(ScreenPtr pScreen, int mode);
static void RADEONSave(ScrnInfoPtr pScrn);

static void RADEONSetDynamicClock(ScrnInfoPtr pScrn, int mode);
static void RADEONForceSomeClocks(ScrnInfoPtr pScrn);
static void RADEONSaveMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save);

static void
RADEONSaveBIOSRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save);

#ifdef XF86DRI
static void RADEONAdjustMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save);
#endif

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
    { OPTION_PCIAPER_SIZE,  "PCIAPERSize",      OPTV_INTEGER, {0}, FALSE },
#ifdef USE_EXA
    { OPTION_ACCEL_DFS,      "AccelDFS",         OPTV_BOOLEAN, {0}, FALSE },
#endif
#endif
    { OPTION_IGNORE_EDID,    "IgnoreEDID",       OPTV_BOOLEAN, {0}, FALSE },
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
    { OPTION_RAGE_THEATRE_MICROC_PATH,    "RageTheatreMicrocPath",    OPTV_STRING, {0}, FALSE },
    { OPTION_RAGE_THEATRE_MICROC_TYPE,    "RageTheatreMicrocType",    OPTV_STRING, {0}, FALSE },
    { OPTION_SCALER_WIDTH,                "ScalerWidth",              OPTV_INTEGER, {0}, FALSE }, 
#endif
#ifdef RENDER
    { OPTION_RENDER_ACCEL,   "RenderAccel",      OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_SUBPIXEL_ORDER, "SubPixelOrder",    OPTV_ANYSTR,  {0}, FALSE },
#endif
    { OPTION_SHOWCACHE,      "ShowCache",        OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DYNAMIC_CLOCKS, "DynamicClocks",    OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_VGA_ACCESS,     "VGAAccess",        OPTV_BOOLEAN, {0}, TRUE  },
    { OPTION_REVERSE_DDC,    "ReverseDDC",       OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_LVDS_PROBE_PLL, "LVDSProbePLL",     OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_ACCELMETHOD,    "AccelMethod",      OPTV_STRING,  {0}, FALSE },
    { OPTION_DRI,            "DRI",       	 OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_CONNECTORTABLE, "ConnectorTable",   OPTV_STRING,  {0}, FALSE },
    { OPTION_DEFAULT_CONNECTOR_TABLE, "DefaultConnectorTable", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DEFAULT_TMDS_PLL, "DefaultTMDSPLL", OPTV_BOOLEAN, {0}, FALSE },
#if defined(__powerpc__)
    { OPTION_MAC_MODEL,      "MacModel",         OPTV_STRING,  {0}, FALSE },
#endif
    { OPTION_TVDAC_LOAD_DETECT, "TVDACLoadDetect", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_FORCE_TVOUT,    "ForceTVOut",         OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_TVSTD,          "TVStandard",         OPTV_STRING,  {0}, FALSE },
    { OPTION_IGNORE_LID_STATUS, "IgnoreLidStatus", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_DEFAULT_TVDAC_ADJ, "DefaultTVDACAdj", OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_INT10,             "Int10",           OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_EXA_VSYNC,         "EXAVSync",        OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_ATOM_TVOUT,	"ATOMTVOut",	   OPTV_BOOLEAN, {0}, FALSE },
    { OPTION_R4XX_ATOM,	        "R4xxATOM",	   OPTV_BOOLEAN, {0}, FALSE },
    { -1,                    NULL,               OPTV_NONE,    {0}, FALSE }
};

const OptionInfoRec *RADEONOptionsWeak(void) { return RADEONOptions; }

extern _X_EXPORT int gRADEONEntityIndex;

static int getRADEONEntityIndex(void)
{
    return gRADEONEntityIndex;
}

struct RADEONInt10Save {
	uint32_t MEM_CNTL;
	uint32_t MEMSIZE;
	uint32_t MPP_TB_CONFIG;
};

static Bool RADEONMapMMIO(ScrnInfoPtr pScrn);
static Bool RADEONUnmapMMIO(ScrnInfoPtr pScrn);

static void *
radeonShadowWindow(ScreenPtr screen, CARD32 row, CARD32 offset, int mode,
		   CARD32 *size, void *closure)
{
    ScrnInfoPtr pScrn = xf86Screens[screen->myNum];
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    int stride;

    stride = (pScrn->displayWidth * pScrn->bitsPerPixel) / 8;
    *size = stride;

    return ((uint8_t *)info->FB + row * stride + offset);
}
static Bool
RADEONCreateScreenResources (ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   RADEONInfoPtr  info   = RADEONPTR(pScrn);
   PixmapPtr pixmap;

   pScreen->CreateScreenResources = info->CreateScreenResources;
   if (!(*pScreen->CreateScreenResources)(pScreen))
      return FALSE;
   pScreen->CreateScreenResources = RADEONCreateScreenResources;

   if (info->r600_shadow_fb) {
       pixmap = pScreen->GetScreenPixmap(pScreen);

       if (!shadowAdd(pScreen, pixmap, shadowUpdatePackedWeak(),
		      radeonShadowWindow, 0, NULL))
	   return FALSE;
   }
   return TRUE;
}

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
    uint32_t       CardTmp;
    static struct  RADEONInt10Save SaveStruct = { 0, 0, 0 };

    if (!IS_AVIVO_VARIANT) {
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
    }

    *pPtr = (void *)&SaveStruct;
}

static void
RADEONPostInt10Check(ScrnInfoPtr pScrn, void *ptr)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    struct RADEONInt10Save *pSave = ptr;
    uint32_t CardTmp;

    /* If we don't have a valid (non-zero) saved MEM_CNTL, get out now */
    if (!pSave || !pSave->MEM_CNTL)
	return;

    if (IS_AVIVO_VARIANT)
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
    RADEONInfoPtr  info;
    int i;

    if (!pScrn || !pScrn->driverPrivate) return;

    info = RADEONPTR(pScrn);

    if (info->cp) {
	xfree(info->cp);
	info->cp = NULL;
    }

    if (info->dri) {
	xfree(info->dri);
	info->dri = NULL;
    }

    if (info->accel_state) {
	xfree(info->accel_state);
	info->accel_state = NULL;
    }

    for (i = 0; i < RADEON_MAX_BIOS_CONNECTOR; i++) {
	if (info->encoders[i]) {
	    if (info->encoders[i]->dev_priv) {
		xfree(info->encoders[i]->dev_priv);
		info->encoders[i]->dev_priv = NULL;
	    }
	    xfree(info->encoders[i]);
	    info->encoders[i]= NULL;
	}
    }

    xfree(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
}

/* Memory map the MMIO region.  Used during pre-init and by RADEONMapMem,
 * below
 */
static Bool RADEONMapMMIO(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

    if (pRADEONEnt->MMIO) {
        info->MMIO = pRADEONEnt->MMIO;
        return TRUE;
    }

#ifndef XSERVER_LIBPCIACCESS

    info->MMIO = xf86MapPciMem(pScrn->scrnIndex,
			       VIDMEM_MMIO | VIDMEM_READSIDEEFFECT,
			       info->PciTag,
			       info->MMIOAddr,
			       info->MMIOSize);

    if (!info->MMIO) return FALSE;
#else

    void** result = (void**)&info->MMIO;
    int err = pci_device_map_range(info->PciInfo,
				   info->MMIOAddr,
				   info->MMIOSize,
				   PCI_DEV_MAP_FLAG_WRITABLE,
				   result);

    if (err) {
	xf86DrvMsg (pScrn->scrnIndex, X_ERROR,
                    "Unable to map MMIO aperture. %s (%d)\n",
                    strerror (err), err);
	return FALSE;
    }

#endif

    pRADEONEnt->MMIO = info->MMIO;
    return TRUE;
}

/* Unmap the MMIO region.  Used during pre-init and by RADEONUnmapMem,
 * below
 */
static Bool RADEONUnmapMMIO(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

    if (info->IsPrimary || info->IsSecondary) {
      /* never unmap on zaphod */
      info->MMIO = NULL;
      return TRUE;
    }

#ifndef XSERVER_LIBPCIACCESS
    xf86UnMapVidMem(pScrn->scrnIndex, info->MMIO, info->MMIOSize);
#else
    pci_device_unmap_range(info->PciInfo, info->MMIO, info->MMIOSize);
#endif

    pRADEONEnt->MMIO = NULL;
    info->MMIO = NULL;
    return TRUE;
}

/* Memory map the frame buffer.  Used by RADEONMapMem, below. */
static Bool RADEONMapFB(ScrnInfoPtr pScrn)
{
#ifdef XSERVER_LIBPCIACCESS
    int err;
#endif
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Map: 0x%016llx, 0x%08lx\n", info->LinearAddr, info->FbMapSize);

#ifndef XSERVER_LIBPCIACCESS

    info->FB = xf86MapPciMem(pScrn->scrnIndex,
			     VIDMEM_FRAMEBUFFER,
			     info->PciTag,
			     info->LinearAddr,
			     info->FbMapSize);

    if (!info->FB) return FALSE;

#else

    err = pci_device_map_range(info->PciInfo,
				   info->LinearAddr,
				   info->FbMapSize,
				   PCI_DEV_MAP_FLAG_WRITABLE |
				   PCI_DEV_MAP_FLAG_WRITE_COMBINE,
				   &info->FB);

    if (err) {
	xf86DrvMsg (pScrn->scrnIndex, X_ERROR,
                    "Unable to map FB aperture. %s (%d)\n",
                    strerror (err), err);
	return FALSE;
    }

#endif

    return TRUE;
}

/* Unmap the frame buffer.  Used by RADEONUnmapMem, below. */
static Bool RADEONUnmapFB(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

#ifndef XSERVER_LIBPCIACCESS
    xf86UnMapVidMem(pScrn->scrnIndex, info->FB, info->FbMapSize);
#else
    pci_device_unmap_range(info->PciInfo, info->FB, info->FbMapSize);
#endif

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
	uint32_t save, tmp;

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
    uint32_t       data;

    OUTREG8(RADEON_CLOCK_CNTL_INDEX, addr & 0x3f);
    RADEONPllErrataAfterIndex(info);
    data = INREG(RADEON_CLOCK_CNTL_DATA);
    RADEONPllErrataAfterData(info);

    return data;
}

/* Write PLL information */
void RADEONOUTPLL(ScrnInfoPtr pScrn, int addr, uint32_t data)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG8(RADEON_CLOCK_CNTL_INDEX, (((addr) & 0x3f) |
				      RADEON_PLL_WR_EN));
    RADEONPllErrataAfterIndex(info);
    OUTREG(RADEON_CLOCK_CNTL_DATA, data);
    RADEONPllErrataAfterData(info);
}

/* Read MC register */
unsigned RADEONINMC(ScrnInfoPtr pScrn, int addr)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t       data;

    if ((info->ChipFamily == CHIP_FAMILY_RS690) ||
	(info->ChipFamily == CHIP_FAMILY_RS740)) {
	OUTREG(RS690_MC_INDEX, (addr & RS690_MC_INDEX_MASK));
	data = INREG(RS690_MC_DATA);
    } else if (info->ChipFamily == CHIP_FAMILY_RS600) {
	OUTREG(RS600_MC_INDEX, ((addr & RS600_MC_ADDR_MASK) | RS600_MC_IND_CITF_ARB0));
	data = INREG(RS600_MC_DATA);
    } else if (IS_AVIVO_VARIANT) {
	OUTREG(AVIVO_MC_INDEX, (addr & 0xff) | 0x7f0000);
	(void)INREG(AVIVO_MC_INDEX);
	data = INREG(AVIVO_MC_DATA);

	OUTREG(AVIVO_MC_INDEX, 0);
	(void)INREG(AVIVO_MC_INDEX);
    } else {
	OUTREG(R300_MC_IND_INDEX, addr & 0x3f);
	(void)INREG(R300_MC_IND_INDEX);
	data = INREG(R300_MC_IND_DATA);

	OUTREG(R300_MC_IND_INDEX, 0);
	(void)INREG(R300_MC_IND_INDEX);
    }

    return data;
}

/* Write MC information */
void RADEONOUTMC(ScrnInfoPtr pScrn, int addr, uint32_t data)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if ((info->ChipFamily == CHIP_FAMILY_RS690) ||
	(info->ChipFamily == CHIP_FAMILY_RS740)) {
	OUTREG(RS690_MC_INDEX, ((addr & RS690_MC_INDEX_MASK) |
				RS690_MC_INDEX_WR_EN));
	OUTREG(RS690_MC_DATA, data);
	OUTREG(RS690_MC_INDEX, RS690_MC_INDEX_WR_ACK);
    } else if (info->ChipFamily == CHIP_FAMILY_RS600) {
	OUTREG(RS600_MC_INDEX, ((addr & RS600_MC_ADDR_MASK) |
				RS600_MC_IND_CITF_ARB0 |
				RS600_MC_IND_WR_EN));
	OUTREG(RS600_MC_DATA, data);
    } else if (IS_AVIVO_VARIANT) {
	OUTREG(AVIVO_MC_INDEX, (addr & 0xff) | 0xff0000);
	(void)INREG(AVIVO_MC_INDEX);
	OUTREG(AVIVO_MC_DATA, data);
	OUTREG(AVIVO_MC_INDEX, 0);
	(void)INREG(AVIVO_MC_INDEX);
    } else {
	OUTREG(R300_MC_IND_INDEX, (((addr) & 0x3f) |
				   R300_MC_IND_WR_EN));
	(void)INREG(R300_MC_IND_INDEX);
	OUTREG(R300_MC_IND_DATA, data);
	OUTREG(R300_MC_IND_INDEX, 0);
	(void)INREG(R300_MC_IND_INDEX);
    }
}

/* Read PCIE register */
unsigned RADEONINPCIE(ScrnInfoPtr pScrn, int addr)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    CARD32         data;

    OUTREG(RADEON_PCIE_INDEX, addr & 0xff);
    data = INREG(RADEON_PCIE_DATA);

    return data;
}

/* Write PCIE register */
void RADEONOUTPCIE(ScrnInfoPtr pScrn, int addr, uint32_t data)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    OUTREG(RADEON_PCIE_INDEX, ((addr) & 0xff));
    OUTREG(RADEON_PCIE_DATA, data);
}

static Bool radeon_get_mc_idle(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (info->ChipFamily >= CHIP_FAMILY_R600) {
	if (INREG(R600_SRBM_STATUS) & 0x3f00)
	    return FALSE;
	else
	    return TRUE;
    } else if (info->ChipFamily == CHIP_FAMILY_RV515) {
	if (INMC(pScrn, RV515_MC_STATUS) & RV515_MC_STATUS_IDLE)
	    return TRUE;
	else
	    return FALSE;
    } else if (info->ChipFamily == CHIP_FAMILY_RS600) {
	if (INMC(pScrn, RS600_MC_STATUS) & RS600_MC_IDLE)
	    return TRUE;
	else
	    return FALSE;
    } else if ((info->ChipFamily == CHIP_FAMILY_RS690) ||
	       (info->ChipFamily == CHIP_FAMILY_RS740)) {
	if (INMC(pScrn, RS690_MC_STATUS) & RS690_MC_STATUS_IDLE)
	    return TRUE;
	else
	    return FALSE;
    } else if (info->ChipFamily >= CHIP_FAMILY_R520) {
	if (INMC(pScrn, R520_MC_STATUS) & R520_MC_STATUS_IDLE)
	    return TRUE;
	else
	    return FALSE;
    } else if (IS_R300_VARIANT) {
	if (INREG(RADEON_MC_STATUS) & R300_MC_IDLE)
	    return TRUE;
	else
	    return FALSE;
    } else {
	if (INREG(RADEON_MC_STATUS) & RADEON_MC_IDLE)
	    return TRUE;
	else
	    return FALSE;
    }
}

#define LOC_FB 0x1
#define LOC_AGP 0x2
static void radeon_write_mc_fb_agp_location(ScrnInfoPtr pScrn, int mask, uint32_t fb_loc, uint32_t agp_loc, uint32_t agp_loc_hi)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (info->ChipFamily >= CHIP_FAMILY_RV770) {
	if (mask & LOC_FB)
	    OUTREG(R700_MC_VM_FB_LOCATION, fb_loc);
	if (mask & LOC_AGP) {
	    OUTREG(R700_MC_VM_AGP_BOT, agp_loc);
	    OUTREG(R700_MC_VM_AGP_TOP, agp_loc_hi);
	}
    } else if (info->ChipFamily >= CHIP_FAMILY_R600) {
	if (mask & LOC_FB)
	    OUTREG(R600_MC_VM_FB_LOCATION, fb_loc);
	if (mask & LOC_AGP) {
	    OUTREG(R600_MC_VM_AGP_BOT, agp_loc);
	    OUTREG(R600_MC_VM_AGP_TOP, agp_loc_hi);
	}
    } else if (info->ChipFamily == CHIP_FAMILY_RV515) {
	if (mask & LOC_FB)
	    OUTMC(pScrn, RV515_MC_FB_LOCATION, fb_loc);
	if (mask & LOC_AGP)
	    OUTMC(pScrn, RV515_MC_AGP_LOCATION, agp_loc);
	(void)INMC(pScrn, RV515_MC_AGP_LOCATION);
    } else if (info->ChipFamily == CHIP_FAMILY_RS600) {
	if (mask & LOC_FB)
	    OUTMC(pScrn, RS600_MC_FB_LOCATION, fb_loc);
	if (mask & LOC_AGP)
	    OUTMC(pScrn, RS600_MC_AGP_LOCATION, agp_loc);
    } else if ((info->ChipFamily == CHIP_FAMILY_RS690) ||
	       (info->ChipFamily == CHIP_FAMILY_RS740)) {
	if (mask & LOC_FB)
	    OUTMC(pScrn, RS690_MC_FB_LOCATION, fb_loc);
	if (mask & LOC_AGP)
	    OUTMC(pScrn, RS690_MC_AGP_LOCATION, agp_loc);
    } else if (info->ChipFamily >= CHIP_FAMILY_R520) {
	if (mask & LOC_FB)
	    OUTMC(pScrn, R520_MC_FB_LOCATION, fb_loc);
	if (mask & LOC_AGP)
	    OUTMC(pScrn, R520_MC_AGP_LOCATION, agp_loc);
	(void)INMC(pScrn, R520_MC_FB_LOCATION);
    } else {
	if (mask & LOC_FB)
	    OUTREG(RADEON_MC_FB_LOCATION, fb_loc);
	if (mask & LOC_AGP)
	    OUTREG(RADEON_MC_AGP_LOCATION, agp_loc);
    }
}

static void radeon_read_mc_fb_agp_location(ScrnInfoPtr pScrn, int mask, uint32_t *fb_loc, uint32_t *agp_loc, uint32_t *agp_loc_hi)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (info->ChipFamily >= CHIP_FAMILY_RV770) {
	if (mask & LOC_FB)
	    *fb_loc = INREG(R700_MC_VM_FB_LOCATION);
	if (mask & LOC_AGP) {
	    *agp_loc = INREG(R700_MC_VM_AGP_BOT);
	    *agp_loc_hi = INREG(R700_MC_VM_AGP_TOP);
	}
    } else if (info->ChipFamily >= CHIP_FAMILY_R600) {
	if (mask & LOC_FB)
	    *fb_loc = INREG(R600_MC_VM_FB_LOCATION);
	if (mask & LOC_AGP) {
	    *agp_loc = INREG(R600_MC_VM_AGP_BOT);
	    *agp_loc_hi = INREG(R600_MC_VM_AGP_TOP);
	}
    } else if (info->ChipFamily == CHIP_FAMILY_RV515) {
	if (mask & LOC_FB)
	    *fb_loc = INMC(pScrn, RV515_MC_FB_LOCATION);
	if (mask & LOC_AGP) {
	    *agp_loc = INMC(pScrn, RV515_MC_AGP_LOCATION);
	    *agp_loc_hi = 0;
	}
    } else if (info->ChipFamily == CHIP_FAMILY_RS600) {
	if (mask & LOC_FB)
	    *fb_loc = INMC(pScrn, RS600_MC_FB_LOCATION);
	if (mask & LOC_AGP) {
	    *agp_loc = INMC(pScrn, RS600_MC_AGP_LOCATION);
	    *agp_loc_hi = 0;
	}
    } else if ((info->ChipFamily == CHIP_FAMILY_RS690) ||
	       (info->ChipFamily == CHIP_FAMILY_RS740)) {
	if (mask & LOC_FB)
	    *fb_loc = INMC(pScrn, RS690_MC_FB_LOCATION);
	if (mask & LOC_AGP) {
	    *agp_loc = INMC(pScrn, RS690_MC_AGP_LOCATION);
	    *agp_loc_hi = 0;
	}
    } else if (info->ChipFamily >= CHIP_FAMILY_R520) {
	if (mask & LOC_FB)
	    *fb_loc = INMC(pScrn, R520_MC_FB_LOCATION);
	if (mask & LOC_AGP) {
	    *agp_loc = INMC(pScrn, R520_MC_AGP_LOCATION);
	    *agp_loc_hi = 0;
	}
    } else {
	if (mask & LOC_FB)
	    *fb_loc = INREG(RADEON_MC_FB_LOCATION);
	if (mask & LOC_AGP)
	    *agp_loc = INREG(RADEON_MC_AGP_LOCATION);
    }
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
    uint32_t       crtc_gen_cntl;
    struct timeval timeout;

    crtc_gen_cntl = INREG(RADEON_CRTC_GEN_CNTL);
    if ((crtc_gen_cntl & RADEON_CRTC_DISP_REQ_EN_B) ||
	!(crtc_gen_cntl & RADEON_CRTC_EN))
	return;

    /* Clear the CRTC_VBLANK_SAVE bit */
    OUTREG(RADEON_CRTC_STATUS, RADEON_CRTC_VBLANK_SAVE_CLEAR);

    /* Wait for it to go back up */
    radeon_init_timeout(&timeout, RADEON_VSYNC_TIMEOUT);
    while (!(INREG(RADEON_CRTC_STATUS) & RADEON_CRTC_VBLANK_SAVE) &&
        !radeon_timedout(&timeout))
	usleep(100);
}

/* Wait for vertical sync on secondary CRTC */
void RADEONWaitForVerticalSync2(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t       crtc2_gen_cntl;
    struct timeval timeout;
 
    crtc2_gen_cntl = INREG(RADEON_CRTC2_GEN_CNTL);
    if ((crtc2_gen_cntl & RADEON_CRTC2_DISP_REQ_EN_B) ||
	!(crtc2_gen_cntl & RADEON_CRTC2_EN))
	return;

    /* Clear the CRTC2_VBLANK_SAVE bit */
    OUTREG(RADEON_CRTC2_STATUS, RADEON_CRTC2_VBLANK_SAVE_CLEAR);

    /* Wait for it to go back up */
    radeon_init_timeout(&timeout, RADEON_VSYNC_TIMEOUT);
    while (!(INREG(RADEON_CRTC2_STATUS) & RADEON_CRTC2_VBLANK_SAVE) &&
        !radeon_timedout(&timeout))
	usleep(100);
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
    long total_usecs;
    struct timeval start, stop, to1, to2;
    unsigned int f1, f2, f3;
    int tries = 0;

    prev_xtal = 0;
 again:
    xtal = 0;
    if (++tries > 10)
           goto failed;

    gettimeofday(&to1, NULL);
    f1 = INREG(RADEON_CRTC_CRNT_FRAME);
    for (;;) {
       f2 = INREG(RADEON_CRTC_CRNT_FRAME);
       if (f1 != f2)
	    break;
       gettimeofday(&to2, NULL);
       if ((to2.tv_sec - to1.tv_sec) > 1) {
           xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Clock not counting...\n");
           goto failed;
       }
    }
    gettimeofday(&start, NULL);
    for(;;) {
       f3 = INREG(RADEON_CRTC_CRNT_FRAME);
       if (f3 != f2)
	    break;
       gettimeofday(&to2, NULL);
       if ((to2.tv_sec - start.tv_sec) > 1)
           goto failed;
    }
    gettimeofday(&stop, NULL);

    if ((stop.tv_sec - start.tv_sec) != 0)
           goto again;
    total_usecs = abs(stop.tv_usec - start.tv_usec);
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
       uint32_t tmp;
       tmp = INPLL(pScrn, RADEON_PPLL_REF_DIV);
       if (IS_R300_VARIANT
	   || (info->ChipFamily == CHIP_FAMILY_RS300)
	   || (info->ChipFamily == CHIP_FAMILY_RS400)
	   || (info->ChipFamily == CHIP_FAMILY_RS480))
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
    case 7: info->sclk = mpll; break;
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
	    uint32_t tmp;
	    tmp = INPLL(pScrn, RADEON_PPLL_REF_DIV);
	    if (IS_R300_VARIANT ||
		(info->ChipFamily == CHIP_FAMILY_RS300) ||
		(info->ChipFamily == CHIP_FAMILY_RS400) ||
		(info->ChipFamily == CHIP_FAMILY_RS480)) {
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
	   pll->pll_in_min = 100;
	   pll->pll_in_max = 1350;
	   pll->pll_out_min = 20000;
	   pll->pll_out_max = 50000;
       } else {
	   pll->pll_in_min = 40;
	   pll->pll_in_max = 500;
	   pll->pll_out_min = 12500;
	   pll->pll_out_max = 35000;
       }

       if (!RADEONProbePLLParameters(pScrn)) {
	   if (info->IsIGP)
	       pll->reference_freq = 1432;
	   else
	       pll->reference_freq = 2700;

	   pll->reference_div = 12;
	   pll->xclk = 10300;

	   info->sclk = 200.00;
	   info->mclk = 200.00;
       }
    }

    /* card limits for computing PLLs */
    if (IS_AVIVO_VARIANT) {
	pll->min_post_div = 2;
	pll->max_post_div = 0x7f;
    } else {
	pll->min_post_div = 1;
	pll->max_post_div = 12; //16 on crtc0
    }
    pll->min_ref_div = 2;
    pll->max_ref_div = 0x3ff;
    pll->min_feedback_div = 4;
    pll->max_feedback_div = 0x7ff;
    pll->best_vco = 0;

    xf86DrvMsg (pScrn->scrnIndex, X_INFO,
		"PLL parameters: rf=%u rd=%u min=%u max=%u; xclk=%u\n",
		pll->reference_freq,
		pll->reference_div,
		(unsigned)pll->pll_out_min, (unsigned)pll->pll_out_max,
		pll->xclk);

    /* (Some?) Radeon BIOSes seem too lie about their minimum dot
     * clocks.  Allow users to override the detected minimum dot clock
     * value (e.g., and allow it to be suitable for TV sets).
     */
    if (xf86GetOptValFreq(info->Options, OPTION_MIN_DOTCLOCK,
			  OPTUNITS_MHZ, &min_dotclock)) {
	if (min_dotclock < 12 || min_dotclock*100 >= pll->pll_out_max) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Illegal minimum dotclock specified %.2f MHz "
		       "(option ignored)\n",
		       min_dotclock);
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Forced minimum dotclock to %.2f MHz "
		       "(instead of detected %.2f MHz)\n",
		       min_dotclock, ((double)pll->pll_out_min/1000));
	    pll->pll_out_min = min_dotclock * 1000;
	}
    }
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

void RADEONInitMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save,
				      RADEONInfoPtr info)
{
    save->mc_fb_location = info->mc_fb_location;
    save->mc_agp_location = info->mc_agp_location;

    if (IS_AVIVO_VARIANT) {
	save->mc_agp_location_hi = info->mc_agp_location_hi;
    } else {
	save->display_base_addr = info->fbLocation;
	save->display2_base_addr = info->fbLocation;
	save->ov0_base_addr = info->fbLocation;
    }
}

static void RADEONInitMemoryMap(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint64_t       mem_size;
    uint64_t       aper_size;

    radeon_read_mc_fb_agp_location(pScrn, LOC_FB | LOC_AGP, &info->mc_fb_location,
				   &info->mc_agp_location, &info->mc_agp_location_hi);

    /* We shouldn't use info->videoRam here which might have been clipped
     * but the real video RAM instead
     */
    if (info->ChipFamily >= CHIP_FAMILY_R600) {
	mem_size = INREG(R600_CONFIG_MEMSIZE);
	aper_size = INREG(R600_CONFIG_APER_SIZE);
    } else {
	mem_size = INREG(RADEON_CONFIG_MEMSIZE);
	aper_size = INREG(RADEON_CONFIG_APER_SIZE);
    }

    if (mem_size == 0)
	mem_size = 0x800000;

    /* Fix for RN50, M6, M7 with 8/16/32(??) MBs of VRAM - 
       Novell bug 204882 + along with lots of ubuntu ones */
    if (aper_size > mem_size)
	mem_size = aper_size;

#ifdef XF86DRI
    /* Apply memory map limitation if using an old DRI */
    if (info->directRenderingEnabled && !info->dri->newMemoryMap) {
	    if (aper_size < mem_size)
		mem_size = aper_size;
    }
#endif

    if ((info->ChipFamily != CHIP_FAMILY_RS600) &&
	(info->ChipFamily != CHIP_FAMILY_RS690) &&
	(info->ChipFamily != CHIP_FAMILY_RS740) &&
	(info->ChipFamily != CHIP_FAMILY_RS780) &&
	(info->ChipFamily != CHIP_FAMILY_RS880)) {
	if (info->IsIGP)
	    info->mc_fb_location = INREG(RADEON_NB_TOM);
	else
#ifdef XF86DRI
	/* Old DRI has restrictions on the memory map */
	if ( info->directRenderingEnabled &&
	     info->dri->pKernelDRMVersion->version_minor < 10 )
	    info->mc_fb_location = (mem_size - 1) & 0xffff0000U;
	else
#endif
	{
	    uint64_t aper0_base;

	    if (info->ChipFamily >= CHIP_FAMILY_R600) {
		aper0_base = INREG(R600_CONFIG_F0_BASE);
	    } else {
		aper0_base = INREG(RADEON_CONFIG_APER_0_BASE);
	    }

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

	    if (info->ChipFamily >= CHIP_FAMILY_R600) {
		uint64_t mc_fb = ((aper0_base >> 24) & 0xffff) |
		    (((aper0_base + mem_size - 1) >> 8) & 0xffff0000);
		info->mc_fb_location = mc_fb & 0xffffffff;
		ErrorF("mc fb loc is %08x\n", (unsigned int)info->mc_fb_location);
	    } else {
		uint64_t mc_fb = ((aper0_base >> 16) & 0xffff) |
		    ((aper0_base + mem_size - 1) & 0xffff0000U);
		info->mc_fb_location = mc_fb & 0xffffffff;
	    }
	}
    }
    if (info->ChipFamily >= CHIP_FAMILY_R600) {
	info->fbLocation = (info->mc_fb_location & 0xffff) << 24;
    } else {
	info->fbLocation = (info->mc_fb_location & 0xffff) << 16;
    }
    /* Just disable the damn AGP apertures for now, it may be
     * re-enabled later by the DRM
     */
    if (IS_AVIVO_VARIANT)
	info->mc_agp_location = 0x003f0000;
    else
	info->mc_agp_location = 0xffffffc0;
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "RADEONInitMemoryMap() : \n");
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "  mem_size         : 0x%08x\n", (unsigned)mem_size);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "  MC_FB_LOCATION   : 0x%08x\n", (unsigned)info->mc_fb_location);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "  MC_AGP_LOCATION  : 0x%08x\n",
	       (unsigned)info->mc_agp_location);
}

static void RADEONGetVRamType(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t tmp;

    if (info->IsIGP || (info->ChipFamily >= CHIP_FAMILY_R300))
	info->IsDDR = TRUE;
    else if (INREG(RADEON_MEM_SDRAM_MODE_REG) & RADEON_MEM_CFG_TYPE_DDR)
	info->IsDDR = TRUE;
    else
	info->IsDDR = FALSE;

    if ((info->ChipFamily >= CHIP_FAMILY_R600) &&
	(info->ChipFamily <= CHIP_FAMILY_RV635)) {
	int chansize;
	/* r6xx */
	tmp = INREG(R600_RAMCFG);
	if (tmp & R600_CHANSIZE_OVERRIDE)
	    chansize = 16;
	else if (tmp & R600_CHANSIZE)
	    chansize = 64;
	else
	    chansize = 32;
	if (info->ChipFamily == CHIP_FAMILY_R600)
	    info->RamWidth = 8 * chansize;
	else if (info->ChipFamily == CHIP_FAMILY_RV670)
	    info->RamWidth = 4 * chansize;
	else if ((info->ChipFamily == CHIP_FAMILY_RV610) ||
		 (info->ChipFamily == CHIP_FAMILY_RV620))
	    info->RamWidth = chansize;
	else if ((info->ChipFamily == CHIP_FAMILY_RV630) ||
		 (info->ChipFamily == CHIP_FAMILY_RV635))
	    info->RamWidth = 2 * chansize;
    } else if (info->ChipFamily == CHIP_FAMILY_RV515) {
	/* rv515/rv550 */
	tmp = INMC(pScrn, RV515_MC_CNTL);
	tmp &= RV515_MEM_NUM_CHANNELS_MASK;
	switch (tmp) {
	case 0: info->RamWidth = 64; break;
	case 1: info->RamWidth = 128; break;
	default: info->RamWidth = 128; break;
	}
    } else if ((info->ChipFamily >= CHIP_FAMILY_R520) &&
	       (info->ChipFamily <= CHIP_FAMILY_RV570)){
	/* r520/rv530/rv560/rv570/r580 */
	tmp = INMC(pScrn, R520_MC_CNTL0);
	switch ((tmp & R520_MEM_NUM_CHANNELS_MASK) >> R520_MEM_NUM_CHANNELS_SHIFT) {
	case 0: info->RamWidth = 32; break;
	case 1: info->RamWidth = 64; break;
	case 2: info->RamWidth = 128; break;
	case 3: info->RamWidth = 256; break;
	default: info->RamWidth = 64; break;
	}
	if (tmp & R520_MC_CHANNEL_SIZE) {
	    info->RamWidth *= 2;
	}
    } else if ((info->ChipFamily >= CHIP_FAMILY_R300) &&
	       (info->ChipFamily <= CHIP_FAMILY_RV410)) {
	/* r3xx, r4xx */
	tmp = INREG(RADEON_MEM_CNTL);
	tmp &= R300_MEM_NUM_CHANNELS_MASK;
	switch (tmp) {
	case 0: info->RamWidth = 64; break;
	case 1: info->RamWidth = 128; break;
	case 2: info->RamWidth = 256; break;
	default: info->RamWidth = 128; break;
	}
    } else if ((info->ChipFamily == CHIP_FAMILY_RV100) ||
	       (info->ChipFamily == CHIP_FAMILY_RS100) ||
	       (info->ChipFamily == CHIP_FAMILY_RS200)){
	tmp = INREG(RADEON_MEM_CNTL);
	if (tmp & RV100_HALF_MODE)
	    info->RamWidth = 32;
	else
	    info->RamWidth = 64;

	if (!pRADEONEnt->HasCRTC2) {
	    info->RamWidth /= 4;
	    info->IsDDR = TRUE;
	}
    } else if (info->ChipFamily <= CHIP_FAMILY_RV280) {
	tmp = INREG(RADEON_MEM_CNTL);
	if (tmp & RADEON_MEM_NUM_CHANNELS_MASK)
	    info->RamWidth = 128;
	else
	    info->RamWidth = 64;
    } else {
	/* newer IGPs */
	info->RamWidth = 128;
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
static uint32_t RADEONGetAccessibleVRAM(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t	   aper_size;
    unsigned char  byte;

    if (info->ChipFamily >= CHIP_FAMILY_R600)
	aper_size = INREG(R600_CONFIG_APER_SIZE) / 1024;
    else
	aper_size = INREG(RADEON_CONFIG_APER_SIZE) / 1024;

#ifdef XF86DRI
    /* If we use the DRI, we need to check if it's a version that has the
     * bug of always cropping MC_FB_LOCATION to one aperture, in which case
     * we need to limit the amount of accessible video memory
     */
    if (info->directRenderingEnabled &&
	info->dri->pKernelDRMVersion->version_minor < 23) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "[dri] limiting video memory to one aperture of %uK\n",
		   (unsigned)aper_size);
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "[dri] detected radeon kernel module version 1.%d but"
		   " 1.23 or newer is required for full memory mapping.\n",
		   info->dri->pKernelDRMVersion->version_minor);
	info->dri->newMemoryMap = FALSE;
	return aper_size;
    }
    info->dri->newMemoryMap = TRUE;
#endif /* XF86DRI */

    if (info->ChipFamily >= CHIP_FAMILY_R600)
	return aper_size;

    /* Set HDP_APER_CNTL only on cards that are known not to be broken,
     * that is has the 2nd generation multifunction PCI interface
     */
    if (info->ChipFamily == CHIP_FAMILY_RV280 ||
	info->ChipFamily == CHIP_FAMILY_RV350 ||
	info->ChipFamily == CHIP_FAMILY_RV380 ||
	info->ChipFamily == CHIP_FAMILY_R420 ||
	info->ChipFamily == CHIP_FAMILY_RV410 ||
	IS_AVIVO_VARIANT) {
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
    PCI_READ_BYTE(info->PciInfo, &byte, 0xe);
    if (byte & 0x80) {
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
    uint32_t         accessible, bar_size;

    if ((!IS_AVIVO_VARIANT) && info->IsIGP) {
	uint32_t tom = INREG(RADEON_NB_TOM);

	pScrn->videoRam = (((tom >> 16) -
			    (tom & 0xffff) + 1) << 6);

	OUTREG(RADEON_CONFIG_MEMSIZE, pScrn->videoRam * 1024);
    } else {
	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    pScrn->videoRam = INREG(R600_CONFIG_MEMSIZE) / 1024;
	else {
	    /* Read VRAM size from card */
	    pScrn->videoRam      = INREG(RADEON_CONFIG_MEMSIZE) / 1024;

	    /* Some production boards of m6 will return 0 if it's 8 MB */
	    if (pScrn->videoRam == 0) {
		pScrn->videoRam = 8192;
		OUTREG(RADEON_CONFIG_MEMSIZE, 0x800000);
	    }
	}
    }

    /* Get accessible memory */
    accessible = RADEONGetAccessibleVRAM(pScrn);

    /* Crop it to the size of the PCI BAR */
    bar_size = PCI_REGION_SIZE(info->PciInfo, 0) / 1024;
    if (bar_size == 0)
	bar_size = 0x20000;
    if (accessible > bar_size)
	accessible = bar_size;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "Detected total video RAM=%dK, accessible=%uK (PCI BAR=%uK)\n",
	       pScrn->videoRam, (unsigned)accessible, (unsigned)bar_size);
    if (pScrn->videoRam > accessible)
	pScrn->videoRam = accessible;

    if (!IS_AVIVO_VARIANT) {
	info->MemCntl            = INREG(RADEON_SDRAM_MODE_REG);
	info->BusCntl            = INREG(RADEON_BUS_CNTL);
    }

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

    if (info->IsPrimary) {
	pScrn->videoRam /= 2;
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
      /* work out the size of pcie aperture */
        info->FbSecureSize = RADEONDRIGetPciAperTableSize(pScrn);
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
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    EntityInfoPtr  pEnt   = info->pEnt;
    GDevPtr        dev    = pEnt->device;
    unsigned char *RADEONMMIO = info->MMIO;
    MessageType    from = X_PROBED;
    int i;
#ifdef XF86DRI
    const char *s;
    uint32_t cmd_stat;
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
	info->Chipset = PCI_DEV_DEVICE_ID(info->PciInfo);
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

    pRADEONEnt->HasCRTC2 = TRUE;
    info->IsMobility = FALSE;
    info->IsIGP = FALSE;
    info->IsDellServer = FALSE;
    info->HasSingleDAC = FALSE;
    info->InternalTVOut = TRUE;
    info->get_hardcoded_edid_from_bios = FALSE;

    for (i = 0; i < sizeof(RADEONCards) / sizeof(RADEONCardInfo); i++) {
	if (info->Chipset == RADEONCards[i].pci_device_id) {
	    RADEONCardInfo *card = &RADEONCards[i];
	    info->ChipFamily = card->chip_family;
	    info->IsMobility = card->mobility;
	    info->IsIGP = card->igp;
	    pRADEONEnt->HasCRTC2 = !card->nocrtc2;
	    info->HasSingleDAC = card->singledac;
	    info->InternalTVOut = !card->nointtvout;
	    break;
	}
    }

    switch (info->Chipset) {
    case PCI_CHIP_RN50_515E:  /* RN50 is based on the RV100 but 3D isn't guaranteed to work.  YMMV. */
    case PCI_CHIP_RN50_5969:
	/* Some Sun servers have a hardcoded edid so KVMs work properly */
	if ((PCI_SUB_VENDOR_ID(info->PciInfo) == 0x108e) &&
	    (PCI_SUB_DEVICE_ID(info->PciInfo) == 0x4133))
	    info->get_hardcoded_edid_from_bios = TRUE;
    case PCI_CHIP_RV100_QY:
    case PCI_CHIP_RV100_QZ:
	/* DELL triple-head configuration. */
	if ((PCI_SUB_VENDOR_ID(info->PciInfo) == PCI_VENDOR_DELL) &&
	    ((PCI_SUB_DEVICE_ID(info->PciInfo) == 0x016c) ||
	     (PCI_SUB_DEVICE_ID(info->PciInfo) == 0x016d) ||
	     (PCI_SUB_DEVICE_ID(info->PciInfo) == 0x016e) ||
	     (PCI_SUB_DEVICE_ID(info->PciInfo) == 0x016f) ||
	     (PCI_SUB_DEVICE_ID(info->PciInfo) == 0x0170) ||
	     (PCI_SUB_DEVICE_ID(info->PciInfo) == 0x017d) ||
	     (PCI_SUB_DEVICE_ID(info->PciInfo) == 0x017e) ||
	     (PCI_SUB_DEVICE_ID(info->PciInfo) == 0x0183) ||
	     (PCI_SUB_DEVICE_ID(info->PciInfo) == 0x018a) ||
	     (PCI_SUB_DEVICE_ID(info->PciInfo) == 0x019a))) {
	    info->IsDellServer = TRUE;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "DELL server detected, force to special setup\n");
	}
	break;
    case PCI_CHIP_RS482_5974:
	/* RH BZ 444586 - non mobility version
 	 * Dell appear to have the Vostro 1100 with a mobility part with the same pci-id */
	if ((PCI_SUB_VENDOR_ID(info->PciInfo) == 0x1462) &&
            (PCI_SUB_DEVICE_ID(info->PciInfo) == 0x7141)) {
		info->IsMobility = FALSE;
	}
    default:
	break;
    }

    if (info->ChipFamily >= CHIP_FAMILY_R600) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "R600 support is mostly incomplete and very experimental\n");
    }

    if ((info->ChipFamily >= CHIP_FAMILY_RV515) && (info->ChipFamily < CHIP_FAMILY_R600)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "R500 support is under development. Please report any issues to xorg-driver-ati@lists.x.org\n");
    }

    from               = X_PROBED;
    info->LinearAddr   = PCI_REGION_BASE(info->PciInfo, 0, REGION_MEM) & ~0x1ffffffULL;
    pScrn->memPhysBase = info->LinearAddr;
    if (dev->MemBase) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Linear address override, using 0x%016lx instead of 0x%016llx\n",
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
	       "Linear framebuffer at 0x%016llx\n", info->LinearAddr);

#ifndef XSERVER_LIBPCIACCESS
				/* BIOS */
    from              = X_PROBED;
    info->BIOSAddr    = info->PciInfo->biosBase & 0xfffe0000;
    if (dev->BiosBase) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "BIOS address override, using 0x%08lx instead of 0x%08lx\n",
		   (unsigned long)dev->BiosBase,
		   (unsigned long)info->BIOSAddr);
	info->BIOSAddr = dev->BiosBase;
	from           = X_CONFIG;
    }
    if (info->BIOSAddr) {
	xf86DrvMsg(pScrn->scrnIndex, from,
		   "BIOS at 0x%08lx\n", (unsigned long)info->BIOSAddr);
    }
#endif

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

    PCI_READ_LONG(info->PciInfo, &cmd_stat, PCI_CMD_STAT_REG);
    if (cmd_stat & RADEON_CAP_LIST) {
	uint32_t cap_ptr, cap_id;

	PCI_READ_LONG(info->PciInfo, &cap_ptr, RADEON_CAPABILITIES_PTR_PCI_CONFIG);
	cap_ptr &= RADEON_CAP_PTR_MASK;

	while(cap_ptr != RADEON_CAP_ID_NULL) {
	    PCI_READ_LONG(info->PciInfo, &cap_id, cap_ptr);
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

    /* treat PCIE IGP cards as PCI */
    if (info->cardType == CARD_PCIE && info->IsIGP)
	info->cardType = CARD_PCI;

    if ((info->ChipFamily >= CHIP_FAMILY_R600) && info->IsIGP)
	info->cardType = CARD_PCIE;

    /* not sure about gart table requirements */
    if ((info->ChipFamily == CHIP_FAMILY_RS600) && info->IsIGP)
	info->cardType = CARD_PCIE;

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

    info->r4xx_atom = FALSE;
    if (((info->ChipFamily == CHIP_FAMILY_R420) || (info->ChipFamily == CHIP_FAMILY_RV410)) &&
	xf86ReturnOptValBool(info->Options, OPTION_R4XX_ATOM, FALSE)) {
	info->r4xx_atom = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Using ATOMBIOS for R4xx chip\n");
    }

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
	info->ddc2 = TRUE;
    }

    /* DDC can use I2C bus */
    /* Load I2C if we have the code to use it */
    if (info->ddc2) {
	xf86LoadSubModule(pScrn, "i2c");
    }
}

/* This is called by RADEONPreInit to initialize gamma correction */
static Bool RADEONPreInitGamma(ScrnInfoPtr pScrn)
{
    Gamma  zeros = { 0.0, 0.0, 0.0 };

    if (!xf86SetGamma(pScrn, zeros)) return FALSE;
    return TRUE;
}

/* This is called by RADEONPreInit to initialize the hardware cursor */
static Bool RADEONPreInitCursor(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);

    if (!xf86ReturnOptValBool(info->Options, OPTION_SW_CURSOR, FALSE)) {
	if (!xf86LoadSubModule(pScrn, "ramdac")) return FALSE;
    }
    return TRUE;
}

/* This is called by RADEONPreInit to initialize hardware acceleration */
static Bool RADEONPreInitAccel(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    MessageType from;
#if defined(USE_EXA) && defined(USE_XAA)
    char *optstr;
#endif

    if (!(info->accel_state = xcalloc(1, sizeof(struct radeon_accel_state)))) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to allocate accel_state rec!\n");
	return FALSE;
    }
    info->accel_state->fifo_slots                 = 0;

    if ((info->ChipFamily == CHIP_FAMILY_RS100) ||
	(info->ChipFamily == CHIP_FAMILY_RS200) ||
	(info->ChipFamily == CHIP_FAMILY_RS300) ||
	(info->ChipFamily == CHIP_FAMILY_RS400) ||
	(info->ChipFamily == CHIP_FAMILY_RS480) ||
	(info->ChipFamily == CHIP_FAMILY_RS600) ||
	(info->ChipFamily == CHIP_FAMILY_RS690) ||
	(info->ChipFamily == CHIP_FAMILY_RS740))
	info->accel_state->has_tcl = FALSE;
    else {
	info->accel_state->has_tcl = TRUE;
    }

    info->useEXA = FALSE;

    if (info->ChipFamily >= CHIP_FAMILY_R600) {
	xf86DrvMsg(pScrn->scrnIndex, X_DEFAULT,
	    "Will attempt to use R6xx/R7xx EXA support if DRI is enabled.\n");
	info->useEXA = TRUE;
    }

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
        if (info->ChipFamily < CHIP_FAMILY_R600)
	    xf86DrvMsg(pScrn->scrnIndex, from,
		       "Using %s acceleration architecture\n",
		       info->useEXA ? "EXA" : "XAA");

#ifdef USE_EXA
	if (info->useEXA) {
	    info->exaReq.majorversion = EXA_VERSION_MAJOR;
	    info->exaReq.minorversion = EXA_VERSION_MINOR;

	    if (!LoadSubModule(pScrn->module, "exa", NULL, NULL, NULL,
			       &info->exaReq, &errmaj, &errmin)) {
		LoaderErrorMsg(NULL, "exa", errmaj, errmin);
		return FALSE;
	    }
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
	}
#endif /* USE_XAA */
    }

    return TRUE;
}

static Bool RADEONPreInitInt10(ScrnInfoPtr pScrn, xf86Int10InfoPtr *ppInt10)
{
#if (!defined(__powerpc__) && !defined(__sparc__)) || \
    (defined(XSERVER_LIBPCIACCESS) && HAVE_PCI_DEVICE_ENABLE)
    RADEONInfoPtr  info = RADEONPTR(pScrn);
#endif
#if !defined(__powerpc__) && !defined(__sparc__)
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t       fp2_gen_ctl_save   = 0;
#endif

#ifdef XSERVER_LIBPCIACCESS
#if HAVE_PCI_DEVICE_ENABLE
    pci_device_enable(info->PciInfo);
#endif
#endif

#if !defined(__powerpc__) && !defined(__sparc__)
    /* don't need int10 on atom cards.
     * in theory all radeons, but the older stuff
     * isn't 100% yet
     * secondary atom cards tend to hang when initializing int10,
     * however, on some stom cards, you can't read the bios without
     * intitializing int10.
     */
    if (!xf86ReturnOptValBool(info->Options, OPTION_INT10, TRUE))
	return TRUE;

    if (xf86LoadSubModule(pScrn, "int10")) {
	/* The VGA BIOS on the RV100/QY cannot be read when the digital output
	 * is enabled.  Clear and restore FP2_ON around int10 to avoid this.
	 */
	if (PCI_DEV_DEVICE_ID(info->PciInfo) == PCI_CHIP_RV100_QY) {
	    fp2_gen_ctl_save = INREG(RADEON_FP2_GEN_CNTL);
	    if (fp2_gen_ctl_save & RADEON_FP2_ON) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "disabling digital out\n");
		OUTREG(RADEON_FP2_GEN_CNTL, fp2_gen_ctl_save & ~RADEON_FP2_ON);
	    }
	}

	xf86DrvMsg(pScrn->scrnIndex,X_INFO,"initializing int10\n");
	*ppInt10 = xf86InitInt10(info->pEnt->index);

	if (PCI_DEV_DEVICE_ID(info->PciInfo) == PCI_CHIP_RV100_QY) {
	    if (fp2_gen_ctl_save & RADEON_FP2_ON) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "re-enabling digital out\n");
		OUTREG(RADEON_FP2_GEN_CNTL, fp2_gen_ctl_save);
	    }
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
    char          *reason;

    info->directRenderingEnabled = FALSE;
    info->directRenderingInited = FALSE;

    if (!(info->dri = xcalloc(1, sizeof(struct radeon_dri)))) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,"Unable to allocate dri rec!\n");
	return FALSE;
    }

    if (!(info->cp = xcalloc(1, sizeof(struct radeon_cp)))) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,"Unable to allocate cp rec!\n");
	return FALSE;
    }
    info->cp->CPInUse = FALSE;
    info->cp->CPStarted = FALSE;
    info->cp->CPusecTimeout = RADEON_DEFAULT_CP_TIMEOUT;

   if (xf86IsEntityShared(info->pEnt->index)) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "Direct Rendering Disabled -- "
                   "Zaphod Dual-head configuration is not working with "
                   "DRI at present.\n"
                   "Please use the xrandr 1.2 if you "
                   "want Dual-head with DRI.\n");
        return FALSE;
    }
    if (info->IsSecondary)
        return FALSE;

    if (info->Chipset == PCI_CHIP_RN50_515E ||
	info->Chipset == PCI_CHIP_RN50_5969) {
	if (xf86ReturnOptValBool(info->Options, OPTION_DRI, FALSE)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"Direct rendering for RN50 forced on -- "
		"This is NOT officially supported at the hardware level "
		"and may cause instability or lockups\n");
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Direct rendering not officially supported on RN50\n");
	    return FALSE;
	}
    }

    if (info->ChipFamily == CHIP_FAMILY_RS880)
	return FALSE;

    if (!xf86ReturnOptValBool(info->Options, OPTION_DRI, TRUE)) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Direct rendering forced off\n");
	return FALSE;
    }

    if (xf86ReturnOptValBool(info->Options, OPTION_NOACCEL, FALSE)) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "[dri] Acceleration disabled, not initializing the DRI\n");
	return FALSE;
    }

    info->dri->pLibDRMVersion = NULL;
    info->dri->pKernelDRMVersion = NULL;

    if (!RADEONDRIGetVersion(pScrn))
	return FALSE;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "[dri] Found DRI library version %d.%d.%d and kernel"
	       " module version %d.%d.%d\n",
	       info->dri->pLibDRMVersion->version_major,
	       info->dri->pLibDRMVersion->version_minor,
	       info->dri->pLibDRMVersion->version_patchlevel,
	       info->dri->pKernelDRMVersion->version_major,
	       info->dri->pKernelDRMVersion->version_minor,
	       info->dri->pKernelDRMVersion->version_patchlevel);

    if (info->Chipset == PCI_CHIP_RS400_5A41 ||
	info->Chipset == PCI_CHIP_RS400_5A42 ||
	info->Chipset == PCI_CHIP_RC410_5A61 ||
	info->Chipset == PCI_CHIP_RC410_5A62 ||
	info->Chipset == PCI_CHIP_RS480_5954 ||
	info->Chipset == PCI_CHIP_RS480_5955 ||
	info->Chipset == PCI_CHIP_RS482_5974 ||
	info->Chipset == PCI_CHIP_RS485_5975) {

	if (info->dri->pKernelDRMVersion->version_minor < 27) {
 	     xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			"Direct rendering broken on XPRESS 200 and 200M with DRI less than 1.27\n");
	     return FALSE;
	}
 	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	"Direct rendering experimental on RS400/Xpress 200 enabled\n");
    }

    if (info->ChipFamily >= CHIP_FAMILY_R300)
	info->dri->gartSize      = R300_DEFAULT_GART_SIZE;
    else
	info->dri->gartSize      = RADEON_DEFAULT_GART_SIZE;

    info->dri->ringSize      = RADEON_DEFAULT_RING_SIZE;
    info->dri->bufSize       = RADEON_DEFAULT_BUFFER_SIZE;
    info->dri->gartTexSize   = RADEON_DEFAULT_GART_TEX_SIZE;
    info->dri->pciAperSize   = RADEON_DEFAULT_PCI_APER_SIZE;
    info->cp->CPusecTimeout = RADEON_DEFAULT_CP_TIMEOUT;

    if ((xf86GetOptValInteger(info->Options,
			     OPTION_GART_SIZE, (int *)&(info->dri->gartSize))) ||
			     (xf86GetOptValInteger(info->Options,
			     OPTION_GART_SIZE_OLD, (int *)&(info->dri->gartSize)))) {
	switch (info->dri->gartSize) {
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
		       "Illegal GART size: %d MB\n", info->dri->gartSize);
	    return FALSE;
	}
    }

    if (xf86GetOptValInteger(info->Options,
			     OPTION_RING_SIZE, &(info->dri->ringSize))) {
	if (info->dri->ringSize < 1 || info->dri->ringSize >= (int)info->dri->gartSize) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Illegal ring buffer size: %d MB\n",
		       info->dri->ringSize);
	    return FALSE;
	}
    }

    if (xf86GetOptValInteger(info->Options,
			     OPTION_PCIAPER_SIZE, &(info->dri->pciAperSize))) {
      switch(info->dri->pciAperSize) {
      case 32:
      case 64:
      case 128:
      case 256:
	break;
      default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Illegal pci aper size: %d MB\n",
		       info->dri->pciAperSize);
	return FALSE;
      }
    }


    if (xf86GetOptValInteger(info->Options,
			     OPTION_BUFFER_SIZE, &(info->dri->bufSize))) {
	if (info->dri->bufSize < 1 || info->dri->bufSize >= (int)info->dri->gartSize) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Illegal vertex/indirect buffers size: %d MB\n",
		       info->dri->bufSize);
	    return FALSE;
	}
	if (info->dri->bufSize > 2) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Illegal vertex/indirect buffers size: %d MB\n",
		       info->dri->bufSize);
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Clamping vertex/indirect buffers size to 2 MB\n");
	    info->dri->bufSize = 2;
	}
    }

    if (info->dri->ringSize + info->dri->bufSize + info->dri->gartTexSize >
	(int)info->dri->gartSize) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Buffers are too big for requested GART space\n");
	return FALSE;
    }

    info->dri->gartTexSize = info->dri->gartSize - (info->dri->ringSize + info->dri->bufSize);

    if (xf86GetOptValInteger(info->Options, OPTION_USEC_TIMEOUT,
			     &(info->cp->CPusecTimeout))) {
	/* This option checked by the RADEON DRM kernel module */
    }

    /* Two options to try and squeeze as much texture memory as possible
     * for dedicated 3d rendering boxes
     */
    info->dri->noBackBuffer = xf86ReturnOptValBool(info->Options,
						   OPTION_NO_BACKBUFFER,
						   FALSE);

    info->dri->allowPageFlip = 0;

#ifdef DAMAGE
    if (info->dri->noBackBuffer) {
	from = X_DEFAULT;
	reason = " because back buffer disabled";
    } else {
	from = xf86GetOptValBool(info->Options, OPTION_PAGE_FLIP,
				 &info->dri->allowPageFlip) ? X_CONFIG : X_DEFAULT;

	if (IS_AVIVO_VARIANT) {
	    info->dri->allowPageFlip = 0;
	    reason = " on r5xx and newer chips.\n";
	} else {
	    reason = "";
	}

    }
#else
    from = X_DEFAULT;
    reason = " because Damage layer not available at build time";
#endif

    xf86DrvMsg(pScrn->scrnIndex, from, "Page Flipping %sabled%s\n",
	       info->dri->allowPageFlip ? "en" : "dis", reason);

    /* AGP seems to have problems with gart transfers */
    if ((info->ChipFamily >= CHIP_FAMILY_R600) && (info->cardType == CARD_AGP))
	info->DMAForXv = FALSE;
    else
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
    if (IS_R300_VARIANT || IS_AVIVO_VARIANT) {
	/* this may be 4096 on r4xx -- need to double check */
	info->MaxSurfaceWidth = 3968; /* one would have thought 4096...*/
	info->MaxLines = 4096;
    } else {
	info->MaxSurfaceWidth = 2048;
	info->MaxLines = 2048;
    }

    if (!info->allowColorTiling)
	return;

    if (info->ChipFamily >= CHIP_FAMILY_R600)
	info->allowColorTiling = FALSE;

    /* for zaphod disable tiling for now */
    if (info->IsPrimary || info->IsSecondary)
	info->allowColorTiling = FALSE;

#ifdef XF86DRI
    if (info->directRenderingEnabled &&
	info->dri->pKernelDRMVersion->version_minor < 14) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "[dri] color tiling disabled because of version "
		   "mismatch.\n"
		   "[dri] radeon.o kernel module version is %d.%d.%d but "
		   "1.14.0 or later is required for color tiling.\n",
		   info->dri->pKernelDRMVersion->version_major,
		   info->dri->pKernelDRMVersion->version_minor,
		   info->dri->pKernelDRMVersion->version_patchlevel);
	   info->allowColorTiling = FALSE;
	   return;
    }
#endif /* XF86DRI */

    if (info->allowColorTiling) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Color tiling enabled by default\n");
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Color tiling disabled\n");
    }
}


static Bool RADEONPreInitXv(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    uint16_t mm_table;
    uint16_t bios_header;
    uint16_t pll_info_block;
#ifdef XvExtension
    char* microc_path = NULL;
    char* microc_type = NULL;
    MessageType from;

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

    if(xf86GetOptValInteger(info->Options, OPTION_SCALER_WIDTH, &(info->overlay_scaler_buffer_width))) {
	if ((info->overlay_scaler_buffer_width < 1024) ||
	  (info->overlay_scaler_buffer_width > 2048) ||
	  ((info->overlay_scaler_buffer_width % 64) != 0)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Attempt to set illegal scaler width. Using default\n");
	    from = X_DEFAULT;
	    info->overlay_scaler_buffer_width = 0;
	} else
	    from = X_CONFIG;
    } else {
	from = X_DEFAULT;
	info->overlay_scaler_buffer_width = 0;
    }
    if (!info->overlay_scaler_buffer_width) {
       /* overlay scaler line length differs for different revisions
       this needs to be maintained by hand  */
	switch(info->ChipFamily){
	case CHIP_FAMILY_R200:
	case CHIP_FAMILY_R300:
	case CHIP_FAMILY_RV350:
		info->overlay_scaler_buffer_width = 1920;
		break;
	default:
		info->overlay_scaler_buffer_width = 1536;
	}
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Assuming overlay scaler buffer width is %d\n",
	info->overlay_scaler_buffer_width);
#endif

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

static Bool
RADEONPreInitBIOS(ScrnInfoPtr pScrn, xf86Int10InfoPtr  pInt10)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    if (!RADEONGetBIOSInfo(pScrn, pInt10)) {
	/* Avivo chips require bios for atom */
	if (IS_AVIVO_VARIANT)
	    return FALSE;
    }
    return TRUE;
}

static void RADEONFixZaphodOutputs(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    xf86CrtcConfigPtr   config = XF86_CRTC_CONFIG_PTR(pScrn);

    if (info->IsPrimary) {
	xf86OutputDestroy(config->output[0]);
	while(config->num_output > 1) {
	    xf86OutputDestroy(config->output[1]);
	}
    } else {
	while(config->num_output > 1) {
	    xf86OutputDestroy(config->output[1]);
	}
    }
}

static Bool RADEONPreInitControllers(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr   config = XF86_CRTC_CONFIG_PTR(pScrn);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int i;
    int mask;
    int found = 0;

    if (!info->IsPrimary && !info->IsSecondary)
	mask = 3;
    else if (info->IsPrimary)
	mask = 1;
    else
	mask = 2;
	
    if (!RADEONAllocateControllers(pScrn, mask))
	return FALSE;

    RADEONGetClockInfo(pScrn);

    if (!RADEONSetupConnectors(pScrn)) {
	return FALSE;
    }

    if (info->IsPrimary || info->IsSecondary) {
	/* fixup outputs for zaphod */
	RADEONFixZaphodOutputs(pScrn);
    }
      
    RADEONPrintPortMap(pScrn);

    info->first_load_no_devices = FALSE;
    for (i = 0; i < config->num_output; i++) {
	xf86OutputPtr	      output = config->output[i];
      
	output->status = (*output->funcs->detect) (output);
	ErrorF("finished output detect: %d\n", i);
	if (info->IsPrimary || info->IsSecondary) {
	    if (output->status != XF86OutputStatusConnected)
		return FALSE;
	}
	if (output->status != XF86OutputStatusDisconnected)
	    found++;
    }

    if (!found) {
	/* nothing connected, light up some defaults so the server comes up */
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No connected devices found!\n");
	info->first_load_no_devices = TRUE;
    }

    ErrorF("finished all detect\n");
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

static Bool
RADEONCRTCResize(ScrnInfoPtr scrn, int width, int height)
{
    scrn->virtualX = width;
    scrn->virtualY = height;
    /* RADEONSetPitch(scrn); */
    return TRUE;
}

static const xf86CrtcConfigFuncsRec RADEONCRTCResizeFuncs = {
    RADEONCRTCResize
};

Bool RADEONPreInit(ScrnInfoPtr pScrn, int flags)
{
    xf86CrtcConfigPtr   xf86_config;
    RADEONInfoPtr     info;
    xf86Int10InfoPtr  pInt10 = NULL;
    void *int10_save = NULL;
    const char *s;
    int crtc_max_X, crtc_max_Y;
    RADEONEntPtr pRADEONEnt;
    DevUnion* pPriv;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONPreInit\n");
    if (pScrn->numEntities != 1) return FALSE;

    if (!RADEONGetRec(pScrn)) return FALSE;

    info               = RADEONPTR(pScrn);
    info->MMIO         = NULL;

    info->IsSecondary  = FALSE;
    info->IsPrimary = FALSE;

    info->pEnt         = xf86GetEntityInfo(pScrn->entityList[pScrn->numEntities - 1]);
    if (info->pEnt->location.type != BUS_PCI) goto fail;

    pPriv = xf86GetEntityPrivate(pScrn->entityList[0], 
				 getRADEONEntityIndex());
    pRADEONEnt = pPriv->ptr;

    if(xf86IsEntityShared(pScrn->entityList[0]))
    {
        if(xf86IsPrimInitDone(pScrn->entityList[0]))
        {
            info->IsSecondary = TRUE;
            pRADEONEnt->pSecondaryScrn = pScrn;
	    info->SavedReg = &pRADEONEnt->SavedReg;
	    info->ModeReg = &pRADEONEnt->ModeReg;
        }
        else
        {
	    info->IsPrimary = TRUE;
            xf86SetPrimInitDone(pScrn->entityList[0]);
            pRADEONEnt->pPrimaryScrn = pScrn;
            pRADEONEnt->HasSecondary = FALSE;
	    info->SavedReg = &pRADEONEnt->SavedReg;
	    info->ModeReg = &pRADEONEnt->ModeReg;
        }
    } else {
	info->SavedReg = &pRADEONEnt->SavedReg;
	info->ModeReg = &pRADEONEnt->ModeReg;
    }

    info->PciInfo = xf86GetPciInfoForEntity(info->pEnt->index);
    info->PciTag  = pciTag(PCI_DEV_BUS(info->PciInfo),
			   PCI_DEV_DEV(info->PciInfo),
			   PCI_DEV_FUNC(info->PciInfo));
    info->MMIOAddr = PCI_REGION_BASE(info->PciInfo, 2, REGION_MEM) & ~0xffULL;
    info->MMIOSize = PCI_REGION_SIZE(info->PciInfo, 2);
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "TOTO SAYS %016llx\n", 
		(unsigned long long)PCI_REGION_BASE(info->PciInfo,
		2, REGION_MEM));
    if (info->pEnt->device->IOBase) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "MMIO address override, using 0x%08lx instead of 0x%016llx\n",
		   info->pEnt->device->IOBase,
		   info->MMIOAddr);
	info->MMIOAddr = info->pEnt->device->IOBase;
    } else if (!info->MMIOAddr) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid MMIO address\n");
	goto fail1;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "MMIO registers at 0x%016llx: size %ldKB\n", info->MMIOAddr, info->MMIOSize / 1024);

    if(!RADEONMapMMIO(pScrn)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Memory map the MMIO region failed\n");
	goto fail1;
    }

#if !defined(__alpha__)
    if (
#ifndef XSERVER_LIBPCIACCESS
	xf86GetPciDomain(info->PciTag) ||
#endif
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

    if (flags & PROBE_DETECT) {
	RADEONProbeDDC(pScrn, info->pEnt->index);
	RADEONPostInt10Check(pScrn, int10_save);
	if(info->MMIO) RADEONUnmapMMIO(pScrn);
	return TRUE;
    }


    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "PCI bus %d card %d func %d\n",
	       PCI_DEV_BUS(info->PciInfo),
	       PCI_DEV_DEV(info->PciInfo),
	       PCI_DEV_FUNC(info->PciInfo));

    if (xf86RegisterResources(info->pEnt->index, 0, ResExclusive))
	goto fail;

    xf86SetOperatingState(resVga, info->pEnt->index, ResUnusedOpr);

    pScrn->racMemFlags = RAC_FB | RAC_COLORMAP | RAC_VIEWPORT | RAC_CURSOR;
    pScrn->monitor     = pScrn->confScreen->monitor;

   /* Allocate an xf86CrtcConfig */
    xf86CrtcConfigInit (pScrn, &RADEONCRTCResizeFuncs);
    xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);


    if (!RADEONPreInitVisual(pScrn))
	goto fail;

				/* We can't do this until we have a
				   pScrn->display. */
    xf86CollectOptions(pScrn, NULL);
    if (!(info->Options = xalloc(sizeof(RADEONOptions))))
	goto fail;

    memcpy(info->Options, RADEONOptions, sizeof(RADEONOptions));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, info->Options);

    /* By default, don't do VGA IOs on ppc/sparc */
#if defined(__powerpc__) || defined(__sparc__) || !defined(WITH_VGAHW)
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

    if (!RADEONPreInitChipType(pScrn))
	goto fail;

    if (!RADEONPreInitInt10(pScrn, &pInt10))
	goto fail;

    RADEONPostInt10Check(pScrn, int10_save);

    if (!RADEONPreInitBIOS(pScrn, pInt10))
	goto fail;

    /* Save BIOS scratch registers */
    RADEONSaveBIOSRegisters(pScrn, info->SavedReg);

#ifdef XF86DRI
    /* PreInit DRI first of all since we need that for getting a proper
     * memory map
     */
    info->directRenderingEnabled = RADEONPreInitDRI(pScrn);
#endif
    if (!info->directRenderingEnabled) {
	if (info->ChipFamily >= CHIP_FAMILY_R600) {
	    info->r600_shadow_fb = TRUE;
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "using shadow framebuffer\n");
	    if (!xf86LoadSubModule(pScrn, "shadow"))
		info->r600_shadow_fb = FALSE;
	}
    }

    if (!RADEONPreInitVRAM(pScrn))
	goto fail;

    RADEONPreInitColorTiling(pScrn);

    /* we really need an FB manager... */
    if (pScrn->display->virtualX) {
	crtc_max_X = pScrn->display->virtualX;
	crtc_max_Y = pScrn->display->virtualY;
	if (info->allowColorTiling) {
	    if (crtc_max_X > info->MaxSurfaceWidth ||
		crtc_max_Y > info->MaxLines) {
		info->allowColorTiling = FALSE;
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "Requested desktop size exceeds surface limts for tiling, ColorTiling disabled\n");
	    }
	}
	if (crtc_max_X > 8192)
	    crtc_max_X = 8192;
	if (crtc_max_Y > 8192)
	    crtc_max_Y = 8192;
    } else {
	/*
	 * note that these aren't really the CRTC limits, they're just
	 * heuristics until we have a better memory manager.
	 */
	if (pScrn->videoRam <= 16384) {
	    crtc_max_X = 1600;
	    crtc_max_Y = 1200;
	} else if (IS_R300_VARIANT) {
	    crtc_max_X = 2560;
	    crtc_max_Y = 1200;
	} else if (IS_AVIVO_VARIANT) {
	    crtc_max_X = 2560;
	    crtc_max_Y = 1600;
	} else {
	    crtc_max_X = 2048;
	    crtc_max_Y = 1200;
	}
    }
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Max desktop size set to %dx%d\n",
	       crtc_max_X, crtc_max_Y);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "For a larger or smaller max desktop size, add a Virtual line to your xorg.conf\n");
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "If you are having trouble with 3D, "
	       "reduce the desktop size by adjusting the Virtual line to your xorg.conf\n");

    /*xf86CrtcSetSizeRange (pScrn, 320, 200, info->MaxSurfaceWidth, info->MaxLines);*/
    xf86CrtcSetSizeRange (pScrn, 320, 200, crtc_max_X, crtc_max_Y);

    RADEONPreInitDDC(pScrn);

    if (!RADEONPreInitControllers(pScrn))
       goto fail;


    ErrorF("before xf86InitialConfiguration\n");

    if (!xf86InitialConfiguration (pScrn, FALSE))
   {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes.\n");
      goto fail;
   }

    /* fix up cloning on rn50 cards
     * since they only have one crtc sometimes the xserver doesn't assign
     * a crtc to one of the outputs even though both outputs have common modes
     * which results in only one monitor being enabled.  Assign a crtc here so
     * that both outputs light up.
     */
    if (info->ChipFamily == CHIP_FAMILY_RV100 && !pRADEONEnt->HasCRTC2) {
	int i;

	for (i = 0; i < xf86_config->num_output; i++) {
	    xf86OutputPtr output = xf86_config->output[i];

	    /* XXX: double check crtc mode */
	    if ((output->probed_modes != NULL) && (output->crtc == NULL))
		output->crtc = xf86_config->crtc[0];
	}
    }

    ErrorF("after xf86InitialConfiguration\n");

    RADEONSetPitch(pScrn);

   /* Set display resolution */
   xf86SetDpi(pScrn, 0, 0);

	/* Get ScreenInit function */
    if (!xf86LoadSubModule(pScrn, "fb")) return FALSE;

    if (!RADEONPreInitGamma(pScrn))              goto fail;

    if (!RADEONPreInitCursor(pScrn))             goto fail;

    if (!RADEONPreInitAccel(pScrn))              goto fail;

    if (!IS_AVIVO_VARIANT) {
	if (!RADEONPreInitXv(pScrn))                 goto fail;
    }

    if (!xf86RandR12PreInit (pScrn))
    {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "RandR initialization failure\n");
      goto fail;
    }

    if (pScrn->modes == NULL) {
      xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No modes.\n");
      goto fail;
   }


				/* Free int10 info */
    if (pInt10)
	xf86FreeInt10(pInt10);

    if(info->MMIO) RADEONUnmapMMIO(pScrn);
    info->MMIO = NULL;

    xf86DrvMsg(pScrn->scrnIndex, X_NOTICE,
	       "For information on using the multimedia capabilities\n\tof this"
	       " adapter, please see http://gatos.sf.net.\n");

    xf86DrvMsg(pScrn->scrnIndex, X_NOTICE,
	       "MergedFB support has been removed and replaced with"
	       " xrandr 1.2 support\n");

    return TRUE;

fail:
				/* Pre-init failed. */
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
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int            i;
    int            index, j;
    uint16_t       lut_r[256], lut_g[256], lut_b[256];
    int c;

#ifdef XF86DRI
    if (info->cp->CPStarted && pScrn->pScreen) DRILock(pScrn->pScreen, 0);
#endif

    if (info->accelOn && pScrn->pScreen)
        RADEON_SYNC(info, pScrn);

    {

      for (c = 0; c < xf86_config->num_crtc; c++) {
	  xf86CrtcPtr crtc = xf86_config->crtc[c];
	  RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;

	  for (i = 0 ; i < 256; i++) {
	      lut_r[i] = radeon_crtc->lut_r[i] << 6;
	      lut_g[i] = radeon_crtc->lut_g[i] << 6;
	      lut_b[i] = radeon_crtc->lut_b[i] << 6;
	  }

	  switch (info->CurrentLayout.depth) {
	  case 15:
	      for (i = 0; i < numColors; i++) {
		  index = indices[i];
		  for (j = 0; j < 8; j++) {
		      lut_r[index * 8 + j] = colors[index].red << 6;
		      lut_g[index * 8 + j] = colors[index].green << 6;
		      lut_b[index * 8 + j] = colors[index].blue << 6;
		  }
	      }
	  case 16:
	      for (i = 0; i < numColors; i++) {
		  index = indices[i];

		  if (i <= 31) {
		      for (j = 0; j < 8; j++) {
			  lut_r[index * 8 + j] = colors[index].red << 6;
			  lut_b[index * 8 + j] = colors[index].blue << 6;
		      }
		  }

		  for (j = 0; j < 4; j++) {
		      lut_g[index * 4 + j] = colors[index].green << 6;
		  }
	      }
	  default:
	      for (i = 0; i < numColors; i++) {
		  index = indices[i];
		  lut_r[index] = colors[index].red << 6;
		  lut_g[index] = colors[index].green << 6;
		  lut_b[index] = colors[index].blue << 6;
	      }
	      break;
	  }

	      /* Make the change through RandR */
#ifdef RANDR_12_INTERFACE
	  if (crtc->randr_crtc)
	      RRCrtcGammaSet(crtc->randr_crtc, lut_r, lut_g, lut_b);
	  else
#endif
	      crtc->funcs->gamma_set(crtc, lut_r, lut_g, lut_b, 256);
      }
    }

#ifdef XF86DRI
    if (info->cp->CPStarted && pScrn->pScreen) DRIUnlock(pScrn->pScreen);
#endif
}

static void RADEONBlockHandler(int i, pointer blockData,
			       pointer pTimeout, pointer pReadmask)
{
    ScreenPtr      pScreen = screenInfo.screens[i];
    ScrnInfoPtr    pScrn   = xf86Screens[i];
    RADEONInfoPtr  info    = RADEONPTR(pScrn);

    pScreen->BlockHandler = info->BlockHandler;
    (*pScreen->BlockHandler) (i, blockData, pTimeout, pReadmask);
    pScreen->BlockHandler = RADEONBlockHandler;

    if (info->VideoTimerCallback)
	(*info->VideoTimerCallback)(pScrn, currentTime.milliseconds);

#if defined(RENDER) && defined(USE_XAA)
    if(info->accel_state->RenderCallback)
	(*info->accel_state->RenderCallback)(pScrn);
#endif

#ifdef USE_EXA
    info->accel_state->engineMode = EXA_ENGINEMODE_UNKNOWN;
#endif
}

static void
RADEONPointerMoved(int index, int x, int y)
{
    ScrnInfoPtr pScrn = xf86Screens[index];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    int newX = x, newY = y;

    switch (info->rotation) {
    case RR_Rotate_0:
	break;
    case RR_Rotate_90:
	newX = y;
	newY = pScrn->pScreen->width - x - 1;
	break;
    case RR_Rotate_180:
	newX = pScrn->pScreen->width - x - 1;
	newY = pScrn->pScreen->height - y - 1;
	break;
    case RR_Rotate_270:
	newX = pScrn->pScreen->height - y - 1;
	newY = x;
	break;
    }

    (*info->PointerMoved)(index, newX, newY);
}

static void
RADEONInitBIOSRegisters(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONSavePtr save = info->ModeReg;

    save->bios_0_scratch = info->SavedReg->bios_0_scratch;
    save->bios_1_scratch = info->SavedReg->bios_1_scratch;
    save->bios_2_scratch = info->SavedReg->bios_2_scratch;
    save->bios_3_scratch = info->SavedReg->bios_3_scratch;
    save->bios_4_scratch = info->SavedReg->bios_4_scratch;
    save->bios_5_scratch = info->SavedReg->bios_5_scratch;
    save->bios_6_scratch = info->SavedReg->bios_6_scratch;
    save->bios_7_scratch = info->SavedReg->bios_7_scratch;

    if (info->IsAtomBios) {
	/* let the bios control the backlight */
	save->bios_2_scratch &= ~ATOM_S2_VRI_BRIGHT_ENABLE;
	/* tell the bios not to handle mode switching */
	save->bios_6_scratch |= (ATOM_S6_ACC_BLOCK_DISPLAY_SWITCH |
				 ATOM_S6_ACC_MODE);

	if (info->ChipFamily >= CHIP_FAMILY_R600) {
	    OUTREG(R600_BIOS_2_SCRATCH, save->bios_2_scratch);
	    OUTREG(R600_BIOS_6_SCRATCH, save->bios_6_scratch);
	} else {
	    OUTREG(RADEON_BIOS_2_SCRATCH, save->bios_2_scratch);
	    OUTREG(RADEON_BIOS_6_SCRATCH, save->bios_6_scratch);
	}
    } else {
	/* let the bios control the backlight */
	save->bios_0_scratch &= ~RADEON_DRIVER_BRIGHTNESS_EN;
	/* tell the bios not to handle mode switching */
	save->bios_6_scratch |= (RADEON_DISPLAY_SWITCHING_DIS |
				 RADEON_ACC_MODE_CHANGE);
	/* tell the bios a driver is loaded */
	save->bios_7_scratch |= RADEON_DRV_LOADED;

	OUTREG(RADEON_BIOS_0_SCRATCH, save->bios_0_scratch);
	OUTREG(RADEON_BIOS_6_SCRATCH, save->bios_6_scratch);
	//OUTREG(RADEON_BIOS_7_SCRATCH, save->bios_7_scratch);
    }

}


/* Called at the start of each server generation. */
Bool RADEONScreenInit(int scrnIndex, ScreenPtr pScreen,
                                int argc, char **argv)
{
    ScrnInfoPtr    pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    int            hasDRI = 0;
#ifdef RENDER
    int            subPixelOrder = SubPixelUnknown;
    char*          s;
#endif


    info->accelOn      = FALSE;
#ifdef USE_XAA
    info->accel_state->accel        = NULL;
#endif
#ifdef XF86DRI
    pScrn->fbOffset    = info->dri->frontOffset;
#endif

    if (info->IsSecondary) pScrn->fbOffset = pScrn->videoRam * 1024;
#ifdef XF86DRI
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
		   "RADEONScreenInit %lx %ld %d\n",
		   pScrn->memPhysBase, pScrn->fbOffset, info->dri->frontOffset);
#else
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONScreenInit %lx %ld\n",
		   pScrn->memPhysBase, pScrn->fbOffset);
#endif
    if (!RADEONMapMem(pScrn)) return FALSE;

#ifdef XF86DRI
    info->dri->fbX = 0;
    info->dri->fbY = 0;
#endif

    info->PaletteSavedOnVT = FALSE;

    info->crtc_on = FALSE;
    info->crtc2_on = FALSE;

    /* save the real front buffer size
     * it changes with randr, rotation, etc.
     */
    info->virtualX = pScrn->virtualX;
    info->virtualY = pScrn->virtualY;

    RADEONSave(pScrn);

    /* set initial bios scratch reg state */
    RADEONInitBIOSRegisters(pScrn);

    /* blank the outputs/crtcs */
    RADEONBlank(pScrn);

    if (info->IsMobility && !IS_AVIVO_VARIANT) {
	if (xf86ReturnOptValBool(info->Options, OPTION_DYNAMIC_CLOCKS, FALSE)) {
	    RADEONSetDynamicClock(pScrn, 1);
	} else {
	    RADEONSetDynamicClock(pScrn, 0);
	}
    } else if (IS_AVIVO_VARIANT) {
	if (xf86ReturnOptValBool(info->Options, OPTION_DYNAMIC_CLOCKS, FALSE)) {
	    atombios_static_pwrmgt_setup(pScrn, 1);
	    atombios_dyn_clk_setup(pScrn, 1);
	}
    }

    if (IS_R300_VARIANT || IS_RV100_VARIANT)
	RADEONForceSomeClocks(pScrn);

    if (info->allowColorTiling && (pScrn->virtualX > info->MaxSurfaceWidth)) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Color tiling not supported with virtual x resolutions larger than %d, disabling\n",
		    info->MaxSurfaceWidth);
	info->allowColorTiling = FALSE;
    }
    if (info->allowColorTiling) {
        info->tilingEnabled = (pScrn->currentMode->Flags & (V_DBLSCAN | V_INTERLACE)) ? FALSE : TRUE;
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

	info->dri->depthBits = pScrn->depth;

	from = xf86GetOptValInteger(info->Options, OPTION_DEPTH_BITS,
				    &info->dri->depthBits)
	     ? X_CONFIG : X_DEFAULT;

	if (info->dri->depthBits != 16 && info->dri->depthBits != 24) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Value for Option \"DepthBits\" must be 16 or 24\n");
	    info->dri->depthBits = pScrn->depth;
	    from = X_DEFAULT;
	}

	xf86DrvMsg(pScrn->scrnIndex, from,
		   "Using %d bit depth buffer\n", info->dri->depthBits);
    }


    hasDRI = info->directRenderingEnabled;
#endif /* XF86DRI */

    /* Initialize the memory map, this basically calculates the values
     * we'll use later on for MC_FB_LOCATION & MC_AGP_LOCATION
     */
    RADEONInitMemoryMap(pScrn);

    /* empty the surfaces */
    if (info->ChipFamily < CHIP_FAMILY_R600) {
	unsigned char *RADEONMMIO = info->MMIO;
	unsigned int j;
	for (j = 0; j < 8; j++) {
	    OUTREG(RADEON_SURFACE0_INFO + 16 * j, 0);
	    OUTREG(RADEON_SURFACE0_LOWER_BOUND + 16 * j, 0);
	    OUTREG(RADEON_SURFACE0_UPPER_BOUND + 16 * j, 0);
	}
    }

#ifdef XF86DRI
    /* Depth moves are disabled by default since they are extremely slow */
    info->dri->depthMoves = xf86ReturnOptValBool(info->Options,
						 OPTION_DEPTH_MOVE, FALSE);
    if (info->dri->depthMoves && info->allowColorTiling) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Enabling depth moves\n");
    } else if (info->dri->depthMoves) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Depth moves don't work without color tiling, disabled\n");
	info->dri->depthMoves = FALSE;
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Depth moves disabled by default\n");
    }
#endif

    /* Initial setup of surfaces */
    if (info->ChipFamily < CHIP_FAMILY_R600) {
        xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
                       "Setting up initial surfaces\n");
        RADEONChangeSurfaces(pScrn);
    }

				/* Memory manager setup */

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Setting up accel memmap\n");

#ifdef USE_EXA
    if (info->useEXA) {
#ifdef XF86DRI
	if (hasDRI) {
	    info->accelDFS = xf86ReturnOptValBool(info->Options, OPTION_ACCEL_DFS,
						  info->cardType != CARD_AGP);

	    /* Reserve approx. half of offscreen memory for local textures by
	     * default, can be overridden with Option "FBTexPercent".
	     * Round down to a whole number of texture regions.
	     */
	    info->dri->textureSize = 50;

	    if (xf86GetOptValInteger(info->Options, OPTION_FBTEX_PERCENT,
				     &(info->dri->textureSize))) {
		if (info->dri->textureSize < 0 || info->dri->textureSize > 100) {
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			       "Illegal texture memory percentage: %dx, setting to default 50%%\n",
			       info->dri->textureSize);
		    info->dri->textureSize = 50;
		}
	    }
	}
#endif /* XF86DRI */

	if (!RADEONSetupMemEXA(pScreen))
	    return FALSE;
    }
#endif

#if defined(XF86DRI) && defined(USE_XAA)
    if (!info->useEXA && hasDRI) {
	info->dri->textureSize = -1;
	if (xf86GetOptValInteger(info->Options, OPTION_FBTEX_PERCENT,
				 &(info->dri->textureSize))) {
	    if (info->dri->textureSize < 0 || info->dri->textureSize > 100) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Illegal texture memory percentage: %dx, using default behaviour\n",
			   info->dri->textureSize);
		info->dri->textureSize = -1;
	    }
	}
	if (!RADEONSetupMemXAA_DRI(scrnIndex, pScreen))
	    return FALSE;
    	pScrn->fbOffset    = info->dri->frontOffset;
    }
#endif

#ifdef USE_XAA
    if (!info->useEXA && !hasDRI && !RADEONSetupMemXAA(scrnIndex, pScreen))
	return FALSE;
#endif

    info->accel_state->dst_pitch_offset =
	(((pScrn->displayWidth * info->CurrentLayout.pixel_bytes / 64)
	  << 22) | ((info->fbLocation + pScrn->fbOffset) >> 10));

    /* Setup DRI after visuals have been established, but before fbScreenInit is
     * called.  fbScreenInit will eventually call the driver's InitGLXVisuals
     * call back. */
#ifdef XF86DRI
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
    if (info->directRenderingEnabled && info->dri->newMemoryMap) {
        if (RADEONDRISetParam(pScrn, RADEON_SETPARAM_NEW_MEMMAP, 1) < 0) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "[drm] failed to enable new memory map\n");
		RADEONDRICloseScreen(pScreen);
		info->directRenderingEnabled = FALSE;
	}
    }
#endif
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Initializing fb layer\n");

    if (info->r600_shadow_fb) {
	info->fb_shadow = xcalloc(1,
				  pScrn->displayWidth * pScrn->virtualY *
				  ((pScrn->bitsPerPixel + 7) >> 3));
	if (info->fb_shadow == NULL) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Failed to allocate shadow framebuffer\n");
	    info->r600_shadow_fb = FALSE;
	} else {
	    if (!fbScreenInit(pScreen, info->fb_shadow,
			      pScrn->virtualX, pScrn->virtualY,
			      pScrn->xDpi, pScrn->yDpi, pScrn->displayWidth,
			      pScrn->bitsPerPixel))
		return FALSE;
	}
    }

    if (info->r600_shadow_fb == FALSE) {
	/* Init fb layer */
	if (!fbScreenInit(pScreen, info->FB,
			  pScrn->virtualX, pScrn->virtualY,
			  pScrn->xDpi, pScrn->yDpi, pScrn->displayWidth,
			  pScrn->bitsPerPixel))
	    return FALSE;
    }

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
#endif

    pScrn->vtSema = TRUE;

    /* restore the memory map here otherwise we may get a hang when
     * initializing the drm below
     */
    RADEONInitMemMapRegisters(pScrn, info->ModeReg, info);
    RADEONRestoreMemMapRegisters(pScrn, info->ModeReg);

    /* Backing store setup */
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Initializing backing store\n");
    miInitializeBackingStore(pScreen);
    xf86SetBackingStore(pScreen);

    /* DRI finalisation */
#ifdef XF86DRI
    if (info->directRenderingEnabled && info->cardType==CARD_PCIE &&
        info->dri->pKernelDRMVersion->version_minor >= 19)
    {
      if (RADEONDRISetParam(pScrn, RADEON_SETPARAM_PCIGART_LOCATION, info->dri->pciGartOffset) < 0)
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "[drm] failed set pci gart location\n");

      if (info->dri->pKernelDRMVersion->version_minor >= 26) {
	if (RADEONDRISetParam(pScrn, RADEON_SETPARAM_PCIGART_TABLE_SIZE, info->dri->pciGartSize) < 0)
	  xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		     "[drm] failed set pci gart table size\n");
      }
    }
    if (info->directRenderingEnabled) {
        xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "DRI Finishing init !\n");
	info->directRenderingEnabled = RADEONDRIFinishScreenInit(pScreen);
    }
    if (info->directRenderingEnabled) {
	/* DRI final init might have changed the memory map, we need to adjust
	 * our local image to make sure we restore them properly on mode
	 * changes or VT switches
	 */
	RADEONAdjustMemMapRegisters(pScrn, info->ModeReg);

	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Direct rendering enabled\n");

	/* we might already be in tiled mode, tell drm about it */
	if (info->directRenderingEnabled && info->tilingEnabled) {
	  if (RADEONDRISetParam(pScrn, RADEON_SETPARAM_SWITCH_TILING, (info->tilingEnabled ? 1 : 0)) < 0)
  	      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			 "[drm] failed changing tiling status\n");
	}
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
		   "Direct rendering disabled\n");
    }
#endif

    /* Make sure surfaces are allright since DRI setup may have changed them */
    if (info->ChipFamily < CHIP_FAMILY_R600) {
        xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
                       "Setting up final surfaces\n");

        RADEONChangeSurfaces(pScrn);
    }


    /* Enable aceleration */
    if (!xf86ReturnOptValBool(info->Options, OPTION_NOACCEL, FALSE)) {
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Initializing Acceleration\n");
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

    /* Init DPMS */
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Initializing DPMS\n");
    xf86DPMSInit(pScreen, xf86DPMSSet, 0);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Initializing Cursor\n");

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
	xf86DrvMsg(scrnIndex, X_INFO, "Using software cursor\n");
    }

    /* DGA setup */
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Initializing DGA\n");
    RADEONDGAInit(pScreen);

    /* Init Xv */
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Initializing Xv\n");
    RADEONInitVideo(pScreen);

    if (info->r600_shadow_fb == TRUE) {
        if (!shadowSetup(pScreen)) {
            return FALSE;
        }
    }

    /* xf86SetDesiredModes() accesses pScrn->pScreen */
    pScrn->pScreen = pScreen;

    /* set the modes with desired rotation, etc. */
    if (!xf86SetDesiredModes (pScrn))
	return FALSE;

    /* Provide SaveScreen & wrap BlockHandler and CloseScreen */
    /* Wrap CloseScreen */
    info->CloseScreen    = pScreen->CloseScreen;
    pScreen->CloseScreen = RADEONCloseScreen;
    pScreen->SaveScreen  = RADEONSaveScreen;
    info->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = RADEONBlockHandler;
    info->CreateScreenResources = pScreen->CreateScreenResources;
    pScreen->CreateScreenResources = RADEONCreateScreenResources;

    if (!xf86CrtcScreenInit (pScreen))
       return FALSE;

    /* Wrap pointer motion to flip touch screen around */
    info->PointerMoved = pScrn->PointerMoved;
    pScrn->PointerMoved = RADEONPointerMoved;

    /* Colormap setup */
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
                   "Initializing color map\n");
    if (!miCreateDefColormap(pScreen)) return FALSE;
    /* all radeons support 10 bit CLUTs */
    if (!xf86HandleColormaps(pScreen, 256, 10,
			     RADEONLoadPalette, NULL,
			     CMAP_PALETTED_TRUECOLOR
#if 0 /* This option messes up text mode! (eich@suse.de) */
			     | CMAP_LOAD_EVEN_IF_OFFSCREEN
#endif
			     | CMAP_RELOAD_ON_MODE_SWITCH)) return FALSE;

    /* Note unused options */
    if (serverGeneration == 1)
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONScreenInit finished\n");

    return TRUE;
}

/* Write memory mapping registers */
void RADEONRestoreMemMapRegisters(ScrnInfoPtr pScrn,
					 RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int timeout;
    uint32_t mc_fb_loc, mc_agp_loc, mc_agp_loc_hi;

    radeon_read_mc_fb_agp_location(pScrn, LOC_FB | LOC_AGP, &mc_fb_loc,
				   &mc_agp_loc, &mc_agp_loc_hi);

    if (info->IsSecondary)
      return;
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "RADEONRestoreMemMapRegisters() : \n");
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "  MC_FB_LOCATION   : 0x%08x 0x%08x\n",
	       (unsigned)restore->mc_fb_location, (unsigned int)mc_fb_loc);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "  MC_AGP_LOCATION  : 0x%08x\n",
	       (unsigned)restore->mc_agp_location);

    if (IS_AVIVO_VARIANT) {

	if (mc_fb_loc != restore->mc_fb_location ||
	    mc_agp_loc != restore->mc_agp_location) {
	    uint32_t tmp;

	    RADEONWaitForIdleMMIO(pScrn);

	    OUTREG(AVIVO_D1VGA_CONTROL, INREG(AVIVO_D1VGA_CONTROL) & ~AVIVO_DVGA_CONTROL_MODE_ENABLE);
	    OUTREG(AVIVO_D2VGA_CONTROL, INREG(AVIVO_D2VGA_CONTROL) & ~AVIVO_DVGA_CONTROL_MODE_ENABLE);

	    /* Stop display & memory access */
	    tmp = INREG(AVIVO_D1CRTC_CONTROL);
	    OUTREG(AVIVO_D1CRTC_CONTROL, tmp & ~AVIVO_CRTC_EN);

	    tmp = INREG(AVIVO_D2CRTC_CONTROL);
	    OUTREG(AVIVO_D2CRTC_CONTROL, tmp & ~AVIVO_CRTC_EN);

	    tmp = INREG(AVIVO_D2CRTC_CONTROL);

	    usleep(10000);
	    timeout = 0;
	    while (!(radeon_get_mc_idle(pScrn))) {
		if (++timeout > 1000000) {
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			       "Timeout trying to update memory controller settings !\n");
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

	    radeon_write_mc_fb_agp_location(pScrn, LOC_FB | LOC_AGP,
					    restore->mc_fb_location,
					    restore->mc_agp_location,
					    restore->mc_agp_location_hi);

	    if (info->ChipFamily < CHIP_FAMILY_R600) {
		OUTREG(AVIVO_HDP_FB_LOCATION, restore->mc_fb_location);
	    } else {
		OUTREG(R600_HDP_NONSURFACE_BASE, (restore->mc_fb_location << 16) & 0xff0000);
	    }

	    /* Reset the engine and HDP */
	    if (info->ChipFamily < CHIP_FAMILY_R600)
		RADEONEngineReset(pScrn);
	}
    } else {

	/* Write memory mapping registers only if their value change
	 * since we must ensure no access is done while they are
	 * reprogrammed
	 */
	if (mc_fb_loc != restore->mc_fb_location ||
	    mc_agp_loc != restore->mc_agp_location) {
	    uint32_t crtc_ext_cntl, crtc_gen_cntl, crtc2_gen_cntl=0, ov0_scale_cntl;
	    uint32_t old_mc_status;

	    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
			   "  Map Changed ! Applying ...\n");

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

	    if (pRADEONEnt->HasCRTC2) {
		crtc2_gen_cntl = INREG(RADEON_CRTC2_GEN_CNTL);
		RADEONWaitForVerticalSync2(pScrn);
		OUTREG(RADEON_CRTC2_GEN_CNTL,
		       (crtc2_gen_cntl
			& ~(RADEON_CRTC2_CUR_EN | RADEON_CRTC2_ICON_EN))
		       | RADEON_CRTC2_DISP_REQ_EN_B);
	    }

	    /* Make sure the chip settles down (paranoid !) */ 
	    usleep(100000);
	    timeout = 0;
	    while (!(radeon_get_mc_idle(pScrn))) {
		if (++timeout > 1000000) {
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			       "Timeout trying to update memory controller settings !\n");
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			       "MC_STATUS = 0x%08x (on entry = 0x%08x)\n",
			       (unsigned int)INREG(RADEON_MC_STATUS), (unsigned int)old_mc_status);
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
	    radeon_write_mc_fb_agp_location(pScrn, LOC_FB | LOC_AGP, restore->mc_fb_location,
					    0xfffffffc, 0);
	igp_no_mcfb:
	    radeon_write_mc_fb_agp_location(pScrn, LOC_AGP, 0,
					    restore->mc_agp_location, 0);
	    /* Make sure map fully reached the chip */
	    (void)INREG(RADEON_MC_FB_LOCATION);

	    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
			   "  Map applied, resetting engine ...\n");

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
	    if (pRADEONEnt->HasCRTC2) {
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

	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Updating display base addresses...\n");

	OUTREG(RADEON_DISPLAY_BASE_ADDR, restore->display_base_addr);
	if (pRADEONEnt->HasCRTC2)
	    OUTREG(RADEON_DISPLAY2_BASE_ADDR, restore->display2_base_addr);
	OUTREG(RADEON_OV0_BASE_ADDR, restore->ov0_base_addr);
	(void)INREG(RADEON_OV0_BASE_ADDR);

	/* More paranoia delays, wait 100ms */
	usleep(100000);

	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		       "Memory map updated.\n");
    }
}

#ifdef XF86DRI
static void RADEONAdjustMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info   = RADEONPTR(pScrn);
    uint32_t fb, agp, agp_hi;
    int changed = 0;

    if (info->IsSecondary)
      return;

    radeon_read_mc_fb_agp_location(pScrn, LOC_FB | LOC_AGP, &fb, &agp, &agp_hi);

    if (fb != save->mc_fb_location || agp != save->mc_agp_location ||
	agp_hi != save->mc_agp_location_hi)
	changed = 1;

    if (changed) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "DRI init changed memory map, adjusting ...\n");
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "  MC_FB_LOCATION  was: 0x%08lx is: 0x%08lx\n",
		   (long unsigned int)info->mc_fb_location, (long unsigned int)fb);
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "  MC_AGP_LOCATION was: 0x%08lx is: 0x%08lx\n",
		   (long unsigned int)info->mc_agp_location, (long unsigned int)agp);
	info->mc_fb_location = fb;
	info->mc_agp_location = agp;
	if (info->ChipFamily >= CHIP_FAMILY_R600)
	    info->fbLocation = (info->mc_fb_location & 0xffff) << 24;
	else
	    info->fbLocation = (info->mc_fb_location & 0xffff) << 16;

	info->accel_state->dst_pitch_offset =
	    (((pScrn->displayWidth * info->CurrentLayout.pixel_bytes / 64)
	      << 22) | ((info->fbLocation + pScrn->fbOffset) >> 10));
	RADEONInitMemMapRegisters(pScrn, save, info);
	RADEONRestoreMemMapRegisters(pScrn, save);
    }

#ifdef USE_EXA
    if (info->accelDFS || (info->ChipFamily >= CHIP_FAMILY_R600))
    {
	drm_radeon_getparam_t gp;
	int gart_base;

	memset(&gp, 0, sizeof(gp));
	gp.param = RADEON_PARAM_GART_BASE;
	gp.value = &gart_base;

	if (drmCommandWriteRead(info->dri->drmFD, DRM_RADEON_GETPARAM, &gp,
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
    } else if (IS_R300_VARIANT || IS_AVIVO_VARIANT) {
       color_pattern = R300_SURF_TILE_COLOR_MACRO;
    } else {
	color_pattern = R200_SURF_TILE_COLOR_MACRO;
    }   
#ifdef XF86DRI
    if (info->directRenderingInited) {
	drm_radeon_surface_free_t drmsurffree;
	drm_radeon_surface_alloc_t drmsurfalloc;
	int retvalue;
	int depthCpp = (info->dri->depthBits - 8) / 4;
	int depth_width_bytes = pScrn->displayWidth * depthCpp;
	int depthBufferSize = ((((pScrn->virtualY + 15) & ~15) * depth_width_bytes
				+ RADEON_BUFFER_ALIGN) & ~RADEON_BUFFER_ALIGN);
	unsigned int depth_pattern;

	drmsurffree.address = info->dri->frontOffset;
	retvalue = drmCommandWrite(info->dri->drmFD, DRM_RADEON_SURF_FREE,
	    &drmsurffree, sizeof(drmsurffree));

	if (!((info->ChipFamily == CHIP_FAMILY_RV100) ||
	    (info->ChipFamily == CHIP_FAMILY_RS100) ||
	    (info->ChipFamily == CHIP_FAMILY_RS200))) {
	    drmsurffree.address = info->dri->depthOffset;
	    retvalue = drmCommandWrite(info->dri->drmFD, DRM_RADEON_SURF_FREE,
		&drmsurffree, sizeof(drmsurffree));
	}

	if (!info->dri->noBackBuffer) {
	    drmsurffree.address = info->dri->backOffset;
	    retvalue = drmCommandWrite(info->dri->drmFD, DRM_RADEON_SURF_FREE,
		&drmsurffree, sizeof(drmsurffree));
	}

	drmsurfalloc.size = bufferSize;
	drmsurfalloc.address = info->dri->frontOffset;
	drmsurfalloc.flags = swap_pattern;

	if (info->tilingEnabled) {
	    if (IS_R300_VARIANT || IS_AVIVO_VARIANT)
		drmsurfalloc.flags |= (width_bytes / 8) | color_pattern;
	    else
		drmsurfalloc.flags |= (width_bytes / 16) | color_pattern;
	}
	retvalue = drmCommandWrite(info->dri->drmFD, DRM_RADEON_SURF_ALLOC,
				   &drmsurfalloc, sizeof(drmsurfalloc));
	if (retvalue < 0)
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "drm: could not allocate surface for front buffer!\n");
	
	if ((info->dri->have3DWindows) && (!info->dri->noBackBuffer)) {
	    drmsurfalloc.address = info->dri->backOffset;
	    retvalue = drmCommandWrite(info->dri->drmFD, DRM_RADEON_SURF_ALLOC,
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
	} else if (IS_R300_VARIANT || IS_AVIVO_VARIANT) {
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
	if (info->dri->have3DWindows &&
	    (!((info->ChipFamily == CHIP_FAMILY_RV100) ||
	    (info->ChipFamily == CHIP_FAMILY_RS100) ||
	    (info->ChipFamily == CHIP_FAMILY_RS200)))) {
	    drm_radeon_surface_alloc_t drmsurfalloc;
	    drmsurfalloc.size = depthBufferSize;
	    drmsurfalloc.address = info->dri->depthOffset;
            if (IS_R300_VARIANT || IS_AVIVO_VARIANT)
                drmsurfalloc.flags = swap_pattern | (depth_width_bytes / 8) | depth_pattern;
            else
                drmsurfalloc.flags = swap_pattern | (depth_width_bytes / 16) | depth_pattern;
	    retvalue = drmCommandWrite(info->dri->drmFD, DRM_RADEON_SURF_ALLOC,
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
	if (info->tilingEnabled) {
	    if (IS_R300_VARIANT || IS_AVIVO_VARIANT)
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

    /* Update surface images */
    if (info->ChipFamily < CHIP_FAMILY_R600)
        RADEONSaveSurfaces(pScrn, info->ModeReg);
}

/* Read memory map */
static void RADEONSaveMemMapRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    radeon_read_mc_fb_agp_location(pScrn, LOC_FB | LOC_AGP, &save->mc_fb_location,
				   &save->mc_agp_location, &save->mc_agp_location_hi);

    if (!IS_AVIVO_VARIANT) {
        save->display_base_addr  = INREG(RADEON_DISPLAY_BASE_ADDR);
        save->display2_base_addr = INREG(RADEON_DISPLAY2_BASE_ADDR);
        save->ov0_base_addr      = INREG(RADEON_OV0_BASE_ADDR);
    }
}


#if 0
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
#endif

static void
avivo_save(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    struct avivo_state *state = &save->avivo;
    int i, j;

    //    state->vga_memory_base = INREG(AVIVO_VGA_MEMORY_BASE);
    //    state->vga_fb_start = INREG(AVIVO_VGA_FB_START);
    state->vga1_cntl = INREG(AVIVO_D1VGA_CONTROL);
    state->vga2_cntl = INREG(AVIVO_D2VGA_CONTROL);

    state->crtc_master_en = INREG(AVIVO_DC_CRTC_MASTER_EN);
    state->crtc_tv_control = INREG(AVIVO_DC_CRTC_TV_CONTROL);
    state->dc_lb_memory_split = INREG(AVIVO_DC_LB_MEMORY_SPLIT);

    state->pll1.ref_div_src = INREG(AVIVO_EXT1_PPLL_REF_DIV_SRC);
    state->pll1.ref_div = INREG(AVIVO_EXT1_PPLL_REF_DIV);
    state->pll1.fb_div = INREG(AVIVO_EXT1_PPLL_FB_DIV);
    state->pll1.post_div_src = INREG(AVIVO_EXT1_PPLL_POST_DIV_SRC);
    state->pll1.post_div = INREG(AVIVO_EXT1_PPLL_POST_DIV);
    state->pll1.ext_ppll_cntl = INREG(AVIVO_EXT1_PPLL_CNTL);
    state->pll1.pll_cntl = INREG(AVIVO_P1PLL_CNTL);
    state->pll1.int_ss_cntl = INREG(AVIVO_P1PLL_INT_SS_CNTL);

    state->pll2.ref_div_src = INREG(AVIVO_EXT1_PPLL_REF_DIV_SRC);
    state->pll2.ref_div = INREG(AVIVO_EXT2_PPLL_REF_DIV);
    state->pll2.fb_div = INREG(AVIVO_EXT2_PPLL_FB_DIV);
    state->pll2.post_div_src = INREG(AVIVO_EXT2_PPLL_POST_DIV_SRC);
    state->pll2.post_div = INREG(AVIVO_EXT2_PPLL_POST_DIV);
    state->pll2.ext_ppll_cntl = INREG(AVIVO_EXT2_PPLL_CNTL);
    state->pll2.pll_cntl = INREG(AVIVO_P2PLL_CNTL);
    state->pll2.int_ss_cntl = INREG(AVIVO_P2PLL_INT_SS_CNTL);

    state->vga25_ppll.ref_div_src = INREG(AVIVO_VGA25_PPLL_REF_DIV_SRC);
    state->vga25_ppll.ref_div = INREG(AVIVO_VGA25_PPLL_REF_DIV);
    state->vga25_ppll.fb_div = INREG(AVIVO_VGA25_PPLL_FB_DIV);
    state->vga25_ppll.post_div_src = INREG(AVIVO_VGA25_PPLL_POST_DIV_SRC);
    state->vga25_ppll.post_div = INREG(AVIVO_VGA25_PPLL_POST_DIV);
    state->vga25_ppll.pll_cntl = INREG(AVIVO_VGA25_PPLL_CNTL);

    state->vga28_ppll.ref_div_src = INREG(AVIVO_VGA28_PPLL_REF_DIV_SRC);
    state->vga28_ppll.ref_div = INREG(AVIVO_VGA28_PPLL_REF_DIV);
    state->vga28_ppll.fb_div = INREG(AVIVO_VGA28_PPLL_FB_DIV);
    state->vga28_ppll.post_div_src = INREG(AVIVO_VGA28_PPLL_POST_DIV_SRC);
    state->vga28_ppll.post_div = INREG(AVIVO_VGA28_PPLL_POST_DIV);
    state->vga28_ppll.pll_cntl = INREG(AVIVO_VGA28_PPLL_CNTL);

    state->vga41_ppll.ref_div_src = INREG(AVIVO_VGA41_PPLL_REF_DIV_SRC);
    state->vga41_ppll.ref_div = INREG(AVIVO_VGA41_PPLL_REF_DIV);
    state->vga41_ppll.fb_div = INREG(AVIVO_VGA41_PPLL_FB_DIV);
    state->vga41_ppll.post_div_src = INREG(AVIVO_VGA41_PPLL_POST_DIV_SRC);
    state->vga41_ppll.post_div = INREG(AVIVO_VGA41_PPLL_POST_DIV);
    state->vga41_ppll.pll_cntl = INREG(AVIVO_VGA41_PPLL_CNTL);

    state->crtc1.pll_source = INREG(AVIVO_PCLK_CRTC1_CNTL);

    state->crtc1.h_total = INREG(AVIVO_D1CRTC_H_TOTAL);
    state->crtc1.h_blank_start_end = INREG(AVIVO_D1CRTC_H_BLANK_START_END);
    state->crtc1.h_sync_a = INREG(AVIVO_D1CRTC_H_SYNC_A);
    state->crtc1.h_sync_a_cntl = INREG(AVIVO_D1CRTC_H_SYNC_A_CNTL);
    state->crtc1.h_sync_b = INREG(AVIVO_D1CRTC_H_SYNC_B);
    state->crtc1.h_sync_b_cntl = INREG(AVIVO_D1CRTC_H_SYNC_B_CNTL);

    state->crtc1.v_total = INREG(AVIVO_D1CRTC_V_TOTAL);
    state->crtc1.v_blank_start_end = INREG(AVIVO_D1CRTC_V_BLANK_START_END);
    state->crtc1.v_sync_a = INREG(AVIVO_D1CRTC_V_SYNC_A);
    state->crtc1.v_sync_a_cntl = INREG(AVIVO_D1CRTC_V_SYNC_A_CNTL);
    state->crtc1.v_sync_b = INREG(AVIVO_D1CRTC_V_SYNC_B);
    state->crtc1.v_sync_b_cntl = INREG(AVIVO_D1CRTC_V_SYNC_B_CNTL);

    state->crtc1.control = INREG(AVIVO_D1CRTC_CONTROL);
    state->crtc1.blank_control = INREG(AVIVO_D1CRTC_BLANK_CONTROL);
    state->crtc1.interlace_control = INREG(AVIVO_D1CRTC_INTERLACE_CONTROL);
    state->crtc1.stereo_control = INREG(AVIVO_D1CRTC_STEREO_CONTROL);

    state->crtc1.cursor_control = INREG(AVIVO_D1CUR_CONTROL);

    state->grph1.enable = INREG(AVIVO_D1GRPH_ENABLE);
    state->grph1.control = INREG(AVIVO_D1GRPH_CONTROL);
    state->grph1.control = INREG(AVIVO_D1GRPH_CONTROL);
    state->grph1.prim_surf_addr = INREG(AVIVO_D1GRPH_PRIMARY_SURFACE_ADDRESS);
    state->grph1.sec_surf_addr = INREG(AVIVO_D1GRPH_SECONDARY_SURFACE_ADDRESS);
    state->grph1.pitch = INREG(AVIVO_D1GRPH_PITCH);
    state->grph1.x_offset = INREG(AVIVO_D1GRPH_SURFACE_OFFSET_X);
    state->grph1.y_offset = INREG(AVIVO_D1GRPH_SURFACE_OFFSET_Y);
    state->grph1.x_start = INREG(AVIVO_D1GRPH_X_START);
    state->grph1.y_start = INREG(AVIVO_D1GRPH_Y_START);
    state->grph1.x_end = INREG(AVIVO_D1GRPH_X_END);
    state->grph1.y_end = INREG(AVIVO_D1GRPH_Y_END);

    state->grph1.desktop_height = INREG(AVIVO_D1MODE_DESKTOP_HEIGHT);
    state->grph1.viewport_start = INREG(AVIVO_D1MODE_VIEWPORT_START);
    state->grph1.viewport_size = INREG(AVIVO_D1MODE_VIEWPORT_SIZE);
    state->grph1.mode_data_format = INREG(AVIVO_D1MODE_DATA_FORMAT);

    state->crtc2.pll_source = INREG(AVIVO_PCLK_CRTC2_CNTL);

    state->crtc2.h_total = INREG(AVIVO_D2CRTC_H_TOTAL);
    state->crtc2.h_blank_start_end = INREG(AVIVO_D2CRTC_H_BLANK_START_END);
    state->crtc2.h_sync_a = INREG(AVIVO_D2CRTC_H_SYNC_A);
    state->crtc2.h_sync_a_cntl = INREG(AVIVO_D2CRTC_H_SYNC_A_CNTL);
    state->crtc2.h_sync_b = INREG(AVIVO_D2CRTC_H_SYNC_B);
    state->crtc2.h_sync_b_cntl = INREG(AVIVO_D2CRTC_H_SYNC_B_CNTL);

    state->crtc2.v_total = INREG(AVIVO_D2CRTC_V_TOTAL);
    state->crtc2.v_blank_start_end = INREG(AVIVO_D2CRTC_V_BLANK_START_END);
    state->crtc2.v_sync_a = INREG(AVIVO_D2CRTC_V_SYNC_A);
    state->crtc2.v_sync_a_cntl = INREG(AVIVO_D2CRTC_V_SYNC_A_CNTL);
    state->crtc2.v_sync_b = INREG(AVIVO_D2CRTC_V_SYNC_B);
    state->crtc2.v_sync_b_cntl = INREG(AVIVO_D2CRTC_V_SYNC_B_CNTL);

    state->crtc2.control = INREG(AVIVO_D2CRTC_CONTROL);
    state->crtc2.blank_control = INREG(AVIVO_D2CRTC_BLANK_CONTROL);
    state->crtc2.interlace_control = INREG(AVIVO_D2CRTC_INTERLACE_CONTROL);
    state->crtc2.stereo_control = INREG(AVIVO_D2CRTC_STEREO_CONTROL);

    state->crtc2.cursor_control = INREG(AVIVO_D2CUR_CONTROL);

    state->grph2.enable = INREG(AVIVO_D2GRPH_ENABLE);
    state->grph2.control = INREG(AVIVO_D2GRPH_CONTROL);
    state->grph2.control = INREG(AVIVO_D2GRPH_CONTROL);
    state->grph2.prim_surf_addr = INREG(AVIVO_D2GRPH_PRIMARY_SURFACE_ADDRESS);
    state->grph2.sec_surf_addr = INREG(AVIVO_D2GRPH_SECONDARY_SURFACE_ADDRESS);
    state->grph2.pitch = INREG(AVIVO_D2GRPH_PITCH);
    state->grph2.x_offset = INREG(AVIVO_D2GRPH_SURFACE_OFFSET_X);
    state->grph2.y_offset = INREG(AVIVO_D2GRPH_SURFACE_OFFSET_Y);
    state->grph2.x_start = INREG(AVIVO_D2GRPH_X_START);
    state->grph2.y_start = INREG(AVIVO_D2GRPH_Y_START);
    state->grph2.x_end = INREG(AVIVO_D2GRPH_X_END);
    state->grph2.y_end = INREG(AVIVO_D2GRPH_Y_END);

    state->grph2.desktop_height = INREG(AVIVO_D2MODE_DESKTOP_HEIGHT);
    state->grph2.viewport_start = INREG(AVIVO_D2MODE_VIEWPORT_START);
    state->grph2.viewport_size = INREG(AVIVO_D2MODE_VIEWPORT_SIZE);
    state->grph2.mode_data_format = INREG(AVIVO_D2MODE_DATA_FORMAT);

    if (IS_DCE3_VARIANT) {
	/* save DVOA regs */
	state->dvoa[0] = INREG(0x7080);
	state->dvoa[1] = INREG(0x7084);
	state->dvoa[2] = INREG(0x708c);
	state->dvoa[3] = INREG(0x7090);
	state->dvoa[4] = INREG(0x7094);
	state->dvoa[5] = INREG(0x70ac);
	state->dvoa[6] = INREG(0x70b0);

	j = 0;
	/* save DAC regs */
	for (i = 0x7000; i <= 0x7040; i += 4) {
	    state->daca[j] = INREG(i);
	    state->dacb[j] = INREG(i + 0x100);
	    j++;
	}
	for (i = 0x7058; i <= 0x7060; i += 4) {
	    state->daca[j] = INREG(i);
	    state->dacb[j] = INREG(i + 0x100);
	    j++;
	}
	for (i = 0x7068; i <= 0x706c; i += 4) {
	    state->daca[j] = INREG(i);
	    state->dacb[j] = INREG(i + 0x100);
	    j++;
	}
	for (i = 0x7ef0; i <= 0x7ef8; i += 4) {
	    state->daca[j] = INREG(i);
	    state->dacb[j] = INREG(i + 0x100);
	    j++;
	}
	state->daca[j] = INREG(0x7050);
	state->dacb[j] = INREG(0x7050 + 0x100);

	j = 0;
	/* save FMT regs */
	for (i = 0x6700; i <= 0x6744; i += 4) {
	    state->fmt1[j] = INREG(i);
	    state->fmt2[j] = INREG(i + 0x800);
	    j++;
	}

	j = 0;
	/* save DIG regs */
	for (i = 0x75a0; i <= 0x75e0; i += 4) {
	    state->dig1[j] = INREG(i);
	    state->dig2[j] = INREG(i + 0x400);
	    j++;
	}
	for (i = 0x75e8; i <= 0x75ec; i += 4) {
	    state->dig1[j] = INREG(i);
	    state->dig2[j] = INREG(i + 0x400);
	    j++;
	}

	j = 0;
	/* save HDMI regs */
	for (i = 0x7400; i <= 0x741c; i += 4) {
	    state->hdmi1[j] = INREG(i);
	    state->hdmi2[j] = INREG(i + 0x400);
	    j++;
	}
	for (i = 0x7430; i <= 0x74ec; i += 4) {
	    state->hdmi1[j] = INREG(i);
	    state->hdmi2[j] = INREG(i + 0x400);
	    j++;
	}
	state->hdmi1[j] = INREG(0x7428);
	state->hdmi2[j] = INREG(0x7828);

	j = 0;
	/* save AUX regs */
	for (i = 0x7780; i <= 0x77b4; i += 4) {
	    state->aux_cntl1[j] = INREG(i);
	    state->aux_cntl2[j] = INREG(i + 0x040);
	    state->aux_cntl3[j] = INREG(i + 0x400);
	    state->aux_cntl4[j] = INREG(i + 0x440);
	    if (IS_DCE32_VARIANT) {
		state->aux_cntl5[j] = INREG(i + 0x500);
		state->aux_cntl6[j] = INREG(i + 0x540);
	    }
	    j++;
	}

	j = 0;
	/* save UNIPHY regs */
	if (IS_DCE32_VARIANT) {
	    for (i = 0x7680; i <= 0x7690; i += 4) {
		state->uniphy1[j] = INREG(i);
		state->uniphy2[j] = INREG(i + 0x20);
		state->uniphy3[j] = INREG(i + 0x400);
		state->uniphy4[j] = INREG(i + 0x420);
		state->uniphy5[j] = INREG(i + 0x840);
		state->uniphy6[j] = INREG(i + 0x940);
		j++;
	    }
	    for (i = 0x7698; i <= 0x769c; i += 4) {
		state->uniphy1[j] = INREG(i);
		state->uniphy2[j] = INREG(i + 0x20);
		state->uniphy3[j] = INREG(i + 0x400);
		state->uniphy4[j] = INREG(i + 0x420);
		state->uniphy5[j] = INREG(i + 0x840);
		state->uniphy6[j] = INREG(i + 0x940);
		j++;
	    }
	} else {
	    for (i = 0x7ec0; i <= 0x7edc; i += 4) {
		state->uniphy1[j] = INREG(i);
		state->uniphy2[j] = INREG(i + 0x100);
		j++;
	    }
	}
	j = 0;
	/* save PHY,LINK regs */
	for (i = 0x7f20; i <= 0x7f34; i += 4) {
	    state->phy[j] = INREG(i);
	    j++;
	}
	for (i = 0x7f9c; i <= 0x7fa4; i += 4) {
	    state->phy[j] = INREG(i);
	    j++;
	}
	state->phy[j] = INREG(0x7f40);

	j = 0;
	/* save LVTMA regs */
	for (i = 0x7f00; i <= 0x7f1c; i += 4) {
	    state->lvtma[j] = INREG(i);
	    j++;
	}
	for (i = 0x7f80; i <= 0x7f98; i += 4) {
	    state->lvtma[j] = INREG(i);
	    j++;
	}
    } else {
	j = 0;
	/* save DVOA regs */
	for (i = 0x7980; i <= 0x79bc; i += 4) {
	    state->dvoa[j] = INREG(i);
	    j++;
	}

	j = 0;
	/* save DAC regs */
	for (i = 0x7800; i <= 0x782c; i += 4) {
	    state->daca[j] = INREG(i);
	    state->dacb[j] = INREG(i + 0x200);
	    j++;
	}
	for (i = 0x7834; i <= 0x7840; i += 4) {
	    state->daca[j] = INREG(i);
	    state->dacb[j] = INREG(i + 0x200);
	    j++;
	}
	for (i = 0x7850; i <= 0x7868; i += 4) {
	    state->daca[j] = INREG(i);
	    state->dacb[j] = INREG(i + 0x200);
	    j++;
	}

	j = 0;
	/* save TMDSA regs */
	for (i = 0x7880; i <= 0x78e0; i += 4) {
	    state->tmdsa[j] = INREG(i);
	    j++;
	}
	for (i = 0x7904; i <= 0x7918; i += 4) {
	    state->tmdsa[j] = INREG(i);
	    j++;
	}

	j = 0;
	/* save LVTMA regs */
	for (i = 0x7a80; i <= 0x7b18; i += 4) {
	    state->lvtma[j] = INREG(i);
	    j++;
	}

	if ((info->ChipFamily == CHIP_FAMILY_RS600) ||
	    (info->ChipFamily == CHIP_FAMILY_RS690) ||
	    (info->ChipFamily == CHIP_FAMILY_RS740)) {
	    j = 0;
	    /* save DDIA regs */
	    for (i = 0x7200; i <= 0x7290; i += 4) {
		state->ddia[j] = INREG(i);
		j++;
	    }
	}
    }

    /* scalers */
    j = 0;
    for (i = 0x6578; i <= 0x65e4; i += 4) {
	state->d1scl[j] = INREG(i);
	state->d2scl[j] = INREG(i + 0x800);
	j++;
    }
    for (i = 0x6600; i <= 0x662c; i += 4) {
	state->d1scl[j] = INREG(i);
	state->d2scl[j] = INREG(i + 0x800);
	j++;
    }
    j = 0;
    for (i = 0x66e8; i <= 0x66fc; i += 4) {
	state->dxscl[j] = INREG(i);
	j++;
    }
    state->dxscl[6] = INREG(0x6e30);
    state->dxscl[7] = INREG(0x6e34);

    if (state->crtc1.control & AVIVO_CRTC_EN)
	info->crtc_on = TRUE;

    if (state->crtc2.control & AVIVO_CRTC_EN)
	info->crtc2_on = TRUE;

}

static void
avivo_restore(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    struct avivo_state *state = &restore->avivo;
    int i, j;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "avivo_restore !\n");

    /* Disable VGA control for now.. maybe needs to be changed */
    OUTREG(AVIVO_D1VGA_CONTROL, 0);
    OUTREG(AVIVO_D2VGA_CONTROL, 0);

    /* Disable CRTCs */
    OUTREG(AVIVO_D1CRTC_CONTROL,
	   (INREG(AVIVO_D1CRTC_CONTROL) & ~0x300) | 0x01000000);
    OUTREG(AVIVO_D2CRTC_CONTROL,
	   (INREG(AVIVO_D2CRTC_CONTROL) & ~0x300) | 0x01000000);
    OUTREG(AVIVO_D1CRTC_CONTROL,
	   INREG(AVIVO_D1CRTC_CONTROL) & ~0x1);
    OUTREG(AVIVO_D2CRTC_CONTROL,
	   INREG(AVIVO_D2CRTC_CONTROL) & ~0x1);
    OUTREG(AVIVO_D1CRTC_CONTROL,
	   INREG(AVIVO_D1CRTC_CONTROL) | 0x100);
    OUTREG(AVIVO_D2CRTC_CONTROL,
	   INREG(AVIVO_D2CRTC_CONTROL) | 0x100);

    /* Lock graph registers */
    OUTREG(AVIVO_D1GRPH_UPDATE, AVIVO_D1GRPH_UPDATE_LOCK);
    OUTREG(AVIVO_D1GRPH_PRIMARY_SURFACE_ADDRESS, state->grph1.prim_surf_addr);
    OUTREG(AVIVO_D1GRPH_SECONDARY_SURFACE_ADDRESS, state->grph1.sec_surf_addr);
    OUTREG(AVIVO_D1GRPH_CONTROL, state->grph1.control);
    OUTREG(AVIVO_D1GRPH_SURFACE_OFFSET_X, state->grph1.x_offset);
    OUTREG(AVIVO_D1GRPH_SURFACE_OFFSET_Y, state->grph1.y_offset);
    OUTREG(AVIVO_D1GRPH_X_START, state->grph1.x_start);
    OUTREG(AVIVO_D1GRPH_Y_START, state->grph1.y_start);
    OUTREG(AVIVO_D1GRPH_X_END, state->grph1.x_end);
    OUTREG(AVIVO_D1GRPH_Y_END, state->grph1.y_end);
    OUTREG(AVIVO_D1GRPH_PITCH, state->grph1.pitch);
    OUTREG(AVIVO_D1GRPH_ENABLE, state->grph1.enable);
    OUTREG(AVIVO_D1GRPH_UPDATE, 0);

    OUTREG(AVIVO_D2GRPH_UPDATE, AVIVO_D1GRPH_UPDATE_LOCK);
    OUTREG(AVIVO_D2GRPH_PRIMARY_SURFACE_ADDRESS, state->grph2.prim_surf_addr);
    OUTREG(AVIVO_D2GRPH_SECONDARY_SURFACE_ADDRESS, state->grph2.sec_surf_addr);
    OUTREG(AVIVO_D2GRPH_CONTROL, state->grph2.control);
    OUTREG(AVIVO_D2GRPH_SURFACE_OFFSET_X, state->grph2.x_offset);
    OUTREG(AVIVO_D2GRPH_SURFACE_OFFSET_Y, state->grph2.y_offset);
    OUTREG(AVIVO_D2GRPH_X_START, state->grph2.x_start);
    OUTREG(AVIVO_D2GRPH_Y_START, state->grph2.y_start);
    OUTREG(AVIVO_D2GRPH_X_END, state->grph2.x_end);
    OUTREG(AVIVO_D2GRPH_Y_END, state->grph2.y_end);
    OUTREG(AVIVO_D2GRPH_PITCH, state->grph2.pitch);
    OUTREG(AVIVO_D2GRPH_ENABLE, state->grph2.enable);
    OUTREG(AVIVO_D2GRPH_UPDATE, 0);

    /* Whack some mode regs too */
    OUTREG(AVIVO_D1SCL_UPDATE, AVIVO_D1SCL_UPDATE_LOCK);
    OUTREG(AVIVO_D1MODE_DESKTOP_HEIGHT, state->grph1.desktop_height);
    OUTREG(AVIVO_D1MODE_VIEWPORT_START, state->grph1.viewport_start);
    OUTREG(AVIVO_D1MODE_VIEWPORT_SIZE, state->grph1.viewport_size);
    OUTREG(AVIVO_D1MODE_DATA_FORMAT, state->grph1.mode_data_format);
    OUTREG(AVIVO_D1SCL_UPDATE, 0);

    OUTREG(AVIVO_D2SCL_UPDATE, AVIVO_D1SCL_UPDATE_LOCK);
    OUTREG(AVIVO_D2MODE_DESKTOP_HEIGHT, state->grph2.desktop_height);
    OUTREG(AVIVO_D2MODE_VIEWPORT_START, state->grph2.viewport_start);
    OUTREG(AVIVO_D2MODE_VIEWPORT_SIZE, state->grph2.viewport_size);
    OUTREG(AVIVO_D2MODE_DATA_FORMAT, state->grph2.mode_data_format);
    OUTREG(AVIVO_D2SCL_UPDATE, 0);

    /* Set the PLL */
    OUTREG(AVIVO_EXT1_PPLL_REF_DIV_SRC, state->pll1.ref_div_src);
    OUTREG(AVIVO_EXT1_PPLL_REF_DIV, state->pll1.ref_div);
    OUTREG(AVIVO_EXT1_PPLL_FB_DIV, state->pll1.fb_div);
    OUTREG(AVIVO_EXT1_PPLL_POST_DIV_SRC, state->pll1.post_div_src);
    OUTREG(AVIVO_EXT1_PPLL_POST_DIV, state->pll1.post_div);
    OUTREG(AVIVO_EXT1_PPLL_CNTL, state->pll1.ext_ppll_cntl);
    OUTREG(AVIVO_P1PLL_CNTL, state->pll1.pll_cntl);
    OUTREG(AVIVO_P1PLL_INT_SS_CNTL, state->pll1.int_ss_cntl);

    OUTREG(AVIVO_EXT2_PPLL_REF_DIV_SRC, state->pll2.ref_div_src);
    OUTREG(AVIVO_EXT2_PPLL_REF_DIV, state->pll2.ref_div);
    OUTREG(AVIVO_EXT2_PPLL_FB_DIV, state->pll2.fb_div);
    OUTREG(AVIVO_EXT2_PPLL_POST_DIV_SRC, state->pll2.post_div_src);
    OUTREG(AVIVO_EXT2_PPLL_POST_DIV, state->pll2.post_div);
    OUTREG(AVIVO_EXT2_PPLL_CNTL, state->pll2.ext_ppll_cntl);
    OUTREG(AVIVO_P2PLL_CNTL, state->pll2.pll_cntl);
    OUTREG(AVIVO_P2PLL_INT_SS_CNTL, state->pll2.int_ss_cntl);

    OUTREG(AVIVO_PCLK_CRTC1_CNTL, state->crtc1.pll_source);
    OUTREG(AVIVO_PCLK_CRTC2_CNTL, state->crtc2.pll_source);

    /* Set the vga PLL */
    OUTREG(AVIVO_VGA25_PPLL_REF_DIV_SRC, state->vga25_ppll.ref_div_src);
    OUTREG(AVIVO_VGA25_PPLL_REF_DIV, state->vga25_ppll.ref_div);
    OUTREG(AVIVO_VGA25_PPLL_FB_DIV, state->vga25_ppll.fb_div);
    OUTREG(AVIVO_VGA25_PPLL_POST_DIV_SRC, state->vga25_ppll.post_div_src);
    OUTREG(AVIVO_VGA25_PPLL_POST_DIV, state->vga25_ppll.post_div);
    OUTREG(AVIVO_VGA25_PPLL_CNTL, state->vga25_ppll.pll_cntl);

    OUTREG(AVIVO_VGA28_PPLL_REF_DIV_SRC, state->vga28_ppll.ref_div_src);
    OUTREG(AVIVO_VGA28_PPLL_REF_DIV, state->vga28_ppll.ref_div);
    OUTREG(AVIVO_VGA28_PPLL_FB_DIV, state->vga28_ppll.fb_div);
    OUTREG(AVIVO_VGA28_PPLL_POST_DIV_SRC, state->vga28_ppll.post_div_src);
    OUTREG(AVIVO_VGA28_PPLL_POST_DIV, state->vga28_ppll.post_div);
    OUTREG(AVIVO_VGA28_PPLL_CNTL, state->vga28_ppll.pll_cntl);

    OUTREG(AVIVO_VGA41_PPLL_REF_DIV_SRC, state->vga41_ppll.ref_div_src);
    OUTREG(AVIVO_VGA41_PPLL_REF_DIV, state->vga41_ppll.ref_div);
    OUTREG(AVIVO_VGA41_PPLL_FB_DIV, state->vga41_ppll.fb_div);
    OUTREG(AVIVO_VGA41_PPLL_POST_DIV_SRC, state->vga41_ppll.post_div_src);
    OUTREG(AVIVO_VGA41_PPLL_POST_DIV, state->vga41_ppll.post_div);
    OUTREG(AVIVO_VGA41_PPLL_CNTL, state->vga41_ppll.pll_cntl);

    /* Set the CRTC */
    OUTREG(AVIVO_D1CRTC_H_TOTAL, state->crtc1.h_total);
    OUTREG(AVIVO_D1CRTC_H_BLANK_START_END, state->crtc1.h_blank_start_end);
    OUTREG(AVIVO_D1CRTC_H_SYNC_A, state->crtc1.h_sync_a);
    OUTREG(AVIVO_D1CRTC_H_SYNC_A_CNTL, state->crtc1.h_sync_a_cntl);
    OUTREG(AVIVO_D1CRTC_H_SYNC_B, state->crtc1.h_sync_b);
    OUTREG(AVIVO_D1CRTC_H_SYNC_B_CNTL, state->crtc1.h_sync_b_cntl);

    OUTREG(AVIVO_D1CRTC_V_TOTAL, state->crtc1.v_total);
    OUTREG(AVIVO_D1CRTC_V_BLANK_START_END, state->crtc1.v_blank_start_end);
    OUTREG(AVIVO_D1CRTC_V_SYNC_A, state->crtc1.v_sync_a);
    OUTREG(AVIVO_D1CRTC_V_SYNC_A_CNTL, state->crtc1.v_sync_a_cntl);
    OUTREG(AVIVO_D1CRTC_V_SYNC_B, state->crtc1.v_sync_b);
    OUTREG(AVIVO_D1CRTC_V_SYNC_B_CNTL, state->crtc1.v_sync_b_cntl);

    OUTREG(AVIVO_D1CRTC_INTERLACE_CONTROL, state->crtc1.interlace_control);
    OUTREG(AVIVO_D1CRTC_STEREO_CONTROL, state->crtc1.stereo_control);

    OUTREG(AVIVO_D1CUR_CONTROL, state->crtc1.cursor_control);

    /* XXX Fix scaler */

    OUTREG(AVIVO_D2CRTC_H_TOTAL, state->crtc2.h_total);
    OUTREG(AVIVO_D2CRTC_H_BLANK_START_END, state->crtc2.h_blank_start_end);
    OUTREG(AVIVO_D2CRTC_H_SYNC_A, state->crtc2.h_sync_a);
    OUTREG(AVIVO_D2CRTC_H_SYNC_A_CNTL, state->crtc2.h_sync_a_cntl);
    OUTREG(AVIVO_D2CRTC_H_SYNC_B, state->crtc2.h_sync_b);
    OUTREG(AVIVO_D2CRTC_H_SYNC_B_CNTL, state->crtc2.h_sync_b_cntl);

    OUTREG(AVIVO_D2CRTC_V_TOTAL, state->crtc2.v_total);
    OUTREG(AVIVO_D2CRTC_V_BLANK_START_END, state->crtc2.v_blank_start_end);
    OUTREG(AVIVO_D2CRTC_V_SYNC_A, state->crtc2.v_sync_a);
    OUTREG(AVIVO_D2CRTC_V_SYNC_A_CNTL, state->crtc2.v_sync_a_cntl);
    OUTREG(AVIVO_D2CRTC_V_SYNC_B, state->crtc2.v_sync_b);
    OUTREG(AVIVO_D2CRTC_V_SYNC_B_CNTL, state->crtc2.v_sync_b_cntl);

    OUTREG(AVIVO_D2CRTC_INTERLACE_CONTROL, state->crtc2.interlace_control);
    OUTREG(AVIVO_D2CRTC_STEREO_CONTROL, state->crtc2.stereo_control);

    OUTREG(AVIVO_D2CUR_CONTROL, state->crtc2.cursor_control);

    if (IS_DCE3_VARIANT) {
	/* DVOA regs */
	OUTREG(0x7080, state->dvoa[0]);
	OUTREG(0x7084, state->dvoa[1]);
	OUTREG(0x708c, state->dvoa[2]);
	OUTREG(0x7090, state->dvoa[3]);
	OUTREG(0x7094, state->dvoa[4]);
	OUTREG(0x70ac, state->dvoa[5]);
	OUTREG(0x70b0, state->dvoa[6]);

	j = 0;
	/* DAC regs */
	for (i = 0x7000; i <= 0x7040; i += 4) {
	    OUTREG(i, state->daca[j]);
	    OUTREG((i + 0x100), state->dacb[j]);
	    j++;
	}
	for (i = 0x7058; i <= 0x7060; i += 4) {
	    OUTREG(i, state->daca[j]);
	    OUTREG((i + 0x100), state->dacb[j]);
	    j++;
	}
	for (i = 0x7068; i <= 0x706c; i += 4) {
	    OUTREG(i, state->daca[j]);
	    OUTREG((i + 0x100), state->dacb[j]);
	    j++;
	}
	for (i = 0x7ef0; i <= 0x7ef8; i += 4) {
	    OUTREG(i, state->daca[j]);
	    OUTREG((i + 0x100), state->dacb[j]);
	    j++;
	}
	OUTREG(0x7050, state->daca[j]);
	OUTREG((0x7050 + 0x100), state->dacb[j]);

	j = 0;
	/* FMT regs */
	for (i = 0x6700; i <= 0x6744; i += 4) {
	    OUTREG(i, state->fmt1[j]);
	    OUTREG((i + 0x800), state->fmt2[j]);
	    j++;
	}

	j = 0;
	/* DIG regs */
	for (i = 0x75a0; i <= 0x75e0; i += 4) {
	    OUTREG(i, state->dig1[j]);
	    OUTREG((i + 0x400), state->dig2[j]);
	    j++;
	}
	for (i = 0x75e8; i <= 0x75ec; i += 4) {
	    OUTREG(i, state->dig1[j]);
	    OUTREG((i + 0x400), state->dig2[j]);
	    j++;
	}

	j = 0;
	/* HDMI regs */
	for (i = 0x7400; i <= 0x741c; i += 4) {
	    OUTREG(i, state->hdmi1[j]);
	    OUTREG((i + 0x400), state->hdmi2[j]);
	    j++;
	}
	for (i = 0x7430; i <= 0x74ec; i += 4) {
	    OUTREG(i, state->hdmi1[j]);
	    OUTREG((i + 0x400), state->hdmi2[j]);
	    j++;
	}
	OUTREG(0x7428, state->hdmi1[j]);
	OUTREG((0x7428 + 0x400), state->hdmi2[j]);

	j = 0;
	/* save AUX regs */
	for (i = 0x7780; i <= 0x77b4; i += 4) {
	    OUTREG(i, state->aux_cntl1[j]);
	    OUTREG((i + 0x040), state->aux_cntl2[j]);
	    OUTREG((i + 0x400), state->aux_cntl3[j]);
	    OUTREG((i + 0x440), state->aux_cntl4[j]);
	    if (IS_DCE32_VARIANT) {
		OUTREG((i + 0x500), state->aux_cntl5[j]);
		OUTREG((i + 0x540), state->aux_cntl6[j]);
	    }
	    j++;
	}

	j = 0;
	/* save UNIPHY regs */
	if (IS_DCE32_VARIANT) {
	    for (i = 0x7680; i <= 0x7690; i += 4) {
		OUTREG(i, state->uniphy1[j]);
		OUTREG((i + 0x20), state->uniphy2[j]);
		OUTREG((i + 0x400), state->uniphy3[j]);
		OUTREG((i + 0x420), state->uniphy4[j]);
		OUTREG((i + 0x840), state->uniphy5[j]);
		OUTREG((i + 0x940), state->uniphy6[j]);
		j++;
	    }
	    for (i = 0x7698; i <= 0x769c; i += 4) {
		OUTREG(i, state->uniphy1[j]);
		OUTREG((i + 0x20), state->uniphy2[j]);
		OUTREG((i + 0x400), state->uniphy3[j]);
		OUTREG((i + 0x420), state->uniphy4[j]);
		OUTREG((i + 0x840), state->uniphy5[j]);
		OUTREG((i + 0x940), state->uniphy6[j]);
		j++;
	    }
	} else {
	    for (i = 0x7ec0; i <= 0x7edc; i += 4) {
		OUTREG(i, state->uniphy1[j]);
		OUTREG((i + 0x100), state->uniphy2[j]);
		j++;
	    }
	}
	j = 0;
	/* save PHY,LINK regs */
	for (i = 0x7f20; i <= 0x7f34; i += 4) {
	    OUTREG(i, state->phy[j]);
	    j++;
	}
	for (i = 0x7f9c; i <= 0x7fa4; i += 4) {
	    OUTREG(i, state->phy[j]);
	    j++;
	}
	state->phy[j] = INREG(0x7f40);

	j = 0;
	/* save LVTMA regs */
	for (i = 0x7f00; i <= 0x7f1c; i += 4) {
	    OUTREG(i, state->lvtma[j]);
	    j++;
	}
	for (i = 0x7f80; i <= 0x7f98; i += 4) {
	    OUTREG(i, state->lvtma[j]);
	    j++;
	}
    } else {
	j = 0;
	/* DVOA regs */
	for (i = 0x7980; i <= 0x79bc; i += 4) {
	    OUTREG(i, state->dvoa[j]);
	    j++;
	}

	j = 0;
	/* DAC regs */ /* -- MIGHT NEED ORDERING FIX & DELAYS -- */
	for (i = 0x7800; i <= 0x782c; i += 4) {
	    OUTREG(i, state->daca[j]);
	    OUTREG((i + 0x200), state->dacb[j]);
	    j++;
	}
	for (i = 0x7834; i <= 0x7840; i += 4) {
	    OUTREG(i, state->daca[j]);
	    OUTREG((i + 0x200), state->dacb[j]);
	    j++;
	}
	for (i = 0x7850; i <= 0x7868; i += 4) {
	    OUTREG(i, state->daca[j]);
	    OUTREG((i + 0x200), state->dacb[j]);
	    j++;
	}

	j = 0;
	/* TMDSA regs */
	for (i = 0x7880; i <= 0x78e0; i += 4) {
	    OUTREG(i, state->tmdsa[j]);
	    j++;
	}
	for (i = 0x7904; i <= 0x7918; i += 4) {
	    OUTREG(i, state->tmdsa[j]);
	    j++;
	}

	j = 0;
	/* LVTMA regs */
	for (i = 0x7a80; i <= 0x7b18; i += 4) {
	    OUTREG(i, state->lvtma[j]);
	    j++;
	}

	/* DDIA regs */
	if ((info->ChipFamily == CHIP_FAMILY_RS600) ||
	    (info->ChipFamily == CHIP_FAMILY_RS690) ||
	    (info->ChipFamily == CHIP_FAMILY_RS740)) {
	    j = 0;
	    for (i = 0x7200; i <= 0x7290; i += 4) {
		OUTREG(i, state->ddia[j]);
		j++;
	    }
	}
    }

    /* scalers */
    j = 0;
    for (i = 0x6578; i <= 0x65e4; i += 4) {
	OUTREG(i, state->d1scl[j]);
	OUTREG((i + 0x800), state->d2scl[j]);
	j++;
    }
    for (i = 0x6600; i <= 0x662c; i += 4) {
	OUTREG(i, state->d1scl[j]);
	OUTREG((i + 0x800), state->d2scl[j]);
	j++;
    }
    j = 0;
    for (i = 0x66e8; i <= 0x66fc; i += 4) {
	OUTREG(i, state->dxscl[j]);
	j++;
    }
    OUTREG(0x6e30, state->dxscl[6]);
    OUTREG(0x6e34, state->dxscl[7]);

    /* Enable CRTCs */
    if (state->crtc1.control & 1) {
	    OUTREG(AVIVO_D1CRTC_CONTROL, 0x01000101);
	    INREG(AVIVO_D1CRTC_CONTROL);
	    OUTREG(AVIVO_D1CRTC_CONTROL, 0x00010101);
    }
    if (state->crtc2.control & 1) {
	    OUTREG(AVIVO_D2CRTC_CONTROL, 0x01000101);
	    INREG(AVIVO_D2CRTC_CONTROL);
	    OUTREG(AVIVO_D2CRTC_CONTROL, 0x00010101);
    }

    /* Where should that go ? */
    OUTREG(AVIVO_DC_CRTC_TV_CONTROL, state->crtc_tv_control);
    OUTREG(AVIVO_DC_LB_MEMORY_SPLIT, state->dc_lb_memory_split);

    /* Need fixing too ? */
    OUTREG(AVIVO_D1CRTC_BLANK_CONTROL, state->crtc1.blank_control);
    OUTREG(AVIVO_D2CRTC_BLANK_CONTROL, state->crtc2.blank_control);

    /* Dbl check */
    OUTREG(AVIVO_D1VGA_CONTROL, state->vga1_cntl);
    OUTREG(AVIVO_D2VGA_CONTROL, state->vga2_cntl);

    /* Should only enable outputs here */
}

static void avivo_restore_vga_regs(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    struct avivo_state *state = &restore->avivo;

    OUTREG(AVIVO_D1VGA_CONTROL, state->vga1_cntl);
    OUTREG(AVIVO_D2VGA_CONTROL, state->vga2_cntl);
}

static void
RADEONRestoreBIOSRegisters(ScrnInfoPtr pScrn, RADEONSavePtr restore)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (info->ChipFamily >= CHIP_FAMILY_R600) {
	OUTREG(R600_BIOS_0_SCRATCH, restore->bios_0_scratch);
	OUTREG(R600_BIOS_1_SCRATCH, restore->bios_1_scratch);
	OUTREG(R600_BIOS_2_SCRATCH, restore->bios_2_scratch);
	OUTREG(R600_BIOS_3_SCRATCH, restore->bios_3_scratch);
	OUTREG(R600_BIOS_4_SCRATCH, restore->bios_4_scratch);
	OUTREG(R600_BIOS_5_SCRATCH, restore->bios_5_scratch);
	OUTREG(R600_BIOS_6_SCRATCH, restore->bios_6_scratch);
	OUTREG(R600_BIOS_7_SCRATCH, restore->bios_7_scratch);
    } else {
	OUTREG(RADEON_BIOS_0_SCRATCH, restore->bios_0_scratch);
	OUTREG(RADEON_BIOS_1_SCRATCH, restore->bios_1_scratch);
	OUTREG(RADEON_BIOS_2_SCRATCH, restore->bios_2_scratch);
	OUTREG(RADEON_BIOS_3_SCRATCH, restore->bios_3_scratch);
	OUTREG(RADEON_BIOS_4_SCRATCH, restore->bios_4_scratch);
	OUTREG(RADEON_BIOS_5_SCRATCH, restore->bios_5_scratch);
	OUTREG(RADEON_BIOS_6_SCRATCH, restore->bios_6_scratch);
	OUTREG(RADEON_BIOS_7_SCRATCH, restore->bios_7_scratch);
    }
}

static void
RADEONSaveBIOSRegisters(ScrnInfoPtr pScrn, RADEONSavePtr save)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    if (info->ChipFamily >= CHIP_FAMILY_R600) {
	save->bios_0_scratch       = INREG(R600_BIOS_0_SCRATCH);
	save->bios_1_scratch       = INREG(R600_BIOS_1_SCRATCH);
	save->bios_2_scratch       = INREG(R600_BIOS_2_SCRATCH);
	save->bios_3_scratch       = INREG(R600_BIOS_3_SCRATCH);
	save->bios_4_scratch       = INREG(R600_BIOS_4_SCRATCH);
	save->bios_5_scratch       = INREG(R600_BIOS_5_SCRATCH);
	save->bios_6_scratch       = INREG(R600_BIOS_6_SCRATCH);
	save->bios_7_scratch       = INREG(R600_BIOS_7_SCRATCH);
    } else {
	save->bios_0_scratch       = INREG(RADEON_BIOS_0_SCRATCH);
	save->bios_1_scratch       = INREG(RADEON_BIOS_1_SCRATCH);
	save->bios_2_scratch       = INREG(RADEON_BIOS_2_SCRATCH);
	save->bios_3_scratch       = INREG(RADEON_BIOS_3_SCRATCH);
	save->bios_4_scratch       = INREG(RADEON_BIOS_4_SCRATCH);
	save->bios_5_scratch       = INREG(RADEON_BIOS_5_SCRATCH);
	save->bios_6_scratch       = INREG(RADEON_BIOS_6_SCRATCH);
	save->bios_7_scratch       = INREG(RADEON_BIOS_7_SCRATCH);
    }
}

/* Save everything needed to restore the original VC state */
static void RADEONSave(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt   = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONSavePtr  save       = info->SavedReg;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONSave\n");

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
# elif defined(__linux__)
	/* Save only mode * & fonts */	
	vgaHWSave(pScrn, &hwp->SavedReg, VGA_SR_MODE | VGA_SR_FONTS );
# else
	/* Save mode * & fonts & cmap */
	vgaHWSave(pScrn, &hwp->SavedReg, VGA_SR_ALL);
# endif
	vgaHWLock(hwp);
    }
#endif

    if (IS_AVIVO_VARIANT) {
	RADEONSaveMemMapRegisters(pScrn, save);
	avivo_save(pScrn, save);
    } else {
	save->dp_datatype      = INREG(RADEON_DP_DATATYPE);
	save->rbbm_soft_reset  = INREG(RADEON_RBBM_SOFT_RESET);
	save->clock_cntl_index = INREG(RADEON_CLOCK_CNTL_INDEX);
	RADEONPllErrataAfterIndex(info);

	RADEONSaveMemMapRegisters(pScrn, save);
	RADEONSaveCommonRegisters(pScrn, save);
	RADEONSavePLLRegisters(pScrn, save);
	RADEONSaveCrtcRegisters(pScrn, save);
	RADEONSaveFPRegisters(pScrn, save);
	RADEONSaveDACRegisters(pScrn, save);
	if (pRADEONEnt->HasCRTC2) {
	    RADEONSaveCrtc2Registers(pScrn, save);
	    RADEONSavePLL2Registers(pScrn, save);
	}
	if (info->InternalTVOut)
	    RADEONSaveTVRegisters(pScrn, save);
    }

    if (info->ChipFamily < CHIP_FAMILY_R600)
        RADEONSaveSurfaces(pScrn, save);

}

/* Restore the original (text) mode */
static void RADEONRestore(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    RADEONSavePtr  restore    = info->SavedReg;
    xf86CrtcConfigPtr   xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86CrtcPtr crtc;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONRestore\n");

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if (info->ChipFamily < CHIP_FAMILY_R600) {
	RADEONWaitForFifo(pScrn, 1);
	OUTREG(RADEON_RBBM_GUICNTL, RADEON_HOST_DATA_SWAP_NONE);
    }
#endif

    RADEONBlank(pScrn);

    if (IS_AVIVO_VARIANT) {
	RADEONRestoreMemMapRegisters(pScrn, restore);
	avivo_restore(pScrn, restore);
    } else {
	OUTREG(RADEON_CLOCK_CNTL_INDEX, restore->clock_cntl_index);
	RADEONPllErrataAfterIndex(info);
	OUTREG(RADEON_RBBM_SOFT_RESET,  restore->rbbm_soft_reset);
	OUTREG(RADEON_DP_DATATYPE,      restore->dp_datatype);
	OUTREG(RADEON_GRPH_BUFFER_CNTL, restore->grph_buffer_cntl);
	OUTREG(RADEON_GRPH2_BUFFER_CNTL, restore->grph2_buffer_cntl);

	if (!info->IsSecondary) {
	    RADEONRestoreMemMapRegisters(pScrn, restore);
	    RADEONRestoreCommonRegisters(pScrn, restore);

	    if (pRADEONEnt->HasCRTC2) {
		RADEONRestoreCrtc2Registers(pScrn, restore);
		RADEONRestorePLL2Registers(pScrn, restore);
	    }

	    RADEONRestoreCrtcRegisters(pScrn, restore);
	    RADEONRestorePLLRegisters(pScrn, restore);
	    RADEONRestoreRMXRegisters(pScrn, restore);
	    RADEONRestoreFPRegisters(pScrn, restore);
	    RADEONRestoreFP2Registers(pScrn, restore);
	    RADEONRestoreLVDSRegisters(pScrn, restore);

	    if (info->InternalTVOut)
		RADEONRestoreTVRegisters(pScrn, restore);
	}

	RADEONRestoreBIOSRegisters(pScrn, restore);
    }


#if 1
    /* Temp fix to "solve" VT switch problems.  When switching VTs on
     * some systems, the console can either hang or the fonts can be
     * corrupted.  This hack solves the problem 99% of the time.  A
     * correct fix is being worked on.
     */
    usleep(100000);
#endif

    if (info->ChipFamily < CHIP_FAMILY_R600)
	RADEONRestoreSurfaces(pScrn, restore);

    /* need to make sure we don't enable a crtc by accident or we may get a hang */
    if (pRADEONEnt->HasCRTC2 && !info->IsSecondary) {
	if (info->crtc2_on && xf86_config->num_crtc > 1) {
	    crtc = xf86_config->crtc[1];
	    crtc->funcs->dpms(crtc, DPMSModeOn);
	}
    }
    if (info->crtc_on) {
	crtc = xf86_config->crtc[0];
	crtc->funcs->dpms(crtc, DPMSModeOn);
    }

#ifdef WITH_VGAHW
    if (info->VGAAccess) {
       vgaHWPtr hwp = VGAHWPTR(pScrn);
       vgaHWUnlock(hwp);
# if defined(__powerpc__)
       /* Temporary hack to prevent crashing on PowerMacs when trying to
	* write VGA fonts, will find a better solution in the future
	*/
       vgaHWRestore(pScrn, &hwp->SavedReg, VGA_SR_MODE );
# elif defined(__linux__)
       vgaHWRestore(pScrn, &hwp->SavedReg, VGA_SR_MODE | VGA_SR_FONTS );
# else 
       vgaHWRestore(pScrn, &hwp->SavedReg, VGA_SR_ALL );
# endif
       vgaHWLock(hwp);
    }
#endif

    /* to restore console mode, DAC registers should be set after every other registers are set,
     * otherwise,we may get blank screen 
     */
    if (IS_AVIVO_VARIANT)
	avivo_restore_vga_regs(pScrn, restore);

    if (!IS_AVIVO_VARIANT)
	RADEONRestoreDACRegisters(pScrn, restore);

#if 0
    RADEONWaitForVerticalSync(pScrn);
#endif
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

static Bool RADEONSaveScreen(ScreenPtr pScreen, int mode)
{
    ScrnInfoPtr  pScrn = xf86Screens[pScreen->myNum];
    Bool         unblank;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONSaveScreen(%d)\n", mode);

    unblank = xf86IsUnblank(mode);
    if (unblank) SetTimeSinceLastInputEvent();

    if ((pScrn != NULL) && pScrn->vtSema) {
	if (unblank)
	    RADEONUnblank(pScrn);
	else
	    RADEONBlank(pScrn);
    }
    return TRUE;
}

Bool RADEONSwitchMode(int scrnIndex, DisplayModePtr mode, int flags)
{
    ScrnInfoPtr    pScrn       = xf86Screens[scrnIndex];
    RADEONInfoPtr  info        = RADEONPTR(pScrn);
    Bool           tilingOld   = info->tilingEnabled;
    Bool           ret;
#ifdef XF86DRI
    Bool           CPStarted   = info->cp->CPStarted;

    if (CPStarted) {
	DRILock(pScrn->pScreen, 0);
	RADEONCP_STOP(pScrn, info);
    }
#endif

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONSwitchMode() !n");

    if (info->allowColorTiling) {
        info->tilingEnabled = (mode->Flags & (V_DBLSCAN | V_INTERLACE)) ? FALSE : TRUE;
#ifdef XF86DRI	
	if (info->directRenderingEnabled && (info->tilingEnabled != tilingOld)) {
	    drm_radeon_sarea_t *pSAREAPriv;
	  if (RADEONDRISetParam(pScrn, RADEON_SETPARAM_SWITCH_TILING, (info->tilingEnabled ? 1 : 0)) < 0)
  	      xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			 "[drm] failed changing tiling status\n");
	    pSAREAPriv = DRIGetSAREAPrivate(pScrn->pScreen);
	    info->tilingEnabled = pSAREAPriv->tiling_enabled ? TRUE : FALSE;
	}
#endif
    }

    if (info->accelOn)
        RADEON_SYNC(info, pScrn);

    ret = xf86SetSingleMode (pScrn, mode, RR_Rotate_0);

    if (info->tilingEnabled != tilingOld) {
	/* need to redraw front buffer, I guess this can be considered a hack ? */
	xf86EnableDisableFBAccess(scrnIndex, FALSE);
	RADEONChangeSurfaces(pScrn);
	xf86EnableDisableFBAccess(scrnIndex, TRUE);
	/* xf86SetRootClip would do, but can't access that here */
    }

    if (info->accelOn) {
        RADEON_SYNC(info, pScrn);
	if (info->ChipFamily < CHIP_FAMILY_R600)
	    RADEONEngineRestore(pScrn);
    }

#ifdef XF86DRI
    if (CPStarted) {
	RADEONCP_START(pScrn, info);
	DRIUnlock(pScrn->pScreen);
    }
#endif

    /* reset ecp for overlay */
    info->ecp_div = -1;

    return ret;
}

#ifdef X_XF86MiscPassMessage
Bool RADEONHandleMessage(int scrnIndex, const char* msgtype,
                                   const char* msgval, char** retmsg)
{
    ErrorF("RADEONHandleMessage(%d, \"%s\", \"%s\", retmsg)\n", scrnIndex,
		    msgtype, msgval);
    *retmsg = "";
    return 0;
}
#endif

#ifndef HAVE_XF86MODEBANDWIDTH
/** Calculates the memory bandwidth (in MiB/sec) of a mode. */
_X_HIDDEN unsigned int
xf86ModeBandwidth(DisplayModePtr mode, int depth)
{
    float a_active, a_total, active_percent, pixels_per_second;
    int bytes_per_pixel = (depth + 7) / 8;

    if (!mode->HTotal || !mode->VTotal || !mode->Clock)
	return 0;

    a_active = mode->HDisplay * mode->VDisplay;
    a_total = mode->HTotal * mode->VTotal;
    active_percent = a_active / a_total;
    pixels_per_second = active_percent * mode->Clock * 1000.0;

    return (unsigned int)(pixels_per_second * bytes_per_pixel / (1024 * 1024));
}
#endif

/* Used to disallow modes that are not supported by the hardware */
ModeStatus RADEONValidMode(int scrnIndex, DisplayModePtr mode,
                                     Bool verbose, int flag)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);

    /*
     * RN50 has effective maximum mode bandwidth of about 300MiB/s.
     * XXX should really do this for all chips by properly computing
     * memory bandwidth and an overhead factor.
     */
    if (info->ChipFamily == CHIP_FAMILY_RV100 && !pRADEONEnt->HasCRTC2) {
	if (xf86ModeBandwidth(mode, pScrn->bitsPerPixel) > 300)
	    return MODE_BANDWIDTH;
    }

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
void RADEONDoAdjustFrame(ScrnInfoPtr pScrn, int x, int y, Bool crtc2)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    int            Base, reg, regcntl, crtcoffsetcntl, xytilereg, crtcxytile = 0;
#ifdef XF86DRI
    drm_radeon_sarea_t *pSAREAPriv;
    XF86DRISAREAPtr pSAREA;
#endif

#if 0 /* Verbose */
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONDoAdjustFrame(%d,%d,%d)\n", x, y, clone);
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
    if (crtc2) {
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
    if (!info->dri->have3DWindows)
#endif
    crtcoffsetcntl &= ~RADEON_CRTC_OFFSET_FLIP_CNTL;
#endif
    if (info->tilingEnabled) {
        if (IS_R300_VARIANT || IS_AVIVO_VARIANT) {
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

	if (crtc2) {
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
	    Base += info->dri->backOffset - info->dri->frontOffset;
	}
    }
#endif

    if (IS_R300_VARIANT || IS_AVIVO_VARIANT) {
	OUTREG(xytilereg, crtcxytile);
    } else {
	OUTREG(regcntl, crtcoffsetcntl);
    }

    OUTREG(reg, Base);
}

void RADEONAdjustFrame(int scrnIndex, int x, int y, int flags)
{
    ScrnInfoPtr    pScrn      = xf86Screens[scrnIndex];
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    xf86CrtcConfigPtr	config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86OutputPtr  output = config->output[config->compat_output];
    xf86CrtcPtr	crtc = output->crtc;

    /* not handled */
    if (IS_AVIVO_VARIANT)
	return;

#ifdef XF86DRI
    if (info->cp->CPStarted && pScrn->pScreen) DRILock(pScrn->pScreen, 0);
#endif

    if (info->accelOn)
        RADEON_SYNC(info, pScrn);

    if (crtc && crtc->enabled) {
	if (crtc == pRADEONEnt->pCrtc[0])
	    RADEONDoAdjustFrame(pScrn, crtc->desiredX + x, crtc->desiredY + y, FALSE);
	else
	    RADEONDoAdjustFrame(pScrn, crtc->desiredX + x, crtc->desiredY + y, TRUE);
	crtc->x = output->initial_x + x;
	crtc->y = output->initial_y + y;
    }


#ifdef XF86DRI
	if (info->cp->CPStarted && pScrn->pScreen) DRIUnlock(pScrn->pScreen);
#endif
}

/* Called when VT switching back to the X server.  Reinitialize the
 * video mode.
 */
Bool RADEONEnterVT(int scrnIndex, int flags)
{
    ScrnInfoPtr    pScrn = xf86Screens[scrnIndex];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONEnterVT\n");

    if (!radeon_card_posted(pScrn)) { /* Softboot V_BIOS */
	if (info->IsAtomBios) {
	    rhdAtomASICInit(info->atomBIOS);
	} else {
	    xf86Int10InfoPtr pInt;

	    pInt = xf86InitInt10 (info->pEnt->index);
	    if (pInt) {
		pInt->num = 0xe6;
		xf86ExecX86int10 (pInt);
		xf86FreeInt10 (pInt);
	    } else {
		RADEONGetBIOSInitTableOffsets(pScrn);
		RADEONPostCardFromBIOSTables(pScrn);
	    }
	}
    }

    /* Makes sure the engine is idle before doing anything */
    RADEONWaitForIdleMMIO(pScrn);

    if (info->IsMobility && !IS_AVIVO_VARIANT) {
	if (xf86ReturnOptValBool(info->Options, OPTION_DYNAMIC_CLOCKS, FALSE)) {
	    RADEONSetDynamicClock(pScrn, 1);
	} else {
	    RADEONSetDynamicClock(pScrn, 0);
	}
    } else if (IS_AVIVO_VARIANT) {
	if (xf86ReturnOptValBool(info->Options, OPTION_DYNAMIC_CLOCKS, FALSE)) {
	    atombios_static_pwrmgt_setup(pScrn, 1);
	    atombios_dyn_clk_setup(pScrn, 1);
	}
    }

    if (IS_R300_VARIANT || IS_RV100_VARIANT)
	RADEONForceSomeClocks(pScrn);

    for (i = 0; i < config->num_crtc; i++)
	radeon_crtc_modeset_ioctl(config->crtc[i], TRUE);

    pScrn->vtSema = TRUE;

    if (!xf86SetDesiredModes(pScrn))
	return FALSE;

    if (info->ChipFamily < CHIP_FAMILY_R600)
        RADEONRestoreSurfaces(pScrn, info->ModeReg);
#ifdef XF86DRI
    if (info->directRenderingEnabled) {
    	if (info->cardType == CARD_PCIE &&
	    info->dri->pKernelDRMVersion->version_minor >= 19 &&
	    info->FbSecureSize) {
#if X_BYTE_ORDER == X_BIG_ENDIAN
	    unsigned char *RADEONMMIO = info->MMIO;
	    unsigned int sctrl = INREG(RADEON_SURFACE_CNTL);

	    /* we need to backup the PCIE GART TABLE from fb memory */
	    OUTREG(RADEON_SURFACE_CNTL, 0);
#endif
	    memcpy(info->FB + info->dri->pciGartOffset, info->dri->pciGartBackup, info->dri->pciGartSize);
#if X_BYTE_ORDER == X_BIG_ENDIAN
	    OUTREG(RADEON_SURFACE_CNTL, sctrl);
#endif
    	}

	/* get the DRI back into shape after resume */
	RADEONDRISetVBlankInterrupt (pScrn, TRUE);
	RADEONDRIResume(pScrn->pScreen);
	RADEONAdjustMemMapRegisters(pScrn, info->ModeReg);

    }
#endif
    /* this will get XVideo going again, but only if XVideo was initialised
       during server startup (hence the info->adaptor if). */
    if (info->adaptor)
	RADEONResetVideo(pScrn);

    if (info->accelOn && (info->ChipFamily < CHIP_FAMILY_R600))
	RADEONEngineRestore(pScrn);

    if (info->accelOn && info->accel_state)
	info->accel_state->XInited3D = FALSE;

#ifdef XF86DRI
    if (info->directRenderingEnabled) {
        if (info->ChipFamily >= CHIP_FAMILY_R600)
		R600LoadShaders(pScrn);
	RADEONCP_START(pScrn, info);
	DRIUnlock(pScrn->pScreen);
    }
#endif

    return TRUE;
}

/* Called when VT switching away from the X server.  Restore the
 * original text mode.
 */
void RADEONLeaveVT(int scrnIndex, int flags)
{
    ScrnInfoPtr    pScrn = xf86Screens[scrnIndex];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONLeaveVT\n");
#ifdef XF86DRI
    if (RADEONPTR(pScrn)->directRenderingInited) {

	RADEONDRISetVBlankInterrupt (pScrn, FALSE);
	DRILock(pScrn->pScreen, 0);
	RADEONCP_STOP(pScrn, info);

        if (info->cardType == CARD_PCIE &&
	    info->dri->pKernelDRMVersion->version_minor >= 19 &&
	    info->FbSecureSize) {
#if X_BYTE_ORDER == X_BIG_ENDIAN
	    unsigned char *RADEONMMIO = info->MMIO;
	    unsigned int sctrl = INREG(RADEON_SURFACE_CNTL);

            /* we need to backup the PCIE GART TABLE from fb memory */
	    OUTREG(RADEON_SURFACE_CNTL, 0);
#endif
            memcpy(info->dri->pciGartBackup, (info->FB + info->dri->pciGartOffset), info->dri->pciGartSize);
#if X_BYTE_ORDER == X_BIG_ENDIAN
	    OUTREG(RADEON_SURFACE_CNTL, sctrl);
#endif
        }

	/* Make sure 3D clients will re-upload textures to video RAM */
	if (info->dri->textureSize) {
	    drm_radeon_sarea_t *pSAREAPriv =
		(drm_radeon_sarea_t*)DRIGetSAREAPrivate(pScrn->pScreen);
	    struct drm_tex_region *list = pSAREAPriv->tex_list[0];
	    int age = ++pSAREAPriv->tex_age[0];

	    i = 0;

	    do {
		list[i].age = age;
		i = list[i].next;
	    } while (i != 0);
	}
    }
#endif


    for (i = 0; i < config->num_crtc; i++) {
	xf86CrtcPtr crtc = config->crtc[i];
	RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;

	radeon_crtc->initialized = FALSE;

#ifndef HAVE_FREE_SHADOW
	if (crtc->rotatedPixmap || crtc->rotatedData) {
	    crtc->funcs->shadow_destroy(crtc, crtc->rotatedPixmap,
					crtc->rotatedData);
	    crtc->rotatedPixmap = NULL;
	    crtc->rotatedData = NULL;
	}
#endif
    }

#ifdef HAVE_FREE_SHADOW
    xf86RotateFreeShadow(pScrn);
#endif

    xf86_hide_cursors (pScrn);

    RADEONRestore(pScrn);

    for (i = 0; i < config->num_crtc; i++)
	radeon_crtc_modeset_ioctl(config->crtc[i], FALSE);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Ok, leaving now...\n");
}

/* Called at the end of each server generation.  Restore the original
 * text mode, unmap video memory, and unwrap and call the saved
 * CloseScreen function.
 */
static Bool RADEONCloseScreen(int scrnIndex, ScreenPtr pScreen)
{
    ScrnInfoPtr    pScrn = xf86Screens[scrnIndex];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONCloseScreen\n");

    /* Mark acceleration as stopped or we might try to access the engine at
     * wrong times, especially if we had DRI, after DRI has been stopped
     */
    info->accelOn = FALSE;

    for (i = 0; i < config->num_crtc; i++) {
	xf86CrtcPtr crtc = config->crtc[i];
	RADEONCrtcPrivatePtr radeon_crtc = crtc->driver_private;

	radeon_crtc->initialized = FALSE;
    }

#ifdef XF86DRI
#ifdef DAMAGE
    if (info->dri && info->dri->pDamage) {
	PixmapPtr pPix = pScreen->GetScreenPixmap(pScreen);

	DamageUnregister(&pPix->drawable, info->dri->pDamage);
	DamageDestroy(info->dri->pDamage);
	info->dri->pDamage = NULL;
    }
#endif

    RADEONDRIStop(pScreen);
#endif

#ifdef USE_XAA
    if(!info->useEXA && info->accel_state->RenderTex) {
        xf86FreeOffscreenLinear(info->accel_state->RenderTex);
        info->accel_state->RenderTex = NULL;
    }
#endif /* USE_XAA */

    if (pScrn->vtSema) {
	RADEONRestore(pScrn);
    }

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Disposing accel...\n");
#ifdef USE_EXA
    if (info->accel_state->exa) {
	exaDriverFini(pScreen);
	xfree(info->accel_state->exa);
	info->accel_state->exa = NULL;
    }
#endif /* USE_EXA */
#ifdef USE_XAA
    if (!info->useEXA) {
	if (info->accel_state->accel)
		XAADestroyInfoRec(info->accel_state->accel);
	info->accel_state->accel = NULL;

	if (info->accel_state->scratch_save)
	    xfree(info->accel_state->scratch_save);
	info->accel_state->scratch_save = NULL;
    }
#endif /* USE_XAA */

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Disposing cursor info\n");
    if (info->cursor) xf86DestroyCursorInfoRec(info->cursor);
    info->cursor = NULL;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Disposing DGA\n");
    if (info->DGAModes) xfree(info->DGAModes);
    info->DGAModes = NULL;
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "Unmapping memory\n");
    RADEONUnmapMem(pScrn);

    pScrn->vtSema = FALSE;

    xf86ClearPrimInitDone(info->pEnt->index);

    pScreen->BlockHandler = info->BlockHandler;
    pScreen->CloseScreen = info->CloseScreen;
    return (*pScreen->CloseScreen)(scrnIndex, pScreen);
}

void RADEONFreeScreen(int scrnIndex, int flags)
{
    ScrnInfoPtr  pScrn = xf86Screens[scrnIndex];
    RADEONInfoPtr  info  = RADEONPTR(pScrn);
    
    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, RADEON_LOGLEVEL_DEBUG,
		   "RADEONFreeScreen\n");

    /* when server quits at PreInit, we don't need do this anymore*/
    if (!info) return;

#ifdef WITH_VGAHW
    if (info->VGAAccess && xf86LoaderCheckSymbol("vgaHWFreeHWRec"))
	vgaHWFreeHWRec(pScrn);
#endif
    RADEONFreeRec(pScrn);
}

static void RADEONForceSomeClocks(ScrnInfoPtr pScrn)
{
    /* It appears from r300 and rv100 may need some clocks forced-on */
     uint32_t tmp;

     tmp = INPLL(pScrn, RADEON_SCLK_CNTL);
     tmp |= RADEON_SCLK_FORCE_CP | RADEON_SCLK_FORCE_VIP;
     OUTPLL(pScrn, RADEON_SCLK_CNTL, tmp);
}

static void RADEONSetDynamicClock(ScrnInfoPtr pScrn, int mode)
{
    RADEONInfoPtr  info       = RADEONPTR(pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;
    uint32_t tmp;
    switch(mode) {
        case 0: /* Turn everything OFF (ForceON to everything)*/
            if ( !pRADEONEnt->HasCRTC2 ) {
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

		if ( !pRADEONEnt->HasCRTC2 ) {
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
            if (!pRADEONEnt->HasCRTC2) {
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
