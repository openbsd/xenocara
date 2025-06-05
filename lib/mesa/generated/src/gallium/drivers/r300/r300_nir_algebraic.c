#include "compiler/r300_nir.h"
#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 2
 * transforms:
 *    ('fsin', 'a(needs_vs_trig_input_fixup)') => ('fsin', ('fadd', ('fmul', ('ffract', ('fadd', ('fmul', 'a', 0.15915494309189535), 0.5)), 6.283185307179586), -3.141592653589793))
 *    ('fcos', 'a(needs_vs_trig_input_fixup)') => ('fcos', ('fadd', ('fmul', ('ffract', ('fadd', ('fmul', 'a', 0.15915494309189535), 0.5)), 6.283185307179586), -3.141592653589793))
 */


static const nir_search_value_union r300_transform_vs_trig_input_values[] = {
   /* ('fsin', 'a(needs_vs_trig_input_fixup)') => ('fsin', ('fadd', ('fmul', ('ffract', ('fadd', ('fmul', 'a', 0.15915494309189535), 0.5)), 6.283185307179586), -3.141592653589793)) */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      0,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fsin,
      -1, 0,
      { 0 },
      -1,
   } },

   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x3fc45f306dc9c883ull /* 0.15915494309189535 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      3, 1,
      { 2, 3 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x3fe0000000000000ull /* 0.5 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      2, 2,
      { 4, 5 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ffract,
      -1, 2,
      { 6 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x401921fb54442d18ull /* 6.283185307179586 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      1, 3,
      { 7, 8 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0xc00921fb54442d18ull /* -3.141592653589793 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 4,
      { 9, 10 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fsin,
      -1, 4,
      { 11 },
      -1,
   } },

   /* ('fcos', 'a(needs_vs_trig_input_fixup)') => ('fcos', ('fadd', ('fmul', ('ffract', ('fadd', ('fmul', 'a', 0.15915494309189535), 0.5)), 6.283185307179586), -3.141592653589793)) */
   /* search1_0 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcos,
      -1, 0,
      { 0 },
      -1,
   } },

   /* replace1_0_0_0_0_0_0 -> 2 in the cache */
   /* replace1_0_0_0_0_0_1 -> 3 in the cache */
   /* replace1_0_0_0_0_0 -> 4 in the cache */
   /* replace1_0_0_0_0_1 -> 5 in the cache */
   /* replace1_0_0_0_0 -> 6 in the cache */
   /* replace1_0_0_0 -> 7 in the cache */
   /* replace1_0_0_1 -> 8 in the cache */
   /* replace1_0_0 -> 9 in the cache */
   /* replace1_0_1 -> 10 in the cache */
   /* replace1_0 -> 11 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcos,
      -1, 4,
      { 11 },
      -1,
   } },

};


static const nir_search_variable_cond r300_transform_vs_trig_input_variable_cond[] = {
   (needs_vs_trig_input_fixup),
};

static const struct transform r300_transform_vs_trig_input_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 1, 12, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 13, 14, 0 },
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
   .variable_cond = r300_transform_vs_trig_input_variable_cond,
};

bool
r300_transform_vs_trig_input(
   nir_shader *shader
) {
   bool progress = false;
   bool condition_flags[1];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(15 == ARRAY_SIZE(r300_transform_vs_trig_input_values));
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
      false,
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
      nir_type_float, { 0x3fc45f306dc9c883ull /* 0.15915494309189535 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
r300_transform_fs_trig_input(
   nir_shader *shader
) {
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
      false,
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
      nir_type_float, { 0x3fe0000000000000ull /* 0.5 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      nir_type_float, { 0x0ull /* 0.0 */ },
   } },
   /* search4_1_0_0_1 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      true,
      false,
      false,
      false,
      false,
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
      false,
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
      false,
      false,
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
r300_nir_fuse_fround_d3d9(
   nir_shader *shader
) {
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

/* What follows is NIR algebraic transform code for the following 8
 * transforms:
 *    ('bcsel@32(is_only_used_as_float)', ('feq', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('seq', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('seq', 'a', 'b'))))
 *    ('bcsel@32(is_only_used_as_float)', ('fneu', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('sne', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('sne', 'a', 'b'))))
 *    ('bcsel@32(is_only_used_as_float)', ('flt', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('slt', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('slt', 'a', 'b'))))
 *    ('bcsel@32(is_only_used_as_float)', ('fge', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fadd', ('fmul', 'c', ('sge', 'a', 'b')), ('fsub', 'd', ('fmul', 'd', ('sge', 'a', 'b'))))
 *    ('bcsel@32(is_only_used_as_float)', ('feq', 'a@32', 'b@32'), 'c', 'd') => ('fcsel', ('seq', 'a', 'b'), 'c', 'd')
 *    ('bcsel@32(is_only_used_as_float)', ('fneu', 'a@32', 'b@32'), 'c', 'd') => ('fcsel', ('sne', 'a', 'b'), 'c', 'd')
 *    ('bcsel@32(is_only_used_as_float)', ('flt', 'a@32', 'b@32'), 'c', 'd') => ('fcsel', ('slt', 'a', 'b'), 'c', 'd')
 *    ('bcsel@32(is_only_used_as_float)', ('fge', 'a@32', 'b@32'), 'c', 'd') => ('fcsel', ('sge', 'a', 'b'), 'c', 'd')
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
      false,
      false,
      false,
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
      false,
      false,
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
      false,
      false,
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
      false,
      false,
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
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
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 3,
      { 30, 32 },
      -1,
   } },

   /* ('bcsel@32(is_only_used_as_float)', ('feq', 'a@32', 'b@32'), 'c', 'd') => ('fcsel', ('seq', 'a', 'b'), 'c', 'd') */
   /* search9_0_0 -> 0 in the cache */
   /* search9_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_feq,
      0, 1,
      { 0, 1 },
      -1,
   } },
   /* search9_1 -> 3 in the cache */
   /* search9_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 1,
      { 34, 3, 4 },
      0,
   } },

   /* replace9_0_0 -> 0 in the cache */
   /* replace9_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_seq,
      0, 1,
      { 0, 1 },
      -1,
   } },
   /* replace9_1 -> 3 in the cache */
   /* replace9_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel,
      -1, 1,
      { 36, 3, 4 },
      -1,
   } },

   /* ('bcsel@32(is_only_used_as_float)', ('fneu', 'a@32', 'b@32'), 'c', 'd') => ('fcsel', ('sne', 'a', 'b'), 'c', 'd') */
   /* search10_0_0 -> 0 in the cache */
   /* search10_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneu,
      0, 1,
      { 0, 1 },
      -1,
   } },
   /* search10_1 -> 3 in the cache */
   /* search10_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 1,
      { 38, 3, 4 },
      0,
   } },

   /* replace10_0_0 -> 0 in the cache */
   /* replace10_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_sne,
      0, 1,
      { 0, 1 },
      -1,
   } },
   /* replace10_1 -> 3 in the cache */
   /* replace10_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel,
      -1, 1,
      { 40, 3, 4 },
      -1,
   } },

   /* ('bcsel@32(is_only_used_as_float)', ('flt', 'a@32', 'b@32'), 'c', 'd') => ('fcsel', ('slt', 'a', 'b'), 'c', 'd') */
   /* search11_0_0 -> 0 in the cache */
   /* search11_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_flt,
      -1, 0,
      { 0, 1 },
      -1,
   } },
   /* search11_1 -> 3 in the cache */
   /* search11_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 0,
      { 42, 3, 4 },
      0,
   } },

   /* replace11_0_0 -> 0 in the cache */
   /* replace11_0_1 -> 1 in the cache */
   /* replace11_0 -> 22 in the cache */
   /* replace11_1 -> 3 in the cache */
   /* replace11_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel,
      -1, 0,
      { 22, 3, 4 },
      -1,
   } },

   /* ('bcsel@32(is_only_used_as_float)', ('fge', 'a@32', 'b@32'), 'c', 'd') => ('fcsel', ('sge', 'a', 'b'), 'c', 'd') */
   /* search12_0_0 -> 0 in the cache */
   /* search12_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fge,
      -1, 0,
      { 0, 1 },
      -1,
   } },
   /* search12_1 -> 3 in the cache */
   /* search12_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 0,
      { 45, 3, 4 },
      0,
   } },

   /* replace12_0_0 -> 0 in the cache */
   /* replace12_0_1 -> 1 in the cache */
   /* replace12_0 -> 29 in the cache */
   /* replace12_1 -> 3 in the cache */
   /* replace12_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel,
      -1, 0,
      { 29, 3, 4 },
      -1,
   } },

};

static const nir_search_expression_cond r300_nir_lower_bool_to_float_expression_cond[] = {
   is_only_used_as_float,
};


static const struct transform r300_nir_lower_bool_to_float_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 5, 11, 1 },
   { 35, 37, 2 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 13, 19, 1 },
   { 39, 41, 2 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 21, 26, 1 },
   { 43, 44, 2 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 28, 33, 1 },
   { 46, 47, 2 },
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
   4,
   7,
   10,
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
r300_nir_lower_bool_to_float(
   nir_shader *shader
) {
   bool progress = false;
   bool condition_flags[3];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(48 == ARRAY_SIZE(r300_nir_lower_bool_to_float_values));
   condition_flags[0] = true;
   condition_flags[1] = !options->has_fused_comp_and_csel;
   condition_flags[2] = options->has_fused_comp_and_csel;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_lower_bool_to_float_table);
   }

   return progress;
}

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 8
 * transforms:
 *    ('bcsel@32(r300_is_only_used_as_float)', ('feq', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 'c', 'd')
 *    ('bcsel@32(r300_is_only_used_as_float)', ('fneu', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 'd', 'c')
 *    ('bcsel@32(r300_is_only_used_as_float)', ('flt', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 'd', 'c')
 *    ('bcsel@32(r300_is_only_used_as_float)', ('fge', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 'c', 'd')
 *    ('b2f32', ('feq', 'a@32', 'b@32')) => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 1.0, 0.0)
 *    ('b2f32', ('fneu', 'a@32', 'b@32')) => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 0.0, 1.0)
 *    ('b2f32', ('flt', 'a@32', 'b@32')) => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 0.0, 1.0)
 *    ('b2f32', ('fge', 'a@32', 'b@32')) => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 1.0, 0.0)
 */


