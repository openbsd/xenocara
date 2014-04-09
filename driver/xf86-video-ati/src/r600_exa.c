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
#include "radeon_reg.h"
#include "r600_shader.h"
#include "r600_reg.h"
#include "r600_state.h"
#include "radeon_exa_shared.h"
#include "radeon_vbo.h"

/* #define SHOW_VERTEXES */

Bool
R600SetAccelState(ScrnInfoPtr pScrn,
		  struct r600_accel_object *src0,
		  struct r600_accel_object *src1,
		  struct r600_accel_object *dst,
		  uint32_t vs_offset, uint32_t ps_offset,
		  int rop, Pixel planemask)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    uint32_t pitch_align = 0x7;
    int ret;

    if (src0) {
	memcpy(&accel_state->src_obj[0], src0, sizeof(struct r600_accel_object));
	accel_state->src_size[0] = src0->pitch * src0->height * (src0->bpp/8);
	if (src0->surface)
		accel_state->src_size[0] = src0->surface->bo_size;

	/* bad pitch */
	if (accel_state->src_obj[0].pitch & pitch_align)
	    RADEON_FALLBACK(("Bad src pitch 0x%08x\n", accel_state->src_obj[0].pitch));

    } else {
	memset(&accel_state->src_obj[0], 0, sizeof(struct r600_accel_object));
	accel_state->src_size[0] = 0;
    }

    if (src1) {
	memcpy(&accel_state->src_obj[1], src1, sizeof(struct r600_accel_object));
	accel_state->src_size[1] = src1->pitch * src1->height * (src1->bpp/8);
	if (src1->surface) {
		accel_state->src_size[1] = src1->surface->bo_size;
	}

	/* bad pitch */
	if (accel_state->src_obj[1].pitch & pitch_align)
	    RADEON_FALLBACK(("Bad src pitch 0x%08x\n", accel_state->src_obj[1].pitch));

    } else {
	memset(&accel_state->src_obj[1], 0, sizeof(struct r600_accel_object));
	accel_state->src_size[1] = 0;
    }

    if (dst) {
	memcpy(&accel_state->dst_obj, dst, sizeof(struct r600_accel_object));
	accel_state->dst_size = dst->pitch * dst->height * (dst->bpp/8);
	if (dst->surface) {
		accel_state->dst_size = dst->surface->bo_size;
	} else
	{
		accel_state->dst_obj.tiling_flags = 0;
	}
	if (accel_state->dst_obj.pitch & pitch_align)
	    RADEON_FALLBACK(("Bad dst pitch 0x%08x\n", accel_state->dst_obj.pitch));

    } else {
	memset(&accel_state->dst_obj, 0, sizeof(struct r600_accel_object));
	accel_state->dst_size = 0;
    }

    if (CS_FULL(info->cs))
	radeon_cs_flush_indirect(pScrn);

    accel_state->rop = rop;
    accel_state->planemask = planemask;

    accel_state->vs_size = 512;
    accel_state->ps_size = 512;
    accel_state->vs_mc_addr = vs_offset;
    accel_state->ps_mc_addr = ps_offset;

    radeon_cs_space_reset_bos(info->cs);
    radeon_cs_space_add_persistent_bo(info->cs, accel_state->shaders_bo,
				      RADEON_GEM_DOMAIN_VRAM, 0);
    if (accel_state->src_obj[0].bo)
	radeon_cs_space_add_persistent_bo(info->cs, accel_state->src_obj[0].bo,
					  accel_state->src_obj[0].domain, 0);
    if (accel_state->src_obj[1].bo)
	radeon_cs_space_add_persistent_bo(info->cs, accel_state->src_obj[1].bo,
					  accel_state->src_obj[1].domain, 0);
    if (accel_state->dst_obj.bo)
	radeon_cs_space_add_persistent_bo(info->cs, accel_state->dst_obj.bo,
					  0, accel_state->dst_obj.domain);
    ret = radeon_cs_space_check(info->cs);
    if (ret)
	RADEON_FALLBACK(("Not enough RAM to hw accel operation\n"));

    return TRUE;
}

static Bool
R600PrepareSolid(PixmapPtr pPix, int alu, Pixel pm, Pixel fg)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPix->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    cb_config_t     cb_conf;
    shader_config_t vs_conf, ps_conf;
    uint32_t a, r, g, b;
    float ps_alu_consts[4];
    struct r600_accel_object dst;

    if (!RADEONCheckBPP(pPix->drawable.bitsPerPixel))
	RADEON_FALLBACK(("R600CheckDatatype failed\n"));
    if (!RADEONValidPM(pm, pPix->drawable.bitsPerPixel))
	RADEON_FALLBACK(("invalid planemask\n"));

    dst.bo = radeon_get_pixmap_bo(pPix);
    dst.tiling_flags = radeon_get_pixmap_tiling(pPix);
    dst.surface = radeon_get_pixmap_surface(pPix);

    dst.pitch = exaGetPixmapPitch(pPix) / (pPix->drawable.bitsPerPixel / 8);
    dst.width = pPix->drawable.width;
    dst.height = pPix->drawable.height;
    dst.bpp = pPix->drawable.bitsPerPixel;
    dst.domain = RADEON_GEM_DOMAIN_VRAM;

    if (!R600SetAccelState(pScrn,
			   NULL,
			   NULL,
			   &dst,
			   accel_state->solid_vs_offset, accel_state->solid_ps_offset,
			   alu, pm))
	return FALSE;

    CLEAR (cb_conf);
    CLEAR (vs_conf);
    CLEAR (ps_conf);

    radeon_vbo_check(pScrn, &accel_state->vbo, 16);
    radeon_cp_start(pScrn);

    r600_set_default_state(pScrn);

    r600_set_generic_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);
    r600_set_screen_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);
    r600_set_window_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);

    /* Shader */
    vs_conf.shader_addr         = accel_state->vs_mc_addr;
    vs_conf.shader_size         = accel_state->vs_size;
    vs_conf.num_gprs            = 2;
    vs_conf.stack_size          = 0;
    vs_conf.bo                  = accel_state->shaders_bo;
    r600_vs_setup(pScrn, &vs_conf, RADEON_GEM_DOMAIN_VRAM);

    ps_conf.shader_addr         = accel_state->ps_mc_addr;
    ps_conf.shader_size         = accel_state->ps_size;
    ps_conf.num_gprs            = 1;
    ps_conf.stack_size          = 0;
    ps_conf.uncached_first_inst = 1;
    ps_conf.clamp_consts        = 0;
    ps_conf.export_mode         = 2;
    ps_conf.bo                  = accel_state->shaders_bo;
    r600_ps_setup(pScrn, &ps_conf, RADEON_GEM_DOMAIN_VRAM);

    cb_conf.id = 0;
    cb_conf.w = accel_state->dst_obj.pitch;
    cb_conf.h = accel_state->dst_obj.height;
    cb_conf.base = 0;
    cb_conf.bo = accel_state->dst_obj.bo;
    cb_conf.surface = accel_state->dst_obj.surface;

    if (accel_state->dst_obj.bpp == 8) {
	cb_conf.format = COLOR_8;
	cb_conf.comp_swap = 3; /* A */
    } else if (accel_state->dst_obj.bpp == 16) {
	cb_conf.format = COLOR_5_6_5;
	cb_conf.comp_swap = 2; /* RGB */
#if X_BYTE_ORDER == X_BIG_ENDIAN
	cb_conf.endian = ENDIAN_8IN16;
#endif
    } else {
	cb_conf.format = COLOR_8_8_8_8;
	cb_conf.comp_swap = 1; /* ARGB */
#if X_BYTE_ORDER == X_BIG_ENDIAN
	cb_conf.endian = ENDIAN_8IN32;
#endif
    }
    cb_conf.source_format = 1;
    cb_conf.blend_clamp = 1;
    /* Render setup */
    if (accel_state->planemask & 0x000000ff)
	cb_conf.pmask |= 4; /* B */
    if (accel_state->planemask & 0x0000ff00)
	cb_conf.pmask |= 2; /* G */
    if (accel_state->planemask & 0x00ff0000)
	cb_conf.pmask |= 1; /* R */
    if (accel_state->planemask & 0xff000000)
	cb_conf.pmask |= 8; /* A */
    cb_conf.rop = accel_state->rop;
    if (accel_state->dst_obj.tiling_flags == 0)
	cb_conf.array_mode = 0;
    r600_set_render_target(pScrn, &cb_conf, accel_state->dst_obj.domain);

    r600_set_spi(pScrn, 0, 0);

    /* PS alu constants */
    if (accel_state->dst_obj.bpp == 16) {
	r = (fg >> 11) & 0x1f;
	g = (fg >> 5) & 0x3f;
	b = (fg >> 0) & 0x1f;
	ps_alu_consts[0] = (float)r / 31; /* R */
	ps_alu_consts[1] = (float)g / 63; /* G */
	ps_alu_consts[2] = (float)b / 31; /* B */
	ps_alu_consts[3] = 1.0; /* A */
    } else if (accel_state->dst_obj.bpp == 8) {
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
    r600_set_alu_consts(pScrn, SQ_ALU_CONSTANT_ps,
			sizeof(ps_alu_consts) / SQ_ALU_CONSTANT_offset, ps_alu_consts);

    if (accel_state->vsync)
	RADEONVlineHelperClear(pScrn);

    accel_state->dst_pix = pPix;
    accel_state->fg = fg;

    return TRUE;
}

