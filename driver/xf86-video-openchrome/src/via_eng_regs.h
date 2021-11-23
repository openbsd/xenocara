/*
 * Copyright 1998-2003 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2003 S3 Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _VIA_ENG_REGS_H_
#define _VIA_H_ 1

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdlib.h>

/* Video status flag */

#define VIDEO_HIDE              0x00000000  /*Video off*/
#define VIDEO_SHOW              0x80000000  /*Video on*/
#define VIDEO_ACTIVE            0x10000000  /*Video active*/
#define VIDEO_MPEG_INUSE        0x08000000  /*Video is used with MPEG */
#define VIDEO_HQV_INUSE         0x04000000  /*Video is used with HQV*/
#define VIDEO_CAPTURE0_INUSE    0x02000000  /*Video is used with CAPTURE 0*/
#define VIDEO_CAPTURE1_INUSE    0x00000000  /*Video is used with CAPTURE 1*/
#define VIDEO_1_INUSE           0x01000000  /*Video 1 is used with software flip*/
#define VIDEO_3_INUSE           0x00000000  /*Video 3 is used with software flip*/
#define VIDEO_ON                0x00100000
#define MPEG_USE_V1             0x00010000  /*[16] : 1:MPEG use V1, 0:MPEG use V3*/
#define MPEG_USE_V3             0x00000000  /*[16] : 1:MPEG use V1, 0:MPEG use V3*/
#define MPEG_USE_HQV            0x00020000  /*[17] : 1:MPEG use HQV,0:MPEG not use HQV*/
#define MPEG_USE_HW_FLIP        0x00040000  /*[18] : 1:MPEG use H/W flip,0:MPEG use S/W flip*/
#define MPEG_USE_SW_FLIP        0x00000000  /*[18] : 1:MPEG use H/W flip,0:MPEG use S/W flip*/
#define CAP0_USE_V1             0x00001000  /*[12] : 1:Capture 0 use V1, 0:Capture 0 use V3*/
#define CAP0_USE_V3             0x00000000  /*[12] : 1:Capture 0 use V1, 0:Capture 0 use V3*/
#define CAP0_USE_HQV            0x00002000  /*[13] : 1:Capture 0 use HQV,0:Capture 0 not use HQV*/
#define CAP0_USE_HW_FLIP        0x00004000  /*[14] : 1:Capture 0 use H/W flip,0:Capture 0 use S/W flip*/
#define CAP0_USE_CCIR656        0x00008000  /*[15] : 1:Capture 0 use CCIR656,0:Capture 0 CCIR601*/
#define CAP1_USE_V1             0x00000100  /*[ 8] : 1:Capture 1 use V1, 0:Capture 1 use V3*/
#define CAP1_USE_V3             0x00000000  /*[ 8] : 1:Capture 1 use V1, 0:Capture 1 use V3*/
#define CAP1_USE_HQV            0x00000200  /*[ 9] : 1:Capture 1 use HQV,0:Capture 1 not use HQV*/
#define CAP1_USE_HW_FLIP        0x00000400  /*[10] : 1:Capture 1 use H/W flip,0:Capture 1 use S/W flip  */
#define SW_USE_V1               0x00000010  /*[ 4] : 1:Capture 1 use V1, 0:Capture 1 use V3             */
#define SW_USE_V3               0x00000000  /*[ 4] : 1:Capture 1 use V1, 0:Capture 1 use V3             */
#define SW_USE_HQV              0x00000020  /*[ 5] : 1:Capture 1 use HQV,0:Capture 1 not use HQV        */

/*
#define VIDEO1_INUSE            0x00000010  //[ 4] : 1:Video 1 is used with S/W flip
#define VIDEO1_USE_HQV          0x00000020  //[ 5] : 1:Video 1 use HQV with S/W flip
#define VIDEO3_INUSE            0x00000001  //[ 0] : 1:Video 3 is used with S/W flip
#define VIDEO3_USE_HQV          0x00000002  //[ 1] : 1:Video 3 use HQV with S/W flip
*/

/* H/W registers for Video Engine */

/*
 *      bus master
 */
#define PCI_MASTER_ENABLE       0x01
#define PCI_MASTER_SCATTER      0x00
#define PCI_MASTER_SINGLE       0x02
#define PCI_MASTER_GUI          0x00
#define PCI_MASTER_VIDEO        0x04
#define PCI_MASTER_INPUT        0x00
#define PCI_MASTER_OUTPUT       0x08

/*
 *      video registers
 */
