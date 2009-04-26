/*
 * Copyright 2008 Advanced Micro Devices, Inc.
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
 * Author: Alex Deucher <alexander.deucher@amd.com>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include "exa.h"

#include "rhd.h"
#include "rhd_cs.h"
#include "r6xx_accel.h"
#include "r600_shader.h"
#include "r600_reg.h"
#include "r600_state.h"

#include "rhd_video.h"

#include <X11/extensions/Xv.h>
#include "fourcc.h"

# ifdef DAMAGE
#  include "damage.h"
# endif

/* seriously ?! @#$%% */
# define uint32_t CARD32
# define uint64_t CARD64

static void
R600DoneTexturedVideo(ScrnInfoPtr pScrn)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct r6xx_accel_state *accel_state = rhdPtr->TwoDPrivate;
    draw_config_t   draw_conf;
    vtx_resource_t  vtx_res;

    CLEAR (draw_conf);
    CLEAR (vtx_res);

    if (accel_state->vb_index == 0) {
	R600IBDiscard(pScrn, accel_state->ib);
	return;
    }

    accel_state->vb_mc_addr = RHDDRIGetIntGARTLocation(pScrn) +
	(accel_state->ib->idx * accel_state->ib->total) + (accel_state->ib->total / 2);
    accel_state->vb_size = accel_state->vb_index * 16;

    /* flush vertex cache */
    if ((rhdPtr->ChipSet == RHD_RV610) ||
	(rhdPtr->ChipSet == RHD_RV620) ||
	(rhdPtr->ChipSet == RHD_M72) ||
	(rhdPtr->ChipSet == RHD_M74) ||
	(rhdPtr->ChipSet == RHD_M82) ||
	(rhdPtr->ChipSet == RHD_RS780) ||
	(rhdPtr->ChipSet == RHD_RV710))
	cp_set_surface_sync(pScrn, accel_state->ib, TC_ACTION_ENA_bit,
			    accel_state->vb_size, accel_state->vb_mc_addr);
    else
	cp_set_surface_sync(pScrn, accel_state->ib, VC_ACTION_ENA_bit,
			    accel_state->vb_size, accel_state->vb_mc_addr);

    /* Vertex buffer setup */
    vtx_res.id              = SQ_VTX_RESOURCE_vs;
    vtx_res.vtx_size_dw     = 16 / 4;
    vtx_res.vtx_num_entries = accel_state->vb_size / 4;
    vtx_res.mem_req_size    = 1;
    vtx_res.vb_addr         = accel_state->vb_mc_addr;
    set_vtx_resource        (pScrn, accel_state->ib, &vtx_res);

    draw_conf.prim_type          = DI_PT_RECTLIST;
    draw_conf.vgt_draw_initiator = DI_SRC_SEL_AUTO_INDEX;
    draw_conf.num_instances      = 1;
    draw_conf.num_indices        = vtx_res.vtx_num_entries / vtx_res.vtx_size_dw;
    draw_conf.index_type         = DI_INDEX_SIZE_16_BIT;

    draw_auto(pScrn, accel_state->ib, &draw_conf);

    wait_3d_idle_clean(pScrn, accel_state->ib);

    /* sync destination surface */
    cp_set_surface_sync(pScrn, accel_state->ib, (CB_ACTION_ENA_bit | CB0_DEST_BASE_ENA_bit),
			accel_state->dst_size, accel_state->dst_mc_addr);

    R600CPFlushIndirect(pScrn, accel_state->ib);
}

