#include "brw_nir.h"

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 17
 * transforms:
 *    ('fmul@32(is_only_used_as_float)', ('fsign(is_used_once)', 'a(is_not_negative)'), 'b') => ('fcsel_gt', 'a', 'b', ('fmul', 'b', 0.0))
 *    ('~fmul@32', ('fsign(is_used_once)', 'a(is_not_negative)'), 'b') => ('fcsel_gt', 'a', 'b', 0.0)
 *    ('fmul@32(is_only_used_as_float)', ('fsign(is_used_once)', 'a(is_not_positive)'), 'b') => ('fcsel_gt', ('fneg', 'a'), ('fneg', 'b'), ('fmul', 'b', 2147483648))
 *    ('~fmul@32', ('fsign(is_used_once)', 'a(is_not_positive)'), 'b') => ('fcsel_gt', ('fneg', 'a'), ('fneg', 'b'), 2147483648)
 *    ('fmul@16(is_only_used_as_float)', ('fsign(is_used_once)', 'a(is_not_negative)'), 'b') => ('bcsel', ('!flt', 0, 'a'), 'b', ('fmul', 'b', 0.0))
 *    ('~fmul@16', ('fsign(is_used_once)', 'a(is_not_negative)'), 'b') => ('bcsel', ('!flt', 0, 'a'), 'b', 0.0)
 *    ('fmul@16(is_only_used_as_float)', ('fsign(is_used_once)', 'a(is_not_positive)'), 'b') => ('bcsel', ('!flt', 0, ('fneg', 'a')), ('fneg', 'b'), ('fmul', 'b', 32768))
 *    ('~fmul@16', ('fsign(is_used_once)', 'a(is_not_positive)'), 'b') => ('bcsel', ('!flt', 0, ('fneg', 'a')), ('fneg', 'b'), 32768)
 *    ('fmul@32(is_only_used_as_float,nsz)', ('fsign(is_used_once)', 'a'), 'b(is_finite)') => ('fcsel_gt', 'a', 'b', ('fcsel_gt', ('fneg', 'a'), ('fneg', 'b'), 0.0))
 *    ('fmul@32(is_only_used_as_float,nsz,nnan)', ('fsign(is_used_once)', 'a'), 'b') => ('fcsel_gt', 'a', 'b', ('fcsel_gt', ('fneg', 'a'), ('fneg', 'b'), 0.0))
 *    ('~fmul@32', ('fsign(is_used_once)', 'a'), 'b') => ('fcsel_gt', 'a', 'b', ('fcsel_gt', ('fneg', 'a'), ('fneg', 'b'), 0.0))
 *    ('fsign@32', 'a') => ('fcsel_gt', ('fabs', 'a'), ('ior', ('iand', 'a', 2147483648), 1065353216), ('iand', 'a', 2147483648))
 *    ('fsign@16', 'a') => ('bcsel', ('!flt', 0, ('fabs', 'a')), ('ior', ('iand', 'a', 32768), 15360), ('iand', 'a', 32768))
 *    ('fmul(nsz,nnan)', 'a', 0.0) => 0.0
 *    ('fmul(nsz)', 'a(is_finite)', 0.0) => 0.0
 *    ('fmul(nsz,nnan)', 'a@32', 2147483648) => 0.0
 *    ('fmul(nsz,nnan)', 'a@16', 32768) => 0.0
 */


