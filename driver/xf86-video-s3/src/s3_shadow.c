/*
Copyright (C) 1994-2000 The XFree86 Project, Inc.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the XFree86 Project shall not
be used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the XFree86 Project.
*/

/*
   Copyright (c) 1999,2000  The XFree86 Project Inc. 
   based on code written by Mark Vojkovich <markv@valinux.com>
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86PciInfo.h"
#include "xf86Pci.h"
#include "shadowfb.h"
#include "servermd.h"
#include "s3.h"


void
S3RefreshArea(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    S3Ptr pS3 = S3PTR(pScrn);
    int width, height, Bpp, FBPitch;
    unsigned char *src, *dst;
   
    Bpp = pScrn->bitsPerPixel >> 3;
    FBPitch = BitmapBytePad(pScrn->displayWidth * pScrn->bitsPerPixel);

    while(num--) {
	width = (pbox->x2 - pbox->x1) * Bpp;
	height = pbox->y2 - pbox->y1;
	src = pS3->ShadowPtr + (pbox->y1 * pS3->ShadowPitch) + 
		(pbox->x1 * Bpp);
	dst = pS3->FBBase + (pbox->y1 * FBPitch) + (pbox->x1 * Bpp);

	while(height--) {
	    memcpy(dst, src, width);
	    dst += FBPitch;
	    src += pS3->ShadowPitch;
	}
	
	pbox++;
    }
} 

void
S3PointerMoved(SCRN_ARG_TYPE arg, int x, int y)
{
    SCRN_INFO_PTR(arg);
    S3Ptr pS3 = S3PTR(pScrn);
    int newX, newY;

    if(pS3->rotate == 1) {
	newX = pScrn->pScreen->height - y - 1;
	newY = x;
    } else {
	newX = y;
	newY = pScrn->pScreen->width - x - 1;
    }

    (*pS3->PointerMoved)(arg, newX, newY);
}

void
S3RefreshArea8(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    S3Ptr pS3 = S3PTR(pScrn);
    int count, width, height, y1, y2, dstPitch, srcPitch;
    CARD8 *dstPtr, *srcPtr, *src;
    CARD32 *dst;

    dstPitch = pScrn->displayWidth;
    srcPitch = -pS3->rotate * pS3->ShadowPitch;

    while(num--) {
	width = pbox->x2 - pbox->x1;
	y1 = pbox->y1 & ~3;
	y2 = (pbox->y2 + 3) & ~3;
	height = (y2 - y1) >> 2;  /* in dwords */

	if(pS3->rotate == 1) {
	    dstPtr = pS3->FBBase + 
			(pbox->x1 * dstPitch) + pScrn->virtualX - y2;
	    srcPtr = pS3->ShadowPtr + ((1 - y2) * srcPitch) + pbox->x1;
	} else {
	    dstPtr = pS3->FBBase + 
			((pScrn->virtualY - pbox->x2) * dstPitch) + y1;
	    srcPtr = pS3->ShadowPtr + (y1 * srcPitch) + pbox->x2 - 1;
	}

	while(width--) {
	    src = srcPtr;
	    dst = (CARD32*)dstPtr;
	    count = height;
	    while(count--) {
		*(dst++) = src[0] | (src[srcPitch] << 8) | 
					(src[srcPitch * 2] << 16) | 
					(src[srcPitch * 3] << 24);
		src += srcPitch * 4;
	    }
	    srcPtr += pS3->rotate;
	    dstPtr += dstPitch;
	}

	pbox++;
    }
} 


void
S3RefreshArea16(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    S3Ptr pS3 = S3PTR(pScrn);
    int count, width, height, y1, y2, dstPitch, srcPitch;
    CARD16 *dstPtr, *srcPtr, *src;
    CARD32 *dst;

    dstPitch = pScrn->displayWidth;
    srcPitch = -pS3->rotate * pS3->ShadowPitch >> 1;

    while(num--) {
	width = pbox->x2 - pbox->x1;
	y1 = pbox->y1 & ~1;
	y2 = (pbox->y2 + 1) & ~1;
	height = (y2 - y1) >> 1;  /* in dwords */

	if(pS3->rotate == 1) {
	    dstPtr = (CARD16*)pS3->FBBase + 
			(pbox->x1 * dstPitch) + pScrn->virtualX - y2;
	    srcPtr = (CARD16*)pS3->ShadowPtr + 
			((1 - y2) * srcPitch) + pbox->x1;
	} else {
	    dstPtr = (CARD16*)pS3->FBBase + 
			((pScrn->virtualY - pbox->x2) * dstPitch) + y1;
	    srcPtr = (CARD16*)pS3->ShadowPtr + 
			(y1 * srcPitch) + pbox->x2 - 1;
	}

	while(width--) {
	    src = srcPtr;
	    dst = (CARD32*)dstPtr;
	    count = height;
	    while(count--) {
		*(dst++) = src[0] | (src[srcPitch] << 16);
		src += srcPitch * 2;
	    }
	    srcPtr += pS3->rotate;
	    dstPtr += dstPitch;
	}

	pbox++;
    }
}


