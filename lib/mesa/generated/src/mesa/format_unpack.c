/*
 * Mesa 3-D graphics library
 *
 * Copyright (c) 2011 VMware, Inc.
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */


/**
 * Color, depth, stencil packing functions.
 * Used to pack basic color, depth and stencil formats to specific
 * hardware formats.
 *
 * There are both per-pixel and per-row packing functions:
 * - The former will be used by swrast to write values to the color, depth,
 *   stencil buffers when drawing points, lines and masked spans.
 * - The later will be used for image-oriented functions like glDrawPixels,
 *   glAccum, and glTexImage.
 */

#include <stdint.h>
#include <stdlib.h>

#include "format_unpack.h"
#include "format_utils.h"
#include "macros.h"
#include "util/format_rgb9e5.h"
#include "util/format_r11g11b10f.h"
#include "util/format_srgb.h"

#define UNPACK(SRC, OFFSET, BITS) (((SRC) >> (OFFSET)) & MAX_UINT(BITS))



/* float unpacking functions */


static inline void
unpack_float_a8b8g8r8_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 8);
            uint8_t b = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t r = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         
               dst[1] = _mesa_unorm_to_float(g, 8);
      
         
               dst[2] = _mesa_unorm_to_float(b, 8);
      
         
               dst[3] = _mesa_unorm_to_float(a, 8);
}

static inline void
unpack_float_x8b8g8r8_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t r = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         
               dst[1] = _mesa_unorm_to_float(g, 8);
      
         
               dst[2] = _mesa_unorm_to_float(b, 8);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_r8g8b8a8_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t b = UNPACK(*src, 16, 8);
            uint8_t a = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         
               dst[1] = _mesa_unorm_to_float(g, 8);
      
         
               dst[2] = _mesa_unorm_to_float(b, 8);
      
         
               dst[3] = _mesa_unorm_to_float(a, 8);
}

static inline void
unpack_float_r8g8b8x8_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t b = UNPACK(*src, 16, 8);

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         
               dst[1] = _mesa_unorm_to_float(g, 8);
      
         
               dst[2] = _mesa_unorm_to_float(b, 8);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_b8g8r8a8_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t r = UNPACK(*src, 16, 8);
            uint8_t a = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         
               dst[1] = _mesa_unorm_to_float(g, 8);
      
         
               dst[2] = _mesa_unorm_to_float(b, 8);
      
         
               dst[3] = _mesa_unorm_to_float(a, 8);
}

static inline void
unpack_float_b8g8r8x8_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t r = UNPACK(*src, 16, 8);

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         
               dst[1] = _mesa_unorm_to_float(g, 8);
      
         
               dst[2] = _mesa_unorm_to_float(b, 8);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_a8r8g8b8_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 8);
            uint8_t r = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t b = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         
               dst[1] = _mesa_unorm_to_float(g, 8);
      
         
               dst[2] = _mesa_unorm_to_float(b, 8);
      
         
               dst[3] = _mesa_unorm_to_float(a, 8);
}

static inline void
unpack_float_x8r8g8b8_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t b = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         
               dst[1] = _mesa_unorm_to_float(g, 8);
      
         
               dst[2] = _mesa_unorm_to_float(b, 8);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_b5g6r5_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 6);
            uint8_t r = UNPACK(*src, 11, 5);

      
         
               dst[0] = _mesa_unorm_to_float(r, 5);
      
         
               dst[1] = _mesa_unorm_to_float(g, 6);
      
         
               dst[2] = _mesa_unorm_to_float(b, 5);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_r5g6b5_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 6);
            uint8_t b = UNPACK(*src, 11, 5);

      
         
               dst[0] = _mesa_unorm_to_float(r, 5);
      
         
               dst[1] = _mesa_unorm_to_float(g, 6);
      
         
               dst[2] = _mesa_unorm_to_float(b, 5);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_b4g4r4a4_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 4);
            uint8_t g = UNPACK(*src, 4, 4);
            uint8_t r = UNPACK(*src, 8, 4);
            uint8_t a = UNPACK(*src, 12, 4);

      
         
               dst[0] = _mesa_unorm_to_float(r, 4);
      
         
               dst[1] = _mesa_unorm_to_float(g, 4);
      
         
               dst[2] = _mesa_unorm_to_float(b, 4);
      
         
               dst[3] = _mesa_unorm_to_float(a, 4);
}

static inline void
unpack_float_b4g4r4x4_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 4);
            uint8_t g = UNPACK(*src, 4, 4);
            uint8_t r = UNPACK(*src, 8, 4);

      
         
               dst[0] = _mesa_unorm_to_float(r, 4);
      
         
               dst[1] = _mesa_unorm_to_float(g, 4);
      
         
               dst[2] = _mesa_unorm_to_float(b, 4);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_a4r4g4b4_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 4);
            uint8_t r = UNPACK(*src, 4, 4);
            uint8_t g = UNPACK(*src, 8, 4);
            uint8_t b = UNPACK(*src, 12, 4);

      
         
               dst[0] = _mesa_unorm_to_float(r, 4);
      
         
               dst[1] = _mesa_unorm_to_float(g, 4);
      
         
               dst[2] = _mesa_unorm_to_float(b, 4);
      
         
               dst[3] = _mesa_unorm_to_float(a, 4);
}

static inline void
unpack_float_a1b5g5r5_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 1);
            uint8_t b = UNPACK(*src, 1, 5);
            uint8_t g = UNPACK(*src, 6, 5);
            uint8_t r = UNPACK(*src, 11, 5);

      
         
               dst[0] = _mesa_unorm_to_float(r, 5);
      
         
               dst[1] = _mesa_unorm_to_float(g, 5);
      
         
               dst[2] = _mesa_unorm_to_float(b, 5);
      
         
               dst[3] = _mesa_unorm_to_float(a, 1);
}

static inline void
unpack_float_x1b5g5r5_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 1, 5);
            uint8_t g = UNPACK(*src, 6, 5);
            uint8_t r = UNPACK(*src, 11, 5);

      
         
               dst[0] = _mesa_unorm_to_float(r, 5);
      
         
               dst[1] = _mesa_unorm_to_float(g, 5);
      
         
               dst[2] = _mesa_unorm_to_float(b, 5);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_b5g5r5a1_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 5);
            uint8_t r = UNPACK(*src, 10, 5);
            uint8_t a = UNPACK(*src, 15, 1);

      
         
               dst[0] = _mesa_unorm_to_float(r, 5);
      
         
               dst[1] = _mesa_unorm_to_float(g, 5);
      
         
               dst[2] = _mesa_unorm_to_float(b, 5);
      
         
               dst[3] = _mesa_unorm_to_float(a, 1);
}

static inline void
unpack_float_b5g5r5x1_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 5);
            uint8_t r = UNPACK(*src, 10, 5);

      
         
               dst[0] = _mesa_unorm_to_float(r, 5);
      
         
               dst[1] = _mesa_unorm_to_float(g, 5);
      
         
               dst[2] = _mesa_unorm_to_float(b, 5);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_a1r5g5b5_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 1);
            uint8_t r = UNPACK(*src, 1, 5);
            uint8_t g = UNPACK(*src, 6, 5);
            uint8_t b = UNPACK(*src, 11, 5);

      
         
               dst[0] = _mesa_unorm_to_float(r, 5);
      
         
               dst[1] = _mesa_unorm_to_float(g, 5);
      
         
               dst[2] = _mesa_unorm_to_float(b, 5);
      
         
               dst[3] = _mesa_unorm_to_float(a, 1);
}

static inline void
unpack_float_l4a4_unorm(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = UNPACK(*src, 0, 4);
            uint8_t a = UNPACK(*src, 4, 4);

      
         
               dst[0] = _mesa_unorm_to_float(l, 4);
      
         
               dst[1] = _mesa_unorm_to_float(l, 4);
      
         
               dst[2] = _mesa_unorm_to_float(l, 4);
      
         
               dst[3] = _mesa_unorm_to_float(a, 4);
}

static inline void
unpack_float_b2g3r3_unorm(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 2);
            uint8_t g = UNPACK(*src, 2, 3);
            uint8_t r = UNPACK(*src, 5, 3);

      
         
               dst[0] = _mesa_unorm_to_float(r, 3);
      
         
               dst[1] = _mesa_unorm_to_float(g, 3);
      
         
               dst[2] = _mesa_unorm_to_float(b, 2);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_b10g10r10a2_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint16_t b = UNPACK(*src, 0, 10);
            uint16_t g = UNPACK(*src, 10, 10);
            uint16_t r = UNPACK(*src, 20, 10);
            uint8_t a = UNPACK(*src, 30, 2);

      
         
               dst[0] = _mesa_unorm_to_float(r, 10);
      
         
               dst[1] = _mesa_unorm_to_float(g, 10);
      
         
               dst[2] = _mesa_unorm_to_float(b, 10);
      
         
               dst[3] = _mesa_unorm_to_float(a, 2);
}

static inline void
unpack_float_b10g10r10x2_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint16_t b = UNPACK(*src, 0, 10);
            uint16_t g = UNPACK(*src, 10, 10);
            uint16_t r = UNPACK(*src, 20, 10);

      
         
               dst[0] = _mesa_unorm_to_float(r, 10);
      
         
               dst[1] = _mesa_unorm_to_float(g, 10);
      
         
               dst[2] = _mesa_unorm_to_float(b, 10);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_r10g10b10a2_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint16_t r = UNPACK(*src, 0, 10);
            uint16_t g = UNPACK(*src, 10, 10);
            uint16_t b = UNPACK(*src, 20, 10);
            uint8_t a = UNPACK(*src, 30, 2);

      
         
               dst[0] = _mesa_unorm_to_float(r, 10);
      
         
               dst[1] = _mesa_unorm_to_float(g, 10);
      
         
               dst[2] = _mesa_unorm_to_float(b, 10);
      
         
               dst[3] = _mesa_unorm_to_float(a, 2);
}

static inline void
unpack_float_r10g10b10x2_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint16_t r = UNPACK(*src, 0, 10);
            uint16_t g = UNPACK(*src, 10, 10);
            uint16_t b = UNPACK(*src, 20, 10);

      
         
               dst[0] = _mesa_unorm_to_float(r, 10);
      
         
               dst[1] = _mesa_unorm_to_float(g, 10);
      
         
               dst[2] = _mesa_unorm_to_float(b, 10);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_r3g3b2_unorm(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 3);
            uint8_t g = UNPACK(*src, 3, 3);
            uint8_t b = UNPACK(*src, 6, 2);

      
         
               dst[0] = _mesa_unorm_to_float(r, 3);
      
         
               dst[1] = _mesa_unorm_to_float(g, 3);
      
         
               dst[2] = _mesa_unorm_to_float(b, 2);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_a4b4g4r4_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 4);
            uint8_t b = UNPACK(*src, 4, 4);
            uint8_t g = UNPACK(*src, 8, 4);
            uint8_t r = UNPACK(*src, 12, 4);

      
         
               dst[0] = _mesa_unorm_to_float(r, 4);
      
         
               dst[1] = _mesa_unorm_to_float(g, 4);
      
         
               dst[2] = _mesa_unorm_to_float(b, 4);
      
         
               dst[3] = _mesa_unorm_to_float(a, 4);
}

static inline void
unpack_float_r4g4b4a4_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 4);
            uint8_t g = UNPACK(*src, 4, 4);
            uint8_t b = UNPACK(*src, 8, 4);
            uint8_t a = UNPACK(*src, 12, 4);

      
         
               dst[0] = _mesa_unorm_to_float(r, 4);
      
         
               dst[1] = _mesa_unorm_to_float(g, 4);
      
         
               dst[2] = _mesa_unorm_to_float(b, 4);
      
         
               dst[3] = _mesa_unorm_to_float(a, 4);
}

static inline void
unpack_float_r5g5b5a1_unorm(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 5);
            uint8_t b = UNPACK(*src, 10, 5);
            uint8_t a = UNPACK(*src, 15, 1);

      
         
               dst[0] = _mesa_unorm_to_float(r, 5);
      
         
               dst[1] = _mesa_unorm_to_float(g, 5);
      
         
               dst[2] = _mesa_unorm_to_float(b, 5);
      
         
               dst[3] = _mesa_unorm_to_float(a, 1);
}

