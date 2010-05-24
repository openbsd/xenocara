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

#include "ati.h"
#include "atibus.h"
#include "atichip.h"
#include "atimach64io.h"
#include "atistruct.h"

/*
 * Definitions related to an adapter's system bus interface.
 */

const char *ATIBusNames[] =
{
    "PCI",
    "AGP"
};

/*
 * ATIClaimResources --
 *
 * This function registers most of the bus resources used by an adapter.  The
 * exceptions are PCI-configured resources and non-PCI non-AGP linear
 * apertures, both of which are registered by ATIPreInit().  This function also
 * attempts to register unshareable resources for inactive PCI adapters,
 * whether or not they are relocatable.
 */
static void
ATIClaimResources
(
    ATIPtr pATI,
    Bool   Active
)
{

#ifndef XSERVER_LIBPCIACCESS
#ifndef AVOID_CPIO

    resRange Resources[2] = {{0, 0, 0}, _END};

    /* Claim VGA and VGAWonder resources */
    if ((pATI->VGAAdapter) && (Active))
    {
        /*
         * 18800-x's are the only ATI controllers that decode all ISA aliases
         * of VGA and VGA Wonder I/O ports.  Other x8800's do not decode >any<
         * VGA aliases, but do decode VGA Wonder aliases whose most significant
         * nibble is zero.
         */
        xf86ClaimFixedResources(resVgaShared, pATI->iEntity);

        if (pATI->CPIO_VGAWonder)
        {
            Resources[0].type = ResShrIoSparse | ResBus;
            Resources[0].rBase = pATI->CPIO_VGAWonder;
            Resources[0].rMask = 0xF3FEU;

            xf86ClaimFixedResources(Resources, pATI->iEntity);

            (void)memcpy(pATI->VGAWonderResources,
                Resources, SizeOf(Resources));
        }
    }

    if (!Active)
        return;

    /* Claim Mach64 sparse I/O resources */
    if ((pATI->CPIODecoding == SPARSE_IO))
    {
        Resources[0].type = ResShrIoSparse | ResBus;
        Resources[0].rBase = pATI->CPIOBase;
        Resources[0].rMask = 0x03FCU;

        xf86ClaimFixedResources(Resources, pATI->iEntity);
    }

#endif /* AVOID_CPIO */
#endif
}

/*
 * ATIClaimBusSlot --
 *
 * Claim an adapter and register its resources.
 */
int
ATIClaimBusSlot
(
    Bool      Active,
    ATIPtr    pATI
)
{
    if (pATI->iEntity >= 0)
        ATIClaimResources(pATI, Active);

    return pATI->iEntity;
}
