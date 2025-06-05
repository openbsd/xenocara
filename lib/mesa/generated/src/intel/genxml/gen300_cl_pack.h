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


/* Instructions, enums and structures for XE3.
 *
 * This file has been generated, do not hand edit.
 */

#ifndef GFX30_XE3_CL_PACK_H
#define GFX30_XE3_CL_PACK_H

#ifndef __OPENCL_VERSION__
#include <stdio.h>
#include "util/bitpack_helpers.h"
#include "genX_helpers.h"
#else
#include "genX_cl_helpers.h"
#endif


enum GFX30_3D_Color_Buffer_Blend_Factor {
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

enum GFX30_3D_Color_Buffer_Blend_Function {
   BLENDFUNCTION_ADD                    =      0,
   BLENDFUNCTION_SUBTRACT               =      1,
   BLENDFUNCTION_REVERSE_SUBTRACT       =      2,
   BLENDFUNCTION_MIN                    =      3,
   BLENDFUNCTION_MAX                    =      4,
};

enum GFX30_3D_Compare_Function {
   COMPAREFUNCTION_ALWAYS               =      0,
   COMPAREFUNCTION_NEVER                =      1,
   COMPAREFUNCTION_LESS                 =      2,
   COMPAREFUNCTION_EQUAL                =      3,
   COMPAREFUNCTION_LEQUAL               =      4,
   COMPAREFUNCTION_GREATER              =      5,
   COMPAREFUNCTION_NOTEQUAL             =      6,
   COMPAREFUNCTION_GEQUAL               =      7,
};

enum GFX30_3D_Logic_Op_Function {
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

enum GFX30_3D_Prim_Topo_Type {
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
   _3DPRIM_PATCHLIST_1                  =     32,
   _3DPRIM_PATCHLIST_2                  =     33,
   _3DPRIM_PATCHLIST_3                  =     34,
   _3DPRIM_PATCHLIST_4                  =     35,
   _3DPRIM_PATCHLIST_5                  =     36,
   _3DPRIM_PATCHLIST_6                  =     37,
   _3DPRIM_PATCHLIST_7                  =     38,
   _3DPRIM_PATCHLIST_8                  =     39,
   _3DPRIM_PATCHLIST_9                  =     40,
   _3DPRIM_PATCHLIST_10                 =     41,
   _3DPRIM_PATCHLIST_11                 =     42,
   _3DPRIM_PATCHLIST_12                 =     43,
   _3DPRIM_PATCHLIST_13                 =     44,
   _3DPRIM_PATCHLIST_14                 =     45,
   _3DPRIM_PATCHLIST_15                 =     46,
   _3DPRIM_PATCHLIST_16                 =     47,
   _3DPRIM_PATCHLIST_17                 =     48,
   _3DPRIM_PATCHLIST_18                 =     49,
   _3DPRIM_PATCHLIST_19                 =     50,
   _3DPRIM_PATCHLIST_20                 =     51,
   _3DPRIM_PATCHLIST_21                 =     52,
   _3DPRIM_PATCHLIST_22                 =     53,
   _3DPRIM_PATCHLIST_23                 =     54,
   _3DPRIM_PATCHLIST_24                 =     55,
   _3DPRIM_PATCHLIST_25                 =     56,
   _3DPRIM_PATCHLIST_26                 =     57,
   _3DPRIM_PATCHLIST_27                 =     58,
   _3DPRIM_PATCHLIST_28                 =     59,
   _3DPRIM_PATCHLIST_29                 =     60,
   _3DPRIM_PATCHLIST_30                 =     61,
   _3DPRIM_PATCHLIST_31                 =     62,
   _3DPRIM_PATCHLIST_32                 =     63,
};

enum GFX30_3D_Stencil_Operation {
   STENCILOP_KEEP                       =      0,
   STENCILOP_ZERO                       =      1,
   STENCILOP_REPLACE                    =      2,
   STENCILOP_INCRSAT                    =      3,
   STENCILOP_DECRSAT                    =      4,
   STENCILOP_INCR                       =      5,
   STENCILOP_DECR                       =      6,
   STENCILOP_INVERT                     =      7,
};

enum GFX30_3D_Vertex_Component_Control {
   VFCOMP_NOSTORE                       =      0,
   VFCOMP_STORE_SRC                     =      1,
   VFCOMP_STORE_0                       =      2,
   VFCOMP_STORE_1_FP                    =      3,
   VFCOMP_STORE_1_INT                   =      4,
   VFCOMP_STORE_PID                     =      7,
};

enum GFX30_Atomic_OPCODE {
   MI_ATOMIC_OP_AND                     =      1,
   MI_ATOMIC_OP_OR                      =      2,
   MI_ATOMIC_OP_XOR                     =      3,
   MI_ATOMIC_OP_MOVE                    =      4,
   MI_ATOMIC_OP_INC                     =      5,
   MI_ATOMIC_OP_DEC                     =      6,
   MI_ATOMIC_OP_ADD                     =      7,
   MI_ATOMIC_OP_SUB                     =      8,
   MI_ATOMIC_OP_RSUB                    =      9,
   MI_ATOMIC_OP_IMAX                    =     10,
   MI_ATOMIC_OP_IMIN                    =     11,
   MI_ATOMIC_OP_UMAX                    =     12,
   MI_ATOMIC_OP_UMIN                    =     13,
   MI_ATOMIC_OP_CMP_WR                  =     14,
   MI_ATOMIC_OP_PREDEC                  =     15,
   MI_ATOMIC_OP_AND8B                   =     33,
   MI_ATOMIC_OP_OR8B                    =     34,
   MI_ATOMIC_OP_XOR8B                   =     35,
   MI_ATOMIC_OP_MOVE8B                  =     36,
   MI_ATOMIC_OP_INC8B                   =     37,
   MI_ATOMIC_OP_DEC8B                   =     38,
   MI_ATOMIC_OP_ADD8B                   =     39,
   MI_ATOMIC_OP_SUB8B                   =     40,
   MI_ATOMIC_OP_RSUB8B                  =     41,
   MI_ATOMIC_OP_IMAX8B                  =     42,
   MI_ATOMIC_OP_IMIN8B                  =     43,
   MI_ATOMIC_OP_UMAX8B                  =     44,
   MI_ATOMIC_OP_UMIN8B                  =     45,
   MI_ATOMIC_OP_CMP_WR8B                =     46,
   MI_ATOMIC_OP_PREDEC8B                =     47,
   MI_ATOMIC_OP_CMP_WR16B               =     78,
};

enum GFX30_Attribute_Component_Format {
   ACF_DISABLED                         =      0,
   ACF_XY                               =      1,
   ACF_XYZ                              =      2,
   ACF_XYZW                             =      3,
};

enum GFX30_COMPONENT_ENABLES {
   CE_NONE                              =      0,
   CE_X                                 =      1,
   CE_Y                                 =      2,
   CE_XY                                =      3,
   CE_Z                                 =      4,
   CE_XZ                                =      5,
   CE_YZ                                =      6,
   CE_XYZ                               =      7,
   CE_W                                 =      8,
   CE_XW                                =      9,
   CE_YW                                =     10,
   CE_XYW                               =     11,
   CE_ZW                                =     12,
   CE_XZW                               =     13,
   CE_YZW                               =     14,
   CE_XYZW                              =     15,
};

enum GFX30_L1_CACHE_CONTROL {
   L1CC_WBP                             =      0,
   L1CC_UC                              =      1,
   L1CC_WB                              =      2,
   L1CC_WT                              =      3,
   L1CC_WS                              =      4,
};

enum GFX30_PREF_SLM_ALLOCATION_SIZE {
   SLM_ENCODES_0K                       =      0,
   SLM_ENCODES_16K                      =      1,
   SLM_ENCODES_32K                      =      2,
   SLM_ENCODES_64K                      =      3,
   SLM_ENCODES_96K                      =      4,
   SLM_ENCODES_128K                     =      5,
   SLM_ENCODES_160K                     =      6,
   SLM_ENCODES_192K                     =      7,
   SLM_ENCODES_224K                     =      8,
   SLM_ENCODES_256K                     =      9,
   SLM_ENCODES_384K                     =     10,
};

enum GFX30_RESOURCE_BARRIER_STAGE {
   RESOURCE_BARRIER_STAGE_NONE          =      0,
   RESOURCE_BARRIER_STAGE_TOP           =      1,
   RESOURCE_BARRIER_STAGE_COLOR         =      2,
   RESOURCE_BARRIER_STAGE_GPGPU         =      4,
   RESOURCE_BARRIER_STAGE_COLORANDCOMPUTE =      6,
   RESOURCE_BARRIER_STAGE_GEOM          =     16,
   RESOURCE_BARRIER_STAGE_GEOMETRYANDCOMPUTE =     20,
   RESOURCE_BARRIER_STAGE_RASTER        =     32,
   RESOURCE_BARRIER_STAGE_DEPTH         =     64,
   RESOURCE_BARRIER_STAGE_PIXEL         =    128,
};

enum GFX30_RESOURCE_BARRIER_TYPE {
   RESOURCE_BARRIER_TYPE_IMMEDIATE      =      0,
   RESOURCE_BARRIER_TYPE_SIGNAL         =      1,
   RESOURCE_BARRIER_TYPE_WAIT           =      2,
   RESOURCE_BARRIER_TYPE_UAV            =      3,
};

enum GFX30_STATE_CPS_COMBINER_OPCODE {
   CPS_COMB_OP_PASSTHROUGH              =      0,
   CPS_COMB_OP_OVERRIDE                 =      1,
   CPS_COMB_OP_HIGH_QUALITY             =      2,
   CPS_COMB_OP_LOW_QUALITY              =      3,
   CPS_COMB_OP_RELATIVE                 =      4,
};

enum GFX30_STATE_CPS_SIZE {
   CPSIZE_1                             =      0,
   CPSIZE_2                             =      1,
   CPSIZE_4                             =      2,
};

enum GFX30_STATE_SURFACE_TYPE {
   SURFTYPE_1D                          =      0,
   SURFTYPE_2D                          =      1,
   SURFTYPE_3D                          =      2,
   SURFTYPE_CUBE                        =      3,
   SURFTYPE_BUFFER                      =      4,
   SURFTYPE_RES5                        =      5,
   SURFTYPE_SCRATCH                     =      6,
   SURFTYPE_NULL                        =      7,
};

enum GFX30_ShaderChannelSelect {
   SCS_ZERO                             =      0,
   SCS_ONE                              =      1,
   SCS_RED                              =      4,
   SCS_GREEN                            =      5,
   SCS_BLUE                             =      6,
   SCS_ALPHA                            =      7,
};

enum GFX30_TextureCoordinateMode {
   TCM_WRAP                             =      0,
   TCM_MIRROR                           =      1,
   TCM_CLAMP                            =      2,
   TCM_CUBE                             =      3,
   TCM_CLAMP_BORDER                     =      4,
   TCM_MIRROR_ONCE                      =      5,
   TCM_HALF_BORDER                      =      6,
   TCM_MIRROR_101                       =      7,
};

enum GFX30_UNIFIED_COMPRESSION_FORMAT {
   CMF_R8                               =      0,
   CMF_R8_G8                            =      1,
   CMF_R8_G8_B8_A8                      =      2,
   CMF_R10_G10_B10_A2                   =      3,
   CMF_R11_G11_B10                      =      4,
   CMF_R16                              =      5,
   CMF_R16_G16                          =      6,
   CMF_R16_G16_B16_A16                  =      7,
   CMF_R32                              =      8,
   CMF_R32_G32                          =      9,
   CMF_R32_G32_B32_A32                  =     10,
   CMF_Y16_U16_Y16_V16                  =     11,
   CMF_ML8                              =     15,
};

enum GFX30_WRAP_SHORTEST_ENABLE {
   WSE_X                                =      1,
   WSE_Y                                =      2,
   WSE_XY                               =      3,
   WSE_Z                                =      4,
   WSE_XZ                               =      5,
   WSE_YZ                               =      6,
   WSE_XYZ                              =      7,
   WSE_W                                =      8,
   WSE_XW                               =      9,
   WSE_YW                               =     10,
   WSE_XYW                              =     11,
   WSE_ZW                               =     12,
   WSE_XZW                              =     13,
   WSE_YZW                              =     14,
   WSE_XYZW                             =     15,
};

#define GFX30_VERTEX_BUFFER_STATE_length       4
struct GFX30_VERTEX_BUFFER_STATE {
   uint32_t                             BufferPitch;
   bool                                 NullVertexBuffer;
   bool                                 AddressModifyEnable;
   uint32_t                             MOCS;
   bool                                 L3BypassDisable;
   uint32_t                             VertexBufferIndex;
   uint64_t                             BufferStartingAddress;
   uint32_t                             BufferSize;
};

static inline __attribute__((always_inline)) void
GFX30_VERTEX_BUFFER_STATE_pack(__attribute__((unused)) global void * restrict dst,
                               __attribute__((unused)) private const struct GFX30_VERTEX_BUFFER_STATE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->BufferPitch, 0, 11) |
      util_bitpack_uint(values->NullVertexBuffer, 13, 13) |
      util_bitpack_uint(values->AddressModifyEnable, 14, 14) |
      util_bitpack_uint_nonzero(values->MOCS, 16, 22) |
      util_bitpack_uint(values->L3BypassDisable, 25, 25) |
      util_bitpack_uint(values->VertexBufferIndex, 26, 31);

