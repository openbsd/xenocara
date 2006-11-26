/* $XFree86$ */
/* $XdotOrg$ */
/*
 * 2D acceleration for 5597/5598 and 6326
 * Definitions for the SIS engine communication
 *
 * Copyright (C) 1998, 1999 by Alan Hourihane, Wigan, England.
 * Parts Copyright (C) 2001-2005 Thomas Winischhofer, Vienna, Austria.
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
 *
 * Authors:  Alan Hourihane <alanh@fairlite.demon.co.uk>,
 *           Mike Chapman <mike@paranoia.com>,
 *           Juanjo Santamarta <santamarta@ctv.es>,
 *           Mitani Hiroshi <hmitani@drl.mei.co.jp>,
 *           David Thomas <davtom@dream.org.uk>,
 *	     Thomas Winischhofer <thomas@winischhofer.net>.
 */


/* Definitions for the SIS engine communication. ------------------------------------ */

/* For pre-530 chipsets only!!! */

/* Engine Registers for 1st generation engines (5597/5598/6326) */
const int sisReg32MMIO[] = {
            0x8280,0x8284,0x8288,0x828C,0x8290,0x8294,
            0x8298,0x829C,0x82A0,0x82A4,0x82A8,0x82AC
};

#define BR(x) sisReg32MMIO[x]

/* These are done using Memory Mapped IO, of the registers */
/*
 * Modified for Sis by Xavier Ducoin (xavier@rd.lectra.fr)
 *
 */

/* Command Reg 0 (0x82aa, [15:0]) */
#define sisSRCBG            0x0000   /* source select */
#define sisSRCFG            0x0001
#define sisSRCVIDEO         0x0002
#define sisSRCSYSTEM        0x0003

#define sisPATFG            0x0004   /* pattern select */
#define sisPATREG           0x0008
#define sisPATBG            0x0000

#define sisLEFT2RIGHT       0x0010   /* Direction select */
#define sisRIGHT2LEFT       0x0000
#define sisTOP2BOTTOM       0x0020
#define sisBOTTOM2TOP       0x0000
#define sisXINCREASE        sisLEFT2RIGHT
#define sisYINCREASE        sisTOP2BOTTOM

#define sisCLIPENABL        0x0040   /* Clipping select */
#define sisCLIPINTRN        0x0080
#define sisCLIPEXTRN        0x0000

#define sisCMDBLT           0x0000   /* Command select */
#define sisCMDBLTMSK        0x0100
#define sisCMDCOLEXP        0x0200
#define sisCMDLINE          0x0300
#define sisFLGECOLEXP       0x2000
#define sisCMDECOLEXP       (sisCMDCOLEXP | sisFLGECOLEXP)

#define sisLASTPIX          0x0800   /* Line parameters */
#define sisXMAJOR           0x0400


/* Macros to do useful things with the SIS BitBLT engine */

#define sisBLTSync \
  while(SIS_MMIO_IN16(pSiS->IOBase, BR(10) + 2) & 0x4000) {}

/* According to SiS 6326 2D programming guide, 16 bits position at   */
/* 0x82A8 returns queue free. But this don't work, so don't wait     */
/* anything when turbo-queue is enabled. If there are frequent syncs */
/* this should work. But not for xaa_benchmark :-(     */

/* TW: Bit 16 only applies to the hardware queue, not the software
 *     (=turbo) queue.
 */

#define sisBLTWAIT \
  if(!pSiS->TurboQueue) { \
    while(SIS_MMIO_IN16(pSiS->IOBase, BR(10) + 2) & 0x4000) {} \
  } else { \
    sisBLTSync \
  }

#define sisSETPATREG() \
   ((UChar *)(pSiS->IOBase + BR(11)))

#define sisSETPATREGL() \
   ((ULong *)(pSiS->IOBase + BR(11)))

/* trigger command */
#define sisSETCMD(op) \
  { \
  ULong temp; \
  SIS_MMIO_OUT16(pSiS->IOBase, BR(10) + 2, op); \
  temp = SIS_MMIO_IN32(pSiS->IOBase, BR(10)); \
  (void)temp; \
  }

