/* $XFree86$ */
/* $XdotOrg: driver/xf86-video-sisusb/src/sisusb_init.c,v 1.8 2005/07/11 02:30:00 ajax Exp $ */
/*
 * Mode initializing code (CRT1 section) for SiS315/USB
 * (Universal module for Linux kernel framebuffer and X.org/XFree86 4.x)
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria
 *
 * * Redistribution and use in source and binary forms, with or without
 * * modification, are permitted provided that the following conditions
 * * are met:
 * * 1) Redistributions of source code must retain the above copyright
 * *    notice, this list of conditions and the following disclaimer.
 * * 2) Redistributions in binary form must reproduce the above copyright
 * *    notice, this list of conditions and the following disclaimer in the
 * *    documentation and/or other materials provided with the distribution.
 * * 3) The name of the author may not be used to endorse or promote products
 * *    derived from this software without specific prior written permission.
 * *
 * * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: 	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "sisusb_init.h"

#if defined(ALLOC_PRAGMA)
#pragma alloc_text(PAGE,SiSUSBSetMode)
#endif

/*********************************************/
/*         POINTER INITIALIZATION            */
/*********************************************/

static void
SiSUSB_InitPtr(SiS_Private *SiS_Pr)
{
   SiS_Pr->SiS_ModeResInfo   = SiS_ModeResInfo;
   SiS_Pr->SiS_StandTable    = SiS_StandTable;
   SiS_Pr->pSiS_SoftSetting  = &SiS_SoftSetting;

   SiS_Pr->SiS_EModeIDTable  = SiSUSB_EModeIDTable;
   SiS_Pr->SiS_RefIndex      = SiSUSB_RefIndex;
   SiS_Pr->SiS_CRT1Table     = SiSUSB_CRT1Table;

   SiS_Pr->SiS_VCLKData      = SiSUSB_VCLKData;
}

/*********************************************/
/*            HELPER: Get ModeID             */
/*********************************************/

USHORT
SiSUSB_GetModeID(int VGAEngine, unsigned int VBFlags, int HDisplay, int VDisplay,
              int Depth, BOOLEAN FSTN, int LCDwidth, int LCDheight)
{
   USHORT ModeIndex = 0;

   switch(HDisplay)
   {
     case 320:
     	  if(VDisplay == 200)      ModeIndex = ModeIndex_320x200[Depth];
	  else if(VDisplay == 240) ModeIndex = ModeIndex_320x240[Depth];
          break;
     case 400:
          if(VDisplay == 300) ModeIndex = ModeIndex_400x300[Depth];
          break;
     case 512:
          if(VDisplay == 384) ModeIndex = ModeIndex_512x384[Depth];
          break;
     case 640:
          if(VDisplay == 480)      ModeIndex = ModeIndex_640x480[Depth];
	  else if(VDisplay == 400) ModeIndex = ModeIndex_640x400[Depth];
          break;
     case 720:
          if(VDisplay == 480)      ModeIndex = ModeIndex_720x480[Depth];
          else if(VDisplay == 576) ModeIndex = ModeIndex_720x576[Depth];
          break;
     case 768:
          if(VDisplay == 576) ModeIndex = ModeIndex_768x576[Depth];
	  break;
     case 800:
	  if(VDisplay == 600)      ModeIndex = ModeIndex_800x600[Depth];
	  else if(VDisplay == 480) ModeIndex = ModeIndex_800x480[Depth];
          break;
     case 848:
	  if(VDisplay == 480) ModeIndex = ModeIndex_848x480[Depth];
	  break;
     case 856:
	  if(VDisplay == 480) ModeIndex = ModeIndex_856x480[Depth];
	  break;
     case 960:
	  if(VDisplay == 540)      ModeIndex = ModeIndex_960x540[Depth];
	  else if(VDisplay == 600) ModeIndex = ModeIndex_960x600[Depth];
	  break;
     case 1024:
          if(VDisplay == 576)      ModeIndex = ModeIndex_1024x576[Depth];
          else if(VDisplay == 768) ModeIndex = ModeIndex_1024x768[Depth];
          break;
     case 1152:
	  if(VDisplay == 864)      ModeIndex = ModeIndex_1152x864[Depth];
	  break;
     case 1280:
          switch(VDisplay) {
	  case 720:
	     ModeIndex = ModeIndex_1280x720[Depth];
	     break;
	  case 768:
	     ModeIndex = ModeIndex_1280x768[Depth];
	     break;
	  case 1024:
	     ModeIndex = ModeIndex_1280x1024[Depth];
	     break;
	  }
   }

   return(ModeIndex);
}

/*********************************************/
/*          HELPER: SetReg, GetReg           */
/*********************************************/

static void
SiS_SetReg(SiS_Private *SiS_Pr, SISIOADDRESS port, USHORT index, USHORT data)
{
   outSISIDXREG((SISUSBPtr)SiS_Pr->pSiSUSB, port, index, data);
}

static void
SiS_SetRegByte(SiS_Private *SiS_Pr, SISIOADDRESS port, USHORT data)
{
   outSISREG((SISUSBPtr)SiS_Pr->pSiSUSB, port, data);
}

static UCHAR
SiS_GetReg(SiS_Private *SiS_Pr, SISIOADDRESS port, USHORT index)
{
   return(__inSISIDXREG((SISUSBPtr)SiS_Pr->pSiSUSB, port, index));
}

static UCHAR
SiS_GetRegByte(SiS_Private *SiS_Pr, SISIOADDRESS port)
{
   return(inSISREG((SISUSBPtr)SiS_Pr->pSiSUSB, port));
}

static void
SiS_SetRegANDOR(SiS_Private *SiS_Pr, SISIOADDRESS port, USHORT index,
                USHORT DataAND, USHORT DataOR)
{
  setSISIDXREG((SISUSBPtr)SiS_Pr->pSiSUSB, port, index, DataAND, DataOR);
}

static void
SiS_SetRegAND(SiS_Private *SiS_Pr, SISIOADDRESS port, USHORT index, USHORT DataAND)
{
  andSISIDXREG((SISUSBPtr)SiS_Pr->pSiSUSB, port, index, DataAND);
}

static void
SiS_SetRegOR(SiS_Private *SiS_Pr,SISIOADDRESS port, USHORT index, USHORT DataOR)
{
  orSISIDXREG((SISUSBPtr)SiS_Pr->pSiSUSB, port, index, DataOR);
}

/*********************************************/
/*      HELPER: DisplayOn, DisplayOff        */
/*********************************************/

static void
SiS_DisplayOn(SiS_Private *SiS_Pr)
{
   SiS_SetRegAND(SiS_Pr,SiS_Pr->SiS_P3c4,0x01,0xDF);
}

/*********************************************/
/*        HELPER: Init Port Addresses        */
/*********************************************/

void
SiSUSBRegInit(SiS_Private *SiS_Pr, SISIOADDRESS BaseAddr)
{
   SiS_Pr->SiS_P3c4 = BaseAddr + 0x14;
   SiS_Pr->SiS_P3d4 = BaseAddr + 0x24;
   SiS_Pr->SiS_P3c0 = BaseAddr + 0x10;
   SiS_Pr->SiS_P3ce = BaseAddr + 0x1e;
   SiS_Pr->SiS_P3c2 = BaseAddr + 0x12;
   SiS_Pr->SiS_P3ca = BaseAddr + 0x1a;
   SiS_Pr->SiS_P3c6 = BaseAddr + 0x16;
   SiS_Pr->SiS_P3c7 = BaseAddr + 0x17;
   SiS_Pr->SiS_P3c8 = BaseAddr + 0x18;
   SiS_Pr->SiS_P3c9 = BaseAddr + 0x19;
   SiS_Pr->SiS_P3cb = BaseAddr + 0x1b;
   SiS_Pr->SiS_P3cc = BaseAddr + 0x1c;
   SiS_Pr->SiS_P3cd = BaseAddr + 0x1d;
   SiS_Pr->SiS_P3da = BaseAddr + 0x2a;
   SiS_Pr->SiS_Part1Port = BaseAddr + SIS_CRT2_PORT_04;     /* Digital video interface registers (LCD) */
}