static inline void
unpack_float_a2b10g10r10_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 2);
            uint16_t b = UNPACK(*src, 2, 10);
            uint16_t g = UNPACK(*src, 12, 10);
            uint16_t r = UNPACK(*src, 22, 10);

      
         
               dst[0] = _mesa_unorm_to_float(r, 10);
      
         
               dst[1] = _mesa_unorm_to_float(g, 10);
      
         
               dst[2] = _mesa_unorm_to_float(b, 10);
      
         
               dst[3] = _mesa_unorm_to_float(a, 2);
}

static inline void
unpack_float_a2r10g10b10_unorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 2);
            uint16_t r = UNPACK(*src, 2, 10);
            uint16_t g = UNPACK(*src, 12, 10);
            uint16_t b = UNPACK(*src, 22, 10);

      
         
               dst[0] = _mesa_unorm_to_float(r, 10);
      
         
               dst[1] = _mesa_unorm_to_float(g, 10);
      
         
               dst[2] = _mesa_unorm_to_float(b, 10);
      
         
               dst[3] = _mesa_unorm_to_float(a, 2);
}

static inline void
unpack_float_a_unorm8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t a = src[0];

      
         dst[0] = 0.0f;
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         
               dst[3] = _mesa_unorm_to_float(a, 8);
}

static inline void
unpack_float_a_unorm16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t a = src[0];

      
         dst[0] = 0.0f;
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         
               dst[3] = _mesa_unorm_to_float(a, 16);
}

static inline void
unpack_float_l_unorm8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = src[0];

      
         
               dst[0] = _mesa_unorm_to_float(l, 8);
      
         
               dst[1] = _mesa_unorm_to_float(l, 8);
      
         
               dst[2] = _mesa_unorm_to_float(l, 8);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_l_unorm16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t l = src[0];

      
         
               dst[0] = _mesa_unorm_to_float(l, 16);
      
         
               dst[1] = _mesa_unorm_to_float(l, 16);
      
         
               dst[2] = _mesa_unorm_to_float(l, 16);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_la_unorm8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = src[0];
            uint8_t a = src[1];

      
         
               dst[0] = _mesa_unorm_to_float(l, 8);
      
         
               dst[1] = _mesa_unorm_to_float(l, 8);
      
         
               dst[2] = _mesa_unorm_to_float(l, 8);
      
         
               dst[3] = _mesa_unorm_to_float(a, 8);
}

static inline void
unpack_float_la_unorm16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t l = src[0];
            uint16_t a = src[1];

      
         
               dst[0] = _mesa_unorm_to_float(l, 16);
      
         
               dst[1] = _mesa_unorm_to_float(l, 16);
      
         
               dst[2] = _mesa_unorm_to_float(l, 16);
      
         
               dst[3] = _mesa_unorm_to_float(a, 16);
}

static inline void
unpack_float_i_unorm8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t i = src[0];

      
         
               dst[0] = _mesa_unorm_to_float(i, 8);
      
         
               dst[1] = _mesa_unorm_to_float(i, 8);
      
         
               dst[2] = _mesa_unorm_to_float(i, 8);
      
         
               dst[3] = _mesa_unorm_to_float(i, 8);
}

static inline void
unpack_float_i_unorm16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t i = src[0];

      
         
               dst[0] = _mesa_unorm_to_float(i, 16);
      
         
               dst[1] = _mesa_unorm_to_float(i, 16);
      
         
               dst[2] = _mesa_unorm_to_float(i, 16);
      
         
               dst[3] = _mesa_unorm_to_float(i, 16);
}

static inline void
unpack_float_r_unorm8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_r_unorm16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];

      
         
               dst[0] = _mesa_unorm_to_float(r, 16);
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rg_unorm8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];
            uint8_t g = src[1];

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         
               dst[1] = _mesa_unorm_to_float(g, 8);
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rg_unorm16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];

      
         
               dst[0] = _mesa_unorm_to_float(r, 16);
      
         
               dst[1] = _mesa_unorm_to_float(g, 16);
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_bgr_unorm8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t b = src[0];
            uint8_t g = src[1];
            uint8_t r = src[2];

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         
               dst[1] = _mesa_unorm_to_float(g, 8);
      
         
               dst[2] = _mesa_unorm_to_float(b, 8);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rgb_unorm8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];
            uint8_t g = src[1];
            uint8_t b = src[2];

      
         
               dst[0] = _mesa_unorm_to_float(r, 8);
      
         
               dst[1] = _mesa_unorm_to_float(g, 8);
      
         
               dst[2] = _mesa_unorm_to_float(b, 8);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rgba_unorm16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];
            uint16_t b = src[2];
            uint16_t a = src[3];

      
         
               dst[0] = _mesa_unorm_to_float(r, 16);
      
         
               dst[1] = _mesa_unorm_to_float(g, 16);
      
         
               dst[2] = _mesa_unorm_to_float(b, 16);
      
         
               dst[3] = _mesa_unorm_to_float(a, 16);
}

static inline void
unpack_float_rgbx_unorm16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];
            uint16_t b = src[2];

      
         
               dst[0] = _mesa_unorm_to_float(r, 16);
      
         
               dst[1] = _mesa_unorm_to_float(g, 16);
      
         
               dst[2] = _mesa_unorm_to_float(b, 16);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_a8b8g8r8_snorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            int8_t a = UNPACK(*src, 0, 8);
            int8_t b = UNPACK(*src, 8, 8);
            int8_t g = UNPACK(*src, 16, 8);
            int8_t r = UNPACK(*src, 24, 8);

      
         
            dst[0] = _mesa_snorm_to_float(r, 8);
      
         
            dst[1] = _mesa_snorm_to_float(g, 8);
      
         
            dst[2] = _mesa_snorm_to_float(b, 8);
      
         
            dst[3] = _mesa_snorm_to_float(a, 8);
}

static inline void
unpack_float_x8b8g8r8_snorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            int8_t b = UNPACK(*src, 8, 8);
            int8_t g = UNPACK(*src, 16, 8);
            int8_t r = UNPACK(*src, 24, 8);

      
         
            dst[0] = _mesa_snorm_to_float(r, 8);
      
         
            dst[1] = _mesa_snorm_to_float(g, 8);
      
         
            dst[2] = _mesa_snorm_to_float(b, 8);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_r8g8b8a8_snorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            int8_t r = UNPACK(*src, 0, 8);
            int8_t g = UNPACK(*src, 8, 8);
            int8_t b = UNPACK(*src, 16, 8);
            int8_t a = UNPACK(*src, 24, 8);

      
         
            dst[0] = _mesa_snorm_to_float(r, 8);
      
         
            dst[1] = _mesa_snorm_to_float(g, 8);
      
         
            dst[2] = _mesa_snorm_to_float(b, 8);
      
         
            dst[3] = _mesa_snorm_to_float(a, 8);
}

