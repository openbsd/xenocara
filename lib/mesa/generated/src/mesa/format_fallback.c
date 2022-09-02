/*
 * Copyright 2017 Google
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL VMWARE AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "formats.h"
#include "util/macros.h"

/**
 * For an sRGB format, return the corresponding linear color space format.
 * For non-sRGB formats, return the format as-is.
 */
mesa_format
_mesa_get_srgb_format_linear(mesa_format format)
{
   switch (format) {
   case MESA_FORMAT_A8B8G8R8_SRGB:
      return MESA_FORMAT_A8B8G8R8_UNORM;
   case MESA_FORMAT_B8G8R8A8_SRGB:
      return MESA_FORMAT_B8G8R8A8_UNORM;
   case MESA_FORMAT_A8R8G8B8_SRGB:
      return MESA_FORMAT_A8R8G8B8_UNORM;
   case MESA_FORMAT_B8G8R8X8_SRGB:
      return MESA_FORMAT_B8G8R8X8_UNORM;
   case MESA_FORMAT_X8R8G8B8_SRGB:
      return MESA_FORMAT_X8R8G8B8_UNORM;
   case MESA_FORMAT_R8G8B8A8_SRGB:
      return MESA_FORMAT_R8G8B8A8_UNORM;
   case MESA_FORMAT_R8G8B8X8_SRGB:
      return MESA_FORMAT_R8G8B8X8_UNORM;
   case MESA_FORMAT_X8B8G8R8_SRGB:
      return MESA_FORMAT_X8B8G8R8_UNORM;
   case MESA_FORMAT_R_SRGB8:
      return MESA_FORMAT_R_UNORM8;
   case MESA_FORMAT_L_SRGB8:
      return MESA_FORMAT_L_UNORM8;
   case MESA_FORMAT_RG_SRGB8:
      return MESA_FORMAT_RG_UNORM8;
   case MESA_FORMAT_LA_SRGB8:
      return MESA_FORMAT_LA_UNORM8;
   case MESA_FORMAT_BGR_SRGB8:
      return MESA_FORMAT_BGR_UNORM8;
   case MESA_FORMAT_SRGB_DXT1:
      return MESA_FORMAT_RGB_DXT1;
   case MESA_FORMAT_SRGBA_DXT1:
      return MESA_FORMAT_RGBA_DXT1;
   case MESA_FORMAT_SRGBA_DXT3:
      return MESA_FORMAT_RGBA_DXT3;
   case MESA_FORMAT_SRGBA_DXT5:
      return MESA_FORMAT_RGBA_DXT5;
   case MESA_FORMAT_ETC2_SRGB8:
      return MESA_FORMAT_ETC2_RGB8;
   case MESA_FORMAT_ETC2_SRGB8_ALPHA8_EAC:
      return MESA_FORMAT_ETC2_RGBA8_EAC;
   case MESA_FORMAT_ETC2_SRGB8_PUNCHTHROUGH_ALPHA1:
      return MESA_FORMAT_ETC2_RGB8_PUNCHTHROUGH_ALPHA1;
   case MESA_FORMAT_BPTC_SRGB_ALPHA_UNORM:
      return MESA_FORMAT_BPTC_RGBA_UNORM;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_4x4:
      return MESA_FORMAT_RGBA_ASTC_4x4;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_5x4:
      return MESA_FORMAT_RGBA_ASTC_5x4;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_5x5:
      return MESA_FORMAT_RGBA_ASTC_5x5;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_6x5:
      return MESA_FORMAT_RGBA_ASTC_6x5;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_6x6:
      return MESA_FORMAT_RGBA_ASTC_6x6;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_8x5:
      return MESA_FORMAT_RGBA_ASTC_8x5;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_8x6:
      return MESA_FORMAT_RGBA_ASTC_8x6;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_8x8:
      return MESA_FORMAT_RGBA_ASTC_8x8;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_10x5:
      return MESA_FORMAT_RGBA_ASTC_10x5;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_10x6:
      return MESA_FORMAT_RGBA_ASTC_10x6;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_10x8:
      return MESA_FORMAT_RGBA_ASTC_10x8;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_10x10:
      return MESA_FORMAT_RGBA_ASTC_10x10;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_12x10:
      return MESA_FORMAT_RGBA_ASTC_12x10;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_12x12:
      return MESA_FORMAT_RGBA_ASTC_12x12;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_3x3x3:
      return MESA_FORMAT_RGBA_ASTC_3x3x3;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_4x3x3:
      return MESA_FORMAT_RGBA_ASTC_4x3x3;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_4x4x3:
      return MESA_FORMAT_RGBA_ASTC_4x4x3;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_4x4x4:
      return MESA_FORMAT_RGBA_ASTC_4x4x4;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_5x4x4:
      return MESA_FORMAT_RGBA_ASTC_5x4x4;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_5x5x4:
      return MESA_FORMAT_RGBA_ASTC_5x5x4;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_5x5x5:
      return MESA_FORMAT_RGBA_ASTC_5x5x5;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_6x5x5:
      return MESA_FORMAT_RGBA_ASTC_6x5x5;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_6x6x5:
      return MESA_FORMAT_RGBA_ASTC_6x6x5;
   case MESA_FORMAT_SRGB8_ALPHA8_ASTC_6x6x6:
      return MESA_FORMAT_RGBA_ASTC_6x6x6;
   default:
      return format;
   }
}

/**
 * For an intensity format, return the corresponding red format.  For other
 * formats, return the format as-is.
 */
mesa_format
_mesa_get_intensity_format_red(mesa_format format)
{
   switch (format) {
   case MESA_FORMAT_I_UNORM8:
      return MESA_FORMAT_R_UNORM8;
   case MESA_FORMAT_I_UNORM16:
      return MESA_FORMAT_R_UNORM16;
   case MESA_FORMAT_I_SNORM8:
      return MESA_FORMAT_R_SNORM8;
   case MESA_FORMAT_I_SNORM16:
      return MESA_FORMAT_R_SNORM16;
   case MESA_FORMAT_I_FLOAT16:
      return MESA_FORMAT_R_FLOAT16;
   case MESA_FORMAT_I_FLOAT32:
      return MESA_FORMAT_R_FLOAT32;
   case MESA_FORMAT_I_UINT8:
      return MESA_FORMAT_R_UINT8;
   case MESA_FORMAT_I_UINT16:
      return MESA_FORMAT_R_UINT16;
   case MESA_FORMAT_I_UINT32:
      return MESA_FORMAT_R_UINT32;
   case MESA_FORMAT_I_SINT8:
      return MESA_FORMAT_R_SINT8;
   case MESA_FORMAT_I_SINT16:
      return MESA_FORMAT_R_SINT16;
   case MESA_FORMAT_I_SINT32:
      return MESA_FORMAT_R_SINT32;
   default:
      return format;
   }
}
