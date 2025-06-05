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


/* Instructions, enums and structures for IVB.
 *
 * This file has been generated, do not hand edit.
 */

#ifndef GFX7_IVB_CL_PACK_H
#define GFX7_IVB_CL_PACK_H

#ifndef __OPENCL_VERSION__
#include <stdio.h>
#include "util/bitpack_helpers.h"
#include "genX_helpers.h"
#else
#include "genX_cl_helpers.h"
#endif


enum GFX7_3D_Color_Buffer_Blend_Factor {
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

enum GFX7_3D_Color_Buffer_Blend_Function {
   BLENDFUNCTION_ADD                    =      0,
   BLENDFUNCTION_SUBTRACT               =      1,
   BLENDFUNCTION_REVERSE_SUBTRACT       =      2,
   BLENDFUNCTION_MIN                    =      3,
   BLENDFUNCTION_MAX                    =      4,
};

enum GFX7_3D_Compare_Function {
   COMPAREFUNCTION_ALWAYS               =      0,
   COMPAREFUNCTION_NEVER                =      1,
   COMPAREFUNCTION_LESS                 =      2,
   COMPAREFUNCTION_EQUAL                =      3,
   COMPAREFUNCTION_LEQUAL               =      4,
   COMPAREFUNCTION_GREATER              =      5,
   COMPAREFUNCTION_NOTEQUAL             =      6,
   COMPAREFUNCTION_GEQUAL               =      7,
};

enum GFX7_3D_Logic_Op_Function {
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

enum GFX7_3D_Prim_Topo_Type {
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

enum GFX7_3D_Stencil_Operation {
   STENCILOP_KEEP                       =      0,
   STENCILOP_ZERO                       =      1,
   STENCILOP_REPLACE                    =      2,
   STENCILOP_INCRSAT                    =      3,
   STENCILOP_DECRSAT                    =      4,
   STENCILOP_INCR                       =      5,
   STENCILOP_DECR                       =      6,
   STENCILOP_INVERT                     =      7,
};

enum GFX7_3D_Vertex_Component_Control {
   VFCOMP_NOSTORE                       =      0,
   VFCOMP_STORE_SRC                     =      1,
   VFCOMP_STORE_0                       =      2,
   VFCOMP_STORE_1_FP                    =      3,
   VFCOMP_STORE_1_INT                   =      4,
   VFCOMP_STORE_VID                     =      5,
   VFCOMP_STORE_IID                     =      6,
   VFCOMP_STORE_PID                     =      7,
};

enum GFX7_TextureCoordinateMode {
   TCM_WRAP                             =      0,
   TCM_MIRROR                           =      1,
   TCM_CLAMP                            =      2,
   TCM_CUBE                             =      3,
   TCM_CLAMP_BORDER                     =      4,
   TCM_MIRROR_ONCE                      =      5,
};

#define GFX7_VERTEX_BUFFER_STATE_length        4
struct GFX7_VERTEX_BUFFER_STATE {
   uint32_t                             BufferPitch;
   bool                                 VertexFetchInvalidate;
   bool                                 NullVertexBuffer;
   bool                                 AddressModifyEnable;
   uint32_t                             MOCS;
   uint32_t                             BufferAccessType;
#define VERTEXDATA                               0
#define INSTANCEDATA                             1
   uint32_t                             VertexBufferIndex;
   uint64_t                             BufferStartingAddress;
   uint64_t                             EndAddress;
   uint32_t                             InstanceDataStepRate;
};

static inline __attribute__((always_inline)) void
GFX7_VERTEX_BUFFER_STATE_pack(__attribute__((unused)) global void * restrict dst,
                              __attribute__((unused)) private const struct GFX7_VERTEX_BUFFER_STATE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->BufferPitch, 0, 11) |
      util_bitpack_uint(values->VertexFetchInvalidate, 12, 12) |
      util_bitpack_uint(values->NullVertexBuffer, 13, 13) |
      util_bitpack_uint(values->AddressModifyEnable, 14, 14) |
      util_bitpack_uint_nonzero(values->MOCS, 16, 19) |
      util_bitpack_uint(values->BufferAccessType, 20, 20) |
      util_bitpack_uint(values->VertexBufferIndex, 26, 31);

   dw[1] = __gen_address(values->BufferStartingAddress, 0, 31) | 0;

   dw[2] = __gen_address(values->EndAddress, 0, 31) | 0;

