/* $XFree86$ */
/* $XdotOrg: driver/xf86-video-sisusb/src/sisusb_regs.h,v 1.5 2005/07/09 02:50:34 twini Exp $ */
/*
 * Register access macros and register definitions
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
 * Author:  	Thomas Winischhofer <thomas@winischhofer.net>
 *
 */

#ifndef _SISUSB_REGS_H_
#define _SISUSB_REGS_H_

/* Endian related macros */

/* About endianness:
 * 1) I/O ports, PCI config registers
 *    The kernel driver handles swapping. Data written or read
 *    back is in machine endianness.
 * 2) Video memory
 *    Data is copied 1:1. Command queue, HW cursor and the like
 *    needs to be swapped.
 * 3) MMIO
 *    Data is copied 1:1. MMIO macros need to handle the swapping.
 *
 */

#if X_BYTE_ORDER == X_BIG_ENDIAN

#ifdef NEED_cpu_to_le32
static __inline CARD32 sisusb_cpu_to_le32(CARD32 v)
{
	return( (((CARD32)(v) & (CARD32)0x000000ffUL) << 24) |  \
		(((CARD32)(v) & (CARD32)0x0000ff00UL) <<  8) |  \
		(((CARD32)(v) & (CARD32)0x00ff0000UL) >>  8) |  \
		(((CARD32)(v) & (CARD32)0xff000000UL) >> 24) );
}

#define sisusb_le32_to_cpu(v) sisusb_cpu_to_le32((v))
#endif

#ifdef NEED_cpu_to_le16
static __inline CARD16 sisusb_cpu_to_le16(CARD16 v)
{
	return( (((CARD16)(v) & (CARD16)0x00ffU) << 8) |
		(((CARD16)(v) & (CARD16)0xff00U) >> 8) );
}

#define sisusb_le16_to_cpu(v) sisusb_cpu_to_le16((v))
#endif

#else

#define sisusb_cpu_to_le32(v) (v)
#define sisusb_cpu_to_le16(v) (v)
#define sisusb_le32_to_cpu(v) (v)
#define sisusb_le16_to_cpu(v) (v)

#endif

/* Video RAM access macros */

/* (Currently, these are use on all platforms; USB2VGA is handled
 * specially in the code)
 */

/* dest is video RAM, src is system RAM */
#define sisfbwritel(dest, data)        	*(dest)     = (data)
#define sisfbwritelinc(dest, data)     	*((dest)++) = (data)
#define sisfbwritelp(dest, dataptr)    	*(dest)     = *(dataptr)
#define sisfbwritelpinc(dest, dataptr) 	*((dest)++) = *((dataptr)++)

#define sisfbwritew(dest, data)        	*(dest)     = (data)
#define sisfbwritewinc(dest, data)     	*((dest)++) = (data)
#define sisfbwritewp(dest, dataptr)    	*(dest)     = *(dataptr)
#define sisfbwritewpinc(dest, dataptr) 	*((dest)++) = *((dataptr)++)

#define sisfbwriteb(dest, data)        	*(dest)     = (data)
#define sisfbwritebinc(dest, data)     	*((dest)++) = (data)
#define sisfbwritebp(dest, dataptr)    	*(dest)     = *(dataptr)
#define sisfbwritebpinc(dest, dataptr) 	*((dest)++) = *((dataptr)++)

/* Source is video RAM */
#define sisfbreadl(src)        		*(src)
#define sisfbreadlinc(src)        	*((src)++)

#define sisfbreadw(src)        		*(src)
#define sisfbreadwinc(src)        	*((src)++)

#define sisfbreadb(src)        		*(src)
#define sisfbreadbinc(src)        	*((src)++)

/* Port offsets  --------------- */

#define AROFFSET        0x40
#define ARROFFSET       0x41
#define GROFFSET        0x4e
#define SROFFSET        0x44
#define CROFFSET        0x54
#define MISCROFFSET     0x4c
#define MISCWOFFSET     0x42
#define INPUTSTATOFFSET 0x5A
#define PART1OFFSET     0x04
#define PART2OFFSET     0x10
#define PART3OFFSET     0x12
#define PART4OFFSET     0x14
#define PART5OFFSET     0x16
#define CAPTUREOFFSET   0x00
#define VIDEOOFFSET     0x02
#define COLREGOFFSET    0x48
#define PELMASKOFFSET   0x46

