/* $XFree86: xc/programs/Xserver/hw/xfree86/drivers/ati/radeon_bios.c,v 1.0 Exp $ */
/*
 * Copyright 2004 ATI Technologies Inc., Markham, Ontario
 *
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation on the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NON-INFRINGEMENT.  IN NO EVENT SHALL ATI, VA LINUX SYSTEMS AND/OR
 * THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include "xf86.h"
#include "xf86_OSproc.h"

#include "radeon.h"
#include "radeon_reg.h"
#include "radeon_macros.h"
#include "radeon_probe.h"
#include "vbe.h"

/* Read the Video BIOS block and the FP registers (if applicable). */
Bool RADEONGetBIOSInfo(ScrnInfoPtr pScrn, xf86Int10InfoPtr  pInt10)
{
    RADEONInfoPtr info     = RADEONPTR(pScrn);
    int tmp;
    unsigned short dptr;

    if (!(info->VBIOS = xalloc(RADEON_VBIOS_SIZE))) {
	xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
		   "Cannot allocate space for hold Video BIOS!\n");
	return FALSE;
    } else {
	if (pInt10) {
	    info->BIOSAddr = pInt10->BIOSseg << 4;
	    (void)memcpy(info->VBIOS, xf86int10Addr(pInt10, info->BIOSAddr),
			 RADEON_VBIOS_SIZE);
	} else {
	    xf86ReadPciBIOS(0, info->PciTag, 0, info->VBIOS, RADEON_VBIOS_SIZE);
	    if (info->VBIOS[0] != 0x55 || info->VBIOS[1] != 0xaa) {
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "Video BIOS not detected in PCI space!\n");
		xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
			   "Attempting to read Video BIOS from "
			   "legacy ISA space!\n");
		info->BIOSAddr = 0x000c0000;
		xf86ReadDomainMemory(info->PciTag, info->BIOSAddr,
				     RADEON_VBIOS_SIZE, info->VBIOS);
	    }
	}
    }

    if (info->VBIOS[0] != 0x55 || info->VBIOS[1] != 0xaa) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Unrecognized BIOS signature, BIOS data will not be used\n");
	xfree (info->VBIOS);
	info->VBIOS = NULL;
	return FALSE;
    }

    /* Verify it's an x86 BIOS not OF firmware, copied from radeonfb */
    dptr = RADEON_BIOS16(0x18);
    /* If PCI data signature is wrong assume x86 video BIOS anyway */
    if (RADEON_BIOS32(dptr) != (('R' << 24) | ('I' << 16) | ('C' << 8) | 'P')) {
       xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "ROM PCI data signature incorrect, ignoring\n");
    }
    else if (info->VBIOS[dptr + 0x14] != 0x0) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Not an x86 BIOS ROM image, BIOS data will not be used\n");
	xfree (info->VBIOS);
	info->VBIOS = NULL;
	return FALSE;
    }

    if (info->VBIOS) info->ROMHeaderStart = RADEON_BIOS16(0x48);

    if(!info->ROMHeaderStart) {
	xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		   "Invalid ROM pointer, BIOS data will not be used\n");
	xfree (info->VBIOS);
	info->VBIOS = NULL;
	return FALSE;
    }
 
    tmp = info->ROMHeaderStart + 4;
    if ((RADEON_BIOS8(tmp)   == 'A' &&
	 RADEON_BIOS8(tmp+1) == 'T' &&
	 RADEON_BIOS8(tmp+2) == 'O' &&
	 RADEON_BIOS8(tmp+3) == 'M') ||
	(RADEON_BIOS8(tmp)   == 'M' &&
	 RADEON_BIOS8(tmp+1) == 'O' &&
	 RADEON_BIOS8(tmp+2) == 'T' &&
	 RADEON_BIOS8(tmp+3) == 'A'))
	info->IsAtomBios = TRUE;
    else
	info->IsAtomBios = FALSE;

    if (info->IsAtomBios) 
	info->MasterDataStart = RADEON_BIOS16 (info->ROMHeaderStart + 32);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "%s BIOS detected\n",
	       info->IsAtomBios ? "ATOM":"Legacy");

    return TRUE;
}