static void
R600DoneSolid(PixmapPtr pPix)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPix->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    if (accel_state->vsync)
	r600_cp_wait_vline_sync(pScrn, pPix,
				accel_state->vline_crtc,
				accel_state->vline_y1,
				accel_state->vline_y2);

    r600_finish_op(pScrn, 8);
}

static void
R600Solid(PixmapPtr pPix, int x1, int y1, int x2, int y2)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPix->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    float *vb;

    if (CS_FULL(info->cs)) {
	R600DoneSolid(info->accel_state->dst_pix);
	radeon_cs_flush_indirect(pScrn);
	R600PrepareSolid(accel_state->dst_pix,
			 accel_state->rop,
			 accel_state->planemask,
			 accel_state->fg);
    }

    if (accel_state->vsync)
	RADEONVlineHelperSet(pScrn, x1, y1, x2, y2);

    vb = radeon_vbo_space(pScrn, &accel_state->vbo, 8);

    vb[0] = (float)x1;
    vb[1] = (float)y1;

    vb[2] = (float)x1;
    vb[3] = (float)y2;

    vb[4] = (float)x2;
    vb[5] = (float)y2;

    radeon_vbo_commit(pScrn, &accel_state->vbo);
}

static void
R600DoPrepareCopy(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    cb_config_t     cb_conf;
    tex_resource_t  tex_res;
    tex_sampler_t   tex_samp;
    shader_config_t vs_conf, ps_conf;

    CLEAR (cb_conf);
    CLEAR (tex_res);
    CLEAR (tex_samp);
    CLEAR (vs_conf);
    CLEAR (ps_conf);

    radeon_vbo_check(pScrn, &accel_state->vbo, 16);
    radeon_cp_start(pScrn);

    r600_set_default_state(pScrn);

    r600_set_generic_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);
    r600_set_screen_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);
    r600_set_window_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);

    /* Shader */
    vs_conf.shader_addr         = accel_state->vs_mc_addr;
    vs_conf.shader_size         = accel_state->vs_size;
    vs_conf.num_gprs            = 2;
    vs_conf.stack_size          = 0;
    vs_conf.bo                  = accel_state->shaders_bo;
    r600_vs_setup(pScrn, &vs_conf, RADEON_GEM_DOMAIN_VRAM);

    ps_conf.shader_addr         = accel_state->ps_mc_addr;
    ps_conf.shader_size         = accel_state->ps_size;
    ps_conf.num_gprs            = 1;
    ps_conf.stack_size          = 0;
    ps_conf.uncached_first_inst = 1;
    ps_conf.clamp_consts        = 0;
    ps_conf.export_mode         = 2;
    ps_conf.bo                  = accel_state->shaders_bo;
    r600_ps_setup(pScrn, &ps_conf, RADEON_GEM_DOMAIN_VRAM);

    /* Texture */
    tex_res.id                  = 0;
    tex_res.w                   = accel_state->src_obj[0].width;
    tex_res.h                   = accel_state->src_obj[0].height;
    tex_res.pitch               = accel_state->src_obj[0].pitch;
    tex_res.depth               = 0;
    tex_res.dim                 = SQ_TEX_DIM_2D;
    tex_res.base                = 0;
    tex_res.mip_base            = 0;
    tex_res.size                = accel_state->src_size[0];
    tex_res.bo                  = accel_state->src_obj[0].bo;
    tex_res.mip_bo              = accel_state->src_obj[0].bo;
    tex_res.surface             = accel_state->src_obj[0].surface;
    if (accel_state->src_obj[0].bpp == 8) {
	tex_res.format              = FMT_8;
	tex_res.dst_sel_x           = SQ_SEL_1; /* R */
	tex_res.dst_sel_y           = SQ_SEL_1; /* G */
	tex_res.dst_sel_z           = SQ_SEL_1; /* B */
	tex_res.dst_sel_w           = SQ_SEL_X; /* A */
    } else if (accel_state->src_obj[0].bpp == 16) {
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
    if (accel_state->src_obj[0].tiling_flags == 0)
	tex_res.tile_mode           = 1;
    r600_set_tex_resource(pScrn, &tex_res, accel_state->src_obj[0].domain);

    tex_samp.id                 = 0;
    tex_samp.clamp_x            = SQ_TEX_CLAMP_LAST_TEXEL;
    tex_samp.clamp_y            = SQ_TEX_CLAMP_LAST_TEXEL;
    tex_samp.clamp_z            = SQ_TEX_WRAP;
    tex_samp.xy_mag_filter      = SQ_TEX_XY_FILTER_POINT;
    tex_samp.xy_min_filter      = SQ_TEX_XY_FILTER_POINT;
    tex_samp.mc_coord_truncate  = 1;
    tex_samp.z_filter           = SQ_TEX_Z_FILTER_NONE;
    tex_samp.mip_filter         = 0;			/* no mipmap */
    r600_set_tex_sampler(pScrn, &tex_samp);

    cb_conf.id = 0;
    cb_conf.w = accel_state->dst_obj.pitch;
    cb_conf.h = accel_state->dst_obj.height;
    cb_conf.base = 0;
    cb_conf.bo = accel_state->dst_obj.bo;
    cb_conf.surface = accel_state->dst_obj.surface;
    if (accel_state->dst_obj.bpp == 8) {
	cb_conf.format = COLOR_8;
	cb_conf.comp_swap = 3; /* A */
    } else if (accel_state->dst_obj.bpp == 16) {
	cb_conf.format = COLOR_5_6_5;
	cb_conf.comp_swap = 2; /* RGB */
    } else {
	cb_conf.format = COLOR_8_8_8_8;
	cb_conf.comp_swap = 1; /* ARGB */
    }
    cb_conf.source_format = 1;
    cb_conf.blend_clamp = 1;

    /* Render setup */
    if (accel_state->planemask & 0x000000ff)
	cb_conf.pmask |= 4; /* B */
    if (accel_state->planemask & 0x0000ff00)
	cb_conf.pmask |= 2; /* G */
    if (accel_state->planemask & 0x00ff0000)
	cb_conf.pmask |= 1; /* R */
    if (accel_state->planemask & 0xff000000)
	cb_conf.pmask |= 8; /* A */
    cb_conf.rop = accel_state->rop;
    if (accel_state->dst_obj.tiling_flags == 0)
	cb_conf.array_mode = 0;
    r600_set_render_target(pScrn, &cb_conf, accel_state->dst_obj.domain);

    r600_set_spi(pScrn, (1 - 1), 1);

}

