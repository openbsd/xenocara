/* $XFree86$ */
/* $XdotOrg$ */
/*
 * 2D Acceleration for SiS 315 and Xabre series
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
 * 2003/08/18: Added VRAM queue support
 *
 */

/* SiS315 and 330 engine commands */
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
#define TRAP_YH			SRC_Y    /* 0x8208 */
#define TRAP_LR			DST_Y    /* 0x820C */
#define TRAP_DL			0x8244
#define TRAP_DR			0x8248
#define TRAP_EL			0x824C
#define TRAP_ER			0x8250

/* Queue */
#define Q_BASE_ADDR		0x85C0  /* Base address of software queue */
#define Q_WRITE_PTR		0x85C4  /* Current write pointer */
#define Q_READ_PTR		0x85C8  /* Current read pointer */
#define Q_STATUS		0x85CC  /* queue status */

/* VRAM queue operation command header definitions */
#define SIS_SPKC_HEADER 	0x16800000L
#define SIS_BURST_HEADER0	0x568A0000L
#define SIS_BURST_HEADER1	0x62100000L
#define SIS_PACKET_HEARER0 	0x968A0000L
#define SIS_PACKET_HEADER1	0x62100000L
#define SIS_NIL_CMD		0x168F0000L

#define SIS_PACKET12_HEADER0	0x968A000CL
#define SIS_PACKET12_HEADER1	0x62100010L
#define SIS_PACKET12_LENGTH	80

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
#define CmdQueLen (*(pSiS->cmdQueueLenPtr))

#define SiSQEmpty \
  { \
     while( (SIS_MMIO_IN16(pSiS->IOBase, Q_STATUS+2) & 0x0400) != 0x0400) {}; \
     while( (SIS_MMIO_IN16(pSiS->IOBase, Q_STATUS+2) & 0x0400) != 0x0400) {}; \
  }

#define SiSResetCmd		pSiS->CommandReg = 0;

#define SiSSetupCMDFlag(flags)  pSiS->CommandReg |= (flags);

/* --- VRAM mode --- */

#define SiSGetSwWP() (CARD32)(*(pSiS->cmdQ_SharedWritePort))
#define SiSGetHwRP() (CARD32)(SIS_MMIO_IN32(pSiS->IOBase, Q_READ_PTR))

#define SiSFlushCmdBuf  \
  if(pSiS->NeedFlush) { \
     CARD32 ttt = ((SiSGetSwWP()) - 4) & pSiS->cmdQueueSizeMask;	\
     pointer tt = (char *)pSiS->cmdQueueBase + ttt; 			\
     dummybuf = SIS_RQINDEX(0);						\
  }

#define SiSSyncWP    \
  SiSFlushCmdBuf;    \
  SIS_MMIO_OUT32(pSiS->IOBase, Q_WRITE_PTR, (CARD32)(*(pSiS->cmdQ_SharedWritePort)));

#define SiSSetHwWP(p) \
  *(pSiS->cmdQ_SharedWritePort) = (p);   	\
  SIS_MMIO_OUT32(pSiS->IOBase, Q_WRITE_PTR, (p));

#define SiSSetSwWP(p) *(pSiS->cmdQ_SharedWritePort) = (p);

#define SiSCheckQueue(amount)

#if 0
      { \
	CARD32 mcurrent, i=0, ttt = SiSGetSwWP(); \
	if((ttt + amount) >= pSiS->cmdQueueSize) { \
	   do { \
	      mcurrent = SIS_MMIO_IN32(pSiS->IOBase, Q_READ_PTR); \
	      i++; \
	   } while((mcurrent > ttt) || (mcurrent < ((ttt + amount) & pSiS->cmdQueueSizeMask))); \
	} else { \
	   do { \
	      mcurrent = SIS_MMIO_IN32(pSiS->IOBase, Q_READ_PTR); \
	      i++; \
	   } while((mcurrent > ttt) && (mcurrent < (ttt + amount))); \
	} \
      }
#endif

