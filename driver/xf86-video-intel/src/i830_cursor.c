
/**************************************************************************

Copyright 1998-1999 Precision Insight, Inc., Cedar Park, Texas.
Copyright © 2002 David Dawes
All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/
/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/i810/i830_cursor.c,v 1.6 2002/12/18 15:49:01 dawes Exp $ */

/*
 * Reformatted with GNU indent (2.2.8), using the following options:
 *
 *    -bad -bap -c41 -cd0 -ncdb -ci6 -cli0 -cp0 -ncs -d0 -di3 -i3 -ip3 -l78
 *    -lp -npcs -psl -sob -ss -br -ce -sc -hnl
 *
 * This provides a good match with the original i810 code and preferred
 * XFree86 formatting conventions.
 *
 * When editing this driver, please follow the existing formatting, and edit
 * with <TAB> characters expanded at 8-column intervals.
 */

/*
 * Authors:
 *   Keith Whitwell <keith@tungstengraphics.com>
 *   David Dawes <dawes@xfree86.org>
 *
 * Updated for Dual Head capabilities:
 *   Alan Hourihane <alanh@tungstengraphics.com>
 *
 * Add ARGB HW cursor support:
 *   Alan Hourihane <alanh@tungstengraphics.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"

#include "xf86fbman.h"

#include "i830.h"

static void I830LoadCursorImage(ScrnInfoPtr pScrn, unsigned char *src);
static void I830ShowCursor(ScrnInfoPtr pScrn);
static void I830HideCursor(ScrnInfoPtr pScrn);
static void I830SetCursorColors(ScrnInfoPtr pScrn, int bg, int fb);
static void I830SetCursorPosition(ScrnInfoPtr pScrn, int x, int y);
static Bool I830UseHWCursor(ScreenPtr pScrn, CursorPtr pCurs);
#ifdef ARGB_CURSOR
static void I830LoadCursorARGB(ScrnInfoPtr pScrn, CursorPtr pCurs);
static Bool I830UseHWCursorARGB(ScreenPtr pScrn, CursorPtr pCurs);
#endif

void
I830InitHWCursor(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   CARD32 temp;

   DPRINTF(PFX, "I830InitHWCursor\n");
   /* Initialise the HW cursor registers, leaving the cursor hidden. */
   if (IS_MOBILE(pI830) || IS_I9XX(pI830)) {
      temp = INREG(CURSOR_A_CONTROL);
      temp &= ~(CURSOR_MODE | MCURSOR_GAMMA_ENABLE | MCURSOR_MEM_TYPE_LOCAL |
		MCURSOR_PIPE_SELECT);
      temp |= CURSOR_MODE_DISABLE;
      temp |= (pI830->pipe << 28);
      if (pI830->CursorIsARGB)
         temp |= CURSOR_MODE_64_ARGB_AX | MCURSOR_GAMMA_ENABLE;
      else
         temp |= CURSOR_MODE_64_4C_AX;
      /* Need to set control, then address. */
      OUTREG(CURSOR_A_CONTROL, temp);
      if (pI830->CursorNeedsPhysical) {
         if (pI830->CursorIsARGB)
            OUTREG(CURSOR_A_BASE, pI830->CursorMemARGB->Physical);
         else
            OUTREG(CURSOR_A_BASE, pI830->CursorMem->Physical);
      } else {
         if (pI830->CursorIsARGB)
            OUTREG(CURSOR_A_BASE, pI830->CursorMemARGB->Start);
         else
            OUTREG(CURSOR_A_BASE, pI830->CursorMem->Start);
      }
      if (pI830->Clone || pI830->MergedFB) {
         temp &= ~MCURSOR_PIPE_SELECT;
         temp |= (!pI830->pipe << 28);
         OUTREG(CURSOR_B_CONTROL, temp);
         if (pI830->CursorNeedsPhysical) {
            if (pI830->CursorIsARGB)
               OUTREG(CURSOR_B_BASE, pI830->CursorMemARGB->Physical);
            else
               OUTREG(CURSOR_B_BASE, pI830->CursorMem->Physical);
	 } else {
            if (pI830->CursorIsARGB)
               OUTREG(CURSOR_B_BASE, pI830->CursorMemARGB->Start);
            else
               OUTREG(CURSOR_B_BASE, pI830->CursorMem->Start);
	 }
      }
   } else {
      temp = INREG(CURSOR_CONTROL);
      temp &= ~(CURSOR_FORMAT_MASK | CURSOR_GAMMA_ENABLE |
		CURSOR_ENABLE  | CURSOR_STRIDE_MASK);
      if (pI830->CursorIsARGB)
         temp |= CURSOR_FORMAT_ARGB | CURSOR_GAMMA_ENABLE;
      else 
         temp |= CURSOR_FORMAT_3C;
      /* This initialises the format and leave the cursor disabled. */
      OUTREG(CURSOR_CONTROL, temp);
      /* Need to set address and size after disabling. */
      if (pI830->CursorIsARGB)
         OUTREG(CURSOR_BASEADDR, pI830->CursorMemARGB->Start);
      else
         OUTREG(CURSOR_BASEADDR, pI830->CursorMem->Start);
      temp = ((I810_CURSOR_X & CURSOR_SIZE_MASK) << CURSOR_SIZE_HSHIFT) |
	     ((I810_CURSOR_Y & CURSOR_SIZE_MASK) << CURSOR_SIZE_VSHIFT);
      OUTREG(CURSOR_SIZE, temp);
   }
}

