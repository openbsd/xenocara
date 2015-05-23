/*
 * MGA-1064, MGA-G100, MGA-G200, MGA-G400, MGA-G550 RAMDAC driver
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "colormapst.h"

/* All drivers should typically include these */
#include "xf86.h"
#include "xf86_OSproc.h"

/* Drivers that need to access the PCI config space directly need this */
#include "xf86Pci.h"

#include "mga_reg.h"
#include "mga.h"
#include "mga_macros.h"
#include "mga_maven.h"

#include "xf86DDC.h"

#include <stdlib.h>
#include <unistd.h>

/*
 * implementation
 */
 
#define DACREGSIZE 0x50
    
/*
 * Only change bits shown in this mask.  Ideally reserved bits should be
 * zeroed here.  Also, don't change the vgaioen bit here since it is
 * controlled elsewhere.
 *
 * XXX These settings need to be checked.
 */
#define OPTION1_MASK	0xFFFFFEFF
#define OPTION2_MASK	0xFFFFFFFF
#define OPTION3_MASK	0xFFFFFFFF

#define OPTION1_MASK_PRIMARY	0xFFFC0FF

static void MGAGRamdacInit(ScrnInfoPtr);
static void MGAGSave(ScrnInfoPtr, vgaRegPtr, MGARegPtr, Bool);
static void MGAGRestore(ScrnInfoPtr, vgaRegPtr, MGARegPtr, Bool);
static Bool MGAGInit(ScrnInfoPtr, DisplayModePtr);
static void MGAGLoadPalette(ScrnInfoPtr, int, int*, LOCO*, VisualPtr);
static Bool MGAG_i2cInit(ScrnInfoPtr pScrn);

static void
MGAG200SEComputePLLParam(ScrnInfoPtr pScrn, long lFo, int *M, int *N, int *P)
{
    unsigned int ulComputedFo;
    unsigned int ulFDelta;
    unsigned int ulFPermitedDelta;
    unsigned int ulFTmpDelta;
    unsigned int ulVCOMax, ulVCOMin;
    unsigned int ulTestP;
    unsigned int ulTestM;
    unsigned int ulTestN;
    unsigned int ulPLLFreqRef;

    ulVCOMax        = 320000;
    ulVCOMin        = 160000;
    ulPLLFreqRef    = 25000;

    ulFDelta = 0xFFFFFFFF;
    /* Permited delta is 0.5% as VESA Specification */
    ulFPermitedDelta = lFo * 5 / 1000;  

    /* Then we need to minimize the M while staying within 0.5% */
    for (ulTestP = 8; ulTestP > 0; ulTestP >>= 1) {
        if ((lFo * ulTestP) > ulVCOMax) continue;
        if ((lFo * ulTestP) < ulVCOMin) continue;

        for (ulTestN = 17; ulTestN <= 256; ulTestN++) {
            for (ulTestM = 1; ulTestM <= 32; ulTestM++) {
                ulComputedFo = (ulPLLFreqRef * ulTestN) / (ulTestM * ulTestP);
                if (ulComputedFo > lFo)
                    ulFTmpDelta = ulComputedFo - lFo;
                else
                    ulFTmpDelta = lFo - ulComputedFo;

                if (ulFTmpDelta < ulFDelta) {
                    ulFDelta = ulFTmpDelta;
                    *M = ulTestM - 1;
                    *N = ulTestN - 1;
                    *P = ulTestP - 1;
                }
            }
        }
    }
}

static void
MGAG200EVComputePLLParam(ScrnInfoPtr pScrn, long lFo, int *M, int *N, int *P)
{
    unsigned int ulComputedFo;
    unsigned int ulFDelta;
    unsigned int ulFPermitedDelta;
    unsigned int ulFTmpDelta;
    unsigned int ulTestP;
    unsigned int ulTestM;
    unsigned int ulTestN;
    unsigned int ulVCOMax;
    unsigned int ulVCOMin;
    unsigned int ulPLLFreqRef;

    ulVCOMax        = 550000;
    ulVCOMin        = 150000;
    ulPLLFreqRef    = 50000;

    ulFDelta = 0xFFFFFFFF;
    /* Permited delta is 0.5% as VESA Specification */
    ulFPermitedDelta = lFo * 5 / 1000;  

    /* Then we need to minimize the M while staying within 0.5% */
    for (ulTestP = 16; ulTestP > 0; ulTestP--) {
	if ((lFo * ulTestP) > ulVCOMax) continue;
	if ((lFo * ulTestP) < ulVCOMin) continue;

	for (ulTestN = 1; ulTestN <= 256; ulTestN++) {
	    for (ulTestM = 1; ulTestM <= 16; ulTestM++) {
		ulComputedFo = (ulPLLFreqRef * ulTestN) / (ulTestM * ulTestP);
		if (ulComputedFo > lFo)
		    ulFTmpDelta = ulComputedFo - lFo;
		else
		    ulFTmpDelta = lFo - ulComputedFo;

		if (ulFTmpDelta < ulFDelta) {
			ulFDelta = ulFTmpDelta;
			*M = (CARD8)(ulTestM - 1);
			*N = (CARD8)(ulTestN - 1);
			*P = (CARD8)(ulTestP - 1);
		}
	    }
	}
    }
#if DEBUG
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "lFo=%ld n=0x%x m=0x%x p=0x%x \n",
		   lFo, *N, *M, *P );
#endif
}

static void
MGAG200WBComputePLLParam(ScrnInfoPtr pScrn, long lFo, int *M, int *N, int *P)
{
    unsigned int ulComputedFo;
    unsigned int ulFDelta;
    unsigned int ulFPermitedDelta;
    unsigned int ulFTmpDelta;
    unsigned int ulVCOMax, ulVCOMin;
    unsigned int ulTestP;
    unsigned int ulTestM;
    unsigned int ulTestN;
    unsigned int ulPLLFreqRef;
    unsigned int ulTestPStart;
    unsigned int ulTestNStart;
    unsigned int ulTestNEnd;
    unsigned int ulTestMStart;
    unsigned int ulTestMEnd;

    ulVCOMax        = 550000;
    ulVCOMin        = 150000;
    ulPLLFreqRef    = 48000;
    ulTestPStart    = 1;
    ulTestNStart    = 1;
    ulTestNEnd      = 150;
    ulTestMStart    = 1;
    ulTestMEnd      = 16;

    ulFDelta = 0xFFFFFFFF;
    /* Permited delta is 0.5% as VESA Specification */
    ulFPermitedDelta = lFo * 5 / 1000;

    /* Then we need to minimize the M while staying within 0.5% */
    for (ulTestP = ulTestPStart; ulTestP < 9; ulTestP++) {
	if ((lFo * ulTestP) > ulVCOMax) continue;
	if ((lFo * ulTestP) < ulVCOMin) continue;

        for (ulTestM = ulTestMStart; ulTestM <= ulTestMEnd; ulTestM++) {
	   for (ulTestN = ulTestNStart; ulTestN <= ulTestNEnd; ulTestN++) {
		ulComputedFo = (ulPLLFreqRef * ulTestN) / (ulTestM * ulTestP);
		if (ulComputedFo > lFo)
			ulFTmpDelta = ulComputedFo - lFo;
		else
			ulFTmpDelta = lFo - ulComputedFo;

		if (ulFTmpDelta < ulFDelta) {
			ulFDelta = ulFTmpDelta;
        		*M = (CARD8)(ulTestM - 1) | (CARD8)(((ulTestN -1) >> 1) & 0x80);
			*N = (CARD8)(ulTestN - 1);
			*P = (CARD8)(ulTestP - 1);
		}
	    }
	}
    }
#if DEBUG
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "lFo=%ld n=0x%x m=0x%x p=0x%x \n",
		   lFo, *N, *M, *P );
#endif
}

static void
MGAG200EHComputePLLParam(ScrnInfoPtr pScrn, long lFo, int *M, int *N, int *P)
{
    unsigned int ulComputedFo;
    unsigned int ulFDelta;
    unsigned int ulFPermitedDelta;
    unsigned int ulFTmpDelta;
    unsigned int ulTestP;
    unsigned int ulTestM;
    unsigned int ulTestN;
    unsigned int ulVCOMax;
    unsigned int ulVCOMin;
    unsigned int ulPLLFreqRef;

    ulVCOMax        = 800000;
    ulVCOMin        = 400000;
    ulPLLFreqRef    = 33333;

    ulFDelta = 0xFFFFFFFF;
    /* Permited delta is 0.5% as VESA Specification */
    ulFPermitedDelta = lFo * 5 / 1000;  

    /* Then we need to minimize the M while staying within 0.5% */
    for (ulTestP = 16; ulTestP > 0; ulTestP>>= 1) {
        if ((lFo * ulTestP) > ulVCOMax) continue;
        if ((lFo * ulTestP) < ulVCOMin) continue;

        for (ulTestM = 1; ulTestM <= 32; ulTestM++) {
           for (ulTestN = 17; ulTestN <= 256; ulTestN++) {
               ulComputedFo = (ulPLLFreqRef * ulTestN) / (ulTestM * ulTestP);
               if (ulComputedFo > lFo)
		           ulFTmpDelta = ulComputedFo - lFo;
               else
                   ulFTmpDelta = lFo - ulComputedFo;

               if (ulFTmpDelta < ulFDelta) {
                   ulFDelta = ulFTmpDelta;
                   *M = (CARD8)(ulTestM - 1);
                   *N = (CARD8)(ulTestN - 1);
                   *P = (CARD8)(ulTestP - 1);
               }

               if ((lFo * ulTestP) >= 600000)
                   *P |= 0x80;
           }
        }
    }
}

