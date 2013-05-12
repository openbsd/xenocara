/*
 * Copyright (c) 2007,2010 NVIDIA Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include <xf86.h>
#include <xf86_OSproc.h>
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include <xf86Resources.h>
#endif
#include <mipointer.h>
#include <micmap.h>
#include <xf86cmap.h>
#include <fb.h>
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif

#include <xf86RandR12.h>

#include "nv_const.h"
#include "g80_type.h"
#include "g80_cursor.h"
#include "g80_display.h"
#include "g80_dma.h"
#include "g80_output.h"
#include "g80_exa.h"
#include "g80_xaa.h"

#define G80_REG_SIZE (1024 * 1024 * 16)
#define G80_RESERVED_VIDMEM 0xe000

typedef enum {
    OPTION_HW_CURSOR,
    OPTION_NOACCEL,
    OPTION_ACCEL_METHOD,
    OPTION_FP_DITHER,
    OPTION_ALLOW_DUAL_LINK,
} G80Opts;

static const OptionInfoRec G80Options[] = {
    { OPTION_HW_CURSOR,         "HWCursor",             OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_NOACCEL,           "NoAccel",              OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_ACCEL_METHOD,      "AccelMethod",          OPTV_STRING,    {0}, FALSE },
    { OPTION_FP_DITHER,         "FPDither",             OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_ALLOW_DUAL_LINK,   "AllowDualLinkModes",   OPTV_BOOLEAN,   {0}, FALSE },
    { -1,                       NULL,                   OPTV_NONE,      {0}, FALSE }
};

static Bool
G80GetRec(ScrnInfoPtr pScrn)
{
    if(pScrn->driverPrivate == NULL)
        pScrn->driverPrivate = calloc(sizeof(G80Rec), 1);

    return (pScrn->driverPrivate != NULL);
}

static void
G80FreeRec(ScrnInfoPtr pScrn)
{
    free(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
}

static Bool
G80ResizeScreen(ScrnInfoPtr pScrn, int width, int height)
{
    ScreenPtr pScreen = pScrn->pScreen;
    G80Ptr pNv = G80PTR(pScrn);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int pitch = width * (pScrn->bitsPerPixel / 8);
    int i;

    pitch = (pitch + 255) & ~255;

    pScrn->virtualX = width;
    pScrn->virtualY = height;

    /* Can resize if XAA is disabled or EXA is enabled */
    if(
#ifdef HAVE_XAA_H
       !pNv->xaa ||
#endif
       pNv->exa) {
        (*pScrn->pScreen->GetScreenPixmap)(pScrn->pScreen)->devKind = pitch;
        pScrn->displayWidth = pitch / (pScrn->bitsPerPixel / 8);

        /* Re-set the modes so the new pitch is taken into account */
        for(i = 0; i < xf86_config->num_crtc; i++) {
            xf86CrtcPtr crtc = xf86_config->crtc[i];
            if(crtc->enabled)
                xf86CrtcSetMode(crtc, &crtc->mode, crtc->rotation, crtc->x, crtc->y);
        }
    }

    /*
     * If EXA is enabled, use exaOffscreenAlloc to carve out a chunk of memory
     * for the screen.
     */
    if(pNv->exa) {
        if(pNv->exaScreenArea)
            exaOffscreenFree(pScreen, pNv->exaScreenArea);
        pNv->exaScreenArea = exaOffscreenAlloc(pScreen, pitch * pScrn->virtualY,
                                               256, TRUE, NULL, NULL);
        if(!pNv->exaScreenArea || pNv->exaScreenArea->offset != 0) {
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "Failed to reserve EXA memory for the screen or EXA "
                       "returned an area with a nonzero offset.  Don't be "
                       "surprised if your screen is corrupt.\n");
        }
    }

    return TRUE;
}

static const xf86CrtcConfigFuncsRec randr12_screen_funcs = {
    .resize = G80ResizeScreen,
};