static const nir_search_value_union r300_nir_lower_bool_to_float_fs_values[] = {
   /* ('bcsel@32(r300_is_only_used_as_float)', ('feq', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 'c', 'd') */
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
      false,
      false,
      false,
      false,
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
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 1,
      { 2, 3, 4 },
      0,
   } },

   /* replace13_0_0_0_0 -> 0 in the cache */
   /* replace13_0_0_0_1_0 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 0, 6 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fabs,
      -1, 1,
      { 7 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 1,
      { 8 },
      -1,
   } },
   /* replace13_1 -> 3 in the cache */
   /* replace13_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 9, 3, 4 },
      -1,
   } },

   /* ('bcsel@32(r300_is_only_used_as_float)', ('fneu', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 'd', 'c') */
   /* search14_0_0 -> 0 in the cache */
   /* search14_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      true,
      false,
      false,
      false,
      false,
      nir_op_fneu,
      0, 1,
      { 0, 1 },
      -1,
   } },
   /* search14_1 -> 3 in the cache */
   /* search14_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 1,
      { 11, 3, 4 },
      0,
   } },

   /* replace14_0_0_0_0 -> 0 in the cache */
   /* replace14_0_0_0_1_0 -> 1 in the cache */
   /* replace14_0_0_0_1 -> 6 in the cache */
   /* replace14_0_0_0 -> 7 in the cache */
   /* replace14_0_0 -> 8 in the cache */
   /* replace14_0 -> 9 in the cache */
   /* replace14_1 -> 4 in the cache */
   /* replace14_2 -> 3 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 9, 4, 3 },
      -1,
   } },

   /* ('bcsel@32(r300_is_only_used_as_float)', ('flt', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 'd', 'c') */
   /* search15_0_0 -> 0 in the cache */
   /* search15_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      true,
      false,
      false,
      false,
      false,
      nir_op_flt,
      -1, 0,
      { 0, 1 },
      -1,
   } },
   /* search15_1 -> 3 in the cache */
   /* search15_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 0,
      { 14, 3, 4 },
      0,
   } },

   /* replace15_0_0 -> 0 in the cache */
   /* replace15_0_1_0 -> 1 in the cache */
   /* replace15_0_1 -> 6 in the cache */
   /* replace15_0 -> 7 in the cache */
   /* replace15_1 -> 4 in the cache */
   /* replace15_2 -> 3 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 7, 4, 3 },
      -1,
   } },

   /* ('bcsel@32(r300_is_only_used_as_float)', ('fge', 'a@32', 'b@32', 'ignore_exact'), 'c', 'd') => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 'c', 'd') */
   /* search16_0_0 -> 0 in the cache */
   /* search16_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      true,
      false,
      false,
      false,
      false,
      nir_op_fge,
      -1, 0,
      { 0, 1 },
      -1,
   } },
   /* search16_1 -> 3 in the cache */
   /* search16_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 0,
      { 17, 3, 4 },
      0,
   } },

   /* replace16_0_0 -> 0 in the cache */
   /* replace16_0_1_0 -> 1 in the cache */
   /* replace16_0_1 -> 6 in the cache */
   /* replace16_0 -> 7 in the cache */
   /* replace16_1 -> 3 in the cache */
   /* replace16_2 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 7, 3, 4 },
      -1,
   } },

   /* ('b2f32', ('feq', 'a@32', 'b@32')) => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 1.0, 0.0) */
   /* search17_0_0 -> 0 in the cache */
   /* search17_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_feq,
      0, 1,
      { 0, 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_b2f32,
      -1, 1,
      { 20 },
      -1,
   } },

   /* replace17_0_0_0_0 -> 0 in the cache */
   /* replace17_0_0_0_1_0 -> 1 in the cache */
   /* replace17_0_0_0_1 -> 6 in the cache */
   /* replace17_0_0_0 -> 7 in the cache */
   /* replace17_0_0 -> 8 in the cache */
   /* replace17_0 -> 9 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_float, { 0x3ff0000000000000ull /* 1.0 */ },
   } },
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_float, { 0x0ull /* 0.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 9, 22, 23 },
      -1,
   } },

   /* ('b2f32', ('fneu', 'a@32', 'b@32')) => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 0.0, 1.0) */
   /* search18_0_0 -> 0 in the cache */
   /* search18_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneu,
      0, 1,
      { 0, 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_b2f32,
      -1, 1,
      { 25 },
      -1,
   } },

   /* replace18_0_0_0_0 -> 0 in the cache */
   /* replace18_0_0_0_1_0 -> 1 in the cache */
   /* replace18_0_0_0_1 -> 6 in the cache */
   /* replace18_0_0_0 -> 7 in the cache */
   /* replace18_0_0 -> 8 in the cache */
   /* replace18_0 -> 9 in the cache */
   /* replace18_1 -> 23 in the cache */
   /* replace18_2 -> 22 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 9, 23, 22 },
      -1,
   } },

   /* ('b2f32', ('flt', 'a@32', 'b@32')) => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 0.0, 1.0) */
   /* search19_0_0 -> 0 in the cache */
   /* search19_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_flt,
      -1, 0,
      { 0, 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_b2f32,
      -1, 0,
      { 28 },
      -1,
   } },

   /* replace19_0_0 -> 0 in the cache */
   /* replace19_0_1_0 -> 1 in the cache */
   /* replace19_0_1 -> 6 in the cache */
   /* replace19_0 -> 7 in the cache */
   /* replace19_1 -> 23 in the cache */
   /* replace19_2 -> 22 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 7, 23, 22 },
      -1,
   } },

   /* ('b2f32', ('fge', 'a@32', 'b@32')) => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 1.0, 0.0) */
   /* search20_0_0 -> 0 in the cache */
   /* search20_0_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fge,
      -1, 0,
      { 0, 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_b2f32,
      -1, 0,
      { 31 },
      -1,
   } },

   /* replace20_0_0 -> 0 in the cache */
   /* replace20_0_1_0 -> 1 in the cache */
   /* replace20_0_1 -> 6 in the cache */
   /* replace20_0 -> 7 in the cache */
   /* replace20_1 -> 22 in the cache */
   /* replace20_2 -> 23 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 7, 22, 23 },
      -1,
   } },

};

static const nir_search_expression_cond r300_nir_lower_bool_to_float_fs_expression_cond[] = {
   r300_is_only_used_as_float,
};


static const struct transform r300_nir_lower_bool_to_float_fs_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 5, 10, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 12, 13, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 15, 16, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 18, 19, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 21, 24, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 26, 27, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 29, 30, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 32, 33, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_nir_lower_bool_to_float_fs_pass_op_table[nir_num_search_ops] = {
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
   [nir_search_op_b2f] = {
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
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 5,
      .table = (const uint16_t []) {
      
         0,
         10,
         11,
         12,
         13,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_nir_lower_bool_to_float_fs_transform_offsets[] = {
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
   9,
   11,
   13,
   15,
};

static const nir_algebraic_table r300_nir_lower_bool_to_float_fs_table = {
   .transforms = r300_nir_lower_bool_to_float_fs_transforms,
   .transform_offsets = r300_nir_lower_bool_to_float_fs_transform_offsets,
   .pass_op_table = r300_nir_lower_bool_to_float_fs_pass_op_table,
   .values = r300_nir_lower_bool_to_float_fs_values,
   .expression_cond = r300_nir_lower_bool_to_float_fs_expression_cond,
   .variable_cond = NULL,
};

bool
r300_nir_lower_bool_to_float_fs(
   nir_shader *shader
) {
   bool progress = false;
   bool condition_flags[3];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(34 == ARRAY_SIZE(r300_nir_lower_bool_to_float_fs_values));
   condition_flags[0] = true;
   condition_flags[1] = !options->has_fused_comp_and_csel;
   condition_flags[2] = options->has_fused_comp_and_csel;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_lower_bool_to_float_fs_table);
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
      false,
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
      nir_type_float, { 0x3ff0000000000000ull /* 1.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 1, 2 },
      -1,
   } },

   /* replace21_0 -> 2 in the cache */
   /* replace21_1_0 -> 0 in the cache */
   /* replace21_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 2, 1 },
      -1,
   } },

   /* ('fadd', 'a', -1.0) => ('fneg', ('fadd', 1.0, ('fneg', 'a'))) */
   /* search22_0 -> 0 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0xbff0000000000000ull /* -1.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 0, 5 },
      -1,
   } },

   /* replace22_0_0 -> 2 in the cache */
   /* replace22_0_1_0 -> 0 in the cache */
   /* replace22_0_1 -> 1 in the cache */
   /* replace22_0 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 1,
      { 4 },
      -1,
   } },

   /* ('fadd', -1.0, 'a') => ('fneg', ('fadd', 1.0, ('fneg', 'a'))) */
   /* search23_0 -> 5 in the cache */
   /* search23_1 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 5, 0 },
      -1,
   } },

   /* replace23_0_0 -> 2 in the cache */
   /* replace23_0_1_0 -> 0 in the cache */
   /* replace23_0_1 -> 1 in the cache */
   /* replace23_0 -> 4 in the cache */
   /* replace23 -> 7 in the cache */

   /* ('ffma', 2.0, ('fneg', 'a'), 1.0) => ('ffma', ('fneg', 'a'), 2.0, 1.0) */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x4000000000000000ull /* 2.0 */ },
   } },
   /* search24_1_0 -> 0 in the cache */
   /* search24_1 -> 1 in the cache */
   /* search24_2 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 9, 1, 2 },
      -1,
   } },

   /* replace24_0_0 -> 0 in the cache */
   /* replace24_0 -> 1 in the cache */
   /* replace24_1 -> 9 in the cache */
   /* replace24_2 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 1, 9, 2 },
      -1,
   } },

   /* ('ffma', 'a', -2.0, 1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0)) */
   /* search25_0 -> 0 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0xc000000000000000ull /* -2.0 */ },
   } },
   /* search25_2 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 0, 12, 2 },
      -1,
   } },

   /* replace25_0_0_0 -> 0 in the cache */
   /* replace25_0_0 -> 1 in the cache */
   /* replace25_0_1 -> 9 in the cache */
   /* replace25_0_2 -> 2 in the cache */
   /* replace25_0 -> 11 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 1,
      { 11 },
      -1,
   } },

   /* ('ffma', -2.0, 'a', 1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0)) */
   /* search26_0 -> 12 in the cache */
   /* search26_1 -> 0 in the cache */
   /* search26_2 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 12, 0, 2 },
      -1,
   } },

   /* replace26_0_0_0 -> 0 in the cache */
   /* replace26_0_0 -> 1 in the cache */
   /* replace26_0_1 -> 9 in the cache */
   /* replace26_0_2 -> 2 in the cache */
   /* replace26_0 -> 11 in the cache */
   /* replace26 -> 14 in the cache */

   /* ('ffma', 2.0, 'a', -1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0)) */
   /* search27_0 -> 9 in the cache */
   /* search27_1 -> 0 in the cache */
   /* search27_2 -> 5 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 9, 0, 5 },
      -1,
   } },

   /* replace27_0_0_0 -> 0 in the cache */
   /* replace27_0_0 -> 1 in the cache */
   /* replace27_0_1 -> 9 in the cache */
   /* replace27_0_2 -> 2 in the cache */
   /* replace27_0 -> 11 in the cache */
   /* replace27 -> 14 in the cache */

   /* ('ffma', 'a', 2.0, -1.0) => ('fneg', ('ffma', ('fneg', 'a'), 2.0, 1.0)) */
   /* search28_0 -> 0 in the cache */
   /* search28_1 -> 9 in the cache */
   /* search28_2 -> 5 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ffma,
      0, 1,
      { 0, 9, 5 },
      -1,
   } },

   /* replace28_0_0_0 -> 0 in the cache */
   /* replace28_0_0 -> 1 in the cache */
   /* replace28_0_1 -> 9 in the cache */
   /* replace28_0_2 -> 2 in the cache */
   /* replace28_0 -> 11 in the cache */
   /* replace28 -> 14 in the cache */

   /* ('fmul', 'a(is_ubo_or_input)', 2.0) => ('fadd', 'a', 'a') */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      0,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   /* search29_1 -> 9 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 18, 9 },
      -1,
   } },

   /* replace29_0 -> 0 in the cache */
   /* replace29_1 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
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
      nir_type_float, { 0x4000000000000000ull /* 2.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
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
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 24 },
      -1,
   } },

   /* replace30_0 -> 23 in the cache */
   /* replace30_1_0 -> 21 in the cache */
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
      false,
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
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 23, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 4.0)) => ('fmul', 4.0, ('fmul', 'a', 'b')) */
   /* search31_0 -> 21 in the cache */
   /* search31_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x4010000000000000ull /* 4.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
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
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 30 },
      -1,
   } },

   /* replace31_0 -> 29 in the cache */
   /* replace31_1_0 -> 21 in the cache */
   /* replace31_1_1 -> 26 in the cache */
   /* replace31_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 29, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 8.0)) => ('fmul', 8.0, ('fmul', 'a', 'b')) */
   /* search32_0 -> 21 in the cache */
   /* search32_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x4020000000000000ull /* 8.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
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
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 34 },
      -1,
   } },

   /* replace32_0 -> 33 in the cache */
   /* replace32_1_0 -> 21 in the cache */
   /* replace32_1_1 -> 26 in the cache */
   /* replace32_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 33, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 16.0)) => ('fmul', 16.0, ('fmul', 'a', 'b')) */
   /* search33_0 -> 21 in the cache */
   /* search33_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x4030000000000000ull /* 16.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
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
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 38 },
      -1,
   } },

   /* replace33_0 -> 37 in the cache */
   /* replace33_1_0 -> 21 in the cache */
   /* replace33_1_1 -> 26 in the cache */
   /* replace33_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 37, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.5)) => ('fmul', 0.5, ('fmul', 'a', 'b')) */
   /* search34_0 -> 21 in the cache */
   /* search34_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x3fe0000000000000ull /* 0.5 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
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
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 42 },
      -1,
   } },

   /* replace34_0 -> 41 in the cache */
   /* replace34_1_0 -> 21 in the cache */
   /* replace34_1_1 -> 26 in the cache */
   /* replace34_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 41, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.25)) => ('fmul', 0.25, ('fmul', 'a', 'b')) */
   /* search35_0 -> 21 in the cache */
   /* search35_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x3fd0000000000000ull /* 0.25 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
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
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 46 },
      -1,
   } },

   /* replace35_0 -> 45 in the cache */
   /* replace35_1_0 -> 21 in the cache */
   /* replace35_1_1 -> 26 in the cache */
   /* replace35_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 45, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.125)) => ('fmul', 0.125, ('fmul', 'a', 'b')) */
   /* search36_0 -> 21 in the cache */
   /* search36_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x3fc0000000000000ull /* 0.125 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
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
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 50 },
      -1,
   } },

   /* replace36_0 -> 49 in the cache */
   /* replace36_1_0 -> 21 in the cache */
   /* replace36_1_1 -> 26 in the cache */
   /* replace36_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 49, 27 },
      -1,
   } },

   /* ('fmul', 'a', ('fmul(is_used_once)', 'b(is_ubo_or_input)', 0.0625)) => ('fmul', 0.0625, ('fmul', 'a', 'b')) */
   /* search37_0 -> 21 in the cache */
   /* search37_1_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_float, { 0x3fb0000000000000ull /* 0.0625 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
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
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 2,
      { 21, 54 },
      -1,
   } },

   /* replace37_0 -> 53 in the cache */
   /* replace37_1_0 -> 21 in the cache */
   /* replace37_1_1 -> 26 in the cache */
   /* replace37_1 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      false,
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
   is_used_once,
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
r300_nir_prepare_presubtract(
   nir_shader *shader
) {
   bool progress = false;
   bool condition_flags[3];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(57 == ARRAY_SIZE(r300_nir_prepare_presubtract_values));
   condition_flags[0] = true;
   condition_flags[1] = !options->has_fused_comp_and_csel;
   condition_flags[2] = options->has_fused_comp_and_csel;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_prepare_presubtract_table);
   }

   return progress;
}

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 5
 * transforms:
 *    ('fneg', ('fneg', 'a')) => a
 *    ('fabs', ('fneg', 'a')) => ('fabs', 'a')
 *    ('fadd', 'a', 0.0) => a
 *    ('fadd', 'a', ('fneg', 0.0)) => a
 *    ('fcsel_ge(is_only_used_by_terminate_if)', 'a', 0.0, 1.0) => ('fneg', 'a')
 */