static void
MGAG200EVPIXPLLSET(ScrnInfoPtr pScrn, MGARegPtr mgaReg)
{
    MGAPtr pMga = MGAPTR(pScrn);

    unsigned char ucTempByte, ucPixCtrl;

    // Set pixclkdis to 1
    ucPixCtrl = inMGAdac(MGA1064_PIX_CLK_CTL);
    ucPixCtrl |= MGA1064_PIX_CLK_CTL_CLK_DIS;
    outMGAdac(MGA1064_PIX_CLK_CTL, ucPixCtrl);

    // Select PLL Set C
    ucTempByte = INREG8(MGAREG_MEM_MISC_READ);
    ucTempByte |= 0x3<<2; //select MGA pixel clock
    OUTREG8(MGAREG_MEM_MISC_WRITE, ucTempByte);

    // Set pixlock to 0
    ucTempByte = inMGAdac(MGA1064_PIX_PLL_STAT);
    outMGAdac(MGA1064_PIX_PLL_STAT, ucTempByte & ~0x40);

    //    Set pix_stby to 1
    ucPixCtrl |= MGA1064_PIX_CLK_CTL_CLK_POW_DOWN;
    outMGAdac(MGA1064_PIX_CLK_CTL, ucPixCtrl);

    // Program the Pixel PLL Register
    outMGAdac(MGA1064_EV_PIX_PLLC_M, mgaReg->PllM);
    outMGAdac(MGA1064_EV_PIX_PLLC_N, mgaReg->PllN);
    outMGAdac(MGA1064_EV_PIX_PLLC_P, mgaReg->PllP);

    // Wait 50 us
    usleep(50);

    // Set pix_stby to 0
    ucPixCtrl &= ~MGA1064_PIX_CLK_CTL_CLK_POW_DOWN;
    outMGAdac(MGA1064_PIX_CLK_CTL, ucPixCtrl);

    // Wait 500 us
    usleep(500);

    // Select the pixel PLL by setting pixclksel to 1
    ucTempByte = inMGAdac(MGA1064_PIX_CLK_CTL);
    ucTempByte &= ~MGA1064_PIX_CLK_CTL_SEL_MSK;
    ucTempByte |= MGA1064_PIX_CLK_CTL_SEL_PLL;
    outMGAdac(MGA1064_PIX_CLK_CTL, ucTempByte);

    // Set pixlock to 1
    ucTempByte = inMGAdac(MGA1064_PIX_PLL_STAT);
    outMGAdac(MGA1064_PIX_CLK_CTL, ucTempByte | 0x40);

    // Reset dotclock rate bit.
    ucTempByte = INREG8(MGAREG_MEM_MISC_READ);
    ucTempByte |= 0x3<<2; //select MGA pixel clock
    OUTREG8(MGAREG_MEM_MISC_WRITE, ucTempByte);

    OUTREG8(MGAREG_SEQ_INDEX, 1);
    ucTempByte = INREG8(MGAREG_SEQ_DATA);
    OUTREG8(MGAREG_SEQ_DATA, ucTempByte & ~0x8);

    // Set pixclkdis to 0
    ucTempByte = inMGAdac(MGA1064_PIX_CLK_CTL);
    ucTempByte &= ~MGA1064_PIX_CLK_CTL_CLK_DIS;
    outMGAdac(MGA1064_PIX_CLK_CTL, ucTempByte);
}

static void
MGAG200WBPIXPLLSET(ScrnInfoPtr pScrn, MGARegPtr mgaReg)
{
    MGAPtr pMga = MGAPTR(pScrn);

    unsigned long ulLoopCount, ulLockCheckIterations = 0, ulTempCount, ulVCount;
    unsigned char ucTempByte, ucPixCtrl, ucPLLLocked = FALSE;

    while(ulLockCheckIterations <= 32 && ucPLLLocked == FALSE)
    {
        if(ulLockCheckIterations > 0)
        {
            OUTREG8(MGAREG_CRTCEXT_INDEX, 0x1E);
            ucTempByte = INREG8(MGAREG_CRTCEXT_DATA);
            if(ucTempByte < 0xFF)
            {
                OUTREG8(MGAREG_CRTCEXT_DATA, ucTempByte+1);
            }
        }

        // Set pixclkdis to 1
        ucPixCtrl = inMGAdac(MGA1064_PIX_CLK_CTL);
        ucPixCtrl |= MGA1064_PIX_CLK_CTL_CLK_DIS;
        outMGAdac(MGA1064_PIX_CLK_CTL, ucPixCtrl);

        ucTempByte = inMGAdac(MGA1064_REMHEADCTL);
        ucTempByte |= MGA1064_REMHEADCTL_CLKDIS;
        outMGAdac(MGA1064_REMHEADCTL, ucTempByte);

        // Select PLL Set C
        ucTempByte = INREG8(MGAREG_MEM_MISC_READ);
        ucTempByte |= 0x3<<2; //select MGA pixel clock
        OUTREG8(MGAREG_MEM_MISC_WRITE, ucTempByte);

        ucPixCtrl |= MGA1064_PIX_CLK_CTL_CLK_POW_DOWN | 0x80;
        outMGAdac(MGA1064_PIX_CLK_CTL, ucPixCtrl);

        // Wait 500 us
        usleep(500);

        // Reset the PLL
        //   When we are varying the output frequency by more than 
        //   10%, we must reset the PLL. However to be prudent, we
        //   will reset it each time that we are changing it.
        ucTempByte = inMGAdac(MGA1064_VREF_CTL);
        ucTempByte &= ~0x04;
        outMGAdac(MGA1064_VREF_CTL, ucTempByte );

        // Wait 50 us
        usleep(50);

        // Program the Pixel PLL Register
        outMGAdac(MGA1064_WB_PIX_PLLC_N, mgaReg->PllN);
        outMGAdac(MGA1064_WB_PIX_PLLC_M, mgaReg->PllM);
        outMGAdac(MGA1064_WB_PIX_PLLC_P, mgaReg->PllP);

        // Wait 50 us
        usleep(50);

        // Turning the PLL on
        ucTempByte = inMGAdac(MGA1064_VREF_CTL);
        ucTempByte |= 0x04;
        outMGAdac(MGA1064_VREF_CTL, ucTempByte );

        // Wait 500 us
        usleep(500);

        // Select the pixel PLL by setting pixclksel to 1
        ucTempByte = inMGAdac(MGA1064_PIX_CLK_CTL);
        ucTempByte &= ~MGA1064_PIX_CLK_CTL_SEL_MSK;
        ucTempByte |= MGA1064_PIX_CLK_CTL_SEL_PLL;
        outMGAdac(MGA1064_PIX_CLK_CTL, ucTempByte);

        ucTempByte = inMGAdac(MGA1064_REMHEADCTL);
        ucTempByte &= ~MGA1064_REMHEADCTL_CLKSL_MSK;
        ucTempByte |= MGA1064_REMHEADCTL_CLKSL_PLL;
        outMGAdac(MGA1064_REMHEADCTL, ucTempByte);

        // Reset dotclock rate bit.
        OUTREG8(MGAREG_SEQ_INDEX, 1);
        ucTempByte = INREG8(MGAREG_SEQ_DATA);
        OUTREG8(MGAREG_SEQ_DATA, ucTempByte & ~0x8);

        // Set pixclkdis to 0
        ucTempByte = inMGAdac(MGA1064_PIX_CLK_CTL);
        ucTempByte &= ~MGA1064_PIX_CLK_CTL_CLK_DIS;
        outMGAdac(MGA1064_PIX_CLK_CTL, ucTempByte);

        // Poll VCount. If it increments twice inside 150us, 
        // we assume that the PLL has locked.
        ulLoopCount = 0;
        ulVCount = INREG(MGAREG_VCOUNT);

        while(ulLoopCount < 30 && ucPLLLocked == FALSE)
        {
            ulTempCount = INREG(MGAREG_VCOUNT);

            if(ulTempCount < ulVCount)
            {
                ulVCount = 0;
            }
            if ((ucTempByte - ulVCount) > 2)
            {
                ucPLLLocked = TRUE;
            }
            else
            {
                usleep(5);
            }
            ulLoopCount++;
        }
        ulLockCheckIterations++;
    }

    // Set remclkdis to 0
    ucTempByte = inMGAdac(MGA1064_REMHEADCTL);
    ucTempByte &= ~MGA1064_REMHEADCTL_CLKDIS;
    outMGAdac(MGA1064_REMHEADCTL, ucTempByte);
}

#define G200ER_PLLREF 48000
#define G200ER_VCOMIN 1056000
#define G200ER_VCOMAX 1488000

static void MGAG200ERComputePLLParam(ScrnInfoPtr pScrn, long lFo, int *piM, int *piN, int *piP)
{

    int  ulM;
    int  ulN;
    int  ulO;
    int  ulR;

    CARD32 ulComputedFo;
    CARD32 ulVco;
    CARD32 ulFDelta;
    CARD32 ulFTmpDelta;

    CARD32 aulMDivValue[] = {1, 2, 4, 8};

    CARD32 ulFo   = lFo;

    ulFDelta = 0xFFFFFFFF;

    for (ulR = 0; ulR < 4;  ulR++)
    {
    	if(ulFDelta==0) break;
        for (ulN = 5; (ulN <= 128) ; ulN++)
        {
            if(ulFDelta==0) break;
            for (ulM = 3; ulM >= 0; ulM--)
            {
            	if(ulFDelta==0) break;
                for (ulO = 5; ulO <= 32; ulO++)
                {
                	ulVco = (G200ER_PLLREF * (ulN+1)) / (ulR+1);
                	// Validate vco
                    if (ulVco < G200ER_VCOMIN) continue;
					if (ulVco > G200ER_VCOMAX) continue;
                	ulComputedFo = ulVco / (aulMDivValue[ulM] * (ulO+1));

                    if (ulComputedFo > ulFo)
                    {
                        ulFTmpDelta = ulComputedFo - ulFo;
                    }
                    else
                    {
                        ulFTmpDelta = ulFo - ulComputedFo;
                    }

                    if (ulFTmpDelta < ulFDelta)
                    {
                        ulFDelta = ulFTmpDelta;
                        // XG200ERPIXPLLCM M<1:0> O<7:3>
                        *piM = (CARD8)ulM | (CARD8)(ulO<<3);
                        //
                        // XG200ERPIXPLLCN N<6:0>
                        *piN = (CARD8)ulN;
                        //
                        // XG200ERPIXPLLCP R<1:0> cg<7:4> (Use R value)
                        *piP = (CARD8)ulR | (CARD8)(ulR<<3);

                        // Test
                        int ftest = (G200ER_PLLREF * (ulN+1)) / ((ulR+1) * aulMDivValue[ulM] * (ulO+1));
                        ftest=ftest;
                    }
                } // End O Loop
            } // End M Loop
        } // End N Loop
    } // End R Loop
}

static void
MGAG200ERPIXPLLSET(ScrnInfoPtr pScrn, MGARegPtr mgaReg)
{
    //TODO  G200ER Validate sequence 
    CARD8 ucPixCtrl, ucTempByte;
    MGAPtr pMga = MGAPTR(pScrn);


    // Set pixclkdis to 1
    ucPixCtrl = inMGAdac(MGA1064_PIX_CLK_CTL);
    ucPixCtrl |= MGA1064_PIX_CLK_CTL_CLK_DIS;
    outMGAdac(MGA1064_PIX_CLK_CTL, ucPixCtrl);

    ucTempByte = inMGAdac(MGA1064_REMHEADCTL);
    ucTempByte |= MGA1064_REMHEADCTL_CLKDIS;
    outMGAdac(MGA1064_REMHEADCTL, ucTempByte);

    // Select PLL Set C
    ucTempByte = INREG8(MGAREG_MEM_MISC_READ);
    ucTempByte |= (0x3<<2) | 0xc0; //select MGA pixel clock
    OUTREG8(MGAREG_MEM_MISC_WRITE, ucTempByte);

    ucPixCtrl &= ~MGA1064_PIX_CLK_CTL_CLK_DIS;
    ucPixCtrl |= MGA1064_PIX_CLK_CTL_CLK_POW_DOWN;
    outMGAdac(MGA1064_PIX_CLK_CTL, ucPixCtrl);

    // Wait 500 us
    usleep(500);

    // Program the Pixel PLL Register
    outMGAdac(MGA1064_ER_PIX_PLLC_N, mgaReg->PllN);
    outMGAdac(MGA1064_ER_PIX_PLLC_M, mgaReg->PllM);
    outMGAdac(MGA1064_ER_PIX_PLLC_P, mgaReg->PllP);

        // Wait 50 us
    usleep(50);

}

