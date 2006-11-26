/* $XFree86$ */
/* $XdotOrg: driver/xf86-video-sisusb/src/sisusb_dac.c,v 1.6 2005/08/15 22:57:51 twini Exp $ */
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

#include "sisusb.h"
#define NEED_cpu_to_le16
#define NEED_cpu_to_le32
#include "sisusb_regs.h"
#include "sisusb_dac.h"

#include "dixstruct.h"

#ifdef XFreeXDGA
#include "dgaproc.h"
#endif

static void SiSUSBSave(ScrnInfoPtr pScrn, SISUSBRegPtr sisReg);
static void SiSUSBRestore(ScrnInfoPtr pScrn, SISUSBRegPtr sisReg);

UChar       SiSUSBGetCopyROP(int rop);
UChar       SiSUSBGetPatternROP(int rop);

int SiSUSB_compute_vclk(
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

    return 1;
}

void
SiSUSBCalcClock(ScrnInfoPtr pScrn, int clock, int max_VLD, unsigned int *vclk)
{
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

/* Save register contents */
static void
SiSUSBSave(ScrnInfoPtr pScrn, SISUSBRegPtr sisReg)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    int i;

#ifdef UNLOCK_ALWAYS
    sisusbSaveUnlockExtRegisterLock(pSiSUSB, NULL, NULL);
#endif

    /* Save SR registers */
    for(i = 0x00; i <= 0x3F; i++) {
       inSISIDXREG(pSiSUSB, SISSR, i, sisReg->sisRegs3C4[i]);
#ifdef TWDEBUG
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
			 "SR%02X - %02X \n", i,sisReg->sisRegs3C4[i]);
#endif
    }

    /* Save command queue location */
    sisReg->sisMMIO85C0 = SIS_MMIO_IN32(pSiSUSB, pSiSUSB->IOBase, 0x85C0);

    /* Save CR registers */
    for(i = 0x00; i <= 0x7c; i++)  {
       inSISIDXREG(pSiSUSB, SISCR, i, sisReg->sisRegs3D4[i]);
#ifdef TWDEBUG
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"CR%02X Contents - %02X \n", i,sisReg->sisRegs3D4[i]);
#endif
    }

    /* Save video capture registers */
    for(i = 0x00; i <= 0x4f; i++)  {
       inSISIDXREG(pSiSUSB, SISCAP, i, sisReg->sisCapt[i]);
    }

    /* Save video playback registers */
    for(i = 0x00; i <= 0x3f; i++) {
       inSISIDXREG(pSiSUSB, SISVID, i, sisReg->sisVid[i]);
#ifdef TWDEBUG
       xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		"Vid%02X Contents - %02X \n", i,sisReg->sisVid[i]);
#endif
    }

    /* Save Misc register */
    sisReg->sisRegs3C2 = inSISREG(pSiSUSB, SISMISCR);

    /* Save mode number */
    sisReg->BIOSModeSave = SiSUSB_GetSetModeID(pScrn,0xFF);
}

/* Restore register contents */
static void
SiSUSBRestore(ScrnInfoPtr pScrn, SISUSBRegPtr sisReg)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
    int i,temp;

#ifdef UNLOCK_ALWAYS
    sisusbSaveUnlockExtRegisterLock(pSiSUSB, NULL, NULL);
