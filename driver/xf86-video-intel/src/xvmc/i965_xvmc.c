/*
 * Copyright © 2008 Intel Corporation
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
 * Author:
 *    Zou Nan hai <nanhai.zou@intel.com>
 *
 */
#include "intel_xvmc.h"
#include "i830_reg.h"
#include "i965_reg.h"
#include "brw_defines.h"
#include "brw_structs.h"
#include "intel_batchbuffer.h"
#include "intel_hwmc.h"
#define BATCH_STRUCT(x) intelBatchbufferData(&x, sizeof(x), 0)
#define URB_SIZE     256	/* XXX */

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

enum interface {
	INTRA_INTERFACE = 0,	/* non field intra */
	NULL_INTERFACE,		/* fill with white, do nothing, for debug */
	FORWARD_INTERFACE,	/* non field forward predict */
	BACKWARD_INTERFACE,	/* non field backward predict */
	F_B_INTERFACE,		/* non field forward and backward predict */
	FIELD_FORWARD_INTERFACE,	/* field forward predict */
	FIELD_BACKWARD_INTERFACE,	/* field backward predict */
	FIELD_F_B_INTERFACE,	/* field forward and backward predict */
	DUAL_PRIME_INTERFACE
};

static const uint32_t ipicture_kernel_static[][4] = {
#include "shader/mc/ipicture.g4b"
};

static const uint32_t null_kernel_static[][4] = {
#include "shader/mc/null.g4b"
};

static const uint32_t frame_forward_kernel_static[][4] = {
#include "shader/mc/frame_forward.g4b"
};

static const uint32_t frame_backward_kernel_static[][4] = {
#include "shader/mc/frame_backward.g4b"
};

static const uint32_t frame_f_b_kernel_static[][4] = {
#include "shader/mc/frame_f_b.g4b"
};

static const uint32_t field_forward_kernel_static[][4] = {
#include "shader/mc/field_forward.g4b"
};

static const uint32_t field_backward_kernel_static[][4] = {
#include "shader/mc/field_backward.g4b"
};

static const uint32_t field_f_b_kernel_static[][4] = {
#include "shader/mc/field_f_b.g4b"
};

static const uint32_t dual_prime_kernel_static[][4] = {
#include "shader/mc/dual_prime.g4b"
};

static const uint32_t frame_forward_igd_kernel_static[][4] = {
#include "shader/mc/frame_forward_igd.g4b"
};

static const uint32_t frame_backward_igd_kernel_static[][4] = {
#include "shader/mc/frame_backward_igd.g4b"
};

static const uint32_t frame_f_b_igd_kernel_static[][4] = {
#include "shader/mc/frame_f_b_igd.g4b"
};

static const uint32_t field_forward_igd_kernel_static[][4] = {
#include "shader/mc/field_forward_igd.g4b"
};

static const uint32_t field_backward_igd_kernel_static[][4] = {
#include "shader/mc/field_backward_igd.g4b"
};

static const uint32_t field_f_b_igd_kernel_static[][4] = {
#include "shader/mc/field_f_b_igd.g4b"
};

static const uint32_t dual_prime_igd_kernel_static[][4] = {
#include "shader/mc/dual_prime_igd.g4b"
};

struct kernel_struct {
	const uint32_t(*bin)[4];
	uint32_t size;
};

struct kernel_struct kernels_igd[] = {
	{ipicture_kernel_static, sizeof(ipicture_kernel_static)}
	,
	{null_kernel_static, sizeof(null_kernel_static)}
	,
	{frame_forward_igd_kernel_static,
	 sizeof(frame_forward_igd_kernel_static)}
	,
	{frame_backward_igd_kernel_static,
	 sizeof(frame_backward_igd_kernel_static)}
	,
	{frame_f_b_igd_kernel_static, sizeof(frame_f_b_igd_kernel_static)}
	,
	{field_forward_igd_kernel_static,
	 sizeof(field_forward_igd_kernel_static)}
	,
	{field_backward_igd_kernel_static,
	 sizeof(field_backward_igd_kernel_static)}
	,
	{field_f_b_igd_kernel_static, sizeof(field_f_b_igd_kernel_static)}
	,
	{dual_prime_igd_kernel_static, sizeof(dual_prime_igd_kernel_static)}
};