   const uint64_t v1_address =
      __gen_address(values->BufferStartingAddress, 0, 63) | 0;
   dw[1] = v1_address;
   dw[2] = v1_address >> 32;

   dw[3] =
      util_bitpack_uint(values->BufferSize, 0, 31);
}

static inline __attribute__((always_inline)) void
GFX30_VERTEX_BUFFER_STATE_repack(__attribute__((unused)) global void * restrict dst,
                                 __attribute__((unused)) global const uint32_t * origin,
                                 __attribute__((unused)) private const struct GFX30_VERTEX_BUFFER_STATE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->BufferPitch, 0, 11) |
      util_bitpack_uint(values->NullVertexBuffer, 13, 13) |
      util_bitpack_uint(values->AddressModifyEnable, 14, 14) |
      util_bitpack_uint_nonzero(values->MOCS, 16, 22) |
      util_bitpack_uint(values->L3BypassDisable, 25, 25) |
      util_bitpack_uint(values->VertexBufferIndex, 26, 31);

   const uint64_t v1 =
      origin[1] |
      ((uint64_t)origin[2] << 32);
   const uint64_t v1_address =
      __gen_address(values->BufferStartingAddress, 0, 63) | v1;
   dw[1] = v1_address;
   dw[2] = v1_address >> 32;

