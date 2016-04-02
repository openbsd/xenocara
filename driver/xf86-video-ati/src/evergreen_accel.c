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
 * Authors: Alex Deucher <alexander.deucher@amd.com>
 *
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include <errno.h>

#include "radeon.h"
#include "evergreen_shader.h"
#include "radeon_reg.h"
#include "evergreen_reg.h"
#include "evergreen_state.h"

#include "radeon_vbo.h"
#include "radeon_exa_shared.h"

static const uint32_t EVERGREEN_ROP[16] = {
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

void
evergreen_start_3d(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    BEGIN_BATCH(3);
    PACK3(IT_CONTEXT_CONTROL, 2);
    E32(0x80000000);
    E32(0x80000000);
    END_BATCH();

}

unsigned eg_tile_split(unsigned tile_split)
{
	switch (tile_split) {
	case 64:	tile_split = 0;	break;
	case 128:	tile_split = 1;	break;
	case 256:	tile_split = 2;	break;
	case 512:	tile_split = 3;	break;
	default:
	case 1024:	tile_split = 4;	break;
	case 2048:	tile_split = 5;	break;
	case 4096:	tile_split = 6;	break;
	}
	return tile_split;
}

static unsigned eg_macro_tile_aspect(unsigned macro_tile_aspect)
{
	switch (macro_tile_aspect) {
	default:
	case 1:	macro_tile_aspect = 0;	break;
	case 2:	macro_tile_aspect = 1;	break;
	case 4:	macro_tile_aspect = 2;	break;
	case 8:	macro_tile_aspect = 3;	break;
	}
	return macro_tile_aspect;
}

static unsigned eg_bank_wh(unsigned bankwh)
{
	switch (bankwh) {
	default:
	case 1:	bankwh = 0;	break;
	case 2:	bankwh = 1;	break;
	case 4:	bankwh = 2;	break;
	case 8:	bankwh = 3;	break;
	}
	return bankwh;
}

static unsigned eg_nbanks(unsigned nbanks)
{
	switch (nbanks) {
	default:
	case 2: nbanks = 0; break;
	case 4: nbanks = 1; break;
	case 8: nbanks = 2; break;
	case 16: nbanks = 3; break;
	}
	return nbanks;
}

/*
 * Setup of functional groups
 */

// asic stack/thread/gpr limits - need to query the drm
static void
evergreen_sq_setup(ScrnInfoPtr pScrn, sq_config_t *sq_conf)
{
    uint32_t sq_config, sq_gpr_resource_mgmt_1, sq_gpr_resource_mgmt_2, sq_gpr_resource_mgmt_3;
    uint32_t sq_thread_resource_mgmt, sq_thread_resource_mgmt_2;
    uint32_t sq_stack_resource_mgmt_1, sq_stack_resource_mgmt_2, sq_stack_resource_mgmt_3;
    RADEONInfoPtr info = RADEONPTR(pScrn);

    if ((info->ChipFamily == CHIP_FAMILY_CEDAR) ||
	(info->ChipFamily == CHIP_FAMILY_PALM) ||
	(info->ChipFamily == CHIP_FAMILY_SUMO) ||
	(info->ChipFamily == CHIP_FAMILY_SUMO2) ||
	(info->ChipFamily == CHIP_FAMILY_CAICOS))
	sq_config = 0;
    else
	sq_config = VC_ENABLE_bit;

    sq_config |= (EXPORT_SRC_C_bit |
		  (sq_conf->cs_prio << CS_PRIO_shift) |
		  (sq_conf->ls_prio << LS_PRIO_shift) |
		  (sq_conf->hs_prio << HS_PRIO_shift) |
		  (sq_conf->ps_prio << PS_PRIO_shift) |
		  (sq_conf->vs_prio << VS_PRIO_shift) |
		  (sq_conf->gs_prio << GS_PRIO_shift) |
		  (sq_conf->es_prio << ES_PRIO_shift));

    sq_gpr_resource_mgmt_1 = ((sq_conf->num_ps_gprs << NUM_PS_GPRS_shift) |
			      (sq_conf->num_vs_gprs << NUM_VS_GPRS_shift) |
			      (sq_conf->num_temp_gprs << NUM_CLAUSE_TEMP_GPRS_shift));
    sq_gpr_resource_mgmt_2 = ((sq_conf->num_gs_gprs << NUM_GS_GPRS_shift) |
			      (sq_conf->num_es_gprs << NUM_ES_GPRS_shift));
    sq_gpr_resource_mgmt_3 = ((sq_conf->num_hs_gprs << NUM_HS_GPRS_shift) |
			      (sq_conf->num_ls_gprs << NUM_LS_GPRS_shift));

    sq_thread_resource_mgmt = ((sq_conf->num_ps_threads << NUM_PS_THREADS_shift) |
			       (sq_conf->num_vs_threads << NUM_VS_THREADS_shift) |
			       (sq_conf->num_gs_threads << NUM_GS_THREADS_shift) |
			       (sq_conf->num_es_threads << NUM_ES_THREADS_shift));
    sq_thread_resource_mgmt_2 = ((sq_conf->num_hs_threads << NUM_HS_THREADS_shift) |
				 (sq_conf->num_ls_threads << NUM_LS_THREADS_shift));

    sq_stack_resource_mgmt_1 = ((sq_conf->num_ps_stack_entries << NUM_PS_STACK_ENTRIES_shift) |
				(sq_conf->num_vs_stack_entries << NUM_VS_STACK_ENTRIES_shift));

    sq_stack_resource_mgmt_2 = ((sq_conf->num_gs_stack_entries << NUM_GS_STACK_ENTRIES_shift) |
				(sq_conf->num_es_stack_entries << NUM_ES_STACK_ENTRIES_shift));

    sq_stack_resource_mgmt_3 = ((sq_conf->num_hs_stack_entries << NUM_HS_STACK_ENTRIES_shift) |
				(sq_conf->num_ls_stack_entries << NUM_LS_STACK_ENTRIES_shift));

    BEGIN_BATCH(16);
    /* disable dyn gprs */
    EREG(SQ_DYN_GPR_CNTL_PS_FLUSH_REQ, 0);
    PACK0(SQ_CONFIG, 4);
    E32(sq_config);
    E32(sq_gpr_resource_mgmt_1);
    E32(sq_gpr_resource_mgmt_2);
    E32(sq_gpr_resource_mgmt_3);
    PACK0(SQ_THREAD_RESOURCE_MGMT, 5);
    E32(sq_thread_resource_mgmt);
    E32(sq_thread_resource_mgmt_2);
    E32(sq_stack_resource_mgmt_1);
    E32(sq_stack_resource_mgmt_2);
    E32(sq_stack_resource_mgmt_3);
    END_BATCH();
}

/* cayman has some minor differences in CB_COLOR*_INFO and _ATTRIB, but none that
 * we use here.
 */
void
evergreen_set_render_target(ScrnInfoPtr pScrn, cb_config_t *cb_conf, uint32_t domain)
{
    uint32_t cb_color_info, cb_color_attrib = 0, cb_color_dim;
    unsigned pitch, slice, w, h, array_mode, nbanks;
    uint32_t tile_split, macro_aspect, bankw, bankh;
    RADEONInfoPtr info = RADEONPTR(pScrn);

    if (cb_conf->surface) {
	switch (cb_conf->surface->level[0].mode) {
	case RADEON_SURF_MODE_1D:
		array_mode = 2;
		break;
	case RADEON_SURF_MODE_2D:
		array_mode = 4;
		break;
	default:
		array_mode = 0;
		break;
	}
	w = cb_conf->surface->level[0].npix_x;
	h = cb_conf->surface->level[0].npix_y;
	pitch = (cb_conf->surface->level[0].nblk_x >> 3) - 1;
	slice = ((cb_conf->surface->level[0].nblk_x * cb_conf->surface->level[0].nblk_y) / 64) - 1;
	tile_split = cb_conf->surface->tile_split;
	macro_aspect = cb_conf->surface->mtilea;
	bankw = cb_conf->surface->bankw;
	bankh = cb_conf->surface->bankh;
	tile_split = eg_tile_split(tile_split);
	macro_aspect = eg_macro_tile_aspect(macro_aspect);
	bankw = eg_bank_wh(bankw);
	bankh = eg_bank_wh(bankh);
    } else {
	pitch = (cb_conf->w / 8) - 1;
	h = RADEON_ALIGN(cb_conf->h, 8);
	slice = ((cb_conf->w * h) / 64) - 1;
	array_mode = cb_conf->array_mode;
	w = cb_conf->w;
	tile_split = 4;
	macro_aspect = 0;
	bankw = 0;
	bankh = 0;
    }
    nbanks = info->num_banks;
    nbanks = eg_nbanks(nbanks);

    cb_color_attrib |= (tile_split << CB_COLOR0_ATTRIB__TILE_SPLIT_shift)|
		       (nbanks << CB_COLOR0_ATTRIB__NUM_BANKS_shift) |
		       (bankw << CB_COLOR0_ATTRIB__BANK_WIDTH_shift) |
		       (bankh << CB_COLOR0_ATTRIB__BANK_HEIGHT_shift) |
		       (macro_aspect << CB_COLOR0_ATTRIB__MACRO_TILE_ASPECT_shift);
    cb_color_info = ((cb_conf->endian      << ENDIAN_shift)				|
		     (cb_conf->format      << CB_COLOR0_INFO__FORMAT_shift)		|
		     (array_mode  << CB_COLOR0_INFO__ARRAY_MODE_shift)		|
		     (cb_conf->number_type << NUMBER_TYPE_shift)			|
		     (cb_conf->comp_swap   << COMP_SWAP_shift)				|
		     (cb_conf->source_format << SOURCE_FORMAT_shift)                    |
		     (cb_conf->resource_type << RESOURCE_TYPE_shift));
    if (cb_conf->blend_clamp)
	cb_color_info |= BLEND_CLAMP_bit;
    if (cb_conf->fast_clear)
	cb_color_info |= FAST_CLEAR_bit;
    if (cb_conf->compression)
	cb_color_info |= COMPRESSION_bit;
    if (cb_conf->blend_bypass)
	cb_color_info |= BLEND_BYPASS_bit;
    if (cb_conf->simple_float)
	cb_color_info |= SIMPLE_FLOAT_bit;
    if (cb_conf->round_mode)
	cb_color_info |= CB_COLOR0_INFO__ROUND_MODE_bit;
    if (cb_conf->tile_compact)
	cb_color_info |= CB_COLOR0_INFO__TILE_COMPACT_bit;
    if (cb_conf->rat)
	cb_color_info |= RAT_bit;

    /* bit 4 needs to be set for linear and depth/stencil surfaces */
    if (cb_conf->non_disp_tiling)
	cb_color_attrib |= CB_COLOR0_ATTRIB__NON_DISP_TILING_ORDER_bit;

    switch (cb_conf->resource_type) {
    case BUFFER:
	/* number of elements in the surface */
	cb_color_dim = pitch * slice;
	break;
    default:
	/* w/h of the surface */
	cb_color_dim = (((w - 1) << WIDTH_MAX_shift) |
			((cb_conf->h - 1) << HEIGHT_MAX_shift));
	break;
    }

    BEGIN_BATCH(3 + 2);
    EREG(CB_COLOR0_BASE + (0x3c * cb_conf->id), (cb_conf->base >> 8));
    RELOC_BATCH(cb_conf->bo, 0, domain);
    END_BATCH();

    /* Set CMASK & FMASK buffer to the offset of color buffer as
     * we don't use those this shouldn't cause any issue and we
     * then have a valid cmd stream
     */
    BEGIN_BATCH(3 + 2);
    EREG(CB_COLOR0_CMASK + (0x3c * cb_conf->id), (0     >> 8));
    RELOC_BATCH(cb_conf->bo, 0, domain);
    END_BATCH();
    BEGIN_BATCH(3 + 2);
    EREG(CB_COLOR0_FMASK + (0x3c * cb_conf->id), (0     >> 8));
    RELOC_BATCH(cb_conf->bo, 0, domain);
    END_BATCH();

    /* tiling config */
    BEGIN_BATCH(3 + 2);
    EREG(CB_COLOR0_ATTRIB + (0x3c * cb_conf->id), cb_color_attrib);
    RELOC_BATCH(cb_conf->bo, 0, domain);
    END_BATCH();
    BEGIN_BATCH(3 + 2);
    EREG(CB_COLOR0_INFO + (0x3c * cb_conf->id), cb_color_info);
    RELOC_BATCH(cb_conf->bo, 0, domain);
    END_BATCH();

    BEGIN_BATCH(33);
    EREG(CB_COLOR0_PITCH + (0x3c * cb_conf->id), pitch);
    EREG(CB_COLOR0_SLICE + (0x3c * cb_conf->id), slice);
    EREG(CB_COLOR0_VIEW + (0x3c * cb_conf->id), 0);
    EREG(CB_COLOR0_DIM + (0x3c * cb_conf->id), cb_color_dim);
    EREG(CB_COLOR0_CMASK_SLICE + (0x3c * cb_conf->id), 0);
    EREG(CB_COLOR0_FMASK_SLICE + (0x3c * cb_conf->id), 0);
    PACK0(CB_COLOR0_CLEAR_WORD0 + (0x3c * cb_conf->id), 4);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    EREG(CB_TARGET_MASK,                      (cb_conf->pmask << TARGET0_ENABLE_shift));
    EREG(CB_COLOR_CONTROL,                    (EVERGREEN_ROP[cb_conf->rop] |
					       (CB_NORMAL << CB_COLOR_CONTROL__MODE_shift)));
    EREG(CB_BLEND0_CONTROL,                   cb_conf->blendcntl);
    END_BATCH();
}

void evergreen_set_blend_color(ScrnInfoPtr pScrn, float *color)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    BEGIN_BATCH(2 + 4);
    PACK0(CB_BLEND_RED, 4);
    EFLOAT(color[0]); /* R */
    EFLOAT(color[1]); /* G */
    EFLOAT(color[2]); /* B */
    EFLOAT(color[3]); /* A */
    END_BATCH();
}

