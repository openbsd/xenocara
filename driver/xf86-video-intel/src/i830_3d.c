/**************************************************************************
 * 
 * Copyright 2003 Tungsten Graphics, Inc., Cedar Park, Texas.
 * All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 **************************************************************************/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "i830.h"

#include "i830_reg.h"

#define CMD_3D (0x3<<29)

void I830EmitInvarientState( ScrnInfoPtr pScrn )
{
   I830Ptr pI830 = I830PTR(pScrn);

   BEGIN_LP_RING(38);

   OUT_RING(_3DSTATE_MAP_CUBE | MAP_UNIT(0));
   OUT_RING(_3DSTATE_MAP_CUBE | MAP_UNIT(1));
   OUT_RING(_3DSTATE_MAP_CUBE | MAP_UNIT(2));
   OUT_RING(_3DSTATE_MAP_CUBE | MAP_UNIT(3));

   OUT_RING(_3DSTATE_DFLT_DIFFUSE_CMD);
   OUT_RING(0);

   OUT_RING(_3DSTATE_DFLT_SPEC_CMD);
   OUT_RING(0);

   OUT_RING(_3DSTATE_DFLT_Z_CMD);
   OUT_RING(0);

   OUT_RING(_3DSTATE_FOG_MODE_CMD);
   OUT_RING(FOGFUNC_ENABLE |
	     FOG_LINEAR_CONST | 
	     FOGSRC_INDEX_Z | 
	     ENABLE_FOG_DENSITY);
   OUT_RING(0);
   OUT_RING(0);


   OUT_RING(_3DSTATE_MAP_TEX_STREAM_CMD |
	     MAP_UNIT(0) |
	     DISABLE_TEX_STREAM_BUMP |
	     ENABLE_TEX_STREAM_COORD_SET |
	     TEX_STREAM_COORD_SET(0) |
	     ENABLE_TEX_STREAM_MAP_IDX | TEX_STREAM_MAP_IDX(0));
   OUT_RING(_3DSTATE_MAP_TEX_STREAM_CMD |
	     MAP_UNIT(1) |
	     DISABLE_TEX_STREAM_BUMP |
	     ENABLE_TEX_STREAM_COORD_SET |
	     TEX_STREAM_COORD_SET(1) |
	     ENABLE_TEX_STREAM_MAP_IDX | TEX_STREAM_MAP_IDX(1));
   OUT_RING(_3DSTATE_MAP_TEX_STREAM_CMD |
	     MAP_UNIT(2) |
	     DISABLE_TEX_STREAM_BUMP |
	     ENABLE_TEX_STREAM_COORD_SET |
	     TEX_STREAM_COORD_SET(2) |
	     ENABLE_TEX_STREAM_MAP_IDX | TEX_STREAM_MAP_IDX(2));
   OUT_RING(_3DSTATE_MAP_TEX_STREAM_CMD |
	     MAP_UNIT(3) |
	     DISABLE_TEX_STREAM_BUMP |
	     ENABLE_TEX_STREAM_COORD_SET |
	     TEX_STREAM_COORD_SET(3) |
	     ENABLE_TEX_STREAM_MAP_IDX | TEX_STREAM_MAP_IDX(3));

   OUT_RING(_3DSTATE_MAP_COORD_TRANSFORM);
   OUT_RING(DISABLE_TEX_TRANSFORM | TEXTURE_SET(0));
   OUT_RING(_3DSTATE_MAP_COORD_TRANSFORM);
   OUT_RING(DISABLE_TEX_TRANSFORM | TEXTURE_SET(1));
   OUT_RING(_3DSTATE_MAP_COORD_TRANSFORM);
   OUT_RING(DISABLE_TEX_TRANSFORM | TEXTURE_SET(2));
   OUT_RING(_3DSTATE_MAP_COORD_TRANSFORM);
   OUT_RING(DISABLE_TEX_TRANSFORM | TEXTURE_SET(3));

   OUT_RING(_3DSTATE_RASTER_RULES_CMD |
	     ENABLE_POINT_RASTER_RULE |
	     OGL_POINT_RASTER_RULE |
	     ENABLE_LINE_STRIP_PROVOKE_VRTX |
	     ENABLE_TRI_FAN_PROVOKE_VRTX |
	     ENABLE_TRI_STRIP_PROVOKE_VRTX |
	     LINE_STRIP_PROVOKE_VRTX(1) |
	     TRI_FAN_PROVOKE_VRTX(2) | 
	     TRI_STRIP_PROVOKE_VRTX(2));

   OUT_RING(_3DSTATE_SCISSOR_ENABLE_CMD | 
	     DISABLE_SCISSOR_RECT);

   OUT_RING(_3DSTATE_SCISSOR_RECT_0_CMD);
   OUT_RING(0);
   OUT_RING(0);

   OUT_RING(_3DSTATE_VERTEX_TRANSFORM);
   OUT_RING(DISABLE_VIEWPORT_TRANSFORM | DISABLE_PERSPECTIVE_DIVIDE);

   OUT_RING(_3DSTATE_W_STATE_CMD);
   OUT_RING(MAGIC_W_STATE_DWORD1);
   OUT_RING(0x3f800000 /* 1.0 in IEEE float */ );


   OUT_RING(_3DSTATE_COLOR_FACTOR_CMD);
   OUT_RING(0x80808080);	/* .5 required in alpha for GL_DOT3_RGBA_EXT */

   ADVANCE_LP_RING();
}
