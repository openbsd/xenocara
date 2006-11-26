/* $XFree86$ */
/* $XdotOrg$ */
/*
 * DAC helper functions (Save/Restore, MemClk, etc)
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
 * --------------------------------------------------------------------------
 *
 * SiS_compute_vclk(), SiSCalcClock() and parts of SiSMclk():
 *
 * Copyright (C) 1998, 1999 by Alan Hourihane, Wigan, England
 * Written by:
 *	 Alan Hourihane <alanh@fairlite.demon.co.uk>,
 *       Mike Chapman <mike@paranoia.com>,
 *       Juanjo Santamarta <santamarta@ctv.es>,
 *       Mitani Hiroshi <hmitani@drl.mei.co.jp>,
 *       David Thomas <davtom@dream.org.uk>,
 *	 Thomas Winischhofer <thomas@winischhofer.net>.
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
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sis.h"
#define SIS_NEED_inSISREG
#define SIS_NEED_inSISREGW
#define SIS_NEED_inSISREGL
#define SIS_NEED_outSISREG
#define SIS_NEED_outSISREGW
#define SIS_NEED_outSISREGL
#define SIS_NEED_inSISIDXREG
#define SIS_NEED_outSISIDXREG
#define SIS_NEED_orSISIDXREG
#define SIS_NEED_andSISIDXREG
#define SIS_NEED_MYMMIO
#include "sis_regs.h"
#include "sis_dac.h"

static void SiSSave(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiSRestore(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiS300Save(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiS300Restore(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiS315Save(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiS315Restore(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiS301Save(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiS301BSave(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiSLVDSChrontelSave(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiS301Restore(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiS301BRestore(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiSLVDSChrontelRestore(ScrnInfoPtr pScrn, SISRegPtr sisReg);
static void SiS301LoadPalette(ScrnInfoPtr pScrn, int numColors,
                      int *indicies, LOCO *colors, int myshift);
static void SetBlock(CARD16 port, CARD8 from, CARD8 to, CARD8 *DataPtr);

UChar       SiSGetCopyROP(int rop);
UChar       SiSGetPatternROP(int rop);

static const UShort ch700xidx[] = {
      0x00,0x07,0x08,0x0a,0x0b,0x04,0x09,0x20,0x21,0x18,0x19,0x1a,
      0x1b,0x1c,0x1d,0x1e,0x1f,  /* 0x0e,  - Don't save the power register */
      0x01,0x03,0x06,0x0d,0x11,0x13,0x14,0x15,0x17,0x22,0x23,0x24
   };

static const UShort ch701xidx[] = {
      0x1c,0x5f,0x64,0x6f,0x70,0x71,0x72,0x73,0x74,0x76,0x78,0x7d,
      0x67,0x68,0x69,0x6a,0x6b,0x1e,0x00,0x01,0x02,0x04,0x03,0x05,
      0x06,0x07,0x08,0x15,0x1f,0x0c,0x0d,0x0e,0x0f,0x10,0x66
   };

int SiS_compute_vclk(
        int Clock,
        int *out_n,
        int *out_dn,
        int *out_div,
        int *out_sbit,
        int *out_scale)
{
    float f,x,y,t, error, min_error;
    int n, dn, best_n=0, best_dn=0;

    /*
     * Rules
     *
     * VCLK = 14.318 * (Divider/Post Scalar) * (Numerator/DeNumerator)
     * Factor = (Divider/Post Scalar)
     * Divider is 1 or 2
     * Post Scalar is 1, 2, 3, 4, 6 or 8
     * Numberator ranged from 1 to 128
     * DeNumerator ranged from 1 to 32
     * a. VCO = VCLK/Factor, suggest range is 150 to 250 Mhz
     * b. Post Scalar selected from 1, 2, 4 or 8 first.
     * c. DeNumerator selected from 2.
     *
     * According to rule a and b, the VCO ranges that can be scaled by
     * rule b are:
     *      150    - 250    (Factor = 1)
     *       75    - 125    (Factor = 2)
     *       37.5  -  62.5  (Factor = 4)
     *       18.75 -  31.25 (Factor = 8)
     *
     * The following ranges use Post Scalar 3 or 6:
     *      125    - 150    (Factor = 1.5)
     *       62.5  -  75    (Factor = 3)
     *       31.25 -  37.5  (Factor = 6)
     *
     * Steps:
     * 1. divide the Clock by 2 until the Clock is less or equal to 31.25.
     * 2. if the divided Clock is range from 18.25 to 31.25, than
     *    the Factor is 1, 2, 4 or 8.
     * 3. if the divided Clock is range from 15.625 to 18.25, than
     *    the Factor is 1.5, 3 or 6.
     * 4. select the Numberator and DeNumberator with minimum deviation.
     *
     * ** this function can select VCLK ranged from 18.75 to 250 Mhz
     */

    f = (float) Clock;
    f /= 1000.0;
    if((f > 250.0) || (f < 18.75))
       return 0;

    min_error = f;
    y = 1.0;
    x = f;
    while(x > 31.25) {
       y *= 2.0;
       x /= 2.0;
    }
    if(x >= 18.25) {
       x *= 8.0;
       y = 8.0 / y;
    } else if(x >= 15.625) {
       x *= 12.0;
       y = 12.0 / y;
    }

    t = y;
    if(t == (float) 1.5) {
       *out_div = 2;
       t *= 2.0;
    } else {
       *out_div = 1;
    }
    if(t > (float) 4.0) {
       *out_sbit = 1;
       t /= 2.0;
    } else {
       *out_sbit = 0;
    }

    *out_scale = (int) t;

    for(dn = 2; dn <= 32; dn++) {
       for(n = 1; n <= 128; n++) {
          error = x;
          error -= ((float) 14.318 * (float) n / (float) dn);
          if(error < (float) 0)
             error = -error;
          if(error < min_error) {
             min_error = error;
             best_n = n;
             best_dn = dn;
          }
       }
    }
    *out_n = best_n;
    *out_dn = best_dn;
    PDEBUG(ErrorF("SiS_compute_vclk: Clock=%d, n=%d, dn=%d, div=%d, sbit=%d,"
                    " scale=%d\n", Clock, best_n, best_dn, *out_div,
                    *out_sbit, *out_scale));
    return 1;
}

void
SiSCalcClock(ScrnInfoPtr pScrn, int clock, int max_VLD, unsigned int *vclk)
{
    SISPtr pSiS = SISPTR(pScrn);
    int M, N, P , PSN, VLD , PSNx ;
    int bestM=0, bestN=0, bestP=0, bestPSN=0, bestVLD=0;
    double abest = 42.0;
    double target;
    double Fvco, Fout;
    double error, aerror;

    /*
     *  fd = fref*(Numerator/Denumerator)*(Divider/PostScaler)
     *
     *  M       = Numerator [1:128]
     *  N       = DeNumerator [1:32]
     *  VLD     = Divider (Vco Loop Divider) : divide by 1, 2
     *  P       = Post Scaler : divide by 1, 2, 3, 4
     *  PSN     = Pre Scaler (Reference Divisor Select)
     *
     * result in vclk[]
     */
#define Midx    0
#define Nidx    1
#define VLDidx  2
#define Pidx    3
#define PSNidx  4
#define Fref 14318180
/* stability constraints for internal VCO -- MAX_VCO also determines
 * the maximum Video pixel clock */
#define MIN_VCO      Fref
#define MAX_VCO      135000000
#define MAX_VCO_5597 353000000
#define MAX_PSN      0          /* no pre scaler for this chip */
#define TOLERANCE    0.01       /* search smallest M and N in this tolerance */

  int M_min = 2;
  int M_max = 128;

  target = clock * 1000;

  if(pSiS->Chipset == PCI_CHIP_SIS5597 || pSiS->Chipset == PCI_CHIP_SIS6326) {

     int low_N = 2;
     int high_N = 5;

     PSN = 1;
     P = 1;
     if(target < MAX_VCO_5597 / 2)  P = 2;
     if(target < MAX_VCO_5597 / 3)  P = 3;
     if(target < MAX_VCO_5597 / 4)  P = 4;
     if(target < MAX_VCO_5597 / 6)  P = 6;
     if(target < MAX_VCO_5597 / 8)  P = 8;

     Fvco = P * target;

     for(N = low_N; N <= high_N; N++) {

         double M_desired = Fvco / Fref * N;
         if(M_desired > M_max * max_VLD)  continue;

         if(M_desired > M_max) {
            M = M_desired / 2 + 0.5;
            VLD = 2;
         } else {
            M = Fvco / Fref * N + 0.5;
            VLD = 1;
         }

         Fout = (double)Fref * (M * VLD)/(N * P);

         error = (target - Fout) / target;
         aerror = (error < 0) ? -error : error;
         if(aerror < abest) {
            abest = aerror;
            bestM = M;
            bestN = N;
            bestP = P;
            bestPSN = PSN;
            bestVLD = VLD;
         }
     }

  } else {

     for(PSNx = 0; PSNx <= MAX_PSN ; PSNx++) {

        int low_N, high_N;
        double FrefVLDPSN;

        PSN = !PSNx ? 1 : 4;

        low_N = 2;
        high_N = 32;

        for(VLD = 1 ; VLD <= max_VLD ; VLD++) {

           FrefVLDPSN = (double)Fref * VLD / PSN;

	   for(N = low_N; N <= high_N; N++) {
              double tmp = FrefVLDPSN / N;

              for(P = 1; P <= 4; P++) {
                 double Fvco_desired = target * ( P );
                 double M_desired = Fvco_desired / tmp;

                 /* Which way will M_desired be rounded?
                  *  Do all three just to be safe.
                  */
                 int M_low = M_desired - 1;
                 int M_hi = M_desired + 1;

                 if(M_hi < M_min || M_low > M_max) continue;

		 if(M_low < M_min)  M_low = M_min;

		 if(M_hi > M_max)   M_hi = M_max;

                 for(M = M_low; M <= M_hi; M++) {
                    Fvco = tmp * M;
                    if(Fvco <= MIN_VCO) continue;
                    if(Fvco > MAX_VCO)  break;

                    Fout = Fvco / ( P );

                    error = (target - Fout) / target;
                    aerror = (error < 0) ? -error : error;
                    if(aerror < abest) {
                       abest = aerror;
                       bestM = M;
                       bestN = N;
                       bestP = P;
                       bestPSN = PSN;
                       bestVLD = VLD;
                    }
#ifdef TWDEBUG
                    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO,3,
			       "Freq. selected: %.2f MHz, M=%d, N=%d, VLD=%d, P=%d, PSN=%d\n",
                               (float)(clock / 1000.), M, N, P, VLD, PSN);
                    xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO,3,
			       "Freq. set: %.2f MHz\n", Fout / 1.0e6);
#endif
                 }
              }
           }
        }
     }
  }

  vclk[Midx]   = bestM;
  vclk[Nidx]   = bestN;
  vclk[VLDidx] = bestVLD;
  vclk[Pidx]   = bestP;
  vclk[PSNidx] = bestPSN;
}

