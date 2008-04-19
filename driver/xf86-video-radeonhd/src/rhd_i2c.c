/*
 * Copyright 2007, 2008  Egbert Eich   <eich@novell.com>
 * Copyright 2007, 2008  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007, 2008  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007, 2008  Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86i2c.h"
#if HAVE_XF86_ANSIC_H
# include "xf86_ansic.h"
#else
# include <unistd.h>
# include <stdio.h>
#endif

#include "rhd.h"
#include "rhd_i2c.h"
#include "rhd_regs.h"

#ifdef ATOM_BIOS
#include "rhd_atombios.h"
#endif

typedef struct _rhdI2CRec
{
    CARD16 prescale;
    CARD8 line;
    int scrnIndex;
} rhdI2CRec;

enum _rhdR6xxI2CBits {
    /* R6_DC_I2C_TRANSACTION0 */
    R6_DC_I2C_RW0   = (0x1 << 0),
    R6_DC_I2C_STOP_ON_NACK0         = (0x1 << 8),
    R6_DC_I2C_ACK_ON_READ0  = (0x1 << 9),
    R6_DC_I2C_START0        = (0x1 << 12),
    R6_DC_I2C_STOP0         = (0x1 << 13),
    R6_DC_I2C_COUNT0        = (0xff << 16),
    /* R6_DC_I2C_TRANSACTION1 */
    R6_DC_I2C_RW1   = (0x1 << 0),
    R6_DC_I2C_STOP_ON_NACK1         = (0x1 << 8),
    R6_DC_I2C_ACK_ON_READ1  = (0x1 << 9),
    R6_DC_I2C_START1        = (0x1 << 12),
    R6_DC_I2C_STOP1         = (0x1 << 13),
    R6_DC_I2C_COUNT1        = (0xff << 16),
    /* R6_DC_I2C_DATA */
    R6_DC_I2C_DATA_RW       = (0x1 << 0),
    R6_DC_I2C_DATA_BIT      = (0xff << 8),
    R6_DC_I2C_INDEX         = (0xff << 16),
    R6_DC_I2C_INDEX_WRITE   = (0x1 << 31),
    /* R6_DC_I2C_CONTROL */
    R6_DC_I2C_GO    = (0x1 << 0),
    R6_DC_I2C_SOFT_RESET    = (0x1 << 1),
    R6_DC_I2C_SEND_RESET    = (0x1 << 2),
    R6_DC_I2C_SW_STATUS_RESET       = (0x1 << 3),
    R6_DC_I2C_SDVO_EN       = (0x1 << 4),
    R6_DC_I2C_SDVO_ADDR_SEL         = (0x1 << 6),
    R6_DC_I2C_DDC_SELECT    = (0x7 << 8),
    R6_DC_I2C_TRANSACTION_COUNT     = (0x3 << 20),
    R6_DC_I2C_SW_DONE_INT   = (0x1 << 0),
    R6_DC_I2C_SW_DONE_ACK   = (0x1 << 1),
    R6_DC_I2C_SW_DONE_MASK  = (0x1 << 2),
    R6_DC_I2C_DDC1_HW_DONE_INT      = (0x1 << 4),
    R6_DC_I2C_DDC1_HW_DONE_ACK      = (0x1 << 5),
    R6_DC_I2C_DDC1_HW_DONE_MASK     = (0x1 << 6),
    R6_DC_I2C_DDC2_HW_DONE_INT      = (0x1 << 8),
    R6_DC_I2C_DDC2_HW_DONE_ACK      = (0x1 << 9),
    R6_DC_I2C_DDC2_HW_DONE_MASK     = (0x1 << 10),
    R6_DC_I2C_DDC3_HW_DONE_INT      = (0x1 << 12),
    R6_DC_I2C_DDC3_HW_DONE_ACK      = (0x1 << 13),
    R6_DC_I2C_DDC3_HW_DONE_MASK     = (0x1 << 14),
    R6_DC_I2C_DDC4_HW_DONE_INT      = (0x1 << 16),
    R6_DC_I2C_DDC4_HW_DONE_ACK      = (0x1 << 17),
    R6_DC_I2C_DDC4_HW_DONE_MASK     = (0x1 << 18),
    /* R6_DC_I2C_SW_STATUS */
    R6_DC_I2C_SW_STATUS_BIT         = (0x3 << 0),
    R6_DC_I2C_SW_DONE       = (0x1 << 2),
    R6_DC_I2C_SW_ABORTED    = (0x1 << 4),
    R6_DC_I2C_SW_TIMEOUT    = (0x1 << 5),
    R6_DC_I2C_SW_INTERRUPTED        = (0x1 << 6),
    R6_DC_I2C_SW_BUFFER_OVERFLOW    = (0x1 << 7),
    R6_DC_I2C_SW_STOPPED_ON_NACK    = (0x1 << 8),
    R6_DC_I2C_SW_SDVO_NACK  = (0x1 << 10),
    R6_DC_I2C_SW_NACK0      = (0x1 << 12),
    R6_DC_I2C_SW_NACK1      = (0x1 << 13),
    R6_DC_I2C_SW_NACK2      = (0x1 << 14),
    R6_DC_I2C_SW_NACK3      = (0x1 << 15),
    R6_DC_I2C_SW_REQ        = (0x1 << 18)
};

