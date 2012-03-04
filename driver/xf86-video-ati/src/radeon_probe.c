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
#include <stdlib.h>

/*
 * Authors:
 *   Kevin E. Martin <martin@xfree86.org>
 *   Rickard E. Faith <faith@valinux.com>
 * KMS support - Dave Airlie <airlied@redhat.com>
 */

#include "radeon_probe.h"
#include "radeon_version.h"
#include "atipciids.h"
#include "atipcirename.h"

#include "xf86.h"
#if GET_ABI_MAJOR(ABI_VIDEODRV_VERSION) < 6
#include "xf86Resources.h"
#endif

#ifdef XF86DRM_MODE
#include "xf86drmMode.h"
#include "dri.h"
#endif

#include "radeon_chipset_gen.h"

#include "radeon_pci_chipset_gen.h"

#include "radeon_chipinfo_gen.h"

#ifdef XSERVER_LIBPCIACCESS
#include "radeon_pci_device_match_gen.h"

static Bool radeon_ums_supported(ScrnInfoPtr pScrn, struct pci_device *pci_dev)
{
    unsigned family = 0, i;

    for (i = 0; i < sizeof(RADEONCards) / sizeof(RADEONCardInfo); i++) {
        if (pci_dev->device_id == RADEONCards[i].pci_device_id) {
            family = RADEONCards[i].chip_family;
            break;
        }
    }

    if (family >= CHIP_FAMILY_SUMO) {
        xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 0,
                       "GPU only supported with KMS, using vesa instead.\n");
        return FALSE;
    }
    return TRUE;
}
#else
#define radeon_ums_supported(x, y) TRUE
#endif

#ifndef XSERVER_LIBPCIACCESS
static Bool RADEONProbe(DriverPtr drv, int flags);
#endif

_X_EXPORT int gRADEONEntityIndex = -1;

/* Return the options for supported chipset 'n'; NULL otherwise */
static const OptionInfoRec *
RADEONAvailableOptions(int chipid, int busid)
{
    return RADEONOptionsWeak();
}

/* Return the string name for supported chipset 'n'; NULL otherwise. */
static void
RADEONIdentify(int flags)
{
    xf86PrintChipsets(RADEON_NAME,
		      "Driver for ATI Radeon chipsets",
		      RADEONChipsets);
}


#ifdef XF86DRM_MODE
static Bool radeon_kernel_mode_enabled(ScrnInfoPtr pScrn, struct pci_device *pci_dev)
{
    char *busIdString;
    int ret;

    if (!xf86LoaderCheckSymbol("DRICreatePCIBusID")) {
      xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 0,
		   "[KMS] No DRICreatePCIBusID symbol, no kernel modesetting.\n");
	return FALSE;
    }

    busIdString = DRICreatePCIBusID(pci_dev);
    ret = drmCheckModesettingSupported(busIdString);
    free(busIdString);
    if (ret) {
      xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 0,
		   "[KMS] drm report modesetting isn't supported.\n");
	return FALSE;
    }

    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 0,
		   "[KMS] Kernel modesetting enabled.\n");
    return TRUE;
}
#else
#define radeon_kernel_mode_enabled(x, y) FALSE
#endif

