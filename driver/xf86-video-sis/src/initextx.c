/* $XFree86$ */
/* $XdotOrg$ */
/*
 * X.org/XFree86 specific extensions to init.c/init301.c
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
 * Author: 	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "initextx.h"

static void
SiS_MakeClockRegs(ScrnInfoPtr pScrn, int clock, unsigned char *p2b, unsigned char *p2c)
{
   int          out_n, out_dn, out_div, out_sbit, out_scale;
   unsigned int vclk[5];

#define Midx         0
#define Nidx         1
#define VLDidx       2
#define Pidx         3
#define PSNidx       4

   if(SiS_compute_vclk(clock, &out_n, &out_dn, &out_div, &out_sbit, &out_scale)) {
      (*p2b) = (out_div == 2) ? 0x80 : 0x00;
      (*p2b) |= ((out_n - 1) & 0x7f);
      (*p2c) = (out_dn - 1) & 0x1f;
      (*p2c) |= (((out_scale - 1) & 3) << 5);
      (*p2c) |= ((out_sbit & 0x01) << 7);
#ifdef TWDEBUG
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Clock %d: n %d dn %d div %d sb %d sc %d\n",
        	 clock, out_n, out_dn, out_div, out_sbit, out_scale);
#endif
   } else {
      SiSCalcClock(pScrn, clock, 2, vclk);
      (*p2b) = (vclk[VLDidx] == 2) ? 0x80 : 0x00;
      (*p2b) |= (vclk[Midx] - 1) & 0x7f;
      (*p2c) = (vclk[Nidx] - 1) & 0x1f;
      if(vclk[Pidx] <= 4) {
	 /* postscale 1,2,3,4 */
	 (*p2c) |= ((vclk[Pidx] - 1) & 3) << 5;
      } else {
	 /* postscale 6,8 */
	 (*p2c) |= (((vclk[Pidx] / 2) - 1) & 3) << 5;
	 (*p2c) |= 0x80;
      }
#ifdef TWDEBUG
      xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Clock %d: n %d dn %d div %d sc %d\n",
		clock, vclk[Midx], vclk[Nidx], vclk[VLDidx], vclk[Pidx]);
#endif
   }
}

unsigned short
SiS_CheckBuildCustomMode(ScrnInfoPtr pScrn, DisplayModePtr mode, unsigned int VBFlags)
{
   SISPtr pSiS = SISPTR(pScrn);
   int    depth = pSiS->CurrentLayout.bitsPerPixel;

   pSiS->SiS_Pr->CModeFlag = 0;

   pSiS->SiS_Pr->CDClock = mode->Clock;

   pSiS->SiS_Pr->CHDisplay = mode->HDisplay;
   pSiS->SiS_Pr->CHSyncStart = mode->HSyncStart;
   pSiS->SiS_Pr->CHSyncEnd = mode->HSyncEnd;
   pSiS->SiS_Pr->CHTotal = mode->HTotal;

   pSiS->SiS_Pr->CVDisplay = mode->VDisplay;
   pSiS->SiS_Pr->CVSyncStart = mode->VSyncStart;
   pSiS->SiS_Pr->CVSyncEnd = mode->VSyncEnd;
   pSiS->SiS_Pr->CVTotal = mode->VTotal;

   pSiS->SiS_Pr->CFlags = mode->Flags;

   if(pSiS->SiS_Pr->CFlags & V_INTERLACE) {
      pSiS->SiS_Pr->CVDisplay >>= 1;
      pSiS->SiS_Pr->CVSyncStart >>= 1;
      pSiS->SiS_Pr->CVSyncEnd >>= 1;
      pSiS->SiS_Pr->CVTotal >>= 1;
   } else if(pSiS->SiS_Pr->CFlags & V_DBLSCAN) {
      pSiS->SiS_Pr->CVDisplay <<= 1;
      pSiS->SiS_Pr->CVSyncStart <<= 1;
      pSiS->SiS_Pr->CVSyncEnd <<= 1;
      pSiS->SiS_Pr->CVTotal <<= 1;
   }

   pSiS->SiS_Pr->CHBlankStart = pSiS->SiS_Pr->CHDisplay;
   pSiS->SiS_Pr->CHBlankEnd = pSiS->SiS_Pr->CHTotal;
   pSiS->SiS_Pr->CVBlankStart = pSiS->SiS_Pr->CVSyncStart - 1;
   pSiS->SiS_Pr->CVBlankEnd = pSiS->SiS_Pr->CVTotal;

   if((!(mode->type & M_T_BUILTIN)) && (mode->HDisplay <= 512)) {
      pSiS->SiS_Pr->CModeFlag |= HalfDCLK;
      pSiS->SiS_Pr->CDClock <<= 1;
   }

   /* Note: For CRT2, HDisplay, HSync* and HTotal must be shifted left
    * in HalfDCLK mode.
    */

   SiS_MakeClockRegs(pScrn, pSiS->SiS_Pr->CDClock, &pSiS->SiS_Pr->CSR2B, &pSiS->SiS_Pr->CSR2C);

   pSiS->SiS_Pr->CSRClock = (pSiS->SiS_Pr->CDClock / 1000) + 1;

   SiS_CalcCRRegisters(pSiS->SiS_Pr, depth);

   switch(depth) {
   case  8: pSiS->SiS_Pr->CModeFlag |= 0x223b; break;
   case 16: pSiS->SiS_Pr->CModeFlag |= 0x227d; break;
   case 32: pSiS->SiS_Pr->CModeFlag |= 0x22ff; break;
   default: return 0;
   }

   if(pSiS->SiS_Pr->CFlags & V_DBLSCAN)
      pSiS->SiS_Pr->CModeFlag |= DoubleScanMode;

   if((pSiS->SiS_Pr->CVDisplay >= 1024)	||
      (pSiS->SiS_Pr->CVTotal >= 1024)   ||
      (pSiS->SiS_Pr->CHDisplay >= 1024))
      pSiS->SiS_Pr->CModeFlag |= LineCompareOff;

   pSiS->SiS_Pr->CInfoFlag = 0x0007;

   if(pSiS->SiS_Pr->CFlags & V_NHSYNC)
      pSiS->SiS_Pr->CInfoFlag |= 0x4000;

   if(pSiS->SiS_Pr->CFlags & V_NVSYNC)
      pSiS->SiS_Pr->CInfoFlag |= 0x8000;

   if(pSiS->SiS_Pr->CFlags & V_INTERLACE)
      pSiS->SiS_Pr->CInfoFlag |= InterlaceMode;

   pSiS->SiS_Pr->UseCustomMode = TRUE;
#ifdef TWDEBUG
   xf86DrvMsg(0, X_INFO, "Custom mode %dx%d:\n",
	pSiS->SiS_Pr->CHDisplay,pSiS->SiS_Pr->CVDisplay);
   xf86DrvMsg(0, X_INFO, "Modeflag %04x, Infoflag %04x\n",
	pSiS->SiS_Pr->CModeFlag, pSiS->SiS_Pr->CInfoFlag);
   xf86DrvMsg(0, X_INFO, " {{0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,\n",
	pSiS->SiS_Pr->CCRT1CRTC[0], pSiS->SiS_Pr->CCRT1CRTC[1],
	pSiS->SiS_Pr->CCRT1CRTC[2], pSiS->SiS_Pr->CCRT1CRTC[3],
	pSiS->SiS_Pr->CCRT1CRTC[4], pSiS->SiS_Pr->CCRT1CRTC[5],
	pSiS->SiS_Pr->CCRT1CRTC[6], pSiS->SiS_Pr->CCRT1CRTC[7]);
   xf86DrvMsg(0, X_INFO, "  0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,\n",
	pSiS->SiS_Pr->CCRT1CRTC[8], pSiS->SiS_Pr->CCRT1CRTC[9],
	pSiS->SiS_Pr->CCRT1CRTC[10], pSiS->SiS_Pr->CCRT1CRTC[11],
	pSiS->SiS_Pr->CCRT1CRTC[12], pSiS->SiS_Pr->CCRT1CRTC[13],
	pSiS->SiS_Pr->CCRT1CRTC[14], pSiS->SiS_Pr->CCRT1CRTC[15]);
   xf86DrvMsg(0, X_INFO, "  0x%02x}},\n", pSiS->SiS_Pr->CCRT1CRTC[16]);
   xf86DrvMsg(0, X_INFO, "Clock: 0x%02x, 0x%02x, %d\n",
	pSiS->SiS_Pr->CSR2B, pSiS->SiS_Pr->CSR2C, pSiS->SiS_Pr->CSRClock);
#endif
   return 1;
}

/* Build a list of supported modes:
 * Built-in modes for which we have all data are M_T_DEFAULT,
 * modes derived from DDC or database data are M_T_BUILTIN
 */
