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

#include "radeon.h"
#include "radeon_macros.h"
#include "radeon_reg.h"
#include "r600_shader.h"
#include "r600_reg.h"
#include "r600_state.h"

extern PixmapPtr
RADEONGetDrawablePixmap(DrawablePtr pDrawable);

/* #define SHOW_VERTEXES */

#       define RADEON_ROP3_ZERO             0x00000000
#       define RADEON_ROP3_DSa              0x00880000
#       define RADEON_ROP3_SDna             0x00440000
#       define RADEON_ROP3_S                0x00cc0000
#       define RADEON_ROP3_DSna             0x00220000
#       define RADEON_ROP3_D                0x00aa0000
#       define RADEON_ROP3_DSx              0x00660000
#       define RADEON_ROP3_DSo              0x00ee0000
#       define RADEON_ROP3_DSon             0x00110000
#       define RADEON_ROP3_DSxn             0x00990000
#       define RADEON_ROP3_Dn               0x00550000
#       define RADEON_ROP3_SDno             0x00dd0000
#       define RADEON_ROP3_Sn               0x00330000
#       define RADEON_ROP3_DSno             0x00bb0000
#       define RADEON_ROP3_DSan             0x00770000
#       define RADEON_ROP3_ONE              0x00ff0000

uint32_t RADEON_ROP[16] = {
    RADEON_ROP3_ZERO, /* GXclear        */
    RADEON_ROP3_DSa,  /* Gxand          */
    RADEON_ROP3_SDna, /* GXandReverse   */
    RADEON_ROP3_S,    /* GXcopy         */
    RADEON_ROP3_DSna, /* GXandInverted  */
    RADEON_ROP3_D,    /* GXnoop         */
    RADEON_ROP3_DSx,  /* GXxor          */
    RADEON_ROP3_DSo,  /* GXor           */
    RADEON_ROP3_DSon, /* GXnor          */
    RADEON_ROP3_DSxn, /* GXequiv        */
    RADEON_ROP3_Dn,   /* GXinvert       */
    RADEON_ROP3_SDno, /* GXorReverse    */
    RADEON_ROP3_Sn,   /* GXcopyInverted */
    RADEON_ROP3_DSno, /* GXorInverted   */
    RADEON_ROP3_DSan, /* GXnand         */
    RADEON_ROP3_ONE,  /* GXset          */
};

static void
R600DoneSolid(PixmapPtr pPix);

static void
R600DoneComposite(PixmapPtr pDst);


