/*
 * Copyright 1998-2003 VIA Technologies, Inc. All Rights Reserved.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via.h"
#include "via_driver.h"
#include "shadowfb.h"
#include "servermd.h"

/* RandR support */
#include "xf86RandR12.h"


static void
VIARefreshArea(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    VIAPtr pVia = VIAPTR(pScrn);
    int width, height, Bpp, FBPitch;
    unsigned char *src, *dst;

    Bpp = pScrn->bitsPerPixel >> 3;
    FBPitch = BitmapBytePad(pScrn->displayWidth * pScrn->bitsPerPixel);

    while (num--) {
        width = (pbox->x2 - pbox->x1) * Bpp;
        height = pbox->y2 - pbox->y1;
        src = (pVia->ShadowPtr + (pbox->y1 * pVia->ShadowPitch)
               + (pbox->x1 * Bpp));
        dst = pVia->FBBase + (pbox->y1 * FBPitch) + (pbox->x1 * Bpp);

        while (height--) {
            memcpy(dst, src, width);
            dst += FBPitch;
            src += pVia->ShadowPitch;
        }

        pbox++;
    }
}

void
VIARefreshArea_UD(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    VIAPtr pVia = VIAPTR(pScrn);
    int width, height, Bpp, FBPitch;
    unsigned char *src, *dst;
    int i;
   
    Bpp = pScrn->bitsPerPixel >> 3;
    FBPitch = BitmapBytePad(pScrn->displayWidth * pScrn->bitsPerPixel);

    while (num--) {
        width = pbox->x2 - pbox->x1;
        height = pbox->y2 - pbox->y1;
        dst = pVia->FBBase + ((pScrn->virtualY - 1 - pbox->y1) * FBPitch) + ((pScrn->virtualX - 1 - pbox->x1) * Bpp);
        src = pVia->ShadowPtr + (pbox->y1 * pVia->ShadowPitch) + 
              (pbox->x1 * Bpp);


        switch(pScrn->bitsPerPixel) {
            case 8:
                while (height--) {
                    for(i=0; i<width; i++) {
                         *(dst-i)=*(src+i);
                    }
                    dst -= FBPitch;
                    src += pVia->ShadowPitch;
                }
                break;
            case 16:
                while (height--) {
                    /* TODO Faster method - need test
                    for(i=0; i<width*2; i=i+2) {
                         *(CARD16 *)(dst-i)=*(CARD16 *)(src+i);
                    }
                    */
                    for(i=0; i<width; i++) {
                         *(CARD16 *)(dst-i*2)=*(CARD16 *)(src+i*2);
                    }
                    dst -= FBPitch;
                    src += pVia->ShadowPitch;
                }
                break;
            case 24:
                while (height--) {
                    /* TODO Faster method - need test
                    for(i=0; i<width*3; i=i+3) {
                         *(CARD16 *)(dst-i)=*(CARD16 *)(src+i);
                         *(dst-i+2)=*(src+i+2);
                    }
                    */
                    for(i=0; i<width*3; i=i+3) {
                         *(dst-i)=*(src+i);
                         *(dst-i+1)=*(src+i+1);
                         *(dst-i+2)=*(src+i+2);
                    }
                    dst -= FBPitch;
                    src += pVia->ShadowPitch;
                }
                break;
           case 32:
                while (height--) {
                    /* TODO Faster method  - need test
                    for(i=0; i<width*4; i=i+4) {
                         *(CARD32 *)(dst-i)=*(CARD32 *)(src+i);
                    }
                    */
                    for(i=0; i<width; i++) {
                         *(CARD32 *)(dst-i*4)=*(CARD32 *)(src+i*4);
                    }
                    dst -= FBPitch;
                    src += pVia->ShadowPitch;
                }
                break;
        }
        pbox++;
    }
}

static void
VIAPointerMoved(int index, int x, int y)
{
    ScrnInfoPtr pScrn = xf86Screens[index];
    VIAPtr pVia = VIAPTR(pScrn);
    int newX, newY;

    if (pVia->rotate == RR_Rotate_90) {
        newX = pScrn->pScreen->height - y - 1;
        newY = x;
    } else if (pVia->rotate == RR_Rotate_270) {
        newX = y;
        newY = pScrn->pScreen->width - x - 1;
    } else {
        newX = pScrn->pScreen->width - x - 1;
        newY = pScrn->pScreen->height - y - 1;
    }

    (*pVia->PointerMoved) (index, newX, newY);
}