Bool
I830CursorInit(ScreenPtr pScreen)
{
   ScrnInfoPtr pScrn;
   I830Ptr pI830;
   xf86CursorInfoPtr infoPtr;

   DPRINTF(PFX, "I830CursorInit\n");
   pScrn = xf86Screens[pScreen->myNum];
   pI830 = I830PTR(pScrn);
   pI830->CursorInfoRec = infoPtr = xf86CreateCursorInfoRec();
   if (!infoPtr)
      return FALSE;

   infoPtr->MaxWidth = I810_CURSOR_X;
   infoPtr->MaxHeight = I810_CURSOR_Y;
   infoPtr->Flags = (HARDWARE_CURSOR_TRUECOLOR_AT_8BPP |
		     HARDWARE_CURSOR_BIT_ORDER_MSBFIRST |
		     HARDWARE_CURSOR_INVERT_MASK |
		     HARDWARE_CURSOR_SWAP_SOURCE_AND_MASK |
		     HARDWARE_CURSOR_AND_SOURCE_WITH_MASK |
		     HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_64 | 0);

   infoPtr->SetCursorColors = I830SetCursorColors;
   infoPtr->SetCursorPosition = I830SetCursorPosition;
   infoPtr->LoadCursorImage = I830LoadCursorImage;
   infoPtr->HideCursor = I830HideCursor;
   infoPtr->ShowCursor = I830ShowCursor;
   infoPtr->UseHWCursor = I830UseHWCursor;

   pI830->pCurs = NULL;

#ifdef ARGB_CURSOR
   pI830->CursorIsARGB = FALSE;

   if (pI830->CursorMemARGB->Start) {
      /* Use ARGB if we were able to allocate the 16kb needed */
      infoPtr->UseHWCursorARGB = I830UseHWCursorARGB;
      infoPtr->LoadCursorARGB = I830LoadCursorARGB;
   }
#endif

   if (pI830->CursorNeedsPhysical && !pI830->CursorMem->Physical) 
      return FALSE;

   I830HideCursor(pScrn);

   return xf86InitCursor(pScreen, infoPtr);
}

static Bool
I830UseHWCursor(ScreenPtr pScreen, CursorPtr pCurs)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);

   pI830->pCurs = pCurs;

   DPRINTF(PFX, "I830UseHWCursor\n");
   if (pI830->CursorNeedsPhysical && !pI830->CursorMem->Physical) 
      return FALSE;

   return TRUE;
}

