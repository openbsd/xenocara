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
#include "intel.h"

#include "i915_reg.h"

void I915EmitInvarientState(ScrnInfoPtr scrn)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);

	assert(intel->in_batch_atomic);

	OUT_BATCH(_3DSTATE_AA_CMD |
		  AA_LINE_ECAAR_WIDTH_ENABLE |
		  AA_LINE_ECAAR_WIDTH_1_0 |
		  AA_LINE_REGION_WIDTH_ENABLE | AA_LINE_REGION_WIDTH_1_0);

	/* Disable independent alpha blend */
	OUT_BATCH(_3DSTATE_INDEPENDENT_ALPHA_BLEND_CMD |
		  IAB_MODIFY_ENABLE |
		  IAB_MODIFY_FUNC | (BLENDFUNC_ADD << IAB_FUNC_SHIFT) |
		  IAB_MODIFY_SRC_FACTOR | (BLENDFACT_ONE <<
					   IAB_SRC_FACTOR_SHIFT) |
		  IAB_MODIFY_DST_FACTOR | (BLENDFACT_ZERO <<
					   IAB_DST_FACTOR_SHIFT));

	OUT_BATCH(_3DSTATE_DFLT_DIFFUSE_CMD);
	OUT_BATCH(0);

	OUT_BATCH(_3DSTATE_DFLT_SPEC_CMD);
	OUT_BATCH(0);

	OUT_BATCH(_3DSTATE_DFLT_Z_CMD);
	OUT_BATCH(0);

	/* Don't support texture crossbar yet */
	OUT_BATCH(_3DSTATE_COORD_SET_BINDINGS |
		  CSB_TCB(0, 0) |
		  CSB_TCB(1, 1) |
		  CSB_TCB(2, 2) |
		  CSB_TCB(3, 3) |
		  CSB_TCB(4, 4) |
		  CSB_TCB(5, 5) | CSB_TCB(6, 6) | CSB_TCB(7, 7));

	OUT_BATCH(_3DSTATE_RASTER_RULES_CMD |
		  ENABLE_POINT_RASTER_RULE |
		  OGL_POINT_RASTER_RULE |
		  ENABLE_LINE_STRIP_PROVOKE_VRTX |
		  ENABLE_TRI_FAN_PROVOKE_VRTX |
		  LINE_STRIP_PROVOKE_VRTX(1) |
		  TRI_FAN_PROVOKE_VRTX(2) | ENABLE_TEXKILL_3D_4D | TEXKILL_4D);

	OUT_BATCH(_3DSTATE_MODES_4_CMD |
		  ENABLE_LOGIC_OP_FUNC | LOGIC_OP_FUNC(LOGICOP_COPY) |
		  ENABLE_STENCIL_WRITE_MASK | STENCIL_WRITE_MASK(0xff) |
		  ENABLE_STENCIL_TEST_MASK | STENCIL_TEST_MASK(0xff));

	OUT_BATCH(_3DSTATE_LOAD_STATE_IMMEDIATE_1 | I1_LOAD_S(3) | I1_LOAD_S(4) | I1_LOAD_S(5) | 2);
	OUT_BATCH(0x00000000);	/* Disable texture coordinate wrap-shortest */
	OUT_BATCH((1 << S4_POINT_WIDTH_SHIFT) |
		  S4_LINE_WIDTH_ONE |
		  S4_CULLMODE_NONE |
		  S4_VFMT_XY);
	OUT_BATCH(0x00000000);	/* Stencil. */

	OUT_BATCH(_3DSTATE_SCISSOR_ENABLE_CMD | DISABLE_SCISSOR_RECT);
	OUT_BATCH(_3DSTATE_SCISSOR_RECT_0_CMD);
	OUT_BATCH(0);
	OUT_BATCH(0);

	OUT_BATCH(_3DSTATE_DEPTH_SUBRECT_DISABLE);

	OUT_BATCH(_3DSTATE_LOAD_INDIRECT | 0);	/* disable indirect state */
	OUT_BATCH(0);

	OUT_BATCH(_3DSTATE_STIPPLE);
	OUT_BATCH(0x00000000);

	OUT_BATCH(_3DSTATE_BACKFACE_STENCIL_OPS | BFO_ENABLE_STENCIL_TWO_SIDE | 0);
}