   dw[3] =
      util_bitpack_uint(values->InstanceDataStepRate, 0, 31);
}

static inline __attribute__((always_inline)) void
GFX7_VERTEX_BUFFER_STATE_repack(__attribute__((unused)) global void * restrict dst,
                                __attribute__((unused)) global const uint32_t * origin,
                                __attribute__((unused)) private const struct GFX7_VERTEX_BUFFER_STATE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->BufferPitch, 0, 11) |
      util_bitpack_uint(values->VertexFetchInvalidate, 12, 12) |
      util_bitpack_uint(values->NullVertexBuffer, 13, 13) |
      util_bitpack_uint(values->AddressModifyEnable, 14, 14) |
      util_bitpack_uint_nonzero(values->MOCS, 16, 19) |
      util_bitpack_uint(values->BufferAccessType, 20, 20) |
      util_bitpack_uint(values->VertexBufferIndex, 26, 31);

   const uint32_t v1 =
      origin[1];
   dw[1] = __gen_address(values->BufferStartingAddress, 0, 31) | v1;

   const uint32_t v2 =
      origin[2];
   dw[2] = __gen_address(values->EndAddress, 0, 31) | v2;

   dw[3] =
      origin[3] |
      util_bitpack_uint(values->InstanceDataStepRate, 0, 31);
}

#define GFX7_3DPRIMITIVE_length                7
#define GFX7_3DPRIMITIVE_length_bias           2
#define GFX7_3DPRIMITIVE_header                 \
   .DWordLength                         =      5,  \
   ._3DCommandSubOpcode                 =      0,  \
   ._3DCommandOpcode                    =      3,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX7_3DPRIMITIVE {
   uint32_t                             DWordLength;
   bool                                 PredicateEnable;
   bool                                 IndirectParameterEnable;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   enum GFX7_3D_Prim_Topo_Type          PrimitiveTopologyType;
   uint32_t                             VertexAccessType;
#define SEQUENTIAL                               0
#define RANDOM                                   1
   bool                                 EndOffsetEnable;
   uint32_t                             VertexCountPerInstance;
   uint32_t                             StartVertexLocation;
   uint32_t                             InstanceCount;
   uint32_t                             StartInstanceLocation;
   int32_t                              BaseVertexLocation;
};

static inline __attribute__((always_inline)) void
GFX7_3DPRIMITIVE_pack(__attribute__((unused)) global void * restrict dst,
                      __attribute__((unused)) private const struct GFX7_3DPRIMITIVE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->PredicateEnable, 8, 8) |
      util_bitpack_uint(values->IndirectParameterEnable, 10, 10) |
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
GFX7_3DPRIMITIVE_repack(__attribute__((unused)) global void * restrict dst,
                        __attribute__((unused)) global const uint32_t * origin,
                        __attribute__((unused)) private const struct GFX7_3DPRIMITIVE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->PredicateEnable, 8, 8) |
      util_bitpack_uint(values->IndirectParameterEnable, 10, 10) |
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

#define GFX7_3DSTATE_DS_length                 6
#define GFX7_3DSTATE_DS_length_bias            2
#define GFX7_3DSTATE_DS_header                  \
   .DWordLength                         =      4,  \
   ._3DCommandSubOpcode                 =     29,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX7_3DSTATE_DS {
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
   uint32_t                             SingleDomainPointDispatch;
   uint32_t                             PerThreadScratchSpace;
   uint64_t                             ScratchSpaceBasePointer;
   uint32_t                             PatchURBEntryReadOffset;
   uint32_t                             PatchURBEntryReadLength;
   uint32_t                             DispatchGRFStartRegisterForURBData;
   bool                                 Enable;
   bool                                 DSCacheDisable;
   bool                                 ComputeWCoordinateEnable;
   bool                                 StatisticsEnable;
   uint32_t                             MaximumNumberofThreads;
};

static inline __attribute__((always_inline)) void
GFX7_3DSTATE_DS_pack(__attribute__((unused)) global void * restrict dst,
                     __attribute__((unused)) private const struct GFX7_3DSTATE_DS * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      __gen_offset(values->KernelStartPointer, 6, 31);

   dw[2] =
      util_bitpack_uint(values->SoftwareExceptionEnable, 7, 7) |
      util_bitpack_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      util_bitpack_uint(values->FloatingPointMode, 16, 16) |
      util_bitpack_uint(values->BindingTableEntryCount, 18, 25) |
      util_bitpack_uint(values->SamplerCount, 27, 29) |
      util_bitpack_uint(values->VectorMaskEnable, 30, 30) |
      util_bitpack_uint(values->SingleDomainPointDispatch, 31, 31);

   const uint32_t v3 =
      util_bitpack_uint(values->PerThreadScratchSpace, 0, 3);
   dw[3] = __gen_address(values->ScratchSpaceBasePointer, 10, 31) | v3;

   dw[4] =
      util_bitpack_uint(values->PatchURBEntryReadOffset, 4, 9) |
      util_bitpack_uint(values->PatchURBEntryReadLength, 11, 17) |
      util_bitpack_uint(values->DispatchGRFStartRegisterForURBData, 20, 24);

   dw[5] =
      util_bitpack_uint(values->Enable, 0, 0) |
      util_bitpack_uint(values->DSCacheDisable, 1, 1) |
      util_bitpack_uint(values->ComputeWCoordinateEnable, 2, 2) |
      util_bitpack_uint(values->StatisticsEnable, 10, 10) |
      util_bitpack_uint(values->MaximumNumberofThreads, 25, 31);
}

static inline __attribute__((always_inline)) void
GFX7_3DSTATE_DS_repack(__attribute__((unused)) global void * restrict dst,
                       __attribute__((unused)) global const uint32_t * origin,
                       __attribute__((unused)) private const struct GFX7_3DSTATE_DS * restrict values)
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
      __gen_offset(values->KernelStartPointer, 6, 31);