static const nir_search_value_union r300_nir_opt_algebraic_late_values[] = {
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
      false,
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
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 1 },
      -1,
   } },

   /* replace38 -> 0 in the cache */

   /* ('fabs', ('fneg', 'a')) => ('fabs', 'a') */
   /* search39_0_0 -> 0 in the cache */
   /* search39_0 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fabs,
      -1, 0,
      { 1 },
      -1,
   } },

   /* replace39_0 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fabs,
      -1, 0,
      { 0 },
      -1,
   } },

   /* ('fadd', 'a', 0.0) => a */
   /* search40_0 -> 0 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x0ull /* 0.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 0, 5 },
      -1,
   } },

   /* replace40 -> 0 in the cache */

   /* ('fadd', 'a', ('fneg', 0.0)) => a */
   /* search41_0 -> 0 in the cache */
   /* search41_1_0 -> 5 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 5 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 0, 7 },
      -1,
   } },

   /* replace41 -> 0 in the cache */

   /* ('fcsel_ge(is_only_used_by_terminate_if)', 'a', 0.0, 1.0) => ('fneg', 'a') */
   { .variable = {
      { nir_search_value_variable, 32 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_float, { 0x0ull /* 0.0 */ },
   } },
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_float, { 0x3ff0000000000000ull /* 1.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 0,
      { 9, 10, 11 },
      0,
   } },

   /* replace42_0 -> 9 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 9 },
      -1,
   } },

};

static const nir_search_expression_cond r300_nir_opt_algebraic_late_expression_cond[] = {
   is_only_used_by_terminate_if,
};


static const struct transform r300_nir_opt_algebraic_late_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 6, 0, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 12, 13, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 2, 0, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 3, 4, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 8, 0, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 6, 0, 0 },
   { 8, 0, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_nir_opt_algebraic_late_pass_op_table[nir_num_search_ops] = {
   [nir_op_fneg] = {
      .filter = (const uint16_t []) {
         0,
         1,
         2,
         2,
         0,
         0,
         2,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         2,
         3,
         6,
      },
   },
   [nir_op_fabs] = {
      .filter = (const uint16_t []) {
         0,
         0,
         1,
         1,
         0,
         0,
         1,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         7,
      },
   },
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
         4,
         8,
         4,
         4,
         9,
         8,
         9,
         8,
      },
   },
   [nir_op_fcsel_ge] = {
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
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         0,
         0,
         5,
         0,
         0,
         0,
         5,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_nir_opt_algebraic_late_transform_offsets[] = {
   0,
   0,
   0,
   0,
   1,
   3,
   5,
   7,
   9,
   11,
};

static const nir_algebraic_table r300_nir_opt_algebraic_late_table = {
   .transforms = r300_nir_opt_algebraic_late_transforms,
   .transform_offsets = r300_nir_opt_algebraic_late_transform_offsets,
   .pass_op_table = r300_nir_opt_algebraic_late_pass_op_table,
   .values = r300_nir_opt_algebraic_late_values,
   .expression_cond = r300_nir_opt_algebraic_late_expression_cond,
   .variable_cond = NULL,
};

bool
r300_nir_opt_algebraic_late(
   nir_shader *shader
) {
   bool progress = false;
   bool condition_flags[3];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(14 == ARRAY_SIZE(r300_nir_opt_algebraic_late_values));
   condition_flags[0] = true;
   condition_flags[1] = !options->has_fused_comp_and_csel;
   condition_flags[2] = options->has_fused_comp_and_csel;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_opt_algebraic_late_table);
   }

   return progress;
}

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 3
 * transforms:
 *    ('fadd(is_only_used_by_load_ubo_vec4)', 'a', ('fneg', ('ffract', 'a'))) => a
 *    ('fround_even(is_only_used_by_load_ubo_vec4)', ('fadd', 'a', ('fneg', ('ffract', 'a')))) => a
 *    ('ftrunc', 'a@32') => ('fcsel_ge', 'a', ('fadd', ('fabs', 'a'), ('fneg', ('ffract', ('fabs', 'a')))), ('fneg', ('fadd', ('fabs', 'a'), ('fneg', ('ffract', ('fabs', 'a'))))))
 */


static const nir_search_value_union r300_nir_post_integer_lowering_values[] = {
   /* ('fadd(is_only_used_by_load_ubo_vec4)', 'a', ('fneg', ('ffract', 'a'))) => a */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   /* search43_1_0_0 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ffract,
      -1, 0,
      { 0 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 0, 2 },
      0,
   } },

   /* replace43 -> 0 in the cache */

   /* ('fround_even(is_only_used_by_load_ubo_vec4)', ('fadd', 'a', ('fneg', ('ffract', 'a')))) => a */
   /* search44_0_0 -> 0 in the cache */
   /* search44_0_1_0_0 -> 0 in the cache */
   /* search44_0_1_0 -> 1 in the cache */
   /* search44_0_1 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 0, 2 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fround_even,
      -1, 1,
      { 4 },
      0,
   } },

   /* replace44 -> 0 in the cache */

   /* ('ftrunc', 'a@32') => ('fcsel_ge', 'a', ('fadd', ('fabs', 'a'), ('fneg', ('ffract', ('fabs', 'a')))), ('fneg', ('fadd', ('fabs', 'a'), ('fneg', ('ffract', ('fabs', 'a')))))) */
   { .variable = {
      { nir_search_value_variable, 32 },
      0, /* a */
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
      false,
      false,
      false,
      false,
      nir_op_ftrunc,
      -1, 0,
      { 6 },
      -1,
   } },

   /* replace45_0 -> 6 in the cache */
   /* replace45_1_0_0 -> 6 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fabs,
      -1, 0,
      { 6 },
      -1,
   } },
   /* replace45_1_1_0_0_0 -> 6 in the cache */
   /* replace45_1_1_0_0 -> 8 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ffract,
      -1, 0,
      { 8 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 9 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 8, 10 },
      -1,
   } },
   /* replace45_2_0_0_0 -> 6 in the cache */
   /* replace45_2_0_0 -> 8 in the cache */
   /* replace45_2_0_1_0_0_0 -> 6 in the cache */
   /* replace45_2_0_1_0_0 -> 8 in the cache */
   /* replace45_2_0_1_0 -> 9 in the cache */
   /* replace45_2_0_1 -> 10 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      1, 1,
      { 8, 10 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 1,
      { 12 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 2,
      { 6, 11, 13 },
      -1,
   } },

};

static const nir_search_expression_cond r300_nir_post_integer_lowering_expression_cond[] = {
   is_only_used_by_load_ubo_vec4,
};


static const struct transform r300_nir_post_integer_lowering_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 7, 14, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 3, 0, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 5, 0, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_nir_post_integer_lowering_pass_op_table[nir_num_search_ops] = {
   [nir_op_fadd] = {
      .filter = (const uint16_t []) {
         0,
         0,
         0,
         0,
         1,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         5,
         5,
         5,
      },
   },
   [nir_op_fneg] = {
      .filter = (const uint16_t []) {
         0,
         0,
         1,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         4,
      },
   },
   [nir_op_ffract] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         2,
      },
   },
   [nir_op_fround_even] = {
      .filter = (const uint16_t []) {
         0,
         0,
         0,
         0,
         0,
         1,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         6,
      },
   },
   [nir_op_ftrunc] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         3,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_nir_post_integer_lowering_transform_offsets[] = {
   0,
   0,
   0,
   1,
   0,
   3,
   5,
};

static const nir_algebraic_table r300_nir_post_integer_lowering_table = {
   .transforms = r300_nir_post_integer_lowering_transforms,
   .transform_offsets = r300_nir_post_integer_lowering_transform_offsets,
   .pass_op_table = r300_nir_post_integer_lowering_pass_op_table,
   .values = r300_nir_post_integer_lowering_values,
   .expression_cond = r300_nir_post_integer_lowering_expression_cond,
   .variable_cond = NULL,
};

bool
r300_nir_post_integer_lowering(
   nir_shader *shader
) {
   bool progress = false;
   bool condition_flags[3];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(15 == ARRAY_SIZE(r300_nir_post_integer_lowering_values));
   condition_flags[0] = true;
   condition_flags[1] = !options->has_fused_comp_and_csel;
   condition_flags[2] = options->has_fused_comp_and_csel;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_post_integer_lowering_table);
   }

   return progress;
}

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 1
 * transforms:
 *    ('flrp', 'a', 'b', 'c') => ('ffma', 'b', 'c', ('ffma', ('fneg', 'a'), 'c', 'a'))
 */


static const nir_search_value_union r300_nir_lower_flrp_values[] = {
   /* ('flrp', 'a', 'b', 'c') => ('ffma', 'b', 'c', ('ffma', ('fneg', 'a'), 'c', 'a')) */
   { .variable = {
      { nir_search_value_variable, -3 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, -3 },
      1, /* b */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, -3 },
      2, /* c */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_flrp,
      -1, 0,
      { 0, 1, 2 },
      -1,
   } },

   /* replace46_0 -> 1 in the cache */
   /* replace46_1 -> 2 in the cache */
   /* replace46_2_0_0 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 0 },
      -1,
   } },
   /* replace46_2_1 -> 2 in the cache */
   /* replace46_2_2 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ffma,
      1, 1,
      { 4, 2, 0 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ffma,
      0, 2,
      { 1, 2, 5 },
      -1,
   } },

};



static const struct transform r300_nir_lower_flrp_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 3, 6, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_nir_lower_flrp_pass_op_table[nir_num_search_ops] = {
   [nir_op_flrp] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         2,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_nir_lower_flrp_transform_offsets[] = {
   0,
   0,
   1,
};

static const nir_algebraic_table r300_nir_lower_flrp_table = {
   .transforms = r300_nir_lower_flrp_transforms,
   .transform_offsets = r300_nir_lower_flrp_transform_offsets,
   .pass_op_table = r300_nir_lower_flrp_pass_op_table,
   .values = r300_nir_lower_flrp_values,
   .expression_cond = NULL,
   .variable_cond = NULL,
};

bool
r300_nir_lower_flrp(
   nir_shader *shader
) {
   bool progress = false;
   bool condition_flags[3];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(7 == ARRAY_SIZE(r300_nir_lower_flrp_values));
   condition_flags[0] = true;
   condition_flags[1] = !options->has_fused_comp_and_csel;
   condition_flags[2] = options->has_fused_comp_and_csel;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_lower_flrp_table);
   }

   return progress;
}

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 1
 * transforms:
 *    ('fcsel_ge', 'a', 'b', 'c') => ('flrp', 'c', 'b', ('sge', 'a', 0.0))
 */


static const nir_search_value_union r300_nir_lower_fcsel_r300_values[] = {
   /* ('fcsel_ge', 'a', 'b', 'c') => ('flrp', 'c', 'b', ('sge', 'a', 0.0)) */
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
   { .variable = {
      { nir_search_value_variable, 32 },
      2, /* c */
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
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 0,
      { 0, 1, 2 },
      -1,
   } },

   /* replace47_0 -> 2 in the cache */
   /* replace47_1 -> 1 in the cache */
   /* replace47_2_0 -> 0 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_float, { 0x0ull /* 0.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_sge,
      -1, 0,
      { 0, 4 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_flrp,
      -1, 0,
      { 2, 1, 5 },
      -1,
   } },

};



static const struct transform r300_nir_lower_fcsel_r300_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 3, 6, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_nir_lower_fcsel_r300_pass_op_table[nir_num_search_ops] = {
   [nir_op_fcsel_ge] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         2,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_nir_lower_fcsel_r300_transform_offsets[] = {
   0,
   0,
   1,
};

static const nir_algebraic_table r300_nir_lower_fcsel_r300_table = {
   .transforms = r300_nir_lower_fcsel_r300_transforms,
   .transform_offsets = r300_nir_lower_fcsel_r300_transform_offsets,
   .pass_op_table = r300_nir_lower_fcsel_r300_pass_op_table,
   .values = r300_nir_lower_fcsel_r300_values,
   .expression_cond = NULL,
   .variable_cond = NULL,
};

bool
r300_nir_lower_fcsel_r300(
   nir_shader *shader
) {
   bool progress = false;
   bool condition_flags[3];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(7 == ARRAY_SIZE(r300_nir_lower_fcsel_r300_values));
   condition_flags[0] = true;
   condition_flags[1] = !options->has_fused_comp_and_csel;
   condition_flags[2] = options->has_fused_comp_and_csel;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_lower_fcsel_r300_table);
   }

   return progress;
}

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 4
 * transforms:
 *    ('seq(is_not_used_in_single_if)', 'a@32', 'b@32') => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 1.0, 0.0)
 *    ('sne(is_not_used_in_single_if)', 'a@32', 'b@32') => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 0.0, 1.0)
 *    ('slt(is_not_used_in_single_if)', 'a@32', 'b@32') => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 0.0, 1.0)
 *    ('sge(is_not_used_in_single_if)', 'a@32', 'b@32') => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 1.0, 0.0)
 */


static const nir_search_value_union r300_nir_lower_comparison_fs_values[] = {
   /* ('seq(is_not_used_in_single_if)', 'a@32', 'b@32') => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 1.0, 0.0) */
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
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_seq,
      0, 1,
      { 0, 1 },
      0,
   } },

   /* replace48_0_0_0_0 -> 0 in the cache */
   /* replace48_0_0_0_1_0 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fadd,
      0, 1,
      { 0, 3 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fabs,
      -1, 1,
      { 4 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 1,
      { 5 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_float, { 0x3ff0000000000000ull /* 1.0 */ },
   } },
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_float, { 0x0ull /* 0.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 6, 7, 8 },
      -1,
   } },

   /* ('sne(is_not_used_in_single_if)', 'a@32', 'b@32') => ('fcsel_ge', ('fneg', ('fabs', ('fadd', 'a', ('fneg', 'b')))), 0.0, 1.0) */
   /* search49_0 -> 0 in the cache */
   /* search49_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_sne,
      0, 1,
      { 0, 1 },
      0,
   } },

   /* replace49_0_0_0_0 -> 0 in the cache */
   /* replace49_0_0_0_1_0 -> 1 in the cache */
   /* replace49_0_0_0_1 -> 3 in the cache */
   /* replace49_0_0_0 -> 4 in the cache */
   /* replace49_0_0 -> 5 in the cache */
   /* replace49_0 -> 6 in the cache */
   /* replace49_1 -> 8 in the cache */
   /* replace49_2 -> 7 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 6, 8, 7 },
      -1,
   } },

   /* ('slt(is_not_used_in_single_if)', 'a@32', 'b@32') => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 0.0, 1.0) */
   /* search50_0 -> 0 in the cache */
   /* search50_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_slt,
      -1, 0,
      { 0, 1 },
      0,
   } },

   /* replace50_0_0 -> 0 in the cache */
   /* replace50_0_1_0 -> 1 in the cache */
   /* replace50_0_1 -> 3 in the cache */
   /* replace50_0 -> 4 in the cache */
   /* replace50_1 -> 8 in the cache */
   /* replace50_2 -> 7 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 4, 8, 7 },
      -1,
   } },

   /* ('sge(is_not_used_in_single_if)', 'a@32', 'b@32') => ('fcsel_ge', ('fadd', 'a', ('fneg', 'b')), 1.0, 0.0) */
   /* search51_0 -> 0 in the cache */
   /* search51_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_sge,
      -1, 0,
      { 0, 1 },
      0,
   } },

   /* replace51_0_0 -> 0 in the cache */
   /* replace51_0_1_0 -> 1 in the cache */
   /* replace51_0_1 -> 3 in the cache */
   /* replace51_0 -> 4 in the cache */
   /* replace51_1 -> 7 in the cache */
   /* replace51_2 -> 8 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_ge,
      -1, 1,
      { 4, 7, 8 },
      -1,
   } },

};

static const nir_search_expression_cond r300_nir_lower_comparison_fs_expression_cond[] = {
   is_not_used_in_single_if,
};


static const struct transform r300_nir_lower_comparison_fs_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 2, 9, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 10, 11, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 12, 13, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 14, 15, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table r300_nir_lower_comparison_fs_pass_op_table[nir_num_search_ops] = {
   [nir_op_seq] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         2,
      },
   },
   [nir_op_sne] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         3,
      },
   },
   [nir_op_slt] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         4,
      },
   },
   [nir_op_sge] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         5,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t r300_nir_lower_comparison_fs_transform_offsets[] = {
   0,
   0,
   1,
   3,
   5,
   7,
};

static const nir_algebraic_table r300_nir_lower_comparison_fs_table = {
   .transforms = r300_nir_lower_comparison_fs_transforms,
   .transform_offsets = r300_nir_lower_comparison_fs_transform_offsets,
   .pass_op_table = r300_nir_lower_comparison_fs_pass_op_table,
   .values = r300_nir_lower_comparison_fs_values,
   .expression_cond = r300_nir_lower_comparison_fs_expression_cond,
   .variable_cond = NULL,
};

bool
r300_nir_lower_comparison_fs(
   nir_shader *shader
) {
   bool progress = false;
   bool condition_flags[3];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(16 == ARRAY_SIZE(r300_nir_lower_comparison_fs_values));
   condition_flags[0] = true;
   condition_flags[1] = !options->has_fused_comp_and_csel;
   condition_flags[2] = options->has_fused_comp_and_csel;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &r300_nir_lower_comparison_fs_table);
   }

   return progress;
}