struct kernel_struct kernels_965[] = {
	{ipicture_kernel_static, sizeof(ipicture_kernel_static)}
	,
	{null_kernel_static, sizeof(null_kernel_static)}
	,
	{frame_forward_kernel_static, sizeof(frame_forward_kernel_static)}
	,
	{frame_backward_kernel_static, sizeof(frame_backward_kernel_static)}
	,
	{frame_f_b_kernel_static, sizeof(frame_f_b_kernel_static)}
	,
	{field_forward_kernel_static, sizeof(field_forward_kernel_static)}
	,
	{field_backward_kernel_static, sizeof(field_backward_kernel_static)}
	,
	{field_f_b_kernel_static, sizeof(field_f_b_kernel_static)}
	,
	{dual_prime_kernel_static, sizeof(dual_prime_kernel_static)}
};

#define ALIGN(i,m)    (((i) + (m) - 1) & ~((m) - 1))

#define MAX_SURFACE_NUM	10
#define DESCRIPTOR_NUM 12

struct media_kernel_obj {
	dri_bo *bo;
};

struct interface_descriptor_obj {
	dri_bo *bo;
	struct media_kernel_obj kernels[DESCRIPTOR_NUM];
};

struct vfe_state_obj {
	dri_bo *bo;
	struct interface_descriptor_obj interface;
};

struct surface_obj {
	dri_bo *bo;
};

struct surface_state_obj {
	struct surface_obj surface;
	dri_bo *bo;
};

struct binding_table_obj {
	dri_bo *bo;
	struct surface_state_obj surface_states[MAX_SURFACE_NUM];
};

struct indirect_data_obj {
	dri_bo *bo;
};

struct media_state {
	unsigned int is_g4x:1;
	unsigned int is_965_q:1;

	struct vfe_state_obj vfe_state;
	struct binding_table_obj binding_table;
	struct indirect_data_obj indirect_data;
};
struct media_state media_state;

static void free_object(struct media_state *s)
{
	int i;
#define FREE_ONE_BO(bo) drm_intel_bo_unreference(bo)
	FREE_ONE_BO(s->vfe_state.bo);
	FREE_ONE_BO(s->vfe_state.interface.bo);
	for (i = 0; i < DESCRIPTOR_NUM; i++)
		FREE_ONE_BO(s->vfe_state.interface.kernels[i].bo);
	FREE_ONE_BO(s->binding_table.bo);
	for (i = 0; i < MAX_SURFACE_NUM; i++)
		FREE_ONE_BO(s->binding_table.surface_states[i].bo);
	FREE_ONE_BO(s->indirect_data.bo);
}

static int alloc_object(struct media_state *s)
{
	int i;

	for (i = 0; i < MAX_SURFACE_NUM; i++) {
		s->binding_table.surface_states[i].bo =
		    drm_intel_bo_alloc(xvmc_driver->bufmgr, "surface_state",
				       sizeof(struct brw_surface_state),
				       0x1000);
		if (!s->binding_table.surface_states[i].bo)
			goto out;
	}
	return 0;
out:
	free_object(s);
	return BadAlloc;
}

static Status destroy_context(Display * display, XvMCContext * context)
{
	struct intel_xvmc_context *intel_ctx;
	intel_ctx = context->privData;
	Xfree(intel_ctx->hw);
	free(intel_ctx);
	return Success;
}

#define STRIDE(w)               (w)
#define SIZE_YUV420(w, h)       (h * (STRIDE(w) + STRIDE(w >> 1)))

static void flush()
{
	struct brw_mi_flush flush;
	memset(&flush, 0, sizeof(flush));
	flush.opcode = CMD_MI_FLUSH;
	flush.flags = (1 << 1);
	BATCH_STRUCT(flush);
}