/*********************************************/
/*             HELPER: GetSysFlags           */
/*********************************************/

static void
SiS_GetSysFlags(SiS_Private *SiS_Pr)
{
   SiS_Pr->SiS_MyCR63 = 0x63;
}

/*********************************************/
/*         HELPER: Init PCI & Engines        */
/*********************************************/

static void
SiSInitPCIetc(SiS_Private *SiS_Pr)
{
   SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x20,0xa1);
   /*  - Enable 2D (0x40)
    *  - Enable 3D (0x02)
    *  - Enable 3D vertex command fetch (0x10)
    *  - Enable 3D command parser (0x08)
    *  - Enable 3D G/L transformation engine (0x80)
    */
   SiS_SetRegOR(SiS_Pr,SiS_Pr->SiS_P3c4,0x1E,0xDA);
}

/*********************************************/
/*        HELPER: SET SEGMENT REGISTERS      */
/*********************************************/

static void
SiS_SetSegRegLower(SiS_Private *SiS_Pr, USHORT value)
{
   USHORT temp;

   value &= 0x00ff;
   temp = SiS_GetRegByte(SiS_Pr,SiS_Pr->SiS_P3cb) & 0xf0;
   temp |= (value >> 4);
   SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3cb, temp);
   temp = SiS_GetRegByte(SiS_Pr,SiS_Pr->SiS_P3cd) & 0xf0;
   temp |= (value & 0x0f);
   SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3cd, temp);
}

static void
SiS_SetSegRegUpper(SiS_Private *SiS_Pr, USHORT value)
{
   USHORT temp;

   value &= 0x00ff;
   temp = SiS_GetRegByte(SiS_Pr,SiS_Pr->SiS_P3cb) & 0x0f;
   temp |= (value & 0xf0);
   SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3cb, temp);
   temp = SiS_GetRegByte(SiS_Pr,SiS_Pr->SiS_P3cd) & 0x0f;
   temp |= (value << 4);
   SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3cd, temp);
}

static void
SiS_SetSegmentReg(SiS_Private *SiS_Pr, USHORT value)
{
   SiS_SetSegRegLower(SiS_Pr, value);
   SiS_SetSegRegUpper(SiS_Pr, value);
}

static void
SiS_ResetSegmentReg(SiS_Private *SiS_Pr)
{
   SiS_SetSegmentReg(SiS_Pr, 0);
}

static void
SiS_SetSegmentRegOver(SiS_Private *SiS_Pr, USHORT value)
{
   USHORT temp = value >> 8;

   temp &= 0x07;
   temp |= (temp << 4);
   SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x1d,temp);
   SiS_SetSegmentReg(SiS_Pr, value);
}

static void
SiS_ResetSegmentRegOver(SiS_Private *SiS_Pr)
{
   SiS_SetSegmentRegOver(SiS_Pr, 0);
}

static void
SiS_ResetSegmentRegisters(SiS_Private *SiS_Pr)
{
   SiS_ResetSegmentReg(SiS_Pr);
   SiS_ResetSegmentRegOver(SiS_Pr);
}

/*********************************************/
/*           HELPER: SearchModeID            */
/*********************************************/

static BOOLEAN
SiS_SearchModeID(SiS_Private *SiS_Pr, USHORT *ModeNo, USHORT *ModeIdIndex)
{
   if((*ModeNo) <= 0x13) {

      return FALSE;

   } else {

      for(*ModeIdIndex = 0; ;(*ModeIdIndex)++) {
         if(SiS_Pr->SiS_EModeIDTable[*ModeIdIndex].Ext_ModeID == (*ModeNo)) break;
         if(SiS_Pr->SiS_EModeIDTable[*ModeIdIndex].Ext_ModeID == 0xFF)      return FALSE;
      }

   }
   return TRUE;
}

/*********************************************/
/*            HELPER: ENABLE CRT1            */
/*********************************************/

static void
SiS_HandleCRT1(SiS_Private *SiS_Pr)
{
  /* Enable CRT1 gating */
  SiS_SetRegAND(SiS_Pr, SiS_Pr->SiS_P3d4,SiS_Pr->SiS_MyCR63,0xbf);
}

/*********************************************/
/*           HELPER: GetColorDepth           */
/*********************************************/

static USHORT
SiS_GetColorDepth(SiS_Private *SiS_Pr, USHORT ModeNo, USHORT ModeIdIndex)
{
  USHORT ColorDepth[6] = { 1, 2, 4, 4, 6, 8};
  SHORT  index;
  USHORT modeflag;

  /* Do NOT check UseCustomMode, will skrew up FIFO */
  if(ModeNo == 0xfe) {
     modeflag = SiS_Pr->CModeFlag;
  } else {
     modeflag = SiS_Pr->SiS_EModeIDTable[ModeIdIndex].Ext_ModeFlag;
  }

  index = (modeflag & ModeTypeMask) - ModeEGA;
  if(index < 0) index = 0;
  return ColorDepth[index];
}

/*********************************************/
/*             HELPER: GetOffset             */
/*********************************************/

static USHORT
SiS_GetOffset(SiS_Private *SiS_Pr, USHORT ModeNo, USHORT ModeIdIndex,
              USHORT RefreshRateTableIndex)
{
  USHORT xres, temp, colordepth, infoflag;

  if(SiS_Pr->UseCustomMode) {
     infoflag = SiS_Pr->CInfoFlag;
     xres = SiS_Pr->CHDisplay;
  } else {
     infoflag = SiS_Pr->SiS_RefIndex[RefreshRateTableIndex].Ext_InfoFlag;
     xres = SiS_Pr->SiS_RefIndex[RefreshRateTableIndex].XRes;
  }

  colordepth = SiS_GetColorDepth(SiS_Pr,ModeNo,ModeIdIndex);

  temp = xres / 16;
  if(infoflag & InterlaceMode) temp <<= 1;
  temp *= colordepth;
  if(xres % 16) {
     colordepth >>= 1;
     temp += colordepth;
  }

  return(temp);
}

/*********************************************/
/*                   SEQ                     */
/*********************************************/

static void
SiS_SetSeqRegs(SiS_Private *SiS_Pr, USHORT StandTableIndex)
{
   UCHAR SRdata;
   int   i;

   SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x00,0x03);

   /* "display off"  */
   SRdata = SiS_Pr->SiS_StandTable[StandTableIndex].SR[0] | 0x20;
   SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x01,SRdata);

   for(i = 2; i <= 4; i++) {
      SRdata = SiS_Pr->SiS_StandTable[StandTableIndex].SR[i-1];
      SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,i,SRdata);
   }
}

/*********************************************/
/*                  MISC                     */
/*********************************************/

static void
SiS_SetMiscRegs(SiS_Private *SiS_Pr, USHORT StandTableIndex)
{
   UCHAR Miscdata;

   Miscdata = SiS_Pr->SiS_StandTable[StandTableIndex].MISC;
   SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3c2,Miscdata);
}

/*********************************************/
/*                  CRTC                     */
/*********************************************/