void
R600DisplayTexturedVideo(ScrnInfoPtr pScrn, struct RHDPortPriv *pPriv)
{
    RHDPtr rhdPtr = RHDPTR(pScrn);
    struct r6xx_accel_state *accel_state = rhdPtr->TwoDPrivate;
    PixmapPtr pPixmap = pPriv->pPixmap;
    BoxPtr pBox = REGION_RECTS(&pPriv->clip);
    int nBox = REGION_NUM_RECTS(&pPriv->clip);
    int dstxoff, dstyoff;
    cb_config_t     cb_conf;
    tex_resource_t  tex_res;
    tex_sampler_t   tex_samp;
    shader_config_t vs_conf, ps_conf;
    int uv_offset;

    static float ps_alu_consts[] = {
        1.0,  0.0,      1.4020,    0,  // r - c[0]
        1.0, -0.34414, -0.71414,  0,  // g - c[1]
        1.0,  1.7720,   0.0,        0,  // b - c[2]
	/* Constants for undoing Y'CbCr scaling
	 *  - Y' is scaled from 16:235
	 *  - Cb/Cr are scaled from 16:240
	 * Unscaled value N' = N * N_mul + N_shift (N' in range [-0.5, 0.5])
	 * Vector is [Y_mul, Y_shfit, C_mul, C_shift]
	 */
        256.0/219.0, -16.0/219.0, 256.0/224.0, -128.0/224.0,
    };

    CLEAR (cb_conf);
    CLEAR (tex_res);
    CLEAR (tex_samp);
    CLEAR (vs_conf);
    CLEAR (ps_conf);

    accel_state->dst_pitch = exaGetPixmapPitch(pPixmap) / (pPixmap->drawable.bitsPerPixel / 8);
    accel_state->src_pitch[0] = pPriv->BufferPitch;

    // bad pitch
    if (accel_state->src_pitch[0] & 7)
	return;
    if (accel_state->dst_pitch & 7)
	return;

#ifdef COMPOSITE
    dstxoff = -pPixmap->screen_x + pPixmap->drawable.x;
    dstyoff = -pPixmap->screen_y + pPixmap->drawable.y;
#else
    dstxoff = 0;
    dstyoff = 0;
#endif

    accel_state->ib = RHDDRMCPBuffer(pScrn->scrnIndex);

    /* Init */
    start_3d(pScrn, accel_state->ib);

    //cp_set_surface_sync(pScrn, accel_state->ib);

    set_default_state(pScrn, accel_state->ib);

    /* Scissor / viewport */
    EREG(accel_state->ib, PA_CL_VTE_CNTL,                      VTX_XY_FMT_bit);
    EREG(accel_state->ib, PA_CL_CLIP_CNTL,                     CLIP_DISABLE_bit);

    accel_state->vs_mc_addr = rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart + accel_state->shaders->offset +
	accel_state->xv_vs_offset;

    accel_state->ps_mc_addr = rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart + accel_state->shaders->offset +
	accel_state->xv_ps_offset;

    switch(pPriv->id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	set_bool_consts(pScrn, accel_state->ib, SQ_BOOL_CONST_ps, (1 << 0));
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	set_bool_consts(pScrn, accel_state->ib, SQ_BOOL_CONST_ps, (0 << 0));
	break;
    }

    accel_state->vs_size = 512;
    accel_state->ps_size = 512;

    /* Shader */

    /* flush SQ cache */
    cp_set_surface_sync(pScrn, accel_state->ib, SH_ACTION_ENA_bit,
			accel_state->vs_size, accel_state->vs_mc_addr);

    vs_conf.shader_addr         = accel_state->vs_mc_addr;
    vs_conf.num_gprs            = 2;
    vs_conf.stack_size          = 0;
    vs_setup                    (pScrn, accel_state->ib, &vs_conf);

    /* flush SQ cache */
    cp_set_surface_sync(pScrn, accel_state->ib, SH_ACTION_ENA_bit,
			accel_state->ps_size, accel_state->ps_mc_addr);

    ps_conf.shader_addr         = accel_state->ps_mc_addr;
    ps_conf.num_gprs            = 3;
    ps_conf.stack_size          = 1;
    ps_conf.uncached_first_inst = 1;
    ps_conf.clamp_consts        = 0;
    ps_conf.export_mode         = 2;
    ps_setup                    (pScrn, accel_state->ib, &ps_conf);

    // PS alu constants
    set_alu_consts(pScrn, accel_state->ib, 0, sizeof(ps_alu_consts) / SQ_ALU_CONSTANT_offset, ps_alu_consts);

    /* Texture */
    switch(pPriv->id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	accel_state->src_mc_addr[0] = pPriv->BufferOffset + rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart;
	accel_state->src_size[0] = accel_state->src_pitch[0] * pPriv->h;

	/* flush texture cache */
	cp_set_surface_sync(pScrn, accel_state->ib, TC_ACTION_ENA_bit, accel_state->src_size[0],
			    accel_state->src_mc_addr[0]);

	// Y texture
	tex_res.id                  = 0;
	tex_res.w                   = pPriv->w;
	tex_res.h                   = pPriv->h;
	tex_res.pitch               = accel_state->src_pitch[0];
	tex_res.depth               = 0;
	tex_res.dim                 = SQ_TEX_DIM_2D;
	tex_res.base                = accel_state->src_mc_addr[0];
	tex_res.mip_base            = accel_state->src_mc_addr[0];

	tex_res.format              = FMT_8;
	tex_res.dst_sel_x           = SQ_SEL_X; //Y
	tex_res.dst_sel_y           = SQ_SEL_1;
	tex_res.dst_sel_z           = SQ_SEL_1;
	tex_res.dst_sel_w           = SQ_SEL_1;

	tex_res.request_size        = 1;
	tex_res.base_level          = 0;
	tex_res.last_level          = 0;
	tex_res.perf_modulation     = 0;
	tex_res.interlaced          = 0;
	set_tex_resource            (pScrn, accel_state->ib, &tex_res);

	// Y sampler
	tex_samp.id                 = 0;
	tex_samp.clamp_x            = SQ_TEX_CLAMP_LAST_TEXEL;
	tex_samp.clamp_y            = SQ_TEX_CLAMP_LAST_TEXEL;
	tex_samp.clamp_z            = SQ_TEX_WRAP;


	// UV texture
	uv_offset = accel_state->src_pitch[0] * pPriv->h;
	uv_offset = (uv_offset + 255) & ~255;

	cp_set_surface_sync(pScrn, accel_state->ib, TC_ACTION_ENA_bit,
			    accel_state->src_size[0] / 4,
			    accel_state->src_mc_addr[0] + uv_offset);

	tex_res.id                  = 1;
	tex_res.format              = FMT_8;
	tex_res.w                   = pPriv->w >> 1;
	tex_res.h                   = pPriv->h >> 1;
	tex_res.pitch               = accel_state->src_pitch[0] >> 1;
	tex_res.dst_sel_x           = SQ_SEL_X; //V or U
	tex_res.dst_sel_y           = SQ_SEL_1;
	tex_res.dst_sel_z           = SQ_SEL_1;
	tex_res.dst_sel_w           = SQ_SEL_1;
	tex_res.interlaced          = 0;
	// XXX tex bases need to be 256B aligned
	tex_res.base                = accel_state->src_mc_addr[0] + uv_offset;
	tex_res.mip_base            = accel_state->src_mc_addr[0] + uv_offset;
	set_tex_resource            (pScrn, accel_state->ib, &tex_res);

	// xxx: switch to bicubic
	tex_samp.xy_mag_filter      = SQ_TEX_XY_FILTER_BILINEAR;
	tex_samp.xy_min_filter      = SQ_TEX_XY_FILTER_BILINEAR;

	tex_samp.z_filter           = SQ_TEX_Z_FILTER_NONE;
	tex_samp.mip_filter         = 0;			/* no mipmap */
	set_tex_sampler             (pScrn, accel_state->ib, &tex_samp);

	// UV sampler
	tex_samp.id                 = 1;
	set_tex_sampler             (pScrn, accel_state->ib, &tex_samp);

	// UV texture
	uv_offset += ((accel_state->src_pitch[0] >> 1) * (pPriv->h >> 1));
	uv_offset = (uv_offset + 255) & ~255;

	cp_set_surface_sync(pScrn, accel_state->ib, TC_ACTION_ENA_bit,
			    accel_state->src_size[0] / 4,
			    accel_state->src_mc_addr[0] + uv_offset);

	tex_res.id                  = 2;
	tex_res.format              = FMT_8;
	tex_res.w                   = pPriv->w >> 1;
	tex_res.h                   = pPriv->h >> 1;
	tex_res.pitch               = accel_state->src_pitch[0] >> 1;
	tex_res.dst_sel_x           = SQ_SEL_X; //V or U
	tex_res.dst_sel_y           = SQ_SEL_1;
	tex_res.dst_sel_z           = SQ_SEL_1;
	tex_res.dst_sel_w           = SQ_SEL_1;
	tex_res.interlaced          = 0;
	// XXX tex bases need to be 256B aligned
	tex_res.base                = accel_state->src_mc_addr[0] + uv_offset;
	tex_res.mip_base            = accel_state->src_mc_addr[0] + uv_offset;
	set_tex_resource            (pScrn, accel_state->ib, &tex_res);

	// xxx: switch to bicubic
	tex_samp.xy_mag_filter      = SQ_TEX_XY_FILTER_BILINEAR;
	tex_samp.xy_min_filter      = SQ_TEX_XY_FILTER_BILINEAR;

	tex_samp.z_filter           = SQ_TEX_Z_FILTER_NONE;
	tex_samp.mip_filter         = 0;			/* no mipmap */
	set_tex_sampler             (pScrn, accel_state->ib, &tex_samp);

	// UV sampler
	tex_samp.id                 = 2;
	set_tex_sampler             (pScrn, accel_state->ib, &tex_samp);
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	accel_state->src_mc_addr[0] = pPriv->BufferOffset + rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart;
	accel_state->src_size[0] = accel_state->src_pitch[0] * pPriv->h;

	/* flush texture cache */
	cp_set_surface_sync(pScrn, accel_state->ib, TC_ACTION_ENA_bit, accel_state->src_size[0],
			    accel_state->src_mc_addr[0]);

	// Y texture
	tex_res.id                  = 0;
	tex_res.w                   = pPriv->w;
	tex_res.h                   = pPriv->h;
	tex_res.pitch               = accel_state->src_pitch[0] >> 1;
	tex_res.depth               = 0;
	tex_res.dim                 = SQ_TEX_DIM_2D;
	tex_res.base                = accel_state->src_mc_addr[0];
	tex_res.mip_base            = accel_state->src_mc_addr[0];

	tex_res.format              = FMT_8_8;
	if (pPriv->id == FOURCC_UYVY)
	    tex_res.dst_sel_x           = SQ_SEL_Y; //Y
	else
	    tex_res.dst_sel_x           = SQ_SEL_X; //Y
	tex_res.dst_sel_y           = SQ_SEL_1;
	tex_res.dst_sel_z           = SQ_SEL_1;
	tex_res.dst_sel_w           = SQ_SEL_1;

	tex_res.request_size        = 1;
	tex_res.base_level          = 0;
	tex_res.last_level          = 0;
	tex_res.perf_modulation     = 0;
	tex_res.interlaced          = 0;
	set_tex_resource            (pScrn, accel_state->ib, &tex_res);

	// Y sampler
	tex_samp.id                 = 0;
	tex_samp.clamp_x            = SQ_TEX_CLAMP_LAST_TEXEL;
	tex_samp.clamp_y            = SQ_TEX_CLAMP_LAST_TEXEL;
	tex_samp.clamp_z            = SQ_TEX_WRAP;


	// UV texture
	tex_res.id                  = 1;
	tex_res.format              = FMT_8_8_8_8;
	tex_res.w                   = pPriv->w >> 1;
	tex_res.h                   = pPriv->h;
	tex_res.pitch               = accel_state->src_pitch[0] >> 2;
	if (pPriv->id == FOURCC_UYVY) {
	    tex_res.dst_sel_x = SQ_SEL_X; //V
	    tex_res.dst_sel_y = SQ_SEL_Z; //U
	} else {
	    tex_res.dst_sel_x = SQ_SEL_Y; //V
	    tex_res.dst_sel_y = SQ_SEL_W; //U
	}
	tex_res.dst_sel_z           = SQ_SEL_1;
	tex_res.dst_sel_w           = SQ_SEL_1;
	tex_res.interlaced          = 0;
	// XXX tex bases need to be 256B aligned
	tex_res.base                = accel_state->src_mc_addr[0];
	tex_res.mip_base            = accel_state->src_mc_addr[0];
	set_tex_resource            (pScrn, accel_state->ib, &tex_res);

	// xxx: switch to bicubic
	tex_samp.xy_mag_filter      = SQ_TEX_XY_FILTER_BILINEAR;
	tex_samp.xy_min_filter      = SQ_TEX_XY_FILTER_BILINEAR;

	tex_samp.z_filter           = SQ_TEX_Z_FILTER_NONE;
	tex_samp.mip_filter         = 0;			/* no mipmap */
	set_tex_sampler             (pScrn, accel_state->ib, &tex_samp);

	// UV sampler
	tex_samp.id                 = 1;
	set_tex_sampler             (pScrn, accel_state->ib, &tex_samp);
	break;
    }

    /* Render setup */
    EREG(accel_state->ib, CB_SHADER_MASK,                      (0x0f << OUTPUT0_ENABLE_shift));
    EREG(accel_state->ib, R7xx_CB_SHADER_CONTROL,              (RT0_ENABLE_bit));
    EREG(accel_state->ib, CB_COLOR_CONTROL,                    (0xcc << ROP3_shift)); /* copy */

    cb_conf.id = 0;

    accel_state->dst_mc_addr = exaGetPixmapOffset(pPixmap) + rhdPtr->FbIntAddress + rhdPtr->FbScanoutStart;

    cb_conf.w = accel_state->dst_pitch;
    cb_conf.h = pPixmap->drawable.height;
    cb_conf.base = accel_state->dst_mc_addr;

    switch (pPixmap->drawable.bitsPerPixel) {
    case 16:
	if (pPixmap->drawable.depth == 15) {
	    cb_conf.format = COLOR_1_5_5_5;
	    cb_conf.comp_swap = 1; //ARGB
	} else {
	    cb_conf.format = COLOR_5_6_5;
	    cb_conf.comp_swap = 2; //RGB
	}
	break;
    case 32:
	cb_conf.format = COLOR_8_8_8_8;
	cb_conf.comp_swap = 1; //ARGB
	break;
    default:
	return;
    }

    cb_conf.source_format = 1;
    cb_conf.blend_clamp = 1;
    set_render_target(pScrn, accel_state->ib, &cb_conf);

    EREG(accel_state->ib, PA_SU_SC_MODE_CNTL,                  (FACE_bit			|
								(POLYMODE_PTYPE__TRIANGLES << POLYMODE_FRONT_PTYPE_shift)	|
								(POLYMODE_PTYPE__TRIANGLES << POLYMODE_BACK_PTYPE_shift)));
    EREG(accel_state->ib, DB_SHADER_CONTROL,                   ((1 << Z_ORDER_shift)		| /* EARLY_Z_THEN_LATE_Z */
								DUAL_EXPORT_ENABLE_bit)); /* Only useful if no depth export */

    /* Interpolator setup */
    // export tex coords from VS
    EREG(accel_state->ib, SPI_VS_OUT_CONFIG, ((1 - 1) << VS_EXPORT_COUNT_shift));
    EREG(accel_state->ib, SPI_VS_OUT_ID_0, (0 << SEMANTIC_0_shift));

    /* Enabling flat shading needs both FLAT_SHADE_bit in SPI_PS_INPUT_CNTL_x
     * *and* FLAT_SHADE_ENA_bit in SPI_INTERP_CONTROL_0 */
    EREG(accel_state->ib, SPI_PS_IN_CONTROL_0,                 ((1 << NUM_INTERP_shift)));
    EREG(accel_state->ib, SPI_PS_IN_CONTROL_1,                 0);
    EREG(accel_state->ib, SPI_PS_INPUT_CNTL_0 + (0 <<2),       ((0    << SEMANTIC_shift)	|
								(0x03 << DEFAULT_VAL_shift)	|
								SEL_CENTROID_bit));
    EREG(accel_state->ib, SPI_INTERP_CONTROL_0,                0);

    if (exaGetPixmapOffset(pPixmap) == 0)
	wait_vline_range(
	    pScrn,
	    accel_state->ib,
	    0 /* TODO */,
	    dstyoff + pPriv->drw_y,
	    dstyoff + pPriv->drw_y + pPriv->dst_h - 1
	    );

    accel_state->vb_index = 0;

    while (nBox--) {
	int srcX, srcY, srcw, srch;
	int dstX, dstY, dstw, dsth;
	float *vb;

	if (((accel_state->vb_index + 3) * 16) > (accel_state->ib->total / 2)) {
	    R600DoneTexturedVideo(pScrn);
	    accel_state->vb_index = 0;
	    accel_state->ib = RHDDRMCPBuffer(pScrn->scrnIndex);
	}

	vb = (pointer)((char*)accel_state->ib->address +
		       (accel_state->ib->total / 2) +
		       accel_state->vb_index * 16);

	dstX = pBox->x1 + dstxoff;
	dstY = pBox->y1 + dstyoff;
	dstw = pBox->x2 - pBox->x1;
	dsth = pBox->y2 - pBox->y1;

	srcX = ((pBox->x1 - pPriv->drw_x) *
		pPriv->src_w) / pPriv->dst_w;
	srcY = ((pBox->y1 - pPriv->drw_y) *
		pPriv->src_h) / pPriv->dst_h;

	srcw = (pPriv->src_w * dstw) / pPriv->dst_w;
	srch = (pPriv->src_h * dsth) / pPriv->dst_h;

	vb[0] = (float)dstX;
	vb[1] = (float)dstY;
	vb[2] = (float)srcX / pPriv->w;
	vb[3] = (float)srcY / pPriv->h;

	vb[4] = (float)dstX;
	vb[5] = (float)(dstY + dsth);
	vb[6] = (float)srcX / pPriv->w;
	vb[7] = (float)(srcY + srch) / pPriv->h;

	vb[8] = (float)(dstX + dstw);
	vb[9] = (float)(dstY + dsth);
	vb[10] = (float)(srcX + srcw) / pPriv->w;
	vb[11] = (float)(srcY + srch) / pPriv->h;

	accel_state->vb_index += 3;

	pBox++;
    }

    R600DoneTexturedVideo(pScrn);

    DamageDamageRegion(pPriv->pDraw, &pPriv->clip);
}
