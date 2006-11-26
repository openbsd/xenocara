/*
 * Copyright 2004 The Unichrome Project  [unichrome.sf.net]
 * Copyright 1998-2003 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2003 S3 Graphics, Inc. All Rights Reserved.
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
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
/*
 * Implements three i2c busses through registers SR26, SR2c and SR31
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "via_driver.h"
#include "via_vgahw.h"

#define SDA_READ  0x04
#define SCL_READ  0x08
#define SDA_WRITE 0x10
#define SCL_WRITE 0x20

/*
 *
 * CRT I2C
 *
 */
/*
 *
 */
static void
ViaI2C1PutBits(I2CBusPtr Bus, int clock,  int data)
{
    vgaHWPtr hwp = VGAHWPTR(xf86Screens[Bus->scrnIndex]);
    CARD8 value = 0x01; /* Enable */
    
    if (clock)
        value |= SCL_WRITE;
    
    if (data)
        value |= SDA_WRITE;

    ViaSeqMask(hwp, 0x26, value, 0x01 | SCL_WRITE | SDA_WRITE);
}

/*
 *
 */
static void
ViaI2C1GetBits(I2CBusPtr Bus, int *clock, int *data)
{
    vgaHWPtr hwp = VGAHWPTR(xf86Screens[Bus->scrnIndex]);
    CARD8 value = hwp->readSeq(hwp, 0x26);
    
    *clock = (value & SCL_READ) != 0;
    *data  = (value & SDA_READ) != 0;
}

/*
 *
 */
static I2CBusPtr
ViaI2CBus1Init(int scrnIndex)
{
    I2CBusPtr pI2CBus = xf86CreateI2CBusRec();
    
    DEBUG(xf86DrvMsg(scrnIndex, X_INFO, "ViaI2CBus1Init\n"));

    if (!pI2CBus)
	return NULL;
    
    pI2CBus->BusName    = "I2C bus 1";
    pI2CBus->scrnIndex  = scrnIndex;
    pI2CBus->I2CPutBits = ViaI2C1PutBits;
    pI2CBus->I2CGetBits = ViaI2C1GetBits;

    if (!xf86I2CBusInit(pI2CBus)) {
        xf86DestroyI2CBusRec(pI2CBus, TRUE, FALSE);
        return NULL;
    }

    return pI2CBus;
}

/*
 *
 * First data bus I2C: tends to have TV-encoders
 *
 */
/*
 *
 */
static void
ViaI2C2PutBits(I2CBusPtr Bus, int clock,  int data)
{
    vgaHWPtr hwp = VGAHWPTR(xf86Screens[Bus->scrnIndex]);
    CARD8 value = 0x01; /* Enable */
    
    if (clock)
        value |= SCL_WRITE;
    
    if (data)
        value |= SDA_WRITE;

    ViaSeqMask(hwp, 0x31, value, 0x01 | SCL_WRITE | SDA_WRITE);
}

/*
 *
 */
static void
ViaI2C2GetBits(I2CBusPtr Bus, int *clock, int *data)
{
    vgaHWPtr hwp = VGAHWPTR(xf86Screens[Bus->scrnIndex]);
    CARD8 value = hwp->readSeq(hwp, 0x31);
    
    *clock = (value & SCL_READ) != 0;
    *data  = (value & SDA_READ) != 0;
}

/*
 *
 */
static I2CBusPtr
ViaI2CBus2Init(int scrnIndex)
{
    I2CBusPtr pI2CBus = xf86CreateI2CBusRec();
    
    DEBUG(xf86DrvMsg(scrnIndex, X_INFO, "ViaI2cBus2Init\n"));

    if (!pI2CBus)
	return NULL;
    
    pI2CBus->BusName    = "I2C bus 2";
    pI2CBus->scrnIndex  = scrnIndex;
    pI2CBus->I2CPutBits = ViaI2C2PutBits;
    pI2CBus->I2CGetBits = ViaI2C2GetBits;

    if (!xf86I2CBusInit(pI2CBus)) {
        xf86DestroyI2CBusRec(pI2CBus, TRUE, FALSE);
        return NULL;
    }

    return pI2CBus;
}

/*
 * A third I2C bus implemented by a few IO pins.
 * Requires higher level functions to be used properly.
 * Former via_gpioi2c.
 *
 */
