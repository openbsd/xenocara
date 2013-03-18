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
#include "exa_wm_src_affine.g7b"
#include "exa_wm_src_sample_argb.g7b"
#include "exa_wm_write.g7b"
};

static const uint32_t ps_kernel_nomask_projective[][4] = {
#include "exa_wm_src_projective.g7b"
#include "exa_wm_src_sample_argb.g7b"
#include "exa_wm_write.g7b"
};

static const uint32_t ps_kernel_maskca_affine[][4] = {
#include "exa_wm_src_affine.g7b"
#include "exa_wm_src_sample_argb.g7b"
#include "exa_wm_mask_affine.g7b"
#include "exa_wm_mask_sample_argb.g7b"
#include "exa_wm_ca.g6b"
#include "exa_wm_write.g7b"
};

static const uint32_t ps_kernel_maskca_projective[][4] = {
#include "exa_wm_src_projective.g7b"
#include "exa_wm_src_sample_argb.g7b"
#include "exa_wm_mask_projective.g7b"
#include "exa_wm_mask_sample_argb.g7b"
#include "exa_wm_ca.g6b"
#include "exa_wm_write.g7b"
};

static const uint32_t ps_kernel_maskca_srcalpha_affine[][4] = {
#include "exa_wm_src_affine.g7b"
#include "exa_wm_src_sample_a.g7b"
#include "exa_wm_mask_affine.g7b"
#include "exa_wm_mask_sample_argb.g7b"
#include "exa_wm_ca_srcalpha.g6b"
#include "exa_wm_write.g7b"
};

static const uint32_t ps_kernel_maskca_srcalpha_projective[][4] = {
#include "exa_wm_src_projective.g7b"
#include "exa_wm_src_sample_a.g7b"
#include "exa_wm_mask_projective.g7b"
#include "exa_wm_mask_sample_argb.g7b"
#include "exa_wm_ca_srcalpha.g6b"
#include "exa_wm_write.g7b"
};

static const uint32_t ps_kernel_masknoca_affine[][4] = {
#include "exa_wm_src_affine.g7b"
#include "exa_wm_src_sample_argb.g7b"
#include "exa_wm_mask_affine.g7b"
#include "exa_wm_mask_sample_a.g7b"
#include "exa_wm_noca.g6b"
#include "exa_wm_write.g7b"
};

static const uint32_t ps_kernel_masknoca_projective[][4] = {
#include "exa_wm_src_projective.g7b"
#include "exa_wm_src_sample_argb.g7b"
#include "exa_wm_mask_projective.g7b"
#include "exa_wm_mask_sample_a.g7b"
#include "exa_wm_noca.g6b"
#include "exa_wm_write.g7b"
};

static const uint32_t ps_kernel_packed[][4] = {
#include "exa_wm_src_affine.g7b"
#include "exa_wm_src_sample_argb.g7b"
#include "exa_wm_yuv_rgb.g7b"
#include "exa_wm_write.g7b"
};

static const uint32_t ps_kernel_planar[][4] = {
#include "exa_wm_src_affine.g7b"
#include "exa_wm_src_sample_planar.g7b"
#include "exa_wm_yuv_rgb.g7b"
#include "exa_wm_write.g7b"
};

#define compare(old) brw_test_compare(__FUNCTION__, p.gen, p.store, p.nr_insn, (struct brw_instruction *)old, ARRAY_SIZE(old))
#define GEN 70

static void gen7_ps_nomask_affine(void)
{
	uint32_t store[1024];
	struct brw_compile p;

	brw_compile_init(&p, GEN, store);
	brw_wm_kernel__affine(&p, 8);

	compare(ps_kernel_nomask_affine);
}

static void gen7_ps_mask_affine(void)
{
	uint32_t store[1024];
	struct brw_compile p;

	brw_compile_init(&p, GEN, store);
	brw_wm_kernel__affine_mask(&p, 8);

	compare(ps_kernel_masknoca_affine);
}

static void gen7_ps_maskca_affine(void)
{
	uint32_t store[1024];
	struct brw_compile p;

	brw_compile_init(&p, GEN, store);
	brw_wm_kernel__affine_mask_ca(&p, 8);

	compare(ps_kernel_maskca_affine);
}

static void gen7_ps_masksa_affine(void)
{
	uint32_t store[1024];
	struct brw_compile p;

	brw_compile_init(&p, GEN, store);
	brw_wm_kernel__affine_mask_sa(&p, 8);

	compare(ps_kernel_maskca_srcalpha_affine);
}

static void gen7_ps_nomask_projective(void)
{
	uint32_t store[1024];
	struct brw_compile p;

	brw_compile_init(&p, GEN, store);
	brw_wm_kernel__projective(&p, 8);

	compare(ps_kernel_nomask_projective);
}

static void gen7_ps_opacity(void)
{
	uint32_t store[1024];
	struct brw_compile p;

	brw_compile_init(&p, GEN, store);
	brw_wm_kernel__affine_opacity(&p, 16);

	compare(ps_kernel_nomask_affine);
}

void brw_test_gen7(void)
{
	gen7_ps_nomask_affine();
	gen7_ps_mask_affine();
	gen7_ps_maskca_affine();
	gen7_ps_masksa_affine();

	gen7_ps_nomask_projective();

	gen7_ps_opacity();
}
