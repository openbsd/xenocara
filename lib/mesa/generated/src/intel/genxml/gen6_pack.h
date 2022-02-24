/*
 * Copyright (C) 2016 Intel Corporation
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


/* Instructions, enums and structures for SNB.
 *
 * This file has been generated, do not hand edit.
 */

#ifndef GFX6_PACK_H
#define GFX6_PACK_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

#ifndef __gen_validate_value
#define __gen_validate_value(x)
#endif

#ifndef __intel_field_functions
#define __intel_field_functions

#ifdef NDEBUG
#define NDEBUG_UNUSED __attribute__((unused))
#else
#define NDEBUG_UNUSED
#endif

union __intel_value {
   float f;
   uint32_t dw;
};

static inline __attribute__((always_inline)) uint64_t
__gen_mbo(uint32_t start, uint32_t end)
{
   return (~0ull >> (64 - (end - start + 1))) << start;
}

static inline __attribute__((always_inline)) uint64_t
__gen_uint(uint64_t v, uint32_t start, NDEBUG_UNUSED uint32_t end)
{
   __gen_validate_value(v);

#ifndef NDEBUG
   const int width = end - start + 1;
   if (width < 64) {
      const uint64_t max = (1ull << width) - 1;
      assert(v <= max);
   }
#endif

   return v << start;
}

static inline __attribute__((always_inline)) uint64_t
__gen_sint(int64_t v, uint32_t start, uint32_t end)
{
   const int width = end - start + 1;

   __gen_validate_value(v);

#ifndef NDEBUG
   if (width < 64) {
      const int64_t max = (1ll << (width - 1)) - 1;
      const int64_t min = -(1ll << (width - 1));
      assert(min <= v && v <= max);
   }
#endif

   const uint64_t mask = ~0ull >> (64 - width);

   return (v & mask) << start;
}

static inline __attribute__((always_inline)) uint64_t
__gen_offset(uint64_t v, NDEBUG_UNUSED uint32_t start, NDEBUG_UNUSED uint32_t end)
{
   __gen_validate_value(v);
#ifndef NDEBUG
   uint64_t mask = (~0ull >> (64 - (end - start + 1))) << start;

   assert((v & ~mask) == 0);
#endif

   return v;
}

static inline __attribute__((always_inline)) uint64_t
__gen_address(__gen_user_data *data, void *location,
              __gen_address_type address, uint32_t delta,
              __attribute__((unused)) uint32_t start, uint32_t end)
{
   uint64_t addr_u64 = __gen_combine_address(data, location, address, delta);
   if (end == 31) {
      return addr_u64;
   } else if (end < 63) {
      const unsigned shift = 63 - end;
      return (addr_u64 << shift) >> shift;
   } else {
      return addr_u64;
   }
}

static inline __attribute__((always_inline)) uint32_t
__gen_float(float v)
{
   __gen_validate_value(v);
   return ((union __intel_value) { .f = (v) }).dw;
}

static inline __attribute__((always_inline)) uint64_t
__gen_sfixed(float v, uint32_t start, uint32_t end, uint32_t fract_bits)
{
   __gen_validate_value(v);

   const float factor = (1 << fract_bits);

#ifndef NDEBUG
   const float max = ((1 << (end - start)) - 1) / factor;
   const float min = -(1 << (end - start)) / factor;
   assert(min <= v && v <= max);
#endif

   const int64_t int_val = llroundf(v * factor);
   const uint64_t mask = ~0ull >> (64 - (end - start + 1));

   return (int_val & mask) << start;
}

static inline __attribute__((always_inline)) uint64_t
__gen_ufixed(float v, uint32_t start, NDEBUG_UNUSED uint32_t end, uint32_t fract_bits)
{
   __gen_validate_value(v);

   const float factor = (1 << fract_bits);

#ifndef NDEBUG
   const float max = ((1 << (end - start + 1)) - 1) / factor;
   const float min = 0.0f;
   assert(min <= v && v <= max);
#endif

   const uint64_t uint_val = llroundf(v * factor);

   return uint_val << start;
}

#ifndef __gen_address_type
#error #define __gen_address_type before including this file
#endif

#ifndef __gen_user_data
#error #define __gen_combine_address before including this file
#endif

#undef NDEBUG_UNUSED

#endif


enum GFX6_3D_Color_Buffer_Blend_Factor {
   BLENDFACTOR_ONE                      =      1,
   BLENDFACTOR_SRC_COLOR                =      2,
   BLENDFACTOR_SRC_ALPHA                =      3,
   BLENDFACTOR_DST_ALPHA                =      4,
   BLENDFACTOR_DST_COLOR                =      5,
   BLENDFACTOR_SRC_ALPHA_SATURATE       =      6,
   BLENDFACTOR_CONST_COLOR              =      7,
   BLENDFACTOR_CONST_ALPHA              =      8,
   BLENDFACTOR_SRC1_COLOR               =      9,
   BLENDFACTOR_SRC1_ALPHA               =     10,
   BLENDFACTOR_ZERO                     =     17,
   BLENDFACTOR_INV_SRC_COLOR            =     18,
   BLENDFACTOR_INV_SRC_ALPHA            =     19,
   BLENDFACTOR_INV_DST_ALPHA            =     20,
   BLENDFACTOR_INV_DST_COLOR            =     21,
   BLENDFACTOR_INV_CONST_COLOR          =     23,
   BLENDFACTOR_INV_CONST_ALPHA          =     24,
   BLENDFACTOR_INV_SRC1_COLOR           =     25,
   BLENDFACTOR_INV_SRC1_ALPHA           =     26,
};

enum GFX6_3D_Color_Buffer_Blend_Function {
   BLENDFUNCTION_ADD                    =      0,
   BLENDFUNCTION_SUBTRACT               =      1,
   BLENDFUNCTION_REVERSE_SUBTRACT       =      2,
   BLENDFUNCTION_MIN                    =      3,
   BLENDFUNCTION_MAX                    =      4,
};

enum GFX6_3D_Compare_Function {
   COMPAREFUNCTION_ALWAYS               =      0,
   COMPAREFUNCTION_NEVER                =      1,
   COMPAREFUNCTION_LESS                 =      2,
   COMPAREFUNCTION_EQUAL                =      3,
   COMPAREFUNCTION_LEQUAL               =      4,
   COMPAREFUNCTION_GREATER              =      5,
   COMPAREFUNCTION_NOTEQUAL             =      6,
   COMPAREFUNCTION_GEQUAL               =      7,
};

enum GFX6_3D_Logic_Op_Function {
   LOGICOP_CLEAR                        =      0,
   LOGICOP_NOR                          =      1,
   LOGICOP_AND_INVERTED                 =      2,
   LOGICOP_COPY_INVERTED                =      3,
   LOGICOP_AND_REVERSE                  =      4,
   LOGICOP_INVERT                       =      5,
   LOGICOP_XOR                          =      6,
   LOGICOP_NAND                         =      7,
   LOGICOP_AND                          =      8,
   LOGICOP_EQUIV                        =      9,
   LOGICOP_NOOP                         =     10,
   LOGICOP_OR_INVERTED                  =     11,
   LOGICOP_COPY                         =     12,
   LOGICOP_OR_REVERSE                   =     13,
   LOGICOP_OR                           =     14,
   LOGICOP_SET                          =     15,
};

enum GFX6_3D_Prim_Topo_Type {
   _3DPRIM_POINTLIST                    =      1,
   _3DPRIM_LINELIST                     =      2,
   _3DPRIM_LINESTRIP                    =      3,
   _3DPRIM_TRILIST                      =      4,
   _3DPRIM_TRISTRIP                     =      5,
   _3DPRIM_TRIFAN                       =      6,
   _3DPRIM_QUADLIST                     =      7,
   _3DPRIM_QUADSTRIP                    =      8,
   _3DPRIM_LINELIST_ADJ                 =      9,
   _3DPRIM_LINESTRIP_ADJ                =     10,
   _3DPRIM_TRILIST_ADJ                  =     11,
   _3DPRIM_TRISTRIP_ADJ                 =     12,
   _3DPRIM_TRISTRIP_REVERSE             =     13,
   _3DPRIM_POLYGON                      =     14,
   _3DPRIM_RECTLIST                     =     15,
   _3DPRIM_LINELOOP                     =     16,
   _3DPRIM_POINTLIST_BF                 =     17,
   _3DPRIM_LINESTRIP_CONT               =     18,
   _3DPRIM_LINESTRIP_BF                 =     19,
   _3DPRIM_LINESTRIP_CONT_BF            =     20,
   _3DPRIM_TRIFAN_NOSTIPPLE             =     22,
};

enum GFX6_3D_Stencil_Operation {
   STENCILOP_KEEP                       =      0,
   STENCILOP_ZERO                       =      1,
   STENCILOP_REPLACE                    =      2,
   STENCILOP_INCRSAT                    =      3,
   STENCILOP_DECRSAT                    =      4,
   STENCILOP_INCR                       =      5,
   STENCILOP_DECR                       =      6,
   STENCILOP_INVERT                     =      7,
};

enum GFX6_3D_Vertex_Component_Control {
   VFCOMP_NOSTORE                       =      0,
   VFCOMP_STORE_SRC                     =      1,
   VFCOMP_STORE_0                       =      2,
   VFCOMP_STORE_1_FP                    =      3,
   VFCOMP_STORE_1_INT                   =      4,
   VFCOMP_STORE_VID                     =      5,
   VFCOMP_STORE_IID                     =      6,
   VFCOMP_STORE_PID                     =      7,
};

enum GFX6_TextureCoordinateMode {
   TCM_WRAP                             =      0,
   TCM_MIRROR                           =      1,
   TCM_CLAMP                            =      2,
   TCM_CUBE                             =      3,
   TCM_CLAMP_BORDER                     =      4,
   TCM_MIRROR_ONCE                      =      5,
};

#define GFX6_3DSTATE_CONSTANT_BODY_length      4
struct GFX6_3DSTATE_CONSTANT_BODY {
   uint32_t                             ConstantBuffer0ReadLength;
   uint64_t                             PointertoConstantBuffer0;
   uint32_t                             ConstantBuffer1ReadLength;
   __gen_address_type                   PointertoConstantBuffer1;
   uint32_t                             ConstantBuffer2ReadLength;
   __gen_address_type                   PointertoConstantBuffer2;
   uint32_t                             ConstantBuffer3ReadLength;
   __gen_address_type                   PointertoConstantBuffer3;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_CONSTANT_BODY_pack(__attribute__((unused)) __gen_user_data *data,
                                __attribute__((unused)) void * restrict dst,
                                __attribute__((unused)) const struct GFX6_3DSTATE_CONSTANT_BODY * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->ConstantBuffer0ReadLength, 0, 4) |
      __gen_offset(values->PointertoConstantBuffer0, 5, 31);

   const uint32_t v1 =
      __gen_uint(values->ConstantBuffer1ReadLength, 0, 4);
   dw[1] = __gen_address(data, &dw[1], values->PointertoConstantBuffer1, v1, 5, 31);

   const uint32_t v2 =
      __gen_uint(values->ConstantBuffer2ReadLength, 0, 4);
   dw[2] = __gen_address(data, &dw[2], values->PointertoConstantBuffer2, v2, 5, 31);

   const uint32_t v3 =
      __gen_uint(values->ConstantBuffer3ReadLength, 0, 4);
   dw[3] = __gen_address(data, &dw[3], values->PointertoConstantBuffer3, v3, 5, 31);
}

#define GFX6_BINDING_TABLE_STATE_length        1
struct GFX6_BINDING_TABLE_STATE {
   uint64_t                             SurfaceStatePointer;
};

static inline __attribute__((always_inline)) void
GFX6_BINDING_TABLE_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_BINDING_TABLE_STATE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_offset(values->SurfaceStatePointer, 5, 31);
}

#define GFX6_BLEND_STATE_ENTRY_length          2
struct GFX6_BLEND_STATE_ENTRY {
   enum GFX6_3D_Color_Buffer_Blend_Factor DestinationBlendFactor;
   enum GFX6_3D_Color_Buffer_Blend_Factor SourceBlendFactor;
   enum GFX6_3D_Color_Buffer_Blend_Function ColorBlendFunction;
   enum GFX6_3D_Color_Buffer_Blend_Factor DestinationAlphaBlendFactor;
   enum GFX6_3D_Color_Buffer_Blend_Factor SourceAlphaBlendFactor;
   enum GFX6_3D_Color_Buffer_Blend_Function AlphaBlendFunction;
   bool                                 IndependentAlphaBlendEnable;
   bool                                 ColorBufferBlendEnable;
   bool                                 PostBlendColorClampEnable;
   bool                                 PreBlendColorClampEnable;
   uint32_t                             ColorClampRange;
#define COLORCLAMP_UNORM                         0
#define COLORCLAMP_SNORM                         1
#define COLORCLAMP_RTFORMAT                      2
   uint32_t                             YDitherOffset;
   uint32_t                             XDitherOffset;
   bool                                 ColorDitherEnable;
   enum GFX6_3D_Compare_Function        AlphaTestFunction;
   bool                                 AlphaTestEnable;
   enum GFX6_3D_Logic_Op_Function       LogicOpFunction;
   bool                                 LogicOpEnable;
   bool                                 WriteDisableBlue;
   bool                                 WriteDisableGreen;
   bool                                 WriteDisableRed;
   bool                                 WriteDisableAlpha;
   bool                                 AlphaToCoverageDitherEnable;
   bool                                 AlphaToOneEnable;
   bool                                 AlphaToCoverageEnable;
};

static inline __attribute__((always_inline)) void
GFX6_BLEND_STATE_ENTRY_pack(__attribute__((unused)) __gen_user_data *data,
                            __attribute__((unused)) void * restrict dst,
                            __attribute__((unused)) const struct GFX6_BLEND_STATE_ENTRY * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DestinationBlendFactor, 0, 4) |
      __gen_uint(values->SourceBlendFactor, 5, 9) |
      __gen_uint(values->ColorBlendFunction, 11, 13) |
      __gen_uint(values->DestinationAlphaBlendFactor, 15, 19) |
      __gen_uint(values->SourceAlphaBlendFactor, 20, 24) |
      __gen_uint(values->AlphaBlendFunction, 26, 28) |
      __gen_uint(values->IndependentAlphaBlendEnable, 30, 30) |
      __gen_uint(values->ColorBufferBlendEnable, 31, 31);

   dw[1] =
      __gen_uint(values->PostBlendColorClampEnable, 0, 0) |
      __gen_uint(values->PreBlendColorClampEnable, 1, 1) |
      __gen_uint(values->ColorClampRange, 2, 3) |
      __gen_uint(values->YDitherOffset, 8, 9) |
      __gen_uint(values->XDitherOffset, 10, 11) |
      __gen_uint(values->ColorDitherEnable, 12, 12) |
      __gen_uint(values->AlphaTestFunction, 13, 15) |
      __gen_uint(values->AlphaTestEnable, 16, 16) |
      __gen_uint(values->LogicOpFunction, 18, 21) |
      __gen_uint(values->LogicOpEnable, 22, 22) |
      __gen_uint(values->WriteDisableBlue, 24, 24) |
      __gen_uint(values->WriteDisableGreen, 25, 25) |
      __gen_uint(values->WriteDisableRed, 26, 26) |
      __gen_uint(values->WriteDisableAlpha, 27, 27) |
      __gen_uint(values->AlphaToCoverageDitherEnable, 29, 29) |
      __gen_uint(values->AlphaToOneEnable, 30, 30) |
      __gen_uint(values->AlphaToCoverageEnable, 31, 31);
}

#define GFX6_BLEND_STATE_length                0
struct GFX6_BLEND_STATE {
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX6_BLEND_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                      __attribute__((unused)) void * restrict dst,
                      __attribute__((unused)) const struct GFX6_BLEND_STATE * restrict values)
{
}

#define GFX6_CC_VIEWPORT_length                2
struct GFX6_CC_VIEWPORT {
   float                                MinimumDepth;
   float                                MaximumDepth;
};

static inline __attribute__((always_inline)) void
GFX6_CC_VIEWPORT_pack(__attribute__((unused)) __gen_user_data *data,
                      __attribute__((unused)) void * restrict dst,
                      __attribute__((unused)) const struct GFX6_CC_VIEWPORT * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_float(values->MinimumDepth);

   dw[1] =
      __gen_float(values->MaximumDepth);
}

#define GFX6_CLIP_VIEWPORT_length              4
struct GFX6_CLIP_VIEWPORT {
   float                                XMinClipGuardband;
   float                                XMaxClipGuardband;
   float                                YMinClipGuardband;
   float                                YMaxClipGuardband;
};

static inline __attribute__((always_inline)) void
GFX6_CLIP_VIEWPORT_pack(__attribute__((unused)) __gen_user_data *data,
                        __attribute__((unused)) void * restrict dst,
                        __attribute__((unused)) const struct GFX6_CLIP_VIEWPORT * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_float(values->XMinClipGuardband);

   dw[1] =
      __gen_float(values->XMaxClipGuardband);

   dw[2] =
      __gen_float(values->YMinClipGuardband);

   dw[3] =
      __gen_float(values->YMaxClipGuardband);
}

#define GFX6_COLOR_CALC_STATE_length           6
struct GFX6_COLOR_CALC_STATE {
   uint32_t                             AlphaTestFormat;
#define ALPHATEST_UNORM8                         0
#define ALPHATEST_FLOAT32                        1
   bool                                 RoundDisableFunctionDisable;
   uint32_t                             BackfaceStencilReferenceValue;
   uint32_t                             StencilReferenceValue;
   uint32_t                             AlphaReferenceValueAsUNORM8;
   float                                AlphaReferenceValueAsFLOAT32;
   float                                BlendConstantColorRed;
   float                                BlendConstantColorGreen;
   float                                BlendConstantColorBlue;
   float                                BlendConstantColorAlpha;
};

static inline __attribute__((always_inline)) void
GFX6_COLOR_CALC_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                           __attribute__((unused)) void * restrict dst,
                           __attribute__((unused)) const struct GFX6_COLOR_CALC_STATE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->AlphaTestFormat, 0, 0) |
      __gen_uint(values->RoundDisableFunctionDisable, 15, 15) |
      __gen_uint(values->BackfaceStencilReferenceValue, 16, 23) |
      __gen_uint(values->StencilReferenceValue, 24, 31);

   dw[1] =
      __gen_uint(values->AlphaReferenceValueAsUNORM8, 0, 31) |
      __gen_float(values->AlphaReferenceValueAsFLOAT32);

   dw[2] =
      __gen_float(values->BlendConstantColorRed);

   dw[3] =
      __gen_float(values->BlendConstantColorGreen);

   dw[4] =
      __gen_float(values->BlendConstantColorBlue);

   dw[5] =
      __gen_float(values->BlendConstantColorAlpha);
}

#define GFX6_DEPTH_STENCIL_STATE_length        3
struct GFX6_DEPTH_STENCIL_STATE {
   enum GFX6_3D_Stencil_Operation       BackfaceStencilPassDepthPassOp;
   enum GFX6_3D_Stencil_Operation       BackfaceStencilPassDepthFailOp;
   enum GFX6_3D_Stencil_Operation       BackfaceStencilFailOp;
   enum GFX6_3D_Compare_Function        BackfaceStencilTestFunction;
   bool                                 DoubleSidedStencilEnable;
   bool                                 StencilBufferWriteEnable;
   enum GFX6_3D_Stencil_Operation       StencilPassDepthPassOp;
   enum GFX6_3D_Stencil_Operation       StencilPassDepthFailOp;
   enum GFX6_3D_Stencil_Operation       StencilFailOp;
   enum GFX6_3D_Compare_Function        StencilTestFunction;
   bool                                 StencilTestEnable;
   uint32_t                             BackfaceStencilWriteMask;
   uint32_t                             BackfaceStencilTestMask;
   uint32_t                             StencilWriteMask;
   uint32_t                             StencilTestMask;
   bool                                 DepthBufferWriteEnable;
   enum GFX6_3D_Compare_Function        DepthTestFunction;
   bool                                 DepthTestEnable;
};

static inline __attribute__((always_inline)) void
GFX6_DEPTH_STENCIL_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_DEPTH_STENCIL_STATE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->BackfaceStencilPassDepthPassOp, 3, 5) |
      __gen_uint(values->BackfaceStencilPassDepthFailOp, 6, 8) |
      __gen_uint(values->BackfaceStencilFailOp, 9, 11) |
      __gen_uint(values->BackfaceStencilTestFunction, 12, 14) |
      __gen_uint(values->DoubleSidedStencilEnable, 15, 15) |
      __gen_uint(values->StencilBufferWriteEnable, 18, 18) |
      __gen_uint(values->StencilPassDepthPassOp, 19, 21) |
      __gen_uint(values->StencilPassDepthFailOp, 22, 24) |
      __gen_uint(values->StencilFailOp, 25, 27) |
      __gen_uint(values->StencilTestFunction, 28, 30) |
      __gen_uint(values->StencilTestEnable, 31, 31);

   dw[1] =
      __gen_uint(values->BackfaceStencilWriteMask, 0, 7) |
      __gen_uint(values->BackfaceStencilTestMask, 8, 15) |
      __gen_uint(values->StencilWriteMask, 16, 23) |
      __gen_uint(values->StencilTestMask, 24, 31);

   dw[2] =
      __gen_uint(values->DepthBufferWriteEnable, 26, 26) |
      __gen_uint(values->DepthTestFunction, 27, 29) |
      __gen_uint(values->DepthTestEnable, 31, 31);
}

#define GFX6_INTERFACE_DESCRIPTOR_DATA_length      8
struct GFX6_INTERFACE_DESCRIPTOR_DATA {
   uint64_t                             KernelStartPointer;
   bool                                 SoftwareExceptionEnable;
   bool                                 MaskStackExceptionEnable;
   bool                                 IllegalOpcodeExceptionEnable;
   uint32_t                             FloatingPointMode;
#define IEEE754                                  0
#define Alternate                                1
   uint32_t                             ThreadPriority;
#define NormalPriority                           0
#define HighPriority                             1
   bool                                 SingleProgramFlow;
   uint32_t                             SamplerCount;
#define Nosamplersused                           0
#define Between1and4samplersused                 1
#define Between5and8samplersused                 2
#define Between9and12samplersused                3
#define Between13and16samplersused               4
   uint64_t                             SamplerStatePointer;
   uint32_t                             BindingTableEntryCount;
   uint64_t                             BindingTablePointer;
   uint32_t                             ConstantURBEntryReadOffset;
   uint32_t                             ConstantURBEntryReadLength;
   uint32_t                             BarrierID;
   uint32_t                             BarrierReturnByte;
   uint32_t                             BarrierReturnGRFOffset;
};