static Bool
G80PreInit(ScrnInfoPtr pScrn, int flags)
{
    G80Ptr pNv;
    EntityInfoPtr pEnt;
#if XSERVER_LIBPCIACCESS
    struct pci_device *pPci;
    int err;
    void *p;
#else
    pciVideoPtr pPci;
    PCITAG pcitag;
#endif
    MessageType from;
    Bool primary;
    const rgb zeros = {0, 0, 0};
    const Gamma gzeros = {0.0, 0.0, 0.0};
    char *s;
    CARD32 tmp;
    memType BAR1sizeKB;

    if(flags & PROBE_DETECT)
        return TRUE;

    /* Check the number of entities, and fail if it isn't one. */
    if(pScrn->numEntities != 1)
        return FALSE;

    /* Allocate the NVRec driverPrivate */
    if(!G80GetRec(pScrn)) {
        return FALSE;
    }
    pNv = G80PTR(pScrn);

    /* Get the entity, and make sure it is PCI. */
    pEnt = xf86GetEntityInfo(pScrn->entityList[0]);
    if(pEnt->location.type != BUS_PCI) goto fail;
    pPci = xf86GetPciInfoForEntity(pEnt->index);
#if XSERVER_LIBPCIACCESS
    /* Need this to unmap */
    pNv->pPci = pPci;
#endif
    primary = xf86IsPrimaryPci(pPci);

    /* The ROM size sometimes isn't read correctly, so fix it up here. */
#if XSERVER_LIBPCIACCESS
    if(pPci->rom_size == 0)
        /* The BIOS is 64k */
        pPci->rom_size = 64 * 1024;
#else
    if(pPci->biosSize == 0)
        /* The BIOS is 64k */
        pPci->biosSize = 16;
#endif

    pNv->int10 = NULL;
    pNv->int10Mode = 0;
    if(xf86LoadSubModule(pScrn, "int10")) {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Initializing int10\n");
        pNv->int10 = xf86InitInt10(pEnt->index);
    }

    if(!pNv->int10) {
        if(primary) {
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "Failed to initialize the int10 module; the console "
                       "will not be restored.\n");
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Failed to initialize the int10 module; this screen "
                       "will not be initialized.\n");
            goto fail;
        }
    }

    if(primary && pNv->int10) {
        const xf86Int10InfoPtr int10 = pNv->int10;

        /* Get the current video mode */
        int10->num = 0x10;
        int10->ax = 0x4f03;
        int10->bx = int10->cx = int10->dx = 0;
        xf86ExecX86int10(int10);
        pNv->int10Mode = int10->bx & 0x3fff;
        xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Console is VGA mode 0x%x\n",
                   pNv->int10Mode);
    }

    /* Disable VGA access */
#ifndef XSERVER_LIBPCIACCESS
    xf86SetOperatingState(resVgaIo, pEnt->index, ResUnusedOpr);
    xf86SetOperatingState(resVgaMem, pEnt->index, ResDisableOpr);
#endif

    pScrn->monitor = pScrn->confScreen->monitor;

    if(!xf86SetDepthBpp(pScrn, 0, 0, 0, Support32bppFb)) goto fail;
    switch (pScrn->depth) {
        case 8:
        case 15:
        case 16:
        case 24:
            /* OK */
            break;
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Given depth (%d) is not supported by this driver\n",
                    pScrn->depth);
            goto fail;
    }
    xf86PrintDepthBpp(pScrn);

    if(!xf86SetWeight(pScrn, zeros, zeros)) goto fail;
    if(!xf86SetDefaultVisual(pScrn, -1)) goto fail;

    /* We use a programmable clock */
    pScrn->progClock = TRUE;

    /* Process options */
    xf86CollectOptions(pScrn, NULL);
    if(!(pNv->Options = malloc(sizeof(G80Options)))) goto fail;
    memcpy(pNv->Options, G80Options, sizeof(G80Options));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pNv->Options);

    from = X_DEFAULT;
    pNv->HWCursor = TRUE;
    if(xf86GetOptValBool(pNv->Options, OPTION_HW_CURSOR, &pNv->HWCursor))
        from = X_CONFIG;
    xf86DrvMsg(pScrn->scrnIndex, from, "Using %s cursor\n",
               pNv->HWCursor ? "hardware" : "software");
    if(xf86ReturnOptValBool(pNv->Options, OPTION_NOACCEL, FALSE)) {
        pNv->NoAccel = TRUE;
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Acceleration disabled\n");
    }
    s = xf86GetOptValString(pNv->Options, OPTION_ACCEL_METHOD);
    if(!s || !strcasecmp(s, "xaa"))
        pNv->AccelMethod = XAA;
    else if(!strcasecmp(s, "exa"))
        pNv->AccelMethod = EXA;
    else {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Unrecognized AccelMethod "
        "\"%s\".\n", s);
        goto fail;
    }

    pNv->Dither = xf86ReturnOptValBool(pNv->Options, OPTION_FP_DITHER, FALSE);
    pNv->AllowDualLink = xf86ReturnOptValBool(pNv->Options, OPTION_ALLOW_DUAL_LINK, FALSE);

    /* Set the bits per RGB for 8bpp mode */
    if(pScrn->depth == 8)
        pScrn->rgbBits = 8;

    if(!xf86SetGamma(pScrn, gzeros)) goto fail;

    /* Map memory */
    pScrn->memPhysBase = MEMBASE(pPci, 1);
    pScrn->fbOffset = 0;