#define SISAR       pSiSUSB->RelIO + AROFFSET
#define SISARR      pSiSUSB->RelIO + ARROFFSET
#define SISGR       pSiSUSB->RelIO + GROFFSET
#define SISSR       pSiSUSB->RelIO + SROFFSET
#define SISCR       pSiSUSB->RelIO + CROFFSET
#define SISMISCR    pSiSUSB->RelIO + MISCROFFSET
#define SISMISCW    pSiSUSB->RelIO + MISCWOFFSET
#define SISINPSTAT  pSiSUSB->RelIO + INPUTSTATOFFSET
#define SISPART1    pSiSUSB->RelIO + PART1OFFSET
#define SISPART2    pSiSUSB->RelIO + PART2OFFSET
#define SISPART3    pSiSUSB->RelIO + PART3OFFSET
#define SISPART4    pSiSUSB->RelIO + PART4OFFSET
#define SISPART5    pSiSUSB->RelIO + PART5OFFSET
#define SISCAP      pSiSUSB->RelIO + CAPTUREOFFSET
#define SISVID      pSiSUSB->RelIO + VIDEOOFFSET
#define SISCOLIDXR  pSiSUSB->RelIO + COLREGOFFSET - 1
#define SISCOLIDX   pSiSUSB->RelIO + COLREGOFFSET
#define SISCOLDATA  pSiSUSB->RelIO + COLREGOFFSET + 1
#define SISCOL2IDX  SISPART5
#define SISCOL2DATA SISPART5 + 1
#define SISPEL      pSiSUSB->RelIO + PELMASKOFFSET

/* Video registers (300/315/330/340 series only)  --------------- */
#define  Index_VI_Passwd                        0x00

/* Video overlay horizontal start/end, unit=screen pixels */
#define  Index_VI_Win_Hor_Disp_Start_Low        0x01
#define  Index_VI_Win_Hor_Disp_End_Low          0x02
#define  Index_VI_Win_Hor_Over                  0x03 /* Overflow */

/* Video overlay vertical start/end, unit=screen pixels */
#define  Index_VI_Win_Ver_Disp_Start_Low        0x04
#define  Index_VI_Win_Ver_Disp_End_Low          0x05
#define  Index_VI_Win_Ver_Over                  0x06 /* Overflow */

/* Y Plane (4:2:0) or YUV (4:2:2) buffer start address, unit=word */
#define  Index_VI_Disp_Y_Buf_Start_Low          0x07
#define  Index_VI_Disp_Y_Buf_Start_Middle       0x08
#define  Index_VI_Disp_Y_Buf_Start_High         0x09

/* U Plane (4:2:0) buffer start address, unit=word */
#define  Index_VI_U_Buf_Start_Low               0x0A
#define  Index_VI_U_Buf_Start_Middle            0x0B
#define  Index_VI_U_Buf_Start_High              0x0C

/* V Plane (4:2:0) buffer start address, unit=word */
#define  Index_VI_V_Buf_Start_Low               0x0D
#define  Index_VI_V_Buf_Start_Middle            0x0E
#define  Index_VI_V_Buf_Start_High              0x0F

/* Pitch for Y, UV Planes, unit=word */
#define  Index_VI_Disp_Y_Buf_Pitch_Low          0x10
#define  Index_VI_Disp_UV_Buf_Pitch_Low         0x11
#define  Index_VI_Disp_Y_UV_Buf_Pitch_Middle    0x12

/* What is this ? */
#define  Index_VI_Disp_Y_Buf_Preset_Low         0x13
#define  Index_VI_Disp_Y_Buf_Preset_Middle      0x14

#define  Index_VI_UV_Buf_Preset_Low             0x15
#define  Index_VI_UV_Buf_Preset_Middle          0x16
#define  Index_VI_Disp_Y_UV_Buf_Preset_High     0x17

/* Scaling control registers */
#define  Index_VI_Hor_Post_Up_Scale_Low         0x18
#define  Index_VI_Hor_Post_Up_Scale_High        0x19
#define  Index_VI_Ver_Up_Scale_Low              0x1A
#define  Index_VI_Ver_Up_Scale_High             0x1B
#define  Index_VI_Scale_Control                 0x1C

/* Playback line buffer control */
#define  Index_VI_Play_Threshold_Low            0x1D
#define  Index_VI_Play_Threshold_High           0x1E
#define  Index_VI_Line_Buffer_Size              0x1F

