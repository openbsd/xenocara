/*
 * Copyright (c) 2007-2008 NVIDIA, Corporation
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

static unsigned G80FindLoadVal(const unsigned char *table1)
{
    const unsigned char *p = table1;
    int count;
    const CARD32 def = 340;

    for(p = table1; *(CARD16*)p != 0xb8ff && p < table1 + 64000; p += 2);
    if(p == table1 + 64000)
        return def;
    p += 2;
    if(*(CARD32*)p != 0x544942)
        return def;
    p += 4;
    if(*(CARD16*)p != 0x100)
        return def;
    p += 2;
    if(*p != 12)
        return def;
    p++;
    if(*p != 6)
        return def;
    p++;
    count = *p;
    p += 2;
    for(; *p != 'A' && count >= 0; count--, p += 6);
    if(count == -1)
        return def;
    p += 4;
    p = table1 + *(CARD16*)p;
    p = table1 + *(CARD16*)p;
    if(p[0] != 0x10 || p[1] != 4 || p[2] != 4 || p[3] != 2)
        return def;
    return *(CARD32*)(p + 4) & 0x3ff;
}

static Bool G80ReadPortMapping(int scrnIndex, G80Ptr pNv)
{
    unsigned char *table2, *table3;
    unsigned char headerSize, entries, table3Entries, table3EntSize;
    int i;
    CARD16 a;
    CARD32 b;

    /* Clear the i2c map to invalid */
    for(i = 0; i < G80_NUM_I2C_PORTS; i++)
        pNv->i2cMap[i].dac = pNv->i2cMap[i].sor = -1;

    if(*(CARD16*)pNv->table1 != 0xaa55) goto fail;

    a = *(CARD16*)(pNv->table1 + 0x36);
    table2 = (unsigned char*)pNv->table1 + a;

    if(table2[0] != 0x40) goto fail;

    b = *(CARD32*)(table2 + 6);
    if(b != 0x4edcbdcb) goto fail;

    table3 = (unsigned char*)pNv->table1 + *(CARD16*)(table2 + 4);
    table3Entries = table3[2];
    table3EntSize = table3[3];
    table3 += table3[1];

    headerSize = table2[1];
    entries = table2[2];

    for(i = 0; i < entries; i++) {
        int type, port, portType;
        ORNum or;

        b = *(CARD32*)&table2[headerSize + 8*i];
        type = b & 0xf;
        port = (b >> 4) & 0xf;
        or = ffs((b >> 24) & 0xf) - 1;

        if(b & 0x300000) {
            /* Can't handle this type of output yet */
            xf86DrvMsg(scrnIndex, X_INFO,
                       "Ignoring unsupported external output type %d at output "
                       "%d\n", type, or);
            continue;
        }

        if(type == 0xe) break;

        switch(type) {
            case 0: /* CRT */
                if(port >= table3Entries) {
                    xf86DrvMsg(scrnIndex, X_WARNING,
                               "VGA%d: invalid port %d\n", or, port);
                    break;
                }
                b = *(CARD32*)&table3[table3EntSize * port];
                port = b & 0xff;
                portType = b >> 24;
                if(portType != 5) {
                    xf86DrvMsg(scrnIndex, X_WARNING,
                               "VGA%d: invalid port type %d\n", or, portType);
                    break;
                }
                if(port >= G80_NUM_I2C_PORTS) {
                    xf86DrvMsg(scrnIndex, X_WARNING,
                               "VGA%d: unrecognized port %d\n", or, port);
                    break;
                }
                if(pNv->i2cMap[port].dac != -1) {
                    xf86DrvMsg(scrnIndex, X_WARNING,
                               "DDC routing table corrupt!  DAC %i -> %i for "
                               "port %i\n", or, pNv->i2cMap[port].dac, port);
                }
                pNv->i2cMap[port].dac = or;
                break;
            case 1: /* TV */
                xf86DrvMsg(scrnIndex, X_INFO,
                           "Ignoring unsupported TV output %d\n", or);
                break;

            case 2: /* TMDS */
                if(port >= table3Entries) {
                    xf86DrvMsg(scrnIndex, X_WARNING,
                               "DVI%d: invalid port %d\n", or, port);
                    break;
                }
                b = *(CARD32*)&table3[table3EntSize * port];
                port = b & 0xff;
                portType = b >> 24;
                if(portType != 5) {
                    xf86DrvMsg(scrnIndex, X_WARNING,
                               "DVI%d: invalid port type %d\n", or, portType);
                    break;
                }
                if(port >= G80_NUM_I2C_PORTS) {
                    xf86DrvMsg(scrnIndex, X_WARNING,
                               "DVI%d: unrecognized port %d\n", or, port);
                    break;
                }
                if(pNv->i2cMap[port].sor != -1)
                    xf86DrvMsg(scrnIndex, X_WARNING,
                               "DDC routing table corrupt!  SOR %i -> %i for "
                               "port %i\n", or, pNv->i2cMap[port].sor, port);
                pNv->i2cMap[port].sor = or;
                break;

            case 3: /* LVDS */
                pNv->lvds.present = TRUE;
                pNv->lvds.or = or;
                pNv->lvds.i2cPort = -1;

                if(port == 15) {
                    xf86DrvMsg(scrnIndex, X_INFO, "LVDS has no I2C port\n");
                    break;
                }
                if(port >= table3Entries) {
                    xf86DrvMsg(scrnIndex, X_WARNING,
                               "LVDS: invalid port %d\n", port);
                    break;
                }
                b = *(CARD32*)&table3[table3EntSize * port];
                port = b & 0xff;
                portType = b >> 24;
                if(portType != 5) {
                    xf86DrvMsg(scrnIndex, X_WARNING,
                               "LVDS: invalid port type %d\n", portType);
                    break;
                }
                if(port >= G80_NUM_I2C_PORTS) {
                    xf86DrvMsg(scrnIndex, X_WARNING,
                               "LVDS: unrecognized port %d\n", port);
                    break;
                }
                pNv->lvds.i2cPort = port;

                break;

            case 6: /* DisplayPort */
                xf86DrvMsg(scrnIndex, X_INFO,
                           "Ignoring unsupported DisplayPort output %d\n", or);
                break;

            default:
                xf86DrvMsg(scrnIndex, X_INFO,
                           "Ignoring unsupported output type %d at port %d\n",
                           type, or);
                break;
        }
    }

    xf86DrvMsg(scrnIndex, X_PROBED, "Connector map:\n");
    if(pNv->lvds.present) {
        if (pNv->lvds.i2cPort != -1)
            xf86DrvMsg(scrnIndex, X_PROBED, "  Bus %i -> SOR%i (LVDS)\n", pNv->lvds.i2cPort, pNv->lvds.or);
        else
            xf86DrvMsg(scrnIndex, X_PROBED, "  [N/A] -> SOR%i (LVDS)\n", pNv->lvds.or);
    }
    for(i = 0; i < G80_NUM_I2C_PORTS; i++) {
        if(pNv->i2cMap[i].dac != -1)
            xf86DrvMsg(scrnIndex, X_PROBED, "  Bus %i -> DAC%i\n", i, pNv->i2cMap[i].dac);
        if(pNv->i2cMap[i].sor != -1)
            xf86DrvMsg(scrnIndex, X_PROBED, "  Bus %i -> SOR%i\n", i, pNv->i2cMap[i].sor);
    }

    pNv->loadVal = G80FindLoadVal(pNv->table1);
    xf86DrvMsg(scrnIndex, X_PROBED, "Load detection: %d\n", pNv->loadVal);

    return TRUE;

