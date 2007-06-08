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

#include <float.h>
#include <math.h>
#include <strings.h>
#include <unistd.h>

#include "g80_type.h"
#include "g80_display.h"

#define DPMS_SERVER
#include <X11/extensions/dpms.h>

/*
 * PLL calculation.  pclk is in kHz.
 */
static void
G80CalcPLL(float pclk, int *pNA, int *pMA, int *pNB, int *pMB, int *pP)
{
    const float refclk = 27000.0f;
    const float minVcoA = 100000;
    const float maxVcoA = 400000;
    const float minVcoB = 600000;
    float maxVcoB = 1400000;
    const float minUA = 2000;
    const float maxUA = 400000;
    const float minUB = 50000;
    const float maxUB = 200000;
    const int minNA = 1, maxNA = 255;
    const int minNB = 1, maxNB = 31;
    const int minMA = 1, maxMA = 255;
    const int minMB = 1, maxMB = 31;
    const int minP = 0, maxP = 6;
    int lowP, highP;
    float vcoB;

    int na, ma, nb, mb, p;
    float bestError = FLT_MAX;

    *pNA = *pMA = *pNB = *pMB = *pP = 0;

    if(maxVcoB < pclk + pclk / 200)
        maxVcoB = pclk + pclk / 200;
    if(minVcoB / (1 << maxP) > pclk)
        pclk = minVcoB / (1 << maxP);

    vcoB = maxVcoB - maxVcoB / 200;
    lowP = minP;
    vcoB /= 1 << (lowP + 1);

    while(pclk <= vcoB && lowP < maxP)
    {
        vcoB /= 2;
        lowP++;
    }

    vcoB = maxVcoB + maxVcoB / 200;
    highP = lowP;
    vcoB /= 1 << (highP + 1);

    while(pclk <= vcoB && highP < maxP)
    {
        vcoB /= 2;
        highP++;
    }

    for(p = lowP; p <= highP; p++)
    {
        for(ma = minMA; ma <= maxMA; ma++)
        {
            if(refclk / ma < minUA)
                break;
            else if(refclk / ma > maxUA)
                continue;

            for(na = minNA; na <= maxNA; na++)
            {
                if(refclk * na / ma < minVcoA || refclk * na / ma > maxVcoA)
                    continue;

                for(mb = minMB; mb <= maxMB; mb++)
                {
                    if(refclk * na / ma / mb < minUB)
                        break;
                    else if(refclk * na / ma / mb > maxUB)
                        continue;

                    nb = rint(pclk * (1 << p) * (ma / (float)na) * mb / refclk);

                    if(nb > maxNB)
                        break;
                    else if(nb < minNB)
                        continue;
                    else
                    {
                        float freq = refclk * (na / (float)ma) * (nb / (float)mb) / (1 << p);
                        float error = fabsf(pclk - freq);
                        if(error < bestError) {
                            *pNA = na;
                            *pMA = ma;
                            *pNB = nb;
                            *pMB = mb;
                            *pP = p;
                            bestError = error;
                        }
                    }
                }
            }
        }
    }
}

