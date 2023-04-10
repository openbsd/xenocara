/*
 * Copyright 2010 NVIDIA Corporation
 * Copyright 1996-1997  David J. McKay
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * DAVID J. MCKAY BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Hacked together from mga driver and 3.3.4 NVIDIA driver by Jarno Paananen
   <jpaana@s2.org> */

#include "nv_include.h"

#include "xf86int10.h"
#include "vbeModes.h"

const   OptionInfoRec * RivaAvailableOptions(int chipid, int busid);
Bool    RivaGetScrnInfoRec(PciChipsets *chips, int chip);
Bool    G80GetScrnInfoRec(PciChipsets *chips, int chip);

/*
 * Forward definitions for the functions that make up the driver.
 */
/* Mandatory functions */
static const OptionInfoRec * NVAvailableOptions(int chipid, int busid);
static void    NVIdentify(int flags);
#if XSERVER_LIBPCIACCESS
static Bool    NVPciProbe(DriverPtr, int entity, struct pci_device*, intptr_t data);
#else
static Bool    NVProbe(DriverPtr drv, int flags);
#endif
static Bool    NVPreInit(ScrnInfoPtr pScrn, int flags);
static Bool    NVScreenInit(SCREEN_INIT_ARGS_DECL);
static Bool    NVEnterVT(VT_FUNC_ARGS_DECL);
static Bool    NVEnterVTFBDev(VT_FUNC_ARGS_DECL);
static void    NVLeaveVT(VT_FUNC_ARGS_DECL);
static Bool    NVCloseScreen(CLOSE_SCREEN_ARGS_DECL);
static Bool    NVSaveScreen(ScreenPtr pScreen, int mode);

/* Optional functions */
static void    NVFreeScreen(FREE_SCREEN_ARGS_DECL);
static ModeStatus NVValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode,
			      Bool verbose, int flags);
#ifdef RANDR
static Bool    NVDriverFunc(ScrnInfoPtr pScrnInfo, xorgDriverFuncOp op,
			      pointer data);
#endif

/* Internally used functions */

static Bool	NVMapMem(ScrnInfoPtr pScrn);
static Bool	NVMapMemFBDev(ScrnInfoPtr pScrn);
static Bool	NVUnmapMem(ScrnInfoPtr pScrn);
static void	NVSave(ScrnInfoPtr pScrn);
static void	NVSaveRestoreVBE(ScrnInfoPtr, vbeSaveRestoreFunction);
static void	NVRestore(ScrnInfoPtr pScrn);
static Bool	NVModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode);
static Bool	NVSetModeVBE(ScrnInfoPtr pScrn, DisplayModePtr pMode);

#if XSERVER_LIBPCIACCESS
/* For now, just match any NVIDIA display device and sort through them in the
 * probe routine */

/*
 * libpciaccess's masks are shifted by 8 bits compared to the ones in xf86Pci.h.
 */
#define LIBPCIACCESS_CLASS_SHIFT (PCI_CLASS_SHIFT - 8)
#define LIBPCIACCESS_CLASS_MASK (PCI_CLASS_MASK >> 8)

static const struct pci_id_match NVPciIdMatchList[] = {
    { PCI_VENDOR_NVIDIA, PCI_MATCH_ANY, PCI_MATCH_ANY, PCI_MATCH_ANY,
      PCI_CLASS_DISPLAY << LIBPCIACCESS_CLASS_SHIFT, LIBPCIACCESS_CLASS_MASK, 0 },

    { PCI_VENDOR_NVIDIA_SGS, PCI_MATCH_ANY, PCI_MATCH_ANY, PCI_MATCH_ANY,
      PCI_CLASS_DISPLAY << LIBPCIACCESS_CLASS_SHIFT, LIBPCIACCESS_CLASS_MASK, 0 },

    { 0, 0, 0 }
};
#endif

/*
 * This contains the functions needed by the server after loading the
 * driver module.  It must be supplied, and gets added the driver list by
 * the Module Setup function in the dynamic case.  In the static case a
 * reference to this is compiled in, and this requires that the name of
 * this DriverRec be an upper-case version of the driver name.
 */

_X_EXPORT DriverRec NV = {
        NV_VERSION,
	NV_DRIVER_NAME,
        NVIdentify,
#if XSERVER_LIBPCIACCESS
        NULL,
#else
        NVProbe,
#endif
	NVAvailableOptions,
        NULL,
        0,
        NULL,
#if XSERVER_LIBPCIACCESS
        NVPciIdMatchList,
        NVPciProbe,
#endif
};

/* Known cards as of 2010/07/19 */

