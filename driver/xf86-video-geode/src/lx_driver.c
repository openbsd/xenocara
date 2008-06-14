/* Copyright (c) 2003-2007 Advanced Micro Devices, Inc.
 *
 * Portioned modeled from xf86-video-intel/src/i830_driver.c
 * Copyright 2001 VA Linux Systems Inc., Fremont, California.
 * Copyright \ufffd 2002 by David Dawes

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Neither the name of the Advanced Micro Devices, Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 */

/* TODO:
   TV out support
   Detect panels better
   Better VGA support
   GX:  cursor position needs to be correctly set
   use CB data wrapper to save a variable
   consolidate the saved timings
   implement panning
*/

/* The effort to make things common:
   define CmdBfrSize in the GX
   add the output flag to GX
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Resources.h"
#include "xf86cmap.h"
#include "compiler.h"
#include "mipointer.h"
#include <shadow.h>		       /* setupShadow() */
#include <X11/extensions/randr.h>
#include "fb.h"
#include "miscstruct.h"
#include "micmap.h"
#include "vbe.h"
#include "fb.h"
#include "randrstr.h"
#include "cim_defs.h"
#include "cim_regs.h"
#include "geode.h"

/* Bring in VGA functions */
#include "lx_vga.c"

/* Chipset types */

#define LX_MIN_PITCH 1024
#define LX_MAX_PITCH 8192
#define LX_MAX_WIDTH  1940
#define LX_MIN_HEIGHT 400
#define LX_MAX_HEIGHT 1600
#define LX_CB_PITCH   544
#define LX_CB_SIZE    544

#define LX_GP_REG_SIZE  0x4000
#define LX_VG_REG_SIZE  0x4000
#define LX_VID_REG_SIZE 0x4000
#define LX_VIP_REG_SIZE 0x4000

extern OptionInfoRec LX_GeodeOptions[];

extern const char *amdVgahwSymbols[];
extern const char *amdVbeSymbols[];
extern const char *amdInt10Symbols[];
extern const char *amdFbSymbols[];
extern const char *amdExaSymbols[];
extern const char *amdRamdacSymbols[];

unsigned char *XpressROMPtr;

/* Reference: Video Graphics Suite Specification:
 * VG Config Register (0x00) page 16
 * VG FP Register (0x02) page 18
 */

#define LX_READ_VG(reg) \
                (outw(0xAC1C,0xFC53), outw(0xAC1C,0x0200|(reg)), inw(0xAC1E))

static inline void
lx_enable_dac_power(ScrnInfoPtr pScrni, int option)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);

    df_set_crt_enable(DF_CRT_ENABLE);

    /* Turn off the DAC if we don't need the CRT */

    if (option && (!(pGeode->Output & OUTPUT_CRT))) {
	unsigned int misc = READ_VID32(DF_VID_MISC);

	misc |= DF_DAC_POWER_DOWN;
	WRITE_VID32(DF_VID_MISC, misc);
    }

    if (pGeode->Output & OUTPUT_PANEL)
	df_set_panel_enable(1);
}

static inline void
lx_disable_dac_power(ScrnInfoPtr pScrni, int option)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);

    if (pGeode->Output & OUTPUT_PANEL)
	df_set_panel_enable(0);

    if (pGeode->Output & OUTPUT_CRT) {

	/* Wait for the panel to finish its procedure */

	if (pGeode->Output & OUTPUT_PANEL)
	    while ((READ_VID32(DF_POWER_MANAGEMENT) & 2) == 0) ;
	df_set_crt_enable(option);
    }
}

static int
lx_get_panel(int *xres, int *yres)
{
    static struct
    {
	int xres, yres;
    } fpres[] = {
	{
	320, 240}, {
	640, 480}, {
	800, 600}, {
	1024, 768}, {
	1152, 864}, {
	1280, 1024}, {
    1600, 1200}};

    unsigned short reg = LX_READ_VG(0x00);
    unsigned char ret = (reg >> 8) & 0x07;

    if ((ret == 1 || ret == 5)) {

	reg = LX_READ_VG(0x02);
	ret = (reg >> 3) & 0x07;

	/* 7 is a "reserved" value - if we get it, we can only assume that
	 * a panel doesn't exist (or it hasn't been configured in the BIOS)
	 */

	if (ret < 7) {
	    *xres = fpres[ret].xres;
	    *yres = fpres[ret].yres;

	    return TRUE;
	}
    }

    return FALSE;
}

static int
lx_set_custom_mode(GeodeRec * pGeode, DisplayModePtr pMode, int bpp)
{
    VG_DISPLAY_MODE mode;
    int hsync, vsync;

    memset(&mode, 0, sizeof(mode));

    /* Cimarron purposely swaps the sync when panels are enabled -this is
     * presumably to allow for "default" panels which are normally active
     * low, so we need to swizzle the flags
     */

    hsync = (pMode->Flags & V_NHSYNC) ? 1 : 0;
    vsync = (pMode->Flags & V_NVSYNC) ? 1 : 0;

    if (pGeode->Output & OUTPUT_PANEL) {
	hsync = !vsync;
	vsync = !vsync;
    }

    mode.flags |= (hsync) ? VG_MODEFLAG_NEG_HSYNC : 0;
    mode.flags |= (vsync) ? VG_MODEFLAG_NEG_VSYNC : 0;

    mode.flags |= pGeode->Output & OUTPUT_CRT ? VG_MODEFLAG_CRT_AND_FP : 0;

    if (pGeode->Output & OUTPUT_PANEL) {
	mode.panel_width = mode.mode_width = pGeode->PanelX;
	mode.panel_height = mode.mode_height = pGeode->PanelY;

	mode.flags |= VG_MODEFLAG_PANELOUT;
	mode.flags |=
	    pGeode->Output & OUTPUT_CRT ? VG_MODEFLAG_CRT_AND_FP : 0;
    } else {
	mode.mode_width = pMode->CrtcHDisplay;
	mode.mode_height = pMode->CrtcVDisplay;
    }

    mode.src_width = pMode->CrtcHDisplay;
    mode.src_height = pMode->CrtcVDisplay;

    mode.hactive = pMode->CrtcHDisplay;
    mode.hblankstart = pMode->CrtcHBlankStart;
    mode.hsyncstart = pMode->CrtcHSyncStart;
    mode.hsyncend = pMode->CrtcHSyncEnd;
    mode.hblankend = pMode->CrtcHBlankEnd;
    mode.htotal = pMode->CrtcHTotal;

    mode.vactive = pMode->CrtcVDisplay;
    mode.vblankstart = pMode->CrtcVBlankStart;
    mode.vsyncstart = pMode->CrtcVSyncStart;
    mode.vsyncend = pMode->CrtcVSyncEnd;
    mode.vblankend = pMode->CrtcVBlankEnd;
    mode.vtotal = pMode->CrtcVTotal;

    mode.vactive_even = pMode->CrtcVDisplay;
    mode.vblankstart_even = pMode->CrtcVBlankStart;
    mode.vsyncstart_even = pMode->CrtcVSyncStart;
    mode.vsyncend_even = pMode->CrtcVSyncEnd;
    mode.vblankend_even = pMode->CrtcVBlankEnd;
    mode.vtotal_even = pMode->CrtcVTotal;

    mode.frequency = (int)((pMode->SynthClock / 1000.0) * 0x10000);

    return vg_set_custom_mode(&mode, bpp);
}

