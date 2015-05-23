/*
 * Copyright 1999 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
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

#include "ati.h"
#include "atiaudio.h"
#include "atibus.h"
#include "atichip.h"
#include "aticursor.h"
#include "atidac.h"
#include "atidsp.h"
#include "atii2c.h"
#include "atiload.h"
#include "atilock.h"
#include "atimach64.h"
#include "atimach64accel.h"
#include "atimach64io.h"
#include "atimach64probe.h"
#include "atimode.h"
#include "atioption.h"
#include "atipreinit.h"
#include "atiprint.h"
#include "atiprobe.h"
#include "atividmem.h"
#include "atiwonderio.h"
#include "atixv.h"
#include "atiadjust.h"

#include "vbe.h"
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86RAC.h"
#endif

/*
 * FreeScreen handles the clean-up.
 */
Bool
ATIGetRec(ScrnInfoPtr pScrn)
{
    if (!pScrn->driverPrivate) {
        pScrn->driverPrivate = xnfcalloc(sizeof(ATIRec), 1);
        memset(pScrn->driverPrivate, 0, sizeof(ATIRec));
    }

    return TRUE;
}

/*
 * ATIReportMemory --
 *
 * This function reports on the amount and type of video memory found.
 */
static void
ATIReportMemory
(
    ScrnInfoPtr pScreenInfo,
    ATIPtr      pATI,
    const char *MemoryTypeName
)
{
    char Buffer[128], *Message;

    Message = Buffer +
        snprintf(Buffer, SizeOf(Buffer), "%d kB of %s detected",
            pATI->VideoRAM, MemoryTypeName);

    if (pATI->VideoRAM > pScreenInfo->videoRam)
    {
        Message += snprintf(Message, Buffer + SizeOf(Buffer) - Message,
            " (using %d kB)", pScreenInfo->videoRam);
    }
    xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED, "%s.\n", Buffer);
}

static const int videoRamSizes[] =
    {512, 1024, 2*1024, 4*1024, 6*1024, 8*1024, 12*1024, 16*1024};
static const rgb   defaultWeight = {0, 0, 0};
static const Gamma defaultGamma  = {0.0, 0.0, 0.0};

/*
 * ATIPrintNoiseIfRequested --
 *
 * This function formats debugging information on the server's stderr when
 * requested by the user through the server's verbosity setting.
 */
static void
ATIPrintNoiseIfRequested
(
    ATIPtr       pATI,
    CARD8       *BIOS,
    unsigned int BIOSSize
)
{
    if (xf86GetVerbosity() <= 3)
        return;

    if (BIOSSize > 0)
        ATIPrintBIOS(BIOS, BIOSSize);
    xf86ErrorFVerb(4, "\n On server entry:\n");
    ATIPrintRegisters(pATI);
}

#define BIOS_SIZE    0x00010000U     /* 64kB */
#define BIOSByte(_n) ((CARD8)(BIOS[_n]))
#define BIOSWord(_n) ((CARD16)(BIOS[_n] |                \
                               (BIOS[(_n) + 1] << 8)))

/*
 * For Mach64 adapters, pick up, from the BIOS, the type of programmable
 * clock generator (if any), and various information about it.
 */
static void
ati_bios_clock
(
    ScrnInfoPtr  pScreenInfo,
    ATIPtr       pATI,
    CARD8       *BIOS,
    unsigned int ClockTable,
    GDevPtr      pGDev
)
{
    CARD16 ClockDac;

    if (ClockTable > 0)
    {
        pATI->ProgrammableClock = BIOSByte(ClockTable);
        pATI->ClockNumberToProgramme = BIOSByte(ClockTable + 0x06U);
        pATI->refclk = BIOSWord(ClockTable + 0x08U);
        pATI->refclk *= 10000;
    }
    else
    {
        /*
         * Compensate for BIOS absence.  Note that the reference
         * frequency has already been set by option processing.
         */
        if ((pATI->DAC & ~0x0FU) == ATI_DAC_INTERNAL)
        {
            pATI->ProgrammableClock = ATI_CLOCK_INTERNAL;
        }
        else switch (pATI->DAC)
        {
            case ATI_DAC_STG1703:
                pATI->ProgrammableClock = ATI_CLOCK_STG1703;
                break;

            case ATI_DAC_CH8398:
                pATI->ProgrammableClock = ATI_CLOCK_CH8398;
                break;

            case ATI_DAC_ATT20C408:
                pATI->ProgrammableClock = ATI_CLOCK_ATT20C408;
                break;

            case ATI_DAC_IBMRGB514:
                pATI->ProgrammableClock = ATI_CLOCK_IBMRGB514;
                break;

            default:        /* Provisional */
                pATI->ProgrammableClock = ATI_CLOCK_ICS2595;
                break;
        }

        /* This should be safe for all generators except IBM's RGB514 */
        pATI->ClockNumberToProgramme = 3;
    }

    pATI->ClockDescriptor = ATIClockDescriptors[ATI_CLOCK_FIXED];

    if ((pATI->ProgrammableClock > ATI_CLOCK_FIXED) &&
        (pATI->ProgrammableClock < ATI_CLOCK_MAX))
    {
        /*
         * Graphics PRO TURBO 1600's are unusual in that an ICS2595 is used
         * to generate clocks for VGA modes, and an IBM RGB514 is used for
         * accelerator modes.
         */
        if ((pATI->ProgrammableClock == ATI_CLOCK_ICS2595) &&
            (pATI->DAC == ATI_DAC_IBMRGB514))
            pATI->ProgrammableClock = ATI_CLOCK_IBMRGB514;

        pATI->ClockDescriptor = ATIClockDescriptors[pATI->ProgrammableClock];
    }

    ClockDac = pATI->DAC;
    switch (pATI->ProgrammableClock)
    {
        case ATI_CLOCK_ICS2595:
            /*
             * Pick up reference divider (43 or 46) appropriate to the chip
             * revision level.
             */
            if (ClockTable > 0)
                pATI->ClockDescriptor.MinM =
                pATI->ClockDescriptor.MaxM = BIOSWord(ClockTable + 0x0AU);
            else if (!xf86NameCmp(pGDev->clockchip, "ATI 18818-0"))
                pATI->ClockDescriptor.MinM =
                pATI->ClockDescriptor.MaxM = 43;
            else if (!xf86NameCmp(pGDev->clockchip, "ATI 18818-1"))
                pATI->ClockDescriptor.MinM =
                pATI->ClockDescriptor.MaxM = 46;
            else
                pATI->ProgrammableClock = ATI_CLOCK_UNKNOWN;
            break;

        case ATI_CLOCK_STG1703:
            /* This one's also a RAMDAC */
            ClockDac = ATI_DAC_STG1703;
            break;

        case ATI_CLOCK_CH8398:
            /* This one's also a RAMDAC */
            ClockDac = ATI_DAC_CH8398;
            break;

        case ATI_CLOCK_INTERNAL:
            /*
             * The reference divider has already been programmed by BIOS
             * initialisation.  Because, there is only one reference
             * divider for all generated frequencies (including MCLK), it
             * cannot be changed without reprogramming all clocks every
             * time one of them needs a different reference divider.
             *
             * Besides, it's not a good idea to change the reference
             * divider.  BIOS initialisation sets it to a value that
             * effectively prevents generating frequencies beyond the
             * graphics controller's tolerance.
             */
            pATI->ClockDescriptor.MinM =
            pATI->ClockDescriptor.MaxM = ATIMach64GetPLLReg(PLL_REF_DIV);

            /* The DAC is also integrated */
            if ((pATI->DAC & ~0x0FU) != ATI_DAC_INTERNAL)
                ClockDac = ATI_DAC_INTERNAL;

            break;

        case ATI_CLOCK_ATT20C408:
            /* This one's also a RAMDAC */
            ClockDac = ATI_DAC_ATT20C408;
            break;

        case ATI_CLOCK_IBMRGB514:
            /* This one's also a RAMDAC */
            ClockDac = ATI_DAC_IBMRGB514;
            pATI->ClockNumberToProgramme = 7;
            break;

        default:
            break;
    }

    /*
     * We now have up to two indications of what RAMDAC the adapter uses.
     * They should be the same.  The following test and corresponding
     * action are under construction.
     */
    if (pATI->DAC != ClockDac)
    {
        xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                   "Mach64 RAMDAC probe discrepancy detected:\n"
                   "  DAC=0x%02X;  ClockDac=0x%02X.\n",
                   pATI->DAC, ClockDac);

        if (pATI->DAC == ATI_DAC_IBMRGB514)
        {
            pATI->ProgrammableClock = ATI_CLOCK_IBMRGB514;
            pATI->ClockDescriptor = ATIClockDescriptors[ATI_CLOCK_IBMRGB514];
            pATI->ClockNumberToProgramme = 7;
        }
        else
        {
            pATI->DAC = ClockDac;   /* For now */
        }
    }

    switch (pATI->refclk / 100000)
    {
        case 143:
            pATI->ReferenceNumerator = 157500;
            pATI->ReferenceDenominator = 11;
            break;

        case 286:
            pATI->ReferenceNumerator = 315000;
            pATI->ReferenceDenominator = 11;
            break;

        default:
            pATI->ReferenceNumerator = pATI->refclk / 1000;
            pATI->ReferenceDenominator = 1;
            break;
    }
}

/*
 * Pick up multimedia information, which will be at different
 * displacements depending on table revision.
 */
static void
ati_bios_mmedia
(
    ScrnInfoPtr  pScreenInfo,
    ATIPtr       pATI,
    CARD8       *BIOS,
    unsigned int VideoTable,
    unsigned int HardwareTable
)
{
    pATI->Audio = ATI_AUDIO_NONE;

    if (VideoTable > 0)
    {
        switch (BIOSByte(VideoTable - 0x02U))
        {
            case 0x00U:
                pATI->Tuner = BIOSByte(VideoTable) & 0x1FU;

                /*
                 * XXX  The VideoTable[1] byte is known to have been
                 *      omitted in LTPro and Mobility BIOS'es.  Any others?
                 */
                switch (pATI->Chip)
                {
                    case ATI_CHIP_264LTPRO:
                    case ATI_CHIP_MOBILITY:
                        pATI->Decoder = BIOSByte(VideoTable + 0x01U) & 0x07U;
                        pATI->Audio = BIOSByte(VideoTable + 0x02U) & 0x0FU;
                        break;

                    default:
                        pATI->Decoder = BIOSByte(VideoTable + 0x02U) & 0x07U;
                        pATI->Audio = BIOSByte(VideoTable + 0x03U) & 0x0FU;
                        break;
                }

                break;

            case 0x01U:
                pATI->Tuner = BIOSByte(VideoTable) & 0x1FU;
                pATI->Audio = BIOSByte(VideoTable + 0x01U) & 0x0FU;
                pATI->Decoder = BIOSByte(VideoTable + 0x05U) & 0x0FU;
                break;

            default:
                break;
        }
    }

    if (HardwareTable > 0)
    {
        pATI->I2CType = BIOSByte(HardwareTable + 0x06U) & 0x0FU;
    }
}

/*
 * Determine panel dimensions and model.
 */
