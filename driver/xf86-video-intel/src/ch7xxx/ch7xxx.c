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
#include <string.h>
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
#include "ch7xxx.h"
#include "ch7xxx_reg.h"

/** @file
 * driver for the Chrontel 7xxx DVI chip over DVO.
 */

static struct ch7xxx_id_struct {
    uint8_t vid;
    char *name;
} ch7xxx_ids[] = { 
	{ CH7011_VID, "CH7011" },
	{ CH7009A_VID, "CH7009A" },
	{ CH7009B_VID, "CH7009B" },
	{ CH7301_VID, "CH7301" },
};

#define ID_ARRAY_SIZE (sizeof(ch7xxx_ids) / sizeof(ch7xxx_ids[0]))

struct ch7xxx_reg_state {
    uint8_t regs[CH7xxx_NUM_REGS];
};

struct ch7xxx_priv {
    I2CDevRec d;
    Bool quiet;

    struct ch7xxx_reg_state SavedReg;
    struct ch7xxx_reg_state ModeReg;
    uint8_t save_TCTL, save_TPCP, save_TPD, save_TPVT;
    uint8_t save_TLPF, save_TCT, save_PM, save_IDF;
};

static void ch7xxx_save(I2CDevPtr d);

static char *ch7xxx_get_id(uint8_t vid)
{
    int i;

    for (i = 0; i < ID_ARRAY_SIZE; i++) {
        if (ch7xxx_ids[i].vid == vid)
		return ch7xxx_ids[i].name;
    }

    return NULL;
}

/** Reads an 8 bit register */
static Bool
ch7xxx_read(struct ch7xxx_priv *dev_priv, int addr, unsigned char *ch)
{
    if (!xf86I2CReadByte(&dev_priv->d, addr, ch)) {
	if (!dev_priv->quiet) {
	    xf86DrvMsg(dev_priv->d.pI2CBus->scrnIndex,
		       X_ERROR, "Unable to read from %s Slave %d.\n",
		       dev_priv->d.pI2CBus->BusName, dev_priv->d.SlaveAddr);
	}
	return FALSE;
    }

    return TRUE;
}

/** Writes an 8 bit register */
static Bool
ch7xxx_write(struct ch7xxx_priv *dev_priv, int addr, unsigned char ch)
{
    if (!xf86I2CWriteByte(&dev_priv->d, addr, ch)) {
	if (!dev_priv->quiet) {
	    xf86DrvMsg(dev_priv->d.pI2CBus->scrnIndex, X_ERROR,
		       "Unable to write to %s Slave %d.\n",
		       dev_priv->d.pI2CBus->BusName, dev_priv->d.SlaveAddr);
	}
	return FALSE;
    }

    return TRUE;
}

static void *
ch7xxx_init(I2CBusPtr b, I2CSlaveAddr addr)
{
    /* this will detect the CH7xxx chip on the specified i2c bus */
    struct ch7xxx_priv *dev_priv;
    uint8_t vendor, device;
    char *name;

    dev_priv = xcalloc(1, sizeof(struct ch7xxx_priv));
    if (dev_priv == NULL)
	return NULL;

    dev_priv->d.DevName = "CH7xxx TMDS Controller";
    dev_priv->d.SlaveAddr = addr;
    dev_priv->d.pI2CBus = b;
    dev_priv->d.StartTimeout = b->StartTimeout;
    dev_priv->d.BitTimeout = b->BitTimeout;
    dev_priv->d.AcknTimeout = b->AcknTimeout;
    dev_priv->d.ByteTimeout = b->ByteTimeout;
    dev_priv->d.DriverPrivate.ptr = dev_priv;

    dev_priv->quiet = TRUE;
    if (!ch7xxx_read(dev_priv, CH7xxx_REG_VID, &vendor))
	goto out;

    name = ch7xxx_get_id(vendor);
    if (!name) {
	xf86DrvMsg(dev_priv->d.pI2CBus->scrnIndex, X_INFO,
		   "ch7xxx not detected; got 0x%02x from %s slave %d.\n",
		   vendor, dev_priv->d.pI2CBus->BusName,
		   dev_priv->d.SlaveAddr);
	goto out;
    }


    if (!ch7xxx_read(dev_priv, CH7xxx_REG_DID, &device))
	goto out;

    if (device != CH7xxx_DID) {
	xf86DrvMsg(dev_priv->d.pI2CBus->scrnIndex, X_INFO,
		   "ch7xxx not detected; got 0x%02x from %s slave %d.\n",
		   device, dev_priv->d.pI2CBus->BusName,
		   dev_priv->d.SlaveAddr);
	goto out;
    }
    dev_priv->quiet = FALSE;

    xf86DrvMsg(dev_priv->d.pI2CBus->scrnIndex, X_INFO,
	       "Detected %s chipset, vendor/device ID 0x%02x/0x%02x\n",
	       name, vendor, device);

    if (!xf86I2CDevInit(&dev_priv->d)) {
	goto out;
    }

    return dev_priv;

out:
    xfree(dev_priv);
    return NULL;
}

