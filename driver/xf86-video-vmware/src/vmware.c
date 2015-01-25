/* **********************************************************
 * Copyright (C) 1998-2001 VMware, Inc.
 * All Rights Reserved
 * **********************************************************/
#ifdef VMX86_DEVEL
char rcsId_vmware[] =
    "Id: vmware.c,v 1.11 2001/02/23 02:10:39 yoel Exp $";
#endif

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * TODO: support the vmware linux kernel fb driver (Option "UseFBDev").
 */

#include "xf86.h"
#include "xf86_OSproc.h"

#include "compiler.h"	/* inb/outb */

#include "xf86Pci.h"		/* pci */

#include "mipointer.h"		/* sw cursor */
#include "micmap.h"		/* mi color map */
#include "vgaHW.h"		/* VGA hardware */
#include "fb.h"
#include "shadowfb.h"           /* ShadowFB wrappers */

#include "xf86cmap.h"		/* xf86HandleColormaps */

#include "vmware.h"
#include "guest_os.h"
#include "vm_device_version.h"
#include "svga_modes.h"
#include "vmware_bootstrap.h"
#include "vmware_common.h"

#ifndef HAVE_XORG_SERVER_1_5_0
#include <xf86_ansic.h>
#include <xf86_libc.h>
#endif

#if (GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) >= 5)

#define xf86LoaderReqSymLists(...) do {} while (0)
#define LoaderRefSymLists(...) do {} while (0)

#else

const char *vgahwSymbols[] = {
    "vgaHWGetHWRec",
    "vgaHWGetIOBase",
    "vgaHWGetIndex",
    "vgaHWInit",
    "vgaHWProtect",
    "vgaHWRestore",
    "vgaHWSave",
    "vgaHWSaveScreen",
    "vgaHWUnlock",
    NULL
};

static const char *fbSymbols[] = {
    "fbCreateDefColormap",
    "fbPictureInit",
    "fbScreenInit",
    NULL
};

static const char *ramdacSymbols[] = {
    "xf86CreateCursorInfoRec",
    "xf86DestroyCursorInfoRec",
    "xf86InitCursor",
    NULL
};

static const char *shadowfbSymbols[] = {
    "ShadowFBInit2",
    NULL
};
#endif

/* Table of default modes to always add to the mode list. */

typedef struct {
   int width;
   int height;
} VMWAREDefaultMode;

#define VMW_MIN_INITIAL_WIDTH 800
#define VMW_MIN_INITIAL_HEIGHT 600

#define SVGA_DEFAULT_MODE(width, height) { width, height, },

static const VMWAREDefaultMode VMWAREDefaultModes[] = {
   SVGA_DEFAULT_MODES
};

#undef SVGA_DEFAULT_MODE

static void VMWAREStopFIFO(ScrnInfoPtr pScrn);
static void VMWARESave(ScrnInfoPtr pScrn);

static Bool
VMWAREGetRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate != NULL) {
        return TRUE;
    }
    pScrn->driverPrivate = xnfcalloc(sizeof(VMWARERec), 1);
    /* FIXME: Initialize driverPrivate... */
    return TRUE;
}

static void
VMWAREFreeRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate) {
        free(pScrn->driverPrivate);
        pScrn->driverPrivate = NULL;
    }
}

CARD32
vmwareReadReg(VMWAREPtr pVMWARE, int index)
{
    /*
     * Block SIGIO for the duration, so we don't get interrupted after the
     * outl but before the inl by a mouse move (which write to our registers).
     */
    int oldsigio, ret;
    oldsigio = xf86BlockSIGIO();
    outl(pVMWARE->indexReg, index);
    ret = inl(pVMWARE->valueReg);
    xf86UnblockSIGIO(oldsigio);
    return ret;
}

void
vmwareWriteReg(VMWAREPtr pVMWARE, int index, CARD32 value)
{
    /*
     * Block SIGIO for the duration, so we don't get interrupted in between
     * the outls by a mouse move (which write to our registers).
     */
    int oldsigio;
    oldsigio = xf86BlockSIGIO();
    outl(pVMWARE->indexReg, index);
    outl(pVMWARE->valueReg, value);
    xf86UnblockSIGIO(oldsigio);
}

void
vmwareWriteWordToFIFO(VMWAREPtr pVMWARE, CARD32 value)
{
    volatile CARD32* vmwareFIFO = pVMWARE->vmwareFIFO;

    /* Need to sync? */
    if ((vmwareFIFO[SVGA_FIFO_NEXT_CMD] + sizeof(CARD32) == vmwareFIFO[SVGA_FIFO_STOP])
     || (vmwareFIFO[SVGA_FIFO_NEXT_CMD] == vmwareFIFO[SVGA_FIFO_MAX] - sizeof(CARD32) &&
	 vmwareFIFO[SVGA_FIFO_STOP] == vmwareFIFO[SVGA_FIFO_MIN])) {
        VmwareLog(("Syncing because of full fifo\n"));
        vmwareWaitForFB(pVMWARE);
    }

    vmwareFIFO[vmwareFIFO[SVGA_FIFO_NEXT_CMD] / sizeof(CARD32)] = value;

    write_mem_barrier();

    if(vmwareFIFO[SVGA_FIFO_NEXT_CMD] == vmwareFIFO[SVGA_FIFO_MAX] -
       sizeof(CARD32)) {
        vmwareFIFO[SVGA_FIFO_NEXT_CMD] = vmwareFIFO[SVGA_FIFO_MIN];
    } else {
        vmwareFIFO[SVGA_FIFO_NEXT_CMD] += sizeof(CARD32);
    }
}

void
vmwareWaitForFB(VMWAREPtr pVMWARE)
{
    vmwareWriteReg(pVMWARE, SVGA_REG_SYNC, 1);
    while (vmwareReadReg(pVMWARE, SVGA_REG_BUSY));
}

void
vmwareSendSVGACmdUpdate(VMWAREPtr pVMWARE, BoxPtr pBB)
{
    vmwareWriteWordToFIFO(pVMWARE, SVGA_CMD_UPDATE);
    vmwareWriteWordToFIFO(pVMWARE, pBB->x1);
    vmwareWriteWordToFIFO(pVMWARE, pBB->y1);
    vmwareWriteWordToFIFO(pVMWARE, pBB->x2 - pBB->x1);
    vmwareWriteWordToFIFO(pVMWARE, pBB->y2 - pBB->y1);
}

void
vmwareSendSVGACmdUpdateFullScreen(VMWAREPtr pVMWARE)
{
    BoxRec BB;

    BB.x1 = 0;
    BB.y1 = 0;
    BB.x2 = pVMWARE->ModeReg.svga_reg_width;
    BB.y2 = pVMWARE->ModeReg.svga_reg_height;
    vmwareSendSVGACmdUpdate(pVMWARE, &BB);
}

static CARD32
vmwareCalculateWeight(CARD32 mask)
{
    CARD32 weight;

    for (weight = 0; mask; mask >>= 1) {
        if (mask & 1) {
            weight++;
        }
    }
    return weight;
}

/*
 *-----------------------------------------------------------------------------
 *
 * VMXGetVMwareSvgaId --
 *
 *    Retrieve the SVGA_ID of the VMware SVGA adapter.
 *    This function should hide any backward compatibility mess.
 *
 * Results:
 *    The SVGA_ID_* of the present VMware adapter.
 *
 * Side effects:
 *    ins/outs
 *
 *-----------------------------------------------------------------------------
 */

