/*
 * Copyright (c) 2007 NVIDIA, Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include <cursorstr.h>

#include "g80_type.h"
#include "g80_cursor.h"
#include "g80_display.h"

#define CURSOR_PTR ((CARD32*)pNv->mem + pNv->videoRam * 256 - 0x1000)

void G80SetCursorPosition(xf86CrtcPtr crtc, int x, int y)
{
    G80Ptr pNv = G80PTR(crtc->scrn);
    const int headOff = 0x1000*G80CrtcGetHead(crtc);

    x &= 0xffff;
    y &= 0xffff;
    pNv->reg[(0x00647084 + headOff)/4] = y << 16 | x;
    pNv->reg[(0x00647080 + headOff)/4] = 0;
}

void G80LoadCursorARGB(xf86CrtcPtr crtc, CARD32 *src)
{
    G80Ptr pNv = G80PTR(crtc->scrn);
    CARD32 *dst = CURSOR_PTR;

    /* Assume cursor is 64x64 */
    memcpy(dst, src, 64 * 64 * 4);
}

Bool G80CursorAcquire(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    if(!pNv->HWCursor) return TRUE;

    /* Initialize the cursor on each head */
    for(i = 0; i < xf86_config->num_crtc; i++) {
        const int headOff = 0x10 * G80CrtcGetHead(xf86_config->crtc[i]);

        pNv->reg[(0x00610270+headOff)/4] = 0x2000;
        while(pNv->reg[(0x00610270+headOff)/4] & 0x30000);

        pNv->reg[(0x00610270+headOff)/4] = 1;
        while((pNv->reg[(0x00610270+headOff)/4] & 0x30000) != 0x10000);
    }

    return TRUE;
}

void G80CursorRelease(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    if(!pNv->HWCursor) return;

    /* Release the cursor on each head */
    for(i = 0; i < xf86_config->num_crtc; i++) {
        const int headOff = 0x10 * G80CrtcGetHead(xf86_config->crtc[i]);

        pNv->reg[(0x00610270+headOff)/4] = 0;
        while(pNv->reg[(0x00610270+headOff)/4] & 0x30000);
    }
}

Bool G80CursorInit(ScreenPtr pScreen)
{
    return xf86_cursors_init(pScreen, 64, 64,
            HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
            HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_32 |
            HARDWARE_CURSOR_ARGB);
}