   dw[3] =
      origin[3] |
      util_bitpack_uint(values->BufferSize, 0, 31);
}

#define GFX30_3DPRIMITIVE_length               7
#define GFX30_3DPRIMITIVE_length_bias          2
#define GFX30_3DPRIMITIVE_header                \
   .DWordLength                         =      5,  \
   ._3DCommandSubOpcode                 =      0,  \
   ._3DCommandOpcode                    =      3,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX30_3DPRIMITIVE {
   uint32_t                             DWordLength;
   bool                                 PredicateEnable;
   bool                                 UAVCoherencyRequired;
   bool                                 IndirectParameterEnable;
   uint32_t                             ExtendedParametersPresent;
   uint32_t                             TBIMREnable;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   enum GFX30_3D_Prim_Topo_Type         PrimitiveTopologyType;
   uint32_t                             VertexAccessType;
#define SEQUENTIAL                               0
#define RANDOM                                   1
   bool                                 EndOffsetEnable;
   uint32_t                             VertexCountPerInstance;
   uint32_t                             StartVertexLocation;
   uint32_t                             InstanceCount;
   uint32_t                             StartInstanceLocation;
   int32_t                              BaseVertexLocation;
   uint32_t                             ExtendedParameter0;
   uint32_t                             ExtendedParameter1;
   uint32_t                             ExtendedParameter2;
};