static void
MGAG200WBPrepareForModeSwitch(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);

    unsigned char ucTmpData = 0;
    int ulIterationMax = 0;
    // 1- The first step is to warn the BMC of an upcoming mode change.
    // We are putting the misc<0> to output.
    ucTmpData = inMGAdac(MGA1064_GEN_IO_CTL);
    ucTmpData |= 0x10;
    outMGAdac(MGA1064_GEN_IO_CTL, ucTmpData);

    // We are putting a 1 on the misc<0> line.
    ucTmpData = inMGAdac(MGA1064_GEN_IO_DATA);
    ucTmpData |= 0x10;
    outMGAdac(MGA1064_GEN_IO_DATA, ucTmpData);

    // 2- The second step is to mask any further scan request
    // This will be done by asserting the remfreqmsk bit (XSPAREREG<7>)
    ucTmpData = inMGAdac(MGA1064_SPAREREG);
    ucTmpData |= 0x80;
    outMGAdac(MGA1064_SPAREREG, ucTmpData);

    // 3a- The third step is to verify if there is an active scan
    // We are searching for a 0 on remhsyncsts (XSPAREREG<0>)
    ulIterationMax = 300;
    while (!(ucTmpData & 0x01) && ulIterationMax)
    {
        ucTmpData = inMGAdac(MGA1064_SPAREREG);
        usleep(1000);
        ulIterationMax--;
    }

    // 3b- This step occurs only if the remote is actually scanning
    // We are waiting for the end of the frame which is a 1 on 
    // remvsyncsts (XSPAREREG<1>)
    if (ulIterationMax)
    {
        ulIterationMax = 300;
        while ((ucTmpData & 0x02) && ulIterationMax)
        {
            ucTmpData = inMGAdac(MGA1064_SPAREREG);
            usleep(1000);
            ulIterationMax--;
        }
    }
}

static void
MGAG200WBRestoreFromModeSwitch(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);

    unsigned char ucTmpData = 0;

    // 1- The first step is to ensure that the vrsten and hrsten are set
    OUTREG8(MGAREG_CRTCEXT_INDEX, 0x01);
    ucTmpData = INREG8(MGAREG_CRTCEXT_DATA);
    OUTREG8(MGAREG_CRTCEXT_DATA, ucTmpData | 0x88);

    // 2- The second step is is to assert the rstlvl2
    ucTmpData = inMGAdac(MGA1064_REMHEADCTL2);
    ucTmpData |= 0x08;
    outMGAdac(MGA1064_REMHEADCTL2, ucTmpData);

    // - Wait for 10 us
    usleep(10);

    // 3- The next step is is to deassert the rstlvl2
    ucTmpData &= ~0x08;
    outMGAdac(MGA1064_REMHEADCTL2, ucTmpData);

    // - Wait for 10 us
    usleep(10);

    // 4- The fourth step is to remove the mask of scan request
    // This will be done by deasserting the remfreqmsk bit (XSPAREREG<7>)
    ucTmpData = inMGAdac(MGA1064_SPAREREG);
    ucTmpData &= ~0x80;
    outMGAdac(MGA1064_SPAREREG, ucTmpData);

    // 5- Finally, we are putting back a 0 on the misc<0> line.
    ucTmpData = inMGAdac(MGA1064_GEN_IO_DATA);
    ucTmpData &= ~0x10;
    outMGAdac(MGA1064_GEN_IO_DATA, ucTmpData);
}

static void
MGAG200EHPIXPLLSET(ScrnInfoPtr pScrn, MGARegPtr mgaReg)
{
    MGAPtr pMga = MGAPTR(pScrn);

    unsigned long ulFallBackCounter, ulLoopCount, ulLockCheckIterations = 0, ulTempCount, ulVCount;
    unsigned char ucTempByte, ucPixCtrl, ucPLLLocked = FALSE;
    unsigned char ucM;
    unsigned char ucN;
    unsigned char ucP;
    unsigned char ucS;

    while(ulLockCheckIterations <= 32 && ucPLLLocked == FALSE)
    {
        // Set pixclkdis to 1
        ucPixCtrl = inMGAdac(MGA1064_PIX_CLK_CTL);
        ucPixCtrl |= MGA1064_PIX_CLK_CTL_CLK_DIS;
        outMGAdac(MGA1064_PIX_CLK_CTL, ucPixCtrl);

        // Select PLL Set C
        ucTempByte = INREG8(MGAREG_MEM_MISC_READ);
        ucTempByte |= 0x3<<2; //select MGA pixel clock
        OUTREG8(MGAREG_MEM_MISC_WRITE, ucTempByte);

        ucPixCtrl |= MGA1064_PIX_CLK_CTL_CLK_POW_DOWN;
        ucPixCtrl &= ~0x80;
        outMGAdac(MGA1064_PIX_CLK_CTL, ucPixCtrl);

        // Wait 500 us
        usleep(500);

        // Program the Pixel PLL Register
        outMGAdac(MGA1064_EH_PIX_PLLC_N, mgaReg->PllN);
        outMGAdac(MGA1064_EH_PIX_PLLC_M, mgaReg->PllM);
        outMGAdac(MGA1064_EH_PIX_PLLC_P, mgaReg->PllP);

        // Wait 500 us
        usleep(500);

        // Select the pixel PLL by setting pixclksel to 1
        ucTempByte = inMGAdac(MGA1064_PIX_CLK_CTL);
        ucTempByte &= ~MGA1064_PIX_CLK_CTL_SEL_MSK;
        ucTempByte |= MGA1064_PIX_CLK_CTL_SEL_PLL;
        outMGAdac(MGA1064_PIX_CLK_CTL, ucTempByte);

        // Reset dotclock rate bit.
        OUTREG8(MGAREG_SEQ_INDEX, 1);
        ucTempByte = INREG8(MGAREG_SEQ_DATA);
        OUTREG8(MGAREG_SEQ_DATA, ucTempByte & ~0x8);

        // Set pixclkdis to 0 and pixplldn to 0
        ucTempByte = inMGAdac(MGA1064_PIX_CLK_CTL);
        ucTempByte &= ~MGA1064_PIX_CLK_CTL_CLK_DIS;
        ucTempByte &= ~MGA1064_PIX_CLK_CTL_CLK_POW_DOWN;
        outMGAdac(MGA1064_PIX_CLK_CTL, ucTempByte);

        // Poll VCount. If it increments twice inside 150us, 
        // we assume that the PLL has locked.
        ulLoopCount = 0;
        ulVCount = INREG(MGAREG_VCOUNT);

        while(ulLoopCount < 30 && ucPLLLocked == FALSE)
        {
            ulTempCount = INREG(MGAREG_VCOUNT);

            if(ulTempCount < ulVCount)
            {
                ulVCount = 0;
            }
            if ((ucTempByte - ulVCount) > 2)
            {
                ucPLLLocked = TRUE;
            }
            else
            {
                usleep(5);
            }
            ulLoopCount++;
        }
        ulLockCheckIterations++;
    }
}

/**
 * Calculate the PLL settings (m, n, p, s).
 *
 * For more information, refer to the Matrox "MGA1064SG Developer
 * Specification" (document 10524-MS-0100).  chapter 5.7.8. "PLLs Clocks
 * Generators"
 *
 * \param f_out   Desired clock frequency, measured in kHz.
 * \param best_m  Value of PLL 'm' register.
 * \param best_n  Value of PLL 'n' register.
 * \param p       Value of PLL 'p' register.
 * \param s       Value of PLL 's' filter register (pix pll clock only).
 */

static void
MGAGCalcClock ( ScrnInfoPtr pScrn, long f_out,
		int *best_m, int *best_n, int *p, int *s )
{
	MGAPtr pMga = MGAPTR(pScrn);
	int m, n;
	double f_vco;
	double m_err, calc_f;
	const double ref_freq = (double) pMga->bios.pll_ref_freq;
	const int feed_div_max = 127;
	const int in_div_min = 1;
	const int post_div_max = 7;
	int feed_div_min;
	int in_div_max;


	switch( pMga->Chipset )
	{
	case PCI_CHIP_MGA1064:
		feed_div_min = 100;
		in_div_max   = 31;
		break;
	case PCI_CHIP_MGAG400:
	case PCI_CHIP_MGAG550:
		feed_div_min = 7;
		in_div_max   = 31;
		break;
	case PCI_CHIP_MGAG200_SE_A_PCI:
	case PCI_CHIP_MGAG200_SE_B_PCI:
	case PCI_CHIP_MGAG100:
	case PCI_CHIP_MGAG100_PCI:
	case PCI_CHIP_MGAG200:
	case PCI_CHIP_MGAG200_PCI:
	default:
		feed_div_min = 7;
		in_div_max   = 6;
		break;
	}

	/* Make sure that f_min <= f_out */
	if ( f_out < ( pMga->bios.pixel.min_freq / 8))
		f_out = pMga->bios.pixel.min_freq / 8;

	/*
	 * f_pll = f_vco / (p+1)
	 * Choose p so that 
	 * pMga->bios.pixel.min_freq <= f_vco <= pMga->bios.pixel.max_freq
	 * we don't have to bother checking for this maximum limit.
	 */
	f_vco = ( double ) f_out;
	for ( *p = 0; *p <= post_div_max && f_vco < pMga->bios.pixel.min_freq;
		*p = *p * 2 + 1, f_vco *= 2.0);

	/* Initial amount of error for frequency maximum */
	m_err = f_out;

	/* Search for the different values of ( m ) */
	for ( m = in_div_min ; m <= in_div_max ; m++ )
	{
		/* see values of ( n ) which we can't use */
		for ( n = feed_div_min; n <= feed_div_max; n++ )
		{ 
			calc_f = ref_freq * (n + 1) / (m + 1) ;

			/*
			 * Pick the closest frequency.
			 */
			if ( abs(calc_f - f_vco) < m_err ) {
				m_err = abs(calc_f - f_vco);
				*best_m = m;
				*best_n = n;
			}
		}
	}
	
	/* Now all the calculations can be completed */
	f_vco = ref_freq * (*best_n + 1) / (*best_m + 1);

	/* Adjustments for filtering pll feed back */
	if ( (50000.0 <= f_vco)
	&& (f_vco < 100000.0) )
		*s = 0;	
	if ( (100000.0 <= f_vco)
	&& (f_vco < 140000.0) )
		*s = 1;	
	if ( (140000.0 <= f_vco)
	&& (f_vco < 180000.0) )
		*s = 2;	
	if ( (180000.0 <= f_vco) )
		*s = 3;	

#ifdef DEBUG
	xf86DrvMsg(pScrn->scrnIndex, X_INFO,
		   "f_out_requ =%ld f_pll_real=%.1f f_vco=%.1f n=0x%x m=0x%x p=0x%x s=0x%x\n",
		   f_out, (f_vco / (*p + 1)), f_vco, *best_n, *best_m, *p, *s );
#endif
}

