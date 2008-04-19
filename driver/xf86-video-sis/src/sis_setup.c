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
 * Ideas and methods for old series based on code by Can-Ru Yeou, SiS Inc.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sis.h"
#define SIS_NEED_inSISREGW
#define SIS_NEED_inSISREGL
#define SIS_NEED_outSISREGW
#define SIS_NEED_outSISREGL
#define SIS_NEED_inSISIDXREG
#define SIS_NEED_outSISIDXREG
#include "sis_regs.h"

extern int SiSMclk(SISPtr pSiS);

static const char *dramTypeStr[] = {
	"Fast Page DRAM",
	"2 cycle EDO RAM",
	"1 cycle EDO RAM",
	"SDRAM/SGRAM",
	"SDR SDRAM",
	"SGRAM",
	"ESDRAM",
	"DDR SDRAM",  /* for 550/650/etc */
	"DDR SDRAM",  /* for 550/650/etc */
	"VCM",	      /* for 630 */
	"DDR2 SDRAM", /* for 340 */
	""
};

/* MCLK tables for SiS6326 */
static const int SiS6326MCLKIndex[4][8] = {
	{ 10, 12, 14, 16, 17, 18, 19,  7 },  /* SGRAM */
	{  4,  6,  8, 10, 11, 12, 13,  3 },  /* Fast Page */
	{  9, 11, 12, 13, 15, 16,  5,  7 },  /* 2 cycle EDO */
	{ 10, 12, 14, 16, 17, 18, 19,  7 }   /* ? (Not 1 cycle EDO) */
};

static const struct _sis6326mclk {
    CARD16 mclk;
    UChar  sr13;
    UChar  sr28;
    UChar  sr29;
} SiS6326MCLK[] = {
	{  0, 0,    0,    0 },
	{  0, 0,    0,    0 },
	{  0, 0,    0,    0 },
	{ 45, 0, 0x2b, 0x26 },
	{ 53, 0, 0x49, 0xe4 },
	{ 55, 0, 0x7c, 0xe7 },
	{ 56, 0, 0x7c, 0xe7 },
	{ 60, 0, 0x42, 0xe3 },
	{ 61, 0, 0x21, 0xe1 },
	{ 65, 0, 0x5a, 0xe4 },
	{ 66, 0, 0x5a, 0xe4 },
	{ 70, 0, 0x61, 0xe4 },
	{ 75, 0, 0x3e, 0xe2 },
	{ 80, 0, 0x42, 0xe2 },
	{ 83, 0, 0xb3, 0xc5 },
	{ 85, 0, 0x5e, 0xe3 },
	{ 90, 0, 0xae, 0xc4 },
	{100, 0, 0x37, 0xe1 },
	{115, 0, 0x78, 0x0e },
	{134, 0, 0x4a, 0xa3 }
};

#ifdef XSERVER_LIBPCIACCESS
struct pci_device *
sis_get_device (int device)
{
    struct pci_slot_match bridge_match = {
	0, 0, device, PCI_MATCH_ANY, 0
    };
    struct pci_device_iterator	*slot_iterator;
    struct pci_device		*bridge;

    slot_iterator = pci_slot_match_iterator_create (&bridge_match);
    bridge = pci_device_next (slot_iterator);
    pci_iterator_destroy (slot_iterator);
    return bridge;
}

unsigned int
sis_pci_read_device_u32(int device, int offset)
{
    struct pci_device *host_bridge = sis_get_device(device);
    unsigned int result;

    pci_device_cfg_read_u32(host_bridge, &result, offset);
    return result;
}

unsigned char
sis_pci_read_device_u8(int device, int offset)
{
    struct pci_device *host_bridge = sis_get_device(device);
    unsigned char result;

    pci_device_cfg_read_u8(host_bridge, &result, offset);
    return result;
}

void
sis_pci_write_host_bridge_u32(int offset, unsigned int value)
{
    struct pci_device *host_bridge = sis_get_device(0);
    pci_device_cfg_write_u32(host_bridge, value, offset);
}

void
sis_pci_write_host_bridge_u8(int offset, unsigned char value)
{
    struct pci_device *host_bridge = sis_get_device(0);
    pci_device_cfg_write_u8(host_bridge, value, offset);
}