static void
ati_bios_panel_info
(
    ScrnInfoPtr  pScreenInfo,
    ATIPtr       pATI,
    CARD8       *BIOS,
    unsigned int BIOSSize,
    unsigned int LCDTable
)
{
    unsigned int LCDPanelInfo = 0;
    char         Buffer[128];
    int          i, j;

    if (LCDTable > 0)
    {
        LCDPanelInfo = BIOSWord(LCDTable + 0x0AU);
        if (((LCDPanelInfo + 0x1DU) > BIOSSize) ||
            ((BIOSByte(LCDPanelInfo) != pATI->LCDPanelID) &&
             (pATI->LCDPanelID || (BIOSByte(LCDPanelInfo) > 0x1FU) ||
              (pATI->Chip <= ATI_CHIP_264LTPRO))))
            LCDPanelInfo = 0;
    }

    if (!LCDPanelInfo)
    {
        /*
         * Scan BIOS for panel info table.
         */
        for (i = 0;  i <= (int)(BIOSSize - 0x1DU);  i++)
        {
            /* Look for panel ID ... */
            if ((BIOSByte(i) != pATI->LCDPanelID) &&
                (pATI->LCDPanelID || (BIOSByte(i) > 0x1FU) ||
                 (pATI->Chip <= ATI_CHIP_264LTPRO)))
                continue;

            /* ... followed by 24-byte panel model name ... */
            for (j = 0;  j < 24;  j++)
            {
                if ((CARD8)(BIOSByte(i + j + 1) - 0x20U) > 0x5FU)
                {
                    i += j;
                    goto NextBIOSByte;
                }
            }

            /* ... verify panel width ... */
            if (pATI->LCDHorizontal &&
                (pATI->LCDHorizontal != BIOSWord(i + 0x19U)))
                continue;

            /* ... and verify panel height */
            if (pATI->LCDVertical &&
                (pATI->LCDVertical != BIOSWord(i + 0x1BU)))
                continue;

            if (LCDPanelInfo)
            {
                /*
                 * More than one possibility, but don't care if all
                 * tables describe panels of the same size.
                 */
                if ((BIOSByte(LCDPanelInfo + 0x19U) ==
                     BIOSByte(i + 0x19U)) &&
                    (BIOSByte(LCDPanelInfo + 0x1AU) ==
                     BIOSByte(i + 0x1AU)) &&
                    (BIOSByte(LCDPanelInfo + 0x1BU) ==
                     BIOSByte(i + 0x1BU)) &&
                    (BIOSByte(LCDPanelInfo + 0x1CU) ==
                     BIOSByte(i + 0x1CU)))
                    continue;

                LCDPanelInfo = 0;
                break;
            }

            LCDPanelInfo = i;

    NextBIOSByte:  ;
        }
    }

    if (LCDPanelInfo > 0)
    {
        pATI->LCDPanelID = BIOSByte(LCDPanelInfo);
        pATI->LCDHorizontal = BIOSWord(LCDPanelInfo + 0x19U);
        pATI->LCDVertical = BIOSWord(LCDPanelInfo + 0x1BU);
    }

    if (LCDPanelInfo)
    {
        for (i = 0;  i < 24;  i++)
            Buffer[i] = BIOSByte(LCDPanelInfo + 1 + i);
        for (;  --i >= 0;  )
            if (Buffer[i] && Buffer[i] != ' ')
            {
                Buffer[i + 1] = '\0';
                xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
                    "Panel model %s.\n", Buffer);
                break;
            }
    }
}

/*
 * ATIPreInit --
 *
 * This function is only called once per screen at the start of the first
 * server generation.
 */
Bool
ATIPreInit
(
    ScrnInfoPtr pScreenInfo,
    int flags
)
{
    CARD8            BIOS[BIOS_SIZE];
    unsigned int     BIOSSize = 0;
    unsigned int     ROMTable = 0, ClockTable = 0, FrequencyTable = 0;
    unsigned int     LCDTable = 0, VideoTable = 0;
    unsigned int     HardwareTable = 0;

    char             Buffer[128], *Message;
    ATIPtr           pATI;
    GDevPtr          pGDev;
    EntityInfoPtr    pEntity;
#ifndef XSERVER_LIBPCIACCESS
    resPtr           pResources;
#endif
    pciVideoPtr      pVideo;
    DisplayModePtr   pMode;
    CARD32           IOValue;
    int              i, j;
    int              Numerator, Denominator;
    int              MinX, MinY;
    ClockRange       ATIClockRange = {NULL, 0, 80000, -1, TRUE, TRUE, 1, 1, 0};
    int              DefaultmaxClock = 0;
    int              minPitch, maxPitch = 0xFFU, pitchInc, maxHeight = 0;
    int              ApertureSize = 0x00010000U;
    int              ModeType = M_T_BUILTIN;
    LookupModeFlags  Strategy = LOOKUP_CLOSEST_CLOCK;
    int              DefaultDepth;
    Bool             PreInitSuccess = FALSE;

#   define           pATIHW     (&pATI->OldHW)

#ifndef AVOID_CPIO

    vbeInfoPtr       pVBE = NULL;
    pointer          pVBEModule = NULL;

#endif /* AVOID_CPIO */

    if (pScreenInfo->numEntities != 1)
    {
        xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
            "Logic error:  Number of attached entities not 1.\n");
        return FALSE;
    }

    if (!ATIGetRec(pScreenInfo))
        return FALSE;

    pATI = ATIPTR(pScreenInfo);

    /* Register resources */
    pEntity = xf86GetEntityInfo(pScreenInfo->entityList[0]);
    pGDev = pEntity->device;
#ifndef XSERVER_LIBPCIACCESS
    pResources = pEntity->resources;
#endif

    pATI->iEntity = pEntity->index;
#ifndef XSERVER_LIBPCIACCESS
    pATI->Chip = pEntity->chipset;
#endif
    pVideo = xf86GetPciInfoForEntity(pATI->iEntity);

    free(pEntity);

#ifndef XSERVER_LIBPCIACCESS
    if (!pResources)
        pResources = xf86RegisterResources(pATI->iEntity, NULL, ResShared);
    if (pResources)
    {
        xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
            "Unable to register bus resources\n");
        xf86FreeResList(pResources);
        return FALSE;
    }
#endif
    ConfiguredMonitor = NULL;
    (void)memset(BIOS, 0, SizeOf(BIOS));

    if (!(flags & PROBE_DETECT))
    {
        /* Set monitor */
        pScreenInfo->monitor = pScreenInfo->confScreen->monitor;

        /* Set depth, bpp, etc. */
        if ((pATI->Chip < ATI_CHIP_264CT))
        {
            i = NoDepth24Support;       /* No support for >8bpp either */
            DefaultDepth = 8;
        }
        else
        {
            i = Support24bppFb | Support32bppFb;
            DefaultDepth = 0;
        }

        if (!xf86SetDepthBpp(pScreenInfo, DefaultDepth, 0, 0, i))
            return FALSE;

        for (j = 0;  ;  j++)
        {
            static const CARD8 AllowedDepthBpp[][2] =
            {
                { 8,  8},
                {15, 16},
                {16, 16},
                {24, 24},
                {24, 32}
            };

            if (j < NumberOf(AllowedDepthBpp))
            {
                if (pScreenInfo->depth > AllowedDepthBpp[j][0])
                    continue;

                if (pScreenInfo->depth == AllowedDepthBpp[j][0])
                {
                    if (pScreenInfo->bitsPerPixel > AllowedDepthBpp[j][1])
                        continue;

                    if (pScreenInfo->bitsPerPixel == AllowedDepthBpp[j][1])
                        break;
                }
            }

            xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
                "Driver does not support depth %d at fbbpp %d.\n",
                pScreenInfo->depth, pScreenInfo->bitsPerPixel);
            return FALSE;
        }

        xf86PrintDepthBpp(pScreenInfo);

        if ((i == NoDepth24Support) && (pScreenInfo->depth > 8))
        {
            xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
                "Depth %d is not supported through this adapter.\n",
                pScreenInfo->depth);
            return FALSE;
        }

        /* Pick up XF86Config options */
        ATIProcessOptions(pScreenInfo, pATI);
    }

    if (!ATIMach64ProbeIO(pVideo, pATI))
        return FALSE;

    ATIClaimBusSlot(pGDev->active, pATI);

#ifndef AVOID_CPIO

#ifdef TV_OUT

    pATI->pVBE = NULL;
    pATI->pInt10 = NULL;

#endif /* TV_OUT */

    /*
     * If VBE setup works, grab DDC from it
     */
    if (!(pVBEModule = xf86LoadSubModule(pScreenInfo, "vbe"))) {
	xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
		   "Unable to load vbe module.\n");
    }
    else
    {
	if ((pVBE = VBEInit(NULL, pATI->iEntity)))
	    ConfiguredMonitor = vbeDoEDID(pVBE, NULL);

        if (pVBE && !(flags & PROBE_DETECT))
        {
	    xf86Int10InfoPtr pInt10Info = pVBE->pInt10;

            /* Validate, then make a private copy of, the initialised BIOS */
            CARD8 *pBIOS = xf86int10Addr(pInt10Info, pInt10Info->BIOSseg << 4);

            if ((pBIOS[0] != 0x55U) || (pBIOS[1] != 0xAAU) || !pBIOS[2])
            {
                xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                    "Unable to correctly retrieve adapter BIOS.\n");
            }
            else
            {
                BIOSSize = pBIOS[2] << 9;
                if (BIOSSize > BIOS_SIZE)
                    BIOSSize = BIOS_SIZE;
                (void)memcpy(BIOS, pBIOS, BIOSSize);
            }
        }
    }

#ifndef TV_OUT
    /* De-activate VBE */
    vbeFree(pVBE);
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 12
    xf86UnloadSubModule(pVBEModule);
#endif
#else
    pATI->pVBE = pVBE;
    pVBE = NULL;
    pInt10Info = NULL;
#endif /* TV_OUT */

    if (ConfiguredMonitor && !(flags & PROBE_DETECT))
    {
        xf86PrintEDID(ConfiguredMonitor);
        xf86SetDDCproperties(pScreenInfo, ConfiguredMonitor);
    }

#endif /* AVOID_CPIO */

    if (flags & PROBE_DETECT)
    {
        return TRUE;
    }

#ifndef AVOID_CPIO

    /* I/O bases might no longer be valid after BIOS initialisation */
    {
        if (pATI->CPIODecoding == BLOCK_IO)
            pATI->CPIOBase = PCI_REGION_BASE(pVideo, 1, REGION_IO);

        pATI->MMIOInLinear = FALSE;

        /* Set MMIO address from PCI configuration space, if available */
        if ((pATI->Block0Base = PCI_REGION_BASE(pVideo, 2, REGION_MEM)))
        {
            pATI->Block0Base += 0x0400U;
        }
    }

#endif /* AVOID_CPIO */

#ifndef XSERVER_LIBPCIACCESS
#ifdef AVOID_CPIO

    pScreenInfo->racMemFlags =
        RAC_FB | RAC_COLORMAP | RAC_VIEWPORT | RAC_CURSOR;

#else /* AVOID_CPIO */

    pScreenInfo->racIoFlags =
        RAC_FB | RAC_COLORMAP | RAC_VIEWPORT | RAC_CURSOR;
    pScreenInfo->racMemFlags = RAC_FB | RAC_CURSOR;

#endif /* AVOID_CPIO */
#endif
    /* Finish private area initialisation */
    pATI->nFIFOEntries = 16;                    /* For now */

    /* Finish probing the adapter */
    {
        /*
         * For MMIO register access, the MMIO address is computed when probing
         * and there are no BIOS calls. This mapping should always succeed.
         *
         * For CPIO register access, the MMIO address is computed above in the
         * presence of an auxiliary aperture. Otherwise, it is set to zero and
         * gets computed when we read the linear aperture configuration. This
         * mapping is either irrelevant or a no-op.
         */
        if (!ATIMapApertures(pScreenInfo->scrnIndex, pATI))
            return FALSE;

#ifdef AVOID_CPIO

            if (!pATI->pBlock[0])
            {
                xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
                    "Unable to mmap() adapter registers.\n");
                return FALSE;
            }