static void clear_sf_state()
{
	struct brw_sf_unit_state sf;
	memset(&sf, 0, sizeof(sf));
	/* TODO */
}

/* urb fence must be aligned to cacheline */
static void align_urb_fence()
{
	BATCH_LOCALS;
	int i, offset_to_next_cacheline;
	unsigned long batch_offset;
	BEGIN_BATCH(3);
	batch_offset = (void *)batch_ptr - xvmc_driver->alloc.ptr;
	offset_to_next_cacheline = ALIGN(batch_offset, 64) - batch_offset;
	if (offset_to_next_cacheline <= 12 && offset_to_next_cacheline != 0) {
		for (i = 0; i < offset_to_next_cacheline / 4; i++)
			OUT_BATCH(0);
		ADVANCE_BATCH();
	}
}

/* setup urb layout for media */
static void urb_layout()
{
	BATCH_LOCALS;
	align_urb_fence();
	BEGIN_BATCH(3);
	OUT_BATCH(BRW_URB_FENCE |
		  UF0_VFE_REALLOC |
		  UF0_CS_REALLOC |
		  UF0_SF_REALLOC |
		  UF0_CLIP_REALLOC | UF0_GS_REALLOC | UF0_VS_REALLOC | 1);
	OUT_BATCH((0 << UF1_CLIP_FENCE_SHIFT) |
		  (0 << UF1_GS_FENCE_SHIFT) | (0 << UF1_VS_FENCE_SHIFT));

	OUT_BATCH(((URB_SIZE) << UF2_VFE_FENCE_SHIFT) |	/* VFE_SIZE */
		  ((URB_SIZE) << UF2_CS_FENCE_SHIFT));	/* CS_SIZE is 0 */
	ADVANCE_BATCH();
}

static void media_state_pointers(struct media_state *media_state)
{
	BATCH_LOCALS;
	BEGIN_BATCH(3);
	OUT_BATCH(BRW_MEDIA_STATE_POINTERS | 1);
	OUT_BATCH(0);
	OUT_RELOC(media_state->vfe_state.bo, I915_GEM_DOMAIN_INSTRUCTION, 0, 0);
	ADVANCE_BATCH();
}

/* setup 2D surface for media_read or media_write 
 */
static Status setup_media_surface(struct media_state *media_state,
				  int surface_num, dri_bo * bo,
				  unsigned long offset, int w, int h,
				  Bool write)
{
	struct brw_surface_state s, *ss = &s;

	memset(ss, 0, sizeof(struct brw_surface_state));
	ss->ss0.surface_type = BRW_SURFACE_2D;
	ss->ss0.surface_format = BRW_SURFACEFORMAT_R8_SINT;
	ss->ss1.base_addr = offset + bo->offset;
	ss->ss2.width = w - 1;
	ss->ss2.height = h - 1;
	ss->ss3.pitch = w - 1;

	if (media_state->binding_table.surface_states[surface_num].bo)
		drm_intel_bo_unreference(media_state->
					 binding_table.surface_states
					 [surface_num].bo);
	media_state->binding_table.surface_states[surface_num].bo =
	    drm_intel_bo_alloc(xvmc_driver->bufmgr, "surface_state",
			       sizeof(struct brw_surface_state), 0x1000);
	if (!media_state->binding_table.surface_states[surface_num].bo)
		return BadAlloc;

	drm_intel_bo_subdata(media_state->
			     binding_table.surface_states[surface_num].bo, 0,
			     sizeof(*ss), ss);

	drm_intel_bo_emit_reloc(media_state->
				binding_table.surface_states[surface_num].bo,
				offsetof(struct brw_surface_state, ss1), bo,
				offset, I915_GEM_DOMAIN_RENDER,
				write ? I915_GEM_DOMAIN_RENDER : 0);

	return Success;
}

