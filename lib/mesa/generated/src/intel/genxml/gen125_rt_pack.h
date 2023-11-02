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


/* Instructions, enums and structures for RT.
 *
 * This file has been generated, do not hand edit.
 */

#ifndef GFX125_RT_PACK_H
#define GFX125_RT_PACK_H

#include <stdio.h>

#include "util/bitpack_helpers.h"

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
__gen_offset_nonzero(uint64_t v, uint32_t start, uint32_t end)
{
   assert(v != 0ull);
   return __gen_offset(v, start, end);
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

#ifndef __gen_address_type
#error #define __gen_address_type before including this file
#endif

#ifndef __gen_user_data
#error #define __gen_combine_address before including this file
#endif

#undef NDEBUG_UNUSED

#endif


#define GFX125_BINDLESS_SHADER_RECORD_length      2
struct GFX125_BINDLESS_SHADER_RECORD {
   uint32_t                             OffsetToLocalArguments;
   uint32_t                             BindlessShaderDispatchMode;
#define RT_SIMD16                                0
#define RT_SIMD8                                 1
   uint64_t                             KernelStartPointer;
};

static inline __attribute__((always_inline)) void
GFX125_BINDLESS_SHADER_RECORD_pack(__attribute__((unused)) __gen_user_data *data,
                                   __attribute__((unused)) void * restrict dst,
                                   __attribute__((unused)) const struct GFX125_BINDLESS_SHADER_RECORD * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      util_bitpack_uint(values->OffsetToLocalArguments, 0, 2) |
      util_bitpack_uint(values->BindlessShaderDispatchMode, 4, 4) |
      __gen_offset(values->KernelStartPointer, 6, 31);

   dw[1] = 0;
}

#define GFX125_RT_BVH_VEC3_length              3
struct GFX125_RT_BVH_VEC3 {
   float                                X;
   float                                Y;
   float                                Z;
};

static inline __attribute__((always_inline)) void
GFX125_RT_BVH_VEC3_pack(__attribute__((unused)) __gen_user_data *data,
                        __attribute__((unused)) void * restrict dst,
                        __attribute__((unused)) const struct GFX125_RT_BVH_VEC3 * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      util_bitpack_float(values->X);

   dw[1] =
      util_bitpack_float(values->Y);

   dw[2] =
      util_bitpack_float(values->Z);
}

#define GFX125_RT_BVH_METADATA_length         16
struct GFX125_RT_BVH_METADATA {
   struct GFX125_RT_BVH_VEC3            BoundsMin;
   struct GFX125_RT_BVH_VEC3            BoundsMax;
};

static inline __attribute__((always_inline)) void
GFX125_RT_BVH_METADATA_pack(__attribute__((unused)) __gen_user_data *data,
                            __attribute__((unused)) void * restrict dst,
                            __attribute__((unused)) const struct GFX125_RT_BVH_METADATA * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   GFX125_RT_BVH_VEC3_pack(data, &dw[0], &values->BoundsMin);

   GFX125_RT_BVH_VEC3_pack(data, &dw[3], &values->BoundsMax);

   dw[6] = 0;

   dw[7] = 0;

   dw[8] = 0;

   dw[9] = 0;

   dw[10] = 0;

   dw[11] = 0;

   dw[12] = 0;

   dw[13] = 0;

   dw[14] = 0;

   dw[15] = 0;
}

#define GFX125_RT_BVH_length                  32
struct GFX125_RT_BVH {
   uint64_t                             RootNodeOffset;
   struct GFX125_RT_BVH_METADATA        Meta;
};

static inline __attribute__((always_inline)) void
GFX125_RT_BVH_pack(__attribute__((unused)) __gen_user_data *data,
                   __attribute__((unused)) void * restrict dst,
                   __attribute__((unused)) const struct GFX125_RT_BVH * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   const uint64_t v0 =
      __gen_offset(values->RootNodeOffset, 0, 63);
   dw[0] = v0;
   dw[1] = v0 >> 32;

   dw[2] = 0;

   dw[3] = 0;

   dw[4] = 0;

   dw[5] = 0;

   dw[6] = 0;

   dw[7] = 0;

   dw[8] = 0;

   dw[9] = 0;

   dw[10] = 0;

   dw[11] = 0;

   dw[12] = 0;

   dw[13] = 0;

   dw[14] = 0;

   dw[15] = 0;

   GFX125_RT_BVH_METADATA_pack(data, &dw[16], &values->Meta);
}

#define GFX125_RT_BVH_INSTANCE_LEAF_length     32
struct GFX125_RT_BVH_INSTANCE_LEAF {
   uint32_t                             ShaderIndex;
   uint32_t                             GeometryRayMask;
   uint32_t                             InstanceContributionToHitGroupIndex;
   uint32_t                             LeafType;
#define TYPE_OPAQUE_CULLING_ENABLED              0
#define TYPE_OPAQUE_CULLING_DISABLED             1
   uint32_t                             GeometryFlags;
#define GEOMETRY_OPAQUE                          1
   __gen_address_type                   StartNodeAddress;
   uint32_t                             InstanceFlags;
#define TRIANGLE_CULL_DISABLE                    1
#define TRIANGLE_FRONT_COUNTERCLOCKWISE          2
#define FORCE_OPAQUE                             4
#define FORCE_NON_OPAQUE                         8
   float                                WorldToObjectm00;
   float                                WorldToObjectm01;
   float                                WorldToObjectm02;
   float                                WorldToObjectm10;
   float                                WorldToObjectm11;
   float                                WorldToObjectm12;
   float                                WorldToObjectm20;
   float                                WorldToObjectm21;
   float                                WorldToObjectm22;
   float                                ObjectToWorldm30;
   float                                ObjectToWorldm31;
   float                                ObjectToWorldm32;
   __gen_address_type                   BVHAddress;
   uint32_t                             InstanceID;
   uint32_t                             InstanceIndex;
   float                                ObjectToWorldm00;
   float                                ObjectToWorldm01;
   float                                ObjectToWorldm02;
   float                                ObjectToWorldm10;
   float                                ObjectToWorldm11;
   float                                ObjectToWorldm12;
   float                                ObjectToWorldm20;
   float                                ObjectToWorldm21;
   float                                ObjectToWorldm22;
   float                                WorldToObjectm30;
   float                                WorldToObjectm31;
   float                                WorldToObjectm32;
};

static inline __attribute__((always_inline)) void
GFX125_RT_BVH_INSTANCE_LEAF_pack(__attribute__((unused)) __gen_user_data *data,
                                 __attribute__((unused)) void * restrict dst,
                                 __attribute__((unused)) const struct GFX125_RT_BVH_INSTANCE_LEAF * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      util_bitpack_uint(values->ShaderIndex, 0, 23) |
      util_bitpack_uint(values->GeometryRayMask, 24, 31);

   dw[1] =
      util_bitpack_uint(values->InstanceContributionToHitGroupIndex, 0, 23) |
      util_bitpack_uint(values->LeafType, 29, 29) |
      util_bitpack_uint(values->GeometryFlags, 30, 31);

   const uint64_t v2 =
      util_bitpack_uint(values->InstanceFlags, 48, 55);
   const uint64_t v2_address =
      __gen_address(data, &dw[2], values->StartNodeAddress, v2, 0, 47);
   dw[2] = v2_address;
   dw[3] = (v2_address >> 32) | (v2 >> 32);

   dw[4] =
      util_bitpack_float(values->WorldToObjectm00);

   dw[5] =
      util_bitpack_float(values->WorldToObjectm01);

   dw[6] =
      util_bitpack_float(values->WorldToObjectm02);

   dw[7] =
      util_bitpack_float(values->WorldToObjectm10);

   dw[8] =
      util_bitpack_float(values->WorldToObjectm11);

   dw[9] =
      util_bitpack_float(values->WorldToObjectm12);

   dw[10] =
      util_bitpack_float(values->WorldToObjectm20);

   dw[11] =
      util_bitpack_float(values->WorldToObjectm21);

   dw[12] =
      util_bitpack_float(values->WorldToObjectm22);

   dw[13] =
      util_bitpack_float(values->ObjectToWorldm30);

   dw[14] =
      util_bitpack_float(values->ObjectToWorldm31);

   dw[15] =
      util_bitpack_float(values->ObjectToWorldm32);

   const uint64_t v16_address =
      __gen_address(data, &dw[16], values->BVHAddress, 0, 0, 47);
   dw[16] = v16_address;
   dw[17] = v16_address >> 32;

   dw[18] =
      util_bitpack_uint(values->InstanceID, 0, 31);

   dw[19] =
      util_bitpack_uint(values->InstanceIndex, 0, 31);

   dw[20] =
      util_bitpack_float(values->ObjectToWorldm00);

   dw[21] =
      util_bitpack_float(values->ObjectToWorldm01);

   dw[22] =
      util_bitpack_float(values->ObjectToWorldm02);

   dw[23] =
      util_bitpack_float(values->ObjectToWorldm10);

   dw[24] =
      util_bitpack_float(values->ObjectToWorldm11);

   dw[25] =
      util_bitpack_float(values->ObjectToWorldm12);

   dw[26] =
      util_bitpack_float(values->ObjectToWorldm20);

   dw[27] =
      util_bitpack_float(values->ObjectToWorldm21);

   dw[28] =
      util_bitpack_float(values->ObjectToWorldm22);

   dw[29] =
      util_bitpack_float(values->WorldToObjectm30);

   dw[30] =
      util_bitpack_float(values->WorldToObjectm31);

   dw[31] =
      util_bitpack_float(values->WorldToObjectm32);
}

#define GFX125_RT_BVH_INTERNAL_NODE_length     16
struct GFX125_RT_BVH_INTERNAL_NODE {
   struct GFX125_RT_BVH_VEC3            Origin;
   int32_t                              ChildOffset;
   uint32_t                             NodeType;
#define NODE_TYPE_INTERNAL                       0
#define NODE_TYPE_INSTANCE                       1
#define NODE_TYPE_PROCEDURAL                     3
#define NODE_TYPE_QUAD                           4
#define NODE_TYPE_INVALID                        7
   int32_t                              ChildBoundsExponentX;
   int32_t                              ChildBoundsExponentY;
   int32_t                              ChildBoundsExponentZ;
   uint32_t                             NodeRayMask;
   uint32_t                             ChildSize[6];
   uint32_t                             ChildType[6];
   uint32_t                             StartPrimitive[6];
   uint32_t                             ChildLowerXBound[6];
   uint32_t                             ChildUpperXBound[6];
   uint32_t                             ChildLowerYBound[6];
   uint32_t                             ChildUpperYBound[6];
   uint32_t                             ChildLowerZBound[6];
   uint32_t                             ChildUpperZBound[6];
};

static inline __attribute__((always_inline)) void
GFX125_RT_BVH_INTERNAL_NODE_pack(__attribute__((unused)) __gen_user_data *data,
                                 __attribute__((unused)) void * restrict dst,
                                 __attribute__((unused)) const struct GFX125_RT_BVH_INTERNAL_NODE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   GFX125_RT_BVH_VEC3_pack(data, &dw[0], &values->Origin);

   dw[3] =
      util_bitpack_sint(values->ChildOffset, 0, 31);

   dw[4] =
      util_bitpack_uint(values->NodeType, 0, 7) |
      util_bitpack_sint(values->ChildBoundsExponentX, 16, 23) |
      util_bitpack_sint(values->ChildBoundsExponentY, 24, 31);

   dw[5] =
      util_bitpack_sint(values->ChildBoundsExponentZ, 0, 8) |
      util_bitpack_uint(values->NodeRayMask, 8, 15) |
      util_bitpack_uint(values->ChildSize[0], 16, 17) |
      util_bitpack_uint(values->ChildType[0], 18, 21) |
      util_bitpack_uint(values->StartPrimitive[0], 18, 21) |
      util_bitpack_uint(values->ChildSize[1], 24, 25) |
      util_bitpack_uint(values->ChildType[1], 26, 29) |
      util_bitpack_uint(values->StartPrimitive[1], 26, 29);

   dw[6] =
      util_bitpack_uint(values->ChildSize[2], 0, 1) |
      util_bitpack_uint(values->ChildType[2], 2, 5) |
      util_bitpack_uint(values->StartPrimitive[2], 2, 5) |
      util_bitpack_uint(values->ChildSize[3], 8, 9) |
      util_bitpack_uint(values->ChildType[3], 10, 13) |
      util_bitpack_uint(values->StartPrimitive[3], 10, 13) |
      util_bitpack_uint(values->ChildSize[4], 16, 17) |
      util_bitpack_uint(values->ChildType[4], 18, 21) |
      util_bitpack_uint(values->StartPrimitive[4], 18, 21) |
      util_bitpack_uint(values->ChildSize[5], 24, 25) |
      util_bitpack_uint(values->ChildType[5], 26, 29) |
      util_bitpack_uint(values->StartPrimitive[5], 26, 29);

   dw[7] =
      util_bitpack_uint(values->ChildLowerXBound[0], 0, 7) |
      util_bitpack_uint(values->ChildLowerXBound[1], 8, 15) |
      util_bitpack_uint(values->ChildLowerXBound[2], 16, 23) |
      util_bitpack_uint(values->ChildLowerXBound[3], 24, 31);

   dw[8] =
      util_bitpack_uint(values->ChildLowerXBound[4], 0, 7) |
      util_bitpack_uint(values->ChildLowerXBound[5], 8, 15) |
      util_bitpack_uint(values->ChildUpperXBound[0], 16, 23) |
      util_bitpack_uint(values->ChildUpperXBound[1], 24, 31);

   dw[9] =
      util_bitpack_uint(values->ChildUpperXBound[2], 0, 7) |
      util_bitpack_uint(values->ChildUpperXBound[3], 8, 15) |
      util_bitpack_uint(values->ChildUpperXBound[4], 16, 23) |
      util_bitpack_uint(values->ChildUpperXBound[5], 24, 31);

   dw[10] =
      util_bitpack_uint(values->ChildLowerYBound[0], 0, 7) |
      util_bitpack_uint(values->ChildLowerYBound[1], 8, 15) |
      util_bitpack_uint(values->ChildLowerYBound[2], 16, 23) |
      util_bitpack_uint(values->ChildLowerYBound[3], 24, 31);

   dw[11] =
      util_bitpack_uint(values->ChildLowerYBound[4], 0, 7) |
      util_bitpack_uint(values->ChildLowerYBound[5], 8, 15) |
      util_bitpack_uint(values->ChildUpperYBound[0], 16, 23) |
      util_bitpack_uint(values->ChildUpperYBound[1], 24, 31);

   dw[12] =
      util_bitpack_uint(values->ChildUpperYBound[2], 0, 7) |
      util_bitpack_uint(values->ChildUpperYBound[3], 8, 15) |
      util_bitpack_uint(values->ChildUpperYBound[4], 16, 23) |
      util_bitpack_uint(values->ChildUpperYBound[5], 24, 31);

   dw[13] =
      util_bitpack_uint(values->ChildLowerZBound[0], 0, 7) |
      util_bitpack_uint(values->ChildLowerZBound[1], 8, 15) |
      util_bitpack_uint(values->ChildLowerZBound[2], 16, 23) |
      util_bitpack_uint(values->ChildLowerZBound[3], 24, 31);

   dw[14] =
      util_bitpack_uint(values->ChildLowerZBound[4], 0, 7) |
      util_bitpack_uint(values->ChildLowerZBound[5], 8, 15) |
      util_bitpack_uint(values->ChildUpperZBound[0], 16, 23) |
      util_bitpack_uint(values->ChildUpperZBound[1], 24, 31);

   dw[15] =
      util_bitpack_uint(values->ChildUpperZBound[2], 0, 7) |
      util_bitpack_uint(values->ChildUpperZBound[3], 8, 15) |
      util_bitpack_uint(values->ChildUpperZBound[4], 16, 23) |
      util_bitpack_uint(values->ChildUpperZBound[5], 24, 31);
}

#define GFX125_RT_BVH_PRIMITIVE_LEAF_DESCRIPTOR_length      2
struct GFX125_RT_BVH_PRIMITIVE_LEAF_DESCRIPTOR {
   uint32_t                             ShaderIndex;
   uint32_t                             GeometryRayMask;
   uint32_t                             GeometryIndex;
   uint32_t                             LeafType;
#define TYPE_QUAD                                0
#define TYPE_OPAQUE_CULLING_ENABLED              0
#define TYPE_OPAQUE_CULLING_DISABLED             1
   uint32_t                             GeometryFlags;
#define GEOMETRY_OPAQUE                          1
};

static inline __attribute__((always_inline)) void
GFX125_RT_BVH_PRIMITIVE_LEAF_DESCRIPTOR_pack(__attribute__((unused)) __gen_user_data *data,
                                             __attribute__((unused)) void * restrict dst,
                                             __attribute__((unused)) const struct GFX125_RT_BVH_PRIMITIVE_LEAF_DESCRIPTOR * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   dw[0] =
      util_bitpack_uint(values->ShaderIndex, 0, 23) |
      util_bitpack_uint(values->GeometryRayMask, 24, 31);

   dw[1] =
      util_bitpack_uint(values->GeometryIndex, 0, 28) |
      util_bitpack_uint(values->LeafType, 29, 29) |
      util_bitpack_uint(values->GeometryFlags, 30, 31);
}

#define GFX125_RT_BVH_PROCEDURAL_LEAF_length     16
struct GFX125_RT_BVH_PROCEDURAL_LEAF {
   struct GFX125_RT_BVH_PRIMITIVE_LEAF_DESCRIPTOR LeafDescriptor;
   uint32_t                             NumPrimitives;
   uint32_t                             LastPrimitive;
   uint32_t                             PrimitiveIndex[13];
};

static inline __attribute__((always_inline)) void
GFX125_RT_BVH_PROCEDURAL_LEAF_pack(__attribute__((unused)) __gen_user_data *data,
                                   __attribute__((unused)) void * restrict dst,
                                   __attribute__((unused)) const struct GFX125_RT_BVH_PROCEDURAL_LEAF * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   GFX125_RT_BVH_PRIMITIVE_LEAF_DESCRIPTOR_pack(data, &dw[0], &values->LeafDescriptor);

   dw[2] =
      util_bitpack_uint(values->NumPrimitives, 0, 3) |
      util_bitpack_uint(values->LastPrimitive, 19, 31);

   dw[3] =
      util_bitpack_uint(values->PrimitiveIndex[0], 0, 31);

   dw[4] =
      util_bitpack_uint(values->PrimitiveIndex[1], 0, 31);

   dw[5] =
      util_bitpack_uint(values->PrimitiveIndex[2], 0, 31);

   dw[6] =
      util_bitpack_uint(values->PrimitiveIndex[3], 0, 31);

   dw[7] =
      util_bitpack_uint(values->PrimitiveIndex[4], 0, 31);

   dw[8] =
      util_bitpack_uint(values->PrimitiveIndex[5], 0, 31);

   dw[9] =
      util_bitpack_uint(values->PrimitiveIndex[6], 0, 31);

   dw[10] =
      util_bitpack_uint(values->PrimitiveIndex[7], 0, 31);

   dw[11] =
      util_bitpack_uint(values->PrimitiveIndex[8], 0, 31);

   dw[12] =
      util_bitpack_uint(values->PrimitiveIndex[9], 0, 31);

   dw[13] =
      util_bitpack_uint(values->PrimitiveIndex[10], 0, 31);

   dw[14] =
      util_bitpack_uint(values->PrimitiveIndex[11], 0, 31);

   dw[15] =
      util_bitpack_uint(values->PrimitiveIndex[12], 0, 31);
}

#define GFX125_RT_BVH_QUAD_LEAF_length        16
struct GFX125_RT_BVH_QUAD_LEAF {
   struct GFX125_RT_BVH_PRIMITIVE_LEAF_DESCRIPTOR LeafDescriptor;
   uint32_t                             PrimitiveIndex0;
   uint32_t                             PrimitiveIndex1Delta;
   uint32_t                             j0;
   uint32_t                             j1;
   uint32_t                             j2;
   bool                                 LastQuad;
   struct GFX125_RT_BVH_VEC3            QuadVertex[4];
};

static inline __attribute__((always_inline)) void
GFX125_RT_BVH_QUAD_LEAF_pack(__attribute__((unused)) __gen_user_data *data,
                             __attribute__((unused)) void * restrict dst,
                             __attribute__((unused)) const struct GFX125_RT_BVH_QUAD_LEAF * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   GFX125_RT_BVH_PRIMITIVE_LEAF_DESCRIPTOR_pack(data, &dw[0], &values->LeafDescriptor);

   dw[2] =
      util_bitpack_uint(values->PrimitiveIndex0, 0, 31);

   dw[3] =
      util_bitpack_uint(values->PrimitiveIndex1Delta, 0, 16) |
      util_bitpack_uint(values->j0, 16, 17) |
      util_bitpack_uint(values->j1, 18, 19) |
      util_bitpack_uint(values->j2, 20, 21) |
      util_bitpack_uint(values->LastQuad, 22, 22);

   GFX125_RT_BVH_VEC3_pack(data, &dw[4], &values->QuadVertex[0]);

   GFX125_RT_BVH_VEC3_pack(data, &dw[7], &values->QuadVertex[1]);

   GFX125_RT_BVH_VEC3_pack(data, &dw[10], &values->QuadVertex[2]);

   GFX125_RT_BVH_VEC3_pack(data, &dw[13], &values->QuadVertex[3]);
}

#define GFX125_RT_SHADER_TABLE_length          2
struct GFX125_RT_SHADER_TABLE {
   __gen_address_type                   BaseAddress;
   uint32_t                             Stride;
};

static inline __attribute__((always_inline)) void
GFX125_RT_SHADER_TABLE_pack(__attribute__((unused)) __gen_user_data *data,
                            __attribute__((unused)) void * restrict dst,
                            __attribute__((unused)) const struct GFX125_RT_SHADER_TABLE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   const uint64_t v0 =
      util_bitpack_uint(values->Stride, 48, 63);
   const uint64_t v0_address =
      __gen_address(data, &dw[0], values->BaseAddress, v0, 0, 47);
   dw[0] = v0_address;
   dw[1] = (v0_address >> 32) | (v0 >> 32);
}

#define GFX125_RT_DISPATCH_GLOBALS_length     20
struct GFX125_RT_DISPATCH_GLOBALS {
   __gen_address_type                   MemBaseAddress;
   struct GFX125_BINDLESS_SHADER_RECORD CallStackHandler;
   uint32_t                             AsyncRTStackSize;
   uint32_t                             NumDSSRTStacks;
   uint32_t                             MaxBVHLevels;
   uint32_t                             Flags;
#define RT_DEPTH_TEST_LESS_EQUAL                 1
   struct GFX125_RT_SHADER_TABLE        HitGroupTable;
   struct GFX125_RT_SHADER_TABLE        MissGroupTable;
   uint32_t                             SWStackSize;
   uint32_t                             LaunchWidth;
   uint32_t                             LaunchHeight;
   uint32_t                             LaunchDepth;
   struct GFX125_RT_SHADER_TABLE        CallableGroupTable;
   __gen_address_type                   ResumeShaderTable;
};

static inline __attribute__((always_inline)) void
GFX125_RT_DISPATCH_GLOBALS_pack(__attribute__((unused)) __gen_user_data *data,
                                __attribute__((unused)) void * restrict dst,
                                __attribute__((unused)) const struct GFX125_RT_DISPATCH_GLOBALS * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   const uint64_t v0_address =
      __gen_address(data, &dw[0], values->MemBaseAddress, 0, 0, 63);
   dw[0] = v0_address;
   dw[1] = v0_address >> 32;

   GFX125_BINDLESS_SHADER_RECORD_pack(data, &dw[2], &values->CallStackHandler);

   dw[4] =
      util_bitpack_uint(values->AsyncRTStackSize, 0, 31);

   dw[5] =
      util_bitpack_uint(values->NumDSSRTStacks, 0, 15);

   dw[6] =
      util_bitpack_uint(values->MaxBVHLevels, 0, 2);

   dw[7] =
      util_bitpack_uint(values->Flags, 0, 0);

   GFX125_RT_SHADER_TABLE_pack(data, &dw[8], &values->HitGroupTable);

   GFX125_RT_SHADER_TABLE_pack(data, &dw[10], &values->MissGroupTable);

   dw[12] =
      util_bitpack_uint(values->SWStackSize, 0, 31);

   dw[13] =
      util_bitpack_uint(values->LaunchWidth, 0, 31);

   dw[14] =
      util_bitpack_uint(values->LaunchHeight, 0, 31);

   dw[15] =
      util_bitpack_uint(values->LaunchDepth, 0, 31);

   GFX125_RT_SHADER_TABLE_pack(data, &dw[16], &values->CallableGroupTable);

   const uint64_t v18_address =
      __gen_address(data, &dw[18], values->ResumeShaderTable, 0, 0, 63);
   dw[18] = v18_address;
   dw[19] = v18_address >> 32;
}

#define GFX125_RT_GENERAL_SBT_HANDLE_length      8
struct GFX125_RT_GENERAL_SBT_HANDLE {
   struct GFX125_BINDLESS_SHADER_RECORD General;
};

static inline __attribute__((always_inline)) void
GFX125_RT_GENERAL_SBT_HANDLE_pack(__attribute__((unused)) __gen_user_data *data,
                                  __attribute__((unused)) void * restrict dst,
                                  __attribute__((unused)) const struct GFX125_RT_GENERAL_SBT_HANDLE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   GFX125_BINDLESS_SHADER_RECORD_pack(data, &dw[0], &values->General);

   dw[2] = 0;

   dw[3] = 0;

   dw[4] = 0;

   dw[5] = 0;

   dw[6] = 0;

   dw[7] = 0;
}

#define GFX125_RT_PROCEDURAL_SBT_HANDLE_length      8
struct GFX125_RT_PROCEDURAL_SBT_HANDLE {
   struct GFX125_BINDLESS_SHADER_RECORD ClosestHit;
   struct GFX125_BINDLESS_SHADER_RECORD Intersection;
};

static inline __attribute__((always_inline)) void
GFX125_RT_PROCEDURAL_SBT_HANDLE_pack(__attribute__((unused)) __gen_user_data *data,
                                     __attribute__((unused)) void * restrict dst,
                                     __attribute__((unused)) const struct GFX125_RT_PROCEDURAL_SBT_HANDLE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   GFX125_BINDLESS_SHADER_RECORD_pack(data, &dw[0], &values->ClosestHit);

   GFX125_BINDLESS_SHADER_RECORD_pack(data, &dw[2], &values->Intersection);

   dw[4] = 0;

   dw[5] = 0;

   dw[6] = 0;

   dw[7] = 0;
}

#define GFX125_RT_TRIANGLES_SBT_HANDLE_length      8
struct GFX125_RT_TRIANGLES_SBT_HANDLE {
   struct GFX125_BINDLESS_SHADER_RECORD ClosestHit;
   struct GFX125_BINDLESS_SHADER_RECORD AnyHit;
};

static inline __attribute__((always_inline)) void
GFX125_RT_TRIANGLES_SBT_HANDLE_pack(__attribute__((unused)) __gen_user_data *data,
                                    __attribute__((unused)) void * restrict dst,
                                    __attribute__((unused)) const struct GFX125_RT_TRIANGLES_SBT_HANDLE * restrict values)
{
   uint32_t * restrict dw = (uint32_t * restrict) dst;

   GFX125_BINDLESS_SHADER_RECORD_pack(data, &dw[0], &values->ClosestHit);

   GFX125_BINDLESS_SHADER_RECORD_pack(data, &dw[2], &values->AnyHit);

   dw[4] = 0;

   dw[5] = 0;

   dw[6] = 0;

   dw[7] = 0;
}

#endif /* GFX125_RT_PACK_H */