#if XSERVER_LIBPCIACCESS
    err = pci_device_map_range(pPci, pPci->regions[0].base_addr, G80_REG_SIZE,
                               PCI_DEV_MAP_FLAG_WRITABLE, &p);
    if(err) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Failed to map MMIO registers: %s\n", strerror(err));
        goto fail;
    }
    pNv->reg = p;
#else
    pcitag = pciTag(pPci->bus, pPci->device, pPci->func);
    pNv->reg = xf86MapPciMem(pScrn->scrnIndex,
                             VIDMEM_MMIO | VIDMEM_READSIDEEFFECT,
                             pcitag, pPci->memBase[0], G80_REG_SIZE);
#endif
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "MMIO registers mapped at %p\n",
               (void*)pNv->reg);

#ifndef XSERVER_LIBPCIACCESS
    if(xf86RegisterResources(pEnt->index, NULL, ResExclusive)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "xf86RegisterResources() found "
                   "resource conflicts\n");
        goto fail;
    }
#endif

    pNv->architecture = pNv->reg[0] >> 20 & 0x1ff;
    tmp = pNv->reg[0x0010020C/4];
    pNv->videoRam = pNv->RamAmountKBytes = tmp >> 10 | (tmp & 1) << 22;

    /* Determine the size of BAR1 */
    /* Some configs have BAR1 < total RAM < 256 MB */
#if XSERVER_LIBPCIACCESS
    BAR1sizeKB = pPci->regions[1].size / 1024;
#else
    BAR1sizeKB = 1UL << (pPci->size[1] - 10);
#endif
    if(BAR1sizeKB > 256 * 1024) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "BAR1 is > 256 MB, which is "
                   "probably wrong.  Clamping to 256 MB.\n");
        BAR1sizeKB = 256 * 1024;
    }

    /* Limit videoRam to the size of BAR1 */
    if(pNv->videoRam <= 1024 || BAR1sizeKB == 0) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to determine the amount of "
                   "available video memory\n");
        goto fail;
    }
    pNv->videoRam -= 1024;
    if(pNv->videoRam > BAR1sizeKB)
        pNv->videoRam = BAR1sizeKB;

    pScrn->videoRam = pNv->videoRam;

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Total video RAM: %.1f MB\n",
               pNv->RamAmountKBytes / 1024.0);
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "      BAR1 size: %.1f MB\n",
               BAR1sizeKB / 1024.0);
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "  Mapped memory: %.1f MB\n",
               pScrn->videoRam / 1024.0);

#if XSERVER_LIBPCIACCESS
    err = pci_device_map_range(pPci, pPci->regions[1].base_addr,
                               pScrn->videoRam * 1024,
                               PCI_DEV_MAP_FLAG_WRITABLE |
                               PCI_DEV_MAP_FLAG_WRITE_COMBINE,
                               &p);
    if(err) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Failed to map framebuffer: %s\n", strerror(err));
        goto fail;
    }
    pNv->mem = p;
#else
    pNv->mem = xf86MapPciMem(pScrn->scrnIndex,
                             VIDMEM_MMIO | VIDMEM_READSIDEEFFECT,
                             pcitag, pPci->memBase[1], pScrn->videoRam * 1024);