enum _rhdR5xxI2CBits {
 /* R5_DC_I2C_STATUS1 */
    R5_DC_I2C_DONE	 = (0x1 << 0),
    R5_DC_I2C_NACK	 = (0x1 << 1),
    R5_DC_I2C_HALT	 = (0x1 << 2),
    R5_DC_I2C_GO	 = (0x1 << 3),
 /* R5_DC_I2C_RESET */
    R5_DC_I2C_SOFT_RESET	 = (0x1 << 0),
    R5_DC_I2C_ABORT	 = (0x1 << 8),
 /* R5_DC_I2C_CONTROL1 */
    R5_DC_I2C_START	 = (0x1 << 0),
    R5_DC_I2C_STOP	 = (0x1 << 1),
    R5_DC_I2C_RECEIVE	 = (0x1 << 2),
    R5_DC_I2C_EN	 = (0x1 << 8),
    R5_DC_I2C_PIN_SELECT	 = (0x3 << 16),
 /* R5_DC_I2C_CONTROL2 */
    R5_DC_I2C_ADDR_COUNT	 = (0x7 << 0),
    R5_DC_I2C_DATA_COUNT	 = (0xf << 8),
    R5_DC_I2C_PRESCALE_LOWER	 = (0xff << 16),
    R5_DC_I2C_PRESCALE_UPPER	 = (0xff << 24),
 /* R5_DC_I2C_CONTROL3 */
    R5_DC_I2C_DATA_DRIVE_EN	 = (0x1 << 0),
    R5_DC_I2C_DATA_DRIVE_SEL	 = (0x1 << 1),
    R5_DC_I2C_CLK_DRIVE_EN	 = (0x1 << 7),
    R5_DC_I2C_RD_INTRA_BYTE_DELAY	 = (0xff << 8),
    R5_DC_I2C_WR_INTRA_BYTE_DELAY	 = (0xff << 16),
    R5_DC_I2C_TIME_LIMIT	 = (0xff << 24),
 /* R5_DC_I2C_DATA */
    R5_DC_I2C_DATA_BIT	 = (0xff << 0),
 /* R5_DC_I2C_INTERRUPT_CONTROL */
    R5_DC_I2C_INTERRUPT_STATUS	 = (0x1 << 0),
    R5_DC_I2C_INTERRUPT_AK	 = (0x1 << 8),
    R5_DC_I2C_INTERRUPT_ENABLE	 = (0x1 << 16),
 /* R5_DC_I2C_ARBITRATION */
    R5_DC_I2C_SW_WANTS_TO_USE_I2C	 = (0x1 << 0),
    R5_DC_I2C_SW_CAN_USE_I2C	 = (0x1 << 1),
    R5_DC_I2C_SW_DONE_USING_I2C	 = (0x1 << 8),
    R5_DC_I2C_HW_NEEDS_I2C	 = (0x1 << 9),
    R5_DC_I2C_ABORT_HDCP_I2C	 = (0x1 << 16),
    R5_DC_I2C_HW_USING_I2C	 = (0x1 << 17)
};

enum _rhdRS69I2CBits {
    /* RS69_DC_I2C_TRANSACTION0 */
    RS69_DC_I2C_RW0   = (0x1 << 0),
    RS69_DC_I2C_STOP_ON_NACK0         = (0x1 << 8),
    RS69_DC_I2C_START0        = (0x1 << 12),
    RS69_DC_I2C_STOP0         = (0x1 << 13),
    /* RS69_DC_I2C_TRANSACTION1 */
    RS69_DC_I2C_RW1   = (0x1 << 0),
    RS69_DC_I2C_START1        = (0x1 << 12),
    RS69_DC_I2C_STOP1         = (0x1 << 13),
    /* RS69_DC_I2C_DATA */
    RS69_DC_I2C_DATA_RW       = (0x1 << 0),
    RS69_DC_I2C_INDEX_WRITE   = (0x1 << 31),
    /* RS69_DC_I2C_CONTROL */
    RS69_DC_I2C_GO    = (0x1 << 0),
    RS69_DC_I2C_TRANSACTION_COUNT     = (0x3 << 20),
    RS69_DC_I2C_SW_DONE_ACK   = (0x1 << 1),
    /* RS69_DC_I2C_SW_STATUS */
    RS69_DC_I2C_SW_DONE       = (0x1 << 2),
    RS69_DC_I2C_SW_STOPPED_ON_NACK    = (0x1 << 8),
    RS69_DC_I2C_SW_NACK0      = (0x1 << 12),
    RS69_DC_I2C_SW_NACK1      = (0x1 << 13)
};

/* RV620 */
enum rv620I2CBits {
    /* GENERIC_I2C_CONTROL */
    RV62_DC_I2C_GO    = (0x1 << 0),
    RV62_GENERIC_I2C_GO       = (0x1 << 0),
    RV62_GENERIC_I2C_SOFT_RESET       = (0x1 << 1),
    RV62_GENERIC_I2C_SEND_RESET       = (0x1 << 2),
    /* GENERIC_I2C_INTERRUPT_CONTROL */
    RV62_GENERIC_I2C_DONE_INT         = (0x1 << 0),
    RV62_GENERIC_I2C_DONE_ACK         = (0x1 << 1),
    RV62_GENERIC_I2C_DONE_MASK        = (0x1 << 2),
    /* GENERIC_I2C_STATUS */
    RV62_GENERIC_I2C_STATUS_BIT       = (0xf << 0),
    RV62_GENERIC_I2C_DONE     = (0x1 << 4),
    RV62_GENERIC_I2C_ABORTED  = (0x1 << 5),
    RV62_GENERIC_I2C_TIMEOUT  = (0x1 << 6),
    RV62_GENERIC_I2C_STOPPED_ON_NACK  = (0x1 << 9),
    RV62_GENERIC_I2C_NACK     = (0x1 << 10),
    /* GENERIC_I2C_SPEED */
    RV62_GENERIC_I2C_THRESHOLD        = (0x3 << 0),
    RV62_GENERIC_I2C_DISABLE_FILTER_DURING_STALL      = (0x1 << 4),
    RV62_GENERIC_I2C_PRESCALE         = (0xffff << 16),
    /* GENERIC_I2C_SETUP */
    RV62_GENERIC_I2C_DATA_DRIVE_EN    = (0x1 << 0),
    RV62_GENERIC_I2C_DATA_DRIVE_SEL   = (0x1 << 1),
    RV62_GENERIC_I2C_CLK_DRIVE_EN     = (0x1 << 7),
    RV62_GENERIC_I2C_INTRA_BYTE_DELAY         = (0xff << 8),
    RV62_GENERIC_I2C_TIME_LIMIT       = (0xff << 24),
    /* GENERIC_I2C_TRANSACTION */
    RV62_GENERIC_I2C_RW       = (0x1 << 0),
    RV62_GENERIC_I2C_STOP_ON_NACK     = (0x1 << 8),
    RV62_GENERIC_I2C_ACK_ON_READ      = (0x1 << 9),
    RV62_GENERIC_I2C_START    = (0x1 << 12),
    RV62_GENERIC_I2C_STOP     = (0x1 << 13),
    RV62_GENERIC_I2C_COUNT    = (0xf << 16),
    /* GENERIC_I2C_DATA */
    RV62_GENERIC_I2C_DATA_RW  = (0x1 << 0),
    RV62_GENERIC_I2C_DATA_BIT         = (0xff << 8),
    RV62_GENERIC_I2C_INDEX    = (0xf << 16),
    RV62_GENERIC_I2C_INDEX_WRITE      = (0x1 << 31),
    /* GENERIC_I2C_PIN_SELECTION */
    RV62_GENERIC_I2C_SCL_PIN_SEL      = (0x7f << 0),
    RV62_GENERIC_I2C_SDA_PIN_SEL      = (0x7f << 8)
};