DisplayModePtr
SiSBuildBuiltInModeList(ScrnInfoPtr pScrn, BOOLEAN includelcdmodes, BOOLEAN isfordvi,
			BOOLEAN fakecrt2modes, BOOLEAN IsForCRT2)
{
   SISPtr	  pSiS = SISPTR(pScrn);
   unsigned char  sr2b, sr2c;
   float	  num, denum, postscalar, divider;
   int		  i, j, k, l, index, vclkindex, UseWide;
   DisplayModePtr new = NULL, current = NULL, first = NULL;
   BOOLEAN	  done = FALSE, IsHDCLK;
#if 0
   DisplayModePtr backup = NULL;
#endif

   pSiS->backupmodelist = NULL;
   pSiS->AddedPlasmaModes = FALSE;

   UseWide = pSiS->SiS_Pr->SiS_UseWide;
   if(IsForCRT2) UseWide = pSiS->SiS_Pr->SiS_UseWideCRT2;

   if(!SiSInitPtr(pSiS->SiS_Pr)) return NULL;

   i = 0;
   while(pSiS->SiS_Pr->SiS_RefIndex[i].Ext_InfoFlag != 0xFFFF) {

      if(pSiS->SiS_Pr->SiS_RefIndex[i].Ext_InfoFlag & HaveWideTiming) {
	 if(UseWide == 1) {
	    if((pSiS->SiS_Pr->SiS_RefIndex[i].Ext_CRT1CRTC_WIDE == 0xff) &&
	       (pSiS->SiS_Pr->SiS_RefIndex[i].Ext_CRTVCLK_WIDE == 0xff)) {
	       i++;
	       continue;
	    }
	 } else {
	    if((pSiS->SiS_Pr->SiS_RefIndex[i].Ext_CRT1CRTC_NORM == 0xff) &&
	       (pSiS->SiS_Pr->SiS_RefIndex[i].Ext_CRTVCLK_NORM == 0xff)) {
	       i++;
	       continue;
	    }
	 }
      }

      index = SiS_GetRefCRT1CRTC(pSiS->SiS_Pr, i, UseWide);
      if(fakecrt2modes) {
	 if(pSiS->SiS_Pr->SiS_RefIndex[i].Ext_FakeCRT2CRTC) {
	    index = pSiS->SiS_Pr->SiS_RefIndex[i].Ext_FakeCRT2CRTC;
	 }
      }

      /* 0x5a (320x240) for FTSN - skip, is bad for CRT1 */
      if(pSiS->SiS_Pr->SiS_RefIndex[i].ModeID == 0x5a)  {
	 i++;
	 continue;
      }

      if(!(new = xalloc(sizeof(DisplayModeRec)))) return first;
      memset(new, 0, sizeof(DisplayModeRec));
      if(!(new->name = xalloc(10))) {
	 xfree(new);
	 return first;
      }
      if(!first) first = new;
      if(current) {
	 current->next = new;
	 new->prev = current;
      }

      current = new;

      sprintf(current->name, "%dx%d", pSiS->SiS_Pr->SiS_RefIndex[i].XRes,
				      pSiS->SiS_Pr->SiS_RefIndex[i].YRes);

      current->status = MODE_OK;

      current->type = M_T_DEFAULT;

      vclkindex = SiS_GetRefCRTVCLK(pSiS->SiS_Pr, i, UseWide);
      if(fakecrt2modes) {
	 if(pSiS->SiS_Pr->SiS_RefIndex[i].Ext_FakeCRT2Clk) {
	    vclkindex = pSiS->SiS_Pr->SiS_RefIndex[i].Ext_FakeCRT2Clk;
	 }
      }

      sr2b = pSiS->SiS_Pr->SiS_VCLKData[vclkindex].SR2B;
      sr2c = pSiS->SiS_Pr->SiS_VCLKData[vclkindex].SR2C;

      divider = (sr2b & 0x80) ? 2.0 : 1.0;
      postscalar = (sr2c & 0x80) ?
              ( (((sr2c >> 5) & 0x03) == 0x02) ? 6.0 : 8.0) : (((sr2c >> 5) & 0x03) + 1.0);
      num = (sr2b & 0x7f) + 1.0;
      denum = (sr2c & 0x1f) + 1.0;

#ifdef TWDEBUG
      xf86DrvMsg(0, X_INFO, "------------\n");
      xf86DrvMsg(0, X_INFO, "sr2b: %x sr2c %x div %f ps %f num %f denum %f\n",
         sr2b, sr2c, divider, postscalar, num, denum);
#endif

      current->Clock = (int)(14318 * (divider / postscalar) * (num / denum));

      SiS_Generic_ConvertCRData(pSiS->SiS_Pr,
			(unsigned char *)&pSiS->SiS_Pr->SiS_CRT1Table[index].CR[0],
			pSiS->SiS_Pr->SiS_RefIndex[i].XRes,
			pSiS->SiS_Pr->SiS_RefIndex[i].YRes, current);

      if(pSiS->SiS_Pr->SiS_RefIndex[i].Ext_InfoFlag & 0x4000)
	  current->Flags |= V_NHSYNC;
      else
	  current->Flags |= V_PHSYNC;

      if(pSiS->SiS_Pr->SiS_RefIndex[i].Ext_InfoFlag & 0x8000)
	  current->Flags |= V_NVSYNC;
      else
	  current->Flags |= V_PVSYNC;

      if(pSiS->SiS_Pr->SiS_RefIndex[i].Ext_InfoFlag & 0x0080)
          current->Flags |= V_INTERLACE;

      j = 0;
      IsHDCLK = FALSE;
      while(pSiS->SiS_Pr->SiS_EModeIDTable[j].Ext_ModeID != 0xff) {
	  if(pSiS->SiS_Pr->SiS_EModeIDTable[j].Ext_ModeID ==
				pSiS->SiS_Pr->SiS_RefIndex[i].ModeID) {
	     if(pSiS->SiS_Pr->SiS_EModeIDTable[j].Ext_ModeFlag & DoubleScanMode) {
		  current->Flags |= V_DBLSCAN;
	      }
	      if(pSiS->SiS_Pr->SiS_EModeIDTable[j].Ext_ModeFlag & HalfDCLK) {
		  IsHDCLK = TRUE;
	      }
	      break;
	  }
	  j++;
      }

      if(current->Flags & V_INTERLACE) {
	 current->VDisplay <<= 1;
	 current->VSyncStart <<= 1;
	 current->VSyncEnd <<= 1;
	 current->VTotal <<= 1;
	 current->VTotal |= 1;
      }

      if(IsHDCLK) {
	 current->Clock >>= 1;
      }

      if(current->Flags & V_DBLSCAN) {
	 current->VDisplay >>= 1;
	 current->VSyncStart >>= 1;
	 current->VSyncEnd >>= 1;
	 current->VTotal >>= 1;
      }

#ifdef TWDEBUG
      xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	"Built-in: %s %.2f %d %d %d %d %d %d %d %d\n",
	current->name, (float)current->Clock / 1000,
	current->HDisplay, current->HSyncStart, current->HSyncEnd, current->HTotal,
	current->VDisplay, current->VSyncStart, current->VSyncEnd, current->VTotal);
#endif

      i++;
   }

   /* Add non-standard LCD modes for panel's detailed timings */

   if(!includelcdmodes) return first;

   if(pSiS->SiS_Pr->CP_Vendor) {
      xf86DrvMsg(0, X_INFO, "Checking database for vendor %x, product %x\n",
         pSiS->SiS_Pr->CP_Vendor, pSiS->SiS_Pr->CP_Product);
   }

   i = 0;
   while((!done) && (SiS_PlasmaTable[i].vendor) && (pSiS->SiS_Pr->CP_Vendor)) {

     if(SiS_PlasmaTable[i].vendor == pSiS->SiS_Pr->CP_Vendor) {

	for(j=0; j<SiS_PlasmaTable[i].productnum; j++) {

	    if(SiS_PlasmaTable[i].product[j] == pSiS->SiS_Pr->CP_Product) {

	       xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
		  "Identified %s panel, adding specific modes\n",
		  SiS_PlasmaTable[i].plasmaname);

	       for(k=0; k<SiS_PlasmaTable[i].modenum; k++) {

		  if(isfordvi) {
		     if(!(SiS_PlasmaTable[i].plasmamodes[k] & 0x80)) continue;
		  } else {
		     if(!(SiS_PlasmaTable[i].plasmamodes[k] & 0x40)) continue;
		  }

		  l = SiS_PlasmaTable[i].plasmamodes[k] & 0x3f;

		  if(!(pSiS->VBFlags2 & VB2_LCDOVER1280BRIDGE)) {
		     if(isfordvi) {
		        if(SiS_PlasmaMode[l].VDisplay > 1024) continue;
		     }
		  }

		  if(!(new = xalloc(sizeof(DisplayModeRec)))) return first;

		  memset(new, 0, sizeof(DisplayModeRec));
		  if(!(new->name = xalloc(12))) {
		     xfree(new);
		     return first;
		  }
		  if(!first) first = new;
		  if(current) {
		     current->next = new;
		     new->prev = current;
		  }

		  current = new;

		  pSiS->AddedPlasmaModes = TRUE;

		  strcpy(current->name, SiS_PlasmaMode[l].name);

		  current->status = MODE_OK;

		  current->type = M_T_BUILTIN;

		  current->Clock = SiS_PlasmaMode[l].clock;
		  current->SynthClock = current->Clock;

		  current->HDisplay   = SiS_PlasmaMode[l].HDisplay;
		  current->HSyncStart = current->HDisplay + SiS_PlasmaMode[l].HFrontPorch;
		  current->HSyncEnd   = current->HSyncStart + SiS_PlasmaMode[l].HSyncWidth;
		  current->HTotal     = SiS_PlasmaMode[l].HTotal;

		  current->VDisplay   = SiS_PlasmaMode[l].VDisplay;
		  current->VSyncStart = current->VDisplay + SiS_PlasmaMode[l].VFrontPorch;
		  current->VSyncEnd   = current->VSyncStart + SiS_PlasmaMode[l].VSyncWidth;
		  current->VTotal     = SiS_PlasmaMode[l].VTotal;

		  current->CrtcHDisplay = current->HDisplay;
		  current->CrtcHBlankStart = current->HSyncStart;
		  current->CrtcHSyncStart = current->HSyncStart;
		  current->CrtcHSyncEnd = current->HSyncEnd;
		  current->CrtcHBlankEnd = current->HSyncEnd;
		  current->CrtcHTotal = current->HTotal;

		  current->CrtcVDisplay = current->VDisplay;
		  current->CrtcVBlankStart = current->VSyncStart;
		  current->CrtcVSyncStart = current->VSyncStart;
		  current->CrtcVSyncEnd = current->VSyncEnd;
		  current->CrtcVBlankEnd = current->VSyncEnd;
		  current->CrtcVTotal = current->VTotal;

		  if(SiS_PlasmaMode[l].SyncFlags & SIS_PL_HSYNCP)
		     current->Flags |= V_PHSYNC;
		  else
		     current->Flags |= V_NHSYNC;

		  if(SiS_PlasmaMode[l].SyncFlags & SIS_PL_VSYNCP)
		     current->Flags |= V_PVSYNC;
		  else
		     current->Flags |= V_NVSYNC;

		  if(current->HDisplay > pSiS->LCDwidth)
		     pSiS->LCDwidth = pSiS->SiS_Pr->CP_MaxX = current->HDisplay;
		  if(current->VDisplay > pSiS->LCDheight)
		     pSiS->LCDheight = pSiS->SiS_Pr->CP_MaxY = current->VDisplay;

		  xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			"\tAdding \"%s\" to list of built-in modes\n", current->name);

	       }
	       done = TRUE;
	       break;
	    }
	}
     }

     i++;

   }

   if(pSiS->SiS_Pr->CP_HaveCustomData) {

      for(i=0; i<7; i++) {

	 if(pSiS->SiS_Pr->CP_DataValid[i]) {

	    if(!(new = xalloc(sizeof(DisplayModeRec)))) return first;

	    memset(new, 0, sizeof(DisplayModeRec));
	    if(!(new->name = xalloc(10))) {
	       xfree(new);
	       return first;
	    }
	    if(!first) first = new;
	    if(current) {
	       current->next = new;
	       new->prev = current;
	    }

	    current = new;

	    sprintf(current->name, "%dx%d", pSiS->SiS_Pr->CP_HDisplay[i],
				pSiS->SiS_Pr->CP_VDisplay[i]);

	    current->status = MODE_OK;

	    current->type = M_T_BUILTIN;

	    current->Clock = pSiS->SiS_Pr->CP_Clock[i];
	    current->SynthClock = current->Clock;

	    current->HDisplay   = pSiS->SiS_Pr->CP_HDisplay[i];
	    current->HSyncStart = pSiS->SiS_Pr->CP_HSyncStart[i];
	    current->HSyncEnd   = pSiS->SiS_Pr->CP_HSyncEnd[i];
	    current->HTotal     = pSiS->SiS_Pr->CP_HTotal[i];

	    current->VDisplay   = pSiS->SiS_Pr->CP_VDisplay[i];
	    current->VSyncStart = pSiS->SiS_Pr->CP_VSyncStart[i];
	    current->VSyncEnd   = pSiS->SiS_Pr->CP_VSyncEnd[i];
	    current->VTotal     = pSiS->SiS_Pr->CP_VTotal[i];

	    current->CrtcHDisplay = current->HDisplay;
	    current->CrtcHBlankStart = pSiS->SiS_Pr->CP_HBlankStart[i];
	    current->CrtcHSyncStart = current->HSyncStart;
	    current->CrtcHSyncEnd = current->HSyncEnd;
	    current->CrtcHBlankEnd = pSiS->SiS_Pr->CP_HBlankEnd[i];
	    current->CrtcHTotal = current->HTotal;

	    current->CrtcVDisplay = current->VDisplay;
	    current->CrtcVBlankStart = pSiS->SiS_Pr->CP_VBlankStart[i];
	    current->CrtcVSyncStart = current->VSyncStart;
	    current->CrtcVSyncEnd = current->VSyncEnd;
	    current->CrtcVBlankEnd = pSiS->SiS_Pr->CP_VBlankEnd[i];
	    current->CrtcVTotal = current->VTotal;

	    if(pSiS->SiS_Pr->CP_SyncValid[i]) {
	       if(pSiS->SiS_Pr->CP_HSync_P[i])
		  current->Flags |= V_PHSYNC;
	       else
		  current->Flags |= V_NHSYNC;

	       if(pSiS->SiS_Pr->CP_VSync_P[i])
		  current->Flags |= V_PVSYNC;
	       else
		  current->Flags |= V_NVSYNC;
	    } else {
	       /* No sync data? Use positive sync... */
	       current->Flags |= V_PHSYNC;
	       current->Flags |= V_PVSYNC;
	    }
	 }
      }
   }

   return first;

}