#endif
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Linear framebuffer mapped at %p\n",
               (void*)pNv->mem);

    pNv->table1 = (unsigned const char*)&pNv->reg[0x00800000/4];
    tmp = pNv->reg[0x00619F04/4] >> 8;
    if(tmp)
        pNv->table1 -= ((pNv->RamAmountKBytes << 10) - (tmp << 16));
    else
        pNv->table1 -= 0x10000;

    xf86CrtcConfigInit(pScrn, &randr12_screen_funcs);
    xf86CrtcSetSizeRange(pScrn, 320, 200, 8192, 8192);

    if(!xf86LoadSubModule(pScrn, "i2c")) goto fail;
    if(!xf86LoadSubModule(pScrn, "ddc")) goto fail;

    if(!G80DispPreInit(pScrn)) goto fail;
    /* Read the DDC routing table and create outputs */
    if(!G80CreateOutputs(pScrn)) goto fail;
    /* Create the crtcs */
    G80DispCreateCrtcs(pScrn);

    /* We can grow the desktop if XAA is disabled */
    if(!xf86InitialConfiguration(pScrn, pNv->NoAccel || pNv->AccelMethod == EXA)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
            "No valid initial configuration found\n");
        goto fail;
    }
    pScrn->displayWidth = (pScrn->virtualX + 255) & ~255;

    if(!xf86RandR12PreInit(pScrn)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "RandR initialization failure\n");
        goto fail;
    }
    if(!pScrn->modes) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No modes\n");
        goto fail;
    }

    pScrn->currentMode = pScrn->modes;
    xf86PrintModes(pScrn);
    xf86SetDpi(pScrn, 0, 0);

    /* Load fb */
    if(!xf86LoadSubModule(pScrn, "fb")) goto fail;

    if(!pNv->NoAccel) {
        switch(pNv->AccelMethod) {
        case XAA:
            if(!xf86LoadSubModule(pScrn, "xaa")) pNv->NoAccel = 1;
            break;
        case EXA:
            if(!xf86LoadSubModule(pScrn, "exa")) pNv->NoAccel = 1;
            break;
        }
    }

    /* Load ramdac if needed */
    if(pNv->HWCursor) {
        if(!xf86LoadSubModule(pScrn, "ramdac")) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to load ramdac. "
                       "Falling back to software cursor.\n");
            pNv->HWCursor = FALSE;
	}
    }

    return TRUE;

fail:
    if(pNv->int10) xf86FreeInt10(pNv->int10);
    G80FreeRec(pScrn);
    return FALSE;
}

/*
 * Initialize the display and set the current mode.
 */
static Bool
AcquireDisplay(ScrnInfoPtr pScrn)
{
    if(!G80DispInit(pScrn))
        return FALSE;
    if(!G80CursorAcquire(pScrn))
        return FALSE;
    xf86SetDesiredModes(pScrn);

    return TRUE;
}

/*
 * Tear down the display and restore the console mode.
 */
static Bool
ReleaseDisplay(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);

    G80CursorRelease(pScrn);
    G80DispShutdown(pScrn);

    if(pNv->int10 && pNv->int10Mode) {
        xf86Int10InfoPtr int10 = pNv->int10;

        /* Use int10 to restore the console mode */
        int10->num = 0x10;
        int10->ax = 0x4f02;
        int10->bx = pNv->int10Mode | 0x8000;
        int10->cx = int10->dx = 0;
        xf86ExecX86int10(int10);
    }

    return TRUE;
}

static Bool
G80CloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    G80Ptr pNv = G80PTR(pScrn);

    if(pScrn->vtSema)
        ReleaseDisplay(pScrn);

#ifdef HAVE_XAA_H
    if(pNv->xaa)
        XAADestroyInfoRec(pNv->xaa);
#endif
    if(pNv->exa) {
        if(pNv->exaScreenArea) {
            exaOffscreenFree(pScreen, pNv->exaScreenArea);
            pNv->exaScreenArea = NULL;
        }
        exaDriverFini(pScrn->pScreen);
    }
    xf86_cursors_fini(pScreen);

    if(xf86ServerIsExiting()) {
        if(pNv->int10) xf86FreeInt10(pNv->int10);
#if XSERVER_LIBPCIACCESS
        pci_device_unmap_range(pNv->pPci, pNv->mem, pNv->videoRam * 1024);
        pci_device_unmap_range(pNv->pPci, (void*)pNv->reg, G80_REG_SIZE);
#else
        xf86UnMapVidMem(pScrn->scrnIndex, pNv->mem, pNv->videoRam * 1024);
        xf86UnMapVidMem(pScrn->scrnIndex, (void*)pNv->reg, G80_REG_SIZE);
#endif
        pNv->reg = NULL;
        pNv->mem = NULL;
    }

    pScrn->vtSema = FALSE;
    pScreen->CloseScreen = pNv->CloseScreen;
    pScreen->BlockHandler = pNv->BlockHandler;
    return (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);
}

