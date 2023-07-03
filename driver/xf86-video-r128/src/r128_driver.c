/*
 * Copyright 1999, 2000 ATI Technologies Inc., Markham, Ontario,
 *                      Precision Insight, Inc., Cedar Park, Texas, and
 *                      VA Linux Systems Inc., Fremont, California.
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
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, PRECISION INSIGHT, VA LINUX
 * SYSTEMS AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * Authors:
 *   Rickard E. Faith <faith@valinux.com>
 *   Kevin E. Martin <martin@valinux.com>
 *   Gareth Hughes <gareth@valinux.com>
 *
 * Credits:
 *
 *   Thanks to Alan Hourihane <alanh@fairlite.demon..co.uk> and SuSE for
 *   providing source code to their 3.3.x Rage 128 driver.  Portions of
 *   this file are based on the initialization code for that driver.
 *
 * References:
 *
 *   RAGE 128 VR/ RAGE 128 GL Register Reference Manual (Technical
 *   Reference Manual P/N RRG-G04100-C Rev. 0.04), ATI Technologies: April
 *   1999.
 *
 *   RAGE 128 Software Development Manual (Technical Reference Manual P/N
 *   SDK-G04000 Rev. 0.01), ATI Technologies: June 1999.
 *
 * This server does not yet support these XFree86 4.0 features:
 *   DDC1 & DDC2
 *   shadowfb
 *   overlay planes
 *
 * Modified by Marc Aurele La France <tsi@xfree86.org> for ATI driver merge.
 *
 * Dualhead support - Alex Deucher <agd5f@yahoo.com>
 */

#include <string.h>
#include <stdio.h>

				/* Driver data structures */
#include "r128.h"
#include "r128_probe.h"
#include "r128_reg.h"
#include "r128_version.h"

#ifdef R128DRI
#define _XF86DRI_SERVER_
#include "r128_dri.h"
#include "r128_common.h"
#include "r128_sarea.h"
#endif

				/* colormap initialization */
#include "micmap.h"

				/* X and server generic header files */
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86RandR12.h"
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86RAC.h"
#include "xf86Resources.h"
#endif
#include "xf86cmap.h"
#include "xf86xv.h"
#include "vbe.h"

				/* fbdevhw & vgahw */
#ifdef WITH_VGAHW
#include "vgaHW.h"
#endif
#include "fbdevhw.h"
#include "dixstruct.h"

				/* DPMS support. */
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif


static Bool R128CloseScreen(CLOSE_SCREEN_ARGS_DECL);
static Bool R128SaveScreen(ScreenPtr pScreen, int mode);
static void R128Save(ScrnInfoPtr pScrn);
static void R128Restore(ScrnInfoPtr pScrn);

typedef enum {
  OPTION_NOACCEL,
  OPTION_FBDEV,
  OPTION_DAC_6BIT,
  OPTION_VGA_ACCESS,
  OPTION_SHOW_CACHE,
  OPTION_SW_CURSOR,
  OPTION_VIDEO_KEY,
  OPTION_PANEL_WIDTH,
  OPTION_PANEL_HEIGHT,
  OPTION_PROG_FP_REGS,
#ifdef R128DRI
  OPTION_XV_DMA,
  OPTION_IS_PCI,
  OPTION_CCE_PIO,
  OPTION_NO_SECURITY,
  OPTION_USEC_TIMEOUT,
  OPTION_AGP_MODE,
  OPTION_AGP_SIZE,
  OPTION_RING_SIZE,
  OPTION_BUFFER_SIZE,
  OPTION_PAGE_FLIP,
#endif
  OPTION_ACCELMETHOD,
  OPTION_RENDERACCEL
} R128Opts;

static const OptionInfoRec R128Options[] = {
{ OPTION_NOACCEL,      "NoAccel",          OPTV_BOOLEAN, {0}, FALSE },
{ OPTION_FBDEV,        "UseFBDev",         OPTV_BOOLEAN, {0}, FALSE },
{ OPTION_DAC_6BIT,     "Dac6Bit",          OPTV_BOOLEAN, {0}, FALSE },
{ OPTION_VGA_ACCESS,   "VGAAccess",        OPTV_BOOLEAN, {0}, TRUE  },
{ OPTION_SHOW_CACHE,   "ShowCache",        OPTV_BOOLEAN, {0}, FALSE },
{ OPTION_SW_CURSOR,    "SWcursor",         OPTV_BOOLEAN, {0}, FALSE },
{ OPTION_VIDEO_KEY,    "VideoKey",         OPTV_INTEGER, {0}, FALSE },
{ OPTION_PANEL_WIDTH,  "PanelWidth",       OPTV_INTEGER, {0}, FALSE },
{ OPTION_PANEL_HEIGHT, "PanelHeight",      OPTV_INTEGER, {0}, FALSE },
{ OPTION_PROG_FP_REGS, "ProgramFPRegs",    OPTV_BOOLEAN, {0}, FALSE },
#ifdef R128DRI
  { OPTION_XV_DMA,       "DMAForXv",         OPTV_BOOLEAN, {0}, FALSE },
  { OPTION_IS_PCI,       "ForcePCIMode",     OPTV_BOOLEAN, {0}, FALSE },
  { OPTION_CCE_PIO,      "CCEPIOMode",       OPTV_BOOLEAN, {0}, FALSE },
  { OPTION_NO_SECURITY,  "CCENoSecurity",    OPTV_BOOLEAN, {0}, FALSE },
  { OPTION_USEC_TIMEOUT, "CCEusecTimeout",   OPTV_INTEGER, {0}, FALSE },
  { OPTION_AGP_MODE,     "AGPMode",          OPTV_INTEGER, {0}, FALSE },
  { OPTION_AGP_SIZE,     "AGPSize",          OPTV_INTEGER, {0}, FALSE },
  { OPTION_RING_SIZE,    "RingSize",         OPTV_INTEGER, {0}, FALSE },
  { OPTION_BUFFER_SIZE,  "BufferSize",       OPTV_INTEGER, {0}, FALSE },
  { OPTION_PAGE_FLIP,    "EnablePageFlip",   OPTV_BOOLEAN, {0}, FALSE },
#endif
  { OPTION_ACCELMETHOD,  "AccelMethod",      OPTV_STRING,  {0}, FALSE },
  { OPTION_RENDERACCEL,  "RenderAccel",      OPTV_BOOLEAN, {0}, FALSE },
  { -1,                  NULL,               OPTV_NONE,    {0}, FALSE }
};

const OptionInfoRec *R128OptionsWeak(void) { return R128Options; }

R128RAMRec R128RAM[] = {        /* Memory Specifications
				   From RAGE 128 Software Development
				   Manual (Technical Reference Manual P/N
				   SDK-G04000 Rev 0.01), page 3-21.  */
    { 4, 4, 3, 3, 1, 3, 1, 16, 12, "128-bit SDR SGRAM 1:1" },
    { 4, 8, 3, 3, 1, 3, 1, 17, 13, "64-bit SDR SGRAM 1:1" },
    { 4, 4, 1, 2, 1, 2, 1, 16, 12, "64-bit SDR SGRAM 2:1" },
    { 4, 4, 3, 3, 2, 3, 1, 16, 12, "64-bit DDR SGRAM" },
};

extern _X_EXPORT int gR128EntityIndex;

int getR128EntityIndex(void)
{
    return gR128EntityIndex;
}

R128EntPtr R128EntPriv(ScrnInfoPtr pScrn)
{
    DevUnion     *pPriv;
    R128InfoPtr  info   = R128PTR(pScrn);
    pPriv = xf86GetEntityPrivate(info->pEnt->index,
                                 getR128EntityIndex());
    return pPriv->ptr;
}

/* Allocate our private R128InfoRec. */
static Bool R128GetRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate) return TRUE;

    pScrn->driverPrivate = xnfcalloc(sizeof(R128InfoRec), 1);
    return TRUE;
}

/* Free our private R128InfoRec. */
static void R128FreeRec(ScrnInfoPtr pScrn)
{
    if (!pScrn || !pScrn->driverPrivate) return;
    free(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
}

/* Memory map the MMIO region.  Used during pre-init and by R128MapMem,
   below. */
static Bool R128MapMMIO(ScrnInfoPtr pScrn)
{
    R128InfoPtr info          = R128PTR(pScrn);

    if (info->FBDev) {
	info->MMIO = fbdevHWMapMMIO(pScrn);
    } else {
#ifndef XSERVER_LIBPCIACCESS
	info->MMIO = xf86MapPciMem(pScrn->scrnIndex,
				   VIDMEM_MMIO | VIDMEM_READSIDEEFFECT,
				   info->PciTag,
				   info->MMIOAddr,
				   R128_MMIOSIZE);
        if (!info->MMIO) return FALSE;
#else
	int err = pci_device_map_range(info->PciInfo,
				       info->MMIOAddr,
				       R128_MMIOSIZE,
				       PCI_DEV_MAP_FLAG_WRITABLE,
				       &info->MMIO);

	if (err) {
	    xf86DrvMsg (pScrn->scrnIndex, X_ERROR,
                        "Unable to map MMIO aperture. %s (%d)\n",
                        strerror (err), err);
	    return FALSE;
	}
#endif
    }

    return TRUE;
}

/* Unmap the MMIO region.  Used during pre-init and by R128UnmapMem,
   below. */
static Bool R128UnmapMMIO(ScrnInfoPtr pScrn)
{
    R128InfoPtr info          = R128PTR(pScrn);

    if (info->FBDev)
	fbdevHWUnmapMMIO(pScrn);
    else {
#ifndef XSERVER_LIBPCIACCESS
	xf86UnMapVidMem(pScrn->scrnIndex, info->MMIO, R128_MMIOSIZE);
#else
	pci_device_unmap_range(info->PciInfo, info->MMIO, R128_MMIOSIZE);
#endif
    }
    info->MMIO = NULL;
    return TRUE;
}

/* Memory map the frame buffer.  Used by R128MapMem, below. */
static Bool R128MapFB(ScrnInfoPtr pScrn)
{
    R128InfoPtr info          = R128PTR(pScrn);

    if (info->FBDev) {
	info->FB = fbdevHWMapVidmem(pScrn);
    } else {
#ifndef XSERVER_LIBPCIACCESS
	info->FB = xf86MapPciMem(pScrn->scrnIndex,
				 VIDMEM_FRAMEBUFFER,
				 info->PciTag,
				 info->LinearAddr,
				 info->FbMapSize);
#else
	int err = pci_device_map_range(info->PciInfo,
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
    }

    if (!info->FB) return FALSE;
    return TRUE;
}

/* Unmap the frame buffer.  Used by R128UnmapMem, below. */
static Bool R128UnmapFB(ScrnInfoPtr pScrn)
{
    R128InfoPtr info          = R128PTR(pScrn);

    if (info->FBDev)
	fbdevHWUnmapVidmem(pScrn);
    else
#ifndef XSERVER_LIBPCIACCESS
	xf86UnMapVidMem(pScrn->scrnIndex, info->FB, info->FbMapSize);
#else
	pci_device_unmap_range(info->PciInfo, info->FB, info->FbMapSize);
#endif
    info->FB = NULL;
    return TRUE;
}

/* Memory map the MMIO region and the frame buffer. */
static Bool R128MapMem(ScrnInfoPtr pScrn)
{
    if (!R128MapMMIO(pScrn)) return FALSE;
    if (!R128MapFB(pScrn)) {
	R128UnmapMMIO(pScrn);
	return FALSE;
    }
    return TRUE;
}

/* Unmap the MMIO region and the frame buffer. */
static Bool R128UnmapMem(ScrnInfoPtr pScrn)
{
    if (!R128UnmapMMIO(pScrn) || !R128UnmapFB(pScrn)) return FALSE;
    return TRUE;
}

/* Read PLL information */
unsigned R128INPLL(ScrnInfoPtr pScrn, int addr)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    OUTREG8(R128_CLOCK_CNTL_INDEX, addr & 0x3f);
    return INREG(R128_CLOCK_CNTL_DATA);
}

#if 0
/* Read PAL information (only used for debugging). */
static int R128INPAL(int idx)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    OUTREG(R128_PALETTE_INDEX, idx << 16);
    return INREG(R128_PALETTE_DATA);
}
#endif

/* Wait for vertical sync. */
void R128WaitForVerticalSync(ScrnInfoPtr pScrn)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    int           i;

    OUTREG(R128_GEN_INT_STATUS, R128_VSYNC_INT_AK);
    for (i = 0; i < R128_TIMEOUT; i++) {
	if (INREG(R128_GEN_INT_STATUS) & R128_VSYNC_INT) break;
    }
}