static uint32
VMXGetVMwareSvgaId(VMWAREPtr pVMWARE)
{
    uint32 vmware_svga_id;

    /* Any version with any SVGA_ID_* support will initialize SVGA_REG_ID
     * to SVGA_ID_0 to support versions of this driver with SVGA_ID_0.
     *
     * Versions of SVGA_ID_0 ignore writes to the SVGA_REG_ID register.
     *
     * Versions of SVGA_ID_1 will allow us to overwrite the content
     * of the SVGA_REG_ID register only with the values SVGA_ID_0 or SVGA_ID_1.
     *
     * Versions of SVGA_ID_2 will allow us to overwrite the content
     * of the SVGA_REG_ID register only with the values SVGA_ID_0 or SVGA_ID_1
     * or SVGA_ID_2.
     */

    vmwareWriteReg(pVMWARE, SVGA_REG_ID, SVGA_ID_2);
    vmware_svga_id = vmwareReadReg(pVMWARE, SVGA_REG_ID);
    if (vmware_svga_id == SVGA_ID_2) {
        return SVGA_ID_2;
    }

    vmwareWriteReg(pVMWARE, SVGA_REG_ID, SVGA_ID_1);
    vmware_svga_id = vmwareReadReg(pVMWARE, SVGA_REG_ID);
    if (vmware_svga_id == SVGA_ID_1) {
        return SVGA_ID_1;
    }

    if (vmware_svga_id == SVGA_ID_0) {
        return SVGA_ID_0;
    }

    /* No supported VMware SVGA devices found */
    return SVGA_ID_INVALID;
}

static Bool
VMWAREPreInit(ScrnInfoPtr pScrn, int flags)
{
    MessageType from;
    VMWAREPtr pVMWARE;
    OptionInfoPtr options;
    int bpp24flags;
    uint32 id;
    int i;
    ClockRange* clockRanges;
    unsigned long domainIOBase = 0;
    uint32 width = 0, height = 0;
    Bool defaultMode;

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 12
#ifndef BUILD_FOR_420
    domainIOBase = pScrn->domainIOBase;
#endif
#endif

    if (flags & PROBE_DETECT) {
        return FALSE;
    }

    if (pScrn->numEntities != 1) {
        return FALSE;
    }

    if (!VMWAREGetRec(pScrn)) {
        return FALSE;
    }
    pVMWARE = VMWAREPTR(pScrn);

    pVMWARE->pvtSema = &pScrn->vtSema;

    pVMWARE->pEnt = xf86GetEntityInfo(pScrn->entityList[0]);
    pVMWARE->PciInfo = xf86GetPciInfoForEntity(pVMWARE->pEnt->index);
    if (pVMWARE->PciInfo == NULL) {
        return FALSE;
    }

    if (DEVICE_ID(pVMWARE->PciInfo) == PCI_DEVICE_ID_VMWARE_SVGA) {
        pVMWARE->indexReg = domainIOBase +
           SVGA_LEGACY_BASE_PORT + SVGA_INDEX_PORT*sizeof(uint32);
        pVMWARE->valueReg = domainIOBase +
           SVGA_LEGACY_BASE_PORT + SVGA_VALUE_PORT*sizeof(uint32);
    } else {
        /* Note:  This setting of valueReg causes unaligned I/O */
#if XSERVER_LIBPCIACCESS
        pVMWARE->portIOBase = pVMWARE->PciInfo->regions[0].base_addr;
#else
        pVMWARE->portIOBase = pVMWARE->PciInfo->ioBase[0];
#endif
        pVMWARE->indexReg = domainIOBase +
           pVMWARE->portIOBase + SVGA_INDEX_PORT;
        pVMWARE->valueReg = domainIOBase +
           pVMWARE->portIOBase + SVGA_VALUE_PORT;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
               "VMware SVGA regs at (0x%04lx, 0x%04lx)\n",
               pVMWARE->indexReg, pVMWARE->valueReg);

    if (!xf86LoadSubModule(pScrn, "vgahw")) {
        return FALSE;
    }

    xf86LoaderReqSymLists(vgahwSymbols, NULL);

    if (!vgaHWGetHWRec(pScrn)) {
        return FALSE;
    }

#ifdef HAVE_XORG_SERVER_1_12_0
    vgaHWSetStdFuncs(VGAHWPTR(pScrn));
#endif

    /*
     * Save the current video state.  Do it here before VMXGetVMwareSvgaId
     * writes to any registers.
     */
    VMWARESave(pScrn);

    id = VMXGetVMwareSvgaId(pVMWARE);
    if (id == SVGA_ID_0 || id == SVGA_ID_INVALID) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "No supported VMware SVGA found (read ID 0x%08x).\n", id);
        return FALSE;
    }
    pVMWARE->suspensionSavedRegId = id;

#if !XSERVER_LIBPCIACCESS
    pVMWARE->PciTag = pciTag(pVMWARE->PciInfo->bus, pVMWARE->PciInfo->device,
                             pVMWARE->PciInfo->func);
#endif
    pVMWARE->Primary = xf86IsPrimaryPci(pVMWARE->PciInfo);

    pScrn->monitor = pScrn->confScreen->monitor;

#ifdef ACCELERATE_OPS
    pVMWARE->vmwareCapability = vmwareReadReg(pVMWARE, SVGA_REG_CAPABILITIES);
#else
    pVMWARE->vmwareCapability = vmwareReadReg(pVMWARE, SVGA_REG_CAPABILITIES) &
	SVGA_CAP_PITCHLOCK;