static void
G80DispCommand(G80Ptr pNv, CARD32 addr, CARD32 data)
{
    pNv->reg[0x00610304/4] = data;
    pNv->reg[0x00610300/4] = addr | 0x80010001;

    while(pNv->reg[0x00610300/4] & 0x80000000) {
        const int super = ffs((pNv->reg[0x00610024/4] >> 4) & 7);

        if(super) {
            if(super == 2) {
                const int headOff = 0x800 * pNv->head;
                const int orOff = 0x800 * pNv->or;

                if(pNv->reg[0x00610030/4] & 0x600) {
                    int lo_n, lo_m, hi_n, hi_m, p;
                    CARD32 lo = pNv->reg[(0x00614104+headOff)/4];
                    CARD32 hi = pNv->reg[(0x00614108+headOff)/4];

                    pNv->reg[(0x00614100+headOff)/4] = 0x10000610;
                    lo &= 0xff00ff00;
                    hi &= 0x8000ff00;

                    G80CalcPLL(pNv->pclk, &lo_n, &lo_m, &hi_n, &hi_m, &p);

                    lo |= (lo_m << 16) | lo_n;
                    hi |= (p << 28) | (hi_m << 16) | hi_n;
                    pNv->reg[(0x00614104+headOff)/4] = lo;
                    pNv->reg[(0x00614108+headOff)/4] = hi;
                }

                pNv->reg[(0x00614200+headOff)/4] = 0;
                switch(pNv->orType) {
                case DAC:
                    pNv->reg[(0x00614280+orOff)/4] = 0;
                    break;
                case SOR:
                    pNv->reg[(0x00614300+orOff)/4] =
                        (pNv->pclk > 165000) ? 0x101 : 0;
                    break;
                }
            }

            pNv->reg[0x00610024/4] = 8 << super;
            pNv->reg[0x00610030/4] = 0x80000000;
        }
    }
}
#define C(mthd, data) G80DispCommand(pNv, (mthd), (data))

/*
 * Performs load detection on a single DAC.
 */
Bool G80DispDetectLoad(ScrnInfoPtr pScrn, ORNum or)
{
    G80Ptr pNv = G80PTR(pScrn);
    const int dacOff = 2048 * or;
    CARD32 load, tmp;

    pNv->reg[(0x0061A010+dacOff)/4] = 0x00000001;
    pNv->reg[(0x0061A004+dacOff)/4] = 0x80150000;
    while(pNv->reg[(0x0061A004+dacOff)/4] & 0x80000000);
    tmp = pNv->architecture == 0x50 ? 420 : 340;
    pNv->reg[(0x0061A00C+dacOff)/4] = tmp | 0x100000;
    usleep(4500);
    load = pNv->reg[(0x0061A00C+dacOff)/4];
    pNv->reg[(0x0061A00C+dacOff)/4] = 0;
    pNv->reg[(0x0061A004+dacOff)/4] = 0x80550000;

    return (load & 0x38000000) == 0x38000000;
}

/*
 * Performs load detection on the DACs.  Sets pNv->orType and pNv->or
 * accordingly.
 */
Bool G80LoadDetect(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);
    const int scrnIndex = pScrn->scrnIndex;
    ORNum or;

    pNv->orType = DAC;

    for(or = DAC0; or <= DAC2; or++) {
        xf86DrvMsg(scrnIndex, X_PROBED, "Trying load detection on DAC%i ... ", or);

        if(G80DispDetectLoad(pScrn, or)) {
            xf86ErrorF("found one!\n");
            pNv->or = or;
            return TRUE;
        }

        xf86ErrorF("nothing.\n");
    }

    return FALSE;
}

