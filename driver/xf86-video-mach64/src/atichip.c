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
#include "atibus.h"
#include "atichip.h"
#include "atimach64io.h"
#include "atimach64version.h"

const char *ATIFoundryNames[] =
    { "SGS", "NEC", "KCS", "UMC", "TSMC", "5", "6", "UMC" };

/*
 * ATIMach64ChipID --
 *
 * Set variables whose value is dependent upon a Mach64's CONFIG_CHIP_ID
 * register.
 */
void
ATIMach64ChipID
(
    ATIPtr       pATI,
    const CARD16 ExpectedChipType
)
{
    pATI->config_chip_id = inr(CONFIG_CHIP_ID);
    pATI->ChipType       = GetBits(pATI->config_chip_id, CFG_CHIP_TYPE);
    pATI->ChipClass      = GetBits(pATI->config_chip_id, CFG_CHIP_CLASS);
    pATI->ChipRev        = GetBits(pATI->config_chip_id, CFG_CHIP_REV);
    pATI->ChipVersion    = GetBits(pATI->config_chip_id, CFG_CHIP_VERSION);
    pATI->ChipFoundry    = GetBits(pATI->config_chip_id, CFG_CHIP_FOUNDRY);
    pATI->ChipRevision   = GetBits(pATI->config_chip_id, CFG_CHIP_REVISION);
    switch (pATI->ChipType)
    {
        case OldChipID('G', 'X'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('G', 'X'):
            switch (pATI->ChipRev)
            {
                case 0x00U:
                    pATI->Chip = ATI_CHIP_88800GXC;
                    break;

                case 0x01U:
                    pATI->Chip = ATI_CHIP_88800GXD;
                    break;

                case 0x02U:
                    pATI->Chip = ATI_CHIP_88800GXE;
                    break;

                case 0x03U:
                    pATI->Chip = ATI_CHIP_88800GXF;
                    break;

                default:
                    pATI->Chip = ATI_CHIP_88800GX;
                    break;
            }
            break;

        case OldChipID('C', 'X'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('C', 'X'):
            pATI->Chip = ATI_CHIP_88800CX;
            break;

        case OldChipID('C', 'T'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('C', 'T'):
            pATI->Chip = ATI_CHIP_264CT;
            pATI->BusType = ATI_BUS_PCI;
            break;

        case OldChipID('E', 'T'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('E', 'T'):
            pATI->Chip = ATI_CHIP_264ET;
            pATI->BusType = ATI_BUS_PCI;
            break;

        case OldChipID('V', 'T'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('V', 'T'):
            pATI->Chip = ATI_CHIP_264VT;
            pATI->BusType = ATI_BUS_PCI;
            /* Some early GT's are detected as VT's */
            if (ExpectedChipType && (pATI->ChipType != ExpectedChipType))
            {
                if (ExpectedChipType == NewChipID('G', 'T'))
                    pATI->Chip = ATI_CHIP_264GT;
                else
                    xf86Msg(X_WARNING,
                            MACH64_NAME ":  Mach64 chip type probe discrepancy"
                            " detected:  PCI=0x%04X;  CHIP_ID=0x%04X.\n",
                            ExpectedChipType, pATI->ChipType);
            }
            else if (pATI->ChipVersion)
                pATI->Chip = ATI_CHIP_264VTB;
            break;

        case OldChipID('G', 'T'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('G', 'T'):
            pATI->BusType = ATI_BUS_PCI;
            if (!pATI->ChipVersion)
                pATI->Chip = ATI_CHIP_264GT;
            else
                pATI->Chip = ATI_CHIP_264GTB;
            break;

        case OldChipID('V', 'U'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('V', 'U'):
            pATI->Chip = ATI_CHIP_264VT3;
            pATI->BusType = ATI_BUS_PCI;
            break;

        case OldChipID('G', 'U'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('G', 'U'):
            pATI->Chip = ATI_CHIP_264GTDVD;
            pATI->BusType = ATI_BUS_PCI;
            break;

        case OldChipID('L', 'G'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('L', 'G'):
            pATI->Chip = ATI_CHIP_264LT;
            pATI->BusType = ATI_BUS_PCI;
            break;

        case OldChipID('V', 'V'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('V', 'V'):
            pATI->Chip = ATI_CHIP_264VT4;
            pATI->BusType = ATI_BUS_PCI;
            break;

        case OldChipID('G', 'V'):
        case OldChipID('G', 'Y'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('G', 'V'):
        case NewChipID('G', 'Y'):
            pATI->Chip = ATI_CHIP_264GT2C;
            pATI->BusType = ATI_BUS_PCI;
            break;

        case OldChipID('G', 'W'):
        case OldChipID('G', 'Z'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('G', 'W'):
        case NewChipID('G', 'Z'):
            pATI->Chip = ATI_CHIP_264GT2C;
            pATI->BusType = ATI_BUS_AGP;
            break;

        case OldChipID('G', 'I'):
        case OldChipID('G', 'P'):
        case OldChipID('G', 'Q'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('G', 'I'):
        case NewChipID('G', 'P'):
        case NewChipID('G', 'Q'):
            pATI->Chip = ATI_CHIP_264GTPRO;
            pATI->BusType = ATI_BUS_PCI;
            break;

        case OldChipID('G', 'B'):
        case OldChipID('G', 'D'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('G', 'B'):
        case NewChipID('G', 'D'):
            pATI->Chip = ATI_CHIP_264GTPRO;
            pATI->BusType = ATI_BUS_AGP;
            break;

        case OldChipID('L', 'I'):
        case OldChipID('L', 'P'):
        case OldChipID('L', 'Q'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('L', 'I'):
        case NewChipID('L', 'P'):
        case NewChipID('L', 'Q'):
            pATI->Chip = ATI_CHIP_264LTPRO;
            pATI->BusType = ATI_BUS_PCI;
            pATI->LCDVBlendFIFOSize = 800;
            break;

        case OldChipID('L', 'B'):
        case OldChipID('L', 'D'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('L', 'B'):
        case NewChipID('L', 'D'):
            pATI->Chip = ATI_CHIP_264LTPRO;
            pATI->BusType = ATI_BUS_AGP;
            pATI->LCDVBlendFIFOSize = 800;
            break;

        case OldChipID('G', 'L'):
        case OldChipID('G', 'O'):
        case OldChipID('G', 'R'):
        case OldChipID('G', 'S'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('G', 'L'):
        case NewChipID('G', 'O'):
        case NewChipID('G', 'R'):
        case NewChipID('G', 'S'):
            pATI->Chip = ATI_CHIP_264XL;
            pATI->BusType = ATI_BUS_PCI;
            pATI->LCDVBlendFIFOSize = 1024;
            break;

        case OldChipID('G', 'M'):
        case OldChipID('G', 'N'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('G', 'M'):
        case NewChipID('G', 'N'):
            pATI->Chip = ATI_CHIP_264XL;
            pATI->BusType = ATI_BUS_AGP;
            pATI->LCDVBlendFIFOSize = 1024;
            break;

        case OldChipID('L', 'R'):
        case OldChipID('L', 'S'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('L', 'R'):
        case NewChipID('L', 'S'):
            pATI->Chip = ATI_CHIP_MOBILITY;
            pATI->BusType = ATI_BUS_PCI;
            pATI->LCDVBlendFIFOSize = 1024;
            break;

        case OldChipID('L', 'M'):
        case OldChipID('L', 'N'):
            pATI->ChipType = OldToNewChipID(pATI->ChipType);
        case NewChipID('L', 'M'):
        case NewChipID('L', 'N'):
            pATI->Chip = ATI_CHIP_MOBILITY;
            pATI->BusType = ATI_BUS_AGP;
            pATI->LCDVBlendFIFOSize = 1024;
            break;

        default:
            pATI->Chip = ATI_CHIP_Mach64;
            break;
    }
}
