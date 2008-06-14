/* Copyright (c) 2003-2007 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Neither the name of the Advanced Micro Devices, Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 * */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Pci.h"
#include "xf86PciInfo.h"
#include "geode.h"

/* Forward declarations of the functions */
static void LXSetCursorColors(ScrnInfoPtr pScrni, int bg, int fg);
static void LXSetCursorPosition(ScrnInfoPtr pScrni, int x, int y);
static Bool LXUseHWCursor(ScreenPtr pScrn, CursorPtr pCurs);
extern void LXSetVideoPosition(int x, int y, int width, int height,
    short src_w, short src_h, short drw_w,
    short drw_h, int id, int offset, ScrnInfoPtr pScrn);

Bool
LXHWCursorInit(ScreenPtr pScrn)
{
    ScrnInfoPtr pScrni = xf86Screens[pScrn->myNum];
    GeodeRec *pGeode = GEODEPTR(pScrni);
    xf86CursorInfoPtr infoPtr;

    infoPtr = xf86CreateCursorInfoRec();
    if (!infoPtr)
	return FALSE;
    /* the geode structure is intiallized with the cursor infoRec */
    pGeode->CursorInfo = infoPtr;
    infoPtr->MaxWidth = 32;
    infoPtr->MaxHeight = 32;
    /* seeting up the cursor flags */
    infoPtr->Flags = HARDWARE_CURSOR_BIT_ORDER_MSBFIRST |
	HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
	HARDWARE_CURSOR_SOURCE_MASK_NOT_INTERLEAVED;

    infoPtr->SetCursorColors = LXSetCursorColors;
    infoPtr->SetCursorPosition = LXSetCursorPosition;
    infoPtr->LoadCursorImage = LXLoadCursorImage;
    infoPtr->HideCursor = LXHideCursor;
    infoPtr->ShowCursor = LXShowCursor;
    infoPtr->UseHWCursor = LXUseHWCursor;

    return (xf86InitCursor(pScrn, infoPtr));
}

static void
LXSetCursorColors(ScrnInfoPtr pScrni, int bg, int fg)
{
    vg_set_mono_cursor_colors(bg, fg);
}

static void
LXSetCursorPosition(ScrnInfoPtr pScrni, int x, int y)
{
    GeodeRec *pGeode = GEODEPTR(pScrni);
    int savex, savey;
    int newX, newY;
    int hsx, hsy;

    /* Adjust xf86HWCursor messing about */

    savex = x + pScrni->frameX0;
    savey = y + pScrni->frameY0;

    switch (pGeode->rotation) {
    default:
	ErrorF("%s:%d invalid rotation %d\n", __func__, __LINE__,
	    pGeode->rotation);
    case RR_Rotate_0:
	newX = savex;
	newY = savey;
	hsx = 31;
	hsy = 31;
	break;

    case RR_Rotate_270:
	newX = savey;
	newY = pScrni->pScreen->width - savex;
	hsx = 31;
	hsy = 0;
	break;

    case RR_Rotate_180:
	newX = pScrni->pScreen->width - savex;
	newY = pScrni->pScreen->height - savey;
	hsx = 0;
	hsy = 0;
	break;

    case RR_Rotate_90:
	newX = pScrni->pScreen->height - savey;
	newY = savex;
	hsx = 0;
	hsy = 31;
	break;
    }

    newX -= pScrni->frameX0;
    newY -= pScrni->frameY0;

    {
	VG_PANNING_COORDINATES panning;

	vg_set_cursor_position(newX + hsx, newY + hsy, &panning);
    }

    vg_set_cursor_enable(1);

    /* FIXME:  Adjust for video panning? */
}

void
LXLoadCursorImage(ScrnInfoPtr pScrni, unsigned char *src)
{
    int i, n, x, y, newX, newY;
    unsigned long andMask[32], xorMask[32];
    GeodeRec *pGeode = GEODEPTR(pScrni);
    unsigned long mskb, rowb;
    unsigned char *rowp = &src[0];
    unsigned char *mskp = &src[128];

    if (src != NULL) {
	mskb = rowb = 0;
	for (y = 32; --y >= 0;)
	    andMask[y] = xorMask[y] = 0;
	for (y = 0; y < 32; ++y) {
	    for (x = 0; x < 32; ++x) {
		if ((i = x & 7) == 0) {
		    rowb = (*rowp & *mskp);
		    mskb = ~(*mskp);
		    ++rowp;
		    ++mskp;
		}

		switch (pGeode->rotation) {
		default:
		    ErrorF("%s:%d invalid rotation %d\n", __func__, __LINE__,
			pGeode->rotation);
		case RR_Rotate_0:
		    newX = x;
		    newY = y;
		    break;
		case RR_Rotate_270:
		    newX = y;
		    newY = 31 - x;
		    break;
		case RR_Rotate_180:
		    newX = 31 - x;
		    newY = 31 - y;
		    break;
		case RR_Rotate_90:
		    newX = 31 - y;
		    newY = x;
		    break;
		}

		i = 7 - i;
		n = 31 - newX;
		andMask[newY] |= (((mskb >> i) & 1) << n);
		xorMask[newY] |= (((rowb >> i) & 1) << n);
	    }
	}
    } else {
	for (y = 32; --y >= 0;) {
	    andMask[y] = ~0;
	    xorMask[y] = 0;
	}
    }

    vg_set_mono_cursor_shape32(pGeode->CursorStartOffset, &andMask[0],
	&xorMask[0], 31, 31);
}

void
LXHideCursor(ScrnInfoPtr pScrni)
{
    vg_set_cursor_enable(0);
}

void
LXShowCursor(ScrnInfoPtr pScrni)
{
    vg_set_cursor_enable(1);
}

static Bool
LXUseHWCursor(ScreenPtr pScrn, CursorPtr pCurs)
{
    ScrnInfoPtr pScrni = XF86SCRNINFO(pScrn);
    GeodeRec *pGeode = GEODEPTR(pScrni);

    return pGeode->HWCursor;
}
