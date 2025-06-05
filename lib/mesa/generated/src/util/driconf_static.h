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



    
static const struct driconf_option engine_506_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};


static const struct driconf_engine device_1_engines[] = {
    { .engine_name_match = "UnrealEngine4.*",
      .engine_versions = "0:23",
      .num_options = 1,
      .options = engine_506_options,
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
    { .name = "force_gl_depth_component_type_int", .value = "true" },
};

    
static const struct driconf_option application_79_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_81_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_83_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_85_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
    { .name = "force_gl_vendor", .value = "ATI Technologies, Inc." },
};

    
static const struct driconf_option application_89_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
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
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_103_options[] = {
    { .name = "force_compat_shaders", .value = "true" },
};

    
static const struct driconf_option application_105_options[] = {
    { .name = "force_compat_shaders", .value = "true" },
};

    
static const struct driconf_option application_107_options[] = {
    { .name = "force_glsl_version", .value = "440" },
};

    
static const struct driconf_option application_109_options[] = {
    { .name = "force_glsl_abs_sqrt", .value = "true" },
};

    
static const struct driconf_option application_111_options[] = {
    { .name = "force_glsl_abs_sqrt", .value = "true" },
};

    
static const struct driconf_option application_113_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_115_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_117_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_119_options[] = {
    { .name = "force_glsl_version", .value = "130" },
    { .name = "glsl_ignore_write_to_readonly_var", .value = "true" },
};

    
static const struct driconf_option application_122_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_124_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_126_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_128_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_130_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_132_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_134_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_136_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_138_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_140_options[] = {
    { .name = "allow_glsl_builtin_const_expression", .value = "true" },
    { .name = "allow_glsl_relaxed_es", .value = "true" },
};

    
static const struct driconf_option application_143_options[] = {
    { .name = "allow_extra_pp_tokens", .value = "true" },
};

    
static const struct driconf_option application_145_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_147_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_149_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_151_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_153_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_155_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
};

    
static const struct driconf_option application_157_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
};

    
static const struct driconf_option application_159_options[] = {
    { .name = "vs_position_always_invariant", .value = "true" },
};

    
static const struct driconf_option application_161_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
};

    
static const struct driconf_option application_163_options[] = {
    { .name = "lower_depth_range_rate", .value = "0.8" },
};

    
static const struct driconf_option application_165_options[] = {
    { .name = "lower_depth_range_rate", .value = "0.8" },
};

    
static const struct driconf_option application_167_options[] = {
    { .name = "allow_multisampled_copyteximage", .value = "true" },
};

    
static const struct driconf_option application_169_options[] = {
    { .name = "vblank_mode", .value = "0" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
    { .name = "force_gl_vendor", .value = "NVIDIA Corporation" },
    { .name = "force_glsl_extensions_warn", .value = "true" },
    { .name = "mesa_glthread_app_profile", .value = "1" },
    { .name = "mesa_no_error", .value = "true" },
    { .name = "allow_rgb10_configs", .value = "false" },
    { .name = "allow_invalid_glx_destroy_window", .value = "true" },
};

    
static const struct driconf_option application_179_options[] = {
    { .name = "ignore_map_unsynchronized", .value = "true" },
};

    
static const struct driconf_option application_181_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_184_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_186_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_188_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_190_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_192_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_194_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_196_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_198_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
};

    
static const struct driconf_option application_200_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
    { .name = "do_dce_before_clip_cull_analysis", .value = "true" },
};

    
static const struct driconf_option application_203_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
};

    
static const struct driconf_option application_205_options[] = {
    { .name = "force_glsl_extensions_warn", .value = "true" },
};

    
static const struct driconf_option application_207_options[] = {
    { .name = "force_direct_glx_context", .value = "true" },
};

    
static const struct driconf_option application_209_options[] = {
    { .name = "keep_native_window_glx_drawable", .value = "true" },
    { .name = "allow_rgb10_configs", .value = "false" },
};

    
static const struct driconf_option application_212_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_214_options[] = {
    { .name = "force_compat_shaders", .value = "true" },
};

    
static const struct driconf_option application_216_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
};

    
static const struct driconf_option application_218_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_220_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_222_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_224_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_226_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_228_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_230_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_232_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_234_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_236_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_238_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_240_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_242_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_244_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_246_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_248_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_250_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_252_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_254_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_256_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_258_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_260_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_262_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_264_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_266_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_268_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_270_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_272_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_274_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_276_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_278_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_280_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_282_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
    { .name = "vk_x11_ignore_suboptimal", .value = "true" },
};

    
static const struct driconf_option application_286_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
    { .name = "vk_x11_ignore_suboptimal", .value = "true" },
};

    
static const struct driconf_option application_290_options[] = {
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
    { .name = "vk_x11_ignore_suboptimal", .value = "true" },
};

    
static const struct driconf_option application_293_options[] = {
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
    { .name = "vk_x11_ignore_suboptimal", .value = "true" },
};

    
static const struct driconf_option application_296_options[] = {
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
    { .name = "vk_x11_ignore_suboptimal", .value = "true" },
};

    
static const struct driconf_option application_299_options[] = {
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
    { .name = "vk_x11_ignore_suboptimal", .value = "true" },
};

    
static const struct driconf_option application_302_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_304_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_306_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_308_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_310_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_312_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_314_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_316_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_318_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_320_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_322_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_324_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_326_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_328_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_330_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_332_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_334_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_336_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_338_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_340_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_342_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_344_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_346_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_348_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_350_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_352_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_354_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_356_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
};

    
static const struct driconf_option application_358_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
};

    
static const struct driconf_option application_360_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
};

    
static const struct driconf_option application_362_options[] = {
    { .name = "adaptive_sync", .value = "false" },
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_365_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_367_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_369_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_371_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_373_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_375_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_377_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_379_options[] = {
    { .name = "adaptive_sync", .value = "false" },
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
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_398_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_400_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_402_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_404_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_406_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_408_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_410_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_412_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_414_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_416_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_418_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_420_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_422_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_424_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_426_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_428_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_430_options[] = {
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_432_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
    { .name = "vk_x11_override_min_image_count", .value = "2" },
    { .name = "vk_wsi_force_bgra8_unorm_first", .value = "true" },
};

    
static const struct driconf_option application_436_options[] = {
    { .name = "vk_x11_override_min_image_count", .value = "2" },
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_439_options[] = {
    { .name = "vk_x11_override_min_image_count", .value = "2" },
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_442_options[] = {
    { .name = "vk_x11_override_min_image_count", .value = "3" },
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_445_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_OML_swap_method" },
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_448_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_450_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_452_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_454_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_456_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_458_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_460_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_462_options[] = {
    { .name = "ignore_discard_framebuffer", .value = "true" },
};

    
static const struct driconf_option application_464_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_466_options[] = {
    { .name = "vk_x11_ensure_min_image_count", .value = "true" },
};

    
static const struct driconf_option application_468_options[] = {
    { .name = "vk_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_470_options[] = {
    { .name = "vk_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_472_options[] = {
    { .name = "vk_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_474_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_476_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_478_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_480_options[] = {
    { .name = "vk_dont_care_as_load", .value = "true" },
};

    
static const struct driconf_option application_482_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_484_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_486_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_488_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_490_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_492_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_494_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_496_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_498_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_500_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_502_options[] = {
    { .name = "no_fp16", .value = "true" },
};

    
static const struct driconf_option application_504_options[] = {
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
    { .name = "TombRaider 4-5-6 Remastered",
      .executable = "tomb456.exe",
      .num_options = 1,
      .options = application_77_options,
    },
    { .name = "Warsow (32-bit)",
      .executable = "warsow.i386",
      .num_options = 1,
      .options = application_79_options,
    },
    { .name = "Warsow (64-bit)",
      .executable = "warsow.x86_64",
      .num_options = 1,
      .options = application_81_options,
    },
    { .name = "Rust",
      .executable = "rust",
      .num_options = 1,
      .options = application_83_options,
    },
    { .name = "Divinity: Original Sin Enhanced Edition",
      .executable = "EoCApp",
      .num_options = 3,
      .options = application_85_options,
    },
    { .name = "Metro 2033 Redux / Metro Last Night Redux",
      .executable = "metro",
      .num_options = 1,
      .options = application_89_options,
    },
    { .name = "Worms W.M.D",
      .executable = "Worms W.M.Dx64",
      .num_options = 1,
      .options = application_91_options,
    },
    { .name = "Crookz - The Big Heist",
      .executable = "Crookz",
      .num_options = 1,
      .options = application_93_options,
    },
    { .name = "Tropico 5",
      .executable = "Tropico5",
      .num_options = 1,
      .options = application_95_options,
    },
    { .name = "Faster than Light (32-bit)",
      .executable = "FTL.x86",
      .num_options = 1,
      .options = application_97_options,
    },
    { .name = "Faster than Light (64-bit)",
      .executable = "FTL.amd64",
      .num_options = 1,
      .options = application_99_options,
    },
    { .name = "Final Fantasy VIII: Remastered",
      .executable = "FFVIII.exe",
      .num_options = 1,
      .options = application_101_options,
    },
    { .name = "SNK HEROINES Tag Team Frenzy",
      .executable = "SNKHEROINES.exe",
      .num_options = 1,
      .options = application_103_options,
    },
    { .name = "Metal Slug XX",
      .executable = "MSXX_main.exe",
      .num_options = 1,
      .options = application_105_options,
    },
    { .name = "The Culling",
      .executable = "Victory",
      .num_options = 1,
      .options = application_107_options,
    },
    { .name = "Spec Ops: The Line (32-bit)",
      .executable = "specops.i386",
      .num_options = 1,
      .options = application_109_options,
    },
    { .name = "Spec Ops: The Line (64-bit)",
      .executable = "specops",
      .num_options = 1,
      .options = application_111_options,
    },
    { .name = "Jamestown+",
      .executable = "JamestownPlus.exe",
      .num_options = 1,
      .options = application_113_options,
    },
    { .name = "Kerbal Space Program (32-bit)",
      .executable = "KSP.x86",
      .num_options = 1,
      .options = application_115_options,
    },
    { .name = "Kerbal Space Program (64-bit)",
      .executable = "KSP.x86_64",
      .num_options = 1,
      .options = application_117_options,
    },
    { .name = "Luna Sky",
      .executable = "lunasky",
      .num_options = 2,
      .options = application_119_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_122_options,
    },
    { .name = "The Witcher 2",
      .executable = "witcher2",
      .num_options = 1,
      .options = application_124_options,
    },
    { .name = "Unreal 4 Editor",
      .executable = "UE4Editor",
      .num_options = 1,
      .options = application_126_options,
    },
    { .name = "Observer",
      .executable = "TheObserver-Linux-Shipping",
      .num_options = 1,
      .options = application_128_options,
    },
    { .name = "Pixel Game Maker MV",
      .executable = "player.exe",
      .num_options = 1,
      .options = application_130_options,
    },
    { .name = "Eden Gamma",
      .executable = "EdenGammaGame-0.3.0.2.exe",
      .num_options = 1,
      .options = application_132_options,
    },
    { .name = "Steamroll",
      .executable = "Steamroll-Linux-Shipping",
      .num_options = 1,
      .options = application_134_options,
    },
    { .name = "Refunct",
      .executable = "Refunct-Linux-Shipping",
      .num_options = 1,
      .options = application_136_options,
    },
    { .name = "We Happy Few",
      .executable = "GlimpseGame",
      .num_options = 1,
      .options = application_138_options,
    },
    { .name = "Google Earth VR",
      .executable = "Earth.exe",
      .num_options = 2,
      .options = application_140_options,
    },
    { .name = "Champions of Regnum",
      .executable = "game",
      .num_options = 1,
      .options = application_143_options,
    },
    { .name = "Wolfenstein The Old Blood",
      .executable = "WolfOldBlood_x64.exe",
      .num_options = 1,
      .options = application_145_options,
    },
    { .name = "ARMA 3",
      .executable = "arma3.x86_64",
      .num_options = 1,
      .options = application_147_options,
    },
    { .name = "Epic Games Launcher",
      .executable = "EpicGamesLauncher.exe",
      .num_options = 1,
      .options = application_149_options,
    },
    { .name = "GpuTest",
      .executable = "GpuTest",
      .num_options = 1,
      .options = application_151_options,
    },
    { .name = "Curse of the Dead Gods",
      .executable = "Curse of the Dead Gods.exe",
      .num_options = 1,
      .options = application_153_options,
    },
    { .name = "GRID Autosport",
      .executable = "GridAutosport",
      .num_options = 1,
      .options = application_155_options,
    },
    { .name = "DIRT: Showdown",
      .executable = "dirt.i386",
      .num_options = 1,
      .options = application_157_options,
    },
    { .name = "DiRT Rally",
      .executable = "DirtRally",
      .num_options = 1,
      .options = application_159_options,
    },
    { .name = "Foundation",
      .executable = "foundation.exe",
      .num_options = 1,
      .options = application_161_options,
    },
    { .name = "Homerun Clash",
      .executable = "com.haegin.homerunclash",
      .num_options = 1,
      .options = application_163_options,
    },
    { .name = "The Spirit and The Mouse",
      .executable = "TheSpiritAndTheMouse.exe",
      .num_options = 1,
      .options = application_165_options,
    },
    { .name = "Penumbra: Overture",
      .executable = "Penumbra.exe",
      .num_options = 1,
      .options = application_167_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 9,
      .options = application_169_options,
    },
    { .name = "Dead-Cells",
      .executable = "com.playdigious.deadcells.mobile",
      .num_options = 1,
      .options = application_179_options,
    },
    { .name = "Teardown",
      .executable = "teardown.exe",
      .num_options = 2,
      .options = application_181_options,
    },
    { .name = "Golf With Your Friends",
      .executable = "Golf With Your Friends.x86_64",
      .num_options = 1,
      .options = application_184_options,
    },
    { .name = "Cossacks 3",
      .executable = "cossacks.exe",
      .num_options = 1,
      .options = application_186_options,
    },
    { .name = "Kaiju-A-Gogo",
      .executable = "kaiju.exe",
      .num_options = 1,
      .options = application_188_options,
    },
    { .name = "Captain Lycop: Invasion of the Heters (Wine)",
      .executable = "lycop.exe",
      .num_options = 1,
      .options = application_190_options,
    },
    { .name = "Captain Lycop: Invasion of the Heters",
      .executable = "lycop",
      .num_options = 1,
      .options = application_192_options,
    },
    { .name = "Joe Danger (Wine)",
      .executable = "JoeDanger.exe",
      .num_options = 1,
      .options = application_194_options,
    },
    { .name = "Joe Danger 2 (Wine)",
      .executable = "JoeDanger2.exe",
      .num_options = 1,
      .options = application_196_options,
    },
    { .name = "BETA CAE Systems - GL detect tool",
      .executable = "detect_opengl_tool",
      .num_options = 1,
      .options = application_198_options,
    },
    { .name = "BETA CAE Systems - ANSA",
      .executable = "ansa_linux_x86_64",
      .num_options = 2,
      .options = application_200_options,
    },
    { .name = "BETA CAE Systems - META",
      .executable = "meta_post_x86_64",
      .num_options = 1,
      .options = application_203_options,
    },
    { .name = "Mari",
      .executable_regexp = "Mari[0-9]+[.][0-9]+v[0-9]+",
      .num_options = 1,
      .options = application_205_options,
    },
    { .name = "Discovery Studio 2020",
      .executable = "DiscoveryStudio2020-bin",
      .num_options = 1,
      .options = application_207_options,
    },
    { .name = "Abaqus",
      .executable = "ABQcaeK",
      .num_options = 2,
      .options = application_209_options,
    },
    { .name = "Maya",
      .executable = "maya.bin",
      .num_options = 1,
      .options = application_212_options,
    },
    { .name = "SD Gundam G Generation Cross Rays",
      .executable = "togg.exe",
      .num_options = 1,
      .options = application_214_options,
    },
    { .name = "FINAL FANTASY XI",
      .executable = "pol.exe",
      .num_options = 1,
      .options = application_216_options,
    },
    { .name = "Minecraft",
      .executable = "minecraft-launcher",
      .num_options = 1,
      .options = application_218_options,
    },
    { .name = "Minecraft-FTB",
      .executable = "ftb-app",
      .num_options = 1,
      .options = application_220_options,
    },
    { .name = "Alien Isolation",
      .executable = "AlienIsolation",
      .num_options = 1,
      .options = application_222_options,
    },
    { .name = "BioShock Infinite",
      .executable = "bioshock.i386",
      .num_options = 1,
      .options = application_224_options,
    },
    { .name = "Borderlands 2",
      .executable = "Borderlands2",
      .num_options = 1,
      .options = application_226_options,
    },
    { .name = "Civilization 5",
      .executable = "Civ5XP",
      .num_options = 1,
      .options = application_228_options,
    },
    { .name = "Civilization 6",
      .executable = "Civ6",
      .num_options = 1,
      .options = application_230_options,
    },
    { .name = "Civilization 6",
      .executable = "Civ6Sub",
      .num_options = 1,
      .options = application_232_options,
    },
    { .name = "Dreamfall Chapters",
      .executable = "Dreamfall Chapters",
      .num_options = 1,
      .options = application_234_options,
    },
    { .name = "Hitman",
      .executable = "HitmanPro",
      .num_options = 1,
      .options = application_236_options,
    },
    { .name = "Renowned Explorers: International Society",
      .executable = "abbeycore_steam",
      .num_options = 1,
      .options = application_238_options,
    },
    { .name = "Saints Row 2",
      .executable = "saintsrow2.i386",
      .num_options = 1,
      .options = application_240_options,
    },
    { .name = "Saints Row: The Third",
      .executable = "SaintsRow3.i386",
      .num_options = 1,
      .options = application_242_options,
    },
    { .name = "Saints Row IV",
      .executable = "SaintsRow4.i386",
      .num_options = 1,
      .options = application_244_options,
    },
    { .name = "Saints Row: Gat out of Hell",
      .executable = "SaintsRow4GooH.i386",
      .num_options = 1,
      .options = application_246_options,
    },
    { .name = "Sid Meier's: Civilization Beyond Earth",
      .executable = "CivBE",
      .num_options = 1,
      .options = application_248_options,
    },
    { .name = "The Witcher 2",
      .executable = "witcher2",
      .num_options = 1,
      .options = application_250_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_252_options,
    },
    { .name = "Euro Truck Simulator 2",
      .executable = "eurotrucks2",
      .num_options = 1,
      .options = application_254_options,
    },
    { .name = "Overlord",
      .executable = "overlord.i386",
      .num_options = 1,
      .options = application_256_options,
    },
    { .name = "Overlord 2",
      .executable = "overlord2.i386",
      .num_options = 1,
      .options = application_258_options,
    },
    { .name = "Oil Rush",
      .executable = "OilRush_x86",
      .num_options = 1,
      .options = application_260_options,
    },
    { .name = "War Thunder",
      .executable = "aces",
      .num_options = 1,
      .options = application_262_options,
    },
    { .name = "War Thunder (Wine)",
      .executable = "aces.exe",
      .num_options = 1,
      .options = application_264_options,
    },
    { .name = "Outlast",
      .executable = "OLGame.x86_64",
      .num_options = 1,
      .options = application_266_options,
    },
    { .name = "Spec Ops: The Line (32-bit)",
      .executable = "specops.i386",
      .num_options = 1,
      .options = application_268_options,
    },
    { .name = "Spec Ops: The Line (64-bit)",
      .executable = "specops",
      .num_options = 1,
      .options = application_270_options,
    },
    { .name = "Mount and Blade Warband",
      .executable = "mb_warband_linux",
      .num_options = 1,
      .options = application_272_options,
    },
    { .name = "Valheim",
      .executable = "valheim.x86_64",
      .num_options = 1,
      .options = application_274_options,
    },
    { .name = "Medieval II: Total War",
      .executable = "Medieval2",
      .num_options = 1,
      .options = application_276_options,
    },
    { .name = "Carnivores: Dinosaur Hunter Reborn (wine)",
      .executable = "Carnivores-master.exe",
      .num_options = 1,
      .options = application_278_options,
    },
    { .name = "Far Cry 2 (wine)",
      .executable = "farcry2.exe",
      .num_options = 1,
      .options = application_280_options,
    },
    { .name = "Talos Principle",
      .executable = "Talos",
      .num_options = 3,
      .options = application_282_options,
    },
    { .name = "Talos Principle (Unrestricted)",
      .executable = "Talos_Unrestricted",
      .num_options = 3,
      .options = application_286_options,
    },
    { .name = "Talos Principle VR",
      .executable = "Talos_VR",
      .num_options = 2,
      .options = application_290_options,
    },
    { .name = "Talos Principle VR (Unrestricted)",
      .executable = "Talos_Unrestricted_VR",
      .num_options = 2,
      .options = application_293_options,
    },
    { .name = "Serious Sam Fusion",
      .executable = "Sam2017",
      .num_options = 2,
      .options = application_296_options,
    },
    { .name = "Serious Sam Fusion (Unrestricted)",
      .executable = "Sam2017_Unrestricted",
      .num_options = 2,
      .options = application_299_options,
    },
    { .name = "7 Days To Die (64-bit)",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_302_options,
    },
    { .name = "7 Days To Die (32-bit)",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_304_options,
    },
    { .name = "Dolphin Emulator",
      .executable = "dolphin-emu",
      .num_options = 1,
      .options = application_306_options,
    },
    { .name = "Citra - Nintendo 3DS Emulator",
      .executable = "citra-qt",
      .num_options = 1,
      .options = application_308_options,
    },
    { .name = "Yuzu - Nintendo Switch Emulator",
      .executable = "yuzu",
      .num_options = 1,
      .options = application_310_options,
    },
    { .name = "RPCS3",
      .executable = "rpcs3",
      .num_options = 1,
      .options = application_312_options,
    },
    { .name = "PCSX2",
      .executable = "PCSX2",
      .num_options = 1,
      .options = application_314_options,
    },
    { .name = "From The Depths",
      .executable = "From_The_Depths.x86_64",
      .num_options = 1,
      .options = application_316_options,
    },
    { .name = "Plague Inc Evolved (32-bit)",
      .executable = "PlagueIncEvolved.x86",
      .num_options = 1,
      .options = application_318_options,
    },
    { .name = "Plague Inc Evolved (64-bit)",
      .executable = "PlagueIncEvolved.x86_64",
      .num_options = 1,
      .options = application_320_options,
    },
    { .name = "Beholder (32-bit)",
      .executable = "Beholder.x86",
      .num_options = 1,
      .options = application_322_options,
    },
    { .name = "Beholder (64-bit)",
      .executable = "Beholder.x86_64",
      .num_options = 1,
      .options = application_324_options,
    },
    { .name = "X3 Reunion",
      .executable = "X3R_main",
      .num_options = 1,
      .options = application_326_options,
    },
    { .name = "X3 Terran Conflict",
      .executable = "X3TR_main",
      .num_options = 1,
      .options = application_328_options,
    },
    { .name = "X3 Albion Prelude",
      .executable = "X3AP_main",
      .num_options = 1,
      .options = application_330_options,
    },
    { .name = "Borderlands: The Pre-Sequel",
      .executable = "BorderlandsPreSequel",
      .num_options = 1,
      .options = application_332_options,
    },
    { .name = "Transport Fever",
      .executable = "TransportFever",
      .num_options = 1,
      .options = application_334_options,
    },
    { .name = "Unigine Sanctuary",
      .executable = "Sanctuary",
      .num_options = 1,
      .options = application_336_options,
    },
    { .name = "Unigine Tropics",
      .executable = "Tropics",
      .num_options = 1,
      .options = application_338_options,
    },
    { .name = "Unigine Heaven (32-bit)",
      .executable = "heaven_x86",
      .num_options = 1,
      .options = application_340_options,
    },
    { .name = "Unigine Heaven (64-bit)",
      .executable = "heaven_x64",
      .num_options = 1,
      .options = application_342_options,
    },
    { .name = "Unigine Valley (32-bit)",
      .executable = "valley_x86",
      .num_options = 1,
      .options = application_344_options,
    },
    { .name = "Unigine Valley (64-bit)",
      .executable = "valley_x64",
      .num_options = 1,
      .options = application_346_options,
    },
    { .name = "Unigine Superposition",
      .executable = "superposition",
      .num_options = 1,
      .options = application_348_options,
    },
    { .name = "Basemark GPU",
      .executable = "BasemarkGPU_gl",
      .num_options = 1,
      .options = application_350_options,
    },
    { .name = "Stellaris",
      .executable = "stellaris",
      .num_options = 1,
      .options = application_352_options,
    },
    { .name = "Battletech",
      .executable = "BattleTech",
      .num_options = 1,
      .options = application_354_options,
    },
    { .name = "DeusExMD",
      .executable = "DeusExMD",
      .num_options = 1,
      .options = application_356_options,
    },
    { .name = "F1 2015",
      .executable = "F12015",
      .num_options = 1,
      .options = application_358_options,
    },
    { .name = "KWin Wayland",
      .executable = "kwin_wayland",
      .num_options = 1,
      .options = application_360_options,
    },
    { .name = "gnome-shell",
      .executable = "gnome-shell",
      .num_options = 2,
      .options = application_362_options,
    },
    { .name = "Desktop — Plasma",
      .executable = "plasmashell",
      .num_options = 1,
      .options = application_365_options,
    },
    { .name = "budgie-wm",
      .executable = "budgie-wm",
      .num_options = 1,
      .options = application_367_options,
    },
    { .name = "kwin_x11",
      .executable = "kwin_x11",
      .num_options = 1,
      .options = application_369_options,
    },
    { .name = "ksmserver-logout-greeter",
      .executable = "ksmserver-logout-greeter",
      .num_options = 1,
      .options = application_371_options,
    },
    { .name = "ksmserver-switchuser-greeter",
      .executable = "ksmserver-switchuser-greeter",
      .num_options = 1,
      .options = application_373_options,
    },
    { .name = "kscreenlocker_greet",
      .executable = "kscreenlocker_greet",
      .num_options = 1,
      .options = application_375_options,
    },
    { .name = "startplasma",
      .executable = "startplasma",
      .num_options = 1,
      .options = application_377_options,
    },
    { .name = "sddm-greeter",
      .executable = "sddm-greeter",
      .num_options = 1,
      .options = application_379_options,
    },
    { .name = "krunner",
      .executable = "krunner",
      .num_options = 1,
      .options = application_381_options,
    },
    { .name = "spectacle",
      .executable = "spectacle",
      .num_options = 1,
      .options = application_383_options,
    },
    { .name = "marco",
      .executable = "marco",
      .num_options = 1,
      .options = application_385_options,
    },
    { .name = "compton",
      .executable = "compton",
      .num_options = 1,
      .options = application_387_options,
    },
    { .name = "picom",
      .executable = "picom",
      .num_options = 1,
      .options = application_389_options,
    },
    { .name = "xfwm4",
      .executable = "xfwm4",
      .num_options = 1,
      .options = application_391_options,
    },
    { .name = "Enlightenment",
      .executable = "enlightenment",
      .num_options = 1,
      .options = application_393_options,
    },
    { .name = "mutter",
      .executable = "mutter",
      .num_options = 2,
      .options = application_395_options,
    },
    { .name = "muffin",
      .executable = "muffin",
      .num_options = 1,
      .options = application_398_options,
    },
    { .name = "cinnamon",
      .executable = "cinnamon",
      .num_options = 1,
      .options = application_400_options,
    },
    { .name = "compiz",
      .executable = "compiz",
      .num_options = 1,
      .options = application_402_options,
    },
    { .name = "Firefox",
      .executable = "firefox",
      .num_options = 1,
      .options = application_404_options,
    },
    { .name = "Firefox ESR",
      .executable = "firefox-esr",
      .num_options = 1,
      .options = application_406_options,
    },
    { .name = "Chromium",
      .executable = "chromium",
      .num_options = 1,
      .options = application_408_options,
    },
    { .name = "Google Chrome",
      .executable = "chrome",
      .num_options = 1,
      .options = application_410_options,
    },
    { .name = "Iceweasel",
      .executable = "iceweasel",
      .num_options = 1,
      .options = application_412_options,
    },
    { .name = "Epiphany",
      .executable = "epiphany",
      .num_options = 1,
      .options = application_414_options,
    },
    { .name = "Konqueror",
      .executable = "konqueror",
      .num_options = 1,
      .options = application_416_options,
    },
    { .name = "Falkon",
      .executable = "falkon",
      .num_options = 1,
      .options = application_418_options,
    },
    { .name = "Seamonkey",
      .executable = "seamonkey",
      .num_options = 1,
      .options = application_420_options,
    },
    { .name = "Waterfox",
      .executable = "waterfox",
      .num_options = 1,
      .options = application_422_options,
    },
    { .name = "VLC Media Player",
      .executable = "vlc",
      .num_options = 1,
      .options = application_424_options,
    },
    { .name = "Totem",
      .executable = "totem",
      .num_options = 1,
      .options = application_426_options,
    },
    { .name = "Dragon Player",
      .executable = "dragon",
      .num_options = 1,
      .options = application_428_options,
    },
    { .name = "Xorg",
      .executable = "Xorg",
      .num_options = 1,
      .options = application_430_options,
    },
    { .name = "gfxbench",
      .executable = "testfw_app",
      .num_options = 3,
      .options = application_432_options,
    },
    { .name = "Rainbow Six Siege (Vulkan)",
      .executable = "RainbowSix_Vulkan.exe",
      .num_options = 2,
      .options = application_436_options,
    },
    { .name = "Rainbow Six Extraction (Wine)",
      .executable = "R6-Extraction.exe",
      .num_options = 2,
      .options = application_439_options,
    },
    { .name = "Hades",
      .executable = "Hades.exe",
      .num_options = 2,
      .options = application_442_options,
    },
    { .name = "Brink",
      .executable = "brink.exe",
      .num_options = 2,
      .options = application_445_options,
    },
    { .name = "Enter The Gungeon (32 bits)",
      .executable = "EtG.x86",
      .num_options = 1,
      .options = application_448_options,
    },
    { .name = "Enter The Gungeon (64 bits)",
      .executable = "EtG.x86_64",
      .num_options = 1,
      .options = application_450_options,
    },
    { .name = "Assault Android Cactus (32-bit)",
      .executable = "cactus_demo.x86",
      .num_options = 1,
      .options = application_452_options,
    },
    { .name = "Assault Android Cactus (64-bit)",
      .executable = "cactus_demo.x86_64",
      .num_options = 1,
      .options = application_454_options,
    },
    { .name = "Assault Android Cactus (32-bit)",
      .executable = "cactus.x86",
      .num_options = 1,
      .options = application_456_options,
    },
    { .name = "Assault Android Cactus (64-bit)",
      .executable = "cactus.x86_64",
      .num_options = 1,
      .options = application_458_options,
    },
    { .name = "Limbo",
      .executable = "limbo",
      .num_options = 1,
      .options = application_460_options,
    },
    { .name = "Genshin Impact",
      .executable = "com.miHoYo.GenshinImpact",
      .num_options = 1,
      .options = application_462_options,
    },
    { .name = "DOOM",
      .executable = "DOOMx64vk.exe",
      .num_options = 1,
      .options = application_464_options,
    },
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 1,
      .options = application_466_options,
    },
    { .name = "Path of Exile",
      .executable = "PathOfExile_x64Steam.exe",
      .num_options = 1,
      .options = application_468_options,
    },
    { .name = "Path of Exile",
      .executable = "PathOfExileSteam.exe",
      .num_options = 1,
      .options = application_470_options,
    },
    { .name = "X4 Foundations",
      .executable = "X4",
      .num_options = 1,
      .options = application_472_options,
    },
    { .name = "Wolfenstein: Youngblood(x64vk)",
      .executable = "Youngblood_x64vk.exe",
      .num_options = 1,
      .options = application_474_options,
    },
    { .name = "Wolfenstein II: The New Colossus",
      .executable = "NewColossus_x64vk.exe",
      .num_options = 1,
      .options = application_476_options,
    },
    { .name = "Metro: Exodus",
      .application_name_match = "metroexodus",
      .num_options = 1,
      .options = application_478_options,
    },
    { .name = "Forsaken Remastered",
      .executable = "ForsakenEx",
      .num_options = 1,
      .options = application_480_options,
    },
    { .name = "Atlas Fallen",
      .executable = "AtlasFallen (VK).exe",
      .num_options = 1,
      .options = application_482_options,
    },
    { .name = "Firefox",
      .executable = "firefox",
      .num_options = 1,
      .options = application_484_options,
    },
    { .name = "Firefox ESR",
      .executable = "firefox-esr",
      .num_options = 1,
      .options = application_486_options,
    },
    { .name = "Chromium",
      .executable = "chromium",
      .num_options = 1,
      .options = application_488_options,
    },
    { .name = "Google Chrome",
      .executable = "chrome",
      .num_options = 1,
      .options = application_490_options,
    },
    { .name = "Iceweasel",
      .executable = "iceweasel",
      .num_options = 1,
      .options = application_492_options,
    },
    { .name = "Epiphany",
      .executable = "epiphany",
      .num_options = 1,
      .options = application_494_options,
    },
    { .name = "Konqueror",
      .executable = "konqueror",
      .num_options = 1,
      .options = application_496_options,
    },
    { .name = "Falkon",
      .executable = "falkon",
      .num_options = 1,
      .options = application_498_options,
    },
    { .name = "Seamonkey",
      .executable = "seamonkey",
      .num_options = 1,
      .options = application_500_options,
    },
    { .name = "Waterfox",
      .executable = "waterfox",
      .num_options = 1,
      .options = application_502_options,
    },
    { .name = "Detroit Become Human",
      .application_name_match = "DetroitBecomeHuman",
      .num_options = 1,
      .options = application_504_options,
    },
};

static const struct driconf_device device_1 = {
    .num_engines = 1,
    .engines = device_1_engines,
    .num_applications = 227,
    .applications = device_1_applications,
};


    
static const struct driconf_option application_509_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_OML_sync_control -GLX_SGI_video_sync" },
};

    
static const struct driconf_option application_511_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_OML_sync_control -GLX_SGI_video_sync" },
};

    
static const struct driconf_option application_513_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_OML_sync_control" },
};


static const struct driconf_application device_508_applications[] = {
    { .name = "gnome-shell",
      .executable = "gnome-shell",
      .num_options = 1,
      .options = application_509_options,
    },
    { .name = "cinnamon",
      .executable = "cinnamon",
      .num_options = 1,
      .options = application_511_options,
    },
    { .name = "Compiz",
      .executable = "Compiz",
      .num_options = 1,
      .options = application_513_options,
    },
};

static const struct driconf_device device_508 = {
    .driver = "vmwgfx",
    .num_engines = 0,
    .num_applications = 3,
    .applications = device_508_applications,
};


    
static const struct driconf_option application_516_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_518_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_520_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_523_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_525_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_527_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_529_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "1" },
};

    
static const struct driconf_option application_531_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_534_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_536_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_538_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_540_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_542_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_544_options[] = {
    { .name = "radeonsi_no_infinite_interp", .value = "true" },
};

    
static const struct driconf_option application_546_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_548_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_550_options[] = {
    { .name = "glthread_nop_check_framebuffer_status", .value = "true" },
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_553_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_555_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_557_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
    { .name = "radeonsi_no_infinite_interp", .value = "true" },
};

    
static const struct driconf_option application_560_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_562_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_564_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_566_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
    { .name = "radeonsi_force_use_fma32", .value = "true" },
};

    
static const struct driconf_option application_569_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};


