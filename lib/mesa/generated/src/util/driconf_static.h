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



    
static const struct driconf_option engine_448_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};


static const struct driconf_engine device_1_engines[] = {
    { .engine_name_match = "UnrealEngine4.*",
      .engine_versions = "0:23",
      .num_options = 1,
      .options = engine_448_options,
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
    { .name = "disable_xcb_surface", .value = "true" },
};

    
static const struct driconf_option application_14_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_17_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_20_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_23_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_26_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_29_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_32_options[] = {
    { .name = "disable_blend_func_extended", .value = "true" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_35_options[] = {
    { .name = "disable_blend_func_extended", .value = "true" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_38_options[] = {
    { .name = "disable_glsl_line_continuations", .value = "true" },
};

    
static const struct driconf_option application_40_options[] = {
    { .name = "always_have_depth_buffer", .value = "true" },
};

    
static const struct driconf_option application_42_options[] = {
    { .name = "always_have_depth_buffer", .value = "true" },
};

    
static const struct driconf_option application_44_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_46_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_48_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_50_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_53_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_55_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
    { .name = "force_glsl_extensions_warn", .value = "true" },
};

    
static const struct driconf_option application_58_options[] = {
    { .name = "disable_uniform_array_resize", .value = "true" },
};

    
static const struct driconf_option application_60_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_62_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_64_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_66_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
    { .name = "force_gl_map_buffer_synchronized", .value = "true" },
};

    
static const struct driconf_option application_69_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
    { .name = "force_gl_map_buffer_synchronized", .value = "true" },
};

    
static const struct driconf_option application_72_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_74_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_76_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_78_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_80_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
    { .name = "force_gl_vendor", .value = "ATI Technologies, Inc." },
};

    
static const struct driconf_option application_84_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_86_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_88_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_90_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_92_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_94_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_96_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_98_options[] = {
    { .name = "force_compat_shaders", .value = "true" },
};

    
static const struct driconf_option application_100_options[] = {
    { .name = "force_compat_shaders", .value = "true" },
};

    
static const struct driconf_option application_102_options[] = {
    { .name = "force_glsl_version", .value = "440" },
};

    
static const struct driconf_option application_104_options[] = {
    { .name = "force_glsl_abs_sqrt", .value = "true" },
};

    
static const struct driconf_option application_106_options[] = {
    { .name = "force_glsl_abs_sqrt", .value = "true" },
};

    
static const struct driconf_option application_108_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_110_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_112_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_114_options[] = {
    { .name = "force_glsl_version", .value = "130" },
    { .name = "glsl_ignore_write_to_readonly_var", .value = "true" },
};

    
static const struct driconf_option application_117_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_119_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_121_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_123_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_125_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_127_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_129_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_131_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_133_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_135_options[] = {
    { .name = "allow_glsl_builtin_const_expression", .value = "true" },
    { .name = "allow_glsl_relaxed_es", .value = "true" },
};

    
static const struct driconf_option application_138_options[] = {
    { .name = "allow_extra_pp_tokens", .value = "true" },
};

    
static const struct driconf_option application_140_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_142_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_144_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_146_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_148_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_150_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
};

    
static const struct driconf_option application_152_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
};

    
static const struct driconf_option application_154_options[] = {
    { .name = "vs_position_always_invariant", .value = "true" },
};

    
static const struct driconf_option application_156_options[] = {
    { .name = "lower_depth_range_rate", .value = "0.8" },
};

    
static const struct driconf_option application_158_options[] = {
    { .name = "vblank_mode", .value = "0" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
    { .name = "force_gl_names_reuse", .value = "true" },
    { .name = "force_gl_vendor", .value = "NVIDIA Corporation" },
    { .name = "force_glsl_extensions_warn", .value = "true" },
    { .name = "mesa_glthread", .value = "true" },
    { .name = "mesa_no_error", .value = "true" },
    { .name = "allow_rgb10_configs", .value = "false" },
    { .name = "allow_invalid_glx_destroy_window", .value = "true" },
};

    
static const struct driconf_option application_169_options[] = {
    { .name = "ignore_map_unsynchronized", .value = "true" },
};

    
static const struct driconf_option application_171_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_174_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_176_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_178_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_180_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_182_options[] = {
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
};

    
static const struct driconf_option application_184_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
};

    
static const struct driconf_option application_186_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
    { .name = "do_dce_before_clip_cull_analysis", .value = "true" },
};

    
static const struct driconf_option application_189_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
};

    
static const struct driconf_option application_191_options[] = {
    { .name = "force_glsl_extensions_warn", .value = "true" },
};

    
static const struct driconf_option application_193_options[] = {
    { .name = "force_direct_glx_context", .value = "true" },
};

    
static const struct driconf_option application_195_options[] = {
    { .name = "keep_native_window_glx_drawable", .value = "true" },
    { .name = "allow_rgb10_configs", .value = "false" },
};

    
static const struct driconf_option application_198_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_200_options[] = {
    { .name = "force_compat_shaders", .value = "true" },
};

    
static const struct driconf_option application_202_options[] = {
    { .name = "mesa_glthread", .value = "false" },
};

    
static const struct driconf_option application_204_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_206_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_208_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_210_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_212_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_214_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_216_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_218_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_220_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_222_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_224_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_226_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_228_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_230_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_232_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_234_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_236_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_238_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_240_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_242_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_244_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_246_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_248_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_250_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_252_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_254_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_256_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_258_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_260_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_262_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_264_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_266_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_268_options[] = {
    { .name = "mesa_glthread", .value = "true" },
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
};

    
static const struct driconf_option application_271_options[] = {
    { .name = "mesa_glthread", .value = "true" },
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
};

    
static const struct driconf_option application_274_options[] = {
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
};

    
static const struct driconf_option application_276_options[] = {
    { .name = "vk_wsi_force_swapchain_to_current_extent", .value = "true" },
};

    
static const struct driconf_option application_278_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_280_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_282_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_284_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_286_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_288_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_290_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_292_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_294_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_296_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_298_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_300_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_302_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_304_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_306_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_308_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_310_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_312_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_314_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_316_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_318_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_320_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_322_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_324_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_326_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_328_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_330_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_332_options[] = {
    { .name = "mesa_glthread", .value = "false" },
};

    
static const struct driconf_option application_334_options[] = {
    { .name = "mesa_glthread", .value = "false" },
};

    
static const struct driconf_option application_336_options[] = {
    { .name = "mesa_glthread", .value = "false" },
};

    
static const struct driconf_option application_338_options[] = {
    { .name = "adaptive_sync", .value = "false" },
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_341_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_343_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_345_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_347_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_349_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_351_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_353_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_355_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_357_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_359_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_361_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_363_options[] = {
    { .name = "adaptive_sync", .value = "false" },
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
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_374_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_376_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_378_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_380_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_382_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_384_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_386_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_388_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_390_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_392_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_394_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_396_options[] = {
    { .name = "adaptive_sync", .value = "false" },
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
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_408_options[] = {
    { .name = "mesa_glthread", .value = "false" },
    { .name = "vk_x11_override_min_image_count", .value = "2" },
    { .name = "vk_wsi_force_bgra8_unorm_first", .value = "true" },
};

    
static const struct driconf_option application_412_options[] = {
    { .name = "vk_x11_override_min_image_count", .value = "2" },
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_415_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_OML_swap_method" },
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_418_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_420_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_422_options[] = {
    { .name = "force_gl_names_reuse", .value = "true" },
};

    
static const struct driconf_option application_424_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_426_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_428_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_430_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_432_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_434_options[] = {
    { .name = "ignore_discard_framebuffer", .value = "true" },
};

    
static const struct driconf_option application_436_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_438_options[] = {
    { .name = "vk_x11_ensure_min_image_count", .value = "true" },
};

    
static const struct driconf_option application_440_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_442_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_444_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_446_options[] = {
    { .name = "vk_dont_care_as_load", .value = "true" },
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
    { .name = "Xwayland",
      .executable = "Xwayland",
      .num_options = 1,
      .options = application_12_options,
    },
    { .name = "Unigine Heaven (32-bit)",
      .executable = "heaven_x86",
      .num_options = 2,
      .options = application_14_options,
    },
    { .name = "Unigine Heaven (64-bit)",
      .executable = "heaven_x64",
      .num_options = 2,
      .options = application_17_options,
    },
    { .name = "Unigine Heaven (Windows)",
      .executable = "heaven.exe",
      .num_options = 2,
      .options = application_20_options,
    },
    { .name = "Unigine Valley (32-bit)",
      .executable = "valley_x86",
      .num_options = 2,
      .options = application_23_options,
    },
    { .name = "Unigine Valley (64-bit)",
      .executable = "valley_x64",
      .num_options = 2,
      .options = application_26_options,
    },
    { .name = "Unigine Valley (Windows)",
      .executable = "valley.exe",
      .num_options = 2,
      .options = application_29_options,
    },
    { .name = "Unigine OilRush (32-bit)",
      .executable = "OilRush_x86",
      .num_options = 2,
      .options = application_32_options,
    },
    { .name = "Unigine OilRush (64-bit)",
      .executable = "OilRush_x64",
      .num_options = 2,
      .options = application_35_options,
    },
    { .name = "Savage 2",
      .executable = "savage2.bin",
      .num_options = 1,
      .options = application_38_options,
    },
    { .name = "Topogun (32-bit)",
      .executable = "topogun32",
      .num_options = 1,
      .options = application_40_options,
    },
    { .name = "Topogun (64-bit)",
      .executable = "topogun64",
      .num_options = 1,
      .options = application_42_options,
    },
    { .name = "Half Life 2",
      .executable = "hl2_linux",
      .num_options = 1,
      .options = application_44_options,
    },
    { .name = "Black Mesa",
      .executable = "bms_linux",
      .num_options = 1,
      .options = application_46_options,
    },
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_48_options,
    },
    { .name = "Dead Island (incl. Definitive Edition)",
      .executable = "DeadIslandGame",
      .num_options = 2,
      .options = application_50_options,
    },
    { .name = "Dead Island Riptide Definitive Edition",
      .executable = "DeadIslandRiptideGame",
      .num_options = 1,
      .options = application_53_options,
    },
    { .name = "Doom 3: BFG",
      .executable = "Doom3BFG.exe",
      .num_options = 2,
      .options = application_55_options,
    },
    { .name = "Dune: Spice Wars",
      .executable = "D4X.exe",
      .num_options = 1,
      .options = application_58_options,
    },
    { .name = "Dying Light",
      .executable = "DyingLightGame",
      .num_options = 1,
      .options = application_60_options,
    },
    { .name = "Exanima",
      .executable = "Exanima.exe",
      .num_options = 1,
      .options = application_62_options,
    },
    { .name = "Full Bore",
      .executable = "fullbore",
      .num_options = 1,
      .options = application_64_options,
    },
    { .name = "RAGE (64-bit)",
      .executable = "Rage64.exe",
      .num_options = 2,
      .options = application_66_options,
    },
    { .name = "RAGE (32-bit)",
      .executable = "Rage.exe",
      .num_options = 2,
      .options = application_69_options,
    },
    { .name = "Second Life",
      .executable = "do-not-directly-run-secondlife-bin",
      .num_options = 1,
      .options = application_72_options,
    },
    { .name = "Warsow (32-bit)",
      .executable = "warsow.i386",
      .num_options = 1,
      .options = application_74_options,
    },
    { .name = "Warsow (64-bit)",
      .executable = "warsow.x86_64",
      .num_options = 1,
      .options = application_76_options,
    },
    { .name = "Rust",
      .executable = "rust",
      .num_options = 1,
      .options = application_78_options,
    },
    { .name = "Divinity: Original Sin Enhanced Edition",
      .executable = "EoCApp",
      .num_options = 3,
      .options = application_80_options,
    },
    { .name = "Metro 2033 Redux / Metro Last Night Redux",
      .executable = "metro",
      .num_options = 1,
      .options = application_84_options,
    },
    { .name = "Worms W.M.D",
      .executable = "Worms W.M.Dx64",
      .num_options = 1,
      .options = application_86_options,
    },
    { .name = "Crookz - The Big Heist",
      .executable = "Crookz",
      .num_options = 1,
      .options = application_88_options,
    },
    { .name = "Tropico 5",
      .executable = "Tropico5",
      .num_options = 1,
      .options = application_90_options,
    },
    { .name = "Faster than Light (32-bit)",
      .executable = "FTL.x86",
      .num_options = 1,
      .options = application_92_options,
    },
    { .name = "Faster than Light (64-bit)",
      .executable = "FTL.amd64",
      .num_options = 1,
      .options = application_94_options,
    },
    { .name = "Final Fantasy VIII: Remastered",
      .executable = "FFVIII.exe",
      .num_options = 1,
      .options = application_96_options,
    },
    { .name = "SNK HEROINES Tag Team Frenzy",
      .executable = "SNKHEROINES.exe",
      .num_options = 1,
      .options = application_98_options,
    },
    { .name = "Metal Slug XX",
      .executable = "MSXX_main.exe",
      .num_options = 1,
      .options = application_100_options,
    },
    { .name = "The Culling",
      .executable = "Victory",
      .num_options = 1,
      .options = application_102_options,
    },
    { .name = "Spec Ops: The Line (32-bit)",
      .executable = "specops.i386",
      .num_options = 1,
      .options = application_104_options,
    },
    { .name = "Spec Ops: The Line (64-bit)",
      .executable = "specops",
      .num_options = 1,
      .options = application_106_options,
    },
    { .name = "Jamestown+",
      .executable = "JamestownPlus.exe",
      .num_options = 1,
      .options = application_108_options,
    },
    { .name = "Kerbal Space Program (32-bit)",
      .executable = "KSP.x86",
      .num_options = 1,
      .options = application_110_options,
    },
    { .name = "Kerbal Space Program (64-bit)",
      .executable = "KSP.x86_64",
      .num_options = 1,
      .options = application_112_options,
    },
    { .name = "Luna Sky",
      .executable = "lunasky",
      .num_options = 2,
      .options = application_114_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_117_options,
    },
    { .name = "The Witcher 2",
      .executable = "witcher2",
      .num_options = 1,
      .options = application_119_options,
    },
    { .name = "Unreal 4 Editor",
      .executable = "UE4Editor",
      .num_options = 1,
      .options = application_121_options,
    },
    { .name = "Observer",
      .executable = "TheObserver-Linux-Shipping",
      .num_options = 1,
      .options = application_123_options,
    },
    { .name = "Pixel Game Maker MV",
      .executable = "player.exe",
      .num_options = 1,
      .options = application_125_options,
    },
    { .name = "Eden Gamma",
      .executable = "EdenGammaGame-0.3.0.2.exe",
      .num_options = 1,
      .options = application_127_options,
    },
    { .name = "Steamroll",
      .executable = "Steamroll-Linux-Shipping",
      .num_options = 1,
      .options = application_129_options,
    },
    { .name = "Refunct",
      .executable = "Refunct-Linux-Shipping",
      .num_options = 1,
      .options = application_131_options,
    },
    { .name = "We Happy Few",
      .executable = "GlimpseGame",
      .num_options = 1,
      .options = application_133_options,
    },
    { .name = "Google Earth VR",
      .executable = "Earth.exe",
      .num_options = 2,
      .options = application_135_options,
    },
    { .name = "Champions of Regnum",
      .executable = "game",
      .num_options = 1,
      .options = application_138_options,
    },
    { .name = "Wolfenstein The Old Blood",
      .executable = "WolfOldBlood_x64.exe",
      .num_options = 1,
      .options = application_140_options,
    },
    { .name = "ARMA 3",
      .executable = "arma3.x86_64",
      .num_options = 1,
      .options = application_142_options,
    },
    { .name = "Epic Games Launcher",
      .executable = "EpicGamesLauncher.exe",
      .num_options = 1,
      .options = application_144_options,
    },
    { .name = "GpuTest",
      .executable = "GpuTest",
      .num_options = 1,
      .options = application_146_options,
    },
    { .name = "Curse of the Dead Gods",
      .executable = "Curse of the Dead Gods.exe",
      .num_options = 1,
      .options = application_148_options,
    },
    { .name = "GRID Autosport",
      .executable = "GridAutosport",
      .num_options = 1,
      .options = application_150_options,
    },
    { .name = "DIRT: Showdown",
      .executable = "dirt.i386",
      .num_options = 1,
      .options = application_152_options,
    },
    { .name = "DiRT Rally",
      .executable = "DirtRally",
      .num_options = 1,
      .options = application_154_options,
    },
    { .name = "Homerun Clash",
      .executable = "com.haegin.homerunclash",
      .num_options = 1,
      .options = application_156_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 10,
      .options = application_158_options,
    },
    { .name = "Dead-Cells",
      .executable = "com.playdigious.deadcells.mobile",
      .num_options = 1,
      .options = application_169_options,
    },
    { .name = "Teardown",
      .executable = "teardown.exe",
      .num_options = 2,
      .options = application_171_options,
    },
    { .name = "Golf With Your Friends",
      .executable = "Golf With Your Friends.x86_64",
      .num_options = 1,
      .options = application_174_options,
    },
    { .name = "Cossacks 3",
      .executable = "cossacks.exe",
      .num_options = 1,
      .options = application_176_options,
    },
    { .name = "Kaiju-A-Gogo",
      .executable = "kaiju.exe",
      .num_options = 1,
      .options = application_178_options,
    },
    { .name = "Captain Lycop: Invasion of the Heters (Wine)",
      .executable = "lycop.exe",
      .num_options = 1,
      .options = application_180_options,
    },
    { .name = "Captain Lycop: Invasion of the Heters",
      .executable = "lycop",
      .num_options = 1,
      .options = application_182_options,
    },
    { .name = "BETA CAE Systems - GL detect tool",
      .executable = "detect_opengl_tool",
      .num_options = 1,
      .options = application_184_options,
    },
    { .name = "BETA CAE Systems - ANSA",
      .executable = "ansa_linux_x86_64",
      .num_options = 2,
      .options = application_186_options,
    },
    { .name = "BETA CAE Systems - META",
      .executable = "meta_post_x86_64",
      .num_options = 1,
      .options = application_189_options,
    },
    { .name = "Mari",
      .executable_regexp = "Mari[0-9]+[.][0-9]+v[0-9]+",
      .num_options = 1,
      .options = application_191_options,
    },
    { .name = "Discovery Studio 2020",
      .executable = "DiscoveryStudio2020-bin",
      .num_options = 1,
      .options = application_193_options,
    },
    { .name = "Abaqus",
      .executable = "ABQcaeK",
      .num_options = 2,
      .options = application_195_options,
    },
    { .name = "Maya",
      .executable = "maya.bin",
      .num_options = 1,
      .options = application_198_options,
    },
    { .name = "SD Gundam G Generation Cross Rays",
      .executable = "togg.exe",
      .num_options = 1,
      .options = application_200_options,
    },
    { .name = "FINAL FANTASY XI",
      .executable = "pol.exe",
      .num_options = 1,
      .options = application_202_options,
    },
    { .name = "Minecraft",
      .executable = "minecraft-launcher",
      .num_options = 1,
      .options = application_204_options,
    },
    { .name = "Minecraft-FTB",
      .executable = "ftb-app",
      .num_options = 1,
      .options = application_206_options,
    },
    { .name = "Alien Isolation",
      .executable = "AlienIsolation",
      .num_options = 1,
      .options = application_208_options,
    },
    { .name = "BioShock Infinite",
      .executable = "bioshock.i386",
      .num_options = 1,
      .options = application_210_options,
    },
    { .name = "Borderlands 2",
      .executable = "Borderlands2",
      .num_options = 1,
      .options = application_212_options,
    },
    { .name = "Civilization 5",
      .executable = "Civ5XP",
      .num_options = 1,
      .options = application_214_options,
    },
    { .name = "Civilization 6",
      .executable = "Civ6",
      .num_options = 1,
      .options = application_216_options,
    },
    { .name = "Civilization 6",
      .executable = "Civ6Sub",
      .num_options = 1,
      .options = application_218_options,
    },
    { .name = "Dreamfall Chapters",
      .executable = "Dreamfall Chapters",
      .num_options = 1,
      .options = application_220_options,
    },
    { .name = "Hitman",
      .executable = "HitmanPro",
      .num_options = 1,
      .options = application_222_options,
    },
    { .name = "Renowned Explorers: International Society",
      .executable = "abbeycore_steam",
      .num_options = 1,
      .options = application_224_options,
    },
    { .name = "Saints Row 2",
      .executable = "saintsrow2.i386",
      .num_options = 1,
      .options = application_226_options,
    },
    { .name = "Saints Row: The Third",
      .executable = "SaintsRow3.i386",
      .num_options = 1,
      .options = application_228_options,
    },
    { .name = "Saints Row IV",
      .executable = "SaintsRow4.i386",
      .num_options = 1,
      .options = application_230_options,
    },
    { .name = "Saints Row: Gat out of Hell",
      .executable = "SaintsRow4GooH.i386",
      .num_options = 1,
      .options = application_232_options,
    },
    { .name = "Sid Meier's: Civilization Beyond Earth",
      .executable = "CivBE",
      .num_options = 1,
      .options = application_234_options,
    },
    { .name = "The Witcher 2",
      .executable = "witcher2",
      .num_options = 1,
      .options = application_236_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_238_options,
    },
    { .name = "Euro Truck Simulator 2",
      .executable = "eurotrucks2",
      .num_options = 1,
      .options = application_240_options,
    },
    { .name = "Overlord",
      .executable = "overlord.i386",
      .num_options = 1,
      .options = application_242_options,
    },
    { .name = "Overlord 2",
      .executable = "overlord2.i386",
      .num_options = 1,
      .options = application_244_options,
    },
    { .name = "Oil Rush",
      .executable = "OilRush_x86",
      .num_options = 1,
      .options = application_246_options,
    },
    { .name = "War Thunder",
      .executable = "aces",
      .num_options = 1,
      .options = application_248_options,
    },
    { .name = "War Thunder (Wine)",
      .executable = "aces.exe",
      .num_options = 1,
      .options = application_250_options,
    },
    { .name = "Outlast",
      .executable = "OLGame.x86_64",
      .num_options = 1,
      .options = application_252_options,
    },
    { .name = "Spec Ops: The Line (32-bit)",
      .executable = "specops.i386",
      .num_options = 1,
      .options = application_254_options,
    },
    { .name = "Spec Ops: The Line (64-bit)",
      .executable = "specops",
      .num_options = 1,
      .options = application_256_options,
    },
    { .name = "Mount and Blade Warband",
      .executable = "mb_warband_linux",
      .num_options = 1,
      .options = application_258_options,
    },
    { .name = "Valheim",
      .executable = "valheim.x86_64",
      .num_options = 1,
      .options = application_260_options,
    },
    { .name = "Medieval II: Total War",
      .executable = "Medieval2",
      .num_options = 1,
      .options = application_262_options,
    },
    { .name = "Carnivores: Dinosaur Hunter Reborn (wine)",
      .executable = "Carnivores-master.exe",
      .num_options = 1,
      .options = application_264_options,
    },
    { .name = "Far Cry 2 (wine)",
      .executable = "farcry2.exe",
      .num_options = 1,
      .options = application_266_options,
    },
    { .name = "Talos Principle",
      .executable = "Talos",
      .num_options = 2,
      .options = application_268_options,
    },
    { .name = "Talos Principle (Unrestricted)",
      .executable = "Talos_Unrestricted",
      .num_options = 2,
      .options = application_271_options,
    },
    { .name = "Serious Sam Fusion",
      .executable = "Sam2017",
      .num_options = 1,
      .options = application_274_options,
    },
    { .name = "Serious Sam Fusion (Unrestricted)",
      .executable = "Sam2017_Unrestricted",
      .num_options = 1,
      .options = application_276_options,
    },
    { .name = "7 Days To Die (64-bit)",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_278_options,
    },
    { .name = "7 Days To Die (32-bit)",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_280_options,
    },
    { .name = "Dolphin Emulator",
      .executable = "dolphin-emu",
      .num_options = 1,
      .options = application_282_options,
    },
    { .name = "Citra - Nintendo 3DS Emulator",
      .executable = "citra-qt",
      .num_options = 1,
      .options = application_284_options,
    },
    { .name = "Yuzu - Nintendo Switch Emulator",
      .executable = "yuzu",
      .num_options = 1,
      .options = application_286_options,
    },
    { .name = "RPCS3",
      .executable = "rpcs3",
      .num_options = 1,
      .options = application_288_options,
    },
    { .name = "PCSX2",
      .executable = "PCSX2",
      .num_options = 1,
      .options = application_290_options,
    },
    { .name = "From The Depths",
      .executable = "From_The_Depths.x86_64",
      .num_options = 1,
      .options = application_292_options,
    },
    { .name = "Plague Inc Evolved (32-bit)",
      .executable = "PlagueIncEvolved.x86",
      .num_options = 1,
      .options = application_294_options,
    },
    { .name = "Plague Inc Evolved (64-bit)",
      .executable = "PlagueIncEvolved.x86_64",
      .num_options = 1,
      .options = application_296_options,
    },
    { .name = "Beholder (32-bit)",
      .executable = "Beholder.x86",
      .num_options = 1,
      .options = application_298_options,
    },
    { .name = "Beholder (64-bit)",
      .executable = "Beholder.x86_64",
      .num_options = 1,
      .options = application_300_options,
    },
    { .name = "X3 Reunion",
      .executable = "X3R_main",
      .num_options = 1,
      .options = application_302_options,
    },
    { .name = "X3 Terran Conflict",
      .executable = "X3TR_main",
      .num_options = 1,
      .options = application_304_options,
    },
    { .name = "X3 Albion Prelude",
      .executable = "X3AP_main",
      .num_options = 1,
      .options = application_306_options,
    },
    { .name = "Borderlands: The Pre-Sequel",
      .executable = "BorderlandsPreSequel",
      .num_options = 1,
      .options = application_308_options,
    },
    { .name = "Transport Fever",
      .executable = "TransportFever",
      .num_options = 1,
      .options = application_310_options,
    },
    { .name = "Unigine Sanctuary",
      .executable = "Sanctuary",
      .num_options = 1,
      .options = application_312_options,
    },
    { .name = "Unigine Tropics",
      .executable = "Tropics",
      .num_options = 1,
      .options = application_314_options,
    },
    { .name = "Unigine Heaven (32-bit)",
      .executable = "heaven_x86",
      .num_options = 1,
      .options = application_316_options,
    },
    { .name = "Unigine Heaven (64-bit)",
      .executable = "heaven_x64",
      .num_options = 1,
      .options = application_318_options,
    },
    { .name = "Unigine Valley (32-bit)",
      .executable = "valley_x86",
      .num_options = 1,
      .options = application_320_options,
    },
    { .name = "Unigine Valley (64-bit)",
      .executable = "valley_x64",
      .num_options = 1,
      .options = application_322_options,
    },
    { .name = "Unigine Superposition",
      .executable = "superposition",
      .num_options = 1,
      .options = application_324_options,
    },
    { .name = "Basemark GPU",
      .executable = "BasemarkGPU_gl",
      .num_options = 1,
      .options = application_326_options,
    },
    { .name = "Stellaris",
      .executable = "stellaris",
      .num_options = 1,
      .options = application_328_options,
    },
    { .name = "Battletech",
      .executable = "BattleTech",
      .num_options = 1,
      .options = application_330_options,
    },
    { .name = "DeusExMD",
      .executable = "DeusExMD",
      .num_options = 1,
      .options = application_332_options,
    },
    { .name = "F1 2015",
      .executable = "F12015",
      .num_options = 1,
      .options = application_334_options,
    },
    { .name = "KWin Wayland",
      .executable = "kwin_wayland",
      .num_options = 1,
      .options = application_336_options,
    },
    { .name = "gnome-shell",
      .executable = "gnome-shell",
      .num_options = 2,
      .options = application_338_options,
    },
    { .name = "Desktop — Plasma",
      .executable = "plasmashell",
      .num_options = 1,
      .options = application_341_options,
    },
    { .name = "budgie-wm",
      .executable = "budgie-wm",
      .num_options = 1,
      .options = application_343_options,
    },
    { .name = "kwin_x11",
      .executable = "kwin_x11",
      .num_options = 1,
      .options = application_345_options,
    },
    { .name = "ksmserver-logout-greeter",
      .executable = "ksmserver-logout-greeter",
      .num_options = 1,
      .options = application_347_options,
    },
    { .name = "ksmserver-switchuser-greeter",
      .executable = "ksmserver-switchuser-greeter",
      .num_options = 1,
      .options = application_349_options,
    },
    { .name = "kscreenlocker_greet",
      .executable = "kscreenlocker_greet",
      .num_options = 1,
      .options = application_351_options,
    },
    { .name = "startplasma",
      .executable = "startplasma",
      .num_options = 1,
      .options = application_353_options,
    },
    { .name = "sddm-greeter",
      .executable = "sddm-greeter",
      .num_options = 1,
      .options = application_355_options,
    },
    { .name = "krunner",
      .executable = "krunner",
      .num_options = 1,
      .options = application_357_options,
    },
    { .name = "spectacle",
      .executable = "spectacle",
      .num_options = 1,
      .options = application_359_options,
    },
    { .name = "marco",
      .executable = "marco",
      .num_options = 1,
      .options = application_361_options,
    },
    { .name = "compton",
      .executable = "compton",
      .num_options = 1,
      .options = application_363_options,
    },
    { .name = "picom",
      .executable = "picom",
      .num_options = 1,
      .options = application_365_options,
    },
    { .name = "xfwm4",
      .executable = "xfwm4",
      .num_options = 1,
      .options = application_367_options,
    },
    { .name = "Enlightenment",
      .executable = "enlightenment",
      .num_options = 1,
      .options = application_369_options,
    },
    { .name = "mutter",
      .executable = "mutter",
      .num_options = 2,
      .options = application_371_options,
    },
    { .name = "muffin",
      .executable = "muffin",
      .num_options = 1,
      .options = application_374_options,
    },
    { .name = "cinnamon",
      .executable = "cinnamon",
      .num_options = 1,
      .options = application_376_options,
    },
    { .name = "compiz",
      .executable = "compiz",
      .num_options = 1,
      .options = application_378_options,
    },
    { .name = "Firefox",
      .executable = "firefox",
      .num_options = 1,
      .options = application_380_options,
    },
    { .name = "Firefox ESR",
      .executable = "firefox-esr",
      .num_options = 1,
      .options = application_382_options,
    },
    { .name = "Chromium",
      .executable = "chromium",
      .num_options = 1,
      .options = application_384_options,
    },
    { .name = "Google Chrome",
      .executable = "chrome",
      .num_options = 1,
      .options = application_386_options,
    },
    { .name = "Iceweasel",
      .executable = "iceweasel",
      .num_options = 1,
      .options = application_388_options,
    },
    { .name = "Epiphany",
      .executable = "epiphany",
      .num_options = 1,
      .options = application_390_options,
    },
    { .name = "Konqueror",
      .executable = "konqueror",
      .num_options = 1,
      .options = application_392_options,
    },
    { .name = "Falkon",
      .executable = "falkon",
      .num_options = 1,
      .options = application_394_options,
    },
    { .name = "Seamonkey",
      .executable = "seamonkey",
      .num_options = 1,
      .options = application_396_options,
    },
    { .name = "Waterfox",
      .executable = "waterfox",
      .num_options = 1,
      .options = application_398_options,
    },
    { .name = "VLC Media Player",
      .executable = "vlc",
      .num_options = 1,
      .options = application_400_options,
    },
    { .name = "Totem",
      .executable = "totem",
      .num_options = 1,
      .options = application_402_options,
    },
    { .name = "Dragon Player",
      .executable = "dragon",
      .num_options = 1,
      .options = application_404_options,
    },
    { .name = "Xorg",
      .executable = "Xorg",
      .num_options = 1,
      .options = application_406_options,
    },
    { .name = "gfxbench",
      .executable = "testfw_app",
      .num_options = 3,
      .options = application_408_options,
    },
    { .name = "Rainbow Six Extraction (Wine)",
      .executable = "R6-Extraction.exe",
      .num_options = 2,
      .options = application_412_options,
    },
    { .name = "Brink",
      .executable = "brink.exe",
      .num_options = 2,
      .options = application_415_options,
    },
    { .name = "Enter The Gungeon (32 bits)",
      .executable = "EtG.x86",
      .num_options = 1,
      .options = application_418_options,
    },
    { .name = "Enter The Gungeon (64 bits)",
      .executable = "EtG.x86_64",
      .num_options = 1,
      .options = application_420_options,
    },
    { .name = "SWKOTOR (wine)",
      .executable = "swkotor.exe",
      .num_options = 1,
      .options = application_422_options,
    },
    { .name = "Assault Android Cactus (32-bit)",
      .executable = "cactus_demo.x86",
      .num_options = 1,
      .options = application_424_options,
    },
    { .name = "Assault Android Cactus (64-bit)",
      .executable = "cactus_demo.x86_64",
      .num_options = 1,
      .options = application_426_options,
    },
    { .name = "Assault Android Cactus (32-bit)",
      .executable = "cactus.x86",
      .num_options = 1,
      .options = application_428_options,
    },
    { .name = "Assault Android Cactus (64-bit)",
      .executable = "cactus.x86_64",
      .num_options = 1,
      .options = application_430_options,
    },
    { .name = "Limbo",
      .executable = "limbo",
      .num_options = 1,
      .options = application_432_options,
    },
    { .name = "Genshin Impact",
      .executable = "com.miHoYo.GenshinImpact",
      .num_options = 1,
      .options = application_434_options,
    },
    { .name = "DOOM",
      .executable = "DOOMx64vk.exe",
      .num_options = 1,
      .options = application_436_options,
    },
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 1,
      .options = application_438_options,
    },
    { .name = "Wolfenstein: Youngblood(x64vk)",
      .executable = "Youngblood_x64vk.exe",
      .num_options = 1,
      .options = application_440_options,
    },
    { .name = "Wolfenstein II: The New Colossus",
      .executable = "NewColossus_x64vk.exe",
      .num_options = 1,
      .options = application_442_options,
    },
    { .name = "Metro: Exodus",
      .application_name_match = "metroexodus",
      .num_options = 1,
      .options = application_444_options,
    },
    { .name = "Forsaken Remastered",
      .executable = "ForsakenEx",
      .num_options = 1,
      .options = application_446_options,
    },
};

static const struct driconf_device device_1 = {
    .num_engines = 1,
    .engines = device_1_engines,
    .num_applications = 203,
    .applications = device_1_applications,
};


    
static const struct driconf_option application_451_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_EXT_buffer_age -GLX_OML_sync_control -GLX_SGI_video_sync" },
};

    
static const struct driconf_option application_453_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_EXT_buffer_age -GLX_OML_sync_control -GLX_SGI_video_sync" },
};

    
static const struct driconf_option application_455_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_EXT_buffer_age -GLX_OML_sync_control" },
};


