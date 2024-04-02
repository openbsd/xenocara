
/* Copyright © 2022 Collabora, Ltd.
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

/* This file generated from vk_format_info_gen.py, don't edit directly. */

#include "vk_format_info.h"

#include "util/macros.h"

#include "vk_format.h"

struct vk_format_info {
   enum vk_format_class class;
};

static const struct vk_format_info ext0_format_infos[] = {
   [1] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT,
   },
   [2] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [3] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [4] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [5] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [6] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [7] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [8] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [9] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT,
   },
   [10] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT,
   },
   [11] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT,
   },
   [12] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT,
   },
   [13] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT,
   },
   [14] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT,
   },
   [15] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT,
   },
   [16] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [17] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [18] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [19] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [20] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [21] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [22] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [23] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [24] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [25] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [26] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [27] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [28] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [29] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [30] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [31] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [32] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [33] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [34] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [35] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [36] = {
      .class = MESA_VK_FORMAT_CLASS_24_BIT,
   },
   [37] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [38] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [39] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [40] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [41] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [42] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [43] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [44] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [45] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [46] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [47] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [48] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [49] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [50] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [51] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [52] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [53] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [54] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [55] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [56] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [57] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [58] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [59] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [60] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [61] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [62] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [63] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [64] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [65] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [66] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [67] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [68] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [69] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [70] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [71] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [72] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [73] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [74] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [75] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [76] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [77] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [78] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [79] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [80] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [81] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [82] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [83] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [84] = {
      .class = MESA_VK_FORMAT_CLASS_48_BIT,
   },
   [85] = {
      .class = MESA_VK_FORMAT_CLASS_48_BIT,
   },
   [86] = {
      .class = MESA_VK_FORMAT_CLASS_48_BIT,
   },
   [87] = {
      .class = MESA_VK_FORMAT_CLASS_48_BIT,
   },
   [88] = {
      .class = MESA_VK_FORMAT_CLASS_48_BIT,
   },
   [89] = {
      .class = MESA_VK_FORMAT_CLASS_48_BIT,
   },
   [90] = {
      .class = MESA_VK_FORMAT_CLASS_48_BIT,
   },
   [91] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [92] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [93] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [94] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [95] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [96] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [97] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [98] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [99] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [100] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [101] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [102] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [103] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [104] = {
      .class = MESA_VK_FORMAT_CLASS_96_BIT,
   },
   [105] = {
      .class = MESA_VK_FORMAT_CLASS_96_BIT,
   },
   [106] = {
      .class = MESA_VK_FORMAT_CLASS_96_BIT,
   },
   [107] = {
      .class = MESA_VK_FORMAT_CLASS_128_BIT,
   },
   [108] = {
      .class = MESA_VK_FORMAT_CLASS_128_BIT,
   },
   [109] = {
      .class = MESA_VK_FORMAT_CLASS_128_BIT,
   },
   [110] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [111] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [112] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT,
   },
   [113] = {
      .class = MESA_VK_FORMAT_CLASS_128_BIT,
   },
   [114] = {
      .class = MESA_VK_FORMAT_CLASS_128_BIT,
   },
   [115] = {
      .class = MESA_VK_FORMAT_CLASS_128_BIT,
   },
   [116] = {
      .class = MESA_VK_FORMAT_CLASS_192_BIT,
   },
   [117] = {
      .class = MESA_VK_FORMAT_CLASS_192_BIT,
   },
   [118] = {
      .class = MESA_VK_FORMAT_CLASS_192_BIT,
   },
   [119] = {
      .class = MESA_VK_FORMAT_CLASS_256_BIT,
   },
   [120] = {
      .class = MESA_VK_FORMAT_CLASS_256_BIT,
   },
   [121] = {
      .class = MESA_VK_FORMAT_CLASS_256_BIT,
   },
   [122] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [123] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [124] = {
      .class = MESA_VK_FORMAT_CLASS_D16,
   },
   [125] = {
      .class = MESA_VK_FORMAT_CLASS_D24,
   },
   [126] = {
      .class = MESA_VK_FORMAT_CLASS_D32,
   },
   [127] = {
      .class = MESA_VK_FORMAT_CLASS_S8,
   },
   [128] = {
      .class = MESA_VK_FORMAT_CLASS_D16S8,
   },
   [129] = {
      .class = MESA_VK_FORMAT_CLASS_D24S8,
   },
   [130] = {
      .class = MESA_VK_FORMAT_CLASS_D32S8,
   },
   [131] = {
      .class = MESA_VK_FORMAT_CLASS_BC1_RGB,
   },
   [132] = {
      .class = MESA_VK_FORMAT_CLASS_BC1_RGB,
   },
   [133] = {
      .class = MESA_VK_FORMAT_CLASS_BC1_RGBA,
   },
   [134] = {
      .class = MESA_VK_FORMAT_CLASS_BC1_RGBA,
   },
   [135] = {
      .class = MESA_VK_FORMAT_CLASS_BC2,
   },
   [136] = {
      .class = MESA_VK_FORMAT_CLASS_BC2,
   },
   [137] = {
      .class = MESA_VK_FORMAT_CLASS_BC3,
   },
   [138] = {
      .class = MESA_VK_FORMAT_CLASS_BC3,
   },
   [139] = {
      .class = MESA_VK_FORMAT_CLASS_BC4,
   },
   [140] = {
      .class = MESA_VK_FORMAT_CLASS_BC4,
   },
   [141] = {
      .class = MESA_VK_FORMAT_CLASS_BC5,
   },
   [142] = {
      .class = MESA_VK_FORMAT_CLASS_BC5,
   },
   [143] = {
      .class = MESA_VK_FORMAT_CLASS_BC6H,
   },
   [144] = {
      .class = MESA_VK_FORMAT_CLASS_BC6H,
   },
   [145] = {
      .class = MESA_VK_FORMAT_CLASS_BC7,
   },
   [146] = {
      .class = MESA_VK_FORMAT_CLASS_BC7,
   },
   [147] = {
      .class = MESA_VK_FORMAT_CLASS_ETC2_RGB,
   },
   [148] = {
      .class = MESA_VK_FORMAT_CLASS_ETC2_RGB,
   },
   [149] = {
      .class = MESA_VK_FORMAT_CLASS_ETC2_RGBA,
   },
   [150] = {
      .class = MESA_VK_FORMAT_CLASS_ETC2_RGBA,
   },
   [151] = {
      .class = MESA_VK_FORMAT_CLASS_ETC2_EAC_RGBA,
   },
   [152] = {
      .class = MESA_VK_FORMAT_CLASS_ETC2_EAC_RGBA,
   },
   [153] = {
      .class = MESA_VK_FORMAT_CLASS_EAC_R,
   },
   [154] = {
      .class = MESA_VK_FORMAT_CLASS_EAC_R,
   },
   [155] = {
      .class = MESA_VK_FORMAT_CLASS_EAC_RG,
   },
   [156] = {
      .class = MESA_VK_FORMAT_CLASS_EAC_RG,
   },
   [157] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_4X4,
   },
   [158] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_4X4,
   },
   [159] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_5X4,
   },
   [160] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_5X4,
   },
   [161] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_5X5,
   },
   [162] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_5X5,
   },
   [163] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_6X5,
   },
   [164] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_6X5,
   },
   [165] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_6X6,
   },
   [166] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_6X6,
   },
   [167] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_8X5,
   },
   [168] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_8X5,
   },
   [169] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_8X6,
   },
   [170] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_8X6,
   },
   [171] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_8X8,
   },
   [172] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_8X8,
   },
   [173] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X5,
   },
   [174] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X5,
   },
   [175] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X6,
   },
   [176] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X6,
   },
   [177] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X8,
   },
   [178] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X8,
   },
   [179] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X10,
   },
   [180] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X10,
   },
   [181] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_12X10,
   },
   [182] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_12X10,
   },
   [183] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_12X12,
   },
   [184] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_12X12,
   },
};

