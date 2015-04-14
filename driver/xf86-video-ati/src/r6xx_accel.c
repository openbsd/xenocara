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
 * Authors: Alex Deucher <alexander.deucher@amd.com>
 *          Matthias Hopf <mhopf@suse.de>
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xf86.h"

#include <errno.h>

#include "radeon.h"
#include "r600_shader.h"
#include "radeon_reg.h"
#include "r600_reg.h"
#include "r600_state.h"

#include "radeon_vbo.h"
#include "radeon_exa_shared.h"

static const uint32_t R600_ROP[16] = {
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

/* we try and batch operations together under KMS -
   but it doesn't work yet without misrendering */
#define KMS_MULTI_OP 1

/* Flush the indirect buffer to the kernel for submission to the card */
void R600CPFlushIndirect(ScrnInfoPtr pScrn)
{
    radeon_cs_flush_indirect(pScrn);
}

void R600IBDiscard(ScrnInfoPtr pScrn)
{
    radeon_ib_discard(pScrn);
}

void
r600_wait_3d_idle_clean(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    //flush caches, don't generate timestamp
    BEGIN_BATCH(5);
    PACK3(IT_EVENT_WRITE, 1);
    E32(CACHE_FLUSH_AND_INV_EVENT);
    // wait for 3D idle clean
    EREG(WAIT_UNTIL,                          (WAIT_3D_IDLE_bit |
						   WAIT_3D_IDLECLEAN_bit));
    END_BATCH();
}

void
r600_wait_3d_idle(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    BEGIN_BATCH(3);
    EREG(WAIT_UNTIL,                          WAIT_3D_IDLE_bit);
    END_BATCH();
}

void
r600_start_3d(ScrnInfoPtr pScrn)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    if (info->ChipFamily < CHIP_FAMILY_RV770) {
	BEGIN_BATCH(5);
	PACK3(IT_START_3D_CMDBUF, 1);
	E32(0);
    } else
	BEGIN_BATCH(3);

    PACK3(IT_CONTEXT_CONTROL, 2);
    E32(0x80000000);
    E32(0x80000000);
    END_BATCH();

}

/*
 * Setup of functional groups
 */

// asic stack/thread/gpr limits - need to query the drm
static void
r600_sq_setup(ScrnInfoPtr pScrn, sq_config_t *sq_conf)
{
    uint32_t sq_config, sq_gpr_resource_mgmt_1, sq_gpr_resource_mgmt_2;
    uint32_t sq_thread_resource_mgmt, sq_stack_resource_mgmt_1, sq_stack_resource_mgmt_2;
    RADEONInfoPtr info = RADEONPTR(pScrn);

    if ((info->ChipFamily == CHIP_FAMILY_RV610) ||
	(info->ChipFamily == CHIP_FAMILY_RV620) ||
	(info->ChipFamily == CHIP_FAMILY_RS780) ||
	(info->ChipFamily == CHIP_FAMILY_RS880) ||
	(info->ChipFamily == CHIP_FAMILY_RV710))
	sq_config = 0;						// no VC
    else
	sq_config = VC_ENABLE_bit;

    sq_config |= (DX9_CONSTS_bit |
		  ALU_INST_PREFER_VECTOR_bit |
		  (sq_conf->ps_prio << PS_PRIO_shift) |
		  (sq_conf->vs_prio << VS_PRIO_shift) |
		  (sq_conf->gs_prio << GS_PRIO_shift) |
		  (sq_conf->es_prio << ES_PRIO_shift));

    sq_gpr_resource_mgmt_1 = ((sq_conf->num_ps_gprs << NUM_PS_GPRS_shift) |
			      (sq_conf->num_vs_gprs << NUM_VS_GPRS_shift) |
			      (sq_conf->num_temp_gprs << NUM_CLAUSE_TEMP_GPRS_shift));
    sq_gpr_resource_mgmt_2 = ((sq_conf->num_gs_gprs << NUM_GS_GPRS_shift) |
			      (sq_conf->num_es_gprs << NUM_ES_GPRS_shift));

    sq_thread_resource_mgmt = ((sq_conf->num_ps_threads << NUM_PS_THREADS_shift) |
			       (sq_conf->num_vs_threads << NUM_VS_THREADS_shift) |
			       (sq_conf->num_gs_threads << NUM_GS_THREADS_shift) |
			       (sq_conf->num_es_threads << NUM_ES_THREADS_shift));

    sq_stack_resource_mgmt_1 = ((sq_conf->num_ps_stack_entries << NUM_PS_STACK_ENTRIES_shift) |
				(sq_conf->num_vs_stack_entries << NUM_VS_STACK_ENTRIES_shift));

    sq_stack_resource_mgmt_2 = ((sq_conf->num_gs_stack_entries << NUM_GS_STACK_ENTRIES_shift) |
				(sq_conf->num_es_stack_entries << NUM_ES_STACK_ENTRIES_shift));

    BEGIN_BATCH(8);
    PACK0(SQ_CONFIG, 6);
    E32(sq_config);
    E32(sq_gpr_resource_mgmt_1);
    E32(sq_gpr_resource_mgmt_2);
    E32(sq_thread_resource_mgmt);
    E32(sq_stack_resource_mgmt_1);
    E32(sq_stack_resource_mgmt_2);
    END_BATCH();
}

