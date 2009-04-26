/*
 * Copyright 2007-2008  Luc Verhaegen <libv@exsuse.de>
 * Copyright 2007-2008  Matthias Hopf <mhopf@novell.com>
 * Copyright 2007-2008  Egbert Eich   <eich@novell.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include "rhd.h"
#include "rhd_lut.h"
#include "rhd_regs.h"

#include <compiler.h>

#define RHD_REGOFFSET_LUTA 0x000
#define RHD_REGOFFSET_LUTB 0x800

/*
 *
 */
static void
LUTxSave(struct rhdLUT *LUT)
{
    CARD16 RegOff;
    int i;
    RHDFUNC(LUT);

    if (LUT->Id == RHD_LUT_A)
	RegOff = RHD_REGOFFSET_LUTA;
    else
	RegOff = RHD_REGOFFSET_LUTB;

    LUT->StoreControl = RHDRegRead(LUT, RegOff + DC_LUTA_CONTROL);

    LUT->StoreBlackBlue = RHDRegRead(LUT, RegOff + DC_LUTA_BLACK_OFFSET_BLUE);
    LUT->StoreBlackGreen = RHDRegRead(LUT, RegOff + DC_LUTA_BLACK_OFFSET_GREEN);
    LUT->StoreBlackRed = RHDRegRead(LUT, RegOff + DC_LUTA_BLACK_OFFSET_RED);

    LUT->StoreWhiteBlue = RHDRegRead(LUT, RegOff + DC_LUTA_WHITE_OFFSET_BLUE);
    LUT->StoreWhiteGreen = RHDRegRead(LUT, RegOff + DC_LUTA_WHITE_OFFSET_GREEN);
    LUT->StoreWhiteRed = RHDRegRead(LUT, RegOff + DC_LUTA_WHITE_OFFSET_RED);

    RHDRegWrite(LUT, DC_LUT_RW_MODE, 0); /* Table */
    if (LUT->Id == RHD_LUT_A)
	RHDRegWrite(LUT, DC_LUT_READ_PIPE_SELECT, 0);
    else
	RHDRegWrite(LUT, DC_LUT_READ_PIPE_SELECT, 1);

    RHDRegWrite(LUT, DC_LUT_RW_INDEX, 0);
    for (i = 0; i < 0x300; i++)
	LUT->StoreEntry[i] = RHDRegRead(LUT, DC_LUT_SEQ_COLOR);

    LUT->Stored = TRUE;
}

/*
 *
 */
static void
LUTxRestore(struct rhdLUT *LUT)
{
    CARD16 RegOff;
    int i;
    RHDFUNC(LUT);

    if (!LUT->Stored) {
	xf86DrvMsg(LUT->scrnIndex, X_ERROR, "%s: %s: nothing stored!\n",
		   __func__, LUT->Name);
	return;
    }

    if (LUT->Id == RHD_LUT_A)
	RegOff = RHD_REGOFFSET_LUTA;
    else
	RegOff = RHD_REGOFFSET_LUTB;

    RHDRegWrite(LUT, RegOff + DC_LUTA_BLACK_OFFSET_BLUE, LUT->StoreBlackBlue);
    RHDRegWrite(LUT, RegOff + DC_LUTA_BLACK_OFFSET_GREEN, LUT->StoreBlackGreen);
    RHDRegWrite(LUT, RegOff + DC_LUTA_BLACK_OFFSET_RED, LUT->StoreBlackRed);

    RHDRegWrite(LUT, RegOff + DC_LUTA_WHITE_OFFSET_BLUE, LUT->StoreWhiteBlue);
    RHDRegWrite(LUT, RegOff + DC_LUTA_WHITE_OFFSET_GREEN, LUT->StoreWhiteGreen);
    RHDRegWrite(LUT, RegOff + DC_LUTA_WHITE_OFFSET_RED, LUT->StoreWhiteRed);

    if (LUT->Id == RHD_LUT_A)
	RHDRegWrite(LUT, DC_LUT_RW_SELECT, 0);
    else
	RHDRegWrite(LUT, DC_LUT_RW_SELECT, 1);

    RHDRegWrite(LUT, DC_LUT_RW_MODE, 0); /* Table */
    RHDRegWrite(LUT, DC_LUT_WRITE_EN_MASK, 0x0000003F);
    RHDRegWrite(LUT, DC_LUT_RW_INDEX, 0);
    for (i = 0; i < 0x300; i++)
	RHDRegWrite(LUT, DC_LUT_SEQ_COLOR, LUT->StoreEntry[i]);

    RHDRegWrite(LUT, RegOff + DC_LUTA_CONTROL, LUT->StoreControl);
}

/*
 *
 */
