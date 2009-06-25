/*
 * Copyright 2007 The Openchrome Project [openchrome.org]
 * Copyright 1998-2007 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2003 S3 Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*************************************************************************
 *
 *  File:       via_cursor.c
 *  Content:    Hardware cursor support for VIA/S3G UniChrome
 *
 ************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via.h"
#include "via_driver.h"
#include "via_id.h"
#include "cursorstr.h"

void viaShowCursor(ScrnInfoPtr pScrn);
void viaHideCursor(ScrnInfoPtr pScrn);
static void viaSetCursorPosition(ScrnInfoPtr pScrn, int x, int y);
static Bool viaUseHWCursor(ScreenPtr pScreen, CursorPtr pCurs);
static Bool viaUseHWCursorARGB(ScreenPtr pScreen, CursorPtr pCurs);
static void viaLoadCursorImage(ScrnInfoPtr pScrn, unsigned char *src);
static void viaSetCursorColors(ScrnInfoPtr pScrn, int bg, int fg);
static void viaLoadCursorARGB(ScrnInfoPtr pScrn, CursorPtr pCurs);

static CARD32 mono_cursor_color[] = {
	0x00000000,
	0x00000000,
	0xffffffff,
	0xff000000,
};

Bool
viaHWCursorInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    VIAPtr pVia = VIAPTR(pScrn);
    xf86CursorInfoPtr infoPtr;

	switch (pVia->Chipset) {
		case VIA_CLE266:
		case VIA_KM400:
			/* FIXME Mono HW Cursors not working */
			pVia->hwcursor = FALSE;
			pVia->CursorARGBSupported = FALSE;
			pVia->CursorMaxWidth = 32;
			pVia->CursorMaxHeight = 32;
			pVia->CursorSize = ((pVia->CursorMaxWidth * pVia->CursorMaxHeight) / 8) * 2;
			break;
		default:
			pVia->CursorARGBSupported = TRUE;
			pVia->CursorMaxWidth = 64;
			pVia->CursorMaxHeight = 64;
			pVia->CursorSize = pVia->CursorMaxWidth * (pVia->CursorMaxHeight + 1) * 4;
			break;
    }

    if (pVia->NoAccel) 
    	viaCursorSetFB(pScrn);

    pVia->cursorMap = pVia->FBBase + pVia->CursorStart;

    if (pVia->cursorMap == NULL)
		return FALSE;

    pVia->cursorOffset = pScrn->fbOffset + pVia->CursorStart;
    memset(pVia->cursorMap, 0x00, pVia->CursorSize);

    switch (pVia->Chipset) {
        case VIA_CX700:
        /* case VIA_CN750: */
        case VIA_P4M890:
        case VIA_P4M900:
        case VIA_VX800:
			if (pVia->pBIOSInfo->FirstCRTC->IsActive) {
				pVia->CursorRegControl  = VIA_REG_HI_CONTROL0;
				pVia->CursorRegBase     = VIA_REG_HI_BASE0;
				pVia->CursorRegPos      = VIA_REG_HI_POS0;
				pVia->CursorRegOffset   = VIA_REG_HI_OFFSET0;
				pVia->CursorRegFifo     = VIA_REG_HI_FIFO0;
				pVia->CursorRegTransKey = VIA_REG_HI_TRANSKEY0;
			}
			if (pVia->pBIOSInfo->SecondCRTC->IsActive) {
				pVia->CursorRegControl  = VIA_REG_HI_CONTROL1;
				pVia->CursorRegBase     = VIA_REG_HI_BASE1;
				pVia->CursorRegPos      = VIA_REG_HI_POS1;
				pVia->CursorRegOffset   = VIA_REG_HI_OFFSET1;
				pVia->CursorRegFifo     = VIA_REG_HI_FIFO1;
				pVia->CursorRegTransKey = VIA_REG_HI_TRANSKEY1;
			}
			break;
		default:
			pVia->CursorRegControl = VIA_REG_ALPHA_CONTROL;
			pVia->CursorRegBase = VIA_REG_ALPHA_BASE;
			pVia->CursorRegPos = VIA_REG_ALPHA_POS;
			pVia->CursorRegOffset = VIA_REG_ALPHA_OFFSET;
			pVia->CursorRegFifo = VIA_REG_ALPHA_FIFO;
			pVia->CursorRegTransKey = VIA_REG_ALPHA_TRANSKEY;
    }

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "VIAHWCursorInit\n"));
    infoPtr = xf86CreateCursorInfoRec();
    if (!infoPtr)
        return FALSE;

    pVia->CursorInfoRec = infoPtr;

    infoPtr->MaxWidth = pVia->CursorMaxWidth;
    infoPtr->MaxHeight = pVia->CursorMaxHeight;
    infoPtr->Flags = (HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_1 |
                      HARDWARE_CURSOR_AND_SOURCE_WITH_MASK |
                      HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
                      0);

    infoPtr->SetCursorColors = viaSetCursorColors;
    infoPtr->SetCursorPosition = viaSetCursorPosition;
    infoPtr->LoadCursorImage = viaLoadCursorImage;
    infoPtr->HideCursor = viaHideCursor;
    infoPtr->ShowCursor = viaShowCursor;
    infoPtr->UseHWCursor = viaUseHWCursor;

    infoPtr->UseHWCursorARGB = viaUseHWCursorARGB;
	if (pVia->CursorARGBSupported)
    	infoPtr->LoadCursorARGB = viaLoadCursorARGB;

    /* Set cursor location in frame buffer. */
    VIASETREG(VIA_REG_CURSOR_MODE, pVia->cursorOffset);

    pVia->CursorPipe = (pVia->pBIOSInfo->Panel->IsActive) ? 1 : 0;

    /* Init HI_X0 */
    VIASETREG(pVia->CursorRegControl, 0);
    VIASETREG(pVia->CursorRegBase, pVia->cursorOffset);
    VIASETREG(pVia->CursorRegTransKey, 0);

    switch (pVia->Chipset) {
        case VIA_CX700:
        /* case VIA_CN750: */
        case VIA_P4M890:
        case VIA_P4M900:
        case VIA_VX800:
			if (pVia->pBIOSInfo->FirstCRTC->IsActive) {
				VIASETREG(VIA_REG_PRIM_HI_INVTCOLOR, 0x00FFFFFF);
				VIASETREG(VIA_REG_V327_HI_INVTCOLOR, 0x00FFFFFF);
				VIASETREG(pVia->CursorRegFifo, 0x0D000D0F);
			}
			if (pVia->pBIOSInfo->SecondCRTC->IsActive) {
				VIASETREG(VIA_REG_HI_INVTCOLOR, 0X00FFFFFF);
				VIASETREG(VIA_REG_ALPHA_PREFIFO, 0xE0000);
				VIASETREG(pVia->CursorRegFifo, 0xE0F0000);

				/* Just in case */
				VIASETREG(VIA_REG_HI_BASE0, pVia->cursorOffset);
			}
			break;
    	default:
			VIASETREG(VIA_REG_HI_INVTCOLOR, 0X00FFFFFF);
			VIASETREG(VIA_REG_ALPHA_PREFIFO, 0xE0000);
			VIASETREG(pVia->CursorRegFifo, 0xE0F0000);
	}

    return xf86InitCursor(pScreen, infoPtr);
}