Bool RADEONGetConnectorInfoFromBIOS (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    RADEONEntPtr pRADEONEnt = RADEONEntPriv(pScrn);
    int i = 0, j, tmp, tmp0=0, tmp1=0;

    if(!info->VBIOS) return FALSE;

    if (info->IsAtomBios) {
	if((tmp = RADEON_BIOS16 (info->MasterDataStart + 22))) {
	    int crtc = 0, id[2];
	    tmp1 = RADEON_BIOS16 (tmp + 4);
	    for (i=0; i<8; i++) {
		if(tmp1 & (1<<i)) {
		    CARD16 portinfo = RADEON_BIOS16(tmp+6+i*2);
		    if (crtc < 2) {
			if ((i==2) || (i==6)) continue; /* ignore TV here */

			if (crtc == 1) {
			    /* sharing same port with id[0] */
			    if (((portinfo>>8) & 0xf) == id[0]) {
				if (i == 3) 
				    pRADEONEnt->PortInfo[0].TMDSType = TMDS_INT;
				else if (i == 7)
				    pRADEONEnt->PortInfo[0].TMDSType = TMDS_EXT;

				if (pRADEONEnt->PortInfo[0].DACType == DAC_UNKNOWN)
				    pRADEONEnt->PortInfo[0].DACType = (portinfo & 0xf) - 1;
				continue;
			    }
			}

			id[crtc] = (portinfo>>8) & 0xf; 
			pRADEONEnt->PortInfo[crtc].DACType = (portinfo & 0xf) - 1;
			pRADEONEnt->PortInfo[crtc].ConnectorType = (portinfo>>4) & 0xf;
			if (i == 3) 
			    pRADEONEnt->PortInfo[crtc].TMDSType = TMDS_INT;
			else if (i == 7)
			    pRADEONEnt->PortInfo[crtc].TMDSType = TMDS_EXT;
			
			if((tmp0 = RADEON_BIOS16 (info->MasterDataStart + 24)) && id[crtc]) {
			    switch (RADEON_BIOS16 (tmp0 + 4 + 27 * id[crtc]) * 4) 
			    {
			    case RADEON_GPIO_MONID:
				pRADEONEnt->PortInfo[crtc].DDCType = DDC_MONID;
				break;
			    case RADEON_GPIO_DVI_DDC:
				pRADEONEnt->PortInfo[crtc].DDCType = DDC_DVI;
				break;
			    case RADEON_GPIO_VGA_DDC:
				pRADEONEnt->PortInfo[crtc].DDCType = DDC_VGA;
				break;
			    case RADEON_GPIO_CRT2_DDC:
				pRADEONEnt->PortInfo[crtc].DDCType = DDC_CRT2;
				break;
			    default:
				pRADEONEnt->PortInfo[crtc].DDCType = DDC_NONE_DETECTED;
				break;
			    }

			} else {
			    pRADEONEnt->PortInfo[crtc].DDCType = DDC_NONE_DETECTED;
			}
			crtc++;
		    } else {
			/* we have already had two CRTCs assigned. the rest may share the same
			 * port with the existing connector, fill in them accordingly.
			 */
			for (j=0; j<2; j++) {
			    if (((portinfo>>8) & 0xf) == id[j]) {
				if (i == 3) 
				    pRADEONEnt->PortInfo[j].TMDSType = TMDS_INT;
				else if (i == 7)
				    pRADEONEnt->PortInfo[j].TMDSType = TMDS_EXT;

				if (pRADEONEnt->PortInfo[j].DACType == DAC_UNKNOWN)
				    pRADEONEnt->PortInfo[j].DACType = (portinfo & 0xf) - 1;
			    }
			}
		    }
		}
	    }

	    for (i=0; i<2; i++) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Port%d: DDCType-%d, DACType-%d, TMDSType-%d, ConnectorType-%d\n",
			   i, pRADEONEnt->PortInfo[i].DDCType, pRADEONEnt->PortInfo[i].DACType,
			   pRADEONEnt->PortInfo[i].TMDSType, pRADEONEnt->PortInfo[i].ConnectorType);
	    }	    
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "No Device Info Table found!\n");
	    return FALSE;
	}
    } else {
	/* Some laptops only have one connector (VGA) listed in the connector table, 
	 * we need to add LVDS in as a non-DDC display. 
	 * Note, we can't assume the listed VGA will be filled in PortInfo[0],
	 * when walking through connector table. connector_found has following meaning: 
	 * 0 -- nothing found, 
	 * 1 -- only PortInfo[0] filled, 
	 * 2 -- only PortInfo[1] filled,
	 * 3 -- both are filled.
	 */
	int connector_found = 0;

	if ((tmp = RADEON_BIOS16(info->ROMHeaderStart + 0x50))) {
	    for (i = 1; i < 4; i++) {

		if (!RADEON_BIOS16(tmp + i*2))
			break; /* end of table */
		
		tmp0 = RADEON_BIOS16(tmp + i*2);
		if (((tmp0 >> 12) & 0x0f) == 0) continue;     /* no connector */
		if (connector_found > 0) {
		    if (pRADEONEnt->PortInfo[tmp1].DDCType == ((tmp0 >> 8) & 0x0f))
			continue;                             /* same connector */
		}

		/* internal DDC_DVI port will get assigned to PortInfo[0], or if there is no DDC_DVI (like in some IGPs). */
		tmp1 = ((((tmp0 >> 8) & 0xf) == DDC_DVI) || (tmp1 == 1)) ? 0 : 1; /* determine port info index */
		
		pRADEONEnt->PortInfo[tmp1].DDCType        = (tmp0 >> 8) & 0x0f;
		if (pRADEONEnt->PortInfo[tmp1].DDCType > DDC_CRT2) pRADEONEnt->PortInfo[tmp1].DDCType = DDC_NONE_DETECTED;
		pRADEONEnt->PortInfo[tmp1].DACType        = (tmp0 & 0x01) ? DAC_TVDAC : DAC_PRIMARY;
		pRADEONEnt->PortInfo[tmp1].ConnectorType  = (tmp0 >> 12) & 0x0f;
		if (pRADEONEnt->PortInfo[tmp1].ConnectorType > CONNECTOR_UNSUPPORTED) pRADEONEnt->PortInfo[tmp1].ConnectorType = CONNECTOR_UNSUPPORTED;
		pRADEONEnt->PortInfo[tmp1].TMDSType       = ((tmp0 >> 4) & 0x01) ? TMDS_EXT : TMDS_INT;

		/* some sanity checks */
		if (((pRADEONEnt->PortInfo[tmp1].ConnectorType != CONNECTOR_DVI_D) &&
		     (pRADEONEnt->PortInfo[tmp1].ConnectorType != CONNECTOR_DVI_I)) &&
		    pRADEONEnt->PortInfo[tmp1].TMDSType == TMDS_INT)
		    pRADEONEnt->PortInfo[tmp1].TMDSType = TMDS_UNKNOWN;
		
		connector_found += (tmp1 + 1);
	    }
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "No Connector Info Table found!\n");
	    return FALSE;
	}

	if (info->IsMobility) {
	    /* For the cases where only one VGA connector is found, 
	       we assume LVDS is not listed in the connector table, 
	       add it in here as the first port.
	    */
	    if ((connector_found < 3) && (pRADEONEnt->PortInfo[tmp1].ConnectorType == CONNECTOR_CRT)) {
		if (connector_found == 1) {
		    memcpy (&pRADEONEnt->PortInfo[1], &pRADEONEnt->PortInfo[0], 
			    sizeof (pRADEONEnt->PortInfo[0]));
		}
		pRADEONEnt->PortInfo[0].DACType = DAC_TVDAC;
		pRADEONEnt->PortInfo[0].TMDSType = TMDS_UNKNOWN;
		pRADEONEnt->PortInfo[0].DDCType = DDC_NONE_DETECTED;
		pRADEONEnt->PortInfo[0].ConnectorType = CONNECTOR_PROPRIETARY;

		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "LVDS port is not in connector table, added in.\n");
		if (connector_found == 0) connector_found = 1;
		else connector_found = 3;
	    }

	    if ((tmp = RADEON_BIOS16(info->ROMHeaderStart + 0x42))) {
	        if ((tmp0 = RADEON_BIOS16(tmp + 0x15))) {
		    if ((tmp1 = RADEON_BIOS8(tmp0+2) & 0x07)) {	    
			pRADEONEnt->PortInfo[0].DDCType	= tmp1;      
			if (pRADEONEnt->PortInfo[0].DDCType > DDC_CRT2) {
			    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
				       "Unknown DDCType %d found\n",
				       pRADEONEnt->PortInfo[0].DDCType);
			    pRADEONEnt->PortInfo[0].DDCType = DDC_NONE_DETECTED;
			}
			xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "LCD DDC Info Table found!\n");
		    }
		}
	    } 
	} else if (connector_found == 2) {
	    memcpy (&pRADEONEnt->PortInfo[0], &pRADEONEnt->PortInfo[1], 
		    sizeof (pRADEONEnt->PortInfo[0]));	
	    pRADEONEnt->PortInfo[1].DACType = DAC_UNKNOWN;
	    pRADEONEnt->PortInfo[1].TMDSType = TMDS_UNKNOWN;
	    pRADEONEnt->PortInfo[1].DDCType = DDC_NONE_DETECTED;
	    pRADEONEnt->PortInfo[1].ConnectorType = CONNECTOR_NONE;
	    connector_found = 1;
	}

	if (connector_found == 0) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, "No connector found in Connector Info Table.\n");
	} else {
	    xf86DrvMsg(0, X_INFO, "Connector0: DDCType-%d, DACType-%d, TMDSType-%d, ConnectorType-%d\n",
		       pRADEONEnt->PortInfo[0].DDCType, pRADEONEnt->PortInfo[0].DACType,
		       pRADEONEnt->PortInfo[0].TMDSType, pRADEONEnt->PortInfo[0].ConnectorType);
	}
	if (connector_found == 3) {
	    xf86DrvMsg(0, X_INFO, "Connector1: DDCType-%d, DACType-%d, TMDSType-%d, ConnectorType-%d\n",
		       pRADEONEnt->PortInfo[1].DDCType, pRADEONEnt->PortInfo[1].DACType,
		       pRADEONEnt->PortInfo[1].TMDSType, pRADEONEnt->PortInfo[1].ConnectorType);
	}