static SymTabRec NVKnownChipsets[] =
{
  { 0x12D20018, "RIVA 128" },

  { 0x10DE0020, "RIVA TNT" },

  { 0x10DE0028, "RIVA TNT2" },
  { 0x10DE002A, "Unknown TNT2" },
  { 0x10DE002C, "Vanta" },
  { 0x10DE0029, "RIVA TNT2 Ultra" },
  { 0x10DE002D, "RIVA TNT2 Model 64" },

  { 0x10DE00A0, "Aladdin TNT2" },

  { 0x10DE0100, "GeForce 256" },
  { 0x10DE0101, "GeForce DDR" },
  { 0x10DE0103, "Quadro" },

  { 0x10DE0110, "GeForce2 MX/MX 400" },
  { 0x10DE0111, "GeForce2 MX 100/200" },
  { 0x10DE0112, "GeForce2 Go" },
  { 0x10DE0113, "Quadro2 MXR/EX/Go" },

  { 0x10DE01A0, "GeForce2 Integrated GPU" },

  { 0x10DE0150, "GeForce2 GTS" },
  { 0x10DE0151, "GeForce2 Ti" },
  { 0x10DE0152, "GeForce2 Ultra" },
  { 0x10DE0153, "Quadro2 Pro" },

  { 0x10DE0170, "GeForce4 MX 460" },
  { 0x10DE0171, "GeForce4 MX 440" },
  { 0x10DE0172, "GeForce4 MX 420" },
  { 0x10DE0173, "GeForce4 MX 440-SE" },
  { 0x10DE0174, "GeForce4 440 Go" },
  { 0x10DE0175, "GeForce4 420 Go" },
  { 0x10DE0176, "GeForce4 420 Go 32M" },
  { 0x10DE0177, "GeForce4 460 Go" },
  { 0x10DE0178, "Quadro4 550 XGL" },
#if defined(__powerpc__)
  { 0x10DE0179, "GeForce4 MX (Mac)" },
#else
  { 0x10DE0179, "GeForce4 440 Go 64M" },
#endif
  { 0x10DE017A, "Quadro NVS" },
  { 0x10DE017C, "Quadro4 500 GoGL" },
  { 0x10DE017D, "GeForce4 410 Go 16M" },

  { 0x10DE0181, "GeForce4 MX 440 with AGP8X" },
  { 0x10DE0182, "GeForce4 MX 440SE with AGP8X" },
  { 0x10DE0183, "GeForce4 MX 420 with AGP8X" },
  { 0x10DE0185, "GeForce4 MX 4000" },
  { 0x10DE0186, "GeForce4 448 Go" },
  { 0x10DE0187, "GeForce4 488 Go" },
  { 0x10DE0188, "Quadro4 580 XGL" },
#if defined(__powerpc__)
  { 0x10DE0189, "GeForce4 MX with AGP8X (Mac)" },
#endif
  { 0x10DE018A, "Quadro4 NVS 280 SD" },
  { 0x10DE018B, "Quadro4 380 XGL" },
  { 0x10DE018C, "Quadro NVS 50 PCI" },
  { 0x10DE018D, "GeForce4 448 Go" },

  { 0x10DE01F0, "GeForce4 MX Integrated GPU" },

  { 0x10DE0200, "GeForce3" },
  { 0x10DE0201, "GeForce3 Ti 200" },
  { 0x10DE0202, "GeForce3 Ti 500" },
  { 0x10DE0203, "Quadro DCC" },

  { 0x10DE0250, "GeForce4 Ti 4600" },
  { 0x10DE0251, "GeForce4 Ti 4400" },
  { 0x10DE0253, "GeForce4 Ti 4200" },
  { 0x10DE0258, "Quadro4 900 XGL" },
  { 0x10DE0259, "Quadro4 750 XGL" },
  { 0x10DE025B, "Quadro4 700 XGL" },

  { 0x10DE0280, "GeForce4 Ti 4800" },
  { 0x10DE0281, "GeForce4 Ti 4200 with AGP8X" },
  { 0x10DE0282, "GeForce4 Ti 4800 SE" },
  { 0x10DE0286, "GeForce4 4200 Go" },
  { 0x10DE028C, "Quadro4 700 GoGL" },
  { 0x10DE0288, "Quadro4 980 XGL" },
  { 0x10DE0289, "Quadro4 780 XGL" },

  { 0x10DE0301, "GeForce FX 5800 Ultra" },
  { 0x10DE0302, "GeForce FX 5800" },
  { 0x10DE0308, "Quadro FX 2000" },
  { 0x10DE0309, "Quadro FX 1000" },

  { 0x10DE0311, "GeForce FX 5600 Ultra" },
  { 0x10DE0312, "GeForce FX 5600" },
  { 0x10DE0314, "GeForce FX 5600XT" },
  { 0x10DE031A, "GeForce FX Go5600" },
  { 0x10DE031B, "GeForce FX Go5650" },
  { 0x10DE031C, "Quadro FX Go700" },

  { 0x10DE0320, "GeForce FX 5200" },
  { 0x10DE0321, "GeForce FX 5200 Ultra" },
  { 0x10DE0322, "GeForce FX 5200" },
  { 0x10DE0323, "GeForce FX 5200LE" },
  { 0x10DE0324, "GeForce FX Go5200" },
  { 0x10DE0325, "GeForce FX Go5250" },
  { 0x10DE0326, "GeForce FX 5500" },
  { 0x10DE0327, "GeForce FX 5100" },
  { 0x10DE0328, "GeForce FX Go5200 32M/64M" },
#if defined(__powerpc__)
  { 0x10DE0329, "GeForce FX 5200 (Mac)" },
#endif
  { 0x10DE032A, "Quadro NVS 55/280 PCI" },
  { 0x10DE032B, "Quadro FX 500/600 PCI" },
  { 0x10DE032C, "GeForce FX Go53xx Series" },
  { 0x10DE032D, "GeForce FX Go5100" },

  { 0x10DE0330, "GeForce FX 5900 Ultra" },
  { 0x10DE0331, "GeForce FX 5900" },
  { 0x10DE0332, "GeForce FX 5900XT" },
  { 0x10DE0333, "GeForce FX 5950 Ultra" },
  { 0x10DE0334, "GeForce FX 5900ZT" },
  { 0x10DE0338, "Quadro FX 3000" },
  { 0x10DE033F, "Quadro FX 700" },

  { 0x10DE0341, "GeForce FX 5700 Ultra" },
  { 0x10DE0342, "GeForce FX 5700" },
  { 0x10DE0343, "GeForce FX 5700LE" },
  { 0x10DE0344, "GeForce FX 5700VE" },
  { 0x10DE0347, "GeForce FX Go5700" },
  { 0x10DE0348, "GeForce FX Go5700" },
  { 0x10DE034C, "Quadro FX Go1000" },
  { 0x10DE034E, "Quadro FX 1100" },

  { 0x10DE0040, "GeForce 6800 Ultra" },
  { 0x10DE0041, "GeForce 6800" },
  { 0x10DE0042, "GeForce 6800 LE" },
  { 0x10DE0043, "GeForce 6800 XE" },
  { 0x10DE0044, "GeForce 6800 XT" },
  { 0x10DE0045, "GeForce 6800 GT" },
  { 0x10DE0046, "GeForce 6800 GT" },
  { 0x10DE0047, "GeForce 6800 GS" },
  { 0x10DE0048, "GeForce 6800 XT" },
  { 0x10DE004E, "Quadro FX 4000" },

  { 0x10DE00C0, "GeForce 6800 GS" },
  { 0x10DE00C1, "GeForce 6800" },
  { 0x10DE00C2, "GeForce 6800 LE" },
  { 0x10DE00C3, "GeForce 6800 XT" },
  { 0x10DE00C8, "GeForce Go 6800" },
  { 0x10DE00C9, "GeForce Go 6800 Ultra" },
  { 0x10DE00CC, "Quadro FX Go1400" },
  { 0x10DE00CD, "Quadro FX 3450/4000 SDI" },
  { 0x10DE00CE, "Quadro FX 1400" },

  { 0x10DE0140, "GeForce 6600 GT" },
  { 0x10DE0141, "GeForce 6600" },
  { 0x10DE0142, "GeForce 6600 LE" },
  { 0x10DE0143, "GeForce 6600 VE" },
  { 0x10DE0144, "GeForce Go 6600" },
  { 0x10DE0145, "GeForce 6610 XL" },
  { 0x10DE0146, "GeForce Go 6600 TE/6200 TE" },
  { 0x10DE0147, "GeForce 6700 XL" },
  { 0x10DE0148, "GeForce Go 6600" },
  { 0x10DE0149, "GeForce Go 6600 GT" },
  { 0x10DE014A, "Quadro NVS 440" },
  { 0x10DE014C, "Quadro FX 550" },
  { 0x10DE014D, "Quadro FX 550" },
  { 0x10DE014E, "Quadro FX 540" },
  { 0x10DE014F, "GeForce 6200" },

  { 0x10DE0160, "GeForce 6500" },
  { 0x10DE0161, "GeForce 6200 TurboCache(TM)" },
  { 0x10DE0162, "GeForce 6200SE TurboCache(TM)" },
  { 0x10DE0163, "GeForce 6200 LE" },
  { 0x10DE0164, "GeForce Go 6200" },
  { 0x10DE0165, "Quadro NVS 285" },
  { 0x10DE0166, "GeForce Go 6400" },
  { 0x10DE0167, "GeForce Go 6200" },
  { 0x10DE0168, "GeForce Go 6400" },
  { 0x10DE0169, "GeForce 6250" },
  { 0x10DE016A, "GeForce 7100 GS" },

  { 0x10DE0211, "GeForce 6800" },
  { 0x10DE0212, "GeForce 6800 LE" },
  { 0x10DE0215, "GeForce 6800 GT" },
  { 0x10DE0218, "GeForce 6800 XT" },

  { 0x10DE0221, "GeForce 6200" },
  { 0x10DE0222, "GeForce 6200 A-LE" },

  { 0x10DE0090, "GeForce 7800 GTX" },
  { 0x10DE0091, "GeForce 7800 GTX" },
  { 0x10DE0092, "GeForce 7800 GT" },
  { 0x10DE0093, "GeForce 7800 GS" },
  { 0x10DE0095, "GeForce 7800 SLI" },
  { 0x10DE0098, "GeForce Go 7800" },
  { 0x10DE0099, "GeForce Go 7800 GTX" },
  { 0x10DE009D, "Quadro FX 4500" },

  { 0x10DE01D0, "GeForce 7350 LE" },
  { 0x10DE01D1, "GeForce 7300 LE" },
  { 0x10DE01D3, "GeForce 7300 SE" },
  { 0x10DE01D6, "GeForce Go 7200" },
  { 0x10DE01D7, "GeForce Go 7300" },
  { 0x10DE01D8, "GeForce Go 7400" },
  { 0x10DE01D9, "GeForce Go 7400 GS" },
  { 0x10DE01DA, "Quadro NVS 110M" },
  { 0x10DE01DB, "Quadro NVS 120M" },
  { 0x10DE01DC, "Quadro FX 350M" },
  { 0x10DE01DD, "GeForce 7500 LE" },
  { 0x10DE01DE, "Quadro FX 350" },
  { 0x10DE01DF, "GeForce 7300 GS" },

  { 0x10DE0390, "GeForce 7650 GS" },
  { 0x10DE0391, "GeForce 7600 GT" },
  { 0x10DE0392, "GeForce 7600 GS" },
  { 0x10DE0393, "GeForce 7300 GT" },
  { 0x10DE0394, "GeForce 7600 LE" },
  { 0x10DE0395, "GeForce 7300 GT" },
  { 0x10DE0397, "GeForce Go 7700" },
  { 0x10DE0398, "GeForce Go 7600" },
  { 0x10DE0399, "GeForce Go 7600 GT"},
  { 0x10DE039A, "Quadro NVS 300M" },
  { 0x10DE039B, "GeForce Go 7900 SE" },
  { 0x10DE039C, "Quadro FX 550M" },
  { 0x10DE039E, "Quadro FX 560" },

  { 0x10DE0290, "GeForce 7900 GTX" },
  { 0x10DE0291, "GeForce 7900 GT" },
  { 0x10DE0292, "GeForce 7900 GS" },
  { 0x10DE0293, "GeForce 7950 GX2" },
  { 0x10DE0294, "GeForce 7950 GX2" },
  { 0x10DE0295, "GeForce 7950 GT"},
  { 0x10DE0297, "GeForce Go 7950 GTX" },
  { 0x10DE0298, "GeForce Go 7900 GS" },
  { 0x10DE0299, "GeForce Go 7900 GTX" },
  { 0x10DE029A, "Quadro FX 2500M" },
  { 0x10DE029B, "Quadro FX 1500M" },
  { 0x10DE029C, "Quadro FX 5500" },
  { 0x10DE029D, "Quadro FX 3500" },
  { 0x10DE029E, "Quadro FX 1500" },
  { 0x10DE029F, "Quadro FX 4500 X2" },

  { 0x10DE0240, "GeForce 6150" },
  { 0x10DE0241, "GeForce 6150 LE" },
  { 0x10DE0242, "GeForce 6100" },
  { 0x10DE0244, "GeForce Go 6150" },
  { 0x10DE0245, "Quadro NVS 210S / NVIDIA GeForce 6150LE" },
  { 0x10DE0247, "GeForce Go 6100" },

  { 0x10DE03D0, "GeForce 6150SE" },
  { 0x10DE03D1, "GeForce 6100 nForce 405" },
  { 0x10DE03D2, "GeForce 6100 nForce 400" },
  { 0x10DE03D5, "GeForce 6100 nForce 420" },

  { 0x10DE0531, "GeForce 7150M / nForce 630M" },
  { 0x10DE0533, "GeForce 7000M / nForce 610M" },
  { 0x10DE053A, "GeForce 7050 PV / nForce 630a" },
  { 0x10DE053B, "GeForce 7050 PV / nForce 630a" },
  { 0x10DE053E, "GeForce 7025 / nForce 630a" },

/*************** G8x ***************/
  { 0x10DE0191, "GeForce 8800 GTX" },
  { 0x10DE0193, "GeForce 8800 GTS" },
  { 0x10DE0194, "GeForce 8800 Ultra" },
  { 0x10DE019D, "Quadro FX 5600" },
  { 0x10DE019E, "Quadro FX 4600" },
  { 0x10DE0400, "GeForce 8600 GTS" },
  { 0x10DE0401, "GeForce 8600 GT" },
  { 0x10DE0402, "GeForce 8600 GT" },
  { 0x10DE0403, "GeForce 8600 GS" },
  { 0x10DE0404, "GeForce 8400 GS" },
  { 0x10DE0405, "GeForce 9500M GS" },
  { 0x10DE0406, "GeForce 8300 GS" },
  { 0x10DE0407, "GeForce 8600M GT" },
  { 0x10DE0408, "GeForce 9650M GS" },
  { 0x10DE0409, "GeForce 8700M GT" },
  { 0x10DE040A, "Quadro FX 370" },
  { 0x10DE040B, "Quadro NVS 320M" },
  { 0x10DE040C, "Quadro FX 570M" },
  { 0x10DE040D, "Quadro FX 1600M" },
  { 0x10DE040E, "Quadro FX 570" },
  { 0x10DE040F, "Quadro FX 1700" },
  { 0x10DE0410, "GeForce GT 330" },
  { 0x10DE0420, "GeForce 8400 SE" },
  { 0x10DE0421, "GeForce 8500 GT" },
  { 0x10DE0422, "GeForce 8400 GS" },
  { 0x10DE0423, "GeForce 8300 GS" },
  { 0x10DE0424, "GeForce 8400 GS" },
  { 0x10DE0425, "GeForce 8600M GS" },
  { 0x10DE0426, "GeForce 8400M GT" },
  { 0x10DE0427, "GeForce 8400M GS" },
  { 0x10DE0428, "GeForce 8400M G" },
  { 0x10DE0429, "Quadro NVS 140M" },
  { 0x10DE042A, "Quadro NVS 130M" },
  { 0x10DE042B, "Quadro NVS 135M" },
  { 0x10DE042C, "GeForce 9400 GT" },
  { 0x10DE042D, "Quadro FX 360M" },
  { 0x10DE042E, "GeForce 9300M G" },
  { 0x10DE042F, "Quadro NVS 290" },
  { 0x10DE05E0, "GeForce GTX 295" },
  { 0x10DE05E1, "GeForce GTX 280" },
  { 0x10DE05E2, "GeForce GTX 260" },
  { 0x10DE05E3, "GeForce GTX 285" },
  { 0x10DE05E6, "GeForce GTX 275" },
  { 0x10DE05EA, "GeForce GTX 260" },
  { 0x10DE05EB, "GeForce GTX 295" },
  { 0x10DE05ED, "Quadroplex 2200 D2" },
  { 0x10DE05F8, "Quadroplex 2200 S4" },
  { 0x10DE05F9, "Quadro CX" },
  { 0x10DE05FD, "Quadro FX 5800" },
  { 0x10DE05FE, "Quadro FX 4800" },
  { 0x10DE05FF, "Quadro FX 3800" },
  { 0x10DE0600, "GeForce 8800 GTS 512" },
  { 0x10DE0601, "GeForce 9800 GT" },
  { 0x10DE0602, "GeForce 8800 GT" },
  { 0x10DE0603, "GeForce GT 230" },
  { 0x10DE0604, "GeForce 9800 GX2" },
  { 0x10DE0605, "GeForce 9800 GT" },
  { 0x10DE0606, "GeForce 8800 GS" },
  { 0x10DE0607, "GeForce GTS 240" },
  { 0x10DE0608, "GeForce 9800M GTX" },
  { 0x10DE0609, "GeForce 8800M GTS" },
  { 0x10DE060A, "GeForce GTX 280M" },
  { 0x10DE060B, "GeForce 9800M GT" },
  { 0x10DE060C, "GeForce 8800M GTX" },
  { 0x10DE060D, "GeForce 8800 GS" },
  { 0x10DE060F, "GeForce GTX 285M" },
  { 0x10DE0610, "GeForce 9600 GSO" },
  { 0x10DE0611, "GeForce 8800 GT" },
  { 0x10DE0612, "GeForce 9800 GTX/9800 GTX+" },
  { 0x10DE0613, "GeForce 9800 GTX+" },
  { 0x10DE0614, "GeForce 9800 GT" },
  { 0x10DE0615, "GeForce GTS 250" },
  { 0x10DE0617, "GeForce 9800M GTX" },
  { 0x10DE0618, "GeForce GTX 260M" },
  { 0x10DE0619, "Quadro FX 4700 X2" },
  { 0x10DE061A, "Quadro FX 3700" },
  { 0x10DE061B, "Quadro VX 200" },
  { 0x10DE061C, "Quadro FX 3600M" },
  { 0x10DE061D, "Quadro FX 2800M" },
  { 0x10DE061E, "Quadro FX 3700M" },
  { 0x10DE061F, "Quadro FX 3800M" },
  { 0x10DE0622, "GeForce 9600 GT" },
  { 0x10DE0623, "GeForce 9600 GS" },
  { 0x10DE0625, "GeForce 9600 GSO 512" },
  { 0x10DE0626, "GeForce GT 130" },
  { 0x10DE0627, "GeForce GT 140" },
  { 0x10DE0628, "GeForce 9800M GTS" },
  { 0x10DE062A, "GeForce 9700M GTS" },
  { 0x10DE062B, "GeForce 9800M GS" },
  { 0x10DE062C, "GeForce 9800M GTS" },
  { 0x10DE062D, "GeForce 9600 GT" },
  { 0x10DE062E, "GeForce 9600 GT" },
  { 0x10DE0631, "GeForce GTS 160M" },
  { 0x10DE0632, "GeForce GTS 150M" },
  { 0x10DE0635, "GeForce 9600 GSO" },
  { 0x10DE0637, "GeForce 9600 GT" },
  { 0x10DE0638, "Quadro FX 1800" },
  { 0x10DE063A, "Quadro FX 2700M" },
  { 0x10DE0640, "GeForce 9500 GT" },
  { 0x10DE0641, "GeForce 9400 GT" },
  { 0x10DE0643, "GeForce 9500 GT" },
  { 0x10DE0644, "GeForce 9500 GS" },
  { 0x10DE0645, "GeForce 9500 GS" },
  { 0x10DE0646, "GeForce GT 120" },
  { 0x10DE0647, "GeForce 9600M GT" },
  { 0x10DE0648, "GeForce 9600M GS" },
  { 0x10DE0649, "GeForce 9600M GT" },
  { 0x10DE064A, "GeForce 9700M GT" },
  { 0x10DE064B, "GeForce 9500M G" },
  { 0x10DE064C, "GeForce 9650M GT" },
  { 0x10DE0651, "GeForce G 110M" },
  { 0x10DE0652, "GeForce GT 130M" },
  { 0x10DE0653, "GeForce GT 120M" },
  { 0x10DE0654, "GeForce GT 220M" },
  { 0x10DE0656, "GeForce 9650 S" },
  { 0x10DE0658, "Quadro FX 380" },
  { 0x10DE0659, "Quadro FX 580" },
  { 0x10DE065A, "Quadro FX 1700M" },
  { 0x10DE065B, "GeForce 9400 GT" },
  { 0x10DE065C, "Quadro FX 770M" },
  { 0x10DE06E0, "GeForce 9300 GE" },
  { 0x10DE06E1, "GeForce 9300 GS" },
  { 0x10DE06E2, "GeForce 8400" },
  { 0x10DE06E3, "GeForce 8400 SE" },
  { 0x10DE06E4, "GeForce 8400 GS" },
  { 0x10DE06E5, "GeForce 9300M GS" },
  { 0x10DE06E6, "GeForce G100" },
  { 0x10DE06E7, "GeForce 9300 SE" },
  { 0x10DE06E8, "GeForce 9200M GS" },
  { 0x10DE06E9, "GeForce 9300M GS" },
  { 0x10DE06EA, "Quadro NVS 150M" },
  { 0x10DE06EB, "Quadro NVS 160M" },
  { 0x10DE06EC, "GeForce G 105M" },
  { 0x10DE06EF, "GeForce G 103M" },
  { 0x10DE06F1, "GeForce G105M" },
  { 0x10DE06F8, "Quadro NVS 420" },
  { 0x10DE06F9, "Quadro FX 370 LP" },
  { 0x10DE06FA, "Quadro NVS 450" },
  { 0x10DE06FB, "Quadro FX 370M" },
  { 0x10DE06FD, "Quadro NVS 295" },
  { 0x10DE0844, "GeForce 9100M G" },
  { 0x10DE0845, "GeForce 8200M G" },
  { 0x10DE0846, "GeForce 9200" },
  { 0x10DE0847, "GeForce 9100" },
  { 0x10DE0848, "GeForce 8300" },
  { 0x10DE0849, "GeForce 8200" },
  { 0x10DE084A, "nForce 730a" },
  { 0x10DE084B, "GeForce 9200" },
  { 0x10DE084C, "nForce 980a/780a SLI" },
  { 0x10DE084D, "nForce 750a SLI" },
  { 0x10DE084F, "GeForce 8100 / nForce 720a" },
  { 0x10DE0860, "GeForce 9400" },
  { 0x10DE0861, "GeForce 9400" },
  { 0x10DE0862, "GeForce 9400M G" },
  { 0x10DE0863, "GeForce 9400M" },
  { 0x10DE0864, "GeForce 9300" },
  { 0x10DE0865, "ION" },
  { 0x10DE0866, "GeForce 9400M G" },
  { 0x10DE0867, "GeForce 9400" },
  { 0x10DE0868, "nForce 760i SLI" },
  { 0x10DE086A, "GeForce 9400" },
  { 0x10DE086C, "GeForce 9300 / nForce 730i" },
  { 0x10DE086D, "GeForce 9200" },
  { 0x10DE086E, "GeForce 9100M G" },
  { 0x10DE086F, "GeForce 8200M G" },
  { 0x10DE0870, "GeForce 9400M" },
  { 0x10DE0871, "GeForce 9200" },
  { 0x10DE0872, "GeForce G102M" },
  { 0x10DE0873, "GeForce G102M" },
  { 0x10DE0874, "ION" },
  { 0x10DE0876, "ION" },
  { 0x10DE087A, "GeForce 9400" },
  { 0x10DE087D, "ION" },
  { 0x10DE087E, "ION LE" },
  { 0x10DE087F, "ION LE" },
  { 0x10DE0A20, "GeForce GT 220" },
  { 0x10DE0A22, "GeForce 315" },
  { 0x10DE0A23, "GeForce 210" },
  { 0x10DE0A28, "GeForce GT 230M" },
  { 0x10DE0A29, "GeForce GT 330M" },
  { 0x10DE0A2A, "GeForce GT 230M" },
  { 0x10DE0A2B, "GeForce GT 330M" },
  { 0x10DE0A2C, "NVS 5100M" },
  { 0x10DE0A2D, "GeForce GT 320M" },
  { 0x10DE0A34, "GeForce GT 240M" },
  { 0x10DE0A35, "GeForce GT 325M" },
  { 0x10DE0A3C, "Quadro FX 880M" },
  { 0x10DE0A60, "GeForce G210" },
  { 0x10DE0A62, "GeForce 205" },
  { 0x10DE0A63, "GeForce 310" },
  { 0x10DE0A64, "ION" },
  { 0x10DE0A65, "GeForce 210" },
  { 0x10DE0A66, "GeForce 310" },
  { 0x10DE0A67, "GeForce 315" },
  { 0x10DE0A68, "GeForce G105M" },
  { 0x10DE0A69, "GeForce G105M" },
  { 0x10DE0A6A, "NVS 2100M" },
  { 0x10DE0A6C, "NVS 3100M" },
  { 0x10DE0A6E, "GeForce 305M" },
  { 0x10DE0A6F, "ION" },
  { 0x10DE0A70, "GeForce 310M" },
  { 0x10DE0A71, "GeForce 305M" },
  { 0x10DE0A72, "GeForce 310M" },
  { 0x10DE0A73, "GeForce 305M" },
  { 0x10DE0A74, "GeForce G210M" },
  { 0x10DE0A75, "GeForce 310M" },
  { 0x10DE0A78, "Quadro FX 380 LP" },
  { 0x10DE0A7C, "Quadro FX 380M" },
  { 0x10DE0CA0, "GeForce GT 330" },
  { 0x10DE0CA2, "GeForce GT 320" },
  { 0x10DE0CA3, "GeForce GT 240" },
  { 0x10DE0CA4, "GeForce GT 340" },
  { 0x10DE0CA7, "GeForce GT 330" },
  { 0x10DE0CA8, "GeForce GTS 260M" },
  { 0x10DE0CA9, "GeForce GTS 250M" },
  { 0x10DE0CAC, "GeForce 315" },
  { 0x10DE0CAF, "GeForce GT 335M" },
  { 0x10DE0CB0, "GeForce GTS 350M" },
  { 0x10DE0CB1, "GeForce GTS 360M" },
  { 0x10DE0CBC, "Quadro FX 1800M" },

  {-1, NULL}
};

