#include "compiler/r300_nir.h"
#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 2
 * transforms:
 *    ('fsin', 'a') => ('fsin', ('fadd', ('fmul', ('ffract', ('fadd', ('fmul', 'a', 0.15915494309189535), 0.5)), 6.283185307179586), -3.141592653589793))
 *    ('fcos', 'a') => ('fcos', ('fadd', ('fmul', ('ffract', ('fadd', ('fmul', 'a', 0.15915494309189535), 0.5)), 6.283185307179586), -3.141592653589793))
 */


static const nir_search_value_union r300_transform_vs_trig_input_values[] = {
   /* ('fsin', 'a') => ('fsin', ('fadd', ('fmul', ('ffract', ('fadd', ('fmul', 'a', 0.15915494309189535), 0.5)), 6.283185307179586), -3.141592653589793)) */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fsin,
      -1, 0,
      { 0 },
      -1,
   } },

   /* replace0_0_0_0_0_0_0 -> 0 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x3fc45f306dc9c883 /* 0.15915494309189535 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fmul,
      3, 1,
      { 0, 2 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x3fe0000000000000 /* 0.5 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fadd,
      2, 2,
      { 3, 4 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ffract,
      -1, 2,
      { 5 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x401921fb54442d18 /* 6.283185307179586 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 3,
      { 6, 7 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0xc00921fb54442d18 /* -3.141592653589793 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fadd,
      0, 4,
      { 8, 9 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fsin,
      -1, 4,
      { 10 },
      -1,
   } },

   /* ('fcos', 'a') => ('fcos', ('fadd', ('fmul', ('ffract', ('fadd', ('fmul', 'a', 0.15915494309189535), 0.5)), 6.283185307179586), -3.141592653589793)) */
   /* search1_0 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fcos,
      -1, 0,
      { 0 },
      -1,
   } },

   /* replace1_0_0_0_0_0_0 -> 0 in the cache */
   /* replace1_0_0_0_0_0_1 -> 2 in the cache */
   /* replace1_0_0_0_0_0 -> 3 in the cache */
   /* replace1_0_0_0_0_1 -> 4 in the cache */
   /* replace1_0_0_0_0 -> 5 in the cache */
   /* replace1_0_0_0 -> 6 in the cache */
   /* replace1_0_0_1 -> 7 in the cache */
   /* replace1_0_0 -> 8 in the cache */
   /* replace1_0_1 -> 9 in the cache */
   /* replace1_0 -> 10 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fcos,
      -1, 4,
      { 10 },
      -1,
   } },

};



static const struct transform r300_transform_vs_trig_input_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 1, 11, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 12, 13, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_transform_vs_trig_input_pass_op_table[nir_num_search_ops] = {
   [nir_op_fsin] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         2,
      },
   },
   [nir_op_fcos] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         3,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_transform_vs_trig_input_transform_offsets[] = {
   0,
   0,
   1,
   3,
};

static const nir_algebraic_table r300_transform_vs_trig_input_table = {
   .transforms = r300_transform_vs_trig_input_transforms,
   .transform_offsets = r300_transform_vs_trig_input_transform_offsets,
   .pass_op_table = r300_transform_vs_trig_input_pass_op_table,
   .values = r300_transform_vs_trig_input_values,
   .expression_cond = NULL,
   .variable_cond = NULL,
};

bool
r300_transform_vs_trig_input(nir_shader *shader)
{
   bool progress = false;
   bool condition_flags[1];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(14 == ARRAY_SIZE(r300_transform_vs_trig_input_values));
   condition_flags[0] = true;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_transform_vs_trig_input_table);
   }

   return progress;
}

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 2
 * transforms:
 *    ('fsin', 'a') => ('fsin', ('ffract', ('fmul', 'a', 0.15915494309189535)))
 *    ('fcos', 'a') => ('fcos', ('ffract', ('fmul', 'a', 0.15915494309189535)))
 */