#else
unsigned int
sis_pci_read_device_u32(int device, int offset)
{
    PCITAG tag = pciTag(0, device, 0);
    return pciReadLong(tag, offset);
}

unsigned char
sis_pci_read_device_u8(int device, int offset)
{
    PCITAG tag = pciTag(0, device, 0);
    return pciReadByte(tag, offset);
}

void
sis_pci_write_host_bridge_u32(int offset, unsigned int value)
{
    pciWriteLong(0x00000000, offset, value);
}

void
sis_pci_write_host_bridge_u8(int offset, unsigned char value)
{
    pciWriteByte(0x00000000, offset, value);
}


#endif

unsigned int
sis_pci_read_host_bridge_u32(int offset)
{
    return sis_pci_read_device_u32(0, offset);
}

unsigned char
sis_pci_read_host_bridge_u8(int offset)
{
    return sis_pci_read_device_u8(0, offset);
}
   
static int sisESSPresent(ScrnInfoPtr pScrn)
{
  int flags = 0;
#ifndef XSERVER_LIBPCIACCESS
  int i;
  pciConfigPtr pdptr, *systemPCIdevices = NULL;

  if((systemPCIdevices = xf86GetPciConfigInfo())) {
      i = 0;
      while((pdptr = systemPCIdevices[i])) {
	  if((pdptr->pci_vendor == 0x1274) &&
	     ((pdptr->pci_device == 0x5000) ||
	      ((pdptr->pci_device & 0xFFF0) == 0x1370))) {
	      flags |= ESS137xPRESENT;
	      break;
	  }
	  i++;
      }
  }
  return flags;
#else
  struct pci_id_match id_match = { 0x1274, PCI_MATCH_ANY,
				   PCI_MATCH_ANY, PCI_MATCH_ANY,
				   PCI_MATCH_ANY, PCI_MATCH_ANY,
				   0 };
  struct pci_device_iterator *id_iterator;
  struct pci_device *ess137x;

  id_iterator = pci_id_match_iterator_create(&id_match);

  ess137x = pci_device_next(id_iterator);  
  while (ess137x) {
      if ((ess137x->device_id == 0x5000) ||
	  ((ess137x->device_id & 0xfff0) == 0x1370)) {
	  flags |= ESS137xPRESENT;
      }
      ess137x = pci_device_next(id_iterator);  
  }
  return flags;
#endif
}