static void
I830LoadCursorImage(ScrnInfoPtr pScrn, unsigned char *src)
{
   I830Ptr pI830 = I830PTR(pScrn);
   CARD8 *pcurs = (CARD8 *) (pI830->FbBase + pI830->CursorMem->Start);
   int x, y;

   DPRINTF(PFX, "I830LoadCursorImage\n");

#ifdef ARGB_CURSOR
   pI830->CursorIsARGB = FALSE;
#endif
 
   memset(pcurs, 0, 64 * 64 / 4);

#define GetBit(image, x, y)\
    ((int)((*(image + ((x) / 8) + ((y) * (128/8))) &\
	    (1 << ( 7 -((x) % 8) ))) ? 1 : 0))

#define SetBit(image, x, y)\
    (*(image + (x) / 8 + (y) * (128/8)) |=\
     (int) (1 <<  (7-((x) % 8))))

   switch (pI830->rotation) {
      case RR_Rotate_90:
         for (y = 0; y < 64; y++) {
            for (x = 0; x < 64; x++) {
               if (GetBit(src, 64 - y - 1, x))
                  SetBit(pcurs, x, y);
               if (GetBit(src, 128 - y - 1, x))
                  SetBit(pcurs, x + 64, y);
            }
         }

         return;
      case RR_Rotate_180:
         for (y = 0; y < 64; y++) {
            for (x = 0; x < 64; x++) {
               if (GetBit(src, 64 - x - 1, 64 - y - 1))
                  SetBit(pcurs, x, y);
               if (GetBit(src, 128 - x - 1, 64 - y - 1))
                  SetBit(pcurs, x + 64, y);
            }
         }

         return;
      case RR_Rotate_270:
         for (y = 0; y < 64; y++) {
            for (x = 0; x < 64; x++) {
               if (GetBit(src, y, 64 - x - 1))
                  SetBit(pcurs, x, y);
               if (GetBit(src, y + 64, 64 - x - 1))
                  SetBit(pcurs, x + 64, y);
            }
         }

         return;
   }

   for (y = 0; y < 64; y++) {
      for (x = 0; x < 64 / 4; x++) {
	 *pcurs++ = *src++;
      }
   }
}

#ifdef ARGB_CURSOR
#include "cursorstr.h"

static Bool I830UseHWCursorARGB (ScreenPtr pScreen, CursorPtr pCurs)
{
   ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
   I830Ptr pI830 = I830PTR(pScrn);

   DPRINTF(PFX, "I830UseHWCursorARGB\n");

   pI830->pCurs = pCurs;

   if (pScrn->bitsPerPixel == 8)
      return FALSE;

   if (pI830->CursorNeedsPhysical && !pI830->CursorMemARGB->Physical) 
      return FALSE;

   if (pCurs->bits->height <= 64 && pCurs->bits->width <= 64) 
	return TRUE;

   return FALSE;
}

static void I830LoadCursorARGB (ScrnInfoPtr pScrn, CursorPtr pCurs)
{
   I830Ptr pI830 = I830PTR(pScrn);
   CARD32 *dst = (CARD32 *) (pI830->FbBase + pI830->CursorMemARGB->Start);
   CARD32 *image = (CARD32 *)pCurs->bits->argb;
   int x, y, w, h;

   DPRINTF(PFX, "I830LoadCursorARGB\n");

   if (!image)
	return;	/* XXX can't happen */
    
   pI830->CursorIsARGB = TRUE;

   w = pCurs->bits->width;
   h = pCurs->bits->height;

   switch (pI830->rotation) {
      case RR_Rotate_90:
         for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++)
               dst[(y) + ((64 - x - 1) * 64)] = *image++;
            for(; x < 64; x++)
               dst[(y) + ((64 - x - 1) * 64)] = 0;
         }
         for(; y < 64; y++) {
   	    for(x = 0; x < 64; x++)
               dst[(y) + ((64 - x - 1) * 64)] = 0;
         }
         return;

      case RR_Rotate_180:
         for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++)
               dst[(64 - x - 1) + ((64 - y - 1) * 64)] = *image++;
            for(; x < 64; x++)
               dst[(64 - x - 1) + ((64 - y - 1) * 64)] = 0;
         }
         for(; y < 64; y++) {
            for(x = 0; x < 64; x++)
               dst[(64 - x - 1) + ((64 - y - 1) * 64)] = 0;
         }
         return;

      case RR_Rotate_270:
         for (y = 0; y < h; y++) {
            for (x = 0; x < w; x++)
               dst[(64 - y - 1) + (x * 64)] = *image++;
            for(; x < 64; x++)
               dst[(64 - y - 1) + (x * 64)] = 0;
         }
         for(; y < 64; y++) {
            for(x = 0; x < 64; x++)
               dst[(64 - y - 1) + (x * 64)] = 0;
         }
         return;
   }

   for(y = 0; y < h; y++) {
      for(x = 0; x < w; x++)
          *dst++ = *image++;
      for(; x < 64; x++)
          *dst++ = 0;
   }

   for(; y < 64; y++) {
      for(x = 0; x < 64; x++)
          *dst++ = 0;
   }
}
#endif

