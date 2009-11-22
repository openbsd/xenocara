/*
 * Copyright (c) 2005 ASPEED Technology Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the authors not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The authors makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86cmap.h"
#include "compiler.h"
#include "mibstore.h"
#include "vgaHW.h"
#include "mipointer.h"
#include "micmap.h"

#include "fb.h"
#include "regionstr.h"
#include "xf86xv.h"
#include <X11/extensions/Xv.h>
#include "vbe.h"

#include "xf86PciInfo.h"
#include "xf86Pci.h"

/* framebuffer offscreen manager */
#include "xf86fbman.h"

/* include xaa includes */
#include "xaa.h"
#include "xaarop.h"

/* H/W cursor support */
#include "xf86Cursor.h"

/* Driver specific headers */
#include "ast.h"

/* Prototype type declaration*/
void vASTOpenKey(ScrnInfoPtr pScrn);
Bool bASTRegInit(ScrnInfoPtr pScrn);
void GetDRAMInfo(ScrnInfoPtr pScrn);
ULONG GetVRAMInfo(ScrnInfoPtr pScrn);
ULONG GetMaxDCLK(ScrnInfoPtr pScrn);
void GetChipType(ScrnInfoPtr pScrn);
void vAST1000DisplayOn(ASTRecPtr pAST);
void vAST1000DisplayOff(ASTRecPtr pAST);
void ASTBlankScreen(ScrnInfoPtr pScrn, Bool unblack);
void vSetStartAddressCRT1(ASTRecPtr pAST, ULONG base);
void vASTLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors, VisualPtr pVisual);
void ASTDisplayPowerManagementSet(ScrnInfoPtr pScrn, int PowerManagementMode, int flags);
Bool GetVGA2EDID(ScrnInfoPtr pScrn, unsigned char *pEDIDBuffer);
void vInitDRAMReg(ScrnInfoPtr pScrn);
Bool bIsVGAEnabled(ScrnInfoPtr pScrn);
Bool InitVGA(ScrnInfoPtr pScrn);

void
vASTOpenKey(ScrnInfoPtr pScrn)
{   	
   ASTRecPtr pAST = ASTPTR(pScrn);
   
   SetIndexReg(CRTC_PORT,0x80, 0xA8);     
   
}

Bool
bASTRegInit(ScrnInfoPtr pScrn)
{
   ASTRecPtr pAST = ASTPTR(pScrn);

   /* Enable MMIO */
   SetIndexRegMask(CRTC_PORT,0xA1, 0xFF, 0x04);

   return (TRUE);
   	
}

void
GetDRAMInfo(ScrnInfoPtr pScrn)
{
    ASTRecPtr pAST = ASTPTR(pScrn);
    ULONG ulData;
    
    if ( (pAST->jChipType != AST2000) )
    {
        *(ULONG *) (pAST->MMIOVirtualAddr + 0xF004) = 0x1e6e0000;
        *(ULONG *) (pAST->MMIOVirtualAddr + 0xF000) = 0x1;
                        
        *(ULONG *) (pAST->MMIOVirtualAddr + 0x10000) = 0xFC600309;
        do {
           ; 	
        } while (*(volatile ULONG *) (pAST->MMIOVirtualAddr + 0x10000) != 0x01);
        
        ulData = *(volatile ULONG *) (pAST->MMIOVirtualAddr + 0x10004);
        
        switch (ulData & 0x0C)
        {
        case 0x00:	
        case 0x04:
            pAST->jDRAMType = DRAMTYPE_512Mx16;
            break;        
        
        case 0x08:
            if (ulData & 0x40)		/* 16bits */
                pAST->jDRAMType = DRAMTYPE_1Gx16;
            else			/* 32bits */
                pAST->jDRAMType = DRAMTYPE_512Mx32;
            break;
            
        case 0x0C:
            pAST->jDRAMType = DRAMTYPE_1Gx32;
            break;
        }	
   }	
	
} /* GetDRAMInfo */