/*
 * MGAGSetPCLK - Set the pixel (PCLK) clock.
 */
static void 
MGAGSetPCLK( ScrnInfoPtr pScrn, long f_out )
{
	MGAPtr pMga = MGAPTR(pScrn);
	MGARegPtr pReg = &pMga->ModeReg;

	/* Pixel clock values */
	int m, n, p, s;
        m = n = p = s = 0;

	if(MGAISGx50(pMga)) {
	    pReg->Clock = f_out;
	    if (pMga->Chipset == PCI_CHIP_MGAG550) {
		if (f_out < 45000) {
		    pReg->Pan_Ctl = 0x00;
		} else if (f_out < 55000) {
		    pReg->Pan_Ctl = 0x08;
		} else if (f_out < 70000) {
		    pReg->Pan_Ctl = 0x10;
		} else if (f_out < 85000) {
		    pReg->Pan_Ctl = 0x18;
		} else if (f_out < 100000) {
		    pReg->Pan_Ctl = 0x20;
		} else if (f_out < 115000) {
		    pReg->Pan_Ctl = 0x28;
		} else if (f_out < 125000) {
		    pReg->Pan_Ctl = 0x30;
		} else {
		    pReg->Pan_Ctl = 0x38;
		}
	    } else {
		if (f_out < 45000) {
		    pReg->Pan_Ctl = 0x00;
		} else if (f_out < 65000) {
		    pReg->Pan_Ctl = 0x08;
		} else if (f_out < 85000) {
		    pReg->Pan_Ctl = 0x10;
		} else if (f_out < 105000) {
		    pReg->Pan_Ctl = 0x18;
		} else if (f_out < 135000) {
		    pReg->Pan_Ctl = 0x20;
		} else if (f_out < 160000) {
		    pReg->Pan_Ctl = 0x28;
		} else if (f_out < 175000) {
		    pReg->Pan_Ctl = 0x30;
		} else {
		    pReg->Pan_Ctl = 0x38;
		}
	    }
	    return;
	}

	if (pMga->is_G200SE) {
	    MGAG200SEComputePLLParam(pScrn, f_out, &m, &n, &p);

	    pReg->DacRegs[ MGA1064_PIX_PLLC_M ] = m;
	    pReg->DacRegs[ MGA1064_PIX_PLLC_N ] = n;
	    pReg->DacRegs[ MGA1064_PIX_PLLC_P ] = p;
	} else if (pMga->is_G200EV) {
	    MGAG200EVComputePLLParam(pScrn, f_out, &m, &n, &p);

	    pReg->PllM = m;
	    pReg->PllN = n;
	    pReg->PllP = p;
	} else if (pMga->is_G200WB) {
	    MGAG200WBComputePLLParam(pScrn, f_out, &m, &n, &p);

	    pReg->PllM = m;
	    pReg->PllN = n;
	    pReg->PllP = p;
    } else if (pMga->is_G200EH) {
	    MGAG200EHComputePLLParam(pScrn, f_out, &m, &n, &p);

	    pReg->PllM = m;
	    pReg->PllN = n;
	    pReg->PllP = p;		
	} else if (pMga->is_G200ER) {
	    MGAG200ERComputePLLParam(pScrn, f_out, &m, &n, &p);
	    pReg->PllM = m;
	    pReg->PllN = n;
	    pReg->PllP = p;		
    } else {
	    /* Do the calculations for m, n, p and s */
	    MGAGCalcClock( pScrn, f_out, &m, &n, &p, &s );

	    /* Values for the pixel clock PLL registers */
	    pReg->DacRegs[ MGA1064_PIX_PLLC_M ] = m & 0x1F;
	    pReg->DacRegs[ MGA1064_PIX_PLLC_N ] = n & 0x7F;
	    pReg->DacRegs[ MGA1064_PIX_PLLC_P ] = (p & 0x07) |
						  ((s & 0x03) << 3);
	}
}

/*
 * MGAGInit 
 */
