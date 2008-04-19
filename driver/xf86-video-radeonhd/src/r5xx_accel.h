/*
 * Copyright 2008  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2008  Matthias Hopf <mhopf@novell.com>
 * Copyright 2008  Egbert Eich   <eich@novell.com>
 * Copyright 2008  Advanced Micro Devices, Inc.
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

/*
 * Functionality shared by both R5xx XAA and EXA code.
 */

#ifndef _RHD_ACCEL_H
#define _RHD_ACCEL_H 1

#define R5XX_LOOP_COUNT 2000000

struct R5xxRop {
    CARD32 rop;
    CARD32 pattern;
};

struct R5xx2DInfo {
    int scrnIndex;

    CARD32 dst_pitch_offset; /* Base value for R5XX_DST_PITCH_OFFSET */
    CARD32 control; /* Base value for R5XX_DP_GUI_MASTER_CNTL */
    CARD32 control_saved;
    CARD32 surface_cntl;

    int xdir;
    int ydir;
    int trans_color;
    int scanline_bpp;
    int scanline_h;
    int scanline_words;

    CARD8 *Buffer;

#ifdef USE_EXA
    int exaSyncMarker;
    int exaMarkerSynced;
#if X_BYTE_ORDER == X_BIG_ENDIAN
    unsigned long swapper_surfaces[3];
#endif /* X_BYTE_ORDER */
#endif /* USE_EXA */
};

void R5xxFIFOWait(int scrnIndex, CARD32 required);
void R5xx2DIdle(ScrnInfoPtr pScrn);
void R5xx2DSetup(ScrnInfoPtr pScrn); /* to be called after VT switch and such */
void R5xx2DInit(ScrnInfoPtr pScrn);
void R5xx2DDestroy(ScrnInfoPtr pScrn);

Bool R5xx2DFBValid(RHDPtr rhdPtr, CARD16 Width, CARD16 Height, int bpp,
		   CARD32 Offset, CARD32 Size, CARD32 Pitch);

/* XAA specific -- #ifdef this? */
Bool R5xxXAAInit(ScrnInfoPtr pScrn, ScreenPtr pScreen);
void R5xxXAADestroy(ScrnInfoPtr pScrn);

/* EXA specific */
#ifdef USE_EXA
Bool R5xxEXAInit(ScrnInfoPtr pScrn, ScreenPtr pScreen);
void R5xxEXACloseScreen(ScreenPtr pScreen);
void R5xxEXADestroy(ScrnInfoPtr pScrn);
#endif /* USE_EXA */

#endif /* _RHD_ACCEL_H */