static void
VIARefreshArea8(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    VIAPtr pVia = VIAPTR(pScrn);
    int count, width, height, y1, y2, dstPitch, srcPitch, tmpRot;
    CARD8 *dstPtr, *srcPtr, *src;
    CARD32 *dst;

    if (pVia->rotate == RR_Rotate_0)
    {
        VIARefreshArea(pScrn, num, pbox);
        return;
    }

    if (pVia->rotate == RR_Rotate_180) {
        VIARefreshArea_UD(pScrn, num, pbox);
        return;
    }

    if (pVia->rotate == RR_Rotate_90) {
       tmpRot=1;
    } else {
       tmpRot=-1;       
    }

    dstPitch = pScrn->displayWidth;
    srcPitch = -tmpRot*pVia->ShadowPitch;

    while (num--) {
        width = pbox->x2 - pbox->x1;
        y1 = pbox->y1 & ~3;
        y2 = (pbox->y2 + 3) & ~3;
        height = (y2 - y1) >> 2;  /* in dwords */

        if (pVia->rotate == RR_Rotate_90) {
            dstPtr = (pVia->FBBase
                      + (pbox->x1 * dstPitch) + pScrn->virtualX - y2);
            srcPtr = pVia->ShadowPtr + ((1 - y2) * srcPitch) + pbox->x1;
        } else {
            dstPtr = (pVia->FBBase
                      + ((pScrn->virtualY - pbox->x2) * dstPitch) + y1);
            srcPtr = pVia->ShadowPtr + (y1 * srcPitch) + pbox->x2 - 1;
        }

        while (width--) {
            src = srcPtr;
            dst = (CARD32*) dstPtr;
            count = height;

            while (count--) {
                *(dst++) = (src[0] | (src[srcPitch] << 8) |
                            (src[srcPitch * 2] << 16) |
                            (src[srcPitch * 3] << 24));
                src += srcPitch * 4;
            }

            srcPtr += tmpRot;
            dstPtr += dstPitch;
        }

        pbox++;
    }
}

static void
VIARefreshArea16(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    VIAPtr pVia = VIAPTR(pScrn);
    int count, width, height, y1, y2, dstPitch, srcPitch, tmpRot;
    CARD16 *dstPtr, *srcPtr, *src;
    CARD32 *dst;

    if (pVia->rotate == RR_Rotate_0)
    {
        VIARefreshArea(pScrn, num, pbox);
        return;
    }

    if (pVia->rotate == RR_Rotate_180) {
        VIARefreshArea_UD(pScrn, num, pbox);
        return;
    }

    if (pVia->rotate == RR_Rotate_90) {
       tmpRot=1;
    } else {
       tmpRot=-1;       
    }

    dstPitch = pScrn->displayWidth;
    srcPitch = -tmpRot*pVia->ShadowPitch >> 1;

    while (num--) {
        width = pbox->x2 - pbox->x1;
        y1 = pbox->y1 & ~1;
        y2 = (pbox->y2 + 1) & ~1;
        height = (y2 - y1) >> 1;  /* in dwords */

        if (pVia->rotate == RR_Rotate_90) {
            dstPtr = ((CARD16*) pVia->FBBase
                      + (pbox->x1 * dstPitch) + pScrn->virtualX - y2);
            srcPtr = ((CARD16*) pVia->ShadowPtr
                      + ((1 - y2) * srcPitch) + pbox->x1);
        } else {
            dstPtr = ((CARD16*) pVia->FBBase
                      + ((pScrn->virtualY - pbox->x2) * dstPitch) + y1);
            srcPtr = ((CARD16*) pVia->ShadowPtr
                      + (y1 * srcPitch) + pbox->x2 - 1);
        }

        while (width--) {
            src = srcPtr;
            dst = (CARD32*) dstPtr;
            count = height;

            while (count--) {
                *(dst++) = src[0] | (src[srcPitch] << 16);
                src += srcPitch * 2;
            }

            srcPtr += tmpRot;
            dstPtr += dstPitch;
        }

        pbox++;
    }
}

