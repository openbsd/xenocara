/*
 * Copyright 1999, 2000 ATI Technologies Inc., Markham, Ontario,
 *                      Precision Insight, Inc., Cedar Park, Texas, and
 *                      VA Linux Systems Inc., Fremont, California.
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, PRECISION INSIGHT, VA LINUX
 * SYSTEMS AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

/*
 * Authors:
 *   Rickard E. Faith <faith@valinux.com>
 *   Kevin E. Martin <martin@valinux.com>
 */

#include "r128_version.h"
#include "atipcirename.h"

#include "xf86.h"

#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#endif

#include "r128_probe.h"

#ifndef XSERVER_LIBPCIACCESS
static Bool R128Probe(DriverPtr drv, int flags);
#endif

SymTabRec R128Chipsets[] = {
    { PCI_CHIP_RAGE128LE, "ATI Rage 128 Mobility M3 LE (PCI)" },
    { PCI_CHIP_RAGE128LF, "ATI Rage 128 Mobility M3 LF (AGP)" },
    { PCI_CHIP_RAGE128MF, "ATI Rage 128 Mobility M4 MF (AGP)" },
    { PCI_CHIP_RAGE128ML, "ATI Rage 128 Mobility M4 ML (AGP)" },
    { PCI_CHIP_RAGE128PA, "ATI Rage 128 Pro GL PA (PCI/AGP)" },
    { PCI_CHIP_RAGE128PB, "ATI Rage 128 Pro GL PB (PCI/AGP)" },
    { PCI_CHIP_RAGE128PC, "ATI Rage 128 Pro GL PC (PCI/AGP)" },
    { PCI_CHIP_RAGE128PD, "ATI Rage 128 Pro GL PD (PCI)" },
    { PCI_CHIP_RAGE128PE, "ATI Rage 128 Pro GL PE (PCI/AGP)" },
    { PCI_CHIP_RAGE128PF, "ATI Rage 128 Pro GL PF (AGP)" },
    { PCI_CHIP_RAGE128PG, "ATI Rage 128 Pro VR PG (PCI/AGP)" },
    { PCI_CHIP_RAGE128PH, "ATI Rage 128 Pro VR PH (PCI/AGP)" },
    { PCI_CHIP_RAGE128PI, "ATI Rage 128 Pro VR PI (PCI/AGP)" },
    { PCI_CHIP_RAGE128PJ, "ATI Rage 128 Pro VR PJ (PCI/AGP)" },
    { PCI_CHIP_RAGE128PK, "ATI Rage 128 Pro VR PK (PCI/AGP)" },
    { PCI_CHIP_RAGE128PL, "ATI Rage 128 Pro VR PL (PCI/AGP)" },
    { PCI_CHIP_RAGE128PM, "ATI Rage 128 Pro VR PM (PCI/AGP)" },
    { PCI_CHIP_RAGE128PN, "ATI Rage 128 Pro VR PN (PCI/AGP)" },
    { PCI_CHIP_RAGE128PO, "ATI Rage 128 Pro VR PO (PCI/AGP)" },
    { PCI_CHIP_RAGE128PP, "ATI Rage 128 Pro VR PP (PCI)" },
    { PCI_CHIP_RAGE128PQ, "ATI Rage 128 Pro VR PQ (PCI/AGP)" },
    { PCI_CHIP_RAGE128PR, "ATI Rage 128 Pro VR PR (PCI)" },
    { PCI_CHIP_RAGE128PS, "ATI Rage 128 Pro VR PS (PCI/AGP)" },
    { PCI_CHIP_RAGE128PT, "ATI Rage 128 Pro VR PT (PCI/AGP)" },
    { PCI_CHIP_RAGE128PU, "ATI Rage 128 Pro VR PU (PCI/AGP)" },
    { PCI_CHIP_RAGE128PV, "ATI Rage 128 Pro VR PV (PCI/AGP)" },
    { PCI_CHIP_RAGE128PW, "ATI Rage 128 Pro VR PW (PCI/AGP)" },
    { PCI_CHIP_RAGE128PX, "ATI Rage 128 Pro VR PX (PCI/AGP)" },
    { PCI_CHIP_RAGE128RE, "ATI Rage 128 GL RE (PCI)" },
    { PCI_CHIP_RAGE128RF, "ATI Rage 128 GL RF (AGP)" },
    { PCI_CHIP_RAGE128RG, "ATI Rage 128 RG (AGP)" },
    { PCI_CHIP_RAGE128RK, "ATI Rage 128 VR RK (PCI)" },
    { PCI_CHIP_RAGE128RL, "ATI Rage 128 VR RL (AGP)" },
    { PCI_CHIP_RAGE128SE, "ATI Rage 128 4X SE (PCI/AGP)" },
    { PCI_CHIP_RAGE128SF, "ATI Rage 128 4X SF (PCI/AGP)" },
    { PCI_CHIP_RAGE128SG, "ATI Rage 128 4X SG (PCI/AGP)" },
    { PCI_CHIP_RAGE128SH, "ATI Rage 128 4X SH (PCI/AGP)" },
    { PCI_CHIP_RAGE128SK, "ATI Rage 128 4X SK (PCI/AGP)" },
    { PCI_CHIP_RAGE128SL, "ATI Rage 128 4X SL (PCI/AGP)" },
    { PCI_CHIP_RAGE128SM, "ATI Rage 128 4X SM (AGP)" },
    { PCI_CHIP_RAGE128SN, "ATI Rage 128 4X SN (PCI/AGP)" },
    { PCI_CHIP_RAGE128TF, "ATI Rage 128 Pro ULTRA TF (AGP)" },
    { PCI_CHIP_RAGE128TL, "ATI Rage 128 Pro ULTRA TL (AGP)" },
    { PCI_CHIP_RAGE128TR, "ATI Rage 128 Pro ULTRA TR (AGP)" },
    { PCI_CHIP_RAGE128TS, "ATI Rage 128 Pro ULTRA TS (AGP?)" },
    { PCI_CHIP_RAGE128TT, "ATI Rage 128 Pro ULTRA TT (AGP?)" },
    { PCI_CHIP_RAGE128TU, "ATI Rage 128 Pro ULTRA TU (AGP?)" },
    { -1,                 NULL }
};