static const struct driconf_application device_515_applications[] = {
    { .name = "Alien Isolation",
      .executable = "AlienIsolation",
      .num_options = 1,
      .options = application_516_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_518_options,
    },
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 2,
      .options = application_520_options,
    },
    { .name = "Exanima",
      .executable = "Exanima.exe",
      .num_options = 1,
      .options = application_523_options,
    },
    { .name = "Nowhere Patrol",
      .executable = "NowherePatrol.exe",
      .num_options = 1,
      .options = application_525_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_527_options,
    },
    { .name = "Gfx Bench",
      .executable = "gfxbench_gl",
      .num_options = 1,
      .options = application_529_options,
    },
    { .name = "Monolith demo5",
      .executable = "runner",
      .sha1 = "0c40d509a74e357f0280cb1bd882e9cd94b91bdf",
      .num_options = 2,
      .options = application_531_options,
    },
    { .name = "Memoranda / Riptale",
      .executable = "runner",
      .sha1 = "aa13dec6af63c88f308ebb487693896434a4db56",
      .num_options = 1,
      .options = application_534_options,
    },
    { .name = "Nuclear Throne",
      .executable = "runner",
      .sha1 = "84814e8db125e889f5d9d4195a0ca72a871ea1fd",
      .num_options = 1,
      .options = application_536_options,
    },
    { .name = "Undertale",
      .executable = "runner",
      .sha1 = "dfa302e7ec78641d0696dbbc1a06fc29f34ff1ff",
      .num_options = 1,
      .options = application_538_options,
    },
    { .name = "Turmoil",
      .executable = "runner",
      .sha1 = "cbbf757aaab289859f8dae191a7d63afc30643d9",
      .num_options = 1,
      .options = application_540_options,
    },
    { .name = "Peace, Death!",
      .executable = "runner",
      .sha1 = "5b909f3d21799773370adf084f649848f098234e",
      .num_options = 1,
      .options = application_542_options,
    },
    { .name = "Kerbal Space Program",
      .executable = "KSP.x86_64",
      .num_options = 1,
      .options = application_544_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_546_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_548_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 2,
      .options = application_550_options,
    },
    { .name = "Road Redemption",
      .executable = "RoadRedemption.x86_64",
      .num_options = 1,
      .options = application_553_options,
    },
    { .name = "Wasteland 2",
      .executable = "WL2",
      .num_options = 1,
      .options = application_555_options,
    },
    { .name = "Teardown",
      .executable = "teardown.exe",
      .num_options = 2,
      .options = application_557_options,
    },
    { .name = "SpaceEngine",
      .executable = "SpaceEngine.exe",
      .num_options = 1,
      .options = application_560_options,
    },
    { .name = "BETA CAE Systems - GL detect tool",
      .executable = "detect_opengl_tool",
      .num_options = 1,
      .options = application_562_options,
    },
    { .name = "BETA CAE Systems - ANSA",
      .executable = "ansa_linux_x86_64",
      .num_options = 1,
      .options = application_564_options,
    },
    { .name = "BETA CAE Systems - META",
      .executable = "meta_post_x86_64",
      .num_options = 2,
      .options = application_566_options,
    },
    { .name = "Black Geyser: Couriers of Darkness",
      .executable = "BlackGeyser.x86_64",
      .num_options = 1,
      .options = application_569_options,
    },
};