/* For old chipsets, 5597, 6326, 530/620 */
static void
sisOldSetup(ScrnInfoPtr pScrn)
{
    SISPtr pSiS = SISPTR(pScrn);
    int    ramsize[8]  = { 1,  2,  4, 0, 0,  2,  4,  8};
    int    buswidth[8] = {32, 64, 64, 0, 0, 32, 32, 64 };
    int    clockTable[4] = { 66, 75, 83, 100 };
    int    ramtype[4]  = { 5, 0, 1, 3 };
    int    config, temp, i;
    UChar  sr23, sr33, sr37;
#if 0
    UChar  newsr13, newsr28, newsr29;
#endif


    if(pSiS->oldChipset <= OC_SIS6225) {
	inSISIDXREG(SISSR, 0x0F, temp);
	pScrn->videoRam = (1 << (temp & 0x03)) * 1024;
	if(pScrn->videoRam > 4096) pScrn->videoRam = 4096;
	pSiS->BusWidth = 32;
    } else if(pSiS->Chipset == PCI_CHIP_SIS5597) {
	inSISIDXREG(SISSR, 0x2F, temp);
	pScrn->videoRam = ((temp & 0x07) + 1) * 256;
	inSISIDXREG(SISSR, 0x0C, temp);
	if(temp & 0x06) {
		pScrn->videoRam *= 2;
		pSiS->BusWidth = 64;
	} else  pSiS->BusWidth = 32;
    } else {
	inSISIDXREG(SISSR, 0x0C, temp);
	config = ((temp & 0x10) >> 2 ) | ((temp & 0x06) >> 1);
	pScrn->videoRam = ramsize[config] * 1024;
	pSiS->BusWidth = buswidth[config];
    }

    if(pSiS->Chipset == PCI_CHIP_SIS530)  {

	inSISIDXREG(SISSR, 0x0D, temp);
	pSiS->Flags &= ~(UMA);
	if(temp & 0x01) {
		pSiS->Flags |= UMA;  		/* Shared fb mode */
		inSISIDXREG(SISSR, 0x10, temp);
		pSiS->MemClock = clockTable[temp & 0x03] * 1000;
	} else  {
		pSiS->MemClock = SiSMclk(pSiS); /* Local fb mode */
	}

    } else if(pSiS->Chipset == PCI_CHIP_SIS6326) {

	inSISIDXREG(SISSR,0x0e,temp);

	i = temp & 0x03;

	xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	    "DRAM type: %s\n",
	    dramTypeStr[ramtype[i]]);

	temp = (temp >> 5) & 0x07;
	i = SiS6326MCLKIndex[i][temp];
	pSiS->MemClock = SiS6326MCLK[i].mclk;
#if 0
	/* Correct invalid MCLK settings by old BIOSes */
	newsr13 = SiS6326MCLK[i].sr13;
	newsr28 = SiS6326MCLK[i].sr28;
	newsr29 = SiS6326MCLK[i].sr29;
	if((pSiS->ChipRev == 0x92) ||
	   (pSiS->ChipRev == 0xd1) ||
	   (pSiS->ChipRev == 0xd2)) {
	   if(pSiS->MemClock == 60) {
	      newsr28 = 0xae;
	      newsr29 = 0xc4;
	   }
	}
#endif
	pSiS->MemClock *= 1000;
#if 0
	inSISIDXREG(SISSR, 0x13, temp);
	temp &= 0x80;
	temp |= (newsr13 & 0x80);
	outSISIDXREG(SISSR,0x13,temp);
	outSISIDXREG(SISSR,0x28,newsr28);
	outSISIDXREG(SISSR,0x29,newsr29);
#endif

    } else {

	pSiS->MemClock = SiSMclk(pSiS);

    }

    pSiS->Flags &= ~(SYNCDRAM | RAMFLAG);
    if(pSiS->oldChipset >= OC_SIS82204) {
       inSISIDXREG(SISSR, 0x23, sr23);
       inSISIDXREG(SISSR, 0x33, sr33);
       if(pSiS->oldChipset >= OC_SIS530A) sr33 &= ~0x08;
       if(sr33 & 0x09) {				/* 5597: Sync DRAM timing | One cycle EDO ram;   */
		pSiS->Flags |= (sr33 & SYNCDRAM);	/* 6326: Enable SGRam timing | One cycle EDO ram */
		pSiS->Flags |= RAMFLAG;			/* 530:  Enable SGRAM timing | reserved (0)      */
       } else if((pSiS->oldChipset < OC_SIS530A) && (sr23 & 0x20)) {
		pSiS->Flags |= SYNCDRAM;		/* 5597, 6326: EDO DRAM enabled */
       }						/* 530/620:    reserved (0)     */
    }

    pSiS->Flags &= ~(ESS137xPRESENT);
    if(pSiS->Chipset == PCI_CHIP_SIS530) {
       if(pSiS->oldChipset == OC_SIS530A) {
	   pSiS->Flags |= sisESSPresent(pScrn);
       }
       if(pSiS->Flags & ESS137xPRESENT) {
	   xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		      "SiS530/620: Found ESS device\n");
       }
    }

    pSiS->Flags &= ~(SECRETFLAG);
    if(pSiS->oldChipset >= OC_SIS5597) {
	inSISIDXREG(SISSR, 0x37, sr37);
	if(sr37 & 0x80) pSiS->Flags |= SECRETFLAG;
    }

    pSiS->Flags &= ~(A6326REVAB);
    if(pSiS->Chipset == PCI_CHIP_SIS6326) {
       if(((pSiS->ChipRev & 0x0f) == 0x0a) ||
	  ((pSiS->ChipRev & 0x0f) == 0x0b)) {
	    pSiS->Flags |= A6326REVAB;
       }
    }

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	       "Memory clock: %3.3f MHz\n",
	       pSiS->MemClock/1000.0);

    if(pSiS->oldChipset > OC_SIS6225) {
       xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	       "DRAM bus width: %d bit\n",
	       pSiS->BusWidth);
    }

