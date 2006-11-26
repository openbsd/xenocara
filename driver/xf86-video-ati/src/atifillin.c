/*
 * Copyright 2004 Adam Jackson.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * on the rights to use, copy, modify, merge, publish, distribute, sub
 * license, and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * atifillin.c: fill in a ScrnInfoPtr with the relevant information for
 * atimisc.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "atifillin.h"

_X_EXPORT void ATIFillInScreenInfo(ScrnInfoPtr pScreenInfo)
{
    pScreenInfo->driverVersion = ATI_VERSION_CURRENT;
    pScreenInfo->driverName    = ATI_DRIVER_NAME;
    pScreenInfo->name          = ATI_NAME;
    pScreenInfo->Probe         = ATIProbe;
    pScreenInfo->PreInit       = ATIPreInit;
    pScreenInfo->ScreenInit    = ATIScreenInit;
    pScreenInfo->SwitchMode    = ATISwitchMode;
    pScreenInfo->AdjustFrame   = ATIAdjustFrame;
    pScreenInfo->EnterVT       = ATIEnterVT;
    pScreenInfo->LeaveVT       = ATILeaveVT;
    pScreenInfo->FreeScreen    = ATIFreeScreen;
    pScreenInfo->ValidMode     = ATIValidMode;
}