static Bool
MGAGInit(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
	/*
	 * initial values of the DAC registers
	 */
	const static unsigned char initDAC[] = {
	/* 0x00: */	   0,    0,    0,    0,    0,    0, 0x00,    0,
	/* 0x08: */	   0,    0,    0,    0,    0,    0,    0,    0,
	/* 0x10: */	   0,    0,    0,    0,    0,    0,    0,    0,
	/* 0x18: */	0x00,    0, 0xC9, 0xFF, 0xBF, 0x20, 0x1F, 0x20,
	/* 0x20: */	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	/* 0x28: */	0x00, 0x00, 0x00, 0x00,    0,    0,    0, 0x40,
	/* 0x30: */	0x00, 0xB0, 0x00, 0xC2, 0x34, 0x14, 0x02, 0x83,
	/* 0x38: */	0x00, 0x93, 0x00, 0x77, 0x00, 0x00, 0x00, 0x3A,
	/* 0x40: */	   0,    0,    0,    0,    0,    0,    0,    0,
	/* 0x48: */	   0,    0,    0,    0,    0,    0,    0,    0
	};

	int i;
	int hd, hs, he, ht, vd, vs, ve, vt, wd;
	int BppShift;
	MGAPtr pMga;
	MGARegPtr pReg;
	vgaRegPtr pVga;
	MGAFBLayout *pLayout;
	xMODEINFO ModeInfo;

	ModeInfo.ulDispWidth = mode->HDisplay;
        ModeInfo.ulDispHeight = mode->VDisplay;
        ModeInfo.ulFBPitch = mode->HDisplay; 
        ModeInfo.ulBpp = pScrn->bitsPerPixel;    
        ModeInfo.flSignalMode = 0;
        ModeInfo.ulPixClock = mode->Clock;
        ModeInfo.ulHFPorch = mode->HSyncStart - mode->HDisplay;
        ModeInfo.ulHSync = mode->HSyncEnd - mode->HSyncStart;
        ModeInfo.ulHBPorch = mode->HTotal - mode->HSyncEnd;
        ModeInfo.ulVFPorch = mode->VSyncStart - mode->VDisplay;
        ModeInfo.ulVSync = mode->VSyncEnd - mode->VSyncStart;
        ModeInfo.ulVBPorch = mode->VTotal - mode->VSyncEnd;

	pMga = MGAPTR(pScrn);
	pReg = &pMga->ModeReg;
	pVga = &VGAHWPTR(pScrn)->ModeReg;
	pLayout = &pMga->CurrentLayout;

	BppShift = pMga->BppShifts[(pLayout->bitsPerPixel >> 3) - 1];

	MGA_NOT_HAL(
	/* Allocate the DacRegs space if not done already */
	if (pReg->DacRegs == NULL) {
		pReg->DacRegs = xnfcalloc(DACREGSIZE, 1);
	}
	for (i = 0; i < DACREGSIZE; i++) {
	    pReg->DacRegs[i] = initDAC[i]; 
	}
	);	/* MGA_NOT_HAL */
	    
	switch(pMga->Chipset)
	{
	case PCI_CHIP_MGA1064:
		pReg->DacRegs[ MGA1064_SYS_PLL_M ] = 0x04;
		pReg->DacRegs[ MGA1064_SYS_PLL_N ] = 0x44;
		pReg->DacRegs[ MGA1064_SYS_PLL_P ] = 0x18;
		pReg->Option  = 0x5F094F21;
		pReg->Option2 = 0x00000000;
		break;
	case PCI_CHIP_MGAG100:
	case PCI_CHIP_MGAG100_PCI:
                pReg->DacRegs[MGA1064_VREF_CTL] = 0x03;

		if(pMga->HasSDRAM) {
		    if(pMga->OverclockMem) {
                        /* 220 Mhz */
			pReg->DacRegs[ MGA1064_SYS_PLL_M ] = 0x06;
			pReg->DacRegs[ MGA1064_SYS_PLL_N ] = 0x38;
			pReg->DacRegs[ MGA1064_SYS_PLL_P ] = 0x18;
		    } else {
                        /* 203 Mhz */
			pReg->DacRegs[ MGA1064_SYS_PLL_M ] = 0x01;
			pReg->DacRegs[ MGA1064_SYS_PLL_N ] = 0x0E;
			pReg->DacRegs[ MGA1064_SYS_PLL_P ] = 0x18;
		    }
		    pReg->Option = 0x404991a9;
		} else {
		    if(pMga->OverclockMem) {
                        /* 143 Mhz */
			pReg->DacRegs[ MGA1064_SYS_PLL_M ] = 0x06;
			pReg->DacRegs[ MGA1064_SYS_PLL_N ] = 0x24;
			pReg->DacRegs[ MGA1064_SYS_PLL_P ] = 0x10;
		    } else {
		        /* 124 Mhz */
			pReg->DacRegs[ MGA1064_SYS_PLL_M ] = 0x04;
			pReg->DacRegs[ MGA1064_SYS_PLL_N ] = 0x16;
			pReg->DacRegs[ MGA1064_SYS_PLL_P ] = 0x08;
		    }
		    pReg->Option = 0x4049d121;
		}
		pReg->Option2 = 0x0000007;
		break;
	case PCI_CHIP_MGAG400:
	case PCI_CHIP_MGAG550:
	       if (MGAISGx50(pMga))
		       break;

	       if(pMga->Dac.maxPixelClock == 360000) {  /* G400 MAX */
	           if(pMga->OverclockMem) {
			/* 150/200  */
			pReg->DacRegs[ MGA1064_SYS_PLL_M ] = 0x05;
			pReg->DacRegs[ MGA1064_SYS_PLL_N ] = 0x42;
			pReg->DacRegs[ MGA1064_SYS_PLL_P ] = 0x18;
			pReg->Option3 = 0x019B8419;
			pReg->Option = 0x50574120;
		   } else {
			/* 125/166  */
			pReg->DacRegs[ MGA1064_SYS_PLL_M ] = 0x02;
			pReg->DacRegs[ MGA1064_SYS_PLL_N ] = 0x1B;
			pReg->DacRegs[ MGA1064_SYS_PLL_P ] = 0x18;
			pReg->Option3 = 0x019B8419;
			pReg->Option = 0x5053C120;
		   } 
		} else {
	           if(pMga->OverclockMem) {
			/* 125/166  */
			pReg->DacRegs[ MGA1064_SYS_PLL_M ] = 0x02;
			pReg->DacRegs[ MGA1064_SYS_PLL_N ] = 0x1B;
			pReg->DacRegs[ MGA1064_SYS_PLL_P ] = 0x18;
			pReg->Option3 = 0x019B8419;
			pReg->Option = 0x5053C120;
		   } else {
			/* 110/166  */
			pReg->DacRegs[ MGA1064_SYS_PLL_M ] = 0x13;
			pReg->DacRegs[ MGA1064_SYS_PLL_N ] = 0x7A;
			pReg->DacRegs[ MGA1064_SYS_PLL_P ] = 0x08;
			pReg->Option3 = 0x0190a421;
			pReg->Option = 0x50044120;
		   } 
		}
		if(pMga->HasSDRAM)
		   pReg->Option &= ~(1 << 14);
		pReg->Option2 = 0x01003000;
		break;
	case PCI_CHIP_MGAG200_SE_A_PCI:
	case PCI_CHIP_MGAG200_SE_B_PCI:
        pReg->DacRegs[ MGA1064_VREF_CTL ] = 0x03;
                pReg->DacRegs[MGA1064_PIX_CLK_CTL] =
                    MGA1064_PIX_CLK_CTL_SEL_PLL;

                pReg->DacRegs[MGA1064_MISC_CTL] =
                    MGA1064_MISC_CTL_DAC_EN |
                    MGA1064_MISC_CTL_VGA8 |
                    MGA1064_MISC_CTL_DAC_RAM_CS;

		if (pMga->HasSDRAM)
		    pReg->Option = 0x40049120;
	        pReg->Option2 = 0x00008000;
		break;

        case PCI_CHIP_MGAG200_WINBOND_PCI:
                pReg->DacRegs[MGA1064_VREF_CTL] = 0x07;
                pReg->Option = 0x41049120;
                pReg->Option2 = 0x0000b000;
                break;

        case PCI_CHIP_MGAG200_EV_PCI:
                pReg->DacRegs[MGA1064_PIX_CLK_CTL] =
                    MGA1064_PIX_CLK_CTL_SEL_PLL;

                pReg->DacRegs[MGA1064_MISC_CTL] =
                    MGA1064_MISC_CTL_VGA8 |
                    MGA1064_MISC_CTL_DAC_RAM_CS;

                pReg->Option = 0x00000120;
                pReg->Option2 = 0x0000b000;
                break;

		case PCI_CHIP_MGAG200_ER_PCI:
			pReg->Dac_Index90 = 0;
			break;

        case PCI_CHIP_MGAG200_EH_PCI:
                pReg->DacRegs[MGA1064_MISC_CTL] =
                    MGA1064_MISC_CTL_VGA8 |
                    MGA1064_MISC_CTL_DAC_RAM_CS;

                pReg->Option = 0x00000120;
                pReg->Option2 = 0x0000b000;
                break;

	case PCI_CHIP_MGAG200:
	case PCI_CHIP_MGAG200_PCI:
	default:
		if(pMga->OverclockMem) {
                     /* 143 Mhz */
		    pReg->DacRegs[ MGA1064_SYS_PLL_M ] = 0x06;
		    pReg->DacRegs[ MGA1064_SYS_PLL_N ] = 0x24;
		    pReg->DacRegs[ MGA1064_SYS_PLL_P ] = 0x10;
		} else {
		    /* 124 Mhz */
		    pReg->DacRegs[ MGA1064_SYS_PLL_M ] = 0x04;
		    pReg->DacRegs[ MGA1064_SYS_PLL_N ] = 0x2D;
		    pReg->DacRegs[ MGA1064_SYS_PLL_P ] = 0x19;
		}
	        pReg->Option2 = 0x00008000;
		if(pMga->HasSDRAM)
		    pReg->Option = 0x40499121;
		else
		    pReg->Option = 0x4049cd21;
		break;
	}

	MGA_NOT_HAL(
	/* must always have the pci retries on but rely on 
	   polling to keep them from occuring */
	pReg->Option &= ~0x20000000;

	switch(pLayout->bitsPerPixel)
	{
	case 8:
		pReg->DacRegs[ MGA1064_MUL_CTL ] = MGA1064_MUL_CTL_8bits;
		break;
	case 16:
		pReg->DacRegs[ MGA1064_MUL_CTL ] = MGA1064_MUL_CTL_16bits;
		if ( (pLayout->weight.red == 5) && (pLayout->weight.green == 5)
					&& (pLayout->weight.blue == 5) ) {
		    pReg->DacRegs[ MGA1064_MUL_CTL ] = MGA1064_MUL_CTL_15bits;
		}
		break;
	case 24:
		pReg->DacRegs[ MGA1064_MUL_CTL ] = MGA1064_MUL_CTL_24bits;
		break;
	case 32:
	    pReg->DacRegs[ MGA1064_MUL_CTL ] = MGA1064_MUL_CTL_32_24bits;
	    break;
	default:
		FatalError("MGA: unsupported depth\n");
	}
	);	/* MGA_NOT_HAL */
		
	/*
	 * This will initialize all of the generic VGA registers.
	 */
	if (!vgaHWInit(pScrn, mode))
		return(FALSE);

	/*
	 * Here all of the MGA registers get filled in.
	 */
	hd = (mode->CrtcHDisplay	>> 3)	- 1;
	hs = (mode->CrtcHSyncStart	>> 3)	- 1;
	he = (mode->CrtcHSyncEnd	>> 3)	- 1;
	ht = (mode->CrtcHTotal		>> 3)	- 1;
	vd = mode->CrtcVDisplay			- 1;
	vs = mode->CrtcVSyncStart		- 1;
	ve = mode->CrtcVSyncEnd			- 1;
	vt = mode->CrtcVTotal			- 2;
	
	/* HTOTAL & 0x7 equal to 0x6 in 8bpp or 0x4 in 24bpp causes strange
	 * vertical stripes
	 */  
	if((ht & 0x07) == 0x06 || (ht & 0x07) == 0x04)
		ht++;
		
	if (pLayout->bitsPerPixel == 24)
		wd = (pLayout->displayWidth * 3) >> (4 - BppShift);
	else
		wd = pLayout->displayWidth >> (4 - BppShift);

	pReg->ExtVga[0] = 0;
	pReg->ExtVga[5] = 0;
	
	if (mode->Flags & V_INTERLACE)
	{
		pReg->ExtVga[0] = 0x80;
		pReg->ExtVga[5] = (hs + he - ht) >> 1;
		wd <<= 1;
		vt &= 0xFFFE;
	}

	pReg->ExtVga[0]	|= (wd & 0x300) >> 4;
	pReg->ExtVga[1]	= (((ht - 4) & 0x100) >> 8) |
				((hd & 0x100) >> 7) |
				((hs & 0x100) >> 6) |
				(ht & 0x40);
	pReg->ExtVga[2]	= ((vt & 0xc00) >> 10) |
				((vd & 0x400) >> 8) |
				((vd & 0xc00) >> 7) |
				((vs & 0xc00) >> 5) |
				((vd & 0x400) >> 3); /* linecomp */
	if (pLayout->bitsPerPixel == 24)
		pReg->ExtVga[3]	= (((1 << BppShift) * 3) - 1) | 0x80;
	else
		pReg->ExtVga[3]	= ((1 << BppShift) - 1) | 0x80;

        pReg->ExtVga[4]	= 0;

        if (pMga->is_G200WB){
            pReg->ExtVga[1] |= 0x88;
        }
	pReg->ExtVga_Index24 = 0x05;
		
	pVga->CRTC[0]	= ht - 4;
	pVga->CRTC[1]	= hd;
	pVga->CRTC[2]	= hd;
	pVga->CRTC[3]	= (ht & 0x1F) | 0x80;
	pVga->CRTC[4]	= hs;
	pVga->CRTC[5]	= ((ht & 0x20) << 2) | (he & 0x1F);
	pVga->CRTC[6]	= vt & 0xFF;
	pVga->CRTC[7]	= ((vt & 0x100) >> 8 ) |
				((vd & 0x100) >> 7 ) |
				((vs & 0x100) >> 6 ) |
				((vd & 0x100) >> 5 ) |
				((vd & 0x100) >> 4 ) | /* linecomp */
				((vt & 0x200) >> 4 ) |
				((vd & 0x200) >> 3 ) |
				((vs & 0x200) >> 2 );
	pVga->CRTC[9]	= ((vd & 0x200) >> 4) |
			  ((vd & 0x200) >> 3); /* linecomp */
	pVga->CRTC[16] = vs & 0xFF;
	pVga->CRTC[17] = (ve & 0x0F) | 0x20;
	pVga->CRTC[18] = vd & 0xFF;
	pVga->CRTC[19] = wd & 0xFF;
	pVga->CRTC[21] = vd & 0xFF;
	pVga->CRTC[22] = (vt + 1) & 0xFF;
	pVga->CRTC[24] = vd & 0xFF; /* linecomp */

	MGA_NOT_HAL(pReg->DacRegs[MGA1064_CURSOR_BASE_ADR_LOW] = pMga->FbCursorOffset >> 10);
	MGA_NOT_HAL(pReg->DacRegs[MGA1064_CURSOR_BASE_ADR_HI] = pMga->FbCursorOffset >> 18);
	
	if (pMga->SyncOnGreen) {
	    MGA_NOT_HAL(
                pReg->DacRegs[MGA1064_GEN_CTL] &=
                    ~MGA1064_GEN_CTL_SYNC_ON_GREEN_DIS;
            );

	    pReg->ExtVga[3] |= 0x40;
	}

	/* select external clock */
	pVga->MiscOutReg |= 0x0C; 

	MGA_NOT_HAL(
	if (mode->Flags & V_DBLSCAN)
		pVga->CRTC[9] |= 0x80;

	if(MGAISGx50(pMga)) {
		OUTREG(MGAREG_ZORG, 0);
	}

  	MGAGSetPCLK(pScrn, mode->Clock);
	);	/* MGA_NOT_HAL */

	/* This disables the VGA memory aperture */
	pVga->MiscOutReg &= ~0x02;

	/* Urgh. Why do we define our own xMODEINFO structure instead 
	 * of just passing the blinkin' DisplayModePtr? If we're going to
	 * just cut'n'paste routines from the HALlib, it would be better
	 * just to strip the MacroVision stuff out of the HALlib and release
	 * that, surely?
	 */
        /*********************  Second Crtc programming **************/
        /* Writing values to crtc2[] array */
        if (pMga->SecondCrtc)
        {
            MGACRTC2Get(pScrn, &ModeInfo); 
            MGACRTC2GetPitch(pScrn, &ModeInfo); 
            MGACRTC2GetDisplayStart(pScrn, &ModeInfo,0,0,0);
        }

#if X_BYTE_ORDER == X_BIG_ENDIAN
	/* Disable byte-swapping for big-endian architectures - the XFree
	   driver seems to like a little-endian framebuffer -ReneR */
	/* pReg->Option |= 0x80000000; */
	pReg->Option &= ~0x80000000;
#endif

	return(TRUE);
}