static const struct driconf_device device_515 = {
    .driver = "radeonsi",
    .num_engines = 0,
    .num_applications = 25,
    .applications = device_515_applications,
};


    
static const struct driconf_option application_572_options[] = {
    { .name = "mesa_glthread_app_profile", .value = "0" },
};

    
static const struct driconf_option application_574_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_576_options[] = {
    { .name = "zink_emulate_point_smooth", .value = "true" },
};

    
static const struct driconf_option application_578_options[] = {
    { .name = "zink_emulate_point_smooth", .value = "true" },
};

    
static const struct driconf_option application_580_options[] = {
    { .name = "zink_emulate_point_smooth", .value = "true" },
};

    
static const struct driconf_option application_582_options[] = {
    { .name = "zink_shader_object_enable", .value = "true" },
};


static const struct driconf_application device_571_applications[] = {
    { .name = "Hyperdimension Neptunia Re;Birth1",
      .executable = "NeptuniaReBirth1.exe",
      .num_options = 1,
      .options = application_572_options,
    },
    { .name = "Borderlands 2 (Native, OpenGL, 32bit)",
      .executable = "Borderlands2",
      .num_options = 1,
      .options = application_574_options,
    },
    { .name = "Quake II",
      .executable = "quake2-engine",
      .num_options = 1,
      .options = application_576_options,
    },
    { .name = "Quake II (yamagi)",
      .executable = "yamagi-quake2",
      .num_options = 1,
      .options = application_578_options,
    },
    { .name = "Quake II (wine)",
      .executable = "quake2.exe",
      .num_options = 1,
      .options = application_580_options,
    },
    { .name = "Tomb Raider 2013",
      .executable = "TombRaider",
      .num_options = 1,
      .options = application_582_options,
    },
};

