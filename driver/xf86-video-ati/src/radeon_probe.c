/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/ati/radeon_probe.c,v 1.30 2003/10/07 22:47:12 martin Exp $ */
/*
 * Copyright 2000 ATI Technologies Inc., Markham, Ontario, and
 *                VA Linux Systems Inc., Fremont, California.
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
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 *
 * Modified by Marc Aurele La France <tsi@xfree86.org> for ATI driver merge.
 */

#include "atimodule.h"
#include "ativersion.h"

#include "radeon_probe.h"
#include "radeon_version.h"
#include "atipciids.h"

#include "xf86.h"
#define _XF86MISC_SERVER_
#include <X11/extensions/xf86misc.h>
#include "xf86Resources.h"

#include "radeon_chipset.h"

PciChipsets RADEONPciChipsets[] = {
    { PCI_CHIP_RADEON_QD, PCI_CHIP_RADEON_QD, RES_SHARED_VGA },
    { PCI_CHIP_RADEON_QE, PCI_CHIP_RADEON_QE, RES_SHARED_VGA },
    { PCI_CHIP_RADEON_QF, PCI_CHIP_RADEON_QF, RES_SHARED_VGA },
    { PCI_CHIP_RADEON_QG, PCI_CHIP_RADEON_QG, RES_SHARED_VGA },
    { PCI_CHIP_RV100_QY, PCI_CHIP_RV100_QY, RES_SHARED_VGA },
    { PCI_CHIP_RV100_QZ, PCI_CHIP_RV100_QZ, RES_SHARED_VGA },
    { PCI_CHIP_RN50_515E, PCI_CHIP_RN50_515E, RES_SHARED_VGA },
    { PCI_CHIP_RN50_5969, PCI_CHIP_RN50_5969, RES_SHARED_VGA },
    { PCI_CHIP_RADEON_LW, PCI_CHIP_RADEON_LW, RES_SHARED_VGA },
    { PCI_CHIP_RADEON_LX, PCI_CHIP_RADEON_LX, RES_SHARED_VGA },
    { PCI_CHIP_RADEON_LY, PCI_CHIP_RADEON_LY, RES_SHARED_VGA },
    { PCI_CHIP_RADEON_LZ, PCI_CHIP_RADEON_LZ, RES_SHARED_VGA },
    { PCI_CHIP_RS100_4136, PCI_CHIP_RS100_4136, RES_SHARED_VGA },
    { PCI_CHIP_RS100_4336, PCI_CHIP_RS100_4336, RES_SHARED_VGA },
    { PCI_CHIP_RS200_4137, PCI_CHIP_RS200_4137, RES_SHARED_VGA },
    { PCI_CHIP_RS200_4337, PCI_CHIP_RS200_4337, RES_SHARED_VGA },
    { PCI_CHIP_RS250_4237, PCI_CHIP_RS250_4237, RES_SHARED_VGA },
    { PCI_CHIP_RS250_4437, PCI_CHIP_RS250_4437, RES_SHARED_VGA },
    { PCI_CHIP_R200_QH, PCI_CHIP_R200_QH, RES_SHARED_VGA },
    { PCI_CHIP_R200_QL, PCI_CHIP_R200_QL, RES_SHARED_VGA },
    { PCI_CHIP_R200_QM, PCI_CHIP_R200_QM, RES_SHARED_VGA },
    { PCI_CHIP_R200_BB, PCI_CHIP_R200_BB, RES_SHARED_VGA },
    { PCI_CHIP_R200_BC, PCI_CHIP_R200_BC, RES_SHARED_VGA },
    { PCI_CHIP_RV200_QW, PCI_CHIP_RV200_QW, RES_SHARED_VGA },
    { PCI_CHIP_RV200_QX, PCI_CHIP_RV200_QX, RES_SHARED_VGA },
    { PCI_CHIP_RV250_If, PCI_CHIP_RV250_If, RES_SHARED_VGA },
    { PCI_CHIP_RV250_Ig, PCI_CHIP_RV250_Ig, RES_SHARED_VGA },
    { PCI_CHIP_RV250_Ld, PCI_CHIP_RV250_Ld, RES_SHARED_VGA },
    { PCI_CHIP_RV250_Lf, PCI_CHIP_RV250_Lf, RES_SHARED_VGA },
    { PCI_CHIP_RV250_Lg, PCI_CHIP_RV250_Lg, RES_SHARED_VGA },
    { PCI_CHIP_RS300_5834, PCI_CHIP_RS300_5834, RES_SHARED_VGA },
    { PCI_CHIP_RS300_5835, PCI_CHIP_RS300_5835, RES_SHARED_VGA },
    { PCI_CHIP_RS350_7834, PCI_CHIP_RS350_7834, RES_SHARED_VGA },
    { PCI_CHIP_RS350_7835, PCI_CHIP_RS350_7835, RES_SHARED_VGA },
    { PCI_CHIP_RV280_5960, PCI_CHIP_RV280_5960, RES_SHARED_VGA },
    { PCI_CHIP_RV280_5961, PCI_CHIP_RV280_5961, RES_SHARED_VGA },
    { PCI_CHIP_RV280_5962, PCI_CHIP_RV280_5962, RES_SHARED_VGA },
    { PCI_CHIP_RV280_5964, PCI_CHIP_RV280_5964, RES_SHARED_VGA },
    { PCI_CHIP_RV280_5965, PCI_CHIP_RV280_5965, RES_SHARED_VGA },
    { PCI_CHIP_RV280_5C61, PCI_CHIP_RV280_5C61, RES_SHARED_VGA },
    { PCI_CHIP_RV280_5C63, PCI_CHIP_RV280_5C63, RES_SHARED_VGA },
    { PCI_CHIP_R300_AD, PCI_CHIP_R300_AD, RES_SHARED_VGA },
    { PCI_CHIP_R300_AE, PCI_CHIP_R300_AE, RES_SHARED_VGA },
    { PCI_CHIP_R300_AF, PCI_CHIP_R300_AF, RES_SHARED_VGA },
    { PCI_CHIP_R300_AG, PCI_CHIP_R300_AG, RES_SHARED_VGA },
    { PCI_CHIP_R300_ND, PCI_CHIP_R300_ND, RES_SHARED_VGA },
    { PCI_CHIP_R300_NE, PCI_CHIP_R300_NE, RES_SHARED_VGA },
    { PCI_CHIP_R300_NF, PCI_CHIP_R300_NF, RES_SHARED_VGA },
    { PCI_CHIP_R300_NG, PCI_CHIP_R300_NG, RES_SHARED_VGA },
    { PCI_CHIP_RV350_AP, PCI_CHIP_RV350_AP, RES_SHARED_VGA },
    { PCI_CHIP_RV350_AQ, PCI_CHIP_RV350_AQ, RES_SHARED_VGA },
    { PCI_CHIP_RV360_AR, PCI_CHIP_RV360_AR, RES_SHARED_VGA },
    { PCI_CHIP_RV350_AS, PCI_CHIP_RV350_AS, RES_SHARED_VGA },
    { PCI_CHIP_RV350_AT, PCI_CHIP_RV350_AT, RES_SHARED_VGA },
    { PCI_CHIP_RV350_AV, PCI_CHIP_RV350_AV, RES_SHARED_VGA },
    { PCI_CHIP_RV350_NP, PCI_CHIP_RV350_NP, RES_SHARED_VGA },
    { PCI_CHIP_RV350_NQ, PCI_CHIP_RV350_NQ, RES_SHARED_VGA },
    { PCI_CHIP_RV350_NR, PCI_CHIP_RV350_NR, RES_SHARED_VGA },
    { PCI_CHIP_RV350_NS, PCI_CHIP_RV350_NS, RES_SHARED_VGA },
    { PCI_CHIP_RV350_NT, PCI_CHIP_RV350_NT, RES_SHARED_VGA },
    { PCI_CHIP_RV350_NV, PCI_CHIP_RV350_NV, RES_SHARED_VGA },
    { PCI_CHIP_RV350_4155, PCI_CHIP_RV350_4155, RES_SHARED_VGA },
    { PCI_CHIP_R350_AH, PCI_CHIP_R350_AH, RES_SHARED_VGA },
    { PCI_CHIP_R350_AI, PCI_CHIP_R350_AI, RES_SHARED_VGA },
    { PCI_CHIP_R350_AJ, PCI_CHIP_R350_AJ, RES_SHARED_VGA },
    { PCI_CHIP_R350_AK, PCI_CHIP_R350_AK, RES_SHARED_VGA },
    { PCI_CHIP_R350_NH, PCI_CHIP_R350_NH, RES_SHARED_VGA },
    { PCI_CHIP_R350_NI, PCI_CHIP_R350_NI, RES_SHARED_VGA },
    { PCI_CHIP_R350_NK, PCI_CHIP_R350_NK, RES_SHARED_VGA },
    { PCI_CHIP_R360_NJ, PCI_CHIP_R360_NJ, RES_SHARED_VGA },
    { PCI_CHIP_RV380_3E50, PCI_CHIP_RV380_3E50, RES_SHARED_VGA },
    { PCI_CHIP_RV380_3E54, PCI_CHIP_RV380_3E54, RES_SHARED_VGA },
    { PCI_CHIP_RV380_3150, PCI_CHIP_RV380_3150, RES_SHARED_VGA },
    { PCI_CHIP_RV380_3152, PCI_CHIP_RV380_3152, RES_SHARED_VGA },
    { PCI_CHIP_RV380_3154, PCI_CHIP_RV380_3154, RES_SHARED_VGA },
    { PCI_CHIP_RV370_5B60, PCI_CHIP_RV370_5B60, RES_SHARED_VGA },
    { PCI_CHIP_RV370_5B62, PCI_CHIP_RV370_5B62, RES_SHARED_VGA },
    { PCI_CHIP_RV370_5B63, PCI_CHIP_RV370_5B63, RES_SHARED_VGA },
    { PCI_CHIP_RV370_5B64, PCI_CHIP_RV370_5B64, RES_SHARED_VGA },
    { PCI_CHIP_RV370_5B65, PCI_CHIP_RV370_5B65, RES_SHARED_VGA },
    { PCI_CHIP_RV370_5460, PCI_CHIP_RV370_5460, RES_SHARED_VGA },
    { PCI_CHIP_RV370_5462, PCI_CHIP_RV370_5462, RES_SHARED_VGA },
    { PCI_CHIP_RV370_5464, PCI_CHIP_RV370_5464, RES_SHARED_VGA },
    { PCI_CHIP_RS400_5A41, PCI_CHIP_RS400_5A41, RES_SHARED_VGA },
    { PCI_CHIP_RS400_5A42, PCI_CHIP_RS400_5A42, RES_SHARED_VGA },
    { PCI_CHIP_RC410_5A61, PCI_CHIP_RC410_5A61, RES_SHARED_VGA },
    { PCI_CHIP_RC410_5A62, PCI_CHIP_RC410_5A62, RES_SHARED_VGA },
    { PCI_CHIP_RS480_5954, PCI_CHIP_RS480_5954, RES_SHARED_VGA },
    { PCI_CHIP_RS480_5955, PCI_CHIP_RS480_5955, RES_SHARED_VGA },
    { PCI_CHIP_RS482_5974, PCI_CHIP_RS482_5974, RES_SHARED_VGA },
    { PCI_CHIP_RS482_5975, PCI_CHIP_RS482_5975, RES_SHARED_VGA },
    { PCI_CHIP_RV410_5E48, PCI_CHIP_RV410_5E48, RES_SHARED_VGA },
    { PCI_CHIP_RV410_564A, PCI_CHIP_RV410_564A, RES_SHARED_VGA },
    { PCI_CHIP_RV410_564B, PCI_CHIP_RV410_564B, RES_SHARED_VGA },
    { PCI_CHIP_RV410_564F, PCI_CHIP_RV410_564F, RES_SHARED_VGA },
    { PCI_CHIP_RV410_5652, PCI_CHIP_RV410_5652, RES_SHARED_VGA },
    { PCI_CHIP_RV410_5653, PCI_CHIP_RV410_5653, RES_SHARED_VGA },
    { PCI_CHIP_RV410_5E4B, PCI_CHIP_RV410_5E4B, RES_SHARED_VGA },
    { PCI_CHIP_RV410_5E4A, PCI_CHIP_RV410_5E4A, RES_SHARED_VGA },
    { PCI_CHIP_RV410_5E4D, PCI_CHIP_RV410_5E4D, RES_SHARED_VGA },
    { PCI_CHIP_RV410_5E4C, PCI_CHIP_RV410_5E4C, RES_SHARED_VGA },
    { PCI_CHIP_RV410_5E4F, PCI_CHIP_RV410_5E4F, RES_SHARED_VGA },
    { PCI_CHIP_R420_JH, PCI_CHIP_R420_JH, RES_SHARED_VGA },
    { PCI_CHIP_R420_JI, PCI_CHIP_R420_JI, RES_SHARED_VGA },
    { PCI_CHIP_R420_JJ, PCI_CHIP_R420_JJ, RES_SHARED_VGA },
    { PCI_CHIP_R420_JK, PCI_CHIP_R420_JK, RES_SHARED_VGA },
    { PCI_CHIP_R420_JL, PCI_CHIP_R420_JL, RES_SHARED_VGA },
    { PCI_CHIP_R420_JM, PCI_CHIP_R420_JM, RES_SHARED_VGA },
    { PCI_CHIP_R420_JN, PCI_CHIP_R420_JN, RES_SHARED_VGA },
    { PCI_CHIP_R420_JP, PCI_CHIP_R420_JP, RES_SHARED_VGA },
    { PCI_CHIP_R420_4A4F, PCI_CHIP_R420_4A4F, RES_SHARED_VGA },
    { PCI_CHIP_R420_4A54, PCI_CHIP_R420_4A54, RES_SHARED_VGA },
    { PCI_CHIP_R423_UH, PCI_CHIP_R423_UH, RES_SHARED_VGA },
    { PCI_CHIP_R423_UI, PCI_CHIP_R423_UI, RES_SHARED_VGA },
    { PCI_CHIP_R423_UJ, PCI_CHIP_R423_UJ, RES_SHARED_VGA },
    { PCI_CHIP_R423_UK, PCI_CHIP_R423_UK, RES_SHARED_VGA },
    { PCI_CHIP_R423_UQ, PCI_CHIP_R423_UQ, RES_SHARED_VGA },
    { PCI_CHIP_R423_UR, PCI_CHIP_R423_UR, RES_SHARED_VGA },
    { PCI_CHIP_R423_UT, PCI_CHIP_R423_UT, RES_SHARED_VGA },
    { PCI_CHIP_R423_5D57, PCI_CHIP_R423_5D57, RES_SHARED_VGA },
    { PCI_CHIP_R423_5550, PCI_CHIP_R423_5550, RES_SHARED_VGA },
    { PCI_CHIP_R430_5D49, PCI_CHIP_R430_5D49, RES_SHARED_VGA },
    { PCI_CHIP_R430_5D4A, PCI_CHIP_R430_5D4A, RES_SHARED_VGA },
    { PCI_CHIP_R430_5D48, PCI_CHIP_R430_5D48, RES_SHARED_VGA },
    { PCI_CHIP_R430_554F, PCI_CHIP_R430_554F, RES_SHARED_VGA },
    { PCI_CHIP_R430_554D, PCI_CHIP_R430_554D, RES_SHARED_VGA },
    { PCI_CHIP_R430_554E, PCI_CHIP_R430_554E, RES_SHARED_VGA },
    { PCI_CHIP_R430_554C, PCI_CHIP_R430_554C, RES_SHARED_VGA },
    { PCI_CHIP_R480_5D4C, PCI_CHIP_R480_5D4C, RES_SHARED_VGA },
    { PCI_CHIP_R480_5D50, PCI_CHIP_R480_5D50, RES_SHARED_VGA },
    { PCI_CHIP_R480_5D4E, PCI_CHIP_R480_5D4E, RES_SHARED_VGA },
    { PCI_CHIP_R480_5D4F, PCI_CHIP_R480_5D4F, RES_SHARED_VGA },
    { PCI_CHIP_R480_5D52, PCI_CHIP_R480_5D52, RES_SHARED_VGA },
    { PCI_CHIP_R480_5D4D, PCI_CHIP_R480_5D4D, RES_SHARED_VGA },
    { PCI_CHIP_R481_4B4B, PCI_CHIP_R481_4B4B, RES_SHARED_VGA },
    { PCI_CHIP_R481_4B4A, PCI_CHIP_R481_4B4A, RES_SHARED_VGA },
    { PCI_CHIP_R481_4B49, PCI_CHIP_R481_4B49, RES_SHARED_VGA },
    { PCI_CHIP_R481_4B4C, PCI_CHIP_R481_4B4C, RES_SHARED_VGA },

    { -1,                 -1,                 RES_UNDEFINED }
};