#endif /* AVOID_CPIO */

            /*
             * Verify register access by comparing against the CONFIG_CHIP_ID
             * value saved by adapter detection.
             */
            if (pATI->config_chip_id != inr(CONFIG_CHIP_ID))
            {
                xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
                    "Adapter registers not mapped correctly.\n");
                ATIUnmapApertures(pScreenInfo->scrnIndex, pATI);
                return FALSE;
            }

            pATIHW->crtc_gen_cntl = inr(CRTC_GEN_CNTL);
            if (!(pATIHW->crtc_gen_cntl & CRTC_EN) &&
                (pATI->Chip >= ATI_CHIP_264CT))
            {
                xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
                    "Adapter has not been initialised.\n");
                goto bail_locked;
            }

#ifdef AVOID_CPIO

            if (!(pATIHW->crtc_gen_cntl & CRTC_EXT_DISP_EN))
            {
                xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
                    "Adapters found to be in VGA mode on server entry are not"
                    " supported by the MMIO-only version of this driver.\n");
                goto bail_locked;
            }

#endif /* AVOID_CPIO */

            pATIHW->mem_cntl = inr(MEM_CNTL);
            if (pATI->Chip < ATI_CHIP_264VTB)
            {
                IOValue = GetBits(pATIHW->mem_cntl, CTL_MEM_SIZE);
                pATI->VideoRAM = videoRamSizes[IOValue];
            }
            else
            {
                pATI->nFIFOEntries =            /* Don't care */
                    (unsigned int)(-1) >> 1;

                IOValue = GetBits(pATIHW->mem_cntl, CTL_MEM_SIZEB);
                if (IOValue < 8)
                    pATI->VideoRAM = (IOValue + 1) * 512;
                else if (IOValue < 12)
                    pATI->VideoRAM = (IOValue - 3) * 1024;
                else
                    pATI->VideoRAM = (IOValue - 7) * 2048;
            }

            IOValue = inr(CONFIG_STATUS64_0);
            if (pATI->Chip >= ATI_CHIP_264CT)
            {
                pATI->MemoryType = GetBits(IOValue, CFG_MEM_TYPE_T);
            }
            else
            {
                pATI->MemoryType = GetBits(IOValue, CFG_MEM_TYPE);
            }

            pATI->LCDPanelID = -1;

            if (pATI->Chip >= ATI_CHIP_264CT)
            {
                /* Get LCD panel id */
                if (pATI->Chip == ATI_CHIP_264LT)
                {
                    pATI->LCDPanelID = GetBits(IOValue, CFG_PANEL_ID);

                    pATIHW->horz_stretching = inr(HORZ_STRETCHING);
                    pATIHW->vert_stretching = inr(VERT_STRETCHING);
                    pATIHW->lcd_gen_ctrl = inr(LCD_GEN_CTRL);
                }
                else if ((pATI->Chip == ATI_CHIP_264LTPRO) ||
                         (pATI->Chip == ATI_CHIP_264XL) ||
                         (pATI->Chip == ATI_CHIP_MOBILITY))
                {
                    pATI->LCDPanelID = GetBits(IOValue, CFG_PANEL_ID);

                    pATIHW->lcd_index = inr(LCD_INDEX);
                    pATIHW->horz_stretching =
                        ATIMach64GetLCDReg(LCD_HORZ_STRETCHING);
                    pATI->LCDHorizontal =
                        GetBits(pATIHW->horz_stretching, HORZ_PANEL_SIZE);
                    if (pATI->LCDHorizontal)
                    {
                        if (pATI->LCDHorizontal == MaxBits(HORZ_PANEL_SIZE))
                            pATI->LCDHorizontal = 0;
                        else
                            pATI->LCDHorizontal =
                                (pATI->LCDHorizontal + 1) << 3;
                    }
                    pATIHW->ext_vert_stretch =
                        ATIMach64GetLCDReg(LCD_EXT_VERT_STRETCH);
                    pATI->LCDVertical =
                        GetBits(pATIHW->ext_vert_stretch, VERT_PANEL_SIZE);
                    if (pATI->LCDVertical)
                    {
                        if (pATI->LCDVertical == MaxBits(VERT_PANEL_SIZE))
                            pATI->LCDVertical = 0;
                        else
                            pATI->LCDVertical++;
                    }
                    pATIHW->vert_stretching =
                        ATIMach64GetLCDReg(LCD_VERT_STRETCHING);
                    pATIHW->lcd_gen_ctrl = ATIMach64GetLCDReg(LCD_GEN_CNTL);
                    outr(LCD_INDEX, pATIHW->lcd_index);
                }

                /*
                 * Don't bother with panel support if it hasn't been previously
                 * enabled.
                 */
                if ((pATI->LCDPanelID >= 0) &&
                    !(pATIHW->horz_stretching & HORZ_STRETCH_EN) &&
                    !(pATIHW->vert_stretching & VERT_STRETCH_EN) &&
                    !(pATIHW->lcd_gen_ctrl & LCD_ON))
                {
                    /*
                     * At this point, if an XL or Mobility BIOS hasn't set
                     * panel dimensions, then there is no panel.  Otherwise,
                     * keep any panel disabled to allow for modes greater than
                     * the panel's dimensions.
                     */
                    if ((pATI->Chip >= ATI_CHIP_264XL) &&
                        (!pATI->LCDHorizontal || !pATI->LCDVertical))
                        pATI->LCDPanelID = -1;
                    else
                        pATI->OptionPanelDisplay = FALSE;
                }
            }

            /* Get DAC type */
            pATI->DAC = GetBits(inr(DAC_CNTL), DAC_TYPE);

            if (pATI->Chip < ATI_CHIP_264CT)
            {
                /* Factor in what the BIOS says the DAC is */
                pATI->DAC = ATI_DAC(pATI->DAC,
                    GetBits(inr(SCRATCH_REG1), BIOS_INIT_DAC_SUBTYPE));
            }

            /*
             * RAMDAC types 0 & 1 for Mach64's are different than those for
             * Mach32's.
             */
            if (pATI->DAC < ATI_DAC_ATI68875)
                pATI->DAC += ATI_DAC_INTERNAL;
    }

    {
        ROMTable = BIOSWord(0x48U);
        if ((ROMTable < 0x0002U) ||
            (BIOSWord(ROMTable - 0x02U) < 0x0012U) ||
            ((ROMTable + BIOSWord(ROMTable - 0x02U)) > BIOSSize))
            ROMTable = 0;

        if (ROMTable > 0)
        {
            ClockTable = BIOSWord(ROMTable + 0x10U);
            if ((ClockTable + 0x20U) > BIOSSize)
                ClockTable = 0;

            if (BIOSWord(ROMTable - 0x02U) >= 0x0048U)
            {
                VideoTable = BIOSWord(ROMTable + 0x46U);
                if ((VideoTable < 0x08U) ||
                    (BIOSByte(VideoTable - 0x01U) < 0x08U) ||
                    (BIOSByte(VideoTable - 0x02U) > 0x01U) ||
                    ((VideoTable + BIOSByte(VideoTable - 0x01U)) > BIOSSize))
                    VideoTable = 0;
            }

            if (BIOSWord(ROMTable - 0x02U) >= 0x004AU)
            {
                HardwareTable = BIOSWord(ROMTable + 0x48U);
                if (((HardwareTable + 0x08U) > BIOSSize) ||
                    (memcmp(BIOS + HardwareTable, "$ATI", 4) != 0))
                    HardwareTable = 0;
            }
        }

        ati_bios_clock(pScreenInfo, pATI, BIOS, ClockTable, pGDev);

        ati_bios_mmedia(pScreenInfo, pATI, BIOS, VideoTable, HardwareTable);

        if (pATI->LCDPanelID >= 0)
        {
            LCDTable = BIOSWord(0x78U);
            if ((LCDTable + BIOSByte(LCDTable + 5)) > BIOSSize)
                LCDTable = 0;

            ati_bios_panel_info(pScreenInfo, pATI, BIOS, BIOSSize, LCDTable);
        }

        xf86DrvMsgVerb(pScreenInfo->scrnIndex, X_INFO, 3,
            "BIOS Data:  BIOSSize=0x%04X, ROMTable=0x%04X.\n",
            BIOSSize, ROMTable);
        xf86DrvMsgVerb(pScreenInfo->scrnIndex, X_INFO, 3,
            "BIOS Data:  ClockTable=0x%04X, FrequencyTable=0x%04X.\n",
            ClockTable, FrequencyTable);
        xf86DrvMsgVerb(pScreenInfo->scrnIndex, X_INFO, 3,
            "BIOS Data:  LCDTable=0x%04X.\n",
            LCDTable);
        xf86DrvMsgVerb(pScreenInfo->scrnIndex, X_INFO, 3,
            "BIOS Data:  VideoTable=0x%04X, HardwareTable=0x%04X.\n",
            VideoTable, HardwareTable);
        xf86DrvMsgVerb(pScreenInfo->scrnIndex, X_INFO, 3,
            "BIOS Data:  I2CType=0x%02X, Tuner=0x%02X, Decoder=0x%02X,"
            " Audio=0x%02X.\n",
            pATI->I2CType, pATI->Tuner, pATI->Decoder, pATI->Audio);
    }

    ATIUnlock(pATI);            /* Unlock registers */

    /* Report what was found */
    xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
        "%s graphics controller detected.\n",
        xf86TokenToString(Mach64Chipsets, pATI->Chip));

    {
        Message = Buffer + snprintf(Buffer, SizeOf(Buffer), "Chip type %04X",
            pATI->ChipType);
        if (!(pATI->ChipType & ~(CHIP_CODE_0 | CHIP_CODE_1)))
            Message += snprintf(Message, Buffer + SizeOf(Buffer) - Message,
                " (%c%c)",
                GetBits(pATI->ChipType, CHIP_CODE_1) + 0x41U,
                GetBits(pATI->ChipType, CHIP_CODE_0) + 0x41U);
        else if ((pATI->ChipType & 0x4040U) == 0x4040U)
            Message += snprintf(Message, Buffer + SizeOf(Buffer) - Message,
                " \"%c%c\"",
                GetByte(pATI->ChipType, 1), GetByte(pATI->ChipType, 0));
        if ((pATI->Chip >= ATI_CHIP_264CT) && (pATI->Chip != ATI_CHIP_Mach64))
            Message += snprintf(Message, Buffer + SizeOf(Buffer) - Message,
                ", version %d, foundry %s",
                pATI->ChipVersion, ATIFoundryNames[pATI->ChipFoundry]);
        xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
            "%s, class %d, revision 0x%02X.\n",
            Buffer, pATI->ChipClass, pATI->ChipRevision);
    }

    {
        Message = Buffer + snprintf(Buffer, SizeOf(Buffer),
            "%s bus interface detected", ATIBusNames[pATI->BusType]);

#ifndef AVOID_CPIO

        {
            Message += snprintf(Message, Buffer + SizeOf(Buffer) - Message,
                ";  %s I/O base is 0x%04lX",
                (pATI->CPIODecoding == SPARSE_IO) ? "sparse" : "block",
                pATI->CPIOBase);
        }

#endif /* AVOID_CPIO */

        xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED, "%s.\n", Buffer);
    }

