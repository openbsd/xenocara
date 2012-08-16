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

#include <miline.h>

#include "g80_type.h"
#include "g80_dma.h"
#include "g80_xaa.h"

void
G80Sync(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);
    volatile CARD16 *pSync = (volatile CARD16*)&pNv->reg[0x00711008/4] + 1;

    G80DmaStart(pNv, 0x104, 1);
    G80DmaNext (pNv, 0);
    G80DmaStart(pNv, 0x100, 1);
    G80DmaNext (pNv, 0);

    *pSync = 0x8000;
    G80DmaKickoff(pNv);
    while(*pSync);
}

void
G80DMAKickoffCallback(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);

    G80DmaKickoff(pNv);
    pNv->DMAKickoffCallback = NULL;
}

void
G80SetPattern(G80Ptr pNv, int bg, int fg, int pat0, int pat1)
{
    G80DmaStart(pNv, 0x2f0, 4);
    G80DmaNext (pNv, bg);
    G80DmaNext (pNv, fg);
    G80DmaNext (pNv, pat0);
    G80DmaNext (pNv, pat1);
}

void
G80SetRopSolid(G80Ptr pNv, CARD32 rop, CARD32 planemask)
{
    static const int rops[] = {
        0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0,
        0x30, 0xB0, 0x70, 0xF0
    };

    if(planemask != ~0) {
        G80SetPattern(pNv, 0, planemask, ~0, ~0);
        if(pNv->currentRop != (rop + 32)) {
            pNv->currentRop = rop + 32;

            rop = rops[rop] | 0xA;
            G80DmaStart(pNv, 0x2a0, 1);
            G80DmaNext (pNv, rop);
        }
    } else if(pNv->currentRop != rop) {
        if(pNv->currentRop >= 16)
            G80SetPattern(pNv, ~0, ~0, ~0, ~0);
        pNv->currentRop = rop;

        rop = rops[rop];
        rop |= rop >> 4;
        G80DmaStart(pNv, 0x2a0, 1);
        G80DmaNext (pNv, rop);
    }
}

inline void
G80SetClip(G80Ptr pNv, int x, int y, int w, int h)
{
    G80DmaStart(pNv, 0x280, 4);
    G80DmaNext (pNv, x);
    G80DmaNext (pNv, y);
    G80DmaNext (pNv, w);
    G80DmaNext (pNv, h);
}

#ifdef HAVE_XAA_H
/* Screen to screen copies */

static void
G80SetupForScreenToScreenCopy(
    ScrnInfoPtr pScrn,
    int xdir, int ydir,
    int rop,
    unsigned planemask,
    int transparency_color
)
{
    G80Ptr pNv = G80PTR(pScrn);

    planemask |= ~0 << pScrn->depth;

    G80SetClip(pNv, 0, 0, 0x7fff, 0x7fff);
    G80DmaStart(pNv, 0x2ac, 1);
    if(rop == GXcopy && planemask == ~0) {
        G80DmaNext (pNv, 3);
    } else {
        G80DmaNext (pNv, 4);
        G80SetRopSolid(pNv, rop, planemask);
    }
    pNv->DMAKickoffCallback = G80DMAKickoffCallback;
}

static void
G80SubsequentScreenToScreenCopy(
    ScrnInfoPtr pScrn,
    int x1, int y1,
    int x2, int y2,
    int w, int h
)
{
    G80Ptr pNv = G80PTR(pScrn);

    G80DmaStart(pNv, 0x110, 1);
    G80DmaNext (pNv, 0);
    G80DmaStart(pNv, 0x8b0, 12);
    G80DmaNext (pNv, x2);
    G80DmaNext (pNv, y2);
    G80DmaNext (pNv, w);
    G80DmaNext (pNv, h);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, 1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, 1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, x1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, y1);

    if(w * h >= 512)
        G80DmaKickoff(pNv);
}

/* Solid fills */

static void
G80SetupForSolidFill(
    ScrnInfoPtr pScrn,
    int color,
    int rop,
    unsigned planemask
)
{
    G80Ptr pNv = G80PTR(pScrn);

    planemask |= ~0 << pScrn->depth;

    G80SetClip(pNv, 0, 0, 0x7fff, 0x7fff);
    G80DmaStart(pNv, 0x2ac, 1);
    G80DmaNext (pNv, 4);
    G80SetRopSolid(pNv, rop, planemask);
    G80DmaStart(pNv, 0x580, 1);
    G80DmaNext (pNv, 4);
    G80DmaStart(pNv, 0x588, 1);
    G80DmaNext (pNv, color);

    pNv->DMAKickoffCallback = G80DMAKickoffCallback;
}