int gRADEONEntityIndex = -1;

/* Return the options for supported chipset 'n'; NULL otherwise */
const OptionInfoRec *
RADEONAvailableOptions(int chipid, int busid)
{
    int  i;

    /*
     * Return options defined in the radeon submodule which will have been
     * loaded by this point.
     */
    if ((chipid >> 16) == PCI_VENDOR_ATI)
	chipid -= PCI_VENDOR_ATI << 16;
    for (i = 0; RADEONPciChipsets[i].PCIid > 0; i++) {
	if (chipid == RADEONPciChipsets[i].PCIid)
	    return RADEONOptionsWeak();
    }
    return NULL;
}

/* Return the string name for supported chipset 'n'; NULL otherwise. */
void
RADEONIdentify(int flags)
{
    xf86PrintChipsets(RADEON_NAME,
		      "Driver for ATI Radeon chipsets",
		      RADEONChipsets);
}

/* Return TRUE if chipset is present; FALSE otherwise. */
Bool
RADEONProbe(DriverPtr drv, int flags)
{
    int      numUsed;
    int      numDevSections, nATIGDev, nRadeonGDev;
    int     *usedChips;
    GDevPtr *devSections, *ATIGDevs, *RadeonGDevs;
    Bool     foundScreen = FALSE;
    int      i;

    if (!xf86GetPciVideoInfo()) return FALSE;

    /* Collect unclaimed device sections for both driver names */
    nATIGDev    = xf86MatchDevice(ATI_NAME, &ATIGDevs);
    nRadeonGDev = xf86MatchDevice(RADEON_NAME, &RadeonGDevs);

    if (!(numDevSections = nATIGDev + nRadeonGDev)) return FALSE;

    if (!ATIGDevs) {
	if (!(devSections = RadeonGDevs)) numDevSections = 1;
	else                              numDevSections = nRadeonGDev;
    } if (!RadeonGDevs) {
	devSections    = ATIGDevs;
	numDevSections = nATIGDev;
    } else {
	/* Combine into one list */
	devSections = xnfalloc((numDevSections + 1) * sizeof(GDevPtr));
	(void)memcpy(devSections,
		     ATIGDevs, nATIGDev * sizeof(GDevPtr));
	(void)memcpy(devSections + nATIGDev,
		     RadeonGDevs, nRadeonGDev * sizeof(GDevPtr));
	devSections[numDevSections] = NULL;
	xfree(ATIGDevs);
	xfree(RadeonGDevs);
    }

    numUsed = xf86MatchPciInstances(RADEON_NAME,
				    PCI_VENDOR_ATI,
				    RADEONChipsets,
				    RADEONPciChipsets,
				    devSections,
				    numDevSections,
				    drv,
				    &usedChips);

    if (numUsed <= 0) return FALSE;

    if (flags & PROBE_DETECT) {
	foundScreen = TRUE;
    } else {
	for (i = 0; i < numUsed; i++) {
	    ScrnInfoPtr    pScrn = NULL;
	    EntityInfoPtr  pEnt;
	    pEnt = xf86GetEntityInfo(usedChips[i]);
	    if ((pScrn = xf86ConfigPciEntity(pScrn, 0, usedChips[i],
					     RADEONPciChipsets, 0, 0, 0,
					     0, 0))) {
#ifdef XFree86LOADER
		if (!xf86LoadSubModule(pScrn, "radeon")) {
		    xf86Msg(X_ERROR, RADEON_NAME
			    ":  Failed to load \"radeon\" module.\n");
		    xf86DeleteScreen(pScrn->scrnIndex, 0);
		    continue;
		}

		xf86LoaderReqSymLists(RADEONSymbols, NULL);
#endif

		pScrn->Probe         = RADEONProbe;
		RADEONFillInScreenInfo(pScrn);
		foundScreen          = TRUE;
	    }

	    pEnt = xf86GetEntityInfo(usedChips[i]);

            /* create a RADEONEntity for all chips, even with
               old single head Radeon, need to use pRADEONEnt
               for new monitor detection routines
            */
            {
		DevUnion   *pPriv;
		RADEONEntPtr pRADEONEnt;

		xf86SetEntitySharable(usedChips[i]);

		if (gRADEONEntityIndex == -1)
		    gRADEONEntityIndex = xf86AllocateEntityPrivateIndex();

		pPriv = xf86GetEntityPrivate(pEnt->index,
					     gRADEONEntityIndex);

		if (!pPriv->ptr) {
		    int j;
		    int instance = xf86GetNumEntityInstances(pEnt->index);

		    for (j = 0; j < instance; j++)
			xf86SetEntityInstanceForScreen(pScrn, pEnt->index, j);

		    pPriv->ptr = xnfcalloc(sizeof(RADEONEntRec), 1);
		    pRADEONEnt = pPriv->ptr;
		    pRADEONEnt->HasSecondary = FALSE;
		} else {
		    pRADEONEnt = pPriv->ptr;
		    pRADEONEnt->HasSecondary = TRUE;
		}
	    }
	    xfree(pEnt);
	}
    }

    xfree(usedChips);
    xfree(devSections);

    return foundScreen;
}
