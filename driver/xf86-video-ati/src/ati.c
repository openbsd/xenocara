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

/*************************************************************************/

/*
 * Author:  Marc Aurele La France (TSI @ UQV), tsi@xfree86.org
 *
 * This is the ATI driver for XFree86.
 *
 * John Donne once said "No man is an island", and I am most certainly not an
 * exception.  Contributions, intentional or not, to this and previous versions
 * of this driver by the following are hereby acknowledged:
 *
 * Thomas Roell, Per Lindqvist, Doug Evans, Rik Faith, Arthur Tateishi,
 * Alain Hebert, Ton van Rosmalen, David Chambers, William Shubert,
 * ATI Technologies Incorporated, Robert Wolff, David Dawes, Mark Weaver,
 * Hans Nasten, Kevin Martin, Frederic Rienthaler, Marc Bolduc, Reuben Sumner,
 * Benjamin T. Yang, James Fast Kane, Randall Hopper, W. Marcus Miller,
 * Henrik Harmsen, Christian Lupien, Precision Insight Incorporated,
 * Mark Vojkovich, Huw D M Davies, Andrew C Aitchison, Ani Joshi,
 * Kostas Gewrgiou, Jakub Jelinek, David S. Miller, A E Lawrence,
 * Linus Torvalds, William Blew, Ignacio Garcia Etxebarria, Patrick Chase,
 * Vladimir Dergachev, Egbert Eich, Mike A. Harris
 *
 * ... and, many, many others from around the world.
 *
 * In addition, this work would not have been possible without the active
 * support, both moral and otherwise, of the staff and management of Computing
 * and Network Services at the University of Alberta, in Edmonton, Alberta,
 * Canada.
 *
 * The driver is intended to support all ATI adapters since their VGA Wonder
 * V3, including OEM counterparts.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <pciaccess.h>
#include "atipcirename.h"

#include "ati.h"
#include "atipciids.h"
#include "ativersion.h"

/* names duplicated from version headers */
#define MACH64_DRIVER_NAME  "mach64"
#define R128_DRIVER_NAME    "r128"
#define RADEON_DRIVER_NAME  "radeon"

enum
{
    ATI_CHIP_FAMILY_NONE = 0,
    ATI_CHIP_FAMILY_Mach64,
    ATI_CHIP_FAMILY_Rage128,
    ATI_CHIP_FAMILY_Radeon
};

static int ATIChipID(const uint16_t);

/* domain defines (stolen from xserver) */
#if (defined(__alpha__) || defined(__ia64__)) && defined (linux)
# define PCI_DOM_MASK 0x01fful
#else
# define PCI_DOM_MASK 0x0ffu
#endif

#define PCI_DOM_FROM_BUS(bus)  (((bus) >> 8) & (PCI_DOM_MASK))
#define PCI_BUS_NO_DOMAIN(bus) ((bus) & 0xffu)

static struct pci_device*
ati_device_get_from_busid(int bus, int dev, int func)
{
    return pci_device_find_by_slot(PCI_DOM_FROM_BUS(bus),
                                   PCI_BUS_NO_DOMAIN(bus),
                                   dev,
                                   func);
}

#ifndef XSERVER_PLATFORM_BUS
static struct pci_device*
ati_device_get_primary(void)
{
    struct pci_device *device = NULL;
    struct pci_device_iterator *device_iter;

    device_iter = pci_slot_match_iterator_create(NULL);

    while ((device = pci_device_next(device_iter)) != NULL) {
        if (xf86IsPrimaryPci(device))
            break;
    }

    pci_iterator_destroy(device_iter);

    return device;
}
#else
static struct pci_device *
ati_device_get_indexed(int index)
{
    struct pci_device *device = NULL;
    struct pci_device_iterator *device_iter;
    int count = 0;

    device_iter = pci_slot_match_iterator_create(NULL);

    while ((device = pci_device_next(device_iter)) != NULL) {
        if (device->vendor_id == PCI_VENDOR_ATI) {
            if (count == index)
                return device;
            count++;
        }
    }
    return NULL;
}
#endif

void
ati_gdev_subdriver(pointer options)
{
    int      nATIGDev, nMach64GDev, nR128GDev, nRadeonGDev;
    GDevPtr *ATIGDevs;
    Bool     load_mach64 = FALSE, load_r128 = FALSE, load_radeon = FALSE;
    int      i;

    /* let the subdrivers configure for themselves */
    if (xf86ServerIsOnlyDetecting())
        return;

    /* get Device sections with Driver "ati" */
    nATIGDev = xf86MatchDevice(ATI_DRIVER_NAME, &ATIGDevs);
    nMach64GDev = xf86MatchDevice(MACH64_DRIVER_NAME, NULL);
    nR128GDev = xf86MatchDevice(R128_DRIVER_NAME, NULL);
    nRadeonGDev = xf86MatchDevice(RADEON_DRIVER_NAME, NULL);

    for (i = 0; i < nATIGDev; i++) {
        GDevPtr     ati_gdev = ATIGDevs[i];
        pciVideoPtr device = NULL;
        int         chip_family;

        /* get pci device for the Device section */
        if (ati_gdev->busID) {
            int bus, dev, func;

            if (!xf86ParsePciBusString(ati_gdev->busID, &bus, &dev, &func))
                continue;

            device = ati_device_get_from_busid(bus, dev, func);
        }
#ifdef XSERVER_PLATFORM_BUS
        else
            device = ati_device_get_indexed(i);
#else
        else {
            device = ati_device_get_primary();
        }
#endif

        if (!device)
            continue;

        /* check for non-ati devices and prehistoric mach32 */
        if ((PCI_DEV_VENDOR_ID(device) != PCI_VENDOR_ATI) ||
            (PCI_DEV_DEVICE_ID(device) == PCI_CHIP_MACH32))
            continue;

        /* replace Driver line in the Device section */
        chip_family = ATIChipID(PCI_DEV_DEVICE_ID(device));

        if (chip_family == ATI_CHIP_FAMILY_Mach64) {
            ati_gdev->driver = MACH64_DRIVER_NAME;
            load_mach64 = TRUE;
        }

        if (chip_family == ATI_CHIP_FAMILY_Rage128) {
            ati_gdev->driver = R128_DRIVER_NAME;
            load_r128 = TRUE;
        }

        if (chip_family == ATI_CHIP_FAMILY_Radeon) {
            ati_gdev->driver = RADEON_DRIVER_NAME;
            load_radeon = TRUE;
        }
    }

    free(ATIGDevs);

    /* load subdrivers as primary modules and only if they do not get loaded
     * from other device sections
     */

    if (load_mach64 && (nMach64GDev == 0))
         xf86LoadOneModule(MACH64_DRIVER_NAME, options);

    if (load_r128 && (nR128GDev == 0))
         xf86LoadOneModule(R128_DRIVER_NAME, options);

    if (load_radeon && (nRadeonGDev == 0))
         xf86LoadOneModule(RADEON_DRIVER_NAME, options);
}

