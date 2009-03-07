/*
 * General structure definitions for universal mode switching modules
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria
 *
 * If distributed as part of the Linux kernel, the following license terms
 * apply:
 *
 * * This program is free software; you can redistribute it and/or modify
 * * it under the terms of the GNU General Public License as published by
 * * the Free Software Foundation; either version 2 of the named License,
 * * or any later version.
 * *
 * * This program is distributed in the hope that it will be useful,
 * * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * * GNU General Public License for more details.
 * *
 * * You should have received a copy of the GNU General Public License
 * * along with this program; if not, write to the Free Software
 * * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
 *
 * Otherwise, the following license terms apply:
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

#ifndef _VSTRUCT_
#define _VSTRUCT_

typedef struct _SiS_StandTableStruct
{
	UCHAR  CRT_COLS;
	UCHAR  ROWS;
	UCHAR  CHAR_HEIGHT;
	USHORT CRT_LEN;
	UCHAR  SR[4];
	UCHAR  MISC;
	UCHAR  CRTC[0x19];
	UCHAR  ATTR[0x14];
	UCHAR  GRC[9];
} SiS_StandTableStruct;

typedef struct _SiS_ExtStruct
{
	UCHAR  Ext_ModeID;
	USHORT Ext_ModeFlag;
	USHORT Ext_VESAID;
	UCHAR  Ext_RESINFO;
	UCHAR  VB_ExtTVFlickerIndex;
	UCHAR  VB_ExtTVEdgeIndex;
	UCHAR  VB_ExtTVYFilterIndex;
	UCHAR  VB_ExtTVYFilterIndexROM661;
	UCHAR  REFindex;
	CHAR   ROMMODEIDX661;
} SiS_ExtStruct;

typedef struct _SiS_Ext2Struct
{
	USHORT Ext_InfoFlag;
	UCHAR  Ext_CRT1CRTC;
	UCHAR  Ext_CRTVCLK;
	UCHAR  Ext_CRT2CRTC;
	UCHAR  Ext_CRT2CRTC_NS;
	UCHAR  ModeID;
	USHORT XRes;
	USHORT YRes;
	UCHAR  Ext_PDC;
	UCHAR  Ext_FakeCRT2CRTC;
	UCHAR  Ext_FakeCRT2Clk;
} SiS_Ext2Struct;

typedef struct _SiS_CRT1TableStruct
{
	UCHAR  CR[17];
} SiS_CRT1TableStruct;

typedef struct _SiS_VCLKDataStruct
{
	UCHAR  SR2B,SR2C;
	USHORT CLOCK;
} SiS_VCLKDataStruct;

typedef struct _SiS_ModeResInfoStruct
{
	USHORT HTotal;
	USHORT VTotal;
	UCHAR  XChar;
	UCHAR  YChar;
} SiS_ModeResInfoStruct;

typedef struct _SiS_Private
{
	unsigned char ChipType;
	unsigned char ChipRevision;

	void *pSiSUSB;
	SISIOADDRESS IOAddress;

	SISIOADDRESS SiS_P3c4;
	SISIOADDRESS SiS_P3d4;
	SISIOADDRESS SiS_P3c0;
	SISIOADDRESS SiS_P3ce;
	SISIOADDRESS SiS_P3c2;
	SISIOADDRESS SiS_P3ca;
	SISIOADDRESS SiS_P3c6;
	SISIOADDRESS SiS_P3c7;
	SISIOADDRESS SiS_P3c8;
	SISIOADDRESS SiS_P3c9;
	SISIOADDRESS SiS_P3cb;
	SISIOADDRESS SiS_P3cc;
	SISIOADDRESS SiS_P3cd;
	SISIOADDRESS SiS_P3da;
	SISIOADDRESS SiS_Part1Port;

	UCHAR  SiS_MyCR63;
	USHORT SiS_CRT1Mode;
	int    SiS_RAMType;
	UCHAR  SiS_ChannelAB;
	UCHAR  SiS_DataBusWidth;
	USHORT SiS_ModeType;
	USHORT SiS_SetFlag;

	const SiS_StandTableStruct  *SiS_StandTable;
	const SiS_ExtStruct         *SiS_EModeIDTable;
	const SiS_Ext2Struct        *SiS_RefIndex;
	const SiS_CRT1TableStruct   *SiS_CRT1Table;
	SiS_VCLKDataStruct    	    *SiS_VCLKData;
	const SiS_ModeResInfoStruct *SiS_ModeResInfo;
	const UCHAR                 *pSiS_SoftSetting;
	BOOLEAN UseCustomMode;
	BOOLEAN CRT1UsesCustomMode;
	USHORT  CHDisplay;
	USHORT  CHSyncStart;
	USHORT  CHSyncEnd;
	USHORT  CHTotal;
	USHORT  CHBlankStart;
	USHORT  CHBlankEnd;
	USHORT  CVDisplay;
	USHORT  CVSyncStart;
	USHORT  CVSyncEnd;
	USHORT  CVTotal;
	USHORT  CVBlankStart;
	USHORT  CVBlankEnd;
	ULONG   CDClock;
	ULONG   CFlags;
	UCHAR   CCRT1CRTC[17];
	UCHAR   CSR2B;
	UCHAR   CSR2C;
	USHORT  CSRClock;
	USHORT  CSRClock_CRT1;
	USHORT  CModeFlag;
	USHORT  CModeFlag_CRT1;
	USHORT  CInfoFlag;

} SiS_Private;

#endif