static Bool
LXAllocateMemory(ScreenPtr pScrn, ScrnInfoPtr pScrni, int rotate)
{
    GeodePtr pGeode = GEODEPTR(pScrni);

    unsigned int fboffset, fbavail;
    unsigned int size;
    unsigned int bytpp = (pScrni->bitsPerPixel + 7) / 8;
    Bool ret = TRUE;

    if (pGeode->tryCompression)
	pGeode->displayPitch =
	    GeodeCalculatePitchBytes(pScrni->virtualX, pScrni->bitsPerPixel);
    else
	pGeode->displayPitch =
	    ((pScrni->virtualX + 3) & ~3) * (pScrni->bitsPerPixel >> 3);

    pGeode->displayWidth = pGeode->displayPitch / bytpp;

    /* Sets pGeode->Pitch and pScrni->displayWidth based on the rotate settings */
    LXSetRotatePitch(pScrni);

    fbavail = pGeode->FBAvail - GP3_SCRATCH_BUFFER_SIZE;

    pGeode->displayOffset = fboffset = 0;
    pGeode->displaySize = pScrni->virtualY * pGeode->displayPitch;

    fbavail -= pGeode->displaySize;
    fboffset += pGeode->displaySize;

    if (pGeode->tryCompression) {
	size = pScrni->virtualY * LX_CB_PITCH;

	if (size <= fbavail) {
	    pGeode->CBData.compression_offset = fboffset;
	    pGeode->CBData.size = LX_CB_PITCH;
	    pGeode->CBData.pitch = LX_CB_PITCH;
	    fboffset += size;
	    fbavail -= size;

	    pGeode->Compression = TRUE;
	} else {
	    xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
		"Not enough memory for compression\n");
	    pGeode->Compression = FALSE;
	}
    }

    if (pGeode->tryHWCursor) {
	pGeode->CursorSize = 1024;

	if (pGeode->CursorSize <= fbavail) {
	    pGeode->CursorStartOffset = fboffset;
	    fboffset += pGeode->CursorSize;
	    fbavail -= pGeode->CursorSize;
	    pGeode->HWCursor = TRUE;
	} else {
	    xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
		"Not enough memory for the hardware cursor\n");
	    pGeode->HWCursor = FALSE;
	}
    }

    /* Try to set up some EXA scratch memory for blending */

    pGeode->exaBfrOffset = 0;

    if (!pGeode->NoAccel) {
	if (pGeode->exaBfrSz > 0 && pGeode->exaBfrSz <= fbavail) {
	    pGeode->exaBfrOffset = fboffset;
	    fboffset += pGeode->exaBfrSz;
	    fbavail -= pGeode->exaBfrSz;
	}
    }

    /* Adjust the available EXA offscreen space to account for the buffer */

    if (!pGeode->NoAccel && pGeode->pExa) {
	pGeode->pExa->offScreenBase = fboffset;
	pGeode->pExa->memorySize = fboffset + fbavail;
    }

    return ret;
}

static Bool
LXSaveScreen(ScreenPtr pScrn, int mode)
{
    ScrnInfoPtr pScrni = xf86Screens[pScrn->myNum];
    GeodePtr pGeode = GEODEPTR(pScrni);

    if (pGeode->useVGA && !pScrni->vtSema)
	return vgaHWSaveScreen(pScrn, mode);

    return TRUE;
}

/* This is an overly complex MSR read mechanism */

/* From Cimarron - the VSAII read/write methods - we use these as fallback */

#define LX_MSR_READ(adr,lo,hi)              \
     __asm__ __volatile__(                      \
        " mov $0x0AC1C, %%edx\n"                \
        " mov $0xFC530007, %%eax\n"             \
        " out %%eax,%%dx\n"                     \
        " add $2,%%dl\n"                        \
        " in %%dx, %%ax"                        \
        : "=a" (lo), "=d" (hi)                  \
        : "c" (adr))

#define LX_MSR_WRITE(adr,low,high) \
  { int d0, d1, d2, d3, d4;        \
  __asm__ __volatile__(            \
    " push %%ebx\n"                \
    " mov $0x0AC1C, %%edx\n"       \
    " mov $0xFC530007, %%eax\n"    \
    " out %%eax,%%dx\n"            \
    " add $2,%%dl\n"               \
    " mov %6, %%ebx\n"             \
    " mov %7, %0\n"                \
    " mov %5, %3\n"                \
    " xor %2, %2\n"                \
    " xor %1, %1\n"                \
    " out %%ax, %%dx\n"            \
    " pop %%ebx\n"                 \
    : "=a"(d0),"=&D"(d1),"=&S"(d2), \
      "=c"(d3),"=d"(d4)  \
    : "1"(adr),"2"(high),"3"(low)); \
  }

static void
LXReadMSR(unsigned long addr, unsigned long *lo, unsigned long *hi)
{
    if (GeodeReadMSR(addr, lo, hi) == -1) {
	unsigned int l, h;

	LX_MSR_READ(addr, l, h);
	*lo = l;
	*hi = h;
    }
}

static void
LXWriteMSR(unsigned long addr, unsigned long lo, unsigned long hi)
{
    if (GeodeWriteMSR(addr, lo, hi) == -1)
	LX_MSR_WRITE(addr, lo, hi);
}

#ifdef XSERVER_LIBPCIACCESS
static inline void *
map_pci_mem(ScrnInfoPtr pScrni, int vram,
    struct pci_device *dev, int bar, int size)
{
    void *ptr;
    void **result = (void **)&ptr;
    int map_size = size ? size : dev->regions[bar].size;

    int err = pci_device_map_range(dev,
	dev->regions[bar].base_addr,
	map_size,
	PCI_DEV_MAP_FLAG_WRITABLE |
	(vram ? PCI_DEV_MAP_FLAG_WRITE_COMBINE : 0),
	result);

    if (err)
	return NULL;
    return ptr;
}

static inline int
unmap_pci_mem(ScrnInfoPtr pScrni, struct pci_device *dev, void *ptr, int size)
{
       return pci_device_unmap_range(dev, ptr, size);
}

#endif