#if 0
/* External TMDS Table, not used now */
        if ((tmp0 = RADEON_BIOS16(info->ROMHeaderStart + 0x58))) {

            //pRADEONEnt->PortInfo[1].DDCType = (RADEON_BIOS8(tmp0 + 7) & 0x07);
            //pRADEONEnt->PortInfo[1].ConnectorType  = CONNECTOR_DVI_I;
            //pRADEONEnt->PortInfo[1].TMDSType = TMDS_EXT;
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "External TMDS found.\n");

        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_INFO, "NO External TMDS Info found\n");

        }
#endif

    }
    return TRUE;
}

/* Read PLL parameters from BIOS block.  Default to typical values if there
   is no BIOS. */
Bool RADEONGetClockInfoFromBIOS (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR (pScrn);
    RADEONPLLPtr pll = &info->pll;
    CARD16 pll_info_block;

    if (!info->VBIOS) {
	return FALSE;
    } else {
	if (info->IsAtomBios) {
	    pll_info_block = RADEON_BIOS16 (info->MasterDataStart + 12);

	    pll->reference_freq = RADEON_BIOS16 (pll_info_block + 82);
	    pll->reference_div = 0; /* Need to derive from existing setting
					or use a new algorithm to calculate
					from min_input and max_input
				     */
	    pll->min_pll_freq = RADEON_BIOS16 (pll_info_block + 78);
	    pll->max_pll_freq = RADEON_BIOS32 (pll_info_block + 32);
	    pll->xclk = RADEON_BIOS16 (pll_info_block + 72);

	    info->sclk = RADEON_BIOS32(pll_info_block + 8) / 100.0;
	    info->mclk = RADEON_BIOS32(pll_info_block + 12) / 100.0;
	    if (info->sclk == 0) info->sclk = 200;
	    if (info->mclk == 0) info->mclk = 200;
		
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ref_freq: %d, min_pll: %ld, max_pll: %ld, xclk: %d, sclk: %f, mclk: %f\n",
		       pll->reference_freq, pll->min_pll_freq, pll->max_pll_freq, pll->xclk, info->sclk, info->mclk);

	} else {
	    pll_info_block = RADEON_BIOS16 (info->ROMHeaderStart + 0x30);

	    pll->reference_freq = RADEON_BIOS16 (pll_info_block + 0x0e);
	    pll->reference_div = RADEON_BIOS16 (pll_info_block + 0x10);
	    pll->min_pll_freq = RADEON_BIOS32 (pll_info_block + 0x12);
	    pll->max_pll_freq = RADEON_BIOS32 (pll_info_block + 0x16);
	    pll->xclk = RADEON_BIOS16 (pll_info_block + 0x08);

	    info->sclk = RADEON_BIOS16(pll_info_block + 8) / 100.0;
	    info->mclk = RADEON_BIOS16(pll_info_block + 10) / 100.0;
	}
    }

    return TRUE;
}