static Bool
R600PrepareSolid(PixmapPtr pPix, int alu, Pixel pm, Pixel fg)
{
    ScrnInfoPtr pScrn = xf86Screens[pPix->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    cb_config_t     cb_conf;
    shader_config_t vs_conf, ps_conf;
    int pmask = 0;
    uint32_t a, r, g, b;
    float ps_alu_consts[4];

    accel_state->dst_mc_addr = exaGetPixmapOffset(pPix) + info->fbLocation + pScrn->fbOffset;
    accel_state->dst_size = exaGetPixmapPitch(pPix) * pPix->drawable.height;
    accel_state->dst_pitch = exaGetPixmapPitch(pPix) / (pPix->drawable.bitsPerPixel / 8);

    /* bad pitch */
    if (accel_state->dst_pitch & 7)
	return FALSE;

    /* bad offset */
    if (accel_state->dst_mc_addr & 0xff)
	return FALSE;

    if (pPix->drawable.bitsPerPixel == 24)
	return FALSE;

    CLEAR (cb_conf);
    CLEAR (vs_conf);
    CLEAR (ps_conf);

    /* return FALSE; */

#ifdef SHOW_VERTEXES
    ErrorF("%dx%d @ %dbpp, 0x%08x\n", pPix->drawable.width, pPix->drawable.height,
	   pPix->drawable.bitsPerPixel, exaGetPixmapPitch(pPix));
#endif

    accel_state->ib = RADEONCPGetBuffer(pScrn);

    /* Init */
    start_3d(pScrn, accel_state->ib);

    set_default_state(pScrn, accel_state->ib);

    /* Scissor / viewport */
    EREG(accel_state->ib, PA_CL_VTE_CNTL,                      VTX_XY_FMT_bit);
    EREG(accel_state->ib, PA_CL_CLIP_CNTL,                     CLIP_DISABLE_bit);

    accel_state->vs_mc_addr = info->fbLocation + pScrn->fbOffset + accel_state->shaders->offset +
	accel_state->solid_vs_offset;
    accel_state->ps_mc_addr = info->fbLocation + pScrn->fbOffset + accel_state->shaders->offset +
	accel_state->solid_ps_offset;
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
    ps_conf.num_gprs            = 1;
    ps_conf.stack_size          = 0;
    ps_conf.uncached_first_inst = 1;
    ps_conf.clamp_consts        = 0;
    ps_conf.export_mode         = 2;
    ps_setup                    (pScrn, accel_state->ib, &ps_conf);

    /* Render setup */
    if (pm & 0x000000ff)
	pmask |= 4; /* B */
    if (pm & 0x0000ff00)
	pmask |= 2; /* G */
    if (pm & 0x00ff0000)
	pmask |= 1; /* R */
    if (pm & 0xff000000)
	pmask |= 8; /* A */
    EREG(accel_state->ib, CB_SHADER_MASK,                      (pmask << OUTPUT0_ENABLE_shift));
    EREG(accel_state->ib, R7xx_CB_SHADER_CONTROL,              (RT0_ENABLE_bit));
    EREG(accel_state->ib, CB_COLOR_CONTROL,                    RADEON_ROP[alu]);

    cb_conf.id = 0;
    cb_conf.w = accel_state->dst_pitch;
    cb_conf.h = pPix->drawable.height;
    cb_conf.base = accel_state->dst_mc_addr;

    if (pPix->drawable.bitsPerPixel == 8) {
	cb_conf.format = COLOR_8;
	cb_conf.comp_swap = 3; /* A */
    } else if (pPix->drawable.bitsPerPixel == 16) {
	cb_conf.format = COLOR_5_6_5;
	cb_conf.comp_swap = 2; /* RGB */
    } else {
	cb_conf.format = COLOR_8_8_8_8;
	cb_conf.comp_swap = 1; /* ARGB */
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
    /* one unused export from VS (VS_EXPORT_COUNT is zero based, count minus one) */
    EREG(accel_state->ib, SPI_VS_OUT_CONFIG, (0 << VS_EXPORT_COUNT_shift));
    EREG(accel_state->ib, SPI_VS_OUT_ID_0, (0 << SEMANTIC_0_shift));

    /* Enabling flat shading needs both FLAT_SHADE_bit in SPI_PS_INPUT_CNTL_x
     * *and* FLAT_SHADE_ENA_bit in SPI_INTERP_CONTROL_0 */
    /* no VS exports as PS input (NUM_INTERP is not zero based, no minus one) */
    EREG(accel_state->ib, SPI_PS_IN_CONTROL_0,                 (0 << NUM_INTERP_shift));
    EREG(accel_state->ib, SPI_PS_IN_CONTROL_1,                 0);
    /* color semantic id 0 -> GPR[0] */
    EREG(accel_state->ib, SPI_PS_INPUT_CNTL_0 + (0 <<2),       ((0    << SEMANTIC_shift)	|
								  (0x03 << DEFAULT_VAL_shift)	|
								  FLAT_SHADE_bit		|
								  SEL_CENTROID_bit));
    EREG(accel_state->ib, SPI_INTERP_CONTROL_0,                FLAT_SHADE_ENA_bit | 0);

    /* PS alu constants */
    if (pPix->drawable.bitsPerPixel == 16) {
	r = (fg >> 11) & 0x1f;
	g = (fg >> 5) & 0x3f;
	b = (fg >> 0) & 0x1f;
	ps_alu_consts[0] = (float)r / 31; /* R */
	ps_alu_consts[1] = (float)g / 63; /* G */
	ps_alu_consts[2] = (float)b / 31; /* B */
	ps_alu_consts[3] = 1.0; /* A */
    } else if (pPix->drawable.bitsPerPixel == 8) {
	a = (fg >> 0) & 0xff;
	ps_alu_consts[0] = 0.0; /* R */
	ps_alu_consts[1] = 0.0; /* G */
	ps_alu_consts[2] = 0.0; /* B */
	ps_alu_consts[3] = (float)a / 255; /* A */
    } else {
	a = (fg >> 24) & 0xff;
	r = (fg >> 16) & 0xff;
	g = (fg >> 8) & 0xff;
	b = (fg >> 0) & 0xff;
	ps_alu_consts[0] = (float)r / 255; /* R */
	ps_alu_consts[1] = (float)g / 255; /* G */
	ps_alu_consts[2] = (float)b / 255; /* B */
	ps_alu_consts[3] = (float)a / 255; /* A */
    }
    set_alu_consts(pScrn, accel_state->ib, SQ_ALU_CONSTANT_ps,
		   sizeof(ps_alu_consts) / SQ_ALU_CONSTANT_offset, ps_alu_consts);

    accel_state->vb_index = 0;

#ifdef SHOW_VERTEXES
    ErrorF("PM: 0x%08x\n", pm);
#endif

    return TRUE;
}


static void
R600Solid(PixmapPtr pPix, int x1, int y1, int x2, int y2)
{
    ScrnInfoPtr pScrn = xf86Screens[pPix->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    float *vb;

    if (((accel_state->vb_index + 3) * 8) > (accel_state->ib->total / 2)) {
	R600DoneSolid(pPix);
	accel_state->vb_index = 0;
	accel_state->ib = RADEONCPGetBuffer(pScrn);
    }

    vb = (pointer)((char*)accel_state->ib->address +
		   (accel_state->ib->total / 2) +
		   accel_state->vb_index * 8);

    vb[0] = (float)x1;
    vb[1] = (float)y1;

    vb[2] = (float)x1;
    vb[3] = (float)y2;

    vb[4] = (float)x2;
    vb[5] = (float)y2;

    accel_state->vb_index += 3;

}

static void
R600DoneSolid(PixmapPtr pPix)
{
    ScrnInfoPtr pScrn = xf86Screens[pPix->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    draw_config_t   draw_conf;
    vtx_resource_t  vtx_res;

    CLEAR (draw_conf);
    CLEAR (vtx_res);

    if (accel_state->vb_index == 0) {
	R600IBDiscard(pScrn, accel_state->ib);
	return;
    }

    accel_state->vb_mc_addr = info->gartLocation + info->dri->bufStart +
	(accel_state->ib->idx * accel_state->ib->total) + (accel_state->ib->total / 2);
    accel_state->vb_size = accel_state->vb_index * 8;

    /* flush vertex cache */
    if ((info->ChipFamily == CHIP_FAMILY_RV610) ||
	(info->ChipFamily == CHIP_FAMILY_RV620) ||
	(info->ChipFamily == CHIP_FAMILY_RS780) ||
	(info->ChipFamily == CHIP_FAMILY_RV710))
	cp_set_surface_sync(pScrn, accel_state->ib, TC_ACTION_ENA_bit,
			    accel_state->vb_size, accel_state->vb_mc_addr);
    else
	cp_set_surface_sync(pScrn, accel_state->ib, VC_ACTION_ENA_bit,
			    accel_state->vb_size, accel_state->vb_mc_addr);

    /* Vertex buffer setup */
    vtx_res.id              = SQ_VTX_RESOURCE_vs;
    vtx_res.vtx_size_dw     = 8 / 4;
    vtx_res.vtx_num_entries = accel_state->vb_size / 4;
    vtx_res.mem_req_size    = 1;
    vtx_res.vb_addr         = accel_state->vb_mc_addr;
    set_vtx_resource        (pScrn, accel_state->ib, &vtx_res);

    /* Draw */
    draw_conf.prim_type          = DI_PT_RECTLIST;
    draw_conf.vgt_draw_initiator = DI_SRC_SEL_AUTO_INDEX;
    draw_conf.num_instances      = 1;
    draw_conf.num_indices        = vtx_res.vtx_num_entries / vtx_res.vtx_size_dw;
    draw_conf.index_type         = DI_INDEX_SIZE_16_BIT;

    draw_auto(pScrn, accel_state->ib, &draw_conf);

    wait_3d_idle_clean(pScrn, accel_state->ib);

    /* sync dst surface */
    cp_set_surface_sync(pScrn, accel_state->ib, (CB_ACTION_ENA_bit | CB0_DEST_BASE_ENA_bit),
			accel_state->dst_size, accel_state->dst_mc_addr);

    R600CPFlushIndirect(pScrn, accel_state->ib);
}

static void
R600DoPrepareCopy(ScrnInfoPtr pScrn,
		  int src_pitch, int src_width, int src_height, uint32_t src_offset, int src_bpp,
		  int dst_pitch, int dst_height, uint32_t dst_offset, int dst_bpp,
		  int rop, Pixel planemask)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    int pmask = 0;
    cb_config_t     cb_conf;
    tex_resource_t  tex_res;
    tex_sampler_t   tex_samp;
    shader_config_t vs_conf, ps_conf;

    CLEAR (cb_conf);
    CLEAR (tex_res);
    CLEAR (tex_samp);
    CLEAR (vs_conf);
    CLEAR (ps_conf);

    accel_state->ib = RADEONCPGetBuffer(pScrn);

    /* Init */
    start_3d(pScrn, accel_state->ib);

    set_default_state(pScrn, accel_state->ib);

    /* Scissor / viewport */
    EREG(accel_state->ib, PA_CL_VTE_CNTL,                      VTX_XY_FMT_bit);
    EREG(accel_state->ib, PA_CL_CLIP_CNTL,                     CLIP_DISABLE_bit);

    accel_state->vs_mc_addr = info->fbLocation + pScrn->fbOffset + accel_state->shaders->offset +
	accel_state->copy_vs_offset;
    accel_state->ps_mc_addr = info->fbLocation + pScrn->fbOffset + accel_state->shaders->offset +
	accel_state->copy_ps_offset;
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
    ps_conf.num_gprs            = 1;
    ps_conf.stack_size          = 0;
    ps_conf.uncached_first_inst = 1;
    ps_conf.clamp_consts        = 0;
    ps_conf.export_mode         = 2;
    ps_setup                    (pScrn, accel_state->ib, &ps_conf);

    accel_state->src_size[0] = src_pitch * src_height * (src_bpp/8);
    accel_state->src_mc_addr[0] = src_offset;
    accel_state->src_pitch[0] = src_pitch;
    accel_state->src_width[0] = src_width;
    accel_state->src_height[0] = src_height;
    accel_state->src_bpp[0] = src_bpp;

    /* flush texture cache */
    cp_set_surface_sync(pScrn, accel_state->ib, TC_ACTION_ENA_bit,
			accel_state->src_size[0], accel_state->src_mc_addr[0]);

    /* Texture */
    tex_res.id                  = 0;
    tex_res.w                   = src_width;
    tex_res.h                   = src_height;
    tex_res.pitch               = accel_state->src_pitch[0];
    tex_res.depth               = 0;
    tex_res.dim                 = SQ_TEX_DIM_2D;
    tex_res.base                = accel_state->src_mc_addr[0];
    tex_res.mip_base            = accel_state->src_mc_addr[0];
    if (src_bpp == 8) {
	tex_res.format              = FMT_8;
	tex_res.dst_sel_x           = SQ_SEL_1; /* R */
	tex_res.dst_sel_y           = SQ_SEL_1; /* G */
	tex_res.dst_sel_z           = SQ_SEL_1; /* B */
	tex_res.dst_sel_w           = SQ_SEL_X; /* A */
    } else if (src_bpp == 16) {
	tex_res.format              = FMT_5_6_5;
	tex_res.dst_sel_x           = SQ_SEL_Z; /* R */
	tex_res.dst_sel_y           = SQ_SEL_Y; /* G */
	tex_res.dst_sel_z           = SQ_SEL_X; /* B */
	tex_res.dst_sel_w           = SQ_SEL_1; /* A */
    } else {
	tex_res.format              = FMT_8_8_8_8;
	tex_res.dst_sel_x           = SQ_SEL_Z; /* R */
	tex_res.dst_sel_y           = SQ_SEL_Y; /* G */
	tex_res.dst_sel_z           = SQ_SEL_X; /* B */
	tex_res.dst_sel_w           = SQ_SEL_W; /* A */
    }

    tex_res.request_size        = 1;
    tex_res.base_level          = 0;
    tex_res.last_level          = 0;
    tex_res.perf_modulation     = 0;
    set_tex_resource            (pScrn, accel_state->ib, &tex_res);

    tex_samp.id                 = 0;
    tex_samp.clamp_x            = SQ_TEX_CLAMP_LAST_TEXEL;
    tex_samp.clamp_y            = SQ_TEX_CLAMP_LAST_TEXEL;
    tex_samp.clamp_z            = SQ_TEX_WRAP;
    tex_samp.xy_mag_filter      = SQ_TEX_XY_FILTER_POINT;
    tex_samp.xy_min_filter      = SQ_TEX_XY_FILTER_POINT;
    tex_samp.z_filter           = SQ_TEX_Z_FILTER_NONE;
    tex_samp.mip_filter         = 0;			/* no mipmap */
    set_tex_sampler             (pScrn, accel_state->ib, &tex_samp);


    /* Render setup */
    if (planemask & 0x000000ff)
	pmask |= 4; /* B */
    if (planemask & 0x0000ff00)
	pmask |= 2; /* G */
    if (planemask & 0x00ff0000)
	pmask |= 1; /* R */
    if (planemask & 0xff000000)
	pmask |= 8; /* A */
    EREG(accel_state->ib, CB_SHADER_MASK,                      (pmask << OUTPUT0_ENABLE_shift));
    EREG(accel_state->ib, R7xx_CB_SHADER_CONTROL,              (RT0_ENABLE_bit));
    EREG(accel_state->ib, CB_COLOR_CONTROL,                    RADEON_ROP[rop]);

    accel_state->dst_size = dst_pitch * dst_height * (dst_bpp/8);
    accel_state->dst_mc_addr = dst_offset;
    accel_state->dst_pitch = dst_pitch;
    accel_state->dst_height = dst_height;
    accel_state->dst_bpp = dst_bpp;

    cb_conf.id = 0;
    cb_conf.w = accel_state->dst_pitch;
    cb_conf.h = dst_height;
    cb_conf.base = accel_state->dst_mc_addr;
    if (dst_bpp == 8) {
	cb_conf.format = COLOR_8;
	cb_conf.comp_swap = 3; /* A */
    } else if (dst_bpp == 16) {
	cb_conf.format = COLOR_5_6_5;
	cb_conf.comp_swap = 2; /* RGB */
    } else {
	cb_conf.format = COLOR_8_8_8_8;
	cb_conf.comp_swap = 1; /* ARGB */
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
    /* export tex coord from VS */
    EREG(accel_state->ib, SPI_VS_OUT_CONFIG, ((1 - 1) << VS_EXPORT_COUNT_shift));
    EREG(accel_state->ib, SPI_VS_OUT_ID_0, (0 << SEMANTIC_0_shift));

    /* Enabling flat shading needs both FLAT_SHADE_bit in SPI_PS_INPUT_CNTL_x
     * *and* FLAT_SHADE_ENA_bit in SPI_INTERP_CONTROL_0 */
    /* input tex coord from VS */
    EREG(accel_state->ib, SPI_PS_IN_CONTROL_0,                 ((1 << NUM_INTERP_shift)));
    EREG(accel_state->ib, SPI_PS_IN_CONTROL_1,                 0);
    /* color semantic id 0 -> GPR[0] */
    EREG(accel_state->ib, SPI_PS_INPUT_CNTL_0 + (0 <<2),       ((0    << SEMANTIC_shift)	|
								(0x01 << DEFAULT_VAL_shift)	|
								SEL_CENTROID_bit));
    EREG(accel_state->ib, SPI_INTERP_CONTROL_0,                0);

    accel_state->vb_index = 0;

}

static void
R600DoCopy(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    draw_config_t   draw_conf;
    vtx_resource_t  vtx_res;

    CLEAR (draw_conf);
    CLEAR (vtx_res);

    if (accel_state->vb_index == 0) {
	R600IBDiscard(pScrn, accel_state->ib);
	return;
    }

    accel_state->vb_mc_addr = info->gartLocation + info->dri->bufStart +
	(accel_state->ib->idx * accel_state->ib->total) + (accel_state->ib->total / 2);
    accel_state->vb_size = accel_state->vb_index * 16;

    /* flush vertex cache */
    if ((info->ChipFamily == CHIP_FAMILY_RV610) ||
	(info->ChipFamily == CHIP_FAMILY_RV620) ||
	(info->ChipFamily == CHIP_FAMILY_RS780) ||
	(info->ChipFamily == CHIP_FAMILY_RV710))
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

    /* sync dst surface */
    cp_set_surface_sync(pScrn, accel_state->ib, (CB_ACTION_ENA_bit | CB0_DEST_BASE_ENA_bit),
			accel_state->dst_size, accel_state->dst_mc_addr);

    R600CPFlushIndirect(pScrn, accel_state->ib);
}

static void
R600AppendCopyVertex(ScrnInfoPtr pScrn,
		     int srcX, int srcY,
		     int dstX, int dstY,
		     int w, int h)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    float *vb;

    if (((accel_state->vb_index + 3) * 16) > (accel_state->ib->total / 2)) {
	R600DoCopy(pScrn);
	accel_state->vb_index = 0;
	accel_state->ib = RADEONCPGetBuffer(pScrn);
    }

    vb = (pointer)((char*)accel_state->ib->address +
		   (accel_state->ib->total / 2) +
		   accel_state->vb_index * 16);

    vb[0] = (float)dstX;
    vb[1] = (float)dstY;
    vb[2] = (float)srcX;
    vb[3] = (float)srcY;

    vb[4] = (float)dstX;
    vb[5] = (float)(dstY + h);
    vb[6] = (float)srcX;
    vb[7] = (float)(srcY + h);

    vb[8] = (float)(dstX + w);
    vb[9] = (float)(dstY + h);
    vb[10] = (float)(srcX + w);
    vb[11] = (float)(srcY + h);

    accel_state->vb_index += 3;
}

static Bool
R600PrepareCopy(PixmapPtr pSrc,   PixmapPtr pDst,
		int xdir, int ydir,
		int rop,
		Pixel planemask)
{
    ScrnInfoPtr pScrn = xf86Screens[pDst->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    accel_state->dst_pitch = exaGetPixmapPitch(pDst) / (pDst->drawable.bitsPerPixel / 8);
    accel_state->src_pitch[0] = exaGetPixmapPitch(pSrc) / (pSrc->drawable.bitsPerPixel / 8);

    accel_state->src_mc_addr[0] = exaGetPixmapOffset(pSrc) + info->fbLocation + pScrn->fbOffset;
    accel_state->dst_mc_addr = exaGetPixmapOffset(pDst) + info->fbLocation + pScrn->fbOffset;

    accel_state->src_width[0] = pSrc->drawable.width;
    accel_state->src_height[0] = pSrc->drawable.height;
    accel_state->src_bpp[0] = pSrc->drawable.bitsPerPixel;
    accel_state->dst_height = pDst->drawable.height;
    accel_state->dst_bpp = pDst->drawable.bitsPerPixel;

    /* bad pitch */
    if (accel_state->src_pitch[0] & 7)
	return FALSE;
    if (accel_state->dst_pitch & 7)
	return FALSE;

    /* bad offset */
    if (accel_state->src_mc_addr[0] & 0xff)
	return FALSE;
    if (accel_state->dst_mc_addr & 0xff)
	return FALSE;

    if (pSrc->drawable.bitsPerPixel == 24)
	return FALSE;
    if (pDst->drawable.bitsPerPixel == 24)
	return FALSE;

    /* return FALSE; */

#ifdef SHOW_VERTEXES
    ErrorF("src: %dx%d @ %dbpp, 0x%08x\n", pSrc->drawable.width, pSrc->drawable.height,
	   pSrc->drawable.bitsPerPixel, exaGetPixmapPitch(pSrc));
    ErrorF("dst: %dx%d @ %dbpp, 0x%08x\n", pDst->drawable.width, pDst->drawable.height,
	   pDst->drawable.bitsPerPixel, exaGetPixmapPitch(pDst));
#endif

    accel_state->rop = rop;
    accel_state->planemask = planemask;

    if (exaGetPixmapOffset(pSrc) == exaGetPixmapOffset(pDst)) {
	unsigned long size = pDst->drawable.height * accel_state->dst_pitch * pDst->drawable.bitsPerPixel/8;
	accel_state->same_surface = TRUE;

	if (accel_state->copy_area) {
	    exaOffscreenFree(pDst->drawable.pScreen, accel_state->copy_area);
	    accel_state->copy_area = NULL;
	}
	accel_state->copy_area = exaOffscreenAlloc(pDst->drawable.pScreen, size, 256, TRUE, NULL, NULL);
    } else {
	accel_state->same_surface = FALSE;

	R600DoPrepareCopy(pScrn,
			  accel_state->src_pitch[0], pSrc->drawable.width, pSrc->drawable.height,
			  accel_state->src_mc_addr[0], pSrc->drawable.bitsPerPixel,
			  accel_state->dst_pitch, pDst->drawable.height,
			  accel_state->dst_mc_addr, pDst->drawable.bitsPerPixel,
			  rop, planemask);

    }

    return TRUE;
}

static Bool
is_overlap(int sx1, int sx2, int sy1, int sy2, int dx1, int dx2, int dy1, int dy2)
{
    if (((sx1 >= dx1) && (sx1 <= dx2) && (sy1 >= dy1) && (sy1 <= dy2)) || /* TL x1, y1 */
	((sx2 >= dx1) && (sx2 <= dx2) && (sy1 >= dy1) && (sy1 <= dy2)) || /* TR x2, y1 */
	((sx1 >= dx1) && (sx1 <= dx2) && (sy2 >= dy1) && (sy2 <= dy2)) || /* BL x1, y2 */
	((sx2 >= dx1) && (sx2 <= dx2) && (sy2 >= dy1) && (sy2 <= dy2)))   /* BR x2, y2 */
	return TRUE;
    else
	return FALSE;
}

static void
R600OverlapCopy(PixmapPtr pDst,
		int srcX, int srcY,
		int dstX, int dstY,
		int w, int h)
{
    ScrnInfoPtr pScrn = xf86Screens[pDst->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    uint32_t dst_pitch = exaGetPixmapPitch(pDst) / (pDst->drawable.bitsPerPixel / 8);
    uint32_t dst_offset = exaGetPixmapOffset(pDst) + info->fbLocation + pScrn->fbOffset;
    int i, hchunk, vchunk;

    if (is_overlap(srcX, srcX + w, srcY, srcY + h,
		   dstX, dstX + w, dstY, dstY + h)) {
        /* Calculate height/width of non-overlapping area */
        hchunk = (srcX < dstX) ? (dstX - srcX) : (srcX - dstX);
        vchunk = (srcY < dstY) ? (dstY - srcY) : (srcY - dstY);

        /* Diagonally offset overlap is reduced to either horizontal or vertical offset-only
         * by copying a part of the  non-overlapping portion, then adjusting coordinates
         * Choose horizontal vs vertical to minimize the total number of copy operations
         */
        if (vchunk != 0 && hchunk != 0) { /* diagonal */
            if ((w / hchunk) <= (h / vchunk)) { /* reduce to horizontal  */
                if (srcY > dstY ) { /* diagonal up */
                    R600DoPrepareCopy(pScrn,
                                      dst_pitch, pDst->drawable.width, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      dst_pitch, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      accel_state->rop, accel_state->planemask);
                    R600AppendCopyVertex(pScrn, srcX, srcY, dstX, dstY, w, vchunk);
                    R600DoCopy(pScrn);

                    srcY = srcY + vchunk;
                    dstY = dstY + vchunk;
                } else { /* diagonal down */
                    R600DoPrepareCopy(pScrn,
                                      dst_pitch, pDst->drawable.width, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      dst_pitch, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      accel_state->rop, accel_state->planemask);
                    R600AppendCopyVertex(pScrn, srcX, srcY + h - vchunk, dstX, dstY + h - vchunk, w, vchunk);
                    R600DoCopy(pScrn);
                }
                h = h - vchunk;
                vchunk = 0;
            } else { /* reduce to vertical */
                if (srcX > dstX ) { /* diagonal left */
                    R600DoPrepareCopy(pScrn,
                                      dst_pitch, pDst->drawable.width, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      dst_pitch, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      accel_state->rop, accel_state->planemask);
                    R600AppendCopyVertex(pScrn, srcX, srcY, dstX, dstY, hchunk, h);
                    R600DoCopy(pScrn);

                    srcX = srcX + hchunk;
                    dstX = dstX + hchunk;
                } else { /* diagonal right */
                    R600DoPrepareCopy(pScrn,
                                      dst_pitch, pDst->drawable.width, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      dst_pitch, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      accel_state->rop, accel_state->planemask);
                    R600AppendCopyVertex(pScrn, srcX + w - hchunk, srcY, dstX + w - hchunk, dstY, hchunk, h);
                    R600DoCopy(pScrn);
                }
                w = w - hchunk;
                hchunk = 0;
            }
        }

	if (vchunk == 0) { /* left/right */
	    if (srcX < dstX) { /* right */
		/* copy right to left */
		for (i = w; i > 0; i -= hchunk) {
		    R600DoPrepareCopy(pScrn,
				      dst_pitch, pDst->drawable.width, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
				      dst_pitch, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
				      accel_state->rop, accel_state->planemask);
		    R600AppendCopyVertex(pScrn, srcX + i - hchunk, srcY, dstX + i - hchunk, dstY, hchunk, h);
		    R600DoCopy(pScrn);
		}
	    } else { /* left */
		/* copy left to right */
		for (i = 0; i < w; i += hchunk) {
		    R600DoPrepareCopy(pScrn,
				      dst_pitch, pDst->drawable.width, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
				      dst_pitch, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
				      accel_state->rop, accel_state->planemask);

		    R600AppendCopyVertex(pScrn, srcX + i, srcY, dstX + i, dstY, hchunk, h);
		    R600DoCopy(pScrn);
		}
	    }
	} else { /* up/down */
	    if (srcY > dstY) { /* up */
		/* copy top to bottom */
                for (i = 0; i < h; i += vchunk) {
                    R600DoPrepareCopy(pScrn,
                                      dst_pitch, pDst->drawable.width, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      dst_pitch, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      accel_state->rop, accel_state->planemask);

                    if (vchunk > h - i) vchunk = h - i;
                    R600AppendCopyVertex(pScrn, srcX, srcY + i, dstX, dstY + i, w, vchunk);
                    R600DoCopy(pScrn);
                }
	    } else { /* down */
		/* copy bottom to top */
                for (i = h; i > 0; i -= vchunk) {
                    R600DoPrepareCopy(pScrn,
                                      dst_pitch, pDst->drawable.width, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      dst_pitch, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
                                      accel_state->rop, accel_state->planemask);

                    if (vchunk > i) vchunk = i;
                    R600AppendCopyVertex(pScrn, srcX, srcY + i - vchunk, dstX, dstY + i - vchunk, w, vchunk);
                    R600DoCopy(pScrn);
                }
            }
	}
    } else {
	R600DoPrepareCopy(pScrn,
			  dst_pitch, pDst->drawable.width, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
			  dst_pitch, pDst->drawable.height, dst_offset, pDst->drawable.bitsPerPixel,
			  accel_state->rop, accel_state->planemask);

	R600AppendCopyVertex(pScrn, srcX, srcY, dstX, dstY, w, h);
	R600DoCopy(pScrn);
    }
}

static void
R600Copy(PixmapPtr pDst,
	 int srcX, int srcY,
	 int dstX, int dstY,
	 int w, int h)
{
    ScrnInfoPtr pScrn = xf86Screens[pDst->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    if (accel_state->same_surface && (srcX == dstX) && (srcY == dstY))
	return;

    if (accel_state->same_surface && is_overlap(srcX, srcX + w, srcY, srcY + h, dstX, dstX + w, dstY, dstY + h)) {
	if (accel_state->copy_area) {
	    uint32_t pitch = exaGetPixmapPitch(pDst) / (pDst->drawable.bitsPerPixel / 8);
	    uint32_t orig_offset, tmp_offset;

	    tmp_offset = accel_state->copy_area->offset + info->fbLocation + pScrn->fbOffset;
	    orig_offset = exaGetPixmapOffset(pDst) + info->fbLocation + pScrn->fbOffset;

	    R600DoPrepareCopy(pScrn,
			      pitch, pDst->drawable.width, pDst->drawable.height, orig_offset, pDst->drawable.bitsPerPixel,
			      pitch,                       pDst->drawable.height, tmp_offset, pDst->drawable.bitsPerPixel,
			      accel_state->rop, accel_state->planemask);
	    R600AppendCopyVertex(pScrn, srcX, srcY, dstX, dstY, w, h);
	    R600DoCopy(pScrn);
	    R600DoPrepareCopy(pScrn,
			      pitch, pDst->drawable.width, pDst->drawable.height, tmp_offset, pDst->drawable.bitsPerPixel,
			      pitch,                       pDst->drawable.height, orig_offset, pDst->drawable.bitsPerPixel,
			      accel_state->rop, accel_state->planemask);
	    R600AppendCopyVertex(pScrn, dstX, dstY, dstX, dstY, w, h);
	    R600DoCopy(pScrn);
	} else
	    R600OverlapCopy(pDst, srcX, srcY, dstX, dstY, w, h);
    } else if (accel_state->same_surface) {
	uint32_t pitch = exaGetPixmapPitch(pDst) / (pDst->drawable.bitsPerPixel / 8);
	uint32_t offset = exaGetPixmapOffset(pDst) + info->fbLocation + pScrn->fbOffset;

	R600DoPrepareCopy(pScrn,
			  pitch, pDst->drawable.width, pDst->drawable.height, offset, pDst->drawable.bitsPerPixel,
			  pitch,                       pDst->drawable.height, offset, pDst->drawable.bitsPerPixel,
			  accel_state->rop, accel_state->planemask);
	R600AppendCopyVertex(pScrn, srcX, srcY, dstX, dstY, w, h);
	R600DoCopy(pScrn);
    } else {
	R600AppendCopyVertex(pScrn, srcX, srcY, dstX, dstY, w, h);
    }

}

static void
R600DoneCopy(PixmapPtr pDst)
{
    ScrnInfoPtr pScrn = xf86Screens[pDst->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    if (!accel_state->same_surface)
	R600DoCopy(pScrn);

    if (accel_state->copy_area) {
	exaOffscreenFree(pDst->drawable.pScreen, accel_state->copy_area);
	accel_state->copy_area = NULL;
    }

}

#define RADEON_TRACE_FALL 0
#define RADEON_TRACE_DRAW 0

#if RADEON_TRACE_FALL
#define RADEON_FALLBACK(x)     		\
do {					\
	ErrorF("%s: ", __FUNCTION__);	\
	ErrorF x;			\
	return FALSE;			\
} while (0)
#else
#define RADEON_FALLBACK(x) return FALSE
#endif

#define xFixedToFloat(f) (((float) (f)) / 65536)

static inline void transformPoint(PictTransform *transform, xPointFixed *point)
{
    PictVector v;
    v.vector[0] = point->x;
    v.vector[1] = point->y;
    v.vector[2] = xFixed1;
    PictureTransformPoint(transform, &v);
    point->x = v.vector[0];
    point->y = v.vector[1];
}

struct blendinfo {
    Bool dst_alpha;
    Bool src_alpha;
    uint32_t blend_cntl;
};

static struct blendinfo R600BlendOp[] = {
    /* Clear */
    {0, 0, (BLEND_ZERO << COLOR_SRCBLEND_shift) | (BLEND_ZERO << COLOR_DESTBLEND_shift)},
    /* Src */
    {0, 0, (BLEND_ONE << COLOR_SRCBLEND_shift) | (BLEND_ZERO << COLOR_DESTBLEND_shift)},
    /* Dst */
    {0, 0, (BLEND_ZERO << COLOR_SRCBLEND_shift) | (BLEND_ONE << COLOR_DESTBLEND_shift)},
    /* Over */
    {0, 1, (BLEND_ONE << COLOR_SRCBLEND_shift) | (BLEND_ONE_MINUS_SRC_ALPHA << COLOR_DESTBLEND_shift)},
    /* OverReverse */
    {1, 0, (BLEND_ONE_MINUS_DST_ALPHA << COLOR_SRCBLEND_shift) | (BLEND_ONE << COLOR_DESTBLEND_shift)},
    /* In */
    {1, 0, (BLEND_DST_ALPHA << COLOR_SRCBLEND_shift) | (BLEND_ZERO << COLOR_DESTBLEND_shift)},
    /* InReverse */
    {0, 1, (BLEND_ZERO << COLOR_SRCBLEND_shift) | (BLEND_SRC_ALPHA << COLOR_DESTBLEND_shift)},
    /* Out */
    {1, 0, (BLEND_ONE_MINUS_DST_ALPHA << COLOR_SRCBLEND_shift) | (BLEND_ZERO << COLOR_DESTBLEND_shift)},
    /* OutReverse */
    {0, 1, (BLEND_ZERO << COLOR_SRCBLEND_shift) | (BLEND_ONE_MINUS_SRC_ALPHA << COLOR_DESTBLEND_shift)},
    /* Atop */
    {1, 1, (BLEND_DST_ALPHA << COLOR_SRCBLEND_shift) | (BLEND_ONE_MINUS_SRC_ALPHA << COLOR_DESTBLEND_shift)},
    /* AtopReverse */
    {1, 1, (BLEND_ONE_MINUS_DST_ALPHA << COLOR_SRCBLEND_shift) | (BLEND_SRC_ALPHA << COLOR_DESTBLEND_shift)},
    /* Xor */
    {1, 1, (BLEND_ONE_MINUS_DST_ALPHA << COLOR_SRCBLEND_shift) | (BLEND_ONE_MINUS_SRC_ALPHA << COLOR_DESTBLEND_shift)},
    /* Add */
    {0, 0, (BLEND_ONE << COLOR_SRCBLEND_shift) | (BLEND_ONE << COLOR_DESTBLEND_shift)},
};

struct formatinfo {
    unsigned int fmt;
    uint32_t card_fmt;
};

static struct formatinfo R600TexFormats[] = {
    {PICT_a8r8g8b8,	FMT_8_8_8_8},
    {PICT_x8r8g8b8,	FMT_8_8_8_8},
    {PICT_a8b8g8r8,	FMT_8_8_8_8},
    {PICT_x8b8g8r8,	FMT_8_8_8_8},
    {PICT_r5g6b5,	FMT_5_6_5},
    {PICT_a1r5g5b5,	FMT_1_5_5_5},
    {PICT_x1r5g5b5,     FMT_1_5_5_5},
    {PICT_a8,		FMT_8},
};

static uint32_t R600GetBlendCntl(int op, PicturePtr pMask, uint32_t dst_format)
{
    uint32_t sblend, dblend;

    sblend = R600BlendOp[op].blend_cntl & COLOR_SRCBLEND_mask;
    dblend = R600BlendOp[op].blend_cntl & COLOR_DESTBLEND_mask;

    /* If there's no dst alpha channel, adjust the blend op so that we'll treat
     * it as always 1.
     */
    if (PICT_FORMAT_A(dst_format) == 0 && R600BlendOp[op].dst_alpha) {
	if (sblend == (BLEND_DST_ALPHA << COLOR_SRCBLEND_shift))
	    sblend = (BLEND_ONE << COLOR_SRCBLEND_shift);
	else if (sblend == (BLEND_ONE_MINUS_DST_ALPHA << COLOR_SRCBLEND_shift))
	    sblend = (BLEND_ZERO << COLOR_SRCBLEND_shift);
    }

    /* If the source alpha is being used, then we should only be in a case where
     * the source blend factor is 0, and the source blend value is the mask
     * channels multiplied by the source picture's alpha.
     */
    if (pMask && pMask->componentAlpha && R600BlendOp[op].src_alpha) {
	if (dblend == (BLEND_SRC_ALPHA << COLOR_DESTBLEND_shift)) {
	    dblend = (BLEND_SRC_COLOR << COLOR_DESTBLEND_shift);
	} else if (dblend == (BLEND_ONE_MINUS_SRC_ALPHA << COLOR_DESTBLEND_shift)) {
	    dblend = (BLEND_ONE_MINUS_SRC_COLOR << COLOR_DESTBLEND_shift);
	}
    }

    return sblend | dblend;
}

static Bool R600GetDestFormat(PicturePtr pDstPicture, uint32_t *dst_format)
{
    switch (pDstPicture->format) {
    case PICT_a8r8g8b8:
    case PICT_x8r8g8b8:
	*dst_format = COLOR_8_8_8_8;
	break;
    case PICT_r5g6b5:
	*dst_format = COLOR_5_6_5;
	break;
    case PICT_a1r5g5b5:
    case PICT_x1r5g5b5:
	*dst_format = COLOR_1_5_5_5;
	break;
    case PICT_a8:
	*dst_format = COLOR_8;
	break;
    default:
	RADEON_FALLBACK(("Unsupported dest format 0x%x\n",
	       (int)pDstPicture->format));
    }
    return TRUE;
}

static Bool R600CheckCompositeTexture(PicturePtr pPict,
				      PicturePtr pDstPict,
				      int op,
				      int unit)
{
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    unsigned int i;
    int max_tex_w, max_tex_h;

    max_tex_w = 8192;
    max_tex_h = 8192;

    if ((w > max_tex_w) || (h > max_tex_h))
	RADEON_FALLBACK(("Picture w/h too large (%dx%d)\n", w, h));

    for (i = 0; i < sizeof(R600TexFormats) / sizeof(R600TexFormats[0]); i++) {
	if (R600TexFormats[i].fmt == pPict->format)
	    break;
    }
    if (i == sizeof(R600TexFormats) / sizeof(R600TexFormats[0]))
	RADEON_FALLBACK(("Unsupported picture format 0x%x\n",
			 (int)pPict->format));

    if (pPict->filter != PictFilterNearest &&
	pPict->filter != PictFilterBilinear)
	RADEON_FALLBACK(("Unsupported filter 0x%x\n", pPict->filter));

    /* for REPEAT_NONE, Render semantics are that sampling outside the source
     * picture results in alpha=0 pixels. We can implement this with a border color
     * *if* our source texture has an alpha channel, otherwise we need to fall
     * back. If we're not transformed then we hope that upper layers have clipped
     * rendering to the bounds of the source drawable, in which case it doesn't
     * matter. I have not, however, verified that the X server always does such
     * clipping.
     */
    /* FIXME R6xx */
    if (pPict->transform != 0 && !pPict->repeat && PICT_FORMAT_A(pPict->format) == 0) {
	if (!(((op == PictOpSrc) || (op == PictOpClear)) && (PICT_FORMAT_A(pDstPict->format) == 0)))
	    RADEON_FALLBACK(("REPEAT_NONE unsupported for transformed xRGB source\n"));
    }

    return TRUE;
}

static Bool R600TextureSetup(PicturePtr pPict, PixmapPtr pPix,
					int unit)
{
    ScrnInfoPtr pScrn = xf86Screens[pPix->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    int w = pPict->pDrawable->width;
    int h = pPict->pDrawable->height;
    unsigned int i;
    tex_resource_t  tex_res;
    tex_sampler_t   tex_samp;
    int pix_r, pix_g, pix_b, pix_a;

    CLEAR (tex_res);
    CLEAR (tex_samp);

    accel_state->src_mc_addr[unit] = exaGetPixmapOffset(pPix) + info->fbLocation + pScrn->fbOffset;
    accel_state->src_pitch[unit] = exaGetPixmapPitch(pPix) / (pPix->drawable.bitsPerPixel / 8);
    accel_state->src_size[unit] = exaGetPixmapPitch(pPix) * pPix->drawable.height;

    if (accel_state->src_pitch[1] & 7)
	RADEON_FALLBACK(("Bad pitch %d 0x%x\n", (int)accel_state->src_pitch[unit], unit));

    if (accel_state->src_mc_addr[1] & 0xff)
	RADEON_FALLBACK(("Bad offset %d 0x%x\n", (int)accel_state->src_mc_addr[unit], unit));

    for (i = 0; i < sizeof(R600TexFormats) / sizeof(R600TexFormats[0]); i++) {
	if (R600TexFormats[i].fmt == pPict->format)
	    break;
    }

    accel_state->texW[unit] = w;
    accel_state->texH[unit] = h;

    /* ErrorF("Tex %d setup %dx%d\n", unit, w, h);  */

    /* flush texture cache */
    cp_set_surface_sync(pScrn, accel_state->ib, TC_ACTION_ENA_bit,
			accel_state->src_size[unit], accel_state->src_mc_addr[unit]);

    /* Texture */
    tex_res.id                  = unit;
    tex_res.w                   = w;
    tex_res.h                   = h;
    tex_res.pitch               = accel_state->src_pitch[unit];
    tex_res.depth               = 0;
    tex_res.dim                 = SQ_TEX_DIM_2D;
    tex_res.base                = accel_state->src_mc_addr[unit];
    tex_res.mip_base            = accel_state->src_mc_addr[unit];
    tex_res.format              = R600TexFormats[i].card_fmt;
    tex_res.request_size        = 1;

    /* component swizzles */
    switch (pPict->format) {
    case PICT_a1r5g5b5:
    case PICT_a8r8g8b8:
	pix_r = SQ_SEL_Z; /* R */
	pix_g = SQ_SEL_Y; /* G */
	pix_b = SQ_SEL_X; /* B */
	pix_a = SQ_SEL_W; /* A */
	break;
    case PICT_a8b8g8r8:
	pix_r = SQ_SEL_X; /* R */
	pix_g = SQ_SEL_Y; /* G */
	pix_b = SQ_SEL_Z; /* B */
	pix_a = SQ_SEL_W; /* A */
	break;
    case PICT_x8b8g8r8:
	pix_r = SQ_SEL_X; /* R */
	pix_g = SQ_SEL_Y; /* G */
	pix_b = SQ_SEL_Z; /* B */
	pix_a = SQ_SEL_1; /* A */
	break;
    case PICT_x1r5g5b5:
    case PICT_x8r8g8b8:
    case PICT_r5g6b5:
	pix_r = SQ_SEL_Z; /* R */
	pix_g = SQ_SEL_Y; /* G */
	pix_b = SQ_SEL_X; /* B */
	pix_a = SQ_SEL_1; /* A */
	break;
    case PICT_a8:
	pix_r = SQ_SEL_0; /* R */
	pix_g = SQ_SEL_0; /* G */
	pix_b = SQ_SEL_0; /* B */
	pix_a = SQ_SEL_X; /* A */
	break;
    default:
	RADEON_FALLBACK(("Bad format 0x%x\n", pPict->format));
    }

    if (unit == 0) {
	if (!accel_state->has_mask) {
	    if (PICT_FORMAT_RGB(pPict->format) == 0) {
		pix_r = SQ_SEL_0;
		pix_g = SQ_SEL_0;
		pix_b = SQ_SEL_0;
	    }

	    if (PICT_FORMAT_A(pPict->format) == 0)
		pix_a = SQ_SEL_1;
	} else {
	    if (accel_state->component_alpha) {
		if (accel_state->src_alpha) {
		    if (PICT_FORMAT_A(pPict->format) == 0) {
			pix_r = SQ_SEL_1;
			pix_g = SQ_SEL_1;
			pix_b = SQ_SEL_1;
			pix_a = SQ_SEL_1;
		    } else {
			pix_r = pix_a;
			pix_g = pix_a;
			pix_b = pix_a;
		    }
		} else {
		    if (PICT_FORMAT_A(pPict->format) == 0)
			pix_a = SQ_SEL_1;
		}
	    } else {
		if (PICT_FORMAT_RGB(pPict->format) == 0) {
		    pix_r = SQ_SEL_0;
		    pix_g = SQ_SEL_0;
		    pix_b = SQ_SEL_0;
		}

		if (PICT_FORMAT_A(pPict->format) == 0)
		    pix_a = SQ_SEL_1;
	    }
	}
    } else {
	if (accel_state->component_alpha) {
	    if (PICT_FORMAT_A(pPict->format) == 0)
		pix_a = SQ_SEL_1;
	} else {
	    if (PICT_FORMAT_A(pPict->format) == 0) {
		pix_r = SQ_SEL_1;
		pix_g = SQ_SEL_1;
		pix_b = SQ_SEL_1;
		pix_a = SQ_SEL_1;
	    } else {
		pix_r = pix_a;
		pix_g = pix_a;
		pix_b = pix_a;
	    }
	}
    }

    tex_res.dst_sel_x           = pix_r; /* R */
    tex_res.dst_sel_y           = pix_g; /* G */
    tex_res.dst_sel_z           = pix_b; /* B */
    tex_res.dst_sel_w           = pix_a; /* A */

    tex_res.base_level          = 0;
    tex_res.last_level          = 0;
    tex_res.perf_modulation     = 0;
    set_tex_resource            (pScrn, accel_state->ib, &tex_res);

    tex_samp.id                 = unit;
    tex_samp.border_color       = SQ_TEX_BORDER_COLOR_TRANS_BLACK;

    if (pPict->repeat) {
	switch (pPict->repeatType) {
	case RepeatNormal:
	    tex_samp.clamp_x            = SQ_TEX_WRAP;
	    tex_samp.clamp_y            = SQ_TEX_WRAP;
	    break;
	case RepeatPad:
	    tex_samp.clamp_x            = SQ_TEX_CLAMP_LAST_TEXEL;
	    tex_samp.clamp_y            = SQ_TEX_CLAMP_LAST_TEXEL;
	    break;
	case RepeatReflect:
	    tex_samp.clamp_x            = SQ_TEX_MIRROR;
	    tex_samp.clamp_y            = SQ_TEX_MIRROR;
	    break;
	case RepeatNone:
	    tex_samp.clamp_x            = SQ_TEX_CLAMP_BORDER;
	    tex_samp.clamp_y            = SQ_TEX_CLAMP_BORDER;
	    break;
	default:
	    RADEON_FALLBACK(("Bad repeat 0x%x\n", pPict->repeatType));
	}
    } else {
	tex_samp.clamp_x            = SQ_TEX_CLAMP_BORDER;
	tex_samp.clamp_y            = SQ_TEX_CLAMP_BORDER;
    }

    switch (pPict->filter) {
    case PictFilterNearest:
	tex_samp.xy_mag_filter      = SQ_TEX_XY_FILTER_POINT;
	tex_samp.xy_min_filter      = SQ_TEX_XY_FILTER_POINT;
	break;
    case PictFilterBilinear:
	tex_samp.xy_mag_filter      = SQ_TEX_XY_FILTER_BILINEAR;
	tex_samp.xy_min_filter      = SQ_TEX_XY_FILTER_BILINEAR;
	break;
    default:
	RADEON_FALLBACK(("Bad filter 0x%x\n", pPict->filter));
    }

    tex_samp.clamp_z            = SQ_TEX_WRAP;
    tex_samp.z_filter           = SQ_TEX_Z_FILTER_NONE;
    tex_samp.mip_filter         = 0;			/* no mipmap */
    set_tex_sampler             (pScrn, accel_state->ib, &tex_samp);

    if (pPict->transform != 0) {
	accel_state->is_transform[unit] = TRUE;
	accel_state->transform[unit] = pPict->transform;
    } else
	accel_state->is_transform[unit] = FALSE;

    return TRUE;
}

static Bool R600CheckComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
			       PicturePtr pDstPicture)
{
    uint32_t tmp1;
    PixmapPtr pSrcPixmap, pDstPixmap;
    int max_tex_w, max_tex_h, max_dst_w, max_dst_h;

    /* Check for unsupported compositing operations. */
    if (op >= (int) (sizeof(R600BlendOp) / sizeof(R600BlendOp[0])))
	RADEON_FALLBACK(("Unsupported Composite op 0x%x\n", op));

    if (!pSrcPicture->pDrawable)
	RADEON_FALLBACK(("Solid or gradient pictures not supported yet\n"));

    pSrcPixmap = RADEONGetDrawablePixmap(pSrcPicture->pDrawable);

    max_tex_w = 8192;
    max_tex_h = 8192;
    max_dst_w = 8192;
    max_dst_h = 8192;

    if (pSrcPixmap->drawable.width >= max_tex_w ||
	pSrcPixmap->drawable.height >= max_tex_h) {
	RADEON_FALLBACK(("Source w/h too large (%d,%d).\n",
			 pSrcPixmap->drawable.width,
			 pSrcPixmap->drawable.height));
    }

    pDstPixmap = RADEONGetDrawablePixmap(pDstPicture->pDrawable);

    if (pDstPixmap->drawable.width >= max_dst_w ||
	pDstPixmap->drawable.height >= max_dst_h) {
	RADEON_FALLBACK(("Dest w/h too large (%d,%d).\n",
			 pDstPixmap->drawable.width,
			 pDstPixmap->drawable.height));
    }

    if (pMaskPicture) {
	PixmapPtr pMaskPixmap;

	if (!pMaskPicture->pDrawable)
	    RADEON_FALLBACK(("Solid or gradient pictures not supported yet\n"));

	pMaskPixmap = RADEONGetDrawablePixmap(pMaskPicture->pDrawable);

	if (pMaskPixmap->drawable.width >= max_tex_w ||
	    pMaskPixmap->drawable.height >= max_tex_h) {
	    RADEON_FALLBACK(("Mask w/h too large (%d,%d).\n",
			     pMaskPixmap->drawable.width,
			     pMaskPixmap->drawable.height));
	}

	if (pMaskPicture->componentAlpha) {
	    /* Check if it's component alpha that relies on a source alpha and
	     * on the source value.  We can only get one of those into the
	     * single source value that we get to blend with.
	     */
	    if (R600BlendOp[op].src_alpha &&
		(R600BlendOp[op].blend_cntl & COLOR_SRCBLEND_mask) !=
		(BLEND_ZERO << COLOR_SRCBLEND_shift)) {
		RADEON_FALLBACK(("Component alpha not supported with source "
				 "alpha and source value blending.\n"));
	    }
	}

	if (!R600CheckCompositeTexture(pMaskPicture, pDstPicture, op, 1))
	    return FALSE;
    }

    if (!R600CheckCompositeTexture(pSrcPicture, pDstPicture, op, 0))
	return FALSE;

    if (!R600GetDestFormat(pDstPicture, &tmp1))
	return FALSE;

    return TRUE;

}

static Bool R600PrepareComposite(int op, PicturePtr pSrcPicture,
				 PicturePtr pMaskPicture, PicturePtr pDstPicture,
				 PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
    ScrnInfoPtr pScrn = xf86Screens[pSrc->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    uint32_t blendcntl, dst_format;
    cb_config_t cb_conf;
    shader_config_t vs_conf, ps_conf;

    /* return FALSE; */

    if (pMask) {
	accel_state->has_mask = TRUE;
	if (pMaskPicture->componentAlpha) {
	    accel_state->component_alpha = TRUE;
	    if (R600BlendOp[op].src_alpha)
		accel_state->src_alpha = TRUE;
	    else
		accel_state->src_alpha = FALSE;
	} else {
	    accel_state->component_alpha = FALSE;
	    accel_state->src_alpha = FALSE;
	}
    } else {
	accel_state->has_mask = FALSE;
	accel_state->component_alpha = FALSE;
	accel_state->src_alpha = FALSE;
    }

    accel_state->dst_mc_addr = exaGetPixmapOffset(pDst) + info->fbLocation + pScrn->fbOffset;
    accel_state->dst_pitch = exaGetPixmapPitch(pDst) / (pDst->drawable.bitsPerPixel / 8);
    accel_state->dst_size = exaGetPixmapPitch(pDst) * pDst->drawable.height;

    if (accel_state->dst_pitch & 7)
	RADEON_FALLBACK(("Bad dst pitch 0x%x\n", (int)accel_state->dst_pitch));

    if (accel_state->dst_mc_addr & 0xff)
	RADEON_FALLBACK(("Bad destination offset 0x%x\n", (int)accel_state->dst_mc_addr));

    if (!R600GetDestFormat(pDstPicture, &dst_format))
	return FALSE;

    CLEAR (cb_conf);
    CLEAR (vs_conf);
    CLEAR (ps_conf);

    accel_state->ib = RADEONCPGetBuffer(pScrn);

    /* Init */
    start_3d(pScrn, accel_state->ib);

    set_default_state(pScrn, accel_state->ib);

    /* Scissor / viewport */
    EREG(accel_state->ib, PA_CL_VTE_CNTL,                      VTX_XY_FMT_bit);
    EREG(accel_state->ib, PA_CL_CLIP_CNTL,                     CLIP_DISABLE_bit);

    if (!R600TextureSetup(pSrcPicture, pSrc, 0)) {
	R600IBDiscard(pScrn, accel_state->ib);
	return FALSE;
    }

    if (pMask) {
	if (!R600TextureSetup(pMaskPicture, pMask, 1)) {
	    R600IBDiscard(pScrn, accel_state->ib);
	    return FALSE;
	}
    } else
	accel_state->is_transform[1] = FALSE;

    if (pMask) {
	set_bool_consts(pScrn, accel_state->ib, SQ_BOOL_CONST_vs, (1 << 0));
	accel_state->ps_mc_addr = info->fbLocation + pScrn->fbOffset + accel_state->shaders->offset +
	    accel_state->comp_mask_ps_offset;
    } else {
	set_bool_consts(pScrn, accel_state->ib, SQ_BOOL_CONST_vs, (0 << 0));
	accel_state->ps_mc_addr = info->fbLocation + pScrn->fbOffset + accel_state->shaders->offset +
	    accel_state->comp_ps_offset;
    }

    accel_state->vs_mc_addr = info->fbLocation + pScrn->fbOffset + accel_state->shaders->offset +
	accel_state->comp_vs_offset;

    accel_state->vs_size = 512;
    accel_state->ps_size = 512;

    /* Shader */

    /* flush SQ cache */
    cp_set_surface_sync(pScrn, accel_state->ib, SH_ACTION_ENA_bit,
			accel_state->vs_size, accel_state->vs_mc_addr);

    vs_conf.shader_addr         = accel_state->vs_mc_addr;
    vs_conf.num_gprs            = 3;
    vs_conf.stack_size          = 1;
    vs_setup                    (pScrn, accel_state->ib, &vs_conf);

    /* flush SQ cache */
    cp_set_surface_sync(pScrn, accel_state->ib, SH_ACTION_ENA_bit,
			accel_state->ps_size, accel_state->ps_mc_addr);

    ps_conf.shader_addr         = accel_state->ps_mc_addr;
    ps_conf.num_gprs            = 3;
    ps_conf.stack_size          = 0;
    ps_conf.uncached_first_inst = 1;
    ps_conf.clamp_consts        = 0;
    ps_conf.export_mode         = 2;
    ps_setup                    (pScrn, accel_state->ib, &ps_conf);

    EREG(accel_state->ib, CB_SHADER_MASK,                      (0xf << OUTPUT0_ENABLE_shift));
    EREG(accel_state->ib, R7xx_CB_SHADER_CONTROL,              (RT0_ENABLE_bit));

    blendcntl = R600GetBlendCntl(op, pMaskPicture, pDstPicture->format);

    if (info->ChipFamily == CHIP_FAMILY_R600) {
	/* no per-MRT blend on R600 */
	EREG(accel_state->ib, CB_COLOR_CONTROL,                    RADEON_ROP[3] | (1 << TARGET_BLEND_ENABLE_shift));
	EREG(accel_state->ib, CB_BLEND_CONTROL,                    blendcntl);
    } else {
	EREG(accel_state->ib, CB_COLOR_CONTROL,                    (RADEON_ROP[3] |
								    (1 << TARGET_BLEND_ENABLE_shift) |
								    PER_MRT_BLEND_bit));
	EREG(accel_state->ib, CB_BLEND0_CONTROL,                   blendcntl);
    }

    cb_conf.id = 0;
    cb_conf.w = accel_state->dst_pitch;
    cb_conf.h = pDst->drawable.height;
    cb_conf.base = accel_state->dst_mc_addr;
    cb_conf.format = dst_format;

    switch (pDstPicture->format) {
    case PICT_a8r8g8b8:
    case PICT_x8r8g8b8:
    case PICT_a1r5g5b5:
    case PICT_x1r5g5b5:
    default:
	cb_conf.comp_swap = 1; /* ARGB */
	break;
    case PICT_r5g6b5:
	cb_conf.comp_swap = 2; /* RGB */
	break;
    case PICT_a8:
	cb_conf.comp_swap = 3; /* A */
	break;
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
    if (pMask) {
	/* export 2 tex coords from VS */
	EREG(accel_state->ib, SPI_VS_OUT_CONFIG, ((2 - 1) << VS_EXPORT_COUNT_shift));
	/* src = semantic id 0; mask = semantic id 1 */
	EREG(accel_state->ib, SPI_VS_OUT_ID_0, ((0 << SEMANTIC_0_shift) |
						  (1 << SEMANTIC_1_shift)));
	/* input 2 tex coords from VS */
	EREG(accel_state->ib, SPI_PS_IN_CONTROL_0, (2 << NUM_INTERP_shift));
    } else {
	/* export 1 tex coords from VS */
	EREG(accel_state->ib, SPI_VS_OUT_CONFIG, ((1 - 1) << VS_EXPORT_COUNT_shift));
	/* src = semantic id 0 */
	EREG(accel_state->ib, SPI_VS_OUT_ID_0,   (0 << SEMANTIC_0_shift));
	/* input 1 tex coords from VS */
	EREG(accel_state->ib, SPI_PS_IN_CONTROL_0, (1 << NUM_INTERP_shift));
    }
    EREG(accel_state->ib, SPI_PS_IN_CONTROL_1,                 0);
    /* SPI_PS_INPUT_CNTL_0 maps to GPR[0] - load with semantic id 0 */
    EREG(accel_state->ib, SPI_PS_INPUT_CNTL_0 + (0 <<2),       ((0    << SEMANTIC_shift)	|
								(0x01 << DEFAULT_VAL_shift)	|
								SEL_CENTROID_bit));
    /* SPI_PS_INPUT_CNTL_1 maps to GPR[1] - load with semantic id 1 */
    EREG(accel_state->ib, SPI_PS_INPUT_CNTL_0 + (1 <<2),       ((1    << SEMANTIC_shift)	|
								(0x01 << DEFAULT_VAL_shift)	|
								SEL_CENTROID_bit));
    EREG(accel_state->ib, SPI_INTERP_CONTROL_0,                0);

    accel_state->vb_index = 0;

    return TRUE;
}

static void R600Composite(PixmapPtr pDst,
			  int srcX, int srcY,
			  int maskX, int maskY,
			  int dstX, int dstY,
			  int w, int h)
{
    ScrnInfoPtr pScrn = xf86Screens[pDst->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    float *vb;
    xPointFixed srcTopLeft, srcTopRight, srcBottomLeft, srcBottomRight;

    /* ErrorF("R600Composite (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",
       srcX, srcY, maskX, maskY,dstX, dstY, w, h); */

    srcTopLeft.x     = IntToxFixed(srcX);
    srcTopLeft.y     = IntToxFixed(srcY);
    srcTopRight.x    = IntToxFixed(srcX + w);
    srcTopRight.y    = IntToxFixed(srcY);
    srcBottomLeft.x  = IntToxFixed(srcX);
    srcBottomLeft.y  = IntToxFixed(srcY + h);
    srcBottomRight.x = IntToxFixed(srcX + w);
    srcBottomRight.y = IntToxFixed(srcY + h);

    /* XXX do transform in vertex shader */
    if (accel_state->is_transform[0]) {
	transformPoint(accel_state->transform[0], &srcTopLeft);
	transformPoint(accel_state->transform[0], &srcTopRight);
	transformPoint(accel_state->transform[0], &srcBottomLeft);
	transformPoint(accel_state->transform[0], &srcBottomRight);
    }

    if (accel_state->has_mask) {
	xPointFixed maskTopLeft, maskTopRight, maskBottomLeft, maskBottomRight;

	if (((accel_state->vb_index + 3) * 24) > (accel_state->ib->total / 2)) {
	    R600DoneComposite(pDst);
	    accel_state->vb_index = 0;
	    accel_state->ib = RADEONCPGetBuffer(pScrn);
	}

	vb = (pointer)((char*)accel_state->ib->address +
		       (accel_state->ib->total / 2) +
		       accel_state->vb_index * 24);

	maskTopLeft.x     = IntToxFixed(maskX);
	maskTopLeft.y     = IntToxFixed(maskY);
	maskTopRight.x    = IntToxFixed(maskX + w);
	maskTopRight.y    = IntToxFixed(maskY);
	maskBottomLeft.x  = IntToxFixed(maskX);
	maskBottomLeft.y  = IntToxFixed(maskY + h);
	maskBottomRight.x = IntToxFixed(maskX + w);
	maskBottomRight.y = IntToxFixed(maskY + h);

	if (accel_state->is_transform[1]) {
	    transformPoint(accel_state->transform[1], &maskTopLeft);
	    transformPoint(accel_state->transform[1], &maskTopRight);
	    transformPoint(accel_state->transform[1], &maskBottomLeft);
	    transformPoint(accel_state->transform[1], &maskBottomRight);
	}

	vb[0] = (float)dstX;
	vb[1] = (float)dstY;
	vb[2] = xFixedToFloat(srcTopLeft.x) / accel_state->texW[0];
	vb[3] = xFixedToFloat(srcTopLeft.y) / accel_state->texH[0];
	vb[4] = xFixedToFloat(maskTopLeft.x) / accel_state->texW[1];
	vb[5] = xFixedToFloat(maskTopLeft.y) / accel_state->texH[1];

	vb[6] = (float)dstX;
	vb[7] = (float)(dstY + h);
	vb[8] = xFixedToFloat(srcBottomLeft.x) / accel_state->texW[0];
	vb[9] = xFixedToFloat(srcBottomLeft.y) / accel_state->texH[0];
	vb[10] = xFixedToFloat(maskBottomLeft.x) / accel_state->texW[1];
	vb[11] = xFixedToFloat(maskBottomLeft.y) / accel_state->texH[1];

	vb[12] = (float)(dstX + w);
	vb[13] = (float)(dstY + h);
	vb[14] = xFixedToFloat(srcBottomRight.x) / accel_state->texW[0];
	vb[15] = xFixedToFloat(srcBottomRight.y) / accel_state->texH[0];
	vb[16] = xFixedToFloat(maskBottomRight.x) / accel_state->texW[1];
	vb[17] = xFixedToFloat(maskBottomRight.y) / accel_state->texH[1];

    } else {
	if (((accel_state->vb_index + 3) * 16) > (accel_state->ib->total / 2)) {
	    R600DoneComposite(pDst);
	    accel_state->vb_index = 0;
	    accel_state->ib = RADEONCPGetBuffer(pScrn);
	}

	vb = (pointer)((char*)accel_state->ib->address +
		       (accel_state->ib->total / 2) +
		       accel_state->vb_index * 16);

	vb[0] = (float)dstX;
	vb[1] = (float)dstY;
	vb[2] = xFixedToFloat(srcTopLeft.x) / accel_state->texW[0];
	vb[3] = xFixedToFloat(srcTopLeft.y) / accel_state->texH[0];

	vb[4] = (float)dstX;
	vb[5] = (float)(dstY + h);
	vb[6] = xFixedToFloat(srcBottomLeft.x) / accel_state->texW[0];
	vb[7] = xFixedToFloat(srcBottomLeft.y) / accel_state->texH[0];

	vb[8] = (float)(dstX + w);
	vb[9] = (float)(dstY + h);
	vb[10] = xFixedToFloat(srcBottomRight.x) / accel_state->texW[0];
	vb[11] = xFixedToFloat(srcBottomRight.y) / accel_state->texH[0];
    }

    accel_state->vb_index += 3;

}

static void R600DoneComposite(PixmapPtr pDst)
{
    ScrnInfoPtr pScrn = xf86Screens[pDst->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    draw_config_t   draw_conf;
    vtx_resource_t  vtx_res;

    CLEAR (draw_conf);
    CLEAR (vtx_res);

    if (accel_state->vb_index == 0) {
	R600IBDiscard(pScrn, accel_state->ib);
	return;
    }

    accel_state->vb_mc_addr = info->gartLocation + info->dri->bufStart +
	(accel_state->ib->idx * accel_state->ib->total) + (accel_state->ib->total / 2);


    /* Vertex buffer setup */
    if (accel_state->has_mask) {
	accel_state->vb_size = accel_state->vb_index * 24;
	vtx_res.id              = SQ_VTX_RESOURCE_vs;
	vtx_res.vtx_size_dw     = 24 / 4;
	vtx_res.vtx_num_entries = accel_state->vb_size / 4;
	vtx_res.mem_req_size    = 1;
	vtx_res.vb_addr         = accel_state->vb_mc_addr;
    } else {
	accel_state->vb_size = accel_state->vb_index * 16;
	vtx_res.id              = SQ_VTX_RESOURCE_vs;
	vtx_res.vtx_size_dw     = 16 / 4;
	vtx_res.vtx_num_entries = accel_state->vb_size / 4;
	vtx_res.mem_req_size    = 1;
	vtx_res.vb_addr         = accel_state->vb_mc_addr;
    }
    /* flush vertex cache */
    if ((info->ChipFamily == CHIP_FAMILY_RV610) ||
	(info->ChipFamily == CHIP_FAMILY_RV620) ||
	(info->ChipFamily == CHIP_FAMILY_RS780) ||
	(info->ChipFamily == CHIP_FAMILY_RV710))
	cp_set_surface_sync(pScrn, accel_state->ib, TC_ACTION_ENA_bit,
			    accel_state->vb_size, accel_state->vb_mc_addr);
    else
	cp_set_surface_sync(pScrn, accel_state->ib, VC_ACTION_ENA_bit,
			    accel_state->vb_size, accel_state->vb_mc_addr);

    set_vtx_resource        (pScrn, accel_state->ib, &vtx_res);

    draw_conf.prim_type          = DI_PT_RECTLIST;
    draw_conf.vgt_draw_initiator = DI_SRC_SEL_AUTO_INDEX;
    draw_conf.num_instances      = 1;
    draw_conf.num_indices        = vtx_res.vtx_num_entries / vtx_res.vtx_size_dw;
    draw_conf.index_type         = DI_INDEX_SIZE_16_BIT;

    draw_auto(pScrn, accel_state->ib, &draw_conf);

    wait_3d_idle_clean(pScrn, accel_state->ib);

    cp_set_surface_sync(pScrn, accel_state->ib, (CB_ACTION_ENA_bit | CB0_DEST_BASE_ENA_bit),
			accel_state->dst_size, accel_state->dst_mc_addr);

    R600CPFlushIndirect(pScrn, accel_state->ib);
}

Bool
R600CopyToVRAM(ScrnInfoPtr pScrn,
	       char *src, int src_pitch,
	       uint32_t dst_pitch, uint32_t dst_mc_addr, uint32_t dst_height, int bpp,
	       int x, int y, int w, int h)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t scratch_mc_addr;
    int wpass = w * (bpp/8);
    int scratch_pitch_bytes = (wpass + 255) & ~255;
    uint32_t scratch_pitch = scratch_pitch_bytes / (bpp / 8);
    int scratch_offset = 0, hpass, temph;
    char *dst;
    drmBufPtr scratch;

    if (dst_pitch & 7)
	return FALSE;

    if (dst_mc_addr & 0xff)
	return FALSE;

    scratch = RADEONCPGetBuffer(pScrn);
    if (scratch == NULL)
	return FALSE;

    scratch_mc_addr = info->gartLocation + info->dri->bufStart + (scratch->idx * scratch->total);
    temph = hpass = min(h, scratch->total/2 / scratch_pitch_bytes);
    dst = (char *)scratch->address;

    /* memcopy from sys to scratch */
    while (temph--) {
	memcpy (dst, src, wpass);
	src += src_pitch;
	dst += scratch_pitch_bytes;
    }

    while (h) {
	uint32_t offset = scratch_mc_addr + scratch_offset;
	int oldhpass = hpass;
	h -= oldhpass;
	temph = hpass = min(h, scratch->total/2 / scratch_pitch_bytes);

	if (hpass) {
	    scratch_offset = scratch->total/2 - scratch_offset;
	    dst = (char *)scratch->address + scratch_offset;
	    /* wait for the engine to be idle */
	    RADEONWaitForIdleCP(pScrn);
	    //memcopy from sys to scratch
	    while (temph--) {
		memcpy (dst, src, wpass);
		src += src_pitch;
		dst += scratch_pitch_bytes;
	    }
	}
	/* blit from scratch to vram */
	R600DoPrepareCopy(pScrn,
			  scratch_pitch, w, oldhpass, offset, bpp,
			  dst_pitch, dst_height, dst_mc_addr, bpp,
			  3, 0xffffffff);
	R600AppendCopyVertex(pScrn, 0, 0, x, y, w, oldhpass);
	R600DoCopy(pScrn);
	y += oldhpass;
    }

    R600IBDiscard(pScrn, scratch);

    return TRUE;
}

static Bool
R600UploadToScreen(PixmapPtr pDst, int x, int y, int w, int h,
		   char *src, int src_pitch)
{
    ScrnInfoPtr pScrn = xf86Screens[pDst->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t dst_pitch = exaGetPixmapPitch(pDst) / (pDst->drawable.bitsPerPixel / 8);
    uint32_t dst_mc_addr = exaGetPixmapOffset(pDst) + info->fbLocation + pScrn->fbOffset;
    uint32_t dst_height = pDst->drawable.height;
    int bpp = pDst->drawable.bitsPerPixel;

    return R600CopyToVRAM(pScrn,
			  src, src_pitch,
			  dst_pitch, dst_mc_addr, dst_height, bpp,
			  x, y, w, h);
}

static Bool
R600DownloadFromScreen(PixmapPtr pSrc, int x, int y, int w, int h,
		       char *dst, int dst_pitch)
{
    ScrnInfoPtr pScrn = xf86Screens[pSrc->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t src_pitch = exaGetPixmapPitch(pSrc) / (pSrc->drawable.bitsPerPixel / 8);
    uint32_t src_mc_addr = exaGetPixmapOffset(pSrc) + info->fbLocation + pScrn->fbOffset;
    uint32_t src_width = pSrc->drawable.width;
    uint32_t src_height = pSrc->drawable.height;
    int bpp = pSrc->drawable.bitsPerPixel;
    uint32_t scratch_mc_addr;
    int scratch_pitch_bytes = (dst_pitch + 255) & ~255;
    int scratch_offset = 0, hpass;
    uint32_t scratch_pitch = scratch_pitch_bytes / (bpp / 8);
    int wpass = w * (bpp/8);
    drmBufPtr scratch;

    if (src_pitch & 7)
	return FALSE;

    scratch = RADEONCPGetBuffer(pScrn);
    if (scratch == NULL)
	return FALSE;

    scratch_mc_addr = info->gartLocation + info->dri->bufStart + (scratch->idx * scratch->total);
    hpass = min(h, scratch->total/2 / scratch_pitch_bytes);

    /* blit from vram to scratch */
    R600DoPrepareCopy(pScrn,
		      src_pitch, src_width, src_height, src_mc_addr, bpp,
		      scratch_pitch, hpass, scratch_mc_addr, bpp,
		      3, 0xffffffff);
    R600AppendCopyVertex(pScrn, x, y, 0, 0, w, hpass);
    R600DoCopy(pScrn);

    while (h) {
	char *src = (char *)scratch->address + scratch_offset;
	int oldhpass = hpass;
	h -= oldhpass;
	y += oldhpass;
	hpass = min(h, scratch->total/2 / scratch_pitch_bytes);

	if (hpass) {
	    scratch_offset = scratch->total/2 - scratch_offset;
	    /* blit from vram to scratch */
	    R600DoPrepareCopy(pScrn,
			      src_pitch, src_width, src_height, src_mc_addr, bpp,
			      scratch_pitch, hpass, scratch_mc_addr + scratch_offset, bpp,
			      3, 0xffffffff);
	    R600AppendCopyVertex(pScrn, x, y, 0, 0, w, hpass);
	    R600DoCopy(pScrn);
	}

	/* wait for the engine to be idle */
	RADEONWaitForIdleCP(pScrn);
	/* memcopy from scratch to sys */
	while (oldhpass--) {
	    memcpy (dst, src, wpass);
	    dst += dst_pitch;
	    src += scratch_pitch_bytes;
	}
    }

    R600IBDiscard(pScrn, scratch);

    return TRUE;

}

static int
R600MarkSync(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    return ++accel_state->exaSyncMarker;

}

static void
R600Sync(ScreenPtr pScreen, int marker)
{
    ScrnInfoPtr pScrn = xf86Screens[pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    if (accel_state->exaMarkerSynced != marker) {
	RADEONWaitForIdleCP(pScrn);
	accel_state->exaMarkerSynced = marker;
    }

}

static Bool
R600AllocShaders(ScrnInfoPtr pScrn, ScreenPtr pScreen)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    /* 512 bytes per shader for now */
    int size = 512 * 9;

    accel_state->shaders = NULL;

    accel_state->shaders = exaOffscreenAlloc(pScreen, size, 256,
					     TRUE, NULL, NULL);

    if (accel_state->shaders == NULL)
	return FALSE;
    return TRUE;
}

Bool
R600LoadShaders(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    RADEONChipFamily ChipSet = info->ChipFamily;
    uint32_t *shader;

    shader = (pointer)((char *)info->FB + accel_state->shaders->offset);

    /*  solid vs --------------------------------------- */
    accel_state->solid_vs_offset = 0;
    R600_solid_vs(ChipSet, shader + accel_state->solid_vs_offset / 4);

    /*  solid ps --------------------------------------- */
    accel_state->solid_ps_offset = 512;
    R600_solid_ps(ChipSet, shader + accel_state->solid_ps_offset / 4);

    /*  copy vs --------------------------------------- */
    accel_state->copy_vs_offset = 1024;
    R600_copy_vs(ChipSet, shader + accel_state->copy_vs_offset / 4);

    /*  copy ps --------------------------------------- */
    accel_state->copy_ps_offset = 1536;
    R600_copy_ps(ChipSet, shader + accel_state->copy_ps_offset / 4);

    /*  comp vs --------------------------------------- */
    accel_state->comp_vs_offset = 2048;
    R600_comp_vs(ChipSet, shader + accel_state->comp_vs_offset / 4);

    /*  comp ps --------------------------------------- */
    accel_state->comp_ps_offset = 2560;
    R600_comp_ps(ChipSet, shader + accel_state->comp_ps_offset / 4);

    /*  comp mask ps --------------------------------------- */
    accel_state->comp_mask_ps_offset = 3072;
    R600_comp_mask_ps(ChipSet, shader + accel_state->comp_mask_ps_offset / 4);

    /*  xv vs --------------------------------------- */
    accel_state->xv_vs_offset = 3584;
    R600_xv_vs(ChipSet, shader + accel_state->xv_vs_offset / 4);

    /*  xv ps --------------------------------------- */
    accel_state->xv_ps_offset = 4096;
    R600_xv_ps(ChipSet, shader + accel_state->xv_ps_offset / 4);

    return TRUE;
}

static Bool
R600PrepareAccess(PixmapPtr pPix, int index)
{
    ScrnInfoPtr pScrn = xf86Screens[pPix->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    /* flush HDP read/write caches */
    OUTREG(HDP_MEM_COHERENCY_FLUSH_CNTL, 0x1);

    return TRUE;
}

static void
R600FinishAccess(PixmapPtr pPix, int index)
{
    ScrnInfoPtr pScrn = xf86Screens[pPix->drawable.pScreen->myNum];
    RADEONInfoPtr info = RADEONPTR(pScrn);
    unsigned char *RADEONMMIO = info->MMIO;

    /* flush HDP read/write caches */
    OUTREG(HDP_MEM_COHERENCY_FLUSH_CNTL, 0x1);

}


Bool
R600DrawInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn =  xf86Screens[pScreen->myNum];
    RADEONInfoPtr info   = RADEONPTR(pScrn);

    if (info->accel_state->exa == NULL) {
	xf86DrvMsg(pScreen->myNum, X_ERROR, "Memory map not set up\n");
	return FALSE;
    }

    info->accel_state->exa->exa_major = EXA_VERSION_MAJOR;
    info->accel_state->exa->exa_minor = EXA_VERSION_MINOR;

    info->accel_state->exa->PrepareSolid = R600PrepareSolid;
    info->accel_state->exa->Solid = R600Solid;
    info->accel_state->exa->DoneSolid = R600DoneSolid;

    info->accel_state->exa->PrepareCopy = R600PrepareCopy;
    info->accel_state->exa->Copy = R600Copy;
    info->accel_state->exa->DoneCopy = R600DoneCopy;

    info->accel_state->exa->MarkSync = R600MarkSync;
    info->accel_state->exa->WaitMarker = R600Sync;

    info->accel_state->exa->PrepareAccess = R600PrepareAccess;
    info->accel_state->exa->FinishAccess = R600FinishAccess;

    /* AGP seems to have problems with gart transfers */
    if (info->accelDFS) {
	info->accel_state->exa->UploadToScreen = R600UploadToScreen;
	info->accel_state->exa->DownloadFromScreen = R600DownloadFromScreen;
    }

    info->accel_state->exa->flags = EXA_OFFSCREEN_PIXMAPS;
#ifdef EXA_SUPPORTS_PREPARE_AUX
    info->accel_state->exa->flags |= EXA_SUPPORTS_PREPARE_AUX;
#endif
    info->accel_state->exa->pixmapOffsetAlign = 256;
    info->accel_state->exa->pixmapPitchAlign = 256;

    info->accel_state->exa->CheckComposite = R600CheckComposite;
    info->accel_state->exa->PrepareComposite = R600PrepareComposite;
    info->accel_state->exa->Composite = R600Composite;
    info->accel_state->exa->DoneComposite = R600DoneComposite;

#if EXA_VERSION_MAJOR > 2 || (EXA_VERSION_MAJOR == 2 && EXA_VERSION_MINOR >= 3)
    xf86DrvMsg(pScrn->scrnIndex, X_INFO, "Setting EXA maxPitchBytes\n");

    info->accel_state->exa->maxPitchBytes = 32768;
    info->accel_state->exa->maxX = 8192;
#else
    info->accel_state->exa->maxX = 8192;
#endif
    info->accel_state->exa->maxY = 8192;

    /* not supported yet */
    info->accel_state->vsync = FALSE;

    if (!exaDriverInit(pScreen, info->accel_state->exa)) {
	xfree(info->accel_state->exa);
	return FALSE;
    }

    if (!info->gartLocation)
	return FALSE;

    info->accel_state->XInited3D = FALSE;
    info->accel_state->copy_area = NULL;

    if (!R600AllocShaders(pScrn, pScreen))
	return FALSE;

    if (!R600LoadShaders(pScrn))
	return FALSE;

    exaMarkSync(pScreen);

    return TRUE;

}