/* Translate a mode number into the VESA pendant */
int
SiSTranslateToVESA(ScrnInfoPtr pScrn, int modenumber)
{
   SISPtr pSiS = SISPTR(pScrn);
   int    i = 0;

   if(!SiSInitPtr(pSiS->SiS_Pr)) return -1;

   if(modenumber <= 0x13) return modenumber;

#ifdef SIS315H
   if(pSiS->ROM661New) { /* Not XGI! */
      while(SiS_EModeIDTable661[i].Ext_ModeID != 0xff) {
	 if(SiS_EModeIDTable661[i].Ext_ModeID == modenumber) {
	    return (int)SiS_EModeIDTable661[i].Ext_VESAID;
	 }
	 i++;
      }
   } else {
#endif
      while(pSiS->SiS_Pr->SiS_EModeIDTable[i].Ext_ModeID != 0xff) {
	 if(pSiS->SiS_Pr->SiS_EModeIDTable[i].Ext_ModeID == modenumber) {
	    return (int)pSiS->SiS_Pr->SiS_EModeIDTable[i].Ext_VESAID;
	 }
	 i++;
      }
#ifdef SIS315H
   }
#endif
   return -1;
}

/* Translate a new (SiS or XGI) BIOS mode number into the driver's pendant */
int
SiSTranslateToOldMode(int modenumber)
{
#ifdef SIS315H
   int    i = 0;

   while(SiS_EModeIDTable661[i].Ext_ModeID != 0xff) {
      if(SiS_EModeIDTable661[i].Ext_ModeID == modenumber) {
	 if(SiS_EModeIDTable661[i].Ext_MyModeID)
	    return (int)SiS_EModeIDTable661[i].Ext_MyModeID;
	 else
	    return modenumber;
      }
      i++;
   }
#endif
   return modenumber;
}

BOOLEAN
SiS_GetPanelID(struct SiS_Private *SiS_Pr)
{
  unsigned short tempax, tempbx, temp;
  static const unsigned short PanelTypeTable300[16] = {
      0xc101, 0xc117, 0x0121, 0xc135, 0xc142, 0xc152, 0xc162, 0xc072,
      0xc181, 0xc192, 0xc1a1, 0xc1b6, 0xc1c2, 0xc0d2, 0xc1e2, 0xc1f2
  };
  static const unsigned short PanelTypeTable31030x[16] = {
      0xc102, 0xc112, 0x0122, 0xc132, 0xc142, 0xc152, 0xc169, 0xc179,
      0x0189, 0xc192, 0xc1a2, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
  };
  static const unsigned short PanelTypeTable310LVDS[16] = {
      0xc111, 0xc122, 0xc133, 0xc144, 0xc155, 0xc166, 0xc177, 0xc188,
      0xc199, 0xc0aa, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
  };

  if(SiS_Pr->ChipType < SIS_315H) {

     tempbx = SiS_GetReg(SiS_Pr->SiS_P3c4,0x18);
     if(!(tempbx & 0x10)) {
	if(SiS_Pr->SiS_IF_DEF_LVDS == 1) {
	   tempbx = 0;
	   temp = SiS_GetReg(SiS_Pr->SiS_P3c4,0x38);
	   if(temp & 0x40) tempbx |= 0x08;
	   if(temp & 0x20) tempbx |= 0x02;
	   if(temp & 0x01) tempbx |= 0x01;
	   temp = SiS_GetReg(SiS_Pr->SiS_P3c4,0x39);
	   if(temp & 0x80) tempbx |= 0x04;
	} else {
	   return FALSE;
	}
     }
     tempbx = PanelTypeTable300[(tempbx & 0x0f)] | LCDSync;
     SiS_SetReg(SiS_Pr->SiS_P3d4,0x36,tempbx);
     SiS_SetRegANDOR(SiS_Pr->SiS_P3d4,0x37,~(LCDSyncBit|LCDRGB18Bit),(tempbx >> 8));

  } else {

     if(SiS_Pr->ChipType >= SIS_661) return FALSE;

     tempax = (SiS_GetReg(SiS_Pr->SiS_P3c4,0x1a) & 0x1e) >> 1;
     if(SiS_Pr->SiS_IF_DEF_LVDS == 1) {
	if(tempax == 0) return FALSE;
	tempbx = PanelTypeTable310LVDS[tempax - 1];
	temp = tempax & 0xff;
     } else {
	tempbx = PanelTypeTable31030x[tempax];
	temp = tempbx & 0xff;
     }
     SiS_SetReg(SiS_Pr->SiS_P3d4,0x36,temp);
     tempbx >>= 8;
     SiS_SetRegANDOR(SiS_Pr->SiS_P3d4,0x37,~(LCDSyncBit|LCDRGB18Bit),(tempbx & 0xc1));
     if(SiS_Pr->SiS_VBType & VB_SISVB) {
	SiS_SetRegANDOR(SiS_Pr->SiS_P3d4,0x39,0xfb,(tempbx & 0x04));
     }

  }
  return TRUE;
}