#define CDMPTR    ((I830ModePrivatePtr)pI830->currentMode->Private)->merged

static void
I830SetCursorPositionMerged(ScrnInfoPtr pScrn, int x, int y)
{
   I830Ptr pI830 = I830PTR(pScrn);
   ScrnInfoPtr    pScrn2 = pI830->pScrn_2;
   DisplayModePtr mode1 = CDMPTR.First;
   Bool hide = FALSE, show = FALSE;
   DisplayModePtr mode2 = CDMPTR.Second;
   int x1, y1, x2, y2;
   int total_y1 = pScrn->frameY1 - pScrn->frameY0;
   int total_y2 = pScrn2->frameY1 - pScrn2->frameY0;
   CARD32 temp = 0, temp2 = 0;

   x += pScrn->frameX0;
   y += pScrn->frameY0;

   x1 = x - pI830->FirstframeX0;
   y1 = y - pI830->FirstframeY0;

   x2 = x - pScrn2->frameX0;
   y2 = y - pScrn2->frameY0;

   if (y1 > total_y1)
      y1 = total_y1;
   if (y2 > total_y2)                  
      y2 = total_y2;

   /* move cursor offscreen */
   if (y1 >= 0 && y2 >= mode2->VDisplay) {
      y2 = -I810_CURSOR_Y;  
   } else if (y2 >= 0 && y1 >= mode1->VDisplay) {
      y1 = -I810_CURSOR_Y;  
   }
   if (x1 >= 0 && x2 >= mode2->HDisplay) {
      x2 = -I810_CURSOR_X;  
   } else if (x2 >= 0 && x1 >= mode1->HDisplay) {
      x1 = -I810_CURSOR_X;  
   }

   /* Clamp the cursor position to the visible screen area */
   if (x1 >= mode1->HDisplay) x1 = mode1->HDisplay - 1;
   if (y1 >= mode1->VDisplay) y1 = mode1->VDisplay - 1;
   if (x1 <= -I810_CURSOR_X) x1 = -I810_CURSOR_X + 1;
   if (y1 <= -I810_CURSOR_Y) y1 = -I810_CURSOR_Y + 1;
   if (x2 >= mode2->HDisplay) x2 = mode2->HDisplay - 1;
   if (y2 >= mode2->VDisplay) y2 = mode2->VDisplay - 1;
   if (x2 <= -I810_CURSOR_X) x2 = -I810_CURSOR_X + 1;
   if (y2 <= -I810_CURSOR_Y) y2 = -I810_CURSOR_Y + 1;

   if (x1 < 0) {
      temp |= (CURSOR_POS_SIGN << CURSOR_X_SHIFT);
      x1 = -x1;
   }
   if (y1 < 0) {
      temp |= (CURSOR_POS_SIGN << CURSOR_Y_SHIFT);
      y1 = -y1;
   }
   if (x2 < 0) {
      temp2 |= (CURSOR_POS_SIGN << CURSOR_X_SHIFT);
      x2 = -x2;
   }
   if (y2 < 0) {
      temp2 |= (CURSOR_POS_SIGN << CURSOR_Y_SHIFT);
      y2 = -y2;
   }

   temp |= ((x1 & CURSOR_POS_MASK) << CURSOR_X_SHIFT);
   temp |= ((y1 & CURSOR_POS_MASK) << CURSOR_Y_SHIFT);
   temp2 |= ((x2 & CURSOR_POS_MASK) << CURSOR_X_SHIFT);
   temp2 |= ((y2 & CURSOR_POS_MASK) << CURSOR_Y_SHIFT);

   OUTREG(CURSOR_A_POSITION, temp);
   OUTREG(CURSOR_B_POSITION, temp2);

   if (pI830->cursorOn) {
      if (hide)
	 pI830->CursorInfoRec->HideCursor(pScrn);
      else if (show)
	 pI830->CursorInfoRec->ShowCursor(pScrn);
      pI830->cursorOn = TRUE;
   }

   /* have to upload the base for the new position */
   if (IS_I9XX(pI830)) {
      if (pI830->CursorIsARGB) {
         OUTREG(CURSOR_A_BASE, pI830->CursorMemARGB->Physical);
         OUTREG(CURSOR_B_BASE, pI830->CursorMemARGB->Physical);
      } else {
         OUTREG(CURSOR_A_BASE, pI830->CursorMem->Physical);
         OUTREG(CURSOR_B_BASE, pI830->CursorMem->Physical);
      }
   }
}

