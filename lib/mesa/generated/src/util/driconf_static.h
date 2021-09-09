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
    unsigned num_engines;
    const struct driconf_engine *engines;
    unsigned num_applications;
    const struct driconf_application *applications;
};



    
static const struct driconf_option engine_323_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};


static const struct driconf_engine device_1_engines[] = {
    { .engine_name_match = "UnrealEngine4.*",
      .engine_versions = "0:23",
      .num_options = 1,
      .options = engine_323_options,
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
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_13_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_16_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_19_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "dual_color_blend_by_location", .value = "true" },
};

    
static const struct driconf_option application_22_options[] = {
    { .name = "disable_blend_func_extended", .value = "true" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_25_options[] = {
    { .name = "disable_blend_func_extended", .value = "true" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_28_options[] = {
    { .name = "disable_glsl_line_continuations", .value = "true" },
};

    
static const struct driconf_option application_30_options[] = {
    { .name = "always_have_depth_buffer", .value = "true" },
};

    
static const struct driconf_option application_32_options[] = {
    { .name = "always_have_depth_buffer", .value = "true" },
};

    
static const struct driconf_option application_34_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_37_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_39_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
    { .name = "force_glsl_extensions_warn", .value = "true" },
};

    
static const struct driconf_option application_42_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_44_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_46_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_48_options[] = {
    { .name = "allow_glsl_builtin_variable_redeclaration", .value = "true" },
};

    
static const struct driconf_option application_50_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_52_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_54_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_56_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_58_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
    { .name = "force_gl_vendor", .value = "ATI Technologies, Inc." },
};

    
static const struct driconf_option application_62_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_64_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_66_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_68_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_70_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_72_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_74_options[] = {
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_76_options[] = {
    { .name = "force_glsl_version", .value = "440" },
};

    
static const struct driconf_option application_78_options[] = {
    { .name = "force_glsl_abs_sqrt", .value = "true" },
};

    
static const struct driconf_option application_80_options[] = {
    { .name = "force_glsl_abs_sqrt", .value = "true" },
};

    
static const struct driconf_option application_82_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_84_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_86_options[] = {
    { .name = "force_glsl_version", .value = "130" },
    { .name = "glsl_ignore_write_to_readonly_var", .value = "true" },
};

    
static const struct driconf_option application_89_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_91_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_93_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_95_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_97_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_99_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_101_options[] = {
    { .name = "allow_glsl_cross_stage_interpolation_mismatch", .value = "true" },
};

    
static const struct driconf_option application_103_options[] = {
    { .name = "allow_glsl_builtin_const_expression", .value = "true" },
    { .name = "allow_glsl_relaxed_es", .value = "true" },
};

    
static const struct driconf_option application_106_options[] = {
    { .name = "allow_extra_pp_tokens", .value = "true" },
};

    
static const struct driconf_option application_108_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_110_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_112_options[] = {
    { .name = "force_compat_profile", .value = "true" },
};

    
static const struct driconf_option application_114_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_116_options[] = {
    { .name = "glsl_zero_init", .value = "true" },
};

    
static const struct driconf_option application_118_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
};

    
static const struct driconf_option application_120_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
};

    
static const struct driconf_option application_122_options[] = {
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
    { .name = "allow_glsl_120_subset_in_110", .value = "true" },
    { .name = "force_gl_names_reuse", .value = "true" },
    { .name = "force_gl_vendor", .value = "NVIDIA Corporation" },
    { .name = "force_glsl_extensions_warn", .value = "true" },
    { .name = "allow_incorrect_primitive_id", .value = "true" },
    { .name = "allow_draw_out_of_order", .value = "true" },
    { .name = "mesa_glthread", .value = "true" },
    { .name = "mesa_no_error", .value = "true" },
};

    
static const struct driconf_option application_132_options[] = {
    { .name = "ignore_map_unsynchronized", .value = "true" },
};

    
static const struct driconf_option application_134_options[] = {
    { .name = "force_integer_tex_nearest", .value = "true" },
    { .name = "allow_glsl_extension_directive_midshader", .value = "true" },
};

    
static const struct driconf_option application_137_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_139_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_141_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_143_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_145_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_147_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_149_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_151_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_153_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_155_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_157_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_159_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_161_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_163_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_165_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_167_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_169_options[] = {
    { .name = "mesa_glthread", .value = "true" },
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
    { .name = "adaptive_sync", .value = "false" },
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_240_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_242_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_244_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_246_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_248_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_250_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_252_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_254_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_256_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_258_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_260_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_262_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_264_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_266_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_268_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_270_options[] = {
    { .name = "adaptive_sync", .value = "false" },
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_273_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_275_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_277_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_279_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_281_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_283_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_285_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_287_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_289_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_291_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_293_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_295_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_297_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_299_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_301_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_303_options[] = {
    { .name = "adaptive_sync", .value = "false" },
};

    
static const struct driconf_option application_305_options[] = {
    { .name = "v3d_nonmsaa_texture_size_limit", .value = "true" },
};

    
static const struct driconf_option application_307_options[] = {
    { .name = "vk_x11_override_min_image_count", .value = "2" },
    { .name = "vk_wsi_force_bgra8_unorm_first", .value = "true" },
};

    
static const struct driconf_option application_310_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_OML_swap_method" },
    { .name = "allow_higher_compat_version", .value = "true" },
};

    
static const struct driconf_option application_313_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_315_options[] = {
    { .name = "glsl_correct_derivatives_after_discard", .value = "true" },
};

    
static const struct driconf_option application_317_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
};

    
static const struct driconf_option application_319_options[] = {
    { .name = "vk_x11_ensure_min_image_count", .value = "true" },
};

    
static const struct driconf_option application_321_options[] = {
    { .name = "vk_x11_strict_image_count", .value = "true" },
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
    { .name = "Unigine Heaven (32-bit)",
      .executable = "heaven_x86",
      .num_options = 2,
      .options = application_10_options,
    },
    { .name = "Unigine Heaven (64-bit)",
      .executable = "heaven_x64",
      .num_options = 2,
      .options = application_13_options,
    },
    { .name = "Unigine Valley (32-bit)",
      .executable = "valley_x86",
      .num_options = 2,
      .options = application_16_options,
    },
    { .name = "Unigine Valley (64-bit)",
      .executable = "valley_x64",
      .num_options = 2,
      .options = application_19_options,
    },
    { .name = "Unigine OilRush (32-bit)",
      .executable = "OilRush_x86",
      .num_options = 2,
      .options = application_22_options,
    },
    { .name = "Unigine OilRush (64-bit)",
      .executable = "OilRush_x64",
      .num_options = 2,
      .options = application_25_options,
    },
    { .name = "Savage 2",
      .executable = "savage2.bin",
      .num_options = 1,
      .options = application_28_options,
    },
    { .name = "Topogun (32-bit)",
      .executable = "topogun32",
      .num_options = 1,
      .options = application_30_options,
    },
    { .name = "Topogun (64-bit)",
      .executable = "topogun64",
      .num_options = 1,
      .options = application_32_options,
    },
    { .name = "Dead Island (incl. Definitive Edition)",
      .executable = "DeadIslandGame",
      .num_options = 2,
      .options = application_34_options,
    },
    { .name = "Dead Island Riptide Definitive Edition",
      .executable = "DeadIslandRiptideGame",
      .num_options = 1,
      .options = application_37_options,
    },
    { .name = "Doom 3: BFG",
      .executable = "Doom3BFG.exe",
      .num_options = 2,
      .options = application_39_options,
    },
    { .name = "Dying Light",
      .executable = "DyingLightGame",
      .num_options = 1,
      .options = application_42_options,
    },
    { .name = "Full Bore",
      .executable = "fullbore",
      .num_options = 1,
      .options = application_44_options,
    },
    { .name = "RAGE (64-bit)",
      .executable = "Rage64.exe",
      .num_options = 1,
      .options = application_46_options,
    },
    { .name = "RAGE (32-bit)",
      .executable = "Rage.exe",
      .num_options = 1,
      .options = application_48_options,
    },
    { .name = "Second Life",
      .executable = "do-not-directly-run-secondlife-bin",
      .num_options = 1,
      .options = application_50_options,
    },
    { .name = "Warsow (32-bit)",
      .executable = "warsow.i386",
      .num_options = 1,
      .options = application_52_options,
    },
    { .name = "Warsow (64-bit)",
      .executable = "warsow.x86_64",
      .num_options = 1,
      .options = application_54_options,
    },
    { .name = "Rust",
      .executable = "rust",
      .num_options = 1,
      .options = application_56_options,
    },
    { .name = "Divinity: Original Sin Enhanced Edition",
      .executable = "EoCApp",
      .num_options = 3,
      .options = application_58_options,
    },
    { .name = "Metro 2033 Redux / Metro Last Night Redux",
      .executable = "metro",
      .num_options = 1,
      .options = application_62_options,
    },
    { .name = "Worms W.M.D",
      .executable = "Worms W.M.Dx64",
      .num_options = 1,
      .options = application_64_options,
    },
    { .name = "Crookz - The Big Heist",
      .executable = "Crookz",
      .num_options = 1,
      .options = application_66_options,
    },
    { .name = "Tropico 5",
      .executable = "Tropico5",
      .num_options = 1,
      .options = application_68_options,
    },
    { .name = "Faster than Light (32-bit)",
      .executable = "FTL.x86",
      .num_options = 1,
      .options = application_70_options,
    },
    { .name = "Faster than Light (64-bit)",
      .executable = "FTL.amd64",
      .num_options = 1,
      .options = application_72_options,
    },
    { .name = "Final Fantasy VIII: Remastered",
      .executable = "FFVIII.exe",
      .num_options = 1,
      .options = application_74_options,
    },
    { .name = "The Culling",
      .executable = "Victory",
      .num_options = 1,
      .options = application_76_options,
    },
    { .name = "Spec Ops: The Line (32-bit)",
      .executable = "specops.i386",
      .num_options = 1,
      .options = application_78_options,
    },
    { .name = "Spec Ops: The Line (64-bit)",
      .executable = "specops",
      .num_options = 1,
      .options = application_80_options,
    },
    { .name = "Kerbal Space Program (32-bit)",
      .executable = "KSP.x86",
      .num_options = 1,
      .options = application_82_options,
    },
    { .name = "Kerbal Space Program (64-bit)",
      .executable = "KSP.x86_64",
      .num_options = 1,
      .options = application_84_options,
    },
    { .name = "Luna Sky",
      .executable = "lunasky",
      .num_options = 2,
      .options = application_86_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_89_options,
    },
    { .name = "The Witcher 2",
      .executable = "witcher2",
      .num_options = 1,
      .options = application_91_options,
    },
    { .name = "Unreal 4 Editor",
      .executable = "UE4Editor",
      .num_options = 1,
      .options = application_93_options,
    },
    { .name = "Observer",
      .executable = "TheObserver-Linux-Shipping",
      .num_options = 1,
      .options = application_95_options,
    },
    { .name = "Steamroll",
      .executable = "Steamroll-Linux-Shipping",
      .num_options = 1,
      .options = application_97_options,
    },
    { .name = "Refunct",
      .executable = "Refunct-Linux-Shipping",
      .num_options = 1,
      .options = application_99_options,
    },
    { .name = "We Happy Few",
      .executable = "GlimpseGame",
      .num_options = 1,
      .options = application_101_options,
    },
    { .name = "Google Earth VR",
      .executable = "Earth.exe",
      .num_options = 2,
      .options = application_103_options,
    },
    { .name = "Champions of Regnum",
      .executable = "game",
      .num_options = 1,
      .options = application_106_options,
    },
    { .name = "Wolfenstein The Old Blood",
      .executable = "WolfOldBlood_x64.exe",
      .num_options = 1,
      .options = application_108_options,
    },
    { .name = "ARMA 3",
      .executable = "arma3.x86_64",
      .num_options = 1,
      .options = application_110_options,
    },
    { .name = "Epic Games Launcher",
      .executable = "EpicGamesLauncher.exe",
      .num_options = 1,
      .options = application_112_options,
    },
    { .name = "GpuTest",
      .executable = "GpuTest",
      .num_options = 1,
      .options = application_114_options,
    },
    { .name = "Curse of the Dead Gods",
      .executable = "Curse of the Dead Gods.exe",
      .num_options = 1,
      .options = application_116_options,
    },
    { .name = "GRID Autosport",
      .executable = "GridAutosport",
      .num_options = 1,
      .options = application_118_options,
    },
    { .name = "DIRT: Showdown",
      .executable = "dirt.i386",
      .num_options = 1,
      .options = application_120_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 9,
      .options = application_122_options,
    },
    { .name = "Dead-Cells",
      .executable = "com.playdigious.deadcells.mobile",
      .num_options = 1,
      .options = application_132_options,
    },
    { .name = "Teardown",
      .executable = "teardown.exe",
      .num_options = 2,
      .options = application_134_options,
    },
    { .name = "Golf With Your Friends",
      .executable = "Golf With Your Friends.x86_64",
      .num_options = 1,
      .options = application_137_options,
    },
    { .name = "Alien Isolation",
      .executable = "AlienIsolation",
      .num_options = 1,
      .options = application_139_options,
    },
    { .name = "BioShock Infinite",
      .executable = "bioshock.i386",
      .num_options = 1,
      .options = application_141_options,
    },
    { .name = "Borderlands 2",
      .executable = "Borderlands2",
      .num_options = 1,
      .options = application_143_options,
    },
    { .name = "Civilization 5",
      .executable = "Civ5XP",
      .num_options = 1,
      .options = application_145_options,
    },
    { .name = "Civilization 6",
      .executable = "Civ6",
      .num_options = 1,
      .options = application_147_options,
    },
    { .name = "Civilization 6",
      .executable = "Civ6Sub",
      .num_options = 1,
      .options = application_149_options,
    },
    { .name = "Dreamfall Chapters",
      .executable = "Dreamfall Chapters",
      .num_options = 1,
      .options = application_151_options,
    },
    { .name = "Hitman",
      .executable = "HitmanPro",
      .num_options = 1,
      .options = application_153_options,
    },
    { .name = "Renowned Explorers: International Society",
      .executable = "abbeycore_steam",
      .num_options = 1,
      .options = application_155_options,
    },
    { .name = "Saints Row 2",
      .executable = "saintsrow2.i386",
      .num_options = 1,
      .options = application_157_options,
    },
    { .name = "Saints Row: The Third",
      .executable = "SaintsRow3.i386",
      .num_options = 1,
      .options = application_159_options,
    },
    { .name = "Saints Row IV",
      .executable = "SaintsRow4.i386",
      .num_options = 1,
      .options = application_161_options,
    },
    { .name = "Saints Row: Gat out of Hell",
      .executable = "SaintsRow4GooH.i386",
      .num_options = 1,
      .options = application_163_options,
    },
    { .name = "Sid Meier's: Civilization Beyond Earth",
      .executable = "CivBE",
      .num_options = 1,
      .options = application_165_options,
    },
    { .name = "The Witcher 2",
      .executable = "witcher2",
      .num_options = 1,
      .options = application_167_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_169_options,
    },
    { .name = "Euro Truck Simulator 2",
      .executable = "eurotrucks2",
      .num_options = 1,
      .options = application_171_options,
    },
    { .name = "Overlord",
      .executable = "overlord.i386",
      .num_options = 1,
      .options = application_173_options,
    },
    { .name = "Overlord 2",
      .executable = "overlord2.i386",
      .num_options = 1,
      .options = application_175_options,
    },
    { .name = "Oil Rush",
      .executable = "OilRush_x86",
      .num_options = 1,
      .options = application_177_options,
    },
    { .name = "War Thunder",
      .executable = "aces",
      .num_options = 1,
      .options = application_179_options,
    },
    { .name = "War Thunder (Wine)",
      .executable = "aces.exe",
      .num_options = 1,
      .options = application_181_options,
    },
    { .name = "Outlast",
      .executable = "OLGame.x86_64",
      .num_options = 1,
      .options = application_183_options,
    },
    { .name = "Spec Ops: The Line (32-bit)",
      .executable = "specops.i386",
      .num_options = 1,
      .options = application_185_options,
    },
    { .name = "Spec Ops: The Line (64-bit)",
      .executable = "specops",
      .num_options = 1,
      .options = application_187_options,
    },
    { .name = "Mount and Blade Warband",
      .executable = "mb_warband_linux",
      .num_options = 1,
      .options = application_189_options,
    },
    { .name = "Valheim",
      .executable = "valheim.x86_64",
      .num_options = 1,
      .options = application_191_options,
    },
    { .name = "Medieval II: Total War",
      .executable = "Medieval2",
      .num_options = 1,
      .options = application_193_options,
    },
    { .name = "Carnivores: Dinosaur Hunter Reborn (wine)",
      .executable = "Carnivores-master.exe",
      .num_options = 1,
      .options = application_195_options,
    },
    { .name = "Far Cry 2 (wine)",
      .executable = "farcry2.exe",
      .num_options = 1,
      .options = application_197_options,
    },
    { .name = "Talos Principle",
      .executable = "Talos",
      .num_options = 1,
      .options = application_199_options,
    },
    { .name = "Talos Principle (Unrestricted)",
      .executable = "Talos_Unrestricted",
      .num_options = 1,
      .options = application_201_options,
    },
    { .name = "7 Days To Die (64-bit)",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_203_options,
    },
    { .name = "7 Days To Die (32-bit)",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_205_options,
    },
    { .name = "Dolphin Emulator",
      .executable = "dolphin-emu",
      .num_options = 1,
      .options = application_207_options,
    },
    { .name = "Citra - Nintendo 3DS Emulator",
      .executable = "citra-qt",
      .num_options = 1,
      .options = application_209_options,
    },
    { .name = "Yuzu - Nintendo Switch Emulator",
      .executable = "yuzu",
      .num_options = 1,
      .options = application_211_options,
    },
    { .name = "RPCS3",
      .executable = "rpcs3",
      .num_options = 1,
      .options = application_213_options,
    },
    { .name = "PCSX2",
      .executable = "PCSX2",
      .num_options = 1,
      .options = application_215_options,
    },
    { .name = "From The Depths",
      .executable = "From_The_Depths.x86_64",
      .num_options = 1,
      .options = application_217_options,
    },
    { .name = "Plague Inc Evolved (32-bit)",
      .executable = "PlagueIncEvolved.x86",
      .num_options = 1,
      .options = application_219_options,
    },
    { .name = "Plague Inc Evolved (64-bit)",
      .executable = "PlagueIncEvolved.x86_64",
      .num_options = 1,
      .options = application_221_options,
    },
    { .name = "Beholder (32-bit)",
      .executable = "Beholder.x86",
      .num_options = 1,
      .options = application_223_options,
    },
    { .name = "Beholder (64-bit)",
      .executable = "Beholder.x86_64",
      .num_options = 1,
      .options = application_225_options,
    },
    { .name = "X3 Reunion",
      .executable = "X3R_main",
      .num_options = 1,
      .options = application_227_options,
    },
    { .name = "X3 Terran Conflict",
      .executable = "X3TR_main",
      .num_options = 1,
      .options = application_229_options,
    },
    { .name = "X3 Albion Prelude",
      .executable = "X3AP_main",
      .num_options = 1,
      .options = application_231_options,
    },
    { .name = "Borderlands: The Pre-Sequel",
      .executable = "BorderlandsPreSequel",
      .num_options = 1,
      .options = application_233_options,
    },
    { .name = "Transport Fever",
      .executable = "TransportFever",
      .num_options = 1,
      .options = application_235_options,
    },
    { .name = "gnome-shell",
      .executable = "gnome-shell",
      .num_options = 2,
      .options = application_237_options,
    },
    { .name = "Desktop — Plasma",
      .executable = "plasmashell",
      .num_options = 1,
      .options = application_240_options,
    },
    { .name = "budgie-wm",
      .executable = "budgie-wm",
      .num_options = 1,
      .options = application_242_options,
    },
    { .name = "kwin_x11",
      .executable = "kwin_x11",
      .num_options = 1,
      .options = application_244_options,
    },
    { .name = "ksmserver-logout-greeter",
      .executable = "ksmserver-logout-greeter",
      .num_options = 1,
      .options = application_246_options,
    },
    { .name = "ksmserver-switchuser-greeter",
      .executable = "ksmserver-switchuser-greeter",
      .num_options = 1,
      .options = application_248_options,
    },
    { .name = "kscreenlocker_greet",
      .executable = "kscreenlocker_greet",
      .num_options = 1,
      .options = application_250_options,
    },
    { .name = "startplasma",
      .executable = "startplasma",
      .num_options = 1,
      .options = application_252_options,
    },
    { .name = "sddm-greeter",
      .executable = "sddm-greeter",
      .num_options = 1,
      .options = application_254_options,
    },
    { .name = "krunner",
      .executable = "krunner",
      .num_options = 1,
      .options = application_256_options,
    },
    { .name = "spectacle",
      .executable = "spectacle",
      .num_options = 1,
      .options = application_258_options,
    },
    { .name = "marco",
      .executable = "marco",
      .num_options = 1,
      .options = application_260_options,
    },
    { .name = "compton",
      .executable = "compton",
      .num_options = 1,
      .options = application_262_options,
    },
    { .name = "picom",
      .executable = "picom",
      .num_options = 1,
      .options = application_264_options,
    },
    { .name = "xfwm4",
      .executable = "xfwm4",
      .num_options = 1,
      .options = application_266_options,
    },
    { .name = "Enlightenment",
      .executable = "enlightenment",
      .num_options = 1,
      .options = application_268_options,
    },
    { .name = "mutter",
      .executable = "mutter",
      .num_options = 2,
      .options = application_270_options,
    },
    { .name = "muffin",
      .executable = "muffin",
      .num_options = 1,
      .options = application_273_options,
    },
    { .name = "compiz",
      .executable = "compiz",
      .num_options = 1,
      .options = application_275_options,
    },
    { .name = "Firefox",
      .executable = "firefox",
      .num_options = 1,
      .options = application_277_options,
    },
    { .name = "Firefox ESR",
      .executable = "firefox-esr",
      .num_options = 1,
      .options = application_279_options,
    },
    { .name = "Chromium",
      .executable = "chromium",
      .num_options = 1,
      .options = application_281_options,
    },
    { .name = "Google Chrome",
      .executable = "chrome",
      .num_options = 1,
      .options = application_283_options,
    },
    { .name = "Iceweasel",
      .executable = "iceweasel",
      .num_options = 1,
      .options = application_285_options,
    },
    { .name = "Epiphany",
      .executable = "epiphany",
      .num_options = 1,
      .options = application_287_options,
    },
    { .name = "Konqueror",
      .executable = "konqueror",
      .num_options = 1,
      .options = application_289_options,
    },
    { .name = "Falkon",
      .executable = "falkon",
      .num_options = 1,
      .options = application_291_options,
    },
    { .name = "Seamonkey",
      .executable = "seamonkey",
      .num_options = 1,
      .options = application_293_options,
    },
    { .name = "Waterfox",
      .executable = "waterfox",
      .num_options = 1,
      .options = application_295_options,
    },
    { .name = "VLC Media Player",
      .executable = "vlc",
      .num_options = 1,
      .options = application_297_options,
    },
    { .name = "Totem",
      .executable = "totem",
      .num_options = 1,
      .options = application_299_options,
    },
    { .name = "Dragon Player",
      .executable = "dragon",
      .num_options = 1,
      .options = application_301_options,
    },
    { .name = "mpv",
      .executable = "mpv",
      .num_options = 1,
      .options = application_303_options,
    },
    { .name = "Xorg",
      .executable = "Xorg",
      .num_options = 1,
      .options = application_305_options,
    },
    { .name = "gfxbench",
      .executable = "testfw_app",
      .num_options = 2,
      .options = application_307_options,
    },
    { .name = "Brink",
      .executable = "brink.exe",
      .num_options = 2,
      .options = application_310_options,
    },
    { .name = "Enter The Gungeon (32 bits)",
      .executable = "EtG.x86",
      .num_options = 1,
      .options = application_313_options,
    },
    { .name = "Enter The Gungeon (64 bits)",
      .executable = "EtG.x86_64",
      .num_options = 1,
      .options = application_315_options,
    },
    { .name = "DOOM",
      .executable = "DOOMx64vk.exe",
      .num_options = 1,
      .options = application_317_options,
    },
    { .name = "DOOMEternal",
      .executable = "DOOMEternalx64vk.exe",
      .num_options = 1,
      .options = application_319_options,
    },
    { .name = "Metro: Exodus",
      .application_name_match = "metroexodus",
      .num_options = 1,
      .options = application_321_options,
    },
};

static const struct driconf_device device_1 = {
    .num_engines = 1,
    .engines = device_1_engines,
    .num_applications = 146,
    .applications = device_1_applications,
};


    
static const struct driconf_option application_326_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_EXT_buffer_age -GLX_OML_sync_control -GLX_SGI_video_sync" },
};

    
static const struct driconf_option application_328_options[] = {
    { .name = "glx_extension_override", .value = "-GLX_EXT_buffer_age -GLX_OML_sync_control" },
};


