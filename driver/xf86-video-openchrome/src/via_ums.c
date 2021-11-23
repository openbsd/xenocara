/*
 * Copyright 2011-2016 The OpenChrome Project
 *                     [https://www.freedesktop.org/wiki/Openchrome]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "globals.h"
#include "via_driver.h"

static const
xf86CrtcConfigFuncsRec via_xf86crtc_config_funcs = {
    via_xf86crtc_resize
};

static void
viaMMIOEnable(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaMMIOEnable.\n"));

    switch (pVia->Chipset) {
        case VIA_CX700:
        case VIA_K8M890:
        case VIA_P4M900:
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            ViaSeqMask(hwp, 0x1A, 0x08, 0x08);
            break;
        default:
            if (pVia->IsSecondary)
                ViaSeqMask(hwp, 0x1A, 0x38, 0x38);
            else
                ViaSeqMask(hwp, 0x1A, 0x68, 0x68);
            break;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaMMIOEnable.\n"));
}

static void
viaMMIODisable(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaMMIODisable.\n"));

    switch (pVia->Chipset) {
        case VIA_CX700:
        case VIA_K8M890:
        case VIA_P4M900:
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            ViaSeqMask(hwp, 0x1A, 0x00, 0x08);
            break;
        default:
            ViaSeqMask(hwp, 0x1A, 0x00, 0x60);
            break;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaMMIODisable.\n"));
}

static Bool
viaMapMMIO(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    CARD8 val;
#ifdef XSERVER_LIBPCIACCESS
    int err;
#else
    unsigned char *tmp;
#endif

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaMapMMIO.\n"));

#ifdef XSERVER_LIBPCIACCESS
    pVia->MmioBase = pVia->PciInfo->regions[1].base_addr;
#else
    pVia->MmioBase = pVia->PciInfo->memBase[1];
#endif

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                "Mapping MMIO at address 0x%lx with "
                "size %u KB.\n",
                pVia->MmioBase, VIA_MMIO_REGSIZE / 1024);

#ifdef XSERVER_LIBPCIACCESS
    err = pci_device_map_range(pVia->PciInfo,
                               pVia->MmioBase,
                               VIA_MMIO_REGSIZE, PCI_DEV_MAP_FLAG_WRITABLE,
                               (void **)&pVia->MapBase);

    if (err) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Unable to map MMIO.\n"
                    "Error: %s (%u)\n",
                    strerror(err), err);
        goto fail;
    }
#else
    pVia->MapBase = xf86MapPciMem(pScrn->scrnIndex,
                                    VIDMEM_MMIO, pVia->PciTag,
                                    pVia->MmioBase, VIA_MMIO_REGSIZE);
    if (!pVia->MapBase) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Unable to map MMIO.\n");
        goto fail;
    }
#endif

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
               "Mapping 2D Host BitBLT space at address 0x%lx with "
               "size %u KB.\n",
               pVia->MmioBase + VIA_MMIO_BLTBASE, VIA_MMIO_BLTSIZE / 1024);

#ifdef XSERVER_LIBPCIACCESS
    err = pci_device_map_range(pVia->PciInfo,
                               pVia->MmioBase + VIA_MMIO_BLTBASE,
                               VIA_MMIO_BLTSIZE, PCI_DEV_MAP_FLAG_WRITABLE,
                               (void **)&pVia->BltBase);

    if (err) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Unable to map 2D Host BitBLT space.\n"
                    "Error: %s (%u)\n",
                    strerror(err), err);
        goto fail;
    }
#else
    pVia->BltBase = xf86MapPciMem(pScrn->scrnIndex,
                                    VIDMEM_MMIO, pVia->PciTag,
                                    pVia->MmioBase + VIA_MMIO_BLTBASE,
                                    VIA_MMIO_BLTSIZE);
    if (!pVia->BltBase) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                   "Unable to map 2D Host BitBLT space.\n");
        goto fail;
    }
#endif

    /* MMIO for MPEG engine. */
    pVia->MpegMapBase = pVia->MapBase + 0xc00;

    /* Set up MMIO vgaHW. */
    vgaHWSetMmioFuncs(hwp, pVia->MapBase, 0x8000);

    val = hwp->readEnable(hwp);
    hwp->writeEnable(hwp, val | 0x01);

    val = hwp->readMiscOut(hwp);
    hwp->writeMiscOut(hwp, val | 0x01);

    /* Unlock extended I/O space. */
    ViaSeqMask(hwp, 0x10, 0x01, 0x01);

    viaMMIOEnable(pScrn);

    vgaHWGetIOBase(hwp);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaMapMMIO.\n"));
    return TRUE;

fail:

#ifdef XSERVER_LIBPCIACCESS
    if (pVia->BltBase) {
        pci_device_unmap_range(pVia->PciInfo, (pointer) pVia->BltBase,
                               VIA_MMIO_BLTSIZE);
    }

    if (pVia->MapBase) {
        pci_device_unmap_range(pVia->PciInfo, (pointer) pVia->MapBase,
                               VIA_MMIO_REGSIZE);
    }
#else
    if (pVia->BltBase) {
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pVia->BltBase,
                        VIA_MMIO_BLTSIZE);
    }

    if (pVia->MapBase) {
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pVia->MapBase,
                        VIA_MMIO_REGSIZE);
    }
#endif

    pVia->BltBase = NULL;
    pVia->MapBase = NULL;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaMapMMIO.\n"));
    return FALSE;
}

static void
viaUnmapMMIO(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaUnmapMMIO.\n"));

    viaMMIODisable(pScrn);

#ifdef XSERVER_LIBPCIACCESS
    if (pVia->BltBase) {
        pci_device_unmap_range(pVia->PciInfo, (pointer) pVia->BltBase,
                               VIA_MMIO_BLTSIZE);
    }

    if (pVia->MapBase) {
        pci_device_unmap_range(pVia->PciInfo, (pointer) pVia->MapBase,
                               VIA_MMIO_REGSIZE);
    }
#else
    if (pVia->BltBase) {
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pVia->BltBase,
                        VIA_MMIO_BLTSIZE);
    }

    if (pVia->MapBase) {
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pVia->MapBase,
                        VIA_MMIO_REGSIZE);
    }
