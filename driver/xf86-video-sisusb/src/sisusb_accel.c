/*
 * 2D Acceleration for SiS 315/USB - not functional!
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria
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
 *
 * Author:  	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sisusb.h"
#include "sisusb_regs.h"
#include "sisusb_accel.h"

/* VRAM queue acceleration specific */

#define SIS_WQINDEX(i)  sis_accel_cmd_buffer[i]
#define SIS_RQINDEX(i)  ((ULong)tt + 1)  /* Bullshit, but we don't flush anyway */

static void
SiSUSBInitializeAccelerator(ScrnInfoPtr pScrn)
{
#ifndef SISVRAMQ
	SISUSBPtr  pSiSUSB = SISUSBPTR(pScrn);

	if(pSiSUSB->ChipFlags & SiSCF_Integrated) {
	   CmdQueLen = 0;
        } else {
	   CmdQueLen = ((128 * 1024) / 4) - 64;
        }
#endif
}

Bool
SiSUSBAccelInit(ScreenPtr pScreen)
{
	ScrnInfoPtr	pScrn = xf86Screens[pScreen->myNum];
	SISUSBPtr	pSiSUSB = SISUSBPTR(pScrn);
	int		topFB, reservedFbSize, usableFbSize;
	BoxRec		Avail;

	pSiSUSB->ColorExpandBufferNumber = 0;
	pSiSUSB->PerColorExpandBufferSize = 0;

	if((pScrn->bitsPerPixel != 8)  &&
	   (pScrn->bitsPerPixel != 16) &&
	   (pScrn->bitsPerPixel != 32)) {
	   pSiSUSB->NoAccel = TRUE;
	}

	if(!pSiSUSB->NoAccel) {

	   SiSUSBInitializeAccelerator(pScrn);

	}

	/* Init framebuffer memory manager */

	topFB = pSiSUSB->maxxfbmem;

	reservedFbSize = pSiSUSB->ColorExpandBufferNumber * pSiSUSB->PerColorExpandBufferSize;

	usableFbSize = topFB - reservedFbSize;

	/* Layout:
	 * |--------------++++++++++++++++++++^************==========~~~~~~~~~~~~|
	 *   UsableFbSize  ColorExpandBuffers |  DRI-Heap   HWCursor  CommandQueue
	 *                                 topFB
	 */

	Avail.x1 = 0;
	Avail.y1 = 0;
	Avail.x2 = pScrn->displayWidth;
	Avail.y2 = (usableFbSize / (pScrn->displayWidth * pScrn->bitsPerPixel/8)) - 1;

	if(Avail.y2 < 0) Avail.y2 = 32767;

	if(Avail.y2 < pScrn->currentMode->VDisplay) {
	   xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"Not enough video RAM for accelerator. At least "
		"%dKB needed, %ldKB available\n",
		((((pScrn->displayWidth * pScrn->bitsPerPixel/8)   /* +8 for make it sure */
		     * pScrn->currentMode->VDisplay) + reservedFbSize) / 1024) + 8,
		pSiSUSB->maxxfbmem/1024);
	   pSiSUSB->NoAccel = TRUE;
	   pSiSUSB->NoXvideo = TRUE;
	   return FALSE;   /* Don't even init fb manager */
	}

	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "Framebuffer from (%d,%d) to (%d,%d)\n",
		   Avail.x1, Avail.y1, Avail.x2 - 1, Avail.y2 - 1);

	xf86InitFBManager(pScreen, &Avail);

	return TRUE;
}