#define SiSUpdateQueue \
      SiSWriteQueue(tt); \
      ttt += 16; \
      ttt &= pSiS->cmdQueueSizeMask; \
      if(!ttt) { \
	 while(SIS_MMIO_IN32(pSiS->IOBase, Q_READ_PTR) < pSiS->cmdQueueSize_div4) {} \
      } else if(ttt == pSiS->cmdQueueSize_div4) { \
	 CARD32 temppp; \
	 do { \
	    temppp = SIS_MMIO_IN32(pSiS->IOBase, Q_READ_PTR); \
	 } while(temppp >= ttt && temppp <= pSiS->cmdQueueSize_div2); \
      } else if(ttt == pSiS->cmdQueueSize_div2) { \
	 CARD32 temppp; \
	 do { \
	    temppp = SIS_MMIO_IN32(pSiS->IOBase, Q_READ_PTR); \
	 } while(temppp >= ttt && temppp <= pSiS->cmdQueueSize_4_3); \
      } else if(ttt == pSiS->cmdQueueSize_4_3) { \
	 while(SIS_MMIO_IN32(pSiS->IOBase, Q_READ_PTR) > ttt) {} \
      }

/* Write-updates MUST be 128bit aligned. */
#define SiSNILandUpdateSWQueue \
      SIS_WQINDEX(2) = (CARD32)(SIS_NIL_CMD); \
      SIS_WQINDEX(3) = (CARD32)(SIS_NIL_CMD); \
      SiSUpdateQueue; \
      SiSSetSwWP(ttt);

#ifdef SISVRAMQ

#define SiSIdle \
  { \
     while( (SIS_MMIO_IN16(pSiS->IOBase, Q_STATUS+2) & 0x8000) != 0x8000) {}; \
     while( (SIS_MMIO_IN16(pSiS->IOBase, Q_STATUS+2) & 0x8000) != 0x8000) {}; \
     while( (SIS_MMIO_IN16(pSiS->IOBase, Q_STATUS+2) & 0x8000) != 0x8000) {}; \
     while( (SIS_MMIO_IN16(pSiS->IOBase, Q_STATUS+2) & 0x8000) != 0x8000) {}; \
  }

#define SiSSetupSRCDSTBase(srcbase,dstbase) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + SRC_ADDR); \
	 SIS_WQINDEX(1) = (CARD32)(srcbase); 			\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + DST_ADDR); \
	 SIS_WQINDEX(3) = (CARD32)(dstbase); 			\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupSRCDSTXY(sx,sy,dx,dy) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + SRC_Y); 	\
	 SIS_WQINDEX(1) = (CARD32)(((sx)<<16) | (sy));		\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + DST_Y); 	\
	 SIS_WQINDEX(3) = (CARD32)(((dx)<<16) | (dy)); 		\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupDSTXYRect(x,y,w,h) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + DST_Y); 		\
	 SIS_WQINDEX(1) = (CARD32)(((x)<<16) | (y));	 		\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + RECT_WIDTH); 	\
	 SIS_WQINDEX(3) = (CARD32)(((h)<<16) | (w));			\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupSRCPitchDSTRect(pitch,x,y) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + SRC_PITCH); 	\
	 SIS_WQINDEX(1) = (CARD32)(pitch);				\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + DST_PITCH); 	\
	 SIS_WQINDEX(3) = (CARD32)(((y)<<16) | (x));			\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupSRCBase(base) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + SRC_ADDR); 	\
	 SIS_WQINDEX(1) = (CARD32)(base); 				\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupSRCPitch(pitch) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + SRC_PITCH); 	\
	 SIS_WQINDEX(1) = (CARD32)(pitch);				\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupSRCXY(x,y) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + SRC_Y); 	\
	 SIS_WQINDEX(1) = (CARD32)(((x)<<16) | (y));		\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupDSTBase(base) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + DST_ADDR); 	\
	 SIS_WQINDEX(1) = (CARD32)(base);				\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupDSTXY(x,y) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + DST_Y); 	\
	 SIS_WQINDEX(1) = (CARD32)(((x)<<16) | (y));	 	\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupDSTRect(x,y) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + DST_PITCH); 	\
	 SIS_WQINDEX(1) = (CARD32)(((y)<<16) | (x));		\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupDSTRectBurstHeader(x,y,reg,num) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + DST_PITCH);	\
	 SIS_WQINDEX(1) = (CARD32)(((y)<<16) | (x));			\
	 SIS_WQINDEX(2) = (CARD32)(SIS_BURST_HEADER0 + reg); 		\
	 SIS_WQINDEX(3) = (CARD32)(SIS_BURST_HEADER1 + num); 		\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupDSTColorDepth(bpp) \
      pSiS->CommandReg = (((CARD32)(bpp)) & (GENMASK(17:16)));