static const struct driconf_device device_571 = {
    .driver = "zink",
    .num_engines = 0,
    .num_applications = 6,
    .applications = device_571_applications,
};


    
static const struct driconf_option application_585_options[] = {
    { .name = "vs_position_always_invariant", .value = "true" },
};

    
static const struct driconf_option application_587_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_589_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_591_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_593_options[] = {
    { .name = "mesa_extension_override", .value = "+GL_EXT_shader_image_load_store" },
};


static const struct driconf_application device_584_applications[] = {
    { .name = "Middle Earth: Shadow of Mordor",
      .executable = "ShadowOfMordor",
      .num_options = 1,
      .options = application_585_options,
    },
    { .name = "glmark2",
      .executable = "glmark2",
      .num_options = 1,
      .options = application_587_options,
    },
    { .name = "Counter-Strike: Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_589_options,
    },
    { .name = "Insurgency",
      .executable = "insurgency_linux",
      .num_options = 1,
      .options = application_591_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 1,
      .options = application_593_options,
    },
};

static const struct driconf_device device_584 = {
    .driver = "iris",
    .num_engines = 0,
    .num_applications = 5,
    .applications = device_584_applications,
};


    
static const struct driconf_option application_596_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_598_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_600_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};


static const struct driconf_application device_595_applications[] = {
    { .name = "glmark2",
      .executable = "glmark2",
      .num_options = 1,
      .options = application_596_options,
    },
    { .name = "Counter-Strike: Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_598_options,
    },
    { .name = "Insurgency",
      .executable = "insurgency_linux",
      .num_options = 1,
      .options = application_600_options,
    },
};

