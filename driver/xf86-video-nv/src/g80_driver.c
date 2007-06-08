/*
 * Copyright (c) 2007 NVIDIA, Corporation
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

#include <xf86_OSproc.h>
#include <xf86Resources.h>
#include <mipointer.h>
#include <mibstore.h>
#include <micmap.h>
#include <xf86cmap.h>
#include <fb.h>
#define DPMS_SERVER
#include <X11/extensions/dpms.h>

#include "nv_const.h"
#include "g80_type.h"
#include "g80_cursor.h"
#include "g80_display.h"
#include "g80_ddc.h"
#include "g80_dma.h"
#include "g80_xaa.h"

#define G80_REG_SIZE (1024 * 1024 * 16)
#define G80_RESERVED_VIDMEM 0xd000

static const char *fbSymbols[] = {
    "fbPictureInit",
    "fbScreenInit",
    NULL
};

static const char *xaaSymbols[] = {
    "XAACopyROP",
    "XAACreateInfoRec",
    "XAADestroyInfoRec",
    "XAAFallbackOps",
    "XAAInit",
    "XAAPatternROP",
    NULL
};

static const char *i2cSymbols[] = {
    "xf86CreateI2CBusRec",
    "xf86I2CBusInit",
    NULL
};

static const char *ramdacSymbols[] = {
    "xf86CreateCursorInfoRec",
    "xf86DestroyCursorInfoRec",
    "xf86InitCursor",
    NULL
};

static const char *ddcSymbols[] = {
    "xf86PrintEDID",
    "xf86DoEDID_DDC2",
    "xf86SetDDCproperties",
    NULL
};

static const char *int10Symbols[] = {
    "xf86FreeInt10",
    "xf86InitInt10",
    "xf86ExecX86int10",
    NULL
};

typedef enum {
    OPTION_HW_CURSOR,
    OPTION_NOACCEL,
    OPTION_BACKEND_MODE,
} G80Opts;

static const OptionInfoRec G80Options[] = {
    { OPTION_HW_CURSOR,         "HWCursor",     OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_NOACCEL,           "NoAccel",      OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_BACKEND_MODE,      "BackendMode",  OPTV_ANYSTR,    {0}, FALSE },
    { -1,                       NULL,           OPTV_NONE,      {0}, FALSE }
};

static Bool
G80GetRec(ScrnInfoPtr pScrn)
{
    if(pScrn->driverPrivate == NULL)
        pScrn->driverPrivate = xcalloc(sizeof(G80Rec), 1);

    return (pScrn->driverPrivate != NULL);
}

static void
G80FreeRec(ScrnInfoPtr pScrn)
{
    xfree(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
}

static Bool
G80PreInit(ScrnInfoPtr pScrn, int flags)
{
    G80Ptr pNv;
    EntityInfoPtr pEnt;
    pciVideoPtr pPci;
    PCITAG pcitag;
    ClockRangePtr clockRanges;
    MessageType from;
    Bool primary;
    int i;
    char *s;
    const rgb zeros = {0, 0, 0};
    const Gamma gzeros = {0.0, 0.0, 0.0};
    CARD32 tmp;

    if(flags & PROBE_DETECT) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                "G80 PROBE_DETECT unimplemented\n");
        return FALSE;
    }

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
    pcitag = pciTag(pPci->bus, pPci->device, pPci->func);
    primary = xf86IsPrimaryPci(pPci);

    /* The ROM size sometimes isn't read correctly, so fix it up here. */
    if(pPci->biosSize == 0)
        /* The BIOS is 64k */
        pPci->biosSize = 16;

    pNv->int10 = NULL;
    pNv->int10Mode = 0;
    if(xf86LoadSubModule(pScrn, "int10")) {
        xf86LoaderReqSymLists(int10Symbols, NULL);
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
    xf86SetOperatingState(resVgaIo, pEnt->index, ResUnusedOpr);
    xf86SetOperatingState(resVgaMem, pEnt->index, ResDisableOpr);

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
    if(!(pNv->Options = xalloc(sizeof(G80Options)))) goto fail;
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

    /* Set the bits per RGB for 8bpp mode */
    if(pScrn->depth == 8)
        pScrn->rgbBits = 8;

    if(!xf86SetGamma(pScrn, gzeros)) goto fail;

    /*
     * Setup the ClockRanges, which describe what clock ranges are available,
     * and what sort of modes they can be used for.
     */
    clockRanges = xnfcalloc(sizeof(ClockRange), 1);
    clockRanges->next = NULL;
    clockRanges->minClock = 0;
    clockRanges->maxClock = 400000;
    clockRanges->clockIndex = -1;       /* programmable */
    clockRanges->doubleScanAllowed = TRUE;
    clockRanges->interlaceAllowed = TRUE;

    /* Map memory */
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "MMIO registers at 0x%lx\n",
               pPci->memBase[0]);
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Linear framebuffer at 0x%lx\n",
               pPci->memBase[1]);
    pScrn->memPhysBase = pPci->memBase[1];
    pScrn->fbOffset = 0;

    pNv->reg = xf86MapPciMem(pScrn->scrnIndex,
                             VIDMEM_MMIO | VIDMEM_READSIDEEFFECT,
                             pcitag, pPci->memBase[0], G80_REG_SIZE);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "MMIO registers mapped at %p\n",
               (void*)pNv->reg);

    if(xf86RegisterResources(pEnt->index, NULL, ResExclusive)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "xf86RegisterResources() found "
                   "resource conflicts\n");
        goto fail;
    }

    pNv->architecture = pNv->reg[0] >> 20 & 0x1ff;
    pNv->RamAmountKBytes = pNv->RamAmountKBytes = (pNv->reg[0x0010020C/4] & 0xFFF00000) >> 10;
    pNv->videoRam = pNv->RamAmountKBytes;
    /* Limit videoRam to the max BAR1 size of 256MB */
    if(pNv->videoRam <= 1024) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to determine the amount of "
                   "available video memory\n");
        goto fail;
    }
    pNv->videoRam -= 1024;
    if(pNv->videoRam > 256 * 1024)
        pNv->videoRam = 256 * 1024;
    pScrn->videoRam = pNv->videoRam;
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Mapping %.1f of %.1f MB of video RAM\n",
               pScrn->videoRam / 1024.0, pNv->RamAmountKBytes / 1024.0);
    pNv->mem = xf86MapPciMem(pScrn->scrnIndex,
                             VIDMEM_MMIO | VIDMEM_READSIDEEFFECT,
                             pcitag, pPci->memBase[1], pScrn->videoRam * 1024);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Linear framebuffer mapped at %p\n",
               (void*)pNv->mem);

    pNv->table1 = (unsigned const char*)&pNv->reg[0x00800000/4];
    tmp = pNv->reg[0x00619F04/4] >> 8;
    if(tmp)
        pNv->table1 -= ((pNv->RamAmountKBytes << 10) - (tmp << 16));
    else
        pNv->table1 -= 0x10000;

    /* Probe DDC */
    /* If no DDC info found, try DAC load detection */
    if(!xf86LoadSubModule(pScrn, "i2c")) goto fail;
    if(!xf86LoadSubModule(pScrn, "ddc")) goto fail;
    xf86LoaderReqSymLists(i2cSymbols, ddcSymbols, NULL);
    if(!G80ProbeDDC(pScrn) && !G80LoadDetect(pScrn)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No display devices found\n");
        goto fail;
    }
    /* Hardcode HEAD0 for now.  RandR 1.2 will move this into a Crtc struct. */
    pNv->head = 0;

    i = xf86ValidateModes(pScrn, pScrn->monitor->Modes,
                          pScrn->display->modes, clockRanges,
                          NULL, 256, 8192,
                          512, 128, 8192,
                          pScrn->display->virtualX,
                          pScrn->display->virtualY,
                          pNv->videoRam * 1024 - G80_RESERVED_VIDMEM,
                          LOOKUP_BEST_REFRESH);
    if(i == -1) goto fail;
    xf86PruneDriverModes(pScrn);
    if(i == 0 || !pScrn->modes) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
        goto fail;
    }
    xf86SetCrtcForModes(pScrn, 0);

    pScrn->currentMode = pScrn->modes;
    xf86PrintModes(pScrn);
    xf86SetDpi(pScrn, 0, 0);

    /* Custom backend timings */
    pNv->BackendMode = NULL;
    if((s = xf86GetOptValString(pNv->Options, OPTION_BACKEND_MODE))) {
        DisplayModePtr mode;

        for(mode = pScrn->modes; ; mode = mode->next) {
            if(!strcmp(mode->name, s))
                break;
            if(mode->next == pScrn->modes) {
                mode = NULL;
                break;
            }
        }

        pNv->BackendMode = mode;

        if(mode)
            xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "BackendMode: Using mode "
                       "\"%s\" for display timings\n", mode->name);
        else
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Cannot honor "
                       "\"BackendMode\" option: no mode named \"%s\" "
                       "found.\n", s);
    }

    /* Load fb */
    if(!xf86LoadSubModule(pScrn, "fb")) goto fail;
    xf86LoaderReqSymLists(fbSymbols, NULL);

    if(!pNv->NoAccel) {
        if(!xf86LoadSubModule(pScrn, "xaa")) goto fail;
        xf86LoaderReqSymLists(xaaSymbols, NULL);
    }

    /* Load ramdac if needed */
    if(pNv->HWCursor) {
        if(!xf86LoadSubModule(pScrn, "ramdac")) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Failed to load ramdac. "
                       "Falling back to software cursor.\n");
            pNv->HWCursor = FALSE;
        } else {
            xf86LoaderReqSymLists(ramdacSymbols, NULL);
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
    G80Ptr pNv = G80PTR(pScrn);

    if(!G80DispInit(pScrn))
        return FALSE;
    if(!G80CursorAcquire(pNv))
        return FALSE;
    if(!G80DispSetMode(pScrn, pScrn->currentMode))
        return FALSE;
    G80DispDPMSSet(pScrn, DPMSModeOn, 0);

    return TRUE;
}