/* R5xx */
static Bool
rhd5xxI2CSetupStatus(I2CBusPtr I2CPtr, int line)
{
    line &= 0xf;

    RHDFUNC(I2CPtr);

    switch (line) {
	case 0:
	    RHDRegMask(I2CPtr, R5_DC_GPIO_DDC1_MASK, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R5_DC_GPIO_DDC1_A, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC1_EN, 0x0, 0xffff);
	    break;
	case 1:
	    RHDRegMask(I2CPtr, R5_DC_GPIO_DDC2_MASK, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R5_DC_GPIO_DDC2_A, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC2_EN, 0x0, 0xffff);
	    break;
	case 2:
	    RHDRegMask(I2CPtr, R5_DC_GPIO_DDC3_MASK, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R5_DC_GPIO_DDC3_A, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R5_DC_GPIO_DDC3_EN, 0x0, 0xffff);
	    break;
	default:
	    xf86DrvMsg(I2CPtr->scrnIndex,X_ERROR,
		       "%s: Trying to initialize non-existent I2C line: %i\n",
		       __func__,line);
	    return FALSE;
    }
    return TRUE;
}

static Bool
rhd5xxI2CStatus(I2CBusPtr I2CPtr)
{
    int count = 5000;
    CARD32 res;

    RHDFUNC(I2CPtr);

    while (count-- != 0) {
	usleep (10);
	if (((RHDRegRead(I2CPtr, R5_DC_I2C_STATUS1)) & R5_DC_I2C_GO) != 0)
	    continue;
	res = RHDRegRead(I2CPtr, R5_DC_I2C_STATUS1);
	RHDDebugVerb(I2CPtr->scrnIndex,1,"SW_STATUS: 0x%x %i\n",
		     (unsigned int)res,count);
	if (res & R5_DC_I2C_DONE)
	    return TRUE;
	else
	    return FALSE;
    }
    RHDRegMask(I2CPtr, R5_DC_I2C_RESET, R5_DC_I2C_ABORT, 0xff00);
    return FALSE;
}

Bool
rhd5xxWriteReadChunk(I2CDevPtr i2cDevPtr, I2CByte *WriteBuffer,
		int nWrite, I2CByte *ReadBuffer, int nRead)
{
    I2CSlaveAddr slave = i2cDevPtr->SlaveAddr;
    rhdI2CPtr I2C = (rhdI2CPtr)(i2cDevPtr->pI2CBus->DriverPrivate.ptr);
    I2CBusPtr I2CPtr = i2cDevPtr->pI2CBus;
    CARD8 line = I2C->line;
    int prescale = I2C->prescale;
    CARD32 save_I2C_CONTROL1, save_494;
    CARD32  tmp32;
    Bool ret = TRUE;

    RHDFUNC(i2cDevPtr->pI2CBus);

    RHDRegMask(I2CPtr, 0x28, 0x200, 0x200);
    save_I2C_CONTROL1 = RHDRegRead(I2CPtr, R5_DC_I2C_CONTROL1);
    save_494 = RHDRegRead(I2CPtr, 0x494);
    RHDRegMask(I2CPtr, 0x494, 1, 1);
    RHDRegMask(I2CPtr, R5_DC_I2C_ARBITRATION,
	       R5_DC_I2C_SW_WANTS_TO_USE_I2C,
	       R5_DC_I2C_SW_WANTS_TO_USE_I2C);

    if (!RHDRegRead(I2CPtr, R5_DC_I2C_ARBITRATION) & R5_DC_I2C_SW_CAN_USE_I2C) {
	RHDDebug(I2CPtr->scrnIndex, "%s SW cannot use I2C line %i\n",__func__,line);
	ret = FALSE;
    } else {

	RHDRegMask(I2CPtr, R5_DC_I2C_STATUS1, R5_DC_I2C_DONE
		   | R5_DC_I2C_NACK
		   | R5_DC_I2C_HALT, 0xff);
	RHDRegMask(I2CPtr, R5_DC_I2C_RESET, R5_DC_I2C_SOFT_RESET, 0xffff);
	RHDRegWrite(I2CPtr, R5_DC_I2C_RESET, 0);

	RHDRegMask(I2CPtr, R5_DC_I2C_CONTROL1,
		   (line  & 0x0f) << 16 | R5_DC_I2C_EN,
		   R5_DC_I2C_PIN_SELECT | R5_DC_I2C_EN);
    }

    if (ret && (nWrite || !nRead)) { /* special case for bus probing */
	/*
	 * chip can't just write the slave address without data.
	 * Add a dummy byte.
	 */
	RHDRegWrite(I2CPtr, R5_DC_I2C_CONTROL2,
		    prescale << 16 |
		    (nWrite ? nWrite : 1) << 8 | 0x01); /* addr_cnt: 1 */
	RHDRegMask(I2CPtr, R5_DC_I2C_CONTROL3,
		   0x30 << 24, 0xff << 24); /* time limit 30 */

	RHDRegWrite(I2CPtr, R5_DC_I2C_DATA, slave);

	/* Add dummy byte */
	if (!nWrite)
	    RHDRegWrite(I2CPtr, R5_DC_I2C_DATA, 0);
	else
	    while (nWrite--)
		RHDRegWrite(I2CPtr, R5_DC_I2C_DATA, *WriteBuffer++);

	RHDRegMask(I2CPtr, R5_DC_I2C_CONTROL1,
		   R5_DC_I2C_START | R5_DC_I2C_STOP, 0xff);
	RHDRegMask(I2CPtr, R5_DC_I2C_STATUS1, R5_DC_I2C_GO, 0xff);

	if ((ret = rhd5xxI2CStatus(I2CPtr)))
	    RHDRegMask(I2CPtr, R5_DC_I2C_STATUS1,R5_DC_I2C_DONE, 0xff);
	else
	    ret = FALSE;
    }

    if (ret && nRead) {

	RHDRegWrite(I2CPtr, R5_DC_I2C_DATA, slave | 1); /*slave*/
	RHDRegWrite(I2CPtr, R5_DC_I2C_CONTROL2,
		    prescale << 16 | nRead << 8 | 0x01); /* addr_cnt: 1 */

	RHDRegMask(I2CPtr, R5_DC_I2C_CONTROL1,
		   R5_DC_I2C_START | R5_DC_I2C_STOP | R5_DC_I2C_RECEIVE, 0xff);
	RHDRegMask(I2CPtr, R5_DC_I2C_STATUS1, R5_DC_I2C_GO, 0xff);
	if ((ret = rhd5xxI2CStatus(I2CPtr))) {
	    RHDRegMask(I2CPtr, R5_DC_I2C_STATUS1, R5_DC_I2C_DONE, 0xff);
	    while (nRead--) {
		*(ReadBuffer++) = (CARD8)RHDRegRead(I2CPtr, R5_DC_I2C_DATA);
	    }
	} else
	    ret = FALSE;
    }

    RHDRegMask(I2CPtr, R5_DC_I2C_STATUS1,
	       R5_DC_I2C_DONE | R5_DC_I2C_NACK | R5_DC_I2C_HALT, 0xff);
    RHDRegMask(I2CPtr, R5_DC_I2C_RESET, R5_DC_I2C_SOFT_RESET, 0xff);
    RHDRegWrite(I2CPtr,R5_DC_I2C_RESET, 0);

    RHDRegMask(I2CPtr,R5_DC_I2C_ARBITRATION,
	       R5_DC_I2C_SW_DONE_USING_I2C, 0xff00);

    RHDRegWrite(I2CPtr,R5_DC_I2C_CONTROL1, save_I2C_CONTROL1);
    RHDRegWrite(I2CPtr,0x494, save_494);
    tmp32 = RHDRegRead(I2CPtr,0x28);
    RHDRegWrite(I2CPtr,0x28, tmp32 & 0xfffffdff);

    return ret;
}