static inline __attribute__((always_inline)) void
GFX6_INTERFACE_DESCRIPTOR_DATA_pack(__attribute__((unused)) __gen_user_data *data,
                                    __attribute__((unused)) void * restrict dst,
                                    __attribute__((unused)) const struct GFX6_INTERFACE_DESCRIPTOR_DATA * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_offset(values->KernelStartPointer, 6, 31);

   dw[1] =
      __gen_uint(values->SoftwareExceptionEnable, 7, 7) |
      __gen_uint(values->MaskStackExceptionEnable, 11, 11) |
      __gen_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      __gen_uint(values->FloatingPointMode, 16, 16) |
      __gen_uint(values->ThreadPriority, 17, 17) |
      __gen_uint(values->SingleProgramFlow, 18, 18);

   dw[2] =
      __gen_uint(values->SamplerCount, 2, 4) |
      __gen_offset(values->SamplerStatePointer, 5, 31);

   dw[3] =
      __gen_uint(values->BindingTableEntryCount, 0, 4) |
      __gen_offset(values->BindingTablePointer, 5, 31);

   dw[4] =
      __gen_uint(values->ConstantURBEntryReadOffset, 0, 15) |
      __gen_uint(values->ConstantURBEntryReadLength, 16, 31);

   dw[5] =
      __gen_uint(values->BarrierID, 0, 3) |
      __gen_uint(values->BarrierReturnByte, 8, 15) |
      __gen_uint(values->BarrierReturnGRFOffset, 24, 31);

   dw[6] = 0;

   dw[7] = 0;
}

#define GFX6_MEMORY_OBJECT_CONTROL_STATE_length      1
struct GFX6_MEMORY_OBJECT_CONTROL_STATE {
   uint32_t                             CacheabilityControl;
   uint32_t                             GraphicsDataTypeGFDT;
};

static inline __attribute__((always_inline)) void
GFX6_MEMORY_OBJECT_CONTROL_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                                      __attribute__((unused)) void * restrict dst,
                                      __attribute__((unused)) const struct GFX6_MEMORY_OBJECT_CONTROL_STATE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->CacheabilityControl, 0, 1) |
      __gen_uint(values->GraphicsDataTypeGFDT, 2, 2);
}

#define GFX6_PALETTE_ENTRY_length              1
struct GFX6_PALETTE_ENTRY {
   uint32_t                             Blue;
   uint32_t                             Green;
   uint32_t                             Red;
   uint32_t                             Alpha;
};

static inline __attribute__((always_inline)) void
GFX6_PALETTE_ENTRY_pack(__attribute__((unused)) __gen_user_data *data,
                        __attribute__((unused)) void * restrict dst,
                        __attribute__((unused)) const struct GFX6_PALETTE_ENTRY * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->Blue, 0, 7) |
      __gen_uint(values->Green, 8, 15) |
      __gen_uint(values->Red, 16, 23) |
      __gen_uint(values->Alpha, 24, 31);
}

#define GFX6_RENDER_SURFACE_STATE_length       6
struct GFX6_RENDER_SURFACE_STATE {
   bool                                 CubeFaceEnablePositiveZ;
   bool                                 CubeFaceEnableNegativeZ;
   bool                                 CubeFaceEnablePositiveY;
   bool                                 CubeFaceEnableNegativeY;
   bool                                 CubeFaceEnablePositiveX;
   bool                                 CubeFaceEnableNegativeX;
   uint32_t                             MediaBoundaryPixelMode;
#define NORMAL_MODE                              0
#define PROGRESSIVE_FRAME                        2
#define INTERLACED_FRAME                         3
   uint32_t                             RenderCacheReadWriteMode;
#define WRITE_ONLY                               0
#define READ_WRITE                               1
   uint32_t                             CubeMapCornerMode;
#define CUBE_REPLICATE                           0
#define CUBE_AVERAGE                             1
   uint32_t                             MIPMapLayoutMode;
#define MIPLAYOUT_BELOW                          0
#define MIPLAYOUT_RIGHT                          1
   uint32_t                             VerticalLineStrideOffset;
   uint32_t                             VerticalLineStride;
   uint32_t                             SurfaceFormat;
   uint32_t                             DataReturnFormat;
#define DATA_RETURN_FLOAT32                      0
   uint32_t                             SurfaceType;
#define SURFTYPE_1D                              0
#define SURFTYPE_2D                              1
#define SURFTYPE_3D                              2
#define SURFTYPE_CUBE                            3
#define SURFTYPE_BUFFER                          4
#define SURFTYPE_NULL                            7
   __gen_address_type                   SurfaceBaseAddress;
   uint32_t                             RenderTargetRotation;
#define RTROTATE_0DEG                            0
#define RTROTATE_90DEG                           1
#define RTROTATE_270DEG                          3
   uint32_t                             MIPCountLOD;
   uint32_t                             Width;
   uint32_t                             Height;
   uint32_t                             TileWalk;
#define TILEWALK_XMAJOR                          0
#define TILEWALK_YMAJOR                          1
   bool                                 TiledSurface;
   uint32_t                             SurfacePitch;
   uint32_t                             Depth;
   uint32_t                             MultisamplePositionPaletteIndex;
   uint32_t                             NumberofMultisamples;
#define MULTISAMPLECOUNT_1                       0
#define MULTISAMPLECOUNT_4                       2
   uint32_t                             RenderTargetViewExtent;
   uint32_t                             MinimumArrayElement;
   uint32_t                             SurfaceMinLOD;
   uint32_t                             MOCS;
   uint32_t                             YOffset;
   uint32_t                             SurfaceVerticalAlignment;
#define VALIGN_2                                 0
#define VALIGN_4                                 1
   uint32_t                             XOffset;
};

static inline __attribute__((always_inline)) void
GFX6_RENDER_SURFACE_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                               __attribute__((unused)) void * restrict dst,
                               __attribute__((unused)) const struct GFX6_RENDER_SURFACE_STATE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->CubeFaceEnablePositiveZ, 0, 0) |
      __gen_uint(values->CubeFaceEnableNegativeZ, 1, 1) |
      __gen_uint(values->CubeFaceEnablePositiveY, 2, 2) |
      __gen_uint(values->CubeFaceEnableNegativeY, 3, 3) |
      __gen_uint(values->CubeFaceEnablePositiveX, 4, 4) |
      __gen_uint(values->CubeFaceEnableNegativeX, 5, 5) |
      __gen_uint(values->MediaBoundaryPixelMode, 6, 7) |
      __gen_uint(values->RenderCacheReadWriteMode, 8, 8) |
      __gen_uint(values->CubeMapCornerMode, 9, 9) |
      __gen_uint(values->MIPMapLayoutMode, 10, 10) |
      __gen_uint(values->VerticalLineStrideOffset, 11, 11) |
      __gen_uint(values->VerticalLineStride, 12, 12) |
      __gen_uint(values->SurfaceFormat, 18, 26) |
      __gen_uint(values->DataReturnFormat, 27, 27) |
      __gen_uint(values->SurfaceType, 29, 31);

   dw[1] = __gen_address(data, &dw[1], values->SurfaceBaseAddress, 0, 0, 31);

   dw[2] =
      __gen_uint(values->RenderTargetRotation, 0, 1) |
      __gen_uint(values->MIPCountLOD, 2, 5) |
      __gen_uint(values->Width, 6, 18) |
      __gen_uint(values->Height, 19, 31);

   dw[3] =
      __gen_uint(values->TileWalk, 0, 0) |
      __gen_uint(values->TiledSurface, 1, 1) |
      __gen_uint(values->SurfacePitch, 3, 19) |
      __gen_uint(values->Depth, 21, 31);

   dw[4] =
      __gen_uint(values->MultisamplePositionPaletteIndex, 0, 2) |
      __gen_uint(values->NumberofMultisamples, 4, 6) |
      __gen_uint(values->RenderTargetViewExtent, 8, 16) |
      __gen_uint(values->MinimumArrayElement, 17, 27) |
      __gen_uint(values->SurfaceMinLOD, 28, 31);

   dw[5] =
      __gen_uint(values->MOCS, 16, 19) |
      __gen_uint(values->YOffset, 20, 23) |
      __gen_uint(values->SurfaceVerticalAlignment, 24, 24) |
      __gen_uint(values->XOffset, 25, 31);
}

#define GFX6_SAMPLER_BORDER_COLOR_STATE_length     12
struct GFX6_SAMPLER_BORDER_COLOR_STATE {
   uint32_t                             BorderColorUnormRed;
   uint32_t                             BorderColorUnormGreen;
   uint32_t                             BorderColorUnormBlue;
   uint32_t                             BorderColorUnormAlpha;
   float                                BorderColorFloatRed;
   float                                BorderColorFloatGreen;
   float                                BorderColorFloatBlue;
   float                                BorderColorFloatAlpha;
   uint32_t                             BorderColorFloat16Red;
   uint32_t                             BorderColorFloat16Green;
   uint32_t                             BorderColorFloat16Blue;
   uint32_t                             BorderColorFloat16Alpha;
   uint32_t                             BorderColorUnorm16Red;
   uint32_t                             BorderColorUnorm16Green;
   uint32_t                             BorderColorUnorm16Blue;
   uint32_t                             BorderColorUnorm16Alpha;
   int32_t                              BorderColorSnorm16Red;
   int32_t                              BorderColorSnorm16Green;
   int32_t                              BorderColorSnorm16Blue;
   int32_t                              BorderColorSnorm16Alpha;
   int32_t                              BorderColorSnorm8Red;
   int32_t                              BorderColorSnorm8Green;
   int32_t                              BorderColorSnorm8Blue;
   int32_t                              BorderColorSnorm8Alpha;
};

static inline __attribute__((always_inline)) void
GFX6_SAMPLER_BORDER_COLOR_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                                     __attribute__((unused)) void * restrict dst,
                                     __attribute__((unused)) const struct GFX6_SAMPLER_BORDER_COLOR_STATE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->BorderColorUnormRed, 0, 7) |
      __gen_uint(values->BorderColorUnormGreen, 8, 15) |
      __gen_uint(values->BorderColorUnormBlue, 16, 23) |
      __gen_uint(values->BorderColorUnormAlpha, 24, 31);

   dw[1] =
      __gen_float(values->BorderColorFloatRed);

   dw[2] =
      __gen_float(values->BorderColorFloatGreen);

   dw[3] =
      __gen_float(values->BorderColorFloatBlue);

   dw[4] =
      __gen_float(values->BorderColorFloatAlpha);

   dw[5] =
      __gen_uint(values->BorderColorFloat16Red, 0, 15) |
      __gen_uint(values->BorderColorFloat16Green, 16, 31);

   dw[6] =
      __gen_uint(values->BorderColorFloat16Blue, 0, 15) |
      __gen_uint(values->BorderColorFloat16Alpha, 16, 31);

   dw[7] =
      __gen_uint(values->BorderColorUnorm16Red, 0, 15) |
      __gen_uint(values->BorderColorUnorm16Green, 16, 31);

   dw[8] =
      __gen_uint(values->BorderColorUnorm16Blue, 0, 15) |
      __gen_uint(values->BorderColorUnorm16Alpha, 16, 31);

   dw[9] =
      __gen_sint(values->BorderColorSnorm16Red, 0, 15) |
      __gen_sint(values->BorderColorSnorm16Green, 16, 31);

   dw[10] =
      __gen_sint(values->BorderColorSnorm16Blue, 0, 15) |
      __gen_sint(values->BorderColorSnorm16Alpha, 16, 31);

   dw[11] =
      __gen_sint(values->BorderColorSnorm8Red, 0, 7) |
      __gen_sint(values->BorderColorSnorm8Green, 8, 15) |
      __gen_sint(values->BorderColorSnorm8Blue, 16, 23) |
      __gen_sint(values->BorderColorSnorm8Alpha, 24, 31);
}

#define GFX6_SAMPLER_STATE_length              4
struct GFX6_SAMPLER_STATE {
   uint32_t                             ShadowFunction;
#define PREFILTEROP_ALWAYS                       0
#define PREFILTEROP_NEVER                        1
#define PREFILTEROP_LESS                         2
#define PREFILTEROP_EQUAL                        3
#define PREFILTEROP_LEQUAL                       4
#define PREFILTEROP_GREATER                      5
#define PREFILTEROP_NOTEQUAL                     6
#define PREFILTEROP_GEQUAL                       7
   float                                TextureLODBias;
   uint32_t                             MinModeFilter;
   uint32_t                             MagModeFilter;
#define MAPFILTER_NEAREST                        0
#define MAPFILTER_LINEAR                         1
#define MAPFILTER_ANISOTROPIC                    2
#define MAPFILTER_MONO                           6
   uint32_t                             MipModeFilter;
#define MIPFILTER_NONE                           0
#define MIPFILTER_NEAREST                        1
#define MIPFILTER_LINEAR                         3
   float                                BaseMipLevel;
   uint32_t                             MinandMagStateNotEqual;
#define MIN_MAG_EQ                               0
#define MIN_MAG_NEQ                              1
   uint32_t                             LODPreClampEnable;
#define CLAMP_ENABLE_OGL                         1
   uint32_t                             TextureBorderColorMode;
#define DX10OGL                                  0
#define DX9                                      1
   bool                                 SamplerDisable;
   enum GFX6_TextureCoordinateMode      TCZAddressControlMode;
   enum GFX6_TextureCoordinateMode      TCYAddressControlMode;
   enum GFX6_TextureCoordinateMode      TCXAddressControlMode;
   uint32_t                             CubeSurfaceControlMode;
#define CUBECTRLMODE_PROGRAMMED                  0
#define CUBECTRLMODE_OVERRIDE                    1
   float                                MaxLOD;
   float                                MinLOD;
   uint64_t                             BorderColorPointer;
   bool                                 NonnormalizedCoordinateEnable;
   bool                                 RAddressMinFilterRoundingEnable;
   bool                                 RAddressMagFilterRoundingEnable;
   bool                                 VAddressMinFilterRoundingEnable;
   bool                                 VAddressMagFilterRoundingEnable;
   bool                                 UAddressMinFilterRoundingEnable;
   bool                                 UAddressMagFilterRoundingEnable;
   uint32_t                             MaximumAnisotropy;
#define RATIO21                                  0
#define RATIO41                                  1
#define RATIO61                                  2
#define RATIO81                                  3
#define RATIO101                                 4
#define RATIO121                                 5
#define RATIO141                                 6
#define RATIO161                                 7
   uint32_t                             ChromaKeyMode;
#define KEYFILTER_KILL_ON_ANY_MATCH              0
#define KEYFILTER_REPLACE_BLACK                  1
   uint32_t                             ChromaKeyIndex;
   bool                                 ChromaKeyEnable;
   uint32_t                             MonochromeFilterWidth;
   uint32_t                             MonochromeFilterHeightReserved;
};

static inline __attribute__((always_inline)) void
GFX6_SAMPLER_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                        __attribute__((unused)) void * restrict dst,
                        __attribute__((unused)) const struct GFX6_SAMPLER_STATE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->ShadowFunction, 0, 2) |
      __gen_sfixed(values->TextureLODBias, 3, 13, 6) |
      __gen_uint(values->MinModeFilter, 14, 16) |
      __gen_uint(values->MagModeFilter, 17, 19) |
      __gen_uint(values->MipModeFilter, 20, 21) |
      __gen_ufixed(values->BaseMipLevel, 22, 26, 1) |
      __gen_uint(values->MinandMagStateNotEqual, 27, 27) |
      __gen_uint(values->LODPreClampEnable, 28, 28) |
      __gen_uint(values->TextureBorderColorMode, 29, 29) |
      __gen_uint(values->SamplerDisable, 31, 31);

   dw[1] =
      __gen_uint(values->TCZAddressControlMode, 0, 2) |
      __gen_uint(values->TCYAddressControlMode, 3, 5) |
      __gen_uint(values->TCXAddressControlMode, 6, 8) |
      __gen_uint(values->CubeSurfaceControlMode, 9, 9) |
      __gen_ufixed(values->MaxLOD, 12, 21, 6) |
      __gen_ufixed(values->MinLOD, 22, 31, 6);

   dw[2] =
      __gen_offset(values->BorderColorPointer, 5, 31);

   dw[3] =
      __gen_uint(values->NonnormalizedCoordinateEnable, 0, 0) |
      __gen_uint(values->RAddressMinFilterRoundingEnable, 13, 13) |
      __gen_uint(values->RAddressMagFilterRoundingEnable, 14, 14) |
      __gen_uint(values->VAddressMinFilterRoundingEnable, 15, 15) |
      __gen_uint(values->VAddressMagFilterRoundingEnable, 16, 16) |
      __gen_uint(values->UAddressMinFilterRoundingEnable, 17, 17) |
      __gen_uint(values->UAddressMagFilterRoundingEnable, 18, 18) |
      __gen_uint(values->MaximumAnisotropy, 19, 21) |
      __gen_uint(values->ChromaKeyMode, 22, 22) |
      __gen_uint(values->ChromaKeyIndex, 23, 24) |
      __gen_uint(values->ChromaKeyEnable, 25, 25) |
      __gen_uint(values->MonochromeFilterWidth, 26, 28) |
      __gen_uint(values->MonochromeFilterHeightReserved, 29, 31);
}

#define GFX6_SCISSOR_RECT_length               2
struct GFX6_SCISSOR_RECT {
   uint32_t                             ScissorRectangleXMin;
   uint32_t                             ScissorRectangleYMin;
   uint32_t                             ScissorRectangleXMax;
   uint32_t                             ScissorRectangleYMax;
};

static inline __attribute__((always_inline)) void
GFX6_SCISSOR_RECT_pack(__attribute__((unused)) __gen_user_data *data,
                       __attribute__((unused)) void * restrict dst,
                       __attribute__((unused)) const struct GFX6_SCISSOR_RECT * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->ScissorRectangleXMin, 0, 15) |
      __gen_uint(values->ScissorRectangleYMin, 16, 31);

   dw[1] =
      __gen_uint(values->ScissorRectangleXMax, 0, 15) |
      __gen_uint(values->ScissorRectangleYMax, 16, 31);
}

#define GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_length      1
struct GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL {
   uint32_t                             SourceAttribute;
   uint32_t                             SwizzleSelect;
#define INPUTATTR                                0
#define INPUTATTR_FACING                         1
#define INPUTATTR_W                              2
#define INPUTATTR_FACING_W                       3
   uint32_t                             ConstantSource;
#define CONST_0000                               0
#define CONST_0001_FLOAT                         1
#define CONST_1111_FLOAT                         2
#define PRIM_ID                                  3
   uint32_t                             SwizzleControlMode;
   bool                                 ComponentOverrideX;
   bool                                 ComponentOverrideY;
   bool                                 ComponentOverrideZ;
   bool                                 ComponentOverrideW;
};

static inline __attribute__((always_inline)) void
GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(__attribute__((unused)) __gen_user_data *data,
                                     __attribute__((unused)) void * restrict dst,
                                     __attribute__((unused)) const struct GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->SourceAttribute, 0, 4) |
      __gen_uint(values->SwizzleSelect, 6, 7) |
      __gen_uint(values->ConstantSource, 9, 10) |
      __gen_uint(values->SwizzleControlMode, 11, 11) |
      __gen_uint(values->ComponentOverrideX, 12, 12) |
      __gen_uint(values->ComponentOverrideY, 13, 13) |
      __gen_uint(values->ComponentOverrideZ, 14, 14) |
      __gen_uint(values->ComponentOverrideW, 15, 15);
}

#define GFX6_SF_VIEWPORT_length                8
struct GFX6_SF_VIEWPORT {
   float                                ViewportMatrixElementm00;
   float                                ViewportMatrixElementm11;
   float                                ViewportMatrixElementm22;
#define ExcludesDWord01                          0
   float                                ViewportMatrixElementm30;
   float                                ViewportMatrixElementm31;
   float                                ViewportMatrixElementm32;
};

static inline __attribute__((always_inline)) void
GFX6_SF_VIEWPORT_pack(__attribute__((unused)) __gen_user_data *data,
                      __attribute__((unused)) void * restrict dst,
                      __attribute__((unused)) const struct GFX6_SF_VIEWPORT * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_float(values->ViewportMatrixElementm00);

   dw[1] =
      __gen_float(values->ViewportMatrixElementm11);

   dw[2] =
      __gen_float(values->ViewportMatrixElementm22);

   dw[3] =
      __gen_float(values->ViewportMatrixElementm30);

   dw[4] =
      __gen_float(values->ViewportMatrixElementm31);

   dw[5] =
      __gen_float(values->ViewportMatrixElementm32);

   dw[6] = 0;

   dw[7] = 0;
}

#define GFX6_VERTEX_BUFFER_STATE_length        4
struct GFX6_VERTEX_BUFFER_STATE {
   uint32_t                             BufferPitch;
   bool                                 VertexFetchInvalidate;
   bool                                 NullVertexBuffer;
   uint32_t                             MOCS;
   uint32_t                             BufferAccessType;
#define VERTEXDATA                               0
#define INSTANCEDATA                             1
   uint32_t                             VertexBufferIndex;
   __gen_address_type                   BufferStartingAddress;
   __gen_address_type                   EndAddress;
   uint32_t                             InstanceDataStepRate;
};

static inline __attribute__((always_inline)) void
GFX6_VERTEX_BUFFER_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_VERTEX_BUFFER_STATE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->BufferPitch, 0, 11) |
      __gen_uint(values->VertexFetchInvalidate, 12, 12) |
      __gen_uint(values->NullVertexBuffer, 13, 13) |
      __gen_uint(values->MOCS, 16, 19) |
      __gen_uint(values->BufferAccessType, 20, 20) |
      __gen_uint(values->VertexBufferIndex, 26, 31);

   dw[1] = __gen_address(data, &dw[1], values->BufferStartingAddress, 0, 0, 31);

   dw[2] = __gen_address(data, &dw[2], values->EndAddress, 0, 0, 31);

   dw[3] =
      __gen_uint(values->InstanceDataStepRate, 0, 31);
}

#define GFX6_VERTEX_ELEMENT_STATE_length       2
struct GFX6_VERTEX_ELEMENT_STATE {
   uint32_t                             SourceElementOffset;
   bool                                 EdgeFlagEnable;
   uint32_t                             SourceElementFormat;
   bool                                 Valid;
   uint32_t                             VertexBufferIndex;
   enum GFX6_3D_Vertex_Component_Control Component3Control;
   enum GFX6_3D_Vertex_Component_Control Component2Control;
   enum GFX6_3D_Vertex_Component_Control Component1Control;
   enum GFX6_3D_Vertex_Component_Control Component0Control;
};

static inline __attribute__((always_inline)) void
GFX6_VERTEX_ELEMENT_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                               __attribute__((unused)) void * restrict dst,
                               __attribute__((unused)) const struct GFX6_VERTEX_ELEMENT_STATE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->SourceElementOffset, 0, 11) |
      __gen_uint(values->EdgeFlagEnable, 15, 15) |
      __gen_uint(values->SourceElementFormat, 16, 24) |
      __gen_uint(values->Valid, 25, 25) |
      __gen_uint(values->VertexBufferIndex, 26, 31);

   dw[1] =
      __gen_uint(values->Component3Control, 16, 18) |
      __gen_uint(values->Component2Control, 20, 22) |
      __gen_uint(values->Component1Control, 24, 26) |
      __gen_uint(values->Component0Control, 28, 30);
}