/*
 * Tear down the display and restore the console mode.
 */
static Bool
ReleaseDisplay(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);

    G80CursorRelease(pNv);
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
G80CloseScreen(int scrnIndex, ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    G80Ptr pNv = G80PTR(pScrn);

    if(pScrn->vtSema)
        ReleaseDisplay(pScrn);

    if(pNv->xaa)
        XAADestroyInfoRec(pNv->xaa);
    if(pNv->HWCursor)
        xf86DestroyCursorInfoRec(pNv->CursorInfo);

    if(xf86ServerIsExiting()) {
        if(pNv->int10) xf86FreeInt10(pNv->int10);
        xf86UnMapVidMem(pScrn->scrnIndex, pNv->mem, pNv->videoRam * 1024);
        xf86UnMapVidMem(pScrn->scrnIndex, (void*)pNv->reg, G80_REG_SIZE);
        pNv->reg = NULL;
        pNv->mem = NULL;
    }

    pScrn->vtSema = FALSE;
    pScreen->CloseScreen = pNv->CloseScreen;
    pScreen->BlockHandler = pNv->BlockHandler;
    return (*pScreen->CloseScreen)(scrnIndex, pScreen);
}

static void
G80BlockHandler(int i, pointer blockData, pointer pTimeout, pointer pReadmask)
{
    ScreenPtr pScreen = screenInfo.screens[i];
    ScrnInfoPtr pScrnInfo = xf86Screens[i];
    G80Ptr pNv = G80PTR(pScrnInfo);

    if(pNv->DMAKickoffCallback)
        (*pNv->DMAKickoffCallback)(pScrnInfo);

    pScreen->BlockHandler = pNv->BlockHandler;
    (*pScreen->BlockHandler) (i, blockData, pTimeout, pReadmask);
    pScreen->BlockHandler = G80BlockHandler;
}

