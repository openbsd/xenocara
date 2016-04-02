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
 *   Kevin E. Martin <martin@xfree86.org>
 *
 * Modified by Marc Aurele La France <tsi@xfree86.org> for ATI driver merge.
 */

#ifndef _RADEON_PROBE_H_
#define _RADEON_PROBE_H_ 1

#include <stdint.h>
#include "xorg-server.h"
#include "xf86str.h"
#include "xf86DDC.h"
#include "randrstr.h"

#include "xf86Crtc.h"

#ifdef XSERVER_PLATFORM_BUS
#include "xf86platformBus.h"
#endif

#include "compat-api.h"
#include "exa.h"

extern DriverRec RADEON;

typedef enum {
    CHIP_FAMILY_UNKNOW,
    CHIP_FAMILY_LEGACY,
    CHIP_FAMILY_RADEON,
    CHIP_FAMILY_RV100,
    CHIP_FAMILY_RS100,    /* U1 (IGP320M) or A3 (IGP320)*/
    CHIP_FAMILY_RV200,
    CHIP_FAMILY_RS200,    /* U2 (IGP330M/340M/350M) or A4 (IGP330/340/345/350), RS250 (IGP 7000) */
    CHIP_FAMILY_R200,
    CHIP_FAMILY_RV250,
    CHIP_FAMILY_RS300,    /* RS300/RS350 */
    CHIP_FAMILY_RV280,
    CHIP_FAMILY_R300,
    CHIP_FAMILY_R350,
    CHIP_FAMILY_RV350,
    CHIP_FAMILY_RV380,    /* RV370/RV380/M22/M24 */
    CHIP_FAMILY_R420,     /* R420/R423/M18 */
    CHIP_FAMILY_RV410,    /* RV410, M26 */
    CHIP_FAMILY_RS400,    /* xpress 200, 200m (RS400) Intel */
    CHIP_FAMILY_RS480,    /* xpress 200, 200m (RS410/480/482/485) AMD */
    CHIP_FAMILY_RV515,    /* rv515 */
    CHIP_FAMILY_R520,    /* r520 */
    CHIP_FAMILY_RV530,    /* rv530 */
    CHIP_FAMILY_R580,    /* r580 */
    CHIP_FAMILY_RV560,   /* rv560 */
    CHIP_FAMILY_RV570,   /* rv570 */
    CHIP_FAMILY_RS600,
    CHIP_FAMILY_RS690,
    CHIP_FAMILY_RS740,
    CHIP_FAMILY_R600,    /* r600 */
    CHIP_FAMILY_RV610,
    CHIP_FAMILY_RV630,
    CHIP_FAMILY_RV670,
    CHIP_FAMILY_RV620,
    CHIP_FAMILY_RV635,
    CHIP_FAMILY_RS780,
    CHIP_FAMILY_RS880,
    CHIP_FAMILY_RV770,   /* r700 */
    CHIP_FAMILY_RV730,
    CHIP_FAMILY_RV710,
    CHIP_FAMILY_RV740,
    CHIP_FAMILY_CEDAR,   /* evergreen */
    CHIP_FAMILY_REDWOOD,
    CHIP_FAMILY_JUNIPER,
    CHIP_FAMILY_CYPRESS,
    CHIP_FAMILY_HEMLOCK,
    CHIP_FAMILY_PALM,
    CHIP_FAMILY_SUMO,
    CHIP_FAMILY_SUMO2,
    CHIP_FAMILY_BARTS,
    CHIP_FAMILY_TURKS,
    CHIP_FAMILY_CAICOS,
    CHIP_FAMILY_CAYMAN,
    CHIP_FAMILY_ARUBA,
    CHIP_FAMILY_TAHITI,
    CHIP_FAMILY_PITCAIRN,
    CHIP_FAMILY_VERDE,
    CHIP_FAMILY_OLAND,
    CHIP_FAMILY_HAINAN,
    CHIP_FAMILY_BONAIRE,
    CHIP_FAMILY_KAVERI,
    CHIP_FAMILY_KABINI,
    CHIP_FAMILY_HAWAII,
    CHIP_FAMILY_MULLINS,
    CHIP_FAMILY_LAST
} RADEONChipFamily;

typedef struct {
    uint32_t pci_device_id;
    RADEONChipFamily chip_family;
    int mobility;
    int igp;
    int nocrtc2;
    int nointtvout;
    int singledac;
} RADEONCardInfo;

typedef struct
{
    Bool              HasCRTC2;         /* All cards except original Radeon  */

    int fd;                             /* for sharing across zaphod heads   */
    int fd_ref;
    unsigned long     fd_wakeup_registered; /* server generation for which fd has been registered for wakeup handling */
    int fd_wakeup_ref;
    unsigned int assigned_crtcs;
#ifdef XSERVER_PLATFORM_BUS
    struct xf86_platform_device *platform_dev;
#endif
} RADEONEntRec, *RADEONEntPtr;

extern const OptionInfoRec *RADEONOptionsWeak(void);

extern Bool                 RADEONPreInit_KMS(ScrnInfoPtr, int);
extern Bool                 RADEONScreenInit_KMS(SCREEN_INIT_ARGS_DECL);
extern Bool                 RADEONSwitchMode_KMS(SWITCH_MODE_ARGS_DECL);
extern void                 RADEONAdjustFrame_KMS(ADJUST_FRAME_ARGS_DECL);
extern Bool                 RADEONEnterVT_KMS(VT_FUNC_ARGS_DECL);
extern void                 RADEONLeaveVT_KMS(VT_FUNC_ARGS_DECL);
extern void RADEONFreeScreen_KMS(FREE_SCREEN_ARGS_DECL);

extern ModeStatus RADEONValidMode(SCRN_ARG_TYPE arg, DisplayModePtr mode,
			   Bool verbose, int flag);
#endif /* _RADEON_PROBE_H_ */