#define GFX6_3DPRIMITIVE_length                6
#define GFX6_3DPRIMITIVE_length_bias           2
#define GFX6_3DPRIMITIVE_header                 \
   .DWordLength                         =      4,  \
   ._3DCommandSubOpcode                 =      0,  \
   ._3DCommandOpcode                    =      3,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DPRIMITIVE {
   uint32_t                             DWordLength;
   uint32_t                             InternalVertexCount;
   enum GFX6_3D_Prim_Topo_Type          PrimitiveTopologyType;
   uint32_t                             VertexAccessType;
#define SEQUENTIAL                               0
#define RANDOM                                   1
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             VertexCountPerInstance;
   uint32_t                             StartVertexLocation;
   uint32_t                             InstanceCount;
#define UNDEFINED                                0
#define noninstancedoperation                    1
   uint32_t                             StartInstanceLocation;
   int32_t                              BaseVertexLocation;
};

static inline __attribute__((always_inline)) void
GFX6_3DPRIMITIVE_pack(__attribute__((unused)) __gen_user_data *data,
                      __attribute__((unused)) void * restrict dst,
                      __attribute__((unused)) const struct GFX6_3DPRIMITIVE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->InternalVertexCount, 9, 9) |
      __gen_uint(values->PrimitiveTopologyType, 10, 14) |
      __gen_uint(values->VertexAccessType, 15, 15) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->VertexCountPerInstance, 0, 31);

   dw[2] =
      __gen_uint(values->StartVertexLocation, 0, 31);

   dw[3] =
      __gen_uint(values->InstanceCount, 0, 31);

   dw[4] =
      __gen_uint(values->StartInstanceLocation, 0, 31);

   dw[5] =
      __gen_sint(values->BaseVertexLocation, 0, 31);
}

#define GFX6_3DSTATE_AA_LINE_PARAMETERS_length      3
#define GFX6_3DSTATE_AA_LINE_PARAMETERS_length_bias      2
#define GFX6_3DSTATE_AA_LINE_PARAMETERS_header  \
   .DWordLength                         =      1,  \
   ._3DCommandSubOpcode                 =     10,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_AA_LINE_PARAMETERS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   float                                AACoverageSlope;
   float                                AACoverageBias;
   float                                AACoverageEndCapSlope;
   float                                AACoverageEndCapBias;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_AA_LINE_PARAMETERS_pack(__attribute__((unused)) __gen_user_data *data,
                                     __attribute__((unused)) void * restrict dst,
                                     __attribute__((unused)) const struct GFX6_3DSTATE_AA_LINE_PARAMETERS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_ufixed(values->AACoverageSlope, 0, 7, 8) |
      __gen_ufixed(values->AACoverageBias, 16, 23, 8);

   dw[2] =
      __gen_ufixed(values->AACoverageEndCapSlope, 0, 7, 8) |
      __gen_ufixed(values->AACoverageEndCapBias, 16, 23, 8);
}

#define GFX6_3DSTATE_BINDING_TABLE_POINTERS_length      4
#define GFX6_3DSTATE_BINDING_TABLE_POINTERS_length_bias      2
#define GFX6_3DSTATE_BINDING_TABLE_POINTERS_header\
   .DWordLength                         =      2,  \
   ._3DCommandSubOpcode                 =      1,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_BINDING_TABLE_POINTERS {
   uint32_t                             DWordLength;
   bool                                 VSBindingTableChange;
   bool                                 GSBindingTableChange;
   bool                                 PSBindingTableChange;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint64_t                             PointertoVSBindingTable;
   uint64_t                             PointertoGSBindingTable;
   uint64_t                             PointertoPSBindingTable;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_BINDING_TABLE_POINTERS_pack(__attribute__((unused)) __gen_user_data *data,
                                         __attribute__((unused)) void * restrict dst,
                                         __attribute__((unused)) const struct GFX6_3DSTATE_BINDING_TABLE_POINTERS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->VSBindingTableChange, 8, 8) |
      __gen_uint(values->GSBindingTableChange, 9, 9) |
      __gen_uint(values->PSBindingTableChange, 12, 12) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->PointertoVSBindingTable, 5, 31);

   dw[2] =
      __gen_offset(values->PointertoGSBindingTable, 5, 31);

   dw[3] =
      __gen_offset(values->PointertoPSBindingTable, 5, 31);
}

#define GFX6_3DSTATE_CC_STATE_POINTERS_length      4
#define GFX6_3DSTATE_CC_STATE_POINTERS_length_bias      2
#define GFX6_3DSTATE_CC_STATE_POINTERS_header   \
   .DWordLength                         =      2,  \
   ._3DCommandSubOpcode                 =     14,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_CC_STATE_POINTERS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   bool                                 BLEND_STATEChange;
   uint64_t                             PointertoBLEND_STATE;
   bool                                 DEPTH_STENCIL_STATEChange;
   uint64_t                             PointertoDEPTH_STENCIL_STATE;
   bool                                 ColorCalcStatePointerValid;
   uint64_t                             ColorCalcStatePointer;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_CC_STATE_POINTERS_pack(__attribute__((unused)) __gen_user_data *data,
                                    __attribute__((unused)) void * restrict dst,
                                    __attribute__((unused)) const struct GFX6_3DSTATE_CC_STATE_POINTERS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->BLEND_STATEChange, 0, 0) |
      __gen_offset(values->PointertoBLEND_STATE, 6, 31);

   dw[2] =
      __gen_uint(values->DEPTH_STENCIL_STATEChange, 0, 0) |
      __gen_offset(values->PointertoDEPTH_STENCIL_STATE, 6, 31);

   dw[3] =
      __gen_uint(values->ColorCalcStatePointerValid, 0, 0) |
      __gen_offset(values->ColorCalcStatePointer, 6, 31);
}

#define GFX6_3DSTATE_CHROMA_KEY_length         4
#define GFX6_3DSTATE_CHROMA_KEY_length_bias      2
#define GFX6_3DSTATE_CHROMA_KEY_header          \
   .DWordLength                         =      2,  \
   ._3DCommandSubOpcode                 =      4,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_CHROMA_KEY {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             ChromaKeyTableIndex;
   uint32_t                             ChromaKeyLowValue;
   uint32_t                             ChromaKeyHighValue;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_CHROMA_KEY_pack(__attribute__((unused)) __gen_user_data *data,
                             __attribute__((unused)) void * restrict dst,
                             __attribute__((unused)) const struct GFX6_3DSTATE_CHROMA_KEY * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->ChromaKeyTableIndex, 30, 31);

   dw[2] =
      __gen_uint(values->ChromaKeyLowValue, 0, 31);

   dw[3] =
      __gen_uint(values->ChromaKeyHighValue, 0, 31);
}

#define GFX6_3DSTATE_CLEAR_PARAMS_length       2
#define GFX6_3DSTATE_CLEAR_PARAMS_length_bias      2
#define GFX6_3DSTATE_CLEAR_PARAMS_header        \
   .DWordLength                         =      0,  \
   ._3DCommandSubOpcode                 =     16,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_CLEAR_PARAMS {
   uint32_t                             DWordLength;
   bool                                 DepthClearValueValid;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             DepthClearValue;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_CLEAR_PARAMS_pack(__attribute__((unused)) __gen_user_data *data,
                               __attribute__((unused)) void * restrict dst,
                               __attribute__((unused)) const struct GFX6_3DSTATE_CLEAR_PARAMS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->DepthClearValueValid, 15, 15) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->DepthClearValue, 0, 31);
}

#define GFX6_3DSTATE_CLIP_length               4
#define GFX6_3DSTATE_CLIP_length_bias          2
#define GFX6_3DSTATE_CLIP_header                \
   .DWordLength                         =      2,  \
   ._3DCommandSubOpcode                 =     18,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_CLIP {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             UserClipDistanceCullTestEnableBitmask;
   bool                                 StatisticsEnable;
   uint32_t                             TriangleFanProvokingVertexSelect;
#define Vertex0                                  0
#define Vertex1                                  1
#define Vertex2                                  2
   uint32_t                             LineStripListProvokingVertexSelect;
#define Vertex0                                  0
#define Vertex1                                  1
   uint32_t                             TriangleStripListProvokingVertexSelect;
#define Vertex0                                  0
#define Vertex1                                  1
#define Vertex2                                  2
   bool                                 NonPerspectiveBarycentricEnable;
   bool                                 PerspectiveDivideDisable;
   uint32_t                             ClipMode;
#define CLIPMODE_NORMAL                          0
#define CLIPMODE_REJECT_ALL                      3
#define CLIPMODE_ACCEPT_ALL                      4
   uint32_t                             UserClipDistanceClipTestEnableBitmask;
   bool                                 GuardbandClipTestEnable;
   bool                                 ViewportZClipTestEnable;
   bool                                 ViewportXYClipTestEnable;
   uint32_t                             APIMode;
#define APIMODE_OGL                              0
#define APIMODE_D3D                              1
   bool                                 ClipEnable;
   uint32_t                             MaximumVPIndex;
   bool                                 ForceZeroRTAIndexEnable;
   float                                MaximumPointWidth;
   float                                MinimumPointWidth;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_CLIP_pack(__attribute__((unused)) __gen_user_data *data,
                       __attribute__((unused)) void * restrict dst,
                       __attribute__((unused)) const struct GFX6_3DSTATE_CLIP * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->UserClipDistanceCullTestEnableBitmask, 0, 7) |
      __gen_uint(values->StatisticsEnable, 10, 10);

   dw[2] =
      __gen_uint(values->TriangleFanProvokingVertexSelect, 0, 1) |
      __gen_uint(values->LineStripListProvokingVertexSelect, 2, 3) |
      __gen_uint(values->TriangleStripListProvokingVertexSelect, 4, 5) |
      __gen_uint(values->NonPerspectiveBarycentricEnable, 8, 8) |
      __gen_uint(values->PerspectiveDivideDisable, 9, 9) |
      __gen_uint(values->ClipMode, 13, 15) |
      __gen_uint(values->UserClipDistanceClipTestEnableBitmask, 16, 23) |
      __gen_uint(values->GuardbandClipTestEnable, 26, 26) |
      __gen_uint(values->ViewportZClipTestEnable, 27, 27) |
      __gen_uint(values->ViewportXYClipTestEnable, 28, 28) |
      __gen_uint(values->APIMode, 30, 30) |
      __gen_uint(values->ClipEnable, 31, 31);

   dw[3] =
      __gen_uint(values->MaximumVPIndex, 0, 3) |
      __gen_uint(values->ForceZeroRTAIndexEnable, 5, 5) |
      __gen_ufixed(values->MaximumPointWidth, 6, 16, 3) |
      __gen_ufixed(values->MinimumPointWidth, 17, 27, 3);
}

#define GFX6_3DSTATE_CONSTANT_GS_length        5
#define GFX6_3DSTATE_CONSTANT_GS_length_bias      2
#define GFX6_3DSTATE_CONSTANT_GS_header         \
   .DWordLength                         =      3,  \
   ._3DCommandSubOpcode                 =     22,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_CONSTANT_GS {
   uint32_t                             DWordLength;
   uint32_t                             MOCS;
   bool                                 Buffer0Valid;
   bool                                 Buffer1Valid;
   bool                                 Buffer2Valid;
   bool                                 Buffer3Valid;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   struct GFX6_3DSTATE_CONSTANT_BODY    ConstantBody;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_CONSTANT_GS_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_3DSTATE_CONSTANT_GS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->MOCS, 8, 11) |
      __gen_uint(values->Buffer0Valid, 12, 12) |
      __gen_uint(values->Buffer1Valid, 13, 13) |
      __gen_uint(values->Buffer2Valid, 14, 14) |
      __gen_uint(values->Buffer3Valid, 15, 15) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   GFX6_3DSTATE_CONSTANT_BODY_pack(data, &dw[1], &values->ConstantBody);
}

#define GFX6_3DSTATE_CONSTANT_PS_length        5
#define GFX6_3DSTATE_CONSTANT_PS_length_bias      2
#define GFX6_3DSTATE_CONSTANT_PS_header         \
   .DWordLength                         =      3,  \
   ._3DCommandSubOpcode                 =     23,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_CONSTANT_PS {
   uint32_t                             DWordLength;
   uint32_t                             MOCS;
   bool                                 Buffer0Valid;
   bool                                 Buffer1Valid;
   bool                                 Buffer2Valid;
   bool                                 Buffer3Valid;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   struct GFX6_3DSTATE_CONSTANT_BODY    ConstantBody;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_CONSTANT_PS_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_3DSTATE_CONSTANT_PS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->MOCS, 8, 11) |
      __gen_uint(values->Buffer0Valid, 12, 12) |
      __gen_uint(values->Buffer1Valid, 13, 13) |
      __gen_uint(values->Buffer2Valid, 14, 14) |
      __gen_uint(values->Buffer3Valid, 15, 15) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   GFX6_3DSTATE_CONSTANT_BODY_pack(data, &dw[1], &values->ConstantBody);
}

#define GFX6_3DSTATE_CONSTANT_VS_length        5
#define GFX6_3DSTATE_CONSTANT_VS_length_bias      2
#define GFX6_3DSTATE_CONSTANT_VS_header         \
   .DWordLength                         =      3,  \
   ._3DCommandSubOpcode                 =     21,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_CONSTANT_VS {
   uint32_t                             DWordLength;
   uint32_t                             MOCS;
   bool                                 Buffer0Valid;
   bool                                 Buffer1Valid;
   bool                                 Buffer2Valid;
   bool                                 Buffer3Valid;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   struct GFX6_3DSTATE_CONSTANT_BODY    ConstantBody;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_CONSTANT_VS_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_3DSTATE_CONSTANT_VS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->MOCS, 8, 11) |
      __gen_uint(values->Buffer0Valid, 12, 12) |
      __gen_uint(values->Buffer1Valid, 13, 13) |
      __gen_uint(values->Buffer2Valid, 14, 14) |
      __gen_uint(values->Buffer3Valid, 15, 15) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   GFX6_3DSTATE_CONSTANT_BODY_pack(data, &dw[1], &values->ConstantBody);
}

#define GFX6_3DSTATE_DEPTH_BUFFER_length       7
#define GFX6_3DSTATE_DEPTH_BUFFER_length_bias      2
#define GFX6_3DSTATE_DEPTH_BUFFER_header        \
   .DWordLength                         =      5,  \
   ._3DCommandSubOpcode                 =      5,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_DEPTH_BUFFER {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             SurfacePitch;
   uint32_t                             SurfaceFormat;
#define D32_FLOAT_S8X24_UINT                     0
#define D32_FLOAT                                1
#define D24_UNORM_S8_UINT                        2
#define D24_UNORM_X8_UINT                        3
#define D16_UNORM                                5
   bool                                 SeparateStencilBufferEnable;
   bool                                 HierarchicalDepthBufferEnable;
   uint32_t                             SoftwareTiledRenderingMode;
#define NORMAL                                   0
#define STR1                                     1
#define STR2                                     3
   uint32_t                             TileWalk;
#define TILEWALK_YMAJOR                          1
   bool                                 TiledSurface;
   uint32_t                             SurfaceType;
#define SURFTYPE_1D                              0
#define SURFTYPE_2D                              1
#define SURFTYPE_3D                              2
#define SURFTYPE_CUBE                            3
#define SURFTYPE_NULL                            7
   __gen_address_type                   SurfaceBaseAddress;
   uint32_t                             MIPMapLayoutMode;
#define MIPLAYOUT_BELOW                          0
#define MIPLAYOUT_RIGHT                          1
   uint32_t                             LOD;
   uint32_t                             Width;
   uint32_t                             Height;
   uint32_t                             RenderTargetViewExtent;
   uint32_t                             MinimumArrayElement;
   uint32_t                             Depth;
   int32_t                              DepthCoordinateOffsetX;
   int32_t                              DepthCoordinateOffsetY;
   uint32_t                             MOCS;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_DEPTH_BUFFER_pack(__attribute__((unused)) __gen_user_data *data,
                               __attribute__((unused)) void * restrict dst,
                               __attribute__((unused)) const struct GFX6_3DSTATE_DEPTH_BUFFER * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->SurfacePitch, 0, 16) |
      __gen_uint(values->SurfaceFormat, 18, 20) |
      __gen_uint(values->SeparateStencilBufferEnable, 21, 21) |
      __gen_uint(values->HierarchicalDepthBufferEnable, 22, 22) |
      __gen_uint(values->SoftwareTiledRenderingMode, 23, 24) |
      __gen_uint(values->TileWalk, 26, 26) |
      __gen_uint(values->TiledSurface, 27, 27) |
      __gen_uint(values->SurfaceType, 29, 31);

   dw[2] = __gen_address(data, &dw[2], values->SurfaceBaseAddress, 0, 0, 31);

   dw[3] =
      __gen_uint(values->MIPMapLayoutMode, 1, 1) |
      __gen_uint(values->LOD, 2, 5) |
      __gen_uint(values->Width, 6, 18) |
      __gen_uint(values->Height, 19, 31);

   dw[4] =
      __gen_uint(values->RenderTargetViewExtent, 1, 9) |
      __gen_uint(values->MinimumArrayElement, 10, 20) |
      __gen_uint(values->Depth, 21, 31);

   dw[5] =
      __gen_sint(values->DepthCoordinateOffsetX, 0, 15) |
      __gen_sint(values->DepthCoordinateOffsetY, 16, 31);

   dw[6] =
      __gen_uint(values->MOCS, 27, 31);
}

#define GFX6_3DSTATE_DRAWING_RECTANGLE_length      4
#define GFX6_3DSTATE_DRAWING_RECTANGLE_length_bias      2
#define GFX6_3DSTATE_DRAWING_RECTANGLE_header   \
   .DWordLength                         =      2,  \
   ._3DCommandSubOpcode                 =      0,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_DRAWING_RECTANGLE {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             ClippedDrawingRectangleXMin;
   uint32_t                             ClippedDrawingRectangleYMin;
   uint32_t                             ClippedDrawingRectangleXMax;
   uint32_t                             ClippedDrawingRectangleYMax;
   int32_t                              DrawingRectangleOriginX;
   int32_t                              DrawingRectangleOriginY;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_DRAWING_RECTANGLE_pack(__attribute__((unused)) __gen_user_data *data,
                                    __attribute__((unused)) void * restrict dst,
                                    __attribute__((unused)) const struct GFX6_3DSTATE_DRAWING_RECTANGLE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->ClippedDrawingRectangleXMin, 0, 15) |
      __gen_uint(values->ClippedDrawingRectangleYMin, 16, 31);

   dw[2] =
      __gen_uint(values->ClippedDrawingRectangleXMax, 0, 15) |
      __gen_uint(values->ClippedDrawingRectangleYMax, 16, 31);

   dw[3] =
      __gen_sint(values->DrawingRectangleOriginX, 0, 15) |
      __gen_sint(values->DrawingRectangleOriginY, 16, 31);
}

#define GFX6_3DSTATE_GS_length                 7
#define GFX6_3DSTATE_GS_length_bias            2
#define GFX6_3DSTATE_GS_header                  \
   .DWordLength                         =      5,  \
   ._3DCommandSubOpcode                 =     17,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_GS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint64_t                             KernelStartPointer;
   bool                                 SoftwareExceptionEnable;
   bool                                 MaskStackExceptionEnable;
   bool                                 IllegalOpcodeExceptionEnable;
   uint32_t                             FloatingPointMode;
#define IEEE754                                  0
#define Alternate                                1
   uint32_t                             ThreadPriority;
#define NormalPriority                           0
#define HighPriority                             1
   uint32_t                             BindingTableEntryCount;
   uint32_t                             SamplerCount;
#define NoSamplers                               0
#define _14Samplers                              1
#define _58Samplers                              2
#define _912Samplers                             3
#define _1316Samplers                            4
   bool                                 VectorMaskEnable;
   bool                                 SingleProgramFlow;
   uint32_t                             PerThreadScratchSpace;
   __gen_address_type                   ScratchSpaceBasePointer;
   uint32_t                             DispatchGRFStartRegisterForURBData;
   uint32_t                             VertexURBEntryReadOffset;
   uint32_t                             VertexURBEntryReadLength;
   uint32_t                             RenderingEnabled;
   bool                                 SOStatisticsEnable;
   bool                                 StatisticsEnable;
   uint32_t                             MaximumNumberofThreads;
   bool                                 Enable;
   uint32_t                             SVBIPostIncrementValue;
   bool                                 SVBIPostIncrementEnable;
   bool                                 SVBIPayloadEnable;
   bool                                 DiscardAdjacency;
   uint32_t                             ReorderMode;
#define LEADING                                  0
#define TRAILING                                 1
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_GS_pack(__attribute__((unused)) __gen_user_data *data,
                     __attribute__((unused)) void * restrict dst,
                     __attribute__((unused)) const struct GFX6_3DSTATE_GS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->KernelStartPointer, 6, 31);

   dw[2] =
      __gen_uint(values->SoftwareExceptionEnable, 7, 7) |
      __gen_uint(values->MaskStackExceptionEnable, 11, 11) |
      __gen_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      __gen_uint(values->FloatingPointMode, 16, 16) |
      __gen_uint(values->ThreadPriority, 17, 17) |
      __gen_uint(values->BindingTableEntryCount, 18, 25) |
      __gen_uint(values->SamplerCount, 27, 29) |
      __gen_uint(values->VectorMaskEnable, 30, 30) |
      __gen_uint(values->SingleProgramFlow, 31, 31);

   const uint32_t v3 =
      __gen_uint(values->PerThreadScratchSpace, 0, 3);
   dw[3] = __gen_address(data, &dw[3], values->ScratchSpaceBasePointer, v3, 10, 31);

   dw[4] =
      __gen_uint(values->DispatchGRFStartRegisterForURBData, 0, 3) |
      __gen_uint(values->VertexURBEntryReadOffset, 4, 9) |
      __gen_uint(values->VertexURBEntryReadLength, 11, 16);

   dw[5] =
      __gen_uint(values->RenderingEnabled, 8, 8) |
      __gen_uint(values->SOStatisticsEnable, 9, 9) |
      __gen_uint(values->StatisticsEnable, 10, 10) |
      __gen_uint(values->MaximumNumberofThreads, 25, 31);

   dw[6] =
      __gen_uint(values->Enable, 15, 15) |
      __gen_uint(values->SVBIPostIncrementValue, 16, 25) |
      __gen_uint(values->SVBIPostIncrementEnable, 27, 27) |
      __gen_uint(values->SVBIPayloadEnable, 28, 28) |
      __gen_uint(values->DiscardAdjacency, 29, 29) |
      __gen_uint(values->ReorderMode, 30, 30);
}