static const struct driconf_application device_325_applications[] = {
    { .name = "gnome-shell",
      .executable = "gnome-shell",
      .num_options = 1,
      .options = application_326_options,
    },
    { .name = "Compiz",
      .executable = "Compiz",
      .num_options = 1,
      .options = application_328_options,
    },
};

static const struct driconf_device device_325 = {
    .driver = "vmwgfx",
    .num_engines = 0,
    .num_applications = 2,
    .applications = device_325_applications,
};


    
static const struct driconf_option application_331_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_333_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_335_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};

    
static const struct driconf_option application_337_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_339_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_342_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_344_options[] = {
    { .name = "radeonsi_prim_restart_tri_strips_only", .value = "true" },
};

    
static const struct driconf_option application_346_options[] = {
    { .name = "mesa_glthread", .value = "true" },
};

    
static const struct driconf_option application_348_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_351_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_353_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_355_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_357_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_359_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
};

    
static const struct driconf_option application_361_options[] = {
    { .name = "radeonsi_no_infinite_interp", .value = "true" },
};

    
static const struct driconf_option application_363_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_365_options[] = {
    { .name = "radeonsi_zerovram", .value = "true" },
};

    
static const struct driconf_option application_367_options[] = {
    { .name = "radeonsi_sync_compile", .value = "true" },
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
    { .name = "radeonsi_inline_uniforms", .value = "true" },
    { .name = "radeonsi_shader_culling", .value = "true" },
};

    
static const struct driconf_option application_372_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_374_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_376_options[] = {
    { .name = "radeonsi_clamp_div_by_zero", .value = "true" },
};

    
static const struct driconf_option application_378_options[] = {
    { .name = "force_gl_vendor", .value = "X.Org" },
};