/* LCD and VGA2 detection */

static BOOLEAN
checkedid1(unsigned char *buffer)
{
   /* Check header */
   if((buffer[0] != 0x00) ||
      (buffer[1] != 0xff) ||
      (buffer[2] != 0xff) ||
      (buffer[3] != 0xff) ||
      (buffer[4] != 0xff) ||
      (buffer[5] != 0xff) ||
      (buffer[6] != 0xff) ||
      (buffer[7] != 0x00))
      return FALSE;

   /* Check EDID version and revision */
   if((buffer[0x12] != 1) || (buffer[0x13] > 4)) return FALSE;

   /* Check week of manufacture for sanity */
   if(buffer[0x10] > 54) return FALSE;

   /* Check year of manufacture for sanity */
   if(buffer[0x11] > 40) return FALSE;

   return TRUE;
}

static BOOLEAN
checkedid2(unsigned char *buffer)
{
   unsigned short year = buffer[6] | (buffer[7] << 8);

   /* Check EDID version */
   if((buffer[0] & 0xf0) != 0x20) return FALSE;

   /* Check week of manufacture for sanity */
   if(buffer[5] > 54) return FALSE;

   /* Check year of manufacture for sanity */
   if((year != 0) && ((year < 1990) || (year > 2030))) return FALSE;

   return TRUE;
}

static int
SiS_FindPanelFromDB(SISPtr pSiS, unsigned short panelvendor, unsigned short panelproduct,
			int *maxx, int *maxy, int *prefx, int *prefy)
{
   int i, j;
   BOOLEAN done = FALSE;

   i = 0;
   while((!done) && (SiS_PlasmaTable[i].vendor) && panelvendor) {
      if(SiS_PlasmaTable[i].vendor == panelvendor) {
	 for(j=0; j<SiS_PlasmaTable[i].productnum; j++) {
	    if(SiS_PlasmaTable[i].product[j] == panelproduct) {
	       if(SiS_PlasmaTable[i].maxx && SiS_PlasmaTable[i].maxy) {
		  (*maxx) = (int)SiS_PlasmaTable[i].maxx;
		  (*maxy) = (int)SiS_PlasmaTable[i].maxy;
		  (*prefx) = (int)SiS_PlasmaTable[i].prefx;
		  (*prefy) = (int)SiS_PlasmaTable[i].prefy;
		  done = TRUE;
		  xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
			"Identified %s, correcting max X res %d, max Y res %d\n",
			 SiS_PlasmaTable[i].plasmaname,
			 SiS_PlasmaTable[i].maxx, SiS_PlasmaTable[i].maxy);
		  break;
	       }
	    }
	 }
      }
      i++;
   }
   return (done) ? 1 : 0;
}

/* Sense the LCD parameters (CR36, CR37) via DDC */
/* SiS TMDS bridges only */
unsigned short
SiS_SenseLCDDDC(struct SiS_Private *SiS_Pr, SISPtr pSiS)
{
   unsigned short DDCdatatype, paneltype, adapternum, flag, xres=0, yres=0;
   unsigned short index, myindex, lumsize, numcodes, panelvendor, panelproduct;
   int maxx=0, maxy=0, prefx=0, prefy=0;
   unsigned char cr37=0, seekcode;
   BOOLEAN checkexpand = FALSE;
   BOOLEAN havesync = FALSE;
   BOOLEAN indb = FALSE;
   int retry, i;
   int panel1280x960 = (pSiS->VGAEngine == SIS_315_VGA) ? Panel310_1280x960 : Panel300_1280x960;
   unsigned char buffer[256];

   for(i=0; i<7; i++) SiS_Pr->CP_DataValid[i] = FALSE;
   SiS_Pr->CP_HaveCustomData = FALSE;
   SiS_Pr->CP_MaxX = SiS_Pr->CP_MaxY = SiS_Pr->CP_MaxClock = 0;
   SiS_Pr->CP_PreferredX = SiS_Pr->CP_PreferredY = 0;
   SiS_Pr->CP_PreferredIndex = -1;
   SiS_Pr->CP_PrefClock = 0;
   SiS_Pr->PanelSelfDetected = FALSE;

   if(!(pSiS->VBFlags2 & VB2_SISTMDSBRIDGE)) return 0;
   if(pSiS->VBFlags2 & VB2_30xBDH) return 0;

   /* Specific for XGI_40/Rev 2/A01 (XGI V3XT A01): This card has CRT1's
    * and CRT2's DDC ports physically connected to each other. There
    * is no connection to the video bridge's DDC port, both DDC
    * channels are routed to the GPU. Smart. If both CRT1 (CRT) and
    * CRT2 (VGA or LCD) are connected, DDC will fail. Hence, no
    * reliable panel detection here...
    */
   adapternum = 1;
   if(SiS_Pr->DDCPortMixup) adapternum = 0;

   if(SiS_InitDDCRegs(SiS_Pr, pSiS->VBFlags, pSiS->VGAEngine, adapternum, 0, FALSE, pSiS->VBFlags2) == 0xFFFF)
      return 0;

   SiS_Pr->SiS_DDC_SecAddr = 0x00;

   /* Probe supported DA's */
   flag = SiS_ProbeDDC(SiS_Pr);
#ifdef TWDEBUG
   xf86DrvMsg(pSiS->pScrn->scrnIndex, X_INFO,
	"CRT2 DDC capabilities 0x%x\n", flag);
#endif
   if(flag & 0x10) {
      SiS_Pr->SiS_DDC_DeviceAddr = 0xa6;	/* EDID V2 (FP) */
      DDCdatatype = 4;
   } else if(flag & 0x08) {
      SiS_Pr->SiS_DDC_DeviceAddr = 0xa2;	/* EDID V2 (P&D-D Monitor) */
      DDCdatatype = 3;
   } else if(flag & 0x02) {
      SiS_Pr->SiS_DDC_DeviceAddr = 0xa0;	/* EDID V1 */
      DDCdatatype = 1;
   } else return 0;				/* no DDC support (or no device attached) */

   /* Read the entire EDID */
   retry = 2;
   do {
      if(SiS_ReadDDC(SiS_Pr, DDCdatatype, buffer)) {
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
		"CRT2: DDC read failed (attempt %d), %s\n",
		(3-retry), (retry == 1) ? "giving up" : "retrying");
	 retry--;
	 if(retry == 0) return 0xFFFF;
      } else break;
   } while(1);

#ifdef TWDEBUG
   for(i=0; i<256; i+=16) {
      xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
	"%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
	buffer[i],    buffer[i+1], buffer[i+2], buffer[i+3],
	buffer[i+4],  buffer[i+5], buffer[i+6], buffer[i+7],
	buffer[i+8],  buffer[i+9], buffer[i+10], buffer[i+11],
	buffer[i+12], buffer[i+13], buffer[i+14], buffer[i+15]);
   }
