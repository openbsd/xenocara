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



    
static const struct driconf_option engine_393_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};


static const struct driconf_engine device_1_engines[] = {
    { .engine_name_match = "UnrealEngine4.*",
      .engine_versions = "0:23",
      .num_options = 1,
      .options = engine_393_options,
    },
};

    
static const struct driconf_option application_2_options[] = {
    { .name = "force_glsl_extensions_warn", .value = "true" },
    { .name = "disable_blend_func_extended", .value = "true" },
    { .name = "disable_arb_gpu_shader5", .value = "true" },
};

    
static const struct driconf_option application_6_options[] = {
    { .name = "force_glsl_extensions_warn", .value = "true" },
    { .name = "disable_blend_func_extended", .value = "true" },
    { .name = "disable_arb_gpu_shader5", .value = "true" },
};

    
static const struct driconf_option application_10_options[] = {
    { .name = "disable_xcb_surface", .value = "true" },
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
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_45_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_47_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
    { .name = "force_glsl_extensions_warn", .value = "true" },
};

    
static const struct driconf_option application_50_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_52_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_54_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
    { .name = "force_gl_map_buffer_synchronized", .value = "true" },
};

    
static const struct driconf_option application_57_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
    { .name = "force_gl_map_buffer_synchronized", .value = "true" },
};

    
static const struct driconf_option application_60_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_62_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_64_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_66_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_68_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
    { .name = "force_gl_vendor", .value = "ATI Technologies, Inc." },
};

    
static const struct driconf_option application_72_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_74_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_76_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_78_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_80_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_82_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_84_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_86_options[] = {
    { .name = "force_compat_shaders", .value = "true" },
};

    
static const struct driconf_option application_88_options[] = {
    { .name = "force_glsl_version", .value = "440" },
};

    
static const struct driconf_option application_90_options[] = {
    { .name = "force_glsl_abs_sqrt", .value = "true" },
};

    
static const struct driconf_option application_92_options[] = {
    { .name = "force_glsl_abs_sqrt", .value = "true" },
};

    
static const struct driconf_option application_94_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_96_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_98_options[] = {
    { .name = "force_glsl_version", .value = "130" },
    { .name = "glsl_ignore_write_to_readonly_var", .value = "true" },
};

    
static const struct driconf_option application_101_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_103_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_105_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_107_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_109_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_111_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_113_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_115_options[] = {
    { .name = "allow_glsl_builtin_const_expression", .value = "true" },
    { .name = "allow_glsl_relaxed_es", .value = "true" },
};

    
static const struct driconf_option application_118_options[] = {
    { .name = "allow_extra_pp_tokens", .value = "true" },
};

    
static const struct driconf_option application_120_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_122_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_124_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_126_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_128_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_130_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
};

    
static const struct driconf_option application_132_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
};

    
static const struct driconf_option application_134_options[] = {
    { .name = "vs_position_always_invariant", .value = "true" },
};

    
static const struct driconf_option application_136_options[] = {
    { .name = "vblank_mode", .value = "0" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
    { .name = "force_gl_names_reuse", .value = "true" },
    { .name = "force_gl_vendor", .value = "NVIDIA Corporation" },
    { .name = "force_glsl_extensions_warn", .value = "true" },
    { .name = "allow_draw_out_of_order", .value = "true" },
    { .name = "mesa_glthread", .value = "true" },
    { .name = "mesa_no_error", .value = "true" },
    { .name = "allow_rgb10_configs", .value = "false" },
    { .name = "allow_invalid_glx_destroy_window", .value = "true" },
};

    
static const struct driconf_option application_148_options[] = {
    { .name = "ignore_map_unsynchronized", .value = "true" },
};

    
static const struct driconf_option application_150_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_153_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_155_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
};

    
static const struct driconf_option application_157_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
    { .name = "do_dce_before_clip_cull_analysis", .value = "true" },
};

    
static const struct driconf_option application_160_options[] = {
    { .name = "mesa_extension_override", .value = "-GL_MESA_pack_invert -GL_MESA_framebuffer_flip_y -GL_MESA_window_pos" },
};

    
static const struct driconf_option application_162_options[] = {
    { .name = "force_glsl_extensions_warn", .value = "true" },
};

    
static const struct driconf_option application_164_options[] = {
    { .name = "force_direct_glx_context", .value = "true" },
};

    
static const struct driconf_option application_166_options[] = {
    { .name = "keep_native_window_glx_drawable", .value = "true" },
    { .name = "allow_rgb10_configs", .value = "false" },
};

    
static const struct driconf_option application_169_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_171_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_173_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_175_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_177_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_179_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_181_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_183_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_185_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_187_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_189_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_191_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_193_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_195_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_197_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_199_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_201_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_203_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_205_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_207_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_209_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_211_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_213_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_215_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_217_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_219_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_221_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_223_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_225_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_227_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_229_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_231_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_233_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_235_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_237_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_239_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_241_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_243_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_245_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_247_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_249_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_251_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_253_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_255_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_257_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_259_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_261_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_263_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_265_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_267_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_269_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_271_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_273_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_275_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_277_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_279_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_281_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_283_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_285_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_287_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_289_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_291_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_293_options[] = {
    { .name = "adaptive_sync", .value = "false" },
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_296_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_298_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_300_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_302_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_304_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_306_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_308_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_310_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_312_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_314_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_316_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_318_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_320_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_322_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_324_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_326_options[] = {
    { .name = "adaptive_sync", .value = "false" },
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_329_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_331_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_333_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_335_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_337_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_339_options[] = {
    { .name = "adaptive_sync", .value = "false" },
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
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_363_options[] = {
    { .name = "vk_x11_override_min_image_count", .value = "2" },
    { .name = "vk_wsi_force_bgra8_unorm_first", .value = "true" },
};

    
static const struct driconf_option application_366_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_OML_swap_method" },
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_369_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_371_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_373_options[] = {
    { .name = "force_gl_names_reuse", .value = "true" },
};

    
static const struct driconf_option application_375_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_377_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_379_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_381_options[] = {
    { .name = "vs_position_always_precise", .value = "true" },
};

    
static const struct driconf_option application_383_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_385_options[] = {
    { .name = "vk_x11_ensure_min_image_count", .value = "true" },
};

    
static const struct driconf_option application_387_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_389_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_391_options[] = {
    { .name = "vk_dont_care_as_load", .value = "true" },
};


static const struct driconf_application device_1_applications[] = {
    { .name = "Unigine Sanctuary",
      .executable = "Sanctuary",
      .num_options = 3,
      .options = application_2_options,
    },
    { .name = "Unigine Tropics",
      .executable = "Tropics",
      .num_options = 3,
      .options = application_6_options,
    },
    { .name = "Xwayland",
      .executable = "Xwayland",
      .num_options = 1,
      .options = application_10_options,
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
    { .name = "Dead Island (incl. Definitive Edition)",
      .executable = "DeadIslandGame",
      .num_options = 2,
      .options = application_42_options,
    },
    { .name = "Dead Island Riptide Definitive Edition",
      .executable = "DeadIslandRiptideGame",
      .num_options = 1,
      .options = application_45_options,
    },
    { .name = "Doom 3: BFG",
      .executable = "Doom3BFG.exe",
      .num_options = 2,
      .options = application_47_options,
    },
    { .name = "Dying Light",
      .executable = "DyingLightGame",
      .num_options = 1,
      .options = application_50_options,
    },
    { .name = "Full Bore",
      .executable = "fullbore",
      .num_options = 1,
      .options = application_52_options,
    },
    { .name = "RAGE (64-bit)",
      .executable = "Rage64.exe",
      .num_options = 2,
      .options = application_54_options,
    },
    { .name = "RAGE (32-bit)",
      .executable = "Rage.exe",
      .num_options = 2,
      .options = application_57_options,
    },
    { .name = "Second Life",
      .executable = "do-not-directly-run-secondlife-bin",
      .num_options = 1,
      .options = application_60_options,
    },
    { .name = "Warsow (32-bit)",
      .executable = "warsow.i386",
      .num_options = 1,
      .options = application_62_options,
    },
    { .name = "Warsow (64-bit)",
      .executable = "warsow.x86_64",
      .num_options = 1,
      .options = application_64_options,
    },
    { .name = "Rust",
      .executable = "rust",
      .num_options = 1,
      .options = application_66_options,
    },
    { .name = "Divinity: Original Sin Enhanced Edition",
      .executable = "EoCApp",
      .num_options = 3,
      .options = application_68_options,
    },
    { .name = "Metro 2033 Redux / Metro Last Night Redux",
      .executable = "metro",
      .num_options = 1,
      .options = application_72_options,
    },
    { .name = "Worms W.M.D",
      .executable = "Worms W.M.Dx64",
      .num_options = 1,
      .options = application_74_options,
    },
    { .name = "Crookz - The Big Heist",
      .executable = "Crookz",
      .num_options = 1,
      .options = application_76_options,
    },
    { .name = "Tropico 5",
      .executable = "Tropico5",
      .num_options = 1,
      .options = application_78_options,
    },
    { .name = "Faster than Light (32-bit)",
      .executable = "FTL.x86",
      .num_options = 1,
      .options = application_80_options,
    },
    { .name = "Faster than Light (64-bit)",
      .executable = "FTL.amd64",
      .num_options = 1,
      .options = application_82_options,
    },
    { .name = "Final Fantasy VIII: Remastered",
      .executable = "FFVIII.exe",
      .num_options = 1,
      .options = application_84_options,
    },
    { .name = "SNK HEROINES Tag Team Frenzy",
      .executable = "SNKHEROINES.exe",
      .num_options = 1,
      .options = application_86_options,
    },
    { .name = "The Culling",
      .executable = "Victory",
      .num_options = 1,
      .options = application_88_options,
    },
    { .name = "Spec Ops: The Line (32-bit)",
      .executable = "specops.i386",
      .num_options = 1,
      .options = application_90_options,
    },
    { .name = "Spec Ops: The Line (64-bit)",
      .executable = "specops",
      .num_options = 1,
      .options = application_92_options,
    },
    { .name = "Kerbal Space Program (32-bit)",
      .executable = "KSP.x86",
      .num_options = 1,
      .options = application_94_options,
    },
    { .name = "Kerbal Space Program (64-bit)",
      .executable = "KSP.x86_64",
      .num_options = 1,
      .options = application_96_options,
    },
    { .name = "Luna Sky",
      .executable = "lunasky",
      .num_options = 2,
      .options = application_98_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_101_options,
    },
    { .name = "The Witcher 2",
      .executable = "witcher2",
      .num_options = 1,
      .options = application_103_options,
    },
    { .name = "Unreal 4 Editor",
      .executable = "UE4Editor",
      .num_options = 1,
      .options = application_105_options,
    },
    { .name = "Observer",
      .executable = "TheObserver-Linux-Shipping",
      .num_options = 1,
      .options = application_107_options,
    },
    { .name = "Steamroll",
      .executable = "Steamroll-Linux-Shipping",
      .num_options = 1,
      .options = application_109_options,
    },
    { .name = "Refunct",
      .executable = "Refunct-Linux-Shipping",
      .num_options = 1,
      .options = application_111_options,
    },
    { .name = "We Happy Few",
      .executable = "GlimpseGame",
      .num_options = 1,
      .options = application_113_options,
    },
    { .name = "Google Earth VR",
      .executable = "Earth.exe",
      .num_options = 2,
      .options = application_115_options,
    },
    { .name = "Champions of Regnum",
      .executable = "game",
      .num_options = 1,
      .options = application_118_options,
    },
    { .name = "Wolfenstein The Old Blood",
      .executable = "WolfOldBlood_x64.exe",
      .num_options = 1,
      .options = application_120_options,
    },
    { .name = "ARMA 3",
      .executable = "arma3.x86_64",
      .num_options = 1,
      .options = application_122_options,
    },
    { .name = "Epic Games Launcher",
      .executable = "EpicGamesLauncher.exe",
      .num_options = 1,
      .options = application_124_options,
    },
    { .name = "GpuTest",
      .executable = "GpuTest",
      .num_options = 1,
      .options = application_126_options,
    },
    { .name = "Curse of the Dead Gods",
      .executable = "Curse of the Dead Gods.exe",
      .num_options = 1,
      .options = application_128_options,
    },
    { .name = "GRID Autosport",
      .executable = "GridAutosport",
      .num_options = 1,
      .options = application_130_options,
    },
    { .name = "DIRT: Showdown",
      .executable = "dirt.i386",
      .num_options = 1,
      .options = application_132_options,
    },
    { .name = "DiRT Rally",
      .executable = "DirtRally",
      .num_options = 1,
      .options = application_134_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 11,
      .options = application_136_options,
    },
    { .name = "Dead-Cells",
      .executable = "com.playdigious.deadcells.mobile",
      .num_options = 1,
      .options = application_148_options,
    },
    { .name = "Teardown",
      .executable = "teardown.exe",
      .num_options = 2,
      .options = application_150_options,
    },
    { .name = "Golf With Your Friends",
      .executable = "Golf With Your Friends.x86_64",
      .num_options = 1,
      .options = application_153_options,
    },
    { .name = "BETA CAE Systems - GL detect tool",
      .executable = "detect_opengl_tool",
      .num_options = 1,
      .options = application_155_options,
    },
    { .name = "BETA CAE Systems - ANSA",
      .executable = "ansa_linux_x86_64",
      .num_options = 2,
      .options = application_157_options,
    },
    { .name = "BETA CAE Systems - META",
      .executable = "meta_post_x86_64",
      .num_options = 1,
      .options = application_160_options,
    },
    { .name = "Mari",
      .executable_regexp = "Mari[0-9]+[.][0-9]+v[0-9]+",
      .num_options = 1,
      .options = application_162_options,
    },
    { .name = "Discovery Studio 2020",
      .executable = "DiscoveryStudio2020-bin",
      .num_options = 1,
      .options = application_164_options,
    },
    { .name = "Abaqus",
      .executable = "ABQcaeK",
      .num_options = 2,
      .options = application_166_options,
    },
    { .name = "Maya",
      .executable = "maya.bin",
      .num_options = 1,
      .options = application_169_options,
    },
    { .name = "Minecraft",
      .executable = "minecraft-launcher",
      .num_options = 1,
      .options = application_171_options,
    },
    { .name = "Minecraft-FTB",
      .executable = "ftb-app",
      .num_options = 1,
      .options = application_173_options,
    },
    { .name = "Alien Isolation",
      .executable = "AlienIsolation",
      .num_options = 1,
      .options = application_175_options,
    },
    { .name = "BioShock Infinite",
      .executable = "bioshock.i386",
      .num_options = 1,
      .options = application_177_options,
    },
    { .name = "Borderlands 2",
      .executable = "Borderlands2",
      .num_options = 1,
      .options = application_179_options,
    },
    { .name = "Civilization 5",
      .executable = "Civ5XP",
      .num_options = 1,
      .options = application_181_options,
    },
    { .name = "Civilization 6",
      .executable = "Civ6",
      .num_options = 1,
      .options = application_183_options,
    },
    { .name = "Civilization 6",
      .executable = "Civ6Sub",
      .num_options = 1,
      .options = application_185_options,
    },
    { .name = "Dreamfall Chapters",
      .executable = "Dreamfall Chapters",
      .num_options = 1,
      .options = application_187_options,
    },
    { .name = "Hitman",
      .executable = "HitmanPro",
      .num_options = 1,
      .options = application_189_options,
    },
    { .name = "Renowned Explorers: International Society",
      .executable = "abbeycore_steam",
      .num_options = 1,
      .options = application_191_options,
    },
    { .name = "Saints Row 2",
      .executable = "saintsrow2.i386",
      .num_options = 1,
      .options = application_193_options,
    },
    { .name = "Saints Row: The Third",
      .executable = "SaintsRow3.i386",
      .num_options = 1,
      .options = application_195_options,
    },
    { .name = "Saints Row IV",
      .executable = "SaintsRow4.i386",
      .num_options = 1,
      .options = application_197_options,
    },
    { .name = "Saints Row: Gat out of Hell",
      .executable = "SaintsRow4GooH.i386",
      .num_options = 1,
      .options = application_199_options,
    },
    { .name = "Sid Meier's: Civilization Beyond Earth",
      .executable = "CivBE",
      .num_options = 1,
      .options = application_201_options,
    },
    { .name = "The Witcher 2",
      .executable = "witcher2",
      .num_options = 1,
      .options = application_203_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_205_options,
    },
    { .name = "Euro Truck Simulator 2",
      .executable = "eurotrucks2",
      .num_options = 1,
      .options = application_207_options,
    },
    { .name = "Overlord",
      .executable = "overlord.i386",
      .num_options = 1,
      .options = application_209_options,
    },
    { .name = "Overlord 2",
      .executable = "overlord2.i386",
      .num_options = 1,
      .options = application_211_options,
    },
    { .name = "Oil Rush",
      .executable = "OilRush_x86",
      .num_options = 1,
      .options = application_213_options,
    },
    { .name = "War Thunder",
      .executable = "aces",
      .num_options = 1,
      .options = application_215_options,
    },
    { .name = "War Thunder (Wine)",
      .executable = "aces.exe",
      .num_options = 1,
      .options = application_217_options,
    },
    { .name = "Outlast",
      .executable = "OLGame.x86_64",
      .num_options = 1,
      .options = application_219_options,
    },
    { .name = "Spec Ops: The Line (32-bit)",
      .executable = "specops.i386",
      .num_options = 1,
      .options = application_221_options,
    },
    { .name = "Spec Ops: The Line (64-bit)",
      .executable = "specops",
      .num_options = 1,
      .options = application_223_options,
    },
    { .name = "Mount and Blade Warband",
      .executable = "mb_warband_linux",
      .num_options = 1,
      .options = application_225_options,
    },
    { .name = "Valheim",
      .executable = "valheim.x86_64",
      .num_options = 1,
      .options = application_227_options,
    },
    { .name = "Medieval II: Total War",
      .executable = "Medieval2",
      .num_options = 1,
      .options = application_229_options,
    },
    { .name = "Carnivores: Dinosaur Hunter Reborn (wine)",
      .executable = "Carnivores-master.exe",
      .num_options = 1,
      .options = application_231_options,
    },
    { .name = "Far Cry 2 (wine)",
      .executable = "farcry2.exe",
      .num_options = 1,
      .options = application_233_options,
    },
    { .name = "Talos Principle",
      .executable = "Talos",
      .num_options = 1,
      .options = application_235_options,
    },
    { .name = "Talos Principle (Unrestricted)",
      .executable = "Talos_Unrestricted",
      .num_options = 1,
      .options = application_237_options,
    },
    { .name = "7 Days To Die (64-bit)",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_239_options,
    },
    { .name = "7 Days To Die (32-bit)",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_241_options,
    },
    { .name = "Dolphin Emulator",
      .executable = "dolphin-emu",
      .num_options = 1,
      .options = application_243_options,
    },
    { .name = "Citra - Nintendo 3DS Emulator",
      .executable = "citra-qt",
      .num_options = 1,
      .options = application_245_options,
    },
    { .name = "Yuzu - Nintendo Switch Emulator",
      .executable = "yuzu",
      .num_options = 1,
      .options = application_247_options,
    },
    { .name = "RPCS3",
      .executable = "rpcs3",
      .num_options = 1,
      .options = application_249_options,
    },
    { .name = "PCSX2",
      .executable = "PCSX2",
      .num_options = 1,
      .options = application_251_options,
    },
    { .name = "From The Depths",
      .executable = "From_The_Depths.x86_64",
      .num_options = 1,
      .options = application_253_options,
    },
    { .name = "Plague Inc Evolved (32-bit)",
      .executable = "PlagueIncEvolved.x86",
      .num_options = 1,
      .options = application_255_options,
    },
    { .name = "Plague Inc Evolved (64-bit)",
      .executable = "PlagueIncEvolved.x86_64",
      .num_options = 1,
      .options = application_257_options,
    },
    { .name = "Beholder (32-bit)",
      .executable = "Beholder.x86",
      .num_options = 1,
      .options = application_259_options,
    },
    { .name = "Beholder (64-bit)",
      .executable = "Beholder.x86_64",
      .num_options = 1,
      .options = application_261_options,
    },
    { .name = "X3 Reunion",
      .executable = "X3R_main",
      .num_options = 1,
      .options = application_263_options,
    },
    { .name = "X3 Terran Conflict",
      .executable = "X3TR_main",
      .num_options = 1,
      .options = application_265_options,
    },
    { .name = "X3 Albion Prelude",
      .executable = "X3AP_main",
      .num_options = 1,
      .options = application_267_options,
    },
    { .name = "Borderlands: The Pre-Sequel",
      .executable = "BorderlandsPreSequel",
      .num_options = 1,
      .options = application_269_options,
    },
    { .name = "Transport Fever",
      .executable = "TransportFever",
      .num_options = 1,
      .options = application_271_options,
    },
    { .name = "Unigine Sanctuary",
      .executable = "Sanctuary",
      .num_options = 1,
      .options = application_273_options,
    },
    { .name = "Unigine Tropics",
      .executable = "Tropics",
      .num_options = 1,
      .options = application_275_options,
    },
    { .name = "Unigine Heaven (32-bit)",
      .executable = "heaven_x86",
      .num_options = 1,
      .options = application_277_options,
    },
    { .name = "Unigine Heaven (64-bit)",
      .executable = "heaven_x64",
      .num_options = 1,
      .options = application_279_options,
    },
    { .name = "Unigine Valley (32-bit)",
      .executable = "valley_x86",
      .num_options = 1,
      .options = application_281_options,
    },
    { .name = "Unigine Valley (64-bit)",
      .executable = "valley_x64",
      .num_options = 1,
      .options = application_283_options,
    },
    { .name = "Unigine Superposition",
      .executable = "superposition",
      .num_options = 1,
      .options = application_285_options,
    },
    { .name = "Basemark GPU",
      .executable = "BasemarkGPU_gl",
      .num_options = 1,
      .options = application_287_options,
    },
    { .name = "Stellaris",
      .executable = "stellaris",
      .num_options = 1,
      .options = application_289_options,
    },
    { .name = "Battletech",
      .executable = "BattleTech",
      .num_options = 1,
      .options = application_291_options,
    },
    { .name = "gnome-shell",
      .executable = "gnome-shell",
      .num_options = 2,
      .options = application_293_options,
    },
    { .name = "Desktop — Plasma",
      .executable = "plasmashell",
      .num_options = 1,
      .options = application_296_options,
    },
    { .name = "budgie-wm",
      .executable = "budgie-wm",
      .num_options = 1,
      .options = application_298_options,
    },
    { .name = "kwin_x11",
      .executable = "kwin_x11",
      .num_options = 1,
      .options = application_300_options,
    },
    { .name = "ksmserver-logout-greeter",
      .executable = "ksmserver-logout-greeter",
      .num_options = 1,
      .options = application_302_options,
    },
    { .name = "ksmserver-switchuser-greeter",
      .executable = "ksmserver-switchuser-greeter",
      .num_options = 1,
      .options = application_304_options,
    },
    { .name = "kscreenlocker_greet",
      .executable = "kscreenlocker_greet",
      .num_options = 1,
      .options = application_306_options,
    },
    { .name = "startplasma",
      .executable = "startplasma",
      .num_options = 1,
      .options = application_308_options,
    },
    { .name = "sddm-greeter",
      .executable = "sddm-greeter",
      .num_options = 1,
      .options = application_310_options,
    },
    { .name = "krunner",
      .executable = "krunner",
      .num_options = 1,
      .options = application_312_options,
    },
    { .name = "spectacle",
      .executable = "spectacle",
      .num_options = 1,
      .options = application_314_options,
    },
    { .name = "marco",
      .executable = "marco",
      .num_options = 1,
      .options = application_316_options,
    },
    { .name = "compton",
      .executable = "compton",
      .num_options = 1,
      .options = application_318_options,
    },
    { .name = "picom",
      .executable = "picom",
      .num_options = 1,
      .options = application_320_options,
    },
    { .name = "xfwm4",
      .executable = "xfwm4",
      .num_options = 1,
      .options = application_322_options,
    },
    { .name = "Enlightenment",
      .executable = "enlightenment",
      .num_options = 1,
      .options = application_324_options,
    },
    { .name = "mutter",
      .executable = "mutter",
      .num_options = 2,
      .options = application_326_options,
    },
    { .name = "muffin",
      .executable = "muffin",
      .num_options = 1,
      .options = application_329_options,
    },
    { .name = "compiz",
      .executable = "compiz",
      .num_options = 1,
      .options = application_331_options,
    },
    { .name = "Firefox",
      .executable = "firefox",
      .num_options = 1,
      .options = application_333_options,
    },
    { .name = "Firefox ESR",
      .executable = "firefox-esr",
      .num_options = 1,
      .options = application_335_options,
    },
    { .name = "Chromium",
      .executable = "chromium",
      .num_options = 1,
      .options = application_337_options,
    },
    { .name = "Google Chrome",
      .executable = "chrome",
      .num_options = 1,
      .options = application_339_options,
    },
    { .name = "Iceweasel",
      .executable = "iceweasel",
      .num_options = 1,
      .options = application_341_options,
    },
    { .name = "Epiphany",
      .executable = "epiphany",
      .num_options = 1,
      .options = application_343_options,
    },
    { .name = "Konqueror",
      .executable = "konqueror",
      .num_options = 1,
      .options = application_345_options,
    },
    { .name = "Falkon",
      .executable = "falkon",
      .num_options = 1,
      .options = application_347_options,
    },
    { .name = "Seamonkey",
      .executable = "seamonkey",
      .num_options = 1,
      .options = application_349_options,
    },
    { .name = "Waterfox",
      .executable = "waterfox",
      .num_options = 1,
      .options = application_351_options,
    },
    { .name = "VLC Media Player",
      .executable = "vlc",
      .num_options = 1,
      .options = application_353_options,
    },
    { .name = "Totem",
      .executable = "totem",
      .num_options = 1,
      .options = application_355_options,
    },
    { .name = "Dragon Player",
      .executable = "dragon",
      .num_options = 1,
      .options = application_357_options,
    },
    { .name = "mpv",
      .executable = "mpv",
      .num_options = 1,
      .options = application_359_options,
    },
    { .name = "Xorg",
      .executable = "Xorg",
      .num_options = 1,
      .options = application_361_options,
    },
    { .name = "gfxbench",
      .executable = "testfw_app",
      .num_options = 2,
      .options = application_363_options,
    },
    { .name = "Brink",
      .executable = "brink.exe",
      .num_options = 2,
      .options = application_366_options,
    },
    { .name = "Enter The Gungeon (32 bits)",
      .executable = "EtG.x86",
      .num_options = 1,
      .options = application_369_options,
    },
    { .name = "Enter The Gungeon (64 bits)",
      .executable = "EtG.x86_64",
      .num_options = 1,
      .options = application_371_options,
    },
    { .name = "SWKOTOR (wine)",
      .executable = "swkotor.exe",
      .num_options = 1,
      .options = application_373_options,
    },
    { .name = "Assault Android Cactus (32-bit)",
      .executable = "cactus_demo.x86",
      .num_options = 1,
      .options = application_375_options,
    },
    { .name = "Assault Android Cactus (64-bit)",
      .executable = "cactus_demo.x86_64",
      .num_options = 1,
      .options = application_377_options,
    },
    { .name = "Assault Android Cactus (32-bit)",
      .executable = "cactus.x86",
      .num_options = 1,
      .options = application_379_options,
    },
    { .name = "Assault Android Cactus (64-bit)",
      .executable = "cactus.x86_64",
      .num_options = 1,
      .options = application_381_options,
    },
    { .name = "DOOM",
      .executable = "DOOMx64vk.exe",
      .num_options = 1,
      .options = application_383_options,
    },
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 1,
      .options = application_385_options,
    },
    { .name = "Wolfenstein: Youngblood(x64vk)",
      .executable = "Youngblood_x64vk.exe",
      .num_options = 1,
      .options = application_387_options,
    },
    { .name = "Metro: Exodus",
      .application_name_match = "metroexodus",
      .num_options = 1,
      .options = application_389_options,
    },
    { .name = "Forsaken Remastered",
      .executable = "ForsakenEx",
      .num_options = 1,
      .options = application_391_options,
    },
};

static const struct driconf_device device_1 = {
    .num_engines = 1,
    .engines = device_1_engines,
    .num_applications = 177,
    .applications = device_1_applications,
};


    
static const struct driconf_option application_396_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_EXT_buffer_age -GLX_OML_sync_control -GLX_SGI_video_sync" },
};

    
static const struct driconf_option application_398_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_EXT_buffer_age -GLX_OML_sync_control" },
};


static const struct driconf_application device_395_applications[] = {
    { .name = "gnome-shell",
      .executable = "gnome-shell",
      .num_options = 1,
      .options = application_396_options,
    },
    { .name = "Compiz",
      .executable = "Compiz",
      .num_options = 1,
      .options = application_398_options,
    },
};

static const struct driconf_device device_395 = {
    .driver = "vmwgfx",
    .num_engines = 0,
    .num_applications = 2,
    .applications = device_395_applications,
};


    
static const struct driconf_option application_401_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_403_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_405_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_407_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_409_options[] = {
    { .name = "mesa_glthread", .value = "true" },
    { .name = "radeonsi_zerovram", .value = "true" },
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_413_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_415_options[] = {
    { .name = "radeonsi_prim_restart_tri_strips_only", .value = "true" },
};

    
static const struct driconf_option application_417_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_419_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_422_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_424_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_426_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_428_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_430_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_432_options[] = {
    { .name = "radeonsi_no_infinite_interp", .value = "true" },
};

    
static const struct driconf_option application_434_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_436_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_438_options[] = {
    { .name = "glthread_nop_check_framebuffer_status", .value = "true" },
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_441_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_443_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_445_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
    { .name = "radeonsi_no_infinite_interp", .value = "true" },
};

    
static const struct driconf_option application_448_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_450_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_452_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
};

    
static const struct driconf_option application_454_options[] = {
    { .name = "force_gl_vendor", .value = "ATI Technologies Inc." },
    { .name = "radeonsi_force_use_fma32", .value = "true" },
};

    
static const struct driconf_option application_457_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};