static Bool
LXMapMem(ScrnInfoPtr pScrni)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);
    int index = pScrni->scrnIndex;
    unsigned long cmd_bfr_phys;

    pciVideoPtr pci = xf86GetPciInfoForEntity(pGeode->pEnt->index);

#ifndef XSERVER_LIBPCIACCESS
    PCITAG tag;

    tag = pciTag(pci->bus, pci->device, pci->func);

    cim_gp_ptr = (unsigned char *)xf86MapPciMem(index, VIDMEM_MMIO,
	tag, pci->memBase[1], LX_GP_REG_SIZE);

    cim_vg_ptr = (unsigned char *)xf86MapPciMem(index, VIDMEM_MMIO,
	tag, pci->memBase[2], LX_VG_REG_SIZE);

    cim_vid_ptr = (unsigned char *)xf86MapPciMem(index, VIDMEM_MMIO,
	tag, pci->memBase[3], LX_VID_REG_SIZE);

    cim_vip_ptr = (unsigned char *)xf86MapPciMem(index, VIDMEM_MMIO,
	tag, pci->memBase[4], LX_VIP_REG_SIZE);

    cim_fb_ptr = (unsigned char *)xf86MapPciMem(index, VIDMEM_FRAMEBUFFER,
	tag, pci->memBase[0], pGeode->FBAvail + CIM_CMD_BFR_SZ);
#else
    cim_gp_ptr = map_pci_mem(pScrni, 0, pci, 1, LX_GP_REG_SIZE);
    cim_vg_ptr = map_pci_mem(pScrni, 0, pci, 2, LX_VG_REG_SIZE);
    cim_vid_ptr = map_pci_mem(pScrni, 0, pci, 3, LX_VID_REG_SIZE);
    cim_vip_ptr = map_pci_mem(pScrni, 0, pci, 4, LX_VIP_REG_SIZE);
    cim_fb_ptr =
	map_pci_mem(pScrni, 1, pci, 0, pGeode->FBAvail + CIM_CMD_BFR_SZ);
#endif

    if (pScrni->memPhysBase == 0)
	pScrni->memPhysBase = PCI_REGION_BASE(pci, 0, REGION_MEM);

    cmd_bfr_phys = PCI_REGION_BASE(pci, 0, REGION_MEM) + pGeode->CmdBfrOffset;
    cim_cmd_base_ptr = cim_fb_ptr + pGeode->CmdBfrOffset;

    if (!cim_gp_ptr || !cim_vg_ptr || !cim_vid_ptr || !cim_fb_ptr ||
	!cim_vip_ptr)
	return FALSE;

    gp_set_frame_buffer_base(PCI_REGION_BASE(pci, 0, REGION_MEM),
	pGeode->FBAvail);
    gp_set_command_buffer_base(cmd_bfr_phys, 0, pGeode->CmdBfrSize);

    XpressROMPtr = xf86MapVidMem(index, VIDMEM_FRAMEBUFFER, 0xF0000, 0x10000);

    pGeode->FBBase = cim_fb_ptr;

    if (!pGeode->NoAccel)
	pGeode->pExa->memoryBase = pGeode->FBBase;

    xf86DrvMsg(index, X_INFO, "Geode LX video memory %x bytes at %p\n",
	pGeode->FBAvail, pGeode->FBBase);

    return TRUE;
}

/* Check to see if VGA exists - we map the space and look for a
   signature - if it doesn't match exactly, then we assume no VGA.
*/

static Bool
LXCheckVGA(ScrnInfoPtr pScrni)
{

    unsigned char *ptr;
    const char *vgasig = "IBM VGA Compatible";
    int ret;

    ptr =
	xf86MapVidMem(pScrni->scrnIndex, VIDMEM_FRAMEBUFFER, 0xC001E,
	strlen(vgasig));

    if (ptr == NULL)
	return FALSE;

    ret = memcmp(ptr, vgasig, strlen(vgasig));
    xf86UnMapVidMem(pScrni->scrnIndex, (pointer) ptr, strlen(vgasig));

    return ret ? FALSE : TRUE;
}

static Bool
LXPreInit(ScrnInfoPtr pScrni, int flags)
{
    GeodePtr pGeode;
    ClockRangePtr GeodeClockRange;
    EntityInfoPtr pEnt;
    OptionInfoRec *GeodeOptions = &LX_GeodeOptions[0];
    rgb defaultWeight = { 0, 0, 0 };
    int modecnt;
    char *s;

    if (pScrni->numEntities != 1)
	return FALSE;

    pEnt = xf86GetEntityInfo(pScrni->entityList[0]);

    if (pEnt->resources)
	return FALSE;

    if (flags & PROBE_DETECT) {
	GeodeProbeDDC(pScrni, pEnt->index);
	return TRUE;
    }

    pGeode = pScrni->driverPrivate = xnfcalloc(sizeof(GeodeRec), 1);

    if (pGeode == NULL)
	return FALSE;

    pGeode->useVGA = LXCheckVGA(pScrni);
    pGeode->VGAActive = FALSE;
    pGeode->pEnt = pEnt;

    if (pGeode->useVGA) {
	if (!xf86LoadSubModule(pScrni, "vgahw") || !vgaHWGetHWRec(pScrni))
	    pGeode->useVGA = FALSE;

	pGeode->vesa = xcalloc(sizeof(VESARec), 1);
    }

    cim_rdmsr = LXReadMSR;
    cim_wrmsr = LXWriteMSR;

    /* Set up the Cimarron MSR tables */
    msr_init_table();

    /* By default, we support panel and CRT - the config file should
     * disable the ones we don't want
     */

    pGeode->Output = OUTPUT_PANEL | OUTPUT_CRT;

    /* Fill in the monitor information */
    pScrni->monitor = pScrni->confScreen->monitor;

    if (!xf86SetDepthBpp(pScrni, 16, 16, 16, Support24bppFb | Support32bppFb))
	return FALSE;

    switch (pScrni->depth) {
    case 8:
	pScrni->rgbBits = 8;
    case 16:
    case 24:
    case 32:
	break;
    default:
	xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
	    "The driver does not support %d as a depth.\n", pScrni->depth);
	return FALSE;
    }

    xf86PrintDepthBpp(pScrni);

    if (!xf86SetWeight(pScrni, defaultWeight, defaultWeight))
	return FALSE;

    if (!xf86SetDefaultVisual(pScrni, -1))
	return FALSE;

    /*
     * If the driver can do gamma correction, it should call xf86SetGamma()
     * here.
     */
    {
	Gamma zeros = { 0.0, 0.0, 0.0 };

	if (!xf86SetGamma(pScrni, zeros)) {
	    return FALSE;
	}
    }

    pScrni->progClock = TRUE;
    xf86CollectOptions(pScrni, NULL);
    xf86ProcessOptions(pScrni->scrnIndex, pScrni->options, GeodeOptions);

    /* Set up our various options that may get reversed as we go on */

    pGeode->tryHWCursor = TRUE;
    pGeode->tryCompression = TRUE;

    /* Protect against old versions of EXA */