static inline __attribute__((always_inline)) void
GFX30_3DPRIMITIVE_pack(__attribute__((unused)) global void * restrict dst,
                       __attribute__((unused)) private const struct GFX30_3DPRIMITIVE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->PredicateEnable, 8, 8) |
      util_bitpack_uint(values->UAVCoherencyRequired, 9, 9) |
      util_bitpack_uint(values->IndirectParameterEnable, 10, 10) |
      util_bitpack_uint(values->ExtendedParametersPresent, 11, 11) |
      util_bitpack_uint(values->TBIMREnable, 13, 13) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      util_bitpack_uint(values->PrimitiveTopologyType, 0, 5) |
      util_bitpack_uint(values->VertexAccessType, 8, 8) |
      util_bitpack_uint(values->EndOffsetEnable, 9, 9);

   dw[2] =
      util_bitpack_uint(values->VertexCountPerInstance, 0, 31);

   dw[3] =
      util_bitpack_uint(values->StartVertexLocation, 0, 31);

   dw[4] =
      util_bitpack_uint(values->InstanceCount, 0, 31);

   dw[5] =
      util_bitpack_uint(values->StartInstanceLocation, 0, 31);

   dw[6] =
      util_bitpack_sint(values->BaseVertexLocation, 0, 31);
}

static inline __attribute__((always_inline)) void
GFX30_3DPRIMITIVE_repack(__attribute__((unused)) global void * restrict dst,
                         __attribute__((unused)) global const uint32_t * origin,
                         __attribute__((unused)) private const struct GFX30_3DPRIMITIVE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->PredicateEnable, 8, 8) |
      util_bitpack_uint(values->UAVCoherencyRequired, 9, 9) |
      util_bitpack_uint(values->IndirectParameterEnable, 10, 10) |
      util_bitpack_uint(values->ExtendedParametersPresent, 11, 11) |
      util_bitpack_uint(values->TBIMREnable, 13, 13) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      origin[1] |
      util_bitpack_uint(values->PrimitiveTopologyType, 0, 5) |
      util_bitpack_uint(values->VertexAccessType, 8, 8) |
      util_bitpack_uint(values->EndOffsetEnable, 9, 9);

   dw[2] =
      origin[2] |
      util_bitpack_uint(values->VertexCountPerInstance, 0, 31);

   dw[3] =
      origin[3] |
      util_bitpack_uint(values->StartVertexLocation, 0, 31);

   dw[4] =
      origin[4] |
      util_bitpack_uint(values->InstanceCount, 0, 31);

   dw[5] =
      origin[5] |
      util_bitpack_uint(values->StartInstanceLocation, 0, 31);

   dw[6] =
      origin[6] |
      util_bitpack_sint(values->BaseVertexLocation, 0, 31);
}

#define GFX30_3DPRIMITIVE_EXTENDED_length     10
#define GFX30_3DPRIMITIVE_EXTENDED_length_bias      2
#define GFX30_3DPRIMITIVE_EXTENDED_header       \
   .DWordLength                         =      8,  \
   .ExtendedParametersPresent           =      1,  \
   ._3DCommandSubOpcode                 =      0,  \
   ._3DCommandOpcode                    =      3,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX30_3DPRIMITIVE_EXTENDED {
   uint32_t                             DWordLength;
   bool                                 PredicateEnable;
   bool                                 UAVCoherencyRequired;
   bool                                 IndirectParameterEnable;
   bool                                 ExtendedParametersPresent;
   uint32_t                             TBIMREnable;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   enum GFX30_3D_Prim_Topo_Type         PrimitiveTopologyType;
   uint32_t                             VertexAccessType;
#define SEQUENTIAL                               0
#define RANDOM                                   1
   bool                                 EndOffsetEnable;
   uint32_t                             VertexCountPerInstance;
   uint32_t                             StartVertexLocation;
   uint32_t                             InstanceCount;
   uint32_t                             StartInstanceLocation;
   int32_t                              BaseVertexLocation;
   uint32_t                             ExtendedParameter0;
   uint32_t                             ExtendedParameter1;
   uint32_t                             ExtendedParameter2;
};

static inline __attribute__((always_inline)) void
GFX30_3DPRIMITIVE_EXTENDED_pack(__attribute__((unused)) global void * restrict dst,
                                __attribute__((unused)) private const struct GFX30_3DPRIMITIVE_EXTENDED * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->PredicateEnable, 8, 8) |
      util_bitpack_uint(values->UAVCoherencyRequired, 9, 9) |
      util_bitpack_uint(values->IndirectParameterEnable, 10, 10) |
      util_bitpack_uint(values->ExtendedParametersPresent, 11, 11) |
      util_bitpack_uint(values->TBIMREnable, 13, 13) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      util_bitpack_uint(values->PrimitiveTopologyType, 0, 5) |
      util_bitpack_uint(values->VertexAccessType, 8, 8) |
      util_bitpack_uint(values->EndOffsetEnable, 9, 9);

   dw[2] =
      util_bitpack_uint(values->VertexCountPerInstance, 0, 31);

   dw[3] =
      util_bitpack_uint(values->StartVertexLocation, 0, 31);

   dw[4] =
      util_bitpack_uint(values->InstanceCount, 0, 31);

   dw[5] =
      util_bitpack_uint(values->StartInstanceLocation, 0, 31);

   dw[6] =
      util_bitpack_sint(values->BaseVertexLocation, 0, 31);

   dw[7] =
      util_bitpack_uint(values->ExtendedParameter0, 0, 31);

   dw[8] =
      util_bitpack_uint(values->ExtendedParameter1, 0, 31);

   dw[9] =
      util_bitpack_uint(values->ExtendedParameter2, 0, 31);
}

