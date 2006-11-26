/* $XFree86$ */
/* $XdotOrg: driver/xf86-video-sisusb/src/sisusb_accel.h,v 1.4 2005/07/09 02:50:34 twini Exp $ */
/*
 * 2D Acceleration for SiS 315/USB
 * Definitions for the SIS engine communication.
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

/* Engine commands */
#define BITBLT                  0x00000000  /* Blit */
#define COLOREXP                0x00000001  /* Color expand */
#define ENCOLOREXP              0x00000002  /* Enhanced color expand (315 only?) */
#define MULTIPLE_SCANLINE       0x00000003  /* 315 only, not 330 */
#define LINE                    0x00000004  /* Draw line */
#define TRAPAZOID_FILL          0x00000005  /* Fill trapezoid */
#define TRANSPARENT_BITBLT      0x00000006  /* Transparent Blit */
#define ALPHA_BLEND		0x00000007  /* Alpha blended BitBlt */
#define A3D_FUNCTION		0x00000008  /* 3D command ? */
#define	CLEAR_Z_BUFFER		0x00000009  /* ? */
#define GRADIENT_FILL		0x0000000A  /* Gradient fill */
#define STRETCH_BITBLT		0x0000000B  /* Stretched BitBlit */

#define YUVRGB_BLIT_325		0x0000000C
#define YUVRGB_BLIT_330		0x00000003

/* Command bits */

/* Source selection */
#define SRCVIDEO                0x00000000  /* source is video RAM */
#define SRCSYSTEM               0x00000010  /* source is system memory */
#define SRCCPUBLITBUF           SRCSYSTEM   /* source is CPU-driven BitBuffer (for color expand) */
#define SRCAGP                  0x00000020  /* source is AGP memory (?) */

/* Pattern source selection */
#define PATFG                   0x00000000  /* foreground color */
#define PATPATREG               0x00000040  /* pattern in pattern buffer (0x8300) */
#define PATMONO                 0x00000080  /* mono pattern */

/* Clipping flags */
#define NOCLIP                  0x00000000
#define NOMERGECLIP             0x04000000
#define CLIPENABLE              0x00040000
#define CLIPWITHOUTMERGE        0x04040000

/* Subfunctions for BitBlt: Transparency */
#define OPAQUE                  0x00000000
#define TRANSPARENT             0x00100000

/* Subfunctions for Alpha Blended BitBlt */
#define A_CONSTANTALPHA         0x00000000
#define A_PERPIXELALPHA		0x00080000
#define A_NODESTALPHA		0x00100000
#define A_3DFULLSCENE		0x00180000

/* Destination */
#define DSTAGP                  0x02000000
#define DSTVIDEO                0x00000000

/* Subfunctions for Color/Enhanced Color Expansion */
#define COLOR_TO_MONO		0x00100000
#define AA_TEXT			0x00200000

/* Line */
#define LINE_STYLE              0x00800000
#define NO_RESET_COUNTER        0x00400000
#define NO_LAST_PIXEL           0x00200000

/* Trapezoid (315 only?) */
#define T_XISMAJORL             0x00800000  /* X axis is driving axis (left) */
#define T_XISMAJORR             0x08000000  /* X axis is driving axis (right) */
#define T_L_Y_INC               0x00000020  /* left edge direction Y */
#define T_L_X_INC               0x00000010  /* left edge direction X */
#define T_R_Y_INC               0x00400000  /* right edge direction Y */
#define T_R_X_INC               0x00200000  /* right edge direction X */

/* YUV to RGB blit */
#define YUV_FORMAT_YUY2		0x00000000
#define YUV_FORMAT_YVYU		0x00002000
#define YUV_FORMAT_UYVY		0x00004000
#define YUV_FORMAT_VYUY		0x00006000
#define YUV_FORMAT_NV12		0x00008000  /* Only supported one */
#define YUV_FORMAT_NV21		0x0000A000

#define YUV_CMD_YUV		0x00800000

/* Scanline trigger (315 only, not 330) */
#define SCANLINE_TR_CRT1        0x00000000
#define SCANLINE_TR_CRT2        0x01000000
#define SCANLINE_TRIGGER_ENABLE 0x80000000