static const struct driconf_application device_400_applications[] = {
    { .name = "Half Life 2",
      .executable = "hl2_linux",
      .num_options = 1,
      .options = application_401_options,
    },
    { .name = "Black Mesa",
      .executable = "bms_linux",
      .num_options = 1,
      .options = application_403_options,
    },
    { .name = "Alien Isolation",
      .executable = "AlienIsolation",
      .num_options = 1,
      .options = application_405_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_407_options,
    },
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 3,
      .options = application_409_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_413_options,
    },
    { .name = "DiRT Rally",
      .executable = "DirtRally",
      .num_options = 1,
      .options = application_415_options,
    },
    { .name = "Gfx Bench",
      .executable = "gfxbench_gl",
      .num_options = 1,
      .options = application_417_options,
    },
    { .name = "Monolith demo5",
      .executable = "runner",
      .sha1 = "0c40d509a74e357f0280cb1bd882e9cd94b91bdf",
      .num_options = 2,
      .options = application_419_options,
    },
    { .name = "Memoranda / Riptale",
      .executable = "runner",
      .sha1 = "aa13dec6af63c88f308ebb487693896434a4db56",
      .num_options = 1,
      .options = application_422_options,
    },
    { .name = "Nuclear Throne",
      .executable = "runner",
      .sha1 = "84814e8db125e889f5d9d4195a0ca72a871ea1fd",
      .num_options = 1,
      .options = application_424_options,
    },
    { .name = "Undertale",
      .executable = "runner",
      .sha1 = "dfa302e7ec78641d0696dbbc1a06fc29f34ff1ff",
      .num_options = 1,
      .options = application_426_options,
    },
    { .name = "Turmoil",
      .executable = "runner",
      .sha1 = "cbbf757aaab289859f8dae191a7d63afc30643d9",
      .num_options = 1,
      .options = application_428_options,
    },
    { .name = "Peace, Death!",
      .executable = "runner",
      .sha1 = "5b909f3d21799773370adf084f649848f098234e",
      .num_options = 1,
      .options = application_430_options,
    },
    { .name = "Kerbal Space Program",
      .executable = "KSP.x86_64",
      .num_options = 1,
      .options = application_432_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_434_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_436_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 2,
      .options = application_438_options,
    },
    { .name = "Road Redemption",
      .executable = "RoadRedemption.x86_64",
      .num_options = 1,
      .options = application_441_options,
    },
    { .name = "Wasteland 2",
      .executable = "WL2",
      .num_options = 1,
      .options = application_443_options,
    },
    { .name = "Teardown",
      .executable = "teardown.exe",
      .num_options = 2,
      .options = application_445_options,
    },
    { .name = "SpaceEngine",
      .executable = "SpaceEngine.exe",
      .num_options = 1,
      .options = application_448_options,
    },
    { .name = "BETA CAE Systems - GL detect tool",
      .executable = "detect_opengl_tool",
      .num_options = 1,
      .options = application_450_options,
    },
    { .name = "BETA CAE Systems - ANSA",
      .executable = "ansa_linux_x86_64",
      .num_options = 1,
      .options = application_452_options,
    },
    { .name = "BETA CAE Systems - META",
      .executable = "meta_post_x86_64",
      .num_options = 2,
      .options = application_454_options,
    },
    { .name = "Black Geyser: Couriers of Darkness",
      .executable = "BlackGeyser.x86_64",
      .num_options = 1,
      .options = application_457_options,
    },
};

