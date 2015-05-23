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
#include "atichip.h"
#include "atimach64io.h"
#include "atimach64probe.h"
#include "atimach64version.h"
#include "atioption.h"

/* include headers corresponding to ScrnInfoPtr fields */
#include "atipreinit.h"
#include "atiscreen.h"
#include "aticonsole.h"
#include "atiadjust.h"
#include "ativalid.h"

/* Chip definitions */
#define PCI_VENDOR_ATI			0x1002
#define PCI_CHIP_MACH64CT		0x4354
#define PCI_CHIP_MACH64CX		0x4358
#define PCI_CHIP_MACH64ET		0x4554
#define PCI_CHIP_MACH64GB		0x4742
#define PCI_CHIP_MACH64GD		0x4744
#define PCI_CHIP_MACH64GI		0x4749
#define PCI_CHIP_MACH64GL		0x474C
#define PCI_CHIP_MACH64GM		0x474D
#define PCI_CHIP_MACH64GN		0x474E
#define PCI_CHIP_MACH64GO		0x474F
#define PCI_CHIP_MACH64GP		0x4750
#define PCI_CHIP_MACH64GQ		0x4751
#define PCI_CHIP_MACH64GR		0x4752
#define PCI_CHIP_MACH64GS		0x4753
#define PCI_CHIP_MACH64GT		0x4754
#define PCI_CHIP_MACH64GU		0x4755
#define PCI_CHIP_MACH64GV		0x4756
#define PCI_CHIP_MACH64GW		0x4757
#define PCI_CHIP_MACH64GX		0x4758
#define PCI_CHIP_MACH64GY		0x4759
#define PCI_CHIP_MACH64GZ		0x475A
#define PCI_CHIP_MACH64LB		0x4C42
#define PCI_CHIP_MACH64LD		0x4C44
#define PCI_CHIP_MACH64LG		0x4C47
#define PCI_CHIP_MACH64LI		0x4C49
#define PCI_CHIP_MACH64LM		0x4C4D
#define PCI_CHIP_MACH64LN		0x4C4E
#define PCI_CHIP_MACH64LP		0x4C50
#define PCI_CHIP_MACH64LQ		0x4C51
#define PCI_CHIP_MACH64LR		0x4C52
#define PCI_CHIP_MACH64LS		0x4C53
#define PCI_CHIP_MACH64VT		0x5654
#define PCI_CHIP_MACH64VU		0x5655
#define PCI_CHIP_MACH64VV		0x5656

#ifndef XSERVER_LIBPCIACCESS
static Bool Mach64Probe(DriverPtr pDriver, int flags);
#endif

SymTabRec
Mach64Chipsets[] = {
    {ATI_CHIP_88800GXC, "ATI 88800GX-C"},
    {ATI_CHIP_88800GXD, "ATI 88800GX-D"},
    {ATI_CHIP_88800GXE, "ATI 88800GX-E"},
    {ATI_CHIP_88800GXF, "ATI 88800GX-F"},
    {ATI_CHIP_88800GX,  "ATI 88800GX"},
    {ATI_CHIP_88800CX,  "ATI 88800CX"},
    {ATI_CHIP_264CT,    "ATI 264CT"},
    {ATI_CHIP_264ET,    "ATI 264ET"},
    {ATI_CHIP_264VT,    "ATI 264VT"},
    {ATI_CHIP_264VTB,   "ATI 264VT-B"},
    {ATI_CHIP_264GT,    "ATI 3D Rage"},
    {ATI_CHIP_264GTB,   "ATI 3D Rage II"},
    {ATI_CHIP_264VT3,   "ATI 264VT3"},
    {ATI_CHIP_264GTDVD, "ATI 3D Rage II+DVD"},
    {ATI_CHIP_264LT,    "ATI 3D Rage LT"},
    {ATI_CHIP_264VT4,   "ATI 264VT4"},
    {ATI_CHIP_264GT2C,  "ATI 3D Rage IIc"},
    {ATI_CHIP_264GTPRO, "ATI 3D Rage Pro"},
    {ATI_CHIP_264LTPRO, "ATI 3D Rage LT Pro"},
    {ATI_CHIP_264XL,    "ATI 3D Rage XL or XC"},
    {ATI_CHIP_MOBILITY, "ATI 3D Rage Mobility"},
    {-1,      NULL }
};

#ifndef XSERVER_LIBPCIACCESS

/*
 * This table maps a PCI device ID to a chipset family identifier.
 */
