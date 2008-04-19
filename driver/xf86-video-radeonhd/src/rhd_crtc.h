/*
 * Copyright 2004-2007  Luc Verhaegen <lverhaegen@novell.com>
 * Copyright 2007       Matthias Hopf <mhopf@novell.com>
 * Copyright 2007       Egbert Eich   <eich@novell.com>
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

#ifndef _RHD_CRTC_H
#define _RHD_CRTC_H

struct rhdFMTDither {
    Bool LVDS24Bit;
    Bool LVDSSpatialDither;
    Bool LVDSTemporalDither;
    int LVDSGreyLevel;
};

struct rhdCrtc {
    int scrnIndex;

    char *Name;
#define RHD_CRTC_1 0
#define RHD_CRTC_2 1
    int Id; /* for others to hook onto */

    Bool Active;

    int Offset; /* Current offset */
    int bpp;
    int Pitch;
    int Width;
    int Height;
    int X, Y; /* Current frame */
    int MinX, MinY, MaxX, MaxY; /* Panning Area: Max != 0 if used */

    struct rhdPLL *PLL; /* Currently attached PLL */
    struct rhdLUT *LUT; /* Currently attached LUT */
    struct rhdCursor *Cursor; /* Fixed to the MODE engine */

    DisplayModePtr CurrentMode;
    DisplayModePtr Modes; /* Validated ones: Cycle through these */

    DisplayModePtr ScaledMode; /* usually a fixed mode from one of the monitors */

    ModeStatus (*FBValid) (struct rhdCrtc *Crtc, CARD16 Width, CARD16 Height,
			   int bpp, CARD32 Offset, CARD32 Size, CARD32 *pPitch);
    void (*FBSet) (struct rhdCrtc *Crtc, CARD16 Pitch, CARD16 Width,
		   CARD16 Height, int bpp, CARD32 Offset);

    ModeStatus (*ModeValid) (struct rhdCrtc *Crtc, DisplayModePtr Mode);
    void (*ModeSet) (struct rhdCrtc *Crtc, DisplayModePtr Mode);

#define RHD_CRTC_SCALE_TYPE_NONE    0   /* top left */
#define RHD_CRTC_SCALE_TYPE_CENTER  1   /* center of the actual mode */
#define RHD_CRTC_SCALE_TYPE_SCALE   2   /* scaled to fullscreen */
    ModeStatus (*ScaleValid) (struct rhdCrtc *Crtc, CARD32 Type, DisplayModePtr Mode, DisplayModePtr ScaledMode);
    void (*ScaleSet) (struct rhdCrtc *Crtc, CARD32 Type, DisplayModePtr Mode, DisplayModePtr ScaledMode);

    void (*FrameSet) (struct rhdCrtc *Crtc, CARD16 X, CARD16 Y);

    void (*PLLSelect) (struct rhdCrtc *Crtc, struct rhdPLL *PLL);
    void (*LUTSelect) (struct rhdCrtc *Crtc, struct rhdLUT *LUT);

    void (*Power) (struct rhdCrtc *Crtc, int Power);
    void (*Blank) (struct rhdCrtc *Crtc, Bool Blank);

    struct rhdCrtcStore *Store;
    void (*Save) (struct rhdCrtc *Crtc);
    void (*Restore) (struct rhdCrtc *Crtc);

    struct rhdFMTStore *FMTStore;
    void (*FMTModeSet)(struct rhdCrtc *Crtc, struct rhdFMTDither *FMTDither);
    void (*FMTSave)(struct rhdCrtc *Crtc);
    void (*FMTRestore)(struct rhdCrtc *Crtc);

    /* Gamma, scaling */
};

void RHDCrtcsInit(RHDPtr rhdPtr);
void RHDCrtcsDestroy(RHDPtr rhdPtr);

#endif /* _RHD_CRTC_H */