#endif

    /* Wait for accelerator to finish on-going drawing operations. */
    inSISIDXREG(pSiSUSB, SISSR, 0x1E, temp);
    if(temp & (0x40|0x10|0x02))  {	/* 0x40 = 2D, 0x10 = 3D enabled*/
       while ( (SIS_MMIO_IN32(pSiSUSB, pSiSUSB->IOBase, 0x85CC) & 0x80000000) != 0x80000000){};
       while ( (SIS_MMIO_IN32(pSiSUSB, pSiSUSB->IOBase, 0x85CC) & 0x80000000) != 0x80000000){};
       while ( (SIS_MMIO_IN32(pSiSUSB, pSiSUSB->IOBase, 0x85CC) & 0x80000000) != 0x80000000){};
    }

    /* We reset the command queue before restoring.
     * This might be required because we never know what
     * console driver (like the kernel framebuffer driver)
     * or application is running and which queue mode it
     * uses.
     */
    andSISIDXREG(pSiSUSB, SISCR, 0x55, 0x33);
    outSISIDXREG(pSiSUSB, SISSR, 0x26, 0x01);
    outSISIDXREG(pSiSUSB, SISSR, 0x27, 0x1F);

    /* Restore extended CR registers */
    for(i = 0x19; i < 0x5C; i++) {
       outSISIDXREG(pSiSUSB, SISCR, i, sisReg->sisRegs3D4[i]);
    }
    outSISIDXREG(pSiSUSB, SISCR, 0x79, sisReg->sisRegs3D4[0x79]);

    outSISIDXREG(pSiSUSB, SISCR, pSiSUSB->myCR63, sisReg->sisRegs3D4[pSiSUSB->myCR63]);

    /* Leave PCI_IO_ENABLE on if accelerators are on (Is this required?) */
    if(sisReg->sisRegs3C4[0x1e] & 0x50) {  /* 0x40=2D, 0x10=3D */
       sisReg->sisRegs3C4[0x20] |= 0x20;
       outSISIDXREG(pSiSUSB, SISSR, 0x20, sisReg->sisRegs3C4[0x20]);
    }

    /* Restore extended SR registers */
    for(i = 0x06; i <= 0x3F; i++) {
       if(i == 0x26) {
          continue;
       } else if(i == 0x27) {
          outSISIDXREG(pSiSUSB, SISSR, 0x27, sisReg->sisRegs3C4[0x27]);
          outSISIDXREG(pSiSUSB, SISSR, 0x26, sisReg->sisRegs3C4[0x26]);
       } else {
          outSISIDXREG(pSiSUSB, SISSR, i, sisReg->sisRegs3C4[i]);
       }
    }

    /* Restore VCLK and ECLK */
    andSISIDXREG(pSiSUSB, SISSR,0x31,0xcf);
    outSISIDXREG(pSiSUSB, SISSR,0x2b,sisReg->sisRegs3C4[0x2b]);
    outSISIDXREG(pSiSUSB, SISSR,0x2c,sisReg->sisRegs3C4[0x2c]);
    outSISIDXREG(pSiSUSB, SISSR,0x2d,0x01);

#ifndef SISVRAMQ
    /* Initialize read/write pointer for command queue */
    SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x85C4, SIS_MMIO_IN32(pSiSUSB, pSiSUSB->IOBase, 0x85C8));
#endif
    /* Restore queue location */
    SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x85C0, sisReg->sisMMIO85C0);

    /* Restore Misc register */
    outSISREG(pSiSUSB, SISMISCW, sisReg->sisRegs3C2);

    /* MemClock needs this to take effect */
    outSISIDXREG(pSiSUSB, SISSR, 0x00, 0x01);    /* Synchronous Reset */
    outSISIDXREG(pSiSUSB, SISSR, 0x00, 0x03);    /* End Reset */

    /* Restore Mode number */
    SiSUSB_GetSetModeID(pScrn,sisReg->BIOSModeSave);
}

/* Restore output selection registers */
void
SiSUSBRestoreBridge(ScrnInfoPtr pScrn, SISUSBRegPtr sisReg)
{
   SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
   int i;

#ifdef UNLOCK_ALWAYS
   sisusbSaveUnlockExtRegisterLock(pSiSUSB, NULL, NULL);
#endif

   for(i = 0x30; i <= 0x3b; i++) {
      if(i == 0x34) continue;
      outSISIDXREG(pSiSUSB, SISCR, i, sisReg->sisRegs3D4[i]);
   }

   outSISIDXREG(pSiSUSB, SISCR, pSiSUSB->myCR63, sisReg->sisRegs3D4[pSiSUSB->myCR63]);
   outSISIDXREG(pSiSUSB, SISCR, 0x79, sisReg->sisRegs3D4[0x79]);
}

