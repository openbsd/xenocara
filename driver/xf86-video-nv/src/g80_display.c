/*
 * Copyright (c) 2007,2010 NVIDIA Corporation
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
#include "g80_cursor.h"
#include "g80_display.h"
#include "g80_output.h"

typedef struct G80CrtcPrivRec {
    Head head;
    int pclk; /* Target pixel clock in kHz */
    Bool cursorVisible;
    Bool skipModeFixup;
    Bool dither;
    /* Look-up table values to be set when the CRTC is enabled */
    uint16_t lut_r[256], lut_g[256], lut_b[256];
} G80CrtcPrivRec, *G80CrtcPrivPtr;

static void G80CrtcShowHideCursor(xf86CrtcPtr crtc, Bool show, Bool update);

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
G80CalcPLL2(float pclk, int *pN, int *pM, int *pPL)
{
    const float refclk = 27000.0f;
    const int minN = 8, maxN = 255;
    const int minM = 1, maxM = 255;
    const int minPL = 1, maxPL = 63;
    const int minU = 25000, maxU = 50000;
    const int minVco = 500000;
    int maxVco = 1000000;
    int lowPL, highPL, pl;
    float vco, bestError = FLT_MAX;

    vco = pclk + pclk / 50;

    if(maxVco < vco) maxVco = vco;

    highPL = (maxVco + vco - 1) / pclk;
    if(highPL > maxPL) highPL = maxPL;
    if(highPL < minPL) highPL = minPL;

    lowPL = minVco / vco;
    if(lowPL > maxPL) lowPL = maxPL;
    if(lowPL < minPL) lowPL = minPL;

    for(pl = highPL; pl >= lowPL; pl--) {
        int m;

        for(m = minM; m <= maxM; m++) {
            int n;
            float freq, error;

            if(refclk / m < minU) break;
            if(refclk / m > maxU) continue;

            n = rint(pclk * pl * m / refclk);
            if(n > maxN) break;
            if(n < minN) continue;

            freq = refclk * (n / (float)m) / pl;
            error = fabsf(pclk - freq);
            if(error < bestError) {
                *pN = n;
                *pM = m;
                *pPL = pl;
                bestError = error;
            }
        }
    }
}

static void
G80CrtcSetPClk(xf86CrtcPtr crtc)
{
    G80Ptr pNv = G80PTR(crtc->scrn);
    G80CrtcPrivPtr pPriv = crtc->driver_private;
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    const int headOff = 0x800 * pPriv->head;
    int i;

    if(pPriv->pclk == 0)
        return;

    if(pNv->architecture <= 0xa0 ||
       pNv->architecture == 0xaa ||
       pNv->architecture == 0xac) {
        int lo_n, lo_m, hi_n, hi_m, p;
        CARD32 lo = pNv->reg[(0x00614104+headOff)/4];
        CARD32 hi = pNv->reg[(0x00614108+headOff)/4];

        pNv->reg[(0x00614100+headOff)/4] = 0x10000610;
        lo &= 0xff00ff00;
        hi &= 0x8000ff00;

        G80CalcPLL(pPriv->pclk, &lo_n, &lo_m, &hi_n, &hi_m, &p);

        lo |= (lo_m << 16) | lo_n;
        hi |= (p << 28) | (hi_m << 16) | hi_n;
        pNv->reg[(0x00614104+headOff)/4] = lo;
        pNv->reg[(0x00614108+headOff)/4] = hi;
    } else {
        int n, m, pl;
        CARD32 r = pNv->reg[(0x00614104+headOff)/4];

        pNv->reg[(0x00614100+headOff)/4] = 0x50000610;
        r &= 0xffc00000;

        G80CalcPLL2(pPriv->pclk, &n, &m, &pl);
        r |= pl << 16 | m << 8 | n;

        pNv->reg[(0x00614104+headOff)/4] = r;
    }
    pNv->reg[(0x00614200+headOff)/4] = 0;

    for(i = 0; i < xf86_config->num_output; i++) {
        xf86OutputPtr output = xf86_config->output[i];

        if(output->crtc != crtc)
            continue;
        G80OutputSetPClk(output, pPriv->pclk);
    }
}