static Bool
rhd5xxWriteRead(I2CDevPtr i2cDevPtr, I2CByte *WriteBuffer, int nWrite, I2CByte *ReadBuffer, int nRead)
{
    /*
     * Since the transaction buffer can only hold
     * 15 bytes (+ the slave address) we bail out
     * on every transaction that is bigger unless
     * it's a read transaction following a write
     * transaction sending just one byte.
     * In this case we assume, that this byte is
     * an offset address. Thus we will restart
     * the transaction after 15 bytes sending
     * a new offset.
     */

    I2CBusPtr I2CPtr = i2cDevPtr->pI2CBus;

    RHDFUNC(I2CPtr);

    if (nWrite > 15 || (nRead > 15 && nWrite != 1)) {
	xf86DrvMsg(i2cDevPtr->pI2CBus->scrnIndex,X_ERROR,
		   "%s: Currently only I2C transfers with "
		   "maximally 15bytes are supported\n",
		   __func__);
	return FALSE;
    }
    rhd5xxI2CSetupStatus(I2CPtr, ((rhdI2CPtr)(I2CPtr->DriverPrivate.ptr))->line);

    if (nRead > 15) {
	I2CByte offset = *WriteBuffer;
	while (nRead) {
	    int n = nRead > 15 ? 15 : nRead;
	    if (!rhd5xxWriteReadChunk(i2cDevPtr, &offset, 1, ReadBuffer, n))
		return FALSE;
	    ReadBuffer += n;
	    nRead -= n;
	    offset += n;
	}
	return TRUE;
    } else
	return rhd5xxWriteReadChunk(i2cDevPtr, WriteBuffer, nWrite,
	    ReadBuffer, nRead);
}

/* RS690 */
static Bool
rhdRS69I2CStatus(I2CBusPtr I2CPtr)
{
    int count = 5000;
    volatile CARD32 val;

    RHDFUNC(I2CPtr);

    while (--count) {

	usleep(10);
	val = RHDRegRead(I2CPtr, RS69_DC_I2C_SW_STATUS);
	RHDDebugVerb(I2CPtr->scrnIndex,1,"SW_STATUS: 0x%x %i\n",(unsigned int)val,count);
	if (val & RS69_DC_I2C_SW_DONE)
	    break;
    }
    RHDRegMask(I2CPtr, RS69_DC_I2C_INTERRUPT_CONTROL, RS69_DC_I2C_SW_DONE_ACK,
	       RS69_DC_I2C_SW_DONE_ACK);
    if (!count || (val & (RS69_DC_I2C_SW_STOPPED_ON_NACK | RS69_DC_I2C_SW_NACK0 | RS69_DC_I2C_SW_NACK1 | 0x3)))
	return FALSE; /* 2 */
    return TRUE; /* 1 */
}

static Bool
rhdRS69I2CSetupStatus(I2CBusPtr I2CPtr, int line, int prescale)
{
    CARD32 ddc;
    unsigned int clk_line = 0; /* invalid clk register */

    RHDFUNC(I2CPtr);

#ifdef ATOM_BIOS
    {
	RHDPtr rhdPtr = RHDPTR(xf86Screens[I2CPtr->scrnIndex]);
	AtomBiosArgRec atomBiosArg;

	atomBiosArg.val = line & 0xf;
	if (ATOM_SUCCESS != RHDAtomBiosFunc(rhdPtr->scrnIndex,
					    rhdPtr->atomBIOS,
					    ATOM_GPIO_I2C_CLK_MASK,
					    &atomBiosArg))
	    return FALSE;
	clk_line = atomBiosArg.val;
#endif /* ATOM_BIOS */

	/* add SDVO handling later */
	switch (clk_line) {
	    case 0x1f90:
		ddc = 0; /* ddc1 */
		break;
	    case 0x1f94: /* ddc2 */
		ddc = 1;
		break;
	    case 0x1f98: /* ddc3 */
		ddc = 2;
		break;
	    default:
		xf86DrvMsg(I2CPtr->scrnIndex, X_ERROR, "Invalid ClkLine for DDC. "
			   "AtomBIOS reported wrong or AtomBIOS unavailable\n");
		return FALSE;
	}

	RHDDebug(I2CPtr->scrnIndex, "%s: DDC Line: %i val: %i port: 0x%x\n",
		 __func__, line & 0xf, ddc, atomBiosArg.val);
    }

    RHDRegMask(I2CPtr, 0x28, 0x200, 0x200);
    RHDRegMask(I2CPtr, RS69_DC_I2C_UNKNOWN_1, prescale << 16 | 0x2, 0xffff00ff);
    RHDRegWrite(I2CPtr, RS69_DC_I2C_DDC_SETUP_Q, 0x30000000);
    RHDRegMask(I2CPtr, RS69_DC_I2C_CONTROL, ((line & 0x3) << 16) | (ddc << 8), 0xffff00);
    RHDRegMask(I2CPtr, RS69_DC_I2C_INTERRUPT_CONTROL, 0x2, 0x2);
    RHDRegMask(I2CPtr, RS69_DC_I2C_UNKNOWN_2, 0x2, 0xff);

    return TRUE;
}