/*
 * MGAGLoadPalette
 */

static void
MGAPaletteLoadCallback(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);
    MGAPaletteInfo *pal = pMga->palinfo;
    int i;

    while (!(INREG8(0x1FDA) & 0x08)); 

    for(i = 0; i < 256; i++) {
	if(pal->update) {
	    outMGAdreg(MGA1064_WADR_PAL, i);
            outMGAdreg(MGA1064_COL_PAL, pal->red);
            outMGAdreg(MGA1064_COL_PAL, pal->green);
            outMGAdreg(MGA1064_COL_PAL, pal->blue);
	    pal->update = FALSE;
	}
	pal++;
    }
    pMga->PaletteLoadCallback = NULL;
}

void MGAGLoadPalette(
    ScrnInfoPtr pScrn, 
    int numColors, 
    int *indices,
    LOCO *colors,
    VisualPtr pVisual
){
    MGAPtr pMga = MGAPTR(pScrn);

     if(pMga->Chipset == PCI_CHIP_MGAG400 || pMga->Chipset == PCI_CHIP_MGAG550){ 
	 /* load them at the retrace in the block handler instead to 
	    work around some problems with static on the screen */
	while(numColors--) {
	    pMga->palinfo[*indices].update = TRUE;
	    pMga->palinfo[*indices].red   = colors[*indices].red;
	    pMga->palinfo[*indices].green = colors[*indices].green;
	    pMga->palinfo[*indices].blue  = colors[*indices].blue;
	    indices++;
	}
	pMga->PaletteLoadCallback = MGAPaletteLoadCallback;
	return;
    } else {
	while(numColors--) {
            outMGAdreg(MGA1064_WADR_PAL, *indices);
            outMGAdreg(MGA1064_COL_PAL, colors[*indices].red);
            outMGAdreg(MGA1064_COL_PAL, colors[*indices].green);
            outMGAdreg(MGA1064_COL_PAL, colors[*indices].blue);
	    indices++;
	}
    }
}

/*
 * MGAGRestorePalette
 */

static void
MGAGRestorePalette(ScrnInfoPtr pScrn, unsigned char* pntr)
{
    MGAPtr pMga = MGAPTR(pScrn);
    int i = 768;

    outMGAdreg(MGA1064_WADR_PAL, 0x00);
    while(i--)
	outMGAdreg(MGA1064_COL_PAL, *(pntr++));
}

/*
 * MGAGSavePalette
 */
static void
MGAGSavePalette(ScrnInfoPtr pScrn, unsigned char* pntr)
{
    MGAPtr pMga = MGAPTR(pScrn);
    int i = 768;

    outMGAdreg(MGA1064_RADR_PAL, 0x00);
    while(i--) 
	*(pntr++) = inMGAdreg(MGA1064_COL_PAL);
}

/*
 * MGAGRestore
 *
 * This function restores a video mode.	 It basically writes out all of
 * the registers that have previously been saved.
 */
static void 
MGAGRestore(ScrnInfoPtr pScrn, vgaRegPtr vgaReg, MGARegPtr mgaReg,
	       Bool restoreFonts)
{
	int i;
	MGAPtr pMga = MGAPTR(pScrn);
	CARD32 optionMask;

MGA_NOT_HAL(
        if (pMga->is_G200WB)
        {
            MGAG200WBPrepareForModeSwitch(pScrn);
        }
);

	/*
	 * Pixel Clock needs to be restored regardless if we use
	 * HALLib or not. HALlib doesn't do a good job restoring
	 * VESA modes. MATROX: hint, hint.
	 */
	if (MGAISGx50(pMga) && mgaReg->Clock) {
	    /* 
	     * With HALlib program only when restoring to console!
	     * To test this we check for Clock == 0.
	     */
	    MGAG450SetPLLFreq(pScrn, mgaReg->Clock);
	    outMGAdac(MGA1064_PAN_CTL, mgaReg->Pan_Ctl);
	    mgaReg->PIXPLLCSaved = FALSE;
	}

        if(!pMga->SecondCrtc) {
           /* Do not set the memory config for primary cards as it
              should be correct already. Only on little endian architectures
              since we need to modify the byteswap bit. -ReneR */
#if X_BYTE_ORDER == X_BIG_ENDIAN
           optionMask = OPTION1_MASK;
#else
           optionMask = (pMga->Primary) ? OPTION1_MASK_PRIMARY : OPTION1_MASK;
#endif

MGA_NOT_HAL(
	   /*
	    * Code is needed to get things back to bank zero.
	    */
	   
	   /* restore DAC registers 
	    * according to the docs we shouldn't write to reserved regs*/
	   for (i = 0; i < DACREGSIZE; i++) {
	      if( (i <= 0x03) ||
		  (i == 0x07) ||
		  (i == 0x0b) ||
		  (i == 0x0f) ||
		  ((i >= 0x13) && (i <= 0x17)) ||
		  (i == 0x1b) ||
		  (i == 0x1c) ||
		  ((i >= 0x1f) && (i <= 0x29)) ||
		  ((i >= 0x30) && (i <= 0x37)) ||
                  (MGAISGx50(pMga) && !mgaReg->PIXPLLCSaved &&
		   ((i == 0x2c) || (i == 0x2d) || (i == 0x2e) ||
		    (i == 0x4c) || (i == 0x4d) || (i == 0x4e))))
		 continue; 
	      if (pMga->is_G200SE
		  && ((i == 0x2C) || (i == 0x2D) || (i == 0x2E)))
	         continue;
	      if ( (pMga->is_G200EV || pMga->is_G200WB || pMga->is_G200EH) &&
		   (i >= 0x44) && (i <= 0x4E))
	         continue;
			 
	      outMGAdac(i, mgaReg->DacRegs[i]);
	   }
	   
		if (pMga->is_G200ER)
        {
			outMGAdac(0x90, mgaReg->Dac_Index90);
        }
   
	   if (!MGAISGx50(pMga)) {
	       /* restore pci_option register */
#ifdef XSERVER_LIBPCIACCESS
	       pci_device_cfg_write_bits(pMga->PciInfo, optionMask, 
					 mgaReg->Option, PCI_OPTION_REG);

	      if (pMga->Chipset != PCI_CHIP_MGA1064) {
		  pci_device_cfg_write_bits(pMga->PciInfo, OPTION2_MASK,
					    mgaReg->Option2, PCI_MGA_OPTION2);

		  if (pMga->Chipset == PCI_CHIP_MGAG400 
		      || pMga->Chipset == PCI_CHIP_MGAG550) {
		      pci_device_cfg_write_bits(pMga->PciInfo, OPTION3_MASK,
						mgaReg->Option3,
						PCI_MGA_OPTION3);
		  }
	      }
#else
	      /* restore pci_option register */
	      pciSetBitsLong(pMga->PciTag, PCI_OPTION_REG, optionMask,
			     mgaReg->Option);
	      if (pMga->Chipset != PCI_CHIP_MGA1064)
		 pciSetBitsLong(pMga->PciTag, PCI_MGA_OPTION2, OPTION2_MASK,
				mgaReg->Option2);
	      if (pMga->Chipset == PCI_CHIP_MGAG400 || pMga->Chipset == PCI_CHIP_MGAG550)
		 pciSetBitsLong(pMga->PciTag, PCI_MGA_OPTION3, OPTION3_MASK,
				mgaReg->Option3);
#endif
	   }

           if (pMga->is_G200ER) {
               MGAG200ERPIXPLLSET(pScrn, mgaReg);               
           } else  if (pMga->is_G200EV) {
               MGAG200EVPIXPLLSET(pScrn, mgaReg);
           } else if (pMga->is_G200WB) {
               MGAG200WBPIXPLLSET(pScrn, mgaReg);
           } else if (pMga->is_G200EH) {
               MGAG200EHPIXPLLSET(pScrn, mgaReg);
           }
);	/* MGA_NOT_HAL */
	   /* restore CRTCEXT regs */
           for (i = 0; i < 6; i++)
	      OUTREG16(MGAREG_CRTCEXT_INDEX, (mgaReg->ExtVga[i] << 8) | i);

           if (pMga->is_G200ER) {
               OUTREG8(MGAREG_CRTCEXT_INDEX, 0x24);
               OUTREG8(MGAREG_CRTCEXT_DATA,  mgaReg->ExtVga_Index24);			   
           }

	   /* This handles restoring the generic VGA registers. */
	   if (pMga->is_G200SE) {
 	      MGAG200SERestoreMode(pScrn, vgaReg);
	      if (restoreFonts)
	         MGAG200SERestoreFonts(pScrn, vgaReg);
	   } else {
	      vgaHWRestore(pScrn, vgaReg,
			VGA_SR_MODE | (restoreFonts ? VGA_SR_FONTS : 0));
	   }
  	   MGAGRestorePalette(pScrn, vgaReg->DAC); 
	   

           if (pMga->is_G200EV) {
               OUTREG16(MGAREG_CRTCEXT_INDEX, 6);
               OUTREG16(MGAREG_CRTCEXT_DATA, 0);
           }
		   
	   /*
	    * this is needed to properly restore start address
	    */
	   OUTREG16(MGAREG_CRTCEXT_INDEX, (mgaReg->ExtVga[0] << 8) | 0);

MGA_NOT_HAL(
           if (pMga->is_G200WB)
           {
               MGAG200WBRestoreFromModeSwitch(pScrn);
           }
);

	} else {
	   /* Second Crtc */
	   xMODEINFO ModeInfo;

MGA_NOT_HAL(
	   /* Enable Dual Head */
	   MGACRTC2Set(pScrn, &ModeInfo); 
	   MGAEnableSecondOutPut(pScrn, &ModeInfo); 
	   MGACRTC2SetPitch(pScrn, &ModeInfo); 
	   MGACRTC2SetDisplayStart(pScrn, &ModeInfo,0,0,0);
            
	   for (i = 0x80; i <= 0xa0; i ++) {
                if (i== 0x8d) {
		   i = 0x8f;
		   continue;
		}
                outMGAdac(i,   mgaReg->dac2[ i - 0x80]);
	   }

); /* MGA_NOT_HAL */

        } 

#ifdef DEBUG		
	ErrorF("Setting DAC:");
	for (i=0; i<DACREGSIZE; i++) {
#if 1
		if(!(i%16)) ErrorF("\n%02X: ",i);
		ErrorF("%02X ", mgaReg->DacRegs[i]);
#else
		if(!(i%8)) ErrorF("\n%02X: ",i);
		ErrorF("0x%02X, ", mgaReg->DacRegs[i]);
#endif
	}
	ErrorF("\nOPTION  = %08lX\n", mgaReg->Option);
	ErrorF("OPTION2 = %08lX\n", mgaReg->Option2);
	ErrorF("CRTCEXT:");
	for (i=0; i<6; i++) ErrorF(" %02X", mgaReg->ExtVga[i]);
	ErrorF("\n");
#endif
	
}

