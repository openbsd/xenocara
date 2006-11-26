/*
 * Copyright © 2006 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86xv.h"
#include "fourcc.h"

#include "i830.h"
#include "i830_video.h"
#include "i915_reg.h"
#include "i915_3d.h"

union intfloat {
   CARD32 ui;
   float f;
};

#define OUT_RING_F(x) do {						\
   union intfloat _tmp;							\
   _tmp.f = x;								\
   OUT_RING(_tmp.ui);							\
} while (0)


void
I915DisplayVideoTextured(ScrnInfoPtr pScrn, I830PortPrivPtr pPriv, int id,
			 RegionPtr dstRegion,
			 short width, short height, int video_pitch,
			 int x1, int y1, int x2, int y2,
			 short src_w, short src_h, short drw_w, short drw_h,
			 DrawablePtr pDraw)
{
   I830Ptr pI830 = I830PTR(pScrn);
   CARD32 format, ms3, s2;
   BoxPtr pbox;
   int nbox, dxo, dyo;
   Bool planar;

   ErrorF("I915DisplayVideo: %dx%d (pitch %d)\n", width, height,
	  video_pitch);

   switch (id) {
   case FOURCC_UYVY:
   case FOURCC_YUY2:
      planar = FALSE;
      break;
   case FOURCC_YV12:
   case FOURCC_I420:
      planar = TRUE;
      break;
   default:
      ErrorF("Unknown format 0x%x\n", id);
      planar = FALSE;
      break;
   }

   /* Tell the rotation code that we have stomped its invariant state by
    * setting a high bit.  We don't use any invariant 3D state for video, so we
    * don't have to worry about it ourselves.
    */
   *pI830->used3D |= 1 << 30;

   BEGIN_LP_RING(44);

   /* invarient state */
   OUT_RING(MI_NOOP);
   OUT_RING(_3DSTATE_AA_CMD |
	    AA_LINE_ECAAR_WIDTH_ENABLE | AA_LINE_ECAAR_WIDTH_1_0 |
	    AA_LINE_REGION_WIDTH_ENABLE | AA_LINE_REGION_WIDTH_1_0);

   OUT_RING(_3DSTATE_DFLT_DIFFUSE_CMD);
   OUT_RING(0x00000000);

   OUT_RING(_3DSTATE_DFLT_SPEC_CMD);
   OUT_RING(0x00000000);

   OUT_RING(_3DSTATE_DFLT_Z_CMD);
   OUT_RING(0x00000000);

   OUT_RING(_3DSTATE_COORD_SET_BINDINGS | CSB_TCB(0, 0) | CSB_TCB(1, 1) |
	    CSB_TCB(2,2) | CSB_TCB(3,3) | CSB_TCB(4,4) | CSB_TCB(5,5) |
	    CSB_TCB(6,6) | CSB_TCB(7,7));

   OUT_RING(_3DSTATE_RASTER_RULES_CMD |
	    ENABLE_TRI_FAN_PROVOKE_VRTX | TRI_FAN_PROVOKE_VRTX(2) |
	    ENABLE_LINE_STRIP_PROVOKE_VRTX | LINE_STRIP_PROVOKE_VRTX(1) |
	    ENABLE_TEXKILL_3D_4D | TEXKILL_4D |
	    ENABLE_POINT_RASTER_RULE | OGL_POINT_RASTER_RULE);

   OUT_RING(_3DSTATE_LOAD_STATE_IMMEDIATE_1 | I1_LOAD_S(3) | 1);
   OUT_RING(0x00000000); /* texture coordinate wrap */

   /* flush map & render cache */
   OUT_RING(MI_FLUSH | MI_WRITE_DIRTY_STATE | MI_INVALIDATE_MAP_CACHE);
   OUT_RING(0x00000000);

   /* draw rect -- just clipping */
   OUT_RING(_3DSTATE_DRAW_RECT_CMD);
   OUT_RING(0x00000000);	/* flags */
   OUT_RING(0x00000000);	/* ymin, xmin */
   OUT_RING((pScrn->virtualX - 1) |
	    (pScrn->virtualY - 1) << 16); /* ymax, xmax */
   OUT_RING(0x00000000);	/* yorigin, xorigin */
   OUT_RING(MI_NOOP);

   /* scissor */
   OUT_RING(_3DSTATE_SCISSOR_ENABLE_CMD | DISABLE_SCISSOR_RECT);
   OUT_RING(_3DSTATE_SCISSOR_RECT_0_CMD);
   OUT_RING(0x00000000);	/* ymin, xmin */
   OUT_RING(0x00000000);	/* ymax, xmax */

   OUT_RING(0x7c000003);	/* unknown command */
   OUT_RING(0x7d070000);
   OUT_RING(0x00000000);
   OUT_RING(0x68000002);

   /* context setup */
   OUT_RING(_3DSTATE_MODES_4_CMD |
	    ENABLE_LOGIC_OP_FUNC | LOGIC_OP_FUNC(LOGICOP_COPY) |
	    ENABLE_STENCIL_WRITE_MASK | STENCIL_WRITE_MASK(0xff) |
	    ENABLE_STENCIL_TEST_MASK | STENCIL_TEST_MASK(0xff));

   OUT_RING(_3DSTATE_LOAD_STATE_IMMEDIATE_1 | I1_LOAD_S(2) |
	    I1_LOAD_S(4) | I1_LOAD_S(5) | I1_LOAD_S(6) | 4);
   s2 = S2_TEXCOORD_FMT(0, TEXCOORDFMT_2D);
   if (planar)
      s2 |= S2_TEXCOORD_FMT(1, TEXCOORDFMT_2D);
   else
      s2 |= S2_TEXCOORD_FMT(1, TEXCOORDFMT_NOT_PRESENT);
   s2 |= S2_TEXCOORD_FMT(2, TEXCOORDFMT_NOT_PRESENT) |
      S2_TEXCOORD_FMT(3, TEXCOORDFMT_NOT_PRESENT) |
      S2_TEXCOORD_FMT(4, TEXCOORDFMT_NOT_PRESENT) |
      S2_TEXCOORD_FMT(5, TEXCOORDFMT_NOT_PRESENT) |
      S2_TEXCOORD_FMT(6, TEXCOORDFMT_NOT_PRESENT) |
      S2_TEXCOORD_FMT(7, TEXCOORDFMT_NOT_PRESENT);
   OUT_RING(s2);
   OUT_RING((1 << S4_POINT_WIDTH_SHIFT) | S4_LINE_WIDTH_ONE |
	    S4_CULLMODE_NONE | S4_VFMT_XY);
   OUT_RING(0x00000000); /* S5 - enable bits */
   OUT_RING((2 << S6_DEPTH_TEST_FUNC_SHIFT) |
	    (2 << S6_CBUF_SRC_BLEND_FACT_SHIFT) |
	    (1 << S6_CBUF_DST_BLEND_FACT_SHIFT) | S6_COLOR_WRITE_ENABLE |
	    (2 << S6_TRISTRIP_PV_SHIFT));

   OUT_RING(_3DSTATE_INDEPENDENT_ALPHA_BLEND_CMD |
	    IAB_MODIFY_ENABLE |
	    IAB_MODIFY_FUNC | (BLENDFUNC_ADD << IAB_FUNC_SHIFT) |
	    IAB_MODIFY_SRC_FACTOR | (BLENDFACT_ONE << IAB_SRC_FACTOR_SHIFT) |
	    IAB_MODIFY_DST_FACTOR | (BLENDFACT_ZERO << IAB_DST_FACTOR_SHIFT));

   OUT_RING(_3DSTATE_CONST_BLEND_COLOR_CMD);
   OUT_RING(0x00000000);

   OUT_RING(_3DSTATE_DST_BUF_VARS_CMD);
   if (pI830->cpp == 2)
      format = COLR_BUF_RGB565;
   else
      format = COLR_BUF_ARGB8888 | DEPTH_FRMT_24_FIXED_8_OTHER;

   OUT_RING(LOD_PRECLAMP_OGL |
     DSTORG_HORT_BIAS(0x80) | DSTORG_VERT_BIAS(0x80) | format);

   OUT_RING(_3DSTATE_STIPPLE);
   OUT_RING(0x00000000);

   /* front buffer, pitch, offset */
   OUT_RING(_3DSTATE_BUF_INFO_CMD);
   OUT_RING(BUF_3D_ID_COLOR_BACK | BUF_3D_USE_FENCE |
	    (((pI830->displayWidth * pI830->cpp) / 4) << 2));
   OUT_RING(pI830->bufferOffset);
   ADVANCE_LP_RING();

   if (!planar) {
      FS_LOCALS(3);

      BEGIN_LP_RING(10);
      OUT_RING(_3DSTATE_SAMPLER_STATE | 3);
      OUT_RING(0x00000001);
      OUT_RING(SS2_COLORSPACE_CONVERSION |
	       (FILTER_LINEAR << SS2_MAG_FILTER_SHIFT) |
	       (FILTER_LINEAR << SS2_MIN_FILTER_SHIFT));
      OUT_RING((TEXCOORDMODE_CLAMP_EDGE << SS3_TCX_ADDR_MODE_SHIFT) |
	       (TEXCOORDMODE_CLAMP_EDGE << SS3_TCY_ADDR_MODE_SHIFT));
      OUT_RING(0x00000000);

      OUT_RING(_3DSTATE_MAP_STATE | 3);
      OUT_RING(0x00000001);	/* texture map #1 */
      OUT_RING(pPriv->YBuf0offset);
      ms3 = MAPSURF_422;
      switch (id) {
      case FOURCC_YUY2:
	 ms3 |= MT_422_YCRCB_NORMAL;
	 break;
      case FOURCC_UYVY:
	 ms3 |= MT_422_YCRCB_SWAPY;
	 break;
      }
      ms3 |= (height - 1) << MS3_HEIGHT_SHIFT;
      ms3 |= (width - 1) << MS3_WIDTH_SHIFT;
      if (!pI830->disableTiling)
	 ms3 |= MS3_USE_FENCE_REGS;
      OUT_RING(ms3);
      OUT_RING(((video_pitch / 4) - 1) << 21);
      ADVANCE_LP_RING();

      FS_BEGIN();
      i915_fs_dcl(FS_S0);
      i915_fs_dcl(FS_T0);
      i915_fs_texld(FS_OC, FS_S0, FS_T0);
      FS_END();
   } else {
      FS_LOCALS(16);

      BEGIN_LP_RING(1 + 18 + 11 + 11);
      OUT_RING(MI_NOOP);
      /* For the planar formats, we set up three samplers -- one for each plane,
       * in a Y8 format.  Because I couldn't get the special PLANAR_TO_PACKED
       * shader setup to work, I did the manual pixel shader:
       *
       * y' = y - .0625
       * u' = u - .5
       * v' = v - .5;
       *
       * r = 1.1643 * y' + 0.0     * u' + 1.5958  * v'
       * g = 1.1643 * y' - 0.39173 * u' - 0.81290 * v'
       * b = 1.1643 * y' + 2.017   * u' + 0.0     * v'
       *
       * register assignment:
       * r0 = (y',u',v',0)
       * r1 = (y,y,y,y)
       * r2 = (u,u,u,u)
       * r3 = (v,v,v,v)
       * OC = (r,g,b,1)
       */
      OUT_RING(_3DSTATE_PIXEL_SHADER_CONSTANTS | 16);
      OUT_RING(0x000000f);	/* constants 0-3 */
      /* constant 0: normalization offsets */
      OUT_RING_F(-0.0625);
      OUT_RING_F(-0.5);
      OUT_RING_F(-0.5);
      OUT_RING_F(0.0);
      /* constant 1: r coefficients*/
      OUT_RING_F(1.1643);
      OUT_RING_F(0.0);
      OUT_RING_F(1.5958);
      OUT_RING_F(0.0);
      /* constant 2: g coefficients */
      OUT_RING_F(1.1643);
      OUT_RING_F(-0.39173);
      OUT_RING_F(-0.81290);
      OUT_RING_F(0.0);
      /* constant 3: b coefficients */
      OUT_RING_F(1.1643);
      OUT_RING_F(2.017);
      OUT_RING_F(0.0);
      OUT_RING_F(0.0);

      OUT_RING(_3DSTATE_SAMPLER_STATE | 9);
      OUT_RING(0x00000007);
      /* sampler 0 */
      OUT_RING(0x00000000);
      OUT_RING((FILTER_LINEAR << SS2_MAG_FILTER_SHIFT) |
	       (FILTER_LINEAR << SS2_MIN_FILTER_SHIFT));
      OUT_RING((TEXCOORDMODE_CLAMP_EDGE << SS3_TCX_ADDR_MODE_SHIFT) |
	       (TEXCOORDMODE_CLAMP_EDGE << SS3_TCY_ADDR_MODE_SHIFT));
      /* sampler 1 */
      OUT_RING(0x00000000);
      OUT_RING((FILTER_LINEAR << SS2_MAG_FILTER_SHIFT) |
	       (FILTER_LINEAR << SS2_MIN_FILTER_SHIFT));
      OUT_RING((TEXCOORDMODE_CLAMP_EDGE << SS3_TCX_ADDR_MODE_SHIFT) |
	       (TEXCOORDMODE_CLAMP_EDGE << SS3_TCY_ADDR_MODE_SHIFT));
      /* sampler 2 */
      OUT_RING(0x00000000);
      OUT_RING((FILTER_LINEAR << SS2_MAG_FILTER_SHIFT) |
	       (FILTER_LINEAR << SS2_MIN_FILTER_SHIFT));
      OUT_RING((TEXCOORDMODE_CLAMP_EDGE << SS3_TCX_ADDR_MODE_SHIFT) |
	       (TEXCOORDMODE_CLAMP_EDGE << SS3_TCY_ADDR_MODE_SHIFT));

      OUT_RING(_3DSTATE_MAP_STATE | 9);
      OUT_RING(0x00000007);

      OUT_RING(pPriv->YBuf0offset);
      ms3 = MAPSURF_8BIT | MT_8BIT_I8;
      ms3 |= (height - 1) << MS3_HEIGHT_SHIFT;
      ms3 |= (width - 1) << MS3_WIDTH_SHIFT;
      OUT_RING(ms3);
      OUT_RING(((video_pitch * 2 / 4) - 1) << 21);

      OUT_RING(pPriv->UBuf0offset);
      ms3 = MAPSURF_8BIT | MT_8BIT_I8;
      ms3 |= (height / 2 - 1) << MS3_HEIGHT_SHIFT;
      ms3 |= (width / 2 - 1) << MS3_WIDTH_SHIFT;
      OUT_RING(ms3);
      OUT_RING(((video_pitch / 4) - 1) << 21);

      OUT_RING(pPriv->VBuf0offset);
      ms3 = MAPSURF_8BIT | MT_8BIT_I8;
      ms3 |= (height / 2 - 1) << MS3_HEIGHT_SHIFT;
      ms3 |= (width / 2 - 1) << MS3_WIDTH_SHIFT;
      OUT_RING(ms3);
      OUT_RING(((video_pitch / 4) - 1) << 21);
      ADVANCE_LP_RING();

      FS_BEGIN();
      /* Declare samplers */
      i915_fs_dcl(FS_S0);
      i915_fs_dcl(FS_S1);
      i915_fs_dcl(FS_S2);
      i915_fs_dcl(FS_T0);
      i915_fs_dcl(FS_T1);

      /* Load samplers to temporaries.  Y (sampler 0) gets the un-halved coords-
       * from t1.
       */
      i915_fs_texld(FS_R1, FS_S0, FS_T1);
      i915_fs_texld(FS_R2, FS_S1, FS_T0);
      i915_fs_texld(FS_R3, FS_S2, FS_T0);

      /* Move the sampled YUV data in R[123] to the first 3 channels of R0. */
      i915_fs_mov_masked(FS_R0, MASK_X, i915_fs_operand_reg(FS_R1));
      i915_fs_mov_masked(FS_R0, MASK_Y, i915_fs_operand_reg(FS_R2));
      i915_fs_mov_masked(FS_R0, MASK_Z, i915_fs_operand_reg(FS_R3));

      /* Normalize the YUV data */
      i915_fs_add(FS_R0, i915_fs_operand_reg(FS_R0),
                 i915_fs_operand_reg(FS_C0));
      /* dot-product the YUV data in R0 by the vectors of coefficients for
       * calculating R, G, and B, storing the results in the R, G, or B channels
       * of the output color.
       */
      i915_fs_dp3_masked(FS_OC, MASK_X | MASK_SATURATE,
                        i915_fs_operand_reg(FS_R0),
                        i915_fs_operand_reg(FS_C1));
      i915_fs_dp3_masked(FS_OC, MASK_Y | MASK_SATURATE,
                        i915_fs_operand_reg(FS_R0),
                        i915_fs_operand_reg(FS_C2));
      i915_fs_dp3_masked(FS_OC, MASK_Z | MASK_SATURATE,
                        i915_fs_operand_reg(FS_R0),
                        i915_fs_operand_reg(FS_C3));
      /* Set alpha of the output to 1.0, by wiring W to 1 and not actually using
       * the source.
       */
      i915_fs_mov_masked(FS_OC, MASK_W, i915_fs_operand_one());
      FS_END();
   }
   
   {
      BEGIN_LP_RING(2);
      OUT_RING(MI_FLUSH | MI_WRITE_DIRTY_STATE | MI_INVALIDATE_MAP_CACHE);
      OUT_RING(0x00000000);
      ADVANCE_LP_RING();
   }

   dxo = dstRegion->extents.x1;
   dyo = dstRegion->extents.y1;

   pbox = REGION_RECTS(dstRegion);
   nbox = REGION_NUM_RECTS(dstRegion);
   while (nbox--)
   {
      int box_x1 = pbox->x1;
      int box_y1 = pbox->y1;
      int box_x2 = pbox->x2;
      int box_y2 = pbox->y2;
      float src_scale_x, src_scale_y;
      int vert_data_count;

      pbox++;

      src_scale_x = (float)src_w / (float)drw_w;
      src_scale_y  = (float)src_h / (float)drw_h;

      if (!planar)
	 vert_data_count = 12;
      else
	 vert_data_count = 18;

      BEGIN_LP_RING(vert_data_count + 8);
      OUT_RING(MI_NOOP);
      OUT_RING(MI_NOOP);
      OUT_RING(MI_NOOP);
      OUT_RING(MI_NOOP);
      OUT_RING(MI_NOOP);
      OUT_RING(MI_NOOP);
      OUT_RING(MI_NOOP);

      /* vertex data - rect list consists of bottom right, bottom left, and top
       * left vertices.
       */
      OUT_RING(PRIM3D_INLINE | PRIM3D_RECTLIST |
	       (vert_data_count - 1));

      /* bottom right */
      OUT_RING_F(box_x2);
      OUT_RING_F(box_y2);
      if (!planar) {
	 OUT_RING_F((box_x2 - dxo) * src_scale_x);
	 OUT_RING_F((box_y2 - dyo) * src_scale_y);
      } else {
	 OUT_RING_F((box_x2 - dxo) * src_scale_x / 2.0);
	 OUT_RING_F((box_y2 - dyo) * src_scale_y / 2.0);
	 OUT_RING_F((box_x2 - dxo) * src_scale_x);
	 OUT_RING_F((box_y2 - dyo) * src_scale_y);
      }

      /* bottom left */
      OUT_RING_F(box_x1);
      OUT_RING_F(box_y2);
      if (!planar) {
	 OUT_RING_F((box_x1 - dxo) * src_scale_x);
	 OUT_RING_F((box_y2 - dyo) * src_scale_y);
      } else {
	 OUT_RING_F((box_x1 - dxo) * src_scale_x / 2.0);
	 OUT_RING_F((box_y2 - dyo) * src_scale_y / 2.0);
	 OUT_RING_F((box_x1 - dxo) * src_scale_x);
	 OUT_RING_F((box_y2 - dyo) * src_scale_y);
      }

      /* top left */
      OUT_RING_F(box_x1);
      OUT_RING_F(box_y1);
      if (!planar) {
	 OUT_RING_F((box_x1 - dxo) * src_scale_x);
	 OUT_RING_F((box_y1 - dyo) * src_scale_y);
      } else {
	 OUT_RING_F((box_x1 - dxo) * src_scale_x / 2.0);
	 OUT_RING_F((box_y1 - dyo) * src_scale_y / 2.0);
	 OUT_RING_F((box_x1 - dxo) * src_scale_x);
	 OUT_RING_F((box_y1 - dyo) * src_scale_y);
      }

      ADVANCE_LP_RING();
   }

   if (pI830->AccelInfoRec)
      pI830->AccelInfoRec->NeedToSync = TRUE;
}