static Bool
rhdRS69WriteRead(I2CDevPtr i2cDevPtr, I2CByte *WriteBuffer,
		 int nWrite, I2CByte *ReadBuffer, int nRead)
{
    Bool ret = FALSE;
    CARD32 data = 0;
    I2CBusPtr I2CPtr = i2cDevPtr->pI2CBus;
    I2CSlaveAddr slave = i2cDevPtr->SlaveAddr;
    rhdI2CPtr I2C = (rhdI2CPtr)I2CPtr->DriverPrivate.ptr;
    CARD8 line = I2C->line;
    int prescale = I2C->prescale;
    int idx = 1;

    enum {
	TRANS_WRITE_READ,
	TRANS_WRITE,
	TRANS_READ
    } trans;

    RHDFUNC(i2cDevPtr->pI2CBus);

    if (nWrite > 0 && nRead > 0) {
	trans = TRANS_WRITE_READ;
    } else if (nWrite > 0) {
	trans = TRANS_WRITE;
    } else if (nRead > 0) {
	trans = TRANS_READ;
    } else {
	/* for bus probing */
	trans = TRANS_WRITE;
    }
    if (slave & 0xff00) {
	xf86DrvMsg(I2CPtr->scrnIndex,X_ERROR,
		   "%s: 10 bit I2C slave addresses not supported\n",__func__);
	return FALSE;
    }

    if (!rhdRS69I2CSetupStatus(I2CPtr, line,  prescale))
	return FALSE;

    RHDRegMask(I2CPtr, RS69_DC_I2C_CONTROL, (trans == TRANS_WRITE_READ)
	       ? (1 << 20) : 0, RS69_DC_I2C_TRANSACTION_COUNT); /* 2 or 1 Transaction */
    RHDRegMask(I2CPtr, RS69_DC_I2C_TRANSACTION0,
	       RS69_DC_I2C_STOP_ON_NACK0
	       | (trans == TRANS_READ ? RS69_DC_I2C_RW0 : 0)
	       | RS69_DC_I2C_START0
	       | (trans == TRANS_WRITE_READ ? 0 : RS69_DC_I2C_STOP0 )
	       | ((trans == TRANS_READ ? nRead : nWrite)  << 16),
	       0xffffff);
    if (trans == TRANS_WRITE_READ)
	RHDRegMask(I2CPtr, RS69_DC_I2C_TRANSACTION1,
		   nRead << 16
		   | RS69_DC_I2C_RW1
		   | RS69_DC_I2C_START1
		   | RS69_DC_I2C_STOP1,
		   0xffffff); /* <bytes> read */

    data = RS69_DC_I2C_INDEX_WRITE
	| (((slave & 0xfe) | (trans == TRANS_READ ? 1 : 0)) << 8 )
	| (0 << 16);
    RHDRegWrite(I2CPtr, RS69_DC_I2C_DATA, data);
    if (trans != TRANS_READ) { /* we have bytes to write */
	while (nWrite--) {
	    data = RS69_DC_I2C_INDEX_WRITE | ( *(WriteBuffer++) << 8 )
		| (idx++ << 16);
	    RHDRegWrite(I2CPtr, RS69_DC_I2C_DATA, data);
	}
    }
    if (trans == TRANS_WRITE_READ) { /* we have bytes to read after write */
	data = RS69_DC_I2C_INDEX_WRITE | ((slave | 0x1) << 8) | (idx++ << 16);
	RHDRegWrite(I2CPtr, RS69_DC_I2C_DATA, data);
    }
    /* Go! */
    RHDRegMask(I2CPtr, RS69_DC_I2C_CONTROL, RS69_DC_I2C_GO, RS69_DC_I2C_GO);
    if (rhdRS69I2CStatus(I2CPtr)) {
	/* Hopefully this doesn't write data to index */
	RHDRegWrite(I2CPtr, RS69_DC_I2C_DATA, RS69_DC_I2C_INDEX_WRITE
		    | RS69_DC_I2C_DATA_RW  | /* idx++ */3 << 16);
	while (nRead--) {
	    data = RHDRegRead(I2CPtr, RS69_DC_I2C_DATA);
	    *(ReadBuffer++) = (data >> 8) & 0xff;
	}
	ret = TRUE;
    }

    RHDRegMask(I2CPtr, RS69_DC_I2C_CONTROL, 0x2, 0xff);
    usleep(10);
    RHDRegWrite(I2CPtr, RS69_DC_I2C_CONTROL, 0);

    return ret;
}


/* R6xx */
static Bool
rhdR6xxI2CStatus(I2CBusPtr I2CPtr)
{
    int count = 5000;
    volatile CARD32 val;

    RHDFUNC(I2CPtr);

    while (--count) {

	usleep(10);
	val = RHDRegRead(I2CPtr, R6_DC_I2C_SW_STATUS);
	RHDDebugVerb(I2CPtr->scrnIndex,1,"SW_STATUS: 0x%x %i\n",(unsigned int)val,count);
	if (val & R6_DC_I2C_SW_DONE)
	    break;
    }
    RHDRegMask(I2CPtr, R6_DC_I2C_INTERRUPT_CONTROL, R6_DC_I2C_SW_DONE_ACK,
	       R6_DC_I2C_SW_DONE_ACK);
    if (!count || (val & (R6_DC_I2C_SW_STOPPED_ON_NACK | R6_DC_I2C_SW_NACK0 | R6_DC_I2C_SW_NACK1 | 0x3)))
	return FALSE; /* 2 */
    return TRUE; /* 1 */
}

static Bool
rhd6xxI2CSetupStatus(I2CBusPtr I2CPtr, int line, int prescale)
{
    line &= 0xf;

    RHDFUNC(I2CPtr);

    switch (line) {
	case 0:
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC1_MASK, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC1_A, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC1_EN, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_I2C_DDC1_SPEED, (prescale << 16) | 2,
		       0xffff00ff);
	    RHDRegWrite(I2CPtr, R6_DC_I2C_DDC1_SETUP, 0x30000000);
	    break;
	case 1:
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC2_MASK, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC2_A, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC2_EN, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_I2C_DDC2_SPEED, (prescale << 16) | 2,
		       0xffff00ff);
	    RHDRegWrite(I2CPtr, R6_DC_I2C_DDC2_SETUP, 0x30000000);
	    break;
	case 2:
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC3_MASK, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC3_A, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC3_EN, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_I2C_DDC3_SPEED, (prescale << 16) | 2,
		       0xffff00ff);
	    RHDRegWrite(I2CPtr, R6_DC_I2C_DDC3_SETUP, 0x30000000);
	    break;
	case 3:
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC4_MASK, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC4_A, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_GPIO_DDC4_EN, 0x0, 0xffff);
	    RHDRegMask(I2CPtr, R6_DC_I2C_DDC4_SPEED, (prescale << 16) | 2,
		       0xffff00ff);
	    RHDRegWrite(I2CPtr, R6_DC_I2C_DDC4_SETUP, 0x30000000);
	    break;
	default:
	    xf86DrvMsg(I2CPtr->scrnIndex,X_ERROR,
		       "%s: Trying to initialize non-existent I2C line: %i\n",
		       __func__,line);
	    return FALSE;
    }
    RHDRegWrite(I2CPtr, R6_DC_I2C_CONTROL, line << 8);
    RHDRegMask(I2CPtr, R6_DC_I2C_INTERRUPT_CONTROL, 0x2, 0x2);
    RHDRegMask(I2CPtr, R6_DC_I2C_ARBITRATION, 0, 0xff);
    return TRUE;
}

