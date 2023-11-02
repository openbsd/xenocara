#include "r300_vs.h"
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

   /* This is not a great place for this, but it seems to be the best place
    * for it. Check that at most one kind of lowering is requested for
    * bitfield extract and bitfield insert. Otherwise the lowering can fight
    * with each other and optimizations.
    */
   assert((int)options->lower_bitfield_extract +
          (int)options->lower_bitfield_extract_to_shifts <= 1);
   assert((int)options->lower_bitfield_insert +
          (int)options->lower_bitfield_insert_to_shifts +
          (int)options->lower_bitfield_insert_to_bitfield_select <= 1);


   STATIC_ASSERT(14 == ARRAY_SIZE(r300_transform_vs_trig_input_values));
   condition_flags[0] = true;

   nir_foreach_function(function, shader) {
      if (function->impl) {
         progress |= nir_algebraic_impl(function->impl, condition_flags,
                                        &r300_transform_vs_trig_input_table);
      }
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

   /* This is not a great place for this, but it seems to be the best place
    * for it. Check that at most one kind of lowering is requested for
    * bitfield extract and bitfield insert. Otherwise the lowering can fight
    * with each other and optimizations.
    */
   assert((int)options->lower_bitfield_extract +
          (int)options->lower_bitfield_extract_to_shifts <= 1);
   assert((int)options->lower_bitfield_insert +
          (int)options->lower_bitfield_insert_to_shifts +
          (int)options->lower_bitfield_insert_to_bitfield_select <= 1);


   STATIC_ASSERT(8 == ARRAY_SIZE(r300_transform_fs_trig_input_values));
   condition_flags[0] = true;

   nir_foreach_function(function, shader) {
      if (function->impl) {
         progress |= nir_algebraic_impl(function->impl, condition_flags,
                                        &r300_transform_fs_trig_input_table);
      }
   }

   return progress;
}