static PciChipsets
Mach64PciChipsets[] = {
    {ATI_CHIP_88800GX,   PCI_CHIP_MACH64GX,  RES_SHARED_VGA},
    {ATI_CHIP_88800CX,   PCI_CHIP_MACH64CX,  RES_SHARED_VGA},
    {ATI_CHIP_264CT,     PCI_CHIP_MACH64CT,  RES_SHARED_VGA},
    {ATI_CHIP_264ET,     PCI_CHIP_MACH64ET,  RES_SHARED_VGA},
    {ATI_CHIP_264VT,     PCI_CHIP_MACH64VT,  RES_SHARED_VGA},
    {ATI_CHIP_264GT,     PCI_CHIP_MACH64GT,  RES_SHARED_VGA},
    {ATI_CHIP_264VT3,    PCI_CHIP_MACH64VU,  RES_SHARED_VGA},
    {ATI_CHIP_264GTDVD,  PCI_CHIP_MACH64GU,  RES_SHARED_VGA},
    {ATI_CHIP_264LT,     PCI_CHIP_MACH64LG,  RES_SHARED_VGA},
    {ATI_CHIP_264VT4,    PCI_CHIP_MACH64VV,  RES_SHARED_VGA},
    {ATI_CHIP_264GT2C,   PCI_CHIP_MACH64GV,  RES_SHARED_VGA},
    {ATI_CHIP_264GT2C,   PCI_CHIP_MACH64GW,  RES_SHARED_VGA},
    {ATI_CHIP_264GT2C,   PCI_CHIP_MACH64GY,  RES_SHARED_VGA},
    {ATI_CHIP_264GT2C,   PCI_CHIP_MACH64GZ,  RES_SHARED_VGA},
    {ATI_CHIP_264GTPRO,  PCI_CHIP_MACH64GB,  RES_SHARED_VGA},
    {ATI_CHIP_264GTPRO,  PCI_CHIP_MACH64GD,  RES_SHARED_VGA},
    {ATI_CHIP_264GTPRO,  PCI_CHIP_MACH64GI,  RES_SHARED_VGA},
    {ATI_CHIP_264GTPRO,  PCI_CHIP_MACH64GP,  RES_SHARED_VGA},
    {ATI_CHIP_264GTPRO,  PCI_CHIP_MACH64GQ,  RES_SHARED_VGA},
    {ATI_CHIP_264LTPRO,  PCI_CHIP_MACH64LB,  RES_SHARED_VGA},
    {ATI_CHIP_264LTPRO,  PCI_CHIP_MACH64LD,  RES_SHARED_VGA},
    {ATI_CHIP_264LTPRO,  PCI_CHIP_MACH64LI,  RES_SHARED_VGA},
    {ATI_CHIP_264LTPRO,  PCI_CHIP_MACH64LP,  RES_SHARED_VGA},
    {ATI_CHIP_264LTPRO,  PCI_CHIP_MACH64LQ,  RES_SHARED_VGA},
    {ATI_CHIP_264XL,     PCI_CHIP_MACH64GL,  RES_SHARED_VGA},
    {ATI_CHIP_264XL,     PCI_CHIP_MACH64GM,  RES_SHARED_VGA},
    {ATI_CHIP_264XL,     PCI_CHIP_MACH64GN,  RES_SHARED_VGA},
    {ATI_CHIP_264XL,     PCI_CHIP_MACH64GO,  RES_SHARED_VGA},
    {ATI_CHIP_264XL,     PCI_CHIP_MACH64GR,  RES_SHARED_VGA},
    {ATI_CHIP_264XL,     PCI_CHIP_MACH64GS,  RES_SHARED_VGA},
    {ATI_CHIP_MOBILITY,  PCI_CHIP_MACH64LM,  RES_SHARED_VGA},
    {ATI_CHIP_MOBILITY,  PCI_CHIP_MACH64LN,  RES_SHARED_VGA},
    {ATI_CHIP_MOBILITY,  PCI_CHIP_MACH64LR,  RES_SHARED_VGA},
    {ATI_CHIP_MOBILITY,  PCI_CHIP_MACH64LS,  RES_SHARED_VGA},
    {-1, -1, RES_UNDEFINED}
};

#else /* XSERVER_LIBPCIACCESS */

static const struct pci_id_match Mach64DeviceMatch[] = {
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GX, ATI_CHIP_88800GX),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64CX, ATI_CHIP_88800CX),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64CT, ATI_CHIP_264CT),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64ET, ATI_CHIP_264ET),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64VT, ATI_CHIP_264VT),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GT, ATI_CHIP_264GT),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64VU, ATI_CHIP_264VT3),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GU, ATI_CHIP_264GTDVD),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64LG, ATI_CHIP_264LT),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64VV, ATI_CHIP_264VT4),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GV, ATI_CHIP_264GT2C),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GW, ATI_CHIP_264GT2C),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GY, ATI_CHIP_264GT2C),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GZ, ATI_CHIP_264GT2C),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GB, ATI_CHIP_264GTPRO),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GD, ATI_CHIP_264GTPRO),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GI, ATI_CHIP_264GTPRO),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GP, ATI_CHIP_264GTPRO),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GQ, ATI_CHIP_264GTPRO),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64LB, ATI_CHIP_264LTPRO),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64LD, ATI_CHIP_264LTPRO),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64LI, ATI_CHIP_264LTPRO),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64LP, ATI_CHIP_264LTPRO),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64LQ, ATI_CHIP_264LTPRO),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GL, ATI_CHIP_264XL),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GM, ATI_CHIP_264XL),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GN, ATI_CHIP_264XL),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GO, ATI_CHIP_264XL),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GR, ATI_CHIP_264XL),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64GS, ATI_CHIP_264XL),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64LM, ATI_CHIP_MOBILITY),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64LN, ATI_CHIP_MOBILITY),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64LR, ATI_CHIP_MOBILITY),
    ATI_DEVICE_MATCH(PCI_CHIP_MACH64LS, ATI_CHIP_MOBILITY),
    { 0, 0, 0 }
};

