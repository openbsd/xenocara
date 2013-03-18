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

static const uint32_t sf_kernel[][4] = {
#include "exa_sf.g5b"
};

static const uint32_t sf_kernel_mask[][4] = {
#include "exa_sf_mask.g5b"
};

static const uint32_t ps_kernel_nomask_affine[][4] = {
#include "exa_wm_xy.g5b"
#include "exa_wm_src_affine.g5b"
#include "exa_wm_src_sample_argb.g5b"
#include "exa_wm_write.g5b"
};

static const uint32_t ps_kernel_nomask_projective[][4] = {
#include "exa_wm_xy.g5b"
#include "exa_wm_src_projective.g5b"
#include "exa_wm_src_sample_argb.g5b"
#include "exa_wm_write.g5b"
};

static const uint32_t ps_kernel_maskca_affine[][4] = {
#include "exa_wm_xy.g5b"
#include "exa_wm_src_affine.g5b"
#include "exa_wm_src_sample_argb.g5b"
#include "exa_wm_mask_affine.g5b"
#include "exa_wm_mask_sample_argb.g5b"
#include "exa_wm_ca.g5b"
#include "exa_wm_write.g5b"
};

static const uint32_t ps_kernel_maskca_projective[][4] = {
#include "exa_wm_xy.g5b"
#include "exa_wm_src_projective.g5b"
#include "exa_wm_src_sample_argb.g5b"
#include "exa_wm_mask_projective.g5b"
#include "exa_wm_mask_sample_argb.g5b"
#include "exa_wm_ca.g5b"
#include "exa_wm_write.g5b"
};

static const uint32_t ps_kernel_maskca_srcalpha_affine[][4] = {
#include "exa_wm_xy.g5b"
#include "exa_wm_src_affine.g5b"
#include "exa_wm_src_sample_a.g5b"
#include "exa_wm_mask_affine.g5b"
#include "exa_wm_mask_sample_argb.g5b"
#include "exa_wm_ca_srcalpha.g5b"
#include "exa_wm_write.g5b"
};

static const uint32_t ps_kernel_maskca_srcalpha_projective[][4] = {
#include "exa_wm_xy.g5b"
#include "exa_wm_src_projective.g5b"
#include "exa_wm_src_sample_a.g5b"
#include "exa_wm_mask_projective.g5b"
#include "exa_wm_mask_sample_argb.g5b"
#include "exa_wm_ca_srcalpha.g5b"
#include "exa_wm_write.g5b"
};

static const uint32_t ps_kernel_masknoca_affine[][4] = {
#include "exa_wm_xy.g5b"
#include "exa_wm_src_affine.g5b"
#include "exa_wm_src_sample_argb.g5b"
#include "exa_wm_mask_affine.g5b"
#include "exa_wm_mask_sample_a.g5b"
#include "exa_wm_noca.g5b"
#include "exa_wm_write.g5b"
};

static const uint32_t ps_kernel_masknoca_projective[][4] = {
#include "exa_wm_xy.g5b"
#include "exa_wm_src_projective.g5b"
#include "exa_wm_src_sample_argb.g5b"
#include "exa_wm_mask_projective.g5b"
#include "exa_wm_mask_sample_a.g5b"
#include "exa_wm_noca.g5b"
#include "exa_wm_write.g5b"
};

static const uint32_t ps_kernel_packed_static[][4] = {
#include "exa_wm_xy.g5b"
#include "exa_wm_src_affine.g5b"
#include "exa_wm_src_sample_argb.g5b"
#include "exa_wm_yuv_rgb.g5b"
#include "exa_wm_write.g5b"
};

static const uint32_t ps_kernel_planar_static[][4] = {
#include "exa_wm_xy.g5b"
#include "exa_wm_src_affine.g5b"
#include "exa_wm_src_sample_planar.g5b"
#include "exa_wm_yuv_rgb.g5b"
#include "exa_wm_write.g5b"
};

#define compare(old) brw_test_compare(__FUNCTION__, p.gen, p.store, p.nr_insn, (struct brw_instruction *)old, ARRAY_SIZE(old))

static void gen5_sf(void)
{
	uint32_t store[128];
	struct brw_compile p;

	brw_compile_init(&p, 50, store);
	brw_sf_kernel__nomask(&p);

	compare(sf_kernel);
}

static void gen5_sf_mask(void)
{
	uint32_t store[128];
	struct brw_compile p;

	brw_compile_init(&p, 50, store);
	brw_sf_kernel__mask(&p);

	compare(sf_kernel_mask);
}

static void gen5_wm_affine_nomask(void)
{
	uint32_t store[128];
	struct brw_compile p;

	brw_compile_init(&p, 50, store);
	brw_wm_kernel__affine(&p, 16);

	compare(ps_kernel_nomask_affine);
}

static void gen5_wm_affine_mask_noca(void)
{
	uint32_t store[128];
	struct brw_compile p;

	brw_compile_init(&p, 50, store);
	brw_wm_kernel__affine_mask(&p, 16);

	compare(ps_kernel_masknoca_affine);
}

static void gen5_wm_affine_mask_ca(void)
{
	uint32_t store[128];
	struct brw_compile p;

	brw_compile_init(&p, 50, store);
	brw_wm_kernel__affine_mask_ca(&p, 16);

	compare(ps_kernel_maskca_affine);
}

static void gen5_wm_projective_nomask(void)
{
	uint32_t store[128];
	struct brw_compile p;

	brw_compile_init(&p, 50, store);
	brw_wm_kernel__projective(&p, 16);

	compare(ps_kernel_nomask_projective);
}

void brw_test_gen5(void)
{
	gen5_sf();
	gen5_sf_mask();

	gen5_wm_affine_nomask();
	gen5_wm_affine_mask_noca();
	gen5_wm_affine_mask_ca();

	gen5_wm_projective_nomask();
}
