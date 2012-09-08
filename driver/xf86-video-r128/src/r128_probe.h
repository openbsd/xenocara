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

extern DriverRec R128;

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