void
G80DispCommand(ScrnInfoPtr pScrn, CARD32 addr, CARD32 data)
{
    G80Ptr pNv = G80PTR(pScrn);

    pNv->reg[0x00610304/4] = data;
    pNv->reg[0x00610300/4] = addr | 0x80010001;

    while(pNv->reg[0x00610300/4] & 0x80000000) {
        const int super = ffs((pNv->reg[0x00610024/4] >> 4) & 7);

        if(super) {
            if(super == 2) {
                xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
                int i;

                for(i = 0; i < xf86_config->num_crtc; i++)
                {
                    xf86CrtcPtr crtc = xf86_config->crtc[i];
                    const int headOff = 0x800 * G80CrtcGetHead(crtc);

                    if((pNv->reg[(0x00614200+headOff)/4] & 0xc0) == 0x80)
                        G80CrtcSetPClk(crtc);
                }
            }

            pNv->reg[0x00610024/4] = 8 << super;
            pNv->reg[0x00610030/4] = 0x80000000;
        }
    }
}

Head
G80CrtcGetHead(xf86CrtcPtr crtc)
{
    G80CrtcPrivPtr pPriv = crtc->driver_private;
    return pPriv->head;
}

Bool
G80DispPreInit(ScrnInfoPtr pScrn)
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
    pNv->reg[0x006101E8/4] = pNv->reg[0x0061D000/4];
    pNv->reg[0x006101EC/4] = pNv->reg[0x0061D800/4];
    pNv->reg[0x0061A004/4] = 0x80550000;
    pNv->reg[0x0061A010/4] = 0x00000001;
    pNv->reg[0x0061A804/4] = 0x80550000;
    pNv->reg[0x0061A810/4] = 0x00000001;
    pNv->reg[0x0061B004/4] = 0x80550000;
    pNv->reg[0x0061B010/4] = 0x00000001;

    return TRUE;
}

Bool
G80DispInit(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);
    CARD32 val;

    if(pNv->reg[0x00610024/4] & 0x100) {
        pNv->reg[0x00610024/4] = 0x100;
        pNv->reg[0x006194E8/4] &= ~1;
        while(pNv->reg[0x006194E8/4] & 2);
    }

    pNv->reg[0x00610200/4] = 0x2b00;
    do {
        val = pNv->reg[0x00610200/4];

        if ((val & 0x9f0000) == 0x20000)
            pNv->reg[0x00610200/4] = val | 0x800000;

        if ((val & 0x3f0000) == 0x30000)
            pNv->reg[0x00610200/4] = val | 0x200000;
    } while ((val & 0x1e0000) != 0);
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
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    for(i = 0; i < xf86_config->num_crtc; i++) {
        xf86CrtcPtr crtc = xf86_config->crtc[i];

        G80CrtcBlankScreen(crtc, TRUE);
    }

    C(0x00000080, 0);

    for(i = 0; i < xf86_config->num_crtc; i++) {
        xf86CrtcPtr crtc = xf86_config->crtc[i];

        if(crtc->enabled) {
            const CARD32 mask = 4 << G80CrtcGetHead(crtc);

            pNv->reg[0x00610024/4] = mask;
            while(!(pNv->reg[0x00610024/4] & mask));
        }
    }

    pNv->reg[0x00610200/4] = 0;
    pNv->reg[0x00610300/4] = 0;
    while((pNv->reg[0x00610200/4] & 0x1e0000) != 0);
    while((pNv->reg[0x61C030/4] & 0x10000000));
    while((pNv->reg[0x61C830/4] & 0x10000000));
}

void
G80CrtcDoModeFixup(DisplayModePtr dst, const DisplayModePtr src)
{
    /* Magic mode timing fudge factor */
    const int fudge = ((src->Flags & V_INTERLACE) && (src->Flags & V_DBLSCAN)) ? 2 : 1;
    const int interlaceDiv = (src->Flags & V_INTERLACE) ? 2 : 1;

    /* Stash the src timings in the Crtc fields in dst */
    dst->CrtcHBlankStart = src->CrtcVTotal << 16 | src->CrtcHTotal;
    dst->CrtcHSyncEnd = ((src->CrtcVSyncEnd - src->CrtcVSyncStart) / interlaceDiv - 1) << 16 |
        (src->CrtcHSyncEnd - src->CrtcHSyncStart - 1);
    dst->CrtcHBlankEnd = ((src->CrtcVBlankEnd - src->CrtcVSyncStart) / interlaceDiv - fudge) << 16 |
        (src->CrtcHBlankEnd - src->CrtcHSyncStart - 1);
    dst->CrtcHTotal = ((src->CrtcVTotal - src->CrtcVSyncStart + src->CrtcVBlankStart) / interlaceDiv - fudge) << 16 |
        (src->CrtcHTotal - src->CrtcHSyncStart + src->CrtcHBlankStart - 1);
    dst->CrtcHSkew = ((src->CrtcVTotal + src->CrtcVBlankEnd - src->CrtcVSyncStart) / 2 - 2) << 16 |
        ((2*src->CrtcVTotal - src->CrtcVSyncStart + src->CrtcVBlankStart) / 2 - 2);
}