/* Compute log base 2 of val. */
int R128MinBits(int val)
{
    int bits;

    if (!val) return 1;
    for (bits = 0; val; val >>= 1, ++bits);
    return bits;
}

/* Finds the first output using a given crtc. */
xf86OutputPtr R128FirstOutput(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    xf86OutputPtr output = xf86_config->output[0];
    int o;

    for (o = 0; o < xf86_config->num_output; o++) {
        output = xf86_config->output[o];
        if (output->crtc == crtc) break;
    }

    return output;
}

/* Read the Video BIOS block. */
static Bool R128GetBIOSParameters(ScrnInfoPtr pScrn, xf86Int10InfoPtr pInt10)
{
    R128InfoPtr info = R128PTR(pScrn);

#ifdef XSERVER_LIBPCIACCESS
    int size = info->PciInfo->rom_size > R128_VBIOS_SIZE ? info->PciInfo->rom_size : R128_VBIOS_SIZE;
    info->VBIOS = malloc(size);
#else
    info->VBIOS = malloc(R128_VBIOS_SIZE);
#endif

    if (!info->VBIOS) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Cannot allocate space for hold Video BIOS!\n");
	return FALSE;
    }

    if (pInt10) {
	info->BIOSAddr = pInt10->BIOSseg << 4;
	(void)memcpy(info->VBIOS, xf86int10Addr(pInt10, info->BIOSAddr),
		     R128_VBIOS_SIZE);
    } else {
#ifdef XSERVER_LIBPCIACCESS
	if (pci_device_read_rom(info->PciInfo, info->VBIOS)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Failed to read PCI ROM!\n");
	}
#else
	xf86ReadPciBIOS(0, info->PciTag, 0, info->VBIOS, R128_VBIOS_SIZE);
	if (info->VBIOS[0] != 0x55 || info->VBIOS[1] != 0xaa) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Video BIOS not detected in PCI space!\n");
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Attempting to read Video BIOS from legacy ISA space!\n");
	    info->BIOSAddr = 0x000c0000;
	    xf86ReadDomainMemory(info->PciTag, info->BIOSAddr, R128_VBIOS_SIZE, info->VBIOS);
	}
#endif
    }
    if (info->VBIOS[0] != 0x55 || info->VBIOS[1] != 0xaa) {
	info->BIOSAddr = 0x00000000;
	free(info->VBIOS);
	info->VBIOS = NULL;
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Video BIOS not found!\n");
    }

    return TRUE;
}

/* Read the FP parameters if an LVDS panel is expected. */
void R128GetPanelInfoFromBIOS(xf86OutputPtr output)
{
    ScrnInfoPtr pScrn = output->scrn;
    R128InfoPtr info  = R128PTR(pScrn);
    R128OutputPrivatePtr r128_output = output->driver_private;
    int FPHeader = 0;
    int i;

    r128_output->PanelPwrDly = 200;
    xf86GetOptValInteger(info->Options, OPTION_PANEL_WIDTH,  &(r128_output->PanelXRes));
    xf86GetOptValInteger(info->Options, OPTION_PANEL_HEIGHT, &(r128_output->PanelYRes));

    if (!info->VBIOS) return;
    info->FPBIOSstart = 0;

    /* FIXME: There should be direct access to the start of the FP info
     * tables, but until we find out where that offset is stored, we
     * must search for the ATI signature string: "M3      ".
     */
    for (i = 4; i < R128_VBIOS_SIZE - 8; i++) {
        if (R128_BIOS8(i)     == 'M' &&
            R128_BIOS8(i + 1) == '3' &&
            R128_BIOS8(i + 2) == ' ' &&
            R128_BIOS8(i + 3) == ' ' &&
            R128_BIOS8(i + 4) == ' ' &&
            R128_BIOS8(i + 5) == ' ' &&
            R128_BIOS8(i + 6) == ' ' &&
            R128_BIOS8(i + 7) == ' ') {
            FPHeader = i - 2;
            break;
        }
    }

    if (!FPHeader) return;

    /* Assume that only one panel is attached and supported */
    for (i = FPHeader + 20; i < FPHeader + 84; i += 2) {
        if (R128_BIOS16(i) != 0) {
            info->FPBIOSstart = R128_BIOS16(i);
            break;
        }
    }

    if (!info->FPBIOSstart) return;

    if (!r128_output->PanelXRes)
        r128_output->PanelXRes = R128_BIOS16(info->FPBIOSstart + 25);
    if (!r128_output->PanelYRes)
        r128_output->PanelYRes = R128_BIOS16(info->FPBIOSstart + 27);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel size: %dx%d\n",
               r128_output->PanelXRes, r128_output->PanelYRes);

    r128_output->PanelPwrDly = R128_BIOS8(info->FPBIOSstart + 56);

    if (!r128_output->PanelXRes || !r128_output->PanelYRes) {
        info->HasPanelRegs = FALSE;
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Can't determine panel dimensions, and none specified.\n"
		   "\tDisabling programming of FP registers.\n");
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel ID: ");
    for (i = 1; i <= 24; i++)
        ErrorF("%c", R128_BIOS8(info->FPBIOSstart + i));

    ErrorF("\n");

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel Type: ");
    i = R128_BIOS16(info->FPBIOSstart + 29);
    if (i & 1) ErrorF("Color, ");
    else       ErrorF("Monochrome, ");
    if (i & 2) ErrorF("Dual(split), ");
    else       ErrorF("Single, ");

    switch ((i >> 2) & 0x3f) {
    case 0:  ErrorF("STN");        break;
    case 1:  ErrorF("TFT");        break;
    case 2:  ErrorF("Active STN"); break;
    case 3:  ErrorF("EL");         break;
    case 4:  ErrorF("Plasma");     break;
    default: ErrorF("UNKNOWN");    break;
    }

    ErrorF("\n");

    if (R128_BIOS8(info->FPBIOSstart + 61) & 1) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Panel Interface: LVDS\n");
    } else {
        /* FIXME: Add Non-LVDS flat pael support */
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "Non-LVDS panel interface detected!  "
                   "This support is untested and may not "
                   "function properly\n");
    }
}

/* Read PLL parameters from BIOS block.  Default to typical values if there
   is no BIOS. */
static Bool R128GetPLLParameters(ScrnInfoPtr pScrn)
{
    R128InfoPtr   info = R128PTR(pScrn);
    R128PLLPtr    pll  = &info->pll;

#if defined(__powerpc__) || defined(__alpha__)
    /* there is no bios under Linux PowerPC but Open Firmware
       does set up the PLL registers properly and we can use
       those to calculate xclk and find the reference divider */

    unsigned x_mpll_ref_fb_div;
    unsigned xclk_cntl;
    unsigned Nx, M;
    unsigned PostDivSet[] = {0, 1, 2, 4, 8, 3, 6, 12};

    /* Assume REF clock is 2950 (in units of 10khz) */
    /* and that all pllclk must be between 125 Mhz and 250Mhz */
    pll->reference_freq = 2950;
    pll->min_pll_freq   = 12500;
    pll->max_pll_freq   = 25000;

    x_mpll_ref_fb_div = INPLL(pScrn, R128_X_MPLL_REF_FB_DIV);
    xclk_cntl = INPLL(pScrn, R128_XCLK_CNTL) & 0x7;
    pll->reference_div =
	INPLL(pScrn,R128_PPLL_REF_DIV) & R128_PPLL_REF_DIV_MASK;

    Nx = (x_mpll_ref_fb_div & 0x00FF00) >> 8;
    M =  (x_mpll_ref_fb_div & 0x0000FF);

    pll->xclk =  R128Div((2 * Nx * pll->reference_freq),
			 (M * PostDivSet[xclk_cntl]));

#else /* !defined(__powerpc__) */

    if (!info->VBIOS) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Video BIOS not detected, using default PLL parameters!\n");
				/* These probably aren't going to work for
				   the card you are using.  Specifically,
				   reference freq can be 29.50MHz,
				   28.63MHz, or 14.32MHz.  YMMV. */
	pll->reference_freq = 2950;
	pll->reference_div  = 65;
	pll->min_pll_freq   = 12500;
	pll->max_pll_freq   = 25000;
	pll->xclk           = 10300;
    } else {
	uint16_t bios_header    = R128_BIOS16(0x48);
	uint16_t pll_info_block = R128_BIOS16(bios_header + 0x30);
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Header at 0x%04x; PLL Information at 0x%04x\n",
                        bios_header, pll_info_block));

	pll->reference_freq = R128_BIOS16(pll_info_block + 0x0e);
	pll->reference_div  = R128_BIOS16(pll_info_block + 0x10);
	pll->min_pll_freq   = R128_BIOS32(pll_info_block + 0x12);
	pll->max_pll_freq   = R128_BIOS32(pll_info_block + 0x16);
	pll->xclk           = R128_BIOS16(pll_info_block + 0x08);
    }
#endif /* __powerpc__ */

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "PLL parameters: rf=%d rd=%d min=%d max=%d; xclk=%d\n",
	       pll->reference_freq,
	       pll->reference_div,
	       pll->min_pll_freq,
	       pll->max_pll_freq,
	       pll->xclk);

    return TRUE;
}

/* This is called by R128PreInit to set up the default visual. */
static Bool R128PreInitVisual(ScrnInfoPtr pScrn)
{
    if (!xf86SetDepthBpp(pScrn, 0, 0, 0, (Support24bppFb
					  | Support32bppFb
					  | SupportConvert32to24
					  )))
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
		   pScrn->depth, R128_DRIVER_NAME);
	return FALSE;
    }

    xf86PrintDepthBpp(pScrn);

    if (!xf86SetDefaultVisual(pScrn, -1)) return FALSE;

    if (pScrn->depth > 8 && pScrn->defaultVisual != TrueColor) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Default visual (%s) is not supported at depth %d\n",
		   xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
	return FALSE;
    }
    return TRUE;

}

/* This is called by R128PreInit to handle all color weight issues. */
static Bool R128PreInitWeight(ScrnInfoPtr pScrn)
{
    R128InfoPtr info          = R128PTR(pScrn);
    rgb defaultWeight = { 0, 0, 0 };

    /*
     * Save flag for 6 bit DAC to use for setting CRTC registers.
     * Otherwise use an 8 bit DAC, even if xf86SetWeight sets
     * pScrn->rgbBits to some value other than 8.
     */
    if (pScrn->depth <= 8) {
        if (info->dac6bits) {
            pScrn->rgbBits = 6;
        } else {
            pScrn->rgbBits = 8;
        }
    } else {
        info->dac6bits = FALSE;
        pScrn->rgbBits = 8;
    }

    if (pScrn->depth > 8) {
        if (!xf86SetWeight(pScrn, defaultWeight, defaultWeight)) return FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
               "Using %d bits per RGB (%d bit DAC)\n",
               pScrn->rgbBits, info->dac6bits ? 6 : 8);

    return TRUE;
}

/* This is called by R128PreInit to handle config file overrides for things
   like chipset and memory regions.  Also determine memory size and type.
   If memory type ever needs an override, put it in this routine. */
static Bool R128PreInitConfig(ScrnInfoPtr pScrn)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    R128EntPtr    pR128Ent  = R128EntPriv(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    EntityInfoPtr pEnt      = info->pEnt;
    GDevPtr       dev       = pEnt->device;
    int           offset    = 0;        /* RAM Type */
    MessageType   from;

				/* Chipset */
    from = X_PROBED;
    if (dev->chipset && *dev->chipset) {
	info->Chipset  = xf86StringToToken(R128Chipsets, dev->chipset);
	from           = X_CONFIG;
    } else if (dev->chipID >= 0) {
	info->Chipset  = dev->chipID;
	from           = X_CONFIG;
    } else {
	info->Chipset = PCI_DEV_DEVICE_ID(info->PciInfo);
    }
    pScrn->chipset = (char *)xf86TokenToString(R128Chipsets, info->Chipset);

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

				/* Framebuffer */

    from             = X_PROBED;
    info->LinearAddr = PCI_REGION_BASE(info->PciInfo, 0, REGION_MEM) & 0xfc000000;
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

				/* MMIO registers */
    from             = X_PROBED;
    info->MMIOAddr   = PCI_REGION_BASE(info->PciInfo, 2, REGION_MEM) & 0xffffff00;
    if (dev->IOBase) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "MMIO address override, using 0x%08lx instead of 0x%08lx\n",
		   dev->IOBase,
		   info->MMIOAddr);
	info->MMIOAddr = dev->IOBase;
	from           = X_CONFIG;
    } else if (!info->MMIOAddr) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid MMIO address\n");
	return FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, from,
	       "MMIO registers at 0x%08lx\n", info->MMIOAddr);