static const struct driconf_application device_330_applications[] = {
    { .name = "Half Life 2",
      .executable = "hl2_linux",
      .num_options = 1,
      .options = application_331_options,
    },
    { .name = "Black Mesa",
      .executable = "bms_linux",
      .num_options = 1,
      .options = application_333_options,
    },
    { .name = "Alien Isolation",
      .executable = "AlienIsolation",
      .num_options = 1,
      .options = application_335_options,
    },
    { .name = "American Truck Simulator",
      .executable = "amtrucks",
      .num_options = 1,
      .options = application_337_options,
    },
    { .name = "Counter-Strike Global Offensive",
      .executable = "csgo_linux64",
      .num_options = 2,
      .options = application_339_options,
    },
    { .name = "Rocket League",
      .executable = "RocketLeague",
      .num_options = 1,
      .options = application_342_options,
    },
    { .name = "DiRT Rally",
      .executable = "DirtRally",
      .num_options = 1,
      .options = application_344_options,
    },
    { .name = "Gfx Bench",
      .executable = "gfxbench_gl",
      .num_options = 1,
      .options = application_346_options,
    },
    { .name = "Monolith demo5",
      .executable = "runner",
      .sha1 = "0c40d509a74e357f0280cb1bd882e9cd94b91bdf",
      .num_options = 2,
      .options = application_348_options,
    },
    { .name = "Memoranda",
      .executable = "runner",
      .sha1 = "aa13dec6af63c88f308ebb487693896434a4db56",
      .num_options = 1,
      .options = application_351_options,
    },
    { .name = "Nuclear Throne",
      .executable = "runner",
      .sha1 = "84814e8db125e889f5d9d4195a0ca72a871ea1fd",
      .num_options = 1,
      .options = application_353_options,
    },
    { .name = "Undertale",
      .executable = "runner",
      .sha1 = "dfa302e7ec78641d0696dbbc1a06fc29f34ff1ff",
      .num_options = 1,
      .options = application_355_options,
    },
    { .name = "Turmoil",
      .executable = "runner",
      .sha1 = "cbbf757aaab289859f8dae191a7d63afc30643d9",
      .num_options = 1,
      .options = application_357_options,
    },
    { .name = "Peace, Death!",
      .executable = "runner",
      .sha1 = "5b909f3d21799773370adf084f649848f098234e",
      .num_options = 1,
      .options = application_359_options,
    },
    { .name = "Kerbal Space Program",
      .executable = "KSP.x86_64",
      .num_options = 1,
      .options = application_361_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86_64",
      .num_options = 1,
      .options = application_363_options,
    },
    { .name = "7 Days to Die",
      .executable = "7DaysToDie.x86",
      .num_options = 1,
      .options = application_365_options,
    },
    { .name = "SPECviewperf13",
      .executable = "viewperf",
      .num_options = 4,
      .options = application_367_options,
    },
    { .name = "Road Redemption",
      .executable = "RoadRedemption.x86_64",
      .num_options = 1,
      .options = application_372_options,
    },
    { .name = "Wasteland 2",
      .executable = "WL2",
      .num_options = 1,
      .options = application_374_options,
    },
    { .name = "Teardown",
      .executable = "teardown.exe",
      .num_options = 1,
      .options = application_376_options,
    },
    { .name = "SpaceEngine",
      .executable = "SpaceEngine.exe",
      .num_options = 1,
      .options = application_378_options,
    },
};