#define V_FLAGS			0x200
#define V_CAP_STATUS            0x204
#define V_FLIP_STATUS           0x204
#define V_ALPHA_WIN_START       0x208
#define V_ALPHA_WIN_END         0x20C
#define V_ALPHA_CONTROL         0x210
#define V_CRT_STARTADDR         0x214
#define V_CRT_STARTADDR_2       0x218
#define V_ALPHA_STRIDE          0x21C
#define V_COLOR_KEY             0x220
#define V_ALPHA_STARTADDR       0x224
#define V_CHROMAKEY_LOW         0x228
#define V_CHROMAKEY_HIGH        0x22C
#define V1_CONTROL              0x230
#define V12_QWORD_PER_LINE      0x234
#define V1_STARTADDR_1          0x238
#define V1_STARTADDR_Y1         V1_STARTADDR_1
#define V1_STRIDE               0x23C
#define V1_WIN_START_Y          0x240
#define V1_WIN_START_X          0x242
#define V1_WIN_END_Y            0x244
#define V1_WIN_END_X            0x246
#define V1_STARTADDR_2          0x248
#define V1_STARTADDR_Y2         V1_STARTADDR_2
#define V1_ZOOM_CONTROL         0x24C
#define V1_MINI_CONTROL         0x250
#define V1_STARTADDR_0          0x254
#define V1_STARTADDR_Y0         V1_STARTADDR_0
#define V_FIFO_CONTROL          0x258
#define V1_STARTADDR_3          0x25C
#define V1_STARTADDR_Y3         V1_STARTADDR_3
#define HI_CONTROL              0x260
#define SND_COLOR_KEY           0x264
#define ALPHA_V3_PREFIFO_CONTROL   0x268
#define V1_SOURCE_HEIGHT        0x26C
#define HI_TRANSPARENT_COLOR    0x270
#define V_DISPLAY_TEMP          0x274  /* No use */
#define ALPHA_V3_FIFO_CONTROL   0x278
#define V3_SOURCE_WIDTH         0x27C
#define V3_COLOR_KEY            0x280
#define V1_ColorSpaceReg_1      0x284
#define V1_ColorSpaceReg_2      0x288
#define V1_STARTADDR_CB0        0x28C
#define V1_OPAQUE_CONTROL       0x290  /* To be deleted */
#define V3_OPAQUE_CONTROL       0x294  /* To be deleted */
#define V_COMPOSE_MODE          0x298
#define V3_STARTADDR_2          0x29C
#define V3_CONTROL              0x2A0
#define V3_STARTADDR_0          0x2A4
#define V3_STARTADDR_1          0x2A8
#define V3_STRIDE               0x2AC
#define V3_WIN_START_Y          0x2B0
#define V3_WIN_START_X          0x2B2
#define V3_WIN_END_Y            0x2B4
#define V3_WIN_END_X            0x2B6
#define V3_ALPHA_QWORD_PER_LINE 0x2B8
#define V3_ZOOM_CONTROL         0x2BC
#define V3_MINI_CONTROL         0x2C0
#define V3_ColorSpaceReg_1      0x2C4
#define V3_ColorSpaceReg_2      0x2C8
#define V3_DISPLAY_TEMP         0x2CC  /* No use */
#define V1_STARTADDR_CB1        0x2E4
#define V1_STARTADDR_CB2        0x2E8
#define V1_STARTADDR_CB3        0x2EC
#define V1_STARTADDR_CR0        0x2F0
#define V1_STARTADDR_CR1        0x2F4
#define V1_STARTADDR_CR2        0x2F8
#define V1_STARTADDR_CR3        0x2FC
/*CN400 and older Hardware Icon engine register*/
#define HI_POSSTART		0x208
#define HI_CENTEROFFSET		0x20C
#define HI_FBOFFSET		0x224
#define HI_CONTROL		0x260
#define HI_TRANSPARENT_COLOR	0x270
#define HI_INVTCOLOR		0x274
/* VT3324 primary Hardware Icon engine register */
#define PRIM_HI_POSEND		0x290
#define V327_HI_INVTCOLOR	0x2E4
#define PRIM_HI_FIFO		0x2E8
#define PRIM_HI_TRANSCOLOR	0x2EC
#define PRIM_HI_CTRL		0x2F0
#define PRIM_HI_FBOFFSET	0x2F4
#define PRIM_HI_POSSTART	0x2F8
#define PRIM_HI_CENTEROFFSET	0x2FC
#define PRIM_HI_INVTCOLOR	0x120C

/* Video Capture Engine Registers
 * Capture Port 1
 */
#define CAP0_MASKS          0x100
#define CAP1_MASKS          0x104
#define CAP0_CONTROL        0x110
#define CAP0_H_RANGE        0x114
#define CAP0_V_RANGE        0x118
#define CAP0_SCAL_CONTROL   0x11C
#define CAP0_VBI_H_RANGE    0x120
#define CAP0_VBI_V_RANGE    0x124
#define CAP0_VBI_STARTADDR  0x128
#define CAP0_VBI_STRIDE     0x12C
#define CAP0_ANCIL_COUNT    0x130
#define CAP0_MAXCOUNT       0x134
#define CAP0_VBIMAX_COUNT   0x138
#define CAP0_DATA_COUNT     0x13C
#define CAP0_FB_STARTADDR0  0x140
#define CAP0_FB_STARTADDR1  0x144
#define CAP0_FB_STARTADDR2  0x148
#define CAP0_STRIDE         0x150
/* Capture Port 2 */
#define CAP1_CONTROL        0x154
#define CAP1_SCAL_CONTROL   0x160
#define CAP1_VBI_H_RANGE    0x164 /*To be deleted*/
#define CAP1_VBI_V_RANGE    0x168 /*To be deleted*/
#define CAP1_VBI_STARTADDR  0x16C /*To be deleted*/
#define CAP1_VBI_STRIDE     0x170 /*To be deleted*/
#define CAP1_ANCIL_COUNT    0x174 /*To be deleted*/
#define CAP1_MAXCOUNT       0x178
#define CAP1_VBIMAX_COUNT   0x17C /*To be deleted*/
#define CAP1_DATA_COUNT     0x180
#define CAP1_FB_STARTADDR0  0x184
#define CAP1_FB_STARTADDR1  0x188
#define CAP1_STRIDE         0x18C