static void
R600DoCopy(ScrnInfoPtr pScrn)
{
    r600_finish_op(pScrn, 16);
}

static void
R600DoCopyVline(PixmapPtr pPix)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPix->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    if (accel_state->vsync)
	r600_cp_wait_vline_sync(pScrn, pPix,
				accel_state->vline_crtc,
				accel_state->vline_y1,
				accel_state->vline_y2);

    r600_finish_op(pScrn, 16);
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

    vb = radeon_vbo_space(pScrn, &accel_state->vbo, 16);

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

    radeon_vbo_commit(pScrn, &accel_state->vbo);
}

static Bool
R600PrepareCopy(PixmapPtr pSrc,   PixmapPtr pDst,
		int xdir, int ydir,
		int rop,
		Pixel planemask)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    struct r600_accel_object src_obj, dst_obj;

    if (!RADEONCheckBPP(pSrc->drawable.bitsPerPixel))
	RADEON_FALLBACK(("R600CheckDatatype src failed\n"));
    if (!RADEONCheckBPP(pDst->drawable.bitsPerPixel))
	RADEON_FALLBACK(("R600CheckDatatype dst failed\n"));
    if (!RADEONValidPM(planemask, pDst->drawable.bitsPerPixel))
	RADEON_FALLBACK(("Invalid planemask\n"));

    dst_obj.pitch = exaGetPixmapPitch(pDst) / (pDst->drawable.bitsPerPixel / 8);
    src_obj.pitch = exaGetPixmapPitch(pSrc) / (pSrc->drawable.bitsPerPixel / 8);

    accel_state->same_surface = FALSE;

    src_obj.bo = radeon_get_pixmap_bo(pSrc);
    dst_obj.bo = radeon_get_pixmap_bo(pDst);
    dst_obj.tiling_flags = radeon_get_pixmap_tiling(pDst);
    src_obj.tiling_flags = radeon_get_pixmap_tiling(pSrc);
    src_obj.surface = radeon_get_pixmap_surface(pSrc);
    dst_obj.surface = radeon_get_pixmap_surface(pDst);
    if (radeon_get_pixmap_bo(pSrc) == radeon_get_pixmap_bo(pDst))
	accel_state->same_surface = TRUE;

    src_obj.width = pSrc->drawable.width;
    src_obj.height = pSrc->drawable.height;
    src_obj.bpp = pSrc->drawable.bitsPerPixel;
    src_obj.domain = RADEON_GEM_DOMAIN_VRAM | RADEON_GEM_DOMAIN_GTT;

    dst_obj.width = pDst->drawable.width;
    dst_obj.height = pDst->drawable.height;
    dst_obj.bpp = pDst->drawable.bitsPerPixel;
    if (radeon_get_pixmap_shared(pDst) == TRUE) {
	dst_obj.domain = RADEON_GEM_DOMAIN_GTT;
    } else
	dst_obj.domain = RADEON_GEM_DOMAIN_VRAM;

    if (!R600SetAccelState(pScrn,
			   &src_obj,
			   NULL,
			   &dst_obj,
			   accel_state->copy_vs_offset, accel_state->copy_ps_offset,
			   rop, planemask))
	return FALSE;

    if (accel_state->same_surface == TRUE) {
	unsigned long size = accel_state->dst_obj.surface->bo_size;
	unsigned long align = accel_state->dst_obj.surface->bo_alignment;

	if (accel_state->copy_area_bo) {
	    radeon_bo_unref(accel_state->copy_area_bo);
	    accel_state->copy_area_bo = NULL;
	}
	accel_state->copy_area_bo = radeon_bo_open(info->bufmgr, 0, size, align,
						   RADEON_GEM_DOMAIN_VRAM,
						   0);
	if (accel_state->copy_area_bo == NULL)
	    RADEON_FALLBACK(("temp copy surface alloc failed\n"));
	
	radeon_cs_space_add_persistent_bo(info->cs, accel_state->copy_area_bo,
					  0, RADEON_GEM_DOMAIN_VRAM);
	if (radeon_cs_space_check(info->cs)) {
	    radeon_bo_unref(accel_state->copy_area_bo);
	    accel_state->copy_area_bo = NULL;
	    return FALSE;
	}
	accel_state->copy_area = (void*)accel_state->copy_area_bo;
    } else
	R600DoPrepareCopy(pScrn);

    if (accel_state->vsync)
	RADEONVlineHelperClear(pScrn);

    accel_state->dst_pix = pDst;
    accel_state->src_pix = pSrc;
    accel_state->xdir = xdir;
    accel_state->ydir = ydir;

    return TRUE;
}

static void
R600DoneCopy(PixmapPtr pDst)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    if (!accel_state->same_surface)
	R600DoCopyVline(pDst);

    if (accel_state->copy_area) {
	accel_state->copy_area = NULL;
    }

}