#ifndef XSERVER_LIBPCIACCESS
				/* BIOS */
    from              = X_PROBED;
    info->BIOSAddr    = info->PciInfo->biosBase & 0xfffe0000;
    if (info->BIOSAddr) {
	xf86DrvMsg(pScrn->scrnIndex, from,
		   "BIOS at 0x%08lx\n", info->BIOSAddr);
    }
#endif

				/* Flat panel (part 1) */
    if (xf86GetOptValBool(info->Options, OPTION_PROG_FP_REGS,
			  &info->HasPanelRegs)) {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		   "Turned flat panel register programming %s\n",
		   info->HasPanelRegs ? "on" : "off");
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "\n\nWARNING: Forcing the driver to use/not use the flat panel registers\nmight damage your flat panel.  Use at your *OWN* *RISK*.\n\n");
    } else {
        info->isDFP = FALSE;
        info->isPro2 = FALSE;
        pR128Ent->HasCRTC2 = FALSE;
	switch (info->Chipset) {
	/* R128 Pro and Pro2 can have DFP, we will deal with it.
	   No support for dual-head/xinerama yet.
           M3 can also have DFP, no support for now */
	case PCI_CHIP_RAGE128TF:
	case PCI_CHIP_RAGE128TL:
	case PCI_CHIP_RAGE128TR:
	/* FIXME: RAGE128 TS/TT/TU are assumed to be PRO2 as all 6 chips came
	 *        out at the same time, so are of the same family likely.
	 *        This requires confirmation however to be fully correct.
	 *        Mike A. Harris <mharris@redhat.com>
	 */
	case PCI_CHIP_RAGE128TS:
	case PCI_CHIP_RAGE128TT:
	case PCI_CHIP_RAGE128TU: info->isPro2 = TRUE;
	/* FIXME: RAGE128 P[ABCEGHIJKLMNOQSTUVWX] are assumed to have DFP
	 *        capability, as the comment at the top suggests.
	 *        This requires confirmation however to be fully correct.
	 *        Mike A. Harris <mharris@redhat.com>
	 */
	case PCI_CHIP_RAGE128PA:
	case PCI_CHIP_RAGE128PB:
	case PCI_CHIP_RAGE128PC:
	case PCI_CHIP_RAGE128PE:
	case PCI_CHIP_RAGE128PG:
	case PCI_CHIP_RAGE128PH:
	case PCI_CHIP_RAGE128PI:
	case PCI_CHIP_RAGE128PJ:
	case PCI_CHIP_RAGE128PK:
	case PCI_CHIP_RAGE128PL:
	case PCI_CHIP_RAGE128PM:
	case PCI_CHIP_RAGE128PN:
	case PCI_CHIP_RAGE128PO:
	case PCI_CHIP_RAGE128PQ:
	case PCI_CHIP_RAGE128PS:
	case PCI_CHIP_RAGE128PT:
	case PCI_CHIP_RAGE128PU:
	case PCI_CHIP_RAGE128PV:
	case PCI_CHIP_RAGE128PW:
	case PCI_CHIP_RAGE128PX:

	case PCI_CHIP_RAGE128PD:
	case PCI_CHIP_RAGE128PF:
	case PCI_CHIP_RAGE128PP:
	case PCI_CHIP_RAGE128PR: info->isDFP = TRUE; break;

	case PCI_CHIP_RAGE128LE:
	case PCI_CHIP_RAGE128LF:
	case PCI_CHIP_RAGE128MF:
	case PCI_CHIP_RAGE128ML: 
			info->HasPanelRegs = TRUE;  
			/* which chips support dualhead? */
			pR128Ent->HasCRTC2 = TRUE;
			break;
	case PCI_CHIP_RAGE128RE:
	case PCI_CHIP_RAGE128RF:
	case PCI_CHIP_RAGE128RG:
	case PCI_CHIP_RAGE128RK:
	case PCI_CHIP_RAGE128RL:
	case PCI_CHIP_RAGE128SM:
	/* FIXME: RAGE128 S[EFGHKLN] are assumed to be like the SM above as
	 *        all of them are listed as "Rage 128 4x" in ATI docs.
	 *        This requires confirmation however to be fully correct.
	 *        Mike A. Harris <mharris@redhat.com>
	 */
	case PCI_CHIP_RAGE128SE:
	case PCI_CHIP_RAGE128SF:
	case PCI_CHIP_RAGE128SG:
	case PCI_CHIP_RAGE128SH:
	case PCI_CHIP_RAGE128SK:
	case PCI_CHIP_RAGE128SL:
	case PCI_CHIP_RAGE128SN:
	default:                 info->HasPanelRegs = FALSE; break;
	}
    }

				/* Read registers used to determine options */
    from                      = X_PROBED;
    if (!R128MapMMIO(pScrn)) return FALSE;
    R128MMIO                  = info->MMIO;

    if (info->FBDev)
	pScrn->videoRam       = fbdevHWGetVidmem(pScrn) / 1024;
    else
	pScrn->videoRam       = INREG(R128_CONFIG_MEMSIZE) / 1024;

    info->MemCntl             = INREG(R128_MEM_CNTL);
    info->BusCntl             = INREG(R128_BUS_CNTL);

				/* RAM */
    switch (info->MemCntl & 0x3) {
    case 0:                     /* SDR SGRAM 1:1 */
	switch (info->Chipset) {
	case PCI_CHIP_RAGE128TF:
	case PCI_CHIP_RAGE128TL:
	case PCI_CHIP_RAGE128TR:
	case PCI_CHIP_RAGE128LE:
	case PCI_CHIP_RAGE128LF:
	case PCI_CHIP_RAGE128MF:
	case PCI_CHIP_RAGE128ML:
	case PCI_CHIP_RAGE128RE:
	case PCI_CHIP_RAGE128RF:
	case PCI_CHIP_RAGE128RG: offset = 0; break; /* 128-bit SDR SGRAM 1:1 */
	case PCI_CHIP_RAGE128RK:
	case PCI_CHIP_RAGE128RL:
	case PCI_CHIP_RAGE128SM:
	default:                 offset = 1; break; /*  64-bit SDR SGRAM 1:1 */
	}
	break;
    case 1:                      offset = 2; break; /*  64-bit SDR SGRAM 2:1 */
    case 2:                      offset = 3; break; /*  64-bit DDR SGRAM     */
    default:                     offset = 1; break; /*  64-bit SDR SGRAM 1:1 */
    }
    info->ram = &R128RAM[offset];

    if (dev->videoRam) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Video RAM override, using %d kB instead of %d kB\n",
		   dev->videoRam,
		   pScrn->videoRam);
	from             = X_CONFIG;
	pScrn->videoRam  = dev->videoRam;
    }

    xf86DrvMsg(pScrn->scrnIndex, from,
	       "VideoRAM: %d kByte (%s)\n", pScrn->videoRam, info->ram->name);

    pScrn->videoRam  &= ~1023;
    info->FbMapSize  = pScrn->videoRam * 1024;

#ifdef R128DRI
    /* AGP/PCI */
    if (!info->IsPCI) {
	switch (info->Chipset) {
	case PCI_CHIP_RAGE128LE:
	case PCI_CHIP_RAGE128RE:
	case PCI_CHIP_RAGE128RK:
	case PCI_CHIP_RAGE128PD:
	case PCI_CHIP_RAGE128PR:
	case PCI_CHIP_RAGE128PP: info->IsPCI = TRUE;  break;
	case PCI_CHIP_RAGE128LF:
	case PCI_CHIP_RAGE128MF:
	case PCI_CHIP_RAGE128ML:
	case PCI_CHIP_RAGE128PF:
	case PCI_CHIP_RAGE128RF:
	case PCI_CHIP_RAGE128RG:
	case PCI_CHIP_RAGE128RL:
	case PCI_CHIP_RAGE128SM:
	case PCI_CHIP_RAGE128TF:
	case PCI_CHIP_RAGE128TL:
	case PCI_CHIP_RAGE128TR:
	/* FIXME: Rage 128 S[EFGHKLN], T[STU], P[ABCEGHIJKLMNOQSTUVWX] are
	 * believed to be AGP, but need confirmation. <mharris@redhat.com>
	 */
	case PCI_CHIP_RAGE128PA:
	case PCI_CHIP_RAGE128PB:
	case PCI_CHIP_RAGE128PC:
	case PCI_CHIP_RAGE128PE:
	case PCI_CHIP_RAGE128PG:
	case PCI_CHIP_RAGE128PH:
	case PCI_CHIP_RAGE128PI:
	case PCI_CHIP_RAGE128PJ:
	case PCI_CHIP_RAGE128PK:
	case PCI_CHIP_RAGE128PL:
	case PCI_CHIP_RAGE128PM:
	case PCI_CHIP_RAGE128PN:
	case PCI_CHIP_RAGE128PO:
	case PCI_CHIP_RAGE128PQ:
	case PCI_CHIP_RAGE128PS:
	case PCI_CHIP_RAGE128PT:
	case PCI_CHIP_RAGE128PU:
	case PCI_CHIP_RAGE128PV:
	case PCI_CHIP_RAGE128PW:
	case PCI_CHIP_RAGE128PX:
	case PCI_CHIP_RAGE128TS:
	case PCI_CHIP_RAGE128TT:
	case PCI_CHIP_RAGE128TU:
	case PCI_CHIP_RAGE128SE:
	case PCI_CHIP_RAGE128SF:
	case PCI_CHIP_RAGE128SG:
	case PCI_CHIP_RAGE128SH:
	case PCI_CHIP_RAGE128SK:
	case PCI_CHIP_RAGE128SL:
	case PCI_CHIP_RAGE128SN:
	default:                 info->IsPCI = FALSE; break;
	}
    }
#endif

    return TRUE;
}

static Bool R128PreInitDDC(ScrnInfoPtr pScrn, xf86Int10InfoPtr pInt10)
{
#if !defined(__powerpc__) && !defined(__alpha__) && !defined(__sparc__)
    R128InfoPtr   info = R128PTR(pScrn);
    vbeInfoPtr pVbe;
#endif

    if (!xf86LoadSubModule(pScrn, "ddc")) return FALSE;
    if (!xf86LoadSubModule(pScrn, "i2c")) return FALSE;

#if defined(__powerpc__) || defined(__alpha__) || defined(__sparc__)
    /* Int10 is broken on PPC and some Alphas */
    return TRUE;
#else
    if (xf86LoadSubModule(pScrn, "vbe")) {
	pVbe = VBEInit(pInt10,info->pEnt->index);
	if (!pVbe) return FALSE;
        xf86SetDDCproperties(pScrn,xf86PrintEDID(vbeDoEDID(pVbe,NULL)));
	vbeFree(pVbe);
	return TRUE;
    } else
	return FALSE;
#endif
}

/* This is called by R128PreInit to initialize gamma correction. */
static Bool R128PreInitGamma(ScrnInfoPtr pScrn)
{
    Gamma zeros = { 0.0, 0.0, 0.0 };

    if (!xf86SetGamma(pScrn, zeros)) return FALSE;
    return TRUE;
}

/* This is called by R128PreInit to initialize the hardware cursor. */
static Bool R128PreInitCursor(ScrnInfoPtr pScrn)
{
    R128InfoPtr   info = R128PTR(pScrn);

    if (!info->swCursor) {
	if (!xf86LoadSubModule(pScrn, "ramdac")) return FALSE;
    }
    return TRUE;
}

static Bool R128PreInitInt10(ScrnInfoPtr pScrn, xf86Int10InfoPtr *ppInt10)
{
#if !defined(__powerpc__) && !defined(__alpha__)
    R128InfoPtr   info = R128PTR(pScrn);

    /* int10 is broken on some Alphas and powerpc */
    if (xf86LoadSubModule(pScrn, "int10")) {
	xf86DrvMsg(pScrn->scrnIndex,X_INFO,"initializing int10\n");
	*ppInt10 = xf86InitInt10(info->pEnt->index);
    }
#endif
    return TRUE;
}