/*
 * MGAGSave
 *
 * This function saves the video state.
 */
static void
MGAGSave(ScrnInfoPtr pScrn, vgaRegPtr vgaReg, MGARegPtr mgaReg,
	    Bool saveFonts)
{
	int i;
	MGAPtr pMga = MGAPTR(pScrn);

	/*
	 * Pixel Clock needs to be restored regardless if we use
	 * HALLib or not. HALlib doesn't do a good job restoring
	 * VESA modes (s.o.). MATROX: hint, hint.
	 */
	if (MGAISGx50(pMga)) {
	    mgaReg->Pan_Ctl = inMGAdac(MGA1064_PAN_CTL);
	    mgaReg->Clock = MGAG450SavePLLFreq(pScrn);
	}

	if(pMga->SecondCrtc == TRUE) {
	   for(i = 0x80; i < 0xa0; i++)
	      mgaReg->dac2[i-0x80] = inMGAdac(i);

	   return;
	}

	MGA_NOT_HAL(
	/* Allocate the DacRegs space if not done already */
	if (mgaReg->DacRegs == NULL) {
		mgaReg->DacRegs = xnfcalloc(DACREGSIZE, 1);
	}
	);	/* MGA_NOT_HAL */

	/*
	 * Code is needed to get back to bank zero.
	 */
	OUTREG16(MGAREG_CRTCEXT_INDEX, 0x0004);
	
	/*
	 * This function will handle creating the data structure and filling
	 * in the generic VGA portion.
	 */
	if (pMga->is_G200SE) {
 	    MGAG200SESaveMode(pScrn, vgaReg);
	    if (saveFonts)
		MGAG200SESaveFonts(pScrn, vgaReg);
	} else {
	    vgaHWSave(pScrn, vgaReg, VGA_SR_MODE |
				     (saveFonts ? VGA_SR_FONTS : 0));
	}
	MGAGSavePalette(pScrn, vgaReg->DAC);
	/* 
	 * Work around another bug in HALlib: it doesn't restore the
	 * DAC width register correctly.
	 */

	MGA_NOT_HAL(
	/*
	 * The port I/O code necessary to read in the extended registers.
	 */
	for (i = 0; i < DACREGSIZE; i++)
		mgaReg->DacRegs[i] = inMGAdac(i);

        if (pMga->is_G200WB) {
            mgaReg->PllM = inMGAdac(MGA1064_WB_PIX_PLLC_M);
            mgaReg->PllN = inMGAdac(MGA1064_WB_PIX_PLLC_N);
            mgaReg->PllP = inMGAdac(MGA1064_WB_PIX_PLLC_P);
        } else if (pMga->is_G200EV) {
            mgaReg->PllM = inMGAdac(MGA1064_EV_PIX_PLLC_M);
            mgaReg->PllN = inMGAdac(MGA1064_EV_PIX_PLLC_N);
            mgaReg->PllP = inMGAdac(MGA1064_EV_PIX_PLLC_P);
        } else if (pMga->is_G200EH) {
            mgaReg->PllM = inMGAdac(MGA1064_EH_PIX_PLLC_M);
            mgaReg->PllN = inMGAdac(MGA1064_EH_PIX_PLLC_N);
            mgaReg->PllP = inMGAdac(MGA1064_EH_PIX_PLLC_P);
        } else if (pMga->is_G200ER) {
            mgaReg->PllM = inMGAdac(MGA1064_ER_PIX_PLLC_M);
            mgaReg->PllN = inMGAdac(MGA1064_ER_PIX_PLLC_N);
            mgaReg->PllP = inMGAdac(MGA1064_ER_PIX_PLLC_P);
            mgaReg->Dac_Index90 = inMGAdac(0x90);
        }

        mgaReg->PIXPLLCSaved = TRUE;

#ifdef XSERVER_LIBPCIACCESS
	pci_device_cfg_read_u32(pMga->PciInfo, & mgaReg->Option,
				PCI_OPTION_REG);
	pci_device_cfg_read_u32(pMga->PciInfo, & mgaReg->Option2,
				PCI_MGA_OPTION2);
#else
	mgaReg->Option = pciReadLong(pMga->PciTag, PCI_OPTION_REG);

	mgaReg->Option2 = pciReadLong(pMga->PciTag, PCI_MGA_OPTION2);
#endif
	if (pMga->Chipset == PCI_CHIP_MGAG400 || pMga->Chipset == PCI_CHIP_MGAG550)
#ifdef XSERVER_LIBPCIACCESS
		    pci_device_cfg_read_u32(pMga->PciInfo, & mgaReg->Option3,
					    PCI_MGA_OPTION3);
#else
	    mgaReg->Option3 = pciReadLong(pMga->PciTag, PCI_MGA_OPTION3);
#endif
	);	/* MGA_NOT_HAL */

	for (i = 0; i < 6; i++)
	{
		OUTREG8(MGAREG_CRTCEXT_INDEX, i);
		mgaReg->ExtVga[i] = INREG8(MGAREG_CRTCEXT_DATA);
	}
	if (pMga->is_G200ER)
	{
		OUTREG8(MGAREG_CRTCEXT_INDEX, 0x24);
		mgaReg->ExtVga_Index24 = INREG8(MGAREG_CRTCEXT_DATA);
	}

#ifdef DEBUG		
	ErrorF("Saved values:\nDAC:");
	for (i=0; i<DACREGSIZE; i++) {
#if 1
		if(!(i%16)) ErrorF("\n%02X: ",i);
		ErrorF("%02X ", mgaReg->DacRegs[i]);
#else
		if(!(i%8)) ErrorF("\n%02X: ",i);
		ErrorF("0x%02X, ", mgaReg->DacRegs[i]);
#endif
	}
	ErrorF("\nOPTION  = %08lX\n:", mgaReg->Option);
	ErrorF("OPTION2 = %08lX\nCRTCEXT:", mgaReg->Option2);
	for (i=0; i<6; i++) ErrorF(" %02X", mgaReg->ExtVga[i]);
	ErrorF("\n");
#endif
}

/****
 ***  HW Cursor
 */
static void
MGAGLoadCursorImage(ScrnInfoPtr pScrn, unsigned char *src)
{
    MGAPtr pMga = MGAPTR(pScrn);
    CARD32 *dst = (CARD32*)(pMga->FbBase + pMga->FbCursorOffset);
    int i = 128;
    
    /* swap bytes in each line */
    while( i-- ) {
        *dst++ = (src[4] << 24) | (src[5] << 16) | (src[6] << 8) | src[7];
        *dst++ = (src[0] << 24) | (src[1] << 16) | (src[2] << 8) | src[3];
        src += 8;
    }
}

static void 
MGAGShowCursor(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);
    /* Enable cursor - X-Windows mode */
    outMGAdac(MGA1064_CURSOR_CTL, 0x03);
}

static void 
MGAGShowCursorG100(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);
    /* Enable cursor - X-Windows mode */
    outMGAdac(MGA1064_CURSOR_CTL, 0x01);
}

static void
MGAGHideCursor(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);
    /* Disable cursor */
    outMGAdac(MGA1064_CURSOR_CTL, 0x00);
}

static void
MGAGSetCursorPosition(ScrnInfoPtr pScrn, int x, int y)
{
    MGAPtr pMga = MGAPTR(pScrn);
    x += 64;
    y += 64;

    /* cursor update must never occurs during a retrace period (pp 4-160) */
    while( INREG( MGAREG_Status ) & 0x08 );
    
    /* Output position - "only" 12 bits of location documented */
    OUTREG8( RAMDAC_OFFSET + MGA1064_CUR_XLOW, (x & 0xFF));
    OUTREG8( RAMDAC_OFFSET + MGA1064_CUR_XHI, (x & 0xF00) >> 8);
    OUTREG8( RAMDAC_OFFSET + MGA1064_CUR_YLOW, (y & 0xFF));
    OUTREG8( RAMDAC_OFFSET + MGA1064_CUR_YHI, (y & 0xF00) >> 8);
}


static void
MGAGSetCursorColors(ScrnInfoPtr pScrn, int bg, int fg)
{
    MGAPtr pMga = MGAPTR(pScrn);

    /* Background color */
    outMGAdac(MGA1064_CURSOR_COL0_RED,   (bg & 0x00FF0000) >> 16);
    outMGAdac(MGA1064_CURSOR_COL0_GREEN, (bg & 0x0000FF00) >> 8);
    outMGAdac(MGA1064_CURSOR_COL0_BLUE,  (bg & 0x000000FF));

    /* Foreground color */
    outMGAdac(MGA1064_CURSOR_COL1_RED,   (fg & 0x00FF0000) >> 16);
    outMGAdac(MGA1064_CURSOR_COL1_GREEN, (fg & 0x0000FF00) >> 8);
    outMGAdac(MGA1064_CURSOR_COL1_BLUE,  (fg & 0x000000FF));
}

static void
MGAGSetCursorColorsG100(ScrnInfoPtr pScrn, int bg, int fg)
{
    MGAPtr pMga = MGAPTR(pScrn);

    /* Background color */
    outMGAdac(MGA1064_CURSOR_COL1_RED,   (bg & 0x00FF0000) >> 16);
    outMGAdac(MGA1064_CURSOR_COL1_GREEN, (bg & 0x0000FF00) >> 8);
    outMGAdac(MGA1064_CURSOR_COL1_BLUE,  (bg & 0x000000FF));

    /* Foreground color */
    outMGAdac(MGA1064_CURSOR_COL2_RED,   (fg & 0x00FF0000) >> 16);
    outMGAdac(MGA1064_CURSOR_COL2_GREEN, (fg & 0x0000FF00) >> 8);
    outMGAdac(MGA1064_CURSOR_COL2_BLUE,  (fg & 0x000000FF));
}

static Bool 
MGAGUseHWCursor(ScreenPtr pScrn, CursorPtr pCurs)
{
    MGAPtr pMga = MGAPTR(xf86ScreenToScrn(pScrn));
   /* This needs to detect if its on the second dac */
    if( XF86SCRNINFO(pScrn)->currentMode->Flags & V_DBLSCAN )
    	return FALSE;
    if( pMga->SecondCrtc == TRUE )
     	return FALSE;
    return TRUE;
}