static const struct driconf_application device_450_applications[] = {
    { .name = "gnome-shell",
      .executable = "gnome-shell",
      .num_options = 1,
      .options = application_451_options,
    },
    { .name = "cinnamon",
      .executable = "cinnamon",
      .num_options = 1,
      .options = application_453_options,
    },
    { .name = "Compiz",
      .executable = "Compiz",
      .num_options = 1,
      .options = application_455_options,
    },
};

static const struct driconf_device device_450 = {
    .driver = "vmwgfx",
    .num_engines = 0,
    .num_applications = 3,
    .applications = device_450_applications,
};


    
static const struct driconf_option application_458_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_460_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_462_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_465_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_467_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_469_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_471_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_473_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_476_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_478_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_480_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_482_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_484_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_486_options[] = {
    { .name = "radeonsi_no_infinite_interp", .value = "true" },
};

    
static const struct driconf_option application_488_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_490_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_492_options[] = {
    { .name = "glthread_nop_check_framebuffer_status", .value = "true" },
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_495_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_497_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_499_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
    { .name = "radeonsi_no_infinite_interp", .value = "true" },
};

    
static const struct driconf_option application_502_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_504_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_506_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_508_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
    { .name = "radeonsi_force_use_fma32", .value = "true" },
};

    
static const struct driconf_option application_511_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};