#define SiSSetupPATFGDSTRect(color,x,y) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + PAT_FGCOLOR); 	\
	 SIS_WQINDEX(1) = (CARD32)(color);	 			\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + DST_PITCH); 	\
	 SIS_WQINDEX(3) = (CARD32)(((y)<<16) | (x));			\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupSRCFGDSTRect(color,x,y) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + SRC_FGCOLOR); 	\
	 SIS_WQINDEX(1) = (CARD32)(color);	 			\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + DST_PITCH); 	\
	 SIS_WQINDEX(3) = (CARD32)(((y)<<16) | (x));			\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupRectSRCPitch(w,h,pitch) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + RECT_WIDTH); 	\
	 SIS_WQINDEX(1) = (CARD32)(((h)<<16) | (w));			\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + SRC_PITCH); 	\
	 SIS_WQINDEX(3) = (CARD32)(pitch);				\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupRect(w,h) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + RECT_WIDTH); 	\
	 SIS_WQINDEX(1) = (CARD32)(((h)<<16) | (w));			\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupPATFG(color) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + PAT_FGCOLOR); 	\
	 SIS_WQINDEX(1) = (CARD32)(color);	 			\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupPATBG(color) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + PAT_BGCOLOR);	\
	 SIS_WQINDEX(1) = (CARD32)(color);	 			\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupSRCFG(color) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + SRC_FGCOLOR);	\
	 SIS_WQINDEX(1) = (CARD32)(color);	 			\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupSRCBG(color) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + SRC_BGCOLOR);	\
	 SIS_WQINDEX(1) = (CARD32)(color);	 			\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupSRCTrans(color) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + TRANS_SRC_KEY_HIGH);	\
	 SIS_WQINDEX(1) = (CARD32)(color);	 				\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + TRANS_SRC_KEY_LOW);	\
	 SIS_WQINDEX(3) = (CARD32)(color);					\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupDSTTrans(color) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + TRANS_DST_KEY_HIGH);	\
	 SIS_WQINDEX(1) = (CARD32)(color);	 				\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + TRANS_DST_KEY_LOW);	\
	 SIS_WQINDEX(3) = (CARD32)(color);					\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupMONOPAT(p0,p1) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + MONO_MASK);		\
	 SIS_WQINDEX(1) = (CARD32)(p0);	 				\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + MONO_MASK + 4);	\
	 SIS_WQINDEX(3) = (CARD32)(p1);					\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupClip(left,top,right,bottom) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + LEFT_CLIP);	\
	 SIS_WQINDEX(1) = (CARD32)(((left) & 0xFFFF) | ((top)<<16));   	\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + RIGHT_CLIP);	\
	 SIS_WQINDEX(3) = (CARD32)(((right) & 0xFFFF)|((bottom)<<16)); 	\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupDSTBaseDoCMD(base) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + DST_ADDR); 	\
	 SIS_WQINDEX(1) = (CARD32)(base);				\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + COMMAND_READY);	\
	 SIS_WQINDEX(3) = (CARD32)(pSiS->CommandReg); 			\
	 if(pSiS->NeedFlush) dummybuf = SIS_RQINDEX(3);   		\
	 SiSUpdateQueue \
	 SiSSetHwWP(ttt); \
      }