void
r600_set_render_target(ScrnInfoPtr pScrn, cb_config_t *cb_conf, uint32_t domain)
{
    uint32_t cb_color_info, cb_color_control;
    unsigned pitch, slice, h, array_mode;
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
	pitch = (cb_conf->surface->level[0].nblk_x >> 3) - 1;
	slice = ((cb_conf->surface->level[0].nblk_x * cb_conf->surface->level[0].nblk_y) / 64) - 1;
    } else
    {
	array_mode = cb_conf->array_mode;
	pitch = (cb_conf->w / 8) - 1;
	h = RADEON_ALIGN(cb_conf->h, 8);
	slice = ((cb_conf->w * h) / 64) - 1;
    }

    cb_color_info = ((cb_conf->endian      << ENDIAN_shift)				|
		     (cb_conf->format      << CB_COLOR0_INFO__FORMAT_shift)		|
		     (array_mode  << CB_COLOR0_INFO__ARRAY_MODE_shift)		|
		     (cb_conf->number_type << NUMBER_TYPE_shift)			|
		     (cb_conf->comp_swap   << COMP_SWAP_shift)				|
		     (cb_conf->tile_mode   << CB_COLOR0_INFO__TILE_MODE_shift));
    if (cb_conf->read_size)
	cb_color_info |= CB_COLOR0_INFO__READ_SIZE_bit;
    if (cb_conf->blend_clamp)
	cb_color_info |= BLEND_CLAMP_bit;
    if (cb_conf->clear_color)
	cb_color_info |= CLEAR_COLOR_bit;
    if (cb_conf->blend_bypass)
	cb_color_info |= BLEND_BYPASS_bit;
    if (cb_conf->blend_float32)
	cb_color_info |= BLEND_FLOAT32_bit;
    if (cb_conf->simple_float)
	cb_color_info |= SIMPLE_FLOAT_bit;
    if (cb_conf->round_mode)
	cb_color_info |= CB_COLOR0_INFO__ROUND_MODE_bit;
    if (cb_conf->tile_compact)
	cb_color_info |= TILE_COMPACT_bit;
    if (cb_conf->source_format)
	cb_color_info |= SOURCE_FORMAT_bit;

    BEGIN_BATCH(3 + 2);
    EREG((CB_COLOR0_BASE + (4 * cb_conf->id)), (cb_conf->base >> 8));
    RELOC_BATCH(cb_conf->bo, 0, domain);
    END_BATCH();

    // rv6xx workaround
    if ((info->ChipFamily > CHIP_FAMILY_R600) &&
        (info->ChipFamily < CHIP_FAMILY_RV770)) {
        BEGIN_BATCH(2);
        PACK3(IT_SURFACE_BASE_UPDATE, 1);
        E32((2 << cb_conf->id));
        END_BATCH();
    }
    /* Set CMASK & TILE buffer to the offset of color buffer as
     * we don't use those this shouldn't cause any issue and we
     * then have a valid cmd stream
     */
    BEGIN_BATCH(3 + 2);
    EREG((CB_COLOR0_TILE + (4 * cb_conf->id)), (0     >> 8));	// CMASK per-tile data base/256
    RELOC_BATCH(cb_conf->bo, 0, domain);
    END_BATCH();
    BEGIN_BATCH(3 + 2);
    EREG((CB_COLOR0_FRAG + (4 * cb_conf->id)), (0     >> 8));	// FMASK per-tile data base/256
    RELOC_BATCH(cb_conf->bo, 0, domain);
    END_BATCH();
    BEGIN_BATCH(9);
    // pitch only for ARRAY_LINEAR_GENERAL, other tiling modes require addrlib
    EREG((CB_COLOR0_SIZE + (4 * cb_conf->id)), ((pitch << PITCH_TILE_MAX_shift)	|
						    (slice << SLICE_TILE_MAX_shift)));
    EREG((CB_COLOR0_VIEW + (4 * cb_conf->id)), ((0    << SLICE_START_shift)		|
						    (0    << SLICE_MAX_shift)));
    EREG((CB_COLOR0_MASK + (4 * cb_conf->id)), ((0    << CMASK_BLOCK_MAX_shift)	|
						    (0    << FMASK_TILE_MAX_shift)));
    END_BATCH();

    BEGIN_BATCH(3 + 2);
    EREG((CB_COLOR0_INFO + (4 * cb_conf->id)), cb_color_info);
    RELOC_BATCH(cb_conf->bo, 0, domain);
    END_BATCH();

    BEGIN_BATCH(9);
    EREG(CB_TARGET_MASK,          (cb_conf->pmask << TARGET0_ENABLE_shift));
    cb_color_control = R600_ROP[cb_conf->rop] |
	(cb_conf->blend_enable << TARGET_BLEND_ENABLE_shift);
    if (info->ChipFamily == CHIP_FAMILY_R600) {
	/* no per-MRT blend on R600 */
	EREG(CB_COLOR_CONTROL,    cb_color_control);
	EREG(CB_BLEND_CONTROL,    cb_conf->blendcntl);
    } else {
	if (cb_conf->blend_enable)
	    cb_color_control |= PER_MRT_BLEND_bit;
	EREG(CB_COLOR_CONTROL,    cb_color_control);
	EREG(CB_BLEND0_CONTROL,   cb_conf->blendcntl);
    }
    END_BATCH();
}

static void
r600_cp_set_surface_sync(ScrnInfoPtr pScrn, uint32_t sync_type,
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
void
r600_cp_wait_vline_sync(ScrnInfoPtr pScrn, PixmapPtr pPix,
			xf86CrtcPtr crtc, int start, int stop)
{
    RADEONInfoPtr  info = RADEONPTR(pScrn);
    drmmode_crtc_private_ptr drmmode_crtc;

    if (!crtc)
        return;

    if (!crtc->enabled)
        return;

    if (pPix != pScrn->pScreen->GetScreenPixmap(pScrn->pScreen))
        return;

    start = max(start, crtc->y);
    stop = min(stop, crtc->y + crtc->mode.VDisplay);

    if (start >= stop)
        return;

    drmmode_crtc = crtc->driver_private;

    BEGIN_BATCH(11);
    /* set the VLINE range */
    EREG(AVIVO_D1MODE_VLINE_START_END, /* this is just a marker */
	 (start << AVIVO_D1MODE_VLINE_START_SHIFT) |
	 (stop << AVIVO_D1MODE_VLINE_END_SHIFT));

    /* tell the CP to poll the VLINE state register */
    PACK3(IT_WAIT_REG_MEM, 6);
    E32(IT_WAIT_REG | IT_WAIT_EQ);
    E32(IT_WAIT_ADDR(AVIVO_D1MODE_VLINE_STATUS));
    E32(0);
    E32(0);                          // Ref value
    E32(AVIVO_D1MODE_VLINE_STAT);    // Mask
    E32(10);                         // Wait interval
    /* add crtc reloc */
    PACK3(IT_NOP, 1);
    E32(drmmode_crtc->mode_crtc->crtc_id);
    END_BATCH();
}

void
r600_set_spi(ScrnInfoPtr pScrn, int vs_export_count, int num_interp)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    BEGIN_BATCH(8);
    /* Interpolator setup */
    EREG(SPI_VS_OUT_CONFIG, (vs_export_count << VS_EXPORT_COUNT_shift));
    PACK0(SPI_PS_IN_CONTROL_0, 3);
    E32((num_interp << NUM_INTERP_shift));
    E32(0);
    E32(0);
    END_BATCH();
}

