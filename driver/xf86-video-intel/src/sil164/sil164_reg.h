/* -*- c-basic-offset: 4 -*- */
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

#ifndef SIL164_REG_H
#define SIL164_REG_H

#define SIL164_VID 0x0001
#define SIL164_DID 0x0006

#define SIL164_VID_LO 0x00
#define SIL164_VID_HI 0x01
#define SIL164_DID_LO 0x02
#define SIL164_DID_HI 0x03
#define SIL164_REV    0x04
#define SIL164_RSVD   0x05
#define SIL164_FREQ_LO 0x06
#define SIL164_FREQ_HI 0x07

#define SIL164_REG8 0x08
#define SIL164_8_VEN (1<<5)
#define SIL164_8_HEN (1<<4)
#define SIL164_8_DSEL (1<<3)
#define SIL164_8_BSEL (1<<2)
#define SIL164_8_EDGE (1<<1)
#define SIL164_8_PD   (1<<0)

#define SIL164_REG9 0x09
#define SIL164_9_VLOW (1<<7)
#define SIL164_9_MSEL_MASK (0x7<<4)
#define SIL164_9_TSEL (1<<3)
#define SIL164_9_RSEN (1<<2)
#define SIL164_9_HTPLG (1<<1)
#define SIL164_9_MDI (1<<0)

#define SIL164_REGC 0x0c

#endif