#endif

   /* Analyze EDID and retrieve LCD panel information */
   paneltype = 0;
   switch(DDCdatatype) {
   case 1:							/* Analyze EDID V1 */
      /* Catch a few clear cases: */
      if(!(checkedid1(buffer))) {
	xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
	 	"LCD sense: EDID corrupt\n");
	 return 0;
      }

      if(!(buffer[0x14] & 0x80)) {
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
		"LCD sense: Attached display expects analog input (0x%02x)\n",
		buffer[0x14]);
	 return 0;
      }

      /* Save given gamma */
      pSiS->CRT2LCDMonitorGamma = (buffer[0x17] + 100) * 10;

      /* Now analyze the first Detailed Timing Block and see
       * if the preferred timing mode is stored there. If so,
       * check if this is a standard panel for which we already
       * know the timing.
       */

      paneltype = Panel_Custom;
      checkexpand = FALSE;

      panelvendor = buffer[9] | (buffer[8] << 8);
      panelproduct = buffer[10] | (buffer[11] << 8);

      /* Overrule bogus preferred modes from database */
      if((indb = SiS_FindPanelFromDB(pSiS, panelvendor, panelproduct, &maxx, &maxy, &prefx, &prefy))) {
	 if(prefx) SiS_Pr->CP_PreferredX = xres = prefx;
	 if(prefy) SiS_Pr->CP_PreferredY = yres = prefy;
      }

      if(buffer[0x18] & 0x02) {

	 unsigned short pclk = (buffer[0x36] | (buffer[0x37] << 8));
	 unsigned short phb  = (buffer[0x39] | ((buffer[0x3a] & 0x0f) << 8));
	 unsigned short pvb  = (buffer[0x3c] | ((buffer[0x3d] & 0x0f) << 8));

	 if(!xres) SiS_Pr->CP_PreferredX = xres = buffer[0x38] | ((buffer[0x3a] & 0xf0) << 4);
	 if(!yres) SiS_Pr->CP_PreferredY = yres = buffer[0x3b] | ((buffer[0x3d] & 0xf0) << 4);

	 switch(xres) {
	    case 1024:
	        if(yres == 768) {
		   paneltype = Panel_1024x768;
		   checkexpand = TRUE;
	        }
	        break;
	    case 1280:
		if(yres == 1024) {
		   paneltype = Panel_1280x1024;
		   checkexpand = TRUE;
		} else if(yres == 960) {
		   paneltype = panel1280x960;
		} else if(yres == 768) {
		   if( (pclk == 8100) &&
		       (phb == (1688 - 1280)) &&
		       (pvb == (802 - 768)) ) {
		      paneltype = Panel_1280x768;
		      checkexpand = FALSE;
		      cr37 |= 0x10;
		   }
		} else if(yres == 800) {
		   if( (pclk == 6900) &&
		       (phb == (1408 - 1280)) &&
		       (pvb == (816 - 800)) ) {
		      paneltype = Panel_1280x800;
		   }
		}
		break;
	    case 1400:
		if(pSiS->VGAEngine == SIS_315_VGA) {
		   if(yres == 1050) {
		      paneltype = Panel310_1400x1050;
		      checkexpand = TRUE;
		   }
		}
		break;
	    case 1600:
		if((pSiS->VGAEngine == SIS_315_VGA) && (pSiS->VBFlags2 & VB2_30xC)) {
		   if(yres == 1200) {
		      if( (pclk == 16200) &&
			  (phb == (2160 - 1600)) &&
			  (pvb == (1250 - 1200)) ) {
			 paneltype = Panel310_1600x1200;
			 checkexpand = TRUE;
		      }
		   }
		}
		break;
	 }

	 /* Save sync: This is used if "Pass 1:1" is off; in this case
	  * we always use the panel's native mode = this "preferred mode"
	  * we just have been analysing. Hence, we also need its sync.
	  */
	 if((buffer[0x47] & 0x18) == 0x18) {
	    cr37 |= ((((buffer[0x47] & 0x06) ^ 0x06) << 5) | 0x20);
	    havesync = TRUE;
	 } else {
	    /* What now? There is no digital separate output timing... */
	    xf86DrvMsg(pSiS->pScrn->scrnIndex, X_WARNING,
		   "LCD sense: Unable to retrieve Sync polarity information\n");
	    cr37 |= 0xc0;  /* Default */
	 }

      }

      /* Check against our database; eg. Sanyo Z2 projector reports
       * 1024x768 as preferred mode, although it supports 1280x720
       * natively in non-HDCP mode. Treat such wrongly reporting
       * panels as custom and fixup actual maximum resolutions.
       */
      if(paneltype != Panel_Custom) {
	 if(indb) {
	    paneltype = Panel_Custom;
	    SiS_Pr->CP_MaxX = maxx;
	    SiS_Pr->CP_MaxY = maxy;
	    /* Leave preferred unchanged (MUST contain a valid mode!) */
	 }
      }

      /* If we still don't know what panel this is, we take it
       * as a custom panel and derive the timing data from the
       * detailed timing blocks
       */
      if(paneltype == Panel_Custom) {

	 int i, temp, base = 0x36;
	 unsigned long estpack;
	 static const unsigned short estx[] = {
		720, 720, 640, 640, 640, 640, 800, 800,
		800, 800, 832,1024,1024,1024,1024,1280,
		1152
	 };
	 static const unsigned short esty[] = {
		400, 400, 480, 480, 480, 480, 600, 600,
		600, 600, 624, 768, 768, 768, 768,1024,
		870
	 };
	 static const int estclk[] = {
		    0,     0, 25100,   0, 31500, 31500, 36100, 40000,
		50100, 49500,     0,   0, 65100, 75200, 78700,135200,
		0
	 };

	 paneltype = 0;
	 SiS_Pr->CP_Supports64048075 = TRUE;

	 /* Find the maximum resolution */

	 /* 1. From Established timings */
	 estpack = (buffer[0x23] << 9) | (buffer[0x24] << 1) | ((buffer[0x25] >> 7) & 0x01);
	 for(i=16; i>=0; i--) {
	     if(estpack & (1 << i)) {
		if(estx[16 - i] > SiS_Pr->CP_MaxX) SiS_Pr->CP_MaxX = estx[16 - i];
		if(esty[16 - i] > SiS_Pr->CP_MaxY) SiS_Pr->CP_MaxY = esty[16 - i];
		if(estclk[16 - i] > SiS_Pr->CP_MaxClock) SiS_Pr->CP_MaxClock = estclk[16 - i];
	     }
	 }

	 /* By default we drive the LCD at 75Hz in 640x480 mode; if
	  * the panel does not provide this mode, use 60hz
	  */
	 if(!(buffer[0x23] & 0x04)) SiS_Pr->CP_Supports64048075 = FALSE;

	 /* 2. From Standard Timings */
	 for(i=0x26; i < 0x36; i+=2) {
	    if((buffer[i] != 0x01) && (buffer[i+1] != 0x01)) {
	       temp = (buffer[i] + 31) * 8;
	       if(temp > SiS_Pr->CP_MaxX) SiS_Pr->CP_MaxX = temp;
	       switch((buffer[i+1] & 0xc0) >> 6) {
	       case 0x03: temp = temp * 9 / 16; break;
	       case 0x02: temp = temp * 4 / 5;  break;
	       case 0x01: temp = temp * 3 / 4;  break;
	       }
	       if(temp > SiS_Pr->CP_MaxY) SiS_Pr->CP_MaxY = temp;
	    }
	 }

	 /* Now extract the Detailed Timings and convert them into modes */

	for(i = 0; i < 4; i++, base += 18) {

	    /* Is this a detailed timing block or a monitor descriptor? */
	    if(buffer[base] || buffer[base+1] || buffer[base+2]) {

	       xres = buffer[base+2] | ((buffer[base+4] & 0xf0) << 4);
	       yres = buffer[base+5] | ((buffer[base+7] & 0xf0) << 4);

	       SiS_Pr->CP_HDisplay[i] = xres;
	       SiS_Pr->CP_HSyncStart[i] = xres + (buffer[base+8] | ((buffer[base+11] & 0xc0) << 2));
	       SiS_Pr->CP_HSyncEnd[i]   = SiS_Pr->CP_HSyncStart[i] + (buffer[base+9] | ((buffer[base+11] & 0x30) << 4));
	       SiS_Pr->CP_HTotal[i] = xres + (buffer[base+3] | ((buffer[base+4] & 0x0f) << 8));
	       SiS_Pr->CP_HBlankStart[i] = xres + 1;
	       SiS_Pr->CP_HBlankEnd[i] = SiS_Pr->CP_HTotal[i];

	       SiS_Pr->CP_VDisplay[i] = yres;
	       SiS_Pr->CP_VSyncStart[i] = yres + (((buffer[base+10] & 0xf0) >> 4) | ((buffer[base+11] & 0x0c) << 2));
	       SiS_Pr->CP_VSyncEnd[i] = SiS_Pr->CP_VSyncStart[i] + ((buffer[base+10] & 0x0f) | ((buffer[base+11] & 0x03) << 4));
	       SiS_Pr->CP_VTotal[i] = yres + (buffer[base+6] | ((buffer[base+7] & 0x0f) << 8));
	       SiS_Pr->CP_VBlankStart[i] = yres + 1;
	       SiS_Pr->CP_VBlankEnd[i] = SiS_Pr->CP_VTotal[i];

	       SiS_Pr->CP_Clock[i] = (buffer[base] | (buffer[base+1] << 8)) * 10;

	       SiS_Pr->CP_DataValid[i] = TRUE;

	       /* Sort out invalid timings, interlace and too high clocks */
	       if((SiS_Pr->CP_HDisplay[i] & 7)						  ||
		  (SiS_Pr->CP_HDisplay[i] > SiS_Pr->CP_HSyncStart[i])			  ||
		  (SiS_Pr->CP_HDisplay[i] >= SiS_Pr->CP_HSyncEnd[i])			  ||
		  (SiS_Pr->CP_HDisplay[i] >= SiS_Pr->CP_HTotal[i])			  ||
		  (SiS_Pr->CP_HSyncStart[i] >= SiS_Pr->CP_HSyncEnd[i])			  ||
		  (SiS_Pr->CP_HSyncStart[i] > SiS_Pr->CP_HTotal[i])			  ||
		  (SiS_Pr->CP_HSyncEnd[i] > SiS_Pr->CP_HTotal[i])			  ||
		  (SiS_Pr->CP_VDisplay[i] > SiS_Pr->CP_VSyncStart[i])			  ||
		  (SiS_Pr->CP_VDisplay[i] >= SiS_Pr->CP_VSyncEnd[i])			  ||
		  (SiS_Pr->CP_VDisplay[i] >= SiS_Pr->CP_VTotal[i])			  ||
		  (SiS_Pr->CP_VSyncStart[i] > SiS_Pr->CP_VSyncEnd[i])			  ||
		  (SiS_Pr->CP_VSyncStart[i] > SiS_Pr->CP_VTotal[i])			  ||
		  (SiS_Pr->CP_VSyncEnd[i] > SiS_Pr->CP_VTotal[i])			  ||
		  (((pSiS->VBFlags2 & VB2_30xC) && (SiS_Pr->CP_Clock[i] > 162500)) ||
		   ((!(pSiS->VBFlags2 & VB2_30xC))        &&
		    ( (SiS_Pr->CP_Clock[i] > 110500)  ||		/* TODO for 307 */
		      (SiS_Pr->CP_VDisplay[i] > 1024) ||
		      (SiS_Pr->CP_HDisplay[i] > 1600) )))				  ||
		  (buffer[base+17] & 0x80)) {

		  SiS_Pr->CP_DataValid[i] = FALSE;

	       } else {

		  SiS_Pr->CP_HaveCustomData = TRUE;

		  if(xres > SiS_Pr->CP_MaxX) SiS_Pr->CP_MaxX = xres;
		  if(yres > SiS_Pr->CP_MaxY) SiS_Pr->CP_MaxY = yres;
		  if(SiS_Pr->CP_Clock[i] > SiS_Pr->CP_MaxClock) SiS_Pr->CP_MaxClock = SiS_Pr->CP_Clock[i];

		  if((SiS_Pr->CP_PreferredX == xres) && (SiS_Pr->CP_PreferredY == yres)) {
		     SiS_Pr->CP_PreferredIndex = i;
		     SiS_MakeClockRegs(pSiS->pScrn, SiS_Pr->CP_Clock[i], &SiS_Pr->CP_PrefSR2B, &SiS_Pr->CP_PrefSR2C);
		     SiS_Pr->CP_PrefClock = (SiS_Pr->CP_Clock[i] / 1000) + 1;
		  }

		  /* Extract the sync polarisation information. This only works
		   * if the Flags indicate a digital separate output.
		   */
		  if((buffer[base+17] & 0x18) == 0x18) {
		     SiS_Pr->CP_HSync_P[i] = (buffer[base+17] & 0x02) ? TRUE : FALSE;
		     SiS_Pr->CP_VSync_P[i] = (buffer[base+17] & 0x04) ? TRUE : FALSE;
		     SiS_Pr->CP_SyncValid[i] = TRUE;
		     if((i == SiS_Pr->CP_PreferredIndex) && (!havesync)) {
			cr37 |= ((((buffer[base+17] & 0x06) ^ 0x06) << 5) | 0x20);
			havesync = TRUE;
		     }
		  } else {
		     SiS_Pr->CP_SyncValid[i] = FALSE;
		  }

	       }

	    } else if((!buffer[base]) && (!buffer[base+1]) && (!buffer[base+2]) && (!buffer[base+4])) {

	       /* Maximum pixclock from Monitor Range Limits */
	       if((buffer[base+3] == 0xfd) && (buffer[base+9] != 0xff)) {
		  int maxclk = buffer[base+9] * 10;
		  /* More than 170 is not supported anyway */
		  if(maxclk <= 170) SiS_Pr->CP_MaxClock = maxclk * 1000;
	       }

	    }

	 }

	 if(SiS_Pr->CP_MaxX && SiS_Pr->CP_MaxY) {
	    paneltype = Panel_Custom;
	    checkexpand = FALSE;
	    cr37 |= 0x10;
	    SiS_Pr->CP_Vendor = panelvendor;
	    SiS_Pr->CP_Product = panelproduct;
	 }

      }

      if(paneltype && checkexpand) {
	 /* If any of the Established low-res modes is supported, the
	  * panel can scale automatically. For 800x600 panels, we only
	  * check the even lower ones.
	  */
	 if(paneltype == Panel_800x600) {
	    if(buffer[0x23] & 0xfc) cr37 |= 0x10;
	 } else {
	    if(buffer[0x23])	    cr37 |= 0x10;
	 }
      }

      break;

   case 3:							/* Analyze EDID V2 */
   case 4:
      index = 0;

      if(!(checkedid2(buffer))) {
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
	 	"LCD sense: EDID corrupt\n");
	 return 0;
      }

      if((buffer[0x41] & 0x0f) == 0x03) {
	 index = 0x42 + 3;
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
	 	"LCD sense: Display supports TMDS input on primary interface\n");
      } else if((buffer[0x41] & 0xf0) == 0x30) {
	 index = 0x46 + 3;
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
	 	"LCD sense: Display supports TMDS input on secondary interface\n");
      } else {
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
		"LCD sense: Display does not support TMDS video interface (0x%02x)\n",
		buffer[0x41]);
	 return 0;
      }

      /* Save given gamma */
      pSiS->CRT2LCDMonitorGamma = (buffer[0x56] + 100) * 10;

      SiS_Pr->CP_Vendor = panelvendor = buffer[2] | (buffer[1] << 8);
      SiS_Pr->CP_Product = panelproduct = buffer[3] | (buffer[4] << 8);

      paneltype = Panel_Custom;
      SiS_Pr->CP_MaxX = SiS_Pr->CP_PreferredX = xres = buffer[0x76] | (buffer[0x77] << 8);
      SiS_Pr->CP_MaxY = SiS_Pr->CP_PreferredY = yres = buffer[0x78] | (buffer[0x79] << 8);

      switch(xres) {
         case 1024:
	     if(yres == 768) {
		paneltype = Panel_1024x768;
		checkexpand = TRUE;
	     }
	     break;
	 case 1280:
	     if(yres == 960) {
		paneltype = panel1280x960;
	     } else if(yres == 1024) {
		paneltype = Panel_1280x1024;
		checkexpand = TRUE;
	     }
	     /* 1280x768, 1280x800 treated as custom here */
	     break;
	 case 1400:
	     if(pSiS->VGAEngine == SIS_315_VGA) {
		if(yres == 1050) {
		   paneltype = Panel310_1400x1050;
		   checkexpand = TRUE;
		}
	     }
	     break;
	 /* 1600x1200 treated as custom */
      }

      /* Determine if RGB18 or RGB24 */
      if(index) {
	 if((buffer[index] == 0x20) || (buffer[index] == 0x34)) {
	    cr37 |= 0x01;
	 }
      }

      if(checkexpand) {
	 /* TODO - for now, we let the panel scale */
	 cr37 |= 0x10;
      }

      /* Now seek 4-Byte Timing codes and extract sync pol info */
      index = 0x80;
      if(buffer[0x7e] & 0x20) {			    /* skip Luminance Table (if provided) */
	 lumsize = buffer[0x80] & 0x1f;
	 if(buffer[0x80] & 0x80) lumsize *= 3;
	 lumsize++;  /* luminance header byte */
	 index += lumsize;
      }
