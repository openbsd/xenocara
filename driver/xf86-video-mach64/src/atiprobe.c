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

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "ati.h"
#include "atibus.h"
#include "atichip.h"
#include "atimach64io.h"
#include "atimach64version.h"
#include "atiprobe.h"
#include "atividmem.h"
#include "atiwonderio.h"

#ifndef AVOID_CPIO

/*
 * ATIVGAWonderProbe --
 *
 * This function determines if ATI extended VGA registers can be accessed
 * through the I/O port specified by pATI->CPIO_VGAWonder.  If not, the
 * function resets pATI->CPIO_VGAWonder to zero.
 */
static void
ATIVGAWonderProbe
(
    pciVideoPtr pVideo,
    ATIPtr      pATI
)
{
    CARD8 IOValue1, IOValue2, IOValue3, IOValue4, IOValue5, IOValue6;

            if (!pATI->OptionProbeSparse)
            {
                xf86Msg(X_WARNING,
                    MACH64_NAME ":  Expected VGA Wonder capability at I/O port"
                    " 0x%04lX will not be probed\n"
                    "set option \"probe_sparse\" to force probing.\n",
                    pATI->CPIO_VGAWonder);

                pATI->CPIO_VGAWonder = 0;
                return;
            }

            if (pVideo && !xf86IsPrimaryPci(pVideo) &&
                (pATI->Chip <= ATI_CHIP_88800GXD))
            {
                /* Set up extended VGA register addressing */
                PutReg(GRAX, 0x50U, GetByte(pATI->CPIO_VGAWonder, 0));
                PutReg(GRAX, 0x51U, GetByte(pATI->CPIO_VGAWonder, 1) | 0x80U);
            }
            /*
             * Register 0xBB is used by the BIOS to keep track of various
             * things (monitor type, etc.).  Except for 18800-x's, register
             * 0xBC must be zero and causes the adapter to enter a test mode
             * when written to with a non-zero value.
             */
            IOValue1 = inb(pATI->CPIO_VGAWonder);
            IOValue2 = ATIGetExtReg(IOValue1);
            IOValue3 = ATIGetExtReg(0xBBU);
            ATIPutExtReg(0xBBU, IOValue3 ^ 0xAAU);
            IOValue4 = ATIGetExtReg(0xBBU);
            ATIPutExtReg(0xBBU, IOValue3 ^ 0x55U);
            IOValue5 = ATIGetExtReg(0xBBU);
            ATIPutExtReg(0xBBU, IOValue3);
            IOValue6 = ATIGetExtReg(0xBCU);
            ATIPutExtReg(IOValue1, IOValue2);

            if ((IOValue4 == (IOValue3 ^ 0xAAU)) &&
                (IOValue5 == (IOValue3 ^ 0x55U)) &&
                (IOValue6 == 0))
            {
                xf86MsgVerb(X_INFO, 3,
                    MACH64_NAME ":  VGA Wonder at I/O port 0x%04lX detected.\n",
                    pATI->CPIO_VGAWonder);
            }
            else
            {
                xf86Msg(X_WARNING,
                    MACH64_NAME ":  Expected VGA Wonder capability at I/O port"
                    " 0x%04lX was not detected.\n", pATI->CPIO_VGAWonder);
                pATI->CPIO_VGAWonder = 0;
            }
}

#endif /* AVOID_CPIO */

/*
 * ATIMach64Detect --
 *
 * This function determines if a Mach64 is detectable at a particular base
 * address.
 */
static Bool
ATIMach64Detect
(
    ATIPtr            pATI,
    const CARD16      ChipType,
    const ATIChipType Chip
)
{
    CARD32 IOValue, bus_cntl, gen_test_cntl;
    Bool DetectSuccess = FALSE;

    (void)ATIMapApertures(-1, pATI);    /* Ignore errors */

#ifdef AVOID_CPIO

    if (!pATI->pBlock[0])
    {
        ATIUnmapApertures(-1, pATI);
        return FALSE;
    }

#endif /* AVOID_CPIO */

    /* Make sure any Mach64 is not in some weird state */
    bus_cntl = inr(BUS_CNTL);
    if (Chip < ATI_CHIP_264VTB)
        outr(BUS_CNTL,
             (bus_cntl & ~(BUS_HOST_ERR_INT_EN | BUS_FIFO_ERR_INT_EN)) |
             (BUS_HOST_ERR_INT | BUS_FIFO_ERR_INT));
    else if (Chip < ATI_CHIP_264VT4)
        outr(BUS_CNTL, (bus_cntl & ~BUS_HOST_ERR_INT_EN) | BUS_HOST_ERR_INT);

    gen_test_cntl = inr(GEN_TEST_CNTL);
    IOValue = gen_test_cntl &
        (GEN_OVR_OUTPUT_EN | GEN_OVR_POLARITY | GEN_CUR_EN | GEN_BLOCK_WR_EN);
    outr(GEN_TEST_CNTL, IOValue | GEN_GUI_EN);
    outr(GEN_TEST_CNTL, IOValue);
    outr(GEN_TEST_CNTL, IOValue | GEN_GUI_EN);

    /* See if a Mach64 answers */
    IOValue = inr(SCRATCH_REG0);

    /* Test odd bits */
    outr(SCRATCH_REG0, 0x55555555U);
    if (inr(SCRATCH_REG0) == 0x55555555U)
    {
        /* Test even bits */
        outr(SCRATCH_REG0, 0xAAAAAAAAU);
        if (inr(SCRATCH_REG0) == 0xAAAAAAAAU)
        {
            /*
             * *Something* has a R/W 32-bit register at this address.  Try to
             * make sure it's a Mach64.  The following assumes that ATI will
             * not be producing any more adapters that do not register
             * themselves in PCI configuration space.
             */
            ATIMach64ChipID(pATI, ChipType);
            if ((pATI->Chip != ATI_CHIP_Mach64) ||
                (pATI->CPIODecoding == BLOCK_IO))
                DetectSuccess = TRUE;
        }
    }

    /* Restore clobbered register value */
    outr(SCRATCH_REG0, IOValue);

    /* If no Mach64 was detected, return now */
    if (!DetectSuccess)
    {
        outr(GEN_TEST_CNTL, gen_test_cntl);
        outr(BUS_CNTL, bus_cntl);
        ATIUnmapApertures(-1, pATI);
        return FALSE;
    }

    ATIUnmapApertures(-1, pATI);
    return TRUE;
}