static const struct driconf_application device_457_applications[] = {
    { .name = "Alien Isolation",
      .executable = "AlienIsolation",
      .num_options = 1,
      .options = application_458_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_460_options,
    },
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 2,
      .options = application_462_options,
    },
    { .name = "Exanima",
      .executable = "Exanima.exe",
      .num_options = 1,
      .options = application_465_options,
    },
    { .name = "Nowhere Patrol",
      .executable = "NowherePatrol.exe",
      .num_options = 1,
      .options = application_467_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_469_options,
    },
    { .name = "Gfx Bench",
      .executable = "gfxbench_gl",
      .num_options = 1,
      .options = application_471_options,
    },
    { .name = "Monolith demo5",
      .executable = "runner",
      .sha1 = "0c40d509a74e357f0280cb1bd882e9cd94b91bdf",
      .num_options = 2,
      .options = application_473_options,
    },
    { .name = "Memoranda / Riptale",
      .executable = "runner",
      .sha1 = "aa13dec6af63c88f308ebb487693896434a4db56",
      .num_options = 1,
      .options = application_476_options,
    },
    { .name = "Nuclear Throne",
      .executable = "runner",
      .sha1 = "84814e8db125e889f5d9d4195a0ca72a871ea1fd",
      .num_options = 1,
      .options = application_478_options,
    },
    { .name = "Undertale",
      .executable = "runner",
      .sha1 = "dfa302e7ec78641d0696dbbc1a06fc29f34ff1ff",
      .num_options = 1,
      .options = application_480_options,
    },
    { .name = "Turmoil",
      .executable = "runner",
      .sha1 = "cbbf757aaab289859f8dae191a7d63afc30643d9",
      .num_options = 1,
      .options = application_482_options,
    },
    { .name = "Peace, Death!",
      .executable = "runner",
      .sha1 = "5b909f3d21799773370adf084f649848f098234e",
      .num_options = 1,
      .options = application_484_options,
    },
    { .name = "Kerbal Space Program",
      .executable = "KSP.x86_64",
      .num_options = 1,
      .options = application_486_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_488_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_490_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 2,
      .options = application_492_options,
    },
    { .name = "Road Redemption",
      .executable = "RoadRedemption.x86_64",
      .num_options = 1,
      .options = application_495_options,
    },
    { .name = "Wasteland 2",
      .executable = "WL2",
      .num_options = 1,
      .options = application_497_options,
    },
    { .name = "Teardown",
      .executable = "teardown.exe",
      .num_options = 2,
      .options = application_499_options,
    },
    { .name = "SpaceEngine",
      .executable = "SpaceEngine.exe",
      .num_options = 1,
      .options = application_502_options,
    },
    { .name = "BETA CAE Systems - GL detect tool",
      .executable = "detect_opengl_tool",
      .num_options = 1,
      .options = application_504_options,
    },
    { .name = "BETA CAE Systems - ANSA",
      .executable = "ansa_linux_x86_64",
      .num_options = 1,
      .options = application_506_options,
    },
    { .name = "BETA CAE Systems - META",
      .executable = "meta_post_x86_64",
      .num_options = 2,
      .options = application_508_options,
    },
    { .name = "Black Geyser: Couriers of Darkness",
      .executable = "BlackGeyser.x86_64",
      .num_options = 1,
      .options = application_511_options,
    },
};