#define SiSSetRectDoCMD(w,h) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + RECT_WIDTH); 	\
	 SIS_WQINDEX(1) = (CARD32)(((h)<<16) | (w));	 		\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + COMMAND_READY);	\
	 SIS_WQINDEX(3) = (CARD32)(pSiS->CommandReg); 			\
	 if(pSiS->NeedFlush) dummybuf = SIS_RQINDEX(3);  		\
	 SiSUpdateQueue 	\
	 SiSSetHwWP(ttt); 	\
      }

#define SiSSetupROP(rop) \
      pSiS->CommandReg |= (rop) << 8;

#define SiSDoCMD \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + COMMAND_READY);	\
	 SIS_WQINDEX(1) = (CARD32)(pSiS->CommandReg); 			\
	 SIS_WQINDEX(2) = (CARD32)(SIS_NIL_CMD); 			\
	 SIS_WQINDEX(3) = (CARD32)(SIS_NIL_CMD); 			\
	 if(pSiS->NeedFlush) dummybuf = SIS_RQINDEX(3);  		\
	 SiSUpdateQueue \
	 SiSSetHwWP(ttt); \
      }

#define SiSDualPipe(disable) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 CARD32 _tmp = SIS_MMIO_IN32(pSiS->IOBase, FIRE_TRIGGER) & ~(1 << 10);	\
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + FIRE_TRIGGER);	\
	 SIS_WQINDEX(1) = (CARD32)(_tmp | ((disable & 1) << 10)); 	\
	 SIS_WQINDEX(2) = (CARD32)(SIS_NIL_CMD); 			\
	 SIS_WQINDEX(3) = (CARD32)(SIS_NIL_CMD); 			\
	 if(pSiS->NeedFlush) dummybuf = SIS_RQINDEX(3);  		\
	 SiSUpdateQueue \
	 SiSSetHwWP(ttt); \
      }

/* Line */

#define SiSSetupX0Y0X1Y1(x1,y1,x2,y2) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + LINE_X0);	\
	 SIS_WQINDEX(1) = (CARD32)(((y1)<<16) | (x1)); 		\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + LINE_X1);	\
	 SIS_WQINDEX(3) = (CARD32)(((y2)<<16) | (x2)); 		\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupX0Y0(x,y) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + LINE_X0);	\
	 SIS_WQINDEX(1) = (CARD32)(((y)<<16) | (x)); 		\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupX1Y1(x,y) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + LINE_X1);	\
	 SIS_WQINDEX(1) = (CARD32)(((y)<<16) | (x)); 		\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupLineCountPeriod(c, p) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + LINE_COUNT);	\
	 SIS_WQINDEX(1) = (CARD32)(((p) << 16) | (c)); 			\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupStyle(ls,hs) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + LINE_STYLE_0);	\
	 SIS_WQINDEX(1) = (CARD32)(ls);					\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + LINE_STYLE_1);	\
	 SIS_WQINDEX(3) = (CARD32)(hs); 				\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

/* Trapezoid */

#define SiSSetupYHLR(y,h,left,right) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + TRAP_YH);	\
	 SIS_WQINDEX(1) = (CARD32)(((y)<<16) | (h)); 		\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + TRAP_LR);	\
	 SIS_WQINDEX(3) = (CARD32)(((right)<<16) | (left));	\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }


#define SiSSetupdLdR(dxL,dyL,fxR,dyR) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + TRAP_DL);	\
	 SIS_WQINDEX(1) = (CARD32)(((dyL)<<16) | (dxL)); 	\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + TRAP_DR);	\
	 SIS_WQINDEX(3) = (CARD32)(((dyR)<<16) | (dxR)); 	\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

#define SiSSetupELER(eL,eR) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + TRAP_EL);	\
	 SIS_WQINDEX(1) = (CARD32)(eL);	 			\
	 SIS_WQINDEX(2) = (CARD32)(SIS_SPKC_HEADER + TRAP_ER);	\
	 SIS_WQINDEX(3) = (CARD32)(eR); 			\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

