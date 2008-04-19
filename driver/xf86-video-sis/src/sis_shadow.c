/*
 * Copyright (C) 1999-2004 by The XFree86 Project, Inc.
 * based on code written by Mark Vojkovich
 * Copyright (C) 2003-2005 Thomas Winischhofer
 *
 * Licensed under the following terms:
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appears in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * and that the name of the copyright holder not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. The copyright holder makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without expressed or implied warranty.
 *
 * THE COPYRIGHT HOLDER DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
 * EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 * This module doesn't use CurrentLayout, because it is never
 * active when DGA is active and vice versa.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sis.h"
#include "servermd.h"

void SISPointerMoved(int index, int x, int y);
void SISPointerMovedReflect(int index, int x, int y);
void SISRefreshArea(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void SISRefreshAreaReflect(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void SISRefreshArea8(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void SISRefreshArea16(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void SISRefreshArea24(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void SISRefreshArea32(ScrnInfoPtr pScrn, int num, BoxPtr pbox);

void
SISPointerMoved(int index, int x, int y)
{
    ScrnInfoPtr pScrn = xf86Screens[index];
    SISPtr pSiS = SISPTR(pScrn);

    if(pSiS->Rotate == 1) {
       (*pSiS->PointerMoved)(index, pScrn->pScreen->height - y - 1, x);
    } else {
       (*pSiS->PointerMoved)(index, y, pScrn->pScreen->width - x - 1);
    }
}

void
SISPointerMovedReflect(int index, int x, int y)
{
    ScrnInfoPtr pScrn = xf86Screens[index];
    SISPtr pSiS = SISPTR(pScrn);

    switch(pSiS->Reflect) {
    case 1: /* x */
       (*pSiS->PointerMoved)(index, pScrn->pScreen->width - x - 1, y);
       break;
    case 2: /* y */
       (*pSiS->PointerMoved)(index, x, pScrn->pScreen->height - y - 1);
       break;
    case 3: /* x + y */
       (*pSiS->PointerMoved)(index, pScrn->pScreen->width - x - 1, pScrn->pScreen->height - y - 1);
    }
}

/* Refresh area (unreflected, unrotated) */

void
SISRefreshArea(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    SISPtr pSiS = SISPTR(pScrn);
    int    width, height, Bpp, FBPitch;
    CARD8  *src, *dst;

    Bpp = pScrn->bitsPerPixel >> 3;
    FBPitch = BitmapBytePad(pScrn->displayWidth * pScrn->bitsPerPixel);

    while(num--) {

       width = (pbox->x2 - pbox->x1) * Bpp;
       height = pbox->y2 - pbox->y1;
       src = pSiS->ShadowPtr + (pbox->y1 * pSiS->ShadowPitch) +  (pbox->x1 * Bpp);
       dst = pSiS->FbBase + (pbox->y1 * FBPitch) + (pbox->x1 * Bpp);

       while(height--) {
          SiSMemCopyToVideoRam(pSiS, dst, src, width);
	  dst += FBPitch;
	  src += pSiS->ShadowPitch;
       }

       pbox++;
    }
}

/* RefreshArea for reflection */