static Bool
G80CrtcModeFixup(xf86CrtcPtr crtc,
                 DisplayModePtr mode, DisplayModePtr adjusted_mode)
{
    G80CrtcPrivPtr pPriv = crtc->driver_private;

    if(pPriv->skipModeFixup)
        return TRUE;

    G80CrtcDoModeFixup(adjusted_mode, mode);
    return TRUE;
}

static void
G80CrtcModeSet(xf86CrtcPtr crtc, DisplayModePtr mode,
               DisplayModePtr adjusted_mode, int x, int y)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    G80CrtcPrivPtr pPriv = crtc->driver_private;
    const int HDisplay = adjusted_mode->HDisplay, VDisplay = adjusted_mode->VDisplay;
    const int headOff = 0x400 * G80CrtcGetHead(crtc);

    pPriv->pclk = adjusted_mode->Clock;

    C(0x00000804 + headOff, adjusted_mode->Clock | 0x800000);
    C(0x00000808 + headOff, (adjusted_mode->Flags & V_INTERLACE) ? 2 : 0);
    C(0x00000810 + headOff, 0);
    C(0x0000082C + headOff, 0);
    C(0x00000814 + headOff, adjusted_mode->CrtcHBlankStart);
    C(0x00000818 + headOff, adjusted_mode->CrtcHSyncEnd);
    C(0x0000081C + headOff, adjusted_mode->CrtcHBlankEnd);
    C(0x00000820 + headOff, adjusted_mode->CrtcHTotal);
    if(adjusted_mode->Flags & V_INTERLACE)
        C(0x00000824 + headOff, adjusted_mode->CrtcHSkew);
    C(0x00000868 + headOff, pScrn->virtualY << 16 | pScrn->virtualX);
    C(0x0000086C + headOff, pScrn->displayWidth * (pScrn->bitsPerPixel / 8) | 0x100000);
    switch(pScrn->depth) {
        case  8: C(0x00000870 + headOff, 0x1E00); break;
        case 15: C(0x00000870 + headOff, 0xE900); break;
        case 16: C(0x00000870 + headOff, 0xE800); break;
        case 24: C(0x00000870 + headOff, 0xCF00); break;
    }
    G80CrtcSetDither(crtc, pPriv->dither, FALSE);
    C(0x000008A8 + headOff, 0x40000);
    C(0x000008C0 + headOff, y << 16 | x);
    C(0x000008C8 + headOff, VDisplay << 16 | HDisplay);
    C(0x000008D4 + headOff, 0);

    G80CrtcBlankScreen(crtc, FALSE);
}

void
G80CrtcBlankScreen(xf86CrtcPtr crtc, Bool blank)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    G80Ptr pNv = G80PTR(pScrn);
    G80CrtcPrivPtr pPriv = crtc->driver_private;
    const int headOff = 0x400 * pPriv->head;

    if(blank) {
        G80CrtcShowHideCursor(crtc, FALSE, FALSE);

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
        pNv->reg[0x00610384/4] = pNv->videoRam * 1024 - 1;
        pNv->reg[0x00610388/4] = 0x150000;
        pNv->reg[0x0061038C/4] = 0;
        C(0x00000884 + headOff, (pNv->videoRam << 2) - 0x40);
        if(pNv->architecture != 0x50)
            C(0x0000089C + headOff, 1);
        if(pPriv->cursorVisible)
            G80CrtcShowHideCursor(crtc, TRUE, FALSE);
        C(0x00000840 + headOff, pScrn->depth == 8 ? 0x80000000 : 0xc0000000);
        C(0x00000844 + headOff, (pNv->videoRam * 1024 - 0x5000 - 0x1000 * pPriv->head) >> 8);
        if(pNv->architecture != 0x50)
            C(0x0000085C + headOff, 1);
        C(0x00000874 + headOff, 1);
    }
}

static void
G80CrtcDPMSSet(xf86CrtcPtr crtc, int mode)
{
}