static Bool
rhd6xxWriteRead(I2CDevPtr i2cDevPtr, I2CByte *WriteBuffer, int nWrite, I2CByte *ReadBuffer, int nRead)
{
    Bool ret = FALSE;
    CARD32 data = 0;
    I2CBusPtr I2CPtr = i2cDevPtr->pI2CBus;
    I2CSlaveAddr slave = i2cDevPtr->SlaveAddr;
    rhdI2CPtr I2C = (rhdI2CPtr)I2CPtr->DriverPrivate.ptr;
    CARD8 line = I2C->line;
    int prescale = I2C->prescale;
    int idx = 1;

    enum {
	TRANS_WRITE_READ,
	TRANS_WRITE,
	TRANS_READ
    } trans;

    RHDFUNC(i2cDevPtr->pI2CBus);

    if (nWrite > 0 && nRead > 0) {
	trans = TRANS_WRITE_READ;
    } else if (nWrite > 0) {
	trans = TRANS_WRITE;
    } else if (nRead > 0) {
	trans = TRANS_READ;
    } else {
	/* for bus probing */
	trans = TRANS_WRITE;
    }
    if (slave & 0xff00) {
	xf86DrvMsg(I2CPtr->scrnIndex,X_ERROR,
		   "%s: 10 bit I2C slave addresses not supported\n",__func__);
	return FALSE;
    }

    if (!rhd6xxI2CSetupStatus(I2CPtr, line,  prescale))
	return FALSE;

    RHDRegMask(I2CPtr, R6_DC_I2C_CONTROL, (trans == TRANS_WRITE_READ)
	       ? (1 << 20) : 0, R6_DC_I2C_TRANSACTION_COUNT); /* 2 or 1 Transaction */
    RHDRegMask(I2CPtr, R6_DC_I2C_TRANSACTION0,
	       R6_DC_I2C_STOP_ON_NACK0
	       | (trans == TRANS_READ ? R6_DC_I2C_RW0 : 0)
	       | R6_DC_I2C_START0
	       | (trans == TRANS_WRITE_READ ? 0 : R6_DC_I2C_STOP0 )
	       | ((trans == TRANS_READ ? nRead : nWrite)  << 16),
	       0xffffff);
    if (trans == TRANS_WRITE_READ)
	RHDRegMask(I2CPtr, R6_DC_I2C_TRANSACTION1,
		   nRead << 16
		   | R6_DC_I2C_RW1
		   | R6_DC_I2C_START1
		   | R6_DC_I2C_STOP1,
		   0xffffff); /* <bytes> read */

    data = R6_DC_I2C_INDEX_WRITE
	| (((slave & 0xfe) | (trans == TRANS_READ ? 1 : 0)) << 8 )
	| (0 << 16);
    RHDRegWrite(I2CPtr, R6_DC_I2C_DATA, data);
    if (trans != TRANS_READ) { /* we have bytes to write */
	while (nWrite--) {
	    data = R6_DC_I2C_INDEX_WRITE | ( *(WriteBuffer++) << 8 )
		| (idx++ << 16);
	    RHDRegWrite(I2CPtr, R6_DC_I2C_DATA, data);
	}
    }
    if (trans == TRANS_WRITE_READ) { /* we have bytes to read after write */
	data = R6_DC_I2C_INDEX_WRITE | ((slave | 0x1) << 8) | (idx++ << 16);
	RHDRegWrite(I2CPtr, R6_DC_I2C_DATA, data);
    }
    /* Go! */
    RHDRegMask(I2CPtr, R6_DC_I2C_CONTROL, R6_DC_I2C_GO, R6_DC_I2C_GO);
    if (rhdR6xxI2CStatus(I2CPtr)) {
	/* Hopefully this doesn't write data to index */
	RHDRegWrite(I2CPtr, R6_DC_I2C_DATA, R6_DC_I2C_INDEX_WRITE
		    | R6_DC_I2C_DATA_RW  | /* idx++ */3 << 16);
	while (nRead--) {
	    data = RHDRegRead(I2CPtr, R6_DC_I2C_DATA);
	    *(ReadBuffer++) = (data >> 8) & 0xff;
	}
	ret = TRUE;
    }

    RHDRegMask(I2CPtr, R6_DC_I2C_CONTROL, 0x2, 0xff);
    usleep(10);
    RHDRegWrite(I2CPtr, R6_DC_I2C_CONTROL, 0);

    return ret;
}

/* RV620 */
static Bool
rhdRV620I2CStatus(I2CBusPtr I2CPtr)
{
    int count = 5000;
    volatile CARD32 val;

    RHDFUNC(I2CPtr);

    while (--count) {

	usleep(10);
	val = RHDRegRead(I2CPtr, RV62_GENERIC_I2C_STATUS);
	RHDDebugVerb(I2CPtr->scrnIndex,1,
		     "SW_STATUS: 0x%x %i\n",(unsigned int)val,count);
	if (val & RV62_GENERIC_I2C_DONE)
	    break;
    }
    RHDRegMask(I2CPtr, RV62_GENERIC_I2C_INTERRUPT_CONTROL, 0x2, 0xff);

    if (!count
	|| (val & (RV62_GENERIC_I2C_STOPPED_ON_NACK | RV62_GENERIC_I2C_NACK
		| RV62_GENERIC_I2C_ABORTED | RV62_GENERIC_I2C_TIMEOUT)))
	return FALSE; /* 2 */

    return TRUE; /* 1 */
}

static  Bool
rhdRV620I2CSetupStatus(I2CBusPtr I2CPtr, int line, int prescale)
{
    CARD32 reg_7d9c = 0; /* 0 is invalid */
#ifdef ATOM_BIOS
    RHDPtr rhdPtr = RHDPTRI(I2CPtr);
    AtomBiosArgRec data;
    int i = 0;
    struct atomGPIOTable {
	unsigned char line;
	unsigned char pad;
	unsigned short reg_7d9c;
    } *table;

    RHDFUNC(I2CPtr);

    if (line > 3)
	return FALSE;

    data.val = 0x36;
    if (RHDAtomBiosFunc(I2CPtr->scrnIndex,
			rhdPtr->atomBIOS,
			ATOMBIOS_GET_CODE_DATA_TABLE,
			&data) == ATOM_SUCCESS) {

	table = (struct atomGPIOTable *)data.CommandDataTable.loc;

	while (i * sizeof(struct atomGPIOTable) < data.CommandDataTable.size) {

	    if (table[i].line == line) {

		reg_7d9c = table[i].reg_7d9c;

		DEBUGP( ErrorF("Line[%i] = 0x%4.4x\n",line, reg_7d9c)) ;

		break;
	    }
	    i++;
	}
    }
    if (!reg_7d9c)
#endif
    {
	CARD32 regList7d9c[] = { 0x1, 0x0203 };
	if (line > 1)
	    return FALSE;

	reg_7d9c = regList7d9c[line];
    }

    RHDRegWrite(I2CPtr, 0x7e40, 0);
    RHDRegWrite(I2CPtr, 0x7e50, 0);
    RHDRegWrite(I2CPtr, 0x7e60, 0);
    RHDRegWrite(I2CPtr, 0x7e20, 0);

    RHDRegWrite(I2CPtr, RV62_GENERIC_I2C_PIN_SELECTION, reg_7d9c);
    RHDRegMask(I2CPtr, RV62_GENERIC_I2C_SPEED,
	    (prescale & 0xffff) << 16 | 0x02, 0xffff00ff);
    RHDRegWrite(I2CPtr, RV62_GENERIC_I2C_SETUP, 0x30000000);
    RHDRegMask(I2CPtr, RV62_GENERIC_I2C_INTERRUPT_CONTROL,
	    RV62_GENERIC_I2C_DONE_ACK, RV62_GENERIC_I2C_DONE_ACK);

    return TRUE;
}