void
SISRefreshAreaReflect(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    SISPtr pSiS = SISPTR(pScrn);
    int    width, height, Bpp, FBPitch, twidth;
    CARD8  *src, *dst, *tdst, *tsrc;
    CARD16 *tdst16, *tsrc16;
    CARD32 *tdst32, *tsrc32;

    Bpp = pScrn->bitsPerPixel >> 3;
    FBPitch = BitmapBytePad(pScrn->displayWidth * pScrn->bitsPerPixel);

    while(num--) {
       width = (pbox->x2 - pbox->x1) * Bpp;
       height = pbox->y2 - pbox->y1;
       src = pSiS->ShadowPtr + (pbox->y1 * pSiS->ShadowPitch) +  (pbox->x1 * Bpp);
       dst = pSiS->FbBase;
       switch(pSiS->Reflect) {
       case 1:	/* x */
	  dst += (pbox->y1 * FBPitch) + ((pScrn->displayWidth - pbox->x1 - 1) * Bpp);
	  switch(Bpp) {
	     case 1:
		while(height--) {
		   tdst = dst;
		   tsrc = src;
		   twidth = width;
		   while(twidth--) *tdst-- = *tsrc++;
		   dst += FBPitch;
		   src += pSiS->ShadowPitch;
		}
		break;
	     case 2:
		width >>= 1;
		while(height--) {
		   tdst16 = (CARD16 *)dst;
		   tsrc16 = (CARD16 *)src;
		   twidth = width;
		   while(twidth--) *tdst16-- = *tsrc16++;
		   dst += FBPitch;
		   src += pSiS->ShadowPitch;
		}
		break;
	     case 4:
		width >>= 2;
		while(height--) {
		   tdst32 = (CARD32 *)dst;
		   tsrc32 = (CARD32 *)src;
		   twidth = width;
		   while(twidth--) *tdst32-- = *tsrc32++;
		   dst += FBPitch;
		   src += pSiS->ShadowPitch;
		}
	  }
	  break;
       case 2:	/* y */
	  dst += ((pScrn->virtualY - pbox->y1 - 1) * FBPitch) + (pbox->x1 * Bpp);
	  while(height--) {
	     SiSMemCopyToVideoRam(pSiS, dst, src, width);
	     dst -= FBPitch;
	     src += pSiS->ShadowPitch;
	  }
	  break;
       case 3:	/* x + y */
	  dst += ((pScrn->virtualY - pbox->y1 - 1) * FBPitch) + ((pScrn->displayWidth - pbox->x1 - 1) * Bpp);
	  switch(Bpp) {
	     case 1:
		while(height--) {
		   tdst = dst;
		   tsrc = src;
		   twidth = width;
		   while(twidth--) *tdst-- = *tsrc++;
		   dst -= FBPitch;
		   src += pSiS->ShadowPitch;
		}
		break;
	     case 2:
	        width >>= 1;
		while(height--) {
		   tdst16 = (CARD16 *)dst;
		   tsrc16 = (CARD16 *)src;
		   twidth = width;
		   while(twidth--) *tdst16-- = *tsrc16++;
		   dst -= FBPitch;
		   src += pSiS->ShadowPitch;
		}
		break;
	     case 4:
		width >>= 2;
		while(height--) {
		   tdst32 = (CARD32 *)dst;
		   tsrc32 = (CARD32 *)src;
		   twidth = width;
		   while(twidth--) *tdst32-- = *tsrc32++;
		   dst -= FBPitch;
		   src += pSiS->ShadowPitch;
		}
		break;
	  }
       }
       pbox++;
    }
}

/* RefreshArea()s for rotation */

void
SISRefreshArea8(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    SISPtr pSiS = SISPTR(pScrn);
    int    count, width, height, y1, y2, dstPitch, srcPitch;
    CARD8  *dstPtr, *srcPtr, *src;
    CARD32 *dst;

    dstPitch = pScrn->displayWidth;
    srcPitch = -pSiS->Rotate * pSiS->ShadowPitch;

    while(num--) {
       width = pbox->x2 - pbox->x1;
       y1 = pbox->y1 & ~3;
       y2 = (pbox->y2 + 3) & ~3;
       height = (y2 - y1) >> 2;  /* in dwords */

       if(pSiS->Rotate == 1) {
	  dstPtr = pSiS->FbBase + (pbox->x1 * dstPitch) + pScrn->virtualX - y2;
	  srcPtr = pSiS->ShadowPtr + ((1 - y2) * srcPitch) + pbox->x1;
       } else {
	  dstPtr = pSiS->FbBase +  ((pScrn->virtualY - pbox->x2) * dstPitch) + y1;
	  srcPtr = pSiS->ShadowPtr + (y1 * srcPitch) + pbox->x2 - 1;
       }

       while(width--) {
	  src = srcPtr;
	  dst = (CARD32 *)dstPtr;
	  count = height;
	  while(count--) {
	     *(dst++) = src[0]                    |
		        (src[srcPitch]     <<  8) |
		        (src[srcPitch * 2] << 16) |
			(src[srcPitch * 3] << 24);
	     src += (srcPitch * 4);
	  }
	  srcPtr += pSiS->Rotate;
	  dstPtr += dstPitch;
       }

       pbox++;
    }
}