/* SUBPICTURE Registers */
#define SUBP_CONTROL_STRIDE     0x3C0
#define SUBP_STARTADDR          0x3C4
#define RAM_TABLE_CONTROL       0x3C8
#define RAM_TABLE_READ          0x3CC

/* HQV Registers*/
#define HQV_CONTROL             0x3D0
#define HQV_SRC_STARTADDR_Y     0x3D4
#define HQV_SRC_STARTADDR_U     0x3D8
#define HQV_SRC_STARTADDR_V     0x3DC
#define HQV_SRC_FETCH_LINE      0x3E0
#define HQV_FILTER_CONTROL      0x3E4
#define HQV_MINIFY_CONTROL      0x3E8
#define HQV_DST_STARTADDR0      0x3EC
#define HQV_DST_STARTADDR1      0x3F0
#define HQV_DST_STARTADDR2      0x3FC
#define HQV_DST_STRIDE          0x3F4
#define HQV_SRC_STRIDE          0x3F8

#define HQV_H_SCALE_CONTROL     0x3B0
#define HQV_V_SCALE_CONTROL     0x3B4

#define PRO_HQV1_OFFSET         0x1000
/*
 *  Video command definition
 */
/* #define V_ALPHA_CONTROL         0x210 */
#define ALPHA_WIN_EXPIRENUMBER_4        0x00040000
#define ALPHA_WIN_CONSTANT_FACTOR_4     0x00004000
#define ALPHA_WIN_CONSTANT_FACTOR_12    0x0000c000
#define ALPHA_WIN_BLENDING_CONSTANT     0x00000000
#define ALPHA_WIN_BLENDING_ALPHA        0x00000001
#define ALPHA_WIN_BLENDING_GRAPHIC      0x00000002
#define ALPHA_WIN_PREFIFO_THRESHOLD_12  0x000c0000
#define ALPHA_WIN_FIFO_THRESHOLD_8      0x000c0000
#define ALPHA_WIN_FIFO_DEPTH_16         0x00100000

/* V_CHROMAKEY_LOW         0x228 */
#define V_CHROMAKEY_V3          0x80000000

/* V1_CONTROL                   0x230 */
#define V1_ENABLE               0x00000001
#define V1_FULL_SCREEN          0x00000002
#define V1_YUV422               0x00000000
#define V1_RGB32                0x00000004
#define V1_RGB15                0x00000008
#define V1_RGB16                0x0000000C
#define V1_YCbCr420             0x00000010
#define V1_COLORSPACE_SIGN      0x00000080
#define V1_SRC_IS_FIELD_PIC     0x00000200
#define V1_SRC_IS_FRAME_PIC     0x00000000
#define V1_BOB_ENABLE           0x00400000
#define V1_FIELD_BASE           0x00000000
#define V1_FRAME_BASE           0x01000000
#define V1_SWAP_SW              0x00000000
#define V1_SWAP_HW_HQV          0x02000000
#define V1_SWAP_HW_CAPTURE      0x04000000
#define V1_SWAP_HW_MC           0x06000000
/* #define V1_DOUBLE_BUFFERS       0x00000000 */
/* #define V1_QUADRUPLE_BUFFERS    0x18000000 */
#define V1_EXPIRE_NUM           0x00050000
#define V1_EXPIRE_NUM_A         0x000a0000
#define V1_EXPIRE_NUM_F         0x000f0000 /* jason */
#define V1_FIFO_EXTENDED        0x00200000
#define V1_ON_PRI               0x00000000
#define V1_ON_SND_DISPLAY       0x80000000
#define V1_FIFO_32V1_32V2       0x00000000
#define V1_FIFO_48V1_32V2       0x00200000
#define V1_PREFETCH_ON_3336     0x40000000  /*V1_PREFETCH_ON*/
#define V1_GAMMA_ENABLE_3336    0x20000000  /*V1_Gamma_ENABLE*/

/* V12_QWORD_PER_LINE           0x234 */
#define V1_FETCH_COUNT          0x3ff00000
#define V1_FETCHCOUNT_ALIGNMENT 0x0000000f
#define V1_FETCHCOUNT_UNIT      0x00000004   /* Doubld QWORD */