static Status setup_surfaces(struct media_state *media_state,
			     dri_bo * dst_bo, dri_bo * past_bo,
			     dri_bo * future_bo, int w, int h)
{
	Status ret;
	ret = setup_media_surface(media_state, 0, dst_bo, 0, w, h, TRUE);
	if (ret != Success)
		return ret;
	ret =
	    setup_media_surface(media_state, 1, dst_bo, w * h, w / 2, h / 2,
				TRUE);
	if (ret != Success)
		return ret;
	ret =
	    setup_media_surface(media_state, 2, dst_bo, w * h + w * h / 4,
				w / 2, h / 2, TRUE);
	if (ret != Success)
		return ret;
	if (past_bo) {
		ret =
		    setup_media_surface(media_state, 4, past_bo, 0, w, h,
					FALSE);
		if (ret != Success)
			return ret;
		ret =
		    setup_media_surface(media_state, 5, past_bo, w * h, w / 2,
					h / 2, FALSE);
		if (ret != Success)
			return ret;
		ret =
		    setup_media_surface(media_state, 6, past_bo,
					w * h + w * h / 4, w / 2, h / 2, FALSE);
		if (ret != Success)
			return ret;
	}
	if (future_bo) {
		ret =
		    setup_media_surface(media_state, 7, future_bo, 0, w, h,
					FALSE);
		if (ret != Success)
			return ret;
		ret =
		    setup_media_surface(media_state, 8, future_bo, w * h, w / 2,
					h / 2, FALSE);
		if (ret != Success)
			return ret;
		ret =
		    setup_media_surface(media_state, 9, future_bo,
					w * h + w * h / 4, w / 2, h / 2, FALSE);
		if (ret != Success)
			return ret;
	}
	return Success;
}

/* BUFFER SURFACE has a strange format
 * the size of the surface is in part of w h and d component
 */

static Status setup_blocks(struct media_state *media_state,
			   unsigned int block_size)
{
	union element {
		struct {
			unsigned int w:7;
			unsigned int h:13;
			unsigned int d:7;
			unsigned int pad:7;
		} whd;
		unsigned int size;
	} e;
	struct brw_surface_state ss;
	memset(&ss, 0, sizeof(struct brw_surface_state));
	ss.ss0.surface_type = BRW_SURFACE_BUFFER;
	ss.ss0.surface_format = BRW_SURFACEFORMAT_R8_UINT;
	ss.ss1.base_addr = media_state->indirect_data.bo->offset;

	e.size = block_size - 1;
	ss.ss2.width = e.whd.w;
	ss.ss2.height = e.whd.h;
	ss.ss3.depth = e.whd.d;
	ss.ss3.pitch = block_size - 1;

	if (media_state->binding_table.surface_states[3].bo)
		drm_intel_bo_unreference(media_state->
					 binding_table.surface_states[3].bo);

	media_state->binding_table.surface_states[3].bo =
	    drm_intel_bo_alloc(xvmc_driver->bufmgr, "surface_state",
			       sizeof(struct brw_surface_state), 0x1000);
	if (!media_state->binding_table.surface_states[3].bo)
		return BadAlloc;

	drm_intel_bo_subdata(media_state->binding_table.surface_states[3].bo, 0,
			     sizeof(ss), &ss);

	drm_intel_bo_emit_reloc(media_state->binding_table.surface_states[3].bo,
				offsetof(struct brw_surface_state, ss1),
				media_state->indirect_data.bo, 0,
				I915_GEM_DOMAIN_SAMPLER, 0);
	return Success;
}

/* setup state base address */
static void state_base_address()
{
	BATCH_LOCALS;
	BEGIN_BATCH(6);
	OUT_BATCH(BRW_STATE_BASE_ADDRESS | 4);
	OUT_BATCH(0 | BASE_ADDRESS_MODIFY);
	OUT_BATCH(0 | BASE_ADDRESS_MODIFY);
	OUT_BATCH(0 | BASE_ADDRESS_MODIFY);
	OUT_BATCH(0 | BASE_ADDRESS_MODIFY);
	OUT_BATCH(0xFFFFF000 | BASE_ADDRESS_MODIFY);
	ADVANCE_BATCH();
}