static void
G80SubsequentFillRect(ScrnInfoPtr pScrn, int x, int y, int w, int h)
{
    G80Ptr pNv = G80PTR(pScrn);

    G80DmaStart(pNv, 0x600, 4);
    G80DmaNext (pNv, x);
    G80DmaNext (pNv, y);
    G80DmaNext (pNv, x + w);
    G80DmaNext (pNv, y + h);

    if(w * h >= 512)
        G80DmaKickoff(pNv);
}

/* 8x8 pattern fills */

static void
G80SetupForMono8x8PatternFill(
    ScrnInfoPtr pScrn,
    int patternx, int patterny,
    int fg, int bg,
    int rop,
    unsigned planemask
)
{
    G80Ptr pNv = G80PTR(pScrn);
    static const int rops[] = {
        0x00, 0xA0, 0x50, 0xF0, 0x0A, 0xAA, 0x5A, 0xFA, 0x05, 0xA5, 0x55, 0xF5,
        0x0F, 0xAF, 0x5F, 0xFF
    };

    planemask = ~0 << pScrn->depth;

    fg |= planemask;
    if(bg == -1) bg = 0;
    else bg |= planemask;

    if(pNv->currentRop != (rop + 16)) {
        G80DmaStart(pNv, 0x2a0, 1);
        G80DmaNext (pNv, rops[rop]);
        pNv->currentRop = rop + 16;
    }

    G80SetClip(pNv, 0, 0, 0x7fff, 0x7fff);
    G80SetPattern(pNv, bg, fg, patternx, patterny);

    G80DmaStart(pNv, 0x2ac, 1);
    G80DmaNext (pNv, 4);
    G80DmaStart(pNv, 0x580, 1);
    G80DmaNext (pNv, 4);
    G80DmaStart(pNv, 0x588, 1);
    G80DmaNext (pNv, fg);

    pNv->DMAKickoffCallback = G80DMAKickoffCallback;
}

static void
G80SubsequentMono8x8PatternFillRect(
    ScrnInfoPtr pScrn,
    int patternx, int patterny,
    int x, int y,
    int w, int h
)
{
    G80SubsequentFillRect(pScrn, x, y, w, h);
}

/* Color expansion fills */

static CARD32 _color_expand_dwords;
static int _remaining;
static unsigned char *_storage_buffer[1];

static void
G80SetupForScanlineCPUToScreenColorExpandFill(
    ScrnInfoPtr pScrn,
    int fg, int bg,
    int rop,
    unsigned int planemask
)
{
    G80Ptr pNv = G80PTR(pScrn);
    CARD32 mask = ~0 << pScrn->depth;

    planemask |= mask;

    G80DmaStart(pNv, 0x2ac, 1);
    G80DmaNext (pNv, 1);
    G80SetRopSolid(pNv, rop, planemask);
    G80DmaStart(pNv, 0x800, 1);
    G80DmaNext (pNv, 1);
    G80DmaStart(pNv, 0x808, 6);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, 1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, bg | mask);
    G80DmaNext (pNv, fg | mask);
    G80DmaNext (pNv, (bg == -1) ? 0 : 1);
}

static void
G80SubsequentScanlineCPUToScreenColorExpandFill(
    ScrnInfoPtr pScrn,
    int x, int y,
    int w, int h,
    int skipleft
)
{
    G80Ptr pNv = G80PTR(pScrn);
    int bw = (w + 31) & ~31;

    _color_expand_dwords = bw >> 5;
    _remaining = h;

    G80SetClip(pNv, x + skipleft, y, w - skipleft, h);

    G80DmaStart(pNv, 0x838, 10);
    G80DmaNext (pNv, bw);
    G80DmaNext (pNv, h);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, 1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, 1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, x);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, y);

    G80DmaStart(pNv, 0x40000860, _color_expand_dwords);
    _storage_buffer[0] = (unsigned char*)&pNv->dmaBase[pNv->dmaCurrent];
}