/* this one could be faster */
void
S3RefreshArea24(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    S3Ptr pS3 = S3PTR(pScrn);
    int count, width, height, y1, y2, dstPitch, srcPitch;
    CARD8 *dstPtr, *srcPtr, *src;
    CARD32 *dst;

    dstPitch = BitmapBytePad(pScrn->displayWidth * 24);
    srcPitch = -pS3->rotate * pS3->ShadowPitch;

    while(num--) {
        width = pbox->x2 - pbox->x1;
        y1 = pbox->y1 & ~3;
        y2 = (pbox->y2 + 3) & ~3;
        height = (y2 - y1) >> 2;  /* blocks of 3 dwords */

	if(pS3->rotate == 1) {
	    dstPtr = pS3->FBBase + 
			(pbox->x1 * dstPitch) + ((pScrn->virtualX - y2) * 3);
	    srcPtr = pS3->ShadowPtr + ((1 - y2) * srcPitch) + (pbox->x1 * 3);
	} else {
	    dstPtr = pS3->FBBase + 
			((pScrn->virtualY - pbox->x2) * dstPitch) + (y1 * 3);
	    srcPtr = pS3->ShadowPtr + (y1 * srcPitch) + (pbox->x2 * 3) - 3;
	}

	while(width--) {
	    src = srcPtr;
	    dst = (CARD32*)dstPtr;
	    count = height;
	    while(count--) {
		dst[0] = src[0] | (src[1] << 8) | (src[2] << 16) |
				(src[srcPitch] << 24);		
		dst[1] = src[srcPitch + 1] | (src[srcPitch + 2] << 8) |
				(src[srcPitch * 2] << 16) |
				(src[(srcPitch * 2) + 1] << 24);		
		dst[2] = src[(srcPitch * 2) + 2] | (src[srcPitch * 3] << 8) |
				(src[(srcPitch * 3) + 1] << 16) |
				(src[(srcPitch * 3) + 2] << 24);	
		dst += 3;
		src += srcPitch * 4;
	    }
	    srcPtr += pS3->rotate * 3;
	    dstPtr += dstPitch; 
	}

	pbox++;
    }
}

void
S3RefreshArea32(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    S3Ptr pS3 = S3PTR(pScrn);
    int count, width, height, dstPitch, srcPitch;
    CARD32 *dstPtr, *srcPtr, *src, *dst;

    dstPitch = pScrn->displayWidth;
    srcPitch = -pS3->rotate * pS3->ShadowPitch >> 2;

    while(num--) {
	width = pbox->x2 - pbox->x1;
	height = pbox->y2 - pbox->y1;

	if(pS3->rotate == 1) {
	    dstPtr = (CARD32*)pS3->FBBase + 
			(pbox->x1 * dstPitch) + pScrn->virtualX - pbox->y2;
	    srcPtr = (CARD32*)pS3->ShadowPtr + 
			((1 - pbox->y2) * srcPitch) + pbox->x1;
	} else {
	    dstPtr = (CARD32*)pS3->FBBase + 
			((pScrn->virtualY - pbox->x2) * dstPitch) + pbox->y1;
	    srcPtr = (CARD32*)pS3->ShadowPtr + 
			(pbox->y1 * srcPitch) + pbox->x2 - 1;
	}

	while(width--) {
	    src = srcPtr;
	    dst = dstPtr;
	    count = height;
	    while(count--) {
		*(dst++) = *src;
		src += srcPitch;
	    }
	    srcPtr += pS3->rotate;
	    dstPtr += dstPitch;
	}

	pbox++;
    }
}