/* select media pipeline */
static void pipeline_select(struct media_state *media_state)
{
	BATCH_LOCALS;
	BEGIN_BATCH(1);
	if (media_state->is_g4x)
		OUT_BATCH(NEW_PIPELINE_SELECT | PIPELINE_SELECT_MEDIA);
	else
		OUT_BATCH(BRW_PIPELINE_SELECT | PIPELINE_SELECT_MEDIA);
	ADVANCE_BATCH();
}

/* kick media object to gpu */
static void send_media_object(XvMCMacroBlock * mb, int offset,
			      enum interface interface)
{
	BATCH_LOCALS;
	BEGIN_BATCH(13);
	OUT_BATCH(BRW_MEDIA_OBJECT | 11);
	OUT_BATCH(interface);
	if (media_state.is_965_q) {
		OUT_BATCH(0);
		OUT_BATCH(0);
	} else {
		OUT_BATCH(6 * 128);
		OUT_RELOC(media_state.indirect_data.bo,
			  I915_GEM_DOMAIN_INSTRUCTION, 0, offset);
	}

	OUT_BATCH(mb->x << 4);	//g1.0
	OUT_BATCH(mb->y << 4);
	OUT_RELOC(media_state.indirect_data.bo,	//g1.8
		  I915_GEM_DOMAIN_INSTRUCTION, 0, offset);
	OUT_BATCH_SHORT(mb->coded_block_pattern);	//g1.12
	OUT_BATCH_SHORT(mb->PMV[0][0][0]);	//g1.14
	OUT_BATCH_SHORT(mb->PMV[0][0][1]);	//g1.16
	OUT_BATCH_SHORT(mb->PMV[0][1][0]);	//g1.18
	OUT_BATCH_SHORT(mb->PMV[0][1][1]);	//g1.20

	OUT_BATCH_SHORT(mb->PMV[1][0][0]);	//g1.22
	OUT_BATCH_SHORT(mb->PMV[1][0][1]);	//g1.24
	OUT_BATCH_SHORT(mb->PMV[1][1][0]);	//g1.26
	OUT_BATCH_SHORT(mb->PMV[1][1][1]);	//g1.28
	OUT_BATCH_CHAR(mb->dct_type);	//g1.30
	OUT_BATCH_CHAR(mb->motion_vertical_field_select);	//g1.31

	if (media_state.is_965_q)
		OUT_BATCH(0x0);
	else
		OUT_BATCH(0xffffffff);
	ADVANCE_BATCH();
}

static Status binding_tables(struct media_state *media_state)
{
	unsigned int binding_table[MAX_SURFACE_NUM];
	int i;

	if (media_state->binding_table.bo)
		drm_intel_bo_unreference(media_state->binding_table.bo);
	media_state->binding_table.bo =
	    drm_intel_bo_alloc(xvmc_driver->bufmgr, "binding_table",
			       MAX_SURFACE_NUM * 4, 0x1000);
	if (!media_state->binding_table.bo)
		return BadAlloc;

	for (i = 0; i < MAX_SURFACE_NUM; i++)
		binding_table[i] =
		    media_state->binding_table.surface_states[i].bo->offset;
	drm_intel_bo_subdata(media_state->binding_table.bo, 0,
			     sizeof(binding_table), binding_table);

	for (i = 0; i < MAX_SURFACE_NUM; i++)
		drm_intel_bo_emit_reloc(media_state->binding_table.bo,
					i * sizeof(unsigned int),
					media_state->
					binding_table.surface_states[i].bo, 0,
					I915_GEM_DOMAIN_INSTRUCTION, 0);
	return Success;
}

