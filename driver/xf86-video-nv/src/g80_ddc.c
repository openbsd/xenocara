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
#include <xf86DDC.h>

#include "g80_type.h"
#include "g80_ddc.h"
#include "g80_display.h"

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

        if(type < 4 && port != 0xf) {
            switch(type) {
                case 0: /* CRT */
                case 1: /* TV */
                    if(pNv->i2cMap[port].dac != -1) {
                        xf86DrvMsg(scrnIndex, X_WARNING,
                                   "DDC routing table corrupt!  DAC %i -> %i "
                                   "for port %i\n",
                                   or, pNv->i2cMap[port].dac, port);
                    }
                    pNv->i2cMap[port].dac = or;
                    break;
                case 2: /* TMDS */
                case 3: /* LVDS */
                    if(pNv->i2cMap[port].sor != -1)
                        xf86DrvMsg(scrnIndex, X_WARNING,
                                   "DDC routing table corrupt!  SOR %i -> %i "
                                   "for port %i\n",
                                   or, pNv->i2cMap[port].sor, port);
                    pNv->i2cMap[port].sor = or;
                    break;
            }
        }
    }

    xf86DrvMsg(scrnIndex, X_PROBED, "I2C map:\n");
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
    const int off = b->DriverPrivate.val;

    pNv->reg[(0x0000E138+off)/4] = 4 | clock | data << 1;
}

static void G80_I2CGetBits(I2CBusPtr b, int *clock, int *data)
{
    G80Ptr pNv = G80PTR(xf86Screens[b->scrnIndex]);
    const int off = b->DriverPrivate.val;
    unsigned char val;

    val = pNv->reg[(0x0000E138+off)/4];
    *clock = !!(val & 1);
    *data = !!(val & 2);
}

static xf86MonPtr G80ProbeDDCBus(ScrnInfoPtr pScrn, int bus)
{
    G80Ptr pNv = G80PTR(pScrn);
    I2CBusPtr i2c;
    xf86MonPtr monInfo = NULL;
    const int off = bus * 0x18;

    /* Allocate the I2C bus structure */
    i2c = xf86CreateI2CBusRec();
    if(!i2c) return NULL;

    i2c->BusName = "DDC";
    i2c->scrnIndex = pScrn->scrnIndex;
    i2c->I2CPutBits = G80_I2CPutBits;
    i2c->I2CGetBits = G80_I2CGetBits;
    i2c->ByteTimeout = 2200; /* VESA DDC spec 3 p. 43 (+10 %) */
    i2c->StartTimeout = 550;
    i2c->BitTimeout = 40;
    i2c->ByteTimeout = 40;
    i2c->AcknTimeout = 40;
    i2c->DriverPrivate.val = off;

    if(!xf86I2CBusInit(i2c)) goto done;

    pNv->reg[(0x0000E138+off)/4] = 7;
    xf86DrvMsg(pScrn->scrnIndex, X_INFO,
            "Probing for EDID on I2C bus %i...\n", bus);
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

done:
    xf86DestroyI2CBusRec(i2c, TRUE, TRUE);

    return monInfo;
}

/*
 * Try DDC on each bus until we find one that works.
 */
Bool G80ProbeDDC(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);
    xf86MonPtr monInfo;
    int port;
    Bool flatPanel;

    if(!G80ReadPortMapping(pScrn->scrnIndex, pNv))
        return FALSE;

    for(port = 0; port < 4; port++) {
        if(pNv->i2cMap[port].dac == -1 && pNv->i2cMap[port].sor == -1)
            /* No outputs on this port.  Skip it. */
            continue;

        monInfo = G80ProbeDDCBus(pScrn, port);
        if(!monInfo)
            /* No EDID on this port. */
            continue;

        flatPanel = (monInfo->features.input_type == 1);

        if(pNv->i2cMap[port].dac != -1 &&
           G80DispDetectLoad(pScrn, pNv->i2cMap[port].dac)) {
            pNv->orType = DAC;
            pNv->or = pNv->i2cMap[port].dac;
        } else if(pNv->i2cMap[port].sor != -1) {
            pNv->orType = SOR;
            pNv->or = pNv->i2cMap[port].sor;
        } else {
            xf86DrvMsg(pScrn->scrnIndex, X_WARNING,
                       "Saw an EDID on I2C port %i but no DAC load was "
                       "detected and no SOR is connected to this port.  Using "
                       "DAC%i.\n", port,
                       pNv->or);
            pNv->orType = DAC;
            pNv->or = pNv->i2cMap[port].dac;
        }

        xf86DrvMsg(pScrn->scrnIndex, X_PROBED,
                   "Found a %s on I2C port %i, assigning %s%i\n",
                   flatPanel ? "flat panel" : "CRT",
                   port, pNv->orType == SOR ? "SOR" : "DAC", pNv->or);

        pScrn->monitor->DDC = monInfo;
        xf86SetDDCproperties(pScrn, monInfo);

        return TRUE;
    }

    return FALSE;
}
