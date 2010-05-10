/*
 * Copyright © 2006 Intel Corporation
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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

#include <unistd.h>

#include "../i2c_vid.h"
#include "../i830_bios.h"
#include "ivch_reg.h"

struct ivch_priv {
    I2CDevRec	    d;

    xf86OutputPtr   output;
    Bool quiet;

    uint16_t width, height;

    uint16_t save_VR01;
    uint16_t save_VR40;
};

struct vch_capabilities {
    struct aimdb_block	aimdb_block;
    uint8_t		panel_type;
    uint8_t		set_panel_type;
    uint8_t		slave_address;
    uint8_t		capabilities;
#define VCH_PANEL_FITTING_SUPPORT	(0x3 << 0)
#define VCH_PANEL_FITTING_TEXT		(1 << 2)
#define VCH_PANEL_FITTING_GRAPHICS	(1 << 3)
#define VCH_PANEL_FITTING_RATIO		(1 << 4)
#define VCH_DITHERING			(1 << 5)
    uint8_t		backlight_gpio;
    uint8_t		set_panel_type_us_gpios;
} __attribute__ ((packed));

static void
ivch_dump_regs(I2CDevPtr d);

/**
 * Reads a register on the ivch.
 *
 * Each of the 256 registers are 16 bits long.
 */
static Bool
ivch_read(struct ivch_priv *priv, int addr, uint16_t *data)
{
    I2CBusPtr b = priv->d.pI2CBus;
    I2CByte *p = (I2CByte *) data;

    if (!b->I2CStart(b, priv->d.StartTimeout))
	goto fail;

    if (!b->I2CPutByte(&priv->d, priv->d.SlaveAddr | 1))
	goto fail;

    if (!b->I2CPutByte(&priv->d, addr))
	goto fail;

    if (!b->I2CGetByte(&priv->d, p++, FALSE))
	goto fail;

    if (!b->I2CGetByte(&priv->d, p++, TRUE))
	goto fail;

    b->I2CStop(&priv->d);

    return TRUE;

 fail:
    if (!priv->quiet) {
	xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_ERROR,
		   "ivch: Unable to read register 0x%02x from %s:%02x.\n",
		   addr, priv->d.pI2CBus->BusName, priv->d.SlaveAddr);
    }
    b->I2CStop(&priv->d);

    return FALSE;
}
 
/** Writes a 16-bit register on the ivch */
static Bool
ivch_write(struct ivch_priv *priv, int addr, uint16_t data)
{
    I2CBusPtr b = priv->d.pI2CBus;

    if (!b->I2CStart(b, priv->d.StartTimeout))
	goto fail;

    if (!b->I2CPutByte(&priv->d, priv->d.SlaveAddr))
	goto fail;

    if (!b->I2CPutByte(&priv->d, addr))
	goto fail;

    if (!b->I2CPutByte(&priv->d, data & 0xff))
	goto fail;

    if (!b->I2CPutByte(&priv->d, data >> 8))
	goto fail;

    b->I2CStop(&priv->d);

    return TRUE;

 fail:
    b->I2CStop(&priv->d);

    if (!priv->quiet) {
	xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_ERROR,
		   "Unable to write register 0x%02x to %s:%d.\n",
		   addr, priv->d.pI2CBus->BusName, priv->d.SlaveAddr);
    }

    return FALSE;
}

/** Probes the given bus and slave address for an ivch */
static void *
ivch_init(I2CBusPtr b, I2CSlaveAddr addr)
{
    struct	ivch_priv *priv;
    uint16_t	temp;

    priv = xcalloc(1, sizeof(struct ivch_priv));
    if (priv == NULL)
	return NULL;

    priv->output = NULL;
    priv->d.DevName = "i82807aa \"ivch\" LVDS/CMOS panel controller";
    priv->d.SlaveAddr = addr;
    priv->d.pI2CBus = b;
    priv->d.StartTimeout = b->StartTimeout;
    priv->d.BitTimeout = b->BitTimeout;
    priv->d.AcknTimeout = b->AcknTimeout;
    priv->d.ByteTimeout = b->ByteTimeout;
    priv->d.DriverPrivate.ptr = priv;
    priv->quiet = TRUE;

    if (!ivch_read(priv, VR00, &temp))
	goto out;
    priv->quiet = FALSE;

    /* Since the identification bits are probably zeroes, which doesn't seem
     * very unique, check that the value in the base address field matches
     * the address it's responding on.
     */
    if ((temp & VR00_BASE_ADDRESS_MASK) != (priv->d.SlaveAddr >> 1)) {
	xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_ERROR,
		   "ivch detect failed due to address mismatch "
		   "(%d vs %d)\n",
		   (temp & VR00_BASE_ADDRESS_MASK), priv->d.SlaveAddr >> 1);
	goto out;
    }

    if (!xf86I2CDevInit(&priv->d)) {
	goto out;
    }

    ivch_read(priv, VR20, &priv->width);
    ivch_read(priv, VR21, &priv->height);

    return priv;

out:
    xfree(priv);
    return NULL;
}

static xf86OutputStatus
ivch_detect(I2CDevPtr d)
{
    return XF86OutputStatusConnected;
}

