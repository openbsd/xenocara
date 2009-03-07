/*
 * SiSUSB hardware cursor handling definitions
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
 */

#define CS(x)   (0x8500 + (x << 2))

/* 315/330 series CRT1 */

/* 80000000 = RGB(1) - MONO(0)
 * 40000000 = enable(1) - disable(0)
 * 20000000 = 32(1) / 16(1) bit RGB
 * 10000000 = "ghost"(1) - Alpha Blend(0)
 */
 
#define sisusbGetCursorStatus \
  SIS_MMIO_IN32(pSiSUSB, pSiSUSB->IOBase, CS(0)) & 0x40000000;
  
#define sisusbSetCursorStatus(status) \
  pSiSUSB->HWCursorBackup[0] &= 0xbfffffff; \
  pSiSUSB->HWCursorBackup[0] |= status; \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(0), pSiSUSB->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(3), pSiSUSB->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(4), pSiSUSB->HWCursorBackup[4]);

#define sisusbEnableHWCursor()\
  pSiSUSB->HWCursorBackup[0] &= 0x0fffffff; \
  pSiSUSB->HWCursorBackup[0] |= 0x40000000; \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(0), pSiSUSB->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(3), pSiSUSB->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(4), pSiSUSB->HWCursorBackup[4]);
  
#define sisusbEnableHWARGBCursor()\
  pSiSUSB->HWCursorBackup[0] &= 0x0FFFFFFF; \
  pSiSUSB->HWCursorBackup[0] |= 0xE0000000; \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(0), pSiSUSB->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(3), pSiSUSB->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(4), pSiSUSB->HWCursorBackup[4]);

#define sisusbSwitchToMONOCursor() \
  pSiSUSB->HWCursorBackup[0] &= 0x4fffffff; \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(0), pSiSUSB->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(3), pSiSUSB->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(4), pSiSUSB->HWCursorBackup[4]);

#define sisusbSwitchToRGBCursor() \
  pSiSUSB->HWCursorBackup[0] &= 0xBFFFFFFF; \
  pSiSUSB->HWCursorBackup[0] |= 0xA0000000; \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(0), pSiSUSB->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(3), pSiSUSB->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(4), pSiSUSB->HWCursorBackup[4]);

#define sisusbDisableHWCursor()\
  pSiSUSB->HWCursorBackup[0] &= 0xBFFFFFFF; \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(0), pSiSUSB->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(3), pSiSUSB->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(4), pSiSUSB->HWCursorBackup[4]);
  
#define sisusbSetCursorBGColor(color) \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(1), (color)); \
  pSiSUSB->HWCursorBackup[1] = color;

#define sisusbSetCursorFGColor(color)\
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(2), (color)); \
  pSiSUSB->HWCursorBackup[2] = color;

#define sisusbSetCursorPositionX(x,preset) \
  pSiSUSB->HWCursorBackup[3] = ((x) | ((preset) << 16)); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(3), pSiSUSB->HWCursorBackup[3]);

#define sisusbSetCursorPositionY(y,preset) \
  pSiSUSB->HWCursorBackup[4] = ((y) | ((preset) << 16)); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(4), pSiSUSB->HWCursorBackup[4]);

#define sisusbSetCursorAddress(address)\
  pSiSUSB->HWCursorBackup[0] &= 0xF0F00000; \
  pSiSUSB->HWCursorBackup[0] |= address; \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(0), pSiSUSB->HWCursorBackup[0]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(1), pSiSUSB->HWCursorBackup[1]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(2), pSiSUSB->HWCursorBackup[2]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(3), pSiSUSB->HWCursorBackup[3]); \
  SIS_MMIO_OUT32(pSiSUSB, pSiSUSB->IOBase, CS(4), pSiSUSB->HWCursorBackup[4]);



