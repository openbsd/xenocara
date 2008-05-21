/* -*- c-basic-offset: 4 -*- */
/*
 * Copyright © 2007 Dave Mueller
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Dave Mueller <dave.mueller@gmx.ch>
 *
 */

#ifndef TFP410_REG_H
#define TFP410_REG_H

/* register definitions according to the TFP410 data sheet */
#define TFP410_VID 0x014C
#define TFP410_DID 0x0410

#define TFP410_VID_LO 0x00
#define TFP410_VID_HI 0x01
#define TFP410_DID_LO 0x02
#define TFP410_DID_HI 0x03
#define TFP410_REV    0x04

#define TFP410_CTL_1 0x08
#define TFP410_CTL_1_TDIS (1<<6)
#define TFP410_CTL_1_VEN (1<<5)
#define TFP410_CTL_1_HEN (1<<4)
#define TFP410_CTL_1_DSEL (1<<3)
#define TFP410_CTL_1_BSEL (1<<2)
#define TFP410_CTL_1_EDGE (1<<1)
#define TFP410_CTL_1_PD   (1<<0)

#define TFP410_CTL_2 0x09
#define TFP410_CTL_2_VLOW (1<<7)
#define TFP410_CTL_2_MSEL_MASK (0x7<<4)
#define TFP410_CTL_2_MSEL (1<<4)
#define TFP410_CTL_2_TSEL (1<<3)
#define TFP410_CTL_2_RSEN (1<<2)
#define TFP410_CTL_2_HTPLG (1<<1)
#define TFP410_CTL_2_MDI (1<<0)

#define TFP410_CTL_3 0x0A
#define TFP410_CTL_3_DK_MASK (0x7<<5)
#define TFP410_CTL_3_DK (1<<5)
#define TFP410_CTL_3_DKEN (1<<4)
#define TFP410_CTL_3_CTL_MASK (0x7<<1)
#define TFP410_CTL_3_CTL (1<<1)

#define TFP410_USERCFG 0x0B

#define TFP410_DE_DLY 0x32

#define TFP410_DE_CTL 0x33
#define TFP410_DE_CTL_DEGEN (1<<6)
#define TFP410_DE_CTL_VSPOL (1<<5)
#define TFP410_DE_CTL_HSPOL (1<<4)
#define TFP410_DE_CTL_DEDLY8 (1<<0)

#define TFP410_DE_TOP 0x34

#define TFP410_DE_CNT_LO 0x36
#define TFP410_DE_CNT_HI 0x37

#define TFP410_DE_LIN_LO 0x38
#define TFP410_DE_LIN_HI 0x39

#define TFP410_H_RES_LO 0x3A
#define TFP410_H_RES_HI 0x3B

#define TFP410_V_RES_LO 0x3C
#define TFP410_V_RES_HI 0x3D

#endif