/* Destination color key */
#define  Index_VI_Overlay_ColorKey_Red_Min      0x20
#define  Index_VI_Overlay_ColorKey_Green_Min    0x21
#define  Index_VI_Overlay_ColorKey_Blue_Min     0x22
#define  Index_VI_Overlay_ColorKey_Red_Max      0x23
#define  Index_VI_Overlay_ColorKey_Green_Max    0x24
#define  Index_VI_Overlay_ColorKey_Blue_Max     0x25

/* Source color key, YUV color space */
#define  Index_VI_Overlay_ChromaKey_Red_Y_Min   0x26
#define  Index_VI_Overlay_ChromaKey_Green_U_Min 0x27
#define  Index_VI_Overlay_ChromaKey_Blue_V_Min  0x28
#define  Index_VI_Overlay_ChromaKey_Red_Y_Max   0x29
#define  Index_VI_Overlay_ChromaKey_Green_U_Max 0x2A
#define  Index_VI_Overlay_ChromaKey_Blue_V_Max  0x2B

/* Contrast enhancement and brightness control */
#define  Index_VI_Contrast_Factor               0x2C	/* obviously unused/undefined */
#define  Index_VI_Brightness                    0x2D
#define  Index_VI_Contrast_Enh_Ctrl             0x2E

#define  Index_VI_Key_Overlay_OP                0x2F

#define  Index_VI_Control_Misc0                 0x30
#define  Index_VI_Control_Misc1                 0x31
#define  Index_VI_Control_Misc2                 0x32

/* Subpicture registers */
#define  Index_VI_SubPict_Buf_Start_Low		0x33
#define  Index_VI_SubPict_Buf_Start_Middle	0x34
#define  Index_VI_SubPict_Buf_Start_High	0x35

/* What is this ? */
#define  Index_VI_SubPict_Buf_Preset_Low	0x36
#define  Index_VI_SubPict_Buf_Preset_Middle	0x37

/* Subpicture pitch, unit=16 bytes */
#define  Index_VI_SubPict_Buf_Pitch		0x38

/* Subpicture scaling control */
#define  Index_VI_SubPict_Hor_Scale_Low		0x39
#define  Index_VI_SubPict_Hor_Scale_High	0x3A
#define  Index_VI_SubPict_Vert_Scale_Low	0x3B
#define  Index_VI_SubPict_Vert_Scale_High	0x3C

#define  Index_VI_SubPict_Scale_Control		0x3D
/* (0x40 = enable/disable subpicture) */

/* Subpicture line buffer control */
#define  Index_VI_SubPict_Threshold		0x3E

/* What is this? */
#define  Index_VI_FIFO_Max			0x3F

/* Subpicture palette; 16 colors, total 32 bytes address space */
#define  Index_VI_SubPict_Pal_Base_Low		0x40
#define  Index_VI_SubPict_Pal_Base_High		0x41

/* I wish I knew how to use these ... */
#define  Index_MPEG_Read_Ctrl0                  0x60	/* MPEG auto flip */
#define  Index_MPEG_Read_Ctrl1                  0x61	/* MPEG auto flip */
#define  Index_MPEG_Read_Ctrl2                  0x62	/* MPEG auto flip */
#define  Index_MPEG_Read_Ctrl3                  0x63	/* MPEG auto flip */

/* MPEG AutoFlip scale */
#define  Index_MPEG_Ver_Up_Scale_Low            0x64
#define  Index_MPEG_Ver_Up_Scale_High           0x65

#define  Index_MPEG_Y_Buf_Preset_Low		0x66
#define  Index_MPEG_Y_Buf_Preset_Middle		0x67
#define  Index_MPEG_UV_Buf_Preset_Low		0x68
#define  Index_MPEG_UV_Buf_Preset_Middle	0x69
#define  Index_MPEG_Y_UV_Buf_Preset_High	0x6A

/* The following registers only exist on the 315/330/340 series */

/* Bit 16:24 of Y_U_V buf start address */
#define  Index_VI_Y_Buf_Start_Over		0x6B
#define  Index_VI_U_Buf_Start_Over		0x6C
#define  Index_VI_V_Buf_Start_Over		0x6D

