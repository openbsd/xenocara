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


/* Instructions, enums and structures for BRW.
 *
 * This file has been generated, do not hand edit.
 */

#ifndef GFX4_BRW_CL_PACK_H
#define GFX4_BRW_CL_PACK_H

#ifndef __OPENCL_VERSION__
#include <stdio.h>
#include "util/bitpack_helpers.h"
#include "genX_helpers.h"
#else
#include "genX_cl_helpers.h"
#endif


enum GFX4_3D_Color_Buffer_Blend_Factor {
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

enum GFX4_3D_Color_Buffer_Blend_Function {
   BLENDFUNCTION_ADD                    =      0,
   BLENDFUNCTION_SUBTRACT               =      1,
   BLENDFUNCTION_REVERSE_SUBTRACT       =      2,
   BLENDFUNCTION_MIN                    =      3,
   BLENDFUNCTION_MAX                    =      4,
};

enum GFX4_3D_Compare_Function {
   COMPAREFUNCTION_ALWAYS               =      0,
   COMPAREFUNCTION_NEVER                =      1,
   COMPAREFUNCTION_LESS                 =      2,
   COMPAREFUNCTION_EQUAL                =      3,
   COMPAREFUNCTION_LEQUAL               =      4,
   COMPAREFUNCTION_GREATER              =      5,
   COMPAREFUNCTION_NOTEQUAL             =      6,
   COMPAREFUNCTION_GEQUAL               =      7,
};

enum GFX4_3D_Logic_Op_Function {
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

enum GFX4_3D_Prim_Topo_Type {
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

enum GFX4_3D_Stencil_Operation {
   STENCILOP_KEEP                       =      0,
   STENCILOP_ZERO                       =      1,
   STENCILOP_REPLACE                    =      2,
   STENCILOP_INCRSAT                    =      3,
   STENCILOP_DECRSAT                    =      4,
   STENCILOP_INCR                       =      5,
   STENCILOP_DECR                       =      6,
   STENCILOP_INVERT                     =      7,
};

enum GFX4_3D_Vertex_Component_Control {
   VFCOMP_NOSTORE                       =      0,
   VFCOMP_STORE_SRC                     =      1,
   VFCOMP_STORE_0                       =      2,
   VFCOMP_STORE_1_FP                    =      3,
   VFCOMP_STORE_1_INT                   =      4,
   VFCOMP_STORE_VID                     =      5,
   VFCOMP_STORE_IID                     =      6,
   VFCOMP_STORE_PID                     =      7,
};

enum GFX4_TextureCoordinateMode {
   TCM_WRAP                             =      0,
   TCM_MIRROR                           =      1,
   TCM_CLAMP                            =      2,
   TCM_CUBE                             =      3,
   TCM_CLAMP_BORDER                     =      4,
   TCM_MIRROR_ONCE                      =      5,
};

#define GFX4_VERTEX_BUFFER_STATE_length        4
struct GFX4_VERTEX_BUFFER_STATE {
   uint32_t                             BufferPitch;
   uint32_t                             BufferAccessType;
#define VERTEXDATA                               0
#define INSTANCEDATA                             1
   uint32_t                             VertexBufferIndex;
   uint64_t                             BufferStartingAddress;
   uint32_t                             MaxIndex;
   uint32_t                             InstanceDataStepRate;
};

static inline __attribute__((always_inline)) void
GFX4_VERTEX_BUFFER_STATE_pack(__attribute__((unused)) global void * restrict dst,
                              __attribute__((unused)) private const struct GFX4_VERTEX_BUFFER_STATE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->BufferPitch, 0, 10) |
      util_bitpack_uint(values->BufferAccessType, 26, 26) |
      util_bitpack_uint(values->VertexBufferIndex, 27, 31);

   dw[1] = __gen_address(values->BufferStartingAddress, 0, 31) | 0;

   dw[2] =
      util_bitpack_uint(values->MaxIndex, 0, 31);

   dw[3] =
      util_bitpack_uint(values->InstanceDataStepRate, 0, 31);
}

static inline __attribute__((always_inline)) void
GFX4_VERTEX_BUFFER_STATE_repack(__attribute__((unused)) global void * restrict dst,
                                __attribute__((unused)) global const uint32_t * origin,
                                __attribute__((unused)) private const struct GFX4_VERTEX_BUFFER_STATE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->BufferPitch, 0, 10) |
      util_bitpack_uint(values->BufferAccessType, 26, 26) |
      util_bitpack_uint(values->VertexBufferIndex, 27, 31);

   const uint32_t v1 =
      origin[1];
   dw[1] = __gen_address(values->BufferStartingAddress, 0, 31) | v1;

   dw[2] =
      origin[2] |
      util_bitpack_uint(values->MaxIndex, 0, 31);

   dw[3] =
      origin[3] |
      util_bitpack_uint(values->InstanceDataStepRate, 0, 31);
}

#define GFX4_3DPRIMITIVE_length                6
#define GFX4_3DPRIMITIVE_length_bias           2
#define GFX4_3DPRIMITIVE_header                 \
   .DWordLength                         =      4,  \
   ._3DCommandSubOpcode                 =      0,  \
   ._3DCommandOpcode                    =      3,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3,  \
   .InstanceCount                       =      1

struct GFX4_3DPRIMITIVE {
   uint32_t                             DWordLength;
   enum GFX4_3D_Prim_Topo_Type          PrimitiveTopologyType;
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
   int32_t                              BaseVertexLocation;
};

static inline __attribute__((always_inline)) void
GFX4_3DPRIMITIVE_pack(__attribute__((unused)) global void * restrict dst,
                      __attribute__((unused)) private const struct GFX4_3DPRIMITIVE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->PrimitiveTopologyType, 10, 14) |
      util_bitpack_uint(values->VertexAccessType, 15, 15) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      util_bitpack_uint(values->VertexCountPerInstance, 0, 31);

