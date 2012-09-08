/*********************************************************************
*  	G450: This is for Dual Head. 
*       Matrox Graphics
*       Author : Luugi Marsan
**********************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* All drivers should typically include these */
#include "xf86.h"
#include "xf86_OSproc.h"

/* Drivers that need to access the PCI config space directly need this */  
#include "xf86Pci.h"

#include "mga_reg.h"
#include "mga.h"

#define MNP_TABLE_SIZE 64
#define CLKSEL_MGA     0x0c
#define PLLLOCK        0x40

/* Misc field*/
#define IOADDSEL        0x01
#define RAMMAPEN        0x02
#define CLKSEL_25175    0x00
#define CLKSEL_28322    0x04
#define CLKSEL_MGA      0x0c
#define VIDEODIS        0x10
#define HPGODDEV        0x20
#define HSYNCPOL        0x40
#define VSYNCPOL        0x80

/* XSYNCCTRL field */
#define XSYNCCTRL_DAC1HSPOL_SHIFT                   2
#define XSYNCCTRL_DAC1HSPOL_MASK                    (1 << XSYNCCTRL_DAC1HSPOL_SHIFT)
#define XSYNCCTRL_DAC1HSPOL_NEG                     (1 << XSYNCCTRL_DAC1HSPOL_SHIFT)
#define XSYNCCTRL_DAC1HSPOL_POS                     0
#define XSYNCCTRL_DAC1VSPOL_SHIFT                   3
#define XSYNCCTRL_DAC1VSPOL_MASK                    (1 << XSYNCCTRL_DAC1VSPOL_SHIFT)
#define XSYNCCTRL_DAC1VSPOL_NEG                     (1 << XSYNCCTRL_DAC1VSPOL_SHIFT)
#define XSYNCCTRL_DAC1VSPOL_POS                     0
#define XSYNCCTRL_DAC2HSPOL_SHIFT                   6
#define XSYNCCTRL_DAC2HSPOL_MASK                    (1 << XSYNCCTRL_DAC2HSPOL_SHIFT)
#define XSYNCCTRL_DAC2HSPOL_NEG                     (1 << XSYNCCTRL_DAC2HSPOL_SHIFT)
#define XSYNCCTRL_DAC2HSPOL_POS                     0
#define XSYNCCTRL_DAC2VSPOL_SHIFT                   7
#define XSYNCCTRL_DAC2VSPOL_MASK                    (1 << XSYNCCTRL_DAC2VSPOL_SHIFT)
#define XSYNCCTRL_DAC2VSPOL_NEG                     (1 << XSYNCCTRL_DAC2VSPOL_SHIFT)
#define XSYNCCTRL_DAC2VSPOL_POS                     0
#define XSYNCCTRL_DAC1HSOFF_SHIFT                   0
#define XSYNCCTRL_DAC1HSOFF_MASK                    (1 << XSYNCCTRL_DAC1HSOFF_SHIFT)
#define XSYNCCTRL_DAC1HSOFF_OFF                     (1 << XSYNCCTRL_DAC1HSOFF_SHIFT)
#define XSYNCCTRL_DAC1HSOFF_ON                      1
#define XSYNCCTRL_DAC1VSOFF_SHIFT                   1
#define XSYNCCTRL_DAC1VSOFF_MASK                    (1 << XSYNCCTRL_DAC1VSOFF_SHIFT)
#define XSYNCCTRL_DAC1VSOFF_OFF                     (1 << XSYNCCTRL_DAC1VSOFF_SHIFT)
#define XSYNCCTRL_DAC1VSOFF_ON                      0
#define XSYNCCTRL_DAC2HSOFF_SHIFT                   4
#define XSYNCCTRL_DAC2HSOFF_MASK                    (1 << XSYNCCTRL_DAC2HSOFF_SHIFT)
#define XSYNCCTRL_DAC2HSOFF_OFF                     (1 << XSYNCCTRL_DAC2HSOFF_SHIFT)
#define XSYNCCTRL_DAC2HSOFF_ON                      0
#define XSYNCCTRL_DAC2VSOFF_SHIFT                   5
#define XSYNCCTRL_DAC2VSOFF_MASK                    (1 << XSYNCCTRL_DAC2VSOFF_SHIFT)
#define XSYNCCTRL_DAC2VSOFF_OFF                     (1 << XSYNCCTRL_DAC2VSOFF_SHIFT)
#define XSYNCCTRL_DAC2VSOFF_ON                      0

#define POS_HSYNC                  0x00000004
#define POS_VSYNC                  0x00000008