#endif

    pVia->BltBase = NULL;
    pVia->MapBase = NULL;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaUnmapMMIO.\n"));
}

static Bool
viaMapFB(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
#ifdef XSERVER_LIBPCIACCESS
    int err;
#else
    unsigned char *tmp;
#endif
    Bool ret = FALSE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    if (!pVia->videoRambytes) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Unable to map the frame buffer!\n");
        goto exit;
    }

#ifdef XSERVER_LIBPCIACCESS
    if (pVia->Chipset == VIA_VX900) {
        pVia->FrameBufferBase = pVia->PciInfo->regions[2].base_addr;
    } else {
        pVia->FrameBufferBase = pVia->PciInfo->regions[0].base_addr;
    }
#else
    if (pVia->Chipset == VIA_VX900) {
        pVia->FrameBufferBase = pVia->PciInfo->memBase[2];
    } else {
        pVia->FrameBufferBase = pVia->PciInfo->memBase[0];
    }
#endif

#ifdef XSERVER_LIBPCIACCESS
    err = pci_device_map_range(pVia->PciInfo, pVia->FrameBufferBase,
                                pVia->videoRambytes,
                                PCI_DEV_MAP_FLAG_WRITABLE |
                                PCI_DEV_MAP_FLAG_WRITE_COMBINE,
                                (void **)&pVia->FBBase);
    if (err) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Unable to map the frame buffer!\n"
                    "Error: %s (%u)\n",
                    strerror(err), err);
        goto exit;
    }
#else
    /*
     * FIXME: This is a hack to get rid of offending wrongly sized
     * MTRR regions set up by the VIA BIOS. Should be taken care of
     * in the OS support layer.
     */
    tmp = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO, pVia->PciTag,
                        pVia->FrameBufferBase, pVia->videoRambytes);
    if (!tmp) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Unable to map the frame buffer!\n");
        goto exit;
    }

    xf86UnMapVidMem(pScrn->scrnIndex,
                    (pointer) tmp, pVia->videoRambytes);

    /*
     * And, as if this wasn't enough, 2.6 series kernels don't
     * remove MTRR regions on the first attempt. So try again.
     */
    tmp = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_MMIO, pVia->PciTag,
                        pVia->FrameBufferBase, pVia->videoRambytes);
    if (!tmp) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Unable to map the frame buffer!\n");
        goto exit;
    }

    xf86UnMapVidMem(pScrn->scrnIndex,
                    (pointer) tmp, pVia->videoRambytes);
    /*
     * End of hack.
     */

    pVia->FBBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_FRAMEBUFFER,
                                 pVia->PciTag, pVia->FrameBufferBase,
                                 pVia->videoRambytes);
    if (!pVia->FBBase) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Unable to map the frame buffer!\n");
        goto exit;
    }
#endif

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                "Mapping the frame buffer at address 0x%lx with "
                "size %lu KB.\n",
                pVia->FrameBufferBase, pVia->videoRambytes / 1024);

    pVia->FBFreeStart = 0;
    pVia->FBFreeEnd = pVia->videoRambytes;

    ret = TRUE;
exit:
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return ret;
}