ULONG
GetVRAMInfo(ScrnInfoPtr pScrn)
{
   ASTRecPtr pAST = ASTPTR(pScrn);
   UCHAR jReg;

   vASTOpenKey(pScrn);

   GetIndexRegMask(CRTC_PORT, 0xAA, 0xFF, jReg);  

   switch (jReg & 0x03)
   {
   case 0x00:
       return (VIDEOMEM_SIZE_08M);           
   case 0x01:
       return (VIDEOMEM_SIZE_16M);                  
   case 0x02:
       return (VIDEOMEM_SIZE_32M);                 
   case 0x03:	
       return (VIDEOMEM_SIZE_64M); 
   }                     
 
   return (DEFAULT_VIDEOMEM_SIZE);
   	
}

ULONG
GetMaxDCLK(ScrnInfoPtr pScrn)
{
   ASTRecPtr pAST = ASTPTR(pScrn);
   UCHAR jReg;
   ULONG ulData, ulData2;
   ULONG ulRefPLL, ulDeNumerator, ulNumerator, ulDivider;
   ULONG ulDRAMBusWidth, ulMCLK, ulDRAMBandwidth, ActualDRAMBandwidth, DRAMEfficiency = 500;
   ULONG ulDCLK;
	
   vASTOpenKey(pScrn);

   *(ULONG *) (pAST->MMIOVirtualAddr + 0xF004) = 0x1e6e0000;
   *(ULONG *) (pAST->MMIOVirtualAddr + 0xF000) = 0x00000001;

   *(ULONG *) (pAST->MMIOVirtualAddr + 0x10100) = 0x000000A8;

   do {
      ;	
   } while (*(volatile ULONG *) (pAST->MMIOVirtualAddr + 0x10100) != 0x000000A8);
   	   
   /* Get BusWidth */
   ulData = *(ULONG * ) (pAST->MMIOVirtualAddr + 0x10004);       
   if (ulData & 0x40)
      ulDRAMBusWidth = 16;
   else    
      ulDRAMBusWidth = 32;
          
   /* Get MCLK */
   {   	
       ulData = *(ULONG *) (pAST->MMIOVirtualAddr + 0x10120);       
       ulData2 = *(ULONG *) (pAST->MMIOVirtualAddr + 0x10170);       
       if (ulData2 & 0x2000)
           ulRefPLL = 14318;
       else
           ulRefPLL = 12000;
           
       ulDeNumerator = ulData & 0x1F;
       ulNumerator = (ulData & 0x3FE0) >> 5;
               
       ulData = (ulData & 0xC000) >> 14;        
       switch (ulData)
       {
       case 0x03:
           ulDivider = 0x04;
           break;
       case 0x02:
       case 0x01:        
           ulDivider = 0x02;
           break;
       default:
           ulDivider = 0x01;                         
       }    
       
       ulMCLK = ulRefPLL * (ulNumerator + 2) / ((ulDeNumerator + 2) * ulDivider * 1000);          
       
   }  
   
   /* Get Bandwidth */
   /* Modify DARM utilization to 60% for AST1100/2100 16bits DRAM, ycchen@032508 */
   if ( ((pAST->jChipType == AST2100) || (pAST->jChipType == AST1100) || (pAST->jChipType == AST2200) || (pAST->jChipType == AST2150)) && (ulDRAMBusWidth == 16) )
       DRAMEfficiency = 600;     
   ulDRAMBandwidth = ulMCLK * ulDRAMBusWidth * 2 / 8;
   ActualDRAMBandwidth = ulDRAMBandwidth * DRAMEfficiency / 1000;
   
   /* Get Max DCLK */
   /* Fixed Fixed KVM + CRT threshold issue on AST2100 8bpp modes, ycchen@100708 */    
   GetIndexRegMask(CRTC_PORT, 0xD0, 0xFF, jReg);  
   if ((jReg & 0x08) && (pAST->jChipType == AST2000))                     
       ulDCLK = ActualDRAMBandwidth / ((pScrn->bitsPerPixel+1+16) / 8);	
   else if ((jReg & 0x08) && (pScrn->bitsPerPixel == 8))
       ulDCLK = ActualDRAMBandwidth / ((pScrn->bitsPerPixel+1+24) / 8);	       
   else    
       ulDCLK = ActualDRAMBandwidth / ((pScrn->bitsPerPixel+1) / 8);	   

   /* Add for AST2100, ycchen@061807 */
   if ((pAST->jChipType == AST2100) || (pAST->jChipType == AST2200))
   {
       if (ulDCLK > 200) ulDCLK = 200;
   }
   else
   {
       if (ulDCLK > 165) ulDCLK = 165;       
   }
    
   return(ulDCLK);
   
}