static ModeStatus
ivch_mode_valid(I2CDevPtr d, DisplayModePtr mode)
{
    if (mode->Clock > 112000)
	return MODE_CLOCK_HIGH;

    return MODE_OK;
}

/** Sets the power state of the panel connected to the ivch */
static void
ivch_dpms(I2CDevPtr d, int mode)
{
    struct ivch_priv *priv = d->DriverPrivate.ptr;
    int i;
    uint16_t vr01, vr30, backlight;

    /* Set the new power state of the panel. */
    if (!ivch_read(priv, VR01, &vr01))
	return;

    if (mode == DPMSModeOn)
	backlight = 1;
    else
	backlight = 0;
    ivch_write(priv, VR80, backlight);
    
    if (mode == DPMSModeOn)
	vr01 |= VR01_LCD_ENABLE | VR01_DVO_ENABLE;
    else
	vr01 &= ~(VR01_LCD_ENABLE | VR01_DVO_ENABLE);

    ivch_write(priv, VR01, vr01);

    /* Wait for the panel to make its state transition */
    for (i = 0; i < 100; i++) {
	if (!ivch_read(priv, VR30, &vr30))
	    break;

	if (((vr30 & VR30_PANEL_ON) != 0) == (mode == DPMSModeOn))
	    break;
	usleep (1000);
    }
    /* And wait some more; without this, the vch fails to resync sometimes */
    usleep (16 * 1000);
}

static void
ivch_mode_set(I2CDevPtr d, DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
    struct ivch_priv	*priv = d->DriverPrivate.ptr;
    uint16_t		vr40 = 0;
    uint16_t		vr01;

    vr01 = 0;
    vr40 = (VR40_STALL_ENABLE |
	    VR40_VERTICAL_INTERP_ENABLE |
	    VR40_HORIZONTAL_INTERP_ENABLE);
    
    if (mode->HDisplay != adjusted_mode->HDisplay || 
	mode->VDisplay != adjusted_mode->VDisplay)
    {
	uint16_t	x_ratio, y_ratio;
	
	vr01 |= VR01_PANEL_FIT_ENABLE;
	vr40 |= VR40_CLOCK_GATING_ENABLE;
	x_ratio = (((mode->HDisplay - 1) << 16) / (adjusted_mode->HDisplay - 1)) >> 2;
	y_ratio = (((mode->VDisplay - 1) << 16) / (adjusted_mode->VDisplay - 1)) >> 2;
	ivch_write (priv, VR42, x_ratio);
	ivch_write (priv, VR41, y_ratio);
    }
    else
    {
	vr01 &= ~VR01_PANEL_FIT_ENABLE;
	vr40 &= ~VR40_CLOCK_GATING_ENABLE;
    }
    vr40 &= ~VR40_AUTO_RATIO_ENABLE;

    ivch_write(priv, VR01, vr01);
    ivch_write(priv, VR40, vr40);

    ivch_dump_regs(d);
}

static void
ivch_dump_regs(I2CDevPtr d)
{
    struct ivch_priv *priv = d->DriverPrivate.ptr;
    uint16_t val;

    ivch_read(priv, VR00, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR00: 0x%04x\n", val);
    ivch_read(priv, VR01, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR01: 0x%04x\n", val);
    ivch_read(priv, VR30, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR30: 0x%04x\n", val);
    ivch_read(priv, VR40, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR40: 0x%04x\n", val);

    /* GPIO registers */
    ivch_read(priv, VR80, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR80: 0x%04x\n", val);
    ivch_read(priv, VR81, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR81: 0x%04x\n", val);
    ivch_read(priv, VR82, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR82: 0x%04x\n", val);
    ivch_read(priv, VR83, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR83: 0x%04x\n", val);
    ivch_read(priv, VR84, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR84: 0x%04x\n", val);
    ivch_read(priv, VR85, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR85: 0x%04x\n", val);
    ivch_read(priv, VR86, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR86: 0x%04x\n", val);
    ivch_read(priv, VR87, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR87: 0x%04x\n", val);
    ivch_read(priv, VR88, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR88: 0x%04x\n", val);

    /* Scratch register 0 - AIM Panel type */
    ivch_read(priv, VR8E, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR8E: 0x%04x\n", val);

    /* Scratch register 1 - Status register */
    ivch_read(priv, VR8F, &val);
    xf86DrvMsg(priv->d.pI2CBus->scrnIndex, X_INFO, "VR8F: 0x%04x\n", val);
}

static void
ivch_save(I2CDevPtr d)
{
    struct ivch_priv *priv = d->DriverPrivate.ptr;

    ivch_read(priv, VR01, &priv->save_VR01);
    ivch_read(priv, VR40, &priv->save_VR40);
}

static void
ivch_restore(I2CDevPtr d)
{
    struct ivch_priv *priv = d->DriverPrivate.ptr;

    ivch_write(priv, VR01, priv->save_VR01);
    ivch_write(priv, VR40, priv->save_VR40);
}


_X_EXPORT I830I2CVidOutputRec ivch_methods = {
    .init = ivch_init,
    .dpms = ivch_dpms,
    .save = ivch_save,
    .restore = ivch_restore,
    .mode_valid = ivch_mode_valid,
    .mode_set = ivch_mode_set,
    .detect = ivch_detect,
    .dump_regs = ivch_dump_regs,
};