static inline void
unpack_float_r8g8b8x8_snorm(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            int8_t r = UNPACK(*src, 0, 8);
            int8_t g = UNPACK(*src, 8, 8);
            int8_t b = UNPACK(*src, 16, 8);

      
         
            dst[0] = _mesa_snorm_to_float(r, 8);
      
         
            dst[1] = _mesa_snorm_to_float(g, 8);
      
         
            dst[2] = _mesa_snorm_to_float(b, 8);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_a_snorm8(const void *void_src, float dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t a = src[0];

      
         dst[0] = 0.0f;
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         
            dst[3] = _mesa_snorm_to_float(a, 8);
}

static inline void
unpack_float_a_snorm16(const void *void_src, float dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t a = src[0];

      
         dst[0] = 0.0f;
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         
            dst[3] = _mesa_snorm_to_float(a, 16);
}

static inline void
unpack_float_l_snorm8(const void *void_src, float dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t l = src[0];

      
         
            dst[0] = _mesa_snorm_to_float(l, 8);
      
         
            dst[1] = _mesa_snorm_to_float(l, 8);
      
         
            dst[2] = _mesa_snorm_to_float(l, 8);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_l_snorm16(const void *void_src, float dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t l = src[0];

      
         
            dst[0] = _mesa_snorm_to_float(l, 16);
      
         
            dst[1] = _mesa_snorm_to_float(l, 16);
      
         
            dst[2] = _mesa_snorm_to_float(l, 16);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_i_snorm8(const void *void_src, float dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t i = src[0];

      
         
            dst[0] = _mesa_snorm_to_float(i, 8);
      
         
            dst[1] = _mesa_snorm_to_float(i, 8);
      
         
            dst[2] = _mesa_snorm_to_float(i, 8);
      
         
            dst[3] = _mesa_snorm_to_float(i, 8);
}

static inline void
unpack_float_i_snorm16(const void *void_src, float dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t i = src[0];

      
         
            dst[0] = _mesa_snorm_to_float(i, 16);
      
         
            dst[1] = _mesa_snorm_to_float(i, 16);
      
         
            dst[2] = _mesa_snorm_to_float(i, 16);
      
         
            dst[3] = _mesa_snorm_to_float(i, 16);
}

static inline void
unpack_float_r_snorm8(const void *void_src, float dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t r = src[0];

      
         
            dst[0] = _mesa_snorm_to_float(r, 8);
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_r_snorm16(const void *void_src, float dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];

      
         
            dst[0] = _mesa_snorm_to_float(r, 16);
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_la_snorm8(const void *void_src, float dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t l = src[0];
            int8_t a = src[1];

      
         
            dst[0] = _mesa_snorm_to_float(l, 8);
      
         
            dst[1] = _mesa_snorm_to_float(l, 8);
      
         
            dst[2] = _mesa_snorm_to_float(l, 8);
      
         
            dst[3] = _mesa_snorm_to_float(a, 8);
}

static inline void
unpack_float_la_snorm16(const void *void_src, float dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t l = src[0];
            int16_t a = src[1];

      
         
            dst[0] = _mesa_snorm_to_float(l, 16);
      
         
            dst[1] = _mesa_snorm_to_float(l, 16);
      
         
            dst[2] = _mesa_snorm_to_float(l, 16);
      
         
            dst[3] = _mesa_snorm_to_float(a, 16);
}

static inline void
unpack_float_rg_snorm8(const void *void_src, float dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t r = src[0];
            int8_t g = src[1];

      
         
            dst[0] = _mesa_snorm_to_float(r, 8);
      
         
            dst[1] = _mesa_snorm_to_float(g, 8);
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rg_snorm16(const void *void_src, float dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];

      
         
            dst[0] = _mesa_snorm_to_float(r, 16);
      
         
            dst[1] = _mesa_snorm_to_float(g, 16);
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rgb_snorm16(const void *void_src, float dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];
            int16_t b = src[2];

      
         
            dst[0] = _mesa_snorm_to_float(r, 16);
      
         
            dst[1] = _mesa_snorm_to_float(g, 16);
      
         
            dst[2] = _mesa_snorm_to_float(b, 16);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rgba_snorm16(const void *void_src, float dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];
            int16_t b = src[2];
            int16_t a = src[3];

      
         
            dst[0] = _mesa_snorm_to_float(r, 16);
      
         
            dst[1] = _mesa_snorm_to_float(g, 16);
      
         
            dst[2] = _mesa_snorm_to_float(b, 16);
      
         
            dst[3] = _mesa_snorm_to_float(a, 16);
}

static inline void
unpack_float_rgbx_snorm16(const void *void_src, float dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];
            int16_t b = src[2];

      
         
            dst[0] = _mesa_snorm_to_float(r, 16);
      
         
            dst[1] = _mesa_snorm_to_float(g, 16);
      
         
            dst[2] = _mesa_snorm_to_float(b, 16);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_a8b8g8r8_srgb(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 8);
            uint8_t b = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t r = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_8unorm_to_linear_float(r);
      
         
               
               dst[1] = util_format_srgb_8unorm_to_linear_float(g);
      
         
               
               dst[2] = util_format_srgb_8unorm_to_linear_float(b);
      
         
               dst[3] = _mesa_unorm_to_float(a, 8);
}

static inline void
unpack_float_b8g8r8a8_srgb(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t r = UNPACK(*src, 16, 8);
            uint8_t a = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_8unorm_to_linear_float(r);
      
         
               
               dst[1] = util_format_srgb_8unorm_to_linear_float(g);
      
         
               
               dst[2] = util_format_srgb_8unorm_to_linear_float(b);
      
         
               dst[3] = _mesa_unorm_to_float(a, 8);
}

static inline void
unpack_float_a8r8g8b8_srgb(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 8);
            uint8_t r = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t b = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_8unorm_to_linear_float(r);
      
         
               
               dst[1] = util_format_srgb_8unorm_to_linear_float(g);
      
         
               
               dst[2] = util_format_srgb_8unorm_to_linear_float(b);
      
         
               dst[3] = _mesa_unorm_to_float(a, 8);
}

static inline void
unpack_float_b8g8r8x8_srgb(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t r = UNPACK(*src, 16, 8);

      
         
               
               dst[0] = util_format_srgb_8unorm_to_linear_float(r);
      
         
               
               dst[1] = util_format_srgb_8unorm_to_linear_float(g);
      
         
               
               dst[2] = util_format_srgb_8unorm_to_linear_float(b);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_x8r8g8b8_srgb(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t b = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_8unorm_to_linear_float(r);
      
         
               
               dst[1] = util_format_srgb_8unorm_to_linear_float(g);
      
         
               
               dst[2] = util_format_srgb_8unorm_to_linear_float(b);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_r8g8b8a8_srgb(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t b = UNPACK(*src, 16, 8);
            uint8_t a = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_8unorm_to_linear_float(r);
      
         
               
               dst[1] = util_format_srgb_8unorm_to_linear_float(g);
      
         
               
               dst[2] = util_format_srgb_8unorm_to_linear_float(b);
      
         
               dst[3] = _mesa_unorm_to_float(a, 8);
}

static inline void
unpack_float_r8g8b8x8_srgb(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t b = UNPACK(*src, 16, 8);

      
         
               
               dst[0] = util_format_srgb_8unorm_to_linear_float(r);
      
         
               
               dst[1] = util_format_srgb_8unorm_to_linear_float(g);
      
         
               
               dst[2] = util_format_srgb_8unorm_to_linear_float(b);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_x8b8g8r8_srgb(const void *void_src, float dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t r = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_8unorm_to_linear_float(r);
      
         
               
               dst[1] = util_format_srgb_8unorm_to_linear_float(g);
      
         
               
               dst[2] = util_format_srgb_8unorm_to_linear_float(b);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_r_srgb8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];

      
         
               
               dst[0] = util_format_srgb_8unorm_to_linear_float(r);
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_l_srgb8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = src[0];

      
         
               dst[0] = _mesa_unorm_to_float(l, 8);
      
         
               dst[1] = _mesa_unorm_to_float(l, 8);
      
         
               dst[2] = _mesa_unorm_to_float(l, 8);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_la_srgb8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = src[0];
            uint8_t a = src[1];

      
         
               dst[0] = _mesa_unorm_to_float(l, 8);
      
         
               dst[1] = _mesa_unorm_to_float(l, 8);
      
         
               dst[2] = _mesa_unorm_to_float(l, 8);
      
         
               dst[3] = _mesa_unorm_to_float(a, 8);
}

static inline void
unpack_float_bgr_srgb8(const void *void_src, float dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t b = src[0];
            uint8_t g = src[1];
            uint8_t r = src[2];

      
         
               
               dst[0] = util_format_srgb_8unorm_to_linear_float(r);
      
         
               
               dst[1] = util_format_srgb_8unorm_to_linear_float(g);
      
         
               
               dst[2] = util_format_srgb_8unorm_to_linear_float(b);
      
         dst[3] = 1.0f;
}
            
static inline void
unpack_float_a_float16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t a = src[0];

      
         dst[0] = 0.0f;
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         
               dst[3] = _mesa_half_to_float(a);
}

static inline void
unpack_float_a_float32(const void *void_src, float dst[4])
{
   float *src = (float *)void_src;
            float a = src[0];

      
         dst[0] = 0.0f;
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         
               dst[3] = a;
}

static inline void
unpack_float_l_float16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t l = src[0];

      
         
               dst[0] = _mesa_half_to_float(l);
      
         
               dst[1] = _mesa_half_to_float(l);
      
         
               dst[2] = _mesa_half_to_float(l);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_l_float32(const void *void_src, float dst[4])
{
   float *src = (float *)void_src;
            float l = src[0];

      
         
               dst[0] = l;
      
         
               dst[1] = l;
      
         
               dst[2] = l;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_la_float16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t l = src[0];
            uint16_t a = src[1];

      
         
               dst[0] = _mesa_half_to_float(l);
      
         
               dst[1] = _mesa_half_to_float(l);
      
         
               dst[2] = _mesa_half_to_float(l);
      
         
               dst[3] = _mesa_half_to_float(a);
}

static inline void
unpack_float_la_float32(const void *void_src, float dst[4])
{
   float *src = (float *)void_src;
            float l = src[0];
            float a = src[1];

      
         
               dst[0] = l;
      
         
               dst[1] = l;
      
         
               dst[2] = l;
      
         
               dst[3] = a;
}

static inline void
unpack_float_i_float16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t i = src[0];

      
         
               dst[0] = _mesa_half_to_float(i);
      
         
               dst[1] = _mesa_half_to_float(i);
      
         
               dst[2] = _mesa_half_to_float(i);
      
         
               dst[3] = _mesa_half_to_float(i);
}

static inline void
unpack_float_i_float32(const void *void_src, float dst[4])
{
   float *src = (float *)void_src;
            float i = src[0];

      
         
               dst[0] = i;
      
         
               dst[1] = i;
      
         
               dst[2] = i;
      
         
               dst[3] = i;
}

static inline void
unpack_float_r_float16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];

      
         
               dst[0] = _mesa_half_to_float(r);
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_r_float32(const void *void_src, float dst[4])
{
   float *src = (float *)void_src;
            float r = src[0];

      
         
               dst[0] = r;
      
         dst[1] = 0.0f;
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rg_float16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];

      
         
               dst[0] = _mesa_half_to_float(r);
      
         
               dst[1] = _mesa_half_to_float(g);
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rg_float32(const void *void_src, float dst[4])
{
   float *src = (float *)void_src;
            float r = src[0];
            float g = src[1];

      
         
               dst[0] = r;
      
         
               dst[1] = g;
      
         dst[2] = 0.0f;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rgb_float16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];
            uint16_t b = src[2];

      
         
               dst[0] = _mesa_half_to_float(r);
      
         
               dst[1] = _mesa_half_to_float(g);
      
         
               dst[2] = _mesa_half_to_float(b);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rgb_float32(const void *void_src, float dst[4])
{
   float *src = (float *)void_src;
            float r = src[0];
            float g = src[1];
            float b = src[2];

      
         
               dst[0] = r;
      
         
               dst[1] = g;
      
         
               dst[2] = b;
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rgba_float16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];
            uint16_t b = src[2];
            uint16_t a = src[3];

      
         
               dst[0] = _mesa_half_to_float(r);
      
         
               dst[1] = _mesa_half_to_float(g);
      
         
               dst[2] = _mesa_half_to_float(b);
      
         
               dst[3] = _mesa_half_to_float(a);
}

static inline void
unpack_float_rgba_float32(const void *void_src, float dst[4])
{
   float *src = (float *)void_src;
            float r = src[0];
            float g = src[1];
            float b = src[2];
            float a = src[3];

      
         
               dst[0] = r;
      
         
               dst[1] = g;
      
         
               dst[2] = b;
      
         
               dst[3] = a;
}

static inline void
unpack_float_rgbx_float16(const void *void_src, float dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];
            uint16_t b = src[2];

      
         
               dst[0] = _mesa_half_to_float(r);
      
         
               dst[1] = _mesa_half_to_float(g);
      
         
               dst[2] = _mesa_half_to_float(b);
      
         dst[3] = 1.0f;
}

static inline void
unpack_float_rgbx_float32(const void *void_src, float dst[4])
{
   float *src = (float *)void_src;
            float r = src[0];
            float g = src[1];
            float b = src[2];

      
         
               dst[0] = r;
      
         
               dst[1] = g;
      
         
               dst[2] = b;
      
         dst[3] = 1.0f;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
static void
unpack_float_r9g9b9e5_float(const void *src, float dst[4])
{
   rgb9e5_to_float3(*(const uint32_t *)src, dst);
   dst[3] = 1.0f;
}

static void
unpack_float_r11g11b10_float(const void *src, float dst[4])
{
   r11g11b10f_to_float3(*(const uint32_t *)src, dst);
   dst[3] = 1.0f;
}

static void
unpack_float_ycbcr(const void *src, float dst[][4], uint32_t n)
{
   uint32_t i;
   for (i = 0; i < n; i++) {
      const uint16_t *src0 = ((const uint16_t *) src) + i * 2; /* even */
      const uint16_t *src1 = src0 + 1;         /* odd */
      const uint8_t y0 = (*src0 >> 8) & 0xff;  /* luminance */
      const uint8_t cb = *src0 & 0xff;         /* chroma U */
      const uint8_t y1 = (*src1 >> 8) & 0xff;  /* luminance */
      const uint8_t cr = *src1 & 0xff;         /* chroma V */
      const uint8_t y = (i & 1) ? y1 : y0;     /* choose even/odd luminance */
      float r = 1.164F * (y - 16) + 1.596F * (cr - 128);
      float g = 1.164F * (y - 16) - 0.813F * (cr - 128) - 0.391F * (cb - 128);
      float b = 1.164F * (y - 16) + 2.018F * (cb - 128);
      r *= (1.0F / 255.0F);
      g *= (1.0F / 255.0F);
      b *= (1.0F / 255.0F);
      dst[i][0] = CLAMP(r, 0.0F, 1.0F);
      dst[i][1] = CLAMP(g, 0.0F, 1.0F);
      dst[i][2] = CLAMP(b, 0.0F, 1.0F);
      dst[i][3] = 1.0F;
   }
}

static void
unpack_float_ycbcr_rev(const void *src, float dst[][4], uint32_t n)
{
   uint32_t i;
   for (i = 0; i < n; i++) {
      const uint16_t *src0 = ((const uint16_t *) src) + i * 2; /* even */
      const uint16_t *src1 = src0 + 1;         /* odd */
      const uint8_t y0 = *src0 & 0xff;         /* luminance */
      const uint8_t cr = (*src0 >> 8) & 0xff;  /* chroma V */
      const uint8_t y1 = *src1 & 0xff;         /* luminance */
      const uint8_t cb = (*src1 >> 8) & 0xff;  /* chroma U */
      const uint8_t y = (i & 1) ? y1 : y0;     /* choose even/odd luminance */
      float r = 1.164F * (y - 16) + 1.596F * (cr - 128);
      float g = 1.164F * (y - 16) - 0.813F * (cr - 128) - 0.391F * (cb - 128);
      float b = 1.164F * (y - 16) + 2.018F * (cb - 128);
      r *= (1.0F / 255.0F);
      g *= (1.0F / 255.0F);
      b *= (1.0F / 255.0F);
      dst[i][0] = CLAMP(r, 0.0F, 1.0F);
      dst[i][1] = CLAMP(g, 0.0F, 1.0F);
      dst[i][2] = CLAMP(b, 0.0F, 1.0F);
      dst[i][3] = 1.0F;
   }
}

/* ubyte packing functions */


static inline void
unpack_ubyte_a8b8g8r8_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 8);
            uint8_t b = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t r = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 8, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_x8b8g8r8_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t r = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 8, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_r8g8b8a8_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t b = UNPACK(*src, 16, 8);
            uint8_t a = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 8, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_r8g8b8x8_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t b = UNPACK(*src, 16, 8);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 8, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_b8g8r8a8_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t r = UNPACK(*src, 16, 8);
            uint8_t a = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 8, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_b8g8r8x8_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t r = UNPACK(*src, 16, 8);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 8, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_a8r8g8b8_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 8);
            uint8_t r = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t b = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 8, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_x8r8g8b8_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t b = UNPACK(*src, 24, 8);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 8, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_b5g6r5_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 6);
            uint8_t r = UNPACK(*src, 11, 5);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 5, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 6, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 5, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_r5g6b5_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 6);
            uint8_t b = UNPACK(*src, 11, 5);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 5, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 6, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 5, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_b4g4r4a4_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 4);
            uint8_t g = UNPACK(*src, 4, 4);
            uint8_t r = UNPACK(*src, 8, 4);
            uint8_t a = UNPACK(*src, 12, 4);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 4, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 4, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 4, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 4, 8);
}