static void
evergreen_cp_set_surface_sync(ScrnInfoPtr pScrn, uint32_t sync_type,
			      uint32_t size, uint64_t mc_addr,
			      struct radeon_bo *bo, uint32_t rdomains, uint32_t wdomain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t cp_coher_size;
    if (size == 0xffffffff)
	cp_coher_size = 0xffffffff;
    else
	cp_coher_size = ((size + 255) >> 8);

    BEGIN_BATCH(5 + 2);
    PACK3(IT_SURFACE_SYNC, 4);
    E32(sync_type);
    E32(cp_coher_size);
    E32((mc_addr >> 8));
    E32(10); /* poll interval */
    RELOC_BATCH(bo, rdomains, wdomain);
    END_BATCH();
}

/* inserts a wait for vline in the command stream */
void evergreen_cp_wait_vline_sync(ScrnInfoPtr pScrn, PixmapPtr pPix,
				  xf86CrtcPtr crtc, int start, int stop)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    drmmode_crtc_private_ptr drmmode_crtc;

    if (!crtc)
        return;

    drmmode_crtc = crtc->driver_private;

    if (!crtc->enabled)
        return;

    if (pPix != pScrn->pScreen->GetScreenPixmap(pScrn->pScreen))
        return;

    start = max(start, crtc->y);
    stop = min(stop, crtc->y + crtc->mode.VDisplay);

    if (start >= stop)
        return;

    BEGIN_BATCH(11);
    /* set the VLINE range */
    EREG(EVERGREEN_VLINE_START_END, /* this is just a marker */
	 (start << EVERGREEN_VLINE_START_SHIFT) |
	 (stop << EVERGREEN_VLINE_END_SHIFT));

    /* tell the CP to poll the VLINE state register */
    PACK3(IT_WAIT_REG_MEM, 6);
    E32(IT_WAIT_REG | IT_WAIT_EQ);
    E32(IT_WAIT_ADDR(EVERGREEN_VLINE_STATUS));
    E32(0);
    E32(0);                          // Ref value
    E32(EVERGREEN_VLINE_STAT);    // Mask
    E32(10);                         // Wait interval
    /* add crtc reloc */
    PACK3(IT_NOP, 1);
    E32(drmmode_crtc->mode_crtc->crtc_id);
    END_BATCH();
}

