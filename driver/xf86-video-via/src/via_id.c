/*
 * Copyright 2004-2005 The Unichrome Project  [unichrome.sf.net]
 *
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via_driver.h"
#include "via_id.h"

/*
 * Since we are going to hold a rather big structure with
 * basic card-id information, we might as well seperate this
 * into its own file.
 *
 */

/*
 * Known missing devices:
 * cle266:
 * Biostar M6VLQ Grand
 * Biostar M6VLQ Pro
 * PcChips M789CLU (with C3 onboard)
 * PcChips M791G
 * Soltek SL-B6A-F800 (C3 800Mhz onboard)
 * Soltek SL-B6A-F1000 (Qbic IQ3601 | C3 1Ghz onboard)
 * + loads of semi-embedded devices.
 *
 * km400:
 * ECS KM400-M
 * ECS KM400-M2
 * ECS KM400A-M2
 * PcChips M851G
 * PcChips M851AG
 * Soltek SL-B7C-FGR (Qbic EQ3704 | km400a)
 * Soyo SY-K7VMP
 * Soyo SY-K7VMP2
 *
 * k8m800:
 * Abit KV8-MAX3
 * Abit KV8
 * Biostar Ideq 210V (km400a)
 * Biostar M7VIZ
 * Chaintech MK8M800
 * Epox EP-8KMM5I (km400a)
 * MSI K8M Neo-V
 * PcChips M861G
 * Soltek SL-B9C-FGR (Qbic EQ3802-300P)
 * Soltek SL-K8M800I-R
 *
 * pm800:
 * Biostar Ideq 210M
 * Biostar P4VMA-M
 * Biostar P4M80-M7 (is this even a unichrome?)
 * PcChips M955G
 * PcChips M957G
 * Soltek SL-PM800I
 * Soltek SL-PM800I-R
 * Soltek SL-PM800
 * Soyo SY-P4VM800
 *
 */
/*
 * There's no reason for this to be known outside of via_id.o
 * Only a pointer to an single entry will ever be used outside.
 *
 */