/* Set CRTC 2*/
/* Uses the mode given by xfree86 to setup the registry */
/* Does not write to the hard yet */
void MGACRTC2Get(ScrnInfoPtr pScrn, xMODEINFO *pModeInfo)
{
    
    
    MGAPtr pMga = MGAPTR(pScrn);
    MGARegPtr pReg = &pMga->ModeReg;
    
    xMODEINFO tmpModeInfo;
    CARD32 ulHTotal;
    CARD32 ulHDispEnd;
    CARD32 ulHBlkStr;
    CARD32 ulHSyncStr;
    CARD32 ulHSyncEnd;
    CARD32 ulVTotal;
    CARD32 ulVDispEnd;
    CARD32 ulVBlkStr;
    CARD32 ulVSyncStr;
    CARD32 ulVSyncEnd;
    CARD32 ulOffset;
    CARD32 ulCtl2;
    CARD32 ulDataCtl2;
    CARD32 ulDispHeight = pModeInfo->ulDispHeight;
    
#ifdef DEBUG 
    ErrorF("ENTER MGACRTC2Get\n");
#endif
    
    tmpModeInfo = *pModeInfo;
    
    
    /*  First compute the Values */
    
    ulHTotal = tmpModeInfo.ulDispWidth +
        tmpModeInfo.ulHFPorch +
        tmpModeInfo.ulHBPorch +
        tmpModeInfo.ulHSync;
    
    ulHDispEnd = tmpModeInfo.ulDispWidth;
    ulHBlkStr  = ulHDispEnd;
    ulHSyncStr = ulHBlkStr + tmpModeInfo.ulHFPorch;
    ulHSyncEnd = ulHSyncStr + tmpModeInfo.ulHSync;
    
    ulVTotal =  ulDispHeight +
        tmpModeInfo.ulVFPorch +
        tmpModeInfo.ulVBPorch +
        tmpModeInfo.ulVSync;
    
    
    ulVDispEnd = ulDispHeight;
    ulVBlkStr = ulVDispEnd;
    ulVSyncStr = ulVBlkStr + tmpModeInfo.ulVFPorch;
    ulVSyncEnd = ulVSyncStr + tmpModeInfo.ulVSync;
    
    ulOffset = tmpModeInfo.ulFBPitch;
    
    
    
    ulCtl2 = INREG(MGAREG_C2CTL);
    ulDataCtl2 = INREG(MGAREG_C2DATACTL);
    
    ulCtl2      &= 0xFF1FFFFF;
    ulDataCtl2  &= 0xFFFFFF00;
    
    switch (tmpModeInfo.ulBpp)
    {
    case 15:    ulCtl2      |= 0x00200000;
        ulOffset <<= 1;
        break;
    case 16:    ulCtl2      |= 0x00400000;
        ulOffset <<= 1;
        break;
    case 32:    ulCtl2      |= 0x00800000;
        ulOffset <<= 2;
        break;
    }
    
    
    pReg->crtc2[ MGAREG2_C2CTL ] = ulCtl2;
    pReg->crtc2[ MGAREG2_C2DATACTL ] = ulDataCtl2;
    
    /* Horizontal Value*/
    pReg->crtc2[MGAREG2_C2HPARAM] = (((ulHDispEnd-8) << 16) | (ulHTotal-8)) ;
    pReg->crtc2[MGAREG2_C2HSYNC] = (((ulHSyncEnd-8) << 16) | (ulHSyncStr-8)) ;  
    
    
    /*Vertical Value*/
    pReg->crtc2[MGAREG2_C2VPARAM] = (((ulVDispEnd-1) << 16) | (ulVTotal-1))  ;  
    pReg->crtc2[MGAREG2_C2VSYNC] =  (((ulVSyncEnd-1) << 16) | (ulVSyncStr-1)) ;  
    
    /** Offset value*/
    
    pReg->crtc2[MGAREG2_C2OFFSET] = ulOffset;

#ifdef DEBUG
    ErrorF("EXIT MGACRTC2Get\n");
#endif
    
}

/* Set CRTC 2*/
/* Writes to the hardware */
void MGACRTC2Set(ScrnInfoPtr pScrn, xMODEINFO *pModeInfo)
{
    
    
    MGAPtr pMga = MGAPTR(pScrn);
    MGARegPtr pReg = &pMga->ModeReg;

#ifdef DEBUG
    ErrorF("ENTER MGACRTC2Set\n");
#endif 

    
    /* This writes to the registers manually */
    OUTREG(MGAREG_C2CTL, pReg->crtc2[MGAREG2_C2CTL]);     
    OUTREG(MGAREG_C2DATACTL,pReg->crtc2[MGAREG2_C2DATACTL]);  
    
    
    /* Horizontal Value*/
    OUTREG(MGAREG_C2HPARAM, pReg->crtc2[MGAREG2_C2HPARAM]);      
    OUTREG(MGAREG_C2HSYNC, pReg->crtc2[MGAREG2_C2HSYNC]);  
    
    
    /*Vertical Value*/
    OUTREG(MGAREG_C2VPARAM, pReg->crtc2[MGAREG2_C2VPARAM]);  
    OUTREG(MGAREG_C2VSYNC,  pReg->crtc2[MGAREG2_C2VSYNC]);  
    
    /** Offset value*/
    
    OUTREG(MGAREG_C2OFFSET, pReg->crtc2[MGAREG2_C2OFFSET]);
#ifdef DEBUG    
    ErrorF("EXIT MGACRTC2Set\n");
#endif
    
}