void
evergreen_set_spi(ScrnInfoPtr pScrn, int vs_export_count, int num_interp)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    BEGIN_BATCH(8);
    /* Interpolator setup */
    EREG(SPI_VS_OUT_CONFIG, (vs_export_count << VS_EXPORT_COUNT_shift));
    PACK0(SPI_PS_IN_CONTROL_0, 3);
    E32(((num_interp << NUM_INTERP_shift) |
	 LINEAR_GRADIENT_ENA_bit)); // SPI_PS_IN_CONTROL_0
    E32(0); // SPI_PS_IN_CONTROL_1
    E32(0); // SPI_INTERP_CONTROL_0
    END_BATCH();
}

void
evergreen_fs_setup(ScrnInfoPtr pScrn, shader_config_t *fs_conf, uint32_t domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t sq_pgm_resources;

    sq_pgm_resources = ((fs_conf->num_gprs << NUM_GPRS_shift) |
			(fs_conf->stack_size << STACK_SIZE_shift));

    if (fs_conf->dx10_clamp)
	sq_pgm_resources |= DX10_CLAMP_bit;

    BEGIN_BATCH(3 + 2);
    EREG(SQ_PGM_START_FS, fs_conf->shader_addr >> 8);
    RELOC_BATCH(fs_conf->bo, domain, 0);
    END_BATCH();

    BEGIN_BATCH(3);
    EREG(SQ_PGM_RESOURCES_FS, sq_pgm_resources);
    END_BATCH();
}

/* cayman has some minor differences in SQ_PGM_RESOUCES_VS and _RESOURCES_2_VS,
 * but none that we use here.
 */
void
evergreen_vs_setup(ScrnInfoPtr pScrn, shader_config_t *vs_conf, uint32_t domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t sq_pgm_resources, sq_pgm_resources_2;

    sq_pgm_resources = ((vs_conf->num_gprs << NUM_GPRS_shift) |
			(vs_conf->stack_size << STACK_SIZE_shift));

    if (vs_conf->dx10_clamp)
	sq_pgm_resources |= DX10_CLAMP_bit;
    if (vs_conf->uncached_first_inst)
	sq_pgm_resources |= UNCACHED_FIRST_INST_bit;

    sq_pgm_resources_2 = ((vs_conf->single_round << SINGLE_ROUND_shift) |
			  (vs_conf->double_round << DOUBLE_ROUND_shift));

    if (vs_conf->allow_sdi)
	sq_pgm_resources_2 |= ALLOW_SINGLE_DENORM_IN_bit;
    if (vs_conf->allow_sd0)
	sq_pgm_resources_2 |= ALLOW_SINGLE_DENORM_OUT_bit;
    if (vs_conf->allow_ddi)
	sq_pgm_resources_2 |= ALLOW_DOUBLE_DENORM_IN_bit;
    if (vs_conf->allow_ddo)
	sq_pgm_resources_2 |= ALLOW_DOUBLE_DENORM_OUT_bit;

    /* flush SQ cache */
    evergreen_cp_set_surface_sync(pScrn, SH_ACTION_ENA_bit,
				  vs_conf->shader_size, vs_conf->shader_addr,
				  vs_conf->bo, domain, 0);

    BEGIN_BATCH(3 + 2);
    EREG(SQ_PGM_START_VS, vs_conf->shader_addr >> 8);
    RELOC_BATCH(vs_conf->bo, domain, 0);
    END_BATCH();

    BEGIN_BATCH(4);
    PACK0(SQ_PGM_RESOURCES_VS, 2);
    E32(sq_pgm_resources);
    E32(sq_pgm_resources_2);
    END_BATCH();
}

/* cayman has some minor differences in SQ_PGM_RESOUCES_PS and _RESOURCES_2_PS,
 * but none that we use here.
 */
void
evergreen_ps_setup(ScrnInfoPtr pScrn, shader_config_t *ps_conf, uint32_t domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t sq_pgm_resources, sq_pgm_resources_2;

    sq_pgm_resources = ((ps_conf->num_gprs << NUM_GPRS_shift) |
			(ps_conf->stack_size << STACK_SIZE_shift));

    if (ps_conf->dx10_clamp)
	sq_pgm_resources |= DX10_CLAMP_bit;
    if (ps_conf->uncached_first_inst)
	sq_pgm_resources |= UNCACHED_FIRST_INST_bit;
    if (ps_conf->clamp_consts)
	sq_pgm_resources |= CLAMP_CONSTS_bit;

    sq_pgm_resources_2 = ((ps_conf->single_round << SINGLE_ROUND_shift) |
			  (ps_conf->double_round << DOUBLE_ROUND_shift));

    if (ps_conf->allow_sdi)
	sq_pgm_resources_2 |= ALLOW_SINGLE_DENORM_IN_bit;
    if (ps_conf->allow_sd0)
	sq_pgm_resources_2 |= ALLOW_SINGLE_DENORM_OUT_bit;
    if (ps_conf->allow_ddi)
	sq_pgm_resources_2 |= ALLOW_DOUBLE_DENORM_IN_bit;
    if (ps_conf->allow_ddo)
	sq_pgm_resources_2 |= ALLOW_DOUBLE_DENORM_OUT_bit;

    /* flush SQ cache */
    evergreen_cp_set_surface_sync(pScrn, SH_ACTION_ENA_bit,
				  ps_conf->shader_size, ps_conf->shader_addr,
				  ps_conf->bo, domain, 0);

    BEGIN_BATCH(3 + 2);
    EREG(SQ_PGM_START_PS, ps_conf->shader_addr >> 8);
    RELOC_BATCH(ps_conf->bo, domain, 0);
    END_BATCH();

    BEGIN_BATCH(5);
    PACK0(SQ_PGM_RESOURCES_PS, 3);
    E32(sq_pgm_resources);
    E32(sq_pgm_resources_2);
    E32(ps_conf->export_mode);
    END_BATCH();
}

void
evergreen_set_alu_consts(ScrnInfoPtr pScrn, const_config_t *const_conf, uint32_t domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    /* size reg is units of 16 consts (4 dwords each) */
    uint32_t size = const_conf->size_bytes >> 8;

    if (size == 0)
	size = 1;

#if X_BYTE_ORDER == X_BIG_ENDIAN
    {
	    uint32_t count = size << 6, *p = const_conf->cpu_ptr;

	    while(count--) {
		    *p = cpu_to_le32(*p);
		    p++;
	    }
    }
#endif

    /* flush SQ cache */
    evergreen_cp_set_surface_sync(pScrn, SH_ACTION_ENA_bit,
				  const_conf->size_bytes, const_conf->const_addr,
				  const_conf->bo, domain, 0);

    switch (const_conf->type) {
    case SHADER_TYPE_VS:
	BEGIN_BATCH(3);
	EREG(SQ_ALU_CONST_BUFFER_SIZE_VS_0, size);
	END_BATCH();
	BEGIN_BATCH(3 + 2);
	EREG(SQ_ALU_CONST_CACHE_VS_0, const_conf->const_addr >> 8);
	RELOC_BATCH(const_conf->bo, domain, 0);
	END_BATCH();
	break;
    case SHADER_TYPE_PS:
	BEGIN_BATCH(3);
	EREG(SQ_ALU_CONST_BUFFER_SIZE_PS_0, size);
	END_BATCH();
	BEGIN_BATCH(3 + 2);
	EREG(SQ_ALU_CONST_CACHE_PS_0, const_conf->const_addr >> 8);
	RELOC_BATCH(const_conf->bo, domain, 0);
	END_BATCH();
	break;
    default:
	ErrorF("Unsupported const type %d\n", const_conf->type);
	break;
    }

}

void
evergreen_set_bool_consts(ScrnInfoPtr pScrn, int offset, uint32_t val)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    /* bool register order is: ps, vs/es, gs, hs, ls, cs; one register each
     * 1 bits per bool; 32 bools each for ps, vs/es, gs, hs, ls, cs.
     */
    BEGIN_BATCH(3);
    EREG(SQ_BOOL_CONST + offset * SQ_BOOL_CONST_offset, val);
    END_BATCH();
}