static const struct driconf_device device_457 = {
    .driver = "radeonsi",
    .num_engines = 0,
    .num_applications = 25,
    .applications = device_457_applications,
};


    
static const struct driconf_option application_514_options[] = {
    { .name = "mesa_glthread", .value = "false" },
};

    
static const struct driconf_option application_516_options[] = {
    { .name = "zink_emulate_point_smooth", .value = "true" },
};

    
static const struct driconf_option application_518_options[] = {
    { .name = "zink_emulate_point_smooth", .value = "true" },
};

    
static const struct driconf_option application_520_options[] = {
    { .name = "zink_emulate_point_smooth", .value = "true" },
};


static const struct driconf_application device_513_applications[] = {
    { .name = "Hyperdimension Neptunia Re;Birth1",
      .executable = "NeptuniaReBirth1.exe",
      .num_options = 1,
      .options = application_514_options,
    },
    { .name = "Quake II",
      .executable = "quake2-engine",
      .num_options = 1,
      .options = application_516_options,
    },
    { .name = "Quake II (yamagi)",
      .executable = "yamagi-quake2",
      .num_options = 1,
      .options = application_518_options,
    },
    { .name = "Quake II (wine)",
      .executable = "quake2.exe",
      .num_options = 1,
      .options = application_520_options,
    },
};