/******************************** Cursor stuff ********************************/
static void G80CrtcShowHideCursor(xf86CrtcPtr crtc, Bool show, Bool update)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    G80CrtcPrivPtr pPriv = crtc->driver_private;
    const int headOff = 0x400 * G80CrtcGetHead(crtc);

    C(0x00000880 + headOff, show ? 0x85000000 : 0x5000000);
    if(update) {
        pPriv->cursorVisible = show;
        C(0x00000080, 0);
    }
}

static void G80CrtcShowCursor(xf86CrtcPtr crtc)
{
    G80CrtcShowHideCursor(crtc, TRUE, TRUE);
}

static void G80CrtcHideCursor(xf86CrtcPtr crtc)
{
    G80CrtcShowHideCursor(crtc, FALSE, TRUE);
}

/******************************** CRTC stuff ********************************/

static Bool
G80CrtcLock(xf86CrtcPtr crtc)
{
    return FALSE;
}

static void
G80CrtcPrepare(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    G80CrtcPrivPtr pPriv = crtc->driver_private;
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i;

    for(i = 0; i < xf86_config->num_output; i++) {
        xf86OutputPtr output = xf86_config->output[i];

        if(!output->crtc)
            output->funcs->mode_set(output, NULL, NULL);
    }

    pPriv->skipModeFixup = FALSE;
}

void
G80CrtcSkipModeFixup(xf86CrtcPtr crtc)
{
    G80CrtcPrivPtr pPriv = crtc->driver_private;
    pPriv->skipModeFixup = TRUE;
}

void
G80CrtcSetDither(xf86CrtcPtr crtc, Bool dither, Bool update)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    G80CrtcPrivPtr pPriv = crtc->driver_private;
    const int headOff = 0x400 * G80CrtcGetHead(crtc);

    pPriv->dither = dither;

    C(0x000008A0 + headOff, dither ? 0x11 : 0);
    if(update) C(0x00000080, 0);
}

static void ComputeAspectScale(DisplayModePtr mode, int *outX, int *outY)
{
    float scaleX, scaleY, scale;

    scaleX = mode->CrtcHDisplay / (float)mode->HDisplay;
    scaleY = mode->CrtcVDisplay / (float)mode->VDisplay;

    if(scaleX > scaleY)
        scale = scaleY;
    else
        scale = scaleX;

    *outX = mode->HDisplay * scale;
    *outY = mode->VDisplay * scale;
}

void G80CrtcSetScale(xf86CrtcPtr crtc, DisplayModePtr mode,
                     enum G80ScaleMode scale)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    G80CrtcPrivPtr pPriv = crtc->driver_private;
    const int headOff = 0x400 * pPriv->head;
    int outX, outY;

    switch(scale) {
        default:
        case G80_SCALE_ASPECT:
            ComputeAspectScale(mode, &outX, &outY);
            break;

        case G80_SCALE_OFF:
        case G80_SCALE_FILL:
            outX = mode->CrtcHDisplay;
            outY = mode->CrtcVDisplay;
            break;

        case G80_SCALE_CENTER:
            outX = mode->HDisplay;
            outY = mode->VDisplay;
            break;
    }

    if((mode->Flags & V_DBLSCAN) || (mode->Flags & V_INTERLACE) ||
       mode->HDisplay != outX || mode->VDisplay != outY) {
        C(0x000008A4 + headOff, 9);
    } else {
        C(0x000008A4 + headOff, 0);
    }
    C(0x000008D8 + headOff, outY << 16 | outX);
    C(0x000008DC + headOff, outY << 16 | outX);
}

static void
G80CrtcCommit(xf86CrtcPtr crtc)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(crtc->scrn);
    int i, crtc_mask = 0;

    /* If any heads are unused, blank them */
    for(i = 0; i < xf86_config->num_output; i++) {
        xf86OutputPtr output = xf86_config->output[i];

        if(output->crtc)
            /* XXXagp: This assumes that xf86_config->crtc[i] is HEADi */
            crtc_mask |= 1 << G80CrtcGetHead(output->crtc);
    }

    for(i = 0; i < xf86_config->num_crtc; i++)
        if(!((1 << i) & crtc_mask))
            G80CrtcBlankScreen(xf86_config->crtc[i], TRUE);

    C(0x00000080, 0);
}