/* set foreground color and fg ROP */
#define sisSETFGROPCOL(rop, color) \
   SIS_MMIO_OUT32(pSiS->IOBase, BR(4), ((rop << 24) | (color & 0xFFFFFF)));

/* set background color and bg ROP */
#define sisSETBGROPCOL(rop, color) \
   SIS_MMIO_OUT32(pSiS->IOBase, BR(5), ((rop << 24) | (color & 0xFFFFFF)));

/* background color */
#define sisSETBGCOLOR(bgColor) \
   SIS_MMIO_OUT32(pSiS->IOBase, BR(5), (bgColor));

/* foreground color */
#define sisSETFGCOLOR(fgColor) \
   SIS_MMIO_OUT32(pSiS->IOBase, BR(4), (fgcolor));

/* ROP */
#define sisSETROPFG(op) \
   SIS_MMIO_OUT8(pSiS->IOBase, BR(4) + 3, op);

#define sisSETROPBG(op) \
  SIS_MMIO_OUT8(pSiS->IOBase, BR(5) + 3, op);

#define sisSETROP(op) \
   sisSETROPFG(op); sisSETROPBG(op);

/* source and dest address */
#define sisSETSRCADDR(srcAddr) \
  SIS_MMIO_OUT32(pSiS->IOBase, BR(0), (srcAddr & 0x3FFFFFL));

#define sisSETDSTADDR(dstAddr) \
  SIS_MMIO_OUT32(pSiS->IOBase, BR(1), (dstAddr & 0x3FFFFFL));

/* pitch */
#define sisSETPITCH(srcPitch,dstPitch) \
  SIS_MMIO_OUT32(pSiS->IOBase, BR(2), ((((dstPitch) & 0xFFFF) << 16) | ((srcPitch) & 0xFFFF)));

#define sisSETSRCPITCH(srcPitch) \
  SIS_MMIO_OUT16(pSiS->IOBase, BR(2), ((srcPitch) & 0xFFFF));

#define sisSETDSTPITCH(dstPitch) \
  SIS_MMIO_OUT16(pSiS->IOBase, BR(2) + 2, ((dstPitch) & 0xFFFF));

/* Height and width
 * According to SIS 2D Engine Programming Guide
 * height -1, width - 1 independant of Bpp
 */
#define sisSETHEIGHTWIDTH(Height, Width) \
  SIS_MMIO_OUT32(pSiS->IOBase, BR(3), ((((Height) & 0xFFFF) << 16) | ((Width) & 0xFFFF)));

/* Clipping */
#define sisSETCLIPTOP(x, y) \
   SIS_MMIO_OUT32(pSiS->IOBase, BR(8), ((((y) & 0xFFFF) << 16) | ((x) & 0xFFFF)));

#define sisSETCLIPBOTTOM(x, y) \
   SIS_MMIO_OUT32(pSiS->IOBase, BR(9), ((((y) & 0xFFFF) << 16) | ((x) & 0xFFFF)));

/* Line drawing */
#define sisSETXStart(XStart) \
  SIS_MMIO_OUT32(pSiS->IOBase, BR(0), ((XStart) & 0xFFFF));

#define sisSETYStart(YStart) \
  SIS_MMIO_OUT32(pSiS->IOBase, BR(1), ((YStart) & 0xFFFF));

#define sisSETLineMajorCount(MajorAxisCount) \
   SIS_MMIO_OUT32(pSiS->IOBase, BR(3), ((MajorAxisCount) & 0xFFFF));

#define sisSETLineSteps(K1,K2) \
   SIS_MMIO_OUT32(pSiS->IOBase, BR(6), ((((K1) & 0xFFFF) << 16) | ((K2) & 0xFFFF)));

#define sisSETLineErrorTerm(ErrorTerm) \
   SIS_MMIO_OUT16(pSiS->IOBase, BR(7), (ErrorTerm));