Bool RADEONGetLVDSInfoFromBIOS (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info     = RADEONPTR(pScrn);
    unsigned long tmp, i;

    if (!info->VBIOS) return FALSE;

    if (info->IsAtomBios) {
	if((tmp = RADEON_BIOS16 (info->MasterDataStart + 16))) {

	    info->PanelXRes = RADEON_BIOS16(tmp+6);
	    info->PanelYRes = RADEON_BIOS16(tmp+10);
	    info->DotClock   = RADEON_BIOS16(tmp+4)*10;
	    info->HBlank     = RADEON_BIOS16(tmp+8);
	    info->HOverPlus  = RADEON_BIOS16(tmp+14);
	    info->HSyncWidth = RADEON_BIOS16(tmp+16);
	    info->VBlank     = RADEON_BIOS16(tmp+12);
	    info->VOverPlus  = RADEON_BIOS16(tmp+18);
	    info->VSyncWidth = RADEON_BIOS16(tmp+20);
	    info->PanelPwrDly = RADEON_BIOS16(tmp+40);

	    info->Flags = 0;
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING, 
		       "LVDS Info:\n"
		       "XRes: %d, YRes: %d, DotClock: %d\n"
		       "HBlank: %d, HOverPlus: %d, HSyncWidth: %d\n"
		       "VBlank: %d, VOverPlus: %d, VSyncWidth: %d\n",
		       info->PanelXRes, info->PanelYRes, info->DotClock,
		       info->HBlank,info->HOverPlus, info->HSyncWidth,
		       info->VBlank, info->VOverPlus, info->VSyncWidth);
	} else {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "No LVDS Info Table found in BIOS!\n");
	    return FALSE;
	}
    } else {

	tmp = RADEON_BIOS16(info->ROMHeaderStart + 0x40);

	if (!tmp) {
	    xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
		       "No Panel Info Table found in BIOS!\n");
	    return FALSE;
	} else {
	    char  stmp[30];
	    int   tmp0;

	    for (i = 0; i < 24; i++)
	    stmp[i] = RADEON_BIOS8(tmp+i+1);
	    stmp[24] = 0;

	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "Panel ID string: %s\n", stmp);

	    info->PanelXRes = RADEON_BIOS16(tmp+25);
	    info->PanelYRes = RADEON_BIOS16(tmp+27);
	    xf86DrvMsg(0, X_INFO, "Panel Size from BIOS: %dx%d\n",
		       info->PanelXRes, info->PanelYRes);
	
	    info->PanelPwrDly = RADEON_BIOS16(tmp+44);
	    if (info->PanelPwrDly > 2000 || info->PanelPwrDly < 0)
		info->PanelPwrDly = 2000;

	    /* some panels only work well with certain divider combinations.
	     */
	    info->RefDivider = RADEON_BIOS16(tmp+46);
	    info->PostDivider = RADEON_BIOS8(tmp+48);
	    info->FeedbackDivider = RADEON_BIOS16(tmp+49);
	    if ((info->RefDivider != 0) &&
		(info->FeedbackDivider > 3)) {
		info->UseBiosDividers = TRUE;
		xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			   "BIOS provided dividers will be used.\n");
	    }

	    /* We don't use a while loop here just in case we have a corrupted BIOS image.
	       The max number of table entries is 23 at present, but may grow in future.
	       To ensure it works with future revisions we loop it to 32.
	    */
	    for (i = 0; i < 32; i++) {
		tmp0 = RADEON_BIOS16(tmp+64+i*2);
		if (tmp0 == 0) break;
		if ((RADEON_BIOS16(tmp0) == info->PanelXRes) &&
		    (RADEON_BIOS16(tmp0+2) == info->PanelYRes)) {
		    info->HBlank     = (RADEON_BIOS16(tmp0+17) -
					RADEON_BIOS16(tmp0+19)) * 8;
		    info->HOverPlus  = (RADEON_BIOS16(tmp0+21) -
					RADEON_BIOS16(tmp0+19) - 1) * 8;
		    info->HSyncWidth = RADEON_BIOS8(tmp0+23) * 8;
		    info->VBlank     = (RADEON_BIOS16(tmp0+24) -
					RADEON_BIOS16(tmp0+26));
		    info->VOverPlus  = ((RADEON_BIOS16(tmp0+28) & 0x7ff) -
					RADEON_BIOS16(tmp0+26));
		    info->VSyncWidth = ((RADEON_BIOS16(tmp0+28) & 0xf800) >> 11);
		    info->DotClock   = RADEON_BIOS16(tmp0+9) * 10;
		    info->Flags = 0;
		}
	    }
	}
    }
    return TRUE;
}

