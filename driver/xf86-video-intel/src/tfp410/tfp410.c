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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdint.h>

#include "xf86.h"
#include "xf86_OSproc.h"
#include "compiler.h"
#include "miscstruct.h"
#include "xf86i2c.h"
#include "xf86Crtc.h"
#ifdef HAVE_XEXTPROTO_71
#include <X11/extensions/dpmsconst.h>
#else
#define DPMS_SERVER
#include <X11/extensions/dpms.h>
#endif


#include "../i2c_vid.h"
#include "tfp410.h"
#include "tfp410_reg.h"

typedef struct _TFP410SaveRec {
    uint8_t ctl1;
    uint8_t ctl2;
} TFP410SaveRec;

typedef struct {
    I2CDevRec d;
    Bool quiet;

    TFP410SaveRec SavedReg;
    TFP410SaveRec ModeReg;
} TFP410Rec, *TFP410Ptr;

#define TFPPTR(d) ((TFP410Ptr)(d->DriverPrivate.ptr))

static Bool
tfp410ReadByte(TFP410Ptr tfp, int addr, uint8_t *ch)
{
    if (!xf86I2CReadByte(&(tfp->d), addr, ch)) {
	if (!tfp->quiet) {
	    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_ERROR,
		       "Unable to read from %s Slave %d.\n",
		       tfp->d.pI2CBus->BusName, tfp->d.SlaveAddr);
	}
	return FALSE;
    }
    return TRUE;
}

static Bool
tfp410WriteByte(TFP410Ptr tfp, int addr, uint8_t ch)
{
    if (!xf86I2CWriteByte(&(tfp->d), addr, ch)) {
	if (!tfp->quiet) {
	    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_ERROR,
		       "Unable to write to %s Slave %d.\n",
		       tfp->d.pI2CBus->BusName, tfp->d.SlaveAddr);
	}
	return FALSE;
    }
    return TRUE;
}

static int
tfp410GetID(TFP410Ptr tfp, int addr)
{
    unsigned char ch1, ch2;

    if (tfp410ReadByte(tfp, addr+0, &ch1) &&
        tfp410ReadByte(tfp, addr+1, &ch2)) {

	return ((ch2<<8) & 0xFF00) | (ch1 & 0x00FF);
    }
    return -1;
}

/* Ti TFP410 driver for chip on i2c bus */
static void *
tfp410_init(I2CBusPtr b, I2CSlaveAddr addr)
{
    /* this will detect the tfp410 chip on the specified i2c bus */
    TFP410Ptr tfp;
    int id;

    tfp = xcalloc(1, sizeof(TFP410Rec));
    if (tfp == NULL)
	return NULL;

    tfp->d.DevName = "TFP410 TMDS Controller";
    tfp->d.SlaveAddr = addr;
    tfp->d.pI2CBus = b;
    tfp->d.StartTimeout = b->StartTimeout;
    tfp->d.BitTimeout = b->BitTimeout;
    tfp->d.AcknTimeout = b->AcknTimeout;
    tfp->d.ByteTimeout = b->ByteTimeout;
    tfp->d.DriverPrivate.ptr = tfp;
    tfp->quiet = TRUE;

    if ((id = tfp410GetID(tfp, TFP410_VID_LO)) != TFP410_VID) {
	if (id != 0xffffffff) {
	    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_ERROR,
		       "tfp410 not detected got VID %X: from %s Slave %d.\n",
		       id, tfp->d.pI2CBus->BusName, tfp->d.SlaveAddr);
	}
	goto out;
    }

    if ((id = tfp410GetID(tfp, TFP410_DID_LO)) != TFP410_DID) {
	xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_ERROR,
		   "tfp410 not detected got DID %X: from %s Slave %d.\n",
		   id, tfp->d.pI2CBus->BusName, tfp->d.SlaveAddr);
	goto out;
    }
    tfp->quiet = FALSE;

    if (!xf86I2CDevInit(&(tfp->d))) {
	goto out;
    }

    return tfp;

out:
    xfree(tfp);
    return NULL;
}

static xf86OutputStatus
tfp410_detect(I2CDevPtr d)
{
    TFP410Ptr tfp = TFPPTR(d);
    xf86OutputStatus ret = XF86OutputStatusDisconnected;
    unsigned char ctl2;

    if (tfp410ReadByte(tfp, TFP410_CTL_2, &ctl2)) {
	if (ctl2 & TFP410_CTL_2_HTPLG)
	    ret = XF86OutputStatusConnected;
	else
	    ret = XF86OutputStatusDisconnected;
    }

    return ret;
}