#if 0
/* this one could be faster */
void
VIARefreshArea24(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    VIAPtr pVia = VIAPTR(pScrn);
    int count, width, height, y1, y2, dstPitch, srcPitch, tmpRot;
    CARD8 *dstPtr, *srcPtr, *src;
    CARD32 *dst;

    if (pVia->rotate == RR_Rotate_0)
    {
        VIARefreshArea(pScrn, num, pbox);
        return;
    }


    if (pVia->rotate == RR_Rotate_180) {
        VIARefreshArea_UD(pScrn, num, pbox);
        return;
    }

    if (pVia->rotate == RR_Rotate_90) {
       tmpRot=1;
    } else {
       tmpRot=-1;       
    }

    dstPitch = BitmapBytePad(pScrn->displayWidth * 24);
    srcPitch = -tmpRot * pVia->ShadowPitch;

    while (num--) {
        width = pbox->x2 - pbox->x1;
        y1 = pbox->y1 & ~3;
        y2 = (pbox->y2 + 3) & ~3;
        height = (y2 - y1) >> 2;  /* blocks of 3 dwords */

        if (pVia->rotate == RR_Rotate_90) {
            dstPtr = (pVia->FBBase
                      + (pbox->x1 * dstPitch) + ((pScrn->virtualX - y2) * 3));
            srcPtr = pVia->ShadowPtr + ((1 - y2) * srcPitch) + (pbox->x1 * 3);
        } else {
            dstPtr = (pVia->FBBase
                      + ((pScrn->virtualY - pbox->x2) * dstPitch) + (y1 * 3));
            srcPtr = pVia->ShadowPtr + (y1 * srcPitch) + (pbox->x2 * 3) - 3;
        }

        while (width--) {
            src = srcPtr;
            dst = (CARD32*) dstPtr;
            count = height;
            while (count--) {
                dst[0] = (src[0] | (src[1] << 8) | (src[2] << 16) |
                          (src[srcPitch] << 24));
                dst[1] = (src[srcPitch + 1] | (src[srcPitch + 2] << 8) |
                          (src[srcPitch * 2] << 16) |
                          (src[(srcPitch * 2) + 1] << 24));
                dst[2] = (src[(srcPitch * 2) + 2] | (src[srcPitch * 3] << 8) |
                          (src[(srcPitch * 3) + 1] << 16) |
                          (src[(srcPitch * 3) + 2] << 24));
                dst += 3;
                src += srcPitch * 4;
            }

            srcPtr += tmpRot * 3;
            dstPtr += dstPitch;
        }

        pbox++;
    }
}
#endif

static void
VIARefreshArea32(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    VIAPtr pVia = VIAPTR(pScrn);
    int count, width, height, dstPitch, srcPitch, tmpRot;
    CARD32 *dstPtr, *srcPtr, *src, *dst;

    if (pVia->rotate == RR_Rotate_0)
    {
        VIARefreshArea(pScrn, num, pbox);
        return;
    }

    if (pVia->rotate == RR_Rotate_180) {
        VIARefreshArea_UD(pScrn, num, pbox);
        return;
    }

    if (pVia->rotate == RR_Rotate_90) {
       tmpRot=1;
    } else {
       tmpRot=-1;       
    }
    dstPitch = pScrn->displayWidth;
    srcPitch = -tmpRot * pVia->ShadowPitch >> 2;

    while (num--) {
        width = pbox->x2 - pbox->x1;
        height = pbox->y2 - pbox->y1;

        if (pVia->rotate == RR_Rotate_90) {
            dstPtr = ((CARD32*) pVia->FBBase
                      + (pbox->x1 * dstPitch) + pScrn->virtualX - pbox->y2);
            srcPtr = ((CARD32*) pVia->ShadowPtr
                      + ((1 - pbox->y2) * srcPitch) + pbox->x1);
        } else {
            dstPtr = ((CARD32*) pVia->FBBase
                      + ((pScrn->virtualY - pbox->x2) * dstPitch) + pbox->y1);
            srcPtr = ((CARD32*) pVia->ShadowPtr
                      + (pbox->y1 * srcPitch) + pbox->x2 - 1);
        }

        while (width--) {
            src = srcPtr;
            dst = dstPtr;
            count = height;

            while (count--) {
                *(dst++) = *src;
                src += srcPitch;
            }

            srcPtr += tmpRot;
            dstPtr += dstPitch;
        }

        pbox++;
    }
}

void
ViaShadowFBInit(ScrnInfoPtr pScrn, ScreenPtr pScreen)
{
    VIAPtr pVia = VIAPTR(pScrn);
    RefreshAreaFuncPtr refreshArea = VIARefreshArea;

    if ((pVia->rotate)||(pVia->RandRRotation==TRUE)) {
        if (!pVia->PointerMoved) {
            pVia->PointerMoved = pScrn->PointerMoved;
            pScrn->PointerMoved = VIAPointerMoved;
        }

        switch (pScrn->bitsPerPixel) {
            case 8:
                refreshArea = VIARefreshArea8;
                break;
            case 16:
                refreshArea = VIARefreshArea16;
                break;
            case 32:
                refreshArea = VIARefreshArea32;
                break;
        }
    }

    ShadowFBInit(pScreen, refreshArea);
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ShadowFB initialised.\n");
}