#ifdef R128DRI
static Bool R128PreInitDRI(ScrnInfoPtr pScrn)
{
    R128InfoPtr   info = R128PTR(pScrn);

    info->agpMode        = R128_DEFAULT_AGP_MODE;
    info->agpSize        = R128_DEFAULT_AGP_SIZE;
    info->ringSize       = R128_DEFAULT_RING_SIZE;
    info->bufSize        = R128_DEFAULT_BUFFER_SIZE;
    info->agpTexSize     = R128_DEFAULT_AGP_TEX_SIZE;

    info->CCEusecTimeout = R128_DEFAULT_CCE_TIMEOUT;

    if (!info->IsPCI) {
	if (xf86GetOptValInteger(info->Options,
				 OPTION_AGP_MODE, &(info->agpMode))) {
	    if (info->agpMode < 1 || info->agpMode > R128_AGP_MAX_MODE) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Illegal AGP Mode: %d\n", info->agpMode);
		return FALSE;
	    }
	    xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		       "Using AGP %dx mode\n", info->agpMode);
	}

	if (xf86GetOptValInteger(info->Options,
				 OPTION_AGP_SIZE, (int *)&(info->agpSize))) {
	    switch (info->agpSize) {
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
			   "Illegal AGP size: %d MB\n", info->agpSize);
		return FALSE;
	    }
	}

	if (xf86GetOptValInteger(info->Options,
				 OPTION_RING_SIZE, &(info->ringSize))) {
	    if (info->ringSize < 1 || info->ringSize >= (int)info->agpSize) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "Illegal ring buffer size: %d MB\n",
			   info->ringSize);
		return FALSE;
	    }
	}

	if (xf86GetOptValInteger(info->Options,
				 OPTION_BUFFER_SIZE, &(info->bufSize))) {
	    if (info->bufSize < 1 || info->bufSize >= (int)info->agpSize) {
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

	if (info->ringSize + info->bufSize + info->agpTexSize >
	    (int)info->agpSize) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Buffers are too big for requested AGP space\n");
	    return FALSE;
	}

	info->agpTexSize = info->agpSize - (info->ringSize + info->bufSize);
    }

    if (xf86GetOptValInteger(info->Options, OPTION_USEC_TIMEOUT,
			     &(info->CCEusecTimeout))) {
	/* This option checked by the R128 DRM kernel module */
    }

    if (!xf86LoadSubModule(pScrn, "shadowfb")) {
	info->allowPageFlip = 0;
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Couldn't load shadowfb module:\n");
    } else {
	info->allowPageFlip = xf86ReturnOptValBool(info->Options,
						   OPTION_PAGE_FLIP,
						   FALSE);
    }

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Page flipping %sabled\n",
	       info->allowPageFlip ? "en" : "dis");

    return TRUE;
}
#endif

static Bool R128PreInitControllers(ScrnInfoPtr pScrn, xf86Int10InfoPtr pInt10)
{
    xf86CrtcConfigPtr config = XF86_CRTC_CONFIG_PTR(pScrn);
    int found = 0;
    int i;

    if (!R128GetBIOSParameters(pScrn, pInt10))
        return FALSE;

    if (!R128GetPLLParameters(pScrn))
        return FALSE;

    if (!R128AllocateControllers(pScrn))
        return FALSE;

    if (!R128SetupConnectors(pScrn))
        return FALSE;

    for (i = 0; i < config->num_output; i++) {
        xf86OutputPtr output = config->output[i];

        output->status = (*output->funcs->detect) (output);
        if (output->status == XF86OutputStatusConnected)
            found++;
    }
    return !!found;
}

static void
r128UMSOption(ScrnInfoPtr pScrn)
{
    R128InfoPtr      info = R128PTR(pScrn);

    info->dac6bits = xf86ReturnOptValBool(info->Options,
                                            OPTION_DAC_6BIT, FALSE);

#if defined(__powerpc__) && defined(__linux__)
    if (xf86ReturnOptValBool(info->Options, OPTION_FBDEV, TRUE))
#else
    if (xf86ReturnOptValBool(info->Options, OPTION_FBDEV, FALSE))
#endif
    {
        info->FBDev = TRUE;
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                    "Using framebuffer device.\n");
    }

    /* By default, don't access VGA IOs on PowerPC or SPARC. */
#if defined(__powerpc__) || defined(__sparc__) || !defined(WITH_VGAHW)
    info->VGAAccess = FALSE;
#else
    info->VGAAccess = TRUE;
#endif

#ifdef WITH_VGAHW
    xf86GetOptValBool(info->Options, OPTION_VGA_ACCESS,
                        &info->VGAAccess);
    if (info->VGAAccess) {
       if (!xf86LoadSubModule(pScrn, "vgahw"))
           info->VGAAccess = FALSE;
        else {
            if (!vgaHWGetHWRec(pScrn))
               info->VGAAccess = FALSE;
       }

       if (!info->VGAAccess) {
           xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "Loading VGA module failed, trying to "
                       "run without it.\n");
       }
    } else
           xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                       "VGAAccess option set to FALSE, VGA "
                       "module load skipped.\n");
    if (info->VGAAccess) {
        vgaHWSetStdFuncs(VGAHWPTR(pScrn));
        vgaHWGetIOBase(VGAHWPTR(pScrn));
    }
#else
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                "VGAHW support not compiled, VGA "
                "module load skipped.\n");
#endif

    if (xf86ReturnOptValBool(info->Options,
                                OPTION_SHOW_CACHE, FALSE)) {
        info->showCache = TRUE;
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                    "ShowCache enabled.\n");
    }

    if (xf86ReturnOptValBool(info->Options,
                                OPTION_SW_CURSOR, FALSE)) {
        info->swCursor = TRUE;
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                    "Software cursor requested.\n");
    }

    if(xf86GetOptValInteger(info->Options,
                            OPTION_VIDEO_KEY, &info->videoKey)) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                    "Video key set to 0x%x.\n", info->videoKey);
    } else {
        info->videoKey = 0x1E;
    }

#ifdef R128DRI
    /* DMA for Xv */
    info->DMAForXv = xf86ReturnOptValBool(info->Options,
                                            OPTION_XV_DMA, FALSE);
    if (info->DMAForXv) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                   "Will try to use DMA for Xv image transfers.\n");
    }

    /* Force PCI Mode */
    info->IsPCI = xf86ReturnOptValBool(info->Options,
                                        OPTION_IS_PCI, FALSE);
    if (info->IsPCI) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                    "Forced into PCI only mode.\n");
    }

    if (xf86ReturnOptValBool(info->Options, OPTION_CCE_PIO, FALSE)) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                    "Forcing CCE into PIO mode.\n");
        info->CCEMode = R128_DEFAULT_CCE_PIO_MODE;
    } else {
        info->CCEMode = R128_DEFAULT_CCE_BM_MODE;
    }

    if (xf86ReturnOptValBool(info->Options, OPTION_NO_SECURITY, FALSE)) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                    "WARNING!!! CCE Security checks disabled!!!\n");
        info->CCESecure = FALSE;
    } else {
        info->CCESecure = TRUE;
    }


#endif
}

static void
r128AcquireOption(ScrnInfoPtr pScrn)
{
    R128InfoPtr      info = R128PTR(pScrn);
#ifdef USE_EXA
    char *optstr;
#endif

    if (xf86ReturnOptValBool(info->Options, OPTION_NOACCEL, FALSE)) {
        info->noAccel = TRUE;
    }

#ifdef USE_EXA
    if (!info->noAccel) {
        optstr = (char *) xf86GetOptValString(info->Options,
                                                OPTION_ACCELMETHOD);
        if (optstr) {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "AccelMethod option found.\n");
            if (xf86NameCmp(optstr, "EXA") == 0) {
                info->useEXA = TRUE;
                xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "AccelMethod is set to EXA, turning "
                            "EXA on.\n");
            }
        }

#ifdef RENDER
        info->RenderAccel = xf86ReturnOptValBool(info->Options,
                                                    OPTION_RENDERACCEL,
                                                    TRUE);
        if (info->RenderAccel)
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Acceleration of RENDER operations will be "
                        "enabled upon successful loading of DRI and "
                        "EXA.\n");
#endif
    }
#endif

    r128UMSOption(pScrn);
}

static Bool R128CRTCResize(ScrnInfoPtr pScrn, int width, int height)
{
    pScrn->virtualX = width;
    pScrn->virtualY = height;
    return TRUE;
}

static const xf86CrtcConfigFuncsRec R128CRTCResizeFuncs = {
    R128CRTCResize
};

static Bool R128LegacyMS(ScrnInfoPtr pScrn)
{
    R128InfoPtr      info = R128PTR(pScrn);
    xf86Int10InfoPtr pInt10 = NULL;
    Bool ret = FALSE;

    if (info->FBDev) {
        /* check for linux framebuffer device */
        if (!xf86LoadSubModule(pScrn, "fbdevhw")) goto exit;
        if (!fbdevHWInit(pScrn, info->PciInfo, NULL)) goto exit;
        pScrn->SwitchMode    = fbdevHWSwitchModeWeak();
        pScrn->AdjustFrame   = fbdevHWAdjustFrameWeak();
        pScrn->ValidMode     = fbdevHWValidModeWeak();
    } else {
        if (!R128PreInitInt10(pScrn, &pInt10)) goto exit;
    }

    if (!R128PreInitConfig(pScrn)) goto freeInt10;

    xf86CrtcSetSizeRange(pScrn, 320, 200, 4096, 4096);

    if (!R128PreInitCursor(pScrn)) goto freeInt10;

    /* Don't fail on this one */
    info->DDC = R128PreInitDDC(pScrn, pInt10);

    if (!R128PreInitControllers(pScrn, pInt10)) goto freeInt10;

#ifdef R128DRI
    if (!R128PreInitDRI(pScrn)) goto freeInt10;
#endif

    ret = TRUE;
freeInt10:
    /* Free int10 info */
    if (pInt10) {
        xf86FreeInt10(pInt10);
    }

exit:
    return ret;
}

static void
R128PreInitAccel(ScrnInfoPtr pScrn)
{
    R128InfoPtr      info = R128PTR(pScrn);
#ifdef USE_EXA
    int errmaj, errmin;
#endif

    if (!info->noAccel) {
        if (info->useEXA) {
#ifdef USE_EXA
            info->exaReq.majorversion = EXA_VERSION_MAJOR;
            info->exaReq.minorversion = EXA_VERSION_MINOR;

            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Loading EXA module...\n");
            if (LoadSubModule(pScrn->module, "exa", NULL, NULL, NULL,
                                &info->exaReq, &errmaj, &errmin)) {
                xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Loading EXA module.\n");
            } else {
                LoaderErrorMsg(NULL, "exa", errmaj, errmin);
            }
#endif
        }

        if ((!info->useEXA) ||
            ((info->useEXA) && (!info->accelOn))) {
#ifdef HAVE_XAA_H
            if (xf86LoadSubModule(pScrn, "xaa")) {
                xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                            "Loading XAA module.\n");
            }
#endif
        }
    }
}

/* R128PreInit is called once at server startup. */
Bool R128PreInit(ScrnInfoPtr pScrn, int flags)
{
    R128InfoPtr      info;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "%s\n", __func__));

    if (flags & PROBE_DETECT) {
        return TRUE;
    }

    pScrn->monitor = pScrn->confScreen->monitor;

    if (!R128PreInitVisual(pScrn)) {
        return FALSE;
    }

    if (!R128PreInitGamma(pScrn)) {
        return FALSE;
    }

    if (pScrn->numEntities != 1) return FALSE;

    if (!R128GetRec(pScrn)) return FALSE;

    info                = R128PTR(pScrn);
    info->SwitchingMode = FALSE;
    info->MMIO          = NULL;

    info->pEnt          = xf86GetEntityInfo(pScrn->entityList[0]);
    if (info->pEnt->location.type != BUS_PCI) goto fail;

    info->PciInfo       = xf86GetPciInfoForEntity(info->pEnt->index);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	       "PCI bus %d card %d func %d\n",
	       PCI_DEV_BUS(info->PciInfo),
	       PCI_DEV_DEV(info->PciInfo),
	       PCI_DEV_FUNC(info->PciInfo));

#ifndef XSERVER_LIBPCIACCESS
    info->PciTag        = pciTag(PCI_DEV_BUS(info->PciInfo),
				PCI_DEV_DEV(info->PciInfo),
				PCI_DEV_FUNC(info->PciInfo));

    if (xf86RegisterResources(info->pEnt->index, 0, ResNone)) goto fail;
    if (xf86SetOperatingState(resVga, info->pEnt->index, ResUnusedOpr)) goto fail;

    pScrn->racMemFlags  = RAC_FB | RAC_COLORMAP | RAC_VIEWPORT | RAC_CURSOR;
#endif

    info->fifo_slots  = 0;
    info->pix24bpp    = xf86GetBppFromDepth(pScrn, pScrn->depth);
    info->CurrentLayout.bitsPerPixel = pScrn->bitsPerPixel;
    info->CurrentLayout.depth        = pScrn->depth;
    info->CurrentLayout.pixel_bytes  = pScrn->bitsPerPixel / 8;
    info->CurrentLayout.pixel_code   = (pScrn->bitsPerPixel != 16
                                       ? pScrn->bitsPerPixel
                                       : pScrn->depth);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
           "Pixel depth = %d bits stored in %d byte%s (%d bpp pixmaps)\n",
           pScrn->depth,
           info->CurrentLayout.pixel_bytes,
           info->CurrentLayout.pixel_bytes > 1 ? "s" : "",
           info->pix24bpp);

				/* We can't do this until we have a
				   pScrn->display. */
    xf86CollectOptions(pScrn, NULL);
    if (!(info->Options = malloc(sizeof(R128Options))))    goto fail;
    memcpy(info->Options, R128Options, sizeof(R128Options));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, info->Options);

    info->noAccel = FALSE;
    info->accelOn = FALSE;

    info->useEXA = FALSE;