   dw[2] =
      origin[2] |
      util_bitpack_uint(values->SoftwareExceptionEnable, 7, 7) |
      util_bitpack_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      util_bitpack_uint(values->FloatingPointMode, 16, 16) |
      util_bitpack_uint(values->BindingTableEntryCount, 18, 25) |
      util_bitpack_uint(values->SamplerCount, 27, 29) |
      util_bitpack_uint(values->VectorMaskEnable, 30, 30) |
      util_bitpack_uint(values->SingleDomainPointDispatch, 31, 31);

   const uint32_t v3 =
      origin[3] |
      util_bitpack_uint(values->PerThreadScratchSpace, 0, 3);
   dw[3] = __gen_address(values->ScratchSpaceBasePointer, 10, 31) | v3;

   dw[4] =
      origin[4] |
      util_bitpack_uint(values->PatchURBEntryReadOffset, 4, 9) |
      util_bitpack_uint(values->PatchURBEntryReadLength, 11, 17) |
      util_bitpack_uint(values->DispatchGRFStartRegisterForURBData, 20, 24);

   dw[5] =
      origin[5] |
      util_bitpack_uint(values->Enable, 0, 0) |
      util_bitpack_uint(values->DSCacheDisable, 1, 1) |
      util_bitpack_uint(values->ComputeWCoordinateEnable, 2, 2) |
      util_bitpack_uint(values->StatisticsEnable, 10, 10) |
      util_bitpack_uint(values->MaximumNumberofThreads, 25, 31);
}

#define GFX7_3DSTATE_HS_length                 7
#define GFX7_3DSTATE_HS_length_bias            2
#define GFX7_3DSTATE_HS_header                  \
   .DWordLength                         =      5,  \
   ._3DCommandSubOpcode                 =     27,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX7_3DSTATE_HS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint32_t                             MaximumNumberofThreads;
   bool                                 SoftwareExceptionEnable;
   bool                                 IllegalOpcodeExceptionEnable;
   uint32_t                             FloatingPointMode;
#define IEEE754                                  0
#define alternate                                1
   uint32_t                             BindingTableEntryCount;
   uint32_t                             SamplerCount;
#define NoSamplers                               0
#define _14Samplers                              1
#define _58Samplers                              2
#define _912Samplers                             3
#define _1316Samplers                            4
   uint32_t                             InstanceCount;
   bool                                 StatisticsEnable;
   bool                                 Enable;
   uint64_t                             KernelStartPointer;
   uint32_t                             PerThreadScratchSpace;
   uint64_t                             ScratchSpaceBasePointer;
   uint32_t                             VertexURBEntryReadOffset;
   uint32_t                             VertexURBEntryReadLength;
   uint32_t                             DispatchGRFStartRegisterForURBData;
   bool                                 IncludeVertexHandles;
   bool                                 VectorMaskEnable;
   bool                                 SingleProgramFlow;
   uint64_t                             SemaphoreHandle;
};