static void
SiS_SetCRTCRegs(SiS_Private *SiS_Pr, USHORT StandTableIndex)
{
  UCHAR CRTCdata;
  USHORT i;

  SiS_SetRegAND(SiS_Pr, SiS_Pr->SiS_P3d4,0x11,0x7f);                       /* Unlock CRTC */

  for(i = 0; i <= 0x18; i++) {
     CRTCdata = SiS_Pr->SiS_StandTable[StandTableIndex].CRTC[i];
     SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,i,CRTCdata);                     /* Set CRTC(3d4) */
  }
}

/*********************************************/
/*                   ATT                     */
/*********************************************/

static void
SiS_SetATTRegs(SiS_Private *SiS_Pr, USHORT StandTableIndex)
{
   UCHAR ARdata;
   USHORT i;

   for(i = 0; i <= 0x13; i++) {
      ARdata = SiS_Pr->SiS_StandTable[StandTableIndex].ATTR[i];
      SiS_GetRegByte(SiS_Pr,SiS_Pr->SiS_P3da);                         /* reset 3da  */
      SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3c0,i);                       /* set index  */
      SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3c0,ARdata);                  /* set data   */
   }
   SiS_GetRegByte(SiS_Pr,SiS_Pr->SiS_P3da);                            /* reset 3da  */
   SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3c0,0x14);                       /* set index  */
   SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3c0,0x00);                       /* set data   */

   SiS_GetRegByte(SiS_Pr,SiS_Pr->SiS_P3da);
   SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3c0,0x20);			/* Enable Attribute  */
   SiS_GetRegByte(SiS_Pr,SiS_Pr->SiS_P3da);
}

/*********************************************/
/*                   GRC                     */
/*********************************************/

static void
SiS_SetGRCRegs(SiS_Private *SiS_Pr, USHORT StandTableIndex)
{
   UCHAR GRdata;
   USHORT i;

   for(i = 0; i <= 0x08; i++) {
      GRdata = SiS_Pr->SiS_StandTable[StandTableIndex].GRC[i];
      SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3ce,i,GRdata);
   }

   if(SiS_Pr->SiS_ModeType > ModeVGA) {
      /* 256 color disable */
      SiS_SetRegAND(SiS_Pr, SiS_Pr->SiS_P3ce,0x05,0xBF);
   }
}

/*********************************************/
/*          CLEAR EXTENDED REGISTERS         */
/*********************************************/

static void
SiS_ClearExt1Regs(SiS_Private *SiS_Pr, USHORT ModeNo)
{
  USHORT i;

  for(i = 0x0A; i <= 0x0E; i++) {
     SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,i,0x00);
  }

  SiS_SetRegAND(SiS_Pr, SiS_Pr->SiS_P3c4,0x37,0xFE);
}

/*********************************************/
/*              Get rate index               */
/*********************************************/

static USHORT
SiS_GetRatePtr(SiS_Private *SiS_Pr, USHORT ModeNo, USHORT ModeIdIndex)
{
  USHORT RRTI,i,index,temp;

  /* Do NOT check for UseCustomMode here, will skrew up FIFO */
  if(ModeNo == 0xfe) return 0;

  index = SiS_GetReg(SiS_Pr,SiS_Pr->SiS_P3d4,0x33) & 0x0F;
  if(index > 0) index--;

  RRTI = SiS_Pr->SiS_EModeIDTable[ModeIdIndex].REFindex;
  ModeNo = SiS_Pr->SiS_RefIndex[RRTI].ModeID;

  i = 0;
  do {
     if(SiS_Pr->SiS_RefIndex[RRTI + i].ModeID != ModeNo) break;
     temp = SiS_Pr->SiS_RefIndex[RRTI + i].Ext_InfoFlag;
     temp &= ModeTypeMask;
     if(temp < SiS_Pr->SiS_ModeType) break;
     i++;
     index--;
  } while(index != 0xFFFF);

  i--;

  return(RRTI + i);
}

/*********************************************/
/*                  SYNC                     */
/*********************************************/

static void
SiS_SetCRT1Sync(SiS_Private *SiS_Pr, USHORT RefreshRateTableIndex)
{
  USHORT sync;

  if(SiS_Pr->UseCustomMode) {
     sync = SiS_Pr->CInfoFlag >> 8;
  } else {
     sync = SiS_Pr->SiS_RefIndex[RefreshRateTableIndex].Ext_InfoFlag >> 8;
  }

  sync &= 0xC0;
  sync |= 0x2f;
  SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3c2,sync);
}

/*********************************************/
/*                  CRTC/2                   */
/*********************************************/

static void
SiS_SetCRT1CRTC(SiS_Private *SiS_Pr, USHORT ModeNo, USHORT ModeIdIndex,
                USHORT RefreshRateTableIndex)
{
  UCHAR  index;
  USHORT temp,i,j,modeflag;

  SiS_SetRegAND(SiS_Pr, SiS_Pr->SiS_P3d4,0x11,0x7f);		/* unlock cr0-7 */

  if(SiS_Pr->UseCustomMode) {

     modeflag = SiS_Pr->CModeFlag;

     for(i=0,j=0;i<=7;i++,j++) {
        SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,j,SiS_Pr->CCRT1CRTC[i]);
     }
     for(j=0x10;i<=10;i++,j++) {
        SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,j,SiS_Pr->CCRT1CRTC[i]);
     }
     for(j=0x15;i<=12;i++,j++) {
        SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,j,SiS_Pr->CCRT1CRTC[i]);
     }
     for(j=0x0A;i<=15;i++,j++) {
        SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,j,SiS_Pr->CCRT1CRTC[i]);
     }

     temp = SiS_Pr->CCRT1CRTC[16] & 0xE0;
     SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x0E,temp);

     temp = (SiS_Pr->CCRT1CRTC[16] & 0x01) << 5;
     if(modeflag & DoubleScanMode) temp |= 0x80;
     SiS_SetRegANDOR(SiS_Pr,SiS_Pr->SiS_P3d4,0x09,0x5F,temp);

  } else {

     modeflag = SiS_Pr->SiS_EModeIDTable[ModeIdIndex].Ext_ModeFlag;

     index = SiS_Pr->SiS_RefIndex[RefreshRateTableIndex].Ext_CRT1CRTC;

     for(i=0,j=0;i<=7;i++,j++) {
        SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,j,SiS_Pr->SiS_CRT1Table[index].CR[i]);
     }
     for(j=0x10;i<=10;i++,j++) {
        SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,j,SiS_Pr->SiS_CRT1Table[index].CR[i]);
     }
     for(j=0x15;i<=12;i++,j++) {
        SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,j,SiS_Pr->SiS_CRT1Table[index].CR[i]);
     }
     for(j=0x0A;i<=15;i++,j++) {
        SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,j,SiS_Pr->SiS_CRT1Table[index].CR[i]);
     }

     temp = SiS_Pr->SiS_CRT1Table[index].CR[16] & 0xE0;
     SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x0E,temp);

     temp = ((SiS_Pr->SiS_CRT1Table[index].CR[16]) & 0x01) << 5;
     if(modeflag & DoubleScanMode)  temp |= 0x80;
     SiS_SetRegANDOR(SiS_Pr,SiS_Pr->SiS_P3d4,0x09,0x5F,temp);

  }

  if(SiS_Pr->SiS_ModeType > ModeVGA) SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,0x14,0x4F);
}

/*********************************************/
/*               OFFSET & PITCH              */
/*********************************************/
/*  (partly overruled by SetPitch() in XF86) */
/*********************************************/

