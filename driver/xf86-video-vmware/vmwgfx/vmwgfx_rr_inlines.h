/**************************************************************************
 * Copyright © 2016 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/
/*
 * This file contains calls into the RandR1.2 code and modification of core
 * RandR data structures that probably were never intended from drivers.
 */

#ifndef _VMWGFX_RR_INLINES_H_
#define _VMWGFX_RR_INLINES_H_

#include <xf86Crtc.h>
#include <xf86RandR12.h>

#define VMW_DPI 96.
#define VMW_INCH_TO_MM 25.4

/**
 * vmwgfx_notify_rr - Notify RandR that our configuration has changed.
 *
 * @pScreen: Pointer to the affected screen.
 *
 * Normally screen configurations are typically only changed using RandR,
 * so when we do it in an udev handler, we need to notify RandR that we've
 * made a change, so that it can be propagated to all RandR clients.
 */
static inline void
vmwgfx_notify_rr(ScreenPtr pScreen)
{
    rrScrPriv(pScreen);


    /*
     * We need to update the time-stamps, otherwise X clients that haven't
     * yet read this config might just overwrite it.
     * This effectively stops the desktop manager from trying to
     * outsmart us, since RandR simply doesn't accept requests from
     * clients that haven't read this config and tag their request with
     * an earlier timestamp.
     */
    pScrPriv->lastSetTime = currentTime;
    pScrPriv->lastConfigTime = currentTime;
#ifdef RANDR_12_INTERFACE
    xf86RandR12TellChanged(pScreen);
#else
    RRTellChanged(pScreen);
#endif
}

/**
 * vmwgfx_rr_screen_set_size - Use RandR to change the root pixmap dimensions.
 *
 * @pScreen: Pointer to the affected screen.
 *
 * Returns: TRUE if successful. False otherwise.
 */
static inline Bool
vmwgfx_rr_screen_set_size(ScreenPtr pScreen, int width, int height)
{
    rrScrPriv(pScreen);
    float mm_width, mm_height;

    mm_width = ((float) width) * VMW_INCH_TO_MM / VMW_DPI + .5;
    mm_height = ((float) height) * VMW_INCH_TO_MM / VMW_DPI + .5;

    return pScrPriv->rrScreenSetSize(pScreen, width, height,
				     (int) mm_width, (int) mm_height);
}

#endif
