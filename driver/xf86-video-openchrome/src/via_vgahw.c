/*
 * Copyright 2004-2005 The Unichrome Project  [unichrome.sf.net]
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * Wrappers around xf86 vgaHW functions.
 * And some generic IO calls lacking in the current vgaHW implementation.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "compiler.h"
#include "xf86.h"
#include "via_driver.h" /* for HAVE_DEBUG */

void
ViaCrtcMask(vgaHWPtr hwp, CARD8 index, CARD8 value, CARD8 mask)
{
    CARD8 tmp;

    tmp = hwp->readCrtc(hwp, index);
    tmp &= ~mask;
    tmp |= (value & mask);

    hwp->writeCrtc(hwp, index, tmp);
}

void
ViaSeqMask(vgaHWPtr hwp, CARD8 index, CARD8 value, CARD8 mask)
{
    CARD8 tmp;

    tmp = hwp->readSeq(hwp, index);
    tmp &= ~mask;
    tmp |= (value & mask);

    hwp->writeSeq(hwp, index, tmp);
}

void
ViaGrMask(vgaHWPtr hwp, CARD8 index, CARD8 value, CARD8 mask)
{
    CARD8 tmp;

    tmp = hwp->readGr(hwp, index);
    tmp &= ~mask;
    tmp |= (value & mask);

    hwp->writeGr(hwp, index, tmp);
}

#ifdef HAVE_DEBUG
void
ViaVgahwPrint(vgaHWPtr hwp)
{
    int i;

    if (!hwp)
	return;

    xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO, "VGA Sequence registers:\n");
    for (i = 0x00; i < 0x80; i++)
        xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO,
                   "SR%02X: 0x%02X\n", i, hwp->readSeq(hwp, i));

    xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO, "VGA CRTM/C registers:\n");
    for (i = 0x00; i < 0x19; i++)
        xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO,
                   "CR%02X: 0x%02X\n", i, hwp->readCrtc(hwp, i));
    for (i = 0x33; i < 0xA3; i++)
        xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO,
                   "CR%02X: 0x%02X\n", i, hwp->readCrtc(hwp, i));

    xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO, "VGA Graphics registers:\n");
    for (i = 0x00; i < 0x08; i++)
        xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO,
                   "GR%02X: 0x%02X\n", i, hwp->readGr(hwp, i));

    xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO, "VGA Attribute registers:\n");
    for (i = 0x00; i < 0x14; i++)
        xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO,
                   "AR%02X: 0x%02X\n", i, hwp->readAttr(hwp, i));

    xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO, "VGA Miscellaneous register:\n");
    xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO,
               "Misc: 0x%02X\n", hwp->readMiscOut(hwp));

    xf86DrvMsg(hwp->pScrn->scrnIndex, X_INFO, "End of VGA registers.\n");
}
#endif /* HAVE_DEBUG */