static void
R600Copy(PixmapPtr pDst,
	 int srcX, int srcY,
	 int dstX, int dstY,
	 int w, int h)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    if (accel_state->same_surface && (srcX == dstX) && (srcY == dstY))
	return;

    if (CS_FULL(info->cs)) {
	R600DoneCopy(info->accel_state->dst_pix);
	radeon_cs_flush_indirect(pScrn);
	R600PrepareCopy(accel_state->src_pix,
			accel_state->dst_pix,
			accel_state->xdir,
			accel_state->ydir,
			accel_state->rop,
			accel_state->planemask);
    }

    if (accel_state->vsync)
	RADEONVlineHelperSet(pScrn, dstX, dstY, dstX + w, dstY + h);

    if (accel_state->same_surface &&
	    (srcX + w <= dstX || dstX + w <= srcX || srcY + h <= dstY || dstY + h <= srcY)) {
	R600DoPrepareCopy(pScrn);
	R600AppendCopyVertex(pScrn, srcX, srcY, dstX, dstY, w, h);
	R600DoCopyVline(pDst);
    } else if (accel_state->same_surface && accel_state->copy_area) {
	uint32_t orig_dst_domain = accel_state->dst_obj.domain;
	uint32_t orig_src_domain = accel_state->src_obj[0].domain;
	uint32_t orig_src_tiling_flags = accel_state->src_obj[0].tiling_flags;
	uint32_t orig_dst_tiling_flags = accel_state->dst_obj.tiling_flags;
	struct radeon_bo *orig_bo = accel_state->dst_obj.bo;
	int orig_rop = accel_state->rop;

	/* src to tmp */
	accel_state->dst_obj.domain = RADEON_GEM_DOMAIN_VRAM;
	accel_state->dst_obj.bo = accel_state->copy_area_bo;
	accel_state->dst_obj.tiling_flags = 0;
	accel_state->rop = 3;
	R600DoPrepareCopy(pScrn);
	R600AppendCopyVertex(pScrn, srcX, srcY, dstX, dstY, w, h);
	R600DoCopy(pScrn);

	/* tmp to dst */
	accel_state->src_obj[0].domain = RADEON_GEM_DOMAIN_VRAM;
	accel_state->src_obj[0].bo = accel_state->copy_area_bo;
	accel_state->src_obj[0].tiling_flags = 0;
	accel_state->dst_obj.domain = orig_dst_domain;
	accel_state->dst_obj.bo = orig_bo;
	accel_state->dst_obj.tiling_flags = orig_dst_tiling_flags;
	accel_state->rop = orig_rop;
	R600DoPrepareCopy(pScrn);
	R600AppendCopyVertex(pScrn, dstX, dstY, dstX, dstY, w, h);
	R600DoCopyVline(pDst);

	/* restore state */
	accel_state->src_obj[0].domain = orig_src_domain;
	accel_state->src_obj[0].bo = orig_bo;
	accel_state->src_obj[0].tiling_flags = orig_src_tiling_flags;
    } else
	R600AppendCopyVertex(pScrn, srcX, srcY, dstX, dstY, w, h);

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
    {PICT_b8g8r8a8,	FMT_8_8_8_8},
    {PICT_b8g8r8x8,	FMT_8_8_8_8},
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
    case PICT_a8b8g8r8:
    case PICT_x8b8g8r8:
    case PICT_b8g8r8a8:
    case PICT_b8g8r8x8:
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
    unsigned int repeatType = pPict->repeat ? pPict->repeatType : RepeatNone;
    unsigned int i;

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
    if (pPict->transform != 0 && repeatType == RepeatNone && PICT_FORMAT_A(pPict->format) == 0) {
	if (!(((op == PictOpSrc) || (op == PictOpClear)) && (PICT_FORMAT_A(pDstPict->format) == 0)))
	    RADEON_FALLBACK(("REPEAT_NONE unsupported for transformed xRGB source\n"));
    }

    if (!radeon_transform_is_affine_or_scaled(pPict->transform))
	RADEON_FALLBACK(("non-affine transforms not supported\n"));

    return TRUE;
}

static Bool R600TextureSetup(PicturePtr pPict, PixmapPtr pPix,
					int unit)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pPix->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    unsigned int repeatType;
    unsigned int i;
    tex_resource_t  tex_res;
    tex_sampler_t   tex_samp;
    int pix_r, pix_g, pix_b, pix_a;
    float vs_alu_consts[8];

    CLEAR (tex_res);
    CLEAR (tex_samp);

    for (i = 0; i < sizeof(R600TexFormats) / sizeof(R600TexFormats[0]); i++) {
	if (R600TexFormats[i].fmt == pPict->format)
	    break;
    }

    /* Texture */
    if (pPict->pDrawable) {
	tex_res.w               = pPict->pDrawable->width;
	tex_res.h               = pPict->pDrawable->height;
	repeatType              = pPict->repeat ? pPict->repeatType : RepeatNone;
    } else {
	tex_res.w               = 1;
	tex_res.h               = 1;
	repeatType              = RepeatNormal;
    }
    tex_res.id                  = unit;
    tex_res.pitch               = accel_state->src_obj[unit].pitch;
    tex_res.depth               = 0;
    tex_res.dim                 = SQ_TEX_DIM_2D;
    tex_res.base                = 0;
    tex_res.mip_base            = 0;
    tex_res.size                = accel_state->src_size[unit];
    tex_res.format              = R600TexFormats[i].card_fmt;
    tex_res.bo                  = accel_state->src_obj[unit].bo;
    tex_res.mip_bo              = accel_state->src_obj[unit].bo;
    tex_res.surface             = accel_state->src_obj[unit].surface;
    tex_res.request_size        = 1;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    switch (accel_state->src_obj[unit].bpp) {
    case 16:
	tex_res.endian = SQ_ENDIAN_8IN16;
	break;
    case 32:
	tex_res.endian = SQ_ENDIAN_8IN32;
	break;
    default :
	break;
    }
