/*
 * Copyright 2006 Thomas Hellstrom. All Rights Reserved.
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

#ifndef VIA_3D_H
#define VIA_3D_H

#include "xorg-server.h"
#include "xf86.h"
#include "via_dmabuffer.h"

#define VIA_NUM_TEXUNITS 2

typedef enum
{
    via_single,
    via_clamp,
    via_repeat,
    via_mirror,
    via_warp
} ViaTextureModes;

typedef enum
{
    via_src,
    via_src_onepix_mask,
    via_src_onepix_comp_mask,
    via_mask,
    via_comp_mask
} ViaTexBlendingModes;

typedef struct _ViaTextureUnit
{
    CARD32 textureLevel0Offset;
    CARD32 textureLevel0Pitch;
    CARD32 textureLevel0Exp;
    CARD32 textureLevel0WExp;
    CARD32 textureLevel0HExp;
    CARD32 textureFormat;
    CARD32 textureModesT;
    CARD32 textureModesS;
    CARD32 texCsat;
    CARD32 texRCa;
    CARD32 texAsat;
    CARD32 texRAa;
    Bool agpTexture;
    Bool textureDirty;
    Bool texBColDirty;
    Bool npot;
} ViaTextureUnit;

typedef struct _Via3DState
{
    Bool destDirty;
    Bool blendDirty;
    Bool enableDirty;
    Bool drawingDirty;
    CARD32 rop;
    CARD32 planeMask;
    CARD32 solidColor;
    CARD32 solidAlpha;
    CARD32 destOffset;
    CARD32 destPitch;
    CARD32 destFormat;
    int destDepth;
    int numTextures;
    Bool blend;
    CARD32 blendCol0;
    CARD32 blendCol1;
    CARD32 blendAl0;
    CARD32 blendAl1;
    Bool writeAlpha;
    Bool writeColor;
    Bool useDestAlpha;
    ViaTextureUnit tex[VIA_NUM_TEXUNITS];
    void (*setDestination) (struct _Via3DState * v3d, CARD32 offset,
	CARD32 pitch, int format);
    void (*setDrawing) (struct _Via3DState * v3d, int rop,
	CARD32 planeMask, CARD32 solidColor, CARD32 solidAlpha);
    void (*setFlags) (struct _Via3DState * v3d, int numTextures,
	Bool writeAlpha, Bool writeColor, Bool blend);
        Bool(*setTexture) (struct _Via3DState * v3d, int tex, CARD32 offset,
	CARD32 pitch, Bool nPot, CARD32 width, CARD32 height, int format,
	ViaTextureModes sMode, ViaTextureModes tMode,
	ViaTexBlendingModes blendingMode, Bool agpTexture);
    void (*setTexBlendCol) (struct _Via3DState * v3d, int tex, Bool component,
	CARD32 color);
    void (*setCompositeOperator) (struct _Via3DState * v3d, CARD8 op);
        Bool(*opSupported) (CARD8 op);
    void (*emitQuad) (struct _Via3DState * v3d, ViaCommandBuffer * cb,
	int dstX, int dstY, int src0X, int src0Y, int src1X, int src1Y, int w,
	int h);
    void (*emitState) (struct _Via3DState * v3d, ViaCommandBuffer * cb,
	Bool forceUpload);
    void (*emitClipRect) (struct _Via3DState * v3d, ViaCommandBuffer * cb,
	int x, int y, int w, int h);
        Bool(*dstSupported) (int format);
        Bool(*texSupported) (int format);
} Via3DState;

void viaInit3DState(Via3DState * v3d);

#endif