#endif

    pVMWARE->bitsPerPixel = vmwareReadReg(pVMWARE,
                                          SVGA_REG_HOST_BITS_PER_PIXEL);
    if (pVMWARE->vmwareCapability & SVGA_CAP_8BIT_EMULATION) {
       vmwareWriteReg(pVMWARE, SVGA_REG_BITS_PER_PIXEL, pVMWARE->bitsPerPixel);
    }

    pVMWARE->depth = vmwareReadReg(pVMWARE, SVGA_REG_DEPTH);
    pVMWARE->videoRam = vmwareReadReg(pVMWARE, SVGA_REG_VRAM_SIZE);
    pVMWARE->memPhysBase = vmwareReadReg(pVMWARE, SVGA_REG_FB_START);
    pVMWARE->maxWidth = vmwareReadReg(pVMWARE, SVGA_REG_MAX_WIDTH);
    pVMWARE->maxHeight = vmwareReadReg(pVMWARE, SVGA_REG_MAX_HEIGHT);
    pVMWARE->cursorDefined = FALSE;
    pVMWARE->cursorShouldBeHidden = FALSE;

    if (pVMWARE->vmwareCapability & SVGA_CAP_CURSOR_BYPASS_2) {
        pVMWARE->cursorRemoveFromFB = SVGA_CURSOR_ON_REMOVE_FROM_FB;
        pVMWARE->cursorRestoreToFB = SVGA_CURSOR_ON_RESTORE_TO_FB;
    } else {
        pVMWARE->cursorRemoveFromFB = SVGA_CURSOR_ON_HIDE;
        pVMWARE->cursorRestoreToFB = SVGA_CURSOR_ON_SHOW;
    }

    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "caps:  0x%08X\n", pVMWARE->vmwareCapability);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "depth: %d\n", pVMWARE->depth);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "bpp:   %d\n", pVMWARE->bitsPerPixel);

    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "vram:  %d\n", pVMWARE->videoRam);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "pbase: 0x%08lx\n", pVMWARE->memPhysBase);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "mwidt: %d\n", pVMWARE->maxWidth);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED, 2, "mheig: %d\n", pVMWARE->maxHeight);

    if (pVMWARE->vmwareCapability & SVGA_CAP_8BIT_EMULATION) {
        bpp24flags = Support24bppFb | Support32bppFb;
    } else {
        switch (pVMWARE->depth) {
        case 16:
            /*
             * In certain cases, the Windows host appears to
             * report 16 bpp and 16 depth but 555 weight.  Just
             * silently convert it to depth of 15.
             */
            if (pVMWARE->bitsPerPixel == 16 &&
                pVMWARE->weight.green == 5)
                pVMWARE->depth = 15;
        case 8:
        case 15:
            bpp24flags = NoDepth24Support;
         break;
        case 32:
            /*
             * There is no 32 bit depth, apparently it can get
             * reported this way sometimes on the Windows host.
             */
            if (pVMWARE->bitsPerPixel == 32)
                pVMWARE->depth = 24;
        case 24:
            if (pVMWARE->bitsPerPixel == 24)
                bpp24flags = Support24bppFb;
            else
                bpp24flags = Support32bppFb;
            break;
       default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Adapter is using an unsupported depth (%d).\n",
                       pVMWARE->depth);
            return FALSE;
       }
    }

    if (!xf86SetDepthBpp(pScrn, pVMWARE->depth, pVMWARE->bitsPerPixel,
                         pVMWARE->bitsPerPixel, bpp24flags)) {
        return FALSE;
    }

    /* Check that the returned depth is one we support */
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
        return FALSE;
    }

    if (pScrn->bitsPerPixel != pVMWARE->bitsPerPixel) {
        if (pVMWARE->vmwareCapability & SVGA_CAP_8BIT_EMULATION) {
            vmwareWriteReg(pVMWARE, SVGA_REG_BITS_PER_PIXEL,
                           pScrn->bitsPerPixel);
            pVMWARE->bitsPerPixel =
               vmwareReadReg(pVMWARE, SVGA_REG_BITS_PER_PIXEL);
            pVMWARE->depth = vmwareReadReg(pVMWARE, SVGA_REG_DEPTH);
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Currently unavailable depth/bpp of %d/%d requested.\n"
                       "\tThe guest X server must run at the same depth and bpp as the host\n"
                       "\t(which are currently %d/%d).  This is automatically detected.  Please\n"
                       "\tdo not specify a depth on the command line or via the config file.\n",
                       pScrn->depth, pScrn->bitsPerPixel,
                       pVMWARE->depth, pVMWARE->bitsPerPixel);
            return FALSE;
        }
    }

    /*
     * Defer reading the colour registers until here in case we changed
     * bpp above.
     */

    pVMWARE->weight.red =
       vmwareCalculateWeight(vmwareReadReg(pVMWARE, SVGA_REG_RED_MASK));
    pVMWARE->weight.green =
       vmwareCalculateWeight(vmwareReadReg(pVMWARE, SVGA_REG_GREEN_MASK));
    pVMWARE->weight.blue =
       vmwareCalculateWeight(vmwareReadReg(pVMWARE, SVGA_REG_BLUE_MASK));
    pVMWARE->offset.blue = 0;
    pVMWARE->offset.green = pVMWARE->weight.blue;
    pVMWARE->offset.red = pVMWARE->weight.green + pVMWARE->offset.green;
    pVMWARE->defaultVisual = vmwareReadReg(pVMWARE, SVGA_REG_PSEUDOCOLOR) ?
       PseudoColor : TrueColor;

    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "depth: %d\n", pVMWARE->depth);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "bpp:   %d\n", pVMWARE->bitsPerPixel);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "w.red: %d\n", (int)pVMWARE->weight.red);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "w.grn: %d\n", (int)pVMWARE->weight.green);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "w.blu: %d\n", (int)pVMWARE->weight.blue);
    xf86DrvMsgVerb(pScrn->scrnIndex, X_PROBED,
                   2, "vis:   %d\n", pVMWARE->defaultVisual);

    if (pScrn->depth != pVMWARE->depth) {
        if (pVMWARE->vmwareCapability & SVGA_CAP_8BIT_EMULATION) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Currently unavailable depth of %d requested.\n"
                       "\tIf the guest X server's BPP matches the host's "
                       "BPP, then\n\tthe guest X server's depth must also "
                       "match the\n\thost's depth (currently %d).\n",
                       pScrn->depth, pVMWARE->depth);
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Currently unavailable depth of %d requested.\n"
                       "\tThe guest X server must run at the same depth as "
                       "the host (which\n\tis currently %d).  This is "
                       "automatically detected.  Please do not\n\tspecify "
                       "a depth on the command line or via the config file.\n",
                       pScrn->depth, pVMWARE->depth);
        }
           return FALSE;
    }
    xf86PrintDepthBpp(pScrn);

#if 0
    if (pScrn->depth == 24 && pix24bpp == 0) {
        pix24bpp = xf86GetBppFromDepth(pScrn, 24);
    }
#endif

    if (pScrn->depth > 8) {
        rgb zeros = { 0, 0, 0 };

        if (!xf86SetWeight(pScrn, pVMWARE->weight, zeros)) {
            return FALSE;
        }
        /* FIXME check returned weight */
    }
    if (!xf86SetDefaultVisual(pScrn, pVMWARE->defaultVisual)) {
        return FALSE;
    }
    if (pScrn->defaultVisual != pVMWARE->defaultVisual) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Given visual (%d) is not supported by this driver (%d is required)\n",
                   pScrn->defaultVisual, pVMWARE->defaultVisual);
        return FALSE;
    }
#if 0
    bytesPerPixel = pScrn->bitsPerPixel / 8;
#endif
    pScrn->progClock = TRUE;

#if 0 /* MGA does not do this */
    if (pScrn->visual != 0) {	/* FIXME */
        /* print error message */
        return FALSE;
    }
#endif

    xf86CollectOptions(pScrn, NULL);
    if (!(options = VMWARECopyOptions()))
        return FALSE;
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, options);

    if (pScrn->depth <= 8) {
        pScrn->rgbBits = 8;
    }

    if (!pScrn->chipset) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "ChipID 0x%04x is not recognised\n", DEVICE_ID(pVMWARE->PciInfo));
        return FALSE;
    }

    from = X_DEFAULT;
    pVMWARE->hwCursor = TRUE;
    if (xf86GetOptValBool(options, OPTION_HW_CURSOR, &pVMWARE->hwCursor)) {
        from = X_CONFIG;
    }
    if (pVMWARE->hwCursor && !(pVMWARE->vmwareCapability & SVGA_CAP_CURSOR)) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "HW cursor is not supported in this configuration\n");
        from = X_PROBED;
        pVMWARE->hwCursor = FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Using %s cursor\n",
               pVMWARE->hwCursor ? "HW" : "SW");
    pScrn->videoRam = pVMWARE->videoRam / 1024;
    pScrn->memPhysBase = pVMWARE->memPhysBase;

    from = X_DEFAULT;
    defaultMode = TRUE;
    if (xf86GetOptValBool(options, OPTION_DEFAULT_MODE, &defaultMode)) {
        from = X_CONFIG;
    }

    width = vmwareReadReg(pVMWARE, SVGA_REG_WIDTH);
    height = vmwareReadReg(pVMWARE, SVGA_REG_HEIGHT);
    width = MAX(width, VMW_MIN_INITIAL_WIDTH);
    height = MAX(height, VMW_MIN_INITIAL_HEIGHT);

    if (width > pVMWARE->maxWidth || height > pVMWARE->maxHeight) {
	/*
	 * This is an error condition and shouldn't happen.
	 * revert to MIN_INITIAL_ values
	 */
	width = VMW_MIN_INITIAL_WIDTH;
	height = VMW_MIN_INITIAL_HEIGHT;
    }

    xf86DrvMsg(pScrn->scrnIndex, from,
	       "Will %sset up a driver mode with dimensions %dx%d.\n",
	       defaultMode ? "" : "not ", width, height);

    free(options);

    {
        Gamma zeros = { 0.0, 0.0, 0.0 };
        if (!xf86SetGamma(pScrn, zeros)) {
            return FALSE;
        }
    }