void
r600_fs_setup(ScrnInfoPtr pScrn, shader_config_t *fs_conf, uint32_t domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t sq_pgm_resources;

    sq_pgm_resources = ((fs_conf->num_gprs << NUM_GPRS_shift) |
			(fs_conf->stack_size << STACK_SIZE_shift));

    if (fs_conf->dx10_clamp)
	sq_pgm_resources |= SQ_PGM_RESOURCES_FS__DX10_CLAMP_bit;

    BEGIN_BATCH(3 + 2);
    EREG(SQ_PGM_START_FS, fs_conf->shader_addr >> 8);
    RELOC_BATCH(fs_conf->bo, domain, 0);
    END_BATCH();

    BEGIN_BATCH(6);
    EREG(SQ_PGM_RESOURCES_FS, sq_pgm_resources);
    EREG(SQ_PGM_CF_OFFSET_FS, 0);
    END_BATCH();
}

void
r600_vs_setup(ScrnInfoPtr pScrn, shader_config_t *vs_conf, uint32_t domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t sq_pgm_resources;

    sq_pgm_resources = ((vs_conf->num_gprs << NUM_GPRS_shift) |
			(vs_conf->stack_size << STACK_SIZE_shift));

    if (vs_conf->dx10_clamp)
	sq_pgm_resources |= SQ_PGM_RESOURCES_VS__DX10_CLAMP_bit;
    if (vs_conf->fetch_cache_lines)
	sq_pgm_resources |= (vs_conf->fetch_cache_lines << FETCH_CACHE_LINES_shift);
    if (vs_conf->uncached_first_inst)
	sq_pgm_resources |= UNCACHED_FIRST_INST_bit;

    /* flush SQ cache */
    r600_cp_set_surface_sync(pScrn, SH_ACTION_ENA_bit,
			     vs_conf->shader_size, vs_conf->shader_addr,
			     vs_conf->bo, domain, 0);

    BEGIN_BATCH(3 + 2);
    EREG(SQ_PGM_START_VS, vs_conf->shader_addr >> 8);
    RELOC_BATCH(vs_conf->bo, domain, 0);
    END_BATCH();

    BEGIN_BATCH(6);
    EREG(SQ_PGM_RESOURCES_VS, sq_pgm_resources);
    EREG(SQ_PGM_CF_OFFSET_VS, 0);
    END_BATCH();
}

void
r600_ps_setup(ScrnInfoPtr pScrn, shader_config_t *ps_conf, uint32_t domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t sq_pgm_resources;

    sq_pgm_resources = ((ps_conf->num_gprs << NUM_GPRS_shift) |
			(ps_conf->stack_size << STACK_SIZE_shift));

    if (ps_conf->dx10_clamp)
	sq_pgm_resources |= SQ_PGM_RESOURCES_PS__DX10_CLAMP_bit;
    if (ps_conf->fetch_cache_lines)
	sq_pgm_resources |= (ps_conf->fetch_cache_lines << FETCH_CACHE_LINES_shift);
    if (ps_conf->uncached_first_inst)
	sq_pgm_resources |= UNCACHED_FIRST_INST_bit;
    if (ps_conf->clamp_consts)
	sq_pgm_resources |= CLAMP_CONSTS_bit;

    /* flush SQ cache */
    r600_cp_set_surface_sync(pScrn, SH_ACTION_ENA_bit,
			     ps_conf->shader_size, ps_conf->shader_addr,
			     ps_conf->bo, domain, 0);

    BEGIN_BATCH(3 + 2);
    EREG(SQ_PGM_START_PS, ps_conf->shader_addr >> 8);
    RELOC_BATCH(ps_conf->bo, domain, 0);
    END_BATCH();

    BEGIN_BATCH(9);
    EREG(SQ_PGM_RESOURCES_PS, sq_pgm_resources);
    EREG(SQ_PGM_EXPORTS_PS, ps_conf->export_mode);
    EREG(SQ_PGM_CF_OFFSET_PS, 0);
    END_BATCH();
}

void
r600_set_alu_consts(ScrnInfoPtr pScrn, int offset, int count, float *const_buf)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    int i;
    const int countreg = count * (SQ_ALU_CONSTANT_offset >> 2);

    BEGIN_BATCH(2 + countreg);
    PACK0(SQ_ALU_CONSTANT + offset * SQ_ALU_CONSTANT_offset, countreg);
    for (i = 0; i < countreg; i++)
	EFLOAT(const_buf[i]);
    END_BATCH();
}

void
r600_set_bool_consts(ScrnInfoPtr pScrn, int offset, uint32_t val)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    /* bool register order is: ps, vs, gs; one register each
     * 1 bits per bool; 32 bools each for ps, vs, gs.
     */
    BEGIN_BATCH(3);
    EREG(SQ_BOOL_CONST + offset * SQ_BOOL_CONST_offset, val);
    END_BATCH();
}

