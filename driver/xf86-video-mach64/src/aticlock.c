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

/*
 * For all supported programmable clock generators, the driver will ignore any
 * XF86Config clock line and programme, as needed, the clock number reserved by
 * the BIOS for accelerated drivers.  The driver's mode initialisation routine
 * finds integers N, M and D such that
 *
 *             N
 *      R * -------  MHz
 *           M * D
 *
 * best approximates the mode's clock frequency, where R is the crystal-
 * generated reference frequency (usually 14.318 MHz).  D is a power of 2
 * except for those integrated controllers that also offer odd dividers.
 * Different clock generators have different restrictions on the value N, M and
 * D can assume.  The driver contains an internal table to record these
 * restrictions (among other things).  The resulting values of N, M and D are
 * then encoded in a generator-specific way and used to programme the clock.
 * The Mach64's clock divider is not used in this case.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include "ati.h"
#include "atichip.h"
#include "atidac.h"
#include "atidsp.h"
#include "atimach64io.h"
#include "atimode.h"
#include "atiwonderio.h"

/*
 * Definitions related to programmable clock generators.
 */
static CARD16 ATIPostDividers[] = {1, 2, 4, 8, 16, 32, 64, 128},
              ATI264xTPostDividers[] = {1, 2, 4, 8, 3, 0, 6, 12};
ClockRec ATIClockDescriptors[] =
{
    {
          0,   0,   0, 1, 1,
          1,   1,   0,
          0, NULL,
        "Non-programmable"
    },
    {
        257, 512, 257, 1, 1,
         46,  46,   0,
          4, ATIPostDividers,
        "ATI 18818 or ICS 2595 or similar"
    },
    {
          2, 129,   2, 1, 1,
          8,  14,   2,
          8, ATIPostDividers,
        "SGS-Thompson 1703 or similar"
    },
    {
         16, 263,   8, 8, 9,
          4,  12,   2,
          4, ATIPostDividers,
        "Chrontel 8398 or similar"
    },
    {
          2, 255,   0, 1, 1,
         45,  45,   0,
          4, ATI264xTPostDividers,
        "Internal"
    },
    {
          2, 257,   2, 1, 1,
          2,  32,   2,
          4, ATIPostDividers,
        "AT&T 20C408 or similar"
    },
    {
         65, 128,  65, 1, 1,
          2,  14,   0,
          4, ATIPostDividers,
        "IBM RGB 514 or similar"
    }
};

/*
 * ATIClockPreInit --
 *
 * This function is called by ATIPreInit() and handles the XF86Config clocks
 * line (or lack thereof).
 */
void
ATIClockPreInit
(
    ScrnInfoPtr   pScreenInfo,
    ATIPtr        pATI
)
{
            /*
             * Recognise supported clock generators.  This involves telling the
             * rest of the server about it and (re-)initializing the XF86Config
             * clocks line.
             */
            pScreenInfo->progClock = TRUE;

            xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
                "%s programmable clock generator detected.\n",
                pATI->ClockDescriptor.ClockName);
            if (pATI->ReferenceDenominator == 1)
                xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
                    "Reference clock %.3f MHz.\n",
                    (double)pATI->ReferenceNumerator / 1000.0);
            else
                xf86DrvMsg(pScreenInfo->scrnIndex, X_PROBED,
                    "Reference clock %.6g/%d (%.3f) MHz.\n",
                    (double)pATI->ReferenceNumerator / 1000.0,
                    pATI->ReferenceDenominator,
                    (double)pATI->ReferenceNumerator /
                        ((double)pATI->ReferenceDenominator * 1000.0));

#if defined(__sparc__)
            if ((pATI->refclk / 100000) != 286 &&
                (pATI->refclk / 100000) != 295)
            {
                xf86DrvMsg(pScreenInfo->scrnIndex, X_INFO,
                    "If modes do not work on Ultra 5/10 or Blade 100/150,\n"
                    "\tset option \"reference_clock\" to \"28.636 MHz\""
                    " or \"29.5 MHz\"\n");
            }