#ifdef TWDEBUG
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"oldChipset = %d, Flags %x\n", pSiS->oldChipset, pSiS->Flags);
#endif
}

static void
sis300Setup(ScrnInfoPtr pScrn)
{
    SISPtr    pSiS = SISPTR(pScrn);
    const int bus[4] = {32, 64, 128, 32};
    const int adaptermclk[8]    = {  66,  83, 100, 133,
                                    100, 100, 100, 100};
    const int adaptermclk300[8] = { 125, 125, 125, 100,
                                    100, 100, 100, 100};
    unsigned int config, pciconfig, sr3a, ramtype;
    UChar        temp;
    int		 cpubuswidth;
    MessageType	 from = X_PROBED;

    pSiS->MemClock = SiSMclk(pSiS);

    inSISIDXREG(SISSR, 0x14, config);
    cpubuswidth = bus[config >> 6];

    inSISIDXREG(SISSR, 0x3A, sr3a);
    ramtype = (sr3a & 0x03) + 4;

    pSiS->IsPCIExpress = FALSE;

    switch(pSiS->Chipset) {
    case PCI_CHIP_SIS300:
	pScrn->videoRam = ((config & 0x3F) + 1) * 1024;
	pSiS->LFBsize = pScrn->videoRam;
	pSiS->BusWidth = cpubuswidth;
	pSiS->IsAGPCard = ((sr3a & 0x30) == 0x30) ? FALSE : TRUE;
	break;
    case PCI_CHIP_SIS540:
    case PCI_CHIP_SIS630:
	pSiS->IsAGPCard = TRUE;
	pciconfig = sis_pci_read_host_bridge_u8(0x63);
	if(pciconfig & 0x80) {
	   pScrn->videoRam = (1 << (((pciconfig & 0x70) >> 4) + 21)) / 1024;
	   pSiS->BusWidth = 64;
	   pciconfig = sis_pci_read_host_bridge_u8(0x64);
	   if((pciconfig & 0x30) == 0x30) {
	      pSiS->BusWidth = 128;
	      pScrn->videoRam <<= 1;
	   }
	   ramtype = sis_pci_read_host_bridge_u8(0x65);
	   ramtype &= 0x03;
	   xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		"Shared Memory Area is on DIMM%d\n", ramtype);
	   ramtype = sis_pci_read_host_bridge_u8(0x60 + ramtype);
	   if(ramtype & 0x80) ramtype = 9;
	   else               ramtype = 4;
	   pSiS->UMAsize = pScrn->videoRam;
	} else {
	   xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		"Shared Memory Area is disabled - awaiting doom\n");
	   pScrn->videoRam = ((config & 0x3F) + 1) * 1024;
	   pSiS->UMAsize = pScrn->videoRam;
	   pSiS->BusWidth = 64;
	   ramtype = 4;
	   from = X_INFO;
	}
	break;
    default:
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		"Internal error: sis300setup() called with invalid chipset!\n");
	pSiS->BusWidth = 64;
	from = X_INFO;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	    "DRAM type: %s\n",
	    dramTypeStr[ramtype]);

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	    "Memory clock: %3.3f MHz\n",
	    pSiS->MemClock/1000.0);

    if(pSiS->Chipset == PCI_CHIP_SIS300) {
       if(pSiS->ChipRev > 0x13) {
	  inSISIDXREG(SISSR, 0x3A, temp);
	  xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	     "(Adapter assumes MCLK being %d Mhz)\n",
	     adaptermclk300[(temp & 0x07)]);
       }
    } else {
       inSISIDXREG(SISSR, 0x1A, temp);
       xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	    "(Adapter assumes MCLK being %d Mhz)\n",
	    adaptermclk[(temp & 0x07)]);
    }

    xf86DrvMsg(pScrn->scrnIndex, from,
	    "DRAM bus width: %d bit\n",
	    pSiS->BusWidth);
}