/* (Constant) Alpha blended BitBlt (alpha = 8 bit) */

#define SiSSetupAlpha(alpha) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + ALPHA_ALPHA);	\
	 SIS_WQINDEX(1) = (CARD32)(alpha);	 			\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetPattern(num, value) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(SIS_SPKC_HEADER + (PATTERN_REG + (num * 4)));	\
	 SIS_WQINDEX(1) = (CARD32)(value); 						\
	 SiSNILandUpdateSWQueue \
      }

#define SiSSetupPatternRegBurst(pat1, pat2, pat3, pat4) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(pat1);		\
	 SIS_WQINDEX(1) = (CARD32)(pat2);		\
	 SIS_WQINDEX(2) = (CARD32)(pat3);		\
	 SIS_WQINDEX(3) = (CARD32)(pat4);		\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
      }

typedef struct _SiS_Packet12_YUV {
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
} SiS_Packet12_YUV;

#define SiSWritePacketPart(part1, part2, part3, part4) \
      { \
	 CARD32 ttt = SiSGetSwWP(); \
	 pointer tt = (char *)pSiS->cmdQueueBase + ttt; \
	 SIS_WQINDEX(0) = (CARD32)(part1);	\
	 SIS_WQINDEX(1) = (CARD32)(part2);	\
	 SIS_WQINDEX(2) = (CARD32)(part3);	\
	 SIS_WQINDEX(3) = (CARD32)(part4);	\
	 SiSUpdateQueue \
	 SiSSetSwWP(ttt); \
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
#define SiSIdle \
  { \
     if(pSiS->ChipFlags & SiSCF_Integrated) { \
	CmdQueLen = 0; \
     } else { \
	CmdQueLen = ((512 * 1024) / 4) - 64; \
     } \
     while( (SIS_MMIO_IN16(pSiS->IOBase, Q_STATUS+2) & 0x8000) != 0x8000) {}; \
     while( (SIS_MMIO_IN16(pSiS->IOBase, Q_STATUS+2) & 0x8000) != 0x8000) {}; \
  }

#define SiSSetupSRCBase(base) \
      if (CmdQueLen <= 0)  SiSIdle; \
      SIS_MMIO_OUT32(pSiS->IOBase, SRC_ADDR, base); \
      CmdQueLen--;

#define SiSSetupSRCPitch(pitch) \
      if (CmdQueLen <= 0)  SiSIdle; \
      SIS_MMIO_OUT16(pSiS->IOBase, SRC_PITCH, pitch); \
      CmdQueLen--;

#define SiSSetupSRCXY(x,y) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, SRC_Y, (x)<<16 | (y) );\
      CmdQueLen--;

#define SiSSetupDSTBase(base) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, DST_ADDR, base);\
      CmdQueLen--;

#define SiSSetupDSTXY(x,y) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, DST_Y, (x)<<16 | (y) );\
      CmdQueLen--;

#define SiSSetupDSTRect(x,y) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, DST_PITCH, (y)<<16 | (x) );\
      CmdQueLen--;

#define SiSSetupDSTColorDepth(bpp) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT16(pSiS->IOBase, AGP_BASE, bpp);\
      CmdQueLen--;

#define SiSSetupRect(w,h) \
      if(CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, RECT_WIDTH, (h)<<16 | (w) );\
      CmdQueLen--;

#define SiSSetupPATFG(color) \
      if(CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, PAT_FGCOLOR, color);\
      CmdQueLen--;

#define SiSSetupPATBG(color) \
      if(CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, PAT_BGCOLOR, color);\
      CmdQueLen--;

#define SiSSetupSRCFG(color) \
      if(CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, SRC_FGCOLOR, color);\
      CmdQueLen--;

#define SiSSetupSRCBG(color) \
      if(CmdQueLen <= 0)  SiSIdle; \
      SIS_MMIO_OUT32(pSiS->IOBase, SRC_BGCOLOR, color); \
      CmdQueLen--;

#define SiSSetupSRCTrans(color) \
      if(CmdQueLen <= 1)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, TRANS_SRC_KEY_HIGH, color);\
      SIS_MMIO_OUT32(pSiS->IOBase, TRANS_SRC_KEY_LOW, color);\
      CmdQueLen -= 2;