#if (EXA_VERSION_MAJOR < 2)
    pGeode->NoAccel = TRUE;
    xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
	"*** This driver was compiled with EXA version %d\n");
    xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
	"*** we need version 2 or greater\n");
    xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
	"*** All accelerations are being turned off.\n");
#else
    pGeode->NoAccel = FALSE;
#endif

    pGeode->exaBfrSz = DEFAULT_EXA_SCRATCH_BFRSZ;

    xf86GetOptValBool(GeodeOptions, LX_OPTION_HW_CURSOR,
	&pGeode->tryHWCursor);

    if (!xf86GetOptValInteger(GeodeOptions, LX_OPTION_FBSIZE,
	    (int *)&(pGeode->FBAvail)))
	pGeode->FBAvail = 0;

    /* For compatability - allow SWCursor too */

    if (xf86ReturnOptValBool(GeodeOptions, LX_OPTION_SW_CURSOR, FALSE))
	pGeode->tryHWCursor = FALSE;

    if (xf86ReturnOptValBool(GeodeOptions, LX_OPTION_NOCOMPRESSION, FALSE))
	pGeode->tryCompression = FALSE;

    if (xf86ReturnOptValBool(GeodeOptions, LX_OPTION_NOACCEL, FALSE))
	pGeode->NoAccel = TRUE;

    pGeode->rotation = RR_Rotate_0;

    if ((s = xf86GetOptValString(GeodeOptions, LX_OPTION_ROTATE))) {

	if (!xf86NameCmp(s, "LEFT"))
	    pGeode->rotation = RR_Rotate_90;
	else if (!xf86NameCmp(s, "INVERT"))
	    pGeode->rotation = RR_Rotate_180;
	else if (!xf86NameCmp(s, "CCW"))
	    pGeode->rotation = RR_Rotate_270;
	else
	    xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
		"Invalid rotation %s.\n", s);
    }

    xf86GetOptValInteger(GeodeOptions, LX_OPTION_EXA_SCRATCH_BFRSZ,
	(int *)&(pGeode->exaBfrSz));

    if (pGeode->exaBfrSz <= 0)
	pGeode->exaBfrSz = 0;

    if (pGeode->Output & OUTPUT_PANEL) {
	if (xf86ReturnOptValBool(GeodeOptions, LX_OPTION_NOPANEL, FALSE))
	    pGeode->Output &= ~OUTPUT_PANEL;
    }

    /* Panel detection code -
     * 1.  See if an OLPC DCON is attached - we can make some assumptions
     * about the panel if so.
     * 2.  Use override options specified in the config
     * 3.  "Autodetect" the panel through VSA
     */

    if (dcon_init(pScrni)) {
	pGeode->Output = OUTPUT_PANEL | OUTPUT_DCON;
    } else if (pGeode->Output & OUTPUT_PANEL) {
	char *panelgeo =
	    xf86GetOptValString(GeodeOptions, LX_OPTION_PANEL_GEOMETRY);

	if (panelgeo != NULL)
	    GeodeGetFPGeometry(panelgeo, &pGeode->PanelX, &pGeode->PanelY);
	else {
	    if (!lx_get_panel(&pGeode->PanelX, &pGeode->PanelY))
		pGeode->Output &= ~OUTPUT_PANEL;
	}
    }

    xf86DrvMsg(pScrni->scrnIndex, X_INFO, "LX output options:\n");
    xf86DrvMsg(pScrni->scrnIndex, X_INFO, " CRT: %s\n",
	pGeode->Output & OUTPUT_CRT ? "YES" : "NO");

    xf86DrvMsg(pScrni->scrnIndex, X_INFO, " PANEL: %s\n",
	pGeode->Output & OUTPUT_PANEL ? "YES" : "NO");

    xf86DrvMsg(pScrni->scrnIndex, X_INFO, " DCON: %s\n",
	pGeode->Output & OUTPUT_DCON ? "YES" : "NO");

    xf86DrvMsg(pScrni->scrnIndex, X_INFO, " VGA: %s\n",
	pGeode->useVGA ? "YES" : "NO");

    /* Set up VGA */
    if (pGeode->useVGA) {
	xf86LoaderReqSymLists(amdVgahwSymbols, NULL);

	VESARec *pVesa;

	if (!xf86LoadSubModule(pScrni, "int10"))
	    return FALSE;

	xf86LoaderReqSymLists(amdInt10Symbols, NULL);

	pVesa = pGeode->vesa;

	if ((pVesa->pInt = xf86InitInt10(pGeode->pEnt->index)) == NULL) {
	    xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
		"Unable to initialize 1NT10 support\n");
	    pGeode->useVGA = FALSE;
	}
    }

    /* Read the amount of framebuffer memory */
    /* First try to read it from the framebuffer, and if that fails,
     * do it the legacy way
     */

    if (pGeode->FBAvail == 0) {
	if (GeodeGetSizeFromFB(&pGeode->FBAvail)) {
	    unsigned long value;

	    cim_outw(0xAC1C, 0xFC53);
	    cim_outw(0xAC1C, 0x0200);

	    value = (unsigned long)(cim_inw(0xAC1E)) & 0xFE;
	    pGeode->FBAvail = value << 20;
	}
    }

    pScrni->fbOffset = 0;

    if (pGeode->pEnt->device->videoRam == 0)
	pScrni->videoRam = pGeode->FBAvail / 1024;
    else {
	pScrni->videoRam = pGeode->pEnt->device->videoRam;
	pGeode->FBAvail = pScrni->videoRam << 10;
    }

    /* Carve out some memory for the command buffer */

    pGeode->CmdBfrSize = CIM_CMD_BFR_SZ;
    pGeode->FBAvail -= CIM_CMD_BFR_SZ;

    pGeode->CmdBfrOffset = pGeode->FBAvail;

    pGeode->maxWidth = LX_MAX_WIDTH;
    pGeode->maxHeight = LX_MAX_HEIGHT;

    GeodeClockRange = (ClockRangePtr) xnfcalloc(sizeof(ClockRange), 1);
    GeodeClockRange->next = NULL;
    GeodeClockRange->minClock = 25175;
    GeodeClockRange->maxClock = 229500;
    GeodeClockRange->clockIndex = -1;
    GeodeClockRange->interlaceAllowed = TRUE;
    GeodeClockRange->doubleScanAllowed = FALSE;

    if (pGeode->Output & OUTPUT_CRT)
	pScrni->monitor->DDC = GeodeDoDDC(pScrni, pGeode->pEnt->index);
    else
	pScrni->monitor->DDC = NULL;

    /* I'm still not 100% sure this uses the right values */

    modecnt = xf86ValidateModes(pScrni,
	pScrni->monitor->Modes,
	pScrni->display->modes,
	GeodeClockRange,
	NULL, LX_MIN_PITCH, LX_MAX_PITCH,
	32, LX_MIN_HEIGHT, LX_MAX_HEIGHT,
	pScrni->display->virtualX,
	pScrni->display->virtualY, pGeode->FBAvail, LOOKUP_BEST_REFRESH);

    if (modecnt <= 0) {
	xf86DrvMsg(pScrni->scrnIndex, X_ERROR, "No valid modes were found\n");
	return FALSE;
    }

    xf86PruneDriverModes(pScrni);

    if (pScrni->modes == NULL) {
	xf86DrvMsg(pScrni->scrnIndex, X_ERROR, "No valid modes were found\n");
	return FALSE;
    }

    xf86SetCrtcForModes(pScrni, 0);
    pScrni->currentMode = pScrni->modes;

    xf86PrintModes(pScrni);
    xf86SetDpi(pScrni, 0, 0);

    /* Load the modules we'll need */

    if (xf86LoadSubModule(pScrni, "fb") == NULL) {
	return FALSE;
    }

    xf86LoaderReqSymLists(amdFbSymbols, NULL);

    if (!pGeode->NoAccel) {
	if (!xf86LoadSubModule(pScrni, "exa"))
	    return FALSE;

	xf86LoaderReqSymLists(&amdExaSymbols[0], NULL);
    }

    if (pGeode->tryHWCursor == TRUE) {
	if (!xf86LoadSubModule(pScrni, "ramdac")) {
	    return FALSE;
	}

	xf86LoaderReqSymLists(amdRamdacSymbols, NULL);
    }

    if (xf86RegisterResources(pGeode->pEnt->index, NULL, ResExclusive)) {
	xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
	    "Couldn't register the resources.\n");
	return FALSE;
    }

    return TRUE;
}