#ifdef XFree86LOADER

static MODULESETUPPROTO(nvSetup);

static XF86ModuleVersionInfo nvVersRec =
{
    "nv",
    MODULEVENDORSTRING,
    MODINFOSTRING1,
    MODINFOSTRING2,
    XORG_VERSION_CURRENT,
    PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
    ABI_CLASS_VIDEODRV,                     /* This is a video driver */
    ABI_VIDEODRV_VERSION,
    MOD_CLASS_VIDEODRV,
    {0,0,0,0}
};

_X_EXPORT XF86ModuleData nvModuleData = { &nvVersRec, nvSetup, NULL };
#endif


typedef enum {
    OPTION_SW_CURSOR,
    OPTION_HW_CURSOR,
    OPTION_NOACCEL,
    OPTION_SHADOW_FB,
    OPTION_FBDEV,
    OPTION_ROTATE,
    OPTION_VIDEO_KEY,
    OPTION_FLAT_PANEL,
    OPTION_FP_DITHER,
    OPTION_CRTC_NUMBER,
    OPTION_FP_SCALE,
    OPTION_FP_TWEAK,
    OPTION_DUALHEAD,
} NVOpts;


static const OptionInfoRec NVOptions[] = {
    { OPTION_SW_CURSOR,         "SWcursor",     OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_HW_CURSOR,         "HWcursor",     OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_NOACCEL,           "NoAccel",      OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_SHADOW_FB,         "ShadowFB",     OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_FBDEV,             "UseFBDev",     OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_ROTATE,		"Rotate",	OPTV_ANYSTR,	{0}, FALSE },
    { OPTION_VIDEO_KEY,		"VideoKey",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_FLAT_PANEL,	"FlatPanel",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_FP_DITHER, 	"FPDither",	OPTV_BOOLEAN,	{0}, FALSE },
    { OPTION_CRTC_NUMBER,	"CrtcNumber",	OPTV_INTEGER,	{0}, FALSE },
    { OPTION_FP_SCALE,          "FPScale",      OPTV_BOOLEAN,   {0}, FALSE },
    { OPTION_FP_TWEAK,          "FPTweak",      OPTV_INTEGER,   {0}, FALSE },
    { OPTION_DUALHEAD,          "DualHead",     OPTV_BOOLEAN,   {0}, FALSE },
    { -1,                       NULL,           OPTV_NONE,      {0}, FALSE }
};

/*
 * This is intentionally screen-independent.  It indicates the binding
 * choice made in the first PreInit.
 */
static int pix24bpp = 0;


static Bool
NVGetRec(ScrnInfoPtr pScrn)
{
    /*
     * Allocate an NVRec, and hook it into pScrn->driverPrivate.
     * pScrn->driverPrivate is initialised to NULL, so we can check if
     * the allocation has already been done.
     */
    if (pScrn->driverPrivate != NULL)
        return TRUE;

    pScrn->driverPrivate = xnfcalloc(sizeof(NVRec), 1);
    /* Initialise it */

    return TRUE;
}

static void
NVFreeRec(ScrnInfoPtr pScrn)
{
    if (pScrn->driverPrivate == NULL)
        return;
    free(pScrn->driverPrivate);
    pScrn->driverPrivate = NULL;
}


#ifdef XFree86LOADER

static pointer
nvSetup(pointer module, pointer opts, int *errmaj, int *errmin)
{
    static Bool setupDone = FALSE;

    /* This module should be loaded only once, but check to be sure. */

    if (!setupDone) {
        setupDone = TRUE;
        xf86AddDriver(&NV, module,
#if XSERVER_LIBPCIACCESS
            HaveDriverFuncs
#else
            0
#endif
        );

        /*
         * The return value must be non-NULL on success even though there
         * is no TearDownProc.
         */
        return (pointer)1;
    } else {
        if (errmaj) *errmaj = LDR_ONCEONLY;
        return NULL;
    }
}


#endif /* XFree86LOADER */

static const OptionInfoRec *
NVAvailableOptions(int chipid, int busid)
{
    if(chipid == 0x12D20018) {
        return RivaAvailableOptions(chipid, busid);
    }
    
    return NVOptions;
}

/* Mandatory */
static void
NVIdentify(int flags)
{
    xf86PrintChipsets(NV_NAME, "driver for NVIDIA chipsets", NVKnownChipsets);
}


static Bool
NVGetScrnInfoRec(PciChipsets *chips, int chip)
{
    ScrnInfoPtr pScrn;

    pScrn = xf86ConfigPciEntity(NULL, 0, chip,
                                chips, NULL, NULL, NULL,
                                NULL, NULL);

    if(!pScrn) return FALSE;

    pScrn->driverVersion    = NV_VERSION;
    pScrn->driverName       = NV_DRIVER_NAME;
    pScrn->name             = NV_NAME;

#if !XSERVER_LIBPCIACCESS
    pScrn->Probe            = NVProbe;
#endif
    pScrn->PreInit          = NVPreInit;
    pScrn->ScreenInit       = NVScreenInit;
    pScrn->SwitchMode       = NVSwitchMode;
    pScrn->AdjustFrame      = NVAdjustFrame;
    pScrn->EnterVT          = NVEnterVT;
    pScrn->LeaveVT          = NVLeaveVT;
    pScrn->FreeScreen       = NVFreeScreen;
    pScrn->ValidMode        = NVValidMode;

    return TRUE;
}

#define MAX_CHIPS MAXSCREENS


static CARD32 
#if XSERVER_LIBPCIACCESS
NVGetPCIXpressChip (struct pci_device *dev)
#else
NVGetPCIXpressChip (pciVideoPtr pVideo)
#endif
{
    volatile CARD32 *regs;
#if XSERVER_LIBPCIACCESS
    uint32_t pciid, pcicmd;
    void *tmp;

    pci_device_cfg_read_u32(dev, &pcicmd, PCI_CMD_STAT_REG);
    pci_device_cfg_write_u32(dev, pcicmd | PCI_CMD_MEM_ENABLE,
                             PCI_CMD_STAT_REG);

    pci_device_map_range(dev, dev->regions[0].base_addr, 0x2000,
                         PCI_DEV_MAP_FLAG_WRITABLE, &tmp);
    regs = tmp;
    pciid = regs[0x1800/4];
    pci_device_unmap_range(dev, tmp, 0x2000);

    pci_device_cfg_write_u32(dev, pcicmd, PCI_CMD_STAT_REG);
#else
    CARD32 pciid, pcicmd;
    PCITAG Tag = ((pciConfigPtr)(pVideo->thisCard))->tag;

    pcicmd = pciReadLong(Tag, PCI_CMD_STAT_REG);
    pciWriteLong(Tag, PCI_CMD_STAT_REG, pcicmd | PCI_CMD_MEM_ENABLE);
    
    regs = xf86MapPciMem(-1, VIDMEM_MMIO, Tag, pVideo->memBase[0], 0x2000);

    pciid = regs[0x1800/4];

    xf86UnMapVidMem(-1, (pointer)regs, 0x2000);

    pciWriteLong(Tag, PCI_CMD_STAT_REG, pcicmd);
#endif

    if((pciid & 0x0000ffff) == 0x000010DE) 
       pciid = 0x10DE0000 | (pciid >> 16);
    else 
    if((pciid & 0xffff0000) == 0xDE100000) /* wrong endian */
       pciid = 0x10DE0000 | ((pciid << 8) & 0x0000ff00) |
                            ((pciid >> 8) & 0x000000ff);

    return pciid;
}