/* Auxiliary function to find real memory clock (in Khz) */
int
SiSUSBMclk(SISUSBPtr pSiSUSB)
{
    int mclk = 0;
    UChar Num, Denum;

    /* Numerator */
    inSISIDXREG(pSiSUSB, SISSR, 0x28, Num);
    mclk = 14318 * ((Num & 0x7f) + 1);

    /* Denumerator */
    inSISIDXREG(pSiSUSB, SISSR, 0x29, Denum);
    mclk = mclk / ((Denum & 0x1f) + 1);

    /* Divider */
    if((Num & 0x80) != 0)  mclk *= 2;

    /* Post-Scaler */
    if((Denum & 0x80) == 0) {
       mclk = mclk / (((Denum & 0x60) >> 5) + 1);
    } else {
       mclk = mclk / ((((Denum & 0x60) >> 5) + 1) * 2);
    }

    return(mclk);
}

/* Calculate the maximum dotclock */
int SiSUSBMemBandWidth(ScrnInfoPtr pScrn, Bool IsForCRT2)
{
        SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

        int          bus = pSiSUSB->BusWidth;
        int          mclk = pSiSUSB->MemClock;
	int          bpp = pSiSUSB->CurrentLayout.bitsPerPixel;
	int	     max = 0;
	float        magic = 0.0, total;
#ifdef __SUNPRO_C
#define const
#endif
        const float  magicDED[4] = { 1.2,      1.368421, 2.263158, 1.2};
#ifdef __SUNPRO_C
#undef const
#endif

        magic = magicDED[bus/64];
	max = 780000;

        total = mclk * bus / bpp;

        xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
			"Memory bandwidth at %d bpp is %g MHz\n", bpp, total/1000);

	total /= magic;
	if(total > (max / 2)) total = max / 2;

	return(int)(total);
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
SISUSBLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors,
               VisualPtr pVisual)
{
    SISUSBPtr  pSiSUSB = SISUSBPTR(pScrn);
    int     i, j, index;
    int     myshift = 8 - pScrn->rgbBits;
    UChar   backup = 0;
    Bool    dogamma1 = pSiSUSB->CRT1gamma;
    Bool    resetxvgamma = FALSE;

    inSISIDXREG(pSiSUSB, SISSR, 0x1f, backup);
    andSISIDXREG(pSiSUSB, SISSR, 0x1f, 0xe7);
    if( (pSiSUSB->XvGamma) &&
        (pSiSUSB->MiscFlags & MISC_CRT1OVERLAYGAMMA) &&
        ((pSiSUSB->CurrentLayout.depth == 16) ||
         (pSiSUSB->CurrentLayout.depth == 24)) ) {
       orSISIDXREG(pSiSUSB, SISSR, 0x1f, 0x10);
       resetxvgamma = TRUE;
    }

     switch(pSiSUSB->CurrentLayout.depth) {
	  case 16:
	     if(dogamma1) {
                orSISIDXREG(pSiSUSB, SISSR, 0x07, 0x04);
		for(i=0; i<numColors; i++) {
                      index = indices[i];
		      if(index < 64) {  /* Paranoia */
		         for(j=0; j<4; j++) {
			    SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x8570,
			    		   (colors[index].green     << (myshift + 8))  |
                            		   (colors[index >> 1].blue << (myshift + 16)) |
                            		   (colors[index >> 1].red  << myshift)        |
					   (((index << 2) + j)      << 24));
		         }
		      }
                   }
	     } else {
	        andSISIDXREG(pSiSUSB, SISSR, 0x07, ~0x04);
	     }
	     break;
          case 24:
	     if(dogamma1) {
	        orSISIDXREG(pSiSUSB, SISSR, 0x07, 0x04);
		for(i=0; i<numColors; i++)  {
                      index = indices[i];
		      if(index < 256) {   /* Paranoia */
		         SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x8570,
			 	        (colors[index].blue  << 16) |
					(colors[index].green <<  8) |
					(colors[index].red)         |
			 		(index               << 24));
		      }
		}
	     } else {
	        andSISIDXREG(pSiSUSB, SISSR, 0x07, ~0x04);
	     }
	     break;
	  default:
	     andSISIDXREG(pSiSUSB, SISSR, 0x07, ~0x04);
	     for(i=0; i<numColors; i++)  {
                   index = indices[i];
		   SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, 0x8570,
		   		  ((colors[index].blue)  << 16) |
				  ((colors[index].green) <<  8) |
				  (colors[index].red)           |
				  (index                 << 24));
	     }
	}

	outSISIDXREG(pSiSUSB, SISSR, 0x1f, backup);
	inSISIDXREG(pSiSUSB, SISSR, 0x07, backup);
	if((backup & 0x04) && (resetxvgamma) && (pSiSUSB->ResetXvGamma)) {
	   (pSiSUSB->ResetXvGamma)(pScrn);
	}

}