static void
LXRestore(ScrnInfoPtr pScrni)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);

    if (pGeode->useVGA) {
	vgaHWPtr pvgaHW = VGAHWPTR(pScrni);

	vgaHWProtect(pScrni, TRUE);
	vgaHWRestore(pScrni, &pvgaHW->SavedReg, VGA_SR_ALL);
	vgaHWProtect(pScrni, FALSE);
    }
}

static Bool
LXUnmapMem(ScrnInfoPtr pScrni)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);
    pciVideoPtr pci = xf86GetPciInfoForEntity(pGeode->pEnt->index);

#ifndef XSERVER_LIBPCIACCESS
    xf86UnMapVidMem(pScrni->scrnIndex, (pointer) cim_gp_ptr, LX_GP_REG_SIZE);
    xf86UnMapVidMem(pScrni->scrnIndex, (pointer) cim_vg_ptr, LX_VG_REG_SIZE);
    xf86UnMapVidMem(pScrni->scrnIndex, (pointer) cim_vid_ptr,
	LX_VID_REG_SIZE);
    xf86UnMapVidMem(pScrni->scrnIndex, (pointer) cim_vip_ptr,
	LX_VIP_REG_SIZE);
#else
    unmap_pci_mem(pScrni, pci, cim_gp_ptr, LX_GP_REG_SIZE);
    unmap_pci_mem(pScrni, pci, cim_vg_ptr, LX_VG_REG_SIZE);
    unmap_pci_mem(pScrni, pci, cim_vid_ptr, LX_VID_REG_SIZE);
    unmap_pci_mem(pScrni, pci, cim_vip_ptr, LX_VIP_REG_SIZE);
    unmap_pci_mem(pScrni, pci, cim_fb_ptr, pGeode->FBAvail + CIM_CMD_BFR_SZ);
#endif

    xf86UnMapVidMem(pScrni->scrnIndex, XpressROMPtr, 0x10000);

    return TRUE;
}

/* These should be correctly accounted for rotation */

static void
LXAdjustFrame(int scrnIndex, int x, int y, int flags)
{
    ScrnInfoPtr pScrni = xf86Screens[scrnIndex];
    GeodeRec *pGeode = GEODEPTR(pScrni);

    unsigned long offset;

    /* XXX:  Is pitch correct here? */

    offset = pGeode->FBOffset + (y * pGeode->Pitch);
    offset += x * (pScrni->bitsPerPixel >> 3);

    vg_set_display_offset(offset);
}

static Bool
LXSetVideoMode(ScrnInfoPtr pScrni, DisplayModePtr pMode)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);
    DF_VIDEO_SOURCE_PARAMS vs_odd, vs_even;
    int flags = 0;
    int video_enable;
    unsigned long video_flags;

    df_get_video_enable(&video_enable, &video_flags);

    if (video_enable != 0)
	df_set_video_enable(0, 0);

    df_get_video_source_configuration(&vs_odd, &vs_even);
    lx_disable_dac_power(pScrni, DF_CRT_DISABLE);
    vg_set_compression_enable(0);

    /* If the mode is a default one, then set the mode with the Cimarron
     * tables */

    if ((pMode->type & M_T_BUILTIN) || (pMode->type & M_T_DEFAULT)) {
	if (pMode->Flags & V_NHSYNC)
	    flags |= VG_MODEFLAG_NEG_HSYNC;
	if (pMode->Flags & V_NVSYNC)
	    flags |= VG_MODEFLAG_NEG_VSYNC;

	if (pGeode->Output & OUTPUT_PANEL) {
	    int activex = pGeode->PanelX;
	    int activey = pGeode->PanelY;

	    flags = pGeode->Output & OUTPUT_CRT ? VG_MODEFLAG_CRT_AND_FP : 0;

	    if (pMode->CrtcHDisplay > 1024 &&
		pMode->CrtcHDisplay != pGeode->PanelX) {
		ErrorF
		    ("The source is greater then 1024 - scaling is disabled.\n");
		activex = pMode->CrtcHDisplay;
		activey = pMode->CrtcVDisplay;

		vg_set_border_color(0);
	    }

	    vg_set_panel_mode(pMode->CrtcHDisplay, pMode->CrtcVDisplay,
		activex, activey, activex, activey,
		pScrni->bitsPerPixel, flags);
	} else {
	    vg_set_display_mode(pMode->CrtcHDisplay, pMode->CrtcVDisplay,
		pMode->CrtcHDisplay, pMode->CrtcVDisplay,
		pScrni->bitsPerPixel, GeodeGetRefreshRate(pMode), 0);
	}
    } else {
	/* For anything other then a default mode - use the passed in
	 * timings */

	lx_set_custom_mode(pGeode, pMode, pScrni->bitsPerPixel);
    }

    if (pGeode->Output & OUTPUT_PANEL)
	df_set_output_path((pGeode->
		Output & OUTPUT_CRT) ? DF_DISPLAY_CRT_FP : DF_DISPLAY_FP);
    else
	df_set_output_path(DF_DISPLAY_CRT);

    vg_set_display_pitch(pGeode->Pitch);
    gp_set_bpp(pScrni->bitsPerPixel);

    vg_set_display_offset(0);
    vg_wait_vertical_blank();

    if (pGeode->Compression) {
	vg_configure_compression(&(pGeode->CBData));
	vg_set_compression_enable(1);
    }

    if (pGeode->HWCursor && !(pMode->Flags & V_DBLSCAN)) {
	VG_PANNING_COORDINATES panning;

	LXLoadCursorImage(pScrni, NULL);
	vg_set_cursor_position(0, 0, &panning);
	LXShowCursor(pScrni);
    } else {
	vg_set_cursor_enable(0);
	pGeode->HWCursor = FALSE;
    }

    LXAdjustFrame(pScrni->scrnIndex, pScrni->frameX0, pScrni->frameY0, 0);

    df_configure_video_source(&vs_odd, &vs_even);

    if (video_enable != 0)
	df_set_video_enable(video_enable, video_flags);

    lx_enable_dac_power(pScrni, 1);

    return TRUE;
}