static Bool
G80SaveScreen(ScreenPtr pScreen, int mode)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];

    if(!pScrn->vtSema) return FALSE;

    G80DispBlankScreen(pScrn, !xf86IsUnblank(mode));

    return TRUE;
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
    pNv->reg[0x00706480/4] = 0x0019003d;
    pNv->reg[0x00706484/4] = (pNv->videoRam << 10) - G80_RESERVED_VIDMEM;
    pNv->reg[0x00706494/4] = 0x00010000;
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
    pNv->reg[0x00003234/4] = G80_RESERVED_VIDMEM - 0x5001;
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
    pNv->dmaMax = (G80_RESERVED_VIDMEM - 0x5000) / 4 - 2;
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

#define DEPTH_SHIFT(val, w) ((val << (8 - w)) | (val >> ((w << 1) - 8)))
#define COLOR(c) (unsigned int)(0x3fff * ((c)/255.0))
static void
G80LoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors,
               VisualPtr pVisual)
{
    G80Ptr pNv = G80PTR(pScrn);
    int i, index;
    volatile struct {
        unsigned short red, green, blue, unused;
    } *lut = (void*)&pNv->mem[pNv->videoRam * 1024 - 0x5000];

    switch(pScrn->depth) {
        case 15:
            for(i = 0; i < numColors; i++) {
                index = indices[i];
                lut[DEPTH_SHIFT(index, 5)].red = COLOR(colors[index].red);
                lut[DEPTH_SHIFT(index, 5)].green = COLOR(colors[index].green);
                lut[DEPTH_SHIFT(index, 5)].blue = COLOR(colors[index].blue);
            }
            break;
        case 16:
            for(i = 0; i < numColors; i++) {
                index = indices[i];
                lut[DEPTH_SHIFT(index, 6)].green = COLOR(colors[index].green);
                if(index < 32) {
                    lut[DEPTH_SHIFT(index, 5)].red = COLOR(colors[index].red);
                    lut[DEPTH_SHIFT(index, 5)].blue = COLOR(colors[index].blue);
                }
            }
            break;
        default:
            for(i = 0; i < numColors; i++) {
                index = indices[i];
                lut[index].red = COLOR(colors[index].red);
                lut[index].green = COLOR(colors[index].green);
                lut[index].blue = COLOR(colors[index].blue);
            }
            break;
    }
}