/* For 315, 315H, 315PRO/E, 330, 340 */
static void
sis315Setup(ScrnInfoPtr pScrn)
{
    SISPtr  pSiS = SISPTR(pScrn);
    int     busSDR[4]  = {64, 64, 128, 128};
    int     busDDR[4]  = {32, 32,  64,  64};
    int     busDDRA[4] = {64+32, 64+32 , (64+32)*2, (64+32)*2};
    unsigned int config, config1, config2, sr3a, cr5f;
    char    *dramTypeStr315[] = {
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
    char    *dramTypeStr330[] = {
	"Single Channel SDR SDRAM",
	"",
	"Single Channel DDR SDRAM",
	"",
	"--unknown--",
	"",
	"--unknown--",
	"",
	"Asymetric Dual Channel SDR SDRAM",
	"",
	"Asymetric Dual Channel DDR SDRAM",
	"",
	"Dual channel SDR SDRAM",
	"",
	"Dual channel DDR SDRAM",
	""
    };
    char    *dramTypeStr340[] = {
	"Single channel DDR SDRAM",
	"Single channel DDR2 SDRAM",
	"Single channel DDR2x SDRAM",
	"",
	"Dual channel DDR SDRAM",
	"Dual channel DDR2 SDRAM",
	"Dual channel DDR2x SDRAM",
	"",
	"Dual channel DDR SDRAM",
	"Dual channel DDR2 SDRAM",
	"Dual channel DDR2x SDRAM",
	"",
	"Quad channel DDR SDRAM",
	"Quad channel DDR2 SDRAM",
	"Quad channel DDR2x SDRAM",
	""
    };

    inSISIDXREG(SISSR, 0x14, config);
    config1 = (config & 0x0C) >> 2;

    inSISIDXREG(SISSR, 0x3a, sr3a);
    config2 = sr3a & 0x03;

    inSISIDXREG(SISCR,0x5f,cr5f);

    pScrn->videoRam = (1 << ((config & 0xf0) >> 4)) * 1024;

    pSiS->IsPCIExpress = FALSE;

    switch(pSiS->Chipset) {

    case PCI_CHIP_SIS340:
    case PCI_CHIP_XGIXG20:
    case PCI_CHIP_XGIXG40:

       if(pSiS->ChipType != XGI_20) {	/* SIS340, XGI_40 */

          pSiS->IsAGPCard = TRUE;

          if(pSiS->ChipRev == 2) {
	     if(config1 & 0x01) config1 = 0x02;
	     else               config1 = 0x00;
          }
          if(config1 == 0x02)      pScrn->videoRam <<= 1; /* dual rank */
          else if(config1 == 0x03) pScrn->videoRam <<= 2; /* quad rank */

	  inSISIDXREG(SISSR, 0x39, config2);
	  config2 &= 0x02;
	  if(!config2) {
	     inSISIDXREG(SISSR, 0x3a, config2);
	     config2 = (config2 & 0x02) >> 1;
          }

	  pSiS->BusWidth = (config & 0x02) ? 64 : 32;

       } else {				/* XGI_20 (Z7) */

	  config1 = 0x00;
	  inSISIDXREG(SISCR, 0x97, config2);
	  config2 &= 0x01;
	  config2 <<= 1;	/* 0 or 2 */

	  pSiS->BusWidth = (config & 0x02) ? 32 :
				((config & 0x01) ? 16 : 8);

       }

       xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	    "DRAM type: %s\n", dramTypeStr340[(config1 * 4) + (config2 & 0x03)]);

       pSiS->MemClock = SiSMclk(pSiS);

       pSiS->MemClock *= 2; /* at least DDR */

       break;

    case PCI_CHIP_SIS330:

       pSiS->IsAGPCard = TRUE;

       if(config1) pScrn->videoRam <<= 1;

       xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	   "DRAM type: %s\n", dramTypeStr330[(config1 * 4) + (config2 & 0x02)]);

       pSiS->MemClock = SiSMclk(pSiS);

       if(config2 & 0x02) {
	  pSiS->MemClock *= 2;
	  if(config1 == 0x02) {
	     pSiS->BusWidth = busDDRA[0];
	  } else {
	     pSiS->BusWidth = busDDR[(config & 0x02)];
	  }
       } else {
	  if(config1 == 0x02) {
	     pSiS->BusWidth = busDDRA[2];
	  } else {
	     pSiS->BusWidth = busSDR[(config & 0x02)];
	  }
       }

       break;

    default: /* 315 */

       pSiS->IsAGPCard = ((sr3a & 0x30) == 0x30) ? FALSE : TRUE;

       if(cr5f & 0x10) pSiS->ChipFlags |= SiSCF_Is315E;

       /* If SINGLE_CHANNEL_2_RANK or DUAL_CHANNEL_1_RANK -> mem * 2 */
       if((config1 == 0x01) || (config1 == 0x03)) pScrn->videoRam <<= 1;

       /* If DDR asymetric -> mem * 1,5 */
       if(config1 == 0x02) pScrn->videoRam += pScrn->videoRam/2;

       xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	    "DRAM type: %s\n", dramTypeStr315[(config1 * 4) + config2]);

       pSiS->MemClock = SiSMclk(pSiS);

       /* If DDR -> memclock * 2 */
       if(config2 & 0x02) pSiS->MemClock *= 2;

       if(config1 == 0x02)
	  pSiS->BusWidth = busDDRA[(config & 0x03)];
       else if(config2 & 0x02)
	  pSiS->BusWidth = busDDR[(config & 0x03)];
       else
	  pSiS->BusWidth = busSDR[(config & 0x03)];

       if(pSiS->ChipFlags & SiSCF_Is315E) {
	  inSISIDXREG(SISSR,0x15,config);
	  if(config & 0x10) pSiS->BusWidth = 32;
       }

    }

    pSiS->LFBsize = pScrn->videoRam;

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		"Memory clock: %3.3f MHz\n",
		pSiS->MemClock/1000.0);

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		"DRAM bus width: %d bit\n",
		pSiS->BusWidth);
}