#endif

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
    case PICT_b8g8r8a8:
	pix_r = SQ_SEL_Y; /* R */
	pix_g = SQ_SEL_Z; /* G */
	pix_b = SQ_SEL_W; /* B */
	pix_a = SQ_SEL_X; /* A */
	break;
    case PICT_b8g8r8x8:
	pix_r = SQ_SEL_Y; /* R */
	pix_g = SQ_SEL_Z; /* G */
	pix_b = SQ_SEL_W; /* B */
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
	if (!accel_state->msk_pic) {
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
    if (accel_state->src_obj[unit].tiling_flags == 0)
	tex_res.tile_mode           = 1;
    r600_set_tex_resource(pScrn, &tex_res, accel_state->src_obj[unit].domain);

    tex_samp.id                 = unit;
    tex_samp.border_color       = SQ_TEX_BORDER_COLOR_TRANS_BLACK;

    switch (repeatType) {
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
	RADEON_FALLBACK(("Bad repeat 0x%x\n", repeatType));
    }

    switch (pPict->filter) {
    case PictFilterNearest:
	tex_samp.xy_mag_filter      = SQ_TEX_XY_FILTER_POINT;
	tex_samp.xy_min_filter      = SQ_TEX_XY_FILTER_POINT;
	tex_samp.mc_coord_truncate  = 1;
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
    r600_set_tex_sampler(pScrn, &tex_samp);

    if (pPict->transform != 0) {
	accel_state->is_transform[unit] = TRUE;
	accel_state->transform[unit] = pPict->transform;

	vs_alu_consts[0] = xFixedToFloat(pPict->transform->matrix[0][0]);
	vs_alu_consts[1] = xFixedToFloat(pPict->transform->matrix[0][1]);
	vs_alu_consts[2] = xFixedToFloat(pPict->transform->matrix[0][2]);
	vs_alu_consts[3] = 1.0 / tex_res.w;

	vs_alu_consts[4] = xFixedToFloat(pPict->transform->matrix[1][0]);
	vs_alu_consts[5] = xFixedToFloat(pPict->transform->matrix[1][1]);
	vs_alu_consts[6] = xFixedToFloat(pPict->transform->matrix[1][2]);
	vs_alu_consts[7] = 1.0 / tex_res.h;
    } else {
	accel_state->is_transform[unit] = FALSE;

	vs_alu_consts[0] = 1.0;
	vs_alu_consts[1] = 0.0;
	vs_alu_consts[2] = 0.0;
	vs_alu_consts[3] = 1.0 / tex_res.w;

	vs_alu_consts[4] = 0.0;
	vs_alu_consts[5] = 1.0;
	vs_alu_consts[6] = 0.0;
	vs_alu_consts[7] = 1.0 / tex_res.h;
    }

    /* VS alu constants */
    r600_set_alu_consts(pScrn, SQ_ALU_CONSTANT_vs + (unit * 2),
			sizeof(vs_alu_consts) / SQ_ALU_CONSTANT_offset, vs_alu_consts);

    return TRUE;
}

static Bool R600CheckComposite(int op, PicturePtr pSrcPicture, PicturePtr pMaskPicture,
			       PicturePtr pDstPicture)
{
    uint32_t tmp1;
    PixmapPtr pSrcPixmap, pDstPixmap;

    /* Check for unsupported compositing operations. */
    if (op >= (int) (sizeof(R600BlendOp) / sizeof(R600BlendOp[0])))
	RADEON_FALLBACK(("Unsupported Composite op 0x%x\n", op));

    if (pSrcPicture->pDrawable) {
	pSrcPixmap = RADEONGetDrawablePixmap(pSrcPicture->pDrawable);

	if (pSrcPixmap->drawable.width >= 8192 ||
	    pSrcPixmap->drawable.height >= 8192) {
	    RADEON_FALLBACK(("Source w/h too large (%d,%d).\n",
			     pSrcPixmap->drawable.width,
			     pSrcPixmap->drawable.height));
	}

	if (!R600CheckCompositeTexture(pSrcPicture, pDstPicture, op, 0))
	    return FALSE;
    } else if (pSrcPicture->pSourcePict->type != SourcePictTypeSolidFill)
	RADEON_FALLBACK(("Gradient pictures not supported yet\n"));

    pDstPixmap = RADEONGetDrawablePixmap(pDstPicture->pDrawable);

    if (pDstPixmap->drawable.width >= 8192 ||
	pDstPixmap->drawable.height >= 8192) {
	RADEON_FALLBACK(("Dest w/h too large (%d,%d).\n",
			 pDstPixmap->drawable.width,
			 pDstPixmap->drawable.height));
    }

    if (pMaskPicture) {
	PixmapPtr pMaskPixmap;

	if (pMaskPicture->pDrawable) {
	    pMaskPixmap = RADEONGetDrawablePixmap(pMaskPicture->pDrawable);

	    if (pMaskPixmap->drawable.width >= 8192 ||
		pMaskPixmap->drawable.height >= 8192) {
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
	} else if (pMaskPicture->pSourcePict->type != SourcePictTypeSolidFill)
	    RADEON_FALLBACK(("Gradient pictures not supported yet\n"));
    }

    if (!R600GetDestFormat(pDstPicture, &tmp1))
	return FALSE;

    return TRUE;

}

static Bool R600PrepareComposite(int op, PicturePtr pSrcPicture,
				 PicturePtr pMaskPicture, PicturePtr pDstPicture,
				 PixmapPtr pSrc, PixmapPtr pMask, PixmapPtr pDst)
{
    ScreenPtr pScreen = pDst->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    uint32_t dst_format;
    cb_config_t cb_conf;
    shader_config_t vs_conf, ps_conf;
    struct r600_accel_object src_obj, mask_obj, dst_obj;

    if (pDst->drawable.bitsPerPixel < 8 || (pSrc && pSrc->drawable.bitsPerPixel < 8))
	return FALSE;

    if (!pSrc) {
	pSrc = RADEONSolidPixmap(pScreen, pSrcPicture->pSourcePict->solidFill.color);
	if (!pSrc)
	    RADEON_FALLBACK(("Failed to create solid scratch pixmap\n"));
    }

    dst_obj.bo = radeon_get_pixmap_bo(pDst);
    src_obj.bo = radeon_get_pixmap_bo(pSrc);
    dst_obj.tiling_flags = radeon_get_pixmap_tiling(pDst);
    src_obj.tiling_flags = radeon_get_pixmap_tiling(pSrc);
    dst_obj.surface = radeon_get_pixmap_surface(pDst);
    src_obj.surface = radeon_get_pixmap_surface(pSrc);

    src_obj.pitch = exaGetPixmapPitch(pSrc) / (pSrc->drawable.bitsPerPixel / 8);
    dst_obj.pitch = exaGetPixmapPitch(pDst) / (pDst->drawable.bitsPerPixel / 8);

    src_obj.width = pSrc->drawable.width;
    src_obj.height = pSrc->drawable.height;
    src_obj.bpp = pSrc->drawable.bitsPerPixel;
    src_obj.domain = RADEON_GEM_DOMAIN_VRAM | RADEON_GEM_DOMAIN_GTT;

    dst_obj.width = pDst->drawable.width;
    dst_obj.height = pDst->drawable.height;
    dst_obj.bpp = pDst->drawable.bitsPerPixel;
    if (radeon_get_pixmap_shared(pDst) == TRUE)
	dst_obj.domain = RADEON_GEM_DOMAIN_GTT;
    else
	dst_obj.domain = RADEON_GEM_DOMAIN_VRAM;

    if (pMaskPicture) {
	if (!pMask) {
	    pMask = RADEONSolidPixmap(pScreen, pMaskPicture->pSourcePict->solidFill.color);
	    if (!pMask) {
		if (!pSrcPicture->pDrawable)
		    pScreen->DestroyPixmap(pSrc);
		RADEON_FALLBACK(("Failed to create solid scratch pixmap\n"));
	    }
	}

	mask_obj.bo = radeon_get_pixmap_bo(pMask);
	mask_obj.tiling_flags = radeon_get_pixmap_tiling(pMask);
	mask_obj.surface = radeon_get_pixmap_surface(pMask);

	mask_obj.pitch = exaGetPixmapPitch(pMask) / (pMask->drawable.bitsPerPixel / 8);

	mask_obj.width = pMask->drawable.width;
	mask_obj.height = pMask->drawable.height;
	mask_obj.bpp = pMask->drawable.bitsPerPixel;
	mask_obj.domain = RADEON_GEM_DOMAIN_VRAM | RADEON_GEM_DOMAIN_GTT;

	if (!R600SetAccelState(pScrn,
			       &src_obj,
			       &mask_obj,
			       &dst_obj,
			       accel_state->comp_vs_offset, accel_state->comp_ps_offset,
			       3, 0xffffffff))
	    return FALSE;

	accel_state->msk_pic = pMaskPicture;
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
	if (!R600SetAccelState(pScrn,
			       &src_obj,
			       NULL,
			       &dst_obj,
			       accel_state->comp_vs_offset, accel_state->comp_ps_offset,
			       3, 0xffffffff))
	    return FALSE;

	accel_state->msk_pic = NULL;
	accel_state->component_alpha = FALSE;
	accel_state->src_alpha = FALSE;
    }

    if (!R600GetDestFormat(pDstPicture, &dst_format))
	return FALSE;

    CLEAR (cb_conf);
    CLEAR (vs_conf);
    CLEAR (ps_conf);

    if (pMask)
        radeon_vbo_check(pScrn, &accel_state->vbo, 24);
    else
        radeon_vbo_check(pScrn, &accel_state->vbo, 16);

    radeon_cp_start(pScrn);

    r600_set_default_state(pScrn);

    r600_set_generic_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);
    r600_set_screen_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);
    r600_set_window_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);

    if (!R600TextureSetup(pSrcPicture, pSrc, 0)) {
        R600IBDiscard(pScrn);
        return FALSE;
    }

    if (pMask) {
        if (!R600TextureSetup(pMaskPicture, pMask, 1)) {
            R600IBDiscard(pScrn);
            return FALSE;
        }
    } else
        accel_state->is_transform[1] = FALSE;

    if (pMask) {
	r600_set_bool_consts(pScrn, SQ_BOOL_CONST_vs, (1 << 0));
	r600_set_bool_consts(pScrn, SQ_BOOL_CONST_ps, (1 << 0));
    } else {
	r600_set_bool_consts(pScrn, SQ_BOOL_CONST_vs, (0 << 0));
	r600_set_bool_consts(pScrn, SQ_BOOL_CONST_ps, (0 << 0));
    }

    /* Shader */
    vs_conf.shader_addr         = accel_state->vs_mc_addr;
    vs_conf.shader_size         = accel_state->vs_size;
    vs_conf.num_gprs            = 5;
    vs_conf.stack_size          = 1;
    vs_conf.bo                  = accel_state->shaders_bo;
    r600_vs_setup(pScrn, &vs_conf, RADEON_GEM_DOMAIN_VRAM);

    ps_conf.shader_addr         = accel_state->ps_mc_addr;
    ps_conf.shader_size         = accel_state->ps_size;
    ps_conf.num_gprs            = 3;
    ps_conf.stack_size          = 1;
    ps_conf.uncached_first_inst = 1;
    ps_conf.clamp_consts        = 0;
    ps_conf.export_mode         = 2;
    ps_conf.bo                  = accel_state->shaders_bo;
    r600_ps_setup(pScrn, &ps_conf, RADEON_GEM_DOMAIN_VRAM);

    cb_conf.id = 0;
    cb_conf.w = accel_state->dst_obj.pitch;
    cb_conf.h = accel_state->dst_obj.height;
    cb_conf.base = 0;
    cb_conf.format = dst_format;
    cb_conf.bo = accel_state->dst_obj.bo;
    cb_conf.surface = accel_state->dst_obj.surface;

    switch (pDstPicture->format) {
    case PICT_a8r8g8b8:
    case PICT_x8r8g8b8:
    case PICT_a1r5g5b5:
    case PICT_x1r5g5b5:
    default:
	cb_conf.comp_swap = 1; /* ARGB */
	break;
    case PICT_a8b8g8r8:
    case PICT_x8b8g8r8:
	cb_conf.comp_swap = 0; /* ABGR */
	break;
    case PICT_b8g8r8a8:
    case PICT_b8g8r8x8:
	cb_conf.comp_swap = 3; /* BGRA */
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
    cb_conf.blendcntl = R600GetBlendCntl(op, pMaskPicture, pDstPicture->format);
    cb_conf.blend_enable = 1;
    cb_conf.pmask = 0xf;
    cb_conf.rop = 3;
    if (accel_state->dst_obj.tiling_flags == 0)
	cb_conf.array_mode = 0;
#if X_BYTE_ORDER == X_BIG_ENDIAN
    switch (dst_obj.bpp) {
    case 16:
	cb_conf.endian = ENDIAN_8IN16;
	break;
    case 32:
	cb_conf.endian = ENDIAN_8IN32;
	break;
    default:
	break;
    }
#endif
    r600_set_render_target(pScrn, &cb_conf, accel_state->dst_obj.domain);

    if (pMask)
	r600_set_spi(pScrn, (2 - 1), 2);
    else
	r600_set_spi(pScrn, (1 - 1), 1);

    if (accel_state->vsync)
	RADEONVlineHelperClear(pScrn);

    accel_state->composite_op = op;
    accel_state->dst_pic = pDstPicture;
    accel_state->src_pic = pSrcPicture;
    accel_state->dst_pix = pDst;
    accel_state->msk_pix = pMask;
    accel_state->src_pix = pSrc;

    return TRUE;
}

