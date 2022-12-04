/*
 * Copyright 1997 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that the name of Marc Aurele La France not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Marc Aurele La France makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as-is" without express or implied warranty.
 *
 * MARC AURELE LA FRANCE DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO
 * EVENT SHALL MARC AURELE LA FRANCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ati.h"
#include "atistruct.h"
#include "atividmem.h"

/* Memory types for 68800's and 88800GX's */
const char *ATIMemoryTypeNames_Mach[] =
{
    "DRAM (256Kx4)",
    "VRAM (256Kx4, x8, x16)",
    "VRAM (256Kx16 with short shift register)",
    "DRAM (256Kx16)",
    "Graphics DRAM (256Kx16)",
    "Enhanced VRAM (256Kx4, x8, x16)",
    "Enhanced VRAM (256Kx16 with short shift register)",
    "Unknown video memory type"
};

/* Memory types for 88800CX's */
const char *ATIMemoryTypeNames_88800CX[] =
{
    "DRAM (256Kx4, x8, x16)",
    "EDO DRAM (256Kx4, x8, x16)",
    "Unknown video memory type",
    "DRAM (256Kx16 with asymmetric RAS/CAS)",
    "Unknown video memory type",
    "Unknown video memory type",
    "Unknown video memory type",
    "Unknown video memory type"
};

/* Memory types for 264xT's */
const char *ATIMemoryTypeNames_264xT[] =
{
    "Disabled video memory",
    "DRAM",
    "EDO DRAM",
    "Pseudo-EDO DRAM",
    "SDRAM (1:1)",
    "SGRAM (1:1)",
    "SGRAM (2:1) 32-bit",
    "Unknown video memory type"
};

#ifndef AVOID_CPIO

/*
 * ATIUnmapVGA --
 *
 * Unmap VGA aperture.
 */
static void
ATIUnmapVGA
(
    int    iScreen,
    ATIPtr pATI
)
{
    if (!pATI->pBank)
        return;

#ifndef XSERVER_LIBPCIACCESS
    xf86UnMapVidMem(iScreen, pATI->pBank, 0x00010000U);
#else
    (void) pci_device_unmap_legacy(pATI->PCIInfo, pATI->pBank, 0x00010000U);
#endif

    pATI->pBank = NULL;
}

#endif /* AVOID_CPIO */

/*
 * ATIUnmapLinear --
 *
 * Unmap linear aperture.
 */
static void
ATIUnmapLinear
(
    int    iScreen,
    ATIPtr pATI
)
{
    if (pATI->pMemory)
    {
#ifndef XSERVER_LIBPCIACCESS
        xf86UnMapVidMem(iScreen, pATI->pMemory, pATI->LinearSize);
#else
        pci_device_unmap_range(pATI->PCIInfo, pATI->pMemory, pATI->LinearSize);
#endif

#if X_BYTE_ORDER != X_LITTLE_ENDIAN

        if (pATI->pMemoryLE)
        {
#ifndef XSERVER_LIBPCIACCESS
            xf86UnMapVidMem(iScreen, pATI->pMemoryLE, pATI->LinearSize);
#else
            pci_device_unmap_range(pATI->PCIInfo, pATI->pMemoryLE, pATI->LinearSize);
#endif
        }

#endif /* X_BYTE_ORDER */

    }

    pATI->pMemory = pATI->pMemoryLE = NULL;
}

/*
 * ATIUnmapMMIO --
 *
 * Unmap MMIO registers.
 */
static void
ATIUnmapMMIO
(
    int    iScreen,
    ATIPtr pATI
)
{
    if (pATI->pMMIO)
    {
#ifndef XSERVER_LIBPCIACCESS
        xf86UnMapVidMem(iScreen, pATI->pMMIO, getpagesize());
#else
        unsigned long size;

        size = PCI_REGION_SIZE(pATI->PCIInfo, 2);
        if (!size || size > getpagesize())
                size = getpagesize();
        pci_device_unmap_range(pATI->PCIInfo, pATI->pMMIO, size);
#endif
    }

    pATI->pMMIO = pATI->pBlock[0] = pATI->pBlock[1] = NULL;
}

/*
 * ATIUnmapCursor --
 *
 * Unmap hardware cursor image area.
 */
static void
ATIUnmapCursor
(
    int    iScreen,
    ATIPtr pATI
)
{
    if (pATI->pCursorPage)
    {
#ifndef XSERVER_LIBPCIACCESS
        xf86UnMapVidMem(iScreen, pATI->pCursorPage, getpagesize());
#else
        pci_device_unmap_range(pATI->PCIInfo, pATI->pCursorPage, getpagesize());
#endif
    }

    pATI->pCursorPage = pATI->pCursorImage = NULL;
}