static Bool
LXSwitchMode(int index, DisplayModePtr pMode, int flags)
{
    ScrnInfoPtr pScrni = xf86Screens[index];
    GeodeRec *pGeode = GEODEPTR(pScrni);
    int ret = TRUE;
    int rotate;

    /* Syn the engine and shutdown the DAC momentarily */
    gp_wait_until_idle();

    /* Set up the memory for the new mode */
    rotate = LXGetRotation(pScrni->pScreen);
    ret = LXAllocateMemory(pScrni->pScreen, pScrni, rotate);

    if (ret) {
	if (pGeode->curMode != pMode)
	    ret = LXSetVideoMode(pScrni, pMode);
    }

    if (ret)
	ret = LXRotate(pScrni, pMode);

    /* Go back the way it was */

    if (ret == FALSE) {
	if (!LXSetVideoMode(pScrni, pGeode->curMode))
	    xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
		"Could not restore the previous mode\n");
    } else
	pGeode->curMode = pMode;

    return ret;
}

static void
LXLeaveGraphics(ScrnInfoPtr pScrni)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);
    VG_PANNING_COORDINATES panning;

    gp_wait_until_idle();

    lx_disable_dac_power(pScrni, DF_CRT_DISABLE);

    vg_set_custom_mode(&(pGeode->FBcimdisplaytiming.vgDisplayMode),
	pGeode->FBcimdisplaytiming.wBpp);

    vg_set_compression_enable(0);

    /* Restore the previous Compression state */
    if (pGeode->FBCompressionEnable) {
	vg_configure_compression(&(pGeode->FBCBData));
	vg_set_compression_enable(1);
    }

    vg_set_display_pitch(pGeode->FBcimdisplaytiming.wPitch);
    vg_set_display_offset(pGeode->FBDisplayOffset);

    /* Restore Cursor */
    vg_set_cursor_position(pGeode->FBCursor.cursor_x,
	pGeode->FBCursor.cursor_y, &panning);

    LXRestore(pScrni);

    if (pGeode->useVGA && pGeode->VGAActive) {
	pGeode->vesa->pInt->num = 0x10;
	pGeode->vesa->pInt->ax = 0x0 | pGeode->FBBIOSMode;
	pGeode->vesa->pInt->bx = 0;
	xf86ExecX86int10(pGeode->vesa->pInt);
	vg_delay_milliseconds(3);
    }

    lx_enable_dac_power(pScrni, 1);
    pScrni->vtSema = FALSE;
}

static Bool
LXCloseScreen(int scrnIndex, ScreenPtr pScrn)
{
    ScrnInfoPtr pScrni = xf86Screens[scrnIndex];
    GeodeRec *pGeode = GEODEPTR(pScrni);

    if (pScrni->vtSema)
	LXLeaveGraphics(pScrni);

    if (pGeode->pExa) {
	exaDriverFini(pScrn);
	xfree(pGeode->pExa);
	pGeode->pExa = NULL;
    }

    LXUnmapMem(pScrni);

    if (pGeode->useVGA)
	vgaHWUnmapMem(pScrni);

    pScrni->PointerMoved = pGeode->PointerMoved;
    pScrn->CloseScreen = pGeode->CloseScreen;

    if (pScrn->CloseScreen)
	return (*pScrn->CloseScreen) (scrnIndex, pScrn);

    return TRUE;
}

static Bool
LXEnterGraphics(ScreenPtr pScrn, ScrnInfoPtr pScrni)
{
    int bpp;
    GeodeRec *pGeode = GEODEPTR(pScrni);

    pGeode->curMode = NULL;

    pGeode->VGAActive = gu3_get_vga_active();

    gp_wait_until_idle();

    //lx_disable_dac_power(pScrni, DF_CRT_DISABLE);

    vg_get_current_display_mode(&pGeode->FBcimdisplaytiming.vgDisplayMode,
	&bpp);

    //dump_previous(&pGeode->FBcimdisplaytiming.vgDisplayMode);

    pGeode->FBcimdisplaytiming.wBpp = bpp;
    pGeode->FBcimdisplaytiming.wPitch = vg_get_display_pitch();

    pGeode->FBDisplayOffset = vg_get_display_offset();

    if (pGeode->useVGA && pGeode->VGAActive) {
	vgaHWPtr pvgaHW = VGAHWPTR(pScrni);

	pGeode->FBBIOSMode = pvgaHW->readCrtc(pvgaHW, 0x040);
    }

    pGeode->FBCompressionEnable = vg_get_compression_enable();
    vg_get_compression_info(&(pGeode->FBCBData));

    /* Save Cursor offset */
    vg_get_cursor_info(&pGeode->FBCursor);

    /* Turn off the VGA */

    if (pGeode->useVGA) {
	unsigned short sequencer;
	vgaHWPtr pvgaHW = VGAHWPTR(pScrni);

	/* Unlock VGA registers */
	vgaHWUnlock(pvgaHW);

	/* Save the current state and setup the current mode */
	vgaHWSave(pScrni, &VGAHWPTR(pScrni)->SavedReg, VGA_SR_ALL);

	/* DISABLE VGA SEQUENCER */
	/* This allows the VGA state machine to terminate. We must delay */
	/* such that there are no pending MBUS requests.  */

	cim_outb(DC3_SEQUENCER_INDEX, DC3_SEQUENCER_CLK_MODE);
	sequencer = cim_inb(DC3_SEQUENCER_DATA);
	sequencer |= DC3_CLK_MODE_SCREEN_OFF;
	cim_outb(DC3_SEQUENCER_DATA, sequencer);

	vg_delay_milliseconds(1);

	/* BLANK THE VGA DISPLAY */
	cim_outw(DC3_SEQUENCER_INDEX, DC3_SEQUENCER_RESET);
	sequencer = cim_inb(DC3_SEQUENCER_DATA);
	sequencer &= ~DC3_RESET_VGA_DISP_ENABLE;
	cim_outb(DC3_SEQUENCER_DATA, sequencer);

	vg_delay_milliseconds(1);
    }

    /* Set up the memory */
    /* XXX - FIXME - when we alow inital rotation, it should be here */
    LXAllocateMemory(pScrn, pScrni, pGeode->rotation);

    /* Clear the framebuffer */
    memset(pGeode->FBBase + pGeode->displayOffset, 0, pGeode->displaySize);

    /* Set the video mode */
    LXSetVideoMode(pScrni, pScrni->currentMode);

    pGeode->curMode = pScrni->currentMode;
    pScrni->vtSema = TRUE;

    return TRUE;
}