/* Some general registers */
#define SRC_ADDR		0x8200
#define SRC_PITCH		0x8204
#define AGP_BASE		0x8206 /* color-depth dependent value */
#define SRC_Y			0x8208
#define SRC_X			0x820A
#define DST_Y			0x820C
#define DST_X			0x820E
#define DST_ADDR		0x8210
#define DST_PITCH		0x8214
#define DST_HEIGHT		0x8216
#define RECT_WIDTH		0x8218
#define RECT_HEIGHT		0x821A
#define PAT_FGCOLOR		0x821C
#define PAT_BGCOLOR		0x8220
#define SRC_FGCOLOR		0x8224
#define SRC_BGCOLOR		0x8228
#define MONO_MASK		0x822C
#define LEFT_CLIP		0x8234
#define TOP_CLIP		0x8236
#define RIGHT_CLIP		0x8238
#define BOTTOM_CLIP		0x823A
#define COMMAND_READY		0x823C
#define FIRE_TRIGGER      	0x8240

#define PATTERN_REG		0x8300  /* 384 bytes pattern buffer */

/* Line registers */
#define LINE_X0			SRC_Y
#define LINE_X1			DST_Y
#define LINE_Y0			SRC_X
#define LINE_Y1			DST_X
#define LINE_COUNT		RECT_WIDTH
#define LINE_STYLE_PERIOD	RECT_HEIGHT
#define LINE_STYLE_0		MONO_MASK
#define LINE_STYLE_1		0x8230
#define LINE_XN			PATTERN_REG
#define LINE_YN			PATTERN_REG+2

/* Transparent bitblit registers */
#define TRANS_DST_KEY_HIGH	PAT_FGCOLOR
#define TRANS_DST_KEY_LOW	PAT_BGCOLOR
#define TRANS_SRC_KEY_HIGH	SRC_FGCOLOR
#define TRANS_SRC_KEY_LOW	SRC_BGCOLOR

#define ALPHA_ALPHA		PAT_FGCOLOR

/* Trapezoid registers */
#define TRAP_YH                 SRC_Y    /* 0x8208 */
#define TRAP_LR                 DST_Y    /* 0x820C */
#define TRAP_DL                 0x8244
#define TRAP_DR                 0x8248
#define TRAP_EL                 0x824C
#define TRAP_ER                 0x8250

/* Queue */
#define Q_BASE_ADDR		0x85C0  /* Base address of software queue */
#define Q_WRITE_PTR		0x85C4  /* Current write pointer */
#define Q_READ_PTR		0x85C8  /* Current read pointer */
#define Q_STATUS		0x85CC  /* queue status */

/* VRAM queue operation command header definitions */
#define SISUSB_SPKC_HEADER 	0x16800000L
#define SISUSB_BURST_HEADER0	0x568A0000L
#define SISUSB_BURST_HEADER1	0x62100000L
#define SISUSB_PACKET_HEARER0 	0x968A0000L
#define SISUSB_PACKET_HEADER1	0x62100000L
#define SISUSB_NIL_CMD		0x168F0000L

#define SISUSB_PACKET12_HEADER0   	0x968A000CL
#define SISUSB_PACKET12_HEADER1 	0x62100010L
#define SISUSB_PACKET12_LENGTH	80

/* Macros to do useful things with the SiS315/330 BitBLT engine */

/* Q_STATUS:
   bit 31 = 1: All engines idle and all queues empty
   bit 30 = 1: Hardware Queue (=HW CQ, 2D queue, 3D queue) empty
   bit 29 = 1: 2D engine is idle
   bit 28 = 1: 3D engine is idle
   bit 27 = 1: HW command queue empty
   bit 26 = 1: 2D queue empty
   bit 25 = 1: 3D queue empty
   bit 24 = 1: SW command queue empty
   bits 23:16: 2D counter 3
   bits 15:8:  2D counter 2
   bits 7:0:   2D counter 1
*/

/* As sis_dri.c and dual head mode relocate the cmd-q len to the sarea/entity,
 * don't use it directly here */