#if 0
    if ((i = xf86GetPciInfoForScreen(pScrn->scrnIndex, &pciList, NULL)) != 1) {
        /* print error message */
        VMWAREFreeRec(pScrn);
        if (i > 0) {
            free(pciList);
        }
        return FALSE;
    }
#endif
    clockRanges = xnfcalloc(sizeof(ClockRange), 1);
    clockRanges->next = NULL;
    clockRanges->minClock = 1;
    clockRanges->maxClock = 400000000;
    clockRanges->clockIndex = -1;
    clockRanges->interlaceAllowed = FALSE;
    clockRanges->doubleScanAllowed = FALSE;
    clockRanges->ClockMulFactor = 1;
    clockRanges->ClockDivFactor = 1;

    if (defaultMode) {
	vmwareAddDefaultMode(pScrn, width, height);
    }

    i = xf86ValidateModes(pScrn, pScrn->monitor->Modes, pScrn->display->modes,
                          clockRanges, NULL, 256, pVMWARE->maxWidth,
                          pVMWARE->bitsPerPixel * 1,
                          128, pVMWARE->maxHeight,
                          pScrn->display->virtualX, pScrn->display->virtualY,
                          pVMWARE->videoRam,
                          LOOKUP_BEST_REFRESH | LOOKUP_OPTIONAL_TOLERANCES);

    if (i == -1) {
        VMWAREFreeRec(pScrn);
        return FALSE;
    }
    xf86PruneDriverModes(pScrn);
    if (i == 0 || pScrn->modes == NULL) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
        VMWAREFreeRec(pScrn);
        return FALSE;
    }

    pScrn->currentMode = pScrn->modes;
    pScrn->virtualX = pScrn->modes->HDisplay;
    pScrn->virtualY = pScrn->modes->VDisplay;

    xf86SetCrtcForModes(pScrn, INTERLACE_HALVE_V);

    xf86PrintModes(pScrn);
    xf86SetDpi(pScrn, 0, 0);
    if (!xf86LoadSubModule(pScrn, "fb") ||
        !xf86LoadSubModule(pScrn, "shadowfb")) {
        VMWAREFreeRec(pScrn);
        return FALSE;
    }
    xf86LoaderReqSymLists(fbSymbols, shadowfbSymbols, NULL);

    /* Need ramdac for hwcursor */
    if (pVMWARE->hwCursor) {
        if (!xf86LoadSubModule(pScrn, "ramdac")) {
            VMWAREFreeRec(pScrn);
            return FALSE;
        }
        xf86LoaderReqSymLists(ramdacSymbols, NULL);
    }

    return TRUE;
}

static Bool
VMWAREMapMem(ScrnInfoPtr pScrn)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
#if XSERVER_LIBPCIACCESS
    int err;
    struct pci_device *const device = pVMWARE->PciInfo;
    void *fbBase;
#endif

#if XSERVER_LIBPCIACCESS
   err = pci_device_map_range(device,
                              pVMWARE->memPhysBase,
                              pVMWARE->videoRam,
                              PCI_DEV_MAP_FLAG_WRITABLE,
			      &fbBase);
   if (err) {
       xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                  "Unable to map frame buffer BAR. %s (%d)\n",
                  strerror (err), err);
       return FALSE;
   }
   pVMWARE->FbBase = fbBase;
#else
    pVMWARE->FbBase = xf86MapPciMem(pScrn->scrnIndex, 0,
                                    pVMWARE->PciTag,
                                    pVMWARE->memPhysBase,
                                    pVMWARE->videoRam);
#endif
    if (!pVMWARE->FbBase)
        return FALSE;

    VmwareLog(("FB Mapped: %p/%u -> %p/%u\n",
               pVMWARE->memPhysBase, pVMWARE->videoRam,
               pVMWARE->FbBase, pVMWARE->videoRam));
    return TRUE;
}

static Bool
VMWAREUnmapMem(ScrnInfoPtr pScrn)
{
    VMWAREPtr pVMWARE;

    pVMWARE = VMWAREPTR(pScrn);

    VmwareLog(("Unmapped: %p/%u\n", pVMWARE->FbBase, pVMWARE->videoRam));

#if XSERVER_LIBPCIACCESS
    pci_device_unmap_range(pVMWARE->PciInfo, pVMWARE->FbBase, pVMWARE->videoRam);
#else
    xf86UnMapVidMem(pScrn->scrnIndex, pVMWARE->FbBase, pVMWARE->videoRam);
#endif
    pVMWARE->FbBase = NULL;
    return TRUE;
}

static void
VMWARESave(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg = &hwp->SavedReg;
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    VMWARERegPtr vmwareReg = &pVMWARE->SavedReg;

    vgaHWSave(pScrn, vgaReg, VGA_SR_ALL);

    vmwareReg->svga_reg_enable = vmwareReadReg(pVMWARE, SVGA_REG_ENABLE);
    vmwareReg->svga_reg_width = vmwareReadReg(pVMWARE, SVGA_REG_WIDTH);
    vmwareReg->svga_reg_height = vmwareReadReg(pVMWARE, SVGA_REG_HEIGHT);
    vmwareReg->svga_reg_bits_per_pixel =
       vmwareReadReg(pVMWARE, SVGA_REG_BITS_PER_PIXEL);
    vmwareReg->svga_reg_id = vmwareReadReg(pVMWARE, SVGA_REG_ID);

    /* XXX this should be based on the cap bit, not hwCursor... */
    if (pVMWARE->hwCursor) {
       vmwareReg->svga_reg_cursor_on =
          vmwareReadReg(pVMWARE, SVGA_REG_CURSOR_ON);
       vmwareReg->svga_reg_cursor_x =
          vmwareReadReg(pVMWARE, SVGA_REG_CURSOR_X);
       vmwareReg->svga_reg_cursor_y =
          vmwareReadReg(pVMWARE, SVGA_REG_CURSOR_Y);
       vmwareReg->svga_reg_cursor_id =
          vmwareReadReg(pVMWARE, SVGA_REG_CURSOR_ID);
    }

    vmwareReg->svga_fifo_enabled = vmwareReadReg(pVMWARE, SVGA_REG_CONFIG_DONE);
}