Bool RADEONGetHardCodedEDIDFromBIOS (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info     = RADEONPTR(pScrn);
    unsigned long tmp;
    char EDID[256];

    if (!info->VBIOS) return FALSE;

    if (info->IsAtomBios) {
	/* Not yet */
	return FALSE;
    } else {
	if (!(tmp = RADEON_BIOS16(info->ROMHeaderStart + 0x4c))) {
	    return FALSE;
	}

	memcpy(EDID, (char*)(info->VBIOS + tmp), 256);

	info->DotClock = (*(CARD16*)(EDID+54)) * 10;
	info->PanelXRes = (*(CARD8*)(EDID+56)) + ((*(CARD8*)(EDID+58))>>4)*256;
	info->HBlank = (*(CARD8*)(EDID+57)) + ((*(CARD8*)(EDID+58)) & 0xf)*256;
	info->HOverPlus = (*(CARD8*)(EDID+62)) + ((*(CARD8*)(EDID+65)>>6)*256);
	info->HSyncWidth = (*(CARD8*)(EDID+63)) + (((*(CARD8*)(EDID+65)>>4) & 3)*256);
	info->PanelYRes = (*(CARD8*)(EDID+59)) + ((*(CARD8*)(EDID+61))>>4)*256;
	info->VBlank = ((*(CARD8*)(EDID+60)) + ((*(CARD8*)(EDID+61)) & 0xf)*256);
	info->VOverPlus = (((*(CARD8*)(EDID+64))>>4) + (((*(CARD8*)(EDID+65)>>2) & 3)*16));
	info->VSyncWidth = (((*(CARD8*)(EDID+64)) & 0xf) + ((*(CARD8*)(EDID+65)) & 3)*256);
	info->Flags      = V_NHSYNC | V_NVSYNC; /**(CARD8*)(EDID+71);*/
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Hardcoded EDID data will be used for TMDS panel\n");
    }
    return TRUE;
}