static void
I830SetCursorPosition(ScrnInfoPtr pScrn, int x, int y)
{
   I830Ptr pI830 = I830PTR(pScrn);
   CARD32 temp = 0;
   Bool hide = FALSE, show = FALSE;
   int oldx = x, oldy = y;
   int hotspotx = 0, hotspoty = 0;
#if 0
   static Bool outsideViewport = FALSE;
#endif

   if (pI830->MergedFB) {
      I830SetCursorPositionMerged(pScrn, x, y);
      return;
   }

   oldx += pScrn->frameX0; /* undo what xf86HWCurs did */
   oldy += pScrn->frameY0;

   switch (pI830->rotation) {
      case RR_Rotate_0:
         x = oldx;
         y = oldy;
         break;
      case RR_Rotate_90:
         x = oldy;
         y = pScrn->pScreen->width - oldx;
         hotspoty = I810_CURSOR_X;
         break;
      case RR_Rotate_180:
         x = pScrn->pScreen->width - oldx;
         y = pScrn->pScreen->height - oldy;
         hotspotx = I810_CURSOR_X;
         hotspoty = I810_CURSOR_Y;
         break;
      case RR_Rotate_270:
         x = pScrn->pScreen->height - oldy;
         y = oldx;
         hotspotx = I810_CURSOR_Y;
         break;
   }

   x -= hotspotx;
   y -= hotspoty;

   /* Now, readjust */
   x -= pScrn->frameX0;
   y -= pScrn->frameY0;

   /* Clamp the cursor position to the visible screen area */
   if (x >= pScrn->currentMode->HDisplay) x = pScrn->currentMode->HDisplay - 1;
   if (y >= pScrn->currentMode->VDisplay) y = pScrn->currentMode->VDisplay - 1;
   if (x <= -I810_CURSOR_X) x = -I810_CURSOR_X + 1;
   if (y <= -I810_CURSOR_Y) y = -I810_CURSOR_Y + 1;

#if 0
   /*
    * There is a screen display problem when the cursor position is set
    * wholely outside of the viewport.  We trap that here, turning the
    * cursor off when that happens, and back on when it comes back into
    * the viewport.
    */
   if (x >= pScrn->currentMode->HDisplay ||
       y >= pScrn->currentMode->VDisplay ||
       x <= -I810_CURSOR_X || y <= -I810_CURSOR_Y) {
      hide = TRUE;
      outsideViewport = TRUE;
   } else if (outsideViewport) {
      show = TRUE;
      outsideViewport = FALSE;
   }
#endif

   if (x < 0) {
      temp |= (CURSOR_POS_SIGN << CURSOR_X_SHIFT);
      x = -x;
   }
   if (y < 0) {
      temp |= (CURSOR_POS_SIGN << CURSOR_Y_SHIFT);
      y = -y;
   }
   temp |= ((x & CURSOR_POS_MASK) << CURSOR_X_SHIFT);
   temp |= ((y & CURSOR_POS_MASK) << CURSOR_Y_SHIFT);

   OUTREG(CURSOR_A_POSITION, temp);
   if (pI830->Clone)
      OUTREG(CURSOR_B_POSITION, temp);

   if (pI830->cursorOn) {
      if (hide)
	 pI830->CursorInfoRec->HideCursor(pScrn);
      else if (show)
	 pI830->CursorInfoRec->ShowCursor(pScrn);
      pI830->cursorOn = TRUE;
   }

   /* have to upload the base for the new position */
   if (IS_I9XX(pI830)) {
      if (pI830->CursorNeedsPhysical) {
         if (pI830->CursorIsARGB)
            OUTREG(CURSOR_A_BASE, pI830->CursorMemARGB->Physical);
         else
            OUTREG(CURSOR_A_BASE, pI830->CursorMem->Physical);
      } else {
         if (pI830->CursorIsARGB)
            OUTREG(CURSOR_A_BASE, pI830->CursorMemARGB->Start);
         else
            OUTREG(CURSOR_A_BASE, pI830->CursorMem->Start);
      }
      if (pI830->Clone) {
         if (pI830->CursorNeedsPhysical) {
            if (pI830->CursorIsARGB)
               OUTREG(CURSOR_B_BASE, pI830->CursorMemARGB->Physical);
            else
               OUTREG(CURSOR_B_BASE, pI830->CursorMem->Physical);
	 } else {
            if (pI830->CursorIsARGB)
               OUTREG(CURSOR_B_BASE, pI830->CursorMemARGB->Start);
            else
               OUTREG(CURSOR_B_BASE, pI830->CursorMem->Start);
	 }
      }
   }
}