void
GetChipType(ScrnInfoPtr pScrn)
{
   ASTRecPtr pAST = ASTPTR(pScrn);
   ULONG ulData;
   UCHAR jReg;
   
   pAST->jChipType = AST2100;

   *(ULONG *) (pAST->MMIOVirtualAddr + 0xF004) = 0x1e6e0000;
   *(ULONG *) (pAST->MMIOVirtualAddr + 0xF000) = 0x1;        

   ulData = *(ULONG *) (pAST->MMIOVirtualAddr + 0x1207c);       
   
#if 0   
   if ((ulData & 0x0300) == 0x0200)
       pAST->jChipType = AST1100;   
#endif       
   switch (ulData & 0x0300)
   {
   case 0x0200:
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "AST1100 Detected.\n");
       pAST->jChipType = AST1100;   
       break;
   case 0x0100:
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "AST2200 Detected.\n");   
       pAST->jChipType = AST2200;   
       break;
   case 0x0000:
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "AST2150 Detected.\n");   
       pAST->jChipType = AST2150;   
       break;
   default:           
       xf86DrvMsg(pScrn->scrnIndex, X_INFO, "AST2100 Detected.\n");   
       pAST->jChipType = AST2100;
   }	
       
   /* VGA2 Clone Support */    
   GetIndexRegMask(CRTC_PORT, 0x90, 0xFF, jReg);
   if (jReg & 0x10)
       pAST->VGA2Clone = TRUE;
          
}

void
vSetStartAddressCRT1(ASTRecPtr pAST, ULONG base)
{
    SetIndexReg(CRTC_PORT,0x0D, (UCHAR) (base & 0xFF));
    SetIndexReg(CRTC_PORT,0x0C, (UCHAR) ((base >> 8) & 0xFF));
    SetIndexReg(CRTC_PORT,0xAF, (UCHAR) ((base >> 16) & 0xFF));
    	
}

void
vAST1000DisplayOff(ASTRecPtr pAST)
{
    SetIndexRegMask(SEQ_PORT,0x01, 0xDF, 0x20);
	    
}


void
vAST1000DisplayOn(ASTRecPtr pAST)
{
	
    SetIndexRegMask(SEQ_PORT,0x01, 0xDF, 0x00);	
    
}	

void ASTBlankScreen(ScrnInfoPtr pScrn, Bool unblack)
{
   ASTRecPtr pAST;

   pAST = ASTPTR(pScrn);
	
    if (unblack)
        vAST1000DisplayOn(pAST);
    else
        vAST1000DisplayOff(pAST);	
} 