#define CmdQueLen (*(pSiSUSB->cmdQueueLenPtr))

#define SiSUSBQEmpty \
  { \
     int watchdog = 300; \
     while( (((SIS_MMIO_IN16(pSiSUSB->IOBase, Q_STATUS+2)) & 0x0400) != 0x0400) && --watchdog ) {}; \
     watchdog = 300; \
     while( (((SIS_MMIO_IN16(pSiSUSB->IOBase, Q_STATUS+2)) & 0x0400) != 0x0400) && --watchdog )  {}; \
  }

#define SiSUSBResetCmd   	   pSiSUSB->CommandReg = 0;

#define SiSUSBSetupCMDFlag(flags)  pSiSUSB->CommandReg |= (flags);

/* --- VRAM mode --- */

#define SiSUSBGetSwWP() (CARD32)(*(pSiSUSB->cmdQ_SharedWritePort))
#define SiSUSBGetHwRP() (CARD32)(SIS_MMIO_IN32(pSiSUSB->IOBase, Q_READ_PTR))

#define SiSUSBFlushCmdBuf

#define SiSUSBSyncWP    \
  SiSUSBFlushCmdBuf;    \
  SIS_MMIO_OUT32(pSiSUSB->IOBase, Q_WRITE_PTR, (CARD32)(*(pSiSUSB->cmdQ_SharedWritePort)));

#define SiSUSBSetHwWP(p) \
  *(pSiSUSB->cmdQ_SharedWritePort) = (p);   	\
  SIS_MMIO_OUT32(pSiSUSB->IOBase, Q_WRITE_PTR, (p));

#define SiSUSBSetSwWP(p) *(pSiSUSB->cmdQ_SharedWritePort) = (p);

#define SiSUSBCheckQueue(amount)

#if 0
      { \
	CARD32 mcurrent, i=0, ttt = SiSUSBGetSwWP(); \
	if((ttt + amount) >= pSiSUSB->cmdQueueSize) { \
	   do { \
	      mcurrent = SIS_MMIO_IN32(pSiSUSB->IOBase, Q_READ_PTR); \
	      i++; \
	   } while((mcurrent > ttt) || (mcurrent < ((ttt + amount) & pSiSUSB->cmdQueueSizeMask))); \
	} else { \
	   do { \
	      mcurrent = SIS_MMIO_IN32(pSiSUSB->IOBase, Q_READ_PTR); \
	      i++; \
	   } while((mcurrent > ttt) && (mcurrent < (ttt + amount))); \
	} \
      }
#endif

#define SiSUSBUpdateQueue \
      SiSUSBWriteQueue(tt); \
      ttt += 16; \
      ttt &= pSiSUSB->cmdQueueSizeMask; \
      if(!ttt) { \
         while(SIS_MMIO_IN32(pSiSUSB->IOBase, Q_READ_PTR) < pSiSUSB->cmdQueueSize_div4) {} \
      } else if(ttt == pSiSUSB->cmdQueueSize_div4) { \
         CARD32 temppp; \
	 do { \
	    temppp = SIS_MMIO_IN32(pSiSUSB->IOBase, Q_READ_PTR); \
	 } while(temppp >= ttt && temppp <= pSiSUSB->cmdQueueSize_div2); \
      } else if(ttt == pSiSUSB->cmdQueueSize_div2) { \
         CARD32 temppp; \
	 do { \
	    temppp = SIS_MMIO_IN32(pSiSUSB->IOBase, Q_READ_PTR); \
	 } while(temppp >= ttt && temppp <= pSiSUSB->cmdQueueSize_4_3); \
      } else if(ttt == pSiSUSB->cmdQueueSize_4_3) { \
         while(SIS_MMIO_IN32(pSiSUSB->IOBase, Q_READ_PTR) > ttt) {} \
      }

/* Write-updates MUST be 128bit aligned. */
#define SiSUSBNILandUpdateSWQueue \
      SIS_WQINDEX(2) = (CARD32)(SISUSB_NIL_CMD); \
      SIS_WQINDEX(3) = (CARD32)(SISUSB_NIL_CMD); \
      SiSUSBUpdateQueue; \
      SiSUSBSetSwWP(ttt);