static const nir_search_value_union r300_transform_fs_trig_input_values[] = {
   /* ('fsin', 'a') => ('fsin', ('ffract', ('fmul', 'a', 0.15915494309189535))) */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fsin,
      -1, 0,
      { 0 },
      -1,
   } },

   /* replace2_0_0_0 -> 0 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x3fc45f306dc9c883 /* 0.15915494309189535 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 0, 2 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ffract,
      -1, 1,
      { 3 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fsin,
      -1, 1,
      { 4 },
      -1,
   } },

   /* ('fcos', 'a') => ('fcos', ('ffract', ('fmul', 'a', 0.15915494309189535))) */
   /* search3_0 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fcos,
      -1, 0,
      { 0 },
      -1,
   } },

   /* replace3_0_0_0 -> 0 in the cache */
   /* replace3_0_0_1 -> 2 in the cache */
   /* replace3_0_0 -> 3 in the cache */
   /* replace3_0 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fcos,
      -1, 1,
      { 4 },
      -1,
   } },

};



static const struct transform r300_transform_fs_trig_input_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 1, 5, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 6, 7, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_transform_fs_trig_input_pass_op_table[nir_num_search_ops] = {
   [nir_op_fsin] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         2,
      },
   },
   [nir_op_fcos] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         3,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_transform_fs_trig_input_transform_offsets[] = {
   0,
   0,
   1,
   3,
};

static const nir_algebraic_table r300_transform_fs_trig_input_table = {
   .transforms = r300_transform_fs_trig_input_transforms,
   .transform_offsets = r300_transform_fs_trig_input_transform_offsets,
   .pass_op_table = r300_transform_fs_trig_input_pass_op_table,
   .values = r300_transform_fs_trig_input_values,
   .expression_cond = NULL,
   .variable_cond = NULL,
};

bool
r300_transform_fs_trig_input(nir_shader *shader)
{
   bool progress = false;
   bool condition_flags[1];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(8 == ARRAY_SIZE(r300_transform_fs_trig_input_values));
   condition_flags[0] = true;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_transform_fs_trig_input_table);
   }

   return progress;
}

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 1
 * transforms:
 *    ('fmul', ('fadd', ('fadd', ('fabs', 'a'), 0.5), ('fneg', ('ffract', ('fadd', ('fabs', 'a'), 0.5)))), ('fadd', ('b2f', ('!flt', 0.0, 'a')), ('fneg', ('b2f', ('!flt', 'a', 0.0))))) => ('fround_even', 'a')
 */