void
vASTLoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors,
               VisualPtr pVisual)
{
	
    ASTRecPtr  pAST = ASTPTR(pScrn);
    int     i, j, index;
    UCHAR DACIndex, DACR, DACG, DACB;
  
    switch (pScrn->bitsPerPixel) {
    case 15:
        for(i=0; i<numColors; i++) {
            index = indices[i];
	    for(j=0; j<8; j++) {
                DACIndex = (index * 8) + j;
                DACR = colors[index].red << (8- pScrn->rgbBits);
                DACG = colors[index].green << (8- pScrn->rgbBits);
                DACB = colors[index].blue << (8- pScrn->rgbBits);
                         
                VGA_LOAD_PALETTE_INDEX (DACIndex, DACR, DACG, DACB);                         
	    }
        }
        break;
        
    case 16:
        for(i=0; i<numColors; i++) {
            index = indices[i];
	    for(j=0; j<4; j++) {
                DACIndex = (index * 4) + j;
                DACR = colors[index/2].red << (8- pScrn->rgbBits);
                DACG = colors[index].green << (8- pScrn->rgbBits);
                DACB = colors[index/2].blue << (8- pScrn->rgbBits);
                         
                VGA_LOAD_PALETTE_INDEX (DACIndex, DACR, DACG, DACB);                         
	    }
        }
        break;
        
    case 24:
        for(i=0; i<numColors; i++) {
            index = indices[i];
            DACIndex = index;
            DACR = colors[index].red;
            DACG = colors[index].green;
            DACB = colors[index].blue;
                         
            VGA_LOAD_PALETTE_INDEX (DACIndex, DACR, DACG, DACB);                         
        }    
        break;
        
    default:
        for(i=0; i<numColors; i++) {
            index = indices[i];
            DACIndex = index;
            DACR = colors[index].red >> (8 - pScrn->rgbBits);
            DACG = colors[index].green >> (8 - pScrn->rgbBits);
            DACB = colors[index].blue >> (8 - pScrn->rgbBits);
                         
            VGA_LOAD_PALETTE_INDEX (DACIndex, DACR, DACG, DACB);                         
        }    

    } /* end of switch */
    
} /* end of vASTLoadPalette */

void
ASTDisplayPowerManagementSet(ScrnInfoPtr pScrn, int PowerManagementMode, int flags)
{
   ASTRecPtr pAST;
   UCHAR SEQ01, CRB6;

   pAST = ASTPTR(pScrn);
   SEQ01=CRB6=0;
   
   vASTOpenKey(pScrn);
   
   switch (PowerManagementMode) {
   case DPMSModeOn:
      /* Screen: On; HSync: On, VSync: On */
      SEQ01 = 0x00;
      CRB6 = 0x00;
      break;
   case DPMSModeStandby:
      /* Screen: Off; HSync: Off, VSync: On */
      SEQ01 = 0x20;
      CRB6  = 0x01;     
      break;
   case DPMSModeSuspend:
      /* Screen: Off; HSync: On, VSync: Off */
      SEQ01 = 0x20;
      CRB6  = 0x02;           
      break;
   case DPMSModeOff:
      /* Screen: Off; HSync: Off, VSync: Off */
      SEQ01 = 0x20;
      CRB6  = 0x03;           
      break;
   }

   SetIndexRegMask(SEQ_PORT,0x01, 0xDF, SEQ01);
   SetIndexRegMask(CRTC_PORT,0xB6, 0xFC, CRB6);


}

#define I2C_BASE	0x1e780000
#define I2C_OFFSET	(0xA000 + 0x40 * 4)	/* port4 */
#define I2C_DEVICEADDR	0x0A0			/* slave addr */