/* V1_STRIDE */
#define V1_STRIDE_YMASK         0x00001fff
#define V1_STRIDE_UVMASK        0x1ff00000

/* V1_ZOOM_CONTROL              0x24C */
#define V1_X_ZOOM_ENABLE        0x80000000
#define V1_Y_ZOOM_ENABLE        0x00008000

/* V1_MINI_CONTROL              0x250 */
#define V1_X_INTERPOLY          0x00000002  /* X interpolation */
#define V1_Y_INTERPOLY          0x00000001  /* Y interpolation */
#define V1_YCBCR_INTERPOLY      0x00000004  /* Y, Cb, Cr all interpolation */
#define V1_X_DIV_2              0x01000000
#define V1_X_DIV_4              0x03000000
#define V1_X_DIV_8              0x05000000
#define V1_X_DIV_16             0x07000000
#define V1_Y_DIV_2              0x00010000
#define V1_Y_DIV_4              0x00030000
#define V1_Y_DIV_8              0x00050000
#define V1_Y_DIV_16             0x00070000

/* V1_STARTADDR0               0x254 */
#define SW_FLIP_ODD             0x08000000

/* V_FIFO_CONTROL               0x258
 * IA2 has 32 level FIFO for packet mode video format
 *         32 level FIFO for planar mode video YV12. with extension reg 230 bit 21 enable
 *         16 level FIFO for planar mode video YV12. with extension reg 230 bit 21 disable
 * BCos of 128 bits. 1 level in IA2 = 2 level in VT3122
 */
#define V1_FIFO_DEPTH12         0x0000000B
#define V1_FIFO_DEPTH16         0x0000000F
#define V1_FIFO_DEPTH32         0x0000001F
#define V1_FIFO_DEPTH48         0x0000002F
#define V1_FIFO_DEPTH64         0x0000003F   
#define V1_FIFO_THRESHOLD6      0x00000600
#define V1_FIFO_THRESHOLD8      0x00000800
#define V1_FIFO_THRESHOLD12     0x00000C00
#define V1_FIFO_THRESHOLD16     0x00001000
#define V1_FIFO_THRESHOLD24     0x00001800
#define V1_FIFO_THRESHOLD32     0x00002000
#define V1_FIFO_THRESHOLD40     0x00002800  
#define V1_FIFO_THRESHOLD48     0x00003000   
#define V1_FIFO_THRESHOLD56     0x00003800  
#define V1_FIFO_THRESHOLD61     0x00003D00  
#define V1_FIFO_PRETHRESHOLD10  0x0A000000
#define V1_FIFO_PRETHRESHOLD12  0x0C000000
#define V1_FIFO_PRETHRESHOLD29  0x1d000000
#define V1_FIFO_PRETHRESHOLD40  0x28000000  
#define V1_FIFO_PRETHRESHOLD44  0x2c000000
#define V1_FIFO_PRETHRESHOLD56  0x38000000   
#define V1_FIFO_PRETHRESHOLD61  0x3D000000   

#define VIDEO_FIFO_DEPTH_VT3336             225  
#define VIDEO_FIFO_THRESHOLD_VT3336         200
#define VIDEO_FIFO_PRETHRESHOLD_VT3336      250
#define VIDEO_EXPIRE_NUM_VT3336             31

/* Those values are only valid for IGA1 */
#define VIDEO_FIFO_DEPTH_VT3409		400
#define VIDEO_FIFO_THRESHOLD_VT3409	320
#define VIDEO_FIFO_PRETHRESHOLD_VT3409	230
#define VIDEO_EXPIRE_NUM_VT3409		160

/* ALPHA_V3_FIFO_CONTROL        0x278
 * IA2 has 32 level FIFO for packet mode video format
 *         32 level FIFO for planar mode video YV12. with extension reg 230 bit 21 enable
 *         16 level FIFO for planar mode video YV12. with extension reg 230 bit 21 disable
 *          8 level FIFO for ALPHA
 * BCos of 128 bits. 1 level in IA2 = 2 level in VT3122
 */
#define V3_FIFO_DEPTH16         0x0000000F
#define V3_FIFO_DEPTH24         0x00000017
#define V3_FIFO_DEPTH32         0x0000001F
#define V3_FIFO_DEPTH48         0x0000002F
#define V3_FIFO_DEPTH64         0x0000003F   
#define V3_FIFO_THRESHOLD8      0x00000800
#define V3_FIFO_THRESHOLD12     0x00000C00
#define V3_FIFO_THRESHOLD16     0x00001000
#define V3_FIFO_THRESHOLD24     0x00001800
#define V3_FIFO_THRESHOLD29     0x00001D00
#define V3_FIFO_THRESHOLD32     0x00002000
#define V3_FIFO_THRESHOLD40     0x00002800  
#define V3_FIFO_THRESHOLD48     0x00003000   
#define V3_FIFO_THRESHOLD56     0x00003800   
#define V3_FIFO_THRESHOLD61     0x00003D00   
#define V3_FIFO_PRETHRESHOLD10  0x0000000A
#define V3_FIFO_PRETHRESHOLD12  0x0000000C
#define V3_FIFO_PRETHRESHOLD29  0x0000001d
#define V3_FIFO_PRETHRESHOLD40  0x00000028  
#define V3_FIFO_PRETHRESHOLD44  0x0000002c
#define V3_FIFO_PRETHRESHOLD56  0x00000038   
#define V3_FIFO_PRETHRESHOLD61  0x0000003D   
#define V3_FIFO_MASK            0x0000007F
#define V3_FIFO_MASK_3314       0x000000FF
#define ALPHA_FIFO_DEPTH8       0x00070000
#define ALPHA_FIFO_THRESHOLD4   0x04000000
#define ALPHA_FIFO_MASK         0xffff0000
#define ALPHA_FIFO_PRETHRESHOLD4 0x00040000