#if 0 /* "pixel rate" = pixel clock? */
      if(buffer[0x7e] & 0x1c) {
	 for(i=0; i<((buffer[0x7e] & 0x1c) >> 2); i++) {
	    if(buffer[index + (i*8) + 6] && (buffer[index + (i*8) + 7] & 0x0f)) {
	       int clk = (buffer[index + (i*8) + 6] | ((buffer[index + (i*8) + 7] & 0x0f) << 4)) * 1000;
	       if(clk > SiS_Pr->CP_MaxClock) SiS_Pr->CP_MaxClock = clk;
	    }
	 }
      }
#endif
      index += (((buffer[0x7e] & 0x1c) >> 2) * 8);   /* skip Frequency Ranges */
      if(buffer[0x7e] & 0x03) {
	 for(i=0; i<(buffer[0x7e] & 0x03); i++) {
	    if((buffer[index + (i*27) + 9]) || (buffer[index + (i*27) + 10])) {
	       int clk = ((buffer[index + (i*27) + 9]) | ((buffer[index + (i*27) + 9]) << 8)) * 10;
	       if(clk > SiS_Pr->CP_MaxClock) SiS_Pr->CP_MaxClock = clk;
	    }
	 }
      }
      index += ((buffer[0x7e] & 0x03) * 27);         /* skip Detailed Range Limits */
      numcodes = (buffer[0x7f] & 0xf8) >> 3;
      if(numcodes) {
	 myindex = index;
	 seekcode = (xres - 256) / 16;
	 for(i=0; i<numcodes; i++) {
	    if(buffer[myindex] == seekcode) break;
	    myindex += 4;
	 }
	 if(buffer[myindex] == seekcode) {
	    cr37 |= ((((buffer[myindex + 1] & 0x0c) ^ 0x0c) << 4) | 0x20);
	    havesync = TRUE;
	 } else {
	    xf86DrvMsg(pSiS->pScrn->scrnIndex, X_WARNING,
		"LCD sense: Unable to retrieve Sync polarity information\n");
	 }
      } else {
         xf86DrvMsg(pSiS->pScrn->scrnIndex, X_WARNING,
	     "LCD sense: Unable to retrieve Sync polarity information\n");
      }

      /* Check against our database; Eg. Sanyo projector reports
       * 1024x768 in non-HDPC mode, although it supports 1280x720.
       * Treat such wrongly reporting panels as custom.
       */
      if(paneltype != Panel_Custom) {
	 int maxx, maxy, prefx, prefy;
	 if((SiS_FindPanelFromDB(pSiS, panelvendor, panelproduct, &maxx, &maxy, &prefx, &prefy))) {
	    paneltype = Panel_Custom;
	    SiS_Pr->CP_MaxX = maxx;
	    SiS_Pr->CP_MaxY = maxy;
	    cr37 |= 0x10;
	    /* Leave preferred unchanged (MUST be a valid mode!) */
	 }
      }

      /* Now seek the detailed timing descriptions for custom panels */
      if(paneltype == Panel_Custom) {

         SiS_Pr->CP_Supports64048075 = TRUE;

         index += (numcodes * 4);
	 numcodes = buffer[0x7f] & 0x07;
	 for(i=0; i<numcodes; i++, index += 18) {
	    xres = buffer[index+2] | ((buffer[index+4] & 0xf0) << 4);
            yres = buffer[index+5] | ((buffer[index+7] & 0xf0) << 4);

	    SiS_Pr->CP_HDisplay[i] = xres;
	    SiS_Pr->CP_HSyncStart[i] = xres + (buffer[index+8] | ((buffer[index+11] & 0xc0) << 2));
            SiS_Pr->CP_HSyncEnd[i] = SiS_Pr->CP_HSyncStart[i] + (buffer[index+9] | ((buffer[index+11] & 0x30) << 4));
	    SiS_Pr->CP_HTotal[i] = xres + (buffer[index+3] | ((buffer[index+4] & 0x0f) << 8));
	    SiS_Pr->CP_HBlankStart[i] = xres + 1;
	    SiS_Pr->CP_HBlankEnd[i] = SiS_Pr->CP_HTotal[i];

	    SiS_Pr->CP_VDisplay[i] = yres;
            SiS_Pr->CP_VSyncStart[i] = yres + (((buffer[index+10] & 0xf0) >> 4) | ((buffer[index+11] & 0x0c) << 2));
            SiS_Pr->CP_VSyncEnd[i] = SiS_Pr->CP_VSyncStart[i] + ((buffer[index+10] & 0x0f) | ((buffer[index+11] & 0x03) << 4));
	    SiS_Pr->CP_VTotal[i] = yres + (buffer[index+6] | ((buffer[index+7] & 0x0f) << 8));
	    SiS_Pr->CP_VBlankStart[i] = yres + 1;
	    SiS_Pr->CP_VBlankEnd[i] = SiS_Pr->CP_VTotal[i];

	    SiS_Pr->CP_Clock[i] = (buffer[index] | (buffer[index+1] << 8)) * 10;

	    SiS_Pr->CP_DataValid[i] = TRUE;

	    if((SiS_Pr->CP_HDisplay[i] & 7)						||
	       (SiS_Pr->CP_HDisplay[i] > SiS_Pr->CP_HSyncStart[i])  			||
	       (SiS_Pr->CP_HDisplay[i] >= SiS_Pr->CP_HSyncEnd[i])   			||
	       (SiS_Pr->CP_HDisplay[i] >= SiS_Pr->CP_HTotal[i])     			||
	       (SiS_Pr->CP_HSyncStart[i] >= SiS_Pr->CP_HSyncEnd[i]) 			||
	       (SiS_Pr->CP_HSyncStart[i] > SiS_Pr->CP_HTotal[i])    			||
	       (SiS_Pr->CP_HSyncEnd[i] > SiS_Pr->CP_HTotal[i])      			||
	       (SiS_Pr->CP_VDisplay[i] > SiS_Pr->CP_VSyncStart[i])  			||
	       (SiS_Pr->CP_VDisplay[i] >= SiS_Pr->CP_VSyncEnd[i])   			||
	       (SiS_Pr->CP_VDisplay[i] >= SiS_Pr->CP_VTotal[i])     			||
	       (SiS_Pr->CP_VSyncStart[i] > SiS_Pr->CP_VSyncEnd[i])  			||
	       (SiS_Pr->CP_VSyncStart[i] > SiS_Pr->CP_VTotal[i])    			||
	       (SiS_Pr->CP_VSyncEnd[i] > SiS_Pr->CP_VTotal[i])      			||
	       (((pSiS->VBFlags2 & VB2_30xC) && (SiS_Pr->CP_Clock[i] > 162500)) ||
	        ((!(pSiS->VBFlags2 & VB2_30xC))        &&
		 ( (SiS_Pr->CP_Clock[i] > 110500)  ||
		   (SiS_Pr->CP_VDisplay[i] > 1024) ||
		   (SiS_Pr->CP_HDisplay[i] > 1600) )))					||
	       (buffer[index + 17] & 0x80)) {

	       SiS_Pr->CP_DataValid[i] = FALSE;

	    } else {

	       SiS_Pr->CP_HaveCustomData = TRUE;

	       if(SiS_Pr->CP_Clock[i] > SiS_Pr->CP_MaxClock) SiS_Pr->CP_MaxClock = SiS_Pr->CP_Clock[i];

	       if((SiS_Pr->CP_PreferredX == xres) && (SiS_Pr->CP_PreferredY == yres)) {
		  SiS_Pr->CP_PreferredIndex = i;
		  SiS_MakeClockRegs(pSiS->pScrn, SiS_Pr->CP_Clock[i], &SiS_Pr->CP_PrefSR2B, &SiS_Pr->CP_PrefSR2C);
		  SiS_Pr->CP_PrefClock = (SiS_Pr->CP_Clock[i] / 1000) + 1;
		  if(!havesync) {
		     cr37 |= ((((buffer[index + 17] & 0x06) ^ 0x06) << 5) | 0x20);
		     havesync = TRUE;
		  }
	       }

	       SiS_Pr->CP_HSync_P[i] = (buffer[index + 17] & 0x02) ? TRUE : FALSE;
	       SiS_Pr->CP_VSync_P[i] = (buffer[index + 17] & 0x04) ? TRUE : FALSE;
	       SiS_Pr->CP_SyncValid[i] = TRUE;

	    }
	 }

	 cr37 |= 0x10;

      }

      break;

   }

   /* 1280x960 panels are always RGB24, unable to scale and use
    * high active sync polarity. (Check is save, other panel types
    * for other chipset series not being set up)
    */
   if(paneltype == panel1280x960) cr37 &= 0x0e;

   for(i = 0; i < 7; i++) {
      if(SiS_Pr->CP_DataValid[i]) {
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
	    "Non-standard LCD/DVI-D timing data no. %d:\n", i);
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
	    "   HDisplay %d HSync %d HSyncEnd %d HTotal %d\n",
	    SiS_Pr->CP_HDisplay[i], SiS_Pr->CP_HSyncStart[i],
	    SiS_Pr->CP_HSyncEnd[i], SiS_Pr->CP_HTotal[i]);
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
	    "   VDisplay %d VSync %d VSyncEnd %d VTotal %d\n",
	    SiS_Pr->CP_VDisplay[i], SiS_Pr->CP_VSyncStart[i],
	    SiS_Pr->CP_VSyncEnd[i], SiS_Pr->CP_VTotal[i]);
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
	    "   Pixel clock: %3.3fMhz\n", (float)SiS_Pr->CP_Clock[i] / 1000);
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_INFO,
	    "   To use this, add \"%dx%d\" to the Modes list in the Screen section\n",
	    SiS_Pr->CP_HDisplay[i],
	    SiS_Pr->CP_VDisplay[i]);
      }
   }

   if(paneltype) {
       if(!SiS_Pr->CP_PreferredX) SiS_Pr->CP_PreferredX = SiS_Pr->CP_MaxX;
       if(!SiS_Pr->CP_PreferredY) SiS_Pr->CP_PreferredY = SiS_Pr->CP_MaxY;
       SiS_SetRegOR(SiS_Pr->SiS_P3d4,0x32,0x08);
       SiS_SetReg(SiS_Pr->SiS_P3d4,0x36,paneltype);
       cr37 &= 0xf1;
       SiS_SetRegANDOR(SiS_Pr->SiS_P3d4,0x37,0x0c,cr37);
       SiS_Pr->PanelSelfDetected = TRUE;
#ifdef TWDEBUG
       xf86DrvMsgVerb(pSiS->pScrn->scrnIndex, X_PROBED, 3,
	   "LCD sense: [DDC LCD results: 0x%02x, 0x%02x]\n", paneltype, cr37);
#endif
   } else {
       SiS_SetRegAND(SiS_Pr->SiS_P3d4,0x32,~0x08);
       SiS_SetReg(SiS_Pr->SiS_P3d4,0x36,0x00);
   }
   return 0;
}