static inline __attribute__((always_inline)) void
GFX7_3DSTATE_HS_pack(__attribute__((unused)) global void * restrict dst,
                     __attribute__((unused)) private const struct GFX7_3DSTATE_HS * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      util_bitpack_uint(values->MaximumNumberofThreads, 0, 6) |
      util_bitpack_uint(values->SoftwareExceptionEnable, 7, 7) |
      util_bitpack_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      util_bitpack_uint(values->FloatingPointMode, 16, 16) |
      util_bitpack_uint(values->BindingTableEntryCount, 18, 25) |
      util_bitpack_uint(values->SamplerCount, 27, 29);

   dw[2] =
      util_bitpack_uint(values->InstanceCount, 0, 3) |
      util_bitpack_uint(values->StatisticsEnable, 29, 29) |
      util_bitpack_uint(values->Enable, 31, 31);

   dw[3] =
      __gen_offset(values->KernelStartPointer, 6, 31);

   const uint32_t v4 =
      util_bitpack_uint(values->PerThreadScratchSpace, 0, 3);
   dw[4] = __gen_address(values->ScratchSpaceBasePointer, 10, 31) | v4;

   dw[5] =
      util_bitpack_uint(values->VertexURBEntryReadOffset, 4, 9) |
      util_bitpack_uint(values->VertexURBEntryReadLength, 11, 16) |
      util_bitpack_uint(values->DispatchGRFStartRegisterForURBData, 19, 23) |
      util_bitpack_uint(values->IncludeVertexHandles, 24, 24) |
      util_bitpack_uint(values->VectorMaskEnable, 26, 26) |
      util_bitpack_uint(values->SingleProgramFlow, 27, 27);

   dw[6] =
      __gen_offset(values->SemaphoreHandle, 0, 11);
}

static inline __attribute__((always_inline)) void
GFX7_3DSTATE_HS_repack(__attribute__((unused)) global void * restrict dst,
                       __attribute__((unused)) global const uint32_t * origin,
                       __attribute__((unused)) private const struct GFX7_3DSTATE_HS * restrict values)
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
      util_bitpack_uint(values->MaximumNumberofThreads, 0, 6) |
      util_bitpack_uint(values->SoftwareExceptionEnable, 7, 7) |
      util_bitpack_uint(values->IllegalOpcodeExceptionEnable, 13, 13) |
      util_bitpack_uint(values->FloatingPointMode, 16, 16) |
      util_bitpack_uint(values->BindingTableEntryCount, 18, 25) |
      util_bitpack_uint(values->SamplerCount, 27, 29);

   dw[2] =
      origin[2] |
      util_bitpack_uint(values->InstanceCount, 0, 3) |
      util_bitpack_uint(values->StatisticsEnable, 29, 29) |
      util_bitpack_uint(values->Enable, 31, 31);

   dw[3] =
      origin[3] |
      __gen_offset(values->KernelStartPointer, 6, 31);

   const uint32_t v4 =
      origin[4] |
      util_bitpack_uint(values->PerThreadScratchSpace, 0, 3);
   dw[4] = __gen_address(values->ScratchSpaceBasePointer, 10, 31) | v4;

   dw[5] =
      origin[5] |
      util_bitpack_uint(values->VertexURBEntryReadOffset, 4, 9) |
      util_bitpack_uint(values->VertexURBEntryReadLength, 11, 16) |
      util_bitpack_uint(values->DispatchGRFStartRegisterForURBData, 19, 23) |
      util_bitpack_uint(values->IncludeVertexHandles, 24, 24) |
      util_bitpack_uint(values->VectorMaskEnable, 26, 26) |
      util_bitpack_uint(values->SingleProgramFlow, 27, 27);

   dw[6] =
      origin[6] |
      __gen_offset(values->SemaphoreHandle, 0, 11);
}

#define GFX7_3DSTATE_INDEX_BUFFER_length       3
#define GFX7_3DSTATE_INDEX_BUFFER_length_bias      2
#define GFX7_3DSTATE_INDEX_BUFFER_header        \
   .DWordLength                         =      1,  \
   ._3DCommandSubOpcode                 =     10,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX7_3DSTATE_INDEX_BUFFER {
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
   uint64_t                             BufferStartingAddress;
   uint64_t                             BufferEndingAddress;
};

static inline __attribute__((always_inline)) void
GFX7_3DSTATE_INDEX_BUFFER_pack(__attribute__((unused)) global void * restrict dst,
                               __attribute__((unused)) private const struct GFX7_3DSTATE_INDEX_BUFFER * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->IndexFormat, 8, 9) |
      util_bitpack_uint(values->CutIndexEnable, 10, 10) |
      util_bitpack_uint_nonzero(values->MOCS, 12, 15) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] = __gen_address(values->BufferStartingAddress, 0, 31) | 0;

   dw[2] = __gen_address(values->BufferEndingAddress, 0, 31) | 0;
}