/*
 * ATIChipID --
 *
 * This returns the ATI_CHIP_FAMILY_* value associated with a particular ChipID.
 */
static int
ATIChipID(const uint16_t ChipID)
{
    switch (ChipID)
    {
        case PCI_CHIP_MACH64GX:
        case PCI_CHIP_MACH64CX:
        case PCI_CHIP_MACH64CT:
        case PCI_CHIP_MACH64ET:
        case PCI_CHIP_MACH64VT:
        case PCI_CHIP_MACH64GT:
        case PCI_CHIP_MACH64VU:
        case PCI_CHIP_MACH64GU:
        case PCI_CHIP_MACH64LG:
        case PCI_CHIP_MACH64VV:
        case PCI_CHIP_MACH64GV:
        case PCI_CHIP_MACH64GW:
        case PCI_CHIP_MACH64GY:
        case PCI_CHIP_MACH64GZ:
        case PCI_CHIP_MACH64GB:
        case PCI_CHIP_MACH64GD:
        case PCI_CHIP_MACH64GI:
        case PCI_CHIP_MACH64GP:
        case PCI_CHIP_MACH64GQ:
        case PCI_CHIP_MACH64LB:
        case PCI_CHIP_MACH64LD:
        case PCI_CHIP_MACH64LI:
        case PCI_CHIP_MACH64LP:
        case PCI_CHIP_MACH64LQ:
        case PCI_CHIP_MACH64GL:
        case PCI_CHIP_MACH64GM:
        case PCI_CHIP_MACH64GN:
        case PCI_CHIP_MACH64GO:
        case PCI_CHIP_MACH64GR:
        case PCI_CHIP_MACH64GS:
        case PCI_CHIP_MACH64LM:
        case PCI_CHIP_MACH64LN:
        case PCI_CHIP_MACH64LR:
        case PCI_CHIP_MACH64LS:
            return ATI_CHIP_FAMILY_Mach64;

        case PCI_CHIP_RAGE128RE:
        case PCI_CHIP_RAGE128RF:
        case PCI_CHIP_RAGE128RG:
        case PCI_CHIP_RAGE128SK:
        case PCI_CHIP_RAGE128SL:
        case PCI_CHIP_RAGE128SM:
        case PCI_CHIP_RAGE128SN:
        case PCI_CHIP_RAGE128RK:
        case PCI_CHIP_RAGE128RL:
        case PCI_CHIP_RAGE128SE:
        case PCI_CHIP_RAGE128SF:
        case PCI_CHIP_RAGE128SG:
        case PCI_CHIP_RAGE128SH:
        case PCI_CHIP_RAGE128PA:
        case PCI_CHIP_RAGE128PB:
        case PCI_CHIP_RAGE128PC:
        case PCI_CHIP_RAGE128PD:
        case PCI_CHIP_RAGE128PE:
        case PCI_CHIP_RAGE128PF:
        case PCI_CHIP_RAGE128PG:
        case PCI_CHIP_RAGE128PH:
        case PCI_CHIP_RAGE128PI:
        case PCI_CHIP_RAGE128PJ:
        case PCI_CHIP_RAGE128PK:
        case PCI_CHIP_RAGE128PL:
        case PCI_CHIP_RAGE128PM:
        case PCI_CHIP_RAGE128PN:
        case PCI_CHIP_RAGE128PO:
        case PCI_CHIP_RAGE128PP:
        case PCI_CHIP_RAGE128PQ:
        case PCI_CHIP_RAGE128PR:
        case PCI_CHIP_RAGE128PS:
        case PCI_CHIP_RAGE128PT:
        case PCI_CHIP_RAGE128PU:
        case PCI_CHIP_RAGE128PV:
        case PCI_CHIP_RAGE128PW:
        case PCI_CHIP_RAGE128PX:
        case PCI_CHIP_RAGE128TF:
        case PCI_CHIP_RAGE128TL:
        case PCI_CHIP_RAGE128TR:
        case PCI_CHIP_RAGE128TS:
        case PCI_CHIP_RAGE128TT:
        case PCI_CHIP_RAGE128TU:
        case PCI_CHIP_RAGE128LE:
        case PCI_CHIP_RAGE128LF:
#if 0
        case PCI_CHIP_RAGE128LK:
        case PCI_CHIP_RAGE128LL:
#endif
        case PCI_CHIP_RAGE128MF:
        case PCI_CHIP_RAGE128ML:
            return ATI_CHIP_FAMILY_Rage128;

        default:
            return ATI_CHIP_FAMILY_Radeon;
    }
}