Bool
G80DispInit(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);

    pNv->reg[0x00610184/4] = pNv->reg[0x00614004/4];
    pNv->reg[0x00610190/4] = pNv->reg[0x00616100/4];
    pNv->reg[0x006101a0/4] = pNv->reg[0x00616900/4];
    pNv->reg[0x00610194/4] = pNv->reg[0x00616104/4];
    pNv->reg[0x006101a4/4] = pNv->reg[0x00616904/4];
    pNv->reg[0x00610198/4] = pNv->reg[0x00616108/4];
    pNv->reg[0x006101a8/4] = pNv->reg[0x00616908/4];
    pNv->reg[0x0061019C/4] = pNv->reg[0x0061610C/4];
    pNv->reg[0x006101ac/4] = pNv->reg[0x0061690c/4];
    pNv->reg[0x006101D0/4] = pNv->reg[0x0061A000/4];
    pNv->reg[0x006101D4/4] = pNv->reg[0x0061A800/4];
    pNv->reg[0x006101D8/4] = pNv->reg[0x0061B000/4];
    pNv->reg[0x006101E0/4] = pNv->reg[0x0061C000/4];
    pNv->reg[0x006101E4/4] = pNv->reg[0x0061C800/4];
    pNv->reg[0x0061c00c/4] = 0x03010700;
    pNv->reg[0x0061c010/4] = 0x0000152f;
    pNv->reg[0x0061c014/4] = 0x00000000;
    pNv->reg[0x0061c018/4] = 0x00245af8;
    pNv->reg[0x0061c80c/4] = 0x03010700;
    pNv->reg[0x0061c810/4] = 0x0000152f;
    pNv->reg[0x0061c814/4] = 0x00000000;
    pNv->reg[0x0061c818/4] = 0x00245af8;
    pNv->reg[0x0061A004/4] = 0x80550000;
    pNv->reg[0x0061A010/4] = 0x00000001;
    pNv->reg[0x0061A804/4] = 0x80550000;
    pNv->reg[0x0061A810/4] = 0x00000001;
    pNv->reg[0x0061B004/4] = 0x80550000;
    pNv->reg[0x0061B010/4] = 0x00000001;

    if(pNv->reg[0x00610024/4] & 0x100) {
        pNv->reg[0x00610024/4] = 0x100;
        pNv->reg[0x006194E8/4] &= ~1;
        while(pNv->reg[0x006194E8/4] & 2);
    }

    pNv->reg[0x00610200/4] = 0x2b00;
    while((pNv->reg[0x00610200/4] & 0x1e0000) != 0);
    pNv->reg[0x00610300/4] = 1;
    pNv->reg[0x00610200/4] = 0x1000b03;
    while(!(pNv->reg[0x00610200/4] & 0x40000000));

    C(0x00000084, 0);
    C(0x00000088, 0);
    C(0x00000874, 0);
    C(0x00000800, 0);
    C(0x00000810, 0);
    C(0x0000082C, 0);

    return TRUE;
}

void
G80DispShutdown(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);
    CARD32 mask;

    G80DispBlankScreen(pScrn, TRUE);

    mask = 4 << pNv->head;
    pNv->reg[0x00610024/4] = mask;
    while(!(pNv->reg[0x00610024/4] & mask));
    pNv->reg[0x00610200/4] = 0;
    pNv->reg[0x00610300/4] = 0;
    while((pNv->reg[0x00610200/4] & 0x1e0000) != 0);
}

static void
setupDAC(G80Ptr pNv, Head head, ORNum or, DisplayModePtr mode)
{
    const int dacOff = 0x80 * pNv->or;

    C(0x00000400 + dacOff, (head == HEAD0 ? 1 : 2) | 0x40);
    C(0x00000404 + dacOff,
        (mode->Flags & V_NHSYNC) ? 1 : 0 |
        (mode->Flags & V_NVSYNC) ? 2 : 0);
}

static void
setupSOR(G80Ptr pNv, Head head, ORNum or, DisplayModePtr mode)
{
    const int sorOff = 0x40 * pNv->or;

    C(0x00000600 + sorOff,
        (head == HEAD0 ? 1 : 2) |
        (mode->SynthClock > 165000 ? 0x500 : 0x100) |
        ((mode->Flags & V_NHSYNC) ? 0x1000 : 0) |
        ((mode->Flags & V_NVSYNC) ? 0x2000 : 0));
}