#endif /* XSERVER_LIBPCIACCESS */

static const OptionInfoRec *
Mach64AvailableOptions(int chipid, int busid)
{
    return ATIOptionsWeak();
}

/*
 * Mach64Identify --
 *
 * Print the driver's list of chipset names.
 */
static void
Mach64Identify
(
    int flags
)
{
    xf86Msg(X_INFO, "%s: %s\n", MACH64_NAME,
            "Driver for ATI Mach64 chipsets");
}

#ifndef XSERVER_LIBPCIACCESS

/*
 * Mach64Probe --
 *
 * This function is called once, at the start of the first server generation to
 * do a minimal probe for supported hardware.
 */
static Bool
Mach64Probe(DriverPtr pDriver, int flags)
{
    ScrnInfoPtr pScrn;
    GDevPtr *devSections;
    int     *usedChips;
    int     numDevSections;
    int     numUsed;
    Bool    ProbeSuccess = FALSE;
    int     i;

    if (xf86GetPciVideoInfo() == NULL)
        return FALSE;

    numDevSections = xf86MatchDevice(MACH64_DRIVER_NAME, &devSections);

    if (numDevSections <= 0)
        return FALSE;

    numUsed = xf86MatchPciInstances(MACH64_NAME, PCI_VENDOR_ATI,
                                    Mach64Chipsets, Mach64PciChipsets,
                                    devSections, numDevSections,
                                    pDriver, &usedChips);
    free(devSections);

    if (numUsed <= 0)
        return FALSE;

    if (flags & PROBE_DETECT) {
        ProbeSuccess = TRUE;
    } else {
        for (i = 0; i < numUsed; i++) {
	    pScrn = xf86ConfigPciEntity(NULL, 0, usedChips[i],
					Mach64PciChipsets, 0, 0, 0, 0, NULL);

	    if (!pScrn)
		continue;

	    pScrn->driverVersion = MACH64_VERSION_CURRENT;
	    pScrn->driverName    = MACH64_DRIVER_NAME;
	    pScrn->name          = MACH64_NAME;
	    pScrn->Probe         = Mach64Probe;
	    pScrn->PreInit       = ATIPreInit;
	    pScrn->ScreenInit    = ATIScreenInit;
	    pScrn->SwitchMode    = ATISwitchMode;
	    pScrn->AdjustFrame   = ATIAdjustFrame;
	    pScrn->EnterVT       = ATIEnterVT;
	    pScrn->LeaveVT       = ATILeaveVT;
	    pScrn->FreeScreen    = ATIFreeScreen;
	    pScrn->ValidMode     = ATIValidMode;

	    ProbeSuccess = TRUE;
        }
    }

    free(usedChips);

    return ProbeSuccess;
}

#else /* XSERVER_LIBPCIACCESS */

static Bool
Mach64PciProbe(
    DriverPtr          pDriver,
    int                entityNum,
    struct pci_device *dev,
    intptr_t           matchData
)
{
    ScrnInfoPtr pScrn;
    ATIPtr pATI;

    pScrn = xf86ConfigPciEntity(NULL, 0, entityNum, NULL,
                                RES_SHARED_VGA, NULL, NULL, NULL, NULL);

    if (!pScrn)
        return FALSE;

    pScrn->driverVersion = MACH64_VERSION_CURRENT;
    pScrn->driverName    = MACH64_DRIVER_NAME;
    pScrn->name          = MACH64_NAME;
    pScrn->Probe         = NULL;
    pScrn->PreInit       = ATIPreInit;
    pScrn->ScreenInit    = ATIScreenInit;
    pScrn->SwitchMode    = ATISwitchMode;
    pScrn->AdjustFrame   = ATIAdjustFrame;
    pScrn->EnterVT       = ATIEnterVT;
    pScrn->LeaveVT       = ATILeaveVT;
    pScrn->FreeScreen    = ATIFreeScreen;
    pScrn->ValidMode     = ATIValidMode;

    if (!ATIGetRec(pScrn))
	return FALSE;

    pATI = ATIPTR(pScrn);
    pATI->Chip = matchData;

    return TRUE;
}

#endif /* XSERVER_LIBPCIACCESS */

_X_EXPORT DriverRec MACH64 =
{
    MACH64_VERSION_CURRENT,
    MACH64_DRIVER_NAME,
    Mach64Identify,
#ifdef XSERVER_LIBPCIACCESS
    NULL,
#else
    Mach64Probe,
#endif
    Mach64AvailableOptions,
    NULL,
    0,
    NULL,
#ifdef XSERVER_LIBPCIACCESS
    Mach64DeviceMatch,
    Mach64PciProbe
#endif
};