void
viaCursorSetFB(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    if ((pVia->FBFreeEnd - pVia->FBFreeStart) > pVia->CursorSize) {
        pVia->CursorStart = pVia->FBFreeEnd - pVia->CursorSize;
        pVia->FBFreeEnd = pVia->CursorStart;
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "CursorStart: 0x%x\n", pVia->CursorStart);
    }
}


void
viaCursorStore(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaCursorStore\n"));

    if (pVia->CursorPipe) {
		pVia->CursorControl1 = VIAGETREG(pVia->CursorRegControl);
    } else {
		pVia->CursorControl0 = VIAGETREG(pVia->CursorRegControl);
    }

    pVia->CursorTransparentKey = VIAGETREG(pVia->CursorRegTransKey);


    switch (pVia->Chipset) {
        case VIA_CX700:
        /* case VIA_CN750: */
        case VIA_P4M890:
        case VIA_P4M900:
        case VIA_VX800:
			if (pVia->pBIOSInfo->FirstCRTC->IsActive) {
	    		pVia->CursorPrimHiInvtColor = VIAGETREG(VIA_REG_PRIM_HI_INVTCOLOR);
	    		pVia->CursorV327HiInvtColor = VIAGETREG(VIA_REG_V327_HI_INVTCOLOR);
			} 
			if (pVia->pBIOSInfo->SecondCRTC->IsActive) {
	    	/* TODO add saves here */
			}
			pVia->CursorFifo = VIAGETREG(pVia->CursorRegFifo);
			break;
		default:
			/* TODO add saves here */
			break;
    }
}

