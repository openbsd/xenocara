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
#include "atibank.h"
#include "atimach64io.h"
#include "atiwonderio.h"

#ifndef AVOID_CPIO

/*
 * ATIx8800SetBank --
 *
 * Set an ATI 28800's, 68800's or 88800's read and write bank numbers.
 */
void
ATIx8800SetBank
(
    ATIPtr       pATI,
    unsigned int iBank
)
{
    (void)iBank; /* always called with iBank = 0 */

    ATIPutExtReg(0xB2U, 0x00U);
    ATIModifyExtReg(pATI, 0xAEU, -1, (CARD8)(~0x0FU), 0x00U);
}

/*
 * Functions to simulate a banked VGA aperture using a Mach64's small dual
 * paged apertures.  There are two sets of these:  one for packed modes, the
 * other for planar modes.
 */

static CARD32
ATIMach64MassagePackedBankNumber
(
    CARD8 iBank
)
{
    iBank <<= 1;
    return ((iBank + 1) << 16) | iBank;
}

/*
 * ATIMach64SetBankPacked --
 *
 * Set read and write bank numbers for small dual paged apertures.
 */
void
ATIMach64SetBankPacked
(
    ATIPtr       pATI,
    unsigned int iBank
)
{
    CARD32 tmp = ATIMach64MassagePackedBankNumber(iBank);

    outr(MEM_VGA_RP_SEL, tmp);
    outr(MEM_VGA_WP_SEL, tmp);
}

static CARD32
ATIMach64MassagePlanarBankNumber
(
    CARD8 iBank
)
{
    iBank <<= 3;
    return ((iBank + 4) << 16) | iBank;
}

/*
 * ATIMach64SetBankPlanar --
 *
 * Set read and write bank numbers for small dual paged apertures.
 */
void
ATIMach64SetBankPlanar
(
    ATIPtr       pATI,
    unsigned int iBank
)
{
    CARD32 tmp = ATIMach64MassagePlanarBankNumber(iBank);

    outr(MEM_VGA_RP_SEL, tmp);
    outr(MEM_VGA_WP_SEL, tmp);
}

#endif /* AVOID_CPIO */