#ifndef XSERVER_LIBPCIACCESS
#ifndef AVOID_CPIO

    if (pATI->CPIO_VGAWonder)
        xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
            "VGA Wonder registers at I/O port 0x%04lX.\n",
            pATI->CPIO_VGAWonder);

#endif /* AVOID_CPIO */
#endif

    xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
        "ATI Mach64 adapter detected.\n");

    if (pATI->Chip >= ATI_CHIP_264GT)
        xf86DrvMsg(pScreenInfo->scrnIndex, X_NOTICE,
            "For information on using the multimedia capabilities\n\tof this"
            " adapter, please see http://gatos.sf.net.\n");

    if ((pATI->DAC & ~0x0FU) == ATI_DAC_INTERNAL)
    {
        xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
            "Internal RAMDAC (subtype %d) detected.\n", pATI->DAC & 0x0FU);
    }
    else
    {
        const SymTabRec *DAC;

        for (DAC = ATIDACDescriptors;  ;  DAC++)
        {
            if (pATI->DAC == DAC->token)
            {
                xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
                    "%s RAMDAC detected.\n", DAC->name);
                break;
            }

            if (pATI->DAC < DAC->token)
            {
                xf86DrvMsgVerb(pScreenInfo->scrnIndex, X_WARNING, 0,
                    "Unknown RAMDAC type 0x%02X detected.\n", pATI->DAC);
                break;
            }
        }
    }

#ifndef XSERVER_LIBPCIACCESS
    if (!xf86LinearVidMem())
    {
        xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
            "A linear aperture is not available.\n");
        goto bail;
    }
#endif

    /*
     * Set colour weights.
     */

    if (pATI->Chip < ATI_CHIP_264CT)
        pScreenInfo->rgbBits = 6;
    else
        pScreenInfo->rgbBits = 8;
    pATI->rgbBits = pScreenInfo->rgbBits;
    if (!xf86SetWeight(pScreenInfo, defaultWeight, defaultWeight))
        goto bail;

    if ((pScreenInfo->depth > 8) &&
        ((pScreenInfo->weight.red != pScreenInfo->weight.blue) ||
         (pScreenInfo->weight.red != (CARD32)(pScreenInfo->depth / 3)) ||
         ((CARD32)pScreenInfo->depth != (pScreenInfo->weight.red +
                                         pScreenInfo->weight.green +
                                         pScreenInfo->weight.blue))))
    {
        xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
            "Driver does not support weight %d%d%d for depth %d.\n",
            (int)pScreenInfo->weight.red, (int)pScreenInfo->weight.green,
            (int)pScreenInfo->weight.blue, pScreenInfo->depth);
        goto bail;
    }

    /*
     * Set default visual.
     */

    if (!xf86SetDefaultVisual(pScreenInfo, -1))
        goto bail;

    if ((pScreenInfo->depth > 8) &&
        (((pScreenInfo->defaultVisual | DynamicClass) != DirectColor) ||
         ((pScreenInfo->defaultVisual == DirectColor) &&
          (pATI->DAC == ATI_DAC_INTERNAL))))
    {
        xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
            "Driver does not support default visual %s for depth %d.\n",
            xf86GetVisualName(pScreenInfo->defaultVisual),
            pScreenInfo->depth);
        goto bail;
    }

    /*
     * Set colour gamma.
     */

    if (!xf86SetGamma(pScreenInfo, defaultGamma))
        goto bail;

    pATI->depth = pScreenInfo->depth;
    pATI->bitsPerPixel = pScreenInfo->bitsPerPixel;
    pATI->weight = pScreenInfo->weight;
    pATI->XModifier = pATI->bitsPerPixel / UnitOf(pATI->bitsPerPixel);

    /*
     * Determine which CRT controller to use for video modes.
     */

    {
        pATI->NewHW.crtc = ATI_CRTC_MACH64;

        xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
            "Using Mach64 accelerator CRTC.\n");

#ifndef XSERVER_LIBPCIACCESS
#ifndef AVOID_CPIO

        if (pATI->VGAAdapter)
        {
            /*
             * No need for VGA I/O resources during operating state (but they
             * are still decoded).
             */
            pResources =
                xf86SetOperatingState(resVgaIo, pATI->iEntity, ResUnusedOpr);
            if (pResources)
            {
                xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                    "Logic error setting operating state for VGA I/O.\n");
                xf86FreeResList(pResources);
            }

            if (pATI->CPIO_VGAWonder)
            {
                pResources = xf86SetOperatingState(pATI->VGAWonderResources,
                    pATI->iEntity, ResUnusedOpr);
                if (pResources)
                {
                    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                        "Logic error setting operating state for"
                        " VGAWonder I/O.\n");
                    xf86FreeResList(pResources);
                }
            }
        }