static const nir_search_value_union r300_nir_fuse_fround_d3d9_values[] = {
   /* ('fmul', ('fadd', ('fadd', ('fabs', 'a'), 0.5), ('fneg', ('ffract', ('fadd', ('fabs', 'a'), 0.5)))), ('fadd', ('b2f', ('!flt', 0.0, 'a')), ('fneg', ('b2f', ('!flt', 'a', 0.0))))) => ('fround_even', 'a') */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fabs,
      -1, 0,
      { 0 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x3fe0000000000000 /* 0.5 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fadd,
      2, 1,
      { 1, 2 },
      -1,
   } },
   /* search4_0_1_0_0_0_0 -> 0 in the cache */
   /* search4_0_1_0_0_0 -> 1 in the cache */
   /* search4_0_1_0_0_1 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fadd,
      3, 1,
      { 1, 2 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ffract,
      -1, 1,
      { 4 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fneg,
      -1, 1,
      { 5 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fadd,
      1, 3,
      { 3, 6 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x0 /* 0.0 */ },
   } },
   /* search4_1_0_0_1 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      true,
      false,
      nir_op_flt,
      -1, 0,
      { 8, 0 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_search_op_b2f,
      -1, 0,
      { 9 },
      -1,
   } },
   /* search4_1_1_0_0_0 -> 0 in the cache */
   /* search4_1_1_0_0_1 -> 8 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      true,
      false,
      nir_op_flt,
      -1, 0,
      { 0, 8 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_search_op_b2f,
      -1, 0,
      { 11 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 12 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fadd,
      4, 1,
      { 10, 13 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 5,
      { 7, 14 },
      -1,
   } },

   /* replace4_0 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fround_even,
      -1, 0,
      { 0 },
      -1,
   } },

};



static const struct transform r300_nir_fuse_fround_d3d9_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 15, 16, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_nir_fuse_fround_d3d9_pass_op_table[nir_num_search_ops] = {
   [nir_op_fmul] = {
      .filter = (const uint16_t []) {
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         1,
         2,
         0,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         0,
         0,
         0,
         0,
         0,
         15,
         0,
         15,
         0,
      },
   },
   [nir_op_fadd] = {
      .filter = (const uint16_t []) {
         0,
         1,
         2,
         0,
         0,
         0,
         3,
         0,
         4,
         4,
         0,
         5,
         6,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 7,
      .table = (const uint16_t []) {
      
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         6,
         0,
         0,
         0,
         0,
         0,
         6,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         14,
         0,
         0,
         0,
         0,
         0,
         13,
         0,
         0,
         0,
         0,
         0,
         13,
         0,
         0,
         0,
         0,
         0,
         14,
         0,
         0,
         0,
      },
   },
   [nir_op_fabs] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         2,
      },
   },
   [nir_op_fneg] = {
      .filter = (const uint16_t []) {
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         1,
         0,
         1,
         2,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         0,
         11,
         12,
      },
   },
   [nir_op_ffract] = {
      .filter = (const uint16_t []) {
         0,
         0,
         0,
         0,
         0,
         0,
         1,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         10,
      },
   },
   [nir_search_op_b2f] = {
      .filter = (const uint16_t []) {
         0,
         0,
         0,
         1,
         2,
         3,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 4,
      .table = (const uint16_t []) {
      
         0,
         7,
         8,
         9,
      },
   },
   [nir_op_flt] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         3,
         4,
         5,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_nir_fuse_fround_d3d9_transform_offsets[] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   1,
};

static const nir_algebraic_table r300_nir_fuse_fround_d3d9_table = {
   .transforms = r300_nir_fuse_fround_d3d9_transforms,
   .transform_offsets = r300_nir_fuse_fround_d3d9_transform_offsets,
   .pass_op_table = r300_nir_fuse_fround_d3d9_pass_op_table,
   .values = r300_nir_fuse_fround_d3d9_values,
   .expression_cond = NULL,
   .variable_cond = NULL,
};

bool
r300_nir_fuse_fround_d3d9(nir_shader *shader)
{
   bool progress = false;
   bool condition_flags[1];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(17 == ARRAY_SIZE(r300_nir_fuse_fround_d3d9_values));
   condition_flags[0] = true;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_fuse_fround_d3d9_table);
   }

   return progress;
}

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 4
 * transforms:
 *    ('bcsel@32(is_only_used_as_float)', ('feq', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('seq', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('seq', 'a', 'b'))))
 *    ('bcsel@32(is_only_used_as_float)', ('fneu', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('sne', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('sne', 'a', 'b'))))
 *    ('bcsel@32(is_only_used_as_float)', ('flt', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('slt', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('slt', 'a', 'b'))))
 *    ('bcsel@32(is_only_used_as_float)', ('fge', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('sge', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('sge', 'a', 'b'))))
 */