static Bool
NVIsG80(int chipType)
{
    switch(chipType & 0xfff0) {
        case 0x0190:
        case 0x0400:
        case 0x0420:
        case 0x05e0:
        case 0x05f0:
        case 0x0600:
        case 0x0610:
        case 0x0620:
        case 0x0630:
        case 0x0640:
        case 0x0650:
        case 0x06e0:
        case 0x06f0:
        case 0x0840:
        case 0x0850:
        case 0x0860:
        case 0x0870:
        case 0x0a20:
        case 0x0a30:
        case 0x0a60:
        case 0x0a70:
        case 0x0ca0:
        case 0x0cb0:
            return TRUE;
    }

    return FALSE;
}

static Bool
NVIsSupported(CARD32 id)
{
    /* look for a compatible devices which may be newer than
       the NVKnownChipsets list above.  */
    switch(id & 0xfff0) {
    case 0x0040:
    case 0x0090:
    case 0x00C0:
    case 0x0120:
    case 0x0140:
    case 0x0160:
    case 0x0170:
    case 0x0180:
    case 0x01D0:
    case 0x0210:
    case 0x0220:
    case 0x0240:
    case 0x0250:
    case 0x0280:
    case 0x0290:
    case 0x0300:
    case 0x0310:
    case 0x0320:
    case 0x0330:
    case 0x0340:
    case 0x0390:
    case 0x03D0:
    case 0x0530:
        return TRUE;
    }

    return FALSE;
}

/* Mandatory */
#if XSERVER_LIBPCIACCESS
static Bool
NVPciProbe(DriverPtr drv, int entity, struct pci_device *dev, intptr_t data)
{
    const CARD32 id = ((dev->device_id & 0xfff0) == 0x00F0 ||
                       (dev->device_id & 0xfff0) == 0x02E0) ?
                      NVGetPCIXpressChip(dev) : dev->vendor_id << 16 | dev->device_id;
    const char *name = xf86TokenToString(NVKnownChipsets, id);

    if (pci_device_has_kernel_driver(dev)) {
        xf86DrvMsg(0, X_ERROR,
                   NV_NAME ": The PCI device 0x%x (%s) at %2.2d@%2.2d:%2.2d:%1.1d has a kernel module claiming it.\n",
                   id, name, dev->bus, dev->domain, dev->dev, dev->func);
        xf86DrvMsg(0, X_ERROR,
                   NV_NAME ": This driver cannot operate until it has been unloaded.\n");
        return FALSE;
    }

    if(dev->vendor_id == PCI_VENDOR_NVIDIA && !name &&
       !NVIsSupported(id) && !NVIsG80(id)) {
        /* See if pci.ids knows what the heck this thing is */
        name = pci_device_get_device_name(dev);
        if(name)
            xf86DrvMsg(0, X_WARNING,
                       NV_NAME ": Ignoring unsupported device 0x%x (%s) at %2.2d@%2.2d:%2.2d:%1.1d\n",
                       id, name, dev->bus, dev->domain, dev->dev, dev->func);
        else
            xf86DrvMsg(0, X_WARNING,
                       NV_NAME ": Ignoring unsupported device 0x%x at %2.2d@%2.2d:%2.2d:%1.1d\n",
                       id, dev->bus, dev->domain, dev->dev, dev->func);
        return FALSE;
    }

    if(!name)
        name = pci_device_get_device_name(dev);
    if(!name)
        name = "Unknown GPU";

    xf86DrvMsg(0, X_PROBED,
               NV_NAME ": Found NVIDIA %s at %2.2d@%2.2d:%2.2d:%1.1d\n",
               name, dev->bus, dev->domain, dev->dev, dev->func);

    if(NVIsG80(id))
        return G80GetScrnInfoRec(NULL, entity);
    else if(dev->vendor_id == PCI_VENDOR_NVIDIA_SGS)
        return RivaGetScrnInfoRec(NULL, entity);
    else
        return NVGetScrnInfoRec(NULL, entity);
}
#else
static Bool
NVProbe(DriverPtr drv, int flags)
{
    int i;
    GDevPtr *devSections;
    int *usedChips;
    SymTabRec NVChipsets[MAX_CHIPS + 1];
    PciChipsets NVPciChipsets[MAX_CHIPS + 1];
    pciVideoPtr *ppPci;
    int numDevSections;
    int numUsed;
    Bool foundScreen = FALSE;


    if ((numDevSections = xf86MatchDevice(NV_DRIVER_NAME, &devSections)) <= 0) 
        return FALSE;  /* no matching device section */

    if (!(ppPci = xf86GetPciVideoInfo())) 
        return FALSE;  /* no PCI cards found */

    numUsed = 0;

    /* Create the NVChipsets and NVPciChipsets from found devices */
    while (*ppPci && (numUsed < MAX_CHIPS)) {
        if(((*ppPci)->vendor == PCI_VENDOR_NVIDIA_SGS) || 
           ((*ppPci)->vendor == PCI_VENDOR_NVIDIA)) 
        {
            SymTabRec *nvchips = NVKnownChipsets;
            int pciid = ((*ppPci)->vendor << 16) | (*ppPci)->chipType;
            int token = pciid;

            if(((token & 0xfff0) == 0x00F0) ||
               ((token & 0xfff0) == 0x02E0))
            {
                token = NVGetPCIXpressChip(*ppPci);
            }

            while(nvchips->name) {
               if(token == nvchips->token)
                  break;
               nvchips++;
            }

            if(nvchips->name ||
               ((*ppPci)->vendor == PCI_VENDOR_NVIDIA &&
                (NVIsSupported(token) || NVIsG80((*ppPci)->chipType)))) {
               NVChipsets[numUsed].token = pciid;
               NVChipsets[numUsed].name = nvchips->name ? nvchips->name : "Unknown NVIDIA chip";
               NVPciChipsets[numUsed].numChipset = pciid;
               NVPciChipsets[numUsed].PCIid = pciid;
               NVPciChipsets[numUsed].resList = RES_SHARED_VGA;
               numUsed++;
            }
        }
        ppPci++;
    }

    /* terminate the list */
    NVChipsets[numUsed].token = -1;
    NVChipsets[numUsed].name = NULL; 
    NVPciChipsets[numUsed].numChipset = -1;
    NVPciChipsets[numUsed].PCIid = -1;
    NVPciChipsets[numUsed].resList = RES_UNDEFINED;

    numUsed = xf86MatchPciInstances(NV_NAME, 0, NVChipsets, NVPciChipsets,
                                    devSections, numDevSections, drv,
                                    &usedChips);
                        
    if (numUsed <= 0) 
        return FALSE;

    if (flags & PROBE_DETECT)
	foundScreen = TRUE;
    else for (i = 0; i < numUsed; i++) {
        pciVideoPtr pPci;

        pPci = xf86GetPciInfoForEntity(usedChips[i]);
        if(pPci->vendor == PCI_VENDOR_NVIDIA_SGS) {
            if(RivaGetScrnInfoRec(NVPciChipsets, usedChips[i]))
                foundScreen = TRUE;
        } else if (NVIsG80(pPci->chipType)) {
            if(G80GetScrnInfoRec(NVPciChipsets, usedChips[i]))
                foundScreen = TRUE;
        } else {
            if(NVGetScrnInfoRec(NVPciChipsets, usedChips[i])) 
	        foundScreen = TRUE;
	}    
    }

    free(devSections);
    free(usedChips);

    return foundScreen;
}
#endif /* XSERVER_LIBPCIACCESS */

/* Usually mandatory */
Bool
NVSwitchMode(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);

    NVSync(pScrn);
    return NVModeInit(pScrn, mode);
}

static Bool
NVSwitchModeVBE(SWITCH_MODE_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    NVPtr pNv = NVPTR(pScrn);
    const Bool disableAccess = pNv->accessEnabled;

    if(disableAccess)
	pScrn->EnableDisableFBAccess(XF86_SCRN_ARG(pScrn), FALSE);

    NVSync(pScrn);
    if (!NVSetModeVBE(pScrn, mode))
        return FALSE;
    NVAdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));

    if(disableAccess)
        pScrn->EnableDisableFBAccess(XF86_SCRN_ARG(pScrn), TRUE);

    return TRUE;
}

/*
 * This function is used to initialize the Start Address - the first
 * displayed location in the video memory.
 */
/* Usually mandatory */
void 
NVAdjustFrame(ADJUST_FRAME_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    int startAddr;
    NVPtr pNv = NVPTR(pScrn);
    NVFBLayout *pLayout = &pNv->CurrentLayout;

    startAddr = (((y*pLayout->displayWidth)+x)*(pLayout->bitsPerPixel/8));
    NVSetStartAddress(pNv, startAddr);
}


/*
 * This is called when VT switching back to the X server.  Its job is
 * to reinitialise the video mode.
 *
 * We may wish to unmap video/MMIO memory too.
 */

/* Mandatory */
static Bool
NVEnterVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    NVPtr pNv = NVPTR(pScrn);

    if (!NVModeInit(pScrn, pScrn->currentMode))
        return FALSE;
    NVAdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));

    if(pNv->overlayAdaptor)
        NVResetVideo(pScrn);
    return TRUE;
}

static Bool
NVEnterVTFBDev(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    fbdevHWEnterVT(VT_FUNC_ARGS);
    return TRUE;
}

static Bool
NVEnterVTVBE(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);

    if (!NVSetModeVBE(pScrn, pScrn->currentMode))
        return FALSE;
    NVAdjustFrame(ADJUST_FRAME_ARGS(pScrn, 0, 0));
    return TRUE;
}

/*
 * This is called when VT switching away from the X server.  Its job is
 * to restore the previous (text) mode.
 *
 * We may wish to remap video/MMIO memory too.
 */

/* Mandatory */
static void
NVLeaveVT(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    NVPtr pNv = NVPTR(pScrn);

    NVSync(pScrn);
    NVRestore(pScrn);
    NVLockUnlock(pNv, 1);
}

static void
NVLeaveVTVBE(VT_FUNC_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);

    NVSync(pScrn);
    NVSaveRestoreVBE(pScrn, MODE_RESTORE);
}

static void 
NVBlockHandler (BLOCKHANDLER_ARGS_DECL)
{
    SCREEN_PTR(arg);
    ScrnInfoPtr   pScrnInfo = xf86ScreenToScrn(pScreen);
    NVPtr         pNv = NVPTR(pScrnInfo);

    if (pNv->DMAKickoffCallback)
        (*pNv->DMAKickoffCallback)(pScrnInfo);
    
    pScreen->BlockHandler = pNv->BlockHandler;
    (*pScreen->BlockHandler) (BLOCKHANDLER_ARGS);
    pScreen->BlockHandler = NVBlockHandler;

    if (pNv->VideoTimerCallback) 
        (*pNv->VideoTimerCallback)(pScrnInfo, currentTime.milliseconds);

}


/*
 * This is called at the end of each server generation.  It restores the
 * original (text) mode.  It should also unmap the video memory, and free
 * any per-generation data allocated by the driver.  It should finish
 * by unwrapping and calling the saved CloseScreen function.
 */

/* Mandatory */
static Bool
NVCloseScreen(CLOSE_SCREEN_ARGS_DECL)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    NVPtr pNv = NVPTR(pScrn);

    if (pScrn->vtSema) {
        if (!pNv->NoAccel)
            NVSync(pScrn);

        if (pNv->VBEDualhead) {
            NVSaveRestoreVBE(pScrn, MODE_RESTORE);
        } else {
            NVRestore(pScrn);
            NVLockUnlock(pNv, 1);
        }
    }

    NVUnmapMem(pScrn);
    vgaHWUnmapMem(pScrn);
#ifdef HAVE_XAA_H
    if (pNv->AccelInfoRec)
        XAADestroyInfoRec(pNv->AccelInfoRec);
#endif
    if (pNv->CursorInfoRec)
        xf86DestroyCursorInfoRec(pNv->CursorInfoRec);
    if (pNv->ShadowPtr)
        free(pNv->ShadowPtr);
    if (pNv->DGAModes)
        free(pNv->DGAModes);
    if (pNv->overlayAdaptor)
	free(pNv->overlayAdaptor);
    if (pNv->blitAdaptor)
        free(pNv->blitAdaptor);

    pScrn->vtSema = FALSE;
    pScreen->CloseScreen = pNv->CloseScreen;
    pScreen->BlockHandler = pNv->BlockHandler;
    return (*pScreen->CloseScreen)(CLOSE_SCREEN_ARGS);
}

static void
NVEnableDisableFBAccess(SCRN_ARG_TYPE arg, Bool enable)
{
    SCRN_INFO_PTR(arg);
    NVPtr pNv = NVPTR(pScrn);

    pNv->accessEnabled = enable;
    pNv->EnableDisableFBAccess(arg, enable);
}


/* Free up any persistent data structures */

/* Optional */
static void
NVFreeScreen(FREE_SCREEN_ARGS_DECL)
{
    SCRN_INFO_PTR(arg);
    /*
     * This only gets called when a screen is being deleted.  It does not
     * get called routinely at the end of a server generation.
     */
    if (xf86LoaderCheckSymbol("vgaHWFreeHWRec"))
	vgaHWFreeHWRec(pScrn);
    NVFreeRec(pScrn);
}


