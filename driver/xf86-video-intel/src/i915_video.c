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
#include "gcstruct.h"

#include "intel.h"
#include "intel_video.h"
#include "i915_reg.h"
#include "i915_3d.h"

void
I915DisplayVideoTextured(ScrnInfoPtr scrn,
			 intel_adaptor_private *adaptor_priv, int id,
			 RegionPtr dstRegion,
			 short width, short height, int video_pitch,
			 int video_pitch2,
			 short src_w, short src_h, short drw_w, short drw_h,
			 PixmapPtr pixmap)
{
	intel_screen_private *intel = intel_get_screen_private(scrn);
	uint32_t format, ms3, s5, tiling;
	BoxPtr pbox = REGION_RECTS(dstRegion);
	int nbox_total = REGION_NUM_RECTS(dstRegion);
	int nbox_this_time;
	int dxo, dyo, pix_xoff, pix_yoff;
	PixmapPtr target;

#if 0
	ErrorF("I915DisplayVideo: %dx%d (pitch %d)\n", width, height,
	       video_pitch);
#endif

	dxo = dstRegion->extents.x1;
	dyo = dstRegion->extents.y1;

	if (pixmap->drawable.width > 2048 || pixmap->drawable.height > 2048 ||
	    !intel_check_pitch_3d(pixmap)) {
		ScreenPtr screen = pixmap->drawable.pScreen;

		target = screen->CreatePixmap(screen,
					      dstRegion->extents.x2 - dxo,
					      dstRegion->extents.y2 - dyo,
					      pixmap->drawable.depth,
					      CREATE_PIXMAP_USAGE_SCRATCH);
		if (target == NULL)
			return;

		pix_xoff = -dxo;
		pix_yoff = -dyo;
	} else {
		target = pixmap;

		/* Set up the offset for translating from the given region
		 * (in screen coordinates) to the backing pixmap.
		 */
#ifdef COMPOSITE
		pix_xoff = -target->screen_x + target->drawable.x;
		pix_yoff = -target->screen_y + target->drawable.y;
#else
		pix_xoff = 0;
		pix_yoff = 0;
#endif
	}

#define BYTES_FOR_BOXES(n)	((200 + (n) * 20) * 4)
#define BOXES_IN_BYTES(s)	((((s)/4) - 200) / 20)
#define BATCH_BYTES(p)		((p)->batch_bo->size - 16)

	while (nbox_total) {
		nbox_this_time = nbox_total;
		if (BYTES_FOR_BOXES(nbox_this_time) > BATCH_BYTES(intel))
			nbox_this_time = BOXES_IN_BYTES(BATCH_BYTES(intel));
		nbox_total -= nbox_this_time;

		intel_batch_start_atomic(scrn, 200 + 20 * nbox_this_time);

		IntelEmitInvarientState(scrn);
		intel->last_3d = LAST_3D_VIDEO;

		/* draw rect -- just clipping */
		OUT_BATCH(_3DSTATE_DRAW_RECT_CMD);
		OUT_BATCH(DRAW_DITHER_OFS_X(pixmap->drawable.x & 3) |
			  DRAW_DITHER_OFS_Y(pixmap->drawable.y & 3));
		OUT_BATCH(0x00000000);	/* ymin, xmin */
		/* ymax, xmax */
		OUT_BATCH((target->drawable.width - 1) |
			  (target->drawable.height - 1) << 16);
		OUT_BATCH(0x00000000);	/* yorigin, xorigin */

		OUT_BATCH(_3DSTATE_LOAD_STATE_IMMEDIATE_1 | I1_LOAD_S(2) |
			  I1_LOAD_S(5) | I1_LOAD_S(6) | 2);
		OUT_BATCH(S2_TEXCOORD_FMT(0, TEXCOORDFMT_2D) |
			  S2_TEXCOORD_FMT(1, TEXCOORDFMT_NOT_PRESENT) |
			  S2_TEXCOORD_FMT(2, TEXCOORDFMT_NOT_PRESENT) |
			  S2_TEXCOORD_FMT(3, TEXCOORDFMT_NOT_PRESENT) |
			  S2_TEXCOORD_FMT(4, TEXCOORDFMT_NOT_PRESENT) |
			  S2_TEXCOORD_FMT(5, TEXCOORDFMT_NOT_PRESENT) |
			  S2_TEXCOORD_FMT(6, TEXCOORDFMT_NOT_PRESENT) |
			  S2_TEXCOORD_FMT(7, TEXCOORDFMT_NOT_PRESENT));
		s5 = 0x0;
		if (intel->cpp == 2)
			s5 |= S5_COLOR_DITHER_ENABLE;
		OUT_BATCH(s5);	/* S5 - enable bits */
		OUT_BATCH((2 << S6_DEPTH_TEST_FUNC_SHIFT) |
			  (2 << S6_CBUF_SRC_BLEND_FACT_SHIFT) |
			  (1 << S6_CBUF_DST_BLEND_FACT_SHIFT) |
			  S6_COLOR_WRITE_ENABLE | (2 << S6_TRISTRIP_PV_SHIFT));

		OUT_BATCH(_3DSTATE_CONST_BLEND_COLOR_CMD);
		OUT_BATCH(0x00000000);

		OUT_BATCH(_3DSTATE_DST_BUF_VARS_CMD);
		if (intel->cpp == 2)
			format = COLR_BUF_RGB565;
		else
			format =
			    COLR_BUF_ARGB8888 | DEPTH_FRMT_24_FIXED_8_OTHER;

		OUT_BATCH(LOD_PRECLAMP_OGL |
			  DSTORG_HORT_BIAS(0x8) |
			  DSTORG_VERT_BIAS(0x8) | format);

		/* front buffer, pitch, offset */
		if (intel_pixmap_tiled(target)) {
			tiling = BUF_3D_TILED_SURFACE;
			if (intel_get_pixmap_private(target)->tiling == I915_TILING_Y)
				tiling |= BUF_3D_TILE_WALK_Y;
		} else
			tiling = 0;
		OUT_BATCH(_3DSTATE_BUF_INFO_CMD);
		OUT_BATCH(BUF_3D_ID_COLOR_BACK | tiling |
			  BUF_3D_PITCH(intel_pixmap_pitch(target)));
		OUT_RELOC_PIXMAP(target, I915_GEM_DOMAIN_RENDER,
				 I915_GEM_DOMAIN_RENDER, 0);

		if (!is_planar_fourcc(id)) {
			FS_LOCALS();

			OUT_BATCH(_3DSTATE_PIXEL_SHADER_CONSTANTS | 4);
			OUT_BATCH(0x0000001);	/* constant 0 */
			/* constant 0: brightness/contrast */
			OUT_BATCH_F(adaptor_priv->brightness / 128.0);
			OUT_BATCH_F(adaptor_priv->contrast / 255.0);
			OUT_BATCH_F(0.0);
			OUT_BATCH_F(0.0);

			OUT_BATCH(_3DSTATE_SAMPLER_STATE | 3);
			OUT_BATCH(0x00000001);
			OUT_BATCH(SS2_COLORSPACE_CONVERSION |
				  (FILTER_LINEAR << SS2_MAG_FILTER_SHIFT) |
				  (FILTER_LINEAR << SS2_MIN_FILTER_SHIFT));
			OUT_BATCH((TEXCOORDMODE_CLAMP_EDGE <<
				   SS3_TCX_ADDR_MODE_SHIFT) |
				  (TEXCOORDMODE_CLAMP_EDGE <<
				   SS3_TCY_ADDR_MODE_SHIFT) |
				  (0 << SS3_TEXTUREMAP_INDEX_SHIFT) |
				  SS3_NORMALIZED_COORDS);
			OUT_BATCH(0x00000000);

			OUT_BATCH(_3DSTATE_MAP_STATE | 3);
			OUT_BATCH(0x00000001);	/* texture map #1 */
			if (adaptor_priv->buf)
				OUT_RELOC(adaptor_priv->buf,
					  I915_GEM_DOMAIN_SAMPLER, 0,
					  adaptor_priv->YBufOffset);
			else
				OUT_BATCH(adaptor_priv->YBufOffset);

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
			OUT_BATCH(ms3);
			OUT_BATCH(((video_pitch / 4) - 1) << MS4_PITCH_SHIFT);

			FS_BEGIN();
			i915_fs_dcl(FS_S0);
			i915_fs_dcl(FS_T0);
			i915_fs_texld(FS_OC, FS_S0, FS_T0);
			if (adaptor_priv->brightness != 0) {
				i915_fs_add(FS_OC,
					    i915_fs_operand_reg(FS_OC),
					    i915_fs_operand(FS_C0, X, X, X,
							    ZERO));
			}
			FS_END();
		} else {
			FS_LOCALS();

			/* For the planar formats, we set up three samplers --
			 * one for each plane, in a Y8 format.  Because I
			 * couldn't get the special PLANAR_TO_PACKED
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
			OUT_BATCH(_3DSTATE_PIXEL_SHADER_CONSTANTS | (22 - 2));
			OUT_BATCH(0x000001f);	/* constants 0-4 */
			/* constant 0: normalization offsets */
			OUT_BATCH_F(-0.0625);
			OUT_BATCH_F(-0.5);
			OUT_BATCH_F(-0.5);
			OUT_BATCH_F(0.0);
			/* constant 1: r coefficients */
			OUT_BATCH_F(1.1643);
			OUT_BATCH_F(0.0);
			OUT_BATCH_F(1.5958);
			OUT_BATCH_F(0.0);
			/* constant 2: g coefficients */
			OUT_BATCH_F(1.1643);
			OUT_BATCH_F(-0.39173);
			OUT_BATCH_F(-0.81290);
			OUT_BATCH_F(0.0);
			/* constant 3: b coefficients */
			OUT_BATCH_F(1.1643);
			OUT_BATCH_F(2.017);
			OUT_BATCH_F(0.0);
			OUT_BATCH_F(0.0);
			/* constant 4: brightness/contrast */
			OUT_BATCH_F(adaptor_priv->brightness / 128.0);
			OUT_BATCH_F(adaptor_priv->contrast / 255.0);
			OUT_BATCH_F(0.0);
			OUT_BATCH_F(0.0);

			OUT_BATCH(_3DSTATE_SAMPLER_STATE | 9);
			OUT_BATCH(0x00000007);
			/* sampler 0 */
			OUT_BATCH((FILTER_LINEAR << SS2_MAG_FILTER_SHIFT) |
				  (FILTER_LINEAR << SS2_MIN_FILTER_SHIFT));
			OUT_BATCH((TEXCOORDMODE_CLAMP_EDGE <<
				   SS3_TCX_ADDR_MODE_SHIFT) |
				  (TEXCOORDMODE_CLAMP_EDGE <<
				   SS3_TCY_ADDR_MODE_SHIFT) |
				  (0 << SS3_TEXTUREMAP_INDEX_SHIFT) |
				  SS3_NORMALIZED_COORDS);
			OUT_BATCH(0x00000000);
			/* sampler 1 */
			OUT_BATCH((FILTER_LINEAR << SS2_MAG_FILTER_SHIFT) |
				  (FILTER_LINEAR << SS2_MIN_FILTER_SHIFT));
			OUT_BATCH((TEXCOORDMODE_CLAMP_EDGE <<
				   SS3_TCX_ADDR_MODE_SHIFT) |
				  (TEXCOORDMODE_CLAMP_EDGE <<
				   SS3_TCY_ADDR_MODE_SHIFT) |
				  (1 << SS3_TEXTUREMAP_INDEX_SHIFT) |
				  SS3_NORMALIZED_COORDS);
			OUT_BATCH(0x00000000);
			/* sampler 2 */
			OUT_BATCH((FILTER_LINEAR << SS2_MAG_FILTER_SHIFT) |
				  (FILTER_LINEAR << SS2_MIN_FILTER_SHIFT));
			OUT_BATCH((TEXCOORDMODE_CLAMP_EDGE <<
				   SS3_TCX_ADDR_MODE_SHIFT) |
				  (TEXCOORDMODE_CLAMP_EDGE <<
				   SS3_TCY_ADDR_MODE_SHIFT) |
				  (2 << SS3_TEXTUREMAP_INDEX_SHIFT) |
				  SS3_NORMALIZED_COORDS);
			OUT_BATCH(0x00000000);

			OUT_BATCH(_3DSTATE_MAP_STATE | 9);
			OUT_BATCH(0x00000007);

			if (adaptor_priv->buf)
				OUT_RELOC(adaptor_priv->buf,
					  I915_GEM_DOMAIN_SAMPLER, 0,
					  adaptor_priv->YBufOffset);
			else
				OUT_BATCH(adaptor_priv->YBufOffset);

			ms3 = MAPSURF_8BIT | MT_8BIT_I8;
			ms3 |= (height - 1) << MS3_HEIGHT_SHIFT;
			ms3 |= (width - 1) << MS3_WIDTH_SHIFT;
			OUT_BATCH(ms3);
			/* check to see if Y has special pitch than normal
			 * double u/v pitch, e.g i915 XvMC hw requires at
			 * least 1K alignment, so Y pitch might
			 * be same as U/V's.*/
			if (video_pitch2)
				OUT_BATCH(((video_pitch2 / 4) -
					   1) << MS4_PITCH_SHIFT);
			else
				OUT_BATCH(((video_pitch * 2 / 4) -
					   1) << MS4_PITCH_SHIFT);

			if (adaptor_priv->buf)
				OUT_RELOC(adaptor_priv->buf,
					  I915_GEM_DOMAIN_SAMPLER, 0,
					  adaptor_priv->UBufOffset);
			else
				OUT_BATCH(adaptor_priv->UBufOffset);

			ms3 = MAPSURF_8BIT | MT_8BIT_I8;
			ms3 |= (height / 2 - 1) << MS3_HEIGHT_SHIFT;
			ms3 |= (width / 2 - 1) << MS3_WIDTH_SHIFT;
			OUT_BATCH(ms3);
			OUT_BATCH(((video_pitch / 4) - 1) << MS4_PITCH_SHIFT);

			if (adaptor_priv->buf)
				OUT_RELOC(adaptor_priv->buf,
					  I915_GEM_DOMAIN_SAMPLER, 0,
					  adaptor_priv->VBufOffset);
			else
				OUT_BATCH(adaptor_priv->VBufOffset);

			ms3 = MAPSURF_8BIT | MT_8BIT_I8;
			ms3 |= (height / 2 - 1) << MS3_HEIGHT_SHIFT;
			ms3 |= (width / 2 - 1) << MS3_WIDTH_SHIFT;
			OUT_BATCH(ms3);
			OUT_BATCH(((video_pitch / 4) - 1) << MS4_PITCH_SHIFT);

			FS_BEGIN();
			/* Declare samplers */
			i915_fs_dcl(FS_S0);	/* Y */
			i915_fs_dcl(FS_S1);	/* U */
			i915_fs_dcl(FS_S2);	/* V */
			i915_fs_dcl(FS_T0);	/* normalized coords */

			/* Load samplers to temporaries. */
			i915_fs_texld(FS_R1, FS_S0, FS_T0);
			i915_fs_texld(FS_R2, FS_S1, FS_T0);
			i915_fs_texld(FS_R3, FS_S2, FS_T0);

			/* Move the sampled YUV data in R[123] to the first
			 * 3 channels of R0.
			 */
			i915_fs_mov_masked(FS_R0, MASK_X,
					   i915_fs_operand_reg(FS_R1));
			i915_fs_mov_masked(FS_R0, MASK_Y,
					   i915_fs_operand_reg(FS_R2));
			i915_fs_mov_masked(FS_R0, MASK_Z,
					   i915_fs_operand_reg(FS_R3));

			/* Normalize the YUV data */
			i915_fs_add(FS_R0, i915_fs_operand_reg(FS_R0),
				    i915_fs_operand_reg(FS_C0));
			/* dot-product the YUV data in R0 by the vectors of
			 * coefficients for calculating R, G, and B, storing
			 * the results in the R, G, or B channels of the output
			 * color.  The OC results are implicitly clamped
			 * at the end of the program.
			 */
			i915_fs_dp3(FS_OC, MASK_X,
				    i915_fs_operand_reg(FS_R0),
				    i915_fs_operand_reg(FS_C1));
			i915_fs_dp3(FS_OC, MASK_Y,
				    i915_fs_operand_reg(FS_R0),
				    i915_fs_operand_reg(FS_C2));
			i915_fs_dp3(FS_OC, MASK_Z,
				    i915_fs_operand_reg(FS_R0),
				    i915_fs_operand_reg(FS_C3));
			/* Set alpha of the output to 1.0, by wiring W to 1
			 * and not actually using the source.
			 */
			i915_fs_mov_masked(FS_OC, MASK_W,
					   i915_fs_operand_one());

			if (adaptor_priv->brightness != 0) {
				i915_fs_add(FS_OC,
					    i915_fs_operand_reg(FS_OC),
					    i915_fs_operand(FS_C4, X, X, X,
							    ZERO));
			}
			FS_END();
		}

		OUT_BATCH(PRIM3D_RECTLIST | (12 * nbox_this_time - 1));
		while (nbox_this_time--) {
			int box_x1 = pbox->x1;
			int box_y1 = pbox->y1;
			int box_x2 = pbox->x2;
			int box_y2 = pbox->y2;
			float src_scale_x, src_scale_y;

			pbox++;

			src_scale_x = ((float)src_w / width) / drw_w;
			src_scale_y = ((float)src_h / height) / drw_h;

			/* vertex data - rect list consists of bottom right,
			 * bottom left, and top left vertices.
			 */

			/* bottom right */
			OUT_BATCH_F(box_x2 + pix_xoff);
			OUT_BATCH_F(box_y2 + pix_yoff);
			OUT_BATCH_F((box_x2 - dxo) * src_scale_x);
			OUT_BATCH_F((box_y2 - dyo) * src_scale_y);

			/* bottom left */
			OUT_BATCH_F(box_x1 + pix_xoff);
			OUT_BATCH_F(box_y2 + pix_yoff);
			OUT_BATCH_F((box_x1 - dxo) * src_scale_x);
			OUT_BATCH_F((box_y2 - dyo) * src_scale_y);

			/* top left */
			OUT_BATCH_F(box_x1 + pix_xoff);
			OUT_BATCH_F(box_y1 + pix_yoff);
			OUT_BATCH_F((box_x1 - dxo) * src_scale_x);
			OUT_BATCH_F((box_y1 - dyo) * src_scale_y);
		}

		intel_batch_end_atomic(scrn);
	}

	if (target != pixmap) {
		GCPtr gc;

		gc = GetScratchGC(pixmap->drawable.depth,
				  pixmap->drawable.pScreen);
		if (gc) {
			gc->subWindowMode = ClipByChildren;

			if (REGION_NUM_RECTS(dstRegion) > 1) {
				RegionPtr tmp;

				tmp = REGION_CREATE(pixmap->drawable.pScreen, NULL, 0);
				if (tmp) {
					REGION_COPY(pixmap->drawable.pScreen, tmp, dstRegion);
					gc->funcs->ChangeClip(gc, CT_REGION, tmp, 0);
				}
			}

			ValidateGC(&pixmap->drawable, gc);
			gc->ops->CopyArea(&target->drawable, &pixmap->drawable, gc,
					  0, 0,
					  target->drawable.width,
					  target->drawable.height,
					  -pix_xoff, -pix_yoff);
			FreeScratchGC(gc);
		}

		target->drawable.pScreen->DestroyPixmap(target);
	}

	intel_debug_flush(scrn);
}
