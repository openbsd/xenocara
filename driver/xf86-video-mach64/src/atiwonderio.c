/*
 * Copyright 2000 through 2004 by Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
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
#include "atichip.h"
#include "atiwonderio.h"

#ifndef AVOID_CPIO

/*
 * ATIModifyExtReg --
 *
 * This function is called to modify certain bits in an ATI extended VGA
 * register while preserving its other bits.  The function will not write the
 * register if it turns out its value would not change.  This helps prevent
 * server hangs on older adapters.
 */
void
ATIModifyExtReg
(
    ATIPtr      pATI,
    const CARD8 Index,
    int         CurrentValue,
    const CARD8 CurrentMask,
    CARD8       NewValue
)
{
    /* Possibly retrieve the current value */
    if (CurrentValue < 0)
        CurrentValue = ATIGetExtReg(Index);

    /* Compute new value */
    NewValue &= (CARD8)(~CurrentMask);
    NewValue |= CurrentValue & CurrentMask;

    /* Check if value will be changed */
    if (CurrentValue == NewValue)
        return;

        ATIPutExtReg(Index, NewValue);
}

#endif /* AVOID_CPIO */