static Bool
rhdRV620Transaction(I2CDevPtr i2cDevPtr, Bool Write, I2CByte *Buffer, int count)
{
    I2CBusPtr I2CPtr = i2cDevPtr->pI2CBus;
    I2CSlaveAddr slave = i2cDevPtr->SlaveAddr;
    Bool Start = TRUE;

    RHDFUNC(I2CPtr);

#define MAX 8

    while (count > 0) {
	int num;
	int idx = 0;
	CARD32 data = 0;

	if (count > MAX) {
	    num = MAX;
	    RHDRegMask(I2CPtr, RV62_GENERIC_I2C_TRANSACTION,
		    (MAX - (((Start) ? 0 : 1))) << 16
		    | RV62_GENERIC_I2C_STOP_ON_NACK
		    | RV62_GENERIC_I2C_ACK_ON_READ
		    | (Start ? RV62_GENERIC_I2C_START : 0)
		    | (!Write ? RV62_GENERIC_I2C_RW : 0 ),
		    0xFFFFFF);
	} else {
	    num = count;
	    data = ( count - (((Start) ? 0 : 1)) ) << 16
		| RV62_GENERIC_I2C_STOP_ON_NACK
		|  RV62_GENERIC_I2C_STOP
		| (Start ? RV62_GENERIC_I2C_START : 0)
		| (!Write ? RV62_GENERIC_I2C_RW : 0);
	    RHDRegMask(I2CPtr, RV62_GENERIC_I2C_TRANSACTION,
		    data,
		    0xFFFFFF);
	}

	if (Start) {
	    data = RV62_GENERIC_I2C_INDEX_WRITE
		| (((slave & 0xfe) | ( Write ? 0 : 1)) << 8)
		| (idx++ << 16);
	    RHDRegWrite(I2CPtr, RV62_GENERIC_I2C_DATA, data);
	}

	if (Write) {
	    while (num--) {
		data = RV62_GENERIC_I2C_INDEX_WRITE
		    | (idx++ << 16)
		    | *(Buffer++) << 8;
		RHDRegWrite(I2CPtr, RV62_GENERIC_I2C_DATA, data);
	    }

	    RHDRegMask(I2CPtr, RV62_GENERIC_I2C_CONTROL,
		    RV62_GENERIC_I2C_GO, RV62_GENERIC_I2C_GO);
	    if (!rhdRV620I2CStatus(I2CPtr))
		return FALSE;
	} else {

	    RHDRegMask(I2CPtr, RV62_GENERIC_I2C_CONTROL,
		    RV62_GENERIC_I2C_GO, RV62_GENERIC_I2C_GO);
	    if (!rhdRV620I2CStatus(I2CPtr))
		return FALSE;

	    RHDRegWrite(I2CPtr, RV62_GENERIC_I2C_DATA,
		     RV62_GENERIC_I2C_INDEX_WRITE
		     | (idx++ << 16)
		     | RV62_GENERIC_I2C_RW);

	    while (num--) {
		data = RHDRegRead(I2CPtr, RV62_GENERIC_I2C_DATA);
		*(Buffer++) = (CARD8)((data >> 8) & 0xff);
	    }
	}
	Start = FALSE;
	count -= MAX;
    }

    return TRUE;
}

static Bool
rhdRV620WriteRead(I2CDevPtr i2cDevPtr, I2CByte *WriteBuffer, int nWrite, I2CByte *ReadBuffer, int nRead)
{
    I2CBusPtr I2CPtr = i2cDevPtr->pI2CBus;
    rhdI2CPtr I2C = (rhdI2CPtr)I2CPtr->DriverPrivate.ptr;
    CARD8 line = I2C->line;
    int prescale = I2C->prescale;

    RHDFUNC(I2C);

    rhdRV620I2CSetupStatus(I2CPtr, line, prescale);

    if (nWrite)
	if (!rhdRV620Transaction(i2cDevPtr, TRUE, WriteBuffer, nWrite))
	    return FALSE;
    if (nRead)
	if (!rhdRV620Transaction(i2cDevPtr, FALSE, ReadBuffer, nRead))
	    return FALSE;

    return TRUE;
}

static void
rhdTearDownI2C(I2CBusPtr *I2C)
{
    int i;

    /*
     * xf86I2CGetScreenBuses() is
     * broken in older server versions.
     * So we cannot use it. How bad!
     */
    for (i = 0; i < I2C_LINES; i++) {
	char *name;
	if (!I2C[i])
	    break;
	name = I2C[i]->BusName;
	xfree(I2C[i]->DriverPrivate.ptr);
	xf86DestroyI2CBusRec(I2C[i], TRUE, TRUE);
	xfree(name);
    }
    xfree(I2C);
}

#define TARGET_HW_I2C_CLOCK 25 /*  kHz */
#define DEFAULT_ENGINE_CLOCK 700000 /* kHz (guessed) */
static CARD32
rhdGetI2CPrescale(RHDPtr rhdPtr)
{
#ifdef ATOM_BIOS
    AtomBiosArgRec atomBiosArg;
    RHDFUNC(rhdPtr);

    if (rhdPtr->ChipSet < RHD_R600) {
	RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			GET_DEFAULT_ENGINE_CLOCK, &atomBiosArg);
	return (0x7f << 8)
	    + (atomBiosArg.val / (4 * 0x7f * TARGET_HW_I2C_CLOCK));
    } else if (rhdPtr->ChipSet < RHD_RV620) {
	RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			GET_REF_CLOCK, &atomBiosArg);
	return (atomBiosArg.val / TARGET_HW_I2C_CLOCK);
    } else {
	RHDAtomBiosFunc(rhdPtr->scrnIndex, rhdPtr->atomBIOS,
			GET_REF_CLOCK, &atomBiosArg);
	return (atomBiosArg.val / (4 * TARGET_HW_I2C_CLOCK));
    }