static const struct driconf_device device_595 = {
    .driver = "crocus",
    .num_engines = 0,
    .num_applications = 3,
    .applications = device_595_applications,
};
    
static const struct driconf_option engine_685_options[] = {
    { .name = "anv_enable_buffer_comp", .value = "true" },
};

    
static const struct driconf_option engine_687_options[] = {
    { .name = "anv_enable_buffer_comp", .value = "true" },
};

    
static const struct driconf_option engine_689_options[] = {
    { .name = "no_16bit", .value = "true" },
};

    
static const struct driconf_option engine_691_options[] = {
    { .name = "anv_disable_fcv", .value = "true" },
    { .name = "anv_assume_full_subgroups", .value = "16" },
};

    
static const struct driconf_option engine_694_options[] = {
    { .name = "compression_control_enabled", .value = "true" },
};


static const struct driconf_engine device_602_engines[] = {
    { .engine_name_match = "Source2",
      .num_options = 1,
      .options = engine_685_options,
    },
    { .engine_name_match = "vkd3d",
      .num_options = 1,
      .options = engine_687_options,
    },
    { .engine_name_match = "mesa zink",
      .num_options = 1,
      .options = engine_689_options,
    },
    { .engine_name_match = "UnrealEngine5.1",
      .num_options = 2,
      .options = engine_691_options,
    },
    { .engine_name_match = "vkd3d",
      .num_options = 1,
      .options = engine_694_options,
    },
};

    
static const struct driconf_option application_603_options[] = {
    { .name = "anv_assume_full_subgroups", .value = "32" },
};

    
static const struct driconf_option application_605_options[] = {
    { .name = "anv_assume_full_subgroups_with_barrier", .value = "true" },
};

    
static const struct driconf_option application_607_options[] = {
    { .name = "hasvk_report_vk_1_3_version", .value = "true" },
};

    
static const struct driconf_option application_609_options[] = {
    { .name = "anv_assume_full_subgroups", .value = "32" },
    { .name = "fp64_workaround_enabled", .value = "true" },
};

    
static const struct driconf_option application_612_options[] = {
    { .name = "anv_assume_full_subgroups_with_shared_memory", .value = "true" },
};

    
static const struct driconf_option application_614_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_616_options[] = {
    { .name = "anv_sample_mask_out_opengl_behaviour", .value = "true" },
};

    
static const struct driconf_option application_618_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_620_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_622_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_624_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_626_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_628_options[] = {
    { .name = "shader_spilling_rate", .value = "15" },
};

    
static const struct driconf_option application_630_options[] = {
    { .name = "fake_sparse", .value = "true" },
};

    
static const struct driconf_option application_632_options[] = {
    { .name = "fake_sparse", .value = "true" },
};

    
static const struct driconf_option application_634_options[] = {
    { .name = "anv_force_filter_addr_rounding", .value = "true" },
};

    
static const struct driconf_option application_636_options[] = {
    { .name = "anv_force_filter_addr_rounding", .value = "true" },
};

    
static const struct driconf_option application_638_options[] = {
    { .name = "anv_force_filter_addr_rounding", .value = "true" },
};

    
static const struct driconf_option application_640_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
    { .name = "shader_spilling_rate", .value = "0" },
};

    
static const struct driconf_option application_643_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_645_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_647_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_649_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_651_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_653_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_655_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_657_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_659_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_661_options[] = {
    { .name = "anv_disable_fcv", .value = "true" },
};

    
static const struct driconf_option application_663_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_665_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_667_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_669_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_671_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_673_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_675_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_677_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_679_options[] = {
    { .name = "fp64_workaround_enabled", .value = "true" },
};

    
static const struct driconf_option application_681_options[] = {
    { .name = "anv_upper_bound_descriptor_pool_sampler", .value = "true" },
};

    
static const struct driconf_option application_683_options[] = {
    { .name = "anv_fake_nonlocal_memory", .value = "true" },
};