static void R600FinishComposite(ScrnInfoPtr pScrn, PixmapPtr pDst,
				struct radeon_accel_state *accel_state)
{
    int vtx_size;

    if (accel_state->vsync)
       r600_cp_wait_vline_sync(pScrn, pDst,
			       accel_state->vline_crtc,
			       accel_state->vline_y1,
			       accel_state->vline_y2);

    vtx_size = accel_state->msk_pic ? 24 : 16;

    r600_finish_op(pScrn, vtx_size);
}

static void R600DoneComposite(PixmapPtr pDst)
{
    ScreenPtr pScreen = pDst->drawable.pScreen;
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    R600FinishComposite(pScrn, pDst, accel_state);

    if (!accel_state->src_pic->pDrawable)
	pScreen->DestroyPixmap(accel_state->src_pix);

    if (accel_state->msk_pic && !accel_state->msk_pic->pDrawable)
	pScreen->DestroyPixmap(accel_state->msk_pix);
}

static void R600Composite(PixmapPtr pDst,
			  int srcX, int srcY,
			  int maskX, int maskY,
			  int dstX, int dstY,
			  int w, int h)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    float *vb;

    /* ErrorF("R600Composite (%d,%d) (%d,%d) (%d,%d) (%d,%d)\n",
       srcX, srcY, maskX, maskY,dstX, dstY, w, h); */

    if (CS_FULL(info->cs)) {
	R600FinishComposite(pScrn, pDst, info->accel_state);
	radeon_cs_flush_indirect(pScrn);
	R600PrepareComposite(info->accel_state->composite_op,
			     info->accel_state->src_pic,
			     info->accel_state->msk_pic,
			     info->accel_state->dst_pic,
			     info->accel_state->src_pix,
			     info->accel_state->msk_pix,
			     info->accel_state->dst_pix);
    }

    if (accel_state->vsync)
	RADEONVlineHelperSet(pScrn, dstX, dstY, dstX + w, dstY + h);

    if (accel_state->msk_pic) {

	vb = radeon_vbo_space(pScrn, &accel_state->vbo, 24);

	vb[0] = (float)dstX;
	vb[1] = (float)dstY;
	vb[2] = (float)srcX;
	vb[3] = (float)srcY;
	vb[4] = (float)maskX;
	vb[5] = (float)maskY;

	vb[6] = (float)dstX;
	vb[7] = (float)(dstY + h);
	vb[8] = (float)srcX;
	vb[9] = (float)(srcY + h);
	vb[10] = (float)maskX;
	vb[11] = (float)(maskY + h);

	vb[12] = (float)(dstX + w);
	vb[13] = (float)(dstY + h);
	vb[14] = (float)(srcX + w);
	vb[15] = (float)(srcY + h);
	vb[16] = (float)(maskX + w);
	vb[17] = (float)(maskY + h);

	radeon_vbo_commit(pScrn, &accel_state->vbo);

    } else {

	vb = radeon_vbo_space(pScrn, &accel_state->vbo, 16);

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

	radeon_vbo_commit(pScrn, &accel_state->vbo);
    }


}