/*
 *
 */
static Bool
ViaI2C3Start(I2CBusPtr b, int timeout)
{
    vgaHWPtr hwp = VGAHWPTR(xf86Screens[b->scrnIndex]);

    ViaSeqMask(hwp, 0x2C, 0xF0, 0xF0);
    b->I2CUDelay(b, b->RiseFallTime);

    ViaSeqMask(hwp, 0x2C, 0x00, 0x10);
    b->I2CUDelay(b, b->HoldTime);
    ViaSeqMask(hwp, 0x2C, 0x00, 0x20);
    b->I2CUDelay(b, b->HoldTime);

    return TRUE;
}

/*
 *
 */
static Bool
ViaI2C3Address(I2CDevPtr d, I2CSlaveAddr addr)
{
    I2CBusPtr b = d->pI2CBus;

    if (b->I2CStart(d->pI2CBus, d->StartTimeout)) {
        if (b->I2CPutByte(d, addr & 0xFF)) {
            if ((addr & 0xF8) != 0xF0 &&
                (addr & 0xFE) != 0x00)
                return TRUE;

            if (b->I2CPutByte(d, (addr >> 8) & 0xFF))
                return TRUE;
        }

        b->I2CStop(d);
    }

    return FALSE;
}

/*
 *
 */
static void
ViaI2C3Stop(I2CDevPtr d)
{
    I2CBusPtr b = d->pI2CBus;
    vgaHWPtr hwp = VGAHWPTR(xf86Screens[b->scrnIndex]);

    ViaSeqMask(hwp, 0x2C, 0xC0, 0xF0);
    b->I2CUDelay(b, b->RiseFallTime);

    ViaSeqMask(hwp, 0x2C, 0x20, 0x20);
    b->I2CUDelay(b, b->HoldTime);
    
    ViaSeqMask(hwp, 0x2C, 0x10, 0x10);
    b->I2CUDelay(b, b->HoldTime);

    ViaSeqMask(hwp, 0x2C, 0x00, 0x20);
    b->I2CUDelay(b, b->HoldTime);
}

/*
 *
 */
static void
ViaI2C3PutBit(I2CBusPtr b, Bool sda, int timeout)
{
    vgaHWPtr hwp = VGAHWPTR(xf86Screens[b->scrnIndex]);

    if (sda)
        ViaSeqMask(hwp, 0x2C, 0x50, 0x50);
    else
        ViaSeqMask(hwp, 0x2C, 0x40, 0x50);
    b->I2CUDelay(b, b->RiseFallTime/5);

    ViaSeqMask(hwp, 0x2C, 0xA0, 0xA0);
    b->I2CUDelay(b, b->HoldTime);
    b->I2CUDelay(b, timeout);

    ViaSeqMask(hwp, 0x2C, 0x80, 0xA0);
    b->I2CUDelay(b, b->RiseFallTime/5);
}

/*
 *
 */
static Bool
ViaI2C3PutByte(I2CDevPtr d, I2CByte data)
{
    I2CBusPtr b = d->pI2CBus;
    vgaHWPtr hwp = VGAHWPTR(xf86Screens[b->scrnIndex]);
    Bool ret;
    int     i;

    for (i = 7; i >= 0; i--)
        ViaI2C3PutBit(b, (data >> i) & 0x01, b->BitTimeout);

    /* raise first to avoid false positives */
    ViaSeqMask(hwp, 0x2C, 0x50, 0x50);
    ViaSeqMask(hwp, 0x2C, 0x00, 0x40);
    b->I2CUDelay(b, b->RiseFallTime);
    ViaSeqMask(hwp, 0x2C, 0xA0, 0xA0);

    if (hwp->readSeq(hwp, 0x2C) & 0x04)
        ret = FALSE;
    else
        ret = TRUE;

    ViaSeqMask(hwp, 0x2C, 0x80, 0xA0);
    b->I2CUDelay(b, b->RiseFallTime);

    return ret;
}

/*
 *
 */