#ifdef USE_EXA
#ifndef HAVE_XAA_H
    info->useEXA = TRUE;
#endif
#endif

    info->swCursor = FALSE;

    r128AcquireOption(pScrn);

    if (!R128PreInitWeight(pScrn))    goto fail;

    /* Allocate an xf86CrtcConfig */
    xf86CrtcConfigInit(pScrn, &R128CRTCResizeFuncs);

    R128LegacyMS(pScrn);

    if (!xf86InitialConfiguration(pScrn, TRUE)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes.\n");
        goto fail;
    }
    pScrn->displayWidth = (pScrn->virtualX + 63) & ~63;

    /* Set display resolution */
    xf86SetDpi(pScrn, 0, 0);

    /* Get ScreenInit function */
    if (!xf86LoadSubModule(pScrn, "fb")) return FALSE;

    R128PreInitAccel(pScrn);

    info->CurrentLayout.displayWidth = pScrn->displayWidth;

    if (!xf86RandR12PreInit(pScrn)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "RandR initialization failure\n");
        goto fail;
    }

    if (pScrn->modes == NULL) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No modes.\n");
        goto fail;
    }

				/* Free the video bios (if applicable) */
    if (info->VBIOS) {
	free(info->VBIOS);
	info->VBIOS = NULL;
    }

    if (info->MMIO) R128UnmapMMIO(pScrn);
    info->MMIO = NULL;

    return TRUE;

  fail:
				/* Pre-init failed. */

				/* Free the video bios (if applicable) */
    if (info->VBIOS) {
	free(info->VBIOS);
	info->VBIOS = NULL;
    }

#ifdef WITH_VGAHW
    if (info->VGAAccess)
           vgaHWFreeHWRec(pScrn);
#endif

    if (info->MMIO) R128UnmapMMIO(pScrn);
    info->MMIO = NULL;

    R128FreeRec(pScrn);
    return FALSE;
}

/* Load a palette. */
static void R128LoadPalette(ScrnInfoPtr pScrn, int numColors,
			    int *indices, LOCO *colors, VisualPtr pVisual)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i, j;
    int c, index;
    uint16_t lut_r[256], lut_g[256], lut_b[256];

    for (c = 0; c < xf86_config->num_crtc; c++) {
        xf86CrtcPtr crtc = xf86_config->crtc[c];
        R128CrtcPrivatePtr r128_crtc = crtc->driver_private;

        for (i = 0 ; i < 256; i++) {
            lut_r[i] = r128_crtc->lut_r[i] << 8;
            lut_g[i] = r128_crtc->lut_g[i] << 8;
            lut_b[i] = r128_crtc->lut_b[i] << 8;
        }

        switch (info->CurrentLayout.depth) {
        case 15:
            for (i = 0; i < numColors; i++) {
                index = indices[i];
                for (j = 0; j < 8; j++) {
                    lut_r[index * 8 + j] = colors[index].red << 8;
                    lut_g[index * 8 + j] = colors[index].green << 8;
                    lut_b[index * 8 + j] = colors[index].blue << 8;
                }
            }
        case 16:
            for (i = 0; i < numColors; i++) {
                index = indices[i];

                /* XXX: The old version of R128LoadPalette did not do this and
                 * the old version of RADEONLoadPalette has a comment asking why.
                 */
                if (i <= 31) {
                    for (j = 0; j < 8; j++) {
                        lut_r[index * 8 + j] = colors[index].red << 8;
                        lut_b[index * 8 + j] = colors[index].blue << 8;
                    }
                }

                for (j = 0; j < 4; j++) {
                    lut_g[index * 4 + j] = colors[index].green << 8;
                }
            }
        default:
            for (i = 0; i < numColors; i++) {
                index = indices[i];
                lut_r[index] = colors[index].red << 8;
                lut_g[index] = colors[index].green << 8;
                lut_b[index] = colors[index].blue << 8;
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

static void
R128BlockHandler(BLOCKHANDLER_ARGS_DECL)
{
    SCREEN_PTR(arg);
    ScrnInfoPtr pScrn   = xf86ScreenToScrn(pScreen);
    R128InfoPtr info    = R128PTR(pScrn);

#ifdef R128DRI
    if (info->directRenderingEnabled)
        FLUSH_RING();
#endif

    pScreen->BlockHandler = info->BlockHandler;
    (*pScreen->BlockHandler) (BLOCKHANDLER_ARGS);
    pScreen->BlockHandler = R128BlockHandler;

    if(info->VideoTimerCallback) {
        (*info->VideoTimerCallback)(pScrn, currentTime.milliseconds);
    }
}

/* Called at the start of each server generation. */
Bool R128ScreenInit(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    R128InfoPtr info   = R128PTR(pScrn);
    BoxRec      MemBox;
    int width_bytes = (pScrn->displayWidth *
                        info->CurrentLayout.pixel_bytes);
    int scanlines;
    int total = info->FbMapSize;
    FBAreaPtr fbarea = NULL;
#ifdef R128DRI
    int cpp = info->CurrentLayout.pixel_bytes;
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
#ifdef USE_EXA
    ExaOffscreenArea*     osArea = NULL;
#endif /* USE_EXA */
#endif /* R128DRI */

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "%s %lx %lx\n",
                        __func__,
                        pScrn->memPhysBase, pScrn->fbOffset));

#ifdef R128DRI
				/* Turn off the CCE for now. */
    info->CCEInUse     = FALSE;
    info->indirectBuffer = NULL;
#endif

    if (!R128MapMem(pScrn)) return FALSE;
    pScrn->fbOffset    = 0;
    //if(info->IsSecondary) pScrn->fbOffset = pScrn->videoRam * 1024;
#ifdef R128DRI
    info->fbX          = 0;
    info->fbY          = 0;
    info->frontOffset  = 0;
    info->frontPitch   = pScrn->displayWidth;
#endif

    info->PaletteSavedOnVT = FALSE;

    R128Save(pScrn);

				/* Visual setup */
    miClearVisualTypes();
    if (!miSetVisualTypes(pScrn->depth,
			  miGetDefaultVisualMask(pScrn->depth),
			  pScrn->rgbBits,
			  pScrn->defaultVisual)) return FALSE;
    miSetPixmapDepths ();

#ifdef R128DRI
				/* Setup DRI after visuals have been
				   established, but before fbScreenInit is
				   called. */
    {
	/* FIXME: When we move to dynamic allocation of back and depth
	   buffers, we will want to revisit the following check for 3
	   times the virtual size of the screen below. */
	int maxy        = info->FbMapSize / width_bytes;

        if (info->noAccel) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Acceleration disabled, not initializing the DRI\n");
	    info->directRenderingEnabled = FALSE;
	} else if (maxy <= pScrn->virtualY * 3) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "Static buffer allocation failed -- "
		       "need at least %d kB video memory\n",
		       (pScrn->displayWidth * pScrn->virtualY *
			info->CurrentLayout.pixel_bytes * 3 + 1023) / 1024);
	    info->directRenderingEnabled = FALSE;
	} else {
            info->directRenderingEnabled = R128DRIScreenInit(pScreen);
	}
    }
#endif

    if (!fbScreenInit (pScreen, info->FB,
		       pScrn->virtualX, pScrn->virtualY,
		       pScrn->xDpi, pScrn->yDpi, pScrn->displayWidth,
		       pScrn->bitsPerPixel))
	return FALSE;

    xf86SetBlackWhitePixels(pScreen);

    if (pScrn->bitsPerPixel > 8) {
	VisualPtr visual;

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

    /* must be after RGB order fixed */
    fbPictureInit (pScreen, 0, 0);

				/* Memory manager setup */
#ifdef R128DRI
    if (info->directRenderingEnabled) {
	int bufferSize = pScrn->virtualY * width_bytes;
	int l;

	switch (info->CCEMode) {
	case R128_DEFAULT_CCE_PIO_MODE:
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CCE in PIO mode\n");
	    break;
	case R128_DEFAULT_CCE_BM_MODE:
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CCE in BM mode\n");
	    break;
	default:
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CCE in UNKNOWN mode\n");
	    break;
	}

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Using %d MB AGP aperture\n", info->agpSize);
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Using %d MB for the ring buffer\n", info->ringSize);
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Using %d MB for vertex/indirect buffers\n", info->bufSize);
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Using %d MB for AGP textures\n", info->agpTexSize);

	/* Try for front, back, depth, and two framebuffers worth of
	 * pixmap cache.  Should be enough for a fullscreen background
	 * image plus some leftovers.
	 */
	info->textureSize = info->FbMapSize - 5 * bufferSize;

	/* If that gives us less than half the available memory, let's
	 * be greedy and grab some more.  Sorry, I care more about 3D
	 * performance than playing nicely, and you'll get around a full
	 * framebuffer's worth of pixmap cache anyway.
	 */
	if (info->textureSize < (int)info->FbMapSize / 2) {
	    info->textureSize = info->FbMapSize - 4 * bufferSize;
	}

	if (info->textureSize > 0) {
	    l = R128MinBits((info->textureSize-1) / R128_NR_TEX_REGIONS);
	    if (l < R128_LOG_TEX_GRANULARITY) l = R128_LOG_TEX_GRANULARITY;

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

        total = info->FbMapSize - info->textureSize;
    }
#endif /* R128DRI */

    scanlines = total / width_bytes;
    if (scanlines > 8191) scanlines = 8191;

#ifdef R128DRI
    if (info->directRenderingEnabled)
        /*
         * Recalculate the texture offset and size to accommodate any
         * rounding to a whole number of scanlines.
         */
        info->textureOffset = scanlines * width_bytes;
#endif /* R128DRI */

    MemBox.x1 = 0;
    MemBox.y1 = 0;
    MemBox.x2 = pScrn->displayWidth;
    MemBox.y2 = scanlines;

	if (!info->useEXA) {
	    if (!xf86InitFBManager(pScreen, &MemBox)) {
	        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		           "Memory manager initialization to (%d,%d) (%d,%d) failed\n",
		           MemBox.x1, MemBox.y1, MemBox.x2, MemBox.y2);
	        return FALSE;
	    } else {
	        int width, height;

	        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		           "Memory manager initialized to (%d,%d) (%d,%d)\n",
		           MemBox.x1, MemBox.y1, MemBox.x2, MemBox.y2);
	        if ((fbarea = xf86AllocateOffscreenArea(pScreen,
						        pScrn->displayWidth,
						        2, 0, NULL, NULL, NULL))) {
		    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			       "Reserved area from (%d,%d) to (%d,%d)\n",
			       fbarea->box.x1, fbarea->box.y1,
			       fbarea->box.x2, fbarea->box.y2);
	        } else {
		    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to reserve area\n");
	        }
	        if (xf86QueryLargestOffscreenArea(pScreen, &width,
						  &height, 0, 0, 0)) {
		    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			       "Largest offscreen area available: %d x %d\n",
				width, height);
	        }

            if (!info->noAccel) {
                if (R128XAAAccelInit(pScreen)) {
                    info->accelOn = TRUE;
                    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                "XAA acceleration enabled.\n");
                } else {
                    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                                "Acceleration disabled.\n");
                }
            }
        }
    }
#ifdef USE_EXA
    else {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "Filling in EXA memory info\n");


        /*
         * Don't give EXA the true full memory size, because
         * the textureSize sized chunk on the end is handled
         * by DRI.
         */
        if (R128EXAInit(pScreen, total)) {
            info->accelOn = TRUE;
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "EXA Acceleration enabled.\n");
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                        "EXA Acceleration initialization "
                        "failed.\n");
            xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Acceleration disabled.\n");
        }
    }
#endif