static void
r600_set_vtx_resource(ScrnInfoPtr pScrn, vtx_resource_t *res, uint32_t domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;
    uint32_t sq_vtx_constant_word2;

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

    /* flush vertex cache */
    if ((info->ChipFamily == CHIP_FAMILY_RV610) ||
	(info->ChipFamily == CHIP_FAMILY_RV620) ||
	(info->ChipFamily == CHIP_FAMILY_RS780) ||
	(info->ChipFamily == CHIP_FAMILY_RS880) ||
	(info->ChipFamily == CHIP_FAMILY_RV710))
	r600_cp_set_surface_sync(pScrn, TC_ACTION_ENA_bit,
				 accel_state->vbo.vb_offset, 0,
				 res->bo,
				 domain, 0);
    else
	r600_cp_set_surface_sync(pScrn, VC_ACTION_ENA_bit,
				 accel_state->vbo.vb_offset, 0,
				 res->bo,
				 domain, 0);

    BEGIN_BATCH(9 + 2);
    PACK0(SQ_VTX_RESOURCE + res->id * SQ_VTX_RESOURCE_offset, 7);
    E32(res->vb_addr & 0xffffffff);				// 0: BASE_ADDRESS
    E32((res->vtx_num_entries << 2) - 1);			// 1: SIZE
    E32(sq_vtx_constant_word2);	// 2: BASE_HI, STRIDE, CLAMP, FORMAT, ENDIAN
    E32(res->mem_req_size << MEM_REQUEST_SIZE_shift);		// 3: MEM_REQUEST_SIZE ?!?
    E32(0);							// 4: n/a
    E32(0);							// 5: n/a
    E32(SQ_TEX_VTX_VALID_BUFFER << SQ_VTX_CONSTANT_WORD6_0__TYPE_shift);	// 6: TYPE
    RELOC_BATCH(res->bo, domain, 0);
    END_BATCH();
}

void
r600_set_tex_resource(ScrnInfoPtr pScrn, tex_resource_t *tex_res, uint32_t domain)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t sq_tex_resource_word0, sq_tex_resource_word1, sq_tex_resource_word4;
    uint32_t sq_tex_resource_word5, sq_tex_resource_word6;
    uint32_t array_mode, pitch;

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
    } else
    {
	array_mode = tex_res->tile_mode;
	pitch = (tex_res->pitch + 7) >> 3;
    }

    sq_tex_resource_word0 = ((tex_res->dim << DIM_shift) |
		     (array_mode << SQ_TEX_RESOURCE_WORD0_0__TILE_MODE_shift));

    if (tex_res->w)
	sq_tex_resource_word0 |= (((pitch - 1) << PITCH_shift) |
				  ((tex_res->w - 1) << TEX_WIDTH_shift));

    if (tex_res->tile_type)
	sq_tex_resource_word0 |= TILE_TYPE_bit;

    sq_tex_resource_word1 = (tex_res->format << SQ_TEX_RESOURCE_WORD1_0__DATA_FORMAT_shift);

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
			     (tex_res->request_size << REQUEST_SIZE_shift) |
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

    sq_tex_resource_word6 = ((tex_res->mpeg_clamp << MPEG_CLAMP_shift) |
			     (tex_res->perf_modulation << PERF_MODULATION_shift) |
			     (SQ_TEX_VTX_VALID_TEXTURE << SQ_TEX_RESOURCE_WORD6_0__TYPE_shift));

    if (tex_res->interlaced)
	sq_tex_resource_word6 |= INTERLACED_bit;

    /* flush texture cache */
    r600_cp_set_surface_sync(pScrn, TC_ACTION_ENA_bit,
			     tex_res->size, tex_res->base,
			     tex_res->bo, domain, 0);

    BEGIN_BATCH(9 + 4);
    PACK0(SQ_TEX_RESOURCE + tex_res->id * SQ_TEX_RESOURCE_offset, 7);
    E32(sq_tex_resource_word0);
    E32(sq_tex_resource_word1);
    E32(((tex_res->base) >> 8));
    E32(((tex_res->mip_base) >> 8));
    E32(sq_tex_resource_word4);
    E32(sq_tex_resource_word5);
    E32(sq_tex_resource_word6);
    RELOC_BATCH(tex_res->bo, domain, 0);
    RELOC_BATCH(tex_res->mip_bo, domain, 0);
    END_BATCH();
}

void
r600_set_tex_sampler (ScrnInfoPtr pScrn, tex_sampler_t *s)
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
    if (s->point_sampling_clamp)
	sq_tex_sampler_word0 |= POINT_SAMPLING_CLAMP_bit;
    if (s->tex_array_override)
	sq_tex_sampler_word0 |= TEX_ARRAY_OVERRIDE_bit;
    if (s->lod_uses_minor_axis)
	sq_tex_sampler_word0 |= LOD_USES_MINOR_AXIS_bit;

    sq_tex_sampler_word1 = ((s->min_lod       << MIN_LOD_shift)					|
			    (s->max_lod       << MAX_LOD_shift)					|
			    (s->lod_bias      << SQ_TEX_SAMPLER_WORD1_0__LOD_BIAS_shift));

    sq_tex_sampler_word2 = ((s->lod_bias2     << LOD_BIAS_SEC_shift)	|
			    (s->perf_mip      << PERF_MIP_shift)	|
			    (s->perf_z        << PERF_Z_shift));
    if (s->mc_coord_truncate)
	sq_tex_sampler_word2 |= MC_COORD_TRUNCATE_bit;
    if (s->force_degamma)
	sq_tex_sampler_word2 |= SQ_TEX_SAMPLER_WORD2_0__FORCE_DEGAMMA_bit;
    if (s->high_precision_filter)
	sq_tex_sampler_word2 |= HIGH_PRECISION_FILTER_bit;
    if (s->fetch_4)
	sq_tex_sampler_word2 |= FETCH_4_bit;
    if (s->sample_is_pcf)
	sq_tex_sampler_word2 |= SAMPLE_IS_PCF_bit;
    if (s->type)
	sq_tex_sampler_word2 |= SQ_TEX_SAMPLER_WORD2_0__TYPE_bit;

    BEGIN_BATCH(5);
    PACK0(SQ_TEX_SAMPLER_WORD + s->id * SQ_TEX_SAMPLER_WORD_offset, 3);
    E32(sq_tex_sampler_word0);
    E32(sq_tex_sampler_word1);
    E32(sq_tex_sampler_word2);
    END_BATCH();
}