static void
G80SubsequentColorExpandScanline(ScrnInfoPtr pScrn, int bufno)
{
    G80Ptr pNv = G80PTR(pScrn);

    pNv->dmaCurrent += _color_expand_dwords;

    if(--_remaining) {
        G80DmaStart(pNv, 0x40000860, _color_expand_dwords);
        _storage_buffer[0] = (unsigned char*)&pNv->dmaBase[pNv->dmaCurrent];
    } else {
        G80DmaKickoff(pNv);
    }
}

/* Scaline image write */

static void
G80SetupForScanlineImageWrite(
    ScrnInfoPtr pScrn, int rop,
    unsigned int planemask,
    int trans_color,
    int bpp, int depth
)
{
    G80Ptr pNv = G80PTR(pScrn);

    planemask |= ~0 << pScrn->depth;

    G80DmaStart(pNv, 0x2ac, 1);
    if(rop == GXcopy && planemask == ~0) {
        G80DmaNext (pNv, 3);
    } else {
        G80DmaNext (pNv, 4);
        G80SetRopSolid(pNv, rop, planemask);
    }

    G80DmaStart(pNv, 0x800, 1);
    G80DmaNext (pNv, 0);
}

static CARD32 _image_dwords;

static void
G80SubsequentScanlineImageWriteRect(
    ScrnInfoPtr pScrn,
    int x, int y,
    int w, int h,
    int skipleft
)
{
    G80Ptr pNv = G80PTR(pScrn);
    int Bpp = pScrn->bitsPerPixel >> 3;

    _remaining = h;
    _image_dwords = (w * Bpp + 3) / 4;

    G80SetClip(pNv, x + skipleft, y, w - skipleft, h);

    G80DmaStart(pNv, 0x838, 10);
    G80DmaNext (pNv, w);
    G80DmaNext (pNv, h);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, 1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, 1);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, x);
    G80DmaNext (pNv, 0);
    G80DmaNext (pNv, y);

    G80DmaStart(pNv, 0x40000860, _image_dwords);
    _storage_buffer[0] = (unsigned char*)&pNv->dmaBase[pNv->dmaCurrent];
}

static void G80SubsequentImageWriteScanline(ScrnInfoPtr pScrn, int bufno)
{
    G80Ptr pNv = G80PTR(pScrn);

    pNv->dmaCurrent += _image_dwords;

    if(--_remaining) {
        G80DmaStart(pNv, 0x40000860, _image_dwords);
        _storage_buffer[0] = (unsigned char*)&pNv->dmaBase[pNv->dmaCurrent];
    } else {
        G80DmaKickoff(pNv);
    }
}

/* Solid lines */

static void
G80SetupForSolidLine(ScrnInfoPtr pScrn, int color, int rop, unsigned planemask)
{
    G80Ptr pNv = G80PTR(pScrn);

    planemask |= ~0 << pScrn->depth;

    G80SetClip(pNv, 0, 0, 0x7fff, 0x7fff);
    G80DmaStart(pNv, 0x2ac, 1);
    G80DmaNext (pNv, 4);
    G80SetRopSolid(pNv, rop, planemask);
    G80DmaStart(pNv, 0x580, 1);
    G80DmaNext (pNv, 1);
    G80DmaStart(pNv, 0x588, 1);
    G80DmaNext (pNv, color);

    pNv->DMAKickoffCallback = G80DMAKickoffCallback;
}

static void
G80SubsequentSolidHorVertLine(ScrnInfoPtr pScrn, int x, int y, int len, int dir)
{
    G80Ptr pNv = G80PTR(pScrn);

    G80DmaStart(pNv, 0x400005e0, 2);
    G80DmaNext (pNv, (y << 16) | (x & 0xffff));
    if(dir == DEGREES_0) {
       G80DmaNext (pNv, (y << 16) | ((x + len) & 0xffff));
    } else {
       G80DmaNext (pNv, ((y + len) << 16) | (x & 0xffff));
    }
}

static void
G80SubsequentSolidTwoPointLine(
    ScrnInfoPtr pScrn,
    int x1, int y1,
    int x2, int y2,
    int flags
)
{
    G80Ptr pNv = G80PTR(pScrn);
    Bool drawLast = !(flags & OMIT_LAST);

    G80DmaStart(pNv, 0x400005e0, drawLast ? 4 : 2);
    G80DmaNext (pNv, (y1 << 16) | (x1 & 0xffff));
    G80DmaNext (pNv, (y2 << 16) | (x2 & 0xffff));
    if(drawLast) {
        G80DmaNext (pNv, (y2 << 16) | (x2 & 0xffff));
        G80DmaNext (pNv, ((y2 + 1) << 16) | (x2 & 0xffff));
    }
}