static const nir_search_value_union r300_nir_lower_bool_to_float_values[] = {
   /* ('bcsel@32(is_only_used_as_float)', ('feq', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('seq', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('seq', 'a', 'b')))) */
   { .variable = {
      { nir_search_value_variable, 32 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, 32 },
      1, /* b */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      true,
      nir_op_feq,
      0, 1,
      { 0, 1 },
      -1,
   } },
   { .variable = {
      { nir_search_value_variable, 32 },
      2, /* c */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, 32 },
      3, /* d */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 1,
      { 2, 3, 4 },
      0,
   } },

   /* replace5_0_0 -> 3 in the cache */
   /* replace5_0_1_0 -> 0 in the cache */
   /* replace5_0_1_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_seq,
      2, 1,
      { 0, 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 2,
      { 3, 6 },
      -1,
   } },
   /* replace5_1_0 -> 4 in the cache */
   /* replace5_1_1_0 -> 4 in the cache */
   /* replace5_1_1_1_0 -> 0 in the cache */
   /* replace5_1_1_1_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_seq,
      4, 1,
      { 0, 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fmul,
      3, 2,
      { 4, 8 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fsub,
      -1, 2,
      { 4, 9 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fadd,
      0, 5,
      { 7, 10 },
      -1,
   } },

   /* ('bcsel@32(is_only_used_as_float)', ('fneu', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('sne', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('sne', 'a', 'b')))) */
   /* search6_0_0 -> 0 in the cache */
   /* search6_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      true,
      nir_op_fneu,
      0, 1,
      { 0, 1 },
      -1,
   } },
   /* search6_1 -> 3 in the cache */
   /* search6_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 1,
      { 12, 3, 4 },
      0,
   } },

   /* replace6_0_0 -> 3 in the cache */
   /* replace6_0_1_0 -> 0 in the cache */
   /* replace6_0_1_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_sne,
      2, 1,
      { 0, 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 2,
      { 3, 14 },
      -1,
   } },
   /* replace6_1_0 -> 4 in the cache */
   /* replace6_1_1_0 -> 4 in the cache */
   /* replace6_1_1_1_0 -> 0 in the cache */
   /* replace6_1_1_1_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_sne,
      4, 1,
      { 0, 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fmul,
      3, 2,
      { 4, 16 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fsub,
      -1, 2,
      { 4, 17 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fadd,
      0, 5,
      { 15, 18 },
      -1,
   } },

   /* ('bcsel@32(is_only_used_as_float)', ('flt', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('slt', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('slt', 'a', 'b')))) */
   /* search7_0_0 -> 0 in the cache */
   /* search7_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      true,
      nir_op_flt,
      -1, 0,
      { 0, 1 },
      -1,
   } },
   /* search7_1 -> 3 in the cache */
   /* search7_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 0,
      { 20, 3, 4 },
      0,
   } },

   /* replace7_0_0 -> 3 in the cache */
   /* replace7_0_1_0 -> 0 in the cache */
   /* replace7_0_1_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_slt,
      -1, 0,
      { 0, 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 1,
      { 3, 22 },
      -1,
   } },
   /* replace7_1_0 -> 4 in the cache */
   /* replace7_1_1_0 -> 4 in the cache */
   /* replace7_1_1_1_0 -> 0 in the cache */
   /* replace7_1_1_1_1 -> 1 in the cache */
   /* replace7_1_1_1 -> 22 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fmul,
      2, 1,
      { 4, 22 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fsub,
      -1, 1,
      { 4, 24 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fadd,
      0, 3,
      { 23, 25 },
      -1,
   } },

   /* ('bcsel@32(is_only_used_as_float)', ('fge', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('sge', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('sge', 'a', 'b')))) */
   /* search8_0_0 -> 0 in the cache */
   /* search8_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      true,
      nir_op_fge,
      -1, 0,
      { 0, 1 },
      -1,
   } },
   /* search8_1 -> 3 in the cache */
   /* search8_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 0,
      { 27, 3, 4 },
      0,
   } },

   /* replace8_0_0 -> 3 in the cache */
   /* replace8_0_1_0 -> 0 in the cache */
   /* replace8_0_1_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_sge,
      -1, 0,
      { 0, 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 1,
      { 3, 29 },
      -1,
   } },
   /* replace8_1_0 -> 4 in the cache */
   /* replace8_1_1_0 -> 4 in the cache */
   /* replace8_1_1_1_0 -> 0 in the cache */
   /* replace8_1_1_1_1 -> 1 in the cache */
   /* replace8_1_1_1 -> 29 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fmul,
      2, 1,
      { 4, 29 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fsub,
      -1, 1,
      { 4, 31 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fadd,
      0, 3,
      { 30, 32 },
      -1,
   } },

};

static const nir_search_expression_cond r300_nir_lower_bool_to_float_expression_cond[] = {
   (is_only_used_as_float),
};


static const struct transform r300_nir_lower_bool_to_float_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 5, 11, 1 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 13, 19, 1 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 21, 26, 1 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 28, 33, 1 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_nir_lower_bool_to_float_pass_op_table[nir_num_search_ops] = {
   [nir_op_bcsel] = {
      .filter = (const uint16_t []) {
         0,
         0,
         1,
         2,
         3,
         4,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 5,
      .table = (const uint16_t []) {
      
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         6,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         7,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         8,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
         9,
      },
   },
   [nir_op_feq] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         2,
      },
   },
   [nir_op_fneu] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         3,
      },
   },
   [nir_op_flt] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         4,
      },
   },
   [nir_op_fge] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         5,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_nir_lower_bool_to_float_transform_offsets[] = {
   0,
   0,
   0,
   0,
   0,
   0,
   1,
   3,
   5,
   7,
};

static const nir_algebraic_table r300_nir_lower_bool_to_float_table = {
   .transforms = r300_nir_lower_bool_to_float_transforms,
   .transform_offsets = r300_nir_lower_bool_to_float_transform_offsets,
   .pass_op_table = r300_nir_lower_bool_to_float_pass_op_table,
   .values = r300_nir_lower_bool_to_float_values,
   .expression_cond = r300_nir_lower_bool_to_float_expression_cond,
   .variable_cond = NULL,
};

bool
r300_nir_lower_bool_to_float(nir_shader *shader)
{
   bool progress = false;
   bool condition_flags[2];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(34 == ARRAY_SIZE(r300_nir_lower_bool_to_float_values));
   condition_flags[0] = true;
   condition_flags[1] = !options->has_fused_comp_and_csel;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_lower_bool_to_float_table);
   }

   return progress;
}

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 17
 * transforms:
 *    ('fadd', ('fneg', 'a'), 1.0) => ('fadd', 1.0, ('fneg', 'a'))
 *    ('fadd', 'a', -1.0) => ('fneg', ('fadd', 1.0, ('fneg', 'a')))
 *    ('fadd', -1.0, 'a') => ('fneg', ('fadd', 1.0, ('fneg', 'a')))
 *    ('ffma', 2.0, ('fneg', 'a'), 1.0) => ('ffma', ('fneg', 'a'), 2.0, 1.0)
 *    ('ffma', 'a', -2.0, 1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0))
 *    ('ffma', -2.0, 'a', 1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0))
 *    ('ffma', 2.0, 'a', -1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0))
 *    ('ffma', 'a', 2.0, -1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0))
 *    ('fmul', 'a(is_ubo_or_input)', 2.0) => ('fadd', 'a', 'a')
 *    ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 2.0)) => ('fmul', 2.0, ('fmul', 'a', 'b'))
 *    ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 4.0)) => ('fmul', 4.0, ('fmul', 'a', 'b'))
 *    ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 8.0)) => ('fmul', 8.0, ('fmul', 'a', 'b'))
 *    ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 16.0)) => ('fmul', 16.0, ('fmul', 'a', 'b'))
 *    ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.5)) => ('fmul', 0.5, ('fmul', 'a', 'b'))
 *    ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.25)) => ('fmul', 0.25, ('fmul', 'a', 'b'))
 *    ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.125)) => ('fmul', 0.125, ('fmul', 'a', 'b'))
 *    ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.0625)) => ('fmul', 0.0625, ('fmul', 'a', 'b'))
 */


