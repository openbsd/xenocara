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

void R5xx2DIdle(ScrnInfoPtr pScrn);

void R5xx2DSetup(ScrnInfoPtr pScrn); /* to be called after VT switch and such */
void R5xx2DStart(ScrnInfoPtr pScrn);

Bool R5xx2DFBValid(RHDPtr rhdPtr, CARD16 Width, CARD16 Height, int bpp,
		   CARD32 Offset, CARD32 Size, CARD32 Pitch);

/* Helper from r5xx_accel.c */
CARD8 R5xx2DDatatypeGet(ScrnInfoPtr pScrn);

/* XAA specific */
Bool R5xxXAAInit(ScrnInfoPtr pScrn, ScreenPtr pScreen);
void R5xxXAADestroy(ScrnInfoPtr pScrn);

/* EXA specific */
#ifdef USE_EXA

/* r5xx_exa.c */
Bool R5xxEXAInit(ScrnInfoPtr pScrn, ScreenPtr pScreen);
void R5xxEXACloseScreen(ScreenPtr pScreen);
void R5xxEXADestroy(ScrnInfoPtr pScrn);

/* radeon_exa_render.c */
void R5xxExaCompositeFuncs(int scrnIndex, struct _ExaDriver *Exa);

#endif /* USE_EXA */

/*
 * EXA Composite and Textured Video both need to know the state of the
 * 3d engine, so we provide the structure here.
 */
struct R5xx3D {
    Bool XHas3DEngineState;

#define R5XX_ENGINEMODE_UNKNOWN   0
#define R5XX_ENGINEMODE_IDLE_FULL 1
#define R5XX_ENGINEMODE_IDLE_2D   2
#define R5XX_ENGINEMODE_IDLE_3D   3
    int engineMode;

    unsigned short texW[2];
    unsigned short texH[2];

    Bool is_transform[2];
    struct _PictTransform *transform[2];
    Bool has_mask;
    /* Whether we are tiling horizontally and vertically */
    Bool need_src_tile_x;
    Bool need_src_tile_y;
    /* Size of tiles ... set to 65536x65536 if not tiling in that direction */
    Bool src_tile_width;
    Bool src_tile_height;
};

void R5xx3DInit(ScrnInfoPtr pScrn);
void R5xx3DSetup(int scrnIndex);
void R5xx3DDestroy(ScrnInfoPtr pScrn);

void R5xxDstCacheFlush(struct RhdCS *CS);
void R5xxZCacheFlush(struct RhdCS *CS);

void R5xxEngineWaitIdleFull(struct RhdCS *CS);
void R5xxEngineWaitIdle3D(struct RhdCS *CS);
void R5xxEngineWaitIdle2D(struct RhdCS *CS);

#endif /* _RHD_ACCEL_H */