static void
VMWARERestoreRegs(ScrnInfoPtr pScrn, VMWARERegPtr vmwareReg)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    VmwareLog(("VMWARERestoreRegs: W: %d, H: %d, BPP: %d, Enable: %d\n",
	       vmwareReg->svga_reg_width, vmwareReg->svga_reg_height,
	       vmwareReg->svga_reg_bits_per_pixel, vmwareReg->svga_reg_enable));
    if (vmwareReg->svga_reg_enable) {
        vmwareWriteReg(pVMWARE, SVGA_REG_ID, vmwareReg->svga_reg_id);
        vmwareWriteReg(pVMWARE, SVGA_REG_WIDTH, vmwareReg->svga_reg_width);
        vmwareWriteReg(pVMWARE, SVGA_REG_HEIGHT, vmwareReg->svga_reg_height);
        vmwareWriteReg(pVMWARE, SVGA_REG_BITS_PER_PIXEL,
                       vmwareReg->svga_reg_bits_per_pixel);
        vmwareWriteReg(pVMWARE, SVGA_REG_ENABLE, vmwareReg->svga_reg_enable);
        vmwareWriteReg(pVMWARE, SVGA_REG_GUEST_ID, GUEST_OS_LINUX);
        if (pVMWARE->hwCursor) {
            vmwareWriteReg(pVMWARE, SVGA_REG_CURSOR_ID,
                           vmwareReg->svga_reg_cursor_id);
            vmwareWriteReg(pVMWARE, SVGA_REG_CURSOR_X,
                           vmwareReg->svga_reg_cursor_x);
            vmwareWriteReg(pVMWARE, SVGA_REG_CURSOR_Y,
                           vmwareReg->svga_reg_cursor_y);
            vmwareWriteReg(pVMWARE, SVGA_REG_CURSOR_ON,
                           vmwareReg->svga_reg_cursor_on);
        }
    } else {
        vmwareWriteReg(pVMWARE, SVGA_REG_ID, vmwareReg->svga_reg_id);
        vmwareWriteReg(pVMWARE, SVGA_REG_WIDTH, vmwareReg->svga_reg_width);
        vmwareWriteReg(pVMWARE, SVGA_REG_HEIGHT, vmwareReg->svga_reg_height);
        vmwareWriteReg(pVMWARE, SVGA_REG_BITS_PER_PIXEL,
                       vmwareReg->svga_reg_bits_per_pixel);
	vmwareWriteReg(pVMWARE, SVGA_REG_ENABLE, vmwareReg->svga_reg_enable);
    }
}

static void
VMWARERestore(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg = &hwp->SavedReg;
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    VMWARERegPtr vmwareReg = &pVMWARE->SavedReg;

    vmwareWaitForFB(pVMWARE);
    if (!vmwareReg->svga_fifo_enabled) {
        VMWAREStopFIFO(pScrn);
    }

    vgaHWProtect(pScrn, TRUE);
    VMWARERestoreRegs(pScrn, vmwareReg);
    vgaHWRestore(pScrn, vgaReg, VGA_SR_ALL);
    vgaHWProtect(pScrn, FALSE);
}

static Bool
VMWAREModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode, Bool rebuildPixmap)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg = &hwp->ModeReg;
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    VMWARERegPtr vmwareReg = &pVMWARE->ModeReg;

    vgaHWUnlock(hwp);
    if (!vgaHWInit(pScrn, mode))
        return FALSE;
    pScrn->vtSema = TRUE;

    if (pVMWARE->vmwareCapability & SVGA_CAP_PITCHLOCK)
	vmwareWriteReg(pVMWARE, SVGA_REG_PITCHLOCK, 0);
    vmwareReg->svga_reg_enable = 1;
    vmwareReg->svga_reg_width = max(mode->HDisplay, pScrn->virtualX);
    vmwareReg->svga_reg_height = max(mode->VDisplay, pScrn->virtualY);
    vmwareReg->svga_reg_bits_per_pixel = pVMWARE->bitsPerPixel;

    vgaHWProtect(pScrn, TRUE);

    vgaHWRestore(pScrn, vgaReg, VGA_SR_ALL);
    VMWARERestoreRegs(pScrn, vmwareReg);

    if (pVMWARE->hwCursor) {
        vmwareCursorModeInit(pScrn, mode);
    }

    VmwareLog(("Required mode: %ux%u\n", mode->HDisplay, mode->VDisplay));
    VmwareLog(("Virtual:       %ux%u\n", pScrn->virtualX, pScrn->virtualY));
    VmwareLog(("dispWidth:     %u\n", pScrn->displayWidth));
    pVMWARE->fbOffset = vmwareReadReg(pVMWARE, SVGA_REG_FB_OFFSET);
    pVMWARE->fbPitch = vmwareReadReg(pVMWARE, SVGA_REG_BYTES_PER_LINE);
    pVMWARE->FbSize = vmwareReadReg(pVMWARE, SVGA_REG_FB_SIZE);

    pScrn->displayWidth = (pVMWARE->fbPitch * 8) / ((pScrn->bitsPerPixel + 7) & ~7);
    VmwareLog(("fbOffset:      %u\n", pVMWARE->fbOffset));
    VmwareLog(("fbPitch:       %u\n", pVMWARE->fbPitch));
    VmwareLog(("fbSize:        %u\n", pVMWARE->FbSize));
    VmwareLog(("New dispWidth: %u\n", pScrn->displayWidth));

    vmwareCheckVideoSanity(pScrn);

    if (rebuildPixmap) {
        pScrn->pScreen->ModifyPixmapHeader((*pScrn->pScreen->GetScreenPixmap)(pScrn->pScreen),
                                           pScrn->pScreen->width,
                                           pScrn->pScreen->height,
                                           pScrn->pScreen->rootDepth,
                                           pScrn->bitsPerPixel,
                                           PixmapBytePad(pScrn->displayWidth,
                                                         pScrn->pScreen->rootDepth),
                                           (pointer)(pVMWARE->FbBase + pScrn->fbOffset));

        (*pScrn->EnableDisableFBAccess)(XF86_SCRN_ARG(pScrn), FALSE);
        (*pScrn->EnableDisableFBAccess)(XF86_SCRN_ARG(pScrn), TRUE);
    }

    vgaHWProtect(pScrn, FALSE);

    /*
     * Push the new Xinerama state to X clients and the hardware,
     * synchronously with the mode change. Note that this must happen
     * AFTER we write the new width and height to the hardware
     * registers, since updating the WIDTH and HEIGHT registers will
     * reset the device's multimon topology.
     */
    vmwareNextXineramaState(pVMWARE);

    return TRUE;
}