void
SISUSBDACPreInit(ScrnInfoPtr pScrn)
{
    SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);

    pSiSUSB->MaxClock = SiSUSBMemBandWidth(pScrn, FALSE);

    pSiSUSB->SiSSave     = SiSUSBSave;
    pSiSUSB->SiSRestore  = SiSUSBRestore;
}

UChar SiSUSBGetCopyROP(int rop)
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

UChar SiSUSBGetPatternROP(int rop)
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

static void SiSLostConnection(SISUSBPtr pSiSUSB)
{
    pSiSUSB->sisusberrorsleepcount = 0;
    pSiSUSB->sisusbfatalerror = 1;
    UpdateCurrentTime();
    pSiSUSB->errorTime = currentTime.milliseconds;
    close(pSiSUSB->sisusbdev);
    pSiSUSB->sisusbdevopen = FALSE;
    xf86DrvMsg(pSiSUSB->pScrn->scrnIndex, X_ERROR,
    		"Lost connection to USB device\n");
    if(pSiSUSB->timeout == -1) {
#ifdef XFreeXDGA
       /* DGAShutdown();   - not resolved, not in symlists - ARGH... */
#endif
       GiveUp(0);
    }
}

void
SiSUSBMemCopyToVideoRam(SISUSBPtr pSiSUSB, UChar *to, UChar *from, int size)
{
   int num, retry = 3;
   if(pSiSUSB->sisusbfatalerror) return;
   do {
      lseek(pSiSUSB->sisusbdev, (int)to, SEEK_SET);
      num = write(pSiSUSB->sisusbdev, from, size);
   } while((num != size) && --retry);
   if(!retry) SiSLostConnection(pSiSUSB);
}

UChar inSISREG(SISUSBPtr pSiSUSB, ULong base)
{
    UChar tmp;
    int num, retry = 3;
    if(pSiSUSB->sisusbfatalerror) return 0;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       num = read(pSiSUSB->sisusbdev, &tmp, 1);
    } while((num != 1) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
    return tmp;
}

UShort inSISREGW(SISUSBPtr pSiSUSB, ULong base)
{
    UShort tmp;
    int num, retry = 3;
    if(pSiSUSB->sisusbfatalerror) return 0;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       num = read(pSiSUSB->sisusbdev, &tmp, 2);
    } while((num != 2) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
    return tmp;
}

ULong inSISREGL(SISUSBPtr pSiSUSB, ULong base)
{
    ULong tmp;
    int num, retry = 3;
    if(pSiSUSB->sisusbfatalerror) return 0;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       num = read(pSiSUSB->sisusbdev, &tmp, 4);
    } while((num != 4) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
    return tmp;
}

void outSISREG(SISUSBPtr pSiSUSB, ULong base, UChar val)
{
    int num, retry = 3;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &val, 1);
    } while((num != 1) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
}

void outSISREGW(SISUSBPtr pSiSUSB, ULong base, UShort val)
{
    int num, retry = 3;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &val, 2);
    } while((num != 2) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
}

void outSISREGL(SISUSBPtr pSiSUSB, ULong base, unsigned int val)
{
    int num, retry = 3;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &val, 4);
    } while((num != 4) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
}

void orSISREG(SISUSBPtr pSiSUSB, ULong base, UChar val)
{
    UChar tmp;
    int num, retry = 3;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       read(pSiSUSB->sisusbdev, &tmp, 1);
       tmp |= val;
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &tmp, 1);
    } while((num != 1) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
}

void andSISREG(SISUSBPtr pSiSUSB, ULong base, UChar val)
{
    UChar tmp;
    int num, retry = 3;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       read(pSiSUSB->sisusbdev, &tmp, 1);
       tmp &= val;
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &tmp, 1);
    } while((num != 1) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
}

void outSISIDXREG(SISUSBPtr pSiSUSB, ULong base, UChar idx, UChar val)
{
    int num, retry = 3;
#ifdef SIS_USEIOCTL
    sisusb_command x;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       x.operation = SUCMD_SET;
       x.data3 = base;
       x.data0 = idx;
       x.data1 = val;
       num = ioctl(pSiSUSB->sisusbdev, SISUSB_COMMAND, &x);
    } while((num) && --retry);