void
viaCursorRestore(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaCursorRestore\n"));

    if (pVia->CursorPipe) {
		VIASETREG(pVia->CursorRegControl, pVia->CursorControl1);
    } else {
		VIASETREG(pVia->CursorRegControl, pVia->CursorControl0);
    }

    VIASETREG(pVia->CursorRegBase, pVia->cursorOffset);

    VIASETREG(pVia->CursorRegTransKey, pVia->CursorTransparentKey);


    switch (pVia->Chipset) {
        case VIA_CX700:
        /* case VIA_CN750: */
        case VIA_P4M890:
        case VIA_P4M900:
        case VIA_VX800:
			if (pVia->pBIOSInfo->FirstCRTC->IsActive) {
	    		VIASETREG(VIA_REG_PRIM_HI_INVTCOLOR, pVia->CursorPrimHiInvtColor);
	    		VIASETREG(VIA_REG_V327_HI_INVTCOLOR, pVia->CursorV327HiInvtColor);
			}
			if (pVia->pBIOSInfo->SecondCRTC->IsActive) {
	    		/* TODO add real restores here */
	    		VIASETREG(VIA_REG_HI_INVTCOLOR, 0X00FFFFFF);
	    		VIASETREG(VIA_REG_ALPHA_PREFIFO, 0xE0000);
			}
			VIASETREG(pVia->CursorRegFifo, pVia->CursorFifo);
			break;
		default:
			/* TODO add real restores here */
			VIASETREG(VIA_REG_ALPHA_PREFIFO, 0xE0000);
			VIASETREG(pVia->CursorRegFifo, 0xE0F0000);
    }
}

/*
 * ARGB Cursor
 */

void
viaShowCursor(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD32 temp;
    CARD32 control = pVia->CursorRegControl;

    temp =
	(1 << 30) |
	(1 << 29) |
	(1 << 28) |
	(1 << 26) |
	(1 << 25) |
	(1 <<  2) |
	(1 <<  0);

    if (pVia->CursorPipe)
	temp |= (1 << 31);

    VIASETREG(control, temp);
}

void
viaHideCursor(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD32 temp;
    CARD32 control = pVia->CursorRegControl;

    temp = VIAGETREG(control);
    VIASETREG(control, temp & 0xFFFFFFFE);
}

static void
viaSetCursorPosition(ScrnInfoPtr pScrn, int x, int y)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD32 temp;
    CARD32 control = pVia->CursorRegControl;
    CARD32 offset = pVia->CursorRegOffset;
    CARD32 pos = pVia->CursorRegPos;
    unsigned xoff, yoff;

    if (x < 0) {
	xoff = ((-x) & 0xFE);
	x = 0;
    } else {
	xoff = 0;
    }

    if (y < 0) {
	yoff = ((-y) & 0xFE);
	y = 0;
    } else {
	yoff = 0;
    }

    temp = VIAGETREG(control);
    VIASETREG(control, temp & 0xFFFFFFFE);

    VIASETREG(pos,    ((x    << 16) | (y    & 0x07ff)));
    VIASETREG(offset, ((xoff << 16) | (yoff & 0x07ff)));

    VIASETREG(control, temp);
}

static Bool
viaUseHWCursorARGB(ScreenPtr pScreen, CursorPtr pCurs)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    VIAPtr pVia = VIAPTR(pScrn);

    return (pVia->hwcursor
            && pVia->CursorARGBSupported
            && pCurs->bits->width <= pVia->CursorMaxWidth
            && pCurs->bits->height <= pVia->CursorMaxHeight);
}