static void
SiSSave(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr pSiS = SISPTR(pScrn);
    int i, max;

    PDEBUG(xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 3, "SiSSave()\n"));

#ifdef UNLOCK_ALWAYS
    sisSaveUnlockExtRegisterLock(pSiS, NULL, NULL);
#endif

    switch(pSiS->Chipset) {
        case PCI_CHIP_SIS5597:
           max=0x3C;
           break;
        case PCI_CHIP_SIS6326:
        case PCI_CHIP_SIS530:
           max=0x3F;
           break;
        default:
           max=0x37;
    }

    /* Save extended SR registers */
    for(i = 0x00; i <= max; i++) {
       inSISIDXREG(SISSR, i, sisReg->sisRegs3C4[i]);
#ifdef TWDEBUG
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		 "SR%02X - %02X \n", i, sisReg->sisRegs3C4[i]);
#endif
    }

#ifdef TWDEBUG
    for(i = 0x00; i <= 0x3f; i++) {
       inSISIDXREG(SISCR, i, max);
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		 "CR%02X - %02X \n", i, max);
    }
#endif

    /* Save lock (will not be restored in SiSRestore()!) */
    inSISIDXREG(SISCR, 0x80, sisReg->sisRegs3D4[0x80]);

    sisReg->sisRegs3C2 = inSISREG(SISMISCR);	 /* Misc */

    /* Save TV registers */
    if((pSiS->Chipset == PCI_CHIP_SIS6326) && (pSiS->SiS6326Flags & SIS6326_HASTV)) {
       outSISIDXREG(SISCR, 0x80, 0x86);
       for(i = 0x00; i <= 0x44; i++) {
          sisReg->sis6326tv[i] = SiS6326GetTVReg(pScrn, i);
#ifdef TWDEBUG
          xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		 "VR%02X - %02X \n", i,sisReg->sis6326tv[i]);
#endif
       }
    }
}

static void
SiSRestore(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr pSiS = SISPTR(pScrn);
    int i, max;
    UChar tmp;

    PDEBUG(xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 4, "SiSRestore()\n"));

#ifdef UNLOCK_ALWAYS
    sisSaveUnlockExtRegisterLock(pSiS, NULL, NULL);
#endif

    switch(pSiS->Chipset) {
        case PCI_CHIP_SIS5597:
           max = 0x3C;
           break;
        case PCI_CHIP_SIS6326:
	case PCI_CHIP_SIS530:
           max = 0x3F;
           break;
        default:
           max = 0x37;
    }

    /* Disable TV on 6326 before restoring */
    if((pSiS->Chipset == PCI_CHIP_SIS6326) && (pSiS->SiS6326Flags & SIS6326_HASTV)) {
       outSISIDXREG(SISCR, 0x80, 0x86);
       tmp = SiS6326GetTVReg(pScrn, 0x00);
       tmp &= ~0x04;
       SiS6326SetTVReg(pScrn, 0x00, tmp);
    }

    /* Restore other extended SR registers */
    for(i = 0x06; i <= max; i++) {
       if((i == 0x13) || (i == 0x2a) || (i == 0x2b)) continue;
       outSISIDXREG(SISSR, i, sisReg->sisRegs3C4[i]);
    }

    /* Now restore VCLK (with correct SR38 setting) */
    outSISIDXREG(SISSR, 0x13, sisReg->sisRegs3C4[0x13]);
    outSISIDXREG(SISSR, 0x2a, sisReg->sisRegs3C4[0x2a]);
    outSISIDXREG(SISSR, 0x2b, sisReg->sisRegs3C4[0x2b]);

    /* Misc */
    outSISREG(SISMISCW, sisReg->sisRegs3C2);

    /* MemClock needs this to take effect */
    outSISIDXREG(SISSR, 0x00, 0x01);    /* Synchronous Reset */
    usleep(10000);
    outSISIDXREG(SISSR, 0x00, 0x03);    /* End Reset */

    /* Restore TV registers */
    pSiS->SiS6326Flags &= ~SIS6326_TVON;
    if((pSiS->Chipset == PCI_CHIP_SIS6326) && (pSiS->SiS6326Flags & SIS6326_HASTV)) {
       for(i = 0x01; i <= 0x44; i++) {
          SiS6326SetTVReg(pScrn, i, sisReg->sis6326tv[i]);
#ifdef TWDEBUG
          xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"VR%02x restored to %02x\n",
		i, sisReg->sis6326tv[i]);
#endif
       }
       tmp = SiS6326GetXXReg(pScrn, 0x13);
       SiS6326SetXXReg(pScrn, 0x13, 0xfa);
       tmp = SiS6326GetXXReg(pScrn, 0x14);
       SiS6326SetXXReg(pScrn, 0x14, 0xc8);
       if(!(sisReg->sisRegs3C4[0x0D] & 0x04)) {
	  tmp = SiS6326GetXXReg(pScrn, 0x13);
	  SiS6326SetXXReg(pScrn, 0x13, 0xf6);
	  tmp = SiS6326GetXXReg(pScrn, 0x14);
	  SiS6326SetXXReg(pScrn, 0x14, 0xbf);
       }
       if(sisReg->sis6326tv[0] & 0x04) pSiS->SiS6326Flags |= SIS6326_TVON;
    }
}

/* Save SiS 300 series register contents */
static void
SiS300Save(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr pSiS = SISPTR(pScrn);
    int i;

    PDEBUG(xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 3, "SiS300Save()\n"));

#ifdef UNLOCK_ALWAYS
    sisSaveUnlockExtRegisterLock(pSiS, NULL, NULL);
#endif

    /* Save SR registers */
    for(i = 0x00; i <= 0x3D; i++) {
       inSISIDXREG(SISSR, i, sisReg->sisRegs3C4[i]);
#ifdef TWDEBUG
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		 "SR%02X - %02X \n", i,sisReg->sisRegs3C4[i]);
#endif
    }

    /* Save CR registers */
    for(i = 0x00; i < 0x40; i++)  {
       inSISIDXREG(SISCR, i, sisReg->sisRegs3D4[i]);
#ifdef TWDEBUG
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"CR%02X Contents - %02X \n", i,sisReg->sisRegs3D4[i]);
#endif
    }

    /* Save Misc register */
    sisReg->sisRegs3C2 = inSISREG(SISMISCR);

    /* Save FQBQ and GUI timer settings */
    if(pSiS->Chipset == PCI_CHIP_SIS630) {
       sisReg->sisRegsPCI50 = pciReadLong(0x00000000, 0x50);
       sisReg->sisRegsPCIA0 = pciReadLong(0x00000000, 0xA0);
#ifdef TWDEBUG
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"PCI Config 50 = %lx\n", sisReg->sisRegsPCI50);
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"PCI Config A0 = %lx\n", sisReg->sisRegsPCIA0);
#endif
    }

    /* Save panel link/video bridge registers */
#ifndef TWDEBUG
    if(!pSiS->UseVESA) {
#endif
       if(pSiS->VBFlags2 & (VB2_LVDS | VB2_CHRONTEL))
          SiSLVDSChrontelSave(pScrn, sisReg);
       else if(pSiS->VBFlags2 & VB2_301)
          SiS301Save(pScrn, sisReg);
       else if(pSiS->VBFlags2 & VB2_30xBLV)
          SiS301BSave(pScrn, sisReg);
#ifndef TWDEBUG
    }
#endif

    /* Save Mode number */
    sisReg->BIOSModeSave = SiS_GetSetModeID(pScrn,0xFF);

#ifdef TWDEBUG
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	"BIOS mode ds:449 = 0x%x\n", sisReg->BIOSModeSave);
#endif
}

/* Restore SiS300 series register contents */
static void
SiS300Restore(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr pSiS = SISPTR(pScrn);
    int i,temp;
    CARD32 temp1, temp2;

    PDEBUG(xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 4, "SiS300Restore()\n"));

#ifdef UNLOCK_ALWAYS
    sisSaveUnlockExtRegisterLock(pSiS, NULL, NULL);