/* For 550, 65x, 740, 661, 741, 660, 760, 761 */
static void
sis550Setup(ScrnInfoPtr pScrn)
{
    SISPtr       pSiS = SISPTR(pScrn);
    unsigned int config, ramtype=0, i;
    CARD8	 pciconfig, temp;
    Bool	 alldone = FALSE;
    Bool	 ddrtimes2 = TRUE;

    pSiS->IsAGPCard = TRUE;
    pSiS->IsPCIExpress = FALSE;
    pSiS->ChipFlags &= ~(SiSCF_760UMA | SiSCF_760LFB);

    pSiS->MemClock = SiSMclk(pSiS);

    if(pSiS->Chipset == PCI_CHIP_SIS660) {

       if(pSiS->ChipType >= SIS_660) {

	  /* UMA - shared fb */
	  pScrn->videoRam = 0;
	  pciconfig = sis_pci_read_host_bridge_u8(0x4c);
	  if(pciconfig & 0xe0) {
	     pScrn->videoRam = (1 << (((pciconfig & 0xe0) >> 5) - 2)) * 32768;
	     pSiS->ChipFlags |= SiSCF_760UMA;
	     pSiS->SiS76xUMASize = pScrn->videoRam * 1024;
	     pSiS->UMAsize = pScrn->videoRam;
	     xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		"%dK shared video RAM (UMA)\n",
		pScrn->videoRam);
	  }

	  /* LFB - local framebuffer: PCI reg hold total RAM (but configurable in BIOS) */
	  /* TODO */
	  pciconfig = sis_pci_read_device_u8(1, 0xcd);
	  pciconfig = (pciconfig >> 1) & 0x03;
	  i = 0;
	  if(pciconfig == 0x01)      i = 32768;
	  else if(pciconfig == 0x03) i = 65536;
	  if(i) {
	     xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "%dK total local video RAM (LFB)\n", i);
	  }

	  /* LFB: CR78 holds amount of LFB memory configured in the BIOS setup */
	  inSISIDXREG(SISCR, 0x78, config);
	  config &= 0x30;
	  if(config) {
	     i = 0;
	     if(config == 0x10)      i = 32768;
	     else if(config == 0x30) i = 65536;
	     if(i) {
		xf86DrvMsg(pScrn->scrnIndex, X_PROBED, "%dK configured local video RAM (LFB)\n", i);
		pScrn->videoRam += i;
		pSiS->ChipFlags |= SiSCF_760LFB;
		pSiS->SiS76xLFBSize = i * 1024;
		pSiS->LFBsize = i;
	     }
	  }

	  if((pScrn->videoRam < 32768) || (pScrn->videoRam > 131072)) {
	     xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		 "Illegal video RAM size (%dK) detected, using BIOS provided setting\n",
		 pScrn->videoRam);
	     pSiS->ChipFlags &= ~(SiSCF_760LFB | SiSCF_760UMA);
	  } else {
	     pSiS->BusWidth = 64;
	     ramtype = 8;
	     alldone = TRUE;
	  }

	  if(pSiS->ChipType >= SIS_761) {
	     pSiS->IsAGPCard = FALSE;
	     pSiS->IsPCIExpress = TRUE;
	  }

       } else {  /* 661, 741 */

	  int dimmnum;

	  if(pSiS->ChipType == SIS_741) {
	     dimmnum = 4;
	  } else {
	     dimmnum = 3;
	  }

	  pciconfig = sis_pci_read_host_bridge_u8(0x64);
	  if(pciconfig & 0x80) {
	     pScrn->videoRam = (1 << (((pciconfig & 0x70) >> 4) - 1)) * 32768;
	     pSiS->UMAsize = pScrn->videoRam;
	     if((pScrn->videoRam < 32768) || (pScrn->videoRam > (128 * 1024))) {
		xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
			"Illegal video RAM size (%dK) detected, using BIOS-provided info\n",
			pScrn->videoRam);
	     } else {
		pSiS->BusWidth = 64;
		for(i = 0; i <= (dimmnum - 1); i++) {
		   if(pciconfig & (1 << i)) {
		      temp = sis_pci_read_host_bridge_u8(0x60 + i);
		      xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			  "DIMM%d is %s SDRAM\n",
			  i, (temp & 0x40) ? "DDR" : "SDR");
		   } else {
		      xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			  "DIMM%d is not installed\n", i);
		   }
		}
		pciconfig = sis_pci_read_host_bridge_u8(0x7c);
		ramtype = (pciconfig & 0x02) ? 8 : 4;
		alldone = TRUE;
	     }
          }

       }

    } else if(pSiS->Chipset == PCI_CHIP_SIS650) {

       pciconfig = sis_pci_read_host_bridge_u8(0x64);
       if(pciconfig & 0x80) {
          pScrn->videoRam = (1 << (((pciconfig & 0x70) >> 4) + 22)) / 1024;
	  pSiS->UMAsize = pScrn->videoRam;
	  pSiS->BusWidth = 64;
	  for(i=0; i<=3; i++) {
	     if(pciconfig & (1 << i)) {
		temp = sis_pci_read_host_bridge_u8(0x60 + i);
		xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		   "DIMM%d is %s SDRAM\n",
		   i, (temp & 0x40) ? "DDR" : "SDR");
	     } else {
		xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		   "DIMM%d is not installed\n", i);
	     }
	  }
	  pciconfig = sis_pci_read_host_bridge_u8(0x7c);
	  if(pciconfig & 0x02) ramtype = 8;
	  else                 ramtype = 4;
	  alldone = TRUE;
       }

    } else {
       pciconfig = sis_pci_read_host_bridge_u8(0x63);
       if(pciconfig & 0x80) {
	  pScrn->videoRam = (1 << (((pciconfig & 0x70) >> 4) + 21)) / 1024;
	  pSiS->UMAsize = pScrn->videoRam;
	  pSiS->BusWidth = 64;
          ramtype = sis_pci_read_host_bridge_u8(0x65);
	  ramtype &= 0x01;
	  xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
	   	"Shared Memory Area is on DIMM%d\n", ramtype);
	  ramtype = 4;
	  alldone = TRUE;
       }

    }

    /* Fall back to BIOS detection results in case of problems: */

    if(!alldone) {

       pSiS->SiS76xLFBSize = pSiS->SiS76xUMASize = 0;
       pSiS->UMAsize = pSiS->LFBsize = 0;

       if(pSiS->Chipset == PCI_CHIP_SIS660) {
	  inSISIDXREG(SISCR, 0x79, config);
	  pSiS->BusWidth = (config & 0x04) ? 128 : 64;
	  ramtype = (config & 0x01) ? 8 : 4;
	  if(pSiS->ChipType >= SIS_660) {
	     pScrn->videoRam = 0;
	     if(config & 0xf0) {
		pScrn->videoRam = (1 << ((config & 0xf0) >> 4)) * 1024;
		pSiS->UMAsize = pScrn->videoRam;
		pSiS->ChipFlags |= SiSCF_760UMA;
		pSiS->SiS76xUMASize = pScrn->videoRam * 1024;
		xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
				"%dK shared video RAM (UMA)\n",
				pScrn->videoRam);
	     }
	     inSISIDXREG(SISCR, 0x78, config);
	     config &= 0x30;
	     if(config) {
	        i = 0;
		if(config == 0x10)      i = 32768;
		else if(config == 0x30) i = 65536;
		if(i) {
		   xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
				"%dK configured local video RAM (LFB)\n", i);
		   pScrn->videoRam += i;
		   pSiS->SiS76xLFBSize = i * 1024;
		   pSiS->LFBsize = i;
		   pSiS->ChipFlags |= SiSCF_760LFB;

		}
	     }
	  } else {
	     pScrn->videoRam = (1 << ((config & 0xf0) >> 4)) * 1024;
	     pSiS->UMAsize = pScrn->videoRam;
	  }
       } else {
	  xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
	      "Shared Memory Area is disabled - awaiting doom\n");
	  inSISIDXREG(SISSR, 0x14, config);
	  pScrn->videoRam = (((config & 0x3F) + 1) * 4) * 1024;
	  pSiS->UMAsize = pScrn->videoRam;
	  if(pSiS->Chipset == PCI_CHIP_SIS650) {
	     ramtype = (((config & 0x80) >> 7) << 2) + 4;
	     pSiS->BusWidth = 64;   /* (config & 0x40) ? 128 : 64; */
	  } else {
	     ramtype = 4;
	     pSiS->BusWidth = 64;
	  }
       }
    }

    /* These need special attention: Memory controller in CPU, hence
     * - no DDR * 2 for bandwidth calculation,
     * - overlay magic (bandwidth dependent one/two overlay stuff)
     */
    if((pSiS->ChipType >= SIS_760) && (pSiS->ChipType <= SIS_770)) {
       if(!(pSiS->ChipFlags & SiSCF_760LFB)) {
	  ddrtimes2 = FALSE;
	  pSiS->SiS_SD2_Flags |= SiS_SD2_SUPPORT760OO;
       }
    }

    /* DDR -> Mclk * 2 - needed for bandwidth calculation */
    if(ddrtimes2) {
       if(ramtype == 8) pSiS->MemClock *= 2;
    }

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		"DRAM type: %s\n",
		dramTypeStr[ramtype]);

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		"Memory clock: %3.3f MHz\n",
		pSiS->MemClock/1000.0);

    xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		"DRAM bus width: %d bit\n",
		pSiS->BusWidth);
}