/* IA2 */
#define ColorSpaceValue_1       0x140020f2
#define ColorSpaceValue_2       0x0a0a2c00

#define ColorSpaceValue_1_3123C0      0x13000DED
#define ColorSpaceValue_2_3123C0      0x13171000
#define ColorSpaceValue_1_32045       0x13000DED
#define ColorSpaceValue_2_32045       0x13171000

/* For TV setting */
#define ColorSpaceValue_1TV     0x140020f2
#define ColorSpaceValue_2TV     0x0a0a2c00

/* V_COMPOSE_MODE               0x298 */
#define SELECT_VIDEO_IF_COLOR_KEY               0x00000001  /* select video if (color key),otherwise select graphics */
#define SELECT_VIDEO3_IF_COLOR_KEY              0x00000020  /* For 3123C0, select video3 if (color key),otherwise select graphics */
#define SELECT_VIDEO_IF_CHROMA_KEY              0x00000002  /* 0x0000000a  //select video if (chroma key ),otherwise select graphics */
#define ALWAYS_SELECT_VIDEO                     0x00000000  /* always select video,Chroma key and Color key disable */
#define COMPOSE_V1_V3           0x00000000  /* V1 on top of V3 */
#define COMPOSE_V3_V1           0x00100000  /* V3 on top of V1 */
#define COMPOSE_V1_TOP          0x00000000
#define COMPOSE_V3_TOP          0x00100000
#define V1_COMMAND_FIRE         0x80000000  /* V1 commands fire */
#define V3_COMMAND_FIRE         0x40000000  /* V3 commands fire */
#define V_COMMAND_LOAD          0x20000000  /* Video register always loaded */
#define V_COMMAND_LOAD_VBI      0x10000000  /* Video register always loaded at vbi without waiting source flip */
#define V3_COMMAND_LOAD         0x08000000  /* CLE_C0 Video3 register always loaded */
#define V3_COMMAND_LOAD_VBI     0x00000100  /* CLE_C0 Video3 register always loaded at vbi without waiting source flip */
#define SECOND_DISPLAY_COLOR_KEY_ENABLE         0x00010000

/* V3_ZOOM_CONTROL              0x2bc */
#define V3_X_ZOOM_ENABLE        0x80000000
#define V3_Y_ZOOM_ENABLE        0x00008000

/* V3_MINI_CONTROL              0x2c0 */
#define V3_X_INTERPOLY          0x00000002  /* X interpolation */
#define V3_Y_INTERPOLY          0x00000001  /* Y interpolation */
#define V3_YCBCR_INTERPOLY      0x00000004  /* Y, Cb, Cr all interpolation */
#define V3_X_DIV_2              0x01000000
#define V3_X_DIV_4              0x03000000
#define V3_X_DIV_8              0x05000000
#define V3_X_DIV_16             0x07000000
#define V3_Y_DIV_2              0x00010000
#define V3_Y_DIV_4              0x00030000
#define V3_Y_DIV_8              0x00050000
#define V3_Y_DIV_16             0x00070000

/* SUBP_CONTROL_STRIDE              0x3c0 */
#define SUBP_HQV_ENABLE             0x00010000
#define SUBP_IA44                   0x00020000
#define SUBP_AI44                   0x00000000
#define SUBP_STRIDE_MASK            0x00001fff
#define SUBP_CONTROL_MASK           0x00070000

/* RAM_TABLE_CONTROL                0x3c8 */
#define RAM_TABLE_RGB_ENABLE        0x00000007

/* CAPTURE0_CONTROL                  0x310 */
#define C0_ENABLE           		0x00000001
#define BUFFER_2_MODE       		0x00000000
#define BUFFER_3_MODE       		0x00000004
#define BUFFER_4_MODE       		0x00000006
#define SWAP_YUYV           		0x00000000 
#define SWAP_UYVY           		0x00000100   
#define SWAP_YVYU           		0x00000200
#define SWAP_VYUY           		0x00000300
#define IN_601_8            		0x00000000
#define IN_656_8            		0x00000010
#define IN_601_16           		0x00000020
#define IN_656_16           		0x00000030
#define DEINTER_ODD         		0x00000000
#define DEINTER_EVEN        		0x00001000   
#define DEINTER_ODD_EVEN    		0x00002000
#define DEINTER_FRAME       		0x00003000
#define VIP_1               		0x00000000 
#define VIP_2               		0x00000400
#define H_FILTER_2          		0x00010000
#define H_FILTER_4          		0x00020000 
#define H_FILTER_8_1331     		0x00030000 
#define H_FILTER_8_12221    		0x00040000
#define VIP_ENABLE          		0x00000008
#define EN_FIELD_SIG        		0x00000800  
#define VREF_INVERT         		0x00100000
#define FIELD_INPUT_INVERSE    		0x00400000
#define FIELD_INVERSE       		0x40000000