static void
I830ShowCursor(ScrnInfoPtr pScrn)
{
   I830Ptr pI830 = I830PTR(pScrn);
   CARD32 temp;

   DPRINTF(PFX, "I830ShowCursor\n");
   DPRINTF(PFX,
	   "Value of CursorMem->Physical is %x, "
	   " Value of CursorMem->Start is %x ",
	   pI830->CursorMem->Physical, pI830->CursorMem->Start);
   DPRINTF(PFX,
	   "Value of CursorMemARGB->Physical is %x, "
	   " Value of CursorMemARGB->Start is %x ",
	   pI830->CursorMemARGB->Physical, pI830->CursorMemARGB->Start);

   pI830->cursorOn = TRUE;
   if (IS_MOBILE(pI830) || IS_I9XX(pI830)) {
      temp = INREG(CURSOR_A_CONTROL);
      temp &= ~(CURSOR_MODE | MCURSOR_PIPE_SELECT | MCURSOR_GAMMA_ENABLE);
      if (pI830->CursorIsARGB)
         temp |= CURSOR_MODE_64_ARGB_AX | MCURSOR_GAMMA_ENABLE;
      else
         temp |= CURSOR_MODE_64_4C_AX;
      temp |= (pI830->pipe << 28); /* Connect to correct pipe */
      /* Need to set mode, then address. */
      OUTREG(CURSOR_A_CONTROL, temp);
      if (pI830->CursorNeedsPhysical) {
         if (pI830->CursorIsARGB)
            OUTREG(CURSOR_A_BASE, pI830->CursorMemARGB->Physical);
         else
            OUTREG(CURSOR_A_BASE, pI830->CursorMem->Physical);
      } else {
         if (pI830->CursorIsARGB)
            OUTREG(CURSOR_A_BASE, pI830->CursorMemARGB->Start);
         else
            OUTREG(CURSOR_A_BASE, pI830->CursorMem->Start);
      }
      if (pI830->Clone || pI830->MergedFB) {
         temp &= ~MCURSOR_PIPE_SELECT;
         temp |= (!pI830->pipe << 28);
         OUTREG(CURSOR_B_CONTROL, temp);
         if (pI830->CursorNeedsPhysical) {
            if (pI830->CursorIsARGB)
               OUTREG(CURSOR_B_BASE, pI830->CursorMemARGB->Physical);
            else
               OUTREG(CURSOR_B_BASE, pI830->CursorMem->Physical);
	 } else {
            if (pI830->CursorIsARGB)
               OUTREG(CURSOR_B_BASE, pI830->CursorMemARGB->Start);
            else
               OUTREG(CURSOR_B_BASE, pI830->CursorMem->Start);
	 }
      }
   } else {
      temp = INREG(CURSOR_CONTROL);
      temp &= ~(CURSOR_FORMAT_MASK | CURSOR_GAMMA_ENABLE);
      temp |= CURSOR_ENABLE;
      if (pI830->CursorIsARGB)
         temp |= CURSOR_FORMAT_ARGB | CURSOR_GAMMA_ENABLE;
      else 
         temp |= CURSOR_FORMAT_3C;
      OUTREG(CURSOR_CONTROL, temp);
      if (pI830->CursorIsARGB)
         OUTREG(CURSOR_BASEADDR, pI830->CursorMemARGB->Start);
      else
         OUTREG(CURSOR_BASEADDR, pI830->CursorMem->Start);
   }
}