static inline __attribute__((always_inline)) void
GFX30_3DPRIMITIVE_EXTENDED_repack(__attribute__((unused)) global void * restrict dst,
                                  __attribute__((unused)) global const uint32_t * origin,
                                  __attribute__((unused)) private const struct GFX30_3DPRIMITIVE_EXTENDED * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->PredicateEnable, 8, 8) |
      util_bitpack_uint(values->UAVCoherencyRequired, 9, 9) |
      util_bitpack_uint(values->IndirectParameterEnable, 10, 10) |
      util_bitpack_uint(values->ExtendedParametersPresent, 11, 11) |
      util_bitpack_uint(values->TBIMREnable, 13, 13) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      origin[1] |
      util_bitpack_uint(values->PrimitiveTopologyType, 0, 5) |
      util_bitpack_uint(values->VertexAccessType, 8, 8) |
      util_bitpack_uint(values->EndOffsetEnable, 9, 9);

   dw[2] =
      origin[2] |
      util_bitpack_uint(values->VertexCountPerInstance, 0, 31);

   dw[3] =
      origin[3] |
      util_bitpack_uint(values->StartVertexLocation, 0, 31);

   dw[4] =
      origin[4] |
      util_bitpack_uint(values->InstanceCount, 0, 31);

   dw[5] =
      origin[5] |
      util_bitpack_uint(values->StartInstanceLocation, 0, 31);

   dw[6] =
      origin[6] |
      util_bitpack_sint(values->BaseVertexLocation, 0, 31);

   dw[7] =
      origin[7] |
      util_bitpack_uint(values->ExtendedParameter0, 0, 31);

   dw[8] =
      origin[8] |
      util_bitpack_uint(values->ExtendedParameter1, 0, 31);

   dw[9] =
      origin[9] |
      util_bitpack_uint(values->ExtendedParameter2, 0, 31);
}

#define GFX30_3DSTATE_DS_length               11
#define GFX30_3DSTATE_DS_length_bias           2
#define GFX30_3DSTATE_DS_header                 \
   .DWordLength                         =      9,  \
   ._3DCommandSubOpcode                 =     29,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX30_3DSTATE_DS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint64_t                             KernelStartPointer;
   uint32_t                             RegistersPerThread;
   bool                                 SoftwareExceptionEnable;
   bool                                 IllegalOpcodeExceptionEnable;
   bool                                 AccessesUAV;
   uint32_t                             FloatingPointMode;
#define IEEE754                                  0
#define Alternate                                1
   uint32_t                             ThreadDispatchPriority;
#define High                                     1
   uint32_t                             BindingTableEntryCount;
   uint32_t                             SamplerCount;
#define NoSamplers                               0
#define _14Samplers                              1
#define _58Samplers                              2
#define _912Samplers                             3
#define _1316Samplers                            4
   bool                                 VectorMaskEnable;
   uint32_t                             ScratchSpaceBuffer;
   uint32_t                             PatchURBEntryReadOffset;
   uint32_t                             PatchURBEntryReadLength;
   uint32_t                             DispatchGRFStartRegisterForURBData;
   bool                                 Enable;
   bool                                 CacheDisable;
   bool                                 ComputeWCoordinateEnable;
   uint32_t                             DispatchMode;
#define DISPATCH_MODE_SIMD8_SINGLE_PATCH         1
#define DISPATCH_MODE_SIMD8_SINGLE_OR_DUAL_PATCH 2
   bool                                 PrimitiveIDNotRequired;
   bool                                 StatisticsEnable;
   uint32_t                             MaximumNumberofThreads;
   uint32_t                             UserClipDistanceCullTestEnableBitmask;
   uint32_t                             UserClipDistanceClipTestEnableBitmask;
   uint32_t                             VertexURBEntryOutputLength;
   uint32_t                             VertexURBEntryOutputReadOffset;
   uint64_t                             QUAD_PATCHKernelStartPointer;
};

static inline __attribute__((always_inline)) void
GFX30_3DSTATE_DS_pack(__attribute__((unused)) global void * restrict dst,
                      __attribute__((unused)) private const struct GFX30_3DSTATE_DS * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   const uint64_t v1 =
      __gen_offset(values->KernelStartPointer, 6, 63);
   dw[1] = v1;
   dw[2] = v1 >> 32;

   dw[3] =
      util_bitpack_uint(values->RegistersPerThread, 0, 5) |
      util_bitpack_uint(values->SoftwareExceptionEnable, 7, 7) |
      util_bitpack_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      util_bitpack_uint(values->AccessesUAV, 14, 14) |
      util_bitpack_uint(values->FloatingPointMode, 16, 16) |
      util_bitpack_uint(values->ThreadDispatchPriority, 17, 17) |
      util_bitpack_uint(values->BindingTableEntryCount, 18, 25) |
      util_bitpack_uint(values->SamplerCount, 27, 29) |
      util_bitpack_uint(values->VectorMaskEnable, 30, 30);

   dw[4] =
      util_bitpack_uint(values->ScratchSpaceBuffer, 10, 31);

   dw[5] = 0;

   dw[6] =
      util_bitpack_uint(values->PatchURBEntryReadOffset, 4, 9) |
      util_bitpack_uint(values->PatchURBEntryReadLength, 11, 17) |
      util_bitpack_uint(values->DispatchGRFStartRegisterForURBData, 20, 24);

   dw[7] =
      util_bitpack_uint(values->Enable, 0, 0) |
      util_bitpack_uint(values->CacheDisable, 1, 1) |
      util_bitpack_uint(values->ComputeWCoordinateEnable, 2, 2) |
      util_bitpack_uint(values->DispatchMode, 3, 4) |
      util_bitpack_uint(values->PrimitiveIDNotRequired, 9, 9) |
      util_bitpack_uint(values->StatisticsEnable, 10, 10) |
      util_bitpack_uint(values->MaximumNumberofThreads, 21, 30);

   dw[8] =
      util_bitpack_uint(values->UserClipDistanceCullTestEnableBitmask, 0, 7) |
      util_bitpack_uint(values->UserClipDistanceClipTestEnableBitmask, 8, 15) |
      util_bitpack_uint(values->VertexURBEntryOutputLength, 16, 20) |
      util_bitpack_uint(values->VertexURBEntryOutputReadOffset, 21, 26);

   const uint64_t v9 =
      __gen_offset(values->QUAD_PATCHKernelStartPointer, 6, 63);
   dw[9] = v9;
   dw[10] = v9 >> 32;
}