static const nir_search_value_union brw_nir_lower_fsign_values[] = {
   /* ('fmul@32(is_only_used_as_float)', ('fsign(is_used_once)', 'a(is_not_negative)'), 'b') => ('fcsel_gt', 'a', 'b', ('fmul', 'b', 0.0)) */
   { .variable = {
      { nir_search_value_variable, 32 },
      0, /* a */
      false,
      nir_type_invalid,
      0,
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
      nir_op_fsign,
      -1, 0,
      { 0 },
      1,
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
      nir_op_fmul,
      0, 1,
      { 1, 2 },
      0,
   } },

   { .variable = {
      { nir_search_value_variable, 32 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   /* replace0_1 -> 2 in the cache */
   /* replace0_2_0 -> 2 in the cache */
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
      nir_op_fmul,
      0, 1,
      { 2, 5 },
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
      nir_op_fcsel_gt,
      -1, 1,
      { 4, 2, 6 },
      -1,
   } },

   /* ('~fmul@32', ('fsign(is_used_once)', 'a(is_not_negative)'), 'b') => ('fcsel_gt', 'a', 'b', 0.0) */
   /* search1_0_0 -> 0 in the cache */
   /* search1_0 -> 1 in the cache */
   /* search1_1 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      true,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 1, 2 },
      -1,
   } },

   /* replace1_0 -> 4 in the cache */
   /* replace1_1 -> 2 in the cache */
   /* replace1_2 -> 5 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_gt,
      -1, 0,
      { 4, 2, 5 },
      -1,
   } },

   /* ('fmul@32(is_only_used_as_float)', ('fsign(is_used_once)', 'a(is_not_positive)'), 'b') => ('fcsel_gt', ('fneg', 'a'), ('fneg', 'b'), ('fmul', 'b', 2147483648)) */
   { .variable = {
      { nir_search_value_variable, 32 },
      0, /* a */
      false,
      nir_type_invalid,
      1,
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
      nir_op_fsign,
      -1, 0,
      { 10 },
      1,
   } },
   /* search2_1 -> 2 in the cache */
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
      0, 1,
      { 11, 2 },
      0,
   } },

   /* replace2_0_0 -> 4 in the cache */
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
      { 4 },
      -1,
   } },
   /* replace2_1_0 -> 2 in the cache */
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
      { 2 },
      -1,
   } },
   /* replace2_2_0 -> 2 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x80000000ull /* 2147483648 */ },
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
      0, 1,
      { 2, 15 },
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
      nir_op_fcsel_gt,
      -1, 1,
      { 13, 14, 16 },
      -1,
   } },

   /* ('~fmul@32', ('fsign(is_used_once)', 'a(is_not_positive)'), 'b') => ('fcsel_gt', ('fneg', 'a'), ('fneg', 'b'), 2147483648) */
   /* search3_0_0 -> 10 in the cache */
   /* search3_0 -> 11 in the cache */
   /* search3_1 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      true,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 11, 2 },
      -1,
   } },

   /* replace3_0_0 -> 4 in the cache */
   /* replace3_0 -> 13 in the cache */
   /* replace3_1_0 -> 2 in the cache */
   /* replace3_1 -> 14 in the cache */
   /* replace3_2 -> 15 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_gt,
      -1, 0,
      { 13, 14, 15 },
      -1,
   } },

   /* ('fmul@16(is_only_used_as_float)', ('fsign(is_used_once)', 'a(is_not_negative)'), 'b') => ('bcsel', ('!flt', 0, 'a'), 'b', ('fmul', 'b', 0.0)) */
   { .variable = {
      { nir_search_value_variable, 16 },
      0, /* a */
      false,
      nir_type_invalid,
      0,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fsign,
      -1, 0,
      { 20 },
      1,
   } },
   { .variable = {
      { nir_search_value_variable, 16 },
      1, /* b */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 21, 22 },
      0,
   } },

   { .constant = {
      { nir_search_value_constant, 16 },
      nir_type_int, { 0x0ull /* 0 */ },
   } },
   { .variable = {
      { nir_search_value_variable, 16 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
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
      { 24, 25 },
      -1,
   } },
   /* replace4_1 -> 22 in the cache */
   /* replace4_2_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, 16 },
      nir_type_float, { 0x0ull /* 0.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 22, 27 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 1,
      { 26, 22, 28 },
      -1,
   } },

   /* ('~fmul@16', ('fsign(is_used_once)', 'a(is_not_negative)'), 'b') => ('bcsel', ('!flt', 0, 'a'), 'b', 0.0) */
   /* search5_0_0 -> 20 in the cache */
   /* search5_0 -> 21 in the cache */
   /* search5_1 -> 22 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      true,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 21, 22 },
      -1,
   } },

   /* replace5_0_0 -> 24 in the cache */
   /* replace5_0_1 -> 25 in the cache */
   /* replace5_0 -> 26 in the cache */
   /* replace5_1 -> 22 in the cache */
   /* replace5_2 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 0,
      { 26, 22, 27 },
      -1,
   } },

   /* ('fmul@16(is_only_used_as_float)', ('fsign(is_used_once)', 'a(is_not_positive)'), 'b') => ('bcsel', ('!flt', 0, ('fneg', 'a')), ('fneg', 'b'), ('fmul', 'b', 32768)) */
   { .variable = {
      { nir_search_value_variable, 16 },
      0, /* a */
      false,
      nir_type_invalid,
      1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fsign,
      -1, 0,
      { 32 },
      1,
   } },
   /* search6_1 -> 22 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 33, 22 },
      0,
   } },

   /* replace6_0_0 -> 24 in the cache */
   /* replace6_0_1_0 -> 25 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 25 },
      -1,
   } },
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
      { 24, 35 },
      -1,
   } },
   /* replace6_1_0 -> 22 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fneg,
      -1, 0,
      { 22 },
      -1,
   } },
   /* replace6_2_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, 16 },
      nir_type_int, { 0x8000ull /* 32768 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 22, 38 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 1,
      { 36, 37, 39 },
      -1,
   } },

   /* ('~fmul@16', ('fsign(is_used_once)', 'a(is_not_positive)'), 'b') => ('bcsel', ('!flt', 0, ('fneg', 'a')), ('fneg', 'b'), 32768) */
   /* search7_0_0 -> 32 in the cache */
   /* search7_0 -> 33 in the cache */
   /* search7_1 -> 22 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      true,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 33, 22 },
      -1,
   } },

   /* replace7_0_0 -> 24 in the cache */
   /* replace7_0_1_0 -> 25 in the cache */
   /* replace7_0_1 -> 35 in the cache */
   /* replace7_0 -> 36 in the cache */
   /* replace7_1_0 -> 22 in the cache */
   /* replace7_1 -> 37 in the cache */
   /* replace7_2 -> 38 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 0,
      { 36, 37, 38 },
      -1,
   } },

   /* ('fmul@32(is_only_used_as_float,nsz)', ('fsign(is_used_once)', 'a'), 'b(is_finite)') => ('fcsel_gt', 'a', 'b', ('fcsel_gt', ('fneg', 'a'), ('fneg', 'b'), 0.0)) */
   /* search8_0_0 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fsign,
      -1, 0,
      { 4 },
      1,
   } },
   { .variable = {
      { nir_search_value_variable, 32 },
      1, /* b */
      false,
      nir_type_invalid,
      2,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      true,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 43, 44 },
      0,
   } },

   /* replace8_0 -> 4 in the cache */
   /* replace8_1 -> 2 in the cache */
   /* replace8_2_0_0 -> 4 in the cache */
   /* replace8_2_0 -> 13 in the cache */
   /* replace8_2_1_0 -> 2 in the cache */
   /* replace8_2_1 -> 14 in the cache */
   /* replace8_2_2 -> 5 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fcsel_gt,
      -1, 0,
      { 13, 14, 5 },
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
      nir_op_fcsel_gt,
      -1, 0,
      { 4, 2, 46 },
      -1,
   } },

   /* ('fmul@32(is_only_used_as_float,nsz,nnan)', ('fsign(is_used_once)', 'a'), 'b') => ('fcsel_gt', 'a', 'b', ('fcsel_gt', ('fneg', 'a'), ('fneg', 'b'), 0.0)) */
   /* search9_0_0 -> 4 in the cache */
   /* search9_0 -> 43 in the cache */
   /* search9_1 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      true,
      true,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 43, 2 },
      0,
   } },

   /* replace9_0 -> 4 in the cache */
   /* replace9_1 -> 2 in the cache */
   /* replace9_2_0_0 -> 4 in the cache */
   /* replace9_2_0 -> 13 in the cache */
   /* replace9_2_1_0 -> 2 in the cache */
   /* replace9_2_1 -> 14 in the cache */
   /* replace9_2_2 -> 5 in the cache */
   /* replace9_2 -> 46 in the cache */
   /* replace9 -> 47 in the cache */

   /* ('~fmul@32', ('fsign(is_used_once)', 'a'), 'b') => ('fcsel_gt', 'a', 'b', ('fcsel_gt', ('fneg', 'a'), ('fneg', 'b'), 0.0)) */
   /* search10_0_0 -> 4 in the cache */
   /* search10_0 -> 43 in the cache */
   /* search10_1 -> 2 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      true,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 43, 2 },
      -1,
   } },

   /* replace10_0 -> 4 in the cache */
   /* replace10_1 -> 2 in the cache */
   /* replace10_2_0_0 -> 4 in the cache */
   /* replace10_2_0 -> 13 in the cache */
   /* replace10_2_1_0 -> 2 in the cache */
   /* replace10_2_1 -> 14 in the cache */
   /* replace10_2_2 -> 5 in the cache */
   /* replace10_2 -> 46 in the cache */
   /* replace10 -> 47 in the cache */

   /* ('fsign@32', 'a') => ('fcsel_gt', ('fabs', 'a'), ('ior', ('iand', 'a', 2147483648), 1065353216), ('iand', 'a', 2147483648)) */
   /* search11_0 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fsign,
      -1, 0,
      { 4 },
      -1,
   } },

   /* replace11_0_0 -> 4 in the cache */
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
      { 4 },
      -1,
   } },
   /* replace11_1_0_0 -> 4 in the cache */
   /* replace11_1_0_1 -> 15 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_iand,
      1, 1,
      { 4, 15 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x3f800000ull /* 1065353216 */ },
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
      nir_op_ior,
      0, 2,
      { 52, 53 },
      -1,
   } },
   /* replace11_2_0 -> 4 in the cache */
   /* replace11_2_1 -> 15 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_iand,
      2, 1,
      { 4, 15 },
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
      nir_op_fcsel_gt,
      -1, 3,
      { 51, 54, 55 },
      -1,
   } },

   /* ('fsign@16', 'a') => ('bcsel', ('!flt', 0, ('fabs', 'a')), ('ior', ('iand', 'a', 32768), 15360), ('iand', 'a', 32768)) */
   /* search12_0 -> 25 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fsign,
      -1, 0,
      { 25 },
      -1,
   } },

   /* replace12_0_0 -> 24 in the cache */
   /* replace12_0_1_0 -> 25 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_fabs,
      -1, 0,
      { 25 },
      -1,
   } },
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
      { 24, 58 },
      -1,
   } },
   /* replace12_1_0_0 -> 25 in the cache */
   /* replace12_1_0_1 -> 38 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_iand,
      1, 1,
      { 25, 38 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, 16 },
      nir_type_int, { 0x3c00ull /* 15360 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_ior,
      0, 2,
      { 60, 61 },
      -1,
   } },
   /* replace12_2_0 -> 25 in the cache */
   /* replace12_2_1 -> 38 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_iand,
      2, 1,
      { 25, 38 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      false,
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 3,
      { 59, 62, 63 },
      -1,
   } },

   /* ('fmul(nsz,nnan)', 'a', 0.0) => 0.0 */
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
      nir_type_float, { 0x0ull /* 0.0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      true,
      true,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 65, 66 },
      -1,
   } },

   /* replace13 -> 66 in the cache */

   /* ('fmul(nsz)', 'a(is_finite)', 0.0) => 0.0 */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      2,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   /* search14_1 -> 66 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      true,
      false,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 68, 66 },
      -1,
   } },

   /* replace14 -> 66 in the cache */

   /* ('fmul(nsz,nnan)', 'a@32', 2147483648) => 0.0 */
   /* search15_0 -> 4 in the cache */
   /* search15_1 -> 15 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      true,
      true,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 4, 15 },
      -1,
   } },

   /* replace15 -> 5 in the cache */

   /* ('fmul(nsz,nnan)', 'a@16', 32768) => 0.0 */
   /* search16_0 -> 25 in the cache */
   /* search16_1 -> 38 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      true,
      true,
      false,
      false,
      nir_op_fmul,
      0, 1,
      { 25, 38 },
      -1,
   } },

   /* replace16 -> 27 in the cache */

};

