/* -*- c-basic-offset: 4 -*- */
/**************************************************************************

Copyright © 2006 Dave Airlie

All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sub license, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice (including the
next paragraph) shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

**************************************************************************/

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
#include "sil164.h"
#include "sil164_reg.h"

typedef struct _Sil164SaveRec {
    uint8_t reg8;
    uint8_t reg9;
    uint8_t regc;
} SIL164SaveRec;

typedef struct {
    I2CDevRec d;
    Bool quiet;
    SIL164SaveRec SavedReg;
    SIL164SaveRec ModeReg;
} SIL164Rec, *SIL164Ptr;

#define SILPTR(d) ((SIL164Ptr)(d->DriverPrivate.ptr))

static Bool
sil164ReadByte(SIL164Ptr sil, int addr, uint8_t *ch)
{
    if (!xf86I2CReadByte(&(sil->d), addr, ch)) {
	if (!sil->quiet) {
	    xf86DrvMsg(sil->d.pI2CBus->scrnIndex, X_ERROR,
		       "Unable to read from %s Slave %d.\n",
		       sil->d.pI2CBus->BusName, sil->d.SlaveAddr);
	}
	return FALSE;
    }
    return TRUE;
}

static Bool
sil164WriteByte(SIL164Ptr sil, int addr, uint8_t ch)
{
    if (!xf86I2CWriteByte(&(sil->d), addr, ch)) {
	if (!sil->quiet) {
	    xf86DrvMsg(sil->d.pI2CBus->scrnIndex, X_ERROR,
		       "Unable to write to %s Slave %d.\n",
		       sil->d.pI2CBus->BusName, sil->d.SlaveAddr);
	}
	return FALSE;
    }
    return TRUE;
}

/* Silicon Image 164 driver for chip on i2c bus */
static void *
sil164_init(I2CBusPtr b, I2CSlaveAddr addr)
{
    /* this will detect the SIL164 chip on the specified i2c bus */
    SIL164Ptr sil;
    unsigned char ch;

    sil = xcalloc(1, sizeof(SIL164Rec));
    if (sil == NULL)
	return NULL;

    sil->d.DevName = "SIL164 TMDS Controller";
    sil->d.SlaveAddr = addr;
    sil->d.pI2CBus = b;
    sil->d.StartTimeout = b->StartTimeout;
    sil->d.BitTimeout = b->BitTimeout;
    sil->d.AcknTimeout = b->AcknTimeout;
    sil->d.ByteTimeout = b->ByteTimeout;
    sil->d.DriverPrivate.ptr = sil;
    sil->quiet = TRUE;

    if (!sil164ReadByte(sil, SIL164_VID_LO, &ch))
	goto out;

    if (ch!=(SIL164_VID & 0xFF)) {
	xf86DrvMsg(sil->d.pI2CBus->scrnIndex, X_ERROR,
		   "sil164 not detected got %d: from %s Slave %d.\n",
		   ch, sil->d.pI2CBus->BusName, sil->d.SlaveAddr);
	goto out;
    }

    if (!sil164ReadByte(sil, SIL164_DID_LO, &ch))
	goto out;

    if (ch!=(SIL164_DID & 0xFF)) {
	xf86DrvMsg(sil->d.pI2CBus->scrnIndex, X_ERROR,
		   "sil164 not detected got %d: from %s Slave %d.\n",
		   ch, sil->d.pI2CBus->BusName, sil->d.SlaveAddr);
	goto out;
    }
    sil->quiet = FALSE;

    if (!xf86I2CDevInit(&(sil->d))) {
	goto out;
    }

    return sil;

out:
    xfree(sil);
    return NULL;
}

static xf86OutputStatus
sil164_detect(I2CDevPtr d)
{
    SIL164Ptr sil = SILPTR(d);
    uint8_t reg9;

    sil164ReadByte(sil, SIL164_REG9, &reg9);

    if (reg9 & SIL164_9_HTPLG)
	return XF86OutputStatusConnected;
    else
	return XF86OutputStatusDisconnected;
}