Bool
GetVGA2EDID(ScrnInfoPtr pScrn, unsigned char *pEDIDBuffer)
{
    ASTRecPtr pAST = ASTPTR(pScrn);
    UCHAR *ulI2CBase = pAST->MMIOVirtualAddr + 0x10000 + I2C_OFFSET;
    ULONG i, ulData;    
    UCHAR *pjEDID;
    
    pjEDID = pEDIDBuffer;

    /* SCU settings */
    *(ULONG *) (pAST->MMIOVirtualAddr + 0xF004) = 0x1e6e0000;
    *(ULONG *) (pAST->MMIOVirtualAddr + 0xF000) = 0x1;
    xf86UDelay(10000);

    *(ULONG *) (pAST->MMIOVirtualAddr + 0x12000) = 0x1688A8A8;    
    ulData = *(ULONG *) (pAST->MMIOVirtualAddr + 0x12004);
    ulData &= 0xfffffffb;
    *(ULONG *) (pAST->MMIOVirtualAddr + 0x12004) = ulData;    
    xf86UDelay(10000);
    
    /* I2C settings */
    *(ULONG *) (pAST->MMIOVirtualAddr + 0xF004) = I2C_BASE;
    *(ULONG *) (pAST->MMIOVirtualAddr + 0xF000) = 0x1;
    xf86UDelay(10000);
    
    /* I2C Start */
    *(ULONG *) (ulI2CBase + 0x00) = 0x0;
    *(ULONG *) (ulI2CBase + 0x04) = 0x77777355;
    *(ULONG *) (ulI2CBase + 0x08) = 0x0;
    *(ULONG *) (ulI2CBase + 0x10) = 0xffffffff;
    *(ULONG *) (ulI2CBase + 0x00) = 0x1;
    *(ULONG *) (ulI2CBase + 0x0C) = 0xAF;
    *(ULONG *) (ulI2CBase + 0x20) = I2C_DEVICEADDR;
    *(ULONG *) (ulI2CBase + 0x14) = 0x03;
    do {
        ulData = *(volatile ULONG *) (ulI2CBase + 0x10);
    } while (!(ulData & 0x03));
    if (ulData & 0x02)				/* NACK */
        return (FALSE);
    *(ULONG *) (ulI2CBase + 0x10) = 0xffffffff;
    *(ULONG *) (ulI2CBase + 0x20) = (ULONG) 0;	/* Offset */
    *(ULONG *) (ulI2CBase + 0x14) = 0x02;
    do {
        ulData = *(volatile ULONG *) (ulI2CBase + 0x10);
    } while (!(ulData & 0x01));
    *(ULONG *) (ulI2CBase + 0x10) = 0xffffffff;
    *(ULONG *) (ulI2CBase + 0x20) = I2C_DEVICEADDR + 1;
    *(ULONG *) (ulI2CBase + 0x14) = 0x03; 
    do {
        ulData = *(volatile ULONG *) (ulI2CBase + 0x10);
    } while (!(ulData & 0x01));
    
    /* I2C Read */
    for (i=0; i<127; i++)
    {
        *(ULONG *) (ulI2CBase + 0x10) = 0xffffffff;
        *(ULONG *) (ulI2CBase + 0x0C) |= 0x10;
        *(ULONG *) (ulI2CBase + 0x14) = 0x08;
        do {
            ulData = *(volatile ULONG *) (ulI2CBase + 0x10);
        } while (!(ulData & 0x04));
        *(ULONG *) (ulI2CBase + 0x10) = 0xffffffff;
        *(UCHAR *) (pjEDID++) = (UCHAR) ((*(ULONG *) (ulI2CBase + 0x20) & 0xFF00) >> 8);        	
    }

    /* Read Last Byte */
    *(ULONG *) (ulI2CBase + 0x10) = 0xffffffff;
    *(ULONG *) (ulI2CBase + 0x0C) |= 0x10;
    *(ULONG *) (ulI2CBase + 0x14) = 0x18;
    do {
        ulData = *(volatile ULONG *) (ulI2CBase + 0x10);
    } while (!(ulData & 0x04));
    *(ULONG *) (ulI2CBase + 0x10) = 0xffffffff;
    *(UCHAR *) (pjEDID++) = (UCHAR) ((*(ULONG *) (ulI2CBase + 0x20) & 0xFF00) >> 8);        	

    /* I2C Stop	 */
    *(ULONG *) (ulI2CBase + 0x10) = 0xffffffff;
    *(ULONG *) (ulI2CBase + 0x14) = 0x20;
    do {
        ulData = *(volatile ULONG *) (ulI2CBase + 0x10);
    } while (!(ulData & 0x10));
    *(ULONG *) (ulI2CBase + 0x0C) &= 0xffffffef;        
    *(ULONG *) (ulI2CBase + 0x10) = 0xffffffff;
    
    return (TRUE);

} /* GetVGA2EDID */

/* Init VGA */
Bool bIsVGAEnabled(ScrnInfoPtr pScrn)
{
    ASTRecPtr pAST;
    UCHAR ch;
   
    pAST = ASTPTR(pScrn);

    ch = GetReg(pAST->RelocateIO+0x43);

    return (ch & 0x01);	
}	

void vEnableVGA(ScrnInfoPtr pScrn)
{
    ASTRecPtr pAST;
   
    pAST = ASTPTR(pScrn);

    SetReg(pAST->RelocateIO+0x43, 0x01);
    SetReg(pAST->RelocateIO+0x42, 0x01);   
	
}	