static inline __attribute__((always_inline)) void
GFX30_3DSTATE_DS_repack(__attribute__((unused)) global void * restrict dst,
                        __attribute__((unused)) global const uint32_t * origin,
                        __attribute__((unused)) private const struct GFX30_3DSTATE_DS * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   const uint64_t v1 =
      origin[1] |
      ((uint64_t)origin[2] << 32) |
      __gen_offset(values->KernelStartPointer, 6, 63);
   dw[1] = v1;
   dw[2] = v1 >> 32;

   dw[3] =
      origin[3] |
      util_bitpack_uint(values->RegistersPerThread, 0, 5) |
      util_bitpack_uint(values->SoftwareExceptionEnable, 7, 7) |
      util_bitpack_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      util_bitpack_uint(values->AccessesUAV, 14, 14) |
      util_bitpack_uint(values->FloatingPointMode, 16, 16) |
      util_bitpack_uint(values->ThreadDispatchPriority, 17, 17) |
      util_bitpack_uint(values->BindingTableEntryCount, 18, 25) |
      util_bitpack_uint(values->SamplerCount, 27, 29) |
      util_bitpack_uint(values->VectorMaskEnable, 30, 30);

   dw[4] =
      origin[4] |
      util_bitpack_uint(values->ScratchSpaceBuffer, 10, 31);

   dw[5] = 0;

   dw[6] =
      origin[6] |
      util_bitpack_uint(values->PatchURBEntryReadOffset, 4, 9) |
      util_bitpack_uint(values->PatchURBEntryReadLength, 11, 17) |
      util_bitpack_uint(values->DispatchGRFStartRegisterForURBData, 20, 24);

   dw[7] =
      origin[7] |
      util_bitpack_uint(values->Enable, 0, 0) |
      util_bitpack_uint(values->CacheDisable, 1, 1) |
      util_bitpack_uint(values->ComputeWCoordinateEnable, 2, 2) |
      util_bitpack_uint(values->DispatchMode, 3, 4) |
      util_bitpack_uint(values->PrimitiveIDNotRequired, 9, 9) |
      util_bitpack_uint(values->StatisticsEnable, 10, 10) |
      util_bitpack_uint(values->MaximumNumberofThreads, 21, 30);

   dw[8] =
      origin[8] |
      util_bitpack_uint(values->UserClipDistanceCullTestEnableBitmask, 0, 7) |
      util_bitpack_uint(values->UserClipDistanceClipTestEnableBitmask, 8, 15) |
      util_bitpack_uint(values->VertexURBEntryOutputLength, 16, 20) |
      util_bitpack_uint(values->VertexURBEntryOutputReadOffset, 21, 26);

   const uint64_t v9 =
      origin[9] |
      ((uint64_t)origin[10] << 32) |
      __gen_offset(values->QUAD_PATCHKernelStartPointer, 6, 63);
   dw[9] = v9;
   dw[10] = v9 >> 32;
}

#define GFX30_3DSTATE_HS_length                8
#define GFX30_3DSTATE_HS_length_bias           2
#define GFX30_3DSTATE_HS_header                 \
   .DWordLength                         =      6,  \
   ._3DCommandSubOpcode                 =     27,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX30_3DSTATE_HS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             PatchCountThreshold;
   uint32_t                             RegistersPerThread;
   bool                                 SoftwareExceptionEnable;
   bool                                 IllegalOpcodeExceptionEnable;
   uint32_t                             FloatingPointMode;
#define IEEE754                                  0
#define alternate                                1
   uint32_t                             ThreadDispatchPriority;
#define Normal                                   0
#define High                                     1
   uint32_t                             BindingTableEntryCount;
   uint32_t                             SamplerCount;
#define NoSamplers                               0
#define _14Samplers                              1
#define _58Samplers                              2
#define _912Samplers                             3
#define _1316Samplers                            4
   uint32_t                             InstanceCount;
   uint32_t                             MaximumNumberofThreads;
   bool                                 StatisticsEnable;
   bool                                 Enable;
   uint64_t                             KernelStartPointer;
   uint32_t                             ScratchSpaceBuffer;
   bool                                 IncludePrimitiveID;
   uint32_t                             VertexURBEntryReadOffset;
   uint32_t                             VertexURBEntryReadLength;
   uint32_t                             DispatchGRFStartRegisterForURBData;
   bool                                 IncludeVertexHandles;
   bool                                 AccessesUAV;
   uint32_t                             VectorMaskEnable;
#define Dmask                                    0
   bool                                 SingleProgramFlow;
   uint32_t                             DispatchGRFStartRegisterForURBData5;
};