static void
viaUnmapFB(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

#ifdef XSERVER_LIBPCIACCESS
    if (pVia->FBBase) {
        pci_device_unmap_range(pVia->PciInfo, (pointer) pVia->FBBase,
                               pVia->videoRambytes);
    }
#else
    if (pVia->FBBase) {
        xf86UnMapVidMem(pScrn->scrnIndex, (pointer) pVia->FBBase,
                        pVia->videoRambytes);
    }
#endif

    pVia->FBBase = NULL;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

/*
 * Leftover from VIA's code.
 */
static void
viaInitPCIe(VIAPtr pVia)
{
    VIASETREG(0x41c, 0x00100000);
    VIASETREG(0x420, 0x680A0000);
    VIASETREG(0x420, 0x02000000);
}

static void
viaInitAGP(VIAPtr pVia)
{
    VIASETREG(VIA_REG_TRANSET, 0x00100000);
    VIASETREG(VIA_REG_TRANSPACE, 0x00000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x00333004);
    VIASETREG(VIA_REG_TRANSPACE, 0x60000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x61000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x62000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x63000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x64000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x7D000000);

    VIASETREG(VIA_REG_TRANSET, 0xfe020000);
    VIASETREG(VIA_REG_TRANSPACE, 0x00000000);
}

/*
 * Initialize the virtual command queue. Header-2 commands can be put
 * in this queue for buffering. AFAIK it doesn't handle Header-1
 * commands, which is really a pity, since it has to be idled before
 * issuing a Header-1 command.
 */
static void
viaEnableAGPVQ(VIAPtr pVia)
{
   CARD32
       vqStartAddr = pVia->VQStart,
       vqEndAddr = pVia->VQEnd,
       vqStartL = 0x50000000 | (vqStartAddr & 0xFFFFFF),
       vqEndL = 0x51000000 | (vqEndAddr & 0xFFFFFF),
       vqStartEndH = 0x52000000 | ((vqStartAddr & 0xFF000000) >> 24) |
       ((vqEndAddr & 0xFF000000) >> 16),
       vqLen = 0x53000000 | (VIA_VQ_SIZE >> 3);

    VIASETREG(VIA_REG_TRANSET, 0x00fe0000);
    VIASETREG(VIA_REG_TRANSPACE, 0x080003fe);
    VIASETREG(VIA_REG_TRANSPACE, 0x0a00027c);
    VIASETREG(VIA_REG_TRANSPACE, 0x0b000260);
    VIASETREG(VIA_REG_TRANSPACE, 0x0c000274);
    VIASETREG(VIA_REG_TRANSPACE, 0x0d000264);
    VIASETREG(VIA_REG_TRANSPACE, 0x0e000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x0f000020);
    VIASETREG(VIA_REG_TRANSPACE, 0x1000027e);
    VIASETREG(VIA_REG_TRANSPACE, 0x110002fe);
    VIASETREG(VIA_REG_TRANSPACE, 0x200f0060);
    VIASETREG(VIA_REG_TRANSPACE, 0x00000006);
    VIASETREG(VIA_REG_TRANSPACE, 0x40008c0f);
    VIASETREG(VIA_REG_TRANSPACE, 0x44000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x45080c04);
    VIASETREG(VIA_REG_TRANSPACE, 0x46800408);

    VIASETREG(VIA_REG_TRANSPACE, vqStartEndH);
    VIASETREG(VIA_REG_TRANSPACE, vqStartL);
    VIASETREG(VIA_REG_TRANSPACE, vqEndL);
    VIASETREG(VIA_REG_TRANSPACE, vqLen);
}

static void
viaEnablePCIeVQ(VIAPtr pVia)
{
   CARD32
       vqStartAddr = pVia->VQStart,
       vqEndAddr = pVia->VQEnd,
       vqStartL = 0x70000000 | (vqStartAddr & 0xFFFFFF),
       vqEndL = 0x71000000 | (vqEndAddr & 0xFFFFFF),
       vqStartEndH = 0x72000000 | ((vqStartAddr & 0xFF000000) >> 24) |
       ((vqEndAddr & 0xFF000000) >> 16),
       vqLen = 0x73000000 | (VIA_VQ_SIZE >> 3);

    VIASETREG(0x41c, 0x00100000);
    VIASETREG(0x420, vqStartEndH);
    VIASETREG(0x420, vqStartL);
    VIASETREG(0x420, vqEndL);
    VIASETREG(0x420, vqLen);
    VIASETREG(0x420, 0x74301001);
    VIASETREG(0x420, 0x00000000);
}

/*
 * Disable the virtual command queue.
 */
void
viaDisableVQ(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaDisableVQ.\n"));

    switch (pVia->Chipset) {
    case VIA_K8M890:
    case VIA_P4M900:
    case VIA_VX800:
    case VIA_VX855:
    case VIA_VX900:
        VIASETREG(0x41c, 0x00100000);
        VIASETREG(0x420, 0x74301000);
        break;
    default:
        VIASETREG(VIA_REG_TRANSET, 0x00fe0000);
        VIASETREG(VIA_REG_TRANSPACE, 0x00000004);
        VIASETREG(VIA_REG_TRANSPACE, 0x40008c0f);
        VIASETREG(VIA_REG_TRANSPACE, 0x44000000);
        VIASETREG(VIA_REG_TRANSPACE, 0x45080c04);
        VIASETREG(VIA_REG_TRANSPACE, 0x46800408);
        break;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaDisableVQ.\n"));
}

/*
 * Initialize the 2D engine and set the 2D context mode to the
 * current screen depth. Also enable the virtual queue.
 */
static void
viaInitialize2DEngine(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    ViaTwodContext *tdc = &pVia->td;
    int i;

    /* Initialize the 2D engine registers to reset the 2D engine. */
    for (i = 0x04; i <= 0x40; i += 4) {
        VIASETREG(i, 0x0);
    }

    if (pVia->Chipset == VIA_VX800 ||
        pVia->Chipset == VIA_VX855 ||
        pVia->Chipset == VIA_VX900) {
        for (i = 0x44; i <= 0x5c; i += 4) {
            VIASETREG(i, 0x0);
        }
    }

    if (pVia->Chipset == VIA_VX900)
    {
        /*410 redefine 0x30 34 38*/
        VIASETREG(0x60, 0x0); /*already useable here*/
    }

    switch (pVia->Chipset) {
        case VIA_K8M890:
        case VIA_P4M900:
        case VIA_VX800:
        case VIA_VX855:
        case VIA_VX900:
            viaInitPCIe(pVia);
            break;
        default:
            viaInitAGP(pVia);
            break;
    }

    if (pVia->VQStart != 0) {
        switch (pVia->Chipset) {
            case VIA_K8M890:
            case VIA_P4M900:
            case VIA_VX800:
            case VIA_VX855:
            case VIA_VX900:
                viaEnablePCIeVQ(pVia);
                break;
            default:
                viaEnableAGPVQ(pVia);
                break;
        }
    } else {
        viaDisableVQ(pScrn);
    }

    viaAccelSetMode(pScrn->bitsPerPixel, tdc);
}

static void
viaInitialize3DEngine(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    int i;

    VIASETREG(VIA_REG_TRANSET, 0x00010000);
    for (i = 0; i <= 0x7D; i++)
        VIASETREG(VIA_REG_TRANSPACE, (CARD32) i << 24);

    VIASETREG(VIA_REG_TRANSET, 0x00020000);
    for (i = 0; i <= 0x94; i++)
        VIASETREG(VIA_REG_TRANSPACE, (CARD32) i << 24);
    VIASETREG(VIA_REG_TRANSPACE, 0x82400000);

    VIASETREG(VIA_REG_TRANSET, 0x01020000);
    for (i = 0; i <= 0x94; i++)
        VIASETREG(VIA_REG_TRANSPACE, (CARD32) i << 24);
    VIASETREG(VIA_REG_TRANSPACE, 0x82400000);

    VIASETREG(VIA_REG_TRANSET, 0xfe020000);
    for (i = 0; i <= 0x03; i++)
        VIASETREG(VIA_REG_TRANSPACE, (CARD32) i << 24);

    VIASETREG(VIA_REG_TRANSET, 0x00030000);
    for (i = 0; i <= 0xff; i++)
        VIASETREG(VIA_REG_TRANSPACE, 0);

    VIASETREG(VIA_REG_TRANSET, 0x00100000);
    VIASETREG(VIA_REG_TRANSPACE, 0x00333004);
    VIASETREG(VIA_REG_TRANSPACE, 0x10000002);
    VIASETREG(VIA_REG_TRANSPACE, 0x60000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x61000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x62000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x63000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x64000000);

    VIASETREG(VIA_REG_TRANSET, 0x00fe0000);
    if (pVia->Chipset == VIA_CLE266 && pVia->ChipRev >= 3)
        VIASETREG(VIA_REG_TRANSPACE, 0x40008c0f);
    else
        VIASETREG(VIA_REG_TRANSPACE, 0x4000800f);
    VIASETREG(VIA_REG_TRANSPACE, 0x44000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x45080C04);
    VIASETREG(VIA_REG_TRANSPACE, 0x46800408);
    VIASETREG(VIA_REG_TRANSPACE, 0x50000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x51000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x52000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x53000000);

    VIASETREG(VIA_REG_TRANSET, 0x00fe0000);
    VIASETREG(VIA_REG_TRANSPACE, 0x08000001);
    VIASETREG(VIA_REG_TRANSPACE, 0x0A000183);
    VIASETREG(VIA_REG_TRANSPACE, 0x0B00019F);
    VIASETREG(VIA_REG_TRANSPACE, 0x0C00018B);
    VIASETREG(VIA_REG_TRANSPACE, 0x0D00019B);
    VIASETREG(VIA_REG_TRANSPACE, 0x0E000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x0F000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x10000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x11000000);
    VIASETREG(VIA_REG_TRANSPACE, 0x20000000);
}

/*
 * Acceleration initialization function. Sets up offscreen memory disposition,
 * and initializes engines and acceleration method.
 */
Bool
viaUMSAccelInit(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    Bool ret = FALSE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    pVia->VQStart = 0;
    pVia->vq_bo = drm_bo_alloc(pScrn, VIA_VQ_SIZE, 16, TTM_PL_VRAM);
    if (!pVia->vq_bo)
        goto err;

    pVia->VQStart = pVia->vq_bo->offset;
    pVia->VQEnd = pVia->vq_bo->offset + pVia->vq_bo->size;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Initializing the 2D engine.\n"));
    viaInitialize2DEngine(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Initializing the 3D engine.\n"));
    viaInitialize3DEngine(pScrn);

    /* Sync marker space. */
    pVia->exa_sync_bo = drm_bo_alloc(pScrn, 32, 32, TTM_PL_VRAM);
    if (!pVia->exa_sync_bo)
        goto err;

    pVia->markerOffset = pVia->exa_sync_bo->offset;
    pVia->markerBuf = drm_bo_map(pScrn, pVia->exa_sync_bo);
    if (!pVia->markerBuf)
        goto err;
    pVia->curMarker = 0;
    pVia->lastMarkerRead = 0;

#ifdef OPENCHROMEDRI
    pVia->dBounce = NULL;
    pVia->scratchAddr = NULL;
#endif /* OPENCHROMEDRI */
    ret = TRUE;
err:
    if (!ret) {
        if (pVia->markerBuf) {
            drm_bo_unmap(pScrn, pVia->exa_sync_bo);
            pVia->markerBuf = NULL;
        }
        if (pVia->exa_sync_bo)
            drm_bo_free(pScrn, pVia->exa_sync_bo);
        if (pVia->vq_bo)
            drm_bo_free(pScrn, pVia->vq_bo);
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return ret;
}

static Bool
viaInitFB(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    BoxRec AvailFBArea;
    int offset, size;
    int maxY;
    Bool ret = TRUE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    maxY = pVia->FBFreeEnd / pVia->Bpl;

    /*
     * FBManager can't handle more than 32767 scan lines.
     */
    if (maxY > 32767)
        maxY = 32767;

    AvailFBArea.x1 = 0;
    AvailFBArea.y1 = 0;
    AvailFBArea.x2 = pScrn->displayWidth;
    AvailFBArea.y2 = maxY;
    pVia->FBFreeStart = (AvailFBArea.y2 + 1) * pVia->Bpl;

    /*
     * Initialization of the XFree86 framebuffer manager is done via
     * Bool xf86InitFBManager(ScreenPtr pScreen, BoxPtr FullBox).
     * FullBox represents the area of the frame buffer that the
     * manager is allowed to manage.  This is typically a box with a
     * width of pScrn->displayWidth and a height of as many lines as
     * can be fit within the total video memory.
     */
    ret = xf86InitFBManager(pScrn->pScreen, &AvailFBArea);
    if (!ret) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "xf86InitFBManager initialization failed.\n");
        goto exit;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Frame buffer from (%d,%d) to (%d,%d).\n",
                        AvailFBArea.x1, AvailFBArea.y1,
                        AvailFBArea.x2, AvailFBArea.y2));

    offset = (pVia->FBFreeStart +
                ((pScrn->bitsPerPixel >> 3) - 1)) /
                (pScrn->bitsPerPixel >> 3);
    size = (pVia->FBFreeEnd / (pScrn->bitsPerPixel >> 3)) - offset;

    if (size > 0) {
        ret = xf86InitFBManagerLinear(pScrn->pScreen, offset, size);
        if (!ret) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                        "xf86InitFBManagerLinear initialization "
                        "failed.\n");
            goto exit;
        }
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Using %d lines for off screen memory.\n",
                        AvailFBArea.y2 - pScrn->virtualY));