UCHAR ExtRegInfo[] = {
    0x0F,
    0x07,
    0x1C,
    0xFF
};

void vSetDefExtReg(ScrnInfoPtr pScrn)
{
    ASTRecPtr pAST;
    UCHAR i, jIndex, *pjExtRegInfo;
   
    pAST = ASTPTR(pScrn);

    /* Reset Scratch */
    for (i=0x81; i<=0x8F; i++)
    {
        SetIndexReg(CRTC_PORT, i, 0x00);
    }

    /* Set Ext. Reg */
    pjExtRegInfo = ExtRegInfo;    
    jIndex = 0xA0;
    while (*(UCHAR *) (pjExtRegInfo) != 0xFF)
    {
        SetIndexRegMask(CRTC_PORT,jIndex, 0x00, *(UCHAR *) (pjExtRegInfo));
        jIndex++;
        pjExtRegInfo++;
    }

    /* Set Ext. Default */
    SetIndexRegMask(CRTC_PORT,0x8C, 0x00, 0x01);    	
    SetIndexRegMask(CRTC_PORT,0xB7, 0x00, 0x00);    	
    
    /* Enable RAMDAC for A1, ycchen@113005 */
    SetIndexRegMask(CRTC_PORT,0xB6, 0xFF, 0x04);    	
      	
}	

typedef struct _AST_DRAMStruct {
	
    USHORT 	Index;
    ULONG	Data;
    
} AST_DRAMStruct, *PAST_DRAMStruct; 

AST_DRAMStruct AST2000DRAMTableData[] = {
    { 0x0108, 0x00000000 },
    { 0x0120, 0x00004a21 },
    { 0xFF00, 0x00000043 },   	
    { 0x0000, 0xFFFFFFFF },
    { 0x0004, 0x00000089 },
    { 0x0008, 0x22331353 },
    { 0x000C, 0x0d07000b },
    { 0x0010, 0x11113333 },
    { 0x0020, 0x00110350 },   
    { 0x0028, 0x1e0828f0 },
    { 0x0024, 0x00000001 },
    { 0x001C, 0x00000000 },
    { 0x0014, 0x00000003 },
    { 0xFF00, 0x00000043 },
    { 0x0018, 0x00000131 },
    { 0x0014, 0x00000001 },
    { 0xFF00, 0x00000043 },
    { 0x0018, 0x00000031 },
    { 0x0014, 0x00000001 },
    { 0xFF00, 0x00000043 },
    { 0x0028, 0x1e0828f1 },
    { 0x0024, 0x00000003 },
    { 0x002C, 0x1f0f28fb },
    { 0x0030, 0xFFFFFE01 },
    { 0xFFFF, 0xFFFFFFFF }
};

AST_DRAMStruct AST1100DRAMTableData[] = {	
    { 0x2000, 0x1688a8a8 },
    { 0x2020, 0x000041f0 },
    { 0xFF00, 0x00000043 },
    { 0x0000, 0xfc600309 },
    { 0x006C, 0x00909090 },
    { 0x0064, 0x00050000 },
    { 0x0004, 0x00000585 },
    { 0x0008, 0x0011030f },
    { 0x0010, 0x22201724 },
    { 0x0018, 0x1e29011a },
    { 0x0020, 0x00c82222 },
    { 0x0014, 0x01001523 },
    { 0x001C, 0x1024010d },
    { 0x0024, 0x00cb2522 },
    { 0x0038, 0xffffff82 },
    { 0x003C, 0x00000000 },
    { 0x0040, 0x00000000 },
    { 0x0044, 0x00000000 },
    { 0x0048, 0x00000000 },
    { 0x004C, 0x00000000 },
    { 0x0050, 0x00000000 },
    { 0x0054, 0x00000000 },
    { 0x0058, 0x00000000 },
    { 0x005C, 0x00000000 },
    { 0x0060, 0x032aa02a },
    { 0x0064, 0x002d3000 },
    { 0x0068, 0x00000000 },
    { 0x0070, 0x00000000 },
    { 0x0074, 0x00000000 },
    { 0x0078, 0x00000000 },
    { 0x007C, 0x00000000 },
    { 0x0034, 0x00000001 },
    { 0xFF00, 0x00000043 },
    { 0x002C, 0x00000732 },
    { 0x0030, 0x00000040 },
    { 0x0028, 0x00000005 },
    { 0x0028, 0x00000007 },
    { 0x0028, 0x00000003 },
    { 0x0028, 0x00000001 },
    { 0x000C, 0x00005a08 },
    { 0x002C, 0x00000632 },
    { 0x0028, 0x00000001 },
    { 0x0030, 0x000003c0 },
    { 0x0028, 0x00000003 },
    { 0x0030, 0x00000040 },
    { 0x0028, 0x00000003 },
    { 0x000C, 0x00005a21 },
    { 0x0034, 0x00007c03 },
    { 0x0120, 0x00004c41 },
    { 0xffff, 0xffffffff },
};