static void
LUTxSet(struct rhdLUT *LUT, int numColors, int *indices, LOCO *colors)
{
    ScrnInfoPtr pScrn = xf86Screens[LUT->scrnIndex];
    CARD16 RegOff;
    int i, index, hw_index;

    LUT->Initialised = TRUE; /* thank you RandR */

    if (LUT->Id == RHD_LUT_A)
	RegOff = RHD_REGOFFSET_LUTA;
    else
	RegOff = RHD_REGOFFSET_LUTB;

    RHDRegWrite(LUT, RegOff + DC_LUTA_CONTROL, 0);

    RHDRegWrite(LUT, RegOff + DC_LUTA_BLACK_OFFSET_BLUE, 0);
    RHDRegWrite(LUT, RegOff + DC_LUTA_BLACK_OFFSET_GREEN, 0);
    RHDRegWrite(LUT, RegOff + DC_LUTA_BLACK_OFFSET_RED, 0);

    RHDRegWrite(LUT, RegOff + DC_LUTA_WHITE_OFFSET_BLUE, 0x0000FFFF);
    RHDRegWrite(LUT, RegOff + DC_LUTA_WHITE_OFFSET_GREEN, 0x0000FFFF);
    RHDRegWrite(LUT, RegOff + DC_LUTA_WHITE_OFFSET_RED, 0x0000FFFF);

    if (LUT->Id == RHD_LUT_A)
	RHDRegWrite(LUT, DC_LUT_RW_SELECT, 0);
    else
	RHDRegWrite(LUT, DC_LUT_RW_SELECT, 1);

    RHDRegWrite(LUT, DC_LUT_RW_MODE, 0); /* table */
    RHDRegWrite(LUT, DC_LUT_WRITE_EN_MASK, 0x0000003F);

    /* DC_LUT_RW_INDEX is incremented automatically when DC_LUT_30_COLOR
     * is accessed; hw_index is used to track the value of DC_LUT_RW_INDEX
     * so that we can properly handle the very unlikely case that the input
     * table indexes are not monotonically increasing
     */
    switch (pScrn->depth) {
    case 8:
    case 24:
    case 32:
        RHDRegWrite(LUT, DC_LUT_RW_INDEX, 0);
        hw_index = 0;
	for (i = 0; i < numColors; i++) {
            index = indices[i];
            if (hw_index != index) {
                RHDRegWrite(LUT, DC_LUT_RW_INDEX, index);
                hw_index = index;
            }
	    RHDRegWrite(LUT, DC_LUT_30_COLOR, (colors[index].red << 20) |
			(colors[index].green << 10) | (colors[index].blue));
            hw_index++;
	}
	break;
    case 16:
        RHDRegWrite(LUT, DC_LUT_RW_INDEX, 0);
        hw_index = 0;
	for (i = 0; i < numColors; i++) {
	    int j;

	    index = indices[i];
            if (hw_index != 4 * index) {
                RHDRegWrite(LUT, DC_LUT_RW_INDEX, 4 * index);
                hw_index = 4 * index;
            }

	    for (j = 0; j < 4; j++) {
		RHDRegWrite(LUT, DC_LUT_30_COLOR, (colors[index/2].red << 20) |
			    (colors[index].green << 10) | (colors[index/2].blue));
                hw_index++;
            }
	}
	break;
    case 15:
        RHDRegWrite(LUT, DC_LUT_RW_INDEX, 0);
        hw_index = 0;
	for (i = 0; i < numColors; i++) {
	    int j;

	    index = indices[i];
            if (hw_index != 8 * index) {
                RHDRegWrite(LUT, DC_LUT_RW_INDEX, 8 * index);
                hw_index = 8 * index;
            }

	    for (j = 0; j < 8; j++) {
		RHDRegWrite(LUT, DC_LUT_30_COLOR, (colors[index].red << 20) |
			    (colors[index].green << 10) | (colors[index].blue));
                hw_index++;
            }
	}
	break;
    }
}

/*
 *
 */
void
RHDLUTsInit(RHDPtr rhdPtr)
{
    struct rhdLUT *LUT;

    RHDFUNC(rhdPtr);

    LUT = xnfcalloc(sizeof(struct rhdLUT), 1);

    LUT->scrnIndex = rhdPtr->scrnIndex;
    LUT->Name = "LUT A";
    LUT->Id = RHD_LUT_A;

    LUT->Save = LUTxSave;
    LUT->Restore = LUTxRestore;
    LUT->Set = LUTxSet;

    rhdPtr->LUT[0] = LUT;

    LUT = xnfcalloc(sizeof(struct rhdLUT), 1);

    LUT->scrnIndex = rhdPtr->scrnIndex;
    LUT->Name = "LUT B";
    LUT->Id = RHD_LUT_B;

    LUT->Save = LUTxSave;
    LUT->Restore = LUTxRestore;
    LUT->Set = LUTxSet;

    rhdPtr->LUT[1] = LUT;
}

/*
 *
 */
struct rhdLUTStore {
    CARD32 Select;
    CARD32 Mode;
    CARD32 Index;
    CARD32 Color;
    CARD32 ReadPipe;
    CARD32 WriteMask;
};

/*
 *
 */
