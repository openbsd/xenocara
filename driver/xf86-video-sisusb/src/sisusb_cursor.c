/* $XFree86$ */
/* $XdotOrg: driver/xf86-video-sisusb/src/sisusb_cursor.c,v 1.7 2005/09/28 18:59:22 twini Exp $ */
/*
 * SiS 315/USB hardware cursor handling
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1) Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2) Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3) The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author:   Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sisusb.h"
#include "cursorstr.h"
#include "sisusb_regs.h"
#include "sisusb_cursor.h"

extern void    SISUSBWaitRetraceCRT1(ScrnInfoPtr pScrn);

static void
SiSUSBHideCursor(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    pSiSUSB->HWCursorIsVisible = FALSE;

    sisusbDisableHWCursor()
    sisusbSetCursorPositionY(2000, 0)
}

static void
SiSUSBShowCursor(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    if(pSiSUSB->HideHWCursor) {
       SiSUSBHideCursor(pScrn);
       pSiSUSB->HWCursorIsVisible = TRUE;
       return;
    }

    pSiSUSB->HWCursorIsVisible = TRUE;

    if(pSiSUSB->UseHWARGBCursor) {
       sisusbEnableHWARGBCursor()
    } else {
       sisusbEnableHWCursor()
    }
}

static void
SiSUSBSetCursorPosition(ScrnInfoPtr pScrn, int x, int y)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    DisplayModePtr mode = pSiSUSB->CurrentLayout.mode;
    UShort x_preset = 0;
    UShort y_preset = 0;

    if(x < 0) {
       x_preset = (-x);
       x = 0;
    }
    if(y < 0) {
       y_preset = (-y);
       y = 0;
    }

    if(mode->Flags & V_INTERLACE)     y /= 2;
    else if(mode->Flags & V_DBLSCAN)  y *= 2;

    sisusbSetCursorPositionX(x, x_preset)
    sisusbSetCursorPositionY(y, y_preset)
}


static void
SiSUSBSetCursorColors(ScrnInfoPtr pScrn, int bg, int fg)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    if(pSiSUSB->UseHWARGBCursor) return;

    sisusbSetCursorBGColor(bg)
    sisusbSetCursorFGColor(fg)
}

static void
SiSUSBLoadCursorImage(ScrnInfoPtr pScrn, UChar *src)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    ULong cursor_addr;
    CARD32 status1 = 0;
    UChar *dest = pSiSUSB->FbBase;
    int bufnum, i;

    pSiSUSB->HWCursorMBufNum ^= 1;
    bufnum = 1 << pSiSUSB->HWCursorMBufNum;

    cursor_addr = pScrn->videoRam - pSiSUSB->cursorOffset - ((pSiSUSB->CursorSize/1024) * bufnum);

    if(pSiSUSB->CurrentLayout.mode->Flags & V_DBLSCAN) {
       UChar *mysrc = (UChar *)pSiSUSB->USBCursorBuf +
				(pSiSUSB->CursorSize * 4) - (pSiSUSB->CursorSize * bufnum);
       for(i = 0; i < 32; i++) {
          memcpy(mysrc + (32 * i)     , src + (16 * i), 16);
	  memcpy(mysrc + (32 * i) + 16, src + (16 * i), 16);
       }
       SiSUSBMemCopyToVideoRam(pSiSUSB, (UChar *)dest + (cursor_addr * 1024), mysrc, 1024);
    } else {
       SiSUSBMemCopyToVideoRam(pSiSUSB, (UChar *)dest + (cursor_addr * 1024), src, 1024);
    }

    if(pSiSUSB->UseHWARGBCursor) {

       status1 = sisusbGetCursorStatus;
       sisusbDisableHWCursor()

       SISUSBWaitRetraceCRT1(pScrn);
       sisusbSwitchToMONOCursor();

    } else  {

       SISUSBWaitRetraceCRT1(pScrn);

    }

    sisusbSetCursorAddress(cursor_addr);
    if(status1) sisusbSetCursorStatus(status1)

    pSiSUSB->UseHWARGBCursor = FALSE;
}

static Bool
SiSUSBUseHWCursor(ScreenPtr pScreen, CursorPtr pCurs)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    DisplayModePtr mode = pSiSUSB->CurrentLayout.mode;

    if((mode->Flags & V_DBLSCAN) && (pCurs->bits->height > 32))
       return FALSE;

    return TRUE;
}

#if XF86_VERSION_CURRENT >= XF86_VERSION_NUMERIC(4,2,99,0,0)
#ifdef ARGB_CURSOR
#ifdef SIS_ARGB_CURSOR
static Bool
SiSUSBUseHWCursorARGB(ScreenPtr pScreen, CursorPtr pCurs)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    DisplayModePtr mode = pSiSUSB->CurrentLayout.mode;

    if((pCurs->bits->height > 64) || (pCurs->bits->width > 64))
       return FALSE;
    if((mode->Flags & V_DBLSCAN) && (pCurs->bits->height > 32))
       return FALSE;

    return TRUE;
}

static void SiSUSBLoadCursorImageARGB(ScrnInfoPtr pScrn, CursorPtr pCurs)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    int cursor_addr, i, j, maxheight = 64;
    CARD32 *src = pCurs->bits->argb, *p, *pb, *dest;
    int srcwidth = pCurs->bits->width;
    int srcheight = pCurs->bits->height;
    CARD32 status1 = 0;
    Bool sizedouble = FALSE;
    int bufnum;
    CARD32 *mysrc;

    if(pSiSUSB->CurrentLayout.mode->Flags & V_DBLSCAN) {
       sizedouble = TRUE;
    }

    pSiSUSB->HWCursorCBufNum ^= 1;
    bufnum = 1 << pSiSUSB->HWCursorCBufNum;

    cursor_addr = pScrn->videoRam - pSiSUSB->cursorOffset - ((pSiSUSB->CursorSize/1024) * (2 + bufnum));

    if(srcwidth > 64)  srcwidth = 64;
    if(srcheight > 64) srcheight = 64;

    dest = mysrc = (CARD32 *)((UChar *)pSiSUSB->USBCursorBuf +
    			(pSiSUSB->CursorSize * 4) - (pSiSUSB->CursorSize * (2 + bufnum)));

    if(sizedouble) {
       if(srcheight > 32) srcheight = 32;
       maxheight = 32;
    }

    for(i = 0; i < srcheight; i++) {
       p = src;
       pb = dest;
       src += pCurs->bits->width;
       for(j = 0; j < srcwidth; j++) sisfbwritelpinc(dest, p); /* *dest++ = *p++; */
       if(srcwidth < 64) {
          for(; j < 64; j++) sisfbwritelinc(dest, 0);  /* *dest++ = 0; */
       }
       if(sizedouble) {
          for(j = 0; j < 64; j++) {
             sisfbwritelinc(dest, sisfbreadlinc(pb)); /* *dest++ = *pb++; */
          }
       }
    }
    if(srcheight < maxheight) {
       for(; i < maxheight; i++) {
	  for(j = 0; j < 64; j++) sisfbwritelinc(dest, 0); /* *dest++ = 0; */
	  if(sizedouble) {
	     for(j = 0; j < 64; j++) sisfbwritelinc(dest, 0); /* *dest++ = 0; */
	  }
       }
    }

    SiSUSBMemCopyToVideoRam(pSiSUSB, (UChar *)pSiSUSB->FbBase + (cursor_addr * 1024),
    						(UChar *)mysrc, pSiSUSB->CursorSize);

    if(!pSiSUSB->UseHWARGBCursor) {
       status1 = sisusbGetCursorStatus;
       sisusbDisableHWCursor()
       SISUSBWaitRetraceCRT1(pScrn);
       sisusbSwitchToRGBCursor();
    }

    sisusbSetCursorAddress(cursor_addr);
    if(status1) sisusbSetCursorStatus(status1)

    pSiSUSB->UseHWARGBCursor = TRUE;
}
#endif
#endif
#endif