#ifdef R128DRI
    if (info->directRenderingEnabled) {
				/* Allocate the shared back buffer */
	if(!info->useEXA) {
	    fbarea = xf86AllocateOffscreenArea(pScreen,
					       pScrn->virtualX,
					       pScrn->virtualY,
					       32, NULL, NULL, NULL);

	    if (fbarea) {
		x1 = fbarea->box.x1;
		x2 = fbarea->box.x2;
		y1 = fbarea->box.y1;
		y2 = fbarea->box.y2;
	    }
	}
#ifdef USE_EXA
	else {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Actually trying an EXA allocation...\n");
	    osArea = exaOffscreenAlloc(pScreen,
				       pScrn->virtualY * width_bytes,
				       32, TRUE, NULL, NULL);

	    if (osArea) {
		x1 = osArea->offset % width_bytes;
		x2 = (osArea->offset + osArea->size) % width_bytes;
		y1 = osArea->offset / width_bytes;
		y2 = (osArea->offset + osArea->size) / width_bytes;

		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Went swimmingly...\n");
	    }
	}
#endif

	if ((!info->useEXA && fbarea) || (info->useEXA && osArea)) {
	    /* info->backOffset = y1 * width_bytes + x1 * cpp; */
	    info->backOffset = R128_ALIGN(y1 * width_bytes + x1 * cpp, 16);
	    info->backX = info->backOffset % width_bytes;
	    info->backY = info->backOffset / width_bytes;
	    info->backPitch = pScrn->displayWidth;

	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Reserved back buffer from (%d,%d) to (%d,%d) offset: %x\n",
		       x1, y1,
		       x2, y2, info->backOffset);
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to reserve back buffer\n");
	    info->backX = -1;
	    info->backY = -1;
	    info->backOffset = -1;
	    info->backPitch = -1;
	}

				/* Allocate the shared depth buffer */
	if(!info->useEXA) {
	    fbarea = xf86AllocateOffscreenArea(pScreen,
					       pScrn->virtualX,
					       pScrn->virtualY + 1,
					       32, NULL, NULL, NULL);
	    if (fbarea) {
		x1 = fbarea->box.x1;
		x2 = fbarea->box.x2;
		y1 = fbarea->box.y1;
		y2 = fbarea->box.y2;
	    }
	}
#ifdef USE_EXA
	else {
	    osArea = exaOffscreenAlloc(pScreen,
				       (pScrn->virtualY + 1) * width_bytes,
				       32, TRUE, NULL, NULL);

	    if (osArea) {
		x1 = osArea->offset % width_bytes;
		x2 = (osArea->offset + osArea->size) % width_bytes;
		y1 = osArea->offset / width_bytes;
		y2 = (osArea->offset + osArea->size) / width_bytes;
	    }
	}
#endif

	if ((!info->useEXA && fbarea) || (info->useEXA && osArea)) {
	    /* info->depthOffset = y1 * width_bytes + x1 * cpp; */
	    info->depthOffset = R128_ALIGN(y1 * width_bytes + x1 * cpp, 16);
	    info->depthX = info->depthOffset % width_bytes;
	    info->depthY = info->depthOffset / width_bytes;
	    info->depthPitch = pScrn->displayWidth;
	    info->spanOffset = (y2 - 1) * width_bytes + x1 * cpp;

	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Reserved depth buffer from (%d,%d) to (%d,%d) offset: %x\n",
		       x1, y1,
		       x2, y2, info->depthOffset);

	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Reserved depth span from (%d,%d) offset 0x%x\n",
		       x1, y2 - 1, info->spanOffset);
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unable to reserve depth buffer\n");
	    info->depthX = -1;
	    info->depthY = -1;
	    info->depthOffset = -1;
	    info->depthPitch = -1;
	    info->spanOffset = -1;
	}

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Reserved %d kb for textures at offset 0x%x\n",
		   info->textureSize/1024, info->textureOffset);
    }
#endif /* R128DRI */

    pScrn->vtSema = TRUE;
    /* xf86CrtcRotate accesses pScrn->pScreen */
    pScrn->pScreen = pScreen;

    if (info->FBDev) {
	if (!fbdevHWModeInit(pScrn, pScrn->currentMode)) return FALSE;
    } else {
	if (!xf86SetDesiredModes(pScrn)) return FALSE;
    }

    R128SaveScreen(pScreen, SCREEN_SAVER_ON);
    //pScrn->AdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));

				/* DGA setup */
#ifdef XFreeXDGA
    xf86DiDGAInit(pScreen, info->LinearAddr + pScrn->fbOffset);
#endif

				/* Backing store setup */
    xf86SetBackingStore(pScreen);

				/* Set Silken Mouse */
    xf86SetSilkenMouse(pScreen);

				/* Cursor setup */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

				/* Hardware cursor setup */
    if (!info->swCursor) {
	if (R128CursorInit(pScreen)) {
	    int width, height;

	    if (xf86QueryLargestOffscreenArea(pScreen, &width, &height,
					      0, 0, 0)) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "Largest offscreen area available: %d x %d\n",
			   width, height);
	    }
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		       "Hardware cursor initialization failed\n");
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using software cursor\n");
	}
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Using software cursor\n");
    }

    /* DPMS setup - FIXME: also for mirror mode in non-fbdev case? - Michel */
    if (info->FBDev)
	xf86DPMSInit(pScreen, fbdevHWDPMSSetWeak(), 0);
    else
        xf86DPMSInit(pScreen, xf86DPMSSet, 0);

    R128InitVideo(pScreen);

				/* Provide SaveScreen */
    pScreen->SaveScreen  = R128SaveScreen;

				/* Wrap CloseScreen */
    info->CloseScreen    = pScreen->CloseScreen;
    pScreen->CloseScreen = R128CloseScreen;

				/* Note unused options */
    if (serverGeneration == 1)
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);

#ifdef R128DRI
				/* DRI finalization */
    if (info->directRenderingEnabled) {
				/* Now that mi, fb, drm and others have
				   done their thing, complete the DRI
				   setup. */
	info->directRenderingEnabled = R128DRIFinishScreenInit(pScreen);
    }
    if (info->directRenderingEnabled) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Direct rendering enabled\n");
    } else {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Direct rendering disabled\n");
    }
#endif

    info->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = R128BlockHandler;

    if (!xf86CrtcScreenInit(pScreen)) return FALSE;

				/* Colormap setup */
    if (!miCreateDefColormap(pScreen)) return FALSE;
    if (!xf86HandleColormaps(pScreen, 256, info->dac6bits ? 6 : 8,
			     (info->FBDev ? fbdevHWLoadPaletteWeak() :
			     R128LoadPalette), NULL,
			     CMAP_PALETTED_TRUECOLOR
			     | CMAP_RELOAD_ON_MODE_SWITCH
#if 0 /* This option messes up text mode! (eich@suse.de) */
			     | CMAP_LOAD_EVEN_IF_OFFSCREEN
#endif
			     )) return FALSE;

    return TRUE;
}

/* Write common registers (initialized to 0). */
void R128RestoreCommonRegisters(ScrnInfoPtr pScrn, R128SavePtr restore)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    OUTREG(R128_FP_GEN_CNTL, restore->fp_gen_cntl | R128_FP_BLANK_DIS);

    OUTREG(R128_OVR_CLR,              restore->ovr_clr);
    OUTREG(R128_OVR_WID_LEFT_RIGHT,   restore->ovr_wid_left_right);
    OUTREG(R128_OVR_WID_TOP_BOTTOM,   restore->ovr_wid_top_bottom);
    OUTREG(R128_OV0_SCALE_CNTL,       restore->ov0_scale_cntl);
    OUTREG(R128_MPP_TB_CONFIG,        restore->mpp_tb_config );
    OUTREG(R128_MPP_GP_CONFIG,        restore->mpp_gp_config );
    OUTREG(R128_SUBPIC_CNTL,          restore->subpic_cntl);
    OUTREG(R128_VIPH_CONTROL,         restore->viph_control);
    OUTREG(R128_I2C_CNTL_1,           restore->i2c_cntl_1);
    OUTREG(R128_GEN_INT_CNTL,         restore->gen_int_cntl);
    OUTREG(R128_CAP0_TRIG_CNTL,       restore->cap0_trig_cntl);
    OUTREG(R128_CAP1_TRIG_CNTL,       restore->cap1_trig_cntl);
    OUTREG(R128_BUS_CNTL,             restore->bus_cntl);
    OUTREG(R128_CONFIG_CNTL,          restore->config_cntl);
}

/* Write RMX registers */
void R128RestoreRMXRegisters(ScrnInfoPtr pScrn, R128SavePtr restore)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    OUTREG(R128_FP_HORZ_STRETCH,      restore->fp_horz_stretch);
    OUTREG(R128_FP_VERT_STRETCH,      restore->fp_vert_stretch);
    OUTREG(R128_FP_CRTC_H_TOTAL_DISP, restore->fp_crtc_h_total_disp);
    OUTREG(R128_FP_CRTC_V_TOTAL_DISP, restore->fp_crtc_v_total_disp);
    OUTREG(R128_FP_H_SYNC_STRT_WID,   restore->fp_h_sync_strt_wid);
    OUTREG(R128_FP_V_SYNC_STRT_WID,   restore->fp_v_sync_strt_wid);
}

/* Write flat panel registers */
void R128RestoreFPRegisters(ScrnInfoPtr pScrn, R128SavePtr restore)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    OUTREG(R128_TMDS_CRC,              restore->tmds_crc);
    OUTREG(R128_TMDS_TRANSMITTER_CNTL, restore->tmds_transmitter_cntl);
    OUTREG(R128_FP_PANEL_CNTL,         restore->fp_panel_cntl);
    OUTREG(R128_FP_GEN_CNTL, restore->fp_gen_cntl & ~(uint32_t)R128_FP_BLANK_DIS);
}

/* Write LVDS registers */
void R128RestoreLVDSRegisters(ScrnInfoPtr pScrn, R128SavePtr restore)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    R128EntPtr    pR128Ent  = R128EntPriv(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    uint32_t      tmp;

    xf86OutputPtr output = R128FirstOutput(pR128Ent->pCrtc[0]);
    R128OutputPrivatePtr r128_output = output->driver_private;

    tmp = INREG(R128_LVDS_GEN_CNTL);
    if ((tmp & (R128_LVDS_ON | R128_LVDS_BLON)) ==
	(restore->lvds_gen_cntl & (R128_LVDS_ON | R128_LVDS_BLON))) {
	OUTREG(R128_LVDS_GEN_CNTL, restore->lvds_gen_cntl);
    } else {
	if (restore->lvds_gen_cntl & (R128_LVDS_ON | R128_LVDS_BLON)) {
	    OUTREG(R128_LVDS_GEN_CNTL,
		   restore->lvds_gen_cntl & (uint32_t)~R128_LVDS_BLON);
	    usleep(r128_output->PanelPwrDly * 1000);
	    OUTREG(R128_LVDS_GEN_CNTL, restore->lvds_gen_cntl);
	} else {
	    OUTREG(R128_LVDS_GEN_CNTL, restore->lvds_gen_cntl | R128_LVDS_BLON);
	    usleep(r128_output->PanelPwrDly * 1000);
	    OUTREG(R128_LVDS_GEN_CNTL, restore->lvds_gen_cntl);
	}
    }
}

/* Write DDA registers. */
void R128RestoreDDARegisters(ScrnInfoPtr pScrn, R128SavePtr restore)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    OUTREG(R128_DDA_CONFIG, restore->dda_config);
    OUTREG(R128_DDA_ON_OFF, restore->dda_on_off);
}

/* Write DDA registers. */
void R128RestoreDDA2Registers(ScrnInfoPtr pScrn, R128SavePtr restore)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    OUTREG(R128_DDA2_CONFIG, restore->dda2_config);
    OUTREG(R128_DDA2_ON_OFF, restore->dda2_on_off);
}

/* Read common registers. */
static void R128SaveCommonRegisters(ScrnInfoPtr pScrn, R128SavePtr save)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    save->ovr_clr            = INREG(R128_OVR_CLR);
    save->ovr_wid_left_right = INREG(R128_OVR_WID_LEFT_RIGHT);
    save->ovr_wid_top_bottom = INREG(R128_OVR_WID_TOP_BOTTOM);
    save->ov0_scale_cntl     = INREG(R128_OV0_SCALE_CNTL);
    save->mpp_tb_config      = INREG(R128_MPP_TB_CONFIG);
    save->mpp_gp_config      = INREG(R128_MPP_GP_CONFIG);
    save->subpic_cntl        = INREG(R128_SUBPIC_CNTL);
    save->viph_control       = INREG(R128_VIPH_CONTROL);
    save->i2c_cntl_1         = INREG(R128_I2C_CNTL_1);
    save->gen_int_cntl       = INREG(R128_GEN_INT_CNTL);
    save->cap0_trig_cntl     = INREG(R128_CAP0_TRIG_CNTL);
    save->cap1_trig_cntl     = INREG(R128_CAP1_TRIG_CNTL);
    save->bus_cntl           = INREG(R128_BUS_CNTL);
    save->config_cntl        = INREG(R128_CONFIG_CNTL);
}