void
vmwareNextXineramaState(VMWAREPtr pVMWARE)
{
    VMWARERegPtr vmwareReg = &pVMWARE->ModeReg;

    /*
     * Switch to the next Xinerama state (from pVMWARE->xineramaNextState).
     *
     * This new state will be available to X clients via the Xinerama
     * extension, and we push the new state to the virtual hardware,
     * in order to configure a number of virtual monitors within the
     * device's framebuffer.
     *
     * This function can be called at any time, but it should usually be
     * called just after a mode switch. This is for two reasons:
     *
     *   1) We don't want X clients to see a Xinerama topology and a video
     *      mode that are inconsistent with each other, so we'd like to switch
     *      both at the same time.
     *
     *   2) We must set the host's display topology registers after setting
     *      the new video mode, since writes to WIDTH/HEIGHT will reset the
     *      hardware display topology.
     */

    /*
     * Update Xinerama info appropriately.
     */
    if (pVMWARE->xinerama && !pVMWARE->xineramaStatic) {
       if (pVMWARE->xineramaNextState) {
          free(pVMWARE->xineramaState);
          pVMWARE->xineramaState = pVMWARE->xineramaNextState;
          pVMWARE->xineramaNumOutputs = pVMWARE->xineramaNextNumOutputs;

          pVMWARE->xineramaNextState = NULL;
          pVMWARE->xineramaNextNumOutputs = 0;

       } else {
          /*
           * There is no next state pending. Switch back to
           * single-monitor mode. This is necessary for resetting the
           * Xinerama state if we get a mode change which doesn't
           * follow a VMwareCtrlDoSetTopology call.
           */
          VMWAREXineramaPtr basicState =
             (VMWAREXineramaPtr)calloc(1, sizeof (VMWAREXineramaRec));
          if (basicState) {
             basicState->x_org = 0;
             basicState->y_org = 0;
             basicState->width = vmwareReg->svga_reg_width;
             basicState->height = vmwareReg->svga_reg_height;

             free(pVMWARE->xineramaState);
             pVMWARE->xineramaState = basicState;
             pVMWARE->xineramaNumOutputs = 1;
          }
       }
    }

    /*
     * Update host's view of guest topology. This tells the device
     * how we're carving up its framebuffer into virtual screens.
     */
    if (pVMWARE->vmwareCapability & SVGA_CAP_DISPLAY_TOPOLOGY) {
        if (pVMWARE->xinerama) {
            int i = 0;
            VMWAREXineramaPtr xineramaState = pVMWARE->xineramaState;
            vmwareWriteReg(pVMWARE, SVGA_REG_NUM_GUEST_DISPLAYS,
                           pVMWARE->xineramaNumOutputs);

            for (i = 0; i < pVMWARE->xineramaNumOutputs; i++) {
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_ID, i);
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_IS_PRIMARY, i == 0);
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_POSITION_X,
                               xineramaState[i].x_org);
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_POSITION_Y,
                               xineramaState[i].y_org);
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_WIDTH,
                               xineramaState[i].width);
                vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_HEIGHT,
                               xineramaState[i].height);
            }
        } else {
            vmwareWriteReg(pVMWARE, SVGA_REG_NUM_GUEST_DISPLAYS, 1);

            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_ID, 0);
            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_IS_PRIMARY, TRUE);
            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_POSITION_X, 0);
            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_POSITION_Y, 0);
            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_WIDTH, vmwareReg->svga_reg_width);
            vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_HEIGHT, vmwareReg->svga_reg_height);
        }

        /* Done. */
        vmwareWriteReg(pVMWARE, SVGA_REG_DISPLAY_ID, SVGA_INVALID_DISPLAY_ID);
    }
}

static void
VMWAREAdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
    /* FIXME */
}

static void
VMWAREInitFIFO(ScrnInfoPtr pScrn)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
#if XSERVER_LIBPCIACCESS
    struct pci_device *const device = pVMWARE->PciInfo;
    int err;
    void *mmioVirtBase;
#endif
    volatile CARD32* vmwareFIFO;
    Bool extendedFifo;
    int min;

    TRACEPOINT

    pVMWARE->mmioPhysBase = vmwareReadReg(pVMWARE, SVGA_REG_MEM_START);
    pVMWARE->mmioSize = vmwareReadReg(pVMWARE, SVGA_REG_MEM_SIZE) & ~3;
#if XSERVER_LIBPCIACCESS
    err = pci_device_map_range(device, pVMWARE->mmioPhysBase,
                               pVMWARE->mmioSize,
                               PCI_DEV_MAP_FLAG_WRITABLE,
                               &mmioVirtBase);
    if (err) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Unable to map mmio BAR. %s (%d)\n",
                   strerror (err), err);
        return;
    }
    pVMWARE->mmioVirtBase = mmioVirtBase;
#else
    pVMWARE->mmioVirtBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO,
                                          pVMWARE->PciTag,
                                          pVMWARE->mmioPhysBase,
                                          pVMWARE->mmioSize);
#endif
    vmwareFIFO = pVMWARE->vmwareFIFO = (CARD32*)pVMWARE->mmioVirtBase;

    extendedFifo = pVMWARE->vmwareCapability & SVGA_CAP_EXTENDED_FIFO;
    min = extendedFifo ? vmwareReadReg(pVMWARE, SVGA_REG_MEM_REGS) : 4;

    vmwareWaitForFB(pVMWARE);
    vmwareWriteReg(pVMWARE, SVGA_REG_CONFIG_DONE, 0);

    vmwareFIFO[SVGA_FIFO_MIN] = min * sizeof(CARD32);
    vmwareFIFO[SVGA_FIFO_MAX] = pVMWARE->mmioSize;
    vmwareFIFO[SVGA_FIFO_NEXT_CMD] = min * sizeof(CARD32);
    vmwareFIFO[SVGA_FIFO_STOP] = min * sizeof(CARD32);
    vmwareWriteReg(pVMWARE, SVGA_REG_CONFIG_DONE, 1);
}

static void
VMWAREStopFIFO(ScrnInfoPtr pScrn)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);

    TRACEPOINT

    vmwareWriteReg(pVMWARE, SVGA_REG_CONFIG_DONE, 0);
#if XSERVER_LIBPCIACCESS
    pci_device_unmap_range(pVMWARE->PciInfo, pVMWARE->mmioVirtBase, pVMWARE->mmioSize);
#else
    xf86UnMapVidMem(pScrn->scrnIndex, pVMWARE->mmioVirtBase, pVMWARE->mmioSize);
#endif
}

static Bool
VMWARECloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    ScreenPtr save = &pVMWARE->ScrnFuncs;

    VmwareLog(("cursorSema: %d\n", pVMWARE->cursorSema));

    if (*pVMWARE->pvtSema) {
        if (pVMWARE->videoStreams) {
            vmwareVideoEnd(pScreen);
        }

        if (pVMWARE->CursorInfoRec) {
            vmwareCursorCloseScreen(pScreen);
        }

        VMWARERestore(pScrn);
        VMWAREUnmapMem(pScrn);

        pScrn->vtSema = FALSE;
    }

    pScreen->CloseScreen = save->CloseScreen;
    pScreen->SaveScreen = save->SaveScreen;

#if VMWARE_DRIVER_FUNC
    pScrn->DriverFunc = NULL;
#endif

    return (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);
}

static Bool
VMWARESaveScreen(ScreenPtr pScreen, int mode)
{
    VmwareLog(("VMWareSaveScreen() mode = %d\n", mode));

    /*
     * This thoroughly fails to do anything useful to svga mode.  I doubt
     * we care; who wants to idle-blank their VM's screen anyway?
     */
    return vgaHWSaveScreen(pScreen, mode);
}

/* disabled by default to reduce spew in DEBUG_LOGGING mode. */
/*#define DEBUG_LOG_UPDATES*/

static void
VMWAREPreDirtyBBUpdate(ScrnInfoPtr pScrn, int nboxes, BoxPtr boxPtr)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);

#ifdef DEBUG_LOG_UPDATES
    {
        int i;
        for (i = 0; i < nboxes; i++) {
            VmwareLog(("PreUpdate #%d (%d, %d, w = %d, h = %d)\n", nboxes - i,
                       boxPtr[i].x1, boxPtr[i].y1,
                       boxPtr[i].x2 - boxPtr[i].x1,
                       boxPtr[i].y2 - boxPtr[i].y1));
        }
    }
#endif

    /*
     * We only register this callback if we have a HW cursor.
     */
    while (nboxes--) {
        if (BOX_INTERSECT(*boxPtr, pVMWARE->hwcur.box)) {
            if (!pVMWARE->cursorExcludedForUpdate) {
                PRE_OP_HIDE_CURSOR();
                pVMWARE->cursorExcludedForUpdate = TRUE;
            }
	    break;
        }
        boxPtr++;
    }
}

