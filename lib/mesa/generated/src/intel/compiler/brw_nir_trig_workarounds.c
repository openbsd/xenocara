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


   static const nir_search_variable search0_0 = {
   { nir_search_value_variable, -1 },
   0, /* x */
   false,
   nir_type_invalid,
   (is_not_const),
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
};
static const nir_search_expression search0 = {
   { nir_search_value_expression, -1 },
   false, false,
   -1, 0,
   nir_op_fsin,
   { &search0_0.value },
   NULL,
};

   static const nir_search_variable replace0_0_0 = {
   { nir_search_value_variable, -1 },
   0, /* x */
   false,
   nir_type_invalid,
   NULL,
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
};
static const nir_search_expression replace0_0 = {
   { nir_search_value_expression, -1 },
   false, false,
   -1, 0,
   nir_op_fsin,
   { &replace0_0_0.value },
   NULL,
};

static const nir_search_constant replace0_1 = {
   { nir_search_value_constant, -1 },
   nir_type_float, { 0x3fefffc115df6556 /* 0.99997 */ },
};
static const nir_search_expression replace0 = {
   { nir_search_value_expression, -1 },
   false, false,
   0, 1,
   nir_op_fmul,
   { &replace0_0.value, &replace0_1.value },
   NULL,
};

   /* search1_0 -> search0_0 in the cache */
static const nir_search_expression search1 = {
   { nir_search_value_expression, -1 },
   false, false,
   -1, 0,
   nir_op_fcos,
   { &search0_0.value },
   NULL,
};

   /* replace1_0_0 -> replace0_0_0 in the cache */
static const nir_search_expression replace1_0 = {
   { nir_search_value_expression, -1 },
   false, false,
   -1, 0,
   nir_op_fcos,
   { &replace0_0_0.value },
   NULL,
};

/* replace1_1 -> replace0_1 in the cache */
static const nir_search_expression replace1 = {
   { nir_search_value_expression, -1 },
   false, false,
   0, 1,
   nir_op_fmul,
   { &replace1_0.value, &replace0_1.value },
   NULL,
};


static const struct transform brw_nir_apply_trig_workarounds_state2_xforms[] = {
  { &search0, &replace0.value, 0 },
};
static const struct transform brw_nir_apply_trig_workarounds_state3_xforms[] = {
  { &search1, &replace1.value, 0 },
};

static const struct per_op_table brw_nir_apply_trig_workarounds_table[nir_num_search_ops] = {
   [nir_op_fsin] = {
      .filter = (uint16_t []) {
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 1,
      .table = (uint16_t []) {
      
         2,
      },
   },
   [nir_op_fcos] = {
      .filter = (uint16_t []) {
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 1,
      .table = (uint16_t []) {
      
         3,
      },
   },
};

const struct transform *brw_nir_apply_trig_workarounds_transforms[] = {
   NULL,
   NULL,
   brw_nir_apply_trig_workarounds_state2_xforms,
   brw_nir_apply_trig_workarounds_state3_xforms,
};

const uint16_t brw_nir_apply_trig_workarounds_transform_counts[] = {
   0,
   0,
   (uint16_t)ARRAY_SIZE(brw_nir_apply_trig_workarounds_state2_xforms),
   (uint16_t)ARRAY_SIZE(brw_nir_apply_trig_workarounds_state3_xforms),
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

   condition_flags[0] = true;

   nir_foreach_function(function, shader) {
      if (function->impl) {
         progress |= nir_algebraic_impl(function->impl, condition_flags,
                                        brw_nir_apply_trig_workarounds_transforms,
                                        brw_nir_apply_trig_workarounds_transform_counts,
                                        brw_nir_apply_trig_workarounds_table);
      }
   }

   return progress;
}