#endif

    /* Wait for accelerator to finish on-going drawing operations. */
    inSISIDXREG(SISSR, 0x1E, temp);
    if(temp & (0x40|0x10|0x02))  {
       while ( (SIS_MMIO_IN16(pSiS->IOBase, 0x8242) & 0xE000) != 0xE000){};
       while ( (SIS_MMIO_IN16(pSiS->IOBase, 0x8242) & 0xE000) != 0xE000){};
       while ( (SIS_MMIO_IN16(pSiS->IOBase, 0x8242) & 0xE000) != 0xE000){};
    }

    if(!(pSiS->UseVESA)) {
       if(pSiS->VBFlags2 & VB2_LVDS) {
	  SiSRegInit(pSiS->SiS_Pr, pSiS->RelIO + 0x30);
	  SiSSetLVDSetc(pSiS->SiS_Pr, 0);
	  SiS_GetVBType(pSiS->SiS_Pr);
	  SiS_UnLockCRT2(pSiS->SiS_Pr);
	  SiS_DisableBridge(pSiS->SiS_Pr);
       }
    }

    /* Restore extended CR registers */
    for(i = 0x19; i < 0x40; i++) {
       outSISIDXREG(SISCR, i, sisReg->sisRegs3D4[i]);
    }

    if(pSiS->Chipset != PCI_CHIP_SIS300)  {
       UChar val;
       inSISIDXREG(SISCR, 0x1A, val);
       if(val == sisReg->sisRegs3D4[0x19])
	  outSISIDXREG(SISCR, 0x1A, sisReg->sisRegs3D4[0x19]);
       inSISIDXREG(SISCR,0x19,val);
       if(val == sisReg->sisRegs3D4[0x1A])
	  outSISIDXREG(SISCR, 0x19, sisReg->sisRegs3D4[0x1A]);
    }

    /* Set (and leave) PCI_IO_ENABLE on if accelerators are on */
    if(sisReg->sisRegs3C4[0x1e] & 0x50) {
       sisReg->sisRegs3C4[0x20] |= 0x20;
       outSISIDXREG(SISSR, 0x20, sisReg->sisRegs3C4[0x20]);
    }

    /* If TQ is switched on, don't switch it off ever again!
     * Therefore, always restore registers with TQ enabled.
     */
    if((!pSiS->NoAccel) && (pSiS->TurboQueue)) {
       temp = (pScrn->videoRam/64) - 8;
       sisReg->sisRegs3C4[0x26] = temp & 0xFF;
       sisReg->sisRegs3C4[0x27] = ((temp >> 8) & 3) | 0xF0;
    }

    /* Restore extended SR registers */
    for(i = 0x06; i <= 0x3D; i++) {
       temp = sisReg->sisRegs3C4[i];
       if(!(pSiS->UseVESA)) {
	  if(pSiS->VBFlags2 & VB2_LVDS) {
	     if(i == 0x11) {
		inSISIDXREG(SISSR,0x11,temp);
		temp &= 0x0c;
		temp |= (sisReg->sisRegs3C4[i] & 0xf3);
	     }
          }
       }
       outSISIDXREG(SISSR, i, temp);
    }

    /* Restore VCLK and ECLK */
    if(pSiS->VBFlags2 & (VB2_LVDS | VB2_30xB)) {
       outSISIDXREG(SISSR,0x31,0x20);
       outSISIDXREG(SISSR,0x2b,sisReg->sisRegs3C4[0x2b]);
       outSISIDXREG(SISSR,0x2c,sisReg->sisRegs3C4[0x2c]);
       outSISIDXREG(SISSR,0x2d,0x80);
       outSISIDXREG(SISSR,0x31,0x10);
       outSISIDXREG(SISSR,0x2b,sisReg->sisRegs3C4[0x2b]);
       outSISIDXREG(SISSR,0x2c,sisReg->sisRegs3C4[0x2c]);
       outSISIDXREG(SISSR,0x2d,0x80);
    }
    outSISIDXREG(SISSR,0x31,0x00);
    outSISIDXREG(SISSR,0x2b,sisReg->sisRegs3C4[0x2b]);
    outSISIDXREG(SISSR,0x2c,sisReg->sisRegs3C4[0x2c]);
    outSISIDXREG(SISSR,0x2d,0x80);
    if(pSiS->VBFlags2 & (VB2_LVDS | VB2_30xB)) {
       outSISIDXREG(SISSR,0x31,0x20);
       outSISIDXREG(SISSR,0x2e,sisReg->sisRegs3C4[0x2e]);
       outSISIDXREG(SISSR,0x2f,sisReg->sisRegs3C4[0x2f]);
       outSISIDXREG(SISSR,0x31,0x10);
       outSISIDXREG(SISSR,0x2e,sisReg->sisRegs3C4[0x2e]);
       outSISIDXREG(SISSR,0x2f,sisReg->sisRegs3C4[0x2f]);
       outSISIDXREG(SISSR,0x31,0x00);
       outSISIDXREG(SISSR,0x2e,sisReg->sisRegs3C4[0x2e]);
       outSISIDXREG(SISSR,0x2f,sisReg->sisRegs3C4[0x2f]);
    }

    /* Restore Misc register */
    outSISREG(SISMISCW, sisReg->sisRegs3C2);

    /* Restore FQBQ and GUI timer settings */
    if(pSiS->Chipset == PCI_CHIP_SIS630) {
       temp1 = pciReadLong(0x00000000, 0x50);
       temp2 = pciReadLong(0x00000000, 0xA0);
       if(pciReadLong(0x00000000, 0x00) == 0x06301039) {
          temp1 &= 0xf0ffffff;
          temp1 |= (sisReg->sisRegsPCI50 & ~0xf0ffffff);
	  temp2 &= 0xf0ffffff;
          temp2 |= (sisReg->sisRegsPCIA0 & ~0xf0ffffff);
       } else {  /* 730 */
          temp1 &= 0xfffff9ff;
          temp1 |= (sisReg->sisRegsPCI50 & ~0xfffff9ff);
	  temp2 &= 0x00ffffff;
          temp2 |= (sisReg->sisRegsPCIA0 & ~0x00ffffff);
       }
       pciWriteLong(0x00000000, 0x50, temp1);
       pciWriteLong(0x00000000, 0xA0, temp2);
    }

    /* Restore panel link/video bridge registers */
    if(!(pSiS->UseVESA)) {
       if(pSiS->VBFlags2 & (VB2_LVDS | VB2_CHRONTEL))
          SiSLVDSChrontelRestore(pScrn, sisReg);
       else if(pSiS->VBFlags2 & VB2_301)
          SiS301Restore(pScrn, sisReg);
       else if(pSiS->VBFlags2 & VB2_30xBLV)
          SiS301BRestore(pScrn, sisReg);
    }

    /* MemClock needs this to take effect */
    outSISIDXREG(SISSR, 0x00, 0x01);    /* Synchronous Reset */
    outSISIDXREG(SISSR, 0x00, 0x03);    /* End Reset */

    /* Restore mode number */
    SiS_GetSetModeID(pScrn,sisReg->BIOSModeSave);
}

/* Save SiS315 series register contents */
static void
SiS315Save(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr pSiS = SISPTR(pScrn);
    int i, max;

    PDEBUG(xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 3, "SiS315Save()\n"));

#ifdef UNLOCK_ALWAYS
    sisSaveUnlockExtRegisterLock(pSiS, NULL, NULL);
#endif

    /* Save SR registers */
    for(i = 0x00; i <= 0x60; i++) {
       inSISIDXREG(SISSR, i, sisReg->sisRegs3C4[i]);
#ifdef TWDEBUG
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			 "SR%02X - %02X \n", i,sisReg->sisRegs3C4[i]);
#endif
    }

    /* Save command queue location */
    sisReg->sisMMIO85C0 = SIS_MMIO_IN32(pSiS->IOBase, 0x85C0);

    /* Save CR registers */
    max = 0x7c;
    if(pSiS->ChipType >= XGI_20) max = 0xff;
    for(i = 0x00; i <= max; i++)  {
       inSISIDXREG(SISCR, i, sisReg->sisRegs3D4[i]);
#ifdef TWDEBUG
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"CR%02X Contents - %02X \n", i,sisReg->sisRegs3D4[i]);
#endif
    }

    /* Save video capture registers */
    for(i = 0x00; i <= 0x4f; i++)  {
       inSISIDXREG(SISCAP, i, sisReg->sisCapt[i]);
#ifdef TWDEBUG_VID
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Capt%02X Contents - %02X \n", i,sisReg->sisCapt[i]);
#endif
    }

    /* Save video playback registers */
    for(i = 0x00; i <= 0x3f; i++) {
       inSISIDXREG(SISVID, i, sisReg->sisVid[i]);
#ifdef TWDEBUG_VID
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Vid%02X Contents - %02X \n", i,sisReg->sisVid[i]);
#endif
    }

    /* Save Misc register */
    sisReg->sisRegs3C2 = inSISREG(SISMISCR);

    /* Save panel link/video bridge registers */
#ifndef TWDEBUG
    if(!pSiS->UseVESA) {
#endif
       if(pSiS->VBFlags2 & (VB2_LVDS | VB2_CHRONTEL))
          SiSLVDSChrontelSave(pScrn, sisReg);
       else if(pSiS->VBFlags2 & VB2_301)
          SiS301Save(pScrn, sisReg);
       else if(pSiS->VBFlags2 & VB2_30xBLV)
          SiS301BSave(pScrn, sisReg);
#ifndef TWDEBUG
    }
#endif

    /* Save mode number */
    sisReg->BIOSModeSave = SiS_GetSetModeID(pScrn,0xFF);

#ifdef TWDEBUG
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
    	"BIOS mode ds:449 = 0x%x\n", sisReg->BIOSModeSave);
#endif
}

/* Restore SiS315/330 series register contents */
static void
SiS315Restore(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr pSiS = SISPTR(pScrn);
    int i,temp;

    PDEBUG(xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 4, "SiS315Restore()\n"));

#ifdef UNLOCK_ALWAYS
    sisSaveUnlockExtRegisterLock(pSiS, NULL, NULL);