static void
I830HideCursor(ScrnInfoPtr pScrn)
{
   CARD32 temp;
   I830Ptr pI830 = I830PTR(pScrn);

   DPRINTF(PFX, "I830HideCursor\n");

   pI830->cursorOn = FALSE;
   if (IS_MOBILE(pI830) || IS_I9XX(pI830)) {
      temp = INREG(CURSOR_A_CONTROL);
      temp &= ~CURSOR_MODE;
      temp |= CURSOR_MODE_DISABLE;
      OUTREG(CURSOR_A_CONTROL, temp);
      /* This is needed to flush the above change. */
      if (pI830->CursorIsARGB)
         OUTREG(CURSOR_A_BASE, pI830->CursorMemARGB->Physical);
      else
         OUTREG(CURSOR_A_BASE, pI830->CursorMem->Physical);
      if (pI830->Clone || pI830->MergedFB) {
         OUTREG(CURSOR_B_CONTROL, temp);
         if (pI830->CursorIsARGB)
            OUTREG(CURSOR_B_BASE, pI830->CursorMemARGB->Physical);
         else
            OUTREG(CURSOR_B_BASE, pI830->CursorMem->Physical);
      }
   } else {
      temp = INREG(CURSOR_CONTROL);
      temp &= ~CURSOR_ENABLE;
      OUTREG(CURSOR_CONTROL, temp);
   }
}

static void
I830SetCursorColors(ScrnInfoPtr pScrn, int bg, int fg)
{
   I830Ptr pI830 = I830PTR(pScrn);

#ifdef ARGB_CURSOR
    /* Don't recolour cursors set with SetCursorARGB. */
    if (pI830->CursorIsARGB)
       return;
#endif

   DPRINTF(PFX, "I830SetCursorColors\n");

   OUTREG(CURSOR_A_PALETTE0, bg & 0x00ffffff);
   OUTREG(CURSOR_A_PALETTE1, fg & 0x00ffffff);
   OUTREG(CURSOR_A_PALETTE2, fg & 0x00ffffff);
   OUTREG(CURSOR_A_PALETTE3, bg & 0x00ffffff);
   if (pI830->Clone || pI830->MergedFB) {
      OUTREG(CURSOR_B_PALETTE0, bg & 0x00ffffff);
      OUTREG(CURSOR_B_PALETTE1, fg & 0x00ffffff);
      OUTREG(CURSOR_B_PALETTE2, fg & 0x00ffffff);
      OUTREG(CURSOR_B_PALETTE3, bg & 0x00ffffff);
   }
}