static const struct driconf_device device_330 = {
    .driver = "radeonsi",
    .num_engines = 0,
    .num_applications = 22,
    .applications = device_330_applications,
};


    
static const struct driconf_option application_381_options[] = {
    { .name = "vs_position_always_invariant", .value = "true" },
};


static const struct driconf_application device_380_applications[] = {
    { .name = "Middle Earth: Shadow of Mordor",
      .executable = "ShadowOfMordor",
      .num_options = 1,
      .options = application_381_options,
    },
};

static const struct driconf_device device_380 = {
    .driver = "i965",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_380_applications,
};


    
static const struct driconf_option application_384_options[] = {
    { .name = "vs_position_always_invariant", .value = "true" },
};


static const struct driconf_application device_383_applications[] = {
    { .name = "Middle Earth: Shadow of Mordor",
      .executable = "ShadowOfMordor",
      .num_options = 1,
      .options = application_384_options,
    },
};

static const struct driconf_device device_383 = {
    .driver = "iris",
    .num_engines = 0,
    .num_applications = 1,
    .applications = device_383_applications,
};


    
static const struct driconf_option application_387_options[] = {
    { .name = "radv_report_llvm9_version_string", .value = "true" },
    { .name = "radv_invariant_geom", .value = "true" },
};

    
static const struct driconf_option application_390_options[] = {
    { .name = "radv_invariant_geom", .value = "true" },
};

    
static const struct driconf_option application_392_options[] = {
    { .name = "radv_enable_mrt_output_nan_fixup", .value = "true" },
};

    
static const struct driconf_option application_394_options[] = {
    { .name = "radv_no_dynamic_bounds", .value = "true" },
    { .name = "radv_absolute_depth_bias", .value = "true" },
};

    
static const struct driconf_option application_397_options[] = {
    { .name = "radv_no_dynamic_bounds", .value = "true" },
    { .name = "radv_absolute_depth_bias", .value = "true" },
};

    
static const struct driconf_option application_400_options[] = {
    { .name = "radv_no_dynamic_bounds", .value = "true" },
    { .name = "radv_absolute_depth_bias", .value = "true" },
};

    
static const struct driconf_option application_403_options[] = {
    { .name = "radv_no_dynamic_bounds", .value = "true" },
    { .name = "radv_absolute_depth_bias", .value = "true" },
};

    
static const struct driconf_option application_406_options[] = {
    { .name = "radv_disable_shrink_image_store", .value = "true" },
};

    
static const struct driconf_option application_408_options[] = {
    { .name = "radv_override_uniform_offset_alignment", .value = "16" },
};

    
static const struct driconf_option application_410_options[] = {
    { .name = "radv_invariant_geom", .value = "true" },
};

    
static const struct driconf_option application_412_options[] = {
    { .name = "radv_disable_dcc", .value = "true" },
};

    
static const struct driconf_option application_414_options[] = {
    { .name = "radv_disable_dcc", .value = "true" },
};


