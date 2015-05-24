/* Header:   //Mercury/Projects/archives/XFree86/4.0/smi_i2c.c-arc   1.10   27 Nov 2000 15:47:58   Frido  $ */

/*
Copyright (C) 1994-2000 The XFree86 Project, Inc.  All Rights Reserved.
Copyright (C) 2000 Silicon Motion, Inc.  All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FIT-
NESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
XFREE86 PROJECT BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the names of the XFree86 Project and
Silicon Motion shall not be used in advertising or otherwise to promote the
sale, use or other dealings in this Software without prior written
authorization from the XFree86 Project and Silicon Motion.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"
#include "xf86Pci.h"
#include "vgaHW.h"

#include "smi.h"

static void
SMI_I2CPutBits(I2CBusPtr b, int clock,  int data)
{
    SMIPtr pSmi = SMIPTR(xf86Screens[b->scrnIndex]);
    unsigned int reg = 0x30;

    if (clock) reg |= 0x01;
    if (data)  reg |= 0x02;

    VGAOUT8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x72, reg);
}

static void
SMI_I2CGetBits(I2CBusPtr b, int *clock, int *data)
{
    SMIPtr pSmi = SMIPTR(xf86Screens[b->scrnIndex]);
    unsigned int reg = VGAIN8_INDEX(pSmi, VGA_SEQ_INDEX, VGA_SEQ_DATA, 0x72);

    *clock = reg & 0x04;
    *data  = reg & 0x08;
}

Bool
SMI_I2CInit(ScrnInfoPtr pScrn)
{
    SMIPtr pSmi = SMIPTR(pScrn);

    if (pSmi->I2C == NULL) {
	I2CBusPtr I2CPtr = xf86CreateI2CBusRec();
	if (I2CPtr == NULL)
	    return FALSE;

	I2CPtr->BusName    = "I2C bus";
	I2CPtr->scrnIndex  = pScrn->scrnIndex;
	I2CPtr->I2CPutBits = SMI_I2CPutBits;
	I2CPtr->I2CGetBits = SMI_I2CGetBits;

	if (!xf86I2CBusInit(I2CPtr)) {
	    xf86DestroyI2CBusRec(I2CPtr, TRUE, TRUE);
	    return FALSE;
	}

	pSmi->I2C = I2CPtr;
    }

    return TRUE;
}