static Bool
R600UploadToScreenCS(PixmapPtr pDst, int x, int y, int w, int h,
		     char *src, int src_pitch)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pDst->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    struct radeon_exa_pixmap_priv *driver_priv;
    struct radeon_bo *scratch = NULL;
    struct radeon_bo *copy_dst;
    unsigned char *dst;
    unsigned size;
    uint32_t dst_domain;
    int bpp = pDst->drawable.bitsPerPixel;
    uint32_t scratch_pitch;
    uint32_t copy_pitch;
    uint32_t dst_pitch_hw = exaGetPixmapPitch(pDst) / (bpp / 8);
    int ret;
    Bool flush = TRUE;
    Bool r;
    int i;
    struct r600_accel_object src_obj, dst_obj;
    uint32_t height, base_align;

    if (bpp < 8)
	return FALSE;

    driver_priv = exaGetPixmapDriverPrivate(pDst);
    if (!driver_priv || !driver_priv->bo)
	return FALSE;

    /* If we know the BO won't be busy / in VRAM, don't bother with a scratch */
    copy_dst = driver_priv->bo;
    copy_pitch = pDst->devKind;
    if (!(driver_priv->tiling_flags & (RADEON_TILING_MACRO | RADEON_TILING_MICRO))) {
	if (!radeon_bo_is_referenced_by_cs(driver_priv->bo, info->cs)) {
	    flush = FALSE;
	    if (!radeon_bo_is_busy(driver_priv->bo, &dst_domain) &&
		!(dst_domain & RADEON_GEM_DOMAIN_VRAM))
		goto copy;
	}
	/* use cpu copy for fast fb access */
	if (info->is_fast_fb)
	    goto copy;
    }

    scratch_pitch = RADEON_ALIGN(w, drmmode_get_pitch_align(pScrn, (bpp / 8), 0));
    height = RADEON_ALIGN(h, drmmode_get_height_align(pScrn, 0));
    base_align = drmmode_get_base_align(pScrn, (bpp / 8), 0);
    size = scratch_pitch * height * (bpp / 8);
    scratch = radeon_bo_open(info->bufmgr, 0, size, base_align, RADEON_GEM_DOMAIN_GTT, 0);
    if (scratch == NULL) {
	goto copy;
    }

    src_obj.pitch = scratch_pitch;
    src_obj.width = w;
    src_obj.height = h;
    src_obj.bpp = bpp;
    src_obj.domain = RADEON_GEM_DOMAIN_GTT;
    src_obj.bo = scratch;
    src_obj.tiling_flags = 0;
    src_obj.surface = NULL;

    dst_obj.pitch = dst_pitch_hw;
    dst_obj.width = pDst->drawable.width;
    dst_obj.height = pDst->drawable.height;
    dst_obj.bpp = bpp;
    dst_obj.domain = RADEON_GEM_DOMAIN_VRAM;
    dst_obj.bo = radeon_get_pixmap_bo(pDst);
    dst_obj.tiling_flags = radeon_get_pixmap_tiling(pDst);
    dst_obj.surface = radeon_get_pixmap_surface(pDst);

    if (!R600SetAccelState(pScrn,
			   &src_obj,
			   NULL,
			   &dst_obj,
			   accel_state->copy_vs_offset, accel_state->copy_ps_offset,
			   3, 0xffffffff)) {
        goto copy;
    }
    copy_dst = scratch;
    copy_pitch = scratch_pitch * (bpp / 8);
    flush = FALSE;

copy:
    if (flush)
	radeon_cs_flush_indirect(pScrn);

    ret = radeon_bo_map(copy_dst, 0);
    if (ret) {
        r = FALSE;
        goto out;
    }
    r = TRUE;
    size = w * bpp / 8;
    dst = copy_dst->ptr;
    if (copy_dst == driver_priv->bo)
	dst += y * copy_pitch + x * bpp / 8;
    for (i = 0; i < h; i++) {
        memcpy(dst + i * copy_pitch, src, size);
        src += src_pitch;
    }
    radeon_bo_unmap(copy_dst);

    if (copy_dst == scratch) {
	if (info->accel_state->vsync)
	    RADEONVlineHelperSet(pScrn, x, y, x + w, y + h);

	/* blit from gart to vram */
	R600DoPrepareCopy(pScrn);
	R600AppendCopyVertex(pScrn, 0, 0, x, y, w, h);
	R600DoCopyVline(pDst);
    }

out:
    if (scratch)
	radeon_bo_unref(scratch);
    return r;
}

static Bool
R600DownloadFromScreenCS(PixmapPtr pSrc, int x, int y, int w,
			 int h, char *dst, int dst_pitch)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pSrc->drawable.pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    struct radeon_exa_pixmap_priv *driver_priv;
    struct radeon_bo *scratch = NULL;
    struct radeon_bo *copy_src;
    unsigned size;
    uint32_t src_domain = 0;
    int bpp = pSrc->drawable.bitsPerPixel;
    uint32_t scratch_pitch;
    uint32_t copy_pitch;
    uint32_t src_pitch_hw = exaGetPixmapPitch(pSrc) / (bpp / 8);
    int ret;
    Bool flush = FALSE;
    Bool r;
    struct r600_accel_object src_obj, dst_obj;
    uint32_t height, base_align;

    if (bpp < 8)
	return FALSE;

    driver_priv = exaGetPixmapDriverPrivate(pSrc);
    if (!driver_priv || !driver_priv->bo)
	return FALSE;

    /* If we know the BO won't end up in VRAM anyway, don't bother with a scratch */
    copy_src = driver_priv->bo;
    copy_pitch = pSrc->devKind;
    if (!(driver_priv->tiling_flags & (RADEON_TILING_MACRO | RADEON_TILING_MICRO))) {
	if (radeon_bo_is_referenced_by_cs(driver_priv->bo, info->cs)) {
	    src_domain = radeon_bo_get_src_domain(driver_priv->bo);
	    if ((src_domain & (RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM)) ==
		(RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM))
		src_domain = 0;
	    else /* A write may be scheduled */
		flush = TRUE;
	}

	if (!src_domain)
	    radeon_bo_is_busy(driver_priv->bo, &src_domain);

	if (src_domain & ~(uint32_t)RADEON_GEM_DOMAIN_VRAM)
	    goto copy;
    }

    scratch_pitch = RADEON_ALIGN(w, drmmode_get_pitch_align(pScrn, (bpp / 8), 0));
    height = RADEON_ALIGN(h, drmmode_get_height_align(pScrn, 0));
    base_align = drmmode_get_base_align(pScrn, (bpp / 8), 0);
    size = scratch_pitch * height * (bpp / 8);
    scratch = radeon_bo_open(info->bufmgr, 0, size, base_align, RADEON_GEM_DOMAIN_GTT, 0);
    if (scratch == NULL) {
	goto copy;
    }
    radeon_cs_space_reset_bos(info->cs);
    radeon_cs_space_add_persistent_bo(info->cs, info->accel_state->shaders_bo,
				      RADEON_GEM_DOMAIN_VRAM, 0);
    accel_state->src_obj[0].domain = RADEON_GEM_DOMAIN_GTT | RADEON_GEM_DOMAIN_VRAM;
    radeon_add_pixmap(info->cs, pSrc, info->accel_state->src_obj[0].domain, 0);
    accel_state->dst_obj.domain = RADEON_GEM_DOMAIN_GTT;
    radeon_cs_space_add_persistent_bo(info->cs, scratch, 0, accel_state->dst_obj.domain);
    ret = radeon_cs_space_check(info->cs);
    if (ret) {
        goto copy;
    }

    src_obj.pitch = src_pitch_hw;
    src_obj.width = pSrc->drawable.width;
    src_obj.height = pSrc->drawable.height;
    src_obj.bpp = bpp;
    src_obj.domain = RADEON_GEM_DOMAIN_VRAM | RADEON_GEM_DOMAIN_GTT;
    src_obj.bo = radeon_get_pixmap_bo(pSrc);
    src_obj.tiling_flags = radeon_get_pixmap_tiling(pSrc);
    src_obj.surface = radeon_get_pixmap_surface(pSrc);

    dst_obj.pitch = scratch_pitch;
    dst_obj.width = w;
    dst_obj.height = h;
    dst_obj.bo = scratch;
    dst_obj.bpp = bpp;
    dst_obj.domain = RADEON_GEM_DOMAIN_GTT;
    dst_obj.tiling_flags = 0;
    dst_obj.surface = NULL;

    if (!R600SetAccelState(pScrn,
			   &src_obj,
			   NULL,
			   &dst_obj,
			   accel_state->copy_vs_offset, accel_state->copy_ps_offset,
			   3, 0xffffffff)) {
        goto copy;
    }

    /* blit from vram to gart */
    R600DoPrepareCopy(pScrn);
    R600AppendCopyVertex(pScrn, x, y, 0, 0, w, h);
    R600DoCopy(pScrn);
    copy_src = scratch;
    copy_pitch = scratch_pitch * (bpp / 8);
    flush = TRUE;