#endif /* AVOID_CPIO */
#endif

    }

    /*
     * Decide between the CRT and the panel.
     */
    if (pATI->LCDPanelID >= 0)
    {
        if (!pATI->OptionPanelDisplay)
        {
            xf86DrvMsg(pScreenInfo->scrnIndex, X_CONFIG,
                "Using CRT interface and disabling digital flat panel.\n");
        }
        else
        {
            unsigned HDisplay, VDisplay;
            CARD8 ClockMask, PostMask;

            /*
             * Determine porch data.  This groks the mode on entry to extract
             * the width and position of its sync and blanking pulses, and
             * considers any overscan as part of the displayed area, given that
             * the overscan is also stretched.
             *
             * This also attempts to determine panel dimensions but cannot do
             * so for one that is "auto-stretched".
             */

            if (pATI->Chip == ATI_CHIP_264LT)
            {
                pATIHW->lcd_gen_ctrl = inr(LCD_GEN_CTRL);

                /* Set up to read non-shadow registers */
                if (pATIHW->lcd_gen_ctrl & SHADOW_RW_EN)
                    outr(LCD_GEN_CTRL, pATIHW->lcd_gen_ctrl & ~SHADOW_RW_EN);
            }
            else /* if ((pATI->Chip == ATI_CHIP_264LTPRO) ||
                        (pATI->Chip == ATI_CHIP_264XL) ||
                        (pATI->Chip == ATI_CHIP_MOBILITY)) */
            {
                pATIHW->lcd_index = inr(LCD_INDEX);
                pATIHW->config_panel = ATIMach64GetLCDReg(LCD_CONFIG_PANEL);
                pATIHW->lcd_gen_ctrl = ATIMach64GetLCDReg(LCD_GEN_CNTL);

                /* Set up to read non-shadow registers */
                if (pATIHW->lcd_gen_ctrl & SHADOW_RW_EN)
                    ATIMach64PutLCDReg(LCD_GEN_CNTL,
                        pATIHW->lcd_gen_ctrl & ~SHADOW_RW_EN);
            }

#ifndef AVOID_CPIO

            if (!(pATIHW->crtc_gen_cntl & CRTC_EXT_DISP_EN))
            {
                unsigned HBlankStart, HSyncStart, HSyncEnd, HBlankEnd, HTotal;
                unsigned VBlankStart, VSyncStart, VSyncEnd, VBlankEnd, VTotal;

                pATIHW->clock = (inb(R_GENMO) & 0x0CU) >> 2;

                pATIHW->crt[2] = GetReg(CRTX(pATI->CPIO_VGABase), 0x02U);
                pATIHW->crt[3] = GetReg(CRTX(pATI->CPIO_VGABase), 0x03U);
                pATIHW->crt[5] = GetReg(CRTX(pATI->CPIO_VGABase), 0x05U);
                pATIHW->crt[7] = GetReg(CRTX(pATI->CPIO_VGABase), 0x07U);
                pATIHW->crt[9] = GetReg(CRTX(pATI->CPIO_VGABase), 0x09U);
                pATIHW->crt[21] = GetReg(CRTX(pATI->CPIO_VGABase), 0x15U);
                pATIHW->crt[22] = GetReg(CRTX(pATI->CPIO_VGABase), 0x16U);

                pATIHW->crtc_h_total_disp = inr(CRTC_H_TOTAL_DISP);
                pATIHW->crtc_h_sync_strt_wid = inr(CRTC_H_SYNC_STRT_WID);
                pATIHW->crtc_v_total_disp = inr(CRTC_V_TOTAL_DISP);
                pATIHW->crtc_v_sync_strt_wid = inr(CRTC_V_SYNC_STRT_WID);

                /* Switch to shadow registers */
                if (pATI->Chip == ATI_CHIP_264LT)
                    outr(LCD_GEN_CTRL, pATIHW->lcd_gen_ctrl | SHADOW_RW_EN);
                else /* if ((pATI->Chip == ATI_CHIP_264LTPRO) ||
                            (pATI->Chip == ATI_CHIP_264XL) ||
                            (pATI->Chip == ATI_CHIP_MOBILITY)) */
                    ATIMach64PutLCDReg(LCD_GEN_CNTL,
                        pATIHW->lcd_gen_ctrl | SHADOW_RW_EN);

                pATIHW->shadow_vga[2] =
                    GetReg(CRTX(pATI->CPIO_VGABase), 0x02U);
                pATIHW->shadow_vga[3] =
                    GetReg(CRTX(pATI->CPIO_VGABase), 0x03U);
                pATIHW->shadow_vga[5] =
                    GetReg(CRTX(pATI->CPIO_VGABase), 0x05U);
                pATIHW->shadow_vga[7] =
                    GetReg(CRTX(pATI->CPIO_VGABase), 0x07U);
                pATIHW->shadow_vga[9] =
                    GetReg(CRTX(pATI->CPIO_VGABase), 0x09U);
                pATIHW->shadow_vga[21] =
                    GetReg(CRTX(pATI->CPIO_VGABase), 0x15U);
                pATIHW->shadow_vga[22] =
                    GetReg(CRTX(pATI->CPIO_VGABase), 0x16U);

                pATIHW->shadow_h_total_disp = inr(CRTC_H_TOTAL_DISP);
                pATIHW->shadow_h_sync_strt_wid = inr(CRTC_H_SYNC_STRT_WID);
                pATIHW->shadow_v_total_disp = inr(CRTC_V_TOTAL_DISP);
                pATIHW->shadow_v_sync_strt_wid = inr(CRTC_V_SYNC_STRT_WID);

                /*
                 * HSyncStart and HSyncEnd should equal their shadow
                 * counterparts.  Otherwise, due to a chip bug, the panel might
                 * not sync, regardless of which register set is used to drive
                 * the panel.  There are certain combinations of register
                 * values where the panel does in fact sync, but it remains
                 * impossible to accurately determine the horizontal sync pulse
                 * timing actually seen by the panel.
                 *
                 * Note that this hardware bug does not affect the CRT output.
                 */
                if (((pATIHW->crtc_h_sync_strt_wid ^
                      pATIHW->shadow_h_sync_strt_wid) &
                     (CRTC_H_SYNC_STRT | CRTC_H_SYNC_STRT_HI |
                      CRTC_H_SYNC_WID)))
                {
                    xf86DrvMsgVerb(pScreenInfo->scrnIndex, X_NOTICE, 0,
                        "Invalid horizontal sync pulse timing detected in mode"
                        " on server entry.\n");

                    /* Don't trust input timing */
                    pATI->OptionLCDSync = TRUE;
                    ModeType = 0;
                }

                /* Merge in shadow registers as appropriate */
                if (pATIHW->lcd_gen_ctrl & SHADOW_EN)
                {
                    pATIHW->crt[2] = pATIHW->shadow_vga[2];
                    pATIHW->crt[3] = pATIHW->shadow_vga[3];
                    pATIHW->crt[5] = pATIHW->shadow_vga[5];

                    /* XXX Does this apply to VGA?  If so, what about the LT? */
                    if ((pATI->Chip < ATI_CHIP_264LTPRO) ||
                        !(pATIHW->config_panel & DONT_SHADOW_HEND))
                    {
                        pATIHW->crtc_h_total_disp &= ~CRTC_H_DISP;
                        pATIHW->crtc_h_total_disp |=
                            pATIHW->shadow_h_total_disp & CRTC_H_DISP;
                    }

                    pATIHW->crtc_h_total_disp &= ~CRTC_H_TOTAL;
                    pATIHW->crtc_h_total_disp |=
                        pATIHW->shadow_h_total_disp & CRTC_H_TOTAL;
                    pATIHW->crtc_h_sync_strt_wid =
                        pATIHW->shadow_h_sync_strt_wid;

                    /* XXX Does this apply to VGA? */
                    if (pATIHW->lcd_gen_ctrl & USE_SHADOWED_VEND)
                    {
                        pATIHW->crtc_v_total_disp &= ~CRTC_V_DISP;
                        pATIHW->crtc_v_total_disp |=
                            pATIHW->shadow_v_total_disp & CRTC_V_DISP;
                    }

                    if (!(pATIHW->lcd_gen_ctrl & DONT_SHADOW_VPAR))
                    {
                        pATIHW->crt[7] = pATIHW->shadow_vga[7];
                        pATIHW->crt[9] = pATIHW->shadow_vga[9];
                        pATIHW->crt[21] = pATIHW->shadow_vga[21];
                        pATIHW->crt[22] = pATIHW->shadow_vga[22];

                        pATIHW->crtc_v_total_disp &= ~CRTC_V_TOTAL;
                        pATIHW->crtc_v_total_disp |=
                            pATIHW->shadow_v_total_disp & CRTC_V_TOTAL;
                    }
                }

                if (!(pATIHW->lcd_gen_ctrl & DONT_SHADOW_VPAR))
                    pATIHW->crtc_v_sync_strt_wid =
                        pATIHW->shadow_v_sync_strt_wid;

                /*
                 * Decipher input timing.  This is complicated by the fact that
                 * the full width of all timing parameters, except for the
                 * blanking pulses, is only available through the accelerator
                 * registers, not the VGA ones.  Blanking pulse boundaries must
                 * then be interpolated.
                 *
                 * Note that, in VGA mode, the accelerator's sync width fields
                 * are actually end positions, not widths.
                 */
                HDisplay = GetBits(pATIHW->crtc_h_total_disp, CRTC_H_DISP);
                HSyncStart =
                    (GetBits(pATIHW->crtc_h_sync_strt_wid,
                        CRTC_H_SYNC_STRT_HI) *
                     (MaxBits(CRTC_H_SYNC_STRT) + 1)) |
                    GetBits(pATIHW->crtc_h_sync_strt_wid, CRTC_H_SYNC_STRT);
                HSyncEnd = (HSyncStart & ~MaxBits(CRTC_H_SYNC_WID)) |
                    GetBits(pATIHW->crtc_h_sync_strt_wid, CRTC_H_SYNC_WID);
                if (HSyncStart >= HSyncEnd)
                    HSyncEnd += MaxBits(CRTC_H_SYNC_WID) + 1;
                HTotal = GetBits(pATIHW->crtc_h_total_disp, CRTC_H_TOTAL);

                HBlankStart = (HDisplay & ~0xFFU) | pATIHW->crt[2];
                if (HDisplay > HBlankStart)
                    HBlankStart += 0x0100U;
                HBlankEnd = (HSyncEnd & ~0x3FU) |
                    ((pATIHW->crt[5] >> 2) & 0x20U) |
                    (pATIHW->crt[3] & 0x1FU);
                if (HSyncEnd > (HBlankEnd + 1))
                    HBlankEnd += 0x40U;

                VDisplay = GetBits(pATIHW->crtc_v_total_disp, CRTC_V_DISP);
                VSyncStart =
                    GetBits(pATIHW->crtc_v_sync_strt_wid, CRTC_V_SYNC_STRT);
                VSyncEnd = (VSyncStart & ~MaxBits(CRTC_V_SYNC_END_VGA)) |
                    GetBits(pATIHW->crtc_v_sync_strt_wid, CRTC_V_SYNC_END_VGA);
                if (VSyncStart > VSyncEnd)
                    VSyncEnd += MaxBits(CRTC_V_SYNC_END_VGA) + 1;
                VTotal = GetBits(pATIHW->crtc_v_total_disp, CRTC_V_TOTAL);

                VBlankStart = (VDisplay & ~0x03FFU) |
                   ((pATIHW->crt[9] << 4) & 0x0200U) |
                   ((pATIHW->crt[7] << 5) & 0x0100U) | pATIHW->crt[21];
                if (VDisplay > VBlankStart)
                   VBlankStart += 0x0400U;
                VBlankEnd = (VSyncEnd & ~0x00FFU) | pATIHW->crt[22];
                if (VSyncEnd > (VBlankEnd + 1))
                   VBlankEnd += 0x0100U;

                pATI->LCDHBlankWidth = HBlankEnd - HBlankStart;
                pATI->LCDHSyncStart = HSyncStart - HBlankStart;
                pATI->LCDHSyncWidth = HSyncEnd - HSyncStart;

                pATI->LCDVBlankWidth = VBlankEnd - VBlankStart;
                pATI->LCDVSyncStart = VSyncStart - VBlankStart;
                pATI->LCDVSyncWidth = VSyncEnd - VSyncStart;

                HDisplay = HTotal + 5 - pATI->LCDHBlankWidth;
                VDisplay = VTotal + 2 - pATI->LCDVBlankWidth;
            }
            else

#endif /* AVOID_CPIO */

            {
                pATIHW->clock = inr(CLOCK_CNTL) & 0x03U;

                pATIHW->crtc_h_total_disp = inr(CRTC_H_TOTAL_DISP);
                pATIHW->crtc_h_sync_strt_wid = inr(CRTC_H_SYNC_STRT_WID);
                pATIHW->crtc_v_total_disp = inr(CRTC_V_TOTAL_DISP);
                pATIHW->crtc_v_sync_strt_wid = inr(CRTC_V_SYNC_STRT_WID);
                pATIHW->ovr_wid_left_right = inr(OVR_WID_LEFT_RIGHT);
                pATIHW->ovr_wid_top_bottom = inr(OVR_WID_TOP_BOTTOM);

                /* Switch to shadow registers */
                if (pATI->Chip == ATI_CHIP_264LT)
                    outr(LCD_GEN_CTRL, pATIHW->lcd_gen_ctrl | SHADOW_RW_EN);
                else /* if ((pATI->Chip == ATI_CHIP_264LTPRO) ||
                            (pATI->Chip == ATI_CHIP_264XL) ||
                            (pATI->Chip == ATI_CHIP_MOBILITY)) */
                    ATIMach64PutLCDReg(LCD_GEN_CNTL,
                        pATIHW->lcd_gen_ctrl | SHADOW_RW_EN);

                /* Oddly enough, there are no shadow overscan registers */
                pATIHW->shadow_h_total_disp = inr(CRTC_H_TOTAL_DISP);
                pATIHW->shadow_h_sync_strt_wid = inr(CRTC_H_SYNC_STRT_WID);
                pATIHW->shadow_v_total_disp = inr(CRTC_V_TOTAL_DISP);
                pATIHW->shadow_v_sync_strt_wid = inr(CRTC_V_SYNC_STRT_WID);

                /*
                 * HSyncStart and HSyncEnd should equal their shadow
                 * counterparts.  Otherwise, due to a chip bug, the panel might
                 * not sync, regardless of which register set is used to drive
                 * the panel.  There are certain combinations of register
                 * values where the panel does in fact sync, but it remains
                 * impossible to accurately determine the horizontal sync pulse
                 * timing actually seen by the panel.
                 *
                 * Note that this hardware bug does not affect the CRT output.
                 */
                if (((pATIHW->crtc_h_sync_strt_wid ^
                      pATIHW->shadow_h_sync_strt_wid) &
                     (CRTC_H_SYNC_STRT | CRTC_H_SYNC_STRT_HI |
                      CRTC_H_SYNC_WID)))
                {
                    xf86DrvMsgVerb(pScreenInfo->scrnIndex, X_NOTICE, 0,
                        "Invalid horizontal sync pulse timing detected in mode"
                        " on server entry.\n");

                    /* Don't trust input timing */
                    pATI->OptionLCDSync = TRUE;
                    ModeType = 0;
                }

                /* Merge in shadow registers as appropriate */
                if (pATIHW->lcd_gen_ctrl & SHADOW_EN)
                {
                    /* XXX What about the LT? */
                    if ((pATI->Chip < ATI_CHIP_264LTPRO) ||
                        !(pATIHW->config_panel & DONT_SHADOW_HEND))
                    {
                        pATIHW->crtc_h_total_disp &= ~CRTC_H_DISP;
                        pATIHW->crtc_h_total_disp |=
                            pATIHW->shadow_h_total_disp & CRTC_H_DISP;
                    }

                    pATIHW->crtc_h_total_disp &= ~CRTC_H_TOTAL;
                    pATIHW->crtc_h_total_disp |=
                        pATIHW->shadow_h_total_disp & CRTC_H_TOTAL;
                    pATIHW->crtc_h_sync_strt_wid =
                        pATIHW->shadow_h_sync_strt_wid;

                    if (pATIHW->lcd_gen_ctrl & USE_SHADOWED_VEND)
                    {
                        pATIHW->crtc_v_total_disp &= ~CRTC_V_DISP;
                        pATIHW->crtc_v_total_disp |=
                            pATIHW->shadow_v_total_disp & CRTC_V_DISP;
                    }

                    if (!(pATIHW->lcd_gen_ctrl & DONT_SHADOW_VPAR))
                    {
                        pATIHW->crtc_v_total_disp &= ~CRTC_V_TOTAL;
                        pATIHW->crtc_v_total_disp |=
                            pATIHW->shadow_v_total_disp & CRTC_V_TOTAL;
                    }
                }

                if (!(pATIHW->lcd_gen_ctrl & DONT_SHADOW_VPAR))
                    pATIHW->crtc_v_sync_strt_wid =
                        pATIHW->shadow_v_sync_strt_wid;

                /* Decipher input timing */
                HDisplay = GetBits(pATIHW->crtc_h_total_disp, CRTC_H_DISP) +
                    GetBits(pATIHW->ovr_wid_left_right, OVR_WID_LEFT) +
                    GetBits(pATIHW->ovr_wid_left_right, OVR_WID_RIGHT);
                VDisplay = GetBits(pATIHW->crtc_v_total_disp, CRTC_V_DISP) +
                    GetBits(pATIHW->ovr_wid_top_bottom, OVR_WID_TOP) +
                    GetBits(pATIHW->ovr_wid_top_bottom, OVR_WID_BOTTOM);

                pATI->LCDHSyncStart =
                    (GetBits(pATIHW->crtc_h_sync_strt_wid,
                        CRTC_H_SYNC_STRT_HI) *
                     (MaxBits(CRTC_H_SYNC_STRT) + 1)) +
                    GetBits(pATIHW->crtc_h_sync_strt_wid, CRTC_H_SYNC_STRT) -
                    HDisplay;
                pATI->LCDHSyncWidth =
                    GetBits(pATIHW->crtc_h_sync_strt_wid, CRTC_H_SYNC_WID);
                pATI->LCDHBlankWidth =
                    GetBits(pATIHW->crtc_h_total_disp, CRTC_H_TOTAL) -
                    HDisplay;
                pATI->LCDVSyncStart =
                    GetBits(pATIHW->crtc_v_sync_strt_wid, CRTC_V_SYNC_STRT) -
                    VDisplay;
                pATI->LCDVSyncWidth =
                    GetBits(pATIHW->crtc_v_sync_strt_wid, CRTC_V_SYNC_WID);
                pATI->LCDVBlankWidth =
                    GetBits(pATIHW->crtc_v_total_disp, CRTC_V_TOTAL) -
                    VDisplay;

                HDisplay++;
                VDisplay++;
            }

            /* Restore LCD registers */
            if (pATI->Chip == ATI_CHIP_264LT)
            {
                outr(LCD_GEN_CTRL, pATIHW->lcd_gen_ctrl);
            }
            else /* if ((pATI->Chip == ATI_CHIP_264LTPRO) ||
                        (pATI->Chip == ATI_CHIP_264XL) ||
                        (pATI->Chip == ATI_CHIP_MOBILITY)) */
            {
                ATIMach64PutLCDReg(LCD_GEN_CNTL, pATIHW->lcd_gen_ctrl);
                outr(LCD_INDEX, pATIHW->lcd_index);
            }

            HDisplay <<= 3;
            pATI->LCDHSyncStart <<= 3;
            pATI->LCDHSyncWidth <<= 3;
            pATI->LCDHBlankWidth <<= 3;

            /* Calculate panel dimensions implied by the input timing */
            if ((pATIHW->horz_stretching &
                 (HORZ_STRETCH_EN | AUTO_HORZ_RATIO)) ==
                HORZ_STRETCH_EN)
            {
                if (pATIHW->horz_stretching & HORZ_STRETCH_MODE)
                {
                    if (pATIHW->horz_stretching & HORZ_STRETCH_BLEND)
                    {
                        HDisplay =
                            (HDisplay * (MaxBits(HORZ_STRETCH_BLEND) + 1)) /
                            GetBits(pATIHW->horz_stretching,
                                HORZ_STRETCH_BLEND);
                    }
                }
                else if (((pATIHW->horz_stretching & HORZ_STRETCH_LOOP) >
                          HORZ_STRETCH_LOOP15) ||
                         (pATIHW->horz_stretching &
                          SetBits(1, HORZ_STRETCH_RATIO)))
                {
                    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                        "Ignoring invalid horizontal stretch ratio in mode on"
                        " server entry.\n");
                }
                else
                {
                    IOValue =
                        GetBits(pATIHW->horz_stretching, HORZ_STRETCH_RATIO);

                    switch (GetBits(pATIHW->horz_stretching,
                                    HORZ_STRETCH_LOOP))
                    {
                        case GetBits(HORZ_STRETCH_LOOP09, HORZ_STRETCH_LOOP):
                            i = 9;
                            IOValue &= (1 << 9) - 1;
                            break;

                        case GetBits(HORZ_STRETCH_LOOP11, HORZ_STRETCH_LOOP):
                            i = 11;
                            IOValue &= (1 << 11) - 1;
                            break;

                        case GetBits(HORZ_STRETCH_LOOP12, HORZ_STRETCH_LOOP):
                            i = 12;
                            IOValue &= (1 << 12) - 1;
                            break;

                        case GetBits(HORZ_STRETCH_LOOP14, HORZ_STRETCH_LOOP):
                            i = 14;
                            IOValue &= (1 << 14) - 1;
                            break;

                        case GetBits(HORZ_STRETCH_LOOP15, HORZ_STRETCH_LOOP):
                        default:    /* Muffle compiler */
                            i = 15;
                            IOValue &= (1 << 15) - 1;
                            break;
                    }

                    if (IOValue)
                    {
                        /* Count the number of bits in IOValue */
                        j = (IOValue >> 1) & 0x36DBU;
                        j = IOValue - j - ((j >> 1) & 0x36DBU);
                        j = ((j + (j >> 3)) & 0x71C7U) % 0x3FU;

                        HDisplay = (HDisplay * i) / j;
                    }
                }
            }

            if ((pATIHW->vert_stretching & VERT_STRETCH_EN) &&
                !(pATIHW->ext_vert_stretch & AUTO_VERT_RATIO))
            {
                if ((pATIHW->vert_stretching & VERT_STRETCH_USE0) ||
                    (VDisplay <= 350))
                    IOValue =
                        GetBits(pATIHW->vert_stretching, VERT_STRETCH_RATIO0);
                else if (VDisplay <= 400)
                    IOValue =
                        GetBits(pATIHW->vert_stretching, VERT_STRETCH_RATIO1);
                else if ((VDisplay <= 480) ||
                         !(pATIHW->ext_vert_stretch & VERT_STRETCH_RATIO3))
                    IOValue =
                        GetBits(pATIHW->vert_stretching, VERT_STRETCH_RATIO2);
                else
                    IOValue =
                        GetBits(pATIHW->ext_vert_stretch, VERT_STRETCH_RATIO3);

                if (IOValue)
                    VDisplay =
                        (VDisplay * (MaxBits(VERT_STRETCH_RATIO0) + 1)) /
                        IOValue;
            }

            /* Match calculated dimensions to probed dimensions */
            if (!pATI->LCDHorizontal)
            {
                if ((pATIHW->horz_stretching &
                     (HORZ_STRETCH_EN | AUTO_HORZ_RATIO)) !=
                     (HORZ_STRETCH_EN | AUTO_HORZ_RATIO))
                    pATI->LCDHorizontal = HDisplay;
            }
            else if (pATI->LCDHorizontal != (int)HDisplay)
            {
                if ((pATIHW->horz_stretching &
                    (HORZ_STRETCH_EN | AUTO_HORZ_RATIO)) !=
                    (HORZ_STRETCH_EN | AUTO_HORZ_RATIO))
                    xf86DrvMsgVerb(pScreenInfo->scrnIndex, X_WARNING, 4,
                        "Inconsistent panel horizontal dimension:"
                        "  %d and %d.\n", pATI->LCDHorizontal, HDisplay);
                HDisplay = pATI->LCDHorizontal;
            }

            if (!pATI->LCDVertical)
            {
                if (!(pATIHW->vert_stretching & VERT_STRETCH_EN) ||
                    !(pATIHW->ext_vert_stretch & AUTO_VERT_RATIO))
                    pATI->LCDVertical = VDisplay;
            }
            else if (pATI->LCDVertical != (int)VDisplay)
            {
                if (!(pATIHW->vert_stretching & VERT_STRETCH_EN) ||
                    !(pATIHW->ext_vert_stretch & AUTO_VERT_RATIO))
                    xf86DrvMsgVerb(pScreenInfo->scrnIndex, X_WARNING, 4,
                        "Inconsistent panel vertical dimension:  %d and %d.\n",
                        pATI->LCDVertical, VDisplay);
                VDisplay = pATI->LCDVertical;
            }

            if (!pATI->LCDHorizontal || !pATI->LCDVertical)
            {
                if (pATI->LCDPanelID || (pATI->Chip <= ATI_CHIP_264LTPRO))
                    xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
                        "Unable to determine dimensions of panel (ID %d).\n",
                        pATI->LCDPanelID);
                else
                    xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
                        "Unable to determine dimensions of panel.\n");

                goto bail;
            }

            /* If the mode on entry wasn't stretched, adjust timings */
            if (!(pATIHW->horz_stretching & HORZ_STRETCH_EN) &&
                (pATI->LCDHorizontal > (int)HDisplay))
            {
                HDisplay = pATI->LCDHorizontal - HDisplay;
                if (pATI->LCDHSyncStart >= HDisplay)
                    pATI->LCDHSyncStart -= HDisplay;
                else
                    pATI->LCDHSyncStart = 0;
                pATI->LCDHBlankWidth -= HDisplay;
                HDisplay = pATI->LCDHSyncStart + pATI->LCDHSyncWidth;
                if (pATI->LCDHBlankWidth < HDisplay)
                    pATI->LCDHBlankWidth = HDisplay;
            }

            if (!(pATIHW->vert_stretching & VERT_STRETCH_EN) &&
                (pATI->LCDVertical > (int)VDisplay))
            {
                VDisplay = pATI->LCDVertical - VDisplay;
                if (pATI->LCDVSyncStart >= VDisplay)
                    pATI->LCDVSyncStart -= VDisplay;
                else
                    pATI->LCDVSyncStart = 0;
                pATI->LCDVBlankWidth -= VDisplay;
                VDisplay = pATI->LCDVSyncStart + pATI->LCDVSyncWidth;
                if (pATI->LCDVBlankWidth < VDisplay)
                    pATI->LCDVBlankWidth = VDisplay;
            }

            if (pATI->LCDPanelID || (pATI->Chip <= ATI_CHIP_264LTPRO))
                xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
                    "%dx%d panel (ID %d) detected.\n",
                    pATI->LCDHorizontal, pATI->LCDVertical, pATI->LCDPanelID);
            else
                xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
                    "%dx%d panel detected.\n",
                    pATI->LCDHorizontal, pATI->LCDVertical);

            /*
             * Determine panel clock.  This must be done after option
             * processing so that the adapter's reference frequency is always
             * available.
             *
             * Get post divider.  A GCC bug has caused the following expression
             * to be broken down into its individual components.
             */
            ClockMask = PLL_VCLK0_XDIV << pATIHW->clock;
            PostMask = PLL_VCLK0_POST_DIV << (pATIHW->clock * 2);
            i = GetBits(ATIMach64GetPLLReg(PLL_XCLK_CNTL), ClockMask);
            i *= MaxBits(PLL_VCLK0_POST_DIV) + 1;
            i |= GetBits(ATIMach64GetPLLReg(PLL_VCLK_POST_DIV), PostMask);

            /* Calculate clock of mode on entry */
            Numerator = ATIMach64GetPLLReg(PLL_VCLK0_FB_DIV + pATIHW->clock) *
                pATI->ReferenceNumerator;
            Denominator = pATI->ClockDescriptor.MinM *
                pATI->ReferenceDenominator *
                pATI->ClockDescriptor.PostDividers[i];
            pATI->LCDClock = ATIDivide(Numerator, Denominator, 1, 0);

            xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
                "Panel clock is %.3f MHz.\n",
                (double)(pATI->LCDClock) / 1000.0);

            xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
                "Using digital flat panel interface%s.\n",
                pATI->OptionCRTDisplay ?
                    " to display on both CRT and panel" : "");
        }
    }

    /*
     * Finish detecting video RAM size.
     */
    pScreenInfo->videoRam = pATI->VideoRAM;

    {
        {
            /* Get adapter's linear aperture configuration */
            pATIHW->config_cntl = inr(CONFIG_CNTL);
            pATI->LinearBase =
                GetBits(pATIHW->config_cntl, CFG_MEM_AP_LOC) << 22;
            if ((pATIHW->config_cntl & CFG_MEM_AP_SIZE) != CFG_MEM_AP_SIZE)
            {
                pATI->LinearSize =
                    GetBits(pATIHW->config_cntl, CFG_MEM_AP_SIZE) << 22;

                /*
                 * Linear aperture could have been disabled (but still
                 * assigned) by BIOS initialisation.
                 */
                if (pATI->LinearBase && !pATI->LinearSize)
                {
                    if ((pATI->Chip <= ATI_CHIP_88800GXD) &&
                        (pATI->VideoRAM < 4096))
                        pATI->LinearSize = 4 * 1024 * 1024;
                    else
                        pATI->LinearSize = 8 * 1024 * 1024;
                }
            }

            if (pATI->LinearBase && pATI->LinearSize)
            {
                int AcceleratorVideoRAM = 0, ServerVideoRAM;

#ifndef AVOID_CPIO

                /*
                 * Unless specified in PCI configuration space, set MMIO
                 * address to tail end of linear aperture.
                 */
                if (!pATI->Block0Base)
                {
                    pATI->Block0Base =
                        pATI->LinearBase + pATI->LinearSize - 0x00000400U;
                    pATI->MMIOInLinear = TRUE;
                }

#endif /* AVOID_CPIO */

                AcceleratorVideoRAM = pATI->LinearSize >> 10;

                /*
                 * Account for MMIO area at the tail end of the linear
                 * aperture, if it is needed or if it cannot be disabled.
                 */
                if (pATI->MMIOInLinear || (pATI->Chip < ATI_CHIP_264VTB))
                    AcceleratorVideoRAM -= 2;

                ServerVideoRAM = pATI->VideoRAM;

                if (pATI->Cursor > ATI_CURSOR_SOFTWARE)
                {
                    /*
                     * Allocate a 1 kB cursor image area at the top of the
                     * little-endian aperture, just before any MMIO area that
                     * might also be there.
                     */
                    if (ServerVideoRAM > AcceleratorVideoRAM)
                        ServerVideoRAM = AcceleratorVideoRAM;

                    ServerVideoRAM--;
                    pATI->CursorOffset = ServerVideoRAM << 10;
                    pATI->CursorBase = pATI->LinearBase + pATI->CursorOffset;

                    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
                        "Storing hardware cursor image at 0x%08lX.\n",
                        pATI->CursorBase);
                }

                {
                    CARD32 PageSize = getpagesize() >> 10;

#if X_BYTE_ORDER == X_LITTLE_ENDIAN

                    /*
                     * MMIO areas must be mmap()'ed separately to avoid write
                     * combining them.  Thus, they might not end up still
                     * adjacent with the little-endian linear aperture after
                     * mmap()'ing.  So, round down the linear aperture size to
                     * avoid an overlap.  Any hardware cursor image area might
                     * not end up being write combined, but this seems
                     * preferable to further reducing the video memory size
                     * advertised to the server.
                     *
                     * XXX Ideally this should be dealt with in the os-support
                     *     layer, i.e., it should be possible to reset a
                     *     subarea's write combining after it has been
                     *     mmap()'ed, but doing so currently causes the removal
                     *     of write combining for the entire aperture.
                     */
                    if (pATI->MMIOInLinear)
                        AcceleratorVideoRAM -= AcceleratorVideoRAM % PageSize;

#else /* if X_BYTE_ORDER != X_LITTLE_ENDIAN */

                    /*
                     * Big-endian apertures are 8 MB higher and don't contain
                     * an MMIO area.
                     */
                    pATI->LinearBase += 0x00800000U;
                    AcceleratorVideoRAM = pATI->LinearSize >> 10;

#endif /* X_BYTE_ORDER */

                    if (ServerVideoRAM > AcceleratorVideoRAM)
                        ServerVideoRAM = AcceleratorVideoRAM;
                    else if (AcceleratorVideoRAM > pATI->VideoRAM)
                        AcceleratorVideoRAM = pATI->VideoRAM;

                    PageSize--;
                    AcceleratorVideoRAM =
                        (AcceleratorVideoRAM + PageSize) & ~PageSize;

                    xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
                        "Using %d MB linear aperture at 0x%08lX.\n",
                        pATI->LinearSize >> 20, pATI->LinearBase);

                    /* Only mmap what is needed */
                    ApertureSize = pATI->LinearSize =
                        AcceleratorVideoRAM << 10;
                }

                if (ServerVideoRAM < pATI->VideoRAM)
                {
                    pScreenInfo->videoRam = ServerVideoRAM;
                    xf86DrvMsg(pScreenInfo->scrnIndex, X_NOTICE,
                        "Virtual resolutions will be limited to %d kB\n due to"
                        " linear aperture size and/or placement of hardware"
                        " cursor image area.\n",
                        ServerVideoRAM);
                }
            }
        }

        if (!pATI->LinearBase || !pATI->LinearSize)
        {
                xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
                    "Linear aperture not available.\n");
                goto bail;
        }

        if (pATI->Block0Base)
        {
            xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
                "Using Block 0 MMIO aperture at 0x%08lX.\n", pATI->Block0Base);

            /* Set Block1 MMIO address if supported */
            if (pATI->Chip >= ATI_CHIP_264VT)
            {
                pATI->Block1Base = pATI->Block0Base - 0x00000400U;
                xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
                    "Using Block 1 MMIO aperture at 0x%08lX.\n",
                    pATI->Block1Base);
            }
        }
    }