Bool
SiSUSBHWCursorInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    xf86CursorInfoPtr infoPtr;

    infoPtr = xf86CreateCursorInfoRec();
    if(!infoPtr) return FALSE;

    pSiSUSB->CursorInfoPtr = infoPtr;
    pSiSUSB->UseHWARGBCursor = FALSE;

    infoPtr->MaxWidth  = 64;
    infoPtr->MaxHeight = 64;
    infoPtr->ShowCursor = SiSUSBShowCursor;
    infoPtr->HideCursor = SiSUSBHideCursor;
    infoPtr->SetCursorPosition = SiSUSBSetCursorPosition;
    infoPtr->SetCursorColors = SiSUSBSetCursorColors;
    infoPtr->LoadCursorImage = SiSUSBLoadCursorImage;
    infoPtr->UseHWCursor = SiSUSBUseHWCursor;
#if XF86_VERSION_CURRENT >= XF86_VERSION_NUMERIC(4,2,99,0,0)
#ifdef ARGB_CURSOR
#ifdef SIS_ARGB_CURSOR
    if(pSiSUSB->OptUseColorCursor) {
       infoPtr->UseHWCursorARGB = SiSUSBUseHWCursorARGB;
       infoPtr->LoadCursorARGB = SiSUSBLoadCursorImageARGB;
    }
#endif
#endif
#endif
    infoPtr->Flags =
            HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
            HARDWARE_CURSOR_INVERT_MASK |
            HARDWARE_CURSOR_BIT_ORDER_MSBFIRST |
            HARDWARE_CURSOR_AND_SOURCE_WITH_MASK |
            HARDWARE_CURSOR_SWAP_SOURCE_AND_MASK |
            HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_64 |
            HARDWARE_CURSOR_UPDATE_UNHIDDEN;

    return(xf86InitCursor(pScreen, infoPtr));
}