void
SISRefreshArea16(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    SISPtr pSiS = SISPTR(pScrn);
    int count, width, height, y1, y2, dstPitch, srcPitch;
    CARD16 *dstPtr, *srcPtr, *src;
    CARD32 *dst;

    dstPitch = pScrn->displayWidth;
    srcPitch = -pSiS->Rotate * pSiS->ShadowPitch >> 1;

    while(num--) {
       width = pbox->x2 - pbox->x1;
       y1 = pbox->y1 & ~1;
       y2 = (pbox->y2 + 1) & ~1;
       height = (y2 - y1) >> 1;  /* in dwords */

       if(pSiS->Rotate == 1) {
	  dstPtr = (CARD16 *)pSiS->FbBase + (pbox->x1 * dstPitch) + pScrn->virtualX - y2;
	  srcPtr = (CARD16 *)pSiS->ShadowPtr + ((1 - y2) * srcPitch) + pbox->x1;
       } else {
	  dstPtr = (CARD16 *)pSiS->FbBase + ((pScrn->virtualY - pbox->x2) * dstPitch) + y1;
	  srcPtr = (CARD16 *)pSiS->ShadowPtr + (y1 * srcPitch) + pbox->x2 - 1;
       }

       while(width--) {
	  src = srcPtr;
	  dst = (CARD32 *)dstPtr;
	  count = height;
	  while(count--) {
	     *(dst++) = src[0] | (src[srcPitch] << 16);
	     src += (srcPitch * 2);
	  }
	  srcPtr += pSiS->Rotate;
	  dstPtr += dstPitch;
       }

       pbox++;
    }
}

/* this one could be faster */
void
SISRefreshArea24(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    SISPtr pSiS = SISPTR(pScrn);
    int    count, width, height, y1, y2, dstPitch, srcPitch;
    CARD8  *dstPtr, *srcPtr, *src;
    CARD32 *dst;

    dstPitch = BitmapBytePad(pScrn->displayWidth * 24);
    srcPitch = -pSiS->Rotate * pSiS->ShadowPitch;

    while(num--) {
       width = pbox->x2 - pbox->x1;
       y1 = pbox->y1 & ~3;
       y2 = (pbox->y2 + 3) & ~3;
       height = (y2 - y1) >> 2;  /* blocks of 3 dwords */

       if(pSiS->Rotate == 1) {
	  dstPtr = pSiS->FbBase + (pbox->x1 * dstPitch) + ((pScrn->virtualX - y2) * 3);
	  srcPtr = pSiS->ShadowPtr + ((1 - y2) * srcPitch) + (pbox->x1 * 3);
       } else {
	  dstPtr = pSiS->FbBase + ((pScrn->virtualY - pbox->x2) * dstPitch) + (y1 * 3);
	  srcPtr = pSiS->ShadowPtr + (y1 * srcPitch) + (pbox->x2 * 3) - 3;
       }

       while(width--) {
	  src = srcPtr;
	  dst = (CARD32 *)dstPtr;
	  count = height;
	  while(count--) {
	     dst[0] = src[0]         |
		      (src[1] << 8)  |
		      (src[2] << 16) |
		      (src[srcPitch] << 24);
	     dst[1] = src[srcPitch + 1]         |
		      (src[srcPitch + 2] << 8)  |
		      (src[srcPitch * 2] << 16) |
		      (src[(srcPitch * 2) + 1] << 24);
	     dst[2] = src[(srcPitch * 2) + 2]         |
		      (src[srcPitch * 3] << 8)        |
		      (src[(srcPitch * 3) + 1] << 16) |
		      (src[(srcPitch * 3) + 2] << 24);
	     dst += 3;
	     src += (srcPitch << 2);
	  }
	  srcPtr += pSiS->Rotate * 3;
	  dstPtr += dstPitch;
       }

       pbox++;
    }
}

void
SISRefreshArea32(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    SISPtr pSiS = SISPTR(pScrn);
    int    count, width, height, dstPitch, srcPitch;
    CARD32 *dstPtr, *srcPtr, *src, *dst;

    dstPitch = pScrn->displayWidth;
    srcPitch = -pSiS->Rotate * pSiS->ShadowPitch >> 2;

    while(num--) {
       width = pbox->x2 - pbox->x1;
       height = pbox->y2 - pbox->y1;

       if(pSiS->Rotate == 1) {
	  dstPtr = (CARD32 *)pSiS->FbBase + (pbox->x1 * dstPitch) + pScrn->virtualX - pbox->y2;
	  srcPtr = (CARD32 *)pSiS->ShadowPtr + ((1 - pbox->y2) * srcPitch) + pbox->x1;
       } else {
	  dstPtr = (CARD32 *)pSiS->FbBase + ((pScrn->virtualY - pbox->x2) * dstPitch) + pbox->y1;
	  srcPtr = (CARD32 *)pSiS->ShadowPtr + (pbox->y1 * srcPitch) + pbox->x2 - 1;
       }

       while(width--) {
	  src = srcPtr;
	  dst = dstPtr;
	  count = height;
	  while(count--) {
	     *(dst++) = *src;
	     src += srcPitch;
	  }
	  srcPtr += pSiS->Rotate;
	  dstPtr += dstPitch;
       }

       pbox++;
    }
}