#endif

    /* Wait for accelerator to finish on-going drawing operations. */
    inSISIDXREG(SISSR, 0x1E, temp);
    if(temp & (0x40|0x10|0x02))  {	/* 0x40 = 2D, 0x02 = 3D enabled*/
       while ( (SIS_MMIO_IN32(pSiS->IOBase, 0x85CC) & 0x80000000) != 0x80000000){};
       while ( (SIS_MMIO_IN32(pSiS->IOBase, 0x85CC) & 0x80000000) != 0x80000000){};
       while ( (SIS_MMIO_IN32(pSiS->IOBase, 0x85CC) & 0x80000000) != 0x80000000){};
    }

    /* We reset the command queue before restoring.
     * This might be required because we never know what
     * console driver (like the kernel framebuffer driver)
     * or application is running and which queue mode it
     * uses.
     */
    andSISIDXREG(SISCR, 0x55, 0x33);
    orSISIDXREG(SISSR, 0x26, 0x01);
    outSISIDXREG(SISSR, 0x27, 0x1F);

    /* Restore extended CR registers */
    for(i = 0x19; i < 0x5C; i++) {
       outSISIDXREG(SISCR, i, sisReg->sisRegs3D4[i]);
    }
    if(pSiS->ChipType < SIS_661) {
       outSISIDXREG(SISCR, 0x79, sisReg->sisRegs3D4[0x79]);
    }
    outSISIDXREG(SISCR, pSiS->myCR63, sisReg->sisRegs3D4[pSiS->myCR63]);

    /* Leave PCI_IO_ENABLE on if accelerators are on (Is this required?) */
    if(sisReg->sisRegs3C4[0x1e] & 0x52) {  /* 0x40=2D, 0x02=3D */
       sisReg->sisRegs3C4[0x20] |= 0x20;
       outSISIDXREG(SISSR, 0x20, sisReg->sisRegs3C4[0x20]);
    }

    if(pSiS->SiS_Pr->SiS_SensibleSR11) {
       sisReg->sisRegs3C4[0x11] &= 0x0f;
    }

    /* Restore extended SR registers */
    for(i = 0x06; i <= 0x3F; i++) {
       if(i == 0x26) {
          continue;
       } else if(i == 0x27) {
          outSISIDXREG(SISSR, 0x27, sisReg->sisRegs3C4[0x27]);
          outSISIDXREG(SISSR, 0x26, sisReg->sisRegs3C4[0x26]);
       } else {
          outSISIDXREG(SISSR, i, sisReg->sisRegs3C4[i]);
       }
    }

    /* Restore VCLK and ECLK */
    andSISIDXREG(SISSR,0x31,0xcf);
    if(pSiS->VBFlags2 & VB2_LVDS) {
       orSISIDXREG(SISSR,0x31,0x20);
       outSISIDXREG(SISSR,0x2b,sisReg->sisRegs3C4[0x2b]);
       outSISIDXREG(SISSR,0x2c,sisReg->sisRegs3C4[0x2c]);
       outSISIDXREG(SISSR,0x2d,0x80);
       andSISIDXREG(SISSR,0x31,0xcf);
       orSISIDXREG(SISSR,0x31,0x10);
       outSISIDXREG(SISSR,0x2b,sisReg->sisRegs3C4[0x2b]);
       outSISIDXREG(SISSR,0x2c,sisReg->sisRegs3C4[0x2c]);
       outSISIDXREG(SISSR,0x2d,0x80);
       andSISIDXREG(SISSR,0x31,0xcf);
       outSISIDXREG(SISSR,0x2b,sisReg->sisRegs3C4[0x2b]);
       outSISIDXREG(SISSR,0x2c,sisReg->sisRegs3C4[0x2c]);
       outSISIDXREG(SISSR,0x2d,0x01);
       outSISIDXREG(SISSR,0x31,0x20);
       outSISIDXREG(SISSR,0x2e,sisReg->sisRegs3C4[0x2e]);
       outSISIDXREG(SISSR,0x2f,sisReg->sisRegs3C4[0x2f]);
       outSISIDXREG(SISSR,0x31,0x10);
       outSISIDXREG(SISSR,0x2e,sisReg->sisRegs3C4[0x2e]);
       outSISIDXREG(SISSR,0x2f,sisReg->sisRegs3C4[0x2f]);
       outSISIDXREG(SISSR,0x31,0x00);
       outSISIDXREG(SISSR,0x2e,sisReg->sisRegs3C4[0x2e]);
       outSISIDXREG(SISSR,0x2f,sisReg->sisRegs3C4[0x2f]);
    } else {
       outSISIDXREG(SISSR,0x2b,sisReg->sisRegs3C4[0x2b]);
       outSISIDXREG(SISSR,0x2c,sisReg->sisRegs3C4[0x2c]);
       outSISIDXREG(SISSR,0x2d,0x01);
    }

#ifndef SISVRAMQ
    /* Initialize read/write pointer for command queue */
    SIS_MMIO_OUT32(pSiS->IOBase, 0x85C4, SIS_MMIO_IN32(pSiS->IOBase, 0x85C8));
#endif
    /* Restore queue location */
    SIS_MMIO_OUT32(pSiS->IOBase, 0x85C0, sisReg->sisMMIO85C0);

    /* Restore Misc register */
    outSISREG(SISMISCW, sisReg->sisRegs3C2);

    /* Restore panel link/video bridge registers */
    if(!(pSiS->UseVESA)) {
       if(pSiS->VBFlags2 & (VB2_LVDS | VB2_CHRONTEL))
          SiSLVDSChrontelRestore(pScrn, sisReg);
       else if(pSiS->VBFlags2 & VB2_301)
          SiS301Restore(pScrn, sisReg);
       else if(pSiS->VBFlags2 & VB2_30xBLV)
          SiS301BRestore(pScrn, sisReg);
    }

    /* MemClock needs this to take effect */
    outSISIDXREG(SISSR, 0x00, 0x01);    /* Synchronous Reset */
    outSISIDXREG(SISSR, 0x00, 0x03);    /* End Reset */

    /* Restore Mode number */
    SiS_GetSetModeID(pScrn,sisReg->BIOSModeSave);
}

static void
SiSVBSave(ScrnInfoPtr pScrn, SISRegPtr sisReg, int p1, int p2, int p3, int p4)
{
    SISPtr  pSiS = SISPTR(pScrn);
    int     i;

    for(i=0; i<=p1; i++)  {
       inSISIDXREG(SISPART1, i, sisReg->VBPart1[i]);
#ifdef TWDEBUG
       xf86DrvMsg(0, X_INFO, "301xSave: Part1 0x%02x = 0x%02x\n", i, sisReg->VBPart1[i]);
#endif
    }
    for(i=0; i<=p2; i++)  {
       inSISIDXREG(SISPART2, i, sisReg->VBPart2[i]);
#ifdef TWDEBUG
       xf86DrvMsg(0, X_INFO, "301xSave: Part2 0x%02x = 0x%02x\n", i, sisReg->VBPart2[i]);
#endif
    }
    for(i=0; i<=p3; i++)  {
       inSISIDXREG(SISPART3, i, sisReg->VBPart3[i]);
#ifdef TWDEBUG
       xf86DrvMsg(0, X_INFO, "301xSave: Part3 0x%02x = 0x%02x\n", i, sisReg->VBPart3[i]);
#endif
    }
    for(i=0; i<=p4; i++)  {
       inSISIDXREG(SISPART4, i, sisReg->VBPart4[i]);
#ifdef TWDEBUG
       xf86DrvMsg(0, X_INFO, "301xSave: Part4 0x%02x = 0x%02x\n", i, sisReg->VBPart4[i]);
#endif
    }
}

/* Save SiS301 bridge register contents */
static void
SiS301Save(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr  pSiS = SISPTR(pScrn);
    int     Part1max, Part2max, Part3max, Part4max;

    /* Highest register number to save/restore */
    if(pSiS->VGAEngine == SIS_300_VGA) Part1max = 0x1d;
    else Part1max = 0x2e;  /* 0x23, but we also need 2d-2e */

    Part2max = 0x45;
    Part3max = 0x3e;
    Part4max = 0x1b;

    SiSVBSave(pScrn, sisReg, Part1max, Part2max, Part3max, Part4max);

    sisReg->VBPart2[0x00] &= ~0x20;      /* Disable VB Processor */
    sisReg->sisRegs3C4[0x32] &= ~0x20;   /* Disable Lock Mode */
}

/* Restore SiS301 bridge register contents */
static void
SiS301Restore(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr  pSiS = SISPTR(pScrn);
    int     Part1max, Part2max, Part3max, Part4max;

    /* Highest register number to save/restore */
    if(pSiS->VGAEngine == SIS_300_VGA) Part1max = 0x1d;
    else Part1max = 0x23;

    Part2max = 0x45;
    Part3max = 0x3e;
    Part4max = 0x1b;

    SiSRegInit(pSiS->SiS_Pr, pSiS->RelIO + 0x30);
    SiSSetLVDSetc(pSiS->SiS_Pr, 0);
    SiS_GetVBType(pSiS->SiS_Pr);
    SiS_DisableBridge(pSiS->SiS_Pr);
    SiS_UnLockCRT2(pSiS->SiS_Pr);

    /* Pre-restore Part1 */
    outSISIDXREG(SISPART1, 0x04, 0x00);
    outSISIDXREG(SISPART1, 0x05, 0x00);
    outSISIDXREG(SISPART1, 0x06, 0x00);
    outSISIDXREG(SISPART1, 0x00, sisReg->VBPart1[0]);
    outSISIDXREG(SISPART1, 0x01, sisReg->VBPart1[1]);

    /* Pre-restore Part4 */
    outSISIDXREG(SISPART4, 0x0D, sisReg->VBPart4[0x0D]);
    outSISIDXREG(SISPART4, 0x0C, sisReg->VBPart4[0x0C]);

    if((!(sisReg->sisRegs3D4[0x30] & 0x03)) &&
       (sisReg->sisRegs3D4[0x31] & 0x20))  {      /* disable CRT2 */
       SiS_LockCRT2(pSiS->SiS_Pr);
       return;
    }

    /* Restore Part1 */
    SetBlock(SISPART1, 0x02, Part1max, &(sisReg->VBPart1[0x02]));
    if(pSiS->VGAEngine == SIS_315_VGA) {
       /* Restore extra registers on 315 series */
       SetBlock(SISPART1, 0x2C, 0x2E, &(sisReg->VBPart1[0x2C]));
    }

    /* Restore Part2 */
    SetBlock(SISPART2, 0x00, Part2max, &(sisReg->VBPart2[0x00]));

    /* Restore Part3 */
    SetBlock(SISPART3, 0x00, Part3max, &(sisReg->VBPart3[0x00]));

    /* Restore Part4 */
    SetBlock(SISPART4, 0x0E, 0x11, &(sisReg->VBPart4[0x0E]));
    SetBlock(SISPART4, 0x13, Part4max, &(sisReg->VBPart4[0x13]));

    /* Post-restore Part4 (CRT2VCLK) */
    outSISIDXREG(SISPART4, 0x0A, 0x01);
    outSISIDXREG(SISPART4, 0x0B, sisReg->VBPart4[0x0B]);
    outSISIDXREG(SISPART4, 0x0A, sisReg->VBPart4[0x0A]);
    outSISIDXREG(SISPART4, 0x12, 0x00);
    outSISIDXREG(SISPART4, 0x12, sisReg->VBPart4[0x12]);

    SiS_EnableBridge(pSiS->SiS_Pr);
    SiS_DisplayOn(pSiS->SiS_Pr);
    SiS_LockCRT2(pSiS->SiS_Pr);
}

/* Save SiS30xB/30xLV bridge register contents */
static void
SiS301BSave(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr  pSiS = SISPTR(pScrn);
    int     Part1max, Part2max, Part3max, Part4max;

    Part1max = 0x60;
    Part2max = 0x4d;
    Part3max = 0x3e;
    Part4max = 0x23;
    if(pSiS->VBFlags2 & (VB2_301LV | VB2_302LV)) {
       Part4max = 0x34;
    } else if(pSiS->VBFlags2 & (VB2_301C | VB2_302ELV)) {
       Part2max = 0xff;
       Part4max = 0x3c;
    } /* TODO for 307 */

    SiSVBSave(pScrn, sisReg, Part1max, Part2max, Part3max, Part4max);

    sisReg->VBPart2[0x00] &= ~0x20;      /* Disable VB Processor */
    sisReg->sisRegs3C4[0x32] &= ~0x20;   /* Disable Lock Mode */
}