static const struct driconf_application device_386_applications[] = {
    { .name = "Shadow Of The Tomb Raider (Native)",
      .application_name_match = "ShadowOfTheTomb",
      .num_options = 2,
      .options = application_387_options,
    },
    { .name = "Shadow Of The Tomb Raider (DX11/DX12)",
      .application_name_match = "SOTTR.exe",
      .num_options = 1,
      .options = application_390_options,
    },
    { .name = "RAGE 2",
      .executable = "RAGE2.exe",
      .num_options = 1,
      .options = application_392_options,
    },
    { .name = "Path of Exile (64-bit, Steam)",
      .executable = "PathOfExile_x64Steam.exe",
      .num_options = 2,
      .options = application_394_options,
    },
    { .name = "Path of Exile (32-bit, Steam)",
      .executable = "PathOfExileSteam.exe",
      .num_options = 2,
      .options = application_397_options,
    },
    { .name = "Path of Exile (64-bit)",
      .executable = "PathOfExile_x64.exe",
      .num_options = 2,
      .options = application_400_options,
    },
    { .name = "Path of Exile (32-bit)",
      .executable = "PathOfExile.exe",
      .num_options = 2,
      .options = application_403_options,
    },
    { .name = "The Surge 2",
      .application_name_match = "Fledge",
      .num_options = 1,
      .options = application_406_options,
    },
    { .name = "World War Z",
      .application_name_match = "WWZ",
      .num_options = 1,
      .options = application_408_options,
    },
    { .name = "Monster Hunter World",
      .application_name_match = "MonsterHunterWorld.exe",
      .num_options = 1,
      .options = application_410_options,
    },
    { .name = "DOOM (2016)",
      .application_name_match = "DOOM$",
      .num_options = 1,
      .options = application_412_options,
    },
    { .name = "Wolfenstein II",
      .application_name_match = "Wolfenstein II The New Colossus",
      .num_options = 1,
      .options = application_414_options,
    },
};

static const struct driconf_device device_386 = {
    .driver = "radv",
    .num_engines = 0,
    .num_applications = 12,
    .applications = device_386_applications,
};

static const struct driconf_device *driconf[] = {
    &device_1,
    &device_325,
    &device_330,
    &device_380,
    &device_383,
    &device_386,
};
