/*
 * Copyright (c) 2007 NVIDIA, Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <strings.h>

#include "g80_type.h"
#include "g80_display.h"
#include "g80_output.h"

static Bool G80ReadPortMapping(int scrnIndex, G80Ptr pNv)
{
    unsigned char *table2;
    unsigned char headerSize, entries;
    int i;
    CARD16 a;
    CARD32 b;

    /* Clear the i2c map to invalid */
    for(i = 0; i < 4; i++)
        pNv->i2cMap[i].dac = pNv->i2cMap[i].sor = -1;

    if(*(CARD16*)pNv->table1 != 0xaa55) goto fail;

    a = *(CARD16*)(pNv->table1 + 0x36);
    table2 = (unsigned char*)pNv->table1 + a;

    if(table2[0] != 0x40) goto fail;

    b = *(CARD32*)(table2 + 6);
    if(b != 0x4edcbdcb) goto fail;

    headerSize = table2[1];
    entries = table2[2];

    for(i = 0; i < entries; i++) {
        int type, port;
        ORNum or;

        b = *(CARD32*)&table2[headerSize + 8*i];
        type = b & 0xf;
        port = (b >> 4) & 0xf;
        or = ffs((b >> 24) & 0xf) - 1;

        if(type == 0xe) break;

        if(type < 4) {
            switch(type) {
                case 0: /* CRT */
                    if(pNv->i2cMap[port].dac != -1) {
                        xf86DrvMsg(scrnIndex, X_WARNING,
                                   "DDC routing table corrupt!  DAC %i -> %i "
                                   "for port %i\n",
                                   or, pNv->i2cMap[port].dac, port);
                    }
                    pNv->i2cMap[port].dac = or;
                    break;
                case 1: /* TV */
                    /* Ignore TVs */
                    break;

                case 2: /* TMDS */
                    if(pNv->i2cMap[port].sor != -1)
                        xf86DrvMsg(scrnIndex, X_WARNING,
                                   "DDC routing table corrupt!  SOR %i -> %i "
                                   "for port %i\n",
                                   or, pNv->i2cMap[port].sor, port);
                    pNv->i2cMap[port].sor = or;
                    break;

                case 3: /* LVDS */
                    pNv->lvds.present = TRUE;
                    pNv->lvds.or = or;
                    break;
            }
        }
    }

    xf86DrvMsg(scrnIndex, X_PROBED, "Connector map:\n");
    if(pNv->lvds.present)
        xf86DrvMsg(scrnIndex, X_PROBED, "  [N/A] -> SOR%i (LVDS)\n", pNv->lvds.or);
    for(i = 0; i < 4; i++) {
        if(pNv->i2cMap[i].dac != -1)
            xf86DrvMsg(scrnIndex, X_PROBED, "  Bus %i -> DAC%i\n", i, pNv->i2cMap[i].dac);
        if(pNv->i2cMap[i].sor != -1)
            xf86DrvMsg(scrnIndex, X_PROBED, "  Bus %i -> SOR%i\n", i, pNv->i2cMap[i].sor);
    }

    return TRUE;

fail:
    xf86DrvMsg(scrnIndex, X_ERROR, "Couldn't find the DDC routing table.  "
               "Mode setting will probably fail!\n");
    return FALSE;
}

static void G80_I2CPutBits(I2CBusPtr b, int clock, int data)
{
    G80Ptr pNv = G80PTR(xf86Screens[b->scrnIndex]);
    const int off = b->DriverPrivate.val * 0x18;

    pNv->reg[(0x0000E138+off)/4] = 4 | clock | data << 1;
}

static void G80_I2CGetBits(I2CBusPtr b, int *clock, int *data)
{
    G80Ptr pNv = G80PTR(xf86Screens[b->scrnIndex]);
    const int off = b->DriverPrivate.val * 0x18;
    unsigned char val;

    val = pNv->reg[(0x0000E138+off)/4];
    *clock = !!(val & 1);
    *data = !!(val & 2);
}

static I2CBusPtr
G80I2CInit(ScrnInfoPtr pScrn, const char *name, const int port)
{
    I2CBusPtr i2c;

    /* Allocate the I2C bus structure */
    i2c = xf86CreateI2CBusRec();
    if(!i2c) return NULL;

    i2c->BusName = strdup(name);
    i2c->scrnIndex = pScrn->scrnIndex;
    i2c->I2CPutBits = G80_I2CPutBits;
    i2c->I2CGetBits = G80_I2CGetBits;
    i2c->ByteTimeout = 2200; /* VESA DDC spec 3 p. 43 (+10 %) */
    i2c->StartTimeout = 550;
    i2c->BitTimeout = 40;
    i2c->ByteTimeout = 40;
    i2c->AcknTimeout = 40;
    i2c->DriverPrivate.val = port;

    if(xf86I2CBusInit(i2c)) {
        return i2c;
    } else {
        xfree(i2c);
        return NULL;
    }
}

void
G80OutputSetPClk(xf86OutputPtr output, int pclk)
{
    G80OutputPrivPtr pPriv = output->driver_private;
    if(pPriv->set_pclk)
        pPriv->set_pclk(output, pclk);
}

int
G80OutputModeValid(xf86OutputPtr output, DisplayModePtr mode)
{
    if(mode->Clock > 400000)
        return MODE_CLOCK_HIGH;
    if(mode->Clock < 25000)
        return MODE_CLOCK_LOW;

    return MODE_OK;
}