#else
    RHDFUNC(rhdPtr);

    if (rhdPtr->ChipSet < RHD_R600) {
	return (0x7f << 8)
	    + (DEFAULT_ENGINE_CLOCK) / (4 * 0x7f * TARGET_HW_I2C_CLOCK);
    } else if {rhdPtr->ChipSet < RHD_RV620) {
	return (DEFAULT_ENGINE_CLOCK / TARGET_HW_I2C_CLOCK);
    } else
	  return (DEFAULT_ENGINE_CLOCK / (4 * TARGET_HW_I2C_CLOCK));
#endif
}

static Bool
rhdI2CAddress(I2CDevPtr d, I2CSlaveAddr addr)
{
    d->SlaveAddr = addr;
    return xf86I2CWriteRead(d, NULL, 0, NULL, 0);
}

/*
 * This stub is needed to keep xf86I2CProbeAddress() happy.
 */
static void
rhdI2CStop(I2CDevPtr d)
{
}

static I2CBusPtr *
rhdInitI2C(int scrnIndex)
{
    int i;
    rhdI2CPtr I2C;
    I2CBusPtr I2CPtr = NULL;
    RHDPtr rhdPtr = RHDPTR(xf86Screens[scrnIndex]);
    I2CBusPtr *I2CList;
    int numLines = (rhdPtr->ChipSet < RHD_R600) ? 3 : I2C_LINES;
    CARD16 prescale = rhdGetI2CPrescale(rhdPtr);

    RHDFUNCI(scrnIndex);

    if (!(I2CList = xcalloc(I2C_LINES, sizeof(I2CBusPtr)))) {
	xf86DrvMsg(scrnIndex, X_ERROR,
		   "%s: Out of memory.\n",__func__);
    }
    /* We have 4 I2C lines */
    for (i = 0; i < numLines; i++) {
	if (!(I2C = xcalloc(sizeof(rhdI2CRec),1))) {
	    xf86DrvMsg(scrnIndex, X_ERROR,
		       "%s: Out of memory.\n",__func__);
	    goto error;
	}
	I2C->scrnIndex = scrnIndex;
        /*
	 * This is a value that has been found to work on many cards.
	 * It nees to be replaced by the proper calculation formula
	 * once this is available.
	 */
	I2C->prescale = prescale;
	xf86DrvMsgVerb(scrnIndex, X_INFO, 5, "I2C clock prescale value: %x\n",I2C->prescale);
	I2C->line = i;
	if (!(I2CPtr = xf86CreateI2CBusRec())) {
	    xf86DrvMsg(scrnIndex, X_ERROR,
		       "Cannot allocate I2C BusRec.\n");
	    xfree(I2C);
	    goto error;
	}
	I2CPtr->DriverPrivate.ptr = I2C;
	if (!(I2CPtr->BusName = xalloc(18))) {
	    xf86DrvMsg(scrnIndex, X_ERROR,
		       "%s: Cannot allocate memory.\n",__func__);
	    xfree(I2C);
	    xf86DestroyI2CBusRec(I2CPtr, TRUE, FALSE);
	    goto error;
	}
	snprintf(I2CPtr->BusName,17,"RHD I2C line %1.1i",i);
	I2CPtr->scrnIndex = scrnIndex;
	if (rhdPtr->ChipSet < RHD_RS600)
	    I2CPtr->I2CWriteRead = rhd5xxWriteRead;
	else if (RHDFamily(rhdPtr->ChipSet) == RHD_FAMILY_RS690)
	    I2CPtr->I2CWriteRead = rhdRS69WriteRead;
	else if (rhdPtr->ChipSet < RHD_RV620)
	    I2CPtr->I2CWriteRead = rhd6xxWriteRead;
	else
	    I2CPtr->I2CWriteRead = rhdRV620WriteRead;
	I2CPtr->I2CAddress = rhdI2CAddress;
	I2CPtr->I2CStop = rhdI2CStop;

	if (!(xf86I2CBusInit(I2CPtr))) {
	    xf86DrvMsg(scrnIndex, X_ERROR,
		       "I2C BusInit failed for bus %i\n",i);
	    xfree(I2CPtr->BusName);
	    xfree(I2C);
	    xf86DestroyI2CBusRec(I2CPtr, TRUE, FALSE);
	    goto error;
	}
	I2CList[i] = I2CPtr;
    }
    return I2CList;
 error:
    rhdTearDownI2C(I2CList);
    return NULL;
}

RHDI2CResult
rhdI2CProbeAddress(int scrnIndex, I2CBusPtr *I2CList,
		   int line, CARD8 slave)
{
    I2CDevPtr dev;
    int ret = FALSE;
    char *name = "I2CProbe";

    if (line >= I2C_LINES || !I2CList[line])
	return RHD_I2C_NOLINE;

    if ((dev = xf86CreateI2CDevRec())) {
	dev->SlaveAddr = slave & 0xFE;
	dev->DevName = name;
	dev->pI2CBus = I2CList[line];

	if (xf86I2CDevInit(dev))
	    ret = xf86I2CWriteRead(dev, NULL, 0, NULL, 0);

	xf86DestroyI2CDevRec(dev, TRUE);
    }

    return ret;
}

RHDI2CResult
RHDI2CFunc(int scrnIndex, I2CBusPtr *I2CList, RHDi2cFunc func,
	   RHDI2CDataArgPtr datap)
{
    RHDFUNCI(scrnIndex);

    if (func == RHD_I2C_INIT) {
	if (!(datap->I2CBusList = rhdInitI2C(scrnIndex)))
	    return RHD_I2C_FAILED;
	else
	    return RHD_I2C_SUCCESS;
    }
    if (func == RHD_I2C_DDC) {
	if (datap->i >= I2C_LINES || !I2CList[datap->i])
	    return RHD_I2C_NOLINE;

	datap->monitor = xf86DoEDID_DDC2(scrnIndex, I2CList[datap->i]);
	return RHD_I2C_SUCCESS;
    }
    if (func == RHD_I2C_PROBE_ADDR) {
	return rhdI2CProbeAddress(scrnIndex, I2CList,
				  datap->target.line,
				  datap->target.slave);
    }
    if (func == RHD_I2C_GETBUS) {
	if (datap->i >= I2C_LINES || !I2CList[datap->i])
	    return RHD_I2C_NOLINE;

	datap->i2cBusPtr = I2CList[datap->i];
	return RHD_I2C_SUCCESS;
    }
    if (func == RHD_I2C_TEARDOWN) {
	if (I2CList)
	    rhdTearDownI2C(I2CList);
	return RHD_I2C_SUCCESS;
    }
    return RHD_I2C_FAILED;
}