/* Checks if a mode is suitable for the selected chipset. */

/* Optional */
static ModeStatus
NVValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode, Bool verbose, int flags)
{
    SCRN_INFO_PTR(arg);
    NVPtr pNv = NVPTR(pScrn);

    if(pNv->fpWidth && pNv->fpHeight)
      if((pNv->fpWidth < mode->HDisplay) || (pNv->fpHeight < mode->VDisplay))
        return (MODE_PANEL);

    return (MODE_OK);
}

static void
nvProbeDDC(ScrnInfoPtr pScrn, int index)
{
    vbeInfoPtr pVbe;

    if (xf86LoadSubModule(pScrn, "vbe")) {
        pVbe = VBEInit(NULL,index);
        ConfiguredMonitor = vbeDoEDID(pVbe, NULL);
	vbeFree(pVbe);
    }
}


Bool NVI2CInit(ScrnInfoPtr pScrn)
{
    char *mod = "i2c";

    if (xf86LoadSubModule(pScrn, mod)) {

        mod = "ddc";
        if(xf86LoadSubModule(pScrn, mod)) {
            return NVDACi2cInit(pScrn);
        } 
    }

    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
              "Couldn't load %s module.  DDC probing can't be done\n", mod);

    return FALSE;
}


/* Copied from ddc/Property.c */
static DisplayModePtr
NVModesAdd(DisplayModePtr Modes, DisplayModePtr Additions)
{
    if (!Modes) {
        if (Additions)
            return Additions;
        else
            return NULL;
    }

    if (Additions) {
        DisplayModePtr Mode = Modes;

        while (Mode->next)
            Mode = Mode->next;
        
        Mode->next = Additions;
        Additions->prev = Mode;
    }

    return Modes;
}

/* Mandatory */
static Bool
NVPreInit(ScrnInfoPtr pScrn, int flags)
{
    NVPtr pNv;
    MessageType from;
    int i, max_width, max_height;
    ClockRangePtr clockRanges;
    const char *s;
    Bool config_mon_rates;

    if (flags & PROBE_DETECT) {
        EntityInfoPtr pEnt = xf86GetEntityInfo(pScrn->entityList[0]);

        if (!pEnt)
            return FALSE;

        i = pEnt->index;
        free(pEnt);

        nvProbeDDC(pScrn, i);
        return TRUE;
    }

    /*
     * Note: This function is only called once at server startup, and
     * not at the start of each server generation.  This means that
     * only things that are persistent across server generations can
     * be initialised here.  xf86Screens[] is (pScrn is a pointer to one
     * of these).  Privates allocated using xf86AllocateScrnInfoPrivateIndex()  
     * are too, and should be used for data that must persist across
     * server generations.
     *
     * Per-generation data should be allocated with
     * AllocateScreenPrivateIndex() from the ScreenInit() function.
     */

    /* Check the number of entities, and fail if it isn't one. */
    if (pScrn->numEntities != 1)
	return FALSE;

    /* Allocate the NVRec driverPrivate */
    if (!NVGetRec(pScrn)) {
	return FALSE;
    }
    pNv = NVPTR(pScrn);

    /* Get the entity, and make sure it is PCI. */
    pNv->pEnt = xf86GetEntityInfo(pScrn->entityList[0]);
    if (pNv->pEnt->location.type != BUS_PCI)
	return FALSE;
 
    /* Find the PCI info for this screen */
    pNv->PciInfo = xf86GetPciInfoForEntity(pNv->pEnt->index);
#if !XSERVER_LIBPCIACCESS
    pNv->PciTag = pciTag(pNv->PciInfo->bus, pNv->PciInfo->device,
			  pNv->PciInfo->func);
#endif

    pNv->Primary = xf86IsPrimaryPci(pNv->PciInfo);

    /* Initialize the card through int10 interface if needed */
    if (xf86LoadSubModule(pScrn, "int10")) {
#if !defined(__alpha__) && !defined(__powerpc__)
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Initializing int10\n");
        pNv->pInt = xf86InitInt10(pNv->pEnt->index);
#endif
    }
   
#ifndef XSERVER_LIBPCIACCESS
    xf86SetOperatingState(resVgaIo, pNv->pEnt->index, ResUnusedOpr);
    xf86SetOperatingState(resVgaMem, pNv->pEnt->index, ResDisableOpr);
#endif

    /* Set pScrn->monitor */
    pScrn->monitor = pScrn->confScreen->monitor;

    /*
     * Set the Chipset and ChipRev, allowing config file entries to
     * override.
     */
    if (pNv->pEnt->device->chipset && *pNv->pEnt->device->chipset) {
	pScrn->chipset = pNv->pEnt->device->chipset;
        pNv->Chipset = xf86StringToToken(NVKnownChipsets, pScrn->chipset);
        from = X_CONFIG;
    } else if (pNv->pEnt->device->chipID >= 0) {
	pNv->Chipset = pNv->pEnt->device->chipID;
	pScrn->chipset = (char *)xf86TokenToString(NVKnownChipsets, 
                                                   pNv->Chipset);
	from = X_CONFIG;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipID override: 0x%04X\n",
		   pNv->Chipset);
    } else {
	from = X_PROBED;
	pNv->Chipset = VENDOR_ID(pNv->PciInfo) << 16 | DEVICE_ID(pNv->PciInfo);

        if(((pNv->Chipset & 0xfff0) == 0x00F0) ||
           ((pNv->Chipset & 0xfff0) == 0x02E0))
        {
            pNv->Chipset = NVGetPCIXpressChip(pNv->PciInfo);
        }

	pScrn->chipset = (char *)xf86TokenToString(NVKnownChipsets, 
                                                   pNv->Chipset);
        if(!pScrn->chipset)
          pScrn->chipset = "Unknown NVIDIA chipset";
    }

    if (pNv->pEnt->device->chipRev >= 0) {
	pNv->ChipRev = pNv->pEnt->device->chipRev;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "ChipRev override: %d\n",
		   pNv->ChipRev);
    } else {
	pNv->ChipRev = CHIP_REVISION(pNv->PciInfo);
    }

    /*
     * This shouldn't happen because such problems should be caught in
     * NVProbe(), but check it just in case.
     */
    if (pScrn->chipset == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "ChipID 0x%04X is not recognised\n", pNv->Chipset);
	xf86FreeInt10(pNv->pInt);
	return FALSE;
    }
    if (pNv->Chipset < 0) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Chipset \"%s\" is not recognised\n", pScrn->chipset);
	xf86FreeInt10(pNv->pInt);
	return FALSE;
    }

    xf86DrvMsg(pScrn->scrnIndex, from, "Chipset: \"%s\"\n", pScrn->chipset);


    /*
     * The first thing we should figure out is the depth, bpp, etc.
     */

    if (!xf86SetDepthBpp(pScrn, 0, 0, 0, Support32bppFb)) {
	xf86FreeInt10(pNv->pInt);
	return FALSE;
    } else {
	/* Check that the returned depth is one we support */
	switch (pScrn->depth) {
            case 8:
            case 15:
            case 16:
            case 24:
                /* OK */
                break;
            default:
                xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Given depth (%d) is not supported by this driver\n",
                    pScrn->depth);
		xf86FreeInt10(pNv->pInt);
                return FALSE;
	}
    }
    xf86PrintDepthBpp(pScrn);

    /* Get the depth24 pixmap format */
    if (pScrn->depth == 24 && pix24bpp == 0)
	pix24bpp = xf86GetBppFromDepth(pScrn, 24);

    /*
     * This must happen after pScrn->display has been set because
     * xf86SetWeight references it.
     */
    if (pScrn->depth > 8) {
	/* The defaults are OK for us */
	rgb zeros = {0, 0, 0};

	if (!xf86SetWeight(pScrn, zeros, zeros)) {
	    xf86FreeInt10(pNv->pInt);
	    return FALSE;
	}
    }

    if (!xf86SetDefaultVisual(pScrn, -1)) {
	xf86FreeInt10(pNv->pInt);
	return FALSE;
    } else {
	/* We don't currently support DirectColor at > 8bpp */
	if (pScrn->depth > 8 && (pScrn->defaultVisual != TrueColor)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "Given default visual"
		       " (%s) is not supported at depth %d\n",
		       xf86GetVisualName(pScrn->defaultVisual), pScrn->depth);
	    xf86FreeInt10(pNv->pInt);
	    return FALSE;
	}
    }

    /* The vgahw module should be loaded here when needed */
    if (!xf86LoadSubModule(pScrn, "vgahw")) {
	xf86FreeInt10(pNv->pInt);
	return FALSE;
    }
    
    /*
     * Allocate a vgaHWRec
     */
    if (!vgaHWGetHWRec(pScrn)) {
	xf86FreeInt10(pNv->pInt);
	return FALSE;
    }
    vgaHWSetStdFuncs(VGAHWPTR(pScrn));

    /* We use a programmable clock */
    pScrn->progClock = TRUE;

    /* Collect all of the relevant option flags (fill in pScrn->options) */
    xf86CollectOptions(pScrn, NULL);

    /* Process the options */
    if (!(pNv->Options = malloc(sizeof(NVOptions))))
	return FALSE;
    memcpy(pNv->Options, NVOptions, sizeof(NVOptions));
    xf86ProcessOptions(pScrn->scrnIndex, pScrn->options, pNv->Options);

    /* Set the bits per RGB for 8bpp mode */
    if (pScrn->depth == 8)
	pScrn->rgbBits = 8;

    from = X_DEFAULT;
    pNv->HWCursor = TRUE;
    /*
     * The preferred method is to use the "hw cursor" option as a tri-state
     * option, with the default set above.
     */
    if (xf86GetOptValBool(pNv->Options, OPTION_HW_CURSOR, &pNv->HWCursor)) {
	from = X_CONFIG;
    }
    /* For compatibility, accept this too (as an override) */
    if (xf86ReturnOptValBool(pNv->Options, OPTION_SW_CURSOR, FALSE)) {
	from = X_CONFIG;
	pNv->HWCursor = FALSE;
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Using %s cursor\n",
		pNv->HWCursor ? "HW" : "SW");

    pNv->FpScale = TRUE;
    if (xf86GetOptValBool(pNv->Options, OPTION_FP_SCALE, &pNv->FpScale)) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Flat panel scaling %s\n",
                   pNv->FpScale ? "on" : "off");
    }
    if (xf86ReturnOptValBool(pNv->Options, OPTION_NOACCEL, FALSE)) {
	pNv->NoAccel = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "Acceleration disabled\n");
    }
    if (xf86ReturnOptValBool(pNv->Options, OPTION_SHADOW_FB, FALSE)) {
	pNv->ShadowFB = TRUE;
	pNv->NoAccel = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, 
		"Using \"Shadow Framebuffer\" - acceleration disabled\n");
    }
    if (xf86ReturnOptValBool(pNv->Options, OPTION_FBDEV, FALSE)) {
	pNv->FBDev = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, 
		"Using framebuffer device\n");
    }
    if (pNv->FBDev) {
	/* check for linux framebuffer device */
	if (!xf86LoadSubModule(pScrn, "fbdevhw")) {
	    xf86FreeInt10(pNv->pInt);
	    return FALSE;
	}
	
	if (!fbdevHWInit(pScrn, pNv->PciInfo, NULL)) {
	    xf86FreeInt10(pNv->pInt);
	    return FALSE;
	}
	pScrn->SwitchMode    = fbdevHWSwitchModeWeak();
	pScrn->AdjustFrame   = fbdevHWAdjustFrameWeak();
	pScrn->EnterVT       = NVEnterVTFBDev;
	pScrn->LeaveVT       = fbdevHWLeaveVTWeak();
	pScrn->ValidMode     = fbdevHWValidModeWeak();
    }
    pNv->Rotate = 0;
    pNv->RandRRotation = FALSE;
    if ((s = xf86GetOptValString(pNv->Options, OPTION_ROTATE))) {
      if(!xf86NameCmp(s, "CW")) {
	pNv->ShadowFB = TRUE;
	pNv->NoAccel = TRUE;
	pNv->HWCursor = FALSE;
	pNv->Rotate = 1;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, 
		"Rotating screen clockwise - acceleration disabled\n");
      } else
      if(!xf86NameCmp(s, "CCW")) {
	pNv->ShadowFB = TRUE;
	pNv->NoAccel = TRUE;
	pNv->HWCursor = FALSE;
	pNv->Rotate = -1;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, 
		"Rotating screen counter clockwise - acceleration disabled\n");
      } else
      if(!xf86NameCmp(s, "RandR")) {
#ifdef RANDR
	pNv->ShadowFB = TRUE;
	pNv->NoAccel = TRUE;
	pNv->HWCursor = FALSE;
	pNv->RandRRotation = TRUE;
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
		"Using RandR rotation - acceleration disabled\n");
#else
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"This driver was not compiled with support for the Resize and "
		"Rotate extension.  Cannot honor 'Option \"Rotate\" "
		"\"RandR\"'.\n");