static void
G80SetClippingRectangle(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2)
{
    G80Ptr pNv = G80PTR(pScrn);
    int h = y2 - y1 + 1;
    int w = x2 - x1 + 1;

    G80SetClip(pNv, x1, y1, w, h);
}

static void
G80DisableClipping(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);

    G80SetClip(pNv, 0, 0, 0x7fff, 0x7fff);
}
#endif

Bool
G80XAAInit(ScreenPtr pScreen)
{
#ifdef HAVE_XAA_H
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    G80Ptr pNv = G80PTR(pScrn);
    XAAInfoRecPtr xaa;

    xaa = pNv->xaa = XAACreateInfoRec();
    if(!xaa) return FALSE;

    xaa->Flags = LINEAR_FRAMEBUFFER | PIXMAP_CACHE | OFFSCREEN_PIXMAPS;
    xaa->Sync = G80Sync;

    /* Screen to screen copies */
    xaa->ScreenToScreenCopyFlags = NO_TRANSPARENCY;
    xaa->SetupForScreenToScreenCopy = G80SetupForScreenToScreenCopy;
    xaa->SubsequentScreenToScreenCopy = G80SubsequentScreenToScreenCopy;

    /* Solid fills */
    xaa->SolidFillFlags = 0;
    xaa->SetupForSolidFill = G80SetupForSolidFill;
    xaa->SubsequentSolidFillRect = G80SubsequentFillRect;

    /* 8x8 pattern fills */
    xaa->Mono8x8PatternFillFlags = HARDWARE_PATTERN_SCREEN_ORIGIN |
                                   HARDWARE_PATTERN_PROGRAMMED_BITS |
                                   NO_PLANEMASK;
    xaa->SetupForMono8x8PatternFill = G80SetupForMono8x8PatternFill;
    xaa->SubsequentMono8x8PatternFillRect = G80SubsequentMono8x8PatternFillRect;

    /* Color expansion fills */
    xaa->ScanlineCPUToScreenColorExpandFillFlags =
        BIT_ORDER_IN_BYTE_LSBFIRST |
        CPU_TRANSFER_PAD_DWORD |
        LEFT_EDGE_CLIPPING |
        LEFT_EDGE_CLIPPING_NEGATIVE_X;
    xaa->NumScanlineColorExpandBuffers = 1;
    xaa->SetupForScanlineCPUToScreenColorExpandFill =
        G80SetupForScanlineCPUToScreenColorExpandFill;
    xaa->SubsequentScanlineCPUToScreenColorExpandFill =
        G80SubsequentScanlineCPUToScreenColorExpandFill;
    xaa->SubsequentColorExpandScanline =
        G80SubsequentColorExpandScanline;
    xaa->ScanlineColorExpandBuffers = _storage_buffer;

    /* Scaline image write */
    xaa->ScanlineImageWriteFlags = NO_GXCOPY |
                                   NO_TRANSPARENCY |
                                   LEFT_EDGE_CLIPPING |
                                   LEFT_EDGE_CLIPPING_NEGATIVE_X;
    xaa->NumScanlineImageWriteBuffers = 1;
    xaa->SetupForScanlineImageWrite = G80SetupForScanlineImageWrite;
    xaa->SubsequentScanlineImageWriteRect = G80SubsequentScanlineImageWriteRect;
    xaa->SubsequentImageWriteScanline = G80SubsequentImageWriteScanline;
    xaa->ScanlineImageWriteBuffers = _storage_buffer;

    /* Solid lines */
    xaa->SolidLineFlags = 0;
    xaa->SetupForSolidLine = G80SetupForSolidLine;
    xaa->SubsequentSolidHorVertLine = G80SubsequentSolidHorVertLine;
    xaa->SubsequentSolidTwoPointLine = G80SubsequentSolidTwoPointLine;
    xaa->SetClippingRectangle = G80SetClippingRectangle;
    xaa->DisableClipping = G80DisableClipping;
    xaa->ClippingFlags = HARDWARE_CLIP_SOLID_LINE;

    miSetZeroLineBias(pScreen, OCTANT1 | OCTANT3 | OCTANT4 | OCTANT6);

    return XAAInit(pScreen, xaa);
#else
    return FALSE;
#endif
}