Bool
G80DispSetMode(ScrnInfoPtr pScrn, DisplayModePtr mode)
{
    G80Ptr pNv = G80PTR(pScrn);
    const int HDisplay = mode->HDisplay, VDisplay = mode->VDisplay;
    const int headOff = 0x400 * pNv->head;
    int interlaceDiv, fudge;

    if(pNv->BackendMode)
        mode = pNv->BackendMode;

    pNv->pclk = mode->SynthClock;

    /* Magic mode timing fudge factor */
    fudge = ((mode->Flags & V_INTERLACE) && (mode->Flags & V_DBLSCAN)) ? 2 : 1;
    interlaceDiv = (mode->Flags & V_INTERLACE) ? 2 : 1;

    switch(pNv->orType) {
    case DAC:
        setupDAC(pNv, pNv->head, pNv->or, mode);
        break;
    case SOR:
        setupSOR(pNv, pNv->head, pNv->or, mode);
        break;
    }

    C(0x00000804 + headOff, mode->SynthClock | 0x800000);
    C(0x00000808 + headOff, (mode->Flags & V_INTERLACE) ? 2 : 0);
    C(0x00000810 + headOff, 0);
    C(0x0000082C + headOff, 0);
    C(0x00000814 + headOff, mode->CrtcVTotal << 16 | mode->CrtcHTotal);
    C(0x00000818 + headOff,
        ((mode->CrtcVSyncEnd - mode->CrtcVSyncStart) / interlaceDiv - 1) << 16 |
        (mode->CrtcHSyncEnd - mode->CrtcHSyncStart - 1));
    C(0x0000081C + headOff,
        ((mode->CrtcVBlankEnd - mode->CrtcVSyncStart) / interlaceDiv - fudge) << 16 |
        (mode->CrtcHBlankEnd - mode->CrtcHSyncStart - 1));
    C(0x00000820 + headOff,
        ((mode->CrtcVTotal - mode->CrtcVSyncStart + mode->CrtcVBlankStart) / interlaceDiv - fudge) << 16 |
        (mode->CrtcHTotal - mode->CrtcHSyncStart + mode->CrtcHBlankStart - 1));
    if(mode->Flags & V_INTERLACE) {
        C(0x00000824 + headOff,
            ((mode->CrtcVTotal + mode->CrtcVBlankEnd - mode->CrtcVSyncStart) / 2 - 2) << 16 |
            ((2*mode->CrtcVTotal - mode->CrtcVSyncStart + mode->CrtcVBlankStart) / 2 - 2));
    }
    C(0x00000868 + headOff, pScrn->virtualY << 16 | pScrn->virtualX);
    C(0x0000086C + headOff, pScrn->displayWidth * (pScrn->bitsPerPixel / 8) | 0x100000);
    switch(pScrn->depth) {
        case  8: C(0x00000870 + headOff, 0x1E00); break;
        case 15: C(0x00000870 + headOff, 0xE900); break;
        case 16: C(0x00000870 + headOff, 0xE800); break;
        case 24: C(0x00000870 + headOff, 0xCF00); break;
    }
    C(0x000008A0 + headOff, 0);
    if((mode->Flags & V_DBLSCAN) || (mode->Flags & V_INTERLACE) ||
       mode->CrtcHDisplay != HDisplay || mode->CrtcVDisplay != VDisplay) {
        C(0x000008A4 + headOff, 9);
    } else {
        C(0x000008A4 + headOff, 0);
    }
    C(0x000008A8 + headOff, 0x40000);
    /* Use the screen's panning, but not if it's bogus */
    if(pScrn->frameX0 >= 0 && pScrn->frameY0 >= 0 &&
       pScrn->frameX0 + HDisplay <= pScrn->virtualX &&
       pScrn->frameY0 + VDisplay <= pScrn->virtualY) {
        C(0x000008C0 + headOff, pScrn->frameY0 << 16 | pScrn->frameX0);
    } else {
        C(0x000008C0 + headOff, 0);
    }
    C(0x000008C8 + headOff, VDisplay << 16 | HDisplay);
    C(0x000008D4 + headOff, 0);
    C(0x000008D8 + headOff, mode->CrtcVDisplay << 16 | mode->CrtcHDisplay);
    C(0x000008DC + headOff, mode->CrtcVDisplay << 16 | mode->CrtcHDisplay);

    G80DispBlankScreen(pScrn, FALSE);

    return TRUE;
}