static void
SiS_SetCRT1Offset(SiS_Private *SiS_Pr, USHORT ModeNo, USHORT ModeIdIndex,
                  USHORT RefreshRateTableIndex)
{
   USHORT temp, DisplayUnit, infoflag;

   if(SiS_Pr->UseCustomMode) {
      infoflag = SiS_Pr->CInfoFlag;
   } else {
      infoflag = SiS_Pr->SiS_RefIndex[RefreshRateTableIndex].Ext_InfoFlag;
   }

   DisplayUnit = SiS_GetOffset(SiS_Pr,ModeNo,ModeIdIndex, RefreshRateTableIndex);

   temp = (DisplayUnit >> 8) & 0x0f;
   SiS_SetRegANDOR(SiS_Pr,SiS_Pr->SiS_P3c4,0x0E,0xF0,temp);

   temp = DisplayUnit & 0xFF;
   SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,0x13,temp);

   if(infoflag & InterlaceMode) DisplayUnit >>= 1;

   DisplayUnit <<= 5;
   temp = (DisplayUnit & 0xff00) >> 8;
   if(DisplayUnit & 0xff) temp++;
   temp++;
   SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x10,temp);
}

/*********************************************/
/*                  VCLK                     */
/*********************************************/

static void
SiS_SetCRT1VCLK(SiS_Private *SiS_Pr, USHORT ModeNo, USHORT ModeIdIndex,
                USHORT RefreshRateTableIndex)
{
  USHORT  index=0, clka, clkb;

  if(SiS_Pr->UseCustomMode) {
     clka = SiS_Pr->CSR2B;
     clkb = SiS_Pr->CSR2C;
  } else {
     index = SiS_Pr->SiS_RefIndex[RefreshRateTableIndex].Ext_CRTVCLK;
     clka = SiS_Pr->SiS_VCLKData[index].SR2B;
     clkb = SiS_Pr->SiS_VCLKData[index].SR2C;
  }

  SiS_SetRegAND(SiS_Pr, SiS_Pr->SiS_P3c4,0x31,0xCF);

  SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x2B,clka);
  SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x2C,clkb);
  SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x2D,0x01);

}

/*********************************************/
/*                  FIFO                     */
/*********************************************/

static void
SiS_SetCRT1FIFO_310(SiS_Private *SiS_Pr, USHORT ModeNo, USHORT ModeIdIndex)
{
  USHORT modeflag;

  /* disable auto-threshold */
  SiS_SetRegAND(SiS_Pr, SiS_Pr->SiS_P3c4,0x3D,0xFE);

  if(SiS_Pr->UseCustomMode) {
     modeflag = SiS_Pr->CModeFlag;
  } else {
     modeflag = SiS_Pr->SiS_EModeIDTable[ModeIdIndex].Ext_ModeFlag;
  }

  SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x08,0xAE);
  SiS_SetRegAND(SiS_Pr, SiS_Pr->SiS_P3c4,0x09,0xF0);

  if((!(modeflag & DoubleScanMode)) || (!(modeflag & HalfDCLK))) {
     SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x08,0x34);
     SiS_SetRegOR(SiS_Pr,SiS_Pr->SiS_P3c4,0x3D,0x01);
  }
}

/*********************************************/
/*              MODE REGISTERS               */
/*********************************************/

static void
SiS_SetVCLKState(SiS_Private *SiS_Pr, USHORT ModeNo, USHORT RefreshRateTableIndex,
                 USHORT ModeIdIndex)
{
  USHORT data=0, VCLK=0, index=0;

  if(SiS_Pr->UseCustomMode) {
     VCLK = SiS_Pr->CSRClock;
  } else {
     index = SiS_Pr->SiS_RefIndex[RefreshRateTableIndex].Ext_CRTVCLK;
     VCLK = SiS_Pr->SiS_VCLKData[index].CLOCK;
  }

  if(VCLK >= 166) data |= 0x0c;
  SiS_SetRegANDOR(SiS_Pr,SiS_Pr->SiS_P3c4,0x32,0xf3,data);

  if(VCLK >= 166) {
     SiS_SetRegAND(SiS_Pr, SiS_Pr->SiS_P3c4,0x1f,0xe7);
  }

  /* DAC speed */
  data = 0x03;
  if((VCLK >= 135) && (VCLK < 160))      data = 0x02;
  else if((VCLK >= 160) && (VCLK < 260)) data = 0x01;
  else if(VCLK >= 260)                   data = 0x00;
  SiS_SetRegANDOR(SiS_Pr,SiS_Pr->SiS_P3c4,0x07,0xF8,data);
}

static void
SiS_SetCRT1ModeRegs(SiS_Private *SiS_Pr, USHORT ModeNo, USHORT ModeIdIndex,
			USHORT RefreshRateTableIndex)
{
  USHORT data,infoflag=0,modeflag;
  USHORT resindex,xres;

  if(SiS_Pr->UseCustomMode) {
     modeflag = SiS_Pr->CModeFlag;
     infoflag = SiS_Pr->CInfoFlag;
     xres = SiS_Pr->CHDisplay;
  } else {
     resindex = SiS_Pr->SiS_EModeIDTable[ModeIdIndex].Ext_RESINFO;
     modeflag = SiS_Pr->SiS_EModeIDTable[ModeIdIndex].Ext_ModeFlag;
     infoflag = SiS_Pr->SiS_RefIndex[RefreshRateTableIndex].Ext_InfoFlag;
     xres = SiS_Pr->SiS_ModeResInfo[resindex].HTotal;
  }

  /* Disable DPMS */
  SiS_SetRegAND(SiS_Pr, SiS_Pr->SiS_P3c4,0x1F,0x3F);

  data = 0;
  if(SiS_Pr->SiS_ModeType > ModeEGA) {
     data |= 0x02;
     data |= ((SiS_Pr->SiS_ModeType - ModeVGA) << 2);
  }
  if(infoflag & InterlaceMode) data |= 0x20;
  SiS_SetRegANDOR(SiS_Pr,SiS_Pr->SiS_P3c4,0x06,0xC0,data);

  data = 0;
  if(infoflag & InterlaceMode) {
        /* data = (Hsync / 8) - ((Htotal / 8) / 2) + 3 */
        USHORT hrs = (SiS_GetReg(SiS_Pr,SiS_Pr->SiS_P3d4,0x04) |
	             ((SiS_GetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x0b) & 0xc0) << 2)) - 3;
	USHORT hto = (SiS_GetReg(SiS_Pr,SiS_Pr->SiS_P3d4,0x00) |
	             ((SiS_GetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x0b) & 0x03) << 8)) + 5;
	data = hrs - (hto >> 1) + 3;
#if 0
        if(xres <= 800)       data = 0x0020; /* guessed */
        else if(xres <= 1024) data = 0x0035; /* 1024x768 */
        else                  data = 0x0048; /* 1280x1024 */
#endif
  }
  SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,0x19,(data & 0xFF));
  SiS_SetRegANDOR(SiS_Pr,SiS_Pr->SiS_P3d4,0x1a,0xFC,(data >> 8));

  if(modeflag & HalfDCLK) {
     SiS_SetRegOR(SiS_Pr,SiS_Pr->SiS_P3c4,0x01,0x08);
  }

  data = 0;
  if(modeflag & LineCompareOff) data = 0x08;
  SiS_SetRegANDOR(SiS_Pr,SiS_Pr->SiS_P3c4,0x0F,0xB7,data);
  if(SiS_Pr->SiS_ModeType == ModeEGA) {
     SiS_SetRegOR(SiS_Pr,SiS_Pr->SiS_P3c4,0x0F,0x40);
  }

  SiS_SetRegAND(SiS_Pr, SiS_Pr->SiS_P3c4,0x31,0xfb);

  data = 0x60;
  if(SiS_Pr->SiS_ModeType != ModeText) {
     data ^= 0x60;
     if(SiS_Pr->SiS_ModeType != ModeEGA) {
        data ^= 0xA0;
     }
  }
  SiS_SetRegANDOR(SiS_Pr,SiS_Pr->SiS_P3c4,0x21,0x1F,data);

  SiS_SetVCLKState(SiS_Pr, ModeNo, RefreshRateTableIndex, ModeIdIndex);

  SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,0x52,0x2c);
}