/* cayman has some minor differences in SQ_VTX_CONSTANT_WORD2_0 and _WORD3_0,
 * but none that we use here.
 */
static void
evergreen_set_vtx_resource(ScrnInfoPtr pScrn, vtx_resource_t *res, uint32_t domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    uint32_t sq_vtx_constant_word2, sq_vtx_constant_word3, sq_vtx_constant_word4;

    sq_vtx_constant_word2 = ((((res->vb_addr) >> 32) & BASE_ADDRESS_HI_mask) |
			     ((res->vtx_size_dw << 2) << SQ_VTX_CONSTANT_WORD2_0__STRIDE_shift) |
			     (res->format << SQ_VTX_CONSTANT_WORD2_0__DATA_FORMAT_shift) |
			     (res->num_format_all << SQ_VTX_CONSTANT_WORD2_0__NUM_FORMAT_ALL_shift) |
			     (res->endian << SQ_VTX_CONSTANT_WORD2_0__ENDIAN_SWAP_shift));
    if (res->clamp_x)
	    sq_vtx_constant_word2 |= SQ_VTX_CONSTANT_WORD2_0__CLAMP_X_bit;

    if (res->format_comp_all)
	    sq_vtx_constant_word2 |= SQ_VTX_CONSTANT_WORD2_0__FORMAT_COMP_ALL_bit;

    if (res->srf_mode_all)
	    sq_vtx_constant_word2 |= SQ_VTX_CONSTANT_WORD2_0__SRF_MODE_ALL_bit;

    sq_vtx_constant_word3 = ((res->dst_sel_x << SQ_VTX_CONSTANT_WORD3_0__DST_SEL_X_shift) |
			     (res->dst_sel_y << SQ_VTX_CONSTANT_WORD3_0__DST_SEL_Y_shift) |
			     (res->dst_sel_z << SQ_VTX_CONSTANT_WORD3_0__DST_SEL_Z_shift) |
			     (res->dst_sel_w << SQ_VTX_CONSTANT_WORD3_0__DST_SEL_W_shift));

    if (res->uncached)
	sq_vtx_constant_word3 |= SQ_VTX_CONSTANT_WORD3_0__UNCACHED_bit;

    /* XXX ??? */
    sq_vtx_constant_word4 = 0;

    /* flush vertex cache */
    if ((info->ChipFamily == CHIP_FAMILY_CEDAR) ||
	(info->ChipFamily == CHIP_FAMILY_PALM) ||
	(info->ChipFamily == CHIP_FAMILY_SUMO) ||
	(info->ChipFamily == CHIP_FAMILY_SUMO2) ||
	(info->ChipFamily == CHIP_FAMILY_CAICOS) ||
	(info->ChipFamily == CHIP_FAMILY_CAYMAN) ||
	(info->ChipFamily == CHIP_FAMILY_ARUBA))
	evergreen_cp_set_surface_sync(pScrn, TC_ACTION_ENA_bit,
				      accel_state->vbo.vb_offset, 0,
				      res->bo,
				      domain, 0);
    else
	evergreen_cp_set_surface_sync(pScrn, VC_ACTION_ENA_bit,
				      accel_state->vbo.vb_offset, 0,
				      res->bo,
				      domain, 0);

    BEGIN_BATCH(10 + 2);
    PACK0(SQ_FETCH_RESOURCE + res->id * SQ_FETCH_RESOURCE_offset, 8);
    E32(res->vb_addr & 0xffffffff);				// 0: BASE_ADDRESS
    E32((res->vtx_num_entries << 2) - 1);			// 1: SIZE
    E32(sq_vtx_constant_word2);	// 2: BASE_HI, STRIDE, CLAMP, FORMAT, ENDIAN
    E32(sq_vtx_constant_word3);		// 3: swizzles
    E32(sq_vtx_constant_word4);		// 4: num elements
    E32(0);							// 5: n/a
    E32(0);							// 6: n/a
    E32(SQ_TEX_VTX_VALID_BUFFER << SQ_VTX_CONSTANT_WORD7_0__TYPE_shift);	// 7: TYPE
    RELOC_BATCH(res->bo, domain, 0);
    END_BATCH();
}

/* cayman has some minor differences in SQ_TEX_CONSTANT_WORD0_0 and _WORD4_0,
 * but none that we use here.
 */
void
evergreen_set_tex_resource(ScrnInfoPtr pScrn, tex_resource_t *tex_res, uint32_t domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t sq_tex_resource_word0, sq_tex_resource_word1, sq_tex_resource_word4;
    uint32_t sq_tex_resource_word5, sq_tex_resource_word6, sq_tex_resource_word7;
    uint32_t array_mode, pitch, tile_split, macro_aspect, bankw, bankh, nbanks;

    if (tex_res->surface) {
	switch (tex_res->surface->level[0].mode) {
	case RADEON_SURF_MODE_1D:
		array_mode = 2;
		break;
	case RADEON_SURF_MODE_2D:
		array_mode = 4;
		break;
	default:
		array_mode = 0;
		break;
	}
	pitch = tex_res->surface->level[0].nblk_x >> 3;
	tile_split = tex_res->surface->tile_split;
	macro_aspect = tex_res->surface->mtilea;
	bankw = tex_res->surface->bankw;
	bankh = tex_res->surface->bankh;
	tile_split = eg_tile_split(tile_split);
	macro_aspect = eg_macro_tile_aspect(macro_aspect);
	bankw = eg_bank_wh(bankw);
	bankh = eg_bank_wh(bankh);
    } else {
	array_mode = tex_res->array_mode;
	pitch = (tex_res->pitch + 7) >> 3;
	tile_split = 4;
	macro_aspect = 0;
	bankw = 0;
	bankh = 0;
    }
    nbanks = info->num_banks;
    nbanks = eg_nbanks(nbanks);

    sq_tex_resource_word0 = (tex_res->dim << DIM_shift);

    if (tex_res->w)
	sq_tex_resource_word0 |= ( ((pitch - 1) << PITCH_shift) |
				   ((tex_res->w - 1) << TEX_WIDTH_shift) );

    if (tex_res->tile_type)
	sq_tex_resource_word0 |= SQ_TEX_RESOURCE_WORD0_0__NON_DISP_TILING_ORDER_bit;

    sq_tex_resource_word1 = (array_mode << SQ_TEX_RESOURCE_WORD1_0__ARRAY_MODE_shift);

    if (tex_res->h)
	sq_tex_resource_word1 |= ((tex_res->h - 1) << TEX_HEIGHT_shift);
    if (tex_res->depth)
	sq_tex_resource_word1 |= ((tex_res->depth - 1) << TEX_DEPTH_shift);

    sq_tex_resource_word4 = ((tex_res->format_comp_x << FORMAT_COMP_X_shift) |
			     (tex_res->format_comp_y << FORMAT_COMP_Y_shift) |
			     (tex_res->format_comp_z << FORMAT_COMP_Z_shift) |
			     (tex_res->format_comp_w << FORMAT_COMP_W_shift) |
			     (tex_res->num_format_all << SQ_TEX_RESOURCE_WORD4_0__NUM_FORMAT_ALL_shift) |
			     (tex_res->endian << SQ_TEX_RESOURCE_WORD4_0__ENDIAN_SWAP_shift) |
			     (tex_res->dst_sel_x << SQ_TEX_RESOURCE_WORD4_0__DST_SEL_X_shift) |
			     (tex_res->dst_sel_y << SQ_TEX_RESOURCE_WORD4_0__DST_SEL_Y_shift) |
			     (tex_res->dst_sel_z << SQ_TEX_RESOURCE_WORD4_0__DST_SEL_Z_shift) |
			     (tex_res->dst_sel_w << SQ_TEX_RESOURCE_WORD4_0__DST_SEL_W_shift) |
			     (tex_res->base_level << BASE_LEVEL_shift));

    if (tex_res->srf_mode_all)
	sq_tex_resource_word4 |= SQ_TEX_RESOURCE_WORD4_0__SRF_MODE_ALL_bit;
    if (tex_res->force_degamma)
	sq_tex_resource_word4 |= SQ_TEX_RESOURCE_WORD4_0__FORCE_DEGAMMA_bit;

    sq_tex_resource_word5 = ((tex_res->last_level << LAST_LEVEL_shift) |
			     (tex_res->base_array << BASE_ARRAY_shift) |
			     (tex_res->last_array << LAST_ARRAY_shift));

    sq_tex_resource_word6 = ((tex_res->min_lod << SQ_TEX_RESOURCE_WORD6_0__MIN_LOD_shift) |
			     (tex_res->perf_modulation << PERF_MODULATION_shift) |
			     (tile_split << SQ_TEX_RESOURCE_WORD6_0__TILE_SPLIT_shift));

    if (tex_res->interlaced)
	sq_tex_resource_word6 |= INTERLACED_bit;

    sq_tex_resource_word7 = ((tex_res->format << SQ_TEX_RESOURCE_WORD7_0__DATA_FORMAT_shift) |
			     (macro_aspect << SQ_TEX_RESOURCE_WORD7_0__MACRO_TILE_ASPECT_shift) |
			     (nbanks << SQ_TEX_RESOURCE_WORD7_0__NUM_BANKS_shift) |
			     (bankw << SQ_TEX_RESOURCE_WORD7_0__BANK_WIDTH_shift) |
			     (bankh << SQ_TEX_RESOURCE_WORD7_0__BANK_HEIGHT_shift) |
			     (SQ_TEX_VTX_VALID_TEXTURE << SQ_TEX_RESOURCE_WORD7_0__TYPE_shift));

    /* flush texture cache */
    evergreen_cp_set_surface_sync(pScrn, TC_ACTION_ENA_bit,
				  tex_res->size, tex_res->base,
				  tex_res->bo, domain, 0);

    BEGIN_BATCH(10 + 4);
    PACK0(SQ_FETCH_RESOURCE + tex_res->id * SQ_FETCH_RESOURCE_offset, 8);
    E32(sq_tex_resource_word0);
    E32(sq_tex_resource_word1);
    E32(((tex_res->base) >> 8));
    E32(((tex_res->mip_base) >> 8));
    E32(sq_tex_resource_word4);
    E32(sq_tex_resource_word5);
    E32(sq_tex_resource_word6);
    E32(sq_tex_resource_word7);
    RELOC_BATCH(tex_res->bo, domain, 0);
    RELOC_BATCH(tex_res->mip_bo, domain, 0);
    END_BATCH();
}