#define GFX6_3DSTATE_GS_SVB_INDEX_length       4
#define GFX6_3DSTATE_GS_SVB_INDEX_length_bias      2
#define GFX6_3DSTATE_GS_SVB_INDEX_header        \
   .DWordLength                         =      2,  \
   ._3DCommandSubOpcode                 =     11,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_GS_SVB_INDEX {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             LoadInternalVertexCount;
   uint32_t                             IndexNumber;
   uint32_t                             StreamedVertexBufferIndex;
   uint32_t                             MaximumIndex;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_GS_SVB_INDEX_pack(__attribute__((unused)) __gen_user_data *data,
                               __attribute__((unused)) void * restrict dst,
                               __attribute__((unused)) const struct GFX6_3DSTATE_GS_SVB_INDEX * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->LoadInternalVertexCount, 0, 0) |
      __gen_uint(values->IndexNumber, 29, 30);

   dw[2] =
      __gen_uint(values->StreamedVertexBufferIndex, 0, 31);

   dw[3] =
      __gen_uint(values->MaximumIndex, 0, 31);
}

#define GFX6_3DSTATE_HIER_DEPTH_BUFFER_length      3
#define GFX6_3DSTATE_HIER_DEPTH_BUFFER_length_bias      2
#define GFX6_3DSTATE_HIER_DEPTH_BUFFER_header   \
   .DWordLength                         =      1,  \
   ._3DCommandSubOpcode                 =     15,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_HIER_DEPTH_BUFFER {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             SurfacePitch;
   uint32_t                             MOCS;
   __gen_address_type                   SurfaceBaseAddress;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_HIER_DEPTH_BUFFER_pack(__attribute__((unused)) __gen_user_data *data,
                                    __attribute__((unused)) void * restrict dst,
                                    __attribute__((unused)) const struct GFX6_3DSTATE_HIER_DEPTH_BUFFER * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->SurfacePitch, 0, 16) |
      __gen_uint(values->MOCS, 25, 28);

   dw[2] = __gen_address(data, &dw[2], values->SurfaceBaseAddress, 0, 0, 31);
}

#define GFX6_3DSTATE_INDEX_BUFFER_length       3
#define GFX6_3DSTATE_INDEX_BUFFER_length_bias      2
#define GFX6_3DSTATE_INDEX_BUFFER_header        \
   .DWordLength                         =      1,  \
   ._3DCommandSubOpcode                 =     10,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_INDEX_BUFFER {
   uint32_t                             DWordLength;
   uint32_t                             IndexFormat;
#define INDEX_BYTE                               0
#define INDEX_WORD                               1
#define INDEX_DWORD                              2
   bool                                 CutIndexEnable;
   uint32_t                             MOCS;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   __gen_address_type                   BufferStartingAddress;
   __gen_address_type                   BufferEndingAddress;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_INDEX_BUFFER_pack(__attribute__((unused)) __gen_user_data *data,
                               __attribute__((unused)) void * restrict dst,
                               __attribute__((unused)) const struct GFX6_3DSTATE_INDEX_BUFFER * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->IndexFormat, 8, 9) |
      __gen_uint(values->CutIndexEnable, 10, 10) |
      __gen_uint(values->MOCS, 12, 15) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] = __gen_address(data, &dw[1], values->BufferStartingAddress, 0, 0, 31);

   dw[2] = __gen_address(data, &dw[2], values->BufferEndingAddress, 0, 0, 31);
}

#define GFX6_3DSTATE_LINE_STIPPLE_length       3
#define GFX6_3DSTATE_LINE_STIPPLE_length_bias      2
#define GFX6_3DSTATE_LINE_STIPPLE_header        \
   .DWordLength                         =      1,  \
   ._3DCommandSubOpcode                 =      8,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_LINE_STIPPLE {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             LineStipplePattern;
   uint32_t                             CurrentStippleIndex;
   uint32_t                             CurrentRepeatCounter;
   bool                                 ModifyEnableCurrentRepeatCounterCurrentStippleIndex;
   uint32_t                             LineStippleRepeatCount;
   float                                LineStippleInverseRepeatCount;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_LINE_STIPPLE_pack(__attribute__((unused)) __gen_user_data *data,
                               __attribute__((unused)) void * restrict dst,
                               __attribute__((unused)) const struct GFX6_3DSTATE_LINE_STIPPLE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->LineStipplePattern, 0, 15) |
      __gen_uint(values->CurrentStippleIndex, 16, 19) |
      __gen_uint(values->CurrentRepeatCounter, 21, 29) |
      __gen_uint(values->ModifyEnableCurrentRepeatCounterCurrentStippleIndex, 31, 31);

   dw[2] =
      __gen_uint(values->LineStippleRepeatCount, 0, 8) |
      __gen_ufixed(values->LineStippleInverseRepeatCount, 16, 31, 13);
}

#define GFX6_3DSTATE_MONOFILTER_SIZE_length      2
#define GFX6_3DSTATE_MONOFILTER_SIZE_length_bias      2
#define GFX6_3DSTATE_MONOFILTER_SIZE_header     \
   .DWordLength                         =      0,  \
   ._3DCommandSubOpcode                 =     17,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_MONOFILTER_SIZE {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             MonochromeFilterHeight;
   uint32_t                             MonochromeFilterWidth;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_MONOFILTER_SIZE_pack(__attribute__((unused)) __gen_user_data *data,
                                  __attribute__((unused)) void * restrict dst,
                                  __attribute__((unused)) const struct GFX6_3DSTATE_MONOFILTER_SIZE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->MonochromeFilterHeight, 0, 2) |
      __gen_uint(values->MonochromeFilterWidth, 3, 5);
}

#define GFX6_3DSTATE_MULTISAMPLE_length        3
#define GFX6_3DSTATE_MULTISAMPLE_length_bias      2
#define GFX6_3DSTATE_MULTISAMPLE_header         \
   .DWordLength                         =      1,  \
   ._3DCommandSubOpcode                 =     13,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_MULTISAMPLE {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             NumberofMultisamples;
#define NUMSAMPLES_1                             0
#define NUMSAMPLES_4                             2
   uint32_t                             PixelLocation;
#define CENTER                                   0
#define UL_CORNER                                1
   float                                Sample0YOffset;
   float                                Sample0XOffset;
   float                                Sample1YOffset;
   float                                Sample1XOffset;
   float                                Sample2YOffset;
   float                                Sample2XOffset;
   float                                Sample3YOffset;
   float                                Sample3XOffset;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_MULTISAMPLE_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_3DSTATE_MULTISAMPLE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->NumberofMultisamples, 1, 3) |
      __gen_uint(values->PixelLocation, 4, 4);

   dw[2] =
      __gen_ufixed(values->Sample0YOffset, 0, 3, 4) |
      __gen_ufixed(values->Sample0XOffset, 4, 7, 4) |
      __gen_ufixed(values->Sample1YOffset, 8, 11, 4) |
      __gen_ufixed(values->Sample1XOffset, 12, 15, 4) |
      __gen_ufixed(values->Sample2YOffset, 16, 19, 4) |
      __gen_ufixed(values->Sample2XOffset, 20, 23, 4) |
      __gen_ufixed(values->Sample3YOffset, 24, 27, 4) |
      __gen_ufixed(values->Sample3XOffset, 28, 31, 4);
}

#define GFX6_3DSTATE_POLY_STIPPLE_OFFSET_length      2
#define GFX6_3DSTATE_POLY_STIPPLE_OFFSET_length_bias      2
#define GFX6_3DSTATE_POLY_STIPPLE_OFFSET_header \
   .DWordLength                         =      0,  \
   ._3DCommandSubOpcode                 =      6,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_POLY_STIPPLE_OFFSET {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             PolygonStippleYOffset;
   uint32_t                             PolygonStippleXOffset;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_POLY_STIPPLE_OFFSET_pack(__attribute__((unused)) __gen_user_data *data,
                                      __attribute__((unused)) void * restrict dst,
                                      __attribute__((unused)) const struct GFX6_3DSTATE_POLY_STIPPLE_OFFSET * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->PolygonStippleYOffset, 0, 4) |
      __gen_uint(values->PolygonStippleXOffset, 8, 12);
}

#define GFX6_3DSTATE_POLY_STIPPLE_PATTERN_length     33
#define GFX6_3DSTATE_POLY_STIPPLE_PATTERN_length_bias      2
#define GFX6_3DSTATE_POLY_STIPPLE_PATTERN_header\
   .DWordLength                         =     31,  \
   ._3DCommandSubOpcode                 =      7,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_POLY_STIPPLE_PATTERN {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             PatternRow[32];
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_POLY_STIPPLE_PATTERN_pack(__attribute__((unused)) __gen_user_data *data,
                                       __attribute__((unused)) void * restrict dst,
                                       __attribute__((unused)) const struct GFX6_3DSTATE_POLY_STIPPLE_PATTERN * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->PatternRow[0], 0, 31);

   dw[2] =
      __gen_uint(values->PatternRow[1], 0, 31);

   dw[3] =
      __gen_uint(values->PatternRow[2], 0, 31);

   dw[4] =
      __gen_uint(values->PatternRow[3], 0, 31);

   dw[5] =
      __gen_uint(values->PatternRow[4], 0, 31);

   dw[6] =
      __gen_uint(values->PatternRow[5], 0, 31);

   dw[7] =
      __gen_uint(values->PatternRow[6], 0, 31);

   dw[8] =
      __gen_uint(values->PatternRow[7], 0, 31);

   dw[9] =
      __gen_uint(values->PatternRow[8], 0, 31);

   dw[10] =
      __gen_uint(values->PatternRow[9], 0, 31);

   dw[11] =
      __gen_uint(values->PatternRow[10], 0, 31);

   dw[12] =
      __gen_uint(values->PatternRow[11], 0, 31);

   dw[13] =
      __gen_uint(values->PatternRow[12], 0, 31);

   dw[14] =
      __gen_uint(values->PatternRow[13], 0, 31);

   dw[15] =
      __gen_uint(values->PatternRow[14], 0, 31);

   dw[16] =
      __gen_uint(values->PatternRow[15], 0, 31);

   dw[17] =
      __gen_uint(values->PatternRow[16], 0, 31);

   dw[18] =
      __gen_uint(values->PatternRow[17], 0, 31);

   dw[19] =
      __gen_uint(values->PatternRow[18], 0, 31);

   dw[20] =
      __gen_uint(values->PatternRow[19], 0, 31);

   dw[21] =
      __gen_uint(values->PatternRow[20], 0, 31);

   dw[22] =
      __gen_uint(values->PatternRow[21], 0, 31);

   dw[23] =
      __gen_uint(values->PatternRow[22], 0, 31);

   dw[24] =
      __gen_uint(values->PatternRow[23], 0, 31);

   dw[25] =
      __gen_uint(values->PatternRow[24], 0, 31);

   dw[26] =
      __gen_uint(values->PatternRow[25], 0, 31);

   dw[27] =
      __gen_uint(values->PatternRow[26], 0, 31);

   dw[28] =
      __gen_uint(values->PatternRow[27], 0, 31);

   dw[29] =
      __gen_uint(values->PatternRow[28], 0, 31);

   dw[30] =
      __gen_uint(values->PatternRow[29], 0, 31);

   dw[31] =
      __gen_uint(values->PatternRow[30], 0, 31);

   dw[32] =
      __gen_uint(values->PatternRow[31], 0, 31);
}

#define GFX6_3DSTATE_SAMPLER_PALETTE_LOAD0_length_bias      2
#define GFX6_3DSTATE_SAMPLER_PALETTE_LOAD0_header\
   ._3DCommandSubOpcode                 =      2,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_SAMPLER_PALETTE_LOAD0 {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_SAMPLER_PALETTE_LOAD0_pack(__attribute__((unused)) __gen_user_data *data,
                                        __attribute__((unused)) void * restrict dst,
                                        __attribute__((unused)) const struct GFX6_3DSTATE_SAMPLER_PALETTE_LOAD0 * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_3DSTATE_SAMPLER_PALETTE_LOAD1_length_bias      2
#define GFX6_3DSTATE_SAMPLER_PALETTE_LOAD1_header\
   .DWordLength                         =      0,  \
   ._3DCommandSubOpcode                 =     12,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_SAMPLER_PALETTE_LOAD1 {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_SAMPLER_PALETTE_LOAD1_pack(__attribute__((unused)) __gen_user_data *data,
                                        __attribute__((unused)) void * restrict dst,
                                        __attribute__((unused)) const struct GFX6_3DSTATE_SAMPLER_PALETTE_LOAD1 * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_3DSTATE_SAMPLER_STATE_POINTERS_length      4
#define GFX6_3DSTATE_SAMPLER_STATE_POINTERS_length_bias      2
#define GFX6_3DSTATE_SAMPLER_STATE_POINTERS_header\
   .DWordLength                         =      2,  \
   ._3DCommandSubOpcode                 =      2,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_SAMPLER_STATE_POINTERS {
   uint32_t                             DWordLength;
   uint32_t                             VSSamplerStateChange;
   uint32_t                             GSSamplerStateChange;
   uint32_t                             PSSamplerStateChange;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint64_t                             PointertoVSSamplerState;
   uint64_t                             PointertoGSSamplerState;
   uint64_t                             PointertoPSSamplerState;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_SAMPLER_STATE_POINTERS_pack(__attribute__((unused)) __gen_user_data *data,
                                         __attribute__((unused)) void * restrict dst,
                                         __attribute__((unused)) const struct GFX6_3DSTATE_SAMPLER_STATE_POINTERS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->VSSamplerStateChange, 8, 8) |
      __gen_uint(values->GSSamplerStateChange, 9, 9) |
      __gen_uint(values->PSSamplerStateChange, 12, 12) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->PointertoVSSamplerState, 5, 31);

   dw[2] =
      __gen_offset(values->PointertoGSSamplerState, 5, 31);

   dw[3] =
      __gen_offset(values->PointertoPSSamplerState, 5, 31);
}

#define GFX6_3DSTATE_SAMPLE_MASK_length        2
#define GFX6_3DSTATE_SAMPLE_MASK_length_bias      2
#define GFX6_3DSTATE_SAMPLE_MASK_header         \
   .DWordLength                         =      0,  \
   ._3DCommandSubOpcode                 =     24,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_SAMPLE_MASK {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             SampleMask;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_SAMPLE_MASK_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_3DSTATE_SAMPLE_MASK * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->SampleMask, 0, 3);
}

#define GFX6_3DSTATE_SCISSOR_STATE_POINTERS_length      2
#define GFX6_3DSTATE_SCISSOR_STATE_POINTERS_length_bias      2
#define GFX6_3DSTATE_SCISSOR_STATE_POINTERS_header\
   .DWordLength                         =      0,  \
   ._3DCommandSubOpcode                 =     15,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_SCISSOR_STATE_POINTERS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint64_t                             ScissorRectPointer;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_SCISSOR_STATE_POINTERS_pack(__attribute__((unused)) __gen_user_data *data,
                                         __attribute__((unused)) void * restrict dst,
                                         __attribute__((unused)) const struct GFX6_3DSTATE_SCISSOR_STATE_POINTERS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->ScissorRectPointer, 5, 31);
}

#define GFX6_3DSTATE_SF_length                20
#define GFX6_3DSTATE_SF_length_bias            2
#define GFX6_3DSTATE_SF_header                  \
   .DWordLength                         =     18,  \
   ._3DCommandSubOpcode                 =     19,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_SF {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             VertexURBEntryReadOffset;
   uint32_t                             VertexURBEntryReadLength;
   uint32_t                             PointSpriteTextureCoordinateOrigin;
#define UPPERLEFT                                0
#define LOWERLEFT                                1
   bool                                 AttributeSwizzleEnable;
   uint32_t                             NumberofSFOutputAttributes;
   uint32_t                             FrontWinding;
#define FRONTWINDING_CW                          0
#define FRONTWINDING_CCW                         1
   bool                                 ViewportTransformEnable;
   uint32_t                             BackFaceFillMode;
#define FILL_MODE_SOLID                          0
#define FILL_MODE_WIREFRAME                      1
#define FILL_MODE_POINT                          2
   uint32_t                             FrontFaceFillMode;
#define FILL_MODE_SOLID                          0
#define FILL_MODE_WIREFRAME                      1
#define FILL_MODE_POINT                          2
   bool                                 GlobalDepthOffsetEnablePoint;
   bool                                 GlobalDepthOffsetEnableWireframe;
   bool                                 GlobalDepthOffsetEnableSolid;
   bool                                 StatisticsEnable;
   bool                                 LegacyGlobalDepthBiasEnable;
   uint32_t                             MultisampleRasterizationMode;
#define MSRASTMODE_OFF_PIXEL                     0
#define MSRASTMODE_OFF_PATTERN                   1
#define MSRASTMODE_ON_PIXEL                      2
#define MSRASTMODE_ON_PATTERN                    3
   bool                                 ScissorRectangleEnable;
   uint32_t                             LineEndCapAntialiasingRegionWidth;
#define _05pixels                                0
#define _10pixels                                1
#define _20pixels                                2
#define _40pixels                                3
   float                                LineWidth;
   uint32_t                             CullMode;
#define CULLMODE_BOTH                            0
#define CULLMODE_NONE                            1
#define CULLMODE_FRONT                           2
#define CULLMODE_BACK                            3
   bool                                 AntialiasingEnable;
   float                                PointWidth;
   uint32_t                             PointWidthSource;
#define Vertex                                   0
#define State                                    1
   uint32_t                             VertexSubPixelPrecisionSelect;
#define _8SubPixelPrecisionBits                  0
#define _4SubPixelPrecisionBits                  1
   uint32_t                             AALineDistanceMode;
#define AALINEDISTANCE_TRUE                      1
   uint32_t                             TriangleFanProvokingVertexSelect;
#define Vertex0                                  0
#define Vertex1                                  1
#define Vertex2                                  2
   uint32_t                             LineStripListProvokingVertexSelect;
#define Vertex0                                  0
#define Vertex1                                  1
   uint32_t                             TriangleStripListProvokingVertexSelect;
#define Vertex0                                  0
#define Vertex1                                  1
#define Vertex2                                  2
   bool                                 LastPixelEnable;
   float                                GlobalDepthOffsetConstant;
   float                                GlobalDepthOffsetScale;
   float                                GlobalDepthOffsetClamp;
   struct GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL Attribute[16];
   uint32_t                             PointSpriteTextureCoordinateEnable;
   uint32_t                             ConstantInterpolationEnable;
   uint32_t                             Attribute0WrapShortestEnables;
   uint32_t                             Attribute1WrapShortestEnables;
   uint32_t                             Attribute2WrapShortestEnables;
   uint32_t                             Attribute3WrapShortestEnables;
   uint32_t                             Attribute4WrapShortestEnables;
   uint32_t                             Attribute5WrapShortestEnables;
   uint32_t                             Attribute6WrapShortestEnables;
   uint32_t                             Attribute7WrapShortestEnables;
   uint32_t                             Attribute8WrapShortestEnables;
   uint32_t                             Attribute9WrapShortestEnables;
   uint32_t                             Attribute10WrapShortestEnables;
   uint32_t                             Attribute11WrapShortestEnables;
   uint32_t                             Attribute12WrapShortestEnables;
   uint32_t                             Attribute13WrapShortestEnables;
   uint32_t                             Attribute14WrapShortestEnables;
   uint32_t                             Attribute15WrapShortestEnables;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_SF_pack(__attribute__((unused)) __gen_user_data *data,
                     __attribute__((unused)) void * restrict dst,
                     __attribute__((unused)) const struct GFX6_3DSTATE_SF * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->VertexURBEntryReadOffset, 4, 9) |
      __gen_uint(values->VertexURBEntryReadLength, 11, 15) |
      __gen_uint(values->PointSpriteTextureCoordinateOrigin, 20, 20) |
      __gen_uint(values->AttributeSwizzleEnable, 21, 21) |
      __gen_uint(values->NumberofSFOutputAttributes, 22, 27);

   dw[2] =
      __gen_uint(values->FrontWinding, 0, 0) |
      __gen_uint(values->ViewportTransformEnable, 1, 1) |
      __gen_uint(values->BackFaceFillMode, 3, 4) |
      __gen_uint(values->FrontFaceFillMode, 5, 6) |
      __gen_uint(values->GlobalDepthOffsetEnablePoint, 7, 7) |
      __gen_uint(values->GlobalDepthOffsetEnableWireframe, 8, 8) |
      __gen_uint(values->GlobalDepthOffsetEnableSolid, 9, 9) |
      __gen_uint(values->StatisticsEnable, 10, 10) |
      __gen_uint(values->LegacyGlobalDepthBiasEnable, 11, 11);

   dw[3] =
      __gen_uint(values->MultisampleRasterizationMode, 8, 9) |
      __gen_uint(values->ScissorRectangleEnable, 11, 11) |
      __gen_uint(values->LineEndCapAntialiasingRegionWidth, 16, 17) |
      __gen_ufixed(values->LineWidth, 18, 27, 7) |
      __gen_uint(values->CullMode, 29, 30) |
      __gen_uint(values->AntialiasingEnable, 31, 31);

   dw[4] =
      __gen_ufixed(values->PointWidth, 0, 10, 3) |
      __gen_uint(values->PointWidthSource, 11, 11) |
      __gen_uint(values->VertexSubPixelPrecisionSelect, 12, 12) |
      __gen_uint(values->AALineDistanceMode, 14, 14) |
      __gen_uint(values->TriangleFanProvokingVertexSelect, 25, 26) |
      __gen_uint(values->LineStripListProvokingVertexSelect, 27, 28) |
      __gen_uint(values->TriangleStripListProvokingVertexSelect, 29, 30) |
      __gen_uint(values->LastPixelEnable, 31, 31);

   dw[5] =
      __gen_float(values->GlobalDepthOffsetConstant);

   dw[6] =
      __gen_float(values->GlobalDepthOffsetScale);

   dw[7] =
      __gen_float(values->GlobalDepthOffsetClamp);

   uint32_t v8_0;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v8_0, &values->Attribute[0]);

   uint32_t v8_1;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v8_1, &values->Attribute[1]);

   dw[8] =
      __gen_uint(v8_0, 0, 15) |
      __gen_uint(v8_1, 16, 31);

   uint32_t v9_0;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v9_0, &values->Attribute[2]);

   uint32_t v9_1;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v9_1, &values->Attribute[3]);

   dw[9] =
      __gen_uint(v9_0, 0, 15) |
      __gen_uint(v9_1, 16, 31);

   uint32_t v10_0;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v10_0, &values->Attribute[4]);

   uint32_t v10_1;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v10_1, &values->Attribute[5]);

   dw[10] =
      __gen_uint(v10_0, 0, 15) |
      __gen_uint(v10_1, 16, 31);

   uint32_t v11_0;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v11_0, &values->Attribute[6]);

   uint32_t v11_1;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v11_1, &values->Attribute[7]);

   dw[11] =
      __gen_uint(v11_0, 0, 15) |
      __gen_uint(v11_1, 16, 31);

   uint32_t v12_0;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v12_0, &values->Attribute[8]);

   uint32_t v12_1;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v12_1, &values->Attribute[9]);

   dw[12] =
      __gen_uint(v12_0, 0, 15) |
      __gen_uint(v12_1, 16, 31);

   uint32_t v13_0;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v13_0, &values->Attribute[10]);

   uint32_t v13_1;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v13_1, &values->Attribute[11]);

   dw[13] =
      __gen_uint(v13_0, 0, 15) |
      __gen_uint(v13_1, 16, 31);

   uint32_t v14_0;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v14_0, &values->Attribute[12]);

   uint32_t v14_1;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v14_1, &values->Attribute[13]);

   dw[14] =
      __gen_uint(v14_0, 0, 15) |
      __gen_uint(v14_1, 16, 31);

   uint32_t v15_0;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v15_0, &values->Attribute[14]);

   uint32_t v15_1;
   GFX6_SF_OUTPUT_ATTRIBUTE_DETAIL_pack(data, &v15_1, &values->Attribute[15]);

   dw[15] =
      __gen_uint(v15_0, 0, 15) |
      __gen_uint(v15_1, 16, 31);

   dw[16] =
      __gen_uint(values->PointSpriteTextureCoordinateEnable, 0, 31);

   dw[17] =
      __gen_uint(values->ConstantInterpolationEnable, 0, 31);

   dw[18] =
      __gen_uint(values->Attribute0WrapShortestEnables, 0, 3) |
      __gen_uint(values->Attribute1WrapShortestEnables, 4, 7) |
      __gen_uint(values->Attribute2WrapShortestEnables, 8, 11) |
      __gen_uint(values->Attribute3WrapShortestEnables, 12, 15) |
      __gen_uint(values->Attribute4WrapShortestEnables, 16, 19) |
      __gen_uint(values->Attribute5WrapShortestEnables, 20, 23) |
      __gen_uint(values->Attribute6WrapShortestEnables, 24, 27) |
      __gen_uint(values->Attribute7WrapShortestEnables, 28, 31);

   dw[19] =
      __gen_uint(values->Attribute8WrapShortestEnables, 0, 3) |
      __gen_uint(values->Attribute9WrapShortestEnables, 4, 7) |
      __gen_uint(values->Attribute10WrapShortestEnables, 8, 11) |
      __gen_uint(values->Attribute11WrapShortestEnables, 12, 15) |
      __gen_uint(values->Attribute12WrapShortestEnables, 16, 19) |
      __gen_uint(values->Attribute13WrapShortestEnables, 20, 23) |
      __gen_uint(values->Attribute14WrapShortestEnables, 24, 27) |
      __gen_uint(values->Attribute15WrapShortestEnables, 28, 31);
}