/*
 * According to mga-1064g.pdf pp215-216 (4-179 & 4-180) the low bits of
 * XGENIODATA and XGENIOCTL are connected to the 4 DDC pins, but don't say
 * which VGA line is connected to each DDC pin, so I've had to guess.
 *
 * DDC1 support only requires DDC_SDA_MASK,
 * DDC2 support requires DDC_SDA_MASK and DDC_SCL_MASK
 *
 * If we want DDC on second head (P2) then we must use DDC2 protocol (I2C)
 *
 * Be careful, DDC1 and DDC2 refer to protocols, DDC_P1 and DDC_P2 refer to
 * DDC data coming in on which videoport on the card 
 */
#define DDC_P1_SDA_MASK (1 << 1)
#define DDC_P1_SCL_MASK (1 << 3)

static const struct mgag_i2c_private {
    unsigned sda_mask;
    unsigned scl_mask;
} i2c_priv[] = {
    { (1 << 1), (1 << 3) },
    { (1 << 0), (1 << 2) },
    { (1 << 4), (1 << 5) },
    { (1 << 0), (1 << 1) },  /* G200SE, G200EV and G200WB I2C bits */
    { (1 << 1), (1 << 0) },  /* G200EH, G200ER I2C bits */
};


static unsigned int
MGAG_ddc1Read(ScrnInfoPtr pScrn)
{
  MGAPtr pMga = MGAPTR(pScrn);
  unsigned char val;
  int i2c_index;

  if (pMga->is_G200SE || pMga->is_G200WB || pMga->is_G200EV)
    i2c_index = 3;
  else if (pMga->is_G200EH || pMga->is_G200ER)
    i2c_index = 4;
  else
    i2c_index = 0;

  const struct mgag_i2c_private *p = & i2c_priv[i2c_index];
 
  /* Define the SDA as an input */
  outMGAdacmsk(MGA1064_GEN_IO_CTL, ~(p->scl_mask | p->sda_mask), 0);

  /* wait for Vsync */
  if (pMga->is_G200SE) {
    usleep(4);
  } else {
    while( INREG( MGAREG_Status ) & 0x08 );
    while( ! (INREG( MGAREG_Status ) & 0x08) );
  }

  /* Get the result */
  val = (inMGAdac(MGA1064_GEN_IO_DATA) & p->sda_mask);
  return val;
}

static void
MGAG_I2CGetBits(I2CBusPtr b, int *clock, int *data)
{
  ScrnInfoPtr pScrn = xf86Screens[b->scrnIndex];
  MGAPtr pMga = MGAPTR(pScrn);
    const struct mgag_i2c_private *p =
	(struct mgag_i2c_private *) b->DriverPrivate.ptr;
  unsigned char val;
  
   /* Get the result. */
   val = inMGAdac(MGA1064_GEN_IO_DATA);
   
   *clock = (val & p->scl_mask) != 0;
   *data  = (val & p->sda_mask) != 0;
#ifdef DEBUG
  ErrorF("MGAG_I2CGetBits(%p,...) val=0x%x, returns clock %d, data %d\n", b, val, *clock, *data);
#endif
}

/*
 * ATTENTION! - the DATA and CLOCK lines need to be tri-stated when
 * high. Therefore turn off output driver for the line to set line
 * to high. High signal is maintained by a 15k Ohm pull-up resistor.
 */
static void
MGAG_I2CPutBits(I2CBusPtr b, int clock, int data)
{
  ScrnInfoPtr pScrn = xf86Screens[b->scrnIndex]; 
  MGAPtr pMga = MGAPTR(pScrn);
    const struct mgag_i2c_private *p =
	(struct mgag_i2c_private *) b->DriverPrivate.ptr;
  unsigned char drv, val;

  val = (clock ? p->scl_mask : 0) | (data ? p->sda_mask : 0);
  drv = ((!clock) ? p->scl_mask : 0) | ((!data) ? p->sda_mask : 0);

  /* Write the values */
  outMGAdacmsk(MGA1064_GEN_IO_CTL, ~(p->scl_mask | p->sda_mask) , drv);
  outMGAdacmsk(MGA1064_GEN_IO_DATA, ~(p->scl_mask | p->sda_mask) , val);
#ifdef DEBUG
  ErrorF("MGAG_I2CPutBits(%p, %d, %d) val=0x%x\n", b, clock, data, val);
#endif
}


static I2CBusPtr
mgag_create_i2c_bus(const char *name, unsigned bus_index, unsigned scrn_index)
{
    I2CBusPtr I2CPtr = xf86CreateI2CBusRec();

    if (I2CPtr != NULL) {
	I2CPtr->BusName = name;
	I2CPtr->scrnIndex = scrn_index;
	I2CPtr->I2CPutBits = MGAG_I2CPutBits;
	I2CPtr->I2CGetBits = MGAG_I2CGetBits;
	I2CPtr->AcknTimeout = 5;
	I2CPtr->DriverPrivate.ptr = & i2c_priv[bus_index];

	if (!xf86I2CBusInit(I2CPtr)) {
	    xf86DestroyI2CBusRec(I2CPtr, TRUE, TRUE);
	    I2CPtr = NULL;
	}
    }
    
    return I2CPtr;
}

	
Bool
MGAG_i2cInit(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);

    if (pMga->SecondCrtc == FALSE) {
        int i2c_index;

        if (pMga->is_G200SE || pMga->is_G200WB || pMga->is_G200EV)
            i2c_index = 3;
        else if (pMga->is_G200EH || pMga->is_G200ER)
            i2c_index = 4;
        else
            i2c_index = 0;

	pMga->DDC_Bus1 = mgag_create_i2c_bus("DDC P1",
					     i2c_index, pScrn->scrnIndex);
	return (pMga->DDC_Bus1 != NULL);
    } else {
	/* We have a dual head setup on G-series, set up DDC #2. */
	pMga->DDC_Bus2 = mgag_create_i2c_bus("DDC P2", 1, pScrn->scrnIndex);

	if (pMga->DDC_Bus2 != NULL) {
	    /* 0xA0 is DDC EEPROM address */
	    if (!xf86I2CProbeAddress(pMga->DDC_Bus2, 0xA0)) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, "DDC #2 unavailable -> TV cable connected or no monitor connected!\n");
		pMga->Crtc2IsTV = TRUE;  /* assume for now.  We need to fix HAL interactions. */
	    }
	}

	/* Then try to set up MAVEN bus. */
	pMga->Maven_Bus = mgag_create_i2c_bus("MAVEN", 2, pScrn->scrnIndex);
	if (pMga->Maven_Bus != NULL) {
	    pMga->Maven = NULL;
	    pMga->Maven_Version = 0;

	    /* Try to detect the MAVEN. */
	    if (xf86I2CProbeAddress(pMga->Maven_Bus, MAVEN_READ)) {
		I2CDevPtr dp = xf86CreateI2CDevRec();
		if (dp) {
		    I2CByte maven_ver;

		    dp->DevName = "MGA-TVO";
		    dp->SlaveAddr = MAVEN_WRITE;
		    dp->pI2CBus = pMga->Maven_Bus;
		    if (!xf86I2CDevInit(dp)) {
			xf86DestroyI2CDevRec(dp, TRUE);
		    } else {
			pMga->Maven = dp;
			if (MGAMavenRead(pScrn, 0xB2, &maven_ver)) {
			    /* heuristic stolen from matroxfb */
			    pMga->Maven_Version = (maven_ver < 0x14)
				? 'B' : 'C';

			    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
				       "MAVEN revision MGA-TVO-%c detected (0x%x)\n",
				       pMga->Maven_Version, maven_ver);
			} else {
			    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Failed to determine MAVEN hardware version!\n");
			}
		    }
		}
	    }

	    if (pMga->Maven == NULL) {
		xf86DrvMsg(pScrn->scrnIndex, X_INFO, 
			   "Failed to register MGA-TVO I2C device!\n");
	    }
	}
    }

    return TRUE;
}


/*
 * MGAGRamdacInit
 * Handle broken G100 special.
 */
static void
MGAGRamdacInit(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);
    MGARamdacPtr MGAdac = &pMga->Dac;

    MGAdac->isHwCursor             = TRUE;
    MGAdac->CursorOffscreenMemSize = 1024;
    MGAdac->CursorMaxWidth         = 64;
    MGAdac->CursorMaxHeight        = 64;
    MGAdac->SetCursorPosition      = MGAGSetCursorPosition;
    MGAdac->LoadCursorImage        = MGAGLoadCursorImage;
    MGAdac->HideCursor             = MGAGHideCursor;
    if ((pMga->Chipset == PCI_CHIP_MGAG100) 
	|| (pMga->Chipset == PCI_CHIP_MGAG100)) {
      MGAdac->SetCursorColors        = MGAGSetCursorColorsG100;
      MGAdac->ShowCursor             = MGAGShowCursorG100;
    } else {
      MGAdac->SetCursorColors        = MGAGSetCursorColors;
      MGAdac->ShowCursor             = MGAGShowCursor;
    }
    MGAdac->UseHWCursor            = MGAGUseHWCursor;
    MGAdac->CursorFlags            =
#if X_BYTE_ORDER == X_LITTLE_ENDIAN
    				HARDWARE_CURSOR_BIT_ORDER_MSBFIRST |
#endif
    				HARDWARE_CURSOR_SOURCE_MASK_INTERLEAVE_64 |
    				HARDWARE_CURSOR_TRUECOLOR_AT_8BPP;

    MGAdac->LoadPalette 	   = MGAGLoadPalette;
    MGAdac->RestorePalette	   = MGAGRestorePalette;


    MGAdac->maxPixelClock = pMga->bios.pixel.max_freq;
    MGAdac->ClockFrom = X_PROBED;

    /* Disable interleaving and set the rounding value */
    pMga->Interleave = FALSE;

    pMga->Roundings[0] = 64;
    pMga->Roundings[1] = 32;
    pMga->Roundings[2] = 64;
    pMga->Roundings[3] = 32;

    /* Clear Fast bitblt flag */
    pMga->HasFBitBlt = FALSE;
}

void MGAGSetupFuncs(ScrnInfoPtr pScrn)
{
    MGAPtr pMga = MGAPTR(pScrn);

    pMga->PreInit = MGAGRamdacInit;
    pMga->Save = MGAGSave;
    pMga->Restore = MGAGRestore;
    pMga->ModeInit = MGAGInit;
    if ((!pMga->is_G200WB) && (!pMga->is_G200ER)) {
        pMga->ddc1Read = MGAG_ddc1Read;
        /* vgaHWddc1SetSpeed will only work if the card is in VGA mode */
        pMga->DDC1SetSpeed = vgaHWddc1SetSpeedWeak();
    } else {
        pMga->ddc1Read = NULL;
        pMga->DDC1SetSpeed = NULL;
    }
    pMga->i2cInit = MGAG_i2cInit;
}