exit:
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return ret;
}

Bool
viaUMSMapIOResources(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    Bool ret = TRUE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    pScrn->fbOffset = 0;
     if (pVia->IsSecondary) {
        pScrn->fbOffset = pScrn->videoRam << 10;
    }

#ifdef XSERVER_LIBPCIACCESS
    if (pVia->Chipset == VIA_VX900) {
        pScrn->memPhysBase = pVia->PciInfo->regions[2].base_addr;
    } else {
        pScrn->memPhysBase = pVia->PciInfo->regions[0].base_addr;
    }
#else
    if (pVia->Chipset == VIA_VX900) {
        pScrn->memPhysBase = pVia->PciInfo->memBase[2];
    } else {
        pScrn->memPhysBase = pVia->PciInfo->memBase[0];
    }
#endif

    /*
     * Map MMIO PCI hardware resources to the memory map.
     */
    if (!viaMapMMIO(pScrn)) {
        ret = FALSE;
        goto exit;
    }

    /*
     * Map FB PCI hardware resource to the memory map.
     */
    if (!viaMapFB(pScrn)) {
        viaUnmapMMIO(pScrn);
        ret = FALSE;
        goto exit;
    }

exit:
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return ret;
}

void
viaUMSDestroy(ScrnInfoPtr pScrn)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    viaUnmapFB(pScrn);
    viaUnmapMMIO(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

Bool
viaUMSScreenInit(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    Bool ret = TRUE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    if (pVia->directRenderingType == DRI_NONE) {
        if (!pVia->useEXA) {
            if (!viaInitFB(pScrn)) {
                ret = FALSE;
            }
        } else {
            if (!viaInitExa(pScrn->pScreen)) {
                ret = FALSE;
            }
        }
#ifdef OPENCHROMEDRI
    } else if (pVia->directRenderingType == DRI_1) {
        if (!VIADRIKernelInit(pScrn)) {
            ret = FALSE;
        }

        if ((!pVia->NoAccel) && (pVia->useEXA)) {
            if (!viaInitExa(pScrn->pScreen)) {
                ret = FALSE;
            }
        }
#endif
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return ret;
}

static Bool
viaProbeVRAM(ScrnInfoPtr pScrn)
{
#ifdef XSERVER_LIBPCIACCESS
    struct pci_device *hostBridge = NULL;
    struct pci_device *dramController = NULL;
#endif
    uint8_t videoRAM;
    int     detectedVideoRAM;
    CARD16 hostBridgeVendorID, hostBridgeDeviceID;
    CARD16 dramControllerVendorID;
    Bool status = TRUE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered viaProbeVRAM.\n"));

#ifdef XSERVER_LIBPCIACCESS
    hostBridge = pci_device_find_by_slot(0, 0, 0, 0);
    hostBridgeVendorID = VENDOR_ID(hostBridge);
#else
    hostBridgeVendorID = pciReadWord(pciTag(0, 0, 0), 0x00);
#endif

    if (hostBridgeVendorID != PCI_VIA_VENDOR_ID) {
        status = FALSE;
        goto exit;
    }

#ifdef XSERVER_LIBPCIACCESS
    hostBridgeDeviceID = DEVICE_ID(hostBridge);
#else
    hostBridgeDeviceID = pciReadWord(pciTag(0, 0, 0), 0x02);
#endif

    if ((hostBridgeDeviceID != PCI_DEVICE_ID_VIA_CLE266_HB) &&
        (hostBridgeDeviceID != PCI_DEVICE_ID_VIA_KM400_HB)) {
#ifdef XSERVER_LIBPCIACCESS
        dramController = pci_device_find_by_slot(0, 0, 0, 3);
        dramControllerVendorID = VENDOR_ID(dramController);
#else
        dramControllerVendorID = pciReadWord(pciTag(0, 0, 3), 0x00);

#endif
        if (dramControllerVendorID != PCI_VIA_VENDOR_ID) {
            status = FALSE;
            goto exit;
        }
    }
    switch (hostBridgeDeviceID) {
    case PCI_DEVICE_ID_VIA_CLE266_HB:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "CLE266 chipset host bridge detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(hostBridge, &videoRAM, 0xE1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 0), 0xE1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 10;
        break;
    case PCI_DEVICE_ID_VIA_KM400_HB:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "KM400 chipset host bridge detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(hostBridge, &videoRAM, 0xE1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 0), 0xE1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 10;
        break;
    case PCI_DEVICE_ID_VIA_P4M800_AGP:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "P4M800 chipset AGP bridge detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(dramController, &videoRAM, 0xA1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 3), 0xA1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 10;
        break;
    case PCI_DEVICE_ID_VIA_P4M800_PRO_AGP:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "P4M800 Pro chipset AGP bridge detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(dramController, &videoRAM, 0xA1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 3), 0xA1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 10;
        break;
    case PCI_DEVICE_ID_VIA_PM800_AGP:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "PM800 chipset AGP bridge detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(dramController, &videoRAM, 0xA1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 3), 0xA1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 10;
        break;
    case PCI_DEVICE_ID_VIA_K8M800_AGP:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "K8M800 chipset AGP bridge detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(dramController, &videoRAM, 0xA1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 3), 0xA1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 10;
        break;
    case PCI_DEVICE_ID_VIA_CX700_AGP:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "CX700 chipset AGP bridge detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(dramController, &videoRAM, 0xA1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 3), 0xA1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 12;
        break;
    case PCI_DEVICE_ID_VIA_P4M890_AGP:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "P4M890 chipset AGP bridge detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(dramController, &videoRAM, 0xA1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 3), 0xA1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 12;
        break;
    case PCI_DEVICE_ID_VIA_K8M890_AGP:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "K8M890 chipset AGP bridge detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(dramController, &videoRAM, 0xA1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 3), 0xA1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 12;
        break;
    case PCI_DEVICE_ID_VIA_P4M900_AGP:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "P4M900 chipset AGP bridge detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(dramController, &videoRAM, 0xA1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 3), 0xA1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 12;
        break;
    case PCI_DEVICE_ID_VIA_VX800_HC:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "VX800 chipset host controller detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(dramController, &videoRAM, 0xA1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 3), 0xA1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 12;
        break;
    case PCI_DEVICE_ID_VIA_VX855_HC:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "VX855 chipset host controller detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(dramController, &videoRAM, 0xA1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 3), 0xA1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 12;
        break;
    case PCI_DEVICE_ID_VIA_VX900_HC:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "VX900 chipset host controller detected.\n");
#ifdef XSERVER_LIBPCIACCESS
        pci_device_cfg_read_u8(dramController, &videoRAM, 0xA1);
#else
        videoRAM = pciReadByte(pciTag(0, 0, 3), 0xA1);
#endif
        detectedVideoRAM = (1 << ((videoRAM & 0x70) >> 4)) << 12;
        break;
    default:
        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                    "Could not detect available video RAM.\n");
        detectedVideoRAM = 0;
        status = FALSE;
        break;
    }

    pScrn->videoRam = detectedVideoRAM;
    if (status) {
        xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                   "Detected Video RAM Size: %d KB\n", pScrn->videoRam);
    }