#define GFX6_3DSTATE_STENCIL_BUFFER_length      3
#define GFX6_3DSTATE_STENCIL_BUFFER_length_bias      2
#define GFX6_3DSTATE_STENCIL_BUFFER_header      \
   .DWordLength                         =      1,  \
   ._3DCommandSubOpcode                 =     14,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_STENCIL_BUFFER {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             SurfacePitch;
   uint32_t                             MOCS;
   __gen_address_type                   SurfaceBaseAddress;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_STENCIL_BUFFER_pack(__attribute__((unused)) __gen_user_data *data,
                                 __attribute__((unused)) void * restrict dst,
                                 __attribute__((unused)) const struct GFX6_3DSTATE_STENCIL_BUFFER * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->SurfacePitch, 0, 16) |
      __gen_uint(values->MOCS, 25, 28);

   dw[2] = __gen_address(data, &dw[2], values->SurfaceBaseAddress, 0, 0, 31);
}

#define GFX6_3DSTATE_URB_length                3
#define GFX6_3DSTATE_URB_length_bias           2
#define GFX6_3DSTATE_URB_header                 \
   .DWordLength                         =      1,  \
   ._3DCommandSubOpcode                 =      5,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_URB {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             VSNumberofURBEntries;
   uint32_t                             VSURBEntryAllocationSize;
   uint32_t                             GSURBEntryAllocationSize;
   uint32_t                             GSNumberofURBEntries;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_URB_pack(__attribute__((unused)) __gen_user_data *data,
                      __attribute__((unused)) void * restrict dst,
                      __attribute__((unused)) const struct GFX6_3DSTATE_URB * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->VSNumberofURBEntries, 0, 15) |
      __gen_uint(values->VSURBEntryAllocationSize, 16, 23);

   dw[2] =
      __gen_uint(values->GSURBEntryAllocationSize, 0, 2) |
      __gen_uint(values->GSNumberofURBEntries, 8, 17);
}

#define GFX6_3DSTATE_VERTEX_BUFFERS_length_bias      2
#define GFX6_3DSTATE_VERTEX_BUFFERS_header      \
   .DWordLength                         =      3,  \
   ._3DCommandSubOpcode                 =      8,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_VERTEX_BUFFERS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_VERTEX_BUFFERS_pack(__attribute__((unused)) __gen_user_data *data,
                                 __attribute__((unused)) void * restrict dst,
                                 __attribute__((unused)) const struct GFX6_3DSTATE_VERTEX_BUFFERS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_3DSTATE_VERTEX_ELEMENTS_length_bias      2
#define GFX6_3DSTATE_VERTEX_ELEMENTS_header     \
   .DWordLength                         =      1,  \
   ._3DCommandSubOpcode                 =      9,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_VERTEX_ELEMENTS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_VERTEX_ELEMENTS_pack(__attribute__((unused)) __gen_user_data *data,
                                  __attribute__((unused)) void * restrict dst,
                                  __attribute__((unused)) const struct GFX6_3DSTATE_VERTEX_ELEMENTS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_3DSTATE_VF_STATISTICS_length      1
#define GFX6_3DSTATE_VF_STATISTICS_length_bias      1
#define GFX6_3DSTATE_VF_STATISTICS_header       \
   ._3DCommandSubOpcode                 =     11,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      1,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_VF_STATISTICS {
   bool                                 StatisticsEnable;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_VF_STATISTICS_pack(__attribute__((unused)) __gen_user_data *data,
                                __attribute__((unused)) void * restrict dst,
                                __attribute__((unused)) const struct GFX6_3DSTATE_VF_STATISTICS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->StatisticsEnable, 0, 0) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_3DSTATE_VIEWPORT_STATE_POINTERS_length      4
#define GFX6_3DSTATE_VIEWPORT_STATE_POINTERS_length_bias      2
#define GFX6_3DSTATE_VIEWPORT_STATE_POINTERS_header\
   .DWordLength                         =      2,  \
   ._3DCommandSubOpcode                 =     13,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_VIEWPORT_STATE_POINTERS {
   uint32_t                             DWordLength;
   uint32_t                             CLIPViewportStateChange;
   uint32_t                             SFViewportStateChange;
   uint32_t                             CCViewportStateChange;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint64_t                             PointertoCLIP_VIEWPORT;
   uint64_t                             PointertoSF_VIEWPORT;
   uint64_t                             PointertoCC_VIEWPORT;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_VIEWPORT_STATE_POINTERS_pack(__attribute__((unused)) __gen_user_data *data,
                                          __attribute__((unused)) void * restrict dst,
                                          __attribute__((unused)) const struct GFX6_3DSTATE_VIEWPORT_STATE_POINTERS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->CLIPViewportStateChange, 10, 10) |
      __gen_uint(values->SFViewportStateChange, 11, 11) |
      __gen_uint(values->CCViewportStateChange, 12, 12) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->PointertoCLIP_VIEWPORT, 5, 31);

   dw[2] =
      __gen_offset(values->PointertoSF_VIEWPORT, 5, 31);

   dw[3] =
      __gen_offset(values->PointertoCC_VIEWPORT, 5, 31);
}

#define GFX6_3DSTATE_VS_length                 6
#define GFX6_3DSTATE_VS_length_bias            2
#define GFX6_3DSTATE_VS_header                  \
   .DWordLength                         =      4,  \
   ._3DCommandSubOpcode                 =     16,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_VS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint64_t                             KernelStartPointer;
   bool                                 SoftwareExceptionEnable;
   bool                                 IllegalOpcodeExceptionEnable;
   uint32_t                             FloatingPointMode;
#define IEEE754                                  0
#define Alternate                                1
   uint32_t                             BindingTableEntryCount;
   uint32_t                             SamplerCount;
#define NoSamplers                               0
#define _14Samplers                              1
#define _58Samplers                              2
#define _912Samplers                             3
#define _1316Samplers                            4
   bool                                 VectorMaskEnable;
   bool                                 SingleVertexDispatch;
   uint32_t                             PerThreadScratchSpace;
   __gen_address_type                   ScratchSpaceBasePointer;
   uint32_t                             VertexURBEntryReadOffset;
   uint32_t                             VertexURBEntryReadLength;
   uint32_t                             DispatchGRFStartRegisterForURBData;
   bool                                 Enable;
   bool                                 VertexCacheDisable;
   bool                                 StatisticsEnable;
   uint32_t                             MaximumNumberofThreads;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_VS_pack(__attribute__((unused)) __gen_user_data *data,
                     __attribute__((unused)) void * restrict dst,
                     __attribute__((unused)) const struct GFX6_3DSTATE_VS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->KernelStartPointer, 6, 31);

   dw[2] =
      __gen_uint(values->SoftwareExceptionEnable, 7, 7) |
      __gen_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      __gen_uint(values->FloatingPointMode, 16, 16) |
      __gen_uint(values->BindingTableEntryCount, 18, 25) |
      __gen_uint(values->SamplerCount, 27, 29) |
      __gen_uint(values->VectorMaskEnable, 30, 30) |
      __gen_uint(values->SingleVertexDispatch, 31, 31);

   const uint32_t v3 =
      __gen_uint(values->PerThreadScratchSpace, 0, 3);
   dw[3] = __gen_address(data, &dw[3], values->ScratchSpaceBasePointer, v3, 10, 31);

   dw[4] =
      __gen_uint(values->VertexURBEntryReadOffset, 4, 9) |
      __gen_uint(values->VertexURBEntryReadLength, 11, 16) |
      __gen_uint(values->DispatchGRFStartRegisterForURBData, 20, 24);

   dw[5] =
      __gen_uint(values->Enable, 0, 0) |
      __gen_uint(values->VertexCacheDisable, 1, 1) |
      __gen_uint(values->StatisticsEnable, 10, 10) |
      __gen_uint(values->MaximumNumberofThreads, 25, 31);
}

#define GFX6_3DSTATE_WM_length                 9
#define GFX6_3DSTATE_WM_length_bias            2
#define GFX6_3DSTATE_WM_header                  \
   .DWordLength                         =      7,  \
   ._3DCommandSubOpcode                 =     20,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_3DSTATE_WM {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint64_t                             KernelStartPointer0;
   bool                                 SoftwareExceptionEnable;
   bool                                 MaskStackExceptionEnable;
   bool                                 IllegalOpcodeExceptionEnable;
   uint32_t                             FloatingPointMode;
#define FLOATING_POINT_MODE_IEEE745              0
#define FLOATING_POINT_MODE_Alternate            1
   uint32_t                             ThreadPriority;
#define High                                     1
   uint32_t                             BindingTableEntryCount;
   uint32_t                             SamplerCount;
   bool                                 VectorMaskEnable;
   bool                                 SingleProgramFlow;
   uint32_t                             PerThreadScratchSpace;
   __gen_address_type                   ScratchSpaceBasePointer;
   uint32_t                             DispatchGRFStartRegisterForConstantSetupData2;
   uint32_t                             DispatchGRFStartRegisterForConstantSetupData1;
   uint32_t                             DispatchGRFStartRegisterForConstantSetupData0;
   bool                                 HierarchicalDepthBufferResolveEnable;
   bool                                 DepthBufferResolveEnable;
   bool                                 DepthBufferClear;
   bool                                 StatisticsEnable;
   bool                                 _8PixelDispatchEnable;
   bool                                 _16PixelDispatchEnable;
   bool                                 _32PixelDispatchEnable;
   bool                                 DualSourceBlendEnable;
   bool                                 PixelShaderUsesSourceW;
   bool                                 oMaskPresenttoRenderTarget;
   bool                                 LineStippleEnable;
   bool                                 PolygonStippleEnable;
   uint32_t                             LineAntialiasingRegionWidth;
#define _05pixels                                0
#define _10pixels                                1
#define _20pixels                                2
#define _40pixels                                3
   uint32_t                             LineEndCapAntialiasingRegionWidth;
#define _05pixels                                0
#define _10pixels                                1
#define _20pixels                                2
#define _40pixels                                3
   bool                                 ThreadDispatchEnable;
   bool                                 PixelShaderUsesSourceDepth;
   bool                                 PixelShaderComputedDepth;
   bool                                 PixelShaderKillsPixel;
   bool                                 LegacyDiamondLineRasterization;
   uint32_t                             MaximumNumberofThreads;
   uint32_t                             MultisampleDispatchMode;
#define MSDISPMODE_PERSAMPLE                     0
#define MSDISPMODE_PERPIXEL                      1
   uint32_t                             MultisampleRasterizationMode;
#define MSRASTMODE_OFF_PIXEL                     0
#define MSRASTMODE_OFF_PATTERN                   1
#define MSRASTMODE_ON_PIXEL                      2
#define MSRASTMODE_ON_PATTERN                    3
   uint32_t                             PointRasterizationRule;
#define RASTRULE_UPPER_LEFT                      0
#define RASTRULE_UPPER_RIGHT                     1
   uint32_t                             BarycentricInterpolationMode;
#define BIM_PERSPECTIVE_PIXEL                    1
#define BIM_PERSPECTIVE_CENTROID                 2
#define BIM_PERSPECTIVE_SAMPLE                   4
#define BIM_LINEAR_PIXEL                         8
#define BIM_LINEAR_CENTROID                      16
#define BIM_LINEAR_SAMPLE                        32
   uint32_t                             PositionZWInterpolationMode;
#define INTERP_PIXEL                             0
#define INTERP_CENTROID                          2
#define INTERP_SAMPLE                            3
   uint32_t                             PositionXYOffsetSelect;
#define POSOFFSET_NONE                           0
#define POSOFFSET_CENTROID                       2
#define POSOFFSET_SAMPLE                         3
   uint32_t                             NumberofSFOutputAttributes;
   uint64_t                             KernelStartPointer1;
   uint64_t                             KernelStartPointer2;
};

static inline __attribute__((always_inline)) void
GFX6_3DSTATE_WM_pack(__attribute__((unused)) __gen_user_data *data,
                     __attribute__((unused)) void * restrict dst,
                     __attribute__((unused)) const struct GFX6_3DSTATE_WM * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->KernelStartPointer0, 6, 31);

   dw[2] =
      __gen_uint(values->SoftwareExceptionEnable, 7, 7) |
      __gen_uint(values->MaskStackExceptionEnable, 11, 11) |
      __gen_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      __gen_uint(values->FloatingPointMode, 16, 16) |
      __gen_uint(values->ThreadPriority, 17, 17) |
      __gen_uint(values->BindingTableEntryCount, 18, 25) |
      __gen_uint(values->SamplerCount, 27, 29) |
      __gen_uint(values->VectorMaskEnable, 30, 30) |
      __gen_uint(values->SingleProgramFlow, 31, 31);

   const uint32_t v3 =
      __gen_uint(values->PerThreadScratchSpace, 0, 3);
   dw[3] = __gen_address(data, &dw[3], values->ScratchSpaceBasePointer, v3, 10, 31);

   dw[4] =
      __gen_uint(values->DispatchGRFStartRegisterForConstantSetupData2, 0, 6) |
      __gen_uint(values->DispatchGRFStartRegisterForConstantSetupData1, 8, 14) |
      __gen_uint(values->DispatchGRFStartRegisterForConstantSetupData0, 16, 22) |
      __gen_uint(values->HierarchicalDepthBufferResolveEnable, 27, 27) |
      __gen_uint(values->DepthBufferResolveEnable, 28, 28) |
      __gen_uint(values->DepthBufferClear, 30, 30) |
      __gen_uint(values->StatisticsEnable, 31, 31);

   dw[5] =
      __gen_uint(values->_8PixelDispatchEnable, 0, 0) |
      __gen_uint(values->_16PixelDispatchEnable, 1, 1) |
      __gen_uint(values->_32PixelDispatchEnable, 2, 2) |
      __gen_uint(values->DualSourceBlendEnable, 7, 7) |
      __gen_uint(values->PixelShaderUsesSourceW, 8, 8) |
      __gen_uint(values->oMaskPresenttoRenderTarget, 9, 9) |
      __gen_uint(values->LineStippleEnable, 11, 11) |
      __gen_uint(values->PolygonStippleEnable, 13, 13) |
      __gen_uint(values->LineAntialiasingRegionWidth, 14, 15) |
      __gen_uint(values->LineEndCapAntialiasingRegionWidth, 16, 17) |
      __gen_uint(values->ThreadDispatchEnable, 19, 19) |
      __gen_uint(values->PixelShaderUsesSourceDepth, 20, 20) |
      __gen_uint(values->PixelShaderComputedDepth, 21, 21) |
      __gen_uint(values->PixelShaderKillsPixel, 22, 22) |
      __gen_uint(values->LegacyDiamondLineRasterization, 23, 23) |
      __gen_uint(values->MaximumNumberofThreads, 25, 31);

   dw[6] =
      __gen_uint(values->MultisampleDispatchMode, 0, 0) |
      __gen_uint(values->MultisampleRasterizationMode, 1, 2) |
      __gen_uint(values->PointRasterizationRule, 9, 9) |
      __gen_uint(values->BarycentricInterpolationMode, 10, 15) |
      __gen_uint(values->PositionZWInterpolationMode, 16, 17) |
      __gen_uint(values->PositionXYOffsetSelect, 18, 19) |
      __gen_uint(values->NumberofSFOutputAttributes, 20, 25);

   dw[7] =
      __gen_offset(values->KernelStartPointer1, 6, 31);

   dw[8] =
      __gen_offset(values->KernelStartPointer2, 6, 31);
}

#define GFX6_MEDIA_CURBE_LOAD_length           4
#define GFX6_MEDIA_CURBE_LOAD_length_bias      2
#define GFX6_MEDIA_CURBE_LOAD_header            \
   .DWordLength                         =      2,  \
   .SubOpcode                           =      1,  \
   .MediaCommandOpcode                  =      0,  \
   .Pipeline                            =      2,  \
   .CommandType                         =      3

struct GFX6_MEDIA_CURBE_LOAD {
   uint32_t                             DWordLength;
   uint32_t                             SubOpcode;
   uint32_t                             MediaCommandOpcode;
   uint32_t                             Pipeline;
   uint32_t                             CommandType;
   uint32_t                             CURBETotalDataLength;
   uint32_t                             CURBEDataStartAddress;
};

static inline __attribute__((always_inline)) void
GFX6_MEDIA_CURBE_LOAD_pack(__attribute__((unused)) __gen_user_data *data,
                           __attribute__((unused)) void * restrict dst,
                           __attribute__((unused)) const struct GFX6_MEDIA_CURBE_LOAD * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 15) |
      __gen_uint(values->SubOpcode, 16, 23) |
      __gen_uint(values->MediaCommandOpcode, 24, 26) |
      __gen_uint(values->Pipeline, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] = 0;

   dw[2] =
      __gen_uint(values->CURBETotalDataLength, 0, 16);

   dw[3] =
      __gen_uint(values->CURBEDataStartAddress, 0, 31);
}

#define GFX6_MEDIA_GATEWAY_STATE_length        2
#define GFX6_MEDIA_GATEWAY_STATE_length_bias      2
#define GFX6_MEDIA_GATEWAY_STATE_header         \
   .DWordLength                         =      0,  \
   .SubOpcode                           =      3,  \
   .MediaCommandOpcode                  =      0,  \
   .Pipeline                            =      2,  \
   .CommandType                         =      3

struct GFX6_MEDIA_GATEWAY_STATE {
   uint32_t                             DWordLength;
   uint32_t                             SubOpcode;
   uint32_t                             MediaCommandOpcode;
   uint32_t                             Pipeline;
   uint32_t                             CommandType;
   uint32_t                             BarrierThreadCount;
   uint32_t                             BarrierByte;
   uint32_t                             BarrierID;
};

static inline __attribute__((always_inline)) void
GFX6_MEDIA_GATEWAY_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_MEDIA_GATEWAY_STATE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 15) |
      __gen_uint(values->SubOpcode, 16, 23) |
      __gen_uint(values->MediaCommandOpcode, 24, 26) |
      __gen_uint(values->Pipeline, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->BarrierThreadCount, 0, 7) |
      __gen_uint(values->BarrierByte, 8, 15) |
      __gen_uint(values->BarrierID, 16, 23);
}

#define GFX6_MEDIA_INTERFACE_DESCRIPTOR_LOAD_length      4
#define GFX6_MEDIA_INTERFACE_DESCRIPTOR_LOAD_length_bias      2
#define GFX6_MEDIA_INTERFACE_DESCRIPTOR_LOAD_header\
   .DWordLength                         =      2,  \
   .SubOpcode                           =      2,  \
   .MediaCommandOpcode                  =      0,  \
   .Pipeline                            =      2,  \
   .CommandType                         =      3

struct GFX6_MEDIA_INTERFACE_DESCRIPTOR_LOAD {
   uint32_t                             DWordLength;
   uint32_t                             SubOpcode;
   uint32_t                             MediaCommandOpcode;
   uint32_t                             Pipeline;
   uint32_t                             CommandType;
   uint32_t                             InterfaceDescriptorTotalLength;
   uint64_t                             InterfaceDescriptorDataStartAddress;
};

static inline __attribute__((always_inline)) void
GFX6_MEDIA_INTERFACE_DESCRIPTOR_LOAD_pack(__attribute__((unused)) __gen_user_data *data,
                                          __attribute__((unused)) void * restrict dst,
                                          __attribute__((unused)) const struct GFX6_MEDIA_INTERFACE_DESCRIPTOR_LOAD * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 15) |
      __gen_uint(values->SubOpcode, 16, 23) |
      __gen_uint(values->MediaCommandOpcode, 24, 26) |
      __gen_uint(values->Pipeline, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] = 0;

   dw[2] =
      __gen_uint(values->InterfaceDescriptorTotalLength, 0, 16);

   dw[3] =
      __gen_offset(values->InterfaceDescriptorDataStartAddress, 0, 31);
}

#define GFX6_MEDIA_OBJECT_length_bias          2
#define GFX6_MEDIA_OBJECT_header                \
   .DWordLength                         =      4,  \
   .MediaCommandSubOpcode               =      0,  \
   .MediaCommandOpcode                  =      1,  \
   .MediaCommandPipeline                =      2,  \
   .CommandType                         =      3

