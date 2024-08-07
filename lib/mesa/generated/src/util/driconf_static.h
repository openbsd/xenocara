/* Copyright (C) 2021 Google, Inc.
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

struct driconf_option {
    const char *name;
    const char *value;
};

struct driconf_application {
    const char *name;
    const char *executable;
    const char *executable_regexp;
    const char *sha1;
    const char *application_name_match;
    const char *application_versions;
    unsigned num_options;
    const struct driconf_option *options;
};

struct driconf_engine {
    const char *engine_name_match;
    const char *engine_versions;
    unsigned num_options;
    const struct driconf_option *options;
};

struct driconf_device {
    const char *driver;
    const char *device;
    unsigned num_engines;
    const struct driconf_engine *engines;
    unsigned num_applications;
    const struct driconf_application *applications;
};



    
static const struct driconf_option engine_482_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};


static const struct driconf_engine device_1_engines[] = {
    { .engine_name_match = "UnrealEngine4.*",
      .engine_versions = "0:23",
      .num_options = 1,
      .options = engine_482_options,
    },
};

    
static const struct driconf_option application_2_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_4_options[] = {
    { .name = "force_glsl_extensions_warn", .value = "true" },
    { .name = "disable_blend_func_extended", .value = "true" },
    { .name = "disable_arb_gpu_shader5", .value = "true" },
};

    
static const struct driconf_option application_8_options[] = {
    { .name = "force_glsl_extensions_warn", .value = "true" },
    { .name = "disable_blend_func_extended", .value = "true" },
    { .name = "disable_arb_gpu_shader5", .value = "true" },
};

    
static const struct driconf_option application_12_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_15_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_18_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_21_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_24_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_27_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_30_options[] = {
    { .name = "disable_blend_func_extended", .value = "true" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_33_options[] = {
    { .name = "disable_blend_func_extended", .value = "true" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_36_options[] = {
    { .name = "disable_glsl_line_continuations", .value = "true" },
};

    
static const struct driconf_option application_38_options[] = {
    { .name = "always_have_depth_buffer", .value = "true" },
};

    
static const struct driconf_option application_40_options[] = {
    { .name = "always_have_depth_buffer", .value = "true" },
};

    
static const struct driconf_option application_42_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_44_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_46_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_48_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_51_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_53_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
    { .name = "force_glsl_extensions_warn", .value = "true" },
};

    
static const struct driconf_option application_56_options[] = {
    { .name = "disable_uniform_array_resize", .value = "true" },
};

    
static const struct driconf_option application_58_options[] = {
    { .name = "disable_uniform_array_resize", .value = "true" },
    { .name = "alias_shader_extension", .value = "GL_ATI_shader_texture_lod:GL_ARB_shader_texture_lod" },
    { .name = "allow_vertex_texture_bias", .value = "true" },
};

    
static const struct driconf_option application_62_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_65_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_67_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_69_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
    { .name = "force_gl_map_buffer_synchronized", .value = "true" },
};

    
static const struct driconf_option application_72_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
    { .name = "force_gl_map_buffer_synchronized", .value = "true" },
};

    
static const struct driconf_option application_75_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_77_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_79_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_81_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_83_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
    { .name = "force_gl_vendor", .value = "ATI Technologies, Inc." },
};

    
static const struct driconf_option application_87_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_89_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_91_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_93_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_95_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_97_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_99_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_101_options[] = {
    { .name = "force_compat_shaders", .value = "true" },
};

    
static const struct driconf_option application_103_options[] = {
    { .name = "force_compat_shaders", .value = "true" },
};

    
static const struct driconf_option application_105_options[] = {
    { .name = "force_glsl_version", .value = "440" },
};

    
static const struct driconf_option application_107_options[] = {
    { .name = "force_glsl_abs_sqrt", .value = "true" },
};

    
static const struct driconf_option application_109_options[] = {
    { .name = "force_glsl_abs_sqrt", .value = "true" },
};

    
static const struct driconf_option application_111_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_113_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_115_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_117_options[] = {
    { .name = "force_glsl_version", .value = "130" },
    { .name = "glsl_ignore_write_to_readonly_var", .value = "true" },
};

    
static const struct driconf_option application_120_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_122_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_124_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_126_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_128_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_130_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_132_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_134_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_136_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_138_options[] = {
    { .name = "allow_glsl_builtin_const_expression", .value = "true" },
    { .name = "allow_glsl_relaxed_es", .value = "true" },
};

    
static const struct driconf_option application_141_options[] = {
    { .name = "allow_extra_pp_tokens", .value = "true" },
};

    
static const struct driconf_option application_143_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_145_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_147_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_149_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_151_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_153_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
};

    
static const struct driconf_option application_155_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
};

    
static const struct driconf_option application_157_options[] = {
    { .name = "vs_position_always_invariant", .value = "true" },
};

    
static const struct driconf_option application_159_options[] = {
    { .name = "lower_depth_range_rate", .value = "0.8" },
};

    
static const struct driconf_option application_161_options[] = {
    { .name = "lower_depth_range_rate", .value = "0.8" },
};

    
static const struct driconf_option application_163_options[] = {
    { .name = "allow_multisampled_copyteximage", .value = "true" },
};

    
static const struct driconf_option application_165_options[] = {
    { .name = "vblank_mode", .value = "0" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
    { .name = "force_gl_names_reuse", .value = "true" },
    { .name = "force_gl_vendor", .value = "NVIDIA Corporation" },
    { .name = "force_glsl_extensions_warn", .value = "true" },
    { .name = "mesa_glthread_app_profile", .value = "1" },
    { .name = "mesa_no_error", .value = "true" },
    { .name = "allow_rgb10_configs", .value = "false" },
    { .name = "allow_invalid_glx_destroy_window", .value = "true" },
};

    
static const struct driconf_option application_176_options[] = {
    { .name = "ignore_map_unsynchronized", .value = "true" },
};

    
static const struct driconf_option application_178_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_181_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_183_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_185_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_187_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_189_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_191_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
};

    
static const struct driconf_option application_193_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
    { .name = "do_dce_before_clip_cull_analysis", .value = "true" },
};

    
static const struct driconf_option application_196_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
};

    
static const struct driconf_option application_198_options[] = {
    { .name = "force_glsl_extensions_warn", .value = "true" },
};

    
static const struct driconf_option application_200_options[] = {
    { .name = "force_direct_glx_context", .value = "true" },
};

    
static const struct driconf_option application_202_options[] = {
    { .name = "keep_native_window_glx_drawable", .value = "true" },
    { .name = "allow_rgb10_configs", .value = "false" },
};

    
static const struct driconf_option application_205_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_207_options[] = {
    { .name = "force_compat_shaders", .value = "true" },
};

    
static const struct driconf_option application_209_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
};

    
static const struct driconf_option application_211_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_213_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_215_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_217_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_219_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_221_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_223_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_225_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_227_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_229_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_231_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_233_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_235_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_237_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_239_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_241_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_243_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_245_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_247_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_249_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_251_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_253_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_255_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_257_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_259_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_261_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_263_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_265_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_267_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_269_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_271_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_273_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_275_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
};

    
static const struct driconf_option application_278_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
};

    
static const struct driconf_option application_281_options[] = {
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
};

    
static const struct driconf_option application_283_options[] = {
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
};

    
static const struct driconf_option application_285_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_287_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_289_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_291_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_293_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_295_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_297_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_299_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_301_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_303_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_305_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_307_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_309_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_311_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_313_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_315_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_317_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_319_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_321_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_323_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_325_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_327_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_329_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_331_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_333_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_335_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_337_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_339_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
};

    
static const struct driconf_option application_341_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
};

    
static const struct driconf_option application_343_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
};

    
static const struct driconf_option application_345_options[] = {
    { .name = "adaptive_sync", .value = "false" },
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_348_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_350_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_352_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_354_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_356_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_358_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_360_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_362_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_364_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_366_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_368_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_370_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_372_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_374_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_376_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_378_options[] = {
    { .name = "adaptive_sync", .value = "false" },
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_381_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_383_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_385_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_387_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_389_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_391_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_393_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_395_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_397_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_399_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_401_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_403_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_405_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_407_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_409_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_411_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_413_options[] = {
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_415_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
    { .name = "vk_x11_override_min_image_count", .value = "2" },
    { .name = "vk_wsi_force_bgra8_unorm_first", .value = "true" },
};

    
static const struct driconf_option application_419_options[] = {
    { .name = "vk_x11_override_min_image_count", .value = "2" },
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_422_options[] = {
    { .name = "vk_x11_override_min_image_count", .value = "3" },
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_425_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_OML_swap_method" },
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_428_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_430_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_432_options[] = {
    { .name = "force_gl_names_reuse", .value = "true" },
};

    
static const struct driconf_option application_434_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_436_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_438_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_440_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_442_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_444_options[] = {
    { .name = "ignore_discard_framebuffer", .value = "true" },
};

    
static const struct driconf_option application_446_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_448_options[] = {
    { .name = "vk_x11_ensure_min_image_count", .value = "true" },
};

    
static const struct driconf_option application_450_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_452_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_454_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_456_options[] = {
    { .name = "vk_dont_care_as_load", .value = "true" },
};

    
static const struct driconf_option application_458_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_460_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_462_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_464_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_466_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_468_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_470_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_472_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_474_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_476_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_478_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_480_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};


static const struct driconf_application device_1_applications[] = {
    { .name = "Akka Arrh",
      .executable = "Project-A.exe",
      .num_options = 1,
      .options = application_2_options,
    },
    { .name = "Unigine Sanctuary",
      .executable = "Sanctuary",
      .num_options = 3,
      .options = application_4_options,
    },
    { .name = "Unigine Tropics",
      .executable = "Tropics",
      .num_options = 3,
      .options = application_8_options,
    },
    { .name = "Unigine Heaven (32-bit)",
      .executable = "heaven_x86",
      .num_options = 2,
      .options = application_12_options,
    },
    { .name = "Unigine Heaven (64-bit)",
      .executable = "heaven_x64",
      .num_options = 2,
      .options = application_15_options,
    },
    { .name = "Unigine Heaven (Windows)",
      .executable = "heaven.exe",
      .num_options = 2,
      .options = application_18_options,
    },
    { .name = "Unigine Valley (32-bit)",
      .executable = "valley_x86",
      .num_options = 2,
      .options = application_21_options,
    },
    { .name = "Unigine Valley (64-bit)",
      .executable = "valley_x64",
      .num_options = 2,
      .options = application_24_options,
    },
    { .name = "Unigine Valley (Windows)",
      .executable = "valley.exe",
      .num_options = 2,
      .options = application_27_options,
    },
    { .name = "Unigine OilRush (32-bit)",
      .executable = "OilRush_x86",
      .num_options = 2,
      .options = application_30_options,
    },
    { .name = "Unigine OilRush (64-bit)",
      .executable = "OilRush_x64",
      .num_options = 2,
      .options = application_33_options,
    },
    { .name = "Savage 2",
      .executable = "savage2.bin",
      .num_options = 1,
      .options = application_36_options,
    },
    { .name = "Topogun (32-bit)",
      .executable = "topogun32",
      .num_options = 1,
      .options = application_38_options,
    },
    { .name = "Topogun (64-bit)",
      .executable = "topogun64",
      .num_options = 1,
      .options = application_40_options,
    },
    { .name = "Half Life 2",
      .executable = "hl2_linux",
      .num_options = 1,
      .options = application_42_options,
    },
    { .name = "Black Mesa",
      .executable = "bms_linux",
      .num_options = 1,
      .options = application_44_options,
    },
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_46_options,
    },
    { .name = "Dead Island (incl. Definitive Edition)",
      .executable = "DeadIslandGame",
      .num_options = 2,
      .options = application_48_options,
    },
    { .name = "Dead Island Riptide Definitive Edition",
      .executable = "DeadIslandRiptideGame",
      .num_options = 1,
      .options = application_51_options,
    },
    { .name = "Doom 3: BFG",
      .executable = "Doom3BFG.exe",
      .num_options = 2,
      .options = application_53_options,
    },
    { .name = "Dune: Spice Wars",
      .executable = "D4X.exe",
      .num_options = 1,
      .options = application_56_options,
    },
    { .name = "The Chronicles of Riddick: Assault on Dark Athena",
      .executable = "DarkAthena.exe",
      .num_options = 3,
      .options = application_58_options,
    },
    { .name = "Dying Light",
      .executable = "DyingLightGame",
      .num_options = 2,
      .options = application_62_options,
    },
    { .name = "Exanima",
      .executable = "Exanima.exe",
      .num_options = 1,
      .options = application_65_options,
    },
    { .name = "Full Bore",
      .executable = "fullbore",
      .num_options = 1,
      .options = application_67_options,
    },
    { .name = "RAGE (64-bit)",
      .executable = "Rage64.exe",
      .num_options = 2,
      .options = application_69_options,
    },
    { .name = "RAGE (32-bit)",
      .executable = "Rage.exe",
      .num_options = 2,
      .options = application_72_options,
    },
    { .name = "Second Life",
      .executable = "do-not-directly-run-secondlife-bin",
      .num_options = 1,
      .options = application_75_options,
    },
    { .name = "Warsow (32-bit)",
      .executable = "warsow.i386",
      .num_options = 1,
      .options = application_77_options,
    },
    { .name = "Warsow (64-bit)",
      .executable = "warsow.x86_64",
      .num_options = 1,
      .options = application_79_options,
    },
    { .name = "Rust",
      .executable = "rust",
      .num_options = 1,
      .options = application_81_options,
    },
    { .name = "Divinity: Original Sin Enhanced Edition",
      .executable = "EoCApp",
      .num_options = 3,
      .options = application_83_options,
    },
    { .name = "Metro 2033 Redux / Metro Last Night Redux",
      .executable = "metro",
      .num_options = 1,
      .options = application_87_options,
    },
    { .name = "Worms W.M.D",
      .executable = "Worms W.M.Dx64",
      .num_options = 1,
      .options = application_89_options,
    },
    { .name = "Crookz - The Big Heist",
      .executable = "Crookz",
      .num_options = 1,
      .options = application_91_options,
    },
    { .name = "Tropico 5",
      .executable = "Tropico5",
      .num_options = 1,
      .options = application_93_options,
    },
    { .name = "Faster than Light (32-bit)",
      .executable = "FTL.x86",
      .num_options = 1,
      .options = application_95_options,
    },
    { .name = "Faster than Light (64-bit)",
      .executable = "FTL.amd64",
      .num_options = 1,
      .options = application_97_options,
    },
    { .name = "Final Fantasy VIII: Remastered",
      .executable = "FFVIII.exe",
      .num_options = 1,
      .options = application_99_options,
    },
    { .name = "SNK HEROINES Tag Team Frenzy",
      .executable = "SNKHEROINES.exe",
      .num_options = 1,
      .options = application_101_options,
    },
    { .name = "Metal Slug XX",
      .executable = "MSXX_main.exe",
      .num_options = 1,
      .options = application_103_options,
    },
    { .name = "The Culling",
      .executable = "Victory",
      .num_options = 1,
      .options = application_105_options,
    },
    { .name = "Spec Ops: The Line (32-bit)",
      .executable = "specops.i386",
      .num_options = 1,
      .options = application_107_options,
    },
    { .name = "Spec Ops: The Line (64-bit)",
      .executable = "specops",
      .num_options = 1,
      .options = application_109_options,
    },
    { .name = "Jamestown+",
      .executable = "JamestownPlus.exe",
      .num_options = 1,
      .options = application_111_options,
    },
    { .name = "Kerbal Space Program (32-bit)",
      .executable = "KSP.x86",
      .num_options = 1,
      .options = application_113_options,
    },
    { .name = "Kerbal Space Program (64-bit)",
      .executable = "KSP.x86_64",
      .num_options = 1,
      .options = application_115_options,
    },
    { .name = "Luna Sky",
      .executable = "lunasky",
      .num_options = 2,
      .options = application_117_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_120_options,
    },
    { .name = "The Witcher 2",
      .executable = "witcher2",
      .num_options = 1,
      .options = application_122_options,
    },
    { .name = "Unreal 4 Editor",
      .executable = "UE4Editor",
      .num_options = 1,
      .options = application_124_options,
    },
    { .name = "Observer",
      .executable = "TheObserver-Linux-Shipping",
      .num_options = 1,
      .options = application_126_options,
    },
    { .name = "Pixel Game Maker MV",
      .executable = "player.exe",
      .num_options = 1,
      .options = application_128_options,
    },
    { .name = "Eden Gamma",
      .executable = "EdenGammaGame-0.3.0.2.exe",
      .num_options = 1,
      .options = application_130_options,
    },
    { .name = "Steamroll",
      .executable = "Steamroll-Linux-Shipping",
      .num_options = 1,
      .options = application_132_options,
    },
    { .name = "Refunct",
      .executable = "Refunct-Linux-Shipping",
      .num_options = 1,
      .options = application_134_options,
    },
    { .name = "We Happy Few",
      .executable = "GlimpseGame",
      .num_options = 1,
      .options = application_136_options,
    },
    { .name = "Google Earth VR",
      .executable = "Earth.exe",
      .num_options = 2,
      .options = application_138_options,
    },
    { .name = "Champions of Regnum",
      .executable = "game",
      .num_options = 1,
      .options = application_141_options,
    },
    { .name = "Wolfenstein The Old Blood",
      .executable = "WolfOldBlood_x64.exe",
      .num_options = 1,
      .options = application_143_options,
    },
    { .name = "ARMA 3",
      .executable = "arma3.x86_64",
      .num_options = 1,
      .options = application_145_options,
    },
    { .name = "Epic Games Launcher",
      .executable = "EpicGamesLauncher.exe",
      .num_options = 1,
      .options = application_147_options,
    },
    { .name = "GpuTest",
      .executable = "GpuTest",
      .num_options = 1,
      .options = application_149_options,
    },
    { .name = "Curse of the Dead Gods",
      .executable = "Curse of the Dead Gods.exe",
      .num_options = 1,
      .options = application_151_options,
    },
    { .name = "GRID Autosport",
      .executable = "GridAutosport",
      .num_options = 1,
      .options = application_153_options,
    },
    { .name = "DIRT: Showdown",
      .executable = "dirt.i386",
      .num_options = 1,
      .options = application_155_options,
    },
    { .name = "DiRT Rally",
      .executable = "DirtRally",
      .num_options = 1,
      .options = application_157_options,
    },
    { .name = "Homerun Clash",
      .executable = "com.haegin.homerunclash",
      .num_options = 1,
      .options = application_159_options,
    },
    { .name = "The Spirit and The Mouse",
      .executable = "TheSpiritAndTheMouse.exe",
      .num_options = 1,
      .options = application_161_options,
    },
    { .name = "Penumbra: Overture",
      .executable = "Penumbra.exe",
      .num_options = 1,
      .options = application_163_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 10,
      .options = application_165_options,
    },
    { .name = "Dead-Cells",
      .executable = "com.playdigious.deadcells.mobile",
      .num_options = 1,
      .options = application_176_options,
    },
    { .name = "Teardown",
      .executable = "teardown.exe",
      .num_options = 2,
      .options = application_178_options,
    },
    { .name = "Golf With Your Friends",
      .executable = "Golf With Your Friends.x86_64",
      .num_options = 1,
      .options = application_181_options,
    },
    { .name = "Cossacks 3",
      .executable = "cossacks.exe",
      .num_options = 1,
      .options = application_183_options,
    },
    { .name = "Kaiju-A-Gogo",
      .executable = "kaiju.exe",
      .num_options = 1,
      .options = application_185_options,
    },
    { .name = "Captain Lycop: Invasion of the Heters (Wine)",
      .executable = "lycop.exe",
      .num_options = 1,
      .options = application_187_options,
    },
    { .name = "Captain Lycop: Invasion of the Heters",
      .executable = "lycop",
      .num_options = 1,
      .options = application_189_options,
    },
    { .name = "BETA CAE Systems - GL detect tool",
      .executable = "detect_opengl_tool",
      .num_options = 1,
      .options = application_191_options,
    },
    { .name = "BETA CAE Systems - ANSA",
      .executable = "ansa_linux_x86_64",
      .num_options = 2,
      .options = application_193_options,
    },
    { .name = "BETA CAE Systems - META",
      .executable = "meta_post_x86_64",
      .num_options = 1,
      .options = application_196_options,
    },
    { .name = "Mari",
      .executable_regexp = "Mari[0-9]+[.][0-9]+v[0-9]+",
      .num_options = 1,
      .options = application_198_options,
    },
    { .name = "Discovery Studio 2020",
      .executable = "DiscoveryStudio2020-bin",
      .num_options = 1,
      .options = application_200_options,
    },
    { .name = "Abaqus",
      .executable = "ABQcaeK",
      .num_options = 2,
      .options = application_202_options,
    },
    { .name = "Maya",
      .executable = "maya.bin",
      .num_options = 1,
      .options = application_205_options,
    },
    { .name = "SD Gundam G Generation Cross Rays",
      .executable = "togg.exe",
      .num_options = 1,
      .options = application_207_options,
    },
    { .name = "FINAL FANTASY XI",
      .executable = "pol.exe",
      .num_options = 1,
      .options = application_209_options,
    },
    { .name = "Minecraft",
      .executable = "minecraft-launcher",
      .num_options = 1,
      .options = application_211_options,
    },
    { .name = "Minecraft-FTB",
      .executable = "ftb-app",
      .num_options = 1,
      .options = application_213_options,
    },
    { .name = "Alien Isolation",
      .executable = "AlienIsolation",
      .num_options = 1,
      .options = application_215_options,
    },
    { .name = "BioShock Infinite",
      .executable = "bioshock.i386",
      .num_options = 1,
      .options = application_217_options,
    },
    { .name = "Borderlands 2",
      .executable = "Borderlands2",
      .num_options = 1,
      .options = application_219_options,
    },
    { .name = "Civilization 5",
      .executable = "Civ5XP",
      .num_options = 1,
      .options = application_221_options,
    },
    { .name = "Civilization 6",
      .executable = "Civ6",
      .num_options = 1,
      .options = application_223_options,
    },
    { .name = "Civilization 6",
      .executable = "Civ6Sub",
      .num_options = 1,
      .options = application_225_options,
    },
    { .name = "Dreamfall Chapters",
      .executable = "Dreamfall Chapters",
      .num_options = 1,
      .options = application_227_options,
    },
    { .name = "Hitman",
      .executable = "HitmanPro",
      .num_options = 1,
      .options = application_229_options,
    },
    { .name = "Renowned Explorers: International Society",
      .executable = "abbeycore_steam",
      .num_options = 1,
      .options = application_231_options,
    },
    { .name = "Saints Row 2",
      .executable = "saintsrow2.i386",
      .num_options = 1,
      .options = application_233_options,
    },
    { .name = "Saints Row: The Third",
      .executable = "SaintsRow3.i386",
      .num_options = 1,
      .options = application_235_options,
    },
    { .name = "Saints Row IV",
      .executable = "SaintsRow4.i386",
      .num_options = 1,
      .options = application_237_options,
    },
    { .name = "Saints Row: Gat out of Hell",
      .executable = "SaintsRow4GooH.i386",
      .num_options = 1,
      .options = application_239_options,
    },
    { .name = "Sid Meier's: Civilization Beyond Earth",
      .executable = "CivBE",
      .num_options = 1,
      .options = application_241_options,
    },
    { .name = "The Witcher 2",
      .executable = "witcher2",
      .num_options = 1,
      .options = application_243_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_245_options,
    },
    { .name = "Euro Truck Simulator 2",
      .executable = "eurotrucks2",
      .num_options = 1,
      .options = application_247_options,
    },
    { .name = "Overlord",
      .executable = "overlord.i386",
      .num_options = 1,
      .options = application_249_options,
    },
    { .name = "Overlord 2",
      .executable = "overlord2.i386",
      .num_options = 1,
      .options = application_251_options,
    },
    { .name = "Oil Rush",
      .executable = "OilRush_x86",
      .num_options = 1,
      .options = application_253_options,
    },
    { .name = "War Thunder",
      .executable = "aces",
      .num_options = 1,
      .options = application_255_options,
    },
    { .name = "War Thunder (Wine)",
      .executable = "aces.exe",
      .num_options = 1,
      .options = application_257_options,
    },
    { .name = "Outlast",
      .executable = "OLGame.x86_64",
      .num_options = 1,
      .options = application_259_options,
    },
    { .name = "Spec Ops: The Line (32-bit)",
      .executable = "specops.i386",
      .num_options = 1,
      .options = application_261_options,
    },
    { .name = "Spec Ops: The Line (64-bit)",
      .executable = "specops",
      .num_options = 1,
      .options = application_263_options,
    },
    { .name = "Mount and Blade Warband",
      .executable = "mb_warband_linux",
      .num_options = 1,
      .options = application_265_options,
    },
    { .name = "Valheim",
      .executable = "valheim.x86_64",
      .num_options = 1,
      .options = application_267_options,
    },
    { .name = "Medieval II: Total War",
      .executable = "Medieval2",
      .num_options = 1,
      .options = application_269_options,
    },
    { .name = "Carnivores: Dinosaur Hunter Reborn (wine)",
      .executable = "Carnivores-master.exe",
      .num_options = 1,
      .options = application_271_options,
    },
    { .name = "Far Cry 2 (wine)",
      .executable = "farcry2.exe",
      .num_options = 1,
      .options = application_273_options,
    },
    { .name = "Talos Principle",
      .executable = "Talos",
      .num_options = 2,
      .options = application_275_options,
    },
    { .name = "Talos Principle (Unrestricted)",
      .executable = "Talos_Unrestricted",
      .num_options = 2,
      .options = application_278_options,
    },
    { .name = "Serious Sam Fusion",
      .executable = "Sam2017",
      .num_options = 1,
      .options = application_281_options,
    },
    { .name = "Serious Sam Fusion (Unrestricted)",
      .executable = "Sam2017_Unrestricted",
      .num_options = 1,
      .options = application_283_options,
    },
    { .name = "7 Days To Die (64-bit)",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_285_options,
    },
    { .name = "7 Days To Die (32-bit)",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_287_options,
    },
    { .name = "Dolphin Emulator",
      .executable = "dolphin-emu",
      .num_options = 1,
      .options = application_289_options,
    },
    { .name = "Citra - Nintendo 3DS Emulator",
      .executable = "citra-qt",
      .num_options = 1,
      .options = application_291_options,
    },
    { .name = "Yuzu - Nintendo Switch Emulator",
      .executable = "yuzu",
      .num_options = 1,
      .options = application_293_options,
    },
    { .name = "RPCS3",
      .executable = "rpcs3",
      .num_options = 1,
      .options = application_295_options,
    },
    { .name = "PCSX2",
      .executable = "PCSX2",
      .num_options = 1,
      .options = application_297_options,
    },
    { .name = "From The Depths",
      .executable = "From_The_Depths.x86_64",
      .num_options = 1,
      .options = application_299_options,
    },
    { .name = "Plague Inc Evolved (32-bit)",
      .executable = "PlagueIncEvolved.x86",
      .num_options = 1,
      .options = application_301_options,
    },
    { .name = "Plague Inc Evolved (64-bit)",
      .executable = "PlagueIncEvolved.x86_64",
      .num_options = 1,
      .options = application_303_options,
    },
    { .name = "Beholder (32-bit)",
      .executable = "Beholder.x86",
      .num_options = 1,
      .options = application_305_options,
    },
    { .name = "Beholder (64-bit)",
      .executable = "Beholder.x86_64",
      .num_options = 1,
      .options = application_307_options,
    },
    { .name = "X3 Reunion",
      .executable = "X3R_main",
      .num_options = 1,
      .options = application_309_options,
    },
    { .name = "X3 Terran Conflict",
      .executable = "X3TR_main",
      .num_options = 1,
      .options = application_311_options,
    },
    { .name = "X3 Albion Prelude",
      .executable = "X3AP_main",
      .num_options = 1,
      .options = application_313_options,
    },
    { .name = "Borderlands: The Pre-Sequel",
      .executable = "BorderlandsPreSequel",
      .num_options = 1,
      .options = application_315_options,
    },
    { .name = "Transport Fever",
      .executable = "TransportFever",
      .num_options = 1,
      .options = application_317_options,
    },
    { .name = "Unigine Sanctuary",
      .executable = "Sanctuary",
      .num_options = 1,
      .options = application_319_options,
    },
    { .name = "Unigine Tropics",
      .executable = "Tropics",
      .num_options = 1,
      .options = application_321_options,
    },
    { .name = "Unigine Heaven (32-bit)",
      .executable = "heaven_x86",
      .num_options = 1,
      .options = application_323_options,
    },
    { .name = "Unigine Heaven (64-bit)",
      .executable = "heaven_x64",
      .num_options = 1,
      .options = application_325_options,
    },
    { .name = "Unigine Valley (32-bit)",
      .executable = "valley_x86",
      .num_options = 1,
      .options = application_327_options,
    },
    { .name = "Unigine Valley (64-bit)",
      .executable = "valley_x64",
      .num_options = 1,
      .options = application_329_options,
    },
    { .name = "Unigine Superposition",
      .executable = "superposition",
      .num_options = 1,
      .options = application_331_options,
    },
    { .name = "Basemark GPU",
      .executable = "BasemarkGPU_gl",
      .num_options = 1,
      .options = application_333_options,
    },
    { .name = "Stellaris",
      .executable = "stellaris",
      .num_options = 1,
      .options = application_335_options,
    },
    { .name = "Battletech",
      .executable = "BattleTech",
      .num_options = 1,
      .options = application_337_options,
    },
    { .name = "DeusExMD",
      .executable = "DeusExMD",
      .num_options = 1,
      .options = application_339_options,
    },
    { .name = "F1 2015",
      .executable = "F12015",
      .num_options = 1,
      .options = application_341_options,
    },
    { .name = "KWin Wayland",
      .executable = "kwin_wayland",
      .num_options = 1,
      .options = application_343_options,
    },
    { .name = "gnome-shell",
      .executable = "gnome-shell",
      .num_options = 2,
      .options = application_345_options,
    },
    { .name = "Desktop — Plasma",
      .executable = "plasmashell",
      .num_options = 1,
      .options = application_348_options,
    },
    { .name = "budgie-wm",
      .executable = "budgie-wm",
      .num_options = 1,
      .options = application_350_options,
    },
    { .name = "kwin_x11",
      .executable = "kwin_x11",
      .num_options = 1,
      .options = application_352_options,
    },
    { .name = "ksmserver-logout-greeter",
      .executable = "ksmserver-logout-greeter",
      .num_options = 1,
      .options = application_354_options,
    },
    { .name = "ksmserver-switchuser-greeter",
      .executable = "ksmserver-switchuser-greeter",
      .num_options = 1,
      .options = application_356_options,
    },
    { .name = "kscreenlocker_greet",
      .executable = "kscreenlocker_greet",
      .num_options = 1,
      .options = application_358_options,
    },
    { .name = "startplasma",
      .executable = "startplasma",
      .num_options = 1,
      .options = application_360_options,
    },
    { .name = "sddm-greeter",
      .executable = "sddm-greeter",
      .num_options = 1,
      .options = application_362_options,
    },
    { .name = "krunner",
      .executable = "krunner",
      .num_options = 1,
      .options = application_364_options,
    },
    { .name = "spectacle",
      .executable = "spectacle",
      .num_options = 1,
      .options = application_366_options,
    },
    { .name = "marco",
      .executable = "marco",
      .num_options = 1,
      .options = application_368_options,
    },
    { .name = "compton",
      .executable = "compton",
      .num_options = 1,
      .options = application_370_options,
    },
    { .name = "picom",
      .executable = "picom",
      .num_options = 1,
      .options = application_372_options,
    },
    { .name = "xfwm4",
      .executable = "xfwm4",
      .num_options = 1,
      .options = application_374_options,
    },
    { .name = "Enlightenment",
      .executable = "enlightenment",
      .num_options = 1,
      .options = application_376_options,
    },
    { .name = "mutter",
      .executable = "mutter",
      .num_options = 2,
      .options = application_378_options,
    },
    { .name = "muffin",
      .executable = "muffin",
      .num_options = 1,
      .options = application_381_options,
    },
    { .name = "cinnamon",
      .executable = "cinnamon",
      .num_options = 1,
      .options = application_383_options,
    },
    { .name = "compiz",
      .executable = "compiz",
      .num_options = 1,
      .options = application_385_options,
    },
    { .name = "Firefox",
      .executable = "firefox",
      .num_options = 1,
      .options = application_387_options,
    },
    { .name = "Firefox ESR",
      .executable = "firefox-esr",
      .num_options = 1,
      .options = application_389_options,
    },
    { .name = "Chromium",
      .executable = "chromium",
      .num_options = 1,
      .options = application_391_options,
    },
    { .name = "Google Chrome",
      .executable = "chrome",
      .num_options = 1,
      .options = application_393_options,
    },
    { .name = "Iceweasel",
      .executable = "iceweasel",
      .num_options = 1,
      .options = application_395_options,
    },
    { .name = "Epiphany",
      .executable = "epiphany",
      .num_options = 1,
      .options = application_397_options,
    },
    { .name = "Konqueror",
      .executable = "konqueror",
      .num_options = 1,
      .options = application_399_options,
    },
    { .name = "Falkon",
      .executable = "falkon",
      .num_options = 1,
      .options = application_401_options,
    },
    { .name = "Seamonkey",
      .executable = "seamonkey",
      .num_options = 1,
      .options = application_403_options,
    },
    { .name = "Waterfox",
      .executable = "waterfox",
      .num_options = 1,
      .options = application_405_options,
    },
    { .name = "VLC Media Player",
      .executable = "vlc",
      .num_options = 1,
      .options = application_407_options,
    },
    { .name = "Totem",
      .executable = "totem",
      .num_options = 1,
      .options = application_409_options,
    },
    { .name = "Dragon Player",
      .executable = "dragon",
      .num_options = 1,
      .options = application_411_options,
    },
    { .name = "Xorg",
      .executable = "Xorg",
      .num_options = 1,
      .options = application_413_options,
    },
    { .name = "gfxbench",
      .executable = "testfw_app",
      .num_options = 3,
      .options = application_415_options,
    },
    { .name = "Rainbow Six Extraction (Wine)",
      .executable = "R6-Extraction.exe",
      .num_options = 2,
      .options = application_419_options,
    },
    { .name = "Hades",
      .executable = "Hades.exe",
      .num_options = 2,
      .options = application_422_options,
    },
    { .name = "Brink",
      .executable = "brink.exe",
      .num_options = 2,
      .options = application_425_options,
    },
    { .name = "Enter The Gungeon (32 bits)",
      .executable = "EtG.x86",
      .num_options = 1,
      .options = application_428_options,
    },
    { .name = "Enter The Gungeon (64 bits)",
      .executable = "EtG.x86_64",
      .num_options = 1,
      .options = application_430_options,
    },
    { .name = "SWKOTOR (wine)",
      .executable = "swkotor.exe",
      .num_options = 1,
      .options = application_432_options,
    },
    { .name = "Assault Android Cactus (32-bit)",
      .executable = "cactus_demo.x86",
      .num_options = 1,
      .options = application_434_options,
    },
    { .name = "Assault Android Cactus (64-bit)",
      .executable = "cactus_demo.x86_64",
      .num_options = 1,
      .options = application_436_options,
    },
    { .name = "Assault Android Cactus (32-bit)",
      .executable = "cactus.x86",
      .num_options = 1,
      .options = application_438_options,
    },
    { .name = "Assault Android Cactus (64-bit)",
      .executable = "cactus.x86_64",
      .num_options = 1,
      .options = application_440_options,
    },
    { .name = "Limbo",
      .executable = "limbo",
      .num_options = 1,
      .options = application_442_options,
    },
    { .name = "Genshin Impact",
      .executable = "com.miHoYo.GenshinImpact",
      .num_options = 1,
      .options = application_444_options,
    },
    { .name = "DOOM",
      .executable = "DOOMx64vk.exe",
      .num_options = 1,
      .options = application_446_options,
    },
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 1,
      .options = application_448_options,
    },
    { .name = "Wolfenstein: Youngblood(x64vk)",
      .executable = "Youngblood_x64vk.exe",
      .num_options = 1,
      .options = application_450_options,
    },
    { .name = "Wolfenstein II: The New Colossus",
      .executable = "NewColossus_x64vk.exe",
      .num_options = 1,
      .options = application_452_options,
    },
    { .name = "Metro: Exodus",
      .application_name_match = "metroexodus",
      .num_options = 1,
      .options = application_454_options,
    },
    { .name = "Forsaken Remastered",
      .executable = "ForsakenEx",
      .num_options = 1,
      .options = application_456_options,
    },
    { .name = "Atlas Fallen",
      .executable = "AtlasFallen (VK).exe",
      .num_options = 1,
      .options = application_458_options,
    },
    { .name = "Firefox",
      .executable = "firefox",
      .num_options = 1,
      .options = application_460_options,
    },
    { .name = "Firefox ESR",
      .executable = "firefox-esr",
      .num_options = 1,
      .options = application_462_options,
    },
    { .name = "Chromium",
      .executable = "chromium",
      .num_options = 1,
      .options = application_464_options,
    },
    { .name = "Google Chrome",
      .executable = "chrome",
      .num_options = 1,
      .options = application_466_options,
    },
    { .name = "Iceweasel",
      .executable = "iceweasel",
      .num_options = 1,
      .options = application_468_options,
    },
    { .name = "Epiphany",
      .executable = "epiphany",
      .num_options = 1,
      .options = application_470_options,
    },
    { .name = "Konqueror",
      .executable = "konqueror",
      .num_options = 1,
      .options = application_472_options,
    },
    { .name = "Falkon",
      .executable = "falkon",
      .num_options = 1,
      .options = application_474_options,
    },
    { .name = "Seamonkey",
      .executable = "seamonkey",
      .num_options = 1,
      .options = application_476_options,
    },
    { .name = "Waterfox",
      .executable = "waterfox",
      .num_options = 1,
      .options = application_478_options,
    },
    { .name = "Detroit Become Human",
      .application_name_match = "DetroitBecomeHuman",
      .num_options = 1,
      .options = application_480_options,
    },
};

static const struct driconf_device device_1 = {
    .num_engines = 1,
    .engines = device_1_engines,
    .num_applications = 218,
    .applications = device_1_applications,
};


    
static const struct driconf_option application_485_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_EXT_buffer_age -GLX_OML_sync_control -GLX_SGI_video_sync" },
};

    
static const struct driconf_option application_487_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_EXT_buffer_age -GLX_OML_sync_control -GLX_SGI_video_sync" },
};

    
static const struct driconf_option application_489_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_EXT_buffer_age -GLX_OML_sync_control" },
};


static const struct driconf_application device_484_applications[] = {
    { .name = "gnome-shell",
      .executable = "gnome-shell",
      .num_options = 1,
      .options = application_485_options,
    },
    { .name = "cinnamon",
      .executable = "cinnamon",
      .num_options = 1,
      .options = application_487_options,
    },
    { .name = "Compiz",
      .executable = "Compiz",
      .num_options = 1,
      .options = application_489_options,
    },
};

static const struct driconf_device device_484 = {
    .driver = "vmwgfx",
    .num_engines = 0,
    .num_applications = 3,
    .applications = device_484_applications,
};


    
static const struct driconf_option application_492_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_494_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_496_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_499_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_501_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_503_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_505_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_507_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_510_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_512_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_514_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_516_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_518_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_520_options[] = {
    { .name = "radeonsi_no_infinite_interp", .value = "true" },
};

    
static const struct driconf_option application_522_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_524_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_526_options[] = {
    { .name = "glthread_nop_check_framebuffer_status", .value = "true" },
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_529_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_531_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_533_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
    { .name = "radeonsi_no_infinite_interp", .value = "true" },
};

    
static const struct driconf_option application_536_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_538_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_540_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_542_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
    { .name = "radeonsi_force_use_fma32", .value = "true" },
};

    
static const struct driconf_option application_545_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};


static const struct driconf_application device_491_applications[] = {
    { .name = "Alien Isolation",
      .executable = "AlienIsolation",
      .num_options = 1,
      .options = application_492_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_494_options,
    },
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 2,
      .options = application_496_options,
    },
    { .name = "Exanima",
      .executable = "Exanima.exe",
      .num_options = 1,
      .options = application_499_options,
    },
    { .name = "Nowhere Patrol",
      .executable = "NowherePatrol.exe",
      .num_options = 1,
      .options = application_501_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_503_options,
    },
    { .name = "Gfx Bench",
      .executable = "gfxbench_gl",
      .num_options = 1,
      .options = application_505_options,
    },
    { .name = "Monolith demo5",
      .executable = "runner",
      .sha1 = "0c40d509a74e357f0280cb1bd882e9cd94b91bdf",
      .num_options = 2,
      .options = application_507_options,
    },
    { .name = "Memoranda / Riptale",
      .executable = "runner",
      .sha1 = "aa13dec6af63c88f308ebb487693896434a4db56",
      .num_options = 1,
      .options = application_510_options,
    },
    { .name = "Nuclear Throne",
      .executable = "runner",
      .sha1 = "84814e8db125e889f5d9d4195a0ca72a871ea1fd",
      .num_options = 1,
      .options = application_512_options,
    },
    { .name = "Undertale",
      .executable = "runner",
      .sha1 = "dfa302e7ec78641d0696dbbc1a06fc29f34ff1ff",
      .num_options = 1,
      .options = application_514_options,
    },
    { .name = "Turmoil",
      .executable = "runner",
      .sha1 = "cbbf757aaab289859f8dae191a7d63afc30643d9",
      .num_options = 1,
      .options = application_516_options,
    },
    { .name = "Peace, Death!",
      .executable = "runner",
      .sha1 = "5b909f3d21799773370adf084f649848f098234e",
      .num_options = 1,
      .options = application_518_options,
    },
    { .name = "Kerbal Space Program",
      .executable = "KSP.x86_64",
      .num_options = 1,
      .options = application_520_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_522_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_524_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 2,
      .options = application_526_options,
    },
    { .name = "Road Redemption",
      .executable = "RoadRedemption.x86_64",
      .num_options = 1,
      .options = application_529_options,
    },
    { .name = "Wasteland 2",
      .executable = "WL2",
      .num_options = 1,
      .options = application_531_options,
    },
    { .name = "Teardown",
      .executable = "teardown.exe",
      .num_options = 2,
      .options = application_533_options,
    },
    { .name = "SpaceEngine",
      .executable = "SpaceEngine.exe",
      .num_options = 1,
      .options = application_536_options,
    },
    { .name = "BETA CAE Systems - GL detect tool",
      .executable = "detect_opengl_tool",
      .num_options = 1,
      .options = application_538_options,
    },
    { .name = "BETA CAE Systems - ANSA",
      .executable = "ansa_linux_x86_64",
      .num_options = 1,
      .options = application_540_options,
    },
    { .name = "BETA CAE Systems - META",
      .executable = "meta_post_x86_64",
      .num_options = 2,
      .options = application_542_options,
    },
    { .name = "Black Geyser: Couriers of Darkness",
      .executable = "BlackGeyser.x86_64",
      .num_options = 1,
      .options = application_545_options,
    },
};

static const struct driconf_device device_491 = {
    .driver = "radeonsi",
    .num_engines = 0,
    .num_applications = 25,
    .applications = device_491_applications,
};


    
static const struct driconf_option application_548_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
};

    
static const struct driconf_option application_550_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_552_options[] = {
    { .name = "zink_emulate_point_smooth", .value = "true" },
};

    
static const struct driconf_option application_554_options[] = {
    { .name = "zink_emulate_point_smooth", .value = "true" },
};

    
static const struct driconf_option application_556_options[] = {
    { .name = "zink_emulate_point_smooth", .value = "true" },
};

    
static const struct driconf_option application_558_options[] = {
    { .name = "zink_shader_object_enable", .value = "true" },
};


static const struct driconf_application device_547_applications[] = {
    { .name = "Hyperdimension Neptunia Re;Birth1",
      .executable = "NeptuniaReBirth1.exe",
      .num_options = 1,
      .options = application_548_options,
    },
    { .name = "Borderlands 2 (Native, OpenGL, 32bit)",
      .executable = "Borderlands2",
      .num_options = 1,
      .options = application_550_options,
    },
    { .name = "Quake II",
      .executable = "quake2-engine",
      .num_options = 1,
      .options = application_552_options,
    },
    { .name = "Quake II (yamagi)",
      .executable = "yamagi-quake2",
      .num_options = 1,
      .options = application_554_options,
    },
    { .name = "Quake II (wine)",
      .executable = "quake2.exe",
      .num_options = 1,
      .options = application_556_options,
    },
    { .name = "Tomb Raider 2013",
      .executable = "TombRaider",
      .num_options = 1,
      .options = application_558_options,
    },
};

static const struct driconf_device device_547 = {
    .driver = "zink",
    .num_engines = 0,
    .num_applications = 6,
    .applications = device_547_applications,
};


    
static const struct driconf_option application_561_options[] = {
    { .name = "vs_position_always_invariant", .value = "true" },
};

    
static const struct driconf_option application_563_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_565_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_567_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_569_options[] = {
    { .name = "mesa_extension_override", .value = "+GL_EXT_shader_image_load_store" },
};


static const struct driconf_application device_560_applications[] = {
    { .name = "Middle Earth: Shadow of Mordor",
      .executable = "ShadowOfMordor",
      .num_options = 1,
      .options = application_561_options,
    },
    { .name = "glmark2",
      .executable = "glmark2",
      .num_options = 1,
      .options = application_563_options,
    },
    { .name = "Counter-Strike: Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_565_options,
    },
    { .name = "Insurgency",
      .executable = "insurgency_linux",
      .num_options = 1,
      .options = application_567_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 1,
      .options = application_569_options,
    },
};

static const struct driconf_device device_560 = {
    .driver = "iris",
    .num_engines = 0,
    .num_applications = 5,
    .applications = device_560_applications,
};


    
static const struct driconf_option application_572_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_574_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_576_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};


static const struct driconf_application device_571_applications[] = {
    { .name = "glmark2",
      .executable = "glmark2",
      .num_options = 1,
      .options = application_572_options,
    },
    { .name = "Counter-Strike: Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_574_options,
    },
    { .name = "Insurgency",
      .executable = "insurgency_linux",
      .num_options = 1,
      .options = application_576_options,
    },
};

static const struct driconf_device device_571 = {
    .driver = "crocus",
    .num_engines = 0,
    .num_applications = 3,
    .applications = device_571_applications,
};
    
static const struct driconf_option engine_616_options[] = {
    { .name = "no_16bit", .value = "true" },
};

    
static const struct driconf_option engine_618_options[] = {
    { .name = "anv_disable_fcv", .value = "true" },
    { .name = "anv_assume_full_subgroups", .value = "16" },
};


static const struct driconf_engine device_578_engines[] = {
    { .engine_name_match = "mesa zink",
      .num_options = 1,
      .options = engine_616_options,
    },
    { .engine_name_match = "UnrealEngine5.1",
      .num_options = 2,
      .options = engine_618_options,
    },
};

    
static const struct driconf_option application_579_options[] = {
    { .name = "anv_assume_full_subgroups", .value = "32" },
};

    
static const struct driconf_option application_581_options[] = {
    { .name = "anv_assume_full_subgroups", .value = "32" },
    { .name = "fp64_workaround_enabled", .value = "true" },
};

    
static const struct driconf_option application_584_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_586_options[] = {
    { .name = "anv_sample_mask_out_opengl_behaviour", .value = "true" },
};

    
static const struct driconf_option application_588_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_590_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_592_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_594_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_596_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_598_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_600_options[] = {
    { .name = "shader_spilling_rate", .value = "15" },
};

    
static const struct driconf_option application_602_options[] = {
    { .name = "fake_sparse", .value = "true" },
};

    
static const struct driconf_option application_604_options[] = {
    { .name = "anv_force_filter_addr_rounding", .value = "true" },
};

    
static const struct driconf_option application_606_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_608_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_610_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_612_options[] = {
    { .name = "anv_disable_fcv", .value = "true" },
};

    
static const struct driconf_option application_614_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};


static const struct driconf_application device_578_applications[] = {
    { .name = "Aperture Desk Job",
      .executable = "deskjob",
      .num_options = 1,
      .options = application_579_options,
    },
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 2,
      .options = application_581_options,
    },
    { .name = "Wolfenstein: Youngblood(x64vk)",
      .executable = "Youngblood_x64vk.exe",
      .num_options = 1,
      .options = application_584_options,
    },
    { .name = "Batman™: Arkham Knight",
      .executable = "BatmanAK.exe",
      .num_options = 1,
      .options = application_586_options,
    },
    { .name = "Rise of the Tomb Raider",
      .executable = "RiseOfTheTombRaider",
      .num_options = 1,
      .options = application_588_options,
    },
    { .name = "Rise of the Tomb Raider",
      .executable = "ROTTR.exe",
      .num_options = 1,
      .options = application_590_options,
    },
    { .name = "NieR Replicant ver.1.22474487139",
      .executable = "NieR Replicant ver.1.22474487139.exe",
      .num_options = 1,
      .options = application_592_options,
    },
    { .name = "NieR:Automata",
      .executable = "NieRAutomata.exe",
      .num_options = 1,
      .options = application_594_options,
    },
    { .name = "Valheim",
      .executable = "valheim.x86_64",
      .num_options = 1,
      .options = application_596_options,
    },
    { .name = "Cyberpunk 2077",
      .executable = "Cyberpunk2077.exe",
      .num_options = 1,
      .options = application_598_options,
    },
    { .name = "Cyberpunk 2077",
      .executable = "Cyberpunk2077.exe",
      .num_options = 1,
      .options = application_600_options,
    },
    { .name = "Elden Ring",
      .executable = "eldenring.exe",
      .num_options = 1,
      .options = application_602_options,
    },
    { .name = "Age of Empires IV",
      .executable = "RelicCardinal.exe",
      .num_options = 1,
      .options = application_604_options,
    },
    { .name = "Marvel's Spider-Man Remastered",
      .executable = "Spider-Man.exe",
      .num_options = 1,
      .options = application_606_options,
    },
    { .name = "Hogwarts Legacy",
      .executable = "HogwartsLegacy.exe",
      .num_options = 1,
      .options = application_608_options,
    },
    { .name = "DEATH STRANDING",
      .executable = "ds.exe",
      .num_options = 1,
      .options = application_610_options,
    },
    { .name = "Baldur's Gate 3",
      .executable = "bg3.exe",
      .num_options = 1,
      .options = application_612_options,
    },
    { .name = "The Finals",
      .executable = "Discovery.exe",
      .num_options = 1,
      .options = application_614_options,
    },
};

static const struct driconf_device device_578 = {
    .driver = "anv",
    .num_engines = 2,
    .engines = device_578_engines,
    .num_applications = 18,
    .applications = device_578_applications,
};


    
static const struct driconf_option application_622_options[] = {
    { .name = "dzn_enable_8bit_loads_stores", .value = "true" },
    { .name = "dzn_claim_wide_lines", .value = "true" },
};

    
static const struct driconf_option application_625_options[] = {
    { .name = "dzn_claim_wide_lines", .value = "true" },
};


static const struct driconf_application device_621_applications[] = {
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 2,
      .options = application_622_options,
    },
    { .name = "No Man's Sky",
      .executable = "NMS.exe",
      .num_options = 1,
      .options = application_625_options,
    },
};

static const struct driconf_device device_621 = {
    .driver = "dzn",
    .num_engines = 0,
    .num_applications = 2,
    .applications = device_621_applications,
};


    
static const struct driconf_option application_628_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_ARB_shader_image_load_store" },
};


static const struct driconf_application device_627_applications[] = {
    { .name = "Tomb Raider 2013",
      .executable = "TombRaider",
      .num_options = 1,
      .options = application_628_options,
    },
};

static const struct driconf_device device_627 = {
    .driver = "r600",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_627_applications,
};


    
static const struct driconf_option application_631_options[] = {
    { .name = "format_l8_srgb_enable_readback", .value = "true" },
};


static const struct driconf_application device_630_applications[] = {
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_631_options,
    },
};

static const struct driconf_device device_630 = {
    .driver = "virtio_gpu",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_630_applications,
};


    
static const struct driconf_option application_634_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 618" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_637_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 630" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_640_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 618" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_643_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 630" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};


static const struct driconf_application device_633_applications[] = {
    { .name = "Call of Duty Mobile",
      .executable = "com.activision.callofduty.shooter",
      .num_options = 2,
      .options = application_634_options,
    },
    { .name = "Asphalt 8",
      .executable = "com.gameloft.android.ANMP.GloftA8HM",
      .num_options = 2,
      .options = application_637_options,
    },
    { .name = "Asphalt 9",
      .executable = "com.gameloft.android.ANMP.GloftA9HM",
      .num_options = 2,
      .options = application_640_options,
    },
    { .name = "PUBG Mobile",
      .executable = "com.tencent.ig",
      .num_options = 2,
      .options = application_643_options,
    },
};

static const struct driconf_device device_633 = {
    .driver = "msm",
    .device = "FD618",
    .num_engines = 0,
    .num_applications = 4,
    .applications = device_633_applications,
};
    
static const struct driconf_option engine_711_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_disable_aniso_single_level", .value = "true" },
    { .name = "radv_disable_trunc_coord", .value = "true" },
    { .name = "radv_dgc", .value = "true" },
};

    
static const struct driconf_option engine_716_options[] = {
    { .name = "radv_disable_aniso_single_level", .value = "true" },
    { .name = "radv_disable_trunc_coord", .value = "true" },
};

    
static const struct driconf_option engine_719_options[] = {
    { .name = "radv_disable_aniso_single_level", .value = "true" },
};

    
static const struct driconf_option engine_721_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_lower_discard_to_demote", .value = "true" },
    { .name = "radv_disable_tc_compat_htile_general", .value = "true" },
};

    
static const struct driconf_option engine_725_options[] = {
    { .name = "radv_flush_before_query_copy", .value = "true" },
    { .name = "radv_flush_before_timestamp_write", .value = "true" },
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option engine_729_options[] = {
    { .name = "radv_tex_non_uniform", .value = "true" },
};


static const struct driconf_engine device_646_engines[] = {
    { .engine_name_match = "vkd3d",
      .num_options = 4,
      .options = engine_711_options,
    },
    { .engine_name_match = "DXVK",
      .num_options = 2,
      .options = engine_716_options,
    },
    { .engine_name_match = "mesa zink",
      .num_options = 1,
      .options = engine_719_options,
    },
    { .engine_name_match = "Quantic Dream Engine",
      .num_options = 3,
      .options = engine_721_options,
    },
    { .engine_name_match = "^UnrealEngine",
      .num_options = 3,
      .options = engine_725_options,
    },
    { .engine_name_match = "DXVK_NvRemix",
      .num_options = 1,
      .options = engine_729_options,
    },
};

    
static const struct driconf_option application_647_options[] = {
    { .name = "radv_report_llvm9_version_string", .value = "true" },
    { .name = "radv_invariant_geom", .value = "true" },
};

    
static const struct driconf_option application_650_options[] = {
    { .name = "radv_invariant_geom", .value = "true" },
    { .name = "radv_split_fma", .value = "true" },
};

    
static const struct driconf_option application_653_options[] = {
    { .name = "radv_enable_mrt_output_nan_fixup", .value = "true" },
    { .name = "radv_app_layer", .value = "rage2" },
};

    
static const struct driconf_option application_656_options[] = {
    { .name = "radv_disable_shrink_image_store", .value = "true" },
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_659_options[] = {
    { .name = "radv_override_uniform_offset_alignment", .value = "16" },
    { .name = "radv_disable_shrink_image_store", .value = "true" },
    { .name = "radv_invariant_geom", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_664_options[] = {
    { .name = "radv_no_dynamic_bounds", .value = "true" },
};

    
static const struct driconf_option application_666_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_force_active_accel_struct_leaves", .value = "true" },
};

    
static const struct driconf_option application_669_options[] = {
    { .name = "radv_lower_discard_to_demote", .value = "true" },
};

    
static const struct driconf_option application_671_options[] = {
    { .name = "radv_disable_dcc", .value = "true" },
};

    
static const struct driconf_option application_673_options[] = {
    { .name = "radv_disable_dcc", .value = "true" },
};

    
static const struct driconf_option application_675_options[] = {
    { .name = "radv_disable_sinking_load_input_fs", .value = "true" },
};

    
static const struct driconf_option application_677_options[] = {
    { .name = "radv_disable_sinking_load_input_fs", .value = "true" },
};

    
static const struct driconf_option application_679_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_681_options[] = {
    { .name = "radv_enable_unified_heap_on_apu", .value = "true" },
};

    
static const struct driconf_option application_683_options[] = {
    { .name = "radv_app_layer", .value = "metroexodus" },
};

    
static const struct driconf_option application_685_options[] = {
    { .name = "radv_rt_wave64", .value = "true" },
};

    
static const struct driconf_option application_687_options[] = {
    { .name = "radv_invariant_geom", .value = "true" },
};

    
static const struct driconf_option application_689_options[] = {
    { .name = "radv_ssbo_non_uniform", .value = "true" },
};

    
static const struct driconf_option application_691_options[] = {
    { .name = "radv_ssbo_non_uniform", .value = "true" },
};

    
static const struct driconf_option application_693_options[] = {
    { .name = "radv_force_active_accel_struct_leaves", .value = "true" },
};

    
static const struct driconf_option application_695_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_697_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_699_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_701_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_703_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_705_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_707_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_709_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};


static const struct driconf_application device_646_applications[] = {
    { .name = "Shadow Of The Tomb Raider (Native)",
      .application_name_match = "ShadowOfTheTomb",
      .num_options = 2,
      .options = application_647_options,
    },
    { .name = "Shadow Of The Tomb Raider (DX11/DX12)",
      .application_name_match = "SOTTR.exe",
      .num_options = 2,
      .options = application_650_options,
    },
    { .name = "RAGE 2",
      .executable = "RAGE2.exe",
      .num_options = 2,
      .options = application_653_options,
    },
    { .name = "The Surge 2",
      .application_name_match = "Fledge",
      .num_options = 2,
      .options = application_656_options,
    },
    { .name = "World War Z (and World War Z: Aftermath)",
      .application_name_match = "WWZ|wwz",
      .num_options = 4,
      .options = application_659_options,
    },
    { .name = "DOOM VFR",
      .application_name_match = "DOOM_VFR",
      .num_options = 1,
      .options = application_664_options,
    },
    { .name = "DOOM Eternal",
      .application_name_match = "DOOMEternal",
      .num_options = 2,
      .options = application_666_options,
    },
    { .name = "No Man's Sky",
      .application_name_match = "No Man's Sky",
      .num_options = 1,
      .options = application_669_options,
    },
    { .name = "DOOM (2016)",
      .application_name_match = "DOOM$",
      .num_options = 1,
      .options = application_671_options,
    },
    { .name = "Wolfenstein II",
      .application_name_match = "Wolfenstein II The New Colossus",
      .num_options = 1,
      .options = application_673_options,
    },
    { .name = "Grid Autosport",
      .application_name_match = "GRIDAutosport_avx.exe",
      .num_options = 1,
      .options = application_675_options,
    },
    { .name = "Grid Autosport",
      .application_name_match = "GRIDAutosport.exe",
      .num_options = 1,
      .options = application_677_options,
    },
    { .name = "Hammerting",
      .application_name_match = "TDTD",
      .num_options = 1,
      .options = application_679_options,
    },
    { .name = "RDR2",
      .application_name_match = "Red Dead Redemption 2",
      .num_options = 1,
      .options = application_681_options,
    },
    { .name = "Metro Exodus (Linux native)",
      .application_name_match = "metroexodus",
      .num_options = 1,
      .options = application_683_options,
    },
    { .name = "Hellblade: Senua's Sacrifice",
      .application_name_match = "HellbladeGame-Win64-Shipping.exe",
      .num_options = 1,
      .options = application_685_options,
    },
    { .name = "War Thunder",
      .executable = "aces",
      .num_options = 1,
      .options = application_687_options,
    },
    { .name = "Crysis 2 Remastered",
      .executable = "Crysis2Remastered.exe",
      .num_options = 1,
      .options = application_689_options,
    },
    { .name = "Crysis 3 Remastered",
      .executable = "Crysis3Remastered.exe",
      .num_options = 1,
      .options = application_691_options,
    },
    { .name = "Star Wars: Jedi Survivor",
      .executable = "JediSurvivor.exe",
      .num_options = 1,
      .options = application_693_options,
    },
    { .name = "Black Geyser: Couriers of Darkness",
      .executable = "BlackGeyser.x86_64",
      .num_options = 1,
      .options = application_695_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_697_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_699_options,
    },
    { .name = "Monolith demo5",
      .executable = "runner",
      .sha1 = "0c40d509a74e357f0280cb1bd882e9cd94b91bdf",
      .num_options = 1,
      .options = application_701_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_703_options,
    },
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_705_options,
    },
    { .name = "Exanima",
      .executable = "Exanima.exe",
      .num_options = 1,
      .options = application_707_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_709_options,
    },
};

static const struct driconf_device device_646 = {
    .driver = "radv",
    .num_engines = 6,
    .engines = device_646_engines,
    .num_applications = 28,
    .applications = device_646_applications,
};

static const struct driconf_device *driconf[] = {
    &device_1,
    &device_484,
    &device_491,
    &device_547,
    &device_560,
    &device_571,
    &device_578,
    &device_621,
    &device_627,
    &device_630,
    &device_633,
    &device_646,
};
