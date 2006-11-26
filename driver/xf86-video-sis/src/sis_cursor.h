/* $XFree86$ */
/* $XdotOrg$ */
/*
 * SiS hardware cursor handling
 * Definitions
 *
 * Copyright (C) 2001-2005 by Thomas Winischhofer, Vienna, Austria.
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
 * Author:   Thomas Winischhofer <thomas@winischhofer.net>
 *
 * Idea based on code by Can-Ru Yeou, SiS Inc.
 *
 */

#define CS(x)   (0x8500 + (x << 2))

/* 300 series, CRT1 */

/* 80000000 = RGB(1) - MONO(0)
 * 40000000 = enable(1) - disable(0)
 * 20000000 = 32(1) / 16(1) bit RGB
 * 10000000 = "ghost"(1) - [other effect](0)
 */

#define sis300GetCursorStatus \
  SIS_MMIO_IN32(pSiS->IOBase, CS(0)) & 0x40000000;

#define sis300SetCursorStatus(status) \
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(0)); \
  temp &= 0xbfffffff; \
  temp |= status; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), temp); \
  }

#define sis300EnableHWCursor() \
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(0)); \
  temp &= 0x0fffffff; \
  temp |= 0x40000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), temp); \
  }

#define sis300EnableHWARGBCursor() \
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(0)); \
  temp |= 0xF0000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), temp); \
  }

#define sis300EnableHWARGB16Cursor() \
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(0)); \
  temp &= 0x0fffffff; \
  temp |= 0xD0000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), temp); \
  }

#define sis300SwitchToMONOCursor() \
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(0)); \
  temp &= 0x4fffffff; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), temp); \
  }

#define sis300SwitchToRGBCursor() \
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(0)); \
  temp |= 0xB0000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), temp); \
  }

#define sis300DisableHWCursor()\
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(0)); \
  temp &= 0xbFFFFFFF; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), temp); \
  }

#define sis300SetCursorBGColor(color)\
  SIS_MMIO_OUT32(pSiS->IOBase, CS(1), (color));
#define sis300SetCursorFGColor(color)\
  SIS_MMIO_OUT32(pSiS->IOBase, CS(2), (color));

#define sis300SetCursorPositionX(x,preset)\
  SIS_MMIO_OUT32(pSiS->IOBase, CS(3), ((x) | ((preset) << 16)));
#define sis300SetCursorPositionY(y,preset)\
  SIS_MMIO_OUT32(pSiS->IOBase, CS(4), ((y) | ((preset) << 16)));

#define sis300SetCursorAddress(address)\
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(0)); \
  temp &= 0xF0FF0000; \
  temp |= address; \
  SIS_MMIO_OUT32(pSiS->IOBase,CS(0),temp); \
  }

/* 300 series, CRT2 */

/* 80000000 = RGB(1) - MONO(0)
 * 40000000 = enable(1) - disable(0)
 * 20000000 = 32(1) / 16(1) bit RGB
 * 10000000 = unused (always "ghosting")
 */

#define sis301GetCursorStatus \
  SIS_MMIO_IN32(pSiS->IOBase, CS(8)) & 0x40000000;

#define sis301SetCursorStatus(status) \
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(8)); \
  temp &= 0xbfffffff; \
  temp |= status; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8), temp); \
  }

#define sis301EnableHWCursor()\
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(8)); \
  temp &= 0x0fffffff; \
  temp |= 0x40000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8), temp); \
  }

#define sis301EnableHWARGBCursor()\
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(8)); \
  temp |= 0xF0000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8), temp); \
  }

#define sis301EnableHWARGB16Cursor()\
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(8)); \
  temp &= 0x0FFFFFFF; \
  temp |= 0xD0000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8), temp); \
  }

#define sis301SwitchToRGBCursor() \
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(8)); \
  temp |= 0xB0000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8), temp); \
  }

#define sis301SwitchToMONOCursor() \
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(8)); \
  temp &= 0x4fffffff; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8), temp); \
  }

#define sis301DisableHWCursor()\
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(8)); \
  temp &= 0xbFFFFFFF; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8), temp); \
  }

#define sis301SetCursorBGColor(color)\
  SIS_MMIO_OUT32(pSiS->IOBase, CS(9), (color));