/* Read CRTC registers. */
static void R128SaveCrtcRegisters(ScrnInfoPtr pScrn, R128SavePtr save)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    save->crtc_gen_cntl        = INREG(R128_CRTC_GEN_CNTL);
    save->crtc_ext_cntl        = INREG(R128_CRTC_EXT_CNTL);
    save->dac_cntl             = INREG(R128_DAC_CNTL);
    save->crtc_h_total_disp    = INREG(R128_CRTC_H_TOTAL_DISP);
    save->crtc_h_sync_strt_wid = INREG(R128_CRTC_H_SYNC_STRT_WID);
    save->crtc_v_total_disp    = INREG(R128_CRTC_V_TOTAL_DISP);
    save->crtc_v_sync_strt_wid = INREG(R128_CRTC_V_SYNC_STRT_WID);
    save->crtc_offset          = INREG(R128_CRTC_OFFSET);
    save->crtc_offset_cntl     = INREG(R128_CRTC_OFFSET_CNTL);
    save->crtc_pitch           = INREG(R128_CRTC_PITCH);
}

/* Read flat panel registers */
static void R128SaveFPRegisters(ScrnInfoPtr pScrn, R128SavePtr save)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    save->fp_crtc_h_total_disp = INREG(R128_FP_CRTC_H_TOTAL_DISP);
    save->fp_crtc_v_total_disp = INREG(R128_FP_CRTC_V_TOTAL_DISP);
    save->fp_gen_cntl          = INREG(R128_FP_GEN_CNTL);
    save->fp_h_sync_strt_wid   = INREG(R128_FP_H_SYNC_STRT_WID);
    save->fp_horz_stretch      = INREG(R128_FP_HORZ_STRETCH);
    save->fp_panel_cntl        = INREG(R128_FP_PANEL_CNTL);
    save->fp_v_sync_strt_wid   = INREG(R128_FP_V_SYNC_STRT_WID);
    save->fp_vert_stretch      = INREG(R128_FP_VERT_STRETCH);
    save->lvds_gen_cntl        = INREG(R128_LVDS_GEN_CNTL);
    save->tmds_crc             = INREG(R128_TMDS_CRC);
    save->tmds_transmitter_cntl = INREG(R128_TMDS_TRANSMITTER_CNTL);
}

/* Read CRTC2 registers. */
static void R128SaveCrtc2Registers(ScrnInfoPtr pScrn, R128SavePtr save)
{
    R128InfoPtr info        = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    save->crtc2_gen_cntl        = INREG(R128_CRTC2_GEN_CNTL);
    save->crtc2_h_total_disp    = INREG(R128_CRTC2_H_TOTAL_DISP);
    save->crtc2_h_sync_strt_wid = INREG(R128_CRTC2_H_SYNC_STRT_WID);
    save->crtc2_v_total_disp    = INREG(R128_CRTC2_V_TOTAL_DISP);
    save->crtc2_v_sync_strt_wid = INREG(R128_CRTC2_V_SYNC_STRT_WID);
    save->crtc2_offset          = INREG(R128_CRTC2_OFFSET);
    save->crtc2_offset_cntl     = INREG(R128_CRTC2_OFFSET_CNTL);
    save->crtc2_pitch           = INREG(R128_CRTC2_PITCH);
}

/* Read PLL registers. */
static void R128SavePLLRegisters(ScrnInfoPtr pScrn, R128SavePtr save)
{
    save->ppll_ref_div         = INPLL(pScrn, R128_PPLL_REF_DIV);
    save->ppll_div_3           = INPLL(pScrn, R128_PPLL_DIV_3);
    save->ppll_div_0           = INPLL(pScrn, R128_PPLL_DIV_0);
    save->htotal_cntl          = INPLL(pScrn, R128_HTOTAL_CNTL);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Read: 0x%08x 0x%08x 0x%08x\n",
                        save->ppll_ref_div,
                        save->ppll_div_3,
                        save->htotal_cntl));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Read: rd=%d, fd=%d, pd=%d\n",
                        save->ppll_ref_div & R128_PPLL_REF_DIV_MASK,
                        save->ppll_div_3 & R128_PPLL_FB3_DIV_MASK,
                        (save->ppll_div_3 &
                                R128_PPLL_POST3_DIV_MASK) >> 16));
}

/* Read PLL2 registers. */
static void R128SavePLL2Registers(ScrnInfoPtr pScrn, R128SavePtr save)
{
    save->p2pll_ref_div        = INPLL(pScrn, R128_P2PLL_REF_DIV);
    save->p2pll_div_0          = INPLL(pScrn, R128_P2PLL_DIV_0);
    save->htotal_cntl2         = INPLL(pScrn, R128_HTOTAL2_CNTL);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Read: 0x%08x 0x%08x 0x%08x\n",
                        save->p2pll_ref_div,
                        save->p2pll_div_0,
                        save->htotal_cntl2));
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Read: rd=%d, fd=%d, pd=%d\n",
                        save->p2pll_ref_div & R128_P2PLL_REF_DIV_MASK,
                        save->p2pll_div_0 & R128_P2PLL_FB0_DIV_MASK,
                        (save->p2pll_div_0 &
                                R128_P2PLL_POST0_DIV_MASK) >> 16));
}

/* Read DDA registers. */
static void R128SaveDDARegisters(ScrnInfoPtr pScrn, R128SavePtr save)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    save->dda_config           = INREG(R128_DDA_CONFIG);
    save->dda_on_off           = INREG(R128_DDA_ON_OFF);
}

/* Read DDA2 registers. */
static void R128SaveDDA2Registers(ScrnInfoPtr pScrn, R128SavePtr save)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;

    save->dda2_config           = INREG(R128_DDA2_CONFIG);
    save->dda2_on_off           = INREG(R128_DDA2_ON_OFF);
}

/* Read palette data. */
static void R128SavePalette(ScrnInfoPtr pScrn, R128SavePtr save)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    int           i;

    PAL_SELECT(1);
    INPAL_START(0);
    for (i = 0; i < 256; i++) save->palette2[i] = INPAL_NEXT();
    PAL_SELECT(0);
    INPAL_START(0);
    for (i = 0; i < 256; i++) save->palette[i] = INPAL_NEXT();
    save->palette_valid = TRUE;
}

/* Save state that defines current video mode. */
static void R128SaveMode(ScrnInfoPtr pScrn, R128SavePtr save)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    R128EntPtr    pR128Ent  = R128EntPriv(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "%s(%p)\n", __func__, save));

    R128SaveCommonRegisters(pScrn, save);
    R128SaveCrtcRegisters(pScrn, save);
    R128SavePLLRegisters(pScrn, save);
    R128SaveDDARegisters(pScrn, save);
    if (pR128Ent->HasCRTC2) {
        R128SaveCrtc2Registers(pScrn, save);
        R128SavePLL2Registers(pScrn, save);
        R128SaveDDA2Registers(pScrn, save);
    }
    if (info->HasPanelRegs) {
        R128SaveFPRegisters(pScrn, save);
    }
    R128SavePalette(pScrn, save);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "%s returns %p\n", __func__, save));
}

/* Save everything needed to restore the original VC state. */
static void R128Save(ScrnInfoPtr pScrn)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    R128SavePtr   save      = &info->SavedReg;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "%s\n", __func__));
    if (info->FBDev) {
	fbdevHWSave(pScrn);
	return;
    }

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

    save->dp_datatype      = INREG(R128_DP_DATATYPE);
    save->gen_reset_cntl   = INREG(R128_GEN_RESET_CNTL);
    save->clock_cntl_index = INREG(R128_CLOCK_CNTL_INDEX);
    save->amcgpio_en_reg   = INREG(R128_AMCGPIO_EN_REG);
    save->amcgpio_mask     = INREG(R128_AMCGPIO_MASK);

    R128SaveMode(pScrn, save);
}

/* Restore the original (text) mode. */
static void R128Restore(ScrnInfoPtr pScrn)
{
    R128InfoPtr   info      = R128PTR(pScrn);
    R128EntPtr    pR128Ent  = R128EntPriv(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    R128SavePtr   restore   = &info->SavedReg;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "%s\n", __func__));
    if (info->FBDev) {
	fbdevHWRestore(pScrn);
	return;
    }

    R128Blank(pScrn);

    R128RestoreCommonRegisters(pScrn, restore);
    if (pR128Ent->HasCRTC2) {
        R128RestoreDDA2Registers(pScrn, restore);
        R128RestoreCrtc2Registers(pScrn, restore);
        R128RestorePLL2Registers(pScrn, restore);
    }
    R128RestoreDDARegisters(pScrn, restore);
    R128RestoreCrtcRegisters(pScrn, restore);
    R128RestorePLLRegisters(pScrn, restore);
    R128RestoreDACRegisters(pScrn, restore);
    R128RestoreRMXRegisters(pScrn, restore);
    R128RestoreFPRegisters(pScrn, restore);
    R128RestoreLVDSRegisters(pScrn, restore);

    OUTREG(R128_AMCGPIO_MASK,     restore->amcgpio_mask);
    OUTREG(R128_AMCGPIO_EN_REG,   restore->amcgpio_en_reg);
    OUTREG(R128_CLOCK_CNTL_INDEX, restore->clock_cntl_index);
    OUTREG(R128_GEN_RESET_CNTL,   restore->gen_reset_cntl);
    OUTREG(R128_DP_DATATYPE,      restore->dp_datatype);

#ifdef WITH_VGAHW
    if (info->VGAAccess) {
        vgaHWPtr hwp = VGAHWPTR(pScrn);
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
    }
#endif

    R128WaitForVerticalSync(pScrn);
    R128Unblank(pScrn);
}

/* Define common registers for requested video mode. */
void R128InitCommonRegisters(R128SavePtr save, R128InfoPtr info)
{
    save->ovr_clr            = 0;
    save->ovr_wid_left_right = 0;
    save->ovr_wid_top_bottom = 0;
    save->ov0_scale_cntl     = 0;
    save->mpp_tb_config      = 0;
    save->mpp_gp_config      = 0;
    save->subpic_cntl        = 0;
    save->viph_control       = 0;
    save->i2c_cntl_1         = 0;
#ifdef R128DRI
    save->gen_int_cntl       = info->gen_int_cntl;
#else
    save->gen_int_cntl       = 0;
#endif
    save->cap0_trig_cntl     = 0;
    save->cap1_trig_cntl     = 0;
    save->bus_cntl           = info->BusCntl;
    /*
     * If bursts are enabled, turn on discards and aborts
     */
    if (save->bus_cntl & (R128_BUS_WRT_BURST|R128_BUS_READ_BURST))
	save->bus_cntl |= R128_BUS_RD_DISCARD_EN | R128_BUS_RD_ABORT_EN;
}

/* Define RMX registers for the requested video mode. */
void R128InitRMXRegisters(R128SavePtr orig, R128SavePtr save,
                          xf86OutputPtr output, DisplayModePtr mode)
{
    R128OutputPrivatePtr r128_output = output->driver_private;

    int   xres = mode->CrtcHDisplay;
    int   yres = mode->CrtcVDisplay;
    float Hratio, Vratio;

    save->fp_crtc_h_total_disp = save->crtc_h_total_disp;
    save->fp_crtc_v_total_disp = save->crtc_v_total_disp;
    save->fp_h_sync_strt_wid   = save->crtc_h_sync_strt_wid;
    save->fp_v_sync_strt_wid   = save->crtc_v_sync_strt_wid;

    if (r128_output->MonType != MT_DFP && r128_output->MonType != MT_LCD)
        return;

    if (r128_output->PanelXRes == 0 || r128_output->PanelYRes == 0) {
        xres = r128_output->PanelXRes;
        yres = r128_output->PanelYRes;

        Hratio = 1.0;
        Vratio = 1.0;
    } else {
        if (xres > r128_output->PanelXRes) xres = r128_output->PanelXRes;
        if (yres > r128_output->PanelYRes) yres = r128_output->PanelYRes;

        Hratio = (float)xres/(float)r128_output->PanelXRes;
        Vratio = (float)yres/(float)r128_output->PanelYRes;
    }

    save->fp_horz_stretch =
	(((((int)(Hratio * R128_HORZ_STRETCH_RATIO_MAX + 0.5))
	   & R128_HORZ_STRETCH_RATIO_MASK) << R128_HORZ_STRETCH_RATIO_SHIFT) |
       (orig->fp_horz_stretch & (R128_HORZ_PANEL_SIZE |
                                 R128_HORZ_FP_LOOP_STRETCH |
                                 R128_HORZ_STRETCH_RESERVED)));
    save->fp_horz_stretch &= ~R128_HORZ_AUTO_RATIO_FIX_EN;
    save->fp_horz_stretch &= ~R128_AUTO_HORZ_RATIO;
    if (xres == r128_output->PanelXRes)
         save->fp_horz_stretch &= ~(R128_HORZ_STRETCH_BLEND | R128_HORZ_STRETCH_ENABLE);
    else
         save->fp_horz_stretch |=  (R128_HORZ_STRETCH_BLEND | R128_HORZ_STRETCH_ENABLE);

    save->fp_vert_stretch =
	(((((int)(Vratio * R128_VERT_STRETCH_RATIO_MAX + 0.5))
	   & R128_VERT_STRETCH_RATIO_MASK) << R128_VERT_STRETCH_RATIO_SHIFT) |
	 (orig->fp_vert_stretch & (R128_VERT_PANEL_SIZE |
				   R128_VERT_STRETCH_RESERVED)));
    save->fp_vert_stretch &= ~R128_VERT_AUTO_RATIO_EN;
    if (yres == r128_output->PanelYRes)
        save->fp_vert_stretch &= ~(R128_VERT_STRETCH_ENABLE | R128_VERT_STRETCH_BLEND);
    else
        save->fp_vert_stretch |=  (R128_VERT_STRETCH_ENABLE | R128_VERT_STRETCH_BLEND);
}