static Bool
G80ScreenInit(int scrnIndex, ScreenPtr pScreen, int argc, char **argv)
{
    ScrnInfoPtr pScrn;
    G80Ptr pNv;
    CARD32 pitch;
    int visualMask;
    BoxRec AvailFBArea;

    /* First get the ScrnInfoRec */
    pScrn = xf86Screens[pScreen->myNum];
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
    pitch = (pitch + 0xff) & ~0xff;
    pScrn->displayWidth = pitch / (pScrn->bitsPerPixel / 8);

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

    if(!pNv->NoAccel) {
        G80InitHW(pScrn);
        if(!G80XAAInit(pScreen)) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Hardware acceleration initialization failed\n");
            pNv->NoAccel = FALSE;
        }
    }

    miInitializeBackingStore(pScreen);
    xf86SetBackingStore(pScreen);
    xf86SetSilkenMouse(pScreen);

    /* Initialize software cursor.
       Must precede creation of the default colormap */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    /* Initialize hardware cursor.  Must follow software cursor initialization. */
    if(pNv->HWCursor && !G80CursorInit(pScreen)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Hardware cursor initialization failed\n");
        pNv->HWCursor = FALSE;
    }

    /* Initialize default colormap */
    if(!miCreateDefColormap(pScreen))
        return FALSE;

    /* Initialize colormap layer.
       Must follow initialization of the default colormap */
    if(!xf86HandleColormaps(pScreen, 256, 8, G80LoadPalette, NULL,
                            CMAP_PALETTED_TRUECOLOR))
        return FALSE;

    xf86DPMSInit(pScreen, G80DispDPMSSet, 0);

    /* Clear the screen */
    if(pNv->xaa) {
        /* Use the acceleration engine */
        pNv->xaa->SetupForSolidFill(pScrn, 0, GXcopy, ~0);
        pNv->xaa->SubsequentSolidFillRect(pScrn,
            0, 0, pScrn->displayWidth, pNv->offscreenHeight);
        G80DmaKickoff(pNv);
    } else {
        /* Use a slow software clear path */
        memset(pNv->mem, 0, pitch * pNv->offscreenHeight);
    }

    /* Initialize the display */
    if(!AcquireDisplay(pScrn))
        return FALSE;

    pScreen->SaveScreen = G80SaveScreen;

    pNv->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = G80CloseScreen;

    pNv->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = G80BlockHandler;

    return TRUE;
}

static void
G80FreeScreen(int scrnIndex, int flags)
{
    G80FreeRec(xf86Screens[scrnIndex]);
}

static Bool
G80SwitchMode(int scrnIndex, DisplayModePtr mode, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];

    return G80DispSetMode(pScrn, mode);
}

static void
G80AdjustFrame(int scrnIndex, int x, int y, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    G80Ptr pNv = G80PTR(pScrn);

    if(x + pScrn->currentMode->HDisplay > pScrn->virtualX ||
       y + pScrn->currentMode->VDisplay > pScrn->virtualY ||
       x < 0 || y < 0)
        /* Ignore bogus panning */
        return;
    G80DispAdjustFrame(pNv, x, y);
}

static Bool
G80EnterVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];
    G80Ptr pNv = G80PTR(pScrn);

    /* Reinit the hardware */
    if(pNv->xaa)
        G80InitHW(pScrn);

    if(!AcquireDisplay(pScrn))
        return FALSE;

    return TRUE;
}

static void
G80LeaveVT(int scrnIndex, int flags)
{
    ScrnInfoPtr pScrn = xf86Screens[scrnIndex];

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