#define C1_H_MINI_EN        		0x00000800
#define C0_H_MINI_EN        		0x00000800
#define C1_V_MINI_EN        		0x04000000
#define C0_V_MINI_EN        		0x04000000
#define C1_H_MINI_2         		0x00000400

/* CAPTURE1_CONTROL                  0x354 */
#define C1_ENABLE           		0x00000001

/* V3_CONTROL                   0x2A0 */
#define V3_ENABLE               0x00000001
#define V3_FULL_SCREEN          0x00000002
#define V3_YUV422               0x00000000
#define V3_RGB32                0x00000004
#define V3_RGB15                0x00000008
#define V3_RGB16                0x0000000C
#define V3_COLORSPACE_SIGN      0x00000080
#define V3_EXPIRE_NUM           0x00040000
#define V3_EXPIRE_NUM_F         0x000f0000
#define V3_EXPIRE_NUM_3204      0x00100000
#define V3_EXPIRE_NUM_3205      0x00080000 
#define V3_BOB_ENABLE           0x00400000
#define V3_FIELD_BASE           0x00000000
#define V3_FRAME_BASE           0x01000000
#define V3_SWAP_SW              0x00000000
#define V3_SWAP_HW_HQV          0x02000000
#define V3_FLIP_HW_CAPTURE0     0x04000000
#define V3_FLIP_HW_CAPTURE1     0x06000000

/* V3_ALPHA_FETCH_COUNT           0x2B8 */
#define V3_FETCH_COUNT          0x3ff00000
#define ALPHA_FETCH_COUNT       0x000003ff

/* HQV_CONTROL             0x3D0 */
#define HQV_RGB32           0x00000000
#define HQV_RGB16           0x20000000
#define HQV_RGB15           0x30000000
#define HQV_YUV422          0x80000000
#define HQV_YUV420          0xC0000000
#define HQV_ENABLE          0x08000000
#define HQV_SRC_SW          0x00000000
#define HQV_SRC_MC          0x01000000
#define HQV_SRC_CAPTURE0    0x02000000
#define HQV_SRC_CAPTURE1    0x03000000
#define HQV_FLIP_EVEN       0x00000000
#define HQV_FLIP_ODD        0x00000020
#define HQV_SW_FLIP         0x00000010   /* Write 1 to flip HQV buffer */
#define HQV_DEINTERLACE     0x00010000   /* First line of odd field will be repeated 3 times */
#define HQV_FIELD_2_FRAME   0x00020000   /* Src is field. Display each line 2 times */
#define HQV_FRAME_2_FIELD   0x00040000   /* Src is field. Display field */
#define HQV_FRAME_UV        0x00000000   /* Src is Non-interleaved */
#define HQV_FIELD_UV        0x00100000   /* Src is interleaved */
#define HQV_IDLE            0x00000008   
#define HQV_FLIP_STATUS     0x00000001   
#define HQV_DOUBLE_BUFF     0x00000000
#define HQV_TRIPLE_BUFF     0x04000000
#define HQV_SUBPIC_FLIP     0x00008000
#define HQV_FIFO_STATUS     0x00001000  
#define HQV_GEN_IRQ         0x00000080
#define HQV_FIFO_DEPTH_1    0x00010000
/* for CME engine */
#define HQV_SW_FLIP_QUEUE_ENABLE    0x00100000

/* for hwDiff->dwNewScaleCtl */
#define HQV_H_SCALE_ENABLE                 0x80000000
#define HQV_H_SCALE_UP                     0x00000000  
#define HQV_H_SCALE_DOWN_FOURTH_TO_1       0x10000000 
#define HQV_H_SCALE_DOWN_FOURTH_TO_EIGHTH  0x20000000 
#define HQV_H_SCALE_DOWN_UNDER_EIGHTH      0x30000000 

#define HQV_V_SCALE_ENABLE 0x80000000
#define HQV_V_SCALE_UP     0x00000000
#define HQV_V_SCALE_DOWN   0x10000000

/* HQV Default Vodeo Color 0x3B8 */
#define HQV_FIX_COLOR           0x0643212c