static const struct driconf_device device_513 = {
    .driver = "zink",
    .num_engines = 0,
    .num_applications = 4,
    .applications = device_513_applications,
};


    
static const struct driconf_option application_523_options[] = {
    { .name = "vs_position_always_invariant", .value = "true" },
};

    
static const struct driconf_option application_525_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_527_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_529_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};


static const struct driconf_application device_522_applications[] = {
    { .name = "Middle Earth: Shadow of Mordor",
      .executable = "ShadowOfMordor",
      .num_options = 1,
      .options = application_523_options,
    },
    { .name = "glmark2",
      .executable = "glmark2",
      .num_options = 1,
      .options = application_525_options,
    },
    { .name = "Counter-Strike: Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_527_options,
    },
    { .name = "Insurgency",
      .executable = "insurgency_linux",
      .num_options = 1,
      .options = application_529_options,
    },
};

static const struct driconf_device device_522 = {
    .driver = "iris",
    .num_engines = 0,
    .num_applications = 4,
    .applications = device_522_applications,
};


    
static const struct driconf_option application_532_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_534_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_536_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};


static const struct driconf_application device_531_applications[] = {
    { .name = "glmark2",
      .executable = "glmark2",
      .num_options = 1,
      .options = application_532_options,
    },
    { .name = "Counter-Strike: Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_534_options,
    },
    { .name = "Insurgency",
      .executable = "insurgency_linux",
      .num_options = 1,
      .options = application_536_options,
    },
};