#define sis301SetCursorFGColor(color)\
  SIS_MMIO_OUT32(pSiS->IOBase, CS(10), (color));

#define sis301SetCursorPositionX(x,preset)\
  SIS_MMIO_OUT32(pSiS->IOBase, CS(11), ((x) | ((preset) << 16)));
#define sis301SetCursorPositionY(y,preset)\
  SIS_MMIO_OUT32(pSiS->IOBase, CS(12), ((y) | ((preset) << 16)));

#define sis301SetCursorAddress(address)\
  { \
  ULong temp; \
  temp = SIS_MMIO_IN32(pSiS->IOBase, CS(8)); \
  temp &= 0xF0FF0000; \
  temp |= address; \
  SIS_MMIO_OUT32(pSiS->IOBase,CS(8),temp); \
  }

/* 315/330 series CRT1 */

/* 80000000 = RGB(1) - MONO(0)
 * 40000000 = enable(1) - disable(0)
 * 20000000 = 32(1) / 16(1) bit RGB
 * 10000000 = "ghost"(1) - Alpha Blend(0)
 */

#define sis310GetCursorStatus \
  SIS_MMIO_IN32(pSiS->IOBase, CS(0)) & 0x40000000;

#define sis310SetCursorStatus(status) \
  pSiS->HWCursorBackup[0] &= 0xbfffffff; \
  pSiS->HWCursorBackup[0] |= status; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), pSiS->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(3), pSiS->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(4), pSiS->HWCursorBackup[4]);

#define sis310EnableHWCursor()\
  pSiS->HWCursorBackup[0] &= 0x0fffffff; \
  pSiS->HWCursorBackup[0] |= 0x40000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), pSiS->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(3), pSiS->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(4), pSiS->HWCursorBackup[4]);

#define sis310EnableHWARGBCursor()\
  pSiS->HWCursorBackup[0] &= 0x0FFFFFFF; \
  pSiS->HWCursorBackup[0] |= 0xE0000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), pSiS->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(3), pSiS->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(4), pSiS->HWCursorBackup[4]);

#define sis310SwitchToMONOCursor() \
  pSiS->HWCursorBackup[0] &= 0x4fffffff; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), pSiS->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(3), pSiS->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(4), pSiS->HWCursorBackup[4]);

#define sis310SwitchToRGBCursor() \
  pSiS->HWCursorBackup[0] &= 0xBFFFFFFF; \
  pSiS->HWCursorBackup[0] |= 0xA0000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), pSiS->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(3), pSiS->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(4), pSiS->HWCursorBackup[4]);

#define sis310DisableHWCursor()\
  pSiS->HWCursorBackup[0] &= 0xBFFFFFFF; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), pSiS->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(3), pSiS->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(4), pSiS->HWCursorBackup[4]);

#define sis310SetCursorBGColor(color) \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(1), (color)); \
  pSiS->HWCursorBackup[1] = color;

#define sis310SetCursorFGColor(color)\
  SIS_MMIO_OUT32(pSiS->IOBase, CS(2), (color)); \
  pSiS->HWCursorBackup[2] = color;

#define sis310SetCursorPositionX(x,preset) \
  pSiS->HWCursorBackup[3] = ((x) | ((preset) << 16)); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(3), pSiS->HWCursorBackup[3]);

#define sis310SetCursorPositionY(y,preset) \
  pSiS->HWCursorBackup[4] = ((y) | ((preset) << 16)); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(4), pSiS->HWCursorBackup[4]);

#define sis310SetCursorAddress(address)\
  pSiS->HWCursorBackup[0] &= 0xF0F00000; \
  pSiS->HWCursorBackup[0] |= address; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(0), pSiS->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(1), pSiS->HWCursorBackup[1]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(2), pSiS->HWCursorBackup[2]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(3), pSiS->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(4), pSiS->HWCursorBackup[4]);

/* 315 series CRT2 */

/* 80000000 = RGB(1) - MONO(0)
 * 40000000 = enable(1) - disable(0)
 * 20000000 = 32(1) / 16(1) bit RGB
 * 10000000 = "ghost"(1) - Alpha Blend(0)  ?
 */

#define sis301GetCursorStatus310 \
  SIS_MMIO_IN32(pSiS->IOBase, CS(8)) & 0x40000000;