static void
VMWAREPostDirtyBBUpdate(ScrnInfoPtr pScrn, int nboxes, BoxPtr boxPtr)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    while (nboxes--) {
#ifdef DEBUG_LOG_UPDATES
        VmwareLog(("PostUpdate #%d (%d, %d, w = %d, h = %d)\n", nboxes,
                   boxPtr->x1, boxPtr->y1,
                   boxPtr->x2 - boxPtr->x1, boxPtr->y2 - boxPtr->y1));
#endif

        /* Clip off (y only) for offscreen memory */
        if (boxPtr->y2 >= pVMWARE->ModeReg.svga_reg_height)
            boxPtr->y2 = pVMWARE->ModeReg.svga_reg_height;
        if (boxPtr->y1 >= pVMWARE->ModeReg.svga_reg_height)
            boxPtr->y1 = pVMWARE->ModeReg.svga_reg_height;
        if (boxPtr->y1 == boxPtr->y2) {
            boxPtr++;
            continue;
        }

        vmwareSendSVGACmdUpdate(pVMWARE, boxPtr++);
    }

    if (pVMWARE->hwCursor && pVMWARE->cursorExcludedForUpdate) {
        POST_OP_SHOW_CURSOR();
        pVMWARE->cursorExcludedForUpdate = FALSE;
    }
}

static void
VMWARELoadPalette(ScrnInfoPtr pScrn, int numColors, int* indices,
                  LOCO* colors, VisualPtr pVisual)
{
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);
    int i;

    for (i = 0; i < numColors; i++) {
        vmwareWriteReg(pVMWARE, SVGA_PALETTE_BASE + *indices * 3 + 0, colors[*indices].red);
        vmwareWriteReg(pVMWARE, SVGA_PALETTE_BASE + *indices * 3 + 1, colors[*indices].green);
        vmwareWriteReg(pVMWARE, SVGA_PALETTE_BASE + *indices * 3 + 2, colors[*indices].blue);
        indices++;
    }
    VmwareLog(("Palette loading done\n"));
}


DisplayModeRec *
VMWAREAddDisplayMode(ScrnInfoPtr pScrn,
                     const char *name,
                     int width,
                     int height)
{
   DisplayModeRec *mode;

   mode = malloc(sizeof(DisplayModeRec));
   memset(mode, 0, sizeof *mode);

   mode->name = malloc(strlen(name) + 1);
   strcpy(mode->name, name);
   mode->status = MODE_OK;
   mode->type = M_T_DEFAULT;
   mode->HDisplay = width;
   mode->VDisplay = height;

   mode->next = pScrn->modes;
   mode->prev = pScrn->modes->prev;
   pScrn->modes->prev->next = mode;
   pScrn->modes->prev = mode;

   return mode;
}


/*
 *-----------------------------------------------------------------------------
 *
 * vmwareIsRegionEqual --
 *
 *    This function implements REGION_EQUAL because older versions of
 *    regionstr.h don't define it.
 *    It is a slightly modified version of miRegionEqual from $Xorg: miregion.c
 *
 * Results:
 *    TRUE if regions are equal; FALSE otherwise
 *
 * Side effects:
 *    None.
 *
 *-----------------------------------------------------------------------------
 */

Bool
vmwareIsRegionEqual(const RegionPtr reg1,
                    const RegionPtr reg2)
{
    int i, num;
    BoxPtr rects1, rects2;

    if ((reg1->extents.x1 != reg2->extents.x1) ||
        (reg1->extents.x2 != reg2->extents.x2) ||
        (reg1->extents.y1 != reg2->extents.y1) ||
        (reg1->extents.y2 != reg2->extents.y2)) {
        return FALSE;
    }

    num = REGION_NUM_RECTS(reg1);
    if (num != REGION_NUM_RECTS(reg2)) {
        return FALSE;
    }

    rects1 = REGION_RECTS(reg1);
    rects2 = REGION_RECTS(reg2);

    for (i = 0; i < num; i++) {
        if ((rects1[i].x1 != rects2[i].x1) ||
            (rects1[i].x2 != rects2[i].x2) ||
            (rects1[i].y1 != rects2[i].y1) ||
            (rects1[i].y2 != rects2[i].y2)) {
            return FALSE;
        }
    }

    return TRUE;
}