static void
LXLoadPalette(ScrnInfoPtr pScrni,
    int numColors, int *indizes, LOCO * colors, VisualPtr pVisual)
{
    int i, index, color;

    for (i = 0; i < numColors; i++) {
	index = indizes[i] & 0xFF;
	color = (((unsigned long)(colors[index].red & 0xFF)) << 16) |
	    (((unsigned long)(colors[index].green & 0xFF)) << 8) |
	    ((unsigned long)(colors[index].blue & 0xFF));

	vg_set_display_palette_entry(index, color);
    }
}

#ifdef DPMSExtension

static void
LXDPMSSet(ScrnInfoPtr pScrni, int mode, int flags)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);

    if (!pScrni->vtSema)
	return;

    if (pGeode->Output & OUTPUT_DCON) {
	if (DCONDPMSSet(pScrni, mode, flags))
	    return;
    }

    switch (mode) {
    case DPMSModeOn:
	lx_enable_dac_power(pScrni, 1);
	break;

    case DPMSModeStandby:
	lx_disable_dac_power(pScrni, DF_CRT_STANDBY);
	break;

    case DPMSModeSuspend:
	lx_disable_dac_power(pScrni, DF_CRT_SUSPEND);
	break;

    case DPMSModeOff:
	lx_disable_dac_power(pScrni, DF_CRT_DISABLE);
	break;
    }
}

#endif

static Bool
LXCreateScreenResources(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrni = xf86Screens[pScreen->myNum];
    GeodeRec *pGeode = GEODEPTR(pScrni);

    pScreen->CreateScreenResources = pGeode->CreateScreenResources;
    if (!(*pScreen->CreateScreenResources) (pScreen))
	return FALSE;

    if (xf86LoaderCheckSymbol("LXRandRSetConfig")
	&& pGeode->rotation != RR_Rotate_0) {
	Rotation(*LXRandRSetConfig) (ScreenPtr pScreen, Rotation rr, int rate,
	    RRScreenSizePtr pSize) = NULL;
	RRScreenSize p;
	Rotation requestedRotation = pGeode->rotation;

	pGeode->rotation = RR_Rotate_0;

	/* Just setup enough for an initial rotate */

	p.width = pScreen->width;
	p.height = pScreen->height;
	p.mmWidth = pScreen->mmWidth;
	p.mmHeight = pScreen->mmHeight;

	LXRandRSetConfig = LoaderSymbol("LXRandRSetConfig");
	if (LXRandRSetConfig) {
	    pGeode->starting = TRUE;
	    (*LXRandRSetConfig) (pScreen, requestedRotation, 0, &p);
	    pGeode->starting = FALSE;
	}
    }

    return TRUE;
}