   dw[2] =
      util_bitpack_uint(values->StartVertexLocation, 0, 31);

   dw[3] =
      util_bitpack_uint(values->InstanceCount, 0, 31);

   dw[4] = 0;

   dw[5] =
      util_bitpack_sint(values->BaseVertexLocation, 0, 31);
}

static inline __attribute__((always_inline)) void
GFX4_3DPRIMITIVE_repack(__attribute__((unused)) global void * restrict dst,
                        __attribute__((unused)) global const uint32_t * origin,
                        __attribute__((unused)) private const struct GFX4_3DPRIMITIVE * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->PrimitiveTopologyType, 10, 14) |
      util_bitpack_uint(values->VertexAccessType, 15, 15) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] =
      origin[1] |
      util_bitpack_uint(values->VertexCountPerInstance, 0, 31);

   dw[2] =
      origin[2] |
      util_bitpack_uint(values->StartVertexLocation, 0, 31);

   dw[3] =
      origin[3] |
      util_bitpack_uint(values->InstanceCount, 0, 31);

   dw[4] = 0;

   dw[5] =
      origin[5] |
      util_bitpack_sint(values->BaseVertexLocation, 0, 31);
}

#define GFX4_3DSTATE_INDEX_BUFFER_length       3
#define GFX4_3DSTATE_INDEX_BUFFER_length_bias      2
#define GFX4_3DSTATE_INDEX_BUFFER_header        \
   .DWordLength                         =      1,  \
   ._3DCommandSubOpcode                 =     10,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX4_3DSTATE_INDEX_BUFFER {
   uint32_t                             DWordLength;
   uint32_t                             IndexFormat;
#define INDEX_BYTE                               0
#define INDEX_WORD                               1
#define INDEX_DWORD                              2
   bool                                 CutIndexEnable;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   uint64_t                             BufferStartingAddress;
   uint64_t                             BufferEndingAddress;
};

static inline __attribute__((always_inline)) void
GFX4_3DSTATE_INDEX_BUFFER_pack(__attribute__((unused)) global void * restrict dst,
                               __attribute__((unused)) private const struct GFX4_3DSTATE_INDEX_BUFFER * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->IndexFormat, 8, 9) |
      util_bitpack_uint(values->CutIndexEnable, 10, 10) |
      util_bitpack_uint(values->_3DCommandSubOpcode, 16, 23) |
      util_bitpack_uint(values->_3DCommandOpcode, 24, 26) |
      util_bitpack_uint(values->CommandSubType, 27, 28) |
      util_bitpack_uint(values->CommandType, 29, 31);

   dw[1] = __gen_address(values->BufferStartingAddress, 0, 31) | 0;

   dw[2] = __gen_address(values->BufferEndingAddress, 0, 31) | 0;
}

static inline __attribute__((always_inline)) void
GFX4_3DSTATE_INDEX_BUFFER_repack(__attribute__((unused)) global void * restrict dst,
                                 __attribute__((unused)) global const uint32_t * origin,
                                 __attribute__((unused)) private const struct GFX4_3DSTATE_INDEX_BUFFER * restrict values)
{
   global uint32_t * dw = (global uint32_t *) dst;

   dw[0] =
      origin[0] |
      util_bitpack_uint(values->DWordLength, 0, 7) |
      util_bitpack_uint(values->IndexFormat, 8, 9) |
      util_bitpack_uint(values->CutIndexEnable, 10, 10) |
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

#define GFX4_3DSTATE_VERTEX_BUFFERS_length_bias      2
#define GFX4_3DSTATE_VERTEX_BUFFERS_header      \
   .DWordLength                         =      3,  \
   ._3DCommandSubOpcode                 =      8,  \
   ._3DCommandOpcode                    =      0,  \
   .CommandSubType                      =      3,  \
   .CommandType                         =      3

struct GFX4_3DSTATE_VERTEX_BUFFERS {
   uint32_t                             DWordLength;
   uint32_t                             _3DCommandSubOpcode;
   uint32_t                             _3DCommandOpcode;
   uint32_t                             CommandSubType;
   uint32_t                             CommandType;
   /* variable length fields follow */
};

static inline __attribute__((always_inline)) void
GFX4_3DSTATE_VERTEX_BUFFERS_pack(__attribute__((unused)) global void * restrict dst,
                                 __attribute__((unused)) private const struct GFX4_3DSTATE_VERTEX_BUFFERS * restrict values)
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
GFX4_3DSTATE_VERTEX_BUFFERS_repack(__attribute__((unused)) global void * restrict dst,
                                   __attribute__((unused)) global const uint32_t * origin,
                                   __attribute__((unused)) private const struct GFX4_3DSTATE_VERTEX_BUFFERS * restrict values)
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

#endif /* GFX4_BRW_CL_PACK_H */