static const struct vk_format_info ext471_format_infos[] = {
   [0] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [1] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT_ALPHA,
   },
};

static const struct vk_format_info ext157_format_infos[] = {
   [0] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT_G8B8G8R8,
   },
   [1] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT_B8G8R8G8,
   },
   [2] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT_3_PLANE_420,
   },
   [3] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT_2_PLANE_420,
   },
   [4] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT_3_PLANE_422,
   },
   [5] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT_2_PLANE_422,
   },
   [6] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT_3_PLANE_444,
   },
   [7] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [8] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [9] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT_R10G10B10A10,
   },
   [10] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT_G10B10G10R10,
   },
   [11] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT_B10G10R10G10,
   },
   [12] = {
      .class = MESA_VK_FORMAT_CLASS_10_BIT_3_PLANE_420,
   },
   [13] = {
      .class = MESA_VK_FORMAT_CLASS_10_BIT_2_PLANE_420,
   },
   [14] = {
      .class = MESA_VK_FORMAT_CLASS_10_BIT_3_PLANE_422,
   },
   [15] = {
      .class = MESA_VK_FORMAT_CLASS_10_BIT_2_PLANE_422,
   },
   [16] = {
      .class = MESA_VK_FORMAT_CLASS_10_BIT_3_PLANE_444,
   },
   [17] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [18] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
   [19] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT_R12G12B12A12,
   },
   [20] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT_G12B12G12R12,
   },
   [21] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT_B12G12R12G12,
   },
   [22] = {
      .class = MESA_VK_FORMAT_CLASS_12_BIT_3_PLANE_420,
   },
   [23] = {
      .class = MESA_VK_FORMAT_CLASS_12_BIT_2_PLANE_420,
   },
   [24] = {
      .class = MESA_VK_FORMAT_CLASS_12_BIT_3_PLANE_422,
   },
   [25] = {
      .class = MESA_VK_FORMAT_CLASS_12_BIT_2_PLANE_422,
   },
   [26] = {
      .class = MESA_VK_FORMAT_CLASS_12_BIT_3_PLANE_444,
   },
   [27] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT_G16B16G16R16,
   },
   [28] = {
      .class = MESA_VK_FORMAT_CLASS_64_BIT_B16G16R16G16,
   },
   [29] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT_3_PLANE_420,
   },
   [30] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT_2_PLANE_420,
   },
   [31] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT_3_PLANE_422,
   },
   [32] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT_2_PLANE_422,
   },
   [33] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT_3_PLANE_444,
   },
};

static const struct vk_format_info ext55_format_infos[] = {
   [0] = {
      .class = MESA_VK_FORMAT_CLASS_PVRTC1_2BPP,
   },
   [1] = {
      .class = MESA_VK_FORMAT_CLASS_PVRTC1_4BPP,
   },
   [2] = {
      .class = MESA_VK_FORMAT_CLASS_PVRTC2_2BPP,
   },
   [3] = {
      .class = MESA_VK_FORMAT_CLASS_PVRTC2_4BPP,
   },
   [4] = {
      .class = MESA_VK_FORMAT_CLASS_PVRTC1_2BPP,
   },
   [5] = {
      .class = MESA_VK_FORMAT_CLASS_PVRTC1_4BPP,
   },
   [6] = {
      .class = MESA_VK_FORMAT_CLASS_PVRTC2_2BPP,
   },
   [7] = {
      .class = MESA_VK_FORMAT_CLASS_PVRTC2_4BPP,
   },
};

static const struct vk_format_info ext67_format_infos[] = {
   [0] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_4X4,
   },
   [1] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_5X4,
   },
   [2] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_5X5,
   },
   [3] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_6X5,
   },
   [4] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_6X6,
   },
   [5] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_8X5,
   },
   [6] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_8X6,
   },
   [7] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_8X8,
   },
   [8] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X5,
   },
   [9] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X6,
   },
   [10] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X8,
   },
   [11] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_10X10,
   },
   [12] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_12X10,
   },
   [13] = {
      .class = MESA_VK_FORMAT_CLASS_ASTC_12X12,
   },
};