static PciChipsets R128PciChipsets[] = {
    { PCI_CHIP_RAGE128LE, PCI_CHIP_RAGE128LE, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128LF, PCI_CHIP_RAGE128LF, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128MF, PCI_CHIP_RAGE128MF, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128ML, PCI_CHIP_RAGE128ML, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PA, PCI_CHIP_RAGE128PA, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PB, PCI_CHIP_RAGE128PB, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PC, PCI_CHIP_RAGE128PC, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PD, PCI_CHIP_RAGE128PD, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PE, PCI_CHIP_RAGE128PE, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PF, PCI_CHIP_RAGE128PF, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PG, PCI_CHIP_RAGE128PG, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PH, PCI_CHIP_RAGE128PH, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PI, PCI_CHIP_RAGE128PI, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PJ, PCI_CHIP_RAGE128PJ, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PK, PCI_CHIP_RAGE128PK, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PL, PCI_CHIP_RAGE128PL, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PM, PCI_CHIP_RAGE128PM, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PN, PCI_CHIP_RAGE128PN, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PO, PCI_CHIP_RAGE128PO, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PP, PCI_CHIP_RAGE128PP, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PQ, PCI_CHIP_RAGE128PQ, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PR, PCI_CHIP_RAGE128PR, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PS, PCI_CHIP_RAGE128PS, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PT, PCI_CHIP_RAGE128PT, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PU, PCI_CHIP_RAGE128PU, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PV, PCI_CHIP_RAGE128PV, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PW, PCI_CHIP_RAGE128PW, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128PX, PCI_CHIP_RAGE128PX, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128RE, PCI_CHIP_RAGE128RE, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128RF, PCI_CHIP_RAGE128RF, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128RG, PCI_CHIP_RAGE128RG, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128RK, PCI_CHIP_RAGE128RK, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128RL, PCI_CHIP_RAGE128RL, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128SE, PCI_CHIP_RAGE128SE, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128SF, PCI_CHIP_RAGE128SF, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128SG, PCI_CHIP_RAGE128SG, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128SH, PCI_CHIP_RAGE128SH, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128SK, PCI_CHIP_RAGE128SK, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128SL, PCI_CHIP_RAGE128SL, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128SM, PCI_CHIP_RAGE128SM, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128SN, PCI_CHIP_RAGE128SN, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128TF, PCI_CHIP_RAGE128TF, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128TL, PCI_CHIP_RAGE128TL, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128TR, PCI_CHIP_RAGE128TR, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128TS, PCI_CHIP_RAGE128TS, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128TT, PCI_CHIP_RAGE128TT, RES_SHARED_VGA },
    { PCI_CHIP_RAGE128TU, PCI_CHIP_RAGE128TU, RES_SHARED_VGA },
    { -1,                 -1,                 RES_UNDEFINED }
};

#ifdef XSERVER_LIBPCIACCESS

static const struct pci_id_match r128_device_match[] = {
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128LE, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128LF, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128MF, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128ML, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PA, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PB, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PC, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PD, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PE, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PF, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PG, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PH, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PI, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PJ, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PK, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PL, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PM, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PN, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PO, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PP, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PQ, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PR, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PS, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PT, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PU, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PV, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PW, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128PX, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128RE, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128RF, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128RG, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128RK, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128RL, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128SE, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128SF, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128SG, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128SH, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128SK, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128SL, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128SM, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128SN, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128TF, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128TL, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128TR, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128TS, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128TT, 0 ),
    ATI_DEVICE_MATCH( PCI_CHIP_RAGE128TU, 0 ),
    { 0, 0, 0 }
};

#endif /* XSERVER_LIBPCIACCESS */