static ModeStatus
sil164_mode_valid(I2CDevPtr d, DisplayModePtr mode)
{
    return MODE_OK;
}

static void
sil164_mode_set(I2CDevPtr d, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
    /* As long as the basics are set up, since we don't have clock dependencies
     * in the mode setup, we can just leave the registers alone and everything
     * will work fine.
     */
    /* recommended programming sequence from doc */
    /*sil164WriteByte(sil, 0x08, 0x30);
      sil164WriteByte(sil, 0x09, 0x00);
      sil164WriteByte(sil, 0x0a, 0x90);
      sil164WriteByte(sil, 0x0c, 0x89);
      sil164WriteByte(sil, 0x08, 0x31);*/
    /* don't do much */
    return;
}

/* set the SIL164 power state */
static void
sil164_dpms(I2CDevPtr d, int mode)
{
    SIL164Ptr sil = SILPTR(d);
    int ret;
    unsigned char ch;

    ret = sil164ReadByte(sil, SIL164_REG8, &ch);
    if (ret == FALSE)
	return;

    if (mode == DPMSModeOn)
	ch |= SIL164_8_PD;
    else
	ch &= ~SIL164_8_PD;

    sil164WriteByte(sil, SIL164_REG8, ch);

    return;
}

static void
sil164_dump_regs(I2CDevPtr d)
{
    SIL164Ptr sil = SILPTR(d);
    uint8_t val;

    sil164ReadByte(sil, SIL164_FREQ_LO, &val);
    xf86DrvMsg(sil->d.pI2CBus->scrnIndex, X_INFO, "SIL164_FREQ_LO: 0x%02x\n",
	       val);
    sil164ReadByte(sil, SIL164_FREQ_HI, &val);
    xf86DrvMsg(sil->d.pI2CBus->scrnIndex, X_INFO, "SIL164_FREQ_HI: 0x%02x\n",
	       val);
    sil164ReadByte(sil, SIL164_REG8, &val);
    xf86DrvMsg(sil->d.pI2CBus->scrnIndex, X_INFO, "SIL164_REG8: 0x%02x\n", val);
    sil164ReadByte(sil, SIL164_REG9, &val);
    xf86DrvMsg(sil->d.pI2CBus->scrnIndex, X_INFO, "SIL164_REG9: 0x%02x\n", val);
    sil164ReadByte(sil, SIL164_REGC, &val);
    xf86DrvMsg(sil->d.pI2CBus->scrnIndex, X_INFO, "SIL164_REGC: 0x%02x\n", val);
}

static void
sil164_save(I2CDevPtr d)
{
    SIL164Ptr sil = SILPTR(d);

    if (!sil164ReadByte(sil, SIL164_REG8, &sil->SavedReg.reg8))
	return;

    if (!sil164ReadByte(sil, SIL164_REG9, &sil->SavedReg.reg9))
	return;

    if (!sil164ReadByte(sil, SIL164_REGC, &sil->SavedReg.regc))
	return;

    return;
}

static void
sil164_restore(I2CDevPtr d)
{
    SIL164Ptr sil = SILPTR(d);

    /* Restore it powered down initially */
    sil164WriteByte(sil, SIL164_REG8, sil->SavedReg.reg8 & ~0x1);

    sil164WriteByte(sil, SIL164_REG9, sil->SavedReg.reg9);
    sil164WriteByte(sil, SIL164_REGC, sil->SavedReg.regc);
    sil164WriteByte(sil, SIL164_REG8, sil->SavedReg.reg8);
}


_X_EXPORT I830I2CVidOutputRec SIL164VidOutput = {
    .init = sil164_init,
    .detect = sil164_detect,
    .mode_valid = sil164_mode_valid,
    .mode_set = sil164_mode_set,
    .dpms = sil164_dpms,
    .dump_regs = sil164_dump_regs,
    .save = sil164_save,
    .restore = sil164_restore,
};