/*********************************************/
/*                 LOAD DAC                  */
/*********************************************/

static void
SiS_WriteDAC(SiS_Private *SiS_Pr, SISIOADDRESS DACData, USHORT shiftflag,
             USHORT dl, USHORT ah, USHORT al, USHORT dh)
{
  USHORT temp,bh,bl;

  bh = ah;
  bl = al;
  if(dl != 0) {
     temp = bh;
     bh = dh;
     dh = temp;
     if(dl == 1) {
        temp = bl;
        bl = dh;
        dh = temp;
     } else {
        temp = bl;
        bl = bh;
        bh = temp;
     }
  }
  if(shiftflag) {
     dh <<= 2;
     bh <<= 2;
     bl <<= 2;
  }
  SiS_SetRegByte(SiS_Pr,DACData,(USHORT)dh);
  SiS_SetRegByte(SiS_Pr,DACData,(USHORT)bh);
  SiS_SetRegByte(SiS_Pr,DACData,(USHORT)bl);
}

static void
SiS_LoadDAC(SiS_Private *SiS_Pr, USHORT ModeNo, USHORT ModeIdIndex)
{
   USHORT data,data2;
   USHORT time,i,j,k,m,n,o;
   USHORT si,di,bx,dl,al,ah,dh;
   USHORT shiftflag;
   SISIOADDRESS DACAddr, DACData;
   const USHORT *table = NULL;

   if(SiS_Pr->UseCustomMode) {
      data = SiS_Pr->CModeFlag;
   } else {
      data = SiS_Pr->SiS_EModeIDTable[ModeIdIndex].Ext_ModeFlag;
   }

   data &= DACInfoFlag;

   time = 256;
   table = SiS_VGA_DAC;

   if(time == 256) j = 16;
   else            j = time;

   DACAddr = SiS_Pr->SiS_P3c8;
   DACData = SiS_Pr->SiS_P3c9;
   shiftflag = 0;
   SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3c6,0xFF);

   SiS_SetRegByte(SiS_Pr,DACAddr,0x00);

   for(i=0; i<j; i++) {
      data = table[i];
      for(k=0; k<3; k++) {
	data2 = 0;
	if(data & 0x01) data2 = 0x2A;
	if(data & 0x02) data2 += 0x15;
	if(shiftflag) data2 <<= 2;
	SiS_SetRegByte(SiS_Pr,DACData, data2);
	data >>= 2;
      }
   }

   if(time == 256) {
      for(i = 16; i < 32; i++) {
   	 data = table[i];
	 if(shiftflag) data <<= 2;
	 for(k = 0; k < 3; k++) SiS_SetRegByte(SiS_Pr,DACData, data);
      }
      si = 32;
      for(m = 0; m < 9; m++) {
         di = si;
         bx = si + 4;
         dl = 0;
         for(n = 0; n < 3; n++) {
  	    for(o = 0; o < 5; o++) {
	       dh = table[si];
	       ah = table[di];
	       al = table[bx];
	       si++;
	       SiS_WriteDAC(SiS_Pr, DACData, shiftflag, dl, ah, al, dh);
	    }
	    si -= 2;
	    for(o = 0; o < 3; o++) {
	       dh = table[bx];
	       ah = table[di];
	       al = table[si];
	       si--;
	       SiS_WriteDAC(SiS_Pr, DACData, shiftflag, dl, ah, al, dh);
	    }
	    dl++;
	 }            /* for n < 3 */
	 si += 5;
      }               /* for m < 9 */
   }
}

/*********************************************/
/*         SET CRT1 REGISTER GROUP           */
/*********************************************/

static void
SiS_SetCRT1Group(SiS_Private *SiS_Pr, USHORT ModeNo, USHORT ModeIdIndex)
{
  USHORT  StandTableIndex,RefreshRateTableIndex;

  SiS_Pr->SiS_CRT1Mode = ModeNo;
  StandTableIndex = 0;

  SiS_ResetSegmentRegisters(SiS_Pr);
  SiS_SetSeqRegs(SiS_Pr, StandTableIndex);
  SiS_SetMiscRegs(SiS_Pr, StandTableIndex);
  SiS_SetCRTCRegs(SiS_Pr, StandTableIndex);
  SiS_SetATTRegs(SiS_Pr, StandTableIndex);
  SiS_SetGRCRegs(SiS_Pr, StandTableIndex);
  SiS_ClearExt1Regs(SiS_Pr, ModeNo);

  RefreshRateTableIndex = SiS_GetRatePtr(SiS_Pr, ModeNo, ModeIdIndex);

  if(RefreshRateTableIndex != 0xFFFF) {
     SiS_SetCRT1Sync(SiS_Pr, RefreshRateTableIndex);
     SiS_SetCRT1CRTC(SiS_Pr, ModeNo, ModeIdIndex, RefreshRateTableIndex);
     SiS_SetCRT1Offset(SiS_Pr, ModeNo, ModeIdIndex, RefreshRateTableIndex);
     SiS_SetCRT1VCLK(SiS_Pr, ModeNo, ModeIdIndex, RefreshRateTableIndex);
  }

  SiS_SetCRT1FIFO_310(SiS_Pr, ModeNo, ModeIdIndex);

  SiS_SetCRT1ModeRegs(SiS_Pr, ModeNo, ModeIdIndex, RefreshRateTableIndex);

  SiS_LoadDAC(SiS_Pr, ModeNo, ModeIdIndex);

  SiS_DisplayOn(SiS_Pr);
}

/*********************************************/
/*         XFree86: SET SCREEN PITCH         */
/*********************************************/

static void
SiS_SetPitch(SiS_Private *SiS_Pr, ScrnInfoPtr pScrn)
{
   SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
   UShort HDisplay = pSiSUSB->scrnPitch >> 3;

   SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,0x13,(HDisplay & 0xFF));
   SiS_SetRegANDOR(SiS_Pr,SiS_Pr->SiS_P3c4,0x0E,0xF0,(HDisplay >> 8));
}

/*********************************************/
/*                 SiSSetMode()              */
/*********************************************/

BOOLEAN
SiSUSBSetMode(SiS_Private *SiS_Pr, ScrnInfoPtr pScrn, USHORT ModeNo, BOOLEAN dosetpitch)
{
   USHORT  ModeIdIndex;
   SISIOADDRESS BaseAddr = SiS_Pr->IOAddress;

   if(SiS_Pr->UseCustomMode) {
      ModeNo = 0xfe;
   }

   SiSUSB_InitPtr(SiS_Pr);
   SiSUSBRegInit(SiS_Pr, BaseAddr);
   SiS_GetSysFlags(SiS_Pr);

   SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3c4,0x05,0x86);

   SiSInitPCIetc(SiS_Pr);

   if(!SiS_Pr->UseCustomMode) ModeNo &= 0x7f;

   if(!SiS_Pr->UseCustomMode) {
      if(!(SiS_SearchModeID(SiS_Pr, &ModeNo, &ModeIdIndex))) return FALSE;
      SiS_Pr->SiS_ModeType = SiS_Pr->SiS_EModeIDTable[ModeIdIndex].Ext_ModeFlag & ModeTypeMask;
   } else {
      ModeIdIndex = 0;
      SiS_Pr->SiS_ModeType = SiS_Pr->CModeFlag & ModeTypeMask;
   }

   SiS_Pr->SiS_SetFlag = LowModeTests;

   /* Set mode on CRT1 */
   SiS_SetCRT1Group(SiS_Pr, ModeNo, ModeIdIndex);

   SiS_HandleCRT1(SiS_Pr);

   SiS_DisplayOn(SiS_Pr);
   SiS_SetRegByte(SiS_Pr,SiS_Pr->SiS_P3c6,0xFF);

   if(pScrn) {
      /* SetPitch: Adapt to virtual size & position */
      if((ModeNo > 0x13) && (dosetpitch)) {
         SiS_SetPitch(SiS_Pr, pScrn);
      }
   }

   /* Store mode number */
   SiS_SetReg(SiS_Pr,SiS_Pr->SiS_P3d4,0x34,ModeNo);

   return TRUE;
}