struct GFX6_MEDIA_OBJECT {
   uint32_t                             DWordLength;
   uint32_t                             MediaCommandSubOpcode;
   uint32_t                             MediaCommandOpcode;
   uint32_t                             MediaCommandPipeline;
   uint32_t                             CommandType;
   uint32_t                             InterfaceDescriptorOffset;
   uint32_t                             IndirectDataLength;
   uint32_t                             UseScoreboard;
#define Notusingscoreboard                       0
#define Usingscoreboard                          1
   uint32_t                             ThreadSynchronization;
#define Nothreadsynchronization                  0
#define Threaddispatchissynchronizedbythespawnrootthreadmessage 1
   bool                                 ChildrenPresent;
   __gen_address_type                   IndirectDataStartAddress;
   uint32_t                             ScoreboardX;
   uint32_t                             ScoredboardY;
   uint32_t                             ScoreboardMask;
   uint32_t                             ScoreboardColor;
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX6_MEDIA_OBJECT_pack(__attribute__((unused)) __gen_user_data *data,
                       __attribute__((unused)) void * restrict dst,
                       __attribute__((unused)) const struct GFX6_MEDIA_OBJECT * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 15) |
      __gen_uint(values->MediaCommandSubOpcode, 16, 23) |
      __gen_uint(values->MediaCommandOpcode, 24, 26) |
      __gen_uint(values->MediaCommandPipeline, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->InterfaceDescriptorOffset, 0, 4);

   dw[2] =
      __gen_uint(values->IndirectDataLength, 0, 16) |
      __gen_uint(values->UseScoreboard, 21, 21) |
      __gen_uint(values->ThreadSynchronization, 24, 24) |
      __gen_uint(values->ChildrenPresent, 31, 31);

   dw[3] = __gen_address(data, &dw[3], values->IndirectDataStartAddress, 0, 0, 31);

   dw[4] =
      __gen_uint(values->ScoreboardX, 0, 8) |
      __gen_uint(values->ScoredboardY, 16, 24);

   dw[5] =
      __gen_uint(values->ScoreboardMask, 0, 7) |
      __gen_uint(values->ScoreboardColor, 16, 19);
}

#define GFX6_MEDIA_OBJECT_PRT_length          16
#define GFX6_MEDIA_OBJECT_PRT_length_bias      2
#define GFX6_MEDIA_OBJECT_PRT_header            \
   .DWordLength                         =     14,  \
   .SubOpcode                           =      2,  \
   .MediaCommandOpcode                  =      1,  \
   .Pipeline                            =      2,  \
   .CommandType                         =      3

struct GFX6_MEDIA_OBJECT_PRT {
   uint32_t                             DWordLength;
   uint32_t                             SubOpcode;
   uint32_t                             MediaCommandOpcode;
   uint32_t                             Pipeline;
   uint32_t                             CommandType;
   uint32_t                             InterfaceDescriptorOffset;
   uint32_t                             PRT_FenceType;
#define Rootthreadqueue                          0
#define VFEstateflush                            1
   bool                                 PRT_FenceNeeded;
   bool                                 ChildrenPresent;
   uint32_t                             InlineData[12];
};

static inline __attribute__((always_inline)) void
GFX6_MEDIA_OBJECT_PRT_pack(__attribute__((unused)) __gen_user_data *data,
                           __attribute__((unused)) void * restrict dst,
                           __attribute__((unused)) const struct GFX6_MEDIA_OBJECT_PRT * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 15) |
      __gen_uint(values->SubOpcode, 16, 23) |
      __gen_uint(values->MediaCommandOpcode, 24, 26) |
      __gen_uint(values->Pipeline, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->InterfaceDescriptorOffset, 0, 4);

   dw[2] =
      __gen_uint(values->PRT_FenceType, 22, 22) |
      __gen_uint(values->PRT_FenceNeeded, 23, 23) |
      __gen_uint(values->ChildrenPresent, 31, 31);

   dw[3] = 0;

   dw[4] =
      __gen_uint(values->InlineData[0], 0, 31);

   dw[5] =
      __gen_uint(values->InlineData[1], 0, 31);

   dw[6] =
      __gen_uint(values->InlineData[2], 0, 31);

   dw[7] =
      __gen_uint(values->InlineData[3], 0, 31);

   dw[8] =
      __gen_uint(values->InlineData[4], 0, 31);

   dw[9] =
      __gen_uint(values->InlineData[5], 0, 31);

   dw[10] =
      __gen_uint(values->InlineData[6], 0, 31);

   dw[11] =
      __gen_uint(values->InlineData[7], 0, 31);

   dw[12] =
      __gen_uint(values->InlineData[8], 0, 31);

   dw[13] =
      __gen_uint(values->InlineData[9], 0, 31);

   dw[14] =
      __gen_uint(values->InlineData[10], 0, 31);

   dw[15] =
      __gen_uint(values->InlineData[11], 0, 31);
}

#define GFX6_MEDIA_OBJECT_WALKER_length_bias      2
#define GFX6_MEDIA_OBJECT_WALKER_header         \
   .DWordLength                         =     15,  \
   .SubOpcode                           =      3,  \
   .MediaCommandOpcode                  =      1,  \
   .Pipeline                            =      2,  \
   .CommandType                         =      3

struct GFX6_MEDIA_OBJECT_WALKER {
   uint32_t                             DWordLength;
   uint32_t                             SubOpcode;
   uint32_t                             MediaCommandOpcode;
   uint32_t                             Pipeline;
   uint32_t                             CommandType;
   uint32_t                             InterfaceDescriptorOffset;
   uint32_t                             IndirectDataLength;
   uint32_t                             UseScoreboard;
#define Notusingscoreboard                       0
#define Usingscoreboard                          1
   uint32_t                             ThreadSynchronization;
#define Nothreadsynchronization                  0
#define Threaddispatchissynchronizedbythespawnrootthreadmessage 1
   uint32_t                             ChildrenPresent;
   uint64_t                             IndirectDataStartAddress;
   uint32_t                             ScoreboardMask;
   int32_t                              MidLoopUnitX;
   int32_t                              LocalMidLoopUnitY;
   uint32_t                             MiddleLoopExtraSteps;
   uint32_t                             ColorCountMinusOne;
   uint32_t                             Repel;
   uint32_t                             DualMode;
   uint32_t                             LocalLoopExecCount;
   uint32_t                             GlobalLoopExecCount;
   uint32_t                             BlockResolutionX;
   uint32_t                             BlockResolutionY;
   uint32_t                             LocalStartX;
   uint32_t                             LocalStartY;
   uint32_t                             LocalEndX;
   uint32_t                             LocalEndY;
   int32_t                              LocalOuterLoopStrideX;
   int32_t                              LocalOuterLoopStrideY;
   int32_t                              LocalInnerLoopUnitX;
   int32_t                              LocalInnerLoopUnitY;
   uint32_t                             GlobalResolutionX;
   uint32_t                             GlobalResolutionY;
   int32_t                              GlobalStartX;
   int32_t                              GlobalStartY;
   int32_t                              GlobalOuterLoopStrideX;
   int32_t                              GlobalOuterLoopStrideY;
   int32_t                              GlobalInnerLoopUnitX;
   int32_t                              GlobalInnerLoopUnitY;
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX6_MEDIA_OBJECT_WALKER_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_MEDIA_OBJECT_WALKER * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 15) |
      __gen_uint(values->SubOpcode, 16, 23) |
      __gen_uint(values->MediaCommandOpcode, 24, 26) |
      __gen_uint(values->Pipeline, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->InterfaceDescriptorOffset, 0, 4);

   dw[2] =
      __gen_uint(values->IndirectDataLength, 0, 16) |
      __gen_uint(values->UseScoreboard, 21, 21) |
      __gen_uint(values->ThreadSynchronization, 24, 24) |
      __gen_uint(values->ChildrenPresent, 31, 31);

   dw[3] =
      __gen_offset(values->IndirectDataStartAddress, 0, 31);

   dw[4] = 0;

   dw[5] =
      __gen_uint(values->ScoreboardMask, 0, 7);

   dw[6] =
      __gen_sint(values->MidLoopUnitX, 8, 9) |
      __gen_sint(values->LocalMidLoopUnitY, 12, 13) |
      __gen_uint(values->MiddleLoopExtraSteps, 16, 20) |
      __gen_uint(values->ColorCountMinusOne, 24, 27) |
      __gen_uint(values->Repel, 30, 30) |
      __gen_uint(values->DualMode, 31, 31);

   dw[7] =
      __gen_uint(values->LocalLoopExecCount, 0, 9) |
      __gen_uint(values->GlobalLoopExecCount, 16, 25);

   dw[8] =
      __gen_uint(values->BlockResolutionX, 0, 8) |
      __gen_uint(values->BlockResolutionY, 16, 24);

   dw[9] =
      __gen_uint(values->LocalStartX, 0, 8) |
      __gen_uint(values->LocalStartY, 16, 24);

   dw[10] =
      __gen_uint(values->LocalEndX, 0, 8) |
      __gen_uint(values->LocalEndY, 16, 24);

   dw[11] =
      __gen_sint(values->LocalOuterLoopStrideX, 0, 9) |
      __gen_sint(values->LocalOuterLoopStrideY, 16, 25);

   dw[12] =
      __gen_sint(values->LocalInnerLoopUnitX, 0, 9) |
      __gen_sint(values->LocalInnerLoopUnitY, 16, 25);

   dw[13] =
      __gen_uint(values->GlobalResolutionX, 0, 8) |
      __gen_uint(values->GlobalResolutionY, 16, 24);

   dw[14] =
      __gen_sint(values->GlobalStartX, 0, 9) |
      __gen_sint(values->GlobalStartY, 16, 25);

   dw[15] =
      __gen_sint(values->GlobalOuterLoopStrideX, 0, 9) |
      __gen_sint(values->GlobalOuterLoopStrideY, 16, 25);

   dw[16] =
      __gen_sint(values->GlobalInnerLoopUnitX, 0, 9) |
      __gen_sint(values->GlobalInnerLoopUnitY, 16, 25);
}

#define GFX6_MEDIA_STATE_FLUSH_length          2
#define GFX6_MEDIA_STATE_FLUSH_length_bias      2
#define GFX6_MEDIA_STATE_FLUSH_header           \
   .DWordLength                         =      0,  \
   .SubOpcode                           =      4,  \
   .MediaCommandOpcode                  =      0,  \
   .Pipeline                            =      2,  \
   .CommandType                         =      3

struct GFX6_MEDIA_STATE_FLUSH {
   uint32_t                             DWordLength;
   uint32_t                             SubOpcode;
   uint32_t                             MediaCommandOpcode;
   uint32_t                             Pipeline;
   uint32_t                             CommandType;
   uint32_t                             BarrierMask;
#define NotWaiting                               0
#define Waiting                                  1
   uint32_t                             ThreadCountWaterMark;
};

static inline __attribute__((always_inline)) void
GFX6_MEDIA_STATE_FLUSH_pack(__attribute__((unused)) __gen_user_data *data,
                            __attribute__((unused)) void * restrict dst,
                            __attribute__((unused)) const struct GFX6_MEDIA_STATE_FLUSH * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 15) |
      __gen_uint(values->SubOpcode, 16, 23) |
      __gen_uint(values->MediaCommandOpcode, 24, 26) |
      __gen_uint(values->Pipeline, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->BarrierMask, 0, 15) |
      __gen_uint(values->ThreadCountWaterMark, 16, 23);
}

#define GFX6_MEDIA_VFE_STATE_length            8
#define GFX6_MEDIA_VFE_STATE_length_bias       2
#define GFX6_MEDIA_VFE_STATE_header             \
   .DWordLength                         =      6,  \
   .SubOpcode                           =      0,  \
   .MediaCommandOpcode                  =      0,  \
   .Pipeline                            =      2,  \
   .CommandType                         =      3

struct GFX6_MEDIA_VFE_STATE {
   uint32_t                             DWordLength;
   uint32_t                             SubOpcode;
   uint32_t                             MediaCommandOpcode;
   uint32_t                             Pipeline;
   uint32_t                             CommandType;
   uint32_t                             PerThreadScratchSpace;
   __gen_address_type                   ScratchSpaceBasePointer;
   uint32_t                             FastPreempt;
#define Preemptonly_PRT                          0
   uint32_t                             BypassGatewayControl;
#define MaintainingOpenGatewayForwardMsgCloseGatewayprotocollegacymode 0
#define BypassingOpenGatewayCloseGatewayprotocol 1
   uint32_t                             ResetGatewayTimer;
#define Maintainingtheexistingtimestampstate     0
#define Resettingrelativetimerandlatchingtheglobaltimestamp 1
   uint32_t                             NumberofURBEntries;
   uint32_t                             MaximumNumberofThreads;
   uint32_t                             CURBEAllocationSize;
   uint32_t                             URBEntryAllocationSize;
   uint32_t                             ScoreboardMask;
   uint32_t                             ScoreboardType;
#define StallingScoreboard                       0
   uint32_t                             ScoreboardEnable;
#define Scoreboarddisabled                       0
#define Scoreboardenabled                        1
   int32_t                              Scoreboard0DeltaX;
   int32_t                              Scoreboard0DeltaY;
   int32_t                              Scoreboard1DeltaX;
   int32_t                              Scoreboard1DeltaY;
   int32_t                              Scoreboard2DeltaX;
   int32_t                              Scoreboard2DeltaY;
   int32_t                              Scoreboard3DeltaX;
   int32_t                              Scoreboard3DeltaY;
   int32_t                              Scoreboard4DeltaX;
   int32_t                              Scoreboard4DeltaY;
   int32_t                              Scoreboard5DeltaX;
   int32_t                              Scoreboard5DeltaY;
   int32_t                              Scoreboard6DeltaX;
   int32_t                              Scoreboard6DeltaY;
   int32_t                              Scoreboard7DeltaX;
   int32_t                              Scoreboard7DeltaY;
};

static inline __attribute__((always_inline)) void
GFX6_MEDIA_VFE_STATE_pack(__attribute__((unused)) __gen_user_data *data,
                          __attribute__((unused)) void * restrict dst,
                          __attribute__((unused)) const struct GFX6_MEDIA_VFE_STATE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 15) |
      __gen_uint(values->SubOpcode, 16, 23) |
      __gen_uint(values->MediaCommandOpcode, 24, 26) |
      __gen_uint(values->Pipeline, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   const uint32_t v1 =
      __gen_uint(values->PerThreadScratchSpace, 0, 3);
   dw[1] = __gen_address(data, &dw[1], values->ScratchSpaceBasePointer, v1, 10, 31);

   dw[2] =
      __gen_uint(values->FastPreempt, 5, 5) |
      __gen_uint(values->BypassGatewayControl, 6, 6) |
      __gen_uint(values->ResetGatewayTimer, 7, 7) |
      __gen_uint(values->NumberofURBEntries, 8, 15) |
      __gen_uint(values->MaximumNumberofThreads, 16, 31);

   dw[3] = 0;

   dw[4] =
      __gen_uint(values->CURBEAllocationSize, 0, 15) |
      __gen_uint(values->URBEntryAllocationSize, 16, 31);

   dw[5] =
      __gen_uint(values->ScoreboardMask, 0, 7) |
      __gen_uint(values->ScoreboardType, 30, 30) |
      __gen_uint(values->ScoreboardEnable, 31, 31);

   dw[6] =
      __gen_sint(values->Scoreboard0DeltaX, 0, 3) |
      __gen_sint(values->Scoreboard0DeltaY, 4, 7) |
      __gen_sint(values->Scoreboard1DeltaX, 8, 11) |
      __gen_sint(values->Scoreboard1DeltaY, 12, 15) |
      __gen_sint(values->Scoreboard2DeltaX, 16, 19) |
      __gen_sint(values->Scoreboard2DeltaY, 20, 23) |
      __gen_sint(values->Scoreboard3DeltaX, 24, 27) |
      __gen_sint(values->Scoreboard3DeltaY, 28, 31);

   dw[7] =
      __gen_sint(values->Scoreboard4DeltaX, 0, 3) |
      __gen_sint(values->Scoreboard4DeltaY, 4, 7) |
      __gen_sint(values->Scoreboard5DeltaX, 8, 11) |
      __gen_sint(values->Scoreboard5DeltaY, 12, 15) |
      __gen_sint(values->Scoreboard6DeltaX, 16, 19) |
      __gen_sint(values->Scoreboard6DeltaY, 20, 23) |
      __gen_sint(values->Scoreboard7DeltaX, 24, 27) |
      __gen_sint(values->Scoreboard7DeltaY, 28, 31);
}

#define GFX6_MI_ARB_CHECK_length               1
#define GFX6_MI_ARB_CHECK_length_bias          1
#define GFX6_MI_ARB_CHECK_header                \
   .MICommandOpcode                     =      5,  \
   .CommandType                         =      0

struct GFX6_MI_ARB_CHECK {
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX6_MI_ARB_CHECK_pack(__attribute__((unused)) __gen_user_data *data,
                       __attribute__((unused)) void * restrict dst,
                       __attribute__((unused)) const struct GFX6_MI_ARB_CHECK * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_MI_ARB_ON_OFF_length              1
#define GFX6_MI_ARB_ON_OFF_length_bias         1
#define GFX6_MI_ARB_ON_OFF_header               \
   .MICommandOpcode                     =      8,  \
   .CommandType                         =      0

struct GFX6_MI_ARB_ON_OFF {
   bool                                 ArbitrationEnable;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX6_MI_ARB_ON_OFF_pack(__attribute__((unused)) __gen_user_data *data,
                        __attribute__((unused)) void * restrict dst,
                        __attribute__((unused)) const struct GFX6_MI_ARB_ON_OFF * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->ArbitrationEnable, 0, 0) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_MI_BATCH_BUFFER_END_length        1
#define GFX6_MI_BATCH_BUFFER_END_length_bias      1
#define GFX6_MI_BATCH_BUFFER_END_header         \
   .MICommandOpcode                     =     10,  \
   .CommandType                         =      0

struct GFX6_MI_BATCH_BUFFER_END {
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX6_MI_BATCH_BUFFER_END_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_MI_BATCH_BUFFER_END * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_MI_BATCH_BUFFER_START_length      2
#define GFX6_MI_BATCH_BUFFER_START_length_bias      2
#define GFX6_MI_BATCH_BUFFER_START_header       \
   .DWordLength                         =      0,  \
   .MICommandOpcode                     =     49,  \
   .CommandType                         =      0

struct GFX6_MI_BATCH_BUFFER_START {
   uint32_t                             DWordLength;
   uint32_t                             AddressSpaceIndicator;
#define ASI_GGTT                                 0
#define ASI_PPGTT                                1
   bool                                 ClearCommandBufferEnable;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   __gen_address_type                   BatchBufferStartAddress;
};

static inline __attribute__((always_inline)) void
GFX6_MI_BATCH_BUFFER_START_pack(__attribute__((unused)) __gen_user_data *data,
                                __attribute__((unused)) void * restrict dst,
                                __attribute__((unused)) const struct GFX6_MI_BATCH_BUFFER_START * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->AddressSpaceIndicator, 8, 8) |
      __gen_uint(values->ClearCommandBufferEnable, 11, 11) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] = __gen_address(data, &dw[1], values->BatchBufferStartAddress, 0, 2, 31);
}

#define GFX6_MI_CLFLUSH_length_bias            2
#define GFX6_MI_CLFLUSH_header                  \
   .DWordLength                         =      0,  \
   .MICommandOpcode                     =     39,  \
   .CommandType                         =      0

struct GFX6_MI_CLFLUSH {
   uint32_t                             DWordLength;
   bool                                 UseGlobalGTT;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint32_t                             StartingCachelineOffset;
   __gen_address_type                   PageBaseAddress;
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX6_MI_CLFLUSH_pack(__attribute__((unused)) __gen_user_data *data,
                     __attribute__((unused)) void * restrict dst,
                     __attribute__((unused)) const struct GFX6_MI_CLFLUSH * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 5) |
      __gen_uint(values->UseGlobalGTT, 22, 22) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);

   const uint32_t v1 =
      __gen_uint(values->StartingCachelineOffset, 6, 11);
   dw[1] = __gen_address(data, &dw[1], values->PageBaseAddress, v1, 12, 31);
}

#define GFX6_MI_CONDITIONAL_BATCH_BUFFER_END_length      2
#define GFX6_MI_CONDITIONAL_BATCH_BUFFER_END_length_bias      2
#define GFX6_MI_CONDITIONAL_BATCH_BUFFER_END_header\
   .DWordLength                         =      0,  \
   .CompareSemaphore                    =      0,  \
   .MICommandOpcode                     =     54,  \
   .CommandType                         =      0

struct GFX6_MI_CONDITIONAL_BATCH_BUFFER_END {
   uint32_t                             DWordLength;
   uint32_t                             CompareSemaphore;
   bool                                 UseGlobalGTT;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint32_t                             CompareDataDword;
   __gen_address_type                   CompareAddress;
};

static inline __attribute__((always_inline)) void
GFX6_MI_CONDITIONAL_BATCH_BUFFER_END_pack(__attribute__((unused)) __gen_user_data *data,
                                          __attribute__((unused)) void * restrict dst,
                                          __attribute__((unused)) const struct GFX6_MI_CONDITIONAL_BATCH_BUFFER_END * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->CompareSemaphore, 21, 21) |
      __gen_uint(values->UseGlobalGTT, 22, 22) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->CompareDataDword, 0, 31);
}

#define GFX6_MI_FLUSH_length                   1
#define GFX6_MI_FLUSH_length_bias              1
#define GFX6_MI_FLUSH_header                    \
   .MICommandOpcode                     =      4,  \
   .CommandType                         =      0

struct GFX6_MI_FLUSH {
   uint32_t                             StateInstructionCacheInvalidate;
#define DontInvalidate                           0
#define Invalidate                               1
   uint32_t                             RenderCacheFlushInhibit;
#define Flush                                    0
#define DontFlush                                1
   uint32_t                             GlobalSnapshotCountReset;
#define DontReset                                0
#define Reset                                    1
   bool                                 GenericMediaStateClear;
   bool                                 IndirectStatePointersDisable;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX6_MI_FLUSH_pack(__attribute__((unused)) __gen_user_data *data,
                   __attribute__((unused)) void * restrict dst,
                   __attribute__((unused)) const struct GFX6_MI_FLUSH * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->StateInstructionCacheInvalidate, 1, 1) |
      __gen_uint(values->RenderCacheFlushInhibit, 2, 2) |
      __gen_uint(values->GlobalSnapshotCountReset, 3, 3) |
      __gen_uint(values->GenericMediaStateClear, 4, 4) |
      __gen_uint(values->IndirectStatePointersDisable, 5, 5) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_MI_LOAD_REGISTER_IMM_length       3
#define GFX6_MI_LOAD_REGISTER_IMM_length_bias      2
#define GFX6_MI_LOAD_REGISTER_IMM_header        \
   .DWordLength                         =      1,  \
   .MICommandOpcode                     =     34,  \
   .CommandType                         =      0

struct GFX6_MI_LOAD_REGISTER_IMM {
   uint32_t                             DWordLength;
   uint32_t                             ByteWriteDisables;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint64_t                             RegisterOffset;
   uint32_t                             DataDWord;
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX6_MI_LOAD_REGISTER_IMM_pack(__attribute__((unused)) __gen_user_data *data,
                               __attribute__((unused)) void * restrict dst,
                               __attribute__((unused)) const struct GFX6_MI_LOAD_REGISTER_IMM * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->ByteWriteDisables, 8, 11) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->RegisterOffset, 2, 22);

