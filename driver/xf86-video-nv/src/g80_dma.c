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

#include "g80_type.h"
#include "g80_dma.h"

void G80DmaKickoff(G80Ptr pNv)
{
    if(pNv->dmaCurrent != pNv->dmaPut) {
        pNv->dmaPut = pNv->dmaCurrent;
        pNv->reg[0x00c02040/4] = pNv->dmaPut << 2;
    }
}

void G80DmaWait(G80Ptr pNv, int size)
{
    CARD32 dmaGet;

    size++;

    while(pNv->dmaFree < size) {
        dmaGet = pNv->reg[0x00c02044/4] >> 2;

        if(pNv->dmaPut >= dmaGet) {
            pNv->dmaFree = pNv->dmaMax - pNv->dmaCurrent;
            if(pNv->dmaFree < size) {
                G80DmaNext(pNv, 0x20000000);
                if(dmaGet <= SKIPS) {
                    if(pNv->dmaPut <= SKIPS) /* corner case - will be idle */
                        pNv->reg[0x00c02040/4] = (SKIPS + 1) << 2;
                    do { dmaGet = pNv->reg[0x00c02044/4] >> 2; }
                    while(dmaGet <= SKIPS);
                }
                pNv->reg[0x00c02040/4] = SKIPS << 2;
                pNv->dmaCurrent = pNv->dmaPut = SKIPS;
                pNv->dmaFree = dmaGet - (SKIPS + 1);
            }
        } else
            pNv->dmaFree = dmaGet - pNv->dmaCurrent - 1;
    }
}
