/*
 * Copyright (c) 2011 Intel Corporation
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
 *    Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#include "brw_test.h"

#include <string.h>

static const uint32_t ps_kernel_nomask_affine[][4] = {
#include "exa_wm_src_affine.g6b"
#include "exa_wm_src_sample_argb.g6b"
#include "exa_wm_write.g6b"
};

static const uint32_t ps_kernel_nomask_projective[][4] = {
#include "exa_wm_src_projective.g6b"
#include "exa_wm_src_sample_argb.g6b"
#include "exa_wm_write.g6b"
};

static const uint32_t ps_kernel_maskca_affine[][4] = {
#include "exa_wm_src_affine.g6b"
#include "exa_wm_src_sample_argb.g6b"
#include "exa_wm_mask_affine.g6b"
#include "exa_wm_mask_sample_argb.g6b"
#include "exa_wm_ca.g6b"
#include "exa_wm_write.g6b"
};

static const uint32_t ps_kernel_maskca_projective[][4] = {
#include "exa_wm_src_projective.g6b"
#include "exa_wm_src_sample_argb.g6b"
#include "exa_wm_mask_projective.g6b"
#include "exa_wm_mask_sample_argb.g6b"
#include "exa_wm_ca.g6b"
#include "exa_wm_write.g6b"
};

static const uint32_t ps_kernel_maskca_srcalpha_affine[][4] = {
#include "exa_wm_src_affine.g6b"
#include "exa_wm_src_sample_a.g6b"
#include "exa_wm_mask_affine.g6b"
#include "exa_wm_mask_sample_argb.g6b"
#include "exa_wm_ca_srcalpha.g6b"
#include "exa_wm_write.g6b"
};

static const uint32_t ps_kernel_maskca_srcalpha_projective[][4] = {
#include "exa_wm_src_projective.g6b"
#include "exa_wm_src_sample_a.g6b"
#include "exa_wm_mask_projective.g6b"
#include "exa_wm_mask_sample_argb.g6b"
#include "exa_wm_ca_srcalpha.g6b"
#include "exa_wm_write.g6b"
};

static const uint32_t ps_kernel_masknoca_affine[][4] = {
#include "exa_wm_src_affine.g6b"
#include "exa_wm_src_sample_argb.g6b"
#include "exa_wm_mask_affine.g6b"
#include "exa_wm_mask_sample_a.g6b"
#include "exa_wm_noca.g6b"
#include "exa_wm_write.g6b"
};

static const uint32_t ps_kernel_masknoca_projective[][4] = {
#include "exa_wm_src_projective.g6b"
#include "exa_wm_src_sample_argb.g6b"
#include "exa_wm_mask_projective.g6b"
#include "exa_wm_mask_sample_a.g6b"
#include "exa_wm_noca.g6b"
#include "exa_wm_write.g6b"
};

static const uint32_t ps_kernel_packed[][4] = {
#include "exa_wm_src_affine.g6b"
#include "exa_wm_src_sample_argb.g6b"
#include "exa_wm_yuv_rgb.g6b"
#include "exa_wm_write.g6b"
};

static const uint32_t ps_kernel_planar[][4] = {
#include "exa_wm_src_affine.g6b"
#include "exa_wm_src_sample_planar.g6b"
#include "exa_wm_yuv_rgb.g6b"
#include "exa_wm_write.g6b"
};

#define compare(old) brw_test_compare(__FUNCTION__, p.gen, p.store, p.nr_insn, (struct brw_instruction *)old, ARRAY_SIZE(old))

#if 0
static void wm_src_affine(struct brw_compile *p)
{
	brw_PLN(p, brw_message_reg(2), brw_vec1_grf(6,0), brw_vec8_grf(2,0));
	brw_PLN(p, brw_message_reg(3), brw_vec1_grf(6,0), brw_vec8_grf(4,0));
	brw_PLN(p, brw_message_reg(4), brw_vec1_grf(6,4), brw_vec8_grf(2,0));
	brw_PLN(p, brw_message_reg(5), brw_vec1_grf(6,4), brw_vec8_grf(4,0));
}

static void wm_src_sample_argb(struct brw_compile *p)
{
	static const uint32_t fragment[][4] = {
#include "exa_wm_src_affine.g6b"
#include "exa_wm_src_sample_argb.g6b"
#include "exa_wm_write.g6b"
	};
	int n;

	brw_push_insn_state(p);
	brw_set_mask_control(p, BRW_MASK_DISABLE);
	brw_set_compression_control(p, BRW_COMPRESSION_NONE);
	brw_MOV(p,
		retype(brw_vec1_grf(0,2), BRW_REGISTER_TYPE_UD),
		brw_imm_ud(0));
	brw_pop_insn_state(p);

	brw_SAMPLE(p,
		   retype(vec16(brw_vec8_grf(14, 0)), BRW_REGISTER_TYPE_UW),
		   1,
		   retype(brw_vec8_grf(0, 0), BRW_REGISTER_TYPE_UD),
		   1, 0,
		   WRITEMASK_XYZW,
		   GEN5_SAMPLER_MESSAGE_SAMPLE,
		   8,
		   5,
		   true,
		   BRW_SAMPLER_SIMD_MODE_SIMD16);


	for (n = 0; n < p->nr_insn; n++) {
		brw_disasm(stdout, &p->store[n], 60);
	}

	printf("\n\n");
	for (n = 0; n < ARRAY_SIZE(fragment); n++) {
		brw_disasm(stdout,
			   (const struct brw_instruction *)&fragment[n][0],
			   60);
	}
}

static void wm_write(struct brw_compile *p)
{
}
#endif

static void gen6_ps_nomask_affine(void)
{
	uint32_t store[1024];
	struct brw_compile p;

	brw_compile_init(&p, 60, store);
	brw_wm_kernel__affine(&p, 16);

	compare(ps_kernel_nomask_affine);
}

static void gen6_ps_mask_affine(void)
{
	uint32_t store[1024];
	struct brw_compile p;

	brw_compile_init(&p, 60, store);
	brw_wm_kernel__affine_mask(&p, 16);

	compare(ps_kernel_masknoca_affine);
}

static void gen6_ps_nomask_projective(void)
{
	uint32_t store[1024];
	struct brw_compile p;

	brw_compile_init(&p, 60, store);
	brw_wm_kernel__projective(&p, 16);

	compare(ps_kernel_nomask_projective);
}

void brw_test_gen6(void)
{
	gen6_ps_nomask_affine();
	gen6_ps_mask_affine();

	gen6_ps_nomask_projective();
}
