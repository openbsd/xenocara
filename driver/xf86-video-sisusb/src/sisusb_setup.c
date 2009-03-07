/*
 * Basic hardware and memory detection
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
 * Author:  	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sisusb.h"
#include "sisusb_regs.h"

extern int SiSUSBMclk(SISUSBPtr pSiSUSB);

static  void
sis315USBSetup(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    int busSDR[4]  = {64, 64, 128, 128};
    int busDDR[4]  = {32, 32,  64,  64};
    int busDDRA[4] = {64+32, 64+32 , (64+32)*2, (64+32)*2};
    unsigned int config, config1, config2, sr3a, cr5f;
    char *dramTypeStr315[] = {
        "Single channel 1 rank SDR SDRAM",
        "Single channel 1 rank SDR SGRAM",
        "Single channel 1 rank DDR SDRAM",
        "Single channel 1 rank DDR SGRAM",
        "Single channel 2 rank SDR SDRAM",
        "Single channel 2 rank SDR SGRAM",
        "Single channel 2 rank DDR SDRAM",
        "Single channel 2 rank DDR SGRAM",
	"Asymmetric SDR SDRAM",
	"Asymmetric SDR SGRAM",
	"Asymmetric DDR SDRAM",
	"Asymmetric DDR SGRAM",
	"Dual channel SDR SDRAM",
	"Dual channel SDR SGRAM",
	"Dual channel DDR SDRAM",
	"Dual channel DDR SGRAM"
    };

    inSISIDXREG(pSiSUSB, SISSR, 0x14, config);
    config1 = (config & 0x0C) >> 2;

    inSISIDXREG(pSiSUSB, SISSR, 0x3A, sr3a);
    config2 = sr3a & 0x03;

    inSISIDXREG(pSiSUSB, SISCR,0x5f,cr5f);

    pScrn->videoRam = (1 << ((config & 0xF0) >> 4)) * 1024;

    pSiSUSB->IsAGPCard = FALSE;

    if(cr5f & 0x10) pSiSUSB->ChipFlags |= SiSCF_Is315E;

    /* If SINGLE_CHANNEL_2_RANK or DUAL_CHANNEL_1_RANK -> mem * 2 */
    if((config1 == 0x01) || (config1 == 0x03)) {
       pScrn->videoRam <<= 1;
    }

    /* If DDR asymetric -> mem * 1,5 */
    if(config1 == 0x02) pScrn->videoRam += pScrn->videoRam/2;

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
            "DRAM type: %s\n", dramTypeStr315[(config1 * 4) + config2]);

    pSiSUSB->MemClock = SiSUSBMclk(pSiSUSB);

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
            "Memory clock: %3.3f MHz\n",
            pSiSUSB->MemClock/1000.0);

    /* DDR -> mclk * 2 - needed for bandwidth calculation */
    if(config2 & 0x02) pSiSUSB->MemClock *= 2;

    if(config1 == 0x02)
       pSiSUSB->BusWidth = busDDRA[(config & 0x03)];
    else if(config2 & 0x02)
       pSiSUSB->BusWidth = busDDR[(config & 0x03)];
    else
       pSiSUSB->BusWidth = busSDR[(config & 0x03)];

    if(pSiSUSB->ChipFlags & SiSCF_Is315E) {
       inSISIDXREG(pSiSUSB, SISSR,0x15,config);
       if(config & 0x10) pSiSUSB->BusWidth = 32;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
            "DRAM bus width: %d bit\n",
	    pSiSUSB->BusWidth);
}

void
SiSUSBSetup(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    pSiSUSB->VBFlags = 0;
    pSiSUSB->VBFlags2 = 0;

    sis315USBSetup(pScrn);
}