   dw[2] =
      __gen_uint(values->DataDWord, 0, 31);
}

#define GFX6_MI_LOAD_SCAN_LINES_EXCL_length      2
#define GFX6_MI_LOAD_SCAN_LINES_EXCL_length_bias      2
#define GFX6_MI_LOAD_SCAN_LINES_EXCL_header     \
   .DWordLength                         =      0,  \
   .MICommandOpcode                     =     19,  \
   .CommandType                         =      0

struct GFX6_MI_LOAD_SCAN_LINES_EXCL {
   uint32_t                             DWordLength;
   uint32_t                             DisplayPlaneSelect;
#define DisplayPlaneA                            0
#define DisplayPlaneB                            1
#define DisplayPlaneC                            4
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint32_t                             EndScanLineNumber;
   uint32_t                             StartScanLineNumber;
};

static inline __attribute__((always_inline)) void
GFX6_MI_LOAD_SCAN_LINES_EXCL_pack(__attribute__((unused)) __gen_user_data *data,
                                  __attribute__((unused)) void * restrict dst,
                                  __attribute__((unused)) const struct GFX6_MI_LOAD_SCAN_LINES_EXCL * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 5) |
      __gen_uint(values->DisplayPlaneSelect, 19, 21) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->EndScanLineNumber, 0, 12) |
      __gen_uint(values->StartScanLineNumber, 16, 28);
}

#define GFX6_MI_NOOP_length                    1
#define GFX6_MI_NOOP_length_bias               1
#define GFX6_MI_NOOP_header                     \
   .MICommandOpcode                     =      0,  \
   .CommandType                         =      0

struct GFX6_MI_NOOP {
   uint32_t                             IdentificationNumber;
   bool                                 IdentificationNumberRegisterWriteEnable;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX6_MI_NOOP_pack(__attribute__((unused)) __gen_user_data *data,
                  __attribute__((unused)) void * restrict dst,
                  __attribute__((unused)) const struct GFX6_MI_NOOP * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->IdentificationNumber, 0, 21) |
      __gen_uint(values->IdentificationNumberRegisterWriteEnable, 22, 22) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_MI_REPORT_HEAD_length             1
#define GFX6_MI_REPORT_HEAD_length_bias        1
#define GFX6_MI_REPORT_HEAD_header              \
   .MICommandOpcode                     =      7,  \
   .CommandType                         =      0

struct GFX6_MI_REPORT_HEAD {
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX6_MI_REPORT_HEAD_pack(__attribute__((unused)) __gen_user_data *data,
                         __attribute__((unused)) void * restrict dst,
                         __attribute__((unused)) const struct GFX6_MI_REPORT_HEAD * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_MI_SEMAPHORE_MBOX_length          3
#define GFX6_MI_SEMAPHORE_MBOX_length_bias      2
#define GFX6_MI_SEMAPHORE_MBOX_header           \
   .DWordLength                         =      1,  \
   .MICommandOpcode                     =     22,  \
   .CommandType                         =      0

struct GFX6_MI_SEMAPHORE_MBOX {
   uint32_t                             DWordLength;
   uint32_t                             RegisterSelect;
#define RVSYNC                                   0
#define RBSYNC                                   2
#define UseGeneralRegisterSelect                 3
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint32_t                             SemaphoreDataDword;
};

static inline __attribute__((always_inline)) void
GFX6_MI_SEMAPHORE_MBOX_pack(__attribute__((unused)) __gen_user_data *data,
                            __attribute__((unused)) void * restrict dst,
                            __attribute__((unused)) const struct GFX6_MI_SEMAPHORE_MBOX * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->RegisterSelect, 16, 17) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->SemaphoreDataDword, 0, 31);

   dw[2] = 0;
}

#define GFX6_MI_SET_CONTEXT_length             2
#define GFX6_MI_SET_CONTEXT_length_bias        2
#define GFX6_MI_SET_CONTEXT_header              \
   .DWordLength                         =      0,  \
   .MICommandOpcode                     =     24,  \
   .CommandType                         =      0

struct GFX6_MI_SET_CONTEXT {
   uint32_t                             DWordLength;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint32_t                             RestoreInhibit;
   uint32_t                             ForceRestore;
   bool                                 ExtendedStateRestoreEnable;
   bool                                 ExtendedStateSaveEnable;
   uint32_t                             ReservedMustbe1;
   bool                                 HDDVDContext;
   __gen_address_type                   LogicalContextAddress;
};

static inline __attribute__((always_inline)) void
GFX6_MI_SET_CONTEXT_pack(__attribute__((unused)) __gen_user_data *data,
                         __attribute__((unused)) void * restrict dst,
                         __attribute__((unused)) const struct GFX6_MI_SET_CONTEXT * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);

   const uint32_t v1 =
      __gen_uint(values->RestoreInhibit, 0, 0) |
      __gen_uint(values->ForceRestore, 1, 1) |
      __gen_uint(values->ExtendedStateRestoreEnable, 2, 2) |
      __gen_uint(values->ExtendedStateSaveEnable, 3, 3) |
      __gen_uint(values->ReservedMustbe1, 8, 8) |
      __gen_uint(values->HDDVDContext, 9, 9);
   dw[1] = __gen_address(data, &dw[1], values->LogicalContextAddress, v1, 12, 31);
}

#define GFX6_MI_STORE_DATA_IMM_length          4
#define GFX6_MI_STORE_DATA_IMM_length_bias      2
#define GFX6_MI_STORE_DATA_IMM_header           \
   .DWordLength                         =      2,  \
   .MICommandOpcode                     =     32,  \
   .CommandType                         =      0

struct GFX6_MI_STORE_DATA_IMM {
   uint32_t                             DWordLength;
   bool                                 UseGlobalGTT;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint32_t                             CoreModeEnable;
   __gen_address_type                   Address;
   uint64_t                             ImmediateData;
};

static inline __attribute__((always_inline)) void
GFX6_MI_STORE_DATA_IMM_pack(__attribute__((unused)) __gen_user_data *data,
                            __attribute__((unused)) void * restrict dst,
                            __attribute__((unused)) const struct GFX6_MI_STORE_DATA_IMM * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 5) |
      __gen_uint(values->UseGlobalGTT, 22, 22) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] = 0;

   const uint32_t v2 =
      __gen_uint(values->CoreModeEnable, 0, 0);
   dw[2] = __gen_address(data, &dw[2], values->Address, v2, 2, 31);

   const uint64_t v3 =
      __gen_uint(values->ImmediateData, 0, 63);
   dw[3] = v3;
   dw[4] = v3 >> 32;
}

#define GFX6_MI_STORE_DATA_INDEX_length        3
#define GFX6_MI_STORE_DATA_INDEX_length_bias      2
#define GFX6_MI_STORE_DATA_INDEX_header         \
   .DWordLength                         =      1,  \
   .MICommandOpcode                     =     33,  \
   .CommandType                         =      0

struct GFX6_MI_STORE_DATA_INDEX {
   uint32_t                             DWordLength;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint32_t                             Offset;
   uint32_t                             DataDWord0;
   uint32_t                             DataDWord1;
};

static inline __attribute__((always_inline)) void
GFX6_MI_STORE_DATA_INDEX_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_MI_STORE_DATA_INDEX * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->Offset, 2, 11);

   dw[2] =
      __gen_uint(values->DataDWord0, 0, 31);
}

#define GFX6_MI_STORE_REGISTER_MEM_length      3
#define GFX6_MI_STORE_REGISTER_MEM_length_bias      2
#define GFX6_MI_STORE_REGISTER_MEM_header       \
   .DWordLength                         =      1,  \
   .MICommandOpcode                     =     36,  \
   .CommandType                         =      0

struct GFX6_MI_STORE_REGISTER_MEM {
   uint32_t                             DWordLength;
   bool                                 UseGlobalGTT;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint64_t                             RegisterAddress;
   __gen_address_type                   MemoryAddress;
};

static inline __attribute__((always_inline)) void
GFX6_MI_STORE_REGISTER_MEM_pack(__attribute__((unused)) __gen_user_data *data,
                                __attribute__((unused)) void * restrict dst,
                                __attribute__((unused)) const struct GFX6_MI_STORE_REGISTER_MEM * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->UseGlobalGTT, 22, 22) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->RegisterAddress, 2, 22);

   dw[2] = __gen_address(data, &dw[2], values->MemoryAddress, 0, 2, 31);
}

#define GFX6_MI_SUSPEND_FLUSH_length           1
#define GFX6_MI_SUSPEND_FLUSH_length_bias      1
#define GFX6_MI_SUSPEND_FLUSH_header            \
   .MICommandOpcode                     =     11,  \
   .CommandType                         =      0

struct GFX6_MI_SUSPEND_FLUSH {
   bool                                 SuspendFlush;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX6_MI_SUSPEND_FLUSH_pack(__attribute__((unused)) __gen_user_data *data,
                           __attribute__((unused)) void * restrict dst,
                           __attribute__((unused)) const struct GFX6_MI_SUSPEND_FLUSH * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->SuspendFlush, 0, 0) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_MI_URB_CLEAR_length               2
#define GFX6_MI_URB_CLEAR_length_bias          2
#define GFX6_MI_URB_CLEAR_header                \
   .DWordLength                         =      0,  \
   .MICommandOpcode                     =     25,  \
   .CommandType                         =      0

struct GFX6_MI_URB_CLEAR {
   uint32_t                             DWordLength;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint64_t                             URBAddress;
   uint32_t                             URBClearLength;
};

static inline __attribute__((always_inline)) void
GFX6_MI_URB_CLEAR_pack(__attribute__((unused)) __gen_user_data *data,
                       __attribute__((unused)) void * restrict dst,
                       __attribute__((unused)) const struct GFX6_MI_URB_CLEAR * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->URBAddress, 0, 13) |
      __gen_uint(values->URBClearLength, 16, 28);
}

#define GFX6_MI_USER_INTERRUPT_length          1
#define GFX6_MI_USER_INTERRUPT_length_bias      1
#define GFX6_MI_USER_INTERRUPT_header           \
   .MICommandOpcode                     =      2,  \
   .CommandType                         =      0

struct GFX6_MI_USER_INTERRUPT {
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX6_MI_USER_INTERRUPT_pack(__attribute__((unused)) __gen_user_data *data,
                            __attribute__((unused)) void * restrict dst,
                            __attribute__((unused)) const struct GFX6_MI_USER_INTERRUPT * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_MI_WAIT_FOR_EVENT_length          1
#define GFX6_MI_WAIT_FOR_EVENT_length_bias      1
#define GFX6_MI_WAIT_FOR_EVENT_header           \
   .MICommandOpcode                     =      3,  \
   .CommandType                         =      0

struct GFX6_MI_WAIT_FOR_EVENT {
   bool                                 DisplayPipeAScanLineWaitEnable;
   bool                                 DisplayPlaneAFlipPendingWaitEnable;
   bool                                 DisplaySpriteAFlipPendingWaitEnable;
   bool                                 DisplayPipeAVerticalBlankWaitEnable;
   bool                                 DisplayPipeAHorizontalBlankWaitEnable;
   bool                                 DisplayPipeBScanLineWaitEnable;
   bool                                 DisplayPlaneBFlipPendingWaitEnable;
   bool                                 DisplaySpriteBFlipPendingWaitEnable;
   bool                                 DisplayPipeBVerticalBlankWaitEnable;
   bool                                 DisplayPipeBHorizontalBlankWaitEnable;
   uint32_t                             ConditionCodeWaitSelect;
#define Notenabled                               0
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX6_MI_WAIT_FOR_EVENT_pack(__attribute__((unused)) __gen_user_data *data,
                            __attribute__((unused)) void * restrict dst,
                            __attribute__((unused)) const struct GFX6_MI_WAIT_FOR_EVENT * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DisplayPipeAScanLineWaitEnable, 0, 0) |
      __gen_uint(values->DisplayPlaneAFlipPendingWaitEnable, 1, 1) |
      __gen_uint(values->DisplaySpriteAFlipPendingWaitEnable, 2, 2) |
      __gen_uint(values->DisplayPipeAVerticalBlankWaitEnable, 3, 3) |
      __gen_uint(values->DisplayPipeAHorizontalBlankWaitEnable, 5, 5) |
      __gen_uint(values->DisplayPipeBScanLineWaitEnable, 8, 8) |
      __gen_uint(values->DisplayPlaneBFlipPendingWaitEnable, 9, 9) |
      __gen_uint(values->DisplaySpriteBFlipPendingWaitEnable, 10, 10) |
      __gen_uint(values->DisplayPipeBVerticalBlankWaitEnable, 11, 11) |
      __gen_uint(values->DisplayPipeBHorizontalBlankWaitEnable, 13, 13) |
      __gen_uint(values->ConditionCodeWaitSelect, 16, 19) |
      __gen_uint(values->MICommandOpcode, 23, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_PIPELINE_SELECT_length            1
#define GFX6_PIPELINE_SELECT_length_bias       1
#define GFX6_PIPELINE_SELECT_header             \
   ._3DCommandSubOpcode                 =      4,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      1,  \
   .CommandType                         =      3

struct GFX6_PIPELINE_SELECT {
   uint32_t                             PipelineSelection;
#define _3D                                      0
#define Media                                    1
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX6_PIPELINE_SELECT_pack(__attribute__((unused)) __gen_user_data *data,
                          __attribute__((unused)) void * restrict dst,
                          __attribute__((unused)) const struct GFX6_PIPELINE_SELECT * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->PipelineSelection, 0, 1) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);
}

#define GFX6_PIPE_CONTROL_length               5
#define GFX6_PIPE_CONTROL_length_bias          2
#define GFX6_PIPE_CONTROL_header                \
   .DWordLength                         =      3,  \
   ._3DCommandSubOpcode                 =      0,  \
   ._3DCommandOpcode                    =      2,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX6_PIPE_CONTROL {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   bool                                 DepthCacheFlushEnable;
   bool                                 StallAtPixelScoreboard;
   bool                                 StateCacheInvalidationEnable;
   bool                                 ConstantCacheInvalidationEnable;
   bool                                 VFCacheInvalidationEnable;
   bool                                 NotifyEnable;
   bool                                 IndirectStatePointersDisable;
   bool                                 TextureCacheInvalidationEnable;
   bool                                 InstructionCacheInvalidateEnable;
   bool                                 RenderTargetCacheFlushEnable;
   bool                                 DepthStallEnable;
   uint32_t                             PostSyncOperation;
#define NoWrite                                  0
#define WriteImmediateData                       1
#define WritePSDepthCount                        2
#define WriteTimestamp                           3
   bool                                 GenericMediaStateClear;
   bool                                 SynchronizeGFDTSurface;
   bool                                 TLBInvalidate;
   bool                                 GlobalSnapshotCountReset;
   bool                                 CommandStreamerStallEnable;
   uint32_t                             StoreDataIndex;
   uint32_t                             DestinationAddressType;
#define DAT_GGTT                                 1
   __gen_address_type                   Address;
   uint64_t                             ImmediateData;
};

static inline __attribute__((always_inline)) void
GFX6_PIPE_CONTROL_pack(__attribute__((unused)) __gen_user_data *data,
                       __attribute__((unused)) void * restrict dst,
                       __attribute__((unused)) const struct GFX6_PIPE_CONTROL * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_uint(values->DepthCacheFlushEnable, 0, 0) |
      __gen_uint(values->StallAtPixelScoreboard, 1, 1) |
      __gen_uint(values->StateCacheInvalidationEnable, 2, 2) |
      __gen_uint(values->ConstantCacheInvalidationEnable, 3, 3) |
      __gen_uint(values->VFCacheInvalidationEnable, 4, 4) |
      __gen_uint(values->NotifyEnable, 8, 8) |
      __gen_uint(values->IndirectStatePointersDisable, 9, 9) |
      __gen_uint(values->TextureCacheInvalidationEnable, 10, 10) |
      __gen_uint(values->InstructionCacheInvalidateEnable, 11, 11) |
      __gen_uint(values->RenderTargetCacheFlushEnable, 12, 12) |
      __gen_uint(values->DepthStallEnable, 13, 13) |
      __gen_uint(values->PostSyncOperation, 14, 15) |
      __gen_uint(values->GenericMediaStateClear, 16, 16) |
      __gen_uint(values->SynchronizeGFDTSurface, 17, 17) |
      __gen_uint(values->TLBInvalidate, 18, 18) |
      __gen_uint(values->GlobalSnapshotCountReset, 19, 19) |
      __gen_uint(values->CommandStreamerStallEnable, 20, 20) |
      __gen_uint(values->StoreDataIndex, 21, 21);

   const uint32_t v2 =
      __gen_uint(values->DestinationAddressType, 2, 2);
   dw[2] = __gen_address(data, &dw[2], values->Address, v2, 3, 31);

   const uint64_t v3 =
      __gen_uint(values->ImmediateData, 0, 63);
   dw[3] = v3;
   dw[4] = v3 >> 32;
}

#define GFX6_STATE_BASE_ADDRESS_length        10
#define GFX6_STATE_BASE_ADDRESS_length_bias      2
#define GFX6_STATE_BASE_ADDRESS_header          \
   .DWordLength                         =      8,  \
   ._3DCommandSubOpcode                 =      1,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      0,  \
   .CommandType                         =      3

struct GFX6_STATE_BASE_ADDRESS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   bool                                 GeneralStateBaseAddressModifyEnable;
   uint32_t                             StatelessDataPortAccessForceWriteThru;
   uint32_t                             StatelessDataPortAccessMOCS;
   uint32_t                             GeneralStateMOCS;
   __gen_address_type                   GeneralStateBaseAddress;
   bool                                 SurfaceStateBaseAddressModifyEnable;
   uint32_t                             SurfaceStateMOCS;
   __gen_address_type                   SurfaceStateBaseAddress;
   bool                                 DynamicStateBaseAddressModifyEnable;
   uint32_t                             DynamicStateMOCS;
   __gen_address_type                   DynamicStateBaseAddress;
   bool                                 IndirectObjectBaseAddressModifyEnable;
   uint32_t                             IndirectObjectMOCS;
   __gen_address_type                   IndirectObjectBaseAddress;
   bool                                 InstructionBaseAddressModifyEnable;
   uint32_t                             InstructionMOCS;
   __gen_address_type                   InstructionBaseAddress;
   bool                                 GeneralStateAccessUpperBoundModifyEnable;
   __gen_address_type                   GeneralStateAccessUpperBound;
   bool                                 DynamicStateAccessUpperBoundModifyEnable;
   __gen_address_type                   DynamicStateAccessUpperBound;
   bool                                 IndirectObjectAccessUpperBoundModifyEnable;
   __gen_address_type                   IndirectObjectAccessUpperBound;
   bool                                 InstructionAccessUpperBoundModifyEnable;
   __gen_address_type                   InstructionAccessUpperBound;
};

static inline __attribute__((always_inline)) void
GFX6_STATE_BASE_ADDRESS_pack(__attribute__((unused)) __gen_user_data *data,
                             __attribute__((unused)) void * restrict dst,
                             __attribute__((unused)) const struct GFX6_STATE_BASE_ADDRESS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   const uint32_t v1 =
      __gen_uint(values->GeneralStateBaseAddressModifyEnable, 0, 0) |
      __gen_uint(values->StatelessDataPortAccessForceWriteThru, 3, 3) |
      __gen_uint(values->StatelessDataPortAccessMOCS, 4, 7) |
      __gen_uint(values->GeneralStateMOCS, 8, 11);
   dw[1] = __gen_address(data, &dw[1], values->GeneralStateBaseAddress, v1, 12, 31);

   const uint32_t v2 =
      __gen_uint(values->SurfaceStateBaseAddressModifyEnable, 0, 0) |
      __gen_uint(values->SurfaceStateMOCS, 8, 11);
   dw[2] = __gen_address(data, &dw[2], values->SurfaceStateBaseAddress, v2, 12, 31);

   const uint32_t v3 =
      __gen_uint(values->DynamicStateBaseAddressModifyEnable, 0, 0) |
      __gen_uint(values->DynamicStateMOCS, 8, 11);
   dw[3] = __gen_address(data, &dw[3], values->DynamicStateBaseAddress, v3, 12, 31);

   const uint32_t v4 =
      __gen_uint(values->IndirectObjectBaseAddressModifyEnable, 0, 0) |
      __gen_uint(values->IndirectObjectMOCS, 8, 11);
   dw[4] = __gen_address(data, &dw[4], values->IndirectObjectBaseAddress, v4, 12, 31);

   const uint32_t v5 =
      __gen_uint(values->InstructionBaseAddressModifyEnable, 0, 0) |
      __gen_uint(values->InstructionMOCS, 8, 11);
   dw[5] = __gen_address(data, &dw[5], values->InstructionBaseAddress, v5, 12, 31);

   const uint32_t v6 =
      __gen_uint(values->GeneralStateAccessUpperBoundModifyEnable, 0, 0);
   dw[6] = __gen_address(data, &dw[6], values->GeneralStateAccessUpperBound, v6, 12, 31);

   const uint32_t v7 =
      __gen_uint(values->DynamicStateAccessUpperBoundModifyEnable, 0, 0);
   dw[7] = __gen_address(data, &dw[7], values->DynamicStateAccessUpperBound, v7, 12, 31);

   const uint32_t v8 =
      __gen_uint(values->IndirectObjectAccessUpperBoundModifyEnable, 0, 0);
   dw[8] = __gen_address(data, &dw[8], values->IndirectObjectAccessUpperBound, v8, 12, 31);

   const uint32_t v9 =
      __gen_uint(values->InstructionAccessUpperBoundModifyEnable, 0, 0);
   dw[9] = __gen_address(data, &dw[9], values->InstructionAccessUpperBound, v9, 12, 31);
}

#define GFX6_STATE_PREFETCH_length             2
#define GFX6_STATE_PREFETCH_length_bias        2
#define GFX6_STATE_PREFETCH_header              \
   .DWordLength                         =      0,  \
   ._3DCommandSubOpcode                 =      3,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      0,  \
   .CommandType                         =      3

struct GFX6_STATE_PREFETCH {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             PrefetchCount;
   __gen_address_type                   PrefetchPointer;
};

static inline __attribute__((always_inline)) void
GFX6_STATE_PREFETCH_pack(__attribute__((unused)) __gen_user_data *data,
                         __attribute__((unused)) void * restrict dst,
                         __attribute__((unused)) const struct GFX6_STATE_PREFETCH * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   const uint32_t v1 =
      __gen_uint(values->PrefetchCount, 0, 2);
   dw[1] = __gen_address(data, &dw[1], values->PrefetchPointer, v1, 6, 31);
}

#define GFX6_STATE_SIP_length                  2
#define GFX6_STATE_SIP_length_bias             2
#define GFX6_STATE_SIP_header                   \
   .DWordLength                         =      0,  \
   ._3DCommandSubOpcode                 =      2,  \
   ._3DCommandOpcode                    =      1,  \
   .CommandSubType                      =      0,  \
   .CommandType                         =      3

struct GFX6_STATE_SIP {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint64_t                             SystemInstructionPointer;
};

static inline __attribute__((always_inline)) void
GFX6_STATE_SIP_pack(__attribute__((unused)) __gen_user_data *data,
                    __attribute__((unused)) void * restrict dst,
                    __attribute__((unused)) const struct GFX6_STATE_SIP * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->DWordLength, 0, 7) |
      __gen_uint(values->_3DCommandSubOpcode, 16, 23) |
      __gen_uint(values->_3DCommandOpcode, 24, 26) |
      __gen_uint(values->CommandSubType, 27, 28) |
      __gen_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->SystemInstructionPointer, 4, 31);
}