static const struct driconf_application device_602_applications[] = {
    { .name = "Aperture Desk Job",
      .executable = "deskjob",
      .num_options = 1,
      .options = application_603_options,
    },
    { .name = "Breaking Limit",
      .executable = "GPUScoreVulkan",
      .num_options = 1,
      .options = application_605_options,
    },
    { .name = "Brawlhalla",
      .executable = "BrawlhallaGame.exe",
      .num_options = 1,
      .options = application_607_options,
    },
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 2,
      .options = application_609_options,
    },
    { .name = "RESIDENT EVIL 2",
      .executable = "re2.exe",
      .num_options = 1,
      .options = application_612_options,
    },
    { .name = "Wolfenstein: Youngblood(x64vk)",
      .executable = "Youngblood_x64vk.exe",
      .num_options = 1,
      .options = application_614_options,
    },
    { .name = "Batman™: Arkham Knight",
      .executable = "BatmanAK.exe",
      .num_options = 1,
      .options = application_616_options,
    },
    { .name = "Rise of the Tomb Raider",
      .executable = "RiseOfTheTombRaider",
      .num_options = 1,
      .options = application_618_options,
    },
    { .name = "Rise of the Tomb Raider",
      .executable = "ROTTR.exe",
      .num_options = 1,
      .options = application_620_options,
    },
    { .name = "NieR Replicant ver.1.22474487139",
      .executable = "NieR Replicant ver.1.22474487139.exe",
      .num_options = 1,
      .options = application_622_options,
    },
    { .name = "NieR:Automata",
      .executable = "NieRAutomata.exe",
      .num_options = 1,
      .options = application_624_options,
    },
    { .name = "Valheim",
      .executable = "valheim.x86_64",
      .num_options = 1,
      .options = application_626_options,
    },
    { .name = "Cyberpunk 2077",
      .executable = "Cyberpunk2077.exe",
      .num_options = 1,
      .options = application_628_options,
    },
    { .name = "Elden Ring",
      .executable = "eldenring.exe",
      .num_options = 1,
      .options = application_630_options,
    },
    { .name = "Armored Core 6",
      .executable = "armoredcore6.exe",
      .num_options = 1,
      .options = application_632_options,
    },
    { .name = "Age of Empires IV",
      .executable = "RelicCardinal.exe",
      .num_options = 1,
      .options = application_634_options,
    },
    { .name = "Dynasty Warriors: Origins",
      .executable = "DWORIGINS.exe",
      .num_options = 1,
      .options = application_636_options,
    },
    { .name = "EVE Online",
      .executable = "exefile.exe",
      .num_options = 1,
      .options = application_638_options,
    },
    { .name = "Marvel's Spider-Man Remastered",
      .executable = "Spider-Man.exe",
      .num_options = 2,
      .options = application_640_options,
    },
    { .name = "Hitman 3",
      .executable = "hitman3.exe",
      .num_options = 1,
      .options = application_643_options,
    },
    { .name = "Hogwarts Legacy",
      .executable = "HogwartsLegacy.exe",
      .num_options = 1,
      .options = application_645_options,
    },
    { .name = "DEATH STRANDING",
      .executable = "ds.exe",
      .num_options = 1,
      .options = application_647_options,
    },
    { .name = "Diablo IV",
      .executable = "Diablo IV.exe",
      .num_options = 1,
      .options = application_649_options,
    },
    { .name = "Faaast Penguin",
      .executable = "FaaastPenguinClient.exe",
      .num_options = 1,
      .options = application_651_options,
    },
    { .name = "Satisfactory Steam",
      .executable = "FactoryGameSteam-Win64-Shipping.exe",
      .num_options = 1,
      .options = application_653_options,
    },
    { .name = "Satisfactory EGS",
      .executable = "FactoryGameEGS-Win64-Shipping.exe",
      .num_options = 1,
      .options = application_655_options,
    },
    { .name = "Dying Light 2",
      .executable = "DyingLightGame_x64_rwdi.exe",
      .num_options = 1,
      .options = application_657_options,
    },
    { .name = "Witcher3",
      .executable = "witcher3.exe",
      .num_options = 1,
      .options = application_659_options,
    },
    { .name = "Baldur's Gate 3",
      .executable = "bg3.exe",
      .num_options = 1,
      .options = application_661_options,
    },
    { .name = "The Finals",
      .executable = "Discovery.exe",
      .num_options = 1,
      .options = application_663_options,
    },
    { .name = "Palworld2",
      .executable = "Palworld-Win64-Shipping.exe",
      .num_options = 1,
      .options = application_665_options,
    },
    { .name = "Red Dead Redemption 2",
      .executable = "RDR2.exe",
      .num_options = 1,
      .options = application_667_options,
    },
    { .name = "Shadow of the Tomb Raider",
      .executable = "SOTTR.exe",
      .num_options = 1,
      .options = application_669_options,
    },
    { .name = "Silent Hill 2",
      .executable = "SHProto-Win64-Shipping.exe",
      .num_options = 1,
      .options = application_671_options,
    },
    { .name = "Marvel Rivals",
      .executable = "Marvel-Win64-Shipping.exe",
      .num_options = 1,
      .options = application_673_options,
    },
    { .name = "Bellwright",
      .executable = "BellwrightGame-Win64-Shipping.exe",
      .num_options = 1,
      .options = application_675_options,
    },
    { .name = "A Game About Digging A Hole",
      .executable = "DiggingGame.exe",
      .num_options = 1,
      .options = application_677_options,
    },
    { .name = "DIRT 5",
      .executable = "DIRT5.exe",
      .num_options = 1,
      .options = application_679_options,
    },
    { .name = "X4 Foundations",
      .executable = "X4",
      .num_options = 1,
      .options = application_681_options,
    },
    { .name = "Total War: WARHAMMER III",
      .executable = "TotalWarhammer3",
      .num_options = 1,
      .options = application_683_options,
    },
};

static const struct driconf_device device_602 = {
    .driver = "anv",
    .num_engines = 5,
    .engines = device_602_engines,
    .num_applications = 40,
    .applications = device_602_applications,
};


    
static const struct driconf_option application_697_options[] = {
    { .name = "dzn_enable_8bit_loads_stores", .value = "true" },
    { .name = "dzn_claim_wide_lines", .value = "true" },
};

    
static const struct driconf_option application_700_options[] = {
    { .name = "dzn_claim_wide_lines", .value = "true" },
};

    
static const struct driconf_option application_702_options[] = {
    { .name = "dzn_disable", .value = "true" },
};

    
static const struct driconf_option application_704_options[] = {
    { .name = "dzn_disable", .value = "true" },
};


static const struct driconf_application device_696_applications[] = {
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 2,
      .options = application_697_options,
    },
    { .name = "No Man's Sky",
      .executable = "NMS.exe",
      .num_options = 1,
      .options = application_700_options,
    },
    { .name = "Red Dead Redemption 2",
      .executable = "RDR2.exe",
      .num_options = 1,
      .options = application_702_options,
    },
    { .name = "Baldur's Gate 3",
      .executable = "bg3.exe",
      .num_options = 1,
      .options = application_704_options,
    },
};

static const struct driconf_device device_696 = {
    .driver = "dzn",
    .num_engines = 0,
    .num_applications = 4,
    .applications = device_696_applications,
};


    
static const struct driconf_option application_707_options[] = {
    { .name = "mesa_extension_override", .value = "+GL_ARB_texture_view" },
};


static const struct driconf_application device_706_applications[] = {
    { .name = "Blender",
      .executable = "blender.exe",
      .num_options = 1,
      .options = application_707_options,
    },
};

static const struct driconf_device device_706 = {
    .driver = "d3d12",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_706_applications,
};


    
static const struct driconf_option application_710_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_712_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};

    
static const struct driconf_option application_714_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};


static const struct driconf_application device_709_applications[] = {
    { .name = "Artifact Classic",
      .executable = "dcg",
      .num_options = 1,
      .options = application_710_options,
    },
    { .name = "Deep Rock Galactic",
      .executable = "FSD.exe",
      .num_options = 1,
      .options = application_712_options,
    },
    { .name = "X4 Foundations",
      .executable = "X4",
      .num_options = 1,
      .options = application_714_options,
    },
};