exit:
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting viaProbeVRAM.\n"));
    return status;
}

/*
 * This is basically a function that lets OpenChrome DDX know that
 * it is dealing with hardware that requires special handling.
 */
static void
viaQuirksInit(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay = pVia->pVIADisplay;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    /*
     * Checking for VIA Technologies NanoBook reference design.
     * Examples include Everex CloudBook and Sylvania g netbook.
     * It is also called FIC CE260 and CE261 by its ODM (Original
     * Design Manufacturer) name.
     * This device has its strapping resistors set to a wrong
     * setting to handle DVI. As a result, we need to make special
     * accommodations to handle DVI properly.
     */
    if ((pVia->Chipset == VIA_CX700) &&
        (SUBVENDOR_ID(pVia->PciInfo) == 0x1509) &&
        (SUBSYS_ID(pVia->PciInfo) == 0x2D30)) {
        pVIADisplay->isVIANanoBook      = TRUE;
    } else {
        pVIADisplay->isVIANanoBook      = FALSE;
    }

    /*
     * Checking for Quanta IL1 netbook. This is necessary
     * due to its flat panel connected to DVP1 (Digital
     * Video Port 1) rather than its LVDS channel.
     */
    if ((pVia->Chipset == VIA_VX800) &&
        (SUBVENDOR_ID(pVia->PciInfo) == 0x152D) &&
        (SUBSYS_ID(pVia->PciInfo) == 0x0771)) {
        pVIADisplay->isQuantaIL1      = TRUE;
    } else {
        pVIADisplay->isQuantaIL1      = FALSE;
    }

    /*
     * Samsung NC20 netbook has its FP connected to LVDS2
     * rather than the more logical LVDS1, hence, a special
     * flag register is needed for properly controlling its
     * FP.
     */
    if ((pVia->Chipset == VIA_VX800) &&
        (SUBVENDOR_ID(pVia->PciInfo) == 0x144d) &&
        (SUBSYS_ID(pVia->PciInfo) == 0xc04e)) {
        pVIADisplay->isSamsungNC20      = TRUE;
    } else {
        pVIADisplay->isSamsungNC20      = FALSE;
    }

    /*
     * OLPC XO-1.5 requires a special code path to handle
     * its unusual FP configuration.
     */
    if ((pVia->Chipset == VIA_VX855) &&
        (SUBVENDOR_ID(pVia->PciInfo) == 0x152D) &&
        (SUBSYS_ID(pVia->PciInfo) == 0x0833)) {
        pVIADisplay->isOLPCXO15      = TRUE;
    } else {
        pVIADisplay->isOLPCXO15      = FALSE;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

static void
viaSaveOriginalRegisters(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
    VIARegPtr Regs = &pVIADisplay->SavedReg;
    uint32_t i;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    vgaHWSave(pScrn, &hwp->SavedReg, VGA_SR_ALL);

    /* Unlock extended registers. */
    hwp->writeSeq(hwp, 0x10, 0x01);

    Regs->SR[0x14] = hwp->readSeq(hwp, 0x14);
    Regs->SR[0x15] = hwp->readSeq(hwp, 0x15);
    Regs->SR[0x16] = hwp->readSeq(hwp, 0x16);
    Regs->SR[0x17] = hwp->readSeq(hwp, 0x17);
    Regs->SR[0x18] = hwp->readSeq(hwp, 0x18);
    Regs->SR[0x19] = hwp->readSeq(hwp, 0x19);
    Regs->SR[0x1A] = hwp->readSeq(hwp, 0x1A);
    Regs->SR[0x1B] = hwp->readSeq(hwp, 0x1B);
    Regs->SR[0x1C] = hwp->readSeq(hwp, 0x1C);
    Regs->SR[0x1D] = hwp->readSeq(hwp, 0x1D);
    Regs->SR[0x1E] = hwp->readSeq(hwp, 0x1E);
    Regs->SR[0x1F] = hwp->readSeq(hwp, 0x1F);
    Regs->SR[0x20] = hwp->readSeq(hwp, 0x20);
    Regs->SR[0x21] = hwp->readSeq(hwp, 0x21);
    Regs->SR[0x22] = hwp->readSeq(hwp, 0x22);
    Regs->SR[0x23] = hwp->readSeq(hwp, 0x23);
    Regs->SR[0x24] = hwp->readSeq(hwp, 0x24);

    Regs->SR[0x27] = hwp->readSeq(hwp, 0x27);
    Regs->SR[0x28] = hwp->readSeq(hwp, 0x28);
    Regs->SR[0x29] = hwp->readSeq(hwp, 0x29);
    Regs->SR[0x2A] = hwp->readSeq(hwp, 0x2A);
    Regs->SR[0x2B] = hwp->readSeq(hwp, 0x2B);

    Regs->SR[0x2C] = hwp->readSeq(hwp, 0x2C);

    Regs->SR[0x2D] = hwp->readSeq(hwp, 0x2D);
    Regs->SR[0x2E] = hwp->readSeq(hwp, 0x2E);
    Regs->SR[0x2F] = hwp->readSeq(hwp, 0x2F);
    Regs->SR[0x30] = hwp->readSeq(hwp, 0x30);

    Regs->SR[0x3D] = hwp->readSeq(hwp, 0x3D);

    Regs->SR[0x44] = hwp->readSeq(hwp, 0x44);
    Regs->SR[0x45] = hwp->readSeq(hwp, 0x45);
    Regs->SR[0x46] = hwp->readSeq(hwp, 0x46);
    Regs->SR[0x47] = hwp->readSeq(hwp, 0x47);
    Regs->SR[0x48] = hwp->readSeq(hwp, 0x48);
    Regs->SR[0x49] = hwp->readSeq(hwp, 0x49);
    Regs->SR[0x4A] = hwp->readSeq(hwp, 0x4A);
    Regs->SR[0x4B] = hwp->readSeq(hwp, 0x4B);

    switch (pVia->Chipset) {
    case VIA_K8M800:
    case VIA_P4M800PRO:
    case VIA_PM800:
    case VIA_CX700:
    case VIA_P4M890:
    case VIA_K8M890:
    case VIA_P4M900:
    case VIA_VX800:
    case VIA_VX855:
    case VIA_VX900:
        Regs->SR[0x4C] = hwp->readSeq(hwp, 0x4C);
        Regs->SR[0x4D] = hwp->readSeq(hwp, 0x4D);
        Regs->SR[0x4E] = hwp->readSeq(hwp, 0x4E);
        Regs->SR[0x4F] = hwp->readSeq(hwp, 0x4F);
        break;
    default:
        break;
    }

    if ((pVia->Chipset == VIA_VX800) ||
        (pVia->Chipset == VIA_VX855) ||
        (pVia->Chipset == VIA_VX900)) {
        Regs->SR[0x14] = hwp->readSeq(hwp, 0x14);
        Regs->SR[0x68] = hwp->readSeq(hwp, 0x68);
        Regs->SR[0x69] = hwp->readSeq(hwp, 0x69);
        Regs->SR[0x6A] = hwp->readSeq(hwp, 0x6A);
        Regs->SR[0x6B] = hwp->readSeq(hwp, 0x6B);
        Regs->SR[0x6C] = hwp->readSeq(hwp, 0x6C);
        Regs->SR[0x6D] = hwp->readSeq(hwp, 0x6D);
        Regs->SR[0x6E] = hwp->readSeq(hwp, 0x6E);
        Regs->SR[0x6F] = hwp->readSeq(hwp, 0x6F);
    }

    Regs->CR[0x0C] = hwp->readCrtc(hwp, 0x0C);
    Regs->CR[0x0D] = hwp->readCrtc(hwp, 0x0D);

    Regs->CR[0x30] = hwp->readCrtc(hwp, 0x30);
    Regs->CR[0x31] = hwp->readCrtc(hwp, 0x31);
    Regs->CR[0x32] = hwp->readCrtc(hwp, 0x32);
    Regs->CR[0x33] = hwp->readCrtc(hwp, 0x33);
    Regs->CR[0x34] = hwp->readCrtc(hwp, 0x34);
    Regs->CR[0x35] = hwp->readCrtc(hwp, 0x35);
    Regs->CR[0x36] = hwp->readCrtc(hwp, 0x36);
    Regs->CR[0x37] = hwp->readCrtc(hwp, 0x37);
    Regs->CR[0x38] = hwp->readCrtc(hwp, 0x38);
    Regs->CR[0x39] = hwp->readCrtc(hwp, 0x39);
    Regs->CR[0x3A] = hwp->readCrtc(hwp, 0x3A);
    Regs->CR[0x3B] = hwp->readCrtc(hwp, 0x3B);
    Regs->CR[0x3C] = hwp->readCrtc(hwp, 0x3C);
    Regs->CR[0x3D] = hwp->readCrtc(hwp, 0x3D);
    Regs->CR[0x3E] = hwp->readCrtc(hwp, 0x3E);
    Regs->CR[0x3F] = hwp->readCrtc(hwp, 0x3F);
    Regs->CR[0x40] = hwp->readCrtc(hwp, 0x40);
    Regs->CR[0x43] = hwp->readCrtc(hwp, 0x43);
    Regs->CR[0x45] = hwp->readCrtc(hwp, 0x45);
    Regs->CR[0x46] = hwp->readCrtc(hwp, 0x46);
    Regs->CR[0x47] = hwp->readCrtc(hwp, 0x47);
    Regs->CR[0x48] = hwp->readCrtc(hwp, 0x48);

    for (i = 0; i < (0xA8 - 0x50 + 1); i++) {
        Regs->CR[i + 0x50] = hwp->readCrtc(hwp, i + 0x50);
    }

    switch (pVia->Chipset) {
    case VIA_CX700:
    case VIA_VX800:
    case VIA_VX855:
    case VIA_VX900:
        for (i = 0; i < (0xEF - 0xA9 + 1); i++) {
            Regs->CR[i + 0xA9] = hwp->readCrtc(hwp, i + 0xA9);
        }

        break;
    default:
        break;
    }

    if ((pVia->Chipset == VIA_VX800) ||
        (pVia->Chipset == VIA_VX855) ||
        (pVia->Chipset == VIA_VX900)) {
        for (i = 0; i < (0xF5 - 0xF0 + 1); i++) {
            Regs->CR[i + 0xF0] = hwp->readCrtc(hwp, i + 0xF0);
        }
    }

    if ((pVia->Chipset == VIA_VX855) ||
        (pVia->Chipset == VIA_VX900)) {
        for (i = 0; i < (0xFC - 0xF6 + 1); i++) {
            Regs->CR[i + 0xF6] = hwp->readCrtc(hwp, i + 0xF6);
        }
    }

    if (pVia->Chipset == VIA_VX900) {
        for (i = 0; i < (0xFF - 0xF7 + 1); i++) {
            Regs->CR[i + 0xF7] = hwp->readCrtc(hwp, i + 0xF7);
        }
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}

/*
 * VIAPreInit code path for UMS (User Mode Setting).
 */
Bool
viaUMSPreInit(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    VIADisplayPtr pVIADisplay = pVia->pVIADisplay;
    drmmode_crtc_private_ptr iga1_rec = NULL, iga2_rec = NULL;
    vgaHWPtr hwp;
#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,7,99,3,0)
    ClockRangePtr clockRanges;
#else
    ClockRangesPtr clockRanges;
#endif
    int max_pitch, max_height;
    xf86CrtcPtr iga1, iga2;
    Bool status = FALSE;

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    /*
     * Initialize special flag registers to handle "quirky"
     * hardware.
     */
    viaQuirksInit(pScrn);

    if (!xf86LoadSubModule(pScrn, "vgahw")) {
        goto exit;
    }

    if (!vgaHWGetHWRec(pScrn)) {
        goto exit;
    }

#if 0
    /* Here we can alter the number of registers saved and restored by the
     * standard vgaHWSave and Restore routines.
     */
    vgaHWSetRegCounts(pScrn, VGA_NUM_CRTC, VGA_NUM_SEQ, VGA_NUM_GFX,
                      VGA_NUM_ATTR);
#endif

    if (!viaProbeVRAM(pScrn)) {
        goto exit;
    }

    /* Split the FB for SAMM. */
    /* FIXME: For now, split the FB into two equal sections.
     * This should be user-adjustable via a config option. */
    if (pVia->IsSecondary) {
        DevUnion *pPriv;
        VIAEntPtr pVIAEnt;
        VIAPtr pVia1;

        pPriv = xf86GetEntityPrivate(pScrn->entityList[0], gVIAEntityIndex);
        pVIAEnt = pPriv->ptr;
        pScrn->videoRam = pScrn->videoRam >> 1;
        pVIAEnt->pPrimaryScrn->videoRam = pScrn->videoRam;
        pVia1 = VIAPTR(pVIAEnt->pPrimaryScrn);
        pVia1->videoRambytes = pScrn->videoRam << 10;
        pVia->FrameBufferBase += (pScrn->videoRam << 10);
    }

    pVia->videoRambytes = pScrn->videoRam << 10;

    /* maybe throw in some more sanity checks here */
#ifndef XSERVER_LIBPCIACCESS
    pVia->PciTag = pciTag(pVia->PciInfo->bus, pVia->PciInfo->device,
                          pVia->PciInfo->func);
#endif

    /* Map PCI hardware resources to the memory map. */
    if (!viaMapMMIO(pScrn)) {
        goto exit;
    }

    hwp = VGAHWPTR(pScrn);

    viaSaveOriginalRegisters(pScrn);

    /* Read memory bandwidth from registers. */
    pVia->MemClk = hwp->readCrtc(hwp, 0x3D) >> 4;
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                     "Detected MemClk %d\n", pVia->MemClk));
    if (pVia->MemClk >= VIA_MEM_END) {
        xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                   "Unknown Memory clock: %d\n", pVia->MemClk);
        pVia->MemClk = VIA_MEM_END - 1;
    }

    pVIADisplay->Bandwidth = ViaGetMemoryBandwidth(pScrn);

    if (pVIADisplay->TVType == TVTYPE_NONE) {
        /* Use jumper to determine TV type. */
        if (hwp->readCrtc(hwp, 0x3B) & 0x02) {
            pVIADisplay->TVType = TVTYPE_PAL;
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                             "Detected TV standard: PAL.\n"));
        } else {
            pVIADisplay->TVType = TVTYPE_NTSC;
            DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                             "Detected TV standard: NTSC.\n"));
        }
    }

    pVIADisplay->I2CDevices = VIA_I2C_BUS1 | VIA_I2C_BUS2 | VIA_I2C_BUS3;

    if (pVIADisplay->isOLPCXO15) {
        pVIADisplay->I2CDevices &= ~VIA_I2C_BUS2;
    }

    if (pVia->drmmode.hwcursor) {
        if (!xf86LoadSubModule(pScrn, "ramdac")) {
            goto free_mmio;
        }
    }

    if (!xf86LoadSubModule(pScrn, "i2c")) {
        goto free_mmio;
    } else {
        ViaI2CInit(pScrn);
    }

    if (!xf86LoadSubModule(pScrn, "ddc")) {
        goto free_mmio;
    }

    /*
     * Set up ClockRanges, which describe what clock ranges are
     * available, and what sort of modes they can be used for.
     */

