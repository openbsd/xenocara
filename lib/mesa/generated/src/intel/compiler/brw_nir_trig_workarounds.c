#include "brw_nir.h"

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 2
 * transforms:
 *    ('fsin', 'x(is_not_const)') => ('fmul', ('fsin', 'x'), 0.99997)
 *    ('fcos', 'x(is_not_const)') => ('fmul', ('fcos', 'x'), 0.99997)
 */


static const nir_search_value_union brw_nir_apply_trig_workarounds_values[] = {
   /* ('fsin', 'x(is_not_const)') => ('fmul', ('fsin', 'x'), 0.99997) */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* x */
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
      nir_op_fsin,
      -1, 0,
      { 0 },
      -1,
   } },

   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* x */
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
      { 2 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_float, { 0x3fefffc115df6556 /* 0.99997 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 3, 4 },
      -1,
   } },

   /* ('fcos', 'x(is_not_const)') => ('fmul', ('fcos', 'x'), 0.99997) */
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

   /* replace1_0_0 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fcos,
      -1, 0,
      { 2 },
      -1,
   } },
   /* replace1_1 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 7, 4 },
      -1,
   } },

};


static const nir_search_variable_cond brw_nir_apply_trig_workarounds_variable_cond[] = {
   (is_not_const),
};

static const struct transform brw_nir_apply_trig_workarounds_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 1, 5, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 6, 8, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table brw_nir_apply_trig_workarounds_pass_op_table[nir_num_search_ops] = {
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
static const uint16_t brw_nir_apply_trig_workarounds_transform_offsets[] = {
   0,
   0,
   1,
   3,
};

static const nir_algebraic_table brw_nir_apply_trig_workarounds_table = {
   .transforms = brw_nir_apply_trig_workarounds_transforms,
   .transform_offsets = brw_nir_apply_trig_workarounds_transform_offsets,
   .pass_op_table = brw_nir_apply_trig_workarounds_pass_op_table,
   .values = brw_nir_apply_trig_workarounds_values,
   .expression_cond = NULL,
   .variable_cond = brw_nir_apply_trig_workarounds_variable_cond,
};

bool
brw_nir_apply_trig_workarounds(nir_shader *shader)
{
   bool progress = false;
   bool condition_flags[1];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(9 == ARRAY_SIZE(brw_nir_apply_trig_workarounds_values));
   condition_flags[0] = true;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &brw_nir_apply_trig_workarounds_table);
   }

   return progress;
}


#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 2
 * transforms:
 *    ('fsin', 'x(is_not_const)') => ('fsin', ('fmod', 'x', 6.283185307179586))
 *    ('fcos', 'x(is_not_const)') => ('fcos', ('fmod', 'x', 6.283185307179586))
 */


static const nir_search_value_union brw_nir_limit_trig_input_range_workaround_values[] = {
   /* ('fsin', 'x(is_not_const)') => ('fsin', ('fmod', 'x', 6.283185307179586)) */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* x */
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
      nir_op_fsin,
      -1, 0,
      { 0 },
      -1,
   } },

   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* x */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
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
      nir_op_fmod,
      -1, 0,
      { 2, 3 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fsin,
      -1, 0,
      { 4 },
      -1,
   } },

   /* ('fcos', 'x(is_not_const)') => ('fcos', ('fmod', 'x', 6.283185307179586)) */
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

   /* replace3_0_0 -> 2 in the cache */
   /* replace3_0_1 -> 3 in the cache */
   /* replace3_0 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_fcos,
      -1, 0,
      { 4 },
      -1,
   } },

};


static const nir_search_variable_cond brw_nir_limit_trig_input_range_workaround_variable_cond[] = {
   (is_not_const),
};

static const struct transform brw_nir_limit_trig_input_range_workaround_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 1, 5, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 6, 7, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table brw_nir_limit_trig_input_range_workaround_pass_op_table[nir_num_search_ops] = {
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
static const uint16_t brw_nir_limit_trig_input_range_workaround_transform_offsets[] = {
   0,
   0,
   1,
   3,
};

static const nir_algebraic_table brw_nir_limit_trig_input_range_workaround_table = {
   .transforms = brw_nir_limit_trig_input_range_workaround_transforms,
   .transform_offsets = brw_nir_limit_trig_input_range_workaround_transform_offsets,
   .pass_op_table = brw_nir_limit_trig_input_range_workaround_pass_op_table,
   .values = brw_nir_limit_trig_input_range_workaround_values,
   .expression_cond = NULL,
   .variable_cond = brw_nir_limit_trig_input_range_workaround_variable_cond,
};

bool
brw_nir_limit_trig_input_range_workaround(nir_shader *shader)
{
   bool progress = false;
   bool condition_flags[1];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(8 == ARRAY_SIZE(brw_nir_limit_trig_input_range_workaround_values));
   condition_flags[0] = true;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &brw_nir_limit_trig_input_range_workaround_table);
   }

   return progress;
}