#endif
      } else {
	xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, 
		"\"%s\" is not a valid value for Option \"Rotate\"\n", s);
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
		"Valid options are \"CW\", \"CCW\", and \"RandR\"\n");
      }
    }

    if(xf86GetOptValInteger(pNv->Options, OPTION_VIDEO_KEY, &(pNv->videoKey))) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "video key set to 0x%x\n",
                                pNv->videoKey);
    } else {
        pNv->videoKey =  (1 << pScrn->offset.red) | 
                          (1 << pScrn->offset.green) |
        (((pScrn->mask.blue >> pScrn->offset.blue) - 1) << pScrn->offset.blue); 
    }

    if (xf86GetOptValBool(pNv->Options, OPTION_FLAT_PANEL, &(pNv->FlatPanel))) {
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "forcing %s usage\n",
                   pNv->FlatPanel ? "DFP" : "CRTC");
    } else {
        pNv->FlatPanel = -1;   /* autodetect later */
    }

    pNv->FPDither = FALSE;
    if (xf86GetOptValBool(pNv->Options, OPTION_FP_DITHER, &(pNv->FPDither))) 
        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, "enabling flat panel dither\n");

    if (xf86GetOptValInteger(pNv->Options, OPTION_CRTC_NUMBER,
                             &pNv->CRTCnumber)) 
    {
	if((pNv->CRTCnumber < 0) || (pNv->CRTCnumber > 1)) {
           pNv->CRTCnumber = -1;
           xf86DrvMsg(pScrn->scrnIndex, X_CONFIG, 
                      "Invalid CRTC number.  Must be 0 or 1\n");
        }
    } else {
        pNv->CRTCnumber = -1; /* autodetect later */
    }


    if (xf86GetOptValInteger(pNv->Options, OPTION_FP_TWEAK, 
                             &pNv->PanelTweak))
    {
        pNv->usePanelTweak = TRUE;
    } else {
        pNv->usePanelTweak = FALSE;
    }

    if (xf86ReturnOptValBool(pNv->Options, OPTION_DUALHEAD, FALSE)) {
        if (pNv->FBDev)
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "FBDev and Dualhead are incompatible.\n");
        else
            pNv->VBEDualhead = TRUE;
    }

    if (pNv->VBEDualhead) {
        if (!xf86LoadSubModule(pScrn, "vbe")) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Couldn't load the VBE module and Dualhead is "
                       "enabled.\n");
            return FALSE;
        }
        pNv->pVbe = VBEExtendedInit(NULL, pNv->pEnt->index,
                                    SET_BIOS_SCRATCH | RESTORE_BIOS_SCRATCH);
        if (!pNv->pVbe) return FALSE;

        pNv->pVbeInfo = VBEGetVBEInfo(pNv->pVbe);
        if (!pNv->pVbeInfo) return FALSE;

        xf86DrvMsg(pScrn->scrnIndex, X_CONFIG,
                   "Using VBE dual-head mode.\n");

        xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                   "Using software cursor.\n");
        pNv->HWCursor = FALSE;

        pScrn->SwitchMode    = NVSwitchModeVBE;
        pScrn->EnterVT       = NVEnterVTVBE;
        pScrn->LeaveVT       = NVLeaveVTVBE;
        pScrn->ValidMode     = NULL;
    }

    if (pNv->pEnt->device->MemBase != 0) {
	/* Require that the config file value matches one of the PCI values. */
	if (!xf86CheckPciMemBase(pNv->PciInfo, pNv->pEnt->device->MemBase)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"MemBase 0x%08lX doesn't match any PCI base register.\n",
		pNv->pEnt->device->MemBase);
	    xf86FreeInt10(pNv->pInt);
	    NVFreeRec(pScrn);
	    return FALSE;
	}
	pNv->FbAddress = pNv->pEnt->device->MemBase;
	from = X_CONFIG;
    } else {
	if (MEMBASE(pNv->PciInfo, 1) != 0) {
	    pNv->FbAddress = MEMBASE(pNv->PciInfo, 1) & 0xff800000;
	    from = X_PROBED;
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			   "No valid FB address in PCI config space\n");
	    xf86FreeInt10(pNv->pInt);
	    NVFreeRec(pScrn);
	    return FALSE;
	}
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "Linear framebuffer at 0x%lX\n",
	       (unsigned long)pNv->FbAddress);

    if (pNv->pEnt->device->IOBase != 0) {
	/* Require that the config file value matches one of the PCI values. */
	if (!xf86CheckPciMemBase(pNv->PciInfo, pNv->pEnt->device->IOBase)) {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"IOBase 0x%08lX doesn't match any PCI base register.\n",
		pNv->pEnt->device->IOBase);
	    xf86FreeInt10(pNv->pInt);
	    NVFreeRec(pScrn);
	    return FALSE;
	}
	pNv->IOAddress = pNv->pEnt->device->IOBase;
	from = X_CONFIG;
    } else {
	if (MEMBASE(pNv->PciInfo, 0) != 0) {
	    pNv->IOAddress = MEMBASE(pNv->PciInfo, 0) & 0xffffc000;
	    from = X_PROBED;
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			"No valid MMIO address in PCI config space\n");
	    xf86FreeInt10(pNv->pInt);
	    NVFreeRec(pScrn);
	    return FALSE;
	}
    }
    xf86DrvMsg(pScrn->scrnIndex, from, "MMIO registers at 0x%lX\n",
	       (unsigned long)pNv->IOAddress);

#if defined(__powerpc__)
    vgaHWSetMmioFuncs(VGAHWPTR(pScrn), (CARD8 *)pNv->IOAddress, 0);
#endif

#ifndef XSERVER_LIBPCIACCESS
    if (xf86RegisterResources(pNv->pEnt->index, NULL, ResExclusive)) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"xf86RegisterResources() found resource conflicts\n");
	xf86FreeInt10(pNv->pInt);
	NVFreeRec(pScrn);
	return FALSE;
    }
#endif

    switch (pNv->Chipset & 0x0ff0) {
    case 0x0100:   /* GeForce 256 */
    case 0x0110:   /* GeForce2 MX */
    case 0x0150:   /* GeForce2 */
    case 0x0170:   /* GeForce4 MX */
    case 0x0180:   /* GeForce4 MX (8x AGP) */
    case 0x01A0:   /* nForce */
    case 0x01F0:   /* nForce2 */
         pNv->Architecture =  NV_ARCH_10;
         break;
    case 0x0200:   /* GeForce3 */
    case 0x0250:   /* GeForce4 Ti */
    case 0x0280:   /* GeForce4 Ti (8x AGP) */
         pNv->Architecture =  NV_ARCH_20;
         break;
    case 0x0300:   /* GeForce FX 5800 */
    case 0x0310:   /* GeForce FX 5600 */
    case 0x0320:   /* GeForce FX 5200 */
    case 0x0330:   /* GeForce FX 5900 */
    case 0x0340:   /* GeForce FX 5700 */
         pNv->Architecture =  NV_ARCH_30;
         break;
    case 0x0040:   /* GeForce 6800 */
    case 0x00C0:   /* GeForce 6800 */
    case 0x0120:   /* GeForce 6800 */
    case 0x0140:   /* GeForce 6600 */
    case 0x0160:   /* GeForce 6200 */
    case 0x01D0:   /* GeForce 7200, 7300, 7400 */
    case 0x0090:   /* GeForce 7800 */
    case 0x0210:   /* GeForce 6800 */
    case 0x0220:   /* GeForce 6200 */
    case 0x0290:   /* GeForce 7900 */
    case 0x0390:   /* GeForce 7600 */
    case 0x0240:   /* GeForce 6100 */
    case 0x0530:   /* GeForce 7050, 7025 */
    case 0x03D0:
         pNv->Architecture =  NV_ARCH_40;
         break;
    default:
         pNv->Architecture =  NV_ARCH_04;
         break;
    }

    pNv->alphaCursor = (pNv->Architecture >= NV_ARCH_10) &&
                       ((pNv->Chipset & 0x0ff0) != 0x0100);

    if ((pScrn->monitor->nHsync == 0) && 
	(pScrn->monitor->nVrefresh == 0))
	config_mon_rates = FALSE;
    else
	config_mon_rates = TRUE;

    NVCommonSetup(pScrn);

    if (pNv->FBDev) {
       pScrn->videoRam = fbdevHWGetVidmem(pScrn)/1024;
    } else {
       pScrn->videoRam = pNv->RamAmountKBytes;
    }
    xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "VideoRAM: %d kBytes\n",
               pScrn->videoRam);
	
    pNv->FbMapSize = pScrn->videoRam * 1024;

    /*
     * If the driver can do gamma correction, it should call xf86SetGamma()
     * here.
     */

    {
	Gamma zeros = {0.0, 0.0, 0.0};

	if (!xf86SetGamma(pScrn, zeros)) {
	    xf86FreeInt10(pNv->pInt);
	    return FALSE;
	}
    }

    if(pNv->Architecture >= NV_ARCH_40)
       pNv->FbUsableSize = pNv->FbMapSize - (560 * 1024);
    else
       pNv->FbUsableSize = pNv->FbMapSize - (128 * 1024);
    pNv->ScratchBufferSize = (pNv->Architecture < NV_ARCH_10) ? 8192 : 16384;
    pNv->ScratchBufferStart = pNv->FbUsableSize - pNv->ScratchBufferSize;
    pNv->CursorStart = pNv->FbUsableSize + (32 * 1024);

    /*
     * Setup the ClockRanges, which describe what clock ranges are available,
     * and what sort of modes they can be used for.
     */

    clockRanges = xnfcalloc(sizeof(ClockRange), 1);
    clockRanges->next = NULL;
    clockRanges->minClock = pNv->MinVClockFreqKHz;
    clockRanges->maxClock = pNv->MaxVClockFreqKHz;
    clockRanges->clockIndex = -1;		/* programmable */
    clockRanges->doubleScanAllowed = TRUE;
    if((pNv->Architecture == NV_ARCH_20) ||
         ((pNv->Architecture == NV_ARCH_10) && 
           ((pNv->Chipset & 0x0ff0) != 0x0100) &&
           ((pNv->Chipset & 0x0ff0) != 0x0150)))
    {
       /* HW is broken */
       clockRanges->interlaceAllowed = FALSE;
    } else {
       clockRanges->interlaceAllowed = TRUE;
    }

    if(pNv->FlatPanel == 1) {
       clockRanges->interlaceAllowed = FALSE;
       clockRanges->doubleScanAllowed = FALSE;
    }

    if(pNv->Architecture < NV_ARCH_10) {
       max_width = (pScrn->bitsPerPixel > 16) ? 2032 : 2048;
       max_height = 2048;
    } else {
       max_width = (pScrn->bitsPerPixel > 16) ? 4080 : 4096;
       max_height = 4096;
    }

    /* If DFP, add a modeline corresponding to its panel size */
    if (pNv->FlatPanel && !pNv->Television && pNv->fpWidth && pNv->fpHeight) {
	DisplayModePtr Mode;

	Mode = xf86CVTMode(pNv->fpWidth, pNv->fpHeight, 60.00, TRUE, FALSE);
	Mode->type = M_T_DRIVER;
	pScrn->monitor->Modes = NVModesAdd(pScrn->monitor->Modes, Mode);

	if (!config_mon_rates) {
	    if (!Mode->HSync)
            	Mode->HSync = ((float) Mode->Clock ) / ((float) Mode->HTotal);
            if (!Mode->VRefresh)
            	Mode->VRefresh = (1000.0 * ((float) Mode->Clock)) /
                    ((float) (Mode->HTotal * Mode->VTotal));

 	    if (Mode->HSync < pScrn->monitor->hsync[0].lo)
            	pScrn->monitor->hsync[0].lo = Mode->HSync;
            if (Mode->HSync > pScrn->monitor->hsync[0].hi)
            	pScrn->monitor->hsync[0].hi = Mode->HSync;
            if (Mode->VRefresh < pScrn->monitor->vrefresh[0].lo)
            	pScrn->monitor->vrefresh[0].lo = Mode->VRefresh;
            if (Mode->VRefresh > pScrn->monitor->vrefresh[0].hi)
            	pScrn->monitor->vrefresh[0].hi = Mode->VRefresh;

	    pScrn->monitor->nHsync = 1;
	    pScrn->monitor->nVrefresh = 1;
	}
    }

    /*
     * xf86ValidateModes will check that the mode HTotal and VTotal values
     * don't exceed the chipset's limit if pScrn->maxHValue and
     * pScrn->maxVValue are set.  Since our NVValidMode() already takes
     * care of this, we don't worry about setting them here.
     */
    if (pNv->VBEDualhead) {
        pScrn->modePool = VBEGetModePool(pScrn, pNv->pVbe, pNv->pVbeInfo,
                                         V_MODETYPE_VBE);

        VBESetModeNames(pScrn->modePool);
        i = VBEValidateModes(pScrn, pScrn->monitor->Modes,
                             pScrn->display->modes, clockRanges,
                             NULL, 256, max_width,
                             512, 128, max_height,
                             pScrn->display->virtualX,
                             pScrn->display->virtualY,
                             pNv->ScratchBufferStart,
                             LOOKUP_BEST_REFRESH);
        if (i > 0)
            VBESetModeParameters(pScrn, pNv->pVbe);
    } else {
        i = xf86ValidateModes(pScrn, pScrn->monitor->Modes,
                              pScrn->display->modes, clockRanges,
                              NULL, 256, max_width,
                              512, 128, max_height,
                              pScrn->display->virtualX,
                              pScrn->display->virtualY,
                              pNv->ScratchBufferStart,
                              LOOKUP_BEST_REFRESH);
    }

    if (i < 1 && pNv->FBDev) {
	fbdevHWUseBuildinMode(pScrn);
	pScrn->displayWidth = pScrn->virtualX; /* FIXME: might be wrong */
	i = 1;
    }
    if (i == -1) {
	xf86FreeInt10(pNv->pInt);
	NVFreeRec(pScrn);
	return FALSE;
    }

    /* Prune the modes marked as invalid */
    xf86PruneDriverModes(pScrn);

    if (i == 0 || pScrn->modes == NULL) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR, "No valid modes found\n");
	xf86FreeInt10(pNv->pInt);
	NVFreeRec(pScrn);
	return FALSE;
    }

    /*
     * Set the CRTC parameters for all of the modes based on the type
     * of mode, and the chipset's interlace requirements.
     *
     * Calling this is required if the mode->Crtc* values are used by the
     * driver and if the driver doesn't provide code to set them.  They
     * are not pre-initialised at all.
     */
    xf86SetCrtcForModes(pScrn, 0);

    if (pNv->VBEDualhead) {
        DisplayModePtr p = pScrn->modes;

        /*
         * Loop through modes and double their widths.  Stash the real width in
         * CrtcHDisplay.  Also adjust the screen dimensions.
         */
        do {
            p->CrtcHDisplay = p->HDisplay;
            p->HDisplay *= 2;
        } while ((p = p->next) != pScrn->modes);

        pScrn->virtualX *= 2;
        pScrn->displayWidth *= 2;
    }

    /* Set the current mode to the first in the list */
    pScrn->currentMode = pScrn->modes;

    /* Print the list of modes being used */
    xf86PrintModes(pScrn);

    /* Set display resolution */
    xf86SetDpi(pScrn, 0, 0);


    /*
     * XXX This should be taken into account in some way in the mode valdation
     * section.
     */

    if (xf86LoadSubModule(pScrn, "fb") == NULL) {
	xf86FreeInt10(pNv->pInt);
	NVFreeRec(pScrn);
	return FALSE;
    }

    /* Load XAA if needed */
    if (!pNv->NoAccel) {
	if (!xf86LoadSubModule(pScrn, "xaa")) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Falling back to shadwwfb\n");
	    pNv->NoAccel = 1;
	    pNv->ShadowFB = 1;
	}
    }

    /* Load ramdac if needed */
    if (pNv->HWCursor) {
	if (!xf86LoadSubModule(pScrn, "ramdac")) {
	    xf86FreeInt10(pNv->pInt);
	    NVFreeRec(pScrn);
	    return FALSE;
	}
    }

    /* Load shadowfb if needed */
    if (pNv->ShadowFB) {
	if (!xf86LoadSubModule(pScrn, "shadow")) {
	    xf86FreeInt10(pNv->pInt);
	    NVFreeRec(pScrn);
	    return FALSE;
	}
    }

    pNv->CurrentLayout.bitsPerPixel = pScrn->bitsPerPixel;
    pNv->CurrentLayout.depth = pScrn->depth;
    pNv->CurrentLayout.displayWidth = pScrn->displayWidth;
    pNv->CurrentLayout.weight.red = pScrn->weight.red;
    pNv->CurrentLayout.weight.green = pScrn->weight.green;
    pNv->CurrentLayout.weight.blue = pScrn->weight.blue;
    pNv->CurrentLayout.mode = pScrn->currentMode;

    xf86FreeInt10(pNv->pInt);

    pNv->pInt = NULL;
    return TRUE;
}