static int media_kernels(struct media_state *media_state)
{
	struct kernel_struct *kernels;
	int kernel_array_size, i;

	if (media_state->is_g4x) {
		kernels = kernels_igd;
		kernel_array_size = ARRAY_SIZE(kernels_igd);
	} else {
		kernels = kernels_965;
		kernel_array_size = ARRAY_SIZE(kernels_965);
	}

	for (i = 0; i < kernel_array_size; i++) {
		media_state->vfe_state.interface.kernels[i].bo =
		    drm_intel_bo_alloc(xvmc_driver->bufmgr, "kernel",
				       kernels[i].size, 0x1000);
		if (!media_state->vfe_state.interface.kernels[i].bo)
			goto out;
	}

	for (i = 0; i < kernel_array_size; i++) {
		dri_bo *bo = media_state->vfe_state.interface.kernels[i].bo;
		drm_intel_bo_subdata(bo, 0, kernels[i].size, kernels[i].bin);
	}
	return 0;
out:
	free_object(media_state);
	return BadAlloc;
}

static void setup_interface(struct media_state *media_state, enum interface i)
{
	struct brw_interface_descriptor desc;
	memset(&desc, 0, sizeof(desc));

	desc.desc0.grf_reg_blocks = 15;
	desc.desc0.kernel_start_pointer =
	    media_state->vfe_state.interface.kernels[i].bo->offset >> 6;

	desc.desc1.floating_point_mode = BRW_FLOATING_POINT_NON_IEEE_754;

	/* use same binding table for all interface
	 * may change this if it affect performance
	 */
	desc.desc3.binding_table_entry_count = MAX_SURFACE_NUM;
	desc.desc3.binding_table_pointer =
	    media_state->binding_table.bo->offset >> 5;

	drm_intel_bo_subdata(media_state->vfe_state.interface.bo,
			     i * sizeof(desc), sizeof(desc), &desc);

	drm_intel_bo_emit_reloc(media_state->vfe_state.interface.bo,
				i * sizeof(desc) +
				offsetof(struct brw_interface_descriptor,
					 desc0),
				media_state->vfe_state.interface.kernels[i].bo,
				desc.desc0.grf_reg_blocks,
				I915_GEM_DOMAIN_INSTRUCTION, 0);

	drm_intel_bo_emit_reloc(media_state->vfe_state.interface.bo,
				i * sizeof(desc) +
				offsetof(struct brw_interface_descriptor,
					 desc3), media_state->binding_table.bo,
				desc.desc3.binding_table_entry_count,
				I915_GEM_DOMAIN_INSTRUCTION, 0);
}

static Status interface_descriptor(struct media_state *media_state)
{
	if (media_state->vfe_state.interface.bo)
		drm_intel_bo_unreference(media_state->vfe_state.interface.bo);
	media_state->vfe_state.interface.bo =
	    drm_intel_bo_alloc(xvmc_driver->bufmgr, "interfaces",
			       DESCRIPTOR_NUM *
			       sizeof(struct brw_interface_descriptor), 0x1000);
	if (!media_state->vfe_state.interface.bo)
		return BadAlloc;

	setup_interface(media_state, INTRA_INTERFACE);
	setup_interface(media_state, NULL_INTERFACE);
	setup_interface(media_state, FORWARD_INTERFACE);
	setup_interface(media_state, FIELD_FORWARD_INTERFACE);
	setup_interface(media_state, BACKWARD_INTERFACE);
	setup_interface(media_state, FIELD_BACKWARD_INTERFACE);
	setup_interface(media_state, F_B_INTERFACE);
	setup_interface(media_state, FIELD_F_B_INTERFACE);
	setup_interface(media_state, DUAL_PRIME_INTERFACE);
	return Success;
}