#ifdef SISVRAMQ

#define SiSUSBIdle \
  { \
     int watchdog = 500; \
     while( ((SIS_MMIO_IN16(pSiSUSB->IOBase, Q_STATUS+2) & 0x8000) != 0x8000) && --watchdog ) {}; \
     watchdog = 500; \
     while( ((SIS_MMIO_IN16(pSiSUSB->IOBase, Q_STATUS+2) & 0x8000) != 0x8000) && --watchdog ) {}; \
  }

#define SiSUSBSetupSRCDSTBase(srcbase,dstbase) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + SRC_ADDR); \
         SIS_WQINDEX(1) = (CARD32)(srcbase); 			\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + DST_ADDR); \
         SIS_WQINDEX(3) = (CARD32)(dstbase); 			\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupSRCDSTXY(sx,sy,dx,dy) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + SRC_Y); 	\
         SIS_WQINDEX(1) = (CARD32)(((sx)<<16) | (sy));		\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + DST_Y); 	\
         SIS_WQINDEX(3) = (CARD32)(((dx)<<16) | (dy)); 		\
	 SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupDSTXYRect(x,y,w,h) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + DST_Y); 		\
         SIS_WQINDEX(1) = (CARD32)(((x)<<16) | (y));	 		\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + RECT_WIDTH); 	\
         SIS_WQINDEX(3) = (CARD32)(((h)<<16) | (w));			\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupSRCPitchDSTRect(pitch,x,y) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + SRC_PITCH); 	\
         SIS_WQINDEX(1) = (CARD32)(pitch);				\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + DST_PITCH); 	\
         SIS_WQINDEX(3) = (CARD32)(((y)<<16) | (x));			\
	 SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupSRCBase(base) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + SRC_ADDR); 	\
         SIS_WQINDEX(1) = (CARD32)(base); 				\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupSRCPitch(pitch) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + SRC_PITCH); 	\
         SIS_WQINDEX(1) = (CARD32)(pitch);				\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupSRCXY(x,y) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + SRC_Y); 	\
         SIS_WQINDEX(1) = (CARD32)(((x)<<16) | (y));		\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupDSTBase(base) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + DST_ADDR); 	\
         SIS_WQINDEX(1) = (CARD32)(base);				\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupDSTXY(x,y) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + DST_Y); 	\
         SIS_WQINDEX(1) = (CARD32)(((x)<<16) | (y));	 	\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupDSTRect(x,y) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + DST_PITCH); 	\
         SIS_WQINDEX(1) = (CARD32)(((y)<<16) | (x));		\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupDSTRectBurstHeader(x,y,reg,num) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + DST_PITCH);	\
         SIS_WQINDEX(1) = (CARD32)(((y)<<16) | (x));			\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_BURST_HEADER0 + reg); 		\
	 SIS_WQINDEX(3) = (CARD32)(SISUSB_BURST_HEADER1 + num); 		\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupDSTColorDepth(bpp) \
      pSiSUSB->CommandReg = (((CARD32)(bpp)) & (GENMASK(17:16)));