static inline __attribute__((always_inline)) void
GFX30_3DSTATE_HS_pack(__attribute__((unused)) global void * restrict dst,
                      __attribute__((unused)) private const struct GFX30_3DSTATE_HS * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      util_bitpack_uint(values->PatchCountThreshold, 0, 3) |
      util_bitpack_uint(values->RegistersPerThread, 4, 9) |
      util_bitpack_uint(values->SoftwareExceptionEnable, 12, 12) |
      util_bitpack_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      util_bitpack_uint(values->FloatingPointMode, 16, 16) |
      util_bitpack_uint(values->ThreadDispatchPriority, 17, 17) |
      util_bitpack_uint(values->BindingTableEntryCount, 18, 25) |
      util_bitpack_uint(values->SamplerCount, 27, 29);

   dw[2] =
      util_bitpack_uint(values->InstanceCount, 0, 4) |
      util_bitpack_uint(values->MaximumNumberofThreads, 8, 16) |
      util_bitpack_uint(values->StatisticsEnable, 29, 29) |
      util_bitpack_uint(values->Enable, 31, 31);

   const uint64_t v3 =
      __gen_offset(values->KernelStartPointer, 6, 63);
   dw[3] = v3;
   dw[4] = v3 >> 32;

   dw[5] =
      util_bitpack_uint(values->ScratchSpaceBuffer, 10, 31);

   dw[6] = 0;

   dw[7] =
      util_bitpack_uint(values->IncludePrimitiveID, 0, 0) |
      util_bitpack_uint(values->VertexURBEntryReadOffset, 4, 9) |
      util_bitpack_uint(values->VertexURBEntryReadLength, 11, 16) |
      util_bitpack_uint(values->DispatchGRFStartRegisterForURBData, 19, 23) |
      util_bitpack_uint(values->IncludeVertexHandles, 24, 24) |
      util_bitpack_uint(values->AccessesUAV, 25, 25) |
      util_bitpack_uint(values->VectorMaskEnable, 26, 26) |
      util_bitpack_uint(values->SingleProgramFlow, 27, 27) |
      util_bitpack_uint(values->DispatchGRFStartRegisterForURBData5, 28, 28);
}

static inline __attribute__((always_inline)) void
GFX30_3DSTATE_HS_repack(__attribute__((unused)) global void * restrict dst,
                        __attribute__((unused)) global const uint32_t * origin,
                        __attribute__((unused)) private const struct GFX30_3DSTATE_HS * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      origin[1] |
      util_bitpack_uint(values->PatchCountThreshold, 0, 3) |
      util_bitpack_uint(values->RegistersPerThread, 4, 9) |
      util_bitpack_uint(values->SoftwareExceptionEnable, 12, 12) |
      util_bitpack_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      util_bitpack_uint(values->FloatingPointMode, 16, 16) |
      util_bitpack_uint(values->ThreadDispatchPriority, 17, 17) |
      util_bitpack_uint(values->BindingTableEntryCount, 18, 25) |
      util_bitpack_uint(values->SamplerCount, 27, 29);

   dw[2] =
      origin[2] |
      util_bitpack_uint(values->InstanceCount, 0, 4) |
      util_bitpack_uint(values->MaximumNumberofThreads, 8, 16) |
      util_bitpack_uint(values->StatisticsEnable, 29, 29) |
      util_bitpack_uint(values->Enable, 31, 31);

   const uint64_t v3 =
      origin[3] |
      ((uint64_t)origin[4] << 32) |
      __gen_offset(values->KernelStartPointer, 6, 63);
   dw[3] = v3;
   dw[4] = v3 >> 32;

   dw[5] =
      origin[5] |
      util_bitpack_uint(values->ScratchSpaceBuffer, 10, 31);

   dw[6] = 0;

   dw[7] =
      origin[7] |
      util_bitpack_uint(values->IncludePrimitiveID, 0, 0) |
      util_bitpack_uint(values->VertexURBEntryReadOffset, 4, 9) |
      util_bitpack_uint(values->VertexURBEntryReadLength, 11, 16) |
      util_bitpack_uint(values->DispatchGRFStartRegisterForURBData, 19, 23) |
      util_bitpack_uint(values->IncludeVertexHandles, 24, 24) |
      util_bitpack_uint(values->AccessesUAV, 25, 25) |
      util_bitpack_uint(values->VectorMaskEnable, 26, 26) |
      util_bitpack_uint(values->SingleProgramFlow, 27, 27) |
      util_bitpack_uint(values->DispatchGRFStartRegisterForURBData5, 28, 28);
}

#define GFX30_3DSTATE_INDEX_BUFFER_length      5
#define GFX30_3DSTATE_INDEX_BUFFER_length_bias      2
#define GFX30_3DSTATE_INDEX_BUFFER_header       \
   .DWordLength                         =      3,  \
   ._3DCommandSubOpcode                 =     10,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX30_3DSTATE_INDEX_BUFFER {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             MOCS;
   uint32_t                             IndexFormat;
#define INDEX_BYTE                               0
#define INDEX_WORD                               1
#define INDEX_DWORD                              2
   bool                                 L3BypassDisable;
   uint64_t                             BufferStartingAddress;
   uint32_t                             BufferSize;
};

static inline __attribute__((always_inline)) void
GFX30_3DSTATE_INDEX_BUFFER_pack(__attribute__((unused)) global void * restrict dst,
                                __attribute__((unused)) private const struct GFX30_3DSTATE_INDEX_BUFFER * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      util_bitpack_uint_nonzero(values->MOCS, 0, 6) |
      util_bitpack_uint(values->IndexFormat, 8, 9) |
      util_bitpack_uint(values->L3BypassDisable, 11, 11);

   const uint64_t v2_address =
      __gen_address(values->BufferStartingAddress, 0, 63) | 0;
   dw[2] = v2_address;
   dw[3] = v2_address >> 32;

   dw[4] =
      util_bitpack_uint(values->BufferSize, 0, 31);
}