/* cayman has some minor differences in SQ_TEX_SAMPLER_WORD0_0,
 * but none that we use here.
 */
void
evergreen_set_tex_sampler (ScrnInfoPtr pScrn, tex_sampler_t *s)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t sq_tex_sampler_word0, sq_tex_sampler_word1, sq_tex_sampler_word2;

    sq_tex_sampler_word0 = ((s->clamp_x       << SQ_TEX_SAMPLER_WORD0_0__CLAMP_X_shift)		|
			    (s->clamp_y       << CLAMP_Y_shift)					|
			    (s->clamp_z       << CLAMP_Z_shift)					|
			    (s->xy_mag_filter << XY_MAG_FILTER_shift)				|
			    (s->xy_min_filter << XY_MIN_FILTER_shift)				|
			    (s->z_filter      << Z_FILTER_shift)	|
			    (s->mip_filter    << MIP_FILTER_shift)				|
			    (s->border_color  << BORDER_COLOR_TYPE_shift)			|
			    (s->depth_compare << DEPTH_COMPARE_FUNCTION_shift)			|
			    (s->chroma_key    << CHROMA_KEY_shift));

    sq_tex_sampler_word1 = ((s->min_lod       << SQ_TEX_SAMPLER_WORD1_0__MIN_LOD_shift)		|
			    (s->max_lod       << MAX_LOD_shift)					|
			    (s->perf_mip      << PERF_MIP_shift)	|
			    (s->perf_z        << PERF_Z_shift));


    sq_tex_sampler_word2 = ((s->lod_bias      << SQ_TEX_SAMPLER_WORD2_0__LOD_BIAS_shift) |
			    (s->lod_bias2     << LOD_BIAS_SEC_shift));

    if (s->mc_coord_truncate)
	sq_tex_sampler_word2 |= MC_COORD_TRUNCATE_bit;
    if (s->force_degamma)
	sq_tex_sampler_word2 |= SQ_TEX_SAMPLER_WORD2_0__FORCE_DEGAMMA_bit;
    if (s->truncate_coord)
	sq_tex_sampler_word2 |= TRUNCATE_COORD_bit;
    if (s->disable_cube_wrap)
	sq_tex_sampler_word2 |= SQ_TEX_SAMPLER_WORD2_0__DISABLE_CUBE_WRAP_bit;
    if (s->type)
	sq_tex_sampler_word2 |= SQ_TEX_SAMPLER_WORD2_0__TYPE_bit;

    BEGIN_BATCH(5);
    PACK0(SQ_TEX_SAMPLER_WORD + s->id * SQ_TEX_SAMPLER_WORD_offset, 3);
    E32(sq_tex_sampler_word0);
    E32(sq_tex_sampler_word1);
    E32(sq_tex_sampler_word2);
    END_BATCH();
}

/* workarounds for hw bugs in eg+ */
/* only affects screen/window/generic/vport.  cliprects are not affected */
static void
evergreen_fix_scissor_coordinates(ScrnInfoPtr pScrn, int *x1, int *y1, int *x2, int *y2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    /* all eg+ asics */
    if (*x2 == 0)
	*x1 = 1;
    if (*y2 == 0)
	*y1 = 1;

    /* cayman/tn only */
    if (info->ChipFamily >= CHIP_FAMILY_CAYMAN) {
	/* cliprects aren't affected so we can use them to clip if we need
	 * a true 1x1 clip region
	 */
	if ((*x2 == 1) && (*y2 == 1))
	    *x2 = 2;
    }
}

//XXX deal with clip offsets in clip setup
void
evergreen_set_screen_scissor(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    evergreen_fix_scissor_coordinates(pScrn, &x1, &y1, &x2, &y2);

    BEGIN_BATCH(4);
    PACK0(PA_SC_SCREEN_SCISSOR_TL, 2);
    E32(((x1 << PA_SC_SCREEN_SCISSOR_TL__TL_X_shift) |
	 (y1 << PA_SC_SCREEN_SCISSOR_TL__TL_Y_shift)));
    E32(((x2 << PA_SC_SCREEN_SCISSOR_BR__BR_X_shift) |
	 (y2 << PA_SC_SCREEN_SCISSOR_BR__BR_Y_shift)));
    END_BATCH();
}

void
evergreen_set_vport_scissor(ScrnInfoPtr pScrn, int id, int x1, int y1, int x2, int y2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    evergreen_fix_scissor_coordinates(pScrn, &x1, &y1, &x2, &y2);

    BEGIN_BATCH(4);
    PACK0(PA_SC_VPORT_SCISSOR_0_TL + id * PA_SC_VPORT_SCISSOR_0_TL_offset, 2);
    E32(((x1 << PA_SC_VPORT_SCISSOR_0_TL__TL_X_shift) |
	 (y1 << PA_SC_VPORT_SCISSOR_0_TL__TL_Y_shift) |
	 WINDOW_OFFSET_DISABLE_bit));
    E32(((x2 << PA_SC_VPORT_SCISSOR_0_BR__BR_X_shift) |
	 (y2 << PA_SC_VPORT_SCISSOR_0_BR__BR_Y_shift)));
    END_BATCH();
}

void
evergreen_set_generic_scissor(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    evergreen_fix_scissor_coordinates(pScrn, &x1, &y1, &x2, &y2);

    BEGIN_BATCH(4);
    PACK0(PA_SC_GENERIC_SCISSOR_TL, 2);
    E32(((x1 << PA_SC_GENERIC_SCISSOR_TL__TL_X_shift) |
	 (y1 << PA_SC_GENERIC_SCISSOR_TL__TL_Y_shift) |
	 WINDOW_OFFSET_DISABLE_bit));
    E32(((x2 << PA_SC_GENERIC_SCISSOR_BR__BR_X_shift) |
	 (y2 << PA_SC_GENERIC_SCISSOR_TL__TL_Y_shift)));
    END_BATCH();
}