static const nir_search_value_union r300_nir_prepare_presubtract_values[] = {
   /* ('fadd', ('fneg', 'a'), 1.0) => ('fadd', 1.0, ('fneg', 'a')) */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 0 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x3ff0000000000000 /* 1.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 1, 2 },
      -1,
   } },

   /* replace9_0 -> 2 in the cache */
   /* replace9_1_0 -> 0 in the cache */
   /* replace9_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 2, 1 },
      -1,
   } },

   /* ('fadd', 'a', -1.0) => ('fneg', ('fadd', 1.0, ('fneg', 'a'))) */
   /* search10_0 -> 0 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0xbff0000000000000 /* -1.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 0, 5 },
      -1,
   } },

   /* replace10_0_0 -> 2 in the cache */
   /* replace10_0_1_0 -> 0 in the cache */
   /* replace10_0_1 -> 1 in the cache */
   /* replace10_0 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fneg,
      -1, 1,
      { 4 },
      -1,
   } },

   /* ('fadd', -1.0, 'a') => ('fneg', ('fadd', 1.0, ('fneg', 'a'))) */
   /* search11_0 -> 5 in the cache */
   /* search11_1 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 5, 0 },
      -1,
   } },

   /* replace11_0_0 -> 2 in the cache */
   /* replace11_0_1_0 -> 0 in the cache */
   /* replace11_0_1 -> 1 in the cache */
   /* replace11_0 -> 4 in the cache */
   /* replace11 -> 7 in the cache */

   /* ('ffma', 2.0, ('fneg', 'a'), 1.0) => ('ffma', ('fneg', 'a'), 2.0, 1.0) */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x4000000000000000 /* 2.0 */ },
   } },
   /* search12_1_0 -> 0 in the cache */
   /* search12_1 -> 1 in the cache */
   /* search12_2 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 9, 1, 2 },
      -1,
   } },

   /* replace12_0_0 -> 0 in the cache */
   /* replace12_0 -> 1 in the cache */
   /* replace12_1 -> 9 in the cache */
   /* replace12_2 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 1, 9, 2 },
      -1,
   } },

   /* ('ffma', 'a', -2.0, 1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0)) */
   /* search13_0 -> 0 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0xc000000000000000 /* -2.0 */ },
   } },
   /* search13_2 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 0, 12, 2 },
      -1,
   } },

   /* replace13_0_0_0 -> 0 in the cache */
   /* replace13_0_0 -> 1 in the cache */
   /* replace13_0_1 -> 9 in the cache */
   /* replace13_0_2 -> 2 in the cache */
   /* replace13_0 -> 11 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fneg,
      -1, 1,
      { 11 },
      -1,
   } },

   /* ('ffma', -2.0, 'a', 1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0)) */
   /* search14_0 -> 12 in the cache */
   /* search14_1 -> 0 in the cache */
   /* search14_2 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 12, 0, 2 },
      -1,
   } },

   /* replace14_0_0_0 -> 0 in the cache */
   /* replace14_0_0 -> 1 in the cache */
   /* replace14_0_1 -> 9 in the cache */
   /* replace14_0_2 -> 2 in the cache */
   /* replace14_0 -> 11 in the cache */
   /* replace14 -> 14 in the cache */

   /* ('ffma', 2.0, 'a', -1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0)) */
   /* search15_0 -> 9 in the cache */
   /* search15_1 -> 0 in the cache */
   /* search15_2 -> 5 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 9, 0, 5 },
      -1,
   } },

   /* replace15_0_0_0 -> 0 in the cache */
   /* replace15_0_0 -> 1 in the cache */
   /* replace15_0_1 -> 9 in the cache */
   /* replace15_0_2 -> 2 in the cache */
   /* replace15_0 -> 11 in the cache */
   /* replace15 -> 14 in the cache */

   /* ('ffma', 'a', 2.0, -1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0)) */
   /* search16_0 -> 0 in the cache */
   /* search16_1 -> 9 in the cache */
   /* search16_2 -> 5 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 0, 9, 5 },
      -1,
   } },

   /* replace16_0_0_0 -> 0 in the cache */
   /* replace16_0_0 -> 1 in the cache */
   /* replace16_0_1 -> 9 in the cache */
   /* replace16_0_2 -> 2 in the cache */
   /* replace16_0 -> 11 in the cache */
   /* replace16 -> 14 in the cache */

   /* ('fmul', 'a(is_ubo_or_input)', 2.0) => ('fadd', 'a', 'a') */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      0,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   /* search17_1 -> 9 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 18, 9 },
      -1,
   } },

   /* replace17_0 -> 0 in the cache */
   /* replace17_1 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fadd,
      -1, 0,
      { 0, 0 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 2.0)) => ('fmul', 2.0, ('fmul', 'a', 'b')) */
   { .variable = {
      { nir_search_value_variable, -2 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, -2 },
      1, /* b */
      false,
      nir_type_invalid,
      0,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x4000000000000000 /* 2.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 1,
      { 22, 23 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 24 },
      -1,
   } },

   /* replace18_0 -> 23 in the cache */
   /* replace18_1_0 -> 21 in the cache */
   { .variable = {
      { nir_search_value_variable, -2 },
      1, /* b */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 1,
      { 21, 26 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 23, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 4.0)) => ('fmul', 4.0, ('fmul', 'a', 'b')) */
   /* search19_0 -> 21 in the cache */
   /* search19_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x4010000000000000 /* 4.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 1,
      { 22, 29 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 30 },
      -1,
   } },

   /* replace19_0 -> 29 in the cache */
   /* replace19_1_0 -> 21 in the cache */
   /* replace19_1_1 -> 26 in the cache */
   /* replace19_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 29, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 8.0)) => ('fmul', 8.0, ('fmul', 'a', 'b')) */
   /* search20_0 -> 21 in the cache */
   /* search20_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x4020000000000000 /* 8.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 1,
      { 22, 33 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 34 },
      -1,
   } },

   /* replace20_0 -> 33 in the cache */
   /* replace20_1_0 -> 21 in the cache */
   /* replace20_1_1 -> 26 in the cache */
   /* replace20_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 33, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 16.0)) => ('fmul', 16.0, ('fmul', 'a', 'b')) */
   /* search21_0 -> 21 in the cache */
   /* search21_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x4030000000000000 /* 16.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 1,
      { 22, 37 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 38 },
      -1,
   } },

   /* replace21_0 -> 37 in the cache */
   /* replace21_1_0 -> 21 in the cache */
   /* replace21_1_1 -> 26 in the cache */
   /* replace21_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 37, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.5)) => ('fmul', 0.5, ('fmul', 'a', 'b')) */
   /* search22_0 -> 21 in the cache */
   /* search22_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x3fe0000000000000 /* 0.5 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 1,
      { 22, 41 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 42 },
      -1,
   } },

   /* replace22_0 -> 41 in the cache */
   /* replace22_1_0 -> 21 in the cache */
   /* replace22_1_1 -> 26 in the cache */
   /* replace22_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 41, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.25)) => ('fmul', 0.25, ('fmul', 'a', 'b')) */
   /* search23_0 -> 21 in the cache */
   /* search23_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x3fd0000000000000 /* 0.25 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 1,
      { 22, 45 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 46 },
      -1,
   } },

   /* replace23_0 -> 45 in the cache */
   /* replace23_1_0 -> 21 in the cache */
   /* replace23_1_1 -> 26 in the cache */
   /* replace23_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 45, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.125)) => ('fmul', 0.125, ('fmul', 'a', 'b')) */
   /* search24_0 -> 21 in the cache */
   /* search24_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x3fc0000000000000 /* 0.125 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 1,
      { 22, 49 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 50 },
      -1,
   } },

   /* replace24_0 -> 49 in the cache */
   /* replace24_1_0 -> 21 in the cache */
   /* replace24_1_1 -> 26 in the cache */
   /* replace24_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 49, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.0625)) => ('fmul', 0.0625, ('fmul', 'a', 'b')) */
   /* search25_0 -> 21 in the cache */
   /* search25_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x3fb0000000000000 /* 0.0625 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      1, 1,
      { 22, 53 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 54 },
      -1,
   } },

   /* replace25_0 -> 53 in the cache */
   /* replace25_1_0 -> 21 in the cache */
   /* replace25_1_1 -> 26 in the cache */
   /* replace25_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 53, 27 },
      -1,
   } },

};

static const nir_search_expression_cond r300_nir_prepare_presubtract_expression_cond[] = {
   (is_used_once),
};

static const nir_search_variable_cond r300_nir_prepare_presubtract_variable_cond[] = {
   (is_ubo_or_input),
};

static const struct transform r300_nir_prepare_presubtract_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 6, 7, 0 },
   { 8, 7, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 13, 14, 0 },
   { 15, 14, 0 },
   { 16, 14, 0 },
   { 17, 14, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 19, 20, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 3, 4, 0 },
   { 6, 7, 0 },
   { 8, 7, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 10, 11, 0 },
   { 13, 14, 0 },
   { 15, 14, 0 },
   { 16, 14, 0 },
   { 17, 14, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 25, 28, 0 },
   { 31, 32, 0 },
   { 35, 36, 0 },
   { 39, 40, 0 },
   { 43, 44, 0 },
   { 47, 48, 0 },
   { 51, 52, 0 },
   { 55, 56, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 19, 20, 0 },
   { 25, 28, 0 },
   { 31, 32, 0 },
   { 35, 36, 0 },
   { 39, 40, 0 },
   { 43, 44, 0 },
   { 47, 48, 0 },
   { 51, 52, 0 },
   { 55, 56, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_nir_prepare_presubtract_pass_op_table[nir_num_search_ops] = {
   [nir_op_fadd] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         2,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         0,
         2,
         0,
         2,
         2,
         6,
         0,
         6,
         0,
      },
   },
   [nir_op_fneg] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         3,
      },
   },
   [nir_op_ffma] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         2,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         0,
         0,
         0,
         0,
         4,
         0,
         0,
         0,
         0,
         0,
         4,
         0,
         0,
         4,
         0,
         0,
         7,
         0,
         0,
         0,
         0,
         0,
         7,
         0,
         0,
         0,
         0,
      },
   },
   [nir_op_fmul] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         0,
         0,
         2,
         0,
         0,
         0,
         2,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         0,
         5,
         8,
         5,
         5,
         9,
         8,
         9,
         8,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_nir_prepare_presubtract_transform_offsets[] = {
   0,
   0,
   1,
   0,
   4,
   9,
   11,
   15,
   21,
   30,
};

static const nir_algebraic_table r300_nir_prepare_presubtract_table = {
   .transforms = r300_nir_prepare_presubtract_transforms,
   .transform_offsets = r300_nir_prepare_presubtract_transform_offsets,
   .pass_op_table = r300_nir_prepare_presubtract_pass_op_table,
   .values = r300_nir_prepare_presubtract_values,
   .expression_cond = r300_nir_prepare_presubtract_expression_cond,
   .variable_cond = r300_nir_prepare_presubtract_variable_cond,
};

bool
r300_nir_prepare_presubtract(nir_shader *shader)
{
   bool progress = false;
   bool condition_flags[2];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(57 == ARRAY_SIZE(r300_nir_prepare_presubtract_values));
   condition_flags[0] = true;
   condition_flags[1] = !options->has_fused_comp_and_csel;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_prepare_presubtract_table);
   }

   return progress;
}

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 1
 * transforms:
 *    ('fneg', ('fneg', 'a')) => a
 */


static const nir_search_value_union r300_nir_clean_double_fneg_values[] = {
   /* ('fneg', ('fneg', 'a')) => a */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 0 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 1 },
      -1,
   } },

   /* replace26 -> 0 in the cache */

};



static const struct transform r300_nir_clean_double_fneg_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 2, 0, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_nir_clean_double_fneg_pass_op_table[nir_num_search_ops] = {
   [nir_op_fneg] = {
      .filter = (const uint16_t []) {
         0,
         0,
         1,
         1,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         2,
         3,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_nir_clean_double_fneg_transform_offsets[] = {
   0,
   0,
   0,
   1,
};

static const nir_algebraic_table r300_nir_clean_double_fneg_table = {
   .transforms = r300_nir_clean_double_fneg_transforms,
   .transform_offsets = r300_nir_clean_double_fneg_transform_offsets,
   .pass_op_table = r300_nir_clean_double_fneg_pass_op_table,
   .values = r300_nir_clean_double_fneg_values,
   .expression_cond = NULL,
   .variable_cond = NULL,
};

bool
r300_nir_clean_double_fneg(nir_shader *shader)
{
   bool progress = false;
   bool condition_flags[2];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(3 == ARRAY_SIZE(r300_nir_clean_double_fneg_values));
   condition_flags[0] = true;
   condition_flags[1] = !options->has_fused_comp_and_csel;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_clean_double_fneg_table);
   }

   return progress;
}