#define SiSUSBSetupPATFGDSTRect(color,x,y) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + PAT_FGCOLOR); 	\
         SIS_WQINDEX(1) = (CARD32)(color);	 			\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + DST_PITCH); 	\
         SIS_WQINDEX(3) = (CARD32)(((y)<<16) | (x));			\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupSRCFGDSTRect(color,x,y) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + SRC_FGCOLOR); 	\
         SIS_WQINDEX(1) = (CARD32)(color);	 			\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + DST_PITCH); 	\
         SIS_WQINDEX(3) = (CARD32)(((y)<<16) | (x));			\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupRectSRCPitch(w,h,pitch) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + RECT_WIDTH); 	\
         SIS_WQINDEX(1) = (CARD32)(((h)<<16) | (w));			\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + SRC_PITCH); 	\
         SIS_WQINDEX(3) = (CARD32)(pitch);				\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupRect(w,h) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + RECT_WIDTH); 	\
         SIS_WQINDEX(1) = (CARD32)(((h)<<16) | (w));			\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupPATFG(color) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + PAT_FGCOLOR); 	\
         SIS_WQINDEX(1) = (CARD32)(color);	 			\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupPATBG(color) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + PAT_BGCOLOR);	\
         SIS_WQINDEX(1) = (CARD32)(color);	 			\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupSRCFG(color) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + SRC_FGCOLOR);	\
         SIS_WQINDEX(1) = (CARD32)(color);	 			\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupSRCBG(color) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + SRC_BGCOLOR);	\
         SIS_WQINDEX(1) = (CARD32)(color);	 			\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupSRCTrans(color) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + TRANS_SRC_KEY_HIGH);	\
         SIS_WQINDEX(1) = (CARD32)(color);	 				\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + TRANS_SRC_KEY_LOW);	\
         SIS_WQINDEX(3) = (CARD32)(color);					\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupDSTTrans(color) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + TRANS_DST_KEY_HIGH);	\
         SIS_WQINDEX(1) = (CARD32)(color);	 				\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + TRANS_DST_KEY_LOW);	\
         SIS_WQINDEX(3) = (CARD32)(color);					\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupMONOPAT(p0,p1) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + MONO_MASK);		\
         SIS_WQINDEX(1) = (CARD32)(p0);	 				\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + MONO_MASK + 4);	\
         SIS_WQINDEX(3) = (CARD32)(p1);					\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupClip(left,top,right,bottom) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + LEFT_CLIP);	\
         SIS_WQINDEX(1) = (CARD32)(((left) & 0xFFFF) | ((top)<<16));   	\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + RIGHT_CLIP);	\
         SIS_WQINDEX(3) = (CARD32)(((right) & 0xFFFF)|((bottom)<<16)); 	\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupDSTBaseDoCMD(base) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + DST_ADDR); 	\
         SIS_WQINDEX(1) = (CARD32)(base);				\
         SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + COMMAND_READY);	\
         SIS_WQINDEX(3) = (CARD32)(pSiSUSB->CommandReg); 			\
	 if(pSiSUSB->NeedFlush) dummybuf = SIS_RQINDEX(3);   		\
	 SiSUSBUpdateQueue \
	 SiSUSBSetHwWP(ttt); \
      }

#define SiSUSBSetRectDoCMD(w,h) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + RECT_WIDTH); 	\
         SIS_WQINDEX(1) = (CARD32)(((h)<<16) | (w));	 		\
         SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + COMMAND_READY);	\
         SIS_WQINDEX(3) = (CARD32)(pSiSUSB->CommandReg); 			\
	 if(pSiSUSB->NeedFlush) dummybuf = SIS_RQINDEX(3);  		\
	 SiSUSBUpdateQueue 	\
	 SiSUSBSetHwWP(ttt); 	\
      }

#define SiSUSBSetupROP(rop) \
      pSiSUSB->CommandReg |= (rop) << 8;

#define SiSUSBDoCMD \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + COMMAND_READY);	\
         SIS_WQINDEX(1) = (CARD32)(pSiSUSB->CommandReg); 			\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_NIL_CMD); 			\
         SIS_WQINDEX(3) = (CARD32)(SISUSB_NIL_CMD); 			\
	 if(pSiSUSB->NeedFlush) dummybuf = SIS_RQINDEX(3);  		\
	 SiSUSBUpdateQueue \
	 SiSUSBSetHwWP(ttt); \
      }

/* Line */

