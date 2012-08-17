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
#include "atichip.h"
#include "atilock.h"
#include "atimach64io.h"
#include "atiwonderio.h"

#include "atiadjust.h"

/*
 * The display start address is expressed in units of 32-bit (VGA) or 64-bit
 * (accelerator) words where all planar modes are considered as 4bpp modes.
 * These functions ensure the start address does not exceed architectural
 * limits.  Also, to avoid colour changes while panning, these 32-bit or 64-bit
 * boundaries may not fall within a pixel.
 */

/*
 * ATIAjustPreInit --
 *
 * This function calculates values needed to speed up the setting of the
 * display start address.
 */
void
ATIAdjustPreInit
(
    ATIPtr      pATI
)
{
    unsigned long MaxBase;

    {
        pATI->AdjustDepth = (pATI->bitsPerPixel + 7) >> 3;

        pATI->AdjustMask = 64;
        while (pATI->AdjustMask % (unsigned long)(pATI->AdjustDepth))
            pATI->AdjustMask += 64;
        pATI->AdjustMask =
            ~(((pATI->AdjustMask / (unsigned long)(pATI->AdjustDepth)) >> 3) -
              1);
    }

    {
            pATI->AdjustMaxBase = MaxBits(CRTC_OFFSET) << 3;
    }

    MaxBase = (pATI->AdjustMaxBase / (unsigned long)pATI->AdjustDepth) |
        ~pATI->AdjustMask;

    pATI->AdjustMaxX = MaxBase % pATI->displayWidth;
    pATI->AdjustMaxY = MaxBase / pATI->displayWidth;
}

/*
 * ATIAdjustFrame --
 *
 * This function is used to initialise the SVGA Start Address - the first
 * displayed location in video memory.  This is used to implement the virtual
 * window.
 */
void
ATIAdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    ATIPtr      pATI = ATIPTR(pScreenInfo);
    int         Base, xy;

    /*
     * Assume the caller has already done its homework in ensuring the physical
     * screen is still contained in the virtual resolution.
     */
    if (y >= pATI->AdjustMaxY)
    {
        y = pATI->AdjustMaxY;
        if (x > pATI->AdjustMaxX)
            y--;
    }

    Base = ((((y * pATI->displayWidth) + x) & pATI->AdjustMask) *
            pATI->AdjustDepth) >> 3;

    if (!pATI->currentMode)
    {
        /*
         * Not in DGA.  This reverse-calculates pScreenInfo->frame[XY][01] so
         * that the cursor does not move on mode switches.
         */
        xy = (Base << 3) / pATI->AdjustDepth;
        pScreenInfo->frameX0 = xy % pATI->displayWidth;
        pScreenInfo->frameY0 = xy / pATI->displayWidth;
        pScreenInfo->frameX1 =
            pScreenInfo->frameX0 + pScreenInfo->currentMode->HDisplay - 1;
        pScreenInfo->frameY1 =
            pScreenInfo->frameY0 + pScreenInfo->currentMode->VDisplay - 1;
    }

    /* Unlock registers */
    ATIUnlock(pATI);

    {
        outr(CRTC_OFF_PITCH, SetBits(pATI->displayWidth >> 3, CRTC_PITCH) |
            SetBits(Base, CRTC_OFFSET));
    }
}