static const struct driconf_device device_531 = {
    .driver = "crocus",
    .num_engines = 0,
    .num_applications = 3,
    .applications = device_531_applications,
};
    
static const struct driconf_option engine_558_options[] = {
    { .name = "no_16bit", .value = "true" },
};


static const struct driconf_engine device_538_engines[] = {
    { .engine_name_match = "mesa zink",
      .num_options = 1,
      .options = engine_558_options,
    },
};

    
static const struct driconf_option application_539_options[] = {
    { .name = "anv_assume_full_subgroups", .value = "true" },
};

    
static const struct driconf_option application_541_options[] = {
    { .name = "anv_assume_full_subgroups", .value = "true" },
    { .name = "fp64_workaround_enabled", .value = "true" },
};

    
static const struct driconf_option application_544_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_546_options[] = {
    { .name = "anv_sample_mask_out_opengl_behaviour", .value = "true" },
};

    
static const struct driconf_option application_548_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_550_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_552_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_554_options[] = {
    { .name = "limit_trig_input_range", .value = "true" },
};

    
static const struct driconf_option application_556_options[] = {
    { .name = "force_vk_vendor", .value = "-1" },
};


static const struct driconf_application device_538_applications[] = {
    { .name = "Aperture Desk Job",
      .executable = "deskjob",
      .num_options = 1,
      .options = application_539_options,
    },
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 2,
      .options = application_541_options,
    },
    { .name = "Wolfenstein: Youngblood(x64vk)",
      .executable = "Youngblood_x64vk.exe",
      .num_options = 1,
      .options = application_544_options,
    },
    { .name = "Batman™: Arkham Knight",
      .executable = "BatmanAK.exe",
      .num_options = 1,
      .options = application_546_options,
    },
    { .name = "Rise of the Tomb Raider",
      .executable = "RiseOfTheTombRaider",
      .num_options = 1,
      .options = application_548_options,
    },
    { .name = "Rise of the Tomb Raider",
      .executable = "ROTTR.exe",
      .num_options = 1,
      .options = application_550_options,
    },
    { .name = "NieR Replicant ver.1.22474487139",
      .executable = "NieR Replicant ver.1.22474487139.exe",
      .num_options = 1,
      .options = application_552_options,
    },
    { .name = "NieR:Automata",
      .executable = "NieRAutomata.exe",
      .num_options = 1,
      .options = application_554_options,
    },
    { .name = "DEATH STRANDING",
      .executable = "ds.exe",
      .num_options = 1,
      .options = application_556_options,
    },
};