AST_DRAMStruct AST2100DRAMTableData[] = {	
    { 0x2000, 0x1688a8a8 },
    { 0x2020, 0x00004120 },
    { 0xFF00, 0x00000043 },
    { 0x0000, 0xfc600309 },
    { 0x006C, 0x00909090 },
    { 0x0064, 0x00070000 },
    { 0x0004, 0x00000489 },
    { 0x0008, 0x0011030f },
    { 0x0010, 0x32302926 },
    { 0x0018, 0x274c0122 },
    { 0x0020, 0x00ce2222 },
    { 0x0014, 0x01001523 },
    { 0x001C, 0x1024010d },
    { 0x0024, 0x00cb2522 },
    { 0x0038, 0xffffff82 },
    { 0x003C, 0x00000000 },
    { 0x0040, 0x00000000 },
    { 0x0044, 0x00000000 },
    { 0x0048, 0x00000000 },
    { 0x004C, 0x00000000 },
    { 0x0050, 0x00000000 },
    { 0x0054, 0x00000000 },
    { 0x0058, 0x00000000 },
    { 0x005C, 0x00000000 },
    { 0x0060, 0x0f2aa02a },
    { 0x0064, 0x003f3005 },
    { 0x0068, 0x02020202 },
    { 0x0070, 0x00000000 },
    { 0x0074, 0x00000000 },
    { 0x0078, 0x00000000 },
    { 0x007C, 0x00000000 },
    { 0x0034, 0x00000001 },
    { 0xFF00, 0x00000043 },
    { 0x002C, 0x00000942 },
    { 0x0030, 0x00000040 },
    { 0x0028, 0x00000005 },
    { 0x0028, 0x00000007 },
    { 0x0028, 0x00000003 },
    { 0x0028, 0x00000001 },
    { 0x000C, 0x00005a08 },
    { 0x002C, 0x00000842 },
    { 0x0028, 0x00000001 },
    { 0x0030, 0x000003c0 },
    { 0x0028, 0x00000003 },
    { 0x0030, 0x00000040 },
    { 0x0028, 0x00000003 },
    { 0x000C, 0x00005a21 },
    { 0x0034, 0x00007c03 },
    { 0x0120, 0x00005061 },
    { 0xffff, 0xffffffff },
};