static const struct driconf_device device_400 = {
    .driver = "radeonsi",
    .num_engines = 0,
    .num_applications = 26,
    .applications = device_400_applications,
};


    
static const struct driconf_option application_460_options[] = {
    { .name = "vs_position_always_invariant", .value = "true" },
};


static const struct driconf_application device_459_applications[] = {
    { .name = "Middle Earth: Shadow of Mordor",
      .executable = "ShadowOfMordor",
      .num_options = 1,
      .options = application_460_options,
    },
};

static const struct driconf_device device_459 = {
    .driver = "i965",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_459_applications,
};


    
static const struct driconf_option application_463_options[] = {
    { .name = "vs_position_always_invariant", .value = "true" },
};


static const struct driconf_application device_462_applications[] = {
    { .name = "Middle Earth: Shadow of Mordor",
      .executable = "ShadowOfMordor",
      .num_options = 1,
      .options = application_463_options,
    },
};

static const struct driconf_device device_462 = {
    .driver = "iris",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_462_applications,
};


    
static const struct driconf_option application_466_options[] = {
    { .name = "anv_assume_full_subgroups", .value = "true" },
};

    
static const struct driconf_option application_468_options[] = {
    { .name = "anv_assume_full_subgroups", .value = "true" },
};


static const struct driconf_application device_465_applications[] = {
    { .name = "Aperture Desk Job",
      .executable = "deskjob",
      .num_options = 1,
      .options = application_466_options,
    },
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 1,
      .options = application_468_options,
    },
};