#else
    UShort value = (val << 8) | idx;	/* sic! reads/writes wordwise! */
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &value, 2);
    } while((num != 2) && --retry);
#endif
    if(!retry) SiSLostConnection(pSiSUSB);
}

UChar __inSISIDXREG(SISUSBPtr pSiSUSB, ULong base, UChar idx)
{
    int num, retry = 3;
#ifdef SIS_USEIOCTL
    sisusb_command x;
    if(pSiSUSB->sisusbfatalerror) return 0;
    do {
       x.operation = SUCMD_GET;
       x.data3 = base;
       x.data0 = idx;
       num = ioctl(pSiSUSB->sisusbdev, SISUSB_COMMAND, &x);
    } while((num) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
    return x.data1;
#else
    UChar tmp;
    if(pSiSUSB->sisusbfatalerror) return 0;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       write(pSiSUSB->sisusbdev, &idx, 1);
       num = read(pSiSUSB->sisusbdev, &tmp, 1);
    } while((num != 1) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
    return tmp;
#endif
}

void orSISIDXREG(SISUSBPtr pSiSUSB, ULong base, UChar idx, UChar val)
{
    int num, retry = 3;
#ifdef SIS_USEIOCTL
    sisusb_command x;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       x.operation = SUCMD_SETOR;
       x.data3 = base;
       x.data0 = idx;
       x.data1 = val;
       num = ioctl(pSiSUSB->sisusbdev, SISUSB_COMMAND, &x);
    } while((num) && --retry);
#else
    UChar tmp;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       write(pSiSUSB->sisusbdev, &idx, 1);
       read(pSiSUSB->sisusbdev, &tmp, 1);
       tmp |= val;
       lseek(pSiSUSB->sisusbdev, base + 1, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &tmp, 1);
    } while((num != 1) && --retry);
#endif
    if(!retry) SiSLostConnection(pSiSUSB);
}

void andSISIDXREG(SISUSBPtr pSiSUSB, ULong base, UChar idx, UChar val)
{
    int num, retry = 3;
#ifdef SIS_USEIOCTL
    sisusb_command x;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       x.operation = SUCMD_SETAND;
       x.data3 = base;
       x.data0 = idx;
       x.data1 = val;
       num = ioctl(pSiSUSB->sisusbdev, SISUSB_COMMAND, &x);
    } while((num) && --retry);
#else
    UChar tmp;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       write(pSiSUSB->sisusbdev, &idx, 1);
       read(pSiSUSB->sisusbdev, &tmp, 1);
       tmp &= val;
       lseek(pSiSUSB->sisusbdev, base + 1, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &tmp, 1);
    } while((num != 1) && --retry);
#endif
    if(!retry) SiSLostConnection(pSiSUSB);
}

void setSISIDXREG(SISUSBPtr pSiSUSB, ULong base, UChar idx,
			 UChar myand, UChar myor)
{
    int num, retry = 3;
#ifdef SIS_USEIOCTL
    sisusb_command x;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       x.operation = SUCMD_SETANDOR;
       x.data3 = base;
       x.data0 = idx;
       x.data1 = myand;
       x.data2 = myor;
       num = ioctl(pSiSUSB->sisusbdev, SISUSB_COMMAND, &x);
    } while((num) && --retry);
#else
    UChar tmp;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       write(pSiSUSB->sisusbdev, &idx, 1);
       read(pSiSUSB->sisusbdev, &tmp, 1);
       tmp &= myand;
       tmp |= myor;
       lseek(pSiSUSB->sisusbdev, base + 1, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &tmp, 1);
    } while((num != 1) && --retry);
#endif
    if(!retry) SiSLostConnection(pSiSUSB);
}

void setSISIDXREGmask(SISUSBPtr pSiSUSB, ULong base, UChar idx,
                             UChar data, UChar mask)
{
    int num, retry = 3;
#ifdef SIS_USEIOCTL
    sisusb_command x;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       x.operation = SUCMD_SETMASK;
       x.data3 = base;
       x.data0 = idx;
       x.data1 = data;
       x.data2 = mask;
       num = ioctl(pSiSUSB->sisusbdev, SISUSB_COMMAND, &x);
    } while((num) && --retry);