/*
 * Map the framebuffer and MMIO memory.
 */

static Bool
NVMapMem(ScrnInfoPtr pScrn)
{
    NVPtr pNv = NVPTR(pScrn);

#if XSERVER_LIBPCIACCESS
    void *tmp;

    pci_device_map_range(pNv->PciInfo, pNv->FbAddress, pNv->FbMapSize,
                         PCI_DEV_MAP_FLAG_WRITABLE |
                         PCI_DEV_MAP_FLAG_WRITE_COMBINE, &tmp);
    pNv->FbBase = tmp;
#else
    pNv->FbBase = xf86MapPciMem(pScrn->scrnIndex, VIDMEM_FRAMEBUFFER,
				 pNv->PciTag, pNv->FbAddress,
				 pNv->FbMapSize);
#endif
    if (pNv->FbBase == NULL)
	return FALSE;

    pNv->FbStart = pNv->FbBase;

    return TRUE;
}

static Bool
NVMapMemFBDev(ScrnInfoPtr pScrn)
{
    NVPtr pNv;

    pNv = NVPTR(pScrn);

    pNv->FbBase = fbdevHWMapVidmem(pScrn);
    if (pNv->FbBase == NULL)
        return FALSE;

    pNv->FbStart = pNv->FbBase;

    return TRUE;
}

/*
 * Unmap the framebuffer and MMIO memory.
 */

static Bool
NVUnmapMem(ScrnInfoPtr pScrn)
{
    NVPtr pNv;
    
    pNv = NVPTR(pScrn);

#if XSERVER_LIBPCIACCESS
    pci_device_unmap_range(pNv->PciInfo, pNv->FbBase, pNv->FbMapSize);
#else
    xf86UnMapVidMem(pScrn->scrnIndex, (pointer)pNv->FbBase, pNv->FbMapSize);
#endif
    pNv->FbBase = NULL;
    pNv->FbStart = NULL;

    return TRUE;
}


/*
 * Initialise a new mode. 
 */

static Bool
NVModeInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg;
    NVPtr pNv = NVPTR(pScrn);
    NVRegPtr nvReg;

    /* Initialise the ModeReg values */
    if (!vgaHWInit(pScrn, mode))
	return FALSE;
    pScrn->vtSema = TRUE;

    vgaReg = &hwp->ModeReg;
    nvReg = &pNv->ModeReg;

    if(!NVDACInit(pScrn, mode))
        return FALSE;

    NVLockUnlock(pNv, 0);
    if(pNv->twoHeads) {
        VGA_WR08(pNv->PCIO, 0x03D4, 0x44);
        VGA_WR08(pNv->PCIO, 0x03D5, nvReg->crtcOwner);
        NVLockUnlock(pNv, 0);
    }

    /* Program the registers */
    vgaHWProtect(pScrn, TRUE);

    NVDACRestore(pScrn, vgaReg, nvReg, FALSE);

#if X_BYTE_ORDER == X_BIG_ENDIAN
    /* turn on LFB swapping */
    {
	unsigned char tmp;

	VGA_WR08(pNv->PCIO, 0x3d4, 0x46);
	tmp = VGA_RD08(pNv->PCIO, 0x3d5);
	tmp |= (1 << 7);
	VGA_WR08(pNv->PCIO, 0x3d5, tmp);
    }
#endif

    NVResetGraphics(pScrn);

    vgaHWProtect(pScrn, FALSE);

    pNv->CurrentLayout.mode = mode;

    return TRUE;
}

static Bool
NVSetModeVBE(ScrnInfoPtr pScrn, DisplayModePtr pMode)
{
    NVPtr pNv = NVPTR(pScrn);
    VbeModeInfoData *data;
    int mode;

    data = (VbeModeInfoData*)pMode->Private;
    mode = data->mode | 1 << 14;

    if(!VBESetVBEMode(pNv->pVbe, mode, data->block))
        return FALSE;
    pNv->PCRTC0[0x820/4] = pNv->PCRTC0[0x2820/4] =
        pScrn->displayWidth * (pScrn->bitsPerPixel / 8);
    pNv->vbeCRTC1Offset = pMode->CrtcHDisplay * (pScrn->bitsPerPixel / 8);

    pScrn->vtSema = TRUE;

    NVLoadStateExt(pNv, NULL);
    NVResetGraphics(pScrn);

    pNv->CurrentLayout.mode = pMode;

    return TRUE;
}

/*
 * Restore the initial (text) mode.
 */
static void 
NVRestore(ScrnInfoPtr pScrn)
{
    vgaHWPtr hwp = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg = &hwp->SavedReg;
    NVPtr pNv = NVPTR(pScrn);
    NVRegPtr nvReg = &pNv->SavedReg;

    if(pNv->HWCursor) {
        NVShowHideCursor(pNv, 0);
        sleep(1);
    }
    NVLockUnlock(pNv, 0);

    if(pNv->twoHeads) {
        VGA_WR08(pNv->PCIO, 0x03D4, 0x44);
        VGA_WR08(pNv->PCIO, 0x03D5, pNv->CRTCnumber * 0x3);
        NVLockUnlock(pNv, 0);
    }

    /* Only restore text mode fonts/text for the primary card */
    vgaHWProtect(pScrn, TRUE);
    NVDACRestore(pScrn, vgaReg, nvReg, pNv->Primary);
    if(pNv->twoHeads) {
        VGA_WR08(pNv->PCIO, 0x03D4, 0x44);
        VGA_WR08(pNv->PCIO, 0x03D5, nvReg->crtcOwner);
    }
    vgaHWProtect(pScrn, FALSE);
}

static void NVBacklightEnable(NVPtr pNv,  Bool on)
{
    /* This is done differently on each laptop.  Here we
       define the ones we know for sure. */

#if defined(__powerpc__)
    if((pNv->Chipset == 0x10DE0179) || 
       (pNv->Chipset == 0x10DE0189) || 
       (pNv->Chipset == 0x10DE0329))
    {
       /* NV17,18,34 Apple iMac, iBook, PowerBook */
      CARD32 tmp_pmc, tmp_pcrt;
      tmp_pmc = pNv->PMC[0x10F0/4] & 0x7FFFFFFF;
      tmp_pcrt = pNv->PCRTC0[0x081C/4] & 0xFFFFFFFC;
      if(on) {
          tmp_pmc |= (1 << 31);
          tmp_pcrt |= 0x1;
      }
      pNv->PMC[0x10F0/4] = tmp_pmc;
      pNv->PCRTC0[0x081C/4] = tmp_pcrt;
    }
#endif
    
    if(pNv->LVDS) {
       if(pNv->twoHeads) {
           if((pNv->Chipset & 0x0ff0) != 0x0110) {
               pNv->PMC[0x130C/4] = on ? 3 : 7;
           } else if(SUBVENDOR_ID(pNv->PciInfo) == 0x1028 &&
                     SUBDEVICE_ID(pNv->PciInfo) == 0xd4) {
               // Dell Inspiron 8200, GeForce2 Go
               CARD32 tmp_pcrt = pNv->PCRTC0[0x081C/4] & 0xFFFFFFFC;
               if(on)
                   tmp_pcrt |= 0x1;
               pNv->PCRTC0[0x081C/4] = tmp_pcrt;
           }
       }
    } else {
       CARD32 fpcontrol;

       fpcontrol = pNv->PRAMDAC[0x0848/4] & 0xCfffffCC;

       /* cut the TMDS output */
       if(on) fpcontrol |= pNv->fpSyncs;
       else fpcontrol |= 0x20000022;

       pNv->PRAMDAC[0x0848/4] = fpcontrol;
    }
}

static void
NVDPMSSetLCD(ScrnInfoPtr pScrn, int PowerManagementMode, int flags)
{
  NVPtr pNv = NVPTR(pScrn);

  if (!pScrn->vtSema) return;

  vgaHWDPMSSet(pScrn, PowerManagementMode, flags);

  switch (PowerManagementMode) {
  case DPMSModeStandby:  /* HSync: Off, VSync: On */
  case DPMSModeSuspend:  /* HSync: On, VSync: Off */
  case DPMSModeOff:      /* HSync: Off, VSync: Off */
    NVBacklightEnable(pNv, 0);
    break;
  case DPMSModeOn:       /* HSync: On, VSync: On */
    NVBacklightEnable(pNv, 1);
  default:
    break;
  }
}


static void
NVDPMSSet(ScrnInfoPtr pScrn, int PowerManagementMode, int flags)
{
  unsigned char crtc1A;
  vgaHWPtr hwp = VGAHWPTR(pScrn);

  if (!pScrn->vtSema) return;

  crtc1A = hwp->readCrtc(hwp, 0x1A) & ~0xC0;

  switch (PowerManagementMode) {
  case DPMSModeStandby:  /* HSync: Off, VSync: On */
    crtc1A |= 0x80;
    break;
  case DPMSModeSuspend:  /* HSync: On, VSync: Off */
    crtc1A |= 0x40;
    break;
  case DPMSModeOff:      /* HSync: Off, VSync: Off */
    crtc1A |= 0xC0;
    break;
  case DPMSModeOn:       /* HSync: On, VSync: On */
  default:
    break;
  }

  /* vgaHWDPMSSet will merely cut the dac output */
  vgaHWDPMSSet(pScrn, PowerManagementMode, flags);

  hwp->writeCrtc(hwp, 0x1A, crtc1A);
}