static ModeStatus
tfp410_mode_valid(I2CDevPtr d, DisplayModePtr mode)
{
    return MODE_OK;
}

static void
tfp410_mode_set(I2CDevPtr d, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
    /* As long as the basics are set up, since we don't have clock dependencies
     * in the mode setup, we can just leave the registers alone and everything
     * will work fine.
     */
    /* don't do much */
    return;
}

/* set the tfp410 power state */
static void
tfp410_dpms(I2CDevPtr d, int mode)
{
    TFP410Ptr tfp = TFPPTR(d);
    unsigned char ctl1;

    if (!tfp410ReadByte(tfp, TFP410_CTL_1, &ctl1))
	return;

    if (mode == DPMSModeOn)
	ctl1 |= TFP410_CTL_1_PD;
    else
	ctl1 &= ~TFP410_CTL_1_PD;

    tfp410WriteByte(tfp, TFP410_CTL_1, ctl1);
}

static void
tfp410_dump_regs(I2CDevPtr d)
{
    TFP410Ptr tfp = TFPPTR(d);
    uint8_t val, val2;

    tfp410ReadByte(tfp, TFP410_REV, &val);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_REV: 0x%02X\n", val);
    tfp410ReadByte(tfp, TFP410_CTL_1, &val);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_CTL1: 0x%02X\n", val);
    tfp410ReadByte(tfp, TFP410_CTL_2, &val);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_CTL2: 0x%02X\n", val);
    tfp410ReadByte(tfp, TFP410_CTL_3, &val);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_CTL3: 0x%02X\n", val);
    tfp410ReadByte(tfp, TFP410_USERCFG, &val);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_USERCFG: 0x%02X\n", val);
    tfp410ReadByte(tfp, TFP410_DE_DLY, &val);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_DE_DLY: 0x%02X\n", val);
    tfp410ReadByte(tfp, TFP410_DE_CTL, &val);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_DE_CTL: 0x%02X\n", val);
    tfp410ReadByte(tfp, TFP410_DE_TOP, &val);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_DE_TOP: 0x%02X\n", val);
    tfp410ReadByte(tfp, TFP410_DE_CNT_LO, &val);
    tfp410ReadByte(tfp, TFP410_DE_CNT_HI, &val2);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_DE_CNT: 0x%02X%02X\n", val2, val);
    tfp410ReadByte(tfp, TFP410_DE_LIN_LO, &val);
    tfp410ReadByte(tfp, TFP410_DE_LIN_HI, &val2);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_DE_LIN: 0x%02X%02X\n", val2, val);
    tfp410ReadByte(tfp, TFP410_H_RES_LO, &val);
    tfp410ReadByte(tfp, TFP410_H_RES_HI, &val2);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_H_RES: 0x%02X%02X\n", val2, val);
    tfp410ReadByte(tfp, TFP410_V_RES_LO, &val);
    tfp410ReadByte(tfp, TFP410_V_RES_HI, &val2);
    xf86DrvMsg(tfp->d.pI2CBus->scrnIndex, X_INFO,
	       "TFP410_V_RES: 0x%02X%02X\n", val2, val);
}

static void
tfp410_save(I2CDevPtr d)
{
    TFP410Ptr tfp = TFPPTR(d);

    if (!tfp410ReadByte(tfp, TFP410_CTL_1, &tfp->SavedReg.ctl1))
	return;

    if (!tfp410ReadByte(tfp, TFP410_CTL_2, &tfp->SavedReg.ctl2))
	return;
}

static void
tfp410_restore(I2CDevPtr d)
{
    TFP410Ptr tfp = TFPPTR(d);

    /* Restore it powered down initially */
    tfp410WriteByte(tfp, TFP410_CTL_1, tfp->SavedReg.ctl1 & ~0x1);

    tfp410WriteByte(tfp, TFP410_CTL_2, tfp->SavedReg.ctl2);
    tfp410WriteByte(tfp, TFP410_CTL_1, tfp->SavedReg.ctl1);
}

_X_EXPORT I830I2CVidOutputRec TFP410VidOutput = {
    .init = tfp410_init,
    .detect = tfp410_detect,
    .mode_valid = tfp410_mode_valid,
    .mode_set = tfp410_mode_set,
    .dpms = tfp410_dpms,
    .dump_regs = tfp410_dump_regs,
    .save = tfp410_save,
    .restore = tfp410_restore,
};