#if XORG_VERSION_CURRENT >= XORG_VERSION_NUMERIC(1,7,99,3,0)
    clockRanges = xnfalloc(sizeof(ClockRange));
#else
    clockRanges = xnfalloc(sizeof(ClockRanges));
#endif
    clockRanges->next = NULL;
    clockRanges->minClock = 20000;
    clockRanges->maxClock = 230000;

    clockRanges->clockIndex = -1;
    clockRanges->interlaceAllowed = TRUE;
    clockRanges->doubleScanAllowed = FALSE;
    pScrn->clockRanges = clockRanges;

    xf86CrtcConfigInit(pScrn, &via_xf86crtc_config_funcs);

    /*
     * Now handle the outputs
     */
    iga1_rec = (drmmode_crtc_private_ptr) xnfcalloc(sizeof(drmmode_crtc_private_rec), 1);
    if (!iga1_rec) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "IGA1 Rec allocation failed.\n");
        goto free_mmio;
    }

    iga1 = xf86CrtcCreate(pScrn, &via_crtc_funcs);
    if (!iga1) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "xf86CrtcCreate failed.\n");
        free(iga1_rec);
        goto free_mmio;
    }

    iga1_rec->drmmode = &pVia->drmmode;
    iga1_rec->index = 0;
    iga1->driver_private = iga1_rec;

    iga2_rec = (drmmode_crtc_private_ptr) xnfcalloc(sizeof(drmmode_crtc_private_rec), 1);
    if (!iga2_rec) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "IGA1 Rec allocation failed.\n");
        xf86CrtcDestroy(iga1);
        goto free_mmio;
    }

    iga2 = xf86CrtcCreate(pScrn, &via_crtc_funcs);
    if (!iga2) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "xf86CrtcCreate failed.\n");
        xf86CrtcDestroy(iga1);
        free(iga2_rec);
        goto free_mmio;
    }

    iga2_rec->drmmode = &pVia->drmmode;
    iga2_rec->index = 1;
    iga2->driver_private = iga2_rec;

    if (!pScrn->bitsPerPixel) {
        xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Detected bitsPerPixel to be 0 bit.\n");
        xf86CrtcDestroy(iga2);
        xf86CrtcDestroy(iga1);
        goto free_mmio;
    }

    /*
     * CLE266A:
     *   Max Line Pitch: 4080, (FB corruption when higher, driver problem?)
     *   Max Height: 4096 (and beyond)
     *
     * CLE266A: primary AdjustFrame can use only 24 bits, so we are limited
     * to 12x11 bits; 4080x2048 (~2:1), 3344x2508 (4:3), or 2896x2896 (1:1).
     * TODO Test CLE266Cx, KM400, KM400A, K8M800, CN400 please.
     *
     * We should be able to limit the memory available for a mode to 32 MB,
     * but miScanLineWidth fails to catch this properly (apertureSize).
     */
    max_pitch = (8192 / ((pScrn->bitsPerPixel + 7) >> 3)) - (16 / ((pScrn->bitsPerPixel + 7) >> 3));
    max_height = 8192 / ((pScrn->bitsPerPixel + 7) >> 3);

    xf86CrtcSetSizeRange(pScrn, 320, 200, max_pitch, max_height);

    viaInitDisplay(pScrn);

    status = xf86InitialConfiguration(pScrn, TRUE);
    goto exit;
free_mmio:
    viaUnmapMMIO(pScrn);
exit:
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
    return status;
}

void
viaUMSPreInitExit(ScrnInfoPtr pScrn)
{
    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Entered %s.\n", __func__));

    viaUnmapMMIO(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                        "Exiting %s.\n", __func__));
}