static inline __attribute__((always_inline)) void
GFX30_3DSTATE_INDEX_BUFFER_repack(__attribute__((unused)) global void * restrict dst,
                                  __attribute__((unused)) global const uint32_t * origin,
                                  __attribute__((unused)) private const struct GFX30_3DSTATE_INDEX_BUFFER * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      origin[1] |
      util_bitpack_uint_nonzero(values->MOCS, 0, 6) |
      util_bitpack_uint(values->IndexFormat, 8, 9) |
      util_bitpack_uint(values->L3BypassDisable, 11, 11);

   const uint64_t v2 =
      origin[2] |
      ((uint64_t)origin[3] << 32);
   const uint64_t v2_address =
      __gen_address(values->BufferStartingAddress, 0, 63) | v2;
   dw[2] = v2_address;
   dw[3] = v2_address >> 32;

   dw[4] =
      origin[4] |
      util_bitpack_uint(values->BufferSize, 0, 31);
}

#define GFX30_3DSTATE_VERTEX_BUFFERS_length_bias      2
#define GFX30_3DSTATE_VERTEX_BUFFERS_header     \
   .DWordLength                         =      3,  \
   ._3DCommandSubOpcode                 =      8,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX30_3DSTATE_VERTEX_BUFFERS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX30_3DSTATE_VERTEX_BUFFERS_pack(__attribute__((unused)) global void * restrict dst,
                                  __attribute__((unused)) private const struct GFX30_3DSTATE_VERTEX_BUFFERS * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);
}

static inline __attribute__((always_inline)) void
GFX30_3DSTATE_VERTEX_BUFFERS_repack(__attribute__((unused)) global void * restrict dst,
                                    __attribute__((unused)) global const uint32_t * origin,
                                    __attribute__((unused)) private const struct GFX30_3DSTATE_VERTEX_BUFFERS * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);
}

#define GFX30_MI_ARB_CHECK_length              1
#define GFX30_MI_ARB_CHECK_length_bias         1
#define GFX30_MI_ARB_CHECK_header               \
   .MICommandOpcode                     =      5,  \
   .CommandType                         =      0

struct GFX30_MI_ARB_CHECK {
   bool                                 PreParserDisable;
   bool                                 PreParserDisableMask;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX30_MI_ARB_CHECK_pack(__attribute__((unused)) global void * restrict dst,
                        __attribute__((unused)) private const struct GFX30_MI_ARB_CHECK * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->PreParserDisable, 0, 0) |
      util_bitpack_uint(values->PreParserDisableMask, 8, 8) |
      util_bitpack_uint(values->MICommandOpcode, 23, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);
}

static inline __attribute__((always_inline)) void
GFX30_MI_ARB_CHECK_repack(__attribute__((unused)) global void * restrict dst,
                          __attribute__((unused)) global const uint32_t * origin,
                          __attribute__((unused)) private const struct GFX30_MI_ARB_CHECK * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->PreParserDisable, 0, 0) |
      util_bitpack_uint(values->PreParserDisableMask, 8, 8) |
      util_bitpack_uint(values->MICommandOpcode, 23, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);
}

#define GFX30_MI_BATCH_BUFFER_START_length      3
#define GFX30_MI_BATCH_BUFFER_START_length_bias      2
#define GFX30_MI_BATCH_BUFFER_START_header      \
   .DWordLength                         =      1,  \
   .MICommandOpcode                     =     49,  \
   .CommandType                         =      0

struct GFX30_MI_BATCH_BUFFER_START {
   uint32_t                             DWordLength;
   uint32_t                             AddressSpaceIndicator;
#define ASI_GGTT                                 0
#define ASI_PPGTT                                1
   bool                                 ResourceStreamerEnable;
   bool                                 PredicationEnable;
   uint32_t                             SecondLevelBatchBuffer;
#define Firstlevelbatch                          0
#define Secondlevelbatch                         1
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint64_t                             BatchBufferStartAddress;
};

static inline __attribute__((always_inline)) void
GFX30_MI_BATCH_BUFFER_START_pack(__attribute__((unused)) global void * restrict dst,
                                 __attribute__((unused)) private const struct GFX30_MI_BATCH_BUFFER_START * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->AddressSpaceIndicator, 8, 8) |
      util_bitpack_uint(values->ResourceStreamerEnable, 10, 10) |
      util_bitpack_uint(values->PredicationEnable, 15, 15) |
      util_bitpack_uint(values->SecondLevelBatchBuffer, 22, 22) |
      util_bitpack_uint(values->MICommandOpcode, 23, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   const uint64_t v1_address =
      __gen_address(values->BatchBufferStartAddress, 2, 63) | 0;
   dw[1] = v1_address;
   dw[2] = v1_address >> 32;
}

static inline __attribute__((always_inline)) void
GFX30_MI_BATCH_BUFFER_START_repack(__attribute__((unused)) global void * restrict dst,
                                   __attribute__((unused)) global const uint32_t * origin,
                                   __attribute__((unused)) private const struct GFX30_MI_BATCH_BUFFER_START * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->AddressSpaceIndicator, 8, 8) |
      util_bitpack_uint(values->ResourceStreamerEnable, 10, 10) |
      util_bitpack_uint(values->PredicationEnable, 15, 15) |
      util_bitpack_uint(values->SecondLevelBatchBuffer, 22, 22) |
      util_bitpack_uint(values->MICommandOpcode, 23, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   const uint64_t v1 =
      origin[1] |
      ((uint64_t)origin[2] << 32);
   const uint64_t v1_address =
      __gen_address(values->BatchBufferStartAddress, 2, 63) | v1;
   dw[1] = v1_address;
   dw[2] = v1_address >> 32;
}

#endif /* GFX30_XE3_CL_PACK_H */