#ifndef XSERVER_LIBPCIACCESS
#ifndef AVOID_CPIO

        if (pATI->VGAAdapter)
        {
            /*
             * Free VGA memory aperture during operating state (but it is still
             * decoded).
             */
            pResources = xf86SetOperatingState(resVgaMem, pATI->iEntity,
                ResUnusedOpr);
            if (pResources)
            {
                xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                    "Logic error setting operating state for VGA memory"
                    " aperture.\n");
                xf86FreeResList(pResources);
            }
        }

#endif /* AVOID_CPIO */
#endif

    /*
     * Remap apertures.  Must lock and re-unlock around this in case the
     * remapping fails.
     */
    ATILock(pATI);
    ATIUnmapApertures(pScreenInfo->scrnIndex, pATI);
    if (!ATIMapApertures(pScreenInfo->scrnIndex, pATI))
        return FALSE;

    ATIUnlock(pATI);

    if (pATI->OptionAccel)
    {
            xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
                "MMIO write caching %sabled.\n",
                pATI->OptionMMIOCache ? "en" : "dis");
    }

    {
        if (pATI->Chip >= ATI_CHIP_264CT)
            ATIReportMemory(pScreenInfo, pATI,
                ATIMemoryTypeNames_264xT[pATI->MemoryType]);
        else if (pATI->Chip == ATI_CHIP_88800CX)
            ATIReportMemory(pScreenInfo, pATI,
                ATIMemoryTypeNames_88800CX[pATI->MemoryType]);
        else
            ATIReportMemory(pScreenInfo, pATI,
                ATIMemoryTypeNames_Mach[pATI->MemoryType]);
    }

    /*
     * Finish banking setup.  This needs to be fixed to not assume the mode on
     * entry is a VGA mode.  XXX
     */

