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

/*
 * Authors:
 *   Kevin E. Martin <martin@valinux.com>
 *
 * Modified by Marc Aurele La France <tsi@xfree86.org> for ATI driver merge.
 */

#ifndef _R128_PROBE_H_
#define _R128_PROBE_H_ 1

#include "xf86str.h"
#include "xf86DDC.h"
#include "randrstr.h"
#include "xf86Crtc.h"

#include "compat-api.h"

#ifdef USE_EXA
#include "exa.h"
#endif

/* Chip definitions */
#define PCI_VENDOR_ATI			0x1002
#define PCI_CHIP_RAGE128LE		0x4C45
#define PCI_CHIP_RAGE128LF		0x4C46
#define PCI_CHIP_RAGE128MF		0x4D46
#define PCI_CHIP_RAGE128ML		0x4D4C
#define PCI_CHIP_RAGE128PA		0x5041
#define PCI_CHIP_RAGE128PB		0x5042
#define PCI_CHIP_RAGE128PC		0x5043
#define PCI_CHIP_RAGE128PD		0x5044
#define PCI_CHIP_RAGE128PE		0x5045
#define PCI_CHIP_RAGE128PF		0x5046
#define PCI_CHIP_RAGE128PG		0x5047
#define PCI_CHIP_RAGE128PH		0x5048
#define PCI_CHIP_RAGE128PI		0x5049
#define PCI_CHIP_RAGE128PJ		0x504A
#define PCI_CHIP_RAGE128PK		0x504B
#define PCI_CHIP_RAGE128PL		0x504C
#define PCI_CHIP_RAGE128PM		0x504D
#define PCI_CHIP_RAGE128PN		0x504E
#define PCI_CHIP_RAGE128PO		0x504F
#define PCI_CHIP_RAGE128PP		0x5050
#define PCI_CHIP_RAGE128PQ		0x5051
#define PCI_CHIP_RAGE128PR		0x5052
#define PCI_CHIP_RAGE128PS		0x5053
#define PCI_CHIP_RAGE128PT		0x5054
#define PCI_CHIP_RAGE128PU		0x5055
#define PCI_CHIP_RAGE128PV		0x5056
#define PCI_CHIP_RAGE128PW		0x5057
#define PCI_CHIP_RAGE128PX		0x5058
#define PCI_CHIP_RAGE128RE		0x5245
#define PCI_CHIP_RAGE128RF		0x5246
#define PCI_CHIP_RAGE128RG		0x5247
#define PCI_CHIP_RAGE128RK		0x524B
#define PCI_CHIP_RAGE128RL		0x524C
#define PCI_CHIP_RAGE128SE		0x5345
#define PCI_CHIP_RAGE128SF		0x5346
#define PCI_CHIP_RAGE128SG		0x5347
#define PCI_CHIP_RAGE128SH		0x5348
#define PCI_CHIP_RAGE128SK		0x534B
#define PCI_CHIP_RAGE128SL		0x534C
#define PCI_CHIP_RAGE128SM		0x534D
#define PCI_CHIP_RAGE128SN		0x534E
#define PCI_CHIP_RAGE128TF		0x5446
#define PCI_CHIP_RAGE128TL		0x544C
#define PCI_CHIP_RAGE128TR		0x5452
#define PCI_CHIP_RAGE128TS		0x5453
#define PCI_CHIP_RAGE128TT		0x5454
#define PCI_CHIP_RAGE128TU		0x5455

extern DriverRec R128;

typedef enum
{
    MT_UNKNOWN = -1,
    MT_NONE    = 0,
    MT_CRT     = 1,
    MT_LCD     = 2,
    MT_DFP     = 3,
    MT_CTV     = 4,
    MT_STV     = 5
} R128MonitorType;

typedef enum
{
    OUTPUT_NONE,
    OUTPUT_VGA,
    OUTPUT_DVI,
    OUTPUT_LVDS
} R128OutputType;

typedef struct {
    uint32_t ddc_reg;
    uint32_t put_clk_mask;
    uint32_t put_data_mask;
    uint32_t get_clk_mask;
    uint32_t get_data_mask;
} R128I2CBusRec, *R128I2CBusPtr;

typedef struct _R128CrtcPrivateRec {
    void *rotate_mem;
    int crtc_id;
    uint32_t cursor_offset;
    /* Lookup table values to be set when the CRTC is enabled */
    uint8_t lut_r[256], lut_g[256], lut_b[256];
} R128CrtcPrivateRec, *R128CrtcPrivatePtr;

typedef struct _R128OutputPrivateRec {
    int num;
    R128OutputType type;
    R128MonitorType MonType;
    I2CBusPtr pI2CBus;
    R128I2CBusRec ddc_i2c;
    int PanelXRes;
    int PanelYRes;
    int PanelPwrDly;
} R128OutputPrivateRec, *R128OutputPrivatePtr;

#define R128_MAX_CRTC 2
#define R128_MAX_BIOS_CONNECTOR 2

typedef struct
{
    Bool HasSecondary;
    Bool HasCRTC2;
    /*These two registers are used to make sure the CRTC2 is
      retored before CRTC_EXT, otherwise it could lead to blank screen.*/
    Bool IsSecondaryRestored;
    Bool RestorePrimary;

    xf86CrtcPtr pCrtc[R128_MAX_CRTC];
    R128CrtcPrivatePtr Controller[R128_MAX_CRTC];

    ScrnInfoPtr pSecondaryScrn;
    ScrnInfoPtr pPrimaryScrn;
} R128EntRec, *R128EntPtr;

/* r128_probe.c */
extern SymTabRec             R128Chipsets[];

/* r128_driver.c */
extern Bool                  R128PreInit(ScrnInfoPtr, int);
extern Bool                  R128ScreenInit(SCREEN_INIT_ARGS_DECL);
extern Bool                  R128SwitchMode(SWITCH_MODE_ARGS_DECL);
extern void                  R128AdjustFrame(ADJUST_FRAME_ARGS_DECL);
extern Bool                  R128EnterVT(VT_FUNC_ARGS_DECL);
extern void                  R128LeaveVT(VT_FUNC_ARGS_DECL);
extern void                  R128FreeScreen(FREE_SCREEN_ARGS_DECL);
extern ModeStatus            R128ValidMode(SCRN_ARG_TYPE, DisplayModePtr, Bool, int);

extern const OptionInfoRec * R128OptionsWeak(void);

#endif /* _R128_PROBE_H_ */