static const struct vk_format_info ext331_format_infos[] = {
   [0] = {
      .class = MESA_VK_FORMAT_CLASS_8_BIT_2_PLANE_444,
   },
   [1] = {
      .class = MESA_VK_FORMAT_CLASS_10_BIT_2_PLANE_444,
   },
   [2] = {
      .class = MESA_VK_FORMAT_CLASS_12_BIT_2_PLANE_444,
   },
   [3] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT_2_PLANE_444,
   },
};

static const struct vk_format_info ext341_format_infos[] = {
   [0] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
   [1] = {
      .class = MESA_VK_FORMAT_CLASS_16_BIT,
   },
};

static const struct vk_format_info ext465_format_infos[] = {
   [0] = {
      .class = MESA_VK_FORMAT_CLASS_32_BIT,
   },
};

static const struct vk_format_info *
vk_format_get_info(VkFormat format)
{
   uint32_t extnumber =
      format < 1000000000 ? 0 : (((format % 1000000000) / 1000) + 1);
   uint32_t offset = format % 1000;

   switch (extnumber) {
   case 0:
      assert(offset < ARRAY_SIZE(ext0_format_infos));
      return &ext0_format_infos[offset];
   case 471:
      assert(offset < ARRAY_SIZE(ext471_format_infos));
      return &ext471_format_infos[offset];
   case 157:
      assert(offset < ARRAY_SIZE(ext157_format_infos));
      return &ext157_format_infos[offset];
   case 55:
      assert(offset < ARRAY_SIZE(ext55_format_infos));
      return &ext55_format_infos[offset];
   case 67:
      assert(offset < ARRAY_SIZE(ext67_format_infos));
      return &ext67_format_infos[offset];
   case 331:
      assert(offset < ARRAY_SIZE(ext331_format_infos));
      return &ext331_format_infos[offset];
   case 341:
      assert(offset < ARRAY_SIZE(ext341_format_infos));
      return &ext341_format_infos[offset];
   case 465:
      assert(offset < ARRAY_SIZE(ext465_format_infos));
      return &ext465_format_infos[offset];
   default:
      unreachable("Invalid extension");
   }
}

static const VkFormat mesa_vk_format_class_8_bit_formats[] = {
   VK_FORMAT_R4G4_UNORM_PACK8,
   VK_FORMAT_R8_UNORM,
   VK_FORMAT_R8_SNORM,
   VK_FORMAT_R8_USCALED,
   VK_FORMAT_R8_SSCALED,
   VK_FORMAT_R8_UINT,
   VK_FORMAT_R8_SINT,
   VK_FORMAT_R8_SRGB,
};

static const VkFormat mesa_vk_format_class_16_bit_formats[] = {
   VK_FORMAT_R4G4B4A4_UNORM_PACK16,
   VK_FORMAT_B4G4R4A4_UNORM_PACK16,
   VK_FORMAT_R5G6B5_UNORM_PACK16,
   VK_FORMAT_B5G6R5_UNORM_PACK16,
   VK_FORMAT_R5G5B5A1_UNORM_PACK16,
   VK_FORMAT_B5G5R5A1_UNORM_PACK16,
   VK_FORMAT_A1R5G5B5_UNORM_PACK16,
   VK_FORMAT_A1B5G5R5_UNORM_PACK16_KHR,
   VK_FORMAT_R8G8_UNORM,
   VK_FORMAT_R8G8_SNORM,
   VK_FORMAT_R8G8_USCALED,
   VK_FORMAT_R8G8_SSCALED,
   VK_FORMAT_R8G8_UINT,
   VK_FORMAT_R8G8_SINT,
   VK_FORMAT_R8G8_SRGB,
   VK_FORMAT_R16_UNORM,
   VK_FORMAT_R16_SNORM,
   VK_FORMAT_R16_USCALED,
   VK_FORMAT_R16_SSCALED,
   VK_FORMAT_R16_UINT,
   VK_FORMAT_R16_SINT,
   VK_FORMAT_R16_SFLOAT,
   VK_FORMAT_R10X6_UNORM_PACK16,
   VK_FORMAT_R12X4_UNORM_PACK16,
   VK_FORMAT_A4R4G4B4_UNORM_PACK16,
   VK_FORMAT_A4B4G4R4_UNORM_PACK16,
};

static const VkFormat mesa_vk_format_class_8_bit_alpha_formats[] = {
   VK_FORMAT_A8_UNORM_KHR,
};

static const VkFormat mesa_vk_format_class_24_bit_formats[] = {
   VK_FORMAT_R8G8B8_UNORM,
   VK_FORMAT_R8G8B8_SNORM,
   VK_FORMAT_R8G8B8_USCALED,
   VK_FORMAT_R8G8B8_SSCALED,
   VK_FORMAT_R8G8B8_UINT,
   VK_FORMAT_R8G8B8_SINT,
   VK_FORMAT_R8G8B8_SRGB,
   VK_FORMAT_B8G8R8_UNORM,
   VK_FORMAT_B8G8R8_SNORM,
   VK_FORMAT_B8G8R8_USCALED,
   VK_FORMAT_B8G8R8_SSCALED,
   VK_FORMAT_B8G8R8_UINT,
   VK_FORMAT_B8G8R8_SINT,
   VK_FORMAT_B8G8R8_SRGB,
};