static void
G80CrtcGammaSet(xf86CrtcPtr crtc, CARD16 *red, CARD16 *green, CARD16 *blue,
                int size)
{
    ScrnInfoPtr pScrn = crtc->scrn;
    G80Ptr pNv = G80PTR(pScrn);
    G80CrtcPrivPtr pPriv = crtc->driver_private;
    int i;
    volatile struct {
        uint16_t red, green, blue, unused;
    } *lut = (void*)&pNv->mem[pNv->videoRam * 1024 - 0x5000 - 0x1000 * pPriv->head];

    assert(size == 256);

    for(i = 0; i < size; i++) {
        pPriv->lut_r[i] = lut[i].red   = red[i] >> 2;
        pPriv->lut_g[i] = lut[i].green = green[i] >> 2;
        pPriv->lut_b[i] = lut[i].blue  = blue[i] >> 2;
    }

    lut[256] = lut[255];
}

void
G80LoadPalette(ScrnInfoPtr pScrn, int numColors, int *indices, LOCO *colors,
               VisualPtr pVisual)
{
    xf86CrtcConfigPtr xf86_config = XF86_CRTC_CONFIG_PTR(pScrn);
    int i, j, index;
    int p;
    uint16_t lut_r[256], lut_g[256], lut_b[256];

    for(p = 0; p < xf86_config->num_crtc; p++) {
        xf86CrtcPtr crtc = xf86_config->crtc[p];
        G80CrtcPrivPtr pPriv = crtc->driver_private;

        /* Initialize to the old lookup table values. */
        for(i = 0; i < 256; i++) {
            lut_r[i] = pPriv->lut_r[i] << 2;
            lut_g[i] = pPriv->lut_g[i] << 2;
            lut_b[i] = pPriv->lut_b[i] << 2;
        }

        switch(pScrn->depth) {
            case 15:
                for(i = 0; i < numColors; i++) {
                    index = indices[i];
                    for(j = 0; j < 8; j++) {
                        lut_r[index * 8 + j] =
                            colors[index].red << 8;
                        lut_g[index * 8 + j] =
                            colors[index].green << 8;
                        lut_b[index * 8 + j] =
                            colors[index].blue << 8;
                    }
                }
                break;
            case 16:
                for(i = 0; i < numColors; i++) {
                    index = indices[i];

                    if(index <= 31) {
                        for(j = 0; j < 8; j++) {
                            lut_r[index * 8 + j] =
                                colors[index].red << 8;
                            lut_b[index * 8 + j] =
                                colors[index].blue << 8;
                        }
                    }

                    for(j = 0; j < 4; j++) {
                        lut_g[index * 4 + j] =
                            colors[index].green << 8;
                    }
                }
                break;
            default:
                for(i = 0; i < numColors; i++) {
                    index = indices[i];
                    lut_r[index] = colors[index].red << 8;
                    lut_g[index] = colors[index].green << 8;
                    lut_b[index] = colors[index].blue << 8;
                }
                break;
        }

        /* Make the change through RandR */
        RRCrtcGammaSet(crtc->randr_crtc, lut_r, lut_g, lut_b);
    }
}

static const xf86CrtcFuncsRec g80_crtc_funcs = {
    .dpms = G80CrtcDPMSSet,
    .save = NULL,
    .restore = NULL,
    .lock = G80CrtcLock,
    .unlock = NULL,
    .mode_fixup = G80CrtcModeFixup,
    .prepare = G80CrtcPrepare,
    .mode_set = G80CrtcModeSet,
    .gamma_set = G80CrtcGammaSet,
    .commit = G80CrtcCommit,
    .shadow_create = NULL,
    .shadow_destroy = NULL,
    .set_cursor_position = G80SetCursorPosition,
    .show_cursor = G80CrtcShowCursor,
    .hide_cursor = G80CrtcHideCursor,
    .load_cursor_argb = G80LoadCursorARGB,
    .destroy = NULL,
};

void
G80DispCreateCrtcs(ScrnInfoPtr pScrn)
{
    G80Ptr pNv = G80PTR(pScrn);
    Head head;
    xf86CrtcPtr crtc;
    G80CrtcPrivPtr g80_crtc;

    /* Create a "crtc" object for each head */
    for(head = HEAD0; head <= HEAD1; head++) {
        crtc = xf86CrtcCreate(pScrn, &g80_crtc_funcs);
        if(!crtc) return;

        g80_crtc = xnfcalloc(sizeof(*g80_crtc), 1);
        g80_crtc->head = head;
        g80_crtc->dither = pNv->Dither;
        crtc->driver_private = g80_crtc;
    }
}