#define  Index_VI_Disp_Y_Buf_Pitch_High		0x6E
#define  Index_VI_Disp_UV_Buf_Pitch_High	0x6F

/* Hue and saturation */
#define	 Index_VI_Hue				0x70
#define  Index_VI_Saturation			0x71

#define  Index_VI_SubPict_Start_Over		0x72
#define  Index_VI_SubPict_Buf_Pitch_High	0x73

#define  Index_VI_Control_Misc3			0x74

/* 340 and later: */
/* DDA registers 0x75 - 0xb4 */
/* threshold high 0xb5, 0xb6 */
#define  Index_VI_Line_Buffer_Size_High		0xb7


/* Bits in Scale control (0x1c) */
#define  VI_Scale_Ctrl_Horiz_DDA                0x20
#define  VI_Scale_Ctrl_Vert_DDA                 0x40

/* Bits (and helpers) for Index_VI_Control_Misc0 */
#define  VI_Misc0_Enable_Capture_AutoFlip       0x01 /* 340 only? */
#define  VI_Misc0_Enable_Overlay		0x02
#define  VI_Misc0_420_Plane_Enable		0x04 /* Select Plane or Packed mode */
#define  VI_Misc0_422_Enable			0x20 /* Select 422 or 411 mode */
#define  VI_Misc0_Fmt_YVU420P			0x0C /* YUV420 Planar (I420, YV12) */
#define  VI_Misc0_Fmt_YUYV			0x28 /* YUYV Packed (=YUY2) */
#define  VI_Misc0_Fmt_UYVY			0x08 /* (UYVY) */
#define  VI_Misc0_Fmt_YVYU                      0x38 /* (YVYU) (315 series only?) */
#define  VI_Misc0_Fmt_NV21			0x5c /* (330 series only?) */
#define  VI_Misc0_Fmt_NV12			0x4c /* (330 series only?) */
#define  VI_Misc0_ChromaKeyRGBYUV               0x40 /* 300 series only: 0 = RGB, 1 = YUV */

/* Bits for Index_VI_Control_Misc1 */
#define  VI_Misc1_DisableGraphicsAtOverlay      0x01 /* Disables graphics display in overlay area */
#define  VI_Misc1_BOB_Enable			0x02 /* Enable BOB de-interlacer */
#define	 VI_Misc1_Line_Merge			0x04
#define  VI_Misc1_Field_Mode			0x08 /* ? Assume even/odd fields interleaved in memory ? */
#define  VI_Misc1_Non_Interleave                0x10 /* ? Odd and Even fields are not interleaved ? */
#define  VI_Misc1_Buf_Addr_Lock			0x20 /* 315 series only? */
/* #define  VI_Misc1_?                          0x40  */
/* #define  VI_Misc1_?                          0x80  */

/* Bits for Index_VI_Control_Misc2 */
#define  VI_Misc2_Select_Video2			0x01
#define  VI_Misc2_Video2_On_Top			0x02
#define  VI_Misc2_DisableGraphics              	0x04 /* Disable graphics display entirely (<= 650 only, not >= M650, 651) */
#define  VI_Misc2_Vertical_Interpol		0x08
#define  VI_Misc2_Dual_Line_Merge               0x10  /* dual-overlay chips only; "dual video windows relative line buffer merge" */
#define  VI_Misc2_All_Line_Merge                0x20  /* > 315 only */
#define  VI_Misc2_Auto_Flip_Enable		0x40
#define  VI_Misc2_Video_Reg_Write_Enable        0x80  /* 315 series only? */

/* Bits for Index_VI_Control_Misc3 */
#define  VI_Misc3_Submit_Video_1		0x01  /* AKA "address ready" */
#define  VI_Misc3_Submit_Video_2		0x02  /* AKA "address ready" */
#define  VI_Misc3_Submit_SubPict		0x04  /* AKA "address ready" */

/* Values for Index_VI_Key_Overlay_OP (0x2F) */
#define  VI_ROP_Never				0x00
#define  VI_ROP_DestKey				0x03
#define  VI_ROP_ChromaKey			0x05
#define  VI_ROP_NotChromaKey                    0x0A
#define  VI_ROP_Always				0x0F


/* mmio registers for video */
#define REG_PRIM_CRT_COUNTER    0x8514

/* MPEG MMIO registers (630 and later) ----------------------------------------- */

/* Not public (yet?) */


#endif  /* SIS_REGS_H_ */