static inline void
unpack_ubyte_b4g4r4x4_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 4);
            uint8_t g = UNPACK(*src, 4, 4);
            uint8_t r = UNPACK(*src, 8, 4);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 4, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 4, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 4, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_a4r4g4b4_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 4);
            uint8_t r = UNPACK(*src, 4, 4);
            uint8_t g = UNPACK(*src, 8, 4);
            uint8_t b = UNPACK(*src, 12, 4);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 4, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 4, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 4, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 4, 8);
}

static inline void
unpack_ubyte_a1b5g5r5_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 1);
            uint8_t b = UNPACK(*src, 1, 5);
            uint8_t g = UNPACK(*src, 6, 5);
            uint8_t r = UNPACK(*src, 11, 5);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 5, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 5, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 5, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 1, 8);
}

static inline void
unpack_ubyte_x1b5g5r5_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 1, 5);
            uint8_t g = UNPACK(*src, 6, 5);
            uint8_t r = UNPACK(*src, 11, 5);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 5, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 5, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 5, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_b5g5r5a1_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 5);
            uint8_t r = UNPACK(*src, 10, 5);
            uint8_t a = UNPACK(*src, 15, 1);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 5, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 5, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 5, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 1, 8);
}

static inline void
unpack_ubyte_b5g5r5x1_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 5);
            uint8_t r = UNPACK(*src, 10, 5);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 5, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 5, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 5, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_a1r5g5b5_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 1);
            uint8_t r = UNPACK(*src, 1, 5);
            uint8_t g = UNPACK(*src, 6, 5);
            uint8_t b = UNPACK(*src, 11, 5);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 5, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 5, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 5, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 1, 8);
}

static inline void
unpack_ubyte_l4a4_unorm(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = UNPACK(*src, 0, 4);
            uint8_t a = UNPACK(*src, 4, 4);

      
         
               dst[0] = _mesa_unorm_to_unorm(l, 4, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(l, 4, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(l, 4, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 4, 8);
}

static inline void
unpack_ubyte_b2g3r3_unorm(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 2);
            uint8_t g = UNPACK(*src, 2, 3);
            uint8_t r = UNPACK(*src, 5, 3);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 3, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 3, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 2, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_b10g10r10a2_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint16_t b = UNPACK(*src, 0, 10);
            uint16_t g = UNPACK(*src, 10, 10);
            uint16_t r = UNPACK(*src, 20, 10);
            uint8_t a = UNPACK(*src, 30, 2);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 10, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 10, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 10, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 2, 8);
}

static inline void
unpack_ubyte_b10g10r10x2_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint16_t b = UNPACK(*src, 0, 10);
            uint16_t g = UNPACK(*src, 10, 10);
            uint16_t r = UNPACK(*src, 20, 10);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 10, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 10, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 10, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_r10g10b10a2_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint16_t r = UNPACK(*src, 0, 10);
            uint16_t g = UNPACK(*src, 10, 10);
            uint16_t b = UNPACK(*src, 20, 10);
            uint8_t a = UNPACK(*src, 30, 2);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 10, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 10, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 10, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 2, 8);
}

static inline void
unpack_ubyte_r10g10b10x2_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint16_t r = UNPACK(*src, 0, 10);
            uint16_t g = UNPACK(*src, 10, 10);
            uint16_t b = UNPACK(*src, 20, 10);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 10, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 10, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 10, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_r3g3b2_unorm(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 3);
            uint8_t g = UNPACK(*src, 3, 3);
            uint8_t b = UNPACK(*src, 6, 2);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 3, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 3, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 2, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_a4b4g4r4_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 4);
            uint8_t b = UNPACK(*src, 4, 4);
            uint8_t g = UNPACK(*src, 8, 4);
            uint8_t r = UNPACK(*src, 12, 4);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 4, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 4, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 4, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 4, 8);
}

static inline void
unpack_ubyte_r4g4b4a4_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 4);
            uint8_t g = UNPACK(*src, 4, 4);
            uint8_t b = UNPACK(*src, 8, 4);
            uint8_t a = UNPACK(*src, 12, 4);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 4, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 4, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 4, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 4, 8);
}

static inline void
unpack_ubyte_r5g5b5a1_unorm(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 5);
            uint8_t b = UNPACK(*src, 10, 5);
            uint8_t a = UNPACK(*src, 15, 1);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 5, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 5, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 5, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 1, 8);
}

static inline void
unpack_ubyte_a2b10g10r10_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 2);
            uint16_t b = UNPACK(*src, 2, 10);
            uint16_t g = UNPACK(*src, 12, 10);
            uint16_t r = UNPACK(*src, 22, 10);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 10, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 10, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 10, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 2, 8);
}

static inline void
unpack_ubyte_a2r10g10b10_unorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 2);
            uint16_t r = UNPACK(*src, 2, 10);
            uint16_t g = UNPACK(*src, 12, 10);
            uint16_t b = UNPACK(*src, 22, 10);

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 10, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 10, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 10, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 2, 8);
}

static inline void
unpack_ubyte_a_unorm8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t a = src[0];

      
         dst[0] = 0;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_a_unorm16(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t a = src[0];

      
         dst[0] = 0;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 16, 8);
}