static const struct driconf_device device_709 = {
    .driver = "nvk",
    .num_engines = 0,
    .num_applications = 3,
    .applications = device_709_applications,
};


    
static const struct driconf_option application_717_options[] = {
    { .name = "r300_nohiz", .value = "true" },
    { .name = "r300_nozmask", .value = "true" },
};

    
static const struct driconf_option application_720_options[] = {
    { .name = "r300_nohiz", .value = "true" },
    { .name = "r300_nozmask", .value = "true" },
};

    
static const struct driconf_option application_723_options[] = {
    { .name = "r300_nohiz", .value = "true" },
    { .name = "r300_nozmask", .value = "true" },
};

    
static const struct driconf_option application_726_options[] = {
    { .name = "r300_nohiz", .value = "true" },
    { .name = "r300_nozmask", .value = "true" },
};

    
static const struct driconf_option application_729_options[] = {
    { .name = "r300_nohiz", .value = "true" },
    { .name = "r300_nozmask", .value = "true" },
};

    
static const struct driconf_option application_732_options[] = {
    { .name = "r300_nohiz", .value = "true" },
    { .name = "r300_nozmask", .value = "true" },
};

    
static const struct driconf_option application_735_options[] = {
    { .name = "r300_nohiz", .value = "true" },
    { .name = "r300_nozmask", .value = "true" },
};

    
static const struct driconf_option application_738_options[] = {
    { .name = "r300_nohiz", .value = "true" },
    { .name = "r300_nozmask", .value = "true" },
};

    
static const struct driconf_option application_741_options[] = {
    { .name = "r300_ffmath", .value = "true" },
};

    
static const struct driconf_option application_743_options[] = {
    { .name = "r300_ffmath", .value = "true" },
};


static const struct driconf_application device_716_applications[] = {
    { .name = "X server",
      .executable = "X",
      .num_options = 2,
      .options = application_717_options,
    },
    { .name = "X server",
      .executable = "Xorg",
      .num_options = 2,
      .options = application_720_options,
    },
    { .name = "Compiz",
      .executable = "check_gl_texture_size",
      .num_options = 2,
      .options = application_723_options,
    },
    { .name = "Compiz",
      .executable = "Compiz",
      .num_options = 2,
      .options = application_726_options,
    },
    { .name = "GNOME Shell",
      .executable = "gnome-session-check-accelerated-helper",
      .num_options = 2,
      .options = application_729_options,
    },
    { .name = "GNOME Shell",
      .executable = "gnome-shell",
      .num_options = 2,
      .options = application_732_options,
    },
    { .name = "KWin",
      .executable = "kwin_opengl_test",
      .num_options = 2,
      .options = application_735_options,
    },
    { .name = "KWin",
      .executable = "kwin",
      .num_options = 2,
      .options = application_738_options,
    },
    { .name = "Unigine Tropics",
      .executable = "Tropics",
      .num_options = 1,
      .options = application_741_options,
    },
    { .name = "Oilrush",
      .executable = "Oilrush_x86",
      .num_options = 1,
      .options = application_743_options,
    },
};

static const struct driconf_device device_716 = {
    .driver = "r300",
    .num_engines = 0,
    .num_applications = 10,
    .applications = device_716_applications,
};


    
static const struct driconf_option application_746_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_ARB_shader_image_load_store" },
};


static const struct driconf_application device_745_applications[] = {
    { .name = "Tomb Raider 2013",
      .executable = "TombRaider",
      .num_options = 1,
      .options = application_746_options,
    },
};

static const struct driconf_device device_745 = {
    .driver = "r600",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_745_applications,
};


    
static const struct driconf_option application_749_options[] = {
    { .name = "format_l8_srgb_enable_readback", .value = "true" },
};


static const struct driconf_application device_748_applications[] = {
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_749_options,
    },
};

static const struct driconf_device device_748 = {
    .driver = "virtio_gpu",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_748_applications,
};


    
static const struct driconf_option application_752_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 618" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_755_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 630" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_758_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 618" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_761_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 630" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};


static const struct driconf_application device_751_applications[] = {
    { .name = "Call of Duty Mobile",
      .executable = "com.activision.callofduty.shooter",
      .num_options = 2,
      .options = application_752_options,
    },
    { .name = "Asphalt 8",
      .executable = "com.gameloft.android.ANMP.GloftA8HM",
      .num_options = 2,
      .options = application_755_options,
    },
    { .name = "Asphalt 9",
      .executable = "com.gameloft.android.ANMP.GloftA9HM",
      .num_options = 2,
      .options = application_758_options,
    },
    { .name = "PUBG Mobile",
      .executable = "com.tencent.ig",
      .num_options = 2,
      .options = application_761_options,
    },
};

static const struct driconf_device device_751 = {
    .driver = "msm",
    .device = "FD618",
    .num_engines = 0,
    .num_applications = 4,
    .applications = device_751_applications,
};
    
static const struct driconf_option engine_765_options[] = {
    { .name = "tu_allow_oob_indirect_ubo_loads", .value = "true" },
};

    
static const struct driconf_option engine_767_options[] = {
    { .name = "tu_disable_d24s8_border_color_workaround", .value = "true" },
};


static const struct driconf_engine device_764_engines[] = {
    { .engine_name_match = "DXVK",
      .engine_versions = "0:8400896",
      .num_options = 1,
      .options = engine_765_options,
    },
    { .engine_name_match = "DXVK|vkd3d",
      .num_options = 1,
      .options = engine_767_options,
    },
};



static const struct driconf_device device_764 = {
    .driver = "turnip",
    .num_engines = 2,
    .engines = device_764_engines,
    .num_applications = 0,
};
    
static const struct driconf_option engine_780_options[] = {
    { .name = "hk_disable_rgba4_border_color_workaround", .value = "true" },
};


static const struct driconf_engine device_769_engines[] = {
    { .engine_name_match = "DXVK",
      .num_options = 1,
      .options = engine_780_options,
    },
};

    
static const struct driconf_option application_770_options[] = {
    { .name = "hk_disable_border_emulation", .value = "true" },
};

    
static const struct driconf_option application_772_options[] = {
    { .name = "hk_disable_border_emulation", .value = "true" },
};

    
static const struct driconf_option application_774_options[] = {
    { .name = "hk_disable_border_emulation", .value = "true" },
};

    
static const struct driconf_option application_776_options[] = {
    { .name = "hk_disable_border_emulation", .value = "true" },
};

    
static const struct driconf_option application_778_options[] = {
    { .name = "hk_disable_border_emulation", .value = "true" },
};


static const struct driconf_application device_769_applications[] = {
    { .name = "vkcube",
      .executable = "vkcube",
      .num_options = 1,
      .options = application_770_options,
    },
    { .name = "vkmark",
      .executable = "vkmark",
      .num_options = 1,
      .options = application_772_options,
    },
    { .name = "Party Animals",
      .executable = "PartyAnimals.exe",
      .num_options = 1,
      .options = application_774_options,
    },
    { .name = "Castle Crashers",
      .executable = "castle.exe",
      .num_options = 1,
      .options = application_776_options,
    },
    { .name = "Control (DX11)",
      .executable = "Control_DX11.exe",
      .num_options = 1,
      .options = application_778_options,
    },
};

static const struct driconf_device device_769 = {
    .driver = "hk",
    .num_engines = 1,
    .engines = device_769_engines,
    .num_applications = 5,
    .applications = device_769_applications,
};
    
