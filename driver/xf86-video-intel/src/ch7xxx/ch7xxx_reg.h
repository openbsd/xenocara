/**************************************************************************

 Copyright 2006 Dave Airlie <airlied@linux.ie>
 
Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
on the rights to use, copy, modify, merge, publish, distribute, sub
license, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice (including the next
paragraph) shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
THE COPYRIGHT HOLDERS AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

#ifndef CH7xxx_REG_H
#define CH7xxx_REG_H

#define CH7xxx_REG_VID 0x4a
#define CH7xxx_REG_DID 0x4b

#define CH7011_VID 0x83 /* 7010 as well */
#define CH7009A_VID 0x84
#define CH7009B_VID 0x85
#define CH7301_VID 0x95

#define CH7xxx_VID 0x84
#define CH7xxx_DID 0x17

#define CH7xxx_NUM_REGS 0x4c

#define CH7xxx_CM 0x1C
#define CH7xxx_CM_XCM (1<<0)
#define CH7xxx_CM_MCP (1<<2)
#define CH7xxx_INPUT_CLOCK 0x1D
#define CH7xxx_GPIO 0x1E
#define CH7xxx_GPIO_HPIR (1<<3)
#define CH7xxx_IDF 0x1F

#define CH7xxx_IDF_HSP (1<<3)
#define CH7xxx_IDF_VSP (1<<4)

#define CH7xxx_CONNECTION_DETECT 0x20
#define CH7xxx_CDET_DVI (1<<5)

#define CH7301_DAC_CNTL 0x21
#define CH7301_HOTPLUG 0x23
#define CH7xxx_TCTL 0x31
#define CH7xxx_TVCO 0x32
#define CH7xxx_TPCP 0x33
#define CH7xxx_TPD 0x34
#define CH7xxx_TPVT 0x35
#define CH7xxx_TLPF 0x36
#define CH7xxx_TCT 0x37
#define CH7301_TEST_PATTERN 0x48
#define CH7xxx_PM 0x49

#define CH7xxx_PM_FPD (1<<0)
#define CH7301_PM_DACPD0 (1<<1)
#define CH7301_PM_DACPD1 (1<<2)
#define CH7301_PM_DACPD2 (1<<3)
#define CH7xxx_PM_DVIL (1<<6)
#define CH7xxx_PM_DVIP (1<<7)

#define CH7301_SYNC_POLARITY 0x56

#define CH7301_SYNC_RGB_YUV (1<<0)
#define CH7301_SYNC_POL_DVI (1<<5)

#endif