/* Restore SiS30xB/30xLV bridge register contents */
static void
SiS301BRestore(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr  pSiS = SISPTR(pScrn);
    int     Part1max, Part2max, Part3max, Part4max;

    Part1max = 0x23;
    Part2max = 0x4d;
    Part3max = 0x3e;
    Part4max = 0x22;
    if(pSiS->VBFlags2 & (VB2_301LV|VB2_302LV)) {
       Part4max = 0x34;
    } else if(pSiS->VBFlags2 & (VB2_301C|VB2_302ELV)) {
       Part2max = 0xff;
       Part4max = 0x3c;
    } /* TODO for 307 */

    SiSRegInit(pSiS->SiS_Pr, pSiS->RelIO + 0x30);
    SiSSetLVDSetc(pSiS->SiS_Pr, 0);
    SiS_GetVBType(pSiS->SiS_Pr);
    SiS_DisableBridge(pSiS->SiS_Pr);
    SiS_UnLockCRT2(pSiS->SiS_Pr);

    /* Pre-restore Part1 */
    outSISIDXREG(SISPART1, 0x04, 0x00);
    outSISIDXREG(SISPART1, 0x05, 0x00);
    outSISIDXREG(SISPART1, 0x06, 0x00);
    outSISIDXREG(SISPART1, 0x00, sisReg->VBPart1[0x00]);
    outSISIDXREG(SISPART1, 0x01, sisReg->VBPart1[0x01]);
    /* Mode reg 0x01 became 0x2e on 315 series (0x01 still contains FIFO) */
    if(pSiS->VGAEngine == SIS_315_VGA) {
       outSISIDXREG(SISPART1, 0x2e, sisReg->VBPart1[0x2e]);
    }

    /* Pre-restore Part4 */
    outSISIDXREG(SISPART4, 0x0D, sisReg->VBPart4[0x0D]);
    outSISIDXREG(SISPART4, 0x0C, sisReg->VBPart4[0x0C]);

    if((!(sisReg->sisRegs3D4[0x30] & 0x03)) &&
       (sisReg->sisRegs3D4[0x31] & 0x20)) {      /* disable CRT2 */
       SiS_LockCRT2(pSiS->SiS_Pr);
       return;
    }

    /* Restore Part1  */
    SetBlock(SISPART1, 0x02, Part1max, &(sisReg->VBPart1[0x02]));
    if(pSiS->VGAEngine == SIS_315_VGA) {
       SetBlock(SISPART1, 0x2C, 0x2D, &(sisReg->VBPart1[0x2C]));
       SetBlock(SISPART1, 0x35, 0x37, &(sisReg->VBPart1[0x35]));
       if((pSiS->ChipFlags & SiSCF_Is65x) || (pSiS->ChipType >= SIS_661)) {
          outSISIDXREG(SISPART1, 0x4c, sisReg->VBPart1[0x4c]);
       }
       outSISIDXREG(SISPART1, 0x2e, sisReg->VBPart1[0x2e] & 0x7f);
    }

    /* Restore Part2 */
    SetBlock(SISPART2, 0x00, Part2max, &(sisReg->VBPart2[0x00]));

    /* Restore Part3 */
    SetBlock(SISPART3, 0x00, Part3max, &(sisReg->VBPart3[0x00]));

    /* Restore Part4 */
    SetBlock(SISPART4, 0x0E, 0x11, &(sisReg->VBPart4[0x0E]));
    SetBlock(SISPART4, 0x13, Part4max, &(sisReg->VBPart4[0x13]));

    /* Post-restore Part4 (CRT2VCLK) */
    outSISIDXREG(SISPART4, 0x0A, sisReg->VBPart4[0x0A]);
    outSISIDXREG(SISPART4, 0x0B, sisReg->VBPart4[0x0B]);
    outSISIDXREG(SISPART4, 0x12, 0x00);
    outSISIDXREG(SISPART4, 0x12, sisReg->VBPart4[0x12]);

    SiS_EnableBridge(pSiS->SiS_Pr);
    SiS_DisplayOn(pSiS->SiS_Pr);
    SiS_LockCRT2(pSiS->SiS_Pr);
}

/* Save LVDS bridge (+ Chrontel) register contents */
static void
SiSLVDSChrontelSave(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr  pSiS = SISPTR(pScrn);
    int     i;

    /* Save Part1 */
    for(i=0; i<0x46; i++) {
       inSISIDXREG(SISPART1, i, sisReg->VBPart1[i]);
#ifdef TWDEBUG
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	                "LVDSSave: Part1Port 0x%02x = 0x%02x\n",
			i, sisReg->VBPart1[i]);
#endif
    }

    /* Save Chrontel registers */
    if(pSiS->VBFlags2 & VB2_CHRONTEL) {
       if(pSiS->ChrontelType == CHRONTEL_700x) {
          for(i=0; i<0x1D; i++)  {
             sisReg->ch70xx[i] = SiS_GetCH700x(pSiS->SiS_Pr, ch700xidx[i]);
#ifdef TWDEBUG
	     xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	                "LVDSSave: Chrontel 0x%02x = 0x%02x\n",
			ch700xidx[i], sisReg->ch70xx[i]);
#endif
	  }
       } else {
          for(i=0; i<35; i++)  {
             sisReg->ch70xx[i] = SiS_GetCH701x(pSiS->SiS_Pr, ch701xidx[i]);
#ifdef TWDEBUG
	     xf86DrvMsg(pScrn->scrnIndex, X_INFO,
	                "LVDSSave: Chrontel 0x%02x = 0x%02x\n",
			ch701xidx[i], sisReg->ch70xx[i]);
#endif
          }
       }
    }

    sisReg->sisRegs3C4[0x32] &= ~0x20;      /* Disable Lock Mode */
}

/* Restore LVDS bridge (+ Chrontel) register contents */
static void
SiSLVDSChrontelRestore(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
    SISPtr pSiS = SISPTR(pScrn);
    int i;

    SiSRegInit(pSiS->SiS_Pr, pSiS->RelIO + 0x30);
    SiSSetLVDSetc(pSiS->SiS_Pr, 0);
    SiS_GetVBType(pSiS->SiS_Pr);
    SiS_DisableBridge(pSiS->SiS_Pr);
    if(pSiS->ChipType == SIS_730) {
       outSISIDXREG(SISPART1, 0x00, 0x80);
    }
    SiS_UnLockCRT2(pSiS->SiS_Pr);

    if(pSiS->VBFlags2 & VB2_CHRONTEL) {
       /* Restore Chrontel registers */
       if(pSiS->ChrontelType == CHRONTEL_700x) {
          for(i=0; i<0x11; i++) {
             SiS_SetCH700x(pSiS->SiS_Pr, ch700xidx[i] & 0xFF, sisReg->ch70xx[i]);
          }
       } else {
          for(i=0; i<34; i++) {
             SiS_SetCH701x(pSiS->SiS_Pr, ch701xidx[i] & 0xFF, sisReg->ch70xx[i]);
          }
       }
    }

    /* pre-restore Part1 */
    outSISIDXREG(SISPART1, 0x04, 0x00);
    outSISIDXREG(SISPART1, 0x05, 0x00);
    outSISIDXREG(SISPART1, 0x06, 0x00);
    outSISIDXREG(SISPART1, 0x00, sisReg->VBPart1[0]);
    if(pSiS->VGAEngine == SIS_300_VGA) {
       outSISIDXREG(SISPART1, 0x01, (sisReg->VBPart1[1] | 0x80));
    } else {
       outSISIDXREG(SISPART1, 0x01, sisReg->VBPart1[1]);
    }

    if((!(sisReg->sisRegs3D4[0x30] & 0x03)) &&
       (sisReg->sisRegs3D4[0x31] & 0x20)) {      /* disable CRT2 */
       SiS_LockCRT2(pSiS->SiS_Pr);
       return;
    }

    /* Restore Part1 */
    if(pSiS->VGAEngine == SIS_300_VGA) {
       outSISIDXREG(SISPART1, 0x02, (sisReg->VBPart1[2] | 0x40));
    } else {
       outSISIDXREG(SISPART1, 0x02, sisReg->VBPart1[2]);
    }
    SetBlock(SISPART1, 0x03, 0x23, &(sisReg->VBPart1[0x03]));
    if(pSiS->VGAEngine == SIS_315_VGA) {
       SetBlock(SISPART1, 0x2C, 0x2E, &(sisReg->VBPart1[0x2C]));
       SetBlock(SISPART1, 0x35, 0x37, &(sisReg->VBPart1[0x35]));  /* Panel Link Scaler */
    }

    /* For 550 DSTN registers */
    if(pSiS->DSTN || pSiS->FSTN) {
       SetBlock(SISPART1, 0x25, 0x2E, &(sisReg->VBPart1[0x25]));
       SetBlock(SISPART1, 0x30, 0x45, &(sisReg->VBPart1[0x30]));
    }

    SiS_EnableBridge(pSiS->SiS_Pr);
    SiS_DisplayOn(pSiS->SiS_Pr);
    SiS_LockCRT2(pSiS->SiS_Pr);
}

/* Restore output selection registers */
void
SiSRestoreBridge(ScrnInfoPtr pScrn, SISRegPtr sisReg)
{
   SISPtr pSiS = SISPTR(pScrn);
   int i;

#ifdef UNLOCK_ALWAYS
   sisSaveUnlockExtRegisterLock(pSiS, NULL, NULL);
#endif

   for(i = 0x30; i <= 0x3b; i++) {
      if(i == 0x34) continue;
      outSISIDXREG(SISCR, i, sisReg->sisRegs3D4[i]);
   }

   if(pSiS->VGAEngine == SIS_315_VGA) {
      outSISIDXREG(SISCR, pSiS->myCR63, sisReg->sisRegs3D4[pSiS->myCR63]);
      if(pSiS->ChipType < SIS_661) {
         outSISIDXREG(SISCR, 0x79, sisReg->sisRegs3D4[0x79]);
      }
   }
}