#endif

            if (pATI->ProgrammableClock == ATI_CLOCK_CH8398)
            {   /* First two are fixed */
                pScreenInfo->numClocks = 2;
                pScreenInfo->clock[0] = 25175;
                pScreenInfo->clock[1] = 28322;
            }
            else if (pATI->ProgrammableClock == ATI_CLOCK_INTERNAL)
            {
                /*
                 * The integrated PLL generates clocks as if the reference
                 * frequency were doubled.
                 */
                pATI->ReferenceNumerator <<= 1;
            }
}

/*
 * ATIClockCalculate --
 *
 * This function is called to generate, if necessary, the data needed for clock
 * programming, and set clock select bits in various register values.
 */
Bool
ATIClockCalculate
(
    int            iScreen,
    ATIPtr         pATI,
    ATIHWPtr       pATIHW,
    DisplayModePtr pMode
)
{
    int N, M, D;
    int ClockSelect, N1, MinimumGap;
    int Frequency, Multiple;            /* Used as temporaries */

    /* Set default values */
    pATIHW->FeedbackDivider = pATIHW->ReferenceDivider = pATIHW->PostDivider = 0;

    if (((pATI->ProgrammableClock == ATI_CLOCK_CH8398) &&
         (pMode->ClockIndex < 2)))
    {
        xf86DrvMsg(iScreen, X_ERROR,
            "First two clocks of Chrontel 8398 clock generator are fixed\n");
        return FALSE;
    }

    {
        /* Generate clock programme word, using units of kHz */
        MinimumGap = ((unsigned int)(-1)) >> 1;

        /* Loop through reference dividers */
        for (M = pATI->ClockDescriptor.MinM;
             M <= pATI->ClockDescriptor.MaxM;
             M++)
        {
            /* Loop through post-dividers */
            for (D = 0;  D < pATI->ClockDescriptor.NumD;  D++)
            {
                if (!pATI->ClockDescriptor.PostDividers[D])
                    continue;

                /* Limit undivided VCO to maxClock */
                if (pATI->maxClock &&
                    ((pATI->maxClock / pATI->ClockDescriptor.PostDividers[D]) <
                     pMode->Clock))
                    continue;

                /*
                 * Calculate closest feedback divider and apply its
                 * restrictions.
                 */
                Multiple = M * pATI->ReferenceDenominator *
                    pATI->ClockDescriptor.PostDividers[D];
                N = ATIDivide(pMode->Clock * Multiple,
                    pATI->ReferenceNumerator, 0, 0);
                if (N < pATI->ClockDescriptor.MinN)
                    N = pATI->ClockDescriptor.MinN;
                else if (N > pATI->ClockDescriptor.MaxN)
                    N = pATI->ClockDescriptor.MaxN;
                N -= pATI->ClockDescriptor.NAdjust;
                N1 = (N / pATI->ClockDescriptor.N1) * pATI->ClockDescriptor.N2;
                if (N > N1)
                    N = ATIDivide(N1 + 1, pATI->ClockDescriptor.N1, 0, 1);
                N += pATI->ClockDescriptor.NAdjust;
                N1 += pATI->ClockDescriptor.NAdjust;

                for (;  ;  N = N1)
                {
                    /* Pick the closest setting */
                    Frequency = abs(ATIDivide(N * pATI->ReferenceNumerator,
                        Multiple, 0, 0) - pMode->Clock);
                    if ((Frequency < MinimumGap) ||
                        ((Frequency == MinimumGap) &&
                         (pATIHW->FeedbackDivider < N)))
                    {
                        /* Save settings */
                        pATIHW->FeedbackDivider = N;
                        pATIHW->ReferenceDivider = M;
                        pATIHW->PostDivider = D;
                        MinimumGap = Frequency;
                    }

                    if (N <= N1)
                        break;
                }
            }
        }

        Multiple = pATIHW->ReferenceDivider * pATI->ReferenceDenominator *
            pATI->ClockDescriptor.PostDividers[pATIHW->PostDivider];
        Frequency = pATIHW->FeedbackDivider * pATI->ReferenceNumerator;
        Frequency = ATIDivide(Frequency, Multiple, 0, 0);
        if (abs(Frequency - pMode->Clock) > CLOCK_TOLERANCE)
        {
            xf86DrvMsg(iScreen, X_ERROR,
                "Unable to programme clock %.3fMHz for mode %s.\n",
                (double)(pMode->Clock) / 1000.0, pMode->name);
            return FALSE;
        }
        pMode->SynthClock = Frequency;
        ClockSelect = pATI->ClockNumberToProgramme;

        xf86ErrorFVerb(4,
            "\n Programming clock %d to %.3fMHz for mode %s."
            "  N=%d, M=%d, D=%d.\n",
            ClockSelect, (double)Frequency / 1000.0, pMode->name,
            pATIHW->FeedbackDivider, pATIHW->ReferenceDivider,
            pATIHW->PostDivider);

        if (pATI->Chip >= ATI_CHIP_264VTB)
            ATIDSPCalculate(pATI, pATIHW, pMode);
    }

    /* Set clock select bits */
    pATIHW->clock = ClockSelect;

    {
            pATIHW->clock_cntl = CLOCK_STROBE |
                SetBits(ClockSelect, CLOCK_SELECT | CLOCK_DIVIDER);
    }

    return TRUE;
}

