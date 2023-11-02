/* DO NOT EDIT - This file generated automatically by gl_marshal_h.py script */

/*
 * Copyright (C) 2012 Intel Corporation
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
 * INTEL CORPORATION,
 * AND/OR THEIR SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#ifndef MARSHAL_GENERATED_H
#define MARSHAL_GENERATED_H

#include "util/glheader.h"

enum marshal_dispatch_cmd_id
{
   DISPATCH_CMD_NewList,
   DISPATCH_CMD_EndList,
   DISPATCH_CMD_CallList,
   DISPATCH_CMD_CallLists,
   DISPATCH_CMD_DeleteLists,
   DISPATCH_CMD_ListBase,
   DISPATCH_CMD_Begin,
   DISPATCH_CMD_Bitmap,
   DISPATCH_CMD_Color3b,
   DISPATCH_CMD_Color3bv,
   DISPATCH_CMD_Color3d,
   DISPATCH_CMD_Color3dv,
   DISPATCH_CMD_Color3f,
   DISPATCH_CMD_Color3fv,
   DISPATCH_CMD_Color3i,
   DISPATCH_CMD_Color3iv,
   DISPATCH_CMD_Color3s,
   DISPATCH_CMD_Color3sv,
   DISPATCH_CMD_Color3ub,
   DISPATCH_CMD_Color3ubv,
   DISPATCH_CMD_Color3ui,
   DISPATCH_CMD_Color3uiv,
   DISPATCH_CMD_Color3us,
   DISPATCH_CMD_Color3usv,
   DISPATCH_CMD_Color4b,
   DISPATCH_CMD_Color4bv,
   DISPATCH_CMD_Color4d,
   DISPATCH_CMD_Color4dv,
   DISPATCH_CMD_Color4f,
   DISPATCH_CMD_Color4fv,
   DISPATCH_CMD_Color4i,
   DISPATCH_CMD_Color4iv,
   DISPATCH_CMD_Color4s,
   DISPATCH_CMD_Color4sv,
   DISPATCH_CMD_Color4ub,
   DISPATCH_CMD_Color4ubv,
   DISPATCH_CMD_Color4ui,
   DISPATCH_CMD_Color4uiv,
   DISPATCH_CMD_Color4us,
   DISPATCH_CMD_Color4usv,
   DISPATCH_CMD_EdgeFlag,
   DISPATCH_CMD_EdgeFlagv,
   DISPATCH_CMD_End,
   DISPATCH_CMD_Indexd,
   DISPATCH_CMD_Indexdv,
   DISPATCH_CMD_Indexf,
   DISPATCH_CMD_Indexfv,
   DISPATCH_CMD_Indexi,
   DISPATCH_CMD_Indexiv,
   DISPATCH_CMD_Indexs,
   DISPATCH_CMD_Indexsv,
   DISPATCH_CMD_Normal3b,
   DISPATCH_CMD_Normal3bv,
   DISPATCH_CMD_Normal3d,
   DISPATCH_CMD_Normal3dv,
   DISPATCH_CMD_Normal3f,
   DISPATCH_CMD_Normal3fv,
   DISPATCH_CMD_Normal3i,
   DISPATCH_CMD_Normal3iv,
   DISPATCH_CMD_Normal3s,
   DISPATCH_CMD_Normal3sv,
   DISPATCH_CMD_RasterPos2d,
   DISPATCH_CMD_RasterPos2dv,
   DISPATCH_CMD_RasterPos2f,
   DISPATCH_CMD_RasterPos2fv,
   DISPATCH_CMD_RasterPos2i,
   DISPATCH_CMD_RasterPos2iv,
   DISPATCH_CMD_RasterPos2s,
   DISPATCH_CMD_RasterPos2sv,
   DISPATCH_CMD_RasterPos3d,
   DISPATCH_CMD_RasterPos3dv,
   DISPATCH_CMD_RasterPos3f,
   DISPATCH_CMD_RasterPos3fv,
   DISPATCH_CMD_RasterPos3i,
   DISPATCH_CMD_RasterPos3iv,
   DISPATCH_CMD_RasterPos3s,
   DISPATCH_CMD_RasterPos3sv,
   DISPATCH_CMD_RasterPos4d,
   DISPATCH_CMD_RasterPos4dv,
   DISPATCH_CMD_RasterPos4f,
   DISPATCH_CMD_RasterPos4fv,
   DISPATCH_CMD_RasterPos4i,
   DISPATCH_CMD_RasterPos4iv,
   DISPATCH_CMD_RasterPos4s,
   DISPATCH_CMD_RasterPos4sv,
   DISPATCH_CMD_Rectd,
   DISPATCH_CMD_Rectdv,
   DISPATCH_CMD_Rectf,
   DISPATCH_CMD_Rectfv,
   DISPATCH_CMD_Recti,
   DISPATCH_CMD_Rectiv,
   DISPATCH_CMD_Rects,
   DISPATCH_CMD_Rectsv,
   DISPATCH_CMD_TexCoord1d,
   DISPATCH_CMD_TexCoord1dv,
   DISPATCH_CMD_TexCoord1f,
   DISPATCH_CMD_TexCoord1fv,
   DISPATCH_CMD_TexCoord1i,
   DISPATCH_CMD_TexCoord1iv,
   DISPATCH_CMD_TexCoord1s,
   DISPATCH_CMD_TexCoord1sv,
   DISPATCH_CMD_TexCoord2d,
   DISPATCH_CMD_TexCoord2dv,
   DISPATCH_CMD_TexCoord2f,
   DISPATCH_CMD_TexCoord2fv,
   DISPATCH_CMD_TexCoord2i,
   DISPATCH_CMD_TexCoord2iv,
   DISPATCH_CMD_TexCoord2s,
   DISPATCH_CMD_TexCoord2sv,
   DISPATCH_CMD_TexCoord3d,
   DISPATCH_CMD_TexCoord3dv,
   DISPATCH_CMD_TexCoord3f,
   DISPATCH_CMD_TexCoord3fv,
   DISPATCH_CMD_TexCoord3i,
   DISPATCH_CMD_TexCoord3iv,
   DISPATCH_CMD_TexCoord3s,
   DISPATCH_CMD_TexCoord3sv,
   DISPATCH_CMD_TexCoord4d,
   DISPATCH_CMD_TexCoord4dv,
   DISPATCH_CMD_TexCoord4f,
   DISPATCH_CMD_TexCoord4fv,
   DISPATCH_CMD_TexCoord4i,
   DISPATCH_CMD_TexCoord4iv,
   DISPATCH_CMD_TexCoord4s,
   DISPATCH_CMD_TexCoord4sv,
   DISPATCH_CMD_Vertex2d,
   DISPATCH_CMD_Vertex2dv,
   DISPATCH_CMD_Vertex2f,
   DISPATCH_CMD_Vertex2fv,
   DISPATCH_CMD_Vertex2i,
   DISPATCH_CMD_Vertex2iv,
   DISPATCH_CMD_Vertex2s,
   DISPATCH_CMD_Vertex2sv,
   DISPATCH_CMD_Vertex3d,
   DISPATCH_CMD_Vertex3dv,
   DISPATCH_CMD_Vertex3f,
   DISPATCH_CMD_Vertex3fv,
   DISPATCH_CMD_Vertex3i,
   DISPATCH_CMD_Vertex3iv,
   DISPATCH_CMD_Vertex3s,
   DISPATCH_CMD_Vertex3sv,
   DISPATCH_CMD_Vertex4d,
   DISPATCH_CMD_Vertex4dv,
   DISPATCH_CMD_Vertex4f,
   DISPATCH_CMD_Vertex4fv,
   DISPATCH_CMD_Vertex4i,
   DISPATCH_CMD_Vertex4iv,
   DISPATCH_CMD_Vertex4s,
   DISPATCH_CMD_Vertex4sv,
   DISPATCH_CMD_ClipPlane,
   DISPATCH_CMD_ColorMaterial,
   DISPATCH_CMD_CullFace,
   DISPATCH_CMD_Fogf,
   DISPATCH_CMD_Fogfv,
   DISPATCH_CMD_Fogi,
   DISPATCH_CMD_Fogiv,
   DISPATCH_CMD_FrontFace,
   DISPATCH_CMD_Hint,
   DISPATCH_CMD_Lightf,
   DISPATCH_CMD_Lightfv,
   DISPATCH_CMD_Lighti,
   DISPATCH_CMD_Lightiv,
   DISPATCH_CMD_LightModelf,
   DISPATCH_CMD_LightModelfv,
   DISPATCH_CMD_LightModeli,
   DISPATCH_CMD_LightModeliv,
   DISPATCH_CMD_LineStipple,
   DISPATCH_CMD_LineWidth,
   DISPATCH_CMD_Materialf,
   DISPATCH_CMD_Materialfv,
   DISPATCH_CMD_Materiali,
   DISPATCH_CMD_Materialiv,
   DISPATCH_CMD_PointSize,
   DISPATCH_CMD_PolygonMode,
   DISPATCH_CMD_PolygonStipple,
   DISPATCH_CMD_Scissor,
   DISPATCH_CMD_ShadeModel,
   DISPATCH_CMD_TexParameterf,
   DISPATCH_CMD_TexParameterfv,
   DISPATCH_CMD_TexParameteri,
   DISPATCH_CMD_TexParameteriv,
   DISPATCH_CMD_TexImage1D,
   DISPATCH_CMD_TexImage2D,
   DISPATCH_CMD_TexEnvf,
   DISPATCH_CMD_TexEnvfv,
   DISPATCH_CMD_TexEnvi,
   DISPATCH_CMD_TexEnviv,
   DISPATCH_CMD_TexGend,
   DISPATCH_CMD_TexGendv,
   DISPATCH_CMD_TexGenf,
   DISPATCH_CMD_TexGenfv,
   DISPATCH_CMD_TexGeni,
   DISPATCH_CMD_TexGeniv,
   DISPATCH_CMD_InitNames,
   DISPATCH_CMD_LoadName,
   DISPATCH_CMD_PassThrough,
   DISPATCH_CMD_PopName,
   DISPATCH_CMD_PushName,
   DISPATCH_CMD_DrawBuffer,
   DISPATCH_CMD_Clear,
   DISPATCH_CMD_ClearAccum,
   DISPATCH_CMD_ClearIndex,
   DISPATCH_CMD_ClearColor,
   DISPATCH_CMD_ClearStencil,
   DISPATCH_CMD_ClearDepth,
   DISPATCH_CMD_StencilMask,
   DISPATCH_CMD_ColorMask,
   DISPATCH_CMD_DepthMask,
   DISPATCH_CMD_IndexMask,
   DISPATCH_CMD_Accum,
   DISPATCH_CMD_Disable,
   DISPATCH_CMD_Enable,
   DISPATCH_CMD_Flush,
   DISPATCH_CMD_PopAttrib,
   DISPATCH_CMD_PushAttrib,
   DISPATCH_CMD_MapGrid1d,
   DISPATCH_CMD_MapGrid1f,
   DISPATCH_CMD_MapGrid2d,
   DISPATCH_CMD_MapGrid2f,
   DISPATCH_CMD_EvalCoord1d,
   DISPATCH_CMD_EvalCoord1dv,
   DISPATCH_CMD_EvalCoord1f,
   DISPATCH_CMD_EvalCoord1fv,
   DISPATCH_CMD_EvalCoord2d,
   DISPATCH_CMD_EvalCoord2dv,
   DISPATCH_CMD_EvalCoord2f,
   DISPATCH_CMD_EvalCoord2fv,
   DISPATCH_CMD_EvalMesh1,
   DISPATCH_CMD_EvalPoint1,
   DISPATCH_CMD_EvalMesh2,
   DISPATCH_CMD_EvalPoint2,
   DISPATCH_CMD_AlphaFunc,
   DISPATCH_CMD_BlendFunc,
   DISPATCH_CMD_LogicOp,
   DISPATCH_CMD_StencilFunc,
   DISPATCH_CMD_StencilOp,
   DISPATCH_CMD_DepthFunc,
   DISPATCH_CMD_PixelZoom,
   DISPATCH_CMD_PixelTransferf,
   DISPATCH_CMD_PixelTransferi,
   DISPATCH_CMD_PixelStoref,
   DISPATCH_CMD_PixelStorei,
   DISPATCH_CMD_PixelMapfv,
   DISPATCH_CMD_PixelMapuiv,
   DISPATCH_CMD_PixelMapusv,
   DISPATCH_CMD_ReadBuffer,
   DISPATCH_CMD_CopyPixels,
   DISPATCH_CMD_ReadPixels,
   DISPATCH_CMD_DrawPixels,
   DISPATCH_CMD_GetIntegerv,
   DISPATCH_CMD_GetPixelMapfv,
   DISPATCH_CMD_GetPixelMapuiv,
   DISPATCH_CMD_GetPixelMapusv,
   DISPATCH_CMD_GetPolygonStipple,
   DISPATCH_CMD_GetTexImage,
   DISPATCH_CMD_DepthRange,
   DISPATCH_CMD_Frustum,
   DISPATCH_CMD_LoadIdentity,
   DISPATCH_CMD_LoadMatrixf,
   DISPATCH_CMD_LoadMatrixd,
   DISPATCH_CMD_MatrixMode,
   DISPATCH_CMD_MultMatrixf,
   DISPATCH_CMD_MultMatrixd,
   DISPATCH_CMD_Ortho,
   DISPATCH_CMD_PopMatrix,
   DISPATCH_CMD_PushMatrix,
   DISPATCH_CMD_Rotated,
   DISPATCH_CMD_Rotatef,
   DISPATCH_CMD_Scaled,
   DISPATCH_CMD_Scalef,
   DISPATCH_CMD_Translated,
   DISPATCH_CMD_Translatef,
   DISPATCH_CMD_Viewport,
   DISPATCH_CMD_ColorPointer,
   DISPATCH_CMD_DisableClientState,
   DISPATCH_CMD_DrawArrays,
   DISPATCH_CMD_DrawElements,
   DISPATCH_CMD_EdgeFlagPointer,
   DISPATCH_CMD_EnableClientState,
   DISPATCH_CMD_IndexPointer,
   DISPATCH_CMD_InterleavedArrays,
   DISPATCH_CMD_NormalPointer,
   DISPATCH_CMD_TexCoordPointer,
   DISPATCH_CMD_VertexPointer,
   DISPATCH_CMD_PolygonOffset,
   DISPATCH_CMD_CopyTexImage1D,
   DISPATCH_CMD_CopyTexImage2D,
   DISPATCH_CMD_CopyTexSubImage1D,
   DISPATCH_CMD_CopyTexSubImage2D,
   DISPATCH_CMD_TexSubImage1D,
   DISPATCH_CMD_TexSubImage2D,
   DISPATCH_CMD_BindTexture,
   DISPATCH_CMD_DeleteTextures,
   DISPATCH_CMD_PrioritizeTextures,
   DISPATCH_CMD_Indexub,
   DISPATCH_CMD_Indexubv,
   DISPATCH_CMD_PopClientAttrib,
   DISPATCH_CMD_PushClientAttrib,
   DISPATCH_CMD_BlendColor,
   DISPATCH_CMD_BlendEquation,
   DISPATCH_CMD_DrawRangeElements,
   DISPATCH_CMD_TexImage3D,
   DISPATCH_CMD_TexSubImage3D,
   DISPATCH_CMD_CopyTexSubImage3D,
   DISPATCH_CMD_ActiveTexture,
   DISPATCH_CMD_ClientActiveTexture,
   DISPATCH_CMD_MultiTexCoord1d,
   DISPATCH_CMD_MultiTexCoord1dv,
   DISPATCH_CMD_MultiTexCoord1fARB,
   DISPATCH_CMD_MultiTexCoord1fvARB,
   DISPATCH_CMD_MultiTexCoord1i,
   DISPATCH_CMD_MultiTexCoord1iv,
   DISPATCH_CMD_MultiTexCoord1s,
   DISPATCH_CMD_MultiTexCoord1sv,
   DISPATCH_CMD_MultiTexCoord2d,
   DISPATCH_CMD_MultiTexCoord2dv,
   DISPATCH_CMD_MultiTexCoord2fARB,
   DISPATCH_CMD_MultiTexCoord2fvARB,
   DISPATCH_CMD_MultiTexCoord2i,
   DISPATCH_CMD_MultiTexCoord2iv,
   DISPATCH_CMD_MultiTexCoord2s,
   DISPATCH_CMD_MultiTexCoord2sv,
   DISPATCH_CMD_MultiTexCoord3d,
   DISPATCH_CMD_MultiTexCoord3dv,
   DISPATCH_CMD_MultiTexCoord3fARB,
   DISPATCH_CMD_MultiTexCoord3fvARB,
   DISPATCH_CMD_MultiTexCoord3i,
   DISPATCH_CMD_MultiTexCoord3iv,
   DISPATCH_CMD_MultiTexCoord3s,
   DISPATCH_CMD_MultiTexCoord3sv,
   DISPATCH_CMD_MultiTexCoord4d,
   DISPATCH_CMD_MultiTexCoord4dv,
   DISPATCH_CMD_MultiTexCoord4fARB,
   DISPATCH_CMD_MultiTexCoord4fvARB,
   DISPATCH_CMD_MultiTexCoord4i,
   DISPATCH_CMD_MultiTexCoord4iv,
   DISPATCH_CMD_MultiTexCoord4s,
   DISPATCH_CMD_MultiTexCoord4sv,
   DISPATCH_CMD_LoadTransposeMatrixf,
   DISPATCH_CMD_LoadTransposeMatrixd,
   DISPATCH_CMD_MultTransposeMatrixf,
   DISPATCH_CMD_MultTransposeMatrixd,
   DISPATCH_CMD_SampleCoverage,
   DISPATCH_CMD_CompressedTexImage3D,
   DISPATCH_CMD_CompressedTexImage2D,
   DISPATCH_CMD_CompressedTexImage1D,
   DISPATCH_CMD_CompressedTexSubImage3D,
   DISPATCH_CMD_CompressedTexSubImage2D,
   DISPATCH_CMD_CompressedTexSubImage1D,
   DISPATCH_CMD_GetCompressedTexImage,
   DISPATCH_CMD_BlendFuncSeparate,
   DISPATCH_CMD_FogCoordfEXT,
   DISPATCH_CMD_FogCoordfvEXT,
   DISPATCH_CMD_FogCoordd,
   DISPATCH_CMD_FogCoorddv,
   DISPATCH_CMD_FogCoordPointer,
   DISPATCH_CMD_MultiDrawArrays,
   DISPATCH_CMD_MultiDrawElements,
   DISPATCH_CMD_PointParameterf,
   DISPATCH_CMD_PointParameterfv,
   DISPATCH_CMD_PointParameteri,
   DISPATCH_CMD_PointParameteriv,
   DISPATCH_CMD_SecondaryColor3b,
   DISPATCH_CMD_SecondaryColor3bv,
   DISPATCH_CMD_SecondaryColor3d,
   DISPATCH_CMD_SecondaryColor3dv,
   DISPATCH_CMD_SecondaryColor3fEXT,
   DISPATCH_CMD_SecondaryColor3fvEXT,
   DISPATCH_CMD_SecondaryColor3i,
   DISPATCH_CMD_SecondaryColor3iv,
   DISPATCH_CMD_SecondaryColor3s,
   DISPATCH_CMD_SecondaryColor3sv,
   DISPATCH_CMD_SecondaryColor3ub,
   DISPATCH_CMD_SecondaryColor3ubv,
   DISPATCH_CMD_SecondaryColor3ui,
   DISPATCH_CMD_SecondaryColor3uiv,
   DISPATCH_CMD_SecondaryColor3us,
   DISPATCH_CMD_SecondaryColor3usv,
   DISPATCH_CMD_SecondaryColorPointer,
   DISPATCH_CMD_WindowPos2d,
   DISPATCH_CMD_WindowPos2dv,
   DISPATCH_CMD_WindowPos2f,
   DISPATCH_CMD_WindowPos2fv,
   DISPATCH_CMD_WindowPos2i,
   DISPATCH_CMD_WindowPos2iv,
   DISPATCH_CMD_WindowPos2s,
   DISPATCH_CMD_WindowPos2sv,
   DISPATCH_CMD_WindowPos3d,
   DISPATCH_CMD_WindowPos3dv,
   DISPATCH_CMD_WindowPos3f,
   DISPATCH_CMD_WindowPos3fv,
   DISPATCH_CMD_WindowPos3i,
   DISPATCH_CMD_WindowPos3iv,
   DISPATCH_CMD_WindowPos3s,
   DISPATCH_CMD_WindowPos3sv,
   DISPATCH_CMD_BindBuffer,
   DISPATCH_CMD_BufferData,
   DISPATCH_CMD_BufferSubData,
   DISPATCH_CMD_DeleteBuffers,
   DISPATCH_CMD_UnmapBuffer,
   DISPATCH_CMD_DeleteQueries,
   DISPATCH_CMD_BeginQuery,
   DISPATCH_CMD_EndQuery,
   DISPATCH_CMD_BlendEquationSeparate,
   DISPATCH_CMD_DrawBuffers,
   DISPATCH_CMD_StencilFuncSeparate,
   DISPATCH_CMD_StencilOpSeparate,
   DISPATCH_CMD_StencilMaskSeparate,
   DISPATCH_CMD_AttachShader,
   DISPATCH_CMD_BindAttribLocation,
   DISPATCH_CMD_CompileShader,
   DISPATCH_CMD_DeleteProgram,
   DISPATCH_CMD_DeleteShader,
   DISPATCH_CMD_DetachShader,
   DISPATCH_CMD_DisableVertexAttribArray,
   DISPATCH_CMD_EnableVertexAttribArray,
   DISPATCH_CMD_GetActiveUniform,
   DISPATCH_CMD_GetUniformLocation,
   DISPATCH_CMD_LinkProgram,
   DISPATCH_CMD_UseProgram,
   DISPATCH_CMD_Uniform1f,
   DISPATCH_CMD_Uniform2f,
   DISPATCH_CMD_Uniform3f,
   DISPATCH_CMD_Uniform4f,
   DISPATCH_CMD_Uniform1i,
   DISPATCH_CMD_Uniform2i,
   DISPATCH_CMD_Uniform3i,
   DISPATCH_CMD_Uniform4i,
   DISPATCH_CMD_Uniform1fv,
   DISPATCH_CMD_Uniform2fv,
   DISPATCH_CMD_Uniform3fv,
   DISPATCH_CMD_Uniform4fv,
   DISPATCH_CMD_Uniform1iv,
   DISPATCH_CMD_Uniform2iv,
   DISPATCH_CMD_Uniform3iv,
   DISPATCH_CMD_Uniform4iv,
   DISPATCH_CMD_UniformMatrix2fv,
   DISPATCH_CMD_UniformMatrix3fv,
   DISPATCH_CMD_UniformMatrix4fv,
   DISPATCH_CMD_ValidateProgram,
   DISPATCH_CMD_VertexAttrib1d,
   DISPATCH_CMD_VertexAttrib1dv,
   DISPATCH_CMD_VertexAttrib1fARB,
   DISPATCH_CMD_VertexAttrib1fvARB,
   DISPATCH_CMD_VertexAttrib1s,
   DISPATCH_CMD_VertexAttrib1sv,
   DISPATCH_CMD_VertexAttrib2d,
   DISPATCH_CMD_VertexAttrib2dv,
   DISPATCH_CMD_VertexAttrib2fARB,
   DISPATCH_CMD_VertexAttrib2fvARB,
   DISPATCH_CMD_VertexAttrib2s,
   DISPATCH_CMD_VertexAttrib2sv,
   DISPATCH_CMD_VertexAttrib3d,
   DISPATCH_CMD_VertexAttrib3dv,
   DISPATCH_CMD_VertexAttrib3fARB,
   DISPATCH_CMD_VertexAttrib3fvARB,
   DISPATCH_CMD_VertexAttrib3s,
   DISPATCH_CMD_VertexAttrib3sv,
   DISPATCH_CMD_VertexAttrib4Nbv,
   DISPATCH_CMD_VertexAttrib4Niv,
   DISPATCH_CMD_VertexAttrib4Nsv,
   DISPATCH_CMD_VertexAttrib4Nub,
   DISPATCH_CMD_VertexAttrib4Nubv,
   DISPATCH_CMD_VertexAttrib4Nuiv,
   DISPATCH_CMD_VertexAttrib4Nusv,
   DISPATCH_CMD_VertexAttrib4bv,
   DISPATCH_CMD_VertexAttrib4d,
   DISPATCH_CMD_VertexAttrib4dv,
   DISPATCH_CMD_VertexAttrib4fARB,
   DISPATCH_CMD_VertexAttrib4fvARB,
   DISPATCH_CMD_VertexAttrib4iv,
   DISPATCH_CMD_VertexAttrib4s,
   DISPATCH_CMD_VertexAttrib4sv,
   DISPATCH_CMD_VertexAttrib4ubv,
   DISPATCH_CMD_VertexAttrib4uiv,
   DISPATCH_CMD_VertexAttrib4usv,
   DISPATCH_CMD_VertexAttribPointer,
   DISPATCH_CMD_UniformMatrix2x3fv,
   DISPATCH_CMD_UniformMatrix3x2fv,
   DISPATCH_CMD_UniformMatrix2x4fv,
   DISPATCH_CMD_UniformMatrix4x2fv,
   DISPATCH_CMD_UniformMatrix3x4fv,
   DISPATCH_CMD_UniformMatrix4x3fv,
   DISPATCH_CMD_ProgramStringARB,
   DISPATCH_CMD_BindProgramARB,
   DISPATCH_CMD_DeleteProgramsARB,
   DISPATCH_CMD_ProgramEnvParameter4dARB,
   DISPATCH_CMD_ProgramEnvParameter4dvARB,
   DISPATCH_CMD_ProgramEnvParameter4fARB,
   DISPATCH_CMD_ProgramEnvParameter4fvARB,
   DISPATCH_CMD_ProgramLocalParameter4dARB,
   DISPATCH_CMD_ProgramLocalParameter4dvARB,
   DISPATCH_CMD_ProgramLocalParameter4fARB,
   DISPATCH_CMD_ProgramLocalParameter4fvARB,
   DISPATCH_CMD_DeleteObjectARB,
   DISPATCH_CMD_DetachObjectARB,
   DISPATCH_CMD_AttachObjectARB,
   DISPATCH_CMD_ClampColor,
   DISPATCH_CMD_DrawArraysInstanced,
   DISPATCH_CMD_DrawElementsInstanced,
   DISPATCH_CMD_BindRenderbuffer,
   DISPATCH_CMD_DeleteRenderbuffers,
   DISPATCH_CMD_RenderbufferStorage,
   DISPATCH_CMD_RenderbufferStorageMultisample,
   DISPATCH_CMD_BindFramebuffer,
   DISPATCH_CMD_DeleteFramebuffers,
   DISPATCH_CMD_FramebufferTexture1D,
   DISPATCH_CMD_FramebufferTexture2D,
   DISPATCH_CMD_FramebufferTexture3D,
   DISPATCH_CMD_FramebufferTextureLayer,
   DISPATCH_CMD_FramebufferRenderbuffer,
   DISPATCH_CMD_BlitFramebuffer,
   DISPATCH_CMD_GenerateMipmap,
   DISPATCH_CMD_VertexAttribDivisor,
   DISPATCH_CMD_VertexArrayVertexAttribDivisorEXT,
   DISPATCH_CMD_FlushMappedBufferRange,
   DISPATCH_CMD_TexBuffer,
   DISPATCH_CMD_BindVertexArray,
   DISPATCH_CMD_DeleteVertexArrays,
   DISPATCH_CMD_UniformBlockBinding,
   DISPATCH_CMD_CopyBufferSubData,
   DISPATCH_CMD_DrawElementsBaseVertex,
   DISPATCH_CMD_DrawRangeElementsBaseVertex,
   DISPATCH_CMD_MultiDrawElementsBaseVertex,
   DISPATCH_CMD_DrawElementsInstancedBaseVertex,
   DISPATCH_CMD_DeleteSync,
   DISPATCH_CMD_WaitSync,
   DISPATCH_CMD_TexImage2DMultisample,
   DISPATCH_CMD_TexImage3DMultisample,
   DISPATCH_CMD_SampleMaski,
   DISPATCH_CMD_BlendEquationiARB,
   DISPATCH_CMD_BlendEquationSeparateiARB,
   DISPATCH_CMD_BlendFunciARB,
   DISPATCH_CMD_BlendFuncSeparateiARB,
   DISPATCH_CMD_MinSampleShading,
   DISPATCH_CMD_NamedStringARB,
   DISPATCH_CMD_DeleteNamedStringARB,
   DISPATCH_CMD_BindFragDataLocationIndexed,
   DISPATCH_CMD_DeleteSamplers,
   DISPATCH_CMD_BindSampler,
   DISPATCH_CMD_SamplerParameteri,
   DISPATCH_CMD_SamplerParameterf,
   DISPATCH_CMD_SamplerParameteriv,
   DISPATCH_CMD_SamplerParameterfv,
   DISPATCH_CMD_SamplerParameterIiv,
   DISPATCH_CMD_SamplerParameterIuiv,
   DISPATCH_CMD_QueryCounter,
   DISPATCH_CMD_VertexP2ui,
   DISPATCH_CMD_VertexP3ui,
   DISPATCH_CMD_VertexP4ui,
   DISPATCH_CMD_VertexP2uiv,
   DISPATCH_CMD_VertexP3uiv,
   DISPATCH_CMD_VertexP4uiv,
   DISPATCH_CMD_TexCoordP1ui,
   DISPATCH_CMD_TexCoordP2ui,
   DISPATCH_CMD_TexCoordP3ui,
   DISPATCH_CMD_TexCoordP4ui,
   DISPATCH_CMD_TexCoordP1uiv,
   DISPATCH_CMD_TexCoordP2uiv,
   DISPATCH_CMD_TexCoordP3uiv,
   DISPATCH_CMD_TexCoordP4uiv,
   DISPATCH_CMD_MultiTexCoordP1ui,
   DISPATCH_CMD_MultiTexCoordP2ui,
   DISPATCH_CMD_MultiTexCoordP3ui,
   DISPATCH_CMD_MultiTexCoordP4ui,
   DISPATCH_CMD_MultiTexCoordP1uiv,
   DISPATCH_CMD_MultiTexCoordP2uiv,
   DISPATCH_CMD_MultiTexCoordP3uiv,
   DISPATCH_CMD_MultiTexCoordP4uiv,
   DISPATCH_CMD_NormalP3ui,
   DISPATCH_CMD_NormalP3uiv,
   DISPATCH_CMD_ColorP3ui,
   DISPATCH_CMD_ColorP4ui,
   DISPATCH_CMD_ColorP3uiv,
   DISPATCH_CMD_ColorP4uiv,
   DISPATCH_CMD_SecondaryColorP3ui,
   DISPATCH_CMD_SecondaryColorP3uiv,
   DISPATCH_CMD_VertexAttribP1ui,
   DISPATCH_CMD_VertexAttribP2ui,
   DISPATCH_CMD_VertexAttribP3ui,
   DISPATCH_CMD_VertexAttribP4ui,
   DISPATCH_CMD_VertexAttribP1uiv,
   DISPATCH_CMD_VertexAttribP2uiv,
   DISPATCH_CMD_VertexAttribP3uiv,
   DISPATCH_CMD_VertexAttribP4uiv,
   DISPATCH_CMD_UniformSubroutinesuiv,
   DISPATCH_CMD_PatchParameteri,
   DISPATCH_CMD_PatchParameterfv,
   DISPATCH_CMD_DrawArraysIndirect,
   DISPATCH_CMD_DrawElementsIndirect,
   DISPATCH_CMD_MultiDrawArraysIndirect,
   DISPATCH_CMD_MultiDrawElementsIndirect,
   DISPATCH_CMD_Uniform1d,
   DISPATCH_CMD_Uniform2d,
   DISPATCH_CMD_Uniform3d,
   DISPATCH_CMD_Uniform4d,
   DISPATCH_CMD_Uniform1dv,
   DISPATCH_CMD_Uniform2dv,
   DISPATCH_CMD_Uniform3dv,
   DISPATCH_CMD_Uniform4dv,
   DISPATCH_CMD_UniformMatrix2dv,
   DISPATCH_CMD_UniformMatrix3dv,
   DISPATCH_CMD_UniformMatrix4dv,
   DISPATCH_CMD_UniformMatrix2x3dv,
   DISPATCH_CMD_UniformMatrix2x4dv,
   DISPATCH_CMD_UniformMatrix3x2dv,
   DISPATCH_CMD_UniformMatrix3x4dv,
   DISPATCH_CMD_UniformMatrix4x2dv,
   DISPATCH_CMD_UniformMatrix4x3dv,
   DISPATCH_CMD_ProgramUniform1d,
   DISPATCH_CMD_ProgramUniform2d,
   DISPATCH_CMD_ProgramUniform3d,
   DISPATCH_CMD_ProgramUniform4d,
   DISPATCH_CMD_ProgramUniform1dv,
   DISPATCH_CMD_ProgramUniform2dv,
   DISPATCH_CMD_ProgramUniform3dv,
   DISPATCH_CMD_ProgramUniform4dv,
   DISPATCH_CMD_ProgramUniformMatrix2dv,
   DISPATCH_CMD_ProgramUniformMatrix3dv,
   DISPATCH_CMD_ProgramUniformMatrix4dv,
   DISPATCH_CMD_ProgramUniformMatrix2x3dv,
   DISPATCH_CMD_ProgramUniformMatrix2x4dv,
   DISPATCH_CMD_ProgramUniformMatrix3x2dv,
   DISPATCH_CMD_ProgramUniformMatrix3x4dv,
   DISPATCH_CMD_ProgramUniformMatrix4x2dv,
   DISPATCH_CMD_ProgramUniformMatrix4x3dv,
   DISPATCH_CMD_DrawTransformFeedbackStream,
   DISPATCH_CMD_BeginQueryIndexed,
   DISPATCH_CMD_EndQueryIndexed,
   DISPATCH_CMD_UseProgramStages,
   DISPATCH_CMD_ActiveShaderProgram,
   DISPATCH_CMD_BindProgramPipeline,
   DISPATCH_CMD_DeleteProgramPipelines,
   DISPATCH_CMD_ProgramUniform1i,
   DISPATCH_CMD_ProgramUniform2i,
   DISPATCH_CMD_ProgramUniform3i,
   DISPATCH_CMD_ProgramUniform4i,
   DISPATCH_CMD_ProgramUniform1ui,
   DISPATCH_CMD_ProgramUniform2ui,
   DISPATCH_CMD_ProgramUniform3ui,
   DISPATCH_CMD_ProgramUniform4ui,
   DISPATCH_CMD_ProgramUniform1f,
   DISPATCH_CMD_ProgramUniform2f,
   DISPATCH_CMD_ProgramUniform3f,
   DISPATCH_CMD_ProgramUniform4f,
   DISPATCH_CMD_ProgramUniform1iv,
   DISPATCH_CMD_ProgramUniform2iv,
   DISPATCH_CMD_ProgramUniform3iv,
   DISPATCH_CMD_ProgramUniform4iv,
   DISPATCH_CMD_ProgramUniform1uiv,
   DISPATCH_CMD_ProgramUniform2uiv,
   DISPATCH_CMD_ProgramUniform3uiv,
   DISPATCH_CMD_ProgramUniform4uiv,
   DISPATCH_CMD_ProgramUniform1fv,
   DISPATCH_CMD_ProgramUniform2fv,
   DISPATCH_CMD_ProgramUniform3fv,
   DISPATCH_CMD_ProgramUniform4fv,
   DISPATCH_CMD_ProgramUniformMatrix2fv,
   DISPATCH_CMD_ProgramUniformMatrix3fv,
   DISPATCH_CMD_ProgramUniformMatrix4fv,
   DISPATCH_CMD_ProgramUniformMatrix2x3fv,
   DISPATCH_CMD_ProgramUniformMatrix3x2fv,
   DISPATCH_CMD_ProgramUniformMatrix2x4fv,
   DISPATCH_CMD_ProgramUniformMatrix4x2fv,
   DISPATCH_CMD_ProgramUniformMatrix3x4fv,
   DISPATCH_CMD_ProgramUniformMatrix4x3fv,
   DISPATCH_CMD_ValidateProgramPipeline,
   DISPATCH_CMD_VertexAttribL1d,
   DISPATCH_CMD_VertexAttribL2d,
   DISPATCH_CMD_VertexAttribL3d,
   DISPATCH_CMD_VertexAttribL4d,
   DISPATCH_CMD_VertexAttribL1dv,
   DISPATCH_CMD_VertexAttribL2dv,
   DISPATCH_CMD_VertexAttribL3dv,
   DISPATCH_CMD_VertexAttribL4dv,
   DISPATCH_CMD_VertexAttribLPointer,
   DISPATCH_CMD_VertexArrayVertexAttribLOffsetEXT,
   DISPATCH_CMD_ReleaseShaderCompiler,
   DISPATCH_CMD_ShaderBinary,
   DISPATCH_CMD_ClearDepthf,
   DISPATCH_CMD_DepthRangef,
   DISPATCH_CMD_ProgramBinary,
   DISPATCH_CMD_ProgramParameteri,
   DISPATCH_CMD_DebugMessageControl,
   DISPATCH_CMD_DebugMessageInsert,
   DISPATCH_CMD_GetnPolygonStippleARB,
   DISPATCH_CMD_GetnTexImageARB,
   DISPATCH_CMD_ReadnPixelsARB,
   DISPATCH_CMD_GetnCompressedTexImageARB,
   DISPATCH_CMD_DrawArraysInstancedBaseInstance,
   DISPATCH_CMD_DrawElementsInstancedBaseInstance,
   DISPATCH_CMD_DrawElementsInstancedBaseVertexBaseInstance,
   DISPATCH_CMD_DrawTransformFeedbackInstanced,
   DISPATCH_CMD_DrawTransformFeedbackStreamInstanced,
   DISPATCH_CMD_BindImageTexture,
   DISPATCH_CMD_MemoryBarrier,
   DISPATCH_CMD_TexStorage1D,
   DISPATCH_CMD_TexStorage2D,
   DISPATCH_CMD_TexStorage3D,
   DISPATCH_CMD_TextureStorage1DEXT,
   DISPATCH_CMD_TextureStorage2DEXT,
   DISPATCH_CMD_TextureStorage3DEXT,
   DISPATCH_CMD_PushDebugGroup,
   DISPATCH_CMD_PopDebugGroup,
   DISPATCH_CMD_ObjectLabel,
   DISPATCH_CMD_DispatchCompute,
   DISPATCH_CMD_DispatchComputeIndirect,
   DISPATCH_CMD_CopyImageSubData,
   DISPATCH_CMD_TextureView,
   DISPATCH_CMD_BindVertexBuffer,
   DISPATCH_CMD_VertexAttribFormat,
   DISPATCH_CMD_VertexAttribIFormat,
   DISPATCH_CMD_VertexAttribLFormat,
   DISPATCH_CMD_VertexAttribBinding,
   DISPATCH_CMD_VertexBindingDivisor,
   DISPATCH_CMD_VertexArrayBindVertexBufferEXT,
   DISPATCH_CMD_VertexArrayVertexAttribFormatEXT,
   DISPATCH_CMD_VertexArrayVertexAttribIFormatEXT,
   DISPATCH_CMD_VertexArrayVertexAttribLFormatEXT,
   DISPATCH_CMD_VertexArrayVertexAttribBindingEXT,
   DISPATCH_CMD_VertexArrayVertexBindingDivisorEXT,
   DISPATCH_CMD_FramebufferParameteri,
   DISPATCH_CMD_NamedFramebufferParameteriEXT,
   DISPATCH_CMD_InvalidateTexSubImage,
   DISPATCH_CMD_InvalidateTexImage,
   DISPATCH_CMD_InvalidateBufferSubData,
   DISPATCH_CMD_InvalidateBufferData,
   DISPATCH_CMD_InvalidateSubFramebuffer,
   DISPATCH_CMD_InvalidateFramebuffer,
   DISPATCH_CMD_ShaderStorageBlockBinding,
   DISPATCH_CMD_TexBufferRange,
   DISPATCH_CMD_TextureBufferRangeEXT,
   DISPATCH_CMD_TexStorage2DMultisample,
   DISPATCH_CMD_TexStorage3DMultisample,
   DISPATCH_CMD_TextureStorage2DMultisampleEXT,
   DISPATCH_CMD_TextureStorage3DMultisampleEXT,
   DISPATCH_CMD_BindBuffersBase,
   DISPATCH_CMD_BindBuffersRange,
   DISPATCH_CMD_BindTextures,
   DISPATCH_CMD_BindSamplers,
   DISPATCH_CMD_BindImageTextures,
   DISPATCH_CMD_BindVertexBuffers,
   DISPATCH_CMD_MakeTextureHandleResidentARB,
   DISPATCH_CMD_MakeTextureHandleNonResidentARB,
   DISPATCH_CMD_MakeImageHandleResidentARB,
   DISPATCH_CMD_MakeImageHandleNonResidentARB,
   DISPATCH_CMD_UniformHandleui64ARB,
   DISPATCH_CMD_UniformHandleui64vARB,
   DISPATCH_CMD_ProgramUniformHandleui64ARB,
   DISPATCH_CMD_ProgramUniformHandleui64vARB,
   DISPATCH_CMD_VertexAttribL1ui64ARB,
   DISPATCH_CMD_VertexAttribL1ui64vARB,
   DISPATCH_CMD_DispatchComputeGroupSizeARB,
   DISPATCH_CMD_MultiDrawArraysIndirectCountARB,
   DISPATCH_CMD_MultiDrawElementsIndirectCountARB,
   DISPATCH_CMD_TexPageCommitmentARB,
   DISPATCH_CMD_TexturePageCommitmentEXT,
   DISPATCH_CMD_ClipControl,
   DISPATCH_CMD_TransformFeedbackBufferBase,
   DISPATCH_CMD_TransformFeedbackBufferRange,
   DISPATCH_CMD_NamedBufferData,
   DISPATCH_CMD_NamedBufferSubData,
   DISPATCH_CMD_CopyNamedBufferSubData,
   DISPATCH_CMD_UnmapNamedBufferEXT,
   DISPATCH_CMD_FlushMappedNamedBufferRange,
   DISPATCH_CMD_NamedFramebufferRenderbuffer,
   DISPATCH_CMD_NamedFramebufferParameteri,
   DISPATCH_CMD_NamedFramebufferTexture,
   DISPATCH_CMD_NamedFramebufferTextureLayer,
   DISPATCH_CMD_NamedFramebufferDrawBuffer,
   DISPATCH_CMD_NamedFramebufferDrawBuffers,
   DISPATCH_CMD_NamedFramebufferReadBuffer,
   DISPATCH_CMD_InvalidateNamedFramebufferData,
   DISPATCH_CMD_InvalidateNamedFramebufferSubData,
   DISPATCH_CMD_ClearNamedFramebufferiv,
   DISPATCH_CMD_ClearNamedFramebufferuiv,
   DISPATCH_CMD_ClearNamedFramebufferfv,
   DISPATCH_CMD_ClearNamedFramebufferfi,
   DISPATCH_CMD_BlitNamedFramebuffer,
   DISPATCH_CMD_NamedRenderbufferStorage,
   DISPATCH_CMD_NamedRenderbufferStorageMultisample,
   DISPATCH_CMD_TextureBuffer,
   DISPATCH_CMD_TextureBufferRange,
   DISPATCH_CMD_TextureStorage1D,
   DISPATCH_CMD_TextureStorage2D,
   DISPATCH_CMD_TextureStorage3D,
   DISPATCH_CMD_TextureStorage2DMultisample,
   DISPATCH_CMD_TextureStorage3DMultisample,
   DISPATCH_CMD_TextureSubImage1D,
   DISPATCH_CMD_TextureSubImage2D,
   DISPATCH_CMD_TextureSubImage3D,
   DISPATCH_CMD_CompressedTextureSubImage1D,
   DISPATCH_CMD_CompressedTextureSubImage2D,
   DISPATCH_CMD_CompressedTextureSubImage3D,
   DISPATCH_CMD_CopyTextureSubImage1D,
   DISPATCH_CMD_CopyTextureSubImage2D,
   DISPATCH_CMD_CopyTextureSubImage3D,
   DISPATCH_CMD_TextureParameterf,
   DISPATCH_CMD_TextureParameterfv,
   DISPATCH_CMD_TextureParameteri,
   DISPATCH_CMD_TextureParameterIiv,
   DISPATCH_CMD_TextureParameterIuiv,
   DISPATCH_CMD_TextureParameteriv,
   DISPATCH_CMD_GenerateTextureMipmap,
   DISPATCH_CMD_BindTextureUnit,
   DISPATCH_CMD_GetTextureImage,
   DISPATCH_CMD_GetCompressedTextureImage,
   DISPATCH_CMD_DisableVertexArrayAttrib,
   DISPATCH_CMD_EnableVertexArrayAttrib,
   DISPATCH_CMD_VertexArrayElementBuffer,
   DISPATCH_CMD_VertexArrayVertexBuffer,
   DISPATCH_CMD_VertexArrayVertexBuffers,
   DISPATCH_CMD_VertexArrayAttribFormat,
   DISPATCH_CMD_VertexArrayAttribIFormat,
   DISPATCH_CMD_VertexArrayAttribLFormat,
   DISPATCH_CMD_VertexArrayAttribBinding,
   DISPATCH_CMD_VertexArrayBindingDivisor,
   DISPATCH_CMD_GetQueryBufferObjectiv,
   DISPATCH_CMD_GetQueryBufferObjectuiv,
   DISPATCH_CMD_GetQueryBufferObjecti64v,
   DISPATCH_CMD_GetQueryBufferObjectui64v,
   DISPATCH_CMD_GetTextureSubImage,
   DISPATCH_CMD_GetCompressedTextureSubImage,
   DISPATCH_CMD_TextureBarrierNV,
   DISPATCH_CMD_BufferPageCommitmentARB,
   DISPATCH_CMD_NamedBufferPageCommitmentEXT,
   DISPATCH_CMD_NamedBufferPageCommitmentARB,
   DISPATCH_CMD_PrimitiveBoundingBox,
   DISPATCH_CMD_BlendBarrier,
   DISPATCH_CMD_Uniform1i64ARB,
   DISPATCH_CMD_Uniform2i64ARB,
   DISPATCH_CMD_Uniform3i64ARB,
   DISPATCH_CMD_Uniform4i64ARB,
   DISPATCH_CMD_Uniform1i64vARB,
   DISPATCH_CMD_Uniform2i64vARB,
   DISPATCH_CMD_Uniform3i64vARB,
   DISPATCH_CMD_Uniform4i64vARB,
   DISPATCH_CMD_Uniform1ui64ARB,
   DISPATCH_CMD_Uniform2ui64ARB,
   DISPATCH_CMD_Uniform3ui64ARB,
   DISPATCH_CMD_Uniform4ui64ARB,
   DISPATCH_CMD_Uniform1ui64vARB,
   DISPATCH_CMD_Uniform2ui64vARB,
   DISPATCH_CMD_Uniform3ui64vARB,
   DISPATCH_CMD_Uniform4ui64vARB,
   DISPATCH_CMD_ProgramUniform1i64ARB,
   DISPATCH_CMD_ProgramUniform2i64ARB,
   DISPATCH_CMD_ProgramUniform3i64ARB,
   DISPATCH_CMD_ProgramUniform4i64ARB,
   DISPATCH_CMD_ProgramUniform1i64vARB,
   DISPATCH_CMD_ProgramUniform2i64vARB,
   DISPATCH_CMD_ProgramUniform3i64vARB,
   DISPATCH_CMD_ProgramUniform4i64vARB,
   DISPATCH_CMD_ProgramUniform1ui64ARB,
   DISPATCH_CMD_ProgramUniform2ui64ARB,
   DISPATCH_CMD_ProgramUniform3ui64ARB,
   DISPATCH_CMD_ProgramUniform4ui64ARB,
   DISPATCH_CMD_ProgramUniform1ui64vARB,
   DISPATCH_CMD_ProgramUniform2ui64vARB,
   DISPATCH_CMD_ProgramUniform3ui64vARB,
   DISPATCH_CMD_ProgramUniform4ui64vARB,
   DISPATCH_CMD_MaxShaderCompilerThreadsKHR,
   DISPATCH_CMD_SpecializeShaderARB,
   DISPATCH_CMD_ColorPointerEXT,
   DISPATCH_CMD_EdgeFlagPointerEXT,
   DISPATCH_CMD_IndexPointerEXT,
   DISPATCH_CMD_NormalPointerEXT,
   DISPATCH_CMD_TexCoordPointerEXT,
   DISPATCH_CMD_VertexPointerEXT,
   DISPATCH_CMD_LockArraysEXT,
   DISPATCH_CMD_UnlockArraysEXT,
   DISPATCH_CMD_ViewportArrayv,
   DISPATCH_CMD_ViewportIndexedf,
   DISPATCH_CMD_ViewportIndexedfv,
   DISPATCH_CMD_ScissorArrayv,
   DISPATCH_CMD_ScissorIndexed,
   DISPATCH_CMD_ScissorIndexedv,
   DISPATCH_CMD_DepthRangeArrayv,
   DISPATCH_CMD_DepthRangeIndexed,
   DISPATCH_CMD_FramebufferSampleLocationsfvARB,
   DISPATCH_CMD_NamedFramebufferSampleLocationsfvARB,
   DISPATCH_CMD_EvaluateDepthValuesARB,
   DISPATCH_CMD_WindowPos4dMESA,
   DISPATCH_CMD_WindowPos4dvMESA,
   DISPATCH_CMD_WindowPos4fMESA,
   DISPATCH_CMD_WindowPos4fvMESA,
   DISPATCH_CMD_WindowPos4iMESA,
   DISPATCH_CMD_WindowPos4ivMESA,
   DISPATCH_CMD_WindowPos4sMESA,
   DISPATCH_CMD_WindowPos4svMESA,
   DISPATCH_CMD_MultiModeDrawArraysIBM,
   DISPATCH_CMD_MultiModeDrawElementsIBM,
   DISPATCH_CMD_VertexAttrib1sNV,
   DISPATCH_CMD_VertexAttrib1svNV,
   DISPATCH_CMD_VertexAttrib2sNV,
   DISPATCH_CMD_VertexAttrib2svNV,
   DISPATCH_CMD_VertexAttrib3sNV,
   DISPATCH_CMD_VertexAttrib3svNV,
   DISPATCH_CMD_VertexAttrib4sNV,
   DISPATCH_CMD_VertexAttrib4svNV,
   DISPATCH_CMD_VertexAttrib1fNV,
   DISPATCH_CMD_VertexAttrib1fvNV,
   DISPATCH_CMD_VertexAttrib2fNV,
   DISPATCH_CMD_VertexAttrib2fvNV,
   DISPATCH_CMD_VertexAttrib3fNV,
   DISPATCH_CMD_VertexAttrib3fvNV,
   DISPATCH_CMD_VertexAttrib4fNV,
   DISPATCH_CMD_VertexAttrib4fvNV,
   DISPATCH_CMD_VertexAttrib1dNV,
   DISPATCH_CMD_VertexAttrib1dvNV,
   DISPATCH_CMD_VertexAttrib2dNV,
   DISPATCH_CMD_VertexAttrib2dvNV,
   DISPATCH_CMD_VertexAttrib3dNV,
   DISPATCH_CMD_VertexAttrib3dvNV,
   DISPATCH_CMD_VertexAttrib4dNV,
   DISPATCH_CMD_VertexAttrib4dvNV,
   DISPATCH_CMD_VertexAttrib4ubNV,
   DISPATCH_CMD_VertexAttrib4ubvNV,
   DISPATCH_CMD_VertexAttribs1svNV,
   DISPATCH_CMD_VertexAttribs2svNV,
   DISPATCH_CMD_VertexAttribs3svNV,
   DISPATCH_CMD_VertexAttribs4svNV,
   DISPATCH_CMD_VertexAttribs1fvNV,
   DISPATCH_CMD_VertexAttribs2fvNV,
   DISPATCH_CMD_VertexAttribs3fvNV,
   DISPATCH_CMD_VertexAttribs4fvNV,
   DISPATCH_CMD_VertexAttribs1dvNV,
   DISPATCH_CMD_VertexAttribs2dvNV,
   DISPATCH_CMD_VertexAttribs3dvNV,
   DISPATCH_CMD_VertexAttribs4dvNV,
   DISPATCH_CMD_VertexAttribs4ubvNV,
   DISPATCH_CMD_BindFragmentShaderATI,
   DISPATCH_CMD_DeleteFragmentShaderATI,
   DISPATCH_CMD_BeginFragmentShaderATI,
   DISPATCH_CMD_EndFragmentShaderATI,
   DISPATCH_CMD_PassTexCoordATI,
   DISPATCH_CMD_SampleMapATI,
   DISPATCH_CMD_ColorFragmentOp1ATI,
   DISPATCH_CMD_ColorFragmentOp2ATI,
   DISPATCH_CMD_ColorFragmentOp3ATI,
   DISPATCH_CMD_AlphaFragmentOp1ATI,
   DISPATCH_CMD_AlphaFragmentOp2ATI,
   DISPATCH_CMD_AlphaFragmentOp3ATI,
   DISPATCH_CMD_SetFragmentShaderConstantATI,
   DISPATCH_CMD_ActiveStencilFaceEXT,
   DISPATCH_CMD_DepthBoundsEXT,
   DISPATCH_CMD_BindRenderbufferEXT,
   DISPATCH_CMD_BindFramebufferEXT,
   DISPATCH_CMD_ProvokingVertex,
   DISPATCH_CMD_ColorMaski,
   DISPATCH_CMD_Enablei,
   DISPATCH_CMD_Disablei,
   DISPATCH_CMD_DeletePerfMonitorsAMD,
   DISPATCH_CMD_SelectPerfMonitorCountersAMD,
   DISPATCH_CMD_BeginPerfMonitorAMD,
   DISPATCH_CMD_EndPerfMonitorAMD,
   DISPATCH_CMD_CopyImageSubDataNV,
   DISPATCH_CMD_MatrixLoadfEXT,
   DISPATCH_CMD_MatrixLoaddEXT,
   DISPATCH_CMD_MatrixMultfEXT,
   DISPATCH_CMD_MatrixMultdEXT,
   DISPATCH_CMD_MatrixLoadIdentityEXT,
   DISPATCH_CMD_MatrixRotatefEXT,
   DISPATCH_CMD_MatrixRotatedEXT,
   DISPATCH_CMD_MatrixScalefEXT,
   DISPATCH_CMD_MatrixScaledEXT,
   DISPATCH_CMD_MatrixTranslatefEXT,
   DISPATCH_CMD_MatrixTranslatedEXT,
   DISPATCH_CMD_MatrixOrthoEXT,
   DISPATCH_CMD_MatrixFrustumEXT,
   DISPATCH_CMD_MatrixPushEXT,
   DISPATCH_CMD_MatrixPopEXT,
   DISPATCH_CMD_ClientAttribDefaultEXT,
   DISPATCH_CMD_PushClientAttribDefaultEXT,
   DISPATCH_CMD_TextureParameteriEXT,
   DISPATCH_CMD_TextureParameterivEXT,
   DISPATCH_CMD_TextureParameterfEXT,
   DISPATCH_CMD_TextureParameterfvEXT,
   DISPATCH_CMD_TextureImage1DEXT,
   DISPATCH_CMD_TextureImage2DEXT,
   DISPATCH_CMD_TextureImage3DEXT,
   DISPATCH_CMD_TextureSubImage1DEXT,
   DISPATCH_CMD_TextureSubImage2DEXT,
   DISPATCH_CMD_TextureSubImage3DEXT,
   DISPATCH_CMD_CopyTextureImage1DEXT,
   DISPATCH_CMD_CopyTextureImage2DEXT,
   DISPATCH_CMD_CopyTextureSubImage1DEXT,
   DISPATCH_CMD_CopyTextureSubImage2DEXT,
   DISPATCH_CMD_CopyTextureSubImage3DEXT,
   DISPATCH_CMD_GetTextureImageEXT,
   DISPATCH_CMD_BindMultiTextureEXT,
   DISPATCH_CMD_EnableClientStateiEXT,
   DISPATCH_CMD_DisableClientStateiEXT,
   DISPATCH_CMD_MultiTexEnviEXT,
   DISPATCH_CMD_MultiTexEnvivEXT,
   DISPATCH_CMD_MultiTexEnvfEXT,
   DISPATCH_CMD_MultiTexEnvfvEXT,
   DISPATCH_CMD_MultiTexParameteriEXT,
   DISPATCH_CMD_MultiTexParameterivEXT,
   DISPATCH_CMD_MultiTexParameterfEXT,
   DISPATCH_CMD_MultiTexParameterfvEXT,
   DISPATCH_CMD_GetMultiTexImageEXT,
   DISPATCH_CMD_MultiTexImage1DEXT,
   DISPATCH_CMD_MultiTexImage2DEXT,
   DISPATCH_CMD_MultiTexImage3DEXT,
   DISPATCH_CMD_MultiTexSubImage1DEXT,
   DISPATCH_CMD_MultiTexSubImage2DEXT,
   DISPATCH_CMD_MultiTexSubImage3DEXT,
   DISPATCH_CMD_CopyMultiTexImage1DEXT,
   DISPATCH_CMD_CopyMultiTexImage2DEXT,
   DISPATCH_CMD_CopyMultiTexSubImage1DEXT,
   DISPATCH_CMD_CopyMultiTexSubImage2DEXT,
   DISPATCH_CMD_CopyMultiTexSubImage3DEXT,
   DISPATCH_CMD_MultiTexGendEXT,
   DISPATCH_CMD_MultiTexGendvEXT,
   DISPATCH_CMD_MultiTexGenfEXT,
   DISPATCH_CMD_MultiTexGenfvEXT,
   DISPATCH_CMD_MultiTexGeniEXT,
   DISPATCH_CMD_MultiTexGenivEXT,
   DISPATCH_CMD_MultiTexCoordPointerEXT,
   DISPATCH_CMD_MatrixLoadTransposefEXT,
   DISPATCH_CMD_MatrixLoadTransposedEXT,
   DISPATCH_CMD_MatrixMultTransposefEXT,
   DISPATCH_CMD_MatrixMultTransposedEXT,
   DISPATCH_CMD_CompressedTextureImage1DEXT,
   DISPATCH_CMD_CompressedTextureImage2DEXT,
   DISPATCH_CMD_CompressedTextureImage3DEXT,
   DISPATCH_CMD_CompressedTextureSubImage1DEXT,
   DISPATCH_CMD_CompressedTextureSubImage2DEXT,
   DISPATCH_CMD_CompressedTextureSubImage3DEXT,
   DISPATCH_CMD_GetCompressedTextureImageEXT,
   DISPATCH_CMD_CompressedMultiTexImage1DEXT,
   DISPATCH_CMD_CompressedMultiTexImage2DEXT,
   DISPATCH_CMD_CompressedMultiTexImage3DEXT,
   DISPATCH_CMD_CompressedMultiTexSubImage1DEXT,
   DISPATCH_CMD_CompressedMultiTexSubImage2DEXT,
   DISPATCH_CMD_CompressedMultiTexSubImage3DEXT,
   DISPATCH_CMD_GetCompressedMultiTexImageEXT,
   DISPATCH_CMD_NamedBufferDataEXT,
   DISPATCH_CMD_NamedBufferSubDataEXT,
   DISPATCH_CMD_FlushMappedNamedBufferRangeEXT,
   DISPATCH_CMD_FramebufferDrawBufferEXT,
   DISPATCH_CMD_FramebufferDrawBuffersEXT,
   DISPATCH_CMD_FramebufferReadBufferEXT,
   DISPATCH_CMD_NamedFramebufferTexture1DEXT,
   DISPATCH_CMD_NamedFramebufferTexture2DEXT,
   DISPATCH_CMD_NamedFramebufferTexture3DEXT,
   DISPATCH_CMD_NamedFramebufferRenderbufferEXT,
   DISPATCH_CMD_NamedRenderbufferStorageEXT,
   DISPATCH_CMD_GenerateTextureMipmapEXT,
   DISPATCH_CMD_GenerateMultiTexMipmapEXT,
   DISPATCH_CMD_NamedRenderbufferStorageMultisampleEXT,
   DISPATCH_CMD_NamedCopyBufferSubDataEXT,
   DISPATCH_CMD_VertexArrayVertexOffsetEXT,
   DISPATCH_CMD_VertexArrayColorOffsetEXT,
   DISPATCH_CMD_VertexArrayEdgeFlagOffsetEXT,
   DISPATCH_CMD_VertexArrayIndexOffsetEXT,
   DISPATCH_CMD_VertexArrayNormalOffsetEXT,
   DISPATCH_CMD_VertexArrayTexCoordOffsetEXT,
   DISPATCH_CMD_VertexArrayMultiTexCoordOffsetEXT,
   DISPATCH_CMD_VertexArrayFogCoordOffsetEXT,
   DISPATCH_CMD_VertexArraySecondaryColorOffsetEXT,
   DISPATCH_CMD_VertexArrayVertexAttribOffsetEXT,
   DISPATCH_CMD_VertexArrayVertexAttribIOffsetEXT,
   DISPATCH_CMD_EnableVertexArrayEXT,
   DISPATCH_CMD_DisableVertexArrayEXT,
   DISPATCH_CMD_EnableVertexArrayAttribEXT,
   DISPATCH_CMD_DisableVertexArrayAttribEXT,
   DISPATCH_CMD_NamedProgramStringEXT,
   DISPATCH_CMD_NamedProgramLocalParameter4fEXT,
   DISPATCH_CMD_NamedProgramLocalParameter4fvEXT,
   DISPATCH_CMD_NamedProgramLocalParameter4dEXT,
   DISPATCH_CMD_NamedProgramLocalParameter4dvEXT,
   DISPATCH_CMD_TextureBufferEXT,
   DISPATCH_CMD_MultiTexBufferEXT,
   DISPATCH_CMD_TextureParameterIivEXT,
   DISPATCH_CMD_TextureParameterIuivEXT,
   DISPATCH_CMD_MultiTexParameterIivEXT,
   DISPATCH_CMD_MultiTexParameterIuivEXT,
   DISPATCH_CMD_NamedProgramLocalParameters4fvEXT,
   DISPATCH_CMD_BindImageTextureEXT,
   DISPATCH_CMD_SubpixelPrecisionBiasNV,
   DISPATCH_CMD_ConservativeRasterParameterfNV,
   DISPATCH_CMD_ConservativeRasterParameteriNV,
   DISPATCH_CMD_DeletePerfQueryINTEL,
   DISPATCH_CMD_BeginPerfQueryINTEL,
   DISPATCH_CMD_EndPerfQueryINTEL,
   DISPATCH_CMD_AlphaToCoverageDitherControlNV,
   DISPATCH_CMD_PolygonOffsetClampEXT,
   DISPATCH_CMD_WindowRectanglesEXT,
   DISPATCH_CMD_FramebufferFetchBarrierEXT,
   DISPATCH_CMD_RenderbufferStorageMultisampleAdvancedAMD,
   DISPATCH_CMD_NamedRenderbufferStorageMultisampleAdvancedAMD,
   DISPATCH_CMD_StencilFuncSeparateATI,
   DISPATCH_CMD_ProgramEnvParameters4fvEXT,
   DISPATCH_CMD_ProgramLocalParameters4fvEXT,
   DISPATCH_CMD_InternalBufferSubDataCopyMESA,
   DISPATCH_CMD_InternalSetError,
   DISPATCH_CMD_DrawArraysUserBuf,
   DISPATCH_CMD_DrawElementsUserBuf,
   DISPATCH_CMD_MultiDrawArraysUserBuf,
   DISPATCH_CMD_MultiDrawElementsUserBuf,
   DISPATCH_CMD_DrawArraysInstancedBaseInstanceDrawID,
   DISPATCH_CMD_DrawElementsInstancedBaseVertexBaseInstanceDrawID,
   DISPATCH_CMD_InternalInvalidateFramebufferAncillaryMESA,
   DISPATCH_CMD_ClearColorIiEXT,
   DISPATCH_CMD_ClearColorIuiEXT,
   DISPATCH_CMD_TexParameterIiv,
   DISPATCH_CMD_TexParameterIuiv,
   DISPATCH_CMD_VertexAttribI1iEXT,
   DISPATCH_CMD_VertexAttribI2iEXT,
   DISPATCH_CMD_VertexAttribI3iEXT,
   DISPATCH_CMD_VertexAttribI4iEXT,
   DISPATCH_CMD_VertexAttribI1uiEXT,
   DISPATCH_CMD_VertexAttribI2uiEXT,
   DISPATCH_CMD_VertexAttribI3uiEXT,
   DISPATCH_CMD_VertexAttribI4uiEXT,
   DISPATCH_CMD_VertexAttribI1iv,
   DISPATCH_CMD_VertexAttribI2ivEXT,
   DISPATCH_CMD_VertexAttribI3ivEXT,
   DISPATCH_CMD_VertexAttribI4ivEXT,
   DISPATCH_CMD_VertexAttribI1uiv,
   DISPATCH_CMD_VertexAttribI2uivEXT,
   DISPATCH_CMD_VertexAttribI3uivEXT,
   DISPATCH_CMD_VertexAttribI4uivEXT,
   DISPATCH_CMD_VertexAttribI4bv,
   DISPATCH_CMD_VertexAttribI4sv,
   DISPATCH_CMD_VertexAttribI4ubv,
   DISPATCH_CMD_VertexAttribI4usv,
   DISPATCH_CMD_VertexAttribIPointer,
   DISPATCH_CMD_Uniform1ui,
   DISPATCH_CMD_Uniform2ui,
   DISPATCH_CMD_Uniform3ui,
   DISPATCH_CMD_Uniform4ui,
   DISPATCH_CMD_Uniform1uiv,
   DISPATCH_CMD_Uniform2uiv,
   DISPATCH_CMD_Uniform3uiv,
   DISPATCH_CMD_Uniform4uiv,
   DISPATCH_CMD_BindFragDataLocation,
   DISPATCH_CMD_ClearBufferiv,
   DISPATCH_CMD_ClearBufferuiv,
   DISPATCH_CMD_ClearBufferfv,
   DISPATCH_CMD_ClearBufferfi,
   DISPATCH_CMD_BeginTransformFeedback,
   DISPATCH_CMD_EndTransformFeedback,
   DISPATCH_CMD_BindBufferRange,
   DISPATCH_CMD_BindBufferBase,
   DISPATCH_CMD_BeginConditionalRender,
   DISPATCH_CMD_EndConditionalRender,
   DISPATCH_CMD_PrimitiveRestartIndex,
   DISPATCH_CMD_FramebufferTexture,
   DISPATCH_CMD_PrimitiveRestartNV,
   DISPATCH_CMD_BindBufferOffsetEXT,
   DISPATCH_CMD_BindTransformFeedback,
   DISPATCH_CMD_DeleteTransformFeedbacks,
   DISPATCH_CMD_PauseTransformFeedback,
   DISPATCH_CMD_ResumeTransformFeedback,
   DISPATCH_CMD_DrawTransformFeedback,
   DISPATCH_CMD_VDPAUFiniNV,
   DISPATCH_CMD_VDPAUUnregisterSurfaceNV,
   DISPATCH_CMD_VDPAUSurfaceAccessNV,
   DISPATCH_CMD_VDPAUMapSurfacesNV,
   DISPATCH_CMD_VDPAUUnmapSurfacesNV,
   DISPATCH_CMD_DeleteMemoryObjectsEXT,
   DISPATCH_CMD_MemoryObjectParameterivEXT,
   DISPATCH_CMD_TexStorageMem2DEXT,
   DISPATCH_CMD_TexStorageMem2DMultisampleEXT,
   DISPATCH_CMD_TexStorageMem3DEXT,
   DISPATCH_CMD_TexStorageMem3DMultisampleEXT,
   DISPATCH_CMD_BufferStorageMemEXT,
   DISPATCH_CMD_TextureStorageMem2DEXT,
   DISPATCH_CMD_TextureStorageMem2DMultisampleEXT,
   DISPATCH_CMD_TextureStorageMem3DEXT,
   DISPATCH_CMD_TextureStorageMem3DMultisampleEXT,
   DISPATCH_CMD_NamedBufferStorageMemEXT,
   DISPATCH_CMD_TexStorageMem1DEXT,
   DISPATCH_CMD_TextureStorageMem1DEXT,
   DISPATCH_CMD_DeleteSemaphoresEXT,
   DISPATCH_CMD_SemaphoreParameterui64vEXT,
   DISPATCH_CMD_WaitSemaphoreEXT,
   DISPATCH_CMD_ImportMemoryFdEXT,
   DISPATCH_CMD_ImportSemaphoreFdEXT,
   DISPATCH_CMD_ViewportSwizzleNV,
   DISPATCH_CMD_Vertex2hNV,
   DISPATCH_CMD_Vertex2hvNV,
   DISPATCH_CMD_Vertex3hNV,
   DISPATCH_CMD_Vertex3hvNV,
   DISPATCH_CMD_Vertex4hNV,
   DISPATCH_CMD_Vertex4hvNV,
   DISPATCH_CMD_Normal3hNV,
   DISPATCH_CMD_Normal3hvNV,
   DISPATCH_CMD_Color3hNV,
   DISPATCH_CMD_Color3hvNV,
   DISPATCH_CMD_Color4hNV,
   DISPATCH_CMD_Color4hvNV,
   DISPATCH_CMD_TexCoord1hNV,
   DISPATCH_CMD_TexCoord1hvNV,
   DISPATCH_CMD_TexCoord2hNV,
   DISPATCH_CMD_TexCoord2hvNV,
   DISPATCH_CMD_TexCoord3hNV,
   DISPATCH_CMD_TexCoord3hvNV,
   DISPATCH_CMD_TexCoord4hNV,
   DISPATCH_CMD_TexCoord4hvNV,
   DISPATCH_CMD_MultiTexCoord1hNV,
   DISPATCH_CMD_MultiTexCoord1hvNV,
   DISPATCH_CMD_MultiTexCoord2hNV,
   DISPATCH_CMD_MultiTexCoord2hvNV,
   DISPATCH_CMD_MultiTexCoord3hNV,
   DISPATCH_CMD_MultiTexCoord3hvNV,
   DISPATCH_CMD_MultiTexCoord4hNV,
   DISPATCH_CMD_MultiTexCoord4hvNV,
   DISPATCH_CMD_VertexAttrib1hNV,
   DISPATCH_CMD_VertexAttrib1hvNV,
   DISPATCH_CMD_VertexAttrib2hNV,
   DISPATCH_CMD_VertexAttrib2hvNV,
   DISPATCH_CMD_VertexAttrib3hNV,
   DISPATCH_CMD_VertexAttrib3hvNV,
   DISPATCH_CMD_VertexAttrib4hNV,
   DISPATCH_CMD_VertexAttrib4hvNV,
   DISPATCH_CMD_VertexAttribs1hvNV,
   DISPATCH_CMD_VertexAttribs2hvNV,
   DISPATCH_CMD_VertexAttribs3hvNV,
   DISPATCH_CMD_VertexAttribs4hvNV,
   DISPATCH_CMD_FogCoordhNV,
   DISPATCH_CMD_FogCoordhvNV,
   DISPATCH_CMD_SecondaryColor3hNV,
   DISPATCH_CMD_SecondaryColor3hvNV,
   DISPATCH_CMD_MemoryBarrierByRegion,
   DISPATCH_CMD_AlphaFuncx,
   DISPATCH_CMD_ClearColorx,
   DISPATCH_CMD_ClearDepthx,
   DISPATCH_CMD_Color4x,
   DISPATCH_CMD_DepthRangex,
   DISPATCH_CMD_Fogx,
   DISPATCH_CMD_Fogxv,
   DISPATCH_CMD_Frustumx,
   DISPATCH_CMD_LightModelx,
   DISPATCH_CMD_LightModelxv,
   DISPATCH_CMD_Lightx,
   DISPATCH_CMD_Lightxv,
   DISPATCH_CMD_LineWidthx,
   DISPATCH_CMD_LoadMatrixx,
   DISPATCH_CMD_Materialx,
   DISPATCH_CMD_Materialxv,
   DISPATCH_CMD_MultMatrixx,
   DISPATCH_CMD_MultiTexCoord4x,
   DISPATCH_CMD_Normal3x,
   DISPATCH_CMD_Orthox,
   DISPATCH_CMD_PointSizex,
   DISPATCH_CMD_PolygonOffsetx,
   DISPATCH_CMD_Rotatex,
   DISPATCH_CMD_SampleCoveragex,
   DISPATCH_CMD_Scalex,
   DISPATCH_CMD_TexEnvx,
   DISPATCH_CMD_TexEnvxv,
   DISPATCH_CMD_TexParameterx,
   DISPATCH_CMD_Translatex,
   DISPATCH_CMD_ClipPlanex,
   DISPATCH_CMD_PointParameterx,
   DISPATCH_CMD_PointParameterxv,
   DISPATCH_CMD_TexParameterxv,
   DISPATCH_CMD_TexGenxOES,
   DISPATCH_CMD_TexGenxvOES,
   DISPATCH_CMD_ClipPlanef,
   DISPATCH_CMD_Frustumf,
   DISPATCH_CMD_Orthof,
   DISPATCH_CMD_DrawTexiOES,
   DISPATCH_CMD_DrawTexivOES,
   DISPATCH_CMD_DrawTexfOES,
   DISPATCH_CMD_DrawTexfvOES,
   DISPATCH_CMD_DrawTexsOES,
   DISPATCH_CMD_DrawTexsvOES,
   DISPATCH_CMD_DrawTexxOES,
   DISPATCH_CMD_DrawTexxvOES,
   DISPATCH_CMD_PointSizePointerOES,
   DISPATCH_CMD_DiscardFramebufferEXT,
   DISPATCH_CMD_FramebufferTexture2DMultisampleEXT,
   DISPATCH_CMD_DepthRangeArrayfvOES,
   DISPATCH_CMD_DepthRangeIndexedfOES,
   DISPATCH_CMD_FramebufferParameteriMESA,
   NUM_DISPATCH_CMD,
};

struct marshal_cmd_NewList;
uint32_t _mesa_unmarshal_NewList(struct gl_context *ctx, const struct marshal_cmd_NewList *restrict cmd);
struct marshal_cmd_EndList;
uint32_t _mesa_unmarshal_EndList(struct gl_context *ctx, const struct marshal_cmd_EndList *restrict cmd);
struct marshal_cmd_CallList;
uint32_t _mesa_unmarshal_CallList(struct gl_context *ctx, const struct marshal_cmd_CallList *restrict cmd);
void GLAPIENTRY _mesa_marshal_CallList(GLuint list);
struct marshal_cmd_CallLists;
uint32_t _mesa_unmarshal_CallLists(struct gl_context *ctx, const struct marshal_cmd_CallLists *restrict cmd);
void GLAPIENTRY _mesa_marshal_CallLists(GLsizei n, GLenum type, const GLvoid *lists);
struct marshal_cmd_DeleteLists;
uint32_t _mesa_unmarshal_DeleteLists(struct gl_context *ctx, const struct marshal_cmd_DeleteLists *restrict cmd);
struct marshal_cmd_ListBase;
uint32_t _mesa_unmarshal_ListBase(struct gl_context *ctx, const struct marshal_cmd_ListBase *restrict cmd);
struct marshal_cmd_Begin;
uint32_t _mesa_unmarshal_Begin(struct gl_context *ctx, const struct marshal_cmd_Begin *restrict cmd);
void GLAPIENTRY _mesa_marshal_Begin(GLenum mode);
struct marshal_cmd_Bitmap;
uint32_t _mesa_unmarshal_Bitmap(struct gl_context *ctx, const struct marshal_cmd_Bitmap *restrict cmd);
struct marshal_cmd_Color3b;
uint32_t _mesa_unmarshal_Color3b(struct gl_context *ctx, const struct marshal_cmd_Color3b *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3b(GLbyte red, GLbyte green, GLbyte blue);
struct marshal_cmd_Color3bv;
uint32_t _mesa_unmarshal_Color3bv(struct gl_context *ctx, const struct marshal_cmd_Color3bv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3bv(const GLbyte *v);
struct marshal_cmd_Color3d;
uint32_t _mesa_unmarshal_Color3d(struct gl_context *ctx, const struct marshal_cmd_Color3d *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3d(GLdouble red, GLdouble green, GLdouble blue);
struct marshal_cmd_Color3dv;
uint32_t _mesa_unmarshal_Color3dv(struct gl_context *ctx, const struct marshal_cmd_Color3dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3dv(const GLdouble *v);
struct marshal_cmd_Color3f;
uint32_t _mesa_unmarshal_Color3f(struct gl_context *ctx, const struct marshal_cmd_Color3f *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3f(GLfloat red, GLfloat green, GLfloat blue);
struct marshal_cmd_Color3fv;
uint32_t _mesa_unmarshal_Color3fv(struct gl_context *ctx, const struct marshal_cmd_Color3fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3fv(const GLfloat *v);
struct marshal_cmd_Color3i;
uint32_t _mesa_unmarshal_Color3i(struct gl_context *ctx, const struct marshal_cmd_Color3i *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3i(GLint red, GLint green, GLint blue);
struct marshal_cmd_Color3iv;
uint32_t _mesa_unmarshal_Color3iv(struct gl_context *ctx, const struct marshal_cmd_Color3iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3iv(const GLint *v);
struct marshal_cmd_Color3s;
uint32_t _mesa_unmarshal_Color3s(struct gl_context *ctx, const struct marshal_cmd_Color3s *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3s(GLshort red, GLshort green, GLshort blue);
struct marshal_cmd_Color3sv;
uint32_t _mesa_unmarshal_Color3sv(struct gl_context *ctx, const struct marshal_cmd_Color3sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3sv(const GLshort *v);
struct marshal_cmd_Color3ub;
uint32_t _mesa_unmarshal_Color3ub(struct gl_context *ctx, const struct marshal_cmd_Color3ub *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3ub(GLubyte red, GLubyte green, GLubyte blue);
struct marshal_cmd_Color3ubv;
uint32_t _mesa_unmarshal_Color3ubv(struct gl_context *ctx, const struct marshal_cmd_Color3ubv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3ubv(const GLubyte *v);
struct marshal_cmd_Color3ui;
uint32_t _mesa_unmarshal_Color3ui(struct gl_context *ctx, const struct marshal_cmd_Color3ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3ui(GLuint red, GLuint green, GLuint blue);
struct marshal_cmd_Color3uiv;
uint32_t _mesa_unmarshal_Color3uiv(struct gl_context *ctx, const struct marshal_cmd_Color3uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3uiv(const GLuint *v);
struct marshal_cmd_Color3us;
uint32_t _mesa_unmarshal_Color3us(struct gl_context *ctx, const struct marshal_cmd_Color3us *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3us(GLushort red, GLushort green, GLushort blue);
struct marshal_cmd_Color3usv;
uint32_t _mesa_unmarshal_Color3usv(struct gl_context *ctx, const struct marshal_cmd_Color3usv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3usv(const GLushort *v);
struct marshal_cmd_Color4b;
uint32_t _mesa_unmarshal_Color4b(struct gl_context *ctx, const struct marshal_cmd_Color4b *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4b(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
struct marshal_cmd_Color4bv;
uint32_t _mesa_unmarshal_Color4bv(struct gl_context *ctx, const struct marshal_cmd_Color4bv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4bv(const GLbyte *v);
struct marshal_cmd_Color4d;
uint32_t _mesa_unmarshal_Color4d(struct gl_context *ctx, const struct marshal_cmd_Color4d *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
struct marshal_cmd_Color4dv;
uint32_t _mesa_unmarshal_Color4dv(struct gl_context *ctx, const struct marshal_cmd_Color4dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4dv(const GLdouble *v);
struct marshal_cmd_Color4f;
uint32_t _mesa_unmarshal_Color4f(struct gl_context *ctx, const struct marshal_cmd_Color4f *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
struct marshal_cmd_Color4fv;
uint32_t _mesa_unmarshal_Color4fv(struct gl_context *ctx, const struct marshal_cmd_Color4fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4fv(const GLfloat *v);
struct marshal_cmd_Color4i;
uint32_t _mesa_unmarshal_Color4i(struct gl_context *ctx, const struct marshal_cmd_Color4i *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4i(GLint red, GLint green, GLint blue, GLint alpha);
struct marshal_cmd_Color4iv;
uint32_t _mesa_unmarshal_Color4iv(struct gl_context *ctx, const struct marshal_cmd_Color4iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4iv(const GLint *v);
struct marshal_cmd_Color4s;
uint32_t _mesa_unmarshal_Color4s(struct gl_context *ctx, const struct marshal_cmd_Color4s *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4s(GLshort red, GLshort green, GLshort blue, GLshort alpha);
struct marshal_cmd_Color4sv;
uint32_t _mesa_unmarshal_Color4sv(struct gl_context *ctx, const struct marshal_cmd_Color4sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4sv(const GLshort *v);
struct marshal_cmd_Color4ub;
uint32_t _mesa_unmarshal_Color4ub(struct gl_context *ctx, const struct marshal_cmd_Color4ub *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4ub(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
struct marshal_cmd_Color4ubv;
uint32_t _mesa_unmarshal_Color4ubv(struct gl_context *ctx, const struct marshal_cmd_Color4ubv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4ubv(const GLubyte *v);
struct marshal_cmd_Color4ui;
uint32_t _mesa_unmarshal_Color4ui(struct gl_context *ctx, const struct marshal_cmd_Color4ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4ui(GLuint red, GLuint green, GLuint blue, GLuint alpha);
struct marshal_cmd_Color4uiv;
uint32_t _mesa_unmarshal_Color4uiv(struct gl_context *ctx, const struct marshal_cmd_Color4uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4uiv(const GLuint *v);
struct marshal_cmd_Color4us;
uint32_t _mesa_unmarshal_Color4us(struct gl_context *ctx, const struct marshal_cmd_Color4us *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4us(GLushort red, GLushort green, GLushort blue, GLushort alpha);
struct marshal_cmd_Color4usv;
uint32_t _mesa_unmarshal_Color4usv(struct gl_context *ctx, const struct marshal_cmd_Color4usv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4usv(const GLushort *v);
struct marshal_cmd_EdgeFlag;
uint32_t _mesa_unmarshal_EdgeFlag(struct gl_context *ctx, const struct marshal_cmd_EdgeFlag *restrict cmd);
void GLAPIENTRY _mesa_marshal_EdgeFlag(GLboolean flag);
struct marshal_cmd_EdgeFlagv;
uint32_t _mesa_unmarshal_EdgeFlagv(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagv *restrict cmd);
void GLAPIENTRY _mesa_marshal_EdgeFlagv(const GLboolean *flag);
struct marshal_cmd_End;
uint32_t _mesa_unmarshal_End(struct gl_context *ctx, const struct marshal_cmd_End *restrict cmd);
void GLAPIENTRY _mesa_marshal_End(void);
struct marshal_cmd_Indexd;
uint32_t _mesa_unmarshal_Indexd(struct gl_context *ctx, const struct marshal_cmd_Indexd *restrict cmd);
void GLAPIENTRY _mesa_marshal_Indexd(GLdouble c);
struct marshal_cmd_Indexdv;
uint32_t _mesa_unmarshal_Indexdv(struct gl_context *ctx, const struct marshal_cmd_Indexdv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Indexdv(const GLdouble *c);
struct marshal_cmd_Indexf;
uint32_t _mesa_unmarshal_Indexf(struct gl_context *ctx, const struct marshal_cmd_Indexf *restrict cmd);
void GLAPIENTRY _mesa_marshal_Indexf(GLfloat c);
struct marshal_cmd_Indexfv;
uint32_t _mesa_unmarshal_Indexfv(struct gl_context *ctx, const struct marshal_cmd_Indexfv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Indexfv(const GLfloat *c);
struct marshal_cmd_Indexi;
uint32_t _mesa_unmarshal_Indexi(struct gl_context *ctx, const struct marshal_cmd_Indexi *restrict cmd);
void GLAPIENTRY _mesa_marshal_Indexi(GLint c);
struct marshal_cmd_Indexiv;
uint32_t _mesa_unmarshal_Indexiv(struct gl_context *ctx, const struct marshal_cmd_Indexiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Indexiv(const GLint *c);
struct marshal_cmd_Indexs;
uint32_t _mesa_unmarshal_Indexs(struct gl_context *ctx, const struct marshal_cmd_Indexs *restrict cmd);
void GLAPIENTRY _mesa_marshal_Indexs(GLshort c);
struct marshal_cmd_Indexsv;
uint32_t _mesa_unmarshal_Indexsv(struct gl_context *ctx, const struct marshal_cmd_Indexsv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Indexsv(const GLshort *c);
struct marshal_cmd_Normal3b;
uint32_t _mesa_unmarshal_Normal3b(struct gl_context *ctx, const struct marshal_cmd_Normal3b *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3b(GLbyte nx, GLbyte ny, GLbyte nz);
struct marshal_cmd_Normal3bv;
uint32_t _mesa_unmarshal_Normal3bv(struct gl_context *ctx, const struct marshal_cmd_Normal3bv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3bv(const GLbyte *v);
struct marshal_cmd_Normal3d;
uint32_t _mesa_unmarshal_Normal3d(struct gl_context *ctx, const struct marshal_cmd_Normal3d *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3d(GLdouble nx, GLdouble ny, GLdouble nz);
struct marshal_cmd_Normal3dv;
uint32_t _mesa_unmarshal_Normal3dv(struct gl_context *ctx, const struct marshal_cmd_Normal3dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3dv(const GLdouble *v);
struct marshal_cmd_Normal3f;
uint32_t _mesa_unmarshal_Normal3f(struct gl_context *ctx, const struct marshal_cmd_Normal3f *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3f(GLfloat nx, GLfloat ny, GLfloat nz);
struct marshal_cmd_Normal3fv;
uint32_t _mesa_unmarshal_Normal3fv(struct gl_context *ctx, const struct marshal_cmd_Normal3fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3fv(const GLfloat *v);
struct marshal_cmd_Normal3i;
uint32_t _mesa_unmarshal_Normal3i(struct gl_context *ctx, const struct marshal_cmd_Normal3i *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3i(GLint nx, GLint ny, GLint nz);
struct marshal_cmd_Normal3iv;
uint32_t _mesa_unmarshal_Normal3iv(struct gl_context *ctx, const struct marshal_cmd_Normal3iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3iv(const GLint *v);
struct marshal_cmd_Normal3s;
uint32_t _mesa_unmarshal_Normal3s(struct gl_context *ctx, const struct marshal_cmd_Normal3s *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3s(GLshort nx, GLshort ny, GLshort nz);
struct marshal_cmd_Normal3sv;
uint32_t _mesa_unmarshal_Normal3sv(struct gl_context *ctx, const struct marshal_cmd_Normal3sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3sv(const GLshort *v);
struct marshal_cmd_RasterPos2d;
uint32_t _mesa_unmarshal_RasterPos2d(struct gl_context *ctx, const struct marshal_cmd_RasterPos2d *restrict cmd);
struct marshal_cmd_RasterPos2dv;
uint32_t _mesa_unmarshal_RasterPos2dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2dv *restrict cmd);
struct marshal_cmd_RasterPos2f;
uint32_t _mesa_unmarshal_RasterPos2f(struct gl_context *ctx, const struct marshal_cmd_RasterPos2f *restrict cmd);
struct marshal_cmd_RasterPos2fv;
uint32_t _mesa_unmarshal_RasterPos2fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2fv *restrict cmd);
struct marshal_cmd_RasterPos2i;
uint32_t _mesa_unmarshal_RasterPos2i(struct gl_context *ctx, const struct marshal_cmd_RasterPos2i *restrict cmd);
struct marshal_cmd_RasterPos2iv;
uint32_t _mesa_unmarshal_RasterPos2iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2iv *restrict cmd);
struct marshal_cmd_RasterPos2s;
uint32_t _mesa_unmarshal_RasterPos2s(struct gl_context *ctx, const struct marshal_cmd_RasterPos2s *restrict cmd);
struct marshal_cmd_RasterPos2sv;
uint32_t _mesa_unmarshal_RasterPos2sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos2sv *restrict cmd);
struct marshal_cmd_RasterPos3d;
uint32_t _mesa_unmarshal_RasterPos3d(struct gl_context *ctx, const struct marshal_cmd_RasterPos3d *restrict cmd);
struct marshal_cmd_RasterPos3dv;
uint32_t _mesa_unmarshal_RasterPos3dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3dv *restrict cmd);
struct marshal_cmd_RasterPos3f;
uint32_t _mesa_unmarshal_RasterPos3f(struct gl_context *ctx, const struct marshal_cmd_RasterPos3f *restrict cmd);
struct marshal_cmd_RasterPos3fv;
uint32_t _mesa_unmarshal_RasterPos3fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3fv *restrict cmd);
struct marshal_cmd_RasterPos3i;
uint32_t _mesa_unmarshal_RasterPos3i(struct gl_context *ctx, const struct marshal_cmd_RasterPos3i *restrict cmd);
struct marshal_cmd_RasterPos3iv;
uint32_t _mesa_unmarshal_RasterPos3iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3iv *restrict cmd);
struct marshal_cmd_RasterPos3s;
uint32_t _mesa_unmarshal_RasterPos3s(struct gl_context *ctx, const struct marshal_cmd_RasterPos3s *restrict cmd);
struct marshal_cmd_RasterPos3sv;
uint32_t _mesa_unmarshal_RasterPos3sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos3sv *restrict cmd);
struct marshal_cmd_RasterPos4d;
uint32_t _mesa_unmarshal_RasterPos4d(struct gl_context *ctx, const struct marshal_cmd_RasterPos4d *restrict cmd);
struct marshal_cmd_RasterPos4dv;
uint32_t _mesa_unmarshal_RasterPos4dv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4dv *restrict cmd);
struct marshal_cmd_RasterPos4f;
uint32_t _mesa_unmarshal_RasterPos4f(struct gl_context *ctx, const struct marshal_cmd_RasterPos4f *restrict cmd);
struct marshal_cmd_RasterPos4fv;
uint32_t _mesa_unmarshal_RasterPos4fv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4fv *restrict cmd);
struct marshal_cmd_RasterPos4i;
uint32_t _mesa_unmarshal_RasterPos4i(struct gl_context *ctx, const struct marshal_cmd_RasterPos4i *restrict cmd);
struct marshal_cmd_RasterPos4iv;
uint32_t _mesa_unmarshal_RasterPos4iv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4iv *restrict cmd);
struct marshal_cmd_RasterPos4s;
uint32_t _mesa_unmarshal_RasterPos4s(struct gl_context *ctx, const struct marshal_cmd_RasterPos4s *restrict cmd);
struct marshal_cmd_RasterPos4sv;
uint32_t _mesa_unmarshal_RasterPos4sv(struct gl_context *ctx, const struct marshal_cmd_RasterPos4sv *restrict cmd);
struct marshal_cmd_Rectd;
uint32_t _mesa_unmarshal_Rectd(struct gl_context *ctx, const struct marshal_cmd_Rectd *restrict cmd);
struct marshal_cmd_Rectdv;
uint32_t _mesa_unmarshal_Rectdv(struct gl_context *ctx, const struct marshal_cmd_Rectdv *restrict cmd);
struct marshal_cmd_Rectf;
uint32_t _mesa_unmarshal_Rectf(struct gl_context *ctx, const struct marshal_cmd_Rectf *restrict cmd);
struct marshal_cmd_Rectfv;
uint32_t _mesa_unmarshal_Rectfv(struct gl_context *ctx, const struct marshal_cmd_Rectfv *restrict cmd);
struct marshal_cmd_Recti;
uint32_t _mesa_unmarshal_Recti(struct gl_context *ctx, const struct marshal_cmd_Recti *restrict cmd);
struct marshal_cmd_Rectiv;
uint32_t _mesa_unmarshal_Rectiv(struct gl_context *ctx, const struct marshal_cmd_Rectiv *restrict cmd);
struct marshal_cmd_Rects;
uint32_t _mesa_unmarshal_Rects(struct gl_context *ctx, const struct marshal_cmd_Rects *restrict cmd);
struct marshal_cmd_Rectsv;
uint32_t _mesa_unmarshal_Rectsv(struct gl_context *ctx, const struct marshal_cmd_Rectsv *restrict cmd);
struct marshal_cmd_TexCoord1d;
uint32_t _mesa_unmarshal_TexCoord1d(struct gl_context *ctx, const struct marshal_cmd_TexCoord1d *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord1d(GLdouble s);
struct marshal_cmd_TexCoord1dv;
uint32_t _mesa_unmarshal_TexCoord1dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord1dv(const GLdouble *v);
struct marshal_cmd_TexCoord1f;
uint32_t _mesa_unmarshal_TexCoord1f(struct gl_context *ctx, const struct marshal_cmd_TexCoord1f *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord1f(GLfloat s);
struct marshal_cmd_TexCoord1fv;
uint32_t _mesa_unmarshal_TexCoord1fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord1fv(const GLfloat *v);
struct marshal_cmd_TexCoord1i;
uint32_t _mesa_unmarshal_TexCoord1i(struct gl_context *ctx, const struct marshal_cmd_TexCoord1i *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord1i(GLint s);
struct marshal_cmd_TexCoord1iv;
uint32_t _mesa_unmarshal_TexCoord1iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord1iv(const GLint *v);
struct marshal_cmd_TexCoord1s;
uint32_t _mesa_unmarshal_TexCoord1s(struct gl_context *ctx, const struct marshal_cmd_TexCoord1s *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord1s(GLshort s);
struct marshal_cmd_TexCoord1sv;
uint32_t _mesa_unmarshal_TexCoord1sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord1sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord1sv(const GLshort *v);
struct marshal_cmd_TexCoord2d;
uint32_t _mesa_unmarshal_TexCoord2d(struct gl_context *ctx, const struct marshal_cmd_TexCoord2d *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord2d(GLdouble s, GLdouble t);
struct marshal_cmd_TexCoord2dv;
uint32_t _mesa_unmarshal_TexCoord2dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord2dv(const GLdouble *v);
struct marshal_cmd_TexCoord2f;
uint32_t _mesa_unmarshal_TexCoord2f(struct gl_context *ctx, const struct marshal_cmd_TexCoord2f *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord2f(GLfloat s, GLfloat t);
struct marshal_cmd_TexCoord2fv;
uint32_t _mesa_unmarshal_TexCoord2fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord2fv(const GLfloat *v);
struct marshal_cmd_TexCoord2i;
uint32_t _mesa_unmarshal_TexCoord2i(struct gl_context *ctx, const struct marshal_cmd_TexCoord2i *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord2i(GLint s, GLint t);
struct marshal_cmd_TexCoord2iv;
uint32_t _mesa_unmarshal_TexCoord2iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord2iv(const GLint *v);
struct marshal_cmd_TexCoord2s;
uint32_t _mesa_unmarshal_TexCoord2s(struct gl_context *ctx, const struct marshal_cmd_TexCoord2s *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord2s(GLshort s, GLshort t);
struct marshal_cmd_TexCoord2sv;
uint32_t _mesa_unmarshal_TexCoord2sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord2sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord2sv(const GLshort *v);
struct marshal_cmd_TexCoord3d;
uint32_t _mesa_unmarshal_TexCoord3d(struct gl_context *ctx, const struct marshal_cmd_TexCoord3d *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord3d(GLdouble s, GLdouble t, GLdouble r);
struct marshal_cmd_TexCoord3dv;
uint32_t _mesa_unmarshal_TexCoord3dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord3dv(const GLdouble *v);
struct marshal_cmd_TexCoord3f;
uint32_t _mesa_unmarshal_TexCoord3f(struct gl_context *ctx, const struct marshal_cmd_TexCoord3f *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord3f(GLfloat s, GLfloat t, GLfloat r);
struct marshal_cmd_TexCoord3fv;
uint32_t _mesa_unmarshal_TexCoord3fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord3fv(const GLfloat *v);
struct marshal_cmd_TexCoord3i;
uint32_t _mesa_unmarshal_TexCoord3i(struct gl_context *ctx, const struct marshal_cmd_TexCoord3i *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord3i(GLint s, GLint t, GLint r);
struct marshal_cmd_TexCoord3iv;
uint32_t _mesa_unmarshal_TexCoord3iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord3iv(const GLint *v);
struct marshal_cmd_TexCoord3s;
uint32_t _mesa_unmarshal_TexCoord3s(struct gl_context *ctx, const struct marshal_cmd_TexCoord3s *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord3s(GLshort s, GLshort t, GLshort r);
struct marshal_cmd_TexCoord3sv;
uint32_t _mesa_unmarshal_TexCoord3sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord3sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord3sv(const GLshort *v);
struct marshal_cmd_TexCoord4d;
uint32_t _mesa_unmarshal_TexCoord4d(struct gl_context *ctx, const struct marshal_cmd_TexCoord4d *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord4d(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
struct marshal_cmd_TexCoord4dv;
uint32_t _mesa_unmarshal_TexCoord4dv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord4dv(const GLdouble *v);
struct marshal_cmd_TexCoord4f;
uint32_t _mesa_unmarshal_TexCoord4f(struct gl_context *ctx, const struct marshal_cmd_TexCoord4f *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
struct marshal_cmd_TexCoord4fv;
uint32_t _mesa_unmarshal_TexCoord4fv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord4fv(const GLfloat *v);
struct marshal_cmd_TexCoord4i;
uint32_t _mesa_unmarshal_TexCoord4i(struct gl_context *ctx, const struct marshal_cmd_TexCoord4i *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord4i(GLint s, GLint t, GLint r, GLint q);
struct marshal_cmd_TexCoord4iv;
uint32_t _mesa_unmarshal_TexCoord4iv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord4iv(const GLint *v);
struct marshal_cmd_TexCoord4s;
uint32_t _mesa_unmarshal_TexCoord4s(struct gl_context *ctx, const struct marshal_cmd_TexCoord4s *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord4s(GLshort s, GLshort t, GLshort r, GLshort q);
struct marshal_cmd_TexCoord4sv;
uint32_t _mesa_unmarshal_TexCoord4sv(struct gl_context *ctx, const struct marshal_cmd_TexCoord4sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord4sv(const GLshort *v);
struct marshal_cmd_Vertex2d;
uint32_t _mesa_unmarshal_Vertex2d(struct gl_context *ctx, const struct marshal_cmd_Vertex2d *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex2d(GLdouble x, GLdouble y);
struct marshal_cmd_Vertex2dv;
uint32_t _mesa_unmarshal_Vertex2dv(struct gl_context *ctx, const struct marshal_cmd_Vertex2dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex2dv(const GLdouble *v);
struct marshal_cmd_Vertex2f;
uint32_t _mesa_unmarshal_Vertex2f(struct gl_context *ctx, const struct marshal_cmd_Vertex2f *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex2f(GLfloat x, GLfloat y);
struct marshal_cmd_Vertex2fv;
uint32_t _mesa_unmarshal_Vertex2fv(struct gl_context *ctx, const struct marshal_cmd_Vertex2fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex2fv(const GLfloat *v);
struct marshal_cmd_Vertex2i;
uint32_t _mesa_unmarshal_Vertex2i(struct gl_context *ctx, const struct marshal_cmd_Vertex2i *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex2i(GLint x, GLint y);
struct marshal_cmd_Vertex2iv;
uint32_t _mesa_unmarshal_Vertex2iv(struct gl_context *ctx, const struct marshal_cmd_Vertex2iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex2iv(const GLint *v);
struct marshal_cmd_Vertex2s;
uint32_t _mesa_unmarshal_Vertex2s(struct gl_context *ctx, const struct marshal_cmd_Vertex2s *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex2s(GLshort x, GLshort y);
struct marshal_cmd_Vertex2sv;
uint32_t _mesa_unmarshal_Vertex2sv(struct gl_context *ctx, const struct marshal_cmd_Vertex2sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex2sv(const GLshort *v);
struct marshal_cmd_Vertex3d;
uint32_t _mesa_unmarshal_Vertex3d(struct gl_context *ctx, const struct marshal_cmd_Vertex3d *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex3d(GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_Vertex3dv;
uint32_t _mesa_unmarshal_Vertex3dv(struct gl_context *ctx, const struct marshal_cmd_Vertex3dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex3dv(const GLdouble *v);
struct marshal_cmd_Vertex3f;
uint32_t _mesa_unmarshal_Vertex3f(struct gl_context *ctx, const struct marshal_cmd_Vertex3f *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex3f(GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_Vertex3fv;
uint32_t _mesa_unmarshal_Vertex3fv(struct gl_context *ctx, const struct marshal_cmd_Vertex3fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex3fv(const GLfloat *v);
struct marshal_cmd_Vertex3i;
uint32_t _mesa_unmarshal_Vertex3i(struct gl_context *ctx, const struct marshal_cmd_Vertex3i *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex3i(GLint x, GLint y, GLint z);
struct marshal_cmd_Vertex3iv;
uint32_t _mesa_unmarshal_Vertex3iv(struct gl_context *ctx, const struct marshal_cmd_Vertex3iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex3iv(const GLint *v);
struct marshal_cmd_Vertex3s;
uint32_t _mesa_unmarshal_Vertex3s(struct gl_context *ctx, const struct marshal_cmd_Vertex3s *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex3s(GLshort x, GLshort y, GLshort z);
struct marshal_cmd_Vertex3sv;
uint32_t _mesa_unmarshal_Vertex3sv(struct gl_context *ctx, const struct marshal_cmd_Vertex3sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex3sv(const GLshort *v);
struct marshal_cmd_Vertex4d;
uint32_t _mesa_unmarshal_Vertex4d(struct gl_context *ctx, const struct marshal_cmd_Vertex4d *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex4d(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_Vertex4dv;
uint32_t _mesa_unmarshal_Vertex4dv(struct gl_context *ctx, const struct marshal_cmd_Vertex4dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex4dv(const GLdouble *v);
struct marshal_cmd_Vertex4f;
uint32_t _mesa_unmarshal_Vertex4f(struct gl_context *ctx, const struct marshal_cmd_Vertex4f *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex4f(GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_Vertex4fv;
uint32_t _mesa_unmarshal_Vertex4fv(struct gl_context *ctx, const struct marshal_cmd_Vertex4fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex4fv(const GLfloat *v);
struct marshal_cmd_Vertex4i;
uint32_t _mesa_unmarshal_Vertex4i(struct gl_context *ctx, const struct marshal_cmd_Vertex4i *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex4i(GLint x, GLint y, GLint z, GLint w);
struct marshal_cmd_Vertex4iv;
uint32_t _mesa_unmarshal_Vertex4iv(struct gl_context *ctx, const struct marshal_cmd_Vertex4iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex4iv(const GLint *v);
struct marshal_cmd_Vertex4s;
uint32_t _mesa_unmarshal_Vertex4s(struct gl_context *ctx, const struct marshal_cmd_Vertex4s *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex4s(GLshort x, GLshort y, GLshort z, GLshort w);
struct marshal_cmd_Vertex4sv;
uint32_t _mesa_unmarshal_Vertex4sv(struct gl_context *ctx, const struct marshal_cmd_Vertex4sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex4sv(const GLshort *v);
struct marshal_cmd_ClipPlane;
uint32_t _mesa_unmarshal_ClipPlane(struct gl_context *ctx, const struct marshal_cmd_ClipPlane *restrict cmd);
struct marshal_cmd_ColorMaterial;
uint32_t _mesa_unmarshal_ColorMaterial(struct gl_context *ctx, const struct marshal_cmd_ColorMaterial *restrict cmd);
struct marshal_cmd_CullFace;
uint32_t _mesa_unmarshal_CullFace(struct gl_context *ctx, const struct marshal_cmd_CullFace *restrict cmd);
struct marshal_cmd_Fogf;
uint32_t _mesa_unmarshal_Fogf(struct gl_context *ctx, const struct marshal_cmd_Fogf *restrict cmd);
struct marshal_cmd_Fogfv;
uint32_t _mesa_unmarshal_Fogfv(struct gl_context *ctx, const struct marshal_cmd_Fogfv *restrict cmd);
struct marshal_cmd_Fogi;
uint32_t _mesa_unmarshal_Fogi(struct gl_context *ctx, const struct marshal_cmd_Fogi *restrict cmd);
struct marshal_cmd_Fogiv;
uint32_t _mesa_unmarshal_Fogiv(struct gl_context *ctx, const struct marshal_cmd_Fogiv *restrict cmd);
struct marshal_cmd_FrontFace;
uint32_t _mesa_unmarshal_FrontFace(struct gl_context *ctx, const struct marshal_cmd_FrontFace *restrict cmd);
struct marshal_cmd_Hint;
uint32_t _mesa_unmarshal_Hint(struct gl_context *ctx, const struct marshal_cmd_Hint *restrict cmd);
struct marshal_cmd_Lightf;
uint32_t _mesa_unmarshal_Lightf(struct gl_context *ctx, const struct marshal_cmd_Lightf *restrict cmd);
struct marshal_cmd_Lightfv;
uint32_t _mesa_unmarshal_Lightfv(struct gl_context *ctx, const struct marshal_cmd_Lightfv *restrict cmd);
struct marshal_cmd_Lighti;
uint32_t _mesa_unmarshal_Lighti(struct gl_context *ctx, const struct marshal_cmd_Lighti *restrict cmd);
struct marshal_cmd_Lightiv;
uint32_t _mesa_unmarshal_Lightiv(struct gl_context *ctx, const struct marshal_cmd_Lightiv *restrict cmd);
struct marshal_cmd_LightModelf;
uint32_t _mesa_unmarshal_LightModelf(struct gl_context *ctx, const struct marshal_cmd_LightModelf *restrict cmd);
struct marshal_cmd_LightModelfv;
uint32_t _mesa_unmarshal_LightModelfv(struct gl_context *ctx, const struct marshal_cmd_LightModelfv *restrict cmd);
struct marshal_cmd_LightModeli;
uint32_t _mesa_unmarshal_LightModeli(struct gl_context *ctx, const struct marshal_cmd_LightModeli *restrict cmd);
struct marshal_cmd_LightModeliv;
uint32_t _mesa_unmarshal_LightModeliv(struct gl_context *ctx, const struct marshal_cmd_LightModeliv *restrict cmd);
struct marshal_cmd_LineStipple;
uint32_t _mesa_unmarshal_LineStipple(struct gl_context *ctx, const struct marshal_cmd_LineStipple *restrict cmd);
struct marshal_cmd_LineWidth;
uint32_t _mesa_unmarshal_LineWidth(struct gl_context *ctx, const struct marshal_cmd_LineWidth *restrict cmd);
struct marshal_cmd_Materialf;
uint32_t _mesa_unmarshal_Materialf(struct gl_context *ctx, const struct marshal_cmd_Materialf *restrict cmd);
void GLAPIENTRY _mesa_marshal_Materialf(GLenum face, GLenum pname, GLfloat param);
struct marshal_cmd_Materialfv;
uint32_t _mesa_unmarshal_Materialfv(struct gl_context *ctx, const struct marshal_cmd_Materialfv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Materialfv(GLenum face, GLenum pname, const GLfloat *params);
struct marshal_cmd_Materiali;
uint32_t _mesa_unmarshal_Materiali(struct gl_context *ctx, const struct marshal_cmd_Materiali *restrict cmd);
void GLAPIENTRY _mesa_marshal_Materiali(GLenum face, GLenum pname, GLint param);
struct marshal_cmd_Materialiv;
uint32_t _mesa_unmarshal_Materialiv(struct gl_context *ctx, const struct marshal_cmd_Materialiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Materialiv(GLenum face, GLenum pname, const GLint *params);
struct marshal_cmd_PointSize;
uint32_t _mesa_unmarshal_PointSize(struct gl_context *ctx, const struct marshal_cmd_PointSize *restrict cmd);
struct marshal_cmd_PolygonMode;
uint32_t _mesa_unmarshal_PolygonMode(struct gl_context *ctx, const struct marshal_cmd_PolygonMode *restrict cmd);
struct marshal_cmd_PolygonStipple;
uint32_t _mesa_unmarshal_PolygonStipple(struct gl_context *ctx, const struct marshal_cmd_PolygonStipple *restrict cmd);
struct marshal_cmd_Scissor;
uint32_t _mesa_unmarshal_Scissor(struct gl_context *ctx, const struct marshal_cmd_Scissor *restrict cmd);
struct marshal_cmd_ShadeModel;
uint32_t _mesa_unmarshal_ShadeModel(struct gl_context *ctx, const struct marshal_cmd_ShadeModel *restrict cmd);
struct marshal_cmd_TexParameterf;
uint32_t _mesa_unmarshal_TexParameterf(struct gl_context *ctx, const struct marshal_cmd_TexParameterf *restrict cmd);
struct marshal_cmd_TexParameterfv;
uint32_t _mesa_unmarshal_TexParameterfv(struct gl_context *ctx, const struct marshal_cmd_TexParameterfv *restrict cmd);
struct marshal_cmd_TexParameteri;
uint32_t _mesa_unmarshal_TexParameteri(struct gl_context *ctx, const struct marshal_cmd_TexParameteri *restrict cmd);
struct marshal_cmd_TexParameteriv;
uint32_t _mesa_unmarshal_TexParameteriv(struct gl_context *ctx, const struct marshal_cmd_TexParameteriv *restrict cmd);
struct marshal_cmd_TexImage1D;
uint32_t _mesa_unmarshal_TexImage1D(struct gl_context *ctx, const struct marshal_cmd_TexImage1D *restrict cmd);
struct marshal_cmd_TexImage2D;
uint32_t _mesa_unmarshal_TexImage2D(struct gl_context *ctx, const struct marshal_cmd_TexImage2D *restrict cmd);
struct marshal_cmd_TexEnvf;
uint32_t _mesa_unmarshal_TexEnvf(struct gl_context *ctx, const struct marshal_cmd_TexEnvf *restrict cmd);
struct marshal_cmd_TexEnvfv;
uint32_t _mesa_unmarshal_TexEnvfv(struct gl_context *ctx, const struct marshal_cmd_TexEnvfv *restrict cmd);
struct marshal_cmd_TexEnvi;
uint32_t _mesa_unmarshal_TexEnvi(struct gl_context *ctx, const struct marshal_cmd_TexEnvi *restrict cmd);
struct marshal_cmd_TexEnviv;
uint32_t _mesa_unmarshal_TexEnviv(struct gl_context *ctx, const struct marshal_cmd_TexEnviv *restrict cmd);
struct marshal_cmd_TexGend;
uint32_t _mesa_unmarshal_TexGend(struct gl_context *ctx, const struct marshal_cmd_TexGend *restrict cmd);
struct marshal_cmd_TexGendv;
uint32_t _mesa_unmarshal_TexGendv(struct gl_context *ctx, const struct marshal_cmd_TexGendv *restrict cmd);
struct marshal_cmd_TexGenf;
uint32_t _mesa_unmarshal_TexGenf(struct gl_context *ctx, const struct marshal_cmd_TexGenf *restrict cmd);
struct marshal_cmd_TexGenfv;
uint32_t _mesa_unmarshal_TexGenfv(struct gl_context *ctx, const struct marshal_cmd_TexGenfv *restrict cmd);
struct marshal_cmd_TexGeni;
uint32_t _mesa_unmarshal_TexGeni(struct gl_context *ctx, const struct marshal_cmd_TexGeni *restrict cmd);
struct marshal_cmd_TexGeniv;
uint32_t _mesa_unmarshal_TexGeniv(struct gl_context *ctx, const struct marshal_cmd_TexGeniv *restrict cmd);
struct marshal_cmd_InitNames;
uint32_t _mesa_unmarshal_InitNames(struct gl_context *ctx, const struct marshal_cmd_InitNames *restrict cmd);
struct marshal_cmd_LoadName;
uint32_t _mesa_unmarshal_LoadName(struct gl_context *ctx, const struct marshal_cmd_LoadName *restrict cmd);
struct marshal_cmd_PassThrough;
uint32_t _mesa_unmarshal_PassThrough(struct gl_context *ctx, const struct marshal_cmd_PassThrough *restrict cmd);
struct marshal_cmd_PopName;
uint32_t _mesa_unmarshal_PopName(struct gl_context *ctx, const struct marshal_cmd_PopName *restrict cmd);
struct marshal_cmd_PushName;
uint32_t _mesa_unmarshal_PushName(struct gl_context *ctx, const struct marshal_cmd_PushName *restrict cmd);
struct marshal_cmd_DrawBuffer;
uint32_t _mesa_unmarshal_DrawBuffer(struct gl_context *ctx, const struct marshal_cmd_DrawBuffer *restrict cmd);
struct marshal_cmd_Clear;
uint32_t _mesa_unmarshal_Clear(struct gl_context *ctx, const struct marshal_cmd_Clear *restrict cmd);
struct marshal_cmd_ClearAccum;
uint32_t _mesa_unmarshal_ClearAccum(struct gl_context *ctx, const struct marshal_cmd_ClearAccum *restrict cmd);
struct marshal_cmd_ClearIndex;
uint32_t _mesa_unmarshal_ClearIndex(struct gl_context *ctx, const struct marshal_cmd_ClearIndex *restrict cmd);
struct marshal_cmd_ClearColor;
uint32_t _mesa_unmarshal_ClearColor(struct gl_context *ctx, const struct marshal_cmd_ClearColor *restrict cmd);
struct marshal_cmd_ClearStencil;
uint32_t _mesa_unmarshal_ClearStencil(struct gl_context *ctx, const struct marshal_cmd_ClearStencil *restrict cmd);
struct marshal_cmd_ClearDepth;
uint32_t _mesa_unmarshal_ClearDepth(struct gl_context *ctx, const struct marshal_cmd_ClearDepth *restrict cmd);
struct marshal_cmd_StencilMask;
uint32_t _mesa_unmarshal_StencilMask(struct gl_context *ctx, const struct marshal_cmd_StencilMask *restrict cmd);
struct marshal_cmd_ColorMask;
uint32_t _mesa_unmarshal_ColorMask(struct gl_context *ctx, const struct marshal_cmd_ColorMask *restrict cmd);
struct marshal_cmd_DepthMask;
uint32_t _mesa_unmarshal_DepthMask(struct gl_context *ctx, const struct marshal_cmd_DepthMask *restrict cmd);
struct marshal_cmd_IndexMask;
uint32_t _mesa_unmarshal_IndexMask(struct gl_context *ctx, const struct marshal_cmd_IndexMask *restrict cmd);
struct marshal_cmd_Accum;
uint32_t _mesa_unmarshal_Accum(struct gl_context *ctx, const struct marshal_cmd_Accum *restrict cmd);
struct marshal_cmd_Disable;
uint32_t _mesa_unmarshal_Disable(struct gl_context *ctx, const struct marshal_cmd_Disable *restrict cmd);
struct marshal_cmd_Enable;
uint32_t _mesa_unmarshal_Enable(struct gl_context *ctx, const struct marshal_cmd_Enable *restrict cmd);
struct marshal_cmd_Flush;
uint32_t _mesa_unmarshal_Flush(struct gl_context *ctx, const struct marshal_cmd_Flush *restrict cmd);
struct marshal_cmd_PopAttrib;
uint32_t _mesa_unmarshal_PopAttrib(struct gl_context *ctx, const struct marshal_cmd_PopAttrib *restrict cmd);
struct marshal_cmd_PushAttrib;
uint32_t _mesa_unmarshal_PushAttrib(struct gl_context *ctx, const struct marshal_cmd_PushAttrib *restrict cmd);
struct marshal_cmd_MapGrid1d;
uint32_t _mesa_unmarshal_MapGrid1d(struct gl_context *ctx, const struct marshal_cmd_MapGrid1d *restrict cmd);
struct marshal_cmd_MapGrid1f;
uint32_t _mesa_unmarshal_MapGrid1f(struct gl_context *ctx, const struct marshal_cmd_MapGrid1f *restrict cmd);
struct marshal_cmd_MapGrid2d;
uint32_t _mesa_unmarshal_MapGrid2d(struct gl_context *ctx, const struct marshal_cmd_MapGrid2d *restrict cmd);
struct marshal_cmd_MapGrid2f;
uint32_t _mesa_unmarshal_MapGrid2f(struct gl_context *ctx, const struct marshal_cmd_MapGrid2f *restrict cmd);
struct marshal_cmd_EvalCoord1d;
uint32_t _mesa_unmarshal_EvalCoord1d(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1d *restrict cmd);
void GLAPIENTRY _mesa_marshal_EvalCoord1d(GLdouble u);
struct marshal_cmd_EvalCoord1dv;
uint32_t _mesa_unmarshal_EvalCoord1dv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_EvalCoord1dv(const GLdouble *u);
struct marshal_cmd_EvalCoord1f;
uint32_t _mesa_unmarshal_EvalCoord1f(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1f *restrict cmd);
void GLAPIENTRY _mesa_marshal_EvalCoord1f(GLfloat u);
struct marshal_cmd_EvalCoord1fv;
uint32_t _mesa_unmarshal_EvalCoord1fv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord1fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_EvalCoord1fv(const GLfloat *u);
struct marshal_cmd_EvalCoord2d;
uint32_t _mesa_unmarshal_EvalCoord2d(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2d *restrict cmd);
void GLAPIENTRY _mesa_marshal_EvalCoord2d(GLdouble u, GLdouble v);
struct marshal_cmd_EvalCoord2dv;
uint32_t _mesa_unmarshal_EvalCoord2dv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_EvalCoord2dv(const GLdouble *u);
struct marshal_cmd_EvalCoord2f;
uint32_t _mesa_unmarshal_EvalCoord2f(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2f *restrict cmd);
void GLAPIENTRY _mesa_marshal_EvalCoord2f(GLfloat u, GLfloat v);
struct marshal_cmd_EvalCoord2fv;
uint32_t _mesa_unmarshal_EvalCoord2fv(struct gl_context *ctx, const struct marshal_cmd_EvalCoord2fv *restrict cmd);
void GLAPIENTRY _mesa_marshal_EvalCoord2fv(const GLfloat *u);
struct marshal_cmd_EvalMesh1;
uint32_t _mesa_unmarshal_EvalMesh1(struct gl_context *ctx, const struct marshal_cmd_EvalMesh1 *restrict cmd);
struct marshal_cmd_EvalPoint1;
uint32_t _mesa_unmarshal_EvalPoint1(struct gl_context *ctx, const struct marshal_cmd_EvalPoint1 *restrict cmd);
void GLAPIENTRY _mesa_marshal_EvalPoint1(GLint i);
struct marshal_cmd_EvalMesh2;
uint32_t _mesa_unmarshal_EvalMesh2(struct gl_context *ctx, const struct marshal_cmd_EvalMesh2 *restrict cmd);
struct marshal_cmd_EvalPoint2;
uint32_t _mesa_unmarshal_EvalPoint2(struct gl_context *ctx, const struct marshal_cmd_EvalPoint2 *restrict cmd);
void GLAPIENTRY _mesa_marshal_EvalPoint2(GLint i, GLint j);
struct marshal_cmd_AlphaFunc;
uint32_t _mesa_unmarshal_AlphaFunc(struct gl_context *ctx, const struct marshal_cmd_AlphaFunc *restrict cmd);
struct marshal_cmd_BlendFunc;
uint32_t _mesa_unmarshal_BlendFunc(struct gl_context *ctx, const struct marshal_cmd_BlendFunc *restrict cmd);
struct marshal_cmd_LogicOp;
uint32_t _mesa_unmarshal_LogicOp(struct gl_context *ctx, const struct marshal_cmd_LogicOp *restrict cmd);
struct marshal_cmd_StencilFunc;
uint32_t _mesa_unmarshal_StencilFunc(struct gl_context *ctx, const struct marshal_cmd_StencilFunc *restrict cmd);
struct marshal_cmd_StencilOp;
uint32_t _mesa_unmarshal_StencilOp(struct gl_context *ctx, const struct marshal_cmd_StencilOp *restrict cmd);
struct marshal_cmd_DepthFunc;
uint32_t _mesa_unmarshal_DepthFunc(struct gl_context *ctx, const struct marshal_cmd_DepthFunc *restrict cmd);
struct marshal_cmd_PixelZoom;
uint32_t _mesa_unmarshal_PixelZoom(struct gl_context *ctx, const struct marshal_cmd_PixelZoom *restrict cmd);
struct marshal_cmd_PixelTransferf;
uint32_t _mesa_unmarshal_PixelTransferf(struct gl_context *ctx, const struct marshal_cmd_PixelTransferf *restrict cmd);
struct marshal_cmd_PixelTransferi;
uint32_t _mesa_unmarshal_PixelTransferi(struct gl_context *ctx, const struct marshal_cmd_PixelTransferi *restrict cmd);
struct marshal_cmd_PixelStoref;
uint32_t _mesa_unmarshal_PixelStoref(struct gl_context *ctx, const struct marshal_cmd_PixelStoref *restrict cmd);
struct marshal_cmd_PixelStorei;
uint32_t _mesa_unmarshal_PixelStorei(struct gl_context *ctx, const struct marshal_cmd_PixelStorei *restrict cmd);
struct marshal_cmd_PixelMapfv;
uint32_t _mesa_unmarshal_PixelMapfv(struct gl_context *ctx, const struct marshal_cmd_PixelMapfv *restrict cmd);
struct marshal_cmd_PixelMapuiv;
uint32_t _mesa_unmarshal_PixelMapuiv(struct gl_context *ctx, const struct marshal_cmd_PixelMapuiv *restrict cmd);
struct marshal_cmd_PixelMapusv;
uint32_t _mesa_unmarshal_PixelMapusv(struct gl_context *ctx, const struct marshal_cmd_PixelMapusv *restrict cmd);
struct marshal_cmd_ReadBuffer;
uint32_t _mesa_unmarshal_ReadBuffer(struct gl_context *ctx, const struct marshal_cmd_ReadBuffer *restrict cmd);
struct marshal_cmd_CopyPixels;
uint32_t _mesa_unmarshal_CopyPixels(struct gl_context *ctx, const struct marshal_cmd_CopyPixels *restrict cmd);
struct marshal_cmd_ReadPixels;
uint32_t _mesa_unmarshal_ReadPixels(struct gl_context *ctx, const struct marshal_cmd_ReadPixels *restrict cmd);
struct marshal_cmd_DrawPixels;
uint32_t _mesa_unmarshal_DrawPixels(struct gl_context *ctx, const struct marshal_cmd_DrawPixels *restrict cmd);
struct marshal_cmd_GetIntegerv;
uint32_t _mesa_unmarshal_GetIntegerv(struct gl_context *ctx, const struct marshal_cmd_GetIntegerv *restrict cmd);
void GLAPIENTRY _mesa_marshal_GetIntegerv(GLenum pname, GLint *params);
struct marshal_cmd_GetPixelMapfv;
uint32_t _mesa_unmarshal_GetPixelMapfv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapfv *restrict cmd);
struct marshal_cmd_GetPixelMapuiv;
uint32_t _mesa_unmarshal_GetPixelMapuiv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapuiv *restrict cmd);
struct marshal_cmd_GetPixelMapusv;
uint32_t _mesa_unmarshal_GetPixelMapusv(struct gl_context *ctx, const struct marshal_cmd_GetPixelMapusv *restrict cmd);
struct marshal_cmd_GetPolygonStipple;
uint32_t _mesa_unmarshal_GetPolygonStipple(struct gl_context *ctx, const struct marshal_cmd_GetPolygonStipple *restrict cmd);
struct marshal_cmd_GetTexImage;
uint32_t _mesa_unmarshal_GetTexImage(struct gl_context *ctx, const struct marshal_cmd_GetTexImage *restrict cmd);
struct marshal_cmd_DepthRange;
uint32_t _mesa_unmarshal_DepthRange(struct gl_context *ctx, const struct marshal_cmd_DepthRange *restrict cmd);
struct marshal_cmd_Frustum;
uint32_t _mesa_unmarshal_Frustum(struct gl_context *ctx, const struct marshal_cmd_Frustum *restrict cmd);
struct marshal_cmd_LoadIdentity;
uint32_t _mesa_unmarshal_LoadIdentity(struct gl_context *ctx, const struct marshal_cmd_LoadIdentity *restrict cmd);
struct marshal_cmd_LoadMatrixf;
uint32_t _mesa_unmarshal_LoadMatrixf(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixf *restrict cmd);
struct marshal_cmd_LoadMatrixd;
uint32_t _mesa_unmarshal_LoadMatrixd(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixd *restrict cmd);
struct marshal_cmd_MatrixMode;
uint32_t _mesa_unmarshal_MatrixMode(struct gl_context *ctx, const struct marshal_cmd_MatrixMode *restrict cmd);
struct marshal_cmd_MultMatrixf;
uint32_t _mesa_unmarshal_MultMatrixf(struct gl_context *ctx, const struct marshal_cmd_MultMatrixf *restrict cmd);
struct marshal_cmd_MultMatrixd;
uint32_t _mesa_unmarshal_MultMatrixd(struct gl_context *ctx, const struct marshal_cmd_MultMatrixd *restrict cmd);
struct marshal_cmd_Ortho;
uint32_t _mesa_unmarshal_Ortho(struct gl_context *ctx, const struct marshal_cmd_Ortho *restrict cmd);
struct marshal_cmd_PopMatrix;
uint32_t _mesa_unmarshal_PopMatrix(struct gl_context *ctx, const struct marshal_cmd_PopMatrix *restrict cmd);
struct marshal_cmd_PushMatrix;
uint32_t _mesa_unmarshal_PushMatrix(struct gl_context *ctx, const struct marshal_cmd_PushMatrix *restrict cmd);
struct marshal_cmd_Rotated;
uint32_t _mesa_unmarshal_Rotated(struct gl_context *ctx, const struct marshal_cmd_Rotated *restrict cmd);
struct marshal_cmd_Rotatef;
uint32_t _mesa_unmarshal_Rotatef(struct gl_context *ctx, const struct marshal_cmd_Rotatef *restrict cmd);
struct marshal_cmd_Scaled;
uint32_t _mesa_unmarshal_Scaled(struct gl_context *ctx, const struct marshal_cmd_Scaled *restrict cmd);
struct marshal_cmd_Scalef;
uint32_t _mesa_unmarshal_Scalef(struct gl_context *ctx, const struct marshal_cmd_Scalef *restrict cmd);
struct marshal_cmd_Translated;
uint32_t _mesa_unmarshal_Translated(struct gl_context *ctx, const struct marshal_cmd_Translated *restrict cmd);
struct marshal_cmd_Translatef;
uint32_t _mesa_unmarshal_Translatef(struct gl_context *ctx, const struct marshal_cmd_Translatef *restrict cmd);
struct marshal_cmd_Viewport;
uint32_t _mesa_unmarshal_Viewport(struct gl_context *ctx, const struct marshal_cmd_Viewport *restrict cmd);
void GLAPIENTRY _mesa_marshal_ArrayElement(GLint i);
struct marshal_cmd_ColorPointer;
uint32_t _mesa_unmarshal_ColorPointer(struct gl_context *ctx, const struct marshal_cmd_ColorPointer *restrict cmd);
struct marshal_cmd_DisableClientState;
uint32_t _mesa_unmarshal_DisableClientState(struct gl_context *ctx, const struct marshal_cmd_DisableClientState *restrict cmd);
struct marshal_cmd_DrawArrays;
uint32_t _mesa_unmarshal_DrawArrays(struct gl_context *ctx, const struct marshal_cmd_DrawArrays *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawArrays(GLenum mode, GLint first, GLsizei count);
struct marshal_cmd_DrawElements;
uint32_t _mesa_unmarshal_DrawElements(struct gl_context *ctx, const struct marshal_cmd_DrawElements *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
struct marshal_cmd_EdgeFlagPointer;
uint32_t _mesa_unmarshal_EdgeFlagPointer(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagPointer *restrict cmd);
struct marshal_cmd_EnableClientState;
uint32_t _mesa_unmarshal_EnableClientState(struct gl_context *ctx, const struct marshal_cmd_EnableClientState *restrict cmd);
struct marshal_cmd_IndexPointer;
uint32_t _mesa_unmarshal_IndexPointer(struct gl_context *ctx, const struct marshal_cmd_IndexPointer *restrict cmd);
struct marshal_cmd_InterleavedArrays;
uint32_t _mesa_unmarshal_InterleavedArrays(struct gl_context *ctx, const struct marshal_cmd_InterleavedArrays *restrict cmd);
struct marshal_cmd_NormalPointer;
uint32_t _mesa_unmarshal_NormalPointer(struct gl_context *ctx, const struct marshal_cmd_NormalPointer *restrict cmd);
struct marshal_cmd_TexCoordPointer;
uint32_t _mesa_unmarshal_TexCoordPointer(struct gl_context *ctx, const struct marshal_cmd_TexCoordPointer *restrict cmd);
struct marshal_cmd_VertexPointer;
uint32_t _mesa_unmarshal_VertexPointer(struct gl_context *ctx, const struct marshal_cmd_VertexPointer *restrict cmd);
struct marshal_cmd_PolygonOffset;
uint32_t _mesa_unmarshal_PolygonOffset(struct gl_context *ctx, const struct marshal_cmd_PolygonOffset *restrict cmd);
struct marshal_cmd_CopyTexImage1D;
uint32_t _mesa_unmarshal_CopyTexImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTexImage1D *restrict cmd);
struct marshal_cmd_CopyTexImage2D;
uint32_t _mesa_unmarshal_CopyTexImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTexImage2D *restrict cmd);
struct marshal_cmd_CopyTexSubImage1D;
uint32_t _mesa_unmarshal_CopyTexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage1D *restrict cmd);
struct marshal_cmd_CopyTexSubImage2D;
uint32_t _mesa_unmarshal_CopyTexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage2D *restrict cmd);
struct marshal_cmd_TexSubImage1D;
uint32_t _mesa_unmarshal_TexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage1D *restrict cmd);
struct marshal_cmd_TexSubImage2D;
uint32_t _mesa_unmarshal_TexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage2D *restrict cmd);
struct marshal_cmd_BindTexture;
uint32_t _mesa_unmarshal_BindTexture(struct gl_context *ctx, const struct marshal_cmd_BindTexture *restrict cmd);
struct marshal_cmd_DeleteTextures;
uint32_t _mesa_unmarshal_DeleteTextures(struct gl_context *ctx, const struct marshal_cmd_DeleteTextures *restrict cmd);
struct marshal_cmd_PrioritizeTextures;
uint32_t _mesa_unmarshal_PrioritizeTextures(struct gl_context *ctx, const struct marshal_cmd_PrioritizeTextures *restrict cmd);
struct marshal_cmd_Indexub;
uint32_t _mesa_unmarshal_Indexub(struct gl_context *ctx, const struct marshal_cmd_Indexub *restrict cmd);
void GLAPIENTRY _mesa_marshal_Indexub(GLubyte c);
struct marshal_cmd_Indexubv;
uint32_t _mesa_unmarshal_Indexubv(struct gl_context *ctx, const struct marshal_cmd_Indexubv *restrict cmd);
void GLAPIENTRY _mesa_marshal_Indexubv(const GLubyte *c);
struct marshal_cmd_PopClientAttrib;
uint32_t _mesa_unmarshal_PopClientAttrib(struct gl_context *ctx, const struct marshal_cmd_PopClientAttrib *restrict cmd);
struct marshal_cmd_PushClientAttrib;
uint32_t _mesa_unmarshal_PushClientAttrib(struct gl_context *ctx, const struct marshal_cmd_PushClientAttrib *restrict cmd);
struct marshal_cmd_BlendColor;
uint32_t _mesa_unmarshal_BlendColor(struct gl_context *ctx, const struct marshal_cmd_BlendColor *restrict cmd);
struct marshal_cmd_BlendEquation;
uint32_t _mesa_unmarshal_BlendEquation(struct gl_context *ctx, const struct marshal_cmd_BlendEquation *restrict cmd);
struct marshal_cmd_DrawRangeElements;
uint32_t _mesa_unmarshal_DrawRangeElements(struct gl_context *ctx, const struct marshal_cmd_DrawRangeElements *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
struct marshal_cmd_TexImage3D;
uint32_t _mesa_unmarshal_TexImage3D(struct gl_context *ctx, const struct marshal_cmd_TexImage3D *restrict cmd);
struct marshal_cmd_TexSubImage3D;
uint32_t _mesa_unmarshal_TexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_TexSubImage3D *restrict cmd);
struct marshal_cmd_CopyTexSubImage3D;
uint32_t _mesa_unmarshal_CopyTexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CopyTexSubImage3D *restrict cmd);
struct marshal_cmd_ActiveTexture;
uint32_t _mesa_unmarshal_ActiveTexture(struct gl_context *ctx, const struct marshal_cmd_ActiveTexture *restrict cmd);
struct marshal_cmd_ClientActiveTexture;
uint32_t _mesa_unmarshal_ClientActiveTexture(struct gl_context *ctx, const struct marshal_cmd_ClientActiveTexture *restrict cmd);
struct marshal_cmd_MultiTexCoord1d;
uint32_t _mesa_unmarshal_MultiTexCoord1d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1d *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1d(GLenum target, GLdouble s);
struct marshal_cmd_MultiTexCoord1dv;
uint32_t _mesa_unmarshal_MultiTexCoord1dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1dv(GLenum target, const GLdouble *v);
struct marshal_cmd_MultiTexCoord1fARB;
uint32_t _mesa_unmarshal_MultiTexCoord1fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1fARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1fARB(GLenum target, GLfloat s);
struct marshal_cmd_MultiTexCoord1fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord1fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1fvARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1fvARB(GLenum target, const GLfloat *v);
struct marshal_cmd_MultiTexCoord1i;
uint32_t _mesa_unmarshal_MultiTexCoord1i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1i *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1i(GLenum target, GLint s);
struct marshal_cmd_MultiTexCoord1iv;
uint32_t _mesa_unmarshal_MultiTexCoord1iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1iv(GLenum target, const GLint *v);
struct marshal_cmd_MultiTexCoord1s;
uint32_t _mesa_unmarshal_MultiTexCoord1s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1s *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1s(GLenum target, GLshort s);
struct marshal_cmd_MultiTexCoord1sv;
uint32_t _mesa_unmarshal_MultiTexCoord1sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1sv(GLenum target, const GLshort *v);
struct marshal_cmd_MultiTexCoord2d;
uint32_t _mesa_unmarshal_MultiTexCoord2d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2d *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2d(GLenum target, GLdouble s, GLdouble t);
struct marshal_cmd_MultiTexCoord2dv;
uint32_t _mesa_unmarshal_MultiTexCoord2dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2dv(GLenum target, const GLdouble *v);
struct marshal_cmd_MultiTexCoord2fARB;
uint32_t _mesa_unmarshal_MultiTexCoord2fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2fARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2fARB(GLenum target, GLfloat s, GLfloat t);
struct marshal_cmd_MultiTexCoord2fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord2fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2fvARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2fvARB(GLenum target, const GLfloat *v);
struct marshal_cmd_MultiTexCoord2i;
uint32_t _mesa_unmarshal_MultiTexCoord2i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2i *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2i(GLenum target, GLint s, GLint t);
struct marshal_cmd_MultiTexCoord2iv;
uint32_t _mesa_unmarshal_MultiTexCoord2iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2iv(GLenum target, const GLint *v);
struct marshal_cmd_MultiTexCoord2s;
uint32_t _mesa_unmarshal_MultiTexCoord2s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2s *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2s(GLenum target, GLshort s, GLshort t);
struct marshal_cmd_MultiTexCoord2sv;
uint32_t _mesa_unmarshal_MultiTexCoord2sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2sv(GLenum target, const GLshort *v);
struct marshal_cmd_MultiTexCoord3d;
uint32_t _mesa_unmarshal_MultiTexCoord3d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3d *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3d(GLenum target, GLdouble s, GLdouble t, GLdouble r);
struct marshal_cmd_MultiTexCoord3dv;
uint32_t _mesa_unmarshal_MultiTexCoord3dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3dv(GLenum target, const GLdouble *v);
struct marshal_cmd_MultiTexCoord3fARB;
uint32_t _mesa_unmarshal_MultiTexCoord3fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3fARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r);
struct marshal_cmd_MultiTexCoord3fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord3fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3fvARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3fvARB(GLenum target, const GLfloat *v);
struct marshal_cmd_MultiTexCoord3i;
uint32_t _mesa_unmarshal_MultiTexCoord3i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3i *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3i(GLenum target, GLint s, GLint t, GLint r);
struct marshal_cmd_MultiTexCoord3iv;
uint32_t _mesa_unmarshal_MultiTexCoord3iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3iv(GLenum target, const GLint *v);
struct marshal_cmd_MultiTexCoord3s;
uint32_t _mesa_unmarshal_MultiTexCoord3s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3s *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3s(GLenum target, GLshort s, GLshort t, GLshort r);
struct marshal_cmd_MultiTexCoord3sv;
uint32_t _mesa_unmarshal_MultiTexCoord3sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3sv(GLenum target, const GLshort *v);
struct marshal_cmd_MultiTexCoord4d;
uint32_t _mesa_unmarshal_MultiTexCoord4d(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4d *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4d(GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
struct marshal_cmd_MultiTexCoord4dv;
uint32_t _mesa_unmarshal_MultiTexCoord4dv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4dv(GLenum target, const GLdouble *v);
struct marshal_cmd_MultiTexCoord4fARB;
uint32_t _mesa_unmarshal_MultiTexCoord4fARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4fARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4fARB(GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
struct marshal_cmd_MultiTexCoord4fvARB;
uint32_t _mesa_unmarshal_MultiTexCoord4fvARB(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4fvARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4fvARB(GLenum target, const GLfloat *v);
struct marshal_cmd_MultiTexCoord4i;
uint32_t _mesa_unmarshal_MultiTexCoord4i(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4i *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4i(GLenum target, GLint s, GLint t, GLint r, GLint q);
struct marshal_cmd_MultiTexCoord4iv;
uint32_t _mesa_unmarshal_MultiTexCoord4iv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4iv(GLenum target, const GLint *v);
struct marshal_cmd_MultiTexCoord4s;
uint32_t _mesa_unmarshal_MultiTexCoord4s(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4s *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4s(GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
struct marshal_cmd_MultiTexCoord4sv;
uint32_t _mesa_unmarshal_MultiTexCoord4sv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4sv(GLenum target, const GLshort *v);
struct marshal_cmd_LoadTransposeMatrixf;
uint32_t _mesa_unmarshal_LoadTransposeMatrixf(struct gl_context *ctx, const struct marshal_cmd_LoadTransposeMatrixf *restrict cmd);
struct marshal_cmd_LoadTransposeMatrixd;
uint32_t _mesa_unmarshal_LoadTransposeMatrixd(struct gl_context *ctx, const struct marshal_cmd_LoadTransposeMatrixd *restrict cmd);
struct marshal_cmd_MultTransposeMatrixf;
uint32_t _mesa_unmarshal_MultTransposeMatrixf(struct gl_context *ctx, const struct marshal_cmd_MultTransposeMatrixf *restrict cmd);
struct marshal_cmd_MultTransposeMatrixd;
uint32_t _mesa_unmarshal_MultTransposeMatrixd(struct gl_context *ctx, const struct marshal_cmd_MultTransposeMatrixd *restrict cmd);
struct marshal_cmd_SampleCoverage;
uint32_t _mesa_unmarshal_SampleCoverage(struct gl_context *ctx, const struct marshal_cmd_SampleCoverage *restrict cmd);
struct marshal_cmd_CompressedTexImage3D;
uint32_t _mesa_unmarshal_CompressedTexImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage3D *restrict cmd);
struct marshal_cmd_CompressedTexImage2D;
uint32_t _mesa_unmarshal_CompressedTexImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage2D *restrict cmd);
struct marshal_cmd_CompressedTexImage1D;
uint32_t _mesa_unmarshal_CompressedTexImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexImage1D *restrict cmd);
struct marshal_cmd_CompressedTexSubImage3D;
uint32_t _mesa_unmarshal_CompressedTexSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage3D *restrict cmd);
struct marshal_cmd_CompressedTexSubImage2D;
uint32_t _mesa_unmarshal_CompressedTexSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage2D *restrict cmd);
struct marshal_cmd_CompressedTexSubImage1D;
uint32_t _mesa_unmarshal_CompressedTexSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTexSubImage1D *restrict cmd);
struct marshal_cmd_GetCompressedTexImage;
uint32_t _mesa_unmarshal_GetCompressedTexImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTexImage *restrict cmd);
struct marshal_cmd_BlendFuncSeparate;
uint32_t _mesa_unmarshal_BlendFuncSeparate(struct gl_context *ctx, const struct marshal_cmd_BlendFuncSeparate *restrict cmd);
struct marshal_cmd_FogCoordfEXT;
uint32_t _mesa_unmarshal_FogCoordfEXT(struct gl_context *ctx, const struct marshal_cmd_FogCoordfEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_FogCoordfEXT(GLfloat coord);
struct marshal_cmd_FogCoordfvEXT;
uint32_t _mesa_unmarshal_FogCoordfvEXT(struct gl_context *ctx, const struct marshal_cmd_FogCoordfvEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_FogCoordfvEXT(const GLfloat *coord);
struct marshal_cmd_FogCoordd;
uint32_t _mesa_unmarshal_FogCoordd(struct gl_context *ctx, const struct marshal_cmd_FogCoordd *restrict cmd);
void GLAPIENTRY _mesa_marshal_FogCoordd(GLdouble coord);
struct marshal_cmd_FogCoorddv;
uint32_t _mesa_unmarshal_FogCoorddv(struct gl_context *ctx, const struct marshal_cmd_FogCoorddv *restrict cmd);
void GLAPIENTRY _mesa_marshal_FogCoorddv(const GLdouble *coord);
struct marshal_cmd_FogCoordPointer;
uint32_t _mesa_unmarshal_FogCoordPointer(struct gl_context *ctx, const struct marshal_cmd_FogCoordPointer *restrict cmd);
struct marshal_cmd_MultiDrawArrays;
uint32_t _mesa_unmarshal_MultiDrawArrays(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArrays *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawArrays(GLenum mode, const GLint *first, const GLsizei *count, GLsizei primcount);
struct marshal_cmd_MultiDrawElements;
uint32_t _mesa_unmarshal_MultiDrawElements(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElements *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawElements(GLenum mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount);
struct marshal_cmd_PointParameterf;
uint32_t _mesa_unmarshal_PointParameterf(struct gl_context *ctx, const struct marshal_cmd_PointParameterf *restrict cmd);
struct marshal_cmd_PointParameterfv;
uint32_t _mesa_unmarshal_PointParameterfv(struct gl_context *ctx, const struct marshal_cmd_PointParameterfv *restrict cmd);
struct marshal_cmd_PointParameteri;
uint32_t _mesa_unmarshal_PointParameteri(struct gl_context *ctx, const struct marshal_cmd_PointParameteri *restrict cmd);
struct marshal_cmd_PointParameteriv;
uint32_t _mesa_unmarshal_PointParameteriv(struct gl_context *ctx, const struct marshal_cmd_PointParameteriv *restrict cmd);
struct marshal_cmd_SecondaryColor3b;
uint32_t _mesa_unmarshal_SecondaryColor3b(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3b *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3b(GLbyte red, GLbyte green, GLbyte blue);
struct marshal_cmd_SecondaryColor3bv;
uint32_t _mesa_unmarshal_SecondaryColor3bv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3bv *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3bv(const GLbyte *v);
struct marshal_cmd_SecondaryColor3d;
uint32_t _mesa_unmarshal_SecondaryColor3d(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3d *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3d(GLdouble red, GLdouble green, GLdouble blue);
struct marshal_cmd_SecondaryColor3dv;
uint32_t _mesa_unmarshal_SecondaryColor3dv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3dv(const GLdouble *v);
struct marshal_cmd_SecondaryColor3fEXT;
uint32_t _mesa_unmarshal_SecondaryColor3fEXT(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3fEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3fEXT(GLfloat red, GLfloat green, GLfloat blue);
struct marshal_cmd_SecondaryColor3fvEXT;
uint32_t _mesa_unmarshal_SecondaryColor3fvEXT(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3fvEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3fvEXT(const GLfloat *v);
struct marshal_cmd_SecondaryColor3i;
uint32_t _mesa_unmarshal_SecondaryColor3i(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3i *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3i(GLint red, GLint green, GLint blue);
struct marshal_cmd_SecondaryColor3iv;
uint32_t _mesa_unmarshal_SecondaryColor3iv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3iv(const GLint *v);
struct marshal_cmd_SecondaryColor3s;
uint32_t _mesa_unmarshal_SecondaryColor3s(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3s *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3s(GLshort red, GLshort green, GLshort blue);
struct marshal_cmd_SecondaryColor3sv;
uint32_t _mesa_unmarshal_SecondaryColor3sv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3sv(const GLshort *v);
struct marshal_cmd_SecondaryColor3ub;
uint32_t _mesa_unmarshal_SecondaryColor3ub(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ub *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3ub(GLubyte red, GLubyte green, GLubyte blue);
struct marshal_cmd_SecondaryColor3ubv;
uint32_t _mesa_unmarshal_SecondaryColor3ubv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ubv *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3ubv(const GLubyte *v);
struct marshal_cmd_SecondaryColor3ui;
uint32_t _mesa_unmarshal_SecondaryColor3ui(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3ui(GLuint red, GLuint green, GLuint blue);
struct marshal_cmd_SecondaryColor3uiv;
uint32_t _mesa_unmarshal_SecondaryColor3uiv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3uiv(const GLuint *v);
struct marshal_cmd_SecondaryColor3us;
uint32_t _mesa_unmarshal_SecondaryColor3us(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3us *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3us(GLushort red, GLushort green, GLushort blue);
struct marshal_cmd_SecondaryColor3usv;
uint32_t _mesa_unmarshal_SecondaryColor3usv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3usv *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3usv(const GLushort *v);
struct marshal_cmd_SecondaryColorPointer;
uint32_t _mesa_unmarshal_SecondaryColorPointer(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorPointer *restrict cmd);
struct marshal_cmd_WindowPos2d;
uint32_t _mesa_unmarshal_WindowPos2d(struct gl_context *ctx, const struct marshal_cmd_WindowPos2d *restrict cmd);
struct marshal_cmd_WindowPos2dv;
uint32_t _mesa_unmarshal_WindowPos2dv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2dv *restrict cmd);
struct marshal_cmd_WindowPos2f;
uint32_t _mesa_unmarshal_WindowPos2f(struct gl_context *ctx, const struct marshal_cmd_WindowPos2f *restrict cmd);
struct marshal_cmd_WindowPos2fv;
uint32_t _mesa_unmarshal_WindowPos2fv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2fv *restrict cmd);
struct marshal_cmd_WindowPos2i;
uint32_t _mesa_unmarshal_WindowPos2i(struct gl_context *ctx, const struct marshal_cmd_WindowPos2i *restrict cmd);
struct marshal_cmd_WindowPos2iv;
uint32_t _mesa_unmarshal_WindowPos2iv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2iv *restrict cmd);
struct marshal_cmd_WindowPos2s;
uint32_t _mesa_unmarshal_WindowPos2s(struct gl_context *ctx, const struct marshal_cmd_WindowPos2s *restrict cmd);
struct marshal_cmd_WindowPos2sv;
uint32_t _mesa_unmarshal_WindowPos2sv(struct gl_context *ctx, const struct marshal_cmd_WindowPos2sv *restrict cmd);
struct marshal_cmd_WindowPos3d;
uint32_t _mesa_unmarshal_WindowPos3d(struct gl_context *ctx, const struct marshal_cmd_WindowPos3d *restrict cmd);
struct marshal_cmd_WindowPos3dv;
uint32_t _mesa_unmarshal_WindowPos3dv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3dv *restrict cmd);
struct marshal_cmd_WindowPos3f;
uint32_t _mesa_unmarshal_WindowPos3f(struct gl_context *ctx, const struct marshal_cmd_WindowPos3f *restrict cmd);
struct marshal_cmd_WindowPos3fv;
uint32_t _mesa_unmarshal_WindowPos3fv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3fv *restrict cmd);
struct marshal_cmd_WindowPos3i;
uint32_t _mesa_unmarshal_WindowPos3i(struct gl_context *ctx, const struct marshal_cmd_WindowPos3i *restrict cmd);
struct marshal_cmd_WindowPos3iv;
uint32_t _mesa_unmarshal_WindowPos3iv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3iv *restrict cmd);
struct marshal_cmd_WindowPos3s;
uint32_t _mesa_unmarshal_WindowPos3s(struct gl_context *ctx, const struct marshal_cmd_WindowPos3s *restrict cmd);
struct marshal_cmd_WindowPos3sv;
uint32_t _mesa_unmarshal_WindowPos3sv(struct gl_context *ctx, const struct marshal_cmd_WindowPos3sv *restrict cmd);
struct marshal_cmd_BindBuffer;
uint32_t _mesa_unmarshal_BindBuffer(struct gl_context *ctx, const struct marshal_cmd_BindBuffer *restrict cmd);
void GLAPIENTRY _mesa_marshal_BindBuffer(GLenum target, GLuint buffer);
struct marshal_cmd_BufferData;
uint32_t _mesa_unmarshal_BufferData(struct gl_context *ctx, const struct marshal_cmd_BufferData *restrict cmd);
void GLAPIENTRY _mesa_marshal_BufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
struct marshal_cmd_BufferSubData;
uint32_t _mesa_unmarshal_BufferSubData(struct gl_context *ctx, const struct marshal_cmd_BufferSubData *restrict cmd);
void GLAPIENTRY _mesa_marshal_BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
struct marshal_cmd_DeleteBuffers;
uint32_t _mesa_unmarshal_DeleteBuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteBuffers *restrict cmd);
struct marshal_cmd_UnmapBuffer;
uint32_t _mesa_unmarshal_UnmapBuffer(struct gl_context *ctx, const struct marshal_cmd_UnmapBuffer *restrict cmd);
struct marshal_cmd_DeleteQueries;
uint32_t _mesa_unmarshal_DeleteQueries(struct gl_context *ctx, const struct marshal_cmd_DeleteQueries *restrict cmd);
struct marshal_cmd_BeginQuery;
uint32_t _mesa_unmarshal_BeginQuery(struct gl_context *ctx, const struct marshal_cmd_BeginQuery *restrict cmd);
struct marshal_cmd_EndQuery;
uint32_t _mesa_unmarshal_EndQuery(struct gl_context *ctx, const struct marshal_cmd_EndQuery *restrict cmd);
struct marshal_cmd_BlendEquationSeparate;
uint32_t _mesa_unmarshal_BlendEquationSeparate(struct gl_context *ctx, const struct marshal_cmd_BlendEquationSeparate *restrict cmd);
struct marshal_cmd_DrawBuffers;
uint32_t _mesa_unmarshal_DrawBuffers(struct gl_context *ctx, const struct marshal_cmd_DrawBuffers *restrict cmd);
struct marshal_cmd_StencilFuncSeparate;
uint32_t _mesa_unmarshal_StencilFuncSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilFuncSeparate *restrict cmd);
struct marshal_cmd_StencilOpSeparate;
uint32_t _mesa_unmarshal_StencilOpSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilOpSeparate *restrict cmd);
struct marshal_cmd_StencilMaskSeparate;
uint32_t _mesa_unmarshal_StencilMaskSeparate(struct gl_context *ctx, const struct marshal_cmd_StencilMaskSeparate *restrict cmd);
struct marshal_cmd_AttachShader;
uint32_t _mesa_unmarshal_AttachShader(struct gl_context *ctx, const struct marshal_cmd_AttachShader *restrict cmd);
struct marshal_cmd_BindAttribLocation;
uint32_t _mesa_unmarshal_BindAttribLocation(struct gl_context *ctx, const struct marshal_cmd_BindAttribLocation *restrict cmd);
struct marshal_cmd_CompileShader;
uint32_t _mesa_unmarshal_CompileShader(struct gl_context *ctx, const struct marshal_cmd_CompileShader *restrict cmd);
struct marshal_cmd_DeleteProgram;
uint32_t _mesa_unmarshal_DeleteProgram(struct gl_context *ctx, const struct marshal_cmd_DeleteProgram *restrict cmd);
struct marshal_cmd_DeleteShader;
uint32_t _mesa_unmarshal_DeleteShader(struct gl_context *ctx, const struct marshal_cmd_DeleteShader *restrict cmd);
struct marshal_cmd_DetachShader;
uint32_t _mesa_unmarshal_DetachShader(struct gl_context *ctx, const struct marshal_cmd_DetachShader *restrict cmd);
struct marshal_cmd_DisableVertexAttribArray;
uint32_t _mesa_unmarshal_DisableVertexAttribArray(struct gl_context *ctx, const struct marshal_cmd_DisableVertexAttribArray *restrict cmd);
struct marshal_cmd_EnableVertexAttribArray;
uint32_t _mesa_unmarshal_EnableVertexAttribArray(struct gl_context *ctx, const struct marshal_cmd_EnableVertexAttribArray *restrict cmd);
struct marshal_cmd_GetActiveUniform;
uint32_t _mesa_unmarshal_GetActiveUniform(struct gl_context *ctx, const struct marshal_cmd_GetActiveUniform *restrict cmd);
void GLAPIENTRY _mesa_marshal_GetActiveUniform(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
struct marshal_cmd_GetUniformLocation;
uint32_t _mesa_unmarshal_GetUniformLocation(struct gl_context *ctx, const struct marshal_cmd_GetUniformLocation *restrict cmd);
GLint GLAPIENTRY _mesa_marshal_GetUniformLocation(GLuint program, const GLchar *name);
struct marshal_cmd_LinkProgram;
uint32_t _mesa_unmarshal_LinkProgram(struct gl_context *ctx, const struct marshal_cmd_LinkProgram *restrict cmd);
struct marshal_cmd_UseProgram;
uint32_t _mesa_unmarshal_UseProgram(struct gl_context *ctx, const struct marshal_cmd_UseProgram *restrict cmd);
struct marshal_cmd_Uniform1f;
uint32_t _mesa_unmarshal_Uniform1f(struct gl_context *ctx, const struct marshal_cmd_Uniform1f *restrict cmd);
struct marshal_cmd_Uniform2f;
uint32_t _mesa_unmarshal_Uniform2f(struct gl_context *ctx, const struct marshal_cmd_Uniform2f *restrict cmd);
struct marshal_cmd_Uniform3f;
uint32_t _mesa_unmarshal_Uniform3f(struct gl_context *ctx, const struct marshal_cmd_Uniform3f *restrict cmd);
struct marshal_cmd_Uniform4f;
uint32_t _mesa_unmarshal_Uniform4f(struct gl_context *ctx, const struct marshal_cmd_Uniform4f *restrict cmd);
struct marshal_cmd_Uniform1i;
uint32_t _mesa_unmarshal_Uniform1i(struct gl_context *ctx, const struct marshal_cmd_Uniform1i *restrict cmd);
struct marshal_cmd_Uniform2i;
uint32_t _mesa_unmarshal_Uniform2i(struct gl_context *ctx, const struct marshal_cmd_Uniform2i *restrict cmd);
struct marshal_cmd_Uniform3i;
uint32_t _mesa_unmarshal_Uniform3i(struct gl_context *ctx, const struct marshal_cmd_Uniform3i *restrict cmd);
struct marshal_cmd_Uniform4i;
uint32_t _mesa_unmarshal_Uniform4i(struct gl_context *ctx, const struct marshal_cmd_Uniform4i *restrict cmd);
struct marshal_cmd_Uniform1fv;
uint32_t _mesa_unmarshal_Uniform1fv(struct gl_context *ctx, const struct marshal_cmd_Uniform1fv *restrict cmd);
struct marshal_cmd_Uniform2fv;
uint32_t _mesa_unmarshal_Uniform2fv(struct gl_context *ctx, const struct marshal_cmd_Uniform2fv *restrict cmd);
struct marshal_cmd_Uniform3fv;
uint32_t _mesa_unmarshal_Uniform3fv(struct gl_context *ctx, const struct marshal_cmd_Uniform3fv *restrict cmd);
struct marshal_cmd_Uniform4fv;
uint32_t _mesa_unmarshal_Uniform4fv(struct gl_context *ctx, const struct marshal_cmd_Uniform4fv *restrict cmd);
struct marshal_cmd_Uniform1iv;
uint32_t _mesa_unmarshal_Uniform1iv(struct gl_context *ctx, const struct marshal_cmd_Uniform1iv *restrict cmd);
struct marshal_cmd_Uniform2iv;
uint32_t _mesa_unmarshal_Uniform2iv(struct gl_context *ctx, const struct marshal_cmd_Uniform2iv *restrict cmd);
struct marshal_cmd_Uniform3iv;
uint32_t _mesa_unmarshal_Uniform3iv(struct gl_context *ctx, const struct marshal_cmd_Uniform3iv *restrict cmd);
struct marshal_cmd_Uniform4iv;
uint32_t _mesa_unmarshal_Uniform4iv(struct gl_context *ctx, const struct marshal_cmd_Uniform4iv *restrict cmd);
struct marshal_cmd_UniformMatrix2fv;
uint32_t _mesa_unmarshal_UniformMatrix2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2fv *restrict cmd);
struct marshal_cmd_UniformMatrix3fv;
uint32_t _mesa_unmarshal_UniformMatrix3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3fv *restrict cmd);
struct marshal_cmd_UniformMatrix4fv;
uint32_t _mesa_unmarshal_UniformMatrix4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4fv *restrict cmd);
struct marshal_cmd_ValidateProgram;
uint32_t _mesa_unmarshal_ValidateProgram(struct gl_context *ctx, const struct marshal_cmd_ValidateProgram *restrict cmd);
struct marshal_cmd_VertexAttrib1d;
uint32_t _mesa_unmarshal_VertexAttrib1d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1d *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1d(GLuint index, GLdouble x);
struct marshal_cmd_VertexAttrib1dv;
uint32_t _mesa_unmarshal_VertexAttrib1dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1dv(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttrib1fARB;
uint32_t _mesa_unmarshal_VertexAttrib1fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1fARB(GLuint index, GLfloat x);
struct marshal_cmd_VertexAttrib1fvARB;
uint32_t _mesa_unmarshal_VertexAttrib1fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fvARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1fvARB(GLuint index, const GLfloat *v);
struct marshal_cmd_VertexAttrib1s;
uint32_t _mesa_unmarshal_VertexAttrib1s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1s *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1s(GLuint index, GLshort x);
struct marshal_cmd_VertexAttrib1sv;
uint32_t _mesa_unmarshal_VertexAttrib1sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1sv(GLuint index, const GLshort *v);
struct marshal_cmd_VertexAttrib2d;
uint32_t _mesa_unmarshal_VertexAttrib2d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2d *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2d(GLuint index, GLdouble x, GLdouble y);
struct marshal_cmd_VertexAttrib2dv;
uint32_t _mesa_unmarshal_VertexAttrib2dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2dv(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttrib2fARB;
uint32_t _mesa_unmarshal_VertexAttrib2fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2fARB(GLuint index, GLfloat x, GLfloat y);
struct marshal_cmd_VertexAttrib2fvARB;
uint32_t _mesa_unmarshal_VertexAttrib2fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fvARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2fvARB(GLuint index, const GLfloat *v);
struct marshal_cmd_VertexAttrib2s;
uint32_t _mesa_unmarshal_VertexAttrib2s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2s *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2s(GLuint index, GLshort x, GLshort y);
struct marshal_cmd_VertexAttrib2sv;
uint32_t _mesa_unmarshal_VertexAttrib2sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2sv(GLuint index, const GLshort *v);
struct marshal_cmd_VertexAttrib3d;
uint32_t _mesa_unmarshal_VertexAttrib3d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3d *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3d(GLuint index, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_VertexAttrib3dv;
uint32_t _mesa_unmarshal_VertexAttrib3dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3dv(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttrib3fARB;
uint32_t _mesa_unmarshal_VertexAttrib3fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_VertexAttrib3fvARB;
uint32_t _mesa_unmarshal_VertexAttrib3fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fvARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3fvARB(GLuint index, const GLfloat *v);
struct marshal_cmd_VertexAttrib3s;
uint32_t _mesa_unmarshal_VertexAttrib3s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3s *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3s(GLuint index, GLshort x, GLshort y, GLshort z);
struct marshal_cmd_VertexAttrib3sv;
uint32_t _mesa_unmarshal_VertexAttrib3sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3sv(GLuint index, const GLshort *v);
struct marshal_cmd_VertexAttrib4Nbv;
uint32_t _mesa_unmarshal_VertexAttrib4Nbv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nbv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nbv(GLuint index, const GLbyte *v);
struct marshal_cmd_VertexAttrib4Niv;
uint32_t _mesa_unmarshal_VertexAttrib4Niv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Niv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Niv(GLuint index, const GLint *v);
struct marshal_cmd_VertexAttrib4Nsv;
uint32_t _mesa_unmarshal_VertexAttrib4Nsv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nsv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nsv(GLuint index, const GLshort *v);
struct marshal_cmd_VertexAttrib4Nub;
uint32_t _mesa_unmarshal_VertexAttrib4Nub(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nub *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nub(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
struct marshal_cmd_VertexAttrib4Nubv;
uint32_t _mesa_unmarshal_VertexAttrib4Nubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nubv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nubv(GLuint index, const GLubyte *v);
struct marshal_cmd_VertexAttrib4Nuiv;
uint32_t _mesa_unmarshal_VertexAttrib4Nuiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nuiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nuiv(GLuint index, const GLuint *v);
struct marshal_cmd_VertexAttrib4Nusv;
uint32_t _mesa_unmarshal_VertexAttrib4Nusv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4Nusv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4Nusv(GLuint index, const GLushort *v);
struct marshal_cmd_VertexAttrib4bv;
uint32_t _mesa_unmarshal_VertexAttrib4bv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4bv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4bv(GLuint index, const GLbyte *v);
struct marshal_cmd_VertexAttrib4d;
uint32_t _mesa_unmarshal_VertexAttrib4d(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4d *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_VertexAttrib4dv;
uint32_t _mesa_unmarshal_VertexAttrib4dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4dv(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttrib4fARB;
uint32_t _mesa_unmarshal_VertexAttrib4fARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4fARB(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_VertexAttrib4fvARB;
uint32_t _mesa_unmarshal_VertexAttrib4fvARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fvARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4fvARB(GLuint index, const GLfloat *v);
struct marshal_cmd_VertexAttrib4iv;
uint32_t _mesa_unmarshal_VertexAttrib4iv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4iv(GLuint index, const GLint *v);
struct marshal_cmd_VertexAttrib4s;
uint32_t _mesa_unmarshal_VertexAttrib4s(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4s *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4s(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
struct marshal_cmd_VertexAttrib4sv;
uint32_t _mesa_unmarshal_VertexAttrib4sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4sv(GLuint index, const GLshort *v);
struct marshal_cmd_VertexAttrib4ubv;
uint32_t _mesa_unmarshal_VertexAttrib4ubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4ubv(GLuint index, const GLubyte *v);
struct marshal_cmd_VertexAttrib4uiv;
uint32_t _mesa_unmarshal_VertexAttrib4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4uiv(GLuint index, const GLuint *v);
struct marshal_cmd_VertexAttrib4usv;
uint32_t _mesa_unmarshal_VertexAttrib4usv(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4usv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4usv(GLuint index, const GLushort *v);
struct marshal_cmd_VertexAttribPointer;
uint32_t _mesa_unmarshal_VertexAttribPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribPointer *restrict cmd);
struct marshal_cmd_UniformMatrix2x3fv;
uint32_t _mesa_unmarshal_UniformMatrix2x3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x3fv *restrict cmd);
struct marshal_cmd_UniformMatrix3x2fv;
uint32_t _mesa_unmarshal_UniformMatrix3x2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x2fv *restrict cmd);
struct marshal_cmd_UniformMatrix2x4fv;
uint32_t _mesa_unmarshal_UniformMatrix2x4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x4fv *restrict cmd);
struct marshal_cmd_UniformMatrix4x2fv;
uint32_t _mesa_unmarshal_UniformMatrix4x2fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x2fv *restrict cmd);
struct marshal_cmd_UniformMatrix3x4fv;
uint32_t _mesa_unmarshal_UniformMatrix3x4fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x4fv *restrict cmd);
struct marshal_cmd_UniformMatrix4x3fv;
uint32_t _mesa_unmarshal_UniformMatrix4x3fv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x3fv *restrict cmd);
struct marshal_cmd_ProgramStringARB;
uint32_t _mesa_unmarshal_ProgramStringARB(struct gl_context *ctx, const struct marshal_cmd_ProgramStringARB *restrict cmd);
struct marshal_cmd_BindProgramARB;
uint32_t _mesa_unmarshal_BindProgramARB(struct gl_context *ctx, const struct marshal_cmd_BindProgramARB *restrict cmd);
struct marshal_cmd_DeleteProgramsARB;
uint32_t _mesa_unmarshal_DeleteProgramsARB(struct gl_context *ctx, const struct marshal_cmd_DeleteProgramsARB *restrict cmd);
struct marshal_cmd_ProgramEnvParameter4dARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4dARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4dARB *restrict cmd);
struct marshal_cmd_ProgramEnvParameter4dvARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4dvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4dvARB *restrict cmd);
struct marshal_cmd_ProgramEnvParameter4fARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4fARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4fARB *restrict cmd);
struct marshal_cmd_ProgramEnvParameter4fvARB;
uint32_t _mesa_unmarshal_ProgramEnvParameter4fvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameter4fvARB *restrict cmd);
struct marshal_cmd_ProgramLocalParameter4dARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4dARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4dARB *restrict cmd);
struct marshal_cmd_ProgramLocalParameter4dvARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4dvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4dvARB *restrict cmd);
struct marshal_cmd_ProgramLocalParameter4fARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4fARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4fARB *restrict cmd);
struct marshal_cmd_ProgramLocalParameter4fvARB;
uint32_t _mesa_unmarshal_ProgramLocalParameter4fvARB(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameter4fvARB *restrict cmd);
struct marshal_cmd_DeleteObjectARB;
uint32_t _mesa_unmarshal_DeleteObjectARB(struct gl_context *ctx, const struct marshal_cmd_DeleteObjectARB *restrict cmd);
struct marshal_cmd_DetachObjectARB;
uint32_t _mesa_unmarshal_DetachObjectARB(struct gl_context *ctx, const struct marshal_cmd_DetachObjectARB *restrict cmd);
struct marshal_cmd_AttachObjectARB;
uint32_t _mesa_unmarshal_AttachObjectARB(struct gl_context *ctx, const struct marshal_cmd_AttachObjectARB *restrict cmd);
struct marshal_cmd_ClampColor;
uint32_t _mesa_unmarshal_ClampColor(struct gl_context *ctx, const struct marshal_cmd_ClampColor *restrict cmd);
struct marshal_cmd_DrawArraysInstanced;
uint32_t _mesa_unmarshal_DrawArraysInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawArraysInstanced *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawArraysInstanced(GLenum mode, GLint first, GLsizei count, GLsizei primcount);
struct marshal_cmd_DrawElementsInstanced;
uint32_t _mesa_unmarshal_DrawElementsInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstanced *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsInstanced(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);
struct marshal_cmd_BindRenderbuffer;
uint32_t _mesa_unmarshal_BindRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_BindRenderbuffer *restrict cmd);
struct marshal_cmd_DeleteRenderbuffers;
uint32_t _mesa_unmarshal_DeleteRenderbuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteRenderbuffers *restrict cmd);
struct marshal_cmd_RenderbufferStorage;
uint32_t _mesa_unmarshal_RenderbufferStorage(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorage *restrict cmd);
struct marshal_cmd_RenderbufferStorageMultisample;
uint32_t _mesa_unmarshal_RenderbufferStorageMultisample(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorageMultisample *restrict cmd);
struct marshal_cmd_BindFramebuffer;
uint32_t _mesa_unmarshal_BindFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BindFramebuffer *restrict cmd);
struct marshal_cmd_DeleteFramebuffers;
uint32_t _mesa_unmarshal_DeleteFramebuffers(struct gl_context *ctx, const struct marshal_cmd_DeleteFramebuffers *restrict cmd);
struct marshal_cmd_FramebufferTexture1D;
uint32_t _mesa_unmarshal_FramebufferTexture1D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture1D *restrict cmd);
struct marshal_cmd_FramebufferTexture2D;
uint32_t _mesa_unmarshal_FramebufferTexture2D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2D *restrict cmd);
struct marshal_cmd_FramebufferTexture3D;
uint32_t _mesa_unmarshal_FramebufferTexture3D(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture3D *restrict cmd);
struct marshal_cmd_FramebufferTextureLayer;
uint32_t _mesa_unmarshal_FramebufferTextureLayer(struct gl_context *ctx, const struct marshal_cmd_FramebufferTextureLayer *restrict cmd);
struct marshal_cmd_FramebufferRenderbuffer;
uint32_t _mesa_unmarshal_FramebufferRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_FramebufferRenderbuffer *restrict cmd);
struct marshal_cmd_BlitFramebuffer;
uint32_t _mesa_unmarshal_BlitFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BlitFramebuffer *restrict cmd);
struct marshal_cmd_GenerateMipmap;
uint32_t _mesa_unmarshal_GenerateMipmap(struct gl_context *ctx, const struct marshal_cmd_GenerateMipmap *restrict cmd);
struct marshal_cmd_VertexAttribDivisor;
uint32_t _mesa_unmarshal_VertexAttribDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexAttribDivisor *restrict cmd);
struct marshal_cmd_VertexArrayVertexAttribDivisorEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribDivisorEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribDivisorEXT *restrict cmd);
struct marshal_cmd_FlushMappedBufferRange;
uint32_t _mesa_unmarshal_FlushMappedBufferRange(struct gl_context *ctx, const struct marshal_cmd_FlushMappedBufferRange *restrict cmd);
struct marshal_cmd_TexBuffer;
uint32_t _mesa_unmarshal_TexBuffer(struct gl_context *ctx, const struct marshal_cmd_TexBuffer *restrict cmd);
struct marshal_cmd_BindVertexArray;
uint32_t _mesa_unmarshal_BindVertexArray(struct gl_context *ctx, const struct marshal_cmd_BindVertexArray *restrict cmd);
struct marshal_cmd_DeleteVertexArrays;
uint32_t _mesa_unmarshal_DeleteVertexArrays(struct gl_context *ctx, const struct marshal_cmd_DeleteVertexArrays *restrict cmd);
struct marshal_cmd_UniformBlockBinding;
uint32_t _mesa_unmarshal_UniformBlockBinding(struct gl_context *ctx, const struct marshal_cmd_UniformBlockBinding *restrict cmd);
struct marshal_cmd_CopyBufferSubData;
uint32_t _mesa_unmarshal_CopyBufferSubData(struct gl_context *ctx, const struct marshal_cmd_CopyBufferSubData *restrict cmd);
struct marshal_cmd_DrawElementsBaseVertex;
uint32_t _mesa_unmarshal_DrawElementsBaseVertex(struct gl_context *ctx, const struct marshal_cmd_DrawElementsBaseVertex *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
struct marshal_cmd_DrawRangeElementsBaseVertex;
uint32_t _mesa_unmarshal_DrawRangeElementsBaseVertex(struct gl_context *ctx, const struct marshal_cmd_DrawRangeElementsBaseVertex *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawRangeElementsBaseVertex(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
struct marshal_cmd_MultiDrawElementsBaseVertex;
uint32_t _mesa_unmarshal_MultiDrawElementsBaseVertex(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsBaseVertex *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawElementsBaseVertex(GLenum mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount, const GLint *basevertex);
struct marshal_cmd_DrawElementsInstancedBaseVertex;
uint32_t _mesa_unmarshal_DrawElementsInstancedBaseVertex(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseVertex *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsInstancedBaseVertex(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex);
struct marshal_cmd_DeleteSync;
uint32_t _mesa_unmarshal_DeleteSync(struct gl_context *ctx, const struct marshal_cmd_DeleteSync *restrict cmd);
struct marshal_cmd_WaitSync;
uint32_t _mesa_unmarshal_WaitSync(struct gl_context *ctx, const struct marshal_cmd_WaitSync *restrict cmd);
struct marshal_cmd_TexImage2DMultisample;
uint32_t _mesa_unmarshal_TexImage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexImage2DMultisample *restrict cmd);
struct marshal_cmd_TexImage3DMultisample;
uint32_t _mesa_unmarshal_TexImage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexImage3DMultisample *restrict cmd);
struct marshal_cmd_SampleMaski;
uint32_t _mesa_unmarshal_SampleMaski(struct gl_context *ctx, const struct marshal_cmd_SampleMaski *restrict cmd);
struct marshal_cmd_BlendEquationiARB;
uint32_t _mesa_unmarshal_BlendEquationiARB(struct gl_context *ctx, const struct marshal_cmd_BlendEquationiARB *restrict cmd);
struct marshal_cmd_BlendEquationSeparateiARB;
uint32_t _mesa_unmarshal_BlendEquationSeparateiARB(struct gl_context *ctx, const struct marshal_cmd_BlendEquationSeparateiARB *restrict cmd);
struct marshal_cmd_BlendFunciARB;
uint32_t _mesa_unmarshal_BlendFunciARB(struct gl_context *ctx, const struct marshal_cmd_BlendFunciARB *restrict cmd);
struct marshal_cmd_BlendFuncSeparateiARB;
uint32_t _mesa_unmarshal_BlendFuncSeparateiARB(struct gl_context *ctx, const struct marshal_cmd_BlendFuncSeparateiARB *restrict cmd);
struct marshal_cmd_MinSampleShading;
uint32_t _mesa_unmarshal_MinSampleShading(struct gl_context *ctx, const struct marshal_cmd_MinSampleShading *restrict cmd);
struct marshal_cmd_NamedStringARB;
uint32_t _mesa_unmarshal_NamedStringARB(struct gl_context *ctx, const struct marshal_cmd_NamedStringARB *restrict cmd);
struct marshal_cmd_DeleteNamedStringARB;
uint32_t _mesa_unmarshal_DeleteNamedStringARB(struct gl_context *ctx, const struct marshal_cmd_DeleteNamedStringARB *restrict cmd);
struct marshal_cmd_BindFragDataLocationIndexed;
uint32_t _mesa_unmarshal_BindFragDataLocationIndexed(struct gl_context *ctx, const struct marshal_cmd_BindFragDataLocationIndexed *restrict cmd);
struct marshal_cmd_DeleteSamplers;
uint32_t _mesa_unmarshal_DeleteSamplers(struct gl_context *ctx, const struct marshal_cmd_DeleteSamplers *restrict cmd);
struct marshal_cmd_BindSampler;
uint32_t _mesa_unmarshal_BindSampler(struct gl_context *ctx, const struct marshal_cmd_BindSampler *restrict cmd);
struct marshal_cmd_SamplerParameteri;
uint32_t _mesa_unmarshal_SamplerParameteri(struct gl_context *ctx, const struct marshal_cmd_SamplerParameteri *restrict cmd);
struct marshal_cmd_SamplerParameterf;
uint32_t _mesa_unmarshal_SamplerParameterf(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterf *restrict cmd);
struct marshal_cmd_SamplerParameteriv;
uint32_t _mesa_unmarshal_SamplerParameteriv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameteriv *restrict cmd);
struct marshal_cmd_SamplerParameterfv;
uint32_t _mesa_unmarshal_SamplerParameterfv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterfv *restrict cmd);
struct marshal_cmd_SamplerParameterIiv;
uint32_t _mesa_unmarshal_SamplerParameterIiv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterIiv *restrict cmd);
struct marshal_cmd_SamplerParameterIuiv;
uint32_t _mesa_unmarshal_SamplerParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_SamplerParameterIuiv *restrict cmd);
struct marshal_cmd_QueryCounter;
uint32_t _mesa_unmarshal_QueryCounter(struct gl_context *ctx, const struct marshal_cmd_QueryCounter *restrict cmd);
struct marshal_cmd_VertexP2ui;
uint32_t _mesa_unmarshal_VertexP2ui(struct gl_context *ctx, const struct marshal_cmd_VertexP2ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexP2ui(GLenum type, GLuint value);
struct marshal_cmd_VertexP3ui;
uint32_t _mesa_unmarshal_VertexP3ui(struct gl_context *ctx, const struct marshal_cmd_VertexP3ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexP3ui(GLenum type, GLuint value);
struct marshal_cmd_VertexP4ui;
uint32_t _mesa_unmarshal_VertexP4ui(struct gl_context *ctx, const struct marshal_cmd_VertexP4ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexP4ui(GLenum type, GLuint value);
struct marshal_cmd_VertexP2uiv;
uint32_t _mesa_unmarshal_VertexP2uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP2uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexP2uiv(GLenum type, const GLuint *value);
struct marshal_cmd_VertexP3uiv;
uint32_t _mesa_unmarshal_VertexP3uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP3uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexP3uiv(GLenum type, const GLuint *value);
struct marshal_cmd_VertexP4uiv;
uint32_t _mesa_unmarshal_VertexP4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexP4uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexP4uiv(GLenum type, const GLuint *value);
struct marshal_cmd_TexCoordP1ui;
uint32_t _mesa_unmarshal_TexCoordP1ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP1ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoordP1ui(GLenum type, GLuint coords);
struct marshal_cmd_TexCoordP2ui;
uint32_t _mesa_unmarshal_TexCoordP2ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP2ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoordP2ui(GLenum type, GLuint coords);
struct marshal_cmd_TexCoordP3ui;
uint32_t _mesa_unmarshal_TexCoordP3ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP3ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoordP3ui(GLenum type, GLuint coords);
struct marshal_cmd_TexCoordP4ui;
uint32_t _mesa_unmarshal_TexCoordP4ui(struct gl_context *ctx, const struct marshal_cmd_TexCoordP4ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoordP4ui(GLenum type, GLuint coords);
struct marshal_cmd_TexCoordP1uiv;
uint32_t _mesa_unmarshal_TexCoordP1uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP1uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoordP1uiv(GLenum type, const GLuint *coords);
struct marshal_cmd_TexCoordP2uiv;
uint32_t _mesa_unmarshal_TexCoordP2uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP2uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoordP2uiv(GLenum type, const GLuint *coords);
struct marshal_cmd_TexCoordP3uiv;
uint32_t _mesa_unmarshal_TexCoordP3uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP3uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoordP3uiv(GLenum type, const GLuint *coords);
struct marshal_cmd_TexCoordP4uiv;
uint32_t _mesa_unmarshal_TexCoordP4uiv(struct gl_context *ctx, const struct marshal_cmd_TexCoordP4uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoordP4uiv(GLenum type, const GLuint *coords);
struct marshal_cmd_MultiTexCoordP1ui;
uint32_t _mesa_unmarshal_MultiTexCoordP1ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP1ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP1ui(GLenum texture, GLenum type, GLuint coords);
struct marshal_cmd_MultiTexCoordP2ui;
uint32_t _mesa_unmarshal_MultiTexCoordP2ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP2ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP2ui(GLenum texture, GLenum type, GLuint coords);
struct marshal_cmd_MultiTexCoordP3ui;
uint32_t _mesa_unmarshal_MultiTexCoordP3ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP3ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP3ui(GLenum texture, GLenum type, GLuint coords);
struct marshal_cmd_MultiTexCoordP4ui;
uint32_t _mesa_unmarshal_MultiTexCoordP4ui(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP4ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP4ui(GLenum texture, GLenum type, GLuint coords);
struct marshal_cmd_MultiTexCoordP1uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP1uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP1uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP1uiv(GLenum texture, GLenum type, const GLuint *coords);
struct marshal_cmd_MultiTexCoordP2uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP2uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP2uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP2uiv(GLenum texture, GLenum type, const GLuint *coords);
struct marshal_cmd_MultiTexCoordP3uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP3uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP3uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP3uiv(GLenum texture, GLenum type, const GLuint *coords);
struct marshal_cmd_MultiTexCoordP4uiv;
uint32_t _mesa_unmarshal_MultiTexCoordP4uiv(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordP4uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoordP4uiv(GLenum texture, GLenum type, const GLuint *coords);
struct marshal_cmd_NormalP3ui;
uint32_t _mesa_unmarshal_NormalP3ui(struct gl_context *ctx, const struct marshal_cmd_NormalP3ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_NormalP3ui(GLenum type, GLuint coords);
struct marshal_cmd_NormalP3uiv;
uint32_t _mesa_unmarshal_NormalP3uiv(struct gl_context *ctx, const struct marshal_cmd_NormalP3uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_NormalP3uiv(GLenum type, const GLuint *coords);
struct marshal_cmd_ColorP3ui;
uint32_t _mesa_unmarshal_ColorP3ui(struct gl_context *ctx, const struct marshal_cmd_ColorP3ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_ColorP3ui(GLenum type, GLuint color);
struct marshal_cmd_ColorP4ui;
uint32_t _mesa_unmarshal_ColorP4ui(struct gl_context *ctx, const struct marshal_cmd_ColorP4ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_ColorP4ui(GLenum type, GLuint color);
struct marshal_cmd_ColorP3uiv;
uint32_t _mesa_unmarshal_ColorP3uiv(struct gl_context *ctx, const struct marshal_cmd_ColorP3uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_ColorP3uiv(GLenum type, const GLuint *color);
struct marshal_cmd_ColorP4uiv;
uint32_t _mesa_unmarshal_ColorP4uiv(struct gl_context *ctx, const struct marshal_cmd_ColorP4uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_ColorP4uiv(GLenum type, const GLuint *color);
struct marshal_cmd_SecondaryColorP3ui;
uint32_t _mesa_unmarshal_SecondaryColorP3ui(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorP3ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColorP3ui(GLenum type, GLuint color);
struct marshal_cmd_SecondaryColorP3uiv;
uint32_t _mesa_unmarshal_SecondaryColorP3uiv(struct gl_context *ctx, const struct marshal_cmd_SecondaryColorP3uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColorP3uiv(GLenum type, const GLuint *color);
struct marshal_cmd_VertexAttribP1ui;
uint32_t _mesa_unmarshal_VertexAttribP1ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP1ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribP1ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
struct marshal_cmd_VertexAttribP2ui;
uint32_t _mesa_unmarshal_VertexAttribP2ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP2ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribP2ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
struct marshal_cmd_VertexAttribP3ui;
uint32_t _mesa_unmarshal_VertexAttribP3ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP3ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribP3ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
struct marshal_cmd_VertexAttribP4ui;
uint32_t _mesa_unmarshal_VertexAttribP4ui(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP4ui *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribP4ui(GLuint index, GLenum type, GLboolean normalized, GLuint value);
struct marshal_cmd_VertexAttribP1uiv;
uint32_t _mesa_unmarshal_VertexAttribP1uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP1uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribP1uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
struct marshal_cmd_VertexAttribP2uiv;
uint32_t _mesa_unmarshal_VertexAttribP2uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP2uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribP2uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
struct marshal_cmd_VertexAttribP3uiv;
uint32_t _mesa_unmarshal_VertexAttribP3uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP3uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribP3uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
struct marshal_cmd_VertexAttribP4uiv;
uint32_t _mesa_unmarshal_VertexAttribP4uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribP4uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribP4uiv(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
struct marshal_cmd_UniformSubroutinesuiv;
uint32_t _mesa_unmarshal_UniformSubroutinesuiv(struct gl_context *ctx, const struct marshal_cmd_UniformSubroutinesuiv *restrict cmd);
struct marshal_cmd_PatchParameteri;
uint32_t _mesa_unmarshal_PatchParameteri(struct gl_context *ctx, const struct marshal_cmd_PatchParameteri *restrict cmd);
struct marshal_cmd_PatchParameterfv;
uint32_t _mesa_unmarshal_PatchParameterfv(struct gl_context *ctx, const struct marshal_cmd_PatchParameterfv *restrict cmd);
struct marshal_cmd_DrawArraysIndirect;
uint32_t _mesa_unmarshal_DrawArraysIndirect(struct gl_context *ctx, const struct marshal_cmd_DrawArraysIndirect *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawArraysIndirect(GLenum mode, const GLvoid *indirect);
struct marshal_cmd_DrawElementsIndirect;
uint32_t _mesa_unmarshal_DrawElementsIndirect(struct gl_context *ctx, const struct marshal_cmd_DrawElementsIndirect *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsIndirect(GLenum mode, GLenum type, const GLvoid *indirect);
struct marshal_cmd_MultiDrawArraysIndirect;
uint32_t _mesa_unmarshal_MultiDrawArraysIndirect(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArraysIndirect *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawArraysIndirect(GLenum mode, const GLvoid *indirect, GLsizei primcount, GLsizei stride);
struct marshal_cmd_MultiDrawElementsIndirect;
uint32_t _mesa_unmarshal_MultiDrawElementsIndirect(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsIndirect *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawElementsIndirect(GLenum mode, GLenum type, const GLvoid *indirect, GLsizei primcount, GLsizei stride);
struct marshal_cmd_Uniform1d;
uint32_t _mesa_unmarshal_Uniform1d(struct gl_context *ctx, const struct marshal_cmd_Uniform1d *restrict cmd);
struct marshal_cmd_Uniform2d;
uint32_t _mesa_unmarshal_Uniform2d(struct gl_context *ctx, const struct marshal_cmd_Uniform2d *restrict cmd);
struct marshal_cmd_Uniform3d;
uint32_t _mesa_unmarshal_Uniform3d(struct gl_context *ctx, const struct marshal_cmd_Uniform3d *restrict cmd);
struct marshal_cmd_Uniform4d;
uint32_t _mesa_unmarshal_Uniform4d(struct gl_context *ctx, const struct marshal_cmd_Uniform4d *restrict cmd);
struct marshal_cmd_Uniform1dv;
uint32_t _mesa_unmarshal_Uniform1dv(struct gl_context *ctx, const struct marshal_cmd_Uniform1dv *restrict cmd);
struct marshal_cmd_Uniform2dv;
uint32_t _mesa_unmarshal_Uniform2dv(struct gl_context *ctx, const struct marshal_cmd_Uniform2dv *restrict cmd);
struct marshal_cmd_Uniform3dv;
uint32_t _mesa_unmarshal_Uniform3dv(struct gl_context *ctx, const struct marshal_cmd_Uniform3dv *restrict cmd);
struct marshal_cmd_Uniform4dv;
uint32_t _mesa_unmarshal_Uniform4dv(struct gl_context *ctx, const struct marshal_cmd_Uniform4dv *restrict cmd);
struct marshal_cmd_UniformMatrix2dv;
uint32_t _mesa_unmarshal_UniformMatrix2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2dv *restrict cmd);
struct marshal_cmd_UniformMatrix3dv;
uint32_t _mesa_unmarshal_UniformMatrix3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3dv *restrict cmd);
struct marshal_cmd_UniformMatrix4dv;
uint32_t _mesa_unmarshal_UniformMatrix4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4dv *restrict cmd);
struct marshal_cmd_UniformMatrix2x3dv;
uint32_t _mesa_unmarshal_UniformMatrix2x3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x3dv *restrict cmd);
struct marshal_cmd_UniformMatrix2x4dv;
uint32_t _mesa_unmarshal_UniformMatrix2x4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix2x4dv *restrict cmd);
struct marshal_cmd_UniformMatrix3x2dv;
uint32_t _mesa_unmarshal_UniformMatrix3x2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x2dv *restrict cmd);
struct marshal_cmd_UniformMatrix3x4dv;
uint32_t _mesa_unmarshal_UniformMatrix3x4dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix3x4dv *restrict cmd);
struct marshal_cmd_UniformMatrix4x2dv;
uint32_t _mesa_unmarshal_UniformMatrix4x2dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x2dv *restrict cmd);
struct marshal_cmd_UniformMatrix4x3dv;
uint32_t _mesa_unmarshal_UniformMatrix4x3dv(struct gl_context *ctx, const struct marshal_cmd_UniformMatrix4x3dv *restrict cmd);
struct marshal_cmd_ProgramUniform1d;
uint32_t _mesa_unmarshal_ProgramUniform1d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1d *restrict cmd);
struct marshal_cmd_ProgramUniform2d;
uint32_t _mesa_unmarshal_ProgramUniform2d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2d *restrict cmd);
struct marshal_cmd_ProgramUniform3d;
uint32_t _mesa_unmarshal_ProgramUniform3d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3d *restrict cmd);
struct marshal_cmd_ProgramUniform4d;
uint32_t _mesa_unmarshal_ProgramUniform4d(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4d *restrict cmd);
struct marshal_cmd_ProgramUniform1dv;
uint32_t _mesa_unmarshal_ProgramUniform1dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1dv *restrict cmd);
struct marshal_cmd_ProgramUniform2dv;
uint32_t _mesa_unmarshal_ProgramUniform2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2dv *restrict cmd);
struct marshal_cmd_ProgramUniform3dv;
uint32_t _mesa_unmarshal_ProgramUniform3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3dv *restrict cmd);
struct marshal_cmd_ProgramUniform4dv;
uint32_t _mesa_unmarshal_ProgramUniform4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4dv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix2dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2dv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix3dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3dv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix4dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4dv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix2x3dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x3dv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix2x4dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x4dv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix3x2dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x2dv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix3x4dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x4dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x4dv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix4x2dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x2dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x2dv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix4x3dv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x3dv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x3dv *restrict cmd);
struct marshal_cmd_DrawTransformFeedbackStream;
uint32_t _mesa_unmarshal_DrawTransformFeedbackStream(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackStream *restrict cmd);
struct marshal_cmd_BeginQueryIndexed;
uint32_t _mesa_unmarshal_BeginQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_BeginQueryIndexed *restrict cmd);
struct marshal_cmd_EndQueryIndexed;
uint32_t _mesa_unmarshal_EndQueryIndexed(struct gl_context *ctx, const struct marshal_cmd_EndQueryIndexed *restrict cmd);
struct marshal_cmd_UseProgramStages;
uint32_t _mesa_unmarshal_UseProgramStages(struct gl_context *ctx, const struct marshal_cmd_UseProgramStages *restrict cmd);
struct marshal_cmd_ActiveShaderProgram;
uint32_t _mesa_unmarshal_ActiveShaderProgram(struct gl_context *ctx, const struct marshal_cmd_ActiveShaderProgram *restrict cmd);
struct marshal_cmd_BindProgramPipeline;
uint32_t _mesa_unmarshal_BindProgramPipeline(struct gl_context *ctx, const struct marshal_cmd_BindProgramPipeline *restrict cmd);
struct marshal_cmd_DeleteProgramPipelines;
uint32_t _mesa_unmarshal_DeleteProgramPipelines(struct gl_context *ctx, const struct marshal_cmd_DeleteProgramPipelines *restrict cmd);
struct marshal_cmd_ProgramUniform1i;
uint32_t _mesa_unmarshal_ProgramUniform1i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i *restrict cmd);
struct marshal_cmd_ProgramUniform2i;
uint32_t _mesa_unmarshal_ProgramUniform2i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i *restrict cmd);
struct marshal_cmd_ProgramUniform3i;
uint32_t _mesa_unmarshal_ProgramUniform3i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i *restrict cmd);
struct marshal_cmd_ProgramUniform4i;
uint32_t _mesa_unmarshal_ProgramUniform4i(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i *restrict cmd);
struct marshal_cmd_ProgramUniform1ui;
uint32_t _mesa_unmarshal_ProgramUniform1ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui *restrict cmd);
struct marshal_cmd_ProgramUniform2ui;
uint32_t _mesa_unmarshal_ProgramUniform2ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui *restrict cmd);
struct marshal_cmd_ProgramUniform3ui;
uint32_t _mesa_unmarshal_ProgramUniform3ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui *restrict cmd);
struct marshal_cmd_ProgramUniform4ui;
uint32_t _mesa_unmarshal_ProgramUniform4ui(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui *restrict cmd);
struct marshal_cmd_ProgramUniform1f;
uint32_t _mesa_unmarshal_ProgramUniform1f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1f *restrict cmd);
struct marshal_cmd_ProgramUniform2f;
uint32_t _mesa_unmarshal_ProgramUniform2f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2f *restrict cmd);
struct marshal_cmd_ProgramUniform3f;
uint32_t _mesa_unmarshal_ProgramUniform3f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3f *restrict cmd);
struct marshal_cmd_ProgramUniform4f;
uint32_t _mesa_unmarshal_ProgramUniform4f(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4f *restrict cmd);
struct marshal_cmd_ProgramUniform1iv;
uint32_t _mesa_unmarshal_ProgramUniform1iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1iv *restrict cmd);
struct marshal_cmd_ProgramUniform2iv;
uint32_t _mesa_unmarshal_ProgramUniform2iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2iv *restrict cmd);
struct marshal_cmd_ProgramUniform3iv;
uint32_t _mesa_unmarshal_ProgramUniform3iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3iv *restrict cmd);
struct marshal_cmd_ProgramUniform4iv;
uint32_t _mesa_unmarshal_ProgramUniform4iv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4iv *restrict cmd);
struct marshal_cmd_ProgramUniform1uiv;
uint32_t _mesa_unmarshal_ProgramUniform1uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1uiv *restrict cmd);
struct marshal_cmd_ProgramUniform2uiv;
uint32_t _mesa_unmarshal_ProgramUniform2uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2uiv *restrict cmd);
struct marshal_cmd_ProgramUniform3uiv;
uint32_t _mesa_unmarshal_ProgramUniform3uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3uiv *restrict cmd);
struct marshal_cmd_ProgramUniform4uiv;
uint32_t _mesa_unmarshal_ProgramUniform4uiv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4uiv *restrict cmd);
struct marshal_cmd_ProgramUniform1fv;
uint32_t _mesa_unmarshal_ProgramUniform1fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1fv *restrict cmd);
struct marshal_cmd_ProgramUniform2fv;
uint32_t _mesa_unmarshal_ProgramUniform2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2fv *restrict cmd);
struct marshal_cmd_ProgramUniform3fv;
uint32_t _mesa_unmarshal_ProgramUniform3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3fv *restrict cmd);
struct marshal_cmd_ProgramUniform4fv;
uint32_t _mesa_unmarshal_ProgramUniform4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4fv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix2fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2fv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix3fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3fv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix4fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4fv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix2x3fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x3fv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix3x2fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x2fv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix2x4fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix2x4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix2x4fv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix4x2fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x2fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x2fv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix3x4fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix3x4fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix3x4fv *restrict cmd);
struct marshal_cmd_ProgramUniformMatrix4x3fv;
uint32_t _mesa_unmarshal_ProgramUniformMatrix4x3fv(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformMatrix4x3fv *restrict cmd);
struct marshal_cmd_ValidateProgramPipeline;
uint32_t _mesa_unmarshal_ValidateProgramPipeline(struct gl_context *ctx, const struct marshal_cmd_ValidateProgramPipeline *restrict cmd);
struct marshal_cmd_VertexAttribL1d;
uint32_t _mesa_unmarshal_VertexAttribL1d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1d *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribL1d(GLuint index, GLdouble x);
struct marshal_cmd_VertexAttribL2d;
uint32_t _mesa_unmarshal_VertexAttribL2d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL2d *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribL2d(GLuint index, GLdouble x, GLdouble y);
struct marshal_cmd_VertexAttribL3d;
uint32_t _mesa_unmarshal_VertexAttribL3d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL3d *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribL3d(GLuint index, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_VertexAttribL4d;
uint32_t _mesa_unmarshal_VertexAttribL4d(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL4d *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribL4d(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_VertexAttribL1dv;
uint32_t _mesa_unmarshal_VertexAttribL1dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribL1dv(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttribL2dv;
uint32_t _mesa_unmarshal_VertexAttribL2dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL2dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribL2dv(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttribL3dv;
uint32_t _mesa_unmarshal_VertexAttribL3dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL3dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribL3dv(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttribL4dv;
uint32_t _mesa_unmarshal_VertexAttribL4dv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL4dv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribL4dv(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttribLPointer;
uint32_t _mesa_unmarshal_VertexAttribLPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribLPointer *restrict cmd);
struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribLOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribLOffsetEXT *restrict cmd);
struct marshal_cmd_ReleaseShaderCompiler;
uint32_t _mesa_unmarshal_ReleaseShaderCompiler(struct gl_context *ctx, const struct marshal_cmd_ReleaseShaderCompiler *restrict cmd);
struct marshal_cmd_ShaderBinary;
uint32_t _mesa_unmarshal_ShaderBinary(struct gl_context *ctx, const struct marshal_cmd_ShaderBinary *restrict cmd);
struct marshal_cmd_ClearDepthf;
uint32_t _mesa_unmarshal_ClearDepthf(struct gl_context *ctx, const struct marshal_cmd_ClearDepthf *restrict cmd);
struct marshal_cmd_DepthRangef;
uint32_t _mesa_unmarshal_DepthRangef(struct gl_context *ctx, const struct marshal_cmd_DepthRangef *restrict cmd);
struct marshal_cmd_ProgramBinary;
uint32_t _mesa_unmarshal_ProgramBinary(struct gl_context *ctx, const struct marshal_cmd_ProgramBinary *restrict cmd);
struct marshal_cmd_ProgramParameteri;
uint32_t _mesa_unmarshal_ProgramParameteri(struct gl_context *ctx, const struct marshal_cmd_ProgramParameteri *restrict cmd);
struct marshal_cmd_DebugMessageControl;
uint32_t _mesa_unmarshal_DebugMessageControl(struct gl_context *ctx, const struct marshal_cmd_DebugMessageControl *restrict cmd);
struct marshal_cmd_DebugMessageInsert;
uint32_t _mesa_unmarshal_DebugMessageInsert(struct gl_context *ctx, const struct marshal_cmd_DebugMessageInsert *restrict cmd);
struct marshal_cmd_GetnPolygonStippleARB;
uint32_t _mesa_unmarshal_GetnPolygonStippleARB(struct gl_context *ctx, const struct marshal_cmd_GetnPolygonStippleARB *restrict cmd);
struct marshal_cmd_GetnTexImageARB;
uint32_t _mesa_unmarshal_GetnTexImageARB(struct gl_context *ctx, const struct marshal_cmd_GetnTexImageARB *restrict cmd);
struct marshal_cmd_ReadnPixelsARB;
uint32_t _mesa_unmarshal_ReadnPixelsARB(struct gl_context *ctx, const struct marshal_cmd_ReadnPixelsARB *restrict cmd);
struct marshal_cmd_GetnCompressedTexImageARB;
uint32_t _mesa_unmarshal_GetnCompressedTexImageARB(struct gl_context *ctx, const struct marshal_cmd_GetnCompressedTexImageARB *restrict cmd);
struct marshal_cmd_DrawArraysInstancedBaseInstance;
uint32_t _mesa_unmarshal_DrawArraysInstancedBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawArraysInstancedBaseInstance *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawArraysInstancedBaseInstance(GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
struct marshal_cmd_DrawElementsInstancedBaseInstance;
uint32_t _mesa_unmarshal_DrawElementsInstancedBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseInstance *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsInstancedBaseInstance(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLuint baseinstance);
struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstance;
uint32_t _mesa_unmarshal_DrawElementsInstancedBaseVertexBaseInstance(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstance *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsInstancedBaseVertexBaseInstance(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);
struct marshal_cmd_DrawTransformFeedbackInstanced;
uint32_t _mesa_unmarshal_DrawTransformFeedbackInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackInstanced *restrict cmd);
struct marshal_cmd_DrawTransformFeedbackStreamInstanced;
uint32_t _mesa_unmarshal_DrawTransformFeedbackStreamInstanced(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedbackStreamInstanced *restrict cmd);
struct marshal_cmd_BindImageTexture;
uint32_t _mesa_unmarshal_BindImageTexture(struct gl_context *ctx, const struct marshal_cmd_BindImageTexture *restrict cmd);
struct marshal_cmd_MemoryBarrier;
uint32_t _mesa_unmarshal_MemoryBarrier(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrier *restrict cmd);
struct marshal_cmd_TexStorage1D;
uint32_t _mesa_unmarshal_TexStorage1D(struct gl_context *ctx, const struct marshal_cmd_TexStorage1D *restrict cmd);
struct marshal_cmd_TexStorage2D;
uint32_t _mesa_unmarshal_TexStorage2D(struct gl_context *ctx, const struct marshal_cmd_TexStorage2D *restrict cmd);
struct marshal_cmd_TexStorage3D;
uint32_t _mesa_unmarshal_TexStorage3D(struct gl_context *ctx, const struct marshal_cmd_TexStorage3D *restrict cmd);
struct marshal_cmd_TextureStorage1DEXT;
uint32_t _mesa_unmarshal_TextureStorage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage1DEXT *restrict cmd);
struct marshal_cmd_TextureStorage2DEXT;
uint32_t _mesa_unmarshal_TextureStorage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DEXT *restrict cmd);
struct marshal_cmd_TextureStorage3DEXT;
uint32_t _mesa_unmarshal_TextureStorage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DEXT *restrict cmd);
struct marshal_cmd_PushDebugGroup;
uint32_t _mesa_unmarshal_PushDebugGroup(struct gl_context *ctx, const struct marshal_cmd_PushDebugGroup *restrict cmd);
struct marshal_cmd_PopDebugGroup;
uint32_t _mesa_unmarshal_PopDebugGroup(struct gl_context *ctx, const struct marshal_cmd_PopDebugGroup *restrict cmd);
struct marshal_cmd_ObjectLabel;
uint32_t _mesa_unmarshal_ObjectLabel(struct gl_context *ctx, const struct marshal_cmd_ObjectLabel *restrict cmd);
struct marshal_cmd_DispatchCompute;
uint32_t _mesa_unmarshal_DispatchCompute(struct gl_context *ctx, const struct marshal_cmd_DispatchCompute *restrict cmd);
struct marshal_cmd_DispatchComputeIndirect;
uint32_t _mesa_unmarshal_DispatchComputeIndirect(struct gl_context *ctx, const struct marshal_cmd_DispatchComputeIndirect *restrict cmd);
struct marshal_cmd_CopyImageSubData;
uint32_t _mesa_unmarshal_CopyImageSubData(struct gl_context *ctx, const struct marshal_cmd_CopyImageSubData *restrict cmd);
struct marshal_cmd_TextureView;
uint32_t _mesa_unmarshal_TextureView(struct gl_context *ctx, const struct marshal_cmd_TextureView *restrict cmd);
struct marshal_cmd_BindVertexBuffer;
uint32_t _mesa_unmarshal_BindVertexBuffer(struct gl_context *ctx, const struct marshal_cmd_BindVertexBuffer *restrict cmd);
struct marshal_cmd_VertexAttribFormat;
uint32_t _mesa_unmarshal_VertexAttribFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribFormat *restrict cmd);
struct marshal_cmd_VertexAttribIFormat;
uint32_t _mesa_unmarshal_VertexAttribIFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribIFormat *restrict cmd);
struct marshal_cmd_VertexAttribLFormat;
uint32_t _mesa_unmarshal_VertexAttribLFormat(struct gl_context *ctx, const struct marshal_cmd_VertexAttribLFormat *restrict cmd);
struct marshal_cmd_VertexAttribBinding;
uint32_t _mesa_unmarshal_VertexAttribBinding(struct gl_context *ctx, const struct marshal_cmd_VertexAttribBinding *restrict cmd);
struct marshal_cmd_VertexBindingDivisor;
uint32_t _mesa_unmarshal_VertexBindingDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexBindingDivisor *restrict cmd);
struct marshal_cmd_VertexArrayBindVertexBufferEXT;
uint32_t _mesa_unmarshal_VertexArrayBindVertexBufferEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayBindVertexBufferEXT *restrict cmd);
struct marshal_cmd_VertexArrayVertexAttribFormatEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribFormatEXT *restrict cmd);
struct marshal_cmd_VertexArrayVertexAttribIFormatEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribIFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribIFormatEXT *restrict cmd);
struct marshal_cmd_VertexArrayVertexAttribLFormatEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribLFormatEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribLFormatEXT *restrict cmd);
struct marshal_cmd_VertexArrayVertexAttribBindingEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribBindingEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribBindingEXT *restrict cmd);
struct marshal_cmd_VertexArrayVertexBindingDivisorEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexBindingDivisorEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBindingDivisorEXT *restrict cmd);
struct marshal_cmd_FramebufferParameteri;
uint32_t _mesa_unmarshal_FramebufferParameteri(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteri *restrict cmd);
struct marshal_cmd_NamedFramebufferParameteriEXT;
uint32_t _mesa_unmarshal_NamedFramebufferParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferParameteriEXT *restrict cmd);
struct marshal_cmd_InvalidateTexSubImage;
uint32_t _mesa_unmarshal_InvalidateTexSubImage(struct gl_context *ctx, const struct marshal_cmd_InvalidateTexSubImage *restrict cmd);
struct marshal_cmd_InvalidateTexImage;
uint32_t _mesa_unmarshal_InvalidateTexImage(struct gl_context *ctx, const struct marshal_cmd_InvalidateTexImage *restrict cmd);
struct marshal_cmd_InvalidateBufferSubData;
uint32_t _mesa_unmarshal_InvalidateBufferSubData(struct gl_context *ctx, const struct marshal_cmd_InvalidateBufferSubData *restrict cmd);
struct marshal_cmd_InvalidateBufferData;
uint32_t _mesa_unmarshal_InvalidateBufferData(struct gl_context *ctx, const struct marshal_cmd_InvalidateBufferData *restrict cmd);
struct marshal_cmd_InvalidateSubFramebuffer;
uint32_t _mesa_unmarshal_InvalidateSubFramebuffer(struct gl_context *ctx, const struct marshal_cmd_InvalidateSubFramebuffer *restrict cmd);
struct marshal_cmd_InvalidateFramebuffer;
uint32_t _mesa_unmarshal_InvalidateFramebuffer(struct gl_context *ctx, const struct marshal_cmd_InvalidateFramebuffer *restrict cmd);
struct marshal_cmd_ShaderStorageBlockBinding;
uint32_t _mesa_unmarshal_ShaderStorageBlockBinding(struct gl_context *ctx, const struct marshal_cmd_ShaderStorageBlockBinding *restrict cmd);
struct marshal_cmd_TexBufferRange;
uint32_t _mesa_unmarshal_TexBufferRange(struct gl_context *ctx, const struct marshal_cmd_TexBufferRange *restrict cmd);
struct marshal_cmd_TextureBufferRangeEXT;
uint32_t _mesa_unmarshal_TextureBufferRangeEXT(struct gl_context *ctx, const struct marshal_cmd_TextureBufferRangeEXT *restrict cmd);
struct marshal_cmd_TexStorage2DMultisample;
uint32_t _mesa_unmarshal_TexStorage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexStorage2DMultisample *restrict cmd);
struct marshal_cmd_TexStorage3DMultisample;
uint32_t _mesa_unmarshal_TexStorage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TexStorage3DMultisample *restrict cmd);
struct marshal_cmd_TextureStorage2DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorage2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DMultisampleEXT *restrict cmd);
struct marshal_cmd_TextureStorage3DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorage3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DMultisampleEXT *restrict cmd);
struct marshal_cmd_BindBuffersBase;
uint32_t _mesa_unmarshal_BindBuffersBase(struct gl_context *ctx, const struct marshal_cmd_BindBuffersBase *restrict cmd);
struct marshal_cmd_BindBuffersRange;
uint32_t _mesa_unmarshal_BindBuffersRange(struct gl_context *ctx, const struct marshal_cmd_BindBuffersRange *restrict cmd);
struct marshal_cmd_BindTextures;
uint32_t _mesa_unmarshal_BindTextures(struct gl_context *ctx, const struct marshal_cmd_BindTextures *restrict cmd);
struct marshal_cmd_BindSamplers;
uint32_t _mesa_unmarshal_BindSamplers(struct gl_context *ctx, const struct marshal_cmd_BindSamplers *restrict cmd);
struct marshal_cmd_BindImageTextures;
uint32_t _mesa_unmarshal_BindImageTextures(struct gl_context *ctx, const struct marshal_cmd_BindImageTextures *restrict cmd);
struct marshal_cmd_BindVertexBuffers;
uint32_t _mesa_unmarshal_BindVertexBuffers(struct gl_context *ctx, const struct marshal_cmd_BindVertexBuffers *restrict cmd);
struct marshal_cmd_MakeTextureHandleResidentARB;
uint32_t _mesa_unmarshal_MakeTextureHandleResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeTextureHandleResidentARB *restrict cmd);
struct marshal_cmd_MakeTextureHandleNonResidentARB;
uint32_t _mesa_unmarshal_MakeTextureHandleNonResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeTextureHandleNonResidentARB *restrict cmd);
struct marshal_cmd_MakeImageHandleResidentARB;
uint32_t _mesa_unmarshal_MakeImageHandleResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeImageHandleResidentARB *restrict cmd);
struct marshal_cmd_MakeImageHandleNonResidentARB;
uint32_t _mesa_unmarshal_MakeImageHandleNonResidentARB(struct gl_context *ctx, const struct marshal_cmd_MakeImageHandleNonResidentARB *restrict cmd);
struct marshal_cmd_UniformHandleui64ARB;
uint32_t _mesa_unmarshal_UniformHandleui64ARB(struct gl_context *ctx, const struct marshal_cmd_UniformHandleui64ARB *restrict cmd);
struct marshal_cmd_UniformHandleui64vARB;
uint32_t _mesa_unmarshal_UniformHandleui64vARB(struct gl_context *ctx, const struct marshal_cmd_UniformHandleui64vARB *restrict cmd);
struct marshal_cmd_ProgramUniformHandleui64ARB;
uint32_t _mesa_unmarshal_ProgramUniformHandleui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformHandleui64ARB *restrict cmd);
struct marshal_cmd_ProgramUniformHandleui64vARB;
uint32_t _mesa_unmarshal_ProgramUniformHandleui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniformHandleui64vARB *restrict cmd);
struct marshal_cmd_VertexAttribL1ui64ARB;
uint32_t _mesa_unmarshal_VertexAttribL1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1ui64ARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribL1ui64ARB(GLuint index, GLuint64EXT x);
struct marshal_cmd_VertexAttribL1ui64vARB;
uint32_t _mesa_unmarshal_VertexAttribL1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_VertexAttribL1ui64vARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribL1ui64vARB(GLuint index, const GLuint64EXT *v);
struct marshal_cmd_DispatchComputeGroupSizeARB;
uint32_t _mesa_unmarshal_DispatchComputeGroupSizeARB(struct gl_context *ctx, const struct marshal_cmd_DispatchComputeGroupSizeARB *restrict cmd);
struct marshal_cmd_MultiDrawArraysIndirectCountARB;
uint32_t _mesa_unmarshal_MultiDrawArraysIndirectCountARB(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArraysIndirectCountARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawArraysIndirectCountARB(GLenum mode, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
struct marshal_cmd_MultiDrawElementsIndirectCountARB;
uint32_t _mesa_unmarshal_MultiDrawElementsIndirectCountARB(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsIndirectCountARB *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawElementsIndirectCountARB(GLenum mode, GLenum type, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
struct marshal_cmd_TexPageCommitmentARB;
uint32_t _mesa_unmarshal_TexPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_TexPageCommitmentARB *restrict cmd);
struct marshal_cmd_TexturePageCommitmentEXT;
uint32_t _mesa_unmarshal_TexturePageCommitmentEXT(struct gl_context *ctx, const struct marshal_cmd_TexturePageCommitmentEXT *restrict cmd);
struct marshal_cmd_ClipControl;
uint32_t _mesa_unmarshal_ClipControl(struct gl_context *ctx, const struct marshal_cmd_ClipControl *restrict cmd);
struct marshal_cmd_TransformFeedbackBufferBase;
uint32_t _mesa_unmarshal_TransformFeedbackBufferBase(struct gl_context *ctx, const struct marshal_cmd_TransformFeedbackBufferBase *restrict cmd);
struct marshal_cmd_TransformFeedbackBufferRange;
uint32_t _mesa_unmarshal_TransformFeedbackBufferRange(struct gl_context *ctx, const struct marshal_cmd_TransformFeedbackBufferRange *restrict cmd);
struct marshal_cmd_NamedBufferData;
uint32_t _mesa_unmarshal_NamedBufferData(struct gl_context *ctx, const struct marshal_cmd_NamedBufferData *restrict cmd);
void GLAPIENTRY _mesa_marshal_NamedBufferData(GLuint buffer, GLsizeiptr size, const GLvoid *data, GLenum usage);
struct marshal_cmd_NamedBufferSubData;
uint32_t _mesa_unmarshal_NamedBufferSubData(struct gl_context *ctx, const struct marshal_cmd_NamedBufferSubData *restrict cmd);
void GLAPIENTRY _mesa_marshal_NamedBufferSubData(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid *data);
struct marshal_cmd_CopyNamedBufferSubData;
uint32_t _mesa_unmarshal_CopyNamedBufferSubData(struct gl_context *ctx, const struct marshal_cmd_CopyNamedBufferSubData *restrict cmd);
struct marshal_cmd_UnmapNamedBufferEXT;
uint32_t _mesa_unmarshal_UnmapNamedBufferEXT(struct gl_context *ctx, const struct marshal_cmd_UnmapNamedBufferEXT *restrict cmd);
struct marshal_cmd_FlushMappedNamedBufferRange;
uint32_t _mesa_unmarshal_FlushMappedNamedBufferRange(struct gl_context *ctx, const struct marshal_cmd_FlushMappedNamedBufferRange *restrict cmd);
struct marshal_cmd_NamedFramebufferRenderbuffer;
uint32_t _mesa_unmarshal_NamedFramebufferRenderbuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferRenderbuffer *restrict cmd);
struct marshal_cmd_NamedFramebufferParameteri;
uint32_t _mesa_unmarshal_NamedFramebufferParameteri(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferParameteri *restrict cmd);
struct marshal_cmd_NamedFramebufferTexture;
uint32_t _mesa_unmarshal_NamedFramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture *restrict cmd);
struct marshal_cmd_NamedFramebufferTextureLayer;
uint32_t _mesa_unmarshal_NamedFramebufferTextureLayer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTextureLayer *restrict cmd);
struct marshal_cmd_NamedFramebufferDrawBuffer;
uint32_t _mesa_unmarshal_NamedFramebufferDrawBuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferDrawBuffer *restrict cmd);
struct marshal_cmd_NamedFramebufferDrawBuffers;
uint32_t _mesa_unmarshal_NamedFramebufferDrawBuffers(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferDrawBuffers *restrict cmd);
struct marshal_cmd_NamedFramebufferReadBuffer;
uint32_t _mesa_unmarshal_NamedFramebufferReadBuffer(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferReadBuffer *restrict cmd);
struct marshal_cmd_InvalidateNamedFramebufferData;
uint32_t _mesa_unmarshal_InvalidateNamedFramebufferData(struct gl_context *ctx, const struct marshal_cmd_InvalidateNamedFramebufferData *restrict cmd);
struct marshal_cmd_InvalidateNamedFramebufferSubData;
uint32_t _mesa_unmarshal_InvalidateNamedFramebufferSubData(struct gl_context *ctx, const struct marshal_cmd_InvalidateNamedFramebufferSubData *restrict cmd);
struct marshal_cmd_ClearNamedFramebufferiv;
uint32_t _mesa_unmarshal_ClearNamedFramebufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferiv *restrict cmd);
struct marshal_cmd_ClearNamedFramebufferuiv;
uint32_t _mesa_unmarshal_ClearNamedFramebufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferuiv *restrict cmd);
struct marshal_cmd_ClearNamedFramebufferfv;
uint32_t _mesa_unmarshal_ClearNamedFramebufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferfv *restrict cmd);
struct marshal_cmd_ClearNamedFramebufferfi;
uint32_t _mesa_unmarshal_ClearNamedFramebufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearNamedFramebufferfi *restrict cmd);
struct marshal_cmd_BlitNamedFramebuffer;
uint32_t _mesa_unmarshal_BlitNamedFramebuffer(struct gl_context *ctx, const struct marshal_cmd_BlitNamedFramebuffer *restrict cmd);
struct marshal_cmd_NamedRenderbufferStorage;
uint32_t _mesa_unmarshal_NamedRenderbufferStorage(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorage *restrict cmd);
struct marshal_cmd_NamedRenderbufferStorageMultisample;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageMultisample(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisample *restrict cmd);
struct marshal_cmd_TextureBuffer;
uint32_t _mesa_unmarshal_TextureBuffer(struct gl_context *ctx, const struct marshal_cmd_TextureBuffer *restrict cmd);
struct marshal_cmd_TextureBufferRange;
uint32_t _mesa_unmarshal_TextureBufferRange(struct gl_context *ctx, const struct marshal_cmd_TextureBufferRange *restrict cmd);
struct marshal_cmd_TextureStorage1D;
uint32_t _mesa_unmarshal_TextureStorage1D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage1D *restrict cmd);
struct marshal_cmd_TextureStorage2D;
uint32_t _mesa_unmarshal_TextureStorage2D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2D *restrict cmd);
struct marshal_cmd_TextureStorage3D;
uint32_t _mesa_unmarshal_TextureStorage3D(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3D *restrict cmd);
struct marshal_cmd_TextureStorage2DMultisample;
uint32_t _mesa_unmarshal_TextureStorage2DMultisample(struct gl_context *ctx, const struct marshal_cmd_TextureStorage2DMultisample *restrict cmd);
struct marshal_cmd_TextureStorage3DMultisample;
uint32_t _mesa_unmarshal_TextureStorage3DMultisample(struct gl_context *ctx, const struct marshal_cmd_TextureStorage3DMultisample *restrict cmd);
struct marshal_cmd_TextureSubImage1D;
uint32_t _mesa_unmarshal_TextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage1D *restrict cmd);
struct marshal_cmd_TextureSubImage2D;
uint32_t _mesa_unmarshal_TextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage2D *restrict cmd);
struct marshal_cmd_TextureSubImage3D;
uint32_t _mesa_unmarshal_TextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage3D *restrict cmd);
struct marshal_cmd_CompressedTextureSubImage1D;
uint32_t _mesa_unmarshal_CompressedTextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage1D *restrict cmd);
struct marshal_cmd_CompressedTextureSubImage2D;
uint32_t _mesa_unmarshal_CompressedTextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage2D *restrict cmd);
struct marshal_cmd_CompressedTextureSubImage3D;
uint32_t _mesa_unmarshal_CompressedTextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage3D *restrict cmd);
struct marshal_cmd_CopyTextureSubImage1D;
uint32_t _mesa_unmarshal_CopyTextureSubImage1D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage1D *restrict cmd);
struct marshal_cmd_CopyTextureSubImage2D;
uint32_t _mesa_unmarshal_CopyTextureSubImage2D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage2D *restrict cmd);
struct marshal_cmd_CopyTextureSubImage3D;
uint32_t _mesa_unmarshal_CopyTextureSubImage3D(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage3D *restrict cmd);
struct marshal_cmd_TextureParameterf;
uint32_t _mesa_unmarshal_TextureParameterf(struct gl_context *ctx, const struct marshal_cmd_TextureParameterf *restrict cmd);
struct marshal_cmd_TextureParameterfv;
uint32_t _mesa_unmarshal_TextureParameterfv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfv *restrict cmd);
struct marshal_cmd_TextureParameteri;
uint32_t _mesa_unmarshal_TextureParameteri(struct gl_context *ctx, const struct marshal_cmd_TextureParameteri *restrict cmd);
struct marshal_cmd_TextureParameterIiv;
uint32_t _mesa_unmarshal_TextureParameterIiv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIiv *restrict cmd);
struct marshal_cmd_TextureParameterIuiv;
uint32_t _mesa_unmarshal_TextureParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIuiv *restrict cmd);
struct marshal_cmd_TextureParameteriv;
uint32_t _mesa_unmarshal_TextureParameteriv(struct gl_context *ctx, const struct marshal_cmd_TextureParameteriv *restrict cmd);
struct marshal_cmd_GenerateTextureMipmap;
uint32_t _mesa_unmarshal_GenerateTextureMipmap(struct gl_context *ctx, const struct marshal_cmd_GenerateTextureMipmap *restrict cmd);
struct marshal_cmd_BindTextureUnit;
uint32_t _mesa_unmarshal_BindTextureUnit(struct gl_context *ctx, const struct marshal_cmd_BindTextureUnit *restrict cmd);
struct marshal_cmd_GetTextureImage;
uint32_t _mesa_unmarshal_GetTextureImage(struct gl_context *ctx, const struct marshal_cmd_GetTextureImage *restrict cmd);
struct marshal_cmd_GetCompressedTextureImage;
uint32_t _mesa_unmarshal_GetCompressedTextureImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureImage *restrict cmd);
struct marshal_cmd_DisableVertexArrayAttrib;
uint32_t _mesa_unmarshal_DisableVertexArrayAttrib(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayAttrib *restrict cmd);
struct marshal_cmd_EnableVertexArrayAttrib;
uint32_t _mesa_unmarshal_EnableVertexArrayAttrib(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayAttrib *restrict cmd);
struct marshal_cmd_VertexArrayElementBuffer;
uint32_t _mesa_unmarshal_VertexArrayElementBuffer(struct gl_context *ctx, const struct marshal_cmd_VertexArrayElementBuffer *restrict cmd);
struct marshal_cmd_VertexArrayVertexBuffer;
uint32_t _mesa_unmarshal_VertexArrayVertexBuffer(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBuffer *restrict cmd);
struct marshal_cmd_VertexArrayVertexBuffers;
uint32_t _mesa_unmarshal_VertexArrayVertexBuffers(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexBuffers *restrict cmd);
struct marshal_cmd_VertexArrayAttribFormat;
uint32_t _mesa_unmarshal_VertexArrayAttribFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribFormat *restrict cmd);
struct marshal_cmd_VertexArrayAttribIFormat;
uint32_t _mesa_unmarshal_VertexArrayAttribIFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribIFormat *restrict cmd);
struct marshal_cmd_VertexArrayAttribLFormat;
uint32_t _mesa_unmarshal_VertexArrayAttribLFormat(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribLFormat *restrict cmd);
struct marshal_cmd_VertexArrayAttribBinding;
uint32_t _mesa_unmarshal_VertexArrayAttribBinding(struct gl_context *ctx, const struct marshal_cmd_VertexArrayAttribBinding *restrict cmd);
struct marshal_cmd_VertexArrayBindingDivisor;
uint32_t _mesa_unmarshal_VertexArrayBindingDivisor(struct gl_context *ctx, const struct marshal_cmd_VertexArrayBindingDivisor *restrict cmd);
struct marshal_cmd_GetQueryBufferObjectiv;
uint32_t _mesa_unmarshal_GetQueryBufferObjectiv(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectiv *restrict cmd);
struct marshal_cmd_GetQueryBufferObjectuiv;
uint32_t _mesa_unmarshal_GetQueryBufferObjectuiv(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectuiv *restrict cmd);
struct marshal_cmd_GetQueryBufferObjecti64v;
uint32_t _mesa_unmarshal_GetQueryBufferObjecti64v(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjecti64v *restrict cmd);
struct marshal_cmd_GetQueryBufferObjectui64v;
uint32_t _mesa_unmarshal_GetQueryBufferObjectui64v(struct gl_context *ctx, const struct marshal_cmd_GetQueryBufferObjectui64v *restrict cmd);
struct marshal_cmd_GetTextureSubImage;
uint32_t _mesa_unmarshal_GetTextureSubImage(struct gl_context *ctx, const struct marshal_cmd_GetTextureSubImage *restrict cmd);
struct marshal_cmd_GetCompressedTextureSubImage;
uint32_t _mesa_unmarshal_GetCompressedTextureSubImage(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureSubImage *restrict cmd);
struct marshal_cmd_TextureBarrierNV;
uint32_t _mesa_unmarshal_TextureBarrierNV(struct gl_context *ctx, const struct marshal_cmd_TextureBarrierNV *restrict cmd);
struct marshal_cmd_BufferPageCommitmentARB;
uint32_t _mesa_unmarshal_BufferPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_BufferPageCommitmentARB *restrict cmd);
struct marshal_cmd_NamedBufferPageCommitmentEXT;
uint32_t _mesa_unmarshal_NamedBufferPageCommitmentEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferPageCommitmentEXT *restrict cmd);
struct marshal_cmd_NamedBufferPageCommitmentARB;
uint32_t _mesa_unmarshal_NamedBufferPageCommitmentARB(struct gl_context *ctx, const struct marshal_cmd_NamedBufferPageCommitmentARB *restrict cmd);
struct marshal_cmd_PrimitiveBoundingBox;
uint32_t _mesa_unmarshal_PrimitiveBoundingBox(struct gl_context *ctx, const struct marshal_cmd_PrimitiveBoundingBox *restrict cmd);
struct marshal_cmd_BlendBarrier;
uint32_t _mesa_unmarshal_BlendBarrier(struct gl_context *ctx, const struct marshal_cmd_BlendBarrier *restrict cmd);
struct marshal_cmd_Uniform1i64ARB;
uint32_t _mesa_unmarshal_Uniform1i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1i64ARB *restrict cmd);
struct marshal_cmd_Uniform2i64ARB;
uint32_t _mesa_unmarshal_Uniform2i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2i64ARB *restrict cmd);
struct marshal_cmd_Uniform3i64ARB;
uint32_t _mesa_unmarshal_Uniform3i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3i64ARB *restrict cmd);
struct marshal_cmd_Uniform4i64ARB;
uint32_t _mesa_unmarshal_Uniform4i64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4i64ARB *restrict cmd);
struct marshal_cmd_Uniform1i64vARB;
uint32_t _mesa_unmarshal_Uniform1i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1i64vARB *restrict cmd);
struct marshal_cmd_Uniform2i64vARB;
uint32_t _mesa_unmarshal_Uniform2i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2i64vARB *restrict cmd);
struct marshal_cmd_Uniform3i64vARB;
uint32_t _mesa_unmarshal_Uniform3i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3i64vARB *restrict cmd);
struct marshal_cmd_Uniform4i64vARB;
uint32_t _mesa_unmarshal_Uniform4i64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4i64vARB *restrict cmd);
struct marshal_cmd_Uniform1ui64ARB;
uint32_t _mesa_unmarshal_Uniform1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui64ARB *restrict cmd);
struct marshal_cmd_Uniform2ui64ARB;
uint32_t _mesa_unmarshal_Uniform2ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui64ARB *restrict cmd);
struct marshal_cmd_Uniform3ui64ARB;
uint32_t _mesa_unmarshal_Uniform3ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui64ARB *restrict cmd);
struct marshal_cmd_Uniform4ui64ARB;
uint32_t _mesa_unmarshal_Uniform4ui64ARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui64ARB *restrict cmd);
struct marshal_cmd_Uniform1ui64vARB;
uint32_t _mesa_unmarshal_Uniform1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui64vARB *restrict cmd);
struct marshal_cmd_Uniform2ui64vARB;
uint32_t _mesa_unmarshal_Uniform2ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui64vARB *restrict cmd);
struct marshal_cmd_Uniform3ui64vARB;
uint32_t _mesa_unmarshal_Uniform3ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui64vARB *restrict cmd);
struct marshal_cmd_Uniform4ui64vARB;
uint32_t _mesa_unmarshal_Uniform4ui64vARB(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui64vARB *restrict cmd);
struct marshal_cmd_ProgramUniform1i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform1i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i64ARB *restrict cmd);
struct marshal_cmd_ProgramUniform2i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform2i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i64ARB *restrict cmd);
struct marshal_cmd_ProgramUniform3i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform3i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i64ARB *restrict cmd);
struct marshal_cmd_ProgramUniform4i64ARB;
uint32_t _mesa_unmarshal_ProgramUniform4i64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i64ARB *restrict cmd);
struct marshal_cmd_ProgramUniform1i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform1i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1i64vARB *restrict cmd);
struct marshal_cmd_ProgramUniform2i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform2i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2i64vARB *restrict cmd);
struct marshal_cmd_ProgramUniform3i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform3i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3i64vARB *restrict cmd);
struct marshal_cmd_ProgramUniform4i64vARB;
uint32_t _mesa_unmarshal_ProgramUniform4i64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4i64vARB *restrict cmd);
struct marshal_cmd_ProgramUniform1ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform1ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui64ARB *restrict cmd);
struct marshal_cmd_ProgramUniform2ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform2ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui64ARB *restrict cmd);
struct marshal_cmd_ProgramUniform3ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform3ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui64ARB *restrict cmd);
struct marshal_cmd_ProgramUniform4ui64ARB;
uint32_t _mesa_unmarshal_ProgramUniform4ui64ARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui64ARB *restrict cmd);
struct marshal_cmd_ProgramUniform1ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform1ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform1ui64vARB *restrict cmd);
struct marshal_cmd_ProgramUniform2ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform2ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform2ui64vARB *restrict cmd);
struct marshal_cmd_ProgramUniform3ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform3ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform3ui64vARB *restrict cmd);
struct marshal_cmd_ProgramUniform4ui64vARB;
uint32_t _mesa_unmarshal_ProgramUniform4ui64vARB(struct gl_context *ctx, const struct marshal_cmd_ProgramUniform4ui64vARB *restrict cmd);
struct marshal_cmd_MaxShaderCompilerThreadsKHR;
uint32_t _mesa_unmarshal_MaxShaderCompilerThreadsKHR(struct gl_context *ctx, const struct marshal_cmd_MaxShaderCompilerThreadsKHR *restrict cmd);
struct marshal_cmd_SpecializeShaderARB;
uint32_t _mesa_unmarshal_SpecializeShaderARB(struct gl_context *ctx, const struct marshal_cmd_SpecializeShaderARB *restrict cmd);
struct marshal_cmd_ColorPointerEXT;
uint32_t _mesa_unmarshal_ColorPointerEXT(struct gl_context *ctx, const struct marshal_cmd_ColorPointerEXT *restrict cmd);
struct marshal_cmd_EdgeFlagPointerEXT;
uint32_t _mesa_unmarshal_EdgeFlagPointerEXT(struct gl_context *ctx, const struct marshal_cmd_EdgeFlagPointerEXT *restrict cmd);
struct marshal_cmd_IndexPointerEXT;
uint32_t _mesa_unmarshal_IndexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_IndexPointerEXT *restrict cmd);
struct marshal_cmd_NormalPointerEXT;
uint32_t _mesa_unmarshal_NormalPointerEXT(struct gl_context *ctx, const struct marshal_cmd_NormalPointerEXT *restrict cmd);
struct marshal_cmd_TexCoordPointerEXT;
uint32_t _mesa_unmarshal_TexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_TexCoordPointerEXT *restrict cmd);
struct marshal_cmd_VertexPointerEXT;
uint32_t _mesa_unmarshal_VertexPointerEXT(struct gl_context *ctx, const struct marshal_cmd_VertexPointerEXT *restrict cmd);
struct marshal_cmd_LockArraysEXT;
uint32_t _mesa_unmarshal_LockArraysEXT(struct gl_context *ctx, const struct marshal_cmd_LockArraysEXT *restrict cmd);
struct marshal_cmd_UnlockArraysEXT;
uint32_t _mesa_unmarshal_UnlockArraysEXT(struct gl_context *ctx, const struct marshal_cmd_UnlockArraysEXT *restrict cmd);
struct marshal_cmd_ViewportArrayv;
uint32_t _mesa_unmarshal_ViewportArrayv(struct gl_context *ctx, const struct marshal_cmd_ViewportArrayv *restrict cmd);
struct marshal_cmd_ViewportIndexedf;
uint32_t _mesa_unmarshal_ViewportIndexedf(struct gl_context *ctx, const struct marshal_cmd_ViewportIndexedf *restrict cmd);
struct marshal_cmd_ViewportIndexedfv;
uint32_t _mesa_unmarshal_ViewportIndexedfv(struct gl_context *ctx, const struct marshal_cmd_ViewportIndexedfv *restrict cmd);
struct marshal_cmd_ScissorArrayv;
uint32_t _mesa_unmarshal_ScissorArrayv(struct gl_context *ctx, const struct marshal_cmd_ScissorArrayv *restrict cmd);
struct marshal_cmd_ScissorIndexed;
uint32_t _mesa_unmarshal_ScissorIndexed(struct gl_context *ctx, const struct marshal_cmd_ScissorIndexed *restrict cmd);
struct marshal_cmd_ScissorIndexedv;
uint32_t _mesa_unmarshal_ScissorIndexedv(struct gl_context *ctx, const struct marshal_cmd_ScissorIndexedv *restrict cmd);
struct marshal_cmd_DepthRangeArrayv;
uint32_t _mesa_unmarshal_DepthRangeArrayv(struct gl_context *ctx, const struct marshal_cmd_DepthRangeArrayv *restrict cmd);
struct marshal_cmd_DepthRangeIndexed;
uint32_t _mesa_unmarshal_DepthRangeIndexed(struct gl_context *ctx, const struct marshal_cmd_DepthRangeIndexed *restrict cmd);
struct marshal_cmd_FramebufferSampleLocationsfvARB;
uint32_t _mesa_unmarshal_FramebufferSampleLocationsfvARB(struct gl_context *ctx, const struct marshal_cmd_FramebufferSampleLocationsfvARB *restrict cmd);
struct marshal_cmd_NamedFramebufferSampleLocationsfvARB;
uint32_t _mesa_unmarshal_NamedFramebufferSampleLocationsfvARB(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferSampleLocationsfvARB *restrict cmd);
struct marshal_cmd_EvaluateDepthValuesARB;
uint32_t _mesa_unmarshal_EvaluateDepthValuesARB(struct gl_context *ctx, const struct marshal_cmd_EvaluateDepthValuesARB *restrict cmd);
struct marshal_cmd_WindowPos4dMESA;
uint32_t _mesa_unmarshal_WindowPos4dMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dMESA *restrict cmd);
struct marshal_cmd_WindowPos4dvMESA;
uint32_t _mesa_unmarshal_WindowPos4dvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4dvMESA *restrict cmd);
struct marshal_cmd_WindowPos4fMESA;
uint32_t _mesa_unmarshal_WindowPos4fMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fMESA *restrict cmd);
struct marshal_cmd_WindowPos4fvMESA;
uint32_t _mesa_unmarshal_WindowPos4fvMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4fvMESA *restrict cmd);
struct marshal_cmd_WindowPos4iMESA;
uint32_t _mesa_unmarshal_WindowPos4iMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4iMESA *restrict cmd);
struct marshal_cmd_WindowPos4ivMESA;
uint32_t _mesa_unmarshal_WindowPos4ivMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4ivMESA *restrict cmd);
struct marshal_cmd_WindowPos4sMESA;
uint32_t _mesa_unmarshal_WindowPos4sMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4sMESA *restrict cmd);
struct marshal_cmd_WindowPos4svMESA;
uint32_t _mesa_unmarshal_WindowPos4svMESA(struct gl_context *ctx, const struct marshal_cmd_WindowPos4svMESA *restrict cmd);
struct marshal_cmd_MultiModeDrawArraysIBM;
uint32_t _mesa_unmarshal_MultiModeDrawArraysIBM(struct gl_context *ctx, const struct marshal_cmd_MultiModeDrawArraysIBM *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiModeDrawArraysIBM(const GLenum *mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride);
struct marshal_cmd_MultiModeDrawElementsIBM;
uint32_t _mesa_unmarshal_MultiModeDrawElementsIBM(struct gl_context *ctx, const struct marshal_cmd_MultiModeDrawElementsIBM *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiModeDrawElementsIBM(const GLenum *mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount, GLint modestride);
struct marshal_cmd_VertexAttrib1sNV;
uint32_t _mesa_unmarshal_VertexAttrib1sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1sNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1sNV(GLuint index, GLshort x);
struct marshal_cmd_VertexAttrib1svNV;
uint32_t _mesa_unmarshal_VertexAttrib1svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1svNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1svNV(GLuint index, const GLshort *v);
struct marshal_cmd_VertexAttrib2sNV;
uint32_t _mesa_unmarshal_VertexAttrib2sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2sNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2sNV(GLuint index, GLshort x, GLshort y);
struct marshal_cmd_VertexAttrib2svNV;
uint32_t _mesa_unmarshal_VertexAttrib2svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2svNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2svNV(GLuint index, const GLshort *v);
struct marshal_cmd_VertexAttrib3sNV;
uint32_t _mesa_unmarshal_VertexAttrib3sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3sNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3sNV(GLuint index, GLshort x, GLshort y, GLshort z);
struct marshal_cmd_VertexAttrib3svNV;
uint32_t _mesa_unmarshal_VertexAttrib3svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3svNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3svNV(GLuint index, const GLshort *v);
struct marshal_cmd_VertexAttrib4sNV;
uint32_t _mesa_unmarshal_VertexAttrib4sNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4sNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4sNV(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
struct marshal_cmd_VertexAttrib4svNV;
uint32_t _mesa_unmarshal_VertexAttrib4svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4svNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4svNV(GLuint index, const GLshort *v);
struct marshal_cmd_VertexAttrib1fNV;
uint32_t _mesa_unmarshal_VertexAttrib1fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1fNV(GLuint index, GLfloat x);
struct marshal_cmd_VertexAttrib1fvNV;
uint32_t _mesa_unmarshal_VertexAttrib1fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1fvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1fvNV(GLuint index, const GLfloat *v);
struct marshal_cmd_VertexAttrib2fNV;
uint32_t _mesa_unmarshal_VertexAttrib2fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2fNV(GLuint index, GLfloat x, GLfloat y);
struct marshal_cmd_VertexAttrib2fvNV;
uint32_t _mesa_unmarshal_VertexAttrib2fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2fvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2fvNV(GLuint index, const GLfloat *v);
struct marshal_cmd_VertexAttrib3fNV;
uint32_t _mesa_unmarshal_VertexAttrib3fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z);
struct marshal_cmd_VertexAttrib3fvNV;
uint32_t _mesa_unmarshal_VertexAttrib3fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3fvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3fvNV(GLuint index, const GLfloat *v);
struct marshal_cmd_VertexAttrib4fNV;
uint32_t _mesa_unmarshal_VertexAttrib4fNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4fNV(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
struct marshal_cmd_VertexAttrib4fvNV;
uint32_t _mesa_unmarshal_VertexAttrib4fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4fvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4fvNV(GLuint index, const GLfloat *v);
struct marshal_cmd_VertexAttrib1dNV;
uint32_t _mesa_unmarshal_VertexAttrib1dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1dNV(GLuint index, GLdouble x);
struct marshal_cmd_VertexAttrib1dvNV;
uint32_t _mesa_unmarshal_VertexAttrib1dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1dvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1dvNV(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttrib2dNV;
uint32_t _mesa_unmarshal_VertexAttrib2dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2dNV(GLuint index, GLdouble x, GLdouble y);
struct marshal_cmd_VertexAttrib2dvNV;
uint32_t _mesa_unmarshal_VertexAttrib2dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2dvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2dvNV(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttrib3dNV;
uint32_t _mesa_unmarshal_VertexAttrib3dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z);
struct marshal_cmd_VertexAttrib3dvNV;
uint32_t _mesa_unmarshal_VertexAttrib3dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3dvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3dvNV(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttrib4dNV;
uint32_t _mesa_unmarshal_VertexAttrib4dNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4dNV(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
struct marshal_cmd_VertexAttrib4dvNV;
uint32_t _mesa_unmarshal_VertexAttrib4dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4dvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4dvNV(GLuint index, const GLdouble *v);
struct marshal_cmd_VertexAttrib4ubNV;
uint32_t _mesa_unmarshal_VertexAttrib4ubNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4ubNV(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
struct marshal_cmd_VertexAttrib4ubvNV;
uint32_t _mesa_unmarshal_VertexAttrib4ubvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4ubvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4ubvNV(GLuint index, const GLubyte *v);
struct marshal_cmd_VertexAttribs1svNV;
uint32_t _mesa_unmarshal_VertexAttribs1svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1svNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs1svNV(GLuint index, GLsizei n, const GLshort *v);
struct marshal_cmd_VertexAttribs2svNV;
uint32_t _mesa_unmarshal_VertexAttribs2svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2svNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs2svNV(GLuint index, GLsizei n, const GLshort *v);
struct marshal_cmd_VertexAttribs3svNV;
uint32_t _mesa_unmarshal_VertexAttribs3svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3svNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs3svNV(GLuint index, GLsizei n, const GLshort *v);
struct marshal_cmd_VertexAttribs4svNV;
uint32_t _mesa_unmarshal_VertexAttribs4svNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4svNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs4svNV(GLuint index, GLsizei n, const GLshort *v);
struct marshal_cmd_VertexAttribs1fvNV;
uint32_t _mesa_unmarshal_VertexAttribs1fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1fvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs1fvNV(GLuint index, GLsizei n, const GLfloat *v);
struct marshal_cmd_VertexAttribs2fvNV;
uint32_t _mesa_unmarshal_VertexAttribs2fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2fvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs2fvNV(GLuint index, GLsizei n, const GLfloat *v);
struct marshal_cmd_VertexAttribs3fvNV;
uint32_t _mesa_unmarshal_VertexAttribs3fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3fvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs3fvNV(GLuint index, GLsizei n, const GLfloat *v);
struct marshal_cmd_VertexAttribs4fvNV;
uint32_t _mesa_unmarshal_VertexAttribs4fvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4fvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs4fvNV(GLuint index, GLsizei n, const GLfloat *v);
struct marshal_cmd_VertexAttribs1dvNV;
uint32_t _mesa_unmarshal_VertexAttribs1dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1dvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs1dvNV(GLuint index, GLsizei n, const GLdouble *v);
struct marshal_cmd_VertexAttribs2dvNV;
uint32_t _mesa_unmarshal_VertexAttribs2dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2dvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs2dvNV(GLuint index, GLsizei n, const GLdouble *v);
struct marshal_cmd_VertexAttribs3dvNV;
uint32_t _mesa_unmarshal_VertexAttribs3dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3dvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs3dvNV(GLuint index, GLsizei n, const GLdouble *v);
struct marshal_cmd_VertexAttribs4dvNV;
uint32_t _mesa_unmarshal_VertexAttribs4dvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4dvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs4dvNV(GLuint index, GLsizei n, const GLdouble *v);
struct marshal_cmd_VertexAttribs4ubvNV;
uint32_t _mesa_unmarshal_VertexAttribs4ubvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4ubvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs4ubvNV(GLuint index, GLsizei n, const GLubyte *v);
struct marshal_cmd_BindFragmentShaderATI;
uint32_t _mesa_unmarshal_BindFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_BindFragmentShaderATI *restrict cmd);
struct marshal_cmd_DeleteFragmentShaderATI;
uint32_t _mesa_unmarshal_DeleteFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_DeleteFragmentShaderATI *restrict cmd);
struct marshal_cmd_BeginFragmentShaderATI;
uint32_t _mesa_unmarshal_BeginFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_BeginFragmentShaderATI *restrict cmd);
struct marshal_cmd_EndFragmentShaderATI;
uint32_t _mesa_unmarshal_EndFragmentShaderATI(struct gl_context *ctx, const struct marshal_cmd_EndFragmentShaderATI *restrict cmd);
struct marshal_cmd_PassTexCoordATI;
uint32_t _mesa_unmarshal_PassTexCoordATI(struct gl_context *ctx, const struct marshal_cmd_PassTexCoordATI *restrict cmd);
struct marshal_cmd_SampleMapATI;
uint32_t _mesa_unmarshal_SampleMapATI(struct gl_context *ctx, const struct marshal_cmd_SampleMapATI *restrict cmd);
struct marshal_cmd_ColorFragmentOp1ATI;
uint32_t _mesa_unmarshal_ColorFragmentOp1ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp1ATI *restrict cmd);
struct marshal_cmd_ColorFragmentOp2ATI;
uint32_t _mesa_unmarshal_ColorFragmentOp2ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp2ATI *restrict cmd);
struct marshal_cmd_ColorFragmentOp3ATI;
uint32_t _mesa_unmarshal_ColorFragmentOp3ATI(struct gl_context *ctx, const struct marshal_cmd_ColorFragmentOp3ATI *restrict cmd);
struct marshal_cmd_AlphaFragmentOp1ATI;
uint32_t _mesa_unmarshal_AlphaFragmentOp1ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp1ATI *restrict cmd);
struct marshal_cmd_AlphaFragmentOp2ATI;
uint32_t _mesa_unmarshal_AlphaFragmentOp2ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp2ATI *restrict cmd);
struct marshal_cmd_AlphaFragmentOp3ATI;
uint32_t _mesa_unmarshal_AlphaFragmentOp3ATI(struct gl_context *ctx, const struct marshal_cmd_AlphaFragmentOp3ATI *restrict cmd);
struct marshal_cmd_SetFragmentShaderConstantATI;
uint32_t _mesa_unmarshal_SetFragmentShaderConstantATI(struct gl_context *ctx, const struct marshal_cmd_SetFragmentShaderConstantATI *restrict cmd);
struct marshal_cmd_ActiveStencilFaceEXT;
uint32_t _mesa_unmarshal_ActiveStencilFaceEXT(struct gl_context *ctx, const struct marshal_cmd_ActiveStencilFaceEXT *restrict cmd);
struct marshal_cmd_DepthBoundsEXT;
uint32_t _mesa_unmarshal_DepthBoundsEXT(struct gl_context *ctx, const struct marshal_cmd_DepthBoundsEXT *restrict cmd);
struct marshal_cmd_BindRenderbufferEXT;
uint32_t _mesa_unmarshal_BindRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindRenderbufferEXT *restrict cmd);
struct marshal_cmd_BindFramebufferEXT;
uint32_t _mesa_unmarshal_BindFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_BindFramebufferEXT *restrict cmd);
struct marshal_cmd_ProvokingVertex;
uint32_t _mesa_unmarshal_ProvokingVertex(struct gl_context *ctx, const struct marshal_cmd_ProvokingVertex *restrict cmd);
struct marshal_cmd_ColorMaski;
uint32_t _mesa_unmarshal_ColorMaski(struct gl_context *ctx, const struct marshal_cmd_ColorMaski *restrict cmd);
struct marshal_cmd_Enablei;
uint32_t _mesa_unmarshal_Enablei(struct gl_context *ctx, const struct marshal_cmd_Enablei *restrict cmd);
struct marshal_cmd_Disablei;
uint32_t _mesa_unmarshal_Disablei(struct gl_context *ctx, const struct marshal_cmd_Disablei *restrict cmd);
struct marshal_cmd_DeletePerfMonitorsAMD;
uint32_t _mesa_unmarshal_DeletePerfMonitorsAMD(struct gl_context *ctx, const struct marshal_cmd_DeletePerfMonitorsAMD *restrict cmd);
struct marshal_cmd_SelectPerfMonitorCountersAMD;
uint32_t _mesa_unmarshal_SelectPerfMonitorCountersAMD(struct gl_context *ctx, const struct marshal_cmd_SelectPerfMonitorCountersAMD *restrict cmd);
struct marshal_cmd_BeginPerfMonitorAMD;
uint32_t _mesa_unmarshal_BeginPerfMonitorAMD(struct gl_context *ctx, const struct marshal_cmd_BeginPerfMonitorAMD *restrict cmd);
struct marshal_cmd_EndPerfMonitorAMD;
uint32_t _mesa_unmarshal_EndPerfMonitorAMD(struct gl_context *ctx, const struct marshal_cmd_EndPerfMonitorAMD *restrict cmd);
struct marshal_cmd_CopyImageSubDataNV;
uint32_t _mesa_unmarshal_CopyImageSubDataNV(struct gl_context *ctx, const struct marshal_cmd_CopyImageSubDataNV *restrict cmd);
struct marshal_cmd_MatrixLoadfEXT;
uint32_t _mesa_unmarshal_MatrixLoadfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadfEXT *restrict cmd);
struct marshal_cmd_MatrixLoaddEXT;
uint32_t _mesa_unmarshal_MatrixLoaddEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoaddEXT *restrict cmd);
struct marshal_cmd_MatrixMultfEXT;
uint32_t _mesa_unmarshal_MatrixMultfEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultfEXT *restrict cmd);
struct marshal_cmd_MatrixMultdEXT;
uint32_t _mesa_unmarshal_MatrixMultdEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultdEXT *restrict cmd);
struct marshal_cmd_MatrixLoadIdentityEXT;
uint32_t _mesa_unmarshal_MatrixLoadIdentityEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadIdentityEXT *restrict cmd);
struct marshal_cmd_MatrixRotatefEXT;
uint32_t _mesa_unmarshal_MatrixRotatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatefEXT *restrict cmd);
struct marshal_cmd_MatrixRotatedEXT;
uint32_t _mesa_unmarshal_MatrixRotatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixRotatedEXT *restrict cmd);
struct marshal_cmd_MatrixScalefEXT;
uint32_t _mesa_unmarshal_MatrixScalefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScalefEXT *restrict cmd);
struct marshal_cmd_MatrixScaledEXT;
uint32_t _mesa_unmarshal_MatrixScaledEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixScaledEXT *restrict cmd);
struct marshal_cmd_MatrixTranslatefEXT;
uint32_t _mesa_unmarshal_MatrixTranslatefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatefEXT *restrict cmd);
struct marshal_cmd_MatrixTranslatedEXT;
uint32_t _mesa_unmarshal_MatrixTranslatedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixTranslatedEXT *restrict cmd);
struct marshal_cmd_MatrixOrthoEXT;
uint32_t _mesa_unmarshal_MatrixOrthoEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixOrthoEXT *restrict cmd);
struct marshal_cmd_MatrixFrustumEXT;
uint32_t _mesa_unmarshal_MatrixFrustumEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixFrustumEXT *restrict cmd);
struct marshal_cmd_MatrixPushEXT;
uint32_t _mesa_unmarshal_MatrixPushEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPushEXT *restrict cmd);
struct marshal_cmd_MatrixPopEXT;
uint32_t _mesa_unmarshal_MatrixPopEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixPopEXT *restrict cmd);
struct marshal_cmd_ClientAttribDefaultEXT;
uint32_t _mesa_unmarshal_ClientAttribDefaultEXT(struct gl_context *ctx, const struct marshal_cmd_ClientAttribDefaultEXT *restrict cmd);
struct marshal_cmd_PushClientAttribDefaultEXT;
uint32_t _mesa_unmarshal_PushClientAttribDefaultEXT(struct gl_context *ctx, const struct marshal_cmd_PushClientAttribDefaultEXT *restrict cmd);
struct marshal_cmd_TextureParameteriEXT;
uint32_t _mesa_unmarshal_TextureParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameteriEXT *restrict cmd);
struct marshal_cmd_TextureParameterivEXT;
uint32_t _mesa_unmarshal_TextureParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterivEXT *restrict cmd);
struct marshal_cmd_TextureParameterfEXT;
uint32_t _mesa_unmarshal_TextureParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfEXT *restrict cmd);
struct marshal_cmd_TextureParameterfvEXT;
uint32_t _mesa_unmarshal_TextureParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterfvEXT *restrict cmd);
struct marshal_cmd_TextureImage1DEXT;
uint32_t _mesa_unmarshal_TextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage1DEXT *restrict cmd);
struct marshal_cmd_TextureImage2DEXT;
uint32_t _mesa_unmarshal_TextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage2DEXT *restrict cmd);
struct marshal_cmd_TextureImage3DEXT;
uint32_t _mesa_unmarshal_TextureImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureImage3DEXT *restrict cmd);
struct marshal_cmd_TextureSubImage1DEXT;
uint32_t _mesa_unmarshal_TextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage1DEXT *restrict cmd);
struct marshal_cmd_TextureSubImage2DEXT;
uint32_t _mesa_unmarshal_TextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage2DEXT *restrict cmd);
struct marshal_cmd_TextureSubImage3DEXT;
uint32_t _mesa_unmarshal_TextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureSubImage3DEXT *restrict cmd);
struct marshal_cmd_CopyTextureImage1DEXT;
uint32_t _mesa_unmarshal_CopyTextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureImage1DEXT *restrict cmd);
struct marshal_cmd_CopyTextureImage2DEXT;
uint32_t _mesa_unmarshal_CopyTextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureImage2DEXT *restrict cmd);
struct marshal_cmd_CopyTextureSubImage1DEXT;
uint32_t _mesa_unmarshal_CopyTextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage1DEXT *restrict cmd);
struct marshal_cmd_CopyTextureSubImage2DEXT;
uint32_t _mesa_unmarshal_CopyTextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage2DEXT *restrict cmd);
struct marshal_cmd_CopyTextureSubImage3DEXT;
uint32_t _mesa_unmarshal_CopyTextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyTextureSubImage3DEXT *restrict cmd);
struct marshal_cmd_GetTextureImageEXT;
uint32_t _mesa_unmarshal_GetTextureImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetTextureImageEXT *restrict cmd);
struct marshal_cmd_BindMultiTextureEXT;
uint32_t _mesa_unmarshal_BindMultiTextureEXT(struct gl_context *ctx, const struct marshal_cmd_BindMultiTextureEXT *restrict cmd);
struct marshal_cmd_EnableClientStateiEXT;
uint32_t _mesa_unmarshal_EnableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_EnableClientStateiEXT *restrict cmd);
struct marshal_cmd_DisableClientStateiEXT;
uint32_t _mesa_unmarshal_DisableClientStateiEXT(struct gl_context *ctx, const struct marshal_cmd_DisableClientStateiEXT *restrict cmd);
struct marshal_cmd_MultiTexEnviEXT;
uint32_t _mesa_unmarshal_MultiTexEnviEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnviEXT *restrict cmd);
struct marshal_cmd_MultiTexEnvivEXT;
uint32_t _mesa_unmarshal_MultiTexEnvivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvivEXT *restrict cmd);
struct marshal_cmd_MultiTexEnvfEXT;
uint32_t _mesa_unmarshal_MultiTexEnvfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfEXT *restrict cmd);
struct marshal_cmd_MultiTexEnvfvEXT;
uint32_t _mesa_unmarshal_MultiTexEnvfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexEnvfvEXT *restrict cmd);
struct marshal_cmd_MultiTexParameteriEXT;
uint32_t _mesa_unmarshal_MultiTexParameteriEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameteriEXT *restrict cmd);
struct marshal_cmd_MultiTexParameterivEXT;
uint32_t _mesa_unmarshal_MultiTexParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterivEXT *restrict cmd);
struct marshal_cmd_MultiTexParameterfEXT;
uint32_t _mesa_unmarshal_MultiTexParameterfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfEXT *restrict cmd);
struct marshal_cmd_MultiTexParameterfvEXT;
uint32_t _mesa_unmarshal_MultiTexParameterfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterfvEXT *restrict cmd);
struct marshal_cmd_GetMultiTexImageEXT;
uint32_t _mesa_unmarshal_GetMultiTexImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetMultiTexImageEXT *restrict cmd);
struct marshal_cmd_MultiTexImage1DEXT;
uint32_t _mesa_unmarshal_MultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage1DEXT *restrict cmd);
struct marshal_cmd_MultiTexImage2DEXT;
uint32_t _mesa_unmarshal_MultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage2DEXT *restrict cmd);
struct marshal_cmd_MultiTexImage3DEXT;
uint32_t _mesa_unmarshal_MultiTexImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexImage3DEXT *restrict cmd);
struct marshal_cmd_MultiTexSubImage1DEXT;
uint32_t _mesa_unmarshal_MultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage1DEXT *restrict cmd);
struct marshal_cmd_MultiTexSubImage2DEXT;
uint32_t _mesa_unmarshal_MultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage2DEXT *restrict cmd);
struct marshal_cmd_MultiTexSubImage3DEXT;
uint32_t _mesa_unmarshal_MultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexSubImage3DEXT *restrict cmd);
struct marshal_cmd_CopyMultiTexImage1DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexImage1DEXT *restrict cmd);
struct marshal_cmd_CopyMultiTexImage2DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexImage2DEXT *restrict cmd);
struct marshal_cmd_CopyMultiTexSubImage1DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage1DEXT *restrict cmd);
struct marshal_cmd_CopyMultiTexSubImage2DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage2DEXT *restrict cmd);
struct marshal_cmd_CopyMultiTexSubImage3DEXT;
uint32_t _mesa_unmarshal_CopyMultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CopyMultiTexSubImage3DEXT *restrict cmd);
struct marshal_cmd_MultiTexGendEXT;
uint32_t _mesa_unmarshal_MultiTexGendEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendEXT *restrict cmd);
struct marshal_cmd_MultiTexGendvEXT;
uint32_t _mesa_unmarshal_MultiTexGendvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGendvEXT *restrict cmd);
struct marshal_cmd_MultiTexGenfEXT;
uint32_t _mesa_unmarshal_MultiTexGenfEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfEXT *restrict cmd);
struct marshal_cmd_MultiTexGenfvEXT;
uint32_t _mesa_unmarshal_MultiTexGenfvEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenfvEXT *restrict cmd);
struct marshal_cmd_MultiTexGeniEXT;
uint32_t _mesa_unmarshal_MultiTexGeniEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGeniEXT *restrict cmd);
struct marshal_cmd_MultiTexGenivEXT;
uint32_t _mesa_unmarshal_MultiTexGenivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexGenivEXT *restrict cmd);
struct marshal_cmd_MultiTexCoordPointerEXT;
uint32_t _mesa_unmarshal_MultiTexCoordPointerEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoordPointerEXT *restrict cmd);
struct marshal_cmd_MatrixLoadTransposefEXT;
uint32_t _mesa_unmarshal_MatrixLoadTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposefEXT *restrict cmd);
struct marshal_cmd_MatrixLoadTransposedEXT;
uint32_t _mesa_unmarshal_MatrixLoadTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixLoadTransposedEXT *restrict cmd);
struct marshal_cmd_MatrixMultTransposefEXT;
uint32_t _mesa_unmarshal_MatrixMultTransposefEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposefEXT *restrict cmd);
struct marshal_cmd_MatrixMultTransposedEXT;
uint32_t _mesa_unmarshal_MatrixMultTransposedEXT(struct gl_context *ctx, const struct marshal_cmd_MatrixMultTransposedEXT *restrict cmd);
struct marshal_cmd_CompressedTextureImage1DEXT;
uint32_t _mesa_unmarshal_CompressedTextureImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage1DEXT *restrict cmd);
struct marshal_cmd_CompressedTextureImage2DEXT;
uint32_t _mesa_unmarshal_CompressedTextureImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage2DEXT *restrict cmd);
struct marshal_cmd_CompressedTextureImage3DEXT;
uint32_t _mesa_unmarshal_CompressedTextureImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureImage3DEXT *restrict cmd);
struct marshal_cmd_CompressedTextureSubImage1DEXT;
uint32_t _mesa_unmarshal_CompressedTextureSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage1DEXT *restrict cmd);
struct marshal_cmd_CompressedTextureSubImage2DEXT;
uint32_t _mesa_unmarshal_CompressedTextureSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage2DEXT *restrict cmd);
struct marshal_cmd_CompressedTextureSubImage3DEXT;
uint32_t _mesa_unmarshal_CompressedTextureSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedTextureSubImage3DEXT *restrict cmd);
struct marshal_cmd_GetCompressedTextureImageEXT;
uint32_t _mesa_unmarshal_GetCompressedTextureImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetCompressedTextureImageEXT *restrict cmd);
struct marshal_cmd_CompressedMultiTexImage1DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage1DEXT *restrict cmd);
struct marshal_cmd_CompressedMultiTexImage2DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage2DEXT *restrict cmd);
struct marshal_cmd_CompressedMultiTexImage3DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexImage3DEXT *restrict cmd);
struct marshal_cmd_CompressedMultiTexSubImage1DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexSubImage1DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage1DEXT *restrict cmd);
struct marshal_cmd_CompressedMultiTexSubImage2DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexSubImage2DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage2DEXT *restrict cmd);
struct marshal_cmd_CompressedMultiTexSubImage3DEXT;
uint32_t _mesa_unmarshal_CompressedMultiTexSubImage3DEXT(struct gl_context *ctx, const struct marshal_cmd_CompressedMultiTexSubImage3DEXT *restrict cmd);
struct marshal_cmd_GetCompressedMultiTexImageEXT;
uint32_t _mesa_unmarshal_GetCompressedMultiTexImageEXT(struct gl_context *ctx, const struct marshal_cmd_GetCompressedMultiTexImageEXT *restrict cmd);
struct marshal_cmd_NamedBufferDataEXT;
uint32_t _mesa_unmarshal_NamedBufferDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferDataEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_NamedBufferDataEXT(GLuint buffer, GLsizeiptr size, const GLvoid *data, GLenum usage);
struct marshal_cmd_NamedBufferSubDataEXT;
uint32_t _mesa_unmarshal_NamedBufferSubDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferSubDataEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_NamedBufferSubDataEXT(GLuint buffer, GLintptr offset, GLsizeiptr size, const GLvoid *data);
struct marshal_cmd_FlushMappedNamedBufferRangeEXT;
uint32_t _mesa_unmarshal_FlushMappedNamedBufferRangeEXT(struct gl_context *ctx, const struct marshal_cmd_FlushMappedNamedBufferRangeEXT *restrict cmd);
struct marshal_cmd_FramebufferDrawBufferEXT;
uint32_t _mesa_unmarshal_FramebufferDrawBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBufferEXT *restrict cmd);
struct marshal_cmd_FramebufferDrawBuffersEXT;
uint32_t _mesa_unmarshal_FramebufferDrawBuffersEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferDrawBuffersEXT *restrict cmd);
struct marshal_cmd_FramebufferReadBufferEXT;
uint32_t _mesa_unmarshal_FramebufferReadBufferEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferReadBufferEXT *restrict cmd);
struct marshal_cmd_NamedFramebufferTexture1DEXT;
uint32_t _mesa_unmarshal_NamedFramebufferTexture1DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture1DEXT *restrict cmd);
struct marshal_cmd_NamedFramebufferTexture2DEXT;
uint32_t _mesa_unmarshal_NamedFramebufferTexture2DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture2DEXT *restrict cmd);
struct marshal_cmd_NamedFramebufferTexture3DEXT;
uint32_t _mesa_unmarshal_NamedFramebufferTexture3DEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferTexture3DEXT *restrict cmd);
struct marshal_cmd_NamedFramebufferRenderbufferEXT;
uint32_t _mesa_unmarshal_NamedFramebufferRenderbufferEXT(struct gl_context *ctx, const struct marshal_cmd_NamedFramebufferRenderbufferEXT *restrict cmd);
struct marshal_cmd_NamedRenderbufferStorageEXT;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageEXT *restrict cmd);
struct marshal_cmd_GenerateTextureMipmapEXT;
uint32_t _mesa_unmarshal_GenerateTextureMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateTextureMipmapEXT *restrict cmd);
struct marshal_cmd_GenerateMultiTexMipmapEXT;
uint32_t _mesa_unmarshal_GenerateMultiTexMipmapEXT(struct gl_context *ctx, const struct marshal_cmd_GenerateMultiTexMipmapEXT *restrict cmd);
struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleEXT *restrict cmd);
struct marshal_cmd_NamedCopyBufferSubDataEXT;
uint32_t _mesa_unmarshal_NamedCopyBufferSubDataEXT(struct gl_context *ctx, const struct marshal_cmd_NamedCopyBufferSubDataEXT *restrict cmd);
struct marshal_cmd_VertexArrayVertexOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexOffsetEXT *restrict cmd);
struct marshal_cmd_VertexArrayColorOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayColorOffsetEXT *restrict cmd);
struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayEdgeFlagOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayEdgeFlagOffsetEXT *restrict cmd);
struct marshal_cmd_VertexArrayIndexOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayIndexOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayIndexOffsetEXT *restrict cmd);
struct marshal_cmd_VertexArrayNormalOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayNormalOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayNormalOffsetEXT *restrict cmd);
struct marshal_cmd_VertexArrayTexCoordOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayTexCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayTexCoordOffsetEXT *restrict cmd);
struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayMultiTexCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayMultiTexCoordOffsetEXT *restrict cmd);
struct marshal_cmd_VertexArrayFogCoordOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayFogCoordOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayFogCoordOffsetEXT *restrict cmd);
struct marshal_cmd_VertexArraySecondaryColorOffsetEXT;
uint32_t _mesa_unmarshal_VertexArraySecondaryColorOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArraySecondaryColorOffsetEXT *restrict cmd);
struct marshal_cmd_VertexArrayVertexAttribOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribOffsetEXT *restrict cmd);
struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT;
uint32_t _mesa_unmarshal_VertexArrayVertexAttribIOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_VertexArrayVertexAttribIOffsetEXT *restrict cmd);
struct marshal_cmd_EnableVertexArrayEXT;
uint32_t _mesa_unmarshal_EnableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayEXT *restrict cmd);
struct marshal_cmd_DisableVertexArrayEXT;
uint32_t _mesa_unmarshal_DisableVertexArrayEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayEXT *restrict cmd);
struct marshal_cmd_EnableVertexArrayAttribEXT;
uint32_t _mesa_unmarshal_EnableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_EnableVertexArrayAttribEXT *restrict cmd);
struct marshal_cmd_DisableVertexArrayAttribEXT;
uint32_t _mesa_unmarshal_DisableVertexArrayAttribEXT(struct gl_context *ctx, const struct marshal_cmd_DisableVertexArrayAttribEXT *restrict cmd);
struct marshal_cmd_NamedProgramStringEXT;
uint32_t _mesa_unmarshal_NamedProgramStringEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramStringEXT *restrict cmd);
struct marshal_cmd_NamedProgramLocalParameter4fEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4fEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4fEXT *restrict cmd);
struct marshal_cmd_NamedProgramLocalParameter4fvEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4fvEXT *restrict cmd);
struct marshal_cmd_NamedProgramLocalParameter4dEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4dEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4dEXT *restrict cmd);
struct marshal_cmd_NamedProgramLocalParameter4dvEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameter4dvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameter4dvEXT *restrict cmd);
struct marshal_cmd_TextureBufferEXT;
uint32_t _mesa_unmarshal_TextureBufferEXT(struct gl_context *ctx, const struct marshal_cmd_TextureBufferEXT *restrict cmd);
struct marshal_cmd_MultiTexBufferEXT;
uint32_t _mesa_unmarshal_MultiTexBufferEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexBufferEXT *restrict cmd);
struct marshal_cmd_TextureParameterIivEXT;
uint32_t _mesa_unmarshal_TextureParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIivEXT *restrict cmd);
struct marshal_cmd_TextureParameterIuivEXT;
uint32_t _mesa_unmarshal_TextureParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_TextureParameterIuivEXT *restrict cmd);
struct marshal_cmd_MultiTexParameterIivEXT;
uint32_t _mesa_unmarshal_MultiTexParameterIivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIivEXT *restrict cmd);
struct marshal_cmd_MultiTexParameterIuivEXT;
uint32_t _mesa_unmarshal_MultiTexParameterIuivEXT(struct gl_context *ctx, const struct marshal_cmd_MultiTexParameterIuivEXT *restrict cmd);
struct marshal_cmd_NamedProgramLocalParameters4fvEXT;
uint32_t _mesa_unmarshal_NamedProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_NamedProgramLocalParameters4fvEXT *restrict cmd);
struct marshal_cmd_BindImageTextureEXT;
uint32_t _mesa_unmarshal_BindImageTextureEXT(struct gl_context *ctx, const struct marshal_cmd_BindImageTextureEXT *restrict cmd);
struct marshal_cmd_SubpixelPrecisionBiasNV;
uint32_t _mesa_unmarshal_SubpixelPrecisionBiasNV(struct gl_context *ctx, const struct marshal_cmd_SubpixelPrecisionBiasNV *restrict cmd);
struct marshal_cmd_ConservativeRasterParameterfNV;
uint32_t _mesa_unmarshal_ConservativeRasterParameterfNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameterfNV *restrict cmd);
struct marshal_cmd_ConservativeRasterParameteriNV;
uint32_t _mesa_unmarshal_ConservativeRasterParameteriNV(struct gl_context *ctx, const struct marshal_cmd_ConservativeRasterParameteriNV *restrict cmd);
struct marshal_cmd_DeletePerfQueryINTEL;
uint32_t _mesa_unmarshal_DeletePerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_DeletePerfQueryINTEL *restrict cmd);
struct marshal_cmd_BeginPerfQueryINTEL;
uint32_t _mesa_unmarshal_BeginPerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_BeginPerfQueryINTEL *restrict cmd);
struct marshal_cmd_EndPerfQueryINTEL;
uint32_t _mesa_unmarshal_EndPerfQueryINTEL(struct gl_context *ctx, const struct marshal_cmd_EndPerfQueryINTEL *restrict cmd);
struct marshal_cmd_AlphaToCoverageDitherControlNV;
uint32_t _mesa_unmarshal_AlphaToCoverageDitherControlNV(struct gl_context *ctx, const struct marshal_cmd_AlphaToCoverageDitherControlNV *restrict cmd);
struct marshal_cmd_PolygonOffsetClampEXT;
uint32_t _mesa_unmarshal_PolygonOffsetClampEXT(struct gl_context *ctx, const struct marshal_cmd_PolygonOffsetClampEXT *restrict cmd);
struct marshal_cmd_WindowRectanglesEXT;
uint32_t _mesa_unmarshal_WindowRectanglesEXT(struct gl_context *ctx, const struct marshal_cmd_WindowRectanglesEXT *restrict cmd);
struct marshal_cmd_FramebufferFetchBarrierEXT;
uint32_t _mesa_unmarshal_FramebufferFetchBarrierEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferFetchBarrierEXT *restrict cmd);
struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD;
uint32_t _mesa_unmarshal_RenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_RenderbufferStorageMultisampleAdvancedAMD *restrict cmd);
struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD;
uint32_t _mesa_unmarshal_NamedRenderbufferStorageMultisampleAdvancedAMD(struct gl_context *ctx, const struct marshal_cmd_NamedRenderbufferStorageMultisampleAdvancedAMD *restrict cmd);
struct marshal_cmd_StencilFuncSeparateATI;
uint32_t _mesa_unmarshal_StencilFuncSeparateATI(struct gl_context *ctx, const struct marshal_cmd_StencilFuncSeparateATI *restrict cmd);
struct marshal_cmd_ProgramEnvParameters4fvEXT;
uint32_t _mesa_unmarshal_ProgramEnvParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramEnvParameters4fvEXT *restrict cmd);
struct marshal_cmd_ProgramLocalParameters4fvEXT;
uint32_t _mesa_unmarshal_ProgramLocalParameters4fvEXT(struct gl_context *ctx, const struct marshal_cmd_ProgramLocalParameters4fvEXT *restrict cmd);
struct marshal_cmd_InternalBufferSubDataCopyMESA;
uint32_t _mesa_unmarshal_InternalBufferSubDataCopyMESA(struct gl_context *ctx, const struct marshal_cmd_InternalBufferSubDataCopyMESA *restrict cmd);
void GLAPIENTRY _mesa_marshal_InternalBufferSubDataCopyMESA(GLintptr srcBuffer, GLuint srcOffset, GLuint dstTargetOrName, GLintptr dstOffset, GLsizeiptr size, GLboolean named, GLboolean ext_dsa);
struct marshal_cmd_InternalSetError;
uint32_t _mesa_unmarshal_InternalSetError(struct gl_context *ctx, const struct marshal_cmd_InternalSetError *restrict cmd);
void GLAPIENTRY _mesa_marshal_InternalSetError(GLenum error);
struct marshal_cmd_DrawArraysUserBuf;
uint32_t _mesa_unmarshal_DrawArraysUserBuf(struct gl_context *ctx, const struct marshal_cmd_DrawArraysUserBuf *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawArraysUserBuf(void);
struct marshal_cmd_DrawElementsUserBuf;
uint32_t _mesa_unmarshal_DrawElementsUserBuf(struct gl_context *ctx, const struct marshal_cmd_DrawElementsUserBuf *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsUserBuf(GLintptr indexBuf, GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
struct marshal_cmd_MultiDrawArraysUserBuf;
uint32_t _mesa_unmarshal_MultiDrawArraysUserBuf(struct gl_context *ctx, const struct marshal_cmd_MultiDrawArraysUserBuf *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawArraysUserBuf(void);
struct marshal_cmd_MultiDrawElementsUserBuf;
uint32_t _mesa_unmarshal_MultiDrawElementsUserBuf(struct gl_context *ctx, const struct marshal_cmd_MultiDrawElementsUserBuf *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiDrawElementsUserBuf(GLintptr indexBuf, GLenum mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount, const GLint *basevertex);
struct marshal_cmd_DrawArraysInstancedBaseInstanceDrawID;
uint32_t _mesa_unmarshal_DrawArraysInstancedBaseInstanceDrawID(struct gl_context *ctx, const struct marshal_cmd_DrawArraysInstancedBaseInstanceDrawID *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawArraysInstancedBaseInstanceDrawID(void);
struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstanceDrawID;
uint32_t _mesa_unmarshal_DrawElementsInstancedBaseVertexBaseInstanceDrawID(struct gl_context *ctx, const struct marshal_cmd_DrawElementsInstancedBaseVertexBaseInstanceDrawID *restrict cmd);
void GLAPIENTRY _mesa_marshal_DrawElementsInstancedBaseVertexBaseInstanceDrawID(void);
struct marshal_cmd_InternalInvalidateFramebufferAncillaryMESA;
uint32_t _mesa_unmarshal_InternalInvalidateFramebufferAncillaryMESA(struct gl_context *ctx, const struct marshal_cmd_InternalInvalidateFramebufferAncillaryMESA *restrict cmd);
void GLAPIENTRY _mesa_marshal_InternalInvalidateFramebufferAncillaryMESA(void);
struct marshal_cmd_ClearColorIiEXT;
uint32_t _mesa_unmarshal_ClearColorIiEXT(struct gl_context *ctx, const struct marshal_cmd_ClearColorIiEXT *restrict cmd);
struct marshal_cmd_ClearColorIuiEXT;
uint32_t _mesa_unmarshal_ClearColorIuiEXT(struct gl_context *ctx, const struct marshal_cmd_ClearColorIuiEXT *restrict cmd);
struct marshal_cmd_TexParameterIiv;
uint32_t _mesa_unmarshal_TexParameterIiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIiv *restrict cmd);
struct marshal_cmd_TexParameterIuiv;
uint32_t _mesa_unmarshal_TexParameterIuiv(struct gl_context *ctx, const struct marshal_cmd_TexParameterIuiv *restrict cmd);
struct marshal_cmd_VertexAttribI1iEXT;
uint32_t _mesa_unmarshal_VertexAttribI1iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1iEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI1iEXT(GLuint index, GLint x);
struct marshal_cmd_VertexAttribI2iEXT;
uint32_t _mesa_unmarshal_VertexAttribI2iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2iEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI2iEXT(GLuint index, GLint x, GLint y);
struct marshal_cmd_VertexAttribI3iEXT;
uint32_t _mesa_unmarshal_VertexAttribI3iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3iEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI3iEXT(GLuint index, GLint x, GLint y, GLint z);
struct marshal_cmd_VertexAttribI4iEXT;
uint32_t _mesa_unmarshal_VertexAttribI4iEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4iEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI4iEXT(GLuint index, GLint x, GLint y, GLint z, GLint w);
struct marshal_cmd_VertexAttribI1uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI1uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1uiEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI1uiEXT(GLuint index, GLuint x);
struct marshal_cmd_VertexAttribI2uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI2uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2uiEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI2uiEXT(GLuint index, GLuint x, GLuint y);
struct marshal_cmd_VertexAttribI3uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI3uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3uiEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI3uiEXT(GLuint index, GLuint x, GLuint y, GLuint z);
struct marshal_cmd_VertexAttribI4uiEXT;
uint32_t _mesa_unmarshal_VertexAttribI4uiEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4uiEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI4uiEXT(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
struct marshal_cmd_VertexAttribI1iv;
uint32_t _mesa_unmarshal_VertexAttribI1iv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1iv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI1iv(GLuint index, const GLint *v);
struct marshal_cmd_VertexAttribI2ivEXT;
uint32_t _mesa_unmarshal_VertexAttribI2ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2ivEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI2ivEXT(GLuint index, const GLint *v);
struct marshal_cmd_VertexAttribI3ivEXT;
uint32_t _mesa_unmarshal_VertexAttribI3ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3ivEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI3ivEXT(GLuint index, const GLint *v);
struct marshal_cmd_VertexAttribI4ivEXT;
uint32_t _mesa_unmarshal_VertexAttribI4ivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4ivEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI4ivEXT(GLuint index, const GLint *v);
struct marshal_cmd_VertexAttribI1uiv;
uint32_t _mesa_unmarshal_VertexAttribI1uiv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI1uiv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI1uiv(GLuint index, const GLuint *v);
struct marshal_cmd_VertexAttribI2uivEXT;
uint32_t _mesa_unmarshal_VertexAttribI2uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI2uivEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI2uivEXT(GLuint index, const GLuint *v);
struct marshal_cmd_VertexAttribI3uivEXT;
uint32_t _mesa_unmarshal_VertexAttribI3uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI3uivEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI3uivEXT(GLuint index, const GLuint *v);
struct marshal_cmd_VertexAttribI4uivEXT;
uint32_t _mesa_unmarshal_VertexAttribI4uivEXT(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4uivEXT *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI4uivEXT(GLuint index, const GLuint *v);
struct marshal_cmd_VertexAttribI4bv;
uint32_t _mesa_unmarshal_VertexAttribI4bv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4bv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI4bv(GLuint index, const GLbyte *v);
struct marshal_cmd_VertexAttribI4sv;
uint32_t _mesa_unmarshal_VertexAttribI4sv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4sv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI4sv(GLuint index, const GLshort *v);
struct marshal_cmd_VertexAttribI4ubv;
uint32_t _mesa_unmarshal_VertexAttribI4ubv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4ubv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI4ubv(GLuint index, const GLubyte *v);
struct marshal_cmd_VertexAttribI4usv;
uint32_t _mesa_unmarshal_VertexAttribI4usv(struct gl_context *ctx, const struct marshal_cmd_VertexAttribI4usv *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribI4usv(GLuint index, const GLushort *v);
struct marshal_cmd_VertexAttribIPointer;
uint32_t _mesa_unmarshal_VertexAttribIPointer(struct gl_context *ctx, const struct marshal_cmd_VertexAttribIPointer *restrict cmd);
struct marshal_cmd_Uniform1ui;
uint32_t _mesa_unmarshal_Uniform1ui(struct gl_context *ctx, const struct marshal_cmd_Uniform1ui *restrict cmd);
struct marshal_cmd_Uniform2ui;
uint32_t _mesa_unmarshal_Uniform2ui(struct gl_context *ctx, const struct marshal_cmd_Uniform2ui *restrict cmd);
struct marshal_cmd_Uniform3ui;
uint32_t _mesa_unmarshal_Uniform3ui(struct gl_context *ctx, const struct marshal_cmd_Uniform3ui *restrict cmd);
struct marshal_cmd_Uniform4ui;
uint32_t _mesa_unmarshal_Uniform4ui(struct gl_context *ctx, const struct marshal_cmd_Uniform4ui *restrict cmd);
struct marshal_cmd_Uniform1uiv;
uint32_t _mesa_unmarshal_Uniform1uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform1uiv *restrict cmd);
struct marshal_cmd_Uniform2uiv;
uint32_t _mesa_unmarshal_Uniform2uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform2uiv *restrict cmd);
struct marshal_cmd_Uniform3uiv;
uint32_t _mesa_unmarshal_Uniform3uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform3uiv *restrict cmd);
struct marshal_cmd_Uniform4uiv;
uint32_t _mesa_unmarshal_Uniform4uiv(struct gl_context *ctx, const struct marshal_cmd_Uniform4uiv *restrict cmd);
struct marshal_cmd_BindFragDataLocation;
uint32_t _mesa_unmarshal_BindFragDataLocation(struct gl_context *ctx, const struct marshal_cmd_BindFragDataLocation *restrict cmd);
struct marshal_cmd_ClearBufferiv;
uint32_t _mesa_unmarshal_ClearBufferiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferiv *restrict cmd);
struct marshal_cmd_ClearBufferuiv;
uint32_t _mesa_unmarshal_ClearBufferuiv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferuiv *restrict cmd);
struct marshal_cmd_ClearBufferfv;
uint32_t _mesa_unmarshal_ClearBufferfv(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfv *restrict cmd);
struct marshal_cmd_ClearBufferfi;
uint32_t _mesa_unmarshal_ClearBufferfi(struct gl_context *ctx, const struct marshal_cmd_ClearBufferfi *restrict cmd);
struct marshal_cmd_BeginTransformFeedback;
uint32_t _mesa_unmarshal_BeginTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BeginTransformFeedback *restrict cmd);
struct marshal_cmd_EndTransformFeedback;
uint32_t _mesa_unmarshal_EndTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_EndTransformFeedback *restrict cmd);
struct marshal_cmd_BindBufferRange;
uint32_t _mesa_unmarshal_BindBufferRange(struct gl_context *ctx, const struct marshal_cmd_BindBufferRange *restrict cmd);
struct marshal_cmd_BindBufferBase;
uint32_t _mesa_unmarshal_BindBufferBase(struct gl_context *ctx, const struct marshal_cmd_BindBufferBase *restrict cmd);
struct marshal_cmd_BeginConditionalRender;
uint32_t _mesa_unmarshal_BeginConditionalRender(struct gl_context *ctx, const struct marshal_cmd_BeginConditionalRender *restrict cmd);
struct marshal_cmd_EndConditionalRender;
uint32_t _mesa_unmarshal_EndConditionalRender(struct gl_context *ctx, const struct marshal_cmd_EndConditionalRender *restrict cmd);
struct marshal_cmd_PrimitiveRestartIndex;
uint32_t _mesa_unmarshal_PrimitiveRestartIndex(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartIndex *restrict cmd);
struct marshal_cmd_FramebufferTexture;
uint32_t _mesa_unmarshal_FramebufferTexture(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture *restrict cmd);
struct marshal_cmd_PrimitiveRestartNV;
uint32_t _mesa_unmarshal_PrimitiveRestartNV(struct gl_context *ctx, const struct marshal_cmd_PrimitiveRestartNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_PrimitiveRestartNV(void);
struct marshal_cmd_BindBufferOffsetEXT;
uint32_t _mesa_unmarshal_BindBufferOffsetEXT(struct gl_context *ctx, const struct marshal_cmd_BindBufferOffsetEXT *restrict cmd);
struct marshal_cmd_BindTransformFeedback;
uint32_t _mesa_unmarshal_BindTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_BindTransformFeedback *restrict cmd);
struct marshal_cmd_DeleteTransformFeedbacks;
uint32_t _mesa_unmarshal_DeleteTransformFeedbacks(struct gl_context *ctx, const struct marshal_cmd_DeleteTransformFeedbacks *restrict cmd);
struct marshal_cmd_PauseTransformFeedback;
uint32_t _mesa_unmarshal_PauseTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_PauseTransformFeedback *restrict cmd);
struct marshal_cmd_ResumeTransformFeedback;
uint32_t _mesa_unmarshal_ResumeTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_ResumeTransformFeedback *restrict cmd);
struct marshal_cmd_DrawTransformFeedback;
uint32_t _mesa_unmarshal_DrawTransformFeedback(struct gl_context *ctx, const struct marshal_cmd_DrawTransformFeedback *restrict cmd);
struct marshal_cmd_VDPAUFiniNV;
uint32_t _mesa_unmarshal_VDPAUFiniNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUFiniNV *restrict cmd);
struct marshal_cmd_VDPAUUnregisterSurfaceNV;
uint32_t _mesa_unmarshal_VDPAUUnregisterSurfaceNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUUnregisterSurfaceNV *restrict cmd);
struct marshal_cmd_VDPAUSurfaceAccessNV;
uint32_t _mesa_unmarshal_VDPAUSurfaceAccessNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUSurfaceAccessNV *restrict cmd);
struct marshal_cmd_VDPAUMapSurfacesNV;
uint32_t _mesa_unmarshal_VDPAUMapSurfacesNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUMapSurfacesNV *restrict cmd);
struct marshal_cmd_VDPAUUnmapSurfacesNV;
uint32_t _mesa_unmarshal_VDPAUUnmapSurfacesNV(struct gl_context *ctx, const struct marshal_cmd_VDPAUUnmapSurfacesNV *restrict cmd);
struct marshal_cmd_DeleteMemoryObjectsEXT;
uint32_t _mesa_unmarshal_DeleteMemoryObjectsEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteMemoryObjectsEXT *restrict cmd);
struct marshal_cmd_MemoryObjectParameterivEXT;
uint32_t _mesa_unmarshal_MemoryObjectParameterivEXT(struct gl_context *ctx, const struct marshal_cmd_MemoryObjectParameterivEXT *restrict cmd);
struct marshal_cmd_TexStorageMem2DEXT;
uint32_t _mesa_unmarshal_TexStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DEXT *restrict cmd);
struct marshal_cmd_TexStorageMem2DMultisampleEXT;
uint32_t _mesa_unmarshal_TexStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem2DMultisampleEXT *restrict cmd);
struct marshal_cmd_TexStorageMem3DEXT;
uint32_t _mesa_unmarshal_TexStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DEXT *restrict cmd);
struct marshal_cmd_TexStorageMem3DMultisampleEXT;
uint32_t _mesa_unmarshal_TexStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem3DMultisampleEXT *restrict cmd);
struct marshal_cmd_BufferStorageMemEXT;
uint32_t _mesa_unmarshal_BufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_BufferStorageMemEXT *restrict cmd);
struct marshal_cmd_TextureStorageMem2DEXT;
uint32_t _mesa_unmarshal_TextureStorageMem2DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DEXT *restrict cmd);
struct marshal_cmd_TextureStorageMem2DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorageMem2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem2DMultisampleEXT *restrict cmd);
struct marshal_cmd_TextureStorageMem3DEXT;
uint32_t _mesa_unmarshal_TextureStorageMem3DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DEXT *restrict cmd);
struct marshal_cmd_TextureStorageMem3DMultisampleEXT;
uint32_t _mesa_unmarshal_TextureStorageMem3DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem3DMultisampleEXT *restrict cmd);
struct marshal_cmd_NamedBufferStorageMemEXT;
uint32_t _mesa_unmarshal_NamedBufferStorageMemEXT(struct gl_context *ctx, const struct marshal_cmd_NamedBufferStorageMemEXT *restrict cmd);
struct marshal_cmd_TexStorageMem1DEXT;
uint32_t _mesa_unmarshal_TexStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TexStorageMem1DEXT *restrict cmd);
struct marshal_cmd_TextureStorageMem1DEXT;
uint32_t _mesa_unmarshal_TextureStorageMem1DEXT(struct gl_context *ctx, const struct marshal_cmd_TextureStorageMem1DEXT *restrict cmd);
struct marshal_cmd_DeleteSemaphoresEXT;
uint32_t _mesa_unmarshal_DeleteSemaphoresEXT(struct gl_context *ctx, const struct marshal_cmd_DeleteSemaphoresEXT *restrict cmd);
struct marshal_cmd_SemaphoreParameterui64vEXT;
uint32_t _mesa_unmarshal_SemaphoreParameterui64vEXT(struct gl_context *ctx, const struct marshal_cmd_SemaphoreParameterui64vEXT *restrict cmd);
struct marshal_cmd_WaitSemaphoreEXT;
uint32_t _mesa_unmarshal_WaitSemaphoreEXT(struct gl_context *ctx, const struct marshal_cmd_WaitSemaphoreEXT *restrict cmd);
struct marshal_cmd_ImportMemoryFdEXT;
uint32_t _mesa_unmarshal_ImportMemoryFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportMemoryFdEXT *restrict cmd);
struct marshal_cmd_ImportSemaphoreFdEXT;
uint32_t _mesa_unmarshal_ImportSemaphoreFdEXT(struct gl_context *ctx, const struct marshal_cmd_ImportSemaphoreFdEXT *restrict cmd);
struct marshal_cmd_ViewportSwizzleNV;
uint32_t _mesa_unmarshal_ViewportSwizzleNV(struct gl_context *ctx, const struct marshal_cmd_ViewportSwizzleNV *restrict cmd);
struct marshal_cmd_Vertex2hNV;
uint32_t _mesa_unmarshal_Vertex2hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex2hNV(GLhalfNV x, GLhalfNV y);
struct marshal_cmd_Vertex2hvNV;
uint32_t _mesa_unmarshal_Vertex2hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex2hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex2hvNV(const GLhalfNV *v);
struct marshal_cmd_Vertex3hNV;
uint32_t _mesa_unmarshal_Vertex3hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex3hNV(GLhalfNV x, GLhalfNV y, GLhalfNV z);
struct marshal_cmd_Vertex3hvNV;
uint32_t _mesa_unmarshal_Vertex3hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex3hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex3hvNV(const GLhalfNV *v);
struct marshal_cmd_Vertex4hNV;
uint32_t _mesa_unmarshal_Vertex4hNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex4hNV(GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
struct marshal_cmd_Vertex4hvNV;
uint32_t _mesa_unmarshal_Vertex4hvNV(struct gl_context *ctx, const struct marshal_cmd_Vertex4hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Vertex4hvNV(const GLhalfNV *v);
struct marshal_cmd_Normal3hNV;
uint32_t _mesa_unmarshal_Normal3hNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3hNV(GLhalfNV nx, GLhalfNV ny, GLhalfNV nz);
struct marshal_cmd_Normal3hvNV;
uint32_t _mesa_unmarshal_Normal3hvNV(struct gl_context *ctx, const struct marshal_cmd_Normal3hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Normal3hvNV(const GLhalfNV *v);
struct marshal_cmd_Color3hNV;
uint32_t _mesa_unmarshal_Color3hNV(struct gl_context *ctx, const struct marshal_cmd_Color3hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue);
struct marshal_cmd_Color3hvNV;
uint32_t _mesa_unmarshal_Color3hvNV(struct gl_context *ctx, const struct marshal_cmd_Color3hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color3hvNV(const GLhalfNV *v);
struct marshal_cmd_Color4hNV;
uint32_t _mesa_unmarshal_Color4hNV(struct gl_context *ctx, const struct marshal_cmd_Color4hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha);
struct marshal_cmd_Color4hvNV;
uint32_t _mesa_unmarshal_Color4hvNV(struct gl_context *ctx, const struct marshal_cmd_Color4hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_Color4hvNV(const GLhalfNV *v);
struct marshal_cmd_TexCoord1hNV;
uint32_t _mesa_unmarshal_TexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord1hNV(GLhalfNV s);
struct marshal_cmd_TexCoord1hvNV;
uint32_t _mesa_unmarshal_TexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord1hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord1hvNV(const GLhalfNV *v);
struct marshal_cmd_TexCoord2hNV;
uint32_t _mesa_unmarshal_TexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord2hNV(GLhalfNV s, GLhalfNV t);
struct marshal_cmd_TexCoord2hvNV;
uint32_t _mesa_unmarshal_TexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord2hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord2hvNV(const GLhalfNV *v);
struct marshal_cmd_TexCoord3hNV;
uint32_t _mesa_unmarshal_TexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord3hNV(GLhalfNV s, GLhalfNV t, GLhalfNV r);
struct marshal_cmd_TexCoord3hvNV;
uint32_t _mesa_unmarshal_TexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord3hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord3hvNV(const GLhalfNV *v);
struct marshal_cmd_TexCoord4hNV;
uint32_t _mesa_unmarshal_TexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord4hNV(GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
struct marshal_cmd_TexCoord4hvNV;
uint32_t _mesa_unmarshal_TexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_TexCoord4hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_TexCoord4hvNV(const GLhalfNV *v);
struct marshal_cmd_MultiTexCoord1hNV;
uint32_t _mesa_unmarshal_MultiTexCoord1hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1hNV(GLenum target, GLhalfNV s);
struct marshal_cmd_MultiTexCoord1hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord1hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord1hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord1hvNV(GLenum target, const GLhalfNV *v);
struct marshal_cmd_MultiTexCoord2hNV;
uint32_t _mesa_unmarshal_MultiTexCoord2hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2hNV(GLenum target, GLhalfNV s, GLhalfNV t);
struct marshal_cmd_MultiTexCoord2hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord2hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord2hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord2hvNV(GLenum target, const GLhalfNV *v);
struct marshal_cmd_MultiTexCoord3hNV;
uint32_t _mesa_unmarshal_MultiTexCoord3hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r);
struct marshal_cmd_MultiTexCoord3hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord3hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord3hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord3hvNV(GLenum target, const GLhalfNV *v);
struct marshal_cmd_MultiTexCoord4hNV;
uint32_t _mesa_unmarshal_MultiTexCoord4hNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4hNV(GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q);
struct marshal_cmd_MultiTexCoord4hvNV;
uint32_t _mesa_unmarshal_MultiTexCoord4hvNV(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_MultiTexCoord4hvNV(GLenum target, const GLhalfNV *v);
struct marshal_cmd_VertexAttrib1hNV;
uint32_t _mesa_unmarshal_VertexAttrib1hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1hNV(GLuint index, GLhalfNV x);
struct marshal_cmd_VertexAttrib1hvNV;
uint32_t _mesa_unmarshal_VertexAttrib1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib1hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib1hvNV(GLuint index, const GLhalfNV *v);
struct marshal_cmd_VertexAttrib2hNV;
uint32_t _mesa_unmarshal_VertexAttrib2hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2hNV(GLuint index, GLhalfNV x, GLhalfNV y);
struct marshal_cmd_VertexAttrib2hvNV;
uint32_t _mesa_unmarshal_VertexAttrib2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib2hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib2hvNV(GLuint index, const GLhalfNV *v);
struct marshal_cmd_VertexAttrib3hNV;
uint32_t _mesa_unmarshal_VertexAttrib3hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3hNV(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z);
struct marshal_cmd_VertexAttrib3hvNV;
uint32_t _mesa_unmarshal_VertexAttrib3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib3hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib3hvNV(GLuint index, const GLhalfNV *v);
struct marshal_cmd_VertexAttrib4hNV;
uint32_t _mesa_unmarshal_VertexAttrib4hNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4hNV(GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w);
struct marshal_cmd_VertexAttrib4hvNV;
uint32_t _mesa_unmarshal_VertexAttrib4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttrib4hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttrib4hvNV(GLuint index, const GLhalfNV *v);
struct marshal_cmd_VertexAttribs1hvNV;
uint32_t _mesa_unmarshal_VertexAttribs1hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs1hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs1hvNV(GLuint index, GLsizei n, const GLhalfNV *v);
struct marshal_cmd_VertexAttribs2hvNV;
uint32_t _mesa_unmarshal_VertexAttribs2hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs2hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs2hvNV(GLuint index, GLsizei n, const GLhalfNV *v);
struct marshal_cmd_VertexAttribs3hvNV;
uint32_t _mesa_unmarshal_VertexAttribs3hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs3hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs3hvNV(GLuint index, GLsizei n, const GLhalfNV *v);
struct marshal_cmd_VertexAttribs4hvNV;
uint32_t _mesa_unmarshal_VertexAttribs4hvNV(struct gl_context *ctx, const struct marshal_cmd_VertexAttribs4hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_VertexAttribs4hvNV(GLuint index, GLsizei n, const GLhalfNV *v);
struct marshal_cmd_FogCoordhNV;
uint32_t _mesa_unmarshal_FogCoordhNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_FogCoordhNV(GLhalfNV x);
struct marshal_cmd_FogCoordhvNV;
uint32_t _mesa_unmarshal_FogCoordhvNV(struct gl_context *ctx, const struct marshal_cmd_FogCoordhvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_FogCoordhvNV(const GLhalfNV *v);
struct marshal_cmd_SecondaryColor3hNV;
uint32_t _mesa_unmarshal_SecondaryColor3hNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3hNV(GLhalfNV red, GLhalfNV green, GLhalfNV blue);
struct marshal_cmd_SecondaryColor3hvNV;
uint32_t _mesa_unmarshal_SecondaryColor3hvNV(struct gl_context *ctx, const struct marshal_cmd_SecondaryColor3hvNV *restrict cmd);
void GLAPIENTRY _mesa_marshal_SecondaryColor3hvNV(const GLhalfNV *v);
struct marshal_cmd_MemoryBarrierByRegion;
uint32_t _mesa_unmarshal_MemoryBarrierByRegion(struct gl_context *ctx, const struct marshal_cmd_MemoryBarrierByRegion *restrict cmd);
struct marshal_cmd_AlphaFuncx;
uint32_t _mesa_unmarshal_AlphaFuncx(struct gl_context *ctx, const struct marshal_cmd_AlphaFuncx *restrict cmd);
struct marshal_cmd_ClearColorx;
uint32_t _mesa_unmarshal_ClearColorx(struct gl_context *ctx, const struct marshal_cmd_ClearColorx *restrict cmd);
struct marshal_cmd_ClearDepthx;
uint32_t _mesa_unmarshal_ClearDepthx(struct gl_context *ctx, const struct marshal_cmd_ClearDepthx *restrict cmd);
struct marshal_cmd_Color4x;
uint32_t _mesa_unmarshal_Color4x(struct gl_context *ctx, const struct marshal_cmd_Color4x *restrict cmd);
struct marshal_cmd_DepthRangex;
uint32_t _mesa_unmarshal_DepthRangex(struct gl_context *ctx, const struct marshal_cmd_DepthRangex *restrict cmd);
struct marshal_cmd_Fogx;
uint32_t _mesa_unmarshal_Fogx(struct gl_context *ctx, const struct marshal_cmd_Fogx *restrict cmd);
struct marshal_cmd_Fogxv;
uint32_t _mesa_unmarshal_Fogxv(struct gl_context *ctx, const struct marshal_cmd_Fogxv *restrict cmd);
struct marshal_cmd_Frustumx;
uint32_t _mesa_unmarshal_Frustumx(struct gl_context *ctx, const struct marshal_cmd_Frustumx *restrict cmd);
struct marshal_cmd_LightModelx;
uint32_t _mesa_unmarshal_LightModelx(struct gl_context *ctx, const struct marshal_cmd_LightModelx *restrict cmd);
struct marshal_cmd_LightModelxv;
uint32_t _mesa_unmarshal_LightModelxv(struct gl_context *ctx, const struct marshal_cmd_LightModelxv *restrict cmd);
struct marshal_cmd_Lightx;
uint32_t _mesa_unmarshal_Lightx(struct gl_context *ctx, const struct marshal_cmd_Lightx *restrict cmd);
struct marshal_cmd_Lightxv;
uint32_t _mesa_unmarshal_Lightxv(struct gl_context *ctx, const struct marshal_cmd_Lightxv *restrict cmd);
struct marshal_cmd_LineWidthx;
uint32_t _mesa_unmarshal_LineWidthx(struct gl_context *ctx, const struct marshal_cmd_LineWidthx *restrict cmd);
struct marshal_cmd_LoadMatrixx;
uint32_t _mesa_unmarshal_LoadMatrixx(struct gl_context *ctx, const struct marshal_cmd_LoadMatrixx *restrict cmd);
struct marshal_cmd_Materialx;
uint32_t _mesa_unmarshal_Materialx(struct gl_context *ctx, const struct marshal_cmd_Materialx *restrict cmd);
struct marshal_cmd_Materialxv;
uint32_t _mesa_unmarshal_Materialxv(struct gl_context *ctx, const struct marshal_cmd_Materialxv *restrict cmd);
struct marshal_cmd_MultMatrixx;
uint32_t _mesa_unmarshal_MultMatrixx(struct gl_context *ctx, const struct marshal_cmd_MultMatrixx *restrict cmd);
struct marshal_cmd_MultiTexCoord4x;
uint32_t _mesa_unmarshal_MultiTexCoord4x(struct gl_context *ctx, const struct marshal_cmd_MultiTexCoord4x *restrict cmd);
struct marshal_cmd_Normal3x;
uint32_t _mesa_unmarshal_Normal3x(struct gl_context *ctx, const struct marshal_cmd_Normal3x *restrict cmd);
struct marshal_cmd_Orthox;
uint32_t _mesa_unmarshal_Orthox(struct gl_context *ctx, const struct marshal_cmd_Orthox *restrict cmd);
struct marshal_cmd_PointSizex;
uint32_t _mesa_unmarshal_PointSizex(struct gl_context *ctx, const struct marshal_cmd_PointSizex *restrict cmd);
struct marshal_cmd_PolygonOffsetx;
uint32_t _mesa_unmarshal_PolygonOffsetx(struct gl_context *ctx, const struct marshal_cmd_PolygonOffsetx *restrict cmd);
struct marshal_cmd_Rotatex;
uint32_t _mesa_unmarshal_Rotatex(struct gl_context *ctx, const struct marshal_cmd_Rotatex *restrict cmd);
struct marshal_cmd_SampleCoveragex;
uint32_t _mesa_unmarshal_SampleCoveragex(struct gl_context *ctx, const struct marshal_cmd_SampleCoveragex *restrict cmd);
struct marshal_cmd_Scalex;
uint32_t _mesa_unmarshal_Scalex(struct gl_context *ctx, const struct marshal_cmd_Scalex *restrict cmd);
struct marshal_cmd_TexEnvx;
uint32_t _mesa_unmarshal_TexEnvx(struct gl_context *ctx, const struct marshal_cmd_TexEnvx *restrict cmd);
struct marshal_cmd_TexEnvxv;
uint32_t _mesa_unmarshal_TexEnvxv(struct gl_context *ctx, const struct marshal_cmd_TexEnvxv *restrict cmd);
struct marshal_cmd_TexParameterx;
uint32_t _mesa_unmarshal_TexParameterx(struct gl_context *ctx, const struct marshal_cmd_TexParameterx *restrict cmd);
struct marshal_cmd_Translatex;
uint32_t _mesa_unmarshal_Translatex(struct gl_context *ctx, const struct marshal_cmd_Translatex *restrict cmd);
struct marshal_cmd_ClipPlanex;
uint32_t _mesa_unmarshal_ClipPlanex(struct gl_context *ctx, const struct marshal_cmd_ClipPlanex *restrict cmd);
struct marshal_cmd_PointParameterx;
uint32_t _mesa_unmarshal_PointParameterx(struct gl_context *ctx, const struct marshal_cmd_PointParameterx *restrict cmd);
struct marshal_cmd_PointParameterxv;
uint32_t _mesa_unmarshal_PointParameterxv(struct gl_context *ctx, const struct marshal_cmd_PointParameterxv *restrict cmd);
struct marshal_cmd_TexParameterxv;
uint32_t _mesa_unmarshal_TexParameterxv(struct gl_context *ctx, const struct marshal_cmd_TexParameterxv *restrict cmd);
struct marshal_cmd_TexGenxOES;
uint32_t _mesa_unmarshal_TexGenxOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxOES *restrict cmd);
struct marshal_cmd_TexGenxvOES;
uint32_t _mesa_unmarshal_TexGenxvOES(struct gl_context *ctx, const struct marshal_cmd_TexGenxvOES *restrict cmd);
struct marshal_cmd_ClipPlanef;
uint32_t _mesa_unmarshal_ClipPlanef(struct gl_context *ctx, const struct marshal_cmd_ClipPlanef *restrict cmd);
struct marshal_cmd_Frustumf;
uint32_t _mesa_unmarshal_Frustumf(struct gl_context *ctx, const struct marshal_cmd_Frustumf *restrict cmd);
struct marshal_cmd_Orthof;
uint32_t _mesa_unmarshal_Orthof(struct gl_context *ctx, const struct marshal_cmd_Orthof *restrict cmd);
struct marshal_cmd_DrawTexiOES;
uint32_t _mesa_unmarshal_DrawTexiOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexiOES *restrict cmd);
struct marshal_cmd_DrawTexivOES;
uint32_t _mesa_unmarshal_DrawTexivOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexivOES *restrict cmd);
struct marshal_cmd_DrawTexfOES;
uint32_t _mesa_unmarshal_DrawTexfOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfOES *restrict cmd);
struct marshal_cmd_DrawTexfvOES;
uint32_t _mesa_unmarshal_DrawTexfvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexfvOES *restrict cmd);
struct marshal_cmd_DrawTexsOES;
uint32_t _mesa_unmarshal_DrawTexsOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsOES *restrict cmd);
struct marshal_cmd_DrawTexsvOES;
uint32_t _mesa_unmarshal_DrawTexsvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexsvOES *restrict cmd);
struct marshal_cmd_DrawTexxOES;
uint32_t _mesa_unmarshal_DrawTexxOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxOES *restrict cmd);
struct marshal_cmd_DrawTexxvOES;
uint32_t _mesa_unmarshal_DrawTexxvOES(struct gl_context *ctx, const struct marshal_cmd_DrawTexxvOES *restrict cmd);
struct marshal_cmd_PointSizePointerOES;
uint32_t _mesa_unmarshal_PointSizePointerOES(struct gl_context *ctx, const struct marshal_cmd_PointSizePointerOES *restrict cmd);
struct marshal_cmd_DiscardFramebufferEXT;
uint32_t _mesa_unmarshal_DiscardFramebufferEXT(struct gl_context *ctx, const struct marshal_cmd_DiscardFramebufferEXT *restrict cmd);
struct marshal_cmd_FramebufferTexture2DMultisampleEXT;
uint32_t _mesa_unmarshal_FramebufferTexture2DMultisampleEXT(struct gl_context *ctx, const struct marshal_cmd_FramebufferTexture2DMultisampleEXT *restrict cmd);
struct marshal_cmd_DepthRangeArrayfvOES;
uint32_t _mesa_unmarshal_DepthRangeArrayfvOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeArrayfvOES *restrict cmd);
struct marshal_cmd_DepthRangeIndexedfOES;
uint32_t _mesa_unmarshal_DepthRangeIndexedfOES(struct gl_context *ctx, const struct marshal_cmd_DepthRangeIndexedfOES *restrict cmd);
struct marshal_cmd_FramebufferParameteriMESA;
uint32_t _mesa_unmarshal_FramebufferParameteriMESA(struct gl_context *ctx, const struct marshal_cmd_FramebufferParameteriMESA *restrict cmd);

#endif /* MARSHAL_GENERATED_H */