#define SiSUSBSetupX0Y0X1Y1(x1,y1,x2,y2) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + LINE_X0);	\
         SIS_WQINDEX(1) = (CARD32)(((y1)<<16) | (x1)); 		\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + LINE_X1);	\
         SIS_WQINDEX(3) = (CARD32)(((y2)<<16) | (x2)); 		\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupX0Y0(x,y) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + LINE_X0);	\
         SIS_WQINDEX(1) = (CARD32)(((y)<<16) | (x)); 		\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupX1Y1(x,y) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + LINE_X1);	\
         SIS_WQINDEX(1) = (CARD32)(((y)<<16) | (x)); 		\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupLineCountPeriod(c, p) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + LINE_COUNT);	\
         SIS_WQINDEX(1) = (CARD32)(((p) << 16) | (c)); 			\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupStyle(ls,hs) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + LINE_STYLE_0);	\
         SIS_WQINDEX(1) = (CARD32)(ls);					\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + LINE_STYLE_1);	\
         SIS_WQINDEX(3) = (CARD32)(hs); 				\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

/* Trapezoid */

#define SiSUSBSetupYHLR(y,h,left,right) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + TRAP_YH);	\
         SIS_WQINDEX(1) = (CARD32)(((y)<<16) | (h)); 		\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + TRAP_LR);	\
         SIS_WQINDEX(3) = (CARD32)(((right)<<16) | (left));	\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }


#define SiSUSBSetupdLdR(dxL,dyL,fxR,dyR) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + TRAP_DL);	\
         SIS_WQINDEX(1) = (CARD32)(((dyL)<<16) | (dxL)); 	\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + TRAP_DR);	\
         SIS_WQINDEX(3) = (CARD32)(((dyR)<<16) | (dxR)); 	\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#define SiSUSBSetupELER(eL,eR) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + TRAP_EL);	\
         SIS_WQINDEX(1) = (CARD32)(eL);	 			\
	 SIS_WQINDEX(2) = (CARD32)(SISUSB_SPKC_HEADER + TRAP_ER);	\
         SIS_WQINDEX(3) = (CARD32)(eR); 			\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

/* (Constant) Alpha blended BitBlt (alpha = 8 bit) */

#define SiSUSBSetupAlpha(alpha) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + ALPHA_ALPHA);	\
         SIS_WQINDEX(1) = (CARD32)(alpha);	 			\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetPattern(num, value) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(SISUSB_SPKC_HEADER + (PATTERN_REG + (num * 4)));	\
         SIS_WQINDEX(1) = (CARD32)(value); 						\
         SiSUSBNILandUpdateSWQueue \
      }

#define SiSUSBSetupPatternRegBurst(pat1, pat2, pat3, pat4) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(pat1);		\
         SIS_WQINDEX(1) = (CARD32)(pat2);		\
	 SIS_WQINDEX(2) = (CARD32)(pat3);		\
         SIS_WQINDEX(3) = (CARD32)(pat4);		\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

typedef struct _SiSUSB_Packet12_YUV {
      CARD32 P12_Header0;
      CARD32 P12_Header1;
      CARD16 P12_UVPitch;	/* 8200 UV if planar, Y if packed */
      CARD16 P12_Unused0;	/* 8202 */
      CARD16 P12_YPitch;	/* 8204 Y if planar */
      CARD16 P12_AGPBase;	/* 8206 */
      CARD16 P12_Unused1;	/* 8208 */
      CARD16 P12_Unused2;	/* 820a */
      CARD16 P12_DstY;		/* 820c */
      CARD16 P12_DstX;		/* 820e */
      CARD32 P12_DstAddr;	/* 8210 */
      CARD16 P12_DstPitch;	/* 8214 */
      CARD16 P12_DstHeight;	/* 8216 */
      CARD16 P12_RectWidth;	/* 8218 */
      CARD16 P12_RectHeight;	/* 821a */
      CARD32 P12_Unused3;	/* 821c */
      CARD32 P12_Unused4;	/* 8220 */
      CARD32 P12_UVSrcAddr;	/* 8224 UV if planar, Y if packed */
      CARD32 P12_YSrcAddr;	/* 8228 Y if planar */
      CARD32 P12_Unused5;	/* 822c */
      CARD32 P12_Unused6;	/* 8230 */
      CARD16 P12_ClipLeft;	/* 8234 */
      CARD16 P12_ClipTop;	/* 8236 */
      CARD16 P12_ClipRight;	/* 8238 */
      CARD16 P12_ClipBottom;	/* 823a */
      CARD32 P12_Command;	/* 823c */
      CARD32 P12_Null1;
      CARD32 P12_Null2;
} SiSUSB_Packet12_YUV;

