/*
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

#ifndef _VIA_VT1632_H_
#define _VIA_VT1632_H_ 1

#define VIA_VT1632_I2C_ADDR 0x10

#define VIA_VT1632_VEN  0x20
#define VIA_VT1632_HEN  0x10
#define VIA_VT1632_DSEL 0x08
#define VIA_VT1632_BSEL 0x04
#define VIA_VT1632_EDGE 0x02
#define VIA_VT1632_PDB  0x01

typedef struct _VIAVT1632 {
    I2CDevPtr   VT1632I2CDev;

    uint32_t    diPort;
    CARD8       i2cBus;
    CARD8       transmitter;

    int DotclockMin;
    int DotclockMax;

    CARD8   Register08;
    CARD8   Register09;
    CARD8   Register0A;
    CARD8   Register0C;
} viaVT1632Rec, *viaVT1632RecPtr;


extern const xf86OutputFuncsRec via_vt1632_funcs;

Bool viaVT1632Probe(ScrnInfoPtr pScrn, I2CBusPtr pI2CBus);
void viaVT1632Init(ScrnInfoPtr pScrn);

#endif /* _VIA_VT1632_H_ */