static void
G80BlockHandler(BLOCKHANDLER_ARGS_DECL)
{
    SCREEN_PTR(arg);
    ScrnInfoPtr pScrnInfo = xf86ScreenToScrn(pScreen);
    G80Ptr pNv = G80PTR(pScrnInfo);

    if(pNv->DMAKickoffCallback)
        (*pNv->DMAKickoffCallback)(pScrnInfo);

    G80OutputResetCachedStatus(pScrnInfo);

    pScreen->BlockHandler = pNv->BlockHandler;
    (*pScreen->BlockHandler) (BLOCKHANDLER_ARGS);
    pScreen->BlockHandler = G80BlockHandler;
}

static Bool
G80SaveScreen(ScreenPtr pScreen, int mode)
{
    return FALSE;
}

static void
G80InitHW(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);
    CARD32 bar0_pramin = 0;
    const int pitch = pScrn->displayWidth * (pScrn->bitsPerPixel / 8);
    volatile CARD32 *p;

    /* Clear registers */
    for(p = &pNv->reg[0x00700000/4]; p < (const CARD32*)pNv->table1; p++)
        *p = 0;

    bar0_pramin = pNv->reg[0x00001700/4] << 16;

    pNv->reg[0x00000200/4] = 0xffff00ff;
    pNv->reg[0x00000200/4] = 0xffffffff;
    pNv->reg[0x00002100/4] = 0xffffffff;
    pNv->reg[0x0000250c/4] = 0x6f3cfc34;
    pNv->reg[0x00400804/4] = 0xc0000000;
    pNv->reg[0x00406800/4] = 0xc0000000;
    pNv->reg[0x00400c04/4] = 0xc0000000;
    pNv->reg[0x00401800/4] = 0xc0000000;
    pNv->reg[0x00405018/4] = 0xc0000000;
    pNv->reg[0x00402000/4] = 0xc0000000;
    pNv->reg[0x00400108/4] = 0xffffffff;
    pNv->reg[0x00400100/4] = 0xffffffff;

    if(pNv->architecture != 0x50) {
        pNv->reg[0x00700000/4] = 0x00000001;
        pNv->reg[0x00700004/4] = bar0_pramin + 0x00200;
        pNv->reg[0x00700020/4] = 0x00190002;
        pNv->reg[0x00700024/4] = bar0_pramin + 0x7ffff;
        pNv->reg[0x00700028/4] = bar0_pramin + 0x20000;
        pNv->reg[0x00700034/4] = 0x00010000;
    } else {
        pNv->reg[0x00700200/4] = 0x00190002;
        pNv->reg[0x00700204/4] = bar0_pramin + 0x7ffff;
        pNv->reg[0x00700208/4] = bar0_pramin + 0x20000;
        pNv->reg[0x00700214/4] = 0x00010000;
    }

    pNv->reg[0x00710004/4] = 0x00100642;
    pNv->reg[0x00710008/4] = 0x80000011;
    pNv->reg[0x0071000c/4] = 0x00000644;
    pNv->reg[0x00710010/4] = 0x80000012;
    pNv->reg[0x00710014/4] = 0x00100646;
    pNv->reg[0x00710018/4] = 0x80000013;
    pNv->reg[0x0071001c/4] = 0x00100648;
    pNv->reg[0x00710020/4] = 0x80000014;
    pNv->reg[0x00710024/4] = 0x0000064a;
    pNv->reg[0x00706420/4] = 0x00190030;
    pNv->reg[0x00706434/4] = 0x00010000;
    pNv->reg[0x00706440/4] = 0x0019003d;
    pNv->reg[0x00706444/4] = (pNv->videoRam << 10) - 0x4001;
    pNv->reg[0x00706448/4] = (pNv->videoRam << 10) - G80_RESERVED_VIDMEM;
    pNv->reg[0x00706454/4] = 0x00010000;
    pNv->reg[0x00706460/4] = 0x0000502d;
    pNv->reg[0x00706474/4] = 0x00010000;
    if(pNv->architecture == 0xaa || pNv->architecture == 0xac) {
        uint64_t base = (uint64_t)pNv->reg[0x00100E10/4] << 12;
        size_t size = (uint64_t)pNv->reg[0x00100E14/4] << 12;
        uint64_t limit = base + size - G80_RESERVED_VIDMEM;

        pNv->reg[0x00706480/4] = 0x1a003d;
        pNv->reg[0x00706484/4] = limit;
        pNv->reg[0x00706488/4] = base;
        pNv->reg[0x0070648c/4] = base >> 32 | ((limit >> 8) & 0xff000000);
    } else {
        pNv->reg[0x00706480/4] = 0x0019003d;
        pNv->reg[0x00706484/4] = (pNv->videoRam << 10) - G80_RESERVED_VIDMEM;
        pNv->reg[0x00706494/4] = 0x00010000;
    }
    pNv->reg[0x007064a0/4] = 0x0019003d;
    pNv->reg[0x007064a4/4] = bar0_pramin + 0x1100f;
    pNv->reg[0x007064a8/4] = bar0_pramin + 0x11000;
    pNv->reg[0x007064b4/4] = 0x00010000;

    if(pNv->architecture != 0x50) {
        pNv->reg[0x00002604/4] = 0x80000002 | (bar0_pramin >> 8);
    } else {
        pNv->reg[0x00002604/4] = 0x80000000 | (bar0_pramin >> 12);
    }

    pNv->reg[0x00003224/4] = 0x000f0078;
    pNv->reg[0x0000322c/4] = 0x00000644;
    pNv->reg[0x00003234/4] = G80_RESERVED_VIDMEM - 0x6001;
    pNv->reg[0x00003254/4] = 0x00000001;
    pNv->reg[0x00002210/4] = 0x1c001000;

    if(pNv->architecture != 0x50) {
        pNv->reg[0x0000340c/4] = (bar0_pramin + 0x1000) >> 10;
        pNv->reg[0x00003410/4] = (bar0_pramin >> 12);
    }

    pNv->reg[0x00400824/4] = 0x00004000;
    pNv->reg[0x00400784/4] = 0x80000000 | (bar0_pramin >> 12);
    pNv->reg[0x00400320/4] = 0x00000004;
    pNv->reg[0x0040032C/4] = 0x80000000 | (bar0_pramin >> 12);
    pNv->reg[0x00400500/4] = 0x00010001;
    pNv->reg[0x00003250/4] = 0x00000001;
    pNv->reg[0x00003200/4] = 0x00000001;
    pNv->reg[0x00003220/4] = 0x00001001;
    pNv->reg[0x00003204/4] = 0x00010001;

    pNv->dmaBase = (CARD32*)(pNv->mem + (pNv->videoRam << 10) -
        G80_RESERVED_VIDMEM);
    memset(pNv->dmaBase, 0, SKIPS*4);

    pNv->dmaPut = 0;
    pNv->dmaCurrent = SKIPS;
    pNv->dmaMax = (G80_RESERVED_VIDMEM - 0x6000) / 4 - 2;
    pNv->dmaFree = pNv->dmaMax - pNv->dmaCurrent;

    G80DmaStart(pNv, 0, 1);
    G80DmaNext (pNv, 0x80000012);
    G80DmaStart(pNv, 0x180, 3);
    G80DmaNext (pNv, 0x80000014);
    G80DmaNext (pNv, 0x80000013);
    G80DmaNext (pNv, 0x80000013);
    G80DmaStart(pNv, 0x200, 2);
    switch(pScrn->depth) {
        case  8: G80DmaNext (pNv, 0x000000f3); break;
        case 15: G80DmaNext (pNv, 0x000000f8); break;
        case 16: G80DmaNext (pNv, 0x000000e8); break;
        case 24: G80DmaNext (pNv, 0x000000e6); break;
    }
    G80DmaNext (pNv, 0x00000001);
    G80DmaStart(pNv, 0x214, 5);
    G80DmaNext (pNv, pitch);
    G80DmaNext (pNv, pitch);
    G80DmaNext (pNv, pNv->offscreenHeight);
    G80DmaNext (pNv, 0x00000000);
    G80DmaNext (pNv, 0x00000000);
    G80DmaStart(pNv, 0x230, 2);
    switch(pScrn->depth) {
        case  8: G80DmaNext (pNv, 0x000000f3); break;
        case 15: G80DmaNext (pNv, 0x000000f8); break;
        case 16: G80DmaNext (pNv, 0x000000e8); break;
        case 24: G80DmaNext (pNv, 0x000000e6); break;
    }
    G80DmaNext (pNv, 0x00000001);
    G80DmaStart(pNv, 0x244, 5);
    G80DmaNext (pNv, pitch);
    G80DmaNext (pNv, pitch);
    G80DmaNext (pNv, pNv->offscreenHeight);
    G80DmaNext (pNv, 0x00000000);
    G80DmaNext (pNv, 0x00000000);
    G80DmaStart(pNv, 0x260, 1);
    G80DmaNext (pNv, 0x00000001);
    G80DmaStart(pNv, 0x290, 1);
    G80DmaNext (pNv, 1);
    G80DmaStart(pNv, 0x29c, 1);
    G80DmaNext (pNv, 0);
    G80DmaStart(pNv, 0x2e8, 2);
    switch(pScrn->depth) {
        case  8: G80DmaNext (pNv, 3); break;
        case 15: G80DmaNext (pNv, 1); break;
        case 16: G80DmaNext (pNv, 0); break;
        case 24: G80DmaNext (pNv, 2); break;
    }
    G80DmaNext (pNv, 1);
    G80DmaStart(pNv, 0x584, 1);
    switch(pScrn->depth) {
        case  8: G80DmaNext (pNv, 0xf3); break;
        case 15: G80DmaNext (pNv, 0xf8); break;
        case 16: G80DmaNext (pNv, 0xe8); break;
        case 24: G80DmaNext (pNv, 0xe6); break;
    }
    G80DmaStart(pNv, 0x58c, 1);
    G80DmaNext (pNv, 0x111);
    G80DmaStart(pNv, 0x804, 1);
    switch(pScrn->depth) {
        case  8: G80DmaNext (pNv, 0xf3); break;
        case 15: G80DmaNext (pNv, 0xf8); break;
        case 16: G80DmaNext (pNv, 0xe8); break;
        case 24: G80DmaNext (pNv, 0xe6); break;
    }

    pNv->currentRop = ~0; /* Set to something invalid */
}