#ifndef AVOID_CPIO

    if (!pATI->VGAAdapter)
    {
        pATI->NewHW.SetBank = ATIx8800SetBank;
        pATI->NewHW.nPlane = 0;

        pATIHW->crtc = pATI->NewHW.crtc;

        pATIHW->SetBank = (ATIBankProcPtr)NoopDDA;
    }
    else
    {
        Bool ext_disp_en = (pATI->LockData.crtc_gen_cntl & CRTC_EXT_DISP_EN);
        Bool vga_ap_en = (pATI->LockData.config_cntl & CFG_MEM_VGA_AP_EN);
        Bool vga_color_256 = (GetReg(SEQX, 0x04U) & 0x08U);

        pATI->NewHW.SetBank = ATIMach64SetBankPacked;
        pATI->NewHW.nPlane = 1;

        pATIHW->crtc = ATI_CRTC_VGA;

        if (ext_disp_en)
            pATIHW->crtc = ATI_CRTC_MACH64;

        if ((pATIHW->crtc != ATI_CRTC_VGA) || vga_color_256)
            pATIHW->nPlane = 1;
        else
            pATIHW->nPlane = 4;

        /* VideoRAM is a multiple of 512kB and BankSize is 64kB */
        pATIHW->nBank = pATI->VideoRAM / (pATIHW->nPlane * 0x40U);

        if ((pATIHW->crtc == ATI_CRTC_VGA) && !vga_ap_en)
        {
            pATIHW->SetBank = (ATIBankProcPtr)NoopDDA;
            pATIHW->nBank = 1;
        }
        else if (pATIHW->nPlane == 1)
        {
            pATIHW->SetBank = ATIMach64SetBankPacked;
        }
        else
        {
            pATIHW->SetBank = ATIMach64SetBankPlanar;
        }
    }

#else /* AVOID_CPIO */

    {
        pATIHW->crtc = pATI->NewHW.crtc;
    }