static Bool
viaUseHWCursor(ScreenPtr pScreen, CursorPtr pCurs)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    VIAPtr pVia = VIAPTR(pScrn);

    return (pVia->hwcursor
            /* Can't enable HW cursor on both CRTCs at the same time. */
            && !(pVia->pBIOSInfo->FirstCRTC->IsActive
                 && pVia->pBIOSInfo->SecondCRTC->IsActive)
            && pCurs->bits->width <= pVia->CursorMaxWidth
            && pCurs->bits->height <= pVia->CursorMaxHeight);
}

static void
viaLoadCursorImage(ScrnInfoPtr pScrn, unsigned char *s)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD32 control = pVia->CursorRegControl;
    CARD32 temp;
    CARD32 *dst;
    CARD8 *src;
    CARD8 chunk;
    int i, j;

    temp = VIAGETREG(control);
    VIASETREG(control, temp & 0xFFFFFFFE);

    pVia->CursorARGB = FALSE;

    dst = (CARD32*)(pVia->cursorMap);
    src = (CARD8*)s;

    if (pVia->CursorARGBSupported) {
#define ARGB_PER_CHUNK	(8 * sizeof (chunk) / 2)
		for (i = 0; i < (pVia->CursorMaxWidth * pVia->CursorMaxHeight / ARGB_PER_CHUNK); i++) {
		chunk = *s++;
		for (j = 0; j < ARGB_PER_CHUNK; j++, chunk >>= 2)
			*dst++ = mono_cursor_color[chunk & 3];
		}

		pVia->CursorFG = mono_cursor_color[3];
		pVia->CursorBG = mono_cursor_color[2];
	} else {
		memcpy(dst, src, pVia->CursorSize);
	}

    VIASETREG(control, temp);
}

static void
viaSetCursorColors(ScrnInfoPtr pScrn, int bg, int fg)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD32 control = pVia->CursorRegControl;
    CARD32 pixel;
    CARD32 temp;
    CARD32 *dst;
    int i;

    if (pVia->CursorFG)
	return;

    fg |= 0xff000000;
    bg |= 0xff000000;

    if (fg == pVia->CursorFG && bg == pVia->CursorBG)
	return;

    temp = VIAGETREG(control);
    VIASETREG(control, temp & 0xFFFFFFFE);

    dst = (CARD32*)pVia->cursorMap;
    for (i = 0; i < pVia->CursorMaxWidth * pVia->CursorMaxHeight; i++, dst++)
	if ((pixel = *dst))
	    *dst = (pixel == pVia->CursorFG) ? fg : bg;

    pVia->CursorFG = fg;
    pVia->CursorBG = bg;

    VIASETREG(control, temp);
}

static void
viaLoadCursorARGB(ScrnInfoPtr pScrn, CursorPtr pCurs)
{
    VIAPtr pVia = VIAPTR(pScrn);
    CARD32 control = pVia->CursorRegControl;
    int x, y, w, h;
    CARD32 *image;
    CARD32 *dst;
    CARD32 *src;
    CARD32 temp;

    temp = VIAGETREG(control);
    VIASETREG(control, temp & 0xFFFFFFFE);

    pVia->CursorARGB = TRUE;

    dst = (CARD32*)pVia->cursorMap;
    image = pCurs->bits->argb;

    w = pCurs->bits->width;
    if (w > pVia->CursorMaxWidth)
	w = pVia->CursorMaxWidth;

    h = pCurs->bits->height;
    if (h > pVia->CursorMaxHeight)
	h = pVia->CursorMaxHeight;

    for (y = 0; y < h; y++) {

	src = image;
	image += pCurs->bits->width;

	for (x = 0; x < w; x++)
	    *dst++ = *src++;
	for (; x < pVia->CursorMaxHeight; x++)
	    *dst++ = 0;
    }

    for (; y < pVia->CursorMaxHeight; y++)
	for (x = 0; x < pVia->CursorMaxWidth; x++)
	    *dst++ = 0;

    VIASETREG(control, temp);
}