static const VkFormat mesa_vk_format_class_32_bit_formats[] = {
   VK_FORMAT_R8G8B8A8_UNORM,
   VK_FORMAT_R8G8B8A8_SNORM,
   VK_FORMAT_R8G8B8A8_USCALED,
   VK_FORMAT_R8G8B8A8_SSCALED,
   VK_FORMAT_R8G8B8A8_UINT,
   VK_FORMAT_R8G8B8A8_SINT,
   VK_FORMAT_R8G8B8A8_SRGB,
   VK_FORMAT_B8G8R8A8_UNORM,
   VK_FORMAT_B8G8R8A8_SNORM,
   VK_FORMAT_B8G8R8A8_USCALED,
   VK_FORMAT_B8G8R8A8_SSCALED,
   VK_FORMAT_B8G8R8A8_UINT,
   VK_FORMAT_B8G8R8A8_SINT,
   VK_FORMAT_B8G8R8A8_SRGB,
   VK_FORMAT_A8B8G8R8_UNORM_PACK32,
   VK_FORMAT_A8B8G8R8_SNORM_PACK32,
   VK_FORMAT_A8B8G8R8_USCALED_PACK32,
   VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
   VK_FORMAT_A8B8G8R8_UINT_PACK32,
   VK_FORMAT_A8B8G8R8_SINT_PACK32,
   VK_FORMAT_A8B8G8R8_SRGB_PACK32,
   VK_FORMAT_A2R10G10B10_UNORM_PACK32,
   VK_FORMAT_A2R10G10B10_SNORM_PACK32,
   VK_FORMAT_A2R10G10B10_USCALED_PACK32,
   VK_FORMAT_A2R10G10B10_SSCALED_PACK32,
   VK_FORMAT_A2R10G10B10_UINT_PACK32,
   VK_FORMAT_A2R10G10B10_SINT_PACK32,
   VK_FORMAT_A2B10G10R10_UNORM_PACK32,
   VK_FORMAT_A2B10G10R10_SNORM_PACK32,
   VK_FORMAT_A2B10G10R10_USCALED_PACK32,
   VK_FORMAT_A2B10G10R10_SSCALED_PACK32,
   VK_FORMAT_A2B10G10R10_UINT_PACK32,
   VK_FORMAT_A2B10G10R10_SINT_PACK32,
   VK_FORMAT_R16G16_UNORM,
   VK_FORMAT_R16G16_SNORM,
   VK_FORMAT_R16G16_USCALED,
   VK_FORMAT_R16G16_SSCALED,
   VK_FORMAT_R16G16_UINT,
   VK_FORMAT_R16G16_SINT,
   VK_FORMAT_R16G16_SFLOAT,
   VK_FORMAT_R32_UINT,
   VK_FORMAT_R32_SINT,
   VK_FORMAT_R32_SFLOAT,
   VK_FORMAT_B10G11R11_UFLOAT_PACK32,
   VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
   VK_FORMAT_R10X6G10X6_UNORM_2PACK16,
   VK_FORMAT_R12X4G12X4_UNORM_2PACK16,
   VK_FORMAT_R16G16_S10_5_NV,
};

static const VkFormat mesa_vk_format_class_48_bit_formats[] = {
   VK_FORMAT_R16G16B16_UNORM,
   VK_FORMAT_R16G16B16_SNORM,
   VK_FORMAT_R16G16B16_USCALED,
   VK_FORMAT_R16G16B16_SSCALED,
   VK_FORMAT_R16G16B16_UINT,
   VK_FORMAT_R16G16B16_SINT,
   VK_FORMAT_R16G16B16_SFLOAT,
};

static const VkFormat mesa_vk_format_class_64_bit_formats[] = {
   VK_FORMAT_R16G16B16A16_UNORM,
   VK_FORMAT_R16G16B16A16_SNORM,
   VK_FORMAT_R16G16B16A16_USCALED,
   VK_FORMAT_R16G16B16A16_SSCALED,
   VK_FORMAT_R16G16B16A16_UINT,
   VK_FORMAT_R16G16B16A16_SINT,
   VK_FORMAT_R16G16B16A16_SFLOAT,
   VK_FORMAT_R32G32_UINT,
   VK_FORMAT_R32G32_SINT,
   VK_FORMAT_R32G32_SFLOAT,
   VK_FORMAT_R64_UINT,
   VK_FORMAT_R64_SINT,
   VK_FORMAT_R64_SFLOAT,
};

static const VkFormat mesa_vk_format_class_96_bit_formats[] = {
   VK_FORMAT_R32G32B32_UINT,
   VK_FORMAT_R32G32B32_SINT,
   VK_FORMAT_R32G32B32_SFLOAT,
};

static const VkFormat mesa_vk_format_class_128_bit_formats[] = {
   VK_FORMAT_R32G32B32A32_UINT,
   VK_FORMAT_R32G32B32A32_SINT,
   VK_FORMAT_R32G32B32A32_SFLOAT,
   VK_FORMAT_R64G64_UINT,
   VK_FORMAT_R64G64_SINT,
   VK_FORMAT_R64G64_SFLOAT,
};

static const VkFormat mesa_vk_format_class_192_bit_formats[] = {
   VK_FORMAT_R64G64B64_UINT,
   VK_FORMAT_R64G64B64_SINT,
   VK_FORMAT_R64G64B64_SFLOAT,
};

static const VkFormat mesa_vk_format_class_256_bit_formats[] = {
   VK_FORMAT_R64G64B64A64_UINT,
   VK_FORMAT_R64G64B64A64_SINT,
   VK_FORMAT_R64G64B64A64_SFLOAT,
};

static const VkFormat mesa_vk_format_class_d16_formats[] = {
   VK_FORMAT_D16_UNORM,
};

static const VkFormat mesa_vk_format_class_d24_formats[] = {
   VK_FORMAT_X8_D24_UNORM_PACK32,
};

static const VkFormat mesa_vk_format_class_d32_formats[] = {
   VK_FORMAT_D32_SFLOAT,
};

static const VkFormat mesa_vk_format_class_s8_formats[] = {
   VK_FORMAT_S8_UINT,
};

static const VkFormat mesa_vk_format_class_d16s8_formats[] = {
   VK_FORMAT_D16_UNORM_S8_UINT,
};

static const VkFormat mesa_vk_format_class_d24s8_formats[] = {
   VK_FORMAT_D24_UNORM_S8_UINT,
};

static const VkFormat mesa_vk_format_class_d32s8_formats[] = {
   VK_FORMAT_D32_SFLOAT_S8_UINT,
};

static const VkFormat mesa_vk_format_class_bc1_rgb_formats[] = {
   VK_FORMAT_BC1_RGB_UNORM_BLOCK,
   VK_FORMAT_BC1_RGB_SRGB_BLOCK,
};

static const VkFormat mesa_vk_format_class_bc1_rgba_formats[] = {
   VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
   VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
};

