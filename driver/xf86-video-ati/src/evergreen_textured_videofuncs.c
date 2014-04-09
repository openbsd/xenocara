/*
 * Copyright 2010 Advanced Micro Devices, Inc.
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
#include "evergreen_shader.h"
#include "evergreen_reg.h"
#include "evergreen_state.h"

#include "radeon_video.h"

#include <X11/extensions/Xv.h>
#include "fourcc.h"

#include "damage.h"

#include "radeon_exa_shared.h"
#include "radeon_vbo.h"

/* Parameters for ITU-R BT.601 and ITU-R BT.709 colour spaces
   note the difference to the parameters used in overlay are due
   to 10bit vs. float calcs */
static REF_TRANSFORM trans[2] =
{
    {1.1643, 0.0, 1.5960, -0.3918, -0.8129, 2.0172, 0.0}, /* BT.601 */
    {1.1643, 0.0, 1.7927, -0.2132, -0.5329, 2.1124, 0.0}  /* BT.709 */
};

void
EVERGREENDisplayTexturedVideo(ScrnInfoPtr pScrn, RADEONPortPrivPtr pPriv)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    PixmapPtr pPixmap = pPriv->pPixmap;
    BoxPtr pBox = REGION_RECTS(&pPriv->clip);
    int nBox = REGION_NUM_RECTS(&pPriv->clip);
    int dstxoff, dstyoff;
    struct r600_accel_object src_obj, dst_obj;
    cb_config_t     cb_conf;
    tex_resource_t  tex_res;
    tex_sampler_t   tex_samp;
    shader_config_t vs_conf, ps_conf;
    /*
     * y' = y - .0625
     * u' = u - .5
     * v' = v - .5;
     *
     * r = 1.1643 * y' + 0.0     * u' + 1.5958  * v'
     * g = 1.1643 * y' - 0.39173 * u' - 0.81290 * v'
     * b = 1.1643 * y' + 2.017   * u' + 0.0     * v'
     *
     * DP3 might look like the straightforward solution
     * but we'd need to move the texture yuv values in
     * the same reg for this to work. Therefore use MADs.
     * Brightness just adds to the off constant.
     * Contrast is multiplication of luminance.
     * Saturation and hue change the u and v coeffs.
     * Default values (before adjustments - depend on colorspace):
     * yco = 1.1643
     * uco = 0, -0.39173, 2.017
     * vco = 1.5958, -0.8129, 0
     * off = -0.0625 * yco + -0.5 * uco[r] + -0.5 * vco[r],
     *       -0.0625 * yco + -0.5 * uco[g] + -0.5 * vco[g],
     *       -0.0625 * yco + -0.5 * uco[b] + -0.5 * vco[b],
     *
     * temp = MAD(yco, yuv.yyyy, off)
     * temp = MAD(uco, yuv.uuuu, temp)
     * result = MAD(vco, yuv.vvvv, temp)
     */
    /* TODO: calc consts in the shader */
    const float Loff = -0.0627;
    const float Coff = -0.502;
    float uvcosf, uvsinf;
    float yco;
    float uco[3], vco[3], off[3];
    float bright, cont, gamma;
    int ref = pPriv->transform_index;
    float *ps_alu_consts;
    const_config_t ps_const_conf;
    float *vs_alu_consts;
    const_config_t vs_const_conf;

    cont = RTFContrast(pPriv->contrast);
    bright = RTFBrightness(pPriv->brightness);
    gamma = (float)pPriv->gamma / 1000.0;
    uvcosf = RTFSaturation(pPriv->saturation) * cos(RTFHue(pPriv->hue));
    uvsinf = RTFSaturation(pPriv->saturation) * sin(RTFHue(pPriv->hue));
    /* overlay video also does pre-gamma contrast/sat adjust, should we? */

    yco = trans[ref].RefLuma * cont;
    uco[0] = -trans[ref].RefRCr * uvsinf;
    uco[1] = trans[ref].RefGCb * uvcosf - trans[ref].RefGCr * uvsinf;
    uco[2] = trans[ref].RefBCb * uvcosf;
    vco[0] = trans[ref].RefRCr * uvcosf;
    vco[1] = trans[ref].RefGCb * uvsinf + trans[ref].RefGCr * uvcosf;
    vco[2] = trans[ref].RefBCb * uvsinf;
    off[0] = Loff * yco + Coff * (uco[0] + vco[0]) + bright;
    off[1] = Loff * yco + Coff * (uco[1] + vco[1]) + bright;
    off[2] = Loff * yco + Coff * (uco[2] + vco[2]) + bright;

    // XXX
    gamma = 1.0;

    CLEAR (cb_conf);
    CLEAR (tex_res);
    CLEAR (tex_samp);
    CLEAR (vs_conf);
    CLEAR (ps_conf);
    CLEAR (vs_const_conf);
    CLEAR (ps_const_conf);

    dst_obj.bo = radeon_get_pixmap_bo(pPixmap);
    dst_obj.tiling_flags = radeon_get_pixmap_tiling(pPixmap);
    dst_obj.surface = radeon_get_pixmap_surface(pPixmap);

    dst_obj.pitch = exaGetPixmapPitch(pPixmap) / (pPixmap->drawable.bitsPerPixel / 8);

    src_obj.pitch = pPriv->src_pitch;
    src_obj.width = pPriv->w;
    src_obj.height = pPriv->h;
    src_obj.bpp = 16;
    src_obj.domain = RADEON_GEM_DOMAIN_VRAM | RADEON_GEM_DOMAIN_GTT;
    src_obj.bo = pPriv->src_bo[pPriv->currentBuffer];
    src_obj.tiling_flags = 0;
    src_obj.surface = NULL;

    dst_obj.width = pPixmap->drawable.width;
    dst_obj.height = pPixmap->drawable.height;
    dst_obj.bpp = pPixmap->drawable.bitsPerPixel;
    dst_obj.domain = RADEON_GEM_DOMAIN_VRAM;

    if (!R600SetAccelState(pScrn,
			   &src_obj,
			   NULL,
			   &dst_obj,
			   accel_state->xv_vs_offset, accel_state->xv_ps_offset,
			   3, 0xffffffff))
	return;

