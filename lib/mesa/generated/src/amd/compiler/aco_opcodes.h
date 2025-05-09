/*
 * Copyright (c) 2018 Valve Corporation
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
 *
 * This file was generated by aco_opcodes_h.py
 */

#ifndef _ACO_OPCODES_
#define _ACO_OPCODES_

#include <cstdint>



enum class aco_opcode : std::uint16_t {
   buffer_atomic_add,
   buffer_atomic_add_f32,
   buffer_atomic_add_x2,
   buffer_atomic_and,
   buffer_atomic_and_x2,
   buffer_atomic_cmpswap,
   buffer_atomic_cmpswap_x2,
   buffer_atomic_csub,
   buffer_atomic_dec,
   buffer_atomic_dec_x2,
   buffer_atomic_fcmpswap,
   buffer_atomic_fcmpswap_x2,
   buffer_atomic_fmax,
   buffer_atomic_fmax_x2,
   buffer_atomic_fmin,
   buffer_atomic_fmin_x2,
   buffer_atomic_inc,
   buffer_atomic_inc_x2,
   buffer_atomic_or,
   buffer_atomic_or_x2,
   buffer_atomic_rsub,
   buffer_atomic_rsub_x2,
   buffer_atomic_smax,
   buffer_atomic_smax_x2,
   buffer_atomic_smin,
   buffer_atomic_smin_x2,
   buffer_atomic_sub,
   buffer_atomic_sub_x2,
   buffer_atomic_swap,
   buffer_atomic_swap_x2,
   buffer_atomic_umax,
   buffer_atomic_umax_x2,
   buffer_atomic_umin,
   buffer_atomic_umin_x2,
   buffer_atomic_xor,
   buffer_atomic_xor_x2,
   buffer_gl0_inv,
   buffer_gl1_inv,
   buffer_load_dword,
   buffer_load_dwordx2,
   buffer_load_dwordx3,
   buffer_load_dwordx4,
   buffer_load_format_d16_hi_x,
   buffer_load_format_d16_x,
   buffer_load_format_d16_xy,
   buffer_load_format_d16_xyz,
   buffer_load_format_d16_xyzw,
   buffer_load_format_x,
   buffer_load_format_xy,
   buffer_load_format_xyz,
   buffer_load_format_xyzw,
   buffer_load_lds_b32,
   buffer_load_lds_format_x,
   buffer_load_lds_i16,
   buffer_load_lds_i8,
   buffer_load_lds_u16,
   buffer_load_lds_u8,
   buffer_load_sbyte,
   buffer_load_sbyte_d16,
   buffer_load_sbyte_d16_hi,
   buffer_load_short_d16,
   buffer_load_short_d16_hi,
   buffer_load_sshort,
   buffer_load_ubyte,
   buffer_load_ubyte_d16,
   buffer_load_ubyte_d16_hi,
   buffer_load_ushort,
   buffer_store_byte,
   buffer_store_byte_d16_hi,
   buffer_store_dword,
   buffer_store_dwordx2,
   buffer_store_dwordx3,
   buffer_store_dwordx4,
   buffer_store_format_d16_hi_x,
   buffer_store_format_d16_x,
   buffer_store_format_d16_xy,
   buffer_store_format_d16_xyz,
   buffer_store_format_d16_xyzw,
   buffer_store_format_x,
   buffer_store_format_xy,
   buffer_store_format_xyz,
   buffer_store_format_xyzw,
   buffer_store_lds_dword,
   buffer_store_short,
   buffer_store_short_d16_hi,
   buffer_wbinvl1,
   buffer_wbinvl1_vol,
   ds_add_f32,
   ds_add_gs_reg_rtn,
   ds_add_rtn_f32,
   ds_add_rtn_u32,
   ds_add_rtn_u64,
   ds_add_src2_f32,
   ds_add_src2_u32,
   ds_add_src2_u64,
   ds_add_u32,
   ds_add_u64,
   ds_and_b32,
   ds_and_b64,
   ds_and_rtn_b32,
   ds_and_rtn_b64,
   ds_and_src2_b32,
   ds_and_src2_b64,
   ds_append,
   ds_bpermute_b32,
   ds_cmpst_b32,
   ds_cmpst_b64,
   ds_cmpst_f32,
   ds_cmpst_f64,
   ds_cmpst_rtn_b32,
   ds_cmpst_rtn_b64,
   ds_cmpst_rtn_f32,
   ds_cmpst_rtn_f64,
   ds_condxchg32_rtn_b128,
   ds_condxchg32_rtn_b64,
   ds_consume,
   ds_dec_rtn_u32,
   ds_dec_rtn_u64,
   ds_dec_src2_u32,
   ds_dec_src2_u64,
   ds_dec_u32,
   ds_dec_u64,
   ds_gws_barrier,
   ds_gws_init,
   ds_gws_sema_br,
   ds_gws_sema_p,
   ds_gws_sema_release_all,
   ds_gws_sema_v,
   ds_inc_rtn_u32,
   ds_inc_rtn_u64,
   ds_inc_src2_u32,
   ds_inc_src2_u64,
   ds_inc_u32,
   ds_inc_u64,
   ds_max_f32,
   ds_max_f64,
   ds_max_i32,
   ds_max_i64,
   ds_max_rtn_f32,
   ds_max_rtn_f64,
   ds_max_rtn_i32,
   ds_max_rtn_i64,
   ds_max_rtn_u32,
   ds_max_rtn_u64,
   ds_max_src2_f32,
   ds_max_src2_f64,
   ds_max_src2_i32,
   ds_max_src2_i64,
   ds_max_src2_u32,
   ds_max_src2_u64,
   ds_max_u32,
   ds_max_u64,
   ds_min_f32,
   ds_min_f64,
   ds_min_i32,
   ds_min_i64,
   ds_min_rtn_f32,
   ds_min_rtn_f64,
   ds_min_rtn_i32,
   ds_min_rtn_i64,
   ds_min_rtn_u32,
   ds_min_rtn_u64,
   ds_min_src2_f32,
   ds_min_src2_f64,
   ds_min_src2_i32,
   ds_min_src2_i64,
   ds_min_src2_u32,
   ds_min_src2_u64,
   ds_min_u32,
   ds_min_u64,
   ds_mskor_b32,
   ds_mskor_b64,
   ds_mskor_rtn_b32,
   ds_mskor_rtn_b64,
   ds_nop,
   ds_or_b32,
   ds_or_b64,
   ds_or_rtn_b32,
   ds_or_rtn_b64,
   ds_or_src2_b32,
   ds_or_src2_b64,
   ds_ordered_count,
   ds_permute_b32,
   ds_read2_b32,
   ds_read2_b64,
   ds_read2st64_b32,
   ds_read2st64_b64,
   ds_read_addtid_b32,
   ds_read_b128,
   ds_read_b32,
   ds_read_b64,
   ds_read_b96,
   ds_read_i16,
   ds_read_i8,
   ds_read_i8_d16,
   ds_read_i8_d16_hi,
   ds_read_u16,
   ds_read_u16_d16,
   ds_read_u16_d16_hi,
   ds_read_u8,
   ds_read_u8_d16,
   ds_read_u8_d16_hi,
   ds_rsub_rtn_u32,
   ds_rsub_rtn_u64,
   ds_rsub_src2_u32,
   ds_rsub_src2_u64,
   ds_rsub_u32,
   ds_rsub_u64,
   ds_sub_gs_reg_rtn,
   ds_sub_rtn_u32,
   ds_sub_rtn_u64,
   ds_sub_src2_u32,
   ds_sub_src2_u64,
   ds_sub_u32,
   ds_sub_u64,
   ds_swizzle_b32,
   ds_wrap_rtn_b32,
   ds_write2_b32,
   ds_write2_b64,
   ds_write2st64_b32,
   ds_write2st64_b64,
   ds_write_addtid_b32,
   ds_write_b128,
   ds_write_b16,
   ds_write_b16_d16_hi,
   ds_write_b32,
   ds_write_b64,
   ds_write_b8,
   ds_write_b8_d16_hi,
   ds_write_b96,
   ds_write_src2_b32,
   ds_write_src2_b64,
   ds_wrxchg2_rtn_b32,
   ds_wrxchg2_rtn_b64,
   ds_wrxchg2st64_rtn_b32,
   ds_wrxchg2st64_rtn_b64,
   ds_wrxchg_rtn_b32,
   ds_wrxchg_rtn_b64,
   ds_xor_b32,
   ds_xor_b64,
   ds_xor_rtn_b32,
   ds_xor_rtn_b64,
   ds_xor_src2_b32,
   ds_xor_src2_b64,
   exp,
   flat_atomic_add,
   flat_atomic_add_f32,
   flat_atomic_add_x2,
   flat_atomic_and,
   flat_atomic_and_x2,
   flat_atomic_cmpswap,
   flat_atomic_cmpswap_x2,
   flat_atomic_dec,
   flat_atomic_dec_x2,
   flat_atomic_fcmpswap,
   flat_atomic_fcmpswap_x2,
   flat_atomic_fmax,
   flat_atomic_fmax_x2,
   flat_atomic_fmin,
   flat_atomic_fmin_x2,
   flat_atomic_inc,
   flat_atomic_inc_x2,
   flat_atomic_or,
   flat_atomic_or_x2,
   flat_atomic_smax,
   flat_atomic_smax_x2,
   flat_atomic_smin,
   flat_atomic_smin_x2,
   flat_atomic_sub,
   flat_atomic_sub_x2,
   flat_atomic_swap,
   flat_atomic_swap_x2,
   flat_atomic_umax,
   flat_atomic_umax_x2,
   flat_atomic_umin,
   flat_atomic_umin_x2,
   flat_atomic_xor,
   flat_atomic_xor_x2,
   flat_load_dword,
   flat_load_dwordx2,
   flat_load_dwordx3,
   flat_load_dwordx4,
   flat_load_sbyte,
   flat_load_sbyte_d16,
   flat_load_sbyte_d16_hi,
   flat_load_short_d16,
   flat_load_short_d16_hi,
   flat_load_sshort,
   flat_load_ubyte,
   flat_load_ubyte_d16,
   flat_load_ubyte_d16_hi,
   flat_load_ushort,
   flat_store_byte,
   flat_store_byte_d16_hi,
   flat_store_dword,
   flat_store_dwordx2,
   flat_store_dwordx3,
   flat_store_dwordx4,
   flat_store_short,
   flat_store_short_d16_hi,
   global_atomic_add,
   global_atomic_add_f32,
   global_atomic_add_x2,
   global_atomic_and,
   global_atomic_and_x2,
   global_atomic_cmpswap,
   global_atomic_cmpswap_x2,
   global_atomic_csub,
   global_atomic_dec,
   global_atomic_dec_x2,
   global_atomic_fcmpswap,
   global_atomic_fcmpswap_x2,
   global_atomic_fmax,
   global_atomic_fmax_x2,
   global_atomic_fmin,
   global_atomic_fmin_x2,
   global_atomic_inc,
   global_atomic_inc_x2,
   global_atomic_or,
   global_atomic_or_x2,
   global_atomic_smax,
   global_atomic_smax_x2,
   global_atomic_smin,
   global_atomic_smin_x2,
   global_atomic_sub,
   global_atomic_sub_x2,
   global_atomic_swap,
   global_atomic_swap_x2,
   global_atomic_umax,
   global_atomic_umax_x2,
   global_atomic_umin,
   global_atomic_umin_x2,
   global_atomic_xor,
   global_atomic_xor_x2,
   global_load_dword,
   global_load_dword_addtid,
   global_load_dwordx2,
   global_load_dwordx3,
   global_load_dwordx4,
   global_load_sbyte,
   global_load_sbyte_d16,
   global_load_sbyte_d16_hi,
   global_load_short_d16,
   global_load_short_d16_hi,
   global_load_sshort,
   global_load_ubyte,
   global_load_ubyte_d16,
   global_load_ubyte_d16_hi,
   global_load_ushort,
   global_store_byte,
   global_store_byte_d16_hi,
   global_store_dword,
   global_store_dword_addtid,
   global_store_dwordx2,
   global_store_dwordx3,
   global_store_dwordx4,
   global_store_short,
   global_store_short_d16_hi,
   image_atomic_add,
   image_atomic_and,
   image_atomic_cmpswap,
   image_atomic_dec,
   image_atomic_fcmpswap,
   image_atomic_fmax,
   image_atomic_fmin,
   image_atomic_inc,
   image_atomic_or,
   image_atomic_rsub,
   image_atomic_smax,
   image_atomic_smin,
   image_atomic_sub,
   image_atomic_swap,
   image_atomic_umax,
   image_atomic_umin,
   image_atomic_xor,
   image_bvh64_intersect_ray,
   image_bvh_intersect_ray,
   image_gather4,
   image_gather4_b,
   image_gather4_b_cl,
   image_gather4_b_cl_o,
   image_gather4_b_o,
   image_gather4_c,
   image_gather4_c_b,
   image_gather4_c_b_cl,
   image_gather4_c_b_cl_o,
   image_gather4_c_b_o,
   image_gather4_c_cl,
   image_gather4_c_cl_o,
   image_gather4_c_l,
   image_gather4_c_l_o,
   image_gather4_c_lz,
   image_gather4_c_lz_o,
   image_gather4_c_o,
   image_gather4_cl,
   image_gather4_cl_o,
   image_gather4_l,
   image_gather4_l_o,
   image_gather4_lz,
   image_gather4_lz_o,
   image_gather4_o,
   image_get_lod,
   image_get_resinfo,
   image_load,
   image_load_mip,
   image_load_mip_pck,
   image_load_mip_pck_sgn,
   image_load_pck,
   image_load_pck_sgn,
   image_msaa_load,
   image_sample,
   image_sample_b,
   image_sample_b_cl,
   image_sample_b_cl_o,
   image_sample_b_o,
   image_sample_c,
   image_sample_c_b,
   image_sample_c_b_cl,
   image_sample_c_b_cl_o,
   image_sample_c_b_o,
   image_sample_c_cd,
   image_sample_c_cd_cl,
   image_sample_c_cd_cl_o,
   image_sample_c_cd_o,
   image_sample_c_cl,
   image_sample_c_cl_o,
   image_sample_c_d,
   image_sample_c_d_cl,
   image_sample_c_d_cl_g16,
   image_sample_c_d_cl_o,
   image_sample_c_d_cl_o_g16,
   image_sample_c_d_g16,
   image_sample_c_d_o,
   image_sample_c_d_o_g16,
   image_sample_c_l,
   image_sample_c_l_o,
   image_sample_c_lz,
   image_sample_c_lz_o,
   image_sample_c_o,
   image_sample_cd,
   image_sample_cd_cl,
   image_sample_cd_cl_o,
   image_sample_cd_o,
   image_sample_cl,
   image_sample_cl_o,
   image_sample_d,
   image_sample_d_cl,
   image_sample_d_cl_g16,
   image_sample_d_cl_o,
   image_sample_d_cl_o_g16,
   image_sample_d_g16,
   image_sample_d_o,
   image_sample_d_o_g16,
   image_sample_l,
   image_sample_l_o,
   image_sample_lz,
   image_sample_lz_o,
   image_sample_o,
   image_store,
   image_store_mip,
   image_store_mip_pck,
   image_store_pck,
   lds_direct_load,
   lds_param_load,
   p_as_uniform,
   p_barrier,
   p_bpermute_permlane,
   p_bpermute_readlane,
   p_bpermute_shared_vgpr,
   p_branch,
   p_cbranch,
   p_cbranch_nz,
   p_cbranch_z,
   p_constaddr,
   p_constaddr_addlo,
   p_constaddr_getpc,
   p_create_vector,
   p_cvt_f16_f32_rtne,
   p_demote_to_helper,
   p_discard_if,
   p_dual_src_export_gfx11,
   p_elect,
   p_end_linear_vgpr,
   p_end_with_regs,
   p_end_wqm,
   p_exclusive_scan,
   p_exit_early_if,
   p_extract,
   p_extract_vector,
   p_inclusive_scan,
   p_init_scratch,
   p_insert,
   p_interp_gfx11,
   p_is_helper,
   p_jump_to_epilog,
   p_linear_phi,
   p_load_symbol,
   p_logical_end,
   p_logical_start,
   p_parallelcopy,
   p_phi,
   p_pops_gfx9_add_exiting_wave_id,
   p_pops_gfx9_ordered_section_done,
   p_pops_gfx9_overlapped_wave_wait_done,
   p_reduce,
   p_reload,
   p_resume_shader_address,
   p_resumeaddr_addlo,
   p_resumeaddr_getpc,
   p_return,
   p_spill,
   p_split_vector,
   p_start_linear_vgpr,
   p_startpgm,
   p_unit_test,
   s_abs_i32,
   s_absdiff_i32,
   s_add_i32,
   s_add_u32,
   s_addc_u32,
   s_addk_i32,
   s_and_b32,
   s_and_b64,
   s_and_saveexec_b32,
   s_and_saveexec_b64,
   s_andn1_saveexec_b32,
   s_andn1_saveexec_b64,
   s_andn1_wrexec_b32,
   s_andn1_wrexec_b64,
   s_andn2_b32,
   s_andn2_b64,
   s_andn2_saveexec_b32,
   s_andn2_saveexec_b64,
   s_andn2_wrexec_b32,
   s_andn2_wrexec_b64,
   s_ashr_i32,
   s_ashr_i64,
   s_atc_probe,
   s_atc_probe_buffer,
   s_atomic_add,
   s_atomic_add_x2,
   s_atomic_and,
   s_atomic_and_x2,
   s_atomic_cmpswap,
   s_atomic_cmpswap_x2,
   s_atomic_dec,
   s_atomic_dec_x2,
   s_atomic_inc,
   s_atomic_inc_x2,
   s_atomic_or,
   s_atomic_or_x2,
   s_atomic_smax,
   s_atomic_smax_x2,
   s_atomic_smin,
   s_atomic_smin_x2,
   s_atomic_sub,
   s_atomic_sub_x2,
   s_atomic_swap,
   s_atomic_swap_x2,
   s_atomic_umax,
   s_atomic_umax_x2,
   s_atomic_umin,
   s_atomic_umin_x2,
   s_atomic_xor,
   s_atomic_xor_x2,
   s_barrier,
   s_bcnt0_i32_b32,
   s_bcnt0_i32_b64,
   s_bcnt1_i32_b32,
   s_bcnt1_i32_b64,
   s_bfe_i32,
   s_bfe_i64,
   s_bfe_u32,
   s_bfe_u64,
   s_bfm_b32,
   s_bfm_b64,
   s_bitcmp0_b32,
   s_bitcmp0_b64,
   s_bitcmp1_b32,
   s_bitcmp1_b64,
   s_bitreplicate_b64_b32,
   s_bitset0_b32,
   s_bitset0_b64,
   s_bitset1_b32,
   s_bitset1_b64,
   s_branch,
   s_brev_b32,
   s_brev_b64,
   s_buffer_atomic_add,
   s_buffer_atomic_add_x2,
   s_buffer_atomic_and,
   s_buffer_atomic_and_x2,
   s_buffer_atomic_cmpswap,
   s_buffer_atomic_cmpswap_x2,
   s_buffer_atomic_dec,
   s_buffer_atomic_dec_x2,
   s_buffer_atomic_inc,
   s_buffer_atomic_inc_x2,
   s_buffer_atomic_or,
   s_buffer_atomic_or_x2,
   s_buffer_atomic_smax,
   s_buffer_atomic_smax_x2,
   s_buffer_atomic_smin,
   s_buffer_atomic_smin_x2,
   s_buffer_atomic_sub,
   s_buffer_atomic_sub_x2,
   s_buffer_atomic_swap,
   s_buffer_atomic_swap_x2,
   s_buffer_atomic_umax,
   s_buffer_atomic_umax_x2,
   s_buffer_atomic_umin,
   s_buffer_atomic_umin_x2,
   s_buffer_atomic_xor,
   s_buffer_atomic_xor_x2,
   s_buffer_load_dword,
   s_buffer_load_dwordx16,
   s_buffer_load_dwordx2,
   s_buffer_load_dwordx4,
   s_buffer_load_dwordx8,
   s_buffer_store_dword,
   s_buffer_store_dwordx2,
   s_buffer_store_dwordx4,
   s_call_b64,
   s_cbranch_cdbgsys,
   s_cbranch_cdbgsys_and_user,
   s_cbranch_cdbgsys_or_user,
   s_cbranch_cdbguser,
   s_cbranch_execnz,
   s_cbranch_execz,
   s_cbranch_g_fork,
   s_cbranch_i_fork,
   s_cbranch_join,
   s_cbranch_scc0,
   s_cbranch_scc1,
   s_cbranch_vccnz,
   s_cbranch_vccz,
   s_clause,
   s_cmov_b32,
   s_cmov_b64,
   s_cmovk_i32,
   s_cmp_eq_i32,
   s_cmp_eq_u32,
   s_cmp_eq_u64,
   s_cmp_ge_i32,
   s_cmp_ge_u32,
   s_cmp_gt_i32,
   s_cmp_gt_u32,
   s_cmp_le_i32,
   s_cmp_le_u32,
   s_cmp_lg_i32,
   s_cmp_lg_u32,
   s_cmp_lg_u64,
   s_cmp_lt_i32,
   s_cmp_lt_u32,
   s_cmpk_eq_i32,
   s_cmpk_eq_u32,
   s_cmpk_ge_i32,
   s_cmpk_ge_u32,
   s_cmpk_gt_i32,
   s_cmpk_gt_u32,
   s_cmpk_le_i32,
   s_cmpk_le_u32,
   s_cmpk_lg_i32,
   s_cmpk_lg_u32,
   s_cmpk_lt_i32,
   s_cmpk_lt_u32,
   s_code_end,
   s_cselect_b32,
   s_cselect_b64,
   s_dcache_discard,
   s_dcache_discard_x2,
   s_dcache_inv,
   s_dcache_inv_vol,
   s_dcache_wb,
   s_dcache_wb_vol,
   s_decperflevel,
   s_delay_alu,
   s_denorm_mode,
   s_endpgm,
   s_endpgm_ordered_ps_done,
   s_endpgm_saved,
   s_ff0_i32_b32,
   s_ff0_i32_b64,
   s_ff1_i32_b32,
   s_ff1_i32_b64,
   s_flbit_i32,
   s_flbit_i32_b32,
   s_flbit_i32_b64,
   s_flbit_i32_i64,
   s_get_waveid_in_workgroup,
   s_getpc_b64,
   s_getreg_b32,
   s_gl1_inv,
   s_icache_inv,
   s_incperflevel,
   s_inst_prefetch,
   s_load_dword,
   s_load_dwordx16,
   s_load_dwordx2,
   s_load_dwordx4,
   s_load_dwordx8,
   s_lshl1_add_u32,
   s_lshl2_add_u32,
   s_lshl3_add_u32,
   s_lshl4_add_u32,
   s_lshl_b32,
   s_lshl_b64,
   s_lshr_b32,
   s_lshr_b64,
   s_max_i32,
   s_max_u32,
   s_memrealtime,
   s_memtime,
   s_min_i32,
   s_min_u32,
   s_mov_b32,
   s_mov_b64,
   s_mov_fed_b32,
   s_movk_i32,
   s_movreld_b32,
   s_movreld_b64,
   s_movrels_b32,
   s_movrels_b64,
   s_movrelsd_2_b32,
   s_mul_hi_i32,
   s_mul_hi_u32,
   s_mul_i32,
   s_mulk_i32,
   s_nand_b32,
   s_nand_b64,
   s_nand_saveexec_b32,
   s_nand_saveexec_b64,
   s_nop,
   s_nor_b32,
   s_nor_b64,
   s_nor_saveexec_b32,
   s_nor_saveexec_b64,
   s_not_b32,
   s_not_b64,
   s_or_b32,
   s_or_b64,
   s_or_saveexec_b32,
   s_or_saveexec_b64,
   s_orn1_saveexec_b32,
   s_orn1_saveexec_b64,
   s_orn2_b32,
   s_orn2_b64,
   s_orn2_saveexec_b32,
   s_orn2_saveexec_b64,
   s_pack_hh_b32_b16,
   s_pack_hl_b32_b16,
   s_pack_lh_b32_b16,
   s_pack_ll_b32_b16,
   s_quadmask_b32,
   s_quadmask_b64,
   s_rfe_b64,
   s_rfe_restore_b64,
   s_round_mode,
   s_scratch_load_dword,
   s_scratch_load_dwordx2,
   s_scratch_load_dwordx4,
   s_scratch_store_dword,
   s_scratch_store_dwordx2,
   s_scratch_store_dwordx4,
   s_sendmsg,
   s_sendmsg_rtn_b32,
   s_sendmsg_rtn_b64,
   s_sendmsghalt,
   s_set_gpr_idx_idx,
   s_set_gpr_idx_mode,
   s_set_gpr_idx_off,
   s_set_gpr_idx_on,
   s_sethalt,
   s_setkill,
   s_setpc_b64,
   s_setprio,
   s_setreg_b32,
   s_setreg_imm32_b32,
   s_setvskip,
   s_sext_i32_i16,
   s_sext_i32_i8,
   s_sleep,
   s_store_dword,
   s_store_dwordx2,
   s_store_dwordx4,
   s_sub_i32,
   s_sub_u32,
   s_subb_u32,
   s_subvector_loop_begin,
   s_subvector_loop_end,
   s_swappc_b64,
   s_trap,
   s_ttracedata,
   s_ttracedata_imm,
   s_version,
   s_wait_event,
   s_wait_idle,
   s_waitcnt,
   s_waitcnt_depctr,
   s_waitcnt_expcnt,
   s_waitcnt_lgkmcnt,
   s_waitcnt_vmcnt,
   s_waitcnt_vscnt,
   s_wakeup,
   s_wqm_b32,
   s_wqm_b64,
   s_xnor_b32,
   s_xnor_b64,
   s_xnor_saveexec_b32,
   s_xnor_saveexec_b64,
   s_xor_b32,
   s_xor_b64,
   s_xor_saveexec_b32,
   s_xor_saveexec_b64,
   scratch_load_dword,
   scratch_load_dwordx2,
   scratch_load_dwordx3,
   scratch_load_dwordx4,
   scratch_load_sbyte,
   scratch_load_sbyte_d16,
   scratch_load_sbyte_d16_hi,
   scratch_load_short_d16,
   scratch_load_short_d16_hi,
   scratch_load_sshort,
   scratch_load_ubyte,
   scratch_load_ubyte_d16,
   scratch_load_ubyte_d16_hi,
   scratch_load_ushort,
   scratch_store_byte,
   scratch_store_byte_d16_hi,
   scratch_store_dword,
   scratch_store_dwordx2,
   scratch_store_dwordx3,
   scratch_store_dwordx4,
   scratch_store_short,
   scratch_store_short_d16_hi,
   tbuffer_load_format_d16_x,
   tbuffer_load_format_d16_xy,
   tbuffer_load_format_d16_xyz,
   tbuffer_load_format_d16_xyzw,
   tbuffer_load_format_x,
   tbuffer_load_format_xy,
   tbuffer_load_format_xyz,
   tbuffer_load_format_xyzw,
   tbuffer_store_format_d16_x,
   tbuffer_store_format_d16_xy,
   tbuffer_store_format_d16_xyz,
   tbuffer_store_format_d16_xyzw,
   tbuffer_store_format_x,
   tbuffer_store_format_xy,
   tbuffer_store_format_xyz,
   tbuffer_store_format_xyzw,
   v_add3_u32,
   v_add_co_u32,
   v_add_co_u32_e64,
   v_add_f16,
   v_add_f32,
   v_add_f64,
   v_add_i16,
   v_add_i32,
   v_add_lshl_u32,
   v_add_u16,
   v_add_u16_e64,
   v_add_u32,
   v_addc_co_u32,
   v_alignbit_b32,
   v_alignbyte_b32,
   v_and_b16,
   v_and_b32,
   v_and_or_b32,
   v_ashr_i32,
   v_ashr_i64,
   v_ashrrev_i16,
   v_ashrrev_i16_e64,
   v_ashrrev_i32,
   v_ashrrev_i64,
   v_bcnt_u32_b32,
   v_bfe_i32,
   v_bfe_u32,
   v_bfi_b32,
   v_bfm_b32,
   v_bfrev_b32,
   v_ceil_f16,
   v_ceil_f32,
   v_ceil_f64,
   v_clrexcp,
   v_cmp_class_f16,
   v_cmp_class_f32,
   v_cmp_class_f64,
   v_cmp_eq_f16,
   v_cmp_eq_f32,
   v_cmp_eq_f64,
   v_cmp_eq_i16,
   v_cmp_eq_i32,
   v_cmp_eq_i64,
   v_cmp_eq_u16,
   v_cmp_eq_u32,
   v_cmp_eq_u64,
   v_cmp_f_f16,
   v_cmp_f_f32,
   v_cmp_f_f64,
   v_cmp_f_i16,
   v_cmp_f_i32,
   v_cmp_f_i64,
   v_cmp_f_u16,
   v_cmp_f_u32,
   v_cmp_f_u64,
   v_cmp_ge_f16,
   v_cmp_ge_f32,
   v_cmp_ge_f64,
   v_cmp_ge_i16,
   v_cmp_ge_i32,
   v_cmp_ge_i64,
   v_cmp_ge_u16,
   v_cmp_ge_u32,
   v_cmp_ge_u64,
   v_cmp_gt_f16,
   v_cmp_gt_f32,
   v_cmp_gt_f64,
   v_cmp_gt_i16,
   v_cmp_gt_i32,
   v_cmp_gt_i64,
   v_cmp_gt_u16,
   v_cmp_gt_u32,
   v_cmp_gt_u64,
   v_cmp_le_f16,
   v_cmp_le_f32,
   v_cmp_le_f64,
   v_cmp_le_i16,
   v_cmp_le_i32,
   v_cmp_le_i64,
   v_cmp_le_u16,
   v_cmp_le_u32,
   v_cmp_le_u64,
   v_cmp_lg_f16,
   v_cmp_lg_f32,
   v_cmp_lg_f64,
   v_cmp_lg_i16,
   v_cmp_lg_i32,
   v_cmp_lg_i64,
   v_cmp_lg_u16,
   v_cmp_lg_u32,
   v_cmp_lg_u64,
   v_cmp_lt_f16,
   v_cmp_lt_f32,
   v_cmp_lt_f64,
   v_cmp_lt_i16,
   v_cmp_lt_i32,
   v_cmp_lt_i64,
   v_cmp_lt_u16,
   v_cmp_lt_u32,
   v_cmp_lt_u64,
   v_cmp_neq_f16,
   v_cmp_neq_f32,
   v_cmp_neq_f64,
   v_cmp_nge_f16,
   v_cmp_nge_f32,
   v_cmp_nge_f64,
   v_cmp_ngt_f16,
   v_cmp_ngt_f32,
   v_cmp_ngt_f64,
   v_cmp_nle_f16,
   v_cmp_nle_f32,
   v_cmp_nle_f64,
   v_cmp_nlg_f16,
   v_cmp_nlg_f32,
   v_cmp_nlg_f64,
   v_cmp_nlt_f16,
   v_cmp_nlt_f32,
   v_cmp_nlt_f64,
   v_cmp_o_f16,
   v_cmp_o_f32,
   v_cmp_o_f64,
   v_cmp_tru_f16,
   v_cmp_tru_f32,
   v_cmp_tru_f64,
   v_cmp_tru_i16,
   v_cmp_tru_i32,
   v_cmp_tru_i64,
   v_cmp_tru_u16,
   v_cmp_tru_u32,
   v_cmp_tru_u64,
   v_cmp_u_f16,
   v_cmp_u_f32,
   v_cmp_u_f64,
   v_cmpx_class_f16,
   v_cmpx_class_f32,
   v_cmpx_class_f64,
   v_cmpx_eq_f16,
   v_cmpx_eq_f32,
   v_cmpx_eq_f64,
   v_cmpx_eq_i16,
   v_cmpx_eq_i32,
   v_cmpx_eq_i64,
   v_cmpx_eq_u16,
   v_cmpx_eq_u32,
   v_cmpx_eq_u64,
   v_cmpx_f_f16,
   v_cmpx_f_f32,
   v_cmpx_f_f64,
   v_cmpx_f_i16,
   v_cmpx_f_i32,
   v_cmpx_f_i64,
   v_cmpx_f_u16,
   v_cmpx_f_u32,
   v_cmpx_f_u64,
   v_cmpx_ge_f16,
   v_cmpx_ge_f32,
   v_cmpx_ge_f64,
   v_cmpx_ge_i16,
   v_cmpx_ge_i32,
   v_cmpx_ge_i64,
   v_cmpx_ge_u16,
   v_cmpx_ge_u32,
   v_cmpx_ge_u64,
   v_cmpx_gt_f16,
   v_cmpx_gt_f32,
   v_cmpx_gt_f64,
   v_cmpx_gt_i16,
   v_cmpx_gt_i32,
   v_cmpx_gt_i64,
   v_cmpx_gt_u16,
   v_cmpx_gt_u32,
   v_cmpx_gt_u64,
   v_cmpx_le_f16,
   v_cmpx_le_f32,
   v_cmpx_le_f64,
   v_cmpx_le_i16,
   v_cmpx_le_i32,
   v_cmpx_le_i64,
   v_cmpx_le_u16,
   v_cmpx_le_u32,
   v_cmpx_le_u64,
   v_cmpx_lg_f16,
   v_cmpx_lg_f32,
   v_cmpx_lg_f64,
   v_cmpx_lg_i16,
   v_cmpx_lg_i32,
   v_cmpx_lg_i64,
   v_cmpx_lg_u16,
   v_cmpx_lg_u32,
   v_cmpx_lg_u64,
   v_cmpx_lt_f16,
   v_cmpx_lt_f32,
   v_cmpx_lt_f64,
   v_cmpx_lt_i16,
   v_cmpx_lt_i32,
   v_cmpx_lt_i64,
   v_cmpx_lt_u16,
   v_cmpx_lt_u32,
   v_cmpx_lt_u64,
   v_cmpx_neq_f16,
   v_cmpx_neq_f32,
   v_cmpx_neq_f64,
   v_cmpx_nge_f16,
   v_cmpx_nge_f32,
   v_cmpx_nge_f64,
   v_cmpx_ngt_f16,
   v_cmpx_ngt_f32,
   v_cmpx_ngt_f64,
   v_cmpx_nle_f16,
   v_cmpx_nle_f32,
   v_cmpx_nle_f64,
   v_cmpx_nlg_f16,
   v_cmpx_nlg_f32,
   v_cmpx_nlg_f64,
   v_cmpx_nlt_f16,
   v_cmpx_nlt_f32,
   v_cmpx_nlt_f64,
   v_cmpx_o_f16,
   v_cmpx_o_f32,
   v_cmpx_o_f64,
   v_cmpx_tru_f16,
   v_cmpx_tru_f32,
   v_cmpx_tru_f64,
   v_cmpx_tru_i16,
   v_cmpx_tru_i32,
   v_cmpx_tru_i64,
   v_cmpx_tru_u16,
   v_cmpx_tru_u32,
   v_cmpx_tru_u64,
   v_cmpx_u_f16,
   v_cmpx_u_f32,
   v_cmpx_u_f64,
   v_cndmask_b16,
   v_cndmask_b32,
   v_cos_f16,
   v_cos_f32,
   v_cubeid_f32,
   v_cubema_f32,
   v_cubesc_f32,
   v_cubetc_f32,
   v_cvt_f16_f32,
   v_cvt_f16_i16,
   v_cvt_f16_u16,
   v_cvt_f32_f16,
   v_cvt_f32_f64,
   v_cvt_f32_i32,
   v_cvt_f32_u32,
   v_cvt_f32_ubyte0,
   v_cvt_f32_ubyte1,
   v_cvt_f32_ubyte2,
   v_cvt_f32_ubyte3,
   v_cvt_f64_f32,
   v_cvt_f64_i32,
   v_cvt_f64_u32,
   v_cvt_flr_i32_f32,
   v_cvt_i16_f16,
   v_cvt_i32_f32,
   v_cvt_i32_f64,
   v_cvt_i32_i16,
   v_cvt_norm_i16_f16,
   v_cvt_norm_u16_f16,
   v_cvt_off_f32_i4,
   v_cvt_pk_i16_f32,
   v_cvt_pk_i16_i32,
   v_cvt_pk_u16_f32,
   v_cvt_pk_u16_u32,
   v_cvt_pk_u8_f32,
   v_cvt_pkaccum_u8_f32,
   v_cvt_pknorm_i16_f16,
   v_cvt_pknorm_i16_f32,
   v_cvt_pknorm_u16_f16,
   v_cvt_pknorm_u16_f32,
   v_cvt_pkrtz_f16_f32,
   v_cvt_pkrtz_f16_f32_e64,
   v_cvt_rpi_i32_f32,
   v_cvt_u16_f16,
   v_cvt_u32_f32,
   v_cvt_u32_f64,
   v_cvt_u32_u16,
   v_div_fixup_f16,
   v_div_fixup_f32,
   v_div_fixup_f64,
   v_div_fixup_legacy_f16,
   v_div_fmas_f32,
   v_div_fmas_f64,
   v_div_scale_f32,
   v_div_scale_f64,
   v_dot2_bf16_bf16,
   v_dot2_f16_f16,
   v_dot2_f32_bf16,
   v_dot2_f32_f16,
   v_dot2_i32_i16,
   v_dot2_u32_u16,
   v_dot2c_f32_f16,
   v_dot4_i32_i8,
   v_dot4_i32_iu8,
   v_dot4_u32_u8,
   v_dot4c_i32_i8,
   v_dot8_i32_iu4,
   v_dot8_u32_u4,
   v_exp_f16,
   v_exp_f32,
   v_exp_legacy_f32,
   v_ffbh_i32,
   v_ffbh_u32,
   v_ffbl_b32,
   v_floor_f16,
   v_floor_f32,
   v_floor_f64,
   v_fma_f16,
   v_fma_f32,
   v_fma_f64,
   v_fma_legacy_f16,
   v_fma_legacy_f32,
   v_fma_mix_f32,
   v_fma_mixhi_f16,
   v_fma_mixlo_f16,
   v_fmaak_f16,
   v_fmaak_f32,
   v_fmac_f16,
   v_fmac_f32,
   v_fmac_legacy_f32,
   v_fmamk_f16,
   v_fmamk_f32,
   v_fract_f16,
   v_fract_f32,
   v_fract_f64,
   v_frexp_exp_i16_f16,
   v_frexp_exp_i32_f32,
   v_frexp_exp_i32_f64,
   v_frexp_mant_f16,
   v_frexp_mant_f32,
   v_frexp_mant_f64,
   v_interp_mov_f32,
   v_interp_p10_f16_f32_inreg,
   v_interp_p10_f32_inreg,
   v_interp_p10_rtz_f16_f32_inreg,
   v_interp_p1_f32,
   v_interp_p1ll_f16,
   v_interp_p1lv_f16,
   v_interp_p2_f16,
   v_interp_p2_f16_f32_inreg,
   v_interp_p2_f32,
   v_interp_p2_f32_inreg,
   v_interp_p2_legacy_f16,
   v_interp_p2_rtz_f16_f32_inreg,
   v_ldexp_f16,
   v_ldexp_f32,
   v_ldexp_f64,
   v_lerp_u8,
   v_log_clamp_f32,
   v_log_f16,
   v_log_f32,
   v_log_legacy_f32,
   v_lshl_add_u32,
   v_lshl_b32,
   v_lshl_b64,
   v_lshl_or_b32,
   v_lshlrev_b16,
   v_lshlrev_b16_e64,
   v_lshlrev_b32,
   v_lshlrev_b64,
   v_lshr_b32,
   v_lshr_b64,
   v_lshrrev_b16,
   v_lshrrev_b16_e64,
   v_lshrrev_b32,
   v_lshrrev_b64,
   v_mac_f16,
   v_mac_f32,
   v_mac_legacy_f32,
   v_mad_f16,
   v_mad_f32,
   v_mad_i16,
   v_mad_i32_i16,
   v_mad_i32_i24,
   v_mad_i64_i32,
   v_mad_legacy_f16,
   v_mad_legacy_f32,
   v_mad_legacy_i16,
   v_mad_legacy_u16,
   v_mad_u16,
   v_mad_u32_u16,
   v_mad_u32_u24,
   v_mad_u64_u32,
   v_madak_f16,
   v_madak_f32,
   v_madmk_f16,
   v_madmk_f32,
   v_max3_f16,
   v_max3_f32,
   v_max3_i16,
   v_max3_i32,
   v_max3_u16,
   v_max3_u32,
   v_max_f16,
   v_max_f32,
   v_max_f64,
   v_max_i16,
   v_max_i16_e64,
   v_max_i32,
   v_max_legacy_f32,
   v_max_u16,
   v_max_u16_e64,
   v_max_u32,
   v_maxmin_f16,
   v_maxmin_f32,
   v_maxmin_i32,
   v_maxmin_u32,
   v_mbcnt_hi_u32_b32,
   v_mbcnt_hi_u32_b32_e64,
   v_mbcnt_lo_u32_b32,
   v_med3_f16,
   v_med3_f32,
   v_med3_i16,
   v_med3_i32,
   v_med3_u16,
   v_med3_u32,
   v_min3_f16,
   v_min3_f32,
   v_min3_i16,
   v_min3_i32,
   v_min3_u16,
   v_min3_u32,
   v_min_f16,
   v_min_f32,
   v_min_f64,
   v_min_i16,
   v_min_i16_e64,
   v_min_i32,
   v_min_legacy_f32,
   v_min_u16,
   v_min_u16_e64,
   v_min_u32,
   v_minmax_f16,
   v_minmax_f32,
   v_minmax_i32,
   v_minmax_u32,
   v_mov_b16,
   v_mov_b32,
   v_movreld_b32,
   v_movrels_b32,
   v_movrelsd_2_b32,
   v_movrelsd_b32,
   v_mqsad_pk_u16_u8,
   v_mqsad_u32_u8,
   v_mqsad_u8,
   v_msad_u8,
   v_mul_f16,
   v_mul_f32,
   v_mul_f64,
   v_mul_hi_i32,
   v_mul_hi_i32_i24,
   v_mul_hi_u32,
   v_mul_hi_u32_u24,
   v_mul_i32_i24,
   v_mul_legacy_f32,
   v_mul_lo_i32,
   v_mul_lo_u16,
   v_mul_lo_u16_e64,
   v_mul_lo_u32,
   v_mul_u32_u24,
   v_mullit_f32,
   v_nop,
   v_not_b16,
   v_not_b32,
   v_or3_b32,
   v_or_b16,
   v_or_b32,
   v_pack_b32_f16,
   v_perm_b32,
   v_permlane16_b32,
   v_permlane64_b32,
   v_permlanex16_b32,
   v_pipeflush,
   v_pk_add_f16,
   v_pk_add_i16,
   v_pk_add_u16,
   v_pk_ashrrev_i16,
   v_pk_fma_f16,
   v_pk_fmac_f16,
   v_pk_lshlrev_b16,
   v_pk_lshrrev_b16,
   v_pk_mad_i16,
   v_pk_mad_u16,
   v_pk_max_f16,
   v_pk_max_i16,
   v_pk_max_u16,
   v_pk_min_f16,
   v_pk_min_i16,
   v_pk_min_u16,
   v_pk_mul_f16,
   v_pk_mul_lo_u16,
   v_pk_sub_i16,
   v_pk_sub_u16,
   v_qsad_pk_u16_u8,
   v_qsad_u8,
   v_rcp_clamp_f32,
   v_rcp_clamp_f64,
   v_rcp_f16,
   v_rcp_f32,
   v_rcp_f64,
   v_rcp_iflag_f32,
   v_rcp_legacy_f32,
   v_readfirstlane_b32,
   v_readlane_b32,
   v_readlane_b32_e64,
   v_rndne_f16,
   v_rndne_f32,
   v_rndne_f64,
   v_rsq_clamp_f32,
   v_rsq_clamp_f64,
   v_rsq_f16,
   v_rsq_f32,
   v_rsq_f64,
   v_rsq_legacy_f32,
   v_sad_hi_u8,
   v_sad_u16,
   v_sad_u32,
   v_sad_u8,
   v_sat_pk_u8_i16,
   v_screen_partition_4se_b32,
   v_sin_f16,
   v_sin_f32,
   v_sqrt_f16,
   v_sqrt_f32,
   v_sqrt_f64,
   v_sub_co_u32,
   v_sub_co_u32_e64,
   v_sub_f16,
   v_sub_f32,
   v_sub_i16,
   v_sub_i32,
   v_sub_u16,
   v_sub_u16_e64,
   v_sub_u32,
   v_subb_co_u32,
   v_subbrev_co_u32,
   v_subrev_co_u32,
   v_subrev_co_u32_e64,
   v_subrev_f16,
   v_subrev_f32,
   v_subrev_u16,
   v_subrev_u32,
   v_swap_b32,
   v_swaprel_b32,
   v_trig_preop_f64,
   v_trunc_f16,
   v_trunc_f32,
   v_trunc_f64,
   v_wmma_bf16_16x16x16_bf16,
   v_wmma_f16_16x16x16_f16,
   v_wmma_f32_16x16x16_bf16,
   v_wmma_f32_16x16x16_f16,
   v_wmma_i32_16x16x16_iu4,
   v_wmma_i32_16x16x16_iu8,
   v_writelane_b32,
   v_writelane_b32_e64,
   v_xad_u32,
   v_xnor_b32,
   v_xor3_b32,
   v_xor_b16,
   v_xor_b32,
   last_opcode = v_xor_b32,
   num_opcodes = last_opcode + 1
};

#endif /* _ACO_OPCODES_ */
