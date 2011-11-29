/**************************************************************************

 Copyright 2006 Dave Airlie <airlied@linux.ie>
 
All Rights Reserved.

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86cmap.h"
#include "compiler.h"
#include "mibstore.h"
#include "vgaHW.h"
#include "mipointer.h"
#include "micmap.h"
#include "shadowfb.h"
#include <X11/extensions/randr.h>
#include "fb.h"
#include "miscstruct.h"
#include "xf86xv.h"
#include <X11/extensions/Xv.h>
#include "shadow.h"
#include "intel.h"
#include "i830_reg.h"

#define AIRLIED_I2C	0

#if AIRLIED_I2C

#define I2C_TIMEOUT(x)	/*(x)*/  /* Report timeouts */
#define I2C_TRACE(x)    /*(x)*/  /* Report progress */

static void i830_setscl(I2CBusPtr b, int state)
{
    ScrnInfoPtr scrn = xf86Screens[b->scrnIndex];
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t val;

    OUTREG(b->DriverPrivate.uval,
	   (state ? GPIO_CLOCK_VAL_OUT : 0) | GPIO_CLOCK_DIR_OUT |
	   GPIO_CLOCK_DIR_MASK | GPIO_CLOCK_VAL_MASK);
    val = INREG(b->DriverPrivate.uval);
}

static void i830_setsda(I2CBusPtr b, int state)
{
    ScrnInfoPtr scrn = xf86Screens[b->scrnIndex];
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t val;

    OUTREG(b->DriverPrivate.uval,
	   (state ? GPIO_DATA_VAL_OUT : 0) | GPIO_DATA_DIR_OUT |
	   GPIO_DATA_DIR_MASK | GPIO_DATA_VAL_MASK);
    val = INREG(b->DriverPrivate.uval);
}

static void i830_getscl(I2CBusPtr b, int *state)
{
    ScrnInfoPtr scrn = xf86Screens[b->scrnIndex];
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t val;

    OUTREG(b->DriverPrivate.uval, GPIO_CLOCK_DIR_IN | GPIO_CLOCK_DIR_MASK);
    OUTREG(b->DriverPrivate.uval, 0);
    val = INREG(b->DriverPrivate.uval);
    *state = ((val & GPIO_CLOCK_VAL_IN) != 0);
}

static int i830_getsda(I2CBusPtr b)
 {
     ScrnInfoPtr scrn = xf86Screens[b->scrnIndex];
     intel_screen_private *intel = intel_get_screen_private(scrn);
     uint32_t val;

     OUTREG(b->DriverPrivate.uval, GPIO_DATA_DIR_IN | GPIO_DATA_DIR_MASK);
     OUTREG(b->DriverPrivate.uval, 0);
     val = INREG(b->DriverPrivate.uval);
     return ((val & GPIO_DATA_VAL_IN) != 0);
}

static inline void sdalo(I2CBusPtr b)
{
    i830_setsda(b, 0);
    b->I2CUDelay(b, b->RiseFallTime);
}

static inline void sdahi(I2CBusPtr b)
{
    i830_setsda(b, 1);
    b->I2CUDelay(b, b->RiseFallTime);
}

static inline void scllo(I2CBusPtr b)
{
    i830_setscl(b, 0);
    b->I2CUDelay(b, b->RiseFallTime);
}

static inline int sclhi(I2CBusPtr b, int timeout)
{
    int scl = 0;
    int i;

    i830_setscl(b, 1);
    b->I2CUDelay(b, b->RiseFallTime);

    for (i = timeout; i > 0; i -= b->RiseFallTime) {
	i830_getscl(b, &scl);
	if (scl) break;
	b->I2CUDelay(b, b->RiseFallTime);
    }

    if (i <= 0) {
	I2C_TIMEOUT(ErrorF("[I2CRaiseSCL(<%s>, %d) timeout]",
			   b->BusName, timeout));
	return FALSE;
    }
    return TRUE;
}