/*********************************************/
/*          XFree86: SiSBIOSSetMode()        */
/*           for non-Dual-Head mode          */
/*********************************************/

BOOLEAN
SiSUSBBIOSSetMode(SiS_Private *SiS_Pr, ScrnInfoPtr pScrn,
               DisplayModePtr mode, BOOLEAN IsCustom)
{
   SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
   UShort ModeNo = 0;

   SiS_Pr->UseCustomMode = FALSE;

   if((IsCustom) && (SiSUSB_CheckBuildCustomMode(pScrn, mode, pSiSUSB->VBFlags))) {

      xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 3, "Setting custom mode %dx%d\n",
	 	SiS_Pr->CHDisplay,
		(mode->Flags & V_INTERLACE ? SiS_Pr->CVDisplay * 2 :
		   (mode->Flags & V_DBLSCAN ? SiS_Pr->CVDisplay / 2 :
		      SiS_Pr->CVDisplay)));

   } else {

      /* Don't need vbflags here; checks done earlier */
      ModeNo = SiSUSB_GetModeNumber(pScrn, mode, 0);
      if(!ModeNo) return FALSE;

      xf86DrvMsgVerb(pScrn->scrnIndex, X_INFO, 3, "Setting standard mode 0x%x\n", ModeNo);

   }

   return(SiSUSBSetMode(SiS_Pr, pScrn, ModeNo, TRUE));
}

/* Helper functions */

#ifndef GETBITSTR
#define BITMASK(h,l)    	(((unsigned)(1U << ((h)-(l)+1))-1)<<(l))
#define GENMASK(mask)   	BITMASK(1?mask,0?mask)
#define GETBITS(var,mask)   	(((var) & GENMASK(mask)) >> (0?mask))
#define GETBITSTR(val,from,to)  ((GETBITS(val,from)) << (0?to))
#endif

static void
SiSUSB_CalcCRRegisters(SiS_Private *SiS_Pr, int depth)
{
   SiS_Pr->CCRT1CRTC[0]  =  ((SiS_Pr->CHTotal >> 3) - 5) & 0xff;		/* CR0 */
   SiS_Pr->CCRT1CRTC[1]  =  (SiS_Pr->CHDisplay >> 3) - 1;			/* CR1 */
   SiS_Pr->CCRT1CRTC[2]  =  (SiS_Pr->CHBlankStart >> 3) - 1;			/* CR2 */
   SiS_Pr->CCRT1CRTC[3]  =  (((SiS_Pr->CHBlankEnd >> 3) - 1) & 0x1F) | 0x80;	/* CR3 */
   SiS_Pr->CCRT1CRTC[4]  =  (SiS_Pr->CHSyncStart >> 3) + 3;			/* CR4 */
   SiS_Pr->CCRT1CRTC[5]  =  ((((SiS_Pr->CHBlankEnd >> 3) - 1) & 0x20) << 2) |	/* CR5 */
			    (((SiS_Pr->CHSyncEnd >> 3) + 3) & 0x1F);

   SiS_Pr->CCRT1CRTC[6]  =  (SiS_Pr->CVTotal - 2) & 0xFF;			/* CR6 */
   SiS_Pr->CCRT1CRTC[7]  =  (((SiS_Pr->CVTotal - 2) & 0x100) >> 8)		/* CR7 */
			  | (((SiS_Pr->CVDisplay - 1) & 0x100) >> 7)
			  | ((SiS_Pr->CVSyncStart & 0x100) >> 6)
			  | (((SiS_Pr->CVBlankStart - 1) & 0x100) >> 5)
			  | 0x10
			  | (((SiS_Pr->CVTotal - 2) & 0x200)   >> 4)
			  | (((SiS_Pr->CVDisplay - 1) & 0x200) >> 3)
			  | ((SiS_Pr->CVSyncStart & 0x200) >> 2);

   SiS_Pr->CCRT1CRTC[16] = ((((SiS_Pr->CVBlankStart - 1) & 0x200) >> 4) >> 5); 	/* CR9 */

   if(depth != 8) {
      if(SiS_Pr->CHDisplay >= 1600)      SiS_Pr->CCRT1CRTC[16] |= 0x60;		/* SRE */
      else if(SiS_Pr->CHDisplay >= 640)  SiS_Pr->CCRT1CRTC[16] |= 0x40;
   }

   SiS_Pr->CCRT1CRTC[8] =  (SiS_Pr->CVSyncStart     ) & 0xFF;			/* CR10 */
   SiS_Pr->CCRT1CRTC[9] =  ((SiS_Pr->CVSyncEnd      ) & 0x0F) | 0x80;		/* CR11 */
   SiS_Pr->CCRT1CRTC[10] = (SiS_Pr->CVDisplay    - 1) & 0xFF;			/* CR12 */
   SiS_Pr->CCRT1CRTC[11] = (SiS_Pr->CVBlankStart - 1) & 0xFF;			/* CR15 */
   SiS_Pr->CCRT1CRTC[12] = (SiS_Pr->CVBlankEnd   - 1) & 0xFF;			/* CR16 */

   SiS_Pr->CCRT1CRTC[13] =							/* SRA */
                        GETBITSTR((SiS_Pr->CVTotal     -2), 10:10, 0:0) |
                        GETBITSTR((SiS_Pr->CVDisplay   -1), 10:10, 1:1) |
                        GETBITSTR((SiS_Pr->CVBlankStart-1), 10:10, 2:2) |
                        GETBITSTR((SiS_Pr->CVSyncStart   ), 10:10, 3:3) |
                        GETBITSTR((SiS_Pr->CVBlankEnd  -1),   8:8, 4:4) |
                        GETBITSTR((SiS_Pr->CVSyncEnd     ),   4:4, 5:5) ;

   SiS_Pr->CCRT1CRTC[14] =							/* SRB */
                        GETBITSTR((SiS_Pr->CHTotal      >> 3) - 5, 9:8, 1:0) |
                        GETBITSTR((SiS_Pr->CHDisplay    >> 3) - 1, 9:8, 3:2) |
                        GETBITSTR((SiS_Pr->CHBlankStart >> 3) - 1, 9:8, 5:4) |
                        GETBITSTR((SiS_Pr->CHSyncStart  >> 3) + 3, 9:8, 7:6) ;


   SiS_Pr->CCRT1CRTC[15] =							/* SRC */
                        GETBITSTR((SiS_Pr->CHBlankEnd >> 3) - 1, 7:6, 1:0) |
                        GETBITSTR((SiS_Pr->CHSyncEnd  >> 3) + 3, 5:5, 2:2) ;
}