/* HQV_FILTER_CONTROL      0x3E4 */
#define HQV_H_LOWPASS_2TAP  0x00000001
#define HQV_H_LOWPASS_4TAP  0x00000002
#define HQV_H_LOWPASS_8TAP1 0x00000003   /* To be deleted */
#define HQV_H_LOWPASS_8TAP2 0x00000004   /* To be deleted */
#define HQV_H_HIGH_PASS     0x00000008
#define HQV_H_LOW_PASS      0x00000000
#define HQV_V_LOWPASS_2TAP  0x00010000
#define HQV_V_LOWPASS_4TAP  0x00020000
#define HQV_V_LOWPASS_8TAP1 0x00030000
#define HQV_V_LOWPASS_8TAP2 0x00040000
#define HQV_V_HIGH_PASS     0x00080000
#define HQV_V_LOW_PASS      0x00000000
#define HQV_H_HIPASS_F1_DEFAULT 0x00000040
#define HQV_H_HIPASS_F2_DEFAULT 0x00000000
#define HQV_V_HIPASS_F1_DEFAULT 0x00400000
#define HQV_V_HIPASS_F2_DEFAULT 0x00000000
#define HQV_H_HIPASS_F1_2TAP    0x00000050
#define HQV_H_HIPASS_F2_2TAP    0x00000100
#define HQV_V_HIPASS_F1_2TAP    0x00500000
#define HQV_V_HIPASS_F2_2TAP    0x01000000
#define HQV_H_HIPASS_F1_4TAP    0x00000060
#define HQV_H_HIPASS_F2_4TAP    0x00000200
#define HQV_V_HIPASS_F1_4TAP    0x00600000
#define HQV_V_HIPASS_F2_4TAP    0x02000000
#define HQV_H_HIPASS_F1_8TAP    0x00000080
#define HQV_H_HIPASS_F2_8TAP    0x00000400
#define HQV_V_HIPASS_F1_8TAP    0x00800000
#define HQV_V_HIPASS_F2_8TAP    0x04000000
/* IA2 NEW */
#define HQV_V_FILTER2           0x00080000
#define HQV_H_FILTER2           0x00000008
#define HQV_H_TAP2_11           0x00000041
#define HQV_H_TAP4_121          0x00000042
#define HQV_H_TAP4_1111         0x00000401
#define HQV_H_TAP8_1331         0x00000221
#define HQV_H_TAP8_12221        0x00000402
#define HQV_H_TAP16_1991        0x00000159
#define HQV_H_TAP16_141041      0x0000026A
#define HQV_H_TAP32             0x0000015A
#define HQV_V_TAP2_11           0x00410000
#define HQV_V_TAP4_121          0x00420000
#define HQV_V_TAP4_1111         0x04010000
#define HQV_V_TAP8_1331         0x02210000
#define HQV_V_TAP8_12221        0x04020000
#define HQV_V_TAP16_1991        0x01590000
#define HQV_V_TAP16_141041      0x026A0000
#define HQV_V_TAP32             0x015A0000
#define HQV_V_FILTER_DEFAULT    0x00420000
#define HQV_H_FILTER_DEFAULT    0x00000040

/* HQV_MINI_CONTROL        0x3E8 */
#define HQV_H_MINIFY_ENABLE 0x00000800
#define HQV_H_MINIFY_DOWN   0x00001000
#define HQV_V_MINIFY_ENABLE 0x08000000
#define HQV_V_MINIFY_DOWN   0x10000000
#define HQV_VDEBLOCK_FILTER 0x80000000
#define HQV_HDEBLOCK_FILTER 0x00008000

/* new added registers for VT3409.For some registers have different meanings
 * but the same address,we add postfix _409 to distinguish */
#define HQV_COLOR_ADJUSTMENT_PRE_CTRL1              0x360
#define HQV_COLOR_ADJUSTMENT_PRE_CTRL2              0x364
#define HQV_COLOR_ADJUSTMENT_PRE_CTRL3              0x368
#define HQV_COLOR_ADJUSTMENT_PRE_CTRL4              0x36C
#define HQV_SRC_DATA_OFFSET_CTRL1_409               0x370
#define HQV_SRC_DATA_OFFSET_CTRL2_409               0x374
#define HQV_SRC_DATA_OFFSET_CTRL3_409               0x378
#define HQV_SRC_DATA_OFFSET_CTRL4_409               0x37C
#define HQV_DST_DATA_OFFSET_CTRL1                   0x380
#define HQV_DST_DATA_OFFSET_CTRL2                   0x384
#define HQV_DST_DATA_OFFSET_CTRL3                   0x388
#define HQV_DST_DATA_OFFSET_CTRL4                   0x38C
#define HQV_SHARPNESS_DECODER_HANDSHAKE_CTRL_410    0x3A4
#define HQV_RESIDUE_PIXEL_FRAME_STARTADDR           0x3BC
#define HQV_BACKGROUND_DATA_OFFSET                  0x3CC
#define HQV_SUBP_HSCALE_CTRL                        0x3E0
#define HQV_SUBP_VSCALE_CTRL                        0x3E8