static inline void
unpack_ubyte_l_unorm8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = src[0];

      
         
               dst[0] = _mesa_unorm_to_unorm(l, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(l, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(l, 8, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_l_unorm16(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t l = src[0];

      
         
               dst[0] = _mesa_unorm_to_unorm(l, 16, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(l, 16, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(l, 16, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_la_unorm8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = src[0];
            uint8_t a = src[1];

      
         
               dst[0] = _mesa_unorm_to_unorm(l, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(l, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(l, 8, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_la_unorm16(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t l = src[0];
            uint16_t a = src[1];

      
         
               dst[0] = _mesa_unorm_to_unorm(l, 16, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(l, 16, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(l, 16, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 16, 8);
}

static inline void
unpack_ubyte_i_unorm8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t i = src[0];

      
         
               dst[0] = _mesa_unorm_to_unorm(i, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(i, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(i, 8, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(i, 8, 8);
}

static inline void
unpack_ubyte_i_unorm16(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t i = src[0];

      
         
               dst[0] = _mesa_unorm_to_unorm(i, 16, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(i, 16, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(i, 16, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(i, 16, 8);
}

static inline void
unpack_ubyte_r_unorm8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_r_unorm16(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 16, 8);
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_rg_unorm8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];
            uint8_t g = src[1];

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 8, 8);
      
         dst[2] = 0;
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_rg_unorm16(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 16, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 16, 8);
      
         dst[2] = 0;
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_bgr_unorm8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t b = src[0];
            uint8_t g = src[1];
            uint8_t r = src[2];

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 8, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_rgb_unorm8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];
            uint8_t g = src[1];
            uint8_t b = src[2];

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 8, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_rgba_unorm16(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];
            uint16_t b = src[2];
            uint16_t a = src[3];

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 16, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 16, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 16, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 16, 8);
}

static inline void
unpack_ubyte_rgbx_unorm16(const void *void_src, uint8_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];
            uint16_t b = src[2];

      
         
               dst[0] = _mesa_unorm_to_unorm(r, 16, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(g, 16, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(b, 16, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_a8b8g8r8_snorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            int8_t a = UNPACK(*src, 0, 8);
            int8_t b = UNPACK(*src, 8, 8);
            int8_t g = UNPACK(*src, 16, 8);
            int8_t r = UNPACK(*src, 24, 8);

      
         
            dst[0] = _mesa_snorm_to_unorm(r, 8, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(g, 8, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(b, 8, 8);
      
         
            dst[3] = _mesa_snorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_x8b8g8r8_snorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            int8_t b = UNPACK(*src, 8, 8);
            int8_t g = UNPACK(*src, 16, 8);
            int8_t r = UNPACK(*src, 24, 8);

      
         
            dst[0] = _mesa_snorm_to_unorm(r, 8, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(g, 8, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(b, 8, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_r8g8b8a8_snorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            int8_t r = UNPACK(*src, 0, 8);
            int8_t g = UNPACK(*src, 8, 8);
            int8_t b = UNPACK(*src, 16, 8);
            int8_t a = UNPACK(*src, 24, 8);

      
         
            dst[0] = _mesa_snorm_to_unorm(r, 8, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(g, 8, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(b, 8, 8);
      
         
            dst[3] = _mesa_snorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_r8g8b8x8_snorm(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            int8_t r = UNPACK(*src, 0, 8);
            int8_t g = UNPACK(*src, 8, 8);
            int8_t b = UNPACK(*src, 16, 8);

      
         
            dst[0] = _mesa_snorm_to_unorm(r, 8, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(g, 8, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(b, 8, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_a_snorm8(const void *void_src, uint8_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t a = src[0];

      
         dst[0] = 0;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         
            dst[3] = _mesa_snorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_a_snorm16(const void *void_src, uint8_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t a = src[0];

      
         dst[0] = 0;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         
            dst[3] = _mesa_snorm_to_unorm(a, 16, 8);
}

static inline void
unpack_ubyte_l_snorm8(const void *void_src, uint8_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t l = src[0];

      
         
            dst[0] = _mesa_snorm_to_unorm(l, 8, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(l, 8, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(l, 8, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_l_snorm16(const void *void_src, uint8_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t l = src[0];

      
         
            dst[0] = _mesa_snorm_to_unorm(l, 16, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(l, 16, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(l, 16, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_i_snorm8(const void *void_src, uint8_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t i = src[0];

      
         
            dst[0] = _mesa_snorm_to_unorm(i, 8, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(i, 8, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(i, 8, 8);
      
         
            dst[3] = _mesa_snorm_to_unorm(i, 8, 8);
}

static inline void
unpack_ubyte_i_snorm16(const void *void_src, uint8_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t i = src[0];

      
         
            dst[0] = _mesa_snorm_to_unorm(i, 16, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(i, 16, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(i, 16, 8);
      
         
            dst[3] = _mesa_snorm_to_unorm(i, 16, 8);
}

static inline void
unpack_ubyte_r_snorm8(const void *void_src, uint8_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t r = src[0];

      
         
            dst[0] = _mesa_snorm_to_unorm(r, 8, 8);
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_r_snorm16(const void *void_src, uint8_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];

      
         
            dst[0] = _mesa_snorm_to_unorm(r, 16, 8);
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_la_snorm8(const void *void_src, uint8_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t l = src[0];
            int8_t a = src[1];

      
         
            dst[0] = _mesa_snorm_to_unorm(l, 8, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(l, 8, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(l, 8, 8);
      
         
            dst[3] = _mesa_snorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_la_snorm16(const void *void_src, uint8_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t l = src[0];
            int16_t a = src[1];

      
         
            dst[0] = _mesa_snorm_to_unorm(l, 16, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(l, 16, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(l, 16, 8);
      
         
            dst[3] = _mesa_snorm_to_unorm(a, 16, 8);
}

static inline void
unpack_ubyte_rg_snorm8(const void *void_src, uint8_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t r = src[0];
            int8_t g = src[1];

      
         
            dst[0] = _mesa_snorm_to_unorm(r, 8, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(g, 8, 8);
      
         dst[2] = 0;
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_rg_snorm16(const void *void_src, uint8_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];

      
         
            dst[0] = _mesa_snorm_to_unorm(r, 16, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(g, 16, 8);
      
         dst[2] = 0;
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_rgb_snorm16(const void *void_src, uint8_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];
            int16_t b = src[2];

      
         
            dst[0] = _mesa_snorm_to_unorm(r, 16, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(g, 16, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(b, 16, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_rgba_snorm16(const void *void_src, uint8_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];
            int16_t b = src[2];
            int16_t a = src[3];

      
         
            dst[0] = _mesa_snorm_to_unorm(r, 16, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(g, 16, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(b, 16, 8);
      
         
            dst[3] = _mesa_snorm_to_unorm(a, 16, 8);
}

static inline void
unpack_ubyte_rgbx_snorm16(const void *void_src, uint8_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];
            int16_t b = src[2];

      
         
            dst[0] = _mesa_snorm_to_unorm(r, 16, 8);
      
         
            dst[1] = _mesa_snorm_to_unorm(g, 16, 8);
      
         
            dst[2] = _mesa_snorm_to_unorm(b, 16, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_a8b8g8r8_srgb(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 8);
            uint8_t b = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t r = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_to_linear_8unorm(r);
      
         
               
               dst[1] = util_format_srgb_to_linear_8unorm(g);
      
         
               
               dst[2] = util_format_srgb_to_linear_8unorm(b);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_b8g8r8a8_srgb(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t r = UNPACK(*src, 16, 8);
            uint8_t a = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_to_linear_8unorm(r);
      
         
               
               dst[1] = util_format_srgb_to_linear_8unorm(g);
      
         
               
               dst[2] = util_format_srgb_to_linear_8unorm(b);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_a8r8g8b8_srgb(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 8);
            uint8_t r = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t b = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_to_linear_8unorm(r);
      
         
               
               dst[1] = util_format_srgb_to_linear_8unorm(g);
      
         
               
               dst[2] = util_format_srgb_to_linear_8unorm(b);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_b8g8r8x8_srgb(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t r = UNPACK(*src, 16, 8);

      
         
               
               dst[0] = util_format_srgb_to_linear_8unorm(r);
      
         
               
               dst[1] = util_format_srgb_to_linear_8unorm(g);
      
         
               
               dst[2] = util_format_srgb_to_linear_8unorm(b);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_x8r8g8b8_srgb(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t b = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_to_linear_8unorm(r);
      
         
               
               dst[1] = util_format_srgb_to_linear_8unorm(g);
      
         
               
               dst[2] = util_format_srgb_to_linear_8unorm(b);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_r8g8b8a8_srgb(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t b = UNPACK(*src, 16, 8);
            uint8_t a = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_to_linear_8unorm(r);
      
         
               
               dst[1] = util_format_srgb_to_linear_8unorm(g);
      
         
               
               dst[2] = util_format_srgb_to_linear_8unorm(b);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_r8g8b8x8_srgb(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t b = UNPACK(*src, 16, 8);

      
         
               
               dst[0] = util_format_srgb_to_linear_8unorm(r);
      
         
               
               dst[1] = util_format_srgb_to_linear_8unorm(g);
      
         
               
               dst[2] = util_format_srgb_to_linear_8unorm(b);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_x8b8g8r8_srgb(const void *void_src, uint8_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t r = UNPACK(*src, 24, 8);

      
         
               
               dst[0] = util_format_srgb_to_linear_8unorm(r);
      
         
               
               dst[1] = util_format_srgb_to_linear_8unorm(g);
      
         
               
               dst[2] = util_format_srgb_to_linear_8unorm(b);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_r_srgb8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];

      
         
               
               dst[0] = util_format_srgb_to_linear_8unorm(r);
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_l_srgb8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = src[0];

      
         
               dst[0] = _mesa_unorm_to_unorm(l, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(l, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(l, 8, 8);
      
         dst[3] = 255;
}

static inline void
unpack_ubyte_la_srgb8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = src[0];
            uint8_t a = src[1];

      
         
               dst[0] = _mesa_unorm_to_unorm(l, 8, 8);
      
         
               dst[1] = _mesa_unorm_to_unorm(l, 8, 8);
      
         
               dst[2] = _mesa_unorm_to_unorm(l, 8, 8);
      
         
               dst[3] = _mesa_unorm_to_unorm(a, 8, 8);
}

static inline void
unpack_ubyte_bgr_srgb8(const void *void_src, uint8_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t b = src[0];
            uint8_t g = src[1];
            uint8_t r = src[2];

      
         
               
               dst[0] = util_format_srgb_to_linear_8unorm(r);
      
         
               
               dst[1] = util_format_srgb_to_linear_8unorm(g);
      
         
               
               dst[2] = util_format_srgb_to_linear_8unorm(b);
      
         dst[3] = 255;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
/* integer packing functions */

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
static inline void
unpack_int_a8b8g8r8_uint(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 8);
            uint8_t b = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t r = UNPACK(*src, 24, 8);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_a8r8g8b8_uint(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 8);
            uint8_t r = UNPACK(*src, 8, 8);
            uint8_t g = UNPACK(*src, 16, 8);
            uint8_t b = UNPACK(*src, 24, 8);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_r8g8b8a8_uint(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t b = UNPACK(*src, 16, 8);
            uint8_t a = UNPACK(*src, 24, 8);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_b8g8r8a8_uint(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 8);
            uint8_t g = UNPACK(*src, 8, 8);
            uint8_t r = UNPACK(*src, 16, 8);
            uint8_t a = UNPACK(*src, 24, 8);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_b10g10r10a2_uint(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint16_t b = UNPACK(*src, 0, 10);
            uint16_t g = UNPACK(*src, 10, 10);
            uint16_t r = UNPACK(*src, 20, 10);
            uint8_t a = UNPACK(*src, 30, 2);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_r10g10b10a2_uint(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint16_t r = UNPACK(*src, 0, 10);
            uint16_t g = UNPACK(*src, 10, 10);
            uint16_t b = UNPACK(*src, 20, 10);
            uint8_t a = UNPACK(*src, 30, 2);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_a2b10g10r10_uint(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 2);
            uint16_t b = UNPACK(*src, 2, 10);
            uint16_t g = UNPACK(*src, 12, 10);
            uint16_t r = UNPACK(*src, 22, 10);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_a2r10g10b10_uint(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 2);
            uint16_t r = UNPACK(*src, 2, 10);
            uint16_t g = UNPACK(*src, 12, 10);
            uint16_t b = UNPACK(*src, 22, 10);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_b5g6r5_uint(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 6);
            uint8_t r = UNPACK(*src, 11, 5);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_r5g6b5_uint(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 6);
            uint8_t b = UNPACK(*src, 11, 5);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_b2g3r3_uint(const void *void_src, uint32_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 2);
            uint8_t g = UNPACK(*src, 2, 3);
            uint8_t r = UNPACK(*src, 5, 3);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_r3g3b2_uint(const void *void_src, uint32_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 3);
            uint8_t g = UNPACK(*src, 3, 3);
            uint8_t b = UNPACK(*src, 6, 2);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_a4b4g4r4_uint(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 4);
            uint8_t b = UNPACK(*src, 4, 4);
            uint8_t g = UNPACK(*src, 8, 4);
            uint8_t r = UNPACK(*src, 12, 4);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_r4g4b4a4_uint(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 4);
            uint8_t g = UNPACK(*src, 4, 4);
            uint8_t b = UNPACK(*src, 8, 4);
            uint8_t a = UNPACK(*src, 12, 4);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_b4g4r4a4_uint(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 4);
            uint8_t g = UNPACK(*src, 4, 4);
            uint8_t r = UNPACK(*src, 8, 4);
            uint8_t a = UNPACK(*src, 12, 4);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_a4r4g4b4_uint(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 4);
            uint8_t r = UNPACK(*src, 4, 4);
            uint8_t g = UNPACK(*src, 8, 4);
            uint8_t b = UNPACK(*src, 12, 4);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_a1b5g5r5_uint(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 1);
            uint8_t b = UNPACK(*src, 1, 5);
            uint8_t g = UNPACK(*src, 6, 5);
            uint8_t r = UNPACK(*src, 11, 5);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_b5g5r5a1_uint(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t b = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 5);
            uint8_t r = UNPACK(*src, 10, 5);
            uint8_t a = UNPACK(*src, 15, 1);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_a1r5g5b5_uint(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t a = UNPACK(*src, 0, 1);
            uint8_t r = UNPACK(*src, 1, 5);
            uint8_t g = UNPACK(*src, 6, 5);
            uint8_t b = UNPACK(*src, 11, 5);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_r5g5b5a1_uint(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint8_t r = UNPACK(*src, 0, 5);
            uint8_t g = UNPACK(*src, 5, 5);
            uint8_t b = UNPACK(*src, 10, 5);
            uint8_t a = UNPACK(*src, 15, 1);

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_a_uint8(const void *void_src, uint32_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t a = src[0];

      
         dst[0] = 0;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = a;
}

static inline void
unpack_int_a_uint16(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t a = src[0];

      
         dst[0] = 0;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = a;
}

static inline void
unpack_int_a_uint32(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint32_t a = src[0];

      
         dst[0] = 0;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = a;
}

static inline void
unpack_int_a_sint8(const void *void_src, uint32_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t a = src[0];

      
         dst[0] = 0;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = a;
}

static inline void
unpack_int_a_sint16(const void *void_src, uint32_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t a = src[0];

      
         dst[0] = 0;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = a;
}

static inline void
unpack_int_a_sint32(const void *void_src, uint32_t dst[4])
{
   int32_t *src = (int32_t *)void_src;
            int32_t a = src[0];

      
         dst[0] = 0;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = a;
}

static inline void
unpack_int_i_uint8(const void *void_src, uint32_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t i = src[0];

      
         dst[0] = i;
      
         dst[1] = i;
      
         dst[2] = i;
      
         dst[3] = i;
}

static inline void
unpack_int_i_uint16(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t i = src[0];

      
         dst[0] = i;
      
         dst[1] = i;
      
         dst[2] = i;
      
         dst[3] = i;
}

static inline void
unpack_int_i_uint32(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint32_t i = src[0];

      
         dst[0] = i;
      
         dst[1] = i;
      
         dst[2] = i;
      
         dst[3] = i;
}

static inline void
unpack_int_i_sint8(const void *void_src, uint32_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t i = src[0];

      
         dst[0] = i;
      
         dst[1] = i;
      
         dst[2] = i;
      
         dst[3] = i;
}

static inline void
unpack_int_i_sint16(const void *void_src, uint32_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t i = src[0];

      
         dst[0] = i;
      
         dst[1] = i;
      
         dst[2] = i;
      
         dst[3] = i;
}

static inline void
unpack_int_i_sint32(const void *void_src, uint32_t dst[4])
{
   int32_t *src = (int32_t *)void_src;
            int32_t i = src[0];

      
         dst[0] = i;
      
         dst[1] = i;
      
         dst[2] = i;
      
         dst[3] = i;
}

static inline void
unpack_int_l_uint8(const void *void_src, uint32_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = src[0];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = 1;
}

static inline void
unpack_int_l_uint16(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t l = src[0];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = 1;
}

static inline void
unpack_int_l_uint32(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint32_t l = src[0];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = 1;
}

static inline void
unpack_int_l_sint8(const void *void_src, uint32_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t l = src[0];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = 1;
}

static inline void
unpack_int_l_sint16(const void *void_src, uint32_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t l = src[0];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = 1;
}

static inline void
unpack_int_l_sint32(const void *void_src, uint32_t dst[4])
{
   int32_t *src = (int32_t *)void_src;
            int32_t l = src[0];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = 1;
}

static inline void
unpack_int_la_uint8(const void *void_src, uint32_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t l = src[0];
            uint8_t a = src[1];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = a;
}

static inline void
unpack_int_la_uint16(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t l = src[0];
            uint16_t a = src[1];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = a;
}

static inline void
unpack_int_la_uint32(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint32_t l = src[0];
            uint32_t a = src[1];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = a;
}

static inline void
unpack_int_la_sint8(const void *void_src, uint32_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t l = src[0];
            int8_t a = src[1];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = a;
}

static inline void
unpack_int_la_sint16(const void *void_src, uint32_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t l = src[0];
            int16_t a = src[1];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = a;
}

static inline void
unpack_int_la_sint32(const void *void_src, uint32_t dst[4])
{
   int32_t *src = (int32_t *)void_src;
            int32_t l = src[0];
            int32_t a = src[1];

      
         dst[0] = l;
      
         dst[1] = l;
      
         dst[2] = l;
      
         dst[3] = a;
}

static inline void
unpack_int_r_uint8(const void *void_src, uint32_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];

      
         dst[0] = r;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_r_uint16(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];

      
         dst[0] = r;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_r_uint32(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint32_t r = src[0];

      
         dst[0] = r;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_r_sint8(const void *void_src, uint32_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t r = src[0];

      
         dst[0] = r;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_r_sint16(const void *void_src, uint32_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];

      
         dst[0] = r;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_r_sint32(const void *void_src, uint32_t dst[4])
{
   int32_t *src = (int32_t *)void_src;
            int32_t r = src[0];

      
         dst[0] = r;
      
         dst[1] = 0;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_rg_uint8(const void *void_src, uint32_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];
            uint8_t g = src[1];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_rg_uint16(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_rg_uint32(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint32_t r = src[0];
            uint32_t g = src[1];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_rg_sint8(const void *void_src, uint32_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t r = src[0];
            int8_t g = src[1];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_rg_sint16(const void *void_src, uint32_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_rg_sint32(const void *void_src, uint32_t dst[4])
{
   int32_t *src = (int32_t *)void_src;
            int32_t r = src[0];
            int32_t g = src[1];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = 0;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgb_uint8(const void *void_src, uint32_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];
            uint8_t g = src[1];
            uint8_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgb_uint16(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];
            uint16_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgb_uint32(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint32_t r = src[0];
            uint32_t g = src[1];
            uint32_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgb_sint8(const void *void_src, uint32_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t r = src[0];
            int8_t g = src[1];
            int8_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgb_sint16(const void *void_src, uint32_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];
            int16_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgb_sint32(const void *void_src, uint32_t dst[4])
{
   int32_t *src = (int32_t *)void_src;
            int32_t r = src[0];
            int32_t g = src[1];
            int32_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgba_uint16(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];
            uint16_t b = src[2];
            uint16_t a = src[3];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_rgba_uint32(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint32_t r = src[0];
            uint32_t g = src[1];
            uint32_t b = src[2];
            uint32_t a = src[3];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_rgba_sint8(const void *void_src, uint32_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t r = src[0];
            int8_t g = src[1];
            int8_t b = src[2];
            int8_t a = src[3];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_rgba_sint16(const void *void_src, uint32_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];
            int16_t b = src[2];
            int16_t a = src[3];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_rgba_sint32(const void *void_src, uint32_t dst[4])
{
   int32_t *src = (int32_t *)void_src;
            int32_t r = src[0];
            int32_t g = src[1];
            int32_t b = src[2];
            int32_t a = src[3];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = a;
}

static inline void
unpack_int_rgbx_uint8(const void *void_src, uint32_t dst[4])
{
   uint8_t *src = (uint8_t *)void_src;
            uint8_t r = src[0];
            uint8_t g = src[1];
            uint8_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgbx_uint16(const void *void_src, uint32_t dst[4])
{
   uint16_t *src = (uint16_t *)void_src;
            uint16_t r = src[0];
            uint16_t g = src[1];
            uint16_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgbx_uint32(const void *void_src, uint32_t dst[4])
{
   uint32_t *src = (uint32_t *)void_src;
            uint32_t r = src[0];
            uint32_t g = src[1];
            uint32_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgbx_sint8(const void *void_src, uint32_t dst[4])
{
   int8_t *src = (int8_t *)void_src;
            int8_t r = src[0];
            int8_t g = src[1];
            int8_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgbx_sint16(const void *void_src, uint32_t dst[4])
{
   int16_t *src = (int16_t *)void_src;
            int16_t r = src[0];
            int16_t g = src[1];
            int16_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}

static inline void
unpack_int_rgbx_sint32(const void *void_src, uint32_t dst[4])
{
   int32_t *src = (int32_t *)void_src;
            int32_t r = src[0];
            int32_t g = src[1];
            int32_t b = src[2];

      
         dst[0] = r;
      
         dst[1] = g;
      
         dst[2] = b;
      
         dst[3] = 1;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        

void
_mesa_unpack_rgba_row(mesa_format format, uint32_t n,
                      const void *src, float dst[][4])
{
   uint8_t *s = (uint8_t *)src;
   uint32_t i;

   switch (format) {
   case MESA_FORMAT_A8B8G8R8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_a8b8g8r8_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_X8B8G8R8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_x8b8g8r8_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R8G8B8A8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_r8g8b8a8_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R8G8B8X8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_r8g8b8x8_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_B8G8R8A8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_b8g8r8a8_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_B8G8R8X8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_b8g8r8x8_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_A8R8G8B8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_a8r8g8b8_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_X8R8G8B8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_x8r8g8b8_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_B5G6R5_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_b5g6r5_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_R5G6B5_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_r5g6b5_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_B4G4R4A4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_b4g4r4a4_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_B4G4R4X4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_b4g4r4x4_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_A4R4G4B4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_a4r4g4b4_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_A1B5G5R5_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_a1b5g5r5_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_X1B5G5R5_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_x1b5g5r5_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_B5G5R5A1_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_b5g5r5a1_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_B5G5R5X1_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_b5g5r5x1_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_A1R5G5B5_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_a1r5g5b5_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_L4A4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_l4a4_unorm(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_B2G3R3_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_b2g3r3_unorm(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_B10G10R10A2_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_b10g10r10a2_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_B10G10R10X2_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_b10g10r10x2_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R10G10B10A2_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_r10g10b10a2_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R10G10B10X2_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_r10g10b10x2_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R3G3B2_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_r3g3b2_unorm(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_A4B4G4R4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_a4b4g4r4_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_R4G4B4A4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_r4g4b4a4_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_R5G5B5A1_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_r5g5b5a1_unorm(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_A2B10G10R10_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_a2b10g10r10_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_A2R10G10B10_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_a2r10g10b10_unorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_A_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_a_unorm8(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_A_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_a_unorm16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_L_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_l_unorm8(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_L_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_l_unorm16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_LA_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_la_unorm8(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_LA_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_la_unorm16(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_I_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_i_unorm8(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_I_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_i_unorm16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_R_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_r_unorm8(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_R_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_r_unorm16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_RG_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_rg_unorm8(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_RG_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_rg_unorm16(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_BGR_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_bgr_unorm8(s, dst[i]);
         s += 3;
      }
      break;
   case MESA_FORMAT_RGB_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_rgb_unorm8(s, dst[i]);
         s += 3;
      }
      break;
   case MESA_FORMAT_RGBA_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_rgba_unorm16(s, dst[i]);
         s += 8;
      }
      break;
   case MESA_FORMAT_RGBX_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_rgbx_unorm16(s, dst[i]);
         s += 8;
      }
      break;
   case MESA_FORMAT_A8B8G8R8_SNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_a8b8g8r8_snorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_X8B8G8R8_SNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_x8b8g8r8_snorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R8G8B8A8_SNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_r8g8b8a8_snorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R8G8B8X8_SNORM:
      for (i = 0; i < n; ++i) {
         unpack_float_r8g8b8x8_snorm(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_A_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_a_snorm8(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_A_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_a_snorm16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_L_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_l_snorm8(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_L_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_l_snorm16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_I_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_i_snorm8(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_I_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_i_snorm16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_R_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_r_snorm8(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_R_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_r_snorm16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_LA_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_la_snorm8(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_LA_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_la_snorm16(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_RG_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_float_rg_snorm8(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_RG_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_rg_snorm16(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_RGB_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_rgb_snorm16(s, dst[i]);
         s += 6;
      }
      break;
   case MESA_FORMAT_RGBA_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_rgba_snorm16(s, dst[i]);
         s += 8;
      }
      break;
   case MESA_FORMAT_RGBX_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_float_rgbx_snorm16(s, dst[i]);
         s += 8;
      }
      break;
   case MESA_FORMAT_A8B8G8R8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_float_a8b8g8r8_srgb(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_B8G8R8A8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_float_b8g8r8a8_srgb(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_A8R8G8B8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_float_a8r8g8b8_srgb(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_B8G8R8X8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_float_b8g8r8x8_srgb(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_X8R8G8B8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_float_x8r8g8b8_srgb(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R8G8B8A8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_float_r8g8b8a8_srgb(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R8G8B8X8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_float_r8g8b8x8_srgb(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_X8B8G8R8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_float_x8b8g8r8_srgb(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R_SRGB8:
      for (i = 0; i < n; ++i) {
         unpack_float_r_srgb8(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_L_SRGB8:
      for (i = 0; i < n; ++i) {
         unpack_float_l_srgb8(s, dst[i]);
         s += 1;
      }
      break;
   case MESA_FORMAT_LA_SRGB8:
      for (i = 0; i < n; ++i) {
         unpack_float_la_srgb8(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_BGR_SRGB8:
      for (i = 0; i < n; ++i) {
         unpack_float_bgr_srgb8(s, dst[i]);
         s += 3;
      }
      break;
   case MESA_FORMAT_R9G9B9E5_FLOAT:
      for (i = 0; i < n; ++i) {
         unpack_float_r9g9b9e5_float(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R11G11B10_FLOAT:
      for (i = 0; i < n; ++i) {
         unpack_float_r11g11b10_float(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_A_FLOAT16:
      for (i = 0; i < n; ++i) {
         unpack_float_a_float16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_A_FLOAT32:
      for (i = 0; i < n; ++i) {
         unpack_float_a_float32(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_L_FLOAT16:
      for (i = 0; i < n; ++i) {
         unpack_float_l_float16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_L_FLOAT32:
      for (i = 0; i < n; ++i) {
         unpack_float_l_float32(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_LA_FLOAT16:
      for (i = 0; i < n; ++i) {
         unpack_float_la_float16(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_LA_FLOAT32:
      for (i = 0; i < n; ++i) {
         unpack_float_la_float32(s, dst[i]);
         s += 8;
      }
      break;
   case MESA_FORMAT_I_FLOAT16:
      for (i = 0; i < n; ++i) {
         unpack_float_i_float16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_I_FLOAT32:
      for (i = 0; i < n; ++i) {
         unpack_float_i_float32(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_R_FLOAT16:
      for (i = 0; i < n; ++i) {
         unpack_float_r_float16(s, dst[i]);
         s += 2;
      }
      break;
   case MESA_FORMAT_R_FLOAT32:
      for (i = 0; i < n; ++i) {
         unpack_float_r_float32(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_RG_FLOAT16:
      for (i = 0; i < n; ++i) {
         unpack_float_rg_float16(s, dst[i]);
         s += 4;
      }
      break;
   case MESA_FORMAT_RG_FLOAT32:
      for (i = 0; i < n; ++i) {
         unpack_float_rg_float32(s, dst[i]);
         s += 8;
      }
      break;
   case MESA_FORMAT_RGB_FLOAT16:
      for (i = 0; i < n; ++i) {
         unpack_float_rgb_float16(s, dst[i]);
         s += 6;
      }
      break;
   case MESA_FORMAT_RGB_FLOAT32:
      for (i = 0; i < n; ++i) {
         unpack_float_rgb_float32(s, dst[i]);
         s += 12;
      }
      break;
   case MESA_FORMAT_RGBA_FLOAT16:
      for (i = 0; i < n; ++i) {
         unpack_float_rgba_float16(s, dst[i]);
         s += 8;
      }
      break;
   case MESA_FORMAT_RGBA_FLOAT32:
      for (i = 0; i < n; ++i) {
         unpack_float_rgba_float32(s, dst[i]);
         s += 16;
      }
      break;
   case MESA_FORMAT_RGBX_FLOAT16:
      for (i = 0; i < n; ++i) {
         unpack_float_rgbx_float16(s, dst[i]);
         s += 8;
      }
      break;
   case MESA_FORMAT_RGBX_FLOAT32:
      for (i = 0; i < n; ++i) {
         unpack_float_rgbx_float32(s, dst[i]);
         s += 16;
      }
      break;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 case MESA_FORMAT_YCBCR:
      unpack_float_ycbcr(src, dst, n);
      break;
   case MESA_FORMAT_YCBCR_REV:
      unpack_float_ycbcr_rev(src, dst, n);
      break;
   default:
      unreachable("bad format");
   }
}

void
_mesa_unpack_ubyte_rgba_row(mesa_format format, uint32_t n,
                            const void *src, uint8_t dst[][4])
{
   uint8_t *s = (uint8_t *)src;
   uint32_t i;

   switch (format) {

   case MESA_FORMAT_A8B8G8R8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a8b8g8r8_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_X8B8G8R8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_x8b8g8r8_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R8G8B8A8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r8g8b8a8_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R8G8B8X8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r8g8b8x8_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_B8G8R8A8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b8g8r8a8_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_B8G8R8X8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b8g8r8x8_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_A8R8G8B8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a8r8g8b8_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_X8R8G8B8_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_x8r8g8b8_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_B5G6R5_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b5g6r5_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_R5G6B5_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r5g6b5_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_B4G4R4A4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b4g4r4a4_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_B4G4R4X4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b4g4r4x4_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_A4R4G4B4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a4r4g4b4_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_A1B5G5R5_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a1b5g5r5_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_X1B5G5R5_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_x1b5g5r5_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_B5G5R5A1_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b5g5r5a1_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_B5G5R5X1_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b5g5r5x1_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_A1R5G5B5_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a1r5g5b5_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_L4A4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_l4a4_unorm(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_B2G3R3_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b2g3r3_unorm(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_B10G10R10A2_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b10g10r10a2_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_B10G10R10X2_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b10g10r10x2_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R10G10B10A2_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r10g10b10a2_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R10G10B10X2_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r10g10b10x2_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R3G3B2_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r3g3b2_unorm(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_A4B4G4R4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a4b4g4r4_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_R4G4B4A4_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r4g4b4a4_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_R5G5B5A1_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r5g5b5a1_unorm(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_A2B10G10R10_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a2b10g10r10_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_A2R10G10B10_UNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a2r10g10b10_unorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_A_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a_unorm8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_A_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a_unorm16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_L_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_l_unorm8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_L_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_l_unorm16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_LA_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_la_unorm8(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_LA_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_la_unorm16(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_I_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_i_unorm8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_I_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_i_unorm16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_R_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r_unorm8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_R_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r_unorm16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_RG_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_rg_unorm8(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_RG_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_rg_unorm16(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_BGR_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_bgr_unorm8(s, dst[i]);
         s += 3;
      }
      break;

   case MESA_FORMAT_RGB_UNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_rgb_unorm8(s, dst[i]);
         s += 3;
      }
      break;

   case MESA_FORMAT_RGBA_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_rgba_unorm16(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_RGBX_UNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_rgbx_unorm16(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_A8B8G8R8_SNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a8b8g8r8_snorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_X8B8G8R8_SNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_x8b8g8r8_snorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R8G8B8A8_SNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r8g8b8a8_snorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R8G8B8X8_SNORM:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r8g8b8x8_snorm(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_A_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a_snorm8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_A_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a_snorm16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_L_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_l_snorm8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_L_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_l_snorm16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_I_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_i_snorm8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_I_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_i_snorm16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_R_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r_snorm8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_R_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r_snorm16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_LA_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_la_snorm8(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_LA_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_la_snorm16(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_RG_SNORM8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_rg_snorm8(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_RG_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_rg_snorm16(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_RGB_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_rgb_snorm16(s, dst[i]);
         s += 6;
      }
      break;

   case MESA_FORMAT_RGBA_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_rgba_snorm16(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_RGBX_SNORM16:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_rgbx_snorm16(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_A8B8G8R8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a8b8g8r8_srgb(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_B8G8R8A8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b8g8r8a8_srgb(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_A8R8G8B8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_a8r8g8b8_srgb(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_B8G8R8X8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_b8g8r8x8_srgb(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_X8R8G8B8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_x8r8g8b8_srgb(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R8G8B8A8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r8g8b8a8_srgb(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R8G8B8X8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r8g8b8x8_srgb(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_X8B8G8R8_SRGB:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_x8b8g8r8_srgb(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R_SRGB8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_r_srgb8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_L_SRGB8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_l_srgb8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_LA_SRGB8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_la_srgb8(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_BGR_SRGB8:
      for (i = 0; i < n; ++i) {
         unpack_ubyte_bgr_srgb8(s, dst[i]);
         s += 3;
      }
      break;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         default:
      /* get float values, convert to ubyte */
      {
         float *tmp = malloc(n * 4 * sizeof(float));
         if (tmp) {
            uint32_t i;
            _mesa_unpack_rgba_row(format, n, src, (float (*)[4]) tmp);
            for (i = 0; i < n; i++) {
               dst[i][0] = _mesa_float_to_unorm(tmp[i*4+0], 8);
               dst[i][1] = _mesa_float_to_unorm(tmp[i*4+1], 8);
               dst[i][2] = _mesa_float_to_unorm(tmp[i*4+2], 8);
               dst[i][3] = _mesa_float_to_unorm(tmp[i*4+3], 8);
            }
            free(tmp);
         }
      }
      break;
   }
}

void
_mesa_unpack_uint_rgba_row(mesa_format format, uint32_t n,
                           const void *src, uint32_t dst[][4])
{
   uint8_t *s = (uint8_t *)src;
   uint32_t i;

   switch (format) {
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
   case MESA_FORMAT_A8B8G8R8_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_a8b8g8r8_uint(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_A8R8G8B8_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_a8r8g8b8_uint(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R8G8B8A8_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_r8g8b8a8_uint(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_B8G8R8A8_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_b8g8r8a8_uint(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_B10G10R10A2_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_b10g10r10a2_uint(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R10G10B10A2_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_r10g10b10a2_uint(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_A2B10G10R10_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_a2b10g10r10_uint(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_A2R10G10B10_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_a2r10g10b10_uint(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_B5G6R5_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_b5g6r5_uint(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_R5G6B5_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_r5g6b5_uint(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_B2G3R3_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_b2g3r3_uint(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_R3G3B2_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_r3g3b2_uint(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_A4B4G4R4_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_a4b4g4r4_uint(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_R4G4B4A4_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_r4g4b4a4_uint(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_B4G4R4A4_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_b4g4r4a4_uint(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_A4R4G4B4_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_a4r4g4b4_uint(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_A1B5G5R5_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_a1b5g5r5_uint(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_B5G5R5A1_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_b5g5r5a1_uint(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_A1R5G5B5_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_a1r5g5b5_uint(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_R5G5B5A1_UINT:
      for (i = 0; i < n; ++i) {
         unpack_int_r5g5b5a1_uint(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_A_UINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_a_uint8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_A_UINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_a_uint16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_A_UINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_a_uint32(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_A_SINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_a_sint8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_A_SINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_a_sint16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_A_SINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_a_sint32(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_I_UINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_i_uint8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_I_UINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_i_uint16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_I_UINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_i_uint32(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_I_SINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_i_sint8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_I_SINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_i_sint16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_I_SINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_i_sint32(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_L_UINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_l_uint8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_L_UINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_l_uint16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_L_UINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_l_uint32(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_L_SINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_l_sint8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_L_SINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_l_sint16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_L_SINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_l_sint32(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_LA_UINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_la_uint8(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_LA_UINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_la_uint16(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_LA_UINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_la_uint32(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_LA_SINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_la_sint8(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_LA_SINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_la_sint16(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_LA_SINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_la_sint32(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_R_UINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_r_uint8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_R_UINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_r_uint16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_R_UINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_r_uint32(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_R_SINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_r_sint8(s, dst[i]);
         s += 1;
      }
      break;

   case MESA_FORMAT_R_SINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_r_sint16(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_R_SINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_r_sint32(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_RG_UINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_rg_uint8(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_RG_UINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_rg_uint16(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_RG_UINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_rg_uint32(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_RG_SINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_rg_sint8(s, dst[i]);
         s += 2;
      }
      break;

   case MESA_FORMAT_RG_SINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_rg_sint16(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_RG_SINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_rg_sint32(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_RGB_UINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_rgb_uint8(s, dst[i]);
         s += 3;
      }
      break;

   case MESA_FORMAT_RGB_UINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_rgb_uint16(s, dst[i]);
         s += 6;
      }
      break;

   case MESA_FORMAT_RGB_UINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_rgb_uint32(s, dst[i]);
         s += 12;
      }
      break;

   case MESA_FORMAT_RGB_SINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_rgb_sint8(s, dst[i]);
         s += 3;
      }
      break;

   case MESA_FORMAT_RGB_SINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_rgb_sint16(s, dst[i]);
         s += 6;
      }
      break;

   case MESA_FORMAT_RGB_SINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_rgb_sint32(s, dst[i]);
         s += 12;
      }
      break;

   case MESA_FORMAT_RGBA_UINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_rgba_uint16(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_RGBA_UINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_rgba_uint32(s, dst[i]);
         s += 16;
      }
      break;

   case MESA_FORMAT_RGBA_SINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_rgba_sint8(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_RGBA_SINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_rgba_sint16(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_RGBA_SINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_rgba_sint32(s, dst[i]);
         s += 16;
      }
      break;

   case MESA_FORMAT_RGBX_UINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_rgbx_uint8(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_RGBX_UINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_rgbx_uint16(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_RGBX_UINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_rgbx_uint32(s, dst[i]);
         s += 16;
      }
      break;

   case MESA_FORMAT_RGBX_SINT8:
      for (i = 0; i < n; ++i) {
         unpack_int_rgbx_sint8(s, dst[i]);
         s += 4;
      }
      break;

   case MESA_FORMAT_RGBX_SINT16:
      for (i = 0; i < n; ++i) {
         unpack_int_rgbx_sint16(s, dst[i]);
         s += 8;
      }
      break;

   case MESA_FORMAT_RGBX_SINT32:
      for (i = 0; i < n; ++i) {
         unpack_int_rgbx_sint32(s, dst[i]);
         s += 16;
      }
      break;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           default:
      unreachable("bad format");
   }
}

/**
 * Unpack a 2D rect of pixels returning float RGBA colors.
 * \param format  the source image format
 * \param src  start address of the source image
 * \param srcRowStride  source image row stride in bytes
 * \param dst  start address of the dest image
 * \param dstRowStride  dest image row stride in bytes
 * \param x  source image start X pos
 * \param y  source image start Y pos
 * \param width  width of rect region to convert
 * \param height  height of rect region to convert
 */
void
_mesa_unpack_rgba_block(mesa_format format,
                        const void *src, int32_t srcRowStride,
                        float dst[][4], int32_t dstRowStride,
                        uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
   const uint32_t srcPixStride = _mesa_get_format_bytes(format);
   const uint32_t dstPixStride = 4 * sizeof(float);
   const uint8_t *srcRow;
   uint8_t *dstRow;
   uint32_t i;

   /* XXX needs to be fixed for compressed formats */

   srcRow = ((const uint8_t *) src) + srcRowStride * y + srcPixStride * x;
   dstRow = ((uint8_t *) dst) + dstRowStride * y + dstPixStride * x;

   for (i = 0; i < height; i++) {
      _mesa_unpack_rgba_row(format, width, srcRow, (float (*)[4]) dstRow);

      dstRow += dstRowStride;
      srcRow += srcRowStride;
   }
}

/** Helper struct for MESA_FORMAT_Z32_FLOAT_S8X24_UINT */
struct z32f_x24s8
{
   float z;
   uint32_t x24s8;
};

typedef void (*unpack_float_z_func)(uint32_t n, const void *src, float *dst);

static void
unpack_float_z_X8_UINT_Z24_UNORM(uint32_t n, const void *src, float *dst)
{
   /* only return Z, not stencil data */
   const uint32_t *s = ((const uint32_t *) src);
   const double scale = 1.0 / (double) 0xffffff;
   uint32_t i;
   for (i = 0; i < n; i++) {
      dst[i] = (float) ((s[i] >> 8) * scale);
      assert(dst[i] >= 0.0F);
      assert(dst[i] <= 1.0F);
   }
}

static void
unpack_float_z_Z24_UNORM_X8_UINT(uint32_t n, const void *src, float *dst)
{
   /* only return Z, not stencil data */
   const uint32_t *s = ((const uint32_t *) src);
   const double scale = 1.0 / (double) 0xffffff;
   uint32_t i;
   for (i = 0; i < n; i++) {
      dst[i] = (float) ((s[i] & 0x00ffffff) * scale);
      assert(dst[i] >= 0.0F);
      assert(dst[i] <= 1.0F);
   }
}

static void
unpack_float_Z_UNORM16(uint32_t n, const void *src, float *dst)
{
   const uint16_t *s = ((const uint16_t *) src);
   uint32_t i;
   for (i = 0; i < n; i++) {
      dst[i] = s[i] * (1.0F / 65535.0F);
   }
}

static void
unpack_float_Z_UNORM32(uint32_t n, const void *src, float *dst)
{
   const uint32_t *s = ((const uint32_t *) src);
   uint32_t i;
   for (i = 0; i < n; i++) {
      dst[i] = s[i] * (1.0F / 0xffffffff);
   }
}

static void
unpack_float_Z_FLOAT32(uint32_t n, const void *src, float *dst)
{
   memcpy(dst, src, n * sizeof(float));
}

static void
unpack_float_z_Z32X24S8(uint32_t n, const void *src, float *dst)
{
   const struct z32f_x24s8 *s = (const struct z32f_x24s8 *) src;
   uint32_t i;
   for (i = 0; i < n; i++) {
      dst[i] = s[i].z;
   }
}



/**
 * Unpack Z values.
 * The returned values will always be in the range [0.0, 1.0].
 */
void
_mesa_unpack_float_z_row(mesa_format format, uint32_t n,
                         const void *src, float *dst)
{
   unpack_float_z_func unpack;

   switch (format) {
   case MESA_FORMAT_S8_UINT_Z24_UNORM:
   case MESA_FORMAT_X8_UINT_Z24_UNORM:
      unpack = unpack_float_z_X8_UINT_Z24_UNORM;
      break;
   case MESA_FORMAT_Z24_UNORM_S8_UINT:
   case MESA_FORMAT_Z24_UNORM_X8_UINT:
      unpack = unpack_float_z_Z24_UNORM_X8_UINT;
      break;
   case MESA_FORMAT_Z_UNORM16:
      unpack = unpack_float_Z_UNORM16;
      break;
   case MESA_FORMAT_Z_UNORM32:
      unpack = unpack_float_Z_UNORM32;
      break;
   case MESA_FORMAT_Z_FLOAT32:
      unpack = unpack_float_Z_FLOAT32;
      break;
   case MESA_FORMAT_Z32_FLOAT_S8X24_UINT:
      unpack = unpack_float_z_Z32X24S8;
      break;
   default:
      unreachable("bad format in _mesa_unpack_float_z_row");
   }

   unpack(n, src, dst);
}



typedef void (*unpack_uint_z_func)(const void *src, uint32_t *dst, uint32_t n);

static void
unpack_uint_z_X8_UINT_Z24_UNORM(const void *src, uint32_t *dst, uint32_t n)
{
   /* only return Z, not stencil data */
   const uint32_t *s = ((const uint32_t *) src);
   uint32_t i;
   for (i = 0; i < n; i++) {
      dst[i] = (s[i] & 0xffffff00) | (s[i] >> 24);
   }
}

static void
unpack_uint_z_Z24_UNORM_X8_UINT(const void *src, uint32_t *dst, uint32_t n)
{
   /* only return Z, not stencil data */
   const uint32_t *s = ((const uint32_t *) src);
   uint32_t i;
   for (i = 0; i < n; i++) {
      dst[i] = (s[i] << 8) | ((s[i] >> 16) & 0xff);
   }
}

static void
unpack_uint_Z_UNORM16(const void *src, uint32_t *dst, uint32_t n)
{
   const uint16_t *s = ((const uint16_t *)src);
   uint32_t i;
   for (i = 0; i < n; i++) {
      dst[i] = (s[i] << 16) | s[i];
   }
}

static void
unpack_uint_Z_UNORM32(const void *src, uint32_t *dst, uint32_t n)
{
   memcpy(dst, src, n * sizeof(uint32_t));
}

static void
unpack_uint_Z_FLOAT32(const void *src, uint32_t *dst, uint32_t n)
{
   const float *s = (const float *)src;
   uint32_t i;
   for (i = 0; i < n; i++) {
      dst[i] = FLOAT_TO_UINT(CLAMP(s[i], 0.0F, 1.0F));
   }
}

static void
unpack_uint_Z_FLOAT32_X24S8(const void *src, uint32_t *dst, uint32_t n)
{
   const struct z32f_x24s8 *s = (const struct z32f_x24s8 *) src;
   uint32_t i;

   for (i = 0; i < n; i++) {
      dst[i] = FLOAT_TO_UINT(CLAMP(s[i].z, 0.0F, 1.0F));
   }
}


/**
 * Unpack Z values.
 * The returned values will always be in the range [0, 0xffffffff].
 */
void
_mesa_unpack_uint_z_row(mesa_format format, uint32_t n,
                        const void *src, uint32_t *dst)
{
   unpack_uint_z_func unpack;
   const uint8_t *srcPtr = (uint8_t *) src;

   switch (format) {
   case MESA_FORMAT_S8_UINT_Z24_UNORM:
   case MESA_FORMAT_X8_UINT_Z24_UNORM:
      unpack = unpack_uint_z_X8_UINT_Z24_UNORM;
      break;
   case MESA_FORMAT_Z24_UNORM_S8_UINT:
   case MESA_FORMAT_Z24_UNORM_X8_UINT:
      unpack = unpack_uint_z_Z24_UNORM_X8_UINT;
      break;
   case MESA_FORMAT_Z_UNORM16:
      unpack = unpack_uint_Z_UNORM16;
      break;
   case MESA_FORMAT_Z_UNORM32:
      unpack = unpack_uint_Z_UNORM32;
      break;
   case MESA_FORMAT_Z_FLOAT32:
      unpack = unpack_uint_Z_FLOAT32;
      break;
   case MESA_FORMAT_Z32_FLOAT_S8X24_UINT:
      unpack = unpack_uint_Z_FLOAT32_X24S8;
      break;
   default:
      unreachable("bad format %s in _mesa_unpack_uint_z_row");
   }

   unpack(srcPtr, dst, n);
}


static void
unpack_ubyte_s_S_UINT8(const void *src, uint8_t *dst, uint32_t n)
{
   memcpy(dst, src, n);
}

static void
unpack_ubyte_s_S8_UINT_Z24_UNORM(const void *src, uint8_t *dst, uint32_t n)
{
   uint32_t i;
   const uint32_t *src32 = src;

   for (i = 0; i < n; i++)
      dst[i] = src32[i] & 0xff;
}

static void
unpack_ubyte_s_Z24_UNORM_S8_UINT(const void *src, uint8_t *dst, uint32_t n)
{
   uint32_t i;
   const uint32_t *src32 = src;

   for (i = 0; i < n; i++)
      dst[i] = src32[i] >> 24;
}

static void
unpack_ubyte_s_Z32_FLOAT_S8X24_UINT(const void *src, uint8_t *dst, uint32_t n)
{
   uint32_t i;
   const struct z32f_x24s8 *s = (const struct z32f_x24s8 *) src;

   for (i = 0; i < n; i++)
      dst[i] = s[i].x24s8 & 0xff;
}

void
_mesa_unpack_ubyte_stencil_row(mesa_format format, uint32_t n,
			       const void *src, uint8_t *dst)
{
   switch (format) {
   case MESA_FORMAT_S_UINT8:
      unpack_ubyte_s_S_UINT8(src, dst, n);
      break;
   case MESA_FORMAT_S8_UINT_Z24_UNORM:
      unpack_ubyte_s_S8_UINT_Z24_UNORM(src, dst, n);
      break;
   case MESA_FORMAT_Z24_UNORM_S8_UINT:
      unpack_ubyte_s_Z24_UNORM_S8_UINT(src, dst, n);
      break;
   case MESA_FORMAT_Z32_FLOAT_S8X24_UINT:
      unpack_ubyte_s_Z32_FLOAT_S8X24_UINT(src, dst, n);
      break;
   default:
      unreachable("bad format %s in _mesa_unpack_ubyte_s_row");
   }
}

static void
unpack_uint_24_8_depth_stencil_Z24_UNORM_S8_UINT(const uint32_t *src, uint32_t *dst, uint32_t n)
{
   uint32_t i;

   for (i = 0; i < n; i++) {
      uint32_t val = src[i];
      dst[i] = val >> 24 | val << 8;
   }
}

static void
unpack_uint_24_8_depth_stencil_Z32_S8X24(const uint32_t *src,
                                         uint32_t *dst, uint32_t n)
{
   uint32_t i;

   for (i = 0; i < n; i++) {
      /* 8 bytes per pixel (float + uint32) */
      float zf = ((float *) src)[i * 2 + 0];
      uint32_t z24 = (uint32_t) (zf * (float) 0xffffff);
      uint32_t s = src[i * 2 + 1] & 0xff;
      dst[i] = (z24 << 8) | s;
   }
}

static void
unpack_uint_24_8_depth_stencil_S8_UINT_Z24_UNORM(const uint32_t *src, uint32_t *dst, uint32_t n)
{
   memcpy(dst, src, n * 4);
}

/**
 * Unpack depth/stencil returning as GL_UNSIGNED_INT_24_8.
 * \param format  the source data format
 */
void
_mesa_unpack_uint_24_8_depth_stencil_row(mesa_format format, uint32_t n,
					 const void *src, uint32_t *dst)
{
   switch (format) {
   case MESA_FORMAT_S8_UINT_Z24_UNORM:
      unpack_uint_24_8_depth_stencil_S8_UINT_Z24_UNORM(src, dst, n);
      break;
   case MESA_FORMAT_Z24_UNORM_S8_UINT:
      unpack_uint_24_8_depth_stencil_Z24_UNORM_S8_UINT(src, dst, n);
      break;
   case MESA_FORMAT_Z32_FLOAT_S8X24_UINT:
      unpack_uint_24_8_depth_stencil_Z32_S8X24(src, dst, n);
      break;
   default:
      unreachable("bad format %s in _mesa_unpack_uint_24_8_depth_stencil_row");
   }
}

static void
unpack_float_32_uint_24_8_Z24_UNORM_S8_UINT(const uint32_t *src,
                                            uint32_t *dst, uint32_t n)
{
   uint32_t i;
   struct z32f_x24s8 *d = (struct z32f_x24s8 *) dst;
   const double scale = 1.0 / (double) 0xffffff;

   for (i = 0; i < n; i++) {
      const uint32_t z24 = src[i] & 0xffffff;
      d[i].z = z24 * scale;
      d[i].x24s8 = src[i] >> 24;
      assert(d[i].z >= 0.0f);
      assert(d[i].z <= 1.0f);
   }
}

static void
unpack_float_32_uint_24_8_Z32_FLOAT_S8X24_UINT(const uint32_t *src,
                                               uint32_t *dst, uint32_t n)
{
   memcpy(dst, src, n * sizeof(struct z32f_x24s8));
}

static void
unpack_float_32_uint_24_8_S8_UINT_Z24_UNORM(const uint32_t *src,
                                            uint32_t *dst, uint32_t n)
{
   uint32_t i;
   struct z32f_x24s8 *d = (struct z32f_x24s8 *) dst;
   const double scale = 1.0 / (double) 0xffffff;

   for (i = 0; i < n; i++) {
      const uint32_t z24 = src[i] >> 8;
      d[i].z = z24 * scale;
      d[i].x24s8 = src[i] & 0xff;
      assert(d[i].z >= 0.0f);
      assert(d[i].z <= 1.0f);
   }
}

/**
 * Unpack depth/stencil returning as GL_FLOAT_32_UNSIGNED_INT_24_8_REV.
 * \param format  the source data format
 *
 * In GL_FLOAT_32_UNSIGNED_INT_24_8_REV lower 4 bytes contain float
 * component and higher 4 bytes contain packed 24-bit and 8-bit
 * components.
 *
 *    31 30 29 28 ... 4 3 2 1 0    31 30 29 ... 9 8 7 6 5 ... 2 1 0
 *    +-------------------------+  +--------------------------------+
 *    |    Float Component      |  | Unused         | 8 bit stencil |
 *    +-------------------------+  +--------------------------------+
 *          lower 4 bytes                  higher 4 bytes
 */
void
_mesa_unpack_float_32_uint_24_8_depth_stencil_row(mesa_format format, uint32_t n,
			                          const void *src, uint32_t *dst)
{
   switch (format) {
   case MESA_FORMAT_S8_UINT_Z24_UNORM:
      unpack_float_32_uint_24_8_S8_UINT_Z24_UNORM(src, dst, n);
      break;
   case MESA_FORMAT_Z24_UNORM_S8_UINT:
      unpack_float_32_uint_24_8_Z24_UNORM_S8_UINT(src, dst, n);
      break;
   case MESA_FORMAT_Z32_FLOAT_S8X24_UINT:
      unpack_float_32_uint_24_8_Z32_FLOAT_S8X24_UINT(src, dst, n);
      break;
   default:
      unreachable("bad format %s in _mesa_unpack_uint_24_8_depth_stencil_row");
   }
}