#define GFX6_BCS_FAULT_REG_num            0x4294
#define GFX6_BCS_FAULT_REG_length              1
struct GFX6_BCS_FAULT_REG {
   bool                                 ValidBit;
   uint32_t                             FaultType;
#define PageFault                                0
#define InvalidPDFault                           1
#define UnloadedPDFault                          2
#define InvalidandUnloadedPDfault                3
   uint32_t                             SRCIDofFault;
   uint32_t                             GTTSEL;
#define PPGTT                                    0
#define GGTT                                     1
   __gen_address_type                   VirtualAddressofFault;
};

static inline __attribute__((always_inline)) void
GFX6_BCS_FAULT_REG_pack(__attribute__((unused)) __gen_user_data *data,
                        __attribute__((unused)) void * restrict dst,
                        __attribute__((unused)) const struct GFX6_BCS_FAULT_REG * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   const uint32_t v0 =
      __gen_uint(values->ValidBit, 0, 0) |
      __gen_uint(values->FaultType, 1, 2) |
      __gen_uint(values->SRCIDofFault, 3, 10) |
      __gen_uint(values->GTTSEL, 11, 11);
   dw[0] = __gen_address(data, &dw[0], values->VirtualAddressofFault, v0, 12, 31);
}

#define GFX6_BCS_INSTDONE_num             0x2206c
#define GFX6_BCS_INSTDONE_length               1
struct GFX6_BCS_INSTDONE {
   bool                                 RingEnable;
   bool                                 BlitterIDLE;
   bool                                 GABIDLE;
   bool                                 BCSDone;
};

static inline __attribute__((always_inline)) void
GFX6_BCS_INSTDONE_pack(__attribute__((unused)) __gen_user_data *data,
                       __attribute__((unused)) void * restrict dst,
                       __attribute__((unused)) const struct GFX6_BCS_INSTDONE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->RingEnable, 0, 0) |
      __gen_uint(values->BlitterIDLE, 1, 1) |
      __gen_uint(values->GABIDLE, 2, 2) |
      __gen_uint(values->BCSDone, 3, 3);
}

#define GFX6_BCS_RING_BUFFER_CTL_num      0x2203c
#define GFX6_BCS_RING_BUFFER_CTL_length        1
struct GFX6_BCS_RING_BUFFER_CTL {
   bool                                 RingBufferEnable;
   uint32_t                             AutomaticReportHeadPointer;
#define MI_AUTOREPORT_OFF                        0
#define MI_AUTOREPORT_64KB                       1
#define MI_AUTOREPORT_4KB                        2
#define MI_AUTOREPORT_128KB                      3
   bool                                 DisableRegisterAccesses;
   bool                                 SemaphoreWait;
   bool                                 RBWait;
   uint32_t                             BufferLengthinpages1;
};

static inline __attribute__((always_inline)) void
GFX6_BCS_RING_BUFFER_CTL_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_BCS_RING_BUFFER_CTL * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->RingBufferEnable, 0, 0) |
      __gen_uint(values->AutomaticReportHeadPointer, 1, 2) |
      __gen_uint(values->DisableRegisterAccesses, 8, 8) |
      __gen_uint(values->SemaphoreWait, 10, 10) |
      __gen_uint(values->RBWait, 11, 11) |
      __gen_uint(values->BufferLengthinpages1, 12, 20);
}

#define GFX6_GFX_ARB_ERROR_RPT_num        0x40a0
#define GFX6_GFX_ARB_ERROR_RPT_length          1
struct GFX6_GFX_ARB_ERROR_RPT {
   bool                                 TLBPageFaultError;
   bool                                 ContextPageFaultError;
   bool                                 InvalidPageDirectoryentryerror;
   bool                                 HardwareStatusPageFaultError;
   bool                                 TLBPageVTDTranslationError;
   bool                                 ContextPageVTDTranslationError;
   bool                                 PageDirectoryEntryVTDTranslationError;
   bool                                 HardwareStatusPageVTDTranslationError;
   bool                                 UnloadedPDError;
};

static inline __attribute__((always_inline)) void
GFX6_GFX_ARB_ERROR_RPT_pack(__attribute__((unused)) __gen_user_data *data,
                            __attribute__((unused)) void * restrict dst,
                            __attribute__((unused)) const struct GFX6_GFX_ARB_ERROR_RPT * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->TLBPageFaultError, 0, 0) |
      __gen_uint(values->ContextPageFaultError, 1, 1) |
      __gen_uint(values->InvalidPageDirectoryentryerror, 2, 2) |
      __gen_uint(values->HardwareStatusPageFaultError, 3, 3) |
      __gen_uint(values->TLBPageVTDTranslationError, 4, 4) |
      __gen_uint(values->ContextPageVTDTranslationError, 5, 5) |
      __gen_uint(values->PageDirectoryEntryVTDTranslationError, 6, 6) |
      __gen_uint(values->HardwareStatusPageVTDTranslationError, 7, 7) |
      __gen_uint(values->UnloadedPDError, 8, 8);
}

#define GFX6_INSTDONE_1_num               0x206c
#define GFX6_INSTDONE_1_length                 1
struct GFX6_INSTDONE_1 {
   bool                                 PRB0RingEnable;
   bool                                 AVSDone;
   bool                                 HIZDone;
   bool                                 GWDone;
   bool                                 TSDone;
   bool                                 TDDone;
   bool                                 VFEDone;
   bool                                 IEFDone;
   bool                                 VSCDone;
   bool                                 ISC23Done;
   bool                                 ISC10Done;
   bool                                 IC0Done;
   bool                                 IC1Done;
   bool                                 IC2Done;
   bool                                 IC3Done;
   bool                                 EU00Done;
   bool                                 EU01Done;
   bool                                 EU02Done;
   bool                                 MA0Done;
   bool                                 EU10Done;
   bool                                 EU11Done;
   bool                                 EU12Done;
   bool                                 MA1Done;
   bool                                 EU20Done;
   bool                                 EU21Done;
   bool                                 EU22Done;
   bool                                 MA2Done;
   bool                                 EU30Done;
   bool                                 EU31Done;
   bool                                 EU32Done;
   bool                                 MA3Done;
};

static inline __attribute__((always_inline)) void
GFX6_INSTDONE_1_pack(__attribute__((unused)) __gen_user_data *data,
                     __attribute__((unused)) void * restrict dst,
                     __attribute__((unused)) const struct GFX6_INSTDONE_1 * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->PRB0RingEnable, 0, 0) |
      __gen_uint(values->AVSDone, 1, 1) |
      __gen_uint(values->HIZDone, 2, 2) |
      __gen_uint(values->GWDone, 3, 3) |
      __gen_uint(values->TSDone, 4, 4) |
      __gen_uint(values->TDDone, 6, 6) |
      __gen_uint(values->VFEDone, 7, 7) |
      __gen_uint(values->IEFDone, 8, 8) |
      __gen_uint(values->VSCDone, 9, 9) |
      __gen_uint(values->ISC23Done, 10, 10) |
      __gen_uint(values->ISC10Done, 11, 11) |
      __gen_uint(values->IC0Done, 12, 12) |
      __gen_uint(values->IC1Done, 13, 13) |
      __gen_uint(values->IC2Done, 14, 14) |
      __gen_uint(values->IC3Done, 15, 15) |
      __gen_uint(values->EU00Done, 16, 16) |
      __gen_uint(values->EU01Done, 17, 17) |
      __gen_uint(values->EU02Done, 18, 18) |
      __gen_uint(values->MA0Done, 19, 19) |
      __gen_uint(values->EU10Done, 20, 20) |
      __gen_uint(values->EU11Done, 21, 21) |
      __gen_uint(values->EU12Done, 22, 22) |
      __gen_uint(values->MA1Done, 23, 23) |
      __gen_uint(values->EU20Done, 24, 24) |
      __gen_uint(values->EU21Done, 25, 25) |
      __gen_uint(values->EU22Done, 26, 26) |
      __gen_uint(values->MA2Done, 27, 27) |
      __gen_uint(values->EU30Done, 28, 28) |
      __gen_uint(values->EU31Done, 29, 29) |
      __gen_uint(values->EU32Done, 30, 30) |
      __gen_uint(values->MA3Done, 31, 31);
}

#define GFX6_INSTDONE_2_num               0x207c
#define GFX6_INSTDONE_2_length                 1
struct GFX6_INSTDONE_2 {
   bool                                 VFDone;
   bool                                 VS0Done;
   bool                                 GSDone;
   bool                                 CLDone;
   bool                                 SFDone;
   bool                                 VMEDone;
   bool                                 PLDone;
   bool                                 SODone;
   bool                                 SIDone;
   bool                                 DGDone;
   bool                                 FTDone;
   bool                                 DMDone;
   bool                                 SCDone;
   bool                                 FLDone;
   bool                                 QCDone;
   bool                                 SVSMDone;
   bool                                 WMFEDone;
   bool                                 IZDone;
   bool                                 PSDDone;
   bool                                 DAPDone;
   bool                                 RCZDone;
   bool                                 VDIDone;
   bool                                 RCPBEDone;
   bool                                 RCPFEDone;
   bool                                 MTDone;
   bool                                 ISCDone;
   bool                                 SVGDone;
   bool                                 RCCDone;
   bool                                 SVRWDone;
   bool                                 WMBEDone;
   bool                                 CSDone;
   bool                                 GAMDone;
};

static inline __attribute__((always_inline)) void
GFX6_INSTDONE_2_pack(__attribute__((unused)) __gen_user_data *data,
                     __attribute__((unused)) void * restrict dst,
                     __attribute__((unused)) const struct GFX6_INSTDONE_2 * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->VFDone, 0, 0) |
      __gen_uint(values->VS0Done, 1, 1) |
      __gen_uint(values->GSDone, 2, 2) |
      __gen_uint(values->CLDone, 3, 3) |
      __gen_uint(values->SFDone, 4, 4) |
      __gen_uint(values->VMEDone, 5, 5) |
      __gen_uint(values->PLDone, 6, 6) |
      __gen_uint(values->SODone, 7, 7) |
      __gen_uint(values->SIDone, 8, 8) |
      __gen_uint(values->DGDone, 9, 9) |
      __gen_uint(values->FTDone, 10, 10) |
      __gen_uint(values->DMDone, 11, 11) |
      __gen_uint(values->SCDone, 12, 12) |
      __gen_uint(values->FLDone, 13, 13) |
      __gen_uint(values->QCDone, 14, 14) |
      __gen_uint(values->SVSMDone, 15, 15) |
      __gen_uint(values->WMFEDone, 16, 16) |
      __gen_uint(values->IZDone, 17, 17) |
      __gen_uint(values->PSDDone, 18, 18) |
      __gen_uint(values->DAPDone, 19, 19) |
      __gen_uint(values->RCZDone, 20, 20) |
      __gen_uint(values->VDIDone, 21, 21) |
      __gen_uint(values->RCPBEDone, 22, 22) |
      __gen_uint(values->RCPFEDone, 23, 23) |
      __gen_uint(values->MTDone, 24, 24) |
      __gen_uint(values->ISCDone, 25, 25) |
      __gen_uint(values->SVGDone, 26, 26) |
      __gen_uint(values->RCCDone, 27, 27) |
      __gen_uint(values->SVRWDone, 28, 28) |
      __gen_uint(values->WMBEDone, 29, 29) |
      __gen_uint(values->CSDone, 30, 30) |
      __gen_uint(values->GAMDone, 31, 31);
}

#define GFX6_INSTPM_num                   0x20c0
#define GFX6_INSTPM_length                     1
struct GFX6_INSTPM {
   bool                                 _3DStateInstructionDisable;
   bool                                 _3DRenderingInstructionDisable;
   bool                                 MediaInstructionDisable;
   bool                                 CONSTANT_BUFFERAddressOffsetDisable;
   bool                                 _3DStateInstructionDisableMask;
   bool                                 _3DRenderingInstructionDisableMask;
   bool                                 MediaInstructionDisableMask;
   bool                                 CONSTANT_BUFFERAddressOffsetDisableMask;
};

static inline __attribute__((always_inline)) void
GFX6_INSTPM_pack(__attribute__((unused)) __gen_user_data *data,
                 __attribute__((unused)) void * restrict dst,
                 __attribute__((unused)) const struct GFX6_INSTPM * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->_3DStateInstructionDisable, 1, 1) |
      __gen_uint(values->_3DRenderingInstructionDisable, 2, 2) |
      __gen_uint(values->MediaInstructionDisable, 3, 3) |
      __gen_uint(values->CONSTANT_BUFFERAddressOffsetDisable, 6, 6) |
      __gen_uint(values->_3DStateInstructionDisableMask, 17, 17) |
      __gen_uint(values->_3DRenderingInstructionDisableMask, 18, 18) |
      __gen_uint(values->MediaInstructionDisableMask, 19, 19) |
      __gen_uint(values->CONSTANT_BUFFERAddressOffsetDisableMask, 22, 22);
}

#define GFX6_RCS_FAULT_REG_num            0x4094
#define GFX6_RCS_FAULT_REG_length              1
struct GFX6_RCS_FAULT_REG {
   bool                                 ValidBit;
   uint32_t                             FaultType;
#define PageFault                                0
#define InvalidPDFault                           1
#define UnloadedPDFault                          2
#define InvalidandUnloadedPDfault                3
   uint32_t                             SRCIDofFault;
   uint32_t                             GTTSEL;
#define PPGTT                                    0
#define GGTT                                     1
   __gen_address_type                   VirtualAddressofFault;
};

static inline __attribute__((always_inline)) void
GFX6_RCS_FAULT_REG_pack(__attribute__((unused)) __gen_user_data *data,
                        __attribute__((unused)) void * restrict dst,
                        __attribute__((unused)) const struct GFX6_RCS_FAULT_REG * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   const uint32_t v0 =
      __gen_uint(values->ValidBit, 0, 0) |
      __gen_uint(values->FaultType, 1, 2) |
      __gen_uint(values->SRCIDofFault, 3, 10) |
      __gen_uint(values->GTTSEL, 11, 11);
   dw[0] = __gen_address(data, &dw[0], values->VirtualAddressofFault, v0, 12, 31);
}

#define GFX6_RCS_RING_BUFFER_CTL_num      0x203c
#define GFX6_RCS_RING_BUFFER_CTL_length        1
struct GFX6_RCS_RING_BUFFER_CTL {
   bool                                 RingBufferEnable;
   uint32_t                             AutomaticReportHeadPointer;
#define MI_AUTOREPORT_OFF                        0
#define MI_AUTOREPORT_64KBMI_AUTOREPORT_4KB      1
#define MI_AUTOREPORT_128KB                      3
   bool                                 SemaphoreWait;
   bool                                 RBWait;
   uint32_t                             BufferLengthinpages1;
};

static inline __attribute__((always_inline)) void
GFX6_RCS_RING_BUFFER_CTL_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_RCS_RING_BUFFER_CTL * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->RingBufferEnable, 0, 0) |
      __gen_uint(values->AutomaticReportHeadPointer, 1, 2) |
      __gen_uint(values->SemaphoreWait, 10, 10) |
      __gen_uint(values->RBWait, 11, 11) |
      __gen_uint(values->BufferLengthinpages1, 12, 20);
}

#define GFX6_RPSTAT1_num                  0xa01c
#define GFX6_RPSTAT1_length                    1
struct GFX6_RPSTAT1 {
   uint32_t                             PreviousGTFrequency;
   uint32_t                             CurrentGTFrequency;
};

static inline __attribute__((always_inline)) void
GFX6_RPSTAT1_pack(__attribute__((unused)) __gen_user_data *data,
                  __attribute__((unused)) void * restrict dst,
                  __attribute__((unused)) const struct GFX6_RPSTAT1 * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->PreviousGTFrequency, 0, 6) |
      __gen_uint(values->CurrentGTFrequency, 7, 14);
}

#define GFX6_VCS_FAULT_REG_num            0x4194
#define GFX6_VCS_FAULT_REG_length              1
struct GFX6_VCS_FAULT_REG {
   bool                                 ValidBit;
   uint32_t                             FaultType;
#define PageFault                                0
#define InvalidPDFault                           1
#define UnloadedPDFault                          2
#define InvalidandUnloadedPDfault                3
   uint32_t                             SRCIDofFault;
   uint32_t                             GTTSEL;
#define PPGTT                                    0
#define GGTT                                     1
   __gen_address_type                   VirtualAddressofFault;
};

static inline __attribute__((always_inline)) void
GFX6_VCS_FAULT_REG_pack(__attribute__((unused)) __gen_user_data *data,
                        __attribute__((unused)) void * restrict dst,
                        __attribute__((unused)) const struct GFX6_VCS_FAULT_REG * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   const uint32_t v0 =
      __gen_uint(values->ValidBit, 0, 0) |
      __gen_uint(values->FaultType, 1, 2) |
      __gen_uint(values->SRCIDofFault, 3, 10) |
      __gen_uint(values->GTTSEL, 11, 11);
   dw[0] = __gen_address(data, &dw[0], values->VirtualAddressofFault, v0, 12, 31);
}

#define GFX6_VCS_INSTDONE_num             0x1206c
#define GFX6_VCS_INSTDONE_length               1
struct GFX6_VCS_INSTDONE {
   bool                                 RingEnable;
   bool                                 USBDone;
   bool                                 QRCDone;
   bool                                 SECDone;
   bool                                 MPCDone;
   bool                                 VFTDone;
   bool                                 BSPDone;
   bool                                 VLFDone;
   bool                                 VOPDone;
   bool                                 VMCDone;
   bool                                 VIPDone;
   bool                                 VITDone;
   bool                                 VDSDone;
   bool                                 VMXDone;
   bool                                 VCPDone;
   bool                                 VCDDone;
   bool                                 VADDone;
   bool                                 VMDDone;
   bool                                 VISDone;
   bool                                 VACDone;
   bool                                 VAMDone;
   bool                                 JPGDone;
   bool                                 VBPDone;
   bool                                 VHRDone;
   bool                                 VCIDone;
   bool                                 VCRDone;
   bool                                 VINDone;
   bool                                 VPRDone;
   bool                                 VTQDone;
   bool                                 VCSDone;
   bool                                 GACDone;
};

static inline __attribute__((always_inline)) void
GFX6_VCS_INSTDONE_pack(__attribute__((unused)) __gen_user_data *data,
                       __attribute__((unused)) void * restrict dst,
                       __attribute__((unused)) const struct GFX6_VCS_INSTDONE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->RingEnable, 0, 0) |
      __gen_uint(values->USBDone, 1, 1) |
      __gen_uint(values->QRCDone, 2, 2) |
      __gen_uint(values->SECDone, 3, 3) |
      __gen_uint(values->MPCDone, 4, 4) |
      __gen_uint(values->VFTDone, 5, 5) |
      __gen_uint(values->BSPDone, 6, 6) |
      __gen_uint(values->VLFDone, 7, 7) |
      __gen_uint(values->VOPDone, 8, 8) |
      __gen_uint(values->VMCDone, 9, 9) |
      __gen_uint(values->VIPDone, 10, 10) |
      __gen_uint(values->VITDone, 11, 11) |
      __gen_uint(values->VDSDone, 12, 12) |
      __gen_uint(values->VMXDone, 13, 13) |
      __gen_uint(values->VCPDone, 14, 14) |
      __gen_uint(values->VCDDone, 15, 15) |
      __gen_uint(values->VADDone, 16, 16) |
      __gen_uint(values->VMDDone, 17, 17) |
      __gen_uint(values->VISDone, 18, 18) |
      __gen_uint(values->VACDone, 19, 19) |
      __gen_uint(values->VAMDone, 20, 20) |
      __gen_uint(values->JPGDone, 21, 21) |
      __gen_uint(values->VBPDone, 22, 22) |
      __gen_uint(values->VHRDone, 23, 23) |
      __gen_uint(values->VCIDone, 24, 24) |
      __gen_uint(values->VCRDone, 25, 25) |
      __gen_uint(values->VINDone, 26, 26) |
      __gen_uint(values->VPRDone, 27, 27) |
      __gen_uint(values->VTQDone, 28, 28) |
      __gen_uint(values->VCSDone, 30, 30) |
      __gen_uint(values->GACDone, 31, 31);
}

#define GFX6_VCS_RING_BUFFER_CTL_num      0x1203c
#define GFX6_VCS_RING_BUFFER_CTL_length        1
struct GFX6_VCS_RING_BUFFER_CTL {
   bool                                 RingBufferEnable;
   uint32_t                             AutomaticReportHeadPointer;
#define MI_AUTOREPORT_OFF                        0
#define MI_AUTOREPORT_64KB                       1
#define MI_AUTOREPORT_4KB                        2
#define MI_AUTOREPORT_128KB                      3
   bool                                 DisableRegisterAccesses;
   bool                                 SemaphoreWait;
   bool                                 RBWait;
   uint32_t                             BufferLengthinpages1;
};

static inline __attribute__((always_inline)) void
GFX6_VCS_RING_BUFFER_CTL_pack(__attribute__((unused)) __gen_user_data *data,
                              __attribute__((unused)) void * restrict dst,
                              __attribute__((unused)) const struct GFX6_VCS_RING_BUFFER_CTL * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      __gen_uint(values->RingBufferEnable, 0, 0) |
      __gen_uint(values->AutomaticReportHeadPointer, 1, 2) |
      __gen_uint(values->DisableRegisterAccesses, 8, 8) |
      __gen_uint(values->SemaphoreWait, 10, 10) |
      __gen_uint(values->RBWait, 11, 11) |
      __gen_uint(values->BufferLengthinpages1, 12, 20);
}

#endif /* GFX6_PACK_H */