#ifdef COMPOSITE
    dstxoff = -pPixmap->screen_x + pPixmap->drawable.x;
    dstyoff = -pPixmap->screen_y + pPixmap->drawable.y;
#else
    dstxoff = 0;
    dstyoff = 0;
#endif

    radeon_vbo_check(pScrn, &accel_state->vbo, 16);
    radeon_vbo_check(pScrn, &accel_state->cbuf, 512);
    radeon_cp_start(pScrn);

    evergreen_set_default_state(pScrn);

    evergreen_set_generic_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);
    evergreen_set_screen_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);
    evergreen_set_window_scissor(pScrn, 0, 0, accel_state->dst_obj.width, accel_state->dst_obj.height);

    /* PS bool constant */
    switch(pPriv->id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	evergreen_set_bool_consts(pScrn, SQ_BOOL_CONST_ps, (1 << 0));
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	evergreen_set_bool_consts(pScrn, SQ_BOOL_CONST_ps, (0 << 0));
	break;
    }

    /* Shader */
    vs_conf.shader_addr         = accel_state->vs_mc_addr;
    vs_conf.shader_size         = accel_state->vs_size;
    vs_conf.num_gprs            = 2;
    vs_conf.stack_size          = 0;
    vs_conf.bo                  = accel_state->shaders_bo;
    evergreen_vs_setup(pScrn, &vs_conf, RADEON_GEM_DOMAIN_VRAM);

    ps_conf.shader_addr         = accel_state->ps_mc_addr;
    ps_conf.shader_size         = accel_state->ps_size;
    ps_conf.num_gprs            = 3;
    ps_conf.stack_size          = 1;
    ps_conf.clamp_consts        = 0;
    ps_conf.export_mode         = 2;
    ps_conf.bo                  = accel_state->shaders_bo;
    evergreen_ps_setup(pScrn, &ps_conf, RADEON_GEM_DOMAIN_VRAM);

    /* Texture */
    switch(pPriv->id) {
    case FOURCC_YV12:
    case FOURCC_I420:
	accel_state->src_size[0] = accel_state->src_obj[0].pitch * pPriv->h;

	/* Y texture */
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
	tex_res.surface             = NULL;

	tex_res.format              = FMT_8;
	tex_res.dst_sel_x           = SQ_SEL_X; /* Y */
	tex_res.dst_sel_y           = SQ_SEL_1;
	tex_res.dst_sel_z           = SQ_SEL_1;
	tex_res.dst_sel_w           = SQ_SEL_1;

	tex_res.base_level          = 0;
	tex_res.last_level          = 0;
	tex_res.perf_modulation     = 0;
	tex_res.interlaced          = 0;
	if ((accel_state->src_obj[0].tiling_flags & RADEON_TILING_MASK) ==
	    RADEON_TILING_LINEAR)
	    tex_res.array_mode          = 1;
	evergreen_set_tex_resource(pScrn, &tex_res, accel_state->src_obj[0].domain);

	/* Y sampler */
	tex_samp.id                 = 0;
	tex_samp.clamp_x            = SQ_TEX_CLAMP_LAST_TEXEL;
	tex_samp.clamp_y            = SQ_TEX_CLAMP_LAST_TEXEL;
	tex_samp.clamp_z            = SQ_TEX_WRAP;

	/* xxx: switch to bicubic */
	tex_samp.xy_mag_filter      = SQ_TEX_XY_FILTER_BILINEAR;
	tex_samp.xy_min_filter      = SQ_TEX_XY_FILTER_BILINEAR;

	tex_samp.z_filter           = SQ_TEX_Z_FILTER_NONE;
	tex_samp.mip_filter         = 0;			/* no mipmap */
	evergreen_set_tex_sampler(pScrn, &tex_samp);

	/* U or V texture */
	tex_res.id                  = 1;
	tex_res.format              = FMT_8;
	tex_res.w                   = accel_state->src_obj[0].width >> 1;
	tex_res.h                   = accel_state->src_obj[0].height >> 1;
	tex_res.pitch               = RADEON_ALIGN(accel_state->src_obj[0].pitch >> 1, pPriv->hw_align);
	tex_res.dst_sel_x           = SQ_SEL_X; /* V or U */
	tex_res.dst_sel_y           = SQ_SEL_1;
	tex_res.dst_sel_z           = SQ_SEL_1;
	tex_res.dst_sel_w           = SQ_SEL_1;
	tex_res.interlaced          = 0;

	tex_res.base                = pPriv->planev_offset;
	tex_res.mip_base            = pPriv->planev_offset;
	tex_res.size                = tex_res.pitch * (pPriv->h >> 1);
	if ((accel_state->src_obj[0].tiling_flags & RADEON_TILING_MASK) ==
	    RADEON_TILING_LINEAR)
	    tex_res.array_mode          = 1;
	evergreen_set_tex_resource(pScrn, &tex_res, accel_state->src_obj[0].domain);

	/* U or V sampler */
	tex_samp.id                 = 1;
	evergreen_set_tex_sampler(pScrn, &tex_samp);

	/* U or V texture */
	tex_res.id                  = 2;
	tex_res.format              = FMT_8;
	tex_res.w                   = accel_state->src_obj[0].width >> 1;
	tex_res.h                   = accel_state->src_obj[0].height >> 1;
	tex_res.pitch               = RADEON_ALIGN(accel_state->src_obj[0].pitch >> 1, pPriv->hw_align);
	tex_res.dst_sel_x           = SQ_SEL_X; /* V or U */
	tex_res.dst_sel_y           = SQ_SEL_1;
	tex_res.dst_sel_z           = SQ_SEL_1;
	tex_res.dst_sel_w           = SQ_SEL_1;
	tex_res.interlaced          = 0;

	tex_res.base                = pPriv->planeu_offset;
	tex_res.mip_base            = pPriv->planeu_offset;
	tex_res.size                = tex_res.pitch * (pPriv->h >> 1);
	if ((accel_state->src_obj[0].tiling_flags & RADEON_TILING_MASK) ==
	    RADEON_TILING_LINEAR)
	    tex_res.array_mode          = 1;
	evergreen_set_tex_resource(pScrn, &tex_res, accel_state->src_obj[0].domain);

	/* UV sampler */
	tex_samp.id                 = 2;
	evergreen_set_tex_sampler(pScrn, &tex_samp);
	break;
    case FOURCC_UYVY:
    case FOURCC_YUY2:
    default:
	accel_state->src_size[0] = accel_state->src_obj[0].pitch * pPriv->h;

	/* YUV texture */
	tex_res.id                  = 0;
	tex_res.w                   = accel_state->src_obj[0].width;
	tex_res.h                   = accel_state->src_obj[0].height;
	tex_res.pitch               = accel_state->src_obj[0].pitch >> 1;
	tex_res.depth               = 0;
	tex_res.dim                 = SQ_TEX_DIM_2D;
	tex_res.base                = 0;
	tex_res.mip_base            = 0;
	tex_res.size                = accel_state->src_size[0];
	tex_res.bo                  = accel_state->src_obj[0].bo;
	tex_res.mip_bo              = accel_state->src_obj[0].bo;
	tex_res.surface             = NULL;

	if (pPriv->id == FOURCC_UYVY)
	    tex_res.format              = FMT_GB_GR;
	else
	    tex_res.format              = FMT_BG_RG;
	tex_res.dst_sel_x           = SQ_SEL_Y;
	tex_res.dst_sel_y           = SQ_SEL_X;
	tex_res.dst_sel_z           = SQ_SEL_Z;
	tex_res.dst_sel_w           = SQ_SEL_1;

	tex_res.base_level          = 0;
	tex_res.last_level          = 0;
	tex_res.perf_modulation     = 0;
	tex_res.interlaced          = 0;
	if ((accel_state->src_obj[0].tiling_flags & RADEON_TILING_MASK) ==
	    RADEON_TILING_LINEAR)
	    tex_res.array_mode          = 1;
	evergreen_set_tex_resource(pScrn, &tex_res, accel_state->src_obj[0].domain);

	/* YUV sampler */
	tex_samp.id                 = 0;
	tex_samp.clamp_x            = SQ_TEX_CLAMP_LAST_TEXEL;
	tex_samp.clamp_y            = SQ_TEX_CLAMP_LAST_TEXEL;
	tex_samp.clamp_z            = SQ_TEX_WRAP;

	tex_samp.xy_mag_filter      = SQ_TEX_XY_FILTER_BILINEAR;
	tex_samp.xy_min_filter      = SQ_TEX_XY_FILTER_BILINEAR;

	tex_samp.z_filter           = SQ_TEX_Z_FILTER_NONE;
	tex_samp.mip_filter         = 0;			/* no mipmap */
	evergreen_set_tex_sampler(pScrn, &tex_samp);

	break;
    }

    cb_conf.id = 0;
    cb_conf.w = accel_state->dst_obj.pitch;
    cb_conf.h = accel_state->dst_obj.height;
    cb_conf.base = 0;
    cb_conf.bo = accel_state->dst_obj.bo;
    cb_conf.surface = accel_state->dst_obj.surface;

    switch (accel_state->dst_obj.bpp) {
    case 16:
	if (pPixmap->drawable.depth == 15) {
	    cb_conf.format = COLOR_1_5_5_5;
	    cb_conf.comp_swap = 1; /* ARGB */
	} else {
	    cb_conf.format = COLOR_5_6_5;
	    cb_conf.comp_swap = 2; /* RGB */
	}
#if X_BYTE_ORDER == X_BIG_ENDIAN
	cb_conf.endian = ENDIAN_8IN16;
#endif
	break;
    case 32:
	cb_conf.format = COLOR_8_8_8_8;
	cb_conf.comp_swap = 1; /* ARGB */
#if X_BYTE_ORDER == X_BIG_ENDIAN
	cb_conf.endian = ENDIAN_8IN32;
#endif
	break;
    default:
	return;
    }

    cb_conf.source_format = EXPORT_4C_16BPC;
    cb_conf.blend_clamp = 1;
    cb_conf.pmask = 0xf;
    cb_conf.rop = 3;
    if ((accel_state->dst_obj.tiling_flags & RADEON_TILING_MASK) ==
	RADEON_TILING_LINEAR) {
	cb_conf.array_mode = 1;
	cb_conf.non_disp_tiling = 1;
    }
    evergreen_set_render_target(pScrn, &cb_conf, accel_state->dst_obj.domain);

    evergreen_set_spi(pScrn, (1 - 1), 1);

    /* PS alu constants */
    ps_const_conf.size_bytes = 256;
    ps_const_conf.type = SHADER_TYPE_PS;
    ps_alu_consts = radeon_vbo_space(pScrn, &accel_state->cbuf, 256);
    ps_const_conf.bo = accel_state->cbuf.vb_bo;
    ps_const_conf.const_addr = accel_state->cbuf.vb_offset;
    ps_const_conf.cpu_ptr = (uint32_t *)(char *)ps_alu_consts;

    ps_alu_consts[0] = off[0];
    ps_alu_consts[1] = off[1];
    ps_alu_consts[2] = off[2];
    ps_alu_consts[3] = yco;

    ps_alu_consts[4] = uco[0];
    ps_alu_consts[5] = uco[1];
    ps_alu_consts[6] = uco[2];
    ps_alu_consts[7] = gamma;

    ps_alu_consts[8] = vco[0];
    ps_alu_consts[9] = vco[1];
    ps_alu_consts[10] = vco[2];
    ps_alu_consts[11] = 0.0;

    radeon_vbo_commit(pScrn, &accel_state->cbuf);
    evergreen_set_alu_consts(pScrn, &ps_const_conf, RADEON_GEM_DOMAIN_GTT);

    /* VS alu constants */
    vs_const_conf.size_bytes = 256;
    vs_const_conf.type = SHADER_TYPE_VS;
    vs_alu_consts = radeon_vbo_space(pScrn, &accel_state->cbuf, 256);
    vs_const_conf.bo = accel_state->cbuf.vb_bo;
    vs_const_conf.const_addr = accel_state->cbuf.vb_offset;
    vs_const_conf.cpu_ptr = (uint32_t *)(char *)vs_alu_consts;

    vs_alu_consts[0] = 1.0 / pPriv->w;
    vs_alu_consts[1] = 1.0 / pPriv->h;
    vs_alu_consts[2] = 0.0;
    vs_alu_consts[3] = 0.0;

    radeon_vbo_commit(pScrn, &accel_state->cbuf);
    evergreen_set_alu_consts(pScrn, &vs_const_conf, RADEON_GEM_DOMAIN_GTT);

    if (pPriv->vsync) {
	xf86CrtcPtr crtc;
	if (pPriv->desired_crtc)
	    crtc = pPriv->desired_crtc;
	else
	    crtc = radeon_pick_best_crtc(pScrn, FALSE,
					 pPriv->drw_x,
					 pPriv->drw_x + pPriv->dst_w,
					 pPriv->drw_y,
					 pPriv->drw_y + pPriv->dst_h);
	if (crtc)
	    evergreen_cp_wait_vline_sync(pScrn, pPixmap,
					 crtc,
					 pPriv->drw_y - crtc->y,
					 (pPriv->drw_y - crtc->y) + pPriv->dst_h);
    }

    while (nBox--) {
	float srcX, srcY, srcw, srch;
	int dstX, dstY, dstw, dsth;
	float *vb;


	dstX = pBox->x1 + dstxoff;
	dstY = pBox->y1 + dstyoff;
	dstw = pBox->x2 - pBox->x1;
	dsth = pBox->y2 - pBox->y1;

	srcX = pPriv->src_x;
	srcX += ((pBox->x1 - pPriv->drw_x) *
		 pPriv->src_w) / (float)pPriv->dst_w;
	srcY = pPriv->src_y;
	srcY += ((pBox->y1 - pPriv->drw_y) *
		 pPriv->src_h) / (float)pPriv->dst_h;

	srcw = (pPriv->src_w * dstw) / (float)pPriv->dst_w;
	srch = (pPriv->src_h * dsth) / (float)pPriv->dst_h;

	vb = radeon_vbo_space(pScrn, &accel_state->vbo, 16);

	vb[0] = (float)dstX;
	vb[1] = (float)dstY;
	vb[2] = (float)srcX;
	vb[3] = (float)srcY;

	vb[4] = (float)dstX;
	vb[5] = (float)(dstY + dsth);
	vb[6] = (float)srcX;
	vb[7] = (float)(srcY + srch);

	vb[8] = (float)(dstX + dstw);
	vb[9] = (float)(dstY + dsth);
	vb[10] = (float)(srcX + srcw);
	vb[11] = (float)(srcY + srch);

	radeon_vbo_commit(pScrn, &accel_state->vbo);

	pBox++;
    }

    evergreen_finish_op(pScrn, 16);

    DamageDamageRegion(pPriv->pDraw, &pPriv->clip);
}
