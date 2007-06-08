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

static void G80SetCursorColors(ScrnInfoPtr pScrn, int bg, int fg)
{
    G80Ptr pNv = G80PTR(pScrn);
    CARD32 *dst = CURSOR_PTR;
    CARD32 *src = pNv->tmpCursor;
    int i, j;

    fg |= 0xff000000;
    bg |= 0xff000000;

    for(i = 0; i < 128; i++) {
        CARD32 b = *src++;
        CARD32 m = *src++;

        for(j = 0; j < 32; j++) {
            if(m & 1)
                *dst = (b & 1) ? fg : bg;
            else
                *dst = 0;
            b >>= 1;
            m >>= 1;
            dst++;
        }
    }
}

static void G80SetCursorPosition(ScrnInfoPtr pScrn, int x, int y)
{
    G80Ptr pNv = G80PTR(pScrn);
    const int headOff = 0x1000*pNv->head;

    x &= 0xffff;
    y &= 0xffff;
    pNv->reg[(0x00647084 + headOff)/4] = y << 16 | x;
    pNv->reg[(0x00647080 + headOff)/4] = 0;
}

static void G80LoadCursorImage(ScrnInfoPtr pScrn, unsigned char *bits)
{
    G80Ptr pNv = G80PTR(pScrn);
    memcpy(pNv->tmpCursor, bits, sizeof(pNv->tmpCursor));
}

static void G80HideCursor(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);

    pNv->cursorVisible = FALSE;
    G80DispHideCursor(G80PTR(pScrn), TRUE);
}

static void G80ShowCursor(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);

    pNv->cursorVisible = TRUE;
    G80DispShowCursor(G80PTR(pScrn), TRUE);
}

static Bool G80UseHWCursor(ScreenPtr pScreen, CursorPtr pCurs)
{
    return TRUE;
}

#ifdef ARGB_CURSOR
static Bool G80UseHWCursorARGB(ScreenPtr pScreen, CursorPtr pCurs)
{
    if((pCurs->bits->width <= 64) && (pCurs->bits->height <= 64))
        return TRUE;

    return FALSE;
}

static void G80LoadCursorARGB(ScrnInfoPtr pScrn, CursorPtr pCurs)
{
    G80Ptr pNv = G80PTR(pScrn);
    CARD32 *dst = CURSOR_PTR, *src = pCurs->bits->argb;
    int y;

    for(y = 0; y < pCurs->bits->height; y++) {
        memcpy(dst, src, pCurs->bits->width * 4);
        memset(dst + pCurs->bits->width, 0, (64 - pCurs->bits->width) * 4);
        src += pCurs->bits->width;
        dst += 64;
    }

    memset(dst, 0, (64 - y) * 64 * 4);
}
#endif

Bool G80CursorAcquire(G80Ptr pNv)
{
    const int headOff = 0x10 * pNv->head;

    if(!pNv->HWCursor) return TRUE;

    pNv->reg[(0x00610270+headOff)/4] = 0x2000;
    while(pNv->reg[(0x00610270+headOff)/4] & 0x30000);

    pNv->reg[(0x00610270+headOff)/4] = 1;
    while((pNv->reg[(0x00610270+headOff)/4] & 0x30000) != 0x10000);

    return TRUE;
}

void G80CursorRelease(G80Ptr pNv)
{
    const int headOff = 0x10 * pNv->head;

    if(!pNv->HWCursor) return;

    pNv->reg[(0x00610270+headOff)/4] = 0;
    while(pNv->reg[(0x00610270+headOff)/4] & 0x30000);
}

Bool G80CursorInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    G80Ptr pNv = G80PTR(pScrn);
    xf86CursorInfoPtr infoPtr;

    if(!pNv->HWCursor)
        return TRUE;

    infoPtr = xf86CreateCursorInfoRec();
    if(!infoPtr) return FALSE;

    pNv->CursorInfo = infoPtr;
    pNv->cursorVisible = FALSE;

    infoPtr->MaxWidth = infoPtr->MaxHeight = 64;
    infoPtr->Flags = HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
                     HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_32;
    infoPtr->SetCursorColors = G80SetCursorColors;
    infoPtr->SetCursorPosition = G80SetCursorPosition;
    infoPtr->LoadCursorImage = G80LoadCursorImage;
    infoPtr->HideCursor = G80HideCursor;
    infoPtr->ShowCursor = G80ShowCursor;
    infoPtr->UseHWCursor = G80UseHWCursor;

#ifdef ARGB_CURSOR
    infoPtr->UseHWCursorARGB = G80UseHWCursorARGB;
    infoPtr->LoadCursorARGB = G80LoadCursorARGB;
#endif

    return xf86InitCursor(pScreen, infoPtr);
}