copy:
    if (flush)
	radeon_cs_flush_indirect(pScrn);

    ret = radeon_bo_map(copy_src, 0);
    if (ret) {
	ErrorF("failed to map pixmap: %d\n", ret);
        r = FALSE;
        goto out;
    }
    r = TRUE;
    w *= bpp / 8;
    if (copy_src == driver_priv->bo)
	size = y * copy_pitch + x * bpp / 8;
    else
	size = 0;
    while (h--) {
        memcpy(dst, copy_src->ptr + size, w);
        size += copy_pitch;
        dst += dst_pitch;
    }
    radeon_bo_unmap(copy_src);
out:
    if (scratch)
	radeon_bo_unref(scratch);
    return r;
}

static int
R600MarkSync(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    return ++accel_state->exaSyncMarker;

}

static void
R600Sync(ScreenPtr pScreen, int marker)
{
    ScrnInfoPtr pScrn = xf86ScreenToScrn(pScreen);
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    if (accel_state->exaMarkerSynced != marker) {
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

    accel_state->shaders_bo = radeon_bo_open(info->bufmgr, 0, size, 0,
					     RADEON_GEM_DOMAIN_VRAM, 0);
    if (accel_state->shaders_bo == NULL) {
        ErrorF("Allocating shader failed\n");
	return FALSE;
    }
    return TRUE;
}

Bool
R600LoadShaders(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    RADEONChipFamily ChipSet = info->ChipFamily;
    uint32_t *shader;
    int ret;

    ret = radeon_bo_map(accel_state->shaders_bo, 1);
    if (ret) {
        FatalError("failed to map shader %d\n", ret);
	return FALSE;
    }
    shader = accel_state->shaders_bo->ptr;

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

    /*  xv vs --------------------------------------- */
    accel_state->xv_vs_offset = 3072;
    R600_xv_vs(ChipSet, shader + accel_state->xv_vs_offset / 4);

    /*  xv ps --------------------------------------- */
    accel_state->xv_ps_offset = 3584;
    R600_xv_ps(ChipSet, shader + accel_state->xv_ps_offset / 4);

    radeon_bo_unmap(accel_state->shaders_bo);
    return TRUE;
}

Bool
R600DrawInit(ScreenPtr pScreen)
{
    ScrnInfoPtr pScrn =  xf86ScreenToScrn(pScreen);
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

    info->accel_state->exa->CreatePixmap = RADEONEXACreatePixmap;
    info->accel_state->exa->DestroyPixmap = RADEONEXADestroyPixmap;
    info->accel_state->exa->PixmapIsOffscreen = RADEONEXAPixmapIsOffscreen;
    info->accel_state->exa->PrepareAccess = RADEONPrepareAccess_CS;
    info->accel_state->exa->FinishAccess = RADEONFinishAccess_CS;
    info->accel_state->exa->UploadToScreen = R600UploadToScreenCS;
    info->accel_state->exa->DownloadFromScreen = R600DownloadFromScreenCS;
    info->accel_state->exa->CreatePixmap2 = RADEONEXACreatePixmap2;
#if (EXA_VERSION_MAJOR == 2 && EXA_VERSION_MINOR >= 6) 
    info->accel_state->exa->SharePixmapBacking = RADEONEXASharePixmapBacking; 
    info->accel_state->exa->SetSharedPixmapBacking = RADEONEXASetSharedPixmapBacking;
#endif
    info->accel_state->exa->flags = EXA_OFFSCREEN_PIXMAPS | EXA_SUPPORTS_PREPARE_AUX |
	EXA_HANDLES_PIXMAPS | EXA_MIXED_PIXMAPS;
    info->accel_state->exa->pixmapOffsetAlign = 256;
    info->accel_state->exa->pixmapPitchAlign = 256;

    info->accel_state->exa->CheckComposite = R600CheckComposite;
    info->accel_state->exa->PrepareComposite = R600PrepareComposite;
    info->accel_state->exa->Composite = R600Composite;
    info->accel_state->exa->DoneComposite = R600DoneComposite;

    info->accel_state->exa->maxPitchBytes = 32768;
    info->accel_state->exa->maxX = 8192;
    info->accel_state->exa->maxY = 8192;

    /* not supported yet */
    if (xf86ReturnOptValBool(info->Options, OPTION_EXA_VSYNC, FALSE)) {
	xf86DrvMsg(pScrn->scrnIndex, X_INFO, "EXA VSync enabled\n");
	info->accel_state->vsync = TRUE;
    } else
	info->accel_state->vsync = FALSE;

    if (!exaDriverInit(pScreen, info->accel_state->exa)) {
	free(info->accel_state->exa);
	return FALSE;
    }

    info->accel_state->XInited3D = FALSE;
    info->accel_state->src_obj[0].bo = NULL;
    info->accel_state->src_obj[1].bo = NULL;
    info->accel_state->dst_obj.bo = NULL;
    info->accel_state->copy_area_bo = NULL;
    info->accel_state->vbo.vb_start_op = -1;
    info->accel_state->finish_op = r600_finish_op;
    info->accel_state->vbo.verts_per_op = 3;
    RADEONVlineHelperClear(pScrn);

    radeon_vbo_init_lists(pScrn);

    if (!R600AllocShaders(pScrn, pScreen))
	return FALSE;

    if (!R600LoadShaders(pScrn))
	return FALSE;

    exaMarkSync(pScreen);

    return TRUE;

}