static Bool
ViaI2C3GetBit(I2CBusPtr b, int timeout)
{
    vgaHWPtr hwp = VGAHWPTR(xf86Screens[b->scrnIndex]);
    Bool ret;

    ViaSeqMask(hwp, 0x2c, 0x80, 0xC0);
    b->I2CUDelay(b, b->RiseFallTime/5);
    ViaSeqMask(hwp, 0x2c, 0xA0, 0xA0);    
    b->I2CUDelay(b, 3*b->HoldTime);
    b->I2CUDelay(b, timeout);

    if (hwp->readSeq(hwp, 0x2C) & 0x04)
        ret = TRUE;
    else
        ret = FALSE;

    ViaSeqMask(hwp, 0x2C, 0x80, 0xA0);
    b->I2CUDelay(b, b->HoldTime);
    b->I2CUDelay(b, b->RiseFallTime/5);

    return ret;
}

/*
 *
 */
static Bool
ViaI2C3GetByte(I2CDevPtr d, I2CByte *data, Bool last)
{
    I2CBusPtr b = d->pI2CBus;
    vgaHWPtr hwp = VGAHWPTR(xf86Screens[b->scrnIndex]);
    int     i;

    *data = 0x00;

    for (i = 7; i >= 0; i--)
        if (ViaI2C3GetBit(b, b->BitTimeout))
            *data |= 0x01 << i;

    if (last) /* send NACK */
	ViaSeqMask(hwp, 0x2C, 0x50, 0x50);
    else /* send ACK */
	ViaSeqMask(hwp, 0x2C, 0x40, 0x50);

    ViaSeqMask(hwp, 0x2C, 0xA0, 0xA0);
    b->I2CUDelay(b, b->HoldTime);
    
    ViaSeqMask(hwp, 0x2C, 0x80, 0xA0);

    return TRUE;
}

/*
 *
 */
static I2CBusPtr
ViaI2CBus3Init(int scrnIndex)
{
    I2CBusPtr pI2CBus = xf86CreateI2CBusRec();
    
    DEBUG(xf86DrvMsg(scrnIndex, X_INFO, "ViaI2CBus3Init\n"));
    
    if (!pI2CBus)
	return NULL;
    
    pI2CBus->BusName    = "I2C bus 3";
    pI2CBus->scrnIndex  = scrnIndex;
    pI2CBus->I2CAddress = ViaI2C3Address;
    pI2CBus->I2CStart = ViaI2C3Start;
    pI2CBus->I2CStop = ViaI2C3Stop;
    pI2CBus->I2CPutByte = ViaI2C3PutByte;
    pI2CBus->I2CGetByte = ViaI2C3GetByte;
    
    pI2CBus->HoldTime = 10;
    pI2CBus->BitTimeout = 10;
    pI2CBus->ByteTimeout = 10;
    pI2CBus->StartTimeout = 10;
    
    if (!xf86I2CBusInit(pI2CBus)) {
	xf86DestroyI2CBusRec(pI2CBus, TRUE, FALSE);
	return NULL;
    }
    
    return pI2CBus;
}

#ifdef HAVE_DEBUG
/*
 *
 */
static void
ViaI2CScan(I2CBusPtr Bus)
{
    CARD8 i;

    xf86DrvMsg(Bus->scrnIndex, X_INFO, "ViaI2CScan: Scanning %s\n",
	       Bus->BusName);

    for (i = 0x10; i < 0xF0; i += 2)
	if (xf86I2CProbeAddress(Bus, i))
	    xf86DrvMsg(Bus->scrnIndex, X_PROBED, "Found slave on %s "
		       "- 0x%02X\n", Bus->BusName, i);
}
#endif

/*
 *
 *
 *
 */
void
ViaI2CInit(ScrnInfoPtr pScrn)
{
    VIAPtr pVia = VIAPTR(pScrn);

    DEBUG(xf86DrvMsg(pScrn->scrnIndex, X_INFO, "ViaI2CInit\n"));

    pVia->pI2CBus1 = ViaI2CBus1Init(pScrn->scrnIndex);
    pVia->pI2CBus2 = ViaI2CBus2Init(pScrn->scrnIndex);
    pVia->pI2CBus3 = ViaI2CBus3Init(pScrn->scrnIndex);

#ifdef HAVE_DEBUG
    if (pVia->I2CScan) {
	if (pVia->pI2CBus2)
	    ViaI2CScan(pVia->pI2CBus2);
	if (pVia->pI2CBus3)
	    ViaI2CScan(pVia->pI2CBus3);
    }
#endif
}