static Bool
NVCreateScreenResources(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	NVPtr pNv = NVPTR(pScrn);
	PixmapPtr pPixmap;
	Bool ret;

	pScreen->CreateScreenResources = pNv->CreateScreenResources;
	ret = pScreen->CreateScreenResources(pScreen);
	pScreen->CreateScreenResources = NVCreateScreenResources;

	if (!ret)
		return FALSE;

	pPixmap = pScreen->GetScreenPixmap(pScreen);

	if (!shadowAdd(pScreen, pPixmap, NVShadowUpdate,
		NULL, 0, NULL)) {
		return FALSE;
	}
	return TRUE;
}

static Bool
NVShadowInit(ScreenPtr pScreen)
{
	ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
	NVPtr pNv = NVPTR(pScrn);

	if (!shadowSetup(pScreen))
		return FALSE;
	pNv->CreateScreenResources = pScreen->CreateScreenResources;
	pScreen->CreateScreenResources = NVCreateScreenResources;

	return TRUE;
}


/* Mandatory */

/* This gets called at the start of each server generation */

static Bool
NVScreenInit(SCREEN_INIT_ARGS_DECL)
{
    ScrnInfoPtr pScrn;
    vgaHWPtr hwp;
    NVPtr pNv;
    int ret;
    VisualPtr visual;
    unsigned char *FBStart;
    int width, height, displayWidth, offscreenHeight, shadowHeight;
    BoxRec AvailFBArea;

    /* 
     * First get the ScrnInfoRec
     */
    pScrn = xf86ScreenToScrn(pScreen);

    hwp = VGAHWPTR(pScrn);
    pNv = NVPTR(pScrn);

    /* Map the NV memory and MMIO areas */
    if (pNv->FBDev) {
        if (!NVMapMemFBDev(pScrn)) {
            return FALSE;
        }
    } else {
        if (!NVMapMem(pScrn)) {
            return FALSE;
         }
    }

    /* Map the VGA memory when the primary video */
    if (pNv->Primary && !pNv->FBDev) {
	hwp->MapSize = 0x10000;
	if (!vgaHWMapMem(pScrn))
	    return FALSE;
    }

    if (pNv->FBDev) {
	fbdevHWSave(pScrn);
	if (!fbdevHWModeInit(pScrn, pScrn->currentMode))
	    return FALSE;
    } else if (pNv->VBEDualhead) {
        NVSaveRestoreVBE(pScrn, MODE_SAVE);
        if (!NVSetModeVBE(pScrn, pScrn->currentMode))
            return FALSE;
    } else {
	/* Save the current state */
	NVSave(pScrn);
	/* Initialise the first mode */
	if (!NVModeInit(pScrn, pScrn->currentMode))
	    return FALSE;
    }

    /* Darken the screen for aesthetic reasons and set the viewport */
    NVSaveScreen(pScreen, SCREEN_SAVER_ON);
    pScrn->AdjustFrame(ADJUST_FRAME_ARGS(pScrn, pScrn->frameX0, pScrn->frameY0));

    /*
     * The next step is to setup the screen's visuals, and initialise the
     * framebuffer code.  In cases where the framebuffer's default
     * choices for things like visual layouts and bits per RGB are OK,
     * this may be as simple as calling the framebuffer's ScreenInit()
     * function.  If not, the visuals will need to be setup before calling
     * a fb ScreenInit() function and fixed up after.
     *
     * For most PC hardware at depths >= 8, the defaults that fb uses
     * are not appropriate.  In this driver, we fixup the visuals after.
     */

    /*
     * Reset the visual list.
     */
    miClearVisualTypes();

    /* Setup the visuals we support. */

    if (!miSetVisualTypes(pScrn->depth, 
                          miGetDefaultVisualMask(pScrn->depth), 8,
                          pScrn->defaultVisual))
	  return FALSE;
    if (!miSetPixmapDepths ()) return FALSE;

    /*
     * Call the framebuffer layer's ScreenInit function, and fill in other
     * pScreen fields.
     */

    width = pScrn->virtualX;
    height = pScrn->virtualY;
    displayWidth = pScrn->displayWidth;


    if(pNv->Rotate) {
	height = pScrn->virtualX;
	width = pScrn->virtualY;
    }

    /* If RandR rotation is enabled, leave enough space in the
     * framebuffer for us to rotate the screen dimensions without
     * changing the pitch.
     */
    if(pNv->RandRRotation)
        shadowHeight = max(width, height);
    else
        shadowHeight = height;

    if(pNv->ShadowFB) {
 	pNv->ShadowPitch = BitmapBytePad(pScrn->bitsPerPixel * width);
        pNv->ShadowPtr = malloc(pNv->ShadowPitch * shadowHeight);
	displayWidth = pNv->ShadowPitch / (pScrn->bitsPerPixel >> 3);
        FBStart = pNv->ShadowPtr;
    } else {
	pNv->ShadowPtr = NULL;
	FBStart = pNv->FbStart;
    }

    switch (pScrn->bitsPerPixel) {
        case 8:
        case 16:
        case 32:
            ret = fbScreenInit(pScreen, FBStart, width, height,
                               pScrn->xDpi, pScrn->yDpi,
                               displayWidth, pScrn->bitsPerPixel);
            break;
        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Internal error: invalid bpp (%d) in NVScreenInit\n",
                       pScrn->bitsPerPixel);
            ret = FALSE;
            break;
    }
    if (!ret)
	return FALSE;

    if (pScrn->bitsPerPixel > 8) {
        /* Fixup RGB ordering */
        visual = pScreen->visuals + pScreen->numVisuals;
        while (--visual >= pScreen->visuals) {
	    if ((visual->class | DynamicClass) == DirectColor) {
		visual->offsetRed = pScrn->offset.red;
		visual->offsetGreen = pScrn->offset.green;
		visual->offsetBlue = pScrn->offset.blue;
		visual->redMask = pScrn->mask.red;
		visual->greenMask = pScrn->mask.green;
		visual->blueMask = pScrn->mask.blue;
	    }
	}
    }

    fbPictureInit (pScreen, 0, 0);
    
    xf86SetBlackWhitePixels(pScreen);

    if(!pNv->ShadowFB) /* hardware cursor needs to wrap this layer */
	NVDGAInit(pScreen);

    offscreenHeight = pNv->ScratchBufferStart /
                     (pScrn->displayWidth * pScrn->bitsPerPixel >> 3);
    if(offscreenHeight > 32767)
        offscreenHeight = 32767;

    AvailFBArea.x1 = 0;
    AvailFBArea.y1 = 0;
    AvailFBArea.x2 = pScrn->displayWidth;
    AvailFBArea.y2 = offscreenHeight;
    xf86InitFBManager(pScreen, &AvailFBArea);
    
    if (!pNv->NoAccel)
	NVAccelInit(pScreen);
    
    xf86SetBackingStore(pScreen);
    xf86SetSilkenMouse(pScreen);

    /* Initialize software cursor.  
	Must precede creation of the default colormap */
    miDCInitialize(pScreen, xf86GetPointerScreenFuncs());

    /* Initialize HW cursor layer. 
	Must follow software cursor initialization*/
    if (pNv->HWCursor) { 
	if(!NVCursorInit(pScreen))
	    xf86DrvMsg(pScrn->scrnIndex, X_ERROR, 
		"Hardware cursor initialization failed\n");
    }

    /* Initialise default colourmap */
    if (!miCreateDefColormap(pScreen))
	return FALSE;

    /* Initialize colormap layer.  
	Must follow initialization of the default colormap */
    if(!xf86HandleColormaps(pScreen, 256, 8,
	(pNv->FBDev ? fbdevHWLoadPaletteWeak() : NVDACLoadPalette), 
	NULL, CMAP_RELOAD_ON_MODE_SWITCH | CMAP_PALETTED_TRUECOLOR))
	return FALSE;

    if(pNv->ShadowFB) {
	RefreshAreaFuncPtr refreshArea = NVRefreshArea;

	if(pNv->Rotate || pNv->RandRRotation) {
	   pNv->PointerMoved = pScrn->PointerMoved;
	   if(pNv->Rotate)
	       pScrn->PointerMoved = NVPointerMoved;

	   switch(pScrn->bitsPerPixel) {
               case 8:	refreshArea = NVRefreshArea8;	break;
               case 16:	refreshArea = NVRefreshArea16;	break;
               case 32:	refreshArea = NVRefreshArea32;	break;
	   }
#if XORG_VERSION_CURRENT < XORG_VERSION_NUMERIC(1,19,99,1,0)
           if(!pNv->RandRRotation) {
               xf86DisableRandR();
               xf86DrvMsg(pScrn->scrnIndex, X_INFO,
                          "Driver rotation enabled, RandR disabled\n");
           }
#endif
	}
        pNv->refreshArea = refreshArea;
	NVShadowInit(pScreen);
    }

    if(pNv->FlatPanel)
       xf86DPMSInit(pScreen, NVDPMSSetLCD, 0);
    else
       xf86DPMSInit(pScreen, NVDPMSSet, 0);
    
    pScrn->memPhysBase = pNv->FbAddress;
    pScrn->fbOffset = 0;

    if(pNv->Rotate == 0 && !pNv->RandRRotation)
       NVInitVideo(pScreen);

    pScreen->SaveScreen = NVSaveScreen;

    /* Wrap the current CloseScreen function */
    pNv->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = NVCloseScreen;

    pNv->BlockHandler = pScreen->BlockHandler;
    pScreen->BlockHandler = NVBlockHandler;

    pNv->accessEnabled = TRUE;
    pNv->EnableDisableFBAccess = pScrn->EnableDisableFBAccess;
    pScrn->EnableDisableFBAccess = NVEnableDisableFBAccess;

#ifdef RANDR
    /* Install our DriverFunc.  We have to do it this way instead of using the
     * HaveDriverFuncs argument to xf86AddDriver, because InitOutput clobbers
     * pScrn->DriverFunc */
    pScrn->DriverFunc = NVDriverFunc;
#endif

    /* Report any unused options (only for the first generation) */
    if (serverGeneration == 1) {
	xf86ShowUnusedOptions(pScrn->scrnIndex, pScrn->options);
    }
    return TRUE;
}

static Bool
NVSaveScreen(ScreenPtr pScreen, int mode)
{
    return vgaHWSaveScreen(pScreen, mode);
}

static void
NVSave(ScrnInfoPtr pScrn)
{
    NVPtr pNv = NVPTR(pScrn);
    NVRegPtr nvReg = &pNv->SavedReg;
    vgaHWPtr pVga = VGAHWPTR(pScrn);
    vgaRegPtr vgaReg = &pVga->SavedReg;

    NVLockUnlock(pNv, 0);
    if(pNv->twoHeads) {
        VGA_WR08(pNv->PCIO, 0x03D4, 0x44);
        VGA_WR08(pNv->PCIO, 0x03D5, pNv->CRTCnumber * 0x3);
        NVLockUnlock(pNv, 0);
    }

    NVDACSave(pScrn, vgaReg, nvReg, pNv->Primary);
}

static void
NVSaveRestoreVBE(ScrnInfoPtr pScrn, vbeSaveRestoreFunction function)
{
    NVPtr pNv = NVPTR(pScrn);

    if (function == MODE_SAVE) {
        VBEGetVBEMode(pNv->pVbe, &pNv->vbeMode);
        NVSave(pScrn);
    } else if (function == MODE_RESTORE) {
        NVRestore(pScrn);
        VBESetVBEMode(pNv->pVbe, pNv->vbeMode, NULL);
    }
}

#ifdef RANDR
static Bool
NVRandRGetInfo(ScrnInfoPtr pScrn, Rotation *rotations)
{
    NVPtr pNv = NVPTR(pScrn);

    if(pNv->RandRRotation)
       *rotations = RR_Rotate_0 | RR_Rotate_90 | RR_Rotate_270;
    else
       *rotations = RR_Rotate_0;

    return TRUE;
}

static Bool
NVRandRSetConfig(ScrnInfoPtr pScrn, xorgRRConfig *config)
{
    NVPtr pNv = NVPTR(pScrn);

    switch(config->rotation) {
        case RR_Rotate_0:
            pNv->Rotate = 0;
            pScrn->PointerMoved = pNv->PointerMoved;
            break;

        case RR_Rotate_90:
            pNv->Rotate = -1;
            pScrn->PointerMoved = NVPointerMoved;
            break;

        case RR_Rotate_270:
            pNv->Rotate = 1;
            pScrn->PointerMoved = NVPointerMoved;
            break;

        default:
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                    "Unexpected rotation in NVRandRSetConfig!\n");
            pNv->Rotate = 0;
            pScrn->PointerMoved = pNv->PointerMoved;
            return FALSE;
    }

    return TRUE;
}

static Bool
NVDriverFunc(ScrnInfoPtr pScrn, xorgDriverFuncOp op, pointer data)
{
    switch(op) {
       case RR_GET_INFO:
          return NVRandRGetInfo(pScrn, (Rotation*)data);
       case RR_SET_CONFIG:
          return NVRandRSetConfig(pScrn, (xorgRRConfig*)data);
       default:
          return FALSE;
    }

    return FALSE;
}
#endif /* RANDR */
