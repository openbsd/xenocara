/*
 * SiS USB driver shadow framebuffer handling
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
 * Author: Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sisusb.h"
#define NEED_cpu_to_le16
#include "sisusb_regs.h"

#include "servermd.h"

#if 0
extern void SiSUSBSync(ScrnInfoPtr pScrn);
#endif

void SISUSBRefreshArea(ScrnInfoPtr pScrn, int num, BoxPtr pbox);
void SISUSBDoRefreshArea(ScrnInfoPtr pScrn);

void
SISUSBRefreshArea(ScrnInfoPtr pScrn, int num, BoxPtr pbox)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

#if 0
    if(pSiSUSB->IgnoreRefresh) return;
#endif

#if X_BYTE_ORDER == X_BIG_ENDIAN
    if(pScrn->bitsPerPixel == 16) {

       int FBPitch = pSiSUSB->ShadowPitch >> 1;
       int w, wb, h, i, offset;
       int count = num;
       CARD16 *src16, *dst16;
       BoxPtr mbox = pbox;

       while(count--) {

          w = mbox->x2 - mbox->x1;
          h = mbox->y2 - mbox->y1;
	  i = FBPitch - w;
	  offset = (mbox->y1 * FBPitch) + mbox->x1;
          src16 = (CARD16 *)pSiSUSB->ShadowPtrSwap + offset;
          dst16 = (CARD16 *)pSiSUSB->ShadowPtr     + offset;

          while(h--) {
	     wb = w;
	     while(wb--) {
	        *dst16++ = sisusb_cpu_to_le16(*src16++);
	     }
	     dst16 += i;
	     src16 += i;
          }

          mbox++;
       }

    }
#endif

    if(!pSiSUSB->ShBoxcount) {
       pSiSUSB->ShXmin = pbox->x1;
       pSiSUSB->ShXmax = pbox->x2;
       pSiSUSB->ShYmin = pbox->y1;
       pSiSUSB->ShYmax = pbox->y2;
       pbox++;
       pSiSUSB->ShBoxcount++;
       num--;
    }

    while(num--) {
        if(pbox->y1 < pSiSUSB->ShYmin) {
	   pSiSUSB->ShYmin = pbox->y1;
	   pSiSUSB->ShXmin = pbox->x1;
	} else if(pbox->y1 == pSiSUSB->ShYmin) {
	   if(pbox->x1 < pSiSUSB->ShXmin) pSiSUSB->ShXmin = pbox->x1;
	}
	if(pbox->y2 > pSiSUSB->ShYmax) {
	   pSiSUSB->ShYmax = pbox->y2;
	   pSiSUSB->ShXmax = pbox->x2;
	} else if(pbox->y2 == pSiSUSB->ShYmax) {
	   if(pbox->x2 > pSiSUSB->ShXmax) pSiSUSB->ShXmax = pbox->x2;
	}
	pSiSUSB->ShBoxcount++;
	pbox++;
    }
}

void
SISUSBDoRefreshArea(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    CARD8     *src, *dst;
    int       width, height, Bpp, FBPitch;
    int	      xmin = pSiSUSB->ShXmin;
    int	      xmax = pSiSUSB->ShXmax;
    int	      ymin = pSiSUSB->ShYmin;
    int	      ymax = pSiSUSB->ShYmax;

    if(pSiSUSB->delaycount++ < 3) return;
    pSiSUSB->delaycount = 0;

    if(!pSiSUSB->ShBoxcount) return;

    Bpp = pScrn->bitsPerPixel >> 3;
    FBPitch = pSiSUSB->ShadowPitch;

#if 0
    if(pSiSUSB->AccelNeedSync) {
       SiSUSBSync(pScrn);
    }
#endif

    src = pSiSUSB->ShadowPtr + (ymin * FBPitch) + (xmin * Bpp);
    dst = pSiSUSB->FbBase + (ymin * FBPitch) + (xmin * Bpp);
    height = ymax - ymin;
    width = ((height - 1) * FBPitch) - (xmin * Bpp) + (xmax * Bpp);
    SiSUSBMemCopyToVideoRam(pSiSUSB, dst, src, width);

    pSiSUSB->ShBoxcount = 0;
}