static Bool
LXScreenInit(int scrnIndex, ScreenPtr pScrn, int argc, char **argv)
{
    ScrnInfoPtr pScrni = xf86Screens[scrnIndex];
    GeodeRec *pGeode = GEODEPTR(pScrni);
    XF86ModReqInfo shadowReq;
    int maj, min, ret, rotate;

    pGeode->starting = TRUE;

    /* If we are using VGA then go ahead and map the memory */

    if (pGeode->useVGA) {

	if (!vgaHWMapMem(pScrni))
	    return FALSE;

	vgaHWGetIOBase(VGAHWPTR(pScrni));
    }

    if (!pGeode->NoAccel) {

	pGeode->pExa = xnfcalloc(sizeof(ExaDriverRec), 1);

	if (pGeode->pExa) {

	    /* THis is set in LXAllocMem */
	    pGeode->pExa->memoryBase = 0;

	    /* This is set in LXAllocateMemory */
	    pGeode->pExa->memorySize = 0;

	    pGeode->pExa->pixmapOffsetAlign = 32;
	    pGeode->pExa->pixmapPitchAlign = 32;
	    pGeode->pExa->flags = EXA_OFFSCREEN_PIXMAPS;
	    pGeode->pExa->maxX = pGeode->maxWidth - 1;
	    pGeode->pExa->maxY = pGeode->maxHeight - 1;
	} else {
	    xf86DrvMsg(scrnIndex, X_ERROR,
		"Couldn't allocate the EXA structure.\n");
	    pGeode->NoAccel = TRUE;
	}
    }

    /* Map the memory here before doing anything else */

    if (!LXMapMem(pScrni))
	return FALSE;

    /* XXX FIXME - Take down any of the structures on failure? */
    if (!LXEnterGraphics(pScrn, pScrni))
	return FALSE;

    miClearVisualTypes();

    /* XXX Again - take down anything? */

    if (pScrni->bitsPerPixel > 8) {
	if (!miSetVisualTypes(pScrni->depth,
		TrueColorMask, pScrni->rgbBits, pScrni->defaultVisual)) {
	    return FALSE;
	}
    } else {
	if (!miSetVisualTypes(pScrni->depth,
		miGetDefaultVisualMask(pScrni->depth),
		pScrni->rgbBits, pScrni->defaultVisual)) {
	    return FALSE;
	}
    }

    miSetPixmapDepths();

    /* Point at the visible area to start */

    ret = fbScreenInit(pScrn, pGeode->FBBase + pGeode->displayOffset,
	pScrni->virtualX, pScrni->virtualY,
	pScrni->xDpi, pScrni->yDpi, pGeode->displayWidth,
	pScrni->bitsPerPixel);

    if (!ret)
	return FALSE;

    xf86SetBlackWhitePixels(pScrn);

    /* Set up the color ordering */
    if (pScrni->bitsPerPixel > 8) {
	VisualPtr visual = pScrn->visuals + pScrn->numVisuals;

	while (--visual >= pScrn->visuals) {
	    if ((visual->class | DynamicClass) == DirectColor) {
		visual->offsetRed = pScrni->offset.red;
		visual->offsetGreen = pScrni->offset.green;
		visual->offsetBlue = pScrni->offset.blue;
		visual->redMask = pScrni->mask.red;
		visual->greenMask = pScrni->mask.green;
		visual->blueMask = pScrni->mask.blue;
	    }
	}
    }

    /* Must follow the color ordering */
    fbPictureInit(pScrn, 0, 0);

    if (!pGeode->NoAccel)
	pGeode->NoAccel = LXExaInit(pScrn) ? FALSE : TRUE;

    miInitializeBackingStore(pScrn);
    xf86SetBackingStore(pScrn);

    /* Set up the soft cursor */
    miDCInitialize(pScrn, xf86GetPointerScreenFuncs());

    /* Set up the HW cursor - must follow the soft cursor init */

    if (pGeode->tryHWCursor) {
	if (!LXHWCursorInit(pScrn))
	    xf86DrvMsg(scrnIndex, X_ERROR,
		"Hardware cursor initialization failed.\n");
    }

    /* Set up the color map */

    if (!miCreateDefColormap(pScrn))
	return FALSE;

    if (pScrni->bitsPerPixel == 8) {
	/* Must follow initialization of the default colormap */

	if (!xf86HandleColormaps(pScrn, 256, 8,
		LXLoadPalette, NULL,
		CMAP_PALETTED_TRUECOLOR | CMAP_RELOAD_ON_MODE_SWITCH)) {
	    return FALSE;
	}
    }
#ifdef DPMSExtension
    xf86DPMSInit(pScrn, LXDPMSSet, 0);
#endif

    LXInitVideo(pScrn);

    /* Set up RandR */
    /* We provide our own RandR goodness - disable the default */
    xf86DisableRandR();

    memset(&shadowReq, 0, sizeof(shadowReq));
    shadowReq.majorversion = 1;
    shadowReq.minorversion = 1;

    if (LoadSubModule(pScrni->module, "shadow",
	    NULL, NULL, NULL, &shadowReq, &maj, &min)) {

	rotate = RR_Rotate_0 | RR_Rotate_90 | RR_Rotate_180 | RR_Rotate_270;
	shadowSetup(pScrn);
    } else {
	LoaderErrorMsg(NULL, "shadow", maj, min);
	xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
	    "Error loading shadow - rotation not available.\n");

	if (pGeode->rotation != RR_Rotate_0)
	    xf86DrvMsg(pScrni->scrnIndex, X_ERROR,
		"Reverting back to normal rotation.\n");

	rotate = pGeode->rotation = RR_Rotate_0;
    }

    LXRandRInit(pScrn, rotate);

    pGeode->PointerMoved = pScrni->PointerMoved;
    pScrni->PointerMoved = GeodePointerMoved;
    pGeode->CreateScreenResources = pScrn->CreateScreenResources;
    pScrn->CreateScreenResources = LXCreateScreenResources;

    pGeode->CloseScreen = pScrn->CloseScreen;
    pScrn->CloseScreen = LXCloseScreen;
    pScrn->SaveScreen = LXSaveScreen;

    if (serverGeneration == 1)
	xf86ShowUnusedOptions(pScrni->scrnIndex, pScrni->options);

    pGeode->starting = FALSE;

    return TRUE;
}

static int
LXValidMode(int scrnIndex, DisplayModePtr pMode, Bool Verbose, int flags)
{
    ScrnInfoPtr pScrni = xf86Screens[scrnIndex];
    GeodeRec *pGeode = GEODEPTR(pScrni);
    int p, ret;
    VG_QUERY_MODE vgQueryMode;

    memset(&vgQueryMode, 0, sizeof(vgQueryMode));

    /* For builtin and default modes, try to look up the mode in Cimarron */

    if ((pMode->type & M_T_BUILTIN) || (pMode->type & M_T_DEFAULT)) {

	if (pGeode->Output & OUTPUT_PANEL) {

	    /* Can't scale this mode */

	    if ((pGeode->PanelY != pMode->CrtcHDisplay) &&
		pMode->CrtcHDisplay > 1024)
		return MODE_NOMODE;

	    vgQueryMode.panel_width = pGeode->PanelX;
	    vgQueryMode.panel_height = pGeode->PanelY;

	    vgQueryMode.query_flags |=
		VG_QUERYFLAG_PANELWIDTH | VG_QUERYFLAG_PANELHEIGHT;
	}

	vgQueryMode.active_width = pMode->CrtcHDisplay;
	vgQueryMode.active_height = pMode->CrtcVDisplay;
	vgQueryMode.bpp = pScrni->bitsPerPixel;
	vgQueryMode.hz = GeodeGetRefreshRate(pMode);
	vgQueryMode.query_flags |= VG_QUERYFLAG_REFRESH | VG_QUERYFLAG_BPP |
	    VG_QUERYFLAG_ACTIVEWIDTH | VG_QUERYFLAG_ACTIVEHEIGHT;

	ret = vg_get_display_mode_index(&vgQueryMode);

	if (ret < 0)
	    return MODE_BAD;
    }

    if (pGeode->tryCompression)
	p = GeodeCalculatePitchBytes(pMode->CrtcHDisplay,
	    pScrni->bitsPerPixel);
    else
	p = ((pMode->CrtcHDisplay + 3) & ~3) * (pScrni->bitsPerPixel >> 3);

    if (p * pMode->CrtcVDisplay > pGeode->FBAvail)
	return MODE_MEM;

    return MODE_OK;
}

/* XXX - Way more to do here */

static Bool
LXEnterVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrni = xf86Screens[scrnIndex];
    Bool ret = LXEnterGraphics(NULL, pScrni);

    /* Reallocate a shadow area, if we need it */

    if (ret == TRUE)
	ret = LXAllocShadow(pScrni);

    return ret;
}

static void
LXLeaveVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrni = xf86Screens[scrnIndex];
    GeodeRec *pGeode = GEODEPTR(pScrni);

    pGeode->PrevDisplayOffset = vg_get_display_offset();
    LXLeaveGraphics(xf86Screens[scrnIndex]);

    /* Destroy any shadow area, if we have it */

    if (pGeode->shadowArea != NULL) {
	exaOffscreenFree(pScrni->pScreen, pGeode->shadowArea);
	pGeode->shadowArea = NULL;
    }
}

void
LXSetupChipsetFPtr(ScrnInfoPtr pScrn)
{
    pScrn->PreInit = LXPreInit;
    pScrn->ScreenInit = LXScreenInit;
    pScrn->SwitchMode = LXSwitchMode;
    pScrn->AdjustFrame = LXAdjustFrame;
    pScrn->EnterVT = LXEnterVT;
    pScrn->LeaveVT = LXLeaveVT;
    pScrn->FreeScreen = GeodeFreeScreen;
    pScrn->ValidMode = LXValidMode;
}