static struct ViaCardIdStruct ViaCardId[] = {
    /* CLE266 */
    {"LT21 VA28",                             VIA_CLE266,  0x1019, 0x1B44, VIA_DEVICE_CRT},
    {"ECS G320",                              VIA_CLE266,  0x1019, 0xB320, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Asustek Terminator C3V",                VIA_CLE266,  0x1043, 0x8155, VIA_DEVICE_CRT},
    {"VIA VT3122 (CLE266)-EPIA M/MII/...",    VIA_CLE266,  0x1106, 0x3122, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    /* KM400 */
    {"ECS KM400-M2",                          VIA_KM400,   0x1019, 0x1842, VIA_DEVICE_CRT},
    {"Acer Aspire 135x",                      VIA_KM400,   0x1025, 0x0033, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"Asustek A7V8X-MX",                      VIA_KM400,   0x1043, 0x80ED, VIA_DEVICE_CRT},
    {"Asustek A7V8X-LA",                      VIA_KM400,   0x1043, 0x80F9, VIA_DEVICE_CRT},
    {"Asustek A7V8X-MX SE/A7V400-MX",         VIA_KM400,   0x1043, 0x8118, VIA_DEVICE_CRT},
    {"Asustek Terminator A7VT",               VIA_KM400,   0x1043, 0x813E, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"Mitac 8375X",                           VIA_KM400,   0x1071, 0x8375, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV}, /* aka UMAX 585T */
    {"Soltek SL-75MIV2",                      VIA_KM400,   0x1106, 0x0000, VIA_DEVICE_CRT}, /* VIA/0x0000 */
    {"VIA VT3205 (KM400)",                    VIA_KM400,   0x1106, 0x3205, VIA_DEVICE_CRT | VIA_DEVICE_TV}, /* borrowed by Soltek SL-B7C-FGR */ 
    {"VIA VT7205 (KM400A)",                   VIA_KM400,   0x1106, 0x7205, VIA_DEVICE_CRT}, /* borrowed by Biostar iDEQ 200V/Chaintech 7VIF4 */
    {"Shuttle FX43",                          VIA_KM400,   0x1297, 0xF643, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"Giga-byte 7VM400(A)M",                  VIA_KM400,   0x1458, 0xD000, VIA_DEVICE_CRT}, /* 7VM400M, GA-7VM400AM */
    {"MSI KM4(A)M-V",                         VIA_KM400,   0x1462, 0x7061, VIA_DEVICE_CRT}, /* aka "DFI KM400-MLV" */
    {"MSI PM8M2-V",                           VIA_KM400,   0x1462, 0x7071, VIA_DEVICE_CRT},
    {"MSI KM4(A)M-L",                         VIA_KM400,   0x1462, 0x7348, VIA_DEVICE_CRT},
    {"MSI PM8M2-V",                           VIA_KM400,   0x1462, 0x7071, VIA_DEVICE_CRT},
    {"Abit VA-10 (1)",                        VIA_KM400,   0x147B, 0x140B, VIA_DEVICE_CRT},
    {"Abit VA-10 (2)",                        VIA_KM400,   0x147B, 0x140C, VIA_DEVICE_CRT},
    {"Abit VA-20",                            VIA_KM400,   0x147B, 0x1411, VIA_DEVICE_CRT},
    {"Averatec 322x",                         VIA_KM400,   0x14FF, 0x030D, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"FIC K7M-400A",                          VIA_KM400,   0x1509, 0x9233, VIA_DEVICE_CRT},
    {"Biostar P4M800-M7",                     VIA_KM400,   0x1565, 0x1202, VIA_DEVICE_CRT},
    {"Uniwill 755CI",                         VIA_KM400,   0x1584, 0x800A, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV}, /* aka "Gericom hummer advance", "Maxdata M-Book 1200X" */
    {"Packard Bell Quasar2 (MSI MS6786)",     VIA_KM400,   0x1631, 0xD002, VIA_DEVICE_CRT},
    {"Epox EP-8KMM3I",                        VIA_KM400,   0x1695, 0x9023, VIA_DEVICE_CRT},
    {"ASRock Inc. K7VM2/3/4",                 VIA_KM400,   0x1849, 0x7205, VIA_DEVICE_CRT},
    {"ACorp KM400QP",                         VIA_KM400,   0x1915, 0x1100, VIA_DEVICE_CRT| VIA_DEVICE_TV},
    {"Soyo K7VME",                            VIA_KM400,   0xA723, 0x10FD, VIA_DEVICE_CRT},
    /* K8M800 */
    {"ZX-5360",                               VIA_K8M800,  0x1019, 0x0F60, VIA_DEVICE_CRT | VIA_DEVICE_LCD },
    {"ECS K8M800-M2",                         VIA_K8M800,  0x1019, 0x1828, VIA_DEVICE_CRT},
    {"Acer Aspire 136x",                      VIA_K8M800,  0x1025, 0x006E, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"Asustek K8V-MX",                        VIA_K8M800,  0x1043, 0x8129, VIA_DEVICE_CRT},
    {"Mitac 8399",                            VIA_K8M800,  0x1071, 0x8399, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV}, /* aka "pogolinux konabook 3100" */
    {"Mitac 8889",                            VIA_K8M800,  0x1071, 0x8889, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"PCChips M861G",                         VIA_K8M800,  0x1106, 0x0204, VIA_DEVICE_CRT}, 
    {"VIA VT3108 (K8M800)",                   VIA_K8M800,  0x1106, 0x3108, VIA_DEVICE_CRT}, /* borrowed by Asustek A8V-MX */ 
    {"Shuttle FX21",                          VIA_K8M800,  0x1297, 0x3052, VIA_DEVICE_CRT},
    {"Shuttle FX83",                          VIA_K8M800,  0x1297, 0xF683, VIA_DEVICE_CRT | VIA_DEVICE_TV},
    {"Sharp Actius AL27",                     VIA_K8M800,  0x13BD, 0x1044, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Giga-byte GA-K8VM800M",                 VIA_K8M800,  0x1458, 0xD000, VIA_DEVICE_CRT},
    {"MSI K8M Neo-V",                         VIA_K8M800,  0x1462, 0x0320, VIA_DEVICE_CRT},
    {"MSI K8MM-V",                            VIA_K8M800,  0x1462, 0x7142, VIA_DEVICE_CRT},
    {"MSI K8MM3-V",                           VIA_K8M800,  0x1462, 0x7181, VIA_DEVICE_CRT},
    {"MSI K8MM-ILSR",                         VIA_K8M800,  0x1462, 0x7410, VIA_DEVICE_CRT},
    {"Abit KV-80",                            VIA_K8M800,  0x147B, 0x1419, VIA_DEVICE_CRT},
    {"Abit KV-81",                            VIA_K8M800,  0x147B, 0x141A, VIA_DEVICE_CRT},
    {"Averatec 327x",                         VIA_K8M800,  0x14FF, 0x0315, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Twinhead N14RA",                        VIA_K8M800,  0x14FF, 0x0321, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Averatec 3715",                         VIA_K8M800,  0x14FF, 0x0322, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Averatec 54xx",                         VIA_K8M800,  0x1509, 0x3930, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"FIC K8M-800M",                          VIA_K8M800,  0x1509, 0x6001, VIA_DEVICE_CRT},
    {"Biostar K8VGA-M",                       VIA_K8M800,  0x1565, 0x1203, VIA_DEVICE_CRT},
    {"DFI K8M800-MLVF",                       VIA_K8M800,  0x15BD, 0x1002, VIA_DEVICE_CRT},
    {"Packard Bell Easynote E6116/E63xx",     VIA_K8M800,  0x1631, 0xC008, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"Packard Bell Easynote B3 800/B3340",    VIA_K8M800,  0x1631, 0xC009, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"ASRock K8Upgrade-VM800",                VIA_K8M800,  0x1849, 0x3108, VIA_DEVICE_CRT},
    /* PM800 */
    {"VIA VT3118 (PM800)",                    VIA_PM800,   0x1106, 0x3118, VIA_DEVICE_CRT}, /* borrowed by ECS PM800-M2 */
    {"Mitac 8666",                            VIA_PM800,   0x1071, 0x8666, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"Medion MIM2080",                        VIA_PM800,   0x1071, 0x8965, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"MaxSelect Optima C4",                   VIA_PM800,   0x1558, 0x5402, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Biostar P4VMA-M",                       VIA_PM800,   0x1565, 0x1202, VIA_DEVICE_CRT},
    {"Packard Bell Easynote R1100",           VIA_PM800,   0x1631, 0xC015, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Fujitsu/Siemens Amilo Pro V2010",       VIA_PM800,   0x1734, 0x1078, VIA_DEVICE_CRT | VIA_DEVICE_LCD | VIA_DEVICE_TV},
    {"ASRock P4VM8",                          VIA_PM800,   0x1849, 0x3118, VIA_DEVICE_CRT},
    {"Chaintech MPM800-3",                    VIA_PM800,   0x270F, 0x7671, VIA_DEVICE_CRT},
    {"MaxSelect Optima C4",                   VIA_PM800,   0x1558, 0x5402, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    /* VN800 */
    {"Gateway MX3210",                        VIA_VM800,   0x107B, 0x0216, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"Gigabyte GA-8VM800M-775",               VIA_VM800,   0x1458, 0xD000, VIA_DEVICE_CRT},
    {"MSI PM8M-V",                            VIA_VM800,   0x1462, 0x7104, VIA_DEVICE_CRT},
    {"MSI PM8PM",                             VIA_VM800,   0x1462, 0x7222, VIA_DEVICE_CRT},
    {"Biostar P4M80-M4",                      VIA_VM800,   0x1565, 0x1202, VIA_DEVICE_CRT},
    {"Fujitsu/Siemens Amilo Pro V2030",       VIA_VM800,   0x1734, 0x109B, VIA_DEVICE_CRT | VIA_DEVICE_LCD},
    {"ASRock P4VM800",                        VIA_VM800,   0x1849, 0x3344, VIA_DEVICE_CRT},
    {"Asustek P5V800-MX",                     VIA_VM800,   0x3344, 0x1122, VIA_DEVICE_CRT},
    /* keep this */
    {NULL,                                    VIA_UNKNOWN, 0x0000, 0x0000, VIA_DEVICE_NONE}
};

/*
 * Fancy little routine stolen from fb
 * We don't do anything but warn really.
 */
void
ViaDoubleCheckCLE266Revision(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    VIAPtr pVia = VIAPTR(pScrn);
    /* Crtc 0x4F is only defined in CLE266Cx */
    CARD8 tmp = hwp->readCrtc(hwp, 0x4F);
    
    hwp->writeCrtc(hwp, 0x4F, 0x55);
    
    if (hwp->readCrtc(hwp, 0x4F) == 0x55) {
	if (CLE266_REV_IS_AX(pVia->ChipRev))
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "CLE266 Revision seems"
		       " to be Cx, yet %d was detected previously.\n", pVia->ChipRev);
    } else {
	if (CLE266_REV_IS_CX(pVia->ChipRev))
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "CLE266 Revision seems"
		       " to be Ax, yet %d was detected previously.\n", pVia->ChipRev);
    }
    hwp->writeCrtc(hwp, 0x4F, tmp);
}

/*
 *
 */
void
ViaCheckCardId(ScrnInfoPtr pScrn)
{
    struct ViaCardIdStruct *Id;
    VIAPtr pVia = VIAPTR(pScrn);
    
    if ((pVia->PciInfo->subsysVendor == pVia->PciInfo->vendor) &&
	(pVia->PciInfo->subsysCard == pVia->PciInfo->chipType))
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "Manufacturer plainly copied main PCI"
		   " ids to Subsystem/Card ids.\n");
    
    for (Id = ViaCardId; Id->String; Id++) {
	if ((Id->Chip == pVia->Chipset) && 
	    (Id->Vendor == pVia->PciInfo->subsysVendor) &&
	    (Id->Device == pVia->PciInfo->subsysCard)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "Detected %s.\n", Id->String);
	    pVia->Id = Id;
	    return;
	}
    }
    
    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, 
	       "Unknown Card-Ids (%4X|%4X), report this to the driver maintainer ASAP\n"
	       , pVia->PciInfo->subsysVendor, pVia->PciInfo->subsysCard);
    pVia->Id = NULL;
}