static Status vfe_state(struct media_state *media_state)
{
	struct brw_vfe_state state;
	memset(&state, 0, sizeof(state));

	/* no scratch space */
	state.vfe1.vfe_mode = VFE_GENERIC_MODE;
	state.vfe1.num_urb_entries = 1;
	/* XXX TODO */
	/* should carefully caculate those values for performance */
	state.vfe1.urb_entry_alloc_size = 2;
	state.vfe1.max_threads = 31;
	state.vfe2.interface_descriptor_base =
	    media_state->vfe_state.interface.bo->offset >> 4;

	if (media_state->vfe_state.bo)
		drm_intel_bo_unreference(media_state->vfe_state.bo);
	media_state->vfe_state.bo = drm_intel_bo_alloc(xvmc_driver->bufmgr,
						       "vfe state",
						       sizeof(struct
							      brw_vfe_state),
						       0x1000);
	if (!media_state->vfe_state.bo)
		return BadAlloc;

	drm_intel_bo_subdata(media_state->vfe_state.bo, 0, sizeof(state),
			     &state);

	drm_intel_bo_emit_reloc(media_state->vfe_state.bo,
				offsetof(struct brw_vfe_state, vfe2),
				media_state->vfe_state.interface.bo, 0,
				I915_GEM_DOMAIN_INSTRUCTION, 0);
	return Success;
}