#define sis301SetCursorStatus310(status) \
  pSiS->HWCursorBackup[8] &= 0xbfffffff; \
  pSiS->HWCursorBackup[8] |= status; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8),  pSiS->HWCursorBackup[8]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(11), pSiS->HWCursorBackup[11]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(12), pSiS->HWCursorBackup[12]);

#define sis301EnableHWCursor310()\
  pSiS->HWCursorBackup[8] &= 0x0fffffff; \
  pSiS->HWCursorBackup[8] |= 0x40000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8),  pSiS->HWCursorBackup[8]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(11), pSiS->HWCursorBackup[11]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(12), pSiS->HWCursorBackup[12]);

#define sis301EnableHWARGBCursor310()\
  pSiS->HWCursorBackup[8] &= 0x0FFFFFFF; \
  pSiS->HWCursorBackup[8] |= 0xE0000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8),  pSiS->HWCursorBackup[8]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(11), pSiS->HWCursorBackup[11]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(12), pSiS->HWCursorBackup[12]);

#define sis301SwitchToRGBCursor310() \
  pSiS->HWCursorBackup[8] &= 0xBFFFFFFF; \
  pSiS->HWCursorBackup[8] |= 0xA0000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8),  pSiS->HWCursorBackup[8]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(11), pSiS->HWCursorBackup[11]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(12), pSiS->HWCursorBackup[12]);

#define sis301SwitchToMONOCursor310() \
  pSiS->HWCursorBackup[8] &= 0x4fffffff; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8),  pSiS->HWCursorBackup[8]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(11), pSiS->HWCursorBackup[11]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(12), pSiS->HWCursorBackup[12]);

#define sis301DisableHWCursor310()\
  pSiS->HWCursorBackup[8] &= 0xBFFFFFFF; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8),  pSiS->HWCursorBackup[8]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(11), pSiS->HWCursorBackup[11]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(12), pSiS->HWCursorBackup[12]);

#define sis301SetCursorBGColor310(color) \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(9), (color)); \
  pSiS->HWCursorBackup[9] = color;

#define sis301SetCursorFGColor310(color) \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(10), (color)); \
  pSiS->HWCursorBackup[10] = color;

#define sis301SetCursorPositionX310(x,preset) \
  pSiS->HWCursorBackup[11] = ((x) | ((preset) << 16)); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(11), pSiS->HWCursorBackup[11]);

#define sis301SetCursorPositionY310(y,preset) \
  pSiS->HWCursorBackup[12] = ((y) | ((preset) << 16)); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(12), pSiS->HWCursorBackup[12]);

#define sis301SetCursorAddress310(address) \
  if(pSiS->ChipType == SIS_315H) { \
     if(address & 0x10000) { \
        address &= ~0x10000; \
	orSISIDXREG(SISSR, 0x37, 0x80); \
     } else { \
        andSISIDXREG(SISSR, 0x37, 0x7f); \
     } \
  } \
  pSiS->HWCursorBackup[8] &= 0xF0F00000; \
  pSiS->HWCursorBackup[8] |= address; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8),  pSiS->HWCursorBackup[8]);  \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(9),  pSiS->HWCursorBackup[9]);  \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(10), pSiS->HWCursorBackup[10]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(11), pSiS->HWCursorBackup[11]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(12), pSiS->HWCursorBackup[12]);

/* 330 series CRT2 */

/* Mono cursor engine for CRT2 on SiS330 (Xabre) has bugs
 * and cannot be used! Will hang engine.
 */

/* 80000000 = RGB(1) - MONO(0)
 * 40000000 = enable(1) - disable(0)
 * 20000000 = 32(1) / 16(1) bit RGB
 * 10000000 = "ghost"(1) - Alpha Blend(0)  ?
 */

#define sis301EnableHWCursor330() \
  /* andSISIDXREG(SISCR,0x5b,~0x10); */ \
  pSiS->HWCursorBackup[8] &= 0x0fffffff; \
  pSiS->HWCursorBackup[8] |= 0xE0000000; \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(8),  pSiS->HWCursorBackup[8]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(11), pSiS->HWCursorBackup[11]); \
  SIS_MMIO_OUT32(pSiS->IOBase, CS(12), pSiS->HWCursorBackup[12]); \
  /* orSISIDXREG(SISCR,0x5b,0x10); */