_X_EXPORT int gR128EntityIndex = -1;

/* Return the options for supported chipset 'n'; NULL otherwise */
static const OptionInfoRec *
R128AvailableOptions(int chipid, int busid)
{
    return R128OptionsWeak();
}

/* Return the string name for supported chipset 'n'; NULL otherwise. */
static void
R128Identify(int flags)
{
    xf86PrintChipsets(R128_NAME,
		      "Driver for ATI Rage 128 chipsets",
		      R128Chipsets);
}

static Bool
r128_get_scrninfo(int entity_num)
{
    ScrnInfoPtr   pScrn = NULL;
    EntityInfoPtr pEnt;
    DevUnion*     pPriv;

    pScrn = xf86ConfigPciEntity(pScrn, 0, entity_num, R128PciChipsets,
                                NULL,
                                NULL, NULL, NULL, NULL);

    if (!pScrn)
        return FALSE;

    pScrn->driverVersion = R128_VERSION_CURRENT;
    pScrn->driverName    = R128_DRIVER_NAME;
    pScrn->name          = R128_NAME;
#ifdef XSERVER_LIBPCIACCESS
    pScrn->Probe         = NULL;
#else
    pScrn->Probe         = R128Probe;
#endif
    pScrn->PreInit       = R128PreInit;
    pScrn->ScreenInit    = R128ScreenInit;
    pScrn->SwitchMode    = R128SwitchMode;
    pScrn->AdjustFrame   = R128AdjustFrame;
    pScrn->EnterVT       = R128EnterVT;
    pScrn->LeaveVT       = R128LeaveVT;
    pScrn->FreeScreen    = R128FreeScreen;
    pScrn->ValidMode     = R128ValidMode;

    pEnt = xf86GetEntityInfo(entity_num);

    /* Allocate private entity used for convenience with one or two heads. */
    if (gR128EntityIndex < 0) {
        gR128EntityIndex = xf86AllocateEntityPrivateIndex();
        pPriv = xf86GetEntityPrivate(pScrn->entityList[0], gR128EntityIndex);

        if (!pPriv->ptr) {
            R128EntPtr pR128Ent;
            pPriv->ptr = xnfcalloc(sizeof(R128EntRec), 1);
            pR128Ent = pPriv->ptr;
            pR128Ent->HasSecondary = FALSE;
            pR128Ent->IsSecondaryRestored = FALSE;
        }
    }

    /* mobility cards support Dual-Head, mark the entity as sharable*/
    if (pEnt->chipset == PCI_CHIP_RAGE128LE ||
        pEnt->chipset == PCI_CHIP_RAGE128LF ||
        pEnt->chipset == PCI_CHIP_RAGE128MF ||
        pEnt->chipset == PCI_CHIP_RAGE128ML)
    {
        static int instance = 0;

        xf86SetEntitySharable(entity_num);

        xf86SetEntityInstanceForScreen(pScrn,
                                       pScrn->entityList[0],
                                       instance);

        instance++;
    }

    free(pEnt);

    return TRUE;
}

#ifndef XSERVER_LIBPCIACCESS

/* Return TRUE if chipset is present; FALSE otherwise. */
static Bool
R128Probe(DriverPtr drv, int flags)
{
    int           numUsed;
    int           numDevSections;
    int           *usedChips;
    GDevPtr       *devSections;
    Bool          foundScreen = FALSE;
    int           i;

    if (!xf86GetPciVideoInfo()) return FALSE;

    numDevSections = xf86MatchDevice(R128_NAME, &devSections);

    if (!numDevSections) return FALSE;

    numUsed = xf86MatchPciInstances(R128_NAME,
				    PCI_VENDOR_ATI,
				    R128Chipsets,
				    R128PciChipsets,
				    devSections,
				    numDevSections,
				    drv,
				    &usedChips);

    if (numUsed<=0) return FALSE;

    if (flags & PROBE_DETECT)
	foundScreen = TRUE;
    else for (i = 0; i < numUsed; i++) {
	if (r128_get_scrninfo(usedChips[i]))
	    foundScreen = TRUE;
    }

    free(usedChips);
    free(devSections);

    return foundScreen;
}

#else /* XSERVER_LIBPCIACCESS */

static Bool
r128_pci_probe(
    DriverPtr          pDriver,
    int                entity_num,
    struct pci_device *device,
    intptr_t           match_data
)
{
    return r128_get_scrninfo(entity_num);
}

#endif /* XSERVER_LIBPCIACCESS */

_X_EXPORT DriverRec R128 =
{
    R128_VERSION_CURRENT,
    R128_DRIVER_NAME,
    R128Identify,
#ifdef XSERVER_LIBPCIACCESS
    NULL,
#else
    R128Probe,
#endif
    R128AvailableOptions,
    NULL,
    0,
    NULL,
#ifdef XSERVER_LIBPCIACCESS
    r128_device_match,
    r128_pci_probe,
#endif
#ifdef XSERVER_PLATFORM_BUS
    NULL
#endif
};