static const struct driconf_device device_538 = {
    .driver = "anv",
    .num_engines = 1,
    .engines = device_538_engines,
    .num_applications = 9,
    .applications = device_538_applications,
};


    
static const struct driconf_option application_561_options[] = {
    { .name = "dzn_enable_8bit_loads_stores", .value = "true" },
    { .name = "dzn_enable_subgroup_ops_in_vtx_pipeline", .value = "true" },
    { .name = "dzn_claim_wide_lines", .value = "true" },
};

    
static const struct driconf_option application_565_options[] = {
    { .name = "dzn_claim_wide_lines", .value = "true" },
};


static const struct driconf_application device_560_applications[] = {
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 3,
      .options = application_561_options,
    },
    { .name = "No Man's Sky",
      .executable = "NMS.exe",
      .num_options = 1,
      .options = application_565_options,
    },
};

static const struct driconf_device device_560 = {
    .driver = "dzn",
    .num_engines = 0,
    .num_applications = 2,
    .applications = device_560_applications,
};


    
static const struct driconf_option application_568_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_ARB_shader_image_load_store" },
};


static const struct driconf_application device_567_applications[] = {
    { .name = "Tomb Raider 2013",
      .executable = "TombRaider",
      .num_options = 1,
      .options = application_568_options,
    },
};

static const struct driconf_device device_567 = {
    .driver = "r600",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_567_applications,
};


    
static const struct driconf_option application_571_options[] = {
    { .name = "format_l8_srgb_enable_readback", .value = "true" },
};


static const struct driconf_application device_570_applications[] = {
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_571_options,
    },
};

static const struct driconf_device device_570 = {
    .driver = "virtio_gpu",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_570_applications,
};


    
static const struct driconf_option application_574_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 618" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_577_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 630" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_580_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 618" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_583_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 630" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};


static const struct driconf_application device_573_applications[] = {
    { .name = "Call of Duty Mobile",
      .executable = "com.activision.callofduty.shooter",
      .num_options = 2,
      .options = application_574_options,
    },
    { .name = "Asphalt 8",
      .executable = "com.gameloft.android.ANMP.GloftA8HM",
      .num_options = 2,
      .options = application_577_options,
    },
    { .name = "Asphalt 9",
      .executable = "com.gameloft.android.ANMP.GloftA9HM",
      .num_options = 2,
      .options = application_580_options,
    },
    { .name = "PUBG Mobile",
      .executable = "com.tencent.ig",
      .num_options = 2,
      .options = application_583_options,
    },
};

static const struct driconf_device device_573 = {
    .driver = "msm",
    .device = "FD618",
    .num_engines = 0,
    .num_applications = 4,
    .applications = device_573_applications,
};
    
static const struct driconf_option engine_656_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_disable_aniso_single_level", .value = "true" },
    { .name = "radv_dgc", .value = "true" },
};

    
static const struct driconf_option engine_660_options[] = {
    { .name = "radv_disable_aniso_single_level", .value = "true" },
};

    
static const struct driconf_option engine_662_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_lower_discard_to_demote", .value = "true" },
    { .name = "radv_disable_tc_compat_htile_general", .value = "true" },
};

    
static const struct driconf_option engine_666_options[] = {
    { .name = "radv_flush_before_query_copy", .value = "true" },
    { .name = "radv_flush_before_timestamp_write", .value = "true" },
};

    
static const struct driconf_option engine_669_options[] = {
    { .name = "radv_tex_non_uniform", .value = "true" },
};