static xf86OutputStatus
ch7xxx_detect(I2CDevPtr d)
{
    struct ch7xxx_priv *dev_priv = d->DriverPrivate.ptr;
    uint8_t cdet, orig_pm, pm;

    ch7xxx_read(dev_priv, CH7xxx_PM, &orig_pm);

    pm = orig_pm;
    pm &= ~CH7xxx_PM_FPD;
    pm |= CH7xxx_PM_DVIL | CH7xxx_PM_DVIP;

    ch7xxx_write(dev_priv, CH7xxx_PM, pm);

    ch7xxx_read(dev_priv, CH7xxx_CONNECTION_DETECT, &cdet);

    ch7xxx_write(dev_priv, CH7xxx_PM, orig_pm);

    if (cdet & CH7xxx_CDET_DVI) 
    	return XF86OutputStatusConnected;
    return XF86OutputStatusDisconnected;
}

static ModeStatus
ch7xxx_mode_valid(I2CDevPtr d, DisplayModePtr mode)
{
    if (mode->Clock > 165000)
	return MODE_CLOCK_HIGH;

    return MODE_OK;
}

static void
ch7xxx_mode_set(I2CDevPtr d, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
    struct ch7xxx_priv *dev_priv = d->DriverPrivate.ptr;
    uint8_t tvco, tpcp, tpd, tlpf, idf;

    if (mode->Clock <= 65000) {
	tvco = 0x23;
	tpcp = 0x08;
	tpd = 0x16;
	tlpf = 0x60;
    } else {
	tvco = 0x2d;
	tpcp = 0x06;
	tpd = 0x26;
	tlpf = 0xa0;
    }

    ch7xxx_write(dev_priv, CH7xxx_TCTL, 0x00);
    ch7xxx_write(dev_priv, CH7xxx_TVCO, tvco);
    ch7xxx_write(dev_priv, CH7xxx_TPCP, tpcp);
    ch7xxx_write(dev_priv, CH7xxx_TPD, tpd);
    ch7xxx_write(dev_priv, CH7xxx_TPVT, 0x30);
    ch7xxx_write(dev_priv, CH7xxx_TLPF, tlpf);
    ch7xxx_write(dev_priv, CH7xxx_TCT, 0x00);

    ch7xxx_read(dev_priv, CH7xxx_IDF, &idf);

    idf &= ~(CH7xxx_IDF_HSP | CH7xxx_IDF_VSP);
    if (mode->Flags & V_PHSYNC)
	idf |= CH7xxx_IDF_HSP;

    if (mode->Flags & V_PVSYNC)
	idf |= CH7xxx_IDF_HSP;

    ch7xxx_write(dev_priv, CH7xxx_IDF, idf);
}

/* set the CH7xxx power state */
static void
ch7xxx_dpms(I2CDevPtr d, int mode)
{
    struct ch7xxx_priv *dev_priv = d->DriverPrivate.ptr;

    if (mode == DPMSModeOn)
	ch7xxx_write(dev_priv, CH7xxx_PM, CH7xxx_PM_DVIL | CH7xxx_PM_DVIP);
    else
	ch7xxx_write(dev_priv, CH7xxx_PM, CH7xxx_PM_FPD);
}

static void
ch7xxx_dump_regs(I2CDevPtr d)
{
    struct ch7xxx_priv *dev_priv = d->DriverPrivate.ptr;
    int i;

    for (i = 0; i < CH7xxx_NUM_REGS; i++) {
	if (( i % 8 ) == 0 )
	    ErrorF("\n %02X: ", i);
	ErrorF("%02X ", dev_priv->ModeReg.regs[i]);
    }
}

static void
ch7xxx_save(I2CDevPtr d)
{
    struct ch7xxx_priv *dev_priv = d->DriverPrivate.ptr;

    ch7xxx_read(dev_priv, CH7xxx_TCTL, &dev_priv->save_TCTL);
    ch7xxx_read(dev_priv, CH7xxx_TPCP, &dev_priv->save_TPCP);
    ch7xxx_read(dev_priv, CH7xxx_TPD, &dev_priv->save_TPD);
    ch7xxx_read(dev_priv, CH7xxx_TPVT, &dev_priv->save_TPVT);
    ch7xxx_read(dev_priv, CH7xxx_TLPF, &dev_priv->save_TLPF);
    ch7xxx_read(dev_priv, CH7xxx_PM, &dev_priv->save_PM);
    ch7xxx_read(dev_priv, CH7xxx_IDF, &dev_priv->save_IDF);
}

static void
ch7xxx_restore(I2CDevPtr d)
{
    struct ch7xxx_priv *dev_priv = d->DriverPrivate.ptr;

    ch7xxx_write(dev_priv, CH7xxx_TCTL, dev_priv->save_TCTL);
    ch7xxx_write(dev_priv, CH7xxx_TPCP, dev_priv->save_TPCP);
    ch7xxx_write(dev_priv, CH7xxx_TPD, dev_priv->save_TPD);
    ch7xxx_write(dev_priv, CH7xxx_TPVT, dev_priv->save_TPVT);
    ch7xxx_write(dev_priv, CH7xxx_TLPF, dev_priv->save_TLPF);
    ch7xxx_write(dev_priv, CH7xxx_IDF, dev_priv->save_IDF);
    ch7xxx_write(dev_priv, CH7xxx_PM, dev_priv->save_PM);
}

_X_EXPORT I830I2CVidOutputRec CH7xxxVidOutput = {
    .init = ch7xxx_init,
    .detect = ch7xxx_detect,
    .mode_valid = ch7xxx_mode_valid,
    .mode_set = ch7xxx_mode_set,
    .dpms = ch7xxx_dpms,
    .dump_regs = ch7xxx_dump_regs,
    .save = ch7xxx_save,
    .restore = ch7xxx_restore,
};