static Bool
I830I2CGetByte(I2CDevPtr d, I2CByte *data, Bool last)
{
    I2CBusPtr b = d->pI2CBus;
    int i, sda;
    unsigned char indata = 0;

    sdahi(b);

    for (i = 0; i < 8; i++) {
	if (sclhi(b, d->BitTimeout) == FALSE) {
	    I2C_TRACE(ErrorF("timeout at bit #%d\n", 7-i));
	    return FALSE;
	};
	indata *= 2;
	if (i830_getsda(b))
	    indata |= 0x01;
	scllo(b);
    }

    if (last) {
	sdahi(b);
    } else {
	sdalo(b);
    }

    if (sclhi(b, d->BitTimeout) == FALSE) {
	sdahi(b);
	return FALSE;
    };

    scllo(b);
    sdahi(b);

    *data = indata & 0xff;
    I2C_TRACE(ErrorF("R%02x ", (int) *data));

    return TRUE;
}

static Bool
I830I2CPutByte(I2CDevPtr d, I2CByte c)
{
    Bool r;
    int i, scl, sda;
    int sb, ack;
    I2CBusPtr b = d->pI2CBus;

    for (i = 7; i >= 0; i--) {
	sb = c & (1 << i);
	i830_setsda(b, sb);
	b->I2CUDelay(b, b->RiseFallTime);

	if (sclhi(b, d->ByteTimeout) == FALSE) {
	    sdahi(b);
	    return FALSE;
	}

	i830_setscl(b, 0);
	b->I2CUDelay(b, b->RiseFallTime);
    }
    sdahi(b);
    if (sclhi(b, d->ByteTimeout) == FALSE) {
	I2C_TIMEOUT(ErrorF("[I2CPutByte(<%s>, 0x%02x, %d, %d, %d) timeout]",
			   b->BusName, c, d->BitTimeout,
			   d->ByteTimeout, d->AcknTimeout));
	return FALSE;
    }
    ack = i830_getsda(b);
    I2C_TRACE(ErrorF("Put byte 0x%02x , getsda() = %d\n", c & 0xff, ack));

    scllo(b);
    return (0 == ack);
}

static Bool
I830I2CStart(I2CBusPtr b, int timeout)
{
    if (sclhi(b, timeout) == FALSE)
	return FALSE;

    sdalo(b);
    scllo(b);

    return TRUE;
}

static void
I830I2CStop(I2CDevPtr d)
{
    I2CBusPtr b = d->pI2CBus;

    sdalo(b);
    sclhi(b, d->ByteTimeout);
    sdahi(b);
}

static Bool
I830I2CAddress(I2CDevPtr d, I2CSlaveAddr addr)
{
    if (I830I2CStart(d->pI2CBus, d->StartTimeout)) {
	if (I830I2CPutByte(d, addr & 0xFF)) {
	    if ((addr & 0xF8) != 0xF0 &&
		(addr & 0xFE) != 0x00)
		return TRUE;

	    if (I830I2CPutByte(d, (addr >> 8) & 0xFF))
		return TRUE;
	}

	I830I2CStop(d);
    }

    return FALSE;
}

#else

#define I2C_DEBUG 0

#if I2C_DEBUG
static Bool first = TRUE;
#endif

static void
i830I2CGetBits(I2CBusPtr b, int *clock, int *data)
{
    ScrnInfoPtr scrn = xf86Screens[b->scrnIndex];
    intel_screen_private *intel = intel_get_screen_private(scrn);
    uint32_t val;

    val = INREG(b->DriverPrivate.uval);

    /*
     * to read valid data, we must have written a 1 to
     * the associated bit. Writing a 1 is done by
     * tri-stating the bus in PutBits, so we needn't make
     * sure that is true here
     */
    *data = (val & GPIO_DATA_VAL_IN) != 0;
    *clock = (val & GPIO_CLOCK_VAL_IN) != 0;

#if I2C_DEBUG
    ErrorF("Getting %s:                   %c %c\n", b->BusName,
	   *clock ? '^' : 'v',
	   *data ? '^' : 'v');
#endif
}