static Bool
G80ScreenInit(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr pScrn;
    G80Ptr pNv;
    CARD32 pitch;
    int visualMask, i;
    BoxRec AvailFBArea;

    /* First get the ScrnInfoRec */
    pScrn = xf86ScreenToScrn(pScreen);
    pNv = G80PTR(pScrn);

    pScrn->vtSema = TRUE;

    /* DIX visual init */
    miClearVisualTypes();
    visualMask = miGetDefaultVisualMask(pScrn->depth);
    if(!miSetVisualTypes(pScrn->depth, visualMask, 8, pScrn->defaultVisual))
        return FALSE;
    if(!miSetPixmapDepths())
        return FALSE;

    /* pad the screen pitch to 256 bytes */
    pitch = pScrn->displayWidth * (pScrn->bitsPerPixel / 8);

    /* fb init */
    if(!fbScreenInit(pScreen, pNv->mem,
                     pScrn->virtualX, pScrn->virtualY,
                     pScrn->xDpi, pScrn->yDpi,
                     pScrn->displayWidth, pScrn->bitsPerPixel))
        return FALSE;

    if(pScrn->bitsPerPixel > 8) {
        VisualPtr visual;

        /* Fixup RGB ordering */
        visual = pScreen->visuals + pScreen->numVisuals;
        while(--visual >= pScreen->visuals) {
            if((visual->class | DynamicClass) == DirectColor) {
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

    pNv->offscreenHeight = ((pNv->videoRam << 10) - G80_RESERVED_VIDMEM) / pitch;
    if(pNv->offscreenHeight > 32767) pNv->offscreenHeight = 32767;
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
        "%.2f MB available for offscreen pixmaps\n",
        (pNv->offscreenHeight - pScrn->virtualY) * pitch / 1024.0 / 1024.0);

    AvailFBArea.x1 = 0;
    AvailFBArea.y1 = 0;
    AvailFBArea.x2 = pScrn->displayWidth;
    AvailFBArea.y2 = pNv->offscreenHeight;
    xf86InitFBManager(pScreen, &AvailFBArea);

    pNv->reg[0x00001708/4] = 0;
    for(i = 0; i < 8; i++)
        pNv->reg[0x00001900/4 + i] = 0;

    if(!pNv->NoAccel) {
        G80InitHW(pScrn);
        switch(pNv->AccelMethod) {
        case XAA:
            if(!G80XAAInit(pScreen)) {
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                           "XAA hardware acceleration initialization failed\n");
                return FALSE;
            }
            break;
        case EXA:
            if(!G80ExaInit(pScreen, pScrn)) {
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                           "EXA hardware acceleration initialization failed\n");
                return FALSE;
            }
            break;
        }
    }

    xf86SetBackingStore(pScreen);
    xf86SetSilkenMouse(pScreen);

    /* Initialize software cursor.
       Must precede creation of the default colormap */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    if(!xf86CrtcScreenInit(pScreen))
        return FALSE;

    /* Initialize default colormap */
    if(!miCreateDefColormap(pScreen))
        return FALSE;

    /* Initialize colormap layer.
       Must follow initialization of the default colormap */
    if(!xf86HandleColormaps(pScreen, 256, 8, G80LoadPalette, NULL,
                            CMAP_PALETTED_TRUECOLOR))
        return FALSE;

    xf86DPMSInit(pScreen, xf86DPMSSet, 0);

#ifdef HAVE_XAA_H
    /* Clear the screen */
    if(pNv->xaa) {
        /* Use the acceleration engine */
        pNv->xaa->SetupForSolidFill(pScrn, 0, GXcopy, ~0);
        pNv->xaa->SubsequentSolidFillRect(pScrn,
            0, 0, pScrn->displayWidth, pNv->offscreenHeight);
        G80DmaKickoff(pNv);
    } else
#endif
    {
        /* Use a slow software clear path */
        memset(pNv->mem, 0, pitch * pNv->offscreenHeight);
    }

    /* Initialize the display */
    if(!AcquireDisplay(pScrn))
        return FALSE;

    /* Initialize hardware cursor.  Must follow software cursor initialization. */
    if(pNv->HWCursor && !G80CursorInit(pScreen)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Hardware cursor initialization failed\n");
        pNv->HWCursor = FALSE;
    }

    pScreen->SaveScreen = G80SaveScreen;

    pNv->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = G80CloseScreen;

    pNv->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = G80BlockHandler;

    return TRUE;
}