#define SiSSetupDSTTrans(color) \
      if(CmdQueLen <= 1)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, TRANS_DST_KEY_HIGH, color); \
      SIS_MMIO_OUT32(pSiS->IOBase, TRANS_DST_KEY_LOW, color); \
      CmdQueLen -= 2;

#define SiSSetupMONOPAT(p0,p1) \
      if(CmdQueLen <= 1)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, MONO_MASK, p0);\
      SIS_MMIO_OUT32(pSiS->IOBase, MONO_MASK+4, p1);\
      CmdQueLen=CmdQueLen-2;

#define SiSSetupClipLT(left,top) \
      if(CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, LEFT_CLIP, ((left) & 0xFFFF) | (top)<<16); \
      CmdQueLen--;

#define SiSSetupClipRB(right,bottom) \
      if(CmdQueLen <= 0) SiSIdle; \
      SIS_MMIO_OUT32(pSiS->IOBase, RIGHT_CLIP, ((right) & 0xFFFF) | (bottom)<<16); \
      CmdQueLen--;

#define SiSSetupROP(rop) \
      pSiS->CommandReg = (rop) << 8;

#define SiSDoCMD \
      if (CmdQueLen <= 1)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, COMMAND_READY, pSiS->CommandReg); \
      SIS_MMIO_OUT32(pSiS->IOBase, FIRE_TRIGGER, 0); \
      CmdQueLen -= 2;

/* Line */

#define SiSSetupX0Y0(x,y) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, LINE_X0, (y)<<16 | (x) );\
      CmdQueLen--;

#define SiSSetupX1Y1(x,y) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, LINE_X1, (y)<<16 | (x) );\
      CmdQueLen--;

#define SiSSetupLineCount(c) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT16(pSiS->IOBase, LINE_COUNT, c);\
      CmdQueLen--;

#define SiSSetupStylePeriod(p) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT16(pSiS->IOBase, LINE_STYLE_PERIOD, p);\
      CmdQueLen--;

#define SiSSetupStyleLow(ls) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, LINE_STYLE_0, ls);\
      CmdQueLen--;

#define SiSSetupStyleHigh(ls) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, LINE_STYLE_1, ls);\
      CmdQueLen--;

/* Trapezoid */

#define SiSSetupYH(y,h) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, TRAP_YH, (y)<<16 | (h) );\
      CmdQueLen--;

#define SiSSetupLR(left,right) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, TRAP_LR, (right)<<16 | (left) );\
      CmdQueLen--;

#define SiSSetupdL(dxL,dyL) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, TRAP_DL, (dyL)<<16 | (dxL) );\
      CmdQueLen--;

#define SiSSetupdR(dxR,dyR) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, TRAP_DR, (dyR)<<16 | (dxR) );\
      CmdQueLen--;

#define SiSSetupEL(eL) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, TRAP_EL, eL);\
      CmdQueLen--;

#define SiSSetupER(eR) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, TRAP_ER, eR);\
      CmdQueLen--;

/* (Constant) alpha blended BitBlt (alpha = 8 bit) */

#define SiSSetupAlpha(alpha) \
      if (CmdQueLen <= 0)  SiSIdle;\
      SIS_MMIO_OUT32(pSiS->IOBase, ALPHA_ALPHA, alpha);\
      CmdQueLen--;

/* Set Pattern register */

#define SiSSetPattern(num, value) \
      if (CmdQueLen <= 0)  SiSIdle; \
      SIS_MMIO_OUT32(pSiS->IOBase, (PATTERN_REG + (num * 4)), value); \
      CmdQueLen--;

#endif  /* MMIO mode */