static void
i830I2CPutBits(I2CBusPtr b, int clock, int data)
{
    uint32_t reserved = 0;
    uint32_t data_bits, clock_bits;

#if I2C_DEBUG
    int cur_clock, cur_data;
#endif

    ScrnInfoPtr scrn = xf86Screens[b->scrnIndex];
    intel_screen_private *intel = intel_get_screen_private(scrn);

#if I2C_DEBUG
    i830I2CGetBits(b, &cur_clock, &cur_data);

    if (first) {
	ErrorF("%s Debug:        C D      C D\n", b->BusName);
	first = FALSE;
    }

    ErrorF("Setting %s 0x%08x to: %c %c\n", b->BusName,
	   (int)b->DriverPrivate.uval,
	   clock ? '^' : 'v',
	   data ? '^' : 'v');
#endif

    if (!IS_I830(intel) && !IS_845G(intel)) {
	/* On most chips, these bits must be preserved in software. */
	reserved = INREG(b->DriverPrivate.uval) &
	    (GPIO_DATA_PULLUP_DISABLE | GPIO_CLOCK_PULLUP_DISABLE);
    }

    /* data or clock == 1 means to tristate the bus. otherwise, drive it low */
    if (data)
	data_bits = GPIO_DATA_DIR_IN|GPIO_DATA_DIR_MASK;
    else
	data_bits = GPIO_DATA_DIR_OUT|GPIO_DATA_DIR_MASK|GPIO_DATA_VAL_MASK;
    if (clock)
	clock_bits = GPIO_CLOCK_DIR_IN|GPIO_CLOCK_DIR_MASK;
    else
	clock_bits = GPIO_CLOCK_DIR_OUT|GPIO_CLOCK_DIR_MASK|GPIO_CLOCK_VAL_MASK;
    
    OUTREG(b->DriverPrivate.uval, reserved | data_bits | clock_bits);
    POSTING_READ(b->DriverPrivate.uval);
}

#endif

/* the i830 has a number of I2C Buses */
Bool
I830I2CInit(ScrnInfoPtr scrn, I2CBusPtr *bus_ptr, int i2c_reg, char *name)
{
    I2CBusPtr pI2CBus;
    intel_screen_private *intel = intel_get_screen_private(scrn);

    pI2CBus = xf86CreateI2CBusRec();

    if (!pI2CBus)
	return FALSE;

    pI2CBus->BusName = name;
    pI2CBus->scrnIndex = scrn->scrnIndex;
#if AIRLIED_I2C
    pI2CBus->I2CGetByte = I830I2CGetByte;
    pI2CBus->I2CPutByte = I830I2CPutByte;
    pI2CBus->I2CStart = I830I2CStart;
    pI2CBus->I2CStop = I830I2CStop;
    pI2CBus->I2CAddress = I830I2CAddress;
#else
    pI2CBus->I2CGetBits = i830I2CGetBits;
    pI2CBus->I2CPutBits = i830I2CPutBits;
#endif
    pI2CBus->DriverPrivate.uval = i2c_reg;

    /* Assume all busses are used for DDCish stuff */
    
    /* 
     * These were set incorrectly in the server pre-1.3, Having
     * duplicate settings is sub-optimal, but this lets the driver
     * work with older servers
     */
    pI2CBus->ByteTimeout = 2200; /* VESA DDC spec 3 p. 43 (+10 %) */
    pI2CBus->StartTimeout = 550;
    pI2CBus->BitTimeout = 40;
    pI2CBus->AcknTimeout = 40;
    pI2CBus->RiseFallTime = 20;

    /* Disable the GMBUS, which we won't use.  If it is left enabled (for
     * example, by Mac Mini EFI initialization), GPIO access to the pins it
     * uses gets disabled.
     */
    OUTREG(GMBUS0, 0);

    if (!xf86I2CBusInit(pI2CBus))
	return FALSE;

    *bus_ptr = pI2CBus;
    return TRUE;
}