static Bool
VMWAREScreenInit(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    vgaHWPtr hwp;
    VMWAREPtr pVMWARE;
    OptionInfoPtr options;
    Bool useXinerama = TRUE;

    pVMWARE = VMWAREPTR(pScrn);


    xf86CollectOptions(pScrn, NULL);
    if (!(options = VMWARECopyOptions()))
        return FALSE;
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, options);

    /*
     * Init xinerama preferences.
     */
    useXinerama = xf86ReturnOptValBool(options, OPTION_XINERAMA,
                                       pVMWARE->vmwareCapability & SVGA_CAP_MULTIMON);
    if (useXinerama && !(pVMWARE->vmwareCapability & SVGA_CAP_MULTIMON)) {
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                  "Xinerama is not safely supported by the current virtual hardware. "
                  "Do not request resolutions that require > 16MB of framebuffer.\n");
    }


    if (useXinerama && xf86IsOptionSet(options, OPTION_GUI_LAYOUT)) {
       char *topology = xf86GetOptValString(options, OPTION_GUI_LAYOUT);
       if (topology) {
          pVMWARE->xineramaState =
             VMWAREParseTopologyString(pScrn, topology,
				       &pVMWARE->xineramaNumOutputs, "gui");

         pVMWARE->xineramaStatic = pVMWARE->xineramaState != NULL;

         free(topology);
       }
    } else if (useXinerama &&
	       xf86IsOptionSet(options, OPTION_STATIC_XINERAMA)) {
       char *topology = xf86GetOptValString(options, OPTION_STATIC_XINERAMA);
       if (topology) {
          pVMWARE->xineramaState =
             VMWAREParseTopologyString(pScrn, topology,
				       &pVMWARE->xineramaNumOutputs,
				       "static Xinerama");

         pVMWARE->xineramaStatic = pVMWARE->xineramaState != NULL;

         free(topology);
       }
    }

    free(options);

    /* Initialise VMWARE_CTRL extension. */
    VMwareCtrl_ExtInit(pScrn);

    /* Initialise Xinerama extension. */
    if (useXinerama) {
       VMwareXinerama_ExtInit(pScrn);
    }

    if (pVMWARE->xinerama && pVMWARE->xineramaStatic) {
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, pVMWARE->xineramaState ?
                                            "Using static Xinerama.\n" :
                                            "Failed to configure static Xinerama.\n");
    }

    /*
     * If using the vgahw module, its data structures and related
     * things are typically initialised/mapped here.
     */
    hwp = VGAHWPTR(pScrn);
    vgaHWGetIOBase(hwp);

    VMWAREInitFIFO(pScrn);

    /* Initialise the first mode */
    VMWAREModeInit(pScrn, pScrn->currentMode, FALSE);

    /* Set the viewport if supported */
    VMWAREAdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));

    /*
     * Setup the screen's visuals, and initialise the framebuffer
     * code.
     */
    VMWAREMapMem(pScrn);

    /*
     * Clear the framebuffer (and any black-border mode areas).
     */
    memset(pVMWARE->FbBase, 0, pVMWARE->FbSize);
    vmwareSendSVGACmdUpdateFullScreen(pVMWARE);

    /* Reset the visual list */
    miClearVisualTypes();

    /*
     * Setup the visuals supported.  This driver only supports
     * TrueColor for bpp > 8, so the default set of visuals isn't
     * acceptable.  To deal with this, call miSetVisualTypes with
     * the appropriate visual mask.
     */
    if (pScrn->bitsPerPixel > 8) {
        if (!miSetVisualTypes(pScrn->depth, TrueColorMask,
                              pScrn->rgbBits, pScrn->defaultVisual)) {
            return FALSE;
        }
    } else {
        if (!miSetVisualTypes(pScrn->depth,
                              miGetDefaultVisualMask(pScrn->depth),
                              pScrn->rgbBits, pScrn->defaultVisual)) {
            return FALSE;
        }
    }

    miSetPixmapDepths();

    /*
     * Initialise the framebuffer.
     */
    if (!fbScreenInit(pScreen, pVMWARE->FbBase + pVMWARE->fbOffset,
                      pScrn->virtualX, pScrn->virtualY,
                      pScrn->xDpi, pScrn->yDpi,
                      pScrn->displayWidth,
                      pScrn->bitsPerPixel)) {
        return FALSE;
    }

    /* Override the default mask/offset settings */
    if (pScrn->bitsPerPixel > 8) {
        int i;
        VisualPtr visual;

        for (i = 0, visual = pScreen->visuals;
             i < pScreen->numVisuals; i++, visual++) {
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

    /* must be after RGB ordering fixed */
    fbPictureInit (pScreen, 0, 0);

    /*
     * Save the old screen vector.
     */
    pVMWARE->ScrnFuncs = *pScreen;

    /*
     * Set initial black & white colourmap indices.
     */
    xf86SetBlackWhitePixels(pScreen);

    /*
     * Initialize shadowfb to notify us of dirty rectangles.  We only
     * need preFB access callbacks if we're using the hw cursor.
     */
    if (!ShadowFBInit2(pScreen, 
                       pVMWARE->hwCursor ? VMWAREPreDirtyBBUpdate : NULL,
                       VMWAREPostDirtyBBUpdate)) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "ShadowFB initialization failed\n");
        return FALSE;
    }

    /*
     * If we have a hw cursor, we need to hook functions that might
     * read from the framebuffer.
     */
    if (pVMWARE->hwCursor) {
        vmwareCursorHookWrappers(pScreen);
    }

    /*
     * If backing store is to be supported (as is usually the case),
     * initialise it.
     */
    xf86SetBackingStore(pScreen);
    xf86SetSilkenMouse(pScreen);

    /*
     * Initialize software cursor.
     */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    /*
     * Initialize hardware cursor.
     */
    if (pVMWARE->hwCursor) {
        if (!vmwareCursorInit(pScreen)) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Hardware cursor initialization failed\n");
            pVMWARE->hwCursor = FALSE;
        }
    }

    /*
     * Install colourmap functions.  If using the vgahw module,
     * vgaHandleColormaps would usually be called here.
     */
    if (!fbCreateDefColormap(pScreen))
        return FALSE;

    if (!xf86HandleColormaps(pScreen, 256, 8,
                             VMWARELoadPalette, NULL,
                             CMAP_PALETTED_TRUECOLOR |
                             CMAP_RELOAD_ON_MODE_SWITCH)) {
        return FALSE;
    }

    /*
     * We explictly add a set of default modes because the X server will
     * not include modes larger than the initial one.
     */
   {
      unsigned int i;
      unsigned int numModes = sizeof (VMWAREDefaultModes) / sizeof *(VMWAREDefaultModes);
      char name[10];
      for (i = 0; i < numModes; i++) {
         const VMWAREDefaultMode *mode = &VMWAREDefaultModes[i];

         /* Only modes that fit the hardware maximums should be added. */
         if (mode->width <= pVMWARE->maxWidth && mode->height <= pVMWARE->maxHeight) {
            snprintf(name, 10, "%dx%d", mode->width, mode->height);
            VMWAREAddDisplayMode(pScrn, name, mode->width, mode->height);
         }
      }

      /* Add the hardware maximums as a mode. */
      snprintf(name, 10, "%dx%d", pVMWARE->maxWidth, pVMWARE->maxHeight);
      VMWAREAddDisplayMode(pScrn, name, pVMWARE->maxWidth, pVMWARE->maxHeight);
   }

    /*
     * We will lazily add the dynamic modes as the are needed when new
     * modes are requested through the control extension.
     */
    memset(&pVMWARE->dynModes, 0, sizeof pVMWARE->dynModes);

#if VMWARE_DRIVER_FUNC
    pScrn->DriverFunc = VMWareDriverFunc;
#endif

    /* Report any unused options (only for the first generation) */
    if (serverGeneration == 1) {
        xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);
    }

    /* Initialize Xv extension */
    pVMWARE->videoStreams = NULL;
    if (vmwareVideoEnabled(pVMWARE)) {
        if (!vmwareVideoInit(pScreen)) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Xv initialization failed\n");
        }
    }

    /**
     * Wrap CloseScreen and SaveScreen. Do this late since we
     * want to be first in the callchain, to avoid using resources
     * already taken down in CloseScreen.
     */

    pVMWARE->ScrnFuncs.CloseScreen = pScreen->CloseScreen;
    pVMWARE->ScrnFuncs.SaveScreen = pScreen->SaveScreen;

    pScreen->CloseScreen = VMWARECloseScreen;
    pScreen->SaveScreen = VMWARESaveScreen;

    /* Done */
    return TRUE;
}

static Bool
VMWARESwitchMode(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    ScreenPtr pScreen = pScrn->pScreen;

    pScreen->mmWidth = (pScreen->width * VMWARE_INCHTOMM +
			pScrn->xDpi / 2) / pScrn->xDpi;
    pScreen->mmHeight = (pScreen->height * VMWARE_INCHTOMM +
			 pScrn->yDpi / 2) / pScrn->yDpi;

    return VMWAREModeInit(pScrn, mode, TRUE);
}

static Bool
VMWAREEnterVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);

    /*
     * After system resumes from hiberation, EnterVT will be called and this
     * is a good place to restore the SVGA ID register.
     */
    vmwareWriteReg(pVMWARE, SVGA_REG_ID, pVMWARE->suspensionSavedRegId);

    if (!pVMWARE->SavedReg.svga_fifo_enabled) {       
        VMWAREInitFIFO(pScrn);
    }

    return VMWAREModeInit(pScrn, pScrn->currentMode, TRUE);
}

static void
VMWARELeaveVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    VMWAREPtr pVMWARE = VMWAREPTR(pScrn);

    /*
     * Before shutting down system for hibneration, LeaveVT will be called,
     * we save the ID register value here and later restore it in EnterVT.
     */
    pVMWARE->suspensionSavedRegId = vmwareReadReg(pVMWARE, SVGA_REG_ID);

    VMWARERestore(pScrn);
}

static void
VMWAREFreeScreen(FREE_SCREEN_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    /*
     * If the vgahw module is used vgaHWFreeHWRec() would be called
     * here.
     */
   VMWAREFreeRec(pScrn);
}

static ModeStatus
VMWAREValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode, Bool verbose, int flags)
{
    return MODE_OK;
}

void
vmwlegacy_hookup(ScrnInfoPtr pScrn)
{
    pScrn->PreInit = VMWAREPreInit;
    pScrn->ScreenInit = VMWAREScreenInit;
    pScrn->SwitchMode = VMWARESwitchMode;
    pScrn->EnterVT = VMWAREEnterVT;
    pScrn->LeaveVT = VMWARELeaveVT;
    pScrn->FreeScreen = VMWAREFreeScreen;
    pScrn->ValidMode = VMWAREValidMode;
}

#ifdef XFree86LOADER
void
VMWARERefSymLists(void)
{
    LoaderRefSymLists(vgahwSymbols, fbSymbols, ramdacSymbols,
		      shadowfbSymbols, NULL);
}
#endif	/* XFree86LOADER */
