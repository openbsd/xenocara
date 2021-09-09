#include "sfn/sfn_nir.h"

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 2
 * transforms:
 *    ('fsin', 'a@32') => ('fsin_r600', ('fadd', ('ffract', ('ffma', 'a', 0.15915494, 0.5)), -0.5))
 *    ('fcos', 'a@32') => ('fcos_r600', ('fadd', ('ffract', ('ffma', 'a', 0.15915494, 0.5)), -0.5))
 */


   static const nir_search_variable search0_0 = {
   { nir_search_value_variable, 32 },
   0, /* a */
   false,
   nir_type_invalid,
   NULL,
   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
};
static const nir_search_expression search0 = {
   { nir_search_value_expression, 32 },
   false, false,
   -1, 0,
   nir_op_fsin,
   { &search0_0.value },
   NULL,
};

   /* replace0_0_0_0_0 -> search0_0 in the cache */

static const nir_search_constant replace0_0_0_0_1 = {
   { nir_search_value_constant, 32 },
   nir_type_float, { 0x3fc45f306725feed /* 0.15915494 */ },
};

static const nir_search_constant replace0_0_0_0_2 = {
   { nir_search_value_constant, 32 },
   nir_type_float, { 0x3fe0000000000000 /* 0.5 */ },
};
static const nir_search_expression replace0_0_0_0 = {
   { nir_search_value_expression, 32 },
   false, false,
   1, 1,
   nir_op_ffma,
   { &search0_0.value, &replace0_0_0_0_1.value, &replace0_0_0_0_2.value },
   NULL,
};
static const nir_search_expression replace0_0_0 = {
   { nir_search_value_expression, 32 },
   false, false,
   -1, 1,
   nir_op_ffract,
   { &replace0_0_0_0.value },
   NULL,
};

static const nir_search_constant replace0_0_1 = {
   { nir_search_value_constant, 32 },
   nir_type_float, { 0xbfe0000000000000 /* -0.5 */ },
};
static const nir_search_expression replace0_0 = {
   { nir_search_value_expression, 32 },
   false, false,
   0, 2,
   nir_op_fadd,
   { &replace0_0_0.value, &replace0_0_1.value },
   NULL,
};
static const nir_search_expression replace0 = {
   { nir_search_value_expression, 32 },
   false, false,
   -1, 2,
   nir_op_fsin_r600,
   { &replace0_0.value },
   NULL,
};

   /* search1_0 -> search0_0 in the cache */
static const nir_search_expression search1 = {
   { nir_search_value_expression, 32 },
   false, false,
   -1, 0,
   nir_op_fcos,
   { &search0_0.value },
   NULL,
};

   /* replace1_0_0_0_0 -> search0_0 in the cache */

/* replace1_0_0_0_1 -> replace0_0_0_0_1 in the cache */

/* replace1_0_0_0_2 -> replace0_0_0_0_2 in the cache */
/* replace1_0_0_0 -> replace0_0_0_0 in the cache */
/* replace1_0_0 -> replace0_0_0 in the cache */

/* replace1_0_1 -> replace0_0_1 in the cache */
/* replace1_0 -> replace0_0 in the cache */
static const nir_search_expression replace1 = {
   { nir_search_value_expression, 32 },
   false, false,
   -1, 2,
   nir_op_fcos_r600,
   { &replace0_0.value },
   NULL,
};


static const struct transform r600_lower_alu_state2_xforms[] = {
  { &search0, &replace0.value, 0 },
};
static const struct transform r600_lower_alu_state3_xforms[] = {
  { &search1, &replace1.value, 0 },
};

static const struct per_op_table r600_lower_alu_table[nir_num_search_ops] = {
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

const struct transform *r600_lower_alu_transforms[] = {
   NULL,
   NULL,
   r600_lower_alu_state2_xforms,
   r600_lower_alu_state3_xforms,
};

const uint16_t r600_lower_alu_transform_counts[] = {
   0,
   0,
   (uint16_t)ARRAY_SIZE(r600_lower_alu_state2_xforms),
   (uint16_t)ARRAY_SIZE(r600_lower_alu_state3_xforms),
};

bool
r600_lower_alu(nir_shader *shader)
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
                                        r600_lower_alu_transforms,
                                        r600_lower_alu_transform_counts,
                                        r600_lower_alu_table);
      }
   }

   return progress;
}