/* Set CRTC2 on the right output */
void MGAEnableSecondOutPut(ScrnInfoPtr pScrn, xMODEINFO *pModeInfo)
{
    CARD8   ucByte, ucXDispCtrl;
    CARD32   ulC2CTL;
    MGAPtr pMga = MGAPTR(pScrn);
    MGARegPtr pReg;
    pReg = &pMga->ModeReg;
#ifdef DEBUG
    ErrorF("ENTER MGAEnableSecondOutPut\n");
#endif


    /*  Route Video PLL on second CRTC */
    ulC2CTL = INREG( MGAREG_C2CTL);

    /*--- Disable Pixel clock oscillations On Crtc1 */
    OUTREG( MGAREG_C2CTL, ulC2CTL | MGAREG_C2CTL_PIXCLKDIS_MASK);
    /*--- Have to wait minimum time (2 acces will be ok) */
    (void) INREG( MGAREG_Status);
    (void) INREG( MGAREG_Status);
    
    
    ulC2CTL &= ~MGAREG_C2CTL_PIXCLKSEL_MASK;
    ulC2CTL &= ~MGAREG_C2CTL_PIXCLKSELH_MASK;
    
    ulC2CTL |= MGAREG_C2CTL_PIXCLKSEL_VIDEOPLL;

    
    OUTREG( MGAREG_C2CTL, ulC2CTL);

    /*--- Enable Pixel clock oscillations on CRTC2*/
    ulC2CTL &= ~MGAREG_C2CTL_PIXCLKDIS_MASK;
    OUTREG( MGAREG_C2CTL, ulC2CTL);
    
    
    /* We don't use MISC synch pol, must be 0*/
    ucByte = INREG8( MGAREG_MEM_MISC_READ);
    
    OUTREG8(MGAREG_MEM_MISC_WRITE, (CARD8)(ucByte & ~(HSYNCPOL| VSYNCPOL) ));

    


    /* Set Rset to 0.7 V*/
    ucByte = inMGAdac(MGA1064_GEN_IO_CTL);
    ucByte &= ~0x40;
    pReg->DacRegs[MGA1064_GEN_IO_CTL] = ucByte;   
    outMGAdac (MGA1064_GEN_IO_CTL, ucByte);
    
    ucByte = inMGAdac( MGA1064_GEN_IO_DATA);
    ucByte &= ~0x40;
    pReg->DacRegs[MGA1064_GEN_IO_DATA]= ucByte;
    outMGAdac (MGA1064_GEN_IO_DATA, ucByte);
   
    /* Since G550 can swap outputs at BIOS initialisation, we must check which
     * DAC is 'logically' used as the secondary (don't assume its DAC2 anymore) */
    
    ulC2CTL = INREG(MGAREG_C2CTL);
    ucXDispCtrl = inMGAdac(MGA1064_DISP_CTL);

    ucXDispCtrl &= ~MGA1064_DISP_CTL_DAC2OUTSEL_MASK;

    if (!pMga->SecondOutput) {
        /* Route Crtc2 on Output1 */
        ucXDispCtrl |=  MGA1064_DISP_CTL_DAC2OUTSEL_CRTC1;
        ulC2CTL |= MGAREG_C2CTL_CRTCDACSEL_CRTC2;
    }
    else {
        /* Route Crtc2 on Output2*/
        ucXDispCtrl |=  MGA1064_DISP_CTL_DAC2OUTSEL_CRTC2;
        ulC2CTL &= ~MGAREG_C2CTL_CRTCDACSEL_MASK;
    }
    
    /* Enable CRTC2*/
    ulC2CTL |= MGAREG_C2CTL_C2_EN;

    pReg->dac2[ MGA1064_DISP_CTL - 0x80] =  ucXDispCtrl; 



    OUTREG( MGAREG_C2CTL,  ulC2CTL);

   /* Set DAC2 Synch polarity*/
    ucByte = inMGAdac( MGA1064_SYNC_CTL);
    ucByte &= ~(XSYNCCTRL_DAC2HSPOL_MASK | XSYNCCTRL_DAC2VSPOL_MASK);
    if ( !(pModeInfo->flSignalMode & POS_HSYNC) )
    {
        ucByte |= XSYNCCTRL_DAC2HSPOL_NEG;
    }
    if ( !(pModeInfo->flSignalMode & POS_VSYNC) )
    {
        ucByte |= XSYNCCTRL_DAC2VSPOL_NEG;
    }

   /* Enable synch output*/
    ucByte &= ~(XSYNCCTRL_DAC2HSOFF_MASK | XSYNCCTRL_DAC2VSOFF_MASK);
    pReg->dac2[ MGA1064_SYNC_CTL - 0x80] = ucByte;

   /* Power up DAC2, Fifo.
    * The TMDS is powered down here, which is likely wrong.
    */
    pReg->dac2[MGA1064_PWR_CTL - 0x80] =
        MGA1064_PWR_CTL_DAC2_EN |
        MGA1064_PWR_CTL_VID_PLL_EN |
        MGA1064_PWR_CTL_RFIFO_EN |
        MGA1064_PWR_CTL_CFIFO_EN;

    
#ifdef DEBUG
    ErrorF("EXIT MGAEnableSecondOutPut\n");
#endif
}





