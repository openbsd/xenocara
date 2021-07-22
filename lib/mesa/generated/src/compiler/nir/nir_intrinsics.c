/* Copyright (C) 2018 Red Hat
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
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "nir.h"

const nir_intrinsic_info nir_intrinsic_infos[nir_num_intrinsics] = {
{
   .name = "accept_ray_intersection",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "addr_mode_is",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_MEMORY_MODES,
   },
   .index_map = {
      [NIR_INTRINSIC_MEMORY_MODES] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "atomic_counter_add",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "atomic_counter_add_deref",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "atomic_counter_and",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "atomic_counter_and_deref",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "atomic_counter_comp_swap",
   .num_srcs = 3,
   .src_components = {
      1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "atomic_counter_comp_swap_deref",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "atomic_counter_exchange",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "atomic_counter_exchange_deref",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "atomic_counter_inc",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "atomic_counter_inc_deref",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "atomic_counter_max",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "atomic_counter_max_deref",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "atomic_counter_min",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "atomic_counter_min_deref",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "atomic_counter_or",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "atomic_counter_or_deref",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "atomic_counter_post_dec",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "atomic_counter_post_dec_deref",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "atomic_counter_pre_dec",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "atomic_counter_pre_dec_deref",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "atomic_counter_read",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "atomic_counter_read_deref",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "atomic_counter_xor",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "atomic_counter_xor_deref",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "ballot",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "ballot_bit_count_exclusive",
   .num_srcs = 1,
   .src_components = {
      4
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "ballot_bit_count_inclusive",
   .num_srcs = 1,
   .src_components = {
      4
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "ballot_bit_count_reduce",
   .num_srcs = 1,
   .src_components = {
      4
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "ballot_bitfield_extract",
   .num_srcs = 2,
   .src_components = {
      4, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "ballot_find_lsb",
   .num_srcs = 1,
   .src_components = {
      4
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "ballot_find_msb",
   .num_srcs = 1,
   .src_components = {
      4
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "begin_invocation_interlock",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "bindless_image_atomic_add",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_and",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_comp_swap",
   .num_srcs = 5,
   .src_components = {
      1, 4, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_dec_wrap",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_exchange",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_fadd",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_fmax",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_fmin",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_imax",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_imin",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_inc_wrap",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_or",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_umax",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_umin",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_atomic_xor",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_image_format",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "bindless_image_load",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 5,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_DEST_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
      [NIR_INTRINSIC_DEST_TYPE] = 5,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "bindless_image_load_raw_intel",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "bindless_image_order",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "bindless_image_samples",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "bindless_image_size",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "bindless_image_sparse_load",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 5,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_DEST_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
      [NIR_INTRINSIC_DEST_TYPE] = 5,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "bindless_image_store",
   .num_srcs = 5,
   .src_components = {
      1, 4, 1, 0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 5,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_SRC_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
      [NIR_INTRINSIC_SRC_TYPE] = 5,
    },
   .flags = 0,
},
{
   .name = "bindless_image_store_raw_intel",
   .num_srcs = 3,
   .src_components = {
      1, 1, 0
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "bindless_resource_ir3",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_DESC_SET,
   },
   .index_map = {
      [NIR_INTRINSIC_DESC_SET] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "btd_resume_intel",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_RANGE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_RANGE] = 2,
    },
   .flags = 0,
},
{
   .name = "btd_retire_intel",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "btd_spawn_intel",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "btd_stack_push_intel",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_RANGE,
   },
   .index_map = {
      [NIR_INTRINSIC_RANGE] = 1,
    },
   .flags = 0,
},
{
   .name = "cond_end_ir3",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "control_barrier",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "convert_alu_types",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_SRC_TYPE,
      NIR_INTRINSIC_DEST_TYPE,
      NIR_INTRINSIC_ROUNDING_MODE,
      NIR_INTRINSIC_SATURATE,
   },
   .index_map = {
      [NIR_INTRINSIC_SRC_TYPE] = 1,
      [NIR_INTRINSIC_DEST_TYPE] = 2,
      [NIR_INTRINSIC_ROUNDING_MODE] = 3,
      [NIR_INTRINSIC_SATURATE] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "copy_deref",
   .num_srcs = 2,
   .src_components = {
      -1, -1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_DST_ACCESS,
      NIR_INTRINSIC_SRC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_DST_ACCESS] = 1,
      [NIR_INTRINSIC_SRC_ACCESS] = 2,
    },
   .flags = 0,
},
{
   .name = "demote",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "demote_if",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "deref_atomic_add",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_and",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_comp_swap",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_exchange",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_fadd",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_fcomp_swap",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_fmax",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_fmin",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_imax",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_imin",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_or",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_umax",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_umin",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_atomic_xor",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "deref_buffer_array_length",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "deref_mode_is",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_MEMORY_MODES,
   },
   .index_map = {
      [NIR_INTRINSIC_MEMORY_MODES] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "discard",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "discard_if",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "elect",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "emit_vertex",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_STREAM_ID,
   },
   .index_map = {
      [NIR_INTRINSIC_STREAM_ID] = 1,
    },
   .flags = 0,
},
{
   .name = "emit_vertex_with_counter",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_STREAM_ID,
   },
   .index_map = {
      [NIR_INTRINSIC_STREAM_ID] = 1,
    },
   .flags = 0,
},
{
   .name = "end_invocation_interlock",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "end_patch_ir3",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "end_primitive",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_STREAM_ID,
   },
   .index_map = {
      [NIR_INTRINSIC_STREAM_ID] = 1,
    },
   .flags = 0,
},
{
   .name = "end_primitive_with_counter",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_STREAM_ID,
   },
   .index_map = {
      [NIR_INTRINSIC_STREAM_ID] = 1,
    },
   .flags = 0,
},
{
   .name = "exclusive_scan",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_REDUCTION_OP,
   },
   .index_map = {
      [NIR_INTRINSIC_REDUCTION_OP] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "execute_callable",
   .num_srcs = 2,
   .src_components = {
      1, -1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "first_invocation",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "get_ssbo_size",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "get_ubo_size",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "global_atomic_add",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_and",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_comp_swap",
   .num_srcs = 3,
   .src_components = {
      1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_exchange",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_fadd",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_fcomp_swap",
   .num_srcs = 3,
   .src_components = {
      1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_fmax",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_fmin",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_imax",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_imin",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_or",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_umax",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_umin",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "global_atomic_xor",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "group_memory_barrier",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "ignore_ray_intersection",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "image_atomic_add",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_and",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_comp_swap",
   .num_srcs = 5,
   .src_components = {
      1, 4, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_dec_wrap",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_exchange",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_fadd",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_fmax",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_fmin",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_imax",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_imin",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_inc_wrap",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_or",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_umax",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_umin",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_atomic_xor",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_add",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_and",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_comp_swap",
   .num_srcs = 5,
   .src_components = {
      1, 4, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_dec_wrap",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_exchange",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_fadd",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_fmax",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_fmin",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_imax",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_imin",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_inc_wrap",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_or",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_umax",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_umin",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_atomic_xor",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_deref_format",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "image_deref_load",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_DEST_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_DEST_TYPE] = 2,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "image_deref_load_param_intel",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "image_deref_load_raw_intel",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "image_deref_order",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "image_deref_samples",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "image_deref_size",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "image_deref_sparse_load",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_DEST_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_DEST_TYPE] = 2,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "image_deref_store",
   .num_srcs = 5,
   .src_components = {
      1, 4, 1, 0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_SRC_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_SRC_TYPE] = 2,
    },
   .flags = 0,
},
{
   .name = "image_deref_store_raw_intel",
   .num_srcs = 3,
   .src_components = {
      1, 1, 0
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "image_format",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "image_load",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 5,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_DEST_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
      [NIR_INTRINSIC_DEST_TYPE] = 5,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "image_load_raw_intel",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "image_order",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "image_samples",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "image_size",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "image_sparse_load",
   .num_srcs = 4,
   .src_components = {
      1, 4, 1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 5,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_DEST_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
      [NIR_INTRINSIC_DEST_TYPE] = 5,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "image_store",
   .num_srcs = 5,
   .src_components = {
      1, 4, 1, 0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 5,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_SRC_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
      [NIR_INTRINSIC_SRC_TYPE] = 5,
    },
   .flags = 0,
},
{
   .name = "image_store_raw_intel",
   .num_srcs = 3,
   .src_components = {
      1, 1, 0
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_IMAGE_DIM,
      NIR_INTRINSIC_IMAGE_ARRAY,
      NIR_INTRINSIC_FORMAT,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_IMAGE_DIM] = 1,
      [NIR_INTRINSIC_IMAGE_ARRAY] = 2,
      [NIR_INTRINSIC_FORMAT] = 3,
      [NIR_INTRINSIC_ACCESS] = 4,
    },
   .flags = 0,
},
{
   .name = "inclusive_scan",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_REDUCTION_OP,
   },
   .index_map = {
      [NIR_INTRINSIC_REDUCTION_OP] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "interp_deref_at_centroid",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "interp_deref_at_offset",
   .num_srcs = 2,
   .src_components = {
      1, 2
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "interp_deref_at_sample",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "interp_deref_at_vertex",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "is_helper_invocation",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "is_sparse_texels_resident",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x1,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "last_invocation",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_aa_line_width",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_barycentric_at_offset",
   .num_srcs = 1,
   .src_components = {
      2
   },
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_INTERP_MODE,
   },
   .index_map = {
      [NIR_INTRINSIC_INTERP_MODE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_barycentric_at_sample",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_INTERP_MODE,
   },
   .index_map = {
      [NIR_INTRINSIC_INTERP_MODE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_barycentric_centroid",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_INTERP_MODE,
   },
   .index_map = {
      [NIR_INTRINSIC_INTERP_MODE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_barycentric_model",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_INTERP_MODE,
   },
   .index_map = {
      [NIR_INTRINSIC_INTERP_MODE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_barycentric_pixel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_INTERP_MODE,
   },
   .index_map = {
      [NIR_INTRINSIC_INTERP_MODE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_barycentric_sample",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_INTERP_MODE,
   },
   .index_map = {
      [NIR_INTRINSIC_INTERP_MODE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_base_global_invocation_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_base_instance",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_base_vertex",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_base_work_group_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_blend_const_color_a_float",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_blend_const_color_aaaa8888_unorm",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_blend_const_color_b_float",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_blend_const_color_g_float",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_blend_const_color_r_float",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_blend_const_color_rgba",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_blend_const_color_rgba8888_unorm",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_btd_dss_id_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_btd_global_arg_addr_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x40,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_btd_local_arg_addr_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x40,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_btd_resume_sbt_addr_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x40,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_btd_stack_id_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_buffer_amd",
   .num_srcs = 3,
   .src_components = {
      4, 1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_IS_SWIZZLED,
      NIR_INTRINSIC_SLC_AMD,
      NIR_INTRINSIC_MEMORY_MODES,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_IS_SWIZZLED] = 2,
      [NIR_INTRINSIC_SLC_AMD] = 3,
      [NIR_INTRINSIC_MEMORY_MODES] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_callable_sbt_addr_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x40,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_callable_sbt_stride_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x10,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_color0",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_color1",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_constant",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_RANGE,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_RANGE] = 2,
      [NIR_INTRINSIC_ALIGN_MUL] = 3,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_constant_base_ptr",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_deref",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_deref_block_intel",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_desc_set_address_intel",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x40,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_draw_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_fb_layers_v3d",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_first_vertex",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_frag_coord",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_frag_shading_rate",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_front_face",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x21,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_fs_input_interp_deltas",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_COMPONENT,
      NIR_INTRINSIC_IO_SEMANTICS,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_COMPONENT] = 2,
      [NIR_INTRINSIC_IO_SEMANTICS] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_global",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_global_block_intel",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_global_const_block_intel",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_global_constant",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_global_constant_bounded",
   .num_srcs = 3,
   .src_components = {
      1, 1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_global_constant_offset",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_global_invocation_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_global_invocation_id_zero_base",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_global_invocation_index",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_global_ir3",
   .num_srcs = 2,
   .src_components = {
      2, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_gs_header_ir3",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_gs_vertex_offset_amd",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_helper_invocation",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x21,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_hs_patch_stride_ir3",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_input",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_COMPONENT,
      NIR_INTRINSIC_DEST_TYPE,
      NIR_INTRINSIC_IO_SEMANTICS,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_COMPONENT] = 2,
      [NIR_INTRINSIC_DEST_TYPE] = 3,
      [NIR_INTRINSIC_IO_SEMANTICS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_input_vertex",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_COMPONENT,
      NIR_INTRINSIC_DEST_TYPE,
      NIR_INTRINSIC_IO_SEMANTICS,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_COMPONENT] = 2,
      [NIR_INTRINSIC_DEST_TYPE] = 3,
      [NIR_INTRINSIC_IO_SEMANTICS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_instance_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_interpolated_input",
   .num_srcs = 2,
   .src_components = {
      2, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_COMPONENT,
      NIR_INTRINSIC_DEST_TYPE,
      NIR_INTRINSIC_IO_SEMANTICS,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_COMPONENT] = 2,
      [NIR_INTRINSIC_DEST_TYPE] = 3,
      [NIR_INTRINSIC_IO_SEMANTICS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_invocation_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_is_indexed_draw",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_kernel_input",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_RANGE,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_RANGE] = 2,
      [NIR_INTRINSIC_ALIGN_MUL] = 3,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_layer_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_leaf_opaque_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x1,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_leaf_procedural_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x1,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_line_coord",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_line_width",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_local_group_size",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_local_invocation_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_local_invocation_index",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_local_shared_r600",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_num_subgroups",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_num_work_groups",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_output",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_COMPONENT,
      NIR_INTRINSIC_DEST_TYPE,
      NIR_INTRINSIC_IO_SEMANTICS,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_COMPONENT] = 2,
      [NIR_INTRINSIC_DEST_TYPE] = 3,
      [NIR_INTRINSIC_IO_SEMANTICS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_param",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_PARAM_IDX,
   },
   .index_map = {
      [NIR_INTRINSIC_PARAM_IDX] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_patch_vertices_in",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_per_vertex_input",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_COMPONENT,
      NIR_INTRINSIC_DEST_TYPE,
      NIR_INTRINSIC_IO_SEMANTICS,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_COMPONENT] = 2,
      [NIR_INTRINSIC_DEST_TYPE] = 3,
      [NIR_INTRINSIC_IO_SEMANTICS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_per_vertex_output",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_COMPONENT,
      NIR_INTRINSIC_DEST_TYPE,
      NIR_INTRINSIC_IO_SEMANTICS,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_COMPONENT] = 2,
      [NIR_INTRINSIC_DEST_TYPE] = 3,
      [NIR_INTRINSIC_IO_SEMANTICS] = 4,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_point_coord",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_primitive_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_primitive_location_ir3",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_DRIVER_LOCATION,
   },
   .index_map = {
      [NIR_INTRINSIC_DRIVER_LOCATION] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_printf_buffer_address",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ptr_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "load_push_constant",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_RANGE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_RANGE] = 2,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_raw_output_pan",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_base_mem_addr_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x40,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_flags",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_geometry_index",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_hit_kind",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_hit_sbt_addr_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x40,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_hit_sbt_stride_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x10,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_hw_stack_size_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_instance_custom_index",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_launch_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_launch_size",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_miss_sbt_addr_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x40,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_miss_sbt_stride_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x10,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_num_dss_rt_stacks_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_object_direction",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_object_origin",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_object_to_world",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_COLUMN,
   },
   .index_map = {
      [NIR_INTRINSIC_COLUMN] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_sw_stack_size_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_t_max",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_t_min",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_world_direction",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_world_origin",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ray_world_to_object",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_COLUMN,
   },
   .index_map = {
      [NIR_INTRINSIC_COLUMN] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_reloc_const_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_PARAM_IDX,
   },
   .index_map = {
      [NIR_INTRINSIC_PARAM_IDX] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ring_es2gs_offset_amd",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ring_esgs_amd",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ring_tess_factors_amd",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ring_tess_factors_offset_amd",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ring_tess_offchip_amd",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ring_tess_offchip_offset_amd",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_sample_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_sample_id_no_per_sample",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_sample_mask_in",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_sample_pos",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_sample_pos_from_id",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_sample_positions_pan",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x40,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_sampler_lod_parameters_pan",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_scratch",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_ALIGN_MUL] = 1,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 2,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_scratch_base_ptr",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_scratch_dxil",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_shader_record_ptr",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x40,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_shared",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_shared_base_ptr",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_shared_block_intel",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_shared_dxil",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_shared_ir3",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_simd_width_intel",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_size_ir3",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ssbo",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_ssbo_address",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ssbo_block_intel",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_ssbo_ir3",
   .num_srcs = 3,
   .src_components = {
      1, 1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_subgroup_eq_mask",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_subgroup_ge_mask",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_subgroup_gt_mask",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_subgroup_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_subgroup_invocation",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_subgroup_le_mask",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_subgroup_lt_mask",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_subgroup_size",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tcs_header_ir3",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tcs_in_param_base_r600",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tcs_num_patches_amd",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tcs_out_param_base_r600",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tcs_rel_patch_id_r600",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tcs_tess_factor_base_r600",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tess_coord",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tess_coord_r600",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tess_factor_base_ir3",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tess_level_inner",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tess_level_inner_default",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tess_level_outer",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tess_level_outer_default",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tess_param_base_ir3",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tess_rel_patch_id_amd",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_texture_rect_scaling",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_tlb_color_v3d",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_COMPONENT,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_COMPONENT] = 2,
    },
   .flags = 0,
},
{
   .name = "load_ubo",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 5,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
      NIR_INTRINSIC_RANGE_BASE,
      NIR_INTRINSIC_RANGE,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
      [NIR_INTRINSIC_RANGE_BASE] = 4,
      [NIR_INTRINSIC_RANGE] = 5,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_ubo_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "load_ubo_vec4",
   .num_srcs = 2,
   .src_components = {
      -1, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_COMPONENT,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_COMPONENT] = 2,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_uniform",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_RANGE,
      NIR_INTRINSIC_DEST_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_RANGE] = 2,
      [NIR_INTRINSIC_DEST_TYPE] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_user_clip_plane",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_UCP_ID,
   },
   .index_map = {
      [NIR_INTRINSIC_UCP_ID] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_user_data_amd",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 4,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_vertex_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_vertex_id_zero_base",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_view_index",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_viewport_offset",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_viewport_scale",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_viewport_x_scale",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_viewport_y_scale",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_viewport_z_offset",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_viewport_z_scale",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_vs_primitive_stride_ir3",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_vs_vertex_stride_ir3",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_vulkan_descriptor",
   .num_srcs = 1,
   .src_components = {
      -1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_DESC_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_DESC_TYPE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_work_dim",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_work_group_id",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x60,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "load_work_group_id_zero_base",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 3,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "masked_swizzle_amd",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_SWIZZLE_MASK,
   },
   .index_map = {
      [NIR_INTRINSIC_SWIZZLE_MASK] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "mbcnt_amd",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "memcpy_deref",
   .num_srcs = 3,
   .src_components = {
      -1, -1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_DST_ACCESS,
      NIR_INTRINSIC_SRC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_DST_ACCESS] = 1,
      [NIR_INTRINSIC_SRC_ACCESS] = 2,
    },
   .flags = 0,
},
{
   .name = "memory_barrier",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "memory_barrier_atomic_counter",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "memory_barrier_buffer",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "memory_barrier_image",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "memory_barrier_shared",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "memory_barrier_tcs_patch",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "nop",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "printf",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "quad_broadcast",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "quad_swap_diagonal",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "quad_swap_horizontal",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "quad_swap_vertical",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "quad_swizzle_amd",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_SWIZZLE_MASK,
   },
   .index_map = {
      [NIR_INTRINSIC_SWIZZLE_MASK] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "read_first_invocation",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "read_invocation",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "reduce",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_REDUCTION_OP,
      NIR_INTRINSIC_CLUSTER_SIZE,
   },
   .index_map = {
      [NIR_INTRINSIC_REDUCTION_OP] = 1,
      [NIR_INTRINSIC_CLUSTER_SIZE] = 2,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "report_ray_intersection",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "scoped_barrier",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_EXECUTION_SCOPE,
      NIR_INTRINSIC_MEMORY_SCOPE,
      NIR_INTRINSIC_MEMORY_SEMANTICS,
      NIR_INTRINSIC_MEMORY_MODES,
   },
   .index_map = {
      [NIR_INTRINSIC_EXECUTION_SCOPE] = 1,
      [NIR_INTRINSIC_MEMORY_SCOPE] = 2,
      [NIR_INTRINSIC_MEMORY_SEMANTICS] = 3,
      [NIR_INTRINSIC_MEMORY_MODES] = 4,
    },
   .flags = 0,
},
{
   .name = "set_vertex_and_primitive_count",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_STREAM_ID,
   },
   .index_map = {
      [NIR_INTRINSIC_STREAM_ID] = 1,
    },
   .flags = 0,
},
{
   .name = "shader_clock",
   .num_srcs = 0,
   .has_dest = true,
   .dest_components = 2,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_MEMORY_SCOPE,
   },
   .index_map = {
      [NIR_INTRINSIC_MEMORY_SCOPE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "shared_atomic_add",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_add_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "shared_atomic_and",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_and_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "shared_atomic_comp_swap",
   .num_srcs = 3,
   .src_components = {
      1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_comp_swap_dxil",
   .num_srcs = 3,
   .src_components = {
      1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "shared_atomic_exchange",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_exchange_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "shared_atomic_fadd",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_fcomp_swap",
   .num_srcs = 3,
   .src_components = {
      1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_fmax",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_fmin",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_imax",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_imax_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "shared_atomic_imin",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_imin_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "shared_atomic_or",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_or_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "shared_atomic_umax",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_umax_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "shared_atomic_umin",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_umin_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "shared_atomic_xor",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_BASE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
    },
   .flags = 0,
},
{
   .name = "shared_atomic_xor_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "shuffle",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "shuffle_down",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "shuffle_up",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "shuffle_xor",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "sparse_residency_code_and",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x20,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "ssbo_atomic_add",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_add_ir3",
   .num_srcs = 4,
   .src_components = {
      1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_and",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_and_ir3",
   .num_srcs = 4,
   .src_components = {
      1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_comp_swap",
   .num_srcs = 4,
   .src_components = {
      -1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_comp_swap_ir3",
   .num_srcs = 5,
   .src_components = {
      1, 1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_exchange",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_exchange_ir3",
   .num_srcs = 4,
   .src_components = {
      1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_fadd",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_fcomp_swap",
   .num_srcs = 4,
   .src_components = {
      -1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_fmax",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_fmin",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_imax",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_imax_ir3",
   .num_srcs = 4,
   .src_components = {
      1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_imin",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_imin_ir3",
   .num_srcs = 4,
   .src_components = {
      1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_or",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_or_ir3",
   .num_srcs = 4,
   .src_components = {
      1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_umax",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_umax_ir3",
   .num_srcs = 4,
   .src_components = {
      1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_umin",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_umin_ir3",
   .num_srcs = 4,
   .src_components = {
      1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_xor",
   .num_srcs = 3,
   .src_components = {
      -1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "ssbo_atomic_xor_ir3",
   .num_srcs = 4,
   .src_components = {
      1, 1, 1, 1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
    },
   .flags = 0,
},
{
   .name = "store_buffer_amd",
   .num_srcs = 4,
   .src_components = {
      0, 4, 1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 5,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_IS_SWIZZLED,
      NIR_INTRINSIC_SLC_AMD,
      NIR_INTRINSIC_MEMORY_MODES,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_WRITE_MASK] = 2,
      [NIR_INTRINSIC_IS_SWIZZLED] = 3,
      [NIR_INTRINSIC_SLC_AMD] = 4,
      [NIR_INTRINSIC_MEMORY_MODES] = 5,
    },
   .flags = 0,
},
{
   .name = "store_combined_output_pan",
   .num_srcs = 4,
   .src_components = {
      0, 1, 1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_COMPONENT,
      NIR_INTRINSIC_SRC_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_COMPONENT] = 2,
      [NIR_INTRINSIC_SRC_TYPE] = 3,
    },
   .flags = 0,
},
{
   .name = "store_deref",
   .num_srcs = 2,
   .src_components = {
      -1, 0
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_WRITE_MASK] = 1,
      [NIR_INTRINSIC_ACCESS] = 2,
    },
   .flags = 0,
},
{
   .name = "store_deref_block_intel",
   .num_srcs = 2,
   .src_components = {
      -1, 0
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 2,
   .indices = {
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_ACCESS,
   },
   .index_map = {
      [NIR_INTRINSIC_WRITE_MASK] = 1,
      [NIR_INTRINSIC_ACCESS] = 2,
    },
   .flags = 0,
},
{
   .name = "store_global",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_WRITE_MASK] = 1,
      [NIR_INTRINSIC_ACCESS] = 2,
      [NIR_INTRINSIC_ALIGN_MUL] = 3,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 4,
    },
   .flags = 0,
},
{
   .name = "store_global_block_intel",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_WRITE_MASK] = 1,
      [NIR_INTRINSIC_ACCESS] = 2,
      [NIR_INTRINSIC_ALIGN_MUL] = 3,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 4,
    },
   .flags = 0,
},
{
   .name = "store_global_ir3",
   .num_srcs = 3,
   .src_components = {
      0, 2, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_ACCESS] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = 0,
},
{
   .name = "store_local_shared_r600",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_WRITE_MASK,
   },
   .index_map = {
      [NIR_INTRINSIC_WRITE_MASK] = 1,
    },
   .flags = 0,
},
{
   .name = "store_output",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 5,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_COMPONENT,
      NIR_INTRINSIC_SRC_TYPE,
      NIR_INTRINSIC_IO_SEMANTICS,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_WRITE_MASK] = 2,
      [NIR_INTRINSIC_COMPONENT] = 3,
      [NIR_INTRINSIC_SRC_TYPE] = 4,
      [NIR_INTRINSIC_IO_SEMANTICS] = 5,
    },
   .flags = 0,
},
{
   .name = "store_per_vertex_output",
   .num_srcs = 3,
   .src_components = {
      0, 1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 5,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_COMPONENT,
      NIR_INTRINSIC_SRC_TYPE,
      NIR_INTRINSIC_IO_SEMANTICS,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_WRITE_MASK] = 2,
      [NIR_INTRINSIC_COMPONENT] = 3,
      [NIR_INTRINSIC_SRC_TYPE] = 4,
      [NIR_INTRINSIC_IO_SEMANTICS] = 5,
    },
   .flags = 0,
},
{
   .name = "store_raw_output_pan",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "store_scratch",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
      NIR_INTRINSIC_WRITE_MASK,
   },
   .index_map = {
      [NIR_INTRINSIC_ALIGN_MUL] = 1,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 2,
      [NIR_INTRINSIC_WRITE_MASK] = 3,
    },
   .flags = 0,
},
{
   .name = "store_scratch_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "store_shared",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_WRITE_MASK] = 2,
      [NIR_INTRINSIC_ALIGN_MUL] = 3,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 4,
    },
   .flags = 0,
},
{
   .name = "store_shared_block_intel",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_WRITE_MASK] = 2,
      [NIR_INTRINSIC_ALIGN_MUL] = 3,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 4,
    },
   .flags = 0,
},
{
   .name = "store_shared_dxil",
   .num_srcs = 2,
   .src_components = {
      1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "store_shared_ir3",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_ALIGN_MUL] = 2,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 3,
    },
   .flags = 0,
},
{
   .name = "store_shared_masked_dxil",
   .num_srcs = 3,
   .src_components = {
      1, 1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "store_ssbo",
   .num_srcs = 3,
   .src_components = {
      0, -1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_WRITE_MASK] = 1,
      [NIR_INTRINSIC_ACCESS] = 2,
      [NIR_INTRINSIC_ALIGN_MUL] = 3,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 4,
    },
   .flags = 0,
},
{
   .name = "store_ssbo_block_intel",
   .num_srcs = 3,
   .src_components = {
      0, -1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_WRITE_MASK] = 1,
      [NIR_INTRINSIC_ACCESS] = 2,
      [NIR_INTRINSIC_ALIGN_MUL] = 3,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 4,
    },
   .flags = 0,
},
{
   .name = "store_ssbo_ir3",
   .num_srcs = 4,
   .src_components = {
      0, 1, 1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 4,
   .indices = {
      NIR_INTRINSIC_WRITE_MASK,
      NIR_INTRINSIC_ACCESS,
      NIR_INTRINSIC_ALIGN_MUL,
      NIR_INTRINSIC_ALIGN_OFFSET,
   },
   .index_map = {
      [NIR_INTRINSIC_WRITE_MASK] = 1,
      [NIR_INTRINSIC_ACCESS] = 2,
      [NIR_INTRINSIC_ALIGN_MUL] = 3,
      [NIR_INTRINSIC_ALIGN_OFFSET] = 4,
    },
   .flags = 0,
},
{
   .name = "store_ssbo_masked_dxil",
   .num_srcs = 4,
   .src_components = {
      1, 1, 1, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "store_tf_r600",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "store_tlb_sample_color_v3d",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_BASE,
      NIR_INTRINSIC_COMPONENT,
      NIR_INTRINSIC_SRC_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_BASE] = 1,
      [NIR_INTRINSIC_COMPONENT] = 2,
      [NIR_INTRINSIC_SRC_TYPE] = 3,
    },
   .flags = 0,
},
{
   .name = "terminate",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "terminate_if",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "terminate_ray",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "trace_ray",
   .num_srcs = 11,
   .src_components = {
      -1, 1, 1, 1, 1, 1, 3, 1, 3, 1, -1
   },
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "trace_ray_commit_intel",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "trace_ray_continue_intel",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "trace_ray_initial_intel",
   .num_srcs = 0,
   .has_dest = false,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = 0,
},
{
   .name = "vote_all",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "vote_any",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "vote_feq",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "vote_ieq",
   .num_srcs = 1,
   .src_components = {
      0
   },
   .has_dest = true,
   .dest_components = 1,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
{
   .name = "vulkan_resource_index",
   .num_srcs = 1,
   .src_components = {
      1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 3,
   .indices = {
      NIR_INTRINSIC_DESC_SET,
      NIR_INTRINSIC_BINDING,
      NIR_INTRINSIC_DESC_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_DESC_SET] = 1,
      [NIR_INTRINSIC_BINDING] = 2,
      [NIR_INTRINSIC_DESC_TYPE] = 3,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "vulkan_resource_reindex",
   .num_srcs = 2,
   .src_components = {
      0, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = -1,
   .num_indices = 1,
   .indices = {
      NIR_INTRINSIC_DESC_TYPE,
   },
   .index_map = {
      [NIR_INTRINSIC_DESC_TYPE] = 1,
    },
   .flags = NIR_INTRINSIC_CAN_ELIMINATE | NIR_INTRINSIC_CAN_REORDER,
},
{
   .name = "write_invocation_amd",
   .num_srcs = 3,
   .src_components = {
      0, 0, 1
   },
   .has_dest = true,
   .dest_components = 0,
   .dest_bit_sizes = 0x0,
   .bit_size_src = 0,
   .num_indices = 0,
   .flags = NIR_INTRINSIC_CAN_ELIMINATE,
},
};

const char *nir_intrinsic_index_names[NIR_INTRINSIC_NUM_INDEX_FLAGS] = {
   "base",
   "write_mask",
   "stream_id",
   "ucp_id",
   "range_base",
   "range",
   "desc_set",
   "binding",
   "component",
   "column",
   "interp_mode",
   "reduction_op",
   "cluster_size",
   "param_idx",
   "image_dim",
   "image_array",
   "format",
   "access",
   "align_mul",
   "align_offset",
   "desc_type",
   "src_type",
   "dest_type",
   "swizzle_mask",
   "is_swizzled",
   "slc_amd",
   "dst_access",
   "src_access",
   "driver_location",
   "memory_semantics",
   "memory_modes",
   "memory_scope",
   "execution_scope",
   "io_semantics",
   "rounding_mode",
   "saturate",
};