#ifdef AVOID_CPIO

/*
 * ATIMach64Probe --
 *
 * This function looks for a Mach64 at a particular MMIO address and returns an
 * ATIRec if one is found.
 */
static ATIPtr
ATIMach64Probe
(
    ATIPtr            pATI,
    pciVideoPtr       pVideo,
    const ATIChipType Chip
)
{
    CARD16 ChipType = PCI_DEV_DEVICE_ID(pVideo);

        pATI->MMIOInLinear = FALSE;

        /*
         * Probe through auxiliary MMIO aperture if one exists.  Because such
         * apertures can be enabled/disabled only through PCI, this probes no
         * further.
         */
        if ((PCI_REGION_SIZE(pVideo, 2) >= (1 << 12)) &&
            (pATI->Block0Base = PCI_REGION_BASE(pVideo, 2, REGION_MEM)))
        {
            pATI->Block0Base += 0x00000400U;
            if (ATIMach64Detect(pATI, ChipType, Chip))
                return pATI;

            return NULL;
        }

        /*
         * Probe through the primary MMIO aperture that exists at the tail end
         * of the linear aperture.  Test for both 8MB and 4MB linear apertures.
         */
        if ((PCI_REGION_SIZE(pVideo, 0) >= (1 << 22)) &&
            (pATI->Block0Base = PCI_REGION_BASE(pVideo, 0, REGION_MEM)))
        {
            pATI->MMIOInLinear = TRUE;

            pATI->Block0Base += 0x007FFC00U;
            if ((PCI_REGION_SIZE(pVideo, 0) >= (1 << 23)) &&
                ATIMach64Detect(pATI, ChipType, Chip))
                return pATI;

            pATI->Block0Base -= 0x00400000U;
            if (ATIMach64Detect(pATI, ChipType, Chip))
                return pATI;
        }

    return NULL;
}

#else /* AVOID_CPIO */

/*
 * ATIMach64Probe --
 *
 * This function looks for a Mach64 at a particular PIO address and returns an
 * ATIRec if one is found.
 */
static ATIPtr
ATIMach64Probe
(
    ATIPtr            pATI,
    pciVideoPtr       pVideo,
    const ATIChipType Chip
)
{
    CARD32 IOValue;
    CARD16 ChipType = PCI_DEV_DEVICE_ID(pVideo);

        if ((pATI->CPIODecoding == BLOCK_IO) &&
            (PCI_REGION_SIZE(pVideo, 1) < (1 << 8)))
            return NULL;

    if (!ATIMach64Detect(pATI, ChipType, Chip))
    {
        return NULL;
    }

    /*
     * Determine VGA capability.  VGA can always be enabled on integrated
     * controllers.  For the GX/CX, it's a board strap.
     */
    if (pATI->Chip >= ATI_CHIP_264CT)
    {
        pATI->VGAAdapter = TRUE;
    }
    else
    {
        IOValue = inr(CONFIG_STATUS64_0);
        pATI->BusType = GetBits(IOValue, CFG_BUS_TYPE);
        IOValue &= (CFG_VGA_EN | CFG_CHIP_EN);
        if (pATI->Chip == ATI_CHIP_88800CX)
            IOValue |= CFG_VGA_EN;
        if (IOValue == (CFG_VGA_EN | CFG_CHIP_EN))
        {
            pATI->VGAAdapter = TRUE;
            pATI->CPIO_VGAWonder = 0x01CEU;
        }
    }

    return pATI;
}