static inline __attribute__((always_inline)) void
GFX7_3DSTATE_INDEX_BUFFER_repack(__attribute__((unused)) global void * restrict dst,
                                 __attribute__((unused)) global const uint32_t * origin,
                                 __attribute__((unused)) private const struct GFX7_3DSTATE_INDEX_BUFFER * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->IndexFormat, 8, 9) |
      util_bitpack_uint(values->CutIndexEnable, 10, 10) |
      util_bitpack_uint_nonzero(values->MOCS, 12, 15) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   const uint32_t v1 =
      origin[1];
   dw[1] = __gen_address(values->BufferStartingAddress, 0, 31) | v1;

   const uint32_t v2 =
      origin[2];
   dw[2] = __gen_address(values->BufferEndingAddress, 0, 31) | v2;
}

#define GFX7_3DSTATE_VERTEX_BUFFERS_length_bias      2
#define GFX7_3DSTATE_VERTEX_BUFFERS_header      \
   .DWordLength                         =      3,  \
   ._3DCommandSubOpcode                 =      8,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX7_3DSTATE_VERTEX_BUFFERS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX7_3DSTATE_VERTEX_BUFFERS_pack(__attribute__((unused)) global void * restrict dst,
                                 __attribute__((unused)) private const struct GFX7_3DSTATE_VERTEX_BUFFERS * restrict values)
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
GFX7_3DSTATE_VERTEX_BUFFERS_repack(__attribute__((unused)) global void * restrict dst,
                                   __attribute__((unused)) global const uint32_t * origin,
                                   __attribute__((unused)) private const struct GFX7_3DSTATE_VERTEX_BUFFERS * restrict values)
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

#define GFX7_MI_ARB_CHECK_length               1
#define GFX7_MI_ARB_CHECK_length_bias          1
#define GFX7_MI_ARB_CHECK_header                \
   .MICommandOpcode                     =      5,  \
   .CommandType                         =      0

struct GFX7_MI_ARB_CHECK {
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
};

static inline __attribute__((always_inline)) void
GFX7_MI_ARB_CHECK_pack(__attribute__((unused)) global void * restrict dst,
                       __attribute__((unused)) private const struct GFX7_MI_ARB_CHECK * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->MICommandOpcode, 23, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);
}

static inline __attribute__((always_inline)) void
GFX7_MI_ARB_CHECK_repack(__attribute__((unused)) global void * restrict dst,
                         __attribute__((unused)) global const uint32_t * origin,
                         __attribute__((unused)) private const struct GFX7_MI_ARB_CHECK * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->MICommandOpcode, 23, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);
}

#define GFX7_MI_BATCH_BUFFER_START_length      2
#define GFX7_MI_BATCH_BUFFER_START_length_bias      2
#define GFX7_MI_BATCH_BUFFER_START_header       \
   .DWordLength                         =      0,  \
   .MICommandOpcode                     =     49,  \
   .CommandType                         =      0

struct GFX7_MI_BATCH_BUFFER_START {
   uint32_t                             DWordLength;
   uint32_t                             AddressSpaceIndicator;
#define ASI_GGTT                                 0
#define ASI_PPGTT                                1
   bool                                 ClearCommandBufferEnable;
   uint32_t                             MICommandOpcode;
   uint32_t                             CommandType;
   uint64_t                             BatchBufferStartAddress;
};

static inline __attribute__((always_inline)) void
GFX7_MI_BATCH_BUFFER_START_pack(__attribute__((unused)) global void * restrict dst,
                                __attribute__((unused)) private const struct GFX7_MI_BATCH_BUFFER_START * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->AddressSpaceIndicator, 8, 8) |
      util_bitpack_uint(values->ClearCommandBufferEnable, 11, 11) |
      util_bitpack_uint(values->MICommandOpcode, 23, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] = __gen_address(values->BatchBufferStartAddress, 2, 31) | 0;
}

static inline __attribute__((always_inline)) void
GFX7_MI_BATCH_BUFFER_START_repack(__attribute__((unused)) global void * restrict dst,
                                  __attribute__((unused)) global const uint32_t * origin,
                                  __attribute__((unused)) private const struct GFX7_MI_BATCH_BUFFER_START * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->AddressSpaceIndicator, 8, 8) |
      util_bitpack_uint(values->ClearCommandBufferEnable, 11, 11) |
      util_bitpack_uint(values->MICommandOpcode, 23, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   const uint32_t v1 =
      origin[1];
   dw[1] = __gen_address(values->BatchBufferStartAddress, 2, 31) | v1;
}

#endif /* GFX7_IVB_CL_PACK_H */