static Status render_surface(Display * display,
			     XvMCContext * context,
			     unsigned int picture_structure,
			     XvMCSurface * target_surface,
			     XvMCSurface * past_surface,
			     XvMCSurface * future_surface,
			     unsigned int flags,
			     unsigned int num_macroblocks,
			     unsigned int first_macroblock,
			     XvMCMacroBlockArray * macroblock_array,
			     XvMCBlockArray * blocks)
{

	intel_xvmc_context_ptr intel_ctx;
	int i, j;
	struct i965_xvmc_context *i965_ctx;
	XvMCMacroBlock *mb;
	struct intel_xvmc_surface *priv_target_surface =
	    target_surface->privData;
	struct intel_xvmc_surface *priv_past_surface =
	    past_surface ? past_surface->privData : 0;
	struct intel_xvmc_surface *priv_future_surface =
	    future_surface ? future_surface->privData : 0;
	unsigned short *block_ptr;
	intel_ctx = context->privData;
	i965_ctx = context->privData;
	if (!intel_ctx) {
		XVMC_ERR("Can't find intel xvmc context\n");
		return BadValue;
	}

	if (media_state.indirect_data.bo) {
		drm_intel_gem_bo_unmap_gtt(media_state.
					   indirect_data.bo);

		drm_intel_bo_unreference(media_state.indirect_data.bo);
	}
	media_state.indirect_data.bo = drm_intel_bo_alloc(xvmc_driver->bufmgr,
							  "indirect data",
							  128 * 6 *
							  num_macroblocks, 64);
	if (!media_state.indirect_data.bo)
		return BadAlloc;
	setup_surfaces(&media_state,
		       priv_target_surface->bo,
		       past_surface ? priv_past_surface->bo : NULL,
		       future_surface ? priv_future_surface->bo : NULL,
		       context->width, context->height);
	setup_blocks(&media_state, 128 * 6 * num_macroblocks);
	binding_tables(&media_state);
	interface_descriptor(&media_state);
	vfe_state(&media_state);

	drm_intel_gem_bo_map_gtt(media_state.indirect_data.bo);

	block_ptr = media_state.indirect_data.bo->virtual;
	for (i = first_macroblock; i < num_macroblocks + first_macroblock; i++) {
		unsigned short *mb_block_ptr;
		mb = &macroblock_array->macro_blocks[i];
		mb_block_ptr = &blocks->blocks[(mb->index << 6)];
		if (mb->coded_block_pattern & 0x20) {
			for (j = 0; j < 8; j++)
				memcpy(block_ptr + 16 * j, mb_block_ptr + 8 * j,
				       16);
			mb_block_ptr += 64;
		}

		if (mb->coded_block_pattern & 0x10) {
			for (j = 0; j < 8; j++)
				memcpy(block_ptr + 16 * j + 8,
				       mb_block_ptr + 8 * j, 16);
			mb_block_ptr += 64;
		}
		block_ptr += 2 * 64;
		if (mb->coded_block_pattern & 0x08) {
			for (j = 0; j < 8; j++)
				memcpy(block_ptr + 16 * j, mb_block_ptr + 8 * j,
				       16);
			mb_block_ptr += 64;
		}

		if (mb->coded_block_pattern & 0x04) {
			for (j = 0; j < 8; j++)
				memcpy(block_ptr + 16 * j + 8,
				       mb_block_ptr + 8 * j, 16);
			mb_block_ptr += 64;
		}

		block_ptr += 2 * 64;
		if (mb->coded_block_pattern & 0x2) {
			memcpy(block_ptr, mb_block_ptr, 128);
			mb_block_ptr += 64;
		}

		block_ptr += 64;
		if (mb->coded_block_pattern & 0x1)
			memcpy(block_ptr, mb_block_ptr, 128);
		block_ptr += 64;
	}
	{
		int block_offset = 0;
		LOCK_HARDWARE(intel_ctx->hw_context);
		state_base_address();
		flush();
		clear_sf_state();
		pipeline_select(&media_state);
		urb_layout();
		media_state_pointers(&media_state);
		for (i = first_macroblock;
		     i < num_macroblocks + first_macroblock;
		     i++, block_offset += 128 * 6) {
			mb = &macroblock_array->macro_blocks[i];

			if (mb->macroblock_type & XVMC_MB_TYPE_INTRA) {
				send_media_object(mb, block_offset,
						  INTRA_INTERFACE);
			} else {
				if (((mb->motion_type & 3) ==
				     XVMC_PREDICTION_FRAME)) {
					if ((mb->macroblock_type &
					     XVMC_MB_TYPE_MOTION_FORWARD)) {
						if (((mb->macroblock_type &
						      XVMC_MB_TYPE_MOTION_BACKWARD)))
							send_media_object(mb,
									  block_offset,
									  F_B_INTERFACE);
						else
							send_media_object(mb,
									  block_offset,
									  FORWARD_INTERFACE);
					} else
					    if ((mb->macroblock_type &
						 XVMC_MB_TYPE_MOTION_BACKWARD))
					{
						send_media_object(mb,
								  block_offset,
								  BACKWARD_INTERFACE);
					}
				} else if ((mb->motion_type & 3) ==
					   XVMC_PREDICTION_FIELD) {
					if ((mb->macroblock_type &
					     XVMC_MB_TYPE_MOTION_FORWARD)) {
						if (((mb->macroblock_type &
						      XVMC_MB_TYPE_MOTION_BACKWARD)))
							send_media_object(mb,
									  block_offset,
									  FIELD_F_B_INTERFACE);
						else

							send_media_object(mb,
									  block_offset,
									  FIELD_FORWARD_INTERFACE);
					} else
					    if ((mb->macroblock_type &
						 XVMC_MB_TYPE_MOTION_BACKWARD))
					{
						send_media_object(mb,
								  block_offset,
								  FIELD_BACKWARD_INTERFACE);
					}
				} else {
					send_media_object(mb, block_offset,
							  DUAL_PRIME_INTERFACE);
				}
			}
		}
		intelFlushBatch(TRUE);
		UNLOCK_HARDWARE(intel_ctx->hw_context);
	}
	return Success;
}

static Status create_context(Display * display, XvMCContext * context,
			     int priv_count, CARD32 * priv_data)
{
	struct intel_xvmc_context *intel_ctx;
	struct intel_xvmc_hw_context *hw_ctx;
	hw_ctx = (struct intel_xvmc_hw_context *)priv_data;

	intel_ctx = calloc(1, sizeof(struct intel_xvmc_context));
	if (!intel_ctx)
		return BadAlloc;
	intel_ctx->hw = hw_ctx;
	intel_ctx->surface_bo_size
		= SIZE_YUV420(context->width, context->height);
	context->privData = intel_ctx;

	media_state.is_g4x = hw_ctx->i965.is_g4x;
	media_state.is_965_q = hw_ctx->i965.is_965_q;

	if (alloc_object(&media_state))
		return BadAlloc;
	if (media_kernels(&media_state))
		return BadAlloc;
	return Success;
}

struct _intel_xvmc_driver i965_xvmc_mc_driver = {
	.type = XVMC_I965_MPEG2_MC,
	.create_context = create_context,
	.destroy_context = destroy_context,
	.render_surface = render_surface,
};