void
G80OutputPrepare(xf86OutputPtr output)
{
}

void
G80OutputCommit(xf86OutputPtr output)
{
}

static xf86MonPtr
ProbeDDC(I2CBusPtr i2c)
{
    ScrnInfoPtr pScrn = xf86Screens[i2c->scrnIndex];
    G80Ptr pNv = G80PTR(pScrn);
    xf86MonPtr monInfo = NULL;
    const int bus = i2c->DriverPrivate.val, off = bus * 0x18;

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
            "Probing for EDID on I2C bus %i...\n", bus);
    pNv->reg[(0x0000E138+off)/4] = 7;
    /* Should probably use xf86OutputGetEDID here */
    monInfo = xf86DoEDID_DDC2(pScrn->scrnIndex, i2c);
    pNv->reg[(0x0000E138+off)/4] = 3;

    if(monInfo) {
        xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                "DDC detected a %s:\n", monInfo->features.input_type ?
                "DFP" : "CRT");
        xf86PrintEDID(monInfo);
    } else {
        xf86DrvMsg(pScrn->scrnIndex, X_INFO, "  ... none found\n");
    }

    return monInfo;
}

/*
 * Read an EDID from the i2c port.  Perform load detection on the DAC (if
 * present) to see if the display is connected via VGA.  Sets the cached status
 * of both outputs.  The status is marked dirty again in the BlockHandler.
 */
void G80OutputPartnersDetect(xf86OutputPtr dac, xf86OutputPtr sor, I2CBusPtr i2c)
{
    xf86MonPtr monInfo = ProbeDDC(i2c);
    xf86OutputPtr connected = NULL;
    Bool load = dac && G80DacLoadDetect(dac);

    if(dac) {
        G80OutputPrivPtr pPriv = dac->driver_private;

        if(load) {
            pPriv->cached_status = XF86OutputStatusConnected;
            connected = dac;
        } else {
            pPriv->cached_status = XF86OutputStatusDisconnected;
        }
    }

    if(sor) {
        G80OutputPrivPtr pPriv = sor->driver_private;

        if(monInfo && !load) {
            pPriv->cached_status = XF86OutputStatusConnected;
            connected = sor;
        } else {
            pPriv->cached_status = XF86OutputStatusDisconnected;
        }
    }

    if(connected)
        xf86OutputSetEDID(connected, monInfo);
}

/*
 * Reset the cached output status for all outputs.  Called from G80BlockHandler.
 */
void
G80OutputResetCachedStatus(ScrnInfoPtr pScrn)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    for(i = 0; i < xf86_config->num_output; i++) {
        G80OutputPrivPtr pPriv = xf86_config->output[i]->driver_private;
        pPriv->cached_status = XF86OutputStatusUnknown;
    }
}

DisplayModePtr
G80OutputGetDDCModes(xf86OutputPtr output)
{
    /* The EDID is read as part of the detect step */
    output->funcs->detect(output);
    return xf86OutputGetEDIDModes(output);
}

void
G80OutputDestroy(xf86OutputPtr output)
{
    G80OutputPrivPtr pPriv = output->driver_private;

    if(pPriv->partner)
        ((G80OutputPrivPtr)pPriv->partner->driver_private)->partner = NULL;
    else
        xf86DestroyI2CBusRec(pPriv->i2c, TRUE, TRUE);
    pPriv->i2c = NULL;
}

Bool
G80CreateOutputs(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    if(!G80ReadPortMapping(pScrn->scrnIndex, pNv))
        return FALSE;

    /* For each DDC port, create an output for the attached ORs */
    for(i = 0; i < 4; i++) {
        xf86OutputPtr dac = NULL, sor = NULL;
        I2CBusPtr i2c;
        char i2cName[16];

        if(pNv->i2cMap[i].dac == -1 && pNv->i2cMap[i].sor == -1)
            /* No outputs on this port */
            continue;

        snprintf(i2cName, sizeof(i2cName), "I2C%i", i);
        i2c = G80I2CInit(pScrn, i2cName, i);
        if(!i2c) {
            xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                       "Failed to initialize I2C for port %i.\n",
                       i);
            continue;
        }

        if(pNv->i2cMap[i].dac != -1)
            dac = G80CreateDac(pScrn, pNv->i2cMap[i].dac);
        if(pNv->i2cMap[i].sor != -1)
            sor = G80CreateSor(pScrn, pNv->i2cMap[i].sor, TMDS);

        if(dac) {
            G80OutputPrivPtr pPriv = dac->driver_private;

            pPriv->partner = sor;
            pPriv->i2c = i2c;
            pPriv->scale = G80_SCALE_OFF;
        }
        if(sor) {
            G80OutputPrivPtr pPriv = sor->driver_private;

            pPriv->partner = dac;
            pPriv->i2c = i2c;
            pPriv->scale = G80_SCALE_ASPECT;
        }
    }

    if(pNv->lvds.present) {
        xf86OutputPtr lvds = G80CreateSor(pScrn, pNv->lvds.or, LVDS);
        G80OutputPrivPtr pPriv = lvds->driver_private;

        pPriv->scale = G80_SCALE_ASPECT;
    }

    /* For each output, set the crtc and clone masks */
    for(i = 0; i < xf86_config->num_output; i++) {
        xf86OutputPtr output = xf86_config->output[i];

        /* Any output can connect to any head */
        output->possible_crtcs = 0x3;
        output->possible_clones = 0;
    }

    return TRUE;
}