void
G80DispAdjustFrame(G80Ptr pNv, int x, int y)
{
    const int headOff = 0x400 * pNv->head;

    C(0x000008C0 + headOff, y << 16 | x);
    C(0x00000080, 0);
}

void
G80DispBlankScreen(ScrnInfoPtr pScrn, Bool blank)
{
    G80Ptr pNv = G80PTR(pScrn);
    const int headOff = 0x400 * pNv->head;

    if(blank) {
        G80DispHideCursor(pNv, FALSE);

        C(0x00000840 + headOff, 0);
        C(0x00000844 + headOff, 0);
        if(pNv->architecture != 0x50)
            C(0x0000085C + headOff, 0);
        C(0x00000874 + headOff, 0);
        if(pNv->architecture != 0x50)
            C(0x0000089C + headOff, 0);
    } else {
        C(0x00000860 + headOff, 0);
        C(0x00000864 + headOff, 0);
        pNv->reg[0x00610380/4] = 0;
        pNv->reg[0x00610384/4] = pNv->RamAmountKBytes * 1024 - 1;
        pNv->reg[0x00610388/4] = 0x150000;
        pNv->reg[0x0061038C/4] = 0;
        C(0x00000884 + headOff, (pNv->videoRam << 2) - 0x40);
        if(pNv->architecture != 0x50)
            C(0x0000089C + headOff, 1);
        if(pNv->cursorVisible)
            G80DispShowCursor(pNv, FALSE);
        C(0x00000840 + headOff, pScrn->depth == 8 ? 0x80000000 : 0xc0000000);
        C(0x00000844 + headOff, (pNv->videoRam * 1024 - 0x5000) >> 8);
        if(pNv->architecture != 0x50)
            C(0x0000085C + headOff, 1);
        C(0x00000874 + headOff, 1);
    }

    C(0x00000080, 0);
}

void
G80DispDPMSSet(ScrnInfoPtr pScrn, int mode, int flags)
{
    G80Ptr pNv = G80PTR(pScrn);
    const int off = 0x800 * pNv->or;
    CARD32 tmp;

    /*
     * DPMSModeOn       everything on
     * DPMSModeStandby  hsync disabled, vsync enabled
     * DPMSModeSuspend  hsync enabled, vsync disabled
     * DPMSModeOff      sync disabled
     */
    switch(pNv->orType) {
    case DAC:
        while(pNv->reg[(0x0061A004+off)/4] & 0x80000000);

        tmp = pNv->reg[(0x0061A004+off)/4];
        tmp &= ~0x7f;
        tmp |= 0x80000000;

        if(mode == DPMSModeStandby || mode == DPMSModeOff)
            tmp |= 1;
        if(mode == DPMSModeSuspend || mode == DPMSModeOff)
            tmp |= 4;
        if(mode != DPMSModeOn)
            tmp |= 0x10;
        if(mode == DPMSModeOff)
            tmp |= 0x40;

        pNv->reg[(0x0061A004+off)/4] = tmp;

        break;

    case SOR:
        while(pNv->reg[(0x0061C004+off)/4] & 0x80000000);

        tmp = pNv->reg[(0x0061C004+off)/4];
        tmp |= 0x80000000;

        if(mode == DPMSModeOn)
            tmp |= 1;
        else
            tmp &= ~1;

        pNv->reg[(0x0061C004+off)/4] = tmp;

        break;
    }
}

/******************************** Cursor stuff ********************************/
void G80DispShowCursor(G80Ptr pNv, Bool update)
{
    const int headOff = 0x400 * pNv->head;

    C(0x00000880 + headOff, 0x85000000);
    if(update) C(0x00000080, 0);
}

void G80DispHideCursor(G80Ptr pNv, Bool update)
{
    const int headOff = 0x400 * pNv->head;

    C(0x00000880 + headOff, 0x5000000);
    if(update) C(0x00000080, 0);
}