/* Auxiliary function to find real memory clock (in Khz) */
/* Not for 530/620 if UMA (on these, the mclk is stored in SR10) */
int
SiSMclk(SISPtr pSiS)
{
    int mclk=0;
    UChar Num, Denum, Base;

    switch (pSiS->Chipset)  {

    case PCI_CHIP_SIS300:
    case PCI_CHIP_SIS540:
    case PCI_CHIP_SIS630:
    case PCI_CHIP_SIS315:
    case PCI_CHIP_SIS315H:
    case PCI_CHIP_SIS315PRO:
    case PCI_CHIP_SIS550:
    case PCI_CHIP_SIS650:
    case PCI_CHIP_SIS330:
    case PCI_CHIP_SIS660:
    case PCI_CHIP_SIS340:
    case PCI_CHIP_XGIXG20:
    case PCI_CHIP_XGIXG40:

	/* Numerator */
	inSISIDXREG(SISSR, 0x28, Num);
	mclk = 14318 * ((Num & 0x7f) + 1);

	/* Denumerator */
	inSISIDXREG(SISSR, 0x29, Denum);
	mclk = mclk / ((Denum & 0x1f) + 1);

	/* Divider */
	if((Num & 0x80) != 0)  mclk *= 2;

	/* Post-Scaler */
	if((Denum & 0x80) == 0) {
	   mclk = mclk / (((Denum & 0x60) >> 5) + 1);
	} else {
	   mclk = mclk / ((((Denum & 0x60) >> 5) + 1) * 2);
	}
	break;

    case PCI_CHIP_SIS5597:
    case PCI_CHIP_SIS6326:
    case PCI_CHIP_SIS530:
    default:
	/* Numerator */
	inSISIDXREG(SISSR, 0x28, Num);
	mclk = 14318 * ((Num & 0x7f) + 1);

	/* Denumerator */
	inSISIDXREG(SISSR, 0x29, Denum);
	mclk = mclk / ((Denum & 0x1f) + 1);

	/* Divider. Doesn't work on older cards */
	if(pSiS->oldChipset >= OC_SIS5597) {
	   if(Num & 0x80) mclk *= 2;
	}

	/* Post-scaler. Values' meaning depends on SR13 bit 7  */
	inSISIDXREG(SISSR, 0x13, Base);
	if((Base & 0x80) == 0) {
	   mclk = mclk / (((Denum & 0x60) >> 5) + 1);
	} else {
	   /* Values 00 and 01 are reserved */
	   if ((Denum & 0x60) == 0x40)  mclk /= 6;
	   if ((Denum & 0x60) == 0x60)  mclk /= 8;
	}
	break;
    }

    return(mclk);
}

/* This estimates the CRT2 clock we are going to use.
 * The total bandwidth is to be reduced by the value
 * returned here in order to get an idea of the maximum
 * dotclock left for CRT1.
 * Since we don't know yet, what mode the user chose,
 * we return the maximum dotclock used by
 * - either the LCD attached, or
 * - TV
 * For VGA2, we share the bandwith equally.
 */
static int
SiSEstimateCRT2Clock(ScrnInfoPtr pScrn, Bool FakeForCRT2)
{
	SISPtr pSiS = SISPTR(pScrn);

	if(pSiS->VBFlags & CRT2_LCD) {
	   if(pSiS->VBLCDFlags & (VB_LCD_320x480 | VB_LCD_800x600 | VB_LCD_640x480)) {
	      return 40000;
	   } else if(pSiS->VBLCDFlags & (VB_LCD_1024x768 | VB_LCD_1024x600 | VB_LCD_1152x768)) {
	      return 65000;
	   } else if(pSiS->VBLCDFlags & VB_LCD_1280x720) {
	      /* Fake clock; VGA (4:3) mode is 108, but uses only 75 for LCD */
	      if(FakeForCRT2) return 108000;
	      else            return 75000;
	   } else if(pSiS->VBLCDFlags & VB_LCD_1280x768) {
	      /* Fake clock; VGA (4:3) mode is 108, but uses only 81 for LCD */
	      if(FakeForCRT2) return 108000;
	      else            return 81000;
	   } else if(pSiS->VBLCDFlags & VB_LCD_1280x800) {
	      /* Fake clock; VGA (4:3) mode is 108, but uses only 83 for LCD */
	      if(FakeForCRT2) return 108000;
	      else            return 83000;
	   } else if(pSiS->VBLCDFlags & VB_LCD_1280x854) {
	      /* Fake clock; VGA (4:3) mode is 108, but uses only 84 for LCD */
	      if(FakeForCRT2) return 108000;
	      else            return 84000;
	   } else if(pSiS->VBLCDFlags & (VB_LCD_1280x1024 | VB_LCD_1280x960)) {
	      return 108000;
	   } else if(pSiS->VBLCDFlags & VB_LCD_1400x1050) {
	      /* Fake clock; VGA mode is 122, but uses only 108 for LCD */
	      if(FakeForCRT2) return 123000;
	      else            return 108000;
	   } else if(pSiS->VBLCDFlags & VB_LCD_1680x1050) {
	      /* Fake clock; VGA mode is 147, but uses only 122 for LCD */
	      if(FakeForCRT2) return 148000;
	      else            return 122000;
	   } else if(pSiS->VBLCDFlags & VB_LCD_1600x1200) {
	      return 162000;
	   } else if((pSiS->VBLCDFlags & VB_LCD_CUSTOM) && (pSiS->SiS_Pr->CP_MaxClock)) {
	      return pSiS->SiS_Pr->CP_MaxClock;
	   } else {
	      if(pSiS->VBFlags2 & VB2_30xC) return 162000;
	      else                          return 108000;
	   }
	} else if(pSiS->VBFlags & CRT2_TV) {
	   if(pSiS->VBFlags2 & VB2_CHRONTEL) {
	      switch(pSiS->VGAEngine) {
	      case SIS_300_VGA:
		 return 50000;	/* 700x: <= 800x600 */
	      case SIS_315_VGA:
	      default:
		 return 70000;  /* 701x: <= 1024x768 */
	      }
	   } else if(pSiS->VBFlags2 & VB2_SISBRIDGE) {
	      if(pSiS->SiS_SD_Flags & (SiS_SD_SUPPORTYPBPR|SiS_SD_SUPPORTHIVISION)) {
	         if(FakeForCRT2) return 108000;  /* 1280x1024@60 (faked) */
	         else            return 75000;   /* Really used clock */
	      } else {
		 return 70000;
	      }
	   }
	}

	return 0;
}

/* Calculate the maximum dotclock */
int SiSMemBandWidth(ScrnInfoPtr pScrn, Bool IsForCRT2)
{
	SISPtr pSiS = SISPTR(pScrn);
#ifdef SISDUALHEAD
	SISEntPtr pSiSEnt = pSiS->entityPrivate;
#endif
	int          bus = pSiS->BusWidth;
	int          mclk = pSiS->MemClock;
	int          bpp = pSiS->CurrentLayout.bitsPerPixel;
	int	     max = 0;
	float        magic = 0.0, total;
	int          bytesperpixel = (bpp + 7) / 8;
	float  	     crt2used, maxcrt2;
	int	     crt2clock;
	Bool	     DHM, GetForCRT1;
#ifdef __SUNPRO_C
#define const
#endif
	const float  magicDED[4] = { 1.2,      1.368421, 2.263158, 1.2};
	const float  magicINT[4] = { 1.441177, 1.441177, 2.588235, 1.441177 };
#ifdef __SUNPRO_C
#undef const
#endif

	switch(pSiS->Chipset) {

	case PCI_CHIP_SIS5597:
		total = ((mclk * (bus / 8)) * 0.7) / bytesperpixel;
		if(total > 135000) total = 135000;
		xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			"Maximum pixel clock at %d bpp is %g MHz\n",
			bpp, total/1000);
		return(int)(total);

	case PCI_CHIP_SIS6326:
		total = ((mclk * (bus / 8)) * 0.7) / bytesperpixel;
		if(total > 175500) total = 175500;
		xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			"Maximum pixel clock at %d bpp is %g MHz\n",
			bpp, total/1000);
		return(int)(total);

	case PCI_CHIP_SIS530:
		total = ((mclk * (bus / 8)) * 0.7) / bytesperpixel;
		if(total > 230000) total = 230000;
		xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			"Maximum pixel clock at %d bpp is %g MHz\n",
			bpp, total/1000);
		return(int)(total);

	case PCI_CHIP_SIS300:
	case PCI_CHIP_SIS540:
	case PCI_CHIP_SIS630:
	case PCI_CHIP_SIS315:
	case PCI_CHIP_SIS315H:
	case PCI_CHIP_SIS315PRO:
	case PCI_CHIP_SIS550:
	case PCI_CHIP_SIS650:
	case PCI_CHIP_SIS330:
	case PCI_CHIP_SIS660:
	case PCI_CHIP_SIS340:
	case PCI_CHIP_XGIXG20:
	case PCI_CHIP_XGIXG40:
		switch(pSiS->Chipset) {
		case PCI_CHIP_SIS300:
		    magic = magicDED[bus/64];
		    max = 540000;
		    break;
		case PCI_CHIP_SIS540:
		case PCI_CHIP_SIS630:
		    magic = magicINT[bus/64];
		    max = 540000;
		    break;
		case PCI_CHIP_SIS315:
		case PCI_CHIP_SIS315H:
		case PCI_CHIP_SIS315PRO:
		case PCI_CHIP_SIS330:
		    magic = magicDED[bus/64];
		    max = 780000;
		    break;
		case PCI_CHIP_SIS550:
		    magic = magicINT[bus/64];
		    max = 610000;
		    break;
		case PCI_CHIP_SIS650:
		    magic = magicINT[bus/64];
		    max = 680000;
		    break;
		case PCI_CHIP_SIS660:
		    if((pSiS->ChipType >= SIS_660) &&
		       (pSiS->ChipFlags & SiSCF_760LFB)) {
		       magic = magicDED[bus/64];
		    } else {
		       magic = magicINT[bus/64];
		    }
		    max = 680000;
		case PCI_CHIP_SIS340:
		case PCI_CHIP_XGIXG40:
		    magic = magicDED[bus/64];
		    max = 800000;
		    break;
		case PCI_CHIP_XGIXG20:
		    magic = 1.0; /* magicDED[bus/64]; */
		    max = 332000;
		    break;
		}

		PDEBUG(ErrorF("mclk: %d, bus: %d, magic: %g, bpp: %d\n",
				mclk, bus, magic, bpp));

		total = mclk * bus / bpp;

		xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			"Memory bandwidth at %d bpp is %g MHz\n", bpp, total/1000);

		if((pSiS->VBFlags & CRT2_ENABLE) && (!pSiS->CRT1off)) {

		    maxcrt2 = 135000;
		    if(pSiS->VBFlags2 & (VB2_301B|VB2_302B)) maxcrt2 = 162000;
		    else if(pSiS->VBFlags2 & VB2_301C)       maxcrt2 = 203000;
		    else if(pSiS->VBFlags2 & VB2_307T)       maxcrt2 = 203000; /* TODO */
		    /* if(pSiS->VBFlags2 & VB2_30xBDH)       maxcrt2 = 100000;
		       Ignore 301B-DH here; seems the current version is like
		       301B anyway */

		    crt2used = 0.0;
		    crt2clock = SiSEstimateCRT2Clock(pScrn, IsForCRT2);
		    if(crt2clock) {
		       crt2used = crt2clock + 2000;
		    }
		    DHM = FALSE;
		    GetForCRT1 = FALSE;

#ifdef SISDUALHEAD
		    if((pSiS->DualHeadMode) && (pSiSEnt)) {
		       DHM = TRUE;
		       if(pSiS->SecondHead) GetForCRT1 = TRUE;
		    }
#endif
#ifdef SISMERGED
		    if(pSiS->MergedFB && IsForCRT2) {
		       DHM = TRUE;
		       GetForCRT1 = FALSE;
		    }
#endif

		    if(DHM) {

			if(!GetForCRT1) {

			     /* First head = CRT2 */

			     if(crt2clock) {
				/* We use the mem bandwidth as max clock; this
				 * might exceed the 70% limit a bit, but that
				 * does not matter; we take care of that limit
				 * when we calc CRT1. Overall, we might use up
				 * to 85% of the memory bandwidth, which seems
				 * enough to use accel and video.
				 * The "* macic" is just to compensate the
				 * calculation below.
				*/
				total = crt2used * magic;

			     } else {
				/*  We don't know about the second head's
				 *  depth yet. So we assume it uses the
				 *  same. But since the maximum dotclock
				 *  is limited on CRT2, we can assume a
				 *  maximum here.
				 */
				if((total / 2) > (maxcrt2 + 2000)) {
				    total = (maxcrt2 + 2000) * magic;
				    crt2used = maxcrt2 + 2000;
				} else {
				    total /= 2;
				    crt2used = total;
				}

			     }

			     xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
				  "Bandwidth reserved for CRT2 is %g MHz\n",
				      crt2used/1000);

			} else {
#ifdef SISDUALHEAD
			     /* Second head = CRT1 */

			     /*     Now We know about the first head's depth,
			      *     so we can calculate more accurately.
			      */

			     if(crt2clock) {
				total -= (crt2used * pSiSEnt->pScrn_1->bitsPerPixel / bpp);
				xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
				   "Bandwidth reserved for CRT2 at %d bpp is %g Mhz\n",
				      bpp,
				      (crt2used * pSiSEnt->pScrn_1->bitsPerPixel / bpp)/1000);
			     } else {
				total -= (pSiSEnt->maxUsedClock * pSiSEnt->pScrn_1->bitsPerPixel / bpp);
				xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
				   "Bandwidth reserved for CRT2 at %d bpp is %d Mhz\n",
				      bpp,
				      (pSiSEnt->maxUsedClock * pSiSEnt->pScrn_1->bitsPerPixel / bpp)/1000);
			     }

			     xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
				 "Bandwidth available for CRT1 is %g MHz\n", total/1000);