static const VkFormat mesa_vk_format_class_bc2_formats[] = {
   VK_FORMAT_BC2_UNORM_BLOCK,
   VK_FORMAT_BC2_SRGB_BLOCK,
};

static const VkFormat mesa_vk_format_class_bc3_formats[] = {
   VK_FORMAT_BC3_UNORM_BLOCK,
   VK_FORMAT_BC3_SRGB_BLOCK,
};

static const VkFormat mesa_vk_format_class_bc4_formats[] = {
   VK_FORMAT_BC4_UNORM_BLOCK,
   VK_FORMAT_BC4_SNORM_BLOCK,
};

static const VkFormat mesa_vk_format_class_bc5_formats[] = {
   VK_FORMAT_BC5_UNORM_BLOCK,
   VK_FORMAT_BC5_SNORM_BLOCK,
};

static const VkFormat mesa_vk_format_class_bc6h_formats[] = {
   VK_FORMAT_BC6H_UFLOAT_BLOCK,
   VK_FORMAT_BC6H_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_bc7_formats[] = {
   VK_FORMAT_BC7_UNORM_BLOCK,
   VK_FORMAT_BC7_SRGB_BLOCK,
};

static const VkFormat mesa_vk_format_class_etc2_rgb_formats[] = {
   VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,
   VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,
};

static const VkFormat mesa_vk_format_class_etc2_rgba_formats[] = {
   VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
   VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,
};

static const VkFormat mesa_vk_format_class_etc2_eac_rgba_formats[] = {
   VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,
   VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,
};

static const VkFormat mesa_vk_format_class_eac_r_formats[] = {
   VK_FORMAT_EAC_R11_UNORM_BLOCK,
   VK_FORMAT_EAC_R11_SNORM_BLOCK,
};

static const VkFormat mesa_vk_format_class_eac_rg_formats[] = {
   VK_FORMAT_EAC_R11G11_UNORM_BLOCK,
   VK_FORMAT_EAC_R11G11_SNORM_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_4x4_formats[] = {
   VK_FORMAT_ASTC_4x4_UNORM_BLOCK,
   VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
   VK_FORMAT_ASTC_4x4_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_5x4_formats[] = {
   VK_FORMAT_ASTC_5x4_UNORM_BLOCK,
   VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
   VK_FORMAT_ASTC_5x4_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_5x5_formats[] = {
   VK_FORMAT_ASTC_5x5_UNORM_BLOCK,
   VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
   VK_FORMAT_ASTC_5x5_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_6x5_formats[] = {
   VK_FORMAT_ASTC_6x5_UNORM_BLOCK,
   VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
   VK_FORMAT_ASTC_6x5_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_6x6_formats[] = {
   VK_FORMAT_ASTC_6x6_UNORM_BLOCK,
   VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
   VK_FORMAT_ASTC_6x6_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_8x5_formats[] = {
   VK_FORMAT_ASTC_8x5_UNORM_BLOCK,
   VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
   VK_FORMAT_ASTC_8x5_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_8x6_formats[] = {
   VK_FORMAT_ASTC_8x6_UNORM_BLOCK,
   VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
   VK_FORMAT_ASTC_8x6_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_8x8_formats[] = {
   VK_FORMAT_ASTC_8x8_UNORM_BLOCK,
   VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
   VK_FORMAT_ASTC_8x8_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_10x5_formats[] = {
   VK_FORMAT_ASTC_10x5_UNORM_BLOCK,
   VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
   VK_FORMAT_ASTC_10x5_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_10x6_formats[] = {
   VK_FORMAT_ASTC_10x6_UNORM_BLOCK,
   VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
   VK_FORMAT_ASTC_10x6_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_10x8_formats[] = {
   VK_FORMAT_ASTC_10x8_UNORM_BLOCK,
   VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
   VK_FORMAT_ASTC_10x8_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_10x10_formats[] = {
   VK_FORMAT_ASTC_10x10_UNORM_BLOCK,
   VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
   VK_FORMAT_ASTC_10x10_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_12x10_formats[] = {
   VK_FORMAT_ASTC_12x10_UNORM_BLOCK,
   VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
   VK_FORMAT_ASTC_12x10_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_astc_12x12_formats[] = {
   VK_FORMAT_ASTC_12x12_UNORM_BLOCK,
   VK_FORMAT_ASTC_12x12_SRGB_BLOCK,
   VK_FORMAT_ASTC_12x12_SFLOAT_BLOCK,
};

static const VkFormat mesa_vk_format_class_32_bit_g8b8g8r8_formats[] = {
   VK_FORMAT_G8B8G8R8_422_UNORM,
};

static const VkFormat mesa_vk_format_class_32_bit_b8g8r8g8_formats[] = {
   VK_FORMAT_B8G8R8G8_422_UNORM,
};

static const VkFormat mesa_vk_format_class_8_bit_3_plane_420_formats[] = {
   VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM,
};

static const VkFormat mesa_vk_format_class_8_bit_2_plane_420_formats[] = {
   VK_FORMAT_G8_B8R8_2PLANE_420_UNORM,
};

static const VkFormat mesa_vk_format_class_8_bit_3_plane_422_formats[] = {
   VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM,
};

static const VkFormat mesa_vk_format_class_8_bit_2_plane_422_formats[] = {
   VK_FORMAT_G8_B8R8_2PLANE_422_UNORM,
};

static const VkFormat mesa_vk_format_class_8_bit_3_plane_444_formats[] = {
   VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM,
};

static const VkFormat mesa_vk_format_class_64_bit_r10g10b10a10_formats[] = {
   VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16,
};

static const VkFormat mesa_vk_format_class_64_bit_g10b10g10r10_formats[] = {
   VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16,
};

static const VkFormat mesa_vk_format_class_64_bit_b10g10r10g10_formats[] = {
   VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16,
};

static const VkFormat mesa_vk_format_class_10_bit_3_plane_420_formats[] = {
   VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_10_bit_2_plane_420_formats[] = {
   VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_10_bit_3_plane_422_formats[] = {
   VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_10_bit_2_plane_422_formats[] = {
   VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_10_bit_3_plane_444_formats[] = {
   VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_64_bit_r12g12b12a12_formats[] = {
   VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16,
};

static const VkFormat mesa_vk_format_class_64_bit_g12b12g12r12_formats[] = {
   VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16,
};

static const VkFormat mesa_vk_format_class_64_bit_b12g12r12g12_formats[] = {
   VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16,
};

static const VkFormat mesa_vk_format_class_12_bit_3_plane_420_formats[] = {
   VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_12_bit_2_plane_420_formats[] = {
   VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_12_bit_3_plane_422_formats[] = {
   VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_12_bit_2_plane_422_formats[] = {
   VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_12_bit_3_plane_444_formats[] = {
   VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_64_bit_g16b16g16r16_formats[] = {
   VK_FORMAT_G16B16G16R16_422_UNORM,
};

static const VkFormat mesa_vk_format_class_64_bit_b16g16r16g16_formats[] = {
   VK_FORMAT_B16G16R16G16_422_UNORM,
};

static const VkFormat mesa_vk_format_class_16_bit_3_plane_420_formats[] = {
   VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM,
};

static const VkFormat mesa_vk_format_class_16_bit_2_plane_420_formats[] = {
   VK_FORMAT_G16_B16R16_2PLANE_420_UNORM,
};

static const VkFormat mesa_vk_format_class_16_bit_3_plane_422_formats[] = {
   VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM,
};

static const VkFormat mesa_vk_format_class_16_bit_2_plane_422_formats[] = {
   VK_FORMAT_G16_B16R16_2PLANE_422_UNORM,
};

static const VkFormat mesa_vk_format_class_16_bit_3_plane_444_formats[] = {
   VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM,
};

static const VkFormat mesa_vk_format_class_pvrtc1_2bpp_formats[] = {
   VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG,
   VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG,
};

static const VkFormat mesa_vk_format_class_pvrtc1_4bpp_formats[] = {
   VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG,
   VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG,
};

static const VkFormat mesa_vk_format_class_pvrtc2_2bpp_formats[] = {
   VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG,
   VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG,
};

static const VkFormat mesa_vk_format_class_pvrtc2_4bpp_formats[] = {
   VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG,
   VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG,
};

static const VkFormat mesa_vk_format_class_8_bit_2_plane_444_formats[] = {
   VK_FORMAT_G8_B8R8_2PLANE_444_UNORM,
};

static const VkFormat mesa_vk_format_class_10_bit_2_plane_444_formats[] = {
   VK_FORMAT_G10X6_B10X6R10X6_2PLANE_444_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_12_bit_2_plane_444_formats[] = {
   VK_FORMAT_G12X4_B12X4R12X4_2PLANE_444_UNORM_3PACK16,
};

static const VkFormat mesa_vk_format_class_16_bit_2_plane_444_formats[] = {
   VK_FORMAT_G16_B16R16_2PLANE_444_UNORM,
};

static const struct vk_format_class_info class_infos[] = {
   [MESA_VK_FORMAT_CLASS_8_BIT] = {
      .formats = mesa_vk_format_class_8_bit_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_8_bit_formats),
   },
   [MESA_VK_FORMAT_CLASS_16_BIT] = {
      .formats = mesa_vk_format_class_16_bit_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_16_bit_formats),
   },
   [MESA_VK_FORMAT_CLASS_8_BIT_ALPHA] = {
      .formats = mesa_vk_format_class_8_bit_alpha_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_8_bit_alpha_formats),
   },
   [MESA_VK_FORMAT_CLASS_24_BIT] = {
      .formats = mesa_vk_format_class_24_bit_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_24_bit_formats),
   },
   [MESA_VK_FORMAT_CLASS_32_BIT] = {
      .formats = mesa_vk_format_class_32_bit_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_32_bit_formats),
   },
   [MESA_VK_FORMAT_CLASS_48_BIT] = {
      .formats = mesa_vk_format_class_48_bit_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_48_bit_formats),
   },
   [MESA_VK_FORMAT_CLASS_64_BIT] = {
      .formats = mesa_vk_format_class_64_bit_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_64_bit_formats),
   },
   [MESA_VK_FORMAT_CLASS_96_BIT] = {
      .formats = mesa_vk_format_class_96_bit_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_96_bit_formats),
   },
   [MESA_VK_FORMAT_CLASS_128_BIT] = {
      .formats = mesa_vk_format_class_128_bit_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_128_bit_formats),
   },
   [MESA_VK_FORMAT_CLASS_192_BIT] = {
      .formats = mesa_vk_format_class_192_bit_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_192_bit_formats),
   },
   [MESA_VK_FORMAT_CLASS_256_BIT] = {
      .formats = mesa_vk_format_class_256_bit_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_256_bit_formats),
   },
   [MESA_VK_FORMAT_CLASS_D16] = {
      .formats = mesa_vk_format_class_d16_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_d16_formats),
   },
   [MESA_VK_FORMAT_CLASS_D24] = {
      .formats = mesa_vk_format_class_d24_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_d24_formats),
   },
   [MESA_VK_FORMAT_CLASS_D32] = {
      .formats = mesa_vk_format_class_d32_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_d32_formats),
   },
   [MESA_VK_FORMAT_CLASS_S8] = {
      .formats = mesa_vk_format_class_s8_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_s8_formats),
   },
   [MESA_VK_FORMAT_CLASS_D16S8] = {
      .formats = mesa_vk_format_class_d16s8_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_d16s8_formats),
   },
   [MESA_VK_FORMAT_CLASS_D24S8] = {
      .formats = mesa_vk_format_class_d24s8_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_d24s8_formats),
   },
   [MESA_VK_FORMAT_CLASS_D32S8] = {
      .formats = mesa_vk_format_class_d32s8_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_d32s8_formats),
   },
   [MESA_VK_FORMAT_CLASS_BC1_RGB] = {
      .formats = mesa_vk_format_class_bc1_rgb_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_bc1_rgb_formats),
   },
   [MESA_VK_FORMAT_CLASS_BC1_RGBA] = {
      .formats = mesa_vk_format_class_bc1_rgba_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_bc1_rgba_formats),
   },
   [MESA_VK_FORMAT_CLASS_BC2] = {
      .formats = mesa_vk_format_class_bc2_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_bc2_formats),
   },
   [MESA_VK_FORMAT_CLASS_BC3] = {
      .formats = mesa_vk_format_class_bc3_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_bc3_formats),
   },
   [MESA_VK_FORMAT_CLASS_BC4] = {
      .formats = mesa_vk_format_class_bc4_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_bc4_formats),
   },
   [MESA_VK_FORMAT_CLASS_BC5] = {
      .formats = mesa_vk_format_class_bc5_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_bc5_formats),
   },
   [MESA_VK_FORMAT_CLASS_BC6H] = {
      .formats = mesa_vk_format_class_bc6h_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_bc6h_formats),
   },
   [MESA_VK_FORMAT_CLASS_BC7] = {
      .formats = mesa_vk_format_class_bc7_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_bc7_formats),
   },
   [MESA_VK_FORMAT_CLASS_ETC2_RGB] = {
      .formats = mesa_vk_format_class_etc2_rgb_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_etc2_rgb_formats),
   },
   [MESA_VK_FORMAT_CLASS_ETC2_RGBA] = {
      .formats = mesa_vk_format_class_etc2_rgba_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_etc2_rgba_formats),
   },
   [MESA_VK_FORMAT_CLASS_ETC2_EAC_RGBA] = {
      .formats = mesa_vk_format_class_etc2_eac_rgba_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_etc2_eac_rgba_formats),
   },
   [MESA_VK_FORMAT_CLASS_EAC_R] = {
      .formats = mesa_vk_format_class_eac_r_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_eac_r_formats),
   },
   [MESA_VK_FORMAT_CLASS_EAC_RG] = {
      .formats = mesa_vk_format_class_eac_rg_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_eac_rg_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_4X4] = {
      .formats = mesa_vk_format_class_astc_4x4_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_4x4_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_5X4] = {
      .formats = mesa_vk_format_class_astc_5x4_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_5x4_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_5X5] = {
      .formats = mesa_vk_format_class_astc_5x5_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_5x5_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_6X5] = {
      .formats = mesa_vk_format_class_astc_6x5_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_6x5_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_6X6] = {
      .formats = mesa_vk_format_class_astc_6x6_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_6x6_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_8X5] = {
      .formats = mesa_vk_format_class_astc_8x5_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_8x5_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_8X6] = {
      .formats = mesa_vk_format_class_astc_8x6_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_8x6_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_8X8] = {
      .formats = mesa_vk_format_class_astc_8x8_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_8x8_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_10X5] = {
      .formats = mesa_vk_format_class_astc_10x5_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_10x5_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_10X6] = {
      .formats = mesa_vk_format_class_astc_10x6_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_10x6_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_10X8] = {
      .formats = mesa_vk_format_class_astc_10x8_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_10x8_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_10X10] = {
      .formats = mesa_vk_format_class_astc_10x10_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_10x10_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_12X10] = {
      .formats = mesa_vk_format_class_astc_12x10_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_12x10_formats),
   },
   [MESA_VK_FORMAT_CLASS_ASTC_12X12] = {
      .formats = mesa_vk_format_class_astc_12x12_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_astc_12x12_formats),
   },
   [MESA_VK_FORMAT_CLASS_32_BIT_G8B8G8R8] = {
      .formats = mesa_vk_format_class_32_bit_g8b8g8r8_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_32_bit_g8b8g8r8_formats),
   },
   [MESA_VK_FORMAT_CLASS_32_BIT_B8G8R8G8] = {
      .formats = mesa_vk_format_class_32_bit_b8g8r8g8_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_32_bit_b8g8r8g8_formats),
   },
   [MESA_VK_FORMAT_CLASS_8_BIT_3_PLANE_420] = {
      .formats = mesa_vk_format_class_8_bit_3_plane_420_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_8_bit_3_plane_420_formats),
   },
   [MESA_VK_FORMAT_CLASS_8_BIT_2_PLANE_420] = {
      .formats = mesa_vk_format_class_8_bit_2_plane_420_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_8_bit_2_plane_420_formats),
   },
   [MESA_VK_FORMAT_CLASS_8_BIT_3_PLANE_422] = {
      .formats = mesa_vk_format_class_8_bit_3_plane_422_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_8_bit_3_plane_422_formats),
   },
   [MESA_VK_FORMAT_CLASS_8_BIT_2_PLANE_422] = {
      .formats = mesa_vk_format_class_8_bit_2_plane_422_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_8_bit_2_plane_422_formats),
   },
   [MESA_VK_FORMAT_CLASS_8_BIT_3_PLANE_444] = {
      .formats = mesa_vk_format_class_8_bit_3_plane_444_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_8_bit_3_plane_444_formats),
   },
   [MESA_VK_FORMAT_CLASS_64_BIT_R10G10B10A10] = {
      .formats = mesa_vk_format_class_64_bit_r10g10b10a10_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_64_bit_r10g10b10a10_formats),
   },
   [MESA_VK_FORMAT_CLASS_64_BIT_G10B10G10R10] = {
      .formats = mesa_vk_format_class_64_bit_g10b10g10r10_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_64_bit_g10b10g10r10_formats),
   },
   [MESA_VK_FORMAT_CLASS_64_BIT_B10G10R10G10] = {
      .formats = mesa_vk_format_class_64_bit_b10g10r10g10_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_64_bit_b10g10r10g10_formats),
   },
   [MESA_VK_FORMAT_CLASS_10_BIT_3_PLANE_420] = {
      .formats = mesa_vk_format_class_10_bit_3_plane_420_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_10_bit_3_plane_420_formats),
   },
   [MESA_VK_FORMAT_CLASS_10_BIT_2_PLANE_420] = {
      .formats = mesa_vk_format_class_10_bit_2_plane_420_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_10_bit_2_plane_420_formats),
   },
   [MESA_VK_FORMAT_CLASS_10_BIT_3_PLANE_422] = {
      .formats = mesa_vk_format_class_10_bit_3_plane_422_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_10_bit_3_plane_422_formats),
   },
   [MESA_VK_FORMAT_CLASS_10_BIT_2_PLANE_422] = {
      .formats = mesa_vk_format_class_10_bit_2_plane_422_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_10_bit_2_plane_422_formats),
   },
   [MESA_VK_FORMAT_CLASS_10_BIT_3_PLANE_444] = {
      .formats = mesa_vk_format_class_10_bit_3_plane_444_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_10_bit_3_plane_444_formats),
   },
   [MESA_VK_FORMAT_CLASS_64_BIT_R12G12B12A12] = {
      .formats = mesa_vk_format_class_64_bit_r12g12b12a12_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_64_bit_r12g12b12a12_formats),
   },
   [MESA_VK_FORMAT_CLASS_64_BIT_G12B12G12R12] = {
      .formats = mesa_vk_format_class_64_bit_g12b12g12r12_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_64_bit_g12b12g12r12_formats),
   },
   [MESA_VK_FORMAT_CLASS_64_BIT_B12G12R12G12] = {
      .formats = mesa_vk_format_class_64_bit_b12g12r12g12_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_64_bit_b12g12r12g12_formats),
   },
   [MESA_VK_FORMAT_CLASS_12_BIT_3_PLANE_420] = {
      .formats = mesa_vk_format_class_12_bit_3_plane_420_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_12_bit_3_plane_420_formats),
   },
   [MESA_VK_FORMAT_CLASS_12_BIT_2_PLANE_420] = {
      .formats = mesa_vk_format_class_12_bit_2_plane_420_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_12_bit_2_plane_420_formats),
   },
   [MESA_VK_FORMAT_CLASS_12_BIT_3_PLANE_422] = {
      .formats = mesa_vk_format_class_12_bit_3_plane_422_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_12_bit_3_plane_422_formats),
   },
   [MESA_VK_FORMAT_CLASS_12_BIT_2_PLANE_422] = {
      .formats = mesa_vk_format_class_12_bit_2_plane_422_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_12_bit_2_plane_422_formats),
   },
   [MESA_VK_FORMAT_CLASS_12_BIT_3_PLANE_444] = {
      .formats = mesa_vk_format_class_12_bit_3_plane_444_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_12_bit_3_plane_444_formats),
   },
   [MESA_VK_FORMAT_CLASS_64_BIT_G16B16G16R16] = {
      .formats = mesa_vk_format_class_64_bit_g16b16g16r16_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_64_bit_g16b16g16r16_formats),
   },
   [MESA_VK_FORMAT_CLASS_64_BIT_B16G16R16G16] = {
      .formats = mesa_vk_format_class_64_bit_b16g16r16g16_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_64_bit_b16g16r16g16_formats),
   },
   [MESA_VK_FORMAT_CLASS_16_BIT_3_PLANE_420] = {
      .formats = mesa_vk_format_class_16_bit_3_plane_420_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_16_bit_3_plane_420_formats),
   },
   [MESA_VK_FORMAT_CLASS_16_BIT_2_PLANE_420] = {
      .formats = mesa_vk_format_class_16_bit_2_plane_420_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_16_bit_2_plane_420_formats),
   },
   [MESA_VK_FORMAT_CLASS_16_BIT_3_PLANE_422] = {
      .formats = mesa_vk_format_class_16_bit_3_plane_422_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_16_bit_3_plane_422_formats),
   },
   [MESA_VK_FORMAT_CLASS_16_BIT_2_PLANE_422] = {
      .formats = mesa_vk_format_class_16_bit_2_plane_422_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_16_bit_2_plane_422_formats),
   },
   [MESA_VK_FORMAT_CLASS_16_BIT_3_PLANE_444] = {
      .formats = mesa_vk_format_class_16_bit_3_plane_444_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_16_bit_3_plane_444_formats),
   },
   [MESA_VK_FORMAT_CLASS_PVRTC1_2BPP] = {
      .formats = mesa_vk_format_class_pvrtc1_2bpp_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_pvrtc1_2bpp_formats),
   },
   [MESA_VK_FORMAT_CLASS_PVRTC1_4BPP] = {
      .formats = mesa_vk_format_class_pvrtc1_4bpp_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_pvrtc1_4bpp_formats),
   },
   [MESA_VK_FORMAT_CLASS_PVRTC2_2BPP] = {
      .formats = mesa_vk_format_class_pvrtc2_2bpp_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_pvrtc2_2bpp_formats),
   },
   [MESA_VK_FORMAT_CLASS_PVRTC2_4BPP] = {
      .formats = mesa_vk_format_class_pvrtc2_4bpp_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_pvrtc2_4bpp_formats),
   },
   [MESA_VK_FORMAT_CLASS_8_BIT_2_PLANE_444] = {
      .formats = mesa_vk_format_class_8_bit_2_plane_444_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_8_bit_2_plane_444_formats),
   },
   [MESA_VK_FORMAT_CLASS_10_BIT_2_PLANE_444] = {
      .formats = mesa_vk_format_class_10_bit_2_plane_444_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_10_bit_2_plane_444_formats),
   },
   [MESA_VK_FORMAT_CLASS_12_BIT_2_PLANE_444] = {
      .formats = mesa_vk_format_class_12_bit_2_plane_444_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_12_bit_2_plane_444_formats),
   },
   [MESA_VK_FORMAT_CLASS_16_BIT_2_PLANE_444] = {
      .formats = mesa_vk_format_class_16_bit_2_plane_444_formats,
      .format_count = ARRAY_SIZE(mesa_vk_format_class_16_bit_2_plane_444_formats),
   },
};

const struct vk_format_class_info *
vk_format_class_get_info(enum vk_format_class class)
{
   assert(class < ARRAY_SIZE(class_infos));
   return &class_infos[class];
}

const struct vk_format_class_info *
vk_format_get_class_info(VkFormat format)
{
    const struct vk_format_info *format_info = vk_format_get_info(format);
    return &class_infos[format_info->class];
}