void
RHDLUTsSave(RHDPtr rhdPtr)
{
    struct rhdLUTStore *Store = rhdPtr->LUTStore;

    RHDFUNC(rhdPtr);

    if (!Store) {
	Store = xnfcalloc(sizeof(struct rhdLUTStore), 1);
	rhdPtr->LUTStore = Store;
    }

    Store->Select = RHDRegRead(rhdPtr, DC_LUT_RW_SELECT);
    Store->Mode = RHDRegRead(rhdPtr, DC_LUT_RW_MODE);
    Store->Index = RHDRegRead(rhdPtr, DC_LUT_RW_INDEX);
    Store->Color = RHDRegRead(rhdPtr, DC_LUT_30_COLOR);
    Store->ReadPipe = RHDRegRead(rhdPtr, DC_LUT_READ_PIPE_SELECT);
    Store->WriteMask = RHDRegRead(rhdPtr, DC_LUT_WRITE_EN_MASK);

    rhdPtr->LUT[0]->Save(rhdPtr->LUT[0]);
    rhdPtr->LUT[1]->Save(rhdPtr->LUT[1]);
}

/*
 *
 */
void
RHDLUTsRestore(RHDPtr rhdPtr)
{
    struct rhdLUTStore *Store = rhdPtr->LUTStore;

    RHDFUNC(rhdPtr);

    rhdPtr->LUT[0]->Restore(rhdPtr->LUT[0]);
    rhdPtr->LUT[1]->Restore(rhdPtr->LUT[1]);

    if (!Store) {
	xf86DrvMsg(rhdPtr->scrnIndex, X_ERROR, "%s: nothing stored!\n", __func__);
	return;
    }

    RHDRegWrite(rhdPtr, DC_LUT_RW_SELECT, Store->Select);
    RHDRegWrite(rhdPtr, DC_LUT_RW_MODE, Store->Mode);
    RHDRegWrite(rhdPtr, DC_LUT_RW_INDEX, Store->Index);
    RHDRegWrite(rhdPtr, DC_LUT_30_COLOR, Store->Color);
    RHDRegWrite(rhdPtr, DC_LUT_READ_PIPE_SELECT, Store->ReadPipe);
    RHDRegWrite(rhdPtr, DC_LUT_WRITE_EN_MASK, Store->WriteMask);
}

/*
 *
 */
void
RHDLUTsDestroy(RHDPtr rhdPtr)
{
    RHDFUNC(rhdPtr);

    xfree(rhdPtr->LUT[0]);
    xfree(rhdPtr->LUT[1]);
    xfree(rhdPtr->LUTStore);
}

/*
 * Workaround for missing RandR functionality. Initialise this
 * LUT with the content of the other LUT.
 */
void
RHDLUTCopyForRR(struct rhdLUT *LUT)
{
    int indices[0x100];
    LOCO colors[0x100];
    CARD32 entry;
    int i;

    RHDDebug(LUT->scrnIndex, "%s: %s\n", __func__, LUT->Name);

    RHDRegWrite(LUT, DC_LUT_RW_MODE, 0); /* Table */

    if (LUT->Id == RHD_LUT_A)
	RHDRegWrite(LUT, DC_LUT_READ_PIPE_SELECT, 1);
    else
	RHDRegWrite(LUT, DC_LUT_READ_PIPE_SELECT, 0);

    switch (xf86Screens[LUT->scrnIndex]->depth) {
    case 8:
    case 24:
    case 32:
	RHDRegWrite(LUT, DC_LUT_RW_INDEX, 0);

	for (i = 0; i < 0x100; i++) {
	    indices[i] = i;

	    entry = RHDRegRead(LUT, DC_LUT_30_COLOR);

	    colors[i].red = (entry >> 20) & 0x3FF;
	    colors[i].green = (entry >> 10) & 0x3FF;
	    colors[i].blue = (entry) & 0x3FF;
	}
	LUT->Set(LUT, 0x100, indices, colors);
	break;
    case 16:
	for (i = 0; i < 0x40; i++) {
	    indices[i] = i;

	    RHDRegWrite(LUT, DC_LUT_RW_INDEX, 4 * i);

	    entry = RHDRegRead(LUT, DC_LUT_30_COLOR);

	    colors[i / 2].red = (entry >> 20) & 0x3FF;
	    colors[i].green = (entry >> 10) & 0x3FF;
	    colors[i / 2].blue = (entry) & 0x3FF;
	}
	LUT->Set(LUT, 0x40, indices, colors);
	break;
    case 15:
	for (i = 0; i < 0x20; i++) {
	    indices[i] = i;

	    RHDRegWrite(LUT, DC_LUT_RW_INDEX, 8 * i);

	    entry = RHDRegRead(LUT, DC_LUT_30_COLOR);

	    colors[i].red = (entry >> 20) & 0x3FF;
	    colors[i].green = (entry >> 10) & 0x3FF;
	    colors[i].blue = (entry) & 0x3FF;
	}
	LUT->Set(LUT, 0x20, indices, colors);
	break;
    }
}