/* Define flat panel registers for the requested video mode. */
void R128InitFPRegisters(R128SavePtr orig, R128SavePtr save, xf86OutputPtr output)
{
    xf86CrtcPtr crtc = output->crtc;
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;

    /* WARNING: Be careful about turning on the flat panel */
    save->fp_gen_cntl            = orig->fp_gen_cntl;
    save->fp_panel_cntl          = orig->fp_panel_cntl;
    save->tmds_transmitter_cntl  = orig->tmds_transmitter_cntl;
    save->tmds_crc               = orig->tmds_crc;

    if (r128_crtc->crtc_id)
        save->fp_gen_cntl       |=   R128_FP_SEL_CRTC2;
    else
        save->fp_gen_cntl       &=  ~R128_FP_SEL_CRTC2;

    save->fp_gen_cntl           &= ~(R128_FP_CRTC_USE_SHADOW_VEND |
                                     R128_FP_CRTC_USE_SHADOW_ROWCUR |
                                     R128_FP_CRTC_HORZ_DIV2_EN |
                                     R128_FP_CRTC_HOR_CRT_DIV2_DIS |
                                     R128_FP_CRT_SYNC_SEL |
                                     R128_FP_USE_SHADOW_EN);

    save->fp_gen_cntl           |=  (R128_FP_CRTC_DONT_SHADOW_VPAR |
                                     R128_FP_CRTC_DONT_SHADOW_HEND);

    save->fp_panel_cntl         |=  (R128_FP_DIGON | R128_FP_BLON);
    save->tmds_transmitter_cntl &=  ~R128_TMDS_PLLRST;
    save->tmds_transmitter_cntl |=   R128_TMDS_PLLEN;
}

/* Define LVDS registers for the requested video mode. */
void R128InitLVDSRegisters(R128SavePtr orig, R128SavePtr save, xf86OutputPtr output)
{
    xf86CrtcPtr crtc = output->crtc;
    R128CrtcPrivatePtr r128_crtc = crtc->driver_private;

    save->lvds_gen_cntl      =  orig->lvds_gen_cntl;

    if (r128_crtc->crtc_id)
        save->lvds_gen_cntl |=  R128_LVDS_SEL_CRTC2;
    else
        save->lvds_gen_cntl &= ~R128_LVDS_SEL_CRTC2;
}

#if 0
/* Define initial palette for requested video mode.  This doesn't do
   anything for XFree86 4.0. */
static void R128InitPalette(R128SavePtr save)
{
    save->palette_valid = FALSE;
}
#endif

static Bool R128SaveScreen(ScreenPtr pScreen, int mode)
{
    ScrnInfoPtr   pScrn = xf86ScreenToScrn(pScreen);
    Bool unblank;

    unblank = xf86IsUnblank(mode);
    if (unblank)
	SetTimeSinceLastInputEvent();

    if ((pScrn != NULL) && pScrn->vtSema) {
	if (unblank)
		R128Unblank(pScrn);
	else
		R128Blank(pScrn);
    }
    return TRUE;
}

/*
 * SwitchMode() doesn't work right on crtc2 on some laptops.
 * The workaround is to switch the mode, then switch to another VT, then
 * switch back. --AGD
 */
Bool R128SwitchMode(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    R128InfoPtr info        = R128PTR(pScrn);
    Bool ret;

    info->SwitchingMode = TRUE;
    ret = xf86SetSingleMode(pScrn, mode, RR_Rotate_0);
    info->SwitchingMode = FALSE;
    return ret;
}

ModeStatus R128DoValidMode(xf86OutputPtr output, DisplayModePtr mode, int flags)
{
    ScrnInfoPtr pScrn = output->scrn;
    R128InfoPtr info  = R128PTR(pScrn);
    R128OutputPrivatePtr r128_output = output->driver_private;
    int i, j;

    if (r128_output->MonType == MT_CRT)
        return MODE_OK;

    if (r128_output->MonType == MT_DFP || r128_output->MonType == MT_LCD) {
	if (mode->Flags & V_INTERLACE) return MODE_NO_INTERLACE;
	if (mode->Flags & V_DBLSCAN)   return MODE_NO_DBLESCAN;
    }

    if (r128_output->MonType == MT_LCD && info->VBIOS) {
	for (i = info->FPBIOSstart + 64; R128_BIOS16(i) != 0; i += 2) {
	    j = R128_BIOS16(i);

	    if (mode->CrtcHDisplay == R128_BIOS16(j) &&
		mode->CrtcVDisplay == R128_BIOS16(j + 2)) {
		if ((flags & MODECHECK_FINAL) == MODECHECK_FINAL) {
		    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			       "Modifying mode according to VBIOS: %ix%i [pclk %.1f MHz] for FP to: ",
			       mode->CrtcHDisplay, mode->CrtcVDisplay,
			       (float)mode->Clock / 1000);

		    /* Assume we are using expanded mode */
		    if (R128_BIOS16(j + 5)) j  = R128_BIOS16(j + 5);
		    else                    j += 9;

		    mode->Clock = (uint32_t)R128_BIOS16(j) * 10;

		    mode->HDisplay   = mode->CrtcHDisplay   =
			((R128_BIOS16(j + 10) & 0x01ff) + 1) * 8;
		    mode->HSyncStart = mode->CrtcHSyncStart =
			((R128_BIOS16(j + 12) & 0x01ff) + 1) * 8;
		    mode->HSyncEnd   = mode->CrtcHSyncEnd   =
			mode->CrtcHSyncStart + (R128_BIOS8(j + 14) & 0x1f);
		    mode->HTotal     = mode->CrtcHTotal     =
			((R128_BIOS16(j + 8)  & 0x01ff) + 1) * 8;

		    mode->VDisplay   = mode->CrtcVDisplay   =
			(R128_BIOS16(j + 17) & 0x07ff) + 1;
		    mode->VSyncStart = mode->CrtcVSyncStart =
			(R128_BIOS16(j + 19) & 0x07ff) + 1;
		    mode->VSyncEnd   = mode->CrtcVSyncEnd   =
			mode->CrtcVSyncStart + ((R128_BIOS16(j + 19) >> 11) & 0x1f);
		    mode->VTotal     = mode->CrtcVTotal     =
			(R128_BIOS16(j + 15) & 0x07ff) + 1;
		    xf86ErrorF("%ix%i [pclk %.1f MHz]\n",
			       mode->CrtcHDisplay,mode->CrtcVDisplay,
			       (float)mode->Clock/ 1000);
		}
		return MODE_OK;
	    }
	}
	xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 5,
		       "Mode rejected for FP %ix%i [pclk: %.1f] "
		       "(not listed in VBIOS)\n",
		       mode->CrtcHDisplay, mode->CrtcVDisplay,
		       (float)mode->Clock / 1000);
	return MODE_NOMODE;
    }

    return MODE_OK;
}

/* Used to disallow modes that are not supported by the hardware. */
ModeStatus R128ValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode,
                                   Bool verbose, int flags)
{
    SCRN_INFO_PTR(arg);
    R128EntPtr  pR128Ent = R128EntPriv(pScrn);
    xf86OutputPtr output = R128FirstOutput(pR128Ent->pCrtc[0]);

    return R128DoValidMode(output, mode, flags);
}

/* Adjust viewport into virtual desktop such that (0,0) in viewport space
   is (x,y) in virtual space. */
void R128AdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    R128InfoPtr   info      = R128PTR(pScrn);
    unsigned char *R128MMIO = info->MMIO;
    int           Base;

    if(info->showCache && y && pScrn->vtSema)
        y += pScrn->virtualY - 1;

    Base = y * info->CurrentLayout.displayWidth + x;

    switch (info->CurrentLayout.pixel_code) {
    case 15:
    case 16: Base *= 2; break;
    case 24: Base *= 3; break;
    case 32: Base *= 4; break;
    }

    Base &= ~7;                 /* 3 lower bits are always 0 */

    if (info->CurrentLayout.pixel_code == 24)
	Base += 8 * (Base % 3); /* Must be multiple of 8 and 3 */

    OUTREG(R128_CRTC_OFFSET, Base);
}

/* Called when VT switching back to the X server.  Reinitialize the video
   mode. */
Bool R128EnterVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    R128InfoPtr info  = R128PTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "%s\n", __func__));

    pScrn->vtSema = TRUE;
    if (info->FBDev) {
        if (!fbdevHWEnterVT(VT_FUNC_ARGS)) return FALSE;
    } else {
        if (!xf86SetDesiredModes(pScrn)) return FALSE;
    }

    //if (!R128ModeInit(pScrn, pScrn->currentMode)) return FALSE;

    if (info->accelOn)
	R128EngineInit(pScrn);

#ifdef R128DRI
    if (info->directRenderingEnabled) {
	if (info->irq) {
	    /* Need to make sure interrupts are enabled */
	    unsigned char *R128MMIO = info->MMIO;
	    OUTREG(R128_GEN_INT_CNTL, info->gen_int_cntl);
	}
	R128CCE_START(pScrn, info);
	DRIUnlock(pScrn->pScreen);
    }
#endif

    info->PaletteSavedOnVT = FALSE;
    //pScrn->AdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));

    return TRUE;
}

/* Called when VT switching away from the X server.  Restore the original
   text mode. */
void R128LeaveVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    R128InfoPtr info  = R128PTR(pScrn);
    R128SavePtr save  = &info->ModeReg;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "%s\n", __func__));
#ifdef R128DRI
    if (info->directRenderingEnabled) {
	DRILock(pScrn->pScreen, 0);
	R128CCE_STOP(pScrn, info);
    }
#ifdef USE_EXA
    if (info->useEXA)
        info->state_2d.composite_setup = FALSE;
#endif
#endif
    R128SavePalette(pScrn, save);
    info->PaletteSavedOnVT = TRUE;
    if (info->FBDev)
        fbdevHWLeaveVT(VT_FUNC_ARGS);
    else
        R128Restore(pScrn);
}


/* Called at the end of each server generation.  Restore the original text
   mode, unmap video memory, and unwrap and call the saved CloseScreen
   function.  */
static Bool R128CloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    R128InfoPtr info  = R128PTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "%s\n", __func__));

#ifdef R128DRI
				/* Disable direct rendering */
    if (info->directRenderingEnabled) {
	R128DRICloseScreen(pScreen);
	info->directRenderingEnabled = FALSE;
    }
#endif

    if (pScrn->vtSema) {
	R128Restore(pScrn);
	R128UnmapMem(pScrn);
    }

#ifdef USE_EXA
        if (info->useEXA) {
	    exaDriverFini(pScreen);
	    free(info->ExaDriver);
	} else
#endif
#ifdef HAVE_XAA_H
	{
            if (info->accel)             XAADestroyInfoRec(info->accel);
	    info->accel                  = NULL;
        }
#endif

    if (info->scratch_save)      free(info->scratch_save);
    info->scratch_save           = NULL;

    if (info->adaptor) {
        free(info->adaptor->pPortPrivates[0].ptr);
	xf86XVFreeVideoAdaptorRec(info->adaptor);
	info->adaptor = NULL;
    }

    pScrn->vtSema = FALSE;

    pScreen->BlockHandler = info->BlockHandler;
    pScreen->CloseScreen = info->CloseScreen;
    return (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);
}

void R128FreeScreen(FREE_SCREEN_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    R128InfoPtr   info      = R128PTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "%s\n", __func__));
    if (info == NULL)
	return;
#ifdef WITH_VGAHW
    if (info->VGAAccess && xf86LoaderCheckSymbol("vgaHWFreeHWRec"))
	vgaHWFreeHWRec(pScrn);
#endif
    R128FreeRec(pScrn);
}