/* Add new HQV Registers for VT3353: */
#define HQV_SRC_DATA_OFFSET_CONTROL1        0x380
#define HQV_SRC_DATA_OFFSET_CONTROL2        0x384
#define HQV_SRC_DATA_OFFSET_CONTROL3        0x388
#define HQV_SRC_DATA_OFFSET_CONTROL4        0x38C
#define HQV_HW_TUNING_PERFORMANCE           0x390
#define HQV_EXTENDED_CONTROL                0x394
#define HQV_STATIC_RECORD_FB_STARTADDR      0x398
#define HQV_STATIC_RECORD_FB_STRIDE         0x39C
#define HQV_COLOR_ADJUSTMENT_CONTROL1       0x3A0
#define HQV_COLOR_ADJUSTMENT_CONTROL2       0x3A4
#define HQV_COLOR_ADJUSTMENT_CONTROL3       0x3A8
#define HQV_COLOR_ADJUSTMENT_CONTROL5       0x3AC
#define HQV_DEFAULT_VIDEO_COLOR             0x3B8

#define CHROMA_KEY_LOW          0x00FFFFFF
#define CHROMA_KEY_HIGH         0x00FFFFFF

/* V_CAP_STATUS */
#define V_ST_UPDATE_NOT_YET     0x00000003
#define V1_ST_UPDATE_NOT_YET    0x00000001
#define V3_ST_UPDATE_NOT_YET    0x00000008

#define VBI_STATUS              0x00000002

/*
 *      Macros for Video MMIO
 */
#ifndef V4L2
#define MPGOutD(port, data)     *((volatile CARD32 *)(pVia->MpegMapBase +(port))) = (data)
#define MPGInD(port)            *((volatile CARD32 *)(pVia->MpegMapBase +(port)))
#endif

/*
 *      Macros for GE MMIO
 */
#define GEInW(port)             *((volatile CARD16 *)(lpGEMMIO + (port)))
#define GEInD(port)             *((volatile CARD32 *)(lpGEMMIO + (port)))
#define GEOutW(port, data)      *((volatile CARD16 *)(lpGEMMIO + (port))) = (data)
#define GEOutD(port, data)      *((volatile CARD32 *)(lpGEMMIO + (port))) = (data)

/*
 *	MPEG 1/2 Slice Engine (at 0xC00 relative to base)
 */

#define MPG_CONTROL		0x00
#define MPG_CONTROL_STRUCT	0x03
#define			MPG_CONTROL_STRUCT_TOP		0x01
#define			MPG_CONTROL_STRUCT_BOTTOM	0x02
#define			MPG_CONTROL_STRUCT_FRAME	0x03
		/* Use TOP if interlaced */
#define		MPG_CONTROL_TYPE	0x3C
#define			MPG_CONTROL_TYPE_I	(0x01 << 2)
#define			MPG_CONTROL_TYPE_B	(0x02 << 2)
#define			MPG_CONTROL_TYPE_P	(0x03 << 3)
#define		MPG_CONTROL_ALTSCAN	0x40
#define MPG_BLOCK		0x08		/* Unsure */
#define MPG_COMMAND		0x0C
#define MPG_DATA1		0x10
#define MPG_DATA2		0x14
#define MPG_DATA3		0x18
#define MPG_DATA4		0x1C

#define MPG_YPHYSICAL(x)	(0x20 + 12*(x))
#define MPG_CbPHYSICAL(x)	(0x24 + 12*(x))
#define MPG_CrPHYSICAL(x)	(0x28 + 12*(x))

#define MPG_PITCH		0x50
#define MPG_STATUS		0x54

#define MPG_MATRIX_IDX		0x5C
#define		MPG_MATRIX_IDX_INTRA	0x00
#define		MPG_MATRIX_IDX_NON	0x01
#define MPG_MATRIX_DATA		0x60

#define MPG_SLICE_CTRL_1	0x90
#define	MPG_SLICE_MBAMAX		0x2FFF
#define	MPG_SLICE_PREDICTIVE_DCT	0x4000
#define	MPG_SLICE_TOP_FIRST		0x8000
#define	MPG_SLICE_MACROBLOCK_WIDTH(x)	((x)<<18)	/* in 64's */
#define	MPG_SLICE_CTRL_2	0x94
#define	MPG_SLICE_CONCEAL_MVEC		0x0000001
#define	MPG_SLICE_QSCALE_TYPE		0x0000002
#define	MPG_SLICE_DCPRECISION		0x000000C
#define	MPG_SLICE_MACROBQUOT		0x0FFFFF0
#define	MPG_SLICE_INTRAVLC		0x1000000
#define	MPG_SLICE_CTRL_3	0x98
#define	MPG_SLICE_FHMVR			0x0000003
#define	MPG_SLICE_FVMVR			0x000000C
#define	MPG_SLICE_BHMVR			0x0000030
#define	MPG_SLICE_BVMVR			0x00000C0
#define	MPG_SLICE_SECOND_FIELD		0x0100000
#define	MPG_SLICE_RESET			0x0400000
#define MPG_SLICE_LENGTH	0x9C
#define	MPG_SLICE_DATA		0xA0

#endif /* _VIA_ENG_REGS_H_ */