Bool RADEONGetTMDSInfoFromBIOS (ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info     = RADEONPTR(pScrn);
    CARD32 tmp, maxfreq;
    int i, n;

    if (!info->VBIOS) return FALSE;

    if (info->IsAtomBios) {
	if((tmp = RADEON_BIOS16 (info->MasterDataStart + 18))) {

	    maxfreq = RADEON_BIOS16(tmp+4);
	    
	    for (i=0; i<4; i++) {
		info->tmds_pll[i].freq = RADEON_BIOS16(tmp+i*6+6);
		/* This assumes each field in TMDS_PLL has 6 bit as in R300/R420 */
		info->tmds_pll[i].value = ((RADEON_BIOS8(tmp+i*6+8) & 0x3f) |
					   ((RADEON_BIOS8(tmp+i*6+10) & 0x3f)<<6) |
					   ((RADEON_BIOS8(tmp+i*6+9) & 0xf)<<12) |
					   ((RADEON_BIOS8(tmp+i*6+11) & 0xf)<<16));
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
			   "TMDS PLL from BIOS: %ld %lx\n", 
			   info->tmds_pll[i].freq, info->tmds_pll[i].value);
		       
		if (maxfreq == info->tmds_pll[i].freq) {
		    info->tmds_pll[i].freq = 0xffffffff;
		    break;
		}
	    }
	    return TRUE;
	}
    } else {

	tmp = RADEON_BIOS16(info->ROMHeaderStart + 0x34);
	if (tmp) {
	    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		       "DFP table revision: %d\n", RADEON_BIOS8(tmp));
	    if (RADEON_BIOS8(tmp) == 3) {
		n = RADEON_BIOS8(tmp + 5) + 1;
		if (n > 4) n = 4;
		for (i=0; i<n; i++) {
		    info->tmds_pll[i].value = RADEON_BIOS32(tmp+i*10+0x08);
		    info->tmds_pll[i].freq = RADEON_BIOS16(tmp+i*10+0x10);
		}
		return TRUE;
	    } else if (RADEON_BIOS8(tmp) == 4) {
	        int stride = 0;
		n = RADEON_BIOS8(tmp + 5) + 1;
		if (n > 4) n = 4;
		for (i=0; i<n; i++) {
		    info->tmds_pll[i].value = RADEON_BIOS32(tmp+stride+0x08);
		    info->tmds_pll[i].freq = RADEON_BIOS16(tmp+stride+0x10);
		    if (i == 0) stride += 10;
		    else stride += 6;
		}
		return TRUE;
	    }

	    /* revision 4 has some problem as it appears in RV280, 
	       comment it off for now, use default instead */ 
	    /*    
		  else if (RADEON_BIOS8(tmp) == 4) {
		  int stride = 0;
		  n = RADEON_BIOS8(tmp + 5) + 1;
		  if (n > 4) n = 4;
		  for (i=0; i<n; i++) {
		  info->tmds_pll[i].value = RADEON_BIOS32(tmp+stride+0x08);
		  info->tmds_pll[i].freq = RADEON_BIOS16(tmp+stride+0x10);
		  if (i == 0) stride += 10;
		  else stride += 6;
		  }
		  return TRUE;
		  }
	    */  
	}
    }
    return FALSE;
}