static const struct driconf_option engine_871_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_disable_aniso_single_level", .value = "true" },
    { .name = "radv_disable_trunc_coord", .value = "true" },
};

    
static const struct driconf_option engine_875_options[] = {
    { .name = "radv_disable_aniso_single_level", .value = "true" },
    { .name = "radv_disable_trunc_coord", .value = "true" },
};

    
static const struct driconf_option engine_878_options[] = {
    { .name = "radv_disable_aniso_single_level", .value = "true" },
};

    
static const struct driconf_option engine_880_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_disable_tc_compat_htile_general", .value = "true" },
    { .name = "radv_app_layer", .value = "quanticdream" },
};

    
static const struct driconf_option engine_884_options[] = {
    { .name = "radv_flush_before_query_copy", .value = "true" },
    { .name = "radv_flush_before_timestamp_write", .value = "true" },
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option engine_888_options[] = {
    { .name = "radv_tex_non_uniform", .value = "true" },
};


static const struct driconf_engine device_782_engines[] = {
    { .engine_name_match = "vkd3d",
      .num_options = 3,
      .options = engine_871_options,
    },
    { .engine_name_match = "DXVK",
      .num_options = 2,
      .options = engine_875_options,
    },
    { .engine_name_match = "mesa zink",
      .num_options = 1,
      .options = engine_878_options,
    },
    { .engine_name_match = "Quantic Dream Engine",
      .num_options = 3,
      .options = engine_880_options,
    },
    { .engine_name_match = "^UnrealEngine",
      .num_options = 3,
      .options = engine_884_options,
    },
    { .engine_name_match = "DXVK_NvRemix",
      .num_options = 1,
      .options = engine_888_options,
    },
};

    
static const struct driconf_option application_783_options[] = {
    { .name = "radv_report_llvm9_version_string", .value = "true" },
    { .name = "radv_invariant_geom", .value = "true" },
};

    
static const struct driconf_option application_786_options[] = {
    { .name = "radv_invariant_geom", .value = "true" },
    { .name = "radv_split_fma", .value = "true" },
};

    
static const struct driconf_option application_789_options[] = {
    { .name = "radv_enable_mrt_output_nan_fixup", .value = "true" },
    { .name = "radv_app_layer", .value = "rage2" },
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_793_options[] = {
    { .name = "radv_disable_shrink_image_store", .value = "true" },
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_796_options[] = {
    { .name = "radv_override_uniform_offset_alignment", .value = "16" },
    { .name = "radv_disable_shrink_image_store", .value = "true" },
    { .name = "radv_invariant_geom", .value = "true" },
};

    
static const struct driconf_option application_800_options[] = {
    { .name = "radv_no_dynamic_bounds", .value = "true" },
};

    
static const struct driconf_option application_802_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_legacy_sparse_binding", .value = "true" },
};

    
static const struct driconf_option application_805_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_legacy_sparse_binding", .value = "true" },
    { .name = "radv_disable_dcc_stores", .value = "true" },
    { .name = "radv_invariant_geom", .value = "true" },
    { .name = "radv_lower_terminate_to_discard", .value = "true" },
};

    
static const struct driconf_option application_811_options[] = {
    { .name = "radv_disable_dcc", .value = "true" },
    { .name = "radv_invariant_geom", .value = "true" },
};

    
static const struct driconf_option application_814_options[] = {
    { .name = "radv_force_64k_sparse_alignment", .value = "true" },
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_legacy_sparse_binding", .value = "true" },
    { .name = "radv_disable_dcc_stores", .value = "true" },
};

    
static const struct driconf_option application_819_options[] = {
    { .name = "radv_disable_dcc", .value = "true" },
};

    
static const struct driconf_option application_821_options[] = {
    { .name = "radv_disable_sinking_load_input_fs", .value = "true" },
};

    
static const struct driconf_option application_823_options[] = {
    { .name = "radv_disable_sinking_load_input_fs", .value = "true" },
};

    
static const struct driconf_option application_825_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_827_options[] = {
    { .name = "radv_enable_unified_heap_on_apu", .value = "true" },
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_disable_dcc_mips", .value = "true" },
};

    
static const struct driconf_option application_831_options[] = {
    { .name = "radv_app_layer", .value = "metroexodus" },
};

    
static const struct driconf_option application_833_options[] = {
    { .name = "radv_rt_wave64", .value = "true" },
};

    
static const struct driconf_option application_835_options[] = {
    { .name = "radv_invariant_geom", .value = "true" },
};

    
static const struct driconf_option application_837_options[] = {
    { .name = "radv_ssbo_non_uniform", .value = "true" },
};

    
static const struct driconf_option application_839_options[] = {
    { .name = "radv_ssbo_non_uniform", .value = "true" },
};

    
static const struct driconf_option application_841_options[] = {
    { .name = "radv_disable_ngg_gs", .value = "true" },
};

    
static const struct driconf_option application_843_options[] = {
    { .name = "radv_force_pstate_peak_gfx11_dgpu", .value = "true" },
};

    
static const struct driconf_option application_845_options[] = {
    { .name = "radv_disable_dcc", .value = "true" },
};

    
static const struct driconf_option application_847_options[] = {
    { .name = "radv_disable_hiz_his_gfx12", .value = "true" },
};

    
static const struct driconf_option application_849_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_851_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_853_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_855_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_857_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_859_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_861_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_863_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_865_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_867_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_869_options[] = {
    { .name = "radv_disable_depth_storage", .value = "true" },
};


static const struct driconf_application device_782_applications[] = {
    { .name = "Shadow Of The Tomb Raider (Native)",
      .application_name_match = "ShadowOfTheTomb",
      .num_options = 2,
      .options = application_783_options,
    },
    { .name = "Shadow Of The Tomb Raider (DX11/DX12)",
      .application_name_match = "SOTTR.exe",
      .num_options = 2,
      .options = application_786_options,
    },
    { .name = "RAGE 2",
      .application_name_match = "Rage 2",
      .num_options = 3,
      .options = application_789_options,
    },
    { .name = "The Surge 2",
      .application_name_match = "Fledge",
      .num_options = 2,
      .options = application_793_options,
    },
    { .name = "World War Z (and World War Z: Aftermath)",
      .application_name_match = "WWZ|wwz",
      .num_options = 3,
      .options = application_796_options,
    },
    { .name = "DOOM VFR",
      .application_name_match = "DOOM_VFR",
      .num_options = 1,
      .options = application_800_options,
    },
    { .name = "DOOM Eternal",
      .application_name_match = "DOOMEternal",
      .num_options = 2,
      .options = application_802_options,
    },
    { .name = "Indiana Jones: The Great Circle",
      .application_name_match = "TheGreatCircle",
      .num_options = 5,
      .options = application_805_options,
    },
    { .name = "DOOM (2016)",
      .application_name_match = "DOOM$",
      .num_options = 2,
      .options = application_811_options,
    },
    { .name = "DOOM: The Dark Ages",
      .application_name_match = "DOOMTheDarkAges",
      .num_options = 4,
      .options = application_814_options,
    },
    { .name = "Wolfenstein II",
      .application_name_match = "Wolfenstein II The New Colossus",
      .num_options = 1,
      .options = application_819_options,
    },
    { .name = "Grid Autosport",
      .application_name_match = "GRIDAutosport_avx.exe",
      .num_options = 1,
      .options = application_821_options,
    },
    { .name = "Grid Autosport",
      .application_name_match = "GRIDAutosport.exe",
      .num_options = 1,
      .options = application_823_options,
    },
    { .name = "Hammerting",
      .application_name_match = "TDTD",
      .num_options = 1,
      .options = application_825_options,
    },
    { .name = "RDR2",
      .application_name_match = "Red Dead Redemption 2",
      .num_options = 3,
      .options = application_827_options,
    },
    { .name = "Metro Exodus (Linux native)",
      .application_name_match = "metroexodus",
      .num_options = 1,
      .options = application_831_options,
    },
    { .name = "Hellblade: Senua's Sacrifice",
      .application_name_match = "HellbladeGame-Win64-Shipping.exe",
      .num_options = 1,
      .options = application_833_options,
    },
    { .name = "War Thunder",
      .application_name_match = "WarThunder",
      .num_options = 1,
      .options = application_835_options,
    },
    { .name = "Crysis 2 Remastered",
      .application_name_match = "Crysis2Remastered.exe",
      .num_options = 1,
      .options = application_837_options,
    },
    { .name = "Crysis 3 Remastered",
      .application_name_match = "Crysis3Remastered.exe",
      .num_options = 1,
      .options = application_839_options,
    },
    { .name = "Persona 3 Reload",
      .application_name_match = "P3R.exe",
      .num_options = 1,
      .options = application_841_options,
    },
    { .name = "Helldivers 2",
      .executable = "helldivers2.exe",
      .num_options = 1,
      .options = application_843_options,
    },
    { .name = "WWE 2k23",
      .application_name_match = "WWE2K23_x64.exe",
      .num_options = 1,
      .options = application_845_options,
    },
    { .name = "Mafia Definitive Edition",
      .application_name_match = "mafiadefinitiveedition.exe",
      .num_options = 1,
      .options = application_847_options,
    },
    { .name = "Black Geyser: Couriers of Darkness",
      .executable = "BlackGeyser.x86_64",
      .num_options = 1,
      .options = application_849_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_851_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_853_options,
    },
    { .name = "Monolith demo5",
      .executable = "runner",
      .sha1 = "0c40d509a74e357f0280cb1bd882e9cd94b91bdf",
      .num_options = 1,
      .options = application_855_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_857_options,
    },
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_859_options,
    },
    { .name = "Exanima",
      .executable = "Exanima.exe",
      .num_options = 1,
      .options = application_861_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_863_options,
    },
    { .name = "Crystal Project",
      .executable = "Crystal Project.bin.x86_64",
      .num_options = 1,
      .options = application_865_options,
    },
    { .name = "Enshrouded",
      .executable = "enshrouded.exe",
      .num_options = 1,
      .options = application_867_options,
    },
    { .name = "Total War: WARHAMMER III",
      .application_name_match = "TotalWarhammer3",
      .num_options = 1,
      .options = application_869_options,
    },
};

static const struct driconf_device device_782 = {
    .driver = "radv",
    .num_engines = 6,
    .engines = device_782_engines,
    .num_applications = 35,
    .applications = device_782_applications,
};

static const struct driconf_device *driconf[] = {
    &device_1,
    &device_508,
    &device_515,
    &device_571,
    &device_584,
    &device_595,
    &device_602,
    &device_696,
    &device_706,
    &device_709,
    &device_716,
    &device_745,
    &device_748,
    &device_751,
    &device_764,
    &device_769,
    &device_782,
};