/*
 * ATIMapApertures --
 *
 * This function maps all apertures used by the driver.
 *
 * It is called three times:
 * - to setup MMIO for an MMIO-only driver during Probe
 * - to setup MMIO for an MMIO-only driver during PreInit
 * - to setup MMIO (with Block0Base set) and FB (with LinearBase set)
 */
Bool
ATIMapApertures
(
    int    iScreen,
    ATIPtr pATI
)
{
    pciVideoPtr   pVideo = pATI->PCIInfo;
#ifndef XSERVER_LIBPCIACCESS
    PCITAG        Tag = PCI_CFG_TAG(pVideo);
#else
    pciVideoPtr   Tag = pVideo;
#endif
    unsigned long PageSize = getpagesize();

    if (pATI->Mapped)
        return TRUE;

#ifndef AVOID_CPIO

    /* Map VGA aperture */
    if (pATI->VGAAdapter)
    {
        /*
         * No relocation, resizing, caching or write-combining of this
         * aperture is supported.  Hence, the hard-coded values here...
         */
#ifndef XSERVER_LIBPCIACCESS
        pATI->pBank = xf86MapDomainMemory(iScreen, VIDMEM_MMIO_32BIT,
                                          Tag, 0x000A0000U, 0x00010000U);
#else
        (void) pci_device_map_legacy(Tag, 0x000A0000U, 0x00010000U,
                                     PCI_DEV_MAP_FLAG_WRITABLE,
                                     &pATI->pBank);
#endif

        if (!pATI->pBank)
            return FALSE;

        pATI->Mapped = TRUE;
    }

#endif /* AVOID_CPIO */

    /* Map linear aperture */
    if (pATI->LinearBase)
    {

#ifndef XSERVER_LIBPCIACCESS

            pATI->pMemory = xf86MapPciMem(iScreen, VIDMEM_FRAMEBUFFER,
                Tag, pATI->LinearBase, pATI->LinearSize);

#else /* XSERVER_LIBPCIACCESS */

        int mode = PCI_DEV_MAP_FLAG_WRITABLE | PCI_DEV_MAP_FLAG_WRITE_COMBINE;

        int err = pci_device_map_range(pVideo,
                                       pATI->LinearBase,
                                       pATI->LinearSize,
                                       mode, &pATI->pMemory);

        if (err)
        {
            xf86DrvMsg (iScreen, X_ERROR,
                    "Unable to map linear aperture. %s (%d)\n",
                    strerror (err), err);
        }

#endif /* XSERVER_LIBPCIACCESS */

        if (!pATI->pMemory)
        {

#ifndef AVOID_CPIO

            ATIUnmapVGA(iScreen, pATI);

#endif /* AVOID_CPIO */

            pATI->Mapped = FALSE;
            return FALSE;
        }

        pATI->Mapped = TRUE;

#if X_BYTE_ORDER == X_LITTLE_ENDIAN

        if ((pATI->CursorBase >= pATI->LinearBase) &&
            ((pATI->CursorOffset + 0x00000400UL) <= (CARD32)pATI->LinearSize))
            pATI->pCursorImage = (char *)pATI->pMemory + pATI->CursorOffset;

        pATI->pMemoryLE = pATI->pMemory;

#else /* if X_BYTE_ORDER != X_LITTLE_ENDIAN */

        /*
         * Map the little-endian aperture (used for video, etc.).  Note that
         * caching of this area is _not_ wanted.
         */
        {

#ifndef XSERVER_LIBPCIACCESS

            pATI->pMemoryLE = xf86MapPciMem(iScreen, VIDMEM_MMIO, Tag,
                pATI->LinearBase - 0x00800000U, pATI->LinearSize);


#else /* XSERVER_LIBPCIACCESS */

        int mode = PCI_DEV_MAP_FLAG_WRITABLE;

        int err = pci_device_map_range(pVideo,
                                       pATI->LinearBase - 0x00800000U,
                                       pATI->LinearSize,
                                       mode, &pATI->pMemoryLE);

        if (err)
        {
            xf86DrvMsg (iScreen, X_ERROR,
                    "Unable to map extended linear aperture. %s (%d)\n",
                    strerror (err), err);
        }

#endif /* XSERVER_LIBPCIACCESS */

            if (!pATI->pMemoryLE)
            {
                ATIUnmapLinear(iScreen, pATI);

#ifndef AVOID_CPIO

                ATIUnmapVGA(iScreen, pATI);

#endif /* AVOID_CPIO */

                pATI->Mapped = FALSE;
                return FALSE;
            }
        }

#endif /* X_BYTE_ORDER */

    }

    /* Map MMIO aperture */
    if (pATI->Block0Base)
    {
        unsigned long MMIOBase = pATI->Block0Base & ~(PageSize - 1);

#ifndef XSERVER_LIBPCIACCESS

            pATI->pMMIO = xf86MapPciMem(iScreen, VIDMEM_MMIO,
                Tag, MMIOBase, PageSize);

#else /* XSERVER_LIBPCIACCESS */

        int mode = PCI_DEV_MAP_FLAG_WRITABLE;

        int err;
        int size;

        size = PCI_REGION_SIZE(pVideo, 2);
        if (!size || size > PageSize)
               size = PageSize;

	err = pci_device_map_range(pVideo, MMIOBase,
                                   size, mode, &pATI->pMMIO);

        if (err)
        {
            xf86DrvMsg (iScreen, X_ERROR,
                    "Unable to map mmio aperture. %s (%d)\n",
                    strerror (err), err);
        }

#endif /* XSERVER_LIBPCIACCESS */

        if (!pATI->pMMIO)
        {

#if X_BYTE_ORDER == X_LITTLE_ENDIAN

            ATIUnmapCursor(iScreen, pATI);

#endif /* X_BYTE_ORDER */

            ATIUnmapLinear(iScreen, pATI);

#ifndef AVOID_CPIO

            ATIUnmapVGA(iScreen, pATI);

#endif /* AVOID_CPIO */

            pATI->Mapped = FALSE;
            return FALSE;
        }

        pATI->Mapped = TRUE;

        pATI->pBlock[0] = (char *)pATI->pMMIO +
            (pATI->Block0Base - MMIOBase);

        if (pATI->Block1Base)
            pATI->pBlock[1] = (char *)pATI->pBlock[0] - 0x00000400U;

#if X_BYTE_ORDER == X_LITTLE_ENDIAN

        if (!pATI->pCursorImage)

#endif /* X_BYTE_ORDER */

        {
            if ((pATI->CursorBase >= MMIOBase) &&
                ((pATI->CursorBase + 0x00000400UL) <= (MMIOBase + PageSize)))
                pATI->pCursorImage = (char *)pATI->pMMIO +
                    (pATI->CursorBase - MMIOBase);
        }
    }

    /* Map hardware cursor image area */
    if (pATI->CursorBase && !pATI->pCursorImage)
    {
        unsigned long CursorBase = pATI->CursorBase & ~(PageSize - 1);

#ifndef XSERVER_LIBPCIACCESS

            pATI->pCursorPage = xf86MapPciMem(iScreen, VIDMEM_FRAMEBUFFER,
                Tag, CursorBase, PageSize);

#else /* XSERVER_LIBPCIACCESS */

        int mode = PCI_DEV_MAP_FLAG_WRITABLE | PCI_DEV_MAP_FLAG_WRITE_COMBINE;

        int err = pci_device_map_range(pVideo,
                                       CursorBase,
                                       PageSize,
                                       mode, &pATI->pCursorPage);

        if (err)
        {
            xf86DrvMsg (iScreen, X_ERROR,
                    "Unable to map cursor aperture. %s (%d)\n",
                    strerror (err), err);
        }

#endif /* XSERVER_LIBPCIACCESS */

        if (!pATI->pCursorPage)
        {
            ATIUnmapCursor(iScreen, pATI);
            ATIUnmapMMIO(iScreen, pATI);
            ATIUnmapLinear(iScreen, pATI);

#ifndef AVOID_CPIO

            ATIUnmapVGA(iScreen, pATI);

#endif /* AVOID_CPIO */

            pATI->Mapped = FALSE;
            return FALSE;
        }

        pATI->pCursorImage = (char *)pATI->pCursorPage +
            (pATI->CursorBase - CursorBase);
    }

    return TRUE;
}

/*
 * ATIUnmapApertures --
 *
 * This function unmaps all apertures used by the driver.
 */
void
ATIUnmapApertures
(
    int    iScreen,
    ATIPtr pATI
)
{
    if (!pATI->Mapped)
        return;
    pATI->Mapped = FALSE;

    /* Unmap hardware cursor image area */
    ATIUnmapCursor(iScreen, pATI);

    /* Unmap MMIO area */
    ATIUnmapMMIO(iScreen, pATI);

    /* Unmap linear aperture */
    ATIUnmapLinear(iScreen, pATI);

#ifndef AVOID_CPIO

    /* Unmap VGA aperture */
    ATIUnmapVGA(iScreen, pATI);

#endif /* AVOID_CPIO */

}
