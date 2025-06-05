/*
 * Copyright 2024 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL VMWARE AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


/* DO NOT EDIT - This file generated automatically by intel_device_serialize_c.py script */

#include "dev/intel_device_info.h"
#include "brw_compiler.h"
#define SHA_UPDATE_FIELD(field)     _mesa_sha1_update(ctx, &devinfo->field, sizeof(devinfo->field))

void
brw_device_sha1_update(struct mesa_sha1 *ctx,
                       const struct intel_device_info *devinfo) {
   SHA_UPDATE_FIELD(ver);
   SHA_UPDATE_FIELD(verx10);
   SHA_UPDATE_FIELD(gfx_ip_ver);
   SHA_UPDATE_FIELD(platform);
   SHA_UPDATE_FIELD(has_pln);
   SHA_UPDATE_FIELD(has_64bit_float);
   SHA_UPDATE_FIELD(has_64bit_float_via_math_pipe);
   SHA_UPDATE_FIELD(has_64bit_int);
   SHA_UPDATE_FIELD(has_integer_dword_mul);
   SHA_UPDATE_FIELD(has_compr4);
   SHA_UPDATE_FIELD(supports_simd16_3src);
   SHA_UPDATE_FIELD(has_ray_tracing);
   SHA_UPDATE_FIELD(has_lsc);
   SHA_UPDATE_FIELD(has_negative_rhw_bug);
   SHA_UPDATE_FIELD(has_coarse_pixel_primitive_and_cb);
   SHA_UPDATE_FIELD(needs_unlit_centroid_workaround);
   SHA_UPDATE_FIELD(max_slices);
   SHA_UPDATE_FIELD(max_subslices_per_slice);
   SHA_UPDATE_FIELD(max_eus_per_subslice);
   SHA_UPDATE_FIELD(num_thread_per_eu);
   if (devinfo->has_ray_tracing)
      SHA_UPDATE_FIELD(subslice_masks);
   SHA_UPDATE_FIELD(subslice_slice_stride);
   SHA_UPDATE_FIELD(max_cs_workgroup_threads);
   SHA_UPDATE_FIELD(max_scratch_ids);
   SHA_UPDATE_FIELD(max_scratch_size_per_thread);
}

#undef SHA_UPDATE_FIELD