#endif
			}

		    } else {

			if(crt2clock) {
			    total -= crt2used;
			} else {
                            if((total / 2) > (maxcrt2 + 2000)) {
				total -= (maxcrt2 + 2000);
				crt2used = maxcrt2 + 2000;
			    } else {
				total /= 2;
				crt2used = total;
			    }
			}
			xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			  "Bandwidth reserved for CRT2 is %g Mhz\n", crt2used/1000);

			xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			  "Bandwidth available for CRT1 is %g MHz\n", total/1000);

		    }

		}

		total /= magic;
		if(total > (max / 2)) total = max / 2;
		return(int)(total);

        default:
		return(135000);
        }
}

/* Load the palette. We do this for all supported color depths
 * in order to support gamma correction. We hereby convert the
 * given colormap to a complete 24bit color palette and enable
 * the correspoding bit in SR7 to enable the 24bit lookup table.
 * Gamma correction for CRT2 is only supported on SiS video bridges.
 * There are there 6-bit-RGB values submitted even if bpp is 16 and
 * weight is 565, because SetWeight() sets rgbBits to the maximum
 * (which is 6 in the 565 case).
 */
void
SISLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors,
               VisualPtr pVisual)
{
     SISPtr  pSiS = SISPTR(pScrn);
     int     i, j, index;
     int     myshift = 8 - pScrn->rgbBits;
     UChar   backup = 0;
     Bool    dogamma1 = pSiS->CRT1gamma;
     Bool    resetxvgamma = FALSE;
#ifdef SISDUALHEAD
     SISEntPtr pSiSEnt = pSiS->entityPrivate;

     if(pSiS->DualHeadMode) dogamma1 = pSiSEnt->CRT1gamma;
#endif

     PDEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "LoadPalette()\n"));

#ifdef SISDUALHEAD
     if((!pSiS->DualHeadMode) || (pSiS->SecondHead)) {
#endif

	if(pSiS->VGAEngine == SIS_315_VGA) {
	   inSISIDXREG(SISSR, 0x1f, backup);
	   andSISIDXREG(SISSR, 0x1f, 0xe7);
	   if( (pSiS->XvGamma) &&
	       (pSiS->MiscFlags & MISC_CRT1OVERLAYGAMMA) &&
	       ((pSiS->CurrentLayout.depth == 16) ||
	        (pSiS->CurrentLayout.depth == 24)) ) {
	      orSISIDXREG(SISSR, 0x1f, 0x10);
	      resetxvgamma = TRUE;
	   }
	}

	switch(pSiS->CurrentLayout.depth) {
	  case 15:
	     if(dogamma1) {
		orSISIDXREG(SISSR, 0x07, 0x04);
		/* 315/330: depth 15 not supported, no MMIO code needed */
		for(i=0; i<numColors; i++) {
		   index = indices[i];
		   if(index < 32) {   /* Paranoia */
		      for(j=0; j<8; j++) {
			 outSISREG(SISCOLIDX, (index << 3) + j);
			 outSISREG(SISCOLDATA, colors[index].red   << myshift);
			 outSISREG(SISCOLDATA, colors[index].green << myshift);
			 outSISREG(SISCOLDATA, colors[index].blue  << myshift);
		      }
		   }
		}
	     } else {
		andSISIDXREG(SISSR, 0x07, ~0x04);
	     }
	     break;
	  case 16:
	     if(dogamma1) {
		orSISIDXREG(SISSR, 0x07, 0x04);
		if(pSiS->ChipFlags & SiSCF_MMIOPalette) {
		   for(i=0; i<numColors; i++) {
		      index = indices[i];
		      if(index < 64) {  /* Paranoia */
			 for(j=0; j<4; j++) {
			    SIS_MMIO_OUT32(pSiS->IOBase, 0x8570,
					   (colors[index].green     << (myshift + 8))  |
					   (colors[index >> 1].blue << (myshift + 16)) |
					   (colors[index >> 1].red  << myshift)        |
					   (((index << 2) + j)      << 24));
			 }
		      }
		   }
		} else {
		   for(i=0; i<numColors; i++) {
		      index = indices[i];
		      if(index < 64) {  /* Paranoia */
			 for(j=0; j<4; j++) {
			    outSISREG(SISCOLIDX, (index << 2) + j);
			    outSISREG(SISCOLDATA, colors[index >> 1].red  << myshift);
			    outSISREG(SISCOLDATA, colors[index].green     << myshift);
			    outSISREG(SISCOLDATA, colors[index >> 1].blue << myshift);
			 }
		      }
		   }
		}
	     } else {
		andSISIDXREG(SISSR, 0x07, ~0x04);
	     }
	     break;
          case 24:
	     if(dogamma1) {
		orSISIDXREG(SISSR, 0x07, 0x04);
		if(pSiS->ChipFlags & SiSCF_MMIOPalette) {
		   for(i=0; i<numColors; i++)  {
		      index = indices[i];
		      if(index < 256) {   /* Paranoia */
			 SIS_MMIO_OUT32(pSiS->IOBase, 0x8570,
					(colors[index].blue  << 16) |
					(colors[index].green <<  8) |
					(colors[index].red)         |
					(index               << 24));
		      }
		   }
		} else {
		   for(i=0; i<numColors; i++)  {
		      index = indices[i];
		      if(index < 256) {   /* Paranoia */
			 outSISREG(SISCOLIDX, index);
			 outSISREG(SISCOLDATA, colors[index].red);
			 outSISREG(SISCOLDATA, colors[index].green);
			 outSISREG(SISCOLDATA, colors[index].blue);
		      }
		   }
		}
	     } else {
		andSISIDXREG(SISSR, 0x07, ~0x04);
	     }
	     break;
	  default:
	     andSISIDXREG(SISSR, 0x07, ~0x04);
	     if(pSiS->ChipFlags & SiSCF_MMIOPalette) {
	        for(i=0; i<numColors; i++)  {
		   index = indices[i];
		   SIS_MMIO_OUT32(pSiS->IOBase, 0x8570,
				  ((colors[index].blue)  << 16) |
				  ((colors[index].green) <<  8) |
				  (colors[index].red)           |
				  (index                 << 24));
		}
	     } else {
		for(i=0; i<numColors; i++) {
		   /* In pio mode, only 6 bits are supported */
		   index = indices[i];
		   outSISREG(SISCOLIDX, index);
		   outSISREG(SISCOLDATA, colors[index].red   >> 2);
		   outSISREG(SISCOLDATA, colors[index].green >> 2);
		   outSISREG(SISCOLDATA, colors[index].blue  >> 2);
		}
	     }
	}

	if(pSiS->VGAEngine == SIS_315_VGA) {
	   outSISIDXREG(SISSR, 0x1f, backup);
	   inSISIDXREG(SISSR, 0x07, backup);
	   if((backup & 0x04) && (resetxvgamma) && (pSiS->ResetXvGamma)) {
	      (pSiS->ResetXvGamma)(pScrn);
	   }
	}

#ifdef SISDUALHEAD
    }
#endif