static void
G80FreeScreen(FREE_SCREEN_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    G80FreeRec(pScrn);
}

static Bool
G80SwitchMode(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    return xf86SetSingleMode(pScrn, mode, RR_Rotate_0);
}

static void
G80AdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
}

static Bool
G80EnterVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    G80Ptr pNv = G80PTR(pScrn);

    /* Reinit the hardware */
#ifdef HAVE_XAA_H
    if(pNv->xaa)
        G80InitHW(pScrn);
#endif

    if(!AcquireDisplay(pScrn))
        return FALSE;

    return TRUE;
}

static void
G80LeaveVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);

    ReleaseDisplay(pScrn);
}

Bool G80GetScrnInfoRec(PciChipsets *chips, int chip)
{
    ScrnInfoPtr pScrn;

    pScrn = xf86ConfigPciEntity(NULL, 0, chip,
                                chips, NULL, NULL, NULL,
                                NULL, NULL);

    if(!pScrn) return FALSE;

    pScrn->driverVersion    = NV_VERSION;
    pScrn->driverName       = NV_DRIVER_NAME;
    pScrn->name             = NV_NAME;

    pScrn->PreInit          = G80PreInit;
    pScrn->ScreenInit       = G80ScreenInit;
    pScrn->SwitchMode       = G80SwitchMode;
    pScrn->AdjustFrame      = G80AdjustFrame;
    pScrn->EnterVT          = G80EnterVT;
    pScrn->LeaveVT          = G80LeaveVT;
    pScrn->FreeScreen       = G80FreeScreen;
    // pScrn->ValidMode        = G80ValidMode;

    return TRUE;
}