void
evergreen_set_window_scissor(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    evergreen_fix_scissor_coordinates(pScrn, &x1, &y1, &x2, &y2);

    BEGIN_BATCH(4);
    PACK0(PA_SC_WINDOW_SCISSOR_TL, 2);
    E32(((x1 << PA_SC_WINDOW_SCISSOR_TL__TL_X_shift) |
	 (y1 << PA_SC_WINDOW_SCISSOR_TL__TL_Y_shift) |
	 WINDOW_OFFSET_DISABLE_bit));
    E32(((x2 << PA_SC_WINDOW_SCISSOR_BR__BR_X_shift) |
	 (y2 << PA_SC_WINDOW_SCISSOR_BR__BR_Y_shift)));
    END_BATCH();
}

void
evergreen_set_clip_rect(ScrnInfoPtr pScrn, int id, int x1, int y1, int x2, int y2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    BEGIN_BATCH(4);
    PACK0(PA_SC_CLIPRECT_0_TL + id * PA_SC_CLIPRECT_0_TL_offset, 2);
    E32(((x1 << PA_SC_CLIPRECT_0_TL__TL_X_shift) |
	 (y1 << PA_SC_CLIPRECT_0_TL__TL_Y_shift)));
    E32(((x2 << PA_SC_CLIPRECT_0_BR__BR_X_shift) |
	 (y2 << PA_SC_CLIPRECT_0_BR__BR_Y_shift)));
    END_BATCH();
}

/*
 * Setup of default state
 */