fail:
    xf86DrvMsg(scrnIndex, X_ERROR, "Couldn't find the DDC routing table.  "
               "Mode setting will probably fail!\n");
    return FALSE;
}

static CARD32 i2cAddr(const int port)
{
    const CARD32 addrs[G80_NUM_I2C_PORTS] = {
        0xE138, 0xE150, 0xE168, 0xE180, 0xE254, 0xE274, 0xE764, 0xE780, 0xE79C,
        0xE7B8
    };
    return addrs[port];
}

static void G80_I2CPutBits(I2CBusPtr b, int clock, int data)
{
#ifdef XF86_SCRN_INTERFACE
    G80Ptr pNv = G80PTR(b->pScrn);
#else
    G80Ptr pNv = G80PTR(xf86Screens[b->scrnIndex]);
#endif
    pNv->reg[i2cAddr(b->DriverPrivate.val)/4] = 4 | clock | data << 1;
}

static void G80_I2CGetBits(I2CBusPtr b, int *clock, int *data)
{
#ifdef XF86_SCRN_INTERFACE
    G80Ptr pNv = G80PTR(b->pScrn);
#else
    G80Ptr pNv = G80PTR(xf86Screens[b->scrnIndex]);
#endif
    unsigned char val;

    val = pNv->reg[i2cAddr(b->DriverPrivate.val)/4];
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
#ifdef XF86_SCRN_INTERFACE
    i2c->pScrn = pScrn;
#endif
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
        free(i2c);
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
    const int bus = i2c->DriverPrivate.val;
    const CARD32 addr = i2cAddr(bus);

    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
            "Probing for EDID on I2C bus %i...\n", bus);
    pNv->reg[addr/4] = 7;
    /* Should probably use xf86OutputGetEDID here */
#ifdef EDID_COMPLETE_RAWDATA
    monInfo = xf86DoEEDID(XF86_SCRN_ARG(pScrn), i2c, TRUE);
#else
    monInfo = xf86DoEDID_DDC2(XF86_SCRN_ARG(pScrn), i2c);
#endif
    pNv->reg[addr/4] = 3;

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
    for(i = 0; i < G80_NUM_I2C_PORTS; i++) {
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

        if (lvds) {
            G80OutputPrivPtr pPriv = lvds->driver_private;

            pPriv->scale = G80_SCALE_ASPECT;

            if(pNv->lvds.i2cPort != -1) {
                char i2cName[16];

                snprintf(i2cName, sizeof(i2cName), "I2C%i (LVDS)", pNv->lvds.i2cPort);
                pPriv->i2c = G80I2CInit(pScrn, i2cName, pNv->lvds.i2cPort);
                if(!pPriv->i2c) {
                    xf86DrvMsg(pScrn->scrnIndex, X_ERROR,
                               "Failed to initialize I2C for port %i (LVDS)!\n",
                               pNv->lvds.i2cPort);
                }
            }
        }
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
