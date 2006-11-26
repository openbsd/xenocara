/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/ati/radeon_probe.h,v 1.13 2003/10/30 17:37:00 tsi Exp $ */
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

#include "xf86str.h"
#include "xf86DDC.h"

#define _XF86MISC_SERVER_
#include <X11/extensions/xf86misc.h>

typedef enum
{
    DDC_NONE_DETECTED,
    DDC_MONID,
    DDC_DVI,
    DDC_VGA,
    DDC_CRT2
} RADEONDDCType;

typedef enum
{
    MT_UNKNOWN = -1,
    MT_NONE    = 0,
    MT_CRT     = 1,
    MT_LCD     = 2,
    MT_DFP     = 3,
    MT_CTV     = 4,
    MT_STV     = 5
} RADEONMonitorType;

typedef enum
{
    CONNECTOR_NONE,
    CONNECTOR_PROPRIETARY,
    CONNECTOR_CRT,
    CONNECTOR_DVI_I,
    CONNECTOR_DVI_D,
    CONNECTOR_CTV,
    CONNECTOR_STV,
    CONNECTOR_UNSUPPORTED
} RADEONConnectorType;

typedef enum
{
    CONNECTOR_NONE_ATOM,
    CONNECTOR_VGA_ATOM,
    CONNECTOR_DVI_I_ATOM,
    CONNECTOR_DVI_D_ATOM,
    CONNECTOR_DVI_A_ATOM,
    CONNECTOR_STV_ATOM,
    CONNECTOR_CTV_ATOM,
    CONNECTOR_LVDS_ATOM,
    CONNECTOR_DIGITAL_ATOM,
    CONNECTOR_UNSUPPORTED_ATOM
} RADEONConnectorTypeATOM;

typedef enum
{
    DAC_UNKNOWN = -1,
    DAC_PRIMARY = 0,
    DAC_TVDAC   = 1
} RADEONDacType;

typedef enum
{
    TMDS_UNKNOWN = -1,
    TMDS_INT     = 0,
    TMDS_EXT     = 1
} RADEONTmdsType;

typedef struct
{
    RADEONDDCType DDCType;
    RADEONDacType DACType;
    RADEONTmdsType TMDSType;
    RADEONConnectorType ConnectorType;
    RADEONMonitorType MonType;
    xf86MonPtr MonInfo;
} RADEONConnector;

typedef struct
{
    Bool HasSecondary;

    /*
     * The next two are used to make sure CRTC2 is restored before CRTC_EXT,
     * otherwise it could lead to blank screens.
     */
    Bool IsSecondaryRestored;
    Bool RestorePrimary;

    ScrnInfoPtr pSecondaryScrn;
    ScrnInfoPtr pPrimaryScrn;

    int MonType1;
    int MonType2;
    xf86MonPtr MonInfo1;
    xf86MonPtr MonInfo2;
    Bool ReversedDAC;	  /* TVDAC used as primary dac */
    Bool ReversedTMDS;    /* DDC_DVI is used for external TMDS */
    RADEONConnector PortInfo[2];
} RADEONEntRec, *RADEONEntPtr;

/* radeon_probe.c */
extern const OptionInfoRec *RADEONAvailableOptions(int, int);
extern void                 RADEONIdentify(int);
extern Bool                 RADEONProbe(DriverPtr, int);

extern PciChipsets          RADEONPciChipsets[];

/* radeon_driver.c */
extern void                 RADEONLoaderRefSymLists(void);
extern Bool                 RADEONPreInit(ScrnInfoPtr, int);
extern Bool                 RADEONScreenInit(int, ScreenPtr, int, char **);
extern Bool                 RADEONSwitchMode(int, DisplayModePtr, int);
#ifdef X_XF86MiscPassMessage
extern Bool                 RADEONHandleMessage(int, const char*, const char*,
					        char**);
#endif
extern void                 RADEONAdjustFrame(int, int, int, int);
extern Bool                 RADEONEnterVT(int, int);
extern void                 RADEONLeaveVT(int, int);
extern void                 RADEONFreeScreen(int, int);
extern ModeStatus           RADEONValidMode(int, DisplayModePtr, Bool, int);

extern const OptionInfoRec *RADEONOptionsWeak(void);

extern void                 RADEONFillInScreenInfo(ScrnInfoPtr);

#endif /* _RADEON_PROBE_H_ */