#define SiSUSBWritePacketPart(part1, part2, part3, part4) \
      { \
         CARD32 ttt = SiSUSBGetSwWP(); \
	 pointer tt = (char *)pSiSUSB->cmdQueueBase + ttt; \
         SIS_WQINDEX(0) = (CARD32)(part1);	\
         SIS_WQINDEX(1) = (CARD32)(part2);	\
	 SIS_WQINDEX(2) = (CARD32)(part3);	\
         SIS_WQINDEX(3) = (CARD32)(part4);	\
         SiSUSBUpdateQueue \
	 SiSUSBSetSwWP(ttt); \
      }

#endif  /* VRAM mode */

/* ---- MMIO mode ---- */

#ifndef SISVRAMQ

/* We assume a length of 4 bytes per command; since 512K of
 * of RAM are allocated, the number of commands is easily
 * calculated (and written to the address pointed to by
 * CmdQueueLenPtr, since sis_dri.c relocates this)
 * UPDATE: using the command queue without syncing totally
 * (ie assuming a QueueLength of 0) decreases system latency
 * dramatically on the integrated chipsets (sound gets interrupted,
 * etc.). We now sync every time... this is a little slower,
 * but it keeps the rest of the box somewhat alive.
 * This was the reason for switching to VRAM queue mode.
 */
#define SiSUSBIdle \
  { \
     if(pSiSUSB->ChipFlags & SiSCF_Integrated) { \
	CmdQueLen = 0; \
     } else { \
	CmdQueLen = ((512 * 1024) / 4) - 64; \
     } \
     while( (SIS_MMIO_IN16(pSiSUSB->IOBase, Q_STATUS+2) & 0x8000) != 0x8000) {}; \
     while( (SIS_MMIO_IN16(pSiSUSB->IOBase, Q_STATUS+2) & 0x8000) != 0x8000) {}; \
  }

#define SiSUSBSetupSRCBase(base) \
      if (CmdQueLen <= 0)  SiSUSBIdle; \
      SIS_MMIO_OUT32(pSiSUSB->IOBase, SRC_ADDR, base); \
      CmdQueLen--;

#define SiSUSBSetupSRCPitch(pitch) \
      if (CmdQueLen <= 0)  SiSUSBIdle; \
      SIS_MMIO_OUT16(pSiSUSB->IOBase, SRC_PITCH, pitch); \
      CmdQueLen--;

#define SiSUSBSetupSRCXY(x,y) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, SRC_Y, (x)<<16 | (y) );\
      CmdQueLen--;

#define SiSUSBSetupDSTBase(base) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, DST_ADDR, base);\
      CmdQueLen--;

#define SiSUSBSetupDSTXY(x,y) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, DST_Y, (x)<<16 | (y) );\
      CmdQueLen--;

#define SiSUSBSetupDSTRect(x,y) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, DST_PITCH, (y)<<16 | (x) );\
      CmdQueLen--;

#define SiSUSBSetupDSTColorDepth(bpp) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT16(pSiSUSB->IOBase, AGP_BASE, bpp);\
      CmdQueLen--;

#define SiSUSBSetupRect(w,h) \
      if(CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, RECT_WIDTH, (h)<<16 | (w) );\
      CmdQueLen--;

#define SiSUSBSetupPATFG(color) \
      if(CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, PAT_FGCOLOR, color);\
      CmdQueLen--;

#define SiSUSBSetupPATBG(color) \
      if(CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, PAT_BGCOLOR, color);\
      CmdQueLen--;

#define SiSUSBSetupSRCFG(color) \
      if(CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, SRC_FGCOLOR, color);\
      CmdQueLen--;

#define SiSUSBSetupSRCBG(color) \
      if(CmdQueLen <= 0)  SiSUSBIdle; \
      SIS_MMIO_OUT32(pSiSUSB->IOBase, SRC_BGCOLOR, color); \
      CmdQueLen--;