void MGACRTC2GetPitch (ScrnInfoPtr pScrn, xMODEINFO *pModeInfo)
{
    CARD32 ulOffset;
    MGAPtr pMga = MGAPTR(pScrn);
    MGARegPtr pReg;
    
    pReg = &pMga->ModeReg;
#ifdef DEBUG
    ErrorF("ENTER MGACRTC2GetPitch\n");
#endif

    
    switch(pModeInfo->ulBpp)
    {
        case 15:
        case 16:
            ulOffset = pModeInfo->ulFBPitch * 2;
            break;
        case 32:
            ulOffset = pModeInfo->ulFBPitch * 4;
            break;
	default:	/* Muffle compiler */
            ulOffset = pModeInfo->ulFBPitch;
	    break;
    }

    pReg->crtc2[MGAREG2_C2OFFSET] = ulOffset;
    
#ifdef DEBUG
    ErrorF("EXIT MGACRTC2GetPitch\n");
#endif

}

void MGACRTC2SetPitch (ScrnInfoPtr pScrn, xMODEINFO *pModeInfo)
{

    MGAPtr pMga = MGAPTR(pScrn);
    MGARegPtr pReg;
    pReg = &pMga->ModeReg;
    
#ifdef DEBUG
    ErrorF("ENTER CRCT2SetPitch\n");
#endif

    
    OUTREG(MGAREG_C2OFFSET,  pReg->crtc2[MGAREG2_C2OFFSET]);  
#ifdef DEBUG
    ErrorF("EXIT CRCT2SetPitch\n");
#endif
    
}


    /* Set Display Start*/
    /* base in bytes*/
void MGACRTC2GetDisplayStart (ScrnInfoPtr pScrn, xMODEINFO *pModeInfo, CARD32 base, CARD32 ulX, CARD32 ulY)
{

    CARD32 ulAddress;
   MGAPtr pMga = MGAPTR(pScrn);
    MGARegPtr pReg;
    pReg = &pMga->ModeReg;

#ifdef DEBUG
    ErrorF("ENTER MGACRTC2GetDisplayStart\n");
#endif


    pReg = &pMga->ModeReg;

   ulAddress       = (pModeInfo->ulFBPitch * ulY + ulX);
    switch(pModeInfo->ulBpp)
    {
        case 15:
        case 16:
            ulAddress <<= 1;
            break;
        case 32:
            ulAddress <<= 2;
            break;
    }

    pReg->crtc2[MGAREG2_C2STARTADD0] = ulAddress + base;
#ifdef DEBUG
    ErrorF("EXIT MGACRTC2GetDisplayStart\n");
#endif
    
}

void MGACRTC2SetDisplayStart (ScrnInfoPtr pScrn, xMODEINFO *pModeInfo, CARD32 base, CARD32 ulX, CARD32 ulY)
{
    MGAPtr pMga = MGAPTR(pScrn);
    MGARegPtr pReg;
    pReg = &pMga->ModeReg;
#ifdef DEBUG
    ErrorF("ENTER MGACRTC2SetDisplayStart\n");
#endif
    
    OUTREG(MGAREG_C2STARTADD0,  pReg->crtc2[MGAREG2_C2STARTADD0]);  
#ifdef DEBUG
    ErrorF("EXIT MGACRTC2SetDisplayStart\n");
#endif
    
}