//XXX deal with clip offsets in clip setup
void
r600_set_screen_scissor(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

    BEGIN_BATCH(4);
    PACK0(PA_SC_SCREEN_SCISSOR_TL, 2);
    E32(((x1 << PA_SC_SCREEN_SCISSOR_TL__TL_X_shift) |
	     (y1 << PA_SC_SCREEN_SCISSOR_TL__TL_Y_shift)));
    E32(((x2 << PA_SC_SCREEN_SCISSOR_BR__BR_X_shift) |
	     (y2 << PA_SC_SCREEN_SCISSOR_BR__BR_Y_shift)));
    END_BATCH();
}

void
r600_set_vport_scissor(ScrnInfoPtr pScrn, int id, int x1, int y1, int x2, int y2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

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
r600_set_generic_scissor(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

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
r600_set_window_scissor(ScrnInfoPtr pScrn, int x1, int y1, int x2, int y2)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);

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
r600_set_clip_rect(ScrnInfoPtr pScrn, int id, int x1, int y1, int x2, int y2)
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
r600_set_default_state(ScrnInfoPtr pScrn)
{
    tex_resource_t tex_res;
    shader_config_t fs_conf;
    sq_config_t sq_conf;
    int i;
    RADEONInfoPtr info = RADEONPTR(pScrn);
    struct radeon_accel_state *accel_state = info->accel_state;

    if (accel_state->XInited3D)
	return;

    memset(&tex_res, 0, sizeof(tex_resource_t));
    memset(&fs_conf, 0, sizeof(shader_config_t));

    accel_state->XInited3D = TRUE;

    r600_start_3d(pScrn);

    // SQ
    sq_conf.ps_prio = 0;
    sq_conf.vs_prio = 1;
    sq_conf.gs_prio = 2;
    sq_conf.es_prio = 3;
    // need to set stack/thread/gpr limits based on the asic
    // for now just set them low enough so any card will work
    // see r600_cp.c in the drm
    switch (info->ChipFamily) {
    case CHIP_FAMILY_R600:
	sq_conf.num_ps_gprs = 192;
	sq_conf.num_vs_gprs = 56;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 0;
	sq_conf.num_es_gprs = 0;
	sq_conf.num_ps_threads = 136;
	sq_conf.num_vs_threads = 48;
	sq_conf.num_gs_threads = 4;
	sq_conf.num_es_threads = 4;
	sq_conf.num_ps_stack_entries = 128;
	sq_conf.num_vs_stack_entries = 128;
	sq_conf.num_gs_stack_entries = 0;
	sq_conf.num_es_stack_entries = 0;
	break;
    case CHIP_FAMILY_RV630:
    case CHIP_FAMILY_RV635:
	sq_conf.num_ps_gprs = 84;
	sq_conf.num_vs_gprs = 36;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 0;
	sq_conf.num_es_gprs = 0;
	sq_conf.num_ps_threads = 144;
	sq_conf.num_vs_threads = 40;
	sq_conf.num_gs_threads = 4;
	sq_conf.num_es_threads = 4;
	sq_conf.num_ps_stack_entries = 40;
	sq_conf.num_vs_stack_entries = 40;
	sq_conf.num_gs_stack_entries = 32;
	sq_conf.num_es_stack_entries = 16;
	break;
    case CHIP_FAMILY_RV610:
    case CHIP_FAMILY_RV620:
    case CHIP_FAMILY_RS780:
    case CHIP_FAMILY_RS880:
    default:
	sq_conf.num_ps_gprs = 84;
	sq_conf.num_vs_gprs = 36;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 0;
	sq_conf.num_es_gprs = 0;
	sq_conf.num_ps_threads = 136;
	sq_conf.num_vs_threads = 48;
	sq_conf.num_gs_threads = 4;
	sq_conf.num_es_threads = 4;
	sq_conf.num_ps_stack_entries = 40;
	sq_conf.num_vs_stack_entries = 40;
	sq_conf.num_gs_stack_entries = 32;
	sq_conf.num_es_stack_entries = 16;
	break;
    case CHIP_FAMILY_RV670:
	sq_conf.num_ps_gprs = 144;
	sq_conf.num_vs_gprs = 40;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 0;
	sq_conf.num_es_gprs = 0;
	sq_conf.num_ps_threads = 136;
	sq_conf.num_vs_threads = 48;
	sq_conf.num_gs_threads = 4;
	sq_conf.num_es_threads = 4;
	sq_conf.num_ps_stack_entries = 40;
	sq_conf.num_vs_stack_entries = 40;
	sq_conf.num_gs_stack_entries = 32;
	sq_conf.num_es_stack_entries = 16;
	break;
    case CHIP_FAMILY_RV770:
	sq_conf.num_ps_gprs = 192;
	sq_conf.num_vs_gprs = 56;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 0;
	sq_conf.num_es_gprs = 0;
	sq_conf.num_ps_threads = 188;
	sq_conf.num_vs_threads = 60;
	sq_conf.num_gs_threads = 0;
	sq_conf.num_es_threads = 0;
	sq_conf.num_ps_stack_entries = 256;
	sq_conf.num_vs_stack_entries = 256;
	sq_conf.num_gs_stack_entries = 0;
	sq_conf.num_es_stack_entries = 0;
	break;
    case CHIP_FAMILY_RV730:
    case CHIP_FAMILY_RV740:
	sq_conf.num_ps_gprs = 84;
	sq_conf.num_vs_gprs = 36;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 0;
	sq_conf.num_es_gprs = 0;
	sq_conf.num_ps_threads = 188;
	sq_conf.num_vs_threads = 60;
	sq_conf.num_gs_threads = 0;
	sq_conf.num_es_threads = 0;
	sq_conf.num_ps_stack_entries = 128;
	sq_conf.num_vs_stack_entries = 128;
	sq_conf.num_gs_stack_entries = 0;
	sq_conf.num_es_stack_entries = 0;
	break;
    case CHIP_FAMILY_RV710:
	sq_conf.num_ps_gprs = 192;
	sq_conf.num_vs_gprs = 56;
	sq_conf.num_temp_gprs = 4;
	sq_conf.num_gs_gprs = 0;
	sq_conf.num_es_gprs = 0;
	sq_conf.num_ps_threads = 144;
	sq_conf.num_vs_threads = 48;
	sq_conf.num_gs_threads = 0;
	sq_conf.num_es_threads = 0;
	sq_conf.num_ps_stack_entries = 128;
	sq_conf.num_vs_stack_entries = 128;
	sq_conf.num_gs_stack_entries = 0;
	sq_conf.num_es_stack_entries = 0;
	break;
    }

    r600_sq_setup(pScrn, &sq_conf);

    /* set fake reloc for unused depth */
    BEGIN_BATCH(3 + 2);
    EREG(DB_DEPTH_INFO, 0);
    RELOC_BATCH(accel_state->shaders_bo, RADEON_GEM_DOMAIN_VRAM, 0);
    END_BATCH();

    BEGIN_BATCH(80);
    if (info->ChipFamily < CHIP_FAMILY_RV770) {
	EREG(TA_CNTL_AUX, (( 3 << GRADIENT_CREDIT_shift) |
			       (28 << TD_FIFO_CREDIT_shift)));
	EREG(VC_ENHANCE, 0);
	EREG(R7xx_SQ_DYN_GPR_CNTL_PS_FLUSH_REQ, 0);
	EREG(DB_DEBUG, 0x82000000); /* ? */
	EREG(DB_WATERMARKS, ((4 << DEPTH_FREE_shift) |
				 (16 << DEPTH_FLUSH_shift) |
				 (0 << FORCE_SUMMARIZE_shift) |
				 (4 << DEPTH_PENDING_FREE_shift) |
				 (16 << DEPTH_CACHELINE_FREE_shift) |
				 0));
    } else {
	EREG(TA_CNTL_AUX, (( 2 << GRADIENT_CREDIT_shift) |
			       (28 << TD_FIFO_CREDIT_shift)));
	EREG(VC_ENHANCE, 0);
	EREG(R7xx_SQ_DYN_GPR_CNTL_PS_FLUSH_REQ, VS_PC_LIMIT_ENABLE_bit);
	EREG(DB_DEBUG, 0);
	EREG(DB_WATERMARKS, ((4 << DEPTH_FREE_shift) |
				 (16 << DEPTH_FLUSH_shift) |
				 (0 << FORCE_SUMMARIZE_shift) |
				 (4 << DEPTH_PENDING_FREE_shift) |
				 (4 << DEPTH_CACHELINE_FREE_shift) |
				 0));
    }

    PACK0(SQ_VTX_BASE_VTX_LOC, 2);
    E32(0);
    E32(0);

    PACK0(SQ_ESGS_RING_ITEMSIZE, 9);
    E32(0); // SQ_ESGS_RING_ITEMSIZE
    E32(0); // SQ_GSVS_RING_ITEMSIZE
    E32(0); // SQ_ESTMP_RING_ITEMSIZE
    E32(0); // SQ_GSTMP_RING_ITEMSIZE
    E32(0); // SQ_VSTMP_RING_ITEMSIZE
    E32(0); // SQ_PSTMP_RING_ITEMSIZE
    E32(0); // SQ_FBUF_RING_ITEMSIZE
    E32(0); // SQ_REDUC_RING_ITEMSIZE
    E32(0); // SQ_GS_VERT_ITEMSIZE

    // DB
    EREG(DB_DEPTH_CONTROL,                    0);
    PACK0(DB_RENDER_CONTROL, 2);
    E32(STENCIL_COMPRESS_DISABLE_bit | DEPTH_COMPRESS_DISABLE_bit);
    if (info->ChipFamily < CHIP_FAMILY_RV770)
	E32(FORCE_SHADER_Z_ORDER_bit);
    else
	E32(0);
    EREG(DB_ALPHA_TO_MASK,                    ((2 << ALPHA_TO_MASK_OFFSET0_shift)	|
						   (2 << ALPHA_TO_MASK_OFFSET1_shift)	|
						   (2 << ALPHA_TO_MASK_OFFSET2_shift)	|
						   (2 << ALPHA_TO_MASK_OFFSET3_shift)));
    EREG(DB_SHADER_CONTROL, ((1 << Z_ORDER_shift) | /* EARLY_Z_THEN_LATE_Z */
				 DUAL_EXPORT_ENABLE_bit)); /* Only useful if no depth export */

    PACK0(DB_STENCIL_CLEAR, 2);
    E32(0); // DB_STENCIL_CLEAR
    E32(0); // DB_DEPTH_CLEAR

    PACK0(DB_STENCILREFMASK, 3);
    E32(0); // DB_STENCILREFMASK
    E32(0); // DB_STENCILREFMASK_BF
    E32(0); // SX_ALPHA_REF

    PACK0(CB_CLRCMP_CONTROL, 4);
    E32(1 << CLRCMP_FCN_SEL_shift);				// CB_CLRCMP_CONTROL: use CLRCMP_FCN_SRC
    E32(0);							// CB_CLRCMP_SRC
    E32(0);							// CB_CLRCMP_DST
    E32(0);							// CB_CLRCMP_MSK

    EREG(CB_SHADER_MASK,                      OUTPUT0_ENABLE_mask);
    EREG(R7xx_CB_SHADER_CONTROL,              (RT0_ENABLE_bit));

    PACK0(SX_ALPHA_TEST_CONTROL, 5);
    E32(0); // SX_ALPHA_TEST_CONTROL
    E32(0x00000000); // CB_BLEND_RED
    E32(0x00000000); // CB_BLEND_GREEN
    E32(0x00000000); // CB_BLEND_BLUE
    E32(0x00000000); // CB_BLEND_ALPHA

    EREG(PA_SC_WINDOW_OFFSET,                 ((0 << WINDOW_X_OFFSET_shift) |
						   (0 << WINDOW_Y_OFFSET_shift)));

    if (info->ChipFamily < CHIP_FAMILY_RV770)
	EREG(R7xx_PA_SC_EDGERULE,             0x00000000);
    else
	EREG(R7xx_PA_SC_EDGERULE,             0xAAAAAAAA);

    EREG(PA_SC_CLIPRECT_RULE,                 CLIP_RULE_mask);

    END_BATCH();

    /* clip boolean is set to always visible -> doesn't matter */
    for (i = 0; i < PA_SC_CLIPRECT_0_TL_num; i++)
	r600_set_clip_rect(pScrn, i, 0, 0, 8192, 8192);

    for (i = 0; i < PA_SC_VPORT_SCISSOR_0_TL_num; i++)
	r600_set_vport_scissor(pScrn, i, 0, 0, 8192, 8192);

    BEGIN_BATCH(49);
    PACK0(PA_SC_MPASS_PS_CNTL, 2);
    E32(0);
    if (info->ChipFamily < CHIP_FAMILY_RV770)
	E32((WALK_ORDER_ENABLE_bit | FORCE_EOV_CNTDWN_ENABLE_bit));
    else
	E32((FORCE_EOV_CNTDWN_ENABLE_bit | FORCE_EOV_REZ_ENABLE_bit |
		 0x00500000)); /* ? */

    PACK0(PA_SC_LINE_CNTL, 9);
    E32(0); // PA_SC_LINE_CNTL
    E32(0); // PA_SC_AA_CONFIG
    E32(((2 << PA_SU_VTX_CNTL__ROUND_MODE_shift) | PIX_CENTER_bit | // PA_SU_VTX_CNTL
	     (5 << QUANT_MODE_shift))); /* Round to Even, fixed point 1/256 */
    EFLOAT(1.0);						// PA_CL_GB_VERT_CLIP_ADJ
    EFLOAT(1.0);						// PA_CL_GB_VERT_DISC_ADJ
    EFLOAT(1.0);						// PA_CL_GB_HORZ_CLIP_ADJ
    EFLOAT(1.0);						// PA_CL_GB_HORZ_DISC_ADJ
    E32(0);                                                 // PA_SC_AA_SAMPLE_LOCS_MCTX
    E32(0);                                                 // PA_SC_AA_SAMPLE_LOCS_8S_WD1_M

    EREG(PA_SC_AA_MASK,                       0xFFFFFFFF);

    PACK0(PA_CL_CLIP_CNTL, 5);
    E32(CLIP_DISABLE_bit); // PA_CL_CLIP_CNTL
    E32(FACE_bit);         // PA_SU_SC_MODE_CNTL
    E32(VTX_XY_FMT_bit);   // PA_CL_VTE_CNTL
    E32(0);                // PA_CL_VS_OUT_CNTL
    E32(0);                // PA_CL_NANINF_CNTL

    PACK0(PA_SU_POLY_OFFSET_DB_FMT_CNTL, 6);
    E32(0); // PA_SU_POLY_OFFSET_DB_FMT_CNTL
    E32(0); // PA_SU_POLY_OFFSET_CLAMP
    E32(0); // PA_SU_POLY_OFFSET_FRONT_SCALE
    E32(0); // PA_SU_POLY_OFFSET_FRONT_OFFSET
    E32(0); // PA_SU_POLY_OFFSET_BACK_SCALE
    E32(0); // PA_SU_POLY_OFFSET_BACK_OFFSET

    // SPI
    if (info->ChipFamily < CHIP_FAMILY_RV770)
	EREG(R7xx_SPI_THREAD_GROUPING,        0);
    else
	EREG(R7xx_SPI_THREAD_GROUPING,        (1 << PS_GROUPING_shift));

    /* default Interpolator setup */
    EREG(SPI_VS_OUT_ID_0, ((0 << SEMANTIC_0_shift) |
			       (1 << SEMANTIC_1_shift)));
    PACK0(SPI_PS_INPUT_CNTL_0 + (0 << 2), 2);
    /* SPI_PS_INPUT_CNTL_0 maps to GPR[0] - load with semantic id 0 */
    E32(((0    << SEMANTIC_shift)	|
	     (0x01 << DEFAULT_VAL_shift)	|
	     SEL_CENTROID_bit));
    /* SPI_PS_INPUT_CNTL_1 maps to GPR[1] - load with semantic id 1 */
    E32(((1    << SEMANTIC_shift)	|
	     (0x01 << DEFAULT_VAL_shift)	|
	     SEL_CENTROID_bit));

    PACK0(SPI_INPUT_Z, 4);
    E32(0); // SPI_INPUT_Z
    E32(0); // SPI_FOG_CNTL
    E32(0); // SPI_FOG_FUNC_SCALE
    E32(0); // SPI_FOG_FUNC_BIAS

    END_BATCH();

    // clear FS
    fs_conf.bo = accel_state->shaders_bo;
    r600_fs_setup(pScrn, &fs_conf, RADEON_GEM_DOMAIN_VRAM);

    // VGT
    BEGIN_BATCH(46);
    PACK0(VGT_MAX_VTX_INDX, 4);
    E32(0xffffff); // VGT_MAX_VTX_INDX
    E32(0); // VGT_MIN_VTX_INDX
    E32(0); // VGT_INDX_OFFSET
    E32(0); // VGT_MULTI_PRIM_IB_RESET_INDX

    EREG(VGT_PRIMITIVEID_EN,                  0);
    EREG(VGT_MULTI_PRIM_IB_RESET_EN,          0);

    PACK0(VGT_INSTANCE_STEP_RATE_0, 2);
    E32(0); // VGT_INSTANCE_STEP_RATE_0
    E32(0); // VGT_INSTANCE_STEP_RATE_1

    PACK0(PA_SU_POINT_SIZE, 17);
    E32(0); // PA_SU_POINT_SIZE
    E32(0); // PA_SU_POINT_MINMAX
    E32((8 << PA_SU_LINE_CNTL__WIDTH_shift)); /* Line width 1 pixel */ // PA_SU_LINE_CNTL
    E32(0); // PA_SC_LINE_STIPPLE
    E32(0); // VGT_OUTPUT_PATH_CNTL
    E32(0); // VGT_HOS_CNTL
    E32(0); // VGT_HOS_MAX_TESS_LEVEL
    E32(0); // VGT_HOS_MIN_TESS_LEVEL
    E32(0); // VGT_HOS_REUSE_DEPTH
    E32(0); // VGT_GROUP_PRIM_TYPE
    E32(0); // VGT_GROUP_FIRST_DECR
    E32(0); // VGT_GROUP_DECR
    E32(0); // VGT_GROUP_VECT_0_CNTL
    E32(0); // VGT_GROUP_VECT_1_CNTL
    E32(0); // VGT_GROUP_VECT_0_FMT_CNTL
    E32(0); // VGT_GROUP_VECT_1_FMT_CNTL
    E32(0); // VGT_GS_MODE

    PACK0(VGT_STRMOUT_EN, 3);
    E32(0); // VGT_STRMOUT_EN
    E32(0); // VGT_REUSE_OFF
    E32(0); // VGT_VTX_CNT_EN

    EREG(VGT_STRMOUT_BUFFER_EN,               0);
    EREG(SX_MISC,                             0);
    END_BATCH();
}


/*
 * Commands
 */

void
r600_draw_immd(ScrnInfoPtr pScrn, draw_config_t *draw_conf, uint32_t *indices)
{
    RADEONInfoPtr info = RADEONPTR(pScrn);
    uint32_t i, count;

    // calculate num of packets
    count = 2;
    if (draw_conf->index_type == DI_INDEX_SIZE_16_BIT)
	count += (draw_conf->num_indices + 1) / 2;
    else
	count += draw_conf->num_indices;

    BEGIN_BATCH(8 + count);
    EREG(VGT_PRIMITIVE_TYPE, draw_conf->prim_type);
    PACK3(IT_INDEX_TYPE, 1);
#if X_BYTE_ORDER == X_BIG_ENDIAN
    E32(IT_INDEX_TYPE_SWAP_MODE(ENDIAN_8IN32) | draw_conf->index_type);
#else
    E32(draw_conf->index_type);
#endif
    PACK3(IT_NUM_INSTANCES, 1);
    E32(draw_conf->num_instances);

    PACK3(IT_DRAW_INDEX_IMMD, count);
    E32(draw_conf->num_indices);
    E32(draw_conf->vgt_draw_initiator);

    if (draw_conf->index_type == DI_INDEX_SIZE_16_BIT) {
	for (i = 0; i < draw_conf->num_indices; i += 2) {
	    if ((i + 1) == draw_conf->num_indices)
		E32(indices[i]);
	    else
		E32((indices[i] | (indices[i + 1] << 16)));
	}
    } else {
	for (i = 0; i < draw_conf->num_indices; i++)
	    E32(indices[i]);
    }
    END_BATCH();
}

void
r600_draw_auto(ScrnInfoPtr pScrn, draw_config_t *draw_conf)
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

void r600_finish_op(ScrnInfoPtr pScrn, int vtx_size)
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
        R600IBDiscard(pScrn);
	return;
    }

    /* Vertex buffer setup */
    accel_state->vbo.vb_size = accel_state->vbo.vb_offset - accel_state->vbo.vb_start_op;
    vtx_res.id              = SQ_VTX_RESOURCE_vs;
    vtx_res.vtx_size_dw     = vtx_size / 4;
    vtx_res.vtx_num_entries = accel_state->vbo.vb_size / 4;
    vtx_res.mem_req_size    = 1;
    vtx_res.vb_addr         = accel_state->vbo.vb_start_op;
    vtx_res.bo              = accel_state->vbo.vb_bo;
#if X_BYTE_ORDER == X_BIG_ENDIAN
    vtx_res.endian          = SQ_ENDIAN_8IN32;
#endif
    r600_set_vtx_resource(pScrn, &vtx_res, RADEON_GEM_DOMAIN_GTT);

    /* Draw */
    draw_conf.prim_type          = DI_PT_RECTLIST;
    draw_conf.vgt_draw_initiator = DI_SRC_SEL_AUTO_INDEX;
    draw_conf.num_instances      = 1;
    draw_conf.num_indices        = vtx_res.vtx_num_entries / vtx_res.vtx_size_dw;
    draw_conf.index_type         = DI_INDEX_SIZE_16_BIT;

    r600_draw_auto(pScrn, &draw_conf);

    /* XXX drm should handle this in fence submit */
    r600_wait_3d_idle_clean(pScrn);

    /* sync dst surface */
    r600_cp_set_surface_sync(pScrn, (CB_ACTION_ENA_bit | CB0_DEST_BASE_ENA_bit),
			     accel_state->dst_size, 0,
			     accel_state->dst_obj.bo, 0, accel_state->dst_obj.domain);

    accel_state->vbo.vb_start_op = -1;
    accel_state->ib_reset_op = 0;

}