#endif /* AVOID_CPIO */

    if (pATI->OptionShadowFB)
    {
        /* Until ShadowFB becomes a true screen wrapper, if it ever does... */

        if (pATI->OptionAccel)
        {
            xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                "Cannot shadow an accelerated frame buffer.\n");
            pATI->OptionShadowFB = FALSE;
        }
        else
        {
            xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
                "Using shadow frame buffer.\n");
        }
    }

    /* 264VT-B's and later have DSP registers */
    if ((pATI->Chip >= ATI_CHIP_264VTB) &&
        !ATIDSPPreInit(pScreenInfo->scrnIndex, pATI))
        goto bail;

    /*
     * Determine minClock and maxClock.  For adapters with supported
     * programmable clock generators, start with an absolute maximum.
     */
    if (pATI->ClockDescriptor.MaxN > 0)
    {
        Numerator = pATI->ClockDescriptor.MaxN * pATI->ReferenceNumerator;
        Denominator = pATI->ClockDescriptor.MinM * pATI->ReferenceDenominator *
            pATI->ClockDescriptor.PostDividers[0];

        /*
         * An integrated PLL behaves as though the reference frequency were
         * doubled.  It also does not appear to care about the colour depth.
         */
        if (pATI->ProgrammableClock == ATI_CLOCK_INTERNAL)
            Numerator <<= 1;

        ATIClockRange.maxClock = (Numerator / (Denominator * 1000)) * 1000;

        Numerator = pATI->ClockDescriptor.MinN * pATI->ReferenceNumerator;
        Denominator = pATI->ClockDescriptor.MaxM * pATI->ReferenceDenominator *
            pATI->ClockDescriptor.PostDividers[pATI->ClockDescriptor.NumD - 1];

        if (pATI->ProgrammableClock == ATI_CLOCK_INTERNAL)
            Numerator <<= 1;

        ATIClockRange.minClock = (Numerator / (Denominator * 1000)) * 1000;

        if (pATI->XCLKFeedbackDivider)
        {
            /* Possibly reduce maxClock due to memory bandwidth */
            Numerator = pATI->XCLKFeedbackDivider * 2 *
                pATI->ReferenceNumerator;
            Denominator = pATI->ClockDescriptor.MinM *
                pATI->XCLKReferenceDivider * pATI->ReferenceDenominator;

            {
                Denominator *= pATI->bitsPerPixel / 4;
            }

            i = (6 - 2) - pATI->XCLKPostDivider;

            i = (ATIDivide(Numerator, Denominator, i, -1) / 1000) * 1000;
            if (i < ATIClockRange.maxClock)
                ATIClockRange.maxClock = i;
        }
    }

    /*
     * Assume an internal DAC can handle whatever frequency the internal PLL
     * can produce (with the reference divider set by BIOS initialisation), but
     * default maxClock to a lower chip-specific default.
     */
    if ((pATI->DAC & ~0x0FU) == ATI_DAC_INTERNAL)
    {
        int DacSpeed;
        switch (pATI->bitsPerPixel)
        {
            case 15:
            case 16:
                DacSpeed = pGDev->dacSpeeds[DAC_BPP16];
                break;

            case 24:
                DacSpeed = pGDev->dacSpeeds[DAC_BPP24];
                break;

            case 32:
                DacSpeed = pGDev->dacSpeeds[DAC_BPP32];
                break;

            default:
                DacSpeed = 0;
                break;
        }
        if (!DacSpeed)
            DacSpeed = pGDev->dacSpeeds[DAC_BPP8];
        if (DacSpeed < ATIClockRange.maxClock)
        {
            DefaultmaxClock = 135000;

            if (pATI->depth > 8)
                DefaultmaxClock = 80000;

            if ((pATI->Chip >= ATI_CHIP_264VTB) &&
                (pATI->Chip != ATI_CHIP_Mach64))
            {
                if ((pATI->Chip >= ATI_CHIP_264VT4) &&
                    (pATI->Chip != ATI_CHIP_264LTPRO))
                    DefaultmaxClock = 230000;
                else if (pATI->Chip >= ATI_CHIP_264VT3)
                    DefaultmaxClock = 200000;
                else
                    DefaultmaxClock = 170000;
            }
            if (DacSpeed > DefaultmaxClock)
                ATIClockRange.maxClock = DacSpeed;
            else if (DefaultmaxClock < ATIClockRange.maxClock)
                ATIClockRange.maxClock = DefaultmaxClock;
        }
    }
    else
    {
        switch(pATI->DAC)
        {
            case ATI_DAC_STG1700:
            case ATI_DAC_STG1702:
            case ATI_DAC_STG1703:
                DefaultmaxClock = 110000;
                break;

            case ATI_DAC_IBMRGB514:
                pATI->maxClock = 220000;
                {
                    DefaultmaxClock = 220000;
                }
                break;

            default:

#ifndef AVOID_CPIO

                if (pATI->CPIO_VGAWonder && (pATI->VideoRAM < 1024))
                {
                    DefaultmaxClock =
                        (GetBits(BIOSByte(0x44U), 0x04U) * 5000) + 40000;
                }
                else

#endif /* AVOID_CPIO */

                {
                    DefaultmaxClock = 80000;
                }

                break;
        }

        if (DefaultmaxClock < ATIClockRange.maxClock)
            ATIClockRange.maxClock = DefaultmaxClock;
    }

    /*
     * Determine available pixel clock frequencies.
     */

    if ((pATI->ProgrammableClock <= ATI_CLOCK_FIXED) ||
        (pATI->ProgrammableClock >= ATI_CLOCK_MAX))
    {
        xf86DrvMsg(pScreenInfo->scrnIndex, X_ERROR,
            "Unsupported or non-programmable clock generator.\n");
        goto bail;
    }

    ATIClockPreInit(pScreenInfo, pATI);
    Strategy = LOOKUP_BEST_REFRESH;

    /*
     * Mode validation.
     */

    if (pATI->Chip >= ATI_CHIP_264CT)
    {
        minPitch = 8;
    }
    else
    {
        minPitch = 16;
    }

    pitchInc = minPitch * pATI->bitsPerPixel;

    pScreenInfo->maxHValue = (MaxBits(CRTC_H_TOTAL) + 1) << 3;

    if (pATI->Chip < ATI_CHIP_264VT)
    {
        /*
         * ATI finally fixed accelerated doublescanning in the 264VT
         * and later.  On 88800's, the bit is documented to exist, but
         * only doubles the vertical timings.  On the 264CT and 264ET,
         * the bit is ignored.
         */
        ATIClockRange.doubleScanAllowed = FALSE;

        /* CRTC_H_TOTAL is one bit narrower */
        pScreenInfo->maxHValue >>= 1;
    }

    pScreenInfo->maxVValue = MaxBits(CRTC_V_TOTAL) + 1;

    maxPitch = minPitch * MaxBits(CRTC_PITCH);

    if (pATI->OptionAccel)
    {
        /*
         * Set engine restrictions on coordinate space.  Use maxPitch for the
         * horizontal and maxHeight for the vertical.
         */
        if (maxPitch > (ATIMach64MaxX / pATI->XModifier))
            maxPitch = ATIMach64MaxX / pATI->XModifier;

        maxHeight = ATIMach64MaxY;

        /*
         * For SGRAM & WRAM adapters, the display engine limits the pitch to
         * multiples of 64 bytes.
         */
        if ((pATI->Chip >= ATI_CHIP_264CT) &&
            ((pATI->Chip >= ATI_CHIP_264VTB) ||
             (pATI->MemoryType >= MEM_264_SGRAM)))
            pitchInc = pATI->XModifier * (64 * 8);
    }

    if (pATI->OptionPanelDisplay && (pATI->LCDPanelID >= 0))
    {
        /*
         * Given LCD modes are more tightly controlled than CRT modes, allow
         * the user the option of not specifying a panel's horizontal sync
         * and/or vertical refresh tolerances.
         */
        Strategy |= LOOKUP_OPTIONAL_TOLERANCES;

        if (ModeType == M_T_BUILTIN)
        {
            /*
             * Add a mode to the end of the monitor's list for the panel's
             * native resolution.
             */
            pMode = (DisplayModePtr)xnfcalloc(1, SizeOf(DisplayModeRec));
            pMode->name = "Native panel mode";
            pMode->type = M_T_BUILTIN;
            pMode->Clock = pATI->LCDClock;
            pMode->HDisplay = pATI->LCDHorizontal;
            pMode->VDisplay = pATI->LCDVertical;

            /*
             * These timings are bogus, but enough to survive sync tolerance
             * checks.
             */
            pMode->HSyncStart = pMode->HDisplay;
            pMode->HSyncEnd = pMode->HSyncStart + minPitch;
            pMode->HTotal = pMode->HSyncEnd + minPitch;
            pMode->VSyncStart = pMode->VDisplay;
            pMode->VSyncEnd = pMode->VSyncStart + 1;
            pMode->VTotal = pMode->VSyncEnd + 1;

            pMode->CrtcHDisplay = pMode->HDisplay;
            pMode->CrtcHBlankStart = pMode->HDisplay;
            pMode->CrtcHSyncStart = pMode->HSyncStart;
            pMode->CrtcHSyncEnd = pMode->HSyncEnd;
            pMode->CrtcHBlankEnd = pMode->HTotal;
            pMode->CrtcHTotal = pMode->HTotal;

            pMode->CrtcVDisplay = pMode->VDisplay;
            pMode->CrtcVBlankStart = pMode->VDisplay;
            pMode->CrtcVSyncStart = pMode->VSyncStart;
            pMode->CrtcVSyncEnd = pMode->VSyncEnd;
            pMode->CrtcVBlankEnd = pMode->VTotal;
            pMode->CrtcVTotal = pMode->VTotal;

            if (!pScreenInfo->monitor->Modes)
            {
                pScreenInfo->monitor->Modes = pMode;
            }
            else
            {
                pScreenInfo->monitor->Last->next = pMode;
                pMode->prev = pScreenInfo->monitor->Last;
            }

            pScreenInfo->monitor->Last = pMode;
        }

        /*
         * Defeat Xconfigurator brain damage.  Ignore all HorizSync and
         * VertRefresh specifications.  For now, this does not take
         * SYNC_TOLERANCE into account.
         */
        if (pScreenInfo->monitor->nHsync > 0)
        {
            double hsync = (double)pATI->LCDClock /
                           (pATI->LCDHorizontal + pATI->LCDHBlankWidth);

            for (i = 0;  ;  i++)
            {
                if (i >= pScreenInfo->monitor->nHsync)
                {
                    xf86DrvMsg(pScreenInfo->scrnIndex, X_NOTICE,
                        "Conflicting XF86Config HorizSync specification(s)"
                        " ignored.\n");
                    break;
                }

                if ((hsync >= pScreenInfo->monitor->hsync[i].lo) &&
                    (hsync <= pScreenInfo->monitor->hsync[i].hi))
                {
                    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                        "Extraneous XF86Config HorizSync specification(s)"
                        " ignored.\n");
                    break;
                }
            }

            pScreenInfo->monitor->nHsync = 0;
        }

        if (pScreenInfo->monitor->nVrefresh > 0)
        {
            double vrefresh = ((double)pATI->LCDClock * 1000.0) /
                              ((pATI->LCDHorizontal + pATI->LCDHBlankWidth) *
                               (pATI->LCDVertical + pATI->LCDVBlankWidth));

            for (i = 0;  ;  i++)
            {
                if (i >= pScreenInfo->monitor->nVrefresh)
                {
                    xf86DrvMsg(pScreenInfo->scrnIndex, X_NOTICE,
                        "Conflicting XF86Config VertRefresh specification(s)"
                        " ignored.\n");
                    break;
                }

                if ((vrefresh >= pScreenInfo->monitor->vrefresh[i].lo) &&
                    (vrefresh <= pScreenInfo->monitor->vrefresh[i].hi))
                {
                    xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                        "Extraneous XF86Config VertRefresh specification(s)"
                        " ignored.\n");
                    break;
                }
            }

            pScreenInfo->monitor->nVrefresh = 0;
        }
    }

    i = xf86ValidateModes(pScreenInfo,
            pScreenInfo->monitor->Modes, pScreenInfo->display->modes,
            &ATIClockRange, NULL, minPitch, maxPitch,
            pitchInc, 0, maxHeight,
            pScreenInfo->display->virtualX, pScreenInfo->display->virtualY,
            ApertureSize, Strategy);
    if (i <= 0)
        goto bail;

    /* Remove invalid modes */
    xf86PruneDriverModes(pScreenInfo);

    /* Set current mode to the first in the list */
    pScreenInfo->currentMode = pScreenInfo->modes;

    /* Print mode list */
    xf86PrintModes(pScreenInfo);

    /* Set display resolution */
    xf86SetDpi(pScreenInfo, 0, 0);

    /* Load required modules */
    if (!ATILoadModules(pScreenInfo, pATI))
        goto bail;

    pATI->displayWidth = pScreenInfo->displayWidth;

    /* Initialise for panning */
    ATIAdjustPreInit(pATI);

    /*
     * Warn about modes that are too small, or not aligned, to scroll to the
     * bottom right corner of the virtual screen.
     */
    MinX = pScreenInfo->virtualX - pATI->AdjustMaxX;
    MinY = pScreenInfo->virtualY - pATI->AdjustMaxY;

    pMode = pScreenInfo->modes;
    do
    {
        if ((pMode->VDisplay <= MinY) &&
            ((pMode->VDisplay < MinY) || (pMode->HDisplay < MinX)))
            xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                "Mode \"%s\" too small to scroll to bottom right corner of"
                " virtual resolution.\n", pMode->name);
        else if ((pMode->HDisplay & ~pATI->AdjustMask) / pScreenInfo->xInc)
            xf86DrvMsg(pScreenInfo->scrnIndex, X_WARNING,
                "Mode \"%s\" cannot scroll to bottom right corner of virtual"
                " resolution.\n Horizontal dimension not a multiple of %ld.\n",
                pMode->name, ~pATI->AdjustMask + 1);
    } while ((pMode = pMode->next) != pScreenInfo->modes);

    /* Initialise XVideo extension support */
    ATIXVPreInit(pATI);

    /* Initialise CRTC code */
    ATIModePreInit(pScreenInfo, pATI, &pATI->NewHW);

    /* Set up for I2C */
    ATII2CPreInit(pScreenInfo, pATI);

    if (!pScreenInfo->chipset || !*pScreenInfo->chipset)
        pScreenInfo->chipset = "mach64";

    PreInitSuccess = TRUE;

bail:
    ATILock(pATI);

bail_locked:
    ATIPrintNoiseIfRequested(pATI, BIOS, BIOSSize);
    ATIUnmapApertures(pScreenInfo->scrnIndex, pATI);

    return PreInitSuccess;
}