static const struct driconf_engine device_586_engines[] = {
    { .engine_name_match = "vkd3d",
      .num_options = 3,
      .options = engine_656_options,
    },
    { .engine_name_match = "DXVK",
      .num_options = 1,
      .options = engine_660_options,
    },
    { .engine_name_match = "Quantic Dream Engine",
      .num_options = 3,
      .options = engine_662_options,
    },
    { .engine_name_match = "^UnrealEngine",
      .num_options = 2,
      .options = engine_666_options,
    },
    { .engine_name_match = "DXVK_NvRemix",
      .num_options = 1,
      .options = engine_669_options,
    },
};

    
static const struct driconf_option application_587_options[] = {
    { .name = "radv_report_llvm9_version_string", .value = "true" },
    { .name = "radv_invariant_geom", .value = "true" },
};

    
static const struct driconf_option application_590_options[] = {
    { .name = "radv_invariant_geom", .value = "true" },
    { .name = "radv_split_fma", .value = "true" },
};

    
static const struct driconf_option application_593_options[] = {
    { .name = "radv_enable_mrt_output_nan_fixup", .value = "true" },
    { .name = "radv_app_layer", .value = "rage2" },
};

    
static const struct driconf_option application_596_options[] = {
    { .name = "radv_no_dynamic_bounds", .value = "true" },
    { .name = "radv_absolute_depth_bias", .value = "true" },
};

    
static const struct driconf_option application_599_options[] = {
    { .name = "radv_no_dynamic_bounds", .value = "true" },
    { .name = "radv_absolute_depth_bias", .value = "true" },
};

    
static const struct driconf_option application_602_options[] = {
    { .name = "radv_no_dynamic_bounds", .value = "true" },
    { .name = "radv_absolute_depth_bias", .value = "true" },
};

    
static const struct driconf_option application_605_options[] = {
    { .name = "radv_no_dynamic_bounds", .value = "true" },
    { .name = "radv_absolute_depth_bias", .value = "true" },
};

    
static const struct driconf_option application_608_options[] = {
    { .name = "radv_disable_shrink_image_store", .value = "true" },
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_611_options[] = {
    { .name = "radv_override_uniform_offset_alignment", .value = "16" },
    { .name = "radv_disable_shrink_image_store", .value = "true" },
    { .name = "radv_invariant_geom", .value = "true" },
};

    
static const struct driconf_option application_615_options[] = {
    { .name = "radv_no_dynamic_bounds", .value = "true" },
};

    
static const struct driconf_option application_617_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
    { .name = "radv_rt", .value = "true" },
};

    
static const struct driconf_option application_620_options[] = {
    { .name = "radv_lower_discard_to_demote", .value = "true" },
};

    
static const struct driconf_option application_622_options[] = {
    { .name = "radv_disable_dcc", .value = "true" },
};

    
static const struct driconf_option application_624_options[] = {
    { .name = "radv_disable_dcc", .value = "true" },
};

    
static const struct driconf_option application_626_options[] = {
    { .name = "radv_disable_sinking_load_input_fs", .value = "true" },
};

    
static const struct driconf_option application_628_options[] = {
    { .name = "radv_disable_sinking_load_input_fs", .value = "true" },
};

    
static const struct driconf_option application_630_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_632_options[] = {
    { .name = "radv_enable_unified_heap_on_apu", .value = "true" },
};

    
static const struct driconf_option application_634_options[] = {
    { .name = "radv_rt", .value = "true" },
};

    
static const struct driconf_option application_636_options[] = {
    { .name = "radv_rt", .value = "true" },
};

    
static const struct driconf_option application_638_options[] = {
    { .name = "radv_app_layer", .value = "metroexodus" },
};

    
static const struct driconf_option application_640_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_642_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_644_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_646_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_648_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_650_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_652_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};

    
static const struct driconf_option application_654_options[] = {
    { .name = "radv_zero_vram", .value = "true" },
};


static const struct driconf_application device_586_applications[] = {
    { .name = "Shadow Of The Tomb Raider (Native)",
      .application_name_match = "ShadowOfTheTomb",
      .num_options = 2,
      .options = application_587_options,
    },
    { .name = "Shadow Of The Tomb Raider (DX11/DX12)",
      .application_name_match = "SOTTR.exe",
      .num_options = 2,
      .options = application_590_options,
    },
    { .name = "RAGE 2",
      .executable = "RAGE2.exe",
      .num_options = 2,
      .options = application_593_options,
    },
    { .name = "Path of Exile (64-bit, Steam)",
      .executable = "PathOfExile_x64Steam.exe",
      .num_options = 2,
      .options = application_596_options,
    },
    { .name = "Path of Exile (32-bit, Steam)",
      .executable = "PathOfExileSteam.exe",
      .num_options = 2,
      .options = application_599_options,
    },
    { .name = "Path of Exile (64-bit)",
      .executable = "PathOfExile_x64.exe",
      .num_options = 2,
      .options = application_602_options,
    },
    { .name = "Path of Exile (32-bit)",
      .executable = "PathOfExile.exe",
      .num_options = 2,
      .options = application_605_options,
    },
    { .name = "The Surge 2",
      .application_name_match = "Fledge",
      .num_options = 2,
      .options = application_608_options,
    },
    { .name = "World War Z (and World War Z: Aftermath)",
      .application_name_match = "WWZ|wwz",
      .num_options = 3,
      .options = application_611_options,
    },
    { .name = "DOOM VFR",
      .application_name_match = "DOOM_VFR",
      .num_options = 1,
      .options = application_615_options,
    },
    { .name = "DOOM Eternal",
      .application_name_match = "DOOMEternal",
      .num_options = 2,
      .options = application_617_options,
    },
    { .name = "No Man's Sky",
      .application_name_match = "No Man's Sky",
      .num_options = 1,
      .options = application_620_options,
    },
    { .name = "DOOM (2016)",
      .application_name_match = "DOOM$",
      .num_options = 1,
      .options = application_622_options,
    },
    { .name = "Wolfenstein II",
      .application_name_match = "Wolfenstein II The New Colossus",
      .num_options = 1,
      .options = application_624_options,
    },
    { .name = "Grid Autosport",
      .application_name_match = "GRIDAutosport_avx.exe",
      .num_options = 1,
      .options = application_626_options,
    },
    { .name = "Grid Autosport",
      .application_name_match = "GRIDAutosport.exe",
      .num_options = 1,
      .options = application_628_options,
    },
    { .name = "Hammerting",
      .application_name_match = "TDTD",
      .num_options = 1,
      .options = application_630_options,
    },
    { .name = "RDR2",
      .application_name_match = "Red Dead Redemption 2",
      .num_options = 1,
      .options = application_632_options,
    },
    { .name = "Quake II RTX",
      .application_name_match = "quake 2 pathtracing",
      .num_options = 1,
      .options = application_634_options,
    },
    { .name = "Control (DX12)",
      .application_name_match = "Control_DX12.exe",
      .num_options = 1,
      .options = application_636_options,
    },
    { .name = "Metro Exodus (Linux native)",
      .application_name_match = "metroexodus",
      .num_options = 1,
      .options = application_638_options,
    },
    { .name = "Black Geyser: Couriers of Darkness",
      .executable = "BlackGeyser.x86_64",
      .num_options = 1,
      .options = application_640_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_642_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_644_options,
    },
    { .name = "Monolith demo5",
      .executable = "runner",
      .sha1 = "0c40d509a74e357f0280cb1bd882e9cd94b91bdf",
      .num_options = 1,
      .options = application_646_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_648_options,
    },
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_650_options,
    },
    { .name = "Exanima",
      .executable = "Exanima.exe",
      .num_options = 1,
      .options = application_652_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_654_options,
    },
};

static const struct driconf_device device_586 = {
    .driver = "radv",
    .num_engines = 5,
    .engines = device_586_engines,
    .num_applications = 29,
    .applications = device_586_applications,
};

static const struct driconf_device *driconf[] = {
    &device_1,
    &device_450,
    &device_457,
    &device_513,
    &device_522,
    &device_531,
    &device_538,
    &device_560,
    &device_567,
    &device_570,
    &device_573,
    &device_586,
};