static const struct driconf_device device_465 = {
    .driver = "anv",
    .num_engines = 0,
    .num_applications = 2,
    .applications = device_465_applications,
};


    
static const struct driconf_option application_471_options[] = {
    { .name = "format_l8_srgb_enable_readback", .value = "true" },
};


static const struct driconf_application device_470_applications[] = {
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 1,
      .options = application_471_options,
    },
};

static const struct driconf_device device_470 = {
    .driver = "virtio_gpu",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_470_applications,
};


    
static const struct driconf_option application_474_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 618" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_477_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 630" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_480_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 618" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};

    
static const struct driconf_option application_483_options[] = {
    { .name = "force_gl_renderer", .value = "Adreno (TM) 630" },
    { .name = "force_gl_vendor", .value = "Qualcomm" },
};


static const struct driconf_application device_473_applications[] = {
    { .name = "Call of Duty Mobile",
      .executable = "com.activision.callofduty.shooter",
      .num_options = 2,
      .options = application_474_options,
    },
    { .name = "Asphalt 8",
      .executable = "com.gameloft.android.ANMP.GloftA8HM",
      .num_options = 2,
      .options = application_477_options,
    },
    { .name = "Asphalt 9",
      .executable = "com.gameloft.android.ANMP.GloftA9HM",
      .num_options = 2,
      .options = application_480_options,
    },
    { .name = "PUBG Mobile",
      .executable = "com.tencent.ig",
      .num_options = 2,
      .options = application_483_options,
    },
};

static const struct driconf_device device_473 = {
    .driver = "msm",
    .device = "FD618",
    .num_engines = 0,
    .num_applications = 4,
    .applications = device_473_applications,
};

static const struct driconf_device *driconf[] = {
    &device_1,
    &device_395,
    &device_400,
    &device_459,
    &device_462,
    &device_465,
    &device_470,
    &device_473,
};