void
evergreen_set_default_state(ScrnInfoPtr pScrn)
{
    tex_resource_t tex_res;
    shader_config_t fs_conf;
    sq_config_t sq_conf;
    int i;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    if (info->ChipFamily >= CHIP_FAMILY_CAYMAN) {
	cayman_set_default_state(pScrn);
	return;
    }

    if (accel_state->XInited3D)
	return;

    memset(&tex_res, 0, sizeof(tex_resource_t));
    memset(&fs_conf, 0, sizeof(shader_config_t));

    accel_state->XInited3D = TRUE;

    evergreen_start_3d(pScrn);

    /* SQ */
    sq_conf.ps_prio = 0;
    sq_conf.vs_prio = 1;
    sq_conf.gs_prio = 2;
    sq_conf.es_prio = 3;
    sq_conf.hs_prio = 0;
    sq_conf.ls_prio = 0;
    sq_conf.cs_prio = 0;

    switch (info->ChipFamily) {
    case CHIP_FAMILY_CEDAR:
    default:
	sq_conf.num_ps_gprs = 93;
	sq_conf.num_vs_gprs = 46;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 31;
	sq_conf.num_es_gprs = 31;
	sq_conf.num_hs_gprs = 23;
	sq_conf.num_ls_gprs = 23;
	sq_conf.num_ps_threads = 96;
	sq_conf.num_vs_threads = 16;
	sq_conf.num_gs_threads = 16;
	sq_conf.num_es_threads = 16;
	sq_conf.num_hs_threads = 16;
	sq_conf.num_ls_threads = 16;
	sq_conf.num_ps_stack_entries = 42;
	sq_conf.num_vs_stack_entries = 42;
	sq_conf.num_gs_stack_entries = 42;
	sq_conf.num_es_stack_entries = 42;
	sq_conf.num_hs_stack_entries = 42;
	sq_conf.num_ls_stack_entries = 42;
	break;
    case CHIP_FAMILY_REDWOOD:
	sq_conf.num_ps_gprs = 93;
	sq_conf.num_vs_gprs = 46;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 31;
	sq_conf.num_es_gprs = 31;
	sq_conf.num_hs_gprs = 23;
	sq_conf.num_ls_gprs = 23;
	sq_conf.num_ps_threads = 128;
	sq_conf.num_vs_threads = 20;
	sq_conf.num_gs_threads = 20;
	sq_conf.num_es_threads = 20;
	sq_conf.num_hs_threads = 20;
	sq_conf.num_ls_threads = 20;
	sq_conf.num_ps_stack_entries = 42;
	sq_conf.num_vs_stack_entries = 42;
	sq_conf.num_gs_stack_entries = 42;
	sq_conf.num_es_stack_entries = 42;
	sq_conf.num_hs_stack_entries = 42;
	sq_conf.num_ls_stack_entries = 42;
	break;
    case CHIP_FAMILY_JUNIPER:
	sq_conf.num_ps_gprs = 93;
	sq_conf.num_vs_gprs = 46;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 31;
	sq_conf.num_es_gprs = 31;
	sq_conf.num_hs_gprs = 23;
	sq_conf.num_ls_gprs = 23;
	sq_conf.num_ps_threads = 128;
	sq_conf.num_vs_threads = 20;
	sq_conf.num_gs_threads = 20;
	sq_conf.num_es_threads = 20;
	sq_conf.num_hs_threads = 20;
	sq_conf.num_ls_threads = 20;
	sq_conf.num_ps_stack_entries = 85;
	sq_conf.num_vs_stack_entries = 85;
	sq_conf.num_gs_stack_entries = 85;
	sq_conf.num_es_stack_entries = 85;
	sq_conf.num_hs_stack_entries = 85;
	sq_conf.num_ls_stack_entries = 85;
	break;
    case CHIP_FAMILY_CYPRESS:
    case CHIP_FAMILY_HEMLOCK:
	sq_conf.num_ps_gprs = 93;
	sq_conf.num_vs_gprs = 46;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 31;
	sq_conf.num_es_gprs = 31;
	sq_conf.num_hs_gprs = 23;
	sq_conf.num_ls_gprs = 23;
	sq_conf.num_ps_threads = 128;
	sq_conf.num_vs_threads = 20;
	sq_conf.num_gs_threads = 20;
	sq_conf.num_es_threads = 20;
	sq_conf.num_hs_threads = 20;
	sq_conf.num_ls_threads = 20;
	sq_conf.num_ps_stack_entries = 85;
	sq_conf.num_vs_stack_entries = 85;
	sq_conf.num_gs_stack_entries = 85;
	sq_conf.num_es_stack_entries = 85;
	sq_conf.num_hs_stack_entries = 85;
	sq_conf.num_ls_stack_entries = 85;
	break;
    case CHIP_FAMILY_PALM:
	sq_conf.num_ps_gprs = 93;
	sq_conf.num_vs_gprs = 46;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 31;
	sq_conf.num_es_gprs = 31;
	sq_conf.num_hs_gprs = 23;
	sq_conf.num_ls_gprs = 23;
	sq_conf.num_ps_threads = 96;
	sq_conf.num_vs_threads = 16;
	sq_conf.num_gs_threads = 16;
	sq_conf.num_es_threads = 16;
	sq_conf.num_hs_threads = 16;
	sq_conf.num_ls_threads = 16;
	sq_conf.num_ps_stack_entries = 42;
	sq_conf.num_vs_stack_entries = 42;
	sq_conf.num_gs_stack_entries = 42;
	sq_conf.num_es_stack_entries = 42;
	sq_conf.num_hs_stack_entries = 42;
	sq_conf.num_ls_stack_entries = 42;
	break;
    case CHIP_FAMILY_SUMO:
	sq_conf.num_ps_gprs = 93;
	sq_conf.num_vs_gprs = 46;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 31;
	sq_conf.num_es_gprs = 31;
	sq_conf.num_hs_gprs = 23;
	sq_conf.num_ls_gprs = 23;
	sq_conf.num_ps_threads = 96;
	sq_conf.num_vs_threads = 25;
	sq_conf.num_gs_threads = 25;
	sq_conf.num_es_threads = 25;
	sq_conf.num_hs_threads = 25;
	sq_conf.num_ls_threads = 25;
	sq_conf.num_ps_stack_entries = 42;
	sq_conf.num_vs_stack_entries = 42;
	sq_conf.num_gs_stack_entries = 42;
	sq_conf.num_es_stack_entries = 42;
	sq_conf.num_hs_stack_entries = 42;
	sq_conf.num_ls_stack_entries = 42;
	break;
    case CHIP_FAMILY_SUMO2:
	sq_conf.num_ps_gprs = 93;
	sq_conf.num_vs_gprs = 46;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 31;
	sq_conf.num_es_gprs = 31;
	sq_conf.num_hs_gprs = 23;
	sq_conf.num_ls_gprs = 23;
	sq_conf.num_ps_threads = 96;
	sq_conf.num_vs_threads = 25;
	sq_conf.num_gs_threads = 25;
	sq_conf.num_es_threads = 25;
	sq_conf.num_hs_threads = 25;
	sq_conf.num_ls_threads = 25;
	sq_conf.num_ps_stack_entries = 85;
	sq_conf.num_vs_stack_entries = 85;
	sq_conf.num_gs_stack_entries = 85;
	sq_conf.num_es_stack_entries = 85;
	sq_conf.num_hs_stack_entries = 85;
	sq_conf.num_ls_stack_entries = 85;
	break;
    case CHIP_FAMILY_BARTS:
	sq_conf.num_ps_gprs = 93;
	sq_conf.num_vs_gprs = 46;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 31;
	sq_conf.num_es_gprs = 31;
	sq_conf.num_hs_gprs = 23;
	sq_conf.num_ls_gprs = 23;
	sq_conf.num_ps_threads = 128;
	sq_conf.num_vs_threads = 20;
	sq_conf.num_gs_threads = 20;
	sq_conf.num_es_threads = 20;
	sq_conf.num_hs_threads = 20;
	sq_conf.num_ls_threads = 20;
	sq_conf.num_ps_stack_entries = 85;
	sq_conf.num_vs_stack_entries = 85;
	sq_conf.num_gs_stack_entries = 85;
	sq_conf.num_es_stack_entries = 85;
	sq_conf.num_hs_stack_entries = 85;
	sq_conf.num_ls_stack_entries = 85;
	break;
    case CHIP_FAMILY_TURKS:
	sq_conf.num_ps_gprs = 93;
	sq_conf.num_vs_gprs = 46;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 31;
	sq_conf.num_es_gprs = 31;
	sq_conf.num_hs_gprs = 23;
	sq_conf.num_ls_gprs = 23;
	sq_conf.num_ps_threads = 128;
	sq_conf.num_vs_threads = 20;
	sq_conf.num_gs_threads = 20;
	sq_conf.num_es_threads = 20;
	sq_conf.num_hs_threads = 20;
	sq_conf.num_ls_threads = 20;
	sq_conf.num_ps_stack_entries = 42;
	sq_conf.num_vs_stack_entries = 42;
	sq_conf.num_gs_stack_entries = 42;
	sq_conf.num_es_stack_entries = 42;
	sq_conf.num_hs_stack_entries = 42;
	sq_conf.num_ls_stack_entries = 42;
	break;
    case CHIP_FAMILY_CAICOS:
	sq_conf.num_ps_gprs = 93;
	sq_conf.num_vs_gprs = 46;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 31;
	sq_conf.num_es_gprs = 31;
	sq_conf.num_hs_gprs = 23;
	sq_conf.num_ls_gprs = 23;
	sq_conf.num_ps_threads = 128;
	sq_conf.num_vs_threads = 10;
	sq_conf.num_gs_threads = 10;
	sq_conf.num_es_threads = 10;
	sq_conf.num_hs_threads = 10;
	sq_conf.num_ls_threads = 10;
	sq_conf.num_ps_stack_entries = 42;
	sq_conf.num_vs_stack_entries = 42;
	sq_conf.num_gs_stack_entries = 42;
	sq_conf.num_es_stack_entries = 42;
	sq_conf.num_hs_stack_entries = 42;
	sq_conf.num_ls_stack_entries = 42;
	break;
    }

    evergreen_sq_setup(pScrn, &sq_conf);

    BEGIN_BATCH(27);
    EREG(SQ_LDS_ALLOC_PS, 0);
    EREG(SQ_LDS_RESOURCE_MGMT, 0x10001000);
    EREG(SQ_DYN_GPR_RESOURCE_LIMIT_1, 0);

    PACK0(SQ_ESGS_RING_ITEMSIZE, 6);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);

    PACK0(SQ_GS_VERT_ITEMSIZE, 4);
    E32(0);
    E32(0);
    E32(0);
    E32(0);

    PACK0(SQ_VTX_BASE_VTX_LOC, 2);
    E32(0);
    E32(0);
    END_BATCH();

    /* DB */
    BEGIN_BATCH(3 + 2);
    EREG(DB_Z_INFO,                           0);
    RELOC_BATCH(accel_state->shaders_bo, RADEON_GEM_DOMAIN_VRAM, 0);
    END_BATCH();

    BEGIN_BATCH(3 + 2);
    EREG(DB_STENCIL_INFO,                     0);
    RELOC_BATCH(accel_state->shaders_bo, RADEON_GEM_DOMAIN_VRAM, 0);
    END_BATCH();

    BEGIN_BATCH(3 + 2);
    EREG(DB_HTILE_DATA_BASE,                    0);
    RELOC_BATCH(accel_state->shaders_bo, RADEON_GEM_DOMAIN_VRAM, 0);
    END_BATCH();

    BEGIN_BATCH(49);
    EREG(DB_DEPTH_CONTROL,                    0);

    PACK0(PA_SC_VPORT_ZMIN_0, 2);
    EFLOAT(0.0); // PA_SC_VPORT_ZMIN_0
    EFLOAT(1.0); // PA_SC_VPORT_ZMAX_0

    PACK0(DB_RENDER_CONTROL, 5);
    E32(STENCIL_COMPRESS_DISABLE_bit | DEPTH_COMPRESS_DISABLE_bit); // DB_RENDER_CONTROL
    E32(0); // DB_COUNT_CONTROL
    E32(0); // DB_DEPTH_VIEW
    E32(0x2a); // DB_RENDER_OVERRIDE
    E32(0); // DB_RENDER_OVERRIDE2

    PACK0(DB_STENCIL_CLEAR, 2);
    E32(0); // DB_STENCIL_CLEAR
    E32(0); // DB_DEPTH_CLEAR

    EREG(DB_ALPHA_TO_MASK,                    ((2 << ALPHA_TO_MASK_OFFSET0_shift)	|
					       (2 << ALPHA_TO_MASK_OFFSET1_shift)	|
					       (2 << ALPHA_TO_MASK_OFFSET2_shift)	|
					       (2 << ALPHA_TO_MASK_OFFSET3_shift)));

    EREG(DB_SHADER_CONTROL, ((EARLY_Z_THEN_LATE_Z << Z_ORDER_shift) |
			     DUAL_EXPORT_ENABLE_bit)); /* Only useful if no depth export */

    // SX
    EREG(SX_MISC,               0);

    // CB
    PACK0(SX_ALPHA_TEST_CONTROL, 5);
    E32(0); // SX_ALPHA_TEST_CONTROL
    E32(0x00000000); //CB_BLEND_RED
    E32(0x00000000); //CB_BLEND_GREEN
    E32(0x00000000); //CB_BLEND_BLUE
    E32(0x00000000); //CB_BLEND_ALPHA

    EREG(CB_SHADER_MASK,                      OUTPUT0_ENABLE_mask);

    // SC
    EREG(PA_SC_WINDOW_OFFSET,                 ((0 << WINDOW_X_OFFSET_shift) |
					       (0 << WINDOW_Y_OFFSET_shift)));
    EREG(PA_SC_CLIPRECT_RULE,                 CLIP_RULE_mask);
    EREG(PA_SC_EDGERULE,             0xAAAAAAAA);
    EREG(PA_SU_HARDWARE_SCREEN_OFFSET, 0);
    END_BATCH();

    /* clip boolean is set to always visible -> doesn't matter */
    for (i = 0; i < PA_SC_CLIPRECT_0_TL_num; i++)
	evergreen_set_clip_rect (pScrn, i, 0, 0, 8192, 8192);

    for (i = 0; i < PA_SC_VPORT_SCISSOR_0_TL_num; i++)
	evergreen_set_vport_scissor (pScrn, i, 0, 0, 8192, 8192);

    BEGIN_BATCH(57);
    PACK0(PA_SC_MODE_CNTL_0, 2);
    E32(0); // PA_SC_MODE_CNTL_0
    E32(0); // PA_SC_MODE_CNTL_1

    PACK0(PA_SC_LINE_CNTL, 16);
    E32(0); // PA_SC_LINE_CNTL
    E32(0); // PA_SC_AA_CONFIG
    E32(((X_ROUND_TO_EVEN << PA_SU_VTX_CNTL__ROUND_MODE_shift) |
	 PIX_CENTER_bit)); // PA_SU_VTX_CNTL
    EFLOAT(1.0);						// PA_CL_GB_VERT_CLIP_ADJ
    EFLOAT(1.0);						// PA_CL_GB_VERT_DISC_ADJ
    EFLOAT(1.0);						// PA_CL_GB_HORZ_CLIP_ADJ
    EFLOAT(1.0);						// PA_CL_GB_HORZ_DISC_ADJ
    E32(0); // PA_SC_AA_SAMPLE_LOCS_0
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0); // PA_SC_AA_SAMPLE_LOCS_7
    E32(0xFFFFFFFF); // PA_SC_AA_MASK

    // CL
    PACK0(PA_CL_CLIP_CNTL, 8);
    E32(CLIP_DISABLE_bit); // PA_CL_CLIP_CNTL
    E32(FACE_bit); // PA_SU_SC_MODE_CNTL
    E32(VTX_XY_FMT_bit); // PA_CL_VTE_CNTL
    E32(0); // PA_CL_VS_OUT_CNTL
    E32(0); // PA_CL_NANINF_CNTL
    E32(0); // PA_SU_LINE_STIPPLE_CNTL
    E32(0); // PA_SU_LINE_STIPPLE_SCALE
    E32(0); // PA_SU_PRIM_FILTER_CNTL

    // SU
    PACK0(PA_SU_POLY_OFFSET_DB_FMT_CNTL, 6);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);

    /* src = semantic id 0; mask = semantic id 1 */
    EREG(SPI_VS_OUT_ID_0, ((0 << SEMANTIC_0_shift) |
			   (1 << SEMANTIC_1_shift)));
    PACK0(SPI_PS_INPUT_CNTL_0 + (0 << 2), 2);
    /* SPI_PS_INPUT_CNTL_0 maps to GPR[0] - load with semantic id 0 */
    E32(((0    << SEMANTIC_shift)	|
	 (0x01 << DEFAULT_VAL_shift)));
    /* SPI_PS_INPUT_CNTL_1 maps to GPR[1] - load with semantic id 1 */
    E32(((1    << SEMANTIC_shift)	|
	 (0x01 << DEFAULT_VAL_shift)));

    PACK0(SPI_INPUT_Z, 8);
    E32(0); // SPI_INPUT_Z
    E32(0); // SPI_FOG_CNTL
    E32(LINEAR_CENTROID_ENA__X_ON_AT_CENTROID << LINEAR_CENTROID_ENA_shift); // SPI_BARYC_CNTL
    E32(0); // SPI_PS_IN_CONTROL_2
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    END_BATCH();

    // clear FS
    fs_conf.bo = accel_state->shaders_bo;
    evergreen_fs_setup(pScrn, &fs_conf, RADEON_GEM_DOMAIN_VRAM);

    // VGT
    BEGIN_BATCH(46);

    PACK0(VGT_MAX_VTX_INDX, 4);
    E32(0xffffff);
    E32(0);
    E32(0);
    E32(0);

    PACK0(VGT_INSTANCE_STEP_RATE_0, 2);
    E32(0);
    E32(0);

    PACK0(VGT_REUSE_OFF, 2);
    E32(0);
    E32(0);

    PACK0(PA_SU_POINT_SIZE, 17);
    E32(0); // PA_SU_POINT_SIZE
    E32(0); // PA_SU_POINT_MINMAX
    E32((8 << PA_SU_LINE_CNTL__WIDTH_shift)); /* Line width 1 pixel */ // PA_SU_LINE_CNTL
    E32(0); // PA_SC_LINE_STIPPLE
    E32(0); // VGT_OUTPUT_PATH_CNTL
    E32(0); // VGT_HOS_CNTL
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0);
    E32(0); // VGT_GS_MODE

    EREG(VGT_PRIMITIVEID_EN,                  0);
    EREG(VGT_MULTI_PRIM_IB_RESET_EN,          0);
    EREG(VGT_SHADER_STAGES_EN,          0);

    PACK0(VGT_STRMOUT_CONFIG, 2);
    E32(0);
    E32(0);
    END_BATCH();
}