/*
 * ATIClockSet --
 *
 * This function is called to programme a clock for the mode being set.
 */
void
ATIClockSet
(
    ATIPtr      pATI,
    ATIHWPtr    pATIHW
)
{
    CARD32 crtc_gen_cntl, tmp;
    CARD8 clock_cntl0;
    CARD8 tmp2;
    unsigned int Programme;
    int N = pATIHW->FeedbackDivider - pATI->ClockDescriptor.NAdjust;
    int M = pATIHW->ReferenceDivider - pATI->ClockDescriptor.MAdjust;
    int D = pATIHW->PostDivider;

    /* Temporarily switch to accelerator mode */
    crtc_gen_cntl = inr(CRTC_GEN_CNTL);
    if (!(crtc_gen_cntl & CRTC_EXT_DISP_EN))
        outr(CRTC_GEN_CNTL, crtc_gen_cntl | CRTC_EXT_DISP_EN);

    switch (pATI->ProgrammableClock)
    {
        case ATI_CLOCK_ICS2595:
            clock_cntl0 = in8(CLOCK_CNTL);

            Programme = (SetBits(pATIHW->clock, ICS2595_CLOCK) |
                SetBits(N, ICS2595_FB_DIV) | SetBits(D, ICS2595_POST_DIV)) ^
                ICS2595_TOGGLE;

            ATIDelay(50000);            /* 50 milliseconds */

            /* Send all 20 bits of programme word */
            while (Programme >= CLOCK_BIT)
            {
                tmp = (Programme & CLOCK_BIT) | CLOCK_STROBE;
                out8(CLOCK_CNTL, tmp);
                ATIDelay(26);           /* 26 microseconds */
                out8(CLOCK_CNTL, tmp | CLOCK_PULSE);
                ATIDelay(26);           /* 26 microseconds */
                Programme >>= 1;
            }

            /* Restore register */
            out8(CLOCK_CNTL, clock_cntl0 | CLOCK_STROBE);
            break;

        case ATI_CLOCK_STG1703:
            (void)ATIGetDACCmdReg(pATI);
            (void)in8(M64_DAC_MASK);
            out8(M64_DAC_MASK, (pATIHW->clock << 1) + 0x20U);
            out8(M64_DAC_MASK, 0);
            out8(M64_DAC_MASK, SetBits(N, 0xFFU));
            out8(M64_DAC_MASK, SetBits(M, 0x1FU) | SetBits(D, 0xE0U));
            break;

        case ATI_CLOCK_CH8398:
            tmp = inr(DAC_CNTL) | (DAC_EXT_SEL_RS2 | DAC_EXT_SEL_RS3);
            outr(DAC_CNTL, tmp);
            out8(M64_DAC_WRITE, pATIHW->clock);
            out8(M64_DAC_DATA, SetBits(N, 0xFFU));
            out8(M64_DAC_DATA, SetBits(M, 0x3FU) | SetBits(D, 0xC0U));
            out8(M64_DAC_MASK, 0x04U);
            outr(DAC_CNTL, tmp & ~(DAC_EXT_SEL_RS2 | DAC_EXT_SEL_RS3));
            tmp2 = in8(M64_DAC_WRITE);
            out8(M64_DAC_WRITE, (tmp2 & 0x70U) | 0x80U);
            outr(DAC_CNTL, tmp & ~DAC_EXT_SEL_RS2);
            break;

        case ATI_CLOCK_INTERNAL:
            /* Reset VCLK generator */
            ATIMach64PutPLLReg(PLL_VCLK_CNTL, pATIHW->pll_vclk_cntl);

            /* Set post-divider */
            tmp2 = pATIHW->clock << 1;
            tmp = ATIMach64GetPLLReg(PLL_VCLK_POST_DIV);
            tmp &= ~(0x03U << tmp2);
            tmp |= SetBits(D, 0x03U) << tmp2;
            ATIMach64PutPLLReg(PLL_VCLK_POST_DIV, tmp);

            /* Set extended post-divider */
            tmp = ATIMach64GetPLLReg(PLL_XCLK_CNTL);
            tmp &= ~(SetBits(1, PLL_VCLK0_XDIV) << pATIHW->clock);
            tmp |= SetBits(D >> 2, PLL_VCLK0_XDIV) << pATIHW->clock;
            ATIMach64PutPLLReg(PLL_XCLK_CNTL, tmp);

            /* Set feedback divider */
            tmp = PLL_VCLK0_FB_DIV + pATIHW->clock;
            ATIMach64PutPLLReg(tmp, SetBits(N, 0xFFU));

            /* End VCLK generator reset */
            ATIMach64PutPLLReg(PLL_VCLK_CNTL,
                pATIHW->pll_vclk_cntl & ~PLL_VCLK_RESET);

            /* Reset write bit */
            ATIMach64AccessPLLReg(pATI, 0, FALSE);
            break;

        case ATI_CLOCK_ATT20C408:
            (void)ATIGetDACCmdReg(pATI);
            tmp = in8(M64_DAC_MASK);
            (void)ATIGetDACCmdReg(pATI);
            out8(M64_DAC_MASK, tmp | 1);
            out8(M64_DAC_WRITE, 1);
            out8(M64_DAC_MASK, tmp | 9);
            ATIDelay(400);              /* 400 microseconds */
            tmp2 = (pATIHW->clock << 2) + 0x40U;
            out8(M64_DAC_WRITE, tmp2);
            out8(M64_DAC_MASK, SetBits(N, 0xFFU));
            out8(M64_DAC_WRITE, ++tmp2);
            out8(M64_DAC_MASK, SetBits(M, 0x3FU) | SetBits(D, 0xC0U));
            out8(M64_DAC_WRITE, ++tmp2);
            out8(M64_DAC_MASK, 0x77U);
            ATIDelay(400);              /* 400 microseconds */
            out8(M64_DAC_WRITE, 1);
            out8(M64_DAC_MASK, tmp);
            break;

        case ATI_CLOCK_IBMRGB514:
            /*
             * Here, only update in-core data.  It will be written out later by
             * ATIRGB514Set().
             */
            tmp = (pATIHW->clock << 1) + 0x20U;
            pATIHW->ibmrgb514[tmp] =
                (SetBits(N, 0x3FU) | SetBits(D, 0xC0U)) ^ 0xC0U;
            pATIHW->ibmrgb514[tmp + 1] = SetBits(M, 0x3FU);
            break;

        default:
            break;
    }

    (void)in8(M64_DAC_WRITE);    /* Clear DAC counter */

    /* Restore register */
    if (!(crtc_gen_cntl & CRTC_EXT_DISP_EN))
        outr(CRTC_GEN_CNTL, crtc_gen_cntl);
}