static void
ATIAssignVGA
(
    pciVideoPtr pVideo,
    ATIPtr      pATI
)
{
    if (pATI->CPIO_VGAWonder)
    {
        ATIVGAWonderProbe(pVideo, pATI);
        if (!pATI->CPIO_VGAWonder)
        {
            /*
             * Some adapters are reputed to append ATI extended VGA registers
             * to the VGA Graphics controller registers.  In particular, 0x01CE
             * cannot, in general, be used in a PCI environment due to routing
             * of I/O through the bus tree.
             */
            pATI->CPIO_VGAWonder = GRAX;
            ATIVGAWonderProbe(pVideo, pATI);
        }
    }
}

/*
 * ATIFindVGA --
 *
 * This function determines if a VGA associated with an ATI PCI adapter is
 * shareable.
 */
static void
ATIFindVGA
(
    pciVideoPtr pVideo,
    ATIPtr      pATI
)
{
        /*
         * An ATI PCI adapter has been detected at this point, and its VGA, if
         * any, is shareable.  Ensure the VGA isn't in sleep mode.
         */
        outb(GENENA, 0x16U);
        outb(GENVS, 0x01U);
        outb(GENENA, 0x0EU);

    ATIAssignVGA(pVideo, pATI);
}

#endif /* AVOID_CPIO */

/*
 * ATIMach64ProbeIO --
 *
 * This function determines the IO method and IO base of the ATI PCI adapter.
 */
Bool
ATIMach64ProbeIO
(
    pciVideoPtr pVideo,
    ATIPtr      pATI
)
{
    /* Next, look for sparse I/O Mach64's */
    if (!PCI_REGION_SIZE(pVideo, 1))
    {

#ifndef AVOID_CPIO

        static const unsigned long Mach64SparseIOBases[] = {
            0x02ECU,
            0x01CCU,
            0x01C8U
        };
        uint32_t PciReg;
        uint32_t j;

#ifndef XSERVER_LIBPCIACCESS
        pciConfigPtr pPCI = pVideo->thisCard;

        if (pPCI == NULL)
            return FALSE;
#endif

        PCI_READ_LONG(pVideo, &PciReg, PCI_REG_USERCONFIG);
        j = PciReg & 0x03U;

        if (j == 0x03U)
        {
            xf86Msg(X_WARNING, MACH64_NAME ": "
                "PCI Mach64 in slot %d:%d:%d cannot be enabled\n"
                "because it has neither a block, nor a sparse, I/O base.\n",
                PCI_DEV_BUS(pVideo), PCI_DEV_DEV(pVideo), PCI_DEV_FUNC(pVideo));

            return FALSE;
        }

        /* Possibly fix block I/O indicator */
        if (PciReg & 0x00000004U)
        {
            PciReg &= ~0x00000004U;
            PCI_WRITE_LONG(pVideo, PciReg, PCI_REG_USERCONFIG);
        }

        /* FIXME:
         * Should not probe at sparse I/O bases which have been registered to
         * other PCI devices. The old ATIProbe() would scan the PCI space and
         * build a list of registered I/O ports. If there was a conflict
         * between a mach64 sparse I/O base and a registered I/0 port, probing
         * that port was not allowed...
         *
         * We just add an option and let the user decide, this will not work
         * with "X -configure" though...
         */
        if (!pATI->OptionProbeSparse)
        {
            xf86Msg(X_WARNING, MACH64_NAME ": "
                "PCI Mach64 in slot %d:%d:%d will not be probed\n"
                "set option \"probe_sparse\" to force sparse I/O probing.\n",
                PCI_DEV_BUS(pVideo), PCI_DEV_DEV(pVideo), PCI_DEV_FUNC(pVideo));

            return FALSE;
        }

        pATI->CPIOBase = Mach64SparseIOBases[j];
        pATI->CPIODecoding = SPARSE_IO;
        pATI->PCIInfo = pVideo;

#else /* AVOID_CPIO */

        /* The adapter's CPIO base is of little concern here */
        pATI->CPIOBase = 0;
        pATI->CPIODecoding = SPARSE_IO;
        pATI->PCIInfo = pVideo;

#endif /* AVOID_CPIO */

    }

    /* Lastly, look for block I/O devices */
    if (PCI_REGION_SIZE(pVideo, 1))
    {
        pATI->CPIOBase = PCI_REGION_BASE(pVideo, 1, REGION_IO);
        pATI->CPIODecoding = BLOCK_IO;
        pATI->PCIInfo = pVideo;
    }

    if (!ATIMach64Probe(pATI, pVideo, pATI->Chip))
    {
        xf86Msg(X_WARNING, MACH64_NAME ": "
            "Mach64 in slot %d:%d:%d could not be detected!\n",
            PCI_DEV_BUS(pVideo), PCI_DEV_DEV(pVideo), PCI_DEV_FUNC(pVideo));

        return FALSE;
    }

    xf86Msg(X_INFO, MACH64_NAME ": "
        "Mach64 in slot %d:%d:%d detected.\n",
        PCI_DEV_BUS(pVideo), PCI_DEV_DEV(pVideo), PCI_DEV_FUNC(pVideo));

#ifndef AVOID_CPIO

    if (pATI->VGAAdapter)
        ATIFindVGA(pVideo, pATI);

#endif /* AVOID_CPIO */

    return TRUE;
}
