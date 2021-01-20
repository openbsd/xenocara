/*
 * Copyright 2016 Kevin Brace
 * Copyright 2016 The OpenChrome Project
 *                [https://www.freedesktop.org/wiki/Openchrome]
 * Copyright 2014 SHS SERVICES GmbH
 * Copyright 2006-2009 Luc Verhaegen.
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef _VIA_SII164_H_
#define _VIA_SII164_H_ 1

#define VIA_SII164_VEN  0x20
#define VIA_SII164_HEN  0x10
#define VIA_SII164_DSEL 0x08
#define VIA_SII164_BSEL 0x04
#define VIA_SII164_EDGE 0x02
#define VIA_SII164_PDB  0x01

typedef struct _viaSiI164 {
    I2CDevPtr pSiI164I2CDev;

    uint32_t diPort;
    uint8_t i2cBus;
    uint8_t transmitter;

	int DotclockMin;
	int DotclockMax;
	CARD8 Register08;
	CARD8 Register09;
	CARD8 Register0A;
	CARD8 Register0C;
} viaSiI164Rec, *viaSiI164RecPtr;


extern const xf86OutputFuncsRec via_sii164_funcs;

Bool viaSiI164Probe(ScrnInfoPtr pScrn, I2CBusPtr pI2CBus);
void viaSiI164Init(ScrnInfoPtr pScrn);

#endif /* _VIA_SII164_H_ */