void
SiSUSB_MakeClockRegs(ScrnInfoPtr pScrn, int clock, UCHAR *p2b, UCHAR *p2c)
{
   int          out_n, out_dn, out_div, out_sbit, out_scale;
   unsigned int vclk[5];

#define Midx         0
#define Nidx         1
#define VLDidx       2
#define Pidx         3
#define PSNidx       4

   if(SiSUSB_compute_vclk(clock, &out_n, &out_dn, &out_div, &out_sbit, &out_scale)) {
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
      SiSUSBCalcClock(pScrn, clock, 2, vclk);
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

USHORT
SiSUSB_CheckBuildCustomMode(ScrnInfoPtr pScrn, DisplayModePtr mode, unsigned int VBFlags)
{
   SISUSBPtr pSiSUSB = SISUSBPTR(pScrn);
   int depth = pSiSUSB->CurrentLayout.bitsPerPixel;

   pSiSUSB->SiS_Pr->CModeFlag = 0;

   pSiSUSB->SiS_Pr->CDClock = mode->Clock;

   pSiSUSB->SiS_Pr->CHDisplay = mode->HDisplay;
   pSiSUSB->SiS_Pr->CHSyncStart = mode->HSyncStart;
   pSiSUSB->SiS_Pr->CHSyncEnd = mode->HSyncEnd;
   pSiSUSB->SiS_Pr->CHTotal = mode->HTotal;

   pSiSUSB->SiS_Pr->CVDisplay = mode->VDisplay;
   pSiSUSB->SiS_Pr->CVSyncStart = mode->VSyncStart;
   pSiSUSB->SiS_Pr->CVSyncEnd = mode->VSyncEnd;
   pSiSUSB->SiS_Pr->CVTotal = mode->VTotal;

   pSiSUSB->SiS_Pr->CFlags = mode->Flags;

   if(pSiSUSB->SiS_Pr->CFlags & V_INTERLACE) {
      pSiSUSB->SiS_Pr->CVDisplay >>= 1;
      pSiSUSB->SiS_Pr->CVSyncStart >>= 1;
      pSiSUSB->SiS_Pr->CVSyncEnd >>= 1;
      pSiSUSB->SiS_Pr->CVTotal >>= 1;
   } else if(pSiSUSB->SiS_Pr->CFlags & V_DBLSCAN) {
      pSiSUSB->SiS_Pr->CVDisplay <<= 1;
      pSiSUSB->SiS_Pr->CVSyncStart <<= 1;
      pSiSUSB->SiS_Pr->CVSyncEnd <<= 1;
      pSiSUSB->SiS_Pr->CVTotal <<= 1;
   }

   pSiSUSB->SiS_Pr->CHBlankStart = pSiSUSB->SiS_Pr->CHDisplay;
   pSiSUSB->SiS_Pr->CHBlankEnd = pSiSUSB->SiS_Pr->CHTotal;
   pSiSUSB->SiS_Pr->CVBlankStart = pSiSUSB->SiS_Pr->CVSyncStart - 1;
   pSiSUSB->SiS_Pr->CVBlankEnd = pSiSUSB->SiS_Pr->CVTotal;

   if((!(mode->type & M_T_BUILTIN)) && (mode->HDisplay <= 512)) {
      pSiSUSB->SiS_Pr->CModeFlag |= HalfDCLK;
      pSiSUSB->SiS_Pr->CDClock <<= 1;
   }

   /* Note: For CRT2, HDisplay, HSync* and HTotal must be shifted left
    * in HalfDCLK mode.
    */

   SiSUSB_MakeClockRegs(pScrn, pSiSUSB->SiS_Pr->CDClock, &pSiSUSB->SiS_Pr->CSR2B, &pSiSUSB->SiS_Pr->CSR2C);

   pSiSUSB->SiS_Pr->CSRClock = (pSiSUSB->SiS_Pr->CDClock / 1000) + 1;

   SiSUSB_CalcCRRegisters(pSiSUSB->SiS_Pr, depth);

   switch(depth) {
   case 8:  pSiSUSB->SiS_Pr->CModeFlag |= 0x223b; break;
   case 16: pSiSUSB->SiS_Pr->CModeFlag |= 0x227d; break;
   case 32: pSiSUSB->SiS_Pr->CModeFlag |= 0x22ff; break;
   default: return 0;
   }

   if(pSiSUSB->SiS_Pr->CFlags & V_DBLSCAN)
      pSiSUSB->SiS_Pr->CModeFlag |= DoubleScanMode;

   if((pSiSUSB->SiS_Pr->CVDisplay >= 1024)	||
      (pSiSUSB->SiS_Pr->CVTotal >= 1024)   ||
      (pSiSUSB->SiS_Pr->CHDisplay >= 1024))
      pSiSUSB->SiS_Pr->CModeFlag |= LineCompareOff;

   pSiSUSB->SiS_Pr->CInfoFlag = 0x0007;

   if(pSiSUSB->SiS_Pr->CFlags & V_NHSYNC)
      pSiSUSB->SiS_Pr->CInfoFlag |= 0x4000;

   if(pSiSUSB->SiS_Pr->CFlags & V_NVSYNC)
      pSiSUSB->SiS_Pr->CInfoFlag |= 0x8000;

   if(pSiSUSB->SiS_Pr->CFlags & V_INTERLACE)
      pSiSUSB->SiS_Pr->CInfoFlag |= InterlaceMode;

   pSiSUSB->SiS_Pr->UseCustomMode = TRUE;
   return 1;
}

/* Build a list of supported modes:
 * Built-in modes for which we have all data are M_T_DEFAULT,
 * modes derived from DDC or database data are M_T_BUILTIN
 */
DisplayModePtr
SiSUSBBuildBuiltInModeList(ScrnInfoPtr pScrn, BOOLEAN includelcdmodes, BOOLEAN isfordvi, BOOLEAN fakecrt2modes)
{
   SISUSBPtr      pSiSUSB = SISUSBPTR(pScrn);
   unsigned short VRE, VBE, VRS, VBS, VDE, VT;
   unsigned short HRE, HBE, HRS, HBS, HDE, HT;
   unsigned char  sr_data, cr_data, cr_data2, cr_data3;
   unsigned char  sr2b, sr2c;
   float          num, denum, postscalar, divider;
   int            A, B, C, D, E, F, temp, i, j, index, vclkindex;
   DisplayModePtr new = NULL, current = NULL, first = NULL;
   BOOLEAN        IsHDCLK;
#if 0
   DisplayModePtr backup = NULL;
#endif

   pSiSUSB->backupmodelist = NULL;

   SiSUSB_InitPtr(pSiSUSB->SiS_Pr);

   i = 0;
   while(pSiSUSB->SiS_Pr->SiS_RefIndex[i].Ext_InfoFlag != 0xFFFF) {

      index = pSiSUSB->SiS_Pr->SiS_RefIndex[i].Ext_CRT1CRTC;
      if(fakecrt2modes) {
         if(pSiSUSB->SiS_Pr->SiS_RefIndex[i].Ext_FakeCRT2CRTC) {
	    index = pSiSUSB->SiS_Pr->SiS_RefIndex[i].Ext_FakeCRT2CRTC;
	 }
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

      sprintf(current->name, "%dx%d", pSiSUSB->SiS_Pr->SiS_RefIndex[i].XRes,
                                      pSiSUSB->SiS_Pr->SiS_RefIndex[i].YRes);

      current->status = MODE_OK;

      current->type = M_T_DEFAULT;

      vclkindex = pSiSUSB->SiS_Pr->SiS_RefIndex[i].Ext_CRTVCLK;
      if(fakecrt2modes) {
         if(pSiSUSB->SiS_Pr->SiS_RefIndex[i].Ext_FakeCRT2Clk) {
	    vclkindex = pSiSUSB->SiS_Pr->SiS_RefIndex[i].Ext_FakeCRT2Clk;
	 }
      }

      sr2b = pSiSUSB->SiS_Pr->SiS_VCLKData[vclkindex].SR2B;
      sr2c = pSiSUSB->SiS_Pr->SiS_VCLKData[vclkindex].SR2C;

      divider = (sr2b & 0x80) ? 2.0 : 1.0;
      postscalar = (sr2c & 0x80) ?
              ( (((sr2c >> 5) & 0x03) == 0x02) ? 6.0 : 8.0) : (((sr2c >> 5) & 0x03) + 1.0);
      num = (sr2b & 0x7f) + 1.0;
      denum = (sr2c & 0x1f) + 1.0;

      current->Clock = (int)(14318 * (divider / postscalar) * (num / denum));

      sr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[14];
	/* inSISIDXREG(SISSR, 0x0b, sr_data); */

      cr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[0];

      /* Horizontal total */
      HT = (cr_data & 0xff) |
           ((unsigned short) (sr_data & 0x03) << 8);
      A = HT + 5;

      cr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[1];

      /* Horizontal display enable end */
      HDE = (cr_data & 0xff) |
            ((unsigned short) (sr_data & 0x0C) << 6);
      E = HDE + 1;  /* 0x80 0x64 */

      cr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[4];

      /* Horizontal retrace (=sync) start */
      HRS = (cr_data & 0xff) |
            ((unsigned short) (sr_data & 0xC0) << 2);
      F = HRS - E - 3;  /* 0x06 0x06 */

      cr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[2];

      /* Horizontal blank start */
      HBS = (cr_data & 0xff) |
            ((unsigned short) (sr_data & 0x30) << 4);

      sr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[15];

      cr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[3];

      cr_data2 = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[5];

      /* Horizontal blank end */
      HBE = (cr_data & 0x1f) |
            ((unsigned short) (cr_data2 & 0x80) >> 2) |
	    ((unsigned short) (sr_data & 0x03) << 6);

      /* Horizontal retrace (=sync) end */
      HRE = (cr_data2 & 0x1f) | ((sr_data & 0x04) << 3);

      temp = HBE - ((E - 1) & 255);
      B = (temp > 0) ? temp : (temp + 256);

      temp = HRE - ((E + F + 3) & 63);
      C = (temp > 0) ? temp : (temp + 64); /* 0x0b 0x0b */

      D = B - F - C;

      if((pSiSUSB->SiS_Pr->SiS_RefIndex[i].XRes == 320) &&
	 ((pSiSUSB->SiS_Pr->SiS_RefIndex[i].YRes == 200) ||
	  (pSiSUSB->SiS_Pr->SiS_RefIndex[i].YRes == 240))) {

	 /* Terrible hack, but correct CRTC data for
	  * these modes only produces a black screen...
	  * (HRE is 0, leading into a too large C and
	  * a negative D. The CRT controller does not
	  * seem to like correcting HRE to 50
	  */
	 current->HDisplay   = 320;
         current->HSyncStart = 328;
         current->HSyncEnd   = 376;
         current->HTotal     = 400;

      } else {

         current->HDisplay   = (E * 8);
         current->HSyncStart = (E * 8) + (F * 8);
         current->HSyncEnd   = (E * 8) + (F * 8) + (C * 8);
         current->HTotal     = (E * 8) + (F * 8) + (C * 8) + (D * 8);

      }

#ifdef TWDEBUG
      xf86DrvMsg(0, X_INFO,
        "H: A %d B %d C %d D %d E %d F %d  HT %d HDE %d HRS %d HBS %d HBE %d HRE %d\n",
      	A, B, C, D, E, F, HT, HDE, HRS, HBS, HBE, HRE);
#endif

      sr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[13];

      cr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[6];

      cr_data2 = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[7];

      /* Vertical total */
      VT = (cr_data & 0xFF) |
           ((unsigned short) (cr_data2 & 0x01) << 8) |
	   ((unsigned short)(cr_data2 & 0x20) << 4) |
	   ((unsigned short) (sr_data & 0x01) << 10);
      A = VT + 2;

      cr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[10];

      /* Vertical display enable end */
      VDE = (cr_data & 0xff) |
            ((unsigned short) (cr_data2 & 0x02) << 7) |
	    ((unsigned short) (cr_data2 & 0x40) << 3) |
	    ((unsigned short) (sr_data & 0x02) << 9);
      E = VDE + 1;

      cr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[8];

      /* Vertical retrace (=sync) start */
      VRS = (cr_data & 0xff) |
            ((unsigned short) (cr_data2 & 0x04) << 6) |
	    ((unsigned short) (cr_data2 & 0x80) << 2) |
	    ((unsigned short) (sr_data & 0x08) << 7);
      F = VRS + 1 - E;

      cr_data =  pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[11];

      cr_data3 = (pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[16] & 0x01) << 5;

      /* Vertical blank start */
      VBS = (cr_data & 0xff) |
            ((unsigned short) (cr_data2 & 0x08) << 5) |
	    ((unsigned short) (cr_data3 & 0x20) << 4) |
	    ((unsigned short) (sr_data & 0x04) << 8);

      cr_data =  pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[12];

      /* Vertical blank end */
      VBE = (cr_data & 0xff) |
            ((unsigned short) (sr_data & 0x10) << 4);
      temp = VBE - ((E - 1) & 511);
      B = (temp > 0) ? temp : (temp + 512);

      cr_data = pSiSUSB->SiS_Pr->SiS_CRT1Table[index].CR[9];

      /* Vertical retrace (=sync) end */
      VRE = (cr_data & 0x0f) | ((sr_data & 0x20) >> 1);
      temp = VRE - ((E + F - 1) & 31);
      C = (temp > 0) ? temp : (temp + 32);

      D = B - F - C;

      current->VDisplay   = VDE + 1;
      current->VSyncStart = VRS + 1;
      current->VSyncEnd   = ((VRS & ~0x1f) | VRE) + 1;
      if(VRE <= (VRS & 0x1f)) current->VSyncEnd += 32;
      current->VTotal     = E + D + C + F;

#if 0
      current->VDisplay   = E;
      current->VSyncStart = E + D;
      current->VSyncEnd   = E + D + C;
      current->VTotal     = E + D + C + F;
#endif

#ifdef TWDEBUG
      xf86DrvMsg(0, X_INFO,
        "V: A %d B %d C %d D %d E %d F %d  VT %d VDE %d VRS %d VBS %d VBE %d VRE %d\n",
      	A, B, C, D, E, F, VT, VDE, VRS, VBS, VBE, VRE);
#endif

      if(pSiSUSB->SiS_Pr->SiS_RefIndex[i].Ext_InfoFlag & 0x4000)
          current->Flags |= V_NHSYNC;
      else
          current->Flags |= V_PHSYNC;

      if(pSiSUSB->SiS_Pr->SiS_RefIndex[i].Ext_InfoFlag & 0x8000)
      	  current->Flags |= V_NVSYNC;
      else
          current->Flags |= V_PVSYNC;

      if(pSiSUSB->SiS_Pr->SiS_RefIndex[i].Ext_InfoFlag & 0x0080)
          current->Flags |= V_INTERLACE;

      j = 0;
      IsHDCLK = FALSE;
      while(pSiSUSB->SiS_Pr->SiS_EModeIDTable[j].Ext_ModeID != 0xff) {
          if(pSiSUSB->SiS_Pr->SiS_EModeIDTable[j].Ext_ModeID ==
	                  pSiSUSB->SiS_Pr->SiS_RefIndex[i].ModeID) {
              if(pSiSUSB->SiS_Pr->SiS_EModeIDTable[j].Ext_ModeFlag & DoubleScanMode) {
	      	  current->Flags |= V_DBLSCAN;
              }
	      if(pSiSUSB->SiS_Pr->SiS_EModeIDTable[j].Ext_ModeFlag & HalfDCLK) {
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
#else
        (void)VBS;  (void)HBS;  (void)A;
#endif

      i++;
   }

   return first;
}