void vInitDRAMReg(ScrnInfoPtr pScrn)
{
    AST_DRAMStruct *pjDRAMRegInfo;
    ASTRecPtr pAST = ASTPTR(pScrn);
    ULONG i, ulTemp, ulData;
    UCHAR jReg;

    GetIndexRegMask(CRTC_PORT, 0xD0, 0xFF, jReg);  
    
    if ((jReg & 0x80) == 0)			/* VGA only */
    {
    	if (pAST->jChipType == AST2000)
    	{
            pjDRAMRegInfo = AST2000DRAMTableData;
            
            *(ULONG *) (pAST->MMIOVirtualAddr + 0xF004) = 0x1e6e0000;
            *(ULONG *) (pAST->MMIOVirtualAddr + 0xF000) = 0x1;        
            *(ULONG *) (pAST->MMIOVirtualAddr + 0x10100) = 0xa8;

            do {
               ; 	
            } while (*(volatile ULONG *) (pAST->MMIOVirtualAddr + 0x10100) != 0xa8);
                
        }
    	else	/* AST2100/1100 */	
    	{           
    	    if ((pAST->jChipType == AST2100) || (pAST->jChipType == AST2200))
                pjDRAMRegInfo = AST2100DRAMTableData;
    	    else
                pjDRAMRegInfo = AST1100DRAMTableData;
    		
            *(ULONG *) (pAST->MMIOVirtualAddr + 0xF004) = 0x1e6e0000;
            *(ULONG *) (pAST->MMIOVirtualAddr + 0xF000) = 0x1;
            
            *(ULONG *) (pAST->MMIOVirtualAddr + 0x12000) = 0x1688A8A8;
            do {
               ; 	
            } while (*(volatile ULONG *) (pAST->MMIOVirtualAddr + 0x12000) != 0x01);
            
            *(ULONG *) (pAST->MMIOVirtualAddr + 0x10000) = 0xFC600309;
            do {
               ; 	
            } while (*(volatile ULONG *) (pAST->MMIOVirtualAddr + 0x10000) != 0x01);

        }
	
        while (pjDRAMRegInfo->Index != 0xFFFF)
        {
            if (pjDRAMRegInfo->Index == 0xFF00)			/* Delay function */
            {
            	for (i=0; i<15; i++)				
        	    xf86UDelay(pjDRAMRegInfo->Data);
            }
            else if ( (pjDRAMRegInfo->Index == 0x0004) && (pAST->jChipType != AST2000) )
            {
            	ulData = pjDRAMRegInfo->Data;
            	
            	if (pAST->jDRAMType == DRAMTYPE_1Gx16)
            	    ulData = 0x00000d89;
            	else if (pAST->jDRAMType == DRAMTYPE_1Gx32)
            	    ulData = 0x00000c8d;
            	        
                ulTemp = *(ULONG *) (pAST->MMIOVirtualAddr + 0x12070);
                ulTemp &= 0x0000000C;
                ulTemp <<= 2;                
                *(ULONG *) (pAST->MMIOVirtualAddr + 0x10000 + pjDRAMRegInfo->Index) = (ulData | ulTemp);               
            }	                
            else
            {	           	           
                *(ULONG *) (pAST->MMIOVirtualAddr + 0x10000 + pjDRAMRegInfo->Index) = pjDRAMRegInfo->Data;
            }
            pjDRAMRegInfo++;            
        }

        switch (pAST->jChipType)
        {
        case AST2000:
            *(ULONG *) (pAST->MMIOVirtualAddr + 0x10140) |= 0x40;
            break;
            
        case AST1100:
        case AST2100:
        case AST2200:
        case AST2150:        
            ulTemp = *(ULONG *) (pAST->MMIOVirtualAddr + 0x1200c);
            *(ULONG *) (pAST->MMIOVirtualAddr + 0x1200c) = (ulTemp & 0xFFFFFFFD);

            *(ULONG *) (pAST->MMIOVirtualAddr + 0x12040) |= 0x40;
            break;
        }
	            	
    } /* Init DRAM */
    
    /* wait ready */
    do {
        GetIndexRegMask(CRTC_PORT, 0xD0, 0xFF, jReg);      	
    } while ((jReg & 0x40) == 0);
       
} /* vInitDRAMReg */

Bool InitVGA(ScrnInfoPtr pScrn)
{
   ASTRecPtr pAST;
   ULONG ulData;

   pAST = ASTPTR(pScrn);

   {
       /* Enable PCI */
       PCI_READ_LONG(pAST->PciInfo, &ulData, 0x04);
       ulData |= 0x03;
       PCI_WRITE_LONG(pAST->PciInfo, ulData, 0x04);       

       /* Enable VGA */
       vEnableVGA(pScrn);
       
       vASTOpenKey(pScrn);
       vSetDefExtReg(pScrn);
       
       vInitDRAMReg(pScrn);      
             
   }

   return (TRUE);	
} /* Init VGA */
