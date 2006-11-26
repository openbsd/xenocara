/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/ati/r128_probe.c,v 1.17 2003/02/07 20:41:15 martin Exp $ */
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
 *
 * Modified by Marc Aurele La France <tsi@xfree86.org> for ATI driver merge.
 */

#include "atimodule.h"
#include "ativersion.h"

#include "r128_probe.h"
#include "r128_version.h"
#include "atipciids.h"

#include "xf86.h"
#include "xf86Resources.h"

#include "r128_chipset.h"

PciChipsets R128PciChipsets[] = {
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

int gR128EntityIndex = -1;

/* Return the options for supported chipset 'n'; NULL otherwise */
const OptionInfoRec *
R128AvailableOptions(int chipid, int busid)
{
    int i;

    /*
     * Return options defined in the r128 submodule which will have been
     * loaded by this point.
     */
    if ((chipid >> 16) == PCI_VENDOR_ATI)
	chipid -= PCI_VENDOR_ATI << 16;
    for (i = 0; R128PciChipsets[i].PCIid > 0; i++) {
	if (chipid == R128PciChipsets[i].PCIid)
	    return R128OptionsWeak();
    }
    return NULL;
}

/* Return the string name for supported chipset 'n'; NULL otherwise. */
void
R128Identify(int flags)
{
    xf86PrintChipsets(R128_NAME,
		      "Driver for ATI Rage 128 chipsets",
		      R128Chipsets);
}

/* Return TRUE if chipset is present; FALSE otherwise. */
Bool
R128Probe(DriverPtr drv, int flags)
{
    int           numUsed;
    int           numDevSections, nATIGDev, nR128GDev;
    int           *usedChips;
    GDevPtr       *devSections, *ATIGDevs, *R128GDevs;
    Bool          foundScreen = FALSE;
    int           i;

    if (!xf86GetPciVideoInfo()) return FALSE;

    /* Collect unclaimed device sections for both driver names */
    nATIGDev = xf86MatchDevice(ATI_NAME, &ATIGDevs);
    nR128GDev = xf86MatchDevice(R128_NAME, &R128GDevs);

    if (!(numDevSections = nATIGDev + nR128GDev)) return FALSE;

    if (!ATIGDevs) {
	if (!(devSections = R128GDevs))
	    numDevSections = 1;
	else
	    numDevSections = nR128GDev;
    } if (!R128GDevs) {
	devSections = ATIGDevs;
	numDevSections = nATIGDev;
    } else {
	/* Combine into one list */
	devSections = xnfalloc((numDevSections + 1) * sizeof(GDevPtr));
	(void)memcpy(devSections,
		     ATIGDevs, nATIGDev * sizeof(GDevPtr));
	(void)memcpy(devSections + nATIGDev,
		     R128GDevs, nR128GDev * sizeof(GDevPtr));
	devSections[numDevSections] = NULL;
	xfree(ATIGDevs);
	xfree(R128GDevs);
    }

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
        ScrnInfoPtr pScrn;
        EntityInfoPtr pEnt;
 
        pScrn    = NULL;
        if((pScrn = xf86ConfigPciEntity(pScrn, 0, usedChips[i],
             R128PciChipsets, 0, 0, 0, 0, 0)))
	{

#ifdef XFree86LOADER

	    if (!xf86LoadSubModule(pScrn, "r128")) {
		xf86Msg(X_ERROR,
		    R128_NAME ":  Failed to load \"r128\" module.\n");
		xf86DeleteScreen(pScrn->scrnIndex, 0);
		continue;
	    }

	    xf86LoaderReqSymLists(R128Symbols, NULL);

#endif

	    pScrn->Probe         = R128Probe;
	    R128FillInScreenInfo(pScrn);

	    foundScreen          = TRUE;

        pEnt = xf86GetEntityInfo(usedChips[i]);

        /* mobility cards support Dual-Head, mark the entity as sharable*/
        if(pEnt->chipset == PCI_CHIP_RAGE128LE ||
           pEnt->chipset == PCI_CHIP_RAGE128LF ||
           pEnt->chipset == PCI_CHIP_RAGE128MF ||
           pEnt->chipset == PCI_CHIP_RAGE128ML)
        {
            static int instance = 0;
            DevUnion* pPriv;

            xf86SetEntitySharable(usedChips[i]);
            xf86SetEntityInstanceForScreen(pScrn,
                pScrn->entityList[0], instance);

            if(gR128EntityIndex < 0)
            {
                gR128EntityIndex = xf86AllocateEntityPrivateIndex();
                pPriv = xf86GetEntityPrivate(pScrn->entityList[0],
                        gR128EntityIndex);

                if (!pPriv->ptr)
                {
                    R128EntPtr pR128Ent;
                    pPriv->ptr = xnfcalloc(sizeof(R128EntRec), 1);
                    pR128Ent = pPriv->ptr;
                    pR128Ent->IsDRIEnabled = FALSE;
                    pR128Ent->BypassSecondary = FALSE;
                    pR128Ent->HasSecondary = FALSE;
                    pR128Ent->IsSecondaryRestored = FALSE;                   
                } 
            }
            instance++;
	}
	xfree(pEnt);
    }
    }

    xfree(usedChips);
    xfree(devSections);

    return foundScreen;
}