#define SiSUSBSetupSRCTrans(color) \
      if(CmdQueLen <= 1)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, TRANS_SRC_KEY_HIGH, color);\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, TRANS_SRC_KEY_LOW, color);\
      CmdQueLen -= 2;

#define SiSUSBSetupDSTTrans(color) \
      if(CmdQueLen <= 1)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, TRANS_DST_KEY_HIGH, color); \
      SIS_MMIO_OUT32(pSiSUSB->IOBase, TRANS_DST_KEY_LOW, color); \
      CmdQueLen -= 2;

#define SiSUSBSetupMONOPAT(p0,p1) \
      if(CmdQueLen <= 1)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, MONO_MASK, p0);\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, MONO_MASK+4, p1);\
      CmdQueLen=CmdQueLen-2;

#define SiSUSBSetupClipLT(left,top) \
      if(CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, LEFT_CLIP, ((left) & 0xFFFF) | (top)<<16); \
      CmdQueLen--;

#define SiSUSBSetupClipRB(right,bottom) \
      if(CmdQueLen <= 0) SiSUSBIdle; \
      SIS_MMIO_OUT32(pSiSUSB->IOBase, RIGHT_CLIP, ((right) & 0xFFFF) | (bottom)<<16); \
      CmdQueLen--;

#define SiSUSBSetupROP(rop) \
      pSiSUSB->CommandReg = (rop) << 8;

#define SiSUSBDoCMD \
      if (CmdQueLen <= 1)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, COMMAND_READY, pSiSUSB->CommandReg); \
      SIS_MMIO_OUT32(pSiSUSB->IOBase, FIRE_TRIGGER, 0); \
      CmdQueLen -= 2;

/* Line */

#define SiSUSBSetupX0Y0(x,y) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, LINE_X0, (y)<<16 | (x) );\
      CmdQueLen--;

#define SiSUSBSetupX1Y1(x,y) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, LINE_X1, (y)<<16 | (x) );\
      CmdQueLen--;

#define SiSUSBSetupLineCount(c) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT16(pSiSUSB->IOBase, LINE_COUNT, c);\
      CmdQueLen--;

#define SiSUSBSetupStylePeriod(p) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT16(pSiSUSB->IOBase, LINE_STYLE_PERIOD, p);\
      CmdQueLen--;

#define SiSUSBSetupStyleLow(ls) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, LINE_STYLE_0, ls);\
      CmdQueLen--;

#define SiSUSBSetupStyleHigh(ls) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, LINE_STYLE_1, ls);\
      CmdQueLen--;

/* Trapezoid */

#define SiSUSBSetupYH(y,h) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, TRAP_YH, (y)<<16 | (h) );\
      CmdQueLen--;

#define SiSUSBSetupLR(left,right) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, TRAP_LR, (right)<<16 | (left) );\
      CmdQueLen--;

#define SiSUSBSetupdL(dxL,dyL) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, TRAP_DL, (dyL)<<16 | (dxL) );\
      CmdQueLen--;

#define SiSUSBSetupdR(dxR,dyR) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, TRAP_DR, (dyR)<<16 | (dxR) );\
      CmdQueLen--;

#define SiSUSBSetupEL(eL) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, TRAP_EL, eL);\
      CmdQueLen--;

#define SiSUSBSetupER(eR) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, TRAP_ER, eR);\
      CmdQueLen--;

/* (Constant) alpha blended BitBlt (alpha = 8 bit) */

#define SiSUSBSetupAlpha(alpha) \
      if (CmdQueLen <= 0)  SiSUSBIdle;\
      SIS_MMIO_OUT32(pSiSUSB->IOBase, ALPHA_ALPHA, alpha);\
      CmdQueLen--;

/* Set Pattern register */

#define SiSUSBSetPattern(num, value) \
      if (CmdQueLen <= 0)  SiSUSBIdle; \
      SIS_MMIO_OUT32(pSiSUSB->IOBase, (PATTERN_REG + (num * 4)), value); \
      CmdQueLen--;

#endif  /* MMIO mode */

