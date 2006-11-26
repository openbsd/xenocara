/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/ati/r128_probe.h,v 1.6 2002/04/06 19:06:06 tsi Exp $ */
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

typedef struct
{
    Bool IsDRIEnabled;

    Bool HasSecondary;
    Bool BypassSecondary;
    /*These two registers are used to make sure the CRTC2 is
      retored before CRTC_EXT, otherwise it could lead to blank screen.*/
    Bool IsSecondaryRestored;
    Bool RestorePrimary;

    ScrnInfoPtr pSecondaryScrn;    
    ScrnInfoPtr pPrimaryScrn;
} R128EntRec, *R128EntPtr;

/* r128_probe.c */
extern const OptionInfoRec * R128AvailableOptions(int, int);
extern void                  R128Identify(int);
extern Bool                  R128Probe(DriverPtr, int);

extern PciChipsets           R128PciChipsets[];

/* r128_driver.c */
extern void                  R128LoaderRefSymLists(void);
extern Bool                  R128PreInit(ScrnInfoPtr, int);
extern Bool                  R128ScreenInit(int, ScreenPtr, int, char **);
extern Bool                  R128SwitchMode(int, DisplayModePtr, int);
extern void                  R128AdjustFrame(int, int, int, int);
extern Bool                  R128EnterVT(int, int);
extern void                  R128LeaveVT(int, int);
extern void                  R128FreeScreen(int, int);
extern ModeStatus            R128ValidMode(int, DisplayModePtr, Bool, int);

extern const OptionInfoRec * R128OptionsWeak(void);

extern void                  R128FillInScreenInfo(ScrnInfoPtr);

#endif /* _R128_PROBE_H_ */