unsigned short
SiS_SenseVGA2DDC(struct SiS_Private *SiS_Pr, SISPtr pSiS)
{
   unsigned short DDCdatatype, flag;
   BOOLEAN foundcrt = FALSE;
   int retry;
   unsigned char buffer[256];

   if(!(pSiS->VBFlags2 & VB2_SISVGA2BRIDGE)) return 0;

   /* Specific for XGI_40/Rev 2/A01 (XGI V3XT A01): This card has CRT1's
    * and CRT2's DDC ports physically connected to each other. There
    * is no connection to the video bridge's DDC port, both DDC
    * channels are routed to the GPU. Smart. If both CRT1 (CRT) and
    * CRT2 (VGA or LCD) are connected, DDC will fail. If a CRT is
    * connected to the DVI-I port, it will report "analog" as well,
    * so we never know if the monitor is connected to CRT1 or CRT2.
    * Hence, no reliable CRT detection here... we need to fall back to
    * the sensing stuff in sis_vb.c.
    */
   if(SiS_Pr->DDCPortMixup) return 0;

   if(SiS_InitDDCRegs(SiS_Pr, pSiS->VBFlags, pSiS->VGAEngine, 2, 0, FALSE, pSiS->VBFlags2) == 0xFFFF)
      return 0;

   SiS_Pr->SiS_DDC_SecAddr = 0x00;

   /* Probe supported DA's */
   flag = SiS_ProbeDDC(SiS_Pr);
   if(flag & 0x10) {
      SiS_Pr->SiS_DDC_DeviceAddr = 0xa6;	/* EDID V2 (FP) */
      DDCdatatype = 4;
   } else if(flag & 0x08) {
      SiS_Pr->SiS_DDC_DeviceAddr = 0xa2;	/* EDID V2 (P&D-D Monitor) */
      DDCdatatype = 3;
   } else if(flag & 0x02) {
      SiS_Pr->SiS_DDC_DeviceAddr = 0xa0;	/* EDID V1 */
      DDCdatatype = 1;
   } else {
	xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
		"VGA2 sense: Do DDC answer\n");
	return 0;				/* no DDC support (or no device attached) */
   }

   /* Read the entire EDID */
   retry = 2;
   do {
      if(SiS_ReadDDC(SiS_Pr, DDCdatatype, buffer)) {
	 xf86DrvMsg(pSiS->pScrn->scrnIndex, X_PROBED,
		"VGA2 sense: DDC read failed (attempt %d), %s\n",
		(3-retry), (retry == 1) ? "giving up" : "retrying");
	 retry--;
	 if(retry == 0) return 0xFFFF;
      } else break;
   } while(1);

   /* Analyze EDID. We don't have many chances to
    * distinguish a flat panel from a CRT...
    */
   switch(DDCdatatype) {

   case 1:
      if(!(checkedid1(buffer))) {
	  xf86DrvMsg(pSiS->pScrn->scrnIndex, X_ERROR,
		"VGA2 sense: EDID corrupt\n");
	  return 0;
      }
      if(buffer[0x14] & 0x80) {		/* Display uses digital input */
	  xf86DrvMsg(pSiS->pScrn->scrnIndex, X_ERROR,
		"VGA2 sense: Attached display expects digital input\n");
	  return 0;
      }
      SiS_Pr->CP_Vendor = buffer[9] | (buffer[8] << 8);
      SiS_Pr->CP_Product = buffer[10] | (buffer[11] << 8);
      foundcrt = TRUE;

      /* Save given gamma */
      pSiS->CRT2VGAMonitorGamma = (buffer[0x17] + 100) * 10;

      break;

   case 3:
   case 4:
      if(!(checkedid2(buffer))) {
	  xf86DrvMsg(pSiS->pScrn->scrnIndex, X_ERROR,
		"VGA2 sense: EDID corrupt\n");
	  return 0;
      }
      if( ((buffer[0x41] & 0x0f) != 0x01) &&  	/* Display does not support analog input */
	  ((buffer[0x41] & 0x0f) != 0x02) &&
	  ((buffer[0x41] & 0xf0) != 0x10) &&
	  ((buffer[0x41] & 0xf0) != 0x20) ) {
	  xf86DrvMsg(pSiS->pScrn->scrnIndex, X_ERROR,
		"VGA2 sense: Attached display does not support analog input (0x%02x)\n",
		buffer[0x41]);
	  return 0;
      }
      SiS_Pr->CP_Vendor = buffer[2] | (buffer[1] << 8);
      SiS_Pr->CP_Product = buffer[3] | (buffer[4] << 8);
      foundcrt = TRUE;

      /* Save given gamma */
      pSiS->CRT2VGAMonitorGamma = (buffer[0x56] + 100) * 10;

      break;
   }

   if(foundcrt) {
      SiS_SetRegOR(SiS_Pr->SiS_P3d4,0x32,0x10);
   }
   return(0);
}