void
SiSSetup(ScrnInfoPtr pScrn)
{
    SISPtr pSiS = SISPTR(pScrn);

    pSiS->Flags = 0;
    pSiS->VBFlags = 0;
    pSiS->SiS76xLFBSize = pSiS->SiS76xUMASize = 0;
    pSiS->UMAsize = pSiS->LFBsize = 0;

    switch (SISPTR(pScrn)->Chipset) {
    case PCI_CHIP_SIS300:
    case PCI_CHIP_SIS630:  /* +730 */
    case PCI_CHIP_SIS540:
        sis300Setup(pScrn);
        break;
    case PCI_CHIP_SIS315:
    case PCI_CHIP_SIS315H:
    case PCI_CHIP_SIS315PRO:
    case PCI_CHIP_SIS330:
    case PCI_CHIP_SIS340:
    case PCI_CHIP_XGIXG20:
    case PCI_CHIP_XGIXG40:
    	sis315Setup(pScrn);
	break;
    case PCI_CHIP_SIS550:
    case PCI_CHIP_SIS650: /* + 740,M650,651 */
    case PCI_CHIP_SIS660: /* + (M)661,(M)741,(M)760(GX), (M)761(GX), 770? */
        sis550Setup(pScrn);
	break;
    case PCI_CHIP_SIS5597:
    case PCI_CHIP_SIS6326:
    case PCI_CHIP_SIS530:
    default:
        sisOldSetup(pScrn);
        break;
    }
}