/*
 * Commands
 */

void
evergreen_draw_auto(ScrnInfoPtr pScrn, draw_config_t *draw_conf)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    BEGIN_BATCH(10);
    EREG(VGT_PRIMITIVE_TYPE, draw_conf->prim_type);
    PACK3(IT_INDEX_TYPE, 1);
#if X_BYTE_ORDER == X_BIG_ENDIAN
    E32(IT_INDEX_TYPE_SWAP_MODE(ENDIAN_8IN32) | draw_conf->index_type);
#else
    E32(draw_conf->index_type);
#endif
    PACK3(IT_NUM_INSTANCES, 1);
    E32(draw_conf->num_instances);
    PACK3(IT_DRAW_INDEX_AUTO, 2);
    E32(draw_conf->num_indices);
    E32(draw_conf->vgt_draw_initiator);
    END_BATCH();
}

void evergreen_finish_op(ScrnInfoPtr pScrn, int vtx_size)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    draw_config_t   draw_conf;
    vtx_resource_t  vtx_res;

    if (accel_state->vbo.vb_start_op == -1)
      return;

    CLEAR (draw_conf);
    CLEAR (vtx_res);

    if (accel_state->vbo.vb_offset == accel_state->vbo.vb_start_op) {
	radeon_ib_discard(pScrn);
	radeon_cs_flush_indirect(pScrn);
	return;
    }

    /* Vertex buffer setup */
    accel_state->vbo.vb_size = accel_state->vbo.vb_offset - accel_state->vbo.vb_start_op;
    vtx_res.id              = SQ_FETCH_RESOURCE_vs;
    vtx_res.vtx_size_dw     = vtx_size / 4;
    vtx_res.vtx_num_entries = accel_state->vbo.vb_size / 4;
    vtx_res.vb_addr         = accel_state->vbo.vb_start_op;
    vtx_res.bo              = accel_state->vbo.vb_bo;
    vtx_res.dst_sel_x       = SQ_SEL_X;
    vtx_res.dst_sel_y       = SQ_SEL_Y;
    vtx_res.dst_sel_z       = SQ_SEL_Z;
    vtx_res.dst_sel_w       = SQ_SEL_W;
#if X_BYTE_ORDER == X_BIG_ENDIAN
    vtx_res.endian          = SQ_ENDIAN_8IN32;
#endif
    evergreen_set_vtx_resource(pScrn, &vtx_res, RADEON_GEM_DOMAIN_GTT);

    /* Draw */
    draw_conf.prim_type          = DI_PT_RECTLIST;
    draw_conf.vgt_draw_initiator = DI_SRC_SEL_AUTO_INDEX;
    draw_conf.num_instances      = 1;
    draw_conf.num_indices        = vtx_res.vtx_num_entries / vtx_res.vtx_size_dw;
    draw_conf.index_type         = DI_INDEX_SIZE_16_BIT;

    evergreen_draw_auto(pScrn, &draw_conf);

    /* sync dst surface */
    evergreen_cp_set_surface_sync(pScrn, (CB_ACTION_ENA_bit | CB0_DEST_BASE_ENA_bit),
				  accel_state->dst_size, 0,
				  accel_state->dst_obj.bo, 0, accel_state->dst_obj.domain);

    accel_state->vbo.vb_start_op = -1;
    accel_state->cbuf.vb_start_op = -1;
    accel_state->ib_reset_op = 0;

}