/* 4-tap scaler for 301C and later */

static float
rcos(float x)
{
   double pi = 3.14159265358979;
   float  r = 0.5, y;

    if(x == 0.0) {
       y = 1.0;
    } else if(x == -1.0 || x == 1.0) {
       y = 0.0;
    } else {
       y = sin(pi * x) / (pi * x) * cos(r * pi * x) / (1 - x * x);
    }

    return y;
}

static int
roundandconv(float in)
{
    int a = ((int)(in)) * 10;
    int b = (int)(in * 10.0);

    if (in >= 0) {
      if((b - a) < 5)  return (a / 10);
      else             return (a / 10) + 1;
    } else {
      if((b - a) > -5) return (a / 10);
      else             return (a / 10) -1;
    }
}

void
SiS_CalcXTapScaler(struct SiS_Private *SiS_Pr, int srcsize, int destsize, int taps, Bool ishoriz)
{
   float scale = (float)srcsize / (float)destsize;
   int   coe_bit_number = 6;
   float fixnumber = (float)(1 << (coe_bit_number - 1));
   float ops, WW, W[8];
   int   WeightMat[16][8];
   int   i, j, index;

   /* For now: */
   if(taps != 4) taps = 4;

   if(scale < 1.0)      scale = 1.0;
   else if(scale > 1.0) scale *= 1.1;

   for(i = 0; i < 16; i++) {

      ops = (float)i / (16.0 * scale);

      switch(taps) {
      case 4:
	 W[0] = rcos( 1.0 / scale + ops);
	 W[1] = rcos( 0.0 / scale + ops);
	 W[2] = rcos(-1.0 / scale + ops);
	 W[3] = rcos(-2.0 / scale + ops);

	 WW = W[0] + W[1] + W[2] + W[3];

	 WeightMat[i][0] = roundandconv(W[0] / WW * fixnumber);
	 WeightMat[i][1] = roundandconv(W[1] / WW * fixnumber);
	 WeightMat[i][2] = roundandconv(W[2] / WW * fixnumber);
	 WeightMat[i][3] = (int)fixnumber - WeightMat[i][0] - WeightMat[i][1] - WeightMat[i][2];
	 break;
#if 0 /* For future use */
     case 8:
	 W[0] = rcos( 3.0/scale + ops);
	 W[1] = rcos( 2.0/scale + ops);
	 W[2] = rcos( 1.0/scale + ops);
	 W[3] = rcos( 0.0/scale + ops);
	 W[4] = rcos(-1.0/scale + ops);
	 W[5] = rcos(-2.0/scale + ops);
	 W[6] = rcos(-3.0/scale + ops);
	 W[7] = rcos(-4.0/scale + ops);

	 WW = W[0] + W[1] + W[2] + W[3] + W[4] + W[5] + W[6] + W[7];

	 WeightMat[i][0] = roundandconv(W[0]/WW * fixnumber);
	 WeightMat[i][1] = roundandconv(W[1]/WW * fixnumber);
	 WeightMat[i][2] = roundandconv(W[2]/WW * fixnumber);
	 WeightMat[i][3] = roundandconv(W[3]/WW * fixnumber);
	 WeightMat[i][4] = roundandconv(W[4]/WW * fixnumber);
	 WeightMat[i][5] = roundandconv(W[5]/WW * fixnumber);
	 WeightMat[i][6] = roundandconv(W[6]/WW * fixnumber);
	 WeightMat[i][7] = (int)fixnumber - WeightMat[i][0] - WeightMat[i][1] -
					    WeightMat[i][2] - WeightMat[i][3] -
					    WeightMat[i][4] - WeightMat[i][5] - WeightMat[i][6];
	 break;
#endif
      }
   }

   index = ishoriz ? 0x80 : 0xc0;
   for(i = 0; i < 16; i++) {
      for(j = 0; j < 4 /* taps! */; j++) {
         if(WeightMat[i][j] < 0) {
	    WeightMat[i][j] = ((~(-WeightMat[i][j])) + 1) & 0x7f;
         }
         SiS_SetReg(SiS_Pr->SiS_Part2Port, index++, WeightMat[i][j]);
      }
   }

}

void
SiS_SetGroup2_C_ELV(struct SiS_Private *SiS_Pr, unsigned short ModeNo, unsigned short ModeIdIndex,
	      	    unsigned short RefreshRateTableIndex)
{
   unsigned char temp;

   if(!(SiS_Pr->SiS_VBType & VB_SISTAP4SCALER)) return;

   SiS_CalcXTapScaler(SiS_Pr, SiS_Pr->SiS_VGAHDE, SiS_Pr->SiS_HDE, 4, TRUE);
   if(SiS_Pr->SiS_VBInfo & SetCRT2ToTV) {
      SiS_CalcXTapScaler(SiS_Pr, SiS_Pr->SiS_VGAVDE, SiS_Pr->SiS_VDE, 4, FALSE);
   }

   temp = 0x10;
   if(SiS_Pr->SiS_VBInfo & SetCRT2ToTV) temp |= 0x04;
   SiS_SetRegANDOR(SiS_Pr->SiS_Part2Port,0x4e,0xeb,temp);
}