#else
    UChar tmp;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, base, SEEK_SET);
       write(pSiSUSB->sisusbdev, &idx, 1);
       read(pSiSUSB->sisusbdev, &tmp, 1);
       tmp &= ~(mask);
       tmp |= (data & mask);
       lseek(pSiSUSB->sisusbdev, base + 1, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &tmp, 1);
    } while((num != 1) && --retry);
#endif
    if(!retry) SiSLostConnection(pSiSUSB);
}

/* Video RAM and MMIO access ----- */

void sisclearvram(SISUSBPtr pSiSUSB, UChar *where, unsigned int howmuch)
{
   int num, retry = 3;
   sisusb_command x;
   if(pSiSUSB->sisusbfatalerror) return;
   do {
      x.operation = SUCMD_CLRSCR;
      x.data3 = (CARD32)where;
      x.data0 = (howmuch >> 16) & 0xff;
      x.data1 = (howmuch >> 8) & 0xff;
      x.data2 = howmuch & 0xff;
      num = ioctl(pSiSUSB->sisusbdev, SISUSB_COMMAND, &x);
   } while((num) && --retry);
   if(!retry) SiSLostConnection(pSiSUSB);
}

void sisrestoredestroyconsole(SISUSBPtr pSiSUSB, int what)
{
   int num, retry = 3;
   sisusb_command x;
   if(pSiSUSB->sisusbfatalerror) return;
   do {
      x.operation = SUCMD_HANDLETEXTMODE;
      x.data3 = 0;
      x.data0 = what;
      x.data1 = 0;
      x.data2 = 0;
      num = ioctl(pSiSUSB->sisusbdev, SISUSB_COMMAND, &x);
   } while((num) && --retry);
   if(!retry) SiSLostConnection(pSiSUSB);
}

/* MMIO */

void SIS_MMIO_OUT8(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset, CARD8 val)
{
    int num, retry = 3;
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, (int)base + offset, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &val, 1);
    } while((num != 1) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
}

void SIS_MMIO_OUT16(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset, CARD16 val)
{
    int num, retry = 3;
    CARD16 buf = sisusb_cpu_to_le16(val);
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, (int)base + offset, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &buf, 2);
    } while((num != 2) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
}

void SIS_MMIO_OUT32(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset, CARD32 val)
{
    int num, retry = 3;
    CARD32 buf = sisusb_cpu_to_le32(val);
    if(pSiSUSB->sisusbfatalerror) return;
    do {
       lseek(pSiSUSB->sisusbdev, (int)base + offset, SEEK_SET);
       num = write(pSiSUSB->sisusbdev, &buf, 4);
    } while((num != 4) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
}

CARD8 SIS_MMIO_IN8(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset)
{
    int num, retry = 3;
    CARD8 tmp;
    if(pSiSUSB->sisusbfatalerror) return 0;
    do {
       lseek(pSiSUSB->sisusbdev, (int)base + offset, SEEK_SET);
       num = read(pSiSUSB->sisusbdev, &tmp, 1);
    } while((num != 1) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
    return tmp;
}

CARD16 SIS_MMIO_IN16(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset)
{
    int num, retry = 3;
    CARD16 tmp;
    if(pSiSUSB->sisusbfatalerror) return 0;
    do {
       lseek(pSiSUSB->sisusbdev, (int)base + offset, SEEK_SET);
       num = read(pSiSUSB->sisusbdev, &tmp, 2);
    } while((num != 2) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
    return sisusb_le16_to_cpu(tmp);
}

CARD32 SIS_MMIO_IN32(SISUSBPtr pSiSUSB, UChar *base, unsigned int offset)
{
    int num, retry = 3;
    CARD32 tmp;
    if(pSiSUSB->sisusbfatalerror) return 0;
    do {
       lseek(pSiSUSB->sisusbdev, (int)base + offset, SEEK_SET);
       num = read(pSiSUSB->sisusbdev, &tmp, 4);
    } while((num != 4) && --retry);
    if(!retry) SiSLostConnection(pSiSUSB);
    return sisusb_le32_to_cpu(tmp);
}