static Bool
radeon_get_scrninfo(int entity_num, void *pci_dev)
{
    ScrnInfoPtr   pScrn = NULL;
    EntityInfoPtr pEnt;
    int kms = 0;

    pScrn = xf86ConfigPciEntity(pScrn, 0, entity_num, RADEONPciChipsets,
                                NULL,
                                NULL, NULL, NULL, NULL);

    if (!pScrn)
        return FALSE;

    if (pci_dev) {
      if (radeon_kernel_mode_enabled(pScrn, pci_dev)) {
	kms = 1;
      } else {
        if (!radeon_ums_supported(pScrn, pci_dev)) {
          return FALSE;
        }
      }
    }

    pScrn->driverVersion = RADEON_VERSION_CURRENT;
    pScrn->driverName    = RADEON_DRIVER_NAME;
    pScrn->name          = RADEON_NAME;
#ifdef XSERVER_LIBPCIACCESS
    pScrn->Probe         = NULL;
#else
    pScrn->Probe         = RADEONProbe;
#endif

#ifdef XF86DRM_MODE
    if (kms == 1) {
      pScrn->PreInit       = RADEONPreInit_KMS;
      pScrn->ScreenInit    = RADEONScreenInit_KMS;
      pScrn->SwitchMode    = RADEONSwitchMode_KMS;
      pScrn->AdjustFrame   = RADEONAdjustFrame_KMS;
      pScrn->EnterVT       = RADEONEnterVT_KMS;
      pScrn->LeaveVT       = RADEONLeaveVT_KMS;
      pScrn->FreeScreen    = RADEONFreeScreen_KMS;
      pScrn->ValidMode     = RADEONValidMode;
    } else 
#endif 
    {
      pScrn->PreInit       = RADEONPreInit;
      pScrn->ScreenInit    = RADEONScreenInit;
      pScrn->SwitchMode    = RADEONSwitchMode;
      pScrn->AdjustFrame   = RADEONAdjustFrame;
      pScrn->EnterVT       = RADEONEnterVT;
      pScrn->LeaveVT       = RADEONLeaveVT;
      pScrn->FreeScreen    = RADEONFreeScreen;
      pScrn->ValidMode     = RADEONValidMode;
    }

    pEnt = xf86GetEntityInfo(entity_num);

    /* Create a RADEONEntity for all chips, even with old single head
     * Radeon, need to use pRADEONEnt for new monitor detection routines.
     */
    {
        DevUnion    *pPriv;
        RADEONEntPtr pRADEONEnt;

        xf86SetEntitySharable(entity_num);

        if (gRADEONEntityIndex == -1)
            gRADEONEntityIndex = xf86AllocateEntityPrivateIndex();

        pPriv = xf86GetEntityPrivate(pEnt->index,
                                     gRADEONEntityIndex);

	xf86SetEntityInstanceForScreen(pScrn, pEnt->index, xf86GetNumEntityInstances(pEnt->index) - 1);

        if (!pPriv->ptr) {
            pPriv->ptr = xnfcalloc(sizeof(RADEONEntRec), 1);
            pRADEONEnt = pPriv->ptr;
            pRADEONEnt->HasSecondary = FALSE;
        } else {
            pRADEONEnt = pPriv->ptr;
            pRADEONEnt->HasSecondary = TRUE;
        }
    }

    free(pEnt);

    return TRUE;
}

#ifndef XSERVER_LIBPCIACCESS

/* Return TRUE if chipset is present; FALSE otherwise. */
static Bool
RADEONProbe(DriverPtr drv, int flags)
{
    int      numUsed;
    int      numDevSections;
    int     *usedChips;
    GDevPtr *devSections;
    Bool     foundScreen = FALSE;
    int      i;

    if (!xf86GetPciVideoInfo()) return FALSE;

    numDevSections = xf86MatchDevice(RADEON_NAME, &devSections);

    if (!numDevSections) return FALSE;

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
	    if (radeon_get_scrninfo(usedChips[i], NULL))
		foundScreen = TRUE;
	}
    }

    free(usedChips);
    free(devSections);

    return foundScreen;
}

#else /* XSERVER_LIBPCIACCESS */

static Bool
radeon_pci_probe(
    DriverPtr          pDriver,
    int                entity_num,
    struct pci_device *device,
    intptr_t           match_data
)
{
    return radeon_get_scrninfo(entity_num, (void *)device);
}

#endif /* XSERVER_LIBPCIACCESS */

_X_EXPORT DriverRec RADEON =
{
    RADEON_VERSION_CURRENT,
    RADEON_DRIVER_NAME,
    RADEONIdentify,
#ifdef XSERVER_LIBPCIACCESS
    NULL,
#else
    RADEONProbe,
#endif
    RADEONAvailableOptions,
    NULL,
    0,
    NULL,
#ifdef XSERVER_LIBPCIACCESS
    radeon_device_match,
    radeon_pci_probe
#endif
};