static const nir_search_expression_cond brw_nir_lower_fsign_expression_cond[] = {
   is_only_used_as_float,
   is_used_once,
};

static const nir_search_variable_cond brw_nir_lower_fsign_variable_cond[] = {
   (is_not_negative),
   (is_not_positive),
   (is_finite),
};

static const struct transform brw_nir_lower_fsign_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 67, 66, 0 },
   { 69, 66, 0 },
   { 70, 5, 0 },
   { 71, 27, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 50, 56, 0 },
   { 57, 64, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 3, 7, 0 },
   { 8, 9, 0 },
   { 12, 17, 0 },
   { 18, 19, 0 },
   { 23, 29, 0 },
   { 30, 31, 0 },
   { 34, 40, 0 },
   { 41, 42, 0 },
   { 45, 47, 0 },
   { 48, 47, 0 },
   { 49, 47, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 3, 7, 0 },
   { 8, 9, 0 },
   { 12, 17, 0 },
   { 18, 19, 0 },
   { 23, 29, 0 },
   { 30, 31, 0 },
   { 34, 40, 0 },
   { 41, 42, 0 },
   { 45, 47, 0 },
   { 48, 47, 0 },
   { 49, 47, 0 },
   { 67, 66, 0 },
   { 69, 66, 0 },
   { 70, 5, 0 },
   { 71, 27, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table brw_nir_lower_fsign_pass_op_table[nir_num_search_ops] = {
   [nir_op_fmul] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         2,
         0,
         0,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         0,
         2,
         4,
         2,
         2,
         5,
         4,
         5,
         4,
      },
   },
   [nir_op_fsign] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         3,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t brw_nir_lower_fsign_transform_offsets[] = {
   0,
   0,
   1,
   6,
   9,
   21,
};

static const nir_algebraic_table brw_nir_lower_fsign_table = {
   .transforms = brw_nir_lower_fsign_transforms,
   .transform_offsets = brw_nir_lower_fsign_transform_offsets,
   .pass_op_table = brw_nir_lower_fsign_pass_op_table,
   .values = brw_nir_lower_fsign_values,
   .expression_cond = brw_nir_lower_fsign_expression_cond,
   .variable_cond = brw_nir_lower_fsign_variable_cond,
};

bool
brw_nir_lower_fsign(
   nir_shader *shader
) {
   bool progress = false;
   bool condition_flags[1];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(72 == ARRAY_SIZE(brw_nir_lower_fsign_values));
   condition_flags[0] = true;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &brw_nir_lower_fsign_table);
   }

   return progress;
}