#ifdef SISDUALHEAD
    if((!pSiS->DualHeadMode) || (!pSiS->SecondHead)) {
#endif
       switch(pSiS->VGAEngine) {
       case SIS_300_VGA:
       case SIS_315_VGA:
	  if(pSiS->VBFlags & CRT2_ENABLE) {
	     /* Only the SiS bridges support a CRT2 palette */
	     if(pSiS->VBFlags2 & VB2_SISBRIDGE) {
		if((pSiS->CRT2SepGamma) && (pSiS->crt2cindices) && (pSiS->crt2colors)) {
		   SiS301LoadPalette(pScrn, numColors, pSiS->crt2cindices, pSiS->crt2colors, myshift);
		} else {
		   SiS301LoadPalette(pScrn, numColors, indices, colors, myshift);
		}
	     }
          }
       }
#ifdef SISDUALHEAD
    }
#endif

}

void
SiS_UpdateGammaCRT2(ScrnInfoPtr pScrn)
{
    SISPtr  pSiS = SISPTR(pScrn);

    if((!pSiS->CRT2SepGamma) || (!pSiS->crt2cindices) || (!pSiS->crt2gcolortable)) return;

#ifdef SISDUALHEAD
    if(pSiS->DualHeadMode) return;
#endif

    SISCalculateGammaRampCRT2(pScrn);
    SiS301LoadPalette(pScrn, pSiS->CRT2ColNum, pSiS->crt2cindices, pSiS->crt2colors, (8 - pScrn->rgbBits));
}

static  void
SiS301LoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors, int myshift)
{
	SISPtr  pSiS = SISPTR(pScrn);
	int     i, j, index;
	Bool    dogamma2 = pSiS->CRT2gamma;
#ifdef SISDUALHEAD
	SISEntPtr pSiSEnt = pSiS->entityPrivate;

	if(pSiS->DualHeadMode) dogamma2 = pSiSEnt->CRT2gamma;
#endif

	/* 301B-DH does not support a color palette for LCD */
	if((pSiS->VBFlags2 & VB2_30xBDH) && (pSiS->VBFlags & CRT2_LCD)) return;

	switch(pSiS->CurrentLayout.depth) {
          case 15:
	     if(dogamma2) {
		orSISIDXREG(SISPART4, 0x0d, 0x08);
		for(i=0; i<numColors; i++) {
		   index = indices[i];
		   if(index < 32) {   /* Paranoia */
		      for(j=0; j<8; j++) {
			 outSISREG(SISCOL2IDX, (index << 3) + j);
			 outSISREG(SISCOL2DATA, colors[index].red   << myshift);
			 outSISREG(SISCOL2DATA, colors[index].green << myshift);
			 outSISREG(SISCOL2DATA, colors[index].blue  << myshift);
		      }
		   }
		}
	     } else {
		andSISIDXREG(SISPART4, 0x0d, ~0x08);
	     }
	     break;
	  case 16:
	     if(dogamma2) {
		orSISIDXREG(SISPART4, 0x0d, 0x08);
		for(i = 0; i < numColors; i++) {
		   index = indices[i];
		   if(index < 64) {  /* Paranoia */
		      for(j = 0; j < 4; j++) {
			 outSISREG(SISCOL2IDX, (index << 2) + j);
			 outSISREG(SISCOL2DATA, colors[index >> 1].red  << myshift);
			 outSISREG(SISCOL2DATA, colors[index].green     << myshift);
			 outSISREG(SISCOL2DATA, colors[index >> 1].blue << myshift);
		      }
		   }
		}
	     } else {
		andSISIDXREG(SISPART4, 0x0d, ~0x08);
	     }
	     break;
          case 24:
	     if(dogamma2) {
		orSISIDXREG(SISPART4, 0x0d, 0x08);
		for(i = 0; i < numColors; i++) {
		   index = indices[i];
		   if(index < 256) {   /* Paranoia */
		      outSISREG(SISCOL2IDX, index);
		      outSISREG(SISCOL2DATA, colors[index].red);
		      outSISREG(SISCOL2DATA, colors[index].green);
		      outSISREG(SISCOL2DATA, colors[index].blue);
		   }
        	}
	     } else {
		andSISIDXREG(SISPART4, 0x0d, ~0x08);
	     }
	     break;
	  default:
	     orSISIDXREG(SISPART4, 0x0d, 0x08);
	     for(i = 0; i < numColors; i++) {
		index = indices[i];
		outSISREG(SISCOL2IDX,  index);
		outSISREG(SISCOL2DATA, colors[index].red);
		outSISREG(SISCOL2DATA, colors[index].green);
		outSISREG(SISCOL2DATA, colors[index].blue);
             }
	 }
}

void
SISDACPreInit(ScrnInfoPtr pScrn)
{
    SISPtr pSiS = SISPTR(pScrn);
    Bool IsForCRT2 = FALSE;

#ifdef SISDUALHEAD
    if((pSiS->DualHeadMode) && (!pSiS->SecondHead))
       IsForCRT2 = TRUE;
#endif

    pSiS->MaxClock = SiSMemBandWidth(pScrn, IsForCRT2);

    switch (pSiS->Chipset) {
       case PCI_CHIP_SIS550:
       case PCI_CHIP_SIS315:
       case PCI_CHIP_SIS315H:
       case PCI_CHIP_SIS315PRO:
       case PCI_CHIP_SIS650:
       case PCI_CHIP_SIS330:
       case PCI_CHIP_SIS660:
       case PCI_CHIP_SIS340:
       case PCI_CHIP_XGIXG20:
       case PCI_CHIP_XGIXG40:
          pSiS->SiSSave     = SiS315Save;
          pSiS->SiSRestore  = SiS315Restore;
          break;
       case PCI_CHIP_SIS300:
       case PCI_CHIP_SIS540:
       case PCI_CHIP_SIS630:
          pSiS->SiSSave     = SiS300Save;
          pSiS->SiSRestore  = SiS300Restore;
          break;
       case PCI_CHIP_SIS5597:
       case PCI_CHIP_SIS6326:
       case PCI_CHIP_SIS530:
       default:
          pSiS->SiSSave     = SiSSave;
          pSiS->SiSRestore  = SiSRestore;
          break;
    }
}

static void
SetBlock(CARD16 port, CARD8 from, CARD8 to, CARD8 *DataPtr)
{
    CARD8 index;

    for(index = from; index <= to; index++, DataPtr++) {
       outSISIDXREG(port, index, *DataPtr);
    }
}

void
SiS6326SetTVReg(ScrnInfoPtr pScrn, CARD8 index, CARD8 data)
{
    SISPtr  pSiS = SISPTR(pScrn);
    outSISIDXREG(SISCR, 0xE0, index);
    outSISIDXREG(SISCR, 0xE1, data);
#ifdef TWDEBUG
    xf86DrvMsg(0, X_INFO, "SiS6326: Setting Tv %02x to %02x\n", index, data);
#endif
}

UChar
SiS6326GetTVReg(ScrnInfoPtr pScrn, CARD8 index)
{
    SISPtr pSiS = SISPTR(pScrn);
    UChar  data;

    outSISIDXREG(SISCR, 0xE0, index);
    inSISIDXREG(SISCR, 0xE1, data);
    return(data);
}

void
SiS6326SetXXReg(ScrnInfoPtr pScrn, CARD8 index, CARD8 data)
{
    SISPtr  pSiS = SISPTR(pScrn);
    outSISIDXREG(SISCR, 0xE2, index);
    outSISIDXREG(SISCR, 0xE3, data);
}

UChar
SiS6326GetXXReg(ScrnInfoPtr pScrn, CARD8 index)
{
    SISPtr pSiS = SISPTR(pScrn);
    UChar  data;

    outSISIDXREG(SISCR, 0xE2, index);
    inSISIDXREG(SISCR, 0xE3, data);
    return(data);
}

UChar SiSGetCopyROP(int rop)
{
    const UChar sisALUConv[] =
    {
       0x00,       /* dest = 0;            0,      GXclear,        0 */
       0x88,       /* dest &= src;         DSa,    GXand,          0x1 */
       0x44,       /* dest = src & ~dest;  SDna,   GXandReverse,   0x2 */
       0xCC,       /* dest = src;          S,      GXcopy,         0x3 */
       0x22,       /* dest &= ~src;        DSna,   GXandInverted,  0x4 */
       0xAA,       /* dest = dest;         D,      GXnoop,         0x5 */
       0x66,       /* dest = ^src;         DSx,    GXxor,          0x6 */
       0xEE,       /* dest |= src;         DSo,    GXor,           0x7 */
       0x11,       /* dest = ~src & ~dest; DSon,   GXnor,          0x8 */
       0x99,       /* dest ^= ~src ;       DSxn,   GXequiv,        0x9 */
       0x55,       /* dest = ~dest;        Dn,     GXInvert,       0xA */
       0xDD,       /* dest = src|~dest ;   SDno,   GXorReverse,    0xB */
       0x33,       /* dest = ~src;         Sn,     GXcopyInverted, 0xC */
       0xBB,       /* dest |= ~src;        DSno,   GXorInverted,   0xD */
       0x77,       /* dest = ~src|~dest;   DSan,   GXnand,         0xE */
       0xFF,       /* dest = 0xFF;         1,      GXset,          0xF */
    };

    return(sisALUConv[rop]);
}

UChar SiSGetPatternROP(int rop)
{
    const UChar sisPatALUConv[] =
    {
       0x00,       /* dest = 0;            0,      GXclear,        0 */
       0xA0,       /* dest &= src;         DPa,    GXand,          0x1 */
       0x50,       /* dest = src & ~dest;  PDna,   GXandReverse,   0x2 */
       0xF0,       /* dest = src;          P,      GXcopy,         0x3 */
       0x0A,       /* dest &= ~src;        DPna,   GXandInverted,  0x4 */
       0xAA,       /* dest = dest;         D,      GXnoop,         0x5 */
       0x5A,       /* dest = ^src;         DPx,    GXxor,          0x6 */
       0xFA,       /* dest |= src;         DPo,    GXor,           0x7 */
       0x05,       /* dest = ~src & ~dest; DPon,   GXnor,          0x8 */
       0xA5,       /* dest ^= ~src ;       DPxn,   GXequiv,        0x9 */
       0x55,       /* dest = ~dest;        Dn,     GXInvert,       0xA */
       0xF5,       /* dest = src|~dest ;   PDno,   GXorReverse,    0xB */
       0x0F,       /* dest = ~src;         Pn,     GXcopyInverted, 0xC */
       0xAF,       /* dest |= ~src;        DPno,   GXorInverted,   0xD */
       0x5F,       /* dest = ~src|~dest;   DPan,   GXnand,         0xE */
       0xFF,       /* dest = 0xFF;         1,      GXset,          0xF */
    };

    return(sisPatALUConv[rop]);
}
